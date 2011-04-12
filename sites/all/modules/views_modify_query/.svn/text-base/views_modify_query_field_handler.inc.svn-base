<?php

class views_modify_query_field_handler extends views_handler_field {
  
  // Choose the output format
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['vmq_format'] = array(
      '#type' => 'select',
      '#title' => t('Output format'),
      '#options' => array(
        'escape' => t('Escaped'),
        'pre' => t('Escaped, enclosed with pre tags'),
        'raw' => t('Raw (be careful!)'),
      ),
      '#default_value' => !empty($this->options['vmq_format']) ? $this->options['vmq_format'] : 'escape',
    );
  }
  
  // We're not a database field, so we don't want anything done here
  function query() {
  }
  
  // Render it !
  function render() {
    if (empty($this->query->vmq_output)) {
      return check_plain(t('[no Views Modify Query output to display]'));
    }
    
    $output = $this->query->vmq_output;
    foreach ($output as $i => $v) {
      switch ($this->options['vmq_format']) {
      case 'escape':
        $v = check_plain($v);
        break;
      case 'pre':
        $v = '<pre>'.check_plain($v).'</pre>';
        break;
      }
      $output[$i] = $v;
    }
    
    $div = '<div class="wmq_output">';
    return $div.implode("</div>$div", $output)."</div>";
  }
}
