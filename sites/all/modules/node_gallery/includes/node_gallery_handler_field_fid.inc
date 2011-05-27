<?php
// $Id: node_gallery_handler_field_fid.inc,v 1.1.2.8 2010/05/08 14:02:30 justintime Exp $

/**
 * @file node_gallery_handler_field_fid.inc
 *
 * 
 */
 
class node_gallery_handler_field_fid extends views_handler_field {
  
  function construct() {
    parent::construct();
    //$this->additional_fields['gid'] = 'gid';
    $this->additional_fields['nid'] = 'nid';
  }
  
  function option_definition() {
    $options = parent::option_definition();
    $options['node_gallery_setting'] = '';
    $options['view_mode'] = NODE_GALLERY_VIEW_TEASER;
    return $options;
  }
  
  function options_form(&$form, &$form_state) {
    $configs = node_gallery_get_config();
    foreach ($configs as $gallery_type => $config) {
      $options[$gallery_type] = $config['name'];
    }
    
    parent::options_form($form, $form_state);
    $form['node_gallery_setting'] = array(
      '#title' => t('Node Gallery Setting'),
      '#description' => t('Specify which node gallery settings you want to use.'),
      '#type' => 'select',
      '#default_value' => $this->options['node_gallery_setting'],
      '#options' => $options,
    );
    $form['view_mode'] = array(
      '#title' => t('Image List View Mode'),
      '#description' => t('Specify which view mode you want to use to display image list.'),
      '#type' => 'select',
      '#default_value' => $this->options['view_mode'],
      '#options' => drupal_map_assoc(array(NODE_GALLERY_VIEW_TEASER, NODE_GALLERY_VIEW_IMAGE_LIST)),
    );
  }
  
  function pre_render(&$values) {
    global $user;
    
    foreach ($values as $value) {
      $fids[$value->{$this->aliases[$this->additional_fields['nid']]}] = $value->{$this->field_alias};
    }
    if (!empty($fids)) {
      $result = db_query(db_rewrite_sql("SELECT n.nid, ng.*, f.filepath FROM {node} n INNER JOIN {node_galleries} ng
       ON n.nid = ng.nid INNER JOIN {files} f ON ng.fid = f.fid WHERE ng.fid IN (". db_placeholders($fids) .")"), $fids);
      while ($r = db_fetch_array($result)) {
        $items[$r['nid']] = $r;
      }
     // If nga is active, then fetch the access type and password from the db and merge it for use later in render()
      if (module_exists('node_gallery_access')) {
        $nids = array_keys($items);
        $result = db_query("SELECT n.uid, ng.nid, nga.access_type, nga.password FROM {node} n INNER JOIN
         {node_galleries} ng ON n.nid = ng.nid INNER JOIN {node_gallery_access} nga 
        ON ng.gid = nga.nid WHERE ng.nid IN (". db_placeholders($nids) .")", $nids);
        while ($r = db_fetch_array($result)) {
          $items[$r['nid']] = array_merge((array)$items[$r['nid']], $r);
        }
      }
      foreach ($values as $value) {
        $new = array_merge((array)$value, $items[$value->{$this->aliases[$this->additional_fields['nid']]}]);
        $new_items[] = (object)$new;
      }
      $values = $new_items;
    }
  }
  
  function render($value) {
    global $user;
    $config = node_gallery_get_config($this->options['node_gallery_setting']);
    // if nga is there, and our gallery is password protected, show our protected image
    if (module_exists('node_gallery_access')) {
      if ($value->access_type == NODE_GALLERY_ACCESS_TYPE_PASSWORD) {
        if (!node_gallery_access_check_access($value, 'image')) {
          $value->filepath = node_gallery_access_default_pass_image();
        }
      }
    }
    return theme('gallery_image_thumbnail', $value, $config, $this->options['view_mode']);
  }
}