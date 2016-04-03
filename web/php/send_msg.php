<?php
header('Content-type:text/html;charset=gb2312');  //页面显示无乱码
set_time_limit(600); //设置10分钟的超时时间，避免资源浪费

function send_msg($in) {
	echo "PHP Socket\n";
	$ip = "202.192.44.69"; //服务端IP
	$port = 82; //服务端端口
	if(($socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) < 0) { //建立一个套接字
		echo "socket_create() failed: reason: " . socket_strerror($socket) . "\n";
	}
	else {
		echo "socket create success.\n";
	}
	echo "try connect '$ip' port '$port'...\n";
	if(($result = socket_connect($socket, $ip, $port)) < 0) { //连接服务端
		echo "socket_connect() failed.\nReason: ($result) " . socket_strerror($result) . "\n";
	}
	else {
		echo "connect success!!\n";
	}
//$in = "aaaa";//设置传入信息
	if(!socket_write($socket, $in, strlen($in))) { //通过一个socket传送数据到服务端
		echo "socket_write() failed: reason: " . socket_strerror($socket) . "\n";
	}
	else {
		echo "send message to server success!\n";
		echo "message is :" . $in . "\n";
	}
	echo "closing socket...\n";
//sleep(2000);  //睡眠2000秒
	socket_close($socket); //关闭套接字
	echo "close success!\n";
}
//$in = "sb";
//send_msg($in);
?>
