<?php
// $Id: activity_views_handler_filter_uid_access.inc,v 1.1.2.2 2009/08/26 19:10:28 scottreynolds Exp $

class activity_views_handler_filter_uid_access extends views_handler_filter {
  /**
   * tell Views u can't expose this
   */
  function can_expose() {
    return FALSE;
  }
  
  function query() {
    $this->ensure_my_table();
    $group = activity_views_group();
    if (is_null($group)) {
      // static cache the where group for the rest of the handlers
      $group = activity_views_group($this->query->set_where_group('OR'));
    }
    // tell it that you can have access to it as the actor
    $this->query->add_where($group, "($this->table_alias.$this->real_field = %d)", activity_views_access_user()->uid);
  }
}