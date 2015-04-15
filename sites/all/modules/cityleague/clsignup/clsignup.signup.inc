<?php

/** ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 *                                                                                                       Access methods
 */ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

function clsignup_primary_signup_access() {
	global $user;
	
	// Show only if 
	// - not loggon on or 
	// - not signed up or
	// - signed up but not for the league on the current page
	
	if (arg(2)) {
		if (arg(2) == 'completesignup' || arg(2) == 'signup') {
			return false;
		}
	}
	
	if (clleague_is_logged_on()) {
		$signed_up = clsignup_db_get_signed_up_league($user->uid) != 0;
	
		if ($signed_up) {
			return false;
		}
	}
	
	return true;
}
  
function clsignup_secondary_signup_access() {
	global $user;
	
    // If logged on and not signed up / playing for the league currently on the page
	$nid = clsignup_league_on_current_page();
	$page_contains_league = $nid != 0;

	if ($page_contains_league) {
		if (arg(2)) {
			if (arg(2) == 'completesignup' || arg(2) == 'signup') {
				return false;
			}
		}
			
		$node = node_load($nid);
		
		$finalized  = clleague_is_finalized($nid);
		$player     = clleague_is_player($nid, $user->uid);
		$signedup   = clsignup_db_get_signed_up_league($user->uid) != 0;
		
		if (!$finalized && !$player && !$signedup) {
			return true;
		}
	}
	return false;
 }
 
function clsignup_complete_signup_access() {
	global $user;
	
	// If logged on, signed up, and not on password reset / completion page
	
	if (arg(2)) {
		if (arg(2) == 'completesignup' || arg(2) == 'signup') {
			return false;
		}
	}	

	
	if (clleague_is_logged_on()) {
		$sid = clsignup_db_get_signed_up_league($user->uid);
	
		$signed_up = $sid != 0;
	
		if ($signed_up) {
			return true;
		}
	}
	
	return false;
}
 
/** ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 *                                                                                                                Forms
 */ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

function clsignup_signup_form_specific($form, &$form_state) {
	global $user;
	$nid = arg(1);
	
	if (!clleague_signup_open($nid)) {
		$message = t('Unfortunately, the signup for this league is currently closed.');
		
		$form['intro_text'] = array(
			'#markup' => '<p>' . $message . '</p>',
		);
		
		return $form;		
	} else if (!clleague_has_free_spots($nid)) {
		$message = t('Unfortunately, this league does not have any spots left.');
		
		$form['intro_text'] = array(
			'#markup' => '<p>' . $message . '</p>',
		);
		
		return $form;		
	}
	
	$form = array();
	$form['form_id']['#value'] = 'clsignup_signup_form_specific';
	$form['#attached']['css'] = array(drupal_get_path('module', 'clsignup') . '/signupblock.css',);
	
	$logged_on = clleague_is_logged_on();
	if (!$logged_on) {
		honeypot_add_form_protection($form, $form_state, array('honeypot', 'time_restriction'));
	} else {
		honeypot_add_form_protection($form, $form_state, array('honeypot'));
	}
	
	$node = node_load($nid);
	$values = field_get_items('node', $node, 'field_city');
	$city = $values[0]['value'];
	
	$title = t('Join us in @city', array('@city' => $city));
	
	$form['intro_text'] = array(
		'#markup' => '<span class="script-font">' . $title . '</span>',
	);

	// add form elements
	$form['league'] = array(
		'#type' => 'select',
		'#title' => t('Choose your league'),
		'#required' => TRUE,
		'#options' => array($nid => "This League"),
		'#default_value' => $nid,
		'#prefix' => '<div class="signup_select">',  
		'#suffix' => '</div>',
		'#type' => 'hidden',
	);
	
	if (!$logged_on) {
		$form['email'] = array(
			'#type' => 'textfield',
			'#size' => 15,
			'#maxlength' => 80,
			'#required' => TRUE,
			'#prefix' => '<div class="input-group"><div class="signup_email">',  
			'#suffix' => '</div>',
			'#attributes' => array(  
				'class' => array('form-control'),
				'placeholder' => t('Your email address'),
			),
		);
		
		$form['submit'] = array(
			'#type' => 'submit', 
			'#value' => t('Sign up') . '!',
			'#attributes' => array(  
				'class' => array('btn-success',),
			),
			'#prefix' => '<span class="input-group-btn">',  
			'#suffix' => '</span></div>',
		);
	} else {
		$form['submit'] = array(
			'#type' => 'submit', 
			'#value' => t('Sign up') . '!',
			'#attributes' => array(  
				'class' => array('btn-success', 'btn-block'),
			),
		);			
	}
	if (!$logged_on) {
		$message = '* ' . t('You will receive an email with further instructions on how to complete your registration.');
		
		$form['outro_text'] = array(
			'#markup' => '<p><small>' . $message . '</small></p>',
		);
	}
	
	return $form;	
}



