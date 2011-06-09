<?php

/**
 * @file
 * Handles displaying of Node gallery pages.
 *
 */

function node_gallery_upload_image_to_new_gallery_form($image_type) {
  drupal_set_message(t('Using this wizard, a gallery will be created automatically during upload. Please remember to enter a correct title on the next screen.'));
  $relationship = node_gallery_get_relationship(NULL, $image_type);
  $gallery = array(
    'nid' => -1,
    'type' => $relationship['gallery_type'],
    'edit_after_plupload' => TRUE,
  );
  return node_gallery_upload_image_form((object)$gallery);
}

/**
 * Javascript page callback to create an empty gallery.
 *
 * @param String $gallery_type
 *  The type of the gallery to create.
 * @return
 *  Json encoded newly created gallery.
 */
function node_gallery_json_create_gallery($gallery_type) {
  $gallery = (object)array('type' => $gallery_type);
  if (!node_gallery_user_access('create', $gallery)) {
    return drupal_json(array('is_error' => 1, 'message' => t("You don't have permission to create a gallery.")));
  }
  $gallery = _node_gallery_create_new_gallery($gallery_type);
  drupal_json($gallery);
}

/**
 * Displays the content of the "Upload Images" tab.
 *
 * @param $gallery
 *   A populated node gallery object which will house the images.
 *
 * @return
 *   HTML output.
 */
function node_gallery_upload_image_form($gallery) {
  global $user;

  if ($gallery->nid > 0) {
    drupal_set_breadcrumb(array(
      l(t('Home'), NULL),
      l(t('Galleries'), 'galleries'),
      l(t('!user\'s Galleries', array('!user' => $gallery->name)), 'galleries/' . $gallery->uid),
      l($gallery->title, 'node/'. $gallery->nid)
    ));
  }
  $relationship = node_gallery_get_relationship($gallery->type);

  if (module_exists('plupload') && variable_get('node_gallery_plupload_integration', TRUE)) {
    $filefield_name = $relationship['image_type'] .':::'. $relationship['imagefield_name'];
    $options = array('drupal_pluploaded' => TRUE, 'drupal_gid' => $gallery->nid, 'drupal_plupload_import_field_type' => $filefield_name);
    $output = '';
    if (!user_access('bulk upload files with plupload')) {
      drupal_set_message(t('You have permissions to view this tab, but not to upload images.  Please inform an administrator that you need the \'bulk upload files with plupload\' permission.'), 'warning');
    }
    else {
      if ($gallery->edit_after_plupload) {
        $settings = array(
          'gid' => $gallery->nid,
          'galleryType' => $gallery->type,
        );
        drupal_add_js(array('node_gallery' => $settings), 'setting');
        drupal_add_js(drupal_get_path('module', 'node_gallery') .'/js/ng_plupload.js');
      }
      $output = plupload_upload_page($options);
    }
    return $output;
  }

  // Pull the image type from the relationship, and display it's form:
  $imagetype = $relationship['image_type'];
  $node = new stdClass;
  $node->type = $imagetype;
  $node->uid = $user->uid;
  $node->name = (isset($user->name) ? $user->name : '');
  module_load_include('pages.inc', 'node');
  $output = drupal_get_form($imagetype .'_node_form', $node);
  return $output;
}

function node_gallery_browse_images($gallery) {
  $first_image = node_gallery_get_first_image($gallery->nid);
  $path = 'node/';
  if (is_numeric($first_image)) {
    $path .= $first_image;
    // Set our pretty path
    $path = drupal_get_path_alias($path);
  }

  drupal_goto($path);
}

/**
 * Displays the gallery summary page, which is a user-specified view.
 * @param $user
 *   (optional) A user object, defaults to NULL.  If present, displays only
 *   galleries owned by that user.
 * @param $content_type
 *   (optional) A gallery content type.  If present, the view only displays
 *   galleries of this type.
 *
 * @return
 *   HTML Output
 */
