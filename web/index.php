<?php
session_start();
if(isset($_SESSION['ID'])) {
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
	<meta http-equiv="content-type" content="text/html; charset=utf-8">
	<link type="text/css" rel="stylesheet" href="js/easyui/themes/default/easyui.css">
	<link type="text/css" rel="stylesheet" href="js/easyui/themes/icon.css">
	<link type="text/css" rel="stylesheet" href="js/easyui/demo/demo.css">
	<link type="text/css" rel="stylesheet" href="css/index.css">
	<link rel="stylesheet" type="text/css" href="css/menu.css" />
	<script type="text/javascript" src="js/easyui/jquery-1.8.0.min.js"></script>
	<script type="text/javascript" src="js/easyui/jquery.easyui.min.js"></script>
	<script type="text/javascript" src="js/easyui/locale/easyui-lang-zh_CN.js"></script>
	<script type="text/javascript" src="js/menu.js"></script>
	<script type="text/javascript" src="js/index.js"></script>
	<title>上网行为管理系统</title>
</head>
<body class="easyui-layout" fit="true">
<div data-options="region: 'north'" style="height:68px; overflow:hidden;">
	<div id="pagetop">
		<table>
			<tbody>
			<tr>
				<td width="200px">
					<img src="image/ny_01.png">
				</td>
			</tr>
			</tbody>
		</table>
	</div>
	<div id="blank">
		<table>
			<tbody>
			<tr>
				<td width="7px"> </td>
				<td width="20px">
					<image src="image/blank_left.png"></image>
				</td>
				<td id="loginName" width="250px" title="当前用户: localhost/admin">
					<img src="image/viewDetail.png">
					当前用户:
					<span id="changeloginName" name="ChangeHostName">localhost</span>
					/admin
				</td>
				<td width="150px">
					监控状态:
					<img src="image/monitorOn.png">  <!--需要继续添加javascript代码，控制监控变化-->
				</td>
				<td id="RandomDailyTIP" width="300px">
					<marquee id="dailytip" loop="1" direction="left" scrollamount="1" align="CENTER" onmouseout="dailytip.start()" onmouseover="dailytip.stop()"></marquee>
				</td>
				<td style="float:right; text-align:right">
					<a>
						<img src="image/blank_right.png">
					</a>&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="#">
						<img title="首页" src="image/home.png" onclick="addTab('首页', '')">
					</a>&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="#">
						<img title="刷新" src="image/refresh.png" onclick="window.location.reload()">
					</a>&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="#">
						<img title="关闭所有选项卡" src="image/close.png" onclick="closeAllTabs();">
					</a>&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="php/reboot.php">
						<img title="应用" src="image/redo.png">
					</a>&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="php/session_destroy.php">
						<img title="退出" src="image/exit.png">
					</a>
				</td>
				<td width="20px"> </td>
			</tr>
			</tbody>
		</table>
	</div>
</div>
<div data-options="region: 'south'" style="height:30px;">
	<h1 style="text-align: center;">系统开发人员：黄冠霖、黄懿衡、张仕勉</h1>
</div>
<div data-options="region: 'west', title: '菜单'" style="width:235px; overflow-x: hidden;">
	<div class="service-menu" style=" ">
		<input class="menu-code-index" type="hidden" value="c849" />
		<!--默认下值设置data-service-index=c203的列表项展开与子列表标签导航高亮显示-->
		<!--<h2 class="menu-title">首页</h2>-->
		<strong class="menu-p"><i class="menu-i"></i><a href="#">首页</a></strong>
		<div class="menu-c">
			<ul>
				<li><a data-service-index="c849" href="#" onclick="addTab('首页', '')"">首页</a></li>
			</ul>
		</div>
		<strong class="menu-p"><i class="menu-i"></i><a href="#">用户和组</a></strong>
		<div class="menu-c">
			<ul>
				<li><a data-service-index="c634" href="#" onclick="addTab('用户组','php/user_group/user_group.php')">用户组</a></li>
				<li><a data-service-index="c502" href="#" onclick="addTab('用户','php/user/user.php')">用户</a></li>
			</ul>
		</div>
		<strong class="menu-p"><i class="menu-i"></i><a href="#">阻断配置</a></strong>
		<div class="menu-c">
			<ul>
				<!--
				<li><a data-service-index="c634" href="#" onclick="addTab('总策略','php/tactics/tactics/tactics.php')">总策略</a></li>
				-->
				<li><a data-service-index="c502" href="#" onclick="addTab('白名单','php/tactics/tactics_allow/tactics_allow.php')">白名单</a></li>
				<li><a data-service-index="c430" href="#" onclick="addTab('个人黑名单','php/tactics/tactics_user/tactics_user.php')">个人黑名单</a></li>
				<li><a data-service-index="c634" href="#" onclick="addTab('组黑名单','php/tactics/tactics_user_group/tactics_user_group.php')">组黑名单</a></li>
				<li><a data-service-index="c634" href="#" onclick="addTab('全局黑名单','php/tactics/tactics_global/tactics_global.php')">全局黑名单</a> </li>
			</ul>
		</div>
		<strong class="menu-p"><i class="menu-i"></i><a href="#">审计记录</a></strong>
		<div class="menu-c">
			<ul>
				<!--<li><a data-service-index="c634" href="#" onclick="addTab('邮件发送审计', 'php/audit/audit_email/audit_email.php')">邮件发送审计</a></li>-->
				<li><a data-service-index="c634" href="#" onclick="addTab('邮件浏览审计', 'php/audit/audit_email_read/audit_email_read.php')">邮件浏览审计</a></li>
				<li><a data-service-index="c634" href="#" onclick="addTab('搜索引擎审计', 'php/audit/audit_search/audit_search.php')">搜索引擎审计</a></li>
				<li><a data-service-index="c634" href="#" onclick="addTab('论坛审计', 'php/audit/audit_bbs/audit_bbs.php')">论坛审计</a></li>
				<li><a data-service-index="c634" href="#" onclick="addTab('网页审计', 'php/audit/audit_http/audit_http.php')">网页审计</a></li>
				<li><a data-service-index="c634" href="#" onclick="addTab('上传审计', 'php/audit/audit_upload/audit_upload.php')">上传审计</a></li>
				<li><a data-service-index="c634" href="#" onclick="addTab('下载审计', 'php/audit/audit_download/audit_download.php')">下载审计</a></li>
			</ul>
		</div>
		<strong class="menu-p"><i class="menu-i"></i><a href="#">网页分类</a></strong>
		<div class="menu-c">
			<ul>
				<li><a data-service-index="c634" href="#" onclick="addTab('网页分类', 'php/urlClassification/urlClassification_see/urlClassification_see.php')">网页分类</a></li>
				<li><a data-service-index="c502" href="#" onclick="addTab('自定义网页分类', 'php/urlClassification/urlClassification_add/urlClassification_add.php')">自定义网页分类</a></li>
			</ul>
		</div>
		<strong class="menu-p"><i class="menu-i"></i><a href="#">系统配置</a></strong>
		<div class="menu-c">
			<ul>
				<li><a data-service-index="c634" href="#" onclick="addTab('系统配置','php/system_configure/system_configure.php')">系统配置</a></li>
			</ul>
		</div>
		<strong class="menu-p"><i class="menu-i"></i><a href="#">帮助</a>   </strong>
		<div class="menu-c">
			<ul>
				<li><a data-service-index="c488" href="#" onclick="addTab('修改密码','php/changePassword/changePassword.php')">修改密码</a></li>
				<li><a data-service-index="c498" href="#" onclick="closeAllTabs();">关闭所有选项卡</a></li>
				<li><a data-service-index="c498" href="php/reboot.php">重启系统</a></li>
				<li><a data-service-index="c498" href="php/session_destroy.php">退出</a></li>
			</ul>
		</div>
	</div>
</div>
<div data-options="region: 'center', border: false" style="background:#eee;">
	<div class="easyui-tabs" id="tabs" fit="true">
		<div title="首页" align="center">
			<ul>
				<li>
					<div id="p" class="easyui-panel" title="Welcome"
					     style="height:80px;padding:10px;background:#fafafa; text-align: center"
					     data-options="iconCls:'icon-save',collapsible:true, fit:true">
						<p style="font-size: 40px;height: 2em;
line-height: 2em;
overflow: hidden;">管理员，欢迎您！</p>
					</div>
				</li>
				<li>
					<img src="image/shouye.png"/>
				</li>
			</ul>
		</div>
	</div>
</div>
</body>
</html>
<?php
}
else {
	//echo '<meta http-equiv="Refresh" content="0, url=login/login.php">';
	echo '<script>window.location.href="login/login.php"</script>';
}
?>
