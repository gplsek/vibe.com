<?php
/*
 * @file
 * Template functions to override node_gallery navigator and for the slideshow page.
 */
function template_preprocess_node_gallery_slideshow(&$variables) {
  drupal_add_css(drupal_get_path('module', 'node_gallery_slideshow') .'/node_gallery_slideshow.css');
  jquery_ui_add(array('ui.core', 'effects.core', 'effects.scale'));
  drupal_add_js(drupal_get_path('module', 'node_gallery_slideshow') .'/js/ng_slideshow.js');
  // pass settings to the js layer
  $settings = array(
    'gid' => $variables['gid'],
    'imageCount' => node_gallery_get_image_count($variables['gid']),
    'intervalTime' => variable_get('node_gallery_slideshow_initial_interval', 3000),
    'chunkSize' => variable_get('node_gallery_slideshow_chunk_size', 100),
    'retrieveInterval' => variable_get('node_gallery_slideshow_retrieve_interval', 100),
    'preloadCount' => variable_get('node_gallery_slideshow_preload_count', 5),
    'enableMicrothumbNavigator' => variable_get('node_gallery_slideshow_enable_microthumb_navigator', TRUE),
    'microthumbCount' => variable_get('node_gallery_slideshow_microthumb_count', 7),
    'imageOffset' => $variables['start_image']-1,
  );
  drupal_add_js(array('node_gallery_slideshow' => $settings), 'setting');
  unset($settings);
}

function template_preprocess_node_gallery_image_navigator_slideshow(&$variables) {
  module_load_include('inc', 'node_gallery', 'theme/theme');
  template_preprocess_node_gallery_image_navigator($variables);
  $variables['slideshow_link'] = url('node/' . $variables['navigator']['gallery_nid'] . '/slideshow/' . $variables['navigator']['current']);
}
