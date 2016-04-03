<?php
//This page contains functions which generate the XML data for the chart.
//Effectively, we've separated this part from each PHP page to simulate a
//3-tier architecture. In real world, you could replace this by a COM 
//Component or similar technologies which would connect to the database and 
//return data (in XML or normally, which could later be converted to XML).

//getSalesByYear function returns the XML for yearly sales figures (including
//quantity)
function getSalesByYear() {
    // Function to connect to the DB
    $link = connectToDB();
	
    //Initialize <categories> element
    $strCat = "<categories>";

    //Initialize datasets
    $strAmtDS = "<dataset seriesname='Revenue'>";
    $strQtyDS = "<dataset seriesName='Units Sold' parentYAxis='S'>";

    $strSQL = "SELECT Year(o.OrderDate) As SalesYear, ROUND(SUM(d.Quantity*p.UnitPrice),0) As Total, SUM(d.Quantity) as Quantity FROM FC_OrderDetails as d,FC_Orders as o,FC_Products as p WHERE o.OrderID=d.OrderID and d.ProductID=p.ProductID GROUP BY Year(o.OrderDate) ORDER BY Year(o.OrderDate)";
    $result = mysql_query($strSQL) or die(mysql_error());
	
    if ($result) {
        while($ors = mysql_fetch_array($result)) {
            $strCat .= "<category label='" . $ors['SalesYear'] . "'/>";

            //Generate the link
            $strLink = urlencode("javaScript:updateCharts(" . $ors['SalesYear'] . ");");
            $strAmtDS .= "<set value='" . $ors['Total'] . "' link='" . $strLink . "'/>";
            $strQtyDS .= "<set value='" . $ors['Quantity'] . "'/>";
        }
    }
    mysql_close($link);

	//Closing elements
	$strCat .= "</categories>";
	$strAmtDS .= "</dataset>";
	$strQtyDS .= "</dataset>";
	//Entire XML - concatenation
	$strXML = $strCat . $strAmtDS . $strQtyDS;

	return $strXML;
}

//getCumulativeSalesByCatXML returns the cumulative sales for each category
//in a given year
function getCumulativeSalesByCatXML($intYear, $forDataURL) {

	
    // Function to connect to the DB
    $link = connectToDB();

    //To store categories - also flag to check whether category is
	//already generated
    $catXMLDone = false;
	
	//Initialize XML elements
	$strCat = "<categories>";
	
	//First we need to get unique categories in the database
	$strSQL = "Select CategoryID,CategoryName from FC_Categories GROUP BY CategoryID,CategoryName";
    $result = mysql_query($strSQL) or die(mysql_error());
		
    //To store datasets and sets
    $strDataXML = "";

    if ($result) {
        while($orsCat = mysql_fetch_array($result)) {
            //Add this category as dataset
            $strDataXML .= "<dataset seriesName='" . escapeXML($orsCat['CategoryName'],$forDataURL) . "'>";
            //Now, we need to get monthly sales data for products in this category
            $strSQL = "SELECT  Month(o.OrderDate) as MonthNum, g.CategoryID, g.CategoryName, ROUND(SUM(d.Quantity),0) as Quantity, SUM(d.Quantity*p.UnitPrice) As Total FROM FC_Categories as g,  FC_Products as p, FC_Orders as o, FC_OrderDetails as d  WHERE year(o.OrderDate)=" . $intYear ." and g.CategoryID=" . $orsCat['CategoryID'] . " and d.ProductID=p.ProductId and g.CategoryID= p.CategoryID and o.OrderID= d.OrderID GROUP BY g.CategoryID,g.CategoryName,Month(o.OrderDate)";
            //Execute it
            $result2 = mysql_query($strSQL) or die(mysql_error());
            while($ors = mysql_fetch_array($result2)) {
                //Append <category label=''> if not already done
                if (!$catXMLDone) {
                    $strCat .= "<category label='" . MonthName($ors['MonthNum'],true) . "' />";
                }
                //Generate the link
                $strLink = urlencode("javaScript:updateProductChart(" . $intYear . "," . $ors['MonthNum'] . "," . $ors['CategoryID'] . ");");
                //Append data
                $strDataXML .= "<set value='" . $ors['Total'] . "' link='" . $strLink . "'/>";
            }
            //Update flag that we've appended categories		
            $catXMLDone = true;
            //Clear up objects
            mysql_free_result($result2);
            //Close dataset element
            $strDataXML .= "</dataset>";
        }
    }
    mysql_close($link);

    //Close </categories>
	$strCat .= "</categories>";
	//Create full XML
	$strXML = $strCat . $strDataXML;

    //Return
	return $strXML;
}


