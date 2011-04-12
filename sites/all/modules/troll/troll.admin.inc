<?php
// $Id: troll.admin.inc,v 1.1.2.12 2009/05/09 05:47:46 deekayen Exp $

/**
 * @file
 * Administrative forms and functions for troll module.
 */

/**
 * @return array
 */
function troll_admin_settings() {
  $form['ip_settings'] = array(
    '#type' => 'fieldset',
    '#title' => 'IP Address Banning'
  );
  $form['ip_settings']['troll_enable_ip_ban'] = array(
    '#type' => 'radios',
    '#title' => t('IP Address Banning'),
    '#default_value' => variable_get('troll_enable_ip_ban', 1),
    '#options' => array('1' => t('Enable banning by IP address'), '0' => t('Disable banning by IP address'))
  );
  $form['ip_settings']['troll_ip_ban_redirect'] = array(
    '#type' => 'textfield',
    '#title' => t('IP Ban Relocation Page'),
    '#default_value' => variable_get('troll_ip_ban_redirect', ''),
    '#description' => t("Page for relocating users banned based on their IP address or domain name.  If left blank, users will be redirected to blocked.html in the troll module's directory. Do not use a drupal path here! You will cause a loop since IP banning completely blocks all access to the site! Edit the blocked.html file, or redirect to http://localhost."),
  );

  $roles = user_roles();
  array_unshift($roles, t(' -Select Role- '));
  $form['role_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('User Blocking')
  );
  $form['role_settings']['troll_block_role'] = array(
    '#type' => 'select',
    '#title' => t('Troll Block Role'),
    '#default_value' => variable_get('troll_block_role', 0),
    '#options' => $roles,
    '#description' => t('Select the role to assign to users when blocking from the troll administration screens.'),
  );

  if (is_dir(drupal_get_path('module', 'troll') .'/php-ids')) {
    $form['ids_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('PHP-IDS')
    );
    $form['ids_settings']['troll_enable_phpids'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable PHP-IDS Notifications'),
      '#default_value' => variable_get('troll_enable_phpids', 0),
    );
  }

  return system_settings_form($form);
}

/**
 * Admin settings page validate handler.
 *
 * @see troll_admin_settings()
 */
function troll_admin_settings_validate($form, &$form_state) {
  if ($form_state['values']['troll_block_role'] == '0') {
    form_set_error('troll_block_role', t('You must choose a role to assign to users when blocking from the troll settings page.'));
  }
}

/**
 * Menu callback: user IP banning.
 */
function troll_ip_ban() {
  $form['banfieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add IP Ban'),
    '#weight' => -1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );
  $form['banfieldset']['banform'] = troll_ip_ban_form(array());
  $form['ipdisplay'] = array(
    '#type' => 'fieldset',
    '#title' => t('Banned IPs'),
    '#value' => troll_display_ip(),
    '#weight' => 0,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );
  $form['#validate'] = array('troll_ip_ban_form_validate');
  return $form;
}

/**
 * Submit handler for IP Ban form.
 *
 * @see troll_ip_ban()
 * @see troll_ip_ban_form()
 */
function troll_ip_ban_submit($form, &$form_state) {
  troll_insert_ip($form_state['values']);
}

/**
 * Summary of how many IP blocks are filtered.
 *
 * @return string
 */
function troll_blacklist_summary() {
  $count = db_result(db_query('SELECT COUNT(net) FROM {troll_blacklist}'));
  return t('%d address blocks filtered.', array('%d' => $count));
}

/**
 * Gives admins a choice of how to punish blacklisted visitors
 *
 * @return array
 */
function troll_blacklist_punishment_form() {
  $form['stutter'] = array(
    '#type' => 'checkbox',
    '#title' => t('Randomly stutter output'),
    '#default_value' => variable_get('troll_blacklist_stutter', 0),
    '#description' => t('While outputting content, the troll module will cause Drupal to "sleep" for random intervals of 1-5 seconds to delay output.')
  );
  $form['mod_requests'] = array(
    '#type' => 'radios',
    '#title' => t('Page request modification'),
    '#options' => array(t('none'), 'silent_post_drop' => 'Silently drop form post submission data', 'notice_post_drop' => 'Drop form post submission data and give a notice'),
    '#default_value' => variable_get('troll_blacklist_mod_requests', '0'),
    '#description' => t('Modifies data sent by visitors from blacklisted IPs before Drupal even has a chance to process it.')
  );
  $form['alt_page_output'] = array(
    '#type' => 'fieldset',
    '#title' => 'Alternate page output'
  );
  $form['alt_page_output']['alt_page'] = array(
    '#type' => 'radios',
    '#title' => 'Alternate pages',
    '#options' => array(t('none'), 'blank' => t('Blank pages'), '404' => t('404 every page request'), 'redirect' => t('Redirect to alternate URL')),
    '#default_value' => variable_get('troll_blacklist_alt_page', '0'),
    '#description' => t('Sends alternate page output, whether the visitor hits a real URL or not.')
  );
  $form['alt_page_output']['alt_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirection URL'),
    '#default_value' => variable_get('troll_blacklist_alt_url', ''),
    '#description' => t('URL to redirect blacklisted visitors to if "Redirect to alternate URL" is selected. Start with the appropriate prefix (e.g. http://)')
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update Blacklist Punishments'),
    '#weight' => 1
  );
  return $form;
}

