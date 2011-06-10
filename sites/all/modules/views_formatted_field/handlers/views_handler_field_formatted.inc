<?php
// $Id: views_handler_field_formatted.inc,v 1.1 2009/06/22 15:17:23 overall Exp $

/**
 * Field handler to provide custom displays by using one of specified input formats (i.e. Filtered HTML, PHP code, Smarty code)
 * with ability to access value of current field, all arguments values, all rows values, all fields values (of current row),
 * rendered output of previous fields (of current row) and current row index from specified code.
 *
 * Variables available in specified code:
 * $args - Indexed array. Values are arguments values.
 * $field - Scalar. Value of current field.
 * $row - Object. Fields are view fields names. Values are values of current row fields.  
 * $rows - Indexed array. Values are objects like $row.
 * $output - Object. Fields are view fields names. Values are rendered values (HTML code) of current row fields. Contains fields only previous to current.  
 * //$outputs - Indexed array. Values are objects like $output. Contains rows only previous to current. //TODO: implement  
 * $field_name - Name of current field.
 * $row_index - Index of current row. // Only if http://drupal.org/node/430068#comment-1690684 patch is applied
 * 
 * Relations between variables:
 * $field == $row->$field_name; 
 * $row == $rows[$row_index]; // If $row_index is available.
 * //$output == $outputs[$row_index]; //TODO: implement
 *  
 * @ingroup views_field_handlers
 */
class views_handler_field_formatted extends views_handler_field {

  /**
   * Build options.
   */
  function option_definition() {
    //$options = array();
    
    $options = parent::option_definition();
    $options = array('label' => $options['label']);
    
    $options['input_format'] = array('default' => NULL);
    $options['code'] = array('default' => NULL);
    return $options;
  }
  
  /** 
   * Build query to get it fields in get_variables_names_info().
   */
  static function build_view_query($view, $display_id = NULL) {
    if (!empty($view->built)) {
      return;
    }

    if (empty($view->current_display) || $display_id) {
      if (!$view->set_display($display_id)) {
        return FALSE;
      }
    }

    // Let modules modify the view just prior to executing it.
    foreach (module_implements('views_pre_build') as $module) {
      $function = $module . '_views_pre_build';
      $function($view);
    }

    // Attempt to load from cache.
    // @todo Load a build_info from cache.

    $start = views_microtime();
    // If that fails, let's build!
    $view->build_info = array(
      'query' => '',
      'count_query' => '',
      'query_args' => array(),
    );

    $view->init_query();

    // Call a module hook and see if it wants to present us with a
    // pre-built query or instruct us not to build the query for
    // some reason.
    // @todo: Implement this. Use the same mechanism Panels uses.

    // Run through our handlers and ensure they have necessary information.
    $view->init_handlers();

    // Let the handlers interact with each other if they really want.
    $view->_pre_query();

    if ($view->display_handler->uses_exposed()) {
      $view->exposed_widgets = $view->render_exposed_form();
      if (form_set_error() || !empty($view->build_info['abort'])) {
        $view->built = TRUE;
        return empty($view->build_info['fail']);
      }
    }

    // Build all the relationships first thing.
    $view->_build('relationship');

    // Build all the filters.
    $view->_build('filter');

    $view->build_sort = TRUE;

    // Arguments can, in fact, cause this whole thing to abort.
    if (!$view->_build_arguments()) {
      /*$view->build_time = views_microtime() - $start;
      return $view->built;*/
    }

    // Initialize the style; arguments may have changed which style we use,
    // so waiting as long as possible is important. But we need to know
    // about the style when we go to build fields.
    if (!$view->init_style()) {
      $view->build_info['fail'] = TRUE;
      return FALSE;
    }

    if ($view->style_plugin->uses_fields()) {
      $view->_build('field');
    }

    // Build our sort criteria if we were instructed to do so.
    if (!empty($view->build_sort)) {
      // Allow the style handler to deal with sorting.
      if ($view->style_plugin->build_sort()) {
        $view->_build('sort');
      }
    }

    // Allow display handler to affect the query:
    $view->display_handler->query();

    // Allow style handler to affect the query:
    $view->style_plugin->query();

    if (variable_get('views_sql_signature', FALSE)) {
      $view->query->add_field(NULL, "'" . $view->name . ':' . $view->current_display . "'", 'view_name');
    }

    // Let modules modify the query just prior to finalizing it.
    foreach (module_implements('views_query_alter') as $module) {
      $function = $module . '_views_query_alter';
      $function($view, $view->query);
    }

    $view->build_info['query'] = $view->query->query();
    $view->build_info['count_query'] = $view->query->query(TRUE);
    $view->build_info['query_args'] = $view->query->get_where_args();
    $view->built = TRUE;
    $view->build_time = views_microtime() - $start;

    // Attach displays
    $view->attach_displays();
    return TRUE;
  }

