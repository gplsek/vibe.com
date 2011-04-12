<?php
// $Id: activity.admin.inc,v 1.1.2.28 2009/12/09 05:54:48 scottreynolds Exp $

/**
 * @file activity.admin.inc
 * Contains administrative forms for activity.module
 */

/**
 * Menu callback. Provides checkboxes for a user's activity feed.
 */
function activity_user_settings(&$form_state, $account) {
  $form['actions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Actions within !site', array('!site' => variable_get('site_name', 'drupal'))),
    '#description' => t('By default, all of the following actions you perform can be viewed on this site. <br />Select the activities you would like to <strong>exclude</strong> people from seeing.'),
  );
  $form['actions']['activity_ignore']['#tree'] = TRUE;

  // In order to see what actions are being implemented on a given system we
  // need to query the actions table joining the trigger_assignments.
  $query = "SELECT a.aid, a.description, ta.hook, ta.op FROM {actions} a
            INNER JOIN {trigger_assignments} ta ON ta.aid = a.aid
            WHERE a.type = 'activity'";
  $result = db_query($query);
  while ($row = db_fetch_object($result)) {
    $hook = ($row->hook == 'nodeapi') ? 'node' : $row->hook;
    $function = $hook .'_hook_info';
    $info = $function();
    $description = ($info[$hook][$row->hook][$row->op]['runs when']);
    // This means we're basically keying off of the aid and not the triggers. We
    // could possible key off of the triggers instead, and give the user more
    // fine grained control over (i.e. I want to record my own node inserts but
    // not my node updates).
    $form['actions']['activity_ignore'][$row->hook .'_'. $row->op] = array(
      '#type' => 'radios',
      '#title' => $description,
      '#options' => array(1 => t('Record an activity message'), 0 => t('Do not record')),
      '#default_value' => is_null($account->activity_ignore[$row->hook .'_'. $row->op]) ? 1 : $account->activity_ignore[$row->hook .'_'. $row->op],
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['#account'] = $account;
  return $form;
}

/**
 * User settings form submit handler.
 */
function activity_user_settings_submit($form, &$form_state) {
  user_save($form['#account'], array('activity_ignore' => $form_state['values']['activity_ignore']));
}

/**
 * Menu callback to administer activity messages.
 */
function activity_form(&$form_state, $arg1 = NULL, $arg2 = NULL) {
  if (!empty($form_state['storage']['step'])) {
    $function = $form_state['storage']['step'];
    return $function($form_state);
  }
  else {
    // see if we have any current activity actions configured
    $sql = "SELECT * FROM {actions} WHERE type = 'activity'";
    if ($arg1 == 'create') {
      // if we have a specific callback to create a new action, do so
      return _activity_settings_form_triggers($form_state);
    }
    else if ($arg1 == 'configure') {
      $form_state['storage']['action'] = $arg2;
      return _activity_settings_form_messages($form_state);
    }
    else if (!db_result(db_query($sql))) {
      // we've not specifically asked to create a new one, but there aren't any
      // NOTE: we put this in an elseif so that query is not run for first case
      return _activity_settings_form_triggers($form_state);
    }
    else {
      return _activity_settings_actions_list_form();
    }
  }
}

/**
 * Form validation controller.
 */
function activity_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['step_validate'])) {
    $function = $form_state['values']['step_validate'];
    $function($form, $form_state);
  }
}

/**
 * Form submit controller.
 */
