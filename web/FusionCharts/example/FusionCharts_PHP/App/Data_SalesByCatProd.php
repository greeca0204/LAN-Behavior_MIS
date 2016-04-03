<?php
include("Includes/Connection_inc.php");
include("Includes/Functions.php");
include("DataGen.php");

//This method writes the sales by product data for a given category for a given year
$intYear = $_GET['year'];
$intCatId = $_GET['catId'];
//XML Data container
$strXML = "<chart caption='" . getCategoryName($intCatId) . " - Product wise sales in " . $intYear . "' PYAXisName='Revenue' SYAxisName='Units Sold' xAxisName='Products' palette='" . getPalette() . "' animation='" . getAnimationState() . "' showValues='0' formatNumberScale='0' numberPrefix='$' labelDisplay='STAGGER' seriesNameInToolTip='0'>";
//Add the data
$strXML .= getSalesByProdCatXML($intYear,$intCatId,true);
$strXML .= "<styles><definition><style type='font' name='CaptionFont' size='15' color='" . getCaptionFontColor() . "' /><style type='font' name='SubCaptionFont' bold='0' /></definition><application><apply toObject='caption' styles='CaptionFont' /><apply toObject='SubCaption' styles='SubCaptionFont' /></application></styles>";
$strXML .= "</chart>";

//Output it
header('Content-type: text/xml');
echo $strXML;
?>