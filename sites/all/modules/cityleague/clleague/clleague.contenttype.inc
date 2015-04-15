<?php


/** ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 *                                                                                          Create content type: League 
 */ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 /**
 * Implements hook_node_info().
 */
function clleague_node_info() {
	//Define our node content type.
	return array(
		'league' => array(
			'name' => 'Cityleague',
			'base' => 'league',
			'description' => 'A cityleague.',
			'locked' => TRUE,
		),
	);
}

/**
 * Implement hook_form().
 *
 * Drupal needs for us to provide a form that lets the user
 * add content. This is the form that the user will see if
 * they go to node/add/node_clleageu.
 *
 * You can get fancy with this form, or you can just punt
 * and return the default form that node_content will provide.
 */
function league_form($node, $form_state) {
	return node_content_form($node, $form_state);
}

 /**
 * Implements hook_node_type_insert().
 *
 * Much like hook_node_insert() lets us know that a node is being
 * inserted into the database, hook_node_type_insert() lets us know
 * that a new content type has been inserted.
 *
 * Since Drupal will at some point insert our new content type,
 * this gives us a chance to add the fields we want.
 */
function clleague_node_type_insert($content_type) {
	if ($content_type->type == 'league') {
		// Add the body field. 
		$body_instance = node_add_body_field($content_type, 'Description of this league');

		// Create all the fields we are adding to our content type.
		foreach (_league_installed_fields() as $field) {
			if (!field_info_field($field['field_name'])) {
				field_create_field($field);
			}
		}

		// Create all the instances for our fields.
		foreach (_league_installed_instances() as $instance) {
			$instance['entity_type'] = 'node';
			$instance['bundle'] = 'league';
			
			$existing_instance = field_read_instance($instance['entity_type'], $instance['field_name'], $instance['bundle']);
			if (empty($existing_instance)) {
				field_create_instance($instance); 
			}
		}
	}
}

/**
 * Define the fields for our content type.
 *
 * This big array is factored into this function for readability.
 *
 * @return
 *  An associative array specifying the fields we wish to add to our
 *  new node type.
 */
function _league_installed_fields() {
	return array(
		'field_duration' => array(
			'field_name' => 'field_duration',
			'cardinality' => 1,
			'type'        => 'datetime',
			'settings'    => array(
				'granularity' => array(
					'month' => 'month',
					'day' => 'day',
					'year' => 'year',
					'hour' => 0,
					'minute' => 0,
					'second' => 0,
				),
				'todate' => 'required',
			),
		),
		'field_organizers' => array(
			'field_name' => 'field_organizers',
			'cardinality' => FIELD_CARDINALITY_UNLIMITED,
			'type'        => 'entityreference',
			'settings'    => array(
				'target_type' => 'user',
				'handler' => 'base',
				'handler_settings' => array(
					'target_bundles' => array(),
					'sort' => array(
						'type' => 'none',
					),
					'behaviors' => array(
						'views-select-list' => array(
							'status' => 0,
						),
					),
				),
			),
		),
		'field_signup' => array(
			'field_name' => 'field_signup',
			'type'       => 'list_boolean',
			'cardinality' => 1, 
			'settings'    => array(
				'allowed_values' => array(
					'Signing up is not possible',
					'Signing up is possible',
				),
			),
		),
		'field_signout' => array(
			'field_name' => 'field_signout',
			'type'       => 'list_boolean',
			'cardinality' => 1, 
			'settings'    => array(
				'allowed_values' => array(
					'Resigning is not possible',
					'Resigning is possible',
				),
			),
		),
		'field_visible' => array(
			'field_name' => 'field_visible',
			'type'       => 'list_boolean',
			'cardinality' => 1, 
			'settings'    => array(
				'allowed_values' => array(
					'This league is not shown on the website',
					'This league is visible on the website',
				),
			),
		),
		'field_finalized' => array(
			'field_name' => 'field_finalized',
			'type'       => 'list_boolean',
			'cardinality' => 1, 
			'settings'    => array(
				'allowed_values' => array(
					'This leaguee is open',
					'This league is finalized',
				),
			),
		),
		'field_max_partcipants' => array(
			'field_name' => 'field_max_participants',
			'type'       => 'number_integer',
		),
	);
}

/**
 * Define the field instances for our content type.
 * The instance lets Drupal know which widget to use to allow the user to enter
 * data and how to react in different view modes.  
 *
 * This big array is factored into this function for readability.
 *
 * @return
 *  An associative array specifying the instances we wish to add to our new
 *  node type.
 */
function _league_installed_instances() {
	return array(
		'field_duration' => array(
			'field_name'  => 'field_duration',
			'label' => 'Duration of the league',
			'widget'      => array(
				'type'    => 'date_popup',
			),
		),
		'field_organizers' => array(
			'field_name'  => 'field_organizers',
			'label' => 'Organizer(s)',
			'description' => 'Organizers of this league',
			'widget'      => array(
				'type'    => 'entityreference_autocomplete',
				'settings' => array(
					'size' => '60',
				),
			),   
		),
		'field_signup' => array(
			'field_name' => 'field_signup',
			'label'       => 'Signing up is possible',
			'widget'      => array(
				'type'    => 'options_onoff',
			),      
		),
		'field_signout' => array(
			'field_name' => 'field_signout',
			'label'       => 'Resigning is possible',
			'widget'      => array(
				'type'    => 'options_onoff',
			),      
		),
		'field_visible' => array(
			'field_name' => 'field_visible',
			'label'       => 'This league is visiable on the website',
			'widget'      => array(
				'type'    => 'options_onoff',
			),      
		),
		'field_finalized' => array(
			'field_name' => 'field_finalized',
			'label'       => 'This league is finalized',
			'widget'      => array(
				'type'    => 'options_onoff',
			),      
		),
		'field_max_participants' => array(
			'field_name'  => 'field_max_participants',
			'label'       => 'Maximum number of players',
			'description' => 'The maximum number of players in this league',
			'widget'      => array(
				'type'    => 'number',
			),      
		),
	);
}