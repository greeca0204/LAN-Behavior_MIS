<?php
    require_once('../conn.php');
    $sql = "select Gid from user_group";    //ID和Gid 连接html的vauleField和TextField
    $rs = mysql_query($sql);

    $items = array();
    while($row = mysql_fetch_object($rs)){
        array_push($items, $row);
    }
    echo json_encode($items);

    mysql_close($conn);
?>
