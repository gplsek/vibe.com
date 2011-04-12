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
 
 $img_icon = "/sites/all/themes/vibe/images/".$fields[type]->raw."_icon.png";
  $contentBody = ($fields["field_excerpt_value"] && $fields["field_excerpt_value"]->content) ? $fields["field_excerpt_value"]->content : ellipse($fields["body"]->raw, 165);
?>

<?//var_dump(array_keys($fields));?>
<?//comment_count, created?>
<div class="latest_on_vibe_post">
	<div class="latest_on_vibe_thumbnail">
		<?php $thumbnail = ($fields["field_story_thumb_fid"] && $fields["field_story_thumb_fid"]->content) ? $fields["field_story_thumb_fid"] : $fields["field_img_src_fid"]?>
		<div class="latest_on_vibe_thumbnail_img"><?php print $thumbnail->content;?></div>
		<!--  div class="latest_on_vibe_icon"><img src='<?php print $img_icon;?>' border='0'></div -->
	</div>
	<div class="latest_on_vibe_excerpt">
		<div class="latest_on_vibe_title">
			<?php $title = $fields["title"];?>
			<?php $category = $fields["name"];?>
			<?php $raw_link = $fields["view_node"];
				  preg_match("/href=\"(.*?)\"/",$raw_link->content,$matches);
				  $link = $matches[1];
			?>
			<span class="latest_on_vibe_title_inner">
				 <span class="content_title"><a href="<?php print $link?>"><?php print preg_replace("/&amp;.*?;/","",htmlspecialchars($title->raw, ENT_NOQUOTES));?></a></span>
			</span>
		</div>
		<div class="latest_on_vibe_teaser">
			<?php $teaser = $fields["teaser"];?>
			<?php print $contentBody;?>
			<div><b><a href="<?php print $link?>" style="text-decoration: none; color: #000000;"><?php print get_more_text($fields[type]->raw);?></a></b></div>
		</div>
		<div class="latest_on_vibe_info">
			<?php $comment_count = $fields["comment_count"]?>
			<?php $created_date = $fields["created"]?>
			<?php 
				
				$now = time();
				$posttime = strtotime(str_replace(" - ", " ",$created_date->content));
				$date = date("Y-m-d H:i:s", $posttime);
				$timeago = time_ago_string($now, $posttime);
			?>
			
				
			<a href="<?php print $link?>" style="text-decoration: none; color:#000000;"><?php print $timeago . " / " . $comment_count->content . " comments"?></a>
		</div>
	</div>
	<div class="clear-block"></div>
		
</div>