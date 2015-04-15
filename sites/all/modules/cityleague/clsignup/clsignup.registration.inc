<?php

function clsignup_initial_registration_page($node) {
	global $user;
	return drupal_get_form('clsignup_initial_registration_form', $node);
}

function clsignup_edit_registration_page($node) {
	return drupal_get_form('clsignup_edit_registration_form', $node);
}

function clsignup_league_registration_page($node) {
	return drupal_get_form('clsignup_league_registration_form', $node);
}

function clsignup_initial_registration_form($form, &$form_state, $node) {
	$title = t('Complete your signup');
	drupal_set_title($title);
	
	// actual form
	$form = array();
	$form['#attached']['css'] = array(drupal_get_path('module', 'clsignup') . '/signupform.css',);
	
	$form['intro_text'] = array(
		'#markup' => '<p>' . t('Good to see you want to play in @league. Please fill in the form below to complete your registration.', array('@league' => $node->title)) . '</p>',
	);
	
	// Form elements
	clsignup_registration_form_account($form);
		
	// Password confirm field.
	$form['password'] = array(
		'#weight' => 40,		
		'#type' => 'fieldset',
	    '#title' => t('Your password'),
	);
	
	$form['password']['intro'] = array(
		'#markup' => '<p>' . t('Please choose a password and enter it twice. This enables you to log on using your email address and password. Make sure it is at least 6 characters long.') . '</p>',
	);
	
	$form['password']['pass'] = array(
		'#type' => 'password',
		'#size' => 25,
		'#title' => t('Password'),
		'#required' => TRUE,	
	);
  
	$form['password']['pass_confirm'] = array(
		'#type' => 'password',
		'#size' => 25,
		'#title' => t('Confirm password'),
		'#required' => TRUE,
	);
	
	// Buttons
	$form['buttons'] = array(
		'#weight' => 41,		
	);
	
	$form['buttons']['cancel'] = array(
		'#type' => 'submit', 
		'#name' => 'cancel', 	
		'#value' => t('Discard signup'),
		'#submit' => array('clsignup_initial_registration_form_cancel'),
		'#limit_validation_errors' => array(),
		'#weight' => 2,
	);
	
	$form['buttons']['submit'] = array(
		'#type' => 'submit', 
		'#name' => 'apply', 
		'#value' => t('Complete signup'),
		'#weight' => 1,
	);
	
	return $form;
}

function clsignup_league_registration_form($form, &$form_state, $node) {
	$title = t('Signup for @league', array('@league' => $node->title));
	drupal_set_title($title);
	
	$form = array();
	$form['#attached']['css'] = array(drupal_get_path('module', 'clsignup') . '/signupform.css',);
	
	$form['intro_text'] = array(
		'#markup' => '<p>' . t('Good to see you want to play. Please fill in the form below to complete your registration.') . '</p>',
	);
	
	// Form elements
	clsignup_registration_form_account($form);
	
	// Buttons
	$form['buttons'] = array(
		'#weight' => 41,		
	);
	
	$form['buttons']['cancel'] = array(
		'#type' => 'submit', 
		'#name' => 'cancel', 	
		'#value' => t('Cancel'),
		'#submit' => array('clsignup_league_registration_form_cancel'),
		'#limit_validation_errors' => array(),
		'#weight' => 2,
	);
	
	$form['buttons']['submit'] = array(
		'#type' => 'submit', 
		'#name' => 'apply', 
		'#value' => t('Complete signup'),
		'#weight' => 1,
		'#attributes' => array(  
			'class' => array('btn-success'),
		),
	);
	
	return $form;
}

function clsignup_edit_registration_form($form, &$form_state, $node) {
	$title = t('Edit signup for @league', array('@league' => $node->title));
	drupal_set_title($title);
	
	$form = array();
	$form['#attached']['css'] = array(drupal_get_path('module', 'clsignup') . '/signupform.css',);
		
	// Form elements
	clsignup_registration_form_account($form, true);
	
	$href = url('node/' . $node->nid . '/editsignup/resign');
	
	$form['resign'] = array(
		'#markup' => '<a href="' . $href . '" class="btn btn-danger pull-left">' . t('Resign') . '</a>', 
		'#weight' => 40,
	);
	
	
	// Buttons
	$form['buttons'] = array(
		'#weight' => 41,		
	);
	
	$form['buttons']['submit'] = array(
		'#type' => 'submit', 
		'#name' => 'apply', 
		'#value' => t('Apply'),
		'#attributes' => array(  
			'class' => array('btn-success'),
		),
	);
	
	$form['buttons']['cancel'] = array(
		'#type' => 'submit', 
		'#name' => 'cancel', 	
		'#value' => t('Cancel'),
		'#submit' => array('clsignup_edit_registration_form_cancel'),
		'#limit_validation_errors' => array(),
	);
	
	return $form;
}

function clsignup_initial_registration_form_cancel(&$form, &$form_state) {
	$nid = arg(1);
	global $user;
	
	clsignup_db_discard_signup($user->uid);
	drupal_set_message(t('Your sign up has been discarded'));
	
	$form_state['redirect'] = 'node/' . $nid;	
}

function clsignup_initial_registration_form_validate(&$form, &$form_state) {
	$pass = $form_state['values']['password']['pass'];
	$pass_confirm = $form_state['values']['password']['pass_confirm'];
	
	if ($pass != $pass_confirm) {
		form_set_error('password][pass', t('Please enter the same password in both fields.'));
	}
	if (strlen($pass) < 6) {
		form_set_error('password][pass', t('Please choose a password that is at least 6 characters long.'));
	}
}

