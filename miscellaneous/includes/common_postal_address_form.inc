<?php 
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');


function postal_address_form($form, &$form_state, $application_id='' , $act_id = ''){
	
	global $base_root, $base_path, $user;
	
	$user_id 				= $user->uid;
	$readonly_postal_pin	= array('maxlength' => 6);
	
	$decrypt_act_id 		= encryption_decryption_fun('decrypt', $act_id);
	$decrypt_application_id	= encryption_decryption_fun('decrypt', $application_id);
	
	if(!empty($decrypt_application_id) && !empty($decrypt_act_id) && $decrypt_application_id!='' && $decrypt_application_id!=0 ){
		
		if($decrypt_act_id == 4){
			
			$ismw_reg 		 = 	db_select('l_interstate_workman_master', 'liwm');			
			$ismw_reg 		 -> fields('liwm', array('e_postal_address','e_postal_dist','e_postal_subdivision','e_postal_areatype','e_postal_areatype_code','e_postal_vill_ward','e_postal_ps','e_postal_pin_number','status','final_submit_status'));
			$ismw_reg		 ->  condition('liwm.id', $decrypt_application_id);	
			$ismw_reg		 ->  condition('liwm.act_id', $decrypt_act_id);
			$ismw_reg		 -> condition('liwm.user_id', $user_id);
			$ismw_reg_result = $ismw_reg->execute()->fetchAssoc();
			
			
			if(!empty($ismw_reg_result)){
				
				$status 				= $ismw_reg_result['status'];
				$final_submit_status	= $ismw_reg_result['final_submit_status'];	
			}
			
		}
		
	}
	
	
	$form['common_postal']['header']			 			 	= array(
																		'#markup' 			=> t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">POSTAL ADDRESS OF THE ESTABLISHMENT</div><div class="panel-body">'),
																		);
																		
	$form['common_postal']['application_id'] 					= array(
																	  '#type' 				=> 'hidden',
																	  '#default_value'		=> $application_id,
																	  '#attributes'			=> array('readonly'=>'readonly')
																	);
						
	$form['common_postal']['act_id'] 							= array(
																	  '#type' 				=> 'hidden',
																	  '#default_value'		=> $act_id,
																	  '#attributes' 		=> array('readonly'=>'readonly')
																	);
																	
	$form['common_postal']['final_submit_status'] 				= array(
																	  '#type' 				=> 'hidden',
																	  '#default_value'		=> $final_submit_status,
																	  '#attributes' 		=> array('readonly'=>'readonly')
																	);
	
	
	$form['common_postal']['e_postal_address'] 					= array(
																	  '#prefix' 			=> '<div class="col-md-8"><label class="input">',
																	  '#suffix' 			=> '</label></div>',
																	  '#title'				=> 'House No./Village Name/Street/Road',
																	  '#type' 				=> 'textfield',
																	  '#default_value' 		=> !empty($ismw_reg_result['e_postal_address']) ? $ismw_reg_result['e_postal_address'] : '',
																	  '#required' 			=> TRUE,
																	  '#attributes' 		=> $readonly_e_postal_address,
																	  '#element_validate'	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE'),
																	);
																	
																
	$form['common_postal']['e_postal_dist'] 					= array(
																	  '#prefix' 			=> '<div class="col-md-4"><label class="select">',
																	  '#suffix' 			=> '</label></div>',
																	  '#title'				=> 'Select District',
																	  '#type' 				=> 'select',
																	  '#options' 			=> common_custom_user_district_list_new(),
																	  '#default_value' 		=> !empty($ismw_reg_result['e_postal_dist']) ? $ismw_reg_result['e_postal_dist']	:'',
																	  '#required' 			=> TRUE,
																	  '#attributes'			=> $readonly_e_postal_dist,
																	  '#ajax'				=> array(
																								'event' 	=> 'change',
																								'effect' 	=> 'fade',			
																								'callback'	=> 'e_postal_form_ajax_callback',
																								'progress'	=> '',
																							  )
																	);
																	
	
	$value_e_postal_dist 			= 	isset($form_state['values']['e_postal_dist']) ? $form_state['values']['e_postal_dist'] : $ismw_reg_result['e_postal_dist'];	
	
	if($value_e_postal_dist != "" || !empty($value_e_postal_dist) || is_numeric($value_e_postal_dist)){
		
		$form['common_postal']['e_postal_subdivision'] 			= array(
																	  '#prefix' 			=> '<div id="e_postal_subdvsn_replace" class="col-md-4"><label class="select">',
																	  '#suffix' 			=> '<i></i></label></div>',
																	  '#title'				=> 'Select Subdivision',
																	  '#type' 				=> 'select',
																	  '#options' 			=> common_custom_user_subdivisionlist_list_new($value_e_postal_dist),
																	  '#default_value' 		=> !empty($ismw_reg_result['e_postal_subdivision']) ? $ismw_reg_result['e_postal_subdivision']	:'',
																	  '#required' 			=> TRUE,
																	  '#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'e_postal_form_ajax_callback',
																								'progress'			=> '',
																							  ),
																);	
	}else{
		
		$form['common_postal']['e_postal_subdivision'] 			= array(
																	  '#prefix' 			=> '<div id="e_postal_subdvsn_replace" class="col-md-4"><label class="select">',
																	  '#suffix' 			=> '<i></i></label></div>',
																	  '#title'				=> 'Select Subdivision',
																	  '#type' 				=> 'select',
																	  '#options' 			=> array(),
																	  '#required' 			=> TRUE,
																	  '#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'e_postal_form_ajax_callback',
																								'progress'			=> '',
																							  ),
																);
		
	}
	
	$value_e_postal_subdivision 			= isset($form_state['values']['e_postal_subdivision']) ? $form_state['values']['e_postal_subdivision'] : $ismw_reg_result['e_postal_subdivision'];
	
	if($value_e_postal_dist != "" && $value_e_postal_subdivision != ""){
		
		$areatypeOptions_1						= common_custom_user_areatype_list_new($value_e_postal_dist, $value_e_postal_subdivision);
	
		if(!empty($areatypeOptions_1)){
		
	
	
			$form['common_postal']['e_postal_areatype'] 		= array(
																	  '#prefix' 			=> '<div id="e_postal_loc_e_areatype" class="col-md-4"><label class="select">',
																	  '#suffix' 			=> '<i></i></label></div>',
																	  '#title'				=> 'Select Areatype',
																	  '#type' 				=> 'select',
																	  '#options' 			=> $areatypeOptions_1,
																	  '#default_value' 		=> !empty($ismw_reg_result['e_postal_areatype']) ? $ismw_reg_result['e_postal_areatype']	:'',
																	  '#required' 			=> TRUE,
																	  '#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'e_postal_form_ajax_callback',
																								'progress'			=> '',
																							  ),
																		);
																				
	}
	
	}else{
	
		$form['common_postal']['e_postal_areatype'] 			= array(
																	  '#prefix' 			=> '<div id="e_postal_loc_e_areatype" class="col-md-4"><label class="select">',
																	  '#suffix' 			=> '<i></i></label></div>',
																	  '#title'				=> 'Select Areatype',
																	  '#type' 				=> 'select',
																	  '#options' 			=> array(),
																	  '#required' 			=> TRUE,
																	  '#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'e_postal_form_ajax_callback',
																								'progress'			=> '',
																							  ),
																		);
	}
	
	
	$value_e_postal_areatype 			= isset($form_state['values']['e_postal_areatype']) ? $form_state['values']['e_postal_areatype'] : $ismw_reg_result['e_postal_areatype'];
	
	if($value_e_postal_dist != "" && $value_e_postal_subdivision != "" && $value_e_postal_areatype != ""){
		
		if ( $value_e_postal_areatype == 'B' ){
			$area_type  	= 'Block';
			$area_name		= 'Gram Panchayat';
			$area_code 		= 'V';
		}elseif( $value_e_postal_areatype == 'M'){
			$area_type  	= 'Municipality';
			$area_name		= 'Ward';
			$area_code 		= 'W';
		}elseif( $value_e_postal_areatype == 'C'){
			$area_type  	= 'Corporation';
			$area_name		= 'Ward';
			$area_code 		= 'C';
		}elseif( $value_e_postal_areatype == 'S'){
			$area_type  	= 'SEZ';
			$area_name		= 'Sector';
			$area_code 		= 'S';
		}elseif($value_e_postal_areatype == 'N'){
			$area_type  	= 'Notified Area';
			$area_name		= 'Notified Area';
			$area_code 		= 'N';
		}
		$form['common_postal']['e_postal_areatype_code'] 		= array(
																	  '#prefix' 			=> '<div id="e_postal_area_code" class="col-md-4"><label class="select">',
																	  '#suffix' 			=> '<i></i></label></div>',
																	  '#title'				=> 'Select '.$area_type,
																	  '#type' 				=> 'select',
																	  '#default_value' 		=> !empty($ismw_reg_result['e_postal_areatype_code']) ? $ismw_reg_result['e_postal_areatype_code']	:'',
																	  '#options' 			=> common_custom_user_block_list_new($value_e_postal_subdivision, $value_e_postal_areatype),
																	  '#required' 			=> TRUE,
																	  '#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'e_postal_form_ajax_callback',
																								'progress'			=> '',
																							  )
																	);
		
		
	}else{
		
		$form['common_postal']['e_postal_areatype_code'] 		= array(
																	 '#prefix' 				=> '<div id="e_postal_area_code" class="col-md-4"><label class="select">',
																	 '#suffix' 				=> '<i></i></label></div>',
																	 '#title'				=> 'Select Block/Municipality/Corporation/SEZ/Notified Area',
																	 '#type' 				=> 'select',
																	 '#options' 			=> array(),
																	 '#required' 			=> TRUE,
																	 '#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'e_postal_form_ajax_callback',
																								'progress'			=> '',
																							  )
																  );
		
	}
	
	$value_e_postal_areatype_code		= isset($form_state['values']['e_postal_areatype_code']) ? $form_state['values']['e_postal_areatype_code'] : $ismw_reg_result['e_postal_areatype_code'];
	
																			  
	if($value_e_postal_dist != "" && $value_e_postal_subdivision != "" && $value_e_postal_areatype != "" && $value_e_postal_areatype_code != ""){
		
		if ( $value_e_postal_areatype == 'B' ){
			$area_name		= 'Gram Panchayat';
			$area_code 		= 'V';
		}elseif( $value_e_postal_areatype == 'M'){
			$area_name		= 'Ward';
			$area_code 		= 'W';
		}elseif( $value_e_postal_areatype == 'C'){
			$area_name		= 'Ward';
			$area_code 		= 'C';
		}elseif( $value_e_postal_areatype == 'S'){
			$area_name		= 'Sector';
			$area_code 		= 'S';
		}elseif($value_e_postal_areatype == 'N'){
			$area_name		= 'Notified Area';
			$area_code 		= 'N';
		}
		
		$form['common_postal']['e_postal_vill_ward'] 			= array(
																	  '#prefix' 			=> '<div id="vill_ward_replace" class="col-md-4"><label class="select">',
																	  '#suffix' 			=> '<i></i></label></div>',
																	  '#title'				=> 'Select '.$area_name,
																	  '#type' 				=> 'select',
																	  '#default_value' 		=> !empty($ismw_reg_result['e_postal_vill_ward']) ? $ismw_reg_result['e_postal_vill_ward']	:'',
																	  '#options'			=> common_custom_user_villward_list_new($value_e_postal_areatype_code,$area_code),
																	  '#required' 			=> TRUE,
																	  '#ajax'				=> array(
																									'event' 			=> 'change',
																									'effect' 			=> 'fade',			
																									'callback'			=> 'e_postal_form_ajax_callback',
																									'progress'			=> '',
																								  )
																	);	
	}else{
		
		$form['common_postal']['e_postal_vill_ward'] 			= array(
																	 '#prefix' 				=> '<div id="vill_ward_replace" class="col-md-4"><label class="select">',
																	 '#suffix' 				=> '<i></i></label></div>',
																	 '#title'				=> 'Select Gram Panchayat/Ward',
																	 '#type' 				=> 'select',
																	 '#options' 			=> array(),
																	 '#required' 			=> TRUE,
																	 '#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'e_postal_form_ajax_callback',
																								'progress'			=> '',
																							  )
																  );
	}
	
	$e_postal_vill_ward						= isset($form_state['values']['e_postal_vill_ward']) ? $form_state['values']['e_postal_vill_ward'] : $ismw_reg_result['e_postal_vill_ward'];
	
	if($value_e_postal_dist != "" && $value_e_postal_subdivision != "" && $value_e_postal_areatype != "" && $value_e_postal_areatype_code != "" && $e_postal_vill_ward != ""){
		
		$form['common_postal']['e_postal_ps'] 					= array(
																	  '#prefix' 				=> '<div id="postal_ps_replace" class="col-md-4"><label class="select">',
																	  '#suffix' 				=> '<i></i></label></div>',
																	  '#title'					=> 'Select Police Station',
																	  '#type' 					=> 'select',
																	  '#default_value' 			=> !empty($ismw_reg_result['e_postal_ps']) ? $ismw_reg_result['e_postal_ps']	:'',
																	  '#options' 				=> common_custom_user_ps_list_new($value_e_postal_dist),
																	  '#required' 				=> TRUE,
																	  '#ajax'					=> array(
																									'event' 			=> 'change',
																									'effect' 			=> 'fade',			
																									'callback'			=> 'e_postal_form_ajax_callback',
																									'progress'			=> '',
																								  )
																  );
		
	}else{
		$form['common_postal']['e_postal_ps'] 					= array(
																	  '#prefix' 				=> '<div id="postal_ps_replace" class="col-md-4"><label class="select">',
																	  '#suffix' 				=> '<i></i></label></div>',
																	  '#title'					=> 'Select Police Station',
																	  '#type' 					=> 'select',
																	  '#options' 				=> array(),
																	  '#required' 				=> TRUE,
																	  '#ajax'					=> array(
																									'event' 			=> 'change',
																									'effect' 			=> 'fade',			
																									'callback'			=> 'e_postal_form_ajax_callback',
																									'progress'			=> '',
																								  )
																  );
		
	}
	
	$form['common_postal']['pin_number'] 		   				= array(
																	  '#prefix' 			=> '<div class="col-md-4"><label class="input">',
																	  '#suffix' 			=> '</label></div></div></div>',
																	  '#title'				=> 'PIN Code',
																	  '#type'				=> 'textfield',
																	  '#default_value' 		=> !empty($ismw_reg_result['e_postal_pin_number']) ? $ismw_reg_result['e_postal_pin_number']	:'',
																	  '#attributes' 		=> $readonly_postal_pin,
																	  '#element_validate'	=> array('MISC_PIN_NUMBER'),
																	  '#required' 			=> TRUE
																		  );
			
		
	$form['common_postal']['submit'] 						= array(
																	  '#prefix' 			=> '<div class="form-footer"><div class="col-md-12">',  
																	  '#suffix' 			=> '</div></div></div>',  
																	  '#type' 				=> 'submit',
																	  '#attributes' 		=> array('class' => array('btn btn-primary pull-right')),
																	  '#value' 				=> 'SAVE'
																	   );
																
	return $form;
}



