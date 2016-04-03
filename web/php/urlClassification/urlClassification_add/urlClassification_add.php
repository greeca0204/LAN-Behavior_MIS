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
	<title>增加网页分类</title>
	<link type="text/css" rel="stylesheet" href="../../../js/easyui/themes/default/easyui.css">
	<link type="text/css" rel="stylesheet" href="../../../js/easyui/themes/icon.css">
	<link type="text/css" rel="stylesheet" href="../../../js/easyui/demo/demo.css">
	<link rel="stylesheet" type="text/css" href="../../../css/urlClassification_add.css">
	<script type="text/javascript" src="../../../js/easyui/jquery-1.8.0.min.js"></script>
	<script type="text/javascript" src="../../../js/easyui/jquery.easyui.min.js"></script>
	<script type="text/javascript" src="../../../js/easyui/locale/easyui-lang-zh_CN.js"></script>
	<script type="text/javascript" src="../../../js/urlClassification_add.js"></script>
</head>
<body>
<div class="easyui-layout" fit="true">
	<div data-options="region: 'center', border: false" style="background:#eee;">
		<form id="addForm" method="post" style="min-width: 1000px;">
			<table>
				<tr>
					<td>
						<div class="configure">
							<label for="Name"><span class="word">自定义分类名称</span></label>
							<input class="easyui-validatebox" id="Name" name="Name" required="true"/>
						</div>
					</td>
				</tr>
				<tr>
					<td width="750px">
						<div class="configure">
							<label for="Url">新增域名列表</label>
							<textarea rows="10" cols="50" id="Url" name="Url" style="vertical-align: top;"></textarea>
						</div>
					</td>
					<td width="200px">
						将列表中的域名，添加至该自定义分类中，每个域名一行，不需要包含“http://”。
					</td>
				</tr>
				<tr>
					<td>
						<div>
							<button type="button" id="submit">确定</button>
						</div>
					</td>
				</tr>
			</table>
		</form>
	</div>
</div>
</body>
<?php
}
?>