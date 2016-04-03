$(function() {
	var flag1, flag2;    //全局变量，标记正则判断结果

	$('#submit').click(function() {
		var passwordOld = $('#passwordOld').val();
		var password1 = $('#password').val();
		var password2 = $('#password2').val();

		if(flag1 == 1 && flag2 ==1) {
			$.post('post_change.php', {"passwordOld": passwordOld, "password": password1}, function(result){
				if(result.success == 'passwordOld') {
					$('#tip3').html("旧密码有误");
				}
				else if(result.success) {
					alert('密码修改成功！');
					top.location.href = '../../login/login.php';
				}
				else {
					alert('密码修改失败！');
				}
			}, 'json')
		}
	});
	$('#passwordOld').blur(function() {
		$('#tip3').html("");
	});
	$('#password').blur(function() {
		var password = $('#password').val();
		var pattern = /^.{6,20}$/;
		var pattern2 = /^\S+$/gi;
		flag1 = 0;    //标记正则判断结果
		if(!pattern2.test(password)) {
			$('#tip1').html("不能包含空格");
		}
		else {
			if(pattern.test(password)) {
				$('#tip1').html("OK");
				flag1 = 1;
			}
			else {
				$('#tip1').html("长度为6-20个字符");
			}
		}
	});
	$('#password2').blur(function() {
		var password1 = $('#password').val();
		var password2 = $('#password2').val();
		var pattern = /^.{6,20}$/;
		flag2 = 0;    //标记正则判断结果
		if(!pattern.test(password1)) {
			$('#tip2').html("");
		}
		else {
			if(password1 != password2) {
				$('#tip2').html("密码不一致");
			}
			else {
				$('#tip2').html("OK");
				flag2 = 1;
			}
		}
	});
});