<?php
// theme functions for jetpack collections

/**
  * Implementation of hook_theme
  */
function jp_collection_theme() {
	return array (
		'jp_collection_items_admin' => array(
			'arguments' => array('nodes' => array(), 'nid' => NULL, 'vid' => NULL),
		),
	    'jp_collection_popup' => array(
			'arguments' => array('content' => NULL)
		)
	);
}

/**
  * display images associated to the gallery
  */
function theme_jp_collection_items_admin($nodes, $nid, $vid) {
	//drupal_rebuild_theme_registry();
	jquery_ui_add(array('ui.dialog'));
	drupal_add_css(drupal_get_path('module', 'jquery_ui') . '/jquery.ui/themes/smoothness/ui.all.css');
	//drupal_add_css(drupal_get_path('module', 'jquery_ui') . '/jquery.ui/themes/smoothness/jquery-ui.css');
	drupal_add_css(drupal_get_path('module', 'jp_collection') .'/cssjs/jp_collection.css');
	drupal_add_js(drupal_get_path('module', 'jp_collection') . '/cssjs/jp_collection.js');
	//views_add_js('ajax');
	//views_add_js('ajax_views');
	$nodeIds = array();
	$output = "<div class='ui-widget' id='messageBox'>
				<div class='ui-state-error ui-corner-all' style='padding: 0 .7em;'> 
					<p><span class='ui-icon ui-icon-alert' style='float: left; margin-right: .3em;'></span> 
					<strong>Alert:</strong><span id='messageTxt'>Changes have not been saved.</span></p>
				</div>

			</div>";
	if (count($nodes)) {
	$items = array();
		//$output .= "<ul id='sortable'>";
		$nCount = 0;
		foreach($nodes as $node) {
	    	$nodeIds[] = $node->nid;
			$image = filefield_get_node_files(node_load($node->nid));
			$image = array_shift($image);
			$imageStr = theme('imagecache', 'image-55x55', $image['filepath'], '', '');
			//$output .= "<li id='node_" . $node->nid . "'><div class='portlet'><div class='portlet-header' title='" . $node->title . "'><span class='portlet-title'>" . $node->title . "</span></div><div class='portlet-content'>" . $imageStr . "</div></div></li>";
			$data = array("", $node->nid, $node->title, $imageStr, l('Edit', 'node/' . $node->nid . '/edit', array('attributes' =>array('target' => '_blank'))));
			$data[] = "<a href='#' class='node_settings'>Settings</a>";
			$data[] = "<a href='#' class='remove_node'>Remove</a>";
			$items[] = array('data' => $data, 'class' => 'draggable', 'id' => 'node_' . $node->nid); 
		}
		//$output .= "</ul>";
		$output .= theme('table', array('', 'id', 'original title','thumbnail',' Edit Node', 'Configure', 'Remove'), $items, array('id' => 'collectionItems'));
		drupal_add_tabledrag('collectionItems', 'order', 'sibling', '');
	} else {
		$output = "<div>Please add some images.</div>";
	}
	$output .= "<input type='hidden' value='" . implode(",", $nodeIds) . "' name='nodeIdList' id='nodeIdList'/>";	
	$output .= "<input type='hidden' value='" . $nid . "' name='nid' id='nid'/>";
	$output .= "<input type='hidden' value='" . $vid . "' name='vid' id='vid'/>";
	$output .= "<br clear='all'/><div id='manage-buttons'><button id='add-images' class='ui-state-default ui-corner-all'>Browse for Content</button>&nbsp;<button id='save-order' class='ui-state-default ui-corner-all'>Save Changes</button></div>";
	//$output .= "<div id='dialog' title='Choose Images'><div id='search_imgs'><form action='#' onsubmit='searchForGalleryImgNodes(); return false;'><input type='text' name='search_term' id='search_term' size='50' /><input type='submit' value='Search' name='submit' class='searchBtn'></form></div><div id='image-results-container'></div></div>";
	$view = views_get_view('search_image_nodes');
	 $output .= "<div id='dialog' title='Choose Content'><div id='image-results-container'>" . views_embed_view('search_nodes_jpc', 'block') . "</div></div>";
	//return $view->render('searchMe');
	$output .= "<div id='imgSettingsDialog' title='Content Settings'><div>" . jp_collection_image_form(null) . "</div></div>";
	$output .="<div id='progressbar' class='ui-widget ui-widget-content ui-corner-all'><div style='width: 99%;' class='ui-progressbar-value ui-widget-header ui-corner-left'></div></div>";
	return $output;
}

