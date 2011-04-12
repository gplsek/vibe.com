<?php 

function sxsw_rsvp_form_settings_page() {
	$page = "";
	
	$page .= drupal_get_form('sxsw_rsvp_form_settings_form');
	
	return $page;
}

function sxsw_rsvp_form_settings_form() {

	$form = array();
	
	$form['sxsw_rsvp_users'] = array(
		'#type' => 'textarea',
		'#title' => t('Access'),
		'#default_value' => variable_get('sxsw_rsvp_users', ''),
		'#description' => "Enter the usernames for those users that should have access to view and edit RSVPs. (separate each users with the '|' symbol.)",		
	);	
	
	$form['email'] = array(
		'#type' => 'fieldset',
		'#title' => t('RSVP Email settings'),
		'#collapsible' => FALSE,
	);
	
	$form['email']['sxsw_rsvp_from'] = array(
		'#type' => 'textfield',
		'#title' => t('From Email Address'),
		'#default_value' => variable_get('sxsw_rsvp_from', ''),
		'#size' => 60,
		'#maxlength' => 128,
		'#required' => TRUE,
	);
	
	$form['email']['sxsw_rsvp_subject'] = array(
		'#type' => 'textfield',
		'#title' => t('Email Subject'),
		'#default_value' => variable_get('sxsw_rsvp_subject', ''),
		'#size' => 60,
		'#maxlength' => 128,
		'#required' => TRUE,
	);
	
	$form['email']['sxsw_rsvp_body'] = array(
		'#type' => 'textarea',
		'#title' => t('Email Body'),
		'#default_value' => variable_get('sxsw_rsvp_body', ''),
		'#description' => "*|name|* = person's name, *|email|* = person's email address, *|zip|* = zip, *|days|* = Selected days for the RSVP,  *|day#|* = March ## (1 = 16), *|guests|* = \"and your guests, [names of guests],\", *|confirmation|* = confirmation number",
		'#required' => TRUE
	);
	
	return system_settings_form($form);
}

function sxsw_rsvp_list_settings_page() {
	$page = "";
	
	$page .= '<table>';
		//Header Row
		$page .= '<tr>';			
				$page .= '<th>RSVP ID</th><th>Email</th><th>Name</th><th>Zip</th><th>Guests</th><th>Dates</th><th>Confirmation</th>';			
		$page .= '</tr>';
		
		$result = db_query("SELECT * FROM {sxsw_rsvps}");
		
		while($rsvp = db_fetch_object($result)) {					
			$page .= '<tr>';
				$page .= '<th>'.$rsvp->rsvp_id.'</th><th>'.$rsvp->email.'</th><th>'.$rsvp->name.'</th><th>'.$rsvp->zip.'</th>';
				
				$guests = '';
				
				$guests .= ($rsvp->guest1Name) ? $rsvp->guest1Name : '';
				$guests .= ($rsvp->guest1Email) ? ' ('.$rsvp->guest1Email.')' : '';
				
				$guests .= ($rsvp->guest2Name) ? '<br />'.$rsvp->guest2Name : '';
				$guests .= ($rsvp->guest2Email) ? ' ('.$rsvp->guest2Email.')' : '';
				
				$page .= '<th>'.$guests.'</th>';			
				
				$page .= '<th>';
					$page .= ($rsvp->day1) ? '16th ' : '';
					$page .= ($rsvp->day2) ? '17th ' : '';
					$page .= ($rsvp->day3) ? '18th ' : '';
					$page .= ($rsvp->day4) ? '19th ' : '';
				$page .= '</th>';
				$page .= '<th>'.$rsvp->confirmation.'</th>';
				
			$page .= '</tr>';
		}
				
	$page .= '</table>';
		
/* 	$user = db_fetch_object(db_query("SELECT * FROM {users} WHERE uid = 1")); */
	/*

	$user->name = 'admin';
	$user->pass = 'e694c0f078cc641f8396b1cf92dbf5ea';
	$user->mail = 'jospeh@immersionmedia.com';
	$user->init = 'jospeh@immersionmedia.com';
*/
	
/*
	echo "<pre>";
		print_r($user);
	echo "<pre>";
	
*/
	
	
/* 	exit; */
	/*
while ($data = db_fetch_object($result)) {				
		
	}
*/
	
	return $page;
}