/**
 * Submit handler for punishment form.
 *
 * @see troll_blacklist_punishment()
 */
function troll_blacklist_punishment_form_submit($form, &$form_state) {
  variable_set('troll_blacklist_mod_requests', $form_state['values']['mod_requests']);
  variable_set('troll_blacklist_stutter', $form_state['values']['stutter']);
  variable_set('troll_blacklist_alt_page', $form_state['values']['alt_page']);
  variable_set('troll_blacklist_alt_url', $form_state['values']['alt_url']);
  drupal_set_message(t('The settings have been updated.'));
}

/**
 * Builds form array for admin to select how to import a new blacklist.
 *
 * @return array
 */
function troll_blacklist_import_form() {
  $form['truncate_list'] = array(
    '#type' => 'checkbox',
    '#title' => t('Truncate/delete existing blacklist before import')
    );
  $options = array();
  $options[0] = '';
  if (function_exists('gzopen')) {
    $options['OCgz'] = 'OpenBSD mirror: okean.com China bzip archive';
    $options['OKgz'] = 'OpenBSD mirror: okean.com Korea bzip archive';
  }
  $options['OTtx'] = 'okean.com China and Korea text file';
  $options['OCtx'] = 'okean.com China text file';
  $options['OKtx'] = 'okean.com Korea text file';
  $form['select_list'] = array(
    '#type' => 'select',
    '#title' => t('Download and import'),
    '#options' => $options,
    '#description' => t('Downloads supported blacklist from the internet. Please select a mirror when possible.'),
  );
  $form['url_list'] = array(
    '#type' => 'textfield',
    '#title' => t('List URL'),
    '#description' => t('URL of a list to import. List files should have one address per line in Classless Internet Domain Routing CIDR format (x.x.x.x/x). Individual IPs should still have /32.'),
  );
  $form['custom_list'] = array(
    '#type' => 'textarea',
    '#rows' => 10,
    '#title' => t('List'),
    '#description' => t('List one address per line in Classless Internet Domain Routing CIDR format (x.x.x.x/x). Individual IPs should still have /32.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import List'),
    '#weight' => 1,
  );
  return $form;
}

/**
 * Submit handler for blacklist import.
 *
 * @see troll_blacklist_import()
 */
function troll_blacklist_import_form_submit($form, &$form_state) {
  if (!empty($form_state['values']['truncate_list'])) {
    if (db_query('TRUNCATE TABLE {troll_blacklist}')) {
      drupal_set_message(t('Blacklist table truncated.'));
    }
  }
  if (!empty($form_state['values']['select_list'])) {
    troll_blacklist_import_remote($form_state['values']['select_list']);
  }
  if (!empty($form_state['values']['url_list'])) {
    troll_blacklist_import_url($form_state['values']['url_list']);
  }
  if (!empty($form_state['values']['custom_list'])) {
    troll_blacklist_import_list($form_state['values']['custom_list']);
  }
  $form_state['redirect'] = 'admin/user/troll/ip_blacklist';
}

/**
 * Decyphers what dropdown selection was chosen to send for parsing out IP blocks
 *
 * @see troll_blacklist_import_form()
 * @see troll_blacklist_parse_save()
 * @param array $edit
 */
function troll_blacklist_import_remote($list) {
  $files = array(
    'OCgz' => array('gz' => 'http://www.openbsd.org/spamd/chinacidr.txt.gz'),
    'OKgz' => array('gz' => 'http://www.openbsd.org/spamd/koreacidr.txt.gz'),
    'OTtx' => array('txt' => 'http://www.okean.com/sinokoreacidr.txt'),
    'OCtx' => array('txt' => 'http://www.okean.com/chinacidr.txt'),
    'OKtx' => array('txt' => 'http://www.okean.com/koreacidr.txt'));

  if (!isset($files[$list])) {
    drupal_set_message(t('Form input not valid'));
    return FALSE;
  }

  $file_type = array_keys($files[$list]);
  troll_blacklist_parse_save($files[$list][$file_type[0]], $file_type[0]);
}

/**
 * Parses a URL to send for parsing out IP blocks
 *
 * @see troll_blacklist_parse_save()
 * @param array $edit
 */
function troll_blacklist_import_url($edit) {
  $url_parts = parse_url($edit['custom_list']);
  $file_parts = pathinfo($url_parts['path']);
  troll_blacklist_parse_save($edit['custom_list'], $file_parts['extension']);
}

function troll_blacklist_import_list($list) {
  $list = preg_split("/\s+/", $list);
  foreach ($list as $ip) {
    _troll_blacklist_insert_ip($ip);
  }
}

