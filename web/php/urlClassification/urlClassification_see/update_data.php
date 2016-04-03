<?php
header('content-type: text/html; charset=utf-8');
include('../../conn.php');

$ID = $_GET['ID'];
$Url = trim($_POST['Url']);
$Name = trim($_POST['Name']);

$Comment = $_GET['Comment'];
$rs = mysql_query("select Name from app_table where Comment like '$Comment'");
$array = mysql_fetch_array($rs);
$tableName = $array['Name'];

//不用与被选择的记录进行比较
$rs = mysql_query("select * from $tableName where Url like '$Url' and ID not like '$ID'");
$array = mysql_fetch_array($rs);
if($array) {
	echo json_encode(array('msg'=>'该记录已存在！'));
}
else {
	$rs = mysql_query("update $tableName set Url='$Url', Name='$Name' where ID='$ID'");
	if($rs) {
		echo json_encode(array('success'=>'true'));
	}
	else {
		echo json_encode(array('msg'=>'出错了！'));
	}
}

mysql_close($conn);