<?php

/**
 * @file
 * Token module support for the Node Gallery module.
 */

/**
 * Implements hook_token_list().
 */
function node_gallery_token_list($type = 'all') {
  $tokens = array();
  if ($type == 'node' || $type == 'all') {
    $tokens['node']['parent-node-gallery-path'] = t('The path of the parent Gallery.');
    $tokens['node']['parent-node-gallery-path-raw'] = t('Unfiltered path of the parent Gallery.  WARNING - raw user input.');
    $tokens['node']['parent-node-gallery-title'] = t('The title of the parent Gallery.');
    $tokens['node']['parent-node-gallery-title-raw'] = t('Unfiltered title of the parent Gallery.  WARNING - raw user input.');
    $tokens['node']['parent-node-gallery-nid'] = t('The NID of the parent Gallery.');
    return $tokens;
  }
}

/**
 * Implements hook_token_values().
 */
function node_gallery_token_values($type, $object = NULL, $options = array()) {
  $tokens = array();
  if ($type == 'node' && in_array($object->type, (array)node_gallery_get_types('image'))) {

    if (!is_numeric($object->gid)) {
      $object->gid = db_result(db_query("SELECT gid FROM {node_gallery_images} WHERE nid = %d", $object->nid));
    }
    if (is_numeric($object->gid)) {
      $parent_path = 'node/'. $object->gid;
      if (module_exists('path')) {
        $parent_path = drupal_get_path_alias($parent_path);
      }

      // Load up the title of the parent
      $parent_title = db_result(db_query("SELECT title FROM {node} WHERE nid = '". $object->gid ."'"));
      $tokens['parent-node-gallery-path'] = decode_entities(check_plain($parent_path));
      $tokens['parent-node-gallery-path-raw'] = $parent_path;
      $tokens['parent-node-gallery-title'] = decode_entities(check_plain($parent_title));
      $tokens['parent-node-gallery-title-raw'] = $parent_title;
      $tokens['parent-node-gallery-nid'] = $object->gid;
    }
    return $tokens;
  }
}
