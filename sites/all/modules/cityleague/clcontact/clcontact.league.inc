<?php

/** //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*                                                                                                Coordinator dashboard
*/ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Add to League page?

function clcontact_league_page($node) {
	return drupal_get_form('clcontact_league_form', $node);
}
	
function clcontact_league_form($form, &$form_state, $node) {
	global $user;

	$logged_on = clleague_is_logged_on();
	
	$form = array();

	// honeypot protection	
	if (!$logged_on) {
		honeypot_add_form_protection($form, $form_state, array('honeypot', 'time_restriction'));
	} else {
		honeypot_add_form_protection($form, $form_state, array('honeypot'));
	}

	$message = t('Whether you have a question or an experience with @league you want to share: we love to hear from you! Please send your message and we will get back to you as soon as possible.', array('@league' => $node->title)); 


	$form['title'] = array(
		'#markup' => '<br><h2 class="script-font">' . t('We love to hear from you!') . '</h2>',
	);
	
	$form['intro_text'] = array(
		'#markup' => '<p>' . $message . '</p>',
	);

	

	$form['body'] = array(
		'#type' => 'text_format',
		'#title' => t('Message'),
		'#size' => '80',
		'#required' => TRUE,
		'#rows' => 10,
		'#format' => 'plain_text',
	);
	
	if (!$logged_on)
	{
		$form['email'] = array(
			'#type' => 'textfield',
			'#title' => t('Your email address'),
			'#size' => 25,
			'#maxlength' => 80,
			'#required' => FALSE,
		);
	}

	$form['submit'] = array(
		'#type' => 'submit', 
		'#value' => t('Send'),
		'#weight' => 40,
		'#attributes' => array(  
				'class' => array('pull-right',),
			),
	);
	
	return $form;
}	

function clcontact_league_form_submit(&$form, &$form_state) {
	global $user;

	$nid = arg(1);
	$node = node_load($nid);
			
	$email = "";
	$email_prefix = '<p>' . t('An anonymous visitor says:') . '</p>';

	if (isset($form_state['values']['email']) && $form_state['values']['email'] != null) {
		$email = $form_state['values']['email'];
		$email_prefix = '<p><a href="mailto:' . $email . '">' . $email . '</a> ' . t('says:') . '</p>';
	} else if (clleague_is_logged_on()) {
		$email = $user->mail;	
		$email_prefix = '<p><a href="mailto:' . $email . '">' . $email . '</a> ' . t('says:') . '</p>';
	} 

	$body     = $form_state['values']['body'];
	$subject  = '[Cityleague] Contact about ' . $node->title;

	$params = array(
		'body'    => array($email_prefix . check_markup($body['value'], 'plain_text') . '<br /><br />'),
		'subject' => $subject,
		'from'    => 'noreply@cityleague.nl',
    );

	$organizers = clcontact_get_league_email($node);//clmail_organizers($node->nid);
	clmail_sendmail_send_message($organizers, $params);
	
	if ($email == null)
	{
		drupal_set_message(t('Whoosh! Thanks for your message.'));
	}
	else
	{
		drupal_set_message(t('Whoosh! You will hear from us soon!'));		
	}

	$form_state['redirect'] = 'node/' . $node->nid;
}

function clcontact_league_form_validate(&$form, &$form_state) {
	if (array_key_exists('email', $form_state['values'])) {
		$email = $form_state['values']['email'];

		if ($email != null && !valid_email_address($email)) {
	    	form_set_error('email', t('Please enter a valid email address.'));
			return;
		}
	}
}
