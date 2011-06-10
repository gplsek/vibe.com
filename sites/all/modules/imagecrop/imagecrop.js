/* $Id: imagecrop.js,v 1.1.4.6 2009/05/29 22:44:59 longwave Exp $ */

/*
 * Toolbox @copyright from imagefield_crop module with some minor modifications.
 * To be used with Jquery UI.
 */

$(document).ready(function(){
	if ($('#resizeMe').resizable) {
	  $('#resizeMe').resizable({
		containment: $('#image-crop-container'),
		//proxy: 'proxy',
		//ghost: true,
		//animate:true,
		handles: 'all',
		knobHandles: true,
		//transparent: true,
		aspectRatio: false,
		autohide: true,

		resize: function(e, ui) {
  		  this.style.backgroundPosition = '-' + (ui.position.left) + 'px -' + (ui.position.top) + 'px';
		  $("#edit-image-crop-width").val($('#resizeMe').width());
		  $("#edit-image-crop-height").val($('#resizeMe').height());
		  $("#edit-image-crop-x").val(ui.position.left);
		  $("#edit-image-crop-y").val(ui.position.top);
  	    },
		stop: function(e, ui) {
  		  this.style.backgroundPosition = '-' + (ui.position.left) + 'px -' + (ui.position.top) + 'px';
  	    }
	  })
	}

	$('#resizeMe').draggable({
		cursor: 'move',
		containment: $('#image-crop-container'),
		drag: function(e, ui) {
		  this.style.backgroundPosition = '-' + (ui.position.left) + 'px -' + (ui.position.top) + 'px';
		  $("#edit-image-crop-x").val(ui.position.left);
		  $("#edit-image-crop-y").val(ui.position.top);
		}
	});
	
	$('#image-crop-container').css({ opacity: 0.5 });
    var leftpos = $('#edit-image-crop-x').val();
    var toppos = $('#edit-image-crop-y').val();
    $("#resizeMe").css({backgroundPosition: '-'+ leftpos + 'px -'+ toppos +'px'});
    $("#resizeMe").width($('#edit-image-crop-width').val() + 'px');
    $("#resizeMe").height($('#edit-image-crop-height').val() + 'px');
    $("#resizeMe").css({top: $('#edit-image-crop-y').val() +'px' });
    $("#resizeMe").css({left: $('#edit-image-crop-x').val() +'px' });
});
