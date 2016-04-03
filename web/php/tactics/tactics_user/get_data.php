<?php
header('Content-type:text/html;charset=utf-8');
require_once('../../conn.php');

$page = isset($_POST['page']) ? intval($_POST['page']) : 1;
$rows = isset($_POST['rows']) ? intval($_POST['rows']) : 10;
$sort = isset($_POST['sort']) ? strval($_POST['sort']) : 'Uid';
$order = isset($_POST['order']) ? strval($_POST['order']) : 'asc';
$offset = ($page - 1) * $rows;
$result = array ();

//获取搜索条件
$searchName = isset($_POST['searchName']) ? mysql_real_escape_string(trim($_POST['searchName'])) : '';
$searchUid = isset($_POST['searchUid']) ? mysql_real_escape_string(trim($_POST['searchUid'])) : '';
$searchIP = isset($_POST['searchIP']) ? mysql_real_escape_string(trim($_POST['searchIP'])) : '';
$searchTactics = isset($_POST['searchTactics']) ? mysql_real_escape_string(trim($_POST['searchTactics'])) : '';
$searchDetail = isset($_POST['searchDetail']) ? mysql_real_escape_string(trim($_POST['searchDetail'])) : '';
$searchEnable = isset($_POST['searchEnable']) ? mysql_real_escape_string(trim($_POST['searchEnable'])) : '';

//判断$searchEnable是否为空值
if($searchEnable == '') {
	$rs = mysql_query("select count(*) from tactics_user where Name like '%$searchName%' and Uid like '%$searchUid%' and User_IP like '%$searchIP%' and Tactics_type in (select Tactics_type from tactics where Comment like '%$searchTactics%') and (Detail like '%$searchDetail%' or Detail in (select Name from app_table where Comment like '%$searchDetail%'))");
	$row = mysql_fetch_row($rs);
	$result["total"] = $row[0];
	$rs = mysql_query("select * from tactics_user where Name like '%$searchName%' and Uid like '%$searchUid%' and User_IP like '%$searchIP%' and Tactics_type in (select Tactics_type from tactics where Comment like '%$searchTactics%') and (Detail like '%$searchDetail%' or Detail in (select Name from app_table where Comment like '%$searchDetail%')) order by $sort $order limit $offset,$rows");
}
else {
	$rs = mysql_query("select count(*) from tactics_user where Name like '%$searchName%' and Uid like '%$searchUid%' and User_IP like '%$searchIP%' and Tactics_type in (select Tactics_type from tactics where Comment like '%$searchTactics%') and (Detail like '%$searchDetail%' or Detail in (select Name from app_table where Comment like '%$searchDetail%')) and Enable like '$searchEnable'");
	$row = mysql_fetch_row($rs);
	$result["total"] = $row[0];
	$rs = mysql_query("select * from tactics_user where Name like '%$searchName%' and Uid like '%$searchUid%' and User_IP like '%$searchIP%' and Tactics_type in (select Tactics_type from tactics where Comment like '%$searchTactics%') and (Detail like '%$searchDetail%' or Detail in (select Name from app_table where Comment like '%$searchDetail%')) and Enable like '$searchEnable' order by $sort $order limit $offset,$rows");
}

$items = array ();
while($row = mysql_fetch_object($rs)) {
	//将tactics_allow表中Tactics_type字段值替换为tactics表Comment字段的值；
	$sql = "select Comment from tactics where Tactics_type like '$row->Tactics_type'";
	$result2 = mysql_query($sql);
	$array = mysql_fetch_array($result2); //mysql_fetch_array速度上比mysql_fetch_object快
	$row->Tactics_type = $array['Comment'];

	if($row->Enable == 1) {  //将1和0转换为“是”和“否”
		$row->Enable = '是';
	}
	else {
		$row->Enable = '否';
	}

	//将Detail显示内容转换为ap_table字段Comment内容
	$array = mysql_fetch_array(mysql_query("select Comment from app_table where Name like '$row->Detail'"));
	if($array) {
		$row->Detail = $array['Comment'];
	}

	array_push($items, $row);
}
$result["rows"] = $items;
echo json_encode($result);

mysql_close($conn);
?>