function theme_jp_collection_popup($content) {
  //drupal_add_css(drupal_get_path('module', 'noderef_image_helper') . '/noderef_image_helper.css');
 /* drupal_add_css(drupal_get_path('module', 'views_bulk_operations') . '/views_bulk_operations.css');
  drupal_add_css(drupal_get_path('module', 'admin') . '/theme/reset.css');
  drupal_add_css(drupal_get_path('module', 'admin') . '/theme/style.css');
     */
  $output = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">'."\n";
  $output .= '<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">'."\n";
  $output .= "<head>\n";
  $output .= '<title>'. drupal_get_title() ."</title>\n";
  
  $output .= drupal_get_html_head();
  $output .= drupal_get_css();
  $output .= drupal_get_js();
  // Ensure that img_assist.js is imported last.
 // $path = drupal_get_path('module', 'img_assist') .'/img_assist_popup.css';
  //$output .= "<style type=\"text/css\" media=\"all\">@import \"". base_path() . $path ."\";</style>\n";
  $output .= "<style type=\"text/css\" media=\"all\"> body { background-image: none !important; background-color: #ffffff !important; } </style>";
  $output .= "</head>\n";
  $output .= "<body class=\"admin\">\n";
  
  $output .= theme_status_messages();
  
  $output .= "<div class=\"content\">\n";
  $output .= $content;
  $output .= "</div>\n";
  $output .= '</body>';
  $output .= '</html>';
  return $output;
}

function jp_collection_image_form($form_state) {
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
 	 $form['subtitle'] = array(
    	'#type' => 'textfield',
    	'#id' => 'subtitle',
    	'#name' => 'subtitle',
    	'#title' => t('Subtitle'),
    	'#size' => 50,
    	'#maxlength' => 255,
    	'#description' => t('Enter a subtitle'),
 	 );
 	 
 	 $form['override_link'] = array(
    	'#type' => 'hidden',
    	'#id' => 'override_link',
 	 	'#value' => '',
    	'#name' => 'override_link',
    	'#title' => t('Override link'),
    	'#size' => 50,
    	'#maxlength' => 255,
    	'#description' => t('Enter a url here to override the assigned nodes url'),
 	 );

 	 $form['imagecache_settings'] = array(
    	'#type' => 'hidden',
    	'#id' => 'imagecache_settings',
 	 	'#value' => '',
    	'#name' => 'imagecache_settings',
    	'#title' => t('IMage Cache'),
    	'#size' => 50,
    	'#maxlength' => 255,
    	'#description' => t('Enter a url here to override the assigned nodes url'),
 	 );
 	/* $options = array();
 	 $presets = imagecache_presets();
 	 foreach($presets as $preset) {
 	 	$options[$preset['presetname']] = $preset['presetname'];
 	 }*/
 	 //$options = array('1' => t('Enabled'), '0' => t('Disabled'));
 	/* $form['imagecache_settings'] = array(
    	'#type' => 'select',
    	'#title' => t('Image size'),
    	'#id' => 'imagecache_settings',
    	'#name' => 'imagecache_settings',
    	//'#default_value' => , 
    	'#options' => $options,
    	'#description' => t('Choose which version of the image to use'),
 	 );*/
 	 
 	$form['cnid'] = array('#type' => 'hidden', '#value' => '', '#id'=>'cnid', '#name'=>'cnid');
 	return drupal_render($form);
}

/**
  * display images found in search
  */
function jp_collection_items_browse($nodes) {
	$nodeIds = array();
	$output = "<ul id='image-results'>";
	foreach($nodes as $node) {
	    $nodeIds[] = $node->nid;
		$image = filefield_get_node_files(node_load($node->nid));
		$image = array_shift($image);
		$imageStr = theme('imagecache', 'image-thumb', $image['filepath'], '', '');
		$output .= "<li class='ui-widget-content'><div class='portlet ui-widget ui-widget-content ui-helper-clearfix ui-corner-all'><div class='portlet-header ui-widget-header ui-corner-all'><input type='checkbox' name='imageOptions' value='" . $node->nid . "' />" . $node->title . "</div><div class='portlet-content'>" . $imageStr . "</div></div></li>";
	}
	$output .= "</ul>";
	return $output;
}