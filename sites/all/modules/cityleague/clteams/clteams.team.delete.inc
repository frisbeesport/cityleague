<?php

function clteams_team_delete_page($node, $tid)	{
	return drupal_get_form('clteams_team_delete_form');
}

function clteams_team_delete_form($form, &$form_state) {
	global $user;
	
	$tid = arg(4);
	$nid = arg(1);
	
	$teams = clteams_get_list_of_teams($nid);
	$team = $teams[$tid];
	
	$message = t('Delete team: @team');
	drupal_set_title(format_string($message, array('@team' => $team->name)));
	
	$form = array();

	$form['intro_text'] = array(
		'#markup' => '<p>' . t('Are you sure you want to delete this team?') . '</p>',
	);
		
	$form['cancel'] = array(
		'#type' => 'submit', 
		'#name' => 'cancel', 	
		'#value' => t('Cancel'),
		'#submit' => array('clteams_team_delete_form_cancel'),
		'#limit_validation_errors' => array(),
	);

	$form['submit'] = array(
		'#type' => 'submit', 
		'#value' => t('Delete'),
		'#weight' => 40,
	);
	
	return $form;
}

function clteams_team_delete_form_cancel(&$form, &$form_state) {
	$nid = arg(1);
	$tid = arg(4);
	
	drupal_set_message(t('The team has not been removed.'));
	$form_state['redirect'] = 'node/' . $nid . '/teams/team/' . $tid . '/edit';	
}

function clteams_team_delete_form_submit(&$form, &$form_state) {
	global $user;
	
	$nid = arg(1);
	$tid = arg(4);
	
	$query = db_delete('league_teams')
			->condition('tid', $tid)
			->execute();
			
	$query = db_delete('league_team_members')
			->condition('tid', $tid)
			->execute();
	
	$message = t('The team has been deleted!');
	drupal_set_message($message);
	
	$form_state['redirect'] = 'node/' . $nid . '/teams';
}