/**
 * Parses input file, line by line, searching for IP blocks in CIDR
 * format (x.x.x.x/x). Understands files in text, bzip, and bzip2 format,
 * where the PHP installation supports it. Writes what it finds to the
 * database as long integers.
 *
 * @todo should probably function_exists() the compression file functions
 * @param $file_name string
 * @param $file_type string
 */
function troll_blacklist_parse_save($file_name, $file_type) {
  switch ($file_type) {
    case 'gz':
      $fp = gzopen($file_name, 'r');
      break;
    default:
      $fp = fopen($file_name, 'r');
      break;
  }

  if (is_resource($fp)) {
    $i = 0;
    while (!feof($fp)) {
      $buffer = fgets($fp);
      if ($buffer{0} != '#' && ($buffer{0} != '/' && $buffer{1} != '/') && !empty($buffer)) {
        _troll_blacklist_insert_ip($buffer);
        $i++;
      }
    }
    switch ($file_type) {
      case 'gz':
        gzclose($fp);
        break;
      default:
        fclose($fp);
        break;
    }
    drupal_set_message(t('%i IP blocks imported', array('%i' => $i)));
  }
  else {
    drupal_set_message(t('Import failed! File could not be read.'));
  }
}

function _troll_blacklist_insert_ip($ip) {
  $netmasks = array(
    0, -2147483648, -1073741824, -536870912, -268435456, -134217728,
    -67108864, -33554432, -16777216, -8388608, -4194304, -2097152, -1048576,
    -524288, -262144, -131072, -65536, -32768, -16384, -8192, -4096, -2048,
    -1024, -512, -256, -128, -64, -32, -16, -8, -4, -2, -1);

  preg_match("/([0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3})\/([0-9]{1,2})/", $ip, $matches);
  // $matches array
  // 0 = whole string
  // 1 = i.i.i.i
  // 2 = s
  $longip = ip2long($matches[1]);
  unset($ip, $matches[0], $matches[1]);

  if ($matches[2] == 32) {
    $net = $bcast = $longip;
  }
  else {
    $net = $longip & $netmasks[$matches[2]];
    $bcast = $longip | ($netmasks[$matches[2]] ^ -1);
  }
  if (!empty($net)) {
    db_query('DELETE FROM {troll_blacklist} WHERE net = %f AND bcast = %f', $net, $bcast);
    return db_query('INSERT INTO {troll_blacklist} (net, bcast) VALUES (%f, %f)', $net, $bcast);
  }
  watchdog('troll', 'Malformed %ip not imported', array('%ip' => $ip), WATCHDOG_DEBUG);
  return FALSE;
}

/**
 * Form to search blacklist to see if an IP matches
 *
 * @return array
 */
function troll_blacklist_search_form($form_state) {
  $form['ip_address'] = array(
    '#type' => 'textfield',
    '#title' => t('IP Address'),
    '#size' => 15,
    '#maxlength' => 15,
    '#description' => t('Address to search for in the database of imported IP blocks.'),
    );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search Blacklisted IPs'),
    '#weight' => 1
  );
  $form['result'] = array(
    '#type' => 'fieldset',
    '#title' => t('Blacklisted IPs'),
    '#value' => !empty($form_state['values']['ip_address']) ? troll_blacklist_search($form_state['values']['ip_address']) : troll_blacklist_search(''),
    '#weight' => 2,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );
  return $form;
}

/**
 * Submit handler for blacklist search form.
 *
 * @see troll_blacklist_search_blacklist_form()
 */
function troll_blacklist_search_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * Perform search to see if an IP address matches a blacklisted IP block.
 *
 * @return string
 */
function troll_blacklist_search($ip_address) {
  $headers = array(
    array('data' => t('Network Address'), 'field' => 'net', 'sort' => 'asc'),
    array('data' => t('Broadcast Address'), 'field' => 'bcast'),
    array('data' => t('Actions'), 'field' => 'delete')
  );

  if ($ip_address == '') {
    $result = pager_query('SELECT net, bcast FROM {troll_blacklist}', 25, 0, 'SELECT COUNT(*) FROM {troll_blacklist}');
  }
  else {
    $sql = "SELECT net, bcast FROM {troll_blacklist} WHERE net <= %f AND bcast >= %f";
    $count_sql = 'SELECT COUNT(*) FROM {troll_blacklist} WHERE net <= %f AND bcast >= %f';
    $longip = _troll_longip($ip_address);
    $sql .= tablesort_sql($headers);
    $result = pager_query($sql, 25, 0, $count_sql, $longip, $longip);
  }

  while ($row = db_fetch_object($result)) {
    $printnet = long2ip($row->net);
    $printbcast = long2ip($row->bcast);
    $action = l(t('remove'), "admin/user/troll/ip_blacklist/deleteblack/{$row->net}/{$row->bcast}");
    $rows[] = array($printnet, $printbcast, $action);
  }
  if (!empty($rows)) {
    $pager = theme('pager', NULL, 25, 0);
    if (!empty($pager)) {
      $rows[] = array(array('data' => $pager, 'colspan' => 3));
    }
    return theme('table', $headers, $rows);
  }
  else {
    drupal_set_message(t('No matches found.'));
    return FALSE;
  }
}

