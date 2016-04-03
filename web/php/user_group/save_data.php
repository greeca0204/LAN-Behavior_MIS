<?php
header('Content-type:text/html;charset=utf-8');
require_once('../conn.php');

$Name = trim($_POST['Name']);
$Gid = trim($_POST['Gid']);
//$Class = trim($_POST['Class']);
$Flow = trim($_POST['Flow']);
$Upload = trim($_POST['Upload']);
$Download = trim($_POST['Download']);
$Comment = trim($_POST['Comment']);

$preg_name = '/^\w*[!@#$%^&*()_+-]+\w*[!@#$%^&*()_+-]*/'; //匹配输入的数据是否含有特殊字符
$preg_num = '/^\d+$/'; //匹配输入的数据是否是数字
//用preg_match进行正则表达式匹配，匹配成功返回1
$match_name = preg_match($preg_name, $Name);
$match_gid = preg_match($preg_num, $Gid);

//先判断Name 和 Gid 的正则匹配
if($match_name || !$match_gid) { //如果正则匹配错误，返回false
	echo json_encode(array ('msg' => '请输入符合要求的数据'));
}
else { //正则匹配符合要求，继续下一步
/*
	//当$Flow、$Upload、$Download为空时，$match_flow、$match_upload、$match_download都赋1
	if($Flow == '') {
		$match_flow = 1;
	}
	else {
		$match_flow = preg_match($preg_num, $Flow);
	}

	if($Upload == '') {
		$match_upload = 1;
	}
	else {
		$match_upload = preg_match($preg_num, $Upload);
	}

	if($Download == '') {
		$match_download = 1;
	}
	else {
		$match_download = preg_match($preg_num, $Download);
	}

	//再判断输入各个输入框字符串的长度
	if(strlen($Name) <= 20 && strlen($Gid) <= 20 && $match_flow && $Flow >= 0 && $Flow <= 100000 && $match_upload && $Upload >= 0 && $Upload <= 100000 && $match_download && $Download >= 0 && $Download <= 100000) { //字符串长度符合要求，继续下一步
		$sql = "select ID from user_group where Name like '$Name' or Gid like '$Gid'";
		$result = mysql_query($sql);
		$row = mysql_fetch_row($result);

		//最后判断数据库中是否有与输入的Name或Gid相同
		if($row) { //数据库中已经有相同的Name或Gid，返回错误（Name和Gid是unique的)
			echo json_encode(array ('msg' => '该组已存在！'));
		}
		else { //数据在相库中不存同的数据，继续下一步
			//进行判断$Flow, $Upload, $Download有没有输入数据，空就不保存进数据库中。因为$Flow, $Upload, $Download的数据类型为int型，若将空值保存进表中，默认保存为0。
			if($Flow != '' && $Upload != '' && $Download != '') {
				$sql = "insert into user_group(Name, Gid, Flow, Upload, Download, Comment) values('$Name', '$Gid', '$Flow', '$Upload', '$Download', '$Comment')";
			}
			else if($Flow != '' && $Upload != '') {
				$sql = "insert into user_group(Name, Gid, Flow, Upload, Comment) values('$Name', '$Gid', '$Flow', '$Upload', '$Comment')";
			}
			else if($Flow != '' && $Download != '') {
				$sql = "insert into user_group(Name, Gid, Flow, Download, Comment) values('$Name', '$Gid', '$Flow', '$Download', '$Comment')";
			}
			else if($Upload != '' && $Download != '') {
				$sql = "insert into user_group(Name, Gid, Upload, Download, Comment) values('$Name', '$Gid', '$Upload', '$Download', '$Comment')";
			}
			else if($Flow != '') {
				$sql = "insert into user_group(Name, Gid, Flow, Comment) values('$Name', '$Gid', '$Flow', '$Comment')";
			}
			else if($Upload != '') {
				$sql = "insert into user_group(Name, Gid, Upload, Comment) values('$Name', '$Gid', '$Upload', '$Comment')";
			}
			else if($Download != '') {
				$sql = "insert into user_group(Name, Gid, Download, Comment) values('$Name', '$Gid', '$Download', '$Comment')";
			}
			else {
				$sql = "insert into user_group(Name, Gid, Comment) values('$Name', '$Gid', '$Comment')";
			}

			$result = @mysql_query($sql);
			if($result) {
				echo json_encode(array ('success' => true));
			}
			else {
				echo json_encode(array ('msg' => '出错了！'));
			}
		}
	}
*/
	if(strlen($Name) <= 20 && strlen($Gid) <= 20) {
		$sql = "select ID from user_group where Name like '$Name' or Gid like '$Gid'";
		$result = mysql_query($sql);
		$row = mysql_fetch_row($result);


		//最后判断数据库中是否有与输入的Name或Gid相同
		if($row) { //数据库中已经有相同的Name或Gid，返回错误（Name和Gid是unique的)
			echo json_encode(array ('msg' => '该组已存在！'));
		}

		else {
			$sql = "insert into user_group(Name, Gid, Comment) values('$Name', '$Gid', '$Comment')";
$result = @mysql_query($sql);
			if($result) {
				echo json_encode(array ('success' => true));
			}

			else {
				echo json_encode(array ('msg' => '出错了！'));
			}
		}
	}
	else { //字符串长度不符合要求，弹出错误
		echo json_encode(array ('msg' => '出错了！'));
	}
}


mysql_close($conn);
?>
