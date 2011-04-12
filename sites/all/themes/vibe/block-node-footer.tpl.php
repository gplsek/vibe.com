<?php //print theme_render_template("sites/all/themes/vibe/block-content-reaction.tpl.php", array("links"=>$links));?>

<?php //print theme_render_template("sites/all/themes/vibe/block-share-content.tpl.php", array());?>

<?php $block = module_invoke('jp_vlinks', 'block', 'view', 0); print $block['content'];?>

<?php print theme_render_template("sites/all/themes/vibe/block-related-content.tpl.php", array());?>

<div class="clear-block"></div>

<?php //$related_content = related_block_block('view',10); print $related_content["content"];?>

<div class="clear-block" style="margin-top: 10px; margin-bottom: 10px;"></div>
 
<?php print comment_render($node,0);?>
