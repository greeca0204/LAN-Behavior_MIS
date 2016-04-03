<?php
//This page contains functions which would be used by other pages
//We can store application states, messages and constants here
//Or create functions to handle data.

@session_start();

//escapeXML function helps you escape special characters in XML
function escapeXML($strItem, $forDataURL) {
	//Common replacements
    $findStr = array("&", "<", ">", "'");
    $repStr  = array("&amp;", "&lt;", "&gt;", "&apos;");
    $strItem = str_replace($findStr, $repStr, $strItem);        
	//We've not considered any special characters here. 
	//You can add them as per your language and requirements.
	//Return
	return $strItem;
}

//getPalette method returns a value between 1-5 depending on which
//paletter the user wants to plot the chart with. 
//Here, we just read from Session variable and show it
//In your application, you could read this configuration from your 
//User Configuration Manager, database, or global application settings
function getPalette() {
	//Return
	return (((!isset($_SESSION['palette'])) || ($_SESSION['palette']=="")) ? "2" : $_SESSION['palette']);
}

//getAnimationState returns 0 or 1, depending on whether we've to
//animate chart. Here, we just read from Session variable and show it
//In your application, you could read this configuration from your 
//User Configuration Manager, database, or global application settings
function getAnimationState() {
	//Return	
	return (($_SESSION['animation']<>"0") ? "1" : "0");
}

//getCaptionFontColor function returns a color code for caption. Basic
//idea to use this is to demonstrate how to centralize your cosmetic 
//attributes for the chart
function getCaptionFontColor() {
	//Return a hex color code without #
	//FFC30C - Yellow Color
    return "666666";
}

// MonthName function converts a numeric integer into a month name
// Param: $intMonth - a numver between 1-12, otherwise defaults to 1
// Param: $flag -  if true, short name; if true, long name;
function MonthName($intMonth,$flag) {

    $arShortMonth = array (1=>"Jan", 2=>"Feb", 3=>"Mar", 4=>"Apr", 5=>"May", 6=>"Jun", 7=>"Jul", 8=>"Aug", 9=>"Sep", 10=>"Oct", 11=>"Nov", 12=>"Dec");
    $arLongMonth  = array (1=>"January", 2=>"February", 3=>"March", 4=>"April", 5=>"May", 6=>"June", 7=>"July", 8=>"August", 9=>"September", 10=>"October", 11=>"November", 12=>"December");

    if ($intMonth<1 || $intMonth>12)
        $intMonth=1;

    if ($flag)
        return $arShortMonth[$intMonth];
    else
        return $arLongMonth[$intMonth];
}
?>