<?php
// $Id: views-view.tpl.php,v 1.13 2009/06/02 19:30:44 merlinofchaos Exp $
/**
 * @file views-view.tpl.php
 * Main view template
 *
 * Variables available:
 * - $css_name: A css-safe version of the view name.
 * - $header: The view header
 * - $footer: The view footer
 * - $rows: The results of the view query, if any
 * - $empty: The empty text to display if the view is empty
 * - $pager: The pager next/prev links to display, if any
 * - $exposed: Exposed widget form/info to display
 * - $feed_icon: Feed icon to display, if any
 * - $more: A link to view more, if any
 * - $admin_links: A rendered list of administrative links
 * - $admin_links_raw: A list of administrative links suitable for theme('links')
 *
 * @ingroup views_templates
 */
?>
<? $url = curPageURL(); 
   $url_array = split('/',$url);
   $split_tag = strpos($url_array[4],"?");
   
   if($split_tag === FALSE){
   		$tag_name = $url_array[4];
   }
   else{
	   $tag_array = explode("?",$url_array[4]);
	   $tag_name = $tag_array[0];
   }
   
   $tag_name = str_replace("%20"," ",$tag_name);
   
?>
<div id="latest_on_vibe">
	<div id="latest_on_tag_header">
    	<div class="latest_on_tag_header_text">Tag Results for <span class="latest_on_vibe_tags"><? print ucwords($tag_name); ?></span></div>
        <div class="clear-block"></div>
    </div>
    <div class="clear-block"></div>
    <div id="tag_search_wrapper">
    	<div id="search_link"><a href="<? $search = "/search/node/".$tag_name; print $search; ?>">search</a></div>
        <div id="tag_link"><a href="<? print $tag_name; ?>">tags</a></div>
        
    </div>
    <div class="clear-block"></div>
	<div id="latest_on_vibe_posts">
		<?php print $rows;?>
	</div>
</div>
<div><?php print $pager?></div>