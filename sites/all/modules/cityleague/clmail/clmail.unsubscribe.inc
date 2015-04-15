<?php

function clmail_unsubscribe_page($user, $hash) {
	$renderable_array = array();
	
	if (clmail_unsubscribe_hash($user) == $hash) {
		$renderable_array['success'] = array(
			'table' => array(
				'#type' => 'markup',
				'#markup' => '<p>'. t('You have unsubscribed from cityleague related messages. Note that if you are playing in a league at this moment, we are now no longer sending you information regarding this league. Please keep in touch.') . '</p>',
			),
		);
		user_save($user, array ('field_subscribe' => '0'));
	} else {
		$renderable_array['failure'] = array(
			'table' => array(
				'#type' => 'markup',
				'#markup' => '<p>' . t('Unfortunately, we were not able to unsubscribe you from cityleague related messages.') . '</p>',
			),
		);
	}
	return $renderable_array;
}
