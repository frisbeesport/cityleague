<?php

function clleague_delete_page($league)	{
	return drupal_get_form('clleague_delete_form', $league);
}

function clleague_delete_form($form_state, $node) {
	
	$message = t('Are you sure you want to delete this league? This will completely delete the league and all information related to it. There is no thinkable reason to delete a league instead of finalizing it. Are you really really sure?');
	
	$form = array();
	$form['intro_text'] = array(
		'#markup' => '<p>' . $message . '</p>',
	);
		
	$form['buttons']['cancel'] = array(
		'#type' => 'submit', 
		'#name' => 'cancel', 	
		'#value' => t('Cancel'),
		'#submit' => array('clleague_delete_form_cancel'),
		'#limit_validation_errors' => array(),
	);
	
	$form['buttons']['delete'] = array(
		'#type' => 'submit', 
		'#name' => 'delete', 
		'#value' => t('Delete league'),
	);

	return $form;
}

function clleague_delete_form_cancel(&$form, &$form_state) {
	$nid = arg(1);
	
	// Inform user and return to league page
	drupal_set_message(t('The league has not been deleted. Phew!'));
	$form_state['redirect'] = 'node/' . $nid;	
}


function clleague_delete_form_submit(&$form, &$form_state) {
	$nid = arg(1);

	node_delete($nid);
	
	// Inform user and return to main page
	drupal_set_message(t('The league has been deleted.'));
	$form_state['redirect'] = '';	
}

