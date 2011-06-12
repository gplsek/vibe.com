<?php

/**
 * @file
 * Node gallery admin page file
 *
 * The contents of this file are used in displaying the admin/settings/node_gallery/* pages
 */

include_once(drupal_get_path('module', 'views') .'/includes/admin.inc');

/**
 * Displays a list of all defined Gallery-to-Image relationships.
 *
 * @return
 *   A themed table suitable for output
 */
function node_gallery_relationship_list() {
  $ng_rels = node_gallery_get_all_relationships();

  if (!empty($ng_rels)) {
    $headers = array(t('Relationship name'), t('Gallery type'), t('Image type'), t('Image Field'));
    foreach ($ng_rels as $gallery_type => $config) {
      $image_type = content_types($config['image_type']);
      $field_link = 'admin/content/node-type/'. $image_type['url_str'] .'/fields/'. $config['imagefield_name'];
      $rows[] = array(array('data' => $config['settings']['name'], 'rowspan' => 2), $gallery_type, $config['image_type'], l($config['imagefield_name'], $field_link));
      $links = theme('links', node_gallery_relationship_operations($config['rid']));
      $operations = array(
        array('data' => $links, 'align' => 'right', 'colspan' => 3),
      );
      $rows[] = $operations;
    }
    return theme('table', $headers, $rows, array('class' => 'node-gallery-config-list'));
  }
  else {
    return t("There are no gallery relationships defined.");
  }
}

/**
 * Builds a list of all views tagged with 'node_gallery'
 *
 * @return
 *   An associative array where the key is a serialized array, and the value
 *   is the title of the view display.  The serialized array key is:
 *     - name: the name of the view.
 *     - display_id: the display_id of the display.
 */
function node_gallery_build_views_select($tags = 'node_gallery') {
  // load list of views into array for select lists
  $tags = (is_array($tags)) ? $tags : array($tags);
  $select = array();
  foreach (views_get_all_views() as $view) {
    if (!in_array($view->tag, $tags)) {
      continue;
    }
    foreach ($view->display as $display) {
      $key = array();
      $key['name'] = $view->name;
      $key['display_id'] = $display->id;
      $select[$view->name][serialize($key)] = $display->display_title;
    }
  }
  return $select;
}

/**
 * Displays the form at admin/settings/node_gallery/settings.
 *
 * @return
 *   A FAPI form array.
 */