//getSalesByProdXML returns the sales for the products within a category
//for a given year and month 
function getSalesByProdXML($intYear, $intMonth, $intCatId, $forDataURL) {
	$strXML="";
    // Function to connect to the DB
    $link = connectToDB();

	//Initialize <categories> element
	$strCat = "<categories>";
	
	//Initialize datasets
	$strAmtDS = "<dataset seriesname='Revenue'>";
	$strQtyDS = "<dataset seriesName='Units Sold' parentYAxis='S'>";
	
	//First we need to get unique categories in the database
	$strSQL = "SELECT  g.CategoryName,p.ProductName,ROUND(SUM(d.Quantity),0) as Quantity, ROUND(SUM(d.Quantity*p.UnitPrice),0) As Total FROM FC_Categories as g,  FC_Products as p, FC_Orders as o, FC_OrderDetails as d WHERE year(o.OrderDate)=" . $intYear . " and month(o.OrderDate)=" . $intMonth . " and g.CategoryID=" . $intCatId . " and d.ProductID= p.ProductID and g.CategoryID= p.CategoryID and o.OrderID= d.OrderID GROUP BY g.CategoryName,p.ProductName ";
    $result = mysql_query($strSQL) or die(mysql_error());

    if ($result) {
        while($ors = mysql_fetch_array($result)) {
            $strCat .= "<category label='" . escapeXML($ors['ProductName'],$forDataURL) . "'/>";
            $strAmtDS .= "<set value='" . $ors['Total'] . "' />";
            $strQtyDS .= "<set value='" . $ors['Quantity'] . "'/>";
        }
    }
    mysql_close($link);

	//Closing elements
	$strCat .= "</categories>";
	$strAmtDS .= "</dataset>";
	$strQtyDS .= "</dataset>";
	//Entire XML - concatenation
	$strXML = $strCat . $strAmtDS . $strQtyDS;
	
	return $strXML;
} 

//getAvgShipTimeXML function returns the delay in average shipping time required
//to ship an item.
//$intYear - Year for which we calculate average shipping time
//$numCountries - For how many countries. If -1, then all countries
//$addJSLinks - Whether to add JavaScript links
//$forDataURL - Whether XML Data to be generated for dataURL method or dataXML method
//Returns - Single Series XML Data
function getAvgShipTimeXML($intYear, $numCountries, $addJSLinks, $forDataURL) {

	$strXML="";
    // Function to connect to the DB
    $link = connectToDB();
		
	//Retrieve the shipping info	
	if ($numCountries==-1)
		$strSQL = "SELECT c.Country as Country, ROUND(AVG(DAY(o.ShippedDate)-DAY(o.RequiredDate)),0) As Average FROM FC_Customers as c, FC_Orders as o WHERE YEAR(o.OrderDate)=" . $intYear . " and c.CustomerID=o.CustomerID GROUP BY c.Country ORDER BY Average DESC";
	else
        $strSQL = "SELECT c.Country as Country, CInt(AVG(DAY(o.ShippedDate)-DAY(o.RequiredDate))) As Average FROM FC_Customers as c, FC_Orders as o WHERE YEAR(o.OrderDate)=" . $intYear . " and c.CustomerID=o.CustomerID GROUP BY c.Country ORDER BY Average DESC LIMIT " . $numCountries;
 
    $result = mysql_query($strSQL) or die(mysql_error());

    if ($result) {
        while($ors = mysql_fetch_array($result)) {
            //Append the data
            //If JavaScript links are to be added
            if ($addJSLinks) {
                //Generate the link
                //TRICKY: We're having to escape the " character using chr(34) character.
                //In HTML, the data is provided as chart.setXMLData(" - so " is already used and un-terminated
                //For each XML attribute, we use '. So ' is used in <set link='
                //Now, we've to pass Country Name to JavaScript function, so we've to use chr(34)
                $strLink = urlencode("javaScript:updateChart(" . $intYear . "," . chr(34) . $ors['Country'] .  chr(34) . ");");
                $strXML .= "<set label='" . escapeXML($ors['Country'], $forDataURL) . "' value='" . $ors['Average'] . "' link='" . $strLink . "'/>";
            } else
                $strXML .= "<set label='" . escapeXML($ors['Country'], $forDataURL) . "' value='" . $ors['Average'] . "' />";
        }
    }
    mysql_close($link);

    return $strXML;
}

