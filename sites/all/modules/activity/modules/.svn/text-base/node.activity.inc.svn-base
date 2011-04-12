<?php
// $Id: node.activity.inc,v 1.1.2.14 2009/09/22 15:26:16 scottreynolds Exp $

/**
 * @file
 * Activity definition file for node.module
 */

/**
 * Implementation of hook_activity_type_check().
 */
function node_activity_type_check($token_objects, $types) {
  return (in_array($token_objects['node']->type, $types));
}

/**
 * Implementation of hook_activity_token_list().
 */
function node_activity_token_list($type = 'all') {
  if ($type == 'node' || $type == 'all') {
    return array(
      'node' => array(
        'activity-node-link' => t('Link to the node'),
        'activity-node-creator-link' => t('Link to the node creator'),
      ),
    );
  }
}

/**
 * Implementation of hook_activity_token_values().
 */
function node_activity_token_values($type, $object = NULL, $options = array()) {
  if ($type == 'node') {
    return array(
      'activity-node-link' => l($object->title, "node/{$object->nid}"),
      'activity-node-creator-link' => theme('activity_username', $object),
    );
  }
}