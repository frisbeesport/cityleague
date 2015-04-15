<?php

/** //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*                                                                                                     Database methods
*/ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
function clsignup_db_get_signed_up_league($uid) {
	$query = db_select('league_signups')
		->fields('league_signups', array('uid', 'nid'))
		->condition('uid', $uid);
		
	$result = $query->execute();
	
	if ($member = $result->fetchObject()) {
		return $member->nid;
	} else {
		return 0;
	}	
}
 
function clsignup_db_discard_signup($uid) {
	db_delete('league_signups')
		->condition('uid', $uid)
		->execute();
}

function clsignup_db_resign($uid, $nid) {
	$query = db_delete('league_participants')
		->condition('nid', $nid)
		->condition('uid', $uid)
		->execute();
}

function clsignup_db_edit_registration($pid, $fields) {
	$query = db_merge('league_participants')
		->key(array('pid' => $pid))
		->fields($fields)
		->execute();
}

function clsignup_db_complete_registration($uid, $nid, $fields)
{
	db_delete('league_signups')
		->condition('uid', $uid)
		->execute();
	
	db_delete('league_participants')
		->condition('uid', $uid)
		->condition('nid', $nid)
		->execute();
	
	$query = db_insert('league_participants')
		->fields(array('nid', 'uid', 'nickname', 'level', 'timestamp'))
		->values($fields)
		->execute();
}

function clsignup_db_signup($uid, $nid) {
	$fields = array(
		'uid' => $uid,
		'nid' => $nid,
	);
	
	$query = db_merge('league_signups')
			->key(array('uid' => $uid))
			->fields($fields)
			->execute();
}
