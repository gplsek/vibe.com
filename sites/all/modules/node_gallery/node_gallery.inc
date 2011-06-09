<?php

/**
 * @file
 * Shared functions for node_gallery
 */

/**
 * Builds an array that associates image types with gallery types.
 *
 * @param $reset
 *   (optional) A boolean that when set to true will clear the cache.
 *
 * @return
 *   An associative array where the keys are the image content types and the
 *   values are the gallery content types.
 */
function node_gallery_get_image_to_gallery_types($reset = FALSE) {
  static $i2g = array();
  if (empty($i2g) || $reset) {
    $result = db_query("SELECT image_type, gallery_type from {node_gallery_relationships}");
    while ($r = db_fetch_array($result)) {
      $i2g[$r['image_type']] = $r['gallery_type'];
    }
  }
  return $i2g;
}

/**
 * Builds an array that associates rid's with gallery types.
 *
 * @param $reset
 *   (optional) A boolean that when set to true will clear the cache.
 *
 * @return
 *   An associative array where the keys are the rid's and the
 *   values are the gallery content types.
 */
function node_gallery_get_rid_to_gallery_types($reset = FALSE) {
  static $r2g = array();
  if (empty($r2g) || $reset) {
    $result = db_query("SELECT rid, gallery_type from {node_gallery_relationships}");
    while ($r = db_fetch_array($result)) {
      $r2g[$r['rid']] = $r['gallery_type'];
    }
  }
  return $r2g;
}

/**
 * Fetches a gallery-to-image relationship from the database.
 *
 * If one of either $gallery_type or $image_type are supplied, this function
 * returns a single relationship.  If neither are specified, it returns all
 * relationships, keyed by the gallery content type.  If both are supplied,
 * $image_type will be ignored.
 *
 * @param $gallery_type
 *   (optional) The content type of the gallery in the relationship.  If
 *   this is an int, we'll use that as the rid.
 *   Defaults to NULL.
 * @param $image_type
 *   (optional) The content type of the image in the relationship.
 *   Defaults to NULL.
 * @param $reset
 *   (optional) Boolean which if set to TRUE, clears the cache.
 *   Defaults to FALSE.
 *
 * @return
 *   An associative array containing:
 *   - rid: An integer representing the rid column in the database.
 *   - gallery_type: A string that is the content type of the gallery.
 *   - image_type: A string that is the content type of the image.
 *   - imagefield_name: A string that is the name of the imagefield for the image.
 *   - settings: An associative array of settings for the relationship.
 */
function node_gallery_get_relationship($gallery_type = NULL, $image_type = NULL, $reset = FALSE) {
  static $ng_relationships = array();
  static $all_fetched = FALSE; //Our var to track whether or not $ng_relationships contains all records

  $sql = "SELECT rid, gallery_type, image_type, imagefield_name, settings from {node_gallery_relationships} ";
  $result = NULL;
  if (isset($image_type)) {
    // Get the single relationship keyed on the image type
    $i2g = node_gallery_get_image_to_gallery_types($reset);
    if (empty($i2g[$image_type])) {
      return array();
    }
    $gallery_type = $i2g[$image_type];
  }
  if (isset($gallery_type)) {
    if (is_numeric($gallery_type)) {
      // Get the single relationship keyed on the rid
      $r2g = node_gallery_get_rid_to_gallery_types($reset);
      if (empty($r2g[$gallery_type])) {
        return array();
      }
      $gallery_type = $r2g[$gallery_type];
    }
    // We want a single relationship
    if (! empty($ng_relationships[$gallery_type]) && ! isset($reset)) {
      // It's cached, return it
      return $ng_relationships[$gallery_type];
    }
    // We need a single relationship not cached
    $sql .= "WHERE gallery_type = '%s'";
    $result = db_query($sql, $gallery_type);
  }
  else {
    // We want all relationships
    if (! empty($ng_relationships) && ! isset($reset) && $all_fetched) {
      // It's cached, return it
      return $ng_relationships;
    }
    // We need all relationships not cached
    $result = db_query($sql);
  }

  while ($r = db_fetch_array($result)) {
    $ng_relationships[$r['gallery_type']]['rid'] = $r['rid'];
    $ng_relationships[$r['gallery_type']]['gallery_type'] = $r['gallery_type'];
    $ng_relationships[$r['gallery_type']]['image_type'] = $r['image_type'];
    $ng_relationships[$r['gallery_type']]['imagefield_name'] = $r['imagefield_name'];
    $ng_relationships[$r['gallery_type']]['settings'] = array_merge(node_gallery_relationship_settings_defaults(), unserialize($r['settings']));
  }
  if (!empty($gallery_type)) {
    return $ng_relationships[$gallery_type];
  }
  // If we get here, we have a newly fetched, fully populated array
  $all_fetched = TRUE;
  return $ng_relationships;
}

