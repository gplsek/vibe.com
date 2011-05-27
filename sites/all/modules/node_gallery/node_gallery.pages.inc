<?php
// $Id: node_gallery.pages.inc,v 1.11.4.33 2010/05/20 22:29:44 justintime Exp $

/**
 * @file
 * Node gallery pages. 
 *
 */

/**
 * Validate filesystem paths
 * 
 * Take in a string; if transliteration is available use it, otherwise use a simple preg replace to generate a filesystem-friendly path.
 *
 * @param string $path
 * @return string 
 */
function node_gallery_validate_filesystem_path($path = NULL) {
  if (!$path) {
    return NULL;
  }
  // If transliteration is available, let it do the heavy lifting
  if (module_exists('transliteration')) {
    module_load_include('inc', 'transliteration');
    $dest_array = array_filter(explode('/', $path));
    foreach ($dest_array as $key => $dir) {
      $dest_array[$key] = transliteration_clean_filename($dir);
    }
    $validated_path = implode('/', $dest_array);
  }
  else {
    $validated_path = preg_replace('/[^a-zA-Z0-9._\/]/', '', str_replace(' ', '_', $path));
  }
  return $validated_path;
}

function node_gallery_list($account = NULL, $gallery_config = NULL) {
  $uid = isset($account->uid) ? $account->uid : 0;
  $gallery_type = empty($gallery_config) ? '' : $gallery_config['gallery_type'];
  $galleries = node_gallery_get_gallery_list($uid, $gallery_type);
  
  
  $breadcrumbs = array(
    l(t('Home'), NULL),
    l(t('Galleries'), 'galleries'),
  );
  if (isset($account->name)) {
    $breadcrumbs[] = l(t('!user\'s Galleries', array('!user' => $account->name )), 'galleries/'. $account->uid);
  }
  drupal_set_breadcrumb($breadcrumbs);
  if (!empty($galleries)) {
    foreach ($galleries as $gallery) {
      $gallery->config = node_gallery_get_config($gallery->type);
      $items[] = theme('gallery_cover_view', $gallery);
    }
  }
  return theme('gallery_list', $items, $account) . theme('pager', NULL, variable_get('node_gallery_page_number', 20), 0);
}

function node_gallery_upload_form($form_state, $gallery) {
  drupal_set_breadcrumb(array(
    l(t('Home'), NULL),
    l(t('Galleries'), 'galleries'),
    l(t('!user\'s Galleries', array('!user' => $gallery->name)), 'galleries/'. $gallery->uid),
    l($gallery->title, 'node/'. $gallery->nid)
    ));
  
  $config = node_gallery_get_config($gallery->type);
  //bad hack, this is for node_gallery_image item_form, since we use ahah in here;
  drupal_add_tabledrag('upload-attachments', 'order', 'sibling', 'upload-weight');
  drupal_add_js('misc/autocomplete.js');
  
  $form['#attributes']['enctype'] = 'multipart/form-data';
  $form['#node'] = $gallery;
  //just a mark for submit handler.
  $form['is_upload'] = array(
    '#type' => 'value',
    '#value' => TRUE,
  );
  
  $form['upload_wrapper'] = array(
    '#prefix' => '<div id="gallery-upload-wrapper">',
    '#suffix' => '</div>',
  );
  
  $upload_number = is_numeric($config['number_uploads']) ? $config['number_uploads'] : 5;
  for ($i = 1; $i <= $upload_number; $i++) {
    $form['upload_wrapper']['uploads-'. $i] = array(
      '#type' => 'file',
      '#title' => t('Please select an image'),
    );
  }
  
  $form['upload_wrapper']['description'] = array(
    '#value' => node_gallery_upload_limits($config),
  );
  $form['upload_wrapper']['next'] = array(
    '#type' => 'submit',
    '#value' => t('Submit Images'),
    '#weight' => 20,
    '#ahah' => array(
      'path' => 'node_gallery/upload/js',
      'wrapper' => 'gallery-upload-wrapper',
      'progress' => array('type' => 'bar', 'message' => t('Please wait...')),
    ),
  );
  
  return $form;
}

/*function node_gallery_upload_form_validate($form, $form_state) {
  //bad hacks, since we use ahah, we have to validate the upload image nodes here.
  if (!empty($form_state['values']['files'])) {
    foreach ($form_state['values']['files'] as $key => $image_form_state) {
      node_validate($image_form_state['edit_form'], $form['files'][$key]['edit_form']);
    }    
  }
}*/

