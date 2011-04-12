// javascript to handle jetpack collection module admin interactions
function removeGalleryNode(id) {
	$('#'+id).remove();
}

function searchForGalleryImgNodes() {
	$.ajax({
				type: "POST",
  				url: "/admin/jp_collection/browse_items",
  				data: 'search_term=' + $('#search_term').val(),
  				cache: false,
  				success: function(html){
    				$("#image-results-container").html(html);
    				// TODO turn off progress
      			}
			});
}
function jpCollectionAddImages() {
	window.open('/admin/jp_collection/add_new', 'jp_collection_add_new_popup', 'width=700,height=600,scrollbars=yes,status=yes,resizable=yes,toolbar=no,menubar=no');
}
function updateGalleryImageSettings() {	
	$("#messageTxt").html('Saving Changes...');
	$("#messageBox").css('display', 'block');
	$("#imgSettingsDialog").dialog('close');
	var dataStr = 'gvid=' + $('#vid').val() + '&nid=' + $('#cnid').val() + '&override_title=' + $('#override_title').val() + '&subtitle=' + $('#subtitle').val() + '&override_link=' + $('#override_link').val() + '&imagecache_settings=' + $('#imagecache_settings').val();
	$.ajax({
  		type: "POST",
  		url: "/admin/jp_collection/update_item",
   		data: dataStr,
   		success: function(msg){
	//	var msg = '<div class="warning">' + Drupal.theme('tableDragChangedMarker') + ' ' + Drupal.t("Changes made in this table will not be saved until the form is submitted.") + '</div>';
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

		$(".remove_node").click(function() {
			$("#messageTxt").html('Changes have not been saved.');
			$("#messageBox").css('display', 'block');
			removeGalleryNode(this.parentNode.parentNode.id);
		});
		
		$(".node_settings").click(function() {
			//removeThisNode(this.parentNode.parentNode.parentNode.id);
			//TODO update the values in the dialog form
			var cnid = this.parentNode.parentNode.id.replace('node_', '');
			$('#override_title').val("");
			$('#override_link').val("");
			$('#subtitle').val("");
			$('#imagecache_settings').val("");
			$('#cnid').val(cnid);
			$.ajax({
  				type: "POST",
  				url: "/admin/jp_collection/item_data",
   				data: "cnid=" + cnid + "&vid=" + $("#vid").val(),
   				success: function(msg){
   					var obj = eval("("+msg+")");
   					//alert(obj);
					$('#override_title').val(obj.override_title);
					$('#subtitle').val(obj.subtitle);
					$('#override_link').val(obj.override_link);
					$('#imagecache_settings').val(obj.imagecache_setting);
   				}
 			});
			$('#imgSettingsDialog').dialog('open');
		});

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
			width: 700,
			modal: true,
			buttons: {
				'Add Selected Content': function() {
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
  							 url: "/admin/jp_collection/add_items",
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
		
		$("#imgSettingsDialog").dialog({
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
		});
		//upload images
		$('#new-images').click(function() {
			jpCollectionAddImages();
		});
		
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
			$('#collectionItems .draggable').each(function(){
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
  					url: "/admin/jp_collection/add_items",
   					data: dataStr,
   					success: function(msg){
						window.location.reload();
					}
 				});
		});
		
	}
	
);