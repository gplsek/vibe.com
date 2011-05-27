<?php
// $Id: node_gallery.inc,v 1.1.2.21 2010/05/18 17:12:20 justintime Exp $

/**
 * @file
 * 
 * Node gallery models
 */

/**
 * Get node gallery config
 * 
 * the config was stored in variable table with this format: node_gallery_(gallery_type)-(image_type);
 *
 * @param unknown_type $gallery_type
 * @return unknown
 */
function node_gallery_get_config($gallery_type = NULL) {
  static $ng_configs = array();
  
  if (empty($ng_configs)) {
    global $conf;
    $node_types = array_keys(node_get_types());
    foreach ($conf as $key => $value) {
      if (strpos($key, 'node_gallery_') === 0 && !empty($value)) {
        $t = substr($key, 13);
        if (in_array($t, $node_types)) {
          $ng_configs[$t] = $value;
        }
      }
    }
  }
  if (!empty($gallery_type)) {
    return $ng_configs[$gallery_type];
  }
  return $ng_configs;
}

/**
 * get node gallery types list
 *
 * @param string $type: 'gallery', 'image' or 'all'
 * @return unknown
 */
function node_gallery_get_types($type = 'gallery') {
  static $ng_types = array();
  
  if (empty($ng_types)) {
    $ng_configs = node_gallery_get_config();
    $ng_types['gallery'] = array();
    $ng_types['image'] = array();
    foreach ($ng_configs as $key => $value) {
      $ng_types['gallery'][] = $key;
      $ng_types['image'][] = $value['image_type'];
    }
  }
  if ($type == 'all') {
    return array_merge($ng_types['gallery'], $ng_types['image']);
  }
  else {
    return $ng_types[$type];
  }
}

function node_gallery_get_list($type, $uid = NULL) {
  $sql = "SELECT n.nid, n.title FROM {node} n WHERE n.type = '%s'";
  $args[] = $type;
  if ($uid) {
    $sql .= " AND n.uid = %d ";
    $args[] = $uid;
  }
  $result = db_query($sql, $args);
  while ($r = db_fetch_array($result)) {
    $items[$r['nid']] = $r['title'];
  }
  return $items;
}

function node_gallery_get_gallery_list($uid = NULL, $gallery_type = NULL) {
  $items = array();
  
  $gallery_types = empty($gallery_type) ? node_gallery_get_types() : array($gallery_type);
  $args = $gallery_types;
  $sql = "SELECT n.nid, n.title, n.type, n.created, n.uid FROM {node} n 
    WHERE n.type IN (". db_placeholders($gallery_types, 'varchar') .") AND n.status = 1";
  if ($uid) {
    $sql .= " AND n.uid = %d";
    $args[] = $uid;
  }
  $result = pager_query(db_rewrite_sql($sql), variable_get('node_gallery_page_number', 20), 0, NULL, $args);
  while ($object = db_fetch_object($result)) {
    $items[$object->nid] = $object;
  }
  if (!empty($items)) {
    $gids = array_keys($items);
    //we only need cover image when display gallery list;
    $result = db_query("SELECT ng.*, f.* FROM {node_galleries} ng INNER JOIN {files} f ON ng.fid = f.fid WHERE ng.gid IN (".
      db_placeholders($gids) .") AND ng.is_cover > 0", $gids);
    $i = 0;
    while ($object = db_fetch_object($result)) {
      if (!empty($object->nid) && !empty($object->gid)) {
        //we can't use $object->gid because it could be null;
        $items[$object->gid]->images[] = $object;
        $i++;
      }
    }
    //get the images count for display;
    $result = db_query("SELECT ng.gid, COUNT(ng.nid) AS count FROM {node_galleries} ng JOIN {node} n on n.nid = ng.nid WHERE gid IN (". db_placeholders($gids) .") and n.status = 1 GROUP BY gid", $gids);
    while ($r = db_fetch_array($result)) {
      $items[$r['gid']]->images_count = $r['count'];
    }
  }

  return $items;
}

