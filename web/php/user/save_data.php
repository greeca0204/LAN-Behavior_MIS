<?php
header('Content-type:text/html;charset=utf-8');
require_once('../conn.php');

$Name = trim($_POST['Name']);
$Gid = trim($_POST['Gid']);
$Uid = trim($_POST['Uid']);
$IP = trim($_POST['User_IP']);
//$Password = trim($_POST['Password']);
$Email = trim($_POST['Email']);
//$CreateDate = date("Y-m-d H:i:s");
//$Flow = trim($_POST['Flow']);
//$Upload = trim($_POST['Upload']);
//$Download = trim($_POST['Download']);

//正则匹配
$preg_name = '/^\w*[!@#$%^&*()_+-]+\w*[!@#$%^&*()_+-]*/'; //匹配输入的数据是否含有特殊字符
$preg_num = '/^\d+$/'; //匹配输入的数据是否是数字
$preg_uid = '/^\d{1,20}$/';
$preg_ip = '/^\d+\.\d+\.\d+\.\d+$/';
//$preg_password = '/^.{6,20}$/';
$preg_email = '/^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$/';
//用preg_match进行正则表达式匹配，匹配成功返回1
$match_name = preg_match($preg_name, $Name);
$match_uid = preg_match($preg_uid, $Uid);
$match_ip = preg_match($preg_ip, $IP);
//$match_password = preg_match($preg_password, $Password);
if(!empty($Email)) {
	$match_email = preg_match($preg_email, $Email);
}
else {
	$match_email = 1;
}

/*
//当$Flow、$Upload、$Download为空时，$match_flow、$match_upload、$match_download都赋1
if($Flow == '') {
	$match_flow = 1;
}
else {
	$match_flow = preg_match($preg_num, $Flow);
}
if($Upload == '') {
	$match_upload = 1;
}
else {
	$match_upload = preg_match($preg_num, $Upload);
}
if($Download == '') {
	$match_download = 1;
}
else {
	$match_download = preg_match($preg_num, $Download);
}

if(!$match_name && strlen($Name) <= 20 && $match_uid && $match_ip && $match_email && $match_flow && $Flow >= 0 && $Flow <= 100000 && $match_upload && $Upload >= 0 && $Upload <= 100000 && $match_download && $Download >= 0 && $Download <= 100000) {
	$sql = "select ID from user where Uid like '$Uid' or User_IP like '$IP'";
	$result = mysql_query($sql);
	$row = mysql_fetch_row($result);

	if($row) {
		echo json_encode(array ('msg' => '该成员已存在!'));
	}
	else {
		//进行判断$Flow, $Upload, $Download有没有输入数据，空就不保存进数据库中。因为$Flow, $Upload, $Download的数据类型为int型，若将空值保存进表中，默认保存为0。
		if($Flow != '' && $Upload != '' && $Download != '') {
			$sql = "insert into user(Name, Gid, Uid, User_IP, Email, Flow, Upload, Download) values('$Name', '$Gid', '$Uid', '$IP', '$Email', '$Flow', '$Upload', '$Download')";
		}
		else if($Flow != '' && $Upload != '') {
			$sql = "insert into user(Name, Gid, Uid, User_IP, Email, Flow, Upload) values('$Name', '$Gid', '$Uid', '$IP', '$Email', '$Flow', '$Upload')";
		}
		else if($Flow != '' && $Download != '') {
			$sql = "insert into user(Name, Gid, Uid, User_IP, Email, Flow, Download) values('$Name', '$Gid', '$Uid', '$IP', '$Email', '$Flow',  '$Download')";
		}
		else if($Upload != '' && $Download != '') {
			$sql = "insert into user(Name, Gid, Uid, User_IP, Email, Upload, Download) values('$Name', '$Gid', '$Uid', '$IP', '$Email', '$Upload', '$Download')";
		}
		else if($Flow != '') {
			$sql = "insert into user(Name, Gid, Uid, User_IP, Email, Flow) values('$Name', '$Gid', '$Uid', '$IP', '$Email', '$Flow')";
		}
		else if($Upload != '') {
			$sql = "insert into user(Name, Gid, Uid, User_IP, Email, Upload) values('$Name', '$Gid', '$Uid', '$IP', '$Email', '$Upload')";
		}
		else if($Download != '') {
			$sql = "insert into user(Name, Gid, Uid, User_IP, Email, Download) values('$Name', '$Gid', '$Uid', '$IP', '$Email', '$Download')";
		}
		else {
			$sql = "insert into user(Name, Gid, Uid, User_IP,Email) values('$Name', '$Gid', '$Uid', '$IP', '$Email')";
		}
		$result = @mysql_query($sql);
		if($result) {
			echo json_encode(array ('success' => true));
		}
		else {
			echo json_encode(array ('msg' => '出错了！'));
		}
	}
}
*/
if(!$match_name && strlen($Name) <= 20 && $match_uid && $match_ip && $match_email) {
	$sql = "select ID from user where Uid like '$Uid' or User_IP like '$IP'";
	$result = mysql_query($sql);
	$row = mysql_fetch_row($result);


	if($row) {
		echo json_encode(array ('msg' => '该成员已存在!'));
	}
	else {
		$sql = "insert into user(Name, Gid, Uid, User_IP,Email) values('$Name', '$Gid', '$Uid', '$IP', '$Email')";	
		$result = @mysql_query($sql);
		if($result) {
			echo json_encode(array ('success' => true));
		}
		else {
			echo json_encode(array ('msg' => '出错了！'));
		}
	}
}
else {
	echo json_encode(array ('msg' => '出错了！'));
}

mysql_close($conn);
?>
