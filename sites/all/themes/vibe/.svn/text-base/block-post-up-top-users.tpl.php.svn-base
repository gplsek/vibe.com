
<?php $users = get_top_users();?>

<div style="margin-top: -20px;"></div>
<?foreach($users as $user){?>

<div class="post-up-top-users">
	<div class="hot_on_vibe_thumbnail">
		<a href="/users/<?=$user->name?>">
		<?if($user->picture) {?>
			<img width='75' src='/<?php print $user->picture;?>' border='0'>
		<?} else {?>
			<img width='75' src='/sites/all/themes/vibe/images/MISSINGPHOTO_ICON_SML.jpg' border='0'>
		<?}?>
		</a>
	</div>
	<div class="hot_on_vibe_excerpt">
		<div class="hot_on_vibe_title">
			<?php $title = strtoupper($user->name);?>
			<span class="hot_on_vibe_title_inner">
				<span class="hot_on_vibe_content_title"><a href="/users/<?=$user->name?>"><?php print $title;?></a></span>
			</span>
		</div>
		<div class="hot_on_vibe_info">
			<?php $comment_count = $fields["comment_count"]?>
			<?php $created_date = $fields["created"]?>
			<?php 
				
				$now = time();
				$posttime = $user->created;
				$date = date("Y-m-d H:i:s", $posttime);
				$timeago = time_ago_string($now, $posttime);
			?>
			
			<div class="hot_on_vibe_info_text">
				<div class="hot_on_vibe_info_date_comments">
					<div>Joined <?php print $timeago?></div>
					<div><?php print $user->comment_count?> Comments</div>
				</div>
				<div class="hot_on_vibe_info_from">
					
					Rating: <?php print $user->count?></a>
				</div>
			</div>
		</div>
	</div>
	<div class="clear-block"></div>
		
</div>
<?}?>
