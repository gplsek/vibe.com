<?php
//$themepath = drupal_get_path('theme','smooth');
//drupal_add_css(

// This will add variables in the Drupal.settings object
global $base_url;
drupal_add_js(array('theme_path' => $base_url ."/". drupal_get_path('theme', 'smooth')), 'setting');


drupal_add_css(drupal_get_path('theme','smooth') . "/css/".theme_get_setting('theme_style').".css" , 'theme', 'all');

/* CheckBox js and css */
drupal_add_js(drupal_get_path('theme','smooth') . "/js/jquery.checkbox.js");
drupal_add_css(drupal_get_path('theme','smooth') . "/css/jquery.checkbox.css", 'theme', 'all');
drupal_add_css(drupal_get_path('theme','smooth') . "/css/jquery.safari-checkbox.css", 'theme', 'all');

function smooth_terms_of_use($terms, $node) {
  $output  = '<span id="terms-of-use">';
  $output  = $node->teaser;
  $output .= t('&raquo; !here.', array('!here' => l('more', 'node/'."$node->nid")));
  $output .= '</span>';
  return $output;
}


function smooth_menu_item_link($link) {
if (empty($link['localized_options'])) {
    $link['localized_options'] = array();
  }
  $link['title'] = $link['title'] = '<span class="link">' . check_plain($link['title']) . '</span>';
  $link['localized_options'] += array('html'=> TRUE);
  return l($link['title'], $link['href'], $link['localized_options']);
}

function phptemplate_breadcrumb($breadcrumb) {
	if (!empty($breadcrumb)) {
		//$separator = ' <span class="seperator">&nbsp;</span> ';
		//$separator = ' '. theme_get_setting('jackncoke_breadcrumb_separator') . ' ';
		$separator = ' 	&#187; ';
		$breadcrumb_string = '<div class="breadcrumb " style="font-size:10px;line-height:20px;">'
		. implode($separator, $breadcrumb)
		. '</div>';
		return $breadcrumb_string;
	}
}

function smooth_preprocess_search_block_form(&$vars, $hook) {
  // Note that in order to theme a search block you should rename this function
  // 'search_block_form' instead of 'search_theme_form' in the customizations
  // bellow.

  // Modify elements of the search form
  $vars['form']['search_block_form']['#title'] = t('');
 
  // Set a default value for the search box
  $vars['form']['search_block_form']['#value'] = t('Search...');
 
  // Add a custom class and placeholder text to the search box
  $vars['form']['search_block_form']['#attributes'] = array('class' => 'search-box',
  															  'onmouseover' => '$("#search").addClass("hover");',
                                                              'onfocus' => 'if (this.value == "Search...") {this.value = "";};$("#search").addClass("focus");$("#search").removeClass("hover");',
                                                              'onblur' => 'if (this.value == "") {this.value = "Search...";};$("#search").removeClass("focus");',
  															  'onmouseout' => '$("#search").removeClass("hover");');
 
  // Change the text on the submit button
  $vars['form']['submit']['#value'] = t('');

  // Rebuild the rendered version (search form only, rest remains unchanged)
  unset($vars['form']['search_block_form']['#printed']);
  $vars['search']['search_block_form'] = drupal_render($vars['form']['search_block_form']);

  $vars['form']['submit']['#type'] = 'submit';
 // $vars['form']['submit']['#src'] = drupal_get_path('theme','clean') . '/images/search-btn.png';
  
  $vars['form']['submit']['#attributes'] = array('class'=>'search-button');
   
  // Rebuild the rendered version (submit button, rest remains unchanged)
  unset($vars['form']['submit']['#printed']);
  $vars['search']['submit'] = drupal_render($vars['form']['submit']);

  // Collect all form elements to make it easier to print the whole form.
  $vars['search_form'] = implode($vars['search']);
}

function smooth_preprocess_comment(&$variables){
	$newimg = drupal_get_path('theme','smooth')."/images/new.png";
	$variables['new']       =  theme('image', $newimg, 'new','news');//theme_image($newimg, 'new', 'new');
}

function smooth_forum_icon($new_posts, $num_posts = 0, $comment_mode = 0, $sticky = 0) {
  if ($num_posts > variable_get('forum_hot_topic', 15)) {
    $icon = $new_posts ? 'hot-new' : 'hot';
  }
  else {
    $icon = $new_posts ? 'new' : 'default';
  }
  if ($comment_mode == COMMENT_NODE_READ_ONLY || $comment_mode == COMMENT_NODE_DISABLED) {
    $icon = 'closed';
  }
  if ($sticky == 1) {
    $icon = 'sticky';
  }

  $filepath = "images/icons/20/forum/forum-$icon.png" ;
  $theme_filepath = path_to_theme().'/'.$filepath ;
  if ( file_validate_is_image($theme_filepath) ) {
    $filepath = $theme_filepath ;
  }

  $output = theme('image', $filepath);

  if ($new_posts) {
    $output = "<a name=\"new\">$output</a>";
  }

  return $output;
}

