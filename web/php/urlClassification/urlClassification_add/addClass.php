<?php
header('content-type: text/html; charset=utf-8');
include('../../conn.php');
include('../../PinYin.php');

$Name = $_POST['Name'];
$Url = $_POST['Url'];

$rs = mysql_query("select * from app_table where Tactics_type like 'url' and Comment like '$Name'");
$array = mysql_fetch_array($rs);
if($array) {
	echo json_encode(array('msg'=>'该网页分类名已存在！'));
}
else {
	//将的分类名转换为拼音
	$NameEnglish = Pinyin($Name, 1);
	$NameEnglish = 'url_' . $NameEnglish;

	$rs = mysql_query("insert into app_table(Name, Tactics_type, Comment) values('$NameEnglish', 'url', '$Name')");
	if($rs) {
		$sql = "create table $NameEnglish
				 (
					 ID int not null auto_increment,
					 primary key(ID),
					 Url varchar(64) not null,
					 Gname varchar(32),
					 Name varchar(32),
					 Version int
				 )ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='$Name'";
		$result = mysql_query($sql);

		if($result) {
			//注意是 "\r\n" 而不是 ‘\r\n'，注意转义字符的特殊性
			$Url = "\r\n" . $Url . "\r\n";
			$find = "\r\n";
			$positionStart = strpos($Url, $find);
			$positionStart = $positionStart + strlen($find);
			$positionNext = strpos($Url, $find, $positionStart);
			//将url保存进表$NameEnglish中
			while($positionNext) {
				$urlLength = $positionNext-$positionStart;
				$url = substr($Url, $positionStart, $urlLength);    //截取每条url
				$Gname = $Name;
				$Comment = '';
				//$Add_date = date("Y-m-d H:i:s");    //调用系统时间
				$Version = 1;
				$sql = "insert into $NameEnglish(Url, Gname, Name, Version) values('$url', '$Gname', '$Comment', '$Version')";
				$result = mysql_query($sql);

				//继续获取下一条url
				$positionStart = $positionNext;
				$positionStart = $positionStart + strlen($find);
				$positionNext = strpos($Url, $find, $positionStart);
			}
			if($result) {
				echo json_encode(array('success'=>'true'));
			}
			else {
				echo json_encode(array('msg'=>'出错了！'));
			}
		}
		else {
			echo json_encode(array('msg'=>'出错了！'));
		}
	}
	else {
		echo json_encode(array('msg'=>"出错了！"));
	}
}

mysql_close($conn);
