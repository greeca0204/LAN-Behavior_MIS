<?php
header('content-type: text/html; charset=utf-8');
include('../../conn.php');

$q = isset($_POST['q']) ? trim($_POST['q']) : "";

if($q) {
	$rs = mysql_query("select Comment from app_table where Tactics_type like 'url' and Name not like 'all' and Comment like '%$q%'");
}
else {
	$rs = mysql_query("select Comment from app_table where Tactics_type like 'url' and Name not like 'all'");
}

$temp = array();
while($row = mysql_fetch_object($rs)) {
	array_push($temp, $row);
}

echo json_encode($temp);

mysql_close($conn);