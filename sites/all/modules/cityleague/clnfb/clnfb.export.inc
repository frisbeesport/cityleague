<?php

function clnfb_export_page($node) {
	// Retrieve all current league participants
	$query = db_select('league_participants', 'ps')
			->fields('ps', array('pid', 'uid', 'nickname', 'level', 'timestamp'))
			->condition('ps.nid', $node->nid);
		$query->join('league_participants_nfbmembership', 'nfb', 'ps.pid=nfb.pid');
		$query->fields('nfb', array('nfbmember', 'surname', 'callname', 'address', 'sex', 'postal', 'city', 'phone', 'email', 'nationality', 'birthday', 'repr_fullname')); 
		
	$results = $query->execute();
	
	$csv_array = array(
		array(t('Player name'), t('Level'), t('NFB member'), t('Last name'), t('First name'), t('Address'), t('Sex'), t('Postal'), t('City'), t('Phone'), t('Email'), t('Birthday'), t('Nationality'), t('Representative'), t('Protest 3rds')),
	);
	
	foreach ($results as $row) {
		// Format birthday
		$birthday = '';
		if (isset($row->birthday)) {
			$birthday = format_date($row->birthday, 'custom', 'd-m-Y');
		}
		
		// For now, add 'bezwaar' for all new members
		if ($row->nfbmember == '1')
		{
			$bezwaar = '0';	
		}
		else
		{
			$bezwaar = '1';
		}
		
		// Add email address from actual user account if not in table (historical reasons)
		if ($row->email == '') {
			$loadeduser = user_load($row->uid);
			if ($loadeduser) {
				$row->email = $loadeduser->mail;
			}
		}
		
		$csv_array[] = array(
			$row->nickname,
			$row->level,
			$row->nfbmember,
			$row->surname,
			$row->callname,
			$row->address,
			$row->sex,
			$row->postal,
			$row->city,
			$row->phone,
			$row->email,
			$birthday,
			$row->nationality,
			$row->repr_fullname,
			$bezwaar,
		);
	}
	
	// Export csv file to browser for download
	
	// send response headers to the browser
	drupal_add_http_header('Content-Type', 'text/csv');
	drupal_add_http_header('Content-Disposition', 'attachment;filename=participants.csv');

	$fp = fopen('php://output', 'w');
	foreach($csv_array as $line){
		fputcsv($fp, $line, ';');
	}
	
	fclose($fp);
	drupal_exit();
}