<?php
// $Id: node_gallery.admin.inc,v 1.10.4.17 2010/05/08 14:02:30 justintime Exp $

/**
 * @file Node gallery admin page file
 *
 * @return unknown
 */

function node_gallery_config_list() {
  $ng_configs = node_gallery_get_config();
  
  if (!empty($ng_configs)) {
    $headers = array(t('Gallery Name'), t('Gallery type'), t('Image type'), t('Operations'));
    foreach ($ng_configs as $key => $config) {
      $rows[] = array($config['name'], $config['gallery_type'], $config['image_type'], 
      theme('links', node_gallery_config_operations($key)));
    }
    return theme('table', $headers, $rows, array('class' => 'node-gallery-config-list'));
  }
  else {
    return t("There's no gallery type now.");
  }
}

function node_gallery_settings_form() {
  $form['node_gallery_page_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter the Number of galleries displayed on each main gallery list page'),
    '#default_value' => variable_get('node_gallery_page_number', 20),
    '#description' => t('If this number is exceed, a pager will be displayed below the main gallery list.'),
  );
  $form['node_images_page_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter the Number of images displayed on each gallery thumbnail page'),
    '#default_value' => variable_get('node_images_page_number', 20),
    '#description' => t('If this number is exceeded, a pager will be displayed below each corresponding gallery thumbnail page.'),
  );
  $form['node_images_page_fragment'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Named Anchors on Image "Previous" and "Next" links?'),
    '#default_value' => variable_get('node_images_page_fragment', FALSE),
    '#description' => t('When checked, the "Previous" and "Next" links will jump to the top of the image.  You might want to leave this unchecked if you have ads at the top of your image pages.'),
  );
  return system_settings_form($form);
}

function node_gallery_config_operations($type) {
  $items[] = array('title' => t('Edit'), 'href' => 'admin/settings/node_gallery/edit/'. $type);
  $items[] = array('title' => t('Delete'), 'href' => 'admin/settings/node_gallery/delete/'. $type);
  
  return $items;
}

