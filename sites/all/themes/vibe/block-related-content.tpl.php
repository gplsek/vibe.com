

<div id="similar_content">
<div id="similar_content_header"><img src="<? print url('sites/all/themes/vibe/images/VIBE_RELATED_GRAPHIC.jpg'); ?>" /></div>
<?php
			global $node;
		//sets memcache object	
			//node_init();
			//global $memcache;

			//$key = "related_content_".$node->nid;
			//$related_content = $memcache->get($key);
			
			//$related_keys = $memcache->get('related_keys');			
			
			if(!$related_keys){
				$related_keys = array();
			}
			else{
				$related_keys = unserialize($related_keys);
			}

			
			if(!in_array($key, $related_keys))
			{
				array_push($related_keys,$key);
			}
						
			
			//$memcache->set('related_keys',serialize($related_keys));
			
			//if(!$related_content){
				//$message = "Not Cached";
				//var_dump($message);
				$block = module_invoke('similar', 'block', 'view', 0);
				//$memcache->set($key,$block['content'],0,rand(21600,43200));
				print $block['content'];
			//}
			//else{
				//$message = "Cached";
				//var_dump($message);
			//	print $related_content;
			//}
?>
</div>
