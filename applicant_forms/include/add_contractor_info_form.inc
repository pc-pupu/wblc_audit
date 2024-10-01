<?php
drupal_add_js(drupal_get_path('module', 'applicant_forms') . '/js/upload_custom.js');
function add_contractor_info_form($form, &$form_state, $id=''){
	
	global $base_root,$base_path,$user;
	
	drupal_add_library('system','ui.datepicker');
	drupal_add_js(drupal_get_path('module', 'applicant_forms') . '/js/custom.js');
	
	
	$user_id 				= $user->uid;
	$amendment_parent_id	= 0;
	$application_id			= encryption_decryption_fun('decrypt', $id);
		
	$clraMasterData 		= db_query("select lcrm.id, lcrm.final_submit_status, lcrm.identification_number, lcrm.amendment_parent_id , lcrm.e_any_day_max_num_of_workmen, lcrm.status from l_clra_registration_master lcrm where lcrm.user_id=:user_id and lcrm.id=:id", array(':user_id' => $user_id, ':id' =>$application_id));
			
	$clraMaster 			= $clraMasterData->fetchAssoc();
	$application_id 		= $clraMaster['id'];
	$identification_number 	= $clraMaster['identification_number'];
	$final_submit_status 	= $clraMaster['final_submit_status'];
	$status					= $clraMaster['status'];
	$finalNumber			= $clraMaster['e_any_day_max_num_of_workmen'];
	$amendment_parent_id	= $clraMaster['amendment_parent_id'];
			
	$fetchInsertedTillNow			= db_query("select sum(lcim.contractor_max_no_of_labours_on_any_day) from l_contractor_info_master lcim where lcim.user_id=:user_id and lcim.application_id=:applicationid and lcim.status=:status", array(':user_id' => $user_id, ':applicationid' =>$application_id , ':status' => 1));
	if( $fetchInsertedTillNow->rowCount() > 0 ){
		$fetchInsertedTillNow_res	= $fetchInsertedTillNow->fetchAssoc();
	}	
		
	$form['contractor_master']['header']					= array(
																'#markup' 			=> t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">Particulars of Contractors and Contract Labour</div><div class="panel-body">'),
																						);
	$form['contractor_master']['application_id'] 			= array(
																  '#type' 			=> 'hidden',
																  '#default_value'	=> !empty($application_id) ? $id : '',
																  '#attributes' 	=> array('readonly'=>'readonly')
																);
							
	$form['contractor_master']['identification_number']     = array(
																  '#type' 			=> 'hidden',
																  '#default_value'	=> !empty($identification_number) ? $identification_number : '',
																  '#attributes' 	=> array('readonly'=>'readonly')
																);
																				
	$form['contractor_master']['amendment_parent_id'] 		= array(
																  '#type' 			=> 'hidden',
																  '#default_value'	=> !empty($amendment_parent_id) ? encryption_decryption_fun('encrypt', $amendment_parent_id) : '',
																  '#attributes' 	=> array('readonly'=>'readonly')
																);
	
	$form['contractor_master']['contractor_type'] 			= array(														  
																 '#type' 			=> 'hidden',
																 '#default_value'	=> 2														  
																);
																
	/*$form['contractor_master']['contractor_type']	 		=	array(
																	'#title'	 	=>	'1. Contractor has/is',
																	'#prefix'	 	=>	'<fieldset><section><label class="select">',
																	'#suffix'	 	=>	'</label></section>',
																	'#type'		 	=>	'select',
																	'#options'	 	=>	array( 1 => 'Offline generated License', 2 => 'New Contractor'),
																	'#required'	 	=>	TRUE,
																	'#default_value'=>  2,
																	'#attributes'	 =>	array('id' => 'contractor_type_id')
																);*/
																
	$form['contractor_master']['name_of_contractor'] 		= array(
																  '#prefix' 		=> '<div class="col-md-4"><label class="input">',
																  '#suffix' 		=> '</label></div>',
																  '#title'			=> '2. Name of the Contractor',
																  '#type' 			=> 'textfield',
																  '#required' 		=> TRUE,
																  '#element_validate'=> array('MISC_TEXTBOX_TEXT_ENAME_VALIDATE'),
																  //'#autocomplete_path' 	=> 'applicant-profile/autocomplete',
																);
	
	$form['contractor_master']['email_of_contractor'] 		= array(
																  '#prefix' 		=> '<div class="col-md-4"><label class="input">',
																  '#suffix' 		=> '</label></div>',
																  '#title'  		=> '3. Email of the Contractor',
																  '#type'     		=> 'textfield',
																  '#required' 		=> TRUE,
																  '#element_validate'=> array('MISC_EMAIL_VALIDATE')
																);	
																
	$form['contractor_master']['state_opts'] 				= array(
																'#prefix' 			=> '<div class="col-md-4"><label class="select">',
																'#suffix' 			=> '</label></div>',
																'#title'			=> '3. Select State',
																'#type' 			=> 'select',
																'#options' 			=> get_all_state(1),
																'#required' 		=> TRUE,
																'#ajax'				=> array(
																							'event' 		=> 'change',
																							'effect' 		=> 'fade',			
																							'callback'		=> 'my_ajax_callback',
																							'progress'		=>	'',
																						)
																	);

	$value_state_opts 									= isset($form_state['values']['state_opts']) ? $form_state['values']['state_opts'] : '';

	
	$form['contractor_master']['address']				= array(
															'#prefix' 			=> '<div id="address_div">',
															'#suffix' 			=> '</div>',
															);
	
	$form['contractor_master']['district']				= array(
															'#prefix' 			=> '<div id="district_div">',
															'#suffix' 			=> '</div>',
															);	
																	
	$form['contractor_master']['subdivision']			= array(
															'#prefix' 			=> '<div id="subdivision_div">',
															'#suffix' 			=> '</div>',
															);	
																
	$form['contractor_master']['area_type']				= array(
															'#prefix' 			=> '<div id="area_type_div">',
															'#suffix' 			=> '</div>',
															);
																
	$form['contractor_master']['area_code']				= array(
															'#prefix' 			=> '<div id="area_code_div">',
															'#suffix' 			=> '</div>',
															);
																
	$form['contractor_master']['village']				= array(
															'#prefix' 			=> '<div id="village_div">',
															'#suffix' 			=> '</div>',
															);
																
	$form['contractor_master']['police_station']		= array(
															'#prefix' 			=> '<div id="police_station_div">',
															'#suffix' 			=> '</div>',
															);	
																
	$form['contractor_master']['pin_code']				= array(
															'#prefix' 			=> '<div id="pin_code_div">',
															'#suffix' 			=> '</div>',
															);
															
	if( (!empty($value_state_opts) && $value_state_opts != '') && $value_state_opts ==  1) {
		
		$form['contractor_master']['address'] 			 = array(
															'#prefix' 			=> '<div id="address_div" class="col-md-8 min-height-80"><label class="input">',
															'#suffix' 			=> '</label></div>',
															'#title'			=> '3.(a) Address Line1',
															'#type' 			=> 'textfield',
															'#required' 		=> TRUE,
															'#element_validate'	=> array('MISC_TEXTBOX_LOCATION_VALIDATE'),
															);
															
		if(($value_state_opts != '0' && (!empty($value_state_opts) || $value_state_opts != '' ))){
				
			$db_district_value	= db_query("SELECT COUNT(id) FROM {district_master} WHERE state_id = '".$value_state_opts."'")->fetchObject();
				
			if(!empty($db_district_value) && ($db_district_value->count>0) ) {
				
				$form['contractor_master']['district']	 = array(
															 '#prefix' 			=> '<div id="district_div" class="col-md-4"><label class="select">',
															 '#suffix' 			=> '</label></div>',
															 '#title'			=> t('3.(b) Select District'),
															 '#type' 			=> 'select',
															 '#options' 		=> get_district_by_id($value_state_opts),
															 '#default_value'	=> !empty($loc_e_dist) ? $loc_e_dist : '',
															 '#required' 		=> TRUE,
															 '#ajax'			=> array(
																						'event'		=> 'change',
																						'effect' 	=> 'fade',
																						'callback'	=> 'my_ajax_callback',
																						'progress'	=>	'',
																						)
															);	
					
				$value_district = isset($form_state['values']['district']) ? $form_state['values']['district'] : '';
			}
		}
		
		if($value_district != ''  && $value_state_opts!=0){
					
			$db_subdivision_value	= db_query("SELECT COUNT(id) as subdv_total_manager  FROM {sub_division} WHERE district_code = '".$value_district."'")->fetchObject();
				
			if(!empty($db_subdivision_value) && $db_subdivision_value != '' && $db_subdivision_value->subdv_total_manager > 0) {
				
				$form['contractor_master']['subdivision']= array(
															  '#prefix' 		=> '<div id="subdivision_div" class="col-md-4"><label class="select">',
															  '#suffix' 		=> '</label></div>',
															  '#title'			=> '3.(c) Select Subdivision',
															  '#type' 			=> 'select',
															  '#options' 		=> common_custom_user_subdivisionlist_list_new($value_district),
															  '#required' 		=> TRUE,
															  '#ajax'			=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'my_ajax_callback',
																						'progress'			=> '',
																					  ),
														);
													
				$value_subdivision	= isset($form_state['values']['subdivision']) ? $form_state['values']['subdivision'] : 0;
			}
		}
		
		if($value_district != "" && !empty($value_state_opts) && $value_subdivision != ""){
				
			$db_block_mun_value	 = db_query("SELECT COUNT(id) as areatype_total FROM {block_mun_master} WHERE distict_code = '".$value_district."' and sub_division_code  = '".$value_subdivision."'")->fetchObject();
			
			if(!empty($db_block_mun_value) &&  $db_block_mun_value != '' && $db_block_mun_value->areatype_total > 0){
				
				$areatypeOptions	= common_custom_user_areatype_list_new($value_district, $value_subdivision);
					
				if(!empty($areatypeOptions)){
						
				 $form['contractor_master']['area_type'] = array(
															  '#prefix' 		=> '<div id="area_type_div" class="col-md-4"><label class="select">',
															  '#suffix' 		=> '</label></div>',
															  '#title'			=> '3.(d) Select Block / Municipality / Corporation / SEZ / Notified Area',
															  '#type' 			=> 'select',
															  '#options' 		=> $areatypeOptions,
															  '#required' 		=> TRUE,
															  '#ajax'			=> array(
																					'event' 			=> 'change',
																					'effect' 			=> 'fade',			
																					'callback'			=> 'my_ajax_callback',
																					'progress'			=> '',
																				  ),
															);
					$value_area_type 	= isset($form_state['values']['area_type']) ? $form_state['values']['area_type'] : '';
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
			
			$form['contractor_master']['area_code'] 	 = array(
															  '#prefix' 		=> '<div id="area_code_div" class="col-md-4"><label class="select">',
															  '#suffix' 		=> '</label></div>',
															  '#title'			=> '3.(e) Select '.$areatype_name,
															  '#type' 			=> 'select',
															  '#options' 		=> common_custom_user_block_list_new($value_subdivision, $value_area_type),
															  '#required' 		=> TRUE,
															  '#ajax'			=> array(
																					'event' 			=> 'change',
																					'effect' 			=> 'fade',			
																					'callback'			=> 'my_ajax_callback',
																					'progress'			=> '',
																				  )
												 );
														 
			$value_area_code 	= isset($form_state['values']['area_code']) ? $form_state['values']['area_code'] : '';
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
				
			$form['contractor_master']['village'] 		 = array(
															  '#prefix' 		=> '<div id="village_div" class="col-md-4"><label class="select">',
															  '#suffix' 		=> '</label></div>',
															  '#title'			=> '3.(f) Select '.$area_name,
															  '#type' 			=> 'select',
															  '#options'		=> common_custom_user_villward_list_new($value_area_code,$areatypecode),
															  '#required' 		=> TRUE,
															  '#ajax'			=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'my_ajax_callback',
																						'progress'			=> '',
																					  )
															);		
			$value_village 	= isset($form_state['values']['village']) ? $form_state['values']['village'] : '0';	
		}
		
		if ($value_state_opts != '0' && $value_district != "" && $value_subdivision != "" && $value_area_type != "" && $value_area_code != ""  && $value_village != "" ){
				
			$form['contractor_master']['police_station'] = array(
															  '#prefix' 		=> '<div id="police_station_div" class="col-md-4"><label class="select">',
															  '#suffix' 		=> '</label></div>',
															  '#title'			=> '3.(g) Select Police Station',
															  '#type' 			=> 'select',
															  '#options' 		=> common_custom_user_ps_list_new($value_district),
															  '#required' 		=> TRUE,
														  );
																								  
																								  
			$form['contractor_master']['pin_code'] 		= array(
															  '#prefix' 		=> '<div id="pin_code_div" class="col-md-4"><label class="input">',
															  '#suffix' 		=> '</label></div>',
															  '#title'			=> '3.(h)Pin Code',
															  '#type' 			=> 'textfield',
															  '#required' 		=> TRUE,
															  '#attributes' 	=>  array('maxlength' => 6),
															  '#element_validate'=> array('MISC_PIN_NUMBER')
														  );	
		}
		
	}else{
		
		$form['contractor_master']['address'] 			= array(
															'#prefix' 			=> '<div class="col-md-12" id="address_div"><label class="textarea">',
															'#suffix' 			=> '</label></div>',
															'#title'			=> '3.(a) Address Line1 [ If other State please provide detail address ]',
															'#rows'				=> t('2'),
															'#cols'				=> t('146'),	
															'#type' 			=> 'textarea',
															'#required' 		=> TRUE,
															'#element_validate'	=> array('MISC_TEXTBOX_LOCATION_VALIDATE'),
															);
		
	}
	
	$form['contractor_master']['con_nature_of_work'] 	 = array( 
															  '#prefix' 		=> '<div class="col-md-8"><label class="select">',
															  '#suffix' 		=> '</label></div>',
															  '#title'			=> '4.Nature of Work in which Contract Labour is Employed or is to be Employed',
															  '#type' 			=> 'select',
															  '#options' 		=> contractor_works_multiSelect_options(),
															  '#required' 		=> TRUE,
															  '#multiple' 		=> TRUE,
															  '#id' 			=> 'e_nature_of_work_id',
															  '#attributes' 	=> array('style'=>"height: 80px;", 'class'=> array('selected disabled'),'onchange' => 'nature_wrk_others(this.value)'),
														);
														
	$form['contractor_master']['other_nature_of_work_value'] = array(
															 '#prefix' 			=> '<div class="col-md-12" id ="edit_other_nature_work_id"><label class="input">',
															 '#suffix' 			=> '</label></div>',
															 '#title'			=> 'Other Option for Nature of Work',
															 '#type' 			=> 'textfield',
															 '#id' 				=> 'other_value_nature_of_work_id',
															 '#element_validate' => array('MISC_OTHERNATURE_TEXTBOX_TEXT_VALIDATE')
															);
															
	$deductedvalue = ($finalNumber-$fetchInsertedTillNow_res['sum']);
	
	if( $deductedvalue == 0 ){
		$readonly_contractor_max_no_of_labours_on_any_day = array('disabled' => 'disabled', 'class' => array('readonly_view'));
		$form['contractor_master']['max_no_of_labours']   = array(
															  '#prefix' 		=> '<div class="col-md-12"><label class="input">',
															  '#suffix' 		=> '</label></div>',
															  '#title'			=> '5. Maximum Number of Contractor Labour to be Employed on any day Through Each Contractor to be engaged',
															  '#type' 			=> 'textfield',
															  '#required'		=> TRUE,
															  '#element_validate' => array('MISC_TEXTBOX_NUMERIC_VALIDATE')
															  /*'#description'		=> t('<font color="red">Note: Maximum Number of Contractor Labour to be Employed on any day Through Each Contractor should not exceed '.$finalNumber.'.<br> Please increase the number by clicking the first tab and then scroll down to 5.a) Maximum Number of Workmen Employed Directly on any day in the Establishment.</font><br>'.'Total Number = <font color="red">'.$finalNumber.'</font><br> Inserted Till Now = <font color="red">'.$fetchInsertedTillNow_res['sum'].'</font><br>Left Numbers = <font color="red">'.($finalNumber-$fetchInsertedTillNow_res['sum']).'</font>'),
															  '#attributes' 		=> $readonly_contractor_max_no_of_labours_on_any_day,*/
															);	
	}else{
		$form['contractor_master']['max_no_of_labours']   = array(
															  '#prefix' 		=> '<div class="col-md-12"><label class="input">',
															  '#suffix' 		=> '</label></div>',
															  '#title'			=> '5. Maximum Number of Contractor Labour to be Employed on any day Through Each Contractor to be engaged',
															  '#type' 			=> 'textfield',
															  '#required'		=> TRUE,
															  '#element_validate'=> array('MISC_TEXTBOX_NUMERIC_VALIDATE')
															  /*'#description'		=> t('<font color="red">Note:</font> Maximum Number of Contractor Labour to be Employed on any day Through Each Contractor should not exceed '.$finalNumber.'.<br> Please increase the number by clicking the first tab and then scroll down to 5.a) Maximum Number of Workmen Employed Directly on any day in the Establishment.<br>'.'Total Number = <font color="red">'.$finalNumber.'</font><br> Inserted Till Now = <font color="red">'.$fetchInsertedTillNow_res['sum'].'</font><br>Left Numbers = <font color="red">'.($finalNumber-$fetchInsertedTillNow_res['sum']).'</font>'),*/
															);
	}
	
	$form['contractor_master']['est_date_of_work'] 		   = array(
																  '#prefix' 	=> '<div class="col-md-12"><label class="input">',
																  '#suffix' 	=> '</label>',
																  '#type' 		=> 'markup',
																  '#markup'		=> '6. Estimated Date of Employment of Each Contract Work Under Each Contractor <span style="color:#f00;">* </span>',
																);
	
	$form['contractor_master']['from_date'] 			   = array(
																'#prefix' 		=> '<div class="row"><div class="col-md-4"><label class="input">',
																'#suffix' 		=> '</label></div>',
																'#type' 		=> 'textfield',
																//'#required' 	=> TRUE,
																'#attributes'	=>	array('id' => 'est_date_of_commencement_id', /*'class'=> array('est_date_of_work_of_each_labour_from_date_class'),*/ 'readonly'=>'readonly','placeholder' => t('From'), 'onclick' => 'return datepicker_func();'),
																  
																);
							
	$form['contractor_master']['to_date'] 					= array(
																'#prefix' 		=> '<div class="col-md-4"><label class="input">',
																'#suffix' 		=> '</label></div>',
																'#type' 		=> 'textfield',
																//'#required' 	=> TRUE,
																'#attributes'	=>	array('id' => 'est_date_of_termination_id', /*'class'=> array('est_date_of_work_of_each_labour_to_date_class'),*/ 'readonly'=>'readonly', 'placeholder'=> t('To'),'onclick' => 'return datepicker_func();'),
																);
																
	$form['contractor_master']['total_months']				= array(
																'#prefix' 		=> '<div class="col-md-4"><label class="input">',
																'#suffix' 		=> '</label></div></div></div></div></div>',
																'#type' 		=> 'textfield',
																//'#required' 	=> TRUE,
																'#attributes'	=>	array('id'=> 'est_date_of_work_of_each_labour_total_months_edit_id', 'readonly'=>'readonly','placeholder' => t('Total Days')),
																);
	if( $status =='B' || $final_submit_status =='' ){
													
		$form['contractor_master']['submit'] 				= array(
																'#type' 		=> 'submit',
																'#attributes' 	=> array('class' => array('btn btn-primary pull-right')),
																'#value' 		=> 'SAVE'
															   );
	}else{
		
		$form['contractor_master']['row_div'] 				= array (
																'#prefix' 		=> '<div class="row">&nbsp;&nbsp;</div><div class="row">&nbsp;&nbsp;</div>',
																);		
	}
	
	$form['contractor_master']['go_to_dashboard'] 			= array(
																'#type' 		=> 'markup',
																'#markup'	    => l('GO TO DASHBOARD', 'applicant-dashboard', array('attributes' => array('class' => 'link-btn')))
																);
	
	$form['contractor_master']['back_link'] 				= array(
																'#type' 		=> 'markup',
																'#suffix' 		=> '</div>',
																'#markup'	    => l('BACK TO CONTRACTOR LIST', 'view-clra-application-details/clra-contractor-info/'.$id, array('attributes' => array('class' => 'link-btn mleft-15')))
																);
														
			
	return $form;
}
	
	/*function add_contractor_info_form_validate($form, &$form_state){
		
		global $base_root, $base_path, $user;
		
		$user_id 									= $user->uid;
		$contractor_max_no_of_labours_on_any_day 	= $form_state['values']['contractor_max_no_of_labours_on_any_day'];
		$application_id 							= $form_state['values']['application_id'];
		
		/////////////////////////////  Contractor Number Checking //////////////////////////////////////////
			
		$fetchInsertedTillNow			= db_query("select sum(lcim.contractor_max_no_of_labours_on_any_day) from l_contractor_info_master lcim where lcim.user_id=:user_id and lcim.application_id=:applicationid and lcim.status=:status", array(':user_id' => $user_id, ':applicationid' =>$application_id, ':status' =>1));
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
		
		if( $addedValue > $FirstValue ){
			form_error($form['contractor_master']['contractor_max_no_of_labours_on_any_day'], t('Number SHOULD not BE greater'));
			$form_state['rebuild'] = TRUE;
		}
		/////////////////////////////// End Of Checking //////////////////////////////////////////////////
		
	}*/
	
	function add_contractor_info_form_submit($form, &$form_state){
		global $base_root, $base_path, $user;
		 
		$increment 			= 0;
		$val 				= $form_state['values'];
		$conNatureWork		= $val['con_nature_of_work'];
		$amendment_parent_id = encryption_decryption_fun('decrypt',$val['amendment_parent_id']); 
		
		
		$fieldsConInfo 		= array( 
								'application_id'								=>  encryption_decryption_fun('decrypt', $val['application_id']),
								'user_id'										=>	$user->uid,
								'identification_number'							=>	$val['identification_number'],
								
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
								//'state_others'									=>	$state_others,
								
								'contractor_max_no_of_labours_on_any_day'		=>	$val['max_no_of_labours'],
								'est_date_of_work_of_each_labour_from_date'		=>	date('Y-m-d',strtotime($val['from_date'])), 
								'est_date_of_work_of_each_labour_to_date'		=>  date('Y-m-d',strtotime($val['to_date'])),
								'est_date_of_work_of_each_labour_total_months'	=>  $val['total_months'],
								'act_id'										=>	1, 
								'status'										=>	1,
								'amendment_id'									=> ($amendment_parent_id!='') ? $amendment_parent_id : 0,
								'other_nature_work'								=> $val['other_nature_of_work_value']
								);
														
		$last_contractor_id= db_insert('l_contractor_info_master')->fields($fieldsConInfo)->execute();
		
		
		if(!empty($conNatureWork)){
			
			foreach($conNatureWork as $LoopValueData){
				
				$fieldsAllContractorInfoNatureOfWork 				= 	array( 
																			'contractor_id'                 =>  $last_contractor_id,
																			'identification_number'			=>	$val['identification_number'],
																			'user_id'						=>	$user->uid,
																			'act_id'						=>	1,  
																			'application_id'				=>  encryption_decryption_fun('decrypt', $val['application_id']), 
																			'contractor_nature_of_work'		=>	$LoopValueData
																		);
																			
				db_insert('l_clra_con_nature_of_work')->fields($fieldsAllContractorInfoNatureOfWork)->execute();
				
				$increment++;
				
			}
		}
		
		if($amendment_parent_id!='' && $amendment_parent_id!='0'){
			drupal_goto('clra-reg-amendment/clra-contractor-info/'.$val['application_id']);
		}else{
			if( $last_contractor_id != "" ){
				drupal_goto('view-clra-application-details/clra-contractor-info/'.$val['application_id']);
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
	
	function my_ajax_callback($form, $form_state){
		$commands	=	array();
		$commands[] = ajax_command_replace('#address_div', drupal_render($form['contractor_master']['address']));
		$commands[] = ajax_command_replace('#district_div', drupal_render($form['contractor_master']['district']));
		$commands[] = ajax_command_replace('#subdivision_div', drupal_render($form['contractor_master']['subdivision']));
		$commands[] = ajax_command_replace('#area_type_div', drupal_render($form['contractor_master']['area_type']));
		$commands[] = ajax_command_replace('#area_code_div', drupal_render($form['contractor_master']['area_code']));
		$commands[] = ajax_command_replace('#village_div', drupal_render($form['contractor_master']['village']));
		$commands[] = ajax_command_replace('#police_station_div', drupal_render($form['contractor_master']['police_station']));
		$commands[] = ajax_command_replace('#pin_code_div', drupal_render($form['contractor_master']['pin_code']));
		return array('#type' => 'ajax', '#commands' => $commands);
	}