<?php

$userEdit = user_load($form['#uid']);

$form['account']['status'] = null;
$form['account']['roles'] = null;
$form['timezone'] = null;
?>
<div class="profile">
<?php //profile_load_profile($user); ?>
<?php //print '<pre>'. check_plain(print_r((array)$user, 1)) .'</pre>'; ?>
	<div id="basic_profile">
		<div id="profile_photo">
		<?php 
			if($userEdit->picture) {
	    		print theme('user_picture', $userEdit);
			}
			else { 
				print '<img height="75" border="0" width="75" src="/sites/all/themes/vibe/images/MISSINGUSER_ICON_SML.jpg"/>';
			}
		?>
		</div>
		<div id="profile_name"><a href="/users/<?php print $userEdit->name; ?>"><?php print $userEdit->name; ?></a></div>
	</div> 
</div>
<div id="user_edit_form">
<?php print drupal_render($form);?>
</div>