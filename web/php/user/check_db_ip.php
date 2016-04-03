<?php
    header('content-type: text/html; charset=utf-8');
    require_once('../conn.php');

    $IP = trim($_GET['IP']);

    if($IP != NULL) {
        $sql = "select ID from user where User_IP like '$IP'";
        $result = mysql_query($sql);
        $row = mysql_fetch_row($result);

        if($row) {
            echo "该IP已经存在";
        }
        else {
            echo "OK";
        }
    }
?>