<?php
// $Id: activity_views_handler_filter_access.inc,v 1.1.2.7 2009/08/26 19:10:28 scottreynolds Exp $

/**
 * @file
 * generic handler that provides access control on where
 */
class activity_views_handler_filter_access extends views_handler_filter {
  
  /**
   * tell Views UI this filter can't be exposed
   */
  function can_expose() {
    return FALSE;
  }
  
  /**
   * Override the options_definition().
   */
  function option_definition() {
    $options = parent::option_definition();
    
    $info = activity_get_module_info($this->definition['module']);
    $realms = $info->realms;
    foreach ($realms as $realm => $name) {
      $default_realms[$realm] = $realm;
    }

    $options['realms'] = array('default' => $default_realms);
    return $options;
  }
  
  /**
   * Override the options_form().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $info = activity_get_module_info($this->definition['module']);
    $realms = $info->realms;
    $options = array();
    foreach ($realms as $realm => $title) {
      $options[$realm] = t($title);
    }
    $form['realms'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Specific Relationships'),
      '#options' => $options,
      '#default_value' => $this->options['realms'],
    );
  }
  
  /**
   * override query
   */
  function query() {
    $this->ensure_my_table();
    $group = activity_views_group();
    $placeholder = '%d';
    
    // ensure the file is included for this module
    activity_get_module_info($this->definition['module']);
    
    // go through each grant that this user has
    foreach(module_invoke($this->definition['module'], 'activity_access_grants', activity_views_access_user()) as $realm => $values) {
      if (!empty($this->options['realms'][$realm])) {
        if (is_null($group)) {
          // static cache the where group for the rest of the handlers
          $group = activity_views_group($this->query->set_where_group('OR'));
        }
        
        $replace = array_fill(0, sizeof($values), $placeholder);
        $in = ' (' . implode(", ", $replace) . ')';
        // use the group to allow this $module to provide access control on it
        $this->query->add_where($group, "($this->table_alias.realm = '%s' AND $this->table_alias.$this->real_field IN $in)", array_merge(array($realm), $values));
      }
    }
  }
}