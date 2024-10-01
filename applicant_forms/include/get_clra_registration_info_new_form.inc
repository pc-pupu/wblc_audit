<?php
function get_clra_registration_info_new_form($form, &$form_state) {
	
	drupal_add_js(drupal_get_path('module', 'applicant_forms') . '/js/upload_custom.js');
	
	global $base_root,$base_path,$user;
	
	
	$_SESSION['uid'] 	= $user->uid;
	$user_id  = $user->uid;

	$applicant_details	 	= db_select('l_common_application_master', 'lcam');
	$applicant_details->fields('lcam', array('id', 'f_name', 'm_name', 'l_name', 'address', 'dist_code', 'subdivision', 'areatype', 'name_areatype', 'vill_ward', 'ps', 'pin','country','state', 'est_name', 'est_loc', 'est_district', 'est_subdivision', 'est_loc_e_areatype', 'est_name_areatype', 'est_loc_e_vill_ward','est_ps', 'est_pin_number','gender','est_type'));
	$applicant_details->condition('lcam.user_id', $user_id);
	$get_profile_status 	= $applicant_details->execute();
	
	
		
		if( $get_profile_status->rowCount() > 0 ){
			
			$obj 					= 	$get_profile_status->fetchAssoc(); 
			
			$f_name					=	$obj['f_name'];
			$m_name					=	$obj['m_name'];
			$l_name					=	$obj['l_name'];
			$address				=	$obj['address'];
			$country_code			=   $obj['country'];
			$state_code				=   $obj['state'];
			$district_code			=	$obj['dist_code'];
			$subdivision_code		=	$obj['subdivision'];
			$areatype_code			=	$obj['areatype'];
			$name_areatype_code		=	$obj['name_areatype'];
			$vill_ward_code			=	$obj['vill_ward'];
			$ps_code				=	$obj['ps'];
			$pin					=	$obj['pin'];
			
			if($f_name != ""){
				$initial_name		=	$f_name;
			}else{
				$initial_name		=	"";
			}
			
			if($m_name != ""){
				$middle_name		=	$m_name;
			}else{
				$middle_name		=	"";
			}
			
			if($l_name != ""){
				$last_name			=	$l_name;
			}else{
				$last_name			=	"";
			}
			
			
			if($middle_name != ""){			
				$p_full_name			=	$initial_name." ".$middle_name." ".$last_name;
			}else{
				$p_full_name			=	$initial_name." ".$last_name;
			}
			
			if($country_code=='' || $country_code=='0' ){
				$country_code		= 1;
				
			}
			
			if($state_code=='' || $state_code=='0' ){
				$state_code	= 1;
			}
			
			
			$clra_reg 				= 	db_select('l_clra_registration_master', 'liwm');			
			$clra_reg				->	fields('liwm', array('id', 'final_submit_status', 'status', 'amendment_parent_id'));
			$clra_reg				->	condition('liwm.user_id', $user_id);
			$clra_reg				->	range(0, 1);
			$clra_reg				->	orderBy('liwm.id','DESC');
			
			$clra_reg_result 	 	= 	$clra_reg->execute()->fetchAssoc();
			
			$application_id			=   $clra_reg_result['id'];	
			$status					= 	$clra_reg_result['status'];	
			$amendment_parent_id	= 	$clra_reg_result['amendment_parent_id'];
			
			 
			if ( ($application_id != '' && $status=='R' && $amendment_parent_id == '' ) || ($application_id=='' && $status=='' && $amendment_parent_id == '' ) ){
			
			
				$options_dist 	= common_custom_user_district_list_new();
				
				
				$form['clra_reg_master']['header']								 		= array(
																								'#markup' 			=>	 t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">1. Name and location of the Establishment <span id="rlo_location_id" class="form_note"></span></div><div class="panel-body">'),
																								);
				
				$form['clra_reg_master']['clra_act_id'] 		 						= array(
																								'#title'			=>	'Act Id',
																								'#type' 			=> 	'hidden',
																								'#default_value'	=> 	1,
																								'#attributes'		=> 	array('readonly'=>'readonly')
																								);
																										
				$form['clra_reg_master']['e_name'] 										= array(
																							  '#prefix' 			=>  '<div class="col-lg-4 col-md-6"><label class="input">',
																							  '#suffix' 			=>  '</label></div>',
																							  '#title'				=>  'Establishment Name',
																							  '#type' 				=>  'textfield',
																							  '#required' 			=>  TRUE,
																							  '#default_value' 		=>!empty($obj['est_name']) ? $obj['est_name'] : '',
																							  //'#element_validate' =>  array('NEW_CLRA_SPECIAL_CHAR_TEXT_VALIDATE')
																							);
				$form['clra_reg_master']['est_type'] 								= array(
																							'#title' 				=> 'Establishment Type',
																							'#type' 				=> 'select',
																							'#options' 				=> array('micro' => 'Micro', 'small' => 'Small', 'medium' => 'Medium', 'large' => 'Large'),
																							'#required' 			=>  TRUE,
																							'#default_value' 		=> $obj['est_type'],
																							'#attributes' 			=> array('id' => '', 'class'=>array('form-control')),
																							'#prefix' 				=> '<div class="col-lg-4 col-md-6"><label class="select">',
																							'#suffix' 				=> '</label></div>'
																						);
																										
				$form['clra_reg_master']['loc_e_name'] 									= array(
																							  '#prefix' 			=>  '<div class="col-lg-4 col-md-6"><label class="input">',
																							  '#suffix' 			=>  '</label></div>',
																							  '#title'				=>  'House No./Village Name/Street/Road',
																							  '#type' 				=>  'textfield',
																							  '#required' 			=>  TRUE,
																							  '#default_value' 		=>  !empty($obj['est_loc']) ? $obj['est_loc'] : '',
																							  //'#element_validate' 	=> array('NEW_CLRA_SPECIAL_CHAR_TEXT_VALIDATE')
																								);
			
				
				
			
				$form['clra_reg_master']['loc_e_dist'] 									= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select District',
																							  '#type' 				=> 'select',
																							  '#options' 			=> $options_dist,
																							  '#default_value' 		=> !empty($obj['est_district']) ? trim($obj['est_district']):'',
																							  '#required' 			=> TRUE,
																							  '#id'					=> 'loc_e_dist_id',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_1',
																														'progress'			=> '',
																													  )
																							);
						
				$value_loc_e_dist 						= 	isset($form_state['values']['loc_e_dist']) ? $form_state['values']['loc_e_dist'] : trim($obj['est_district']);
				
				if($value_loc_e_dist != "" || !empty($value_loc_e_dist) || is_numeric($value_loc_e_dist)){
					
					
					
					$form['clra_reg_master']['loc_e_subdivision'] 					   	= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6" id="clra_subdvsn_replace"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Subdivision',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist),
																							  '#required' 			=> TRUE,
																							  '#default_value' 		=>!empty($obj['est_subdivision']) ? $obj['est_subdivision'] : '',
																							  '#attributes' 		=> array('onkeyup' => 'getSpanText()', 'onblur' => 'getSpanText()', 'onchange' => 'getSpanText()'),
																							  '#id'					=> 'loc_e_subdivision_id',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_1',
																														'progress'			=> '',
																													  ),
																						);
																											
				}else{
					
					$form['clra_reg_master']['loc_e_subdivision'] 					   	= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6" id="clra_subdvsn_replace"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Subdivision',
																							  '#type' 				=> 'select',
																							  '#options' 			=> array(),
																							  '#attributes' 		=> array('onkeyup' => 'getSpanText()', 'onblur' => 'getSpanText()', 'onchange' => 'getSpanText()'),
																							  '#id'					=> 'loc_e_subdivision_id',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_1',
																														'progress'			=> '',
																													  ),
																						);
				}
				
				$value_loc_e_subdivision 				= isset($form_state['values']['loc_e_subdivision']) ? $form_state['values']['loc_e_subdivision'] : $obj['est_subdivision'];
				
				if($value_loc_e_dist != "" && $value_loc_e_subdivision != ""){
					
					
					$areatypeOptions					= common_custom_user_areatype_list_new($value_loc_e_dist, $value_loc_e_subdivision);
					
					
					$form['clra_reg_master']['loc_e_areatype']  						= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6" id="clra_loc_e_areatype"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Block/Municipality/Corporation/SEZ/NA',
																							  '#type' 				=> 'select',
																							  '#options' 			=> $areatypeOptions,
																							  '#default_value'		=> !empty($obj['est_loc_e_areatype']) ? $obj['est_loc_e_areatype'] : '',
																							  '#required' 			=> TRUE,
																							  '#id'					=> 'loc_e_areatype_id',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_1',
																														'progress'			=> '',
																													  ),
																								);
																													
				}else{
					
					$form['clra_reg_master']['loc_e_areatype'] 						   	= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6" id="clra_loc_e_areatype"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Block / Municipality / Corporation / SEZ / Notified Area',
																							  '#type' 				=> 'select',
																							  '#options' 			=> array(),
																							  '#required' 			=> TRUE,
																							  '#id'					=> 'loc_e_areatype_id',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_1',
																														'progress'			=> '',
																													  ),
																								);
				}
				
				$value_loc_e_areatype 							= isset($form_state['values']['loc_e_areatype']) ? $form_state['values']['loc_e_areatype'] : $obj['est_loc_e_areatype'];
				
				
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
					
					$form['clra_reg_master']['name_areatype'] 							= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6" id="clra_common_1_replace"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select '.$areatype_name,
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_loc_e_subdivision, $value_loc_e_areatype),
																							  '#required' 			=> TRUE,
																							  '#default_value'		=> !empty($obj['est_name_areatype']) ? $obj['est_name_areatype'] : '',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_1',
																														'progress'			=> '',
																													  )
																					 );	
					
					
				}else{
					
					$form['clra_reg_master']['name_areatype'] 							= array(
																							 '#prefix' 				=> '<div class="col-lg-4 col-md-6" id="clra_common_1_replace"><label class="select">',
																							 '#suffix' 				=> '</label></div>',
																							 '#title'				=> 'Select Block',
																							 '#type' 				=> 'select',
																							 '#options' 			=> array(),
																							 '#required' 			=> TRUE,
																							 '#id'					=> 'common_selection_1_id',
																							 '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_1',
																														'progress'			=> '',
																													  )
																						  );
					
				}
				
				$value_loc_areatype_code 		= isset($form_state['values']['name_areatype']) ? $form_state['values']['name_areatype'] : $obj['est_name_areatype'];
				
				
				if($value_loc_e_dist != "" && $value_loc_e_subdivision != "" && $value_loc_e_areatype != ""  && $value_loc_areatype_code != "" ){
					
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
			
					
					$form['clra_reg_master']['loc_e_vill_ward'] 						= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6" id="clra_common_2_replace"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select '.$area_name,
																							  '#type' 				=> 'select',
																							  '#options'			=> common_custom_user_villward_list_new($value_loc_areatype_code,$area_code),
																							  '#default_value'		=> !empty($obj['est_loc_e_vill_ward']) ? $obj['est_loc_e_vill_ward'] : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_ajax_callback_new_1',
																															'progress'			=> '',
																														  )
																							);
					
				}else{
					
					$form['clra_reg_master']['loc_e_vill_ward'] 						= array(
																							 '#prefix' 				=> '<div class="col-lg-4 col-md-6" id="clra_common_2_replace"><label class="select">',
																							 '#suffix' 				=> '</label></div>',
																							 '#title'				=> 'Select Gram Panchayat / Ward / Sector / Notified Area',
																							 '#type' 				=> 'select',
																							 '#options' 			=> array(),
																							 '#required' 			=> TRUE,
																							 '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_1',
																														'progress'			=> '',
																													  )
																						  );
				}
				
				
				
				$value_loc_vill_ward 	= isset($form_state['values']['loc_e_vill_ward']) ? $form_state['values']['loc_e_vill_ward'] : $obj['est_loc_e_vill_ward'];
				
				if ($value_loc_e_dist != "" && $value_loc_e_subdivision != "" && $value_loc_e_areatype != ""  && $value_loc_areatype_code != "" && $value_loc_vill_ward != "" ){
					
					$form['clra_reg_master']['loc_e_ps'] 								= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6" id="clra_ps_replace"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Police Station',
																							  '#type' 				=> 'select',
																							  '#id'					=> 'loc_e_ps_id',
																							  '#options' 			=> common_custom_user_ps_list_new($value_loc_e_dist),
																							  '#default_value'		=> !empty($obj['est_ps']) ? $obj['est_ps'] : '',
																							  '#required' 			=> TRUE
																						  );
					
				}else{
					
					$form['clra_reg_master']['loc_e_ps'] 								= array(
																							'#prefix' 				=> '<div class="col-lg-4 col-md-6" id="clra_ps_replace"><label class="select">',
																							'#suffix' 				=> '</label></div>',
																							'#title'				=> 'Select Police Station',
																							'#type' 				=> 'select',
																							'#options' 				=> array(),
																							'#required' 			=> TRUE,
																							'#id'					=> 'loc_e_ps_id',
																							'#ajax'					=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_1',
																														'progress'			=> '',
																													  )
																					  );
					
				}
				$form['clra_reg_master']['loc_e_pin_number'] 							= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Pin Code',
																							  '#type' 				=> 'textfield',
																							  '#attributes' 		=> array('maxlength' => 6),
																							  '#default_value'		=> !empty($obj['est_pin_number']) ? $obj['est_pin_number'] : '',
																							  '#required' 			=> TRUE,
																							  '#element_validate' 	=> array('REGISTRATION_TEXTBOX_NUMERIC_VALIDATE'),
																						  );

				$form['clra_reg_master']['e_postal_address'] 							= array(
																							  '#prefix' 			=> '</div></div></div><div class="panel panel-default panel-form"><div class="panel-heading">2. Registered Office address of the Establishment</div><div class="panel-body"><div class="col-lg-4 col-md-6"><label class="textarea">',
																							  '#suffix' 			=>  '</label></div>',
																							  '#title'				=>  'House No./Village Name/Street/Road Line 1:',
																							  '#type' 				=>  'textarea',
																							  '#default_value' 		=>  !empty($obj['est_loc']) ? $obj['est_loc'] : '',
																							  '#required' 			=>  TRUE,
																							  '#rows'				=> t('5'),
																							  '#element_validate' 	=> array('NEW_CLRA_SPECIAL_CHAR_TEXT_VALIDATE')
																							);																		
																						
			
				
				$form['clra_reg_master']['e_postal_dist'] 								= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select District',
																							  '#type' 				=> 'select',
																							  '#options' 			=> $options_dist,
																							  '#default_value' 	=> !empty($obj['est_district']) ? trim($obj['est_district']):'',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_2',
																														'progress'			=> '',
																													  )
																							);
				
				$value_loc_e_dist_1 						= 	isset($form_state['values']['e_postal_dist']) ? $form_state['values']['e_postal_dist'] : trim($obj['est_district']);
				
				if($value_loc_e_dist_1 != "" || !empty($value_loc_e_dist_1) || is_numeric($value_loc_e_dist_1)){
					
					$form['clra_reg_master']['e_postal_subdivision'] 					= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6" id="clra_subdvsn_replace_1"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Subdivision',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist_1),
																							  '#default_value' 		=>!empty($obj['est_subdivision']) ? $obj['est_subdivision'] : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_2',
																														'progress'			=> '',
																													  ),
																						);
				}else{
					
					$form['clra_reg_master']['e_postal_subdivision'] 					= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6" id="clra_subdvsn_replace_1"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Subdivision',
																							  '#type' 				=> 'select',
																							  '#options' 			=> array(),
																							  '#id'					=> 'loc_e_subdivision_id_1',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_2',
																														'progress'			=> '',
																													  ),
																						);
				}
				
				$value_loc_e_subdivision_1 				= isset($form_state['values']['e_postal_subdivision']) ? $form_state['values']['e_postal_subdivision'] : $obj['est_subdivision'];
				
				if($value_loc_e_dist_1 != "" && $value_loc_e_subdivision_1 != ""){
					
					$areatypeOptions				   = common_custom_user_areatype_list_new($value_loc_e_dist_1, $value_loc_e_subdivision_1);
					
					$form['clra_reg_master']['e_postal_areatype'] 						= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6" id="clra_loc_e_areatype_1"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Block / Municipality / Corporation / SEZ / Notified Area',
																							  '#type' 				=> 'select',
																							  '#options' 			=> $areatypeOptions,
																							  '#default_value'		=> !empty($obj['est_loc_e_areatype']) ? $obj['est_loc_e_areatype'] : '',
																							  '#required' 			=> TRUE,
																							  '#id'					=> 'loc_e_areatype_id_1',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_2',
																														'progress'			=> '',
																													  ),
																								);
				}else{
					
					$form['clra_reg_master']['e_postal_areatype'] 						= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6" id="clra_loc_e_areatype_1"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Block / Municipality / Corporation / SEZ / Notified Area',
																							  '#type' 				=> 'select',
																							  '#options' 			=> array(),
																							  '#required' 			=> TRUE,
																							  '#id'					=> 'loc_e_areatype_id_1',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_2',
																														'progress'			=> '',
																													  ),
																								);
				}
				
				$value_loc_e_areatype_1 								= isset($form_state['values']['e_postal_areatype']) ? $form_state['values']['e_postal_areatype'] : $obj['est_loc_e_areatype'];
				
				if($value_loc_e_dist_1 != "" && $value_loc_e_subdivision_1 != "" && $value_loc_e_areatype_1 != ""){
					
					if ( $value_loc_e_areatype_1 == 'B' ){
						$areatype_name	= 'Block';
					}elseif( $value_loc_e_areatype_1 == 'M'){
						$areatype_name	= 'Municipality';
					}elseif( $value_loc_e_areatype_1 == 'C'){
						$areatype_name	= 'Corporation';
					}elseif( $value_loc_e_areatype_1 == 'S'){
						$areatype_name	= 'SEZ';
					}elseif($value_loc_e_areatype_1 == 'N'){
						$areatype_name	= 'Notified Area';
					}
					
					$form['clra_reg_master']['e_postal_name_areatype'] 					= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6" id="clra_common_1_replace_1"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select '.$areatype_name,
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_loc_e_subdivision_1, $value_loc_e_areatype_1),
																							  '#default_value'		=> !empty($obj['est_name_areatype']) ? $obj['est_name_areatype'] : '',
																							  '#required' 			=> TRUE,
																							  '#id'					=> 'common_selection_1_id_1',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_2',
																														'progress'			=> '',
																													  )
																							);
					
				}else{
					
					$form['clra_reg_master']['e_postal_name_areatype'] 					= array(
																							 '#prefix' 				=> '<div class="col-lg-4 col-md-6" id="clra_common_1_replace_1"><label class="select">',
																							 '#suffix' 				=> '</label></div>',
																							 '#title'				=> 'Select Block',
																							 '#type' 				=> 'select',
																							 '#options' 			=> array(),
																							 '#required' 			=> TRUE,
																							 '#id'					=> 'common_selection_1_id_1',
																							 '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_2',
																														'progress'			=> '',
																													  )
																						  );
				}
				
				$value_common_selection_1_1 	= isset($form_state['values']['e_postal_name_areatype']) ? $form_state['values']['e_postal_name_areatype'] : $obj['est_name_areatype'];
				
				if($value_loc_e_dist_1 != "" && $value_loc_e_subdivision_1 != "" && $value_loc_e_areatype_1 != "" && $value_common_selection_1_1 != ""){
					
					if ( $value_loc_e_areatype_1 == 'B' ){
						$area_name		= 'Gram Panchayat';
						$area_code 		= 'V';
					}elseif( $value_loc_e_areatype_1 == 'M'){
						$area_name		= 'Ward';
						$area_code 		= 'W';
					}elseif( $value_loc_e_areatype_1 == 'C'){
						$area_name		= 'Ward';
						$area_code 		= 'C';
					}elseif( $value_loc_e_areatype_1 == 'S'){
						$area_name		= 'Sector';
						$area_code 		= 'S';
					}elseif($value_loc_e_areatype_1 == 'N'){
						$area_name		= 'Notified Area';
						$area_code 		= 'N';
					}
					
					$form['clra_reg_master']['e_postal_vill_ward'] 						= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6" id="clra_common_2_replace_1"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select '.$area_name,
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_villward_list_new($value_common_selection_1_1,$area_code),
																							  '#default_value'		=> !empty($obj['est_loc_e_vill_ward']) ? $obj['est_loc_e_vill_ward'] : '',
																							  '#required' 			=> TRUE,
																							  '#id'					=> 'common_selection_2_id_1',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_2',
																														'progress'			=> '',
																													  )
																							); 
				}else{
				
					$form['clra_reg_master']['e_postal_vill_ward'] 						= array(
																							 '#prefix' 				=> '<div class="col-lg-4 col-md-6" id="clra_common_2_replace_1"><label class="select">',
																							 '#suffix' 				=> '</label></div>',
																							 '#title'				=> 'Select Gram Panchayat / Ward / Sector / Notified Area',
																							 '#type' 				=> 'select',
																							 '#options' 			=> array(),
																							 '#required' 			=> TRUE,
																							 '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_2',
																														'progress'			=> '',
																													  )
																						  );
				}
				
				$value_common_selection_2_1 						= isset($form_state['values']['e_postal_vill_ward']) ? $form_state['values']['e_postal_vill_ward'] : $obj['est_loc_e_vill_ward'];
				
				if ($value_loc_e_dist_1 != "" && $value_loc_e_subdivision_1 != "" && $value_loc_e_areatype_1 != "" && $value_common_selection_1_1 != "" && $value_common_selection_2_1 != "" ){
					
					$form['clra_reg_master']['e_postal_ps'] 							= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6" id="clra_ps_replace_1"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Police Station',
																							  '#type' 				=> 'select',
																							  '#id'					=> 'loc_e_ps_id_1',
																							  '#options' 			=> common_custom_user_ps_list_new($value_loc_e_dist_1),
																							  '#default_value'		=> !empty($obj['est_ps']) ? $obj['est_ps'] : '',
																							  '#required' 			=> TRUE
																						  );
				}else{
				
					$form['clra_reg_master']['e_postal_ps'] 							= array(
																							'#prefix' 				=> '<div class="col-lg-4 col-md-6" id="clra_ps_replace_1"><label class="select">',
																							'#suffix' 				=> '</label></div>',
																							'#title'				=> 'Select Police Station',
																							'#type' 				=> 'select',
																							'#options' 				=> array(),
																							'#required' 			=> TRUE,
																							'#id'					=> 'loc_e_ps_id_1',
																							'#ajax'					=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_2',
																														'progress'			=> '',
																													  )
																					  );
				}
				
				
				$form['clra_reg_master']['e_postal_pin_number'] 						= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																							  '#suffix' 			=> '</label></div></div></div>',
																							  '#title'				=> 'Pin Code',
																							  '#type' 				=> 'textfield',
																							  '#attributes' 		=> array('maxlength' => 6),
																							  '#default_value'		=> !empty($obj['est_pin_number']) ? $obj['est_pin_number'] : '',
																							  '#required' 			=> TRUE,
																							  '#element_validate' 	=> array('REGISTRATION_TEXTBOX_NUMERIC_VALIDATE'),	
																						  );																							
																											
				$form['clra_reg_master']['full_name_principal_emp'] 					= array(
																							  '#prefix' 			=> '<div class="panel panel-default panel-form"><div class="panel-heading">3. Full Name and address of the Principal Employer</div><div class="panel-body"><div class="col-md-4"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Principal Employer Name',
																							  '#type' 				=> 'textfield',
																							  '#default_value' 		=> $p_full_name,
																							  '#required' 			=> TRUE,
																							  '#element_validate' 	=> array('REGISTRATION_TEXTBOX_TEXT_VALIDATE')
																							);
				$form['clra_reg_master']['gender_pe'] 									= array(
																							  '#prefix'				=> '<div class="col-md-4"><div class="inline-group">',
																							  '#suffix'				=>'</div></div>',
																							  '#type' 				=> 'radios',
																							  '#title' 				=> t('Gender'),
																							  '#options' 			=> array('M'=>t('Male'), 'F'=>t('Female'), 'O'=>t('Other')),
																							  '#default_value' 		=> isset($obj['gender']) ? $obj['gender'] : 'M',
																							  '#required' 			=> TRUE
																							);
				
				
				$options_country 	= array(1 => 'India', 2 => 'Others'); 
																											
				$form['clra_reg_master']['emp_country'] 								= array(
																								'#prefix' 			=> '<div class="col-md-4"><label class="select">',
																								'#suffix' 			=> '</label></div>',
																								'#title'			=> 'Select Country',
																								'#type' 			=> 'select',
																								'#options' 			=> $options_country,
																								'#default_value'	=> $country_code,
																								'#required' 		=> TRUE,
																								'#ajax'				=> array(
																																'event' 		=> 'change',
																																'effect' 		=> 'fade',			
																																'callback'		=> 'get_ajax_country_emp',
																																'progress'		=>	'',
																														)
																						);
																										
																										
				$value_emp_country 									= isset($form_state['values']['emp_country']) ? $form_state['values']['emp_country'] : $country_code;
				
				
				
				$form['clra_reg_master']['address_principal_emp']	= array(
																			'#prefix' 			=> '<div id="add_line_replace">',
																			'#suffix' 			=> '</div>',
																			);
				$form['clra_reg_master']['emp_state']				= array(
																			'#prefix' 			=> '<div id="clra_emp_state_replace">',
																			'#suffix' 			=> '</div>',
																			);															
				$form['clra_reg_master']['emp_dist']				= array(
																			'#prefix' 			=> '<div id="clra_emp_district_replace">',
																			'#suffix' 			=> '</div>',
																			);	
																			
				$form['clra_reg_master']['loc_emp_subdv']			= array(
																			'#prefix' 			=> '<div id="clra_subdvsn_replace_2">',
																			'#suffix' 			=> '</div>',
																			);	
																			
				$form['clra_reg_master']['loc_emp_areatype']		= array(
																			'#prefix' 			=> '<div id="clra_loc_e_areatype_2">',
																			'#suffix' 			=> '</div>',
																			);
																			
				$form['clra_reg_master']['emp_name_areatype']		= array(
																			'#prefix' 			=> '<div id="clra_common_1_replace_2">',
																			'#suffix' 			=> '</div>',
																			);
																			
				$form['clra_reg_master']['loc_emp_vill_ward']		= array(
																			'#prefix' 			=> '<div id="clra_common_2_replace_2">',
																			'#suffix' 			=> '</div>',
																			);
																			
				$form['clra_reg_master']['loc_emp_ps']				= array(
																			'#prefix' 			=> '<div id="clra_ps_replace_2">',
																			'#suffix' 			=> '</div>',
																			);	
																			
				$form['clra_reg_master']['loc_emp_pin_number']		= array(
																			'#prefix' 			=> '<div id="clra_pin_replace_2">',
																			'#suffix' 			=> '</div>',
																			);	
				/*** If Country == "INDIA" ***/
				
				
				if( (!empty($value_emp_country) && $value_emp_country != '') && $value_emp_country ==  1) {
					
						$db_state_value								= db_query("SELECT COUNT(id) FROM {state_master} WHERE country_id = '".$value_emp_country."'")->fetchObject();
						 
					
					
					
						$form['clra_reg_master']['address_principal_emp'] 				= array(
																							  '#prefix' 			=> '<div id="add_line_replace" class="col-md-4 min-height-80"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'House No./Village Name/Street/Road',
																							  '#type' 				=> 'textfield',
																							  '#default_value' 		=>!empty($address) ? $address : '',
																							  '#required' 			=> TRUE,
																							  '#element_validate' 	=> array('NEW_CLRA_SPECIAL_CHAR_TEXT_VALIDATE')
																							);
																												
						$form['clra_reg_master']['emp_state']							= array(
																								'#prefix' 			=> '<div id="clra_emp_state_replace" class="col-lg-4 col-md-6"><label class="select">',
																								'#suffix' 			=> '</label></div>',
																								'#title'			=> t('Select State'),
																								'#type' 			=> 'select',
																								'#options' 			=> get_all_state($value_emp_country),
																								'#default_value'	=> $state_code,
																								'#required' 		=> TRUE,
																								'#ajax'				=> array(
																															'event'		=> 'change',
																															'effect' 	=> 'fade',
																															'callback'	=> 'get_ajax_callback_new_3',
																															'progress'	=>	'',
																															)
																								);	
																											
						$value_emp_state							= isset($form_state['values']['emp_state'])? $form_state['values']['emp_state'] : $state_code;
						
						
						if($value_emp_state != $state_code ){
							
							$district_code 			= '';
							$subdivision_code 		= '';
							$areatype_code			= '';
							$name_areatype_code		= '';
							$vill_ward_code			= '';
							$ps_code				= '';
							$pin					= '';
							
						}
						
						
						
						if(($value_emp_country != '' && (!empty($value_emp_state) || $value_emp_state != '' )  )){
							
							$db_district_value						= db_query("SELECT COUNT(id) FROM {district_master} WHERE state_id = '".$value_emp_state."'")->fetchObject();
							
							if(!empty($db_district_value) && ($db_district_value->count>0) ) {
							
									$form['clra_reg_master']['emp_dist']				= array(
																								'#prefix' 			=> '<div id="clra_emp_district_replace" class="col-lg-4 col-md-6"><label class="select">',
																								'#suffix' 			=> '</label></div>',
																								'#title'			=> t('Select District'),
																								'#type' 			=> 'select',
																								'#options' 			=> get_district_by_id($value_emp_state),
																								'#default_value' 	=> !empty($district_code) ? $district_code : '',
																								'#required' 		=> TRUE,
																								'#ajax'				=> array(
																															'event'		=> 'change',
																															'effect' 	=> 'fade',
																															'callback'	=> 'get_ajax_callback_new_3',
																															'progress'	=>	'',
																														)
																								);	
																											
							$value_loc_e_dist_2						= isset($form_state['values']['emp_dist']) ? $form_state['values']['emp_dist'] : $district_code;
							
							if($value_loc_e_dist_2 != $district_code ){
							
								$subdivision_code 		= '';
								$areatype_code			= '';
								$name_areatype_code		= '';
								$vill_ward_code			= '';
								$ps_code				= '';
								$pin					= '';
							
							}
							
							
							}
							
							
							
						}
					
						if($value_loc_e_dist_2 != ''  && !empty($value_emp_state)){
	
							$db_subdivision_value					= db_query("SELECT COUNT(id) as subdv_total FROM {sub_division} WHERE district_code = '".$value_loc_e_dist_2."'")->fetchObject();
						
							if(!empty($db_subdivision_value) && $db_subdivision_value != '' && $db_subdivision_value->subdv_total > 0) {
								
									$form['clra_reg_master']['loc_emp_subdv'] 			= array(
																							  '#prefix' 			=> '<div id="clra_subdvsn_replace_2" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Subdivision',
																							  '#type' 				=> 'select',
																							  '#default_value' 		=> !empty($subdivision_code) ? $subdivision_code : '',
																							  '#options' 			=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist_2),
																							  '#id'					=> 'loc_e_subdivision_id_2',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_3',
																														'progress'			=> '',
																													  ),
																							);	
																												
									$value_loc_e_subdivision_2						= isset($form_state['values']['loc_emp_subdv']) ? $form_state['values']['loc_emp_subdv'] : $subdivision_code;
									
									if($value_loc_e_subdivision_2 != $subdivision_code ){
									
										$areatype_code			= '';
										$name_areatype_code		= '';
										$vill_ward_code			= '';
										$ps_code				= '';
										$pin					= '';
									
									}
									
							}
						
						}
						
						
						if($value_loc_e_dist_2 != "" && !empty($value_emp_state) && $value_loc_e_subdivision_2 != ""){
							
							$db_block_mun_value								= db_query("SELECT COUNT(id) as areatype_total FROM {block_mun_master} WHERE distict_code = '".$value_loc_e_dist_2."' and sub_division_code  = '".$value_loc_e_subdivision_2."'")->fetchObject();
					
							if(!empty($db_block_mun_value) &&  $db_block_mun_value != '' && $db_block_mun_value->areatype_total > 0){
							
								$areatypeOptions						    = common_custom_user_areatype_list_new($value_loc_e_dist_2, $value_loc_e_subdivision_2);
								
								if(!empty($areatypeOptions)){
									
									$form['clra_reg_master']['loc_emp_areatype']  		= array(
																							  '#prefix' 			=> '<div id="clra_loc_e_areatype_2" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Block / Municipality / Corporation / SEZ / Notified Area',
																							  '#type' 				=> 'select',
																							  '#options' 			=> $areatypeOptions,
																							  '#required' 			=> TRUE,
																							  '#default_value' 		=> !empty($areatype_code) ? $areatype_code : '',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_3',
																														'progress'			=> '',
																													  ),
																								);
																											
									$value_loc_e_areatype_2 				= isset($form_state['values']['loc_emp_areatype']) ? $form_state['values']['loc_emp_areatype'] : $areatype_code;
								}
								
							}
							
							
							
							if($value_loc_e_subdivision_2 != $subdivision_code && $value_loc_e_areatype_2 != $areatype_code){
								
								$name_areatype_code		= '';
								$vill_ward_code			= '';
	
								$ps_code				= '';
								$pin					= '';
							
							}
							
						}
						
						
						
						if ($value_loc_e_dist_2 != "" && $value_loc_e_subdivision_2 != "" && $value_loc_e_areatype_2 != "") {
							
							if ( $value_loc_e_areatype_2 == 'B' ){
								$areatype_name	= 'Block';
							}elseif( $value_loc_e_areatype_2 == 'M'){
								$areatype_name	= 'Municipality';
							}elseif( $value_loc_e_areatype_2 == 'C'){
								$areatype_name	= 'Corporation';
							}elseif( $value_loc_e_areatype_2 == 'S'){
								$areatype_name	= 'SEZ';
							}elseif($value_loc_e_areatype_2 == 'N'){
								$areatype_name	= 'Notified Area';
							}
							
							
							$form['clra_reg_master']['emp_name_areatype'] 				= array(
																							  '#prefix' 			=> '<div id="clra_common_1_replace_2" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select '.$areatype_name,
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_loc_e_subdivision_2, $value_loc_e_areatype_2),
																							  '#required' 			=> TRUE,
																							  '#default_value' 		=> !empty($name_areatype_code) ? $name_areatype_code : '',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_3',
																														'progress'			=> '',
																													  )
																					 );
																								 
																								 
							$value_common_selection_1_2 						= isset($form_state['values']['emp_name_areatype']) ? $form_state['values']['emp_name_areatype'] : $name_areatype_code;
							
							if($value_common_selection_1_2 != $name_areatype_code){
								
								$vill_ward_code			= '';
								$ps_code				= '';
								$pin					= '';
							
							}
							
							
						}
						
						
						
						if($value_loc_e_dist_2 != "" && $value_loc_e_subdivision_2 != "" && $value_loc_e_areatype_2 != ""  && $value_common_selection_1_2 != "" && !empty($value_emp_state) ){
							
							if ( $value_loc_e_areatype_2 == 'B' ){
								$area_name		= 'Gram Panchayat';
								$area_code 		= 'V';
							}elseif( $value_loc_e_areatype_2 == 'M'){
								$area_name		= 'Ward';
								$area_code 		= 'W';
							}elseif( $value_loc_e_areatype_2 == 'C'){
								$area_name		= 'Ward';
								$area_code 		= 'C';
							}elseif( $value_loc_e_areatype_2 == 'S'){
								$area_name		= 'Sector';
								$area_code 		= 'S';
							}elseif($value_loc_e_areatype_2 == 'N'){
								$area_name		= 'Notified Area';
								$area_code 		= 'N';
							}
							
						
							$form['clra_reg_master']['loc_emp_vill_ward'] 				= array(
																							  '#prefix' 			=> '<div id="clra_common_2_replace_2" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select '.$area_name,
																							  '#type' 				=> 'select',
																							  '#options'			=> common_custom_user_villward_list_new($value_common_selection_1_2,$area_code),
																							  '#required' 			=> TRUE,
																							  '#default_value' 		=> !empty($vill_ward_code) ? $vill_ward_code : '',
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_ajax_callback_new_3',
																															'progress'			=> '',
																														  )
																							);	
																										
							$value_common_selection_2_2 					= isset($form_state['values']['loc_emp_vill_ward']) ? $form_state['values']['loc_emp_vill_ward'] : $vill_ward_code;	
							
							if($value_common_selection_2_2 != $vill_ward_code){
								
								$ps_code				= '';
								$pin					= '';
								
							}
							
						}
						
						if (!empty($value_emp_state) && $value_loc_e_dist_2 != "" && $value_loc_e_subdivision_2 != "" && $value_loc_e_areatype_2 && $value_common_selection_1_2 != "" && $value_common_selection_2_2 != "" ){
							$form['clra_reg_master']['loc_emp_ps'] 						= array(
																							  '#prefix' 			=> '<div id="clra_ps_replace_2" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Police Station',
																							  '#type' 				=> 'select',
																							  '#id'					=> 'loc_e_ps_id_2',
																							  '#options' 			=> common_custom_user_ps_list_new($value_loc_e_dist_2),
																							  '#required' 			=> TRUE,
																							  '#default_value' 		=> !empty($ps_code) ? $ps_code : '',
																						  );
																								  
																								  
							$form['clra_reg_master']['loc_emp_pin_number'] 				= array(
																							  '#prefix' 			=> '<div id="clra_pin_replace_2" class="col-lg-4 col-md-6"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Pin Code',
																							  '#type' 				=> 'textfield',
																							  '#default_value' 		=>!empty($pin) ? $pin : '',
																							  '#attributes' 		=> array('maxlength' => 6),
																							  '#required' 			=> TRUE
																						  );	
							
						}
						
				}else{
					
					
					
					$form['clra_reg_master']['address_principal_emp'] 					= array(
																							  '#prefix' 			=> '<div class="col-md-12" id="add_line_replace"><label class="textarea">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Address [ If other Country/State please provide detail address ]',
																							  '#rows'				=> t('2'),
																							  '#cols'				=> t('146'),	
																							  '#type' 				=> 'textarea',
																							  '#attributes' 		=> array('placeholder' => 'Street Name/Landmark/Pin Code'),
																							  '#default_value' 		=>!empty($address) ? $address : '',
																							  '#required' 			=> TRUE,
																							//  '#element_validate' 	=> array('NEW_CLRA_SPECIAL_CHAR_TEXT_VALIDATE')
																							);
																									
																									
					$form['clra_reg_master']['emp_dist']								= array(
																								'#prefix' 				=> '<div id="clra_emp_district_replace">',
																								'#suffix' 				=> '</div>',
																								);	
																									
					$form['clra_reg_master']['loc_emp_subdv']							= array(
																								'#prefix' 				=> '<div id="clra_subdvsn_replace_2">',
																								'#suffix' 				=> '</div>',
																								);	
																									
					$form['clra_reg_master']['loc_emp_areatype']						= array(
																								'#prefix' 				=> '<div id="clra_loc_e_areatype_2">',
																								'#suffix' 				=> '</div>',
																								);
																									
					$form['clra_reg_master']['emp_name_areatype']						= array(
																								'#prefix' 				=> '<div id="clra_common_1_replace_2">',
																								'#suffix' 				=> '</div>',
																								);
																									
					$form['clra_reg_master']['loc_emp_vill_ward']						= array(
																								'#prefix' 				=> '<div id="clra_common_2_replace_2">',
																								'#suffix' 				=> '</div>',
																								);
																			
					$form['clra_reg_master']['loc_emp_ps']								= array(
																								'#prefix' 				=> '<div id="clra_ps_replace_2">',
																								'#suffix' 				=> '</div>',
																								);	
																				
					$form['clra_reg_master']['loc_emp_pin_number']						= array(
																								'#prefix' 				=> '<div id="clra_pin_replace_2">',
																								'#suffix' 				=> '</div>',
																								);	
																											
				}
				
				
				$form['clra_reg_master']['full_name_manager'] 							= array(
																							  '#prefix' 			=> '</div></div><div class="panel panel-default panel-form"><div class="panel-heading">4. Full name and address of the Manager or Person responsible for the supervision  and control of the Establishment</div><div class="panel-body"><div class="col-md-6"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Name',
																							  '#type' 				=> 'textfield',
																							  '#required' 			=> TRUE,
																							  '#default_value' 		=> $p_full_name,
																							  '#element_validate'	=> array('REGISTRATION_TEXTBOX_TEXT_VALIDATE')
																								);
																												
																												
																											
				$form['clra_reg_master']['manager_country'] 							= array(
																								'#prefix' 			=> '<div class="col-md-6"><label class="select">',
																								'#suffix' 			=> '</label></div>',
																								'#title'			=> 'Select Country',
																								'#type' 			=> 'select',
																								'#options' 			=> $options_country,
																								'#default_value'	=> $country_code,
																								'#required' 		=> TRUE,
																								'#ajax'				=> array(
																																'event' 		=> 'change',
																																'effect' 		=> 'fade',			
																																'callback'		=> 'get_ajax_country_manager',
																																'progress'		=>	'',
																														)
																						);
																										
				$value_manager_country 								= isset($form_state['values']['manager_country']) ? $form_state['values']['manager_country'] : $country_code;
				
				
				
				$form['clra_reg_master']['address_manager']			= array(
																			'#prefix' 			=> '<div id="clra_manager_address_replace">',
																			'#suffix' 			=> '</div>',
																			);
				
				$form['clra_reg_master']['manager_state']			= array(
																			'#prefix' 			=> '<div id="clra_manager_state_replace">',
																			'#suffix' 			=> '</div>',
																			);
																			
				$form['clra_reg_master']['manager_dist']			= array(
																			'#prefix' 			=> '<div id="clra_manager_district_replace">',
																			'#suffix' 			=> '</div>',
																			);
																			
				$form['clra_reg_master']['loc_manager_subdv']		= array(
																			'#prefix' 			=> '<div id="clra_subdvsn_replace_3">',
																			'#suffix' 			=> '</div>',
																			);
																			
				$form['clra_reg_master']['loc_manager_areatype']	= array(
																			'#prefix' 			=> '<div id="clra_loc_e_areatype_3">',
																			'#suffix' 			=> '</div>',
																			);
																			
				$form['clra_reg_master']['manager_name_areatype']	= array(
																			'#prefix' 			=> '<div id="clra_common_1_replace_3">',
																			'#suffix' 			=> '</div>',
																			);
																			
				$form['clra_reg_master']['loc_manager_vill_ward']	= array(
																			'#prefix' 			=> '<div id="clra_common_2_replace_3">',
																			'#suffix' 			=> '</div>',
																			);
																			
				$form['clra_reg_master']['loc_manager_ps']			= array(
																			'#prefix' 			=> '<div id="clra_ps_replace_3">',
																			'#suffix' 			=> '</div>',
																			);
																			
				$form['clra_reg_master']['loc_manager_pin_number']	= array(
																			'#prefix' 			=> '<div id="clra_manager_pin_replace">',
																			'#suffix' 			=> '</div>',
																			);
																			
				if( (!empty($value_manager_country) && $value_manager_country != '') && $value_manager_country ==  1) {
					
					
					$db_manager_state_value								= db_query("SELECT COUNT(id) FROM {state_master} WHERE country_id = '".$value_manager_country."'")->fetchObject();
				
				
					$form['clra_reg_master']['address_manager'] 						= array(
																							  '#prefix' 			=> '<div id="clra_manager_address_replace" class="col-lg-4 col-md-6"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'House No./Village Name/Street/Road',
																							  '#type' 				=> 'textfield',
																							  '#required' 			=> TRUE,
																							  '#default_value' 		=>!empty($address) ? $address : '',
																							 // '#element_validate' => array('NEW_CLRA_SPECIAL_CHAR_TEXT_VALIDATE')
																								);
																										
																										
					$form['clra_reg_master']['manager_state']							= array(
																								'#prefix' 			=> '<div id="clra_manager_state_replace" class="col-lg-4 col-md-6"><label class="select">',
																								'#suffix' 			=> '</label></div>',
																								'#title'			=> t('Select State'),
																								'#type' 			=> 'select',
																								'#options' 			=> get_all_state($value_manager_country),
																								'#default_value'	=> $state_code,
																								'#required' 		=> TRUE,
																								'#ajax'				=> array(
																															'event'		=> 'change',
																															'effect' 	=> 'fade',
																															'callback'	=> 'get_ajax_callback_new_4',
																															'progress'	=>	'',
																															)
																							);	
																										
					$value_manager_state							= isset($form_state['values']['manager_state'])? $form_state['values']['manager_state'] : $state_code;
					
					
					if($value_manager_state != $state_code ){
							
							$district_code 			= '';
							$subdivision_code 		= '';
							$areatype_code			= '';
							$name_areatype_code		= '';
							$vill_ward_code			= '';
							$ps_code				= '';
							$pin					= '';
							
					}
					
					/****** MANAGER DISTRICT BLOCK STARTS ******/
					
					if(($value_manager_country != '' && (!empty($value_manager_state) || $value_manager_state != '' )  )){
				
						$db_manager_district_value					= db_query("SELECT COUNT(id) FROM {district_master} WHERE state_id = '".$value_manager_state."'")->fetchObject();	
					
						if(!empty($db_manager_district_value) && ($db_manager_district_value->count>0) ) {
							
							
							$form['clra_reg_master']['manager_dist']					= array(
																							'#prefix' 				=> '<div id="clra_manager_district_replace" class="col-lg-4 col-md-6"><label class="select">',
																							'#suffix' 				=> '</label></div>',
																							'#title'				=> t('Select District'),
																							'#type' 				=> 'select',
																							'#options' 				=> get_district_by_id($value_manager_state),
																							'#default_value' 		=> !empty($district_code) ? $district_code : '',
																							'#required' 			=> TRUE,
																							'#ajax'					=> array(
																															'event'		=> 'change',
																															'effect' 	=> 'fade',
																															'callback'	=> 'get_ajax_callback_new_4',
																															'progress'	=>	'',
																															)
																							);	
							
							
						
							$value_loc_e_dist_3 							= 	isset($form_state['values']['manager_dist']) ? $form_state['values']['manager_dist'] : $district_code;
							
							if($value_loc_e_dist_3 != $district_code ){
							
								$subdivision_code 		= '';
								$areatype_code			= '';
								$name_areatype_code		= '';
								$vill_ward_code			= '';
								$ps_code				= '';
								$pin					= '';
	
							
							}
							
						}
						
					}
					
					/****** MANAGER SUBDIVISION BLOCK STARTS ******/	
						
						
					if($value_loc_e_dist_3 != ''  && !empty($value_manager_state)){
						
						$db_manager_subdivision_value						= db_query("SELECT COUNT(id) as subdv_total_manager  FROM {sub_division} WHERE district_code = '".$value_loc_e_dist_3."'")->fetchObject();
						
						if(!empty($db_manager_subdivision_value) && $db_manager_subdivision_value != '' && $db_manager_subdivision_value->subdv_total_manager > 0) {
						
						
								$form['clra_reg_master']['loc_manager_subdv'] 			= array(
																							  '#prefix' 			=> '<div id="clra_subdvsn_replace_3" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Subdivision',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist_3),
																							  '#default_value' 		=> !empty($subdivision_code) ? $subdivision_code	:'',
																							  '#required' 			=> TRUE,
																							  '#id'					=> 'loc_e_subdivision_id_3',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_4',
																														'progress'			=> '',
																													  ),
																						);
					
					
							$value_loc_e_subdivision_3 				= isset($form_state['values']['loc_manager_subdv']) ? $form_state['values']['loc_manager_subdv'] : $subdivision_code;
							
							if($value_loc_e_subdivision_3 != $subdivision_code ){
									
								$areatype_code			= '';
								$name_areatype_code		= '';
								$vill_ward_code			= '';
								$ps_code				= '';
								$pin					= '';
							
							}
							
							
						}
							
							
					}
					
					if($value_loc_e_dist_3 != "" && !empty($value_manager_state) && $value_loc_e_subdivision_3 != ""){
						
						$db_block_mun_manager_value					= db_query("SELECT COUNT(id) as areatype_manager_total FROM {block_mun_master} WHERE distict_code = '".$value_loc_e_dist_3."' and sub_division_code  = '".$value_loc_e_subdivision_3."'")->fetchObject();
						
						if(!empty($db_block_mun_manager_value) &&  $db_block_mun_manager_value != '' && $db_block_mun_manager_value->areatype_manager_total > 0){
							
							$areatypeOptions						= common_custom_user_areatype_list_new($value_loc_e_dist_3, $value_loc_e_subdivision_3);
							
							if(!empty($areatypeOptions)){
								
								
								$form['clra_reg_master']['loc_manager_areatype'] 		= array(
																							  '#prefix' 			=> '<div id="clra_loc_e_areatype_3" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Block / Municipality / Corporation / SEZ / Notified Area',
																							  '#type' 				=> 'select',
																							  '#options' 			=> $areatypeOptions,
																							  '#default_value' 		=> !empty($areatype_code) ? $areatype_code : '',
																							  '#required' 			=> TRUE,
																							  '#id'					=> 'loc_e_areatype_id_3',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_4',
																														'progress'			=> '',
																													  ),
																								);	
																										
								$value_loc_e_areatype_3 			= isset($form_state['values']['loc_manager_areatype']) ? $form_state['values']['loc_manager_areatype'] : $areatype_code;
								
							}
							
							if($value_loc_e_subdivision_3 != $subdivision_code && $value_loc_e_areatype_3 != $areatype_code){
								
								$name_areatype_code		= '';
								$vill_ward_code			= '';
								$ps_code				= '';
								$pin					= '';
							
							}
							
						}
						
					}
					
					if ($value_loc_e_dist_3 != "" && $value_loc_e_subdivision_3 != "" && $value_loc_e_areatype_3 != "") {
						
						if ( $value_loc_e_areatype_3 == 'B' ){
							$areatype_name	= 'Block';
						}elseif( $value_loc_e_areatype_3 == 'M'){
							$areatype_name	= 'Municipality';
						}elseif( $value_loc_e_areatype_3 == 'C'){
							$areatype_name	= 'Corporation';
						}elseif( $value_loc_e_areatype_3 == 'S'){
							$areatype_name	= 'SEZ';
						}elseif($value_loc_e_areatype_3 == 'N'){
							$areatype_name	= 'Notified Area';
						}
						
						$form['clra_reg_master']['manager_name_areatype'] 				= array(
																							  '#prefix' 			=> '<div id="clra_common_1_replace_3" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select '.$areatype_name,
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_loc_e_subdivision_3, $value_loc_e_areatype_3),
																							  '#default_value' 		=> !empty($name_areatype_code) ? $name_areatype_code : '',
																							  '#required' 			=> TRUE,
																							  '#id'					=> 'common_selection_1_id_3',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_ajax_callback_new_4',
																														'progress'			=> '',
																													  )
																							);
																									
						$value_common_selection_1_3 				= isset($form_state['values']['manager_name_areatype']) ? $form_state['values']['manager_name_areatype'] : $name_areatype_code;
						
						if($value_common_selection_1_3 != $name_areatype_code){
								
								$vill_ward_code			= '';
								$ps_code				= '';
								$pin					= '';
							
							}
						
						
					}
					
					if($value_loc_e_dist_3 != "" && $value_loc_e_subdivision_3 != "" && $value_loc_e_areatype_3 != ""  && $value_common_selection_1_3 != "" && !empty($value_manager_state) ){
					
						if ( $value_loc_e_areatype_3 == 'B' ){
							$area_name		= 'Gram Panchayat';
							$area_code 		= 'V';
						}elseif( $value_loc_e_areatype_3 == 'M'){
							$area_name		= 'Ward';
							$area_code 		= 'W';
						}elseif( $value_loc_e_areatype_3 == 'C'){
							$area_name		= 'Ward';
							$area_code 		= 'C';
						}elseif( $value_loc_e_areatype_3 == 'S'){
							$area_name		= 'Sector';
							$area_code 		= 'S';
						}elseif($value_loc_e_areatype_3 == 'N'){
							$area_name		= 'Notified Area';
							$area_code 		= 'N';
						}
						
						$form['clra_reg_master']['loc_manager_vill_ward'] 				= array(
																							  '#prefix' 			=> '<div id="clra_common_2_replace_3" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select '.$area_name,
																							  '#type' 				=> 'select',
																							  '#options'			=> common_custom_user_villward_list_new($value_common_selection_1_3,$area_code),
																							  '#default_value' 		=> !empty($vill_ward_code) ? $vill_ward_code : '',
																							  '#required' 			=> TRUE,
																							  '#id'					=> 'common_selection_2_id_3',
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_ajax_callback_new_4',
																															'progress'			=> '',
																														  )
																							);	
																									
						$value_common_selection_2_3 				= isset($form_state['values']['loc_manager_vill_ward']) ? $form_state['values']['loc_manager_vill_ward'] : $vill_ward_code;	
						
						if($value_common_selection_2_3 != $vill_ward_code){
								
								$ps_code				= '';
								$pin					= '';
								
						}
						
						
					}
					
					if (!empty($value_manager_state) && $value_loc_e_dist_3 != "" && $value_loc_e_subdivision_3 != "" && $value_loc_e_areatype_3 && $value_common_selection_1_3 != "" && $value_common_selection_2_3 != "" ){
					
						$form['clra_reg_master']['loc_manager_ps'] 						= array(
																							  '#prefix' 			=> '<div id="clra_ps_replace_3" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Police Station',
																							  '#type' 				=> 'select',
																							  '#id'					=> 'loc_e_ps_id_3',
																							  '#options' 			=> common_custom_user_ps_list_new($value_loc_e_dist_3),
																							  '#default_value' 		=> !empty($ps_code) ? $ps_code : '',
																							  '#required' 			=> TRUE
																						  );
																									  
						$form['clra_reg_master']['loc_manager_pin_number'] 				= array(
																							  '#prefix' 			=> '<div id="clra_manager_pin_replace" class="col-lg-4 col-md-6"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Pin Code',
																							  '#type' 				=> 'textfield',
																							  '#default_value' 		=>!empty($pin) ? $pin : '',
																							  '#attributes' 		=> array('maxlength' => 6),
																							  '#required' 			=> TRUE
																							  );	
						
						
					}
					
				}else{
					
					$form['clra_reg_master']['address_manager'] 						= array(
																							  '#prefix' 			=> '<div id="clra_manager_address_replace" class="col-md-12"><label class="textarea">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Address[ If other Country/State please provide detail address ]',
																							  '#rows'				=> t('2'),	
																							  '#cols'				=> t('146'),	
																							  '#type' 				=> 'textarea',
																							  '#attributes' 		=> array('placeholder' => 'Street Name/Landmark/Pin Code'),
																							  '#default_value' 		=>!empty($address) ? $address : '',
																							  '#required' 			=> TRUE,
																							//  '#element_validate' 	=> array('NEW_CLRA_SPECIAL_CHAR_TEXT_VALIDATE')
																							);
																									
				$form['clra_reg_master']['manager_state']								= array(
																								'#prefix' 			=> '<div id="clra_manager_state_replace">',
																								'#suffix' 			=> '</div>',
																								);
																			
				$form['clra_reg_master']['manager_dist']								= array(
																								'#prefix' 			=> '<div id="clra_manager_district_replace">',
																								'#suffix' 			=> '</div>',
																								);
																			
				$form['clra_reg_master']['loc_manager_subdv']							= array(
																								'#prefix' 			=> '<div id="clra_subdvsn_replace_3">',
																								'#suffix' 			=> '</div>',
																								);
																			
				$form['clra_reg_master']['loc_manager_areatype']						= array(
																								'#prefix' 			=> '<div id="clra_loc_e_areatype_3">',
																								'#suffix' 			=> '</div>',
																								);
																			
				$form['clra_reg_master']['manager_name_areatype']						= array(
																								'#prefix' 			=> '<div id="clra_common_1_replace_3">',
																								'#suffix' 			=> '</div>',
																								);
																			
				$form['clra_reg_master']['loc_manager_vill_ward']						= array(
																								'#prefix' 			=> '<div id="clra_common_2_replace_3">',
																								'#suffix' 			=> '</div>',
																								);
																			
				$form['clra_reg_master']['loc_manager_ps']								= array(
																								'#prefix' 			=> '<div id="clra_ps_replace_3">',
																								'#suffix' 			=> '</div>',
																								);
																			
				$form['clra_reg_master']['loc_manager_pin_number']						= array(
																								'#prefix' 			=> '<div id="clra_manager_pin_replace">',
																								'#suffix' 			=> '</div>',
																								);
					
					
				}
				
				
				$options_cont 	= 	contractor_works_multiSelect_options();
				
				$form['clra_reg_master']['e_nature_of_work'] 							= array(
																							 '#prefix' 				=> '</div></div><div class="panel panel-default panel-form"><div class="panel-heading">5. Nature of work carried on in the establishment</div><div class="panel-body"><div class="col-md-12"><label class="select">',
																							 '#suffix' 				=> '</label></div>',
																							 '#title'				=> 'Select Nature of Work',
																							 '#description'			=> '<span style="color:#f00;">Note: If multiple Nature of work to be selected, press Ctrl and select the options.<br/><br/></span>',
																							 '#type' 				=> 'select',
																							 '#id' 					=> 'e_nature_of_work_id',
																							 '#options' 			=> $options_cont,
																							 '#multiple' 			=> TRUE,
																							 '#attributes' 			=> array('style'=>"height: 120px;", 'onchange' => 'nature_wrk_others(this.value)'),
																							 '#required' 			=> TRUE
																						);	
																										
																											
				$form['clra_reg_master']['other_nature_of_work_value'] 					= array(
																							 '#prefix' 				=> '<div class="col-md-12" id ="edit_other_nature_work_id"><label class="input">',
																							 '#suffix' 				=> '</label></div>',
																							 '#title'				=> 'Other Option for Nature of Work',
																							 '#type' 				=> 'textfield',
																							 '#id' 					=> 'other_value_nature_of_work_id',
																						);
																										
				$form['clra_reg_master']['max_num_wrkmen'] 								= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> '(a) Maximum Number of Workmen Employed Directly on any day in the Establishment',
																							  '#type' 				=> 'textfield',
																							  '#default_value' 		=> !empty($e_any_day_max_num_of_workmen) ? $e_any_day_max_num_of_workmen : '',
																							  '#attributes' 		=> array('id' => 'e_any_day_max_num_of_workmen_id', 'onkeyup' => 'checkFees(this.value)', 'onblur' => 'checkFees(this.value)'),
																							  '#required' 			=> TRUE,
																							  '#element_validate' 	=> array('REGISTRATION_TEXTBOX_NUMERIC_VALIDATE'),	
																								  
																								);
																							
				$form['clra_reg_master']['e_num_of_workmen_per_or_reg'] 			  	= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#required' 			=> TRUE,
																							  '#title'				=> '(b) Number of Workmen Engaged as Permanent/Regular Workmen',
																							  '#type' 				=> 'textfield',
																							  '#default_value' 		=> !empty($e_num_of_workmen_per_or_reg) ? $e_num_of_workmen_per_or_reg : '',
																							  '#element_validate' 	=> array('REGISTRATION_TEXTBOX_NUMERIC_VALIDATE'),
																							);
																						
				$form['clra_reg_master']['e_num_of_workmen_temp_or_reg'] 			 	= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> '(c) Number of Workmen Engaged as Temporary/Regular Workmen',
																							  '#type' 				=> 'textfield',
																							  '#default_value' 		=> !empty($e_num_of_workmen_temp_or_reg) ? $e_num_of_workmen_temp_or_reg : '',
																							  '#required' 			=> TRUE,
																							  '#element_validate' 	=> array('REGISTRATION_TEXTBOX_NUMERIC_VALIDATE'),
																							);
										
				$workmenOptions			= array(0=> t('No'), 1=> t('Yes'));
									
				$form['clra_reg_master']['workmen_if_same_similar_kind_of_work'] 		= array(
																							 '#prefix' 				=> '<div class="col-md-12 inline-group">',
																							 '#suffix' 				=> '</div>',
																							 '#type' 				=> 'radios',
																							 '#title' 				=> '(d) Whether the Workmen employed/intended to be Employment by the Contractor Perform the same or similar kind of work as the Workmen employed directly by the Principal Employer (if yes, please give here information as detailed below)',
																							 '#options' 			=> $workmenOptions,
																							 '#required' 			=> TRUE,
																							);	
										
				$form['clra_reg_master']['con_lab_job_desc'] 							= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="textarea">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'i. A complete job description of the contract labour',
																							  '#type' 				=> 'textarea',
																							  '#rows'				=> t('4'),	
																							  '#required' 			=> TRUE,
																							  '#element_validate' 	=> array('NEW_CLRA_SPECIAL_CHAR_TEXT_VALIDATE')
																							);
																				
				$form['clra_reg_master']['con_lab_wage_rate_other_benefits'] 			= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'ii. Wage rates and other cash benefits paid/to be paid',
																							  '#type' 				=> 'textfield',
																							  '#required' 			=> TRUE,
																							  '#element_validate'   => array('NEW_CLRA_SPECIAL_CHAR_TEXT_VALIDATE')
																						);
															
				$form['clra_reg_master']['con_lab_cat_desig_nom'] 						= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'iii. Category/designation/nomenclature of the job',
																							  '#type' 				=> 'textfield',
																							  '#required' 			=> TRUE,
																							  '#element_validate' 	=> array('NEW_CLRA_SPECIAL_CHAR_TEXT_VALIDATE')
																						);
									
				
															
				$form['clra_reg_master']['e_settlement_award_judgement_min_wage'] 		= array(
																							  '#prefix' 			=> '<div class="col-md-8"><label class="input">',
																							  '#suffix' 			=> '</label></div></div></div></div>',
																							  '#title'				=> '(e) Settlement or award or judgement or minimum wages (if any applicable in the establishment)',
																							  '#type' 				=> 'textfield',
																							  '#required' 			=> TRUE,
																							  '#element_validate' 	=> array('NEW_CLRA_SPECIAL_CHAR_TEXT_VALIDATE')
																								);
																												
				$form['clra_reg_master']['e_any_day_max_num_of_workmen'] 				= array(
																							  '#prefix' 			=> '<div class="panel panel-default panel-form"><div class="panel-heading">6. Maximum number of contract labour to be employed on any day through each contractor</div><div class="panel-body"><div class="col-md-8"><label class="input">',
																							  '#suffix' 			=> '</label></div></div></div>',
																							  '#title'				=> 'Maximum number of contract labour to be employed on any day through each contractor',
																							  '#type' 				=> 'textfield',
																							  '#required' 			=> TRUE,
																							  '#element_validate' 	=> array('REGISTRATION_TEXTBOX_NUMERIC_VALIDATE')
																							);
																													
				$upload_value_1 = array( 'trade_license' => t('Trade License' ));
				$upload_value_2 = array( 'article_of_assoc' => t('Articles of Association and Memorandum of Association/Partnership Deed'));
				$upload_value_3 = array( 'memorandum_of_cert' => t('Any other document in support of correctness of the particulars mentioned in the application if required '));
				$upload_value_4 = array( 'partnership_deed' => t('Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc.'));
				$upload_value_5 = array( 'factory_license' => t('Factory License if any'));			
				
				if ( $trade_license_file != "" ) {
					$tlf = "checked";
				}else{
					$tlf = "";
				}
				if ( $article_of_assoc_file != "" ) {
					$asf = "checked";
				}else{
					$asf = "";
				}
				if ( $memorandum_of_cert_file != "" ) {
					$mcf = "checked";
				}else{
					$mcf = "";
				}
				if ( $partnership_deed_file != "" ) {
					$pdf = "checked";
				}else{
					$pdf = "";
				}
				if ( $factory_license_file != "" ) {
					$flf = "checked";
				}else{
					$flf = "";
				}
				
				$form['clra_reg_master']['upload_check_in_1'] 							= array(
																							  '#prefix' 			=> '<div class="panel panel-default panel-form"><div class="panel-heading">Upload Supporting Documents</div><div class="panel-body"><section><label class="checkboxe">',
																							  '#suffix' 			=> '</label></section>',
																							  '#type' 				=> 'checkboxes',
																							  '#options' 			=> $upload_value_1,
																							  '#attributes'			=> array( 'id' => 'upload_check_in_1_id', 'onclick' => 'upload_check_fun_1()', $tlf => $tlf )
																							);
										
				$form['clra_reg_master']['upload_check_in_2'] 							= array(
																							  '#prefix' 			=> '<section><label class="checkboxe">',
																							  '#suffix' 			=> '</label></section>',
																							  '#type' 				=> 'checkboxes',
																							  '#options' 			=> $upload_value_2,
																							  '#attributes'			=> array( 'id' => 'upload_check_in_2_id', 'onclick' => 'upload_check_fun_2()', $asf => $asf )
																							);
														
				$form['clra_reg_master']['upload_check_in_3'] 							= array(
																							  '#prefix' 			=> '<section><label class="checkboxe">',
																							  '#suffix' 			=> '</label></section>',
																							  '#type' 				=> 'checkboxes',
																							  '#options' 			=> $upload_value_3,
																							  '#attributes'			=> array( 'id' => 'upload_check_in_3_id', 'onclick' => 'upload_check_fun_3()', $mcf => $mcf )
																							);
														
				$form['clra_reg_master']['upload_check_in_4'] 							= array(
																							  '#prefix' 			=> '<section><label class="checkboxe">',
																							  '#suffix' 			=> '</label></section>',
																							  '#type' 				=> 'checkboxes',
																							  '#options' 			=> $upload_value_4,
																							  '#attributes'			=> array( 'id' => 'upload_check_in_4_id', 'onclick' => 'upload_check_fun_4()', $pdf => $pdf )
																							);
																						
				$form['clra_reg_master']['upload_check_in_5'] 							= array(
																							  '#prefix' 			=> '<section><label class="checkboxe">',
																							  '#suffix' 			=> '</label></section>',
																							  '#type' 				=> 'checkboxes',
																							  '#options' 			=> $upload_value_5,
																							  '#attributes'			=> array( 'id' => 'upload_check_in_5_id', 'onclick' => 'upload_check_fun_5()', $flf => $flf )
																							);
																						
														
				$form['clra_reg_master']['row1'] 										= array(
																							  '#markup' 			=> '<section id="trade_license_div" >',
																								); 
																								
				$form['clra_reg_master']['trade_license'] 								= array(
																							'#type' 				=> 'managed_file',
																							'#title'				=> '1. Upload Trade License',
																							'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																							'#upload_location' 		=> 'public://upload/trade_license/',
																							'#process' 				=> array('import_my_file_element_process'),
																							'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																							'#prefix' 				=> '<label class="input input-file">',
																							'#suffix' 				=> '</label>'
																								
																						);
				
				$form['clra_reg_master']['row2'] 										= array(
																							 '#markup'				=> '</section><section id="article_of_assoc_div">',
																							 ); 
				
				$form['clra_reg_master']['article_of_assoc'] 							= array(
																							'#type' 				=> 'managed_file',
																							'#title'				=>'2. Upload Articles of Association and Memorandum of Association/Partnership Deed',
																							'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																							'#upload_location' 		=> 'public://upload/article_of_assoc/',
																							'#process' 				=> array('import_my_file_element_process'),
																							'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																							'#prefix' 				=> '<label class="input input-file">',
																							'#suffix' 				=> '</label>',
																					);
				
				
				$form['clra_reg_master']['row3'] 										= array(
																							 '#markup' 				=> '</section><section id="memorandum_of_cert_div">',
																							);
			
				$form['clra_reg_master']['memorandum_of_cert'] 							= array(
																							'#type' 				=> 'managed_file',
																							'#title'				=>'3. Upload Any other document in support of correctness of the particulars mentioned in the application if required',
																							'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																							'#upload_location' 		=> 'public://upload/memorandum_of_cert/',
																							'#process' 				=> array('import_my_file_element_process'),
																							'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																							'#prefix' 				=> '<label class="input-file">',
																							'#suffix' 				=> '</label>'
																						);	
				
				
				
				$form['clra_reg_master']['row4'] 										=  array(
																							'#markup' 				=> '</section><section id="partnership_deed_div">',
																						  ); 
				
				$form['clra_reg_master']['partnership_deed'] 							= array(
																							'#type' 				=> 'managed_file',
																							'#title'				=>'4. Upload Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc.',
																							'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																							'#upload_location' 		=> 'public://upload/partnership_deed/',
																							'#process' 				=> array('import_my_file_element_process'),
																							'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																							'#field_prefix' 		=> '<label class="input-file">',
																							'#field_suffix' 		=> '</label>'
																							);
				
				
				
				$form['clra_reg_master']['row5'] 										= array(
																							 '#markup' 				=> '</section><section id="factory_license_div">',
																							);
																						
				$form['clra_reg_master']['factory_license'] 							= array(
																							'#type' 				=> 'managed_file',
																							'#title'				=>'5. Upload Factory License if any',
																							'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																							'#upload_location' 		=> 'public://upload/FactoryLicense/',
																							'#process' 				=> array('import_my_file_element_process'),
																							'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																							'#field_prefix' 		=> '<label class="input-file">',
																							'#field_suffix' 		=> '</label>'
																							);
																											
																						
				$form['clra_reg_master']['row6'] 										= array(
																							 '#markup' 				=> '</section></div></div>',
																											);
																		  
				$form['clra_reg_master']['submit'] 										= array (
																						  '#prefix' 				=> '<div class="col-md-12"><div class="row">',
																						  '#type' 					=> 'submit',
																						  '#attributes' 			=> array('class' => array('btn btn-primary pull-right form-submit')),
																						  '#value' 					=> 'SAVE'
																						);
																						
			
																					
				return $form;
					
			}else{
				
				drupal_goto('applicant-dashboard');
			}
		}else {
			
			drupal_goto('applicant-profile');
		}
	
}