function node_gallery_upload_limits($config) {
  global $user;
  
  $limits = node_gallery_upload_file_limits($user, $config);
  // This check is not true for user 1
  if ($user->uid != '1') {
    $items[] = t('Your total storage space is %total and you have used %used',
     array('%total' => format_size($limits['user_size']), '%used' => format_size(file_space_used($user->uid))));
  }
  if ($limits['resolution']) {
    $items[] = t('Images are larger than %resolution will be resized.', array('%resolution' => $limits['resolution']));
  }
  $items[] = t('The maximum upload size per submit is %filesize.', array('%filesize' => format_size($limits['file_size'])));
  $items[] = t('Only images with the following extensions may be uploaded: %extensions.', array('%extensions' => $limits['extensions']));
  
  return theme('item_list', $items, t('notes'), 'ul', array('class' => 'upload-notes'));
}

function node_gallery_upload_js() {
  $cached_form_state = array();
  $files = array();
  
  // Load the form from the Form API cache.
  if (!($cached_form = form_get_cache($_POST['form_build_id'], $cached_form_state)) || !isset($cached_form['#node']) || !isset($cached_form['upload_wrapper'])) {
    form_set_error('form_token', t('The upload failed, please try again.'));
    $output = theme('status_messages');
    print drupal_to_js(array('status' => TRUE, 'data' => $output));
    exit();
  }
  
  $form_state = array('values' => $_POST);
  
  //this is upload form, we don't want to show the node's existing images;
  unset($cached_form['#node']->images);
  // Handle new uploads, and merge tmp files into node-files.
  node_gallery_upload_images($cached_form, $form_state);
  
  // Exit if no images were successfully uploaded
  if (empty($form_state['values']['files'])) {
    if (drupal_get_messages('error', FALSE)) {
      print drupal_to_js(array('status' => TRUE, 'data' => theme('status_messages') . drupal_get_form('node_gallery_upload_form')));
    }
    exit;
  }
  
  $form = node_gallery_edit_images_form($form_state, $cached_form['#node']);
  $cached_form = array_merge($cached_form, $form);
  
  form_set_cache($_POST['form_build_id'], $cached_form, $cached_form_state);
  // Render the form for output.
  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
    '#tree' => FALSE,
    '#parents' => array(),
  );
  //drupal_alter('form', $form, array(), 'upload_js');
  $form_state = array('submitted' => FALSE);
  $form = form_builder('node_gallery_upload_js', $form, $form_state);
  
  print drupal_to_js(array('status' => TRUE, 'data' => theme('status_messages') . drupal_render($form)));
  exit;
}

function node_gallery_upload_file_limits($user, $config) {
  $file_limit = $config['upload_limits']['general']['file_max_size'];
  $user_limit = $config['upload_limits']['general']['user_max_size'];
  $default_number = $config['upload_limits']['general']['user_max_number'];
  $all_extensions = explode(' ', $config['upload_limits']['general']['file_extension']);
  $resolution = $config['upload_limits']['general']['file_resolution'];
  foreach ($user->roles as $rid => $name) {
    if (empty($config['upload_limits']['role_'. $rid])) {
      continue;
    }
    /*$extensions = $gconfig['upload_limits']['role_'. $rid]['file_extension'];
    $user_extensions = explode(' ', $extensions);
    $all_extensions = !empty($user_extensions) ? $user_extensions : (!empty($all_extensions) ? $all_extensions : array('jpg', 'png', 'gif', 'jpeg'));*/

    // A zero value indicates no limit, take the least restrictive limit.
    $file_size = $config['upload_limits']['role_'. $rid]['file_max_size'];
    $file_limit = !empty($file_size) ? $file_size : (!empty($file_limit) ? $file_limit : 0);

    $user_size = $config['upload_limits']['role_'. $rid]['user_max_size'];
    $user_limit = !empty($user_size) ? $user_size : (!empty($user_limit) ? $user_limit : 0);
    
    $user_number = $config['upload_limits']['role_'. $rid]['user_max_number'];
    $default_number = !empty($user_number) ? $user_number : (!empty($default_number) ? $default_number : 0);
  }
  $all_extensions = implode(' ', array_unique($all_extensions));
  return array(
    'extensions' => $all_extensions,
    'file_size' => $file_limit * 1024 * 1024,
    'user_size' => $user_limit * 1024 * 1024,
    'resolution' => $resolution,
    'max_number' => $default_number,
  );
}