function node_gallery_get_all_images($node) {
  $items = array();
  
  if (in_array($node->type, node_gallery_get_types())) {
    $result = db_query("SELECT * FROM {node_galleries} WHERE gid = %d", $node->nid);
    while ($o = db_fetch_object($result)) {
      $items[$o->nid] = $o;
    }
  }
  return $items;
}

function node_gallery_get_gallery_images(&$node, $ignore_pager = FALSE) {
  if (in_array($node->type, node_gallery_get_types())) {
    $sql = "SELECT n.nid, n.vid, n.title, n.type, n.created, nr.body, ng.*, f.* FROM {node} n 
      INNER JOIN {node_revisions} nr ON n.vid = nr.vid INNER JOIN {node_galleries} ng 
      ON n.nid = ng.nid INNER JOIN {files} f ON ng.fid = f.fid WHERE n.nid IN (SELECT g.nid FROM {node_galleries} g WHERE g.gid = %d) AND n.status = 1
      ORDER BY ng.weight, ng.nid";
    if ($ignore_pager == FALSE) {
      $result = pager_query(db_rewrite_sql($sql), variable_get('node_images_page_number', 20), NODE_GALLERY_IMAGE_PAGER_ELEMENT, NULL, $node->nid);
    }
    else {
      $result = db_query(db_rewrite_sql($sql), $node->nid);
    }
    
    $record_counter = 0;
    $images = array();
    while ($object = db_fetch_object($result)) {
      $record_counter++;
      if ($record_counter % 50 == 0) {
        #Clear the cache every 50 nodes
        node_load(FALSE, NULL, TRUE);
      }
      $image = node_load($object->nid);
      $images[$image->fid] = $image;
    }
    //get the cck field content;
    if (module_exists('content') && !empty($images) && is_array($images)) {
      foreach ($images as &$item) {
        $content_type = content_types($item->type);
        if (!empty($content_type['fields'])) {
          content_load($item);
        }
      }
    }
    return $images;
  }
}

function node_gallery_get_cover($gallery) {
  if (!empty($gallery->images) && is_array($gallery->images)) {
    foreach ($gallery->images as $image) {
      if ($image->is_cover) {
        $cover_image = $image;
        break;
      }
    }
  }
  /*$cover_image = db_fetch_object(db_query("SELECT n.title, ng.*, f.* FROM {node} n INNER JOIN {node_galleries} ng ON n.nid = ng.nid
   INNER JOIN {files} f ON ng.fid = f.fid WHERE ng.gid = %d AND is_cover > 0", $gallery->gid));*/
  $config = node_gallery_get_config($gallery->type);
  if (empty($cover_image)) {
    $cover_image->filepath = $config['default_cover'];
  }
  $cover_image->title = $gallery->title;
  return $cover_image;
}

function node_gallery_get_image_navigator($gid, $nid) {
  $result = db_query("SELECT ng.nid FROM {node_galleries} ng JOIN {node} n ON ng.nid = n.nid WHERE n.status = 1 AND gid = %d ORDER BY weight, nid", $gid);
  while ($r = db_fetch_array($result)) {
    $items[] = $r['nid'];
  }
  $navigator['total'] = count($items);
  $navigator['parent'] = $gid;
  for ($i = 0; $i < $navigator['total']; $i++) {
    if ($items[$i] == $nid) {
      $navigator['current'] = $i + 1;
      $navigator['prev_nid'] = ($i == 0) ? $items[$navigator['total'] - 1] : $items[$i - 1];
      $navigator['next_nid'] = ($i == ($navigator['total'] - 1)) ? $items[0] : $items[$i + 1];
    }
  }
  return $navigator;
}

