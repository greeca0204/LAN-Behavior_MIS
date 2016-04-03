//对记录进行搜索
function doSearch() {
    $('#dg').datagrid('load',{
        searchName: $('#searchName').val()  //可以认为为#searchName这个input输入框设置了name属性
    });
}
//返回所有记录
function back() {
    $('#dg').datagrid('load', {
        searchName: ''
    });
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