//getAvgShipTimeCityXML function returns the average shipping time required
//to ship an item for the cities within the given country
//$intYear - Year for which we calculate average shipping time
//$country - Cities of which country?
//Returns - Single Series XML Data
function getAvgShipTimeCityXML($intYear, $country, $forDataURL) {

	$strXML="";
    // Function to connect to the DB
    $link = connectToDB();
		
	//Retrieve the shipping info by city
	$strSQL = "Select ShipCity, ROUND(AVG(DAY(ShippedDate)-DAY(RequiredDate)),0) As Average from FC_Orders WHERE YEAR(OrderDate)=" . $intYear . " and ShipCountry='" . $country . "' GROUP BY ShipCity ORDER BY Average DESC ";
    $result = mysql_query($strSQL) or die(mysql_error());

    //Create the XML data document containing only data
    //We add the <chart> element in the calling function, depending on needs.	
    $strXML = "";
    if ($result) {
        while($ors = mysql_fetch_array($result)) {
            //Append the data
            $strXML .= "<set label='" . escapeXML($ors['ShipCity'],$forDataURL) . "' value='" . $ors['Average'] . "' />";
        }
    }
    mysql_close($link);

    return $strXML;
}

//getTopCustomersXML returns the XML data for top customers for
//the given year.
function getTopCustomersXML($intYear, $howMany, $forDataURL) {
    // Function to connect to the DB
    $link = connectToDB();
	
	$strSQL = "SELECT c.CompanyName as CustomerName, SUM(d.Quantity*p.UnitPrice) As Total, SUM(d.Quantity) As Quantity FROM FC_Customers as c, FC_OrderDetails as d, FC_Orders as o, FC_Products as p WHERE YEAR(OrderDate)=" . $intYear . " and c.CustomerID=o.CustomerID and o.OrderID=d.OrderID and d.ProductID=p.ProductID GROUP BY c.CompanyName ORDER BY Total DESC LIMIT ". $howMany;
    $result = mysql_query($strSQL) or die(mysql_error());

	//Initialize <categories> element
	$strCat = "<categories>";
	
	//Initialize datasets
	$strAmtDS = "<dataset seriesname='Amount'>";
	$strQtyDS = "<dataset seriesName='Quantity' parentYAxis='S'>";
	
	//Iterate through each data row
    if ($result) {
        while($ors = mysql_fetch_array($result)) {
            $strCat .= "<category label='" . escapeXML($ors['CustomerName'],$forDataURL) . "'/>";
            $strAmtDS .= "<set value='" . $ors['Total'] . "'/>";
            $strQtyDS .= "<set value='" . $ors['Quantity'] . "'/>";
        }
    }
    mysql_close($link);

    //Closing elements
	$strCat .= "</categories>";
	$strAmtDS .= "</dataset>";
	$strQtyDS .= "</dataset>";
	//Entire XML - concatenation
	$strXML = $strCat . $strAmtDS . $strQtyDS;
		
	return $strXML;
}

//getCustByCountry function returns number of customers present
//in each country in the database.
function getCustByCountry($forDataURL) {

	$strXML="";
	
    // Function to connect to the DB
    $link = connectToDB();

    $counter = 0;

	//Retrieve the data
	$strSQL = "SELECT count(CustomerID) AS Num, Country FROM FC_Customers GROUP BY Country ORDER BY Num DESC;";
    $result = mysql_query($strSQL) or die(mysql_error());
	
	//Create the XML data document containing only data
	//We add the <chart> element in the calling function, depending on needs.	
    if ($result) {
        while($ors = mysql_fetch_array($result)) {
            //Increase counter
            $counter++;
            //Append the data
            //We slice the first pie (the country having highest number of customers)		
            if ($counter==1)
                $strXML .= "<set label='" . escapeXML($ors['Country'],$forDataURL) . "' value='" . $ors['Num'] . "' isSliced='1'/>";
            else
                $strXML .= "<set label='" . escapeXML($ors['Country'],$forDataURL) . "' value='" . $ors['Num'] . "' />";
        }
    }
    mysql_close($link);

    return $strXML;
}

