<?php
header('Content-type:text/html;charset=utf-8');
include('../../conn.php');

$ID = $_POST['ID'];

$Comment = $_POST['Comment'];
$rs = mysql_query("select Name from app_table where Comment like '$Comment'");
$array = mysql_fetch_array($rs);
$tableName = $array['Name'];

$count = count($ID);
$num = 0;

for($i = 0; $i < $count; $i++) {
	//删除选中的记录
	$sql = "delete from $tableName where ID='$ID[$i]'";
	$result = @mysql_query($sql);
	$num++;
}
if($num == $count) {
	echo json_encode(array ('success' => true));
} else {
	echo json_encode(array ('msg' => '出错了！'));
}

mysql_close($conn);
?>