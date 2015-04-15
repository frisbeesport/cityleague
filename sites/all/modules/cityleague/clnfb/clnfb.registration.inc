<?php

/** //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*                                                            Alter forms related to registration (completion and edit) 
*/ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
// TODO check if works if nfb info  is not available for some reason (user or particiapnt)

/**
 * Implements hook_form_alter().
 */
function clnfb_form_clsignup_initial_registration_form_alter(&$form, &$form_state) {
	clnfb_form_clsignup_registration_form_alter_common($form, $form_state, 'initial');
}

/**
 * Implements hook_form_alter().
 */
function clnfb_form_clsignup_league_registration_form_alter(&$form, &$form_state) {
	clnfb_form_clsignup_registration_form_alter_common($form, $form_state, 'league');
}

/**
 * Implements hook_form_alter().
 */
function clnfb_form_clsignup_edit_registration_form_alter(&$form, $form_state) {
	clnfb_form_clsignup_registration_form_alter_common($form, $form_state, 'edit');
}

function clnfb_form_clsignup_registration_form_alter_common(&$form, &$form_state, $context) {
	global $user;
	
	$nid = arg(1);
	$uid = $user->uid;
	
	$default_values = clnfb_retrieve_default_values($nid, $uid, $form_state);

	clnfb_add_to_form_nfbmember($form, $default_values);
	
	$form['#validate'][] = 'clnfb_signup_form_validate';
	$form['#submit'][] = 'clnfb_signup_form_submit';
}

function clnfb_add_to_form_nfbmember(&$form, $default_values) {
	$form['nfb'] = array(
		'#type' => 'fieldset',
		'#title' => t('Personal information'),		
		'#weight' => 4,
	);
	
	$form['nfb']['intro_text'] = array(
		// TODO: remove link from string
		'#markup' => '<p>' . t('For your insurance, we need some further information. These will stay between us and will not be shared with third parties.') . '</p>',
	);
		
	$form['nfb']['nfbmember'] = array(
		'#type' => 'radios',
		'#title' => t('NFB membership'),
		'#options' => array(
			t('I only play Cityleague'),
			t('I am a club member'),
		),
		'#default_value' => $default_values['nfbmember'],
	);
		
	$form['nfb']['callname'] = array(
		'#type' => 'textfield',
		'#title' => t('First name'),
		'#default_value' => $default_values['callname'],
		'#required' => TRUE,
		'#maxlength' => 40,
	);
		
	$form['nfb']['surname'] = array(
		'#type' => 'textfield',
		'#title' => t('Last name'),
		'#default_value' => $default_values['surname'],
		'#required' => TRUE,
		'#maxlength' => 40,
	);

	$form['nfb']['sex'] = array(
		'#type' => 'select',
		'#title' => t('Sex'),
		'#options' => array(
			'M' => t('Male'),
			'F' => t('Female'),
		),
		'#default_value' => $default_values['sex'],
		'#required' => TRUE,
	);
	
	$form['nfb']['nationality'] = array(
		'#type' => 'textfield',
		'#title' => t('Nationality'),
		'#default_value' => 'NL',
		'#default_value' => $default_values['nationality'],
		'#required' => TRUE,
		'#maxlength' => 20,
		);
	
	$form['nfb']['address'] = array(
		'#type' => 'textfield',
		'#title' => t('Address'),
		'#default_value' => $default_values['address'],
		'#required' => TRUE,
		'#maxlength' => 40,
	);
	
	$form['nfb']['postal'] = array(
		'#type' => 'textfield',
		'#title' => t('Postal'),
		'#default_value' => $default_values['postal'],
		'#required' => TRUE,
		'#maxlength' => 10,
	);
	
	$form['nfb']['city'] = array(
		'#type' => 'textfield',
		'#title' => t('City'),
		'#default_value' => $default_values['city'],
		'#required' => TRUE,
		'#maxlength' => 40,
	);
	
	if (isset($default_values['birthday'])) {
		$form['nfb']['birthday'] = array(
			'#type' => 'date',
			'#title' => t('Date of birth'),
			'#default_value' => $default_values['birthday'],
			'#required' => TRUE,
		);
	} else {
		$form['nfb']['birthday'] = array(
			'#type' => 'date',
			'#title' => t('Date of birth'),
			'#required' => TRUE,
		);
	}
	
	$form['nfb']['phone'] = array(
		'#type' => 'textfield',
		'#title' => t('Phone number'),
		'#default_value' => $default_values['phone'],
		'#required' => TRUE,
		'#maxlength' => 20,
	);
		
	return $form;
}

function clnfb_signup_form_validate(&$form, &$form_state) {
	// Check if birthday is within range
	$fields = clnfb_retrieve_values_from_form($form_state);
	
	if (isset($fields['birthday'])) {
		$date = $fields['birthday'];
		$birthday = mktime(0, 0, 0, $date['month'], $date['day'], $date['year']);
		
		if ($birthday < -2147483647 || $birthday > 2147483647) {
			form_set_error('nfb][birthday', t('Please enter a realistic date of birth.'));
		}
	}
}

function clnfb_signup_form_submit(&$form, &$form_state) {
	global $user;

	$nid = arg(1);
	$uid = $user->uid;
	
	$fields = clnfb_retrieve_values_from_form($form_state);
	
	// Retrieve and add email address for storing
	$mail_user = user_load($uid);
	$fields['email'] = $mail_user->mail;
	
	clnfb_set_nfb_membership_info_league($uid, $nid, $fields);
	clnfb_set_nfb_membership_info_user($uid, $fields);
}

/** ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 *                                                                                                       Default values
 */ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
function clnfb_retrieve_default_values($nid, $uid, &$form_state) {
	$default_values = clnfb_retrieve_default_values_empty();
	$form_values = clnfb_retrieve_values_from_form($form_state);
	
	$league_values = clnfb_retrieve_nfb_membership_info_league($nid, $uid);
	$user_values = clnfb_retrieve_nfb_membership_info_user($uid);

	foreach ($default_values as $key => $value) {
		if (isset($form_values[$key])) {
			$default_values[$key] = $form_values[$key];
		} elseif (isset($league_values[$key])) {
			$default_values[$key] = $league_values[$key];
		} elseif (isset($user_values[$key])) {
			$default_values[$key] = $user_values[$key];
		}
	}
	
	return $default_values;
}

function clnfb_retrieve_values_from_form(&$form_state) {
	$default_values = clnfb_retrieve_default_values_empty();
		
	$form_values = array();
	foreach (array_keys($default_values) as $key) {
		if (isset($form_state['values']['nfb'][$key])) {
			$value = $form_state['values']['nfb'][$key];
			$form_values[$key] = $value;
		}
	}
	
	return $form_values;
}

function clnfb_retrieve_default_values_empty() {
	return array(
		'nfbmember'     => '',      
		'surname'       => '',        
		'callname'      => '',       
		'address'       => '',        
		'sex'           => '',           
		'postal'        => '',        
		'city'          => '',          
		'phone'         => '',          
		'nationality'   => 'NL',   
		'birthday'      => '',     
			
		'repr_fullname' => '',
	);
}



