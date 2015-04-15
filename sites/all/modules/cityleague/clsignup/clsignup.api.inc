<?php

/** ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 *                                                                                                       API for league
 */ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

function clsignup_league_on_current_page() {
	if (arg(1) == NULL) return 0;
	
	$nid = arg(1);
	$node = node_load(arg(1));
	
	if ($node && $node->type == 'league')  {
		return $nid;
	} else {
		return 0;
	}
}
/*
function clsignup_is_signed_up_league($uid, $nid) {
	if (clleague_is_logged_on()) {
		$sid = clsignup_db_get_signed_up_league($uid);
		return $sid == $nid;
	}
	return false;
}*/
  
function clsignup_league_available_for_signup($league) {
	if (!clleague_access_level_guest(null, $league)) return false;
	if (clleague_is_finalized($league->nid)) return false;
	
	$values = field_get_items('node', $league, 'field_signup');
	$signup_open = $values[0]['value'];
	if (!$signup_open) return false;
	
	$has_free_spots = clleague_has_free_spots($league->nid);
	if (!$has_free_spots) return false;
	
	//$values = field_get_items('node', $league, 'field_duration');
	//$end_date = new DateObject($values[0]['value2']);	// format 2013-02-26 00:00:00	
	//$now_date = date_now();
	//if ($end_date < $now_date) return false;
	
	return true;
}

function clsignup_is_league_upcoming($league) {
	$values = field_get_items('node', $league, 'field_visible');
	$visible = $values[0]['value'];
	if (!$visible) return false;

	$values = field_get_items('node', $league, 'field_finalized');
	$finalized = $values[0]['value'];
	if ($finalized) return false;
	
	$values = field_get_items('node', $league, 'field_duration');
	$end_date = new DateObject($values[0]['value2']);	// format 2013-02-26 00:00:00	
	$now_date = date_now();
	if ($end_date < $now_date) return false;
	
	return true;
}

/** ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 *                                                                                             API for lists of leagues
 */ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

function clsignup_get_open_leagues() {
	$open_leagues = array();
	
	$all_leagues = node_load_multiple(array(), array('type' => 'league'));
	
	foreach($all_leagues as $league) {
		if (clsignup_league_available_for_signup($league)) {
			$open_leagues[$league->nid] = $league;
		}
	}
	
	return $open_leagues;
}

function clsignup_get_upcoming_leagues() {
	$upcoming_leagues = array();
	
	$all_leagues = node_load_multiple(array(), array('type' => 'league'));
	
	foreach($all_leagues as $league) {
		if (clsignup_is_league_upcoming($league)) {
			$upcoming_leagues[$league->nid] = $league;
		}
	}
	
	return $upcoming_leagues;
}

function clsignup_get_selectable_leagues() {
	global $user;

	$logged_on = clleague_is_logged_on();

	$open_leagues = clsignup_get_open_leagues();	
	$open_leagues_ids = array_keys($open_leagues);
	
	$selectable_leagues = array();
	if (!$logged_on) {
		foreach (array_keys($open_leagues) as $id) {
			$values = field_get_items('node', $open_leagues[$id], 'field_city');
			$city = $values[0]['value'];
						
			$selectable_leagues[$id] = $city;
		}
	} else {
		foreach (array_keys($open_leagues) as $id) {
			$sid = clsignup_db_get_signed_up_league($user->uid);
			if ($id == $sid) continue; 
			
			if (clleague_is_player($id, $user->uid)) continue;
			
			$values = field_get_items('node', $open_leagues[$id], 'field_city');
			$city = $values[0]['value'];
		
			$selectable_leagues[$id] = $city;
		}
	}
	
	return $selectable_leagues;
}


/** ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 *                                                                                                        API for users
 */ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

function clsignup_is_signed_up_or_player($uid) {
	$sid = clsignup_db_get_signed_up_league($uid);
	
	$upcoming_leagues = clsignup_get_upcoming_leagues();
	$upcoming_leagues_ids = array_keys($upcoming_leagues);

	foreach ($upcoming_leagues_ids as $id) {
		$already_signedup = $id == $sid;
		if ($already_signedup) return true;				
		
		$already_player  = clleague_is_player($id, $uid);
		if ($already_player) return true;		
	}
	
	return false;
}