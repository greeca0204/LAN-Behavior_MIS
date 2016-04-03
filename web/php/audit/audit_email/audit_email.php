<?php
session_start();
if(!isset($_SESSION['ID'])) {
	echo '<meta http-equiv="Refresh" content="0, url=../../../login/login.php">';
}
else {
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<title>邮件发送审计</title>
	<link rel="stylesheet" type="text/css" href="../../../js/easyui/themes/default/easyui.css">
	<link rel="stylesheet" type="text/css" href="../../../js/easyui/themes/icon.css">
	<link rel="stylesheet" type="text/css" href="../../../js/easyui/demo/demo.css">
	<link type="text/css" rel="stylesheet" href="../../../css/search.css">
	<script type="text/javascript" src="../../../js/easyui/jquery-1.8.0.min.js"></script>
	<script type="text/javascript" src="../../../js/easyui/jquery.easyui.min.js"></script>
	<script type="text/javascript" src="../../../js/easyui/locale/easyui-lang-zh_CN.js"></script>
	<script type="text/javascript" src="../../../js/easyui/extends/datagrid-detailview.js"></script>
	<script type="text/javascript" src="../../../js/audit_email.js"></script>
</head>
<body>
<div class="easyui-layout" fit="true">
	<div data-options="region: 'north', title: '查询条件', border: false" style="height: 65px">
		<div id="p" style="background: #eee; min-width: 1100px; overflow-y: hidden;">
			<form id="pForm">
				<table>
					<tr>
						<td>
							<label for="searchName">员工名</label>
							<input id="searchName" class="search" onFocus="onClickSelect()"/>
						</td>
						<td>
							<label for="searchUid">工号</label>
							<input id="searchUid" class="search" onFocus="onClickSelect()"/>
						</td>
						<td>
							<label for="searchIP">IP地址</label>
							<input id="searchIP" class="search" onFocus="onClickSelect()"/>
						</td>
					</tr>
				</table>
			</form>
		</div>
	</div>
	<div data-options="region: 'center', border: false" style="background:#eee;">
		<table id="dg"></table>
		<div class="easyui-dialog" id="dd">
			<p id="emailData" style="font-size: 20px;"></p>
		</div>
	</div>

</div>
</body>
</html>
<?php
}
?>