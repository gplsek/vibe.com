<?php

if($node->type == 'book' || $node->type == 'page'){
 $url_alias = drupal_get_path_alias(arg(0)."/".$node->book['bid']); 
 $fb_url = "http://www.vibe.com/".$url_alias;
?>

<div style="float:right;padding:4px;margin-top:2px;">
  <a id="fb_share" href="javascript:void(0)" onclick="fbs_click()"><img src="<?php print url('sites/all/themes/vibe/images/facebookshare.png');?>" alt="fb_share_image" /></a>
</div> 
<script>
                   function fbs_click(){
                         u="<?php print $fb_url;?>";
                         t=document.title;
                         window.open('http://www.facebook.com/sharer.php?u='+encodeURIComponent(u)+'&t='+encodeURIComponent(t),'sharer','toolbar=0,status=0,width=626,height=436');
                         return false;
                   }
</script>
<?php
}
else{
	?>   
<div style="float:right;padding:4px;">
<a expr:share_url='data:post.url' name='fb_share' rel='nofollow' type='button'></a>
<script type="text/javascript" src="http://static.ak.fbcdn.net/connect.php/js/FB.Share"></script>
</div> 

<? } ?>