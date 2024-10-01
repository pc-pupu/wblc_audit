<?php 

function common_add_person($form, &$form_state, $application_id='' , $act_id = '', $personType = ''){  /** PARAMETERS => WHEN  ACT ID = 100 (SELF-CERTIFICATION) then Application_id used as USER ID  (PAYAL) **/
	
	global $base_root, $base_path, $user;
	
	$user_id 						= $user->uid;
	$decrypt_act_id 				= encryption_decryption_fun('decrypt', $act_id); 
	$decrypt_application_id			= encryption_decryption_fun('decrypt', $application_id); 
	$person_type					= encryption_decryption_fun('decrypt', $personType);
	
	
	$readonly_emp_pin				= array('maxlength' => 6);
	$readonly_emp_contact_number	= array('maxlength' => 10);
	$readonly_emp_guardian_name		= '';
	
	if($person_type == 'PRO'){
		$designation 		= 'Proprietor';
		$headerVariable 	= 'PROPRIETOR INFORMATION';
	}elseif($person_type == 'PE'){
		$designation 		= 'PrincipalEmployer';
		$headerVariable 	= 'PRINCIPAL EMPLOYER INFORMATION';
	}
	
	
	$intr_emp 			=  db_select('l_common_employee_info', 'liwm');			
	$intr_emp  			-> fields('liwm', array());
	$intr_emp 			-> condition('liwm.user_id', $user_id);
	$intr_emp 			-> condition('liwm.designation', $designation);
	$intr_emp 			-> condition('liwm.is_active', 1); 
	$intr_emp           -> condition('liwm.deleted', 'N');
	$intr_emp_result	= $intr_emp ->execute()->fetchAssoc(); 
	
	$common_emp_id 		= $intr_emp_result!='' ?  $intr_emp_result['id'] : '';
	
	
	if(!empty($decrypt_application_id) && $decrypt_application_id!='' && $decrypt_application_id!=0 && !empty($decrypt_act_id) ){ 
		
		$readonly_emp_name				= '';
		$readonly_emp_guardian_name		= '';
		$readonly_emp_address			= '';
		$readonly_emp_contact_number 	= array('maxlength' => 10);
		
		if($decrypt_act_id == 4){
			
			$ismw_reg 				= 	db_select('l_interstate_workman_master', 'liwm');			
			$ismw_reg				->	fields('liwm', array('status','final_submit_status','identification_number'));
			$ismw_reg				->  condition('liwm.id', $decrypt_application_id);	
			$ismw_reg				->  condition('liwm.act_id', $decrypt_act_id);	
			$ismw_reg				->	condition('liwm.user_id', $user_id);
			$ismw_reg_result 	 	= 	$ismw_reg->execute()->fetchAssoc();
			
			if(!empty($ismw_reg_result)){
				
				$status 				= $ismw_reg_result['status'];
				$final_submit_status	= $ismw_reg_result['final_submit_status'];
				$identification_number	= $ismw_reg_result['identification_number'];
				
				if($status == 'B'){
					
					$readonly_emp_name				= array('class' => array('correction_show'));
					$readonly_emp_guardian_name		= array('class' => array('correction_show'));
					$readonly_emp_address			= array('class' => array('correction_show'));
					$readonly_emp_pin				= array('class'=> array('correction_show'), 'maxlength'=> 6);
					$readonly_emp_contact_number	= array('class'=> array('correction_show'), 'maxlength' => 10);
					
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
			
				case "emp_name":
			
						$readonly_emp_name 					= array('class'=> array('verified_show') , 'disabled' => 'disabled');
						$readonly_emp_contact_number		= array('class'=> array('verified_show') , 'disabled' => 'disabled','maxlength' => 10);
						break;
						
				case "gender":
						$readonly_gender_pe 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						break;
					
				case "emp_guardian_name":
				
						$readonly_emp_guardian_name 		= array('class'=> array('verified_show'), 'disabled' => 'disabled');
						break;
						
				case "emp_address":
				
						$readonly_emp_address 				= array('class'=> array('verified_show'), 'disabled' => 'disabled');
						$readonly_emp_addressTextarea 		= array('class'=> array('verified_show'), 'readonly' => 'readonly');
						$readonly_emp_pin					= array('class'=> array('verified_show'), 'disabled' => 'disabled');
						break;	
			}
			
			}
		}
	}
	
	
	$form['common_pe_info']['header']			 			 	= array(
																		'#markup' 			=> t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">'.$headerVariable.'</div><div class="panel-body">'),
																		);
																		
	$form['common_pe_info']['common_emp_id'] 					= array(
																	  '#type' 				=> 'hidden',
																	  '#default_value'		=> encryption_decryption_fun('encrypt',$common_emp_id),
																	  '#attributes'			=> array('readonly'=>'readonly')
																	);
	$form['common_pe_info']['status'] 						= array(
																	  '#type' 				=> 'hidden',
																	  '#default_value'		=> encryption_decryption_fun('encrypt',$status),
																	  '#attributes'			=> array('readonly'=>'readonly')
																	);																
	$form['common_pe_info']['person_type'] 						= array(
																	  '#type' 				=> 'hidden',
																	  '#default_value'		=> $personType,
																	  '#attributes'			=> array('readonly'=>'readonly')
																	);
																		
	$form['common_pe_info']['application_id'] 					= array(
																	  '#type' 				=> 'hidden',
																	  '#default_value'		=> $application_id,
																	  '#attributes'			=> array('readonly'=>'readonly')
																	);
						
	$form['common_pe_info']['act_id'] 							= array(
																	  '#type' 				=> 'hidden',
																	  '#default_value'		=> $act_id,
																	  '#attributes' 		=> array('readonly'=>'readonly')
																	);
																	
	$form['common_pe_info']['final_submit_status'] 				= array(
																	  '#type' 				=> 'hidden',
																	  '#default_value'		=> $final_submit_status,
																	  '#attributes' 		=> array('readonly'=>'readonly')
																	);
																	
	$form['common_pe_info']['identification_number'] 				= array(
																	  '#type' 				=> 'hidden',
																	  '#default_value'		=> $identification_number,
																	  '#attributes' 		=> array('readonly'=>'readonly')
																	);
	
	
	$form['common_pe_info']['principal_emp_name'] 				= array(
																	  '#prefix' 			=> '<div class="col-md-4"><label class="input">',
																	  '#suffix' 			=> '</label></div>',
																	  '#title'				=> 'Full Name of the '.$designation,
																	  '#type' 				=> 'textfield',
																	  '#default_value' 		=> !empty($intr_emp_result['name']) ? $intr_emp_result['name'] : '',
																	  '#required' 			=> TRUE,
																	  '#element_validate' 	=> array('MISC_TEXTBOX_TEXT_VALIDATE'),
																	  '#attributes' 		=> $readonly_emp_name, 
																	);
	$form['common_pe_info']['gender'] 							= array(
																	   '#prefix'				=> '<div class="col-md-4"><div class="inline-group">',
																	   '#suffix'				=>'</div></div>',
																	   '#type' 					=> 'radios',
																	   '#title' 				=> t('5.(b) Gender'),
																	   '#options' 				=> array('M'=>t('Male'), 'F'=>t('Female'), 'O'=>t('Transgender')),
																	   '#default_value' 		=> isset($intr_emp_result['gender']) ? $intr_emp_result['gender'] : '',
																	   '#attributes' 			=> $readonly_gender_pe,
																	   '#required' 				=> TRUE
																		);
	
																	
	$form['common_pe_info']['principal_emp_guardian_name'] 		= array(
																	'#prefix' 			=> '<div class="col-md-4"><label class="input">',
																	'#suffix' 			=> '</section></div>',
																	'#type' 			=> 'textfield',
																	'#title' 			=> t('Furnish Father&#39;s / Husband&#39;s name in case of Individual'),      																
																	'#default_value' 	=> !empty($intr_emp_result['guardian_name']) ? $intr_emp_result['guardian_name']: '',
																	'#attributes' 		=> $readonly_emp_guardian_name,
																	'#element_validate' => array('MISC_TEXTBOX_TEXT_VALIDATE'),
																 );	
																 
	$form['common_pe_info']['email']	 						= array(
																	'#prefix' 	 		=> '<div class="col-md-4"><label class="input">',
																	'#suffix' 	 		=> '</label></div>',
																	'#title' 	 		=> 'Email',
																	'#type' 	 		=> 'textfield',
																	'#attributes' 		=> $readonly_emp_name,
																	'#required' 		=>  TRUE,
																	'#default_value'	=> $intr_emp_result['email'],
																	'#element_validate' => array('MISC_EMAIL_VALIDATE'),
																	);
																	
	$form['common_pe_info']['contact_number']	 				= array(
																	'#prefix' 	 		=> '<div class="col-md-4"><label class="input">',
																	'#suffix' 	 		=> '</label></div>',
																	'#title' 	 		=> 'Contact Number',
																	'#type' 	 		=> 'textfield',
																	'#attributes' 		=> $readonly_emp_contact_number,
																	'#required' 		=>  TRUE,
																	'#default_value'	=> $intr_emp_result['contact_number'],
																	'#element_validate' => array('MISC_PHONE_VALIDATE'),
																	
																	);

																
	$options_country 	= array(''=>'-Select-', '1' => 'India', '2' => 'Others'); 
																										
	$form['common_pe_info']['emp_country'] 						= array(
																		'#prefix' 			=> '<div class="col-md-4"><label class="select">',
																		'#suffix' 			=> '</label></div>',
																		'#title'			=> 'Select Country',
																		'#type' 			=> 'select',
																		'#options' 			=> $options_country,
																		'#default_value'	=> !empty($intr_emp_result['country_code']) ? $intr_emp_result['country_code'] : '',
																		'#required' 		=> TRUE,
																		'#attributes' 		=> $readonly_emp_address,
																		'#ajax'				=> array(
																										'event' 		=> 'change',
																										'effect' 		=> 'fade',			
																										'callback'		=> 'get_ajax_edit_country_emp',
																										'progress'		=>	'',
																								)
																);
																
	$value_emp_country 											= isset($form_state['values']['emp_country']) ? $form_state['values']['emp_country'] :  $intr_emp_result['country_code'];
		
	$form['common_pe_info']['address_principal_emp']			= array(
																		'#prefix' 			=> '<div id="add_line_edit_replace">',
																		'#suffix' 			=> '</div>',
																		);
																						
	$form['common_pe_info']['emp_state']						= array(
																		'#prefix' 			=> '<div id="clra_edit_emp_state_replace">',
																		'#suffix' 			=> '</div>',
																		);
																						
	$form['common_pe_info']['emp_dist']							= array(
																		'#prefix' 			=> '<div id="clra_edit_emp_district_replace">',
																		'#suffix' 			=> '</div>',
																		);	
																						
	$form['common_pe_info']['loc_emp_subdv'] 					= array(
																	 '#prefix' 				=> '<div id="clra_empsubdv_replace">',
																	 '#suffix' 				=> '</div>'
																	);
																					
	$form['common_pe_info']['loc_emp_areatype'] 				= array(
																	 '#prefix' 				=> '<div id="clra_emp_areatype_replace">',
																	 '#suffix' 				=> '</div>'
																	);	
																					
	$form['common_pe_info']['loc_emp_areatype_name'] 			= array(
																	 '#prefix' 				=> '<div id="clra_empareatype_name_replace">',
																	 '#suffix' 				=> '</div>'
																	);	
																					
	$form['common_pe_info']['loc_emp_vill_ward'] 				= array(
																	 '#prefix' 				=> '<div id="clra_emp_vill_ward_replace">',
																	 '#suffix' 				=> '</div>'
																	);
																					
	$form['common_pe_info']['loc_emp_ps'] 						= array(
																		'#prefix' 			=> '<div id="clra_empps_replace">',
																		'#suffix' 			=> '</div>',
																		);																				
																						
	$form['common_pe_info']['loc_emp_pin_number'] 				= array(
																		'#prefix' 			=> '<div id="clra_emp_pin_replace">',
																		'#suffix' 			=> '</div>',
																		);
																		
	if( (!empty($value_emp_country) && $value_emp_country != '') && $value_emp_country ==1) {
			
			
		$form['common_pe_info']['address_principal_emp'] 		= array(
																	  '#prefix' 			=> '<div id="add_line_edit_replace" class="col-md-8 min-height-80"><label class="input">',
																	  '#suffix' 			=> '</label></div>',
																	  '#title'				=> 'House No./Village Name/Street/Road',
																	  '#type' 				=> 'textfield',
																	  '#default_value' 		=>!empty($intr_emp_result['address_line']) ? $intr_emp_result['address_line'] : '',
																	  '#attributes' 		=> $readonly_emp_address,
																	  '#element_validate'	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE'),
																	  '#required' 			=> TRUE,
																	);
																					
		$form['common_pe_info']['emp_state']					= array(
																		'#prefix' 			=> '<div id="clra_edit_emp_state_replace" class="col-md-4"><label class="select">',
																		'#suffix' 			=> '</label></div>',
																		'#title'			=> t('Select State'),
																		'#type' 			=> 'select',
																		'#options' 			=> get_all_state($value_emp_country),
																		'#default_value'	=> !empty($intr_emp_result['state_code']) ? $intr_emp_result['state_code'] : '',
																		'#required' 		=> TRUE,
																		'#attributes' 		=> $readonly_emp_address,
																		'#ajax'				=> array(
																									'event'		=> 'change',
																									'effect' 	=> 'fade',
																									'callback'	=> 'ajax_reload_edit_form',
																									'progress'	=>	'',
																									)
																		);	
																							
		$value_emp_state	= isset($form_state['values']['emp_state'])? $form_state['values']['emp_state'] :  $intr_emp_result['state_code'];
			
		if($value_emp_state != $loc_emp_state ){
					
			$loc_emp_dist 			= '';
			$loc_emp_subdivision 	= '';
			$loc_emp_areatype		= '';
			$emp_name_areatype		= '';
			$loc_emp_vill_ward		= '';
			$l_emp_ps				= '';
			$loc_emp_pin_number		= '';
			
		}
			
		if(($value_emp_country != '' && (!empty($value_emp_state) || $value_emp_state != '' )  )){
				
				
			$db_district_value									= db_query("SELECT COUNT(id) FROM {district_master} WHERE state_id = '".$value_emp_state."'")->fetchObject();
				
				
			if(!empty($db_district_value) && ($db_district_value->count>0) ) {
				
				$form['common_pe_info']['emp_dist'] 			= array(
																	  '#prefix' 			=> '<div id="clra_edit_emp_district_replace" class="col-md-4"><label class="select">',
																	  '#suffix' 			=> '</label></div>',
																	  '#title'				=> 'Select District',
																	  '#type' 				=> 'select',
																	  '#options' 			=> get_district_by_id($value_emp_state),
																	  '#attributes' 		=> $readonly_emp_address,
																	  '#default_value' 		=> !empty($intr_emp_result['district_code']) ? $intr_emp_result['district_code'] : '',
																	  '#required' 			=> TRUE,
																	  '#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'ajax_reload_edit_form',
																								'progress'			=> '',
																							  )
																	);	
																					
				$value_emp_dist 						= isset($form_state['values']['emp_dist']) ? $form_state['values']['emp_dist'] :  $intr_emp_result['district_code'];
				
				if($value_emp_dist != $loc_emp_dist ){
					
					$loc_emp_subdivision 	= '';
					$loc_emp_areatype		= '';
					$emp_name_areatype		= '';
					$loc_emp_vill_ward		= '';
					$l_emp_ps				= '';
					$loc_emp_pin_number		= '';
				}
			}
		}
				
		if($value_emp_dist != ''  && !empty($value_emp_state)){
				
			$db_subdivision_value					= db_query("SELECT COUNT(id) as subdv_total FROM {sub_division} WHERE district_code = '".$value_emp_dist."'")->fetchObject();
			
			if(!empty($db_subdivision_value) && $db_subdivision_value != '' && $db_subdivision_value->subdv_total > 0) {
				
				
				$form['common_pe_info']['loc_emp_subdv'] 		= array(
																	  '#prefix' 		=> '<div id="clra_empsubdv_replace" class="col-md-4"><label class="select">',
																	  '#suffix' 		=> '</label></div>',
																	  '#title'			=> 'Select Subdivision',
																	  '#type' 			=> 'select',
																	  '#options' 		=> common_custom_user_subdivisionlist_list_new($value_emp_dist),
																	  '#attributes' 	=> $readonly_emp_address,
																	  '#default_value' => !empty($intr_emp_result['subdivision_code']) ? $intr_emp_result['subdivision_code'] : '',
																	  '#required' 		=> TRUE,
																	  '#ajax'			=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'ajax_reload_edit_form',
																								'progress'			=> '',
																							  )
																	);
																				
				$value_loc_emp_subdv 				= isset($form_state['values']['loc_emp_subdv']) ? $form_state['values']['loc_emp_subdv'] :  $intr_emp_result['subdivision_code'];
				
				if($value_loc_emp_subdv != $loc_emp_subdivision ){
						
					$loc_emp_areatype		= '';
					$emp_name_areatype		= '';
					$loc_emp_vill_ward		= '';
					$l_emp_ps				= '';
					$loc_emp_pin_number		= '';
				
				}
			}
		}
				
				
		if($value_emp_dist != "" && !empty($value_emp_state) && $value_loc_emp_subdv != ""){
					
			$db_block_mun_value						= db_query("SELECT COUNT(id) as areatype_total FROM {block_mun_master} WHERE distict_code = '".$value_emp_dist."' and sub_division_code  = '".$value_loc_emp_subdv."'")->fetchObject();
					
			if(!empty($db_block_mun_value) &&  $db_block_mun_value != '' && $db_block_mun_value->areatype_total > 0){
				
				$areatypeOptions					= common_custom_user_areatype_list_new($value_emp_dist, $value_loc_emp_subdv);
				
				
				if(!empty($areatypeOptions)){
					
					$form['common_pe_info']['loc_emp_areatype'] = array(
																	  '#prefix' 		=> '<div id="clra_emp_areatype_replace" class="col-md-4"><label class="select">',
																	  '#suffix' 		=> '</label></div>',
																	  '#title'			=> 'Select Area type',
																	  '#type' 			=> 'select',
																	  '#attributes' 	=> $readonly_emp_address,
																	  '#options' 		=> $areatypeOptions,
																	  '#default_value'	=> !empty( $intr_emp_result['area_type']) ?  $intr_emp_result['area_type'] : '',
																	  '#required' 		=> TRUE,
																	  '#ajax'			=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'ajax_reload_edit_form',
																								'progress'			=> '',
																							  )										  
																	);
																				
					$value_loc_emp_areatype 		= isset($form_state['values']['loc_emp_areatype']) ? $form_state['values']['loc_emp_areatype'] :  $intr_emp_result['area_type'];	
					
					if($value_loc_emp_areatype != $loc_emp_areatype){
					
						$emp_name_areatype		= '';
						$loc_emp_vill_ward		= '';
						$l_emp_ps				= '';
						$loc_emp_pin_number		= '';
					
					}
				}
			}
		}
			
			
		if ($value_emp_dist != "" && $value_loc_emp_subdv != "" && $value_loc_emp_areatype != "") {
			
			if ( $value_loc_emp_areatype == 'B' ){
				$areatype_name	= 'Block';
			}elseif( $value_loc_emp_areatype == 'M'){
				$areatype_name	= 'Municipality';
			}elseif( $value_loc_emp_areatype == 'C'){
				$areatype_name	= 'Corporation';
			}elseif( $value_loc_emp_areatype == 'S'){
				$areatype_name	= 'SEZ';
			}elseif($value_loc_emp_areatype == 'N'){
				$areatype_name	= 'Notified Area';
			}else{
				$areatype_name	= 'Block';
			}
			
			
			$form['common_pe_info']['loc_emp_areatype_name'] 	= array(
																	  '#prefix' 		=> '<div id="clra_empareatype_name_replace" class="col-md-4"><label class="select">',
																	  '#suffix' 		=> '</label></div>',
																	  '#title'			=> 'Select '.$areatype_name,
																	  '#type' 			=> 'select',
																	  '#options' 		=> common_custom_user_block_list_new($value_loc_emp_subdv,$value_loc_emp_areatype),
																	  '#default_value' 	=> !empty( $intr_emp_result['area_type_code']) ?  $intr_emp_result['area_type_code'] : '',
																	  '#attributes' 	=> $readonly_emp_address,
																	  '#required' 		=> TRUE,
																	  '#ajax'			=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'ajax_reload_edit_form',
																								'progress'			=> '',
																							  )
																);
																					
		$value_loc_emp_areatype_code = isset($form_state['values']['loc_emp_areatype_name']) ? $form_state['values']['loc_emp_areatype_name'] :  $intr_emp_result['area_type_code'];
			
			if($value_loc_emp_areatype_code != $emp_name_areatype){
						
				$loc_emp_vill_ward		= '';
				$l_emp_ps				= '';
				$loc_emp_pin_number		= '';
			
			}
		}
			
		if($value_emp_dist != "" && $value_loc_emp_subdv != "" && $value_loc_emp_areatype != ""  && $value_loc_emp_areatype_code != "" && !empty($value_emp_state) ){
			
			if ( $value_loc_emp_areatype == 'B' ){
				$area_name		= 'Gram Panchayat';
				$area_code 		= 'V';
			}elseif( $value_loc_emp_areatype == 'M'){
				$area_name		= 'Ward';
				$area_code 		= 'W';
			}elseif( $value_loc_emp_areatype == 'C'){
				$area_name		= 'Ward';
				$area_code 		= 'C';
			}elseif( $value_loc_emp_areatype == 'S'){
				$area_name		= 'Sector';
				$area_code 		= 'S';
			}elseif($value_loc_emp_areatype == 'N'){
				$area_name		= 'Notified Area';
				$area_code 		= 'N';
			}	
			
			$form['common_pe_info']['loc_emp_vill_ward'] 		= array(
																	  '#prefix' 			=> '<div id="clra_emp_vill_ward_replace" class="col-md-4"><label class="select">',
																	  '#suffix' 			=> '</label></div>',
																	  '#title'				=> 'Select '.$area_name,
																	  '#type' 				=> 'select',
																	  '#options'			=> common_custom_user_villward_list_new($value_loc_emp_areatype_code,$area_code),
																	  '#required' 			=> TRUE,
																	  '#default_value' 		=> !empty( $intr_emp_result['village_code']) ?  $intr_emp_result['village_code'] : '',
																	  '#attributes' 		=> $readonly_emp_address,
																	  '#ajax'				=> array(
																									'event' 			=> 'change',
																									'effect' 			=> 'fade',			
																									'callback'			=> 'ajax_reload_edit_form',
																									'progress'			=> '',
																								  )
																	);
																					
			$value_loc_emp_vill_ward 					= isset($form_state['values']['loc_emp_vill_ward']) ? $form_state['values']['loc_emp_vill_ward'] :  $intr_emp_result['village_code'];	
					
			if($value_loc_emp_vill_ward != $loc_emp_vill_ward){
				
				$l_emp_ps				= '';
				$loc_emp_pin_number		= '';
				
			}
			
		}
			
		if (!empty($value_emp_state) && $value_emp_dist != "" && $value_loc_emp_subdv != "" && $value_loc_emp_areatype && $value_loc_emp_areatype_code != "" && $value_loc_emp_vill_ward != "" ){
			
			
			$form['common_pe_info']['loc_emp_ps'] 				= array(
																	  '#prefix' 			=> '<div id="clra_empps_replace" class="col-md-4"><label class="select">',
																	  '#suffix' 			=> '</label></div>',
																	  '#title'				=>'Select Police Station',
																	  '#type' 				=> 'select',
																	  '#options' 			=> common_custom_user_ps_list_new($value_emp_dist),
																	  '#default_value' 		=> !empty( $intr_emp_result['police_station']) ?  $intr_emp_result['police_station']: '',
																	  '#attributes' 		=> $readonly_emp_address,
																	  '#required' 			=> TRUE,
																	); 	
																					
			$form['common_pe_info']['loc_emp_pin_number'] 		= array(
																	  '#prefix' 			=> '<div id="clra_emp_pin_replace" class="col-md-4"><label class="input">',
																	  '#suffix' 			=> '</label></div>',
																	  '#title'				=> 'PIN Number',
																	  '#type' 				=> 'textfield',
																	  '#default_value' 	 	=> !empty( $intr_emp_result['pin']) ?  $intr_emp_result['pin']	:'',
																	  '#attributes' 		=> $readonly_emp_pin,
																	  '#element_validate'	=> array('MISC_PIN_NUMBER'),
																	  '#required' 			=> TRUE
																	  );
			
		}
			
		}else{
			
			$form['common_pe_info']['address_principal_emp'] 	= array(
																	  '#prefix' 			=> '<div id="add_line_edit_replace" class="col-md-12"><label class="textarea">',
																	  '#suffix' 			=> '</label></div>',
																	  '#title'				=> 'Address [ If other Country/State please provide detail address ]',
																	  '#rows'				=> t('2'),
																	  '#type' 				=> 'textarea',
																	  '#attributes' 		=> $readonly_emp_addressTextarea,
																	  '#default_value' 		=> !empty( $intr_emp_result['address_line']) ?  $intr_emp_result['address_line'] : '',
																	  '#element_validate'	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE'),
																	  '#required' 			=> TRUE,
																	);
																						
			$form['common_pe_info']['emp_state']				= array(
																	 '#prefix' 				=> '<div id="clra_edit_emp_state_replace">',
																	 '#suffix' 				=> '</div>',
																		);
																							
			$form['common_pe_info']['emp_dist']					= array(
																	 '#prefix' 				=> '<div id="clra_edit_emp_district_replace">',
																	 '#suffix' 				=> '</div>',
																		);
																							
			$form['common_pe_info']['loc_emp_subdv'] 			= array(
																	 '#prefix' 				=> '<div id="clra_empsubdv_replace">',
																	 '#suffix' 				=> '</div>'
																	);	
																						
			$form['common_pe_info']['loc_emp_areatype'] 		= array(
																	 '#prefix' 				=> '<div id="clra_emp_areatype_replace">',
																	 '#suffix' 				=> '</div>'
																	);
																						
			$form['common_pe_info']['loc_emp_areatype_name'] 	= array(
																	 '#prefix' 				=> '<div id="clra_empareatype_name_replace">',
																	 '#suffix' 				=> '</div>'
																	);	
																						
			$form['common_pe_info']['loc_emp_vill_ward'] 		= array(
																	 '#prefix' 				=> '<div id="clra_emp_vill_ward_replace">',
																	 '#suffix' 				=> '</div>'
																	);
																						
			$form['common_pe_info']['loc_emp_ps'] 				= array(
																		'#prefix' 			=> '<div id="clra_empps_replace">',
																		'#suffix' 			=> '</div>',
																		);
																							
			$form['common_pe_info']['loc_emp_pin_number'] 		= array(
																		'#prefix' 			=> '<div id="clra_emp_pin_replace">',
																		'#suffix' 			=> '</div>',
																		);
																		
			
		}
		
		$form['common_pe_info']['submit'] 						= array(
																	  '#prefix' 			=> '</div></div>',  
																	  '#type' 				=> 'submit',
																	  '#attributes' 		=> array('class' => array('btn btn-primary pull-right form-submit')),
																	  '#value' 				=> 'SAVE'
																	   );
																	   
		if($decrypt_act_id == 100){
			$back_link = l('<< BACK TO SELF CERTIFICATION', 'self-certification-application/particulars', array('attributes' => array('class' => 'link-btn')));	
		}
		
		$form['common_pe_info']['back_link'] 					= array(
																'#type' 			=> 'markup',
																'#suffix' 			=> '</div>', 
																'#markup'	    	=> $back_link
																);
																
	return $form;
}

