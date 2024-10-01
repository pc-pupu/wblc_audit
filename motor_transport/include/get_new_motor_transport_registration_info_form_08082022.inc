<?php

drupal_add_css(drupal_get_path('module', 'motor_transport').'/css/popup.css');
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
drupal_add_js(drupal_get_path('module', 'motor_transport') . '/js/mtw_upload.js');


function get_new_motor_transport_registration_info_form($form, &$form_state,$backlog_id='') {
	global $base_root, $base_path, $user;
	
	$user_id				= $user->uid;

	$applicant_details	 	= db_select('l_common_application_master', 'lcam');
	$applicant_details->fields('lcam', array('id', 'est_name', 'est_loc', 'est_district', 'est_subdivision', 'est_loc_e_areatype', 'est_name_areatype', 'est_loc_e_vill_ward','est_ps', 'est_pin_number','gender','est_type'));
	$applicant_details->condition('lcam.user_id', $user_id);
	$get_profile_status 	= $applicant_details->execute();
	
	//print_r($get_profile_status->fetchAssoc());die;
		
		if( $get_profile_status->rowCount() > 0 ){
			
			$obj 					= 	$get_profile_status->fetchAssoc(); 
			
			
			$est_name					=	$obj['est_name'];
			
			$address				=	$obj['est_loc'];
			$country_code			=   $obj['country'];
			$state_code				=   $obj['state'];
			$district_code			=	$obj['est_district'];
			$subdivision_code		=	$obj['est_subdivision'];
			$areatype_code			=	$obj['est_loc_e_areatype'];
			$name_areatype_code		=	$obj['est_name_areatype'];
			$vill_ward_code			=	$obj['est_loc_e_vill_ward'];
			$ps_code				=	$obj['est_ps'];
			$pin					=	$obj['est_pin_number'];
			
			
		}else{
			$address				=	'';
			$country_code			=   '';
			$state_code				=   '';
			$district_code			=	'';
			$subdivision_code		=	'';
			$areatype_code			=	'';
			$name_areatype_code		=	'';
			$vill_ward_code			=	'';
			$ps_code				=	'';
			$pin					=	'';
		}

	$query = db_select('l_mtw_registration_master','mrm');
			$query->fields('mrm',array());
			$query->condition('mrm.user_id',$user_id);
			$result = $query->execute();
			if($result->rowCount() > 0){
				$mtw_reg = $result->fetchAssoc();
				//print_r($mtw_reg['id']);die;
				if($mtw_reg['final_submit_status'] == ''){
					// $message	=	'Application Not Saved ! Please Try Again';
					// drupal_set_message(t($message),'error');
					drupal_goto('mtw-registration-details/default/'.encryption_decryption_fun('encrypt', $mtw_reg['id']));
				
				}
			}

	$form['mtw_name'] = array(
		'#prefix' 			=> '<div class="panel panel-default panel-form">
									<div class="panel-heading">Name and Address to which communications relating to the Motor Transport undertaking should be sent</div>
									<div class="panel-body">
										
											<div class="col-lg-4 col-md-6"><label class="input">',
		'#suffix' 			=> '</label></div>',
		'#title' 			=> '1) Name of Motor Transport Undertaking',
		'#type' 			=> 'textfield',
		'#default_value'    =>  $est_name,
		'#required'   		=>  TRUE,
		'#element_validate' => array('REGISTRATION_TEXTBOX_TEXT_VALIDATE'),
		
	);	
	
	$form['mtw_location'] = array(
		'#title' 		=> '2.1) Address Line 1',
		'#type' 		=> 'textarea',
		'#default_value'	=> $address,
		'#required' 	=>  TRUE,
		'#attributes' 	=> array('id' => '','autocomplete' => 'off','class'=>array(''),'placeholder'=>'Motor Transport Undertaking Location'),
		'#prefix' 		=> '<div class="col-lg-8 col-md-8 form-textarea-wrapper textarea" style="width:auto;"><label class="input label-block">',
		'#suffix' 		=> '</label></div>'
	);
	
	$options_dist  = common_custom_user_district_list_new();
	
	$form['mtw_district'] = array(
		'#title' 		=> '2.2) Select District',
		'#type' 		=> 'select',
		'#options' 		=> $options_dist,
		'#default_value'	=> $district_code,
		'#required' 	=>  TRUE,
		//'#id' 			=> 'loc_e_dist_id',
		'#ajax'			=> array(
								'event' 	=> 'change',
								'effect' 	=> 'fade',			
								'callback' 	=> 'get_mtw_registration_info_form_ajax_callback',
								'progress' 	=> '',
						),
		'#attributes' 	=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select District'),
		'#prefix' 		=> '<div class="col-lg-4 col-md-6"><label class="select">',
		'#suffix' 		=> '</label></div>'
	);
	
	//$district = ($district_code != '') ? $district_code : '';
	$value_loc_e_dist 			= isset($form_state['values']['mtw_district']) ? $form_state['values']['mtw_district'] : $district_code;
	
	if($value_loc_e_dist != "" || !empty($value_loc_e_dist) || is_numeric($value_loc_e_dist)){				
		
		$form['mtw_subdivision'] = array(
			'#title'		=> '2.3) Select Subdivision',
			'#type' 		=> 'select',
			'#options' 		=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist),
			'#default_value'	=> $subdivision_code,
			'#required' 	=>  TRUE,
			'#ajax'			=> array(
									'event' 	=> 'change',
									'effect' 	=> 'fade',			
									'callback' 	=> 'get_mtw_registration_info_form_ajax_callback',
									'progress' 	=> '',
								),		
			'#attributes' 	=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Sub-Division','onkeyup' => 'getSpanText()', 'onblur' => 'getSpanText()', 'onchange' => 'getSpanText()'),
			'#prefix' 		=> '<div id="mtw_subdvsn_replace"><div class="col-lg-4 col-md-6"><label class="select">',
			'#suffix' 		=> '</label></div></div>'
		);
	}else {
		
		$form['mtw_subdivision'] = array(
			'#title'		=> '2.3) Select Subdivision',
			'#type' 		=> 'select',
			'#options' 		=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist),
			'#default_value'	=> $subdivision_code,
			'#required' 	=>  TRUE,
			'#ajax'			=> array(
									'event' 	=> 'change',
									'effect' 	=> 'fade',			
									'callback' 	=> 'get_mtw_registration_info_form_ajax_callback',
									'progress' 	=> '',
								),		
			'#attributes' 	=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Sub-Division','onkeyup' => 'getSpanText()', 'onblur' => 'getSpanText()', 'onchange' => 'getSpanText()'),
			'#prefix' 		=> '<div id="mtw_subdvsn_replace"><div class="col-lg-4 col-md-6"><label class="select">',
			'#suffix' 		=> '</label></div></div>'
		);
	}
	//$subdivision = ($subdivision_code != '') ? $subdivision_code : '';
	$value_loc_e_subdivision1 = isset($form_state['values']['mtw_subdivision']) ? $form_state['values']['mtw_subdivision'] : $subdivision_code;
	
	$form['divmkup'] = array(
		'#type' => 'markup',
		'#prefix' => '',
	);
	
	$areaTypeData  = array('-Select Area Type-');
	if($value_loc_e_dist != "" && $value_loc_e_subdivision1 != ""){	
		$areatypeOptions 		= common_custom_user_areatype_list_new($value_loc_e_dist, $value_loc_e_subdivision1);
		
		if(empty($areatypeOptions)){
			$areatypeOptions	= array();
		}
		$form['mtw_areatype'] = array(
			'#title'		=> '2.4) Select Block/Municipality/Corporation/SEZ/Notified Area',
			'#type' 		=> 'select',
			'#options' 		=> $areatypeOptions,
			'#default_value'	=> $areatype_code,
			'#required' 	=>  TRUE,
			'#attributes' 	=> array('autocomplete' => 'off', 'placeholder'=>'Select Block/Municipality/Corporation/SEZ/Notified Area'),
			'#ajax'			=> array(
									'event' 	=> 'change',
									'effect' 	=> 'fade',			
									'callback' 	=> 'get_mtw_registration_info_form_ajax_callback',
									'progress'	=> '',
							),	
			'#prefix' 		=> '<div id="mtw_loc_e_areatype"><div class="col-lg-4 col-md-6"><label class="select">',
			'#suffix' 		=> '</label></div></div>'
		);
	}else {
		$form['mtw_areatype'] = array(
			'#title'		=> '2.4) Select Block/Municipality/Corporation/SEZ/Notified Area',
			'#type' 		=> 'select',
			'#options' 		=> $areaTypeData,
			//'#default_value'	=> $areatype_code,
			'#required' 	=>  TRUE,
			'#attributes' 	=> array('autocomplete' => 'off', 'placeholder'=>'Select Block/Municipality/Corporation/SEZ/Notified Area'),
			'#ajax'			=> array(
									'event' 	=> 'change',
									'effect' 	=> 'fade',			
									'callback' 	=> 'get_mtw_registration_info_form_ajax_callback',
									'progress' 	=> '',
							),	
			'#prefix' 		=> '<div id="mtw_loc_e_areatype"><div class="col-lg-4 col-md-6"><label class="select">',
			'#suffix' 		=> '</label></div></div>'
		);
	}
 
 	$value_loc_e_areatype = isset($form_state['values']['mtw_areatype']) ? $form_state['values']['mtw_areatype'] : $areatype_code;
 	
	$blockData = array('-Select Name-');
	$form['mtw_block'] = array(
		'#title'		=> '2.5) Name of Block',
		'#type' 		=> 'select',
		'#options'		=> $blockData,
		
		'#required' 	=>  TRUE,
		'#attributes' 	=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Name of Block'),
		'#ajax'			=> array(
								'event' 	=> 'change',
								'effect' 	=> 'fade',			
								'callback' 	=> 'get_mtw_registration_info_form_ajax_callback',
								'progress' 	=> '',
			),	
		'#prefix' => '<div id="mtw_common_1_replace"><div class="col-lg-4 col-md-6"><label class="select">',
		'#suffix' => '</label></div></div>'
	);

	if($value_loc_e_dist != "" && $value_loc_e_subdivision1 != ""){
		if($value_loc_e_areatype != ""){	
			if ( $value_loc_e_areatype == 'B' ){
				$areatype_name	= 'Block';
			}elseif( $value_loc_e_areatype == 'M'){
				$areatype_name	= 'Municipality';
			}elseif( $value_loc_e_areatype == 'C'){
				$areatype_name	= 'Corporation';
			}elseif( $value_loc_e_areatype == 'S'){
				$areatype_name	= 'SEZ';
			}elseif($value_loc_e_areatype == 'N'){
				$areatype_name	= 'Notified Area';
			}
						
						
			$form['mtw_block'] 		= array(
				'#prefix' => '<div id="mtw_common_1_replace"><div class="col-lg-4 col-md-6"><label class="select">',
				'#suffix' => '</label></div></div>',
				'#title' => '2.5) Name of '.$areatype_name,
				'#type'  => 'select',
				'#default_value'	=> $name_areatype_code,
				'#options'  => common_custom_user_block_list_new($value_loc_e_subdivision1,$value_loc_e_areatype),
				'#attributes'  => array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select '.$areatype_name),
				'#required'  => TRUE,
				'#ajax'	 => array(
					'event' 			=> 'change',
					'effect' 			=> 'fade',			
					'callback'			=> 'get_mtw_registration_info_form_ajax_callback',
					'progress'			=> '',
				  )
			);
		}
	}
				
	$value_common_selection_1 = isset($form_state['values']['mtw_block']) ? $form_state['values']['mtw_block'] : $name_areatype_code;
	$form['divmkup1']  = array(
			'#type'				=> 'markup',
			'#prefix' 			=> '',
	);
	
	$panchyatData = array();
	$form['mtw_panchyat'] = array(
		'#title' 			=> '2.6) Select Gram Panchayat/Ward',
		'#type' 			=> 'select',
		'#options' 			=> $panchyatData,
		
		'#required' 		=> TRUE,
		'#attributes' 		=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Gram Panchayat/Ward'),
		'#ajax'				=> array(
									'event' 	=> 'change',
									'effect' 	=> 'fade',			
									'callback' 	=> 'get_mtw_registration_info_form_ajax_callback',
									'progress' 	=> '',
								  ),	
		'#prefix' 			=> '<div id="mtw_common_2_replace"><div class="col-lg-4 col-md-6"><label class="select">',
		'#suffix' 			=> '</label></div></div>'
	);
	
	if($value_loc_e_dist != "" && $value_loc_e_subdivision1 != ""){
		if($value_loc_e_areatype != ""){
			if($value_common_selection_1 != ""){
				
				if($value_loc_e_areatype == 'B'){
					$titleLable = 'Gram Panchayat Name';
					$funName	= common_custom_user_villward_list_new($value_common_selection_1,'V');
				}else if($value_loc_e_areatype == 'M'){
					$titleLable = 'Ward Number';
					$funName	= common_custom_user_villward_list_new($value_common_selection_1,'W');
				}else if($value_loc_e_areatype == 'C'){
					$titleLable = 'Ward Number';
					$funName	= common_custom_user_corporation_list_new($value_common_selection_1);
				}else if($value_loc_e_areatype == 'S'){
					$titleLable = 'Sector';
					$funName	= common_custom_user_villward_list_new($value_common_selection_1,'S');
				}else if($value_loc_e_areatype == 'N'){
					$titleLable = 'Notified Area';
					$funName	= common_custom_user_villward_list_new($value_common_selection_1,'N');
				}else{
					$titleLable	= 'Gram Panchayat Name';
					$funName	= array();
				}
				
				$form['mtw_panchyat'] = array(
					  '#type' 		=> 'select',
					  '#title' 		=> '2.6) '.$titleLable,
					  '#options' 	=> $funName,
					  '#default_value'	=> $vill_ward_code,
					  '#required' 	=>  TRUE,
					  '#attributes' => array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Gram Panchayat'),
					  '#ajax'		=> array(
											  'event' 		=> 'change',
											  'effect' 		=> 'fade',			
											  'callback' 	=> 'get_mtw_registration_info_form_ajax_callback',
											  'progress' 	=> '',
										),	
					  '#prefix' 	=> '<div id="mtw_common_2_replace"><div class="col-lg-4 col-md-6"><label class="select">',
					  '#suffix' 	=> '</label></div></div>'
				);
													
			}
		}
	}
			 
	$value_common_selection_2 = isset($form_state['values']['mtw_panchyat']) ? $form_state['values']['mtw_panchyat'] : $vill_ward_code;
	
	$policeData = array('-Select-');
	$form['mtw_policestation'] = array(
		'#title' 			=> '2.7) Select Police Station',
		'#type' 			=> 'select',
		'#options' 			=> $policeData,
		
		'#required' 		=>  TRUE,
		'#attributes' 		=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Police Station'),
		'#prefix' 			=> '<div id="mtw_ps_replace"><div class="col-lg-4 col-md-6"><label class="select">',
		'#suffix' 			=> '</label></div></div>'
	);
	
	if($value_loc_e_dist != "" && $value_loc_e_subdivision1 != ""){
		if($value_loc_e_areatype != "" && $value_loc_e_areatype != "0"){
			if($value_common_selection_1 != "" && $value_common_selection_1 != "0"){
				if($value_common_selection_2 != "" && $value_common_selection_2!='0'){
					$form['mtw_policestation'] = array(
						'#title' 			=> '2.7) Select Police Station',
						'#type' 			=> 'select',
						'#options' 			=> common_custom_user_ps_list_new($value_loc_e_dist),
						'#default_value'	=> $ps_code,
						'#required' 		=>  TRUE,
						'#attributes' 		=> array('id' => 'loc_e_ps_id','autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Police Station'),
						'#prefix' 			=> '<div id="mtw_ps_replace"><div class="col-lg-4 col-md-6"><label class="select">',
						'#suffix' 			=> '</label></div></div>'
					);
				}
			}
		}
	}
	$form['mtw_pincode'] = array(
		'#title' 			=> '2.8) Pin Code',
		'#type' 			=> 'textfield',
		'#default_value'	=> $pin,
		'#required' 		=>  TRUE,
		'#element_validate' => array('REGISTRATION__TEXTBOX_NUMERIC_VALIDATE'),
		'#attributes' 		=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Pin Code','maxlength' => 6),
		'#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
		'#suffix' 			=> '</label></div>'
	);
	$mtwNatureArray = array('' => 'Select','city_service'=>'City Service','long_distance'=>'Long Ddistance','passenger_service'=>'Passenger Service','long_distance_freight_service'=>'Long distance freight service', 'other'=>'Other');
	
	$form['markup3'] = array(
		'#type' => 'markup',
		'#markup' => '</div></div>'
	
	);
	
	
	/********** End of Address**********/
	
	 $form['markup4'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="panel panel-default panel-form">
						<div class="panel-heading">Nature of Motor Transport Service </div>
						<div class="panel-body"><div class="row">'
	
	 );
	
	 $form['mtw_nature'] = array(
		'#title' 			=> '3) Nature of Motor Transport Service <span id="hint_nature"><img src="'.$base_root.$base_path.'sites/all/modules/applicant_registration/images/tooltip.jpg"></span>',
		'#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="select">',
		'#suffix' 			=> '</label></div>',
		'#type' 			=> 'select',
		'#options'			=> $mtwNatureArray,
		'#attributes' 		=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Motor Transport Undertaking Nature'),
		//'#required' 		=>  TRUE,
		'#ajax'				=> array(
									  'event' 	=> 'change',
									  'effect' 	=> 'fade',			
									  'callback' => 'get_other_div_ajax_callback',
									  'progress' => '',
									),
	 );
	
	 $form['nature_other'] = array(
		'#prefix' 			=> '<div id="nature_other_div">',
		'#suffix' 			=> '</div>'
	 );
	
	 $natureVal = isset($form_state['values']['mtw_nature'])?$form_state['values']['mtw_nature']:'';
	
	 if(!empty($natureVal) && $natureVal!='' && $natureVal=='other'){
		
		$form['nature_other'] = array(
			'#prefix' 			=> '<div id="nature_other_div"><section class="col-md-4"><label class="input">',
			'#suffix' 			=> '</label></section></div>',
			'#type' 			=> 'textfield',
			'#title' 			=> 'Other Details',
			'#required' 		=>  TRUE,
		);	
	 }
	
	 $form['markup5'] = array(
		'#type' => 'markup',
		'#markup' => '</div></div></div>'
	 );
		
	/******** Nature Block ********/
	
	$form['markup6'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="panel panel-default panel-form">
						<div class="panel-heading">3. Route Information Details </div>
						<div class="panel-body">'
	
	);
	
	
	 $form['mtw_totalroute'] = array(
		'#title' 			=> '4) Total Number Of Routes',
		'#type' 			=> 'textfield',
		'#required' 		=>  TRUE,
		'#attributes' 		=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Total number of routes(if not applicable write 0)'),
		'#element_validate' => array('REGISTRATION__ONLY_NUMERIC_VALIDATE'),
		'#prefix' 			=> '<div class="row"><div class="col-lg-4 col-md-6"><label class="input">',
		'#suffix' 			=> '</label></div></div>',
		'#ajax'				=> array(
			'event'=> 'keyup',
			'effect'=> 'fade',
			'callback'=> 'add_more_ajax_callback'
		)
	 );

	 $form['mtw_route_details'] = array(		
		'#prefix'	=> '<div id="multiple_div"><label class="input">',
		'#suffix'	=> '</label></div>',
	
	);
	$form['mtw_totalmilage'] = array(
		'#prefix'	=> '<div id="multiple_div_milage"><label class="input">',
		'#suffix'	=> '</label></div>'
	);



	 $routeNumber = isset($form_state['values']['mtw_totalroute']) ? $form_state['values']['mtw_totalroute'] : 0;
	 
	if($routeNumber != 0){
		
		for($i = 1; $i<= $routeNumber; $i++){
			
			$form['mtw_route_details']['route_'.$i] = array(
						'#title' 			=> '4.1.'.$i.') Route Details'.$sum,
						'#type' 			=> 'textfield',
						'#attributes' 		=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Route Details'),
						'#prefix' 			=> '<div class="col-md-6"><div id="multiple_div"><label class="input">',
						'#suffix' 			=> '</div></div>'
			);
			$form['mtw_totalmilage']['milage_'.$i] = array(
				'#title' 			=> '4.2.'.$i.') Route Mileage (in km)',
				'#type' 			=> 'textfield',
				'#attributes' 		=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Route Mileage in numbers 20, 25'),
				'#prefix' 			=> '<div class="col-md-6"><div id="multiple_div_milage">',
				'#suffix' 			=> '</label></div></div>'
				

			
			);
			
		}
		
	}


	$form['mtw_totalmilage_ajax'] = array(
		'#title' 			=> '5) Total Routes Mileage(in km)',
		'#type' 			=> 'textfield',
		//'#id'				=> 'mtw_totalmilage',
		//'#required' 		=>  TRUE,
		'#value'	=> $sum,
		'#attributes' 		=> array('autocomplete' => 'off','readonly' => 'readonly','class'=>array(''),'placeholder'=>'Total Routes Mileage'),
		'#element_validate' => array('REGISTRATION__ONLY_NUMERIC_VALIDATE'),
		'#prefix' 			=> '<div class="row"><div class="col-lg-4 col-md-6" id="totalmilage_mtw"><label class="input">',
		'#suffix' 			=> '</label></div></div>',
		
	);

	
		
		
	
	
	
	$form['markup7'] = array(
		'#type'	  => 'markup',
		'#markup' => '</div></div>'
	);
	
	/***************************************************************/
	
	$form['markup8'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="panel panel-default panel-form">
						<div class="panel-heading">4. Number of Motor Transport / Vehicles on the last date of the preceeding year </div>
						<div class="panel-body">'
	
	);
	
	$form['mtw_totalvehicle'] = array(
		'#title' 			=> '6. Total Number of Motor Transport / Vehicles on the last date of the preceeding year',
		'#type' 			=> 'textfield',
		'#attributes' 		=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Total Number of Motor Transport Vehicles'),
		'#element_validate' => array('REGISTRATION__ONLY_NUMERIC_VALIDATE'),
		'#required' 		=>  TRUE,
		'#prefix' 			=> '<div class="row"><div class="col-lg-4 col-md-6"><label class="input">',
		'#suffix' 			=> '</label></div></div>',
		'#ajax'				=>	array(
									'event'=> 'keyup',
									'effect'=> 'fade',
									'callback'=> 'add_reg_number_ajax_callback'
								)
	);
	
	$form['reg_number']	= array(
		'#prefix' 			=> '<div id="renum_div">',
		'#suffix' 			=> '</div>'	
	);
	
	$vehicleNumber = isset($form_state['values']['mtw_totalvehicle']) ? $form_state['values']['mtw_totalvehicle'] : 0;
	
	if($vehicleNumber!=0){
		for($j = 1; $j<= $vehicleNumber; $j++){
			$form['reg_number']['singl_'.$j] = array(
				'#title'			=> '6.'.$j.') Motor Vehicle Registration Number',
				'#type'				=> 'textfield',
				'#prefix' 			=> '<section class="col-md-4"><div id="renum_div"><label class="input">',
				'#suffix' 			=> '</label></div></section>'
			);
		}
	}
	
	$form['markup9'] = array(
		'#type'	  => 'markup',
		'#markup' => '</div></div>'
	);
	
	/******** route details and reg number ***************/
	
	$form['markup10'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="panel panel-default panel-form">
						<div class="panel-heading">5. Number of Motor Transport Worker employed on any day during the preceeding year </div>
						<div class="panel-body">'
	
	);
	
	$form['mtw_maxworkers'] = array(
		'#title' 			=> '7) Maximum number motor transport workers employed on any day during the preceeding year',
		'#type' 			=> 'textfield',
		'#required'   		=>  TRUE,
		'#element_validate' => array('REGISTRATION__ONLY_NUMERIC_VALIDATE'),
		'#prefix' 			=> '<div class="row"><div class="col-lg-4 col-md-6"><label class="input">',
		'#suffix' 			=> '</label></div></div>'
	);
	
	$form['backlog_id'] = array(
		'#title' 			=> 'Backlog Id',
		'#type' 			=> 'hidden',
		'#value'			=>	isset($backlog_id) ? encryption_decryption_fun('decrypt', $backlog_id) : ''
	);
	
	$form['markup11'] = array(
		'#type' => 'markup',
		'#markup' => '</div></div>'
	);
	
	$form['submit_button'] = array(
		'#type'		=>'submit',
		'#value'	=>t('Save & Continue'),
		'#prefix' 	=> '',
		'#suffix' 	=> '',
		'#attributes' 	=> array('class' => array('btn btn-primary pull-right')),
	);
	
  return $form;

}
function REGISTRATION_TEXTBOX_TEXT_VALIDATE($element,&$form_state){
	$txtValue		=	trim($element['#value']); 
	$match_pattern 	= 	'/^[a-zA-Z0-9. ]+$/';
	if(!preg_match($match_pattern, $txtValue)) {
		form_error($element, t($element['#title'].' should be text only'));
	}
}
function REGISTRATION__TEXTBOX_NUMERIC_VALIDATE($element,&$form_state){
	$fieldValue=trim($element['#value']); 
	$match_pattern = '/^[0-9]+$/i';
	
	if(!empty($fieldValue)){
		if(!preg_match($match_pattern, $fieldValue)) {
			form_error($element, t($element['#title'].' should be number only'));
		}
		if(strlen($fieldValue)!=6){
			form_error($element, t($element['#title'].' should be 6 digit'));
		}
	}
}

