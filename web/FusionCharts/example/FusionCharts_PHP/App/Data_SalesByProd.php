<?php
include("Includes/Connection_inc.php");
include("Includes/Functions.php");
include("DataGen.php");

    //This page outputs the sales by product (in a category) XML data for the the specified 
	//year and month.	
	$intYear = $_GET['year'];
	$intMonth = $_GET['month'];
	$intCatId = $_GET['catId'];
		
	$strXML  = "<chart caption='" . getCategoryName($intCatId) . " - Product wise Sales for " . MonthName($intMonth, false) . " " . $intYear . "' xAxisName='Product' palette='" . getPalette() . "' animation='" . getAnimationState() . "' formatNumberScale='0' numberPrefix='$' showValues='0' PYAxisName='Revenue' SYAxisName='Units Sold' seriesNameInToolTip='0'>";
	$strXML .= getSalesByProdXML($intYear, $intMonth, $intCatId, true);
	//Add some styles to increase caption font size
	$strXML .= "<styles><definition><style type='font' name='CaptionFont' size='15' color='" . getCaptionFontColor() . "' /><style type='font' name='SubCaptionFont' bold='0' /></definition><application><apply toObject='caption' styles='CaptionFont' /><apply toObject='SubCaption' styles='SubCaptionFont' /></application></styles>";
	$strXML .= "</chart>";
	
	//Write the XML data to output stream - WITHOUT ANY HTML Tag.
	header('Content-type: text/xml');
	echo $strXML;
?>
