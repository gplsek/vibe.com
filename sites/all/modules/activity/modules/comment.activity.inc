<?php
// $Id: comment.activity.inc,v 1.1.2.18 2009/10/29 18:20:51 scottreynolds Exp $

/**
 * @file
 * Activity definition file for comment.module
 */

/**
 * Implementation of hook_activity_grants().
 */
function comment_activity_grants($activity) {
  $grants = array();
  if (!empty($activity->nid)) {
    // tell activity to record 'comment' | nid
    $grants = array(
      'comment' => array($activity->nid),
    );
  }
  return $grants;
}

/**
 * Implementation of hook_activity_access_grants().
 */
function comment_activity_access_grants($account) {
  // tell activity what activities this user has based on the 'comment' realm
  $grants = array();
  
  // select the nodes you have created
  $created_result = db_query("SELECT nid FROM {node} WHERE uid = %d", $account->uid);
  while ($created = db_fetch_object($created_result)) {
    $grants['comment'][] = $created->nid;
  }
  
  // select the nodes you commented on
  $commented_result = db_query("SELECT nid FROM {comments} WHERE uid = %d", $account->uid);
  while ($commented = db_fetch_object($commented_result)) {
    $grants['comment'][] = $commented->nid;
  }
  return $grants;
}

/**
 * Implementation of hook_activity_type_check().
 */
function comment_activity_type_check($token_objects, $types) {
  return (in_array($token_objects['node']->type, $types));
}

/**
 * Implementation of hook_activity_token_list().
 */
function comment_activity_token_list($type = 'all') {
  if ($type == 'comment' || $type == 'all') {
    return array(
      'comment' => array(
        'activity-comment-author-link' => t('Link to the author of the comment'),
        'comment-link' => t('Link to the comment'),
      ),
    );
  }
}

/**
 * Implementation of hook_activity_token_values().
 */
function comment_activity_token_values($type, $object = NULL, $options = array()) {
  if ($type == 'comment') {
    // This comes 'free' out of the static cache
    $node = node_load($object->nid);
    $page = comment_new_page_count($node->comment_count + 1, 1, $node);
    return array(
      'activity-comment-author-link' => theme('activity_username', $object),
      'comment-link' => l(check_plain($object->subject), 'node/' . $object->nid, array('fragment' => 'comment-' . $object->cid, 'query' => $page)),
    );
  }
}

/**
 * Implementation of hook_activity_objects_alter().
 */
function comment_activity_objects_alter(&$objects) {
  // if the comment and the node are the same author we
  // have to allow that semantics
  if (isset($objects['comment']) && $objects['node']->uid == $objects['comment']->uid) {
    $objects['node_comment_author'] = $objects['comment'];
  }
}