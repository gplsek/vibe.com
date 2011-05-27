<?php
// $Id: node_gallery_handler_field_gid.inc,v 1.1.2.9 2010/06/04 20:51:56 justintime Exp $

/**
* @file node_gallery_handler_field_gid.inc
*
*
*/

class node_gallery_handler_field_gid extends views_handler_field {
 
  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_node'] = array('default' => TRUE);
    $options['node_gallery_setting'] = '';
    return $options;
  }
 
  function options_form(&$form, &$form_state) {
    $configs = node_gallery_get_config();
    foreach ($configs as $gallery_type => $config) {
      $options[$gallery_type] = $config['name'];
    }
    
    parent::options_form($form, $form_state);
    $form['link_to_node'] = array(
      '#title' => t('Link this field to its node'),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['link_to_node'],
    );
    $form['node_gallery_setting'] = array(
      '#title' => t('Specify Node Gallery Setting'),
      '#description' => t('Specify which node gallery settings you want to use.'),
      '#type' => 'select',
      '#default_value' => $this->options['node_gallery_setting'],
      '#options' => $options,
    );
  }
 
  function pre_render(&$values) {
    foreach ($values as $value) {
      //$fids[$this->field_alias] = $value->{$value->{$this->aliases[$this->additional_fields['fid']]}};
      $gids[] = $value->{$this->field_alias};
    }
    if (!empty($gids)) {
      $result = db_query(db_rewrite_sql("SELECT n.nid, ng.*, f.filepath, n.title FROM {node} n INNER JOIN {node_galleries} ng ON
        n.nid = ng.gid INNER JOIN {files} f ON ng.fid = f.fid
        WHERE ng.gid IN (". db_placeholders($gids) .") AND is_cover > 0"), $gids);
      while ($r = db_fetch_array($result)) {
        $items[$r['gid']] = $r;
      }
      // If nga is active, then fetch the access type and password from the db and merge it for use later in render()
      if (module_exists('node_gallery_access')) {
        $nids = array_keys($items);
        $result = db_query("SELECT n.uid, n.nid, nga.access_type, nga.password FROM {node} n INNER JOIN {node_gallery_access} nga ON n.nid = nga.nid WHERE n.nid IN (". db_placeholders($nids) .")", $nids);
        while ($r = db_fetch_array($result)) {
          $items[$r['nid']] = array_merge((array)$items[$r['nid']], $r);
        }
      }
      foreach ($values as $value) {
        $new = array_merge((array)$value, $items[$value->{$this->field_alias}]);
        $new_items[] = (object)$new;
      }
      $values = $new_items;
    }
  }
 
  function render($value) {
    $config = node_gallery_get_config($this->options['node_gallery_setting']);
    // if nga is there, and our gallery is password protected, show our protected image
    if (module_exists('node_gallery_access')) {
      if ($value->access_type == NODE_GALLERY_ACCESS_TYPE_PASSWORD) {
        if (!node_gallery_access_check_access($value, 'gallery')) {
          $value->filepath = node_gallery_access_default_pass_image();
        }
      }
    }
    $image = theme('image_view', $config['image_size']['thumbnail'], $value);
    if ($this->options['link_to_node']) {
      return l($image, 'node/'. $value->{$this->field_alias}, array('html' => TRUE));
    }
    return $image;
  }
}