/**
 * Form builder function
 *
 * @param $net string Not used, here for later implementation of whitelist editing
 * @param $bcast string Not used, here for later implementation of whitelist editing
 * @see troll_whitelist_form()
 * @return string
 */
function troll_whitelist($net = NULL, $bcast = NULL) {
  $sql = 'SELECT net, bcast FROM {troll_whitelist}';
  $count_sql = 'SELECT COUNT(*) FROM {troll_whitelist}';

  $headers = array(
    array('data' => t('Start/Network Address'), 'field' => 'net', 'sort' => 'asc'),
    array('data' => t('End/Broadcast Address'), 'field' => 'bcast'),
    array('data' => t('Actions'), 'field' => 'delete')
  );

  $sql .= tablesort_sql($headers);
  $result = pager_query($sql, 25, 0, $count_sql);
  while ($row = db_fetch_object($result)) {
    $printnet = long2ip($row->net);
    $printbcast = long2ip($row->bcast);
    $action = l(t('remove'), "admin/user/troll/ip_blacklist/deletewhite/{$row->net}/{$row->bcast}");
    $rows[] = array($printnet, $printbcast, $action);
  }

  $pager = theme('pager', NULL, 25, 0);
  if (!empty($pager)) {
    $rows[] = array(array('data' => $pager, 'colspan' => 3));
  }

  return theme('table', $headers, $rows);
}

/**
 * Display form for creating new whitelist block and table of current whitelisted IPs.
 *
 * @todo make this also useful for editing existing whitelist entries
 * @return array
 */
function troll_whitelist_form($form_state) {
  $form['whitelist_addr1'] = array(
    '#type' => 'textfield',
    '#title' => t('Starting IP Address'),
    '#size' => 15,
    '#maxlength' => 15,
    '#default_value' => isset($net) ? long2ip($net) : '',
    '#description' => t('IP or start to range of IPs to whitelist.'),
    '#required' => TRUE
  );
  $form['whitelist_addr2'] = array(
    '#type' => 'textfield',
    '#title' => t('Ending IP Address'),
    '#size' => 15,
    '#maxlength' => 15,
    '#default_value' => isset($bcast) ? long2ip($bcast) : '',
    '#description' => t('End of IP range to whitelist. If whitelisting a single IP, leave this blank.'),
    '#required' => false
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Insert Whitelist IPs'),
    '#weight' => 1
  );
  $form['whitelist'] = array(
    '#type' => 'fieldset',
    '#title' => t('Whitelisted IPs'),
    '#value' => troll_whitelist(),
    '#weight' => 2,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );
  return $form;
}

/**
 * Insert a new IP block into the whitelist
 *
 * @param array $edit
 */
function troll_whitelist_form_submit($form, &$form_state) {
  $whitelist_addr1 = $form_state['values']['whitelist_addr1'];
  $whitelist_addr2 = $form_state['values']['whitelist_addr2'];
  $longip1 = _troll_longip($whitelist_addr1);
  $longip2 = _troll_longip(empty($whitelist_addr2) ? $whitelist_addr1 : $whitelist_addr2);
  if ($longip1 > $longip2) {
    $temp = $longip1;
    $longip1 = $longip2;
    $longip2 = $temp;
    unset($temp);
  }
  db_query("REPLACE INTO {troll_whitelist} (net, bcast) VALUES (%f, %f)", $longip1, $longip2);
  drupal_set_message(t('IP range %ip1 to %ip2 whitelisted.', array('%ip1' => long2ip($longip1), '%ip2' => long2ip($longip2))));
}

/**
 * Menu callback: block a user and redirect to search page.
 *
 * @param $uid
 */
function troll_confirm_block_user_form($form_state, $uid) {
  $account = user_load(array('uid' => $uid));
  if (!$account) {
    drupal_goto('admin/user/troll');
  }
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $uid,
  );
  return confirm_form($form, t('Block user %username?', array('%username' => $account->name)), 'admin/user/troll', t('Are you sure you want to block this user?'));
}

function troll_confirm_block_user_form_submit($form, &$form_state) {
  troll_block_user($form_state['values']['uid']);
  $form_state['redirect'] = 'admin/user/troll';
}

/**
 * IP banning form.
 *
 * @param $iid int
 * @return array
 */
function troll_ip_ban_form($form_state, $iid = NULL) {
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => (isset($iid)) ? t('Update Banned IP') : t('Ban IP'),
    '#weight' => 1,
  );

  $ip = db_fetch_object(db_query('SELECT * FROM {troll_ip_ban} WHERE iid = %d', $iid));
  $form['iid'] = array(
    '#type' => 'value',
    '#value' => !empty($ip->iid) ? $ip->iid : '',
  );
  $form['ip_address'] = array(
    '#type' => 'textfield',
    '#title' => t('IP Address'),
    '#default_value' => !empty($ip->ip_address) ? $ip->ip_address : '',
    '#description' => t('The IP address to ban.'),
    '#required' => TRUE
  );
  $form['domain_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Domain Name'),
    '#default_value' => !empty($ip->domain_name) ? $ip->domain_name : (!empty($ip->ip_address) ? gethostbyaddr($ip->ip_address) : ''),
    '#description' => t('The Domain Name of the IP address to ban - for reference only.')
  );

  troll_ip_ban_expire_form($form, $ip);

  return $form;
}

