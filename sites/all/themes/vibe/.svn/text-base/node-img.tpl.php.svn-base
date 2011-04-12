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
 
 preg_match("/<a.*?>(.*?)<\/a>/",$terms,$matches);
 $category = $matches[1];
 
 preg_match("/<a.*?>(.*?)<\/a>/",$name,$matches);
 $author = $matches[1];
 
 $date = date("n-j-Y g:i a", strtotime(str_replace(" - ", " ", $date)));
 
?>

<div id="story">

  <div id="story_title"><?php if(false && $category) {?><span class="story_category"><?php print $category?>:</span> <?}?><span class="story_title_text_outer"><span class="story_title_text_inner"><?php print $title?></span></span>
  	<div id="story_byline">
  		<span id="story_byline_source"><?php print $node->field_source[0]["value"];?></span>
  		<span id="story_byline_author">By: <a href='/user/<?php print $uid?>'><?php print $node->name;?></a></span>
		<span id="story_byline_date">Posted <?php print $date;?></span>
  	</div>
  </div>
  <div id="story_image"><img src="/<?php print $node->field_img_src[0]["filepath"]?>" border="0"></div>
  <div id="story_body"><?php print $node->content["body"]["#value"]?></div>
  
</div>

<?php print theme_render_template("sites/all/themes/vibe/block-node-footer.tpl.php", array("links"=>$links, "node"=>$node));?>
