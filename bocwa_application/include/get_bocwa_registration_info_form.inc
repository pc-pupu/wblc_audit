<?php

function get_bocwa_registration_info_form($form, &$form_state, $id=''){ 
	drupal_add_library('system','ui.datepicker');
	drupal_add_js(drupal_get_path('module', 'bocwa_application') . '/js/bocwa_upload_custom.js');
	
	global $base_root,$base_path,$user; 
			
	$user_id 		= $user->uid;
	$application_id	= encryption_decryption_fun('decrypt',$id);
	
	
	$bocwa_fetch_reg_det = db_query("select * from l_bocwa_registration_master where id=:application_id and user_id=:user_id", array( ':application_id' => $application_id, ':user_id' => $user_id ));
	
	if($bocwa_fetch_reg_det->rowCount() > 0 ){
				
					$obj2 									=  $bocwa_fetch_reg_det->fetchAssoc();
					$identification_number					=	$obj2['identification_number'];
					$act_id									=   $obj2['act_id'];
					$e_name									=	$obj2['e_name'];
					$est_type								=	$obj2['est_type'];
					$loc_e_name								=	$obj2['loc_e_name'];
					$loc_e_dist								=	$obj2['loc_e_dist'];
					$loc_e_subdivision						=	$obj2['loc_e_subdivision'];
					$loc_e_areatype							=	$obj2['loc_e_areatype'];
					$loc_e_areatype_code					= 	$obj2['loc_e_areatype_code'];
					$loc_e_vill_ward						=	$obj2['loc_e_vill_ward'];					
					$loc_e_ps								=	$obj2['loc_e_ps'];
					$loc_e_pin_number						=   $obj2['loc_e_pin_number'];
					
					
					$e_postal_address						=	$obj2['e_postal_address'];
					$e_postal_dist							=	$obj2['e_postal_dist'];
					$e_postal_subdivision					=	$obj2['e_postal_subdivision'];
					$e_postal_areatype						=	$obj2['e_postal_areatype'];
					$e_postal_areatype_code					=   $obj2['e_postal_areatype_code'];
					$e_postal_vill_ward						=	$obj2['e_postal_vill_ward'];
					$e_postal_borough						=	$obj2['e_postal_vill_ward'];
					$e_postal_ps							=	$obj2['e_postal_ps'];
					$e_postal_pin_number					=   $obj2['e_postal_pin_number'];
					
					
					$e_full_name							=	$obj2['e_full_name'];
					$e_permanent_address					=	$obj2['e_permanent_address'];
					$e_permanent_add_dist					=	$obj2['e_permanent_add_dist'];
					$e_permanent_add_subdivision			=	$obj2['e_permanent_add_subdivision'];
					$e_permanent_add_areatype				=	$obj2['e_permanent_add_areatype'];
					$e_permanent_add_areatype_code			=	$obj2['e_permanent_add_areatype_code'];
					$e_permanent_add_vill_ward				=	$obj2['e_permanent_add_vill_ward'];
					$e_permanent_add_borough				=	$obj2['e_permanent_add_vill_ward'];
					$e_permanent_add_ps						=	$obj2['e_permanent_add_ps'];
					$e_permanent_add_pin_number				=   $obj2['e_permanent_add_pin_number'];
					
					
					$emp_name  								= $obj2['emp_name'];
					$emp_gender  							= $obj2['emp_gender'];
					$emp_address 							= $obj2['emp_address'];
					$emp_dist 								= $obj2['emp_dist'];
					$emp_subdivision						= $obj2['emp_subdivision'];
					$emp_areatype 							= $obj2['emp_areatype'];
					$emp_areatype_code 						= $obj2['emp_areatype_code'];
					$emp_vill_ward 							= $obj2['emp_vill_ward'];
					$emp_borough							= $obj2['emp_vill_ward'];
					$emp_ps 								= $obj2['emp_ps'];
					$emp_pin_number							= $obj2['emp_pin_number'];
					
					
					$full_name_manager						=	$obj2['full_name_manager'];
					$address_manager						=	$obj2['address_manager'];
					$address_manager_dist					=	$obj2['address_manager_dist'];
					$address_manager_subdivision			=	$obj2['address_manager_subdivision'];
					$address_manager_areatype 				=	$obj2['address_manager_areatype'];
					$address_manager_areatype_code			=   $obj2['address_manager_areatype_code'];
					$address_manager_vill_ward				=	$obj2['address_manager_vill_ward'];
					$address_manager_borough				=	$obj2['address_manager_vill_ward'];
					$address_manager_ps						=	$obj2['address_manager_ps'];
					$address_manager_pin_number				=   $obj2['address_manager_pin_number'];
					
					
					$nature_of_build_const					=	$obj2['nature_of_build_const'];
					$max_no_of_building_workers_employed	=	$obj2['max_no_of_building_workers_employed'];
					$est_date_of_commencement_building		=	$obj2['est_date_of_commencement_building'];
					$est_date_of_completion_building		=	$obj2['est_date_of_completion_building'];
					$final_submit_status      				=   $obj2['final_submit_status'];
					$status      							=   $obj2['status'];
					$backlog_id								=	$obj2['backlog_id'];
			}
	
	
	drupal_add_library('system','ui.datepicker');
	
	drupal_add_js('jQuery(document).ready(function(){
		jQuery("#est_date_of_commencement_id").datepicker({
		  dateFormat: "dd/mm/yy",
		  yearRange: "-50:+30",
		  changeMonth: true, 
		  changeYear: true,
		  onSelect: function(selected) {
			  jQuery("#est_date_of_termination_id").datepicker("option","minDate", selected)
		  }
		});
		jQuery("#est_date_of_termination_id").datepicker({
		  dateFormat: "dd/mm/yy",
		  yearRange: "-50:+30",
		  changeMonth: true, 
		  changeYear: true,
		  onSelect: function(selected) {
			  jQuery("#est_date_of_commencement_id").datepicker("option","maxDate", selected)
		  }
		});
		
	});', 'inline');
	
	$readonly_loc_e_pin_number						= array('maxlength' => 6);
	$readonly_e_postal_pin_number					= array('maxlength' => 6);
	$readonly_e_permanent_add_pin_number			= array('maxlength' => 6);	
	$readonly_emp_pin_number						= array('maxlength' => 6);	
	$readonly_address_manager_pin_number			= array('maxlength' => 6);	
	$readonly_est_date_of_commencement_building		= array('readonly'=>'readonly', 'placeholder' => t('From'));
	$readonly_est_date_of_completion_building		= array('readonly'=>'readonly', 'placeholder'=> t('To'));
		
					
					
					
	if( trim($status == 'B') || trim($final_submit_status) == '' || trim($final_submit_status) == 'S' ){
		
		if(!empty($application_id) && $status == 'B') {
			
			
			$query 		 = db_select('l_bocwa_remark_master', 'lbkrm')
						 ->	fields('lbkrm', array('remark_field_title'))
						 ->	condition('lbkrm.remark_to', trim($user_id))
						 ->	condition('lbkrm.application_id', $application_id)
						  ->condition('lbkrm.is_active', '1')
						  ->where('((lbkrm.remark_by_role = 4) OR (lbkrm.remark_by_role = 7))')
						  ->range(0, 1)
						 ->	orderBy('lbkrm.id','DESC');
						
						 
						 
			$remark_field_text_query = $query->execute();
			$remark_field_text_array = $remark_field_text_query->fetchAssoc(); 
			$verified_fields = explode(',', $remark_field_text_array['remark_field_title']);
			
			foreach($verified_fields as $fields){ 
			
			
				switch($fields){	
				
					case "e_name":
					
							$readonly_e_name 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
							
					case "est_type":
					
							$readonly_est_type 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
							
							
					case "loc_e_name":
					
							$readonly_loc_e_name 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_loc_e_dist 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_loc_e_subdivision 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_loc_e_areatype 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_loc_e_block 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_loc_e_mun 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_loc_e_corp 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_loc_e_sez 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_loc_e_not 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_loc_e_other 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_loc_e_vill 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_loc_e_ward 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_loc_e_bugh 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_loc_e_ps 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_loc_e_pin_number 					= array('disabled' => 'disabled', 'maxlength' => 6, 'class' => array('readonly_view'));
							break;	
							
					case "e_postal_address":
					
							$readonly_e_postal_address 					= array('readonly' => 'readonly', 'class' => array('readonly_view'));
							$readonly_e_postal_dist 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_postal_subdivision 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));				
							$readonly_e_postal_areatype 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_postal_block 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_postal_mun 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_postal_corp 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_postal_sez 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_postal_not 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_postal_other 					= array('readonly' => 'readonly', 'class' => array('readonly_view'));
							$readonly_e_postal_vill 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_postal_ward 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_postal_bugh 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_postal_ps 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_postal_pin_number				= array('disabled' => 'disabled', 'maxlength' => 6,  'class' => array('readonly_view'));
							break;
							
					case "full_name_est":
					
							$readonly_e_full_name 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
							
					case "per_address_est":
					
							$readonly_e_permanent_address 				= array('readonly' => 'readonly', 'class' => array('readonly_view'));
							$readonly_e_permanent_add_dist 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_e_permanent_add_subdivision 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_permanent_add_areatype 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_permanent_add_block 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_permanent_add_mun 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_e_permanent_add_corp 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_e_permanent_add_sez 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_e_permanent_add_not 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_e_permanent_add_other 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_e_permanent_add_vill 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_e_permanent_add_ward 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_e_permanent_add_borough 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_permanent_add_ps 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_permanent_add_pin_number		= array('disabled' => 'disabled','maxlength' => 6,  'class' => array('readonly_view'));	
							break;
							
							
					case "emp_name":
					
							$readonly_emp_name 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
							
					case "emp_gender":
					
							$readonly_emp_gender 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
							
					case "emp_address":
					
							$readonly_emp_address 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_emp_dist 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_emp_subdivision 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_emp_areatype 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_emp_block 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_emp_mun 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_emp_corp 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_emp_sez 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_emp_not 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_emp_other 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_emp_vill 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_emp_ward 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_emp_bugh 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_emp_sector 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_emp_not_area 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_emp_ps 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_emp_pin_number					= array('disabled' => 'disabled','maxlength' => 6,  'class' => array('readonly_view'));	
							break;
							
					case "full_name_manager":
					
							$readonly_full_name_manager 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
							
					case "address_manager":
					
							$readonly_address_manager 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_address_manager_dist 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_address_manager_subdivision 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_address_manager_areatype 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_address_manager_block 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_address_manager_mun 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_address_manager_corp 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_address_manager_sez 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_address_manager_not 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_address_manager_other 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_address_manager_vill 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_address_manager_ward 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_address_manager_borough 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_address_manager_sector 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_address_manager_not_area 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_address_manager_ps 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_address_manager_pin_number		= array('disabled' => 'disabled','maxlength' => 6,  'class' => array('readonly_view'));	
							break;
							
					case "e_nature_of_work":
					
							$readonly_nature_of_build_const 			= array('disabled' => 'disabled', 'class' => array('readonly_view')) ;
							break;
							
					case "max_num_of_workmen":
					
						 	$readonly_max_no_of_building_workers_employed= array('disabled' => 'disabled', 'class' => array('readonly_view')) ;
							break;
							
					case "est_date_comm":
							$readonly_est_date_of_commencement_building = array('disabled' => 'disabled', 'class' => array('readonly_view')) ;
							break;
							
					case "est_date_completion":
							$readonly_est_date_of_completion_building	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
							
					default:
					
				}
			
				
			}
		
			
		}
			
		
				
		
		
		$form['bocwa_reg_master']								 			= array(
																				'#prefix' 				=>	'<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">1. Full Name and address of the Employer</div><div class="panel-body">',//<h3>BOCWA REGISTRATION FORM</h3>
																				//'#type' 				=> 	'fieldset',
																				'#attributes' 			=> 	array('class'=>array('')),
																				'#collapsed' 			=> 	TRUE,
																				'#collapsable'			=>	TRUE
																				);
	
		$form['bocwa_reg_master']['bocwa_act_id'] 		 					= array(
																				'#title'				=>	'Act Id',
																				'#size'					=>	t('20'),	
																				'#type' 				=> 	'hidden',
																				'#default_value'		=>  !empty($act_id) ? encryption_decryption_fun('encrypt', $act_id) : '',
																				'#attributes'			=> 	array('readonly'=>'readonly')
																				);
	
		$form['bocwa_reg_master']['identification_number']					= array(
																				'#title'				=>	'Identification Number',
																				'#size'					=>	t('20'),	
																				'#type' 				=>  'hidden',
																				'#default_value'		=>  !empty($identification_number) ? encryption_decryption_fun('encrypt', $identification_number) : '',
																				'#attributes' 			=>  array('readonly'=>'readonly')
																				);
																				
																				
		$form['bocwa_reg_master']['application_id']							= array(
																				'#title'				=>	'Application Id',
																				'#size'					=>	t('20'),	
																				'#type' 				=>  'hidden',
																				'#default_value'		=>  !empty($application_id) ? encryption_decryption_fun('encrypt', $application_id) : '',
																				'#attributes' 			=>  array('readonly'=>'readonly')
																				);
																				
																				
		$form['bocwa_reg_master']['emp_name'] 								= array(
																				  //'#prefix'				=> '<fieldset><legend class="form-title">1. Full Name and address of the Employer</legend><div class="row"><section class="col-md-6"><label class="input">',
																				  //'#suffix' 			=> '</label></section>',
																				  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Full Name of the Employer',
																				  '#type' 				=> 'textfield',
																				  '#default_value' 	 	=> !empty($emp_name) ? $emp_name:'',
																				  '#required' 			=> TRUE,
																				  '#attributes' 		=> $readonly_emp_name,
																				  '#element_validate'	=> array('BOCWA_TEXTBOX_TEXT_VALIDATE')
																				);
		$form['bocwa_reg_master']['emp_gender'] 							= array(
																				  '#prefix'				=> '<div class="col-md-4 col-md-6"><div class="inline-group">',
																				  '#suffix'				=>'</div></div>',
																				  '#type' 				=> 'radios',
																				  '#title' 				=> t('Gender'),
																				  '#options' 			=> array('M'=>t('Male'), 'F'=>t('Female'), 'O'=>t('Other')),
																				  '#default_value' 		=> isset($emp_gender) ? $emp_gender : '',
																				  '#attributes' 		=> $readonly_emp_gender,
																				  '#required' 			=> TRUE
																					);
																					
		$form['bocwa_reg_master']['emp_address'] 							= array(
																				  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'House No./Village Name/Street/Road',
																				  '#type' 				=> 'textfield',
																				  '#required' 			=> TRUE,
																				  '#default_value' 	 	=> !empty($emp_address) ? $emp_address:'',
																				  '#attributes' 		=> $readonly_emp_address,
																				  '#element_validate' 	=> array('BOCWA_SPECIAL_CHAR_TEXTAREA_VALIDATE')
																				);
		$options_dist 	= custom_user_district_list(); //---come from custom_user module
		
		$form['bocwa_reg_master']['emp_dist'] 	   							= array(
																				  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select District',
																				  '#type' 				=> 'select',
																				  '#options' 			=> $options_dist,
																				  '#default_value' 		=> !empty($emp_dist) ? $emp_dist	:'',
																				  '#required' 			=> TRUE,
																				  '#attributes' 		=> $readonly_emp_dist,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				  '#ajax'				=> array(
																												'event' 			=> 'change',
																												'effect' 			=> 'fade',			
																												'callback'			=> 'get_edit_bocwa_emp_ajax_callback',
																												'progress'			=> '',
																											  )
																				);
																				
		$value_emp_dist 	= 	isset($form_state['values']['emp_dist']) ? $form_state['values']['emp_dist'] : '';
		
		if(empty($value_emp_dist) &&!empty($emp_dist))
			$value_emp_dist	=	$emp_dist;


		$form['bocwa_reg_master']['emp_subdivision'] 		  				= array(
																					'#prefix' 			=> '<div id="bocwa_emp_subdivision_replace">',
																					'#suffix' 			=> '</div>',
																				);
	
		$form['bocwa_reg_master']['emp_subdivision'] 		  				= array(
																				  '#prefix' 			=> '<div id="bocwa_emp_subdvsn_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Subdivision',
																				  '#type' 				=> 'select',
																				  '#options' 			=> custom_user_subdivisionlist_list($value_emp_dist),// ---------come from custom_user module
																				  '#default_value' 		=> !empty($emp_subdivision) ? $emp_subdivision	:'',
																				  '#required' 			=> TRUE,
																				  '#attributes' 		=> $readonly_emp_subdivision,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																			);
																				
		$form['bocwa_reg_master']['emp_areatype']   		 	 			= array(
																				  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Area type',
																				  '#type' 				=> 'select',
																				  '#options' 			=> array('T'=>'Select Area Type','B' =>'Block','M' => 'Municipality','C' => 'Corporation', 'S' =>'SEZ','N' =>'Notified Area'),
																				  '#element_validate'	=> array('BOCWA_LOCATION_AREATYPE_VALIDATE'),
																				  '#default_value' 		=> !empty($emp_areatype) ? $emp_areatype	:'',
																				  '#required' 			=> TRUE,
																				  '#attributes' 		=> $readonly_emp_areatype,
																				  '#ajax'				=> array(
																													'event' 			=> 'change',
																													'effect' 			=> 'fade',			
																													'callback'			=> 'get_edit_bocwa_emp_ajax_callback',
																													'progress'			=> '',
																												  ),
																					);
																					
		$form['bocwa_reg_master']['emp_block'] 								= array(
																					'#prefix' 			=> '<div id="bocwa_emp_block_replace">',
																					'#suffix' 			=> '</div>'
																			  );
							  
		$form['bocwa_reg_master']['emp_mun'] 								= array(
																					'#prefix' 			=> '<div id="bocwa_emp_mun_replace">',
																					'#suffix' 			=> '</div>'
																				);
					  
		$form['bocwa_reg_master']['emp_corp'] 								= array(
																					'#prefix' 			=> '<div id="bocwa_emp_corp_replace">',
																					'#suffix' 			=> '</div>'
																					);
					  
		$form['bocwa_reg_master']['emp_sez'] 								= array(
																					'#prefix' 			=> '<div id="bocwa_emp_sez_replace">',
																					'#suffix' 			=> '</div>'
																				  );
					  
		$form['bocwa_reg_master']['emp_not'] 								= array(
																					'#prefix' 			=> '<div id="bocwa_emp_not_replace">',
																					'#suffix' 			=> '</div>'
																					);	
					
															  
		$form['bocwa_reg_master']['emp_other'] 								= array(
																					'#prefix' 			=> '<div id="bocwa_emp_other_replace">',
																					'#suffix' 			=> '</div>'
																				  );
		
		
		
		$value_emp_areatype 					= isset($form_state['values']['emp_areatype']) ? $form_state['values']['emp_areatype'] : '';
		$value_emp_subdivision 					= isset($form_state['values']['emp_subdivision']) ? $form_state['values']['emp_subdivision'] : '';
		
		
		if(empty($value_emp_areatype) && !empty($emp_areatype))
			$value_emp_areatype	=	$emp_areatype;
	
		if(empty($value_emp_subdivision) && !empty($emp_subdivision))
			$value_emp_subdivision = $emp_subdivision;
		
		
		if(!empty($value_emp_areatype)){
			if($value_emp_areatype == 'B'){
			
			$form['bocwa_reg_master']['emp_block'] 							= array(
																			  '#prefix' 				=> '<div id="bocwa_emp_block_replace" class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select Block',
																			  '#type' 					=> 'select',
																			  '#options' 				=> custom_user_block_list($value_emp_subdivision,$value_emp_areatype),
																			  '#default_value' 			=> !empty($emp_areatype_code) ? $emp_areatype_code:'',
																			  '#required' 				=> TRUE,
																			  '#attributes' 			=> $readonly_emp_block,
																			  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																			  '#ajax'					=> array(
																												'event' 			=> 'change',
																												'effect' 			=> 'fade',			
																												'callback'			=> 'get_edit_bocwa_emp_ajax_callback',
																												'progress'			=> '',
																											  )
																		 );
					 
		}else if($value_emp_areatype == 'M'){
			
			$form['bocwa_reg_master']['emp_mun'] 						= array(
																			  '#prefix' 				=> '<div id="bocwa_emp_mun_replace" class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select Municipality',
																			  '#type' 					=> 'select',
																			  '#options' 				=> custom_user_block_list($value_emp_subdivision,$value_emp_areatype),
																			  '#default_value' 			=> !empty($emp_areatype_code) ? $emp_areatype_code:'',
																			  '#required' 				=> TRUE,
																			  '#attributes' 			=> $readonly_emp_mun,
																			  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																			  '#ajax'					=> array(
																												'event' 			=> 'change',
																												'effect' 			=> 'fade',			
																												'callback'			=> 'get_edit_bocwa_emp_ajax_callback',
																												'progress'			=> '',
																											  )
																			);
																			
		}else if($value_emp_areatype == 'C'){
			
			$form['bocwa_reg_master']['emp_corp'] 						= array(
																			  '#prefix' 				=> '<div id="bocwa_emp_corp_replace" class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select Corporation',
																			  '#type' 					=> 'select',
																			  '#options' 				=> custom_user_block_list($value_emp_subdivision,$value_emp_areatype),
																			  '#default_value' 			=> !empty($emp_areatype_code) ? $emp_areatype_code:'',
																			  '#required' 				=> TRUE,
																			  '#attributes' 			=> $readonly_emp_corp,
																			  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																			  '#ajax'					=> array(
																												'event' 			=> 'change',
																												'effect' 			=> 'fade',			
																												'callback'			=> 'get_edit_bocwa_emp_ajax_callback',
																												'progress'			=> '',
																											  )		
																		);
																		
		}else if($value_emp_areatype == 'S'){
		
			$form['bocwa_reg_master']['emp_sez'] 						= array(
																			  '#prefix' 				=> '<div id="bocwa_emp_sez_replace" class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select SEZ ',
																			  '#type' 					=> 'select',
																			  '#options' 				=> custom_user_block_list($value_emp_subdivision,$value_emp_areatype),
																			  '#default_value' 			=> !empty($emp_areatype_code) ? $emp_areatype_code:'',
																			  '#required' 				=> TRUE,
																			  '#attributes' 			=> $readonly_emp_sez,
																			  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																			  '#ajax'					=> array(
																												'event' 			=> 'change',
																												'effect' 			=> 'fade',			
																												'callback'			=> 'get_edit_bocwa_emp_ajax_callback',
																												'progress'			=> '',
																											  )
																			);
																			
		}else if($value_emp_areatype == 'N'){

			$form['bocwa_reg_master']['emp_not'] 						= array(
																			  '#prefix' 				=> '<div id="bocwa_emp_not_replace" class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select Notified area',
																			  '#type' 					=> 'select',
																			  '#options' 				=> custom_user_block_list($value_emp_subdivision,$value_emp_areatype),
																			  '#default_value' 			=> !empty($emp_areatype_code) ? $emp_areatype_code:'',
																			  '#required' 				=> TRUE,
																			  '#attributes' 			=> $readonly_emp_not,
																			  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																			  '#ajax'					=> array(
																												'event' 			=> 'change',
																												'effect' 			=> 'fade',			
																												'callback'			=> 'get_edit_bocwa_emp_ajax_callback',
																												'progress'			=> '',
																											  )
																			);
																			
		}else if($value_emp_areatype == 'O'){
		
			$form['bocwa_reg_master']['emp_other'] 						= array(
																			  '#prefix' 				=> '<div id="bocwa_emp_other_replace" class="col-lg-4 col-md-6"><label class="input">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select Other area',
																			  '#type' 					=> 'textfield',
																			  '#options' 				=> custom_user_block_list($value_emp_subdivision,$value_emp_areatype),
																			  '#default_value' 			=> !empty($emp_areatype_code) ? $emp_areatype_code:'',
																			  '#required' 				=> TRUE,
																			  '#attributes' 			=> $readonly_emp_other,
																			  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																			  '#ajax'					=> array(
																												'event' 			=> 'change',
																												'effect' 			=> 'fade',			
																												'callback'			=> 'get_edit_bocwa_emp_ajax_callback',
																												'progress'			=> '',
																											  )
																			);
					 
		}
	}	
	
	$emp_code	=	"";
	$value_emp_block 	= 	isset($form_state['values']['emp_block']) ? $form_state['values']['emp_block'] : '';
	$value_emp_mun		= 	isset($form_state['values']['emp_mun']) ? $form_state['values']['emp_mun'] : '';
	$value_emp_corp 	= 	isset($form_state['values']['emp_corp']) ? $form_state['values']['emp_corp'] : '';
	$value_emp_sez 		= 	isset($form_state['values']['emp_sez']) ? $form_state['values']['emp_sez'] : '';
	$value_emp_not 		= 	isset($form_state['values']['emp_not']) ? $form_state['values']['emp_not'] : '';
	
	if(empty($value_emp_block)){
		if(empty($value_emp_mun)){
			if(empty($value_emp_corp)){
				if(empty($value_emp_sez)){
					if(empty($value_emp_not)){
						$emp_code	=	"";
					}else
						$emp_code = $value_emp_not;
				}else
					$emp_code = $value_emp_sez;
			}else
				$emp_code = $value_emp_corp;
		}else
			$emp_code = $value_emp_mun;
	}else
		$emp_code = $value_emp_block;
		
	if(empty($emp_code) && !empty($emp_areatype_code))
		$emp_code 	= 	$emp_areatype_code;
		
		$form['bocwa_reg_master']['emp_vill'] 							= array(
																				'#prefix' 				=> '<div id="bocwa_emp_vill_replace">',
																				'#suffix' 				=> '</div>'
																			);
						 
		$form['bocwa_reg_master']['emp_ward'] 							= array(
																				'#prefix' 				=> '<div id="bocwa_emp_ward_replace">',
																				'#suffix' 				=> '</div>'
																			 );
						 
		$form['bocwa_reg_master']['emp_bugh'] 							= array(
																				'#prefix' 				=> '<div id="bocwa_emp_bugh_replace">',
																				'#suffix' 				=> '</div>'
																			 );
																			 
		$form['bocwa_reg_master']['emp_sector'] 						= array(
																				'#prefix' 				=> '<div id="bocwa_emp_sector_replace">',
																				'#suffix' 				=> '</div>'
																			 );
																			 
		$form['bocwa_reg_master']['emp_not_area'] 						= array(
																				'#prefix' 				=> '<div id="bocwa_emp_not_area_replace">',
																				'#suffix' 				=> '</div>'
																			 );
						 
		$form['bocwa_reg_master']['emp_areatype_code'] 					= array(
																				'#prefix' 				=> '<div id="bocwa_emp_code_replace">',
																				'#suffix' 				=> '</div>'
																			);	
		
		if(!empty($emp_code)){

			$form['bocwa_reg_master']['emp_areatype_code'] 				= array(
																			'#prefix' 					=> '<div id="bocwa_emp_code_replace">',
																			'#suffix' 					=> '</div>',
																			'#type' 					=> 'hidden',
																			'#value'					=> !empty($emp_code) ? $emp_code	:''
																		  );


			if($value_emp_areatype == 'B'){
				
					$form['bocwa_reg_master']['emp_vill'] 				= array(
																			  '#prefix' 				=> '<div id="bocwa_emp_vill_replace" class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select Gram Panchayat',
																			  '#type' 					=> 'select',
																			  '#options' 				=> custom_user_villward_list($emp_code,'V'),// come from custom_user module
																			  '#default_value' 			=> !empty($emp_vill_ward) ? $emp_vill_ward	:'',
																			  '#required' 				=> TRUE,
																			  '#attributes' 			=> $readonly_emp_vill,
																			  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																			);
			}
	
			if($value_emp_areatype == 'M'){
				
					$form['bocwa_reg_master']['emp_ward'] 				= array(
																			  '#prefix' 				=> '<div id="bocwa_emp_ward_replace" class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select Ward',
																			  '#type' 					=> 'select',
																			  '#options' 				=> custom_user_villward_list($emp_code,'W'),// come from custom_user module
																			  '#default_value' 			=> !empty($emp_vill_ward) ? $emp_vill_ward	:'',
																			  '#required' 				=> TRUE,
																			  '#attributes' 			=> $readonly_emp_ward,
																			  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																			);
			}
	
			if($value_emp_areatype == 'C'){
				
					$form['bocwa_reg_master']['emp_bugh'] 				= array(
																			  '#prefix' 				=> '<div id="bocwa_emp_bugh_replace" class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select Ward',
																			  '#type' 					=> 'select',
																			  '#options' 				=> custom_user_villward_list($emp_code,'C'),// come from custom_user module
																			  '#default_value' 			=> !empty($emp_borough) ? $emp_borough	:'',
																			  '#required' 				=> TRUE,
																			  '#attributes' 			=> $readonly_emp_bugh,
																			  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																			);
			}
			
			if($value_emp_areatype == 'S'){
				
					$form['bocwa_reg_master']['emp_sector'] 				= array(
																			  '#prefix' 				=> '<div id="bocwa_emp_sector_replace" class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select Sector',
																			  '#type' 					=> 'select',
																			  '#options' 				=> common_custom_user_villward_list_new($emp_code,'S'),
																			  '#default_value' 			=> !empty($emp_vill_ward) ? $emp_vill_ward	:'',
																			  '#required' 				=> TRUE,
																			  '#attributes' 			=> $readonly_emp_sector,
																			  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																			);
			}
			
			if($value_emp_areatype == 'N'){
				
					$form['bocwa_reg_master']['emp_not_area'] 				= array(
																			  '#prefix' 				=> '<div id="bocwa_emp_not_area_replace" class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select Notified Area',
																			  '#type' 					=> 'select',
																			  '#options' 				=> common_custom_user_villward_list_new($emp_code,'N'),
																			  '#default_value' 			=> !empty($emp_vill_ward) ? $emp_vill_ward	:'',
																			  '#required' 				=> TRUE,
																			  '#attributes' 			=> $readonly_emp_not_area,
																			  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																			);
			}
		}
		 
		$form['bocwa_reg_master']['emp_ps'] 							= array(
																				'#prefix' 				=> '<div id="bocwa_emp_ps_replace">',
																				'#suffix' 				=> '</div>'
																		   );
	
						
		$form['bocwa_reg_master']['emp_ps'] 							= array(
																			  '#prefix' 				=> '<div id="bocwa_emp_ps_replace" class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select Police Station',
																			  '#type' 					=> 'select',
																			  '#options' 				=> custom_user_ps_list($value_emp_dist),// come from custom_user module
																			  '#default_value' 			=> !empty($emp_ps) ? $emp_ps	:'',
																			  '#required' 				=> TRUE,
																			  '#attributes' 			=> $readonly_emp_ps,
																			  '#element_validate'		=> array('BOCWA_POLICE_STATION_VALIDATE'),  
																			);
																			
		$form['bocwa_reg_master']['emp_pin_number'] 		   			= array(
																			  '#prefix' 				=> '<div class="col-lg-4 col-md-6"><label class="input">',
																			  '#suffix' 				=> '</label></div></div></div></div>',
																			  '#title'					=> 'PIN Code',
																			  '#type'					=> 'textfield',
																			  '#attributes' 			=> $readonly_emp_pin_number,
																			  '#default_value' 			=> !empty($emp_pin_number) ? $emp_pin_number	:'',
																			  '#required' 				=> TRUE
																				  );
																			
		$form['bocwa_reg_master']['full_name_manager'] 					= array(
																				//'#prefix' 				=> '<fieldset><legend class="form-title">2. Full name and address of the Manager or Person responsible for the supervision  and control of the Establishment</legend><div class="row"><section class="col-md-6"><label class="input">',
																				'#prefix' 				=> '<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">2. Full name and address of the Manager or Person responsible for the supervision  and control of the Establishment</div><div class="panel-body"><div class="col-lg-4 col-md-6"><label class="input">',
																				'#suffix' 				=> '</label></div>',
																				'#title'				=> 'Full name of the Manager or Person Responsible',
																				'#type' 				=> 'textfield',
																				'#default_value' 		=> !empty($full_name_manager) ? $full_name_manager : '',
																				'#required' 			=> TRUE,
																				'#attributes' 			=> $readonly_full_name_manager,
																				'#element_validate'	=> array('BOCWA_TEXTBOX_TEXT_VALIDATE')
																				);
	
	/*================== ADDRESS OF THE MANAGER ==============*/
									
		$form['bocwa_reg_master']['address_manager'] 					= array(
																				'#prefix' 				=> '<div class="col-lg-4 col-md-6"><label class="input">',
																				'#suffix' 				=> '</label></div>',
																				'#title'				=> 'House No./Village Name/Street/Road',
																				'#type' 				=> 'textfield',
																				'#default_value' 		=> !empty($address_manager) ? $address_manager : '',
																				'#required' 			=> TRUE,
																				'#attributes' 			=> $readonly_address_manager,
																				'#element_validate' 	=> array('BOCWA_SPECIAL_CHAR_TEXTAREA_VALIDATE')
																			);
							
		$form['bocwa_reg_master']['address_manager_dist'] 	   			= array(
																			  '#prefix' 				=> '<div class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select District',
																			  '#type' 					=> 'select',
																			  '#options' 				=> $options_dist,
																			  '#default_value' 			=> !empty($address_manager_dist) ? $address_manager_dist	:'',
																			  '#required' 				=> TRUE,
																			  '#attributes' 			=> $readonly_address_manager_dist,
																			  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																			  '#ajax'					=> array(
																											'event' 			=> 'change',
																											'effect' 			=> 'fade',			
																											'callback'			=> 'get_bocwa_address_manager_ajax_callback',
																											'progress'			=> '',
																										  )
																			);
								
	$value_address_manager_dist 	= 	isset($form_state['values']['address_manager_dist']) ? $form_state['values']['address_manager_dist'] : '';
	
	if(empty($value_address_manager_dist) &&!empty($address_manager_dist))
			$value_address_manager_dist	=	$address_manager_dist;


	$form['bocwa_reg_master']['address_manager_subdivision'] 		  		= array(
																				'#prefix' 				=> '<div id="bocwa_address_manager_subdivision_replace">',
																				'#suffix' 				=> '</div>',
																			);

	$form['bocwa_reg_master']['address_manager_subdivision'] 		  		= array(
																				  '#prefix' 			=> '<div id="bocwa_address_manager_subdvsn_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Subdivision',
																				  '#type' 				=> 'select',
																				  '#options' 			=> custom_user_subdivisionlist_list($value_address_manager_dist),// ---------come from custom_user module
																				  '#default_value' 		=> !empty($address_manager_subdivision) ? $address_manager_subdivision	:'',
																				  '#required' 			=> TRUE,
																				  '#attributes' 		=> $readonly_address_manager_subdivision,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																			);
																			
	
		$form['bocwa_reg_master']['address_manager_areatype']   		 	 = array(
																			  '#prefix' 				=> '<div class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select Area type',
																			  '#type' 					=> 'select',
																			  '#options' 				=> array('T'=>'Select Area Type','B' =>'Block','M' => 'Municipality','C' => 'Corporation', 'S' =>'SEZ','N' =>'Notified Area'),
																			  '#element_validate'		=> array('BOCWA_LOCATION_AREATYPE_VALIDATE'),
																			  '#default_value' 			=> !empty($address_manager_areatype) ? $address_manager_areatype	:'',
																			  '#required' 				=> TRUE,
																			  '#attributes' 			=> $readonly_address_manager_areatype,
																			  '#ajax'					=> array(
																												'event' 			=> 'change',
																												'effect' 			=> 'fade',			
																												'callback'			=> 'get_bocwa_address_manager_ajax_callback',
																												'progress'			=> '',
																											  ),
																				);
		
		
		$form['bocwa_reg_master']['address_manager_block'] 					= array(
																				'#prefix' 				=> '<div id="bocwa_address_manager_block_replace">',
																				'#suffix' 				=> '</div>'
																		  );
						  
		$form['bocwa_reg_master']['address_manager_mun'] 					= array(
																				'#prefix' 				=> '<div id="bocwa_address_manager_mun_replace">',
																				'#suffix' 				=> '</div>'
																	  );
				  
		$form['bocwa_reg_master']['address_manager_corp'] 					= array(
																				'#prefix' 				=> '<div id="bocwa_address_manager_corp_replace">',
																				'#suffix' 				=> '</div>'
																		  );
				  
		$form['bocwa_reg_master']['address_manager_sez'] 					= array(
																				'#prefix' 				=> '<div id="bocwa_address_manager_sez_replace">',
																				'#suffix' 				=> '</div>'
																			  );
				  
		$form['bocwa_reg_master']['address_manager_not'] 					= array(
																				'#prefix' 				=> '<div id="bocwa_address_manager_not_replace">',
																				'#suffix' 				=> '</div>'
																				);	
				
														  
		$form['bocwa_reg_master']['address_manager_other'] 					= array(
																				'#prefix' 				=> '<div id="bocwa_address_manager_other_replace">',
																				'#suffix' 				=> '</div>'
																			  );
																			  
		$value_address_manager_subdivision 		= isset($form_state['values']['address_manager_subdivision']) ? $form_state['values']['address_manager_subdivision'] : '';
		$value_address_manager_areatype 		= isset($form_state['values']['address_manager_areatype']) ? $form_state['values']['address_manager_areatype'] : '';
		
		
		if(empty($value_address_manager_subdivision) && !empty($emp_subdivision))
			$value_address_manager_subdivision = $address_manager_subdivision;
	
		
		if(empty($value_address_manager_areatype) && !empty($emp_areatype))
			$value_address_manager_areatype	=	$address_manager_areatype;
	
	
		if(!empty($value_address_manager_areatype)){
			
			if($value_address_manager_areatype == 'B'){
			
			$form['bocwa_reg_master']['address_manager_block'] 			= array(
																			  '#prefix' 				=> '<div id="bocwa_address_manager_block_replace" class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select Block',
																			  '#type' 					=> 'select',
																			  '#options' 				=> custom_user_block_list($value_address_manager_subdivision,$value_address_manager_areatype),
																			  '#default_value' 			=> !empty($address_manager_areatype_code) ? $address_manager_areatype_code:'',
																			  '#required' 				=> TRUE,
																			  '#attributes' 			=> $readonly_address_manager_block,
																			  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																			  '#ajax'					=> array(
																												'event' 			=> 'change',
																												'effect' 			=> 'fade',			
																												'callback'			=> 'get_bocwa_address_manager_ajax_callback',
																												'progress'			=> '',
																											  )
																		 );
					 
		}else if($value_address_manager_areatype == 'M'){
			$form['bocwa_reg_master']['address_manager_mun'] 			= array(
																			  '#prefix' 				=> '<div id="bocwa_address_manager_mun_replace" class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select Municipality ',
																			  '#type' 					=> 'select',
																			  '#options' 				=> custom_user_block_list($value_address_manager_subdivision,$value_address_manager_areatype),
																			  '#default_value' 			=> !empty($address_manager_areatype_code) ? $address_manager_areatype_code	:'',
																			  '#required' 				=> TRUE,
																			  '#attributes' 			=> $readonly_address_manager_mun,
																			  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																			  '#ajax'					=> array(
																												'event' 			=> 'change',
																												'effect' 			=> 'fade',			
																												'callback'			=> 'get_bocwa_address_manager_ajax_callback',
																												'progress'			=> '',
																											  )
																			);
																			
		}else if($value_address_manager_areatype == 'C'){
			
			$form['bocwa_reg_master']['address_manager_corp'] 			= array(
																			  '#prefix' 				=> '<div id="bocwa_address_manager_corp_replace" class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select Corporation',
																			  '#type' 					=> 'select',
																			  '#options' 				=> custom_user_block_list($value_address_manager_subdivision,$value_address_manager_areatype),// come from custom_user module
																			  '#default_value' 			=> !empty($address_manager_areatype_code) ? $address_manager_areatype_code:'',
																			  '#required' 				=> TRUE,
																			  '#attributes' 			=> $readonly_address_manager_corp,
																			  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																			  '#ajax'					=> array(
																												'event' 			=> 'change',
																												'effect' 			=> 'fade',			
																												'callback'			=> 'get_bocwa_address_manager_ajax_callback',
																												'progress'			=> '',
																											  )		
																		);
																		
		}else if($value_address_manager_areatype == 'S'){
		
			$form['bocwa_reg_master']['address_manager_sez'] 			= array(
																			  '#prefix' 				=> '<div id="bocwa_address_manager_sez_replace" class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select SEZ',
																			  '#type' 					=> 'select',
																			  '#options' 				=> custom_user_block_list($value_address_manager_subdivision,$value_address_manager_areatype),// come from custom_user module
																			  '#default_value' 			=> !empty($address_manager_areatype_code) ? $address_manager_areatype_code	:'',
																			  '#required' 				=> TRUE,
																			  '#attributes' 			=> $readonly_address_manager_sez,
																			  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																			  '#ajax'					=> array(
																												'event' 			=> 'change',
																												'effect' 			=> 'fade',			
																												'callback'			=> 'get_bocwa_address_manager_ajax_callback',
																												'progress'			=> '',
																											  )
																			);
																			
		}else if($value_address_manager_areatype == 'N'){

			$form['bocwa_reg_master']['address_manager_not'] 			= array(
																			  '#prefix' 				=> '<div id="bocwa_address_manager_not_replace" class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select Notified area',
																			  '#type' 					=> 'select',
																			  '#options' 				=> custom_user_block_list($value_address_manager_subdivision,$value_address_manager_areatype),
																			  '#default_value' 			=> !empty($address_manager_areatype_code) ? $address_manager_areatype_code	:'',
																			  '#required' 				=> TRUE,
																			  '#attributes' 			=> $readonly_address_manager_not,
																			  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																			  '#ajax'					=> array(
																												'event' 			=> 'change',
																												'effect' 			=> 'fade',			
																												'callback'			=> 'get_bocwa_address_manager_ajax_callback',
																												'progress'			=> '',
																											  )
																			);
																			
		}/*else if($value_address_manager_areatype == 'O'){
		
			$form['bocwa_reg_master']['address_manager_other'] 			= array(
																			  #prefix' 				=> '<div id="bocwa_address_manager_other_replace"><section class="col col-6"><label class="input">',
																			  #suffix' 				=> '</label></section></div>',
																			  #title'					=> 'Select Other area ',
																			  #type' 					=> 'textfield',
																			  #default_value' 			=> !empty($address_manager_areatype_code) ? $address_manager_areatype_code : '',
																			  #required' 				=> TRUE,
																			  #attributes' 			=> $readonly_address_manager_other,
																			  #element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																			  #ajax'					=> array(
																												event' 			=> 'change',
																												effect' 			=> 'fade',			
																												callback'			=> 'get_bocwa_address_manager_ajax_callback',
																												progress'			=> '',
																											  )
																			);
					 
		}*/
	}
	$address_manager_code			=	"";
	$value_address_manager_block 	= 	isset($form_state['values']['address_manager_block']) ? $form_state['values']['address_manager_block'] : '';
	$value_address_manager_mun		= 	isset($form_state['values']['address_manager_mun']) ? $form_state['values']['address_manager_mun'] : '';
	$value_address_manager_corp 	= 	isset($form_state['values']['address_manager_corp']) ? $form_state['values']['address_manager_corp'] : '';
	$value_address_manager_sez 		= 	isset($form_state['values']['address_manager_sez']) ? $form_state['values']['address_manager_sez'] : '';
	$value_address_manager_not 		= 	isset($form_state['values']['address_manager_not']) ? $form_state['values']['address_manager_not'] : '';
	
	if(empty($value_address_manager_block)){
		if(empty($value_address_manager_mun)){
			if(empty($value_address_manager_corp)){
				if(empty($value_address_manager_sez)){
					if(empty($value_address_manager_not)){

						$address_manager_code	=	"";
					}else
						$address_manager_code = $value_address_manager_not;
				}else
					$address_manager_code = $value_address_manager_sez;
			}else
				$address_manager_code = $value_address_manager_corp;
		}else
			$address_manager_code = $value_address_manager_mun;
	}else
		$address_manager_code = $value_address_manager_block;
		
		
	if(empty($address_manager_code) && !empty($address_manager_areatype_code))
		$address_manager_code 	= 	$address_manager_areatype_code;

		$form['bocwa_reg_master']['address_manager_vill'] 				= array(
																				'#prefix' 				=> '<div id="bocwa_address_manager_vill_replace">',
																				'#suffix' 				=> '</div>'
																		 );
						 
		$form['bocwa_reg_master']['address_manager_ward'] 				= array(
																				'#prefix' 				=> '<div id="bocwa_address_manager_ward_replace">',
																				'#suffix' 				=> '</div>'
																		 );
						 
		$form['bocwa_reg_master']['address_manager_bugh'] 				= array(
																				'#prefix' 				=> '<div id="bocwa_address_manager_bugh_replace">',
																				'#suffix' 				=> '</div>'
																		 );
		$form['bocwa_reg_master']['address_manager_sector'] 			= array(
																				'#prefix' 				=> '<div id="bocwa_address_manager_sector_replace">',
																				'#suffix' 				=> '</div>'
																		 );																 
		$form['bocwa_reg_master']['address_manager_notifiedarea'] 			= array(
																				'#prefix' 				=> '<div id="bocwa_address_manager_notifiedarea_replace">',
																				'#suffix' 				=> '</div>'
																		 );						 
								 
								 
		$form['bocwa_reg_master']['address_manager_areatype_code'] 		= array(
																				'#prefix' 				=> '<div id="bocwa_address_manager_code_replace">',
																				'#suffix' 				=> '</div>'
																		);	
				 
				 
		if(!empty($address_manager_code)){

			$form['bocwa_reg_master']['address_manager_areatype_code'] = array(
																			'#prefix' 					=> '<div id="bocwa_address_manager_code_replace">',
																			'#suffix' 					=> '</div>',
																			'#type' 					=> 'hidden',
																			'#value'					=> !empty($address_manager_code) ? $address_manager_code	:''
																	  );


			if($value_address_manager_areatype == 'B'){
				
					$form['bocwa_reg_master']['address_manager_vill'] 	= array(
																				  '#prefix' 				=> '<div id="bocwa_address_manager_vill_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 				=> '</label></div>',
																				  '#title'					=> 'Select Gram Panchayat',
																				  '#type' 					=> 'select',
																				  '#options' 				=> custom_user_villward_list($address_manager_code,'V'),// come from custom_user module
																				  '#default_value' 			=> !empty($address_manager_vill_ward) ? $address_manager_vill_ward	:'',
																				  '#required' 				=> TRUE,
																				  '#attributes' 			=> $readonly_address_manager_vill,
																				  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				);
			}
	
			if($value_address_manager_areatype == 'M'){
				
					$form['bocwa_reg_master']['address_manager_ward'] 		= array(
																				  '#prefix' 				=> '<div id="bocwa_address_manager_ward_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 				=> '</label></div>',
																				  '#title'					=> 'Select Ward',
																				  '#type' 					=> 'select',
																				  '#options' 				=> custom_user_villward_list($address_manager_code,'W'),
																				  '#default_value' 			=> !empty($address_manager_vill_ward) ? $address_manager_vill_ward	:'',
																				  '#required' 				=> TRUE,
																				  '#attributes' 			=> $readonly_address_manager_ward,
																				  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				);
			}
	
			if($value_address_manager_areatype == 'C'){
				
					$form['bocwa_reg_master']['address_manager_bugh'] 		= array(
																				  '#prefix' 				=> '<div id="bocwa_address_manager_bugh_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 				=> '</label></div>',
																				  '#title'					=> 'Select Ward ',
																				  '#type' 					=> 'select',
																				  '#options' 				=> custom_user_villward_list($address_manager_code,'C'),
																				  '#default_value' 			=> !empty($address_manager_borough) ? $address_manager_borough	:'',
																				  '#required' 				=> TRUE,
																				  '#attributes' 			=> $readonly_address_manager_bugh,
																				  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				);
			}
			
			if($value_address_manager_areatype == 'S'){
				
					$form['bocwa_reg_master']['address_manager_sector'] 	= array(
																				  '#prefix' 				=> '<div id="bocwa_address_manager_sector_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 				=> '</label></div>',
																				  '#title'					=> 'Select Sector ',
																				  '#type' 					=> 'select',
																				  '#options' 				=> custom_user_villward_list($address_manager_code,'S'),
																				  '#default_value' 			=> !empty($address_manager_borough) ? $address_manager_borough	:'',
																				  '#required' 				=> TRUE,
																				  '#attributes' 			=> $readonly_address_manager_bugh,
																				  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				);
			}
			if($value_address_manager_areatype == 'N'){
				
					$form['bocwa_reg_master']['address_manager_notifiedarea'] 	= array(
																				  '#prefix' 				=> '<div id="bocwa_address_manager_notifiedarea_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 				=> '</label></div>',
																				  '#title'					=> 'Select Notified Area ',
																				  '#type' 					=> 'select',
																				  '#options' 				=> custom_user_villward_list($address_manager_code,'N'),
																				  '#default_value' 			=> !empty($address_manager_borough) ? $address_manager_borough	:'',
																				  '#required' 				=> TRUE,
																				  '#attributes' 			=> $readonly_address_manager_bugh,
																				  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				);
			}		
			
		}
		
		$form['bocwa_reg_master']['address_manager_ps'] 						= array(
																					'#prefix' 				=> '<div id="bocwa_address_manager_ps_replace">',
																					'#suffix' 				=> '</div>'
																			   	);
	
						
		$form['bocwa_reg_master']['address_manager_ps'] 						= array(
																				  '#prefix' 				=> '<div id="bocwa_address_manager_ps_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 				=> '</label></div>',
																				  '#title'					=> 'Select Police Station',
																				  '#type' 					=> 'select',
																				  '#options' 				=> custom_user_ps_list($value_address_manager_dist),// come from custom_user module
																				  '#default_value' 			=> !empty($address_manager_ps) ? $address_manager_ps	:'',
																				  '#required' 				=> TRUE,
																				  '#attributes' 			=> $readonly_address_manager_ps,
																				  '#element_validate'		=> array('BOCWA_POLICE_STATION_VALIDATE'),  
																				);
		
		
																				
		
		
		$form['bocwa_reg_master']['address_manager_pin_number'] 		   = array(
																				  '#prefix' 				=> '<div class="col-lg-4 col-md-6"><label class="input">',
																				  '#suffix' 				=> '</label></div></div></div></div>',
																				  '#title'					=> 'PIN Code',
																				  '#type'					=> 'textfield',
																				  '#attributes' 			=> $readonly_address_manager_pin_number,
																				  '#default_value' 			=> !empty($address_manager_pin_number) ? $address_manager_pin_number	:'',
																				  '#required' 				=> TRUE
																					  );	
		
		
																			
		$form['bocwa_reg_master']['e_name'] 								= array(
																				  //'#prefix' 			=>  '</fieldset><fieldset><legend class="form-title">3. Name and location (Work Site) of the Establishment</legend><div class="row"><section class="col-lg-4 col-md-6"><label class="input">',
																				  '#prefix' 			=>  '<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">3. Name and location (Work Site) of the Establishment</div><div class="panel-body"><div class="col-lg-4 col-md-6"><label class="input">',
																				  '#suffix' 			=>  '</label></div>',
																				  '#title'				=>  'Name of the Establishment',
																				  '#type' 				=>  'textfield',
																				  '#default_value' 		=>  !empty($e_name) ? $e_name : '',
																				  '#required' 			=>  TRUE,
																				  '#attributes'			=>  $readonly_e_name,
																				  '#element_validate' 	=>  array('BOCWA_SPECIAL_CHAR_TEXT_VALIDATE')
																				);
		$form['bocwa_reg_master']['est_type'] 								= array(
																					'#title' 			=> 'Establishment Type',
																					'#type' 			=> 'select',
																					'#options' 			=> array('micro' => 'Micro', 'small' => 'Small', 'medium' => 'Medium', 'large' => 'Large'),
																					'#required' 		=>  TRUE,
																					'#default_value' 	=> $est_type,
																					'#attributes'		=> $readonly_est_type,
																					'#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="select">',
																					'#suffix' 			=> '</label></div>'
																				);
						
		$form['bocwa_reg_master']['loc_e_name'] 							= array(
																				  '#prefix' 			=>  '<div class="col-lg-4 col-md-6"><label class="input">',
																				  '#suffix' 			=>  '</label></div>',
																				  '#title'				=>  'House No./Village Name/Street/Road',
																				  '#type' 				=>  'textfield',
																				  '#default_value' 		=>  !empty($loc_e_name) ? $loc_e_name : '',
																				  '#required' 			=>  TRUE,
																				  '#attributes'			=>  $readonly_loc_e_name,
																				  '#element_validate'	=> array('BOCWA_SPECIAL_CHAR_TEXTAREA_VALIDATE')
																				);

		
	
		$form['bocwa_reg_master']['loc_e_dist'] 							= array(
																				  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select District',
																				  '#type' 				=> 'select',
																				  '#options' 			=> $options_dist,
																				  '#required' 			=> TRUE,
																				  '#attributes'			=>  $readonly_loc_e_dist,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				  '#default_value' 		=> !empty($loc_e_dist) ? $loc_e_dist	:'',
																				  '#ajax'				=> array(
																											'event' 			=> 'change',
																											'effect' 			=> 'fade',			
																											'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																											'progress'			=> '',
																										  )
																				);
		
		$value_loc_e_dist 						= 	isset($form_state['values']['loc_e_dist']) ? $form_state['values']['loc_e_dist'] : '';
	
		if(empty($value_loc_e_dist) &&!empty($loc_e_dist))
			$value_loc_e_dist	=	$loc_e_dist;
	
		$form['bocwa_reg_master']['loc_e_subdivision'] 						= array(
																				  '#prefix' 			=> '<div id="bocwa_subdivision_replace">',
																				  '#suffix' 			=> '</div>', 
																				);
	
		$form['bocwa_reg_master']['loc_e_subdivision'] 						= array(
																				  '#prefix' 			=> '<div id="bocwa_subdvsn_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Subdivision',
																				  '#type' 				=> 'select',
																				  '#options' 			=> custom_user_subdivisionlist_list($value_loc_e_dist),
																				  '#default_value' 		=> !empty($loc_e_subdivision) ? $loc_e_subdivision	:'',
																				  '#required' 			=> TRUE,
																				  '#attributes'			=>  $readonly_loc_e_subdivision,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE')
																				);
						
		$form['bocwa_reg_master']['loc_e_areatype'] 							= array(
																				  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Area type',
																				  '#type' 				=> 'select',
																				  '#options' 			=> array('T'=>'Select Area Type','B' =>'Block','M' => 'Municipality','C' => 'Corporation', 'S' =>'SEZ','N' =>'Notified Area'),
																				  '#default_value' 		=> !empty($loc_e_areatype) ? $loc_e_areatype : '',
																				  '#required' 			=> TRUE,
																				  '#attributes'			=>  $readonly_loc_e_areatype,
																				  '#element_validate'	=> array('BOCWA_LOCATION_AREATYPE_VALIDATE'),
																				  '#ajax'				=> array(
																											'event' 			=> 'change',
																											'effect' 			=> 'fade',			
																											'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																											'progress'			=> '',
																										  ),
																				);
																					
		$form['bocwa_reg_master']['loc_e_block'] 								= array(
																					  '#prefix' 		=> '<div id="bocwa_block_replace">',
																					  '#suffix' 		=> '</div>'
																					);
		
		$form['bocwa_reg_master']['loc_e_mun'] 									= array(
																					  '#prefix' 		=> '<div id="bocwa_mun_replace">',
																					  '#suffix' 		=> '</div>'
																					);
		
		$form['bocwa_reg_master']['loc_e_other'] 								= array(
																					  '#prefix' 		=> '<div id="bocwa_other_replace">',
																					  '#suffix' 		=> '</div>'
																					);
		
		$form['bocwa_reg_master']['loc_e_corp'] 								= array(
																					  '#prefix' 		=> '<div id="bocwa_corp_replace">',
																					  '#suffix' 		=> '</div>'
																					);
		
		$form['bocwa_reg_master']['loc_e_sez'] 									= array(
																					'#prefix' 			=> '<div id="bocwa_sez_replace">',
																					'#suffix' 			=> '</div>'
																					);
		
		$form['bocwa_reg_master']['loc_e_not'] 									= array(
																					'#prefix' 			=> '<div id="bocwa_not_replace">',
																					'#suffix' 			=> '</div>'
																					);
																					
		$value_loc_e_areatype 					= isset($form_state['values']['loc_e_areatype']) ? $form_state['values']['loc_e_areatype'] : '';
		$value_loc_e_subdivision 				= isset($form_state['values']['loc_e_subdivision']) ? $form_state['values']['loc_e_subdivision'] : '';
	
		if(empty($value_loc_e_areatype) && !empty($loc_e_areatype))
		$value_loc_e_areatype		=	$loc_e_areatype;
	
		if(empty($value_loc_e_subdivision) && !empty($loc_e_subdivision))
		$value_loc_e_subdivision	=	$loc_e_subdivision;
	
		if(!empty($value_loc_e_areatype)){
			if($value_loc_e_areatype == 'B'){
				
				$form['bocwa_reg_master']['loc_e_block'] 						= array(
																				  '#prefix' 			=> '<div id="bocwa_block_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Block',
																				  '#type' 				=> 'select',
																				  '#options' 			=> custom_user_block_list($value_loc_e_subdivision,$value_loc_e_areatype),
																				  '#default_value' 		=> !empty($loc_e_areatype_code) ? $loc_e_areatype_code	:'',
																				  '#attributes'			=>  $readonly_loc_e_block,
																				  '#required' 			=> TRUE,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				  '#ajax'				=> array(
																											'event' 			=> 'change',
																											'effect' 			=> 'fade',			
																											'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																											'progress'			=> '',
																										  )
																				);
			}else if($value_loc_e_areatype == 'M'){
			
				$form['bocwa_reg_master']['loc_e_mun'] 							= array(
																				  '#prefix' 			=> '<div id="bocwa_mun_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Municipality',
																				  '#type' 				=> 'select',
																				  '#options' 			=> custom_user_block_list($value_loc_e_subdivision,$value_loc_e_areatype),// come from custom_user module
																				  '#default_value' 		=> !empty($loc_e_areatype_code) ? $loc_e_areatype_code	:'',
																				  '#required' 			=> TRUE,
																				  '#attributes'			=>  $readonly_loc_e_mun,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				  '#ajax'				=> array(
																											'event' 			=> 'change',
																											'effect' 			=> 'fade',			
																											'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																											'progress'			=> '',
																										  )
																				);
			}else if($value_loc_e_areatype == 'C'){
	
				$form['bocwa_reg_master']['loc_e_corp'] 						= array(
																				  '#prefix' 			=> '<div id="bocwa_corp_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Corporation',
																				  '#type' 				=> 'select',
																				  '#options' 			=> custom_user_block_list($value_loc_e_subdivision,$value_loc_e_areatype),// come from custom_user module
																				  '#default_value' 		=> !empty($loc_e_areatype_code) ? $loc_e_areatype_code	:'',
																				  '#required' 			=> TRUE,
																				  '#attributes'			=>  $readonly_loc_e_corp,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				  '#ajax'				=> array(
																											'event' 			=> 'change',
																											'effect' 			=> 'fade',			
																											'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																											'progress'			=> '',
																										  )
																			);
			}else if($value_loc_e_areatype == 'S'){
	
				$form['bocwa_reg_master']['loc_e_sez'] 						= array(
																				  '#prefix' 			=> '<div id="bocwa_sez_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select SEZ',
																				  '#type' 				=> 'select',
																				  '#options' 			=> custom_user_block_list($value_loc_e_subdivision,$value_loc_e_areatype),// come from custom_user module
																				  '#default_value' 		=> !empty($loc_e_areatype_code) ? $loc_e_areatype_code	:'',
																				  '#required' 			=> TRUE,
																				  '#attributes'			=>  $readonly_loc_e_sez,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				  '#ajax'				=> array(
																											'event' 			=> 'change',
																											'effect' 			=> 'fade',			
																											'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																											'progress'			=> '',
																										  )
																				);
			}else if($value_loc_e_areatype == 'N'){
			
				$form['bocwa_reg_master']['loc_e_not'] 						= array(
																				  '#prefix' 			=> '<div id="bocwa_not_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Notified area',
																				  '#type' 				=> 'select',
																				  '#options' 			=> custom_user_block_list($value_loc_e_subdivision,$value_loc_e_areatype),// come from custom_user module
																				  '#default_value' 		=> !empty($loc_e_areatype_code) ? $loc_e_areatype_code	:'',
																				  '#required' 			=> TRUE,
																				  '#attributes'			=>  $readonly_loc_e_not,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				  '#ajax'				=> array(
																											'event' 			=> 'change',
																											'effect' 			=> 'fade',			
																											'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																											'progress'			=> '',
																										  )
																				);
			}else if($value_loc_e_areatype == 'O'){
	
				$form['bocwa_reg_master']['loc_e_other'] 					= array(
																				  '#prefix' 			=> '<div id="bocwa_other_replace" class="col-lg-4 col-md-6"><label class="input">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Other area',
																				  '#type' 				=> 'textfield',
																				  '#default_value' 		=> !empty($loc_e_areatype_code) ? $loc_e_areatype_code : '',
																				  '#required' 			=> TRUE,
																				  '#attributes'			=>  $readonly_loc_e_other,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				  '#ajax'				=> array(
																												'event' 			=> 'change',
																												'effect' 			=> 'fade',			
																												'callback'			=> 'get_bocwa_registration_info_form_ajax_callback',
																												'progress'			=> '',
																											  )
																				);
			}
		}
	
		$code	=	"";
		$value_loc_e_block 		= 	isset($form_state['values']['loc_e_block']) ? $form_state['values']['loc_e_block'] : '';
		$value_loc_e_mun		= 	isset($form_state['values']['loc_e_mun']) ? $form_state['values']['loc_e_mun'] : '';
		$value_loc_e_corp 		= 	isset($form_state['values']['loc_e_corp']) ? $form_state['values']['loc_e_corp'] : '';
		$value_loc_e_sez 		= 	isset($form_state['values']['loc_e_sez']) ? $form_state['values']['loc_e_sez'] : '';
		$value_loc_e_not 		= 	isset($form_state['values']['loc_e_not']) ? $form_state['values']['loc_e_not'] : '';
		
		if(empty($value_loc_e_block)){
			if(empty($value_loc_e_mun)){
				if(empty($value_loc_e_corp)){
					if(empty($value_loc_e_sez)){
						if(empty($value_loc_e_not)){
							$code	=	"";
						}else
							$code	=	$value_loc_e_not;
					}else
						$code	=	$value_loc_e_sez;
				}else
					$code	=	$value_loc_e_corp;
			}else
				$code	=	$value_loc_e_mun;
		}else
			$code	=	$value_loc_e_block;
	
		if(empty($code) && !empty($loc_e_areatype_code))
			$code 		= 	$loc_e_areatype_code;
	
		$form['bocwa_reg_master']['loc_e_vill'] 							= array(
																					'#prefix' 			=> '<div id="bocwa_vill_replace">',
																					'#suffix' 			=> '</div>'
																				);
					 
		$form['bocwa_reg_master']['loc_e_ward'] 							= array(
																					'#prefix' 			=> '<div id="bocwa_ward_replace">',
																					'#suffix' 			=> '</div>'
																				 );				
						 
		$form['bocwa_reg_master']['loc_e_bugh'] 							= array(
																					'#prefix' 			=> '<div id="bocwa_bugh_replace">',
																					'#suffix' 			=> '</div>'
																				 );
						 
		$form['bocwa_reg_master']['loc_e_areatype_code'] 					= array(
																					'#prefix' 			=> '<div id="bocwa_code_replace">',
																					'#suffix' 			=> '</div>'
																					 );
																				 
		if(!empty($code)){
	
			$form['bocwa_reg_master']['loc_e_areatype_code'] 				= array(
																					'#prefix' 			=> '<div id="bocwa_code_replace">',
																					'#suffix' 			=> '</div>',
																					'#type' 			=> 'hidden',
																					'#value'			=> !empty($code) ? $code	:''
																				  );
	
	
				if($value_loc_e_areatype == 'B'){
					
					$form['bocwa_reg_master']['loc_e_vill'] 				= array(
																				  '#prefix' 			=> '<div id="bocwa_vill_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Gram Panchayat',
																				  '#type' 				=> 'select',
																				  '#options'			=> custom_user_villward_list($code,'V'),// come from custom_user module
																				  '#default_value' 		=> !empty($loc_e_vill_ward) ? $loc_e_vill_ward	:'',
																				  '#required' 			=> TRUE,
																				  '#attributes'			=>  $readonly_loc_e_vill,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				);	
				}
	
				if($value_loc_e_areatype == 'M'){
					
					$form['bocwa_reg_master']['loc_e_ward'] 				= array(
																				  '#prefix' 			=> '<div id="bocwa_ward_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Ward',
																				  '#type' 				=> 'select',
																				  '#options' 			=> custom_user_villward_list($code,'W'),// come from custom_user module
																				  '#default_value' 		=> !empty($loc_e_vill_ward) ? $loc_e_vill_ward	:'',
																				  '#required' 			=> TRUE,
																				  '#attributes'			=>  $readonly_loc_e_ward,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				);
																					
				}
	
				if($value_loc_e_areatype == 'C'){
					
					$form['bocwa_reg_master']['loc_e_ward'] 				= array(
																				  '#prefix' 			=> '<div id="bocwa_bugh_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Ward',
																				  '#type' 				=> 'select',
																				  '#options' 			=> custom_user_villward_list($code,'C'),// come from custom_user module
																				  '#default_value' 		=> !empty($loc_e_vill_ward) ? $loc_e_vill_ward	:'',
																				  '#attributes'			=>  $readonly_loc_e_bugh,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				  '#required' 			=> TRUE,
																				);
				}
				
				/*if($value_loc_e_areatype == 'N'){
					
					$form['bocwa_reg_master']['loc_e_bugh'] 				= array(
																				  '#prefix' 			=> '<div id="bocwa_bugh_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Ward',
																				  '#type' 				=> 'select',
																				  '#options' 			=> custom_user_villward_list($code,'N'),// come from custom_user module
																				  '#default_value' 		=> !empty($loc_e_borough) ? $loc_e_borough	:'',
																				  '#attributes'			=>  $readonly_loc_e_bugh,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				  '#required' 			=> TRUE,
																				);
				}*/
		}
	
		$form['bocwa_reg_master']['loc_e_ps'] 								= array(
																					'#prefix' 			=> '<div id="bocwa_ps_replace">',
																					'#suffix' 			=> '</div>'
																			   );
			   
		$form['bocwa_reg_master']['loc_e_ps'] 								= array(
																				  '#prefix' 			=> '<div id="bocwa_ps_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Police Station',
																				  '#type' 				=> 'select',
																				  '#options' 			=> custom_user_ps_list($value_loc_e_dist),// come from custom_user module
																				  '#default_value' 		=> !empty($loc_e_ps) ? $loc_e_ps	:'',
																				  '#required' 			=> TRUE,
																				  '#attributes'			=>  $readonly_loc_e_ps,
																				  '#element_validate'	=> array('BOCWA_POLICE_STATION_VALIDATE')  
																				);
																				
		$form['bocwa_reg_master']['loc_e_pin_number'] 						= array(
																				  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																				  '#suffix' 			=> '</label></div></div></div></div>',
																				  '#title'				=> 'PIN Code',
																				  '#type' 				=> 'textfield',
																				  '#default_value' 	 	=> !empty($loc_e_pin_number) ? $loc_e_pin_number	:'',
																				   '#attributes'		=>  $readonly_loc_e_pin_number,
																				  '#required' 			=> TRUE
																				  );
																					
		/*================== POSTAL ADDRESS OF THE ESTABLISHMENT ==============*/
			
		$form['bocwa_reg_master']['e_postal_address'] 						= array(
																				  //'#prefix' 			=> '<fieldset><legend class="form-title">4. Registered Office address of the Establishment</legend><section><label class="textarea">',
																				  '#prefix' 			=> '<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">4. Registered Office address of the Establishment</div><div class="panel-body"><div class="col-lg-4 col-md-6"><label class="textarea">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'House No./Village Name/Street/Road',
																				  '#rows'				=> t('5'),	
																				  '#type' 				=> 'textarea',
																				  '#attributes'			=>  $readonly_e_postal_address,
																				  '#default_value' 		=> !empty($e_postal_address) ? $e_postal_address : '',
																				  '#required' 			=> TRUE,
																				  '#element_validate'	=> array('BOCWA_SPECIAL_CHAR_TEXTAREA_VALIDATE')  
																				);
							
		$form['bocwa_reg_master']['e_postal_dist'] 	   						= array(
																				  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select District',
																				  '#type' 				=> 'select',
																				  '#options' 			=> $options_dist,
																				  '#attributes'			=>  $readonly_e_postal_dist,
																				  '#default_value' 		=> !empty($e_postal_dist) ? $e_postal_dist	:'',
																				  '#required' 			=> TRUE,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'), 
																				  '#ajax'				=> array(
																												'event' 			=> 'change',
																												'effect' 			=> 'fade',			
																												'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																												'progress'			=> '',
																											  )
																				);
									
		$value_e_postal_dist 		= 	isset($form_state['values']['e_postal_dist']) ? $form_state['values']['e_postal_dist'] : '';
		
		if(empty($value_e_postal_dist) &&!empty($e_postal_dist))
			$value_e_postal_dist	=	$e_postal_dist;
									
									
				$form['bocwa_reg_master']['e_postal_subdivision'] 		  	= array(
																					'#prefix' 			=> '<div id="bocwa_e_postal_subdivision_replace">',
																					'#suffix' 			=> '</div>',
																				);
	
				$form['bocwa_reg_master']['e_postal_subdivision'] 		 	= array(
																				  '#prefix' 			=> '<div id="bocwa_e_postal_subdvsn_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Subdivision',
																				  '#type' 				=> 'select',
																				  '#options' 			=> custom_user_subdivisionlist_list($value_e_postal_dist),// ---------come from custom_user module
																				  '#default_value' 		=> !empty($e_postal_subdivision) ? $e_postal_subdivision	:'',
																				  '#attributes'			=>  $readonly_e_postal_subdivision,
																				  '#required' 			=> TRUE,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'), 
																			);
								
								
				$form['bocwa_reg_master']['e_postal_areatype']   		  	= array(
																				  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Area type',
																				  '#type' 				=> 'select',
																				  '#options' 			=>array('T'=>'Select Area Type','B' =>'Block','M' => 'Municipality','C' => 'Corporation', 'S' =>'SEZ','N' =>'Notified Area'),
																				  '#default_value' 		=> !empty($e_postal_areatype) ? $e_postal_areatype	:'',
																				  '#required' 			=> TRUE,
																				  '#attributes'			=>  $readonly_e_postal_areatype,
																				  '#element_validate'	=> array('BOCWA_LOCATION_AREATYPE_VALIDATE'), 
																				  '#ajax'				=> array(
																											'event' 			=> 'change',
																											'effect' 			=> 'fade',			
																											'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																											'progress'			=> '',
																										  ),
																					);
																						
			$form['bocwa_reg_master']['e_postal_block'] 					= array(
																				  '#prefix' 			=> '<div id="bocwa_e_postal_block_replace">',
																				  '#suffix' 			=> '</div>'
																			  );
						  
			$form['bocwa_reg_master']['e_postal_mun'] 						= array(
																				  '#prefix' 			=> '<div id="bocwa_e_postal_mun_replace">',
																				  '#suffix' 			=> '</div>'
																			  );
				  
			$form['bocwa_reg_master']['e_postal_corp'] 						= array(
																					'#prefix' 			=> '<div id="bocwa_e_postal_corp_replace">',
																					'#suffix' 			=> '</div>'
																				  );
				  
			$form['bocwa_reg_master']['e_postal_sez'] 						= array(
																					'#prefix' 			=> '<div id="bocwa_e_postal_sez_replace">',
																					'#suffix' 			=> '</div>'
																			  );
				  
			$form['bocwa_reg_master']['e_postal_not'] 						= array(
																					'#prefix' 			=> '<div id="bocwa_e_postal_not_replace">',
																					'#suffix' 			=> '</div>'
																					);	
					
															  
			$form['bocwa_reg_master']['e_postal_other'] 					= array(
																					'#prefix' 			=> '<div id="bocwa_e_postal_other_replace">',
																					'#suffix' 			=> '</div>'
																				  );
																				  
			$value_e_postal_areatype 		= isset($form_state['values']['e_postal_areatype']) ? $form_state['values']['e_postal_areatype'] : '';
			$value_e_postal_subdivision 	= isset($form_state['values']['e_postal_subdivision']) ? $form_state['values']['e_postal_subdivision'] : '';
	
			if(empty($value_e_postal_areatype) && !empty($e_postal_areatype))
				$value_e_postal_areatype	=	$e_postal_areatype;
	
			if(empty($value_e_postal_subdivision) && !empty($e_postal_subdivision))
				$value_e_postal_subdivision = $e_postal_subdivision;
				
			if(!empty($value_e_postal_areatype)){
				if($value_e_postal_areatype == 'B'){
					
					$form['bocwa_reg_master']['e_postal_block'] 			= array(
																				  '#prefix' 			=> '<div id="bocwa_e_postal_block_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Block',
																				  '#type' 				=> 'select',
																				  '#options' 			=> custom_user_block_list($value_e_postal_subdivision,$value_e_postal_areatype),
																				  '#default_value' 		=> !empty($e_postal_areatype_code) ? $e_postal_areatype_code	:'',
																				  '#required' 			=> TRUE,
																				  '#attributes'			=>  $readonly_e_postal_block,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				  '#ajax'				=> array(
																											'event' 			=> 'change',
																											'effect' 			=> 'fade',			
																											'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																											'progress'			=> '',
																										  )
																			 );
						 
			}else if($value_e_postal_areatype == 'M'){
				
					$form['bocwa_reg_master']['e_postal_mun'] 				= array(
																				  '#prefix' 			=> '<div id="bocwa_e_postal_mun_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Municipality',
																				  '#type' 				=> 'select',
																				  '#options' 			=> custom_user_block_list($value_e_postal_subdivision,$value_e_postal_areatype),// come from custom_user module
																				  '#default_value' 		=> !empty($e_postal_areatype_code) ? $e_postal_areatype_code	:'',
																				  '#required' 			=> TRUE,
																				  '#attributes'			=>  $readonly_e_postal_mun,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				  '#ajax'				=> array(
																											'event' 			=> 'change',
																											'effect' 			=> 'fade',			
																											'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																											'progress'			=> '',
																										  )
																				);
																					
			}else if($value_e_postal_areatype == 'C'){
			  
					$form['bocwa_reg_master']['e_postal_corp'] 				= array(
																				  '#prefix' 			=> '<div id="bocwa_e_postal_corp_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Corporation',
																				  '#type' 				=> 'select',
																				  '#options' 			=> custom_user_block_list($value_e_postal_subdivision,$value_e_postal_areatype),// come from custom_user module
																				  '#default_value' 		=> !empty($e_postal_areatype_code) ? $e_postal_areatype_code	:'',
																				  '#required' 			=> TRUE,
																				  '#attributes'			=>  $readonly_e_postal_corp,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				  '#ajax'				=> array(
																											'event' 			=> 'change',
																											'effect' 			=> 'fade',			
																											'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																											'progress'			=> '',
																										  )
																			);
																				
			}else if($value_e_postal_areatype == 'S'){
				
				$form['bocwa_reg_master']['e_postal_sez'] 					= array(
																				  '#prefix' 			=> '<div id="bocwa_e_postal_sez_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select SEZ area',
																				  '#type' 				=> 'select',
																				  '#options' 			=> custom_user_block_list($value_e_postal_subdivision,$value_e_postal_areatype),// come from custom_user module
																				  '#default_value' 		=> !empty($e_postal_areatype_code) ? $e_postal_areatype_code	:'',
																				  '#required' 			=> TRUE,
																				  '#attributes'			=>  $readonly_e_postal_sez,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				  '#ajax'				=> array(
																											'event' 			=> 'change',
																											'effect' 			=> 'fade',			
																											'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																											'progress'			=> '',
																										  )
																				);
																					
			}else if($value_e_postal_areatype == 'N'){
				
				$form['bocwa_reg_master']['e_postal_not'] 					= array(
																				  '#prefix' 			=> '<div id="bocwa_e_postal_not_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Notifed area',
																				  '#type' 				=> 'select',
																				  '#options' 			=> custom_user_block_list($value_e_postal_subdivision,$value_e_postal_areatype),// come from custom_user module
																				  '#default_value' 		=> !empty($e_postal_areatype_code) ? $e_postal_areatype_code	:'',
																				  '#required' 			=> TRUE,
																				  '#attributes'			=>  $readonly_e_postal_not,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				  '#ajax'				=> array(
																											'event' 			=> 'change',
																											'effect' 			=> 'fade',			
																											'callback'			=> 'get_bocwa_e_postal_address__ajax_callback',
																											'progress'			=> '',
																										  )
																				);
																					
			}else if($value_e_postal_areatype == 'O'){
				
				$form['bocwa_reg_master']['e_postal_other'] 				= array(
																				  '#prefix' 			=> '<div id="bocwa_e_postal_other_replace" class="col-lg-4 col-md-6"><label class="input">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Other area',
																				  '#type' 				=> 'textfield',
																				  '#attributes'			=>  $readonly_e_postal_other,
																				  '#default_value' 		=> !empty($e_postal_areatype_code) ? $e_postal_areatype_code : '',
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
	
		$e_postal_code				=	"";
		$value_e_postal_block 				= 	isset($form_state['values']['e_postal_block']) ? $form_state['values']['e_postal_block'] : '';
		$value_e_postal_mun					= 	isset($form_state['values']['e_postal_mun']) ? $form_state['values']['e_postal_mun'] : '';
		$value_e_postal_corp 				= 	isset($form_state['values']['e_postal_corp']) ? $form_state['values']['e_postal_corp'] : '';
		$value_e_postal_sez 				= 	isset($form_state['values']['e_postal_sez']) ? $form_state['values']['e_postal_sez'] : '';
		$value_e_postal_not 				= 	isset($form_state['values']['e_postal_not']) ? $form_state['values']['e_postal_not'] : '';
		
		if(empty($value_e_postal_block)){
			if(empty($value_e_postal_mun)){
				if(empty($value_e_postal_corp)){
					if(empty($value_e_postal_sez)){
						if(empty($value_e_postal_not)){
							$e_postal_code	=	"";
						}else
							$e_postal_code = $value_e_postal_not;
					}else
						$e_postal_code	=	$value_e_postal_sez;
				}else
					$e_postal_code = $value_e_postal_corp;
			}else
				$e_postal_code  = $value_e_postal_mun;
		}else
			$e_postal_code = $value_e_postal_block;
			
			
		if(empty($e_postal_code) && !empty($e_postal_areatype_code))
				$e_postal_code = $e_postal_areatype_code;
	
					$form['bocwa_reg_master']['e_postal_vill'] 				= array(
																					'#prefix' 			=> '<div id="bocwa_e_postal_vill_replace">',
																					'#suffix' 			=> '</div>'
																			 );
									 
					$form['bocwa_reg_master']['e_postal_ward'] 				= array(
																					'#prefix' 			=> '<div id="bocwa_e_postal_ward_replace">',
																					'#suffix' 			=> '</div>'
																			 );
									 
					$form['bocwa_reg_master']['e_postal_bugh'] 				= array(
																					'#prefix' 			=> '<div id="bocwa_e_postal_bugh_replace">',
																					'#suffix' 			=> '</div>'
																			 );
									 
					$form['bocwa_reg_master']['e_postal_areatype_code'] 	= array(
																					'#prefix' 			=> '<div id="bocwa_e_postal_code_replace">',
																					'#suffix' 			=> '</div>'
																				 );
																				 
			if(!empty($e_postal_code)){
	
				$form['bocwa_reg_master']['e_postal_areatype_code'] 		 = array(
																					'#prefix' 			=> '<div id="bocwa_e_postal_code_replace">',
																					'#suffix' 			=> '</div>',
																					'#type' 			=> 'hidden',
																					'#value'			=> !empty($e_postal_code) ? $e_postal_code	:''
																			  );
	
				if($value_e_postal_areatype == 'B'){
					
					$form['bocwa_reg_master']['e_postal_vill'] 				= array(
																				  '#prefix' 			=> '<div id="bocwa_e_postal_vill_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix'				=> '</label></div>',
																				  '#title'				=> 'Select Gram Panchayat',
																				  '#type' 				=> 'select',
																				  '#options' 			=> custom_user_villward_list($e_postal_code,'V'),// come from custom_user module
																				  '#default_value' 		=> !empty($e_postal_vill_ward) ? $e_postal_vill_ward	:'',
																				  '#attributes'			=>  $readonly_e_postal_vill,
																				  '#required' 			=> TRUE,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				);
				}
			
				if($value_e_postal_areatype == 'M'){
					
					$form['bocwa_reg_master']['e_postal_ward'] 				= array(
																				  '#prefix' 			=> '<div id="bocwa_e_postal_ward_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Ward',
																				  '#type' 				=> 'select',
																				  '#options' 			=> custom_user_villward_list($e_postal_code,'W'),// come from custom_user module
																				  '#default_value' 		=> !empty($e_postal_vill_ward) ? $e_postal_vill_ward	:'',
																				  '#required' 			=> TRUE,
																				  '#attributes'			=>  $readonly_e_postal_ward,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				);
																				
				}
			
				if($value_e_postal_areatype == 'C'){
					
					$form['bocwa_reg_master']['e_postal_bugh'] 				= array(
																				  '#prefix' 			=> '<div id="bocwa_e_postal_bugh_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Ward',
																				  '#type' 				=> 'select',
																				  '#options' 			=> custom_user_villward_list($e_postal_code,'C'),// come from custom_user module
																				  '#default_value' 		=> !empty($e_postal_borough) ? $e_postal_borough	:'',
																				  '#required' 			=> TRUE,
																				  '#attributes'			=>  $readonly_e_postal_bugh,
																				  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				);
				}
			}
	
								
		$form['bocwa_reg_master']['e_postal_ps'] 								= array(
																					'#prefix' 				=> '<div id="bocwa_e_postal_ps_replace">',
																					'#suffix' 				=> '</div>'
																			   );
	
							
		$form['bocwa_reg_master']['e_postal_ps'] 								= array(
																					  '#prefix' 			=> '<div id="bocwa_e_postal_ps_replace" class="col-lg-4 col-md-6"><label class="select">',
																					  '#suffix' 			=> '</label></div>',
																					  '#title'				=> 'Select Police Station',
																					  '#type' 				=> 'select',
																					  '#options' 			=> custom_user_ps_list($value_e_postal_dist),// come from custom_user module
																					  '#default_value' 		=> !empty($e_postal_ps) ? $e_postal_ps	:'',
																					  '#required' 			=> TRUE,
																					  '#attributes'			=>  $readonly_e_postal_ps,
																					  '#element_validate'	=> array('BOCWA_POLICE_STATION_VALIDATE'),  
																					);
																					
		$form['bocwa_reg_master']['e_postal_pin_number'] 						= array(
																					  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																					  '#suffix' 			=> '</label></div></div></div></div>',
																					  '#title'				=> 'PIN Code ',
																					  '#type' 				=> 'textfield',
																					  '#default_value' 	 	=> !empty($e_postal_pin_number) ? $e_postal_pin_number	:'',
																					  '#attributes'			=>  $readonly_e_postal_pin_number,
																					  '#required' 			=> TRUE
																						  );
							
		/*================== POSTAL ADDRESS OF THE ESTABLISHMENT ==============*/
		
		
		$form['bocwa_reg_master']['e_full_name'] 								= array(
																					  //'#prefix'				=> '<fieldset><legend class="form-title">5. Full Name and permanent address of the Establishment</legend><div class="row"><section class="col-lg-4 col-md-6"><label class="input">',
																					  '#prefix'				=> '<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">5. Full Name and permanent address of the Establishment</div><div class="panel-body"><div class="col-lg-4 col-md-6"><label class="input">',
																					  '#suffix' 			=> '</label></div>',
																					  '#title'				=> 'Full Name of the Establishment',
																					  '#type' 				=> 'textfield',
																					  '#default_value' 		=> !empty($e_full_name) ? $e_full_name : '',
																					  '#required' 			=> TRUE,
																					  '#attributes' 		=> $readonly_e_full_name,
																					  '#element_validate' 	=> array('BOCWA_SPECIAL_CHAR_TEXT_VALIDATE')
																					);
																					
		/*================== PERMANENT ADDRESS OF THE ESTABLISHMENT ==============*/
									
		$form['bocwa_reg_master']['e_permanent_address'] 						= array(
																					  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																					  '#suffix' 			=> '</label></div>',
																					  '#title'				=> 'House No./Village Name/Street/Road',
																					  '#type' 				=> 'textfield',
																					  '#default_value' 		=> !empty($e_permanent_address) ? $e_permanent_address : '',
																					  '#required' 			=> TRUE,
																					  '#attributes' 		=> $readonly_e_permanent_address,
																					  '#element_validate' => array('BOCWA_SPECIAL_CHAR_TEXTAREA_VALIDATE')
																					);
									
									
		$form['bocwa_reg_master']['e_permanent_add_dist'] 	   					= array(
																					  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="select">',
																					  '#suffix'				=> '</label></div>',
																					  '#title'				=> 'Select District',
																					  '#type' 				=> 'select',
																					  '#options' 			=> $options_dist,
																					  '#default_value' 		=> !empty($e_permanent_add_dist) ? $e_permanent_add_dist	:'',
																					  '#required' 			=> TRUE,
																					  '#attributes' 		=> $readonly_e_permanent_add_dist,
																					  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																					  '#ajax'				=> array(
																													'event' 			=> 'change',
																													'effect' 			=> 'fade',			
																													'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																													'progress'			=> '',
																												  )
																					);
																					
																					
		$value_e_permanent_add_dist 	= 	isset($form_state['values']['e_permanent_add_dist']) ? $form_state['values']['e_permanent_add_dist'] : '';
	
		if(empty($value_e_permanent_add_dist) &&!empty($e_permanent_add_dist))
			$value_e_permanent_add_dist		=	$e_permanent_add_dist;
	
	
	
			$form['bocwa_reg_master']['e_permanent_add_subdivision'] 		  = array(
																					'#prefix' 			=> '<div id="bocwa_e_permanent_add_subdivision_replace">',
																					'#suffix' 			=> '</div>',
																				);
	
			$form['bocwa_reg_master']['e_permanent_add_subdivision'] 		  = array(
																				   '#prefix' 			=> '<div id="bocwa_e_permanent_add_subdvsn_replace" class="col-lg-4 col-md-6"><label class="select">',
																				   '#suffix' 			=> '</label></div>',
																				   '#title'				=> 'Select Subdivision',
																				   '#type' 				=> 'select',
																				   '#options' 			=> custom_user_subdivisionlist_list($value_e_permanent_add_dist),// ---------come from custom_user module
																				   '#default_value' 	=> !empty($e_permanent_add_subdivision) ? $e_permanent_add_subdivision	:'',
																				   '#required' 			=> TRUE,
																				   '#attributes' 		=> $readonly_e_permanent_add_subdivision,
																				   '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				);
							
							
			$form['bocwa_reg_master']['e_permanent_add_areatype']   		  	= array(
																				  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 			=> '</label></div>',
																				  '#title'				=> 'Select Area type',
																				  '#type' 				=> 'select',
																				  '#options' 			=>array('T'=>'Select Area Type','B' =>'Block','M' => 'Municipality','C' => 'Corporation', 'S' =>'SEZ','N' =>'Notified Area'),
																				  '#default_value' 		=> !empty($e_permanent_add_areatype) ? $e_permanent_add_areatype	:'',
																				  '#required' 			=> TRUE,
																				  '#attributes' 		=> $readonly_e_permanent_add_areatype,
																				  '#element_validate'	=> array('BOCWA_LOCATION_AREATYPE_VALIDATE'),
																				  '#ajax'				=> array(
																												'event' 			=> 'change',
																												'effect' 			=> 'fade',			
																												'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																												'progress'			=> '',
																											  ),
																					);
										
										
			$form['bocwa_reg_master']['e_permanent_add_block'] 					= array(
																					'#prefix' 				=> '<div id="bocwa_e_permanent_add_block_replace">',
																					'#suffix' 				=> '</div>'
																			  );
						  
			$form['bocwa_reg_master']['e_permanent_add_mun'] 					= array(
																						'#prefix' 			=> '<div id="bocwa_e_permanent_add_mun_replace">',
																						'#suffix' 			=> '</div>'
																				  );
						  
			$form['bocwa_reg_master']['e_permanent_add_corp'] 					= array(
																						'#prefix' 			=> '<div id="bocwa_e_permanent_add_corp_replace">',
																						'#suffix' 			=> '</div>'
																				  );
						  
			$form['bocwa_reg_master']['e_permanent_add_sez'] 					= array(
																						'#prefix' 			=> '<div id="bocwa_e_permanent_add_sez_replace">',
																						'#suffix' 			=> '</div>'
																					  );
						  
			$form['bocwa_reg_master']['e_permanent_add_not'] 					= array(
																						'#prefix' 			=> '<div id="bocwa_e_permanent_add_not_replace">',
																						'#suffix' 			=> '</div>'
																						);	
							
																	  
			$form['bocwa_reg_master']['e_permanent_add_other'] 					= array(
																					'#prefix' 				=> '<div id="bocwa_e_permanent_add_other_replace">',
																					'#suffix' 				=> '</div>'
																				  );
				  
				  
			$value_e_permanent_add_subdivision 		= isset($form_state['values']['e_permanent_add_subdivision']) ? $form_state['values']['e_permanent_add_subdivision'] : '';
			$value_e_permanent_add_areatype 		= isset($form_state['values']['e_permanent_add_areatype']) ? $form_state['values']['e_permanent_add_areatype'] : '';
			
			
			if(empty($value_e_permanent_add_areatype) && !empty($e_permanent_add_areatype)){
				$value_e_permanent_add_areatype			= $e_permanent_add_areatype;
			}			
			if(empty($value_e_permanent_add_subdivision) && !empty($e_permanent_add_subdivision)){
				$value_e_permanent_add_subdivision 		= $e_permanent_add_subdivision;
			}
				
			if(!empty($value_e_permanent_add_areatype)){
				
				if($value_e_permanent_add_areatype == 'B'){
					
					$form['bocwa_reg_master']['e_permanent_add_block'] 			= array(
																					  '#prefix' 			=> '<div id="bocwa_e_permanent_add_block_replace" class="col-lg-4 col-md-6"><label class="select">',
																					  '#suffix' 			=> '</label></div>',
																					  '#title'				=> 'Select Block',
																					  '#type' 				=> 'select',
																					  '#options' 			=> custom_user_block_list($value_e_permanent_add_subdivision,$value_e_permanent_add_areatype),
																					  '#default_value' 		=> !empty($e_permanent_add_areatype_code) ? $e_permanent_add_areatype_code	:'',
																					  '#required' 			=> TRUE,
																					  '#attributes' 		=> $readonly_e_permanent_add_block,
																					  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																					  '#ajax'				=> array(
																												'event' 			=> 'change',
																												'effect' 			=> 'fade',			
																												'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																												'progress'			=> '',
																											  )
																				 );
						 
				}else if($value_e_permanent_add_areatype == 'M'){
					
					$form['bocwa_reg_master']['e_permanent_add_mun']			= array(
																					  '#prefix' 			=> '<div id="bocwa_e_permanent_add_mun_replace" class="col-lg-4 col-md-6"><label class="select">',
																					  '#suffix' 			=> '</label></div>',
																					  '#title'				=> 'Select Municipality',
																					  '#type' 				=> 'select',
																					  '#options' 			=> custom_user_block_list($value_e_permanent_add_subdivision,$value_e_permanent_add_areatype),// come from custom_user module	
																					  '#default_value' 		=> !empty($e_permanent_add_areatype_code) ? $e_permanent_add_areatype_code	:'',
																					  '#required' 			=> TRUE,
																					  '#attributes' 		=> $readonly_e_permanent_add_mun,
																					  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																					  '#ajax'				=> array(
																													'event' 			=> 'change',
																													'effect' 			=> 'fade',			
																													'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																													'progress'			=> '',
																												  )
																				);
																				
				}else if($value_e_permanent_add_areatype == 'C'){
	
					$form['bocwa_reg_master']['e_permanent_add_corp'] 		 = array(
																				  '#prefix' 				=> '<div id="bocwa_e_permanent_add_corp_replace" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 				=> '</label></div>',
																				  '#title'					=> 'Select Corporation',
																				  '#type' 					=> 'select',
																				  '#options' 				=> custom_user_block_list($value_e_permanent_add_subdivision,$value_e_permanent_add_areatype),// come from custom_user module
																				  '#default_value' 			=> !empty($e_permanent_add_areatype_code) ? $e_permanent_add_areatype_code	:'',
																				  '#required' 				=> TRUE,
																				  '#attributes' 			=> $readonly_e_permanent_add_corp,
																				  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				  '#ajax'					=> array(
																													'event' 			=> 'change',
																													'effect' 			=> 'fade',			
																													'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																													'progress'			=> '',
																												  )
																			);
																			
				}else if($value_e_permanent_add_areatype == 'S'){
					
					$form['bocwa_reg_master']['e_permanent_add_sez'] 			= array(
																					  '#prefix' 			=> '<div id="bocwa_e_permanent_add_sez_replace" class="col-lg-4 col-md-6"><label class="select">',
																					  '#suffix' 			=> '</label></div>',
																					  '#title'				=> 'Select SEZ',
																					  '#type' 				=> 'select',
																					  '#options' 			=> custom_user_block_list($value_e_permanent_add_subdivision,$value_e_permanent_add_areatype),// come from custom_user module
																					  '#default_value' 		=> !empty($e_permanent_add_areatype_code) ? $e_permanent_add_areatype_code	:'',
																					  '#required' 			=> TRUE,
																					  '#attributes' 		=> $readonly_e_permanent_add_sez,
																					  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																					  '#ajax'				=> array(
																													'event' 			=> 'change',
																													'effect' 			=> 'fade',			
																													'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																													'progress'			=> '',
																												  )
																					);
																					
				}else if($value_e_permanent_add_areatype == 'N'){
					
					$form['bocwa_reg_master']['e_permanent_add_not'] 			= array(
																					  '#prefix' 			=> '<div id="bocwa_e_permanent_add_not_replace" class="col-lg-4 col-md-6"><label class="select">',
																					  '#suffix' 			=> '</label></div>',
																					  '#title'				=> 'Select Notified area',
																					  '#type' 				=> 'select',
																					  '#options' 			=> custom_user_block_list($value_e_permanent_add_subdivision,$value_e_permanent_add_areatype),// come from custom_user module
																					  '#default_value' 		=> !empty($e_permanent_add_areatype_code) ? $e_permanent_add_areatype_code	:'',
																					  '#required' 			=> TRUE,
																					  '#attributes' 		=> $readonly_e_permanent_add_not,
																					  '#element_validate'	=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																					  '#ajax'				=> array(
																													'event' 			=> 'change',
																													'effect' 			=> 'fade',			
																													'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																													'progress'			=> '',
																												  )
																					);
																					
				}else if($value_e_permanent_add_areatype == 'O'){
					
					$form['bocwa_reg_master']['e_permanent_add_other'] 		= array(
																				  '#prefix' 				=> '<div id="bocwa_e_permanent_add_other_replace" class="col-lg-4 col-md-6"><label class="input">',
																				  '#suffix' 				=> '</label></div>',
																				  '#title'					=> 'Select Other area ',
																				  '#type' 					=> 'textfield',
																				  '#default_value' 			=> !empty($e_permanent_add_areatype_code) ? $e_permanent_add_areatype_code : '',
																				  '#required' 				=> TRUE,
																				  '#attributes' 			=> $readonly_e_permanent_add_other,
																				  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																				  '#ajax'					=> array(
																													'event' 			=> 'change',
																													'effect' 			=> 'fade',			
																													'callback'			=> 'get_bocwa_e_permanent_address_ajax_callback',
																													'progress'			=> '',
																												  )
																				);
						 
				}
			}	
	
			$e_permanent_add_code	=	"";
			$value_e_permanent_add_block 	= 	isset($form_state['values']['e_permanent_add_block']) ? $form_state['values']['e_permanent_add_block'] : '';
			$value_e_permanent_add_mun		= 	isset($form_state['values']['e_permanent_add_mun']) ? $form_state['values']['e_permanent_add_mun'] : '';
			$value_e_permanent_add_corp 	= 	isset($form_state['values']['e_permanent_add_corp']) ? $form_state['values']['e_permanent_add_corp'] : '';
			$value_e_permanent_add_sez 		= 	isset($form_state['values']['e_permanent_add_sez']) ? $form_state['values']['e_permanent_add_sez'] : '';
			$value_e_permanent_add_not 		= 	isset($form_state['values']['e_permanent_add_not']) ? $form_state['values']['e_permanent_add_not'] : '';
			
			
			if(empty($value_e_permanent_add_block)){
				if(empty($value_e_permanent_add_mun)){
					if(empty($value_e_permanent_add_corp)){
						if(empty($value_e_permanent_add_sez)){
								if(empty($value_e_permanent_add_not)){
									$e_permanent_add_code	=	"";
								}else
									$e_permanent_add_code = $value_e_permanent_add_not;
						}else
							$e_permanent_add_code = $value_e_permanent_add_sez;
					}else
						$e_permanent_add_code = $value_e_permanent_add_corp;
				}else
					$e_permanent_add_code = $value_e_permanent_add_mun;
			}else
				$e_permanent_add_code = $value_e_permanent_add_block;
			
		
			if(empty($e_permanent_add_code) && !empty($e_permanent_add_areatype_code))
					$e_permanent_add_code = $e_permanent_add_areatype_code;
	
			$form['bocwa_reg_master']['e_permanent_add_vill'] 					= array(
																						'#prefix' 			=> '<div id="bocwa_e_permanent_add_vill_replace">',
																						'#suffix' 			=> '</div>'
																				 );
					 
			$form['bocwa_reg_master']['e_permanent_add_ward'] 					= array(
																						'#prefix' 			=> '<div id="bocwa_e_permanent_add_ward_replace">',
																						'#suffix' 			=> '</div>'
																				 );
							 
			$form['bocwa_reg_master']['e_permanent_add_bugh'] 					= array(
																						'#prefix' 			=> '<div id="bocwa_e_permanent_add_bugh_replace">',
																						'#suffix' 			=> '</div>'
																				 );
							 
			$form['bocwa_reg_master']['e_permanent_add_areatype_code'] 			= array(
																						'#prefix' 			=> '<div id="bocwa_e_permanent_add_code_replace">',
																						'#suffix' 			=> '</div>'
																					 );
					 
			if(!empty($e_permanent_add_code)){
	
				$form['bocwa_reg_master']['e_permanent_add_areatype_code'] 	   = array(
																					'#prefix' 				=> '<div id="bocwa_e_permanent_add_code_replace">',
																					'#suffix' 				=> '</div>',
																					'#type' 				=> 'hidden',
																					'#value'				=> !empty($e_permanent_add_code) ? $e_permanent_add_code	:''
																			  );
	
	
				  if($value_e_permanent_add_areatype == 'B'){
				  
					$form['bocwa_reg_master']['e_permanent_add_vill'] 			= array(
																					  '#prefix' 				=> '<div id="bocwa_e_permanent_add_vill_replace" class="col-lg-4 col-md-6"><label class="select">',
																					  '#suffix' 				=> '</label></div>',
																					  '#title'					=> 'Select Gram Panchayat',
																					  '#type' 					=> 'select',
																					  '#options' 				=> custom_user_villward_list($e_permanent_add_code,'V'),// come from custom_user module
																					  '#default_value' 			=> !empty($e_permanent_add_vill_ward) ? $e_permanent_add_vill_ward	:'',
																					  '#required' 				=> TRUE,
																					  '#attributes' 			=> $readonly_e_permanent_add_vill,
																					  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																					);
				}
		
				if($value_e_permanent_add_areatype == 'M'){
				
					$form['bocwa_reg_master']['e_permanent_add_ward'] 			= array(
																					  '#prefix' 				=> '<div id="bocwa_e_permanent_add_ward_replace" class="col-lg-4 col-md-6"><label class="select">',
																					  '#suffix' 				=> '</label></div>',
																					  '#title'					=> 'Select Ward',
																					  '#type' 					=> 'select',
																					  '#options' 				=> custom_user_villward_list($e_permanent_add_code,'W'),// come from custom_user module
																					  '#default_value' 			=> !empty($e_permanent_add_vill_ward) ? $e_permanent_add_vill_ward	:'',
																					  '#required' 				=> TRUE,
																					  '#attributes' 			=> $readonly_e_permanent_add_ward,
																					  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																					);
				}
		
				if($value_e_permanent_add_areatype == 'C'){
				
					$form['bocwa_reg_master']['e_permanent_add_bugh'] 			= array(
																					  '#prefix' 				=> '<div id="bocwa_e_permanent_add_bugh_replace" class="col-lg-4 col-md-6"><label class="select">',
																					  '#suffix' 				=> '</label></div>',
																					  '#title'					=> 'Select Ward',
																					  '#type' 					=> 'select',
																					  '#options' 				=> custom_user_villward_list($e_permanent_add_code,'C'),
																					  '#default_value' 			=> !empty($e_permanent_add_borough) ? $e_permanent_add_borough	:'',
																					  '#required' 				=> TRUE,
																					  '#attributes' 			=> $readonly_e_permanent_add_bugh,
																					  '#element_validate'		=> array('BOCWA_DISTRICT_SUBDIV_BLOCK_VALIDATE'),
																					);
				}
			}
	
			$form['bocwa_reg_master']['e_permanent_add_ps'] 					= array(
																						'#prefix' 			=> '<div id="bocwa_e_permanent_add_ps_replace">',
																						'#suffix' 			=> '</div>'
																				   );
			
									
			$form['bocwa_reg_master']['e_permanent_add_ps'] 					= array(
																					  '#prefix' 			=> '<div id="bocwa_e_permanent_add_ps_replace" class="col-lg-4 col-md-6"><label class="select">',
																					  '#suffix' 			=> '</label></div>',
																					  '#title'				=> 'Select Police Station',
																					  '#type' 				=> 'select',
																					  '#options' 			=> custom_user_ps_list($value_e_permanent_add_dist),// come from custom_user module
																					  '#default_value' 		=> !empty($e_permanent_add_ps) ? $e_permanent_add_ps	:'',
																					  '#required' 			=> TRUE,
																					  '#attributes' 		=> $readonly_e_permanent_add_ps,
																					  '#element_validate'	=> array('BOCWA_POLICE_STATION_VALIDATE'),  
																					);
																					
			$form['bocwa_reg_master']['e_permanent_add_pin_number'] 		   	= array(
																					  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																					  '#suffix' 			=> '</label></div></div></div></div>',
																					  '#title'				=> 'PIN Code',
																					  '#type'				=> 'textfield',
																					  '#default_value' 	 	=> !empty($e_permanent_add_pin_number) ? $e_permanent_add_pin_number	:'',
																					  '#attributes' 		=> $readonly_e_permanent_add_pin_number,
																					  '#required' 			=> TRUE
																					);
																					
			/*================== PERMANENT ADDRESS OF THE ESTABLISHMENT ENDS ==============*/
			
			
		/*================== ADDRESS OF THE MANAGER ENDS==============*/																				
																						
		$form['bocwa_reg_master']['nature_of_build_const'] 					= array(
																					//'#prefix' 				=> '<fieldset><div class="row"><section class="col-lg-4 col-md-6"><label class="input">',
																					'#prefix' 				=> '<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">6. Other Information</div><div class="panel-body"><div class="col-lg-6 col-md-6"><label class="input">',
																					'#suffix' 				=> '</label></div>',
																					'#title'				=> '6. a) Nature of building or other construction work is to be carried on ',
																					'#type' 				=> 'textfield',
																					'#default_value' 		=> !empty($nature_of_build_const) ? $nature_of_build_const : '',
																					'#attributes' 			=> $readonly_nature_of_build_const,
																					'#required' 			=> TRUE,
																					'#element_validate'		=> array('BOCWA_TEXTBOX_TEXT_VALIDATE')
																					);
											
											
		$form['bocwa_reg_master']['max_no_of_building_workers_employed']	= array(
			'#prefix' 				=> '<div class="col-lg-6 col-md-6"><label class="input">',
			'#suffix' 				=> '</label></div>',
			'#title'				=> '6. b) Maximum number of building workers to be employed on any day',
			'#type' 				=> 'textfield',
			'#default_value' 		=> !empty($max_no_of_building_workers_employed) ? $max_no_of_building_workers_employed : '',
			'#required' 			=> TRUE,
			'#attributes' 			=> $readonly_max_no_of_building_workers_employed,
			'#element_validate'		=> array('BOCWA_TEXTBOX_NUMERIC_VALIDATE')
		);	
									
		$form['bocwa_reg_master']['est_date_of_commencement_building'] 	 	= array(
			'#prefix' 					=> '<div class="col-lg-6 col-md-6"><label class="input">',
			'#suffix' 					=> '</label></div>',
			'#title'					=> '6. c) Estimated date of commencement of building or other construction work',
			'#type' 					=> 'textfield',
			'#default_value' 			=> !empty($est_date_of_commencement_building) ? date('d/m/Y',strtotime($est_date_of_commencement_building)) : '',
			'#required' 				=> TRUE,
			'#attributes' 				=> $readonly_est_date_of_commencement_building,
			'#id'						=> 'est_date_of_commencement_id',
			//'#attributes'				=> array('id'=> 'est_date_of_commencement_building_id', 'readonly'=>'readonly','placeholder' => t('From'))
			);
										
		$form['bocwa_reg_master']['est_date_of_completion_building']   		= array(
			'#prefix' 					=> '<div class="col-lg-6 col-md-6"><label class="input">',
			'#suffix' 					=> '</label></div>',
			'#title'					=> '6. d) Estimated date of the completion of building or other construction work',
			'#type' 					=> 'textfield',
			'#default_value' 			=> !empty($est_date_of_completion_building) ? date('d/m/Y',strtotime($est_date_of_completion_building)) : '',
			'#required' 				=> TRUE,
			'#attributes' 				=> $readonly_est_date_of_completion_building,
			'#id'						=> 'est_date_of_termination_id'
			//'#attributes'				=>array('id'=> 'est_date_of_completion_building_id', 'readonly'=>'readonly', 'placeholder'=> t('To'))
		);																				
																					
		
		$form['bocwa_reg_master']['submit'] 								= array (
		  '#prefix' 				=> '</div></div>',  
		  '#suffix' 				=> '</div>',  
		  '#type' 					=> 'submit',
		  '#attributes' 			=> array('class' => array('btn btn-primary pull-right')),
		  '#value' 					=> 'SAVE'
		);
	
	} else {
		
		if(!empty($application_id)){
								
			$get_bocwa_registration_result= get_readable_applicant_bocwa_registration_form($application_id);
				 $form['showmsg'] = array(
					  '#markup' => ''.$get_bocwa_registration_result.'',
					  ); 
		}
		
	}
	
	return $form;
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
	$match_pattern 	= 	'/[\'\~`\!@#\$%\^&\*\(\)_\+=\{\}\[\]\|;:"\<\>.\?\\]/';
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
		form_set_error($form_state['bocwa_reg_master']['loc_e_ps'], t('Police Station SHOULD BE VALID'));
	}
	if( $police_statn_2 == "" ){
		form_set_error($form_state['bocwa_reg_master']['e_postal_ps'], t('Police Station cfgfg SHOULD BE VALID'));
	}
	if( $police_statn_3 == "" ){
		form_set_error($form_state['bocwa_reg_master']['e_permanent_add_ps'], t('Police Station gddgdg SHOULD BE VALID'));
	}
	if( $police_statn_4 == "" ){
		form_set_error($form_state['bocwa_reg_master']['address_manager_ps'], t('Police Station dgdgdgdgdgdgdg SHOULD BE VALID'));
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


function get_bocwa_registration_info_form_submit($form, &$form_state){
	 
		global $base_root, $base_path, $user;
		
		$val 									= $form_state['values'];
		
		
		$user_id								= $user->uid;
		$act_id 								= encryption_decryption_fun('decrypt', $val['bocwa_act_id']);
		$application_id							= encryption_decryption_fun('decrypt', $val['application_id']);
		$identification_number					= encryption_decryption_fun('decrypt', $val['identification_number']);
		
		
		$e_name									= $val['e_name'];
		$loc_e_name								= $val['loc_e_name'];
		$loc_e_dist								= $val['loc_e_dist'];
		$loc_e_subdivision						= $val['loc_e_subdivision'];
		$loc_e_areatype							= $val['loc_e_areatype'];
		$loc_e_areatype_code					= $val['loc_e_areatype_code'];
		$loc_e_vill 							= $val['loc_e_vill'];
		$loc_e_ward 							= $val['loc_e_ward'];
		$loc_e_borough 							= $val['loc_e_bugh'];
		if(!empty($loc_e_vill))
			$loc_e_vill_ward					= $val['loc_e_vill'];
		elseif(!empty($loc_e_ward))
			$loc_e_vill_ward					= $val['loc_e_ward'];
		elseif(!empty($loc_e_borough))
			$loc_e_vill_ward					= $val['loc_e_bugh'];
		
		$loc_e_ps								= $val['loc_e_ps'];
		$loc_e_pin_number						= $val['loc_e_pin_number'];
		
		/**************************************************/
		
		$e_postal_address 						= $val['e_postal_address'];
		$e_postal_dist 							= $val['e_postal_dist'];
		$e_postal_subdivision 					= $val['e_postal_subdivision'];
		$e_postal_areatype 						= $val['e_postal_areatype'];
		$e_postal_areatype_code 				= $val['e_postal_areatype_code'];
		$e_postal_vill 							= $val['e_postal_vill'];
		$e_postal_ward 							= $val['e_postal_ward'];
		$e_postal_borough 						= $val['e_postal_bugh'];
		if(!empty($e_postal_vill))
			$e_postal_vill_ward					= $val['e_postal_vill'];
		elseif(!empty($e_postal_ward))
			$e_postal_vill_ward					= $val['e_postal_ward'];
		elseif(!empty($e_postal_borough ))
			$e_postal_vill_ward					= $val['e_postal_bugh'];
		
		$e_postal_ps 							= $val['e_postal_ps'];
		$e_postal_pin_number					= $val['e_postal_pin_number'];
		$e_postal_pin_number					= $val['e_postal_pin_number'];
		
		
		
		/**************************************************/
		
		$e_full_name 							= $val['e_full_name'];
		$e_permanent_address 					= $val['e_permanent_address'];
		$e_permanent_add_dist 					= $val['e_permanent_add_dist'];
		$e_permanent_add_subdivision 			= $val['e_permanent_add_subdivision'];
		$e_permanent_add_areatype 				= $val['e_permanent_add_areatype'];
		$e_permanent_add_areatype_code 			= $val['e_permanent_add_areatype_code'];
		$e_permanent_add_vill 					= $val['e_permanent_add_vill'];
		$e_permanent_add_ward 					= $val['e_permanent_add_ward'];
		$e_permanent_add_borough 				= $val['e_permanent_add_bugh'];
		if(!empty($e_permanent_add_vill))
			$e_permanent_add_vill_ward			= $val['e_permanent_add_vill'];
		elseif(!empty($e_permanent_add_ward))
			$e_permanent_add_vill_ward			= $val['e_permanent_add_ward'];
		else
			$e_permanent_add_vill_ward			= $val['e_permanent_add_bugh'];
		
		$e_permanent_add_ps 					= $val['e_permanent_add_ps'];
		$e_permanent_add_pin_number				= $val['e_permanent_add_pin_number'];
		
		/*************************************************************************/
		$emp_name 							= $val['emp_name'];
		$emp_address 						= $val['emp_address'];
		$emp_dist 							= $val['emp_dist'];
		$emp_subdivision 					= $val['emp_subdivision'];
		$emp_areatype 						= $val['emp_areatype'];
		$emp_areatype_code 					= $val['emp_areatype_code'];
		$emp_vill 							= $val['emp_vill'];
		$emp_ward 							= $val['emp_ward'];
		$emp_borough 						= $val['emp_bugh'];
		$emp_sector							= $val['emp_sector'];
		$emp_not_area						= $val['emp_not_area'];
		if(!empty($emp_vill))
			$emp_vill_ward					= $val['emp_vill'];
		elseif(!empty($emp_ward))
			$emp_vill_ward					= $val['emp_ward'];
		elseif(!empty($emp_borough))
			$emp_vill_ward					= $val['emp_bugh'];
		elseif(!empty($emp_sector))
			$emp_vill_ward					= $val['emp_sector'];
		elseif(!empty($emp_not_area))
			$emp_vill_ward					= $val['emp_not_area'];
		
		
		$emp_ps 							= $val['emp_ps'];
		$emp_pin_number						= $val['emp_pin_number'];
		
		/**************************************************/
		
		$full_name_manager 						= $val['full_name_manager'];
		$address_manager 						= $val['address_manager'];
		$address_manager_dist 					= $val['address_manager_dist'];
		$address_manager_subdivision			= $val['address_manager_subdivision'];
		$address_manager_areatype 				= $val['address_manager_areatype'];
		$address_manager_areatype_code 			= $val['address_manager_areatype_code'];
		$address_manager_vill 					= $val['address_manager_vill'];
		$address_manager_ward 					= $val['address_manager_ward'];
		$address_manager_borough 				= $val['address_manager_bugh'];
		$address_manager_sector 				= $val['address_manager_sector'];
		$address_manager_notifiedarea 			= $val['address_manager_notifiedarea'];
		if(!empty($address_manager_vill))
			$address_manager_vill_ward			= $val['address_manager_vill'];
		elseif(!empty($address_manager_ward))
			$address_manager_vill_ward			= $val['address_manager_ward'];
		elseif(!empty($address_manager_borough))
			$address_manager_vill_ward			= $val['address_manager_bugh'];
		elseif(!empty($address_manager_sector))
			$address_manager_vill_ward			= $val['address_manager_sector'];
		elseif(!empty($address_manager_notifiedarea))
			$address_manager_vill_ward			= $val['address_manager_notifiedarea'];
		
		$address_manager_ps 					= $val['address_manager_ps'];
		$address_manager_pin_number				= $val['address_manager_pin_number'];
		
		/**************************************************/
		
		$nature_of_build_const   			    = $val['nature_of_build_const'];
		$max_no_of_building_workers_employed	= $val['max_no_of_building_workers_employed'];
		
		$date = str_replace('/', '-', $val['est_date_of_commencement_building']);  	
		$est_date_of_commencement_building = date('Y-m-d',strtotime($date));

		$date = str_replace('/', '-', $val['est_date_of_completion_building']);  
		$est_date_of_completion_building = date('Y-m-d',strtotime($date));
		
		
		
		$fieldsRegBocwa 						= array( 
														'user_id'									=>	$user_id,
														'identification_number'						=>	$identification_number,
														'act_id'									=>  $act_id,
														
														'e_name'									=>	$e_name,
														'est_type'									=>	$val['est_type'],
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
														'emp_gender'								=>	$val['emp_gender'],
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
														'address_manager_vill_ward'					=>	$address_manager_vill_ward ,
														'address_manager_ps'						=>	$address_manager_ps,
														'address_manager_pin_number'				=>  $address_manager_pin_number,
														
														'nature_of_build_const'						=>	$nature_of_build_const ,
														'max_no_of_building_workers_employed'		=>	$max_no_of_building_workers_employed,
														'est_date_of_commencement_building'			=>	$est_date_of_commencement_building ,
														'est_date_of_completion_building'			=>	$est_date_of_completion_building,
														);	
														
														
						
		if( !empty($fieldsRegBocwa) ){
			
					$query = db_update('l_bocwa_registration_master');
					$query->fields($fieldsRegBocwa); 
					$query->condition('id',$application_id);
					$query->condition('user_id',$user_id);
					$query->execute();	
			
			
					$message							=	'Application Updated Successfully';
					drupal_set_message(t($message));
			
	   }
		
}


function get_readable_applicant_bocwa_registration_form($application_id){
	
	global $base_root, $base_path, $user;
		
	$user_id = $user->uid;
			
	$master_part_bocwa_reg = db_select('l_bocwa_registration_master', 'lbrm');
	$master_part_bocwa_reg->fields('lbrm', array());
	
	/////////////////////////  Address Fetch ////////////////////////////////
		
	$master_part_bocwa_reg->fields('dm_loc', array('district_name'));
	$master_part_bocwa_reg->leftJoin('district_master', 'dm_loc', 'dm_loc.district_code=lbrm.loc_e_dist');
	
	$master_part_bocwa_reg->fields('sd_loc', array('sub_div_name'));
	$master_part_bocwa_reg->leftJoin('sub_division', 'sd_loc', 'sd_loc.sub_div_code=lbrm.loc_e_subdivision');
	
	$master_part_bocwa_reg->fields('bmm_loc', array('block_mun_name'));
	$master_part_bocwa_reg->leftJoin('block_mun_master', 'bmm_loc', 'bmm_loc.block_code=lbrm.loc_e_areatype_code');
	
	$master_part_bocwa_reg->fields('vwm_loc', array('village_name'));
	$master_part_bocwa_reg->leftJoin('village_ward_master', 'vwm_loc', 'vwm_loc.village_code=lbrm.loc_e_vill_ward');
	
	$master_part_bocwa_reg->fields('ps_loc', array('name_of_police_station'));
	$master_part_bocwa_reg->leftJoin('police_station', 'ps_loc', 'ps_loc.police_station_code=lbrm.loc_e_ps');
	
	//////////////////////////////  End 1 Location of Establishment/////////////////////////////////////
	
	$master_part_bocwa_reg->fields('dm_postal', array('district_name'));
	$master_part_bocwa_reg->leftJoin('district_master', 'dm_postal', 'dm_postal.district_code=lbrm.e_postal_dist');
	
	$master_part_bocwa_reg->fields('sd_postal', array('sub_div_name'));
	$master_part_bocwa_reg->leftJoin('sub_division', 'sd_postal', 'sd_postal.sub_div_code=lbrm.e_postal_subdivision');
	
	$master_part_bocwa_reg->fields('bmm_postal', array('block_mun_name'));
	$master_part_bocwa_reg->leftJoin('block_mun_master', 'bmm_postal', 'bmm_postal.block_code=lbrm.e_postal_areatype_code');
	
	$master_part_bocwa_reg->fields('vwm_postal', array('village_name'));
	$master_part_bocwa_reg->leftJoin('village_ward_master', 'vwm_postal', 'vwm_postal.village_code=lbrm.e_postal_vill_ward');
	
	$master_part_bocwa_reg->fields('ps_postal', array('name_of_police_station'));
	$master_part_bocwa_reg->leftJoin('police_station', 'ps_postal', 'ps_postal.police_station_code=lbrm.e_postal_ps');
	
	//////////////////////////// End 2 Postal Address of Establishment ENDS//////////////////////////////////////
	
	$master_part_bocwa_reg->fields('dm_per', array('district_name'));
	$master_part_bocwa_reg->leftJoin('district_master', 'dm_per', 'dm_per.district_code=lbrm.e_permanent_add_dist');
	
	$master_part_bocwa_reg->fields('sd_per', array('sub_div_name'));
	$master_part_bocwa_reg->leftJoin('sub_division', 'sd_per', 'sd_per.sub_div_code=lbrm.e_permanent_add_subdivision');
	
	$master_part_bocwa_reg->fields('bmm_per', array('block_mun_name'));
	$master_part_bocwa_reg->leftJoin('block_mun_master', 'bmm_per', 'bmm_per.block_code=lbrm.e_permanent_add_areatype_code');
	
	$master_part_bocwa_reg->fields('vwm_per', array('village_name'));
	$master_part_bocwa_reg->leftJoin('village_ward_master', 'vwm_per', 'vwm_per.village_code=lbrm.e_permanent_add_vill_ward');
	
	$master_part_bocwa_reg->fields('ps_per', array('name_of_police_station'));
	$master_part_bocwa_reg->leftJoin('police_station', 'ps_per', 'ps_per.police_station_code=lbrm.e_permanent_add_ps');
	
	
	//////////////////////////  End 5 ADDRESS OF MANAGER////////////////////////////////////////
	
	$master_part_bocwa_reg->fields('dm_man', array('district_name'));
	$master_part_bocwa_reg->leftJoin('district_master', 'dm_man', 'dm_man.district_code=lbrm.address_manager_dist');
	
	$master_part_bocwa_reg->fields('sd_man', array('sub_div_name'));
	$master_part_bocwa_reg->leftJoin('sub_division', 'sd_man', 'sd_man.sub_div_code=lbrm.address_manager_subdivision');
	
	$master_part_bocwa_reg->fields('bmm_man', array('block_mun_name'));
	$master_part_bocwa_reg->leftJoin('block_mun_master', 'bmm_man', 'bmm_man.block_code=lbrm.address_manager_areatype_code');
	
	$master_part_bocwa_reg->fields('vwm_man', array('village_name'));
	$master_part_bocwa_reg->leftJoin('village_ward_master', 'vwm_man', 'vwm_man.village_code=lbrm.address_manager_vill_ward');
	
	$master_part_bocwa_reg->fields('ps_man', array('name_of_police_station'));
	$master_part_bocwa_reg->leftJoin('police_station', 'ps_man', 'ps_man.police_station_code=lbrm.address_manager_ps');
	
	//////////////////////////  End 4 Address of the Employer////////////////////////////////////////
	
	$master_part_bocwa_reg->fields('dm_emp', array('district_name'));
	$master_part_bocwa_reg->leftJoin('district_master', 'dm_emp', 'dm_emp.district_code=lbrm.emp_dist');
	
	$master_part_bocwa_reg->fields('sd_emp', array('sub_div_name'));
	$master_part_bocwa_reg->leftJoin('sub_division', 'sd_emp', 'sd_emp.sub_div_code=lbrm.emp_subdivision');
	
	$master_part_bocwa_reg->fields('bmm_emp', array('block_mun_name'));
	$master_part_bocwa_reg->leftJoin('block_mun_master', 'bmm_emp', 'bmm_emp.block_code=lbrm.emp_areatype_code');
	
	$master_part_bocwa_reg->fields('vwm_emp', array('village_name'));
	$master_part_bocwa_reg->leftJoin('village_ward_master', 'vwm_emp', 'vwm_emp.village_code=lbrm.emp_vill_ward');
	
	$master_part_bocwa_reg->fields('ps_emp', array('name_of_police_station'));
	$master_part_bocwa_reg->leftJoin('police_station', 'ps_emp', 'ps_emp.police_station_code=lbrm.emp_ps');
	
	
	$master_part_bocwa_reg->condition('lbrm.user_id', $user_id);
	$master_part_bocwa_reg->condition('lbrm.id', $application_id);
	
	
	$master_part_bocwa_reg_result = $master_part_bocwa_reg->execute();  
	$content	=	$master_part_bocwa_reg_result->fetchAssoc();
	
	
	if($content['loc_e_areatype']=="B"){
			$text_AreaType_loc = "Block";
		}elseif($content['loc_e_areatype']=="M"){
			$text_AreaType_loc = "Municipality";
		}elseif($content['loc_e_areatype']=="C"){
			$text_AreaType_loc = "Corporation";
		}elseif($content['loc_e_areatype']=="S"){
			$text_AreaType_loc = "SEZ";
		}elseif($content['loc_e_areatype']=="N"){
			$text_AreaType_loc = "Notified Area";
		}elseif($content['loc_e_areatype']=="O"){
			$text_AreaType_loc = "Other";
		}
		
		if($text_AreaType_loc =='Corporation') {
			$text_AreaType_loc_name='Ward';
			$areatype_loc_name = $content['village_name'];
		}elseif($text_AreaType_loc=='Municipality'){
			$text_AreaType_loc_name='Ward';
			$areatype_loc_name = 'Ward-'.$content['village_name']; 
		}else {
			$text_AreaType_loc_name='Gram Panchayat';	
			$areatype_loc_name = ucwords($content['village_name']);
		}	
		
		if($content['e_postal_areatype']=="B"){
			$text_AreaType_postal = "Block";
		}elseif($content['e_postal_areatype']=="M"){
			$text_AreaType_postal = "Municipality";
		}elseif($content['e_postal_areatype']=="C"){
			$text_AreaType_postal = "Corporation";
		}elseif($content['e_postal_areatype']=="S"){
			$text_AreaType_postal = "SEZ";
		}elseif($content['e_postal_areatype']=="N"){
			$text_AreaType_postal = "Notified Area";
		}elseif($content['e_postal_areatype']=="O"){
			$text_AreaType_postal = "Other";
		}
		
		if($text_AreaType_postal =='Corporation') {
			$text_AreaType_postal_name='Ward';
			$areatype_postal_name = $content['vwm_postal_village_name'];
		}elseif($text_AreaType_postal=='Municipality'){
			$text_AreaType_postal_name='Ward';
			$areatype_postal_name = 'Ward-'.$content['vwm_postal_village_name']; 
		}else {
			$text_AreaType_postal_name='Gram Panchayat';	
			$areatype_postal_name = ucwords($content['vwm_postal_village_name']);
		}
		
		if($content['e_permanent_add_areatype']=="B"){
			$text_AreaType_per = "Block";
		}elseif($content['e_permanent_add_areatype']=="M"){
			$text_AreaType_per = "Municipality";
		}elseif($content['e_permanent_add_areatype']=="C"){
			$text_AreaType_per = "Corporation";
		}elseif($content['e_permanent_add_areatype']=="S"){
			$text_AreaType_per = "SEZ";
		}elseif($content['e_permanent_add_areatype']=="N"){
			$text_AreaType_per = "Notified Area";
		}elseif($content['e_permanent_add_areatype']=="O"){
			$text_AreaType_per = "Other";
		}
		
		if($text_AreaType_per =='Corporation') {
			$text_AreaType_per_name='Ward';
			$areatype_per_name = $content['vwm_per_village_name'];
		}elseif($text_AreaType_per=='Municipality'){
			$text_AreaType_per_name='Ward';
			$areatype_per_name = 'Ward-'.$content['vwm_per_village_name']; 
		}else {
			$text_AreaType_per='Gram Panchayat';	
			$areatype_per_name = ucwords($content['vwm_per_village_name']);
		}
		
		if($content['emp_areatype']=="B"){
			$text_AreaType_employer = "Block";
		}elseif($content['emp_areatype']=="M"){
			$text_AreaType_employer = "Municipality";
		}elseif($content['emp_areatype']=="C"){
			$text_AreaType_employer = "Corporation";
		}elseif($content['emp_areatype']=="S"){
			$text_AreaType_employer = "SEZ";
		}elseif($content['emp_areatype']=="N"){
			$text_AreaType_employer = "Notified Area";
		}elseif($content['emp_areatype']=="O"){
			$text_AreaType_employer = "Other";
		}
		
		if($text_AreaType_employer =='Corporation') {
			$text_AreaType_employer_name='Ward';
			$areatype_trade_name = $content['vwm_emp_village_name'];
		}elseif($text_AreaType_employer=='Municipality'){
			$text_AreaType_employer_name='Ward';
			$areatype_employer_name = 'Ward-'.$content['vwm_emp_village_name']; 
		}else {
			$text_AreaType_employer='Gram Panchayat';	
			$areatype_employer_name = ucwords($content['vwm_emp_village_name']);
		}
		
		if($content['address_manager_areatype']=="B"){
			$text_AreaType_manager = "Block";
		}elseif($content['address_manager_areatype']=="M"){
			$text_AreaType_manager = "Municipality";
		}elseif($content['address_manager_areatype']=="C"){
			$text_AreaType_manager = "Corporation";
		}elseif($content['address_manager_areatype']=="S"){
			$text_AreaType_manager = "SEZ";
		}elseif($content['address_manager_areatype']=="N"){
			$text_AreaType_manager = "Notified Area";
		}elseif($content['address_manager_areatype']=="O"){
			$text_AreaType_manager = "Other";
		}
		if($text_AreaType_manager =='Corporation') {
			$text_AreaType_manager_name='Ward';
			$areatype_manager_name = $content['vwm_man_village_name'];
		}elseif($text_AreaType_manager=='Municipality'){
			$text_AreaType_manager_name='Ward';
			$areatype_manager_name = 'Ward-'.$content['vwm_man_village_name']; 
		}else {
			$text_AreaType_manager_name='Gram Panchayat';	
			$areatype_manager_name = ucwords($content['vwm_man_village_name']);
		}
	
	
	$output='';
		$output.= '<div class="content">
						<div sky-form sky-form-mtw" style="width:100%;">
							<h3>BOCWA REGISTRATION FORM</h3>
							<fieldset>					
								<div class="row">
									<section class="col-lg-4 col-md-6">
										<label class="input">1.)Name of the Establishment 
											<input type="text" value="'.$content['e_name'].'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">1.1.) Location of the Establishment
											<input type="text" value="'.$content['loc_e_name'].'" disabled="disabled">
										</label>
									</section>
								</div>
								
								<div class="row">
									<section class="col-lg-4 col-md-6">
										<label class="input">1.1.a.) District of the Establishment
											<input type="text" value="'.$content['district_name'].'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">1.1.b.) Subdivision of the Establishment
											<input type="text" value="'.$content['sub_div_name'].'" disabled="disabled">
										</label>
									</section>
								</div>
								
								<div class="row">
									<section class="col-lg-4 col-md-6">
										<label class="input">1.1.c.) Area type of the Establishment
											<input type="text" value="'.$text_AreaType_loc.'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">1.1.d.) '.$text_AreaType_loc.' of the Establishment
											<input type="text" value="'.$content['block_mun_name'].'" disabled="disabled">
										</label>
									</section>
								</div>
								
								<div class="row">
									<section class="col-lg-4 col-md-6">
										<label class="input">1.1.e.) '.$text_AreaType_loc_name.'  of the Establishment
											<input type="text" value="'.$areatype_loc_name.'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">1.1.f.) Police Station of the Establishment
											<input type="text" value="'.$content['name_of_police_station'].'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">1.1.g.) Pin Number of the Establishment
											<input type="text" value="'.$content['loc_e_pin_number'].'" disabled="disabled">
										</label>
									</section>
								</div>
							</fieldset>
							
							<fieldset>
								<section>
									<label class="textarea">2.Postal Address of the Establishment
										<textarea rows="2" placeholder="Postal Address of the Establishment" disabled="disabled">'.$content['e_postal_address'].'</textarea>
									</label>
								</section>
									
								<div class="row">
									<section class="col-lg-4 col-md-6">
										<label class="input">2.a.) District of the Establishment
											<input type="text" value="'.$content['dm_postal_district_name'].'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">2.b.) Subdivision of the Establishment
											<input type="text" value="'.$content['sd_postal_sub_div_name'].'" disabled="disabled">
										</label>
									</section>
								</div>
								
								<div class="row">
									<section class="col-lg-4 col-md-6">
										<label class="input">2.c.) Area type of the Establishment
											<input type="text" value="'.$text_AreaType_postal.'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">2.d.) '.$text_AreaType_postal.' of the Establishment
											<input type="text" value="'.$content['bmm_postal_block_mun_name'].'" disabled="disabled">
										</label>
									</section>
								</div>
								
								<div class="row">
									<section class="col-lg-4 col-md-6">
										<label class="input">2.e.) '.$text_AreaType_postal_name.'  of the Establishment
											<input type="text" value="'.$areatype_postal_name.'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">2.f.) Police Station of the Establishment
											<input type="text" value="'.$content['ps_postal_name_of_police_station'].'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">2.g.) Pin Number of the Establishment
											<input type="text" value="'.$content['e_postal_pin_number'].'" disabled="disabled">
										</label>
									</section>
								</div>
							</fieldset>
							
							<fieldset>					
								<div class="row">
									<section class="col-lg-4 col-md-6">
										<label class="input">3.)Full Name of the Establishment
											<input type="text" value="'.$content['e_full_name'].'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">3.1.) Permanent Address of the Establishment
											<input type="text" value="'.$content['e_permanent_address'].'" disabled="disabled">
										</label>
									</section>
								</div>
								
								<div class="row">
									<section class="col-lg-4 col-md-6">
										<label class="input">3.1a.) District of the Establishment
											<input type="text" value="'.$content['dm_per_district_name'].'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">3.1b.) Subdivision of the Establishment
											<input type="text" value="'.$content['sd_per_sub_div_name'].'" disabled="disabled">
										</label>
									</section>
								</div>
								
								<div class="row">
									<section class="col-lg-4 col-md-6">
										<label class="input">3.1c.) Area type of the Establishment
											<input type="text" value="'.$text_AreaType_per.'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">3.1d.) '.$text_AreaType_per.' of the Establishment
											<input type="text" value="'.$content['bmm_per_block_mun_name'].'" disabled="disabled">
										</label>
									</section>
								</div>
								
								<div class="row">
									<section class="col-lg-4 col-md-64">
										<label class="input">3.1e.) '.$text_AreaType_per_name.'  of the Establishment
											<input type="text" value="'.$areatype_per_name.'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">3.1f.) Police Station of the Establishment
											<input type="text" value="'.$content['ps_per_name_of_police_station'].'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">3.1g.) Pin Number of the Establishment
											<input type="text" value="'.$content['e_permanent_add_pin_number'].'" disabled="disabled">
										</label>
									</section>
								</div>
							</fieldset>
							
							<fieldset>					
								<div class="row">
									<section class="col-lg-4 col-md-6">
										<label class="input">4.)Full Name of the Employer
											<input type="text" value="'.$content['e_full_name'].'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">4.1.) Address of the Employer
											<input type="text" value="'.$content['emp_address'].'" disabled="disabled">
										</label>
									</section>
								</div>
								
								<div class="row">
									<section class="col-lg-4 col-md-6">
										<label class="input">4.1a.) District of the Employer
											<input type="text" value="'.$content['dm_emp_district_name'].'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">3.1b.) Subdivision of the Employer
											<input type="text" value="'.$content['sd_emp_sub_div_name'].'" disabled="disabled">
										</label>
									</section>
								</div>
								
								<div class="row">
									<section class="col-lg-4 col-md-6">
										<label class="input">3.1c.) Area type of the Employer
											<input type="text" value="'.$text_AreaType_employer.'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">3.1d.) '.$text_AreaType_employer.' of the Employer
											<input type="text" value="'.$content['bmm_emp_block_mun_name'].'" disabled="disabled">
										</label>
									</section>
								</div>
								
								<div class="row">
									<section class="col-lg-4 col-md-64">
										<label class="input">3.1e.) '.$text_AreaType_employer_name.'  of the Employer
											<input type="text" value="'.$areatype_employer_name.'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">3.1f.) Police Station of the Employer
											<input type="text" value="'.$content['ps_emp_name_of_police_station'].'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">3.1g.) Pin Number of the Employer
											<input type="text" value="'.$content['emp_pin_number'].'" disabled="disabled">
										</label>
									</section>
								</div>
							</fieldset>
							
							<fieldset>					
								<div class="row">
									<section class="col-lg-4 col-md-6">
										<label class="input">4.)Full name of the Manager or Person Responsible for the Supervision  and control of the Establishment
											<input type="text" value="'.$content['full_name_manager'].'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">4.1.) Address of the Manager or Person Responsible for the Supervision and control of the Establishment
											<input type="text" value="'.$content['address_manager'].'" disabled="disabled">
										</label>
									</section>
								</div>
								
								<div class="row">
									<section class="col-lg-4 col-md-6">
										<label class="input">4.1.a.) District of the Manager or Person Responsible
											<input type="text" value="'.$content['dm_man_district_name'].'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">4.1.b.) Subdivision of the Manager or Person Responsible
											<input type="text" value="'.$content['sd_man_sub_div_name'].'" disabled="disabled">
										</label>
									</section>
								</div>
								
								<div class="row">
									<section class="col-lg-4 col-md-6">
										<label class="input">4.1.c.) Area type of the Manager or Person Responsible
											<input type="text" value="'.$text_AreaType_manager.'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">4.1.d.) '.$text_AreaType_manager.' of the Manager or Person Responsible
											<input type="text" value="'.$content['bmm_man_block_mun_name'].'" disabled="disabled">
										</label>
									</section>
								</div>
								
								<div class="row">
									<section class="col-lg-4 col-md-6">
										<label class="input">4.1.e.) '.$text_AreaType_manager_name.'  of the Manager or Person Responsible
											<input type="text" value="'.$areatype_manager_name.'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">4.1.f.) Police Station of the Manager or Person Responsible
											<input type="text" value="'.$content['ps_man_name_of_police_station'].'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">3.1g.) Pin Number of the Manager or Person Responsible
											<input type="text" value="'.$content['address_manager_pin_number'].'" disabled="disabled">
										</label>
									</section>									
								</div>
							</fieldset>
							
							<fieldset>
								<div class="row">
									<section class="col-lg-4 col-md-6">
										<label class="input">5.) Nature of building or other construction work is to be carried on
											<input type="text" value="'.$content['nature_of_build_const'].'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">6.) Maximum number of building workers to be employed on any day
											<input type="text" value="'.$content['max_no_of_building_workers_employed'].'" disabled="disabled">
										</label>
									</section>
								</div>
								
								<div class="row">
									<section class="col-lg-4 col-md-6">
										<label class="input">7.) Estimated date of commencement of building or other construction work
											<input type="text" value="'.date('dS M, Y',strtotime($content['est_date_of_commencement_building'])).'" disabled="disabled">
										</label>
									</section>
									<section class="col-lg-4 col-md-6">
										<label class="input">8.) Estimated date of the completion of building or other construction work
											<input type="text" value="'.date('dS M, Y',strtotime($content['est_date_of_completion_building'])).'" disabled="disabled">
										</label>
									</section>
								</div>
							</fieldset>
								
							
								<div class="row">&nbsp;&nbsp;</div><div class="row">&nbsp;&nbsp;</div>
							
						</div>
				 </div>';
	
		return $output; 
}

