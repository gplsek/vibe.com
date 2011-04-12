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

<? //var_dump($fields['type']->content); ?>
<?//comment_count, created?>
<?php
	$title = $fields["title"];
	$teaser = $fields["teaser"];
	$node = node_load($fields['nid']->content); 
	$blog_type = $fields["phpcode_3"]->content;
	$celeb_icon = $fields["phpcode_4"]->content;
 	$raw_link = $fields["view_node"];
	preg_match("/href=\"(.*?)\"/",$raw_link->content,$matches);
	$link = $matches[1];
?>

<div class="latest_on_vibe_post">
	<div class="blog_post">
	
	
			<div class="latest_on_vibe_excerpt">
            <?php if($blog_type == '1') { ?>
    			<div id="celeb_icon"><img src="<?php print url($celeb_icon);?>" /></div>
			<? } ?>
			<div class="latest_on_vibe_title">
			
			<span class="latest_on_vibe_title_inner">
				 <span class="content_title"><a href="<?php print $link?>"><?php print preg_replace("/&amp;.*?;/","",htmlspecialchars($title->raw, ENT_NOQUOTES));?></a></span>
			</span>
		</div>
        <? if($fields['field_img_src_fid_1']->content){ ?>
		<div class="photo_title"><a href="<?php print $link?>"><img src="<? print $fields['field_img_src_fid_1']->content;?>" /></a></div>
		<? } ?>
              <?  if($fields['type']->content == 'Image')
                {
                    $photo = $fields['field_img_src_fid']->content; ?>
                    <div class="story_photo_title"><?php print $photo; ?></div>
               <? }
                else
                {
                    $photo = $fields['field_story_thumb_fid']->content; ?>
					<? if(!empty($photo)){?>
                    
					<div class="story_photo_title"><a href="<?php print $link?>"><img src="<? print $photo;?>" /></a></div>
               <? }} ?>
           
          
            
       
            
		<div class="latest_on_vibe_teaser">
			
			<?php print $contentBody;?>
			<? if ($fields['type']->content == 'Photo Gallery'){ ?>
         	
         	<div id="gallery_photos">
				<?php			
					$nodeCount = count($node->nodes);
					for($i = 0; $i < 3; $i++) {
						$imgLink = $link . "/" . $i;
						$imgSrc = theme('imagecache', 'image-gallery-thumb', $node->nodes[$i]->field_img_src[0]['filepath'], '', '');
						echo "<div class='image_node'><a href='$imgLink'>$imgSrc</a></div>";
					}
				?>
			</div>
        	<? } else if ($fields['type']->content == 'Video') { ?>
        	<? $node = node_load($fields['nid']->content);  //var_dump($node);?>
         	<div id="blog_video">
			<? 
				preg_match('/brightcove/',strtolower($node->field_vid_src[0]["embed"]),$matches);
				if($matches){
					$html_embed = str_get_html($node->field_vid_src[0]["embed"]);
					$split_html = split('flashvars="',$html_embed);					
					$split_base = split("base",$split_html[1]);
					$split_video = split("&",$split_base[0]);
					$split_id = split("=",$split_video[0]);
					
					$query = db_query("SELECT guid FROM rg_videos WHERE vid ='%s'",$split_id[1]);
					$results = db_fetch_object($query);	
					
					$scripts = '<script type="text/javascript">
									function rg_framework() {
										if (typeof RealGravity != "undefined") {
											var s_'.$m.' = new RealGravity();
											s_'.$m.'.setMode("playlist");
											s_'.$m.'.addItem("'.$results->guid.'");
											s_'.$m.'.setSize({width: 486, height: 412, playlist: 0});
											s_'.$m.'.setPlayerSkin("snel");
											s_'.$m.'.start(); //element id/ref, or empty to use document.write
										}
										else {
											setTimeout(rg_framework, 100);
										}
									}
									rg_framework()
								</script>';
								
					$embeds = $html_embed->find('embed');
					$embeds[0]->outertext = $scripts;	
					
					$node->field_vid_src[0]["embed"] = $embeds[0]->outertext;		
				}
						
			?>
			<?php print $node->field_vid_src[0]["embed"]?></div>
         	<? } ?> 
         	<? if($fields['type']->content == 'Photo Gallery')
		 		{ ?>
					<span class="photo_count"><a href="<?php print $link; ?>/0"><?php echo count($node->nodes); ?> Photos</a><span style="margin-left: 4px; text-decoration:none; color:#098ec1;">&gt;&gt;</span></span> 
            	<? } else
					{	?>
            		<span style='margin-left: 4px'><b>&gt;&gt; <a href="<?php print $link?>" style="color: #000000; text-decoration:underline;"><?php print get_more_text($fields[type]->raw);?></a></b></span>
           	 <? } ?>
			</div>
        
		<? if($fields['type']->content == 'Photo Gallery')
		 	{ ?>
            	<div class="latest_on_vibe_info" style="margin-top:45px;">
            <? } else
				{	?>
        		<div class="latest_on_vibe_info">
                 <? } ?>
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
</div>