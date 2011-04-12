<?php 
 if (!isset($node->nodes[$image_index])) {
  // redirect to gallery landing page
 }
 
 $imgCount = count($node->nodes);
 $imgRowCount = floor($imgCount/2);
 if ($imgRowCount%2 !== 0) {
 	$imgRowCount++;
 }
 // determine width by how many items are going to be in each row
 $carouselWidth = $imgRowCount *67;
 $startPage = 0;
 $adjustedIndex = $image_index;
 if ( $adjustedIndex > 4 || ($adjustedIndex-$imgRowCount) > 4) {
 	if ( $adjustedIndex > $imgRowCount) {
 		$startPage = floor(($adjustedIndex-$imgRowCount)/4);
 	} else {
 		$startPage = floor($adjustedIndex/4);
 	}
 }
//echo $startPage;
 $thmbStr = "<div id='gallery_carousel_inner' style='width: " . $carouselWidth . "px; left: -" . (($startPage)*260) . "px'>";
 for($i = 0; $i < $imgCount; $i++) {
 	$imgTag = theme('imagecache', 'square-cropping', $node->nodes[$i]->field_img_src[0]['filepath'], '', '');
 	$thmbStr .= "<div class='carousel_item'><a href='" . $node_url . "/$i#tp'>$imgTag</a></div>";
 }
 $thmbStr .= "</div>";

?>
 <div id="photo_title"><a href="<?php print $node_url; ?>">Photos :<span><?php print $node->title ?></span></a></div>
<div id="photo_gallery_nav">
	<div id="scroll_left"  class="nav_control"><a href="#"><span>&lt;&lt;</span></a></div>
		<div id ="gallery_carousel">
  			<? echo $thmbStr; ?>
		</div>
	<div id="scroll_right" class="nav_control"><a href="#"><span>&gt;&gt;</span></a></div>
	</div>
	<br clear="both" />
<script>
function carouselScroll(dir, containerId) {
	if (dir == "left") {
		if ($('#'+containerId).position().left < 0) {
			$('#'+containerId).animate({"left": "+=260px"}, "slow");
		}
	} else if (dir == "right") {
		if ($('#'+containerId).position().left > ($('#'+containerId).width() - 291)*-1) {
			$('#'+containerId).animate({"left": "-=260px"}, "slow");
		}
	}
}
$("#scroll_left").click(function () {
	carouselScroll("left", 'gallery_carousel_inner');
	return false;
});

$("#scroll_right").click(function () {
	carouselScroll("right", 'gallery_carousel_inner');
	return false;
});
</script>