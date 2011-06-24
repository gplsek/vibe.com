//jQuery Event Bindings
Drupal.behaviors.gallery_swfupload = function() {
	
	$('input[name="cancel-all"]').click(function() {
		$('a.cancel-upload').each(function(){
			swfu_cancel_file($(this).data('file-id'));
		});
		
		return false;
	});
	
	$('input[name="upload-all"]').click(function() {
	
		$('a.cancel-upload').each(function(){
			swfu.startUpload($(this).data('file-id'));
		});
		
		$(this).attr('disabled', 'disabled');
		
		return false;
	});
}

Drupal.theme.swf_message = function(status, str) {
	/* return $(''). */
}




// Utility & Theming Functions
function swfu_cancel_file(id) {
	$('div#upload-file-'+id).fadeOut('slow', function() {
		$(this).remove();
	});
	swfu.cancelUpload(id, false);		
	swfu_toggle_buttons();
}

function swfu_queue_file(file) {
	var div = '';
	
	div += '<div class="upload-file" id="upload-file-'+file.id+'">';
		div += '<div class="upload upload-file-name" id="upload-file-name-'+file.id+'">';
			div += file.name+' ('+swfu_file_size(file)+'KB)';
		div += '</div>';
		div += '<div id="upload-file-cancel-'+file.id+'" class="upload-file-cancel"><a href="#'+file.id+'" id="cancel-'+file.id+'" class="cancel-upload"><span>cancel</span></a></div>';
		div += '<div class="clear-block"></div>';		
		div += '<div id="upload-progress-'+file.id+'" class="upload-progress">';
			div += '<div id="progress-bar-'+file.id+'" class="progress-bar"></div>';
		div += '</div>';		
	div += '</div>';
	$('#upload-queue').append(div);
	
	$('a#cancel-'+file.id).data('file-id', file.id);	
	
	$('a#cancel-'+file.id).click(function() {	
		swfu_cancel_file($(this).data('file-id'));
	});
	swfu_toggle_buttons();
}

function swfu_toggle_buttons() {
	var stats = swfu.getStats();
	
	if(stats.files_queued > 0) {
		$('input[name="cancel-all"]').removeAttr('disabled');
		$('input[name="upload-all"]').removeAttr('disabled');
	} else {
		$('input[name="cancel-all"]').attr('disabled', 'disabled');
		$('input[name="upload-all"]').attr('disabled', 'disabled');
	}
}

function swfu_file_size(file) {
	var size = file.size / 1024; 
	return size.toFixed(2);
}

function fileQueued(file) {	
	try {
		swfu_queue_file(file);		
	} catch (ex) {
		this.debug(ex);
	}
}

function fileQueueError(file, errorCode, message) {
	try {
		if (errorCode === SWFUpload.QUEUE_ERROR.QUEUE_LIMIT_EXCEEDED) {
			alert("You have attempted to queue too many files.\n" + (message === 0 ? "You have reached the upload limit." : "You may select " + (message > 1 ? "up to " + message + " files." : "one file.")));
			return;
		}

		var progress = new FileProgress(file, this.customSettings.progressTarget);
		progress.setError();
		progress.toggleCancel(false);

		switch (errorCode) {
		case SWFUpload.QUEUE_ERROR.FILE_EXCEEDS_SIZE_LIMIT:
			progress.setStatus("File is too big.");
			this.debug("Error Code: File too big, File name: " + file.name + ", File size: " + file.size + ", Message: " + message);
			break;
		case SWFUpload.QUEUE_ERROR.ZERO_BYTE_FILE:
			progress.setStatus("Cannot upload Zero Byte files.");
			this.debug("Error Code: Zero byte file, File name: " + file.name + ", File size: " + file.size + ", Message: " + message);
			break;
		case SWFUpload.QUEUE_ERROR.INVALID_FILETYPE:
			progress.setStatus("Invalid File Type.");
			this.debug("Error Code: Invalid File Type, File name: " + file.name + ", File size: " + file.size + ", Message: " + message);
			break;
		default:
			if (file !== null) {
				progress.setStatus("Unhandled Error");
			}
			this.debug("Error Code: " + errorCode + ", File name: " + file.name + ", File size: " + file.size + ", Message: " + message);
			break;
		}
	} catch (ex) {
        this.debug(ex);
    }
}

function fileDialogComplete(numFilesSelected, numFilesQueued) {	
	$('#upload-queue .messages').remove();		
	try {	
		if (numFilesSelected > 0) {
			$('input[name="cancel-all"]').removeAttr('disabled');
		}
	} catch (ex)  {
        this.debug(ex);
	}
}

function uploadStart(file) {
	$('input[name="upload-all"]').attr('disabled', 'disabled');	
}


/**
 * This function reads progress information.
 * It reveals the progress bar and expands the inner div to reflect upload progress.
 */
function uploadProgress(file, bytesLoaded, bytesTotal) {
	try {
		var percent = Math.ceil((bytesLoaded / bytesTotal) * 100);
		$('div#progress-bar-'+file.id).css('width', percent+'%');		
	} catch (e) {
		
	}
}

/**
 * This function is huge.
 */
function uploadError(file, errorCode, message) {
}

/**
 * This function handles data that is returned from the PHP script.
 * That data is just whatever the PHP script echos to the page (no JSON or XML).
 * This function expects an ID representing the new file and places it
 * in the hidden field in the form.
 * It also sets the custom 'upload_successful' parameter to true for use later.
 */
function uploadSuccess(file, serverData, serverResponse) {
	try {
		var data = Drupal.parseJson(serverData);
		var messages = $(data.messages).attr('id', file.id);
		if(data.status) {
			messages.text(messages.text() + ': ' + file.name);
		}
		
		$('#upload-queue').append(messages);
		
	} catch(e) {
		//handle error
	}	
}

/**
 * If the upload was completed successfully, this function calls the function
 * that submits the entire form via AJAX.
 * If the upload was unsuccessful, this function alerts an error message.
 */
function uploadComplete(file) {

	var stats = swfu.getStats();

	$('div#progress-bar-'+file.id).css('width', '100%');	
	$('div#upload-file-'+file.id).fadeOut('slow', function() {
		$(this).remove();
	});	
	
	if(stats.files_queued == 0) {
		$('input[name="cancel-all"]').attr('disabled', 'disabled');
	}
}

/**
 * This file decides what do to after the entire form has been submitted.
 * It expects a JSON object with a 'message' and 'status' value.
 * The message value is always displayed to the user.
 * If the server returns a successful status, the upload and processing of form
 * data is complete and the user is redirected to another page.
 * If the server returns an unsuccessful status, the form submit action
 * is reconfigured to try submitted the form again rather than try the entire upload.
 */
function fileUploaded(json) {
/*
    alert(json.message);
    if (json.status == 1) {
        window.location = 'upload_successful.php';
    } else {
        form.setAttribute('onsubmit','submitForm(this);return false;');
    }
*/
}

function show_props(obj) {	
	var result = "";	
	for (var i in obj) {
		result += i + " = " + obj[i] + "\n";
	}	
	console.log(result);
}














