//getSalePerEmp function returns the XML data for sales generated by
//each employee for the given year
function getSalePerEmpXML($intYear, $howMany, $slicePies, $addJSLinks, $forDataURL) {

	$strXML ="";
    // Function to connect to the DB
    $link = connectToDB();

    $count = 0;

    //Retrieve the data
    $strSQL = "SELECT e.LastName, e.EmployeeID, SUM(d.Quantity*p.UnitPrice) As Total FROM FC_Employees as e,FC_Orders as o, FC_OrderDetails as d, FC_Products as p WHERE YEAR(OrderDate)=" . $intYear . " and e.EmployeeID=o.EmployeeID and o.OrderID=d.OrderID and d.ProductID=p.ProductID GROUP BY e.LastName,e.EmployeeID ORDER BY Total DESC";
	if ($howMany!=-1)
		$strSQL .= " LIMIT " . $howMany;

    $result = mysql_query($strSQL) or die(mysql_error());

    //Create the XML data document containing only data
	//We add the <chart> element in the calling function, depending on needs.	
    if ($result) {
        while($ors = mysql_fetch_array($result)) {
            //Append the data
            $count++;

            //If link is to be added
            if ($addJSLinks)
                $strLink = " link='javascript:updateChart(" . $ors['EmployeeID'] . ");' ";
            else
                $strLink = "";

            //If top 2 employees, then sliced out				
            if ($slicePies && ($count<3))
                $slicedOut="1";
            else
                $slicedOut="0";

            $strXML .= "<set label='" . escapeXML($ors['LastName'],$forDataURL) . "' value='" . $ors['Total'] . "' isSliced='" . $slicedOut . "' " . $strLink . " />";
        }
    }
    mysql_close($link);

    return $strXML;
}

//getSalesByCountryXML function returns the XML Data for sales
//for a given country in a given year.
function getSalesByCountryXML($intYear, $howMany, $addJSLinks, $forDataURL) {
    // Function to connect to the DB
    $link = connectToDB();

    $strSQL = "SELECT c.Country, ROUND(SUM(d.Quantity*p.UnitPrice*(1-d.Discount)),0) As Total, SUM(d.Quantity) as Quantity FROM FC_Customers as c, FC_Products as p, FC_Orders as o, FC_OrderDetails as d WHERE YEAR(OrderDate)=" . $intYear . " and d.ProductID=p.ProductID and c.CustomerID=o.CustomerID and o.OrderID=d.OrderID GROUP BY c.Country ORDER BY Total DESC";
	if ($howMany!=-1)
		$strSQL .= " LIMIT " . $howMany;
	
    $result = mysql_query($strSQL) or die(mysql_error());

	//Initialize <categories> element
	$strCat = "<categories>";
	
	//Initialize datasets
	$strAmtDS = "<dataset seriesname='Amount'>";
	$strQtyDS = "<dataset seriesName='Quantity' parentYAxis='S'>";
		
	//Iterate through each data row
    if ($result) {
        while($ors = mysql_fetch_array($result)) {
            $strCat .= "<category label='" . escapeXML($ors['Country'],$forDataURL) . "'/>";
		
            //If JavaScript links are to be added
            if ($addJSLinks) {
                //Generate the link
                //TRICKY: We're having to escape the " character using chr(34) character.
                //In HTML, the data is provided as chart.setXMLData(" - so " is already used and un-terminated
                //For each XML attribute, we use '. So ' is used in <set link='
                //Now, we//ve to pass Country Name to JavaScript function, so we've to use chr(34)
                $strLink = urlencode("javaScript:updateChart(" . $intYear . "," . chr(34) . $ors['Country'] .  chr(34) . ");");
                $strAmtDS .= "<set value='" . $ors['Total'] . "' link='" . $strLink . "'/>";
            }
            else
                $strAmtDS .= "<set value='" . $ors['Total'] . "' />";

            $strQtyDS .= "<set value='" . $ors['Quantity'] . "'/>";
        }
    }
    mysql_close($link);

    //Closing elements
	$strCat .= "</categories>";
	$strAmtDS .= "</dataset>";
	$strQtyDS .= "</dataset>";
	//Entire XML - concatenation
	$strXML = $strCat . $strAmtDS . $strQtyDS;
	
	return $strXML;
}

