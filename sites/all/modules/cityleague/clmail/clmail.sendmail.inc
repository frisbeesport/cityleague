<?php

/** //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*                                                                                                            Mail form
*/ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

function clmail_sendmail_page($node) {
	return drupal_get_form('clmail_sendmail_form');
}

function clmail_sendmail_form($form, &$form_state) {
	$form = array();
	
	$nid = arg(1);
	$href = url('node/' . $nid . '/mail/addresses');
	
	$form['intro_text'] = array(
		'#markup' => '<p>' . t('Mail all participants that have agreed to receive messages by email.') . '</p>',
	);
	
	$form['subject'] = array(
		'#type' => 'textfield',
		'#title' => t('Subject'),
		'#description' => t('The subject will automatically be prefixed with the [Cityleague] tag.'),
		'#size' => '40',
		'#maxlength' => 40,
		'#required' => TRUE,
	);
	
	$form['body'] = array(
		'#type' => 'text_format',
		'#title' => t('Body'),
		'#description' => t('A link to unsubscribe from these messages will be added to the bottom of the email.'),	
		'#size' => '40',
		'#required' => TRUE,
		'#format' => 'eenvoudige_opmaak', 
	);
	
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Send'),
	);
  
	return $form;
}

function clmail_sendmail_form_validate($form, &$form_state) {
}

function clmail_sendmail_form_submit(&$form, &$form_state) {
	$nid = arg(1);
	
	$body     = $form_state['values']['body'];
	$subject  = '[Cityleague] ' . $form_state['values']['subject'];


	$params = array(
		'body'    => array($body['value']),
		'subject' => $subject,
		'from'    => 'noreply@cityleague.nl',
    );

	$organizers = clmail_organizers($nid);
	$players = clmail_participants($nid);
	
	foreach ($organizers as $to_user) {
		clmail_sendmail_send_mail($to_user, $params);
	}
        
    foreach ($players as $to_user) {
		if (!in_array($to_user, $organizers)) {
			clmail_sendmail_send_mail($to_user, $params);
		}
	}
	
	drupal_set_message(t('Whoosh! The email has been sent.'));
	
	$form_state['redirect'] = 'node/' . $nid;
}

function clmail_organizers($nid) {
	
	$node = node_load($nid);
	$values = field_get_items('node', $node, 'field_organizers');
	
	$organizers = array();
	foreach ($values as $organizer) {
		$organizer_uid = $organizer['target_id'];
		$organizer_user = user_load($organizer_uid);
			
		if ($organizer_user->field_subscribe && $organizer_user->mail) {
			$organizers[] = $organizer_user;
		}
	}
	
	return $organizers;
}

function clmail_participants($nid) {
	$query = db_select('league_participants', 'ps')
		->fields('ps', array('pid', 'uid'))
		->condition('nid', $nid)
		->isNotNull('ps.uid');
	$result = $query->execute();

	$players = array();	
	foreach ($result as $player) {
		$player_user = user_load($player->uid);
		
		if ($player_user->field_subscribe && $player_user->mail) {
			$players[] = $player_user;
		}
	}
	
	return $players;
}

function clmail_sendmail_send_mail($to_user, $params) {
	global $base_url;
	
	$unsubscribe_hash = clmail_unsubscribe_hash($to_user);
	$unsubscribe_path = $base_url . '/user/' . $to_user->uid . '/unsubscribe/' . $unsubscribe_hash;
	
	$unsubscribe_html  = '<hr style="display: block; height: 1px; border: 0; border-top: 1px solid #ccc; padding: 0; margin-top:40px;">';
	$unsubscribe_html .= '<a href="' . $unsubscribe_path . '" style="color: #ccc; font-size: 11px;">' . t('Unsubscribe from cityleague related messages.') . '</a>';
	
	$params['body'][0] .= $unsubscribe_html;
	
	if (!drupal_mail('clmail', 'clleague_mail', $to_user->mail, language_default(), $params, $params['from'], TRUE)) {
		drupal_set_message(t('This mail could not be sent to: ') . $to_user->mail);    
	}
}

function clmail_sendmail_send_message($email, $params) {
	if (!drupal_mail('clmail', 'clleague_mail', $email, language_default(), $params, $params['from'], TRUE)) {
		drupal_set_message(t('This message could not be sent'));    
	}
}