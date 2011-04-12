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
<?php 
  	$blog_name = $fields["phpcode"];
	$blog_id = $fields["phpcode_1"];
	$blog_base_uri = $fields["phpcode_2"];
	$picture = $fields["phpcode_3"];
	$blog_type = $fields['phpcode_5']; 
	$term = $fields['type']->content;
?>

<? //var_dump(array_keys($fields));?>
<?//comment_count, created?>

<div class="latest_on_blogs_post">
	<div class="latest_on_blogs_vibe_thumbnail">
		<?php $thumbnail = ($fields["field_story_thumb_fid"] && $fields["field_story_thumb_fid"]->content) ? $fields["field_story_thumb_fid"] : $fields["field_img_src_fid"]?>
		
		<?php
		if($blog_type->content == 0){ 
			?><div class="latest_on_vibe_thumbnail_img" ><a href="/<?php print $blog_base_uri->content;?>"><?php print $thumbnail->content;?></a><?
		}
		else
		{?>
		<div class="latest_on_vibe_blogs_thumbnail_img"><a href="/<?php print $blog_base_uri->content;?>"><img src="<?php print $fields['phpcode_4']->content; ?>" /></a>
		<?php } ?>
		</div>
		<!--  div class="latest_on_vibe_icon"><img src='<?php //print $img_icon;?>' border='0'></div -->
	</div>
	<div class="latest_on_vibe_excerpt">
		
        <div class="latest_on_vibe_title">
            <div id="read_all_post_link">
              <? if ($blog_base_uri->content != "" || $blog_base_uri->content != NULL)
			  { ?>
                <a href="<?php print $blog_base_uri->content;?>"><?php print strtoupper($blog_name->content); ?><span class="more_arrows">&gt;&gt; </span></a><br />
                <? } ?>
            </div>
			<?php $title = $fields["title"];?>
			<?php $category = $fields["name"];?>
			<?php $raw_link = $fields["view_node"];
				  preg_match("/href=\"(.*?)\"/",$raw_link->content,$matches);
				  $link = $matches[1];
			?>
			<span class="latest_on_vibe_title_inner">
				 <span class="content_title"><a href="<?php print $link;?>"><?php print preg_replace("/&amp;.*?;/","",htmlspecialchars($title->raw, ENT_NOQUOTES));?></a></span>
			</span>
		</div>
		<div class="latest_on_vibe_teaser">
			<?php $teaser = $fields["teaser"];?>
			<a href="<?php print $link; ?>"><?php print $contentBody;?></a>
			<span style='margin-left: 4px'><b>&gt;&gt; <a href="<?php print $link?>" style="color: #000000; text-decoration:underline;"><?php print get_more_text($fields[type]->raw);?></a></b></span>
		</div>
		<div class="latest_on_vibe_info" >
			<?php $comment_count = $fields["comment_count"]?>
			<?php $created_date = $fields["created"]?>
			<?php 
				
				$now = time();
				$posttime = strtotime(str_replace(" - ", " ",$created_date->content));
				$date = date("Y-m-d H:i:s", $posttime);
				$timeago = time_ago_string($now, $posttime);
			?>
            
			<a href="<?php print $link?>"><?php print $timeago . " / " . $comment_count->content . " comments"?></a>
		</div>	
	</div>
	<div class="clear-block"></div>
</div>