function node_gallery_list_galleries($user = NULL, $content_type = NULL) {
  if (!$user) {
    $user = NULL;
  }
  if (!$content_type) {
    $content_type = NULL;
  }
  $viewkey = unserialize(variable_get('node_gallery_galleries_summary_view', serialize(array('name' => 'node_gallery_gallery_summaries', 'display_id' => 'page_1'))));
  $breadcrumbs[] = l(t('Home'), NULL);
  $breadcrumbs[] = l(t('Galleries'), 'galleries');
  if (isset($user->name)) {
    $breadcrumbs[] = l(node_gallery_list_galleries_title($user), 'galleries/' . $user->uid);
  }
  drupal_set_breadcrumb($breadcrumbs);
  // When there's no user, use the view title in the all galleries list.
  if ( is_null($user) ) {
    $view = views_get_view($viewkey['name']);
    $title = $view->display[$viewkey['display_id']]->display_options['title'];
    if ( !empty($title) ) {
      drupal_set_title($title);
    }
  }
  // @todo: we should be able to programmatically set some options on the view, such as number of images, imagefield_name, etc.
  $output = views_embed_view($viewkey['name'], $viewkey['display_id'], $user->uid, $content_type);
  return $output;
}

/**
 * Submit handler for our "Upload Images" form.
 *
 * @param $form
 * @param $form_state
 */
function node_gallery_upload_image_form_submit($form, &$form_state) {
  global $user;
  $image = new stdClass;
  $image->uid = $user->uid;
  $image->name = (isset($user->name) ? $user->name : '');
  $values = $form_state['values'];
  foreach ($values as $key => $value) {
    $image->$key = $value;
  }
  node_save($image);
  drupal_set_message(t('Created new image node !title.', array('!title' => $image->title)));
}

/**
 * Displays the content for our "Manage Images" tab, which is a VBO view.
 * @param $form_state
 *   Drupal $form_state array
 * @param $gallery
 *   A populated node gallery object.
 *
 * @return
 *   FAPI array
 */
function node_gallery_manage_images_form($form_state, $gallery) {
  global $pager_page_array, $pager_total;
  $relationship = node_gallery_get_relationship($gallery->type);

  // we must set these globals, because we do not call pager_query.
  $element = 0;
  $pager_page_array = (isset($_GET['page']) ? explode(',', $_GET['page']) : array());
  if (empty($pager_page_array[$element])) {
    $pager_page_array[$element] = 0;
  }
  $items_per_page = $relationship['settings']['manage_images_per_page'];

  drupal_set_breadcrumb(array(
    l(t('Home'), NULL),
    l(t('Galleries'), 'galleries'),
    l(t('!user\'s Galleries', array('!user' => $gallery->name)), 'galleries/'. $gallery->uid),
    l($gallery->title, 'node/'. $gallery->nid)
    ));

  $form = array(
    '#theme' => 'node_gallery_manage_images_form',
    '#cache' => TRUE,
  );

  if ($relationship['settings']['manage_images_show_gallery_list']) {
    $gallery_list = node_gallery_get_gallery_list($gallery->type, $gallery->uid);
  }

  $form['#description'] = t('Manage Images form.');
  $form['#thumb_imagecache'] = 'node-gallery-admin-thumbnail';
  $form['#gallery'] = $gallery;
  $form['#programmed'] = TRUE;

  $images = array();
  if (isset($_SESSION['node_gallery_plupload_nids'][$gallery->nid])) {
    $images = $_SESSION['node_gallery_plupload_nids'][$gallery->nid];
    // We trim the $images array to a max length to prevent OOM's on the manage images form.
    // There's just no easy way to do paging for just one visit to the manage images tab.
    drupal_set_message(t('Current display is filtered to show only images just uploaded.  Please refresh the page to display all images in the gallery.'), 'status');
    $length = min(variable_get('node_gallery_plupload_manage_images_limit', 100), count($images));
    $images = array_slice($images, 0, $length);
    $chunks = array_chunk($images, count($images));
    unset($_SESSION['node_gallery_plupload_nids'][$gallery->nid]);
  }
  else {
    $images = node_gallery_get_image_nids($gallery->nid, TRUE, FALSE);
    $chunks = array_chunk($images, $items_per_page);
    $images = $chunks[$pager_page_array[$element]];
  }
  $pager_total[$element] = count($chunks);
  $enable_rotation = FALSE;
  if ($relationship['settings']['manage_images_enable_rotation'] && (imageapi_default_toolkit() != 'imageapi_gd' || function_exists("imagerotate"))) {
    $enable_rotation = TRUE;
  }
  if (!empty($images)) {
    $count = 0;
    $form['images']['#tree'] = TRUE;
    foreach ($images as $nid) {
      $count++;
      if ($count % 50 == 0) {
        // Make sure we don't run out of memory by clearing the cache.
        $image = node_load(NULL, NULL, TRUE);
      }
      $image = node_load($nid);
      $options[$nid] = '';
      $form['images'][$nid]['remove'] = array('#type' => 'checkbox', '#default_value' => FALSE);
      if ($enable_rotation) {
        $form['images'][$nid]['rotate'] = array(
          '#type' => 'radios',
          '#options' => array(
            0 => t('None'),
            90 => t('90° CW'),
            270 => t('90° CCW'),
            180 => t('180°')
          ),
          '#default_value' => 0,
        );
        if (module_exists('jquery_ui')) {
          // add a link to a jquery ui dialog with previews
          $form['images'][$nid]['rotate']['#suffix'] = '<a href="#" class="ng3-rotate-link" rel="'. imagecache_create_path('node-gallery-thumbnail', $image->{$relationship['imagefield_name']}[0]['filepath']) .'" />rotate</a>';
        }
      }
      if ($relationship['settings']['manage_images_show_gallery_list']) {
        $form['images'][$nid]['gid'] = array('#type' => 'select', '#title' => t('Gallery'), '#default_value' => $gallery->nid, '#options' => $gallery_list);
      }
      else {
        $form['images'][$nid]['gid'] = array('#type' => 'value', '#value' => $gallery->nid);
      }     
      $form['images'][$nid]['edit_form'] = node_gallery_image_item_edit_form($form_state['values']['images'][$nid]['edit_form'], $image, $relationship);
      //Some CCK widgets need to have #field_info populated
      if (!isset($form['#field_info'])) {
        $form['#field_info'] = $form['images'][$nid]['edit_form']['#field_info'];
      };
    }
  }

  $cover_nid = $gallery->cover_image;
  $form['is_cover'] = array(
    '#type' => 'radios',
    '#default_value' => $cover_nid,
    '#options' => $options,
  );

  $form['pager'] = array(
    '#value' => theme('pager', NULL, $items_per_page, $element),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 20,
    //need this because this form is used in image upload ahah also.
    '#submit' => array('node_gallery_manage_images_submit'),
    '#validate' => array('node_gallery_manage_images_validate'),
  );
  return $form;
}

