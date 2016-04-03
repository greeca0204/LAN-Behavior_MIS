function closeAllTabs() {  //关闭所有选项卡
	var object = $('#tabs').tabs('tabs');
	var length = object.length;
	for(var i=1; i<length; i++) {
		var onetab = object[1];  //若要关闭首页，则设i=0; onetab = object[0];
		var title = onetab.panel('options').title;
		$('#tabs').tabs('close', title);
	}
}
