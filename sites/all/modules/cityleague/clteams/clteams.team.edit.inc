<?php

// Team manager is ignored

function clteams_team_edit_page($node, $tid) {	
	return drupal_get_form('clteams_team_edit_form');
}

function clteams_team_edit_form($form, &$form_state) {
	global $user;
	
	$tid = arg(4);
	$nid = arg(1);
	
	$teams = clteams_get_list_of_teams($nid);
	$team = $teams[$tid];
	
	$message = "Edit team: @team";
	drupal_set_title(format_string($message, array('@team' => $team->name)));
		
	$form = array();

	$form['intro_text'] = array(
		'#markup' => '<p>' . t('Edit the team information.') . '</p>',
	);

	$form['name'] = array(
		'#type' => 'textfield',
		'#title' => t('Team name'),
		'#description' => t('The name of the team within this league.'),
		'#size' => 40,
		'#maxlength' => 40,
		'#required' => TRUE,
		'#default_value' => $team->name,
	);
	
	$form['color'] = array(
		'#type' => 'textfield',
		'#title' => t('Color'),
		'#description' => t('The color in which the team plays.'),
		'#size' => 40,
		'#maxlength' => 40,
		'#required' => TRUE,
		'#default_value' => $team->color,
	);
	
	$form['delete_button'] = array(
		'#markup' => '<a class="btn btn-danger pull-left" href="' . url('node/' . $nid . '/teams/team/' . $tid . '/delete') . '">' . t('Delete') . '</a>',
	);

	$form['submit'] = array(
		'#type' => 'submit', 
		'#value' => t('Apply changes'),
		'#weight' => 40,
	);
	
	return $form;
}

function clteams_team_edit_form_validate(&$form, &$form_state) {
	$nid = arg(1);
	$thistid = arg(4);
	
	$name  = $form_state['values']['name'];  // should not already exist within league
	$color = $form_state['values']['color']; // not checked
	
	$teams = clteams_get_list_of_teams($nid);
	foreach ($teams as $tid => $team) {
		if ($team->name == $name && $tid != $thistid) {
			form_set_error('name', t('There is already a team with this name in the league. That is going to be confusing. Please choose a different name for your team.'));
			return false;
		}
	}
}

function clteams_team_edit_form_submit(&$form, &$form_state) {
	$nid = arg(1);
	$tid = arg(4);
	
	global $user;
	
	$name  = $form_state['values']['name'];   
	$color = $form_state['values']['color']; 
	
	$fields = array(
		'name' => $name,
		'color' => $color,
	);
	
	$query = db_merge('league_teams')
			->key(array('tid' => $tid))
			->fields($fields)
			->execute();
	
	$message = t('The team @team has been edited!');
	drupal_set_message(format_string($message, array('@team' => $name)));
	$form_state['redirect'] = 'node/' . $nid . '/teams';
}
