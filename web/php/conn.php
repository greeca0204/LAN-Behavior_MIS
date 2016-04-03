<?php
header('Content-type:text/html; charset=utf-8');

//myself db
$conn = @mysql_connect('localhost', 'root', '');  //$conn=@mysql_connect('localhost', 'mysql用户名'，'mysql密码');
if(!$conn)
{
	die('Could not connected:' . mysql_error());
}
mysql_select_db('8-30', $conn);  //mysql_select_db('数据库名称', $conn);
mysql_query('SET NAMES UTF8');    //防止乱码的生成

?>