function node_gallery_settings_form() {
  $selected_display = variable_get('node_gallery_galleries_summary_view', serialize(array('name' => 'node_gallery_gallery_summaries', 'display_id' => 'page_1')));
  $form['node_gallery_remove_orphans'] = array(
    '#type' => 'submit',
    '#weight' => -5,
    '#value' => t('Remove orphaned rows from database'),
    '#submit' => array('node_gallery_delete_orphans'),
  );
  $form['node_gallery_galleries_summary_view'] = array(
    '#type' => 'select',
    '#weight' => -4,
    '#title' => t('View display to use at /galleries'),
    '#description' => t('Select the display of the gallery view to be used when the /galleries URL is requested.  Only views tagged with \'node_gallery_galleries\' are displayed here.'),
    '#options' => node_gallery_build_views_select('node_gallery_galleries'),
    '#default_value' => $selected_display,
  );
  $form['node_gallery_plupload_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('plupload settings'),
    '#description' => t('This area defines the settings used when integrating Node Gallery with plupload.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['node_gallery_plupload_settings']['node_gallery_plupload_integration'] = array(
    '#type' => 'checkbox',
    '#weight' => 0,
    '#title' => t('Use plupload module to upload images?'),
    '#default_value' => variable_get('node_gallery_plupload_integration', TRUE),
    '#description' => t('When checked, the plupload widget will be used on the "Upload Images" tab to add new images to a gallery.'),
  );
  $form['node_gallery_plupload_settings']['node_gallery_plupload_manage_images_integration'] = array(
    '#type' => 'checkbox',
    '#weight' => 0,
    '#title' => t('Integrate plupload and the Manage Images tab?'),
    '#default_value' => variable_get('node_gallery_plupload_manage_images_integration', TRUE),
    '#description' => t('When checked, the first visit to the Manage Images tab after uploading new images will only show newly uploaded images.'),
  );
  $form['node_gallery_plupload_settings']['node_gallery_plupload_manage_images_limit'] = array(
    '#type' => 'textfield',
    '#maxlength' => 5,
    '#size' => 5,
    '#title' => t('Enter the limit on the number of images displayed after upload.'),
    '#default_value' => variable_get('node_gallery_plupload_manage_images_limit', 100),
    '#description' => t('Displaying too many images on the Manage Images tab at once will exhaust the server\'s memory.  This setting limits the number of images listed.'),
  );
  if (! module_exists('plupload')) {
    $form['node_gallery_plupload_settings']['node_gallery_plupload_integration']['#disabled'] = TRUE;
    $form['node_gallery_plupload_settings']['node_gallery_plupload_integration']['#default_value'] = FALSE;
    $form['node_gallery_plupload_settings']['node_gallery_plupload_integration']['#description'] = t('You need to download and install the <a href="@plupload_url">plupload module</a> to enable this option.', array('@plupload_url' => url('http://drupal.org/project/plupload')));
  }
  $form['node_gallery_jquery_ui_integration'] = array(
    '#type' => 'checkbox',
    '#weight' => 0,
    '#title' => t('Use jQuery UI module to sort images?'),
    '#default_value' => variable_get('node_gallery_jquery_ui_integration', TRUE),
    '#description' => t('When checked, a drag and drop interface using thumbnails will be used to sort images instead of a table of rows.'),
  );
  $form['node_gallery_sort_images_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of images to allow on sort images page'),
    '#default_value' => variable_get('node_gallery_sort_images_max', 750),
    '#weight' => 0,
    '#description' => t('The sort images page can require a lot of memory on both the server and the client if the gallery contains a large amount of images. If the image count exceeds this value, the form rendering will be aborted.  In most cases, the default is sufficient.'),
    '#maxlength' => 5,
    '#size' => 5,
  );
  if (!module_exists('jquery_ui')) {
    $form['node_gallery_jquery_ui_integration']['#disabled'] = TRUE;
    $form['node_gallery_jquery_ui_integration']['#default_value'] = FALSE;
    $form['node_gallery_jquery_ui_integration']['#description'] = t('You need to download and install the <a href="@jquery_ui_url">jquery_ui module</a> to enable this option.', array('@jquery_ui_url' => url('http://drupal.org/project/jquery_ui')));
  }
  $form = system_settings_form($form);
  return $form;
}

/**
 * Validate the input from the global settings form.
 */
function node_gallery_settings_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['node_gallery_sort_images_max']) ||
          $form_state['values']['node_gallery_sort_images_max'] < 0) {
    form_error($form['node_gallery_sort_images_max'], t('The maximum number of images on the sort images page must be a positive integer.'));
  }
}

/**
 * Returns a list of operations available to a Gallery-to-Image relationship.
 *
 * @return
 *   An array containing associative arrays with keys and values of titles and href links
 */
function node_gallery_relationship_operations($rid) {
  $items[] = array('title' => t('Change content types'), 'href' => 'admin/settings/node_gallery/relationship/content_types/'. $rid);
  $items[] = array('title' => t('Change settings'), 'href' => 'admin/settings/node_gallery/relationship/settings/'. $rid);
  $items[] = array('title' => t('Delete'), 'href' => 'admin/settings/node_gallery/delete/'. $rid);

  return $items;
}

