$(function() {
	$('#submit').click(function() {
		var Name = $('#Name').val();
		if(Name != '') {
			$.post('addClass.php', $('#addForm').serialize(), function(result) {
				if(result.success) {
					alert("添加成功");
					$('#Name').val('');
					$('#Url').val('');
				}
				else {
					$.messager.alert('错误', result.msg, 'error');
				}
			}, 'json');
		}
	});
});