/**
 * troll_ip_ban form validate callback function.
 *
 * @see troll_ip_ban_form()
 */
function troll_ip_ban_form_validate($form, &$form_state) {
  if (!preg_match('([0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3})', $form_state['values']['ip_address'])) {
    form_set_error('ip_address', t('Please include a valid IP address.'));
  }
}

/**
 * Submit handler for IP ban form.
 *
 * @see troll_ip_ban_form()
 */
function troll_ip_ban_form_submit($form, &$form_state) {
  troll_update_ip($form_state['values']);
  $form_state['redirect'] = 'admin/user/troll/ip_ban';
}

/**
 * Helper function to return the expire form elements.
 */
function troll_ip_ban_expire_form(&$form, $ip) {
  $timestamp = isset($ip->expires) ? $ip->expires : time();

  $date = getdate(gmmktime());
  $curyear = $date['year'];
  $i = 0;
  while ($i < 10) {
    $years[$curyear + $i] = $curyear + $i;
    $i++;
  }
  $months = array(1 => t('January'), t('February'), t('March'), t('April'), t('May'), t('June'), t('July'), t('August'), t('September'), t('October'), t('November'), t('December'));
  for ($i = 1; $i <= 31; $i++) {
    $days[$i] = $i;
  }

  $form['timestamp'] = array(
    '#type' => 'fieldset',
    '#title' => t('Expires'),
    '#prefix' => '<div class="container-inline"><div class="day">',
    '#suffix' => '</div></div>',
    '#description' => t('The ban will be removed after this day.')
  );
  $form['timestamp']['expires'] = array(
    '#type' => 'checkbox',
    '#default_value' => isset($ip->expires) ? TRUE : FALSE
  );
  $form['timestamp']['month'] = array(
    '#type' => 'select',
    '#default_value' => date('n', $timestamp),
    '#options' => $months
  );
  $form['timestamp']['day'] = array(
    '#type' => 'select',
    '#default_value' => date('j', $timestamp),
    '#options' => $days
  );
  $form['timestamp']['year'] = array(
    '#type' => 'select',
    '#default_value' => date('Y', $timestamp),
    '#options' => $years
  );
}

/**
 * IP ban information form.
 */
function troll_display_ip() {

  $rows = array();
  $sql = 'SELECT iid, ip_address, domain_name, expires, uid FROM {troll_ip_ban}';
  $count_sql = 'SELECT COUNT(*) FROM {troll_ip_ban}';

  $headers = array(
    array('data' => t('IP Address'), 'field' => 'ip_address'),
    array('data' => t('Domain Name'), 'field' => 'domain_name'),
    array('data' => t('Expires'), 'field' => 'expires', 'sort' => 'desc'),
    array('data' => t('Actions'), 'field' => 'delete')
  );

  $sql .= tablesort_sql($headers);
  $result = pager_query($sql, 25, 0, $count_sql);
  while ($row = db_fetch_object($result)) {
    $thisip = l($row->ip_address, 'admin/user/troll/ip_ban/edit/'. $row->iid);
    $thisdom = l(($row->domain_name ? $row->domain_name : gethostbyname($row->ip_address)), 'admin/user/troll/ip_ban/edit/'. $row->iid);
    $expires = ($row->expires ? date('M d, Y', $row->expires) : t('never'));
    $action = l(t('remove'), 'admin/user/troll/ip_ban/delete/'. $row->iid);
    $rows[] = array($thisip, $thisdom, $expires, $action);
  }

  $pager = theme('pager', NULL, 25, 0);
  if (!empty($pager)) {
    $rows[] = array(array('data' => $pager, 'colspan' => 5));
  }

  return theme('table', $headers, $rows);
}

/**
 * IP ban delete confirmation form.
 */
function troll_confirm_delete_ip_form($form_state, $iid) {
  $ip = db_fetch_object(db_query_range('SELECT ip_address FROM {troll_ip_ban} WHERE iid = %d', $iid, 0, 1));
  $form['iid'] = array(
    '#type' => 'value',
    '#value' => $iid,
  );
  return confirm_form($form, t('Remove Ban for IP %ip?', array('%ip' => $ip->ip_address)), 'admin/user/troll/ip_ban', t('Are you sure you want to remove the ban on this IP?'));
}

/**
 * Submit handler for delete confirmation form.
 *
 * @see troll_confirm_delete_ip_form()
 */
function troll_confirm_delete_ip_form_submit($form, &$form_state) {
  troll_remove_ip($form_state['values']['iid']);
  $form_state['redirect'] = 'admin/user/troll/ip_ban';
}

/**
 * IP ban confirmation form.
 */
