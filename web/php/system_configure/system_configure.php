<?php
include('../conn.php');
session_start();
if(!isset($_SESSION['ID'])) {
	echo '<meta http-equiv="Refresh" content="0, url=../../">';
}
else {
	?>
<html>
<head>
	<meta http-equiv="Content-type" content="text/html; charset=utf-8">
	<title>系统配置</title>
	<link type="text/css" rel="stylesheet" href="../../js/easyui/themes/default/easyui.css">
	<link type="text/css" rel="stylesheet" href="../../js/easyui/themes/icon.css">
	<link type="text/css" rel="stylesheet" href="../../js/easyui/demo/demo.css">
	<link rel="stylesheet" type="text/css" href="../../css/system_congfigure.css">
	<script type="text/javascript" src="../../js/easyui/jquery-1.8.0.min.js"></script>
	<script type="text/javascript" src="../../js/easyui/jquery.easyui.min.js"></script>
	<script type="text/javascript" src="../../js/easyui/locale/easyui-lang-zh_CN.js"></script>
	<script type="text/javascript" src="../../js/My97DatePicker/WdatePicker.js"></script>
	<script type="text/javascript" src="../../js/system_configure.js"></script>
</head>
<body>
<div class="easyui-layout" fit="true">
	<div data-options="region: 'center', border: false">
		<form id="configureForm" method="post" style="">
			<table>
				<tr>
					<td>
						<div class="configure">
							<label for="serverIP"><span class="word">服务器IP</span></label>
							<input id="serverIP" name="serverIP" value="<?php
								$result = mysql_query("select Config_data from system_configure where Config_name like 'serverIP'");
								$array = mysql_fetch_array($result);
								echo $array['Config_data'];
								?>"/>
						</div>
						<div class="msg">
							<p id="tips1">提示</p>
						</div>
					</td>
				</tr>
				<tr>
					<td>
						<div class="configure">
							<label for="DBSERV_IP"><span class="word">数据库IP</span></label>
							<input id="DBSERV_IP" name="DBSERV_IP" value="<?php
								$result = mysql_query("select Config_data from system_configure where Config_name like 'DBSERV_IP'");
								$array = mysql_fetch_array($result);
								echo $array['Config_data'];
								?>"/>
						</div>
						<div class="msg">
							<p id="tips3">提示</p>
						</div>
					</td>
				</tr>
				<tr>
					<td>
						<div class="configure">
							<label for="SUB_NET">监听网段</label>
							<input id="SUB_NET" name="SUB_NET" value="<?php
								$result = mysql_query("select Config_data from system_configure where Config_name like 'SUB_NET'");
								$array = mysql_fetch_array($result);
								echo $array['Config_data'];
								?>"/>
						</div>
						<div class="msg">
							<p id="tips5">提示</p>
						</div>
					</td>
				</tr>
				<tr>
					<td>
						<div class="configure">
							<label for="NET_interface">网卡</label>
							<input id="NET_interface" name="NET_interface" value="<?php
								$result = mysql_query("select Config_data from system_configure where Config_name like 'NET_interface'");
								$array = mysql_fetch_array($result);
								echo $array['Config_data'];
								?>"/>
						</div>
					</td>
				</tr>
				<tr>
					<td>
						<div class="configure">
							<label for="Gateway">网关IP</label>
							<input id="Gateway" name="Gateway" value="<?php
								$result = mysql_query("select Config_data from system_configure where Config_name like 'Gateway'");
								$array = mysql_fetch_array($result);
								echo $array['Config_data'];
								?>"/>
						</div>
						<div class="msg">
							<p id="tips4">提示</p>
						</div>
					</td>
				</tr>
				<tr>
					<td>
						<div class="configure">
							<label for="Fack_gateway_mac">虚假网关MAC</label>
							<textarea rows="2" id="Fack_gateway_mac" name="Fack_gateway_mac" style="vertical-align: middle;"><?php
								$result = mysql_query("select Config_data from system_configure where Config_name like 'Fack_gateway_mac'");
								$array = mysql_fetch_array($result);
								echo $array['Config_data'];
								?></textarea>
						</div>
						<div class="msg">
							<p id="tips6">提示</p>
						</div>
					</td>
				</tr>
				<tr>
					<td>
						<div class="configure">
							<label>审计功能</label>
							<input type="radio" name="aduit_up" value="1" class="radio" <?php
								$result = mysql_query("select Config_data from system_configure where Config_name like 'aduit_up'");
								$array = mysql_fetch_array($result);
								if($array['Config_data'] == 1) {
									echo "checked";
								}
								?>/>启用
							<input type="radio" name="aduit_up" value="0" class="radio" <?php
								if($array['Config_data'] == 0) {
									echo "checked";
								}
								?>/>停用
						</div>
					</td>
				</tr>
				<tr>
					<td>
						<div>
							<button type="button" id="submit">保存</button>
						</div>
					</td>
				</tr>
			</table>
		</form>
	</div>
</div>
</body>
</html>
<?php
}
?>