function common_add_person_submit($form, $form_state){
	
	global $base_root, $base_path, $user;
	
	$user_id								= $user->uid;
	$val 									= $form_state['values'];
	$final_submit_status				    = $val['final_submit_status'];
	$identification_number					= $val['identification_number'];
	
	$status				    				= encryption_decryption_fun('decrypt',$val['status']);
	$common_emp_id				    		= encryption_decryption_fun('decrypt',$val['common_emp_id']);
	$person_type							= encryption_decryption_fun('decrypt',$val['person_type']);
	$application_id							= encryption_decryption_fun('decrypt', $val['application_id']);
	$act_id 								= encryption_decryption_fun('decrypt', $val['act_id']);
	
	$name 									= $val['principal_emp_name'];
	$gender									= $val['gender']; 
	$guardian_name							= $val['principal_emp_guardian_name'];
	$contact_number							= $val['contact_number'];
	$email									= $val['email'];
	$address_line 							= $val['address_principal_emp'];
	$country_code							= $val['emp_country'];
	$state_code								= $val['emp_state'];
	$district_code 							= $val['emp_dist'];
	$subdivision_code 						= $val['loc_emp_subdv'];
	$area_type 								= $val['loc_emp_areatype'];
	$area_type_code							= $val['loc_emp_areatype_name'];
	$village_code							= $val['loc_emp_vill_ward'];
	$police_station							= $val['loc_emp_ps'];
	$pin									= $val['loc_emp_pin_number'];
	
	if($person_type == 'PRO'){
		$personAdded = 'Proprietor';
	}elseif($person_type == 'PE'){
		$personAdded = 'PrincipalEmployer';
	}
	
	$fieldsCommon 		= array( 
							'user_id'				=>	$user->uid,
							'designation'			=>  $personAdded,
							'name'					=>	$name,
							'contact_number'		=>  $contact_number,
							'email'					=>	$email,
							'guardian_name' 		=>	$guardian_name,
							'address_line'			=>	$address_line,
							'country_code'			=>	!empty($country_code)? $country_code : 0,
							'state_code'			=>  !empty($state_code) ? $state_code : 0,
							'district_code'			=>	$district_code,
							'subdivision_code'		=>	!empty($subdivision_code) ? $subdivision_code : 0 ,
							'area_type'				=>	!empty($area_type) ? $area_type : '',
							'area_type_code'		=>	!empty($area_type_code) ? $area_type_code : 0 ,
							'village_code'			=>	!empty($village_code) ? $village_code : 0,
							'police_station'		=>	$police_station,
							'pin'					=>  !empty($pin) ? $pin : 0,
							'gender'				=>  $gender,
							'is_active'				=>  1,
							'created_date'			=>  date('Y-m-d H:i:s'),
							'deleted'				=>  'N',
							);
												
	$common 			=  db_select('l_common_employee_info', 'liwm');			
	$common 			-> fields('liwm', array());
	$common				-> condition('liwm.user_id', $user_id);
	$common				-> condition('liwm.designation', $personAdded);
	$common				-> condition('liwm.is_active', 1); 
	$common				-> condition('liwm.deleted', 'N'); 
	$common_pe_result	= $common->execute()->fetchAssoc();
	
	
	if(!empty($common_pe_result)){
		
		db_update('l_common_employee_info')->fields(array(
													'name'										=>	$name,
													'contact_number'							=>  $contact_number,
													'email'										=>	$email,
													'guardian_name' 							=>	$guardian_name,
													'address_line'								=>	$address_line,
													'country_code'								=>	!empty($country_code)? $country_code : 0,
													'state_code'								=>  !empty($state_code) ? $state_code : 0,
													'district_code'								=>	$district_code,
													'subdivision_code'							=>	!empty($subdivision_code) ? $subdivision_code : 0 ,
													'area_type'									=>	!empty($area_type) ? $area_type : '',
													'area_type_code'							=>	!empty($area_type_code) ? $area_type_code : 0 ,
													'village_code'								=>	!empty($village_code) ? $village_code : 0,
													'police_station'							=>	$police_station,
													'pin'										=>  !empty($pin) ? $pin : 0,
													'gender'									=>  $gender,
													'modified_date'								=>  date('Y-m-d H:i:s'),
													)
												)->condition('designation', $personAdded, '=')
												->condition('user_id', $user_id, '=')
												->condition('id', $common_emp_id, '=')
												->execute();
											
		if($act_id == 4) {
		
			$ismw_reg 			=  db_select('l_interstate_employer_master', 'liem');			
			$ismw_reg 			-> fields('liem', array());
			$ismw_reg			-> condition('liem.user_id', $user_id);
			$ismw_reg			-> condition('liem.application_id', $application_id);
			$ismw_reg			-> condition('liem.designation', $personAdded);
			$ismw_reg			-> condition('liem.is_active', 1); 
			$ismw_reg_result	= $ismw_reg->execute()->fetchAssoc();	
			
			if(!empty($ismw_reg_result)&& $status !='I'){ 
			
				db_update('l_interstate_employer_master')->fields(array(
																	'name'										=>	$name,
																	'contact_number'							=>  $contact_number,
																	'email'										=>	$email,
																	'guardian_name' 							=>	$guardian_name,
																	'address_line'								=>	$address_line,
																	'country_code'								=>	!empty($country_code)? $country_code : 0,
																	'state_code'								=>  !empty($state_code) ? $state_code : 0,
																	'district_code'								=>	$district_code,
																	'subdivision_code'							=>	!empty($subdivision_code) ? $subdivision_code : 0 ,
																	'area_type'									=>	!empty($area_type) ? $area_type : '',
																	'area_type_code'							=>	!empty($area_type_code) ? $area_type_code : 0 ,
																	'village_code'								=>	!empty($village_code) ? $village_code : 0,
																	'police_station'							=>	$police_station,
																	'pin'										=>  !empty($pin) ? $pin : 0,
																	'gender'									=>  $gender,
																	)
													)->condition('designation', $personAdded, '=')
													->condition('user_id', $user_id, '=')
													->condition('identification_number', $identification_number, '=')
													->condition('application_id', $application_id)
													->condition('common_emp_id', $common_emp_id, '=')
													->execute();
													
			}else{
				
				$fieldsToInsert = array( 
									'identification_number'	=>  $identification_number,
									'application_id'		=>  $application_id,
									'user_id'				=>	$user->uid,
									'act_id'				=>  $act_id,								
									'designation'			=>  $personAdded,
									'name'					=>	$name,
									'contact_number'		=>  $contact_number,
									'email'					=>	$email,
									'guardian_name' 		=>	$guardian_name,
									'address_line'			=>	$address_line,
									'country_code'			=>	!empty($country_code)? $country_code : 0,
									'state_code'			=>  !empty($state_code) ? $state_code : 0,
									'district_code'			=>	$district_code,
									'subdivision_code'		=>	!empty($subdivision_code) ? $subdivision_code : 0 ,
									'area_type'				=>	!empty($area_type) ? $area_type : '',
									'area_type_code'		=>	!empty($area_type_code) ? $area_type_code : 0 ,
									'village_code'			=>	!empty($village_code) ? $village_code : 0,
									'police_station'		=>	$police_station,
									'pin'					=>  !empty($pin) ? $pin : 0,
									'is_active'				=>  1,
									'common_emp_id'			=>  $common_pe_result['id'],
									'gender'				=>  $gender,
									);
									
				$inserted_pe_id 	= db_insert('l_interstate_employer_master')->fields($fieldsToInsert)->execute();
				
				db_update('l_interstate_workman_master')->fields(array('principal_employer_id'=>$inserted_pe_id))->condition('user_id', $user_id)->condition('id', $application_id)->condition('act_id', $act_id)->execute(); 
			}
			
			drupal_goto('ismw_application/registration-form/'.encryption_decryption_fun('encrypt', $application_id));
			
		}elseif($act_id == 100){
			drupal_goto('self-certification-application/particulars');
		}
		
	}else{
		
		$inserted_common_pe_id = db_insert('l_common_employee_info')->fields($fieldsCommon)->execute();
		
		if($act_id == 4) { /** FOR ISMW REG**/
		
			$fieldsToInsert1 = array( 
								'identification_number'	=>  $identification_number,
								'application_id'		=>  $application_id,
								'user_id'				=>	$user->uid,
								'act_id'				=>  $act_id,								
								'designation'			=>  $personAdded,
								'name'					=>	$name,
								'contact_number'		=>  $contact_number,
								'email'					=>	$email,
								'guardian_name' 		=>	$guardian_name,
								'address_line'			=>	$address_line,
								'country_code'			=>	!empty($country_code)? $country_code : 0,
								'state_code'			=>  !empty($state_code) ? $state_code : 0,
								'district_code'			=>	$district_code,
								'subdivision_code'		=>	!empty($subdivision_code) ? $subdivision_code : 0 ,
								'area_type'				=>	!empty($area_type) ? $area_type : '',
								'area_type_code'		=>	!empty($area_type_code) ? $area_type_code : 0 ,
								'village_code'			=>	!empty($village_code) ? $village_code : 0,
								'police_station'		=>	$police_station,
								'pin'					=>  !empty($pin) ? $pin : 0,
								'is_active'				=>  1,
								'gender'				=>  $gender,
								'common_emp_id'			=>  $inserted_common_pe_id
						);
			
			if($status!='I' && $application_id!=''){
				
				$inserted_pe_id 	= db_insert('l_interstate_employer_master')->fields($fieldsToInsert1)->execute();
				
				db_update('l_interstate_workman_master')->fields(array('principal_employer_id'=>$inserted_pe_id))->condition('user_id', $user_id)->condition('id', $application_id)->condition('act_id', $act_id)->execute(); 
			
				drupal_goto('ismw_application/registration-form/'.encryption_decryption_fun('encrypt', $application_id));
			}
		
		}elseif($act_id == 100){ /** FOR SELF CERTIFICATION **/
			
			drupal_goto('self-certification-application/particulars');
		}
	}
}

