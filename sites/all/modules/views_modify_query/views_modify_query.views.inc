<?php
// $Id: views_modify_query.views.inc,v 1.3 2008/09/15 16:28:56 anselmheaton Exp $

function views_modify_query_views_data_alter(&$cache) {
  $cache['node']['modify_query'] = array(
    'group' => t('Views'),
    'title' => t('Views Modify Query'),
    'help' => t('Run PHP code in the context of a Views Query'),
    'filter' => array(
      'handler' => 'views_modify_query_filter_handler',
    ),
  );
  
  $cache['node']['modify_query_field'] = array(
    'group' => t('Views'),
    'title' => t('Views Modify Query'),
    'help' => t('Display result of code executed as filter by Views Modify Query'),
    'field' => array(
      'handler' =>'views_modify_query_field_handler',
      'click sortable' => false,
    ),
  );
}

/**
 * Implementation of hook_views_handlers
 */
function views_modify_query_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'views_modify_query'),
    ),
    'handlers' => array(
      'views_modify_query_filter_handler' => array(
        'parent' => 'views_handler_filter',
      ),
      'views_modify_query_field_handler' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}
