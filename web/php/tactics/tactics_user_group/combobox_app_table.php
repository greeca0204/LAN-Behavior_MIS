<?php
header('Content-type:text/html; charset=utf-8');
require_once('../../conn.php');

$Tactics_type = $_GET['Tactics_type'];

//将tactics表的Comment字段值替换为tactics表的Tactics_type字段值；
$result = mysql_query("select Tactics_type from tactics where Comment like '$Tactics_type'");
$array = mysql_fetch_array($result);
$Tactics_type = $array['Tactics_type'];

$q = isset($_POST['q']) ? trim($_POST['q']) : '';
if($q) {
	$sql = "select Comment from app_table where Tactics_type like '$Tactics_type' and Comment like '%$q%'";
}
else {
	$sql = "select Comment from app_table where Tactics_type like '$Tactics_type'";
}
$result = @mysql_query($sql);

$items = array ();
while($row = mysql_fetch_array($result)) {
	array_push($items, $row);
}
echo json_encode($items);

mysql_close($conn);
?>