function clsignup_signup_form($form, &$form_state) {
	global $user;
	$nid = arg(1);
	
	$selectable_leagues = clsignup_get_selectable_leagues();
	
	if (count($selectable_leagues) == 0) {
		$message = t('Unfortunately, there are currently no Cityleagues available for signup.');
		
		$form['intro_text'] = array(
			'#markup' => '<p>' . $message . '</p>',
		);
		
		return $form;		
	}
	
	$form = array();
	$form['form_id']['#value'] = 'clsignup_signup_form';
	$form['#attached']['css'] = array(drupal_get_path('module', 'clsignup') . '/signupblock.css',);
	
	$logged_on = clleague_is_logged_on();
	if (!$logged_on) {
		honeypot_add_form_protection($form, $form_state, array('honeypot', 'time_restriction'));
	} else {
		honeypot_add_form_protection($form, $form_state, array('honeypot'));
	}
	
	//$node = clsignup_signup_form_get_current_node($form_state);
	
	// set default values
	if ($logged_on) {
		if (in_array($nid, array_keys($selectable_leagues))) {
			$default_league = $nid;
		} else {
			$league_keys = array_keys($selectable_leagues);
			$default_league = $league_keys[0];
		}
	} else {
		$league_keys = array_keys($selectable_leagues);
		$default_league = $league_keys[0];
	}
	
	$title = t('Join us in your own city');
	
	$form['intro_text'] = array(
		'#markup' => '<span class="script-font">' . $title . '</span>',
	);
	
	
	// add form elements
	$form['league'] = array(
		'#type' => 'select',
		'#required' => TRUE,
		'#options' => $selectable_leagues,
		'#default_value' => $default_league,
		'#prefix' => '<div class="signup_select">',  
		'#suffix' => '</div>',
		'#attributes' => array(  
			'class' => array('form-control'),
			'placeholder' => t('Choose your league'),
		),
	);

	if (!$logged_on) {
		$form['email'] = array(
			'#type' => 'textfield',
			'#size' => 15,
			'#maxlength' => 80,
			'#required' => TRUE,
			'#prefix' => '<div class="input-group"><div class="signup_email">',  
			'#suffix' => '</div>',
			'#attributes' => array(  
				'class' => array('form-control'),
				'placeholder' => t('Your email address'),
			),
		);
		
		$form['submit'] = array(
			'#type' => 'submit', 
			'#value' => t('Sign up') . '!',
			'#attributes' => array(  
				'class' => array('btn-success',),
			),
			'#prefix' => '<span class="input-group-btn">',  
			'#suffix' => '</span></div>',
		);
	} else {
		$form['submit'] = array(
			'#type' => 'submit', 
			'#value' => t('Sign up') . '!',
			'#attributes' => array(  
				'class' => array('btn-success', 'btn-block'),
			),
		);			
	}
	if (!$logged_on) {
		$message = '* ' . t('You will receive an email with further instructions on how to complete your registration.');
		
		$form['outro_text'] = array(
			'#markup' => '<p><small>' . $message . '</small></p>',
		);
	}
	
	return $form;	
}
function clsignup_signup_form_specific_validate(&$form, &$form_state) {
	clsignup_signup_form_validate($form, $form_state);
}

function clsignup_signup_form_specific_submit(&$form, &$form_state) {
	clsignup_signup_form_submit($form, $form_state);
}

function clsignup_signup_form_validate(&$form, &$form_state) {
	if (array_key_exists('league', $form_state['values'])) {
		$nid = $form_state['values']['league'];
	} else {
		$nid = arg(1);
	}
	
	if (array_key_exists('email', $form_state['values'])) {
		$email = $form_state['values']['email'];
		clsignup_signup_form_validate_email($email, $nid);
	}
}

function clsignup_signup_form_validate_email($email, $nid) {
	if (!valid_email_address($email)) {
    	form_set_error('email', t('Please enter a valid email address.'));
		return;
	}
	
	$existing_user = clsignup_get_existing_user($email);
	
	if (clleague_is_player($nid, $existing_user)) {
		form_set_error('email', t('There is already a player with this email address playing in this league.'));
	}
}

function clsignup_get_existing_user($email) {
	$query = db_select('users')
		->fields('users', array('uid'))
		->condition('mail', $email);
	$results = $query->execute();
	
	if ($member = $results->fetchObject()) {
		return $member->uid;
	} else {
		return 0;
	}	
}

