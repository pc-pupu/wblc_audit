<?php
drupal_add_js(drupal_get_path('module', 'applicant_forms') . '/js/upload_custom.js');
function edit_contractor_form($form, &$form_state, $con_id='', $id='') {
		
	drupal_add_library('system','ui.datepicker');
	drupal_add_js(drupal_get_path('module', 'applicant_forms') . '/js/custom.js');
	
	
	global $base_root, $base_path, $user; 
	
	$user_id 			= $user->uid;
	$application_id		= encryption_decryption_fun('decrypt', $id);
	$contractor_id		= encryption_decryption_fun('decrypt', $con_id); 
	
	$resultCon 			= db_select('l_contractor_info_master', 'lcim')->fields('lcim')->condition('lcim.id',$contractor_id)->execute();
	$dataCon 			= $resultCon->fetchAssoc(); 
	
	$fetchReg 			= db_select('l_clra_registration_master', 'lcrm')->fields('lcrm', array('status','final_submit_status','identification_number'))->condition('lcrm.user_id', $user_id)->condition('lcrm.id', $application_id)->execute();
	$resultReg			= $fetchReg->fetchAssoc();
	
	$query_get_each_nature_work  = 	db_query("select a.contractor_nature_of_work,b.contractor_work_id, b.cont_work_name from l_clra_con_nature_of_work a left join contractor_works as b on a.contractor_nature_of_work = b.contractor_work_id where a.application_id =:application_id and a.user_id=:user_id and b.is_default =:is_default and a.contractor_id =:contractor_id order by a.id desc", array( ':user_id' => $user_id , ':application_id' => $application_id , ':is_default' => '1' ,':contractor_id' =>  $contractor_id )); 

		
	$resultChunk	=	 array();		
	if(!empty($query_get_each_nature_work)){
		foreach($query_get_each_nature_work as $listData){ 
				$resultChunk[] = $listData->contractor_work_id;
		}
	}
	
	$form['contractor_edit']['header']					= array(
															'#markup' 			=> t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">Particulars of Contractors and Contract Labour</div><div class="panel-body">'),
																					);
	$form['contractor_edit']['application_id'] 			= array(
															  '#type' 			=> 'hidden',
															  '#default_value'	=> !empty($application_id) ? $id : '',
															  '#attributes' 	=> array('readonly'=>'readonly')
															);
															
	$form['contractor_edit']['contractor_id'] 			= array(
															  '#type' 			=> 'hidden',
															  '#default_value'	=> !empty($contractor_id) ? $con_id : '',
															  '#attributes' 	=> array('readonly'=>'readonly')
															);
															
	$form['contractor_master']['contractor_type'] 		= array(														  
															 '#type' 			=> 'hidden',
															 '#default_value'	=> 2														  
															);
						
	$form['contractor_edit']['identification_number']   = array(
															  '#type' 			=> 'hidden',
															  '#default_value'	=> !empty($resultReg['identification_number']) ? $resultReg['identification_number']: '',
															  '#attributes' 	=> array('readonly'=>'readonly')
															);
															
	$form['contractor_edit']['name_of_contractor'] 		= array(
															  '#prefix' 		=> '<div class="col-md-6"><label class="input">',
															  '#suffix' 		=> '</label></div>',
															  '#title'			=> '2. Name of the Contractor',
															  '#type' 			=> 'textfield',
															  '#required' 		=> TRUE,
															  '#default_value' 	=> $dataCon['name_of_contractor'],
															  '#element_validate'=> array('MISC_TEXTBOX_TEXT_ENAME_VALIDATE'),
															);
															
	$form['contractor_edit']['email_of_contractor'] 	= array(
															  '#prefix' 		=> '<div class="col-md-6"><label class="input">',
															  '#suffix' 		=> '</label></div>',
															  '#title'  		=> '3. Email of the Contractor',
															  '#type'     		=> 'textfield',
															  '#required' 		=> TRUE,
															  '#default_value' 	=> $dataCon['email_of_contractor'],
															  '#element_validate'=> array('MISC_EMAIL_VALIDATE')
															);
															
	$form['contractor_edit']['state_opts']				=	array(
															'#title'			=>	'3. State',
															'#prefix'			=>	'<div class="col-md-6"><label class="select">',
															'#suffix' 			=> '</label></div>',
															'#type'				=>	'select', 
															'#options'			=>	array( 1 => 'West Bengal', 2 => 'Other State' ),
															'#attributes'		=>  $readonly_address,
															'#default_value' 	=>  !empty($dataCon['state_opts']) ? $dataCon['state_opts']:'',
															'#required'			=>	TRUE,
														);	
															
	$form['contractor_edit']['state_opts']				=	array(
															'#title'			=>	'4. State',
															'#prefix'			=>	'<div class="col-md-6"><label class="select">',
															'#suffix' 			=> '</label></div>',
															'#type'				=>	'select', 
															'#options'			=>	array( 1 => 'West Bengal', 2 => 'Other State' ),
															'#attributes'		=>  $readonly_address,
															'#default_value' 	=>  !empty($dataCon['state_opts']) ? $dataCon['state_opts']:'',
															'#required'			=>	TRUE,
														);	
															
	$form['contractor_edit']['state'] 				= array(
															'#prefix' 			=> '<div class="col-md-6"><label class="select">',
															'#suffix' 			=> '</label></div>',
															'#title'			=> '5. Select State',
															'#type' 			=> 'select',
															'#options' 			=> get_all_state(1),
															'#required' 		=> TRUE,
															'#attributes'		=> $readonly_address,
															'#default_value' 	=> !empty($dataCon['state']) ? $dataCon['state']	:'',
															'#ajax'				=> array(
																						'event' 		=> 'change',
																						'effect' 		=> 'fade',			
																						'callback'		=> 'my_ajax_callback',
																						'progress'		=>	'',
																					)
																);
																
	$value_state_opts 									= isset($form_state['values']['state']) ? $form_state['values']['state'] : $dataCon['state'];
	
	$form['contractor_edit']['address']					= array(
															'#prefix' 			=> '<div id="address_div">',
															'#suffix' 			=> '</div>',
															);

	$form['contractor_edit']['district']				= array(
															'#prefix' 			=> '<div id="district_div">',
															'#suffix' 			=> '</div>',
															);	
																	
	$form['contractor_edit']['subdivision']				= array(
															'#prefix' 			=> '<div id="subdivision_div">',
															'#suffix' 			=> '</div>',
															);	
																
	$form['contractor_edit']['area_type']				= array(
															'#prefix' 			=> '<div id="area_type_div">',
															'#suffix' 			=> '</div>',
															);
																
	$form['contractor_edit']['area_code']				= array(
															'#prefix' 			=> '<div id="area_code_div">',
															'#suffix' 			=> '</div>',
															);
																
	$form['contractor_edit']['village']					= array(
															'#prefix' 			=> '<div id="village_div">',
															'#suffix' 			=> '</div>',
															);
																
	$form['contractor_edit']['police_station']			= array(
															'#prefix' 			=> '<div id="police_station_div">',
															'#suffix' 			=> '</div>',
															);	
																
	$form['contractor_edit']['pin_code']				= array(
															'#prefix' 			=> '<div id="pin_code_div">',
															'#suffix' 			=> '</div>',
															);
															
	if( (!empty($value_state_opts) && $value_state_opts != '') && $value_state_opts ==  1) {
		
		$form['contractor_edit']['address'] 			 = array(
															'#prefix' 			=> '<div id="address_div" class="col-md-8 min-height-80"><label class="input">',
															'#suffix' 			=> '</label></div>',
															'#title'			=> '5.(a) Address Line1',
															'#type' 			=> 'textfield',
															'#required' 		=> TRUE,
															'#default_value' 	=> $dataCon['address_of_contractor'],
															'#element_validate'	=> array('MISC_TEXTBOX_LOCATION_VALIDATE'),
															);
															
															
		$db_district_value	= db_query("SELECT COUNT(id) FROM {district_master} WHERE state_id = '".$value_state_opts."'")->fetchObject();
			
		if(!empty($db_district_value) && ($db_district_value->count>0) ) {
			
			$form['contractor_edit']['district']	 = array(
															 '#prefix' 			=> '<div id="district_div" class="col-md-4"><label class="select">',
															 '#suffix' 			=> '</label></div>',
															 '#title'			=> t('5.(b) Select District'),
															 '#type' 			=> 'select',
															 '#options' 		=> get_district_by_id($value_state_opts),
															 '#default_value' 	=> !empty($dataCon['con_loc_e_dist']) ? $dataCon['con_loc_e_dist']:'',
															 '#required' 		=> TRUE,
															 '#ajax'			=> array(
																						'event'		=> 'change',
																						'effect' 	=> 'fade',
																						'callback'	=> 'my_ajax_callback',
																						'progress'	=>	'',
																						)
															);	
				
			$value_district = isset($form_state['values']['district']) ? $form_state['values']['district'] : $dataCon['con_loc_e_dist'];
		}
		
		if($value_district != ''  && $value_state_opts!=0){
				
			$db_subdivision_value	= db_query("SELECT COUNT(id) as subdv_total_manager  FROM {sub_division} WHERE district_code = '".$value_district."'")->fetchObject();
			
			if(!empty($db_subdivision_value) && $db_subdivision_value != '' && $db_subdivision_value->subdv_total_manager > 0) {
				
				$form['contractor_edit']['subdivision']	 = array(
															  '#prefix' 		=> '<div id="subdivision_div" class="col-md-4"><label class="select">',
															  '#suffix' 		=> '</label></div>',
															  '#title'			=> '5.(c) Select Subdivision',
															  '#type' 			=> 'select',
															  '#options' 		=> common_custom_user_subdivisionlist_list_new($value_district),
															  '#default_value' 	=> !empty($dataCon['con_loc_e_subdivision']) ? $dataCon['con_loc_e_subdivision']:'',
															  '#required' 		=> TRUE,
															  '#ajax'			=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'my_ajax_callback',
																						'progress'			=> '',
																					  ),
														);
													
				$value_subdivision	= isset($form_state['values']['subdivision']) ? $form_state['values']['subdivision'] : $dataCon['con_loc_e_subdivision'];
			}
		}
		
		if($value_district != "" && !empty($value_state_opts) && $value_subdivision != ""){
			
			$db_block_mun_value	 = db_query("SELECT COUNT(id) as areatype_total FROM {block_mun_master} WHERE distict_code = '".$value_district."' and sub_division_code  = '".$value_subdivision."'")->fetchObject();
		
			if(!empty($db_block_mun_value) &&  $db_block_mun_value != '' && $db_block_mun_value->areatype_total > 0){
			
				$areatypeOptions	= common_custom_user_areatype_list_new($value_district, $value_subdivision);
				
				if(!empty($areatypeOptions)){
					
				 $form['contractor_edit']['area_type']   = array(
															  '#prefix' 		=> '<div id="area_type_div" class="col-md-4"><label class="select">',
															  '#suffix' 		=> '</label></div>',
															  '#title'			=> '5.(d) Select Block / Municipality / Corporation / SEZ / Notified Area',
															  '#type' 			=> 'select',
															  '#options' 		=> $areatypeOptions,
															  '#default_value' 	=> !empty($dataCon['con_loc_e_areatype']) ? $dataCon['con_loc_e_areatype']:'',
															  '#required' 		=> TRUE,
															  '#ajax'			=> array(
																					'event' 			=> 'change',
																					'effect' 			=> 'fade',			
																					'callback'			=> 'my_ajax_callback',
																					'progress'			=> '',
																				  ),
															);
				 $value_area_type 	= isset($form_state['values']['area_type']) ? $form_state['values']['area_type'] : $dataCon['con_loc_e_areatype'];
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
		
			$form['contractor_edit']['area_code'] 		= array(
															  '#prefix' 		=> '<div id="area_code_div" class="col-md-4"><label class="select">',
															  '#suffix' 		=> '</label></div>',
															  '#title'			=> '5.(e) Select '.$areatype_name,
															  '#type' 			=> 'select',
															  '#options' 		=> common_custom_user_block_list_new($value_subdivision, $value_area_type),
															  '#default_value' 	=> !empty($dataCon['con_name_areatype']) ? $dataCon['con_name_areatype']:'',
															  '#required' 		=> TRUE,
															  '#ajax'			=> array(
																					'event' 			=> 'change',
																					'effect' 			=> 'fade',			
																					'callback'			=> 'my_ajax_callback',
																					'progress'			=> '',
																				  )
												 );
													 
		$value_area_code 	= isset($form_state['values']['area_code']) ? $form_state['values']['area_code'] : $dataCon['con_name_areatype'];
	}
	
	if($value_district != "" && $value_subdivision != "" && $value_area_type != "" && $value_area_code != "" && !empty($value_state_opts) ){
			
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
			
		$form['contractor_edit']['village'] 		 	= array(
															  '#prefix' 		=> '<div id="village_div" class="col-md-4"><label class="select">',
															  '#suffix' 		=> '</label></div>',
															  '#title'			=> '5.(f) Select '.$area_name,
															  '#type' 			=> 'select',
															  '#options'		=> common_custom_user_villward_list_new($value_area_code,$areatypecode),
															  '#default_value' 	=> !empty($dataCon['con_loc_e_vill_ward']) ? $dataCon['con_loc_e_vill_ward']:'',
															  '#required' 		=> TRUE,
															  '#ajax'			=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'my_ajax_callback',
																						'progress'			=> '',
																					  )
															);		
		$value_village 	= isset($form_state['values']['village']) ? $form_state['values']['village'] : $dataCon['con_loc_e_vill_ward'];	
	}
	
	if ($value_state_opts != '0' && $value_district != "" && $value_subdivision != "" && $value_area_type != "" && $value_area_code != ""  && $value_village != "" ){
			
		$form['contractor_edit']['police_station'] 		= array(
															  '#prefix' 		=> '<div id="police_station_div" class="col-md-4"><label class="select">',
															  '#suffix' 		=> '</label></div>',
															  '#title'			=> '5.(g) Select Police Station',
															  '#type' 			=> 'select',
															  '#options' 		=> common_custom_user_ps_list_new($value_district),
															  '#default_value' 	=> !empty($dataCon['con_l_e_ps']) ? $dataCon['con_l_e_ps']:'',
															  '#required' 		=> TRUE,
														  );
																							  
																							  
		$form['contractor_edit']['pin_code'] 			= array(
															  '#prefix' 		=> '<div id="pin_code_div" class="col-md-4"><label class="input">',
															  '#suffix' 		=> '</label></div>',
															  '#title'			=> '5.(h)Pin Code',
															  '#type' 			=> 'textfield',
															  '#required' 		=> TRUE,
															  '#attributes' 	=>  array('maxlength' => 6),
															  '#default_value' => !empty($dataCon['contractor_pin']) ? $dataCon['contractor_pin']:'',
															  '#element_validate'=> array('MISC_PIN_NUMBER'),
														  );	
	}
		
	}else{
		
		$form['contractor_edit']['address'] 			= array(
															'#prefix' 			=> '<div class="col-md-12" id="address_div"><label class="textarea">',
															'#suffix' 			=> '</label></div>',
															'#title'			=> '5.(a) Address Line1 [ If other State please provide detail address ]',
															'#rows'				=> t('2'),
															'#cols'				=> t('146'),	
															'#type' 			=> 'textarea',
															'#default_value' 	=> $dataCon['address_of_contractor'],
															'#required' 		=> TRUE,
															'#element_validate'	=> array('MISC_TEXTBOX_LOCATION_VALIDATE'),
															);
	}
	$options_cont 	= 	contractor_works_multiSelect_options();
	$options_cont 	=  	array(0 => "Select Work") + $options_cont;
	
	$form['contractor_edit']['con_nature_of_work'] 	 	= array( 
															  '#prefix' 		=> '<div class="col-md-8"><label class="select">',
															  '#suffix' 		=> '</label></div>',
															  '#title'			=> '6.Nature of Work in which Contract Labour is Employed or is to be Employed',
															  '#type' 			=> 'select',
															  '#options' 		=> $options_cont,
															  '#required' 		=> TRUE,
															  '#default_value'	=> $resultChunk,
															  '#multiple' 		=> TRUE,
															  '#id' 			=> 'e_nature_of_work_id',
															  '#attributes' 	=> array('style'=>"height: 80px;", 'class'=> array('selected disabled'),'onchange' => 'nature_wrk_others(this.value)'),
														);
	if(!empty($dataCon['other_nature_work'])){												
	$form['contractor_edit']['other_nature_of_work_value'] = array(
															 '#prefix' 			=> '<div class="col-md-12" id ="edit_other_nature_work_id"><label class="input">',
															 '#suffix' 			=> '</label></div>',
															 '#title'			=> 'Other Option for Nature of Work',
															 '#type' 			=> 'textfield',
															 '#default_value' 	=> $dataCon['other_nature_work'],
															 '#id' 				=> 'other_value_nature_of_work_id',
															 '#element_validate' => array('MISC_OTHERNATURE_TEXTBOX_TEXT_VALIDATE')
															);
	}else{
		$form['contractor_edit']['other_nature_of_work_value'] = array(
															 '#prefix' 			=> '<div class="col-md-12" id ="edit_other_nature_work_id"><label class="input">',
															 '#suffix' 			=> '</label></div>',
															 '#title'			=> 'Other Option for Nature of Work',
															 '#type' 			=> 'textfield',
															 '#id' 				=> 'other_value_nature_of_work_id',
															 '#element_validate' => array('MISC_OTHERNATURE_TEXTBOX_TEXT_VALIDATE')
															);
	}
															
	$form['contractor_edit']['max_no_of_labours']   	= array(
															  '#prefix' 		=> '<div class="col-md-12"><label class="input">',
															  '#suffix' 		=> '</label></div>',
															  '#title'			=> '7. Maximum Number of Contractor Labour to be Employed on any day Through Each Contractor to be engaged',
															  '#type' 			=> 'textfield',
															  '#required'		=> TRUE,
															  '#default_value' 	=> $dataCon['contractor_max_no_of_labours_on_any_day'],
															  '#element_validate'=> array('MISC_TEXTBOX_NUMERIC_VALIDATE')
															);
															
	$form['contractor_edit']['est_date_of_work'] 		= array(
															  '#prefix' 		=> '<div class="col-md-12"><label class="input">',
															  '#suffix' 		=> '</label>',
															  '#type' 			=> 'markup',
															  '#markup'			=> '8. Estimated Date of Employment of Each Contract Work Under Each Contractor <span style="color:#f00;">* </span>',
																);

	$form['contractor_edit']['from_date'] 			   	= array(
																'#prefix' 		=> '<div class="row"><div class="col-md-4"><label class="input">',
																'#suffix' 		=> '</label></div>',
																'#type' 		=> 'textfield',
																//'#required' 	=> TRUE,
																'#attributes'	=> array('id' => 'est_date_of_commencement_id', 'readonly'=>'readonly','placeholder' => t('From'), 'onclick' => 'return datepicker_func();'),
																'#default_value'=> !empty($dataCon['est_date_of_work_of_each_labour_from_date']) ? date('d-m-Y',strtotime($dataCon['est_date_of_work_of_each_labour_from_date'])): '',
																
																);
						
	$form['contractor_edit']['to_date'] 				= array(
																'#prefix' 		=> '<div class="col-md-4"><label class="input">',
																'#suffix' 		=> '</label></div>',
																'#type' 		=> 'textfield',
																//'#required' 	=> TRUE,
																'#attributes'	=>	array('id' => 'est_date_of_termination_id','readonly'=>'readonly', 'placeholder'=> t('To'),'onclick' => 'datepicker_func();'),
																'#default_value' => !empty($dataCon['est_date_of_work_of_each_labour_to_date']) ? date('d-m-Y',strtotime($dataCon['est_date_of_work_of_each_labour_to_date'])) : ''
																);
															
$form['contractor_edit']['total_months']				= array(
																'#prefix' 		=> '<div class="col-md-4"><label class="input">',
																'#suffix' 		=> '</label></div></div></div></div></div>',
																'#type' 		=> 'textfield',
																'#default_value'=> $dataCon['est_date_of_work_of_each_labour_total_months'],
																'#attributes'	=>	array('id'=> 'est_date_of_work_of_each_labour_total_months_edit_id', 'readonly'=>'readonly','placeholder' => t('Total Days')),
																	);													
															
	
$form['contractor_edit']['submit'] 						= array(
																'#type' 		=> 'submit',
																'#attributes' 	=> array('class' => array('btn btn-primary pull-right')),
																'#value' 		=> 'UPDATE'
															   );	
	
$form['contractor_edit']['back_link'] 					= array(
																'#type' 		=> 'markup',
																'#markup'	    => l('BACK TO CONTRACTOR LIST', 'view-clra-application-details/clra-contractor-info/'.$id, array('attributes' => array('class' => 'link-btn mleft-15')))
																);	
	return $form;
}

	
	
