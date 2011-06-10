<?php 
function vibe_fusion_menu_item_link($link) {
	if (empty($link['localized_options'])) {
		$link['localized_options'] = array();
	}
	
	if (empty($link['localized_options']['attributes']['class'])) {
		$link['localized_options']['attributes']['class'] = empty($link['title']) ? '' : 'menu-'.vibe_fusion_menu_slug($link['title']).((empty($link['mlid'])) ? '' : ' menu-'.$link['mlid']);
	} else {
		$link['localized_options']['attributes']['class'] .= empty($link['title']) ? '' : ' menu-'.vibe_fusion_menu_slug($link['title']).((empty($link['mlid'])) ? '' : ' menu-'.$link['mlid']);
	}
	
	$link['title'] = $link['title'] = '<span class="link">' . check_plain($link['title']) . '</span>';

	$link['localized_options'] += array('html'=> TRUE);
	
	return l($link['title'], $link['href'], $link['localized_options']);
}

function vibe_fusion_menu_slug($str) {
	$str = strtolower(trim($str));
	$str = preg_replace('/[^a-z0-9-]/', '-', $str);
	$str = preg_replace('/-+/', "-", $str);
	return $str;
}