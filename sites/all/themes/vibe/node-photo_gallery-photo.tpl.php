<?php 
 if (!isset($node->nodes[$image_index])) {
  // redirect to gallery landing page
 }
 
 $imgCount = count($node->nodes);
 
 if ($image_index > 0) { 
 	$prevUrl = $node_url . "/" . ($image_index-1) . "#tp";
 } else {
 	$prevUrl = $node_url . "/" . ($imgCount-1) . "#tp";
 }
 
 if ($image_index < ($imgCount-1)) {
 	$nextUrl = $node_url . "/" . ($image_index+1) . "#tp"; 
 } else {
 	$nextUrl = $node_url . "/0#tp";
 }
 
?>
<div id="story">
	<a href="#" id="tp" name="tp"></a>
  	<div id="photo_wrapper">
  	<span id="photo_body">
  		<div id="photo_paging">
  			<div id="prev_image"  class="nav_control"><a href="<?php print $prevUrl?>">&lt;&lt;PREVIOUS</a></div>
			<div id="next_image" class="nav_control"><a href="<?php print $nextUrl?>">NEXT&gt;&gt;</a></div>
		</div>
  		<a href="<?php print $nextUrl ?>"><?php echo theme('imagecache', 'image-full-size', $node->nodes[$image_index]->field_img_src[0]['filepath'], '', ''); ?></a>
  	</span>
  	</div>
  	<!-- <div id="photo_bottom">
  		<div class="full-size">
  			<a href="#/<?php //echo $node->nodes[$image_index]->field_img_src[0]['filepath'];?>" class="thickbox">View Full-Size&gt;&gt;</a>
		</div>
	</div> -->
	
	<div id="image_title"><?php echo $node->nodes[$image_index]->title; ?></div>
	<div id="image_desc"><?php print $node->nodes[$image_index]->body; ?></div>
	<?php if(!empty($node->nodes[$image_index]->field_img_credit[0]['value'])) { ?>
		<div id="image_source">PHOTO CREDIT:<span> <?php print $node->nodes[$image_index]->field_img_credit[0]['value']; ?></span></div>
	<?php } ?>
</div>

<?php 
print theme_render_template("sites/all/themes/vibe/block-node-footer.tpl.php", array("links"=>$links, "node"=>$node->nodes[$image_index]));
?>