function get_bocwa_registration_info_form_ajax_callback($form, $form_state) {
		$commands = array();
		$commands[] = ajax_command_replace('#bocwa_subdvsn_replace', drupal_render($form['bocwa_reg_master']['loc_e_subdivision']));
		$commands[] = ajax_command_replace('#bocwa_mun_replace', drupal_render($form['bocwa_reg_master']['loc_e_mun']));
		$commands[] = ajax_command_replace('#bocwa_block_replace', drupal_render($form['bocwa_reg_master']['loc_e_block']));
		$commands[] = ajax_command_replace('#bocwa_other_replace', drupal_render($form['bocwa_reg_master']['loc_e_other']));
		$commands[] = ajax_command_replace('#bocwa_corp_replace', drupal_render($form['bocwa_reg_master']['loc_e_corp']));
		$commands[] = ajax_command_replace('#bocwa_sez_replace', drupal_render($form['bocwa_reg_master']['loc_e_sez']));
		$commands[] = ajax_command_replace('#bocwa_not_replace', drupal_render($form['bocwa_reg_master']['loc_e_not']));
		$commands[] = ajax_command_replace('#bocwa_vill_replace', drupal_render($form['bocwa_reg_master']['loc_e_vill']));
		$commands[] = ajax_command_replace('#bocwa_ward_replace', drupal_render($form['bocwa_reg_master']['loc_e_ward']));
		$commands[] = ajax_command_replace('#bocwa_bugh_replace', drupal_render($form['bocwa_reg_master']['loc_e_bugh']));
		$commands[] = ajax_command_replace('#bocwa_code_replace', drupal_render($form['bocwa_reg_master']['loc_e_areatype_code']));
		$commands[] = ajax_command_replace('#bocwa_ps_replace', drupal_render($form['bocwa_reg_master']['loc_e_ps']));
  		return array('#type' => 'ajax', '#commands' => $commands);
}

