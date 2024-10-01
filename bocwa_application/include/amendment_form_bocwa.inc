<?php
drupal_add_library('system','ui.datepicker');
//drupal_add_js(drupal_get_path('module', 'ismw_license') . '/js/custom.js');
drupal_add_js(drupal_get_path('module', 'bocwa_application') . '/js/bocwa_upload_custom.js');
function amendment_bocwa_form($form, &$form_state, $id=''){
    global $base_root,$base_path,$user; 
	$user_id 		= $user->uid;
	$application_id	= encryption_decryption_fun('decrypt',$id);
	
	$getBocwa = db_select('l_bocwa_registration_master', 'lcrm');
	$getBocwa->fields('lcrm', array());
	$getBocwa->condition('lcrm.id', trim($application_id));
	$getBocwa->orderBy('lcrm.id','DESC');
	$getBocwa->range(0, 1);
	$bocwaData = $getBocwa->execute()->fetchAssoc();

			 

    if(!empty($bocwaData) ){
        $identification_number 	= $bocwaData['identification_number'];
		$e_name					= $bocwaData['e_name'];
		$est_type				= $bocwaData['est_type'];
		$loc_e_name				= $bocwaData['loc_e_name'];
		$loc_e_dist				= $bocwaData['loc_e_dist'];
		$loc_e_subdivision 		= $bocwaData['loc_e_subdivision'];
		$loc_e_areatype			= $bocwaData['loc_e_areatype'];
		$loc_e_areacode 		= $bocwaData['loc_e_areatype_code'];
		$loc_e_villward 		= $bocwaData['loc_e_vill_ward'];
		$loc_e_borough 			= $bocwaData['loc_e_vill_ward'];
		$loc_e_ps 				= $bocwaData['loc_e_ps'];
		$loc_e_pin 				= $bocwaData['loc_e_pin_number'];
		
		$e_postal_address 		= $bocwaData['e_postal_address'];
		$e_postal_dist 			= $bocwaData['e_postal_dist'];
		$e_postal_subdivision 	= $bocwaData['e_postal_subdivision'];
		$e_postal_areatype 		= $bocwaData['e_postal_areatype'];
		$e_postal_areacode 		= $bocwaData['e_postal_areatype_code'];
		$e_postal_villward		= $bocwaData['e_postal_vill_ward'];
		$e_postal_ps			= $bocwaData['e_postal_ps'];
		$e_postal_pin			= $bocwaData['e_postal_pin_number'];
			
		$est_full_name			= $bocwaData['e_full_name'];
		$est_address			= $bocwaData['e_permanent_address'];
		$est_dist				= $bocwaData['e_permanent_add_dist'];
		$est_subdivision		= $bocwaData['e_permanent_add_subdivision'];
		$est_areatype			= $bocwaData['e_permanent_add_areatype'];
		$est_areacode			= $bocwaData['e_permanent_add_areatype_code'];
		$est_villward			= $bocwaData['e_permanent_add_vill_ward'];
		$est_ps					= $bocwaData['e_permanent_add_ps'];
		$est_pin				= $bocwaData['e_permanent_add_pin_number'];
		
		$full_name_manager		= $bocwaData['full_name_manager'];
		$manager_country		= $bocwaData['manager_country']!='' ? $bocwaData['manager_country'] : 1;
		$manager_state			= $bocwaData['manager_state']!='' ? $bocwaData['manager_state'] : 1;
		$manager_address		= $bocwaData['address_manager'];
		$manager_dist			= $bocwaData['address_manager_dist'];
		$manager_subdivision	= $bocwaData['address_manager_subdivision'];
		$manager_areatype 		= $bocwaData['address_manager_areatype'];
		$manager_areacode		= $bocwaData['address_manager_areatype_code'];
		$manager_villward		= $bocwaData['address_manager_vill_ward'];
		$manager_ps				= $bocwaData['address_manager_ps'];
		$manager_pin			= $bocwaData['address_manager_pin_number'];
		
		$emp_name  				= $bocwaData['emp_name'];
		$emp_gender  			= $bocwaData['emp_gender'];
		$emp_address 			= $bocwaData['emp_address'];
		$emp_country			= $bocwaData['emp_country']!='' ? $bocwaData['emp_country'] : 1;
		$emp_state				= $bocwaData['emp_state']!='' ? $bocwaData['emp_state'] : 1;
		$emp_dist 				= $bocwaData['emp_dist'];
		$emp_subdivision		= $bocwaData['emp_subdivision'];
		$emp_areatype 			= $bocwaData['emp_areatype'];
		$emp_areacode 			= $bocwaData['emp_areatype_code'];
		$emp_villward 			= $bocwaData['emp_vill_ward'];
		$emp_ps 				= $bocwaData['emp_ps'];
		$emp_pin				= $bocwaData['emp_pin_number'];
		
		$nature_of_build_const	= $bocwaData['nature_of_build_const'];
		$max_workers_employed	= $bocwaData['max_no_of_building_workers_employed'];
		$commencement			= date('d/m/Y',strtotime($bocwaData['est_date_of_commencement_building']));
		$completion				= date('d/m/Y',strtotime($bocwaData['est_date_of_completion_building']));
		$final_submit_status    = $bocwaData['final_submit_status'];
		$status      			= $bocwaData['status'];
		$backlog_id				= $bocwaData['backlog_id'];

		$readonly_e_postal_pin = $readonly_e_permanent_pin =  $readonly_emp_pin = $readonly_manager_pin = array('maxlength' => 6 ,'class' => array('correction_view'));
		$readonly_e_name =$readonly_postal_address = $readonly_e_full_name = $readonly_e_permanent_address = $readonly_emp_info = $readonlyAddressEmployerTextarea = $readonly_manager_info = $readonlyAddressManagerTextarea = $readonly_nature_wrk = $readonly_max_workers =  array('class' => array('correction_view'));
		$readonly_commencement = $readonly_completion = array('readonly'=>'readonly','class' => array('correction_view'));
		$query2 = db_select('l_bocwa_amendment', 'lram')
				 ->	fields('lram', array('not_amended_fields'))
				 ->	condition('lram.applicant_uid', $user_id)
				 ->	range(0, 1)
				 ->	orderBy('id', 'DESC');
		$notAmendedData = $query2->execute()->fetchAssoc();
		
		if($notAmendedData !=""){
			$notAmendedField= explode(',', $notAmendedData['not_amended_fields']);
			
			foreach($notAmendedField as $notAmended){ 
				
				switch($notAmended){	
					
					case "e_name":
							$readonly_e_name 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
							
					case "postal_address":
							
							$readonly_postal_address 		= array('readonly' => 'readonly', 'class' => array('readonly_view'));
							$readonly_e_postal_pin			= array('disabled' => 'disabled', 'maxlength' => 6, 'class' => array('readonly_view'));
							break;
							
					case "e_permanent_address":
					
							$readonly_e_full_name 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_permanent_address 		= array('readonly' => 'readonly', 'class' => array('readonly_view'));
							$readonly_e_permanent_pin		= array('disabled' => 'disabled','maxlength' => 6,  'class' => array('readonly_view'));	
							break;
							
					case "emp_info":
					
							$readonly_emp_info 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_AddressEmployerTextarea= array('readonly' => 'readonly', 'class' => array('readonly_view'));
							$readonly_emp_pin				= array('disabled' => 'disabled','maxlength' => 6,  'class' => array('readonly_view'));	
							break;
									
							
					case "manager_info":
					
							$readonly_manager_info 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_AddressManagerTextarea = array('readonly' => 'readonly', 'class' => array('readonly_view'));
							$readonly_manager_pin			= array('disabled' => 'disabled','maxlength' => 6,  'class' => array('readonly_view'));	
							break;
							
					case "e_nature_of_work":
						   
							$readonly_nature_wrk 			= array('disabled' => 'disabled', 'class' => array('readonly_view')) ;
							break;
							
					case "max_num_of_workmen":
							
							$readonly_max_workers			= array('disabled' => 'disabled', 'class' => array('readonly_view')) ;
							break;
							
					case "est_date_comm":
							
							$readonly_commencement 			= array('disabled' => 'disabled', 'class' => array('readonly_view')) ;
							break;
							
					case "est_date_completion":
						   
							$readonly_completion			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
							
					default:
					
				}
			}
		}
			

		if( $status == 'B' ) { 
			
			$query1 = db_select('l_bocwa_remark_master', 'lbrm')
						 ->	fields('lbrm', array('remark_field_title'))
						 ->	condition('lbrm.application_id', $application_id)
						 ->condition('lbrm.remark_by', $user_id, '!=')
						 ->	condition('lbrm.is_active', 1)
						 ->	orderBy('id', 'DESC');
						 
			$remark_text = $query1->execute()->fetchAssoc();
			$fieldArr = explode(',', $remark_text['remark_field_title']);
			foreach($fieldArr as $fields){ 
			
				switch($fields){	
					
					case "e_name":
							$readonly_e_name 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
							
					case "est_type":
							$readonly_est_type 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
							
					case "postal_address":
							
							$readonly_postal_address 		= array('readonly' => 'readonly', 'class' => array('readonly_view'));
							$readonly_e_postal_pin			= array('disabled' => 'disabled', 'maxlength' => 6, 'class' => array('readonly_view'));
							break;
							
					case "e_permanent_address":
					
							$readonly_e_full_name 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_permanent_address 	= array('readonly' => 'readonly', 'class' => array('readonly_view'));
							$readonly_e_permanent_pin		= array('disabled' => 'disabled','maxlength' => 6,  'class' => array('readonly_view'));	
							break;
							
					case "emp_info":
					
							$readonly_emp_info 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonlyAddressEmployerTextarea= array('readonly' => 'readonly', 'class' => array('readonly_view'));
							$readonly_emp_pin				= array('disabled' => 'disabled','maxlength' => 6,  'class' => array('readonly_view'));	
							break;
									
					case "emp_gender":
							$readonly_emp_gender 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;	
								
					case "manager_info":
					
							$readonly_manager_info 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonlyAddressManagerTextarea = array('readonly' => 'readonly', 'class' => array('readonly_view'));
							$readonly_manager_pin			= array('disabled' => 'disabled','maxlength' => 6,  'class' => array('readonly_view'));	
							break;
							
					case "e_nature_of_work":
						
							$readonly_nature_wrk 			= array('disabled' => 'disabled', 'class' => array('readonly_view')) ;
							break;
							
					case "max_num_of_workmen":
							
							$readonly_max_workers			= array('disabled' => 'disabled', 'class' => array('readonly_view')) ;
							break;
							
					case "est_date_comm":
							
							$readonly_commencement 			= array('disabled' => 'disabled', 'class' => array('readonly_view')) ;
							break;
							
					case "est_date_completion":
						
							$readonly_completion			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
							
					default:
					
				}
		    }	
			
		}

		$check_flag = db_select('l_bocwa_registration_master', 'lcrm');
		$check_flag->fields('lcrm', array('amendment_parent_id'));
		$check_flag->condition('lcrm.id', trim($application_id));
		$fetch_check_flag = $check_flag->execute()->fetchObject(); 
		
		$form['header']						 = array(
			'#type' 			=> 'markup',
			'#markup' 			=> '<div class="content"><div class="reset-amd-fields">',
		);	

		$form['bocwa_reg_master']['markup_link']=	array(
					'#type' 			=> 'markup',
					'#markup'			=> l('Reset Amendment Fields', 'apply-bocwa/'.$id, array('attributes' => array('html' => TRUE, 'class' => 'link-btn mtop-btm-15# pull-right btn-reset-amd-fields')))
				);															
		
					
		$form['bocwa_reg_master']['application_id'] = array(
					'#type' 			=>  'hidden',
					'#default_value'	=>  $id,
					'#attributes' 		=>  array('readonly'=>'readonly')
					);
					
		$form['bocwa_reg_master']['e_name'] = array(																				  
					'#prefix' 		 	 =>  '<br class="clear"></div><div class="panel panel-default panel-form"><div class="panel-heading">1. Name and location (Work Site) of the Establishment</div><div class="panel-body"><div class="col-lg-4 col-md-4"><label class="input">',
					'#suffix' 		 	 =>  '</label></div>',
					'#title'			 =>  '1. Name of the Establishment',
					'#type' 			 =>  'textfield',
					'#default_value' 	 =>  !empty($e_name) ? $e_name : '',
					'#required' 		 =>  TRUE,
					'#attributes'		 =>  $readonly_e_name,
					'#element_validate'  =>  array('MISC_TEXTBOX_EST_VALIDATE')
					);
		$form['bocwa_reg_master']['est_type'] = array(
					'#title' 			=> '1.(a) Establishment Type',
					'#type' 			=> 'select',
					'#options' 			=> array('micro' => 'Micro', 'small' => 'Small', 'medium' => 'Medium', 'large' => 'Large'),
					'#required' 		=>  TRUE,
					'#default_value' 	=> $est_type,
					'#attributes'		=> $readonly_est_type,
					'#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="select">',
					'#suffix' 			=> '</label></div>'
				);
							
		$form['bocwa_reg_master']['loc_e_name'] = array(
					'#prefix' 		 	 =>  '<div class="col-lg-4 col-md-8 min-height-80"><label class="input">',
					'#suffix' 		 	 =>  '</label></div>',
					'#title'			 =>  '1.(b) Location of the Establishment',
					'#type' 			 =>  'textfield',
					'#default_value' 	 =>  !empty($loc_e_name) ? $loc_e_name : '',
					'#required' 		 =>  TRUE,
					//'#attributes'		 =>  array('disabled' => 'disabled', 'class' => array('readonly_view')),
					'#element_validate'  => array('MISC_TEXTBOX_LOCATION_VALIDATE')
					);
				
		$form['bocwa_reg_master']['loc_e_district']	= array(
					'#prefix' 			 => '<div id="district_div" class="col-md-4"><label class="select">',
					'#suffix' 			 => '</label></div>',
					'#title'			 => t('1.(c) Select District'),
					'#type' 			 => 'select',
					'#options' 			 => common_custom_user_district_list_new(),
					'#default_value'	 => !empty($loc_e_dist) ? $loc_e_dist : '',
					'#attributes'		 => array('disabled' => 'disabled', 'class' => array('readonly_view')),
					'#required' 		 => TRUE,
					'#ajax'				 => array(
											'event'		=> 'change',
											'effect' 	=> 'fade',
											'callback'	=> 'my_ajax_callback',
											'progress'	=> array('type'=> 'throbber'),
											)
					);

				
		$value_district = isset($form_state['values']['loc_e_district']) ? $form_state['values']['loc_e_district'] :$loc_e_dist;

		if($value_district != '' ){
					
			$db_subdivision_value	= db_query("SELECT COUNT(id) as subdv_total_manager  FROM {sub_division} WHERE district_code = '".$value_district."'")->fetchObject();
				
			if(!empty($db_subdivision_value) && $db_subdivision_value != '' && $db_subdivision_value->subdv_total_manager > 0) {
				
				$form['bocwa_reg_master']['loc_e_subdivision']= array(
														'#prefix' 		=> '<div id="subdivision_div" class="col-md-4"><label class="select">',
														'#suffix' 		=> '</label></div>',
														'#title'		=> '1.(c) Select Subdivision',
														'#type' 		=> 'select',
														'#default_value'=> !empty($loc_e_subdivision) ? $loc_e_subdivision : '',
														'#options' 		=> common_custom_user_subdivisionlist_list_new($value_district),
														'#id'			=> 'loc_e_subdivision_id',
														'#attributes'	=>  array('disabled' => 'disabled', 'class' => array('readonly_view')),
														'#required' 	=> TRUE,
														'#ajax'			=> array(
																			'event' 			=> 'change',
																			'effect' 			=> 'fade',			
																			'callback'			=> 'my_ajax_callback',
																			'progress'			=> array('type'=> 'throbber'),
																			),
														);
													
				$value_subdivision	= isset($form_state['values']['loc_e_subdivision']) ? $form_state['values']['loc_e_subdivision'] : $loc_e_subdivision;
			}
		}
		
		if($value_district != "" && $value_subdivision != ""){
				
			$db_block_mun_value	 = db_query("SELECT COUNT(id) as areatype_total FROM {block_mun_master} WHERE distict_code = '".$value_district."' and sub_division_code  = '".$value_subdivision."'")->fetchObject();
			
			if(!empty($db_block_mun_value) &&  $db_block_mun_value != '' && $db_block_mun_value->areatype_total > 0){
				
				$areatypeOptions	= common_custom_user_areatype_list_new($value_district, $value_subdivision);
					
				if(!empty($areatypeOptions)){
						
						$form['bocwa_reg_master']['loc_e_areatype'] = array(
														'#prefix' 		=> '<div id="area_type_div" class="col-md-4"><label class="select">',
														'#suffix' 		=> '</label></div>',
														'#title'			=> '3.(e)Select Block/Municipality/Corporation/SEZ/NA',
														'#type' 			=> 'select',
														'#options' 		=> $areatypeOptions,
														'#default_value'	=> !empty($loc_e_areatype) ? $loc_e_areatype : '',
														'#attributes'		 =>  array('disabled' => 'disabled', 'class' => array('readonly_view')),
														'#required' 		=> TRUE,
														'#ajax'			=> array(
																			'event' 			=> 'change',
																			'effect' 			=> 'fade',			
																			'callback'			=> 'my_ajax_callback',
																			'progress'			=> array('type'=> 'throbber'),
																			),
													);
					$value_area_type 	= isset($form_state['values']['loc_e_areatype']) ? $form_state['values']['loc_e_areatype'] : $loc_e_areatype;
				}
			}
		}
		
		if ($value_district != "" && $value_subdivision != "" && $value_area_type != "") {
							
			if ( $value_area_type == 'B' ){
				$areatype_name	= 'Block';
			}elseif( $value_area_type == 'M'){
				$areatype_name	= 'Municipality';
			}elseif( $value_area_type == 'C'){
				$areatype_name	= 'Corporation';
			}elseif( $value_area_type == 'S'){
				$areatype_name	= 'SEZ';
			}elseif($value_area_type == 'N'){
				$areatype_name	= 'Notified Area';
			}
			
			$form['bocwa_reg_master']['loc_e_areacode'] = array(
														'#prefix' 		=> '<div id="area_code_div" class="col-md-4"><label class="select">',
														'#suffix' 		=> '</label></div>',
														'#title'			=> '3.(f) Select '.$areatype_name,
														'#type' 			=> 'select',
														'#options' 		=> common_custom_user_block_list_new($value_subdivision, $value_area_type),
														'#default_value'	=> !empty($loc_e_areacode) ? $loc_e_areacode : '',
														'#attributes'		=>  array('disabled' => 'disabled', 'class' => array('readonly_view')),
														'#required' 		=> TRUE,
														'#ajax'			=> array(
																			'event' 			=> 'change',
																			'effect' 			=> 'fade',			
																			'callback'			=> 'my_ajax_callback',
																			'progress'			=> array('type'=> 'throbber'),
																			)
											);
															
			$value_area_code 	= isset($form_state['values']['loc_e_areacode']) ? $form_state['values']['loc_e_areacode'] : $loc_e_areacode;
		}
		
		if($value_district != "" && $value_subdivision != "" && $value_area_type != "" && $value_area_code != ""){
				
			if ( $value_area_type == 'B' ){
				$area_name			= 'Gram Panchayat';
				$areatypecode 		= 'V';
			}elseif( $value_area_type == 'M'){
				$area_name			= 'Ward';
				$areatypecode 		= 'W';
			}elseif( $value_area_type == 'C'){
				$area_name			= 'Ward';
				$areatypecode 		= 'C';
			}elseif( $value_area_type == 'S'){
				$area_name			= 'Sector';
				$areatypecode 		= 'S';
			}elseif($value_area_type == 'N'){
				$area_name			= 'Notified Area';
				$areatypecode 		= 'N';
			}
				
			$form['bocwa_reg_master']['loc_e_villward'] = array(
														'#prefix' 		=> '<div id="village_div" class="col-md-4"><label class="select">',
														'#suffix' 		=> '</label></div>',
														'#title'			=> '3.(g) Select '.$area_name,
														'#type' 			=> 'select',
														'#options'		=> common_custom_user_villward_list_new($value_area_code,$areatypecode),
														'#default_value'	=> !empty($loc_e_villward) ? $loc_e_villward : '',
														'#attributes'		=>  array('disabled' => 'disabled', 'class' => array('readonly_view')),
														'#required' 		=> TRUE,
														'#ajax'			=> array(
																				'event' 			=> 'change',
																				'effect' 			=> 'fade',			
																				'callback'			=> 'my_ajax_callback',
																				'progress'			=> array('type'=> 'throbber'),
																				)
													);		
			$value_village 	= isset($form_state['values']['loc_e_villward']) ? $form_state['values']['loc_e_villward'] : $loc_e_villward;	
		}
		
		if ($value_district != "" && $value_subdivision != "" && $value_area_type != "" && $value_area_code != ""  && $value_village != "" ){
				
			$form['bocwa_reg_master']['loc_e_ps'] = array(
														'#prefix' 		=> '<div id="police_station_div" class="col-md-4"><label class="select">',
														'#suffix' 		=> '</label></div>',
														'#title'			=> '3.(h) Select Police Station',
														'#type' 			=> 'select',
														'#options' 		=> common_custom_user_ps_list_new($value_district),
														'#attributes'		=>  array('disabled' => 'disabled', 'class' => array('readonly_view')),
														'#default_value'	=> !empty($loc_e_ps) ? $loc_e_ps : '',
														'#required' 		=> TRUE,
													);
																									
																									
			$form['bocwa_reg_master']['loc_e_pin'] 	= array(
														'#prefix' 		=> '<div id="pin_code_div" class="col-md-4"><label class="input">',
														'#suffix' 		=> '</label></div>',
														'#title'			=> '3.(i) PIN Code',
														'#type' 			=> 'textfield',
														'#required' 		=> TRUE,
														'#attributes'		=>  array('disabled' => 'disabled', 'class' => array('readonly_view')),
														'#default_value'	=> !empty($loc_e_pin) ? $loc_e_pin : '',
														'#element_validate'=> array('MISC_PIN_NUMBER'),
													);	
		}

		$form['bocwa_reg_master']['e_postal_address'] = array(
			'#prefix' 			=> '</div></div><div class="panel panel-default panel-form"><div class="panel-heading">2. Registered Office address of the Establishment </div><div class="panel-body"><div class="col-md-8"><label class="input">',
			'#suffix' 			=>  '</label></div>',
			'#title'			=>  '2.(a) Location',
			'#default_value' 	=>  !empty($e_postal_address) ? $e_postal_address : '',
			'#type' 			=>  'textfield',
			'#required' 		=>  TRUE,
			'#attributes'		=>  $readonly_postal_address,
			'#element_validate'	=>  array('MISC_TEXTBOX_LOCATION_VALIDATE'),
			);
			
		$form['bocwa_reg_master']['e_postal_dist'] = array(
				'#prefix' 			=> '<div class="col-md-4" id="postal_district_div"><label class="select">',
				'#suffix' 			=> '</label></div>',
				'#title'			=> '2.(b) Select District',
				'#type' 			=> 'select',
				'#options' 			=> common_custom_user_district_list_new(),
				'#attributes'		=> $readonly_postal_address,
				'#default_value' 	=> !empty($e_postal_dist) ? $e_postal_dist: '',
				'#required' 		=> TRUE,
				'#ajax'				=> array(
												'event' 			=> 'change',
												'effect' 			=> 'fade',			
												'callback'			=> 'get_ajax_callback_e_postal',
												'progress'			=> array('type'=> 'throbber'),
											)
				);
		$form['bocwa_reg_master']['e_postal_subdivision'] = array(
			'#prefix' 			=> '<div id="postal_subdivision_div">',
			'#suffix'			=> '</div>',
			);
		$form['bocwa_reg_master']['e_postal_areatype'] = array(
			'#prefix' 			=> '<div id="postal_areatype_div">',
			'#suffix'			=> '</div>',
			);
		$form['bocwa_reg_master']['e_postal_areacode'] = array(
			'#prefix' 			=> '<div id="postal_areacode_div">',
			'#suffix'			=> '</div>',
			);				
		$form['bocwa_reg_master']['e_postal_villward'] = array(
			'#prefix' 			=> '<div id="postal_vill_div">',
			'#suffix'			=> '</div>',
			);
		$form['bocwa_reg_master']['e_postal_ps'] = array(
			'#prefix' 			=> '<div id="postal_ps_div">',
			'#suffix'			=> '</div>',
			);	
			$form['bocwa_reg_master']['e_postal_pin'] = array(
			'#prefix' 			=> '<div id="postal_pin_div">',
			'#suffix'			=> '</div>',
			);	
		$value_postal_dist 	= 	isset($form_state['values']['e_postal_dist']) ? $form_state['values']['e_postal_dist'] : $e_postal_dist;
		if($value_postal_dist != "" && !empty($value_postal_dist) && is_numeric($value_postal_dist)){
				
				$form['bocwa_reg_master']['e_postal_subdivision']= array(
													'#prefix' 			=> '<div id="postal_subdivision_div" class="col-md-4"><label class="select">',
													'#suffix' 			=> '</label></div>',
													'#title'			=> '2.(c) Select Subdivision',
													'#type' 			=> 'select',
													'#default_value'	=> !empty($e_postal_subdivision) ? $e_postal_subdivision : '',
													'#attributes'		=> $readonly_postal_address,
													'#options' 			=> common_custom_user_subdivisionlist_list_new($value_postal_dist),
													'#required' 		=> TRUE,
													'#ajax'				=> array(
																			'event' 			=> 'change',
																			'effect' 			=> 'fade',			
																			'callback'			=> 'get_ajax_callback_e_postal',
																			'progress'			=> '',
																			),
													);	
				$value_postal_subdivision	= isset($form_state['values']['e_postal_subdivision']) ? $form_state['values']['e_postal_subdivision'] : $e_postal_subdivision;

				if($value_postal_subdivision != $e_postal_subdivision){
					$e_postal_areatype		= '';
					$e_postal_areacode		= '';
					$e_postal_villward		= '';
					$e_postal_ps			= '';
					$e_postal_pin			= '';
				}										
		}

		if($value_postal_dist != "" && $value_postal_subdivision != ""){
				
			$db_block_mun_value	 = db_query("SELECT COUNT(id) as areatype_postal FROM {block_mun_master} WHERE distict_code = '".$value_postal_dist."' and sub_division_code  = '".$value_postal_subdivision."'")->fetchObject();
			
			if(!empty($db_block_mun_value) &&  $db_block_mun_value != ''){
				
				$areatypeOptions	= common_custom_user_areatype_list_new($value_postal_dist, $value_postal_subdivision);
					
				if(!empty($areatypeOptions)){
						
				 	$form['bocwa_reg_master']['e_postal_areatype'] = array(
													'#prefix' 			=> '<div id="postal_areatype_div" class="col-md-4"><label class="select">',
													'#suffix' 			=> '</label></div>',
													'#title'			=> '2.(d) Select Block / Municipality / Corporation / SEZ / Notified Area',
													'#type' 			=> 'select',
													'#options' 			=> $areatypeOptions,
													'#attributes'		=>  $readonly_postal_address,
													'#default_value'	=> !empty($e_postal_areatype) ? $e_postal_areatype : '',
													'#required' 		=> TRUE,
													'#ajax'				=> array(
																			'event' 			=> 'change',
																			'effect' 			=> 'fade',			
																			'callback'			=> 'get_ajax_callback_e_postal',
																			'progress'			=> array('type'=> 'throbber'),
																			),
													);
					$value_postal_areatype 	= isset($form_state['values']['e_postal_areatype']) ? $form_state['values']['e_postal_areatype'] : $e_postal_areatype;
					
					if($value_postal_areatype != $e_postal_areatype){
						$e_postal_areacode		= '';
						$e_postal_villward		= '';
						$e_postal_ps			= '';
						$e_postal_pin			= '';
					}
				}
			}
		}		
		if ($value_postal_dist != "" && $value_postal_subdivision != "" && $value_postal_areatype != "" ) {
					
			if ( $value_postal_areatype == 'B' ){
				$areatype_name	= 'Block';
			}elseif( $value_postal_areatype == 'M'){
				$areatype_name	= 'Municipality';
			}elseif( $value_postal_areatype == 'C'){
				$areatype_name	= 'Corporation';
			}elseif( $value_postal_areatype == 'S'){
				$areatype_name	= 'SEZ';
			}elseif($value_postal_areatype == 'N'){
				$areatype_name	= 'Notified Area';
			}
					
			$form['bocwa_reg_master']['e_postal_areacode']= array(
												  '#prefix' 			=> '<div id="postal_areacode_div" class="col-md-4"><label class="select">',
												  '#suffix' 			=> '<i></i></label></div>',
												  '#title'				=> '2.(e) Select '.$areatype_name,
												  '#type' 				=> 'select',
												  '#options' 			=> common_custom_user_block_list_new($value_postal_subdivision, $value_postal_areatype),
												  '#default_value' 		=> !empty($e_postal_areacode) ? $e_postal_areacode : '',
												  '#attributes'			=> $readonly_postal_address,
												  '#required' 			=> TRUE,
												  '#ajax'				=> array(
																			'event' 	=> 'change',
																			'effect' 	=> 'fade',			
																			'callback'	=> 'get_ajax_callback_e_postal',
																			'progress'	=> array('type'=> 'throbber'),
																		  )
												);
																		
			$value_postal_areacode 	= isset($form_state['values']['e_postal_areacode']) ? $form_state['values']['e_postal_areacode'] : $e_postal_areacode;
						
			if($value_postal_areacode != $e_postal_areacode){
						
				$e_postal_villward		= '';
				$e_postal_ps			= '';
				$e_postal_pin			= '';
			
			}
		}
		if($value_postal_dist != "" && $value_postal_subdivision != "" && $value_postal_areatype != "" && $value_postal_areacode != "" ){
					
			if ( $value_postal_areatype == 'B' ){
				$area_name		= 'Gram Panchayat';
				$area_code 		= 'V';
			}elseif( $value_postal_areatype == 'M'){
				$area_name		= 'Ward';
				$area_code 		= 'W';
			}elseif( $value_postal_areatype == 'C'){
				$area_name		= 'Ward';
				$area_code 		= 'C';
			}elseif( $value_postal_areatype == 'S'){
				$area_name		= 'Sector';
				$area_code 		= 'S';
			}elseif($value_postal_areatype == 'N'){
				$area_name		= 'Notified Area';
				$area_code 		= 'N';
			}
				
			$form['bocwa_reg_master']['e_postal_villward'] 	= array(
												  '#prefix' 			=> '<div id="postal_vill_div" class="col-md-4"><label class="select">',
												  '#suffix' 			=> '<i></i></label></div>',
												  '#title'				=> 'Select '.$area_name,
												  '#type' 				=> 'select',
												  '#options'			=> common_custom_user_villward_list_new($value_postal_areacode,$area_code),
												  '#default_value' 		=> !empty($e_postal_villward) ? $e_postal_villward : '',
												  '#attributes'			=> $readonly_postal_address,
												  '#required' 			=> TRUE,
												  '#ajax'				=> array(
																				'event' 	=> 'change',
																				'effect' 	=> 'fade',			
																				'callback'	=> 'get_ajax_callback_e_postal',
																				'progress'	=> array('type'=> 'throbber'),
																			  )
												);
																	
			$value_e_postal_villward 	= isset($form_state['values']['e_postal_villward']) ? $form_state['values']['e_postal_villward'] : $e_postal_villward;	
					
			if($value_e_postal_villward != $e_postal_villward){
						
				$e_postal_ps			= '';
				$e_postal_pin			= '';
						
			}	
		}
		if ($value_postal_dist != "" && $value_postal_subdivision != "" && $value_postal_areatype != "" && $value_postal_areacode != "" && $value_e_postal_villward != "" ){
				
			$form['bocwa_reg_master']['e_postal_ps']  = array(
												  '#prefix' 			=> '<div id="postal_ps_div" class="col-md-4"><label class="select">',
												  '#suffix' 			=> '</label></div>',
												  '#title'				=> '2.(f) Select Police Station',
												  '#type' 				=> 'select',
												  '#options' 			=> common_custom_user_ps_list_new($value_postal_dist),
												  '#default_value' 		=> !empty($e_postal_ps) ? $e_postal_ps : '',
												  '#attributes'			=> $readonly_postal_address,
												  '#required' 			=> TRUE,
												);
																									
			$form['bocwa_reg_master']['e_postal_pin'] = array(
												  '#prefix' 			=> '<div id="postal_pin_div" class="col-md-4"><label class="input">',
												  '#suffix' 			=> '</label></div>',
												  '#title'				=> '2.(g) PIN Number',
												  '#type' 				=> 'textfield',
												  '#default_value' 		=> !empty($e_postal_pin) ? $e_postal_pin	:'',
												  '#attributes' 		=> $readonly_e_postal_pin,
												  '#required' 			=> TRUE,
												  '#element_validate'	=> array('MISC_PIN_NUMBER'),
											  );	
		}

		$form['bocwa_reg_master']['e_full_name'] = array(
			'#prefix'				=> '</div></div><div class="panel panel-default panel-form"><div class="panel-heading">3. Full Name and permanent address of the Establishment</div><div class="panel-body"><div class="col-lg-4 col-md-6"><label class="input">',
			'#suffix' 			=> '</label></div>',
			'#title'				=> '3.(a) Full Name of the Establishment',
			'#type' 				=> 'textfield',
			'#default_value' 		=> !empty($est_full_name) ? $est_full_name : '',
			'#required' 			=> TRUE,
			'#attributes' 		=> !empty($readonly_e_full_name)? $readonly_e_full_name:'',
			'#element_validate' 	=> array('MISC_TEXTBOX_EST_VALIDATE')
		  );
		  
		$form['bocwa_reg_master']['est_address'] = array(
				'#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
				'#suffix' 			=> '</label></div>',
				'#title'				=> '3.(b) Permanent Address of the Establishment',
				'#type' 				=> 'textfield',
				'#default_value' 		=> !empty($est_address) ? $est_address : '',
				'#required' 			=> TRUE,
				'#attributes' 		=> $readonly_e_permanent_address,
				'#element_validate' => array('MISC_TEXTBOX_LOCATION_VALIDATE')
				);
				
		$form['bocwa_reg_master']['est_dist']	= array(
				'#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="select">',
				'#suffix'				=> '</label></div>',
				'#title'				=> '3.(c) Select District',
				'#type' 				=> 'select',
				'#options' 			=> common_custom_user_district_list_new(),
				'#default_value' 		=> ($est_dist!='') ? $est_dist : '',
				'#required' 			=> TRUE,
				'#attributes' 		=> $readonly_e_full_name,
				'#ajax'				=> array(
												'event' 			=> 'change',
												'effect' 			=> 'fade',			
												'callback'			=> 'est_ajax_callback',
												'progress'			=> array('type'=> 'throbber'),
											)
				);
		
		$form['bocwa_reg_master']['est_subdivision'] = array(
			'#prefix' 			=> '<div id="est_subdivision_div">',
			'#suffix'			=> '</div>',
			);
		$form['bocwa_reg_master']['est_areatype'] = array(
			'#prefix' 			=> '<div id="est_areatype_div">',
			'#suffix'			=> '</div>',
			);
		$form['bocwa_reg_master']['est_areacode'] = array(
			'#prefix' 			=> '<div id="est_areacode_div">',
			'#suffix'			=> '</div>',
			);				
		$form['bocwa_reg_master']['est_villward'] = array(
			'#prefix' 			=> '<div id="est_vill_div">',
			'#suffix'			=> '</div>',
			);
		$form['bocwa_reg_master']['est_ps'] = array(
			'#prefix' 			=> '<div id="est_ps_div">',
			'#suffix'			=> '</div>',
			);	
			$form['bocwa_reg_master']['est_pin'] = array(
			'#prefix' 			=> '<div id="est_pin_div">',
			'#suffix'			=> '</div>',
			);			
		$value_est_dist = isset($form_state['values']['est_dist']) ? $form_state['values']['est_dist'] : $est_dist;



		if($value_est_dist != $est_dist){
		$est_subdivision 	= '';
		$est_areatype		= '';
		$est_areacode		= '';
		$est_villward		= '';
		$est_ps				= '';
		$est_pin			= '';
		}

		if($value_est_dist != "" && !empty($value_est_dist) && is_numeric($value_est_dist)){

		 $db_estsubdiv_value	= db_query("SELECT COUNT(id) as subdv_est  FROM {sub_division} WHERE district_code = '".$value_est_dist."'")->fetchObject();

			if(!empty($db_estsubdiv_value) && $db_estsubdiv_value != '' && $db_estsubdiv_value->subdv_est > 0) {

				$form['bocwa_reg_master']['est_subdivision']= array(
							'#prefix' 			=> '<div id="est_subdivision_div" class="col-md-4"><label class="select">',
							'#suffix' 			=> '</label></div>',
							'#title'			=> '3.(d) Select Subdivision',
							'#type' 			=> 'select',
							'#default_value'	=> !empty($est_subdivision) ? $est_subdivision : '',
							'#attributes'		=> $readonly_e_full_name,
							'#options' 			=> common_custom_user_subdivisionlist_list_new($value_est_dist),
							'#required' 		=> TRUE,
							'#ajax'				=> array(
													'event' 			=> 'change',
													'effect' 			=> 'fade',			
													'callback'			=> 'est_ajax_callback',
													'progress'			=> array('type'=> 'throbber'),
													),
							);
						
				$value_est_subdivision	= isset($form_state['values']['est_subdivision']) ? $form_state['values']['est_subdivision'] : $est_subdivision;

				if($value_est_subdivision != $est_subdivision){
				$est_areatype		= '';
				$est_areacode		= '';
				$est_villward		= '';
				$est_ps				= '';
				$est_pin			= '';
				}

			}
		}
		//echo $value_est_subdivision;

		if($value_est_dist != "" && $value_est_subdivision != ""){

		 $db_block_mun_value	 = db_query("SELECT COUNT(id) as areatype_est FROM {block_mun_master} WHERE distict_code = '".$value_est_dist."' and sub_division_code  = '".$value_est_subdivision."'")->fetchObject();

			if(!empty($db_block_mun_value) &&  $db_block_mun_value != '' && $db_block_mun_value->areatype_est > 0){

				$areatypeOptions	= common_custom_user_areatype_list_new($value_est_dist, $value_est_subdivision);

				if(!empty($areatypeOptions)){

					$form['bocwa_reg_master']['est_areatype'] = array(
								'#prefix' 			=> '<div id="est_areatype_div" class="col-md-4"><label class="select">',
								'#suffix' 			=> '</label></div>',
								'#title'			=> '3.(e) Select Block / Municipality / Corporation / SEZ / Notified Area',
								'#type' 			=> 'select',
								'#options' 			=> $areatypeOptions,
								'#attributes'		=> $readonly_e_full_name,
								'#default_value'	=> !empty($est_areatype) ? $est_areatype : '',
								'#required' 		=> TRUE,
								'#ajax'				=> array(
														'event' 			=> 'change',
														'effect' 			=> 'fade',			
														'callback'			=> 'est_ajax_callback',
														'progress'			=> array('type'=> 'throbber'),
														),
								);
					$value_est_areatype 	= isset($form_state['values']['est_areatype']) ? $form_state['values']['est_areatype'] : $est_areatype;

					if($value_est_areatype != $est_areatype){
					$est_areacode		= '';
					$est_villward		= '';
					$est_ps				= '';
					$est_pin			= '';
					}
				}
			}
		}

		if($value_est_dist != "" && $value_est_subdivision != "" && $value_est_areatype != "" ) {

			if ( $value_est_areatype == 'B' ){
			$areatype_name	= 'Block';
			}elseif( $value_est_areatype == 'M'){
			$areatype_name	= 'Municipality';
			}elseif( $value_est_areatype == 'C'){
			$areatype_name	= 'Corporation';
			}elseif( $value_est_areatype == 'S'){
			$areatype_name	= 'SEZ';
			}elseif($value_est_areatype == 'N'){
			$areatype_name	= 'Notified Area';
			}

			$form['bocwa_reg_master']['est_areacode']= array(
					'#prefix' 			=> '<div id="est_areacode_div" class="col-md-4"><label class="select">',
					'#suffix' 			=> '<i></i></label></div>',
					'#title'				=> '2.(e) Select '.$areatype_name,
					'#type' 				=> 'select',
					'#options' 			=> common_custom_user_block_list_new($value_est_subdivision, $value_est_areatype),
					'#default_value' 		=> !empty($est_areacode) ? $est_areacode : '',
					'#attributes'			=> $readonly_e_full_name,
					'#required' 			=> TRUE,
					'#ajax'				=> array(
												'event' 	=> 'change',
												'effect' 	=> 'fade',			
												'callback'	=> 'est_ajax_callback',
												'progress'	=> array('type'=> 'throbber'),
											)
					);
										
			$value_est_areacode 	= isset($form_state['values']['est_areacode']) ? $form_state['values']['est_areacode'] : $est_areacode;

			if($value_est_areacode != $est_areacode){

			$est_villward		= '';
			$est_ps				= '';
			$est_pin			= '';

			}
		}

		if($value_est_dist != "" && $value_est_subdivision != "" && $value_est_areatype != "" && $value_est_areacode != "" ){

			if ( $value_est_areatype == 'B' ){
			$area_name		= 'Gram Panchayat';
			$area_code 		= 'V';
			}elseif( $value_est_areatype == 'M'){
			$area_name		= 'Ward';
			$area_code 		= 'W';
			}elseif( $value_est_areatype == 'C'){
			$area_name		= 'Ward';
			$area_code 		= 'C';
			}elseif( $value_est_areatype == 'S'){
			$area_name		= 'Sector';
			$area_code 		= 'S';
			}elseif($value_est_areatype == 'N'){
			$area_name		= 'Notified Area';
			$area_code 		= 'N';
			}

			$form['bocwa_reg_master']['est_villward'] 	= array(
					'#prefix' 			=> '<div id="est_vill_div" class="col-md-4"><label class="select">',
					'#suffix' 			=> '<i></i></label></div>',
					'#title'				=> 'Select '.$area_name,
					'#type' 				=> 'select',
					'#options'			=> common_custom_user_villward_list_new($value_est_areacode,$area_code),
					'#default_value' 		=> !empty($est_villward) ? $est_villward : '',
					'#attributes'			=> $readonly_e_full_name,
					'#required' 			=> TRUE,
					'#ajax'				=> array(
													'event' 	=> 'change',
													'effect' 	=> 'fade',			
													'callback'	=> 'est_ajax_callback',
													'progress'	=> array('type'=> 'throbber'),
												)
					);
										
			$value_est_villward 	= isset($form_state['values']['est_villward']) ? $form_state['values']['est_villward'] : $est_villward;	

			if($value_est_villward != $est_villward){

			$est_ps				= '';
			$est_pin			= '';

			}	
		}

		if ($value_est_dist != "" && $value_est_subdivision != "" && $value_est_areatype != "" && $value_est_areacode != "" && $value_est_villward != "" ){

			$form['bocwa_reg_master']['est_ps']  = array(
					'#prefix' 			=> '<div id="est_ps_div" class="col-md-4"><label class="select">',
					'#suffix' 			=> '</label></div>',
					'#title'				=> '2.(h) Select Police Station',
					'#type' 				=> 'select',
					'#options' 			=> common_custom_user_ps_list_new($value_est_dist),
					'#default_value' 		=> !empty($est_ps) ? $est_ps : '',
					'#attributes'			=> $readonly_e_full_name,
					'#required' 			=> TRUE,
					);
																		
			$form['bocwa_reg_master']['est_pin'] = array(
					'#prefix' 			=> '<div id="est_pin_div" class="col-md-4"><label class="input">',
					'#suffix' 			=> '</label></div>',
					'#title'				=> '2.(i) PIN Number',
					'#type' 				=> 'textfield',
					'#default_value' 		=> !empty($est_pin) ? $est_pin	:'',
					'#attributes' 		=> $readonly_e_permanent_pin,
					'#required' 			=> TRUE,
					'#element_validate'	=> array('MISC_PIN_NUMBER'),
				);	
		}
		$form['bocwa_reg_master']['full_name_manager']	= array(
			'#prefix' 				=> '</div></div><div class="panel panel-default panel-form"><div class="panel-heading">4. Full name and address of the Manager or Person responsible for the supervision  and control of the Establishment</div><div class="panel-body"><div class="col-md-6"><label class="input">',
			'#suffix' 				=> '</label></div>',
			'#title'				=> '4.(a) Full name of the Manager or Person Responsible',
			'#type' 				=> 'textfield',
			'#default_value' 		=> !empty($full_name_manager) ? $full_name_manager : '',
			'#required' 			=> TRUE,
			'#attributes' 			=> $readonly_manager_info,
			'#element_validate'		=> array('MISC_TEXTBOX_TEXT_VALIDATE')
			);
			


		$form['bocwa_reg_master']['manager_country'] = array(
					'#prefix' 				=> '<div class="col-md-6"><label class="select">',
					'#suffix' 				=> '</label></div>',
					'#title'				=> '4.(b) Select Country',
					'#default_value'		=> ($manager_country!='') ? $manager_country : '',
					'#type' 				=> 'select',
					'#options' 				=> array('' => '-Select Country-','1' => 'India', '2' => 'Others'),
					'#required' 			=> TRUE,
					'#attributes'			=> $readonly_manager_info, 
					'#ajax'					=> array(
												'event' 		=> 'change',
												'effect' 		=> 'fade',			
												'callback'		=> 'get_ajax_country_manager',
												'progress'		=> array('type'=> 'throbber'),
										)
				);
									
			$valuemanager_country 	= isset($form_state['values']['manager_country']) ? $form_state['values']['manager_country'] : $manager_country;

			if($valuemanager_country != $manager_country ){

			$manager_state			= '';
			$manager_dist 			= '';
			$manager_subdivision 	= '';
			$manager_areatype		= '';
			$manager_areacode		= '';
			$manager_vill_ward		= '';
			$manager_ps				= '';
			$manager_pin			= '';

			}

		$form['bocwa_reg_master']['address_manager'] = array(
						'#prefix' 			=> '<div id="address_manager_replace">',
						'#suffix'			=> '</div>',
						);
																			
		$form['bocwa_reg_master']['manager_state']	= array(
						'#prefix' 			=> '<div id="manager_state_replace">',
						'#suffix' 			=> '</div>',
						);
																
		$form['bocwa_reg_master']['manager_dist']	= array(
						'#prefix' 			=> '<div id="manager_district_replace">',
						'#suffix' 			=> '</div>',
						);
																
		$form['bocwa_reg_master']['manager_subdv']	= array(
						'#prefix' 			=> '<div id="manager_subdv_replace">',
						'#suffix' 			=> '</div>',
						);
																
		$form['bocwa_reg_master']['manager_areatype'] = array(
						'#prefix' 			=> '<div id="manager_areatype">',
						'#suffix' 			=> '</div>',
						);
																
		$form['bocwa_reg_master']['manager_areacode']= array(
						'#prefix' 			=> '<div id="manager_name_areatype">',
						'#suffix' 			=> '</div>',
						);
																
		$form['bocwa_reg_master']['manager_vill_ward'] = array(
						'#prefix' 			=> '<div id="manager_vill_ward">',
						'#suffix' 			=> '</div>',
						);
																
		$form['bocwa_reg_master']['manager_ps'] = array(
						'#prefix' 			=> '<div id="manager_ps_replace">',
						'#suffix' 			=> '</div>',
						);
															
		$form['bocwa_reg_master']['manager_pin'] = array(
						'#prefix' 			=> '<div id="managerpin_replace">',
						'#suffix' 			=> '</div>',
						);
											
		if( (!empty($valuemanager_country) && $valuemanager_country != '') && $valuemanager_country ==  1) {

			$form['bocwa_reg_master']['address_manager'] = array(
						'#prefix' 				=> '<div id="address_manager_replace" class="col-md-4"><label class="input">',
						'#suffix' 				=> '</label></div>',
						'#title'				=> '4.(c) Address Line 1 of the Manager or Person responsible',
						'#type' 				=> 'textfield',
						'#required' 			=> TRUE,
						'#default_value' 		=> !empty($manager_address) ? $manager_address : '',
						'#attributes'			=> $readonly_manager_info,
						'#element_validate'	=> array('MISC_TEXTBOX_LOCATION_VALIDATE')
						);
											
			$form['bocwa_reg_master']['manager_state']	= array(
						'#prefix' 				=> '<div id="manager_state_replace" class="col-md-4"><label class="select">',
						'#suffix' 				=> '</label></div>',
						'#title'				=> t('4.(d) Select State'),
						'#type' 				=> 'select',
						'#options' 				=> get_all_state($valuemanager_country),
						'#default_value'		=> ($manager_state!='') ? $manager_state : '',
						'#attributes'			=> $readonly_manager_info, 
						'#required' 			=> TRUE,
						'#ajax'					=> array(
													'event'		=> 'change',
													'effect' 	=> 'fade',
													'callback'	=> 'get_ajax_manager',
													'progress'	=> array('type'=> 'throbber'),
													)
					);
											
				$value_manager_state	= isset($form_state['values']['manager_state'])? $form_state['values']['manager_state'] : $manager_state;

				if($value_manager_state != $manager_state ){

				$manager_dist 			= '';
				$manager_subdivision 	= '';
				$manager_areatype		= '';
				$manager_areacode		= '';
				$manager_villward		= '';
				$manager_ps				= '';
				$manager_pin			= '';

				}

			if(($value_manager_state != '' && (!empty($value_manager_state) || $value_manager_state != '' )  )){

				$db_manager_district_value	= db_query("SELECT COUNT(id) FROM {district_master} WHERE state_id = '".$value_manager_state."'")->fetchObject();	

				if(!empty($db_manager_district_value) && ($db_manager_district_value->count>0) ) {

					$form['bocwa_reg_master']['manager_dist'] = array(
									'#prefix' 			=> '<div id="manager_district_replace" class="col-md-4"><label class="select">',
									'#suffix' 			=> '</label></div>',
									'#title'			=> t('4.(e) Select District'),
									'#type' 			=> 'select',
									'#options' 			=> get_district_by_id($value_manager_state),
									'#default_value' 	=> !empty($manager_dist) ? $manager_dist : '',
									'#attributes'		=> $readonly_manager_info,
									'#required' 		=> TRUE,
									'#ajax'				=> array(
																'event'		=> 'change',
																'effect' 	=> 'fade',
																'callback'	=> 'get_ajax_manager',
																'progress'	=> array('type'=> 'throbber'),
																)
									);
														
					$value_manager_dist  = 	isset($form_state['values']['manager_dist']) ? $form_state['values']['manager_dist'] : $manager_dist;

					if($value_manager_dist != $manager_dist){
					$manager_subdivision 	= '';
					$manager_areatype		= '';
					$manager_areacode		= '';
					$manager_villward		= '';
					$manager_ps				= '';
					$manager_pin			= '';
					}
				}
			}

			if($value_manager_dist != ''  && !empty($value_manager_state)){

				$db_manager_subdivision_value	= db_query("SELECT COUNT(id) as subdv_total_manager  FROM {sub_division} WHERE district_code = '".$value_manager_dist."'")->fetchObject();

				if(!empty($db_manager_subdivision_value) && $db_manager_subdivision_value != '' && $db_manager_subdivision_value->subdv_total_manager > 0) {

					$form['bocwa_reg_master']['manager_subdv'] 	= array(
							'#prefix' 			=> '<div id="manager_subdv_replace" class="col-md-4"><label class="select">',
							'#suffix' 			=> '<i></i></label></div>',
							'#title'				=> '4.(f) Select Subdivision',
							'#type' 				=> 'select',
							'#options' 			=> common_custom_user_subdivisionlist_list_new($value_manager_dist),
							'#default_value' 		=> !empty($manager_subdivision) ? $manager_subdivision	:'',
							'#required' 			=> TRUE,
							'#attributes'			=> $readonly_manager_info,
							'#ajax'				=> array(
														'event' 	=> 'change',
														'effect' 	=> 'fade',			
														'callback'	=> 'get_ajax_manager',
														'progress'	=> array('type'=> 'throbber'),
													),
						);
					$value_manager_subdv 		= isset($form_state['values']['manager_subdv']) ? $form_state['values']['manager_subdv'] : $manager_subdivision;

					if($value_manager_subdv != $manager_subdivision ){

					$manager_areatype	= '';
					$manager_areacode	= '';
					$manager_villward	= '';
					$manager_ps			= '';
					$manager_pin		= '';

					}
				}
			}

			if($value_manager_dist != ''  && !empty($value_manager_state) && $value_manager_subdv != "" ){ 

			$db_block_mun_manager_value	= db_query("SELECT COUNT(id) as areatype_manager_total FROM {block_mun_master} WHERE distict_code = '".$value_manager_dist."' and sub_division_code  = '".$value_manager_subdv."'")->fetchObject();

				if(!empty($db_block_mun_manager_value) &&  $db_block_mun_manager_value != '' && $db_block_mun_manager_value->areatype_manager_total > 0){

					$areatypeOptions	= common_custom_user_areatype_list_new($value_manager_dist, $value_manager_subdv);

					if(!empty($areatypeOptions)){

						$form['bocwa_reg_master']['manager_areatype'] = array(
										'#prefix' 		=> '<div id="manager_areatype" class="col-md-4"><label class="select">',
										'#suffix' 		=> '<i></i></label></div>',
										'#title'			=> '4.(g) Select Areatype',
										'#type' 			=> 'select',
										'#options' 		=> $areatypeOptions,
										'#default_value' 	=> !empty($manager_areatype) ? $manager_areatype : '',
										'#attributes'		=> $readonly_manager_info,
										'#required' 		=> TRUE,
										'#ajax'			=> array(
																	'event' 	=> 'change',
																	'effect' 	=> 'fade',			
																	'callback'	=> 'get_ajax_manager',
																	'progress'	=> array('type'=> 'throbber'),
																),
											);
																							
						$value_manager_areatype = isset($form_state['values']['manager_areatype']) ? $form_state['values']['manager_areatype'] : $manager_areatype;

						if($value_manager_subdv != $manager_subdivision && $value_manager_areatype != $manager_areatype){

						$manager_areacode	= '';
						$manager_villward	= '';
						$manager_ps			= '';
						$manager_pin		= '';
						}

					}	

				}
			}

			if ($value_manager_dist != "" && $value_manager_subdv != "" && $value_manager_areatype != "" && !empty($value_manager_state)) {

				if ($value_manager_areatype == 'B'){
				$areatype_name	= 'Block';
				}elseif( $value_manager_areatype == 'M'){
				$areatype_name	= 'Municipality';
				}elseif( $value_manager_areatype == 'C'){
				$areatype_name	= 'Corporation';
				}elseif( $value_manager_areatype == 'S'){
				$areatype_name	= 'SEZ';
				}elseif($value_manager_areatype == 'N'){
				$areatype_name	= 'Notified Area';
				}

				$form['bocwa_reg_master']['manager_areacode']= array(
							'#prefix' 		=> '<div id="manager_name_areatype" class="col-md-4"><label class="select">',
							'#suffix' 		=> '<i></i></label></div>',
							'#title'			=> '4.(h) Select '.$areatype_name,
							'#type' 			=> 'select',
							'#options' 		=> common_custom_user_block_list_new($value_manager_subdv, $value_manager_areatype),
							'#default_value' 	=> !empty($manager_areacode) ? $manager_areacode : '',
							'#attributes'		=> $readonly_manager_info,
							'#required' 		=> TRUE,
							'#ajax'			=> array(
														'event' 	=> 'change',
														'effect' 	=> 'fade',			
														'callback'	=> 'get_ajax_manager',
														'progress'	=> array('type'=> 'throbber'),
													)
							);
												
				$value_manager_areacode 	= isset($form_state['values']['manager_areacode']) ? $form_state['values']['manager_areacode'] : $manager_areacode;

				if($value_manager_areacode != $manager_areacode){

				$manager_villward	= '';
				$manager_ps			= '';
				$manager_pin		= '';

				}
			}

			if($value_manager_dist != "" && $value_manager_subdv != "" && $value_manager_areatype != ""  && $value_manager_areacode != "" && !empty($value_manager_state) ){

				if ( $value_manager_areatype == 'B' ){
				$area_name		= 'Gram Panchayat';
				$area_code 		= 'V';
				}elseif( $value_manager_areatype == 'M'){
				$area_name		= 'Ward';
				$area_code 		= 'W';
				}elseif( $value_manager_areatype == 'C'){
				$area_name		= 'Ward';
				$area_code 		= 'C';
				}elseif( $value_manager_areatype == 'S'){
				$area_name		= 'Sector';
				$area_code 		= 'S';
				}elseif($value_manager_areatype == 'N'){
				$area_name		= 'Notified Area';
				$area_code 		= 'N';
				}

				$form['bocwa_reg_master']['manager_vill_ward'] 	= array(
							'#prefix' 		=> '<div id="manager_vill_ward" class="col-md-4"><label class="select">',
							'#suffix' 		=> '<i></i></label></div>',
							'#title'			=> '4.(i) Select '.$area_name,
							'#type' 			=> 'select',
							'#options'		=> common_custom_user_villward_list_new($value_manager_areacode,$area_code),
							'#default_value' 	=> !empty($manager_villward) ? $manager_villward : '',
							'#attributes'		=> $readonly_manager_info,
							'#required' 		=> TRUE,
							'#ajax'			=> array(
														'event' 	=> 'change',
														'effect' 	=> 'fade',			
														'callback'	=> 'get_ajax_manager',
														'progress'	=> array('type'=> 'throbber'),
													)
							);
												
				$value_manager_vill_ward 	= isset($form_state['values']['manager_vill_ward']) ? $form_state['values']['manager_vill_ward'] : $manager_villward;	

				if($value_manager_vill_ward != $manager_villward){

				$manager_ps		= '';
				$manager_pin	= '';

				}	
			}

			if (!empty($value_manager_state) && $value_manager_dist != "" && $value_manager_subdv != "" && $value_manager_areatype != ""  && $value_manager_areacode != "" && $value_manager_vill_ward != "" ){

				$form['bocwa_reg_master']['manager_ps']  = array(
								'#prefix' 		=> '<div id="manager_ps_replace" class="col-md-4"><label class="select">',
								'#suffix' 		=> '</label></div>',
								'#title'			=> '4.(j) Select Police Station',
								'#type' 			=> 'select',
								'#options' 		=> common_custom_user_ps_list_new($value_manager_dist),
								'#default_value' 	=> !empty($manager_ps) ? $manager_ps : '',
								'#attributes'		=> $readonly_manager_info,
								'#required' 		=> TRUE,
								);
																					
				$form['bocwa_reg_master']['manager_pin'] = array(
								'#prefix' 		=> '<div id="managerpin_replace" class="col-md-4"><label class="input">',
								'#suffix' 		=> '</label></div>',
								'#title'			=> '4.(k) PIN Number',
								'#type' 			=> 'textfield',
								'#default_value' 	=> !empty($manager_pin) ? $manager_pin	:'',
								'#attributes' 	=> $readonly_manager_pin,
								'#required' 		=> TRUE,
								'#element_validate'=> array('MISC_PIN_NUMBER'),
							);	
			}

		}else{

			$form['bocwa_reg_master']['address_manager'] = array(
					'#prefix' 			=> '<div id="address_manager_replace" class="col-md-12"><label class="textarea">',
					'#suffix' 			=> '</label></div>',
					'#title'				=> '4.(c) Address of the Manager or Person Responsible [ If other Country/State please provide detail address ]',
					'#rows'				=> t('2'),	
					'#type' 				=> 'textarea',
					'#attributes'		=> $readonlyAddressManagerTextarea,
					'#default_value' 	=> !empty($manager_address) ? $manager_address : '',
					'#required' 			=> TRUE,
					'#element_validate'	=> array('MISC_TEXTBOX_LOCATION_VALIDATE')
					);

		}
		$form['bocwa_reg_master']['emp_name']= array(
												'#prefix' 				=> '</div></div><div class="panel panel-default panel-form"><div class="panel-heading">5. Name and address of the Employer</div><div class="panel-body"><div class="col-md-4"><label class="input">',
												'#suffix' 				=> '</label></div>',
												'#title'				=> '5.(a) Name of the Employer',
												'#type' 				=> 'textfield',
												'#default_value' 		=> !empty($emp_name) ? $emp_name : '',
												'#required' 			=> TRUE,
												'#attributes' 			=> $readonly_emp_info,
												'#element_validate'		=> array('MISC_TEXTBOX_TEXT_VALIDATE')
												);
		$form['bocwa_reg_master']['emp_gender'] = array(
											   '#prefix'				=> '<div class="col-md-4"><div class="inline-group">',
											   '#suffix'				=>'</div></div>',
											   '#type' 					=> 'radios',
											   '#title' 				=> t('5.(b) Gender'),
											   '#options' 				=> array('M'=>t('Male'), 'F'=>t('Female'), 'O'=>t('Other')),
											   '#default_value' 		=> isset($emp_gender) ? $emp_gender : 'O',
											   '#attributes' 			=> $readonly_emp_gender,
											   '#required' 				=> TRUE
												);
												
		$form['bocwa_reg_master']['emp_country'] = array(
												'#prefix' 				=> '<div class="col-md-4"><label class="select">',
												'#suffix' 				=> '</label></div>',
												'#title'				=> '5.(c) Select Country',
												'#default_value'		=> ($emp_country!='') ? $emp_country : '',
												'#type' 				=> 'select',
												'#options' 				=> array('' => '-Select Country-','1' => 'India', '2' => 'Others'),
												'#required' 			=> TRUE,
												'#attributes'			=> $readonly_emp_info, 
												'#ajax'					=> array(
																			'event' 		=> 'change',
																			'effect' 		=> 'fade',			
																			'callback'		=> 'get_ajax_country_emp',
																			'progress'		=> array('type'=> 'throbber'),
																	)
											);
		$value_emp_country 	= isset($form_state['values']['emp_country']) ? $form_state['values']['emp_country'] : $emp_country;

		if($value_emp_country != $emp_country ){
						
			$emp_state			= '';
			$emp_dist 			= '';
			$emp_subdivision 	= '';
			$emp_areatype		= '';
			$emp_areacode		= '';
			$emp_villward		= '';
			$emp_ps				= '';
			$emp_pin			= '';
		}
		$form['bocwa_reg_master']['emp_address'] = array(
													'#prefix' 			=> '<div id="emp_address_replace">',
													'#suffix'			=> '</div>',
													);
																										
		$form['bocwa_reg_master']['emp_state']	= array(
													'#prefix' 			=> '<div id="emp_state_replace">',
													'#suffix' 			=> '</div>',
													);
																							
		$form['bocwa_reg_master']['emp_dist']	= array(
													'#prefix' 			=> '<div id="emp_district_replace">',
													'#suffix' 			=> '</div>',
													);
																							
		$form['bocwa_reg_master']['emp_subdv']	= array(
													'#prefix' 			=> '<div id="emp_subdv_replace">',
													'#suffix' 			=> '</div>',
													);
																							
		$form['bocwa_reg_master']['emp_areatype'] = array(
													'#prefix' 			=> '<div id="emp_areatype">',
													'#suffix' 			=> '</div>',
													);
																							
		$form['bocwa_reg_master']['emp_areacode']= array(
													'#prefix' 			=> '<div id="emp_name_areatype">',
													'#suffix' 			=> '</div>',
													);
																							
		$form['bocwa_reg_master']['emp_vill_ward'] = array(
													'#prefix' 			=> '<div id="emp_vill_ward">',
													'#suffix' 			=> '</div>',
													);
																							
		$form['bocwa_reg_master']['emp_ps'] = array(
													'#prefix' 			=> '<div id="emp_ps_replace">',
													'#suffix' 			=> '</div>',
													);
																						
		$form['bocwa_reg_master']['emp_pin'] = array(
													'#prefix' 			=> '<div id="emppin_replace">',
													'#suffix' 			=> '</div>',
													);
		if( (!empty($value_emp_country) && $value_emp_country != '') && $value_emp_country ==  1) {
			$form['bocwa_reg_master']['emp_address'] = array(
												 '#prefix' 				=> '<div id="emp_address_replace" class="col-md-4"><label class="input">',
												 '#suffix' 				=> '</label></div>',
												 '#title'				=> '5.(d) Address Line 1 of the Employer',
												 '#type' 				=> 'textfield',
												 '#required' 			=> TRUE,
												 '#default_value' 		=> !empty($emp_address) ? $emp_address : '',
												 '#attributes'			=> $readonly_emp_info,
												 '#element_validate'	=> array('MISC_TEXTBOX_LOCATION_VALIDATE')
												);
												
			$form['bocwa_reg_master']['emp_state']	= array(
												'#prefix' 				=> '<div id="emp_state_replace" class="col-md-4"><label class="select">',
												'#suffix' 				=> '</label></div>',
												'#title'				=> t('5.(e) Select State'),
												'#type' 				=> 'select',
												'#options' 				=> get_all_state($value_emp_country),
												'#default_value'		=> ($emp_state!='') ? $emp_state : '',
												'#attributes'			=> $readonly_emp_state, 
												'#required' 			=> TRUE,
												'#ajax'					=> array(
																			'event'		=> 'change',
																			'effect' 	=> 'fade',
																			'callback'	=> 'get_ajax_emp',
																			'progress'	=> array('type'=> 'throbber'),
																			)
											);
											
			$value_emp_state	= isset($form_state['values']['emp_state'])? $form_state['values']['emp_state'] : $emp_state;
				
			if($value_emp_state != $emp_state ){
					
				$emp_dist 			= '';
				$emp_subdv 			= '';
				$emp_areatype		= '';
				$emp_areacode		= '';
				$emp_villward		= '';
				$emp_ps				= '';
				$emp_pin			= '';
						
			}
			
			if(($value_emp_state != '' && (!empty($value_emp_state) || $value_emp_state != '' )  )){
					
				$db_emp_district_value	= db_query("SELECT COUNT(id) FROM {district_master} WHERE state_id = '".$value_emp_state."'")->fetchObject();	
					
				if(!empty($db_emp_district_value) && ($db_emp_district_value->count>0) ) {
						
					$form['bocwa_reg_master']['emp_dist'] = array(
													'#prefix' 			=> '<div id="emp_district_replace" class="col-md-4"><label class="select">',
													'#suffix' 			=> '</label></div>',
													'#title'			=> t('5.(f) Select District'),
													'#type' 			=> 'select',
													'#options' 			=> get_district_by_id($value_emp_state),
													'#default_value' 	=> !empty($emp_dist) ? $emp_dist : '',
													'#attributes'		=> $readonly_emp_info,
													'#required' 		=> TRUE,
													'#ajax'				=> array(
																				'event'		=> 'change',
																				'effect' 	=> 'fade',
																				'callback'	=> 'get_ajax_emp',
																				'progress'	=> array('type'=> 'throbber'),
																				)
													);
																		
					$value_emp_dist  = 	isset($form_state['values']['emp_dist']) ? $form_state['values']['emp_dist'] : $emp_dist;
						
					if($value_emp_dist != $emp_dist){
						$emp_subdivision 	= '';
						$emp_areatype		= '';
						$emp_areacode		= '';
						$emp_villward		= '';
						$emp_ps				= '';
						$emp_pin			= '';
					}
				}
			}
			
			if($value_emp_dist != ''  && !empty($value_emp_state)){
					
				$db_emp_subdivision_value	= db_query("SELECT COUNT(id) as subdv_total_emp  FROM {sub_division} WHERE district_code = '".$value_emp_dist."'")->fetchObject();
							
				if(!empty($db_emp_subdivision_value) && $db_emp_subdivision_value != '' && $db_emp_subdivision_value->subdv_total_emp > 0) {
						
					$form['bocwa_reg_master']['emp_subdv'] 	= array(
												  '#prefix' 			=> '<div id="emp_subdv_replace" class="col-md-4"><label class="select">',
												  '#suffix' 			=> '<i></i></label></div>',
												  '#title'				=> '5.(g) Select Subdivision',
												  '#type' 				=> 'select',
												  '#options' 			=> common_custom_user_subdivisionlist_list_new($value_emp_dist),
												  '#default_value' 		=> !empty($emp_subdivision) ? $emp_subdivision	:'',
												  '#required' 			=> TRUE,
												  '#attributes'			=> $readonly_emp_info,
												  '#ajax'				=> array(
																			'event' 	=> 'change',
																			'effect' 	=> 'fade',			
																			'callback'	=> 'get_ajax_emp',
																			'progress'	=> array('type'=> 'throbber'),
																		  ),
											);
					$value_emp_subdv 		= isset($form_state['values']['emp_subdv']) ? $form_state['values']['emp_subdv'] : $emp_subdivision;
								
					if($value_emp_subdv != $emp_subdivision ){
								
						$emp_areatype	= '';
						$emp_areacode	= '';
						$emp_villward	= '';
						$emp_ps			= '';
						$emp_pin		= '';
						
					}
				}
			}
			
			if($value_emp_dist != ''  && !empty($value_emp_state) && $value_emp_subdv != "" ){ 
					
				$db_block_mun_emp_value	= db_query("SELECT COUNT(id) as areatype_emp_total FROM {block_mun_master} WHERE distict_code = '".$value_emp_dist."' and sub_division_code  = '".$value_emp_subdv."'")->fetchObject();
					
				if(!empty($db_block_mun_emp_value) &&  $db_block_mun_emp_value != '' && $db_block_mun_emp_value->areatype_emp_total > 0){
					
				  $areatypeOptions	= common_custom_user_areatype_list_new($value_emp_dist, $value_emp_subdv);
					
				  if(!empty($areatypeOptions)){
								
					$form['bocwa_reg_master']['emp_areatype'] = array(
													  '#prefix' 		=> '<div id="emp_areatype" class="col-md-4"><label class="select">',
													  '#suffix' 		=> '<i></i></label></div>',
													  '#title'			=> '5.(h) Select Areatype',
													  '#type' 			=> 'select',
													  '#options' 		=> $areatypeOptions,
													  '#default_value' 	=> !empty($emp_areatype) ? $emp_areatype : '',
													  '#attributes'		=> $readonly_emp_info,
													  '#required' 		=> TRUE,
													  '#ajax'			=> array(
																				'event' 	=> 'change',
																				'effect' 	=> 'fade',			
																				'callback'	=> 'get_ajax_emp',
																				'progress'	=> array('type'=> 'throbber'),
																			  ),
														);
																											
					$value_emp_areatype = isset($form_state['values']['emp_areatype']) ? $form_state['values']['emp_areatype'] : $emp_areatype;
								
					if($value_emp_subdv != $emp_subdivision && $value_emp_areatype != $emp_areatype){
									
						$emp_areacode	= '';
						$emp_villward	= '';
						$emp_ps			= '';
						$emp_pin		= '';
					}
								
				  }	
						
				}
			}
			
			if ($value_emp_dist != "" && $value_emp_subdv != "" && $value_emp_areatype != "" && !empty($value_emp_state)) {
					
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
					
				$form['bocwa_reg_master']['emp_areacode']= array(
													  '#prefix' 		=> '<div id="emp_name_areatype" class="col-md-4"><label class="select">',
													  '#suffix' 		=> '<i></i></label></div>',
													  '#title'			=> '5.(i) Select '.$areatype_name,
													  '#type' 			=> 'select',
													  '#options' 		=> common_custom_user_block_list_new($value_emp_subdv, $value_emp_areatype),
													  '#default_value' 	=> !empty($emp_areacode) ? $emp_areacode : '',
													  '#attributes'		=> $readonly_emp_info,
													  '#required' 		=> TRUE,
													  '#ajax'			=> array(
																				'event' 	=> 'change',
																				'effect' 	=> 'fade',			
																				'callback'	=> 'get_ajax_emp',
																				'progress'	=> array('type'=> 'throbber'),
																			  )
													);
																		
				$value_emp_areacode 	= isset($form_state['values']['emp_areacode']) ? $form_state['values']['emp_areacode'] : $emp_areacode;
							
				if($value_emp_areacode != $emp_areacode){
							
					$emp_villward	= '';
					$emp_ps			= '';
					$emp_pin		= '';
				
				}
			}
			
			if($value_emp_dist != "" && $value_emp_subdv != "" && $value_emp_areatype != ""  && $value_emp_areacode != "" && !empty($value_emp_state) ){
					
				if ( $value_emp_areatype == 'B' ){
					$area_name		= 'Gram Panchayat';
					$area_code 		= 'V';
				}elseif( $value_emp_areatype == 'M'){
					$area_name		= 'Ward';
					$area_code 		= 'W';
				}elseif( $value_emp_areatype == 'C'){
					$area_name		= 'Ward';
					$area_code 		= 'C';
				}elseif( $value_emp_areatype == 'S'){
					$area_name		= 'Sector';
					$area_code 		= 'S';
				}elseif($value_emp_areatype == 'N'){
					$area_name		= 'Notified Area';
					$area_code 		= 'N';
				}
					
				$form['bocwa_reg_master']['emp_vill_ward'] 	= array(
													  '#prefix' 		=> '<div id="emp_vill_ward" class="col-md-4"><label class="select">',
													  '#suffix' 		=> '<i></i></label></div>',
													  '#title'			=> '5.(j) Select '.$area_name,
													  '#type' 			=> 'select',
													  '#options'		=> common_custom_user_villward_list_new($value_emp_areacode,$area_code),
													  '#default_value' 	=> !empty($emp_villward) ? $emp_villward : '',
													  '#attributes'		=> $readonly_emp_info,
													  '#required' 		=> TRUE,
													  '#ajax'			=> array(
																				'event' 	=> 'change',
																				'effect' 	=> 'fade',			
																				'callback'	=> 'get_ajax_emp',
																				'progress'	=> array('type'=> 'throbber'),
																			  )
													);
																		
				$value_emp_vill_ward 	= isset($form_state['values']['emp_vill_ward']) ? $form_state['values']['emp_vill_ward'] : $emp_villward;	
						
				if($value_emp_vill_ward != $emp_villward){
							
					$emp_ps		= '';
					$emp_pin	= '';
							
				}	
			}
			
			if (!empty($value_emp_state) && $value_emp_dist != "" && $value_emp_subdv != "" && $value_emp_areatype != ""  && $value_emp_areacode != "" && $value_emp_vill_ward != "" ){
				
				$form['bocwa_reg_master']['emp_ps']  = array(
													  '#prefix' 		=> '<div id="emp_ps_replace" class="col-md-4"><label class="select">',
													  '#suffix' 		=> '</label></div>',
													  '#title'			=> '5.(k) Select Police Station',
													  '#type' 			=> 'select',
													  '#options' 		=> common_custom_user_ps_list_new($value_emp_dist),
													  '#default_value' 	=> !empty($emp_ps) ? $emp_ps : '',
													  '#attributes'		=> $readonly_emp_info,
													  '#required' 		=> TRUE,
													);
																										
				$form['bocwa_reg_master']['emp_pin'] = array(
													  '#prefix' 		=> '<div id="emppin_replace" class="col-md-4"><label class="input">',
													  '#suffix' 		=> '</label></div>',
													  '#title'			=> '5.(l) PIN Number',
													  '#type' 			=> 'textfield',
													  '#default_value' 	=> !empty($emp_pin) ? $emp_pin	:'',
													  '#attributes' 	=> $readonly_emp_pin,
													  '#required' 		=> TRUE,
													  '#element_validate'=> array('MISC_PIN_NUMBER'),
												  );	
			}
			
		}else{
			
			$form['bocwa_reg_master']['emp_address'] = array(
												   '#prefix' 			=> '<div id="emp_address_replace" class="col-md-12"><label class="textarea">',
												   '#suffix' 			=> '</label></div>',
												   '#title'				=> 'Address of the Employer [ If other Country/State please provide detail address ]',
												   '#rows'				=> t('2'),	
												   '#type' 				=> 'textarea',
												   '#attributes'		=> $readonlyAddressEmployerTextarea,
												   '#default_value' 	=> !empty($emp_address) ? $emp_address : '',
												   '#required' 			=> TRUE,
												   '#element_validate'	=> array('MISC_TEXTBOX_LOCATION_VALIDATE')
												);
		}
		$form['bocwa_reg_master']['nature_of_build_const']	= array(
												'#prefix' 				=> '</div></div><div class="panel panel-default panel-form"><div class="panel-heading">6. Other Information</div><div class="panel-body"><div class="col-md-6"><label class="input">',
												'#suffix' 				=> '</label></div>',
												'#title'				=> '6.(a) Nature of building or other construction work is to be carried on',
												'#type' 				=> 'textfield',
												'#default_value' 		=> !empty($nature_of_build_const) ? $nature_of_build_const : '',
												'#required' 			=> TRUE,
												'#attributes' 			=> $readonly_nature_wrk,
												'#element_validate'		=> array('MISC_TEXTBOX_TEXT_VALIDATE')
												);
												
		$form['bocwa_reg_master']['max_no_workers']= array(
												'#prefix' 				=> '<div class="col-lg-6 col-md-6"><label class="input">',
												'#suffix' 				=> '</label></div>',
												'#title'				=> '6. b) Maximum number of building workers to be employed on any day',
												'#type' 				=> 'textfield',
												'#default_value' 		=> !empty($max_workers_employed) ? $max_workers_employed : '',
												'#required' 			=> TRUE,
												'#attributes' 			=> $readonly_max_workers,
												'#element_validate'		=> array('MISC_TEXTBOX_NUMERIC_VALIDATE')
												);
												
		$form['bocwa_reg_master']['est_date_of_commencement_building'] 	= array(
												'#prefix' 				=> '<div class="col-lg-6 col-md-6"><label class="input">',
												'#suffix' 				=> '</label></div>',
												'#title'				=> '6. c) Estimated date of commencement of building or other construction work',	
												'#id'					=> 'est_date_of_commencement_id',
												'#type' 				=> 'textfield',
												'#default_value'		=> !empty($commencement) ? $commencement : '',
												'#required' 			=> TRUE,
												'#attributes' 			=> $readonly_commencement,
											);
										
		$form['bocwa_reg_master']['est_date_of_completion_building'] = array(
												'#prefix' 				=> '<div class="col-lg-6 col-md-6"><label class="input">',
												'#suffix' 				=> '</label></div></div></div><div class="form-footer"><div class="col-md-12">',
												'#title'				=> '6. d) Estimated date of the completion of building or other construction work',
												'#id'					=> 'est_date_of_termination_id',
												'#type' 				=> 'textfield',
												'#default_value' 		=> !empty($completion) ? $completion : '',
												'#required' 			=> TRUE,
												'#attributes' 			=> $readonly_completion,
												);
												
		if(empty($fetch_check_flag->amendment_parent_id)){
		
			$form['bocwa_reg_master']['markup_link2']=	array(
													'#prefix' 			=> '',
													'#type' 			=> 'markup',
													'#markup'			=> l('Reset Amendment Fields', 'apply-bocwa/'.$id, array('attributes' => array('html' => TRUE, 'class' => 'link-btn mtop-btm-15')))
												);															
		}																				
		$form['bocwa_reg_master']['submit'] = array(
												
			'#suffix' 				=> '</div></div></div>', 
			'#type' 				=> 'submit',
			'#value' 				=> 'SAVE ',
			'#attributes'			=> array('class' => array('btn btn-primary pull-right'))
			);				
    } 
	return $form;  
}

