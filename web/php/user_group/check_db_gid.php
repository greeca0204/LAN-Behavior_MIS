<?php
    header('content-type: text/html; charset=utf-8');
    require_once('../conn.php');

    $Gid = trim($_GET['Gid']);

    if($Gid != NULL) {
        $sql = "select ID from user_group where Gid like '$Gid'";
        $result = mysql_query($sql);
        $row = mysql_fetch_row($result);

        if($row) {
            echo '该Gid已经存在';
        }
        else {
            echo 'OK';
        }
    }
?>