function NEW_CLRA_SPECIAL_CHAR_TEXT_VALIDATE($element,&$form_state){
	$txtValue		=	trim($element['#value']); 
	$match_pattern 	= 	'/[A-Za-z]+/';
	if(!preg_match($match_pattern, $txtValue)) {
		form_error($element, t($element['#title'].' SHOULD BE TEXT ONLY'));
	}
}

function REGISTRATION_TEXTBOX_NUMERIC_VALIDATE($element,&$form_state){
	
	$fieldValue=trim($element['#value']); 
	$match_pattern = '/^[0-9]+$/i';
	
	if(!empty($fieldValue)){
		if(!preg_match($match_pattern, $fieldValue)) {
			form_error($element, t($element['#title'].' SHOULD BE NUMBER ONLY'));
		}
	}

}

function CLRA_AREATYPE_VALIDATE($element, &$form_state){
	$val 					= 	$form_state['values'];
	$loc_e_areatype			=	$val['loc_e_areatype'];
	$loc_emp_areatype		=	$val['loc_emp_areatype'];
	$loc_manager_areatype	=	$val['loc_manager_areatype'];
	if( $loc_e_areatype == "T" ){
		form_error($element, t('Establishment AreaType cannot be blank.'));
	}
	if( $loc_emp_areatype == "T" ){
		form_error($element, t('Principal Employer AreaType cannot be blank.'));
	}
	if( $loc_manager_areatype == "T" ){
		form_error($element, t('Manager or Person Responsible AreaType cannot be blank.'));
	}
}

