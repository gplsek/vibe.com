<?php
global $user;

?>

<div id="post_to_vibe">
<div id="post_to_vibe_header">
    Are you good at finding the VIBE?
    </div>
<div id="post_to_vibe_body">
Sign up to post links, images and videos. The bes  posts will show up in the 
<strong>VIBE RAW FEED- REAL TIME!</strong>
</div>
<div id="post_to_vibe_button">
	<?if($user->uid) {?>
    <a class="thickbox" href="#TB_inline?height=430&amp;width=540&amp;inlineId=startPostingModal&amp;modal=true">START POSTING NOW</a>
	<?} else {?>
	<a class="thickbox" href="#TB_inline?height=430&amp;width=540&amp;inlineId=userloginModal&amp;modal=true">START POSTING NOW</a>
	<?}?>
</div>
</div>

<div id="startPostingModal" style="display: none;">
<?php
if ( $user->uid ) {
  // Logged in user
?>

<div class="large_title" style="font-size: 24px; color: #555555; margin-bottom: 3px;">Post a Link on VIBE</div>
<div><?php print drupal_get_form('user_submission_form');?></div>

<div class="form_buttons" style="text-align: center; margin: 0 auto;">
	<div class="vibe_button" style="float: left; margin-left: 195px;">
   	 	<a href="javascript:void(0)" onclick="tb_remove();">Close</a>	
	</div>
	<div class="vibe_button" style="float: left; margin-left: 5px;">
    	<a href="javascript:void(0)" onclick="$('#user-submission-form').submit()">Save</a>
	</div>
</div>

<?php
}
else {
	// Not logged in
/*
?>

<div class="large_title" style="font-size: 24px; color: #555555; margin-bottom: 3px;">Login or Register</div>
<div><?php print drupal_get_form('user_login_block')?></div>

<div class="form_buttons" style="text-align: center; margin: 0 auto; margin-top: -180px;">
	<div class="vibe_button" style="float: left; margin-left: 170px; background-color: #28aae1;">
   	 	<a href="javascript:void(0)" onclick="tb_remove();">Close</a>	
	</div>
	<div class="vibe_button" style="float: left; margin-left: 5px; background-color: #28aae1;">
    	<a href="javascript:void(0)" onclick="$('#user-login-form').submit()">Login</a>
	</div>
</div>

<?php
*/}
?>


	
</div>
