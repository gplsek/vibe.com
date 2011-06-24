var swfu;
	window.onload = function () {
	var settings_object = {
		flash_url: '/sites/all/modules/gallery/swfupload/swfupload.swf',
		upload_url: '/gallery/swfupload',
		file_size_limit: '300 MB',
		file_types : "*.jpg;*.png;*.gif",
		button_image_url: '/sites/all/modules/gallery/images/upload-button.jpg',
		button_placeholder_id: 'swfupload-button',		
		button_width: '200',
		button_height: '35',
		post_params : {
			'gallery_nid' : $('input[name="gallery_nid"]').val(),
			'token' : Drupal.settings.gallery.token			
		},
		custom_settings : {
			progress : "upload-progress",
			cancelButtonId : "cancel-button"
		},
		file_queued_handler: fileQueued,
		file_queue_error_handler : fileQueueError,
		file_dialog_complete_handler : fileDialogComplete,
		upload_start_handler : uploadStart,
		upload_progress_handler: uploadProgress,
		upload_error_handler: uploadError,
		upload_success_handler: uploadSuccess,
		upload_complete_handler: uploadComplete
	};	
	swfu = new SWFUpload(settings_object);
};