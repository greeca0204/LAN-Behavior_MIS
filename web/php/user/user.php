<?php
session_start();
if(!isset($_SESSION['ID'])) {
	echo '<meta http-equiv="Refresh" content="0, url=../../login/login.php">';
} else {
	?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<title>添加用户</title>
	<link type="text/css" rel="stylesheet" href="../../js/easyui/themes/default/easyui.css">
	<link type="text/css" rel="stylesheet" href="../../js/easyui/themes/icon.css">
	<link type="text/css" rel="stylesheet" href="../../js/easyui/demo/demo.css">
	<link type="text/css" rel="stylesheet" href="../../css/jquery_easyui.css">
	<link type="text/css" rel="stylesheet" href="../../css/user.css">
	<link type="text/css" rel="stylesheet" href="../../css/search.css">
	<script type="text/javascript" src="../../js/easyui/jquery-1.8.0.min.js"></script>
	<script type="text/javascript" src="../../js/easyui/jquery.easyui.min.js"></script>
	<script type="text/javascript" src="../../js/easyui/locale/easyui-lang-zh_CN.js"></script>
	<script type="text/javascript" src="../../js/My97DatePicker/WdatePicker.js"></script>
	<script type="text/javascript" src="../../js/jquery_easyui.js"></script>
	<script type="text/javascript" src="../../js/user.js"></script>
</head>
<body>
<div class="easyui-layout" fit="true">
	<div data-options="region: 'north', title:'查询条件', border: false" style="height: 65px;">
		<!--搜索条件-->
		<div class="easyui-panel" id="p" style="background: #eee; min-width: 1100px; overflow-y: hidden;"
		     data-options="fit: true, border: false">
			<!--代码在get_data.php中实现-->
			<form id="pForm">
				<table>
					<tr>
						<td>
							<label for="searchName">用户名</label>
							<input id="searchName" class="search" onFocus="onClickSelect()"/>
						</td>
						<td>
							<label for="searchGid">组号</label>
							<input id="searchGid" class="search" onFocus="onClickSelect()"/>
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
<!--
					<tr>
						
						<td rowspan="2">
							<label for="searchStartTime">创建日期</label>
							<input class="Wdate" id="searchStartTime" type="text"
							       onfocus="WdatePicker({skin:'whyGreen', dateFmt:'yyyy-MM-dd HH:mm:ss'})"
							       style="width: 140px; text-indent: 5px; height: 26px;" readonly/>
							<span>至</span>
							<input class="Wdate" id="searchEndTime" type="text"
							       onfocus="WdatePicker({skin:'whyGreen', dateFmt:'yyyy-MM-dd HH:mm:ss'})"
							       style="width: 140px; text-indent: 5px; height: 26px;" readonly/>
						</td>
					</tr>
-->
				</table>
			</form>
		</div>
	</div>
	<div data-options="region: 'center', border: false">
		<table id="dg" style="background:#eee;">
			<thead>
			<tr>
				<th field="ck" checkbox="true"></th>
				<th field="Name" width="100" align="center" sortable="true">用户名</th>
				<th field="Gid" width="100" align="center" sortable="true">组号</th>
				<th field="Uid" width="100" align="center" sortable="true">工号</th>
				<th field="User_IP" width="120" align="center" sortable="true">IP地址</th>
				<!--<th field="Password" width="100">密码</th>-->
				<th field="Email" width="120" align="center" sortable="true">邮箱</th>
				<!--<th field="CreateDate" width="140" align="center" sortable="true">创建日期</th>
				<th field="Flow" width="100" align="center" sortable="true">流量</th>
				<th field="Upload" width="100" align="center" sortable="true">上传</th>
				<th field="Download" width="100" align="center" sortable="true">下载</th>-->
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

<div id="dlg" class="easyui-dialog"
     closed="true" buttons="#dlg-buttons" modal="true">
	<div class="ftitle">基本信息</div>
	<form id="fm" method="post" novalidate>
		<ul>
			<li>
				<div class="fitem">
					<label class="label">用户名:</label>
					<input name="Name" class="easyui-validatebox" required="true" onkeyup="check_name(this.value)"
					       onFocus="onClickSelect()"/>
				</div>
				<div class="msg">
					<p id="error1">测试</p>
				</div>
			</li>
			<li>
				<div class="fitem">
					<label class="label">组号:</label>
					<!--valueField: 进行赋值给name，传值给后台, textField:显示的文字, editable:false:只定编辑框不能输出文字-->
					<input class="easyui-combobox" name="Gid" data-options="
                 url:'combobox.php',
                 valueField:'Gid',
                 textField:'Gid',
                 panelHeight:'auto',
                 editable:false,
                 required: true
                 "/>
				</div>
			</li>
			<li>
				<div class="fitem">
					<label class="label">工号:</label>
					<input id="Uid" name="Uid" class="easyui-validatebox" required="true"
					       onkeyup="check_uid(this.value)" onFocus="onClickSelect()" onblur="check_db_uid()">
				</div>
				<div class="msg">
					<p id="error2">测试</p>
				</div>
			</li>
			<li>
				<div class="fitem">
					<label class="label">IP地址:</label>
					<input id="User_IP" name="User_IP" class="easyui-validatebox" required="true"
					       onFocus="onClickSelect()" onkeyup="check_ip(this.value)" onblur="check_db_ip()"/>
				</div>
				<div class="msg">
					<p id="error3">测试</p>
				</div>
			</li>
			<!--
			<li>
				<div class="fitem">
					<label class="label">密码:</label>
					<input name="Password" class="easyui-validatebox" required="true" onFocus="onClickSelect()"
					       onkeyup="check_password(this.value)"/>
				</div>
				<div class="msg">
					<p id="error4">测试</p>
				</div>
			</li>
			-->
			<li>
				<div class="fitem">
					<label class="label">邮箱:</label>
					<input name="Email" class="easyui-validatebox"
					       onFocus="onClickSelect()" onfocus="onClickSelect()" onkeyup="check_email(this.value)"/>
				</div>
				<div class="msg">
					<p id="error8">测试</p>
				</div>
			</li>
			<!--
			<li>
				<div class="fitem">
					<label class="label">创建日期:</label>
					<input class="easyui-datetimebox" name="CreateDate" data-options="editable:false, required:true"/>
				</div>
			</li>
			
			<li>
				<div class="fitem">
					<label class="label">流量:</label>
					<input name="Flow" onFocus="onClickSelect()" onkeyup="check_flow(this.value)"/>
				</div>
				<div class="msg">
					<p id="error5">测试</p>
				</div>
			</li>
			<li>
				<div class="fitem">
					<label class="label">上传:</label>
					<input name="Upload" onFocus="onClickSelect()" onkeyup="check_upload(this.value)"/>
				</div>
				<div class="msg">
					<p id="error6">测试</p>
				</div>
			</li>
			<li>
				<div class="fitem">
					<label class="label">下载:</label>
					<input name="Download" onFocus="onClickSelect()" onkeyup="check_download(this.value)"/>
				</div>
				<div class="msg">
					<p id="error7">测试</p>
				</div>
			</li>
-->
		</ul>
	</form>
</div>
<div id="dlg-buttons">
	<a href="#" class="easyui-linkbutton" iconCls="icon-ok" onclick="saveUser()">确定</a>
	<a href="#" class="easyui-linkbutton" iconCls="icon-cancel"
	   onclick="javascript:$('#dlg').dialog('close')">取消</a>
</div>
</div>
</body>
</html>
<?php
}
?>