function clsignup_initial_registration_form_submit(&$form, &$form_state) {
	global $user;
	
	$nid = arg(1);
	$node = node_load($nid);

	// Save the account with the new password.
	$curuser = user_load($user->uid);
	$edit = array('pass' => $form_state['values']['password']['pass']);
	user_save($curuser, $edit);
	
	// finalize the user login such that the one-time login link is no longer valid
	user_login_finalize();
	
	clsignup_commit_registration($form_state);
	
	$message = t('You have successfully signed up for @league. See you soon at the fields!');
	drupal_set_message(format_string($message, array('@league' => $node->title)));
	
	$form_state['redirect'] = 'node/' . $nid;	
}


function clsignup_league_registration_form_cancel(&$form, &$form_state) {
	$nid = arg(1);
	
	drupal_set_message(t('Your sign up has been cancelled'));
	
	$form_state['redirect'] = 'node/' . $nid;	
}

function clsignup_league_registration_form_submit(&$form, &$form_state) {
	$nid = arg(1);
	$node = node_load($nid);
	
	clsignup_commit_registration($form_state);
	
	$message = t('You have successfully signed up for @league. See you soon at the fields!');
	drupal_set_message(format_string($message, array('@league' => $node->title)));
	
	$form_state['redirect'] = 'node/' . $nid;
}

function clsignup_edit_registration_form_cancel(&$form, &$form_state) {
	$nid = arg(1);
	$form_state['redirect'] = 'node/' . $nid;
}

function clsignup_edit_registration_form_submit(&$form, &$form_state) {
	$nid = arg(1);
	clsignup_update_registration($form_state);
	
	drupal_set_message(t('Your changes have been saved.'));
	
	$form_state['redirect'] = 'node/' . $nid;
}

function clsignup_update_registration(&$form_state) {
	$nid = arg(1);
	global $user;
	
	$values = array(
		'nid' => $nid, 
		'uid' => $user->uid, 
		'nickname' => $form_state['values']['signup']['nickname'], 
		'level' => $form_state['values']['signup']['level'], 
	);

	$pid = clleague_retrieve_pid_from_nid_and_uid($nid, $user->uid);
	
	if ($pid) {	
		clsignup_db_edit_registration($pid, $values);
		
		// Add subscription field and field for setting default values
		$user_wrapper = entity_metadata_wrapper('user', $user);		
		$user_wrapper->field_subscribe = $form_state['values']['signup']['subscribe'];
		$user_wrapper->field_last_used_level = $form_state['values']['signup']['level'];
		$user_wrapper->field_last_used_nickname = $form_state['values']['signup']['nickname'];
		$user_wrapper->save();
	}
}

function clsignup_commit_registration(&$form_state) {
	$nid = arg(1);
	global $user;
	
	$values = array(
		'nid' => $nid, 
		'uid' => $user->uid, 
		'nickname' => $form_state['values']['signup']['nickname'], 
		'level' => $form_state['values']['signup']['level'], 
		'timestamp' => time(),
	);
	
	clsignup_db_complete_registration($user->uid, $nid, $values);
	
	// Add subscription field and field for setting default values
	$user_wrapper = entity_metadata_wrapper('user', $user);		
	$user_wrapper->field_subscribe = $form_state['values']['signup']['subscribe'];
	$user_wrapper->field_last_used_level = $form_state['values']['signup']['level'];
	$user_wrapper->field_last_used_nickname = $form_state['values']['signup']['nickname'];
	$user_wrapper->save();
}

function clsignup_registration_form_account(&$form, $edit = false) {
	global $user;
	
	$user_wrapper = entity_metadata_wrapper('user', user_load($user->uid));
	$default_subscribe = $user_wrapper->field_subscribe->value();
	$default_level = $user_wrapper->field_last_used_level->value();
	$default_nickname = $user_wrapper->field_last_used_nickname->value();
		
	$form['#tree'] = true;
	
	$form['signup'] = array(
	    '#type' => 'fieldset',
	    '#title' => t('Your account'),
		'#weight' => 1,
	);
	
	$form['signup']['nickname'] = array(
		'#type' => 'textfield',
		'#title' => t('Player name'),
		'#description' => t('Your name used within the league.'),
		'#size' => 40,
		'#maxlength' => 40,
		'#required' => TRUE,
		'#default_value' => $default_nickname
	);
	
	$form['signup']['level'] = array(
		'#type' => 'select',
		'#title' => t('Level'),
		'#required' => TRUE,
		'#options' => array(
			'1' => t('1 - New'),
			'2' => t('2 - Novice'),
			'3' => t('3 - Average'),
			'4' => t('4 - Good'),
			'5' => t('5 - Really good'),
		),
		'#default_value' => $default_level
	);
	
	$form['signup']['subscribe'] = array(
		'#type' => 'checkbox',
		'#description' => t('Receive Cityleague related notifications by email.'),
		'#default_value' => $default_subscribe,
		'#weight' => 3,
	);
	
	if (!$edit) {
		$form['signup']['agree'] = array(
			'#type' => 'checkbox',
			'#description' => t('I agree to the <a href="../11">conditions</a> for Cityleague players.'), // TODO: remove link from message
			'#default_value' => TRUE,
			'#required' => TRUE,
		);
	}
	
	return $form;
}
