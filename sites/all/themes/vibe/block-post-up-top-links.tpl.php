<?php $links = get_top_links();?>

<div style="margin-top: -20px;"></div>

<?foreach($links as $link) {?>
<div class="post-up-top-links">
	<div class="hot_on_vibe_thumbnail">
		<?php $thumbnail = $fields["field_story_thumb_fid"]?>
		<a href="/<?php print $link->dst?>">
		
		<?if($link->filepath && !preg_match("/tmp\//",$link->filepath)) {?>
			<img width='75' src='/<?php print $link->filepath;?>' border='0'>
		<?} else {?>
			<img width='75' src='/sites/all/themes/vibe/images/MISSINGPHOTO_ICON_SML.jpg' border='0'>
		<?}?>
		</a>
	</div>
	<div class="hot_on_vibe_excerpt">
		<div class="hot_on_vibe_title">
			<?php $title = $link->title;?>
			<span class="hot_on_vibe_title_inner">
				<span class="hot_on_vibe_content_title"><a href="/<?php print $link->dst?>"><?php print $title;?></a></span>
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
				$posttime = $link->changed;
				$date = date("Y-m-d H:i:s", $posttime);
				$timeago = time_ago_string($now, $posttime);
			?>
			
			<div class="hot_on_vibe_author_photo">
				<?if($link->picture) {?>
					<img width='30' height='30' src='/<?php print $link->picture;?>' border='0'>
				<?} else {?>
					<img width='30' height='30' src='/sites/all/themes/vibe/images/MISSINGUSER_ICON_SML.jpg' border='0'>
				<?}?>
			</div>
			<div class="hot_on_vibe_info_text">
				<div class="hot_on_vibe_info_date_comments">
					<?php print $timeago . " / " . $link->comment . " Comments"?>
				</div>
				<div class="hot_on_vibe_info_from">
					FROM: <a href='/user/<?php print $link->name?>'><?php print $link->name?></a>
				</div>
			</div>
		</div>
	</div>
	<div class="clear-block"></div>
		
</div>
<?}?>
