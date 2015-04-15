<?php

/** //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*                                                                            Alter forms related to leaving the league
*/ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
/**
 * Implements hook_form_alter().
 */
function clnfb_form_clsignup_resign_form_alter(&$form, $form_state) {
	array_unshift($form['#submit'], 'clnfb_resign_form_submit');
}

/**
 * Implements hook_form_alter().
 */
function clnfb_form_clleague_player_remove_form_alter(&$form, $form_state) {
	array_unshift($form['#submit'], 'clnfb_player_remove_form_submit');
}

function clnfb_resign_form_submit(&$form, &$form_state) {
	global $user;
	
	$nid = arg(1);
	$uid = $user->uid;
	
	$pid = clleague_retrieve_pid_from_nid_and_uid($nid, $uid);

	$query = db_delete('league_participants_nfbmembership')
		->condition('pid', $pid)
		->execute();
}

function clnfb_player_remove_form_submit(&$form, &$form_state) {
	$nid = arg(1);
	$uid = arg(4);
	
	$pid = clleague_retrieve_pid_from_nid_and_uid($nid, $uid);
	
	$query = db_delete('league_participants_nfbmembership')
		->condition('pid', $pid)
		->execute();
}