function node_gallery_relationship_content_types_form($form_state, $relationship = NULL) {
  static $node_types, $image_types, $imagecaches;
  $form['relationship'] = array(
    '#tree' => TRUE,
  );

  if (empty($node_types)) {
    foreach (node_get_types() as $node_type) {
      $node_types[$node_type->type] = $node_type->name;
    }
  }
  $form['#relationship'] = $relationship;
  $form['relationship']['gallery_type'] = array(
    '#type' => 'radios',
    '#title' => t('Gallery Type'),
    '#options' => $node_types,
    '#description' => t('Select which content type should be used as the gallery type.'),
    '#default_value' => $relationship['gallery_type'],
    '#required' => TRUE,
  );
  $form['relationship']['image_type'] = array(
    '#type' => 'radios',
    '#title' => t('Image Type'),
    '#options' => $node_types,
    '#description' => t('Select which content type should be used as the image type.'),
    '#default_value' => $relationship['image_type'],
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Returns a FAPI form array that renders the settings form when adding/editing a Gallery-to-Image relationship
 *
 * @parm $form_state
 *
 * @param $gallery_config
 *   The existing gallery config relationship
 *
 * @return
 *   A FAPI form array
 */

function node_gallery_relationship_settings_form($form_state, $relationship) {
  static $node_types, $image_types, $imagecaches;

  $image_type = content_types($relationship['image_type']);
  if (module_exists('taxonomy') && taxonomy_get_vocabularies($image_type['type'])) {
    $image_type['extra']['taxonomy'] = array(
      'label' => t('Taxonomy'),
      'description' => t('Taxonomy module form.'),
      'weight' => -3
    );
  }
  //non-cck fields
  foreach ($image_type['extra'] as $name => $field) {
    if ($name != 'menu' && $name != 'attachments' && $name != 'workflow') {
      $image_type_fields[$name] = $field['label'];
    }
  }
  //cck fields
  foreach ($image_type['fields'] as $name => $field) {
    $image_type_fields[$name] = $field['widget']['label'];
  }

  if (empty($imagecaches)) {
    foreach (imagecache_presets() as $id => $imagecache) {
      $imagecaches[$imagecache['presetname']] = $imagecache['presetname'];
    }
  }
  if (empty($imagecaches)) {
    form_set_error('Node gallery config', t('There are no imagecache presets. <a href="@imagecache">You must create at least one preset</a>.', array('@imagecache' => url('admin/build/imagecache/add'))));
  }
  module_load_include('inc', 'content', 'includes/content.crud');
  $imagefield_instances = content_field_instance_read(array('type_name' => $relationship['image_type']));
  $options_imagefields = array();
  foreach ($imagefield_instances as $imagefield_instance) {
    $options_imagefields[$imagefield_instance['field_name']] = $imagefield_instance['field_name'];
  }
  if (!array_key_exists('field_node_gallery_image', $options_imagefields)) {
    $options_imagefields['field_node_gallery_image'] = t('Create new imagefield');
  }

  $form['#relationship'] = $relationship;
  $form['node_gallery_relationship_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Relationship settings'),
    '#description' => t('This area defines the settings used to define the relationship between the Gallery and Image content types.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['node_gallery_relationship_settings']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Relationship Name'),
    '#description' => t('Enter a name for this relationship which will only be viewed by the admin user.'),
    '#default_value' => $relationship['settings']['name'],
    '#required' => TRUE,
  );
  $form['node_gallery_relationship_settings']['imagefield'] = array(
    '#type' => 'select',
    '#title' => t('Image storage field'),
    '#description' => t('Select the CCK imagefield that will be used for storing the gallery images.  In most cases, you shouldn\'t need to change this setting.'),
    '#options' => $options_imagefields,
    '#default_value' => (isset($relationship['imagefield_name']) && !empty($relationship['imagefield_name'])) ? $relationship['imagefield_name'] : 'field_node_gallery_image',
  );
  $viewdisplays = node_gallery_build_views_select('node_gallery_images');

  $form['node_gallery_gallery_view'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gallery settings'),
    '#description' => t('This area defines the settings used to display an individual gallery node.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['node_gallery_gallery_view']['full'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gallery full(page) view settings'),
    '#description' => t('Settings applied to a gallery node when being viewed in full(page) mode.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['node_gallery_gallery_view']['full']['view_gallery_full_image_display'] = array(
    '#type' => 'select',
    '#title' => t('Gallery full(page) view definition'),
    '#description' => t('Select the display of the images which will be attached to the page view of a gallery. Only Views tagged with \'node_gallery_images\' are displayed here.'),
    '#options' => $viewdisplays,
    '#default_value' => $relationship['settings']['view_gallery_full_image_display'],
  );
  $form['node_gallery_gallery_view']['full']['view_navigator_image_display'] = array(
    '#type' => 'select',
    '#title' => t('Gallery image sort view definition'),
    '#description' => t('Select the display that defines the desired sort order to be used when displaying images. You must include the \'Node: Nid\' field in this view. Only Views tagged with \'node_gallery_images\' are displayed here.'),
    '#options' => $viewdisplays,
    '#default_value' => $relationship['settings']['view_navigator_image_display'],
  );
  $form['node_gallery_gallery_view']['teaser'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gallery teaser(list) view settings'),
    '#description' => t('Settings applied to a gallery node when being viewed in teaser(list) mode.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['node_gallery_gallery_view']['teaser']['view_gallery_teaser_view_image_display'] = array(
    '#type' => 'select',
    '#title' => t('Gallery teaser(list) view definition'),
    '#description' => t('Select the display of the images which will be attached to the teaser view of a gallery. Only Views tagged with \'node_gallery_images\' are displayed here.'),
    '#options' => $viewdisplays,
    '#default_value' => $relationship['settings']['view_gallery_teaser_view_image_display'],
  );

  // image view options
  $form['node_gallery_image_view'] = array(
    '#type' => 'fieldset',
    '#title' => t('Image view settings'),
    '#description' => t('This area defines the settings used when displaying an individual image node.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['node_gallery_image_view']['display_navigator'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display image navigator on image pages?'),
    '#default_value' => $relationship['settings']['display_navigator'],
    '#description' => t('When checked, the navigator (bar providing "Previous", "Next", etc. links) will be displayed at the top of the image page.'),
  );
  $form['node_gallery_image_view']['node_images_page_fragment'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Named Anchors on Image "Previous" and "Next" links?'),
    '#default_value' => $relationship['settings']['node_images_page_fragment'],
    '#description' => t('When checked, the "Previous" and "Next" links will jump to the top of the image. You might want to leave this unchecked if you have ads at the top of your image pages.'),
  );

  // Manage images tab settings
  $form['node_gallery_manage_images_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Manage Images tab settings'),
    '#description' => t('This area defines the settings used by the Manage Images tab.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['node_gallery_manage_images_settings']['manage_images_fields'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Manage Images Fields'),
    '#options' => $image_type_fields,
    '#description' => t('Specify which fields should be displayed on the Manage Images tab of a gallery node.'),
    '#default_value' => $relationship['settings']['manage_images_fields'],
  );
  $form['node_gallery_manage_images_settings']['manage_images_per_page'] = array(
    '#type' => 'textfield',
    '#maxlength' => 5,
    '#size' => 5,
    '#title' => t('Enter the number of images displayed on each Manage Images page'),
    '#default_value' => $relationship['settings']['manage_images_per_page'],
    '#description' => t('If this number is exceeded, a pager will be displayed below each corresponding Manage Images page.'),
  );
  $form['node_gallery_manage_images_settings']['manage_images_show_gallery_list'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable changing an image\'s gallery on the Manage Images page'),
    '#default_value' => $relationship['settings']['manage_images_show_gallery_list'],
    '#description' => t('If checked, a gallery dropdown list will be displayed on each image on the Manage Images tab allowing you to move images between galleries.'),
  );
  if (imageapi_default_toolkit() != 'imageapi_gd' || function_exists("imagerotate")) {
    $form['node_gallery_manage_images_settings']['manage_images_enable_rotation'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable the rotation of images on the manage images page'),
      '#default_value' => $relationship['settings']['manage_images_enable_rotation'],
      '#description' => t('If checked, image rotation operations will be displayed on each image on the Manage Images tab.'),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 8,
  );
  $form['#submit'] = array('node_gallery_relationship_settings_form_submit');


  return $form;
}

/**
 * Validation callback for the relationship form.
 * @param $form
 * @param $form_state
 */
function node_gallery_relationship_content_types_form_validate($form, &$form_state) {
  $gallery_type = $form_state['values']['relationship']['gallery_type'];
  $image_type = $form_state['values']['relationship']['image_type'];
  if ($gallery_type == $image_type) {
    form_set_error('Type', t('The gallery type and image type can\'t be the same.'));
  }
  if (($r = node_gallery_get_relationship($gallery_type)) && $r['rid'] != $form['#relationship']['rid']) {
    form_set_error('relationship][gallery_type', t('The !gallery_type type is already part of a relationship.', array('!gallery_type' => $gallery_type)));
  }
  if (($r = node_gallery_get_relationship(NULL, $image_type)) && $r['rid'] != $form['#relationship']['rid']) {
    form_set_error('relationship][image_type', t('The !image_type type is already part of a relationship.', array('!image_type' => $image_type)));
  }
}

/**
 * Submit handler for relationship settings form
 *
 * @param $form
 * @param $form_state
 */
function node_gallery_relationship_content_types_form_submit($form, &$form_state) {
  $r = (object)$form['#relationship'];
  $send_to_settings_form = FALSE;
  foreach ($form_state['values']['relationship'] as $k => $v) {
    if ($r->{$k} != $v) {
      $r->{$k} = $v;
      if ($k == 'image_type' && $r->{k}) {
        // They need to update the imagefield setting if they change image types
        $send_to_settings_form = TRUE;
        drupal_set_message(t('Image content type changed, please verify the Image storage field below.'), 'warning');
      }
    }
  }
  if (!isset($r->rid)) {
    //New relationship
    $r->settings = node_gallery_relationship_settings_defaults();
    $r->imagefield_name = '';
    $send_to_settings_form = TRUE;
    drupal_write_record('node_gallery_relationships', $r);
    menu_cache_clear_all();
    drupal_set_message(t('Default settings applied, please verify the Image storage field below.'), 'warning');
  }
  else {
    drupal_write_record('node_gallery_relationships', $r, 'rid');
    menu_cache_clear_all();
    drupal_set_message(t('Node Gallery relationship updated'));
  }
  if ($send_to_settings_form) {
    $form_state['redirect'] = 'admin/settings/node_gallery/relationship/settings/'. $r->rid;
  }
  else {
    $form_state['redirect'] = 'admin/settings/node_gallery';
  }
  // Repopulate the cache
  node_gallery_get_relationship($r->gallery_type, TRUE);
}

/**
 * Submit handler for relationship settings form
 *
 * @param $form
 * @param $form_state
 */
function node_gallery_relationship_settings_form_submit($form, &$form_state) {
  $r = (object)$form['#relationship'];
  $r->imagefield_name = $form_state['values']['imagefield'];

  if ($r->imagefield_name == 'field_node_gallery_image') {
    // if the field is named 'field_node_gallery_image' check if it exists
    // or if we need to create it. We could hack around that check with a pre- or
    // suffix, but I rate this way cleaner (though more costly).
    $param = array(
      'type_name' => $r->image_type,
      'field_name' => $r->imagefield_name,
    );
    if (! node_gallery_cck_field_exists($param)) {
      // @todo: there's a little bit of DRY going on here and in node_gallery_install_cck_type() in .install.
      //  We should pull this into a common function at some point.

      // create the default imagefield read out of the default type
      $path = drupal_get_path('module', 'node_gallery') . "/cck_types";
      $cck_file = $path . '/node_gallery_image.cck';
      $param = array('type_name' => $r->image_type);
      $content = array();
      eval(file_get_contents($cck_file));
      $param = array_merge($param, array_pop($content['fields']));
      unset($content);
      content_field_instance_create($param);
      drupal_set_message(t('Node Gallery imagefield created.'));
    }
  }
  $settings = isset($r->settings) ? $r->settings : array();
  $valid_settings = array_keys(node_gallery_relationship_settings_defaults());
  foreach ((array)$form_state['values'] as $k => $v) {
    if (in_array($k, $valid_settings)) {
      $settings[$k] = $v;
    }
  }
  $r->settings = $settings;
  drupal_write_record('node_gallery_relationships', $r, 'rid');
  menu_cache_clear_all();
  drupal_set_message(t('Node Gallery relationship updated'));
  // Repopulate the cache
  node_gallery_get_relationship($r->gallery_type, TRUE);

  $form_state['redirect'] = 'admin/settings/node_gallery';
}

/**
 * Builds the form used to delete relationships.
 *
 * @param $form_state
 * @param $relationship
 *   The relationship array.
 *
 * @return
 *   A confirmation form.
 */
function node_gallery_relationship_delete_form($form_state, $relationship) {
  $form['rid'] = array(
    '#type' => 'value',
    '#value' => $relationship['rid'],
  );
  $form['relationship_name'] = array(
    '#type' => 'value',
    '#value' => $relationship['settings']['name'],
  );

  return confirm_form($form, t('Are you sure you want to delete the relationship %name?', array('%name' => $relationship['settings']['name'])),
   'admin/settings/node_gallery', NULL, t('Delete'), t('Back'));
}

/**
 * Submit handler for the deletion form.
 *
 * @param $form
 * @param $form_state
 */
function node_gallery_relationship_delete_form_submit($form, &$form_state) {

  node_gallery_delete_relationship($form_state['values']['rid']);
  drupal_set_message(t('Relationship %name has been deleted.', array('%name' => $form_state['values']['relationship_name'])));
  watchdog(
    'node_gallery',
    'Node Gallery relationship deleted.',
  WATCHDOG_NOTICE
  );

  $form_state['redirect'] = 'admin/settings/node_gallery';
}
