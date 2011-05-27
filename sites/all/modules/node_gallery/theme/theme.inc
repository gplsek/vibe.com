<?php
// $Id: theme.inc,v 1.1.2.18 2010/05/08 14:02:30 justintime Exp $

/**
 * @file theme/theme.inc
 * Node gallery theme functions
 *
 */

function theme_gallery_list($items, $account = NULL) {
  $output = '<div class="gallery-list">';
  $output .= node_gallery_operations('list', $account);
  if (empty($items)) {
    $output .= t('There are currently no galleries.');
  }
  else {
    $output .= theme('item_list', $items, NULL, 'ul', array('class' => 'gallery-cover-list'));
  }
  $output .= '</div>';
  
  return $output;
}

function template_preprocess_gallery_cover_view(&$vars) {
  $gallery = $vars['gallery'];
  $cover_image = node_gallery_get_cover($gallery);
  $vars['cover_image'] = theme('image_view', $gallery->config['image_size']['cover'], $cover_image);
  $vars['meta_data'] = theme('item_list', theme('gallery_meta', $gallery));
  $vars['cover_operations'] = node_gallery_operations('cover', $gallery);
}

function template_preprocess_gallery_teaser(&$vars) {
  $gallery = $vars['gallery'];
  $config = $vars['config'];
  
  //cover display
  if ($config['teaser']['gallery_display_type'] == 'cover') {
    $cover = node_gallery_get_cover($gallery);
    $image_tag = theme('image_view', $config['image_size']['cover'], $cover);
    $teaser = l($image_tag, 'node/'. $gallery->nid, array('html' => TRUE));
  }
  else {
    $display_num = $config['teaser']['thumbnails_num'];
    $i = 0;
    // Make sure to avoid an "Invalid argument supplied for foreach()" error
    if ($gallery->images) {
      foreach ($gallery->images as $image) {
        if ($i < $display_num) {
          $image_tag = theme('gallery_image_thumbnail', $image, $config, NODE_GALLERY_VIEW_TEASER);
          $items[] = l($image_tag, 'node/'. $gallery->nid, array('html' => TRUE));
          $i ++;
        }
      }
    }
    $teaser = $items;
  }
  $vars['gallery_teaser'] = $teaser;
}

function template_preprocess_gallery_images_list(&$vars) {
  $gallery = $vars['gallery'];
  $config = $vars['config'];
  
  if (!count($gallery->images)) {
    $empty_message = '<p>'. t('There are currently no images in this gallery.');
    if (node_gallery_user_access('edit', $gallery)) {
      $empty_message .= '  '. l(t('You may upload images to this gallery.'), 'node/'. $gallery->nid .'/upload', array('query' => 'destination=node/'. $gallery->nid));
    }
    $empty_message .= '</p>';
  }
  else {
    foreach ($gallery->images as $nid => $image) {
      $items[] = theme('gallery_image_thumbnail', $image, $config, NODE_GALLERY_VIEW_IMAGE_LIST);
    }
  }
  $vars['empty_message'] = $empty_message;
  $vars['images_list'] = $items;
}

function template_preprocess_image_detail_view(&$vars) {
  $node = $vars['image'];
  $config = $vars['config'];
  
  $image_view = theme('image_view', $config['image_size']['preview'], $node);
  switch ($config['original']['view_original']) {
    case 'default':
      $output = l($image_view, file_create_url($node->filepath),
         array('attributes' => array('target' => '_blank'), 'html' => TRUE));
      break;
    case 'text':
      $download_text = empty($config['original']['view_original_text']) ? t('Download the Original Image') : $config['original']['view_original_text'];
      $output = $image_view;
      $extra = '<div class="download-full-link">'. l($download_text, file_create_url($node->filepath), array('attributes' => array('target' => '_blank'), 'html' => FALSE)) .'</div>';
      break;
    default:
      $output = $image_view;
      break;
  }
  $vars['image'] = $output;
  $vars['extra'] = $extra;
}

function template_preprocess_gallery_image_thumbnail(&$vars) {
  $image = $vars['image'];
  $config = $vars['config'];
  $mode = $vars['mode'];
  
  switch ($mode) {
    case NODE_GALLERY_VIEW_TEASER:
      $url = "node/". $image->gid;
      break;
    case NODE_GALLERY_VIEW_IMAGE_LIST:
      $url = "node/". $image->nid;
      break;
  }
  
  $vars['image_output'] = l(theme('image_view', $config['image_size']['thumbnail'], $image), $url, array('html' => TRUE));
  $vars['title_output'] = l($image->title, 'node/'. $image->nid);
}

