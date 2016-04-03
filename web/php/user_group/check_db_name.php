<?php
    header('content-type: text/html; charset=utf-8');
    require_once('../conn.php');

    $Name = $_GET['Name'];

    if($Name != NULL) {
        $sql = "select ID from user_group where Name like '$Name'";
        $result = @mysql_query($sql);
        $row = mysql_fetch_row($result);  //判断查询结果有没有记录

        if($row) {
            echo '该组名已经存在，请重新输入';
        }
        else {
            echo 'OK';
        }
    }
?>