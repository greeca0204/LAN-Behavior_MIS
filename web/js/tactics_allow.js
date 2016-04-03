//对记录进行搜索
function doSearch() {
	$('#dg').datagrid('load',{
		searchName: $('#searchName').val(),    //可以认为为#searchName这个input输入框设置了name属性
		searchUid:  $('#searchUid').val(),
		searchIP: $('#searchIP').val(),
		searchTactics: $('#searchTactics').val(),
		searchDetail: $('#searchDetail').val(),
		searchEnable: $('#searchEnable').val()
	});
}
//返回所有记录
function back() {
	$('#dg').datagrid('load', {
	});
	$('#pForm').find('input').val('');
	$('#pForm').find('select').val('');
}
//取消选择
function unSelectAll() {
	$('#dg').datagrid('unselectAll');
}
