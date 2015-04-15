<?php

function clleague_player_remove_page($league, $uid)	{
	return drupal_get_form('clleague_player_remove_form');
}

function clleague_player_remove_form($form_state) {
	$nid = arg(1);
	$pid = arg(4);
	
	$league = node_load($nid);
	
	// Retrieve the player information
	$query = db_select('league_participants', 'ps')
		->fields('ps', array('nickname'))
		->condition('nid', $nid)
		->condition('pid', $pid)
		->execute();
	$player = $query->fetchAssoc();
	
	$values = array(
		'@league' => $league->title, 
		'@player' => $player['nickname'],
	);
	
	$message = t('Are you sure you want to remove player @player from the league @league?', $values);
	
	$form = array();
	$form['intro_text'] = array(
		'#markup' => '<p>' . $message . '</p>',
	);
		
	$form['buttons']['cancel'] = array(
		'#type' => 'submit', 
		'#name' => 'cancel', 	
		'#value' => t('Cancel'),
		'#submit' => array('clleague_player_remove_form_cancel'),
		'#limit_validation_errors' => array(),
	);
	
	$form['buttons']['remove'] = array(
		'#type' => 'submit', 
		'#name' => 'remove', 
		'#value' => t('Remove'),
	);

	return $form;
}

function clleague_player_remove_form_cancel(&$form, &$form_state) {
	$nid = arg(1);
	
	drupal_set_message(t('The player has not been removed.'));
	$form_state['redirect'] = 'node/' . $nid . '/players';	
}

function clleague_player_remove_form_submit(&$form, &$form_state) {
	$nid = arg(1);
	$pid = arg(4);
	
	// Remove player from league in database
	$query = db_delete('league_participants')
		->condition('nid', $nid)
		->condition('pid', $pid)
		->execute();
		
	// Inform user and return to players page
	drupal_set_message(t('The player has been removed.'));
	$form_state['redirect'] = 'node/' . $nid . '/players';	
}

