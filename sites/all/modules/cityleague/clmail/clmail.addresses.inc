<?php

/** //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 *                                                                                                      Addresses page
 */ //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

function clmail_addresses_page($node) {
	$renderable_array = array();
	
	$renderable_array['caption']  = array(
		'#markup' => '<p>' . t('These addresses are from players that have agreed to receive cityleague related messages. Be careful not to spam and please use BCC.') . '</p>',
	);
	
	$nid = arg(1);
	$addresses = clmail_generate_participants_bcc($nid);
	$renderable_array[''] = array(
		'#markup' => '<p><strong>' . t('Adresses') . ':</strong> ' . $addresses,
	); 
	
	return $renderable_array;
}

function clmail_generate_participants_bcc($nid) {
	$query = db_select('league_participants', 'ps')
		->fields('ps', array('pid', 'uid', 'nickname'))
		->condition('ps.nid', $nid)
		->isNotNull('ps.uid');
	$result = $query->execute();
	
	$players = array();
	foreach ($result as $row) {
		$org = user_load($row->uid);

		if ($org->field_subscribe && $org->mail) {
			$players[] = $row->nickname . ' &lt' . $org->mail . '&gt';
		}
	}

	$address_string = implode(', ', $players);
	
	return $address_string;
}