<?php
// $Id: scheduler.views.inc,v 1.1.2.3 2009/08/26 17:40:44 ericschaefer Exp $

/**
 * @file
 * Views integration file for Scheduler module.
 */

function scheduler_views_data() {
  $tables['scheduler']['table']['group'] = t('Scheduler');

  // how is the scheduler table linked to the nodes
  $tables['scheduler']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );

  // description of the fields (table columns)
  $tables['scheduler']['publish_on'] = array(
    'title' => t('Publish on'),
    'help' => t('Date/time on which the article will be automatically published'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
      'label' => t('Publish on'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  $tables['scheduler']['unpublish_on'] = array(
    'title' => t('Unpublish on'),
    'help' => t('Date/time on which the article will be automatically unpublished'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
      'label' => t('Unpublish on'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  return $tables;
}