function activity_form_submit($form, &$form_state) {
  if (empty($form_state['storage'])) {
    $form_state['storage'] = array();
    $form_state['storage']['values'] = array();
  }
  // Call user-defined submit function.
  if (!empty($form_state['values']['step_submit'])) {
    $function = $form_state['values']['step_submit'];
    $function($form, $form_state);
  }
  // Store submitted form values, this must happen after function call above to
  // allow for modifying $form_state['values'].
  $this_step = $form_state['values']['this_step'];
  $form_state['storage']['values'][$this_step] = $form_state['values'];
  // Set up next step.
  if (!empty($form_state['values']['step_next']) && $form_state['values']['op'] != 'Back') {
    $form_state['storage']['step'] = $form_state['values']['step_next'];
  }
  else if ($form_state['values']['op'] == 'Back') {
    // Set up previous step
    $form_state['storage']['step'] = $form_state['values']['step_prev'];
  }
  else {
    // Form complete!
    $values = $form_state['storage']['values'];
    $module = $values['triggers']['module'];
    $op = $values['operations']['operation'];

    // stupid freaking special case for node - nodeapi
    if ($module == 'nodeapi') {
      $module = 'node';
    }

    $info = call_user_func($module .'_activity_info');
    foreach (activity_enabled_languages() as $id => $language) {
      foreach ($info->objects as $type) {
        $params[$type .'-pattern-' . $id] = $values['messages'][$type .'-pattern-' . $id];
      }
      $params['everyone-pattern-' . $id] = $values['messages']['everyone-pattern-' . $id];
    }
    $desc = $values['messages']['description'];
    if (!$aid = $values['messages']['aid']) {
      $aid = NULL;
    }
    
    // handle the types, thanks Checkboxes :-D
    foreach ($values['operations']['activity_types'] as $type => $nice_name) {
      if (empty($nice_name)) {
        unset($values['operations']['activity_types'][$type]);
      }
    }
    $params['activity_types'] = $values['operations']['activity_types'];
    
    // Save the action
    include_once('includes/actions.inc');
    $aid = actions_save('activity_record', 'activity', $params, $desc, $aid);

    // stupid freaking special case for node - nodeapi
    if ($module == 'node') {
      $module = 'nodeapi';
    }

    // Save the trigger assignment
    // @todo: calc weight (last param)
    $record = new stdClass();
    $record->hook = $module;
    $record->op = $op;
    $record->aid = $aid;
    $record->weight = 1;

    // try update first
    drupal_write_record('trigger_assignments', $record, array('aid'));
    if (!db_affected_rows()) {
      // ok then insert
      drupal_write_record('trigger_assignments', $record);
    }
    
    unset($form_state['storage']);
    $form_state['redirect'] = 'admin/build/activity';
    drupal_set_message('Saved.');
  }
}

/**
 * List activity action that are already present in the system.
 */
function _activity_settings_actions_list_form() {
  $form = array();
  $sql = "SELECT ta.hook, ta.aid, a.description
          FROM {trigger_assignments} ta
          INNER JOIN {actions} a
            ON a.aid = ta.aid
          WHERE a.type = 'activity'";
  $query = db_query($sql);
  while ($result = db_fetch_array($query)) {
    $results[] = $result;
  }
  
  // since our originating callback is expecting a form
  $form['activity_actions'] = array(
    '#type' => 'markup',
    '#value' => theme('activity_settings_actions_list', $results),
  );
  return $form;
}

/**
 * List of modules that implement activity_info().
 */
function _activity_settings_form_triggers(&$form_state) {
  $form = array();

  foreach (activity_get_module_info() as $module => $info) {
    if (!empty($info->hooks)) {
      foreach ($info->hooks as $hook => $ops) {
        $options[$hook] = drupal_ucfirst(str_replace('_', ' ', $info->name));
      }
    }
  }
  $form['module'] = array(
    '#type' => 'radios',
    '#title' => t('Available Activity Types'),
    '#options' => $options,
    '#default_value' => $form_state['storage']['values']['triggers']['module'],
    '#required' => TRUE,
    '#description' => t('Please choose which type of activity you would like to record.'),
  );

  $form['continue'] = array(
    '#type' => 'submit',
    '#value' => 'Continue',
  );
  $form['this_step'] = array(
    '#type' => 'value',
    '#value' => 'triggers',
  );
  $form['step_next'] = array(
    '#type' => 'value',
    '#value' => '_activity_settings_form_operations',
  );
  return $form;
}

/**
 * List the ops for the module selected in the _triggers step.
 */
