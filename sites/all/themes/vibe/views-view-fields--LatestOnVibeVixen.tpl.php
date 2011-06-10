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
 /* $contentBody = ($fields["field_excerpt_value"] && $fields["field_excerpt_value"]->content) ? $fields["field_excerpt_value"]->content : ellipse($fields["body"]->raw, 165); */
?>

<? //var_dump($fields["field_excerpt_value"]->content);?>
<?//php print $fields[type]->raw;?>
<?//comment_count, created?>
<div class="latest_on_vixen_post">
	<?php
	$celeb_icon = $fields["phpcode"]->content;
	$blog_type = $fields["phpcode_1"]->content;
	$blog_base_uri = $fields["phpcode_2"]->content;
	$blog_name = $fields["phpcode_3"]->content;
	$terms = $fields['tid']->raw;
	
	//var_dump($celeb_icon);
	
	if( $blog_type == '1') { ?>
    <div id="latest_on_vibe_celeb_icon"><img src="<?php print url($celeb_icon);?>" /></div>
    <? } else { ?>
    <div class="latest_on_vibe_thumbnail">
		<?php $thumbnail = ($fields["field_story_thumb_fid"] && $fields["field_story_thumb_fid"]->content) ? $fields["field_story_thumb_fid"] : $fields["field_img_src_fid"]?>
		<div class="latest_on_vibe_vixen_thumbnail_img"><?php print $thumbnail->content;?></div>
		<?php if (in_array($fields['type']->raw, array('video', 'audio'))) {?><div class="latest_on_vibe_icon"><img src='<?php print $img_icon;?>' border='0'></div> <?php } ?>
	</div>
    <? } ?>
	<div class="latest_on_vibe_excerpt">
		<div class="latest_on_vibe_title">
			
			<?php $title = $fields["title"];
				$category = $fields["tid"]; 
				
				/*$categories = explode(", ",$category->content);
				
				for($k=0; $k<count($categories); $k++)
				{
					
						
					if($categories[$k] != "vixen")
					{
						array_pop($categories[$k]);
						
					}
					else
					{					
						unset($categories[$k]);
					}
					
				}
				
				$categories = array_values($categories);
				
				//var_dump(count($categories));
				*/
				if($category->content == 'Blog')
				{ 
					$category->content = 'Blogs';
				}?>
			<?php $raw_link = $fields["view_node"];
				  preg_match("/href=\"(.*?)\"/",$raw_link->content,$matches);
				 $link = $matches[1];
			?>
			<span class="latest_on_vibe_title_inner">
				<?php if($blog_type == '1') { ?>
                		<div id="vixen_read_all_post_link">
						  <? if ($blog_base_uri != "" || $blog_base_uri != NULL)
                          { ?>
                            <a href="<?php print $blog_base_uri;?>"><?php print strtoupper($blog_name); ?><span class="more_arrows">&gt;&gt; </span></a><br />
                            <? } ?>
                        </div>
                  <? }else { ?>              
               <!-- <div class="vixen_category_title">
             		<?php //for($i=0; $i < count($categories); $i ++) { 
						//if($categories[$i] == 'Blog')
						//{
						//	$categories[$i] = 'Blogs';
						//}
						//var_dump("<br />i is: ".$i);
						//var_dump("<br /> cat is: ".$categories[$i]);
                    	//if( count($categories) > 1 &&  $i < count($categories)-1 ){ if($categories[$i] != 'From The Magazine') { ?>
                    	<a href="/<?php //print strtolower($categories[$i]);?>/"><?php //print $categories[$i].","; 	?>
                        </a>
                        <?// }} else { ?>
                        <a href="/<?php// print strtolower($categories[$i]);?>/"><?php //print $categories[$i]; 	?>
                        </a>
                        <?// } ?>
                    <? //} ?>
                </div> -->
                <? } ?>
                <div class="vixen_content_title"><a href="<?php print $link?>"><?php print preg_replace("/&amp;.*?;/","",htmlspecialchars($title->raw, ENT_NOQUOTES));?></a></div>
			</span>
		</div>
		<div class="latest_on_vibe_teaser">
			<?php print $fields['teaser']->content;?>
			<span style='margin-left: 4px'><b>&gt;&gt; <a href="<?php print $link?>" style="color: #000000; text-decoration:underline;"><?php print get_more_text($fields[type]->raw);?></a></b></span>
		</div>
		<div class="latest_on_vibe_vixen_info">
			<?php $comment_count = $fields["comment_count"]?>
			<?php $created_date = $fields["created"]?>
			<?php 
				
				$now = time();
				$posttime = strtotime(str_replace(" - ", " ",$created_date->content));
				//echo $posttime;
				//$date = date("Y-m-d H:i:s", $posttime);
				//echo "::" . $date;
				$timeago = time_ago_string($now, $posttime);
			?>
			
				
			<a href="<?php print $link?>" style="text-decoration: none; "><?php print $timeago . " / " . $comment_count->content . " comments"?></a>
		</div>
	</div>
 
	<div class="clear-block"></div>
	
</div>