/**
 * Eliminates any fieldsets from the image item edit form.
 *
 * @param array $form_item
 * @param array $flattened
 * @return Flattened array.
 */
function node_gallery_flatten_image_item_edit_form($form_item, &$flattened = array(), $inheritance = array()) {
  if (empty($form_item) || !is_array($form_item)) {
    return $form_item;
  }
  if (empty($flattened)) {
    // we start with the whole form, because nested values will be overwritten
    $flattened = $form_item;
  }
  foreach (element_children($form_item) as $key) {
    $element = $form_item[$key];
    if (isset($element['#type']) && $element['#type'] == 'fieldset') {
      if (isset($element['#tree']) && !empty($element['#tree'])) {
        // we have to keep this as its tree preserving. just remove the fieldset.
        unset($element['#type']);
        $flattened[$key] = $element;
      }
      else {
        // this overwrites the old level if existing, so the field
        // inherits title and weight from the 'deepest' fieldset
        $inheritance['#title'] = $element['#title'];
        $inheritance['#weight'] = $element['#weight'];
        unset($flattened[$key]);
        node_gallery_flatten_image_item_edit_form($element, $flattened, $inheritance);
        $inheritance = array();
      }
    }
    else {
      $flattened[$key] = array_merge($inheritance, $element);
    }
  }
  return $flattened;
}

function node_gallery_manage_images_validate($form, &$form_state) {
  if (!empty($form_state['values']['images'])) {
    foreach ($form_state['values']['images'] as $nid => $image_form_state) {
      node_validate($image_form_state['edit_form'], $form['images'][$nid]['edit_form']);
    }
  }
}

