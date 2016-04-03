<?php
session_start();
if(!isset($_SESSION['ID'])) {
	echo '<meta http-equiv="Refresh" content="0, url=../../../login/login.php">';
}
else {
	?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
	<meta charset='utf-8'>
	<title>个人黑名单</title>
	<link rel="stylesheet" type="text/css" href="../../../js/easyui/themes/default/easyui.css">
	<link rel="stylesheet" type="text/css" href="../../../js/easyui/themes/icon.css">
	<link rel="stylesheet" type="text/css" href="../../../js/easyui/demo/demo.css">
	<link type="text/css" rel="stylesheet" href="../../../css/jquery_easyui.css">
	<link type="text/css" rel="stylesheet" href="../../../css/tactics_user.css">
	<link type="text/css" rel="stylesheet" href="../../../css/search.css">
	<script type="text/javascript" src="../../../js/easyui/jquery-1.8.0.min.js"></script>
	<script type="text/javascript" src="../../../js/easyui/jquery.easyui.min.js"></script>
	<script type="text/javascript" src="../../../js/easyui/locale/easyui-lang-zh_CN.js"></script>
	<script type="text/javascript" src="../../../js/jquery_easyui.js"></script>
	<script type="text/javascript" src="../../../js/tacticsAll.js"></script>
	<script type="text/javascript" src="../../../js/tactics_user.js"></script>
</head>
<body>
<div class="easyui-layout" fit="true">
	<div data-options="region: 'north', title:'查询条件', border: false" style="height: 102px;">
		<!--搜索条件-->
		<div class="easyui-panel" id="p" style="background: #eee; min-width: 1100px; overflow-y: hidden;"
		     data-options="fit: true, border: false">
			<!--代码在get_data.php中实现-->
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
					<tr>
						<td>
							<label for="searchTactics">策略类型</label>
							<input id="searchTactics" class="search" onFocus="onClickSelect()"/>
						</td>
						<td>
							<label for="searchDetail">关键字</label>
							<input id="searchDetail" class="search" onFocus="onClickSelect()"/>
						</td>
						<td>
							<label>启用</label>
							<select id="searchEnable" style="width: 140px; height: 26px;">
								<option value=""></option>
								<option value="1">是</option>
								<option value="0">否</option>
							</select>
						</td>
					</tr>
				</table>
			</form>
		</div>
	</div>
	<div data-options="region: 'center', border: false">
		<table id="dg" style="background:#eee;">
			<thead>
			<tr>
				<th field="ck" checkbox="true"></th>
				<th field="Name" width="50" sortable="true" align="center">员工名</th>
				<th field="Uid" width="50" sortable="true" align="center">工号</th>
				<th field="User_IP" width="80" sortable="true" align="center">IP地址</th>
				<th field="Tactics_type" width="50" sortable="true" align="center">策略类型</th>
				<th field="Detail" width="50" sortable="true" align="center">关键字</th>
				<th field="Enable" width="50" sortable="true" align="center">启用</th>
				<th field="Comment" width="50" sortable="true" align="center">注释</th>
			</tr>
			</thead>
		</table>
		<div id="toolbar">
			<a href="#" class="easyui-linkbutton" iconCls="icon-add" plain="true" title="添加" onclick="newUser()">添加</a>
			<a href="#" class="easyui-linkbutton" iconCls="icon-edit" plain="true" title="编辑"
			   onclick="editUser()">编辑</a>
			<a href="#" class="easyui-linkbutton" iconCls="icon-remove" plain="true" title="删除" onclick="removeUser()">删除</a>
			<a href="#" class="easyui-linkbutton" iconCls="icon-undo" plain="true" title="取消选中" onclick="unSelectAll()">取消选中</a>
			<a href="#" class="easyui-linkbutton" iconCls="icon-search" plain="true" title="查询条件" onclick="doSearch()">查询条件</a>
			<a href="#" class="easyui-linkbutton" iconCls="icon-back" plain="true" title="清空条件"
			   onclick="back()">清空条件</a>
		</div>
	</div>
</div>
</div>
<div id="dlg" class="easyui-dialog" style="width:510px;height:350px;padding:10px 20px"
     closed="true" buttons="#dlg-buttons" modal="true">
	<div class="ftitle">基本信息</div>
	<form id="fm" method="post" novalidate>
		<ul>
			<li>
				<div class="fitem">
					<label class="label">个人信息:</label>
					<select class="easyui-combogrid" id="Uid" name="Uid" data-options="
				panelWidth: 342,
				panelheight: '200',
				idField: 'Uid',
				textField: 'Name',
				url: 'combogrid_user.php',
				mode: 'remote',
				delay: 500,
				required: true,
				editable: true,
				multiple:false,
				columns:[[
                  {field:'Uid',title:'工号',width:100},
                  {field:'Name',title:'员工名',width:100},
                  {field:'User_IP',title:'IP地址',width:120}
				]]
				"></select>
				</div>
			</li>
			<li>
				<div class="fitem">
					<label class="label">策略类型:</label>
					<select class="easyui-combobox" name="Tactics_type" data-options="
                 panelWidth: 180,
				   panelHeight: '150',
                 valueField: 'Comment',
                 textField: 'Comment',
                 url: 'combobox_tactics.php',
                 required: true,
                 editable: false,
                 onSelect: function(rec) {
                    var url = 'combobox_app_table.php?Tactics_type='+rec.Comment;
                    $('#Detail').combobox('clear').combobox('reload', url);
                 }
                 "></select>
				</div>
			</li>
			<li>
				<div class="fitem">
					<label class="label">关键字:</label>
					<select class="easyui-combobox" id="Detail" name="Detail" data-options="
                 valueField:'Comment',
                 textField:'Comment',
                 panelWidth: 180,
                 panelHeight:'200',
                 mode: 'remote',
                 delay: 500,
                 required: true,
                 editable:true
                 "></select>
				</div>
			</li>
			<li>
				<div class="fitem">
					<label class="label">启用:</label>
					<input type="radio" class="radio" name="Enable" value="1" checked="checked"/>是 &nbsp;&nbsp;&nbsp;&nbsp;
					<input type="radio" class="radio" name="Enable" value="0"/>否
				</div>
			</li>
			<li>
				<div class="fitem">
					<label class="label">注释:</label>
					<input class="easyui-validatebox" name="Comment"/>
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