<?php

/** ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 *                                                                                                               Blocks
 */ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 /**
 * Implements hook_block_info().
 *
 * This hook declares what blocks are provided by the module.
 */
function clleague_block_info() {
	// This hook returns an array, each component of which is an array of block
	// information. The array keys are the 'delta' values used in other block
	// hooks.

	// The required block information is a block description, which is shown
	// to the site administrator in the list of possible blocks. You can also
	// provide initial settings for block weight, status, etc.

	// This sample shows how to provide default settings. In this case we'll
	// enable the block in the first sidebar and make it visible only on
	// 'node/*' pages. See the hook_block_info() documentation for these.
 
	$blocks['user_menu'] = array(
		'info' => t('User menu'),
	);
  	
	return $blocks;
}


/**
 * Implements hook_block_view().
 *
 * This hook generates the contents of the blocks themselves.
 */
function clleague_block_view($delta = '') {
	global $user;
  
	$block = array();

	switch ($delta) {
		case 'user_menu':
			$block['content'] = array('content' => array(
					'#markup' => clleague_draw_user_menu()
				),
			);
			break;
	}
	return $block;
}

 
/*function clleague_block_view($delta = '') {
	global $user;
  
	$block =  array();
	
	switch ($delta) {
		case 'upcoming_leagues':
			$upcoming_leagues = clleague_get_leagues_upcoming($user->uid);
			$running_leagues = clleague_get_leagues_running($user->uid);
			
			if (sizeof($upcoming_leagues) + sizeof($running_leagues) > 0) {
				$upcoming_list = clleague_create_league_list(t('Upcoming leagues:'), $upcoming_leagues);
				$running_list = clleague_create_league_list(t('Ongoing leagues:'), $running_leagues);
				
				$block['subject'] = t('Current leagues');  
				$block['content'] = array ('upcoming' => $upcoming_list, 'running' => $running_list);
			}

			break;    
		case 'my_leagues':
			if (clleague_is_logged_on()) {
				$playing_leagues = clleague_get_leagues_player($user->uid);
				$organizing_leagues = clleague_get_leagues_organizer($user->uid);
				
				if (sizeof($playing_leagues) + sizeof($organizing_leagues) > 0) {
					$playing_list = clleague_create_league_list(t('Leagues you play in:'), $playing_leagues);
					$organizing_list = clleague_create_league_list(t('Leagues you organize:'), $organizing_leagues);
				
					$block['subject'] = t('Your leagues');  
					$block['content'] = array ('playing' => $playing_list, 'organizing' => $organizing_list);
				}
			}
			break; 
	}
	return $block;
}*/

function clleague_draw_user_menu() {
	$contents = '<div class="navbar-inner">';
	$contents .= '<div class="btn-group">';
	$contents .= clleague_draw_coordinator();
	$contents .= '</div>';
	$contents .= ' ';
	$contents .= '<div class="btn-group">';
	$contents .= clleague_draw_myleagues();
	$contents .= '</div>';
	$contents .= ' ';
	$contents .= '<div class="btn-group">';
	$contents .= clleague_draw_logon();
	$contents .= clleague_draw_language_selection();
	$contents .= '</div>';
	$contents .= '</div>';
	return $contents;
}

function clleague_draw_coordinator() {
	global $user;
	
	if (!clleague_is_logged_on()) {
		return;
	}
	
	if (!clleague_is_coordinator($user->uid)) {
		return;
	}
	
	$contents = '<a class="btn btn-sm btn-primary dropdown-toggle" data-toggle="dropdown" href="#">';
    $contents .= t('Coordinate') . ' ';
	$contents .= '<span class="caret"></span>';
	$contents .= '</a>';
	$contents .= '<ul class="dropdown-menu">';
	
	$href = url('dashboard');		
	$contents .= '<li><a href="' . $href . '">' . t('Dashboard') . '</a></li>';
	
	$href = url('node/add/league');		
	$contents .= '<li><a href="' . $href . '">' . t('Create new league') . '</a></li>';
	
	$href = url('admin/people/create');		
	$contents .= '<li><a href="' . $href . '">' . t('Create new user') . '</a></li>';
	
	$contents .= '</ul>';
	
	return $contents;
}

function clleague_draw_myleagues() {
	global $user;
	
	if (!clleague_is_logged_on()) {
		return;
	}

	$player    =  clleague_get_leagues_player($user->uid);
	$organizer =  clleague_get_leagues_organizer($user->uid);

	if (count($player) + count($organizer) == 0) {
		return;
	}
		
	$contents = '<a class="btn btn-sm btn-primary dropdown-toggle" data-toggle="dropdown" href="#">';
    $contents .= t('My leagues') . ' ';
	$contents .= '<span class="caret"></span>';
	$contents .= '</a>';
	$contents .= '<ul class="dropdown-menu">';
	
	if (count($organizer) > 0) {
		if (count($player) > 0) {
			$contents .= '<li class="disabled"><a href="">' . t('Organizing') . '</a></li>';
		}
		
		foreach ($organizer as $node) {
			$values = field_get_items('node', $node, 'field_city');
			$city = $values[0]['value'];
		
			$href = url('node/' . $node->nid);		
			$contents .= '<li><a href="' . $href . '">' . $city . '</a></li>';
		}
	}
	
	if (count($player) > 0) {
		if (count($organizer) > 0) {
			$contents .= '<li class="divider"></li>';
			$contents .= '<li class="disabled"><a href="">' . t('Playing') . '</a></li>';
		}			
		
		foreach ($player as $node) {
			$values = field_get_items('node', $node, 'field_city');
			$city = $values[0]['value'];
			
			$href = url('node/' . $node->nid);		
			$contents .= '<li><a href="' . $href . '">' . $city . '</a></li>';
		}
	}
	
	$contents .= '</ul>';
	
	return $contents;
}