function CLRA_DISTRICT_SUBDIV_BLOCK_VALIDATE($element,&$form_state){
	$val			=	$form_state['values'];
	$txtValue		=	trim($element['#value']);
	if($txtValue == 0 ) {
		form_error($element, t($element['#title'].' SHOULD BE VALID'));
		$form_state['rebuild'] = TRUE;
	}
}

function CLRA_POLICE_STATION_VALIDATE($element, &$form_state){
	$val				=	$form_state['values'];
	$police_statn_1		=	$val['loc_e_ps'];
	$police_statn_2		=	$val['e_postal_ps'];
	$police_statn_3		=	$val['e_permanent_add_ps'];
	$police_statn_4		=	$val['address_manager_ps'];
	
	if( $police_statn_1 == "" ){
		form_set_error($form_state['clra_reg_master']['loc_e_ps'], t('Police Station SHOULD BE VALID'));
	}
	if( $police_statn_2 == "" ){
		form_set_error($form_state['clra_reg_master']['e_postal_ps'], t('Police Station cfgfg SHOULD BE VALID'));
	}
	if( $police_statn_3 == "" ){
		form_set_error($form_state['clra_reg_master']['e_permanent_add_ps'], t('Police Station gddgdg SHOULD BE VALID'));
	}
	if( $police_statn_4 == "" ){
		form_set_error($form_state['clra_reg_master']['address_manager_ps'], t('Police Station dgdgdgdgdgdgdg SHOULD BE VALID'));
	}
}

