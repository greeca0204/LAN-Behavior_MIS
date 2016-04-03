<?php
header('content-type: text/html; charset=utf-8');
include('../../conn.php');

$Url = trim($_POST['Url']);
$Name = trim($_POST['Name']);

$Comment = $_GET['Comment'];
$rs = mysql_query("select Name from app_table where Comment like '$Comment'");
$array = mysql_fetch_array($rs);
$tableName = $array['Name'];

$rs = mysql_query("select * from $tableName where Url like '$Url'");
$array = mysql_fetch_array($rs);
if($array) {
	echo json_encode(array('msg'=>'该记录已存在！'));
}
else {
	$rs = mysql_query("insert into $tableName (Url, Gname, Name) values('$Url', '$Comment', '$Name')");
	if($rs) {
		echo json_encode(array('success'=>'true'));
	}
	else {
		echo json_encode(array('msg'=>'出错了！'));
	}
}

mysql_close($conn);
