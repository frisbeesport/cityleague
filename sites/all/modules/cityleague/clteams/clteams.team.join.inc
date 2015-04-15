<?php

function clteams_team_join_page($node, $tid) {
	return drupal_get_form('clteams_team_join_form');
}

function clteams_team_join_form($form, &$form_state) {
	global $user;
	
	$tid = arg(4);
	$nid = arg(1);
	$teams = clteams_get_list_of_teams($nid);
	$team = $teams[$tid];
	
	$message = t('Join team: @team');
	drupal_set_title(format_string($message, array('@team' => $team->name)));
		
	$form = array();
	
	$form['intro_text'] = array(
		'#markup' => '<p>' . t('Are you sure you want to join this team?') . '</p>',
	);
		
	$form['cancel'] = array(
		'#type' => 'submit', 
		'#name' => 'cancel', 	
		'#value' => t('Cancel'),
		'#submit' => array('clteams_team_join_form_cancel'),
		'#limit_validation_errors' => array(),
	);
	
	$form['submit'] = array(
		'#type' => 'submit', 
		'#value' => t('Join'),
		'#weight' => 40,
	);
	
	return $form;
}

function clteams_team_join_form_cancel(&$form, &$form_state) {
	$nid = arg(1);
	$tid = arg(4);
	
	drupal_set_message(t('You have not joined the team.'));
	
	$form_state['redirect'] = 'node/' . $nid . '/teams';
}

function clteams_team_join_form_submit(&$form, &$form_state) {
	global $user;
	
	$nid = arg(1);
	$tid = arg(4);
	$pid = clteams_get_pid_in_league($user->uid, $nid);
	$teams = clteams_get_list_of_teams($nid);
	$team = $teams[$tid];
	
	$fields = array(
		'tid' => $tid,
		'pid' => $pid,
		'manager' => 1,
	);
	
	$query = db_insert('league_team_members')
		->fields($fields)
		->execute();
	
	$message = t('You have joined the team: @team!');
	drupal_set_message(format_string($message, array('@team' => $team->name)));
	
	$form_state['redirect'] = 'node/' . $nid . '/teams';
}

