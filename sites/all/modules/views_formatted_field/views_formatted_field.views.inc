<?php
// $Id: views_formatted_field.views.inc,v 1.1 2009/06/22 15:17:23 overall Exp $

/**
 * Implementation of hook_views_handlers().
 */
function views_formatted_field_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'views_formatted_field').'/handlers/',
    ),
    'handlers' => array(
      'views_handler_field_formatted' => array(
        'parent' => 'views_handler_field',
      ),
      'views_handler_field_formatted_virtual' => array(
        'parent' => 'views_handler_field_formatted',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data()
 */
function views_formatted_field_views_data() {
  $data['views_formatted_field']['table']['group'] = t('Global');
  $data['views_formatted_field']['table']['join'] = array(
    '#global' => array(),
  );

  $data['views_formatted_field']['formatted'] = array(
    'title' => t('Virtual Formatted Field'),
    'help' => t('Provide formatted code evaluated to HTML with ability to access other fields by their names.'),
    'field' => array(
      'handler' => 'views_handler_field_formatted_virtual',
    ),
  );

  return $data;
}
