<?php
/* 
 * @file
 * Template functions to override jcarousel's css with our own.
 */
function theme_node_gallery_jcarousel($image, $viewkey) {
  // Adding CSS as theme so we can be sure to come after jcarousel's css
  drupal_add_css(drupal_get_path('module', 'node_gallery_jcarousel') .'/node_gallery_jcarousel.css', 'theme', 'all', FALSE);
  $view = views_get_view($viewkey['name']);
  if (!$view) {
    return;
  }
  $view->set_display($viewkey['display_id']);
  $view->set_arguments(array($image->gid));          
  
  $pos = node_gallery_get_image_position($image->gid, $image->nid);
  $last = node_gallery_get_image_count($image->gid);
  $viewsize = $view->display[$viewkey['display_id']]->handler->options['style_options']['visible'];
  // @todo: make 7 a variable_get()
  $size = $viewsize > 0 ? $viewsize : 7;
  $maxoffset = $last <= $size ? 0 : $last - $size;
  $offset = min($pos - intval($size/2) - 1, $maxoffset);
  if ($offset <= 0) {
    $offset = NULL;
  }
  $view->display[$viewkey['display_id']]->handler->override_option('offset', $offset);
  $output = $view->preview();
  return $output;
}