function node_gallery_relationship_settings_defaults() {
  $settings = array(
    'name' => 'Node Gallery Default',
    'view_gallery_full_image_display' => serialize(array(
      'name' => 'node_gallery_gallery_image_views',
      'display_id' => 'page_1',
    )),
    'view_gallery_teaser_view_image_display' => serialize(array(
      'name' => 'node_gallery_gallery_image_views',
      'display_id' => 'page_2',
    )),
    'view_navigator_image_display' => serialize(array(
      'name' => 'node_gallery_gallery_image_views',
      'display_id' => 'page_7',
    )),
    'display_navigator' => 1,
    'node_images_page_fragment' => 0,
    'manage_images_fields' => array('title' => 'title'),
    'manage_images_per_page' => 20,
    'manage_images_show_gallery_list' => TRUE,
    'manage_images_enable_rotation' => TRUE,
  );
  drupal_alter('node_gallery_relationship_default_settings', $settings);
  return $settings;
}
/**
 * Convenience wrapper for getting all relationships.
 * @param $reset
 *
 * @return
 *   An associative array of relationship arrays.
 */
function node_gallery_get_all_relationships($reset = FALSE) {
  return node_gallery_get_relationship(NULL, NULL, $reset);
}

/**
 * Deletes a relationship from the database.
 *
 * @param $rid
 *   The value in the rid column in the database.
 */
function node_gallery_delete_relationship($rid) {
  db_query("DELETE FROM {node_gallery_relationships} WHERE rid = %d", $rid);
  menu_cache_clear_all();
}

/**
 * Returns a list of all possible content types of galleries, images, or both.
 *
 * @param $type
 *   (optional) 'gallery', 'image' or 'all'.
 *
 * @return
 *   An array containing the list of content types.
 */
