<?php

/**
 * @file
 * Creates an image count handler for galleries.
 */

class node_gallery_views_handler_image_count extends views_handler_field_numeric {
  function query() {
    $table = $this->ensure_my_table();
    $sql = "SELECT COUNT(*) FROM {node_gallery_images} ng JOIN {node} n on ng.nid = n.nid WHERE n.status = 1 AND ng.gid = node.nid";
    $this->field_alias = $this->query->add_field(NULL, "($sql)", 'image_count');
  }
}