function node_gallery_upload_images(&$form, &$form_state) {
  global $user;

  $gallery_config = node_gallery_get_config($form['#node']->type);
  $limits = node_gallery_upload_file_limits($user, $gallery_config);
  $validators = array(
    'file_validate_extensions' => array($limits['extensions']),
    'file_validate_image_resolution' => array($limits['resolution']),
    'file_validate_size' => array($limits['file_size'], $limits['user_size']),
  );

  // We need to keep track of the number of files being currently updated so
  // that we can avoid http://drupal.org/node/601186#comment-2951536
  $temp_file_count = 0;
  // Save new file uploads.
  if (($user->uid != 1 || user_access('upload files'))) {
    $directory = node_gallery_check_directory($form['#node']);
    foreach ($_FILES['files']['name'] as $id => $name) {
      if (!empty($name)) {
        // An additional check to strip all invalid characters
        $_FILES['files']['name'][$id] = node_gallery_validate_filesystem_path($_FILES['files']['name'][$id]);
        
        // Make sure we have a filename after scripting bad characters
        if (strlen($_FILES['files']['name'][$id]) < 5) {
          $_FILES['files']['name'][$id] = 'file-'. $_FILES['files']['name'][$id];
        }
        
        $temp_file_count++;
        $validators['file_validate_number'] = array($limits['max_number'], $temp_file_count);
        $file = file_save_upload($id, $validators, $directory);

        if ($file != 0) {
          $file->description = $file->filename;
          $file->weight = 0;
          $file->gid = $form['#node']->nid;
          $form['#node']->images[$file->fid] = $file;
          $form_state['values']['files'][$file->fid] = (array) $file;
        }
      }
    }
  }

  // Order the form according to the set file weight values.
  if (!empty($form_state['values']['files'])) {
    $microweight = 0.001;
    foreach ($form_state['values']['files'] as $fid => $file) {
      if (is_numeric($fid)) {
        $form_state['values']['files'][$fid]['#weight'] = $file['weight'] + $microweight;
        $microweight += 0.001;
      }
    }
    uasort($form_state['values']['files'], 'element_sort');
  }
}

function node_gallery_check_directory($gallery) {
  global $user;
  $gallery_directory = strtr($gallery->config['gallery_directory'], 
     array('%uid' => $user->uid, '%username' => $user->name, '%gid' => $gallery->nid, '%gallery_name' => $gallery->title));
     
  $gallery_directory = node_gallery_validate_filesystem_path($gallery_directory);
  
  $directory = rtrim(file_directory_path() .'/'. $gallery_directory, '/\\');
  //recursive mkdir;
  if (!is_dir($directory)) {
    mkdir($directory, 0777, TRUE);
  }
  file_check_directory($directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
  
  return $directory;
}

function node_gallery_edit_images_form($form_state, $gallery) {
  global $user;
  
  drupal_set_breadcrumb(array(
    l(t('Home'), NULL),
    l(t('Galleries'), 'galleries'),
    l(t('!user\'s Galleries', array('!user' => $gallery->name)), 'galleries/'. $gallery->uid),
    l($gallery->title, 'node/'. $gallery->nid)
    ));
   
  $form = array(
    '#theme' => 'gallery_edit_images_form',
    '#cache' => TRUE,
  );
  //if this is new upload, then fid is file id;
  //if this is edit images, then fid is node id;
  //but fid is just an identifier here, so needn't care it.
  if (!empty($gallery->images) && is_array($gallery->images)) {
    foreach ($gallery->images as $fid => $f) {
      $options[$fid] = '';
      if ($f->is_cover) {
        $cover_fid = $fid;
      }
    }
  }
  $form['is_cover'] = array(
    '#type' => 'radios',
    '#default_value' => $cover_fid,
    '#options' => $options,
  );
  //we need this to verify if image was updated later.
  $form['#gallery'] = $gallery;
  
  //for display in theme;
  $form['#thumb_imagecache'] = $gallery->config['image_size']['thumbnail'];
  
  if (user_access('administer nodes') || user_access('edit any '. $gallery->type .' content') || user_access('administer node gallery')) {    
    $gallery_list = node_gallery_get_list($gallery->type);
  }
  else {
    $gallery_list = node_gallery_get_list($gallery->type, $user->uid);
  }

  if (!empty($gallery->images) && is_array($gallery->images)) {
    $form['files']['#tree'] = TRUE;
    
    foreach ($gallery->images as $key => $file) {
      $file = (object)$file;
      
      $form['files'][$key]['remove'] = array('#type' => 'checkbox', '#default_value' => $file->remove);
      $form['files'][$key]['weight'] = array('#type' => 'weight', '#delta' => count($gallery->images), '#default_value' => $file->weight);
      $form['files'][$key]['filename'] = array('#type' => 'value',  '#value' => $file->filename);
      $form['files'][$key]['filepath'] = array('#type' => 'value',  '#value' => $file->filepath);
      $form['files'][$key]['filemime'] = array('#type' => 'value',  '#value' => $file->filemime);
      $form['files'][$key]['filesize'] = array('#type' => 'value',  '#value' => $file->filesize);
      $form['files'][$key]['fid'] = array('#type' => 'value',  '#value' => $file->fid);
      $form['files'][$key]['gid'] = array('#type' => 'select', '#title' => 'Gallery', '#default_value' => $gallery->nid, '#options' => $gallery_list);
      
      //get the required fields for image node edit and saving.
      $file->type = empty($file->type) ? $gallery->config['image_type'] : $file->type;
      $file->gid = empty($file->gid) ? $gallery->nid : $file->gid;
      $form['files'][$key]['edit_form'] = node_gallery_image_item_edit_form($form_state['values']['files'][$key]['edit_form'], $file, $gallery->config);
      //Some CCK widgets need to have #field_info populated
      if (!isset($form['#field_info'])){
        $form['#field_info'] = $form['files'][$key]['edit_form']['#field_info'];
      };
    }
  }
  
  //pager if it exists;
  /* Using the pager appears to break the order of images
  $form['pager'] = array(
    '#value' => theme('pager', NULL, variable_get('node_images_page_number', 20), NODE_GALLERY_IMAGE_PAGER_ELEMENT),
  );
  */
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 20,
    //need this because this form is used in image upload ahah also.
    '#submit' => array('node_gallery_images_edit_submit'),
    '#validate' => array('node_gallery_images_edit_validate'),
  );
  
  return $form;
}

