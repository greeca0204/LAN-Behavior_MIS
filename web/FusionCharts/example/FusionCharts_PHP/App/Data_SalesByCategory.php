<?php
include("Includes/Connection_inc.php");
include("Includes/Functions.php");
include("DataGen.php");

	//This page outputs the sales by category XML data for the the specified year.
	//Request the year for which we've to show data.
	//If year has not been provided, we default to 1996
	if ((!isset($_GET['year'])) || $_GET['year']=="")
		$intYear = 1996;
    else
        $intYear = $_GET['year'];
        
	$strXML = "<chart caption='Category wise Sales for " . $intYear . "' subcaption='(Click on a column to see monthly sales for that category in the chart below this)' xAxisName='Month' palette='" . getPalette() . "' animation='" . getAnimationState() . "' formatNumberScale='0' numberPrefix='$' showValues='0' numDivLines='4' legendPosition='BOTTOM'>";
	$strXML .= getCumulativeSalesByCatXML($intYear,true);
	//Add some styles to increase caption font size
	$strXML .= "<styles><definition><style type='font' name='CaptionFont' color='" . getCaptionFontColor() . "' size='15' /><style type='font' name='SubCaptionFont' bold='0' /></definition><application><apply toObject='caption' styles='CaptionFont' /><apply toObject='SubCaption' styles='SubCaptionFont' /></application></styles>";
	$strXML .= "</chart>";
	
	//Write the XML data to output stream - WITHOUT ANY HTML Tag.
	header('Content-type: text/xml');
	echo $strXML;
?>
