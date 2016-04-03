<?php
header('Content-Type:text/html; charset=UTF-8');
include('../conn.php');

session_start();

$passwordOld = md5($_POST['passwordOld']);
$password = md5($_POST['password']);

$sql = "select * from login where User='admin' and Password='$passwordOld'";
$result = mysql_query($sql);
$array = mysql_fetch_array($result);
//判断当前密码输入是否正确
if(!$array['ID']) {
	echo json_encode(array('success'=>'passwordOld'));     //当前密码输入错误
}
else {
	//更新密码
	$sql = "update login set Password='$password' where User='admin'";
	$result = mysql_query($sql);

	if($result) {
		//要清除会话变量，将$_SESSION超级全局变量设置为一个空数组
		$_SESSION = array();
		//如果存在一个会话cookie，通过将到期时间设置为之前1个小时从而将其删除
		if(isset($_COOKIE[session_name()])){
			setcookie(session_name(),'',time()-3600);
		}
		//使用内置session_destroy()函数调用撤销会话
		session_destroy();

		echo json_encode(array('success'=>true));
	}
	else {
		echo json_encode(array('success'=>false));
	}
}

