<?php
// theme functions for photo galleries

/**
  * Implementation of hook_theme
  */
function photo_gallery_theme() {
	return array (
		'photo_gallery_items_admin' => array(
			'arguments' => array('nodes' => array(), 'nid' => NULL, 'vid' => NULL),
		),
	);
		
}

/**
  * display images associated to the gallery
  */
function theme_photo_gallery_items_admin($nodes, $nid, $vid) {
	//drupal_rebuild_theme_registry();
	jquery_ui_add(array('ui.draggable', 'ui.droppable', 'ui.sortable', 'ui.dialog', 'ui.selectable'));
	drupal_add_css(drupal_get_path('module', 'jquery_ui') . '/jquery.ui/themes/smoothness/ui.all.css');
	//drupal_add_css(drupal_get_path('module', 'jquery_ui') . '/jquery.ui/themes/smoothness/jquery-ui.css');
	drupal_add_css(drupal_get_path('module', 'photo_gallery') .'/cssjs/photo_gallery.css');
	drupal_add_js(drupal_get_path('module', 'photo_gallery') . '/cssjs/photo_gallery.js');
	views_add_js('ajax');
	views_add_js('ajax_views');
	$nodeIds = array();
	$output = "<div class='ui-widget' id='messageBox'>
				<div class='ui-state-error ui-corner-all' style='padding: 0 .7em;'> 
					<p><span class='ui-icon ui-icon-alert' style='float: left; margin-right: .3em;'></span> 
					<strong>Alert:</strong><span id='messageTxt'>Changes have not been saved.</span></p>
				</div>

			</div>";

	if (count($nodes)) {
		$output .= "<ul id='sortable'>";
		foreach($nodes as $node) {
	    	$nodeIds[] = $node->nid;
			$image = filefield_get_node_files(node_load($node->nid));
			$image = array_shift($image);
			$imageStr = theme('imagecache', 'square-cropping', $image['filepath'], '', '');
			$output .= "<li id='node_" . $node->nid . "'><div class='portlet'><div class='portlet-header' title='" . $node->title . "'>" . $node->title . "</div><div class='portlet-content'>" . $imageStr . "</div></div></li>";

		}
		$output .= "</ul>";
	} else {
		$output = "<div>Please add some images.</div>";
	}
	$output .= "<input type='hidden' value='" . implode(",", $nodeIds) . "' name='nodeIdList' id='nodeIdList'/>";	
	$output .= "<input type='hidden' value='" . $nid . "' name='nid' id='nid'/>";
	$output .= "<input type='hidden' value='" . $vid . "' name='vid' id='vid'/>";
	$output .= "<br clear='all'/><div id='manage-buttons'><button id='add-images' class='ui-state-default ui-corner-all'>Add Images</button>&nbsp;<button id='save-order' class='ui-state-default ui-corner-all'>Save Changes</button></div>";
	//$output .= "<div id='dialog' title='Choose Images'><div id='search_imgs'><form action='#' onsubmit='searchForGalleryImgNodes(); return false;'><input type='text' name='search_term' id='search_term' size='50' /><input type='submit' value='Search' name='submit' class='searchBtn'></form></div><div id='image-results-container'></div></div>";
	$view = views_get_view('search_image_nodes');
	 $output .= "<div id='dialog' title='Choose Images'><div id='image-results-container'>" . views_embed_view('search_image_nodes', 'page') . "</div></div>";
	//return $view->render('searchMe');
	//$output .= "<div id='imgSettingsDialog' title='Image Settings'><div>" . photo_gallery_image_form(null) . "</div></div>";
	//$output .="<div id='progressbar' class='ui-widget ui-widget-content ui-corner-all'><div style='width: 99%;' class='ui-progressbar-value ui-widget-header ui-corner-left'></div></div>";
	return $output;
}

function photo_gallery_image_form($form_state) {
   $form = array();
   // Description
 
	$form['override_title'] = array(
    	'#type' => 'textfield',
    	'#id' => 'override_title',
    	'#name' => 'override_title',
    	'#title' => t('Override Title'),
    	'#size' => 50,
    	'#maxlength' => 255,
    	'#description' => t('Enter a title here to override the assigned nodes title'),
 	 );
 	 
 	 $form['override_link'] = array(
    	'#type' => 'textfield',
    	'#id' => 'override_link',
    	'#name' => 'override_link',
    	'#title' => t('Override link'),
    	'#size' => 50,
    	'#maxlength' => 255,
    	'#description' => t('Enter a url here to override the assigned nodes url'),
 	 );

 	 $options = array();
 	 $presets = imagecache_presets();
 	 foreach($presets as $preset) {
 	 	$options[$preset['presetname']] = $preset['presetname'];
 	 }
 	 //$options = array('1' => t('Enabled'), '0' => t('Disabled'));
 	 $form['imagecache_settings'] = array(
    	'#type' => 'select',
    	'#title' => t('Image size'),
    	'#id' => 'imagecache_settings',
    	'#name' => 'imagecache_settings',
    	//'#default_value' => , 
    	'#options' => $options,
    	'#description' => t('Choose which version of the image to use'),
 	 );
 	$form['cnid'] = array('#type' => 'hidden', '#value' => '', '#id'=>'cnid', '#name'=>'cnid');
 	return drupal_render($form);
}

/**
  * display images found in search
  */
function photo_gallery_items_browse($nodes) {
	$nodeIds = array();
	$output = "<ul id='image-results'>";
	foreach($nodes as $node) {
	    $nodeIds[] = $node->nid;
		$image = filefield_get_node_files(node_load($node->nid));
		$image = array_shift($image);
		$imageStr = theme('imagecache', 'square-cropping', $image['filepath'], '', '');
		$output .= "<li class='ui-widget-content'><div class='portlet ui-widget ui-widget-content ui-helper-clearfix ui-corner-all'><div class='portlet-header ui-widget-header ui-corner-all'><input type='checkbox' name='imageOptions' value='" . $node->nid . "' />" . $node->title . "</div><div class='portlet-content'>" . $imageStr . "</div></div></li>";
	}
	$output .= "</ul>";
	return $output;
}