function amendment_bocwa_form_submit($form, $form_state){
	
	global $base_root, $base_path, $user;
	
	$val 					= $form_state['values'];
	$user_id				= $user->uid;
	
	$application_id			= encryption_decryption_fun('decrypt', $val['application_id']);
	$identification_number	= encryption_decryption_fun('decrypt', $val['identification_number']);
	
	$fetchBocwaLastRow		= db_query("select * from l_bocwa_registration_master where user_id =".$user->uid." and status = 'I' order by id desc limit 1");
	$dataLastRow 			= $fetchBocwaLastRow->fetchAssoc();
	
	$registration_number    = $dataLastRow['registration_number'];
	$lastBocwaApplicationId = $dataLastRow['id'];
	$registration_date		= $dataLastRow['registration_date'];
	$amended_indentification_number		= 'BOCWA_AMEND_'.$lastBocwaApplicationId.'_'.time();
	$amendment_date         = date('Y-m-d');
	$amendment_parent_id    = $lastBocwaApplicationId;

	$date = str_replace('/', '-', $val['est_date_of_commencement_building']);  	
	$est_date_of_commencement_building = date('Y-m-d',strtotime($date));

	$date = str_replace('/', '-', $val['est_date_of_completion_building']);  
	$est_date_of_completion_building = date('Y-m-d',strtotime($date));

	$fieldsRegBocwa 		= array(
								'registration_number'						=>	$registration_number,
								'registration_date'							=>	$registration_date,
								'user_id'									=>	$user_id,
								'identification_number'						=>	$amended_indentification_number,
								'act_id'									=>  2,
								'amendment_parent_id'						=> 	$amendment_parent_id,	
								'e_name'									=>	isset($val['e_name'])? $val['e_name'] : $dataLastRow['e_name'],
								'est_type'									=>  isset($val['est_type'])? $val['est_type'] : $dataLastRow['est_type'],
								'loc_e_name'								=>	$dataLastRow['loc_e_name'],
								'loc_e_dist'								=>	$dataLastRow['loc_e_dist'],
								'loc_e_subdivision'							=>	$dataLastRow['loc_e_subdivision'],
								'loc_e_areatype'							=>	$dataLastRow['loc_e_areatype'],
								'loc_e_areatype_code'						=>	$dataLastRow['loc_e_areatype_code'],
								'loc_e_vill_ward'							=>	$dataLastRow['loc_e_vill_ward'],
								'loc_e_ps'									=>	$dataLastRow['loc_e_ps'],
								'loc_e_pin_number'							=>	$dataLastRow['loc_e_pin_number'],
																						 
								'e_postal_address'							=>	isset($val['e_postal_address'])? $val['e_postal_address'] : $dataLastRow['e_postal_address'],
								'e_postal_dist'								=>	isset($val['e_postal_dist'])? $val['e_postal_dist'] : $dataLastRow['e_postal_dist'],
								'e_postal_subdivision'						=>	isset($val['e_postal_subdivision'])? $val['e_postal_subdivision'] : $dataLastRow['e_postal_subdivision'],
								'e_postal_areatype'							=>	isset($val['e_postal_areatype'])? $val['e_postal_areatype'] : $dataLastRow['e_postal_areatype'], 
								'e_postal_areatype_code'					=>	isset($val['e_postal_areacode'])? $val['e_postal_areacode'] : $dataLastRow['e_postal_areatype_code'], 
								'e_postal_vill_ward'						=>	isset($val['e_postal_villward'])? $val['e_postal_villward'] : $dataLastRow['e_postal_vill_ward'], 
								'e_postal_ps'								=>	isset($val['e_postal_ps'])? $val['e_postal_ps'] : $dataLastRow['e_postal_ps'], 
								'e_postal_pin_number'						=>	isset($val['e_postal_pin'])? $val['e_postal_pin'] : $dataLastRow['e_postal_pin_number'], 
								
								'e_full_name'								=>  isset($val['e_full_name'])? $val['e_full_name'] : $dataLastRow['e_full_name'],
								'e_permanent_address'						=>	isset($val['est_address'])? $val['est_address'] : $dataLastRow['e_permanent_address'],
								'e_permanent_add_dist'						=>  isset($val['est_dist'])? $val['est_dist'] : $dataLastRow['e_permanent_add_dist'],
								'e_permanent_add_subdivision'				=>  isset($val['est_subdivision'])? $val['est_subdivision'] : $dataLastRow['e_permanent_add_subdivision'],
								'e_permanent_add_areatype'					=>  isset($val['est_areatype'])? $val['est_areatype'] : $dataLastRow['e_permanent_add_areatype'],
								'e_permanent_add_areatype_code'				=>	isset($val['est_areacode'])? $val['est_areacode'] : $dataLastRow['e_permanent_add_areatype_code'],
								'e_permanent_add_vill_ward'					=>	isset($val['est_villward'])? $val['est_villward'] : $dataLastRow['e_permanent_add_vill_ward'],
								'e_permanent_add_ps'						=>	isset($val['est_ps'])? $val['est_ps'] : $dataLastRow['e_permanent_add_ps'],
								'e_permanent_add_pin_number'				=>  isset($val['est_pin'])? $val['est_pin'] : $dataLastRow['e_permanent_add_pin_number'],
								
								'emp_name'									=>  isset($val['emp_name'])? $val['emp_name'] : $dataLastRow['emp_name'],
								'emp_gender'								=>  isset($val['emp_gender'])? $val['emp_gender'] : $dataLastRow['emp_gender'],
								'emp_country'								=>	isset($val['emp_country'])? $val['emp_country'] : 1,
								'emp_state'									=>	isset($val['emp_state'])? $val['emp_state'] : 1,
								'emp_address'								=>	isset($val['emp_address'])? $val['emp_address'] : $dataLastRow['emp_address'],
								'emp_dist'									=>  isset($val['emp_dist'])? $val['emp_dist'] : $dataLastRow['emp_dist'],
								'emp_subdivision'							=>  isset($val['emp_subdv'])? $val['emp_subdv'] : $dataLastRow['emp_subdivision'],
								'emp_areatype'								=>  isset($val['emp_areatype'])? $val['emp_areatype'] : $dataLastRow['emp_areatype'],
								'emp_areatype_code'							=>	isset($val['emp_areacode'])? $val['emp_areacode'] : $dataLastRow['emp_areatype_code'],
								'emp_vill_ward'								=>	isset($val['emp_vill_ward'])? $val['emp_vill_ward'] : $dataLastRow['emp_vill_ward'],
								'emp_ps'									=>	isset($val['emp_ps'])? $val['emp_ps'] : $dataLastRow['emp_ps'],
								'emp_pin_number'							=>  isset($val['emp_pin'])? $val['emp_pin'] : $dataLastRow['emp_pin_number'],
								
								'full_name_manager'							=>	isset($val['full_name_manager'])? $val['full_name_manager'] : $dataLastRow['full_name_manager'],
								'manager_country'							=>	isset($val['manager_country'])? $val['manager_country'] : 1,
								'manager_state'								=>	isset($val['manager_state'])? $val['manager_state'] : 1,
								'address_manager'							=>	isset($val['address_manager'])? $val['address_manager'] : $dataLastRow['address_manager'],
								'address_manager_dist'						=>	isset($val['manager_dist'])? $val['manager_dist'] : $dataLastRow['address_manager_dist'],
								'address_manager_subdivision'				=>  isset($val['manager_subdv'])? $val['manager_subdv'] : $dataLastRow['address_manager_subdivision'],
								'address_manager_areatype'					=> 	isset($val['manager_areatype'])? $val['manager_areatype'] : $dataLastRow['address_manager_areatype'],
								'address_manager_areatype_code'				=>	isset($val['manager_areacode'])? $val['manager_areacode'] : $dataLastRow['address_manager_areatype_code'],
								'address_manager_vill_ward'					=>	isset($val['manager_vill_ward'])? $val['manager_vill_ward'] : $dataLastRow['address_manager_vill_ward'],
								'address_manager_ps'						=>	isset($val['manager_ps'])? $val['manager_ps'] : $dataLastRow['address_manager_ps'],
								'address_manager_pin_number'				=>  isset($val['manager_pin'])? $val['manager_pin'] : $dataLastRow['address_manager_pin_number'],
								'nature_of_build_const'						=>	isset($val['nature_of_build_const'])? $val['nature_of_build_const'] : $dataLastRow['nature_of_build_const'],
								'max_no_of_building_workers_employed'		=>	isset($val['max_no_workers'])? $val['max_no_workers'] : $dataLastRow['max_no_of_building_workers_employed'],
								'est_date_of_commencement_building'			=>	isset($val['est_date_of_commencement_building'])? $est_date_of_commencement_building : $dataLastRow['est_date_of_commencement_building'],
								'est_date_of_completion_building'			=>	isset($val['est_date_of_completion_building'])? $est_date_of_completion_building : $dataLastRow['est_date_of_completion_building'],
								
								'user_from_portal'							=>  1
											);
										
		if(!empty($dataLastRow) ){ 
		
			$fetchAmendedData = db_query("select id, status FROM l_bocwa_registration_master WHERE user_id = '".$user->uid."' order by id desc limit 1")->fetchAssoc();
			
			if($fetchAmendedData['status'] != 'I' && $application_id == $fetchAmendedData['id']){	
			
				$query = db_update('l_bocwa_registration_master');
				$query->fields($fieldsRegBocwa); 
				$query->condition('id', $application_id);
				$query->condition('user_id', $user_id);
				$query->execute();
				
				$message = 'Application updated successfully';
				drupal_set_message(t($message));
				
			}else{
			
			$newAmendedBocwaAppId = db_insert('l_bocwa_registration_master')->fields($fieldsRegBocwa)->execute();
			$fetchBocwaUpload 	  = db_query("select * from l_documents_upload where application_id=:application_id and user_id=:user_id  and act_id=:act_id", array( ':application_id' => $amendment_parent_id, ':user_id' => $user_id, ':act_id' => 2 ))->fetchObject();
		
			if(!empty($fetchBocwaUpload)){ 	
			
				$trade_license_file_old						=	$fetchBocwaUpload->trade_license_file;
				$article_of_assoc_file_old					=	$fetchBocwaUpload->article_of_assoc_file;
				$memorandum_of_cert_file_old				=	$fetchBocwaUpload->memorandum_of_cert_file;
				$partnership_deed_file_old					=	$fetchBocwaUpload->partnership_deed_file;
				$challan_file_old							=   $fetchBocwaUpload->challan_file;
				$work_order_file_old						=	$fetchBocwaUpload->work_order_file;
				$form_one_asses_ses_file_old				=   $fetchBocwaUpload->form_one_asses_ses_file;
				$supp_asses_ses_file_old					=	$fetchBocwaUpload->supp_asses_ses_file;
				$other_doc_file_old							=	$fetchBocwaUpload->other_doc_file;
				$bocwa_address_proof_file_old				=	$fetchBocwaUpload->address_proof_file;
			
				
			
				$fieldsBocwaDocumentsData = array( 
											'user_id'									=>	$user->uid,
											'act_id'									=>  2,
											'application_id'							=>  $newAmendedBocwaAppId	,
											'identification_number'						=>	$amended_indentification_number,
											'trade_license_file' 						=>  $trade_license_file_old,
											'article_of_assoc_file' 					=>  $article_of_assoc_file_old,
											'memorandum_of_cert_file' 					=>  $memorandum_of_cert_file_old,
											'partnership_deed_file' 					=>  $partnership_deed_file_old,
											'challan_file'								=>  $challan_file_old,
											'work_order_file'							=>  $work_order_file_old,
											'form_one_asses_ses_file'					=>  $form_one_asses_ses_file_old,
											'supp_asses_ses_file'						=>  $supp_asses_ses_file_old,
											'other_doc_file'							=>  $other_doc_file_old,
											'address_proof_file'						=>	$bocwa_address_proof_file_old,
										);
				
				
			
				db_insert('l_documents_upload')->fields($fieldsBocwaDocumentsData)->execute();
			}
			$message			= 'Application saved Successfully';
			drupal_set_message(t($message));
			if($newAmendedBocwaAppId != ""){
				drupal_goto('amendment-bocwa/bocwa-application/'.encryption_decryption_fun('encrypt', $newAmendedBocwaAppId));
			}	
		}
	}
}

