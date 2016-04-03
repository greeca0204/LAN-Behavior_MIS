<?php
header('content-type: text/html; charset=utf-8');
include('../../conn.php');

$tableName = $_POST['tableName'];

$rs = mysql_query("select * from app_table where Comment like '$tableName'");
$array = mysql_fetch_array($rs);

if($array) {
	//获取拼音表名
	$rs = mysql_query("select Name from app_table where Comment like '$tableName'");
	$array = mysql_fetch_array($rs);
	$tableName = $array['Name'];

	//删除表$tableName
	$rs = mysql_query("drop table $tableName");
	if($rs) {
		//删除表app_table上与$tableName的相关记录
		$rs = mysql_query("delete from app_table where Name like '$tableName'");
		if($rs) {
			echo json_encode(array('success'=>'true'));
		}
		else {
			echo json_encode(array('msg'=>'出错了！'));
		}
	}
	else {
		echo json_encode(array('msg'=>'出错了!'));
	}
}
else {
	echo json_encode(array('msg'=>'网页类别不存在!'));
}
mysql_close($conn);