function node_gallery_manage_images_submit(&$form, &$form_state) {
  $relationship = node_gallery_get_relationship($form['#gallery']->type);
  $compare_fields = array();
  if ($relationship['settings']['manage_images_show_gallery_list']) {
    $compare_fields[] = 'gid';
  }
  foreach ($relationship['settings']['manage_images_fields'] as $k => $f) {
    if ($f) {
      if ($k == 'body_field') {
        $compare_fields[] = 'body';
      } else if ($k == 'author') {
        $compare_fields[] = 'name';
        $compare_fields[] = 'date';
      } else if ($k == 'options') {
        // publication settings
        $compare_fields[] = 'status';
        $compare_fields[] = 'promote';
        $compare_fields[] = 'sticky';
      } else {        
        $compare_fields[] = $k;
      }
    }
  }
  if ($form_state['values']['is_cover'] != $form['#gallery']->cover_image) {
    // The user selected a new cover image for the gallery.
    // Build an object with the necessary attributes and update the gallery record.
    $image = new stdClass;
    $image->nid = $form_state['values']['is_cover'];
    $image->is_cover = 1;
    $image->gid = $form['#gallery']->nid;
    node_gallery_set_gallery_cover_image($image);
  }
  foreach ($form_state['values']['images'] as $nid => $form_values) {
    // strip add-more buttons
    foreach ($form_values['edit_form'] as $field => $values) {
      if (strpos($field, 'field_') !== FALSE && is_array($values)) {
        unset($form_values['edit_form'][$field][$field.'_add_more-'.$nid]);
      }
    }
    $image_node = (object)(array_merge($form_values['edit_form'], array('gid' => $form_values['gid'])));
    if ($form_values['remove']) {
      $op_images['delete'][] = $image_node->nid;
    }
    else {
      if (node_gallery_images_check_update($form['images'][$image_node->nid]['edit_form']['#node'], $image_node, $compare_fields)) {
        // Setting oldgid and newgid on our node clears caches on node_save
        if ($relationship['settings']['manage_images_show_gallery_list']) {
          if ($form['images'][$image_node->nid]['edit_form']['#node']->gid != $image_node->gid) {
            $image_node->oldgid = $form['images'][$image_node->nid]['edit_form']['#node']->gid;
            $image_node->newgid = $image_node->gid;
          }
        }
        if (in_array('author', $relationship['settings']['manage_images_fields'])) {
          $new_author = user_load(array('name' => $image_node->name));
          $image_node->uid = $new_author->uid;
          $image_node->created = strtotime($image_node->date);
        }
        $op_images['update'][] = $image_node;
      }
    }
    if ($form_values['rotate']) {
      $allowed_degrees = array('90', '270', '180');
      $degrees = (int)$form_values['rotate'];
      if (in_array($degrees, $allowed_degrees)) {
        $node = $form['images'][$image_node->nid]['edit_form']['#node'];
        $op_images['rotate'][] = array($node->{$relationship['imagefield_name']}[0]['filepath'], $degrees);
      }
    }
  }
  while (!empty($op_images['update'])) {
    $node = array_shift($op_images['update']);
    $operations[] = array('node_gallery_batch_node_save', array($node));
  }
  while (!empty($op_images['delete'])) {
    $nid = array_shift($op_images['delete']);
    $operations[] = array('node_gallery_image_delete_process', array($nid));
  }
  while (!empty($op_images['rotate'])) {
    $params = array_shift($op_images['rotate']);
    $operations[] = array('node_gallery_batch_rotate_callback', $params);
  }
  if (!empty($operations)) {
    $batch = array(
      'operations' => $operations,
      'finished' => 'node_gallery_image_process_finished',
      'title' => t("Modifying Images"),
      'init_message' => t("Images modification process is starting."),
      'progress_message' => t('Processed @current out of @total.'),
      'error_message' => t('Images modification has encountered an error.'),
    );

    batch_set($batch);
    //For memory management, we wipe the values from the form since it's all on the $operations array now
    $tmp = $form_state['values']['form_build_id'];
    $form_state = array();
    $form_state['values']['form_build_id'] = $tmp;
    $form = array();
  }
  $gid = isset($image_node->oldgid) ? $image_node->oldgid : $image_node->gid;
  $form_state['redirect'] = 'node/'. $gid;
}

/**
 * Check if we need to update this image;
 *
 * @param unknown_type $old_image
 * @param unknown_type $new_image
 * @param unknown_type $compare_fields
 * @return unknown
 */
