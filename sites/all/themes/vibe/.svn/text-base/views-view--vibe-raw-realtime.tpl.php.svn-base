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

<div id="latest_on_vibe_header" style="margin-bottom: 8px; padding-left: 0px; "><div class="latest_on_vibe_header_text" style="color: #c0100b; font-size: 15px;">VIBE <span class="latest_on_vibe_section" style="color: #a9a8b2;">POST UP</span> REAL TIME</div><div id="latest_on_vibe_header_bar"  style="width: 80px; background-color: #c0100b;"></div><div class="clear-block"></div></div>

<?php
$qtid = 2; // write here your quicktabs id.
$quicktabs = quicktabs_load($qtid);
print theme('quicktabs', $quicktabs);
?>
