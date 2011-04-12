
<?php if (!preg_match("/([\<])([^\>]{1,})*([\>])/i", $comment->comment)) { ?>

<div class="comment_block">
	<div class="comment_user_photo">
		<?if($comment->picture) {?>
			<img src='/<?=$comment->picture?>' width="75" border='0'>
		<?} else {?>
			<img src='/sites/all/themes/vibe/images/MISSINGUSER_ICON_LRG.jpg' width="75" border='0'>
		<?}?>
	</div>
	<div class="comment_text">
		<div>
			<span class="comment_user"><?php print $comment->name?></span> says:  <?php print $comment->comment;?>
		</div>
		<div class="comment_date">
			Posted <?php print date("m-d-Y h:i a",$comment->timestamp);?>
		</div>
	</div>
	<div class="clear-block"></div>
	<div class="comment_footer">
		<div class="comment_reply"><span style="margin-right: 10px; margin-left: -5px; color: #000000">|</span><a href="/comment/reply/<?php print $node->nid?>/<?php print $comment->cid?>">reply</a></div>
		<div class="comment_report"><?php print flag_create_link('report_comment', $comment->cid);?></div>
        
        <?php if(in_array('Vibe Editor',$user->roles) || in_array('administrative user',$user->roles) || in_array('Celebrity Editor',$user->roles))
	{ ?>
        <div class ="delete_comment"><a href="/delete_comment.php?comment=<?php print $comment->cid?>&url=<?php print curPageURL(); ?>&nid=<?php print $node->nid;?>" onclick="return confirm('Are you sure you want to delete?');">delete </a><span style="margin-left: 5px; margin-right: 5px; color: #000000">|</span>
         <a href="/ban_ip.php?comment=<?php print $comment->cid?>&url=<?php print curPageURL(); ?>&nid=<?php print $node->nid;?>" onclick="return confirm('Are you sure you want to ban <?php print get_comment_ip($comment->cid);?>?');">Ban IP</a></div>
        <?php } ?>
		<div class="clear-block"></div>
	</div>
	<div class="clear-block"></div>
</div>

<?php } ?>