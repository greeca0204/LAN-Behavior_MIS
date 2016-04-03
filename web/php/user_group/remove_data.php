<?php
header('Content-type:text/html;charset=utf-8');
require_once('../conn.php');

$ID = $_POST['ID']; //$_POST['ID']是获取ID数组，所以$ID为数组
$num = 0; //标志成功删除记录的个数
$count = count($ID); //计算数组的元素个数

for($i = 0; $i < $count; $i++) {
	$sql = "delete from user_group where ID='$ID[$i]'"; //与user_group表有关的外键属性设置为cascade.所以当user_group删除某组时，会级联删除子表的数据
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
