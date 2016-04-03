var url;
function newUser(){
    $('#dlg').dialog('open').dialog('setTitle','添加');
    $('#fm').form('clear');
    url = 'save_data.php';
}
function editUser(){
    var row = $('#dg').datagrid('getSelected');
    if (row){
        $('#dlg').dialog('open').dialog('setTitle','编辑');
	    if(row.Enable) {  //存在Enable这个字段，则进行转换
		    if(row.Enable == '是') {
			    row.Enable = 1;  //响应value值
		    }
		    else {
			    row.Enable = 0;
		    }
	    }
	    $('#fm').form('load',row);
        url = 'update_data.php?ID='+row.ID;     //标记每行ID
    }
}
function saveUser(){
	/*
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
    */
	$.post(url, $('#fm').serialize(), function(result) {
		if(result.success) {
			$('#dlg').dialog('close');		// close the dialog
			$('#dg').datagrid('reload');	// reload the user data
		}
		else {
			$.messager.alert('错误', result.msg, 'error');
		}
	}, 'json');
}
function removeUser(){
    var rows = $('#dg').datagrid('getSelections');  //getSelections方法：获取所有选择的记录
    var ids = [];  //定义一个数组变量
    for(var i = 0; i < rows.length; i++) {
        ids.push(rows[i].ID);
    }
    if (rows){
        //$.messager.confirm():显示一个含有确定和取消按钮的确认消息窗口
        $.messager.confirm('选择','确定要删除这些记录?',function(r){
            if (r){
                $.post('remove_data.php',{ID:ids},function(result){  //使用POST方式来进行异步请求,向后台传送ID数组
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