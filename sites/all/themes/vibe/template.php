<?php
// $Id: template.php,v 1.44.2.6 2009/02/13 19:02:49 johnalbin Exp $

/**
 * @file
 * Contains theme override functions and preprocess functions for the Zen theme.
 *
 * IMPORTANT WARNING: DO NOT MODIFY THIS FILE.
 *
 * The base Zen theme is designed to be easily extended by its sub-themes. You
 * shouldn't modify this or any of the CSS or PHP files in the root vibe/ folder.
 * See the online documentation for more information:
 *   http://drupal.org/node/193318
 */

// Auto-rebuild the theme registry during theme development.
if (theme_get_setting('vibe_rebuild_registry')) {
  //drupal_rebuild_theme_registry();
}

//drupal_rebuild_theme_registry();

/*
 * Add stylesheets only needed when Zen is the active theme. Don't do something
 * this dumb in your sub-theme; see how wireframes.css is handled instead.
 */
if ($GLOBALS['theme'] == 'vibe') { // If we're in the main theme
  if (theme_get_setting('vibe_layout') == 'border-politics-fixed') {
    drupal_add_css(drupal_get_path('theme', 'vibe') . '/layout-fixed.css', 'theme', 'all');
  }
  else {
    drupal_add_css(drupal_get_path('theme', 'vibe') . '/layout-liquid.css', 'theme', 'all');
  }
}


/**
 * Implements HOOK_theme().
 */
function vibe_theme(&$existing, $type, $theme, $path) {
  if (!db_is_active()) {
       return array();
  }
  include_once './' . drupal_get_path('theme', 'vibe') . '/template.theme-registry.inc';
  return _vibe_theme($existing, $type, $theme, $path);
}

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return
 *   A string containing the breadcrumb output.
 */
function vibe_breadcrumb($breadcrumb) {
  // Determine if we are to display the breadcrumb.
  $show_breadcrumb = theme_get_setting('vibe_breadcrumb');
  if ($show_breadcrumb == 'yes' || $show_breadcrumb == 'admin' && arg(0) == 'admin') {

    // Optionally get rid of the homepage link.
    $show_breadcrumb_home = theme_get_setting('vibe_breadcrumb_home');
    if (!$show_breadcrumb_home) {
      array_shift($breadcrumb);
    }

    // Return the breadcrumb with separators.
    if (!empty($breadcrumb)) {
      $breadcrumb_separator = theme_get_setting('vibe_breadcrumb_separator');
      $trailing_separator = $title = '';
      if (theme_get_setting('vibe_breadcrumb_title')) {
        $trailing_separator = $breadcrumb_separator;
        $title = menu_get_active_title();
      }
      elseif (theme_get_setting('vibe_breadcrumb_trailing')) {
        $trailing_separator = $breadcrumb_separator;
      }
      return '<div class="breadcrumb">' . implode($breadcrumb_separator, $breadcrumb) . "$trailing_separator$title</div>";
    }
  }
  // Otherwise, return an empty string.
  return '';
}

function vibe_menu_links($links) {
	global $directory; 
	
  if (!count($links)) {
    return '';
  }
  
  
  $terms = null; 
  
  if (arg(0) == 'node' && is_numeric(arg(1)) && is_null(arg(2))) {
  	//var_dump(arg(0));
	$taxonomies = array();
	$category_vocab_id = 0;
	
	foreach(taxonomy_get_vocabularies() as $vocab) {
		if($vocab->name == "Category") {
			$taxonomies["Category"] = $vocab;
			$category_vocab_id = $vocab->vid;
			break;
			}
	}
	
	if($category_vocab_id) {
		$node = node_load(arg(1));
		$terms = taxonomy_node_get_terms_by_vocabulary($node, $category_vocab_id);
		
	}
  }
  
  
  $category_name = null;
  
  if($terms) {
  	$term = array_pop($terms);
  	$category_name = $term->name;
  }
  
  //var_dump($links);
  $level_tmp = explode('-', key($links));
  $level = $level_tmp[0];
  $section = 'homepage';
  $output = "<ul class=\"links-$level links \">\n";
  foreach ($links as $index => $link) {
    $output .= '<li class="inline"'; 
    if (stristr($index, 'active') || ($category_name && strtolower($category_name) == strtolower($link['title']))) {
      	$section = strtolower($link['title']);
		$output .= ' active ';   
	  	$output .= "'><a href='/" . $link['href'] . "'><img src='/" . path_to_theme() . "/images/" . strtolower($link['title']) . "_nav_on.png' border='0'/></a></li>\n";
		
    } else {
    	$output .= "'><a href='/" . $link['href'] . "'><img src='/" . path_to_theme() . "/images/" . strtolower($link['title']) . "_nav_off.png' border='0' onmouseover='this.src=\"/" . path_to_theme() . "/images/" . strtolower($link['title']) ."_nav_on.png\"' onmouseout='this.src=\"/" . path_to_theme() . "/images/" . strtolower($link['title']) ."_nav_off.png\"'/></a></li>\n";
    }
    //$output .= ">". $link['title'] . "</li>\n";
    
    //$output .= "'><a href='/" . $link['href'] . "'><img src='/" . path_to_theme() . "/images/" . strtolower($link['title']) . "_nav_off.png' border='0' /></a></li>\n";
  }
  $output .= '</ul>';
  $output .= "<script type='text/javascript'> var section = '". $section ."';</script>";
  return $output;
}

