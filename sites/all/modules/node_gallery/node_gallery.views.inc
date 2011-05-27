<?php
// $Id: node_gallery.views.inc,v 1.1.2.6 2010/05/08 14:02:30 justintime Exp $

/**
 * @file node_gallery.views.inc
 *
 * All views plugins for Node Gallery
 */

function node_gallery_views_data() {
  $data['node_galleries']['table']['group'] = t("Node Gallery Image");
  $data['node_galleries']['table']['join'] = array(
    'node' => array(
      'type' => 'INNER',
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );
  /*$data['node_galleries']['nid'] = array(
    'title' => t('Image'),
    'help' => t('The node gallery images.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
  );*/
  $data['node_galleries']['gid'] = array(
    'title' => t("Galley Id"),
    'help' => t("The gallery's node id."),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'parent' => 'views_handler_argument_numeric', // make sure parent is included
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['node_galleries']['fid'] = array(
    'title' => t("Image"),
    'help' => t("The gallery's image."),
    'field' => array(
      'handler' => 'node_gallery_handler_field_fid',
    ),
    'relationship' => array(
      'title' => t('Image File'),
      'help' => t('Attach image file.'),
      'base' => 'files',
      'field' => 'fid',
      'handler' => 'views_handler_relationship',
      'label' => t('Node Gallery: Image'),
    ),
  );
  $data['node_galleries']['weight'] = array(
    'title' => t('Image weight'),
    'help' => t('The gallery image weight. Used for sorting ascending or descending.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['node_galleries']['is_cover'] = array(
    'title' => t('Cover Image'),
    'help' => t('The gallery cover image.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
  );
  
  $data['node_galleries_gallery']['table']['group'] = t('Node Gallery');
  $data['node_galleries_gallery']['table']['join'] = array(
    'node' => array(
      'type' => 'INNER',
      'table' => 'node_galleries',
      'left_field' => 'nid',
      'field' => 'gid',
    ),
  );
  $data['node_galleries_gallery']['gid'] = array(
    'title' => t('Gallery ID'),
    'help' => t('The GID of the gallery'),
    'field' => array(
      'handler' => 'node_gallery_handler_field_gid',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'parent' => 'views_handler_argument_numeric', // make sure parent is included
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['node_galleries_gallery']['image_count'] = array(
    'title' => t('Image Count'),
    'help' => t('The number of images in the gallery.'),
    'field' => array(
      'handler' => 'node_gallery_views_handler_image_count',
      'click sortable' => TRUE,
      'notafield' => TRUE,
    ),
  );
  $data['node_galleries_gallery']['gallery_op'] = array(
    'real field' => 'gid',
    'title' => t('Gallery Operations'),
    'help' => t('Links for gallery administration (uploading, editing, deleting etc.).'),
    'field' => array(
      'handler' => 'node_gallery_views_handler_gallery_op',
      'notafield' => TRUE,
    ),
  );
  return $data;
}

function node_gallery_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'node_gallery') .'/includes',
    ),
    'handlers' => array(
      'node_gallery_handler_field_fid' => array(
        'parent' => 'views_handler_field',
      ),
      'node_gallery_handler_field_gid' => array(
        'parent' => 'views_handler_field',
      ),
      'node_gallery_views_handler_image_count' => array(
        'parent' => 'views_handler_field_numeric',
      ),
      'node_gallery_views_handler_gallery_op' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}