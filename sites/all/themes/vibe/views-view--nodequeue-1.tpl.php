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

<div id="main_featured_home">
<div id="billboard_nav">
		<div id="billboard_left_arrow"><a href="#"><span>Left arrow</span></a></div>
		<div id="billboard_steps">
			<div id="billboard_step_1" class="billboard_step"></div>
			<div id="billboard_step_2" class="billboard_step"></div>
		    <div id="billboard_step_3" class="billboard_step"></div>
		    <div id="billboard_step_4" class="billboard_step"></div>
		</div>
		<div id="billboard_right_arrow"><a href="#"><span>Right arrow</span></a></div>
	</div>
	<div id="hp_billboard">
	<?php echo $rows; ?>
	</div>
</div>

<script type='text/javascript' src='/sites/all/themes/vibe/js/billboard.js'></script>