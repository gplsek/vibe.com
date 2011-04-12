<?php 

 preg_match("/<a.*?>(.*?)<\/a>/",$terms,$matches);
 $category = $matches[1];
 
 preg_match("/<a.*?>(.*?)<\/a>/",$name,$matches);
 $author = $matches[1];
 
 $date = date("n-j-Y g:i a", strtotime(str_replace(" - ", " ", $date)));

?>

<?php //var_dump(array_keys($node->nodes)); ?>
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
  <div id="story_image"><a href="<?php print $node_url; ?>/0"><img src="/<?php print $node->field_img_src[0]["filepath"]?>" border="0"></a></div>
  	<?php }?>
    <?php $nodeCount = count($node->nodes);
	if($nodeCount > 0) { ?>
  	<div id="story_body"><?php print $node->content["body"]["#value"]?></div>
<div class="photo_count"><a href="<?php print $node_url; ?>/0">Enter Gallery <?php echo count($node->nodes); ?> Photos <span>&gt;&gt;</span></a></div> 
    <!--<div id="gallery_photos">
			<?php			
				
				/*for($i = 0; $i < $nodeCount; $i++) {
					$imgLink = $node_url . "/" . $i;
					$imgSrc = theme('imagecache', 'image-gallery-thumb', $node->nodes[$i]->field_img_src[0]['filepath'], '', '');
					echo "<div class='image_node'><a href='$imgLink'>$imgSrc</a></div>";
				}*/
			?>
	</div> 
	<div class="photo_count"><a href="<?php //print $node_url; ?>/0"><?php //echo count($node->nodes); ?> Photos <span>&gt;&gt;</span></a></div>--> 
    <? }?>
</div>
<div id="block_footer">
<?php print theme_render_template("sites/all/themes/vibe/block-node-footer.tpl.php", array("links"=>$links, "node"=>$node));?>
</div>