function get_bocwa_e_postal_address__ajax_callback($form, $form_state) {
		$commands = array();
		$commands[] = ajax_command_replace('#bocwa_e_postal_subdvsn_replace', drupal_render($form['bocwa_reg_master']['e_postal_subdivision']));
		$commands[] = ajax_command_replace('#bocwa_e_postal_block_replace', drupal_render($form['bocwa_reg_master']['e_postal_block']));
		$commands[] = ajax_command_replace('#bocwa_e_postal_mun_replace', drupal_render($form['bocwa_reg_master']['e_postal_mun']));
		$commands[] = ajax_command_replace('#bocwa_e_postal_corp_replace', drupal_render($form['bocwa_reg_master']['e_postal_corp']));
		$commands[] = ajax_command_replace('#bocwa_e_postal_sez_replace', drupal_render($form['bocwa_reg_master']['e_postal_sez']));
		$commands[] = ajax_command_replace('#bocwa_e_postal_not_replace', drupal_render($form['bocwa_reg_master']['e_postal_not']));
		$commands[] = ajax_command_replace('#bocwa_e_postal_other_replace', drupal_render($form['bocwa_reg_master']['e_postal_other']));
		$commands[] = ajax_command_replace('#bocwa_e_postal_vill_replace', drupal_render($form['bocwa_reg_master']['e_postal_vill']));
		$commands[] = ajax_command_replace('#bocwa_e_postal_ward_replace', drupal_render($form['bocwa_reg_master']['e_postal_ward']));
		$commands[] = ajax_command_replace('#bocwa_e_postal_bugh_replace', drupal_render($form['bocwa_reg_master']['e_postal_bugh']));
		$commands[] = ajax_command_replace('#bocwa_e_postal_code_replace', drupal_render($form['bocwa_reg_master']['e_postal_code']));
		$commands[] = ajax_command_replace('#bocwa_e_postal_ps_replace', drupal_render($form['bocwa_reg_master']['e_postal_ps']));
  		return array('#type' => 'ajax', '#commands' => $commands);
}