//getSalesByCountryCityXML function generates the XML data for sales
//by city within the given country, for the given year.
function getSalesByCountryCityXML($intYear, $country, $forDataURL) {
    // Function to connect to the DB
    $link = connectToDB();
	
	$strSQL = "SELECT  c.City, ROUND(SUM(d.Quantity*p.UnitPrice*(1-d.Discount)),0) As Total, SUM(d.Quantity) as Quantity  FROM FC_Customers as c, FC_Products as p, FC_Orders as o, FC_OrderDetails as d WHERE YEAR(OrderDate)=" . $intYear . " and d.ProductID=p.ProductID and c.CustomerID=o.CustomerID and o.OrderID=d.OrderID and c.Country='" . $country . "' GROUP BY c.City ORDER BY Total DESC";
    $result = mysql_query($strSQL) or die(mysql_error());

	//Initialize <categories> element
	$strCat = "<categories>";
	
	//Initialize datasets
	$strAmtDS = "<dataset seriesname='Amount'>";
	$strQtyDS = "<dataset seriesName='Quantity' parentYAxis='S'>";
    if ($result) {
        while($ors = mysql_fetch_array($result)) {
            $strCat .= "<category label='" . escapeXML($ors['City'],$forDataURL) . "'/>";

            $strAmtDS .= "<set value='" . $ors['Total'] . "' />";
            $strQtyDS .= "<set value='" . $ors['Quantity'] . "'/>";
        }
    }
    mysql_close($link);
	
    //Closing elements
	$strCat .= "</categories>";
	$strAmtDS .= "</dataset>";
	$strQtyDS .= "</dataset>";
	//Entire XML - concatenation
	$strXML = $strCat . $strAmtDS . $strQtyDS;
	
	return $strXML;
}

//getSalesByCountryCustomerXML function generates the XML data for sales
//by customers within the given country, for the given year.
function getSalesByCountryCustomerXML($intYear, $country, $forDataURL) {
    // Function to connect to the DB
    $link = connectToDB();
	
	$strSQL = "SELECT c.CompanyName as CustomerName, SUM(d.Quantity*p.UnitPrice) As Total, SUM(d.Quantity) As Quantity FROM FC_Customers as c, FC_OrderDetails as d, FC_Orders as o, FC_Products as p WHERE YEAR(OrderDate)=" . $intYear . " and c.CustomerID=o.CustomerID and o.OrderID=d.OrderID and d.ProductID=p.ProductID and c.Country='" . $country . "' GROUP BY c.CompanyName ORDER BY Total DESC";
    $result = mysql_query($strSQL) or die(mysql_error());
		
	//Initialize <categories> element
	$strCat = "<categories>";
	
	//Initialize datasets
	$strAmtDS = "<dataset seriesname='Amount'>";
	$strQtyDS = "<dataset seriesName='Quantity' parentYAxis='S'>";
	
	//Iterate through each data row
    if ($result) {
        while($ors = mysql_fetch_array($result)) {
            //Since customers name are long, we truncate them to 5 characters and then show ellipse
            //The full name is then shown as toolText
            $strCat .= "<category label='" . escapeXML(substr($ors['CustomerName'],0,5) . "...", $forDataURL) . "' toolText='" . escapeXML($ors['CustomerName'],$forDataURL) . "'/>";
            $strAmtDS .= "<set value='" . $ors['Total'] . "' />";
            $strQtyDS .= "<set value='" . $ors['Quantity'] . "'/>";
        }
    }
    mysql_close($link);

    //Closing elements
	$strCat .= "</categories>";
	$strAmtDS .= "</dataset>";
	$strQtyDS .= "</dataset>";
	//Entire XML - concatenation
	$strXML = $strCat . $strAmtDS . $strQtyDS;
	
	return $strXML;
}