function get_ajax_edit_country_emp($form, $form_state){
	$commands = array();
	$commands[] = ajax_command_replace('#add_line_edit_replace', drupal_render($form['common_pe_info']['address_principal_emp']));
	$commands[] = ajax_command_replace('#clra_edit_emp_state_replace', drupal_render($form['common_pe_info']['emp_state']));
	$commands[] = ajax_command_replace('#clra_edit_emp_district_replace', drupal_render($form['common_pe_info']['emp_dist']));
	$commands[] = ajax_command_replace('#clra_empsubdv_replace', drupal_render($form['common_pe_info']['loc_emp_subdv']));
	$commands[] = ajax_command_replace('#clra_emp_areatype_replace', drupal_render($form['common_pe_info']['loc_emp_areatype']));
	$commands[] = ajax_command_replace('#clra_empareatype_name_replace', drupal_render($form['common_pe_info']['loc_emp_areatype_name']));
	$commands[] = ajax_command_replace('#clra_emp_vill_ward_replace', drupal_render($form['common_pe_info']['loc_emp_vill_ward']));
	$commands[] = ajax_command_replace('#clra_empps_replace', drupal_render($form['common_pe_info']['loc_emp_ps']));
	$commands[] = ajax_command_replace('#clra_emp_pin_replace', drupal_render($form['common_pe_info']['loc_emp_pin_number']));
	return array('#type' => 'ajax', '#commands' => $commands);
}