function get_bocwa_e_permanent_address_ajax_callback($form, $form_state) {
		$commands = array();
		$commands[] = ajax_command_replace('#bocwa_e_permanent_add_subdvsn_replace', drupal_render($form['bocwa_reg_master']['e_permanent_add_subdivision']));
		$commands[] = ajax_command_replace('#bocwa_e_permanent_add_block_replace', drupal_render($form['bocwa_reg_master']['e_permanent_add_block']));
		$commands[] = ajax_command_replace('#bocwa_e_permanent_add_mun_replace', drupal_render($form['bocwa_reg_master']['e_permanent_add_mun']));
		$commands[] = ajax_command_replace('#bocwa_e_permanent_add_corp_replace', drupal_render($form['bocwa_reg_master']['e_permanent_add_corp']));
		$commands[] = ajax_command_replace('#bocwa_e_permanent_add_sez_replace', drupal_render($form['bocwa_reg_master']['e_permanent_add_sez']));
		$commands[] = ajax_command_replace('#bocwa_e_permanent_add_not_replace', drupal_render($form['bocwa_reg_master']['e_permanent_add_not']));
		$commands[] = ajax_command_replace('#bocwa_e_permanent_add_other_replace', drupal_render($form['bocwa_reg_master']['e_permanent_add_other']));
		$commands[] = ajax_command_replace('#bocwa_e_permanent_add_vill_replace', drupal_render($form['bocwa_reg_master']['e_permanent_add_vill']));
		$commands[] = ajax_command_replace('#bocwa_e_permanent_add_ward_replace', drupal_render($form['bocwa_reg_master']['e_permanent_add_ward']));
		$commands[] = ajax_command_replace('#bocwa_e_permanent_add_bugh_replace', drupal_render($form['bocwa_reg_master']['e_permanent_add_bugh']));
		$commands[] = ajax_command_replace('#bocwa_e_permanent_add_code_replace', drupal_render($form['bocwa_reg_master']['e_permanent_add_code']));
		$commands[] = ajax_command_replace('#bocwa_e_permanent_add_ps_replace', drupal_render($form['bocwa_reg_master']['e_permanent_add_ps']));
  		return array('#type' => 'ajax', '#commands' => $commands);
}

