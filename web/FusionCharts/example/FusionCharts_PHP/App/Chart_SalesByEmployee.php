<?php
include("Includes/Connection_inc.php");
include("Includes/FusionCharts.php");
include("Includes/PageLayout.php");
include("DataGen.php");
?>
<HTML>
<HEAD>
	<TITLE>
	FusionCharts v3 Demo Application - Sales By Employees
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
	var detailsChartLoaded=false;
	var orderChartLoaded=false;
	
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
			case "EmployeeDetails":				
				detailsChartLoaded = true;
				break;
			case "EmployeeDetailsOrders":				
				orderChartLoaded = true;
				break;
		}
		return;
	}	
	/** 
	 * updateChart method is invoked when the user clicks on a pie slice.
	 * In this method, we get the index of the employee after which we request for XML data
	 * for that that factory from Data_EmpYearlySales.php, Data_EmpYearlyOrder.php, and finally
	 * update the Column Charts.
	 *	@param	employeeId	Id of the employee
	*/		
	function updateChart(employeeId){			
		//Update the chart only if has loaded
		if (detailsChartLoaded){
			//DataURL for the chart
			var strURL = "Data_EmpYearlySales.php?id=" + employeeId;
					
			//Sometimes, the above URL and XML data gets cached by the browser.
			//If you want your charts to get new XML data on each request,
			//you can add the following line:
			strURL = strURL + "&currTime=" + getTimeForURL();
			//getTimeForURL method is defined below and needs to be included
			//This basically adds a ever-changing parameter which bluffs
			//the browser and forces it to re-load the XML data every time.
								
			//URLEncode it - NECESSARY.
			strURL = escape(strURL);
		
			//Get reference to chart object using Dom ID "EmployeeDetails"
			var chartObj = getChartFromId("EmployeeDetails");			
			//Send request for XML
			chartObj.setDataURL(strURL);
		} else {
			//Show error
			alert("Please wait for the charts to load.");
			return;
		}
		
		//Update other chart too (Orders) (if loaded)
		if (orderChartLoaded){
			//DataURL for the chart
			var strURL = "Data_EmpYearlyOrder.php?id=" + employeeId;
			strURL = strURL + "&currTime=" + getTimeForURL();
			strURL = escape(strURL);
		
			var ordersChartObj = getChartFromId("EmployeeDetailsOrders");			
			//Send request for XML
			ordersChartObj.setDataURL(strURL);		
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
		//Generate the XML data for the main pie chart.
		$strXML  = "<chart caption='Sales Per Employee for year " . $intYear ."' subCaption='(Click on a pie slice to view details)' palette='" . getPalette() . "' animation='" . getAnimationState() . "' formatNumberScale='0' numberPrefix='$' pieSliceDepth='30' startingAngle='125'>";
		$strXML .= getSalePerEmpXML($intYear,-1,true,true,false);
		//Add some styles to increase caption font size
		$strXML .= "<styles><definition><style type='font' name='CaptionFont' size='15' color='" . getCaptionFontColor() . "' /><style type='font' name='SubCaptionFont' bold='0' /></definition><application><apply toObject='caption' styles='CaptionFont' /><apply toObject='SubCaption' styles='SubCaptionFont' /></application></styles>";
		$strXML .= "</chart>";
		
		//Render it as pie chart
		echo renderChart("FusionCharts/Pie3D.swf", "", $strXML, "EmpSalesTotal", 700, 350, false, false);
	?>
	</td>
	</tr>
</table>

<?php	
	//Separator line
	echo drawSepLine();
?>	

<P align='center' class='text'>Click on a pie slice above to see the employee's past performance.</P>

<?php
	//Separator line
	echo drawSepLine();
?>	

	<table width='700' align='center'>
		<tr>
		<td width='400' align='center' style='BORDER-RIGHT:#EEEEEE 1px solid;'>
		<?php	
			//Column 3D Chart with changed "No data to display" message
			//We initialize the chart with <chart></chart>
			echo renderChart("FusionCharts/Column3D.swf?ChartNoDataText=Please select an employee above.", "", "<chart></chart>", "EmployeeDetails", 400, 250, false, true);
		?>
		</td>
		<td width='300' align='center'>
		<?php
			//Column 3D Chart with changed "No data to display" message
			//We initialize the chart with <chart></chart>
			echo renderChart("FusionCharts/Column3D.swf?ChartNoDataText=Please select an employee above.", "", "<chart></chart>", "EmployeeDetailsOrders", 300, 250, false, true);
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