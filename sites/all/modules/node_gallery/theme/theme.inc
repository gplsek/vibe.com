<?php

/**
 * @file
 * Node gallery theme functions
 */

function theme_node_gallery_manage_images_form($form) {
  // Pull in the multi-select functionality from core
  drupal_add_js('misc/tableselect.js');
  // get fieldname to retrieve the filepath for the thumbnail without loading the node
  $relationship = node_gallery_get_relationship($form['#gallery']->type);
  $enable_rotation = FALSE;
  if ($relationship['settings']['manage_images_enable_rotation'] && (imageapi_default_toolkit() != 'imageapi_gd' || function_exists("imagerotate"))) {
    $enable_rotation = TRUE;
    if (module_exists('jquery_ui')) {
      drupal_add_css(drupal_get_path('module', 'jquery_ui') .'/jquery.ui/themes/default/ui.all.css');
      jquery_ui_add(array('ui.dialog', 'ui.draggable', 'ui.resizable'));
    }
    drupal_add_js(drupal_get_path('module', 'node_gallery') .'/js/ng_manage_images.js');
  }
  $field_name = $relationship['imagefield_name'];
  $thumb_imagecache = $form['#thumb_imagecache'];
  $header = array(array('data' => t('Delete'), 'class' => 'select-all'), t('Preview'), t('Edit'), t('Cover'));
  if ($enable_rotation) {
    $header = array(array('data' => t('Delete'), 'class' => 'select-all'), t('Preview'), t('Edit'), t('Rotation'), t('Cover'));
  }
  foreach (element_children($form['images']) as $nid) {
    $element = &$form['images'][$nid];
    $filepath = node_gallery_get_image_filepath($element['edit_form']['nid']['#value'], $field_name);

    $row = array();
    $row[] = drupal_render($element['remove']);
    $row[] = '<span>'. l(theme('imagecache', 'node-gallery-admin-thumbnail', $filepath), 'node/'. $nid .'/edit', array('html' => TRUE)) .'</span>';
    $edit = drupal_render($element['edit_form']);
    if (!empty($element['gid'])) {
      $edit .= drupal_render($element['gid']);
    }
    $row[] = $edit;
    if ($enable_rotation) {
      $row[] = drupal_render($element['rotate']);
    }
    if ($form['is_cover']) {
      $row[] = drupal_render($form['is_cover'][$nid]);
    }
    $rows[] = array('data' => $row);
  }
  $output = theme('table', $header, $rows, array('id' => 'upload-attachments'));
  $output .= drupal_render($form);
  return $output;
}

function theme_node_gallery_content_multiple_values($element) {
  $nid = $element['#parents'][1];
  $field_name = $element['#field_name'];
  $field = content_fields($field_name);
  $output = '';

  if ($field['multiple'] >= 1) {
    $table_id = $element['#field_name'] .'_values-' . $nid;
    $order_class = $element['#field_name'] .'-delta-order';
    $required = !empty($element['#required']) ? '<span class="form-required" title="'. t('This field is required.') .'">*</span>' : '';

    $header = array(
      array(
        'data' => t('!title: !required', array('!title' => $element['#title'], '!required' => $required)),
        'colspan' => 2
      ),
      t('Order'),
    );
    $rows = array();

    // Sort items according to '_weight' (needed when the form comes back after
    // preview or failed validation)
    $items = array();
    foreach (element_children($element) as $key) {
      if ($key !== $element['#field_name'] .'_add_more-'.$nid) {
        $items[] = &$element[$key];
      }
    }
    usort($items, '_content_sort_items_value_helper');

    // Add the items as table rows.
    foreach ($items as $key => $item) {
      $item['_weight']['#attributes']['class'] = $order_class;
      $delta_element = drupal_render($item['_weight']);
      $cells = array(
        array('data' => '', 'class' => 'content-multiple-drag'),
        drupal_render($item),
        array('data' => $delta_element, 'class' => 'delta-order'),
      );
      $rows[] = array(
        'data' => $cells,
        'class' => 'draggable',
      );
    }

    $output .= theme('table', $header, $rows, array('id' => $table_id, 'class' => 'content-multiple-table'));
    $output .= $element['#description'] ? '<div class="description">'. $element['#description'] .'</div>' : '';
    $output .= drupal_render($element[$element['#field_name'] .'_add_more-' . $nid]);

    drupal_add_tabledrag($table_id, 'order', 'sibling', $order_class);
  }
  else {
    foreach (element_children($element) as $key) {
      $output .= drupal_render($element[$key]);
    }
  }

  return $output;
}

