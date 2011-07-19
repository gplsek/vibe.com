<?php

/**
 * Implementation of hook_imagecache_default_presets().
 */
function vibes_content_types_imagecache_default_presets() {
  $items = array(
    'image-full-size' => array(
      'presetname' => 'image-full-size',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '500',
            'height' => '',
            'upscale' => 0,
          ),
        ),
      ),
    ),
    'image-thumb' => array(
      'presetname' => 'image-thumb',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'jetpack',
          'action' => 'jetpack_imagecache_scale_and_crop',
          'data' => array(
            'width' => '100',
            'height' => '100',
          ),
        ),
      ),
    ),
    'w310' => array(
      'presetname' => 'w310',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '310px',
            'height' => '',
            'upscale' => 0,
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_node_info().
 */
function vibes_content_types_node_info() {
  $items = array(
    'audio' => array(
      'name' => t('Audio'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Caption'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'billboard' => array(
      'name' => t('Billboard'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'book' => array(
      'name' => t('Multipage post'),
      'module' => 'features',
      'description' => t('A <em>multipage post</em> is a page of content, organized into a collection of related entries.. A <em>multipage post</em> automatically displays links to adjacent pages, providing a simple navigation system for organizing and reviewing structured content.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'celebrity' => array(
      'name' => t('Celebrity'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Name'),
      'has_body' => '1',
      'body_label' => t('Bio'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'event' => array(
      'name' => t('Event'),
      'module' => 'features',
      'description' => t('some event data'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'img' => array(
      'name' => t('Image'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Caption'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'link' => array(
      'name' => t('Link'),
      'module' => 'features',
      'description' => t('Suggested links from an external source'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'page' => array(
      'name' => t('Page'),
      'module' => 'features',
      'description' => t('A <em>page</em>, similar in form to a <em>story</em>, is a simple method for creating and displaying information that rarely changes, such as an "About us" section of a website. By default, a <em>page</em> entry does not allow visitor comments and is not featured on the site\'s initial home page.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'post_up' => array(
      'name' => t('Post Up'),
      'module' => 'features',
      'description' => t('content from external sources/feeds'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'questions' => array(
      'name' => t('User Submitted Question'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Question'),
      'has_body' => '1',
      'body_label' => t('Answer'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'single_column_page' => array(
      'name' => t('Single Column Page'),
      'module' => 'features',
      'description' => t('creates a page with no right column'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'story' => array(
      'name' => t('Story'),
      'module' => 'features',
      'description' => t('A <em>story</em>, similar in form to a <em>page</em>, is ideal for creating and displaying content that informs or engages website visitors. Press releases, site announcements, and informal blog-like entries may all be created with a <em>story</em> entry. By default, a <em>story</em> entry is automatically featured on the site\'s initial home page, and provides the ability to post comments.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'video' => array(
      'name' => t('Video'),
      'module' => 'features',
      'description' => t('Video assets'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Caption'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
