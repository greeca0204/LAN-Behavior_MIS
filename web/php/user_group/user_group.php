<?php
session_start();
if(!isset($_SESSION['ID'])) {
	echo '<meta http-equiv="Refresh" content="0, url=../../login/login.php">';
}
else {
	?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<title>添加组</title>
	<link rel="stylesheet" type="text/css" href="../../js/easyui/themes/default/easyui.css">
	<link rel="stylesheet" type="text/css" href="../../js/easyui/themes/icon.css">
	<link rel="stylesheet" type="text/css" href="../../js/easyui/demo/demo.css">
	<link type="text/css" rel="stylesheet" href="../../css/jquery_easyui.css">
	<link type="text/css" rel="stylesheet" href="../../css/user_group.css">
	<link type="text/css" rel="stylesheet" href="../../css/search.css">
	<script type="text/javascript" src="../../js/easyui/jquery-1.8.0.min.js"></script>
	<script type="text/javascript" src="../../js/easyui/jquery.easyui.min.js"></script>
	<script type="text/javascript" src="../../js/easyui/locale/easyui-lang-zh_CN.js"></script>
	<script type="text/javascript" src="../../js/jquery_easyui.js"></script>
	<script type="text/javascript" src="../../js/user_group.js"></script>
</head>
<body>
<div class="easyui-layout" fit="true">
	<div data-options="region: 'north', title:'查询条件', border: false" style="height: 65px;">
		<!--搜索条件-->
		<div class="easyui-panel" id="p" style="background: #eee; min-width: 1100px; overflow-y: hidden;" data-options="fit: true, border: false">
			<!--代码在get_data.php中实现-->
			<form id="pForm">
				<table>
					<tr>
						<td>
							<label for="searchName">组名</label>
							<input id="searchName" class="search" onFocus="onClickSelect()"/>
						</td>
						<td>
							<label for="searchGid">组号</label>
							<input id="searchGid" class="search" onFocus="onClickSelect()"/>
						</td>
					</tr>
				</table>
			</form>
		</div>
	</div>
	<div data-options="region: 'center', border: false" style="background: #eee">
		<table id="dg">
			<thead>
			<tr>
				<th field="ck" checkbox="true"></th>
				<th field="Name" width="100" sortable="true" align="center">组名</th>
				<th field="Gid" width="100" sortable="true" align="center">组号</th>
<!--
				<th field="Flow" width="100" align="center" sortable="true">流量</th>
				<th field="Upload" width="100" align="center" sortable="true">上传</th>
				<th field="Download" width="100" align="center" sortable="true">下载</th>
-->
				<th field="Comment" width="120" align="center">注释</th>
			</tr>
			</thead>
		</table>
		<div id="toolbar">
			<!--plain="true"设置为true将按钮显示简洁效果,少了外观 。-->
			<a href="#" class="easyui-linkbutton" iconCls="icon-add" plain="true" title="添加" onclick="newUser()">添加</a>
			<a href="#" class="easyui-linkbutton" iconCls="icon-edit" plain="true" title="编辑" onclick="editUser()">编辑</a>
			<a href="#" class="easyui-linkbutton" iconCls="icon-remove" plain="true" title="删除" onclick="removeUser()">删除</a>
			<a href="#" class="easyui-linkbutton" iconCls="icon-undo" plain="true" title="取消选中" onclick="unSelectAll()">取消选中</a>
			<a href="#"  class="easyui-linkbutton" iconCls="icon-search" plain="true" title="查询条件" onclick="doSearch()">查询条件</a>
			<a href="#" class="easyui-linkbutton" iconCls="icon-back" plain="true" title="清空条件" onclick="back()">清空条件</a>
		</div>
	</div>
</div>

<div id="dlg" class="easyui-dialog"
     closed="true" buttons="#dlg-buttons" modal="true">
	<div class="ftitle">基本信息</div>
	<form id="fm" method="post" novalidate>
		<ul>
			<li>
				<div class="fitem">
					<label class="label">组名:</label>
					<input name="Name" class="easyui-validatebox" required="true" onkeyup="check_name(this.value)" onFocus="onClickSelect()" onBlur="check_db_name(this.value)"/> <!--easyui-validatebox：当text发生变化时进行校验-->
				</div>
				<div class="msg">
					<p id="error1">测试</p>
				</div>
			</li>
			<li>
				<div class="fitem">
					<label class="label">组号:</label>
					<input id="Gid" name="Gid" class="easyui-validatebox" required="true" onkeyup="check_gid(this.value)" onFocus="onClickSelect()" onBlur="check_db_gid(this.value)" />
				</div>
				<div class="msg">
					<p id="error2">测试</p>
				</div>
			</li>
<!--
			<li>
				<div class="fitem">
					<label class="label">流量:</label>
					<input name="Flow" onFocus="onClickSelect()" onkeyup="check_flow(this.value)"/>
				</div>
				<div class="msg">
					<p id="error3">测试</p>
				</div>
			</li>
			<li>
				<div class="fitem">
					<label class="label">上传:</label>
					<input name="Upload" onFocus="onClickSelect()" onkeyup="check_upload(this.value)"/>
				</div>
				<div class="msg">
					<p id="error4">测试</p>
				</div>
			</li>
			<li>
				<div class="fitem">
					<label class="label">下载:</label>
					<input name="Download" onFocus="onClickSelect()" onkeyup="check_download(this.value)" onkeyup="this.value=this.value.replace(/\D/g,'')" onafterpaste="this.value=this.value.replace(/\D/g,'')"/>
				</div>
				<div class="msg">
					<p id="error5">测试</p>
				</div>
			</li>
-->
			<li>
				<div class="fitem">
					<label class="label">注释:</label>
					<input name="Comment" onFocus="onClickSelect()"/>
				</div>
			</li>
		</ul>
	</form>
</div>
<div id="dlg-buttons">
	<a href="#" id="saveButton" class="easyui-linkbutton" iconCls="icon-ok" onclick="saveUser()">确定</a>
	<a href="#" class="easyui-linkbutton" iconCls="icon-cancel" onclick="cancel()">取消</a>
</div>
</body>
</html>
<?php
}
?>
