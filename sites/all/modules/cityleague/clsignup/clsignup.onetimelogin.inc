<?php

module_load_include('inc', 'clsignup', 'clsignup.registration');
module_load_include('inc', 'user', 'user.pages');

function clsignup_onetimelogin_page($uid, $timestamp, $hashed_pass) {
	$sid  = clsignup_db_get_signed_up_league($uid);
	$signed_up = $sid != 0;

	if ($signed_up) {
		if (clsignup_complete_signup_form_access($uid, $timestamp, $hashed_pass)) {
			//return drupal_get_form('clsignup_initial_registration_form', $sid, $uid, $timestamp, $hashed_pass);
			drupal_goto('node/' . $sid .  '/completesignup');
		}
	}
	else {
		$action = arg(5);
		return drupal_get_form('user_pass_reset', $uid, $timestamp, $hashed_pass, $action);
	}	
}

function clsignup_complete_signup_form_access($uid, $timestamp, $hashed_pass) {
	global $user;
	
	// Check if there is not a different user already logged in
	if ($user->uid) {
		if ($uid != $user->uid) {
			// Request to log out
			$reset_link_account = user_load($uid);
			if (!empty($reset_link_account)) {
				drupal_set_message(t('Another user (%other_user) is already logged into the site on this computer, but you tried to use a registrion completion link for user %resetting_user. Please <a href="!logout">logout</a> and try using the link again.', array('%other_user' => $user->mail, '%resetting_user' => $reset_link_account->mail, '!logout' => url('user/logout'))));
			} else {
				drupal_set_message(t('The registration completion link you clicked is invalid.'));
			}	
			drupal_goto();
		} else {
			return true;
		}
	}
	
	// Time out, in seconds, until login URL expires. Defaults to 24 hours = 86400 seconds.
    $timeout = variable_get('user_password_reset_timeout', 86400);
    $current = REQUEST_TIME;
	
    // Some redundant checks for extra security ?
    $users = user_load_multiple(array($uid), array('status' => '1'));
    if ($timestamp <= $current && $account = reset($users)) {
		if ($account->uid && $timestamp >= $account->login && $timestamp <= $current && $hashed_pass == user_pass_rehash($account->pass, $timestamp, $account->login, $uid)) {
        
			// Set the new user.
			$user = $account;
		 
			// user_login_finalize() also updates the login timestamp of the
			// user, which invalidates further use of the one-time login link.
			user_login_finalize();
			
			watchdog('user', 'User %name used one-time login link at time %timestamp.', array('%name' => $account->name, '%timestamp' => $timestamp));

			return true;
		} else {
			drupal_set_message(t('You have tried to use a registration completion link that has either been used already or is no longer valid. You can request a new one using the form below.'));
			drupal_goto('user/password');
		}
    } else {
		// Deny access, no more clues.
		// Everything will be in the watchdog's URL for the administrator to check.
		drupal_access_denied();
    }	
	
	return false;
}