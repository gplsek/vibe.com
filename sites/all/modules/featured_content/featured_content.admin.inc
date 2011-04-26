<?php
// $Id: featured_content.admin.inc,v 1.1.2.16 2010/08/11 06:12:51 kepol Exp $

/**
 * @file
 * Provides infrequently used functions for featured content module.
 */

drupal_add_js(drupal_get_path('module', 'featured_content') . '/featured_content.js', 'module');
drupal_add_css(drupal_get_path('module', 'featured_content') . '/featured_content.css', 'module');

/**
 * Display add block form.
 */
function featured_content_add_block_form(&$form_state) {
  include_once './' . drupal_get_path('module', 'block') . '/block.admin.inc';
  return block_admin_configure($form_state, 'featured_content', NULL);
}

/**
 * Save new block.
 */
function featured_content_add_block_form_submit($form, &$form_state) {

  // determine delta of new block
  $block_ids = variable_get('featured_content_block_ids', array());
  $delta = empty($block_ids) ? 1 : max($block_ids) + 1;

  // save new array of blocks ids
  $block_ids[] = $delta;
  variable_set('featured_content_block_ids', $block_ids);

  // save block configuration
  featured_content_save($delta, $form_state['values']);

  // run normal new block submission (borrowed from block_add_block_form_submit)
  foreach (list_themes() as $key => $theme) {
    if ($theme->status) {
      db_query("INSERT INTO {blocks} (visibility, pages, custom, title, module, theme, status, weight, delta, cache) VALUES(%d, '%s', %d, '%s', '%s', '%s', %d, %d, %d, %d)", $form_state['values']['visibility'], trim($form_state['values']['pages']), $form_state['values']['custom'], $form_state['values']['title'], $form_state['values']['module'], $theme->name, 0, 0, $delta, BLOCK_NO_CACHE);
    }
  }
  foreach (array_filter($form_state['values']['roles']) as $rid) {
    db_query("INSERT INTO {blocks_roles} (rid, module, delta) VALUES (%d, '%s', '%s')", $rid, $form_state['values']['module'], $delta);
  }

  drupal_set_message(t('The Featured Content Block has been created.'));
  cache_clear_all();
  $form_state['redirect'] = 'admin/build/block';

  return;
}

/**
 * Confirm deletion of block.
 */
function featured_content_delete_block(&$form_state, $delta = 0) {
  $title = featured_content_format_title($delta);
  $form['block_title'] = array('#type' => 'hidden', '#value' => $title);
  $form['delta'] = array('#type' => 'hidden', '#value' => $delta);
  return confirm_form($form, t('Are you sure you want to delete the "%name" block?', array('%name' => $title)), 'admin/build/block', NULL, t('Delete'), t('Cancel'));
}

/**
 * Delete block.
 */
function featured_content_delete_block_submit($form, &$form_state) {

  // update block ids variable
  $delta = $form_state['values']['delta'];
  $block_ids = variable_get('featured_content_block_ids', array());
  unset($block_ids[array_search($delta, $block_ids)]);
  sort($block_ids);
  variable_set('featured_content_block_ids', $block_ids);

  // update blocks variable
  $featured_blocks = variable_get('featured_content_blocks', array());
  unset($featured_blocks[$delta]);
  variable_set('featured_content_blocks', $featured_blocks);

  // clear data in block tables
  db_query("DELETE FROM {blocks} WHERE module = 'featured_content' AND delta = %d", $delta);
  db_query("DELETE FROM {blocks_roles} WHERE module = 'featured_content' AND delta = %d", $delta);

  drupal_set_message(t('The "%name" block has been removed.', array('%name' => $form_state['values']['block_title'])));
  cache_clear_all();
  $form_state['redirect'] = 'admin/build/block';

  return;
}

/**
 * Returns 'list' info for hook_block().
 */
function featured_content_list() {
  $blocks = array();
  foreach (variable_get('featured_content_block_ids', array()) as $delta) {
    $blocks[$delta]['info'] = featured_content_format_title($delta);
    $blocks[$delta]['cache'] = BLOCK_NO_CACHE;
  }
  return $blocks;
}

/**
 * Format block title based on configuration.
 *
 * @param $delta int The delta of the block
 * @return string The title of the block
 */