function clsignup_signup_form_submit(&$form, &$form_state) {
	global $user, $language;
	
	if (array_key_exists('league', $form_state['values'])) {
		$nid = $form_state['values']['league'];
	} else {
		$nid = arg(1);
	}
	
	$user_exists = 0;
	$email = "";
	if (array_key_exists('email', $form_state['values'])) {
		$email = $form_state['values']['email'];
		$user_exists = clsignup_get_existing_user($email);
	
		if ($user_exists == 0) {
			$newuser = clsignup_create_user_account($form_state);
			$user_created = true;
		} else {
			$newuser = user_load($user_exists);
			$user_created = false;
		}
	} else {
		$newuser = $user;
		$user_created = false;
	}
		
	if ($newuser) 	{
		// Show welcome message
		$node = node_load($nid);
		if ($user_created) {
			// Add additional fields
			$user_wrapper = entity_metadata_wrapper('user', $newuser);
			$user_wrapper->field_subscribe = 1;
			$user_wrapper->field_last_used_level = 1;
			$user_wrapper->field_last_used_nickname = "";
			$user_wrapper->save();
		} else {
			// change language of current user
			$user_wrapper = entity_metadata_wrapper('user', $newuser);
			$user_wrapper->language = $language->language;
			$user_wrapper->save();
			
			// reload
			$newuser = user_load($user_exists);
		}
		
	
		if ($email) {
			// New users are signed up and need to complete registration via e-mail link
			clsignup_db_signup($newuser->uid, $nid);
		
			// Request user confirmation and continue registration by email
			_user_mail_notify('register_no_approval_required', $newuser);
	
			$values = array('@league' => $node->title);
			$message = t('You have signed up for @league. Instructions to complete your registration have been sent to your email address.', $values);
			drupal_set_message($message);
			
			//$form_state['redirect'] = 'node/' . $nid;
		} else {
			$form_state['redirect'] = 'node/' . $nid . '/signup';
		}
	} else {
		// Creating the user failed	
		watchdog('clleague', 'Could not create new user', array(), WATCHDOG_ERROR);
		drupal_set_message(t('Something went wrong during your sign up. You may try again (later).'));
		
		//$form_state['redirect'] = 'node/' . $nid;
	}
}

function clsignup_create_user_account(&$form_state) {
	global $language;
	
	// Create user account	
	$newname = clsignup_email_registration_unique_username($form_state['values']['email']);
	
	$edit = array(
		'name' => $newname,
	    'mail' => $form_state['values']['email'],
		'init' => $form_state['values']['email'],
		'language' => $language->language,
		'status' => 1,
	);

	$user = user_save(NULL, $edit);
	
	return $user;
}

/**
 * Given a starting point for a Drupal username (e.g. the name portion of an
 * email address) return a legal, unique Drupal username. This function is
 * designed to work on the results of the /user/register or /admin/people/create
 * forms which have already called user_validate_name, valid_email_address
 * or a similar function. If your custom code is creating users, you should
 * ensure that the email/name is already validated using something like that.
 *
 * @param $name
 *   A name from which to base the final user name.  May contain illegal characters; these will be stripped.
 *
 * @param $uid
 *   (optional) Uid to ignore when searching for unique user (e.g. if we update the username after the
 *   {users} row is inserted)
 *
 * @return
 *   A unique user name based on $name.
 *
 * @see user_validate_name().
 *
 */
function clsignup_email_registration_unique_username($mail, $uid = 0) {
	$name = preg_replace('/@.*$/', '', $mail);

	// Strip illegal characters.
	$name = preg_replace('/[^\x{80}-\x{F7} a-zA-Z0-9@_.\'-]/', '', $name);

	// Strip leading and trailing spaces.
	$name = trim($name);

	// Convert any other series of spaces to a single underscore.
	$name = preg_replace('/ +/', '_', $name);

	// If there's nothing left use a default.
	$name = ('' === $name) ? t('user') : $name;

	// Truncate to reasonable size.
	$name = (drupal_strlen($name) > (USERNAME_MAX_LENGTH - 10)) ? drupal_substr($name, 0, USERNAME_MAX_LENGTH - 11) : $name;

	// Iterate until we find a unique name.
	$i = 0;
	do {
		$new_name = empty($i) ? $name : $name . '_' . $i;
		$found = db_query_range("SELECT uid from {users} WHERE uid <> :uid AND name = :name", 0, 1, array(':uid' => $uid, ':name' => $new_name))->fetchAssoc();
		$i++;
	} while (!empty($found));

	return $new_name;
}
