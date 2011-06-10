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
?>

<?//var_dump(array_keys($fields));?>
<?//comment_count, created?>
<?php $thumbnail = ($fields["field_story_thumb_fid_1"] && $fields["field_story_thumb_fid_1"]->content) ? $fields["field_story_thumb_fid_1"] : $fields["field_img_src_fid_1"]?>
<?if($thumbnail->content) {?>
<?php
	preg_match("/href=\"(.*?)\"/",$fields["view_node"]->content,$matches);
				  $link = $matches[1];
?>
<li class="latest_on_vibe_thumbnail_horizontal" style="position: relative; background-color: transparent; list-style: none; width: 172px; height: 137px;">
	
	<div style="position: relative; margin-top: 87px; margin-left: 2px; width: 158px; height: 40px; background-color: #000000; opacity: .5; filter: alpha(opacity = 50); z-index: 12; "></div>
	
	<div style="position: absolute; top: 90px; width: 150px; height: 37px; left: 5px; z-index: 15; color: #ffffff; overflow: hidden;  font-size: 11px !important; line-height: 12px !important;"><a style="color: #ffffff; text-decoration: none;" href="<?php print $link?>"><?php print $fields["title"]->content?></a></div>
	
	<div style="position: absolute; top: 0px; left: 0px; width: 162px; height: 127px; z-index: 10; overflow: hidden;"><?php print $thumbnail->content;?></div>
	<div style="position: absolute; top: 4px; left: 6px; width: 162px; height: 127px; z-index: 5; background-color: #000000; opacity: .2; filter: alpha(opacity = 20); /*background: transparent url(/sites/all/themes/vibe/images/dropshadow-bg.png) no-repeat;*/"></div>
 	
</li>
<?}?>