function node_gallery_images_check_update($old_image, $_new_image, $compare_fields) {
  // a list of node attributes that are flattened one level by node_save
  $flattened_attributes = array('options');
  
  $new_image = drupal_clone($_new_image);
  $new_image = node_submit($new_image);

  if ($extra = node_invoke($new_image, 'presave')) {
    foreach ($extra as $key => $value) {
      $new_image->$key = $value;
    }
  }
  if ($extra = node_invoke_nodeapi($new_image, 'presave')) {
    foreach ($extra as $key => $value) {
      $new_image->$key = $value;
    }
  }
  if ($extra = node_invoke($old_image, 'presave')) {
    foreach ($extra as $key => $value) {
      $old_image->$key = $value;
    }
  }
  if ($extra = node_invoke_nodeapi($old_image, 'presave')) {
    foreach ($extra as $key => $value) {
      $old_image->$key = $value;
    }
  }

  foreach ($compare_fields as $f) {
    if (in_array($f, $flattened_attributes)) {
      // We make the old node look like the new one, ie $old_node->options['status'] = $old_node->status
      foreach (array_keys($new_image->{$f}) as $option) {
        $old_image->{$f}[$option] = $old_image->$option;
      }
    }
    //a hack for cck field validate;
    if (is_array($new_image->{$f})) {
      foreach ($new_image->{$f} as &$ff) {
        if (is_array($ff)) {
          unset($ff['_error_element']);
        }
      }
    }
    // special case for taxonomy, as its representation in form_state is different than in the node object.
    if ($f == 'taxonomy') {
      if (node_gallery_taxonomy_compare($old_image, $new_image)) {
        return TRUE;
      }
      else {
        continue;
      }
    }
    if ($new_image->{$f} != $old_image->{$f}) {
      return TRUE;
    }
  }
  return FALSE;
}


function node_gallery_image_item_edit_form($form_values, $image, $relationship) {
  module_load_include('inc', 'node', 'node.pages');
  $form_state = array('values' => $form_values);

  $display_fields = $relationship['settings']['manage_images_fields'];
  $form = drupal_retrieve_form($image->type .'_node_form', $form_state, $image);  
  drupal_prepare_form($image->type ."_node_form", $form, $form_state);
  $item_form = array();
  $display_fields['#field_info'] = '#field_info'; //this should be merged with parent form to optionwidget to work
  $display_fields['#node'] = '#node';

  foreach (array_values($display_fields) as $field_name) {
    if (!empty($field_name) && !empty($form[$field_name])) {
      $item_form[$field_name] = $form[$field_name];
    }
  }  
  $item_form += (array)node_gallery_get_image_form_value_items($form);
  node_gallery_set_image_form_default_values($item_form, $image, $relationship);
  $item_form = node_gallery_flatten_image_item_edit_form($item_form);

  foreach (element_children($item_form) as $key) {
    $element =& $item_form[$key];
    if (isset($element['#theme']) && $element['#theme'] == 'content_multiple_values') {
      // we need to override several properties for multi-value fields
      $nid = $item_form['#node']->nid;
      $fieldname = $element['#field_name'];
      $field_name_css = str_replace('_', '-', $fieldname);
      $item_form[$key]['#prefix'] = '<div id="' . $field_name_css . '-items-'.$nid.'">';
      $item_form[$key][$fieldname.'_add_more-'.$nid] = $item_form[$key][$fieldname.'_add_more'];
      $item_form[$key][$fieldname.'_add_more-'.$nid]['#ahah']['wrapper'] = $field_name_css . '-items-'.$nid;
      // change the path of the AHAH call
      $item_form[$key][$fieldname.'_add_more-'.$nid]['#ahah']['path'] = 'node-gallery/json/js_add_more/'. $image->type .'/'. $fieldname;
      $item_form[$key][$fieldname.'_add_more-'.$nid]['#name'] = $fieldname.'_add_more-'.$nid;
      unset($item_form[$key][$fieldname.'_add_more']);
      //$element['#field_name'] = $fieldname . '-' . $nid;
      $item_form[$key]['#theme'] = 'node_gallery_content_multiple_values';
    }
  }
  return $item_form;
}

/**
 * Menu callback for AHAH addition of new empty widgets, modeled after
 * content_add_more_js() in content.node_form.inc.
 *
 * @see content_add_more_js
 */
