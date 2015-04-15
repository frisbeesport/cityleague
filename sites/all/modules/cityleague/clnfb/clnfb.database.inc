<?php

/** ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 *                                                                                                                 CRUD 
 */ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 
function clnfb_retrieve_nfb_membership_info_league($uid, $nid) { 
	$pid = clleague_retrieve_pid_from_nid_and_uid($nid, $uid);
	
	$query = db_select('league_participants_nfbmembership')
		->fields('league_participants_nfbmembership')
		->condition('pid', $pid);
	$results = $query->execute();
	$participant = $results->fetchAssoc();
	
	if (isset($participant['birthday'])) {
		$date = getdate($participant['birthday']);
		$participant['birthday'] = array(
			'day' => $date['mday'],
			'month' => $date['mon'],
			'year' => $date['year']
		);		
	}
	
	return $participant;
}
	
function clnfb_retrieve_nfb_membership_info_user($uid) { 
	$query = db_select('user_nfbmembership')
		->fields('user_nfbmembership')
		->condition('uid', $uid);
	$results = $query->execute();
	$participant = $results->fetchAssoc();
	
	if (isset($participant['birthday'])) {
		$date = getdate($participant['birthday']);
		$participant['birthday'] = array(
			'day' => $date['mday'],
			'month' => $date['mon'],
			'year' => $date['year']
		);		
	}

	return $participant;
}

function clnfb_set_nfb_membership_info_league($uid, $nid, $fields) {
	$pid = clleague_retrieve_pid_from_nid_and_uid($nid, $uid);

	clnfb_cleanup_values($fields);
	$fields['uid'] = $uid;
	$fields['pid'] = $pid;
	
	db_merge('league_participants_nfbmembership')
		->key(array('pid' => $pid))
		->fields($fields)
		->execute();		
}

function clnfb_set_nfb_membership_info_user($uid, $fields) {
	clnfb_cleanup_values($fields);
	$fields['uid'] = $uid;
	
	db_merge('user_nfbmembership')
		->key(array('uid' => $uid))
		->fields($fields)
		->execute();
}

function clnfb_cleanup_values(&$fields) {
	if (isset($fields['birthday'])) {
		$date = $fields['birthday'];
		$fields['birthday'] = mktime(0, 0, 0, $date['month'], $date['day'], $date['year']);
	}
}
