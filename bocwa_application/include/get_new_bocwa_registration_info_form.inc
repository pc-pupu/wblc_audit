<?php

function get_new_bocwa_registration_info_form($form, &$form_state){ 
	
	global $base_root,$base_path,$user;
			
	$user_id 								= $user->uid;
	drupal_add_library('system','ui.datepicker');
	drupal_add_js(drupal_get_path('module', 'bocwa_application') . '/js/bocwa_upload_custom.js');
	$options_dist 	= common_custom_user_district_list_new();
	$applicant_details	 	= db_select('l_common_application_master', 'lcam');
	$applicant_details->fields('lcam', array());
	$applicant_details->condition('lcam.user_id', $user_id);
	$get_profile_status 	= $applicant_details->execute();
	if( $get_profile_status->rowCount() > 0 ){
				
		$obj 					= 	$get_profile_status->fetchAssoc(); 
	
		$f_name					=	$obj['f_name'];
		$m_name					=	$obj['m_name'];
		$l_name					=	$obj['l_name'];
		$address				=	$obj['address'];
		$est_type 				= 	$obj['est_type'];
		$district_code			=	trim($obj['dist_code']);
		$subdivision_code		=	trim($obj['subdivision']);
		$areatype_code			=	trim($obj['areatype']);
		$name_areatype_code		=	trim($obj['name_areatype']);
		$vill_ward_code			=	trim($obj['vill_ward']);
		$ps_code				=	trim($obj['ps']);
		$pin					=	trim($obj['pin']);
		$gender					=   trim($obj['gender']);
		
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
		
		
		$form['new_bocwa_reg']['header']				= array(
															'#markup' 		=>	 t('<div class="content">'),
																);
																
																
		//if($user_id==64){	 
			$form['new_bocwa_reg']['silpasathi_url'] 		= array(
																		'#prefix' => ' <div class="panel panel-default panel-form"><div class="panel-body"><div class="sky-form"><div class="row"><section class="col-lg-6 col-md-12">',
																		'#suffix' => '</section></div></div></div></div></div>',																
																		'#type' => 'markup',																
																		'#markup'  => 'You will be redirected to Shilpa Sathi Portal (https://silpasathi.wb.gov.in) to avail this service. Click on <a href="https://silpasathi.wb.gov.in"><strong>CONTINUE</strong></a> to proceed'
																	);
		
																	
																	
			return $form;														
		//}			
	
		

		$form['new_bocwa_reg']['bocwa_act_id'] 		   = array(
															'#title'		=>	'Act Id',
															'#size'			=>	t('20'),	
															'#type' 		=> 	'hidden',
															'#default_value'=> 	2,
															'#attributes'	=> 	array('readonly'=>'readonly')
														);
																					
		
		$form['new_bocwa_reg']['e_name'] 			   = array(
														  '#prefix' 		=>  '<div class="panel panel-default panel-form"><div class="panel-heading">1. Name and location (Work Site) of the Establishment</div><div class="panel-body"><div class="col-lg-4 col-md-6"><label class="input">',
														  '#suffix' 		=>  '</label></div>',
														  '#title'			=>  'Name of the Establishment',
														  '#default_value' 	=>  !empty($obj['est_name']) ? $obj['est_name'] : '',
														  '#type' 			=>  'textfield',
														  '#required' 		=>  TRUE,
														  '#element_validate'=>  array('BOCWA_SPECIAL_CHAR_TEXT_VALIDATE')
													);	
				$form['new_bocwa_reg']['est_type'] 			   = array(
																	'#title' 		=> 'Establishment Type',
																	'#type' 		=> 'select',
																	'#options' 		=> array('micro' => 'Micro', 'small' => 'Small', 'medium' => 'Medium', 'large' => 'Large'),
																	'#required' 	=>  TRUE,
																	'#default_value'=> $est_type,
																	'#prefix' 		=> '<div class="col-md-4"><label class="select">',
																	'#suffix' 		=> '</label></div>'
																);
				$form['new_bocwa_reg']['loc_e_name'] 		   = array(
																  '#prefix' 		=>  '<div class="col-lg-4 col-md-6"><label class="input">',
																  '#suffix' 		=>  '</label></div>',
																  '#title'			=>  'Location of the Establishment',
																  '#default_value' 	=>  !empty($obj['est_loc']) ? $obj['est_loc'] : '',
																  '#type' 			=>  'textfield',
																  '#required' 		=>  TRUE,
																  '#element_validate'=> array('BOCWA_SPECIAL_CHAR_TEXTAREA_VALIDATE')
															);
				$form['new_bocwa_reg']['loc_e_dist'] 		  = array(
																  '#prefix' 		=> '<div class="col-lg-4 col-md-6"><label class="select">',
																  '#suffix' 		=> '<i></i></label></div>',
																  '#title'			=> 'Select District',
																  '#type' 			=> 'select',
																  '#options' 		=> $options_dist,
																  '#default_value' 	=> !empty($obj['est_district']) ? $obj['est_district']:'',
																  '#required' 		=> TRUE,
																  '#ajax'			=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																						'progress'			=>	array('type' => 'throbber'),
																						  )
																);	
				$value_loc_e_dist 	= 	isset($form_state['values']['loc_e_dist']) ? $form_state['values']['loc_e_dist'] : $obj['est_district'];	
				
				if($value_loc_e_dist != "" || !empty($value_loc_e_dist) || is_numeric($value_loc_e_dist)){
					$form['new_bocwa_reg']['loc_e_subdivision'] = array(
																  '#prefix' 		=> '<div id="bocwa_subdvsn_replace" class="col-lg-4 col-md-6"><label class="select">',
																  '#suffix' 		=> '<i></i></label></div>',
																  '#title'			=> 'Select Subdivision',
																  '#type' 			=> 'select',
																  '#options' 		=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist),
																  '#default_value' 	=>!empty($obj['est_subdivision']) ? $obj['est_subdivision'] : '',
																  '#required' 		=> TRUE,
																  '#attributes' 	=> array('onkeyup' => 'getSpanText()', 'onblur' => 'getSpanText()', 'onchange' => 'getSpanText()'),
																  '#id'				=> 'loc_e_subdivision_id',
																  '#ajax'			=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																						'progress'			=>	array('type' => 'throbber'),
																				  ),
															);	
					
				}else{
					$form['new_bocwa_reg']['loc_e_subdivision'] = array(
																  '#prefix' 		=> '<div id="bocwa_subdvsn_replace" class="col-lg-4 col-md-6"><label class="select">',
																  '#suffix' 		=> '<i></i></label></div>',
																  '#title'			=> 'Select Subdivision',
																  '#type' 			=> 'select',
																  '#options' 		=> common_custom_user_subdivisionlist_list_new($obj['est_district']),
																  '#attributes' 	=> array('onkeyup' => 'getSpanText()', 'onblur' => 'getSpanText()', 'onchange' => 'getSpanText()'),
																  '#default_value' 	=> !empty($obj['est_subdivision']) ? $obj['est_subdivision'] : '',
																  '#id'				=> 'loc_e_subdivision_id',
																  '#required' 		=> TRUE,
																  '#ajax'			=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																						'progress'			=>	array('type' => 'throbber'),
																					  ),
															);
				}
				$value_loc_e_subdivision = isset($form_state['values']['loc_e_subdivision']) ? $form_state['values']['loc_e_subdivision'] : $obj['est_subdivision'];
				
				if($value_loc_e_dist != "" && $value_loc_e_subdivision != ""){
				
					$areatypeOptions = common_custom_user_areatype_list_new($value_loc_e_dist, $value_loc_e_subdivision);
					if(!empty($areatypeOptions)){
						$form['new_bocwa_reg']['loc_e_areatype']  = array(
																  '#prefix' 		=> '<div id="bocwa_loc_e_areatype" class="col-lg-4 col-md-6"><label class="select">',
																  '#suffix' 		=> '<i></i></label></div>',
																  '#title'			=> 'Select Areatype',
																  '#type' 			=> 'select',
																  '#options' 		=> $areatypeOptions,
																  '#default_value'	=> !empty($obj['est_loc_e_areatype']) ? $obj['est_loc_e_areatype'] : '',
																  '#required' 		=> TRUE,
																  '#id'				=> 'loc_e_areatype_id',
																  '#ajax'			=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																						'progress'			=>	array('type' => 'throbber'),
																					  ),
																	);
					}
					}else{
				
						$form['new_bocwa_reg']['loc_e_areatype'] = array(
																  '#prefix' 		=> '<div id="bocwa_loc_e_areatype" class="col-lg-4 col-md-6"><label class="select">',
																  '#suffix' 		=> '<i></i></label></div>',
																  '#title'			=> 'Select Areatype',
																  '#type' 			=> 'select',
																  '#options' 		=> common_custom_user_areatype_list_new($obj['est_district'], $obj['est_subdivision']),
																  '#default_value'	=> !empty($obj['est_loc_e_areatype']) ? $obj['est_loc_e_areatype'] : '',
																  '#required' 		=> TRUE,
																  '#ajax'			=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																						'progress'			=>	array('type' => 'throbber'),
																					  ),
																	);
				}
			
				$value_loc_e_areatype 	= isset($form_state['values']['loc_e_areatype']) ? $form_state['values']['loc_e_areatype'] : $obj['est_loc_e_areatype'];
				
				$form['new_bocwa_reg']['loc_e_areatype_code'] = array(
																 '#prefix' 				=> '<div id="bocwa_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
																 '#suffix' 				=> '<i></i></label></div>',
																 '#title'				=> 'Select Block / Municipality / Corporation / SEZ / Notified Area',
																 '#type' 				=> 'select',
																 '#options' 			=> common_custom_user_block_list_new($obj['est_subdivision'], $obj['est_loc_e_areatype']),
																 '#default_value'		=> !empty($obj['est_name_areatype']) ? $obj['est_name_areatype'] : '',
																 '#required' 			=> TRUE,
																 '#ajax'				=> array(
																							'event' 			=> 'change',
																							'effect' 			=> 'fade',			
																							'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																							'progress'			=>	array('type' => 'throbber'),
																						  )
															  );
				if($value_loc_e_dist != "" && $value_loc_e_subdivision != ""){
					if($value_loc_e_areatype != ""){
						if($value_loc_e_areatype == 'B'){
							$form['new_bocwa_reg']['loc_e_areatype_code'] = array(
																  '#prefix' 			=>'<div id="bocwa_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
																  '#suffix' 			=> '<i></i></label></div>',
																  '#title'				=> 'Select Block',
																  '#type' 				=> 'select',
																  '#options' 			=> common_custom_user_block_list_new($value_loc_e_subdivision, $value_loc_e_areatype),
																  '#default_value'		=> !empty($obj['est_name_areatype']) ? $obj['est_name_areatype'] : '',
																  '#required' 			=> TRUE,
																  '#ajax'				=> array(
																							'event' 			=> 'change',
																							'effect' 			=> 'fade',			
																							'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																							'progress'			=> '',
																						  )
																);
						}else if($value_loc_e_areatype == 'M'){
						
							$form['new_bocwa_reg']['loc_e_areatype_code'] 				= array(
																							  '#prefix' 			=> '<div id="bocwa_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Municipality',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_loc_e_subdivision, $value_loc_e_areatype),
																							  '#default_value'		=> !empty($obj['est_name_areatype']) ? $obj['est_name_areatype'] : '',
																							  '#required' 			=> TRUE,
																							  '#id'					=> 'common_selection_1_id',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																														'progress'			=> '',
																													  )
																							);
						}else if($value_loc_e_areatype == 'C'){
				
							$form['new_bocwa_reg']['loc_e_areatype_code'] 				= array(
																							  '#prefix' 			=> '<div id="bocwa_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Corporation',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_loc_e_subdivision,$value_loc_e_areatype),
																							  '#default_value'		=> !empty($obj['est_name_areatype']) ? $obj['est_name_areatype'] : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																														'progress'			=> '',
																													  )
																						);
						}else if($value_loc_e_areatype == 'S'){
				
							$form['new_bocwa_reg']['loc_e_areatype_code'] 				= array(
																							  '#prefix' 			=> '<div id="bocwa_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select SEZ',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_loc_e_subdivision, $value_loc_e_areatype),
																							  '#default_value'		=> !empty($obj['est_name_areatype']) ? $obj['est_name_areatype'] : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																														'progress'			=> '',
																													  )
																							);
						}else if($value_loc_e_areatype == 'N'){
						
							$form['new_bocwa_reg']['loc_e_areatype_code'] 				= array(
																							  '#prefix' 			=> '<div id="bocwa_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Notified Area',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_loc_e_subdivision, $value_loc_e_areatype),
																							  '#default_value'		=> !empty($obj['est_name_areatype']) ? $obj['est_name_areatype'] : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																														'progress'			=> '',
																													  )
																							);
						}
					}
				}
				$value_common_selection_1 = isset($form_state['values']['loc_e_areatype_code']) ? $form_state['values']['loc_e_areatype_code'] : $obj['est_name_areatype'];
			
				$form['new_bocwa_reg']['loc_e_vill_ward'] 	= array(
																 '#prefix' 				=> '<div id="bocwa_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																 '#suffix' 				=> '<i></i></label></div>',
																 '#title'				=> 'Select Gram Panchayat/Ward',
																 '#type' 				=> 'select',
																 '#options' 			=> array(),
																 '#required' 			=> TRUE,
																 '#ajax'				=> array(
																							'event' 			=> 'change',
																							'effect' 			=> 'fade',			
																							'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																							'progress'			=> '',
																						  )
															  );
															  
				if($value_loc_e_dist != "" && $value_loc_e_subdivision != ""){
					if($value_loc_e_areatype != ""){
						if($value_common_selection_1 != ""){
							if($value_loc_e_areatype == 'B'){
									
									$form['new_bocwa_reg']['loc_e_vill_ward'] 			= array(
																							  '#prefix' 			=> '<div id="bocwa_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Gram Panchayat',
																							  '#type' 				=> 'select',
																							  '#options'			=> common_custom_user_villward_list_new($value_common_selection_1,'V'),
																							  '#default_value'		=> !empty($obj['est_loc_e_vill_ward']) ? $obj['est_loc_e_vill_ward'] : '',
																							  '#required' 			=> TRUE,
																							  '#id'					=> 'common_selection_2_id',
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																															'progress'			=> '',
																														  )
																							);	
							}
								
							if($value_loc_e_areatype == 'M'){
									
									$form['new_bocwa_reg']['loc_e_vill_ward'] 			= array(
																							  '#prefix' 			=> '<div id="bocwa_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Ward',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_villward_list_new($value_common_selection_1,'W'),
																							  '#default_value'		=> !empty($obj['est_loc_e_vill_ward']) ? $obj['est_loc_e_vill_ward'] : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																															'progress'			=> '',
																														  )
																							);
																									
							}
								
							if($value_loc_e_areatype == 'C'){
									
									$form['new_bocwa_reg']['loc_e_vill_ward'] 			= array(
																							  '#prefix' 			=> '<div id="bocwa_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Ward',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_corporation_list_new($value_common_selection_1),
																							  '#default_value'		=> !empty($obj['est_loc_e_vill_ward']) ? $obj['est_loc_e_vill_ward'] : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																														'progress'			=> '',
																													  )
																							);
							}
							
							if($value_loc_e_areatype == 'S'){
									
									$form['new_bocwa_reg']['loc_e_vill_ward'] 			= array(
																							  '#prefix' 			=> '<div id="bocwa_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Sector',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_villward_list_new($value_common_selection_1,'S'),
																							  '#default_value'		=> !empty($obj['est_loc_e_vill_ward']) ? $obj['est_loc_e_vill_ward'] : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																														'progress'			=> '',
																													  )
																							);
							}
							
							if($value_loc_e_areatype == 'N'){
									
									$form['new_bocwa_reg']['loc_e_vill_ward'] = array(
																	  '#prefix' 			=> '<div id="bocwa_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																	  '#suffix' 			=> '<i></i></label></div>',
																	  '#title'				=> 'Select Notified Area',
																	  '#type' 				=> 'select',
																	  '#options' 			=> common_custom_user_villward_list_new($value_common_selection_1,'N'),
																	  '#default_value'		=> !empty($obj['est_loc_e_vill_ward']) ? $obj['est_loc_e_vill_ward'] : '',
																	  '#required' 			=> TRUE,
																	  '#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																								'progress'			=> '',
																							  )
																	);
							}
						}
					}
				}
			
				$value_common_selection_2 	= isset($form_state['values']['loc_e_vill_ward']) ? $form_state['values']['loc_e_vill_ward'] : $obj['est_loc_e_vill_ward'];
				$form['new_bocwa_reg']['loc_e_ps'] 			= array(
																'#prefix' 				=> '<div id="bocwa_ps_replace" class="col-lg-4 col-md-6"><label class="select">',
																'#suffix' 				=> '<i></i></label></div>',
																'#title'				=> 'Select Police Station',
																'#type' 				=> 'select',
																'#options' 				=> common_custom_user_ps_list_new($obj['est_district']),
																'#default_value'		=> !empty($obj['est_ps']) ? $obj['est_ps'] : '',
																'#required' 			=> TRUE,
																'#ajax'					=> array(
																							'event' 			=> 'change',
																							'effect' 			=> 'fade',			
																							'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																							'progress'			=> '',
																						  )
														  );
			
				if($value_loc_e_dist != "" && $value_loc_e_subdivision != ""){
					if($value_loc_e_areatype != ""){
						if($value_common_selection_1 != ""){
							if($value_common_selection_2 != ""){
								$form['new_bocwa_reg']['loc_e_ps'] = array(
																  '#prefix' 			=> '<div id="bocwa_ps_replace" class="col-lg-4 col-md-6"><label class="select">',
																  '#suffix' 			=> '<i></i></label></div>',
																  '#title'				=> 'Select Police Station',
																  '#type' 				=> 'select',
																  '#id'					=> 'loc_e_ps_id',
																  '#options' 			=> common_custom_user_ps_list_new($value_loc_e_dist),
																  '#default_value'		=> !empty($obj['est_ps']) ? $obj['est_ps'] : '',
																  '#required' 			=> TRUE
															  );
							}
						}
					}
				}
			
			
				$form['new_bocwa_reg']['loc_e_pin_number'] 	= array(
																  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																  '#suffix' 			=> '</label></div></div></div>',
																  '#title'				=> 'PIN Code',
																  '#type' 				=> 'textfield',
																  '#default_value'		=> !empty($obj['est_pin_number']) ? $obj['est_pin_number'] : '',
																  '#attributes' 		=> array('maxlength' => 6),
																  '#required' 			=> TRUE
															  );
															  
				$form['new_bocwa_reg']['e_postal_address'] 	= array(
																  '#prefix' 			=> '<div class="panel panel-default panel-form"><div class="panel-heading">2. Registered Office address of the Establishment</div><div class="panel-body"><div class="col-lg-4 col-md-6"><label class="textarea">',
																  '#suffix' 			=> '</label></div>',
																  '#title'				=> 'House No./Village Name/Street/Road',
																  '#rows'				=> t('2'),	
																  '#type' 				=> 'textarea',
																  '#required' 			=> TRUE,
																  '#element_validate'	=> array('BOCWA_SPECIAL_CHAR_TEXTAREA_VALIDATE'),
																  '#attributes' 		=> array('style'=>"height: 118px;", ),  
															);
									
				$form['new_bocwa_reg']['e_postal_dist'] 	= array(
																  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="select">',
																  '#suffix' 			=> '</label></div>',
																  '#title'				=> 'District of the Establishment',
																  '#type' 				=> 'select',
																  '#options' 			=> $options_dist,
																  '#required' 			=> TRUE,
																  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'), 
																  '#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																								'progress'			=>	array('type' => 'throbber'),
																							  )
																);
				$value_e_postal_dist = 	isset($form_state['values']['e_postal_dist']) ? $form_state['values']['e_postal_dist'] : '';
				
				if($value_e_postal_dist != "" || !empty($value_e_postal_dist) || is_numeric($value_e_postal_dist)){ 
				
					$form['new_bocwa_reg']['e_postal_subdivision'] = array(
																  '#prefix' 			=> '<div id="bocwa_e_postal_subdvsn_replace" class="col-lg-4 col-md-6"><label class="select">',
																  '#suffix' 			=> '<i></i></label></div>',
																  '#title'				=> 'Select Subdivision',
																  '#type' 				=> 'select',
																  '#options' 			=> common_custom_user_subdivisionlist_list_new($value_e_postal_dist),
																  '#required' 			=> TRUE,
																  '#ajax'				=> array(
																							'event' 			=> 'change',
																							'effect' 			=> 'fade',			
																							'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																							'progress'			=>	array('type' => 'throbber'),
																						  ),
															);
				}else{
					
					$form['new_bocwa_reg']['e_postal_subdivision'] 	= array(
																	  '#prefix' 			=> '<div id="bocwa_e_postal_subdvsn_replace" class="col-lg-4 col-md-6"><label class="select">',
																	  '#suffix' 			=> '<i></i></label></div>',
																	  '#title'				=> 'Select Subdivision',
																	  '#type' 				=> 'select',
																	  '#options' 			=> array(),
																	  '#required' 			=> TRUE,
																	  '#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																								'progress'			=>	array('type' => 'throbber'),
																							  ),
																);
				}
											
											
				$value_e_postal_subdivision = isset($form_state['values']['e_postal_subdivision']) ? $form_state['values']['e_postal_subdivision'] : '';
				if($value_e_postal_dist != "" && $value_e_postal_subdivision != ""){
					$areatypeOptions_1	= common_custom_user_areatype_list_new($value_e_postal_dist, $value_e_postal_subdivision);
					if(!empty($areatypeOptions_1)){
						$form['new_bocwa_reg']['e_postal_areatype'] = array(
															  '#prefix' 			=> '<div id="bocwa_e_postal_loc_e_areatype" class="col-lg-4 col-md-6"><label class="select">',
															  '#suffix' 			=> '<i></i></label></div>',
															  '#title'				=> 'Select Areatype',
															  '#type' 				=> 'select',
															  '#options' 			=> $areatypeOptions_1,
															  '#required' 			=> TRUE,
															  '#ajax'				=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																						'progress'			=>	array('type' => 'throbber'),
																					  ),
																);
																								
					}
				}else{
					
					$form['new_bocwa_reg']['e_postal_areatype'] = array(
															  '#prefix' 			=> '<div id="bocwa_e_postal_loc_e_areatype" class="col-lg-4 col-md-6"><label class="select">',
															  '#suffix' 			=> '<i></i></label></div>',
															  '#title'				=> 'Select Areatype',
															  '#type' 				=> 'select',
															  '#options' 			=> array(),
															  '#required' 			=> TRUE,
															  '#id'					=> 'loc_e_areatype_id_1',
															  '#ajax'				=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_ajax_callback_new_2',
																						'progress'			=>	array('type' => 'throbber'),
																					  ),
																);
				}
				$value_e_postal_areatype  = isset($form_state['values']['e_postal_areatype']) ? $form_state['values']['e_postal_areatype'] : '';
				
				$form['new_bocwa_reg']['e_postal_areatype_code'] = array(
																 '#prefix' 				=> '<div id="bocwa_e_postal_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
																 '#suffix' 				=> '<i></i></label></div>',
																 '#title'				=> 'Select Block / Municipality / Corporation / SEZ / Notified Area',
																 '#type' 				=> 'select',
																 '#options' 			=> array(),
																 '#required' 			=> TRUE,
																 '#ajax'				=> array(
																							'event' 			=> 'change',
																							'effect' 			=> 'fade',			
																							'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																							'progress'			=>	array('type' => 'throbber'),
																						  )
															  );
															  
				if($value_e_postal_dist != "" && $value_e_postal_subdivision != ""){
					if($value_e_postal_areatype != ""){
						if($value_e_postal_areatype == 'B'){
							$form['new_bocwa_reg']['e_postal_areatype_code'] 			= array(
																							  '#prefix' 			=> '<div id="bocwa_e_postal_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Block',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_e_postal_subdivision, $value_e_postal_areatype),
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																														'progress'			=> '',
																													  )
																							);
						}else if($value_e_postal_areatype == 'M'){
					
							$form['new_bocwa_reg']['e_postal_areatype_code']  			= array(
																							  '#prefix' 			=> '<div id="bocwa_e_postal_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Municipality',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_e_postal_subdivision, $value_e_postal_areatype),
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																														'progress'			=> '',
																													  )
																							);
						}else if($value_e_postal_areatype == 'C'){
			
							$form['new_bocwa_reg']['e_postal_areatype_code'] 			= array(
																							  '#prefix' 			=> '<div id="bocwa_e_postal_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Corporation',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_e_postal_subdivision, $value_e_postal_areatype),
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																														'progress'			=> '',
																													  )
																						);
						}else if($value_e_postal_areatype == 'S'){
			
							$form['new_bocwa_reg']['e_postal_areatype_code'] 			= array(
																							  '#prefix' 			=> '<div id="bocwa_e_postal_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select SEZ',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_e_postal_subdivision, $value_e_postal_areatype),
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																														'progress'			=> '',
																													  )
																							);
						}else if($value_e_postal_areatype == 'N'){
					
							$form['new_bocwa_reg']['e_postal_areatype_code'] 			= array(
																							  '#prefix' 			=> '<div id="bocwa_e_postal_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Notified Area',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_e_postal_subdivision, $value_e_postal_areatype),
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																														'progress'			=> '',
																													  )
																							);
						}
					}
				}
				
				$value_e_postal_areatype_code = isset($form_state['values']['e_postal_areatype_code']) ? $form_state['values']['e_postal_areatype_code'] : '';
				
				$form['new_bocwa_reg']['e_postal_vill_ward'] 							= array(
																							 '#prefix' 				=> '<div id="bocwa_e_postal_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							 '#suffix' 				=> '<i></i></label></div>',
																							 '#title'				=> 'Select Gram Panchayat/Ward',
																							 '#type' 				=> 'select',
																							 '#options' 			=> array(),
																							 '#required' 			=> TRUE,
																							 '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																														'progress'			=>	array('type' => 'throbber'),
																													  )
																						  );
				
				if($value_e_postal_dist != "" && $value_e_postal_subdivision != ""){
					if($value_e_postal_areatype != ""){
						if($value_e_postal_areatype_code != ""){
							if($value_e_postal_areatype == 'B'){
								
								$form['new_bocwa_reg']['e_postal_vill_ward'] 			= array(
																							  '#prefix' 			=> '<div id="bocwa_e_postal_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Gram Panchayat',
																							  '#type' 				=> 'select',
																							  '#options'			=> common_custom_user_villward_list_new($value_e_postal_areatype_code,'V'),
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																															'progress'			=> '',
																														  )
																							);	
							}
							
							if($value_e_postal_areatype == 'M'){
								
								$form['new_bocwa_reg']['e_postal_vill_ward'] 			= array(
																							  '#prefix' 			=> '<div id="bocwa_e_postal_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Ward',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_villward_list_new($value_e_postal_areatype_code,'W'),
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																															'progress'			=> '',
																														  )
																							);
																								
							}
							
							if($value_e_postal_areatype == 'C'){
								
								$form['new_bocwa_reg']['e_postal_vill_ward'] 			= array(
																							  '#prefix' 			=> '<div id="bocwa_e_postal_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Ward',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_corporation_list_new($value_e_postal_areatype_code),
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																														'progress'			=> '',
																													  )
																							);
							}
							
							if($value_e_postal_areatype == 'S'){
								
								$form['new_bocwa_reg']['e_postal_vill_ward'] 			= array(
																							  '#prefix' 			=> '<div id="bocwa_e_postal_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Sector',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_villward_list_new($value_e_postal_areatype_code,'S'),
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																															'progress'			=> '',
																														  )
																							);
																								
							}
							
							if($value_e_postal_areatype == 'N'){
								
								$form['new_bocwa_reg']['e_postal_vill_ward'] 			= array(
																							  '#prefix' 			=> '<div id="bocwa_e_postal_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Notified Area',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_villward_list_new($value_e_postal_areatype_code,'N'),
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																															'progress'			=> '',
																														  )
																							);
																								
							}
						}
					}
				}
				
				$value_e_postal_vill_ward 	= isset($form_state['values']['e_postal_vill_ward']) ? $form_state['values']['e_postal_vill_ward'] : '';
				
				$form['new_bocwa_reg']['e_postal_ps'] 	= array(
															  '#prefix' 			=> '<div id="bocwa_e_postal_ps_replace" class="col-lg-4 col-md-6"><label class="select">',
															  '#suffix' 			=> '</label></div>',
															  '#title'				=> 'Select Police Station',
															  '#type' 				=> 'select',
															  '#options' 			=> array(),
															  '#required' 			=> TRUE,
															  '#ajax'					=> array(
																							'event' 			=> 'change',
																							'effect' 			=> 'fade',			
																							'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																							'progress'			=>	array('type' => 'throbber'),
																						  )
															  
															);
			
				if($value_e_postal_dist != "" && $value_e_postal_subdivision != ""){
					if($value_e_postal_areatype != ""){
						if($value_e_postal_areatype_code != ""){
							if($value_e_postal_vill_ward != ""){
								$form['new_bocwa_reg']['e_postal_ps'] 					= array(
																							  '#prefix' 			=> '<div id="bocwa_e_postal_ps_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Police Station',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_ps_list_new($value_e_postal_dist),
																							  '#required' 			=> TRUE
																						  );
							}
						}
					}
				}
																						
				$form['new_bocwa_reg']['e_postal_pin_number'] = array(
																  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																  '#suffix' 			=> '</label></div></div></div>',
																  '#title'				=> 'PIN Code',
																  '#type' 				=> 'textfield',
																  '#attributes' 		=> array('maxlength' => 6),
																  '#required' 			=> TRUE
																  );
				$form['new_bocwa_reg']['e_full_name'] 									= array(
																							  '#prefix'				=> '<div class="panel panel-default panel-form"><div class="panel-heading">3. Full Name and permanent address of the Establishment</div><div class="panel-body"><div class="col-lg-4 col-md-6"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Full Name of the Establishment',
																							  '#type' 				=> 'textfield',
																							  '#default_value' 		=>  !empty($obj['est_name']) ? $obj['est_name'] : '',
																							  '#required' 			=> TRUE,
																							  '#element_validate' 	=> array('BOCWA_SPECIAL_CHAR_TEXT_VALIDATE')
																							);
																							
				
											
				$form['new_bocwa_reg']['e_permanent_address'] 							= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'House No./Village Name/Street/Road',
																							  '#type' 				=> 'textfield',
																							  '#required' 			=> TRUE,
																							  '#default_value' 		=>  !empty($obj['est_loc']) ? $obj['est_loc'] : '',
																							  '#element_validate' 	=> array('BOCWA_SPECIAL_CHAR_TEXTAREA_VALIDATE')
																							);
											
											
				$form['new_bocwa_reg']['e_permanent_add_dist'] 	   						= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix'				=> '</label></div>',
																							  '#title'				=> 'District of the Establishment',
																							  '#type' 				=> 'select',
																							  '#options' 			=> $options_dist,
																							  '#default_value' 		=> !empty($obj['est_district']) ? $obj['est_district']:'',
																							  '#required' 			=> TRUE,
																							  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																															'progress'			=> '',
																														  )
																							);
																							
																							
				$value_e_permanent_add_dist 	= 	isset($form_state['values']['e_permanent_add_dist']) ? $form_state['values']['e_permanent_add_dist'] : $obj['est_district'];
				
				if($value_e_permanent_add_dist != "" || !empty($value_e_permanent_add_dist) || is_numeric($value_e_permanent_add_dist)){
				
					$form['new_bocwa_reg']['e_permanent_add_subdivision'] 				= array(
																							  '#prefix' 			=> '<div id="bocwa_e_permanent_add_subdvsn_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Subdivision',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_subdivisionlist_list_new($value_e_permanent_add_dist),
																							  '#required' 			=> TRUE,
																							  '#default_value' 		=>!empty($obj['est_subdivision']) ? $obj['est_subdivision'] : '',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																														'progress'			=> '',
																													  ),
																						);
																									
				}else{
					
					$form['new_bocwa_reg']['e_permanent_add_subdivision'] 				= array(
																							  '#prefix' 			=> '<div id="bocwa_e_permanent_add_subdvsn_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Subdivision',
																							  '#type' 				=> 'select',
																							  '#options' 			=> array(),
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																														'progress'			=> '',
																													  ),
																						);
				}
				
				$value_e_permanent_add_subdivision 	= isset($form_state['values']['e_permanent_add_subdivision']) ? $form_state['values']['e_permanent_add_subdivision'] : $obj['est_subdivision'];
				
				if($value_e_permanent_add_dist != "" && $value_e_permanent_add_subdivision != ""){
					
				   $areatypeOptions_1	= common_custom_user_areatype_list_new($value_e_permanent_add_dist, $value_e_permanent_add_subdivision);
					
				   if(!empty($areatypeOptions_1)){
						
					$form['new_bocwa_reg']['e_permanent_add_areatype'] 					= array(
																							  '#prefix' 			=> '<div id="bocwa_e_permanent_add_loc_e_areatype" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Areatype',
																							  '#type' 				=> 'select',
																							  '#options' 			=> $areatypeOptions_1,
																							  '#default_value'		=> !empty($obj['est_loc_e_areatype']) ? $obj['est_loc_e_areatype'] : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																														'progress'			=> '',
																													  ),
																								);
																								
					}
			
				}else{ 
					
					$form['new_bocwa_reg']['e_permanent_add_areatype'] 					= array(
																							  '#prefix' 			=> '<div id="bocwa_e_permanent_add_loc_e_areatype" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Areatype',
																							  '#type' 				=> 'select',
																							  '#options' 			=> array(),
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																														'progress'			=> '',
																													  ),
																								);
				}
				
				$value_e_permanent_add_areatype = isset($form_state['values']['e_permanent_add_areatype']) ? $form_state['values']['e_permanent_add_areatype'] : $obj['est_loc_e_areatype'];
				
				$form['new_bocwa_reg']['e_permanent_add_areatype_code'] 				= array(
																							 '#prefix' 				=> '<div id="bocwa_e_permanent_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
																							 '#suffix' 				=> '<i></i></label></div>',
																							 '#title'				=> 'Select Block / Municipality / Corporation / SEZ / Notified Area',
																							 '#type' 				=> 'select',
																							 '#options' 			=> array(),
																							 '#required' 			=> TRUE,
																							 '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																														'progress'			=> '',
																													  )
																						  );
				if($value_e_permanent_add_dist != "" && $value_e_permanent_add_subdivision != ""){
					if($value_e_permanent_add_areatype != ""){
						if($value_e_permanent_add_areatype == 'B'){
							$form['new_bocwa_reg']['e_permanent_add_areatype_code'] 	= array(
																							  '#prefix' 			=> '<div id="bocwa_e_permanent_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Block',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_e_permanent_add_subdivision,$value_e_permanent_add_areatype),
																							  '#required' 			=> TRUE,
																							  '#default_value'		=> !empty($obj['est_name_areatype']) ? $obj['est_name_areatype'] : '',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																														'progress'			=> '',
																													  )
																							);
						}else if($value_e_permanent_add_areatype == 'M'){
					
							$form['new_bocwa_reg']['e_permanent_add_areatype_code']  	= array(
																							  '#prefix' 			=> '<div id="bocwa_e_permanent_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Municipality',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_e_permanent_add_subdivision,$value_e_permanent_add_areatype),
																							  '#required' 			=> TRUE,
																							  '#default_value'		=> !empty($obj['est_name_areatype']) ? $obj['est_name_areatype'] : '',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																														'progress'			=> '',
																													  )
																							);
						}else if($value_e_permanent_add_areatype == 'C'){
			
							$form['new_bocwa_reg']['e_permanent_add_areatype_code'] 	= array(
																							  '#prefix' 			=> '<div id="bocwa_e_permanent_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Corporation',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_e_permanent_add_subdivision,$value_e_permanent_add_areatype),
																							  '#required' 			=> TRUE,
																							  '#default_value'		=> !empty($obj['est_name_areatype']) ? $obj['est_name_areatype'] : '',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																														'progress'			=> '',
																													  )
																						);
						}else if($value_e_permanent_add_areatype == 'S'){
			
							$form['new_bocwa_reg']['e_permanent_add_areatype_code'] 	= array(
																							  '#prefix' 			=> '<div id="bocwa_e_permanent_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select SEZ',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_e_permanent_add_subdivision,$value_e_permanent_add_areatype),
																							  '#required' 			=> TRUE,
																							  '#default_value'		=> !empty($obj['est_name_areatype']) ? $obj['est_name_areatype'] : '',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																														'progress'			=> '',
																													  )
																							);
						}else if($value_e_permanent_add_areatype == 'N'){
					
							$form['new_bocwa_reg']['e_permanent_add_areatype_code'] 	= array(
																							  '#prefix' 			=> '<div id="bocwa_e_permanent_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Notified Area',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_e_permanent_add_subdivision,$value_e_permanent_add_areatype),
																							  '#required' 			=> TRUE,
																							  '#default_value'		=> !empty($obj['est_name_areatype']) ? $obj['est_name_areatype'] : '',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																														'progress'			=> '',
																													  )
																							);
						}
					}
				}
				
								
				$value_e_permanent_add_areatype_code = isset($form_state['values']['e_permanent_add_areatype_code']) ? $form_state['values']['e_permanent_add_areatype_code'] : $obj['est_name_areatype'];
			
				$form['new_bocwa_reg']['e_permanent_add_vill_ward'] = array(
																		 '#prefix' 				=> '<div id="bocwa_e_permanent_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																		 '#suffix' 				=> '<i></i></label></div>',
																		 '#title'				=> 'Select Gram Panchayat/Ward',
																		 '#type' 				=> 'select',
																		 '#options' 			=> array(),
																		 '#required' 			=> TRUE,
																		 '#ajax'				=> array(
																									'event' 			=> 'change',
																									'effect' 			=> 'fade',			
																									'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																									'progress'			=> '',
																								  )
																	  );
																						  
				if($value_e_permanent_add_dist != "" && $value_e_permanent_add_subdivision != ""){
				if($value_e_permanent_add_areatype != ""){
					if($value_e_permanent_add_areatype_code != ""){
						if($value_e_permanent_add_areatype == 'B'){
							
							$form['new_bocwa_reg']['e_permanent_add_vill_ward'] 		= array(
																							  '#prefix' 			=> '<div id="bocwa_e_permanent_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Gram Panchayat',
																							  '#type' 				=> 'select',
																							  '#options'			=> common_custom_user_villward_list_new($value_e_permanent_add_areatype_code,'V'),
																							  '#required' 			=> TRUE,
																							  '#default_value'		=> !empty($obj['est_loc_e_vill_ward']) ? $obj['est_loc_e_vill_ward'] : '',
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																															'progress'			=> '',
																														  )
																							);	
						}
						
						if($value_e_permanent_add_areatype == 'M'){
							
							$form['new_bocwa_reg']['e_permanent_add_vill_ward'] 		= array(
																							  '#prefix' 			=> '<div id="bocwa_e_permanent_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Ward',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_villward_list_new($value_e_permanent_add_areatype_code,'W'),
																							  '#required' 			=> TRUE,
																							  '#default_value'		=> !empty($obj['est_loc_e_vill_ward']) ? $obj['est_loc_e_vill_ward'] : '',
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																															'progress'			=> '',
																														  )
																							);
																							
						}
						
						if($value_e_permanent_add_areatype == 'C'){
							
							$form['new_bocwa_reg']['e_permanent_add_vill_ward'] 		= array(
																							  '#prefix' 			=> '<div id="bocwa_e_permanent_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Ward',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_corporation_list_new($value_e_permanent_add_areatype_code),
																							  '#required' 			=> TRUE,
																							  '#default_value'		=> !empty($obj['est_loc_e_vill_ward']) ? $obj['est_loc_e_vill_ward'] : '',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																														'progress'			=> '',
																													  )
																							);
						}
						
						if($value_e_permanent_add_areatype == 'S'){
							
							$form['new_bocwa_reg']['e_permanent_add_vill_ward'] 		= array(
																							  '#prefix' 			=> '<div id="bocwa_e_permanent_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Sector',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_villward_list_new($value_e_permanent_add_areatype_code,'S'),
																							  '#required' 			=> TRUE,
																							  '#default_value'		=> !empty($obj['est_loc_e_vill_ward']) ? $obj['est_loc_e_vill_ward'] : '',
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																															'progress'			=> '',
																														  )
																							);
																							
						}
						
						if($value_e_permanent_add_areatype == 'N'){
							
							$form['new_bocwa_reg']['e_permanent_add_vill_ward'] 		= array(
																							  '#prefix' 			=> '<div id="bocwa_e_permanent_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Notified Area',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_villward_list_new($value_e_permanent_add_areatype_code,'N'),
																							  '#required' 			=> TRUE,
																							  '#default_value'		=> !empty($obj['est_loc_e_vill_ward']) ? $obj['est_loc_e_vill_ward'] : '',
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																															'progress'			=> '',
																														  )
																							);
																							
						}
					}
				}
			}
				
			    $value_e_permanent_add_vill_ward = isset($form_state['values']['e_permanent_add_vill_ward']) ? $form_state['values']['e_permanent_add_vill_ward'] : $obj['est_loc_e_vill_ward'];
				
			    $form['new_bocwa_reg']['e_permanent_add_ps'] = array(
																'#prefix' 			=> '<div id="bocwa_e_permanent_add_ps_replace" class="col-lg-4 col-md-6"><label class="select">',
																
																'#suffix' 			=> '<i></i></label></div>',
																'#title'			=> 'Select Police Station',
																'#type' 			=> 'select',
																'#options' 			=> array(),
																'#required' 		=> TRUE,
																'#ajax'				=> array(
																					'event' 			=> 'change',
																					'effect' 			=> 'fade',			
																					'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																					'progress'			=> '',
																	)
																);
																								  
																								  
																							
				if($value_e_permanent_add_dist != "" && $value_e_permanent_add_subdivision != ""){
					if($value_e_permanent_add_areatype != ""){
						if($value_e_permanent_add_areatype_code != ""){
							if($value_e_permanent_add_vill_ward != ""){
								$form['new_bocwa_reg']['e_permanent_add_ps'] = array(
																  '#prefix' 			=> '<div id="bocwa_e_permanent_add_ps_replace" class="col-lg-4 col-md-6"><label class="select">',
																  '#suffix' 			=> '<i></i></label></div>',
																  '#title'				=> 'Select Police Station',
																  '#type' 				=> 'select',
																  '#default_value'		=> !empty($obj['est_ps']) ? $obj['est_ps'] : '',
																  '#options' 			=> common_custom_user_ps_list_new($value_e_permanent_add_dist),
																  '#required' 			=> TRUE
															  );
				}
						}
					}
				}		
					
				$form['new_bocwa_reg']['e_permanent_add_pin_number'] = array(
																  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																  '#suffix' 			=> '</label></div></div></div>',
																  '#title'				=> 'PIN Code',
																  '#type'				=> 'textfield',
																  '#default_value'		=> !empty($obj['est_pin_number']) ? $obj['est_pin_number'] : '',
																  '#attributes' 		=> array('maxlength' => 6),
																  '#required' 			=> TRUE
															);
																	
				/*--------------------------------*/
				$form['new_bocwa_reg']['emp_name'] 	= array(
														  '#prefix'				=> '<div class="panel panel-default panel-form"><div class="panel-heading">4. Full Name and address of the Employer</div><div class="panel-body"><div class="col-lg-4 col-md-6"><label class="input">',
														  '#suffix' 			=> '</label></div>',
														  '#title'				=> 'Full Name of the Employer',
														  '#type' 				=> 'textfield',
														  '#default_value' 		=> $p_full_name,
														  '#required' 			=> TRUE,
														  '#element_validate'   => array('BOCWA_SPECIAL_CHAR_TEXT_VALIDATE')
														);
				$form['new_bocwa_reg']['emp_gender']  = array(
														  '#prefix'				=> '<div class="col-md-4 col-md-6"><div class="inline-group">',
														  '#suffix'				=>'</div></div>',
														  '#type' 				=> 'radios',
														  '#title' 				=> t('Gender'),
														  '#options' 			=> array('M'=>t('Male'), 'F'=>t('Female'), 'O'=>t('Other')),
														  //'#default_value' 		=> !empty($gender) ? $gender : '',
														  '#required' 			=> TRUE
														);
																							
				$form['new_bocwa_reg']['emp_dist'] 	  = array(
														  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="select">',
														  '#suffix' 			=> '</label></div>',
														  '#title'				=> 'Select District',
														  '#type' 				=> 'select',
														  '#options' 			=> $options_dist,
														  '#default_value' 		=> !empty($obj['dist_code']) ? $obj['dist_code']	:'',
														  '#required' 			=> TRUE,
														  '#ajax'				=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_bocwa_emp_ajax_callback',
																						'progress'			=> '',
																					  )
														);
																				
		$value_emp_dist 	= 	isset($form_state['values']['emp_dist']) ? $form_state['values']['emp_dist'] : $obj['dist_code'];
		
		if($value_emp_dist != "" || !empty($value_emp_dist) || is_numeric($value_emp_dist)){
			$form['new_bocwa_reg']['emp_subdivision'] = array(
														  '#prefix' 			=> '<div id="bocwa_emp_subdvsn_replace" class="col-lg-4 col-md-6"><label class="select">',
														  '#suffix' 			=> '</label></div>',
														  '#title'				=> 'Select Subdivision',
														  '#type' 				=> 'select',
														  '#options' 			=> common_custom_user_subdivisionlist_list_new($value_emp_dist),
														  '#default_value' 		=> ($obj['subdivision']!='') ? $obj['subdivision']	:'',
														  '#required' 			=> TRUE,
														  '#ajax'				=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_bocwa_emp_ajax_callback',
																						'progress'			=> '',
																					  ),
													);
			
		}else{
			$form['new_bocwa_reg']['emp_subdivision'] = array(
															'#prefix' 			=> '<div id="bocwa_emp_subdvsn_replace">',
															'#suffix' 			=> '</div>',
															'#title'			=> 'Select Subdivision',
															'#type' 			=> 'select',
															'#options' 			=> array(),
															'#required' 		=> TRUE,
															 '#ajax'			=> array(
																					'event' 			=> 'change',
																					'effect' 			=> 'fade',			
																					'callback'			=> 'get_bocwa_emp_ajax_callback',
																					'progress'			=> '',
																				  ),
														);
		}
		$value_emp_subdivision = isset($form_state['values']['emp_subdivision']) ? $form_state['values']['emp_subdivision'] : trim($obj['subdivision']);
			
		if($value_emp_dist != "" && $value_emp_subdivision != ""){	
																	  
			$areatypeOptions = common_custom_user_areatype_list_new($value_emp_dist, $value_emp_subdivision);
		
			if(empty($areatypeOptions)){
				
				$form['new_bocwa_reg']['emp_areatype'] = array(
															  '#prefix' 			=> '<div id="bocwa_emp_areatype_replace" class="col-lg-4 col-md-6"><label class="select">',
															  '#suffix' 			=> '</label></div>',
															  '#title'				=> 'Select Areatype',
															  '#type' 				=> 'select',
															  '#options' 			=> array(),
															  '#required' 			=> TRUE,
															  '#ajax'				=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_bocwa_emp_ajax_callback',
																						'progress'			=> '',
																					  ),
																);
			}else{
				$form['new_bocwa_reg']['emp_areatype']  = array(
														  '#prefix' 			=> '<div id="bocwa_emp_areatype_replace" class="col-lg-4 col-md-6"><label class="select">',
														  '#suffix' 			=> '</label></div>',
														  '#title'				=> 'Select Areatype',
														  '#type' 				=> 'select',
														  '#options' 			=> $areatypeOptions,
														  '#required' 			=> TRUE,
														  '#default_value' 		=> !empty($areatype_code) ? $areatype_code : '',
														  '#ajax'				=> array(
																					'event' 			=> 'change',
																					'effect' 			=> 'fade',			
																					'callback'			=> 'get_bocwa_emp_ajax_callback',
																					'progress'			=> '',
																				  ),
															);
				}
			}
			$value_emp_areatype 		= isset($form_state['values']['emp_areatype']) ? $form_state['values']['emp_areatype'] : trim($areatype_code);
			if ($value_emp_dist != "" && $value_emp_subdivision != "" && $value_emp_areatype != "") {
				
				if ( $value_emp_areatype == 'B' ){
					$areatype_name	= 'Block';
				}elseif( $value_emp_areatype == 'M'){
					$areatype_name	= 'Municipality';
				}elseif( $value_emp_areatype == 'C'){
					$areatype_name	= 'Corporation';
				}elseif( $value_emp_areatype == 'S'){
					$areatype_name	= 'SEZ';
				}elseif($value_emp_areatype == 'N'){
					$areatype_name	= 'Notified Area';
				}
				
				
				$form['new_bocwa_reg']['emp_areatype_code'] = array(
															  '#prefix' 			=>'<div id="bocwa_emp_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
															  '#suffix' 			=> '</label></div>',
															  '#title'				=> 'Select '.$areatype_name,
															  '#type' 				=> 'select',
															  '#options' 			=> common_custom_user_block_list_new($value_emp_subdivision, $value_emp_areatype),
															  '#required' 			=> TRUE,
															 
															 '#default_value' 		=> !empty($name_areatype_code) ? $name_areatype_code : '',
															  '#ajax'				=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_bocwa_emp_ajax_callback',
																						'progress'			=> '',
																					  )
													 );
			}else{
				$form['new_bocwa_reg']['emp_areatype_code'] = array(
																 '#prefix' 			=> '<div id="bocwa_emp_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
																 '#suffix' 			=> '</label></div>',
																 '#title'			=> 'Select Block / Municipality / Corporation / SEZ / Notified Area',
																 '#type' 			=> 'select',
																 '#options' 		=> array(),
																 '#required' 		=> TRUE,
																 '#ajax'			=> array(
																							'event' 			=> 'change',
																							'effect' 			=> 'fade',			
																							'callback'			=> 'get_bocwa_emp_ajax_callback',
																							'progress'			=> '',
																						  )
													 );
			}
			$value_emp_areatype_code = isset($form_state['values']['emp_areatype_code']) ? $form_state['values']['emp_areatype_code'] : trim($name_areatype_code);
			
			
			$form['new_bocwa_reg']['emp_vill_ward'] 									= array(
																							 '#prefix' 				=> '<div id="bocwa_emp_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							 '#suffix' 				=> '</label></div>',
																							 '#title'				=> 'Select Gram Panchayat/Ward',
																							 '#type' 				=> 'select',
																							 '#options' 			=> array(),
																							 '#required' 			=> TRUE,
																							
																							 '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_emp_ajax_callback',
																														'progress'			=> '',
																													  )
				
				
																						  );
			
			
			if($value_emp_dist != "" && $value_emp_subdivision != ""){
				if($value_emp_areatype != ""){
					if($value_emp_areatype_code != ""){
						if($value_emp_areatype == 'B'){
								
								$form['new_bocwa_reg']['emp_vill_ward'] 				= array(
																							  '#prefix' 			=> '<div id="bocwa_emp_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Gram Panchayat',
																							  '#type' 				=> 'select',
																							  '#options'			=> common_custom_user_villward_list_new($value_emp_areatype_code,'V'),
																							  '#default_value' 		=> !empty($vill_ward_code) ? $vill_ward_code : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_emp_ajax_callback',
																															'progress'			=> '',
																														  )
																							);	
						}
							
						if($value_emp_areatype == 'M'){
								
								$form['new_bocwa_reg']['emp_vill_ward'] 				= array(
																							  '#prefix' 			=> '<div id="bocwa_emp_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Ward',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_villward_list_new($value_emp_areatype_code,'W'),
																							  '#default_value' 		=> !empty($vill_ward_code) ? $vill_ward_code : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_emp_ajax_callback',
																															'progress'			=> '',
																														  )
																							);
																								
						}
							
						if($value_emp_areatype == 'C'){
								
								$form['new_bocwa_reg']['emp_vill_ward'] 				= array(
																							  '#prefix' 			=> '<div id="bocwa_emp_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Ward',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_corporation_list_new($value_emp_areatype_code),
																							  '#default_value' 		=> !empty($vill_ward_code) ? $vill_ward_code : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_emp_ajax_callback',
																														'progress'			=> '',
																													  )
																							);
						}
						
						if($value_emp_areatype == 'S'){
								
								$form['new_bocwa_reg']['emp_vill_ward'] 				= array(
																							  '#prefix' 			=> '<div id="bocwa_emp_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Sector',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_villward_list_new($value_emp_areatype_code,'S'),
																							  '#default_value' 		=> !empty($vill_ward_code) ? $vill_ward_code : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_emp_ajax_callback',
																															'progress'			=> '',
																														  )
																							);
																								
						}
						
						if($value_emp_areatype == 'N'){
								
								$form['new_bocwa_reg']['emp_vill_ward'] 				= array(
																							  '#prefix' 			=> '<div id="bocwa_emp_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Notified Area',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_villward_list_new($value_emp_areatype_code,'N'),
																							  '#default_value' 		=> !empty($vill_ward_code) ? $vill_ward_code : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_emp_ajax_callback',
																															'progress'			=> '',
																														  )
																							);
																								
						}
					}
				}
			}	
			
			$value_emp_vill_ward														= isset($form_state['values']['emp_vill_ward']) ? $form_state['values']['emp_vill_ward'] : $vill_ward_code;
			
			$form['new_bocwa_reg']['emp_ps'] 											= array(
																						  '#prefix' 				=> '<div id="bocwa_emp_ps_replace" class="col-lg-4 col-md-6"><label class="select">',
																						  '#suffix' 				=> '<i></i></label></div>',
																						  '#title'					=> 'Select Police Station',
																						  '#type' 					=> 'select',
																						  '#options' 				=> array(),
																						  '#required' 				=> TRUE,
																						  '#ajax'					=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_emp_ajax_callback',
																														'progress'			=> '',
																													  )
																					  );
																					  
			if($value_emp_dist != "" && $value_emp_subdivision != ""){
				if($value_emp_areatype != ""){
					if($value_emp_areatype_code != ""){
						if($value_emp_vill_ward != ""){
							$form['new_bocwa_reg']['emp_ps'] 							= array(
																							  '#prefix' 			=> '<div id="bocwa_emp_ps_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Police Station',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_ps_list_new($value_emp_dist),
																							  '#default_value' 		=> !empty($ps_code) ? $ps_code : '',
																							  '#required' 			=> TRUE
																					  );
						}
					}
				}
			}
			
			$form['new_bocwa_reg']['emp_pin_number'] = array(
														  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
														  '#suffix' 			=> '</label></div></div></div>',
														  '#title'				=> 'PIN Code',
														  '#type'				=> 'textfield',
														  '#default_value' 		=>!empty($pin) ? $pin : '',
														  '#attributes' 		=> array('maxlength' => 6),
														  '#required' 			=> TRUE
													);	
					
			
				
				
				
																							  
																							  
		$form['new_bocwa_reg']['full_name_manager'] = array(
														'#prefix' 			=> '<div class="panel panel-default panel-form"><div class="panel-heading">5. Full name and address of the Manager or Person responsible for the supervision  and control of the Establishment</div><div class="panel-body"><div class="col-lg-4 col-md-6"><label class="input">',
														'#suffix' 			=> '</label></div>',
														'#title'			=> 'Full name of the Manager or Person Responsible',
														'#type' 			=> 'textfield',
														'#default_value' 	=> $p_full_name,
														'#required' 		=> TRUE,
														'#element_validate' => array('BOCWA_TEXTBOX_TEXT_VALIDATE')
													);
				
				
												
				$form['new_bocwa_reg']['address_manager'] 								= array(
																							 '#prefix' 				=> '<div class="col-lg-4 col-md-6"><label class="input">',
																							 '#suffix' 				=> '</label></div>',
																							 '#title'				=> 'House No./Village Name/Street/Road',
																							 '#type' 				=> 'textfield',
																							 '#default_value' 		=>!empty($address) ? $address : '',
																							 '#required' 			=> TRUE,
																							 '#element_validate' 	=> array('BOCWA_SPECIAL_CHAR_TEXTAREA_VALIDATE')
																						);
										
				$form['new_bocwa_reg']['address_manager_dist'] 	   						= array(
																							  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select District',
																							  '#type' 				=> 'select',
																							  '#options' 			=> $options_dist,
																							  '#default_value' 		=> !empty($district_code) ? $district_code : '',
																							  '#required' 			=> TRUE,
																							  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_address_manager_ajax_callback',
																															'progress'			=> '',
																														  )
																							);
											
				$value_address_manager_dist 				= 	isset($form_state['values']['address_manager_dist']) ? $form_state['values']['address_manager_dist'] : $district_code;
				
				
				if($value_address_manager_dist != "" || !empty($value_address_manager_dist) || is_numeric($value_address_manager_dist)){
				
				
				
					$form['new_bocwa_reg']['address_manager_subdivision'] 				= array(
																							  '#prefix' 			=> '<div id="bocwa_address_manager_subdvsn_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Subdivision',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_subdivisionlist_list_new($value_address_manager_dist),
																							  '#default_value' 		=> !empty($subdivision_code) ? $subdivision_code:'',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_address_manager_ajax_callback',
																															'progress'			=> '',
																														  ),
																						);
																										
			}else{
				
					$form['new_bocwa_reg']['address_manager_subdivision'] 				= array(
																							  '#prefix' 			=> '<div id="bocwa_address_manager_subdvsn_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Subdivision',
																							  '#type' 				=> 'select',
																							  '#options' 			=> array(),
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_address_manager_ajax_callback',
																															'progress'			=> '',
																														  ),
																						);
			}
				
			$value_address_manager_subdivision 					= isset($form_state['values']['address_manager_subdivision']) ? $form_state['values']['address_manager_subdivision'] : $subdivision_code;
			
			if($value_address_manager_dist != "" && $value_address_manager_subdivision != ""){
				
				$areatypeOptions					= common_custom_user_areatype_list_new($value_address_manager_dist, $value_address_manager_subdivision);
				
				if(empty($areatypeOptions)){
				
				
					$form['new_bocwa_reg']['address_manager_areatype']  				= array(
																							  '#prefix' 			=> '<div id="bocwa_address_manager_areatype" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Areatype',
																							  '#type' 				=> 'select',
																							  '#options' 			=> array(),

																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_address_manager_ajax_callback',
																															'progress'			=> '',
																														  ),
																								);
																												
				}else{
				
					$form['new_bocwa_reg']['address_manager_areatype'] 					= array(
																							  '#prefix' 			=> '<div id="bocwa_address_manager_areatype" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Areatype',
																							  '#type' 				=> 'select',
																							  '#options' 			=> $areatypeOptions,
																							  '#default_value' 		=> !empty($areatype_code) ? $areatype_code : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_address_manager_ajax_callback',
																															'progress'			=> '',
																														  ),
																								);
				}	
			}
			
			$value_address_manager_areatype 			= isset($form_state['values']['address_manager_areatype']) ? $form_state['values']['address_manager_areatype'] : $areatype_code;
			
			if($value_address_manager_dist != "" && $value_address_manager_subdivision != "" && $value_address_manager_areatype != ""){
				
				if ( $value_address_manager_areatype == 'B' ){
					$areatype_name	= 'Block';
				}elseif( $value_address_manager_areatype == 'M'){
					$areatype_name	= 'Municipality';
				}elseif( $value_address_manager_areatype == 'C'){
					$areatype_name	= 'Corporation';
				}elseif( $value_address_manager_areatype == 'S'){
					$areatype_name	= 'SEZ';
				}elseif($value_address_manager_areatype == 'N'){
					$areatype_name	= 'Notified Area';
				}
				
				
				$form['new_bocwa_reg']['address_manager_areatype_code'] 				= array(
																							  '#prefix' 			=>'<div id="bocwa_address_manager_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Block',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_address_manager_subdivision, $value_address_manager_areatype),
																							  '#default_value' 		=> !empty($name_areatype_code) ? $name_areatype_code : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_address_manager_ajax_callback',
																														'progress'			=> '',
																													  )
																							);
				
				
			}else{
				
				$form['new_bocwa_reg']['address_manager_areatype_code'] 				= array(
																							 '#prefix' 				=> '<div id="bocwa_address_manager_common_1_replace" class="col-lg-4 col-md-6"><label class="select">',
																							 '#suffix' 				=> '<i></i></label></div>',
																							 '#title'				=> 'Select Block / Municipality / Corporation / SEZ / Notified Area',
																							 '#type' 				=> 'select',
																							 '#options' 			=> array(),
																							 '#required' 			=> TRUE,
																							 '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_address_manager_ajax_callback',
																														'progress'			=> '',
																													  )
																						  );									
												
			}
				
					  
			$value_address_manager_areatype_code 			= isset($form_state['values']['address_manager_areatype_code']) ? $form_state['values']['address_manager_areatype_code'] : $name_areatype_code;
			
				
			$form['new_bocwa_reg']['address_manager_vill_ward'] 						= array(
																							 '#prefix' 				=> '<div id="bocwa_address_manager_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							 '#suffix' 				=> '<i></i></label></div>',
																							 '#title'				=> 'Select Gram Panchayat/Ward',
																							 '#type' 				=> 'select',
																							 '#options' 			=> array(),
																							 '#required' 			=> TRUE,
																							 '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_address_manager_ajax_callback',
																														'progress'			=> '',
																													  )
																						  );
																						  
			if($value_address_manager_dist != "" && $value_address_manager_subdivision != ""){
				if($value_address_manager_areatype != ""){
					if($value_address_manager_areatype_code != ""){
						if($value_address_manager_areatype == 'B'){
								
								$form['new_bocwa_reg']['address_manager_vill_ward'] 	= array(
																							  '#prefix' 			=> '<div id="bocwa_address_manager_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Gram Panchayat',
																							  '#type' 				=> 'select',
																							  '#options'			=> common_custom_user_villward_list_new($value_address_manager_areatype_code,'V'),
																							  '#default_value' 		=> !empty($vill_ward_code) ? $vill_ward_code : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_address_manager_ajax_callback',
																															'progress'			=> '',
																														  )
																							);	
						}
							
						if($value_address_manager_areatype == 'M'){
								
								$form['new_bocwa_reg']['address_manager_vill_ward'] 	= array(
																							  '#prefix' 			=> '<div id="bocwa_address_manager_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Ward',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_villward_list_new($value_address_manager_areatype_code,'W'),
																							  '#default_value' 		=> !empty($vill_ward_code) ? $vill_ward_code : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_address_manager_ajax_callback',
																															'progress'			=> '',
																														  )
																							);
																								
						}
							
						if($value_address_manager_areatype == 'C'){
								
								$form['new_bocwa_reg']['address_manager_vill_ward'] 	= array(
																							  '#prefix' 			=> '<div id="bocwa_address_manager_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Ward',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_corporation_list_new($value_address_manager_areatype_code),
																							  '#default_value' 		=> !empty($vill_ward_code) ? $vill_ward_code : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_address_manager_ajax_callback',
																														'progress'			=> '',
																													  )
																							);
						}
						if($value_address_manager_areatype == 'S'){
								
								$form['new_bocwa_reg']['address_manager_vill_ward'] 	= array(
																							  '#prefix' 			=> '<div id="bocwa_address_manager_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Sector',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_villward_list_new($value_address_manager_areatype_code,'S'),
																							  '#default_value' 		=> !empty($vill_ward_code) ? $vill_ward_code : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_address_manager_ajax_callback',
																															'progress'			=> '',
																														  )
																							);
																								
						}
						
						if($value_address_manager_areatype == 'N'){
								
								$form['new_bocwa_reg']['address_manager_vill_ward'] 	= array(
																							  '#prefix' 			=> '<div id="bocwa_address_manager_common_2_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Notified Area',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_villward_list_new($value_address_manager_areatype_code,'N'),
																							  '#default_value' 		=> !empty($vill_ward_code) ? $vill_ward_code : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_bocwa_address_manager_ajax_callback',
																															'progress'			=> '',
																														  )
																							);
																								
						}
					}
				}
			}
			
			$value_address_manager_vill_ward						= isset($form_state['values']['address_manager_vill_ward']) ? $form_state['values']['address_manager_vill_ward'] : $vill_ward_code;	
			
				
				
									
			$form['new_bocwa_reg']['address_manager_ps'] 								= array(
																						  '#prefix' 				=> '<div id="bocwa_address_manager_ps_replace" class="col-lg-4 col-md-6"><label class="select">',
																						  '#suffix' 				=> '<i></i></label></div>',
																						  '#title'					=> 'Select Police Station',
																						  '#type' 					=> 'select',
																						  '#options' 				=> array(),
																						  '#required' 				=> TRUE,
																						  '#ajax'					=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_bocwa_address_manager_ajax_callback',
																														'progress'			=> '',
																													  )
																					  );
																					  
																					  
				if($value_address_manager_dist != "" && $value_address_manager_subdivision != ""){
					if($value_address_manager_areatype != ""){
						if($value_address_manager_areatype_code != ""){
							if($value_address_manager_vill_ward != ""){
								$form['new_bocwa_reg']['address_manager_ps'] 			= array(
																							  '#prefix' 			=> '<div id="bocwa_address_manager_ps_replace" class="col-lg-4 col-md-6"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Police Station',
																							  '#type' 				=> 'select',
																							  '#default_value' 		=> !empty($ps_code) ? $ps_code : '',
																							  '#options' 			=> common_custom_user_ps_list_new($value_address_manager_dist),
																							  '#required' 			=> TRUE
																						  );
							}
						}
					}
				}
																								  
																								  
																					
				$form['new_bocwa_reg']['address_manager_pin_number'] 		   = array(
																					  '#prefix' 				=> '<div class="col-lg-4 col-md-6"><label class="input">',
																					  '#suffix' 				=> '</label></div></div></div>',
																					  '#title'					=> 'PIN Code',
																					  '#type'					=> 'textfield',
																					  '#default_value' 			=>!empty($pin) ? $pin : '',
																					  '#attributes' 			=> array('maxlength' => 6),
																					  '#required' 				=> TRUE
																						  );
		
                                                                                          
                                                                                          
                /*******************************************/											 
				
				/*******************************************/											  				
				$form['new_bocwa_reg']['nature_of_build_const'] = array(
																	'#prefix' 			=> '<div class="panel panel-default panel-form"><div class="panel-heading">Other Information</div><div class="panel-body"><div class="col-lg-6 col-md-6"><label class="input">',
																	'#suffix' 			=> '</label></div>',
																	'#title'			=> '6. Nature of building or other construction work is to be carried on',
																	'#type' 			=> 'textfield',
																	'#required' 		=> TRUE,
																	'#element_validate'	=> array('BOCWA_TEXTBOX_TEXT_VALIDATE')
																	);
													
													
				$form['new_bocwa_reg']['max_no_of_building_workers_employed'] = array(
																'#prefix' 				=> '<div class="col-lg-6 col-md-6"><label class="input">',
																'#suffix' 				=> '</label></div>',
																'#title'				=> '7. Maximum number of building workers to be employed on any day',
																'#type' 				=> 'textfield',
																'#required' 			=> TRUE,
																'#element_validate'		=> array('BOCWA_TEXTBOX_NUMERIC_VALIDATE')
																);	
											
				$form['new_bocwa_reg']['est_date_of_commencement_building'] = array(
																'#prefix' 				=> '<div class="col-lg-6 col-md-6"><label class="input">',
																'#suffix' 				=> '</label></div>',
																'#title'				=> '8. Estimated date of commencement of building or other construction work',
																'#type' 				=> 'textfield',
																'#default_value'		=> 	date('d/m/Y'),
																'#required' 			=> TRUE,
																'#id'					=> 'est_date_of_commencement_id',
																'#attributes'			=> array('readonly'=>'readonly','placeholder' => t('From'))
																);
												
				$form['new_bocwa_reg']['est_date_of_completion_building'] = array(
																'#prefix' 				=> '<div class="col-lg-6 col-md-6"><label class="input">',
																'#suffix' 				=> '</label></div>',
																'#title'				=> '9. Estimated date of completion of building or other construction work',
																'#type' 				=> 'textfield',
																'#required' 			=> TRUE,
																'#id'					=> 'est_date_of_termination_id',
																'#attributes'			=>array('readonly'=>'readonly', 'placeholder'=> t('To'))
																);	
				
																							
				
				$form['new_bocwa_reg']['submit'] 			= array(
															   '#prefix' 				=> '</div></div>',  
															   '#suffix' 				=> '</div>',  
															   '#type' 					=> 'submit',
															   '#attributes' 			=> array('class' => array('btn btn-primary pull-right')),
															   '#value' 				=> 'SAVE'
																);
				return $form;
				
		}else {
			drupal_goto('applicant-profile');
		}


}