/*function vibe_menu_links($links = array()) {
  return "TESTING";
  //return _phptemplate_callback('menu_links', array('links' => $links));
}*/

/**
 * Implements theme_menu_item_link()
 */
function vibe_menu_item_link($item, $link_item = NULL ) {
  
  /*if (empty($link['localized_options'])) {
    $link['localized_options'] = array();
  }
  // If an item is a LOCAL TASK, render it as a tab
  if ($link['type'] & MENU_IS_LOCAL_TASK) {
    $link['title'] = '<span class="tab">' . check_plain($link['title']) . '</span>';
    $link['localized_options']['html'] = TRUE;
  }
  return l($link['title'], $link['href'], $link['localized_options']);
*/

   return "ABC DEF";
}

/**
 * Duplicate of theme_menu_local_tasks() but adds clear-block to tabs.
 */
function vibe_menu_local_tasks() {
/*
  $output = '';
  
  if ($primary = menu_primary_local_tasks()) {
    $output .= '<ul class="tabs primary clear-block">' . $primary . '</ul>';
  }
  if ($secondary = menu_secondary_local_tasks()) {
    $output .= '<ul class="tabs secondary clear-block">' . $secondary . '</ul>';
  }

  return $output;
*/
}

/**
 * Override or insert variables into the page templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
function vibe_preprocess_page(&$vars, $hook) {
  drupal_set_html_head('<script type="text/javascript" src="http://m.vibe.com/mobify/redirect.js"></script>');
  drupal_set_html_head('<script type="text/javascript">try{_mobify("http://m.vibe.com/");} catch(err) {};</script>');
  $vars['head'] = drupal_get_html_head();
	$vars['head_title'] = str_replace('Vibe','VIBE',$vars['head_title']);
  // Add conditional stylesheets.
  if (!module_exists('conditional_styles')) {
    $vars['styles'] .= $vars['conditional_styles'] = variable_get('conditional_styles_' . $GLOBALS['theme'], '');
  }

  // Classes for body element. Allows advanced theming based on context
  // (home page, node of certain type, etc.)
  $classes = split(' ', $vars['body_classes']);
  // Remove the mostly useless page-ARG0 class.
  if ($index = array_search(preg_replace('![^abcdefghijklmnopqrstuvwxyz0-9-_]+!s', '', 'page-'. drupal_strtolower(arg(0))), $classes)) {
    unset($classes[$index]);
  }
  if (!$vars['is_front']) {
    // Add unique class for each page.
    $path = drupal_get_path_alias($_GET['q']);
    $classes[] = vibe_id_safe('page-' . $path);
    // Add unique class for each website section.
    list($section, ) = explode('/', $path, 2);
    if (arg(0) == 'node') {
      if (arg(1) == 'add') {
        $section = 'node-add';
      }
      elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
        $section = 'node-' . arg(2);
      }
    }
    $classes[] = vibe_id_safe('section-' . $section);
  }
  if (theme_get_setting('vibe_wireframes')) {
    $classes[] = 'with-wireframes'; // Optionally add the wireframes style.
  }
  $vars['body_classes_array'] = $classes;
  $vars['body_classes'] = implode(' ', $classes); // Concatenate with spaces.
  
  $vars['main_menu'] = menu_tree_data('menu-main-nav');
  $vars['currentUrl'] = $_SERVER['HTTP_HOST'] . "/" . $path;
}

/**
 * Override or insert variables into the node templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
function vibe_preprocess_node(&$vars, $hook) {
  
 global $twitter_post_name;
 global $celeb_post_name;
 global  $celeb_post_link;
 global  $url_alias;
 global $blog_name;
 global $blog_post_type;
 global $header_image;
 global $node;
 global $twitter_post_image;
 
 $query = db_query("SELECT dst FROM url_alias WHERE src = '%s'",arg(0).'/'.arg(1));
 $result = db_fetch_object($query);
 $url_alias = substr($result->dst,0,stripos($result->dst,"/"));
 
 
	$query = db_query("SELECT * FROM jp_blog WHERE base_uri = '%s'",$url_alias);
	$result = db_fetch_object($query);
	$header_image = $result->header_image_path;
	$node = node_load($vars['nid']);
	$twitter_post_name = $result->twitter_name;
	$celeb_post_name = $result->celeb_name;
	$celeb_post_link = $result->celeb_link;
	$blog_name = $result->name;
	$blog_post_type = $result->blog_type;
 
 
 
  
  // Special classes for nodes
  $classes = array('node');
  if ($vars['sticky']) {
    $classes[] = 'sticky';
  }
  if (!$vars['status']) {
    $classes[] = 'node-unpublished';
    $vars['unpublished'] = TRUE;
  }
  else {
    $vars['unpublished'] = FALSE;
  }
  if ($vars['uid'] && $vars['uid'] == $GLOBALS['user']->uid) {
    $classes[] = 'node-mine'; // Node is authored by current user.
  }
  if ($vars['teaser']) {
    $classes[] = 'node-teaser'; // Node is displayed as teaser.
  }
  // Class for node type: "node-type-page", "node-type-story", "node-type-my-custom-type", etc.
  $classes[] = vibe_id_safe('node-type-' . $vars['type']);
  $vars['classes'] = implode(' ', $classes); // Concatenate with spaces
  
  // bwetter - add extra template suggestions
  //template name for current node id
  //what do we need this for the standard one comes up anyways?
  $suggestions = array('node-'. $vars['nid']);
  // additional node template names based on path alias
  if (module_exists('path')) {
    // we already can have a path alias
    if (isset($vars['node']->path)) {
      $alias = ($vars['node']->path);
    }
    else {
      // otherwise do standard check
      $alias = drupal_get_path_alias('node/'. $vars['nid']);
    }
    if ($alias != 'node/'. $vars['nid']) {
      $add_path = '';
      foreach (explode('/', $alias) as $path_part) {
        $add_path .= !empty($path_part) ? "-" . $path_part : '';
        //adding a pure path template name
        $suggestions[] = 'node-' . $add_path;
      }
      //what do we need this for?
      // adding the last one (higher priority) for this path only
      // node-some-long-path-nofollow.tpl.php (not for anchestors)
      $suggestions[] = end($suggestions) .'-nofollow';
    }
  }
  $vars['template_files'] = isset($vars['template_files']) ? array_merge($vars['template_files'], $suggestions) : $suggestions;

  // photo gallery specific template settings
  if ($vars['type'] == 'photo_gallery') {
	if (is_numeric(arg(2))) {

		if (in_array('node-panel-photo_gallery_nav', $vars['template_files'])) {
			$vars['template_files'] = array('node-panel-photo_gallery_nav');
		} else {
  			$vars['template_files'] = array('node-photo_gallery-photo');
  		}
  		
  		$vars['image_index'] = arg(2);
	}
  	drupal_add_css(path_to_theme(). "/vibe-photo-gallery.css", "theme");
   }
   
}

/**
 * Override or insert variables into the comment templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
function vibe_preprocess_comment(&$vars, $hook) {
 include_once './' . drupal_get_path('theme', 'vibe') . '/template.comment.inc';
  _vibe_preprocess_comment($vars, $hook);
}

/**
 * Override or insert variables into the block templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
function vibe_preprocess_block(&$vars, $hook) {
  $block = $vars['block'];

  // Special classes for blocks.
  $classes = array('block');
  $classes[] = 'block-' . $block->module;
  $classes[] = 'region-' . $vars['block_zebra'];
  $classes[] = $vars['zebra'];
  $classes[] = 'region-count-' . $vars['block_id'];
  $classes[] = 'count-' . $vars['id'];

  $vars['edit_links_array'] = array();
  $vars['edit_links'] = '';
  if (theme_get_setting('vibe_block_editing') && user_access('administer blocks')) {
    include_once './' . drupal_get_path('theme', 'vibe') . '/template.block-editing.inc';
    vibe_preprocess_block_editing($vars, $hook);
    $classes[] = 'with-block-editing';
  }

  // Render block classes.
  $vars['classes'] = implode(' ', $classes);
}

/**
 * Converts a string to a suitable html ID attribute.
 *
 * http://www.w3.org/TR/html4/struct/global.html#h-7.5.2 specifies what makes a
 * valid ID attribute in HTML. This function:
 *
 * - Ensure an ID starts with an alpha character by optionally adding an 'id'.
 * - Replaces any character except alphanumeric characters with dashes.
 * - Converts entire string to lowercase.
 *
 * @param $string
 *   The string
 * @return
 *   The converted string
 */