function node_gallery_config_form($form_state, $gallery_config = NULL) {
  static $node_types, $image_types, $imagecaches;
  
  if (empty($imagecaches)) {
    foreach (imagecache_presets() as $id => $imagecache) {
      $imagecaches[$imagecache['presetname']] = $imagecache['presetname'];
    }
  }
  if (empty($imagecaches)) {
    form_set_error('Node gallery config', t('There are no imagecache presets. <a href="@imagecache">You must create at least one preset</a>.', array('@imagecache' => url('admin/build/imagecache/add'))));
  }
  
  $gallery_config = array_merge((array)$gallery_config, (array)$form_state['storage']);
  $gallery_config = array_merge($gallery_config, (array)$form_state['values']['config']);
  //for other module like node_gallery_lightbox2 use.
  $form['#gallery_config'] = $gallery_config;
  
  $step = empty($form_state['values']['step']) ? 1 : 2;
  $form['step'] = array(
    '#type' => 'hidden',
    '#value' => $step,
  );
  
  $form['config'] = array(
    '#tree' => TRUE,
  );
  
  if ($step == 1) {
    if (empty($node_types)) {
      foreach (node_get_types() as $node_type) {
        $node_types[$node_type->type] = $node_type->name;
      }
    }
    $form['config']['gallery_type'] = array(
      '#type' => 'radios',
      '#title' => t('Gallery Type'),
      '#options' => $node_types,
      '#description' => t('Select which content type should be used as the gallery type.'),
      '#default_value' => $gallery_config['gallery_type'],
      '#required' => TRUE,
    );
    $form['config']['image_type'] = array(
      '#type' => 'radios',
      '#title' => t('Image Type'),
      '#options' => $node_types,
      '#description' => t('Select which content type should be used as the image type.'),
      '#default_value' => $gallery_config['image_type'],
      '#required' => TRUE,
    );
    $form['next'] = array(
      '#type' => 'submit',
      '#value' => t('Next'),
      '#submit' => array('node_gallery_config_form_next_submit'),
    );
  }
  else {
    drupal_add_js(drupal_get_path('module', 'node_gallery') .'/node_gallery.admin.js', 'module');
    
    /*cck support*/
    if (module_exists('content')) {
      content_clear_type_cache();
      $image_type = content_types($form_state['storage']['image_type']);
      //non-cck fields
      foreach ($image_type['extra'] as $name => $field) {
        if ($name != 'menu' && $name != 'attachments') {
          $image_type_fields[$name] = $field['label'];
        }
      }
      //cck fields
      foreach ($image_type['fields'] as $name => $field) {
        $image_type_fields[$name] = $field['widget']['label'];
      }
    }
    else {
      $node_fields = node_gallery_get_type_fields($form_state['storage']['image_type']);
      foreach ($node_fields as $name => $field) {
        $image_type_fields[$name] = $field['label'];
      }
    }
    
    $form['config']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Gallery Type Name'),
      '#description' => t('The human-readable name of this content type. This text will be displayed as part of the list on the create content page. It is recommended that this name begin with a capital letter and contain only letters, numbers, and <strong>spaces</strong>.'),
      '#maxlength' => 32,
      '#required' => TRUE,
      '#default_value' => $gallery_config['name']
    );
    
    $form['config']['gallery_directory'] = array(
      '#type' => 'textfield',
      '#title' => t('Gallery Directory'),
      '#description' => t('Specify a subdirectory of the file system where the images for the galleries will be saved. The document root is \'files\'.
       Use %uid for user_id, %username for username, %gid for gallery_id, %gallery_name for gallery_name.'),
      '#default_value' => $gallery_config['gallery_directory']
    );
    $form['config']['default_cover'] = array(
      '#type' => 'textfield',
      '#title' => t('Default Cover Image'),
      '#description' => t('Specify a default cover image to show when there are no images in the gallery. The path begins with your drupal root. <strong>Do not include the leading slash /.</strong>'),
      '#default_value' => $gallery_config['default_cover'],
    );
    $form['config']['number_uploads'] = array(
      '#type' => 'select',
      '#title' => t('Number of Uploads'),
      '#options' => drupal_map_assoc(range(1, 20)),
      '#multiple' => FALSE,
      '#description' => t('This allows you to specify how many image upload fields appear on the image upload page. By default, only 5 fields are provided. We recommend you do not have more than 5 fields if you are on a shared host or have limited resources. If you increase this to more than 5, we highly recommend you test this by uploading lots of large images prior to putting your site into production.'),
      '#default_value' => empty($gallery_config['number_uploads']) ? '5' : $gallery_config['number_uploads'],
    );
    
    $form['config']['display_fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Fields'),
      '#options' => $image_type_fields,
      '#description' => t('Specify which fields should be displayed.'),
      '#required' => TRUE,
      '#default_value' => empty($gallery_config['display_fields']) ? array() : $gallery_config['display_fields'],
    );
    $form['config']['content_display'] = array(
      '#type' => 'radios',
      '#title' => t('Choose the content you want to display'),
      '#options' => array('gallery' => t('Gallery'), 'image' => t('Images')),
      '#description' => t("If you select 'Gallery', the body text of the gallery node will be used. If you select 'Images', the body text of the image node will be used. This allows you choose if you want the text content with each image to be different or the same on all images."),
      '#default_value' => empty($gallery_config['content_display']) ? 'image' : $gallery_config['content_display'],
    );
    $form['config']['image_comment'] = array(
      '#type' => 'radios',
      '#title' => t('Image Comment Setting'),
      '#options' => array(
        'image' => t('Use the image type comment settings.'),
        'gallery' => t('Use the gallery type comment settings.'),
      ),
      '#description' => t('Select how to apply comments to the images.'),
      '#default_value' => isset($gallery_config['image_comment']) ? $gallery_config['image_comment'] : 'image',
    );
    
    //gallery image size setting
    $form['config']['image_size'] = array(
      '#type' => 'fieldset',
      '#title' => t('Image Sizes'),
      '#description' => t('Specify the imagecache preset to use for each type of gallery image.'),
      '#tree' => TRUE,
    );
    $form['config']['image_size']['cover'] = array(
      '#type' => 'select',
      '#title' => t('Cover'),
      '#options' => $imagecaches,
      '#default_value' => $gallery_config['image_size']['cover'],
    );
    $form['config']['image_size']['thumbnail'] = array(
      '#type' => 'select',
      '#title' => t('Thumbnail'),
      '#options' => $imagecaches,
      '#default_value' => $gallery_config['image_size']['thumbnail'],
    );
    $form['config']['image_size']['preview'] = array(
      '#type' => 'select',
      '#title' => t('Preview: This will be the Image Display (A Preview of the Full-Size Image)'),
      '#options' => $imagecaches,
      '#default_value' => $gallery_config['image_size']['preview'],
    );
    
    /*$extra_options = array();
    $extra_description = '';
    if ($lightbox2) {
      $extra_options['lightbox2'] = t('Enable as Lightbox2 Popup');
    }
    else {
      $extra_description = '<br />'. t('If you install and enable the <a href="http://drupal.org/project/lightbox2">Lightbox2</a> module, you can use Lightbox to display a larger version of the image.');
    }*/
    
    $form['config']['original'] = array(
      '#type' => 'fieldset',
      '#title' => t('Original Image Display'),
      '#description' => t('Specify how to display the original image or disable this feature.'),
      '#tree' => TRUE,
    );
    $form['config']['original']['view_original'] = array(
      '#type' => 'radios',
      '#title' => t('View of The Original Image'),
      '#attributes' => array('class' => 'view-original-radios'),
      '#options' => array(
        '0' => t('Disabled'),
        'default' => t('Enable as a link over the displayed image'),
        'text' => t('Enable as text link below the displayed image'),
      ),
      '#description' => t('Specify whether to allow viewing the original image and how to do so.'),
      '#default_value' => isset($gallery_config['original']['view_original']) ? $gallery_config['original']['view_original'] : 0,
    );
    
    $form['config']['original']['view_original_text'] = array(
      '#type' => 'textfield',
      '#title' => t('View Original Image Text Link'),
      '#description' => t('If you have selected the "Enable Text Link Below the Displayed Image" option, enter the text for the link.'),
      '#default_value' => empty($gallery_config['original']['view_original_text']) ? t('Download the Original Image') : $gallery_config['original']['view_original_text'],
      '#prefix' => '<div class="view-original-text-value">', 
      '#suffix' => '</div>'
    );
    
    //gallery teaser image display setting
    $form['config']['teaser'] = array(
      '#type' => 'fieldset',
      '#title' => t('Teaser Setting'),
      '#description' => t('Specify how to display the gallery when viewing the gallery node as a teaser.'),
    );
    $form['config']['teaser']['gallery_display_type'] = array(
      '#type' => 'radios',
      '#title' => t('Display Type'),
      '#attributes' => array('class' => 'teaser-display-type-radios'),
      '#options' => array('cover' => t('Cover'), 'thumbnails' => t('Thumbnails')),
      '#default_value' => empty($gallery_config['teaser']['gallery_display_type']) ? 'cover' : $gallery_config['teaser']['gallery_display_type'],
    );
    $form['config']['teaser']['thumbnails_num'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of Thumbnails to Display'),
      '#description' => t('If thumbnails are selected as the display type above, enter the number of thumbnails to show in the teaser.'),
      '#default_value' => $gallery_config['teaser']['thumbnails_num'],
    );
    $form['config']['teaser']['image'] = array(
      '#type' => 'select',
      '#title' => t('Image Size for Display Type'),
      '#options' => $imagecaches,
      '#description' => t('Select the Imagecache preset that will be used for the display type selected above.'),
      '#default_value' => $gallery_config['teaser']['image'],
    );
    
    /**
     * Gallery Landing page
     */
    $form['config']['gallery'] = array(
      '#type' => 'fieldset',
      '#title' => t('Gallery Landing Page Setting'),
      '#description' => t('Specify how to display the gallery landing page images when viewing the node as a teaser'),
    );
    
    $form['config']['gallery']['gallery_display_type'] = array(
      '#type' => 'radios',
      '#title' => t('Gallery Display Type'),
      '#attributes' => array('class' => 'gallery-display-type-radios'),
      '#options' => array('thumbnails' => t('Thumbnails'), 'cover' => t('Cover')),
      '#default_value' => empty($gallery_config['gallery']['gallery_display_type']) ? 'thumbnails' : $gallery_config['gallery']['gallery_display_type'],
    );
    
    //gallery upload limitation settings;
    node_gallery_upload_limits_form($form, $gallery_config['upload_limits'], $gallery_config['image_type']);
    
    $form['previous'] = array(
      '#type' => 'submit',
      '#value' => t('Previous'),
      '#submit' => array('node_gallery_config_form_prev_submit'),
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );
    
  }
  
  return $form;
}

