<?php

function clteams_assign_page($league)	{
	return drupal_get_form('clteams_assign_form');
}

function clteams_assign_form($form, &$form_state) {
	global $user;
	
	$nid = arg(1);
	
	$form = array();
	
	$form['#tree'] = true;
	$form['#attached']['css'] = array(drupal_get_path('module', 'clteams') . '/assignform.css',);
	
	$form['intro_text'] = array(
		'#markup' => '<p>' . t('Alter the team assignment below.') . '</p>',
	);
	
	$teams = clteams_get_list_of_teams($nid);
		
	foreach ($teams as $tid => $team) {
		clteams_team_join_form_addteam($form, $tid, $teams);
	}
	
	clteams_team_join_form_addunassigned($form, $teams, $nid);
	
	$form['submit'] = array(
		'#type' => 'submit', 
		'#value' => t('Apply'),
		'#weight' => 40,
		'#attributes' => array(  
				'class' => array('btn-success'),
			),
	);
	
	return $form;
}

function clteams_team_join_form_addteam(&$form, $tid, &$teams) {
	$players = clteams_get_players_in_team($tid);

	$options = array();
	foreach ($teams as $option_tid => $team) {
		$options[$option_tid] = $team->name;
	}
	$options['0'] = '-- ' . t('no team') . ' --';

	$form['team' . $tid] = array(
	    '#type' => 'fieldset',
	    '#title' => $teams[$tid]->name,
	);
	
	if (count($players) > 0) {
		foreach ($players as $pid => $player) {
			$form['team' . $tid]['player' . $pid] = array(
				'#type' => 'select',
				'#title' => $player->nickname . ' (' . t('Level') . ' ' . $player->level . ')',
				'#options' => $options,	
				'#default_value' => $tid
			);
		}
	} else {
		$form['team' . $tid]['noplayers'] = array(
	    '#type' => 'markup',
	    '#markup' => t('This team does not have any players yet.'),
	);
	}
}


function clteams_team_join_form_addunassigned(&$form, &$teams, $nid) {
	$players_in_team = clteams_get_players_in_any_team($nid);
	
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

	if (count($all_players) - count($players_in_team) > 0) {
		$form['team0'] = array(
			'#type' => 'fieldset',
			'#title' => t('Players without a team'),
		);

	
		foreach ($all_players as $player) {
			if (!in_array($player->pid, array_keys($players_in_team))) {
				$form['team0']['player' . $player->pid] = array(
					'#type' => 'select',
					'#title' => $player->nickname . ' (' . t('Level') . ' ' . $player->level . ')',
					'#options' => $options,	
					'#default_value' => '0'
				);
			}
		}
	} else {
	}
}

function clteams_assign_form_submit(&$form, &$form_state) {
	$nid = arg(1);
	
	$teams = clteams_assign_get_teams_from_form($form);
	
	foreach ($teams as $tid) {
		$players = clteams_assign_get_players_from_form_team($form, $tid);
		
		foreach ($players as $pid) {
			$oldtid = $form['team' . $tid]['player' . $pid]['#default_value'];
			$newtid = $form_state['values']['team' . $tid]['player' . $pid];
			
			clteams_assign_player($nid, $pid, $oldtid, $newtid);
		}
	}
	
	$message = t('The team assignment has been changed!');
	drupal_set_message($message);
	$form_state['redirect'] = 'node/' . $nid . '/teams';
}

function clteams_assign_get_teams_from_form(&$form) {
	$keys = array_keys($form);
	
	$teams = array();
	foreach ($keys as $key) {
		if (strncmp($key, 'team', 4) == 0) {
			$team = substr($key, 4);
			
			$teams[] = $team;
		}
	}
	
	return $teams;
}

function clteams_assign_get_players_from_form_team(&$form, $tid) {
	$keys = array_keys($form['team' . $tid]);
	
	$players = array();
	foreach ($keys as $key) {
		if (strncmp($key, 'player', 6) == 0) {
			$player = substr($key, 6);
			
			if ($player != 0) {
				$players[] = $player;
			}
		}
	}
	
	return $players;
}

function clteams_assign_player($nid, $pid, $oldtid, $newtid) {
	if ($newtid == 0 && $oldtid != 0) {
		db_delete('league_team_members')
			->condition('tid', $oldtid)
			->condition('pid', $pid)
			->execute();
	} elseif ($newtid != $oldtid) {
		if ($oldtid != 0) {
			db_update('league_team_members')
				->condition('tid',  $oldtid)
				->condition('pid',  $pid)
				->fields(array(
					'tid' => $newtid,
				))->execute();
		} else {
			db_insert('league_team_members')
				->fields(array(
					'tid' => $newtid,
					'pid' => $pid,
					'manager' => 1,
				))->execute();
		}
	}
}

function clteams_get_players_in_team($tid) {
	$query = db_select('league_team_members', 'ts')
		->fields('ts', array('tid', 'pid', 'manager'))
		->condition('ts.tid', $tid);
	$query->join('league_participants', 'ps', 'ps.pid=ts.pid');
	$query->fields('ps', array('nickname', 'level'))
		->orderBy('ps.nickname', 'asc'); 
	$results = $query->execute();
	
	$players = array();
	foreach ($results as $record) {
		$players[$record->pid] = $record;
	}
	return $players;
}

function clteams_get_players_in_any_team($nid) {
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