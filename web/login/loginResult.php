<?php
    header('Content-Type: text/html; charset=UTF-8');
    require_once('../php/conn.php');

	session_start();

    $User = strtolower($_POST['user']);  //将User全转为小写
    $Password = md5($_POST['password']);  //对密码进行md5()加密

    $sql = "select ID from login where User like '$User'";
    $result = mysql_query($sql);
    $row = mysql_fetch_row($result);

    if(!$row) {
        echo json_encode(array('success'=>false));
    }
    else {
        $sql = "select * from login where ID='$row[0]' and Password like '$Password'";
        $result = mysql_query($sql);
        $row = mysql_fetch_array($result);
        if(!$row) {
            echo json_encode(array('success'=>false));
        }
        else {
	        $_SESSION['ID'] = $row['ID'];
	        $_SESSION['User'] = $row['User'];
            echo json_encode(array('success'=>true));
            //echo '<meta http-equiv="Refresh" content="0, url=../index.html">';
        }
    }
    mysql_close($conn);
?>