<?php
// $Id: zenophile.drush.inc,v 1.1.4.2 2009/10/01 23:04:16 garrettalbright Exp $

/**
 * @file
 * Zenophile Drush commands.
 */

/**
 * Implementation of hook_drush_command().
 */
function zenophile_drush_command() {
  return array(
    'zenophile' => array(
      'callback' => 'zenophile_callback',
      'description' => 'Creates Zen subthemes quickly and easily.',
      'arguments' => array(
        'sysname' => 'The machine-compatible name of the new theme. This name may only consist of lowercase letters plus the underscore character.',
        'description' => 'A short description of this theme.',
      ),
      'options' => array(
        '--parent' => 'The parent theme for the new theme. Default: STARTERKIT',
        '--friendly' => 'A human-friendly name for the new theme. This name may contain uppercase letters, spaces, punctuation, etc. Default: Theme\'s sysname.',
        '--layout' => '"fixed" or "liquid". Default: fixed',
        '--site' => 'Which site directory will the new theme to be placed in? Default: all',
        '--fresh' => 'Adds a blank CSS file named "[theme_name]-fresh" to the new theme. Enabled by default; use "--fresh=0" to disable.',
        '--sidebar-left' => 'Left sidebar width, in pixels. Default: 200',
        '--sidebar-right' => 'Right sidebar width, in pixels. Default: 200',
        '--page' => 'Page wrapper (#page) width, in pixels. Ignored if --layout="liquid". Default: 960',
        '--sidebar-pos' => 'Sidebar positions. Possible values are "left" to place both sidebars to the left of the main content area; "right" to place both sidebars to the right of the main content area; and "normal" to have each sidebar on its respective side. Default: normal',
      ),
      'examples' => array(
        'zenophile foo_t "A great theme for Foo!"' => 'Create a new theme named "foo_t" and give it a description',
        // This example is great, but too long… Wish drush didn't try to cram
        // it all on one line.
/*         'zen bar_t "The best theme, bar none." --friendly="The Bar Theme" --layout="liquid" --site="bar.example.com"' => 'Create a new theme named "bar_t", give it a human-readable name and description, have it use a liquid layout, and put it in the "themes" subdirectory for the "bar.example.com" site directory. Note that the "zen" command is an abbreviated alias for the "zenophile" command.', */
      ),
    ),
    'zen' => array(
      'callback' => 'zenophile_callback',
      'description' => 'Alias for the "zenophile" command.',
    ),
  );
}

/**
 * Implementation of hook_drush_help().
 */

function zenophile_drush_help($section) {
  if ($section === 'drush:zenophile') {
    return dt('Creates Zen subthemes quickly and easily.');
  }
  elseif ($section === 'drush:zen') {
    return dt('Alias for the "zenophile" command. See the help documentation for the "zenophile" command for more information.');
  }
}

/**
 * Drush command callback.
 */
function zenophile_callback() {
  $args = func_get_args();
  $fake_form = array(
    'values' => array(
      'sysname' => $args[0],
      'description' => $args[1],
      'parent' => 'STARTERKIT',
      'friendly' => '',
      'layout' => 'fixed',
      'fresh' => TRUE,
      'site' => 'all',
      'sidebar-left' => '200',
      'sidebar-right' => '200',
      'page' => '960',
      'sidebar-pos' => 'normal',
    ),
  );
  
  foreach (array_keys($fake_form['values']) as $key) {
    $value = drush_get_option($key);
    if ($value !== NULL) {
      $fake_form['values'][$key] = stripslashes($value);
    }
  }
  drupal_execute('zenophile_create', $fake_form);
  // _drush_log_drupal_messages() does not log "status" messages, which is how
  // we try to tell the user that we were successful. So we'll log those
  // manually. First, save them in a var, because _drush_log_drupal_messages()'s
  // call to drupal_get_messages() is going to flush them.
  $messages = drupal_get_messages(NULL, FALSE);
  _drush_log_drupal_messages();
  if (isset($messages['status'])) {
    // There will probably never be more than one, but we'll iterate anyway.
    foreach ($messages['status'] as $msg) {
      drush_log(strip_tags($msg), 'success');
    }
  }
}