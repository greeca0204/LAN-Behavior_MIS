<?php
header('content-type: text/html; charset=utf-8');
include("../../conn.php");

$Uid = $_REQUEST['Uid'];
$sort = isset($_POST['sort']) ? strval($_POST['sort']) : 'Time';
$order = isset($_POST['order']) ? strval($_POST['order']) : 'desc';

$rs = mysql_query("select count(*) from audit_search where Uid like '$Uid'");
$array = mysql_fetch_array($rs);
$result["total"] = $array[0];
$rs = mysql_query("select * from audit_search where Uid like '$Uid' order by $sort $order");

$items = array();
while($row = mysql_fetch_object($rs)) {
	array_push($items, $row);
}
$result['rows'] = $items;
echo json_encode($result);

mysql_close($conn);
