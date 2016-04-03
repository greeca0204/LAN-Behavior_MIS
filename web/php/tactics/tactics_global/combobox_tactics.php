<?php
header('Content-type:text/html;charset=utf-8');
require_once('../../conn.php');

$sql = "select Comment from tactics where Enable=1";
$rs = @mysql_query($sql);
$items = array ();
while($row = mysql_fetch_object($rs)) {
	array_push($items, $row);
}

echo json_encode($items);

mysql_close($conn);
?>