function smooth_links($links, $attributes = array('class' => 'links')) {
  global $language;
  $output = '';

  if (count($links) > 0) {
    $output = '<ul'. drupal_attributes($attributes) .'>';

    $num_links = count($links);
    $i = 1;

    foreach ($links as $key => $link) {
      $class = $key;
	  
	  if ($link['title'] == 'Older polls') {
		unset($link);
	  }
	  
      // Add first, last and active classes to the list of links to help out themers.
      if ($i == 1) {
        $class .= ' first';
      }
      if ($i == $num_links) {
        $class .= ' last';
      }
      if (isset($link['href']) && ($link['href'] == $_GET['q'] || ($link['href'] == '<front>' && drupal_is_front_page()))
          && (empty($link['language']) || $link['language']->language == $language->language)) {
        $class .= ' active';
      }
      
      $output .= '<li'. drupal_attributes(array('class' => $class)) .'>';

      if (isset($link['href'])) {
        // Pass in $link as $options, they share the same keys.
        $op = array(
         'attributes' => array(
         'class' => $class),
        'html' => TRUE,
        );
        
        
        $link['title'] = $link['title'] = '<span class="'.$key.'">' . check_plain($link['title']) . '</span>';
        $output .= l($link['title'], $link['href'], $op);
      }
      else if (!empty($link['title'])) {
        // Some links are actually not links, but we wrap these in <span> for adding title and class attributes
        if (empty($link['html'])) {
          $link['title'] = check_plain($link['title']);
        }
        $span_attributes = '';
        if (isset($link['attributes'])) {
          $span_attributes = drupal_attributes($link['attributes']);
        }
        $output .= '<span'. $span_attributes .'>'. $link['title'] .'</span>';
      }

      $i++;
      $output .= "</li>\n";
    }

    $output .= '</ul>';
  }

  return $output;
}
function smooth_preprocess_maintenance_page(&$vars) {
	$vars['theme_style'] = theme_get_setting('theme_style');
	$vars['head_title'] = variable_get('site_name', 'Drupal')." is currently down for maintenance. ";
	$vars['title'] = variable_get('site_name', 'Drupal')." is currently down for maintenance. ";
	
//	print ('<pre>');
//	print_r($vars);
//	print ('</pre>');
}

function smooth_preprocess_page(&$vars){
$suggestions = array();

// Suggest by Node Type 
if (isset($vars['node'])) {
// If the node type is "blog" the template suggestion will be "node-blog.tpl.php".
//  $sug = 'node-'. str_replace('_', '-', $vars['node']->type);
//  $suggestions[] = $sug; 
}

// Suggest by Path 
// if the uri alias is /videos/crazy
// add suggestions of page-videos.tpl.php and page-videos-crazy.tpl.php
if (module_exists('path')) {
  $alias = drupal_get_path_alias(str_replace('/edit','',$_GET['q']));
  if ($alias != $_GET['q']) {
    $template_filename = 'page';
    foreach (explode('/', $alias) as $path_part) {
      $template_filename = $template_filename . '-' . $path_part;
      $suggestions[] = $template_filename;
    }
  }  
}

/**
 *
 * Finally, before sending to node-[x].tpl, make sure there 
 * aren't any duplicate suggestions.
 */

$suggestions = array_unique($suggestions);

/**
 * Now add those suggestions to your template_files array
 */

foreach ($suggestions as $tmp) {
  $vars['template_files'][] = $tmp;
}
$vars['theme_style'] = theme_get_setting('theme_style');

//krumo($vars);
}

function social(&$vars) {
	$S16 = drupal_get_path('theme','smooth'). "/images/socials/16/";
	$S32 = drupal_get_path('theme','smooth'). "/images/socials/32/";
         
	foreach ($vars as $link) {
	$items = explode("|", $link );
	switch($items[2]){
		case 16:
			$img = $S16.$items[0]."_16.png";
			break;
		case 32:
			$img = $S32.$items[0]."_32.png";
			break;
	}
	$op = array('attributes' => array('class' => 'social','title'=>$items[0]),'html' => TRUE,);  
	if(file_exists($img)){
	$make 	= theme('image', $img,$items[0], 'social network '.$items[0]);
	
	if($items[1] <> NULL){$make 	= l($make, $items[1], $op);	}
	
	$output .= $make;
	}
	
	}
	
return $output;
}

?>