function REGISTRATION__ONLY_NUMERIC_VALIDATE($element,&$form_state){
	$fieldValue=trim($element['#value']); 
	$match_pattern = '/^[0-9]+$/i';
	
	if(!empty($fieldValue)){
		if(!preg_match($match_pattern, $fieldValue)) {
			form_error($element, t($element['#title'].' should be number only'));
		}
	}
}

function get_new_motor_transport_registration_info_form_submit($form, &$form_state){
	
	global $base_root, $base_path, $user;
	$val 					= $form_state['values'];
	
	$user_id				= $user->uid;
	$mtw_name				= $val['mtw_name'];
	$mtw_loc_address		= $val['mtw_location'];
	$mtw_loc_dist			= $val['mtw_district'];
	$mtw_loc_subdivision	= $val['mtw_subdivision'];
	$mtw_loc_areatype		= $val['mtw_areatype'];
	$mtw_loc_areatype_code	= $val['mtw_block'];
	$mtw_loc_vill_ward		= isset($val['mtw_panchyat'])?$val['mtw_panchyat']:0;
	$mtw_loc_ps				= $val['mtw_policestation'];
	$mtw_loc_pincode		= $val['mtw_pincode'];
	
	
	$mtw_nature				= $val['mtw_nature'];	
	if($mtw_nature=='other'){
		$mtw_nature			= $val['nature_other'];
	}
	
	$mtw_totalvehicels		= $val['mtw_totalvehicle'];
	$mtw_maxworkers			= $val['mtw_maxworkers'];
	
	$total_routes 			= isset($val['mtw_totalroute'])?$val['mtw_totalroute']:0;	
	
	if($val['backlog_id']!=''){
		$backlog_id	=	$val['backlog_id'];
	}else{
		$backlog_id	=	0;
	}
	// multiple registration number
	
	if($mtw_totalvehicels!=''){
		$tempString	='';
		for($j=1;$j<=$mtw_totalvehicels;$j++){
			$tempString .= $val['singl_'.$j].'??';
		}
	}
	
	// insert array
	$insertArray	= array(
				'identification_number' => 'MTW'.time(),
				'act_id'				=> 3,
				'user_id'				=> $user_id,
				
				'mtw_name'				=> $mtw_name,
				'mtw_loc_address'		=> $mtw_loc_address,
				'mtw_loc_dist'			=> $mtw_loc_dist,
				'mtw_loc_subdivision'	=> $mtw_loc_subdivision,
				'mtw_loc_areatype'		=> $mtw_loc_areatype,
				'mtw_loc_areatype_code'	=> $mtw_loc_areatype_code,
				'mtw_loc_vill_ward'		=> $mtw_loc_vill_ward,
				'mtw_loc_ps'			=> $mtw_loc_ps,
				'mtw_loc_pincode'		=> $mtw_loc_pincode,
				
				'mtw_nature'			=> $mtw_nature,
				'total_routes'			=> $total_routes,
				'vreg_number_details'	=> $tempString,
				'total_mtw_vehicle'		=> $mtw_totalvehicels,
				'mtw_maxworkers'		=> $mtw_maxworkers,
				'backlog_id'			=> $backlog_id
			);
			$query = db_select('l_mtw_registration_master','mrm');
			$query->fields('mrm',array());
			$query->condition('mrm.user_id',$user_id);
			$result = $query->execute();
			if($result->rowCount() > 0){

				$message = 'Already Register';
				drupal_set_message(t($message),'error');
				drupal_goto('mtw-registration');

			}else{
				if( !empty($insertArray) ){		
					$application_id = db_insert('l_mtw_registration_master')->fields($insertArray)->execute();
					
					
					if($total_routes!=0){
						for($i=1;$i<=$total_routes;$i++){
							$insertArray2 = array(
												'application_id'	=> $application_id,
												'route'				=> $val['route_'.$i],
												'milage'			=> $val['milage_'.$i],
																						
												);
							db_insert('l_mtw_registration_extended')->fields($insertArray2)->execute();			
						}
					}	
			}
			
		
		
		if($application_id != ""){ 
			$message = 'Application Saved Successfully';
			$phone = get_common_function_for_phone();
			if(!empty($phone)){
				$msg = "Your Motor Transport Worker application has been successfully saved. You can track your application status from dashboard.";
				 //module_load_include('inc', 'common_encryption_form', 'include/test_cryptography_form.inc');
				 //send_sms($phone, $msg);
				 //send_sms('9749689171', $msg);				
			}else{
				$msg = "Message not sent.Please try again later.";
				drupal_set_message(t($msg));
			}
			
			drupal_set_message(t($message));
			drupal_goto('mtw-registration-details/addmore/'.encryption_decryption_fun('encrypt', $application_id));
		}else{
			$message = 'Application Not Saved ! Please Try Again';
		    drupal_set_message(t($message),'error');
			drupal_goto('mtw-registration');
		}
	}
}

