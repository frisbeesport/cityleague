<?php

/** //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*                                                                                                             City API
*/ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

function clleague_get_open_leagues() {
	$open_leagues = array();
	
	$all_leagues = node_load_multiple(array(), array('type' => 'league'));
	
	foreach($all_leagues as $league) {
		$values = field_get_items('node', $league, 'field_finalized');
		$finalized = $values[0]['value'];

		$values = field_get_items('node', $league, 'field_visible');
		$visible = $values[0]['value'];

		if (!$finalized && $visible) {
			$open_leagues[$league->nid] = $league;
		}
	}
	
	return $open_leagues;
}

function clleague_active_leagues() {
	$open_leagues = clleague_get_open_leagues();	
	$open_leagues_ids = array_keys($open_leagues);
	
	$selectable_leagues = array();
	
	foreach ($open_leagues_ids as $id) {
		$values = field_get_items('node', $open_leagues[$id], 'field_city');
		$city = $values[0]['value'];
		$city = strtolower(str_replace(' ', '', $city));

		$selectable_leagues[$city] = $id;
	}
	
	return $selectable_leagues;
}

/** //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*                                                                                                           Access API
*/ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

function clleague_access_level_coordinator($user) {
	if (isset($user->uid) && $user->uid && clleague_is_coordinator($user->uid)) {
		return true;
	} else {
		return false;
	}	
}

function clleague_access_level_coordinator_and_league($user, $node) {
	if ($node->type != 'league') {
		return false;
	}
	if (isset($user->uid) && $user->uid && clleague_is_coordinator($user->uid)) {
		return true;
	} else {
		return false;
	}	
}

function clleague_access_level_organizer($user, $node) {
	if ($node->type != 'league') {
		return false;
	}
	
	if (isset($user->uid) && $user->uid && clleague_is_coordinator($user->uid)) {
		return true;
	} else if (isset($user->uid) && $user->uid && clleague_is_organizer($node->nid, $user->uid)) {
		return !clleague_is_finalized($node->nid);
	} else {
		return false;
	}	
}

function clleague_access_level_player($user, $node) {
	if ($node->type != 'league') {
		return false;
	}
	
	if (isset($user->uid) && $user->uid && clleague_is_coordinator($user->uid)) {
		return true;
	} else if (isset($user->uid) && $user->uid && clleague_is_organizer($node->nid, $user->uid)) {
		return !clleague_is_finalized($node->nid);
	} else if (isset($user->uid) && $user->uid && clleague_is_player($node->nid, $user->uid)) {
		return !clleague_is_finalized($node->nid) && clleague_is_visible($node->nid);
	} else {
		return false;
	}		
}

function clleague_access_level_user($user, $node) {
	if ($node->type != 'league') {
		return false;
	}
	
	if (isset($user->uid) && $user->uid && clleague_is_coordinator($user->uid)) {
		return true;
	} else if (isset($user->uid) && $user->uid && clleague_is_organizer($node->nid, $user->uid)) {
		return !clleague_is_finalized($node->nid);
	} else if (isset($user->uid) && $user->uid) {
		return !clleague_is_finalized($node->nid) && clleague_is_visible($node->nid);
	} else {
		return false;
	}		
}

function clleague_access_level_guest($user, $node) {
	if ($node->type != 'league') {
		return false;
	}
	
	if (isset($user->uid) && $user->uid && clleague_is_coordinator($user->uid)) {
		return true;
	} else if (isset($user->uid) && $user->uid && clleague_is_organizer($node->nid, $user->uid)) {
		return true;
	} else {
		return clleague_is_visible($node->nid);
	} 		
}

/** ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 *                                                                                                 League state helpers 
 */ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
function clleague_is_visible($nid) {
	return clleague_get_league_value($nid, 'field_visible');
}

function clleague_is_finalized($nid) {
	return clleague_get_league_value($nid, 'field_finalized');
}

function clleague_signup_open($nid) {
	return clleague_get_league_value($nid, 'field_signup');
}

function clleague_signoff_open($nid) {
	return clleague_get_league_value($nid, 'field_signout');
}

function clleague_get_league_value($nid, $fieldname) {
	$node = node_load($nid);
	$values = field_get_items('node', $node, $fieldname);
	if ($values != FALSE) {
		$val = $values[0]['value'];		
	} else {
		$val = '';
	}
	
	return $val;
}

function clleague_has_free_spots($nid) {
	$max_participants = clleague_get_league_value($nid, 'field_max_participants');
	
	if ($max_participants == 0 || $max_participants == NULL) {
		return true;
	}
	
	$query = db_select('league_participants')
	   ->fields('league_participants', array('pid'))
	   ->condition('nid', $nid);
	$results = $query->execute();
	
	$current_participants = $results->rowCount();
	return $current_participants < $max_participants;
}

/** ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 *                                                                                                   User state helpers 
 */ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 
function clleague_is_coordinator($uid) {
	$usr = user_load($uid);
	$result = user_access('coordinate leagues', $usr);

	return $result;
}

function clleague_is_organizer($nid, $uid) {
	$node = node_load($nid);
	$values = field_get_items('node', $node, 'field_organizers');
	
	if ($values != FALSE) {
		foreach ($values as $val) {
			if ($uid == $val['target_id']) {
				return true;
			}
		}
	} 
	
	return false;
}

function clleague_is_player($nid, $uid) {
	$query = db_select('league_participants')
	   ->fields('league_participants', array('uid'))
	   ->condition('nid', $nid)
	   ->condition('uid', $uid);
	$results = $query->execute();
	
	return $results->rowCount() > 0;
}

function clleague_is_player_pid($nid, $pid) {
	$query = db_select('league_participants')
	   ->fields('league_participants', array('pid'))
	   ->condition('pid', $pid)
	   ->condition('nid', $nid);
	$results = $query->execute();
	
	return $results->rowCount() > 0;
}

function clleague_retrieve_pid_from_nid_and_uid($nid, $uid) {
	$query = db_select('league_participants')
	   ->fields('league_participants', array('pid'))
	   ->condition('nid', $nid)
	   ->condition('uid', $uid);
	$results = $query->execute();
	$participant = $results->fetchObject();
	
	if ($participant) {
		return $participant->pid;
	} else {
		return 0;
	}
}
		
function clleague_is_logged_on() {
	global $user;

	if ($user->uid) {
		return true;
	} else {
		return false;
	}
}

/** ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 *                                                                                                               Others 
 */ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

function clleague_level_to_string($level) {
	// TODO: Add textual level?
	return $level;
}
