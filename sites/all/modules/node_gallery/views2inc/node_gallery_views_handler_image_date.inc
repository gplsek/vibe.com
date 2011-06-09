<?php

/**
 * @file
 * Creates an image date handler for galleries.
 */

class node_gallery_views_handler_image_date extends views_handler_field_date {
  function query() {
    $table = $this->ensure_my_table();
    $sql = "SELECT n.changed FROM {node_gallery_images} ng JOIN {node} n on ng.nid = n.nid WHERE n.status = 1 AND ng.gid = node.nid ORDER BY changed DESC LIMIT 1";
    $this->field_alias = $this->query->add_field(NULL, "($sql)", 'image_date');
  }
}