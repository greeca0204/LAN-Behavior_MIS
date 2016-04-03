<?php
header('Content-type:text/html;charset=utf-8');
require_once('../../conn.php');

$page = isset($_POST['page']) ? intval($_POST['page']) : 1;
$rows = isset($_POST['rows']) ? intval($_POST['rows']) : 10;
$sort = isset($_POST['sort']) ? strval($_POST['sort']) : 'Enable';
$order = isset($_POST['order']) ? strval($_POST['order']) : 'asc';
$offset = ($page - 1) * $rows;
$result = array ();

//获取搜索条件
$searchTactics = isset($_POST['searchTactics']) ? mysql_real_escape_string(trim($_POST['searchTactics'])) : '';
$searchEnable = isset($_POST['searchEnable']) ? mysql_real_escape_string(trim($_POST['searchEnable'])) : '';

//判断$searchEnable是否为空值
if($searchEnable == '') {
	$rs = mysql_query("select count(*) from tactics where Tactics_type in (select Tactics_type from tactics where Comment like '%$searchTactics%')");
	$row = mysql_fetch_row($rs);
	$result["total"] = $row[0];
	$rs = mysql_query("select * from tactics where Tactics_type in (select Tactics_type from tactics where Comment like '%$searchTactics%') order by $sort $order limit $offset,$rows");
} else {
	$rs = mysql_query("select count(*) from tactics_allow where Tactics_type in (select Tactics_type from tactics where Comment like '%$searchTactics%') and Enable like '$searchEnable'");
	$row = mysql_fetch_row($rs);
	$result["total"] = $row[0];
	$rs = mysql_query("select * from tactics where Tactics_type in (select Tactics_type from tactics where Comment like '%$searchTactics%') and Enable like '$searchEnable' order by $sort $order limit $offset,$rows");
}

$items = array ();
while($row = mysql_fetch_object($rs)) {
	array_push($items, $row);
}
$result["rows"] = $items;

echo json_encode($result);

mysql_close($conn);
?>