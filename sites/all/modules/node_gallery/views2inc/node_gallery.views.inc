<?php
// @todo Theming for views

/**
 * @file
 * Views integration for Node Gallery
 */

function node_gallery_views_data() {
  $data['node_gallery_images']['table']['group'] = t("Node Gallery: Image");
  $data['node_gallery_images']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );
  $data['node_gallery_images']['gid'] = array(
    'title' => t("Parent Gallery"),
    'help' => t("The Id of the image's gallery."),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'relationship' => array(
      'label' => t('Parent Gallery'),
      'base' => 'node',
      'base field' => 'nid',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['node_gallery_images']['weight'] = array(
    'title' => t('Image weight'),
    'help' => t('The weight assigned to the image. Used for sorting within a gallery.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['node_gallery_images']['navigator'] = array(
    'title' => t('Image Navigator'),
    'help' => t('A themed navigator for browsing previous and next images.'),
    'field' => array(
      'handler' => 'node_gallery_views_handler_image_navigator',
      'click sortable' => FALSE,
      'notafield' => TRUE,
    ),
  );

  $data['node_gallery_galleries']['table']['group'] = t('Node Gallery: Gallery');
  $data['node_gallery_galleries']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'gid',
    ),
  );
  $node_schema = drupal_get_schema('node');
  $fields = array_keys($node_schema['fields']);
  $pos = array_search('nid', $fields);
  if ($pos !== FALSE) {
    array_splice($fields, $pos, 1);
  }
  $node_fields = 'node.'. implode(', node.', $fields);
  
  $data['node_gallery_galleries']['newest_image'] = array(
    'title' => t('Gallery Newest Image'),
    'help' => t('Relationship to link the newest image for the gallery.'),
    'relationship' => array(
      'label' => t('Gallery Newest Image'),
      'base' => 'node',
      'table formula' => '(SELECT gid, '. $node_fields .', MAX(ngi.nid) as nid FROM {node_gallery_images} ngi JOIN {node} node ON ngi.nid = node.nid GROUP BY gid)',
      'base field' => 'gid',
      'relationship field' => 'gid',
    ),
  );
  $data['node_gallery_galleries']['oldest_image'] = array(
    'title' => t('Gallery Oldest Image'),
    'help' => t('Relationship to link the oldest image for the gallery.'),
    'relationship' => array(
      'label' => t('Gallery Oldest Image'),
      'base' => 'node',
      'table formula' => '(SELECT gid, '. $node_fields .', MIN(ngi.nid) AS nid, node.* FROM {node_gallery_images} ngi JOIN {node} node ON ngi.nid = node.nid GROUP BY gid)',
      'base field' => 'gid',
      'relationship field' => 'gid',
    ),
  );
  $data['node_gallery_galleries']['cover_image'] = array(
    'title' => t('Gallery Cover Image'),
    'help' => t('Relationship to link the cover image for the gallery.'),
    'relationship' => array(
      'label' => t('Gallery Cover Image'),
      'base' => 'node',
      'base field' => 'nid',
    ),
  );
  $data['node_gallery_galleries']['image_count'] = array(
    'title' => t('Image Count'),
    'help' => t('The number of images in the gallery.'),
    'field' => array(
      'handler' => 'node_gallery_views_handler_image_count',
      'click sortable' => TRUE,
      'notafield' => TRUE,
    ),
  );
  $data['node_gallery_galleries']['comments_count'] = array(
    'title' => t('Total comments'),
    'help' => t('The total number of comments on the gallery and all its images.'),
    'field' => array(
      'handler' => 'node_gallery_views_handler_comments_count',
      'click sortable' => TRUE,
      'notafield' => TRUE,
    ),
  );
  $data['node_gallery_galleries']['image_date'] = array(
    'title' => t('Image Updated Date'),
    'help' => t('The last changed date of an image in the gallery.'),
    'field' => array(
      'handler' => 'node_gallery_views_handler_image_date',
      'click sortable' => TRUE,
      'notafield' => TRUE,
    ),
  );
  $data['node_gallery_galleries']['gid'] = array(
    'title' => t("Gallery Id"),
    'help' => t("The Id of the gallery."),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
  return $data;
}

function node_gallery_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'node_gallery') .'/views2inc',
    ),
    'handlers' => array(
      'node_gallery_views_handler_image_count' => array(
        'parent' => 'views_handler_field_numeric',
      ),
      'node_gallery_views_handler_comments_count' => array(
        'parent' => 'views_handler_field_numeric',
      ),
      'node_gallery_views_handler_image_date' => array(
        'parent' => 'views_handler_field_date',
      ),
      'node_gallery_views_handler_image_navigator' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}