function node_gallery_config_form_validate($form, &$form_state) {
  if ($form_state['values']['step'] == 1) {
    if ($form_state['values']['config']['gallery_type'] == $form_state['values']['config']['image_type']) {
      unset($form_state['values']['step']);
      form_set_error('Type', t('The gallery type and image type can\'t be the same.'));
    }
    /*$gallery_types = node_gallery_get_types();
    if (in_array($form_state['values']['config']['gallery_type'], (array)$gallery_types)) {
      unset($form_state['values']['step']);
      form_set_error('Type', t('The gallery type selected is already being used, please choose a different one.'));
    }*/
  }
  if ($form_state['values']['step'] == 2) {
    node_gallery_upload_limits_form_validate($form, $form_state['values']['data']['upload_limits']);
  }
}

function node_gallery_config_form_next_submit($form, &$form_state) {
  $form_state['storage'] = array_merge((array)$form_state['storage'], (array)$form_state['values']['config']);
}

function node_gallery_config_form_prev_submit($form, &$form_state) {
  $form_state['storage'] = array_merge((array)$form_state['storage'], (array)$form_state['values']['config']);
  unset($form_state['values']['step']);  
}

function node_gallery_config_form_submit($form, &$form_state) {
  $submit_values = array_merge((array)$form_state['storage'], (array)$form_state['values']['config']);
  
  $node_gallery_id = 'node_gallery_'. $submit_values['gallery_type'];
  variable_set($node_gallery_id, $submit_values);
  
  unset($form_state['storage']);
  $form_state['rebuild'] = FALSE;
  $form_state['redirect'] = 'admin/settings/node_gallery';
}