function node_gallery_get_gallery_content($gid, $teaser = 0) {
  $node = db_fetch_object(db_query("SELECT teaser, body, format FROM {node_revisions} WHERE nid = %d", $gid));
  $node->body = isset($node->body) ? str_replace('<!--break-->', '', $node->body) : '';
  $node = node_prepare($node, $teaser);
  return $node->content['body']['#value'];
}

function node_gallery_get_image_parent_gallery_config($image) {
  static $image_gallery_configs;
  
  //use the $image->nid as a token, because we don't know the image's parent gallery type.
  if (empty($image_gallery_configs[$image->nid])) {
    if (!empty($image->gid)) {
      $parent_type = db_result(db_query("SELECT n.type FROM {node} n INNER JOIN {node_galleries} ng ON n.nid = ng.gid
       WHERE ng.nid = %d", $image->nid));
       $image_gallery_configs[$image->nid] = node_gallery_get_config($parent_type);
    }
  }
  return $image_gallery_configs[$image->nid];
}

function node_gallery_get_image_file($node) {
  $image = db_fetch_object(db_query("SELECT i.*, f.* FROM {node_galleries} i INNER JOIN {files} f ON i.fid = f.fid WHERE i.nid = %d", $node->nid));
  // We have to unset the uid from the files table or else the node uid is
  // lost.  See http://drupal.org/node/637958
  unset($image->uid);
  return $image;
}

function node_gallery_image_save($image_node) {

  module_load_include('inc', 'node', 'node.pages');
  node_object_prepare($image_node);
  $form_state['values'] = (array)$image_node;
  //orignal form;
  $form = drupal_retrieve_form($image_node->type .'_node_form', $form_state, $image_node);
  
  //form's hook #submit and form_alter;
  drupal_prepare_form($image_node->type, $form, $form_state);
  
  node_form_submit($form, $form_state);
  $image_node->nid = $image_node->nid = $form_state['nid'];
  
  file_set_status($image_node, FILE_STATUS_PERMANENT);
  if ($image_node->gid && $image_node->nid) {
    //remove the currently cover flag;
    if ($image_node->is_cover) {
      db_query("UPDATE {node_galleries} SET is_cover = 0 WHERE gid = %d", $image_node->gid);
    }
    //update
    if (db_result(db_query("SELECT nid FROM {node_galleries} WHERE nid = %d", $image_node->nid))) {
      drupal_write_record('node_galleries', $image_node, 'nid');
    }
    //insert
    else {
      drupal_write_record('node_galleries', $image_node);
    }
  }
}

function node_gallery_set_cover($gid) {
  $cover_id = db_result(db_query("SELECT nid FROM {node_galleries} WHERE is_cover > 0 AND gid = %d", $gid));
  if (empty($cover_id)) {
    $first_nid = db_result(db_query_range("SELECT nid FROM {node_galleries} WHERE gid = %d ORDER BY weight", $gid, 0, 1));
    db_query("UPDATE {node_galleries} SET is_cover = 1 WHERE nid = %d AND gid = %d", $first_nid, $gid);
  }
}

function node_gallery_clean_empty_dirs($dirname) {
  //Recursively delete parent directories that are empty, up to the 'files' directory.
  $files = file_scan_directory($dirname, '.*', array('.', '..'), 0, FALSE);
  if (count($files) > 0 || $dirname == file_directory_path()) {
    return;
  } 
  else {
    rmdir($dirname);
    $dirname = dirname($dirname);
    node_gallery_clean_empty_dirs($dirname);
  }    
}

function node_gallery_delete_image($node) {
  //Clean up our tables
  db_query("DELETE FROM {node_galleries} WHERE nid = %d", $node->nid);
  db_query("DELETE FROM {files} WHERE fid = %d", $node->fid);
  //Delete the file from the filesystem
  file_delete($node->filepath);
  //Clean our imagecache - note, this API call leaves empty dirs
  imagecache_image_flush($node->filepath);
  //Clean up empty dirs, if they exist
  node_gallery_clean_empty_dirs(dirname($node->filepath));
}