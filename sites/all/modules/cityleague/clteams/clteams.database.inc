<?php

/** ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 *                                                                                                                 CRUD 
 */ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
function clteams_get_list_of_teams($nid) {
	$teams = array();
	
	$query = db_select('league_teams', 'ts')
		->fields('ts', array('tid', 'nid', 'name', 'color'))
		->condition('nid', $nid)
		->extend('TableSort')
		->orderBy('name', 'asc'); 
		
	$results = $query->execute();
	
	foreach ($results as $record) {
		$teams[$record->tid] = $record;
	}
	
	return $teams;
}

 
function clteams_get_league_for_team($tid) {
	$query = db_select('league_teams')
		->fields('league_teams', array('nid'))
		->condition('tid', $tid);
		
	$result = $query->execute();
	
	$team = $result->fetchObject();
	
	$nid = NULL;
	if ($team) {
		$nid = $team->nid;
	}
	
	return $nid;
}
	
function clteams_get_pid_in_league($uid, $nid) {
	return clleague_retrieve_pid_from_nid_and_uid($nid, $uid);
}
