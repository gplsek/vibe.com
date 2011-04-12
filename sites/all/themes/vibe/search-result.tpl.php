<?php
// $Id: search-result.tpl.php,v 1.1.2.1 2008/08/28 08:21:44 dries Exp $

/**
 * @file search-result.tpl.php
 * Default theme implementation for displaying a single search result.
 *
 * This template renders a single search result and is collected into
 * search-results.tpl.php. This and the parent template are
 * dependent to one another sharing the markup for definition lists.
 *
 * Available variables:
 * - $url: URL of the result.
 * - $title: Title of the result.
 * - $snippet: A small preview of the result. Does not apply to user searches.
 * - $info: String of all the meta information ready for print. Does not apply
 *   to user searches.
 * - $info_split: Contains same data as $info, split into a keyed array.
 * - $type: The type of search, e.g., "node" or "user".
 *
 * Default keys within $info_split:
 * - $info_split['type']: Node type.
 * - $info_split['user']: Author of the node linked to users profile. Depends
 *   on permission.
 * - $info_split['date']: Last update of the node. Short formatted.
 * - $info_split['comment']: Number of comments output as "% comments", %
 *   being the count. Depends on comment.module.
 * - $info_split['upload']: Number of attachments output as "% attachments", %
 *   being the count. Depends on upload.module.
 *
 * Since $info_split is keyed, a direct print of the item is possible.
 * This array does not apply to user searches so it is recommended to check
 * for their existance before printing. The default keys of 'type', 'user' and
 * 'date' always exist for node searches. Modules may provide other data.
 *
 *   <?php if (isset($info_split['comment'])) : ?>
 *     <span class="info-comment">
 *       <?php print $info_split['comment']; ?>
 *     </span>
 *   <?php endif; ?>
 *
 * To check for all available data within $info_split, use the code below.
 *
 *   <?php print '<pre>'. check_plain(print_r($info_split, 1)) .'</pre>'; ?>
 *
 * @see template_preprocess_search_result()
 */

if($search_node['field_img_src']['field']['items'][0]['#node']->field_img_src[0]['filepath']){
	$thumbnail = $search_node['field_img_src']['field']['items'][0]['#node']->field_img_src[0]['filepath'];
}
else if($search_node['field_story_thumb']['field']['items'][0]['#node']->field_story_thumb[0]['filepath']){
	$thumbnail = $search_node['field_story_thumb']['field']['items'][0]['#node']->field_story_thumb[0]['filepath'];
}
else if($search_node['field_vid_src']['field']['items'][0]['#node']->field_vid_src[0]['filepath']){
	$thumbnail = $search_node['field_vid_src']['field']['items'][0]['#node']->field_vid_src[0]['filepath'];
} 


?>
<div class="latest_on_vibe_post">
	<div class="latest_on_vibe_thumbnail">
        <div class="latest_on_vibe_thumbnail_img">
        	<a href="<?php print $url?>"><? print theme('imagecache', 'square-cropping', $thumbnail);?></a>
        </div>
	</div>
	<div class="latest_on_vibe_excerpt">
		<div class="latest_on_vibe_title">
			<span class="latest_on_vibe_title_inner">
				<span class="content_title_search"><a href="<?php print $url?>"><?php print $title;?></a></span>
			</span>
		</div>
		<div class="latest_on_vibe_teaser">
			<?php print $snippet;?>
			<span style='margin-left: 4px'><b>&gt;&gt; <a href="<?php print $url?>" style="color: #000000; text-decoration:underline;"><?php print get_more_text($info_split['type']);?></a></b></span>
		</div>
		<div class="latest_on_vibe_info">
			<?php $comment_count = $info_split[0];?>
			<?php $created_date = $info_split['date'];?>
			<?php 
				
				$now = time();
				$posttime = strtotime(str_replace(" - ", " ",$created_date));
				$date = date("Y-m-d H:i:s", $posttime);
				$timeago = time_ago_string($now, $posttime);
			?>
			
				
			<a href="<?php print $url?>" style="text-decoration: none; color:#000000;"><?php print $timeago . " / " . $comment_count?></a>
		</div>
	</div>
	<div class="clear-block"></div>
		
</div>
