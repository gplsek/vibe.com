<?

$vlinks = $variables['vlinks'];

//var_dump($vlinks);

$main_link_image = $vlinks->vlink_image_path;
$main_link_url = url($vlinks->main_link_url);
$main_link_title = $vlinks->main_link;
$main_link_source = $vlinks->main_link_source;

//var_dump($main_link_image);
$vlink_output .='<div id="vlink_header_image"><img src="'.url('sites/all/themes/vibe/images/VIBE_VLINKS_GRAPHIC.jpg').'" /></div>';
$vlink_output .= '<div class="vibe_stew">';
$vlink_output .= '<div class="vlinks_wrapper">';
$vlink_output .= '<div class="vlink_left" style="float:left;">
					<div class="vlink_photo">
						<a target="_blank" rel="nofollow" href="'.$main_link_url.'">'.
							theme('imagecache', 'vlinks-cropping', $main_link_image, '', '').'
							
						</a>
				 	</div>
				 </div>';
$vlink_output .= '<div class="vlink_right" style="float:right;">
					
						<div class="main_link">
							<a target="_blank" rel="nofollow" href="'.$main_link_url.'">'.$main_link_title.'</a> -- '.$main_link_source.'
						</div>
					
					<div id="vlinks_links_list">
						<div class="vlinks_links_wrapper">
							<div class="vlinks_links">
								<a target="_blank" rel="nofollow" href="'.url($vlinks->link_1_url).'">'.$vlinks->link_1.'</a> -- '.$vlinks->link_1_source.'
							</div>
						
							<div class="vlinks_links">
								<a target="_blank" rel="nofollow" href="'.url($vlinks->link_2_url).'">'.$vlinks->link_2.'</a> -- '.$vlinks->link_2_source.'
							</div>
							<div class="vlinks_links">
								<a target="_blank" rel="nofollow" href="'.url($vlinks->link_3_url).'">'.$vlinks->link_3.'</a> -- '.$vlinks->link_3_source.'
							</div>
							<div class="vlinks_links" id="bottom_link">
								<a target="_blank" rel="nofollow" href="'.url($vlinks->link_4_url).'">'.$vlinks->link_4.'</a> -- '.$vlinks->link_4_source.'
							</div>							
						</div>
					</div>
				</div>';
$vlink_output .= '</div></div>';
if($vlinks){
print $vlink_output;
}