function _activity_settings_form_operations(&$form_state) {
  $form = array();
  $module = $form_state['storage']['values']['triggers']['module'];
  // stupid freaking special case for node - nodeapi
  if ($module == 'nodeapi') {
    $module = 'node';
  }
  foreach (call_user_func($module .'_hook_info') as $module => $trigger_ops) {
    // Compare ops with our activity_info so that we do not provide a trigger
    // that activity doesn't support (ex: nodeapi - presave)
    // @see: node_activity_info()
    $module_info = activity_get_module_info($module);
    // stupid freaking special case for node - nodeapi
    if ($module == 'node') {
      $module = 'nodeapi';
    }
    foreach ($module_info->hooks[$module] as $op) {
      $ops[$module][$op] = $trigger_ops[$module][$op];
    }
    foreach ($ops as $trigger_name => $hooks) {
      $options = array();
      foreach ($hooks as $op => $runs_when) {
        $options[$op] = $op .': '. $runs_when['runs when'];
      }
      $default_value = isset($form_state['storage']['values']['operations']['operation']) ? $form_state['storage']['values']['operations']['operation'] : '';
      $form[$trigger_name]['operation'] = array(
        '#type' => 'radios',
        '#title' => t(drupal_ucfirst(str_replace('_', ' ', $trigger_name)) .' Triggers'),
        '#description' => t('Please choose when you would like to record a message.'),
        '#options' => $options,
        '#default_value' => $default_value,
        '#required' => TRUE,
      );
    }
  }
  if (!empty($module_info->type_options)) {
    if (!empty($form_state['storage']['values']['operations']['activity_types'])) {
      $default_types = $form_state['storage']['values']['operations']['activity_types'];
    }
    else {
      $default_types = array();
    }
    $form['activity_types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Record for the following types'),
      '#options' => $module_info->type_options,
      '#default_value' =>  $default_types,
      '#description' => t('If none are selected then these messages will be recorded for all.'),
    );
  }


  $form['back'] = array(
    '#type' => 'submit',
    '#value' => 'Back',
  );
  $form['continue'] = array(
    '#type' => 'submit',
    '#value' => 'Continue',
  );

  $form['step_prev'] = array(
    '#type' => 'value',
    '#value' => '_activity_settings_form_triggers',
  );
  $form['this_step'] = array(
    '#type' => 'value',
    '#value' => 'operations',
  );
  $form['step_next'] = array(
    '#type' => 'value',
    '#value' => '_activity_settings_form_messages',
  );
  return $form;
}

/**
 * Show the text boxes according to the _triggers and _ops steps.
 */
