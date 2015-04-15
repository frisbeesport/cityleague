<?php

function clsignup_resign_page($league) {
	return drupal_get_form('clsignup_resign_form');
}

function clsignup_resign_form($form_state) {
	$nid = arg(1);

	$message = t('You have chosen to resign from this league. Is this really what you want to do?');
	
	$form = array();
	$form['intro_text'] = array(
		'#markup' => '<p>' . $message . '</p>',
	);
		
	$form['buttons']['cancel'] = array(
		'#type' => 'submit', 
		'#name' => 'cancel', 	
		'#value' => t('Cancel'),
		'#submit' => array('clsignup_resign_form_cancel'),
		'#limit_validation_errors' => array(),
	);
	
	$form['buttons']['submit'] = array(
		'#type' => 'submit', 
		'#name' => 'resign', 
		'#value' => t('Resign'),
	);

	return $form;
}

function clsignup_resign_form_cancel(&$form, &$form_state) {
	$nid = arg(1);
	drupal_set_message(t('You have not resigned. We like that. Keep up the good work!'));	
	$form_state['redirect'] = 'node/' . $nid;	
}

function clsignup_resign_form_submit(&$form, &$form_state) {
	global $user;
	$nid = arg(1);

	clsignup_db_resign($user->uid, $nid);
	
	drupal_set_message(t('You have resigned for this league, but we hope to see you another time!'));
	$form_state['redirect'] = 'node/' . $nid;	
}
