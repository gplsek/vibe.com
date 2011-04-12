<?php

	if(arg(0) == "node") {
		$nid = arg(1);
		$prev_node = get_prev_node($nid);
		$next_node = get_next_node($nid);

		$prev_link = $prev_node->path;
		$next_link = $next_node->path;
	}
?>
<div id="previous_next_content">
	<div id="previous_content"><a href='/<?=$prev_link?>'>&lt;&lt; PREVIOUS</a></div>
	<div id="next_content"><a href='/<?=$next_link?>'>NEXT &gt;&gt;</a></div>
	<div class="clear-block"></div>
</div>