function _activity_settings_form_messages(&$form_state) {
  $form = array();
  if (isset($form_state['storage']['step'])) {
    $module = $form_state['storage']['values']['triggers']['module'];
    $operation = $form_state['storage']['values']['operations']['operation'];
    $aid = FALSE;    
    $activity_types = isset($form_state['storage']['values']['operations']['activity_types']) ? $form_state['storage']['values']['operations']['activity_types'] : array();
  }
  else {
    $aid = $form_state['storage']['action']->aid;
    // by doing this we assume that an action is not assigned to more than one trigger
    $result = db_query('SELECT hook, op FROM {trigger_assignments} WHERE aid = %d', $aid);
    while ($row = db_fetch_array($result)) {
      $module = $form_state['storage']['values']['triggers']['module'] = $row['hook'];
      $operation = $form_state['storage']['values']['operations']['operation'] = $row['op'];
    }
  }
  // stupid freaking special case for node - nodeapi
  if ($module == 'nodeapi') {
    $module = 'node';
  }
  $module_info = activity_get_module_info($module);

  if (isset($form_state['storage']['action'])) {
    $default_values = unserialize($form_state['storage']['action']->parameters);
  }
  else {
    $default_values = array();
  }
  
  // If this came through the multistep process it is already set above.
  // Otherwise we need to get it from the default values
  if (!isset($activity_types)){
    if (isset($default_values['activity_types'])) {
      $activity_types = $default_values['activity_types'];
    }
    else {
      $activity_types = array();
    }
  }
  $form_state['storage']['values']['operations']['activity_types'] = $activity_types;

  // Filter out false checkboxes.
  $activity_types = array_filter($activity_types);

  if (empty($activity_types)) {
    $types = 'All';
  }
  else {
    $types = implode(', ' , array_intersect_key($module_info->type_options, $activity_types));
  }

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => isset($form_state['storage']['action']) ? $form_state['storage']['action']->description : t('Record an activity message when: !module !op types !types', array('!module' => $module, '!op' => $operation, '!types' => $types)),
    '#maxlength' => '254',
    '#description' => t('A unique description for this advanced action. This description will be displayed in the interface of modules that integrate with actions, such as Trigger module.'),
  );
  // @todo: extend ->objects to have a description which will fill out the
  //  #description for this form element. Remember to adjust logic everywhere
  //  ->objects is used.
  foreach (activity_enabled_languages() as $id => $language) {
    $form["{$id}_fieldset"] = array(
      '#type' => 'fieldset',
      '#title' => t('@name messages', array('@name' => $language->name)),
    );
    foreach ($module_info->objects as $label => $type) {
      $clean_type = drupal_ucfirst(str_replace('_', ' ', $label));
      $form["{$id}_fieldset"][$type .'-pattern-' . $id] = array(
        '#type' => 'textarea',
        '#title' => t('!type message', array('!type' => $clean_type)),
        '#default_value' => isset($default_values[$type .'-pattern-' . $id]) ? $default_values[$type .'-pattern-' . $id] : '',
        '#description' => t('Using the available tokens, enter a message as how it should <strong>appear to the !type</strong> of this particular activity.', array('!type' => $clean_type)),
      );
    }
    $form["{$id}_fieldset"]['everyone-pattern-' . $id] = array(
      '#type' => 'textarea',
      '#title' => t('Public message'),
      '#default_value' => isset($default_values['everyone-pattern-' . $id]) ? $default_values['everyone-pattern-' . $id] : '',
      '#description' => t('Using the available tokens, enter a message as how it should <strong>appear to anyone who is <em>not</em> the author</strong> of this particular activity.'),
    );
  }
  
  // don't show the back button when editing
  if (!isset($form_state['storage']['action'])) {
    $form['back'] = array(
      '#type' => 'submit',
      '#value' => 'Back',
    );
  }
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );
  
  $form['step_prev'] = array(
    '#type' => 'value',
    '#value' => '_activity_settings_form_operations',
  );
  $form['this_step'] = array(
    '#type' => 'value',
    '#value' => 'messages',
  );

  $form['token_help'] = array(
    '#type' => 'fieldset',
    '#title' => t('Available tokens'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['token_help']['tokens'] = array(
    '#type' => 'markup',
    '#value' => theme('activity_token_help', array_values($module_info->objects)),
  );

  $form['aid'] = array(
    '#type' => 'value',
    '#value' => $aid,
  );
  
  return $form;
}

/**
 * Create the form for confirmation of deleting an activity action.
 *
 * @ingroup forms
 * @see activity_actions_delete_form_submit()
 */
function activity_actions_delete_form(&$form_state, $action) {
  $form['aid'] = array(
    '#type' => 'hidden',
    '#value' => $action->aid,
  );
  return confirm_form($form,
    t('Are you sure you want to delete the action %action?', array('%action' => $action->description)),
    'admin/settings/actions/manage',
    t('This cannot be undone.'),
    t('Delete'), t('Cancel')
  );
}

/**
 * Process activity_actions_delete form submissions.
 *
 * Post-deletion operations for activity action deletion.
 */
function activity_actions_delete_form_submit($form, &$form_state) {
  $aid = $form_state['values']['aid'];
  $action = actions_load($aid);
  actions_delete($aid);
  watchdog('user', 'Deleted action %aid (%action)', array('%aid' => $aid, '%action' => $action->description));
  drupal_set_message(t('Action %action was deleted', array('%action' => $description)));
  $form_state['redirect'] = 'admin/build/activity';
}

/**
 * Form builder to dispaly settings for activity module
 */
function activity_settings_form(&$form_state = NULL) {
  $form['activity_expiration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Activity Expiration Settings'),
    '#element_validate' => array('activity_expire_validate'),
  );
  
  $form['activity_expiration']['activity_expire'] = array(
    '#type' => 'select',
    '#title' => t('Activity log purge'),
    '#description' => t("Allows you to set a time limit for storing activity records. Select 0 to keep all activity records."),
    '#options' => drupal_map_assoc(array(0, 3600, 7200, 14400, 21600, 43200, 86400, 604800, 1209600, 2419200, 7257600, 15724800, 31536000), 'format_interval'),
    '#default_value' => variable_get('activity_expire', 0),
  );
  
  $form['activity_expiration']['activity_min_count'] = array(
    '#type' => 'select',
    '#title' => t('Minimum Activities'),
    '#description' => t('This is the minimum number activities that the user must have created before deleting any old activities.'),
    '#options' => drupal_map_assoc(range(0, 500, 50)),
    '#default_value' => variable_get('activity_min_count', 0),
  );
  

  // get all the  modules and their realms
  $api_info = activity_get_module_info();
  foreach ($api_info as $module => $info) {
    foreach ($info->realms as $realm => $nice_name) {
      $realms[$realm] = t('@module: @name', array('@module' => drupal_ucfirst($module), '@name' => $nice_name));
    }
  }

  $form['activity_access'] = array(
    '#type' => 'fieldset',
    '#title' => t('Activity Access Control'),
  );
  $form['activity_access']['activity_access_realms'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Realms'),
    '#description' => t('Select the realms for which Activity access records should be recorded. These realms will allow a View to filter in more then just one users Activity.'),
    '#options' => $realms,
    '#default_value' => variable_get('activity_access_realms', array('comment')),
  );

  $form['activity_access']['activity_access_rebuild'] = array(
    '#type' => 'submit',
    '#value' => t('Rebuild Activity Access Table'),
    '#submit' => array('activity_access_batch_set'),
  );

  // This tells system_settings_form to use array_filter for the checkboxes.
  $form['array_filter'] = array('#type' => 'value', '#value' => TRUE);

  return system_settings_form($form);
}

