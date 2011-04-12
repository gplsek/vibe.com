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

<div id="latest_on_vibe">
	<div id="latest_on_vibe_header"><div class="latest_on_vibe_header_text">Latest in <span class="latest_on_vibe_section">Post Up</span></div><div id="latest_on_vibe_header_bar" style="width: 315px;"></div><div class="clear-block"></div></div>
	<div id="latest_on_vibe_posts">
		<?php print $rows;?>
	</div>
</div>
<div><?php print $pager?></div>