function node_gallery_get_types($type = 'gallery', $reset = FALSE) {
  static $ng_types = array();

  if (empty($ng_types) || $reset) {
    $ng_types['gallery'] = array();
    $ng_types['image'] = array();

    $ng_rels = node_gallery_get_all_relationships($reset);
    foreach ($ng_rels as $gallery_type => $value) {
      $ng_types['gallery'][] = $gallery_type;
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

/**
 * Returns an array of galleries, suitable for use in a form select.
 *
 * @param $type
 *   The content type of the galleries to be queried.
 * @param $uid
 *   (optional) if specified, the list returned will only contain galleries the user can modify.
 *
 * @return
 *   Associative array where the keys are the nid, and the value is the node title.
 */
function node_gallery_get_gallery_list($type, $uid = NULL) {
  $sql = "SELECT n.nid, n.title FROM {node} n WHERE n.type = '%s'";
  $args[] = $type;
  $items = array();
  if ($uid && !node_gallery_user_access('edit', $type)) {
    $sql .= " AND n.uid = %d ";
    $args[] = $uid;
  }
  $result = db_query($sql, $args);
  while ($r = db_fetch_array($result)) {
    $items[$r['nid']] = $r['title'];
  }
  return $items;
}

/**
 * Builds an array of gids.
 *
 * @param $gallery_type
 *   (optional) The content type of the galleries to be queried.  Defaults to all gallery types.
 * @param $uid
 *   (optional) UID to filter by.  Defaults to NULL, or no filter.
 *
 * @return
 *   An array of nids.
 */
function node_gallery_get_gallery_gids($gallery_type = NULL, $uid = NULL) {
  $items = array();

  $gallery_types = empty($gallery_type) ? node_gallery_get_types('gallery') : array($gallery_type);
  $args = $gallery_types;
  $sql = "SELECT n.nid FROM {node} n " .
          "WHERE n.type IN (". db_placeholders($gallery_types, 'varchar') .
          ") AND n.status = 1";
  if ($uid) {
    $sql .= " AND n.uid = %d";
    $args[] = $uid;
  }
  $result = db_query(db_rewrite_sql($sql, 'n', 'nid', $args), $args);
  while ($object = db_fetch_object($result)) {
    $items[] = $object->nid;
  }
  return $items;
}

/**
 *
 * @param $gid
 *   The NID of the parent gallery.
 * @param boolean $sorted
 *   Whether to apply the sorts specified by the view
 * @param boolean $filtered
 *   Whether to apply the filters specified by the view
 *
 * @return
 *   An array of nids representing the images of the gallery
 */

function node_gallery_get_image_nids($gid, $sorted = TRUE, $filtered = TRUE, $fallback = FALSE, $reset = FALSE) {
  static $nids = array();
  $cache_keys[] = 'node_gallery';
  $cache_keys[] = $gid;
  $cache_keys[] = 'image_nids';
  $cache_keys[] = $sorted ? 'sort-true' : 'sort-false';
  $cache_keys[] = $filtered ? 'filtered-true' : 'filtered-false';
  $cache_keys[] = $fallback ? 'fallback-true' : 'fallback-false';
  $cache_key = implode(':', $cache_keys);
  if (!isset($nids[$cache_key]) || $reset) {
    if (!$reset && ($cache = cache_get($cache_key)) && !empty($cache->data)) {
      $nids[$cache_key] = $cache->data;
    }
    else {
      if (!$fallback) {
        $gallery = node_load($gid);
        $relationship = node_gallery_get_relationship($gallery->type);
        $viewkey = unserialize($relationship['settings']['view_navigator_image_display']);
        if (isset($viewkey['name'])) {
          $view = views_get_view($viewkey['name']);
          if (!isset($view->display[$viewkey['display_id']])) {
            $message = t('Unable to find the sort view specified in the !relationship, maybe you need to !revert your view?',
              array(
                '!revert' => l(t('revert'), 'admin/build/views/delete/'. $viewkey['name']),
                '!relationship' => l(t('relationship settings'), 'admin/settings/node-gallery/relationship/settings/'. $relationship['rid']),
              ));
          }
          else {
            $view->set_display($viewkey['display_id']);
            $view->set_arguments(array($gallery->nid));
            if (!$sorted) {
              $sorts = $view->get_items('sort', $viewkey['display_id']);
              foreach ($sorts as $sort) {
                $view->set_item($viewkey['display_id'], 'sort', $sort['id'], NULL);
              }
            }
            if (!$filtered) {
              $filters = $view->get_items('filter', $viewkey['display_id']);
              foreach ($filters as $filter) {
                $view->set_item($viewkey['display_id'], 'filter', $filter['id'], NULL);
              }
            }
            $view->build();
            if (isset($view->query->fields['nid'])) {
              // This is for Views2 and Views3 compatability: http://drupal.org/node/1029840
              $sql = isset($view->query->final_query) ? $view->query->final_query : $view->build_info['query'];
            }
            else {
              $message = t('Found the image sort view, but the \'Node: Nid\' field wasn\'t found.', array());
            }
          }
        }
        else {
          $message = t('Unable to find a navigator image sort view.  Please revisit your !rellink.', array('!rellink' => l(t('Node Gallery relationship settings'), 'admin/settings/node-gallery/relationship/settings/'. $relationship['rid'])));
        }
      }
      if (isset($message) || $fallback) {
        // Use our default sql
        if (isset($message)) {
          drupal_set_message($message, 'warning');
        }
        if ($filtered) {
          $where = 'n.status = 1 AND';
        }
        if ($sorted) {
          $orderby = 'ORDER BY weight ASC, n.nid DESC';
        }
        $sql = "SELECT ngi.nid FROM {node_gallery_images} ngi JOIN {node} n on ngi.nid = n.nid WHERE $where gid = %d $orderby";
      }
      $result = db_query($sql, $gid);
      $nids[$cache_key] = array();
      while ($r = db_fetch_array($result)) {
        $nids[$cache_key][] = $r['nid'];
      }
      cache_set($cache_key, $nids[$cache_key], 'cache', CACHE_TEMPORARY);
    }
  }
  return $nids[$cache_key];
}

/**
 * Retrieves image properties sorted by the image sort view in a gallery, the returned objects are NOT complete nodes.
 *
 * @param object $gallery The node object of the gallery.
 * @param boolean $sorted Whether to keep the order as effective in the UI. Defaults to true.
 * @param boolean $filtered Whether to filter down the resulting images by publish status.
 *
 * @return array An array of image objects with the basic properties nid, title, status, created, changed, filepath and weight.
 */
function node_gallery_get_images($gallery, $sorted = TRUE, $filtered = TRUE, $reset = FALSE) {
  static $images = array();
  $cache_keys[] = 'node_gallery';
  $cache_keys[] = $gallery->nid;
  $cache_keys[] = 'images';
  $cache_keys[] = $sorted ? 'sort-true' : 'sort-false';
  $cache_keys[] = $filtered ? 'filtered-true' : 'filtered-false';
  $cache_key = implode(':', $cache_keys);
  if (!isset($images[$cache_key]) || $reset) {
    if (!$reset && ($cache = cache_get($cache_key)) && !empty($cache->data)) {
      return $cache->data;
    }
    else {
      $images[$cache_key] = array();
      // get the order currently in effect; the criteria is unknown here because the user can change the view.
      $image_nids = node_gallery_get_image_nids($gallery->nid, $sorted, $filtered);
      if (count($image_nids) == 0) {
        cache_set($cache_key, $images[$cache_key], 'cache', CACHE_TEMPORARY);
        return $images[$cache_key];
      }
      // get CCK information for retrieving the filepath
      $relationship = node_gallery_get_relationship($gallery->type);
      $field = content_fields($relationship['imagefield_name']);
      $field_db_info = content_database_info($field);
      $field_db_table = $field_db_info['table'];
      $field_db_column = $field_db_info['columns']['fid']['column'];

      $sql = 'SELECT n.nid,n.title,n.status,n.created,n.changed,f.filepath,ngi.weight FROM {node} n'
              . ' INNER JOIN {node_revisions} r ON r.vid = n.vid'
              . ' INNER JOIN {' . $field_db_table . '} cck ON cck.nid = n.nid'
              . ' INNER JOIN {files} f ON f.fid = cck.' . $field_db_column
              . ' INNER JOIN {node_gallery_images} ngi ON ngi.nid = n.nid'
              . ' WHERE n.nid IN (' . db_placeholders($image_nids) . ')';
      if ($filtered) {
        $sql .= ' AND n.status = 1';
      }
      $result = db_query($sql, $image_nids);
      $unordered = array();
      while ($image = db_fetch_object($result)) {
        $unordered[$image->nid] = $image;
      }
      if ($sorted) {
        foreach ($image_nids as $nid) {
          $images[$cache_key][] = $unordered[$nid];
        }
      }
      else {
        $images[$cache_key] = array_values($unordered);
      }
      cache_set($cache_key, $images[$cache_key], 'cache', CACHE_TEMPORARY);
    }
  }
  return $images[$cache_key];
}

/**
 * Returns a specific slice of the node_gallery_get_images() array.  If both
 * $to_left and $to_right are not set or are 0, this returns only 1 element.
 *
 * @param $gallery
 *   Node object representing the gallery
 * @param $image_nid
 *   Nid of the current image
 * @param $to_left
 *   Integer representing the first position to return
 * @param $to_right
 *   Integer representing the last position to return
 */
function node_gallery_get_images_slice($gallery, $image_nid, $to_left = 0, $to_right = 0) {
  $images = node_gallery_get_images($gallery);
  $last_element = (count($images) - 1);
  $range = array();
  $current_element = node_gallery_get_image_position($gallery->nid, $image_nid) - 1;
  $offset = $current_element - $to_left < 0 ? 0 : $current_element - $to_left;
  $end = $current_element + $to_right > $last_element ? $last_element : $current_element + $to_right;
  $length = $end - $offset + 1;
  $range = array_slice($images, $offset, $length);
  return $range;
}

/**
 * Get image nids sorted by a custom field, such as title or filepath.
 *
 * @param object $gallery The node object of a gallery.
 * @param string $op The entry to sort after; one of title, created, changed, filepath, weight or current.
 * @param boolean $reverse If the order should be reversed; defaults to FALSE.
 * @return array Array of image nids.
 */
function node_gallery_get_image_nids_sorted_by(&$gallery, $op = '', $reverse = FALSE) {
  // get detailed image infos to be able to sort by arbitrary fields
  $images = node_gallery_get_images($gallery, TRUE, FALSE);

  $sort_function = NULL;
  switch ($op) {
    case 'title':
      $sort_function = create_function('$a,$b', 'return strnatcasecmp($a->title, $b->title);');
      break;

    case 'filename':
      $sort_function = create_function('$a,$b', 'return strnatcasecmp(basename($a->filepath), basename($b->filepath));');
      break;

    case 'created':
    case 'changed':
    case 'weight':
      $sort_function = create_function('$a,$b', 'return ($a->' . $op . ' <= $b->' . $op . ') ? -1 : 1;');
      break;

    case 'current':
    default:
  }
  if ($sort_function && uasort($images, $sort_function) === FALSE) {
    $image_nids = FALSE;
  }
  else {
    $image_nids = array();
    foreach ($images as $image) {
      $image_nids[] = $image->nid;
    }
    if ($reverse && ($op != 'current')) {
      $image_nids = array_reverse($image_nids);
    }
  }
  return $image_nids;
}

/**
 * Given a gallery, returns it's cover image.
 *
 * @param object $gallery
 *   A populated node object.
 *
 * @return
 *   The nid of the cover image.
 */
function node_gallery_get_cover_nid($gallery) {
  $cover_nid = db_result(db_query("SELECT cover_image FROM {node_gallery_galleries} WHERE gid = %d", $gallery->nid));
  return $cover_nid;
}

/**
 * Builds an array with the data necessary to navigate a gallery.
 *
 * @param $gid
 *   The nid of the gallery to navigate within.
 * @param $nid
 *   The nid of the current image.
 * @param $reset
 *   (optional) boolean when set to true, clears the caches.
 *
 * @return
 *   An associative array consisting of:
 *     - total: The count of published images in the gallery.
 *     - parent: The nid of the gallery.
 *     - current: The nid of the current image.
 *     - prev_nid: The nid of the previous image.
 *     - next_nid: The nid of the next image.
 *     - first_nid: The nid of the first image in the gallery.
 *     - last_nid: The nid of the last image in the gallery.
 */
function node_gallery_get_image_navigator($gid, $nid, $reset = FALSE) {
  $navigator['total'] = node_gallery_get_image_count($gid, $reset);
  $navigator['gallery_nid'] = $gid;
  $navigator['current'] = node_gallery_get_image_position($gid, $nid, $reset);
  $navigator['prev_nid'] = node_gallery_get_prev_image($gid, $nid, $reset);
  $navigator['next_nid'] = node_gallery_get_next_image($gid, $nid, $reset);
  $navigator['first_nid'] = node_gallery_get_first_image($gid, $reset);
  $navigator['last_nid'] = node_gallery_get_last_image($gid, $reset);
  $navigator['image_nids'] = node_gallery_get_image_list($gid, $reset);
  return $navigator;
}

/**
 * If the image is the cover image for a gallery, it sets ->is_cover = 1.
 *
 * @param $image
 *   A reference to the image node object.
 */
function node_gallery_set_image_is_cover_field(&$image) {
  $sql = "SELECT 1 FROM {node_gallery_galleries} where cover_image = %d";
  $image->is_cover = db_result(db_query($sql, $image->nid));
}

/**
 * Clears all the caches for a specific gallery.
 *
 * @param $gid
 *   The nid of the gallery to clear caches on.
 */
function node_gallery_clear_gallery_caches($gid) {
  watchdog('node_gallery', 'Clearing caches for gallery with node id !gid.', array('!gid' => $gid), WATCHDOG_INFO, l('view gallery', 'node/'. $gid));
  cache_clear_all('node_gallery:'. $gid, 'cache', TRUE);
}

/**
 * Builds the caches for a gallery and saves them to the db.
 *
 * There are two caches stored for each gallery.  One is a sorted array of nids
 * that represents the images in proper search order.
 * The second is an associative array where the key is the nid of the image
 * and the value is that nid's respective position in the list (1-based).
 *
 * @param $gid
 *   The nid of the gallery to build caches for.
 */
function _node_gallery_cache_sorted_image_nids($gid) {
  $image_list = node_gallery_get_image_nids($gid, TRUE, TRUE);
  $image_postition = array();
  $position = 1;
  foreach ($image_list as $nid) {
    $image_position[$nid] = $position;
    $position++;
  }
  cache_set('node_gallery:'. $gid .':image_position', $image_position, 'cache', CACHE_TEMPORARY);
  cache_set('node_gallery:'. $gid .':image_list', $image_list, 'cache', CACHE_TEMPORARY);
}

/**
 * Returns the position (starting at one) of the image in the gallery list.
 *
 * @param $gid
 *   The nid of the gallery to use.
 * @param $nid
 *   The nid of the image to return the position of.
 *
 * @return
 *   The position of the image in the list of published images in the gallery.
 */
function node_gallery_get_image_position($gid, $nid, $reset = FALSE) {
  static $image_position = array();

  if (!isset($image_position[$gid]) || $reset) {
    if (!$reset && ($cache = cache_get('node_gallery:'. $gid .':image_position')) && !empty($cache->data)) {
      $image_position[$gid] = $cache->data;
    }
    else {
      _node_gallery_cache_sorted_image_nids($gid);
      $cache =  cache_get('node_gallery:'. $gid .':image_position');
      $image_position[$gid] = $cache->data;
    }
  }
  return $image_position[$gid][$nid];
}

/**
 * Gets the next image in the gallery.
 *
 * @param $gid
 *   The nid of the gallery node.
 * @param $nid
 *   The nid of the image node.
 * @param $reset
 *   (optional)  If TRUE, the caches are cleared.
 *
 * @return
 *   The nid of the next image in the gallery.
 */
function node_gallery_get_next_image($gid, $nid, $reset = FALSE) {
  return node_gallery_seek_from_current_image($gid, $nid, 1, $reset);
}

/**
 * Gets the previous image in the gallery.
 *
 * @param $gid
 *   The nid of the gallery node.
 * @param $nid
 *   The nid of the image node.
 * @param $reset
 *   (optional)  If TRUE, the caches are cleared.
 *
 * @return
 *   The nid of the previous image in the gallery.
 */
function node_gallery_get_prev_image($gid, $nid, $reset = FALSE) {
  return node_gallery_seek_from_current_image($gid, $nid, -1, $reset);
}

/**
 * Gets a sorted list of images nids within a gallery.
 * @param $gid
 *   The nid of the gallery to query.
 * @param $reset
 *   (optional) If TRUE, clears the cache, defaults to FALSE.
 *
 * @return
 *   An array of sorted image nids with a status of published.
 */
function node_gallery_get_image_list($gid, $reset = FALSE) {
  static $image_list = array();

  if (!isset($image_list[$gid]) || $reset) {
    if (!$reset && ($cache = cache_get('node_gallery:'. $gid .':image_list'))) {
      $image_list[$gid] = $cache->data;
    }
    else {
      _node_gallery_cache_sorted_image_nids($gid);
      $cache = cache_get('node_gallery:'. $gid .':image_list');
      $image_list[$gid] = $cache->data;
    }
  }
  return $image_list[$gid];
}

/**
 * Gets the count of published images within a gallery.
 * @param $gid
 *   The nid of the gallery to query.
 * @param $reset
 *   (optional) If TRUE, clears the cache, defaults to FALSE.
 *
 * @return
 *   A count of all images in the gallery.
 */
function node_gallery_get_image_count($gid, $reset = FALSE) {
  $image_list = node_gallery_get_image_list($gid, $reset);
  return count($image_list);
}

/**
 * Gets the first image nid within a gallery.
 * @param $gid
 *   The nid of the gallery to query.
 * @param $reset
 *   (optional) If TRUE, clears the cache, defaults to FALSE.
 *
 * @return
 *   The nid of the first image in the gallery.
 */
function node_gallery_get_first_image($gid, $reset = FALSE) {
  $image_list = node_gallery_get_image_list($gid, $reset);
  return $image_list[0];
}

/**
 * Gets the last image nid within a gallery.
 * @param $gid
 *   The nid of the gallery to query.
 * @param $reset
 *   (optional) If TRUE, clears the cache, defaults to FALSE.
 *
 * @return
 *   The nid of the last image in the gallery.
 */
function node_gallery_get_last_image($gid, $reset = FALSE) {
  $image_list = node_gallery_get_image_list($gid, $reset);
  return $image_list[count($image_list) - 1];
}

/**
 * Returns the nid of the image +/-N steps away from the current image node.
 * @param $gid
 *   The nid of the gallery to query.
 * @param $nid
 *   The nid of the current image.
 * @param signedint $seek
 *   The postive or negative number of slots to seek to.
 * @param $reset
 *   (optional) If TRUE, clears the caches.  Defaults to FALSE.
 */
function node_gallery_seek_from_current_image($gid, $nid, $seek, $reset) {
  $image_list = node_gallery_get_image_list($gid, $reset);
  $current_position = node_gallery_get_image_position($gid, $nid, $reset);
  $current_index = $current_position - 1;
  $seek_index = $current_index + $seek;

  return isset($image_list[$seek_index]) ? $image_list[$seek_index] : NULL;
}

/**
 * Sets the weight on an image node.
 *
 * When the Action API is used to set the weight on an image node, it does not
 * set the weight on the node directly, it sets it on the new_weight attribute.
 * This allows us to determine if the weight actually changed or not, and
 * clear the caches only in cases where the weight has changed.
 *
 * @param $image
 *   A reference to a populated image node object.
 */
function node_gallery_set_image_weight(&$image) {
  $sort_order_changed = FALSE;
  if (isset($image->new_weight)) {
    if (! isset($image->weight) || $image->new_weight != $image->weight) {
      $image->weight = $image->new_weight;
      node_gallery_clear_gallery_caches($image->gid);
    }
  }
}

/**
 * Populates a node object with the node gallery image attributes.
 *
 * @param $node
 *   Reference to a populated node object.
 */
function node_gallery_load_image(&$node) {
  $fields = array('gid', 'weight');
  $image = db_fetch_object(db_query("SELECT ". implode(", ", $fields) ." FROM {node_gallery_images} WHERE nid = %d", $node->nid));
  foreach ($fields as $attribute) {
    $node->$attribute = $image->$attribute;
  }
  node_gallery_set_image_is_cover_field($node);
}

/**
 * Sets the cover image in the DB if necessary.
 *
 * @param $image
 *   A reference to the node object of a node gallery image.
 */
function node_gallery_set_gallery_cover_image($image) {
  if ($image->gid > 0) {
    $gallery = new stdClass;
    $gallery->gid = $image->gid;
    if ($image->is_cover) {
      if (!empty($image->oldgid)) {
        // this image is the cover image for another gallery, it's being moved. Retrieve a new cover for the old gallery
        $new_cover = db_result(db_query('SELECT nid FROM {node_gallery_images} WHERE gid = %d AND nid <> %d LIMIT 1', $image->oldgid, $image->nid));
        if ($new_cover !== FALSE) {
          db_query("UPDATE {node_gallery_galleries} SET cover_image = %d WHERE gid = %d", $new_cover, $image->oldgid);
        } else {
          // Gallery is now empty, set cover to NULL.
          db_query("UPDATE {node_gallery_galleries} SET cover_image = NULL WHERE gid = %d", $image->oldgid);
        }
      }
      else {
        $gallery->cover_image = $image->nid;
      }
    }
    // check if the gallery already has a cover
    $sql = "SELECT count(*) from {node_gallery_galleries} where gid = %d and cover_image IS NULL";
    $count = db_result(db_query($sql, $image->gid));
    if ($count > 0) {
      // This is the first image being added to the gallery.  Make it the cover.
      $gallery->cover_image = $image->nid;
    }
    if (isset($gallery->cover_image)) {
      drupal_write_record('node_gallery_galleries', $gallery, 'gid');
    }
  }
}

/**
 * Recursively removes empty directories up to the files directory.
 *
 * @param $dirname
 *   A path to start pruning from.
 */
function node_gallery_clean_empty_dirs($dirname) {
  if (empty($dirname)) {
    return;
  }
  // Recursively delete parent directories that are empty, up to the 'files' directory.
  $files = file_scan_directory($dirname, '.*', array('.', '..'), 0, FALSE);
  if (count($files) > 0 || $dirname == file_directory_path()) {
    return;
  }
  else {
    if (is_dir($dirname)) {
      rmdir($dirname);
    }
    $dirname = dirname($dirname);
    node_gallery_clean_empty_dirs($dirname);
  }
}

/**
 * Returns the filepath(s) to an imagefield item - saving a full node_load().
 *
 * @param $nids
 *   The nid(s) of the node.
 * @param $fieldname
 *   The name of the imagefield.
 *
 * @return
 *   If a single nid is passed in, a single filepath string is returned.  If an array
 *   of nids are passed in, it returns an array of filepaths.
 */
function node_gallery_get_image_filepath($nids, $fieldname) {
  static $fields = array();

  if (! isset($fields[$fieldname])) {
    $fields[$fieldname]['field'] = content_fields($fieldname);
    $fields[$fieldname]['db_info'] = content_database_info($fields[$fieldname]['field']);
  }
  $table = $fields[$fieldname]['db_info']['table'];
  $sql = 'SELECT nid, filepath FROM {'. $table .'} JOIN {files} on {'. $table .'}.'. $fieldname .'_fid = {files}.fid ';
  if (!is_array($nids)) {
    $sql .= 'WHERE nid = %d';
    $file = db_fetch_array(db_query($sql, $nids));
    $filepath = $file['filepath'];
  }
  else {
    $sql .= 'WHERE nid IN ('. db_placeholders($nids) .')';
    $result = db_query($sql, $nids);
    while ($file = db_fetch_array($result)) {
      $filepaths[$file['nid']] = $file['filepath'];
    }
    foreach ($nids as $nid) {
      $filepath[] = $filepaths[$nid];
    }
  }
  return $filepath;
}

/**
 * Deletes an image from the database, cleans the filesystem of imagecache files.
 * @param $node
 *   A populated node object that is a node gallery image.
 */
function node_gallery_delete_image($node) {
  // Clean up our tables
  db_query("DELETE FROM {node_gallery_images} WHERE nid = %d", $node->nid);
  $gid = db_result(db_query("SELECT gid FROM {node_gallery_galleries} WHERE cover_image = %d", $node->nid));
  if ($gid !== FALSE) {
    // the image was the cover of an gallery
    $new_cover = db_result(db_query('SELECT nid FROM {node_gallery_images} WHERE gid = %d AND nid <> %d LIMIT 1', $gid, $node->nid));
    if ($new_cover !== FALSE) {
      db_query("UPDATE {node_gallery_galleries} SET cover_image = %d WHERE gid = %d", $new_cover, $gid);
    } else {
      // Gallery is now empty, set cover to NULL.
      db_query("UPDATE {node_gallery_galleries} SET cover_image = NULL WHERE gid = %d", $gid);
    }
  }
  // We don't delete the file from the filesystem anymore, because imagefield handles this for us
  // Clean our imagecache - note, this API call leaves empty dirs
  $relationship = node_gallery_get_relationship(NULL, $node->type);
  imagecache_image_flush($node->{$relationship['imagefield_name']}[0]['filepath']);
  // Clean up empty dirs, if they exist
  node_gallery_clean_empty_dirs(dirname($node->field_node_gallery_image[0]['filepath']));
}

/**
 * Given a content type, creates a new gallery node, returning the node object.
 * @param $content_type
 *   The content type of the new node.
 *
 * @return
 *   The completed node object.
 */
function _node_gallery_create_new_gallery($content_type) {
  global $user;
  module_load_include('inc', 'node', 'node.pages');
  $node = new stdClass;
  $node->type = $content_type;
  node_object_prepare($node);
  $node->title = 'Unnamed Gallery';
  $node->name = $user->name;
  node_save($node);
  return $node;
}

/**
 * Deletes orphaned rows from the node_gallery tables.  We shouldn't need this,
 * but sometimes dev code is less than perfect.
 */
function node_gallery_delete_orphans() {
  $tables = array('node_gallery_galleries' => 'gid', 'node_gallery_images' => 'nid');
  $total = 0;
  foreach ($tables as $table => $pk) {
    $basesql  = 'FROM {'. $table .'} as ng LEFT JOIN {node} ON {node}.nid = ng.'. $pk;
    $basesql .= ' WHERE {node}.nid IS NULL';
    $sql = 'SELECT count(*) '. $basesql;
    $count = db_result(db_query($sql));
    $total += $count;
    if ($count > 0) {
      $sql = 'DELETE ng '. $basesql;
      db_query($sql);
      $message = t('Found and removed !count orphaned rows from table !table.', array('!count' => $count, '!table' => $table));
      drupal_set_message($message);
    }
  }
  if ($total == 0) {
    drupal_set_message(t('No orphaned rows were found in the database.'));
  }
}

/**
 * Deletes a gallery from the database.
 * @param $node
 *   A populated node object that is a node gallery gallery.
 */
function node_gallery_delete_gallery($node) {
  // Clean up our tables
  db_query("DELETE FROM {node_gallery_galleries} WHERE gid = %d", $node->nid);
}

/**
 * Given an image type, return the name of the widget used on it's imagefield
 *
 * @param $image_type
 *   A string that is the name of the image content type
 */
function node_gallery_get_current_widget_type($image_type) {
  module_load_include('inc', 'content', 'includes/content.crud');
  $rel = node_gallery_get_relationship(NULL, $image_type);
  $param = array(
    'type_name' => $image_type,
    'field_name' => $rel['imagefield_name'],
  );
  $image_field = array_pop(content_field_instance_read($param));
  return $image_field['widget']['type'];
}

/**
 * Given a settings associative array uses content_crud to apply
 * settings to the widget for all image types
 *
 * @param $default_settings
 *   An associative array of settings to apply
 */
function node_gallery_apply_cck_settings_to_image_types($settings) {
  module_load_include('inc', 'content', 'includes/content.crud');
  $image_types = (array)node_gallery_get_types('image');
  foreach ($image_types as $image_type) {
    $rel = node_gallery_get_relationship(NULL, $image_type);
    $param = array(
      'type_name' => $image_type,
      'field_name' => $rel['imagefield_name'],
    );
    $image_field = array_pop(content_field_instance_read($param));
    $image_field = array_merge($image_field, $settings);
    content_field_instance_update($image_field, FALSE);
  }
  // we clear the caches here once instead of with content_field_instance_update()
  // in case there are several fields.
  content_clear_type_cache(TRUE);
  menu_rebuild();
}

/**
 * Returns true or false if a particular field exists on a particular content type.
 */

function node_gallery_cck_field_exists($param) {
  module_load_include('inc', 'content', 'includes/content.crud');
  // content_field_instance_read croaks if you send it more than it expects
  if (count($param) > 2) {
    $param = array('type_name' => $param['type_name'], 'field_name' => $param['field_name']);
  }
  $field_instance = content_field_instance_read($param);
  if (empty($field_instance)) {
    return FALSE;
  }
  return TRUE;
}

function node_gallery_batch_sorting_callback($images, &$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox']['iterator'] = 0;
    $context['sandbox']['count'] = count($images);
    $context['results']['gid'] = $images[0]['gid'];
  }
  $context['results'][] = drupal_write_record('node_gallery_images', $images[$context['sandbox']['iterator']], 'nid');
  $context['sandbox']['iterator']++;
  if ($context['sandbox']['iterator'] < $context['sandbox']['count']) {
    $context['finished'] = $context['sandbox']['iterator'] / (float)$context['sandbox']['count'];
  }
  else {
    $context['finished'] = 1;
  }
}

function node_gallery_batch_sorting_finished($success, $results, $operation) {
  if ($success) {
    // $results contains ['gid'] as well as the actual results, so we subtract one here.
    $message = format_plural(count($results) - 1, 'One image processed.', '@count images processed.');
  }
  else {
    $message = t('Finished with an error.');
  }
  node_gallery_clear_gallery_caches($results['gid']);
  drupal_set_message($message);
}

function node_gallery_batch_rotate_callback($filepath, $degrees, &$context) {
  $result = 0;
  $image = imageapi_image_open($filepath);
  if ($image !== FALSE) {
    if (imageapi_default_toolkit() == 'imageapi_imagemagick') {
      $result = imageapi_image_rotate($image, $degrees, 0x000000);
    }
    else {
      $result = imageapi_image_rotate($image, $degrees);
    }
    global $conf;
    $conf['imageapi_jpeg_quality'] = 100;
    imageapi_image_close($image);
    imagecache_image_flush($filepath);
    // add a _r$i to the filename for cache distinction of rotated images
    $filepath_new = preg_replace_callback(
        '~(?:_r([0-9]+))?(?=(?:\.[^./]*)?$)~',
        create_function('$matches', 'return "_r".(!empty($matches[1]) ? $matches[1]+1 : 1);'),
        $filepath, 1);
    $file = (object)field_file_load($filepath);
    $file->timestamp = time();
    $file->filepath = $filepath_new;
    $file->filename = basename($filepath_new);
    $filepath_old = $filepath;
    file_move($filepath, $filepath_new);
    drupal_write_record('files', $file, 'fid');
  }
  else {
    watchdog('node_gallery', 'Could not open image for rotation');
  }
  $context['results'][] = $result;
}

/**
 * Compare taxonomy on an unsaved node object versus a node_load()ed object
 * @param $old
 *   The old node_load()ed object.
 * @param $new
 *   The new node object.
 *
 * @return
 *   TRUE if the taxo differs, FALSE if not
 */
function node_gallery_taxonomy_compare($old, $new) {
  $new->taxonomy = taxonomy_preview_terms($new);
  $new_taxonomy = $new->taxonomy;
  $old_taxonomy = $old->taxonomy;

  if (isset($new_taxonomy['tags'])) {
    // in case of free tagging, the preview function does not return term objects
    $existing_tags = array();
    $term2tid = array();
    foreach ($old_taxonomy as $term) {
      $existing_tags[$term->vid][] = $term->name;
      $term2tid[$term->vid][$term->name] = $term->tid;
    }
    foreach ($new_taxonomy['tags'] as $vid => $tag_string) {
      $tags = drupal_explode_tags($tag_string);
      if (count($tags) != count($existing_tags[$vid])) {
        return TRUE;
      }
      foreach ($tags as $tag) {
        if (!in_array($tag, $existing_tags[$vid])) {
          return TRUE;
        }
        else {
          unset($old_taxonomy[$term2tid[$vid][$tag]]);
        }
      }
    }
    unset($new_taxonomy['tags']);
  }
  if ($new_taxonomy != $old_taxonomy) {
    // difference in non-tag vocab
    return TRUE;
  }
  return FALSE;
}
