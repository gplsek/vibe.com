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
 //var_dump(array_keys($fields));
if (!empty($fields['phpcode']->content)) {
	$title = ellipse($fields["phpcode"]->content, 50);
} else {
	$title = ellipse($fields["title"]->raw, 50);
}
$thumbnail = ($fields["field_story_thumb_fid"] && $fields["field_story_thumb_fid"]->content) ? $fields["field_story_thumb_fid"] : $fields["field_img_src_fid"];

$raw_link = $fields["view_node"];
preg_match("/href=\"(.*?)\"/",$raw_link->content,$matches);
$link = $matches[1];
?>

<?//var_dump(array_keys($fields));?>
<?//comment_count, created?>
<div class="right_bar_list_item">
	<div id="top_5_thumb">
    	<?php print $thumbnail->content; ?>
    </div>
    <div id="top_5_content">
    	<div id="top_5_wrapper">
        <span class="position">
			<?php print $fields['counter']->content; ?>.
        </span>
        <div class="title">
            <strong>
           	<a href="<?php print $link;?>"><?php print preg_replace("/&amp;.*?;/","",htmlspecialchars($title, ENT_NOQUOTES));?></a>
            </strong>
        </div>
        </div>
    </div>
</div>
