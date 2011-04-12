<?php

class views_modify_query_filter_handler extends views_handler_filter {
  // Don't display empty space where the operator would be.
  var $no_operator = true;
  
  // Empty operator form
  function operator_form(&$form, &$form_state) {
    $form['operator'] = array();
  }
  
  // Don't expose this, it's dangerous enough as it is
  function can_expose() {
    return false;
  }
  
  // This is the value form - basically just a textarea.
  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'textarea',
      '#title' => t('PHP Code'),
      '#description' => t('This gets executed in the context of the "query" call of a views_handler_filter object.  Should not contain opening tags.<br/><br/>If you do not know what you are doing, do not use this. If you still want to try, start with var_dump($this) and look at the result. <br/><br/> Anything output by the code can be displayed using the Views Modify Query field.'),
      '#default_value' => $this->value,
    );
  }

  function query() {
    ob_start();
    eval($this->value);
    $output = ob_get_clean();
    
    if (!is_array($this->query->vmq_output)) {
      $this->query->vmq_output = array();
    }
    
    $this->query->vmq_output[] = $output;
  }
  
  /**
   * Helper functions for writting query modifiers
   */
   
  /**
   * Finds the first 'where' clause matching the given regular expression,
   * and return an array defining :
   * 'group_id' : Id of the where group
   * 'clause_id' : Id of the clause in the were group
   * 'text' : Actual clause
   * 'matches': Matches from the regexp
   */
  function find_where_clause($regexp) {
    $matches = array();
    foreach($this->query->where as $group_id => $group_content) {
      foreach ($group_content['clauses'] as $clause_id => $clause) {
        if (preg_match($regexp, $clause, $matches)) {
          return array(
            'group_id' => $group_id,
            'clause_id' => $clause_id,
            'text' => $clause,
            'matches' => $matches
          );
        }
      }
    }
    
    return false;
  }
  
  /**
   * Update the where clause defined by $clause (as returned
   * by find_where_clause) with the content in $new
   */
  function update_where_clause($clause, $new) {
    $this->query->where[$clause['group_id']]['clauses'][$clause['clause_id']] = $new;
  }
}
