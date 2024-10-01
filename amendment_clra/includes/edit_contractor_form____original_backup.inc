<?php

	function edit_contractor_form($form, &$form_state, $param1, $param2) {
		
		global $base_root, $base_path, $user; 
		
		$user_id 			= $user->uid;
		$action_encrypt		=   'encrypt';
			
		drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
		drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicant_table_info.css');
		drupal_add_js(drupal_get_path('module', 'applicant_forms') . '/js/custom.js');	
		
		$xx 				 = 	base64_decode($param1); 
		$xxx 				 = 	base64_decode($xx);	
		$contractor_id 	 	 = 	$xxx;
		
		$aa 				 = 	base64_decode($param2); 
		$aaa 				 = 	base64_decode($aa);	
		$application_id 	 = 	$aaa;
		
		
		
		drupal_add_library('system','ui.datepicker');
		
		
		$result 			 = 	db_select('l_contractor_info_master', 'lcim')
									->fields('lcim')
									->condition('lcim.id', trim($contractor_id))
									->execute();
		$data 				 = 	$result->fetchObject();
		
		
		$contractor_type_data 						=   $data->contractor_type;
		$state_options								=   $data->state_opts;
		$state_others								=   $data->state_others;
		$con_loc_e_dist								=	$data->con_loc_e_dist;
		$con_loc_e_subdivision						=	$data->con_loc_e_subdivision;
		$con_loc_e_areatype							=	$data->con_loc_e_areatype;
		$con_name_areatype							=	$data->con_name_areatype;
		$con_loc_e_vill_ward						=	$data->con_loc_e_vill_ward;
		$con_l_e_ps									=	$data->con_l_e_ps;
		$contractor_pin								=	$data->contractor_pin;
		
		
		
		$query_get_each_nature_work  = 	db_query("select a.contractor_nature_of_work,b.contractor_work_id, b.cont_work_name from l_clra_con_nature_of_work a left join contractor_works as b on a.contractor_nature_of_work = b.contractor_work_id where a.application_id =:application_id and a.user_id=:user_id and b.is_default =:is_default and a.contractor_id =:contractor_id order by a.id desc", array( ':user_id' => $user_id , ':application_id' => $application_id , ':is_default' => '1' ,':contractor_id' =>  $data->id )); 
	
			
		$resultChunk	=	 array();		
		if(!empty($query_get_each_nature_work)){
			foreach($query_get_each_nature_work as $listData){ 
					$resultChunk[] = $listData->contractor_work_id;
			}
		}
		
		/*$query 				 		= db_select('l_clra_con_nature_of_work', 'lccnow');
		$get_other_nature_work		->leftJoin('contractor_works', 'cw', 'lccnow.contractor_nature_of_work=cw.contractor_work_id');
		$query->fields('lccnow', array('contractor_nature_of_work'));
		$query->condition('lccnow.contractor_id', $data->id);
		$result 			 = $query->execute();
			
		$result_Id	=	array();
		while( $datum = $result->fetchObject() ){
			$result_Id[] = $datum->contractor_nature_of_work;
		}
		*/
		
		$query_stat 				= db_select('l_clra_registration_master', 'lcrm');
		$query_stat->fields('lcrm', array('final_submit_status', 'status'));
		$query_stat->condition('lcrm.user_id', $user->uid);
		$query_stat->condition('lcrm.id', $application_id);
		$result_stat 			 	= $query_stat->execute();
		$result_data_stat			= $result_stat->fetchObject();
		
		$final_submit_status		= $result_data_stat->final_submit_status;
		$status						= $result_data_stat->status;
		
		
		
		/*** Check Contractor License Issued with this Form V ***/
		
		
		
		$query_chk_license 			= db_select('l_contractor_license_application', 'lcla');
		$query_chk_license			->fields('lcla', array('status'));
		$query_chk_license			->condition('lcla.serial_no_from_v', $data->id);
		$query_chk_license			->condition('lcla.status', 'I');
		$license_chk_status 		= $query_chk_license->execute()->fetchAssoc();
		
		$readonly_license_issued	= array('style'=>"height:150px;", 'class'=> array('selected'));
		
		if(!empty($license_chk_status) && $license_chk_status['status'] == 'I'){
			
			$readonly_license_issued = array('readonly' => 'readonly','style' =>'height: 150px;', 'class' => array('readonly_view selected'));
			
		}
		
		
		
		
		
		
		
		$form['contractor_master_edit']['header']											= array(
																								  '#type' 				=> 	'markup',
																								  '#markup' 				=> 	t('<div class="content"><div class="sky-form" style="width:100%;"><header><h3><b>Particulars of Contractors and Contract Labour</b></h3></header>'),
																								 );
							
		$form['contractor_master_edit']['hidden_id'] 										= array(
																								  '#type' 				=> 'hidden',
																								  '#default_value'		=> $data->id
																							  );
							
		$form['contractor_master_edit']['act_id'] 											= array(
																								  '#type' 				=> 'hidden',
																								  '#default_value'		=> $data->act_id
																								);
							
		$form['contractor_master_edit']['application_id'] 									= array(
																								  '#type' 				=> 'hidden',
																								  '#default_value'		=> $data->application_id
																								);
							
		$form['contractor_master_edit']['identification_number'] 							= array(
																								  '#type' 				=> 'hidden',
																								  '#default_value'		=> $data->identification_number,
																							 );	
																							 
																							 
																							 
		$contractor_type	 =	array( 1 => 'Offline generated License', 2 => 'New Contractor');

		$form['contractor_master_edit']['contractor_type']	 								=	array(
																									'#title'	 	=>	'Contractor has/is',
																									'#prefix'	 	=>	'<fieldset><section><label class="select">',
																									'#suffix'	 	=>	'</label></section>',
																									'#type'		 	=>	'select',
																									'#options'	 	=>	$contractor_type,
																									'#default_value'=>  $contractor_type_data,
																									'#required'	 	=>	TRUE,
																									'#attributes'	=>	array('id' => 'contractor_type_id')
																								);											 
							
		$form['contractor_master_edit']['name_of_contractor'] 								= array(
																								  '#prefix' 			=> 	'<div class="row"><section class="col col-6"><label class="input">',
																								  '#suffix' 			=> 	'</label></section>',
																								  '#title'				=>	'1. Name of the Contractor',
																								  '#type' 				=> 	'textfield',
																								  '#default_value' 		=> 	$data->name_of_contractor
																						  );
							
		$form['contractor_master_edit']['email_of_contractor'] 								= array(
																								  '#prefix' 			=> 	'<section class="col col-6"><label class="input">',
																								  '#suffix' 			=> 	'</label></section></div>',
																								  '#title'				=>	'2. Email of the Contractor</span>',
																								  '#type' 				=> 	'textfield',
																								  '#default_value' 		=> 	$data->email_of_contractor
																							);
													
		$form['contractor_master_edit']['address_of_contractor'] 							= array(
																								  '#prefix' 			=> 	'<section><label class="textarea">',
																								  '#suffix' 			=> 	'</label></section>',
																								  '#title'				=>	'3. Address of the Contractor</span>',
																								  '#rows'				=>	t('2'),	
																								  '#type' 				=> 	'textarea',
																								  '#default_value' 		=> 	$data->address_of_contractor
																							 );
																							 
		$state_opts																			=	array( 1 => 'West Bengal', 2 => 'Other State' );
																						
		$form['contractor_master_edit']['state_opts']										=	array(
																									'#title'			=>	'4. Select State',
																									'#prefix'			=>	'<section><label class="select">',
																									'#suffix'			=>	'</label></section>',
																									'#type'				=>	'select', 
																									'#options'			=>	$state_opts,
																									'#default_value' 	=>  !empty($state_options) ? $state_options	:'',
																									'#required'			=>	TRUE,
																									'#ajax'				=> 	array(
																																'event' 		=> 'change',
																																'effect' 		=> 'fade',			
																																'callback'		=> 'my_ajax_edit_amendment_callback',
																																'progress'		=> '',
																															)
																								);
																								
		$value_state_opts 																	= 	isset($form_state['values']['state_opts']) ? $form_state['values']['state_opts'] : '';
		
		if(empty($value_state_opts) &&!empty($state_options))
				$value_state_opts	=	$state_options;
				
		$form['contractor_master_edit']['state_others']										=	array(
																									'#prefix'			=>	'<div id="textarea_amendment_edit1">',
																									'#suffix'			=>	'</div>'
																								);
																			
		$form['contractor_master_edit']['state_wb_s']										=	array(
																									'#prefix'			=>	'<div id="textarea_amendment_edit2">',
																									'#suffix'			=>	''
																								);
		
		$form['contractor_master_edit']['state_wb_e']										=	array(
																									'#prefix'			=>	'',
																									'#suffix'			=>	'</div>'
																								);
																								
		if($value_state_opts == 1 && $value_state_opts != "" && !empty($value_state_opts)){	
		
			$form['contractor_master_edit']['state_wb_s']									=	array(
																									'#prefix'			=>	'<div id="textarea_amendment_edit2">',
																									'#suffix'			=>	''
																								);
	
		
			$options_dist 	= custom_user_district_list(); 
	
			$form['contractor_master_edit']['state_wb_s']['con_loc_e_dist'] 				= array(
																							  '#prefix' 				=> '<div class="row"><section class="col col-6"><label class="select">',
																							  '#suffix' 				=> '</label></section>',
																							  '#title'					=>'4.a.) Select District',
																							  '#type' 					=> 'select',
																							  '#options' 				=> $options_dist,
																							  '#required' 				=> TRUE,
																							  '#default_value' 			=> !empty($con_loc_e_dist) ? $con_loc_e_dist	:'',
																							  '#ajax'					=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_edit_con_registration_info_form_ajax_callback',
																															'progress'			=> '',
																														  )
																						);
										
		$value_loc_e_dist 	= 	isset($form_state['values']['con_loc_e_dist']) ? $form_state['values']['con_loc_e_dist'] : '';
			
		if(empty($value_loc_e_dist) &&!empty($con_loc_e_dist))
					$value_loc_e_dist	=	$con_loc_e_dist;
					
		$form['contractor_master_edit']['state_wb_s']['con_loc_e_subdivision'] 							= array(
																								'#prefix' 				=> '<div id="clra_subdivision_replace">',
																								'#suffix' 				=> '</div>',
																							);
		
		$form['contractor_master_edit']['state_wb_s']['con_loc_e_subdivision'] 							= array(
																								  '#prefix' 			=> '<div id="clra_subdvsn_replace"><section class="col col-6"><label class="select">',
																								  '#suffix' 			=> '</label></section></div></div>',
																								  '#title'				=>'4.b.) Select Subdivision',
																								  '#type' 				=> 'select',
																								  '#options' 			=> custom_user_subdivisionlist_list($value_loc_e_dist),
																								  '#default_value' 		=> !empty($con_loc_e_subdivision) ? $con_loc_e_subdivision	:'',
																								  '#required' 			=> TRUE
																							);
															
		$form['contractor_master_edit']['state_wb_s']['con_loc_e_areatype'] 								= array(
																								  '#prefix' 			=> '<div class="row"><section class="col col-6"><label class="select">',
																								  '#suffix' 			=> '</label></section>',
																								  '#title'				=>'4.c.) Select Area type',
																								  '#type' 				=> 'select',
																								  '#options' 			=>array('T'=>'Select Area Type','B' =>'Block','M' => 'Municipality','C' => 'Corporation', 'S' =>'SEZ','N' =>'Notified Area'),
																								  '#default_value' 		=> !empty($con_loc_e_areatype) ? $con_loc_e_areatype	:'',
																								  '#required' 			=> TRUE,
																								  '#ajax'				=> array(
																																'event' 			=> 'change',
																																'effect' 			=> 'fade',			
																																'callback'			=> 'get_edit_con_registration_info_form_ajax_callback',
																																'progress'			=> '',
																															  ),
																									);
		$form['contractor_master_edit']['state_wb_s']['con_loc_e_block'] 									= array(
																								'#prefix' 				=> '<div id="clra_block_replace">',
																								'#suffix' 				=> '</div>'
																								);
													  
		$form['contractor_master_edit']['state_wb_s']['con_loc_e_mun'] 									= array(
																								'#prefix' 				=> '<div id="clra_mun_replace">',
																								'#suffix' 				=> '</div>'
																								);
													  
		$form['contractor_master_edit']['state_wb_s']['con_loc_e_corp'] 									= array(
																								'#prefix' 				=> '<div id="clra_other_replace">',
																								'#suffix' 				=> '</div>'
																								);
													  
		$form['contractor_master_edit']['state_wb_s']['con_loc_e_corp'] 									= array(
																								'#prefix' 				=> '<div id="clra_corp_replace">',
																								'#suffix' 				=> '</div>'
																							  );
													  
		$form['contractor_master_edit']['state_wb_s']['con_loc_e_sez'] 									= array(
																								'#prefix' 				=> '<div id="clra_sez_replace">',
																								'#suffix' 				=> '</div>'
																							  );
													  
		$form['contractor_master_edit']['state_wb_s']['con_loc_e_not'] 									= array(
																								'#prefix' 				=> '<div id="clra_not_replace">',
																								'#suffix' 				=> '</div>'
																							  );	
		$value_loc_e_areatype 		= isset($form_state['values']['con_loc_e_areatype']) ? $form_state['values']['con_loc_e_areatype'] : '';
		$value_loc_e_subdivision 	= isset($form_state['values']['con_loc_e_subdivision']) ? $form_state['values']['con_loc_e_subdivision'] : '';
		
		if(empty($value_loc_e_areatype) && !empty($con_loc_e_areatype))
					$value_loc_e_areatype=$con_loc_e_areatype;
	
		if(empty($value_loc_e_subdivision) && !empty($con_loc_e_subdivision))
					$value_loc_e_subdivision=$con_loc_e_subdivision;
	
		if(!empty($value_loc_e_areatype)){
			if($value_loc_e_areatype == 'B'){
				$form['contractor_master_edit']['state_wb_s']['con_loc_e_block'] 							= array(
																								  '#prefix' 			=> '<div id="clra_block_replace"><section class="col col-6"><label class="select">',
																								  '#suffix' 			=> '</label></section></div>',
																								  '#title'				=>'4.d.) Select Block',
																								  '#type' 				=> 'select',
																								  '#options' 			=> custom_user_block_list($value_loc_e_subdivision,$value_loc_e_areatype),
																								  '#default_value' 		=> !empty($con_name_areatype) ? $con_name_areatype	:'',
																								  '#required' 			=> TRUE,
																								  '#ajax'				=> array(
																																'event' 			=> 'change',
																																'effect' 			=> 'fade',			
																																'callback'			=> 'get_edit_con_registration_info_form_ajax_callback',
																																'progress'			=> '',
																															  )
																							 );
			}else if($value_loc_e_areatype == 'M'){
				$form['contractor_master_edit']['state_wb_s']['con_loc_e_mun'] 							= array(
																								  '#prefix' 			=> '<div id="clra_mun_replace"><section class="col col-6"><label class="select">',
																								  '#suffix' 			=> '</label></section></div>',
																								  '#title'				=>'4.d.) Select Municipality',
																								  '#type' 				=> 'select',
																								  '#options' 			=> custom_user_block_list($value_loc_e_subdivision,$value_loc_e_areatype),
																								  '#default_value' 		=> !empty($con_name_areatype) ? $con_name_areatype	:'',
																								  '#required' 			=> TRUE,
																								  '#ajax'				=> array(
																																'event' 			=> 'change',
																																'effect' 			=> 'fade',			
																																'callback'			=> 'get_edit_con_registration_info_form_ajax_callback',
																																'progress'			=> '',
																															  )
																								);	
			}else if($value_loc_e_areatype == 'C'){
				$form['contractor_master_edit']['state_wb_s']['con_loc_e_corp'] 							= array(
																								  '#prefix' 			=> '<div id="clra_corp_replace"><section class="col col-6"><label class="select">',
																								  '#suffix' 			=> '</label></section></div>',
																								  '#title'				=>'4.d.) Select Corporation',
																								  '#type' 				=> 'select',
																								  '#options' 			=> custom_user_block_list($value_loc_e_subdivision,$value_loc_e_areatype),
																								  '#default_value' 		=> !empty($con_name_areatype) ? $con_name_areatype	:'',
																								  '#required' 			=> TRUE,
																								  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_edit_con_registration_info_form_ajax_callback',
																															'progress'			=> '',
																														  )
																							);
			}else if($value_loc_e_areatype == 'S'){
				$form['contractor_master_edit']['state_wb_s']['con_loc_e_sez'] 							= array(
																								  '#prefix' 			=> '<div id="clra_sez_replace"><section class="col col-6"><label class="select">',
																								  '#suffix' 			=> '</label></section></div>',
																								  '#title'				=>'4.d.) Select SEZ',
																								  '#type' 				=> 'select',
																								  '#options' 			=> custom_user_block_list($value_loc_e_subdivision,$value_loc_e_areatype),
																								  '#default_value' 		=> !empty($con_name_areatype) ? $con_name_areatype	:'',
																								  '#required' 			=> TRUE,
																								  '#ajax'				=> array(
																																'event' 			=> 'change',
																																'effect' 			=> 'fade',			
																																'callback'			=> 'get_edit_con_registration_info_form_ajax_callback',
																																'progress'			=> '',
																															  )		
																								);
			}else if($value_loc_e_areatype == 'N'){
				$form['contractor_master_edit']['state_wb_s']['con_loc_e_not'] 							= array(
																								  '#prefix' 			=> '<div id="clra_not_replace"><section class="col col-6"><label class="select">',
																								  '#suffix' 			=> '</label></section></div>',
																								  '#title'				=>'4.d.) Select Notified Area',
																								  '#type' 				=> 'select',
																								  '#options' 			=> custom_user_block_list($value_loc_e_subdivision,$value_loc_e_areatype),
																								  '#default_value' 		=> !empty($con_name_areatype) ? $con_name_areatype	:'',
																								  '#required' 			=> TRUE,
																								  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_edit_con_registration_info_form_ajax_callback',
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
			
				
		if(empty($code) && !empty($con_name_areatype))
			$code = $con_name_areatype; 
		
			$form['contractor_master_edit']['state_wb_s']['con_loc_e_vill'] 								= array(
																								'#prefix' 					=> '<div id="clra_vill_replace">',
																								'#suffix' 					=> '</div>'
																								 );
													 
			$form['contractor_master_edit']['state_wb_s']['con_loc_e_ward'] 								= array(
																								'#prefix' 					=> '<div id="clra_ward_replace">',
																								'#suffix' 					=> '</div>'
																								);	
													 
			$form['contractor_master_edit']['state_wb_s']['con_loc_e_bugh'] 								= array(
																								'#prefix' 					=> '<div id="clra_bugh_replace">',
																								'#suffix' 					=> '</div>'
																								);
																								
			$form['contractor_master']['state_wb_s']['con_loc_e_sector'] 									= array(
																									'#prefix' 				=> '<div id="clra_sector_replace">',
																									'#suffix' 				=> '</div>'
																									);
																									
			$form['contractor_master']['state_wb_s']['con_loc_e_notifyarea'] 							= array(
																							'#prefix' 				=> '<div id="clra_notified_area_replace">',
																							'#suffix' 				=> '</div>'
																						);
																								
													 
			$form['contractor_master_edit']['state_wb_s']['con_code'] 									= array(	
																								'#prefix' 					=> '<div id="clra_code_replace">',
																								'#suffix' 					=> '</div>'
																							 );
											 
		if(!empty($code)){
			
			$form['contractor_master_edit']['state_wb_s']['con_code'] 									 = array(
																									'#prefix' 				=> '<div id="clra_code_replace">',
																									'#suffix' 				=> '</div>',
																									'#type' 				=> 'hidden',
																									'#value'				=> !empty($code) ? $code	:''
																									 );	
			
			
			if($value_loc_e_areatype == 'B'){
				$form['contractor_master_edit']['state_wb_s']['con_loc_e_vill'] 							= array(
																								  '#prefix' 				=> '<div id="clra_vill_replace"><section class="col col-4"><label class="select">',
																								  '#suffix' 				=> '</label></section></div>',
																								  '#title'					=>'4.e.) Select Gram Panchayat',
																								  '#type' 					=> 'select',
																								  '#options' 				=> custom_user_villward_list($code,'V'),
																								  '#default_value' 			=> !empty($con_loc_e_vill_ward) ? $con_loc_e_vill_ward	:'',
																								  '#required' 				=> TRUE,
																								);
			}
			
			if($value_loc_e_areatype == 'M'){
				$form['contractor_master_edit']['state_wb_s']['con_loc_e_ward'] 							= array(
																								  '#prefix' 				=> '<div id="clra_ward_replace"><section class="col col-4"><label class="select">',
																								  '#suffix' 				=> '</label></section></div>',
																								  '#title'					=> '4.e.) Select Ward',
																								  '#type' 					=> 'select',
																								  '#options' 				=> custom_user_villward_list($code,'W'),// come from custom_user module
																								  '#default_value' 			=> !empty($con_loc_e_vill_ward) ? $con_loc_e_vill_ward	:'',
																								  '#required' 				=> TRUE,
																								);
			}
			
			if($value_loc_e_areatype == 'C'){
				$form['contractor_master_edit']['state_wb_s']['con_loc_e_bugh'] 							= array(
																								  '#prefix' 				=> '<div id="clra_bugh_replace"><section class="col col-4"><label class="select">',
																								  '#suffix' 				=> '</label></section></div>',
																								  '#title'					=>'4.e.) Select Ward',
																								  '#type' 					=> 'select',
																								  '#options' 				=> custom_user_villward_list($code,'C'),
																								  '#default_value' 			=> !empty($con_loc_e_vill_ward) ? $con_loc_e_vill_ward	:'',
																								  '#required' 				=> TRUE,
																								);
			}
			if($value_loc_e_areatype == 'S'){
					$form['contractor_master_edit']['state_wb_s']['con_loc_e_sector'] 					= array(
																							  '#prefix' 			=> '<div id="clra_sector_replace"><section class="col col-4"><label class="select">',
																							  '#suffix' 			=> '</label></section></div>',
																							  '#title'				=>'4.e.) Select Sector',
																							  '#type' 				=> 'select',
																							  '#options' 			=> custom_user_villward_list($code,'S'),// come from custom_user module
																							  '#default_value' 		=> !empty($con_loc_e_vill_ward) ? $con_loc_e_vill_ward	:'',
																							  '#required' 			=> TRUE,
																							);
				}
				
			if($value_loc_e_areatype == 'N'){
					$form['contractor_master_edit']['state_wb_s']['con_loc_e_notifyarea'] 					= array(
																							  '#prefix' 			=> '<div id="clra_notified_area_replace"><section class="col col-4"><label class="select">',
																							  '#suffix' 			=> '</label></section></div>',
																							  '#title'				=>'4.e.) Select Notified Area',
																							  '#type' 				=> 'select',
																							  '#options' 			=> custom_user_villward_list($code,'N'),// come from custom_user module
																							  '#default_value' 		=> !empty($con_loc_e_vill_ward) ? $con_loc_e_vill_ward	:'',
																							  '#required' 			=> TRUE,
																							);
				}
		}
	
		$form['contractor_master_edit']['state_wb_s']['con_loc_e_ps'] 									= array(
																								'#prefix' 					=> '<div id="clra_ps_replace">',
																								'#suffix' 					=> '</div>'
																								);
											   
		$form['contractor_master_edit']['state_wb_s']['con_loc_e_ps'] 									= array(
																								  '#prefix' 				=> '<div id="clra_ps_replace"><section class="col col-4"><label class="select">',
																								  '#suffix' 				=> '</label></section></div>',
																								  '#title'					=>'4.f.) Select Police Station',
																								  '#type' 					=> 'select',
																								  '#options' 				=> custom_user_ps_list($value_loc_e_dist),// come from custom_user module
																								  '#default_value' 			=> !empty($con_l_e_ps) ? $con_l_e_ps	:'',
																								  '#required' 				=> TRUE  
																								);
																								
		$form['contractor_master_edit']['state_wb_s']['contractor_pin'] 									= array(
																								  '#prefix' 				=> '<section class="col col-4"><label class="input">',
																								  '#suffix' 				=> '</label></section></div>',
																								  '#title'					=> '4.g.) PIN Code',
																								  '#type' 					=> 'textfield',
																								  '#default_value' 			=> !empty($contractor_pin) ? $contractor_pin	:'',
																								   '#attributes'			=>  array('maxlength' => '6'),
																								  '#required' 				=> TRUE  
																								);
																								
			$form['contractor_master_edit']['state_wb_e']									=	array(
																									'#prefix'			=>	'',
																									'#suffix'			=>	'</div>'
																								);
																								
		}
		
		if($value_state_opts == 2 && $value_state_opts != "" && !empty($value_state_opts)){
			$form['contractor_master_edit']['state_others']					=	array(
																				'#prefix'			=>	'<div id="textarea_amendment_edit1"><section><label class="textarea">',
																				'#suffix'			=>	'</label></section></div>',
																				'#type'				=>	'textarea',
																				'#title'			=>	'4.a.) Other State',
																				'#required'			=>	TRUE,
																				'#default_value'	=>  $data->state_others
																			);
		}
											
		
		$options_cont 	= 	contractor_works_multiSelect_options();
		$options_cont 	=  	array(0 => "Select Work") + $options_cont;
		//$insertedNatureOfWork	=	array(0=>'Select Nature of Work');
		//$insertedNatureOfWork	=	$insertedNatureOfWork + getNatureOfWorkOpts_ed($application_id);
		
		
		$form['contractor_master_edit']['contractor_nature_of_work'] 						= array(
																								  '#prefix' 				=> 	'<section><label class="select">',
																								  '#suffix' 				=> 	'</label></section>',
																								  '#title'					=>	'5. Nature of Work in which Contract Labour is Employed or is to be Employed',
																								  '#type' 					=> 	'select',
																								  //'#options' 				=> 	$insertedNatureOfWork,
																								  //'#default_value'			=> 	$result_Id,
																								  '#options' 				=> 	$options_cont,
																								  '#default_value'			=> 	$resultChunk,
																								  '#multiple' 				=> 	TRUE,
																								  '#attributes' 			=> 	$readonly_license_issued,
																							 );
							
		$form['contractor_master_edit']['contractor_max_no_of_labours_on_any_day'] 			= array(
																								  '#prefix' 				=> 	'<section><label class="input">',
																								  '#suffix' 				=> 	'</label></section>',
																								  '#title'					=>	'6. Maximum Number of Contractor Labour to be Employed on any day Through Each Contractor to be engaged',
																								  '#type' 					=> 	'textfield',
																								  '#required'				=>	true,
																								  '#default_value' 			=> 	$data->contractor_max_no_of_labours_on_any_day
																								);
								
		$form['contractor_master_edit']['est_date_of_work_of_each_labour'] 					= array(
																								  '#prefix' 				=> '<section><label class="input">',
																								  '#suffix' 				=> '</label></section>',
																								  '#type' 					=> 'markup',
																								  '#markup'					=>'7. Estimated Date of Employment of Each Contract Work Under Each Contractor <span style="color:#f00;">* </span>',
																						   );
							
		$form['contractor_master_edit']['est_date_of_work_of_each_labour_from_date'] 		= array(
																								'#prefix' 					=> 	'<div class="row"><section class="col col-4"><label class="input">',
																								'#suffix'	 				=> 	'</label></section>',
																								'#type' 					=> 	'textfield',
																								'#attributes'				=>	array('id' => 'est_date_of_work_of_each_labour_from_date_id', 'class'=> array('est_date_of_work_of_each_labour_from_date_class'), 'readonly'=>'readonly','placeholder' => t('From'), 'onclick' => 'return datepicker_func();'),
																								'#default_value' 			=> 	!empty($data->est_date_of_work_of_each_labour_from_date) ? date('d-m-Y',strtotime($data->est_date_of_work_of_each_labour_from_date)): ''
																						  );
							
		$form['contractor_master_edit']['est_date_of_work_of_each_labour_to_date'] 			= array(
																								'#prefix' 					=> 	'<section class="col col-4"><label class="input">',
																								'#suffix' 					=> 	'</label></section>',
																								'#type' 					=> 	'textfield',
																								'#attributes'				=>	array('id' => 'est_date_of_work_of_each_labour_to_date_id', 'class'=> array('est_date_of_work_of_each_labour_to_date_class'), 'readonly'=>'readonly', 'placeholder'=> t('To'),'onclick' => 'return datepicker_func();'),
																								'#default_value' 			=> 	!empty($data->est_date_of_work_of_each_labour_to_date) ? date('d-m-Y',strtotime($data->est_date_of_work_of_each_labour_to_date)) : ''
																						);	
						
		$form['contractor_master_edit']['est_date_of_work_of_each_labour_total_months'] 	= array(
																								'#prefix' 					=> 	'<section class="col col-4"><label class="input">',
																								'#suffix' 					=> 	'</label></section><div></fieldset>',
																								'#type' 					=> 	'textfield',
																								'#attributes'				=>	array('id'=> 'est_date_of_work_of_each_labour_total_months_edit_id', 'readonly'=>'readonly','placeholder' => t('Total Days')),		
																								'#default_value' 			=> 	$data->est_date_of_work_of_each_labour_total_months
																							);
		
		
		
		$form['contractor_master_edit']['submit'] 											= array (
																								'#prefix' 					=> 	'<footer>',  
																								'#type' 					=> 	'submit',
																								'#attributes' 				=> 	array('class' => array('button')),
																								'#value' 					=> 	'Submit'
																							);
																							
		$form['contractor_master_edit']['back_link'] 										= 	array(
																									'#type' 				=> 'markup',
																									'#suffix' 				=> 	'</footer></div></div>',
																									'#markup'				=> l('Back To Contractor List', 'clra-reg-amendment/clra-contractor-info/'.encryption_decryption_fun($action_encrypt, $application_id))
																								);
		
		return $form;
	}

	function edit_contractor_form_validate($form, &$form_state){
		
	}
	
	function edit_contractor_form_submit($form, &$form_state){
		
		global $base_root, $base_path, $user;
		
		ctools_include('ajax');
		ctools_include('modal');
		ctools_modal_add_js();
		
		
		$val 											= $form_state['values'];
		
		$identification_number							= $val['identification_number'];
		$application_id									= $val['application_id'];
		$act_id											= $val['act_id'];
		$increment 										= 0;
		$contractor_id									= $val['hidden_id'];
		
		
		$contractor_type								= $val['contractor_type'];
		$name_of_contractor 							= $val['name_of_contractor'];
		$address_of_contractor 							= $val['address_of_contractor'];
		$email_of_contractor							= $val['email_of_contractor'];
		
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
		$est_date_of_work_of_each_labour_from_date  	= date('Y-m-d',strtotime($val['est_date_of_work_of_each_labour_from_date']));
		$est_date_of_work_of_each_labour_to_date    	= date('Y-m-d',strtotime($val['est_date_of_work_of_each_labour_to_date']));
		$est_date_of_work_of_each_labour_total_months 	= $val['est_date_of_work_of_each_labour_total_months'];
		
		db_delete('l_clra_con_nature_of_work')
						->condition('contractor_id', trim($contractor_id), '=')
						->execute();
		
		$queryabc 									= db_update('l_contractor_info_master');
        $queryabc->fields(array(
		
								'identification_number'							=>  $identification_number,
								'application_id'								=>  $application_id,
								'act_id'										=>  $act_id,
								
								'contractor_type'								=>	$contractor_type,
								'name_of_contractor'							=>	$name_of_contractor,
								'email_of_contractor'							=>  $email_of_contractor,
								'address_of_contractor'							=>	$address_of_contractor,
								
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
								'est_date_of_work_of_each_labour_to_date'   	=>  $est_date_of_work_of_each_labour_to_date,
								'est_date_of_work_of_each_labour_total_months'	=>  $est_date_of_work_of_each_labour_total_months,
								
								
         					));
							
        $queryabc->condition('id',trim($contractor_id));
		$queryabc->condition('application_id',trim($application_id));
        $queryabc->execute();
		
		if(!empty($contractor_nature_of_work)){	
				  	foreach($contractor_nature_of_work as $LoopValueData){
					  	$fieldsAllContractorInfoNatureOfWork 				= 	array( 
																					'contractor_id'                 =>  $contractor_id,
																					'identification_number'			=>	$identification_number,
																					'user_id'						=>	$user->uid,
																					'application_id'				=>  $application_id,
																					'act_id'						=>  $act_id,
																					'contractor_nature_of_work'		=>	$LoopValueData
																					);	
																					
					  	db_insert('l_clra_con_nature_of_work')->fields($fieldsAllContractorInfoNatureOfWork)->execute();
					  	$increment++;
				  	}
		}
		
		
		if( $contractor_id != "" ){
			drupal_goto('clra-reg-amendment/clra-contractor-info/'.encryption_decryption_fun('encrypt', $application_id));
		}
	}

	function getNatureOfWorkOpts_ed($application_id){
	
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

	function get_edit_con_registration_info_form_ajax_callback($form, $form_state) {
		$commands = array();
		$commands[] = ajax_command_replace('#clra_subdvsn_replace', drupal_render($form['contractor_master_edit']['state_wb_s']['con_loc_e_subdivision']));
		$commands[] = ajax_command_replace('#clra_mun_replace', drupal_render($form['contractor_master_edit']['state_wb_s']['con_loc_e_mun']));
		$commands[] = ajax_command_replace('#clra_block_replace', drupal_render($form['contractor_master_edit']['state_wb_s']['con_loc_e_block']));
		$commands[] = ajax_command_replace('#clra_other_replace', drupal_render($form['contractor_master_edit']['state_wb_s']['con_loc_e_other']));
		$commands[] = ajax_command_replace('#clra_corp_replace', drupal_render($form['contractor_master_edit']['state_wb_s']['con_loc_e_corp']));
		$commands[] = ajax_command_replace('#clra_sez_replace', drupal_render($form['contractor_master_edit']['state_wb_s']['con_loc_e_sez']));
		$commands[] = ajax_command_replace('#clra_not_replace', drupal_render($form['contractor_master_edit']['state_wb_s']['con_loc_e_not']));
		$commands[] = ajax_command_replace('#clra_vill_replace', drupal_render($form['contractor_master_edit']['state_wb_s']['con_loc_e_vill']));
		$commands[] = ajax_command_replace('#clra_ward_replace', drupal_render($form['contractor_master_edit']['state_wb_s']['con_loc_e_ward']));
		$commands[] = ajax_command_replace('#clra_ps_replace', drupal_render($form['contractor_master_edit']['state_wb_s']['con_loc_e_ps']));
		$commands[] = ajax_command_replace('#clra_bugh_replace', drupal_render($form['contractor_master_edit']['state_wb_s']['con_loc_e_bugh']));
		$commands[] = ajax_command_replace('#clra_sector_replace', drupal_render($form['contractor_master']['state_wb_s']['con_loc_e_sector']));
		$commands[] = ajax_command_replace('#clra_notified_area_replace', drupal_render($form['contractor_master']['state_wb_s']['con_loc_e_notifyarea']));
		$commands[] = ajax_command_replace('#clra_code_replace', drupal_render($form['contractor_master_edit']['state_wb_s']['con_code']));
  		return array('#type' => 'ajax', '#commands' => $commands);
	}
	
	function my_ajax_edit_amendment_callback($form, $form_state){
		$commands	=	array();
		$commands[] = ajax_command_replace('#textarea_amendment_edit1', drupal_render($form['contractor_master_edit']['state_others']));
		$commands[] = ajax_command_replace('#textarea_amendment_edit2', drupal_render($form['contractor_master_edit']['state_wb_s'],$form['contractor_master_edit']['state_wb_e']));
		return array('#type' => 'ajax', '#commands' => $commands);
	}
	