/**
 * Element validate callback
 */
function activity_expire_validate($element, &$form_state) {
  if (empty($form_state['values']['activity_expire']) && !empty($form_state['values']['activity_min_count'])) {
    form_set_error($element['activity_expire'], t('You must set a time limit in order to use the minimum count'), $form_state);
  }
}

/**
 * Menu callback -- ask for confirmation of activity deletion
 */
function activity_delete_confirm(&$form_state, $aid) {
  $form['aid'] = array(
    '#type' => 'value',
    '#value' => $aid,
  );

  return confirm_form($form,
    t('Are you sure you want to delete this activity?'),
    $_GET['destination'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Execute activity deletion
 */
function activity_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    activity_delete($form_state['values']['aid']);
  }

  $form_state['redirect'] = '<front>';
}

/**
 * FAPI form submit function for the activity_rebuild button.
 *
 * @param array $form
 *  The FAPI form structure array.
 *
 * @param array &$form_state
 *  The FAPI form state array.
 *
 * @return none
 */
function activity_access_batch_set($form, &$form_state) {
  // Address usability concern and save the realms away
  variable_set('activity_access_realms', array_keys(array_filter($form_state['values']['activity_access_realms'])));
  
  $batch = array(
    'title' => t('Rebuilding Activity Access Table'),
    'operations' => array(
      array('activity_access_rebuild_process', array()),
    ),
    'file' => drupal_get_path('module', 'activity') . '/activity.admin.inc',
  );
  batch_set($batch);
  $form_state['redirect'] = 'admin/settings/activity';
}

/**
 * Batch API processing operation. Rebuilding Access table.
 *
 * @param array $context
 *  The batch api context array.
 *
 * @return none
 */
function activity_access_rebuild_process(&$context) {
  if (!isset($context['sandbox']['last_aid'])) {
    // Set up the sandbox for the first time.
    $context['sandbox']['last_aid'] = 0;
    $context['sandbox']['progress'] = 0;

    // Activity can be happening on the site. Any Activity happening after this point
    // will not be rebuilt. This is ok, as in that case, the new Activity will receive
    // any and all new Activity Access Realms.
    $context['sandbox']['max'] = db_result(db_query("SELECT COUNT(aid) FROM {activity}"));
  }
  // Process 100 Activities at a time.
  $limit = 100;

  $activities = db_query_range("SELECT * FROM {activity} WHERE aid > %d ORDER BY aid ASC", $context['sandbox']['last_aid'], 0, $limit);
  while ($activity = db_fetch_object($activities)) {
    $grants = activity_get_grants($activity);
    // Delete existing records.
    db_query("DELETE FROM {activity_access} WHERE aid = %d", $activity->aid);

    // Insert new ones.
    foreach ($grants as $realm => $values) {
      foreach ($values as $value) {
        // insert one by one. In D7 we can use the DBTNG to insert multiple
        $perm = new stdClass();
        $perm->aid = $activity->aid;
        $perm->realm = $realm;
        $perm->value = $value;
        drupal_write_record('activity_access', $perm);
      }
    }

    // Update sandbox variables.
    $context['sandbox']['last_aid'] = $activity->aid;
    $context['sandbox']['progress']++;
  }
  // Check if not finished.
  if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
  else {
    // If finished, delete the sandbox.
    unset($context['sandbox']);
  } 
}