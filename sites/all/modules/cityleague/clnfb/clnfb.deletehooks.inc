<?php
	
/** ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 *                                                                                                         Delete hooks 
 */ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
/**
* Implements hook_user_delete($account).
*/
function clnfb_user_delete($account) {
	clnfb_anonimize_league_participant($account->uid);
	clnfb_remove_user_information($account->uid);
}

/**
* Implements hook_user_cancel($edit, $account, $method).
*/
function clnfb_user_cancel($edit, $account, $method) {
	switch ($method) {
		case 'user_cancel_reassign':
			clnfb_anonimize_league_participant($account->uid);
			clnfb_remove_user_information($account->uid);
			break;
	}
}

/**
* Implements hook_node_delete($node).
*/
function clnfb_node_delete($node) {
	if ($node->type == 'league') {
		clnfb_remove_league_participants($node->nid);
	}
}

function clnfb_anonimize_league_participant($uid) {
	db_update('league_participants_nfbmembership')
		->condition('uid', $uid)
		->fields(array(
			'uid' => NULL,
		))->execute();
}

function clnfb_remove_user_information($uid) {
	db_delete('user_nfbmembership')
		->condition('uid', $uid)
		->execute();
}

function clnfb_remove_league_participants($nid) {
	//TODO: Fix some time later. Add nid to table or create complex query.
	//db_delete('league_participants_nfbmembership')
	//	->condition('nid', $nid)
	//	->execute();
}

