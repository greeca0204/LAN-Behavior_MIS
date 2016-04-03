<?php
include("Includes/Connection_inc.php");
include("Includes/Functions.php");
include("DataGen.php");

//This method writes the sales data for a given city in a country for the given year
//To this page, we're provided country and year
$intYear = $_GET['year'];
$country = $_GET['country'];
//XML Data container
$strXML = "<chart caption='" . $country . " - City wise Sales Figure for " . $intYear . "'  xAxisName='City' palette='1' animation='" . getAnimationState() . "' formatNumberScale='0' numberPrefix='$' labelDisplay='ROTATE' slantLabels='1' seriesNameInToolTip='0' sNumberSuffix=' pcs.' showValues='0' showBorder='1' showLegend='0'>";
//dd the data
$strXML .= getSalesByCountryCityXML($intYear,$country,true);
$strXML .= "<styles><definition><style type='font' name='CaptionFont' size='13' /><style type='font' name='SubCaptionFont' bold='0' /></definition><application><apply toObject='caption' styles='CaptionFont' /><apply toObject='SubCaption' styles='SubCaptionFont' /></application></styles>";
$strXML .= "</chart>";

//Output it
header('Content-type: text/xml');
echo $strXML;
?>