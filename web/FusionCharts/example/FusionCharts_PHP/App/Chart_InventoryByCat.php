<?php
include("Includes/Connection_inc.php");
include("Includes/FusionCharts.php");
include("Includes/PageLayout.php");
include("DataGen.php");
?>
<HTML>
<HEAD>
	<TITLE>
	FusionCharts v3 Demo Application - Inventory by Product Categories
	</TITLE>
	<?php
	//You need to include the following JS file, if you intend to embed the chart using JavaScript.
	//Embedding using JavaScripts avoids the "Click to Activate..." issue in Internet Explorer
	//When you make your own charts, make sure that the path to this JS file is correct. Else, you would get JavaScript errors.
	?>	
	<SCRIPT LANGUAGE="Javascript" SRC="FusionCharts/FusionCharts.js"></SCRIPT>		
	<SCRIPT LANGUAGE="JavaScript">			
	
	//We keep flags to check whether the charts have loaded successfully.
	//By default, we assume them to false. When each chart loads, it calls
	//a JavaScript function FC_Rendered, in which we'll update the flags
	var prodChartLoaded=false;
	
	/**
	 * FC_Rendered function is invoked by all FusionCharts charts which are registered
	 * with JavaScript. To this function, the chart passes its own DOM Id. We can check
	 * against the DOM id and update charts or loading flags.
	 *	@param	DOMId	Dom Id of the chart that was succesfully loaded.
	*/
	function FC_Rendered(DOMId){
		//Here, we update the loaded flags for each chart
		//Since we already know the charts in the page, we use conditional loop
		switch(DOMId){
			case "ByProduct":				
				prodChartLoaded = true;
				break;
		}
		return;
	}	
	
	
	/** 
	 * updateChart method is invoked when the user clicks on a column.We get the name
	 * of category as parameter. We've to get new XML to show product-wise details for
	 * that category.
	 *	@param	catName	Name of category for which we need detailed data.
	*/			
	function updateChart(catName){			
		//If the product chart has loaded
		if (prodChartLoaded){
			//DataURL for the chart
			var strURL = "Data_InvByProd.php?category=" + catName;
					
			//Sometimes, the above URL and XML data gets cached by the browser.
			//If you want your charts to get new XML data on each request,
			//you can add the following line:
			strURL = strURL + "&currTime=" + getTimeForURL();
			//getTimeForURL method is defined below and needs to be included
			//This basically adds a ever-changing parameter which bluffs
			//the browser and forces it to re-load the XML data every time.
								
			//URLEncode it - NECESSARY.
			strURL = escape(strURL);
					
			//Get reference to chart object using Dom ID "ByProduct"
			var chartObj = getChartFromId("ByProduct");			
			//Send request for XML
			chartObj.setDataURL(strURL);
		}else {
			//Show error
			alert("Please wait for the charts to load.");
			return;
		}
	}
	/**
	 * getTimeForURL method returns the current time 
	 * in a URL friendly format, so that it can be appended to
	 * dataURL for effective non-caching.
	*/
	function getTimeForURL(){
		var dt = new Date();
		var strOutput = "";
		strOutput = dt.getHours() + "_" + dt.getMinutes() + "_" + dt.getSeconds() + "_" + dt.getMilliseconds();
		return strOutput;
	}
	</SCRIPT>
	<LINK REL='stylesheet' HREF='Style.css' />
</HEAD>
<BODY topmargin='0' leftmargin='0' bottomMargin='0' rightMargin='0' bgColor='#EEEEEE'>
<?php
	//Render page headers
	echo render_pageHeader();
	//Render the main table open
	echo render_pageTableOpen();
?>
<table width='875' align='center' cellspacing='0' cellpadding='0'>
	<tr>
	<td align='center'>
	<?php
	
	$strXML  = "<chart caption='Inventory by Product Categories' subcaption='(Click on a category to see product-wise details)' PYAxisName='Cost of Inventory' SYAxisName='Units in Inventory' xAxisName='Product Category' palette='" . getPalette() . "' animation='" . getAnimationState() . "' showValues='0' formatNumberScale='0' numberPrefix='$' labelDisplay='STAGGER' seriesNameInToolTip='0'>";
	$strXML .= getInventoryByCatXML(true,false);
	//Add some styles to increase caption font size
	$strXML .= "<styles><definition><style type='font' name='CaptionFont' size='15' color='" . getCaptionFontColor() . "' /><style type='font' name='SubCaptionFont' bold='0' /></definition><application><apply toObject='caption' styles='CaptionFont' /><apply toObject='SubCaption' styles='SubCaptionFont' /></application></styles>";
	$strXML .= "</chart>";

    echo renderChart("FusionCharts/MSColumn3DLineDY.swf", "", $strXML , "ByCategory", 800, 300, false, false);
	?>
	</td>
	</tr>
</table>

<?php	
	//Separator line
	echo drawSepLine();
?>	

<P align='center' class='text'>Click on a column above to see the product-wise details.</P>

<?php
	//Separator line
	echo drawSepLine();
?>	
<table width='875' align='center' cellspacing='0' cellpadding='0'>
	<tr>
	<td align='center'>
		<?php	
		//Chart with changed "No data to display" message
		//We initialize the chart with <chart></chart>
		echo renderChart("FusionCharts/MSColumn3DLineDY.swf?ChartNoDataText=Please select a category above.", "", "<chart></chart>", "ByProduct", 800, 300, false, true);
		?>
	</td>
	</tr>
</table>	
<?php
//Close the main table
echo render_pageTableClose();
?>
</BODY>
</HTML>
