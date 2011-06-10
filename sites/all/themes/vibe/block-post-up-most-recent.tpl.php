<?php 
	$feed_items = get_most_recent_feed_items(); 
?>

<div style="margin-top: -20px;"></div>
<?foreach($feed_items as $feed_item){?>
<?
	preg_match("/(http:\/\/)(.*?)\.(.*?)\.(.*?)\//",$feed_item->guid,$matches);
	
	$sitename = $matches[2] . "." . $matches[3] . "." . $matches[4];
	
	$siteimage = "/sites/all/themes/vibe/images/" . $matches[3] . "_logo.png";
	if(!file_exists($_SERVER['DOCUMENT_ROOT'] . "/" . $siteimage)) {
		$siteimage = "/sites/all/themes/vibe/images/MISSINGPHOTO_ICON_SML.jpg";
	}
?>

<div class="post-up-most-recent">
	<div class="hot_on_vibe_thumbnail">
		<a href="/<?php print $feed_item->dst?>"><img width='75' src='<?=$siteimage?>' border='0'></a>
	</div>
	<div class="hot_on_vibe_excerpt">
		<div class="hot_on_vibe_title">
			<?php $title = $feed_item->title;?>
			<span class="hot_on_vibe_title_inner">
				<span class="hot_on_vibe_content_title"><a href="/<?php print $feed_item->dst?>"><?php print $title;?></a></span>
			</span>
		</div>
		<div class="hot_on_vibe_info">
			<?php $comment_count = $fields["comment_count"]?>
			<?php $created_date = $fields["created"]?>
			<?php 
				
				$now = time();
				$posttime = $feed_item->changed;
				$date = date("Y-m-d H:i:s", $posttime);
				$timeago = time_ago_string($now, $posttime);
			?>
			
			<div class="hot_on_vibe_info_text">
				<div class="hot_on_vibe_info_date_comments">
					<?php print $timeago . " / " . $feed_item->comment_count . " Comments"?>
				</div>
				<div class="hot_on_vibe_info_from">
					
					FROM: <a target="_blank" href='<?php print $feed_item->url?>'><?php print $sitename?></a>
				</div>
			</div>
		</div>
	</div>
	<div class="clear-block"></div>
		
</div>
<?}?>