  /*static function build_view_query($view, $display_id = NULL) {
    $view->init_query();
    $view->init_handlers();
    $view->_build('field');
  }*/
  
  /** 
   * Theme fields and arguments for option form.
   */
  function theme_variables_names($variables_names, $title) {
    if (empty($variables_names)) {
      return '';
    }
    
    $items = array();
    foreach ($variables_names as $key => $value) {
      if ($value) {
        $items[] = $key .' == '. $value;
      } else {
        $items[] = $key;
      }
    }
    return theme('item_list', $items, $title);
  }
  
  function get_variables_names_info() {
    // Get a list of the available arguments for render.
    $arguments = array();
    $argument_index = 1;
    foreach ($this->view->display_handler->get_handlers('argument') as $arg => $handler) {
      $arguments['$args['.$argument_index.']'] = $handler->ui_name();
      $argument_index++;
    }
    
    $field_handlers = $this->view->display_handler->get_handlers('field');
    
    // Get a list of the available fields for render.
    $fields = array();
    $view_copy = unserialize(serialize($this->view));
    self::build_view_query($view_copy);
    foreach( $view_copy->query->fields as $field ) {
      $fields['$row->'.$field['field']] = NULL;
    }

    // Get a list of the available fields outputs for render.
    $outputs = array();
    foreach ($field_handlers as $field => $handler) {
      // We only use fields up to (and not including) this one.
      if ($field == $this->options['id']) {
        break;
      }
      
      $outputs['$output->'.$field] = t('Output of') .' '. $handler->ui_name();
    }
    
    $variables_names_help =
      $this->theme_variables_names($arguments, t('Arguments')).
      $this->theme_variables_names(array('$field' => t('Value of this field')), t('This Field')).
      $this->theme_variables_names($fields, t('This Row Fields (for more info see "Query" in "Live preview" below)')).
      $this->theme_variables_names(array('$rows[]' => t('Indexed array. Values are arrays like $row.')), t('All Rows')).
      $this->theme_variables_names($outputs, t('This Row Fields Output (contains fields only previous to current)')).
      $this->theme_variables_names(array('$field_name' => '\''. check_plain($this->field/*$this->options['id']*/) .'\''), t('This Field Name')).
      $this->theme_variables_names(array('$row_index' => t('Index of current row. (Only if <a href="!href">"row_index" patch</a> is applied.)', array('!href' => 'http://drupal.org/node/430068#comment-1690684'))), t('This Row Index'));
      
    return $variables_names_help;  
  }
  
  /*static function check_permission() {
    global $user;
    
    if (user_access('use PHP in views', $user)) {
      return TRUE;
    }
    else {
      drupal_set_message(t('You do not have permission to use PHP in views.'));
      return FALSE;
    }
  }*/
  
