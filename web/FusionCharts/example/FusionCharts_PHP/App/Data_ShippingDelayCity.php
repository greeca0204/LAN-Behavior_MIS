<?php
include("Includes/Connection_inc.php");
include("Includes/Functions.php");
include("DataGen.php");

//This page provides the XML data for shipping delay by cities.
//It needs two parameters necessarily:
// - Year
// - Country
$intYear = $_GET['year'];
$country = $_GET['country'];

//Variable to store XML Data
$strXML  = "<chart caption='" . $country . " - Average delay in Shipping Time' yAxisName='Delay (in days)' xAxisName='City' palette='" . getPalette() . "' animation='" . getAnimationState() . "' showValues='0' formatNumberScale='1' numberSuffix=' days'  numDivLines='5' >";
$strXML .= getAvgShipTimeCityXML($intYear, $country,true);
$strXML .= "<styles><definition><style type='font' name='CaptionFont' size='15' color='" . getCaptionFontColor() . "' /><style type='font' name='SubCaptionFont' bold='0' /></definition><application><apply toObject='caption' styles='CaptionFont' /><apply toObject='SubCaption' styles='SubCaptionFont' /></application></styles>";
$strXML .= "</chart>";

header('Content-type: text/xml');
echo $strXML;
?>