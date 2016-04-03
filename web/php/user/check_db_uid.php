<?php
    header('content-type: text/html; charset=utf-8');
    require_once('../conn.php');

    $Uid = trim($_GET['Uid']);

    if($Uid != NULL) {
        $sql = "select ID from user where Uid like '$Uid'";
        $result = mysql_query($sql);
        $row = mysql_fetch_row($result);

        if($row) {
            echo "该Uid已经存在";
        }
        else {
            echo "OK";
        }
    }
?>