function postal_address_form_submit($form, $form_state){
	
	global $base_root, $base_path, $user;
	
	$user_id								= $user->uid;
	$val 									= $form_state['values'];
	
	$application_id							= encryption_decryption_fun('decrypt', $val['application_id']);
	$act_id 								= encryption_decryption_fun('decrypt', $val['act_id']);
	$final_submit_status					= $val['final_submit_status'];
	$address_line 							= $val['e_postal_address'];
	$district_code 							= $val['e_postal_dist'];
	$subdivision_code 						= $val['e_postal_subdivision'];
	$area_type 								= $val['e_postal_areatype'];
	$area_type_code							= $val['e_postal_areatype_code'];
	$village_code							= $val['e_postal_vill_ward'];
	$police_station							= $val['e_postal_ps'];
	$pin									= $val['pin_number'];
	
	$fieldsRegInfo 							= array( 
												'e_postal_address'							=>	$address_line,
												'e_postal_dist'								=>	$district_code,
												'e_postal_subdivision'						=>  !empty($subdivision_code) ? $subdivision_code : 0,
												'e_postal_areatype'							=>	!empty($area_type) ? $area_type : '',
												'e_postal_areatype_code'					=>	!empty($area_type_code) ? $area_type_code : 0 ,
												'e_postal_vill_ward'						=>	!empty($village_code) ? $village_code : 0,
												'e_postal_ps'								=>	$police_station,
												'e_postal_pin_number'						=>	!empty($pin) ? $pin : 0,
												
												);
										
	
	
	if($application_id!='' && $act_id !='' && $application_id!=0){
		
		if($act_id == 4) {
			db_update('l_interstate_workman_master')->fields($fieldsRegInfo)->condition('user_id', $user_id)->condition('id', $application_id)->condition('act_id', $act_id)->execute();
			drupal_goto('ismw_application/registration-form/'.encryption_decryption_fun('encrypt', $application_id));
		}
		
	}
	
}

function e_postal_form_ajax_callback($form, $form_state){ 
	$commands = array(); 
	$commands[] = ajax_command_replace('#e_postal_subdvsn_replace', drupal_render($form['common_postal']['e_postal_subdivision']));
	$commands[] = ajax_command_replace('#e_postal_loc_e_areatype', drupal_render($form['common_postal']['e_postal_areatype']));
	$commands[] = ajax_command_replace('#e_postal_area_code', drupal_render($form['common_postal']['e_postal_areatype_code']));
	$commands[] = ajax_command_replace('#vill_ward_replace', drupal_render($form['common_postal']['e_postal_vill_ward']));
	$commands[] = ajax_command_replace('#postal_ps_replace', drupal_render($form['common_postal']['e_postal_ps']));
  	return array('#type' => 'ajax', '#commands' => $commands);	
}



	
	