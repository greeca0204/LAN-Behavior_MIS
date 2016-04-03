<?php
header('Content-type:text/html;charset=utf-8');
include('../../conn.php');

$page = isset($_POST['page']) ? intval($_POST['page']) : 1;
$rows = isset($_POST['rows']) ? intval($_POST['rows']) : 20;
$offset = ($page - 1) * $rows;
$result = array ();

$Comment = $_POST['Comment'];
$searchUrl = isset($_POST['searchUrl']) ? trim($_POST['searchUrl']) : '';

if(!empty($Comment)) {
	$rs = mysql_query("select Name from app_table where Comment like '$Comment'");
	$array = mysql_fetch_array($rs);
	if($array) {
		$tableName = $array['Name'];

		$rs = mysql_query("select count(*) from $tableName where Url like '%$searchUrl%'");
		$row = mysql_fetch_row($rs);
		$result["total"] = $row[0];

		$rs = mysql_query("select * from $tableName where Url like '%$searchUrl%' limit $offset,$rows");

		$items = array ();
		while($row = mysql_fetch_object($rs)) {
			array_push($items, $row);
		}

		$result["rows"] = $items;
		echo json_encode($result);
	}
}

mysql_close($conn);
?>