function REGISTRATION_TEXTBOX_TEXT_VALIDATE($element,&$form_state){
	$txtValue		=	trim($element['#value']); 
	$match_pattern 	= 	'/^[ A-Za-z.\/\(\) ]+$/';
	if(!preg_match($match_pattern, $txtValue)) {
		form_error($element, t($element['#title'].' SHOULD BE TEXT ONLY'));
	}
}

function get_clra_registration_info_new_form_submit($form, &$form_state){ 

	global $base_root, $base_path, $user;
	
	$val 									= $form_state['values'];
	$increment 								= 0;
	$other_nature_counter					= 0;
	$action_encrypt							= 'encrypt';
	
	/**** DOC UPLOADED CODE *****/


	/* Upload For Trade License */
	
	$arr_1									= array();
	$trade_license 							= file_load($val['trade_license']);
	if( $trade_license != "" ){
		$arr_1 = explode("/", $trade_license->uri);
		$trade_license->status = FILE_STATUS_PERMANENT;
		file_save($trade_license);
		$trade_license_file = $arr_1[4];
	}
	
	
	/* Upload For Article of Association */
	
	$arr_2									= array();
	$article_of_assoc 						= file_load($val['article_of_assoc']);
	if( $article_of_assoc != "" ){
		$arr_2 = explode("/", $article_of_assoc->uri);
		$article_of_assoc->status = FILE_STATUS_PERMANENT;
		file_save($article_of_assoc);
		$article_of_assoc_file = $arr_2[4];
	}
	
	/* Upload For Memorandum of Certificate */
	
	$arr_3									= array();
	$memorandum_of_cert 					= file_load($val['memorandum_of_cert']);
	if( $memorandum_of_cert != "" ){
		$arr_3 = explode("/", $memorandum_of_cert->uri);
		$memorandum_of_cert->status = FILE_STATUS_PERMANENT;
		file_save($memorandum_of_cert);
		$memorandum_of_cert_file = $arr_3[4];
	}
	
	/* Upload For Partnership Deed */
	
	$arr_4									= array();
	$partnership_deed 					= file_load($val['partnership_deed']);
	if( $partnership_deed != "" ){
		$arr_4 = explode("/", $partnership_deed->uri);
		$partnership_deed->status = FILE_STATUS_PERMANENT;
		file_save($partnership_deed);
		$partnership_deed_file = $arr_4[4];
	}
	
	/* Upload For Factory License */
	
	$arr_5									= array();
	$factory_license 					= file_load($val['factory_license']);
	if( $factory_license != "" ){
		$arr_5 = explode("/", $factory_license->uri);
		$factory_license->status = FILE_STATUS_PERMANENT;
		file_save($factory_license);
		$factory_license_file = $arr_5[4];
	}
	
	

	/**** DOC UPLOADED CODE ENDS*****/
	
		$identification_number					= 'CLRA'.time();	
		$act_id									= $val['clra_act_id'];
			
		$e_name 								= $val['e_name'];
		$loc_e_name 							= $val['loc_e_name'];
		
		$loc_e_dist 							= $val['loc_e_dist'];
		$loc_e_subdivision 						= $val['loc_e_subdivision'];
		$loc_e_areatype 						= $val['loc_e_areatype'];
		$name_areatype 							= $val['name_areatype'];
		$loc_e_vill_ward						= $val['loc_e_vill_ward'];
		$l_e_ps									= $val['loc_e_ps'];
		$loc_e_pin_number						= $val['loc_e_pin_number'];
		
		$e_postal_address 						= $val['e_postal_address'];
		
		$e_postal_dist 							= $val['e_postal_dist'];
		$e_postal_subdivision 					= $val['e_postal_subdivision'];
		$e_postal_areatype 						= $val['e_postal_areatype'];
		$e_postal_name_areatype 				= $val['e_postal_name_areatype'];
		$e_postal_vill_ward						= $val['e_postal_vill_ward'];
		$e_postal_ps							= $val['e_postal_ps'];
		$e_postal_pin_number					= $val['e_postal_pin_number'];
		
		
		$full_name_principal_emp 				= $val['full_name_principal_emp'];
		$address_principal_emp 					= $val['address_principal_emp'];
		
		$loc_emp_country						= $val['emp_country'];
		$loc_emp_state							= $val['emp_state'];
		$loc_emp_dist 							= $val['emp_dist'];
		$loc_emp_subdivision 					= $val['loc_emp_subdv'];
		$loc_emp_areatype 						= $val['loc_emp_areatype'];
		$emp_name_areatype 						= $val['emp_name_areatype'];
		$loc_emp_vill_ward						= $val['loc_emp_vill_ward'];
		$l_emp_ps								= $val['loc_emp_ps'];
		$loc_emp_pin_number						= $val['loc_emp_pin_number'];
		
		
		
		$full_name_manager 						= $val['full_name_manager'];
		$address_manager 						= $val['address_manager'];
		
		$loc_manager_country					= $val['manager_country'];
		$loc_manager_state						= $val['manager_state'];
		$loc_manager_dist 						= $val['manager_dist'];
		$loc_manager_subdivision 				= $val['loc_manager_subdv'];
		$loc_manager_areatype 					= $val['loc_manager_areatype'];
		$manager_name_areatype 					= $val['manager_name_areatype'];
		$loc_manager_vill_ward					= $val['loc_manager_vill_ward'];
		$l_manager_ps							= $val['loc_manager_ps'];
		$loc_manager_pin_number					= $val['loc_manager_pin_number'];
		
		
		
		$e_nature_of_work 						= $val['e_nature_of_work'];
		$other_nature_of_work_value				= $val['other_nature_of_work_value'];
		
		$max_num_wrkmen 						= $val['max_num_wrkmen'];
		$e_num_of_workmen_per_or_reg 			= $val['e_num_of_workmen_per_or_reg'];
		$e_num_of_workmen_temp_or_reg 			= $val['e_num_of_workmen_temp_or_reg'];
		$workmen_if_same_similar_kind_of_work 	= $val['workmen_if_same_similar_kind_of_work'];
	 	$con_lab_job_desc 						= $val['con_lab_job_desc'];
	 	$con_lab_wage_rate_other_benefits 		= $val['con_lab_wage_rate_other_benefits'];
	 	$con_lab_cat_desig_nom 					= $val['con_lab_cat_desig_nom'];
	 	$e_settlement_award_judgement_min_wage  = $val['e_settlement_award_judgement_min_wage'];
	 	$e_any_day_max_num_of_workmen 			= $val['e_any_day_max_num_of_workmen'];
		

		$fieldsRegInfo 							= array( 
															'user_id'									=>	$user->uid,
															'act_id'									=>	$act_id,
															'identification_number'						=>	$identification_number,
															'e_name'									=>	$e_name,
															'est_type'									=>  $val['est_type'],
															'loc_e_name'								=>	$loc_e_name,
															
															'loc_e_dist'								=>	$loc_e_dist,
															'loc_e_subdivision'							=>	$loc_e_subdivision ,
															'loc_e_areatype'							=>	$loc_e_areatype ,
															'name_areatype'								=>	$name_areatype ,
															'loc_e_vill_ward'							=>	$loc_e_vill_ward ,
															'l_e_ps'									=>	$l_e_ps,
															'loc_e_pin_number'							=>  $loc_e_pin_number,
															
															'e_postal_address'							=>	$e_postal_address,
															
															'e_postal_dist'								=>	$e_postal_dist,
															'e_postal_subdivision'						=>	$e_postal_subdivision ,
															'e_postal_areatype'							=>	$e_postal_areatype ,
															'e_postal_name_areatype'					=>	$e_postal_name_areatype ,
															'e_postal_vill_ward'						=>	$e_postal_vill_ward ,
															'e_postal_ps'								=>	$e_postal_ps,
															'e_postal_pin_number'						=>	$e_postal_pin_number, 
															
															'full_name_principal_emp'					=>	$full_name_principal_emp, 
															'gender_pe'									=>  $val['gender_pe'],
															'address_principal_emp'						=>	$address_principal_emp, 
															
															'loc_emp_country'							=>  $loc_emp_country,
															'loc_emp_state'								=>  $loc_emp_state,
															'loc_emp_dist'								=>	$loc_emp_dist,
															'loc_emp_subdivision'						=>	$loc_emp_subdivision ,
															'loc_emp_areatype'							=>	$loc_emp_areatype,
															'emp_name_areatype'							=>	$emp_name_areatype ,
															'loc_emp_vill_ward'							=>	$loc_emp_vill_ward,
															'l_emp_ps'									=>	$l_emp_ps,
															'loc_emp_pin_number'						=>  $loc_emp_pin_number,
															
															'full_name_manager'							=>	$full_name_manager, 
															'address_manager'							=>	$address_manager,
															
															'loc_manager_country'						=>  $loc_manager_country,
															'loc_manager_state'							=>  $loc_manager_state,
															'loc_manager_dist'							=>	$loc_manager_dist,
															'loc_manager_subdivision'					=>	$loc_manager_subdivision ,
															'loc_manager_areatype'						=>	$loc_manager_areatype,
															'manager_name_areatype'						=>	$manager_name_areatype ,
															'loc_manager_vill_ward'						=>	$loc_manager_vill_ward,
															'l_manager_ps'								=>	$l_manager_ps,
															'loc_manager_pin_number'					=>  $loc_manager_pin_number,
															
															'max_num_wrkmen'							=>	$max_num_wrkmen, 
															'e_num_of_workmen_per_or_reg'				=>	$e_num_of_workmen_per_or_reg, 
															'e_num_of_workmen_temp_or_reg'				=>	$e_num_of_workmen_temp_or_reg, 
															'workmen_if_same_similar_kind_of_work'		=>	$workmen_if_same_similar_kind_of_work, 
															'con_lab_job_desc'							=>	$con_lab_job_desc, 
															'con_lab_wage_rate_other_benefits'			=>	$con_lab_wage_rate_other_benefits, 
															'con_lab_cat_desig_nom'						=>	$con_lab_cat_desig_nom ,
															'e_settlement_award_judgement_min_wage'		=>  $e_settlement_award_judgement_min_wage,															
															'e_any_day_max_num_of_workmen'				=>	$e_any_day_max_num_of_workmen, 
															
														);
		
		
									
		if (!empty($fieldsRegInfo)) {
			
					$application_id = db_insert('l_clra_registration_master')->fields($fieldsRegInfo)->execute();
									 
					
					
					if(!empty($e_nature_of_work)){
						foreach($e_nature_of_work as $LoopValue){
							  $fieldsEstContractorInfoNatureOfWork 				= 	array( 
																						'identification_number'	=>	$identification_number,
																						'user_id'				=>	$user->uid,
																						'act_id'				=>  $act_id,
																						'application_id'		=>	$application_id,
																						'e_nature_of_work'		=>	$LoopValue
																						);
																						
							  db_insert('l_clra_reg_nature_of_work')->fields($fieldsEstContractorInfoNatureOfWork)->execute();
							  $increment++;
						}
			  	    }
					
					
					
					if(!empty($other_nature_of_work_value)) {
						

							 $ContractorOthersNatureOfWork 					= 	array( 
																						'application_id'		=>	$application_id,
																						'cont_work_name'		=>	ucwords($other_nature_of_work_value),
																						'act_id'				=>  $act_id,
																						'is_default'			=>  0
																						);
																						
							 $last_insert_other_work_id = db_insert('contractor_works')->fields($ContractorOthersNatureOfWork)->execute();
							 
							 $fieldsEstContractorInfoNatureOfWork 				= 	array( 
																						'identification_number'	=>	$identification_number,
																						'user_id'				=>	$user->uid,
																						'act_id'				=>  $act_id,
																						'application_id'		=>	$application_id,
																						'e_nature_of_work'		=>	$last_insert_other_work_id
																						);
																						
							  db_insert('l_clra_reg_nature_of_work')->fields($fieldsEstContractorInfoNatureOfWork)->execute();
						
					}
					
					
					$fieldsDocumentsData 			= array( 
														'user_id'									=>	$user->uid,
														'act_id'									=>  $act_id,
														'identification_number'						=>	$identification_number,
														'trade_license_file' 						=>  $trade_license_file,
														'article_of_assoc_file' 					=>  $article_of_assoc_file,
														'memorandum_of_cert_file' 					=>  $memorandum_of_cert_file,
														'partnership_deed_file' 					=>  $partnership_deed_file,
														'factory_license_file'						=>	$factory_license_file,
														'application_id'							=>	$application_id,
													);
													
													
					if(!empty($fieldsDocumentsData)) {
													
						db_insert('l_documents_upload')->fields($fieldsDocumentsData)->execute();
					
					}
					
					/*$message													=	"Application saved successfully ";
					drupal_set_message($message);*/
					
					if($application_id != ""){
						drupal_goto('view-clra-application-details/view-clra-application/'.encryption_decryption_fun($action_encrypt, $application_id));
					}
		}



		
}