function vibe_id_safe($string) {
  // Replace with dashes anything that isn't A-Z, numbers, dashes, or underscores.
  $string = strtolower(preg_replace('/[^a-zA-Z0-9-]+/', '-', $string));
  // If the first character is not a-z, add 'id' in front.
  if (!ctype_lower($string{0})) { // Don't use ctype_alpha since its locale aware.
    $string = 'id' . $string;
  }
  return $string;
}

function vibe_preprocess_search_theme_form(&$variables) {

}

function vibe_preprocess_search_result(&$vars) {
  $result = $vars['result'];
  $vars['search_node'] = $result['node']->content;
}



function vibe_links($links, $attributes = array('class' => 'links')) {
	//var_dump($attributes);echo "\n\n"
	if($attributes['class'] == "flags") {
		//var_dump($links);
		echo "FLAGS<br>";
	}
	return theme_links($links, $attributes);
}
function vibe_user_bar() {
	global $user;
	module_load_include('inc', 'user', 'user.pages');
	
  $output = '';


////////////////////////////////                   ////////////////////////////////////////////////////////
/////////////////////////////// *User Login Form* ////////////////////////////////////////////////////////
//////////////////////////////				     ////////////////////////////////////////////////////////


  if (!$user->uid) { 

	$output .= t('<script type="text/javascript">
				 
				 
				 $(document).ready(function(){
					
					$("#user-profile-info #register_link a").attr("href","#").addClass("thickbox").click(function(){
						$(\'#userloginWrapper\').css(\'display\',\'none\');
						$(\'#userRegisterWrapper\').css(\'display\',\'block\');
						$(\'#userPassWrapper\').css(\'display\',\'none\');											tb_show(null,"#TB_inline?height=430&amp;width=540&amp;inlineId=userloginModal&amp;modal=true",null);																																									  						});
					
					$("#user-profile-info #login_link a").attr("href","#").addClass("thickbox").click(function(){
						$(\'#userloginWrapper\').css(\'display\',\'block\');
						$(\'#userRegisterWrapper\').css(\'display\',\'none\');
						$(\'#userPassWrapper\').css(\'display\',\'none\');	tb_show(null,"#TB_inline?height=430&amp;width=540&amp;inlineId=userloginModal&amp;modal=true",null);																																									  						});
					
					
					$("#user-login-form .first a").css("style","top:55px").attr("href","javascript:void(0)").click(function() { 
						$("#userloginWrapper").css("display","none"); 
						$("#userRegisterWrapper").css("display","block"); 
						$("#userPassWrapper").css("display","none");
					});																																														
					$("#user-login-form .last a").attr("href","#").addClass("thickbox").click(function() { 
						$("#userloginWrapper").css("display","none"); 
						$("#userRegisterWrapper").css("display","none"); 
						$("#userPassWrapper").css("display","block");
						$(".form-image ajax-trigger").click(function(){
									$(this).attr("href","ajax_register/login");
																	 });
						tb_show(null,"#TB_inline?height=430&amp;width=540&amp;inlineId=userloginModal&amp;modal=true",null);
					});							
				});
		</script>
		<div class="user-info" style="text-align: center">
			<div id="user-profile-info">
				<span id="register_link">
					<a href="user/register" >register</a>
				</span>
				<span id="login_link">
					<a href="user/login" onclick="$(\'#userPassWrapper\').css(\'display\',\'none\'); $(\'#userRegisterWrapper\').css(\'display\',\'none\');$(\'#userloginWrapper\').css(\'display\',\'block\');">sign in</a>
				</span>
			</div>
			<div class="clear-block">
			</div>
		</div> 
		
		<div id="userloginModal" style="display:none; z-index: 3000;">
			<div id="userloginWrapper">
				<div class="large_title" style="font-size: 24px; color: #555555; margin-bottom: 3px;">Login or Register
				</div>
				<div>' . drupal_get_form('user_login_block') .  '
				</div>
				<div class="form_buttons" style="text-align: center; margin: 0 auto; margin-top: 43px;">
					<div class="vibe_button" style="float: left; margin-left: 140px; margin-top: -225px; height: 14px; width: 72px; padding: 9px; background-color: #28aae1; /left: 0px; /padding: 9px; /top: 0px; _padding: 7px; _position: relative; _float: left; _margin-top: -160px; _margin-left: 60px;">
					<a href="javascript:void(0)" onclick="tb_remove();">Cancel</a>
					</div>
				</div>
			</div>
			<div id="userRegisterWrapper">
				<div class="large_title" style="font-size: 24px; color: #555555; margin-bottom: 3px;">Register
				</div>
				<div>' . drupal_get_form('user_register') . '
				</div>
				<div class="form_buttons" style="text-align: center; margin: 0 auto; margin-top: -10px;">
					<div class="vibe_button" style="float: left; margin-left: 140px; margin-top: 17px; height: 13px; width: 72px; padding: 9px; background-color: #28aae1; /left: 0px; /padding: 9px; /margin-top: -32px; _padding: 7px; _margin-top: -21px; _position: relative; _margin-left: 110px; _float: left; ">
					<a href="javascript:void(0)" onclick="tb_remove();">Cancel</a>
					</div>
				</div>
			</div>
			<div id="userPassWrapper">
				<div class="large_title" style="font-size: 24px; color: #555555; margin-bottom: 3px;">Forgot your password
				</div>
				<div>' . drupal_get_form('user_pass') . '
				</div>
				<div class="form_buttons" style="text-align: center; margin: 0 auto; margin-top: -10px;">
				
					<div class="vibe_button" style="float: left; margin-left: 123px; margin-top: 7px; height: 16px; width: 72px; padding: 9px; background-color: #28aae1;  /left: 0px; /padding: 8px; /top: 4px; _padding: 7px; _top: 0px; _position: absolute; _float: left; _margin_left: 100px; _border: 1px solid red;">
					<a href="javascript:void(0)" onclick="tb_remove();">Cancel</a>
					</div> 
				</div>
			</div>
				
		</div>');
                      
  }                                                                           
  else {     
  	if(!$user->picture) {
  		$user->picture = "sites/all/themes/vibe/images/MISSINGUSER_ICON_SML.jpg";
  	} 
	
  	//$roles = 'roles: '.implode(', ', $user->roles);
	//print $roles;
	
	if(in_array('Vibe Editor',$user->roles) || in_array('administrative user',$user->roles) || in_array('Celebrity Editor',$user->roles))
	{
		$output .= t('<div class="user-info-loggedin"><div id="user-profile-pic"><img src="/'.$user->picture.'" border="0" height="35"/></div><div id="user-profile-info-loggedin" style="margin-left: 15px;"><div id="user-greet">hello <span class="username">'.$user->name.'</span></div><div id="user-links" style="margin-left: -30px;"><span id="profile_link"><a href="/user">my profile</a></span><span id="logout_link"><a href="/logout">logout</a></span><span id="admin_link"><a href="/admin">admin</a></span></div></div><div class="clear-block"></div></div>');	
	}
	else
	{
		$output .= t('<div class="user-info-loggedin"><div id="user-profile-pic"><img src="/'.$user->picture.'" border="0" height="35"/></div><div id="user-profile-info-loggedin" style="margin-left: 15px;"><div id="user-greet">hello <span class="username">'.$user->name.'</span></div><div id="user-links"><span id="profile_link"><a href="/user">my profile</a></span><span id="logout_link"><a href="/logout">logout</a></span></div></div><div class="clear-block"></div></div>');
	}
  }
   
  $output = '<div id="user-bar">'.$output.'</div>';
  
  //drupal_add_js($scriptOutput,'inline','footer');
     
  return $output; 
}

function vibe_fb_comment_form() {
global $user;                                                              
  $output = '';
  if (!$user->uid) { 
	$output .= '<div class="fbconnect-button">';
    list($size, $length) = explode('_', variable_get('fbconnect_button', NULL)); 
    $output .= fbconnect_render_button($size, $length) . '<span id="login_link">or <a class="thickbox" href="#TB_inline?height=430&amp;width=540&amp;inlineId=userloginModal&amp;modal=true">LOGIN TO VIBE</a></span>'; 
    $output .= '</div>';
    $output .= '<div id="comment-not-reg">Comment Without Registering:</div>';
  }
  return $output;
}
/*function vibe_flag() {
	
}
function vibe_preprocess_flag() {
	
}*/

/*
 function date_diff($d1, $d2){

  //check higher timestamp and switch if neccessary
  if ($d1 < $d2){
    $temp = $d2;
    $d2 = $d1;
    $d1 = $temp;
  }
  else {
    $temp = $d1; //temp can be used for day count if required
  }
  $d1 = date_parse(date("Y-m-d H:i:s",$d1));
  $d2 = date_parse(date("Y-m-d H:i:s",$d2));
  //seconds
  if ($d1['second'] >= $d2['second']){
    $diff['second'] = $d1['second'] - $d2['second'];
  }
  else {
    $d1['minute']--;
    $diff['second'] = 60-$d2['second']+$d1['second'];
  }
  //minutes
  if ($d1['minute'] >= $d2['minute']){
    $diff['minute'] = $d1['minute'] - $d2['minute'];
  }
  else {
    $d1['hour']--;
    $diff['minute'] = 60-$d2['minute']+$d1['minute'];
  }
  //hours
  if ($d1['hour'] >= $d2['hour']){
    $diff['hour'] = $d1['hour'] - $d2['hour'];
  }
  else {
    $d1['day']--;
    $diff['hour'] = 24-$d2['hour']+$d1['hour'];
  }
  //days
  if ($d1['day'] >= $d2['day']){
    $diff['day'] = $d1['day'] - $d2['day'];
  }
  else {
    $d1['month']--;
    $diff['day'] = date("t",$temp)-$d2['day']+$d1['day'];
  }
  //months
  if ($d1['month'] >= $d2['month']){
    $diff['month'] = $d1['month'] - $d2['month'];
  }
  else {
    $d1['year']--;
    $diff['month'] = 12-$d2['month']+$d1['month'];
  }
  //years
  $diff['year'] = $d1['year'] - $d2['year'];
  return $diff;   
}
*/

/*

function time_ago_string($d1, $d2) {
	$timeago = "";
	$diff = date_diff($d1, $d2);
	if($diff["year"] > 0) {
		$pl = ($diff["year"]>1)?"s":"";
		$timeago = $diff["year"] . " year" . $pl . " ago";
	} else if($diff["month"] > 1) {
		$pl = ($diff["month"]>1)?"s":"";
		$timeago = $diff["month"] . " month" . $pl . " ago";
	} else if($diff["day"] > 1) {
		$pl = ($diff["day"]>1)?"s":"";
		$timeago = $diff["day"] . " day" . $pl . " ago";
	} else if($diff["hour"] > 1) {
		$pl = ($diff["hour"]>1)?"s":"";
		$timeago = $diff["hour"] . " hour" . $pl . " ago";
	} else if($diff["minute"] > 1) {
		$pl = ($diff["minute"]>1)?"s":"";
		$timeago = $diff["minute"] . " minute" . $pl . " ago";
	} else {
		$timeago = "Now";
	}
	
	return $timeago;
}
*/


function vibe_related_block_block( $node, $nodes ) {
	$nodesarg = array();
	foreach($nodes as $n) {
		$nodesarg[] = node_load($n->sid);
	} 
	return theme_render_template("sites/all/themes/vibe/block-related-block.tpl.php",array("node"=>$node, "nodes"=>$nodesarg));
}

//function debug($var, $type = TRUE) {
  //drupal_set_message('<pre>'.var_export($var, TRUE).'</pre>', $type?'status':'error');
//}

function vibe_preprocess_box(&$vars, $hook) {
  switch($vars['title']) {
   case 'Post new comment':
    $vars['title'] = t("ADD A COMMENT &gt;&gt;");
  }
}

function vibe_comment_form($form) {

  // Rename some of the form element labels.
  $form['_author'] = null;
  $form['subject'] = null;
  $form['homepage'] = null;
  $form['comment_filter']['format'] = null;
  $form['comment_filter']['comment']['#title']  = ($form['author']['#value']) ? t('You are signed in as ' . $form['author']['#value'] . '. Please enter your comment below.') : t('Please enter a comment below.');
  $form['comment_filter']['comment']['#id']  = "vibe-edit-comment";
  
  if (empty($form['author']['#value'])) {
  	$form['name']['#value'] = "";
  }
  //$form['sumbit']['#value'] = t('');
  
  /*echo "<pre>";
  print_r($form['submit']);
  echo "</pre>";exit;*/
  
  
   // Remove the preview button
   $form['preview'] = NULL;

  return vibe_fb_comment_form() . drupal_render($form);
}

function vibe_imagecache($presetname, $path, $alt = '', $title = '', $attributes = NULL, $getsize = TRUE) {
  // Check is_null() so people can intentionally pass an empty array of
  // to override the defaults completely.
  if (is_null($attributes)) {
    $attributes = array('class' => 'imagecache imagecache-'. $presetname);
  }
  if ($getsize && ($image = image_get_info(imagecache_create_path($presetname, $path)))) {
    $attributes['width'] = $image['width'];
    $attributes['height'] = $image['height'];
  }

  $attributes = drupal_attributes($attributes);
  $imagecache_path = imagecache_create_path($presetname, $path);
  
  $file_create_url = (module_exists('cdn')) ? 'file_create_url' : 'url';
  $imagecache_url = $file_create_url($imagecache_path);
  
  return '<img src="'. $imagecache_url .'" alt='. check_plain($alt) .'" title="'. check_plain($title) .'" '. $attributes .' />';
}
/*

function get_next_node($nid) {
	//$node = node_load($nid);
	//return $node;
	
   $sql = "SELECT n.nid FROM node n WHERE n.nid > ";
   $sql .= $nid . " AND n.type in ('story','photo_gallery','advpoll_binary','video','event') AND n.status = 1 ORDER BY n.nid asc limit 1";

   $result = db_fetch_array(db_query(db_rewrite_sql($sql)));
   if (!$result) {
     return NULL;
   } else {
     return node_load($result['nid']);
   }
}

function get_prev_node($nid) {
	$sql = "SELECT n.nid FROM node n WHERE n.nid < ";
   $sql .= $nid . " AND  n.type in ('story','photo_gallery','advpoll_binary','video','event') AND n.status = 1 ORDER BY n.nid desc limit 1";
   $result = db_fetch_array(db_query(db_rewrite_sql($sql)));
   if (!$result) {
     return NULL;
   } else {
     return node_load($result['nid']);
   }
}

function ellipse($paragraph, $limit) {
	$clean_paragraph = strip_tags($paragraph);
	
	$substr = substr($clean_paragraph, 0, $limit);
	
	$pos1 = strrpos($substr, " ");

	$cleanstr = substr($substr, 0, $pos1+1);
	
	if(strlen($paragraph) > $limit) {
		$cleanstr .= "...";
	}
	
	return $cleanstr;
}

function excerpt($paragraph, $limit){
	
	$clean_paragraph = strip_tags($paragraph);
	
	$substr = substr($clean_paragraph, 0, $limit);
	
	$pos1 = strrpos($substr, "!");
	$pos2 = strrpos($substr, ".");
	$pos3 = strrpos($substr, "?");
	
	$lastpos = 0;
	if($pos1 > -1 && $pos1 > $pos2 && $pos1 > $pos3) {
		$lastpos = $pos1;
	} else if($pos2 > -1 && $pos2 > $pos3 && $pos2 > $pos1) {
		$lastpos = $pos2;
	} else if($pos3 > -1) {
		$lastpos = $pos3;
	} else {
		return $substr;
	}
	
	return substr($substr, 0, $lastpos+1);
}
*/

function user_submission_validate_image(&$form, &$form_state) {

	$validators = array(
    		'file_validate_is_image' => array(),
	    	'file_validate_image_resolution' => array('600x600'),
    		'file_validate_size' => array(30 * 1024),
  	);
  	
  	$destination = file_directory_path() . "/links/";
  	if ($file = file_save_upload('image', $validators, $destination)) {
    		// Remove the old picture.
    		if (isset($form_state['values']['image']->image_path) && file_exists($form_state['values']['image']->image_path)){
      			file_delete($form_state['values']['image']->image_path);
    		}
 
			$info = image_get_info($file->filepath);
    		
    		if(file_copy($file->filepath, $destination, FILE_EXISTS_REPLACE)) {
      			$form_state['values']['image'] = $file; 
      			//$destination . basename($file->filepath);
      			
      			foreach(imagecache_presets() as $imagecache_preset) {
      				$dest = imagecache_create_path($imagecache_preset["presetname"],$file->filepath);
      				imagecache_build_derivative($imagecache_preset["actions"], $file->filepath, $dest);
      			}
      		}
    		else {
    			echo "ERROR UPLOADING IMAGE";exit;
    		}
  	} else{
  		echo "ERROR SAVING FILE";
  	}
}


function user_submission_form_submit($form, &$form_state) {

	$title = $form_state["values"]["title"];
	$url = $form_state["values"]["url"];
	$description = $form_state["values"]["description"];
	$uid = $form_state["values"]["uid"];
	
	$image = get_object_vars($form_state["values"]["image"]);
	//var_dump($image);exit;
	//var_dump($image);exit;
	//var_dump($images);exit;
		
	
	$node = new StdClass();
	$node->type = "link";
	$node->status = 0;
	
	$node->title = $title;
	$node->body = $description;
	
	$node->field_link[]['value'] = $url;
	$node->field_user_submitted[]['value'] = "Yes";
	
	$node->field_link_image[] = $image;
	
	$node->uid = $uid;
	
	node_save($node);
	
  //db_query("INSERT INTO {table} (name, log, hidden) VALUES ('%s', %d, '%s')", $form_state['values']['name'], $form_state['values']['access']['log'],  $form_state['values']['hidden']);
  /*echo "YOUR FORM WAS SAVED";exit;*/
  /*drupal_set_message(t('Your form has been saved.'));*/
}

function user_submission_form($form_state) {
	global $user;
  // Description

  $form['#attributes'] = array('enctype' => 'multipart/form-data');
  
  $form['uid'] = array(
  	'#type' => 'value',
  	'#value' => $user->uid
  );
  
  $form['title'] = array(
  	'#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' =>  variable_get('description', ''),
    '#size' => 50,
    '#maxlength' => 255,
  );
  
  $form['url'] = array(
  	'#type' => 'textfield',
    '#title' => t('URL'),
    '#default_value' =>  variable_get('description', ''),
    '#size' => 50,
    '#maxlength' => 255,
  );
  
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Describe it'),
    '#default_value' =>  variable_get('description', ''),
    '#cols' => 60,
    '#rows' => 5,
  );
  
  $form['image'] = array(
  	'#type' => 'file',
  	'#title' => t('Image'),
  	'#description' => t('Click "Browse..." to select an image to upload.')
  );
  
  $form['#validate'][] = 'user_submission_validate_image';

  
  /*$form['hidden'] = array('#type' => 'value', '#value' => 'is_it_here');*/
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  return $form;
}

function vibe_preprocess_user_register(&$vars) {
  // That's where $form_markup is created
  list($size, $length) = explode('_', variable_get('fbconnect_button', NULL));    
  $items[] = array('data' => t('Sign in using Facebook ').fbconnect_render_button($size, $length), 'class' => 'fbconnect-button');
  $vars['fbconnect_button'] = array(
        '#value' => theme('item_list', $items),
        '#weight' => 1,
      );

      //print_r($vars);
      //$vars['form']['#redirect'] = array('value' => '/registration-sent');
      /*
  $vars['login_link'] = t('<span id="login_link">or <a class="thickbox" href="#TB_inline?height=430&amp;width=540&amp;inlineId=userloginModal&amp;modal=true" style="text-decoration: none; color: #000000;">LOGIN HERE</a></span>
    <div id="userloginModal" style="display:none; z-index: 3000;"><div class="large_title" style="font-size: 24px; color: #555555; margin-bottom: 3px;">Login or Register</div><div>' . drupal_get_form('user_login_block') . '</div><div class="form_buttons" style="text-align: center; margin: 0 auto; margin-top: -180px;"><div class="vibe_button" style="float: left; margin-left: 170px; background-color: #28aae1;"><a href="javascript:void(0)" onclick="tb_remove();">Close</a></div><div class="vibe_button" style="float: left; margin-left: 5px; background-color: #28aae1;"><a href="javascript:void(0)" onclick="$(\'#user-login-form\').submit()">Login</a></div></div></div>');
        */
  //$vars['form_markup'] = drupal_render($vars['form']);
}
