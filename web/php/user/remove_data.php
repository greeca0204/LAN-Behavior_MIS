<?php
header('Content-type:text/html;charset=utf-8');
require_once('../conn.php');

$ID = $_POST['ID'];
$num = 0;
$count = count($ID);

for($i = 0; $i < $count; $i++) {
	$sql = "delete from user where ID='$ID[$i]'";
	$result = @mysql_query($sql);
	$num++;
}

if($num == $count) {
	echo json_encode(array ('success' => true));
}
else {
	echo json_encode(array ('msg' => '出错了！'));
}

mysql_close($conn);
?>