//getExpensiveProdXML method returns the 10 most expensive products
//in the database along with the sales quantity of those products
//for the given year
function getExpensiveProdXML($intYear, $howMany, $forDataURL) {
    // Function to connect to the DB
    $link = connectToDB();
	
	$strSQL = "SELECT p.ProductName, p.UnitPrice, SUM(d.Quantity) as Quantity FROM FC_Products p, FC_Orders as o, FC_OrderDetails d WHERE YEAR(OrderDate)=" . $intYear . " and d.ProductID=p.ProductID and o.OrderID=d.OrderID GROUP BY p.ProductName,p.UnitPrice  ORDER BY p.UnitPrice DESC LIMIT " . $howMany ;
    $result = mysql_query($strSQL) or die(mysql_error());

	//Initialize <categories> element
	$strCat = "<categories>";
	
	//Initialize datasets
	$strAmtDS = "<dataset seriesname='Unit Price'>";
	$strQtyDS = "<dataset seriesName='Quantity' parentYAxis='S'>";
	
	
    //Iterate through each data row
    if ($result) {
        while($ors = mysql_fetch_array($result)) {
            $strCat .= "<category label='" . escapeXML($ors['ProductName'],$forDataURL) . "'/>";

            $strAmtDS .= "<set value='" . $ors['UnitPrice'] . "' />";
            $strQtyDS .= "<set value='" . $ors['Quantity'] . "'/>";
        }
    }
    mysql_close($link);

    //Closing elements
	$strCat .= "</categories>";
	$strAmtDS .= "</dataset>";
	$strQtyDS .= "</dataset>";
	//Entire XML - concatenation
	$strXML = $strCat . $strAmtDS . $strQtyDS;
	
	return $strXML;
}

//getInventoryByCatXML function returns the inventory of all items
//and their respective quantity
function getInventoryByCatXML($addJSLinks, $forDataURL) {
    // Function to connect to the DB
    $link = connectToDB();

	$strSQL = "SELECT  c.CategoryName,ROUND(SUM(p.UnitsInStock),0) as Quantity, ROUND(SUM(p.UnitsInStock*p.UnitPrice),0) as Total from FC_Categories as c , FC_Products as p WHERE c.CategoryID=p.CategoryID GROUP BY c.CategoryName ORDER BY Total DESC";
    $result = mysql_query($strSQL) or die(mysql_error());

	//Initialize <categories> element
	$strCat = "<categories>";
	
	//Initialize datasets
	$strAmtDS = "<dataset seriesname='Cost of Inventory'>";
	$strQtyDS = "<dataset seriesName='Quantity' parentYAxis='S'>";
	
    //Iterate through each data row
    if ($result) {
        while($ors = mysql_fetch_array($result)) {
            $strCat .= "<category label='" . escapeXML($ors['CategoryName'],$forDataURL) . "'/>";

            //If JavaScript links are to be added
            if ($addJSLinks) {
                //Generate the link
                //TRICKY: We//re having to escape the " character using chr(34) character.
                //In HTML, the data is provided as chart.setXMLData(" - so " is already used and un-terminated
                //For each XML attribute, we use '. So ' is used in <set link='
                //Now, we've to pass Country Name to JavaScript function, so we've to use chr(34)
                $strLink = urlencode("javaScript:updateChart(" . chr(34) . $ors['CategoryName'] .  chr(34) . ");");
                $strAmtDS .= "<set value='" . $ors['Total'] . "' link='" . $strLink . "'/>";
            }
            else
                $strAmtDS .= "<set value='" . $ors['Total'] . "' />";

            $strQtyDS .= "<set value='" . $ors['Quantity'] . "'/>";
        }
    }
    mysql_close($link);

    //Closing elements
	$strCat .= "</categories>";
	$strAmtDS .= "</dataset>";
	$strQtyDS .= "</dataset>";
	//Entire XML - concatenation
	$strXML = $strCat . $strAmtDS . $strQtyDS;
	
	return $strXML;
}

