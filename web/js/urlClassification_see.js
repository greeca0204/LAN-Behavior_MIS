$(function() {
	$('#classification').combobox({
		url: 'get_combobox_data.php',
		valueField: 'Comment',
		textField: 'Comment',
		width: 140,
		height: 22,
		mode: 'remote',
		delay: 500,
		onSelect: function(rec) {
			$('#dg').datagrid('load', { Comment: rec.Comment });
		}
	});
	$('#dg').datagrid({
		url: 'get_datagrid_data.php',
		rownumbers: true,
		singleSelect: false,
		pagination: true,
		pageSize: 20,
		pageList: [20, 40, 60, 80, 100],
		fit: true,
		fitColumns: true,
		idField: 'ID',
		nowrap: false,
		border: false,
		toolbar: '#toolbar',
		columns: [[
			{field: 'ck', checkbox: 'true'},
			{field: 'Url', title: '网址', width: 50, align: 'center',
				formatter: function(value, rowData, rowIndex) {
					return '<a href="http://' + value + '" target="_blank">' + value + '</a>';
				}
			},
			{field: 'Name', title: '注释',width: 50, align: 'center'}
		]]
	});
/*
	$.fn.extChange = function(callback){
		var value = $(this).val();
		setInterval($.proxy(function(){
			if(callback){
				var newValue = $(this).val();
				if(value != null && value != newValue){
					callback.call(this,window.event);
					value = newValue;
				}
			}
		},this),300);
	};
	$('#classification').combobox('textbox').extChange(function(){
		var q = $('#classification').combobox('getText');
		$.post('get_combobox_data.php', {q: q});
	});
	*/
});
//对记录进行搜索
function doSearch() {
	$('#dg').datagrid('load',{
		Comment: $('#classification').combobox('getText'),
		searchUrl: $("#searchUrl").val()
	});
}
//返回所有记录
function back() {
	$('#dg').datagrid('load', {
		Comment: $('#classification').combobox('getText')
	});
	$('#searchUrl').val('');
}
//取消选择
function unSelectAll() {
	$('#dg').datagrid('unselectAll');
}
function onClickSelect() {
	var obj = document.activeElement;
	if(obj.tagName == "TEXTAREA")
	{
		obj.select();
	}
	if(obj.tagName == "INPUT" )
	{
		if(obj.type == "text")
			obj.select();
	}
}
function deleteTable() {
	var tableName = $('#classification').combobox('getText');
	if(tableName != '') {
		$.messager.confirm('选择','删除网页分类,将会删除相关的阻断配置！<br/>确定?',function(r){
			if(r) {
				$.post('deleteTable.php', {tableName: tableName}, function(result) {
					if(result.success) {
						alert("删除类别成功");
						$('#classification').combobox('clear');
						window.location.reload();  //刷新当前页面
					}
					else{
						$.messager.alert('错误', result.msg, 'error');
					}
				}, 'json');
			}
		});
	}
}

//Dialog增删改操作
var url;
function newData(){
	var select = $('#classification').combobox('getText');
	if(select) {
		$('#dlg').dialog('open').dialog('setTitle','添加');
		$('#fm').form('clear');
		url = 'save_data.php?Comment='+select;
	}
}
function editData(){
	var select = $('#classification').combobox('getText');
	var row = $('#dg').datagrid('getSelected');
	if (row){
		$('#dlg').dialog('open').dialog('setTitle','编辑');
		$('#fm').form('load',row);
		url = 'update_data.php?ID='+row.ID+'&Comment='+select;     //标记每行ID
	}
}
function saveData(){
	$('#fm').form('submit',{
		url: url,
		onSubmit: function(){
			return $(this).form('validate');
		},
		success: function(result){
			var result = eval('('+result+')');
			if (result.success){
				$('#dlg').dialog('close');		// close the dialog
				$('#dg').datagrid('reload');	// reload the user data
			}
			else {
				$.messager.alert('错误', result.msg, 'error');
			}
		}
	});
}
function removeData(){
	var select = $('#classification').combobox('getText');
	if(select) {
		var rows = $('#dg').datagrid('getSelections');  //getSelections方法：获取所有选择的记录
		var ids = [];  //定义一个数组变量
		for(var i = 0; i < rows.length; i++) {
			ids.push(rows[i].ID);
		}
		if (rows){
			//$.messager.confirm():显示一个含有确定和取消按钮的确认消息窗口
			$.messager.confirm('选择','确定要删除这些记录?',function(r){
				if (r){
					$.post('remove_data.php',{ID:ids, Comment:select},function(result){  //使用POST方式来进行异步请求,向后台传送ID数组
						if (result.success){
							$('#dg').datagrid('reload');	// reload the user data
						}
						else {
							$.messager.alert('错误', result.msg, 'error');
						}
					},'json');
				}
			});
		}
	}
}
