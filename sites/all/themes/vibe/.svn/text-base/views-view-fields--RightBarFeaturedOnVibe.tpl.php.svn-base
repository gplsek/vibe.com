<?php
// $Id: views-view-fields.tpl.php,v 1.6 2008/09/24 22:48:21 merlinofchaos Exp $
/**
 * @file views-view-fields.tpl.php
 * Default simple view template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->separator: an optional separator that may appear before a field.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */
if (!empty($fields['phpcode_1']->content)) {
	$title = ellipse($fields["phpcode_1"]->content, 30);
} else {
	$title = ellipse($fields["title"]->raw, 35);
}

if (!empty($fields['phpcode']->content)) {
	$subtitle = ellipse($fields['phpcode']->content, 30);
} else {
	$subtitle = '';
}
if (!empty($fields["field_link_value"]->content)) {
	$link = $fields["field_link_value"]->content;
} else {
	$raw_link = $fields["view_node"];
	preg_match("/href=\"(.*?)\"/",$raw_link->content,$matches);
	$link = $matches[1];
}
$type = $fields["type"];

$thumbnail = ($fields["field_story_thumb_fid"] && $fields["field_story_thumb_fid"]->content) ? $fields["field_story_thumb_fid"] : $fields["field_img_src_fid"];



?>

<? //var_dump($row);//print var_export($fields['phpcode'], TRUE);?>
<? //var_dump(array_keys($fields));?>
<? //var_dump($type->content); ?>
<?//comment_count, created?>
<div class="right_bar_gallery_item">
		 <div class="right_bar_gallery_title">
		 		<div class="gallery_title_bg"></div>
				 
                 <?php if($type->content == "Link") { ?>
                 <div class="gallery_title"><a style="text-decoration:none; color:#ffffff" href="<?php print url($link);?>"><?php print preg_replace("/&amp;.*?;/","",htmlspecialchars($title, ENT_NOQUOTES));?></a></div>
                 <? } else { ?>
					 <div class="gallery_title"><a style="text-decoration:none; color:#ffffff" href="<?php print $link;?>"><?php print preg_replace("/&amp;.*?;/","",htmlspecialchars($title, ENT_NOQUOTES));?></a></div>
                     <? } ?>
		</div>
		
			<?php
			//print_r($type);
				if($type->content == "Video")
				{ ?>
                <div class="right_bar_thumbnail">
					<div class="video_thumb_arrow"></div><a style="text-decoration:none; color:#ffffff" href="<?php print $link;?>"><?php print $thumbnail->content;?></a></div>
				<?php 
				}
				elseif($type->content == "Link")
				{ ?>
					<div class="right_bar_thumbnail">
						<a style="text-decoration:none; color:#ffffff" href="<?php print url($link);?>"><?php print $thumbnail->content;?></a></div>
				<? }
				else
				{ ?>
					<div class="right_bar_thumbnail">
						<a href="<?php print $link;?>"><?php print $thumbnail->content;?></a></div>
				<?php } ?>
			
			
</div>