/**
 * This is exactly like the edit images form except it removes the ability
 * to edit the image content and removes pagination
 */
function node_gallery_sort_images_form($form_state, $gallery) {
  global $user;
  
  drupal_set_breadcrumb(array(
    l(t('Home'), NULL),
    l(t('Galleries'), 'galleries'),
    l(t('!user\'s Galleries', array('!user' => $gallery->name)), 'galleries/'. $gallery->uid),
    l($gallery->title, 'node/'. $gallery->nid)
    ));
   
  $form = array(
    '#theme' => 'gallery_sort_images_form',
    '#cache' => TRUE,
  );
  //if this is new upload, then fid is file id;
  //if this is edit images, then fid is node id;
  //but fid is just an identifier here, so needn't care it.
  if (!empty($gallery->images) && is_array($gallery->images)) {
    foreach ($gallery->images as $fid => $f) {
      $options[$fid] = '';
      if ($f->is_cover) {
        $cover_fid = $fid;
      }
    }
  }
  $form['is_cover'] = array(
    '#type' => 'radios',
    '#default_value' => $cover_fid,
    '#options' => $options,
  );
  //we need this to verify if image was updated later.
  $form['#gallery'] = $gallery;
  
  //for display in theme;
  $form['#thumb_imagecache'] = $gallery->config['image_size']['thumbnail'];
  
  if (!empty($gallery->images) && is_array($gallery->images)) {
    $form['files']['#tree'] = TRUE;
    
    foreach ($gallery->images as $key => $file) {
      $file = (object)$file;
      
      $form['files'][$key]['remove'] = array('#type' => 'checkbox', '#default_value' => $file->remove);
      $form['files'][$key]['weight'] = array('#type' => 'weight', '#delta' => count($gallery->images), '#default_value' => $file->weight);
      $form['files'][$key]['filename'] = array('#type' => 'value',  '#value' => $file->filename);
      $form['files'][$key]['filepath'] = array('#type' => 'value',  '#value' => $file->filepath);
      $form['files'][$key]['filemime'] = array('#type' => 'value',  '#value' => $file->filemime);
      $form['files'][$key]['filesize'] = array('#type' => 'value',  '#value' => $file->filesize);
      $form['files'][$key]['fid'] = array('#type' => 'value',  '#value' => $file->fid);
      $form['files'][$key]['gid'] = array('#type' => 'value', '#default_value' => $gallery->nid);
      
      //get the required fields for image node edit and saving.
      $file->type = empty($file->type) ? $gallery->config['image_type'] : $file->type;
      $file->gid = empty($file->gid) ? $gallery->nid : $file->gid;
      $form['files'][$key]['edit_form'] = node_gallery_image_item_edit_form($form_state['values']['files'][$key]['edit_form'], $file, $gallery->config);
      
      foreach ($form['files'][$key]['edit_form'] as $item_key => $item) {
        unset($form['files'][$key]['edit_form'][$item_key]['#title']);
        $form['files'][$key]['edit_form'][$item_key]['#type'] = 'value';
      }
      unset($item_key, $item);
    }
  }
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 20,
    //need this because this form is used in image upload ahah also.
    '#submit' => array('node_gallery_images_edit_submit'),
    '#validate' => array('node_gallery_images_edit_validate'),
  );
  
  return $form;
}

