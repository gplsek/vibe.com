<?php
// $Id: node.tpl.php,v 1.4 2008/09/15 08:11:49 johnalbin Exp $

/**
 * @file node.tpl.php
 *
 * Theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: Node body or teaser depending on $teaser flag.
 * - $picture: The authors picture of the node output from
 *   theme_user_picture().
 * - $date: Formatted creation date (use $created to reformat with
 *   format_date()).
 * - $links: Themed links like "Read more", "Add new comment", etc. output
 *   from theme_links().
 * - $name: Themed username of node author output from theme_user().
 * - $node_url: Direct url of the current node.
 * - $terms: the themed list of taxonomy term links output from theme_links().
 * - $submitted: themed submission information output from
 *   theme_node_submitted().
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $teaser: Flag for the teaser state.
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 */
 //var_dump($node);
 
?>
<div id="feature_billboard">
<div id="billboard_nav">
		<div id="billboard_left_arrow"><a href="#"><span>Left arrow</span></a></div>
		<div id="billboard_steps">
			<div id="billboard_step_1" class="billboard_step"><a href="#"><span>1</span></a></div>
			<div id="billboard_step_2" class="billboard_step"><a href="#"><span>2</span></a></div>
		    <div id="billboard_step_3" class="billboard_step"><a href="#"><span>3</span></a></div>
		    <div id="billboard_step_4" class="billboard_step"><a href="#"><span>4</span></a></div>
            <div id="billboard_step_5" class="billboard_step"><a href="#"><span>5</span></a></div>
		</div>
		<div id="billboard_right_arrow"><a href="#"><span>Right arrow</span></a></div>
	</div>
	<div id="billboard">
	
	<?php foreach ((array)$node->field__imgs as $item) { ?>
		<div class='billboard_item'>
	     <a href="<?=$item['data']['billboardurl']['body']; ?>">
	     <?php 
          	if (empty($item['data']['workflow_img_presetname'])) {
          		$presetSetting = 'billboard-large';
            } else {
            	$presetSetting = $item['data']['workflow_img_presetname'];
            }
           print theme('imagecache', $presetSetting, $item['filepath'], '', '');
           if ($item['data']['workflow_img_txt_align'] == 'top') {
           		$txtAlignClass = 'billboard_title_top';
           	} else {
           		$txtAlignClass = 'billboard_title_bottom';
           	}
         ?>
		<div class='<?=$txtAlignClass?>'>
			<div class="billboard_title_bg"></div>
			<div class="billboard_title_txt">
				<?=$item['data']['maintitle']['body']?>
			</div>
			<div class="billboard_subtitle_txt">
				<?=$item['data']['subtitle']['body']?>
			</div>
		</div>
		</a>
	</div>
    <?php } ?>
	</div>
</div>

<?php drupal_add_js('sites/all/themes/vibe/js/billboard.js', 'theme', 'footer'); ?>