/**
 * copy from cck module for no cck dependency.
 *
 * @param unknown_type $type_name
 * @return unknown
 */
function node_gallery_get_type_fields($type_name) {
  $type = node_get_types('type', $type_name);
  $extra = array();

  if ($type->has_title) {
    $extra['title'] = array(
      'label' => $type->title_label,
      'description' => t('Node module form.'),
      'weight' => -5
    );
  }
  if ($type->has_body) {
    $extra['body_field'] = array(
      'label' => $type->body_label,
      'description' => t('Node module form.'),
      'weight' => 0,
      'view' => 'body'
    );
  }
  if (module_exists('locale') && variable_get("language_content_type_$type_name", 0)) {
    $extra['language'] = array(
      'label' => t('Language'),
      'description' => t('Locale module form.'),
      'weight' => 0
    );
  }
  if (module_exists('taxonomy') && taxonomy_get_vocabularies($type_name)) {
    $extra['taxonomy'] = array(
      'label' => t('Taxonomy'),
      'description' => t('Taxonomy module form.'),
      'weight' => -3
    );
  }

  return $extra;
}

/**
 * gallery upload limitation settings. seperate from upload module for more flexibility.
 *
 * @param unknown_type $default_settings
 * @param unknown_type $image_type
 * @return unknown
 */
