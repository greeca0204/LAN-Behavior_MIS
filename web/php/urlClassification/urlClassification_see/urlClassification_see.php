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
	<title>查看网页分类</title>
	<link type="text/css" rel="stylesheet" href="../../../js/easyui/themes/default/easyui.css">
	<link type="text/css" rel="stylesheet" href="../../../js/easyui/themes/icon.css">
	<link type="text/css" rel="stylesheet" href="../../../js/easyui/demo/demo.css">
	<link type="text/css" rel="stylesheet" href="../../../css/jquery_easyui.css">
	<link type="text/css" rel="stylesheet" href="../../../css/urlClassification_see.css">
	<script type="text/javascript" src="../../../js/easyui/jquery-1.8.0.min.js"></script>
	<script type="text/javascript" src="../../../js/easyui/jquery.easyui.min.js"></script>
	<script type="text/javascript" src="../../../js/easyui/locale/easyui-lang-zh_CN.js"></script>
	<script type="text/javascript" src="../../../js/urlClassification_see.js"></script>
</head>
<body>
<div class="easyui-layout" fit="true">
	<div data-options="region: 'north', border: false" style="height: 38px; overflow-x: hidden; overflow-y: hidden;">
		<table>
			<tr>
				<td>
					<div style="margin: 5px; min-width: 300px">
						<label for="classification" style="display: inline-block; width: 100px; margin: 3px 1px;">选择网页类别</label>
						<input type="text" id="classification" name="classification"/>
					</div>
				</td>
				<td>
					<a href="#" class="easyui-linkbutton" iconCls="icon-remove" plain="false" title="删除分类" onclick="deleteTable()">删除分类</a>
				</td>
			</tr>
		</table>

	</div>
	<div data-options="region: 'center', border: false" id="center" style="background:#eee;">
		<table id="dg"></table>
		<div id="toolbar">
			<a href="#" class="easyui-linkbutton" iconCls="icon-add" plain="true" title="添加" onclick="newData()">添加</a>
			<a href="#" class="easyui-linkbutton" iconCls="icon-edit" plain="true" title="编辑"
			   onclick="editData()">编辑</a>
			<a href="#" class="easyui-linkbutton" iconCls="icon-remove" plain="true" title="删除" onclick="removeData()">删除</a>
			<a href="#" class="easyui-linkbutton" iconCls="icon-undo" plain="true" title="取消选中" onclick="unSelectAll()">取消选中</a>
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<input id="searchUrl" onclick="onClickSelect()"/>
			<a href="#" class="easyui-linkbutton" iconCls="icon-search" plain="true" title="查询条件" onclick="doSearch()">网址查询</a>
			<a href="#" class="easyui-linkbutton" iconCls="icon-back" plain="true" title="清空条件"
			   onclick="back()">返回</a>
		</div>
	</div>

	<div id="dlg" class="easyui-dialog" style="width: 500px;height: 250px;padding: 10px 20px"
	     closed="true" buttons="#dlg-buttons" modal="true">
		<div class="ftitle">基本信息</div>
		<form id="fm" method="post" novalidate>
			<ul>
				<li>
					<div class="fitem">
						<label class="label">网址:</label>
						<input id="Url" name="Url" class="easyui-validatebox" onfocus="onClickSelect()" required="true"/>
					</div>
				</li>
				<li>
					<div class="fitem">
						<label class="label">注释:</label>
						<input id="Name" name="Name" onfocus="onClickSelect()"/>
					</div>
				</li>
			</ul>
		</form>
	</div>
	<div id="dlg-buttons">
		<a href="#" class="easyui-linkbutton" iconCls="icon-ok" onclick="saveData()">确定</a>
		<a href="#" class="easyui-linkbutton" iconCls="icon-cancel"
		   onclick="javascript:$('#dlg').dialog('close')">取消</a>
	</div>
</div>
</body>
</html>
<?php
}
?>