function node_gallery_content_add_more_js($type_name_url, $field_name) {
  module_load_include('inc', 'content', 'includes/content.node_form');
  $type = content_types($type_name_url);
  $field = content_fields($field_name, $type['type']);

  if (($field['multiple'] != 1) || empty($_POST['form_build_id'])) {
    // Invalid request.
    drupal_json(array('data' => ''));
    exit;
  }
  // Retrieve the cached form.
  $form_state = array('submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);
  if (!$form) {
    // Invalid form_build_id.
    drupal_json(array('data' => ''));
    exit;
  }

  $form_copy = $form;
  $form_state_copy = $form_state;
  $form_copy['#post'] = array();
  form_builder($_POST['form_id'], $form_copy, $form_state_copy);
  // Just grab the data we need.
  $form_state['values'] = $form_state_copy['values'];
  // Reset cached ids, so that they don't affect the actual form we output.
  form_clean_id(NULL, TRUE);

  // find out which button was pressed
  $button_name = FALSE;
  $nid = 0;
  foreach ($_POST['images'] as $cur_nid => $image) {
    if (isset($_POST[$field_name . '_add_more-'.$cur_nid])) {
      $button_name = $field_name . '_add_more-'.$cur_nid;
      $nid = $cur_nid;
    }
  }
  if (!$button_name) {
    drupal_json(array('data' => ''));
    exit;
  } 
  // unset button
  unset($form_state['values']['images'][$nid]['edit_form'][$field_name][$field_name . '_add_more-'.$nid]);

  // now simulate the submission of just this image edit form
  $original_form_state = $form_state;
  $original_post = $_POST;
  $original_form = $form;
  $_POST = $_POST['images'][$nid]['edit_form'];
  $form_state['values'] = $form_state['values']['images'][$nid]['edit_form'];

  // iterate current values
  foreach ($_POST[$field_name] as $delta => $item) {
    $form_state['values'][$field_name][$delta]['_weight'] = $item['_weight'];
  }
  $form_state['values'][$field_name] = _content_sort_items($field, $form_state['values'][$field_name]);
  $_POST[$field_name] = _content_sort_items($field, $_POST[$field_name]);

  // Build our new form element for the whole field, asking for one more element.
  $form_state['item_count'] = array($field_name => count($_POST[$field_name]) + 1);
  $form_element = content_field_form($form, $form_state, $field);

  // Make the same modifications as in building the manage images form!
  $field_name_css = str_replace('_', '-', $field_name);
  $form_element[$field_name]['#prefix'] = '<div id="' . $field_name_css . '-items-'.$nid.'">';
  $form_element[$field_name][$field_name.'_add_more-'.$nid] = $form_element[$field_name][$field_name.'_add_more'];
  $form_element[$field_name][$field_name.'_add_more-'.$nid]['#ahah']['wrapper'] = $field_name_css . '-items-'.$nid;
  $form_element[$field_name][$field_name.'_add_more-'.$nid]['#ahah']['path'] = 'node-gallery/json/js_add_more/'. $form['#node']->type .'/'. $field_name;
  $form_element[$field_name][$field_name.'_add_more-'.$nid]['#name'] = $field_name.'_add_more-'.$nid;
  unset($form_element[$field_name][$field_name.'_add_more']);
  $form_element[$field_name]['#theme'] = 'node_gallery_content_multiple_values';
  $form['images'][$nid]['edit_form'][$field_name] = $form_element[$field_name];
  
  // merge it again 
  $form_state = $original_form_state;
  $form_state['values']['images'][$nid]['edit_form'] = array();

  // Save the new definition of the form.
  form_set_cache($form_build_id, $form, $form_state);

  // Build the new form against the incoming $_POST values so that we can
  // render the new element.
  $delta = max(array_keys($_POST[$field_name])) + 1;
  $_POST[$field_name][$delta]['_weight'] = $delta;
  $tmp = $_POST;
  $_POST = $original_post;
  $_POST['images'][$nid]['edit_form'][$field_name] = $tmp[$field_name];
  $form_state = array('submitted' => FALSE);
  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
  );
  $form = form_builder($_POST['form_id'], $form, $form_state);

  // Render the new output.
  $field_form = $form['images'][$nid]['edit_form'][$field_name];
  // We add a div around the new content to receive the ahah effect.
  $field_form[$delta]['#prefix'] = '<div class="ahah-new-content">'. (isset($field_form[$delta]['#prefix']) ? $field_form[$delta]['#prefix'] : '');
  $field_form[$delta]['#suffix'] = (isset($field_form[$delta]['#suffix']) ? $field_form[$delta]['#suffix'] : '') .'</div>';
  // Prevent duplicate wrapper.
  unset($field_form['#prefix'], $field_form['#suffix']);
  $javascript = drupal_add_js(NULL, NULL);
  $output_js = isset($javascript['setting']) ? '<script type="text/javascript">jQuery.extend(Drupal.settings, '. drupal_to_js(call_user_func_array('array_merge_recursive', $javascript['setting'])) .');</script>' : '';
  $output = theme('status_messages') . drupal_render($field_form) . $output_js;
  $GLOBALS['devel_shutdown'] =  FALSE;
  print drupal_to_js(array('status' => TRUE, 'data' => $output));
  exit;
}

