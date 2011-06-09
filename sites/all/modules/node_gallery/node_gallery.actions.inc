<?php

/**
 * @file
 * Contains all actions for node gallery.
 */

/**
 * Implements hook_action_info().
 */
function node_gallery_action_info() {
  return array(
    'node_gallery_change_gallery_action' => array(
      'description' => t('Change parent gallery'),
      'type' => 'node',
      'behavior' => array('changes_node_property'),
      'configurable' => TRUE,
      'hooks' => array(
        'node' => array('presave'),
  ),
  ),
    'node_gallery_change_image_weight_action' => array(
      'description' => t('Change image weight (sort order)'),
      'type' => 'node',
      'behavior' => array('changes_node_property'),
      'configurable' => TRUE,
      'hooks' => array(
        'node' => array('presave'),
  ),
  ),
    'node_gallery_set_as_cover_action' => array(
      'description' => t('Set an image as the cover image'),
      'type' => 'node',
      'behavior' => array('changes_node_property'),
      'configurable' => FALSE,
      'hooks' => array(
        'node' => array('presave'),
  ),
  ),
  );
}

/**
 * Sets an image to be the cover image.
 *
 * @param object $node
 *   A populated node object.
 * @param $context
 *   (optional) An associative array supplied by the Actions API.
 */
function node_gallery_set_as_cover_action(&$node, $context = array()) {
  if (in_array($node->type, (array)node_gallery_get_types('image'))) {
    $node->is_cover = TRUE;
  }
}

/**
 * Sets an image's weight.
 *
 * @param object $node
 *   A populated node object.
 * @param $context
 *   (optional) An associative array supplied by the Actions API.
 */
function node_gallery_change_image_weight_action(&$node, $context = array()) {
  if (in_array($node->type, (array)node_gallery_get_types('image'))) {
    // All nodes are being set to the same weight
    if (is_numeric($context['imageweight'])) {
      $node->new_weight = $context['imageweight'];
    }
    // VBO is sending us a list of nodes to modify with different weights
    else {
      $node->new_weight = $context['imageweight'][$node->nid];
    }
  }
}

/**
 * Builds the form to change an image's weight.
 *
 * @param object $node
 *   A populated node object.
 * @param $context
 *   (optional) An associative array supplied by the Actions API.
 *
 * @return
 *   A FAPI form array.
 */
function node_gallery_change_image_weight_action_form($context = array()) {
  // We're being called from VBO - we can do extra validation
  if ($context['view']->plugin_name == 'bulk') {
    $sql = "SELECT n.nid, n.title, ngi.weight FROM {node} n " .
            "INNER JOIN {node_gallery_images} ngi ON n.nid = ngi.nid " .
            "WHERE n.nid IN (". db_placeholders($context['selection']) .")" .
            "ORDER BY weight ASC, created DESC";
    $result = db_query($sql, array_keys($context['selection']));
    $delta = count($context['selection']) > 20 ? intval(count($context['selection'])/2) : 10;
    // Note that #theme is overridden by VBO, so we set it back in hook_form_alter by looking for our custom attribute
    $form = array(
      '#ngtheme' => 'gallery_sort_images_form',
      '#cache' => TRUE,
      '#thumb_imagecache' => 'node-gallery-thumbnail',
    );
    $form['node_gallery_change_image_weight_action']['#tree'] = TRUE;
    while ($image = db_fetch_object($result)) {
      $form['node_gallery_change_image_weight_action'][$image->nid]['title'] = array(
        '#type' => 'item',
        '#value' => $image->title,
      );
      $form['node_gallery_change_image_weight_action'][$image->nid]['weight'] = array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $image->weight,
        '#delta' => $delta,
      );
    }
  }
  // We're called from a standard advanced action where we assign one weight to all nodes
  else {
    $form['node_gallery_change_image_weight_action'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#description' => t('When listing images in a gallery, heavier items will sink and the lighter items will be positioned near the top'),
      '#delta' => 10,
    );
    if (isset($context['imageweight'])) {
      $form['node_gallery_change_image_weight_action']['#default_value'] = $context['imageweight'];
    }
  }
  return $form;
}

/**
 * Submit handler for the weight change form.
 *
 * @param $form
 * @param $form_state
 */
