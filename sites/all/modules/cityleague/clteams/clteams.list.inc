<?php

function clteams_list_page($league)	{
	$renderable_array = array();
	
	$teams = clteams_get_list_of_teams($league->nid);
	
	if (count($teams) == 0) 
	{
		$renderable_array['intro'] = array(
			'#type' => 'markup',
			'#markup' => '<p>' . t('There are no teams created for this league yet.') . '</p>',
		);
	} else {
		
		$renderable_array['intro'] = array(
			'#type' => 'markup',
			'#markup' => '<p>' . t('These are the teams playing in this league.') . '</p>',
		);
			
		$join = false;
		global $user;
		$pid = clteams_get_pid_in_league($user->uid, $league->nid);
		
		if (clleague_is_player($league->nid, $user->uid)
			&& clteams_supports_user_teams($league->nid)
			&& clteams_is_orphin($pid, $league->nid))
		{	
			$join = true;
		}	
			
		$renderable_array['start'] = array(
				'#type' => 'markup',
				'#markup' => '<div class="row">',
			);
			
		$count = 0;
		foreach ($teams as $tid => $team) {
			$edit = false;
			if (clleague_access_level_organizer($user, $league)) {
				$edit = true;
			} elseif (clleague_access_level_player($user, $league)) {
				$pid = clteams_get_pid_in_league($user->uid, $league->nid);
				$edit = clteams_is_team_member($pid, $tid) && clteams_supports_user_teams($league->nid);
			} 
			
			$unjoin = false;
			if (!$join
				&& clleague_is_player($league->nid, $user->uid)
				&& clteams_supports_user_teams($league->nid)
				&& clteams_is_team_member($pid, $tid)) {
					$unjoin = true;
				}	
			
			
			$renderable_array['team' . $tid] = array(
				'#type' => 'markup',
				'#markup' => '<div class="col-sm-6">' . clteams_list_team($league, $team, $join, $unjoin, $edit) . '</div>',
			);
			
			$count++;
			if ($count % 2 == 0)
			{
				$renderable_array['split' . $tid] = array(
					'#type' => 'markup',
					'#markup' => '</div><div class="row">',
				);
			}
		}
		
		$renderable_array['stop'] = array(
				'#type' => 'markup',
				'#markup' => '</div>',
			);
	}
	
	$unassigned = clteams_list_unassigned($teams, $league->nid);
	if ($unassigned) {
		$renderable_array['separator'] = array(
			'#type' => 'markup',
			'#markup' => '<hr>',
		);
		
		$renderable_array['unassigned'] = array(
			'#type' => 'markup',
			'#markup' => '<div class="row"><div class="col-sm-6">' . $unassigned . '</div></div>',
		);
	}
	
	return $renderable_array;
}

function clteams_list_team($league, $team, $join, $unjoin, $edit) {
	$query = db_select('league_team_members', 'ts')
		->fields('ts', array('tid', 'pid', 'manager'))
		->condition('ts.tid', $team->tid);
	$query->join('league_participants', 'ps', 'ps.pid=ts.pid');
	$query->fields('ps', array('nickname', 'level'))
		->extend('TableSort');
			
	$players = $query->execute()->fetchAll();

	//$rows = array();
	/*foreach ($players as $player) {
		$rows[] = array(
			array('data' => $player->nickname), 
			array('data' => clleague_level_to_string($player->level)),
		);
	}*/
	
	$caption = $team->name . ' (' . $team->color . ')';

	$editurl = url('node/' . $league->nid . '/teams/team/' . $team->tid . '/edit');
	$joinurl = url('node/' . $league->nid . '/teams/team/' . $team->tid . '/join');
	$unjoinurl = url('node/' . $league->nid . '/teams/team/' . $team->tid . '/unjoin');
	
	$buttons = '<div class="buttons pull-right">';
	if ($edit) $buttons = $buttons . '<a class="btn btn-default" href="'. $editurl . '">' . t('Edit') . '</a>';
	if ($join) $buttons = $buttons . '<a class="btn btn-success" href="'. $joinurl . '">' . t('Join') . '</a>';
	if ($unjoin) $buttons = $buttons . '<a class="btn btn-default" href="'. $unjoinurl . '">' . t('Unjoin') . '</a>';
	$buttons .= '</div>';
	
	return clteams_markup_table($players, $caption, $buttons);
}


function clteams_list_unassigned(&$teams, $nid) {
	/*$headers = array(
		array(
			'data' => t('Player name'), 
			'field' => 'ps.nickname',
			'sort' => 'asc'
		),
		array(
			'data' => t('Level'), 
			'field' => 'ps.level',
		),
	);
	$rows = array();*/
	
	$players_in_team = clteams_get_players_in_any_team_list($nid);
	
	$query = db_select('league_participants', 'ps')
		->fields('ps', array('pid', 'nickname', 'level'))
		->condition('ps.nid', $nid);
	$result = $query->execute();

	$all_players = array();
	foreach ($result as $player) {
		$all_players[] = $player;
	}
	
	$options = array();
	foreach ($teams as $option_tid => $team) {
		$options[$option_tid] = $team->name;
	}
	$options['0'] = '-- ' . t('no team') . ' --';

	$players = array();
	if (count($all_players) - count($players_in_team) > 0) {
		foreach ($all_players as $player) {
			if (!in_array($player->pid, array_keys($players_in_team))) {
				$players[] = $player;
				$rows[] = array(
					array('data' => $player->nickname), 
					array('data' => clleague_level_to_string($player->level)),
				);
			}
		}
	} else {
	}
	
	if (count($players) > 0) {
		return clteams_markup_table($players, t('Players without a team'), '');
	} else {
		return "";
	}
}

function clteams_markup_table($players, $caption, $buttons) {
	$markup  = '<div class="panel panel-default">';
	$markup .= '<div class="panel-heading">';
    $markup .= '<h2 class="panel-title">' . $caption . '</h2>';
    $markup .= '</div>';
    $markup .= '<div class="panel-body">';
    
	if (count($players) > 0){
	
		$markup .= '<table class="table borderless">';
		$markup .= '<tr><th>' . t('Name') . '</th><th>' . t('Level') . '</th></tr>';
		
		foreach ($players as $player) {
			$markup .= '<tr><td>' . $player->nickname . '</td><td>' . clleague_level_to_string($player->level) . '</td></tr>';
		}
		
		$markup .= '</table>';
	} else {
		$markup .= t('This team does not have any players yet.');
		
		if ($buttons != "") {
		$markup .= '<br><br>';
		}
	}
	
	$markup .= $buttons;
	
    $markup .= '</div>';
	$markup .= '</div>';
	
	return $markup;
}

function clteams_get_players_in_any_team_list($nid) {
	$query = db_select('league_participants', 'ps')
		->fields('ps', array('pid', 'nickname', 'level'))
		->condition('nid', $nid);
	$query->join('league_team_members', 'ts', 'ps.pid=ts.pid');
	$query->fields('ts', array('pid'));
	
	$results = $query->execute();
	
	$players = array();
	foreach ($results as $record) {
		$players[$record->pid] = $record;
	}
	
	return $players;
}