function get_ajax_callback_new_1($form, $form_state){
	$commands 	= array();
	$commands[] = ajax_command_replace('#clra_subdvsn_replace', drupal_render($form['clra_reg_master']['loc_e_subdivision']));
	$commands[] = ajax_command_replace('#clra_loc_e_areatype', drupal_render($form['clra_reg_master']['loc_e_areatype']));
	$commands[] = ajax_command_replace('#clra_common_1_replace', drupal_render($form['clra_reg_master']['name_areatype']));
	$commands[] = ajax_command_replace('#clra_common_2_replace', drupal_render($form['clra_reg_master']['loc_e_vill_ward']));
	$commands[] = ajax_command_replace('#clra_ps_replace', drupal_render($form['clra_reg_master']['loc_e_ps']));
  	return array('#type' => 'ajax', '#commands' => $commands);
}

function get_ajax_callback_new_2($form, $form_state){
	$commands 	= array();
	$commands[] = ajax_command_replace('#clra_subdvsn_replace_1', drupal_render($form['clra_reg_master']['e_postal_subdivision']));
	$commands[] = ajax_command_replace('#clra_loc_e_areatype_1', drupal_render($form['clra_reg_master']['e_postal_areatype']));
	$commands[] = ajax_command_replace('#clra_common_1_replace_1', drupal_render($form['clra_reg_master']['e_postal_name_areatype']));
	$commands[] = ajax_command_replace('#clra_common_2_replace_1', drupal_render($form['clra_reg_master']['e_postal_vill_ward']));
	$commands[] = ajax_command_replace('#clra_ps_replace_1', drupal_render($form['clra_reg_master']['e_postal_ps']));
  	return array('#type' => 'ajax', '#commands' => $commands);
}


