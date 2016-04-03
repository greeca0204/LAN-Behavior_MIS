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
	<title>总策略</title>
	<link rel="stylesheet" type="text/css" href="../../../js/easyui/themes/default/easyui.css">
	<link rel="stylesheet" type="text/css" href="../../../js/easyui/themes/icon.css">
	<link rel="stylesheet" type="text/css" href="../../../js/easyui/demo/demo.css">
	<link type="text/css" rel="stylesheet" href="../../../css/jquery_easyui.css">
	<link type="text/css" rel="stylesheet" href="../../../css/tactics.css">
	<link type="text/css" rel="stylesheet" href="../../../css/search.css">
	<script type="text/javascript" src="../../../js/easyui/jquery-1.8.0.min.js"></script>
	<script type="text/javascript" src="../../../js/easyui/jquery.easyui.min.js"></script>
	<script type="text/javascript" src="../../../js/easyui/locale/easyui-lang-zh_CN.js"></script>
	<script type="text/javascript" src="../../../js/jquery_easyui.js"></script>
	<script type="text/javascript" src="../../../js/tactics.js"></script>
</head>
<body>
<div class="easyui-layout" fit="true">
	<div data-options="region: 'north', title: '查询条件', border: false" style=" height: 65px;">
		<!--搜索条件-->
		<div class="easyui-panel" id="p" data-options="fit: true, border: false" style="background: #eee; min-width: 1100px;">
			<form id="pForm">
				<table>
					<tr>
						<td >
							<div style="margin-left: 200px;">
								<label for="searchTactics">策略类型</label>
								<input id="searchTactics" class="search" onFocus="onClickSelect()"/>
							</div>
						</td>
						<td >
							<div>
								<label>启用</label>
								<input type="radio" class="radio" name="searchEnable" value="1"/>1
								<input type="radio" class="radio" name="searchEnable" value="0"/>0
								<input type="radio" class="radio" name="searchEnable" value="" checked="checked"/>不选
							</div>
						</td>
						<td></td>
					</tr>
				</table>
			</form>
		</div>
	</div>
	<div data-options="region: 'center', border: false">
		<table id="dg">
			<thead>
			<tr>
				<th field="ck" checkbox="true"></th>
				<th field="Comment" width="50" sortable="true" align="center">策略类型</th>
				<th field="Enable" width="50" sortable="true" align="center">启用</th>
				<th field="Tactics_type" width="50" sortable="true" align="center">注释</th>
			</tr>
			</thead>
		</table>
		<div id="toolbar">
			<a href="#" class="easyui-linkbutton" iconCls="icon-edit" plain="true" title="编辑" onclick="editUser()">编辑</a>
			<!--<a href="#" class="easyui-linkbutton" iconCls="icon-remove" plain="true" title="删除" onclick="removeUser()">删除</a>-->
			<a href="#"  class="easyui-linkbutton" iconCls="icon-search" plain="true" title="查询条件" onclick="doSearch()">查询条件</a>
			<a href="#" class="easyui-linkbutton" iconCls="icon-back" plain="true" title="清空条件" onclick="back()">清空条件</a>
		</div>
	</div>
</div>

<div id="dlg" class="easyui-dialog" style="width:570px;height:280px;padding:10px 20px"
     closed="true" buttons="#dlg-buttons" modal="true">
	<div class="ftitle">基本信息</div>
	<form id="fm" method="post" novalidate>
		<ul>
			<li>
				<div class="fitem">
					<label class="label">策略类型:</label>
					<input id="Tactics_type" name="Comment" class="easyui-validatebox" onfocus="onClickSelect()" onkeyup="check_name(this.value)" onblur="check_db_name()" disabled="false"/> <!--easyui-validatebox：当text发生变化时进行校验-->
				</div>
				<div class="msg">
					<p id="error1">测试</p>
				</div>
			</li>
			<li>
				<div class="fitem">
					<label class="label">启用:</label>
					<!--radio: name属性要一样才能实现单选功能-->
					<input type="radio" class="radio" name="Enable" value="1" checked="checked"/>1
					<input type="radio" class="radio" name="Enable" value="0"/>0
				</div>
				<div class="msg">
					<p id="error2">请选择：1：激活，0：不激活</p>
				</div>
			</li>
			<li>
				<div class="fitem">
					<label class="label">注释:</label>
					<input name="Tactics_type" class="easyui-validatebox" type="text" onfocus="onClickSelect()" disabled="false"/>
				</div>
			</li>
		</ul>
	</form>
</div>
<div id="dlg-buttons">
	<a href="#" class="easyui-linkbutton" iconCls="icon-ok" onclick="saveUser()">确定</a>
	<a href="#" class="easyui-linkbutton" iconCls="icon-cancel" onclick="javascript:$('#dlg').dialog('close')">取消</a>
</div>
</body>
</html>
<?php
}
?>