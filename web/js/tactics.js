$(function() {
	$('#dg').datagrid({
		url: 'get_data.php',
		rownumbers: true,
		singleSelect: true,
		pagination: true,
		pageSize: 20,
		pageList: [20, 40, 60, 80, 100],
		fit: true,
		fitColumns: true,
		toolbar: '#toolbar',
		idField: 'ID',
		nowrap: false,
		border: false
	});
});
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
//对记录进行搜索
function doSearch() {
    $('#dg').datagrid('load',{
	    searchTactics: $('#searchTactics').val(),
	    searchEnable: $("input[name='searchEnable']:checked").val()
    });
}
//清空过滤
function back() {
	$('#dg').datagrid('load', {
	});
	$('#pForm').find('input').val('');
}
//取消选中
function unSelectAll() {
	$('#dg').datagrid('unselectAll');
}
//关闭对话框，并将所有错误提示设置为none
function cancel() {
    $('#dlg').dialog('close');
    var error1 = document.getElementById("error1");
    error1.style.display = "none";
}
$(function(){  //$(function(){});  它是$(document).ready() 的简写
    $('#dlg').dialog({
        onClose: function(){
            var error1 = document.getElementById("error1");
            error1.style.display = "none";
        }
    });
});