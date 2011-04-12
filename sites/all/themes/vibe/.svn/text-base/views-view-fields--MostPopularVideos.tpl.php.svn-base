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
$title = preg_replace("/&amp;.*?;/","", strip_tags($fields['title']->content));
$url = drupal_get_path_alias('node/' . $row->nid);
?>

<?// var_dump($row);?>
<?//comment_count, created?>
<div class="video_thumb">
	<div class="thumb_img">
	<?php $thumbnail = ($fields["field_story_thumb_fid"] && $fields["field_story_thumb_fid"]->content) ? $fields["field_story_thumb_fid"] : $fields["field_img_src_fid"]?>
	<?php print $thumbnail->content;?>
	</div>
	<div class="thumb_title"><a href="<?php print $url; ?>" title="<?php print $title; ?>"><?php print ellipse($title, 36);?></a></div>
</div>
