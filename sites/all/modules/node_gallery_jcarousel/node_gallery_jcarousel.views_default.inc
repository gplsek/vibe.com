<?php

/**
 * @file
 * Installs all default views included in node_gallery_jcarousel.
 */

/**
 * Implements hook_views_default_views_alter().
 */
function node_gallery_jcarousel_views_default_views_alter(&$views) {
  if (!isset($views['node_gallery_gallery_image_views'])) {
    return;
  }
  $count = 81;
  // Loop through IDs based upon our style plugin name until
  // we find one that is unused.
  while (!empty($views['node_gallery_gallery_image_views']->display['page_'. $count])) {
    $count++;
  }
  $handler = $views['node_gallery_gallery_image_views']->new_display('page', 'jcarousel', 'page_'. $count);
  $handler->override_option('fields', array(
    'field_node_gallery_image_fid' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 0,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'node-gallery-jcarousel-thumbnail_linked',
      'multiple' => array(
        'group' => 0,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => 0,
      ),
      'exclude' => 0,
      'id' => 'field_node_gallery_image_fid',
      'table' => 'node_data_field_node_gallery_image',
      'field' => 'field_node_gallery_image_fid',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('use_ajax', TRUE);
  $handler->override_option('items_per_page', 7);
  $handler->override_option('offset', NULL);
  $handler->override_option('use_pager', NULL);
  $handler->override_option('pager_element', NULL);
  $handler->override_option('style_plugin', 'jcarousel');
  $handler->override_option('style_options', array(
    'wrap' => '0',
    'skin' => 'tango',
    'visible' => '',
    'scroll' => '',
    'auto' => '0',
    'animation' => '',
    'easing' => '',
    'start' => '1',
    'vertical' => 0,
  ));
  $handler->override_option('row_plugin', 'fields');
  $handler->override_option('row_options', array(
    'inline' => array(),
    'separator' => '',
    'hide_empty' => 0,
  ));
  $handler->override_option('path', 'node-gallery-jcarousel/%');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
    
}