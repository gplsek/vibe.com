<?php

$reactions = get_top_reactions();

?>
<div id="hot_on_vibe" style="margin-top: -16px;">
	<?foreach($reactions as $reaction) {?>

    <div class="hot_on_vibe_post">
	<div class="hot_on_vibe_thumbnail">
		<?php $thumbnail = $reaction->filepath?>
		<a href="/<?php print $reaction->link?>">
		<?if($thumbnail) {?>
			<img width='75' height='75' src ='/<?php print $thumbnail;?>' border='0'>
		<?} else {?>
			<img width='75' height='75' src='/sites/all/themes/vibe/images/MISSINGPHOTO_ICON_SML.jpg' border='0'>
		<?}?>
		</a>
	</div>
	<div class="hot_on_vibe_excerpt">
		<div class="hot_on_vibe_reaction_title">
			<?php print $reaction->reaction_title;?> (<?php print $reaction->count?>)
		</div>
		<div class="hot_on_vibe_title">
			<?php $title = $fields["title"];?>
			<?php $category = $fields["name"];?>
			<?php $raw_link = $fields["view_node"];
				  preg_match("/href=\"(.*?)\"/",$raw_link->content,$matches);
				  $link = $matches[1];
			?>
			<span class="hot_on_vibe_title_inner">
				<span class="hot_on_vibe_content_title"><a href="/<?php print $reaction->link?>"><?php print $reaction->title;?></a></span>
			</span>
		</div>

		<div class="hot_on_vibe_info">
			<?php $comment_count = $fields["comment_count"]?>
			<?php $created_date = $reaction->created?>
			<?php 
				
				$now = time();
				$posttime = $created_date;
				$date = date("Y-m-d H:i:s", $posttime);
				$timeago = time_ago_string($now, $posttime);
			?>
			
			<div class="hot_on_vibe_author_photo">
				<?if($reaction->picture) {?>
					<img src='/<?php print $reaction->picture;?>' width='30' height='30' border='0'>
				<?} else {?>
					<img width='30' height='30' src='/sites/all/themes/vibe/images/MISSINGUSER_ICON_SML.jpg' border='0'>
				<?}?>
			</div>
			<div class="hot_on_vibe_info_text">
				<div class="hot_on_vibe_info_date_comments">
					<?php print $timeago . " / " . $reaction->comment . " Comments"?>
				</div>
				<div class="hot_on_vibe_info_from">
					FROM: <?php print $reaction->username?>
				</div>
			</div>
		</div>
	</div>
	<div class="clear-block"></div>
			
	</div>
	<?}?>

</div>