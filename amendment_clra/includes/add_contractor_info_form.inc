<?php

function add_contractor_info_form($form, &$form_state, $id=''){
		
	global $base_root,$base_path,$user;
	
	drupal_add_js(drupal_get_path('module', 'applicant_forms') . '/js/contractor_tab.js');
	drupal_add_js(drupal_get_path('module', 'applicant_forms') . '/js/custom.js');	

	
	$user_id 				= $user->uid;
	$action					=  'decrypt';
	$action_encrypt			=  'encrypt';
	$application_id			=  encryption_decryption_fun($action, $id);
		
		
		
	$fetch_common_det 	= db_query("select status from l_common_application_master where user_id=:userId and id=:applicationid",array(':userId'=>$user_id,':applicationid'=>$application_id));
		
	if( $fetch_common_det->rowCount() > 0 ){
		foreach($fetch_common_det as $obj){
			$_SESSION['tabStatus']	=	isset($obj->status) ? $obj->status : 0;
		}
	}
		
	drupal_add_library('system','ui.datepicker');
		
	$fetch_data_from_clra_master = db_query("select lcrm.id, lcrm.final_submit_status, lcrm.identification_number, lcrm.amendment_parent_id , lcrm.e_any_day_max_num_of_workmen, lcrm.status from l_clra_registration_master lcrm where lcrm.user_id=:user_id and lcrm.id=:applicationid", array(':user_id' => $user_id, ':applicationid' =>$application_id));
			
	if( $fetch_data_from_clra_master->rowCount() > 0 ){
				
				while($fetch_data_from_clra_master_res 	= $fetch_data_from_clra_master->fetchObject()){ 
					$application_id 					= $fetch_data_from_clra_master_res->id;
					$identification_number 				= $fetch_data_from_clra_master_res->identification_number;
					$final_submit_status 				= $fetch_data_from_clra_master_res->final_submit_status;
					$status								= $fetch_data_from_clra_master_res->status;
					$finalNumber						= $fetch_data_from_clra_master_res->e_any_day_max_num_of_workmen;
					$amendment_parent_id				= $fetch_data_from_clra_master_res->amendment_parent_id;
				}
			}
			
	$fetchInsertedTillNow			= db_query("select sum(lcim.contractor_max_no_of_labours_on_any_day) from l_contractor_info_master lcim where lcim.user_id=:user_id and lcim.application_id=:applicationid", array(':user_id' => $user_id, ':applicationid' =>$application_id));
	if( $fetchInsertedTillNow->rowCount() > 0 ){
		$fetchInsertedTillNow_res		= $fetchInsertedTillNow->fetchAssoc();
	}
			
			
	$form['contractor_master']['header']								= array(
																			   '#markup' 		=> t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">Particulars of Contractors and Contract Labour</div><div class="panel-body">'),
																			);
									
	$form['contractor_master']['act_id'] 				    			= array(
																			  '#type' 			=> 'hidden',
																			  '#default_value'	=> 1,
																			  '#attributes' 	=> array('readonly'=>'readonly')
																			);
									
									
	$form['contractor_master']['application_id'] 						= array(
																			  '#type' 			=> 'hidden',
																			  '#default_value'	=> !empty($application_id) ? $application_id : '',
																			  '#attributes' 	=> array('readonly'=>'readonly')
																			);
							
	$form['contractor_master']['identification_number'] 				= array(
																			  '#type' 				=> 'hidden',
																			  '#default_value'		=> !empty($identification_number) ? $identification_number : '',
																			  '#attributes' 		=> array('readonly'=>'readonly')
																			);
																						
	$form['contractor_master']['amendment_parent_id'] 					= array(
																			  '#type' 				=> 'hidden',
																			  '#default_value'		=> !empty($amendment_parent_id) ? $amendment_parent_id : '',
																			  '#attributes' 		=> array('readonly'=>'readonly')
																			);
			
	$form['contractor_master']['contractor_type'] 						= array(									  	
																			  '#type' 		   		=> 'hidden',
																			  '#default_value' 		=> 2								  
																			);
																						
	/*$contractor_type	 =	array( 1 => 'Offline generated License', 2 => 'New Contractor');

$form['contractor_master']['contractor_type']	 						= array(
																				'#title'	 		=>	'Contractor has/is',
																				'#prefix'	 		=>	'<fieldset><section><label class="select">',
																				'#suffix'	 		=>	'</label></section>',
																				'#type'		 		=>	'select',
																				'#options'	 		=>	$contractor_type,
																				'#required'	 		=>	TRUE,
																				'#default_value'	=>  2,
																				'#attributes'		=>	array('id' => 'contractor_type_id')
																			);	*/																		
									
	$form['contractor_master']['name_of_contractor'] 					= array(
																			  '#prefix' 			=> '<div class="col-md-6"><label class="input">',
																			  '#suffix' 			=> '</label></div>',
																			  '#title'				=>'1. Name of the Contractor',
																			  '#type' 				=> 'textfield',
																			  '#required' 			=> TRUE,
																			  '#element_validate' 	=> array('DASHBOARD_SPECIAL_CHAR_TEXT_VALIDATE'),
																			  '#autocomplete_path' 	=> 'applicant-profile/autocomplete',
																			);
			
	$form['contractor_master']['email_of_contractor'] 						= array(
																				  '#prefix' 			=> '<section><label class="input">',
																				  '#suffix' 			=> '</label></section>',
																				  '#title'  			=> '2. Email of the Contractor',
																				  '#type'     			=> 'textfield',
																				  '#required' 			=> TRUE,
																				  '#element_validate' 	=> array('CONTRACTOR_TEXTBOX_EMAIL_VALIDATE')
																				);
	
							
	$form['contractor_master']['address_of_contractor'] 					= array(
																				  '#prefix' 			=> '<section><label class="textarea">',
																				  '#suffix' 			=> '</label></section>',
																				  '#title'				=> '3. Address of the Contractor',
																				  '#type' 				=> 'textarea',
																				  '#required' 			=> TRUE,
																				  '#rows' 				=> 2,
																				  '#element_validate' 	=> array('DASHBOARD_SPECIAL_CHAR_TEXT_VALIDATE')
																				);
									
			//////////////////////  Address Break Up ///////////////////////////////////
			
			$state_opts																=	array( 1 => 'West Bengal', 2 => 'Other State' );
			
			$form['contractor_master']['state_opts']								=	array(
																							'#title'			=>	'4. Select State',
																							'#prefix'			=>	'<section><label class="select">',
																							'#suffix'			=>	'</label></section>',
																							'#type'				=>	'select', 
																							'#options'			=>	$state_opts,
																							'#required'			=>	TRUE,
																							'#ajax'				=> 	array(
																														'event' 		=> 'change',
																														'effect' 		=> 'fade',			
																														'callback'		=> 'my_amendment_ajax_callback',
																														'progress'		=> '',
																													)
																						);
																						
			$value_state_opts 														= 	isset($form_state['values']['state_opts']) ? $form_state['values']['state_opts'] : '';
																						
			$form['contractor_master']['state_others']								=	array(
																							'#prefix'			=>	'<div id="textarea_amendment1">',
																							'#suffix'			=>	'</div>'
																						);
																				
			$form['contractor_master']['state_wb_s']								=	array(
																							'#prefix'			=>	'<div id="textarea_amendment2">',
																							'#suffix'			=>	''
																						);
			
			$form['contractor_master']['state_wb_e']								=	array(
																							'#prefix'			=>	'',
																							'#suffix'			=>	'</div>'
																						);
																						
																						
			if($value_state_opts == 1 && $value_state_opts != "" && !empty($value_state_opts)){
				
				$form['contractor_master']['state_wb_s']							=	array(
																							'#prefix'			=>	'<div id="textarea_amendment2">',
																							'#suffix'			=>	''
																							);
			
				$options_dist 	= custom_user_district_list('all'); //---come from custom_user module
		
			$form['contractor_master']['state_wb_s']['con_loc_e_dist'] 				= array(
																						  '#prefix' 			=> '<div class="row"><section class="col col-6"><label class="select">',
																						  '#suffix' 			=> '</label></section>',
																						  '#title'				=>'4.a.) Select District',
																						  '#type' 				=> 'select',
																						  '#options' 			=> $options_dist,
																						  '#required' 			=> TRUE,
																						  '#default_value' 		=> !empty($loc_e_dist) ? $loc_e_dist	:'',
																						  '#ajax'				=> array(
																													'event' 			=> 'change',
																													'effect' 			=> 'fade',			
																													'callback'			=> 'get_con_registration_info_form_ajax_callback',
																													'progress'			=> '',
																												  )
																						);
											
			$value_loc_e_dist 	= 	isset($form_state['values']['con_loc_e_dist']) ? $form_state['values']['con_loc_e_dist'] : '';
			
			if(empty($value_loc_e_dist) &&!empty($loc_e_dist))
					$value_loc_e_dist	=	$loc_e_dist;
						
			$form['contractor_master']['state_wb_s']['con_loc_e_subdivision'] 						= array(
																							'#prefix' 				=> '<div id="clra_subdivision_replace">',
																							'#suffix' 				=> '</div>',
																						);
		
			$form['contractor_master']['state_wb_s']['con_loc_e_subdivision'] 						= array(
																							  '#prefix' 			=> '<div id="clra_subdvsn_replace"><section class="col col-6"><label class="select">',
																							  '#suffix' 			=> '</label></section></div></div>',
																							  '#title'				=>'4.b.) Select Subdivision',
																							  '#type' 				=> 'select',
																							  '#options' 			=> custom_user_subdivisionlist_list($value_loc_e_dist),
																							  '#default_value' 		=> !empty($loc_e_subdivision) ? $loc_e_subdivision	:'',
																							  '#required' 			=> TRUE
																						);
															
			$form['contractor_master']['state_wb_s']['con_loc_e_areatype'] 							= array(
																							  '#prefix' 			=> '<div class="row"><section class="col col-6"><label class="select">',
																							  '#suffix' 			=> '</label></section>',
																							  '#title'				=>'4.c.) Select Area type',
																							  '#type' 				=> 'select',
																							  '#options' 			=>array('T'=>'Select Area Type','B' =>'Block','M' => 'Municipality','C' => 'Corporation', 'S' =>'SEZ','N' =>'Notified Area'),
																							  '#default_value' 		=> !empty($loc_e_areatype) ? $loc_e_areatype	:'',
																							  '#element_validate' 	=> array('contractor_areatype_validate'),
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_con_registration_info_form_ajax_callback',
																														'progress'			=> '',
																													  ),
																								);
			$form['contractor_master']['state_wb_s']['con_loc_e_block'] 								= array(
																							'#prefix' 				=> '<div id="clra_block_replace">',
																							'#suffix' 				=> '</div>'
																						 );
														  
			$form['contractor_master']['state_wb_s']['con_loc_e_mun'] 								= array(
																							'#prefix' 				=> '<div id="clra_mun_replace">',
																							'#suffix' 				=> '</div>'
																							);
														  
			$form['contractor_master']['state_wb_s']['con_loc_e_corp'] 								= array(
																							'#prefix' 				=> '<div id="clra_other_replace">',
																							'#suffix' 				=> '</div>'
																						 );
														  
			$form['contractor_master']['state_wb_s']['con_loc_e_corp'] 								= array(
																							'#prefix' 				=> '<div id="clra_corp_replace">',
																							'#suffix' 				=> '</div>'
																							);
														  
			$form['contractor_master']['state_wb_s']['con_loc_e_sez'] 								= array(
																							'#prefix' 				=> '<div id="clra_sez_replace">',
																							'#suffix' 				=> '</div>'
																							  );
														  
			$form['contractor_master']['state_wb_s']['con_loc_e_not'] 								= array(
																							'#prefix' 				=> '<div id="clra_not_replace">',
																							'#suffix' 				=> '</div>'
																						  );
			$value_loc_e_areatype 		= isset($form_state['values']['con_loc_e_areatype']) ? $form_state['values']['con_loc_e_areatype'] : '';
			$value_loc_e_subdivision 	= isset($form_state['values']['con_loc_e_subdivision']) ? $form_state['values']['con_loc_e_subdivision'] : '';
	
			if(empty($value_loc_e_areatype) && !empty($loc_e_areatype))
						$value_loc_e_areatype=$loc_e_areatype;
		
			if(empty($value_loc_e_subdivision) && !empty($loc_e_subdivision))
						$value_loc_e_subdivision=$loc_e_subdivision;
		
			if(!empty($value_loc_e_areatype)){
				if($value_loc_e_areatype == 'B'){
					$form['contractor_master']['state_wb_s']['con_loc_e_block'] 						= array(
																							  '#prefix' 			=> '<div id="clra_block_replace"><section class="col col-6"><label class="select">',
																							  '#suffix' 			=> '</label></section></div>',
																							  '#title'				=>'4.d.) Select Block',
																							  '#type' 				=> 'select',
																							  '#options' 			=> custom_user_block_list($value_loc_e_subdivision,$value_loc_e_areatype),
																							  '#default_value' 		=> !empty($loc_name_areatype) ? $loc_name_areatype	:'',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_con_registration_info_form_ajax_callback',
																															'progress'			=> '',
																														  )
																						 );
				}else if($value_loc_e_areatype == 'M'){
					$form['contractor_master']['state_wb_s']['con_loc_e_mun'] 						= array(
																							  '#prefix' 			=> '<div id="clra_mun_replace"><section class="col col-6"><label class="select">',
																							  '#suffix' 			=> '</label></section></div>',
																							  '#title'				=>'4.d.) Select Municipality',
																							  '#type' 				=> 'select',
																							  '#options' 			=> custom_user_block_list($value_loc_e_subdivision,$value_loc_e_areatype),
																							  '#default_value' 		=> !empty($loc_name_areatype) ? $loc_name_areatype	:'',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_con_registration_info_form_ajax_callback',
																														'progress'			=> '',
																													  )
																							);
				}else if($value_loc_e_areatype == 'C'){
					$form['contractor_master']['state_wb_s']['con_loc_e_corp'] 						= array(
																							  '#prefix' 			=> '<div id="clra_corp_replace"><section class="col col-6"><label class="select">',
																							  '#suffix' 			=> '</label></section></div>',
																							  '#title'				=>'4.d.) Select Corporation',
																							  '#type' 				=> 'select',
																							  '#options' 			=> custom_user_block_list($value_loc_e_subdivision,$value_loc_e_areatype),
																							  '#default_value' 		=> !empty($loc_name_areatype) ? $loc_name_areatype	:'',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_con_registration_info_form_ajax_callback',
																															'progress'			=> '',
																														  )
																						);
				}else if($value_loc_e_areatype == 'S'){
					$form['contractor_master']['state_wb_s']['con_loc_e_sez'] 						= array(
																							  '#prefix' 			=> '<div id="clra_sez_replace"><section class="col col-6"><label class="select">',
																							  '#suffix' 			=> '</label></section></div>',
																							  '#title'				=>'4.d.) Select SEZ',
																							  '#type' 				=> 'select',
																							  '#options' 			=> custom_user_block_list($value_loc_e_subdivision,$value_loc_e_areatype),
																							  '#default_value' 		=> !empty($loc_name_areatype) ? $loc_name_areatype	:'',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_con_registration_info_form_ajax_callback',
																															'progress'			=> '',
																														  )
																							);
			}else if($value_loc_e_areatype == 'N'){
				$form['contractor_master']['state_wb_s']['con_loc_e_not'] 							= array(
																							  '#prefix' 			=> '<div id="clra_not_replace"><section class="col col-6"><label class="select">',
																							  '#suffix' 			=> '</label></section></div>',
																							  '#title'				=>'4.d.) Select Notified area',
																							  '#type' 				=> 'select',
																							  '#options' 			=> custom_user_block_list($value_loc_e_subdivision,$value_loc_e_areatype),
																							  '#default_value' 		=> !empty($loc_name_areatype) ? $loc_name_areatype	:'',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_con_registration_info_form_ajax_callback',
																															'progress'			=> '',
																														  )
																							);
			}
			}
		
			$code				=	"";
			$value_loc_e_block 	= 	isset($form_state['values']['con_loc_e_block']) ? $form_state['values']['con_loc_e_block'] : '';
			$value_loc_e_mun	= 	isset($form_state['values']['con_loc_e_mun']) ? $form_state['values']['con_loc_e_mun'] : '';
			$value_loc_e_corp 	= 	isset($form_state['values']['con_loc_e_corp']) ? $form_state['values']['con_loc_e_corp'] : '';
			$value_loc_e_sez 	= 	isset($form_state['values']['con_loc_e_sez']) ? $form_state['values']['con_loc_e_sez'] : '';
			$value_loc_e_not 	= 	isset($form_state['values']['con_loc_e_not']) ? $form_state['values']['con_loc_e_not'] : '';
			if(empty($value_loc_e_block)){
				if(empty($value_loc_e_mun)){
					if(empty($value_loc_e_corp)){
						if(empty($value_loc_e_sez)){
							if(empty($value_loc_e_not)){
								$code	=	"";
							}else
								$code=$value_loc_e_not;
							}
							else
								$code=$value_loc_e_sez;
							}
							else
								$code=$value_loc_e_corp;
							}
					else
						$code=$value_loc_e_mun;
				}
				else
					$code=$value_loc_e_block;
					
			if(empty($code) && !empty($loc_e_vill_ward))
				$code = $loc_name_areatype;
		
			$form['contractor_master']['state_wb_s']['con_loc_e_vill'] 								= array(
																							'#prefix' 				=> '<div id="clra_vill_replace">',
																							'#suffix' 				=> '</div>'
																							 );
													 
			$form['contractor_master']['state_wb_s']['con_loc_e_ward'] 								= array(
																							'#prefix' 				=> '<div id="clra_ward_replace">',
																							'#suffix' 				=> '</div>'
																						 );
													 
			$form['contractor_master']['state_wb_s']['con_loc_e_bugh'] 								= array(
																							'#prefix' 				=> '<div id="clra_bugh_replace">',
																							'#suffix' 				=> '</div>'
																							);
																							
			$form['contractor_master']['state_wb_s']['con_loc_e_sector'] 								= array(
																							'#prefix' 				=> '<div id="clra_sector_replace">',
																							'#suffix' 				=> '</div>'
																							);
																							
			$form['contractor_master']['state_wb_s']['con_loc_e_notifyarea'] 							= array(
																							'#prefix' 				=> '<div id="clra_notified_area_replace">',
																							'#suffix' 				=> '</div>'
																						);
													 
			$form['contractor_master']['state_wb_s']['con_code'] 										= array(
																							'#prefix' 				=> '<div id="clra_code_replace">',
																							'#suffix' 				=> '</div>'
																						 );
													 
			if(!empty($code)){
				
				$form['contractor_master']['state_wb_s']['con_code'] 									= array(
																							'#prefix' 				=> '<div id="clra_code_replace">',
																							'#suffix' 				=> '</div>',
																							'#type' 				=> 'hidden',
																							'#value'				=> !empty($code) ? $code	:''
																					  );
				
	
				if($value_loc_e_areatype == 'B'){
					$form['contractor_master']['state_wb_s']['con_loc_e_vill'] 						= array(
																							  '#prefix' 			=> '<div id="clra_vill_replace"><section class="col col-4"><label class="select">',
																							  '#suffix' 			=> '</label></section></div>',
																							  '#title'				=>'4.e.) Select Gram Panchayat',
																							  '#type' 				=> 'select',
																							  '#options' 			=> custom_user_villward_list($code,'V'),// come from custom_user module
																							  '#default_value' 		=> !empty($loc_e_vill_ward) ? $loc_e_vill_ward	:'',
																							  '#required' 			=> TRUE,
																							);
				}
				
				if($value_loc_e_areatype == 'M'){
					$form['contractor_master']['state_wb_s']['con_loc_e_ward'] 						= array(
																							  '#prefix' 			=> '<div id="clra_ward_replace"><section class="col col-4"><label class="select">',
																							  '#suffix' 			=> '</label></section></div>',
																							  '#title'				=> '4.e.)Select Ward',
																							  '#type' 				=> 'select',
																							  '#options' 			=> custom_user_villward_list($code,'W'),// come from custom_user module
																							  '#default_value' 		=> !empty($loc_e_vill_ward) ? $loc_e_vill_ward	:'',
																							  '#required' 			=> TRUE,
																							);
				}
				
				if($value_loc_e_areatype == 'C'){
					$form['contractor_master']['state_wb_s']['con_loc_e_bugh'] 						= array(
																							  '#prefix' 			=> '<div id="clra_bugh_replace"><section class="col col-4"><label class="select">',
																							  '#suffix' 			=> '</label></section></div>',
																							  '#title'				=>'4.e.) Ward of the Contractor',
																							  '#type' 				=> 'select',
																							 // '#options' 			=> custom_user_borough_list($code),// come from custom_user module
																							  '#options' 			=> custom_user_villward_list($code,'C'),// come from custom_user module
																							  '#default_value' 		=> !empty($loc_e_borough) ? $loc_e_borough	:'',
																							  '#required' 			=> TRUE,
																							);
				}
				if($value_loc_e_areatype == 'S'){
					$form['contractor_master']['state_wb_s']['con_loc_e_sector'] 						= array(
																							  '#prefix' 			=> '<div id="clra_sector_replace"><section class="col col-4"><label class="select">',
																							  '#suffix' 			=> '</label></section></div>',
																							  '#title'				=>'4.e.) Select Sector',
																							  '#type' 				=> 'select',
																							  '#options' 			=> custom_user_villward_list($code,'S'),// come from custom_user module
																							  '#default_value' 		=> !empty($loc_e_vill_ward) ? $loc_e_vill_ward	:'',
																							  '#required' 			=> TRUE,
																							);
				}
				if($value_loc_e_areatype == 'N'){
					$form['contractor_master']['state_wb_s']['con_loc_e_notifyarea'] 		= array(
																								  '#prefix' 			=> '<div id="clra_notified_area_replace"><section class="col col-4"><label class="select">',
																								  '#suffix' 			=> '</label></section></div>',
																								  '#title'				=>'5.e.) Select Notified Area',
																								  '#type' 				=> 'select',
																								  '#options' 			=> custom_user_villward_list($code,'N'),// come from custom_user module
																								  '#default_value' 		=> !empty($loc_e_vill_ward) ? $loc_e_vill_ward	:'',
																								  '#required' 			=> TRUE,
																								);
					
				}
				
			}
		
			$form['contractor_master']['state_wb_s']['con_loc_e_ps'] 									= array(
																							'#prefix' 				=> '<div id="clra_ps_replace">',
																							'#suffix' 				=> '</div>'
																							 );
												   
			$form['contractor_master']['state_wb_s']['con_loc_e_ps'] 									= array(
																							  '#prefix' 			=> '<div id="clra_ps_replace"><section class="col col-4"><label class="select">',
																							  '#suffix' 			=> '</label></section></div>',
																							  '#title'				=>'4.f.) Select Police Station',
																							  '#type' 				=> 'select',
																							  '#options' 			=> custom_user_ps_list($value_loc_e_dist),// come from custom_user module
																							  '#default_value' 		=> !empty($l_e_ps) ? $l_e_ps	:'',
																							  '#required' 			=> TRUE  
																							);
																							
			$form['contractor_master']['state_wb_s']['contractor_pin'] 								= array(
																							  '#prefix' 			=> '<section class="col col-4"><label class="input">',
																							  '#suffix' 			=> '</label></section></div>',
																							  '#title'				=>'4.g.) PIN Code',
																							  '#type' 				=> 'textfield',
																							  '#attributes'			=>  array('maxlength' => '6'),
																							  '#required' 			=> TRUE  
																							);
																							
				$form['contractor_master']['state_wb_e']						=	array(
																						'#prefix'			=>	'',
																						'#suffix'			=>	'</div>'
																					);
												
			}
			if($value_state_opts == 2 && $value_state_opts != "" && !empty($value_state_opts)){
				$form['contractor_master']['state_others']					=	array(
																					'#prefix'			=>	'<div id="textarea_amendment1"><section><label class="textarea">',
																					'#suffix'			=>	'</label></section></div>',
																					'#type'				=>	'textarea',
																					'#title'			=>	'5.a.) Other State',
																					'#required'			=>	TRUE
																				);
			}
			
			
			
			$insertedNatureOfWork 	= 	contractor_works_multiSelect_options();
			
			$form['contractor_master']['contractor_nature_of_work'] 					= array( 
																							  '#prefix' 			=> '<section><label class="select">',
																							  '#suffix' 			=> '</label></section>',
																							  '#title'				=>'5.Nature of Work in which Contract Labour is Employed or is to be Employed',
																							  '#type' 				=> 'select',
																							  '#options' 			=> $insertedNatureOfWork,
																							  '#required' 			=> TRUE,
																							  '#multiple' 			=> TRUE,
																							  '#attributes' 		=> array('style'=>"height: 65px;", 'class'=> array('selected disabled')),
																							  '#element_validate' 	=> array('DASHBOARD_CONTRACTOR_TYPE_VALIDATE')
																							);
			
			$deductedvalue = ($finalNumber-$fetchInsertedTillNow_res['sum']);
			
			
	if( $deductedvalue == 0 ){
				//$readonly_contractor_max_no_of_labours_on_any_day 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
		$form['contractor_master']['contractor_max_no_of_labours_on_any_day'] 	= array(
																							  '#prefix' 			=> '<section><label class="input">',
																							  '#suffix' 			=> '</label></section>',
																							  '#title'				=>'6. Maximum Number of Contractor Labour to be Employed on any day Through Each Contractor to be engaged',
																							  '#type' 				=> 'textfield',
																							  '#required'			=> TRUE,
																							  //'#description'		=> t('<font color="red">Note: Maximum Number of Contractor Labour to be Employed on any day Through Each Contractor should not exceed '.$finalNumber.'.<br> Please increase the number by clicking the first tab and then scroll down to 5.a) Maximum Number of Workmen Employed Directly on any day in the Establishment.</font><br>'.'Total Number = <font color="red">'.$finalNumber.'</font><br> Inserted Till Now = <font color="red">'.$fetchInsertedTillNow_res['sum'].'</font><br>Left Numbers = <font color="red">'.($finalNumber-$fetchInsertedTillNow_res['sum']).'</font>'),
																							  //'#attributes' 		=> $readonly_contractor_max_no_of_labours_on_any_day,
																							  '#element_validate'   => array(/*'DASHBOARD_CONTRACTOR_NUMBER_VALIDATION', */'DASHBOARD_TEXTBOX_NUMERIC_VALIDATE')
																							);	
			}else{
				$form['contractor_master']['contractor_max_no_of_labours_on_any_day'] 		= array(
																							  '#prefix' 			=> '<section><label class="input">',
																							  '#suffix' 			=> '</label></section>',
																							  '#title'				=>'6. Maximum Number of Contractor Labour to be Employed on any day Through Each Contractor to be engaged',
																							  '#type' 				=> 'textfield',
																							  '#required'			=> TRUE,
																							  //'#description'		=> t('<font color="red">Note:</font> Maximum Number of Contractor Labour to be Employed on any day Through Each Contractor should not exceed '.$finalNumber.'.<br> Please increase the number by clicking the first tab and then scroll down to 5.a) Maximum Number of Workmen Employed Directly on any day in the Establishment.<br>'.'Total Number = <font color="red">'.$finalNumber.'</font><br> Inserted Till Now = <font color="red">'.$fetchInsertedTillNow_res['sum'].'</font><br>Left Numbers = <font color="red">'.($finalNumber-$fetchInsertedTillNow_res['sum']).'</font>'),
																							  '#element_validate'   => array(/*'DASHBOARD_CONTRACTOR_NUMBER_VALIDATION', */'DASHBOARD_TEXTBOX_NUMERIC_VALIDATE')
																							);
			}
			
			$form['contractor_master']['est_date_of_work_of_each_labour'] 				= array(
																							  '#prefix' 			=> '<section><label class="input">',
																							  '#suffix' 			=> '</label></section>',
																							  '#type' 				=> 'markup',
																							  '#markup'				=>'7. Estimated Date of Employment of Each Contract Work Under Each Contractor <span style="color:#f00;">* </span>',
																							);
			
			$form['contractor_master']['est_date_of_work_of_each_labour_from_date'] 	= array(
																							  '#prefix' 		=> '<div class="row"><div class="col-md-4"><label class="input">',
																							  '#suffix' 		=> '</label></div>',
																							  '#type' 				=> 'textfield',
																							  '#required' 			=> TRUE,
																							  '#attributes'	=>	array('id' => 'est_date_of_commencement_id', /*'class'=> array('est_date_of_work_of_each_labour_from_date_class'),*/ 'readonly'=>'readonly','placeholder' => t('From'), 'onclick' => 'return datepicker_func();'),
																							);
									
			$form['contractor_master']['est_date_of_work_of_each_labour_to_date'] 		= array(
																							  '#prefix' 		=> '<div class="col-md-4"><label class="input">',
																							  '#suffix' 		=> '</label></div>',
																							  '#type' 				=> 'textfield',
																							  '#required' 			=> TRUE,
																							  '#attributes'	=>	array('id' => 'est_date_of_termination_id', /*'class'=> array('est_date_of_work_of_each_labour_to_date_class'),*/ 'readonly'=>'readonly', 'placeholder'=> t('To'),'onclick' => 'return datepicker_func();'),
																							);	
									
			$form['contractor_master']['est_date_of_work_of_each_labour_total_months'] 	= array(
																							  '#prefix' 			=> '<div class="col-md-4"><label class="input">',
																							  '#suffix' 			=> '</label></div></div></div></div></div>',
																							  '#type' 				=> 'textfield',
																							  '#required' 			=> TRUE,
																							  '#attributes'			=>	array('id'=> 'est_date_of_work_of_each_labour_total_months_edit_id', 'readonly'=>'readonly','placeholder' => t('Total Days')),
																							);
			
			if( $status =='B' || $final_submit_status =='' ){											
				$form['contractor_master']['submit'] 									= array (
																							  '#type' 				=> 'submit',
																							  '#attributes' 		=> array('class' => array('btn btn-primary pull-right')),
																							  '#value' 				=> 'SAVE'
																						   );
			}else{
				
			$form['contractor_master']['row_div'] 		= array (
																 '#prefix' 		=> '<div class="row">&nbsp;&nbsp;</div><div class="row">&nbsp;&nbsp;</div>',
														 );		
			}
			
			$form['contractor_master']['go_to_dashboard'] = array(
															'#type' 		=> 'markup',
															'#prefix' 		=> '',  
															'#suffix' 		=> '',
															'#markup'	    => l('GO TO DASHBOARD', 'applicant-dashboard', array('attributes' => array('class' => 'link_btn'))));
			
			$form['contractor_master']['back_link'] = array(
															'#type' 		=> 'markup',
															'#suffix' 		=> '</div>',
															'#markup'	    => l('BACK TO CONTRACTOR LIST', 'clra-reg-amendment/clra-contractor-info/'.encryption_decryption_fun($action_encrypt, $application_id), array('attributes' => array('class' => 'link_btn'))));
														
			
		return $form;
	}
	
	function add_contractor_info_form_validate($form, &$form_state){
		
		global $base_root, $base_path, $user;
		
		$user_id 									= $user->uid;
		$contractor_max_no_of_labours_on_any_day 	= $form_state['values']['contractor_max_no_of_labours_on_any_day'];
		$application_id 							= $form_state['values']['application_id'];
		
		/////////////////////////////  Contractor Number Checking //////////////////////////////////////////
			
		$fetchInsertedTillNow			= db_query("select sum(lcim.contractor_max_no_of_labours_on_any_day) from l_contractor_info_master lcim where lcim.user_id=:user_id and lcim.application_id=:applicationid", array(':user_id' => $user_id, ':applicationid' =>$application_id));
		if( $fetchInsertedTillNow->rowCount() > 0 ){
			$fetchInsertedTillNow_res		= $fetchInsertedTillNow->fetchAssoc();
			$TillNowValue					= $fetchInsertedTillNow_res['sum'];	
		}
		
		$fetch_data_from_clra_master = db_query("select lcrm.e_any_day_max_num_of_workmen from l_clra_registration_master lcrm where lcrm.user_id=:user_id and lcrm.id=:applicationid", array(':user_id' => $user_id, ':applicationid' =>$application_id));
		
		if( $fetch_data_from_clra_master->rowCount() > 0 ){
			$fetch_data_from_clra_master_res		= $fetch_data_from_clra_master->fetchAssoc();
			$FirstValue								= $fetch_data_from_clra_master_res['e_any_day_max_num_of_workmen'];	
		}
		$addedValue									= ($TillNowValue+$contractor_max_no_of_labours_on_any_day);
		
		/*if( $addedValue > $FirstValue ){
			form_error($form['contractor_master']['contractor_max_no_of_labours_on_any_day'], t('Number SHOULD not BE greater'));
			$form_state['rebuild'] = TRUE;
		}*/
		/////////////////////////////// End Of Checking //////////////////////////////////////////////////
		
	}
	
	function add_contractor_info_form_submit($form, &$form_state){
		global $base_root, $base_path, $user;
		 
		$increment 										= 0;
		$action_encrypt									= 'encrypt';
		
		$val 											= $form_state['values'];
		
		$application_id									= $val['application_id'];
		$identification_number							= $val['identification_number'];
		$amendment_parent_id							= $val['amendment_parent_id'];
						
		$contractor_type								= $val['contractor_type'];		
		$name_of_contractor 							= $val['name_of_contractor'];
		$email_of_contractor							= $val['email_of_contractor'];
		$address_of_contractor 							= $val['address_of_contractor'];
		
		$state_opts										= $val['state_opts'];
		$con_loc_e_dist 								= $val['con_loc_e_dist'];
		$con_loc_e_subdivision 							= $val['con_loc_e_subdivision'];
		$con_loc_e_areatype 							= $val['con_loc_e_areatype'];
		$con_name_areatype 								= $val['con_code'];
		$con_loc_e_vill 								= $val['con_loc_e_vill'];
		$con_loc_e_ward 								= $val['con_loc_e_ward'];
		$con_loc_e_borough 								= $val['con_loc_e_bugh'];
		$con_loc_e_sector								= $val['con_loc_e_sector'];
		$con_loc_e_not_area								= $val['con_loc_e_notifyarea'];
		
		if(!empty($con_loc_e_vill)){
			$con_loc_e_vill_ward						= $val['con_loc_e_vill'];
		}elseif(!empty($con_loc_e_ward)){
			$con_loc_e_vill_ward						= $val['con_loc_e_ward'];
		}elseif(!empty($con_loc_e_borough)){
			$con_loc_e_vill_ward						= $val['con_loc_e_bugh'];
		}elseif(!empty($con_loc_e_sector)){
			$con_loc_e_vill_ward						= $val['con_loc_e_sector'];
		}elseif(!empty($con_loc_e_not_area)){
			$con_loc_e_vill_ward						= $val['con_loc_e_notifyarea'];
		}
		$con_l_e_ps										= $val['con_loc_e_ps'];
		$contractor_pin									= $val['contractor_pin'];
		$state_others									= $val['state_others'];
		
		
		$contractor_nature_of_work 						= $val['contractor_nature_of_work'];
		$contractor_max_no_of_labours_on_any_day 		= $val['contractor_max_no_of_labours_on_any_day'];
		$est_date_of_work_of_each_labour 				= $val['est_date_of_work_of_each_labour'];
		$est_date_of_work_of_each_labour_from_date  	= date('Y-m-d',strtotime($val['est_date_of_work_of_each_labour_from_date']));
		$est_date_of_work_of_each_labour_to_date    	= date('Y-m-d',strtotime($val['est_date_of_work_of_each_labour_to_date']));
		$est_date_of_work_of_each_labour_total_months 	= $val['est_date_of_work_of_each_labour_total_months'];
		$act_id 										= $val['act_id'];
		$status 										= 1;
		
		
		
		$fieldsConInfo 								= array( 
														'application_id'								=>  $application_id,
														'user_id'										=>	$user->uid,
														'identification_number'							=>	$identification_number,
														
														'contractor_type'								=>	$contractor_type,
														'name_of_contractor'							=>	$name_of_contractor,
														'address_of_contractor'							=>	$address_of_contractor,
														'email_of_contractor'							=>  $email_of_contractor,
														
														'state_opts'									=>	$state_opts,
														'con_loc_e_dist'								=>	$con_loc_e_dist,
														'con_loc_e_subdivision'							=>	$con_loc_e_subdivision,
														'con_loc_e_areatype'							=>	$con_loc_e_areatype,
														'con_name_areatype'								=>	$con_name_areatype,
														'con_loc_e_vill_ward'							=>	$con_loc_e_vill_ward,
														'con_l_e_ps'									=>	$con_l_e_ps,
														'contractor_pin'								=>  $contractor_pin,
														'state_others'									=>	$state_others,
														
														
														'contractor_max_no_of_labours_on_any_day'		=>	$contractor_max_no_of_labours_on_any_day,
														'est_date_of_work_of_each_labour_from_date'		=>	$est_date_of_work_of_each_labour_from_date, 
														'est_date_of_work_of_each_labour_to_date'		=>  $est_date_of_work_of_each_labour_to_date,
														'est_date_of_work_of_each_labour_total_months'	=>  $est_date_of_work_of_each_labour_total_months,
														'act_id'										=>	$act_id, 
														'status'										=>	$status,
														'amendment_id'									=>  $amendment_parent_id,
														);
														
		
		$last_contractor_id= db_insert('l_contractor_info_master')->fields($fieldsConInfo)->execute();
		
		
		if(!empty($contractor_nature_of_work)){
			
			foreach($contractor_nature_of_work as $LoopValueData){
				
				$fieldsAllContractorInfoNatureOfWork 				= 	array( 
																			'contractor_id'                 =>  $last_contractor_id,
																			'identification_number'			=>	$identification_number,
																			'user_id'						=>	$user->uid,
																			'act_id'						=>	$act_id, 
																			'application_id'				=>  $application_id, 
																			'contractor_nature_of_work'		=>	$LoopValueData
																			);
																			
				db_insert('l_clra_con_nature_of_work')->fields($fieldsAllContractorInfoNatureOfWork)->execute();
				
				$increment++;
				
			}
		}
		
		
		if( $last_contractor_id != "" ){
			drupal_goto('clra-reg-amendment/clra-contractor-info/'.encryption_decryption_fun($action_encrypt, $application_id));
		}
	}
	
	function contractor_areatype_validate($element, &$form_state){
		$val						=	$form_state['values'];
		$contractor_areatype		=	$val['con_loc_e_areatype'];
		if($contractor_areatype == "T" ) {
			form_error($element, t($element['#title'].' SHOULD BE VALID'));
	    	$form_state['rebuild'] = TRUE;
		}
	}

	function DASHBOARD_SPECIAL_CHAR_TEXT_VALIDATE($element, &$form_state){
		$txtValue		=	trim($element['#value']); 
		$match_pattern 	= 	'/[A-Za-z0-9]+/';
		if(!preg_match($match_pattern, $txtValue)) {
			form_error($element, t($element['#title'].' SHOULD BE TEXT ONLY'));
		}
	}
	
	function DASHBOARD_CONTRACTOR_TYPE_VALIDATE($element,&$form_state){
		$val						=	$form_state['values'];
		$contractor_nature_of_work	=	$val['contractor_nature_of_work'];
		if($contractor_nature_of_work == 0 ) {
			form_error($element, t($element['#title'].' SHOULD BE VALID'));
	    	$form_state['rebuild'] = TRUE;
		}
	}		
		
	
	function DASHBOARD_TEXTBOX_NUMERIC_VALIDATE($element, &$form_state){
		$fieldValue=trim($element['#value']); 
		$match_pattern = '/^[0-9]+$/i';
		if(!empty($fieldValue)){
			if(!preg_match($match_pattern, $fieldValue)) {
				form_error($element, t($element['#title'].' SHOULD BE NUMBER ONLY'));
			}
		}
	}
	
	function DASHBOARD_CONTRACTOR_NUMBER_VALIDATION($element, &$form_state){
		$fieldValue	=	trim($element['#value']);
		if(!empty($fieldValue) && is_numeric($fieldValue)){
			if( $fieldValue <= 9 ){
				form_error($element, t($element['#title'].' SHOULD BE GREATER OR EQUAL TO 10.'));
			}
		}
	}
	
	function getNatureOfWorkOpts($application_id){
		
		global $base_root, $base_path, $user;
		
		$user_id = $user->uid;
		
		
		
		$contractor_nat_work_query	=	db_query("select cw.contractor_work_id, cw.cont_work_name from l_clra_reg_nature_of_work lcrnow left join contractor_works cw on cw.contractor_work_id=lcrnow.e_nature_of_work where lcrnow.application_id='".$application_id."' and lcrnow.user_id=".$user_id."");
		 
		if(!empty($contractor_nat_work_query)){
			foreach ($contractor_nat_work_query as $resultData){ 
				$res[$resultData->contractor_work_id]	=	$resultData->cont_work_name;
			}
		}
		return $res;
	}
	
	function CONTRACTOR_TEXTBOX_EMAIL_VALIDATE($element,&$form_state){
		$val			=	$form_state['values'];
		$emailval 		= 	'/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,4})$/';
		if(!preg_match($emailval, $val['email_of_contractor'])){ 
			form_error($element, t($element['#title'].' SHOULD BE VALID'));
		}
		/*$validateEmail					=	db_query("select mail from users where mail=:mailval", array(':mailval'=> $val['email_of_contractor']));
		if( $validateEmail->rowCount() > 0 ){
			form_error($element, t($element['#title']. 'already exists.'));
			$form_state['rebuild'] = TRUE;
		}*/
	}
	
	function get_con_registration_info_form_ajax_callback($form, $form_state) {
		$commands = array();
		$commands[] = ajax_command_replace('#clra_subdvsn_replace', drupal_render($form['contractor_master']['state_wb_s']['con_loc_e_subdivision']));
		$commands[] = ajax_command_replace('#clra_mun_replace', drupal_render($form['contractor_master']['state_wb_s']['con_loc_e_mun']));
		$commands[] = ajax_command_replace('#clra_block_replace', drupal_render($form['contractor_master']['state_wb_s']['con_loc_e_block']));
		$commands[] = ajax_command_replace('#clra_other_replace', drupal_render($form['contractor_master']['state_wb_s']['con_loc_e_other']));
		$commands[] = ajax_command_replace('#clra_corp_replace', drupal_render($form['contractor_master']['state_wb_s']['con_loc_e_corp']));
		$commands[] = ajax_command_replace('#clra_sez_replace', drupal_render($form['contractor_master']['state_wb_s']['con_loc_e_sez']));
		$commands[] = ajax_command_replace('#clra_not_replace', drupal_render($form['contractor_master']['state_wb_s']['con_loc_e_not']));
		$commands[] = ajax_command_replace('#clra_vill_replace', drupal_render($form['contractor_master']['state_wb_s']['con_loc_e_vill']));
		$commands[] = ajax_command_replace('#clra_ward_replace', drupal_render($form['contractor_master']['state_wb_s']['con_loc_e_ward']));
		$commands[] = ajax_command_replace('#clra_ps_replace', drupal_render($form['contractor_master']['state_wb_s']['con_loc_e_ps']));
		$commands[] = ajax_command_replace('#clra_bugh_replace', drupal_render($form['contractor_master']['state_wb_s']['con_loc_e_bugh']));
		$commands[] = ajax_command_replace('#clra_sector_replace', drupal_render($form['contractor_master']['state_wb_s']['con_loc_e_sector']));
		$commands[] = ajax_command_replace('#clra_notified_area_replace', drupal_render($form['contractor_master']['state_wb_s']['con_loc_e_notifyarea']));
		$commands[] = ajax_command_replace('#clra_code_replace', drupal_render($form['contractor_master']['state_wb_s']['con_code']));
  		return array('#type' => 'ajax', '#commands' => $commands);
	}
	
	function my_amendment_ajax_callback($form, $form_state){
		$commands	=	array();
		$commands[] = ajax_command_replace('#textarea_amendment1', drupal_render($form['contractor_master']['state_others']));
		$commands[] = ajax_command_replace('#textarea_amendment2', drupal_render($form['contractor_master']['state_wb_s'],$form['contractor_master']['state_wb_e']));
		return array('#type' => 'ajax', '#commands' => $commands);
	}