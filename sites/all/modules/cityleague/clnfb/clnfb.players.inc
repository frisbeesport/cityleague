<?php

//TODO: evaluate contents and update/check code

function clnfb_players_page($league) {
	global $user;

	// Theme a table for organizers or players
	$table = array();
	
	if (clleague_is_organizer($league->nid, $user->uid) ||
	    clleague_is_coordinator($user->uid)) {
		$table = clnfb_players_page_for_organizers($league);
	} elseif (clleague_is_player($league->nid, $user->uid)) {
		$table = clnfb_players_page_for_players($league);
	}
	
	// Return table as markup in a renderable array
	$renderable_array = array();
	$renderable_array['table'] = array(
		'table' => array(
			'#type' => 'markup',
			'#markup' => $table,
		),
	);

	return $renderable_array;
}
  
function clnfb_players_page_for_organizers($league) {
	$table = array();
	
	$headers = array(t('Player name'), t('Level'), t('Signup date'), t('Cityleague.nl account'), t('NFB member'));
	
	$headers = array(
		array(
			'data' => t('Player name'), 
			'field' => 'ps.nickname',
			'sort' => 'asc'
		),
		array(
			'data' => t('Level'), 
			'field' => 'ps.level',
		),
		array(
			'data' => t('Signup date'), 
			'field' => 'ps.timestamp',
		),
		array(
			'data' => t('Cityleague.nl account'), 
			'field' => 'ps.uid',
		),
		array(
			'data' => t('NFB member'), 
			'field' => 'nfb.nfbmember',
		),
	);
	
	$query = db_select('league_participants', 'ps')
		->fields('ps', array('pid', 'uid', 'nickname', 'level', 'timestamp'))
		->condition('ps.nid', $league->nid);
	$query->join('league_participants_nfbmembership', 'nfb', 'ps.pid=nfb.pid');
	$query->fields('nfb', array('nfbmember', 'phone'))
		->extend('TableSort')
		->orderByHeader($headers); 
	
	$results = $query->execute();

	$headers[] = array(
			array(
				'data' => t('Action'), 
			),
		);
		
	$rows = array();
	foreach ($results as $record) {
		if  ($record->uid != null) {
			$account = t('Yes');
			$user = user_load($record->uid);
			$account = $user->name;	
		} else {
			$account = t('No');
		}
		
		if  ($record->nfbmember) {
			$member = t('Yes');
		} else {
			$member = t('No');
		}
		global $base_url;
		$remove_url = url('node/' . $league->nid . '/players/player/' .  $record->pid . '/remove');
		
		$rows[] = array(
			array('data' => $record->nickname), 
			array('data' => clleague_level_to_string($record->level)),
			array('data' => format_date($record->timestamp, 'custom', 'd-m-Y')), 
			array('data' => $account),
			array('data' => $member),
			array('data' => '<a class="btn btn-default" href="' . $remove_url . '">'. t('Remove') . '</a>'),
			
		);
	}

	$table = theme_table(array(
		'header' => $headers, 
		'rows' => $rows,
		'attributes' => array('class' => 'table'),
		'caption'  => NULL,
		'colgroups'   => NULL,
		'sticky' => NULL,
		'empty' => t('No players have signed up for the league yet.'),
	));
		
	return '<div class="table-responsive">' . $table . '</div>';
}

function clnfb_players_page_for_players($league) {
	global $user;
			
	$table = array();
	
	$headers = array(
		array(
			'data' => t('Player name'), 
			'field' => 'ps.nickname',
			'sort' => 'asc'
		),
	);
	
	$query = db_select('league_participants', 'ps')
		->fields('ps', array('nickname'))
		->condition('nid', $league->nid)
		->extend('TableSort')
		->orderByHeader($headers);
	$results = $query->execute();

	$rows = array();
	foreach ($results as $record) {
		$rows[] = array($record->nickname);
	}

	$table = theme_table(array(
		'header' => $headers, 
		'rows' => $rows,
		'attributes' => array('class' => 'table responsive'),
		'caption'  => NULL,
		'colgroups'   => NULL,
		'sticky'   => NULL,
		'empty' => t('No players have signed up for the league yet.'),
	));
	
	return $table;
}
