<?php

/** ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 *                                                                                                         Delete hooks 
 */ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 /**
 * Implements hook_form_alter().
 */
function clteams_form_clleague_resign_form_alter(&$form, $form_state) {
	array_unshift($form['#submit'], 'clteams_resign_form_submit');
}

function clteams_resign_form_submit(&$form, &$form_state) {
	global $user;
	$nid = arg(1);
	
	$pid = clteams_get_pid_in_league($user->uid, $nid);
	
	db_delete('league_team_members')
		->condition('pid', $pid)
		->execute();
}

 /**
 * Implements hook_form_alter().
 */
function clteams_form_clleague_player_remove_form_alter(&$form, $form_state) {
	array_unshift($form['#submit'], 'clteams_player_remove_form_submit');
}

function clteams_player_remove_form_submit(&$form, &$form_state) {
	$nid = arg(1);
	$pid = arg(4);
	
	db_delete('league_team_members')
		->condition('pid', $pid)
		->execute();
}

/**
* Implements hook_node_delete($node).
*/
function clteams_node_delete($node) {
	if ($node->type == 'league') {
		$teams = clteams_get_list_of_teams($node->nid);
		
		foreach (array_keys($teams) as $tid) {
			db_delete('league_team_members')
				->condition('tid', $tid)
				->execute();
		}
		
		db_delete('league_teams')
			->condition('nid', $node->nid)
			->execute();
	}
}
 