function node_gallery_upload_limits_form(&$parent_form, $default_settings, $image_type) {
  $default_value = array(
    'file_extension' => 'jpg jpeg gif png',
    'file_resolution' => 0,
    'file_max_size' => 1,
    'user_max_size' => 2,
    'user_max_number' => 0,
  );
  
  $default_settings['general'] = array_merge($default_value, (array)$default_settings['general']);
  
  $form['general'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Image Upload Limitation Default Settings'),
    '#description' => t('Specify the image upload limitations'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['general']['file_extension'] = array(
    '#type' => 'textfield',
    '#title' => t('Default permitted file extensions'),
    '#maxlength' => 255,
    '#description' => t('Default extensions that users can upload. Separate extensions with a space and do not include the leading dot.'),
    '#default_value' => $default_settings['general']['file_extension'],
  );
  $form['general']['file_resolution'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum resolution for uploaded images'),
    '#size' => 15,
    '#maxlength' => 10,
    '#description' => t('The maximum allowed image size (e.g. 640x480). Set to 0 for no restriction. If an <a href="!image-toolkit-link">image toolkit</a> is installed, files exceeding this value will be scaled down to fit.', array('!image-toolkit-link' => url('admin/settings/image-toolkit'))),
    '#field_suffix' => '<kbd>'. t('WIDTHxHEIGHT') .'</kbd>',
    '#default_value' => $default_settings['general']['file_resolution'],
  );
  $form['general']['file_max_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Default maximum file size per upload'),
    '#default_value' => $default_settings['general']['file_max_size'],
    '#size' => 5,
    '#maxlength' => 5,
    '#description' => t('The default maximum file size a user can upload. If an image is uploaded and a maximum resolution is set, the size will be checked after the file has been resized.'),
    '#field_suffix' => t('MB'),
  );
  $form['general']['user_max_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Default total file size per user'),
    '#default_value' => $default_settings['general']['user_max_size'],
    '#size' => 5,
    '#maxlength' => 5,
    '#description' => t('The default maximum size of all files a user can have on the gallery.'),
    '#field_suffix' => t('MB'),
  );

  $form['general']['upload_max_size'] = array('#value' => '<p>'. t('Your PHP settings limit the maximum file size per upload to %size.', array('%size' => format_size(file_upload_max_size()))) .'</p>');
  
  $form['general']['user_max_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Default total file number per user'),
    '#default_value' => $default_settings['general']['user_max_number'],
    '#size' => 5,
    '#maxlength' => 5,
    '#description' => t('The maximum number of images a user can upload to the gallery. (Set to 0 for unlimited)'),
  );
  
  $roles = user_roles(FALSE, 'create '. $image_type .' content');
  $form['roles'] = array('#type' => 'value', '#value' => $roles);
  
  foreach ($roles as $rid => $role) {
    $form['role_'. $rid] = array(
      '#type' => 'fieldset',
      '#title' => t('Image Upload Limitation Settings for @role', array('@role' => $role)),
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['role_'. $rid]['role_name'] = array(
      '#type' => 'hidden',
      '#value' => $role,
    );
    $form['role_'. $rid]['file_max_size'] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum file size per upload'),
      '#default_value' => empty($default_settings['role_'. $rid]['file_max_size']) ? $default_settings['general']['file_max_size'] : $default_settings['role_'. $rid]['file_max_size'],
      '#size' => 5,
      '#maxlength' => 5,
      '#description' => t('The maximum size of a file a user can upload. If an image is uploaded and a maximum resolution is set, the size will be checked after the file has been resized.'),
      '#field_suffix' => t('MB'),
    );
    $form['role_'. $rid]['user_max_size'] = array(
      '#type' => 'textfield',
      '#title' => t('Total file size per user'),
      '#default_value' => empty($default_settings['role_'. $rid]['user_max_size']) ? $default_settings['general']['user_max_size'] : $default_settings['role_'. $rid]['user_max_size'],
      '#size' => 5,
      '#maxlength' => 5,
      '#description' => t('The maximum size of all files a user can have on the gallery.'),
      '#field_suffix' => t('MB'),
    );
    $form['role_'. $rid]['user_max_number'] = array(
      '#type' => 'textfield',
      '#title' => t('Total file number per user'),
      '#default_value' => empty($default_settings['role_'. $rid]['user_max_number']) ? $default_settings['general']['user_max_number'] : $default_settings['role_'. $rid]['user_max_number'],
      '#size' => 5,
      '#maxlength' => 5,
      '#description' => t('The maximum number of images a user can upload to the gallery. (Set to 0 for unlimited)'),
    );
  }
  
  $parent_form['#validate'][] = 'node_gallery_upload_limits_form_validate';
  $parent_form['config']['upload_limits'] = array_merge(array(), $form);
  
}

