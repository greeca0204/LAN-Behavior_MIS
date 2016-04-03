<?php
    header('content-type:text/html;charset=utf-8');
    require_once('../conn.php');

	$page = isset($_POST['page']) ? intval($_POST['page']) : 1;
	$rows = isset($_POST['rows']) ? intval($_POST['rows']) : 10;
	$sort = isset($_POST['sort']) ? strval($_POST['sort']) : 'Gid';
	$order = isset($_POST['order']) ? strval($_POST['order']) : 'asc';
	$offset = ($page-1)*$rows;
	$result = array();

	//获取搜索条件
	$searchName = isset($_POST['searchName']) ? mysql_real_escape_string(trim($_POST['searchName'])) : '';
	$searchGid = isset($_POST['searchGid']) ? mysql_real_escape_string(trim($_POST['searchGid'])) : '';
	/*
	$searchFlow = isset($_POST['searchFlow']) ? mysql_real_escape_string(trim($_POST['searchFlow'])) : '';
	$searchUpload = isset($_POST['searchUpload']) ? mysql_real_escape_string(trim($_POST['searchUpload'])) : '';
	$searchDownload = isset($_POST['searchDownload']) ? mysql_real_escape_string(trim($_POST['searchDownload'])) : '';
	*/

	$rs = mysql_query("select count(*) from user_group where Name like '%$searchName%' and Gid like '%$searchGid%'");
	$row = mysql_fetch_row($rs);
	$result["total"] = $row[0];
	$rs = mysql_query("select * from user_group where Name like '%$searchName%' and Gid like '%$searchGid%' order by $sort $order limit $offset,$rows");
	
	$items = array();
	while($row = mysql_fetch_object($rs)){
		array_push($items, $row);
	}
	$result["rows"] = $items;

	echo json_encode($result);

    mysql_close($conn);
?>