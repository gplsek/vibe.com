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
  <div id="story_title">
  	<?php if(false && $category) {?>
    <span class="story_category">
		<?php print $category?>:
    </span>
    <? } ?>
    <span class="story_title_text_outer">
    	<span class="story_title_text_inner">
			<?php print $title?>
        </span>
    </span>
  	<div id="story_byline">
  		<span id="story_byline_source">
			<?php print $node->field_source[0]["value"];?>
        </span>
  		<span id="story_byline_author">
        	By: <a href='/user/<?php print $uid?>'><?php print $node->name;?></a>
        </span>
		<span id="story_byline_date">Posted <?php print $date;?></span>
  	</div>
</div>  
<?php
  if (count($node->taxonomy)) {

    $tags = array();
     foreach ($node->taxonomy as $term) {
        $tags[$term->vid][] = l($term->name, taxonomy_term_path($term));
        }
        foreach ( $tags as $vid => $tag_set ) {
            //get the vocabulary name and name it $name
        	$vocab = taxonomy_vocabulary_load($vid);
			if($vocab->name == "Tags"){
        		print "<div id='tag_links_wrapper'><div class='tags'>$vocab->name: </div><div id='tag_links'>" . implode(', ', $tag_set) . '</div></div>';
			}
        }
    }
?>   
    
<div id="social_network">
        <div id="social_wrapper">
        
            <!--<div id="fbshare">
                <?php
                    //include("sites/all/themes/vibe/facebook_share.php");
                ?>
            </div>
            <div class="clear-block"></div>-->
            <div id="tweetshare">
                <?php
                    include("sites/all/themes/vibe/tweet_meme.php");
                ?>          
            </div>
            <script>$('.connect_widget div.connect_confirmation_cell').css("display","none");</script>
            <div id="fbrecommend">
            <iframe id="iframe_like" name="fbLikeIFrame_0" class="social-iframe" scrolling="no" frameborder="0" src="http://www.facebook.com/widgets/like.php?width=100&amp;show_faces=1&amp;layout=standard&amp;colorscheme=light&amp;href=<?php $curr_url = check_plain("http://" .$_SERVER['HTTP_HOST'] .$node_url); echo $curr_url; ?>" width="100%" height="30"></iframe>
            
            <?php
                //$block = module_invoke('facebook_recommend', 'block', 'view', 0);
                //print $block['content'];
            ?>
        	</div>
       </div>
       <div class="clear-block"></div>
</div>
 <div class="clear-block"></div> 
  <?php if(isset($node->field_hide_main_img[0]) === false || (isset($node->field_hide_main_img[0]) && $node->field_hide_main_img[0]['value'] != "1")) {?>
  <div id="story_image"><? print theme('imagecache', 'image-full-size', $node->field_img_src[0]['filepath'], '', '');?>
</div>
<?php }?>
  <div id="story_body"><?php print $node->content["body"]["#value"]?></div>
  <div id="audio_post">
  		<script type="text/javascript" src="/sites/all/themes/vibe/js/swfobject_v210.js"></script>
		<script type="text/javascript">/*<!--*/

							var flashvars = {};
							flashvars.trackURL = "/<?php print $node->field_audio[0]["filepath"]?>";
							flashvars.title = "<?php print $node->field_audio_title[0]["value"]?>";
							flashvars.width = 400;
							flashvars.height = 30;
							flashvars.autoPlay = "false";
							flashvars.autoLoad = "false";
							flashvars.downloadEnabled = "false";
							flashvars.loop = "false";
							flashvars.volume = 0.7;
							flashvars.duration = 0;
							flashvars.color1 = "0x577ba0";
							flashvars.color2 = "0xeeeeee";
							flashvars.color3 = "0x333333";
							flashvars.color4 = "0xeeeeee";
							flashvars.color5 = "0x577ba0";
							flashvars.color6 = "0xeeeeee";
							var params = {};
							params.wmode = "transparent";
		
							swfobject.embedSWF("/sites/all/themes/vibe/swf/A3_v100.swf", "A3Player", "400", "30", "9.0.0", false, flashvars, params);

						/*-->*/
						</script>

						<style type="text/css">
							object { outline: none; }
						</style>
						<div id="A3Player">
							<p></p>
						</div>

  </div>
  
</div>

<?php print theme_render_template("sites/all/themes/vibe/block-node-footer.tpl.php", array("links"=>$links, "node"=>$node));?>