function edit_contractor_form_submit($form, &$form_state){
		
	global $base_root, $base_path, $user;
	
	$increment 			= 0;
	$val 				= $form_state['values'];
	$contractor_id		= encryption_decryption_fun('decrypt', $val['contractor_id']);
	$application_id		= encryption_decryption_fun('decrypt', $val['application_id']);
	$conNatureWork		= $val['con_nature_of_work'];
	
	$fieldsConInfo 		= array( 
							'application_id'								=>  $application_id,
							
							'contractor_type'								=>	$val['contractor_type'],
							'name_of_contractor'							=>	$val['name_of_contractor'],
							'address_of_contractor'							=>	$val['address'],
							'email_of_contractor'							=>  $val['email_of_contractor'],
							
							
							'con_loc_e_dist'								=>	$val['district'],
							'con_loc_e_subdivision'							=>	$val['subdivision'],
							'con_loc_e_areatype'							=>	$val['area_type'],
							'con_name_areatype'								=>	$val['area_code'],
							'con_loc_e_vill_ward'							=>	$val['village'],
							'con_l_e_ps'									=>	$val['police_station'],
							'contractor_pin'								=>  $val['pin_code'],
							'state_opts'									=>	$val['state_opts'],
							'state'											=>	$val['state'],
							
							'contractor_max_no_of_labours_on_any_day'		=>	$val['max_no_of_labours'],
							'est_date_of_work_of_each_labour_from_date'		=>	date('Y-m-d',strtotime($val['from_date'])), 
							'est_date_of_work_of_each_labour_to_date'		=>  date('Y-m-d',strtotime($val['to_date'])),
							'est_date_of_work_of_each_labour_total_months'	=>  $val['total_months'],
							'status'										=>	1,
							'other_nature_work'								=> $val['other_nature_of_work_value']
							);
		
		
		/*$val 											= $form_state['values'];
		
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
		$max_no_of_labours 								= $val['max_no_of_labours'];
		$from_date  									= date('Y-m-d',strtotime($val['from_date']));
		$to_date    									= date('Y-m-d',strtotime($val['to_date']));
		$total_months 									= $val['total_months'];
		*/
		db_delete('l_clra_con_nature_of_work')
						->condition('contractor_id', trim($contractor_id), '=')
						->execute();
		
		$updateConInfo = db_update('l_contractor_info_master');
        $updateConInfo->fields($fieldsConInfo);
        $updateConInfo->condition('id',$contractor_id);
		$updateConInfo->condition('application_id',$application_id);
		$updateConInfo->condition('user_id',$user->uid);
        $updateConInfo->execute();
		
		if(!empty($conNatureWork)){	
				  	foreach($conNatureWork as $LoopValueData){
					  	$fieldsAllContractorInfoNatureOfWork 				= 	array( 
																					'contractor_id'                 =>  $contractor_id,
																					'identification_number'			=>	$val['identification_number'],
																					'user_id'						=>	$user->uid,
																					'application_id'				=>  $application_id,
																					'act_id'						=>  1,
																					'contractor_nature_of_work'		=>	$LoopValueData
																					);	
																					
					  	db_insert('l_clra_con_nature_of_work')->fields($fieldsAllContractorInfoNatureOfWork)->execute();
					  	$increment++;
				  	}
		}
		
		if( $contractor_id != "" ){
			drupal_goto('view-clra-application-details/clra-contractor-info/'.encryption_decryption_fun('encrypt', $application_id));
		}
		
}

function my_ajax_callback($form, $form_state){
	$commands	=	array();
	$commands[] = ajax_command_replace('#address_div', drupal_render($form['contractor_edit']['address']));
	$commands[] = ajax_command_replace('#district_div', drupal_render($form['contractor_edit']['district']));
	$commands[] = ajax_command_replace('#subdivision_div', drupal_render($form['contractor_edit']['subdivision']));
	$commands[] = ajax_command_replace('#area_type_div', drupal_render($form['contractor_edit']['area_type']));
	$commands[] = ajax_command_replace('#area_code_div', drupal_render($form['contractor_edit']['area_code']));
	$commands[] = ajax_command_replace('#village_div', drupal_render($form['contractor_edit']['village']));
	$commands[] = ajax_command_replace('#police_station_div', drupal_render($form['contractor_edit']['police_station']));
	$commands[] = ajax_command_replace('#pin_code_div', drupal_render($form['contractor_edit']['pin_code']));
	return array('#type' => 'ajax', '#commands' => $commands);
}
	