/**
 * Custom language selection button.
 */
function clleague_draw_language_selection() {
  global $language, $base_path;
  
  $current = $language->language;
  
  $path = drupal_is_front_page() ? $base_path : $_GET['q'];
  
  if ($current == 'en')
  {
    return '<a href="/nl/' . $path . '" class="btn btn-sm btn-default "><img src="/sites/all/themes/clresponsive/images/icons/flagnl.png" class="flagicon"> Nederlands</a>';
  }
  else if ($current == 'nl')
  {
    return '<a href="/en/' . $path . '" class="btn btn-sm btn-default "><img src="/sites/all/themes/clresponsive/images/icons/flagen.png" class="flagicon"> English</a>';
  }
}

function clleague_draw_logon() {
  global $user, $language;
  
  if ($user->uid)
  {
    return '<a href="/' . $language->language . '/user/logout" class="btn btn-sm btn-default ">Log uit</a>';
  } 
  else 
  {
    return '<a href="/' . $language->language . '/user" class="btn btn-sm btn-default ">Log in</a>';
  }
}
 



/**
 * Implements hook_user_login
**/
function clleague_user_login(&$edit, $account) {
	$player = clleague_get_leagues_player($account->uid);
	if (sizeof($player) > 0) {
		$edit['redirect'] = 'node/' . $player[0]->nid;			
		return;
	}
	$organizer = clleague_get_leagues_organizer($account->uid);
	if (sizeof($organizer) > 0) {
		$edit['redirect'] = 'node/' . $organizer[0]->nid;			
		return;
	}	
}

/**
 * Get all current (visible and not finalized) leagues in which the user is a player
*/
function clleague_get_leagues_player($uid) {
	$nodes = array();
	
	$all_leagues = node_load_multiple(array(), array('type' => 'league'));
	
	foreach($all_leagues as $league) {
		$values = field_get_items('node', $league, 'field_visible');
		$visible = $values[0]['value'];
		
		$values = field_get_items('node', $league, 'field_finalized');
		$finalized = $values[0]['value'];
		
		if (clleague_is_player($league->nid, $uid) and $visible and !$finalized) {
			$nodes[] = $league;
		}
	}
	
	return $nodes;
}

/**
 * Get all current (not finalized) leagues in which the user is an organizer
*/
function clleague_get_leagues_organizer($uid) {
	$nodes =  array();
	
	$all_leagues = node_load_multiple(array(), array('type' => 'league'));
	
	foreach($all_leagues as $league) {
		$values = field_get_items('node', $league, 'field_finalized');
		$finalized = $values[0]['value'];
		
		if (clleague_is_organizer($league->nid, $uid) and !$finalized) {
			$nodes[] = $league;
		}
	}
	
	return $nodes;
}

/**
 * Get all upcoming leagues 
*/
/*function clleague_get_leagues_upcoming($uid) {
	$nodes =  array();
	
	$all_leagues = node_load_multiple(array(), array('type' => 'league'));
	
	foreach($all_leagues as $league) {
		$values = field_get_items('node', $league, 'field_visible');
		$visible = $values[0]['value'];
		
		$values = field_get_items('node', $league, 'field_finalized');
		$finalized = $values[0]['value'];
		
		$values = field_get_items('node', $league, 'field_signup');
		$signup = $values[0]['value'];
		
		$values = field_get_items('node', $league, 'field_duration');
		$begin_date = new DateObject($values[0]['value']);	//  format 2013-02-26 00:00:00	
		$end_date = new DateObject($values[0]['value2']);	//  format 2013-02-26 00:00:00	
		$now_date = date_now();
		
		if ($now_date <= $end_date && $begin_date >= $now_date && $visible and !$finalized) {
			$nodes[] = $league;
		}
	}
	
	return $nodes;
}*/

/**
 * Get all running leagues (but closed)
*/
/*function clleague_get_leagues_running($uid) {
	$nodes =  array();
	
	$all_leagues = node_load_multiple(array(), array('type' => 'league'));
	
	foreach($all_leagues as $league)
	{
		$values = field_get_items('node', $league, 'field_visible');
		$visible = $values[0]['value'];
		
		$values = field_get_items('node', $league, 'field_finalized');
		$finalized = $values[0]['value'];
		
		$values = field_get_items('node', $league, 'field_signup');
		$signup = $values[0]['value'];
		
		$values = field_get_items('node', $league, 'field_duration');
		$begin_date = new DateObject($values[0]['value']);	// format 2013-02-26 00:00:00	
		$end_date = new DateObject($values[0]['value2']);	// format 2013-02-26 00:00:00	
		$now_date = date_now();
		
		if ($begin_date <= $now_date && $now_date <= $end_date && $visible and !$finalized) {
			$nodes[] = $league;
		}
	}
	
	return $nodes;
}*/

/**
 * Create a list of leagues for a block (render array)
 */
/*function clleague_create_league_list($caption, $leagues) {
	if (sizeof($leagues) ==  0) return;

	$links = array();
	
	foreach ($leagues as $node) {
		$title = $node->title; 	
		$href = 'node/' . $node->nid;
				
		$links[$node->nid] = array('title' => $title, 'href' => $href);
	}
			
	$heading = array(
		'text'   => $caption,
		'level' => 'p',
	);

	$list = array(
		'#type'   => 'markup',
		'#markup' => theme_links(array(
			'links'      => $links,
			'attributes' => array(),
			'heading'    => $heading
		)),
	);
	
	return $list;
}*/