function node_gallery_get_image_form_value_items($form) {
  if ($children = element_children($form)) {
    foreach ($children as $child) {
      if ($form[$child]['#type'] == 'value' || $form[$child]['#type'] == 'hidden') {
        $value_forms[$child] = $form[$child];
      }
    }
  }
  elseif ($form['#type'] == 'value' || $form['#type'] == 'hidden') {
    $value_forms[key($form)] = $form;
  }

  return $value_forms;
}

function node_gallery_set_image_form_default_values(&$form, $image, $relationship) {
  global $user;

  if (empty($form['title'])) {
    $form['title'] = array(
      '#type' => 'hidden',
      '#value' => empty($image->title) ? basename($image->{$relationship['imagefield_name']}[0]['filepath']) : $image->title,
    );
  }
  else {
    $form['title']['#default_value'] = empty($form['title']['#default_value']) ? basename($image->{$relationship['imagefield_name']}[0]['filepath']) : $form['title']['#default_value'];
  }

  if (!empty($form['body_field'])) {
    $form['body'] = $form['body_field']['body'];
    $form['body']['#rows'] = 3;
    unset($form['body_field']);
  }

  if (!empty($form['changed']) && empty($form['changed']['#value'])) {
    $form['changed']['#value'] = time();
  }
  if (user_access('administer nodes') && !isset($form['author'])) {
    $form['name'] = array(
      '#type' => 'value',
      '#value' => $image->name ? $image->name : $user->name,
    );
  }
  if (!empty($form['uid']) && empty($form['uid']['#value'])) {
    $form['uid']['#value'] = $user->uid;
  }  
}



/**
 * Form function for the "Sort Images" tab.
 * @param $gallery
 *   A populated node gallery object.
 *
 * @return
 *   Form array.
 */
function node_gallery_sort_images_form($form_state, $gallery, $no_jquery = FALSE) {
  drupal_set_breadcrumb(array(
    l(t('Home'), NULL),
    l(t('Galleries'), 'galleries'),
    l(t('!user\'s Galleries', array('!user' => $gallery->name)), 'galleries/'. $gallery->uid),
    l($gallery->title, 'node/'. $gallery->nid)
  ));

  $form = array();
  $form['#images'] = array();

  $images = node_gallery_get_images($gallery, TRUE, FALSE);
  $image_count = count($images);
  $jquery_ui_integration = FALSE;
  if (module_exists('jquery_ui') && variable_get('node_gallery_jquery_ui_integration', TRUE) && $no_jquery != 'no_jquery') {
    $jquery_ui_integration = TRUE;
  }
  $max_image_count = variable_get('node_gallery_sort_images_max', 750);
  if ($image_count > $max_image_count) {
    drupal_set_message(t('There are too many images in this gallery to display. You can increase the limit by !settings_url. The current limit is !max.', array('!settings' => 'admin/settings/node_gallery/settings', '!max' => $max_image_count)), 'warning');
    return $form;
  }

  // form generation starts here
  $original_sort = array();
  for ($i = 0; $i < $image_count; ++$i) {
    $image = $images[$i];
    $original_sort[] = $image->nid;
    // @todo: We should document how many images will exhaust 128M RAM to manage expectations.  I don't think there's
    // anything we can do here, because it's Drupals' form render process that eats up the heap.
    if (!$jquery_ui_integration) {
      $form['images-sort-'. $i] = array(
        '#type' => 'weight',
        '#delta' => $image_count,
        '#default_value' => $i,
        '#attributes' => array(
          'class' => 'sort'
        ),
      );
    }
    $form['#images'][] = array(
        'nid' => $image->nid,
        'title' => $image->title,
        'created' => $image->created,
        'changed' => $image->changed,
        'status' => $image->status,
        'original_sort' => $i,
        'filepath' => $image->filepath,
        'admin_thumb' => theme('imagecache', 'node-gallery-admin-thumbnail', $image->filepath, '', $image->title, array()),
    );
  }
  $original_sort = implode($original_sort, ',');
  $form['original_sort'] = array(
    '#type' => 'hidden',
    '#value' => $original_sort,
  );
  $form['new_sort'] = array(
    '#type' => 'hidden',
    '#default_value' => $original_sort,
  );
  $form['gid'] = array(
    '#type' => 'value',
    '#value' => $gallery->nid,
  );
  $form['gallery_type'] = array(
    '#type' => 'value',
    '#value' => $gallery->type,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save custom sorting'),
  );
  $form['remove'] = array(
    '#type' => 'submit',
    '#value' => t('Restore default sorting'),
    '#submit' => array('node_gallery_sort_images_form_remove_weights_submit'),
  );
  $form['jquery_ui_integration'] = array(
    '#type' => 'value',
    '#value' => $jquery_ui_integration,
  );
  return $form;
}