function node_gallery_image_item_edit_form($form_values, $image, $config) {
  
  module_load_include('inc', 'node', 'node.pages');
  $form_state = array('values' => $form_values);
  $display_fields = $config['display_fields'];
  $form = drupal_retrieve_form($image->type .'_node_form', $form_state, $image);
  drupal_prepare_form($image->type ."_node_form", $form, $form_state);
  $item_form = array();
  $display_fields['#field_info'] = '#field_info'; //this should be merged with parent form to optionwidget to work
  
  foreach (array_values($display_fields) as $field_name) {
    if (!empty($field_name)) {
      //$item_form[$field_name] = (array)get_image_form_item_recursive($form, $field_name);
      $item_form[$field_name] = $form[$field_name]; 
    }
  }
  $item_form += (array)get_image_form_value_items($form);
  set_image_form_default_values($item_form, $image, $config);
  
  return $item_form;
}

function get_image_form_item_recursive($form, $field) {
  if ($form[$field]) {
    return $form[$field];
  }
  elseif ($children = element_children($form)) {
    foreach ($children as $child) {
      get_image_form_item_recursive($form[$child], $field);
    }
  }
  return ;
}

function get_image_form_value_items($form) {
  if ($children = element_children($form)) {
    foreach ($children as $child) {
      //todo: didn't recursive;
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

function set_image_form_default_values(&$form, $image, $config) {
  global $user;

  if (empty($form['title'])) {
    $form['title'] = array(
      '#type' => 'hidden',
      '#value' => $image->filename,
    );
  }
  else {
    $form['title']['#default_value'] = empty($form['title']['#default_value']) ? $image->filename : $form['title']['#default_value'];
  }
  
  if (!empty($form['body_field'])) {
    /*$tmp_item = $form['body_field']['body'];
    unset($form['body_field']);
    $form['body_field'] = $tmp_item;
    $form['body_field']['#rows'] = 3;
    $form['body_field']['#default_value'] = empty($form['body']['#default_value']) ? $image->filename : $form['body']['#default_value'];*/
    $form['body'] = $form['body_field']['body'];
    $form['body']['#rows'] = 3;
    unset($form['body_field']);
  }
  
  if (!empty($form['changed']) && empty($form['changed']['#value'])) {
    $form['changed']['#value'] = time();
  }
  /* This allowed admins to overwrite the username when editing an image.
   * this feature was disabled as it was unexpected behaviour #550994 */
  if (user_access('administer nodes')) {
    $form['name'] = array(
      '#type' => 'value',
      '#value' => $image->name ? $image->name : $user->name,
    );
  }
  /* if (empty($form['name']) || empty($form['name']['#value'])) {
    $form['name'] = array(
      '#type' => 'value',
      '#value' => $user->name,
    );
  } */
  if (!empty($form['uid']) && empty($form['uid']['#value'])) {
    $form['uid']['#value'] = $user->uid;
  }
  $image_comment = variable_get('comment_'. $config['image_type'], COMMENT_NODE_READ_WRITE);
  $gallery_comment = variable_get('comment_'. $config['gallery_type'], COMMENT_NODE_READ_WRITE);
  $form['comment'] = array(
    '#type' => 'value',
    '#value' => $config['image_comment'] == 'image' ? $image_comment : $gallery_comment,
  );
}

function node_gallery_images_edit_validate($form, &$form_state) {
  if (!empty($form_state['values']['files'])) {
    foreach ($form_state['values']['files'] as $key => $image_form_state) {
      node_validate($image_form_state['edit_form'], $form['files'][$key]['edit_form']);
    }    
  }
}

function node_gallery_images_edit_submit(&$form, &$form_state) {
  
  $ng_table = drupal_get_schema_unprocessed('node_gallery', 'node_galleries');
  foreach ($ng_table['fields'] as $k => $f) {
    $compare_fields[] = $k;
  }
  foreach ($form['#gallery']->config['display_fields'] as $k => $f) {
    if ($f) {
      $compare_fields[] = ($k == 'body_field') ? 'body' : $k;
    }
  }
  
  foreach ($form_state['values']['files'] as $fid => $form_values) {
    $image_node = $form_values['edit_form'];
    unset($form_values['edit_form']);
    $image_node = (object)array_merge($image_node, $form_values);
    if ($form_state['values']['is_cover'] == $fid) {
      $image_node->is_cover = 1;
    }
    else {
      $image_node->is_cover = 0;
    }
    
    if ($form_values['remove']) {
      $op_images['delete'][] = $image_node;
    }
    else {
      //modify image;
      if (node_gallery_images_check_update($form['#gallery']->images[$fid], $image_node, $compare_fields)) {
        $op_images['update'][] = $image_node;
      }
      //new update image;
      elseif (empty($image_node->nid)) {
        $op_images['update'][] = $image_node;
      }
    }
  }
  
  $delete_total = count($op_images['delete']);
  if (!empty($delete_total)) {
    $operations[] = array('image_delete_process', array($op_images['delete']));
  }
  $update_total = count($op_images['update']);
  if (!empty($update_total)) {
    $operations[] = array('image_upload_process', array($op_images['update'], $form_state['values']['is_upload']));
  }
  if (!empty($operations)) {
    $batch = array(
      'operations' => $operations,
      'finished' => 'image_process_finished',
      'title' => empty($form_state['values']['is_upload']) ? t("Processing Images Edit") : t('Processing Images Upload.'),
      'init_message' => empty($form_state['values']['is_upload']) ? t("Images update is starting.") : t('Images upload is starting.'),
      //'progress_message' => t('Processed @current out of @total.'),
      'error_message' => empty($form_state['values']['is_upload']) ? t('Images edit has encountered an error.') : t('Images upload has encountered an error.'),
    );
    
    batch_set($batch);
    //hack for batch process in form, see line 426 in form.inc;
    $tmp = $form_state['values']['form_build_id'];
    $form_state = array();
    $form_state['values']['form_build_id'] = $tmp;
    $form = array();
  }
  $form_state['redirect'] = 'node/'. $image_node->gid;
}

/**
 * Check if we need to update this image;
 *
 * @param unknown_type $old_image
 * @param unknown_type $new_image
 * @param unknown_type $compare_fields
 * @return unknown
 */
function node_gallery_images_check_update($old_image, $new_image, $compare_fields) {
  
  foreach ($compare_fields as $f) {
    //a hack for cck field validate;
    if (module_exists('content')) {
      if (is_array($new_image->{$f})) {
        foreach ($new_image->{$f} as &$ff) {
          if (is_array($ff)) {
            unset($ff['_error_element']);
          }
        }
      }
    }
    if ($new_image->{$f} != $old_image->{$f}) {
      return TRUE;
    }
  }
  return FALSE;
}

function node_gallery_manage_form($form_state, $gallery) {
  if (empty($gallery->images)) {
    return t('This gallery has no images presently, please upload some.');
  }
  $form['#theme'] = array(
    '#theme' => 'gallery_manage_images_form',
    '#cache' => TRUE,
  );
  $form['#id'] = 'manage-images-form';
  $form['#gallery'] = $gallery;
  $config = $gallery->config;
  $form['images'] = array();
  foreach ($gallery->images as $image) {
    $subform['image'. $image->nid] = array(
      '#prefix' => '<div class="manage-image-'. $image->nid .'">',
      '#suffix' => '</div>',
    );
    $subform['image'. $image->nid]['check'] = array(
      '#type' => 'checkbox',
    );
    $subform['image'. $image->nid]['thumb'] = array(
      '#value' => theme('image_view', $config['image_size']['thumbnail'], $image),
    );
    $form['images'] = array_merge(array(), $subform);
  }
  
  return $form;
}

function file_validate_number($file, $max_number, $temp_count) {
  global $user;
  $errors = array();
  
  if ($user->uid != 1) {
    $current_number = db_result(db_query("SELECT COUNT(*) FROM {files} WHERE uid = %d and status = %d", $user->uid, FILE_STATUS_PERMANENT));
    if ($temp_count > 0) {
      $current_number += $temp_count;
    }
    if ($current_number > $max_number && $max_number != '0') {
      $errors[] = t("You can only upload a maximum of %num images.", array('%num' => $max_number));
    }
  }
  return $errors;
}