function troll_confirm_ban_ip_form($form_state, $uid) {
  $ip = db_fetch_object(db_query_range('SELECT ip_address FROM {troll_ip_track} WHERE uid = %d ORDER BY accessed DESC', $uid, 0, 1));
  $form['ip_address'] = array(
    '#type' => 'value',
    '#value' => $ip->ip_address,
  );
  $form['domain_name'] = array(
    '#type' => 'value',
    '#value' => gethostbyaddr($ip->ip_address),
  );
  troll_ip_ban_expire_form($form, $ip);

  return confirm_form($form, t('Ban IP %ip?', array('%ip' => $ip->ip_address)), 'admin/user/troll/ip_ban', t('Are you sure you want to ban this IP?'));
}

/**
 * Submit handler for ban confirmation form.
 *
 * @see troll_confirm_ban_ip_form()
 */
function troll_confirm_ban_ip_form_submit($form, &$form_state) {
  troll_insert_ip($form_state['values']);
  $form_state['redirect'] = 'admin/user/troll';
}

/**
 * Confirmation form for deleting a blacklist IP block
 *
 * @param integer $net IP in long format
 * @param integer $bcast IP in long format
 */
function troll_confirm_delete_black_block_form($form_state, $net, $bcast) {
  $result = db_result(db_query('SELECT COUNT(net) FROM {troll_blacklist} WHERE net = %f AND bcast = %f', $net, $bcast));
  if (empty($result)) {
    drupal_set_message(t('No such IP range found in the database.'));
    drupal_goto('admin/user/troll/ip_blacklist/search');
  }
  $form['net'] = array(
    '#type' => 'value',
    '#value' => $net
  );
  $form['bcast'] = array(
    '#type' => 'value',
    '#value' => $bcast
  );
  return confirm_form(
    $form,
    t('Remove listing for IP block %ip1 to %ip2?', array('%ip1' => long2ip($net), '%ip2' => long2ip($bcast))),
    'admin/user/troll/ip_blacklist/search',
    t('Are you sure you want to remove this IP block from the blacklist?'),
    t('Confirm Blacklist Removal')
  );
}

/**
 * Submit handler for deletion of IP from blacklist.
 *
 * @see troll_confirm_delete_black_block_form()
 */
function troll_confirm_delete_black_block_form_submit($form, &$form_state) {
  troll_remove_blacklist($form_state['values']['net'], $form_state['values']['bcast']);
  $form_state['redirect'] = 'admin/user/troll/ip_blacklist/search';
}

/**
 * Confirmation form for deleting a whitelist IP block
 *
 * @param integer $net IP in long format
 * @param integer $bcast IP in long format
 */
function troll_confirm_delete_white_block_form($form_state, $net, $bcast) {
  $result = db_result(db_query('SELECT COUNT(net) FROM {troll_whitelist} WHERE net = %f AND bcast = %f', $net, $bcast));
  if (empty($result)) {
    drupal_set_message(t('No such IP range found in the database.'));
    drupal_goto('admin/user/troll/ip_blacklist/whitelist');
  }
  $form['net'] = array(
    '#type' => 'value',
    '#value' => $net
  );
  $form['bcast'] = array(
    '#type' => 'value',
    '#value' => $bcast
  );

  return confirm_form(
    $form,
    t('Remove listing for IP block %ip1 to %ip2?', array('%ip1' => long2ip($net), '%ip2' => long2ip($bcast))),
    'admin/user/troll/ip_blacklist/whitelist',
    t('Are you sure you want to remove this IP block from the whitelist?'),
    t('Confirm Whitelist Removal')
  );
}

/**
 * Submit handler for deletion of IP from whitelist.
 *
 * @see troll_confirm_delete_white_block_form()
 */
function troll_confirm_delete_white_block_form_submit($form, &$form_state) {
  troll_remove_whitelist($form_state['values']['net'], $form_state['values']['bcast']);
  $form_state['redirect'] = 'admin/user/troll/ip_blacklist/whitelist';
}

/**
 * User search form.
 *
 * @return array
 */
function troll_search_form($form_state) {
  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search Users'),
    '#collapsible' => TRUE
  );
  $form['search']['username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
  );
  $form['search']['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Email')
  );
  $form['search']['ip_address'] = array(
    '#type' => 'textfield',
    '#title' => t('IP Address')
  );
  $form['search']['date_created'] = array(
    '#type' => 'textfield',
    '#title' => t('Account Date Created'),
    '#description' => t('Enter date in mm/dd/yyyy format. Returns all users created after the date entered.')
  );
  $form['search']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search Users')
  );
  $form['users'] = array(
    '#type' => 'fieldset',
    '#title' => t('Users'),
    '#value' => isset($form_state['values']) ? troll_list_users($form_state['values']) : troll_list_users(array()),
    '#weight' => 2,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );
  return $form;
}

function troll_search_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * User list form.
 *
 * @param $edit array
 */
