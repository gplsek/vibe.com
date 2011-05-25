<?php
//$aggregator_category = aggregator_category_load(1);
//var_dump($aggregator_category);
//$feed = node_feed();
//var_dump($feed);

$feed_nodes = get_feed_nodes(array("DJ Booth Feed","All Hip Hop Feed","Datpiff Feed","All Music Feed","Baller Status Feed","Ring TV Feed","Singers Room Feed","PopEater Feed","BlackBox Voices Feed","Hello Beautiful Feed","Huffington Post Feed", "Carlton Jordan"),2); 
//var_dump($feed_nodes);
?>
<div id="vibe-lifestyle-network-footer">
	<div id="vibe-lifestyle-network-header">
		<div id="vibe-lifestyle-newtork-title">Friends of <span>VIBE</span></div>
	</div>	
	<div id="vibe-lifestyle-network-sites">		
		<div class="vibe-lifestyle-network-site">
			<div class="vibe-lifestyle-network-site-content_top">
	        	<div class="vibe-lifestyle-network-site-image_top"><a href='http://www.allhiphop.com/' target='_blank'><img src='/sites/all/themes/vibe/images/VIBE_FOOTER/FOOTER_ALLHIPHOPLOGO.jpg' border='0'></a></div>
			<?php foreach($feed_nodes['All Hip Hop Feed'] as $feed_item) { ?>
				<div class="vibe-lifestyle-network-item"><a target='_blank' href="<?php print $feed_item->url?>"><?php print $feed_item->title;?><span class="article_arrows">&nbsp;>></span></a></div>
			<?php } ?>
			</div>
		</div>
		<div class="vibe-lifestyle-network-site">
			<div class="vibe-lifestyle-network-site-content_top">
	        	<div class="vibe-lifestyle-network-site-image_top"><a href='http://www.datpiff.com/' target='_blank'><img src='/sites/all/themes/vibe/images/VIBE_FOOTER/FOOTER_DATPIFFLOGO.jpg' border='0'></a></div>
			<?php foreach($feed_nodes['Datpiff Feed'] as $feed_item) { ?>
				<div class="vibe-lifestyle-network-item"><a target='_blank' href="<?php print $feed_item->url?>"><?php print $feed_item->title;?><span class="article_arrows">&nbsp;>></span></a></div>
			<?php } ?>
			</div>
		</div>
		<div class="vibe-lifestyle-network-site">
			<div class="vibe-lifestyle-network-site-content_top">
	        	<div class="vibe-lifestyle-network-site-image_top"><a href='http://www.ballerstatus.com/' target='_blank'><img src='/sites/all/themes/vibe/images/VIBE_FOOTER/FOOTER_BALLERLOGO.jpg' border='0'></a></div>
			<?php foreach($feed_nodes['Baller Status Feed'] as $feed_item) { ?>
				<div class="vibe-lifestyle-network-item"><a target='_blank' href="<?php print $feed_item->url?>"><?php print $feed_item->title;?><span class="article_arrows">&nbsp;>></span></a></div>
			<?php } ?>
			</div>
		</div>		
		<div class="vibe-lifestyle-network-site">
			<div class="vibe-lifestyle-network-site-content_top">
		    	<div class="vibe-lifestyle-network-site-image_top"><a href='http://www.singersroom.com/' target='_blank'><img src='/sites/all/themes/vibe/images/VIBE_FOOTER/FOOTER_SINGERSLOGO.jpg' border='0'></a></div>
			<?php foreach($feed_nodes['Singers Room Feed'] as $feed_item) { ?>
				<div class="vibe-lifestyle-network-item"><a target='_blank' href="<?php print $feed_item->url?>"><?php print $feed_item->title;?><span class="article_arrows">&nbsp;>></span></a></div>
			<?php } ?>
			</div>
		</div>
		<div class="vibe-lifestyle-network-site">
			<div class="vibe-lifestyle-network-site-content_top">
	        	<div class="vibe-lifestyle-network-site-image_top"><a href='http://www.popeater.com/' target='_blank'><img src='/sites/all/themes/vibe/images/VIBE_FOOTER/FOOTER_POPEATERLOGO_SMALL.jpg' border='0'></a></div>
			<?php foreach($feed_nodes['PopEater Feed'] as $feed_item) { ?>
				<div class="vibe-lifestyle-network-item"><a target='_blank' href="<?php print $feed_item->url?>"><?php print $feed_item->title;?><span class="article_arrows">&nbsp;>></span></a></div>
			<?php } ?>
			</div>
		</div>
		<div class="vibe-lifestyle-network-site">
			<div class="vibe-lifestyle-network-site-content_top">
	        	<div class="vibe-lifestyle-network-site-image_top"><a href='http://www.huffingtonpost.com/' target='_blank'><img src='/sites/all/themes/vibe/images/VIBE_FOOTER/huffpo_ent.jpg' border='0'></a></div>
			<?php foreach($feed_nodes['Huffington Post Feed'] as $feed_item) { ?>
				<div class="vibe-lifestyle-network-item"><a target='_blank' href="<?php print $feed_item->url?>"><?php print $feed_item->title;?><span class="article_arrows">&nbsp;>></span></a></div>
			<?php } ?>
			</div>
		</div>
		<div class="vibe-lifestyle-network-site">
			<div class="vibe-lifestyle-network-site-content_top">
	        	<div class="vibe-lifestyle-network-site-image_top"><a href='http://www.hellobeautiful.com/' target='_blank'><img src='/sites/all/themes/vibe/images/VIBE_FOOTER/FOOTER_HELLOLOGO.jpg' border='0'></a></div>
			<?php foreach($feed_nodes['Hello Beautiful Feed'] as $feed_item) { ?>
				<div class="vibe-lifestyle-network-item"><a target='_blank' href="<?php print $feed_item->url?>"><?php print $feed_item->title;?><span class="article_arrows">&nbsp;>></span></span></a></div>
			<?php } ?>
			</div>
		</div>
	 	<div class="vibe-lifestyle-network-site">
			<div class="vibe-lifestyle-network-site-content_top">
	        	<div class="vibe-lifestyle-network-site-image_top"><a href='http://www.blackvoices.com/' target='_blank'><img src='/sites/all/themes/vibe/images/VIBE_FOOTER/FOOTER_BLKVOICESLOGO.jpg' border='0'></a></div>
			<?php foreach($feed_nodes['BlackBox Voices Feed'] as $feed_item) { ?>
				<div class="vibe-lifestyle-network-item"><a target='_blank' href="<?php print $feed_item->url?>"><?php print $feed_item->title;?><span class="article_arrows">&nbsp;>></span></a></div>
			<?php } ?>
			</div>
		</div>
		<div class="vibe-lifestyle-network-site">
			<div class="vibe-lifestyle-network-site-content_top">
	        	<div class="vibe-lifestyle-network-site-image_top"><a href='http://www.carltonjordan.com/' target='_blank'><img src='/sites/all/themes/vibe/images/VIBE_FOOTER/FOOTER_CARLTONJORDANLOGO.jpg' border='0'></a></div>
			<?php foreach($feed_nodes['Carlton Jordan'] as $feed_item) { ?>
				<div class="vibe-lifestyle-network-item"><a target='_blank' href="<?php print $feed_item->url?>"><?php print $feed_item->title;?><span class="article_arrows">&nbsp;>></span></a></div>
			<?php } ?>
			</div>
		</div>		
    </div><!-- /#vibe-lifestyle-network-sties -->
</div>