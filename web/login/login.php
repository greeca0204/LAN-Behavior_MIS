<?php
session_start();
if(isset($_SESSION['ID'])) {
	echo '<meta http-equiv="Refresh" content="0, url=../">';
}
else {
?>
<!DOCTYPE html>
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<title>上网行为管理系统登陆</title>
	<link rel="stylesheet" type="text/css" href="login.css">
	<script type="text/javascript" src="../js/jquery-1.10.1.js"></script>
	<script type="text/javascript" src="login.js"></script>
</head>
<body>
	<div>
		<table align="center">
			<tr style="height: 200px;">
				<td align="center" colspan="2">
					<h1>上网行为管理系统登录</h1>
				</td>
			</tr>
			<tr>
				<td width="55%" align="right">
					<img src="../image/login_05.png"/>
				</td>
				<td width="45%" align="left" style="padding-left: 50px;">
					<form action="" method="post">
						<ul>
							<li>
								<h2>欢迎使用本系统</h2>
							</li>
							<li>
								<div class="row">
									<div class="login">
										<label for="user"><span>用户名</span></label>
										<input type="text" id="user" name="user" class="text" placeholder="请输入管理员账号"/>
									</div>
									<div class="msg">
										<p id="tip1">管理员：admin</p>
									</div>
								</div>
							</li>
							<br/>
							<li>
								<div class="row">
									<div class="login">
										<label for="password"><span>密&nbsp;&nbsp;码</span></label>
										<input type="password" id="password" name="password" class="text" placeholder="请输入管理员密码"/>
									</div>
									<div class="msg">
										<p id="tip2">默认：123456</p>
									</div>
								</div>
							</li>
							<li>
								<div class="row">
									<button type="button" id="land" >登陆</button>
								</div>
							</li>
						</ul>
					</form>
				</td>
			</tr>
		</table>
	</div>
</body>
</html>
<?php
}
?>