//getInventoryByProdXML function returns the inventory of all items
//within a given category and their respective quantity
function getInventoryByProdXML($catName, $forDataURL) {
    // Function to connect to the DB
    $link = connectToDB();

	$strSQL = "SELECT p.ProductName,ROUND((SUM(p.UnitsInStock)),0) as Quantity , ROUND((SUM(p.UnitsInStock*p.UnitPrice)),0) as Total from FC_Categories as c , FC_Products as p WHERE c.CategoryID=p.CategoryID and c.CategoryName='" . $catName . "' GROUP BY p.ProductName Having SUM(p.UnitsInStock)>0";
    $result = mysql_query($strSQL) or die(mysql_error());

	//Initialize <categories> element
	$strCat = "<categories>";
	
	//Initialize datasets
	$strAmtDS = "<dataset seriesname='Cost of Inventory'>";
	$strQtyDS = "<dataset seriesName='Quantity' parentYAxis='S'>";
		
	//Iterate through each data row
    if ($result) {
        while($ors = mysql_fetch_array($result)) {
            //Product Names are long - so show 8 characters and ... and show full thing in tooltip
            if (strlen($ors['ProductName'])>8)
                $shortName = escapeXML(substr($ors['ProductName'],0,8) . "...",$forDataURL);
            else
                $shortName = escapeXML($ors['ProductName'],$forDataURL);

            $strCat .= "<category label='" . $shortName . "' toolText='" . escapeXML($ors['ProductName'],$forDataURL) . "'/>";
            $strAmtDS .= "<set value='" . $ors['Total'] . "' />";
            $strQtyDS .= "<set value='" . $ors['Quantity'] . "'/>";
        }
    }
    mysql_close($link);

    //Closing elements
	$strCat .= "</categories>";
	$strAmtDS .= "</dataset>";
	$strQtyDS .= "</dataset>";
	//Entire XML - concatenation
	$strXML = $strCat . $strAmtDS . $strQtyDS;
	
	return $strXML;
}

//getSalesByCityXML function returns the XML Data for sales
//for all cities in a given year.
function getSalesByCityXML($intYear, $howMany, $forDataURL) {

	$strXML= "";
    // Function to connect to the DB
    $link = connectToDB();
	
    $strSQL = "SELECT c.City, SUM(d.Quantity*p.UnitPrice) As Total FROM FC_Customers as c, FC_Products as p, FC_Orders as o, FC_OrderDetails as d   WHERE YEAR(OrderDate)=" . $intYear . " and d.ProductID=p.ProductID and c.CustomerID=o.CustomerID and o.OrderID=d.OrderID GROUP BY c.City ORDER BY Total DESC";
	if ($howMany!=-1)
        $strSQL .= " LIMIT " . $howMany;
	
    $result = mysql_query($strSQL) or die(mysql_error());

	//Iterate through each data row
    if ($result) {
        while($ors = mysql_fetch_array($result)) {
            $strXML .= "<set label='" . escapeXML($ors['City'],$forDataURL) . "' value='" . $ors['Total'] . "' />";
        }
    }
    mysql_close($link);
	
	return $strXML;
}

//getYrlySalesByCatXML function returns the XML Data for sales
//for a given country in a given year.
function getYrlySalesByCatXML($intYear, $addJSLinks, $forDataURL) {
    // Function to connect to the DB
    $link = connectToDB();
	
	$strSQL = "SELECT g.CategoryID,g.CategoryName,SUM(d.Quantity*p.UnitPrice) as Total, SUM(d.Quantity) As Quantity FROM FC_Categories as g, FC_Products as p, FC_Orders as o, FC_OrderDetails as d  WHERE YEAR(OrderDate)=" . $intYear . " and d.ProductID=p.ProductID and g.CategoryID=p.CategoryID and o.OrderID=d.OrderID GROUP BY g.CategoryID,g.CategoryName ORDER BY Total DESC";
    $result = mysql_query($strSQL) or die(mysql_error());

	//Initialize <categories> element
	$strCat = "<categories>";
	
	//Initialize datasets
	$strAmtDS = "<dataset seriesname='Revenue'>";
	$strQtyDS = "<dataset seriesName='Quantity' parentYAxis='S'>";
	
    //Iterate through each data row
    if ($result) {
        while($ors = mysql_fetch_array($result)) {
            $strCat .= "<category label='" . escapeXML($ors['CategoryName'],$forDataURL) . "'/>";

            //If JavaScript links are to be added
            if ($addJSLinks) {
                //Generate the link
                $strLink = urlencode("javaScript:updateChart(" . $intYear . "," . $ors['CategoryID'] . ");");
                $strAmtDS .= "<set value='" . $ors['Total'] . "' link='" . $strLink . "'/>";
            }
            else
                $strAmtDS .= "<set value='" . $ors['Total'] . "' />";
            $strQtyDS .= "<set value='" . $ors['Quantity'] . "'/>";
        }
    }
    mysql_close($link);

    //Closing elements
	$strCat .= "</categories>";
	$strAmtDS .= "</dataset>";
	$strQtyDS .= "</dataset>";
	//Entire XML - concatenation
	$strXML = $strCat . $strAmtDS . $strQtyDS;
	
	return $strXML;
}