function my_ajax_callback($form, $form_state){
	$commands	=	array();
	$commands[] = ajax_command_replace('#district_div', drupal_render($form['bocwa_reg_master']['loc_e_district']));
	$commands[] = ajax_command_replace('#subdivision_div', drupal_render($form['bocwa_reg_master']['loc_e_subdivision']));
	$commands[] = ajax_command_replace('#area_type_div', drupal_render($form['bocwa_reg_master']['loc_e_areatype']));
	$commands[] = ajax_command_replace('#area_code_div', drupal_render($form['bocwa_reg_master']['loc_e_areacode']));
	$commands[] = ajax_command_replace('#village_div', drupal_render($form['bocwa_reg_master']['loc_e_villward']));
	$commands[] = ajax_command_replace('#police_station_div', drupal_render($form['bocwa_reg_master']['loc_e_ps']));
	$commands[] = ajax_command_replace('#pin_code_div', drupal_render($form['bocwa_reg_master']['loc_e_pin']));
	return array('#type' => 'ajax', '#commands' => $commands);
}
function get_ajax_callback_e_postal($form, $form_state){
	$commands	=	array();
	//$commands[] = ajax_command_replace('#postal_district_div', drupal_render($form['bocwa_reg_master']['e_postal_dist']));
	$commands[] = ajax_command_replace('#postal_subdivision_div', drupal_render($form['bocwa_reg_master']['e_postal_subdivision']));
	$commands[] = ajax_command_replace('#postal_areatype_div', drupal_render($form['bocwa_reg_master']['e_postal_areatype']));
	$commands[] = ajax_command_replace('#postal_areacode_div', drupal_render($form['bocwa_reg_master']['e_postal_areacode']));
	$commands[] = ajax_command_replace('#postal_vill_div', drupal_render($form['bocwa_reg_master']['e_postal_villward']));
	$commands[] = ajax_command_replace('#postal_ps_div', drupal_render($form['bocwa_reg_master']['e_postal_ps']));
	$commands[] = ajax_command_replace('#postal_pin_div', drupal_render($form['bocwa_reg_master']['e_postal_pin']));
	return array('#type' => 'ajax', '#commands' => $commands);	
}
function est_ajax_callback($form, $form_state){
	$commands	=	array();
	$commands[] = ajax_command_replace('#est_subdivision_div', drupal_render($form['bocwa_reg_master']['est_subdivision']));
	$commands[] = ajax_command_replace('#est_areatype_div', drupal_render($form['bocwa_reg_master']['est_areatype']));
	$commands[] = ajax_command_replace('#est_areacode_div', drupal_render($form['bocwa_reg_master']['est_areacode']));
	$commands[] = ajax_command_replace('#est_vill_div', drupal_render($form['bocwa_reg_master']['est_villward']));
	$commands[] = ajax_command_replace('#est_ps_div', drupal_render($form['bocwa_reg_master']['est_ps']));
	$commands[] = ajax_command_replace('#est_pin_div', drupal_render($form['bocwa_reg_master']['est_pin']));
	return array('#type' => 'ajax', '#commands' => $commands);	

}
function get_ajax_manager($form, $form_state) {
	$commands = array();
	$commands[] = ajax_command_replace('#manager_state_replace', drupal_render($form['bocwa_reg_master']['manager_state']));
	$commands[] = ajax_command_replace('#manager_district_replace', drupal_render($form['bocwa_reg_master']['manager_dist']));
	$commands[] = ajax_command_replace('#manager_subdv_replace', drupal_render($form['bocwa_reg_master']['manager_subdv']));
	$commands[] = ajax_command_replace('#manager_areatype', drupal_render($form['bocwa_reg_master']['manager_areatype']));
	$commands[] = ajax_command_replace('#manager_name_areatype', drupal_render($form['bocwa_reg_master']['manager_areacode']));
	$commands[] = ajax_command_replace('#manager_vill_ward', drupal_render($form['bocwa_reg_master']['manager_vill_ward']));
	$commands[] = ajax_command_replace('#manager_ps_replace', drupal_render($form['bocwa_reg_master']['manager_ps']));
	$commands[] = ajax_command_replace('#managerpin_replace', drupal_render($form['bocwa_reg_master']['manager_pin']));
  	return array('#type' => 'ajax', '#commands' => $commands);
}
function get_ajax_emp($form, $form_state){
	$commands = array();
	$commands[] = ajax_command_replace('#emp_state_replace', drupal_render($form['bocwa_reg_master']['emp_state']));
	$commands[] = ajax_command_replace('#emp_district_replace', drupal_render($form['bocwa_reg_master']['emp_dist']));
	$commands[] = ajax_command_replace('#emp_subdv_replace', drupal_render($form['bocwa_reg_master']['emp_subdv']));
	$commands[] = ajax_command_replace('#emp_areatype', drupal_render($form['bocwa_reg_master']['emp_areatype']));
	$commands[] = ajax_command_replace('#emp_name_areatype', drupal_render($form['bocwa_reg_master']['emp_areacode']));
	$commands[] = ajax_command_replace('#emp_vill_ward', drupal_render($form['bocwa_reg_master']['emp_vill_ward']));
	$commands[] = ajax_command_replace('#emp_ps_replace', drupal_render($form['bocwa_reg_master']['emp_ps']));
	$commands[] = ajax_command_replace('#emppin_replace', drupal_render($form['bocwa_reg_master']['emp_pin']));
	return array('#type' => 'ajax', '#commands' => $commands);
}
function get_ajax_country_manager($form, $form_state){
	$commands = array();
	$commands[] = ajax_command_replace('#address_manager_replace', drupal_render($form['bocwa_reg_master']['address_manager']));
	$commands[] = ajax_command_replace('#manager_state_replace', drupal_render($form['bocwa_reg_master']['manager_state']));
	$commands[] = ajax_command_replace('#manager_district_replace', drupal_render($form['bocwa_reg_master']['manager_dist']));
	$commands[] = ajax_command_replace('#manager_subdv_replace', drupal_render($form['bocwa_reg_master']['manager_subdv']));
	$commands[] = ajax_command_replace('#manager_areatype', drupal_render($form['bocwa_reg_master']['manager_areatype']));
	$commands[] = ajax_command_replace('#manager_name_areatype', drupal_render($form['bocwa_reg_master']['manager_areacode']));
	$commands[] = ajax_command_replace('#manager_vill_ward', drupal_render($form['bocwa_reg_master']['manager_vill_ward']));
	$commands[] = ajax_command_replace('#manager_ps_replace', drupal_render($form['bocwa_reg_master']['manager_ps']));
	$commands[] = ajax_command_replace('#managerpin_replace', drupal_render($form['bocwa_reg_master']['manager_pin']));
	return array('#type' => 'ajax', '#commands' => $commands);
}
function get_ajax_country_emp($form, $form_state){
	$commands = array();
	$commands[] = ajax_command_replace('#emp_address_replace', drupal_render($form['bocwa_reg_master']['emp_address']));
	$commands[] = ajax_command_replace('#emp_state_replace', drupal_render($form['bocwa_reg_master']['emp_state']));
	$commands[] = ajax_command_replace('#emp_district_replace', drupal_render($form['bocwa_reg_master']['emp_dist']));
	$commands[] = ajax_command_replace('#emp_subdv_replace', drupal_render($form['bocwa_reg_master']['emp_subdv']));
	$commands[] = ajax_command_replace('#emp_areatype', drupal_render($form['bocwa_reg_master']['emp_areatype']));
	$commands[] = ajax_command_replace('#emp_name_areatype', drupal_render($form['bocwa_reg_master']['emp_areacode']));
	$commands[] = ajax_command_replace('#emp_vill_ward', drupal_render($form['bocwa_reg_master']['emp_vill_ward']));
	$commands[] = ajax_command_replace('#emp_ps_replace', drupal_render($form['bocwa_reg_master']['emp_ps']));
	$commands[] = ajax_command_replace('#emppin_replace', drupal_render($form['bocwa_reg_master']['emp_pin']));
	return array('#type' => 'ajax', '#commands' => $commands);
}