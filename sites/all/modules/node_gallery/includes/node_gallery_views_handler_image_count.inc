<?php
// $Id: node_gallery_views_handler_image_count.inc,v 1.1.2.3 2010/05/08 14:02:30 justintime Exp $

/**
 * @file node_gallery_handler_image_count.inc
 *
 * 
 */
 
class node_gallery_views_handler_image_count extends views_handler_field_numeric {
  function query() {
    $table = $this->ensure_my_table();
    $sql = "SELECT COUNT(*) FROM {node_galleries} ng JOIN {node} n on ng.nid = n.nid WHERE n.status = 1 AND ng.gid = %d";
    $sql = str_replace('%d', $this->table .'.gid', $sql);
    $this->query->add_field('', "($sql)", 'image_count');
    $this->field_alias = 'image_count';
  }
}