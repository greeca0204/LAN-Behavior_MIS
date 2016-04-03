$(function(){
    $('.text').focus(function(){
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
    });
    $('#land').click(function(){
        var user = $('#user').val();
        var password = $('#password').val();
	    //console.info($('form').serialize());
        if(user == '' || password == '') {
            alert('用户名或密码错误！');
	        if(user == '') {
		        $('#user').focus();
	        }
	        else {
		        $('#password').focus();
	        }
        }
        else{
            $.post("loginResult.php",$('form').serialize(),function(result){//"$(form').serialize():序列化表单值传到后台，后台用属性name来获取value
                if(result.success) {
                    window.location.href="../";
                }
                else {
                    alert('用户名或密码错误！');
	                $('#user').focus();
                }
            },'json');
	        /*
	        $.ajax({
		        type: "post",
		        url: "loginResult.php",
		        data: $('form').serialize(),
		        dataType: 'json',
		        async: false,
		        success: function(result) {
			        if(result.success) {
				        window.location.href="../";
				        $.get("../php/userOnline/userOnline.php");
			        }
			        else {
				        alert('登陆失败！');
			        }
	        }})*/
        }
    });
	$('body').keydown(function(event) {
		if(event.keyCode == 13) {    //13 指 Enter键
			var user = $('#user').val();
			var password = $('#password').val();
			//console.info($('form').serialize());
			if(user == '' || password == '') {
				alert('用户名或密码错误！');
				if(user == '') {
					$('#user').focus();
				}
				else {
					$('#password').focus();
				}
			}
			else{
				$.post("loginResult.php",$('form').serialize(),function(result){//"$(form').serialize():序列化表单值传到后台，后台用属性name来获取value
					if(result.success) {
						window.location.href="../";
					}
					else {
						alert('用户名或密码错误！');
						$('#user').focus();
					}
				},'json');
			}
		}
	});
    //return false;  //防止页面重新加载
});