function get_edit_bocwa_emp_ajax_callback($form, $form_state) {
		$commands = array();
		$commands[] = ajax_command_replace('#bocwa_emp_subdvsn_replace', drupal_render($form['bocwa_reg_master']['emp_subdivision']));
		$commands[] = ajax_command_replace('#bocwa_emp_block_replace', drupal_render($form['bocwa_reg_master']['emp_block']));
		$commands[] = ajax_command_replace('#bocwa_emp_mun_replace', drupal_render($form['bocwa_reg_master']['emp_mun']));
		$commands[] = ajax_command_replace('#bocwa_emp_corp_replace', drupal_render($form['bocwa_reg_master']['emp_corp']));
		$commands[] = ajax_command_replace('#bocwa_emp_sez_replace', drupal_render($form['bocwa_reg_master']['emp_sez']));
		$commands[] = ajax_command_replace('#bocwa_emp_not_replace', drupal_render($form['bocwa_reg_master']['emp_not']));
		$commands[] = ajax_command_replace('#bocwa_emp_other_replace', drupal_render($form['bocwa_reg_master']['emp_other']));
		$commands[] = ajax_command_replace('#bocwa_emp_vill_replace', drupal_render($form['bocwa_reg_master']['emp_vill']));
		$commands[] = ajax_command_replace('#bocwa_emp_ward_replace', drupal_render($form['bocwa_reg_master']['emp_ward']));
		$commands[] = ajax_command_replace('#bocwa_emp_bugh_replace', drupal_render($form['bocwa_reg_master']['emp_bugh']));
		$commands[] = ajax_command_replace('#bocwa_emp_sector_replace', drupal_render($form['bocwa_reg_master']['emp_sector']));
		$commands[] = ajax_command_replace('#bocwa_emp_not_area_replace', drupal_render($form['bocwa_reg_master']['emp_not_area']));		
		$commands[] = ajax_command_replace('#bocwa_emp_code_replace', drupal_render($form['bocwa_reg_master']['emp_areatype_code']));
		$commands[] = ajax_command_replace('#bocwa_emp_ps_replace', drupal_render($form['bocwa_reg_master']['emp_ps']));
  		return array('#type' => 'ajax', '#commands' => $commands);
}



