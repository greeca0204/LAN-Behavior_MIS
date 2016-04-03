<?php
include("Includes/Connection_inc.php");
include("Includes/Functions.php");
include("DataGen.php");

//This method writes the top n employees  and their sales quantity for a given year.
//To this page, we're provided year and count
$intYear = $_GET['year'];
$count = $_GET['count'];
//XML Data container
$strXML  = "<chart caption='Top 5 Employees for " . $intYear . "' palette='" . getPalette() . "' animation='" . getAnimationState() . "' subCaption='(Click to slice out or right click to choose rotation mode)' xAxisName='Sales Achieved' yAxisName='Quantity' showValues='0' numberPrefix='$' formatNumberScale='0' showPercentInToolTip='0'>";
$strXML .= getSalePerEmpXML($intYear,$count,false,false,true);
//Add some styles to increase caption font size
$strXML .= "<styles><definition><style type='font' name='CaptionFont' size='15' /><style type='font' name='SubCaptionFont' bold='0' /></definition><application><apply toObject='caption' styles='CaptionFont' /><apply toObject='SubCaption' styles='SubCaptionFont' /></application></styles>";
$strXML .= "</chart>";

//Output it
header('Content-type: text/xml');
echo $strXML;
?>