// javascript to handle photo gallery module admin interactions
function removeGalleryNode(id) {
	$('#'+id).remove();
}

function searchForGalleryImgNodes() {
	$.ajax({
				type: "POST",
  				url: "/admin/photo_gallery/browse_items",
  				data: 'search_term=' + $('#search_term').val(),
  				cache: false,
  				success: function(html){
    				$("#image-results-container").html(html);
    				// TODO turn off progress
      			}
			});
}

function updateGalleryImageSettings() {	
	$("#messageTxt").html('Saving Changes...');
	$("#messageBox").css('display', 'block');
	//$("#imgSettingsDialog").dialog('close');
	var dataStr = 'gvid=' + $('#vid').val() + '&nid=' + $('#cnid').val() + '&override_title=' + $('#override_title').val() + '&override_link=' + $('#override_link').val() + '&imagecache_settings=' + $('#imagecache_settings').val();
	$.ajax({
  		type: "POST",
  		url: "/admin/photo_gallery/update_item",
   		data: dataStr,
   		success: function(msg){
   			$("#messageTxt").html('Changes have been saved.');
   		}
 	});
}

$('document').ready(function() {
        // set up icon wrappers
		$(".portlet").addClass("ui-widget ui-widget-content ui-helper-clearfix ui-corner-all")
			.find(".portlet-header")
				.addClass("ui-widget-header ui-corner-all")
				.prepend('<span class="remove ui-icon ui-icon-trash"></span><span class="image_settings ui-icon ui-icon-wrench"></span>')
				.end()
			.find(".portlet-content");

		$(".portlet-header .remove").click(function() {
			$("#messageTxt").html('Changes have not been saved.');
			$("#messageBox").css('display', 'block');
			removeGalleryNode(this.parentNode.parentNode.parentNode.id);
		});
		
		/*$(".portlet-header .image_settings").click(function() {
			//removeThisNode(this.parentNode.parentNode.parentNode.id);
			//TODO update the values in the dialog form
			var cnid = this.parentNode.parentNode.parentNode.id.replace('node_', '');
			$('#override_title').val("");
			$('#override_link').val("");
			$('#imagecache_settings').val("");
			$('#cnid').val(cnid);
			$.ajax({
  				type: "POST",
  				url: "/admin/photo_gallery/item_data",
   				data: "cnid=" + cnid + "&vid=" + $("#vid").val(),
   				success: function(msg){
   					var obj = eval("("+msg+")");
   					//alert(obj);
					$('#override_title').val(obj.override_title);
					$('#override_link').val(obj.override_link);
					$('#imagecache_settings').val(obj.imagecache_setting);
   				}
 			});
			$('#imgSettingsDialog').dialog('open');
		});*/

		$("#sortable").sortable({
   				stop: function(event, ui) {
   				    $("#messageTxt").html('Changes have not been saved.');
   					$("#messageBox").css('display', 'block');
   				}
		});
		/*$("#progressbar").progressbar({
			value: 99
		});*/
		// setup dialogs
		$("#dialog").dialog({
			bgiframe: true,
			autoOpen: false,
			height: 400,
			width: 650,
			modal: true,
			buttons: {
				'Add Selected Images': function() {
					var bValid = true;
					//allFields.removeClass('ui-state-error');
					var strOfIds = $('#nodeIdList').val();
					if (bValid) {
						$('input:checkbox:checked').each(function() {
								if (strOfIds.length) { 
									strOfIds += ",";
								}
								strOfIds += this.value;
							}
						);
						var dataStr = 'nid=' + $('#nid').val() + '&vid=' + $('#vid').val() + '&nodeIds=' + strOfIds;
						$("#messageTxt").html('Saving Changes...');
						$("#messageBox").css('display', 'block');
						$("#manage-buttons").css('visibility', 'hidden');
						$("#dialog").dialog('close');
						$.ajax({
  							 type: "POST",
  							 url: "/admin/photo_gallery/add_items",
   							 data: dataStr,
   							 success: function(msg){
     								window.location.reload();
   							  }
 						});
					}
				},
		   Cancel: function() {
					$(this).dialog('close');
				}
			},
			close: function() {
				//allFields.val('').removeClass('ui-state-error');
			}
		});
		
		/*$("#imgSettingsDialog").dialog({
			bgiframe: true,
			autoOpen: false,
			height: 400,
			width: 800,
			modal: true,
			buttons: {
				'Submit Changes': function() {
					//var bValid = true;
					//allFields.removeClass('ui-state-error');
					 	updateGalleryImageSettings();
				},
		   Cancel: function() {
					$(this).dialog('close');
				}
			},
			close: function() {
				//allFields.val('').removeClass('ui-state-error');
			}
		});*/
		
		// manage items button events
		$('#add-images').click(function() {
			$('#dialog').dialog('open');
			$('#image-results-container').css('display', 'block');
		})
		.hover(
			function(){ 
				$(this).addClass("ui-state-hover"); 
			},
			function(){ 
				$(this).removeClass("ui-state-hover"); 
			}
		).mousedown(function(){
			$(this).addClass("ui-state-active"); 
		})
		.mouseup(function(){
				$(this).removeClass("ui-state-active");
		});
		
		$('#save-order').click(function() {
			// TODO turn on progress
			var itemOrder = '';
			$('#sortable li').each(function(){
					if (itemOrder.length) {
						itemOrder += ",";
					}
					itemOrder += this.id.replace('node_', '');
					//result.value(' ,'+ (index + 1));
				});
				 $('#nodeIdList').val(itemOrder);
				 //TODO ajax call to save item order, then turn off progress
				 var dataStr = 'nid=' + $('#nid').val() + '&vid=' + $('#vid').val() + '&nodeIds=' + itemOrder;
				$.ajax({
  					type: "POST",
  					url: "/admin/photo_gallery/add_items",
   					data: dataStr,
   					success: function(msg){
						window.location.reload();
					}
 				});
		});
		
	}
	
);