function get_mtw_registration_info_form_ajax_callback($form, &$form_state) {
		$commands = array();
		$commands[] = ajax_command_replace('#mtw_subdvsn_replace', drupal_render( $form['mtw_subdivision']));
		$commands[] = ajax_command_replace('#mtw_loc_e_areatype', drupal_render($form['mtw_areatype']));
		$commands[] = ajax_command_replace('#mtw_common_1_replace', drupal_render($form['mtw_block']));
		$commands[] = ajax_command_replace('#mtw_common_2_replace', drupal_render($form['mtw_panchyat']));
		$commands[] = ajax_command_replace('#mtw_ps_replace', drupal_render($form['mtw_policestation']));
		
  		return array('#type' => 'ajax', '#commands' => $commands);
}
function get_other_div_ajax_callback($form, $from_state){
		$data = array();
		$data[]	= ajax_command_replace('#nature_other_div',drupal_render($form['nature_other']));
		return array('#type' => 'ajax', '#commands' => $data);
}
function add_more_ajax_callback($form, &$form_state){
	$commands = array();
	$commands[] = ajax_command_replace('#multiple_div', drupal_render($form['mtw_route_details']));
	$commands[] = ajax_command_replace('#multiple_div_milage', drupal_render($form['mtw_totalmilage']));
	return array('#type' => 'ajax', '#commands' => $commands);
}

function add_reg_number_ajax_callback($form, &$form_state){
		$commands	=	array();
		$commands[]	=	ajax_command_replace('#renum_div',drupal_render($form['reg_number']));
		return array('#type' => 'ajax', '#commands' => $commands);	
}

// function calculate_ajax_callback($form, &$form_state){
// 	$commands = array();
// 	$commands[] = ajax_command_replace('#mtw_totalmilage', drupal_render($form['mtw_totalmilage_ajax']));
// 	return array('#type' => 'ajax', '#commands' => $commands);
// }