function featured_content_format_title($delta) {
  $featured_content = featured_content_get_block_data($delta);
  $type = $featured_content['type'];
  $name = check_plain(featured_content_get_value($featured_content['name'], 'unnamed'));
  return sprintf('%s - %s - %s - %d', t('Featured Content Block'), $name, t(ucwords($type)), $delta);
}

/**
 * Returns 'configure' info for hook_block(). This is the featured block form.
 */
function featured_content_configure($delta) {

  $featured_content = featured_content_get_block_data($delta);
  $vocabularies = taxonomy_get_vocabularies();

  $form['wrapper-start'] = array('#value' => '<div id="featured-content-block-settings">');

  $form['featured-block'] = array(
    '#type' => 'fieldset',
    '#title' => t('Featured Content Block Settings'),
    '#attributes' => array('class' => 'featured-content-block'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['featured-block']['featured_content_block_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#maxlength' => 30,
    '#size' => 30,
    '#description' => t('Provide a descriptive name for internal use. This will
    show up on the !admin_block_page page.', array('!admin_block_page' =>
    l(t('admin/build/block'), 'admin/build/block'))),
      '#default_value' => $featured_content['name'],
      '#prefix' => '<div>' . t('Featured Content Blocks are blocks that show lists of content
      on your site. You can either manually choose what will be shown, or
      you can specify CCK fields or filters to use to determine what will be
      shown along side pages when they are being viewed.') . '</div>',
    );
  $form['featured-block']['featured_content_block_header'] = array(
    '#type' => 'textarea',
    '#title' => t('Header Text'),
    '#description' => t('Will show up between block title and content.'),
    '#rows' => 1,
    '#default_value' => $featured_content['header'],
  );
  $form['featured-block']['featured_content_block_footer'] = array(
    '#type' => 'textarea',
    '#title' => t('Footer Text'),
    '#description' => t('Will show up after content.'),
    '#rows' => 1,
    '#default_value' => $featured_content['footer'],
  );
  $form['featured-block']['featured_content_block_empty'] = array(
    '#type' => 'textarea',
    '#title' => t('Empty Text'),
    '#description' => t('This will show up all by itself if there is no content that
    can be found.'),
    '#rows' => 1,
    '#default_value' => $featured_content['empty'],
  );
  $form['featured-block']['featured_content_block_num_show'] = array(
    '#type' => 'textfield',
    '#title' => t('Number to Show'),
    '#description' => t('The number of content items to show. The maximum allowed
    is @max_num_show. If you enter an invalid number, @max_num_show will be
    used. To configure the maximum, go to the !admin_settings_page.',
    array('@max_num_show' => variable_get('featured_content_max_node_show', 20),
    '!admin_settings_page' => l(t('admin settings page'), 'admin/settings/featured_content'))),
    '#maxlength' => 3,
    '#size' => 2,
    '#default_value' => featured_content_get_value($featured_content['num_show'], variable_get('featured_content_max_node_show', 20)),
  );
  $sort_options = array(
    'alpha_asc' => t('Alphabetical - A to Z'),
    'alpha_desc' => t('Alphabetical - Z to A'),
    'date_desc' => t('Date Created - Newest First'),
    'date_asc' => t('Date Created - Oldest First'),
    'popular_desc' => t('Popularity (Number of Views) - Highest First'),
    'popular_asc' => t('Popularity (Number of Views) - Lowest First'),
    'random' => t('Randomly Chosen from All Available'),
    'terms_desc' => t('Closest Match to Current Page by Terms'),
    'terms_asc' => t('Furthest Match to Current Page by Terms'),
    'none' => t('None - Use Order Provided if Available'),
  );
  $form['featured-block']['featured_content_block_sort'] = array(
    '#type' => 'select',
    '#title' => t('Sort By'),
    '#options' => $sort_options,
    '#default_value' => featured_content_get_value($featured_content['sort'], 'alpha_asc'),
  );
  $display_options = array(
    'links' => t('Linked Titles'),
    'teasers' => t('Teasers/Summaries'),
    'full_nodes' => t('Full Node Content'),
  );
  $form['featured-block']['featured_content_block_display'] = array(
    '#title' => t('What to Display in the Block'),
    '#type' => 'select',
    '#options' => $display_options,
    '#default_value' => featured_content_get_value($featured_content['display'], 'links'),
    '#description' => t('You can choose to show only a list of the titles
    each linked to their own page, to show the teaser for each content item
    which will usually include the title (linked) and the teaser summary,
      or the full node content for each content item.'),
  );
  $style_options = array(
    'div' => t('None'),
    'ol' => t('Ordered List'),
    'ul' => t('Unordered List'),
  );
  $form['featured-block']['featured_content_block_style'] = array(
    '#title' => t('Display Style'),
    '#type' => 'select',
    '#options' => $style_options,
    '#default_value' => featured_content_get_value($featured_content['style'], 'ul'),
    '#description' => t('If you are showing a list of titles, you can choose
    to display them in an ordered or unordered list if desired.'),
  );
  $more_display_options = array(
    'none' => t('No Read More Page'),
    'links' => t('Linked Titles'),
    'teasers' => t('Teasers/Summaries'),
    'full_nodes' => t('Full Node Content'),
    'custom' => t('Link to the Custom Page Specified Below'),
  );
  $form['featured-block']['featured_content_block_more_display'] = array(
    '#title' => t('What to Display on Read More Page'),
    '#type' => 'select',
    '#options' => $more_display_options,
    '#default_value' => featured_content_get_value($featured_content['more']['display'], 'none'),
    '#description' => t('A "read more" link can be added that goes to a "read
    more page" that will show linked titles, teasers/summaries, or full nodes based
    on the chosen option.'),
  );
  $form['featured-block']['more'] = array(
    '#type' => 'fieldset',
    '#title' => t('Read More Page Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['featured-block']['more']['featured_content_block_more_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Read More Text'),
    '#description' => t('The text to show for the read more link if set.'),
    '#size' => 20,
    '#default_value' => featured_content_get_value($featured_content['more']['text'], t('Read more')),
  );
  $form['featured-block']['more']['featured_content_block_more_num'] = array(
    '#type' => 'textfield',
    '#title' => t('Number to Shown on Read More Page'),
    '#maxlength' => 2,
    '#size' => 2,
    '#default_value' => featured_content_get_value($featured_content['more']['num'], 20),
  );
  $form['featured-block']['more']['featured_content_block_more_style'] = array(
    '#title' => t('Read More Display Style'),
    '#type' => 'select',
    '#options' => $style_options,
    '#default_value' => featured_content_get_value($featured_content['more']['style'], 'ul'),
    '#description' => t('If you are showing a list of titles, you can choose
    to display them in an ordered or unordered list if desired.'),
  );
  $form['featured-block']['more']['featured_content_block_more_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Read More Title'),
    '#description' => t('Title shown on the read more page.'),
    '#default_value' => featured_content_get_value($featured_content['more']['title'], ''),
  );
  $form['featured-block']['more']['featured_content_block_more_header'] = array(
    '#type' => 'textarea',
    '#title' => t('Read More Header Text'),
    '#description' => t('Will show up between more title and more content.'),
    '#rows' => 1,
    '#default_value' => featured_content_get_value($featured_content['more']['header'], ''),
  );
  $form['featured-block']['more']['featured_content_block_more_footer'] = array(
    '#type' => 'textarea',
    '#title' => t('Read More Footer Text'),
    '#description' => t('Will show up after the more content.'),
    '#rows' => 1,
    '#default_value' => featured_content_get_value($featured_content['more']['footer'], ''),
  );
  $form['featured-block']['more']['featured_content_block_more_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom Read More Page'),
    '#description' => t('To link to a custom page, fill in the URL here and
    choose the custom option in the list above.'),
    '#default_value' => featured_content_get_value($featured_content['more']['url'], ''),
  );
  $form['featured-block']['featured_content_block_rss_display'] = array(
    '#type' => 'checkbox',
    '#title' => '<strong>' . t('Include RSS Feed') . '</strong>',
    '#description' => t('If checked, then there will be a small RSS icon
    shown at the bottom of the block that links to the RSS feed for the
    block content.'),
    '#default_value' => featured_content_get_value($featured_content['rss']['display'], FALSE),
  );
  $form['featured-block']['rss'] = array(
    '#type' => 'fieldset',
    '#title' => t('RSS Page Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['featured-block']['rss']['featured_content_block_rss_title'] = array(
    '#type' => 'textfield',
    '#title' => t('RSS Title'),
    '#description' => t('Title shown on the RSS page.'),
    '#default_value' => featured_content_get_value($featured_content['rss']['title'], ''),
  );

  $type_options = array();
  $type_options['manual'] = t('Manually-Selected Content');
  if (module_exists('content')) {
    $type_options['cck'] = t('CCK-Selected Content');
  }
  $type_options['filter'] = t('Filtered Content');

  $form['featured-block']['featured_content_block_type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => $type_options,
    '#default_value' => featured_content_get_value($featured_content['type'], 'manual'),
    '#prefix' => '<hr><div><br/>' . t('You can choose from 3 types of content blocks:
    Manually-Selected Content, CCK-Selected Content, and Filtered Content.
    When you choose a type, the type-specific settings form will change to
    show what you can provide for the type chosen.') . '</div>',
  );

  // featured content manual settings
  $form['featured-block']['manual'] = array(
    '#type' => 'fieldset',
    '#title' => t('Manually-Selected Content Settings'),
    '#attributes' => array('class' => 'featured-content-block-manual'),
    '#prefix' => '<div class="featured-content-block-settings">', // put on first setting fieldset
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['featured-block']['manual']['manual_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('URLs to Include'),
    '#description' => t('List the URLs for each page you want to include in
    the block, one per line. Do not include the domain. You can use
    the node/[nid] URL (e.g. node/123) or the path alias (e.g. some/page).
    Assumes all URLs are internal and correspond with a node page. If you
    include more that the configured number to show, then the URLs will be
    filtered out to only show the correct number. This can be useful for
    example if you want to use the random sort and end up showing a
    different combination of content items each page load.'),
    '#default_value' => $featured_content['manual']['paths'],
  );

  if (module_exists('content')) {
    // featured content cck settings
    $form['featured-block']['cck'] = array(
      '#type' => 'fieldset',
      '#title' => t('CCK-Selected Content Settings'),
      '#attributes' => array('class' => 'featured-content-block-cck'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#description' => t('Choose the CCK fields below. If the field is a
      text field, then the field data will be parsed and assumed to have
      a list of comma-delimited node ids. If the field is a node reference,
      the referenced node(s) will be used. If the field is a user reference,
      the referenced user(s) will be used to find content authored by
      those users.'),
    );
    $cck_fields = content_fields();
    if (! empty($cck_fields)) {
      foreach ($cck_fields as $field_name => $field_data) {
        $form['featured-block']['cck']['cck_fields_' . $field_name] = array(
          '#type' => 'checkbox',
          '#title' => sprintf('%s - %s (%s)', check_plain($field_data['widget']['label']), check_plain($field_name), $field_data['type']),
          '#default_value' => $featured_content['cck']['fields'][$field_name],
        );
      }
    }
    else {
      $form['featured-block']['cck']['cck_fields'] = array(
        '#type' => 'markup',
        '#value' => '<div>' . t('There are no CCK fields defined. Once you add
        CCK fields, you can choose them here.') . '</div>',
      );
    }
  }

  // featured content filter settings
  $form['featured-block']['filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filtered Content Settings'),
    '#attributes' => array('class' => 'featured-content-block-filter'),
    '#suffix' => '</div>', // put on last setting fieldset
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['featured-block']['filter']['filter_include_node'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include Node Being Viewed'),
    '#description' => t('If checked, then the node being viewed can be
    included in the block. It may not always show up depending on the
    settings. For example, if random display is chosen, then the current
    node may not always show up in the block.'),
    '#default_value' => $featured_content['filter']['include_node'],
  );

  if (module_exists('views') && function_exists('views_get_all_views')) {
    $views = views_get_all_views();
    $views_names = array_keys($views);
    asort($views_names);
    if (! empty($views)) {
      $form['featured-block']['filter']['views'] = array(
        '#type' => 'fieldset',
        '#title' => t('Views Filter Options'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#description' => t('Select any of the views below if you want the
        resulting nodes from the views to show up in your block.'),
      );
      $form['featured-block']['filter']['views']['prefix'] = array(
        '#type' => 'markup',
        '#value' => '<div class="fc-scrollable-checkboxes">',
      );
      foreach ($views_names as $view_name) {
        $view_data = $views[$view_name];
        if (! empty($view_data->display)) {
          foreach ($view_data->display as $display_name => $display_data) {
            $form['featured-block']['filter']['views']['filter_views_view_' . $view_name . '_' . $display_name] = array(
              '#type' => 'checkbox',
              '#title' => sprintf('%s - %s', $view_name, $display_name),
              '#default_value' => $featured_content['filter']['views']['view'][$view_name][$display_name],
            );
          }
        }
      }
      $form['featured-block']['filter']['views']['suffix'] = array(
        '#type' => 'markup',
        '#value' => '</div>',
      );
    }
  }

  $form['featured-block']['filter']['paths'] = array(
    '#type' => 'fieldset',
    '#title' => t('Path Filter Options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('Select any of the checkboxes below if you want to
    get content based on the path of the current page being viewed. For example,
    if the current page is www.mysite.com/first/second/third/fourth and you
      choose to match on the second and third parts of the path, then pages
      with paths like www.mysite.com/foo/second/third and like
      www.mysite.com/hello/second/third/world and like
      www.mysite.com/a/second/third/b/c/d would match.'),
  );
  $form['featured-block']['filter']['paths']['filter_paths_match_0'] = array(
    '#type' => 'checkbox',
    '#title' => t('Match first part of path (e.g. first/any).'),
    '#default_value' => $featured_content['filter']['paths']['match'][0],
  );
  $form['featured-block']['filter']['paths']['filter_paths_match_1'] = array(
    '#type' => 'checkbox',
    '#title' => t('Match second part of path (e.g. any/second/any).'),
    '#default_value' => $featured_content['filter']['paths']['match'][1],
  );
  $form['featured-block']['filter']['paths']['filter_paths_match_2'] = array(
    '#type' => 'checkbox',
    '#title' => t('Match third part of path (e.g. any/any/third/any).'),
    '#default_value' => $featured_content['filter']['paths']['match'][2],
  );
  $form['featured-block']['filter']['paths']['filter_paths_match_3'] = array(
    '#type' => 'checkbox',
    '#title' => t('Match fourth part of path (e.g. any/any/any/fourth/any).'),
    '#default_value' => $featured_content['filter']['paths']['match'][3],
  );

  $form['featured-block']['filter']['content_types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content Type Filter Options'),
    '#description' => t('Select the content types to use below. If no content types are selected, all will be used. If the first checkbox is checked, then content with the same content type as the page being viewed will be used.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['featured-block']['filter']['content_types']['prefix'] = array(
    '#type' => 'markup',
    '#value' => '<div class="fc-scrollable-checkboxes">',
  );
  $content_types = featured_content_get_content_types();
  foreach ($content_types as $content_type_name => $content_type_label) {
    $form['featured-block']['filter']['content_types']['filter_content_types_type_' . $content_type_name] = array(
      '#type' => 'checkbox',
      '#title' => sprintf('%s', $content_type_label),
      '#default_value' => $featured_content['filter']['content_types']['type'][$content_type_name],
    );
  }
  $form['featured-block']['filter']['content_types']['suffix'] = array(
    '#type' => 'markup',
    '#value' => '</div>',
  );

  $form['featured-block']['filter']['users'] = array(
    '#type' => 'fieldset',
    '#title' => t('User (Author) Filter Options'),
    '#description' => t('Select the users to use below. If no users are selected, all will be used. If the first checkbox is checked, then the user (author) of the page being viewed will be used.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['featured-block']['filter']['users']['prefix'] = array(
    '#type' => 'markup',
    '#value' => '<div class="fc-scrollable-checkboxes">',
  );
  $users = featured_content_get_users();
  foreach ($users as $user_uid => $user_label) {
    $form['featured-block']['filter']['users']['filter_users_user_' . $user_uid] = array(
      '#type' => 'checkbox',
      '#title' => sprintf('%s', $user_label),
      '#default_value' => $featured_content['filter']['users']['user'][$user_uid],
    );
  }
  $form['featured-block']['filter']['users']['suffix'] = array(
    '#type' => 'markup',
    '#value' => '</div>',
  );

  if (sizeof($vocabularies) > 0) {
    $form['featured-block']['filter']['vocab'] = array(
      '#type' => 'fieldset',
      '#title' => t('Vocabularies'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    foreach ($vocabularies as $vocabulary) {
      $vid = $vocabulary->vid;
      $terms = array();
      foreach (taxonomy_get_tree($vid) as $term) {
        $terms[$term->tid] = check_plain($term->name);
      }
      if (! empty($terms)) {
        $form['featured-block']['filter']['vocab'][$vid] = array(
          '#type' => 'fieldset',
          '#title' => t('@vocab_name Terms Filter Options', array('@vocab_name' => $vocabulary->name)),
          '#description' => t('Select the terms to use below. If no terms are selected, all will be used. If the first checkbox is checked, then content with the same terms as the page being viewed will be used.'),
          '#collapsible' => TRUE,
          '#collapsed' => FALSE,
        );
        $terms = featured_content_get_vocabulary_terms($vocabulary);
        $form['featured-block']['filter']['vocab'][$vid]['prefix'] = array(
          '#type' => 'markup',
          '#value' => '<div class="fc-scrollable-checkboxes">',
        );
        foreach ($terms as $term_tid => $term_name) {
          $form['featured-block']['filter']['vocab'][$vid]['filter_vocab_term_' . $term_tid] = array(
            '#type' => 'checkbox',
            '#title' => sprintf('%s', $term_name),
            '#default_value' => $featured_content['filter']['vocab'][$vid]['term'][$term_tid],
          );
        }
        $form['featured-block']['filter']['vocab'][$vid]['suffix'] = array(
          '#type' => 'markup',
          '#value' => '</div>',
        );
      }
    }
  }

  if (module_exists('primary_term')) {
    $form['featured-block']['filter']['primary_term'] = array(
      '#type' => 'fieldset',
      '#title' => t('Primary Term Filter Options'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['featured-block']['filter']['primary_term']['filter_primary_term_node'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use the primary term of the page being shown.'),
      '#description' => t('If checked, then content with the same primary term as the page being viewed will be shown.'),
      '#default_value' => $featured_content['filter']['primary_term']['node'],
    );
  }

  // featured content visibility settings
  $form['featured-block']['visibility'] = array(
    '#type' => 'fieldset',
    '#title' => t('Featured Content Visibility Settings'),
    '#attributes' => array('class' => 'featured-content-block-visibility'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<hr>',
  );
  $form['featured-block']['visibility']['content_types']['visibility_content_types_selected'] = array(
    '#type' => 'select',
    '#title' => 'Content Type Visibility Settings',
    '#options' => $content_types,
    '#multiple' => TRUE,
    '#description' => t('If no content types are selected, block visibility
    is not affected. If content types are selected, then the block will
    only be shown if the current page being shown is one of the selected
    content types.'),
    '#default_value' => $featured_content['visibility']['content_types']['selected'],
    '#prefix' => '<div>' . t('The following block visibility settings can be
    used in <i>conjunction</i> with the regular <i>user-specific</i>,
    <i>role-specific</i>, and <i>page-specific</i> block visibility settings.
    The module will check the current page being shown and, if it is a node
    view page, will use the settings to determine whether or not the block
    should be shown for that page. <strong>Note:</strong> If you use these
    visibility settings, they will be "ANDed" with the other visibility
    settings. For example, if you choose to show the block for the content
    type "blog" and you choose the <i>page specific</i> setting to show the
    block for pages with aliases like "*topic*" then the block will only be
    shown if the node\'s content type is "blog" AND the node\'s alias
    contains "topic" in it.') . '</div>',
  );
  $form['featured-block']['visibility']['users']['visibility_users_selected'] = array(
    '#type' => 'select',
    '#title' => 'User (Author) Visibility Settings',
    '#options' => $users,
    '#multiple' => TRUE,
    '#default_value' => $featured_content['visibility']['users']['selected'],
    '#description' => t('If no users are selected, block visibility
    is not affected. If users are selected, then the block will
    only be shown if the author of the current page being shown is one of
    the selected users.'),
  );
  if (sizeof($vocabularies) > 0) {
    $form['featured-block']['visibility']['vocab'] = array(
      '#type' => 'fieldset',
      '#title' => t('Taxonomy Visibility Settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    foreach ($vocabularies as $vocabulary) {
      $vid = $vocabulary->vid;
      $terms = array();
      foreach (taxonomy_get_tree($vid) as $term) {
        $terms[$term->tid] = check_plain($term->name);
      }
      if (! empty($terms)) {
        $form['featured-block']['visibility']['vocab'][$vid]['visibility_vocab_' . $vid . '_selected'] = array(
          '#type' => 'select',
          '#title' => t('@vocab_name Visibility Settings', array('@vocab_name' => $vocabulary->name)),
          '#options' => $terms,
          '#multiple' => TRUE,
          '#default_value' => $featured_content['visibility']['vocab'][$vid]['selected'],
          '#description' => t('If no terms are selected, block visibility
          is not affected. If terms are selected, then the block will
          only be shown if the current page being shown has one of
          the selected terms.'),
        );
      }
    }
  }

  $form['wrapper-close'] = array('#value' => '</div>');
  return $form;
}

/**
 * Returns the 'save' $op info for hook_block().
 */
function featured_content_save($delta, $edit) {

  $featured_blocks = variable_get('featured_content_blocks', array());
  $vocabularies = taxonomy_get_vocabularies();

  // general settings
  if (! is_array($featured_blocks[$delta])) {
    $featured_blocks[$delta] = array();
  }
  if (! is_array($featured_blocks[$delta]['more'])) {
    $featured_blocks[$delta]['more'] = array();
  }
  if (! is_array($featured_blocks[$delta]['rss'])) {
    $featured_blocks[$delta]['rss'] = array();
  }
  $featured_blocks[$delta]['name'] = $edit['featured_content_block_name'];
  $featured_blocks[$delta]['header'] = $edit['featured_content_block_header'];
  $featured_blocks[$delta]['footer'] = $edit['featured_content_block_footer'];
  $featured_blocks[$delta]['empty'] = $edit['featured_content_block_empty'];
  $featured_blocks[$delta]['type'] = $edit['featured_content_block_type'];
  $featured_blocks[$delta]['sort'] = $edit['featured_content_block_sort'];
  $featured_blocks[$delta]['display'] = $edit['featured_content_block_display'];
  $featured_blocks[$delta]['style'] = $edit['featured_content_block_style'];
  $featured_blocks[$delta]['more']['text'] = $edit['featured_content_block_more_text'];
  $featured_blocks[$delta]['more']['num'] = $edit['featured_content_block_more_num'];
  $featured_blocks[$delta]['more']['display'] = $edit['featured_content_block_more_display'];
  $featured_blocks[$delta]['more']['style'] = $edit['featured_content_block_more_style'];
  $featured_blocks[$delta]['more']['title'] = $edit['featured_content_block_more_title'];
  $featured_blocks[$delta]['more']['header'] = $edit['featured_content_block_more_header'];
  $featured_blocks[$delta]['more']['footer'] = $edit['featured_content_block_more_footer'];
  $featured_blocks[$delta]['more']['url'] = $edit['featured_content_block_more_url'];
  $featured_blocks[$delta]['rss']['display'] = $edit['featured_content_block_rss_display'];
  $featured_blocks[$delta]['rss']['title'] = $edit['featured_content_block_rss_title'];

  // if sort is set to popularity, print warning
  if ($edit['featured_content_block_sort'] == 'popular_asc' ||
    $edit['featured_content_block_sort'] == 'popular_desc') {
      if (featured_content_node_statistics_enabled()) {
        drupal_set_message(t('You have chosen to sort by popularity. Since the
          Statistics module is turned on and Count Content Views is enabled,
          this will work as expected. If you disable either of these in the
          future, the content items will no longer be sorted by popularity.'));
      }
      else {
        if (! module_exists('statistics')) {
          drupal_set_message(t('You have chosen to sort by popularity, but the
            Statistics module is currently turned off. Turn on the Statistics
            module (!admin_modules_page) and enable Count Content Views
            (!statistics_settings_page) so the content items can be sorted
            by popularity.', array(
              '!admin_modules_page' =>
              l(t('admin/build/modules'), 'admin/build/modules'),
                '!statistics_settings_page' =>
                l(t('admin/reports/settings'), 'admin/reports/settings')
              )
            ));
        }
        elseif (! variable_get('statistics_count_content_views', 0)) {
          drupal_set_message(t('You have chosen to sort by popularity. Although
            the Statistics module is currently turned on, Count Content Views
            is currently disabled. Enable Count Content Views
            (!statistics_settings_page) so the content items can be sorted
            by popularity.', array(
              '!statistics_settings_page' =>
              l(t('admin/reports/settings'), 'admin/reports/settings')
            )
          ));
        }
      }
    }

  $num_show = $edit['featured_content_block_num_show'];
  if (! is_numeric($num_show) || $num_show <= 0 || $num_show > variable_get('featured_content_max_node_show', 20)) {
    $num_show = variable_get('featured_content_max_node_show', 20);
    if (! is_numeric($num_show) || $num_show <= 0) {
      $num_show = 20;
    }
  }
  $featured_blocks[$delta]['num_show'] = $num_show;

  // manual settings
  $featured_blocks[$delta]['manual']['paths'] = $edit['manual_paths'];
  $tmp = preg_replace('/(\r\n?|\n)/', '|||', $edit['manual_paths']);
  $paths = explode('|||', $tmp);
  foreach ($paths as $path) {
    $path = trim($path);
    if ($path) {
      if (! valid_url($path, TRUE)) { // check for external URLs
        // assume it's an internal URL
        $path = drupal_get_normal_path($path);
      }
      $matches = array();
      if (preg_match("/^node\/(.*)/", $path, $matches)) {
        $nids[] = $matches[1];
      }
    }
  }
  $featured_blocks[$delta]['manual']['nids'] = $nids;

  // cck settings
  if (module_exists('content')) {
    $cck_fields = content_fields();
    if (! empty($cck_fields)) {
      foreach ($cck_fields as $field_name => $field_data) {
        $featured_blocks[$delta]['cck']['fields'][$field_name] = $edit['cck_fields_' . $field_name];
      }
    }
  }

  // filter settings
  $featured_blocks[$delta]['filter']['include_node'] = $edit['filter_include_node'];
  $featured_blocks[$delta]['filter']['paths']['match'][0] = $edit['filter_paths_match_0'];
  $featured_blocks[$delta]['filter']['paths']['match'][1] = $edit['filter_paths_match_1'];
  $featured_blocks[$delta]['filter']['paths']['match'][2] = $edit['filter_paths_match_2'];
  $featured_blocks[$delta]['filter']['paths']['match'][3] = $edit['filter_paths_match_3'];

  $content_types = featured_content_get_content_types();
  foreach ($content_types as $content_type_name => $content_type_label) {
    $featured_blocks[$delta]['filter']['content_types']['type'][$content_type_name] = $edit['filter_content_types_type_' . $content_type_name];
  }
  $users = featured_content_get_users();
  foreach ($users as $user_uid => $user_label) {
    $featured_blocks[$delta]['filter']['users']['user'][$user_uid] = $edit['filter_users_user_' . $user_uid];
  }

  foreach ($vocabularies as $vocabulary) {
    $vid = $vocabulary->vid;
    $terms = featured_content_get_vocabulary_terms($vocabulary);
    foreach ($terms as $term_tid => $term_name) {
      $featured_blocks[$delta]['filter']['vocab'][$vid]['term'][$term_tid] = $edit['filter_vocab_term_' . $term_tid];
    }
  }
  $featured_blocks[$delta]['filter']['primary_term']['node'] = $edit['filter_primary_term_node'];

  if (module_exists('views') && function_exists('views_get_all_views')) {
    $views = views_get_all_views();
    if (! empty($views)) {
      foreach ($views as $view_name => $view_data) {
        if (! empty($view_data->display)) {
          foreach ($view_data->display as $display_name => $display_data) {
            $featured_blocks[$delta]['filter']['views']['view'][$view_name][$display_name] = $edit['filter_views_view_' . $view_name . '_' . $display_name];
          }
        }
      }
    }
  }

  // visibility settings
  $featured_blocks[$delta]['visibility']['content_types']['selected'] = $edit['visibility_content_types_selected'];
  $featured_blocks[$delta]['visibility']['users']['selected'] = $edit['visibility_users_selected'];
  foreach ($vocabularies as $vocabulary) {
    $vid = $vocabulary->vid;
    $featured_blocks[$delta]['visibility']['vocab'][$vid]['selected'] = $edit['visibility_vocab_' . $vid . '_selected'];
  }

  variable_set('featured_content_blocks', $featured_blocks);
}

/**
 * Admin settings form.
 */
function featured_content_settings_form(&$form_state) {
  $form = array();
  $form['featured_content_max_node_show'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Allowed Number of Nodes Shown'),
    '#description' => t('The number of nodes shown is configurable when
    creating blocks, but the number specified here will be the maximum
    allowed overall.'),
    '#default_value' => variable_get('featured_content_max_node_show', 100),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}
