<?php
// $Id: flag.export.inc,v 1.1.2.2 2009/11/03 02:10:45 quicksketch Exp $

/**
 * @file
 * Import/Export functionality provided by Flag module.
 */

/**
 * Export a flag to code.
 *
 * @param $flags
 *   An array of flag objects, or flag name.
 * @param $module
 *   Optional. The name of the module that will be created if exporting to use
 *   in hook_flag_default_flags().
 */
function flag_export_flags($flags = array(), $module = '') {
  $output = array();
  $output = '$flags = array();'. "\n";
  foreach ($flags as $flag) {
    if (!is_object($flag)) {
      // We got just the flag name, for example from the features
      // implementation.
      $flag = flag_get_flag($flag);
    }
    $new_flag = (array) $flag;

    if (!empty($module)) {
      // Even though Flag adds the module name itself later, we add the module
      // name here for reference by other modules (such as Features).
      $new_flag['module'] = $module;
      // Lock the flag name, as is normally desired by modules using
      // hook_flag_default_flags(), and needed by Features.
      $new_flag['locked'] = array('name');
    }
    // Allow other modules to change the exported flag.
    drupal_alter('flag_export', $new_flag);

    // Remove the flag ID.
    unset($new_flag['fid']);

    $output .= '// Exported flag: "'. check_plain($flag->get_title()) . '"' . ".\n";
    $output .= '$flags[] = ' . var_export($new_flag, TRUE) . ";\n";
  }
  $output .= 'return $flags;';
  return $output;
}

/**
 * Form to import a flag.
 */
function flag_import_form() {
  $form = array();

  $form['import'] = array(
    '#title' => t('Flag import code'),
    '#type' => 'textarea',
    '#default_value' => '',
    '#rows' => 15,
    '#required' => TRUE,
    '#description' => t('Paste the code from a <a href="@export-url">flag export</a> here to import it into you site. Flags imported with the same name will update existing flags. Flags with a new name will be created.', array('@export-url' => url('admin/build/flags/export'))),
  );
  $form['submit'] = array(
    '#value' => t('Import'),
    '#type' => 'submit',
  );

  return $form;
}

/**
 * Validate handler; Import a flag.
 */
function flag_import_form_validate($form, &$form_state) {
  $flags = array();
  ob_start();
  eval($form_state['values']['import']);
  ob_end_clean();

  if (!isset($flags) || !is_array($flags)) {
    form_set_error('import', t('A valid list of flags could be found in the import code.'));
    return;
  }

  // Create the flag object.
  foreach ($flags as $config) {
    $new_flag = flag_flag::factory_by_array($config);

    // Give new flags with the same name a matching FID, which tells Flag to
    // update the existing flag, rather than creating a new one.
    if ($existing_flag = flag_get_flag($new_flag->name)) {
      $new_flag->fid = $existing_flag->fid;
    }

    if ($errors = $new_flag->validate()) {
      $message = t('The import of the %flag flag failed because the following errors were encountered during the import:', array('%flag' => $new_flag->name));
      $message_errors = array();
      foreach ($errors as $field => $field_errors) {
        foreach ($field_errors as $error) {
          $message_errors[] = $error['message'];
        }
      }
      form_set_error('import', $message . theme('item_list', $message_errors));
    }
    else {
      // Save the new flag for the submit handler.
      $form_state['flags'][] = $new_flag;
    }
  }
}

/**
 * Submit handler; Import a flag.
 */
function flag_import_form_submit($form, &$form_state) {
  module_load_include('inc', 'flag', 'includes/flag.admin');

  foreach ($form_state['flags'] as $flag) {
    $flag->save();
    if (!empty($flag->status)) {
      $flag->enable();
    }
    if ($flag->is_new) {
      drupal_set_message(t('Flag @name has been imported.', array('@name' => $flag->name)));
    }
    else {
      drupal_set_message(t('Flag @name has been updated.', array('@name' => $flag->name)));
    }
  }
  _flag_clear_cache();

  $form_state['redirect'] = 'admin/build/flags';
}

/**
 * Export a flag and display it in a form.
 */
function flag_export_form(&$form_state, $flag = NULL) {
  $form = array();

  // Convert a flag name (if any) to the list of export flags.
  if (is_object($flag) || ($flag = flag_get_flag($flag))) {
    $flags = array($flag);
  }

  // Display a list of flags to export.
  if (!isset($flags)) {
    if (isset($form_state['values']['flags'])) {
      $flags = array();
      foreach ($form_state['values']['flags'] as $flag_name) {
        if ($flag_name && $flag = flag_get_flag($flag_name)) {
          $flags[] = $flag;
        }
      }
    }
    else {
      $form['flags'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Flags to export'),
        '#options' => drupal_map_assoc(array_keys(flag_get_flags())),
        '#description' => t('Exporting your flags is useful for moving flags from one site to another, or when including your flag definitions in a module.'),
      );
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Export'),
      );
    }
  }

  if (isset($flags)) {
    $code = flag_export_flags($flags);

    // Link to the Features page if module is present, otherwise link to the
    // Drupal project page.
    $features_link = module_exists('features') ? url('admin/build/features') : url('http://drupal.org/project/features');

    $form['export'] = array(
      '#type' => 'textarea',
      '#title' => t('Flag exports'),
      '#description' => t('Use the exported code to later <a href="@import-flag">import</a> it. Exports can be included in modules using <a href="http://drupal.org/node/305086#default-flags">hook_flag_default_flags()</a> or using the <a href="@features-url">Features</a> module.', array('@import-flag' => url('admin/build/flags/import'), '@features-url' => $features_link)),
      '#value' => $code,
      '#rows' => 15,
    );
  }

  return $form;
}

/**
 * Submit handler; Rebuild the export form after the list of flags has been set.
 */
function flag_export_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * Page for displaying an upgrade message and export form for Flag 1.x flags.
 */
function flag_update_page($flag_name) {
  $flags = flag_get_default_flags(TRUE);
  if (!isset($flags[$flag_name])) {
    drupal_not_found();
    return;
  }

  if ($flags[$flag_name]->api_version == FLAG_API_VERSION) {
    drupal_set_message(t('The flag %name is already up-to-date with the latest Flag API and does not need upgrading.'));
    drupal_goto('admin/build/flags');
  }

  $flag = $flags[$flag_name];
  drupal_set_message(t('The flag %name is currently using the Flag API version @version, which is not compatible with the current version of Flag. You can upgrade this flag by pasting the below code into <em>@module_flag_default_flags()</em> function in the @module.module file.', array('%name' => $flag->name, '@version' => $flag->api_version, '@module' => $flag->module)), 'warning');

  flag_update_export($flag);

  return drupal_get_form('flag_export_form', $flag);
}

/**
 * Update a flag before export.
 *
 * @param $flag
 *   The flag object passed by reference.
 */
function flag_update_export(&$flag) {
  // Update differences.
  if (empty($flag->api_version) || $flag->api_version == 1) {
    if (isset($flag->roles) && !isset($flag->roles['flag'])) {
      $flag->roles = array(
        'flag' => $flag->roles,
        'unflag' => $flag->roles,
      );
    }
    $flag->api_version = FLAG_API_VERSION;
  }
}