function node_gallery_upload_limits_form_validate($form, &$form_state) {
  $has_error = FALSE;
  
  if ($form_state['values']['step'] == 2) {
    $upload_limit_values = $form_state['values']['config']['upload_limits'];
    
    $default_file_max_size = $upload_limit_values['general']['file_max_size'];
    $default_user_max_size = $upload_limit_values['general']['user_max_size'];
    $exceed_max_msg = t('Your PHP settings limit the maximum file size per upload to %size.', array('%size' => format_size(file_upload_max_size()))) .'<br/>';
    $more_info = t("Depending on your server environment, these settings may be changed in the system-wide php.ini file, a php.ini file in your Drupal root directory, in your Drupal site's settings.php file, or in the .htaccess file in your Drupal root directory.");
    
    foreach ($upload_limit_values as $role_key => $role_values) {
      foreach ($role_values as $key => $value) {
        switch ($key) {
          case 'file_resolution':
            if ($value != '0') {
              if (!preg_match('/^[0-9]+x[0-9]+$/', $value)) {
                form_set_error('file_resolution', t('The maximum allowed image size expressed as WIDTHxHEIGHT (e.g. 640x480). Set to 0 for no restriction.'));
                $has_error = TRUE;
              }
            }
            break;
          case 'file_max_size':
            if (!is_numeric($value) || ($value <= 0)) {
              form_set_error('file_max_size', t('The %role file size limit must be a number and greater than zero.', array('%role' => ($role_key == 'general') ? t('default') : $role_values['role_name'])));
              $has_error = TRUE;
            }
            if ($value * 1024 * 1024 > file_upload_max_size()) {
              form_set_error('file_max_size', $exceed_max_msg . $more_info);
              $has_error = TRUE;
            }
            if ($value > $role_values['user_max_size']) {
              form_set_error('file_max_size', t('The %role maximum file size per upload is greater than the total file size allowed per user', array('%role' => ($role_key == 'general') ? t('default') : $role_values['role_name'])));
              $has_error = TRUE;
            }
            break;
          case 'user_max_size':
            if (!is_numeric($value) || ($value <= 0)) {
              form_set_error('user_max_size', t('The %role maximum file size limit must be a number and greater than zero.', array('%role' => ($role_key == 'general') ? t('default') : $role_values['role_name'])));
              $has_error = TRUE;
            }
            if ($value < $role_values['file_max_size']) {
              form_set_error('user_max_size', t('The %role file size per upload is greater than the total file size allowed per user', array('%role' => ($role_key == 'general') ? t('default') : $role_values['role_name'])));
              $has_error = TRUE;
            }
            break;
        }
      }
    }
    if ($has_error) {
      $form_state['values']['step'] --;
    }
  }
}

function node_gallery_config_delete_form($form_state, $gallery_config) {
  $form['gallery_id'] = array(
    '#type' => 'value',
    '#value' => 'node_gallery_'. $gallery_config['gallery_type'],
  );
  $form['gallery_name'] = array(
    '#type' => 'value',
    '#value' => $gallery_config['name'],
  );
  
  return confirm_form($form, t('Are you sure you want to delete gallery type config %name?', array('%name' => $gallery_config['name'])),
   'admin/settings/node_gallery', NULL, t('Delete'), t('Back'));
}

function node_gallery_config_delete_form_submit($form, &$form_state) {
  
  variable_del($form_state['values']['gallery_id']);
  drupal_set_message(t('Gallery type config %name has been deleted.', array('%name' => $form_state['values']['gallery_name'])));
  watchdog(
    'node_gallery',
    'Node Gallery relationship deleted.',
    WATCHDOG_NOTICE
  );
  
    $form_state['redirect'] = 'admin/settings/node_gallery';
}