<?php
include("Includes/Connection_inc.php");
include("Includes/Functions.php");
include("DataGen.php");

//This method writes the inventory by product data for a given category.
$strCat = $_GET['category'];
//XML Data container
$strXML  = "<chart caption='" . $strCat . " - Inventory by Products' PYAxisName='Cost of Inventory' SYAxisName='Units in Inventory' xAxisName='Products' palette='" . getPalette() . "' animation='" . getAnimationState() . "' showValues='0' formatNumberScale='0' numberPrefix='$' labelDisplay='STAGGER' seriesNameInToolTip='0'>";
//Add the data
$strXML .= getInventoryByProdXML($strCat,true);
$strXML .= "<styles><definition><style type='font' name='CaptionFont' size='15' color='" . getCaptionFontColor() . "' /><style type='font' name='SubCaptionFont' bold='0' /></definition><application><apply toObject='caption' styles='CaptionFont' /><apply toObject='SubCaption' styles='SubCaptionFont' /></application></styles>";
$strXML .= "</chart>";

//Output it
header('Content-type: text/xml');
echo $strXML;
?>