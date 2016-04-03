<?php
header('content-type: text/html; charset=utf-8');
include("../../conn.php");

$sort = isset($_POST['sort']) ? strval($_POST['sort']) : 'Datetime';
$order = isset($_POST['order']) ? strval($_POST['order']) : 'asc';
$IP = mysql_real_escape_string($_REQUEST['IP']);

$rs = mysql_query("select distinct file_name, Datetime from audit_email_read where IP like '$IP' order by $sort $order");
$items = array();
while($row = mysql_fetch_object($rs)){
	array_push($items, $row);
}
echo json_encode($items);
