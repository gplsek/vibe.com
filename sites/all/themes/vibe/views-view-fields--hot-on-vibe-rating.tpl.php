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

<?

var_dump(array_keys($fields));

?>
<?//comment_count, created?>
<div class="hot_on_vibe_post">
	<div class="hot_on_vibe_thumbnail">
		<?php $thumbnail = $fields["field_story_thumb_fid"]?>
		<?if($thumbnail->content) {?>
			<?php print $thumbnail->content;?>
		<?} else {?>
			<img width='75' height='75' src='/sites/all/themes/vibe/images/MISSINGPHOTO_ICON_SML.jpg' border='0'>
		<?}?>
	</div>
	<div class="hot_on_vibe_excerpt">
		<div class="hot_on_vibe_title">
			<?php $title = $fields["title"];?>
			<?php $category = $fields["name"];?>
			<?php $raw_link = $fields["view_node"];
				  preg_match("/href=\"(.*?)\"/",$raw_link->content,$matches);
				  $link = $matches[1];
			?>
			<span class="hot_on_vibe_title_inner">
				<span class="hot_on_vibe_content_title"><a href="<?php print $link?>"><?php print $title->content;?></a></span>
			</span>
		</div>
		<?/*<div class="hot_on_vibe_teaser">
			<?php $teaser = $fields["teaser"];?>
			<?php print $teaser->content;?>
		</div>*/?>
		<div class="hot_on_vibe_info">
			<?php $comment_count = $fields["comment_count"]?>
			<?php $created_date = $fields["created"]?>
			<?php 
				
				$now = time();
				$posttime = strtotime(str_replace(" - ", " ",$created_date->content));
				$date = date("Y-m-d H:i:s", $posttime);
				$timeago = time_ago_string($now, $posttime);
			?>
			
			<div class="hot_on_vibe_author_photo">
				<?if(preg_match("/img/",$fields["picture"]->content)) {?>
					<?php print $fields["picture"]->content;?>
				<?} else {?>
					<img width='30' height='30' src='/sites/all/themes/vibe/images/MISSINGUSER_ICON_SML.jpg' border='0'>
				<?}?>
			</div>
			<div class="hot_on_vibe_info_text">
				<div class="hot_on_vibe_info_date_comments">
					<?php print $timeago . " / " . $comment_count->content . " Comments"?>
				</div>
				<div class="hot_on_vibe_info_from">
					FROM: <?php print $fields["name"]->raw; ?>
				</div>
			</div>
		</div>
	</div>
	<div class="clear-block"></div>
		
</div>