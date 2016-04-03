$(function() {
	$('#dg').datagrid({
		url: 'get_data.php',
		rownumbers: false,
		singleSelect: true,
		pagination: true,
		pageSize: 20,
		pageList: [20, 40, 60, 80, 100],
		fit: true,
		fitColumns: true,
		idField: 'ID',
		nowrap: false,
		border: false,
		columns: [[
			{field: 'Name', title: '员工名', width: 100, align: 'center', sortable: 'true'},
			{field: 'Uid', title: '工号',width:100, align: 'center', sortable: 'true'},
			{field: 'IP', title: 'IP地址', width: 100, align: 'center', sortable: 'true'}
		]],
		toolbar: [{
			text: '查询条件',
			iconCls: 'icon-search',
			handler: function() { doSearch() }
		},'-',{
			text: '清空条件',
			iconCls: 'icon-back',
			handler: function() { back() }
		}
		]
	});

	$('#dg').datagrid({
		view: detailview,
		detailFormatter:function(index,row){
			return '<div style="padding:2px"><table id="ddv-' + index + '"></table></div>';
		},
		onExpandRow: function(index,row){
			$('#ddv-'+index).datagrid({
				url:'getDetail.php?IP='+row.IP,
				fitColumns:true,
				singleSelect:true,
				rownumbers:true,
				loadMsg:'',
				height:'150',
				columns:[[
					{field:'Position',title:'上传文件路径',width:100, sortable: 'true',
						formatter:function (value, rowData, rowIndex) {
							return '<a target="_blank" href="../../../' + value + '">' + value + '</a>';
						}
					},
					{field: 'Datetime', title: '时间', width: 100, sortable: 'true'}
				]],
				onResize:function(){
					$('#dg').datagrid('fixDetailRowHeight',index);
				},
				onLoadSuccess:function(){
					setTimeout(function(){
						$('#dg').datagrid('fixDetailRowHeight',index);
					},0);
				}
			});
			$('#dg').datagrid('fixDetailRowHeight',index);
		}
	});

	$('#p').panel({
		fit: true,
		border: false
	});
});
//过滤条件
function doSearch() {
	$('#dg').datagrid('load',{
		searchName: $('#searchName').val(),    //可以认为为#searchName这个input输入框设置了name属性
		searchUid: $('#searchUid').val(),
		searchIP: $('#searchIP').val()
	});
}
//清空过滤
function back() {
	$('#dg').datagrid('load', {
	});
	$('#pForm').find('input').val('');
}
//input输入框聚焦时，文本全选
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
