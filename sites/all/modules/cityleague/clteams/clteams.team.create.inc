<?php

// TODO: When a player, and not an organizer, automatically join team as captain.

function clteams_create_page() {	
	return drupal_get_form('clteams_create_form');
}

function clteams_create_form($form, &$form_state) {
	global $user;
	
	$nid = arg(1);
	$node = node_load($nid);

	$form = array();
	
	$message = t('Create a new team for @league. Please enter the team information below.'); 
	$form['intro_text'] = array(
		'#markup' => '<p>' . format_string($message, array('@league' => $node->title)) . '</p>',
	);

	$form['name'] = array(
		'#type' => 'textfield',
		'#title' => t('Team name'),
		'#description' => t('The name of the team within the league.'),
		'#size' => 40,
		'#maxlength' => 40,
		'#required' => TRUE,
		'#default_value' => '',
	);
	
	$form['color'] = array(
		'#type' => 'textfield',
		'#title' => t('Color'),
		'#description' => t('The color in which the team plays.'),
		'#size' => 40,
		'#maxlength' => 40,
		'#required' => TRUE,
		'#default_value' => '',
	);
	
	$form['submit'] = array(
		'#type' => 'submit', 
		'#value' => t('Create'),
		'#weight' => 40,
	);
	
	return $form;
}

function clteams_create_form_validate(&$form, &$form_state) {
	$nid = arg(1);

	$name  = $form_state['values']['name'];  // should not already exist within league
	$color = $form_state['values']['color']; // not checked
	
	$teams = clteams_get_list_of_teams($nid);
	foreach ($teams as $tid => $team) {
		if ($team->name == $name) {
			form_set_error('name', t('There is already a team with this name in the league. That is going to be confusing. Please choose a different name for your team.'));
			return false;
		}
	}
}

function clteams_create_form_submit(&$form, &$form_state) {
	$nid = arg(1);
	$tid = arg(4);
	
	$name  = $form_state['values']['name'];   
	$color = $form_state['values']['color']; 
	
	$fields = array(
		'nid' => $nid,
		'name' => $name,
		'color' => $color,
	);
	
	$query = db_insert('league_teams')
			->fields($fields)
			->execute();
	
	$message = t('The team @team has been created!');
	drupal_set_message(format_string($message, array('@team' => $name)));
	$form_state['redirect'] = 'node/' . $nid . '/teams';
}
