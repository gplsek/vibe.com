<?php
// $Id: disqus.views.inc,v 1.1.2.4 2009/08/27 23:40:14 robloach Exp $

/**
 * Implementation of hook_views_data().
 */
function disqus_views_data() {
  // Number of Disqus comments made on the given node.
  $data['node']['disqus_comments'] = array(
    'field' => array(
      'title' => t('Disqus: Comments'),
      'help' => t('The number of Disqus comments made on the node.'),
      'handler' => 'views_handler_field_node_disqus_comments',
    ),
  );
  return $data;
}

/**
 * Field handler to present the number of Disqus comments on a node.
 */
class views_handler_field_node_disqus_comments extends views_handler_field_node_link {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }

  /**
   * When rendering the field.
   */
  function render($values) {
    // Ensure Disqus comments are available on the node user has access to edit this node.
    $node = node_load($this->aliases['nid']);
    if (user_access('view disqus comments') && isset($node->disqus)) {
      return theme('disqus_comments_num', $node->disqus);
    }
  }
}
