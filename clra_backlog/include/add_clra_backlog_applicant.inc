<?php
drupal_add_css("sites/all/modules/jquery_ui/jquery.ui/themes/default/ui.datepicker.css");
drupal_add_library('system','ui.datepicker');
		
drupal_add_js('jQuery(document).ready(function(){
					  jQuery("#registration_date").datepicker({
						   dateFormat: "yy-mm-dd",
						   changeMonth: true, 
						   changeYear: true,
						});
					});', 'inline');

function upload_clra_backlog_details($form, &$form_state){
		
	$form['clra_backlog']['note']         = array(                                           
                                            '#prefix' 	 => '<tr><td>',
                                            '#suffix' 	 => '</td></tr>',
											'#type'      => 'markup',
                                            '#markup'    => '<span style="color:#f00;"><i><strong>NOTE:</strong> Click <a href="/clra_backlog">here</a> to back and re-enter your correct registration number or browse and upload your offline previous registration Certificate.</i></span>'
                                        );
											
	$form['clra_backlog']['certificate_file']   = array(
                                            '#prefix' 		 	 => '<tr><td >',
                                            '#suffix' 			 => '</td></tr>',
                                            '#title' 			 => t('Upload Previous Registration Certificate(PDF only)'),
                                            '#required' 		 => TRUE,
                                            '#type' 			 => 'managed_file',
                                            '#upload_validators' => array(
                                                                         'file_validate_extensions' => array('pdf'),
                                                                         'file_validate_size' => array(4*125*1024*1024)
                                                                    ),
                                            '#upload_location' 	=> 'public://upload/backlog_clra_registration_certificate/',
					    					'#process'			=> array('backlog_my_file_element_process')
                                        );   
    
    $form['clra_backlog']['submit'] 	= array (
											'#type' 	=> 'submit',
											'#value' 	=> 'Save',
											'#prefix' 	=> '<tr><td><div>',  
											'#suffix' 	=> '</div></td></tr></table>',
                                        ); 
    return $form;	
}

function upload_clra_backlog_details_submit($form, &$form_state){
    global $base_root, $base_path, $full_base,$user;
    $message	    = 	"";
    $txn 	    	= 	db_transaction();
    $val            =   $form_state['values'];    
    $file_info      =   file_load($val['certificate_file']);
	$identification_number = 'CLRA'.time();	
		   
	if(!empty($file_info)){
		$insermastertblAyyar =  array(														
							'user_id'     			=> $user->uid,
							'identification_number'	=> $identification_number,
							'act_id'				=> 1						
							);
	 	    			
	    $last_application_insert_id = db_insert('l_clra_registration_master')->fields($insermastertblAyyar)->execute();
		
		$file_uri = explode("/", $file_info->uri);
		$tender_file = $file_uri[4];
		$file_info->status = FILE_STATUS_PERMANENT;
		file_save($file_info); // upload file
		$original_file_name = $file_info->filename;
		$fid = trim($file_info->fid);              
	
		// insert data into l_offline_payment_info
		$inserAyyar =  array(															
							'application_id'    	=> $last_application_insert_id,								
							'user_id'     			=> $user->uid,
							'act_id'				=> 1,
							'identification_number'	=> $identification_number,							
							'backlog_clra_registration_certificate_file' => $tender_file
							);
	 	    			
	    $last_documents_id = db_insert('l_documents_upload')->fields($inserAyyar)->execute();
		
		if($last_application_insert_id != ""){
			drupal_goto('view-clra-application-details/view-clra-application/'.encryption_decryption_fun('encrypt', $last_application_insert_id));
		}				
   }
}

function backlog_my_file_element_process($element, &$form_state, $form){
    $element = file_managed_file_process($element, $form_state, $form);
    $element['upload_button']['#access'] = FALSE;
    return $element;
}

