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
//$searchStartTime = !empty($_POST['searchStartTime']) ? mysql_real_escape_string(trim($_POST['searchStartTime'])) : '1000-01-01 00:00:00';
//$searchEndTime = !empty($_POST['searchEndTime']) ? mysql_real_escape_string(trim($_POST['searchEndTime'])) : '9999-12-31 24:00:00';

//进行IP分组，相同的记录不输出
$rs = mysql_query("select count(distinct IP) from audit_email_read where Uid like '%$searchUid%' and Name like '%$searchName%' and IP like '%$searchIP%'");
$array = mysql_fetch_array($rs);
$result["total"] = $array[0];
$rs = mysql_query("select * from audit_email_read where Uid like '%$searchUid%' and Name like '%$searchName%' and IP like '%$searchIP%' group by IP order by $sort $order limit $offset, $rows");

$items = array();
while($row = mysql_fetch_object($rs)) {
	array_push($items, $row);
}
$result['rows'] = $items;
echo json_encode($result);

mysql_close($conn);
