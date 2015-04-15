<?php

/** //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*                                                                                                         Delete hooks 
*/ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Implements hook_user_delete($account).
 */
function clleague_user_delete($account) {
	clleague_anonimize_league_participant($account->uid);
}

/**
 * Implements hook_user_cancel($edit, $account, $method).
 */
function clleague_user_cancel($edit, $account, $method) {
	switch ($method) {
		case 'user_cancel_reassign':
			clleague_anonimize_league_participant($account->uid);
			break;
	}
}

/**
 * Implements hook_node_delete($node).
 */
function clleague_node_delete($node) {
	if ($node->type == 'league') {
		clleague_remove_league($node->nid);
	}
}

function clleague_anonimize_league_participant($uid) {
	db_update('league_participants')
		->condition('uid', ($uid))
		->fields(array(
			'uid' => NULL,
		))->execute();
}

function clleague_remove_league($nid) {
	db_delete('league_participants')
		->condition('nid', $nid)
		->execute();
}