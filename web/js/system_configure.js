$(function() {
	$('#submit').click(function() {

		var serverIP = $('#serverIP').val();
		var DBSERV_IP = $('#DBSERV_IP').val();
		var Gateway = $('#Gateway').val();

		var pattern_IP = /^\d+\.\d+\.\d+\.\d+$/;
		//var pattern_logEffectiveDays = /^\d+$/;
		if(pattern_IP.test(serverIP) && pattern_IP.test(DBSERV_IP) && pattern_IP.test(Gateway)) {
			$.post('post_configure.php', $('#configureForm').serialize(), function(result){
				if(result.success) {
					alert('配置成功！');
				}
				else {
					alert('配置失败！');
				}
			}, 'json')
		}
		else {
		}
	});
	$('#serverIP').blur(function() {
		var serverIP = $('#serverIP').val();
		var pattern = /^\d+\.\d+\.\d+\.\d+$/;
		if(pattern.test(serverIP)) {
			$('#tips1').css({display: "block"});
			$('#tips1').html('OK');
		}
		else {
			$('#tips1').css({display: "block"});
			$('#tips1').html('请输入正确的IP');
		}
	});
	$('#DBSERV_IP').blur(function() {
		var DBSERV_IP = $('#DBSERV_IP').val();
		var pattern = /^\d+\.\d+\.\d+\.\d+$/;
		if(pattern.test(DBSERV_IP)) {
			$('#tips3').css({display: "block"});
			$('#tips3').html('OK');
		}
		else {
			$('#tips3').css({display: "block"});
			$('#tips3').html('请输入正确的IP');
		}
	});
	$('#Gateway').blur(function() {
		var Gateway = $('#Gateway').val();
		var pattern = /^\d+\.\d+\.\d+\.\d+$/;
		if(pattern.test(Gateway)) {
			$('#tips4').css({display: "block"});
			$('#tips4').html('OK');
		}
		else {
			$('#tips4').css({display: "block"});
			$('#tips4').html('请输入正确的IP');
		}
	});
	$('#SUB_NET').blur(function() {
		var SUB_NET = $('#SUB_NET').val();
		var pattern = /^\d+\.\d+\.\d+$/;
		if(pattern.test(SUB_NET)) {
			$('#tips5').css({display: "block"});
			$('#tips5').html('OK');
		}
		else {
			$('#tips5').css({display: "block"});
			$('#tips5').html('请输入正确的网段');
		}
	});
	$('#Fack_gateway_mac').blur(function() {
		var Fack_gateway_mac = $('#Fack_gateway_mac').val();
		var pattern = /^\w+\ \w+\ \w+\ \w+\ \w+\ \w+$/;
		if(pattern.test(Fack_gateway_mac)) {
			$('#tips6').css({display: "block"});
			$('#tips6').html('OK');
		}
		else {
			$('#tips6').css({display: "block"});
			$('#tips6').html('请输入正确的MAC');
		}
	});
});