function template_preprocess_node_gallery_image_navigator(&$variables) {
  $relationship = node_gallery_get_relationship(NULL, $variables['image']->type);
  $fragment = FALSE;
  if ($relationship['settings']['node_images_page_fragment']) {
    $fragment = TRUE;
  }
  $entries = array();
  if (isset($variables['navigator']['prev_nid'])) {
    $entries = array_merge($entries, array('prev', 'first'));
  }
  if (isset($variables['navigator']['next_nid'])) {
    $entries = array_merge($entries, array('next', 'last'));
  }
  foreach ($entries as $type) {
    $nid = $variables['navigator'][$type .'_nid'];
    $variables[$type .'_link'] = $fragment ?
      url('node/'. $nid, array('fragment' => 'node-'. $nid)) :
      url('node/'. $nid);
  }
$variables['gallery_link'] = url('node/'. $variables['navigator']['gallery_nid']);
  $gallery = node_load($variables['navigator']['gallery_nid']);
  $variables['gallery_title'] = check_plain($gallery->title);
}

/**
 * Imports the image properties into the variables namespace for easier theming.
 *
 * @param <type> $variables
 */
function template_preprocess_node_gallery_sort_images_grid_item(&$variables) {
  $variables += $variables['image'];
}

function theme_node_gallery_sort_images_form($form) {
  $output = '';
  if ($form['jquery_ui_integration']['#value']) {
    jquery_ui_add(array('ui.draggable', 'ui.droppable', 'ui.sortable'));
    drupal_add_js(drupal_get_path('module', 'node_gallery') .'/js/ng_sort_jqueryui.js');
    drupal_add_js(array('node_gallery' => array('gid' => $form['gid']['#value'])), 'setting');
    $output .= theme('node_gallery_sort_images_grid', $form['#images']);
  }
  else {
    $rows = array();
    $header = array('&uarr;&darr;', t('Preview'), t('Title'), t('Post date'), t('Updated date'), t('Published'), t('Weight'));
    $i = 0;
    foreach ($form['#images'] as $image) {
      $published = ($image['status']) ? t('Yes') : t('No');
      $row = array(
        // The placeholder for the tabledrag icon
        '',
        $image['admin_thumb'],
        $image['title'],
        format_date($image['created'], 'small'),
        format_date($image['changed'], 'small'),
        $published,
        // the weight drop down
        drupal_render($form['images-sort-'. $i]),
      );
      $rows[] = array('data' => $row, 'class' => 'draggable');
      $i++;
    }
    // this will replace the weight drop downs with drag&drop when javascript is available
    drupal_add_tabledrag('sort-images-table', 'order', 'sibling', 'sort');
    $output .= theme('table', $header, $rows, array('id' => 'sort-images-table'));
  }
  // render the left over elements, if any
  $output .= drupal_render($form);
  return $output;
}

/**
 * Node gallery formatter hooks and callbacks.
 */
function theme_node_gallery_formatter_nextimagelink($element) {
  // Inside a view $element may contain null data. In that case, just return.
  if (empty($element['#item']['fid'])) {
    return '';
  }

  // If node type is not in a gallery relationship or gid is not set, just render the default formatter.
  $imagetag = theme_imagecache_formatter_default($element);
  $node = $element['#node'];
  if (!in_array($node->type, (array)node_gallery_get_types('image'))
          || !isset($node->gid)) {
    return $imagetag;
  }
  $relationship = node_gallery_get_relationship(NULL, $node->type);
  $next = node_gallery_get_next_image($node->gid, $node->nid);
  if ($next !== NULL) {
    $class = 'imagefield image-nextimagelink imagefield-'. $element['#field_name'];
    $options = array('attributes' => array('class' => $class), 'html' => TRUE);
    if ($relationship['settings']['node_images_page_fragment']) {
      $options['fragment'] = 'node-'. $next;
    }
    return l($imagetag, 'node/'. $next, $options);
  }
  else {
    // no link on the last image
    return $imagetag;
  }
}

/**
 * Link an image to its gallery.
 */
function theme_node_gallery_formatter_gallerylink($element) {
  if (empty($element['#item']['fid'])) {
    return '';
  }
  $imagetag = theme_imagecache_formatter_default($element);
  if (!in_array($element['#node']->type, (array)node_gallery_get_types('image'))) {
    return $imagetag;
  }
  $node = node_load($element['#node']->nid);
  $class = 'imagefield image-gallerylink imagefield-'. $element['#field_name'];
  $options = array('attributes' => array('class' => $class), 'html' => TRUE);
  return l($imagetag, 'node/'. $node->gid, $options);
}