/**
 * Page callback for the sort image page, when it retrieves image nids ordered by a specified field.
 *
 * @param object $gallery The node object of a gallery.
 * @param string $op The entry to sort after; one of title, created, changed, filepath, weight or current.
 * @param boolean $reverse If the order should be reversed.
 * @return string JSON encoded array of image nids.
 */
function node_gallery_json_get_sorted_images($gallery, $op = '', $reverse = FALSE) {
  return drupal_json(node_gallery_get_image_nids_sorted_by($gallery, $op, $reverse));
}

/**
 * Page callback for json image object requests.
 *
 * @param object $gallery
 *   The node object of a gallery
 * @param integer $image_nid
 *   The integer that contains the nid of the "current" image.  If not specified, all images are returned.
 * @param integer $to_left
 *   The number of images to the left of the current image to fetch.
 * @param integer $to_right
 *   The number of images to the right of the current image to fetch.
 */
function node_gallery_json_get_images($gallery, $image_nid = NULL, $to_left = 0, $to_right = 0) {
  if (empty($image_nid)) {
    return drupal_json(node_gallery_get_images($gallery));
  }
  if (empty($to_left)) {
    $to_left = 0;
  }
  if (empty($to_right)) {
    $to_right = 0;
  }
  return drupal_json(node_gallery_get_images_slice($gallery, $image_nid, $to_left, $to_right));
}

/**
 * Submit function to reset all image weights in a gallery to 0, effectively removing any custom sort.
 */
function node_gallery_sort_images_form_remove_weights_submit($form, &$form_state) {
  $result = db_query('UPDATE {node_gallery_images} SET weight = 0 WHERE gid = %d', $form_state['values']['gid']);
  if ($result === FALSE) {
    $message = t("There was a problem updating you're images.");
  }
  else {
    $message = t("All image weights have been reset. The global sorting is now active again.");
  }
  node_gallery_clear_gallery_caches($form_state['values']['gid']);
  drupal_set_message($message);
}

function node_gallery_sort_images_form_submit($form, &$form_state) {
  $images = array();
  $gid = $form_state['values']['gid'];
  if ($form_state['values']['jquery_ui_integration']) {
    // jquery ui grid data
    $new_sort = explode(',', $form_state['values']['new_sort']);
    $original_sort = explode(',', $form_state['values']['original_sort']);
    foreach ($new_sort as $position => $nid) {
      $old_position = array_search($nid, $original_sort);
      if ($old_position !== FALSE && $form['#images'][$old_position]['original_sort'] != $position) {
        $images[] = array(
          'nid' => $nid,
          'gid' => $gid,
          'weight' => $position,
        );
      }
    }
  }
  else {
    // tabledrag data
    $i = 0;
    foreach ($form['#images'] as $image) {
      if (isset($form_state['values']['images-sort-'. $i])
          && ($form_state['values']['images-sort-'. $i] != $image['original_sort'])) {
        $images[] = array(
          'nid' => $image['nid'],
          'gid' => $gid,
          'weight' => $form_state['values']['images-sort-'. $i],
        );
      }
      $i++;
    }
  }
  if (!empty($images)) {
    $batch = array(
      'title' => t('Updating image order'),
      'operations' => array(array('node_gallery_batch_sorting_callback', array($images))),
      'finished' => 'node_gallery_batch_sorting_finished',
      'file' => drupal_get_path('module', 'node_gallery') . '/node_gallery.inc',
      'init_message' => t('New image positions are being calculated.'),
      'progress_message' => t('Processing image sorting.'),
      'error_message' => t('Image sorting has encountered an error.'),
    );
    batch_set($batch);
  }
  else {
    drupal_set_message(t("The order of the images was left unchanged."));
  }
}

/**
 * Builds the form used for imagefield_import integration.
 * @param $gallery
 *   A populated gallery node object.
 *
 * @return
 *   A FAPI form array.
 */
function node_gallery_imagefield_import_form($gallery) {
  // Load up the form from imagefield_import
  $form_state = array();
  $form = drupal_retrieve_form('imagefield_import_form', $form_state);

  // Add our gid
  $form['files']['gid']['#tree'] = FALSE;
  $form['files']['gid'] = array('#type' => 'value', '#value' => $gallery->gid);
  return $form;
}