  /**
   * Compose options form.
   */
  function options_form(&$form, &$form_state) {
    //$form = array();

    /*if (!self::check_permission()) {
      $form = array(); //???
      return;
    }*/
    
    $parent_form = $form;
    $parent_form_state = $form_state;
    parent::options_form($parent_form, $parent_form_state);
    $form['label'] = $parent_form['label'];

    $filter_formats = filter_formats();
    $input_format_options = array();
    foreach ($filter_formats as $filter_format_id => $filter_format) {
      $input_format_options[$filter_format_id] = $filter_format->name;
    }
    
    $form['input_format'] = array(
      '#title' => t('Input format'),
      '#type' => 'select',
      '#options' => $input_format_options,
      '#description' => 'Input format to use for formatting of given code.',
      '#default_value' => $this->options['input_format'],
      '#required' => TRUE,
      '#weight' => 1,
    );
    
    $form['code'] = array(
      '#title' => t('Code'),
      '#type' => 'textarea',
      '#default_value' => $this->options['code'],
      '#required' => TRUE,
      '#weight' => 2,
    );
    
    $variables_names_help = t('<p>You must add some additional fields to this display before using this field. These fields may be marked as <em>Exclude from display</em> if you prefer. Note that due to rendering order, you cannot use fields that come after this field; if you need a field not listed here, rearrange your fields.</p>');
    $variables_names_info = $this->get_variables_names_info();
    if (!empty($variables_names_info)) {
      $variables_names_help = t('<p>The following variables are available for this display. Use the variable shown on the left to display the value indicated on the right. Note that due to rendering order, you cannot use fields that come after this field; if you need a field not listed here, rearrange your fields.</p>');
      $variables_names_help .= $variables_names_info;
    }
    
    $form['help'] = array(
      '#type' => 'fieldset',
      '#title' => t('Available variables'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => $variables_names_help,
      '#id' => 'views-tokens-help', 
      '#weight' => 3,
    );

    $variables_test =
      t('If you want to see values for available variables, you can paste following PHP code into "!code" text area.', array('!code' => t('PHP code'))).'<br /><br />'. 
      '<code><span style="color: #000000"><span style="color: #007700">return </span><span style="color: #DD0000">\'&lt;pre&gt;\'</span><span style="color: #007700">.</span><span style="color: #0000BB">print_r</span><span style="color: #007700">(array(<br />&nbsp; </span><span style="color: #DD0000">\'args\' </span><span style="color: #007700">=&gt; </span><span style="color: #0000BB">$args</span><span style="color: #007700">,<br />&nbsp; </span><span style="color: #DD0000">\'field\' </span><span style="color: #007700">=&gt; </span><span style="color: #0000BB">$field</span><span style="color: #007700">,<br />&nbsp; </span><span style="color: #DD0000">\'row\' </span><span style="color: #007700">=&gt; </span><span style="color: #0000BB">$row</span><span style="color: #007700">,<br />&nbsp; </span><span style="color: #DD0000">\'rows\' </span><span style="color: #007700">=&gt; </span><span style="color: #0000BB">$rows</span><span style="color: #007700">,<br />&nbsp; </span><span style="color: #DD0000">\'output\' </span><span style="color: #007700">=&gt; </span><span style="color: #0000BB">$output</span><span style="color: #007700">,<br />&nbsp; </span><span style="color: #DD0000">\'field_name\' </span><span style="color: #007700">=&gt; </span><span style="color: #0000BB">$field_name</span><span style="color: #007700">,<br />&nbsp; </span><span style="color: #DD0000">\'row_index\' </span><span style="color: #007700">=&gt; </span><span style="color: #0000BB">$row_index</span><span style="color: #007700">,<br />), </span><span style="color: #0000BB">TRUE</span><span style="color: #007700">).</span><span style="color: #DD0000">\'&lt;/pre&gt;\'</span><span style="color: #007700">;<br /></span></span></code>';
    
    $form['test'] = array(
      '#type' => 'fieldset',
      '#title' => t('Variables values test PHP code'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => $variables_test,
      '#id' => 'views-tokens-help', //??? 
      '#weight' => 4,
    );
    
    //$form['php_code']['#suffix'] = '<pre>'.print_r($parent_form_state, TRUE).'</pre>'; // 666
  }
  
  /**
   * Get values of all arguments that are available to use for rendering.
   */
  function get_render_arguments() {
    $substitutions = $this->view->build_info['substitutions'];
    $substitutions = !empty($substitutions) ? $substitutions : array();
    
    $argument_index = 1;
    $arguments = array();
    foreach ($this->view->display_handler->get_handlers('argument') as $arg => $handler) {
      $arguments[$argument_index] = $substitutions['%'.$argument_index];
      $argument_index++;
    }

    return $arguments;
  }

  /**
   * Get values of all fields that are available to use for rendering.
   */
  function get_render_outputs() {
    $outputs = array();
    foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {
      $outputs[$field] = $handler->last_render;

      // We only use fields up to (and including) this one.
      if ($field == $this->options['id']) {
        break;
      }
    }
    return $outputs;
  }

  /**
   * Get values of all fields to use for rendering.
   */
  function get_render_fields($values) {
    $fields_values = array();
    foreach( $this->query->fields as $field ) {
      $fields_values[$field['field']] = $values->{$field['alias']};
    }
    return $fields_values;
  }

  /**
   * Get values of all fields of all rows to use for rendering.
   */
  function get_render_rows() {
    $rows = array();
    foreach ($this->view->result as $row_index => $row) {
      foreach ($this->query->fields as $field) {
        $rows[$row_index][$field['field']] = $row->{$field['alias']};
      }
    }
    return $rows;
  }
  
  /**
   * Get all variables that are available to use for rendering.
   */
  function get_render_variables($values) {
    $variables = array();
    
    $variables['args']   = $this->get_render_arguments();
    $variables['row']    = (object)$this->get_render_fields($values);
    $variables['rows']   = $this->get_render_rows();
    $variables['output'] = (object)$this->get_render_outputs();
    
    $variables['field_name'] = $this->field; //$this->options['id']; //???
    $variables['row_index']  = $this->view->row_index;

    $variables['field'] = $variables['row']->{$variables['field_name']};
    
    return $variables;
  }
  
  /**
   * Use the requested format function to render the raw alias value.
   */
  function render($values) {
    $variables = $this->get_render_variables($values);
    
    // $variables is not supported, so use workaround
    $GLOBALS['filter']['variables'] = $variables;
    
    $output = check_markup($this->options['code'], $this->options['input_format'], FALSE); 
    
    unset($GLOBALS['filter']['variables']);

    return $output;
  }
}