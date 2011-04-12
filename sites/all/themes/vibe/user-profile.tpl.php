<?php //var_dump($account); ?>
<div class="profile">
<?php profile_load_profile($user); ?>
<?php //print '<pre>'. check_plain(print_r((array)$user, 1)) .'</pre>'; ?>
	<div id="basic_profile">
		<div id="profile_photo">
		<?php
			if($account->picture) {
				print theme('user_picture', $account);
			}
			else {
				print '<img height="75" border="0" width="75" src="/sites/all/themes/vibe/images/MISSINGUSER_ICON_SML.jpg"/>';
			}
		?>
		</div>
		<div id="profile_name"><?php print $account->name; ?></div>
	</div>
</div>
<div id="profile_tabs">
	<div id="profile_tabs_label">Show:</div><div class="profile_tab"><a href="/user/<?php print $account->uid?>/edit">Account Information</a></div>
</div>