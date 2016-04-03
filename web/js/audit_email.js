$(function () {
	$('#dg').datagrid({
		url:'get_data.php',
		rownumbers:false,
		singleSelect:true,
		pagination:true,
		pageSize:20,
		pageList:[20, 40, 60, 80, 100],
		fit:true,
		fitColumns:true,
		idField:'ID',
		nowrap:false,
		border:false,
		columns:[
			[
				{field:'Name', title:'员工名', width:100, align:'center', sortable:'true'},
				{field:'Uid', title:'工号', width:100, align:'center', sortable:'true'},
				{field:'User_IP', title:'IP地址', width:120, align:'center', sortable:'true'}
			]
		],
		toolbar:[
			{
				text:'查询条件',
				iconCls:'icon-search',
				handler:function () {
					doSearch()
				}
			},
			'-',
			{
				text:'清空条件',
				iconCls:'icon-back',
				handler:function () {
					back()
				}
			}
		]
	});
	$('#dg').datagrid({
		view:detailview,
		detailFormatter:function (index, row) {
			return '<div style="padding:2px"><table id="ddv-' + index + '"></table></div>';
		},
		onExpandRow:function (index, row) {
			$('#ddv-' + index).datagrid({
				url:'getDetailData.php?Uid=' + row.Uid,
				fitColumns:true,
				singleSelect:true,
				rownumbers:true,
				loadMsg:'',
				height:'150',
				nowrap:false,
				border:false,
				columns:[
					[
						{field:'Email_type', title:'邮件类型', width:120, align:'center', sortable:'true'},
						{field:'Title', title:'主题', width:120, align:'center', sortable:'true'},
						{field:'Time', title:'时间', width:140, align:'center', sortable:'true'},
						{field:'Receiver', title:'收件人', width:120, align:'center', sortable:'true'},
						{field:'Sender', title:'发件人', width:120, align:'center', sortable:'true'},
						{field:'ID', title:'操作', width:100, align:'center',
							formatter:function (value, rowData, rowIndex) {
								return '<button type="button" onclick="seeContent(' + index + ' , ' + rowIndex + ')">查看内容</button> ';
							}
						}
					]
				],
				onResize:function () {
					$('#dg').datagrid('fixDetailRowHeight', index);
				},
				onLoadSuccess:function () {
					setTimeout(function () {
						$('#dg').datagrid('fixDetailRowHeight', index);
					}, 0);
				}
			});
			$('#dg').datagrid('fixDetailRowHeight', index);
		}
	});
	$('#dd').dialog({
		width:600,
		height:400,
		maximizable:true,
		close:true,
		cache:false,
		modal:true
	});
	$('#dd').dialog('close');    //默认关闭

	$('#p').panel({
		fit:true,
		border:false
	});
});
//打开dialog, 查看邮件内容
function seeContent(index, i) {
	var rows = $('#ddv-' + index).datagrid('getRows');    //获取所有行数据
	$('#dd').dialog('open').dialog('setTitle', '邮件内容');
	var Email_type = rows[i].Email_type;
	var Detail = rows[i].Detail;
	//163邮件显示html前的操作
	if (Email_type.search(/163/) != -1) {
		Detail = Detail.replace(/&amp;/g, "&");    //乱码替换
		Detail = Detail.replace(/&nbsp;/g, " ");
		Detail = Detail.replace(/&lt;/g, "<");
		Detail = Detail.replace(/&gt;/g, ">");
	}
	$('#emailData').html(Detail);
}
//过滤条件
function doSearch() {
	$('#dg').datagrid('load', {
		searchName:$('#searchName').val(), //可以认为为#searchName这个input输入框设置了name属性
		searchUid:$('#searchUid').val(),
		searchIP:$('#searchIP').val()
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
	if (obj.tagName == "TEXTAREA") {
		obj.select();
	}
	if (obj.tagName == "INPUT") {
		if (obj.type == "text")
			obj.select();
	}
}
