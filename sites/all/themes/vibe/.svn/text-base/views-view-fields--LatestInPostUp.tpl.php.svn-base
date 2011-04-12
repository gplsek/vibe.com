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
 
$title = $fields["title"];
$category = $fields["name"];
$raw_link = $fields["view_node"];
preg_match("/href=\"(.*?)\"/",$raw_link->content,$matches);
$link = $matches[1];

$img_icon = "/sites/all/themes/vibe/images/".$fields[type]->raw."_icon.png";
$sitename = null;
?>


<?//comment_count, created?>
<div class="latest_on_vibe_post">
	<div class="latest_on_vibe_thumbnail">
		<div class="latest_on_vibe_thumbnail_img">
		<a target="_blank" href="<?=$link?>">
		<?if($fields["type"]->content == "Link") {?>
			<?php $thumbnail = $fields["field_link_image_fid"]->content?>
			<?php print $thumbnail->content;?>
			<?if($thumbnail->content) {?>
				<?php print $thumbnail->content;?>
			<?} else {?>
				<img width='75' src='/sites/all/themes/vibe/images/MISSINGPHOTO_ICON_SML.jpg' border='0'>
			<?}?>
		<?} else if($fields["type"]->content == "Feed item") {?>
				<?php
				if($row->feed_item_guid) {
					$url = $row->feed_item_guid;
					if(!preg_match("/^http:/",$url)) {
						$url = $row->feed_item_url;
					}
					preg_match("/(http:\/\/)(.*?)\.(.*?)\.(.*?)\//",$url,$matches);
	
					$sitename = $matches[2] . "." . $matches[3] . "." . $matches[4];
	
					$siteimage = "/sites/all/themes/vibe/images/" . $matches[3] . "_logo.png";
					
					$style = "";
					if(!file_exists($_SERVER['DOCUMENT_ROOT'] . "/" . $siteimage)) {
						$siteimage = "/sites/all/themes/vibe/images/MISSINGPHOTO_ICON_SML.jpg";
						
					} else {
						$style = "style='border: none;'";
					}
				}
				?>
				<img width='75'  src='<?=$siteimage?>' border='0' <?=$style?>>
		<?} else {?>
			<img width='75' src='/sites/all/themes/vibe/images/MISSINGPHOTO_ICON_SML.jpg' border='0'>
		<?}?>
		</a>
		</div>
		
		<?/*<div class="latest_on_vibe_icon"><img src='<?php print $img_icon;?>' border='0'></div>*/?>


	</div>
	<div class="latest_on_vibe_excerpt">
		<div class="latest_on_vibe_title">
						<span class="latest_on_vibe_title_inner">
				<span class="content_title"><a target="_blank" href="<?=$link?>"><?php print preg_replace("/&amp;.*?;/","",$title->content);?></a></span>
			</span>
		</div>
		<div class="latest_on_vibe_teaser">
			<?php $teaser = $fields["teaser"];?>
			<?php print excerpt($fields["body"]->raw, 500);?>
			<div><b><a href="<?php print $link?>" style="text-decoration: none; color: #000000;"><?php print get_more_text($fields[type]->raw);?></a></b></div>
		</div>
		<?if($sitename) {?>
			<div class="latest_on_vibe_source">
				<div style="float: left;">From: </div>
				<div class="latest_on_vibe_source_icon"><img src='/sites/all/themes/vibe/images/vln_icon.png' border='0'></div>
				<div class="latest_on_vibe_source_text"><a target="_blank" href="http://<?php print $sitename?>"><?php print str_replace("WWW.","",strtoupper($sitename));?></a></div>
				<div class="clear-block"></div>
			</div>
		<?}?>
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
		
		<div id="content_rating_<?=$fields["nid"]->content?>" style="float: right"></div><div class="clear-block"></div>
		<?php print flag_create_link("rating_plus", $fields["nid"]->content);?>

		
	</div>
	<div class="clear-block"></div>
		
</div>