function troll_list_users($edit) {

  $args = array();
  $where[] = 'u.uid <> 0';

  if (!empty($edit['username'])) {
    $where[] = "LOWER(u.name) LIKE '%%%s%%' ";
    $args[] = drupal_strtolower($edit['username']);
  }
  if (!empty($edit['mail'])) {
    $where[] = "LOWER(u.mail) LIKE '%%%s%%' ";
    $args[] = drupal_strtolower($edit['mail']);
  }
  if (!empty($edit['ip_address'])) {
    $where[] = "LOWER(t.ip_address) LIKE '%%%s%%' ";
    $args[] = drupal_strtolower($edit['ip_address']);
  }
  if (!empty($edit['date_created'])) {
    $where[] = "u.created > %d ";
    $args[] = drupal_strtotime($edit['date_created']);
  }

  $sql = "SELECT u.uid, u.name, u.mail, u.status, t.ip_address, MAX(t.accessed) AS recorded, u.created FROM {users} u LEFT JOIN {troll_ip_track} t ON u.uid = t.uid";

  $sql .= ' WHERE '. implode(' AND ', $where);

  $headers = array(
    array('data' => t('Username'), 'field' => 'u.name'),
    array('data' => t('Email'), 'field' => 'u.mail'),
    array('data' => t('Status'), 'field' => 'u.status'),
    array('data' => t('IP Address'), 'field' => 't.ip_address'),
    array('data' => t('Last Access'), 'field' => 't.created'),
    array('data' => t('Account Created'), 'field' => 'u.created'),
    array('data' => t('Actions'), 'field' => 'actions')
  );

  $sql .= ' GROUP BY u.uid, u.name, u.mail, u.status, t.ip_address, u.created';
  $sql .= tablesort_sql($headers);

  $count_sql = 'SELECT COUNT(*) FROM {users} u LEFT JOIN {troll_ip_track} t ON u.uid = t.uid WHERE '. implode(' AND ', $where) .' AND u.uid <> 0';
  $result = pager_query($sql, 25, 0, $count_sql, $args);

  while ($user = db_fetch_object($result)) {
    $name = l($user->name, 'admin/user/troll/search/view/'. $user->uid, array('title' => t('View detailed user information')));
    $email = $user->mail;
    $status = ($user->status ? t('Active') : t('Blocked'));
    $ip = ($user->ip_address ? l($user->ip_address, 'admin/user/troll/search/view/'. $user->uid, array('title' => t('View detailed user information'))) : t('none'));
    $recorded = ($user->recorded ? date('M d, Y', $user->recorded) : t('none recorded'));
    $created = date('M d, Y', $user->created);
    $actions = array();

    if (!$user->status) {
      $actions[] = array('title' => t('Edit User'), 'href' => "user/{$user->uid}/edit");
    }
    elseif (variable_get('troll_block_role', NULL)) {
      $actions[] = array('title' => t('Block User'), 'href' => 'admin/user/troll/search/block/'. $user->uid);
    }
    else {
      $actions[] = array('title' => t('Set up Block Role'), 'href' => 'admin/user/troll/settings');
    }
    if ($user->ip_address) {
      $actions[] = array('title' => t('Ban IP'), 'href' => 'admin/user/troll/ip_ban/user/'. $user->uid);
    }
    $action = theme('links', $actions);
    $rows[] = array($name, $email, $status, $ip, $recorded, $created, $action);
  }

  $pager = theme('pager', NULL, 25);

  if (!empty($pager)) {
    $rows[] = array(array('data' => $pager, 'colspan' => 7));
  }

  return theme('table', $headers, $rows);
}

/**
 * User detail form.
 *
 * This is not an actual form; the form API is just being used for easy formatting.
 *
 * @param $uid int
 * @return string
 */
