<?php

/** ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 *                                                                                                        State helpers
 */ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
function clteams_supports_user_teams($nid) {
	$node = node_load($nid);
	$values = field_get_items('node', $node, 'field_user_managed_teams');
	
	if ($values != FALSE) {
		return $values[0]['value'];
	} 
	
	return false;
}

function clteams_supports_teams($nid) {
	$node = node_load($nid);
	$values = field_get_items('node', $node, 'field_teams');
	
	if ($values != FALSE) {
		return $values[0]['value'];
	} 
	
	return false;
}

function clteams_is_orphin($pid, $nid) {
	$manager = NULL;
	
	$query = db_select('league_teams', 'ts')
		->fields('ts', array('tid', 'nid'))
		->condition('ts.nid', $nid);
	$query->join('league_team_members', 'ps', 'ps.tid=ts.tid');
	$query->fields('ps', array('pid'))
		->condition('ps.pid', $pid);
		
	$result = $query->execute();
	
	return $result->rowCount() == 0;
}

function clteams_is_team_member($pid, $tid) {
	$manager = NULL;
	
	$query = db_select('league_team_members')
		->fields('league_team_members', array('pid'))
		->condition('tid', $tid)
		->condition('pid', $pid);
		
	$result = $query->execute();
	
	return $result->rowCount() > 0;
}