function get_ajax_country_emp ($form, $form_state){
	$commands[] = ajax_command_replace('#add_line_replace', drupal_render($form['clra_reg_master']['address_principal_emp']));
	$commands[] = ajax_command_replace('#clra_emp_state_replace', drupal_render($form['clra_reg_master']['emp_state']));
	$commands[] = ajax_command_replace('#clra_emp_district_replace', drupal_render($form['clra_reg_master']['emp_dist']));
	$commands[] = ajax_command_replace('#clra_subdvsn_replace_2', drupal_render($form['clra_reg_master']['loc_emp_subdv']));
	$commands[] = ajax_command_replace('#clra_loc_e_areatype_2', drupal_render($form['clra_reg_master']['loc_emp_areatype']));
	$commands[] = ajax_command_replace('#clra_common_1_replace_2', drupal_render($form['clra_reg_master']['emp_name_areatype']));
	$commands[] = ajax_command_replace('#clra_common_2_replace_2', drupal_render($form['clra_reg_master']['loc_emp_vill_ward']));
	$commands[] = ajax_command_replace('#clra_ps_replace_2', drupal_render($form['clra_reg_master']['loc_emp_ps']));
	$commands[] = ajax_command_replace('#clra_pin_replace_2', drupal_render($form['clra_reg_master']['loc_emp_pin_number']));
	return array('#type' => 'ajax', '#commands' => $commands);
}
		
