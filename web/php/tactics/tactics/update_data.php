<?php
    header('Content-type:text/html;charset=utf-8');
    require_once('../../conn.php');

    $ID = intval($_REQUEST['ID']);
    //$Name = trim($_POST['Tactics_type']);
    $Enable = trim($_POST['Enable']);
    //$Tactics_type = trim($_POST['Tactics_type']);

	$sql = "update tactics set Enable='$Enable' where ID='$ID'";
	$result = @mysql_query($sql);
	if ($result){
		//通知后台，管理员对数据库的操作
		$id = $ID;
		$table = 13;
		$action = 33;
		include('../../protocolSecond.php');

		echo json_encode(array('success'=>true));
	}
	else {
		echo json_encode(array('msg'=>'出错了！'));
	}

    mysql_close($conn);
?>