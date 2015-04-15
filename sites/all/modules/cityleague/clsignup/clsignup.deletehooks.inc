<?php

/** //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*                                                                                                         Delete hooks 
*/ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Implements hook_user_delete($account).
 */
function clsignup_user_delete($account) {
	clsignup_remove_signup_user($account->uid);
}

/**
 * Implements hook_user_cancel($edit, $account, $method).
 */
function clsignup_user_cancel($edit, $account, $method) {
	switch ($method) {
		case 'user_cancel_reassign':
			clsignup_remove_signup_user($account->uid);
			break;
	}
}

/**
 * Implements hook_node_delete($node).
 */
function clsignup_node_delete($node) {
	if ($node->type == 'league') {
		clsignup_remove_signup_league($node->nid);
	}
}

function clsignup_remove_signup_league($nid) {
	db_delete('league_signups')
		->condition('nid', $nid)
		->execute();
}

function clsignup_remove_signup_user($nid) {
	db_delete('league_signups')
		->condition('uid', $uid)
		->execute();
}