function BOCWA_SPECIAL_CHAR_TEXT_VALIDATE($element, &$form_state){
	$txtValue		=	trim($element['#value']);
	$match_pattern 	= 	'/[\/~`\!@#\$%\^&\*\(\)_\-\+=\{\}\[\]\|;:"\<\>,\?\\\]/';
	if(preg_match($match_pattern, $txtValue)) {
		form_error($element, t($element['#title'].' SHOULD BE VALID'));
	}
}

function BOCWA_SPECIAL_CHAR_TEXTAREA_VALIDATE($element, &$form_state){
	$txtValue		=	trim($element['#value']);
	$match_pattern 	= 	'/[\'\~`\!@#\$%\^&\*\(\)_\+=\{\}\[\]\|;:"\<\>\?\\\]/';
	if(preg_match($match_pattern, $txtValue)) {
		form_error($element, t($element['#title'].' SHOULD BE TEXT ONLY'));
	}
}

function BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE($element,&$form_state){
	$val			=	$form_state['values'];
	$txtValue		=	trim($element['#value']);
	if($txtValue == 0 ) {
		form_error($element, t($element['#title'].' SHOULD BE VALID'));
		$form_state['rebuild'] = TRUE;
	}
}

function BOCWA_POLICE_STATION_VALIDATE($element, &$form_state){
	$val				=	$form_state['values'];
	$police_statn_1		=	$val['loc_e_ps'];
	$police_statn_2		=	$val['e_postal_ps'];
	$police_statn_3		=	$val['e_permanent_add_ps'];
	$police_statn_4		=	$val['address_manager_ps'];
	
	if( $police_statn_1 == "" ){
		form_set_error($form_state['new_bocwa_reg']['loc_e_ps'], t('Police Station SHOULD BE VALID'));
	}
	if( $police_statn_2 == "" ){
		form_set_error($form_state['new_bocwa_reg']['e_postal_ps'], t('Police Station cfgfg SHOULD BE VALID'));
	}
	if( $police_statn_3 == "" ){
		form_set_error($form_state['new_bocwa_reg']['e_permanent_add_ps'], t('Police Station gddgdg SHOULD BE VALID'));
	}
	if( $police_statn_4 == "" ){
		form_set_error($form_state['new_bocwa_reg']['address_manager_ps'], t('Police Station dgdgdgdgdgdgdg SHOULD BE VALID'));
	}
}

