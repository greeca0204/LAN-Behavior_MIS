<?php
header('content-type: text/html; charset=utf-8');
include('../conn.php');
session_start();

echo date("Y-m-d H:i:s") . "\n";
//$_SESSION['ID'] = 1;
while(isset($_SESSION['ID'])) {
	$rs = mysql_query("select * from system_configure");
	while($row = mysql_fetch_array($rs)) {
		if(strcmp($row['Config_name'], 'IPStart1') == 0) {
			$IPStart1 = $row['Config_data'];
		}
		if(strcmp($row['Config_name'], 'IPEnd1') == 0) {
			$IPEnd1 = $row['Config_data'];
		}
		if(strcmp($row['Config_name'], 'IPStart2') == 0) {
			$IPStart2 = $row['Config_data'];
		}
		if(strcmp($row['Config_name'], 'IPEnd2') == 0) {
			$IPEnd2 = $row['Config_data'];
		}
		if(strcmp($row['Config_name'], 'IPStart3') == 0) {
			$IPStart3 = $row['Config_data'];
		}
		if(strcmp($row['Config_name'], 'IPEnd3') == 0) {
			$IPEnd3 = $row['Config_data'];
		}
	}

	$findPoint = '.'; //查找IP地址的圆点位置
	$findTTL = 'TTL';
	$num = 0; //在线用户数量
	$arrayIP = array (); //定义一个空数组，存放在线用户的IP
	$j = 0; //数组元素ID
	//第一个IP网段
	if(!empty($IPStart1) && !empty($IPEnd1)) {
		//IP池的开始地址
		$position = strpos($IPStart1, $findPoint); //IP:192.168.100 先找第一个点的位置
		$position = strpos($IPStart1, $findPoint, $position + 1); //找第二个点的位置
		$position = strpos($IPStart1, $findPoint, $position + 1); //找第三个点的位置
		$lenAll = strlen($IPStart1); //IP地址的总长度
		$len = $lenAll - $position - 1; //第三个点后面的IP长度
		$IP = substr($IPStart1, 0, $position + 1); //第三个点前面的IP长度
		$start = substr($IPStart1, $position + 1, $len); //第三个点后面的IP长度
		//IP池的结束地址
		$position = strpos($IPEnd1, $findPoint);
		$position = strpos($IPEnd1, $findPoint, $position + 1);
		$position = strpos($IPEnd1, $findPoint, $position + 1);
		$lenAll = strlen($IPEnd1);
		$len = $lenAll - $position - 1;
		$IP = substr($IPEnd1, 0, $position + 1);
		$end = substr($IPEnd1, $position + 1, $len);

		for($i = $start; $i <= $end; $i++) {
			exec("ping -n 1 -s 1 " . $IP . $i . " 2>&1", $list, $return_var); //获取ping后的结果，放在$list数组中, 2>$1:管道命令，输出错误信息
			$list = implode($list); //$list: 由数组转换为字符串
			$list = iconv('gb2312', 'utf-8', $list); //编码转换

			//在ping后返回的数据中，如果存在TTL,则表明ping成功
			$rs = strpos($list, $findTTL);
			if($rs) {
				$num++;
				$arrayIP[$j++] = $IP . $i;
			}
		}
	}
	//第二个IP网段
	if(!empty($IPStart2) && !empty($IPEnd2)) {
		//IP池的开始地址
		$position = strpos($IPStart2, $findPoint); //IP:192.168.100 先找第一个点的位置
		$position = strpos($IPStart2, $findPoint, $position + 1); //找第二个点的位置
		$position = strpos($IPStart2, $findPoint, $position + 1); //找第三个点的位置
		$lenAll = strlen($IPStart2); //IP地址的总长度
		$len = $lenAll - $position - 1; //第三个点后面的IP长度
		$IP = substr($IPStart2, 0, $position + 1); //第三个点前面的IP长度
		$start = substr($IPStart2, $position + 1, $len); //第三个点后面的IP长度
		//IP池的结束地址
		$position = strpos($IPEnd2, $findPoint);
		$position = strpos($IPEnd2, $findPoint, $position + 1);
		$position = strpos($IPEnd2, $findPoint, $position + 1);
		$lenAll = strlen($IPEnd2);
		$len = $lenAll - $position - 1;
		$IP = substr($IPEnd2, 0, $position + 1);
		$end = substr($IPEnd2, $position + 1, $len);

		for($i = $start; $i <= $end; $i++) {
			exec("ping -n 1 -s 1 " . $IP . $i, $list); //获取ping后的结果，放在$list数组中
			$list = implode($list); //$list: 由数组转换为字符串
			$list = iconv('gb2312', 'utf-8', $list); //编码转换

			$rs = strpos($list, $findTTL);
			if($rs) {
				$num++;
				$arrayIP[$j++] = $IP . $i;
			}
		}
	}
	//第三个IP网段
	if(!empty($IPStart3) && !empty($IPEnd3)) {
		//IP池的开始地址
		$position = strpos($IPStart3, $findPoint); //IP:192.168.100 先找第一个点的位置
		$position = strpos($IPStart3, $findPoint, $position + 1); //找第二个点的位置
		$position = strpos($IPStart3, $findPoint, $position + 1); //找第三个点的位置
		$lenAll = strlen($IPStart3); //IP地址的总长度
		$len = $lenAll - $position - 1; //第三个点后面的IP长度
		$IP = substr($IPStart3, 0, $position + 1); //第三个点前面的IP长度
		$start = substr($IPStart3, $position + 1, $len); //第三个点后面的IP长度
		//IP池的结束地址
		$position = strpos($IPEnd3, $findPoint);
		$position = strpos($IPEnd3, $findPoint, $position + 1);
		$position = strpos($IPEnd3, $findPoint, $position + 1);
		$lenAll = strlen($IPEnd3);
		$len = $lenAll - $position - 1;
		$IP = substr($IPEnd3, 0, $position + 1);
		$end = substr($IPEnd3, $position + 1, $len);

		for($i = $start; $i <= $end; $i++) {
			exec("ping -n 1 -s 1 " . $IP . $i, $list); //获取ping后的结果，放在$list数组中
			$list = implode($list); //$list: 由数组转换为字符串
			$list = iconv('gb2312', 'utf-8', $list); //编码转换

			$rs = strpos($list, $findTTL);
			if($rs) {
				$num++;
				$arrayIP[$j++] = $IP . $i;
			}
		}
	}
	echo $num . "\n";
	$count = count($arrayIP);
	$rs = mysql_query("update user set Online=0");
	for($i = 0; $i < $count; $i++) {
		$rs = mysql_query("update user set Online=1 where User_IP like '$arrayIP[$i]'");
	}
	echo date("Y-m-d H:i:s");

	flush();
	sleep(30); //180秒后再次运行
}


