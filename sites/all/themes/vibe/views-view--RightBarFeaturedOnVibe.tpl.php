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

    <div id="featured_right_bar" class="right_bar_featured">
        <div class="right_bar_gallery_header">Featured on <span>Vibe</span></div>
        <div class="right_bar_gallery_header_bar"></div>
        <div class='clear-block'></div>
	
		<?php print $rows;?>
    </div>
<div class='clear-block'></div>
<?php// print $attachment_after; ?>