//getSalesByProdCatXML function returns the sales of all items
//within a given category in a year and their respective quantity
function getSalesByProdCatXML($intYear, $catId, $forDataURL) {
    // Function to connect to the DB
    $link = connectToDB();

	$strSQL = "SELECT g.CategoryName,p.ProductName,ROUND(SUM(d.Quantity),0) as Quantity, ROUND(SUM(d.Quantity*p.UnitPrice),0) As Total FROM FC_Categories as g,  FC_Products as p, FC_Orders as o, FC_OrderDetails as d WHERE year(o.OrderDate)=" . $intYear . " and g.CategoryID=" . $catId . " and d.ProductID=p.ProductID and g.CategoryID=p.CategoryID and o.OrderID=d.OrderID GROUP BY g.CategoryName,p.ProductName";
    $result = mysql_query($strSQL) or die(mysql_error());

	//Initialize <categories> element
	$strCat = "<categories>";
	
	//Initialize datasets
	$strAmtDS = "<dataset seriesname='Revenue'>";
	$strQtyDS = "<dataset seriesName='Quantity' parentYAxis='S'>";
	
	//Iterate through each data row
    if ($result) {
        while($ors = mysql_fetch_array($result)) {
            //Product Names are long - so show 8 characters and ... and show full thing in tooltip
            if (strlen($ors['ProductName'])>8)
                $shortName = escapeXML(substr($ors['ProductName'],0,8) . "...",$forDataURL);
            else
                $shortName = escapeXML($ors['ProductName'],$forDataURL);
            $strCat .= "<category label='" . $shortName . "' toolText='" . escapeXML($ors['ProductName'],$forDataURL) . "'/>";
            $strAmtDS .= "<set value='" . $ors['Total'] . "' />";
            $strQtyDS .= "<set value='" . $ors['Quantity'] . "'/>";
        }
    }
    mysql_close($link);

    //Closing elements
	$strCat .= "</categories>";
	$strAmtDS .= "</dataset>";
	$strQtyDS .= "</dataset>";
	//Entire XML - concatenation
	$strXML = $strCat . $strAmtDS . $strQtyDS;
	
	return $strXML;
}


//getEmployeeName function returns the name of an employee based
//on his id.
function getEmployeeName($empId) {

    // Function to connect to the DB
    $link = connectToDB();

	//Retrieve the data
    $strSQL = "SELECT FirstName, LastName FROM FC_Employees where EmployeeID=" . $empId;
    $result = mysql_query($strSQL) or die(mysql_error());
    if ($result) {
        if (mysql_num_rows($result) > 0) {
            $ors = mysql_fetch_array($result);
            $name = $ors['FirstName'] . " " . $ors['LastName'];
        } else {
            $name = " N/A ";
        }
    }
    mysql_close($link);

    return $name;
}

//getCategoryName function returns the category name for a given category
//id
function getCategoryName($catId) {
		
    // Function to connect to the DB
    $link = connectToDB();

	//Retrieve the data
	$strSQL = "SELECT CategoryName FROM FC_Categories where CategoryID=" . $catId;
    $result = mysql_query($strSQL) or die(mysql_error());
    if ($result) {
        if (mysql_num_rows($result) > 0) {
            $ors = mysql_fetch_array($result);
            $category = $ors['CategoryName'];
        } else {
            $category = " ";
        }
    }
    mysql_close($link);

    return $category;
}
?>