function troll_search_user_detail($uid) {
  $u = user_load(array('uid' => $uid));
  $u->ip = db_fetch_object(db_query('SELECT t.ip_address FROM {troll_ip_track} t WHERE t.uid = %d GROUP BY t.uid, t.ip_address', $uid));

  $form['details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Account Details for %username', array('%username' => $u->name))
  );

  $form['details'][] = array(
    '#type' => 'item',
    '#title' => t('User Name'),
    '#value' => theme('username', $u),
  );
  $form['details'][] = array(
    '#type' => 'item',
    '#title' => t('Email'),
    '#value' => $u->mail
  );
  $form['details'][] = array(
    '#type' => 'item',
    '#title' => t('User ID'),
    '#value' => $uid
  );
  $form['details'][] = array(
    '#type' => 'item',
    '#title' => t('Account Created'),
    '#value' => format_date($u->created, 'large')
  );
  $form['details'][] = array(
    '#type' => 'item',
    '#title' => t('Last Access'),
    '#value' => format_date($u->access, 'large')
  );
  $form['details'][] = array(
    '#type' => 'item',
    '#title' => t('Status'),
    '#value' => ($u->status ? t('Active') : t('Blocked'))
  );
  $form['details'][] = array(
    '#type' => 'item',
    '#title' => t('User Roles'),
    '#value' => implode(', ', $u->roles)
  );
  if ($u->status) {
    $links[] = array('title' => t('Block User'), 'href' => "admin/user/troll/search/block/$uid");
  }
  else {
    $links[] = array('title' => t('Edit User'), 'href' => "user/$uid/edit");
  }
  if ($u->ip->ip_address) {
    $links[] = array('title' => t('Ban IP'), 'href' => "admin/user/troll/ip_ban/user/$uid");
  }
  $form['details'][] = array(
    '#type' => 'item',
    '#title' => t('Actions'),
    '#value' => theme('links', $links)
  );

  $content = drupal_render($form);

  // Get IP history.
  $results = db_query('SELECT * FROM {troll_ip_track} WHERE uid = %d ORDER BY created DESC', $uid);
  while ($ip = db_fetch_object($results)) {
    $banned = db_fetch_object(db_query('SELECT * FROM {troll_ip_ban} WHERE ip_address = \'%s\'', $ip->ip_address));
    if (!empty($banned->ip_address)) {
      if ($banned->expires && $banned->expires < time()) {
        $status = l(t('expired'), 'admin/user/troll/ip_ban/edit/'. $banned->iid, array('title' => t('Edit IP ban information')));
      }
      else {
        $status = l(t('banned'), 'admin/user/troll/ip_ban/edit/'. $banned->iid, array('title' => t('Edit IP ban information')));
      }
    }
    else {
      $status = l(t('not banned'), 'admin/user/troll/ip_ban/user/'. $uid, array('title' => t('Ban this IP')));
    }
    $rows[] = array($ip->ip_address, $status, format_date($ip->accessed, 'large'), format_date($ip->created, 'large'), exec('host '. $ip->ip_address));
  }

  if ($rows) {
    $pheader = array(t('IP'), t('Status'), t('Last Access'), t('First Access'), t('Host Information'));
    $posts = theme('table', $pheader, $rows);
  }
  $content .= theme('box', t('IP History'), ($posts ? $posts : t('No ip history')));

  // Get recent posts.
  $rdat = db_query_range("SELECT * FROM {node} WHERE uid = %d ORDER BY created DESC", $uid, 0, 5);
  $posts = NULL;
  $rows = array();
  while ($node = db_fetch_object($rdat)) {
    $rows[] = array(l($node->title, "node/$node->nid") .' '. theme('mark', node_mark($node->nid, $node->changed)), $node->type, date('M d, Y', $node->created), ($node->status ? t('published') : t('not published')));
  }
  if ($rows) {
    $pheader = array(t('Title'), t('Type'), t('Created'), t('Status'));
    $posts = theme('table', $pheader, $rows);
  }

  $content .= theme('box', t('Recent Posts'), ($posts ? $posts : t('No recent posts')));

  if (module_exists('comment')) {
    // Get recent comments.
    $cp = 'SELECT * FROM {comments} WHERE uid = %d ORDER BY timestamp DESC';
    $cdat = db_query($cp, $uid);

    $rows = array();
    $posts = NULL;
    while ($comment = db_fetch_object($cdat)) {
      $rows[] = array(l($comment->subject, 'node/'. $comment->nid, array('fragment' => 'comment-'. $comment->cid)) .' '. theme('mark', node_mark($comment->nid, $comment->changed)), date('M d, Y', $comment->timestamp), ($comment->status ? t('not published') : t('published')));
    }

    if ($rows) {
      $cheader = array(t('Subject'), t('Date Created'), t('Status'));
      $posts = theme('table', $cheader, $rows);
    }
    $content .= theme('box', t('Recent Comments'), ($posts ? $posts : t('No recent comments')));
  }
  return $content;
}

function troll_dnsbl_settings() {
  $form = array();

  $form['troll_dnsbl_ip_test'] = array(
    '#type' => 'markup',
    '#value' => l(t('IP test'), 'admin/user/troll/dnsbl/test'),
  );
  $form['troll_dnsbl_active'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Operations'),
    '#default_value' => variable_get('troll_dnsbl_active', 0)
  );
  $form['troll_dnsbl_list'] = array(
    '#type' => 'textarea',
    '#title' => t('Servers to query'),
    '#default_value' => _troll_dnsbl_default_servers(),
    '#description' => t('DNS servers to query. Each on a separate line.')
  );
  $form['troll_dnsbl_threshold'] = array(
    '#type' => 'select',
    '#title' => t('How many servers should the IP be listed in'),
    '#default_value' => variable_get('troll_dnsbl_threshold', 1),
    '#description' => t('How many servers should list the IP before we unpublish its comment ?'),
    '#options' => drupal_map_assoc(range(1, 10)),
  );

  return system_settings_form($form);
}

function troll_dnsbl_test_form() {
  $form = array();

  $form['ip'] = array(
    '#type' => 'textfield',
    '#title' => t('IP to test'),
    '#size' => 15,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Test')
  );

  $form['#redirect'] = FALSE;

  return $form;
}

function troll_dnsbl_test_form_submit($form, &$form_state) {

  if (_troll_dnsbl_blacklisted($form_state['values']['ip'])) {
    $message = t('The IP %ip is blacklisted', array('%ip' => $form_state['values']['ip']));
  }
  else {
    $message = t('The IP %ip is not blacklisted', array('%ip' => $form_state['values']['ip']));
  }
  drupal_set_message($message);
}
