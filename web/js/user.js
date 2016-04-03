$(function(){  //$(function(){});  它是$(document).ready() 的简写
	$('#dg').datagrid({
		url: 'get_data.php',
		rownumbers: true,
		singleSelect: false,
		pagination: true,
		pageSize: 20,
		pageList: [20, 40, 60, 80, 100],
		fit: true,
		fitColumns: true,
		toolbar: '#toolbar',
		idField: 'ID',
		nowrap: false,
		sortName: 'Uid',
		sortOrder: 'asc',
		border: false
	});
	$('#dlg').dialog({
		onClose: function(){
			var error1 = document.getElementById("error1");
			var error2 = document.getElementById("error2");
			var error3 = document.getElementById("error3");
			//var error4 = document.getElementById("error4");
			//var error5 = document.getElementById("error5");
			//var error6 = document.getElementById("error6");
			//var error7 = document.getElementById("error7");
			var error8 = document.getElementById("error8");
			error1.style.display = "none";
			error2.style.display = "none";
			error3.style.display = "none";
			//error4.style.display = "none";
			//error5.style.display = "none";
			//error6.style.display = "none";
			//error7.style.display = "none";
			error8.style.display = "none";
		}
	});
});
function check_name(obj) {
    var preg =  /^[0-9a-zA-Z\u0391-\uFFE5]+$/;
    var error = document.getElementById("error1");
    if (preg.test(obj) && obj.length>=1 && obj.length<=20) {
        error.style.display = "block";
	    error.innerHTML = "OK";
    }
    else {
        error.style.display = "block";
        error.innerHTML = "请输入字母、数字或汉字";  //innerHTML 属性设置或返回表格行的开始和结束标签之间的 HTML；也可以对它进行赋值
    }
}
function check_uid(obj) {
    var preg = /^[0-9]{1,20}$/;
    var error = document.getElementById("error2");
    if (preg.test(obj)) {
        error.style.display = "none";
    }
    else {
        error.style.display = "block";
        error.innerHTML = "请输入1-20位数字";  //innerHTML 属性设置或返回表格行的开始和结束标签之间的 HTML；也可以对它进行赋值
    }
}
//ajax与后台交互，检查数据库是否存在相同的uid，存在则提示错误
function check_db_uid() {
    if(document.getElementById('error2').style.display == "none") {
        var xmlHttp;
        if(window.XMLHttpRequest) {// code for IE7+, Firefox, Chrome, Opera, Safari
            xmlHttp = new XMLHttpRequest();
        }
        else {// code for IE6, IE5
            xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
        }
        var Uid = document.getElementById('Uid').value;
        xmlHttp.open("GET", "check_db_uid.php?Uid=" + Uid, true);
        xmlHttp.send();
        xmlHttp.onreadystatechange = function() {
            if(xmlHttp.readyState==4 && xmlHttp.status==200) {
                document.getElementById("error2").style.display = "block";
                document.getElementById("error2").innerHTML=xmlHttp.responseText;
            }
        }
    }
}
function check_ip(obj) {
    var preg = /^\d+\.\d+\.\d+\.\d+$/;
    var error = document.getElementById("error3");
    if(preg.test(obj)) {
        error.style.display = "none";
    }
    else {
        error.style.display = "block";
        error.innerHTML = "请输入正确的IP";
    }
}
//ajax与后台交互，检查数据库是否存在相同的ip，存在则提示错误
function check_db_ip() {
    if(document.getElementById('error3').style.display == "none") {
        var xmlHttp;
        if(window.XMLHttpRequest) {// code for IE7+, Firefox, Chrome, Opera, Safari
            xmlHttp = new XMLHttpRequest();
        }
        else {// code for IE6, IE5
            xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
        }
        var IP = document.getElementById('User_IP').value;
        xmlHttp.open("GET", "check_db_ip.php?IP=" + IP, true);
        xmlHttp.send();
        xmlHttp.onreadystatechange = function() {
            if(xmlHttp.readyState==4 && xmlHttp.status==200) {
                document.getElementById("error3").style.display = "block";
                document.getElementById("error3").innerHTML=xmlHttp.responseText;
            }
        }
    }
}
/*
function check_password(obj) {
    var preg = /^\S+$/gi;
    var error = document.getElementById("error4");
    if(preg.test(obj) && obj.length >= 6 && obj.length <= 20) {
        error.style.display = "none";
    }
    else {
        error.style.display = "block";
        error.innerHTML = "密码长度6-20，不能包含空格";
    }
}

function check_flow(obj) {
    var preg = /^[0-9]{1,6}$/;
    var error = document.getElementById("error5");
    if ( (preg.test(obj) && obj>=0 && obj <=100000) || obj=='') {
        error.style.display = "none";
    }
    else {
        error.style.display = "block";
        error.innerHTML = "请输入0-100000KB/S";  //innerHTML 属性设置或返回表格行的开始和结束标签之间的 HTML；也可以对它进行赋值
    }
}
function check_upload(obj) {
    var preg = /^[0-9]{1,6}$/;
    var error = document.getElementById("error6");
    if ( (preg.test(obj) && obj>=0 && obj <=100000) || obj=='') {
        error.style.display = "none";
    }
    else {
        error.style.display = "block";
        error.innerHTML = "请输入0-100000KB/S";  //innerHTML 属性设置或返回表格行的开始和结束标签之间的 HTML；也可以对它进行赋值
    }
}
function check_download(obj) {
    var preg = /^[0-9]{1,6}$/;
    var error = document.getElementById("error7");
    if ( (preg.test(obj) && obj>=0 && obj <=100000) || obj=='') {
        error.style.display = "none";
    }
    else {
        error.style.display = "block";
        error.innerHTML = "请输入0-100000KB/S";  //innerHTML 属性设置或返回表格行的开始和结束标签之间的 HTML；也可以对它进行赋值
    }
}
*/
function check_email(obj) {
    var preg =  /^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$/ ;
    var error = document.getElementById("error8");
    if(preg.test(obj)) {
        error.style.display = "none";
    }
    else {
        error.style.display = "block";
        error.innerHTML = "请输入正确的email";
    }
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
//关闭对话框，并将所有错误提示设置为none
function cancel() {
    $('#dlg').dialog('close');
    var error1 = document.getElementById("error1");
    var error2 = document.getElementById("error2");
    var error3 = document.getElementById("error3");
    //var error4 = document.getElementById("error4");
    //var error5 = document.getElementById("error5");
    //var error6 = document.getElementById("error6");
    //var error7 = document.getElementById("error7");
    var error8 = document.getElementById("error8");
    error1.style.display = "none";
    error2.style.display = "none";
    error3.style.display = "none";
    //error4.style.display = "none";
    //error5.style.display = "none";
    //error6.style.display = "none";
    //error7.style.display = "none";
    error8.style.display = "none";
}
//过滤条件
function doSearch() {
	$('#dg').datagrid('load',{
		searchName: $('#searchName').val(),    //可以认为为#searchName这个input输入框设置了name属性
		searchGid: $('#searchGid').val(),
		searchUid:  $('#searchUid').val(),
		searchIP: $('#searchIP').val(),
		searchStartTime: $('#searchStartTime').val(),
		searchEndTime: $('#searchEndTime').val()
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
