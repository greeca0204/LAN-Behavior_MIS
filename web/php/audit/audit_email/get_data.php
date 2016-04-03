<?php
header('content-type: text/html; charset=utf-8');
include("../../conn.php");

$page = isset($_POST['page']) ? intval($_POST['page']) : 1;
$rows = isset($_POST['rows']) ? intval($_POST['rows']) : 20;
$sort = isset($_POST['sort']) ? strval($_POST['sort']) : 'Uid';
$order = isset($_POST['order']) ? strval($_POST['order']) : 'asc';
$offset = ($page - 1) * $rows;
$result = array();    //向前台传递数组参数

//获取查询条件
$searchUid = isset($_POST['searchUid']) ? mysql_real_escape_string(trim($_POST['searchUid'])) : '';
$searchName = isset($_POST['searchName']) ? mysql_real_escape_string(trim($_POST['searchName'])) : '';
$searchIP = isset($_POST['searchIP']) ? mysql_real_escape_string(trim($_POST['searchIP'])) : '';

$rs = mysql_query("select count(distinct Uid) from audit_email where Uid like '%$searchUid%' and Name like '%$searchName%' and User_IP like '%$searchIP%'");
$array = mysql_fetch_array($rs);
$result["total"] = $array[0];
$rs = mysql_query("select * from audit_email where Uid like '%$searchUid%' and Name like '%$searchName%' and User_IP like '%$searchIP%' group by Uid order by $sort $order limit $offset, $rows");

$items = array();
while($row = mysql_fetch_object($rs)) {
	array_push($items, $row);
}
$result['rows'] = $items;
echo json_encode($result);

mysql_close($conn);