function add_clra_backlog_applicant($form, &$form_state){
	
	$form['clra_backlog_master']['registration_no'] 			= array (
																	'#title' 		=>  'Registration Number',
																	'#type'  		=>  'textfield',
																	'#attributes' 	=>  array('placeholder' => 'Enter your Registration Name'),
																	'#required'		=>	TRUE,
																);
	$form['clra_backlog_master']['registration_date'] 			= array (
																	'#title' 		=>  'Registration Date',
																	'#type'  		=>  'textfield',
																	'#date_format' 	=> 'Y-m-d',
																	'#attributes'	=> array('id'=> 'registration_date', 'readonly'=>'readonly', 'placeholder'=> t('Select Date')),
																	'#required'		=>	TRUE,
																);
		 
	$form['clra_backlog_master']['name_of_the_establishment'] 	= array (
																	'#title' 		=>  'Name of the Establishment',
																	'#type'  		=>  'textfield',
																	'#attributes' 	=>  array('placeholder' => 'Enter your Establishment Name'),
																	'#required'		=>	TRUE
																);
															
	$form['clra_backlog_master']['address_of_the_establishment']	= array (
																		'#title' 		=>  'Address Line1 of the Establishment',
																		'#type'  		=>  'textarea',
																		'#attributes' 	=>  array('placeholder' => 'Enter your Establishment Address'),
																		'#required'		=>	TRUE
																	);
	
	/*************************************** start */
	
	$options_dist 												= common_custom_user_district_list_new();
		
	$form['clra_backlog_master']['district_code_estb']   	=  array(
																	'#title'		=> 'Select District',
																	'#type' 		=> 'select',
																	'#options' 		=> $options_dist,
																	'#required' 		=> TRUE,
																	'#ajax'			=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_ajax_callback_establishment',
																						'progress'			=> '',
																				)
																);
								
	
	$value_estb_dist 										= isset($form_state['values']['district_code_estb']) ? $form_state['values']['district_code_estb'] : '';
		
			
	if($value_estb_dist != "" || !empty($value_estb_dist) || is_numeric($value_estb_dist)){				
			
		$form['clra_backlog_master']['subdivision_id']  	= array(
																	'#prefix'			=> '<div id="estb_subdvsn_replace"><section class="col col-6"><label class="select">',
																	'#suffix'			=> '<i></i></label></section></div>',
																	'#title'			=> 'Select Subdivision',
																	'#type' 			=> 'select',
																	'#options' 			=> common_custom_user_subdivisionlist_list_new($value_estb_dist),
																	'#required' 			=> TRUE,
																	'#ajax'				=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_ajax_callback_establishment',
																						'progress'			=> '',
																					),
																);																								
	}else{
				
		$form['clra_backlog_master']['subdivision_id'] 		= array(
																	'#prefix'			=> '<div id="estb_subdvsn_replace"><section class="col col-6"><label class="select">',
																	'#suffix'			=> '<i></i></label></section></div>',
																	'#title'			=> 'Select Subdivision',
																	'#type' 			=> 'select',
																	'#options' 			=> array(),
																	'#required' 			=> TRUE,
																	'#ajax'				=> array(
																						'event' 		=> 'change',
																						'effect' 		=> 'fade',			
																						'callback'		=> 'get_ajax_callback_establishment',
																						'progress'		=> '',
																					  ),
																);
	}
	
	
	$value_estb_subdivision 								= isset($form_state['values']['subdivision_id']) ? $form_state['values']['subdivision_id'] : '';
			
		if($value_estb_dist != "" && $value_estb_subdivision != ""){
			$areatypeOptions								= common_custom_user_areatype_list_new($value_estb_dist, $value_estb_subdivision);
			$areaTypeData									= array();
			if(!empty($areatypeOptions)){
				foreach($areatypeOptions as $key=>$val){
					if($val == 'B'){
						$areaTypeData[$key]	= 'Block';
					}
					if($val == 'M'){
						$areaTypeData[$key]	= 'Municipality';
					}
					if($val == 'C'){
						$areaTypeData[$key]	= 'Corporation';
					}
					if($val == 'S'){
						$areaTypeData[$key]	= 'SEZ';
					}
					if($val == 'N'){
						$areaTypeData[$key]	= 'Notified Area';
					}
				}
			}
			$form['clra_backlog_master']['areatype_estb']  	= array(
																	'#prefix'		=> '<div id="estb_areatype_replace"><section class="col col-6"><label class="select">',
																	'#suffix'		=>'<i></i></label></section></div>',
																	 '#title'		=> 'Select Areatype',
																	 '#type' 		=> 'select',
																	 '#options' 		=> $areaTypeData,
																	 '#required' 		=> TRUE,
																	 '#id'			=> 'loc_e_areatype_id',
																	 '#ajax'		=> array(
																					'event' 		=> 'change',
																					'effect' 		=> 'fade',			
																					'callback'		=> 'get_ajax_callback_establishment',
																					'progress'		=> '',
																				  ),
																);																										
		}else{
				
			$form['clra_backlog_master']['areatype_estb'] 	= array(
																	'#prefix'		=> '<div id="estb_areatype_replace"><section class="col col-6"><label class="select">',
																	'#suffix'		=>'<i></i></label></section></div>',
																	'#title'		=> 'Select Areatype',
																	'#type' 		=> 'select',
																	'#options' 		=> array(),
																	'#required' 		=> TRUE,
																	'#ajax'			=> array(
																					'event' 		=> 'change',
																					'effect' 		=> 'fade',			
																					'callback'		=> 'get_ajax_callback_establishment',
																					'progress'		=> '',
																				),
																);
		}
		
		$value_estb_areatype 								= isset($form_state['values']['areatype_estb']) ? $form_state['values']['areatype_estb'] : '';
			
		$form['clra_backlog_master']['areatype_code_estb'] 	= array(
																	'#prefix'		=> '<div id="estb_areatype_code_replace"><section class="col col-6"><label class="select">',
																	'#suffix'		=>'<i></i></label></section></div>',
																	 '#title'		=> 'Select Block/Municipality/Corporation/SEZ/Notified Area',
																	 '#type' 		=> 'select',
																	 '#options' 		=> array(),
																	 '#required' 		=> TRUE,
																	 '#ajax'		=> array(
																					'event' 		=> 'change',
																					'effect' 		=> 'fade',			
																					'callback'		=> 'get_ajax_callback_establishment',
																					'progress'		=> '',
																				)
																);
			
		if($value_estb_dist != "" && $value_estb_subdivision != ""){
			if($value_estb_areatype != ""){
				if($value_estb_areatype == 'B'){
					$form['clra_backlog_master']['areatype_code_estb'] 	= array(
																				'#prefix'			=> '<div id="estb_areatype_code_replace"><section class="col col-6"><label class="select">',
																				'#suffix'			=> '<i></i></label></section></div>',
																				'#title'			=> 'Select Block',
																				'#type' 			=> 'select',
																				'#options' 			=> common_custom_user_block_list_new($value_estb_subdivision, $value_estb_areatype),
																				'#required' 			=> TRUE,
																				'#ajax'				=> array(
																									'event' 			=> 'change',
																									'effect' 			=> 'fade',			
																									'callback'			=> 'get_ajax_callback_establishment',
																									'progress'			=> '',
																								)
																			);
				}else if($value_estb_areatype == 'M'){
				
					$form['clra_backlog_master']['areatype_code_estb'] 	= array(
																				'#prefix'			=> '<div id="estb_areatype_code_replace"><section class="col col-6"><label class="select">',
																				'#suffix'			=> '<i></i></label></section></div>',
																				'#title'			=> 'Select Municipality' ,
																				'#type' 			=> 'select',
																				'#options' 			=> common_custom_user_block_list_new($value_estb_subdivision, $value_estb_areatype),
																				'#required' 			=> TRUE,
																				'#ajax'				=> array(
																									'event' 			=> 'change',
																									'effect' 			=> 'fade',			
																									'callback'			=> 'get_ajax_callback_establishment',
																									'progress'			=> '',
																								)
																			);
				}else if($value_estb_areatype == 'C'){
		
					$form['clra_backlog_master']['areatype_code_estb'] = array(
																				'#prefix'			=> '<div id="estb_areatype_code_replace"><section class="col col-6"><label class="select">',
																				'#suffix'			=> '<i></i></label></section></div>',
																				'#title'			=> 'Select Corporation',
																				'#type' 			=> 'select',
																				'#options' 			=> common_custom_user_block_list_new($value_estb_subdivision, $value_estb_areatype),
																				'#required' 			=> TRUE,
																				'#ajax'				=> array(
																									'event' 			=> 'change',
																									'effect' 			=> 'fade',			
																									'callback'			=> 'get_ajax_callback_establishment',
																									'progress'			=> '',
																								)
																			);
				}else if($value_estb_areatype == 'S'){
		
					$form['clra_backlog_master']['areatype_code_estb'] = array(
																				'#prefix'			=> '<div id="estb_areatype_code_replace"><section class="col col-6"><label class="select">',
																				'#suffix'			=> '<i></i></label></section></div>',
																				 '#title'			=> 'Select SEZ',
																				 '#type' 			=> 'select',
																				 '#options' 			=> common_custom_user_block_list_new($value_estb_subdivision, $value_estb_areatype),
																				 '#required' 			=> TRUE,
																				 '#ajax'			=> array(
																									'event' 			=> 'change',
																									'effect' 			=> 'fade',			
																									'callback'			=> 'get_ajax_callback_establishment',
																									'progress'			=> '',
																								 )
																			);
				}else if($value_estb_areatype == 'N'){
				
					$form['clra_backlog_master']['areatype_code_estb'] 	= array(
																				'#prefix'			=> '<div id="estb_areatype_code_replace"><section class="col col-6"><label class="select">',
																				'#suffix'			=> '<i></i></label></section></div>',
																				'#title'			=> 'Select Notified Area',
																				'#type' 			=> 'select',
																				'#options' 			=> common_custom_user_block_list_new($value_estb_subdivision, $value_estb_areatype),
																				'#required' 			=> TRUE,
																				'#id'				=> 'common_selection_1_id',
																				'#ajax'				=> array(
																									'event' 			=> 'change',
																									'effect' 			=> 'fade',			
																									'callback'			=> 'get_ajax_callback_establishment',
																									'progress'			=> '',
																								  )
																			);
				}
			}
		}
		
		
		$value_areatype_code_estb					    			= isset($form_state['values']['areatype_code_estb']) ? $form_state['values']['areatype_code_estb'] : '';
			
		$form['clra_backlog_master']['vill_ward_estb'] 				= array(
																			'#prefix'			=> '<div id="estb_vill_ward_replace"><section class="col col-4"><label class="select">',
																			'#suffix'			=> '<i></i></label></section></div>',
																			'#title'			=> 'Select Gram Panchayat/Ward/Sector' ,
																			'#type' 			=> 'select',
																			'#options' 			=> array(),
																			'#required' 			=> TRUE,
																			'#id'					=> 'common_selection_2_id',
																			'#ajax'				=> array(
																										   'event' 			=> 'change',
																										   'effect' 			=> 'fade',			
																										   'callback'			=> 'get_ajax_callback_establishment',
																										   'progress'			=> '',
																									 )
																		);
			
		if($value_estb_dist != "" && $value_estb_subdivision != ""){
			if($value_estb_areatype != ""){
				if($value_areatype_code_estb != ""){
					if($value_estb_areatype == 'B'){
						$form['clra_backlog_master']['vill_ward_estb'] 	= array(
																				'#prefix'		=> '<div id="estb_vill_ward_replace"><section class="col col-4"><label class="select">',
																				'#suffix'		=> '<i></i></label></section></div>',
																				'#title'		=> 'Select Gram Panchayat',
																				'#type' 		=> 'select',
																				'#options'		=> common_custom_user_villward_list_new($value_areatype_code_estb,'V'),
																				'#required' 		=> TRUE,
																				'#id'			=> 'common_selection_2_id',
																				'#ajax'			=> array(
																								'event' 	=> 'change',
																								'effect' 	=> 'fade',			
																								'callback'	=> 'get_ajax_callback_establishment',
																								'progress'	=> '',
																							)
																			);	
					}
						
					if($value_estb_areatype == 'M'){
							
							$form['clra_backlog_master']['vill_ward_estb'] = array(
																					'#prefix'	=> '<div id="estb_vill_ward_replace"><section class="col col-4"><label class="select">',
																					'#suffix'	=> '<i></i></label></section></div>',
																					'#title'	=> 'Select Ward',
																					'#type' 	=> 'select',
																					'#options' 	=> common_custom_user_villward_list_new($value_areatype_code_estb,'W'),
																					'#required' 	=> TRUE,
																					'#id'		=> 'common_selection_2_id',
																					'#ajax'		=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_ajax_callback_establishment',
																								'progress'			=> '',
																							)
																				);
																							
					}
						
					if($value_estb_areatype == 'C'){
							
							$form['clra_backlog_master']['vill_ward_estb'] = array(
																				'#prefix'	=> '<div id="estb_vill_ward_replace"><section class="col col-4"><label class="select">',
																				'#suffix'	=> '<i></i></label></section></div>',
																				'#title'	=> 'Select Ward',
																				'#type' 	=> 'select',
																				'#options' 	=> common_custom_user_corporation_list_new($value_areatype_code_estb),
																				'#required' 	=> TRUE,
																				'#id'		=> 'common_selection_2_id',
																				'#ajax'		=> array(
																							'event' 			=> 'change',
																							'effect' 			=> 'fade',			
																							'callback'			=> 'get_ajax_callback_establishment',
																							'progress'			=> '',
																						)
																			);
					}
					if($value_estb_areatype == 'S'){
							$form['clra_backlog_master']['vill_ward_estb'] 	= array(
																					'#prefix'	=> '<div id="estb_vill_ward_replace"><section class="col col-4"><label class="select">',
																					'#suffix'	=> '<i></i></label></section></div>',
																					'#title'	=> 'Select Ward',
																					'#type' 	=> 'select',
																					'#options' 	=> common_custom_user_villward_list_new($value_areatype_code_estb, 'S'),
																					'#required' 	=> TRUE,
																					'#id'		=> 'common_selection_2_id',
																					'#ajax'		=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_ajax_callback_establishment',
																								'progress'			=> '',
																							)
																				);
					}
				}
			}
		}
			
			
			
		$value_vill_ward_estb 												= isset($form_state['values']['vill_ward_estb']) ? $form_state['values']['vill_ward_estb'] : '';
		
		$form['clra_backlog_master']['ps_estb'] 							= array(
																					'#prefix'		=> '<div id="estb_ps_replace"><section class="col col-4"><label class="select">',
																					'#suffix'		=> '<i></i></label></section></div>',
																					'#title'		=> 'Select Police Station',
																					'#type' 		=> 'select',
																					'#options' 		=> array(),
																					'#required' 		=> TRUE,
																					'#id'			=> 'estb_ps_id',
																					'#ajax'			=> array(
																									'event' 			=> 'change',
																									'effect' 			=> 'fade',			
																									'callback'			=> 'get_ajax_callback_establishment',
																									'progress'			=> '',
																								)
																				);
		
		if($value_estb_dist != "" && $value_estb_subdivision != ""){
			if($value_estb_areatype != ""){
				if($value_areatype_code_estb != ""){
					if($value_vill_ward_estb != ""){
						$form['clra_backlog_master']['ps_estb'] 			= array(
																				  '#prefix'			=> '<div id="estb_ps_replace"><section class="col col-4"><label class="select">',
																				  '#suffix'			=> '<i></i></label></section></div>',
																				  '#title'			=> 'Select Police Station',
																				  '#type' 			=> 'select',
																				  '#id'				=> 'loc_e_ps_id',
																				  '#options' 			=> common_custom_user_ps_list_new($value_estb_dist),
																				  '#required' 			=> TRUE
																			);
					}
				}
			}
		}
															
	$form['clra_backlog_master']['name_of_the_principle_emp'] 	= array (
																		'#title' 		=>  'Name of the Principal Employer',
																		'#type'  		=>  'textfield',
																		'#attributes' 	=>  array('placeholder' => 'Enter your Principle Employer Name'),
																		'#required'		=>	TRUE
																	);
															
	$form['clra_backlog_master']['address_of_the_principle_emp'] = array (
																		'#title' 		=>  'Address Line 1 of the Principal Employer',
																		'#type'  		=>  'textarea',
																		'#attributes' 	=>  array('placeholder' => 'Enter your Principle Employer Address'),
																		'#required'		=>	TRUE
																	);
									
		
	$form['clra_backlog_master']['dist_code']   				=  array(
																		'#title'			=> 'Select District',
																		'#type' 			=> 'select',
																		'#options' 			=> $options_dist,
																		'#required' 		=> TRUE,
																		'#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_ajax_callback_new_1',
																								'progress'			=> '',
																							 )
																	);
					
	$value_loc_e_dist = isset($form_state['values']['dist_code']) ? $form_state['values']['dist_code'] : '';
		
			
	if($value_loc_e_dist != "" || !empty($value_loc_e_dist) || is_numeric($value_loc_e_dist)){				
			
		$form['clra_backlog_master']['subdivision']  			= array(
																		'#prefix'			=> '<div id="clra_subdvsn_replace"><section class="col col-6"><label class="select">',
																		'#suffix'			=> '<i></i></label></section></div>',
																		'#title'			=> 'Select Subdivision',
																		'#type' 			=> 'select',
																		'#options' 			=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist),
																		'#required' 			=> TRUE,
																		'#ajax'				=> array(
																									  'event' 			=> 'change',
																									  'effect' 			=> 'fade',			
																									  'callback'			=> 'get_ajax_callback_new_1',
																									  'progress'			=> '',
																						),
																	);																								
	}else{
				
		$form['clra_backlog_master']['subdivision'] 			= array(
																		'#prefix'			=> '<div id="clra_subdvsn_replace"><section class="col col-6"><label class="select">',
																		'#suffix'			=> '<i></i></label></section></div>',
																		'#title'			=> 'Select Subdivision',
																		'#type' 			=> 'select',
																		'#options' 			=> array(),
																		'#required' 			=> TRUE,
																		'#ajax'				=> array(
																							'event' 		=> 'change',
																							'effect' 		=> 'fade',			
																							'callback'		=> 'get_ajax_callback_new_1',
																							'progress'		=> '',
																						  ),
																	);
	}
	
	$value_loc_e_subdivision 									= isset($form_state['values']['subdivision']) ? $form_state['values']['subdivision'] : '';
			
		if($value_loc_e_dist != "" && $value_loc_e_subdivision != ""){
			$areatypeOptions				= common_custom_user_areatype_list_new($value_loc_e_dist, $value_loc_e_subdivision);
			$areaTypePEData					= array();
			if(!empty($areatypeOptions)){
				foreach($areatypeOptions as $key=>$val){
					if($val == 'B'){
						$areaTypePEData[$key]	= 'Block';
					}
					if($val == 'M'){
						$areaTypePEData[$key]	= 'Municipality';
					}
					if($val == 'C'){
						$areaTypePEData[$key]	= 'Corporation';
					}
					if($val == 'S'){
						$areaTypePEData[$key]	= 'SEZ';
					}
					if($val == 'N'){
						$areaTypePEData[$key]	= 'Notified Area';
					}
				}
			}
			$form['clra_backlog_master']['areatype']  			= array(
																		'#prefix'		=> '<div id="clra_loc_e_areatype"><section class="col col-6"><label class="select">',
																		'#suffix'		=>'<i></i></label></section></div>',
																		 '#title'		=> 'Select Areatype',
																		 '#type' 		=> 'select',
																		 '#options' 		=> $areaTypePEData,
																		 '#required' 		=> TRUE,
																		 //'#id'			=> 'loc_e_areatype_id',
																		 '#ajax'		=> array(
																						'event' 		=> 'change',
																						'effect' 		=> 'fade',			
																						'callback'		=> 'get_ajax_callback_new_1',
																						'progress'		=> '',
																					  ),
																	);																										
		}else{
				
			$form['clra_backlog_master']['areatype'] 			= array(
																		'#prefix'		=> '<div id="clra_loc_e_areatype"><section class="col col-6"><label class="select">',
																		'#suffix'		=>'<i></i></label></section></div>',
																		'#title'		=> 'Select Areatype',
																		'#type' 		=> 'select',
																		'#options' 		=> array(),
																		'#required' 		=> TRUE,
																		'#ajax'			=> array(
																						'event' 		=> 'change',
																						'effect' 		=> 'fade',			
																						'callback'		=> 'get_ajax_callback_new_1',
																						'progress'		=> '',
																					),
																	);
		}
		$value_loc_e_areatype 									= isset($form_state['values']['areatype']) ? $form_state['values']['areatype'] : '';
			
		$form['clra_backlog_master']['name_areatype'] 		= array(
																	'#prefix'		=> '<div id="clra_common_1_replace"><section class="col col-6"><label class="select">',
																	'#suffix'		=>'<i></i></label></section></div>',
																	 '#title'		=> 'Select Block/Municipality/Corporation/SEZ/Notified Area',
																	 '#type' 		=> 'select',
																	 '#options' 		=> array(),
																	 '#required' 		=> TRUE,
																	 '#ajax'		=> array(
																					'event' 		=> 'change',
																					'effect' 		=> 'fade',			
																					'callback'		=> 'get_ajax_callback_new_1',
																					'progress'		=> '',
																				)
																);
			
			if($value_loc_e_dist != "" && $value_loc_e_subdivision != ""){
				if($value_loc_e_areatype != ""){
					if($value_loc_e_areatype == 'B'){
						$form['clra_backlog_master']['name_areatype'] 	= array(
																				'#prefix'			=> '<div id="clra_common_1_replace"><section class="col col-6"><label class="select">',
																				'#suffix'			=> '<i></i></label></section></div>',
																				'#title'			=> 'Select Block',
																				'#type' 			=> 'select',
																				'#options' 			=> common_custom_user_block_list_new($value_loc_e_subdivision, $value_loc_e_areatype),
																				'#required' 			=> TRUE,
																				//'#id'				=> 'common_selection_1_id',
																				'#ajax'				=> array(
																									'event' 			=> 'change',
																									'effect' 			=> 'fade',			
																									'callback'			=> 'get_ajax_callback_new_1',
																									'progress'			=> '',
																								)
																			);
					}else if($value_loc_e_areatype == 'M'){
					
						$form['clra_backlog_master']['name_areatype'] 	= array(
																				'#prefix'			=> '<div id="clra_common_1_replace"><section class="col col-6"><label class="select">',
																				'#suffix'			=> '<i></i></label></section></div>',
																				'#title'			=> 'Select Municipality',
																				'#type' 			=> 'select',
																				'#options' 			=> common_custom_user_block_list_new($value_loc_e_subdivision, $value_loc_e_areatype),
																				'#required' 			=> TRUE,
																				//'#id'				=> 'common_selection_1_id',
																				'#ajax'				=> array(
																									'event' 			=> 'change',
																									'effect' 			=> 'fade',			
																									'callback'			=> 'get_ajax_callback_new_1',
																									'progress'			=> '',
																								)
																			);
					}else if($value_loc_e_areatype == 'C'){
			
						$form['clra_backlog_master']['name_areatype'] = array(
																			'#prefix'			=> '<div id="clra_common_1_replace"><section class="col col-6"><label class="select">',
																			'#suffix'			=> '<i></i></label></section></div>',
																			'#title'			=> 'Select Corporation',
																			'#type' 			=> 'select',
																			'#options' 			=> common_custom_user_block_list_new($value_loc_e_subdivision,$value_loc_e_areatype),
																			'#required' 			=> TRUE,
																			//'#id'				=> 'common_selection_1_id',
																			'#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_ajax_callback_new_1',
																								'progress'			=> '',
																							)
																		);
					}else if($value_loc_e_areatype == 'S'){
			
						$form['clra_backlog_master']['name_areatype'] = array(
																			'#prefix'			=> '<div id="clra_common_1_replace"><section class="col col-6"><label class="select">',
																			'#suffix'			=> '<i></i></label></section></div>',
																			'#title'			=> 'Select SEZ',
																			'#type' 			=> 'select',
																			'#options' 			=> common_custom_user_block_list_new($value_loc_e_subdivision, $value_loc_e_areatype),
																			'#required' 			=> TRUE,
																			//'#id'				=> 'common_selection_1_id',
																			'#ajax'			=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_ajax_callback_new_1',
																								'progress'			=> '',
																							 )
																		);
					}else if($value_loc_e_areatype == 'N'){
					
						$form['clra_backlog_master']['name_areatype'] 	= array(
																				'#prefix'			=> '<div id="clra_common_1_replace"><section class="col col-6"><label class="select">',
																				'#suffix'			=> '<i></i></label></section></div>',
																				'#title'			=> 'Select Notified Area',
																				'#type' 			=> 'select',
																				'#options' 			=> common_custom_user_block_list_new($value_loc_e_subdivision, $value_loc_e_areatype),
																				'#required' 			=> TRUE,
																				//'#id'				=> 'common_selection_1_id',
																				'#ajax'				=> array(
																									'event' 			=> 'change',
																									'effect' 			=> 'fade',			
																									'callback'			=> 'get_ajax_callback_new_1',
																									'progress'			=> '',
																								  )
																			);
					}
				}
			}
			$value_common_selection_1 					    	= isset($form_state['values']['name_areatype']) ? $form_state['values']['name_areatype'] : '';
			
			$form['clra_backlog_master']['vill_ward'] 				= array(
																			'#prefix'			=> '<div id="clra_common_2_replace"><section class="col col-4"><label class="select">',
																			'#suffix'			=> '<i></i></label></section></div>',
																			'#title'			=> 'Select Gram Panchayat/Ward/Sector',
																			'#type' 			=> 'select',
																			'#options' 			=> array(),
																			'#required' 			=> TRUE,
																			//'#id'				=> 'common_selection_2_id',
																			'#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_ajax_callback_new_1',
																								'progress'			=> '',
																							)
																		);
			
			if($value_loc_e_dist != "" && $value_loc_e_subdivision != ""){
				if($value_loc_e_areatype != ""){
					if($value_common_selection_1 != ""){
						if($value_loc_e_areatype == 'B'){
								
							$form['clra_backlog_master']['vill_ward'] 	= array(
																			  '#prefix'			=> '<div id="clra_common_2_replace"><section class="col col-4"><label class="select">',
																			  '#suffix'			=> '<i></i></label></section></div>',
																			  '#title'			=> 'Select Gram Panchayat',
																			  '#type' 			=> 'select',
																			  '#options'			=> common_custom_user_villward_list_new($value_common_selection_1,'V'),
																			  '#required' 			=> TRUE,
																			  //'#id'				=> 'common_selection_2_id',
																			  '#ajax'			=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_ajax_callback_new_1',
																								'progress'			=> '',
																							)
																			);	
						}
							
						if($value_loc_e_areatype == 'M'){
								
								$form['clra_backlog_master']['vill_ward'] = array(
																				  '#prefix'				=> '<div id="clra_common_2_replace"><section class="col col-4"><label class="select">',
																				  '#suffix'				=> '<i></i></label></section></div>',
																				  '#title'				=> 'Select Ward',
																				  '#type' 				=> 'select',
																				  '#options' 			=> common_custom_user_villward_list_new($value_common_selection_1,'W'),
																				  '#required' 			=> TRUE,
																				  //'#id'					=> 'common_selection_2_id',
																				  '#ajax'				=> array(
																												'event' 			=> 'change',
																												'effect' 			=> 'fade',			
																												'callback'			=> 'get_ajax_callback_new_1',
																												'progress'			=> '',
																											  )
																				);
																								
						}
							
						if($value_loc_e_areatype == 'C'){
								
								$form['clra_backlog_master']['vill_ward'] = array(
																			  '#prefix'				=> '<div id="clra_common_2_replace"><section class="col col-4"><label class="select">',
																			  '#suffix'				=> '<i></i></label></section></div>',
																			  '#title'				=> 'Select Ward',
																			  '#type' 				=> 'select',
																			  '#options' 			=> common_custom_user_corporation_list_new($value_common_selection_1),
																			  '#required' 			=> TRUE,
																			  //'#id'					=> 'common_selection_2_id',
																			  '#ajax'				=> array(
																										'event' 			=> 'change',
																										'effect' 			=> 'fade',			
																										'callback'			=> 'get_ajax_callback_new_1',
																										'progress'			=> '',
																									  )
																			);
						}
						
						if($value_loc_e_areatype == 'S'){
								
								$form['clra_backlog_master']['vill_ward'] = array(
																			  '#prefix'				=> '<div id="clra_common_2_replace"><section class="col col-4"><label class="select">',
																			  '#suffix'				=> '<i></i></label></section></div>',
																			  '#title'				=> 'Select Sector',
																			  '#type' 				=> 'select',
																			  '#options' 			=> common_custom_user_villward_list_new($value_common_selection_1,'S'),
																			  '#required' 			=> TRUE,
																			  //'#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
																			  //'#id'					=> 'common_selection_2_id',
																			  '#ajax'				=> array(
																											'event' 			=> 'change',
																											'effect' 			=> 'fade',			
																											'callback'			=> 'get_ajax_callback_new_1',
																											'progress'			=> '',
																										  )
																			);
						}
					}
				}
			}
		$value_common_selection_2 										= isset($form_state['values']['vill_ward']) ? $form_state['values']['vill_ward'] : '';
		
		$form['clra_backlog_master']['ps'] 								= array(
																				'#prefix'		=> '<div id="clra_ps_replace"><section class="col col-4"><label class="select">',
																				'#suffix'		=> '<i></i></label></section></div>',
																				'#title'		=> 'Select Police Station',
																				'#type' 		=> 'select',
																				'#options' 		=> array(),
																				'#required' 		=> TRUE,
																				'#id'			=> 'estb_ps_id',
																				'#ajax'			=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_ajax_callback_establishment',
																								'progress'			=> '',
																							)
																			);
		
		if($value_loc_e_dist != "" && $value_loc_e_subdivision != ""){
			if($value_loc_e_areatype != ""){
				if($value_common_selection_1 != ""){
					if($value_common_selection_2 != ""){
						$form['clra_backlog_master']['ps'] 				= array(
																				  '#prefix'			=> '<div id="clra_ps_replace"><section class="col col-4"><label class="select">',
																				  '#suffix'			=> '<i></i></label></section></div>',
																				  '#title'			=> 'Select Police Station',
																				  '#type' 			=> 'select',
																				  //'#id'				=> 'loc_e_ps_id',
																				  '#options' 			=> common_custom_user_ps_list_new($value_loc_e_dist),
																				  '#required' 			=> TRUE
																			);
					}
				}
			}
		}
	$form['clra_backlog_master']['emp_pin_code'] 						= array (
																				'#title' 		=>  'Pin Code',
																				'#type'  		=>  'textfield',
																				'#attributes' 	=>  array('placeholder' => 'Enter Pin Code Number'),
																				'#required'		=>	TRUE
																			);
	/******/													
	$form['clra_backlog_master']['maximum_no_of_contact_labour'] 		= array (
																				'#title' 		=>  'Maximum Number of Contract Labours',
																				'#type'  		=>  'textfield',
																				'#attributes' 	=>  array('placeholder' => 'Enter Maximum Number of Contract Labours'),
																				'#required'		=>	TRUE
																			);
	$form['clra_backlog_master']['fees'] 								= array (
																				'#title' 		=>  'Fees',
																				'#type'  		=>  'textfield',
																				'#attributes' 	=>  array('placeholder' => 'Enter Fees'),
																				'#required'		=>	TRUE
																			);
															
	$form['clra_backlog_master']['submit'] 								= array (
																				'#type'  		=>  'submit',
																				'#value' 		=>	'Save'
																			);
	
	return $form;
}
function add_clra_backlog_applicant_validate($form, &$form_state){
		$val 											= 	$form_state['values'];
		$registration_no								=	$val['registration_no'];
		$registration_date								=	$val['registration_date'];
		$name_of_the_establishment						=	$val['name_of_the_establishment'];
		
		$addressline1_of_the_establishment				=	$val['address_of_the_establishment'];
		$district_code_establishment					=	$val['district_code_estb'];
		$subdivision_id_establishment					=	$val['subdivision_id'];
		$block_mun_id_establishment						=	$val['areatype_code_estb'];
		$villward_code_establishment					=	$val['vill_ward_estb'];
		$ps_code_establishment							=	$val['ps_estb'];
		
		
		
		
		$name_of_the_principle_emp						=	$val['name_of_the_principle_emp'];
		$address_of_the_principle_emp					=	$val['address_of_the_principle_emp'];
		$district_code_pe								=	$val['dist_code'];
		$subdivision_id_pe								=	$val['subdivision'];
	
		$block_mun_id_pe								=	$val['name_areatype'];
		
		$maximum_no_of_contact_labour					=	$val['maximum_no_of_contact_labour'];
		$fees									=	$val['fees'];
		
		if($registration_no =="" ){
			form_error($form['clra_backlog_master']['registration_no'], t('Registration Number cannot be blank.'));
		}
		if($registration_date == ""){
			form_error($form['clra_backlog_master']['registration_date'],t('Registration Date cannot be blank'));
		}
		if($name_of_the_establishment =="" ){
			form_error($form['clra_backlog_master']['name_of_the_establishment'], t('Name of the Establishment cannot be blank.'));
		}
		if($addressline1_of_the_establishment =="" ){
			form_error($form['clra_backlog_master']['address_of_the_establishment'], t('Address line1 of the Establishment cannot be blank.'));
		}
		if($district_code_establishment =="" ){
			form_error($form['clra_backlog_master']['district_code_estb'], t('District of the Establishment cannot be blank.'));
		}
		if($subdivision_id_establishment =="" ){
			form_error($form['clra_backlog_master']['subdivision_id'], t('Sub Division of the Establishment cannot be blank.'));
		}
		if($block_mun_id_establishment =="" ){
			form_error($form['clra_backlog_master']['areatype_code_estb'], t('Block of the Establishment cannot be blank.'));
		}
		if($villward_code_establishment =="" ){
			form_error($form['clra_backlog_master']['vill_ward_estb'], t('Village/Ward of the Establishment cannot be blank.'));
		}
		if($ps_code_establishment =="" ){
			form_error($form['clra_backlog_master']['ps_estb'], t('Polica Station of the Establishment cannot be blank.'));
		}
		
		
		if($name_of_the_principle_emp =="" ){
			form_error($form['clra_backlog_master']['name_of_the_principle_emp'], t('Name of the Principle Employer cannot be blank.'));
		}
		if($address_of_the_principle_emp =="" ){
			form_error($form['clra_backlog_master']['address_of_the_principle_emp'], t('Address Line 1 of the Principle Employer cannot be blank.'));
		}
		
		if($district_code_pe =="" ){
			form_error($form['clra_backlog_master']['dist_code'], t('District of the Principle Employer cannot be blank.'));
		}
		if($subdivision_id_pe =="" ){
			form_error($form['clra_backlog_master']['subdivision'], t('Sub Division of the Principle Employer cannot be blank.'));
		}
		
		if($block_mun_id_pe =="" ){
			form_error($form['clra_backlog_master']['name_areatype'], t('Block of the Principle Employer cannot be blank.'));
		}
			
		if(!is_numeric($maximum_no_of_contact_labour)){
			form_error($form['clra_backlog_master']['maximum_no_of_contact_labour'], t('Number of labour must be Numeric.'));
		}
		if(!is_numeric($fees)){
			form_error($form['clra_backlog_master']['fees'], t('Fees cannot be blank.'));
		}
		
		
}

