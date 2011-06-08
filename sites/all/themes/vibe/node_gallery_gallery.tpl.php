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
    TEST TEST
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
  <?php 
  
  	$html = str_get_html($node->content["body"]["#value"]);
	$ret = $html->find('object');
	$ids = array();
	$n=0;
	foreach($ret as $r){
		 $pattern = preg_match('/brightcove/',strtolower($r));
  		 if($pattern == 1){
			$xml = new SimpleXMLElement($r);
			foreach($xml->children() as $child){
				$attributes = $child->attributes();
				if(strtolower($attributes['name']) == 'flashvars'){
					$value = $attributes['value'];
					$split_vars = split('&',$value);
					foreach($split_vars as $sv){
						$vid = split("=",$sv);
						if(strtolower($vid[0]) == '@videoplayer'){
							array_push($ids,$vid[1]);
							}
					}
					
				}
			}	
			$query = db_query("SELECT guid FROM rg_videos WHERE vid ='%s'",$ids[$n]);
			$result = db_fetch_object($query);	
			/*$script = '<div id="story_video"><script type="text/javascript">
							function rg_framework() {
								if (typeof RealGravity != "undefined") {
									var r_'.$n.' = new RealGravity();
									r_'.$n.'.setMode("playlist");
									r_'.$n.'.addItem("'.$result->guid.'");
									r_'.$n.'.setSize({width: 486, height: 412, playlist: 0});
									r_'.$n.'.setPlayerSkin("snel");
									r_'.$n.'.setPlayerPlayList("none");
									r_'.$n.'.start(); //element id/ref, or empty to use document.write
								}
								else {
									setTimeout(rg_framework, 100);
								}
							}
							rg_framework()
						</script></div>';
			*/
			$script = "<div id='story_video'><object width='520' height='430' type='application/x-shockwave-flash' data='http://www.realgravity.com/flash/player.swf' classid='clsid:c0e00780-d58f-012d-2afb-12313b079c51' id='c0e00780-d58f-012d-2afb-12313b079c51' name='rg_player_c0e00780-d58f-012d-2afb-12313b079c51' style='visibility: visible;'> <param name='allowscriptaccess' value='always'></param> <param name='allowNetworking' value='all'></param> <param name='menu' value='false'></param> <param name='wmode' value='transparent'></param> <param name='allowFullScreen' value='true'></param> <param name='flashvars' value='config=http://www.realgravity.com/cached/api/staticplayerxml/c0e00780-d58f-012d-2afb-12313b079c51/".$result->guid."'></param> <embed id='c0e00780-d58f-012d-2afb-12313b079c51' name='rg_player_c0e00780-d58f-012d-2afb-12313b079c51' src='http://www.realgravity.com/flash/player.swf' allowNetworking='all' width='520' height='430' allowscriptaccess='always' allowfullscreen='true' flashvars='config=http://www.realgravity.com/cached/api/staticplayerxml/c0e00780-d58f-012d-2afb-12313b079c51/".$result->guid."'></embed> </object></div>";			
			$objects = $html->find('p object');
	
			$objects[$n]->outertext = $script;
			$n++;
		 }
	}
	
	
		
	$node->content["body"]["#value"] = $html->save(); 
	
	$data = $node->content["body"]["#value"];
	
	if(count($ret) > 0){	
		db_query("INSERT IGNORE INTO rg_videos_nodes (nid,data) VALUES ('%d','%s')",$node->nid,$data);
	}
  
  ?>
  
  
  <?php if(isset($node->field_hide_main_img[0]) === false || (isset($node->field_hide_main_img[0]) && $node->field_hide_main_img[0]['value'] != "1")) {?>
  <div id="story_image"><img src="/<?php print $node->field_story_thumb[0]["filepath"]?>" border="0"></div>
  <?php } ?>
  <div id="story_body"><?php print preg_replace("/&lt;!--.*endif.*?-->/","",preg_replace("/&lt;!--/","<!--",$node->content["body"]["#value"]))?></div>
	<div id="story_pages"><?php print $node->paging; ?></div> 


<?php 
$photo_node_array = array();
for($f=0; $f<count($node->field_nref); $f++){
	if($node->field_nref[$f]['nid'] != NULL){
		//var_dump($node->field_nref[$f]);
		$photo_node_array[] = $node->field_nref[$f]['nid'];
	}
}
if(count($photo_node_array) > 0){
for($g=0; $g<count($photo_node_array);$g++){	
	$photo_node = node_load($photo_node_array[$g]);
	$nodeCount = count($photo_node->nodes);
	$node_url = url($photo_node->path);
	$photoNode_title = $photo_node->title;
	$thumbnail_path = $photo_node->field_img_src[0]['filepath'];
	
 ?>
<div id="featured_photo_gallery">
	<div id="fg_wrapper">
        <div id="fg_header">Featured Gallery</div>
        <div id="fg_title"><a href="<?php print $node_url; ?>"><? print $photoNode_title; ?></a></div>
        <div id="fg_photo"><a href="<?php print $node_url; ?>" class="Hover"><img src="<?php print url($thumbnail_path); ?>" /></a></div>
        <div id="fg_teaser_count_wrapper">
            <div id="fg_teaser"><? print $photo_node->teaser; ?></div>
            <div id="fg_photo_gallery">
            	<div id="fg_photo_count"><?php echo count($photo_node->nodes); ?> Photos </div>
                <div id="fg_photo_arrows"><a href="<?php print $node_url; ?>" ><div id="photo_arrows"></div></a></div>
            </div>
        </div>
        
    </div>
</div>
<div class="clear-block"></div>
<? }} ?>
<?php print theme_render_template("sites/all/themes/vibe/block-node-footer.tpl.php", array("links"=>$links, "node"=>$node));?>