function node_gallery_change_image_weight_action_submit($form, $form_state) {
  // We're setting all nodes to the same weight
  if (is_numeric($form_state['values']['node_gallery_change_image_weight_action'])) {
    $weight = $form_state['values']['node_gallery_change_image_weight_action'];
  }
  // VBO is passing us a set of nids
  else {
    foreach ($form_state['values']['node_gallery_change_image_weight_action'] as $nid => $val) {
      $weight[$nid] = $val['weight'];
    }
  }
  return array('imageweight' => $weight);
}

/**
 * Changes the gallery of an image node.
 * @param object $node
 * @param $context
 */
function node_gallery_change_gallery_action(&$node, $context = array()) {
  if (in_array($node->type, (array)node_gallery_get_types('image'))) {
    $node->newgid = $context['gallerynid'];
    $node->oldgid = $node->gid;
    $node->gid = $context['gallerynid'];
  }
}

/**
 * Builds the form to allow a user to change the gallery of an image.
 *
 * @param $context
 *
 * @return
 *   A FAPI form array.
 */
function node_gallery_change_gallery_action_form($context = array()) {
  global $user;

  $i2g = node_gallery_get_image_to_gallery_types();
  static $user_gids = array();

  // We're being called from VBO - we can do extra validation
  if ($context['view']->plugin_name == 'bulk') {
    // Make sure that all the images selected have the same gallery type
    foreach ($context['selection'] as $node) {
      $nids[] = $node->nid;
    }
    $sql = "SELECT DISTINCT n.type FROM {node} n WHERE n.nid IN (". db_placeholders($nids) .")";
    $result = db_query($sql, $nids);
    $imagetypes = (array)node_gallery_get_types('image');
    $count = 0;
    while ($imagetype = db_fetch_array($result)) {
      $count++;
      if ($count > 1) {
        drupal_set_message(t('You must choose only node gallery images with the same gallery type when using the "Change parent gallery" action.'), 'error');
        return array();
      }
      if (in_array($imagetype['type'], $imagetypes)) {
        $gallery_type = $i2g[$imagetype['type']];
        $image_type = $imagetype['type'];
      }
      else {
        drupal_set_message(t('You must choose only node gallery images with the same gallery type when using the "Change parent gallery" action.'), 'error');
        return array();
      }
    }
  }
  else {
    if (isset($context['gallerynid'])) {
      $gallery = node_load($context['gallerynid']);
      $gallery_type = $gallery->type;
      $relationship = node_gallery_get_relationship($gallery_type);
      $image_type = $relationship['image_type'];
    }
    elseif (isset($context['image_type'])) {
      $image_type = $context['image_type'];
      $gallery_type = $i2g[$context['image_type']];
      $gallery = (object)array('type' => $gallery_type);
    }
  }
  if (!isset($user_gids[$user->uid])) {    
    $user_gids[$user->uid] =  node_gallery_get_gallery_list($gallery_type, $user->uid);
  }
  $gallery_list = array();
  if ($context['allow_new_gallery'] == TRUE) {
    if (isset($context['image_type'])) {
      $gallery_type = $i2g[$context['image_type']];
      if (node_gallery_user_access('create', $gallery)) {
        $gallery_list['new_gallery'] = '<'. t('Create a new gallery') .'>';
      }
    }
  }
  // @todo: there's an odd bug here where an anonymous user with privs to view "Manage Images" can see all galleries in the dropdown.
  $gallery_list += $user_gids[$user->uid];

  $form['gid'] = array(
    '#type' => 'select',
    '#title' => t('Assign to Gallery'),
    '#options' => $gallery_list,
    '#weight' => -2,
  );
  if (isset($context['gallerynid'])) {
    $form['gid']['#default_value'] = $context['gallerynid'];
  }
  return $form;

}

/**
 * Validates the change gallery form.
 *
 * @param $form
 * @param $form_state
 */
function node_gallery_change_gallery_action_validate($form, $form_state) {
  $gid = $form_state['values']['gid'];
  if (!is_numeric($gid) && $gid != 'new_gallery') {
    form_set_error('gid', t('Please select a value.'));
  }
  if (is_numeric($gid)) {
    $count = db_result(db_query("SELECT COUNT(*) FROM {node_gallery_galleries} WHERE gid = '%d'", $gid));
    if (intval($count) != 1) {
      form_set_error('gid', t('Please enter a valid gallery.'));
    }
  }
}

/**
 * Submit handler for the change gallery form.
 *
 * @param $form
 * @param $form_state
 */
function node_gallery_change_gallery_action_submit($form, $form_state) {
  $gid = $form_state['values']['gid'];
  return array('gallerynid' => $gid);
}