function BOCWA_LOCATION_AREATYPE_VALIDATE($element,&$form_state){
	$val			=	$form_state['values'];
	$txtValue		=	trim($element['#value']);
	if($txtValue == 'T' ) {
		form_error($element, t($element['#title'].' SHOULD BE VALID'));
		$form_state['rebuild'] = TRUE;
	}
}

function BOCWA_TEXTBOX_TEXT_VALIDATE($element,&$form_state){
	$txtValue		=	trim($element['#value']); 
	$match_pattern 	= 	'/^[ A-Za-z ]+$/';
	if(!preg_match($match_pattern, $txtValue)) {
		form_error($element, t($element['#title'].' SHOULD BE TEXT ONLY'));
	}
}

function BOCWA_TEXTBOX_NUMERIC_VALIDATE($element,&$form_state){
	$fieldValue=trim($element['#value']); 
	$match_pattern = '/^[0-9]+$/i';
	if(!empty($fieldValue)){
		if(!preg_match($match_pattern, $fieldValue)) {
			form_error($element, t($element['#title'].' SHOULD BE NUMBER ONLY'));
		}
	}
}


function get_new_bocwa_registration_info_form_submit($form, &$form_state){
	 
		global $base_root, $base_path, $user;
		
		$val 									= $form_state['values']; 
		$identification_number					= "BOCWA".time();
		$act_id 								= $val['bocwa_act_id'];
		
		$e_name									= $val['e_name'];
		$est_type								= $val['est_type'];
		$loc_e_name								= $val['loc_e_name'];
		$loc_e_dist								= $val['loc_e_dist'];
		$loc_e_subdivision						= $val['loc_e_subdivision'];
		$loc_e_areatype							= $val['loc_e_areatype'];
		$loc_e_areatype_code					= $val['loc_e_areatype_code'];
		$loc_e_vill_ward						= $val['loc_e_vill_ward'];
		$loc_e_ps								= $val['loc_e_ps'];
		$loc_e_pin_number						= $val['loc_e_pin_number'];
		
		
		/**************************************************/
		
		$e_postal_address 						= $val['e_postal_address'];
		$e_postal_dist 							= $val['e_postal_dist'];
		$e_postal_subdivision 					= $val['e_postal_subdivision'];
		$e_postal_areatype 						= $val['e_postal_areatype'];
		$e_postal_areatype_code 				= $val['e_postal_areatype_code'];
		$e_postal_vill_ward						= $val['e_postal_vill_ward'];
		$e_postal_ps 							= $val['e_postal_ps'];
		$e_postal_pin_number					= $val['e_postal_pin_number'];
		
		/**************************************************/
		
		$e_full_name 							= $val['e_full_name'];
		$e_permanent_address 					= $val['e_permanent_address'];
		$e_permanent_add_dist 					= $val['e_permanent_add_dist'];
		$e_permanent_add_subdivision 			= $val['e_permanent_add_subdivision'];
		$e_permanent_add_areatype 				= $val['e_permanent_add_areatype'];
		$e_permanent_add_areatype_code 			= $val['e_permanent_add_areatype_code'];
		$e_permanent_add_vill_ward				= $val['e_permanent_add_vill_ward'];
		$e_permanent_add_ps 					= $val['e_permanent_add_ps'];
		$e_permanent_add_pin_number				= $val['e_permanent_add_pin_number'];
		
		
		/**************************************************/
		
		$emp_name  								= $val['emp_name'];
		$emp_gender  							= $val['emp_gender'];
		$emp_address 							= $val['emp_address'];
		$emp_dist 								= $val['emp_dist'];
		$emp_subdivision						= $val['emp_subdivision'];
		$emp_areatype 							= $val['emp_areatype'];
		$emp_areatype_code 						= $val['emp_areatype_code'];
		$emp_vill_ward 							= $val['emp_vill_ward'];
		$emp_ps 								= $val['emp_ps'];
		$emp_pin_number							= $val['emp_pin_number'];
		
		
		/**************************************************/
		
		$full_name_manager 						= $val['full_name_manager'];
		$address_manager 						= $val['address_manager'];
		$address_manager_dist 					= $val['address_manager_dist'];
		$address_manager_subdivision			= $val['address_manager_subdivision'];
		$address_manager_areatype 				= $val['address_manager_areatype'];
		$address_manager_areatype_code 			= $val['address_manager_areatype_code'];
		$address_manager_vill_ward 				= $val['address_manager_vill_ward'];
		$address_manager_ps 					= $val['address_manager_ps'];
		$address_manager_pin_number				= $val['address_manager_pin_number'];
		
		/**************************************************/
		
		$nature_of_build_const   			    = $val['nature_of_build_const'];
		$max_no_of_building_workers_employed	= $val['max_no_of_building_workers_employed'];
		$est_date_of_commencement_building		= date('Y-m-d',strtotime(str_replace('/', '-',$val['est_date_of_commencement_building'])));
		$est_date_of_completion_building		= date('Y-m-d',strtotime(str_replace('/', '-',$val['est_date_of_completion_building'])));
		
				
		
		$fieldsRegBocwa 						= array( 
														'user_id'									=>	$user->uid,
														'identification_number'						=>	$identification_number,
														'act_id'									=>  $act_id,
														
														'e_name'									=>	$e_name,
														'est_type'									=>  $est_type,
														'loc_e_name'								=>	$loc_e_name,
														'loc_e_dist'								=>	$loc_e_dist,
														'loc_e_subdivision'							=>	$loc_e_subdivision ,
														'loc_e_areatype'							=>	$loc_e_areatype ,
														'loc_e_areatype_code'						=>	$loc_e_areatype_code, 
														'loc_e_vill_ward'							=>	$loc_e_vill_ward,
														'loc_e_ps'									=>	$loc_e_ps,
														'loc_e_pin_number'							=>	$loc_e_pin_number,
																												 
														'e_postal_address'							=>	$e_postal_address,
														'e_postal_dist'								=>	$e_postal_dist, 
														'e_postal_subdivision'						=>	$e_postal_subdivision, 
														'e_postal_areatype'							=>	$e_postal_areatype, 
														'e_postal_areatype_code'					=>	$e_postal_areatype_code, 
														'e_postal_vill_ward'						=>	$e_postal_vill_ward, 
														'e_postal_ps'								=>	$e_postal_ps ,
														'e_postal_pin_number'						=>	$e_postal_pin_number,
														
														'e_full_name'								=>  $e_full_name,
														'e_permanent_address'						=>	$e_permanent_address,
														'e_permanent_add_dist'						=>  $e_permanent_add_dist,
														'e_permanent_add_subdivision'				=>  $e_permanent_add_subdivision,
														'e_permanent_add_areatype'					=>  $e_permanent_add_areatype,
														'e_permanent_add_areatype_code'				=>	$e_permanent_add_areatype_code ,
														'e_permanent_add_vill_ward'					=>	$e_permanent_add_vill_ward ,
														'e_permanent_add_ps'						=>	$e_permanent_add_ps,
														'e_permanent_add_pin_number'				=>  $e_permanent_add_pin_number,
														
														'emp_name'									=>  $emp_name,
														'emp_gender'								=>  $emp_gender,
														'emp_address'								=>	$emp_address,
														'emp_dist'									=>  $emp_dist,
														'emp_subdivision'							=>  $emp_subdivision,
														'emp_areatype'								=>  $emp_areatype,
														'emp_areatype_code'							=>	$emp_areatype_code ,
														'emp_vill_ward'								=>	$emp_vill_ward ,
														'emp_ps'									=>	$emp_ps,
														'emp_pin_number'							=>  $emp_pin_number,
														
														'full_name_manager'							=>	$full_name_manager ,
														'address_manager'							=>	$address_manager,
														'address_manager_dist'						=>	$address_manager_dist ,
														'address_manager_subdivision'				=>  $address_manager_subdivision,
														'address_manager_areatype'					=> 	$address_manager_areatype,
														'address_manager_areatype_code'				=>	$address_manager_areatype_code,
														'address_manager_vill_ward'					=>	$address_manager_vill_ward,
														'address_manager_ps'						=>	$address_manager_ps,
														'address_manager_pin_number'				=>  $address_manager_pin_number,
														
														'nature_of_build_const'						=>	$nature_of_build_const ,
														'max_no_of_building_workers_employed'		=>	$max_no_of_building_workers_employed,
														'est_date_of_commencement_building'			=>	$est_date_of_commencement_building ,
														'est_date_of_completion_building'			=>	$est_date_of_completion_building,
														
														);		
		
									
		if( !empty($fieldsRegBocwa) ){
			
					$application_id = db_insert('l_bocwa_registration_master')->fields($fieldsRegBocwa)->execute(); 
					
					if($application_id != ""){
						drupal_goto('bocwa-application/default/'.encryption_decryption_fun('encrypt', $application_id));
					}
					
					$message							=	'Application Saved Successfully';
					drupal_set_message(t($message));
		} 
		
		
}


