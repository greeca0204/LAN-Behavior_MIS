<?php
    header('Content-type:text/html;charset=utf-8');
    require_once('../conn.php');

	$page = isset($_POST['page']) ? intval($_POST['page']) : 1;
	$rows = isset($_POST['rows']) ? intval($_POST['rows']) : 20;
	$sort = isset($_POST['sort']) ? strval($_POST['sort']) : 'Uid';
	$order = isset($_POST['order']) ? strval($_POST['order']) : 'asc';
	$offset = ($page-1)*$rows;
	$result = array();

	//获取搜索条件
	$searchName = isset($_POST['searchName']) ? mysql_real_escape_string(trim($_POST['searchName'])) : '';
	$searchGid = isset($_POST['searchGid']) ? mysql_real_escape_string(trim($_POST['searchGid'])) : '';
	$searchUid = isset($_POST['searchUid']) ? mysql_real_escape_string(trim($_POST['searchUid'])) : '';
	$searchIP = isset($_POST['searchIP']) ? mysql_real_escape_string(trim($_POST['searchIP'])) : '';
	/*$searchStartTime = !empty($_POST['searchStartTime']) ? mysql_real_escape_string(trim($_POST['searchStartTime'])) : '0000-00-00 00:00:00';
	$searchEndTime = !empty($_POST['searchEndTime']) ? mysql_real_escape_string(trim($_POST['searchEndTime'])) : '9999-12-31 24:00:00';
*/
	$rs = mysql_query("select count(*) from user where Name like '%$searchName%' and Gid like '%$searchGid%' and Uid like '%$searchUid%' and User_IP like '%$searchIP%'");
	$row = mysql_fetch_row($rs);
	$result["total"] = $row[0];
	$rs = mysql_query("select * from user where Name like '%$searchName%' and Gid like '%$searchGid%' and Uid like '%$searchUid%' and User_IP like '%$searchIP%' order by $sort $order limit $offset,$rows");
	
	$items = array();
	while($row = mysql_fetch_object($rs)){
		array_push($items, $row);
	}
	$result["rows"] = $items;
    echo json_encode($result);

    mysql_close($conn);
?>
