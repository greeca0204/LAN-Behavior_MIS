<?php
header('Content-type:text/html;charset=utf-8');
require_once('../../conn.php');

$ID = intval($_REQUEST['ID']);
$Uid = trim($_REQUEST['Uid']);

$Tactics_type = trim($_REQUEST['Tactics_type']);
//将tactics表的Comment字段值替换为tactics表的Tactics_type字段值；
$result = mysql_query("select Tactics_type from tactics where Comment like '$Tactics_type'");
$array = mysql_fetch_array($result);
$Tactics_type = $array['Tactics_type'];

$Detail = trim($_REQUEST['Detail']);
$result = mysql_query("select Name from app_table where Comment like '$Detail'");
$array = mysql_fetch_array($result);
if($array) {
	$Detail = $array['Name'];
}

$Enable = trim($_REQUEST['Enable']);
$Comment = trim($_REQUEST['Comment']);

//级联，以uid为标志，从user表中获取数据，然后进行变量赋值
$sql2 = "select Name, User_IP from user where Uid='$Uid'";
$result2 = @mysql_query($sql2);
$row = mysql_fetch_array($result2);
$Name = $row[0];
$IP = $row[1];

	//查询数据库是否有相同记录
	$sql = "select ID from tactics_allow where ID != '$ID' and Uid like '$Uid' and Tactics_type like '$Tactics_type' and Detail like '$Detail'";
	$result = mysql_query($sql);
	$array = mysql_fetch_array($result);
	if($array) {
		echo json_encode(array ('msg' => '这条策略已存在！'));
	}
	else {
		$sql = "update tactics_allow set Name='$Name', Uid='$Uid', User_IP='$IP',Tactics_type='$Tactics_type', Detail='$Detail', Enable='$Enable', Comment='$Comment' where ID='$ID'";
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