function get_bocwa_registration_info_form_ajax_callback($form, $form_state) {
		$commands = array();
		$commands[] = ajax_command_replace('#bocwa_subdvsn_replace', drupal_render($form['new_bocwa_reg']['loc_e_subdivision']));
		$commands[] = ajax_command_replace('#bocwa_loc_e_areatype', drupal_render($form['new_bocwa_reg']['loc_e_areatype']));
		$commands[] = ajax_command_replace('#bocwa_common_1_replace', drupal_render($form['new_bocwa_reg']['loc_e_areatype_code']));
		$commands[] = ajax_command_replace('#bocwa_common_2_replace', drupal_render($form['new_bocwa_reg']['loc_e_vill_ward']));
		$commands[] = ajax_command_replace('#bocwa_ps_replace', drupal_render($form['new_bocwa_reg']['loc_e_ps']));
  		return array('#type' => 'ajax', '#commands' => $commands);
}

function get_bocwa_e_postal_address__ajax_callback($form, $form_state) {
	
		$commands = array();
		
		$commands[] = ajax_command_replace('#bocwa_e_postal_subdvsn_replace', drupal_render($form['new_bocwa_reg']['e_postal_subdivision']));
		$commands[] = ajax_command_replace('#bocwa_e_postal_loc_e_areatype', drupal_render($form['new_bocwa_reg']['e_postal_areatype']));
		$commands[] = ajax_command_replace('#bocwa_e_postal_common_1_replace', drupal_render($form['new_bocwa_reg']['e_postal_areatype_code']));
		$commands[] = ajax_command_replace('#bocwa_e_postal_common_2_replace', drupal_render($form['new_bocwa_reg']['e_postal_vill_ward']));
		$commands[] = ajax_command_replace('#bocwa_e_postal_ps_replace', drupal_render($form['new_bocwa_reg']['e_postal_ps']));
		
  		return array('#type' => 'ajax', '#commands' => $commands);
}

