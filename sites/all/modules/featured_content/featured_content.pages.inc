<?php
// $Id: featured_content.pages.inc,v 1.1.2.2 2010/06/04 20:17:14 kepol Exp $

/**
 * @file
 * Provides infrequently used pages for Featured Content.
 */

/**
 */
function featured_content_help_text($path, $arg) {
  $output = '';
  switch ($path) {
    case 'admin/build/block':
      $output = '<p>' . t('Use the <a href="@url">add Featured Content Block page</a> to create a customizable Featured Content Block. You will then be able to configure your Featured Content Block before adding it.', array('@url' => url('admin/build/block/add-featured-content-block'))) . '</p>';
      if (module_exists('help')) {
        $output .= theme('more_help_link', url('admin/help/featured_content'));
      }
      break;

    case 'admin/build/block/configure':
    case 'admin/build/block/add-featured-content-block':
      if (module_exists('help')) {
        $output = '<p>' . t('To learn more about configuring Featured Content blocks, see <a href="@url">Featured Content Block’s detailed help</a>.', array('@url' => url('admin/help/featured_content'))) . '</p>';
      }
      break;

    case 'admin/help#featured_content':
      $output =
        '<h3>' . t('Adding Featured Content Blocks') . '</h3>'
      . '<p>' . t('To add new Featured Content blocks, use the "Add Featured Content block" tab (or button) on the <a href="@url">administer blocks page</a>. You will then be able to configure your Featured Content block before adding it.', array('@url' => url('admin/build/block'))) . '</p>'
      . '<h3>' . t('Configuring Featured Content blocks') . '</h3>'
      . '<p>' . t('When adding or configuring a Featured Content block, several configuration options are available:') . '</p>'
      . '<dl>'
      . '<dt><strong>' . t('Block title') . '</strong></dt>'
      . '</dl>'
      . '<h3>' . t('Styling Featured Content Blocks') . '</h3>'
      . '<p>' . t('Themers should look at the classes added to the %div.', array('%div' => '<div>')) . '</p>'
      . '<p>' . t('In addition, the block is generated using the %template_default template.', array('%template_default' => 'featured_content-block.tpl.php')) . '</p>';
      break;
  }
  return $output;
}

/**
 * Menu callback; displays an RSS feed containing featured content entries
 * for given featured content block.
 */
function featured_content_feed() {
  $featured_content = featured_content_get_content(arg(2), 'rss');
  if (isset($featured_content->content)) {
    $channel['title'] = drupal_get_title();
    $channel['link'] = url($_GET['q'], array('absolute' => TRUE));
    node_feed($featured_content->content, $channel);
  }
  else {
    return $featured_content;
  }
}

/**
 * Menu callback; displays read more page containing featured content entries
 * for given featured content block.
 */
function featured_content_more_page() {
  $featured_content = featured_content_get_content(arg(2), 'more');
  if (isset($featured_content->content)) {
    return $featured_content->content;
  }
  return $featured_content;
}

/**
 * Get the content for rss or more page.
 */
function featured_content_get_content($delta, $type) {
  $featured_content = featured_content_view($delta, $type);
  if (empty($featured_content)) {
    return t('No results found.');
  }
  $data = new stdClass();
  $data->content = $featured_content;
  return $data;
}

