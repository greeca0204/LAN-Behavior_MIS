<?php
header('Content-Type: text/html; charset=UTF-8');
include('../conn.php');

$serverIP = trim($_POST['serverIP']);
$DBSERV_IP = trim($_POST['DBSERV_IP']);
$SUB_NET = trim($_POST['SUB_NET']);
$NET_interface = trim($_POST['NET_interface']);
$Gateway = trim($_POST['Gateway']);
$Fack_gateway_mac = trim($_POST['Fack_gateway_mac']);
$aduit_up = trim($_POST['aduit_up']);
$bbs_enable = trim($_POST['bbs_enable']);
$email_enable = trim($_POST['email_enable']);
$http_enable = trim($_POST['http_enable']);
$search_enable = trim($_POST['search_enable']);

$sql = "update system_configure set Config_data='$serverIP' where Config_name like 'serverIP'";
$result1 = mysql_query($sql);

$sql = "update system_configure set Config_data='$DBSERV_IP' where Config_name like 'DBSERV_IP'";
$result2 = mysql_query($sql);

$sql = "update system_configure set Config_data='$SUB_NET' where Config_name like 'SUB_NET'";
$result3 = mysql_query($sql);

$sql = "update system_configure set Config_data='$NET_interface' where Config_name like 'NET_interface'";
$result4 = mysql_query($sql);

$sql = "update system_configure set Config_data='$Gateway' where Config_name like 'Gateway'";
$result5 = mysql_query($sql);

$sql = "update system_configure set Config_data='$Fack_gateway_mac' where Config_name like 'Fack_gateway_mac'";
$result6 = mysql_query($sql);

$sql = "update system_configure set Config_data='$aduit_up' where Config_name like 'aduit_up'";
$result7 = mysql_query($sql);

$sql = "update system_configure set Config_data='$bbs_enable' where Config_name like 'bbs_enable'";
$result8 = mysql_query($sql);

$sql = "update system_configure set Config_data='$email_enable' where Config_name like 'email_enable'";
$result9 = mysql_query($sql);

$sql = "update system_configure set Config_data='$http_enable' where Config_name like 'http_enable'";
$result10 = mysql_query($sql);

$sql = "update system_configure set Config_data='$search_enable' where Config_name like 'search_enable'";
$result11 = mysql_query($sql);


if($result1 && $result2 && $result3 && $result4 && $result5 && $result6 && $result7 && $result8 && $result9 && $result10 && $result11) {
	echo json_encode(array ('success' => true));
}
else {
	echo json_encode(array ('success' => false));
}
mysql_close($conn);