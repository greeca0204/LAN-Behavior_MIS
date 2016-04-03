<?php
session_start();
if(isset($_SESSION['ID'])) {
	//要清除会话变量，将$_SESSION超级全局变量设置为一个空数组
	$_SESSION = array();
	//如果存在一个会话cookie，通过将到期时间设置为之前1个小时从而将其删除
	if(isset($_COOKIE[session_name()])){
		setcookie(session_name(),'',time()-3600);
	}
	//使用内置session_destroy()函数调用撤销会话
	session_destroy();
}
echo '<script>window.location.href="../login/login.php"</script>';