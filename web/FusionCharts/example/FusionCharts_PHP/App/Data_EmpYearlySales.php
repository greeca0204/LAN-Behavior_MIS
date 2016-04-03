<?php
include("Includes/Connection_inc.php");
include("Includes/Functions.php");
include("DataGen.php");

//This method writes the employee yearly sales data as XML.
//To this page, we're provided employeed Id.
$eId = $_GET['id'];

//XML Data container
$strXML  = "<chart caption='Yearly Sales Figure for " . getEmployeeName($eId) . "' xAxisName='Year' palette='" . getPalette() . "' animation='" . getAnimationState() . "' numberPrefix='$' formatNumberScale='0'>";
//Get the data for employee for 3 years - 1994,95,96
$strXML .= "<set label='1994' value='" . getSalesFigure($eId,1994) . "' />";
$strXML .= "<set label='1995' value='" . getSalesFigure($eId,1995) . "' />";
$strXML .= "<set label='1996' value='" . getSalesFigure($eId,1996) . "' />";
$strXML .= "</chart>";

//Output it
header('Content-type: text/xml');
echo $strXML;


function getSalesFigure($eId, $intYear) {

    // Function to connect to the DB
    $link = connectToDB();
		
	//Retrieve the data
	$strSQL = "SELECT e.lastname, SUM(d.quantity*p.UnitPrice) As Total FROM FC_Employees as e,FC_Orders as o, FC_OrderDetails as d, FC_Products as p WHERE YEAR(OrderDate)=" . $intYear . " and e.EmployeeID=" . $eId . " and e.EmployeeID=o.EmployeeID and o.OrderID=d.OrderID and d.ProductID=p.ProductID GROUP BY e.lastname,e.EmployeeID ORDER BY Total DESC";
    $result = mysql_query($strSQL) or die(mysql_error());
    if ($result) {
        if (mysql_num_rows($result) > 0) {
            $ors = mysql_fetch_array($result);
            $salesFigure = $ors['Total'];
        } else {
            $salesFigure = 0;
        }
    }
    mysql_close($link);

    return $salesFigure;
}
?>