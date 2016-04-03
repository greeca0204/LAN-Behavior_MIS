<?php
include("Includes/Connection_inc.php");
include("Includes/Functions.php");
include("DataGen.php");

//This method writes the number of orders the employee processes yearly data as XML.
//To this page, we're provided employeed Id.
$eId = $_GET['id'];

//XML Data container
$strXML = "<chart caption='Number of Orders - " . getEmployeeName($eId) . "' xAxisName='Year' palette='" . getPalette() . "' animation='" . getAnimationState() . "' >";
//Get the data for employee for 3 years - 1994,95,96
$strXML .= "<set label='1994' value='" . getNumOrders($eId,1994) . "' />";
$strXML .= "<set label='1995' value='" . getNumOrders($eId,1995) . "' />";
$strXML .= "<set label='1996' value='" . getNumOrders($eId,1996) . "' />";
$strXML .= "</chart>";

//Output it
header('Content-type: text/xml');
echo $strXML;


function getNumOrders($eId, $intYear) {		
    // Function to connect to the DB
    $link = connectToDB();
		
	//Retrieve the data
	$strSQL = "SELECT Count(OrderID) As Total FROM FC_Orders WHERE YEAR(OrderDate)=" . $intYear . " and EmployeeID=" . $eId;
    $result = mysql_query($strSQL) or die(mysql_error());
    if ($result) {
        if (mysql_num_rows($result) > 0) {
            $ors = mysql_fetch_array($result);
            $getNumOrders = $ors['Total'];
        } else {
            $getNumOrders = 0;
        }
    }
    mysql_close($link);

	return $getNumOrders;
}
?>