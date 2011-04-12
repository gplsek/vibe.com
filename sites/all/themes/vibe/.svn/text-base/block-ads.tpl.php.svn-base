<?php

$actual_size = $size;
if($size == "300x250") {
    $actual_size = "300x250,300x600";
}

$terms = null;
$category_name = null;
$section = null;
$cmts = "off";

//echo "<script type='text/javascript'>alert('" . arg(0) . " " . $_SERVER['REQUEST_URI'] . "');</script>";

if (arg(0) == 'node' && is_numeric(arg(1)) && is_null(arg(2))) {
      $taxonomies = array();
    $category_vocab_id = 0;
    
    foreach(taxonomy_get_vocabularies() as $vocab) {
         if($vocab->name == "Category") {
                $taxonomies["Category"] = $vocab;
                $category_vocab_id = $vocab->vid;
                break;
        }
    }
    
    if($category_vocab_id) {
        $node = node_load(arg(1));
        $terms = taxonomy_node_get_terms_by_vocabulary($node, $category_vocab_id);
    } 
  
    if($terms) {
          $term = array_pop($terms);
          $category_name = strtolower($term->name);
    }

    if(in_array($category_name,array("news","music","culture","photos","blogs","vixen","movies"))) {
        $section = $category_name;
    } else if($category_name == "post-up") {
        $section = "postup";
    } else if($category_name == "vibe-tv") {
        $section = "vibetv";
    }
    
    preg_match("/^\/(.*?)\//",$_SERVER['REQUEST_URI'],$matches);
    
    
    if($matches[1] == "photo-galleries") {
        $section = "photos";
    }
    
    $cmts = "on";
    
} else if(arg(0) == 'user' || arg(0) == 'users') {
    $section = "profiles";
} else if(arg(0) == 'search') {
    $section = "search";
} else if(in_array(arg(0),array("news","music","culture","photos","blogs","vixen","movies"))) {
    $section = arg(0);
} else if(arg(0) == "vibe-tv") {
    $section = "vibetv";
} else if(arg(0) == "post-up") {
    $section = "postup";
} else if(!arg(0) || arg(0) == "homepage") {
    $section = "homepage";
} else if(preg_match("/^\/(.*?)\//",$_SERVER['REQUEST_URI'],$matches) && $matches[1] == "photo-galleries") {
        $section = "photos";
}

$dcopt = "";
if($tile == 1) {
    $dcopt = "dcopt=ist;";
}

if(isset($_GET['testAd'])) {
    $section = "test";
}
?>


<?

$url = curPageURL();

$urls = explode("/",$url);

$sub_section = $urls[3];

if(!$sub_section){
    $sub_section = $section;
}

?>

<div id="ad<?=$size?>_<?=$tile?>" style="text-align: center">

<script type="text/javascript">
                  if(window.location.pathname == '/home.html' || typeof(hide_<?=$size?>_<?=$tile?>) == "undefined" || hide_<?=$size?>_<?=$tile?> != true) {
                      if (typeof sAdOrd=='undefined') {sAdOrd=Math.floor(Math.random()*10000000000000000);}
                      
                    document.write('<scr' + 'ipt src="http://ad.doubleclick.net/adj/vibe.blackrock/<?=$section?>;sect=<?=$section?>;subs=<?=$section?>;subss=<?=$sub_section?>;comments=<?=$cmts?>;page=<?=md5($_SERVER['REQUEST_URI'])?>;tile=<?=$tile?>;sz=<?=$actual_size?>;<?=$dcopt?>ord=' + sAdOrd + '?" type="text/javascript" language="javascript"></scr' + 'ipt>');
                    
                }
</script>
</div>