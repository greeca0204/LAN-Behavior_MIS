<?php
    header('Content-type:text/html;charset=utf-8');
    require_once('../../conn.php');

    //q：是从前台的combobox上赋值的数据
    $q = isset($_POST['q']) ? trim($_POST['q']) : "";
    if($q) {
        $sql = "select Gid, Name from user_group where (Name like '%$q%' or Gid like '%$q%')";
    }
    else {
        $sql = "select Gid, Name from user_group";
    }
    $rs = @mysql_query($sql);

    $items = array();
    while($row = mysql_fetch_array($rs))
    {
        array_push($items, $row);
    }
    echo json_encode($items);

    mysql_close($conn);
?>