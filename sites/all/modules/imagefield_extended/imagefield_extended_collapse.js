$('document').ready ( function () {
	$('.widget-edit').css('display', 'none');
	$('.collapse_widget_link').click(function () {
		var widget_edit =  $(this).parent().parent().find('.widget-edit');
		if (widget_edit.css('display') == 'none') {
			widget_edit.css('display', 'block');
			this.innerHTML = 'Hide Settings';
		} else {
			this.innerHTML = 'Show Settings';
			widget_edit.css('display', 'none');
		}
		
		return false;
	});
});