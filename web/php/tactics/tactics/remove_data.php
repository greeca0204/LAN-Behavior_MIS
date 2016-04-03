<?php
    header('Content-type:text/html;charset=utf-8');
    require_once('../../conn.php');

    $ID = $_REQUEST['ID'];
    $count = count($ID);
    $num = 0;
    for($i = 0; $i < $count; $i++) {
        $sql = "delete from tactics where ID='$ID[$i]'";
        $result = @mysql_query($sql);
        $num++;

	    $id = $ID[$i];
	    $table = 13;
	    $action = 22;
	    include('../../protocolSecond.php');
    }

    if ($num == $count){
	    echo json_encode(array('success'=>true));
    }
    else {
	    echo json_encode(array('msg'=>'出错了！'));
    }

    mysql_close($conn);
?>