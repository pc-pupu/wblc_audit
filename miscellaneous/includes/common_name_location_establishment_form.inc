<?php 
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');


function name_and_location_form($form, &$form_state, $application_id='' , $en_act_id = ''){
	
	global $base_root, $base_path, $user;
	
	$user_id 			 = $user->uid;
	$readonly_est_pin	 = array('maxlength' => 6);
	$act_id 			 = encryption_decryption_fun('decrypt', $en_act_id);	
		
	$common 			=  db_select('l_common_application_master', 'liwm');			
	$common 			-> fields('liwm', array('est_name','est_loc','est_district','est_subdivision','est_loc_e_areatype','est_name_areatype','est_loc_e_vill_ward','est_ps','est_pin_number','est_type'));
	$common				-> condition('liwm.user_id', $user_id);
	$common_pe_result   = $common->execute()->fetchAssoc();
	$estType			= $common_pe_result['est_type'];
	
	if(!empty($application_id) && $application_id!='' && $application_id!=0 && !empty($act_id) ){ 
		
		$decrypt_application_id		= encryption_decryption_fun('decrypt', $application_id);
		
		$readonly_est_name			= '';
		$readonly_est_address		= '';
		$readonly_est_subdivision	= array('onkeyup' => 'getSpanText()', 'onblur' => 'getSpanText()', 'onchange' => 'getSpanText()');
		
		if($act_id == 4){
			$ismw_reg 				= 	db_select('l_interstate_workman_master', 'liwm');			
			$ismw_reg				->	fields('liwm', array('status','final_submit_status'));
			$ismw_reg				->  condition('liwm.id', $decrypt_application_id);	
			$ismw_reg				->  condition('liwm.act_id', $act_id);	
			$ismw_reg				->	condition('liwm.user_id', $user_id);
			$ismw_reg_result 	 	= 	$ismw_reg->execute()->fetchAssoc();
			
			if(!empty($ismw_reg_result)){
				
				$status 				= $ismw_reg_result['status'];
				$final_submit_status	= $ismw_reg_result['final_submit_status'];
				
				if($status == 'B'){
					
					$readonly_est_name			= array('class' => array('correction_show'));
					$readonly_est_address		= array('class' => array('correction_show'));
					$readonly_est_subdivision	= array('onkeyup' => 'getSpanText()', 'onblur' => 'getSpanText()', 'onchange' => 'getSpanText()', 'class' => array('correction_show'));
					$readonly_est_pin			= array('class' => array('correction_show'), 'maxlength' => 6);
					
					$get_remark = db_select('l_interstate_remark_master', 'lirm');
					$get_remark->fields('lirm', array('remark_field_text'));
					$get_remark->condition('lirm.remark_to', trim($user_id));
					$get_remark->condition('lirm.application_id', trim($decrypt_application_id));
					$get_remark->condition('lirm.is_active', 1);
					$get_remark->orderBy('lirm.id','DESC');
					$get_remark->range(0, 1);
					$get_remark_result = $get_remark->execute();
					$content_5 = $get_remark_result->fetchAssoc();
					$remark_field_text_arr = explode(',', $content_5['remark_field_text']);
			
					
					
				}
			}
			
		}
		if(!empty($remark_field_text_arr)){
			
			foreach ($remark_field_text_arr as $fields){
						
			switch($fields){
			
				case "e_name":
			
						$readonly_est_name 					= array('class'=> array('verified_show') , 'disabled' => 'disabled');
						break;
						
				case "est_type":
						$readonly_est_type 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						break;
					
				case "loc_e_name":
				
						$readonly_est_address 				= array('class'=> array('verified_show'), 'disabled' => 'disabled');
						$readonly_est_subdivision			= array('onkeyup' => 'getSpanText()', 'onblur' => 'getSpanText()', 'onchange' => 'getSpanText()', 'class'=> array('verified_show'), 'disabled' => 'disabled');
						$readonly_est_pin					= array('class' => array('verified_show'), 'maxlength' => 6);
						break;	
			}
			
		}
		}
		
	}
	
	
	$form['common_pe_info']['header']			 			 	= array(
																		'#markup' 			=> t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">NAME AND LOCATION OF THE ESTABLISHMENT</div><div class="panel-body">'),
																		);
																		
	$form['common_pe_info']['application_id'] 					= array(
																	  '#type' 				=> 'hidden',
																	  '#default_value'		=> $application_id,
																	  '#attributes'			=> array('readonly'=>'readonly')
																	);
						
	$form['common_pe_info']['act_id'] 							= array(
																	  '#type' 				=> 'hidden',
																	  '#default_value'		=> $en_act_id,
																	  '#attributes' 		=> array('readonly'=>'readonly')
																	);
	
	$form['common_pe_info']['final_submit_status'] 				= array(
																	  '#type' 				=> 'hidden',
																	  '#default_value'		=> $final_submit_status,
																	  '#attributes' 		=> array('readonly'=>'readonly')
																	);
																	
	$form['common_pe_info']['est_name'] 						= array(
																	  '#prefix' 			=> '<div class="col-md-6"><label class="input">',
																	  '#suffix' 			=> '</label></div>',
																	  '#title' 				=> 'Name of Establishment',
																	  '#type' 				=> 'textfield',
																	  '#default_value' 		=> !empty($common_pe_result['est_name']) ? $common_pe_result['est_name'] : '',
																	  '#required' 			=> TRUE,
																	  '#attributes' 		=> $readonly_est_name, 
																	  '#element_validate'	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE'),
																	);
	$form['common_pe_info']['est_type'] 						= array(
																		'#title' 			=> 'Establishment Type',
																		'#type' 			=> 'select',
																		'#options' 			=> array('micro' => 'Micro', 'small' => 'Small', 'medium' => 'Medium', 'large' => 'Large'),
																		'#required' 		=>  TRUE,
																		'#default_value' 	=> !empty($estType) ? $estType : '',
																		'#attributes'		=> $readonly_est_type,
																		'#prefix' 			=> '<div class="col-md-6"><label class="select">',
																		'#suffix' 			=> '</label></div>'
																	);
																	
	$form['common_pe_info']['est_location'] 					= array(
																	'#prefix' 				=> '<div class="col-md-8 min-height-80"><label class="input">',
																	'#suffix' 				=> '</label></div>',
																	'#title' 				=> 'House No./Village Name/Street/Road',
																	'#type' 				=> 'textfield',
																	'#required' 			=>  TRUE,
																	'#default_value'		=> !empty($common_pe_result['est_loc']) ? $common_pe_result['est_loc'] : '',
																	'#attributes' 			=> $readonly_est_address,
																	'#element_validate'		=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE'),
																	
																);
																
	$form['common_pe_info']['est_district'] 					= array(
																	'#title' 				=> 'Select District',
																	'#type' 				=> 'select',
																	'#options' 				=> common_custom_user_district_list_new(),
																	'#default_value'		=> !empty($common_pe_result['est_district']) ? $common_pe_result['est_district'] : '',
																	'#required' 			=>  TRUE,
																	'#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback' 	=> 'est_registration_form_ajax_callback',
																									'progress' 	=> '',
																							),
																	'#attributes' 			=> $readonly_est_address,
																	'#prefix' 				=> '<div class="col-md-4"><label class="select">',
																	'#suffix' 				=> '</label></div>'
																);
																	
	
																
	
		$value_loc_e_dist 						= isset($form_state['values']['est_district']) ? $form_state['values']['est_district'] : $common_pe_result['est_district'];	
		
		if($value_loc_e_dist != "" || !empty($value_loc_e_dist) || is_numeric($value_loc_e_dist)){
		
						
			$form['common_pe_info']['est_subdivision'] 			= array(
																	'#prefix' 				=> '<div id="estb_subdvsn_replace" class="col-md-4"><label class="select">',
																	'#suffix' 				=> '</label></div>',
																	'#title'				=> 'Select Subdivision',
																	'#type' 				=> 'select',
																	'#default_value'		=> !empty($common_pe_result['est_subdivision']) ? $common_pe_result['est_subdivision'] :'',
																	'#options' 				=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist),
																	'#required' 			=>  TRUE,
																	'#attributes' 			=> $readonly_est_subdivision,
																	'#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback' 	=> 'est_registration_form_ajax_callback',
																									'progress' 	=> '',
																								),		
																	
																	
																);
		}else {
		
			$form['common_pe_info']['est_subdivision'] 			= array(
																	'#prefix' 				=> '<div id="estb_subdvsn_replace" class="col-md-4"><label class="select">',
																	'#suffix' 				=> '</label></div>',
																	'#title'				=> 'Select Subdivision',
																	'#type' 				=> 'select',
																	'#options' 				=> array(),
																	'#required' 			=> TRUE,
																	'#attributes' 			=> $readonly_est_subdivision,
																	'#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback' 	=> 'est_registration_form_ajax_callback',
																									'progress' 	=> '',
																						),		
									);
	}
	
	$value_loc_e_subdivision 	= isset($form_state['values']['est_subdivision']) ? $form_state['values']['est_subdivision'] : $common_pe_result['est_subdivision'];
	
	if($value_loc_e_dist != "" && $value_loc_e_subdivision != ""){
		
		
		$areatypeOptions					= common_custom_user_areatype_list_new($value_loc_e_dist, $value_loc_e_subdivision);
		
		$form['common_pe_info']['est_areatype'] 				= array(
																	'#prefix' 				=> '<div id="estb_loc_e_areatype" class="col-md-4"><label class="select">',
																	'#suffix' 				=> '</label></div>',
																	'#title'				=> 'Select AreaType',
																	'#type' 				=> 'select',
																	'#options' 				=> $areatypeOptions,
																	'#required' 			=>  TRUE,
																	'#default_value'		=> !empty($common_pe_result['est_loc_e_areatype']) ? $common_pe_result['est_loc_e_areatype'] :'',
																	'#attributes' 			=> $readonly_est_address,
																	'#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback' 	=> 'est_registration_form_ajax_callback',
																									'progress'	=> '',
																							),	
																	
																);
	}else{
		
		$form['common_pe_info']['est_areatype'] 				= array(
																	'#title'				=> 'Select Area Type',
																	'#type' 				=> 'select',
																	'#options' 				=> array(),
																	'#required' 			=> TRUE,
																	'#attributes' 			=> $readonly_est_address,
																	'#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback' 	=> 'est_registration_form_ajax_callback',
																									'progress' 	=> '',
																							),	
																	'#prefix' 				=> '<div id="estb_loc_e_areatype" class="col-md-4"><label class="select">',
																	'#suffix' 				=> '</label></div>'
																);
	}
	
	$value_loc_e_areatype 		= isset($form_state['values']['est_areatype']) ? $form_state['values']['est_areatype'] : $common_pe_result['est_loc_e_areatype'];
	
	if ($value_loc_e_dist != "" && $value_loc_e_subdivision != "" && $value_loc_e_areatype != "") {
		
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
		
		$form['common_pe_info']['est_areatype_name'] 			= array(
																	  '#prefix' 			=> '<div id="estb_common_1_replace" class="col-md-4"><label class="select">',
																	  '#suffix' 			=> '</label></div>',
																	  '#title'				=> 'Select '.$areatype_name,
																	  '#type' 				=> 'select',
																	  '#options' 			=> common_custom_user_block_list_new($value_loc_e_subdivision,$value_loc_e_areatype),
																	  '#default_value' 		=> !empty($common_pe_result['est_name_areatype']) ? $common_pe_result['est_name_areatype'] : '',
																	  '#attributes' 		=> $readonly_est_address,
																	  '#required' 			=> TRUE,
																	  '#ajax'				=> array(
																									'event' 			=> 'change',
																									'effect' 			=> 'fade',			
																									'callback'			=> 'est_registration_form_ajax_callback',
																									'progress'			=> '',
																								  )
																	);
		
	}else{
		
		$form['common_pe_info']['est_areatype_name'] 			= array(
																	  '#prefix' 			=> '<div id="estb_common_1_replace" class="col-md-4"><label class="select">',
																	  '#suffix' 			=> '</label></div>',
																	  '#title'				=> 'Select Block/Municipality/Corporation/SEZ/Notified Area',
																	  '#type' 				=> 'select',
																	  '#options' 			=> array(),
																	  '#attributes' 		=> $readonly_est_address,
																	  '#required' 			=> TRUE,
																	  '#ajax'				=> array(
																									'event' 			=> 'change',
																									'effect' 			=> 'fade',			
																									'callback'			=> 'est_registration_form_ajax_callback',
																									'progress'			=> '',
																								  )
																	);
	}
	
	$value_est_areatype_code 	= isset($form_state['values']['est_areatype_name']) ? $form_state['values']['est_areatype_name'] : $common_pe_result['est_name_areatype'];
	
	if($value_loc_e_dist != "" && $value_loc_e_subdivision != "" && $value_loc_e_areatype != ""  && $value_est_areatype_code != "" ){
		
		if ( $value_loc_e_areatype == 'B' ){
			$area_name		= 'Gram Panchayat';
			$area_code 		= 'V';
		}elseif( $value_loc_e_areatype == 'M'){
			$area_name		= 'Ward';
			$area_code 		= 'W';
		}elseif( $value_loc_e_areatype == 'C'){
			$area_name		= 'Ward';
			$area_code 		= 'C';
		}elseif( $value_loc_e_areatype == 'S'){
			$area_name		= 'Sector';
			$area_code 		= 'S';
		}elseif($value_loc_e_areatype == 'N'){
			$area_name		= 'Notified Area';
			$area_code 		= 'N';
		}
		
		$form['common_pe_info']['est_vill_ward'] 				= array(
																	  '#prefix' 			=> '<div id="estb_common_2_replace" class="col-md-4"><label class="select">',
																	  '#suffix' 			=> '</label></div>',
																	  '#title'				=> 'Select '.$area_name,
																	  '#type' 				=> 'select',
																	  '#options'			=> common_custom_user_villward_list_new($value_est_areatype_code,$area_code),
																	  '#required' 			=> TRUE,
																	  '#default_value' 		=> !empty($common_pe_result['est_loc_e_vill_ward']) ? $common_pe_result['est_loc_e_vill_ward'] : '',
																	  '#attributes' 		=> $readonly_est_address,
																	  '#ajax'				=> array(
																									'event' 			=> 'change',
																									'effect' 			=> 'fade',			
																									'callback'			=> 'est_registration_form_ajax_callback',
																									'progress'			=> '',
																								  )
																	);
		
	}else{
		
		$form['common_pe_info']['est_vill_ward'] 				= array(
																	  '#prefix' 			=> '<div id="estb_common_2_replace" class="col-md-4"><label class="select">',
																	  '#suffix' 			=> '</label></div>',
																	  '#title'				=> 'Select Gram Panchayat/Ward/Sector/Notified Area',
																	  '#type' 				=> 'select',
																	  '#options' 			=> array(),
																	  '#required' 			=> TRUE,
																	  '#attributes' 		=> $readonly_est_address,
																	  '#ajax'				=> array(
																									'event' 			=> 'change',
																									'effect' 			=> 'fade',			
																									'callback'			=> 'est_registration_form_ajax_callback',
																									'progress'			=> '',
																								  )
																	);
		
	}
	
	$value_loc_vill_ward 			= 	isset($form_state['values']['est_vill_ward']) ? $form_state['values']['est_vill_ward'] :$common_pe_result['est_loc_e_vill_ward'];
	
	
	if ($value_loc_e_dist != "" && $value_loc_e_subdivision != "" && $value_loc_e_areatype != ""  && $value_est_areatype_code != "" && $value_loc_vill_ward != "" ){
		
		$form['common_pe_info']['est_ps'] 						= array(
																	  '#prefix' 			=> '<div id="estb_ps_replace" class="col-md-4"><label class="select">',
																	  '#suffix' 			=> '</label></div>',
																	  '#title'				=> 'Select Police Station',
																	  '#type' 				=> 'select',
																	  '#options' 			=> common_custom_user_ps_list_new($value_loc_e_dist),
																	  '#default_value' 		=> !empty($common_pe_result['est_ps']) ? $common_pe_result['est_ps']: '',
																	  '#attributes' 		=> $readonly_est_address,
																	  '#required' 			=> TRUE,
																		  
																	); 	
		
	}else{
		
		$form['common_pe_info']['est_ps'] 						= array(
																	  '#prefix' 			=> '<div id="estb_ps_replace" class="col-md-4"><label class="select">',
																	  '#suffix' 			=> '</label></div>',
																	  '#title'				=> 'Select Police Station',
																	  '#type' 				=> 'select',
																	  '#options' 			=> array(),
																	  '#attributes' 		=> $readonly_est_address,
																	  '#required' 			=> TRUE,
																		  
																	); 
	}
	
	
	$form['common_pe_info']['est_pincode'] 					   = array(
																	'#title' 				=> 'Pin Code',
																	'#type' 				=> 'textfield',
																	'#required' 			=>  TRUE,
																	'#default_value' 		=> !empty($common_pe_result['est_pin_number']) ? $common_pe_result['est_pin_number']: '',
																	'#attributes' 			=> $readonly_est_pin,
																	'#element_validate'		=> array('MISC_PIN_NUMBER'),
																	'#prefix' 				=> '<div class="col-md-4"><label class="input">',
																	'#suffix' 				=> '</label></div></div></div>'
																	);
		
		
		$form['common_pe_info']['submit'] 						= array(
																	  '#prefix' 			=> '<div class="form-footer"><div class="col-md-12">',  
																	  '#suffix' 			=> '</div></div></div>',  
																	  '#type' 				=> 'submit',
																	  '#attributes' 		=> array('class' => array('btn btn-primary pull-right')),
																	  '#value' 				=> 'SAVE'
																	   );
																
	return $form;
}



