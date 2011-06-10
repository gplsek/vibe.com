<?php
// $Id$

/**
 * @file node_gallery_views_handler_gallery_op.inc
 *
 * 
 */
 
class node_gallery_views_handler_gallery_op extends views_handler_field {
  
  /*function construct() {
    parent::construct();
    $this->additional_fields['gid'] = 'gid';
  }*/
  
  function query() {
    $this->ensure_my_table();
    //$this->add_additional_fields();
  }
  
  function render($value) {
    $gallery = node_load($value->{$this->real_field});
    return node_gallery_operations('cover', $gallery);
  }
}