function ajax_reload_edit_form($form, $form_state) {
	$commands = array();
	$commands[] = ajax_command_replace('#clra_edit_emp_state_replace', drupal_render($form['common_pe_info']['emp_state']));
	$commands[] = ajax_command_replace('#clra_edit_emp_district_replace', drupal_render($form['common_pe_info']['emp_dist']));
	$commands[] = ajax_command_replace('#clra_empsubdv_replace', drupal_render($form['common_pe_info']['loc_emp_subdv']));
	$commands[] = ajax_command_replace('#clra_emp_areatype_replace', drupal_render($form['common_pe_info']['loc_emp_areatype']));
	$commands[] = ajax_command_replace('#clra_empareatype_name_replace', drupal_render($form['common_pe_info']['loc_emp_areatype_name']));
	$commands[] = ajax_command_replace('#clra_emp_vill_ward_replace', drupal_render($form['common_pe_info']['loc_emp_vill_ward']));
	$commands[] = ajax_command_replace('#clra_empps_replace', drupal_render($form['common_pe_info']['loc_emp_ps']));
	$commands[] = ajax_command_replace('#clra_emp_pin_replace', drupal_render($form['common_pe_info']['loc_emp_pin_number']));
    return array('#type' => 'ajax', '#commands' => $commands);
}


	
	