function add_clra_backlog_applicant_submit($form, &$form_state){
	global $user;
	$val 												= 	$form_state['values'];
	$registration_no									=	$val['registration_no'];
	$registration_date									=	$val['registration_date'];
	$name_of_the_establishment							=	$val['name_of_the_establishment'];
	
	$addressline1_of_the_establishment					=	$val['address_of_the_establishment'];
	$district_code_establishment						=	$val['district_code_estb'];
	$subdivision_id_establishment						=	$val['subdivision_id'];
	$estab_areatype_name								=	$val['areatype_estb'];
	$block_mun_id_establishment							=	$val['areatype_code_estb'];
	$villward_code_establishment						=	$val['vill_ward_estb'];
	$ps_code_establishment								=	$val['ps_estb'];
	//$pincode_establishment							=	$val['pin_code_estb'];
	
	$name_of_the_principle_emp							=	$val['name_of_the_principle_emp'];
	$address_of_the_principle_emp						=	$val['address_of_the_principle_emp'];
	$district_code_pe									=	$val['dist_code'];
	$subdivision_id_pe									=	$val['subdivision'];
	$emp_areatype										=	$val['areatype'];
	$block_mun_id_pe									=	$val['name_areatype'];
	$village_code_pe									=	$val['vill_ward'];
	$police_station_code_pe								=	$val['ps'];
	$pincode_emp										=	$val['emp_pin_code'];
	
	$maximum_no_of_contact_labour						=	$val['maximum_no_of_contact_labour'];
	$fees												=	$val['fees'];
	
	
	if($registration_no != "" ){
	$fielddList						=  array(
											'user_id'						=> $user->uid,
											'registration_no'				=> $registration_no,
											'registration_date'				=> $registration_date,					
											'name_of_the_establishment'		=> $name_of_the_establishment,
											'address_of_the_establishment'	=> $addressline1_of_the_establishment,
											'district_code_establishment'	=> $district_code_establishment,
											'subdivision_id_establishment'	=> $subdivision_id_establishment,
											'block_mun_id_establishment'	=> $block_mun_id_establishment,
											'villward_code_establishment'	=> $villward_code_establishment,
											'ps_code_establishment'			=> $ps_code_establishment,
											
											'name_of_the_principle_emp'		=> $name_of_the_principle_emp,
											'address_of_the_principle_emp'	=> $address_of_the_principle_emp,
											'district_code_pe'				=> $district_code_pe,
											'subdivision_id_pe'				=> $subdivision_id_pe,
											'block_mun_id_pe'				=> $block_mun_id_pe,
											'village_code_pe'				=> $village_code_pe,
											'police_station_code_pe'		=> $police_station_code_pe,
											
											'maximum_no_of_contact_labour'	=> $maximum_no_of_contact_labour,
											
											'fees'							=> $fees,
											'form_fillup_by'				=> "Applicant"
									);
	$backlog_id 					= db_insert('l_clra_principle_emp_backlog_data')->fields($fielddList)->execute();
	$fieldsBackLogRegInfo 			= 	array(
											'user_id'					=> $user->uid,
											'identification_number'		=> 'CLRA'.time(),
											'e_name'					=> $name_of_the_establishment,
											
											'loc_e_name'				=> $addressline1_of_the_establishment,
											'loc_e_dist'				=> $district_code_establishment,
											'loc_e_subdivision'			=> $subdivision_id_establishment,
											'name_areatype'				=> $block_mun_id_establishment,
											'loc_e_areatype'			=> $estab_areatype_name,
											'loc_e_vill_ward'			=> $villward_code_establishment,
											'l_e_ps'					=> $ps_code_establishment,
											//'loc_e_pin_number'		=> $pincode_establishment,
											
											
											'e_postal_address'			=> $addressline1_of_the_establishment,
											'e_postal_dist'				=> $district_code_establishment,
											'e_postal_subdivision'		=> $subdivision_id_establishment,
											'e_postal_name_areatype'	=> $block_mun_id_establishment,
											'e_postal_areatype'			=> $estab_areatype_name,
											'e_postal_vill_ward'		=> $villward_code_establishment,
											'e_postal_ps'				=> $ps_code_establishment,
											//'e_postal_pin_number'		=> $pincode_establishment,
											
											
											'full_name_principal_emp'	=> $name_of_the_principle_emp,
											'address_principal_emp'		=> $address_of_the_principle_emp,
											'loc_emp_dist'				=> $district_code_pe,
											'loc_emp_subdivision'		=> $subdivision_id_pe,
											'loc_emp_areatype'			=> $emp_areatype,
											'emp_name_areatype'			=> $block_mun_id_pe,
											'loc_emp_vill_ward'			=> $village_code_pe,
											'l_emp_ps'					=> $pincode_emp,
											'loc_emp_pin_number'		=> $pincode_emp,
											
											'full_name_manager'			=> $name_of_the_principle_emp,
											'address_manager'			=> $address_of_the_principle_emp,
											'loc_manager_dist'			=> $district_code_pe,
											'loc_manager_subdivision'	=> $subdivision_id_pe,
											'loc_manager_areatype'		=> $emp_areatype,
											'manager_name_areatype'		=> $block_mun_id_pe,
											'loc_manager_vill_ward'		=> $village_code_pe,
											'l_manager_ps'				=> $pincode_emp,
											'loc_manager_pin_number'	=> $pincode_emp,
											
											'con_lab_job_desc'			=> 'NIL',
											'con_lab_wage_rate_other_benefits'=> 'NIL',
											'con_lab_cat_desig_nom'		=> 'NIL',
											'e_settlement_award_judgement_min_wage'=> 'NIL',
											
											'backlog_id'				=> $backlog_id,
											// 'finalfees'				=> $finalfees,
											'act_id'					=> 1,
											// 'status'					=> '0',
											'e_any_day_max_num_of_workmen'	=> $maximum_no_of_contact_labour
										);
	$last_id_master						=	db_insert('l_clra_registration_master')->fields($fieldsBackLogRegInfo)->execute();
	/*
	$fieldsBackLogUpdateInfo 			= 	array(
											'status'		=> '2',
											'date_time'		=> time()
										);
						
	$query = db_update('l_clra_principle_emp_backlog_data');
        $query->fields($fieldsBackLogUpdateInfo); 
	$query->condition('id', $backlog_id);
        $query->execute();
	*/
		$last_id_master	= encryption_decryption_fun('encrypt', $last_id_master);
		drupal_goto('view-clra-application-details/view-clra-application/'.$last_id_master);
	}
}
function get_ajax_callback_new_1($form, $form_state) {
	$commands 	= array();
	$commands[] = ajax_command_replace('#clra_subdvsn_replace', drupal_render($form['clra_backlog_master']['subdivision']));
	$commands[] = ajax_command_replace('#clra_loc_e_areatype', drupal_render($form['clra_backlog_master']['areatype']));
	$commands[] = ajax_command_replace('#clra_common_1_replace', drupal_render($form['clra_backlog_master']['name_areatype']));
	$commands[] = ajax_command_replace('#clra_common_2_replace', drupal_render($form['clra_backlog_master']['vill_ward']));
	$commands[] = ajax_command_replace('#clra_ps_replace', drupal_render($form['clra_backlog_master']['ps']));
	return array('#type' => 'ajax', '#commands' => $commands);
}
function get_ajax_callback_establishment($form, $form_state) {
	$commands 	= array();
	$commands[] = ajax_command_replace('#estb_subdvsn_replace', drupal_render($form['clra_backlog_master']['subdivision_id']));
	$commands[] = ajax_command_replace('#estb_areatype_replace', drupal_render($form['clra_backlog_master']['areatype_estb']));
	$commands[] = ajax_command_replace('#estb_areatype_code_replace', drupal_render($form['clra_backlog_master']['areatype_code_estb']));
	$commands[] = ajax_command_replace('#estb_vill_ward_replace', drupal_render($form['clra_backlog_master']['vill_ward_estb']));
	$commands[] = ajax_command_replace('#estb_ps_replace', drupal_render($form['clra_backlog_master']['ps_estb']));
	return array('#type' => 'ajax', '#commands' => $commands);
}	