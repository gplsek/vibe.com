<?php
// $Id: views-view-row-rss.tpl.php,v 1.1 2008/12/02 22:17:42 merlinofchaos Exp $
/**
 * @file views-view-row-rss.tpl.php
 * Default view template to display a item in an RSS feed.
 *
 * @ingroup views_templates
 */

 preg_match("/<p>(.*?)<\/p>/",html_entity_decode($description, ENT_QUOTES, 'UTF-8'), $matches);
 $text = ($matches[1]) ? $matches[1] : $title;

 preg_match("/(<img.*?>)/",html_entity_decode($description, ENT_QUOTES, 'UTF-8'), $imgmatches);
 $image = ($imgmatches[1]) ? $imgmatches[1] : "";

 $description = htmlentities($image . "<br>" . $text, ENT_QUOTES, 'UTF-8');
?>
  <item>
    <title><?php print $title; ?></title>
    <link><?php print $link; ?></link>
    <description><?php print $description;?></description>
    <?php print $item_elements; ?>
  </item>
