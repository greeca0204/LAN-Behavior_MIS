<?php
session_start();
if(!isset($_SESSION['ID'])) {
	echo '<meta http-equiv="Refresh" content="0, url=../../login/login.php">';
}
else {
?>
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<title>修改密码</title>
	<link rel="stylesheet" type="text/css" href="../../css/changePassword.css">
	<script type="text/javascript" src="../../js/jquery-1.10.1.js"></script>
	<script type="text/javascript" src="../../js/changePassword.js"></script>
</head>
<body>
<div class="firstDiv">
	<form id="form" method="post" novalidate>
		<ul>
			<li>
				<div class="fitem">
					<label>用户名</label>
					<input id="user" placeholder="admin" disabled="true"/>
				</div>
			</li>
			<li>
				<div class="fitem">
					<label>当前密码</label>
					<input id="passwordOld" type="password" required="true"/>
				</div>
				<div class="msg">
					<span id="tip3" class="tip"></span>
				</div>
			</li>
			<li>
				<div class="fitem">
					<label for="password">密&nbsp;&nbsp;码</label>
					<input id="password" type="password" required="true"/>
				</div>
				<div class="msg">
					<span id="tip1" class="tip">长度为6-20个字符</span>
				</div>
			</li>
			<li>
				<div class="fitem">
					<label for="password2">确认密码</label>
					<input id="password2" type="password" required="true"/>
				</div>
				<div class="msg">
					<span id="tip2" class="tip"></span>
				</div>
			</li>
			<li>
				<button type="button" id="submit">确认</button>
			</li>
		</ul>
	</form>
</div>
</body>
</html>
<?php
}
?>