function get_bocwa_e_permanent_address_ajax_callback($form, $form_state) {
	
		$commands = array();
		
		$commands[] = ajax_command_replace('#bocwa_e_permanent_add_subdvsn_replace', drupal_render($form['new_bocwa_reg']['e_permanent_add_subdivision']));
		$commands[] = ajax_command_replace('#bocwa_e_permanent_add_loc_e_areatype', drupal_render($form['new_bocwa_reg']['e_permanent_add_areatype']));
		$commands[] = ajax_command_replace('#bocwa_e_permanent_common_1_replace', drupal_render($form['new_bocwa_reg']['e_permanent_add_areatype_code']));
		$commands[] = ajax_command_replace('#bocwa_e_permanent_common_2_replace', drupal_render($form['new_bocwa_reg']['e_permanent_add_vill_ward']));
		$commands[] = ajax_command_replace('#bocwa_e_permanent_add_ps_replace', drupal_render($form['new_bocwa_reg']['e_permanent_add_ps']));
		
  		return array('#type' => 'ajax', '#commands' => $commands);
}

function get_bocwa_emp_ajax_callback($form, $form_state) {
	
		$commands = array();
		
		$commands[] = ajax_command_replace('#bocwa_emp_subdvsn_replace', drupal_render($form['new_bocwa_reg']['emp_subdivision']));
		$commands[] = ajax_command_replace('#bocwa_emp_areatype_replace', drupal_render($form['new_bocwa_reg']['emp_areatype']));
		$commands[] = ajax_command_replace('#bocwa_emp_common_1_replace', drupal_render($form['new_bocwa_reg']['emp_areatype_code']));
		$commands[] = ajax_command_replace('#bocwa_emp_common_2_replace', drupal_render($form['new_bocwa_reg']['emp_vill_ward']));
		$commands[] = ajax_command_replace('#bocwa_emp_ps_replace', drupal_render($form['new_bocwa_reg']['emp_ps']));
		
  		return array('#type' => 'ajax', '#commands' => $commands);
}



function get_bocwa_address_manager_ajax_callback($form, $form_state) {
		
		$commands = array();
		
		$commands[] = ajax_command_replace('#bocwa_address_manager_subdvsn_replace', drupal_render($form['new_bocwa_reg']['address_manager_subdivision']));
		$commands[] = ajax_command_replace('#bocwa_address_manager_areatype', drupal_render($form['new_bocwa_reg']['address_manager_areatype']));
		$commands[] = ajax_command_replace('#bocwa_address_manager_common_1_replace', drupal_render($form['new_bocwa_reg']['address_manager_areatype_code']));
		$commands[] = ajax_command_replace('#bocwa_address_manager_common_2_replace', drupal_render($form['new_bocwa_reg']['address_manager_vill_ward']));
		$commands[] = ajax_command_replace('#bocwa_address_manager_ps_replace', drupal_render($form['new_bocwa_reg']['address_manager_ps']));
		
  		return array('#type' => 'ajax', '#commands' => $commands);
}