function get_ajax_callback_new_3($form, $form_state){ 

	$commands 	= array();
	//$commands[] = ajax_command_replace('#add_line_replace', drupal_render($form['clra_reg_master']['address_principal_emp']));
	$commands[] = ajax_command_replace('#clra_emp_state_replace', drupal_render($form['clra_reg_master']['emp_state']));
	$commands[] = ajax_command_replace('#clra_emp_district_replace', drupal_render($form['clra_reg_master']['emp_dist']));
	$commands[] = ajax_command_replace('#clra_subdvsn_replace_2', drupal_render($form['clra_reg_master']['loc_emp_subdv']));
	$commands[] = ajax_command_replace('#clra_loc_e_areatype_2', drupal_render($form['clra_reg_master']['loc_emp_areatype']));
	$commands[] = ajax_command_replace('#clra_common_1_replace_2', drupal_render($form['clra_reg_master']['emp_name_areatype']));
	$commands[] = ajax_command_replace('#clra_common_2_replace_2', drupal_render($form['clra_reg_master']['loc_emp_vill_ward']));
	$commands[] = ajax_command_replace('#clra_ps_replace_2', drupal_render($form['clra_reg_master']['loc_emp_ps']));
	$commands[] = ajax_command_replace('#clra_pin_replace_2', drupal_render($form['clra_reg_master']['loc_emp_pin_number']));
  	return array('#type' => 'ajax', '#commands' => $commands);
}

