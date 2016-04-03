<?php
include("conn.php");

$User = 'admin';
$array = array();
$rs = mysql_query("select ID from login where User like '$User'");
while($row = mysql_fetch_array($rs)) {
	$array[] = $row['ID'];
}

echo "yes";
?>
