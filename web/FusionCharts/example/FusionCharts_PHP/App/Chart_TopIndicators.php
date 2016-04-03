<?php
include("Includes/Connection_inc.php");
include("Includes/FusionCharts.php");
include("Includes/PageLayout.php");
include("DataGen.php");
?>
<HTML>
<HEAD>
	<TITLE>
	FusionCharts v3 Demo Application - My Top Indicators
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
	var cusChartLoaded=false;
	var countryChartLoaded=false;
	var cityChartLoaded=false;
	var expChartLoaded=false;
	
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
			case "TopCustomer":				
				cusChartLoaded = true;
				break;
			case "TopCountries":				
				countryChartLoaded = true;
				break;
			case "ExpProds":				
				cityChartLoaded = true;
				break;
			case "TopCities":				
				expChartLoaded = true;
				break;
		}
		return;
	}
	
	/** 
	 * updateCusChart method is invoked when the user selects a different number of
	 * items in the select. Here, we get that value and year (parameter) and create
	 * a request for new XML Data to Data_TopCustomers.php. 
	 *	@param	year	Year for which we need the data
	*/		
	function updateCusChart(year){
		//If the chart has loaded...
		if (cusChartLoaded){
			//How many data items to show - collected from SELECT
			var num = (document.cusCountSelect.count.value);
		
			//DataURL for the chart
			var strURL = "Data_TopCustomers.php?year=" + year + "&count=" + num;
		
			//Sometimes, the above URL and XML data gets cached by the browser.
			//If you want your charts to get new XML data on each request,
			//you can add the following line:
			strURL = strURL + "&currTime=" + getTimeForURL();
			//getTimeForURL method is defined below and needs to be included
			//This basically adds a ever-changing parameter which bluffs
			//the browser and forces it to re-load the XML data every time.
								
			//URLEncode it - NECESSARY.
			strURL = escape(strURL);
					
			//Get reference to chart object using Dom ID "TopCustomer"
			var chartObj = getChartFromId("TopCustomer");			
			//Send request for XML
			chartObj.setDataURL(strURL);
		}else {
			//Show error
			alert("Please wait for the charts to load.");
			return;
		}
	}
	/** 
	 * updateCountryChart method is invoked when the user selects a different number of
	 * items in the select. Here, we get that value and year (parameter) and create
	 * a request for new XML Data to Data_TopCountry.php. 
	 *	@param	year	Year for which we need the data
	*/		
	function updateCountryChart(year){
		//If the chart has loaded...
		if (countryChartLoaded){
			//How many data items to show - collected from SELECT
			var num = (document.countryCountSelect.count.value);
		
			//DataURL for the chart
			var strURL = "Data_TopCountry.php?year=" + year + "&count=" + num;
		
			//Sometimes, the above URL and XML data gets cached by the browser.
			//If you want your charts to get new XML data on each request,
			//you can add the following line:
			strURL = strURL + "&currTime=" + getTimeForURL();
			//getTimeForURL method is defined below and needs to be included
			//This basically adds a ever-changing parameter which bluffs
			//the browser and forces it to re-load the XML data every time.
								
			//URLEncode it - NECESSARY.
			strURL = escape(strURL);
					
			//Get reference to chart object using Dom ID "TopCountries"
			var chartObj = getChartFromId("TopCountries");			
			//Send request for XML
			chartObj.setDataURL(strURL);
		}else {
			//Show error
			alert("Please wait for the charts to load.");
			return;
		}
	}
	/** 
	 * updateCityChart method is invoked when the user selects a different number of
	 * items in the select. Here, we get that value and year (parameter) and create
	 * a request for new XML Data to Data_TopCity.php. 
	 *	@param	year	Year for which we need the data
	*/		
	function updateCityChart(year){
		if (cityChartLoaded){
			//How many data items to show - collected from SELECT
			var num = (document.cityCountSelect.count.value);
		
			//DataURL for the chart
			var strURL = "Data_TopCity.php?year=" + year + "&count=" + num;
		
			//Sometimes, the above URL and XML data gets cached by the browser.
			//If you want your charts to get new XML data on each request,
			//you can add the following line:
			strURL = strURL + "&currTime=" + getTimeForURL();
			//getTimeForURL method is defined below and needs to be included
			//This basically adds a ever-changing parameter which bluffs
			//the browser and forces it to re-load the XML data every time.
								
			//URLEncode it - NECESSARY.
			strURL = escape(strURL);
					
			//Get reference to chart object using Dom ID "TopCities"
			var chartObj = getChartFromId("TopCities");			
			//Send request for XML
			chartObj.setDataURL(strURL);
		}else {
			//Show error
			alert("Please wait for the charts to load.");
			return;
		}
	}
	/** 
	 * updateExpChart method is invoked when the user selects a different number of
	 * items in the select. Here, we get that value and year (parameter) and create
	 * a request for new XML Data to ExpProdData.php. 
	 *	@param	year	Year for which we need the data
	*/		
	function updateExpChart(year){
		if (expChartLoaded){
			//How many data items to show - collected from SELECT
			var num = (document.countExpSelect.count.value);
		
			//DataURL for the chart
			var strURL = "Data_TopExpProd.php?year=" + year + "&count=" + num;
		
			//Sometimes, the above URL and XML data gets cached by the browser.
			//If you want your charts to get new XML data on each request,
			//you can add the following line:
			strURL = strURL + "&currTime=" + getTimeForURL();
			//getTimeForURL method is defined below and needs to be included
			//This basically adds a ever-changing parameter which bluffs
			//the browser and forces it to re-load the XML data every time.
								
			//URLEncode it - NECESSARY.
			strURL = escape(strURL);
					
			//Get reference to chart object using Dom ID "ExpProds"
			var chartObj = getChartFromId("ExpProds");			
			//Send request for XML
			chartObj.setDataURL(strURL);
		}else{
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
	//Request the year for which we've to show data.
    if ((!isset($_GET['year'])) || $_GET['year']=="")
        $intYear = 1996;
    else
        $intYear = $_GET['year'];
	
	//Render page headers
	echo render_pageHeader();
	//Render the main table open
	echo render_pageTableOpen();
?>
<table width='875' align='center' cellspacing='0' cellpadding='0'>
	<tr>
	<td align='center'>
	<?php
	$strXML  = "<chart caption='Top 5 Customers for " . $intYear . "' palette='" . getPalette() . "' animation='" . getAnimationState() . "' PYAxisName='Amount' SYAxisName='Quantity' showValues='0' numberPrefix='$' sNumberSuffix=' pcs.' seriesNameInToolTip='0' formatNumberScale='0'>";
	$strXML .= getTopCustomersXML(1995,5,false);
	//Add some styles to increase caption font size
	$strXML .= "<styles><definition><style type='font' name='CaptionFont' size='15' color='" . getCaptionFontColor() . "' /><style type='font' name='SubCaptionFont' bold='0' /></definition><application><apply toObject='caption' styles='CaptionFont' /><apply toObject='SubCaption' styles='SubCaptionFont' /></application></styles>";
	$strXML .= "</chart>";
		
	echo renderChart("FusionCharts/MSColumn3DLineDY.swf", "", $strXML, "TopCustomer", 750, 300, false, true);
	?>
	</td>
	</tr>
</table>

<?php	
	//Separator line
	echo drawSepLine();
?>	
<CENTER>
<FORM NAME='cusCountSelect'>
	<span class='text'>Show top&nbsp;&nbsp;<SELECT NAME='count' class='select' onChange="JavaScript:updateCusChart(<?php echo $intYear;?>);"><OPTION value='5'>5<OPTION value='10'>10<OPTION value='20'>20<OPTION value='30'>30</SELECT>&nbsp;&nbsp;Customers</span>
</FORM>
</CENTER>
<?php	
	//Separator line
	echo drawSepLine();
?>	


<table width='875' align='center' cellspacing='0' cellpadding='0'>
	<tr>
	<td align='center'>
	<?php	
	$strXML  = "<chart caption='" . $intYear . " - Top 5 Countries By Sales ' palette='" . getPalette() . "' animation='" . getAnimationState() . "' formatNumberScale='0' numberPrefix='$' seriesNameInToolTip='0' sNumberSuffix=' pcs.' showValues='0' plotSpacePercent='10'>";
	$strXML .= getSalesByCountryXML($intYear,5,false,false);
	//Add some styles to increase caption font size
	$strXML .= "<styles><definition><style type='font' name='CaptionFont' size='15' color='" . getCaptionFontColor() . "' /><style type='font' name='SubCaptionFont' bold='0' /></definition><application><apply toObject='caption' styles='CaptionFont' /><apply toObject='SubCaption' styles='SubCaptionFont' /></application></styles>";
	$strXML .= "</chart>";
		
	echo renderChart("FusionCharts/MSColumn3DLineDY.swf", "", $strXML, "TopCountries", 750, 300, false, true);
	?>
	</td>
	</tr>
</table>

<?php	
	//Separator line
	echo drawSepLine();
?>	
<CENTER>
<FORM NAME='countryCountSelect'>
	<span class='text'>Show top&nbsp;&nbsp;<SELECT NAME='count' class='select' onChange="JavaScript:updateCountryChart(<?php echo $intYear;?>);"><OPTION value='5'>5<OPTION value='10'>10<OPTION value='15'>15<OPTION value='20'>20</SELECT>&nbsp;&nbsp;Countries</span>
</FORM>
</CENTER>
<?php	
	//Separator line
	echo drawSepLine();
?>	

<table width='875' align='center' cellspacing='0' cellpadding='0'>
	<tr>
	<td align='center'>
	<?php	
	$strXML  = "<chart caption='Top 5 Most Expensive Products' PYAxisName='Unit Price' SYAxisName='Units Sold for the year' palette='" . getPalette() . "' animation='" . getAnimationState() . "' formatNumberScale='0' numberPrefix='$' seriesNameInToolTip='0' sNumberSuffix=' pcs.' showValues='0' plotSpacePercent='10'>";
	$strXML .= getExpensiveProdXML(1995,5, false);
	//Add some styles to increase caption font size
	$strXML .= "<styles><definition><style type='font' name='CaptionFont' size='15' color='" . getCaptionFontColor() . "' /><style type='font' name='SubCaptionFont' bold='0' /></definition><application><apply toObject='caption' styles='CaptionFont' /><apply toObject='SubCaption' styles='SubCaptionFont' /></application></styles>";
	$strXML .= "</chart>"	;
	echo renderChart("FusionCharts/MSColumn3DLineDY.swf", "", $strXML, "ExpProds", 750, 350, false, true);
	?>
	</td>
	</tr>
</table>

<?php	
	//Separator line
	echo drawSepLine();
?>	
<CENTER>
<FORM NAME='countExpSelect'>
	<span class='text'>Show top&nbsp;&nbsp;<SELECT NAME='count' class='select' onChange="JavaScript:updateExpChart(<?php echo $intYear;?>);"><OPTION value='5'>5<OPTION value='10'>10<OPTION value='20'>20<OPTION value='30'>30</SELECT>&nbsp;&nbsp;Expensive Products</span>
</FORM>
</CENTER>
<?php	
	//Separator line
	echo drawSepLine();
?>	


<table width='875' align='center' cellspacing='0' cellpadding='0'>
	<tr>
	<td align='center'>
	<?php	
	
$strXML  = "<chart caption='" . $intYear . "- Top 5 Cities By Sales'  palette='" . getPalette() . "' animation='" . getAnimationState() . "' formatNumberScale='0' numberPrefix='$' seriesNameInToolTip='0' sNumberSuffix='pcs.' showValues='0' plotSpacePercent='10' labelDisplay='STAGGER'>";
	$strXML .= getSalesByCityXML($intYear,5, false);
	//Add some styles to increase caption font size
	$strXML .= "<styles><definition><style type='font' name='CaptionFont' size='15' color='" . getCaptionFontColor() . "' /><style type='font' name='SubCaptionFont' bold='0' /></definition><application><apply toObject='caption' styles='CaptionFont' /><apply toObject='SubCaption' styles='SubCaptionFont' /></application></styles>";
	$strXML .= "</chart>";
		
	echo renderChart("FusionCharts/Column3D.swf", "", $strXML, "TopCities", 700, 250, false, true);
	?>
	</td>
	</tr>
</table>

<?php	
	//Separator line
	echo drawSepLine();
?>	
<CENTER>
<FORM NAME='cityCountSelect'>
	<span class='text'>Show top&nbsp;&nbsp;<SELECT NAME='count' class='select' onChange="JavaScript:updateCityChart(<?php echo $intYear;?>);"><OPTION value='5'>5<OPTION value='10'>10<OPTION value='20'>20<OPTION value='50'>50</SELECT>&nbsp;&nbsp;Cities</span>
</FORM>
</CENTER>
<?php	
	//Separator line
	echo drawSepLine();
?>	



<?php
//Close the main table
echo render_pageTableClose();
?>
</BODY>
</HTML>