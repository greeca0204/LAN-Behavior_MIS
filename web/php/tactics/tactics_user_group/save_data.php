<?php
header('Content-type:text/html;charset=utf-8');
require_once('../../conn.php');

$Gid = trim($_REQUEST['Gid']);

$Tactics_type = trim($_REQUEST['Tactics_type']);
//将tactics表的Comment字段值替换为tactics表的Tactics_type字段值；
$result = mysql_query("select Tactics_type from tactics where Comment like '$Tactics_type'");
$array = mysql_fetch_array($result);
$Tactics_type = $array['Tactics_type'];

$Detail = trim($_REQUEST['Detail']);
$result = mysql_query("select Name from app_table where Comment like '$Detail'");
$array = mysql_fetch_array($result);
if($array) { //存在则进行app_table的Name和Comment字段值的转换
	$Detail = $array['Name'];
}

$Enable = trim($_REQUEST['Enable']);
$Comment = trim($_REQUEST['Comment']);

$sql2 = "select Name from user_group where Gid='$Gid'";
$result2 = mysql_query($sql2);
$row = mysql_fetch_array($result2);
$Name = $row[0];

//查询数据库是否有相同记录
$sql = "select ID from tactics_user_group where Gid like '$Gid' and Tactics_type like '$Tactics_type' and Detail like '$Detail'";
$result = mysql_query($sql);
$array = mysql_fetch_array($result);
if($array) {
	echo json_encode(array ('msg' => '这条记录已存在！'));
}
else {
	$sql = "insert into tactics_user_group(Name, Gid, Tactics_type, Detail, Enable, Comment) value('$Name', '$Gid', '$Tactics_type', '$Detail', '$Enable', '$Comment')";
	$result = @mysql_query($sql);
	if($result) {
		echo json_encode(array ('success' => true));
	}
	else {
		echo json_encode(array ('msg' => '出错了！'));
	}
}


mysql_close($conn);
?>