function get_ajax_country_manager ($form, $form_state){
	
	$commands[] = ajax_command_replace('#clra_manager_address_replace', drupal_render($form['clra_reg_master']['address_manager']));
	$commands[] = ajax_command_replace('#clra_manager_state_replace', drupal_render($form['clra_reg_master']['manager_state']));
	$commands[] = ajax_command_replace('#clra_manager_district_replace', drupal_render($form['clra_reg_master']['manager_dist']));
	$commands[] = ajax_command_replace('#clra_subdvsn_replace_3', drupal_render($form['clra_reg_master']['loc_manager_subdv']));
	$commands[] = ajax_command_replace('#clra_loc_e_areatype_3', drupal_render($form['clra_reg_master']['loc_manager_areatype']));
	$commands[] = ajax_command_replace('#clra_common_1_replace_3', drupal_render($form['clra_reg_master']['manager_name_areatype']));
	$commands[] = ajax_command_replace('#clra_common_2_replace_3', drupal_render($form['clra_reg_master']['loc_manager_vill_ward']));
	$commands[] = ajax_command_replace('#clra_ps_replace_3', drupal_render($form['clra_reg_master']['loc_manager_ps']));
	$commands[] = ajax_command_replace('#clra_manager_pin_replace', drupal_render($form['clra_reg_master']['loc_manager_pin_number']));
	return array('#type' => 'ajax', '#commands' => $commands);
}


function get_ajax_callback_new_4($form, $form_state){
	$commands 	= array();
	$commands[] = ajax_command_replace('#clra_manager_state_replace', drupal_render($form['clra_reg_master']['manager_state']));
	$commands[] = ajax_command_replace('#clra_manager_address_replace', drupal_render($form['clra_reg_master']['address_manager']));
	$commands[] = ajax_command_replace('#clra_manager_district_replace', drupal_render($form['clra_reg_master']['manager_dist']));
	$commands[] = ajax_command_replace('#clra_subdvsn_replace_3', drupal_render($form['clra_reg_master']['loc_manager_subdv']));
	$commands[] = ajax_command_replace('#clra_loc_e_areatype_3', drupal_render($form['clra_reg_master']['loc_manager_areatype']));
	$commands[] = ajax_command_replace('#clra_common_1_replace_3', drupal_render($form['clra_reg_master']['manager_name_areatype']));
	$commands[] = ajax_command_replace('#clra_common_2_replace_3', drupal_render($form['clra_reg_master']['loc_manager_vill_ward']));
	$commands[] = ajax_command_replace('#clra_ps_replace_3', drupal_render($form['clra_reg_master']['loc_manager_ps']));
	$commands[] = ajax_command_replace('#clra_manager_pin_replace', drupal_render($form['clra_reg_master']['loc_manager_pin_number']));
  	return array('#type' => 'ajax', '#commands' => $commands);
}

function get_ajax_callback_new_5($form, $form_state){
	$commands = array();
	$commands[] = ajax_command_replace('#clra_subdvsn_replace_4', drupal_render($form['clra_reg_master']['trade_union_subdv']));
	$commands[] = ajax_command_replace('#clra_loc_e_areatype_4', drupal_render($form['clra_reg_master']['trade_union_areatype']));
	$commands[] = ajax_command_replace('#clra_common_1_replace_4', drupal_render($form['clra_reg_master']['e_trade_union_name_areatype']));
	$commands[] = ajax_command_replace('#clra_common_2_replace_4', drupal_render($form['clra_reg_master']['e_trade_union_vill_ward']));
	$commands[] = ajax_command_replace('#clra_ps_replace_4', drupal_render($form['clra_reg_master']['trade_union_ps']));
  	return array('#type' => 'ajax', '#commands' => $commands);
}
