function get_bocwa_address_manager_ajax_callback($form, $form_state) {
		$commands = array();
		$commands[] = ajax_command_replace('#bocwa_address_manager_subdvsn_replace', drupal_render($form['bocwa_reg_master']['address_manager_subdivision']));
		$commands[] = ajax_command_replace('#bocwa_address_manager_block_replace', drupal_render($form['bocwa_reg_master']['address_manager_block']));
		$commands[] = ajax_command_replace('#bocwa_address_manager_mun_replace', drupal_render($form['bocwa_reg_master']['address_manager_mun']));
		$commands[] = ajax_command_replace('#bocwa_address_manager_corp_replace', drupal_render($form['bocwa_reg_master']['address_manager_corp']));
		$commands[] = ajax_command_replace('#bocwa_address_manager_sez_replace', drupal_render($form['bocwa_reg_master']['address_manager_sez']));
		$commands[] = ajax_command_replace('#bocwa_address_manager_not_replace', drupal_render($form['bocwa_reg_master']['address_manager_not']));
		$commands[] = ajax_command_replace('#bocwa_address_manager_other_replace', drupal_render($form['bocwa_reg_master']['address_manager_other']));
		$commands[] = ajax_command_replace('#bocwa_address_manager_vill_replace', drupal_render($form['bocwa_reg_master']['address_manager_vill']));
		$commands[] = ajax_command_replace('#bocwa_address_manager_ward_replace', drupal_render($form['bocwa_reg_master']['address_manager_ward']));
		$commands[] = ajax_command_replace('#bocwa_address_manager_bugh_replace', drupal_render($form['bocwa_reg_master']['address_manager_bugh']));
		$commands[] = ajax_command_replace('#bocwa_address_manager_sector_replace', drupal_render($form['bocwa_reg_master']['address_manager_sector']));
		$commands[] = ajax_command_replace('#bocwa_address_manager_notifiedarea_replace', drupal_render($form['bocwa_reg_master']['address_manager_notifiedarea']));
		$commands[] = ajax_command_replace('#bocwa_address_manager_code_replace', drupal_render($form['bocwa_reg_master']['address_manager_code']));
		$commands[] = ajax_command_replace('#bocwa_address_manager_ps_replace', drupal_render($form['bocwa_reg_master']['address_manager_ps']));
  		return array('#type' => 'ajax', '#commands' => $commands);
}