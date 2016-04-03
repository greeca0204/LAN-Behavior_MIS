<?php
header('Content-type:text/html;charset=utf-8');
require_once('../../conn.php');

/*$q = isset($_POST['q']) ? trim($_POST['q']) : '';
//显示tactics表Comment字段值；
if($q) {
	$sql = "select Comment from tactics where Enable=1 and Comment like '%$q%'";
} else {
	$sql = "select Comment from tactics where Enable=1";
}*/
$sql = "select Comment from tactics where Enable=1";
$rs = @mysql_query($sql);

$items = array ();
while($row = mysql_fetch_object($rs)) {
	array_push($items, $row);
}

echo json_encode($items);

mysql_close($conn);
?>