function theme_image_view($imagecache, $image) {
  return theme('imagecache', $imagecache, $image->filepath, $image->title, $image->title);
}

function theme_gallery_edit_images_form($form) {
  drupal_add_tabledrag('upload-attachments', 'order', 'sibling', 'upload-weight');
  
  $thumb_imagecache = $form['#thumb_imagecache'];
  $header = array('', t('Delete'), t('Thumbnail'), t('Edit'), t('Weight'), t('Cover'));
  foreach (element_children($form['files']) as $key) {
    // Add class to group weight fields for drag and drop.
    $form['files'][$key]['weight']['#attributes']['class'] = 'upload-weight';

    $row = array('');
    $row[] = drupal_render($form['files'][$key]['remove']);
    $row[] = '<span class="gallery-upload-thumb">'. theme('imagecache', $thumb_imagecache, 
      $form['files'][$key]['filepath']['#value'], $form['files'][$key]['filename']['#value'], $form['files'][$key]['filename']['#value']) .'</span>';
    $row[] = drupal_render($form['files'][$key]['edit_form']) . drupal_render($form['files'][$key]['gid']);
    $row[] = drupal_render($form['files'][$key]['weight']);
    if ($form['is_cover']) {
      $row[] = drupal_render($form['is_cover'][$key]);
    }
    $rows[] = array('data' => $row, 'class' => 'draggable');
  }
  $output = theme('table', $header, $rows, array('id' => 'upload-attachments'));
  $output .= drupal_render($form);
  return $output;
}

function theme_gallery_sort_images_form($form) {
  drupal_add_tabledrag('upload-attachments', 'order', 'sibling', 'upload-weight');
  
  $thumb_imagecache = $form['#thumb_imagecache'];
  $header = array('', t('Delete'), t('Thumbnail'), t('Weight'), t('Cover'));
  foreach (element_children($form['files']) as $key) {
    // Add class to group weight fields for drag and drop.
    $form['files'][$key]['weight']['#attributes']['class'] = 'upload-weight';

    $row = array('');
    $row[] = drupal_render($form['files'][$key]['remove']);
    $row[] = '<span class="gallery-upload-thumb">'. theme('imagecache', $thumb_imagecache, 
      $form['files'][$key]['filepath']['#value'], $form['files'][$key]['filename']['#value'], $form['files'][$key]['filename']['#value']) .'</span>';
    //$row[] = drupal_render($form['files'][$key]['edit_form']) . drupal_render($form['files'][$key]['gid']);
    $row[] = drupal_render($form['files'][$key]['weight']);
    if ($form['is_cover']) {
      $row[] = drupal_render($form['is_cover'][$key]);
    }
    $rows[] = array('data' => $row, 'class' => 'draggable');
  }
  $output = theme('table', $header, $rows, array('id' => 'upload-attachments'));
  $output .= drupal_render($form);
  return $output;
}

function theme_gallery_image_navigator($navigator, $image) {
  $col1 = array('data' => t("Image %current of %total", array('%current' => $navigator['current'], '%total' => $navigator['total'])),
    'class' => 'image-navigator-left');
  // Eliminate prev/next links when we only have one image
  if ($navigator['total'] <= 1) {
    $col2 = array('data' => t('&nbsp;'), 'class' => 'image-navigator-mid');
  } 
  else {
    if (variable_get('node_images_page_fragment', FALSE)) {
      $col2 =  array('data' => l(t('Previous'), 'node/'. $navigator['prev_nid'], array(fragment => 'node-inner')) .'/'. l(t('Next'), 'node/'. $navigator['next_nid'], array(fragment => 'node-inner')),
        'class' => 'image-navigator-mid');
    }
    else {
      $col2 = array('data' => l(t('Previous'), 'node/'. $navigator['prev_nid']) .'/'. l(t('Next'), 'node/'. $navigator['next_nid']),
        'class' => 'image-navigator-mid');
    }
  }
  $col3 = array('data' => node_gallery_operations('image', $image), 'class' => 'image-navigator-right');
  $rows[] = array($col1, $col2, $col3);

  return theme('table', NULL, $rows, array('class' => 'image-navigator')); 
}

function theme_gallery_meta($gallery) {
  $images_count = empty($gallery->images_count) ? count($gallery->images) : $gallery->images_count;
  $items[] = format_plural($images_count, '1 image', '@count images');
  $items[] = t('Created at: !date', array('!date' => format_date($gallery->created, 'custom', 'Y-m-d')));
  
  return $items;
}