function name_and_location_form_submit($form, $form_state){
	
	global $base_root, $base_path, $user;
	
	$user_id								= $user->uid;
	$val 									= $form_state['values'];
	
	$application_id							= encryption_decryption_fun('decrypt', $val['application_id']);
	$act_id 								= encryption_decryption_fun('decrypt', $val['act_id']);
	$final_submit_status				    = $val['final_submit_status'];
	
	
	
	
	$name 									= $val['est_name'];
	$estType								= $val['est_type'];
	$address_line 							= $val['est_location'];
	$district_code 							= $val['est_district'];
	$subdivision_code 						= $val['est_subdivision'];
	$area_type 								= $val['est_areatype'];
	$area_type_code							= $val['est_areatype_name'];
	$village_code							= $val['est_vill_ward'];
	$police_station							= $val['est_ps'];
	$pin									= $val['est_pincode'];
	
	$fieldsRegInfo 							= array( 
												'est_name'									=>	$name,
												'est_loc'									=>	$address_line,
												'est_district'								=>	$district_code,
												'est_subdivision'							=>	!empty($subdivision_code) ? $subdivision_code : 0 ,
												'est_loc_e_areatype'						=>	!empty($area_type) ? $area_type : '',
												'est_name_areatype'							=>	!empty($area_type_code) ? $area_type_code : 0 ,
												'est_loc_e_vill_ward'						=>	!empty($village_code) ? $village_code : 0,
												'est_ps'									=>	$police_station,
												'est_pin_number'							=>  !empty($pin) ? $pin : 0,
												'est_type'									=>	$estType
												);
												


	

	if($application_id!='' && $act_id !='' && $application_id!=0){
		
		if($act_id == 4) {
			
			db_update('l_common_application_master')->fields($fieldsRegInfo)->condition('user_id', $user_id, '=')->execute();
			db_update('l_interstate_workman_master')->fields(
															array(
																'e_name'							=>	$name,
																'loc_e_name'						=>	$address_line,
																'loc_e_dist'						=>	$district_code,
																'loc_e_subdivision'					=>	!empty($subdivision_code) ? $subdivision_code : 0 ,
																'loc_e_areatype'					=>	!empty($area_type) ? $area_type : '',
																'loc_e_areatype_code'				=>	!empty($area_type_code) ? $area_type_code : 0 ,
																'loc_e_vill_ward'					=>	!empty($village_code) ? $village_code : 0,
																'loc_e_ps'							=>	$police_station,
																'loc_e_pin_number'					=>  !empty($pin) ? $pin : 0,
																'est_type'							=>  !empty($estType) ? $estType : '',
																)
															)
													->condition('user_id', $user_id)
													->condition('id', $application_id)
													->condition('act_id', $act_id)
													->execute();
													
			drupal_goto('ismw_application/registration-form/'.encryption_decryption_fun('encrypt', $application_id));
		}
		
	}else{
		
		db_update('l_common_application_master')->fields($fieldsRegInfo)->condition('user_id', $user_id, '=')->execute();
			
			if($act_id == 4) {
				drupal_goto('ismw_establishment_location');
				
			}
		
	}
	
}



function est_registration_form_ajax_callback($form, $form_state){
	$commands = array();
	$commands[] = ajax_command_replace('#estb_subdvsn_replace', drupal_render($form['common_pe_info']['est_subdivision']));
	$commands[] = ajax_command_replace('#estb_loc_e_areatype', drupal_render($form['common_pe_info']['est_areatype']));
	$commands[] = ajax_command_replace('#estb_common_1_replace', drupal_render($form['common_pe_info']['est_areatype_name']));
	$commands[] = ajax_command_replace('#estb_common_2_replace', drupal_render($form['common_pe_info']['est_vill_ward']));
	$commands[] = ajax_command_replace('#estb_ps_replace', drupal_render($form['common_pe_info']['est_ps']));
	return array('#type' => 'ajax', '#commands' => $commands);
	
}



	
	