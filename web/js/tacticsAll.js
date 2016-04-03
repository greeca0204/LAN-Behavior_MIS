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