<?php
include("Includes/Connection_inc.php");
include("Includes/Functions.php");
include("DataGen.php");

//This method writes the top n expensive items and its sales quantity for a given year.
//To this page, we're provided year and count
$intYear = $_GET['year'];
$count = $_GET['count'];

//We do a conditional label formatting
//If number of items to show on chart is less than 10, we wrap them
//else we show as rotated
if ($count > 10)
	$labelFormatting ="labelDisplay='ROTATE' slantLabels='1'";
else
	$labelFormatting ="labelDisplay='WRAP'";

//XML Data container
$strXML  = "<chart caption='Sale of " . $count . " Most Expensive Products for " . $intYear . "' PYAxisName='Unit Price' SYAxisName='Units Sold for the year' palette='" . getPalette() . "' animation='" . getAnimationState() . "' formatNumberScale='0' numberPrefix='$' seriesNameInToolTip='0' sNumberSuffix=' pcs.' showValues='0' plotSpacePercent='10' " . $labelFormatting . ">";
$strXML .= getExpensiveProdXML($intYear,$count,true);
//Add some styles to increase caption font size
$strXML .= "<styles><definition><style type='font' name='CaptionFont' size='15' color='" . getCaptionFontColor() . "' /><style type='font' name='SubCaptionFont' bold='0' /></definition><application><apply toObject='caption' styles='CaptionFont' /><apply toObject='SubCaption' styles='SubCaptionFont' /></application></styles>";
$strXML .= "</chart>";

//Output it
header('Content-type: text/xml');
echo $strXML;
?>