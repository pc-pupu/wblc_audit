<?php

//drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
	
	function applicant_profile_form($form, &$form_state, $refer_url = '') {
		
		global $base_root, $base_path, $user;
		
		if(in_array('TUAPPLICANT', $user->roles)){
			drupal_goto('tu-profile');
		}
		
		$user_id 		 	= $user->uid;
		
		drupal_add_js(drupal_get_path('module', 'applicant_forms') . '/js/applicant_prof.js');
		drupal_add_library('system','ui.datepicker');
		drupal_add_js('jQuery(document).ready(function(){jQuery( \'#applicant_dobId\' ).datepicker({
     			 				dateFormat  : "yy-mm-dd",
   								changeMonth : true,
     			 				changeYear  : true,
     			 				autoSize    : true,
								yearRange	: "-100:+0",
								maxDate		: new Date()
   			                });});', 'inline');
		
		$id						=	'';
		$f_name					=	'';
		$m_name					=	'';
		$l_name					=	'';
		$dob					=	'';
		$email					=	'';
		$mobile					=	'';
		$gender					=	'';
		
		
		$address				=	'';
		$country				=	'';
		$state					=	'';
		$district_code			=	'';
		$subdivision_code		=	'';
		$areatype_code			=	'';
		$name_areatype_code		=	'';
		$vill_ward_code			=	'';
		$ps_code				=	'';
		$pin					=	'';
		
		$borough_code			=	'';
		
		$ps_code				=	'';
		$pin					=	'';
		
		
		$card_type				=	'';
		$card_number			=	'';
		
		$est_name				= 	'';
		$est_location			= 	'';
		$est_district			=	'';
		$est_subdivision		=	'';
		$est_areatype			=	'';
		$est_areatype_code		=	'';
		$est_loc_e_vill_ward	=	'';
		$est_ps					=	'';
		$est_pin_number			= '';
		
		/*******   ********/
		
		$val_ownership				=	'';
		$val_other_ownership		=	'';
		
		$val_ownership_undertaking	=	'';
		$val_other_government_name	=	'';
		$val_local_bodies			=	'';
		$val_urban_local_bodies		=	'';
		
		
		$val_other_private_name		=	'';
		$master_role_adolescent_male 	= 	'';
		$master_role_adolescent_female	= 	'';
		
		$contractual_labour_adolescent_male	= '';
		$contractual_labour_adolescent_female	= '';
		
		$other_labour_adolescent_male	=   '';
		$other_labour_adolescent_female	=   '';
		
		$fetch_common_det 	= db_query("select * from l_common_application_master where user_id=:userId",array(':userId'=>$user_id));
		
		if( $fetch_common_det->rowCount() > 0 ){
			
			$obj 	= 	$fetch_common_det->fetchAssoc();
			
				$id						=	$obj['id'];
				$f_name					=	$obj['f_name'];
				$m_name					=	$obj['m_name'];
				$l_name					=	$obj['l_name'];
				$dob					=	$obj['dob'];
				$email					=	$obj['email'];
				$mobile					=	$obj['mobile'];
				$gender					=	$obj['gender'];
				
				$address				=	$obj['address'];
				$country				=	$obj['country'];
				$state					=	$obj['state'];
				$district_code			=	$obj['dist_code'];
				$subdivision_code		=	$obj['subdivision'];
				$areatype_code			=	$obj['areatype'];
				$name_areatype_code		=	$obj['name_areatype'];
				$vill_ward_code			=	$obj['vill_ward'];
				$ps_code				=	$obj['ps'];
				$pin					=	$obj['pin'];
				
				$card_type				=	$obj['card_type'];
				$card_number			=	$obj['card_number'];
				
				$est_name				= 	$obj['est_name'];
				$type_ofthe_est 		=   $obj['est_type'];
				$est_location			= 	$obj['est_loc'];
				$est_district			=	$obj['est_district'];
				$est_subdivision		=	$obj['est_subdivision'];
				$est_areatype			=	$obj['est_loc_e_areatype'];
				$est_areatype_code		=	$obj['est_name_areatype'];
				$est_loc_e_vill_ward	=	$obj['est_loc_e_vill_ward'];
				$est_ps					=	$obj['est_ps'];
				$est_pin_number			= 	$obj['est_pin_number'];
				
				
				/******* Ownership  ********/
				
				$val_ownership				=	$obj['ownership'];
				$val_other_ownership		=	$obj['other_ownership'];
				
				$val_ownership_undertaking	=	$obj['ownership_undertaking'];
				$val_other_government_name	=	$obj['other_government_name'];
				$val_other_private_name		=	$obj['other_private_name'];
				$val_local_bodies			=	$obj['local_bodies'];
				$val_urban_local_bodies		=	$obj['urban_local_bodies'];
				
				/*******  worker info ********/
				$work_man_master_role_male		=	$obj['work_man_master_role_male'];
				$work_man_master_role_female	=	$obj['work_man_master_role_female'];
				$master_role_adolescent_male 	= 	$obj['work_man_master_role_adolescent_male'];
				$master_role_adolescent_female	= 	$obj['work_man_master_role_adolescent_female'];
				
				$contractual_labour_male		=	$obj['contractual_labour_male'];
				$contractual_labour_female		=	$obj['contractual_labour_female'];
				$contractual_labour_adolescent_male	= $obj['contractual_labour_adolescent_male'];
				$contractual_labour_adolescent_female	= $obj['contractual_labour_adolescent_female'];
				
				$child_labour_male				=	$obj['child_labour_male'];
				$child_labour_female			=	$obj['child_labour_female'];
				$other_labour_adolescent_male	=   $obj['child_labour_adolescent_male'];
				$other_labour_adolescent_female	=   $obj['child_labour_adolescent_female'];
				
				/******* PE / Con Info */
				
				
				$pe_con_name 					= $obj['pe_con_name'];
				$pe_con_gender 					= $obj['pe_con_gender'];
				$pe_con_dob 					= $obj['pe_con_dob'];
				$pe_con_mobile 					= $obj['pe_con_mobile'];
				$pe_con_address 				= $obj['pe_con_address'];
				$pe_con_country 				= $obj['pe_con_country'];
				$pe_con_state 					= $obj['pe_con_state'];
				$pe_con_district 				= $obj['pe_con_district'];
				$pe_con_subdivision 			= $obj['pe_con_subdivision'];
				$pe_con_areatype 				= $obj['pe_con_areatype'];
				$pe_con_block_code 				= $obj['pe_con_block_code'];
				$pe_con_gp 						= $obj['pe_con_gp'];
				$pe_con_ps 						= $obj['pe_con_ps'];
				$pe_con_pin 					= $obj['pe_con_pin'];
				
				
				
				
				//$_SESSION['tabStatus']	=	isset($obj['status']) ? $obj['status'] : 0;
		
		//print_r($obj);die();
		}
		
		if(empty($id)){
			$submit_val = 'SAVE';
		}else{
			$submit_val='UPDATE';
		}
		
		$form['dashboard_master']['refer_url']	= array(						 
		  '#type' 				=> 'hidden',
		  '#default_value' 		=>!empty($refer_url) ? $refer_url : '',					  
		);							
								
		$form['dashboard_master']['id'] = array(
		  '#prefix' 			=>'<div class="content">
		  							<div class="panel panel-default panel-form">
										<div class="panel-heading">Basic Information</div>
										<div class="panel-body">',
		  '#title'				=>'Id',
		  '#type' 				=> 'hidden',
		  '#default_value' 		=>!empty($id) ? $id : '',
		  '#attributes' 		=> array('readonly'=>'readonly'),
		  '#disabled' 			=> TRUE
		);
								
		$form['dashboard_master']['f_name'] = array(
		  '#prefix' 			=>  '<div class="col-md-4"><label class="input">',
		  '#suffix' 			=>  '</label></div>',
		  '#title'				=>  'First Name',
		  '#type' 				=>  'textfield',
		  '#default_value' 		=>  !empty($f_name) ? $f_name : '',
		  '#attributes' 		=>  array('placeholder' => 'Enter your First Name'),
		  '#required' 			=>  TRUE,
		  '#element_validate'	=>  array('DASHBOARD_TEXTBOX_TEXT_VALIDATE')
		);
								
		$form['dashboard_master']['m_name'] = array(
		  '#prefix'				=> '<div class="col-md-4"><label class="input">',
		  '#suffix'				=> '</label></div>',
		  '#title'				=> 'Middle Name',
		  '#type' 				=> 'textfield',
		  '#default_value' 		=>!empty($m_name) ? $m_name : '',
		  '#attributes' 		=> array('placeholder' => 'Enter your Middle Name'),
		  '#element_validate'	=> array('DASHBOARD_TEXTBOX_OPTIONALTEXT_VALIDATE')
		);
								
		$form['dashboard_master']['l_name'] = array(
			  '#prefix'				=> '<div class="col-md-4"><label class="input">',
			  '#suffix'				=> '</label></div>',
			  '#title'				=> 'Last Name',
			  '#type' 				=> 'textfield',
			  '#default_value' 		=>!empty($l_name) ? $l_name : '',
			  '#attributes' 		=> array('placeholder' => 'Enter your Last Name'),
			  '#required' 			=> TRUE,
			  '#element_validate'	=> array('DASHBOARD_TEXTBOX_TEXT_VALIDATE')
		);	
																
		$form['dashboard_master']['email'] = array(
		  '#prefix'				=> '<div class="col-md-4"><label class="input">',
		  '#suffix'				=> '</label></div>',
		  '#title'				=> 'Email',
		  '#type' 				=> 'textfield',
		  '#default_value' 		=>!empty($email) ? $email : '',
		  '#attributes' 		=> array('placeholder' => 'Enter your Email'),
		  '#required' 			=> TRUE,
		  '#element_validate'	=> array('DASHBOARD_TEXTBOX_EMAIL_VALIDATE')
		);
								
		$form['dashboard_master']['mobile'] = array(
		  '#prefix'				=> '<div class="col-md-4"><label class="input">',
		  '#suffix'				=> '</label></div>',
		  '#title'				=> 'Mobile',
		  '#type' 				=> 'textfield',
		  '#default_value' 		=>!empty($mobile) ? $mobile : '',
		  '#attributes' 		=> array('placeholder' => 'Enter your 10 digit Mobile Number', 'maxlength' => 10),
		  '#required' 			=> TRUE,
		  '#element_validate' 	=> array('DASHBOARD_TEXTBOX_CONTACT_NUMBER_VALIDATE')
		);
								
								
		$genderOptions 			=	array('M'=>t('Male'), 'F'=>t('Female'), 'O'=>t('Other'));
		
		$form['dashboard_master']['gender'] = array(
		  '#prefix'				=> '<div class="col-md-4"><div class="inline-group">',
		  '#suffix'				=>'</div></div>',
		  '#type' 				=> 'radios',
		  '#title' 				=> t('Gender'),
		  '#options' 			=> $genderOptions,
		  '#default_value' 		=> isset($gender) ? $gender : '',
		  '#required' 			=> TRUE
		);
		
		if(empty($dob)){
			$form['dashboard_master']['dob'] = array(
				'#prefix'			=> '<div class="col-md-4"><label class="date">',
				'#suffix'			=> '</label></div>',
				'#type' 			=> 'date',
				'#title'			=> 'Date Of Birth',
				'#required' 		=> TRUE,
				'#after_build' 		=> array("applicant_forms_format_dob"),
				'#element_validate' => array('applicant_dob_validate'),
				'#attributes' 		=> array('class' => array('datefieldapp'))
			); 
		}else{
			$form['dashboard_master']['dob'] = array(
				'#prefix'			=> '<div class="col-md-4"><label class="date">',
				'#suffix'			=> '</label></div>',
				'#type' 			=> 'date',
				'#title'			=> 'Date Of Birth',
				'#required' 		=> TRUE,
				'#after_build' 		=> array("applicant_forms_format_dob"),
				'#element_validate' => array('applicant_dob_validate'),
				'#default_value' 	=> array(
											 'month' => format_date(strtotime($dob), 'custom',  'n'),
											 'day' => format_date(strtotime($dob), 'custom',  'j'),
											 'year' => format_date(strtotime($dob), 'custom',  'Y'),
											),
				'#attributes' 		=> array('class' => array('datefieldapp'))
			);
		}															
		
		
		
		$cardTypeOptions 			= 	array(0=>'Select Card Type', 1=>'AADHAR', 2=>'PAN', 3=>'TAN', 4=>'LIN', 5=>'EPIC');
		$value_dropdown_cardType	=	!empty($card_type) ? $card_type : key($cardTypeOptions);
		
		$form['dashboard_master']['card_type'] = array(
		  '#prefix'				=> '<div class="col-md-4"><label class="select">',
		  '#suffix'				=> '</label></div>',
		  '#title'				=>'ID Card Type',
		  '#type' 				=> 'select',
		  '#options' 			=> $cardTypeOptions,
		  '#default_value'		=> $value_dropdown_cardType,
		  '#required' 			=> TRUE,
		  '#element_validate'	=> array('DASHBOARD_CARD_TYPE_VALIDATE')
		);
		
		$form['dashboard_master']['card_number'] = array(
		  '#prefix'				=> '<div class="col-md-4"><label class="input">',
		  '#suffix'				=> '</label></div>',
		  '#title'				=>'ID Card Number',
		  '#type' 				=> 'textfield',
		  '#default_value' 		=>!empty($card_number) ? $card_number : '',
		  '#attributes' 		=> array('placeholder' => 'Enter your Id Card Number', 'id' => 'card_number_id', 'maxlength' => 20),
		  '#required' 			=> TRUE,
		  '#element_validate' 	=> array('DASHBOARD_SPECIAL_CHAR_TEXT_VALIDATE')
		);
		
		
		/*********************************start of establishment*************************************/
	
	
	$form['dashboard_master']['address'] = array(
		'#prefix' 			=>	'<div class="col-md-4"><label class="input">',
		'#suffix' 			=>	'</label></div>',
		'#title'			=>	t('Address Line1'),
		'#default_value'	=>	$address,
		'#type'				=>	'textfield',
		'#required' 		=> 	TRUE,
		'#attributes' 		=> 	$readonly_person_loc
	);
	
	$options_country 	= array(''=>'-Select-', 1 => 'India', 2 => 'Others'); 
	// country block start 	
	$form['dashboard_master']['country'] = array(
			'#prefix' 				=> '<div class="col-md-4"><label class="select">',
			'#suffix' 				=> '</label></div>',
			'#title'				=> 'Country',
			'#type' 				=> 'select',
			'#options' 				=> $options_country,
			'#default_value'		=> $country,
			'#attributes' 			=> $readonly_person_loc,
			'#required' 			=> TRUE,
			'#ajax'					=> array(
											'event' 			=> 'change',
											'effect' 			=> 'fade',			
											'callback'			=> 'get_info_form_ajax_callback',
											'progress'			=>	array('type' => 'throbber','message' => t('Loading State...')),
									)
	);
	// country block end
	
	// Default block start  									
	$value_country = isset($form_state['values']['country']) ? $form_state['values']['country'] : $country;
	
	$form['dashboard_master']['state'] = array(
			'#prefix' 			=> '<div id="country_others_div">',
			'#suffix' 			=> '</div>',
	);
	
	$value_state									= 0;
	$form['dashboard_master']['district'] = array(
			'#prefix' 			=> '<div id="distric_div">',
			'#suffix' 			=> '</div>',
	);
	$form['dashboard_master']['subdivision'] = array(
			'#prefix' 			=> '<div id="subdivisn_div">',
			'#suffix' 			=> '</div>',
	);
	$form['dashboard_master']['areatype'] = array(
			'#prefix' 			=> '<div id="areatype_div">',
			'#suffix' 			=> '</div>',
	);
	$form['dashboard_master']['block_code'] = array(
			'#prefix' 			=> '<div id="block_mun_div">',
			'#suffix' 			=> '</div>',
	);
	$form['dashboard_master']['panchyat'] = array(
			'#prefix' 			=> '<div id="gp_ward_div">',
			'#suffix' 			=> '</div>',
	);
	$form['dashboard_master']['ps'] = array(
			'#prefix' 			=> '<div id="ps_div">',
			'#suffix' 			=> '</div>',
	);
	$form['dashboard_master']['pin'] = array(
			'#prefix' 			=> '<div id="pincode_div">',
			'#suffix' 			=> '</div>',
	);
	
	
	
	// Default block end
 	if(!empty($value_country) || $value_country !=''){
		$db_state_value								= db_query("SELECT COUNT(id) FROM {state_master} WHERE country_id = ".$value_country."")->fetchObject();
	}else{
		$db_state_value								= array();
	}
	if(!empty($value_country) && !empty($db_state_value) && ($db_state_value->count>0)){
		// india section
			/***************************** State block start **********************/
		$options_state = get_all_state1($value_country);
		$form['dashboard_master']['state']								= array(
															'#prefix' 			=> '<div id="country_others_div"><div class="col-md-4"><label class="select">',
															'#suffix' 			=> '</label></div></div>',
															'#title'			=> t('Select State'),
															'#type' 			=> 'select',
															'#options' 			=> $options_state,
															'#default_value'	=> $state,
															'#required' 		=> TRUE,
															'#attributes' 		=> $readonly_person_loc,
															'#ajax'				=> array(
																						'event'		=> 'change',
																						'effect' 	=> 'fade',
																						'callback'	=> 'get_info_form_ajax_callback',
																					)
													);
			/***************************** District block start **********************/
		$value_state								= isset($form_state['values']['state'])?$form_state['values']['state'] : $state;
		$options_dist								= array(''=>'-Select-');																  
		
		if(($value_country!='' && (!empty($value_state) || $value_state!=0 || $value_state!=''))){
			$db_district_value							= db_query("SELECT COUNT(id) FROM {district_master} WHERE state_id =".$value_state." AND is_active='Y' ")->fetchObject();
			if(($db_district_value->count>0) && (!empty($value_state) || $value_state!=0)){
				
				$form['dashboard_master']['district']			= array(
																'#prefix' 			=> '<div id="distric_div"><div class="col-md-4"><label class="select">',
																'#suffix' 			=> '</label></div></div>',
																'#title'			=> t('Select District'),
																'#type' 			=> 'select',
																'#options' 			=> get_discrict_by_id($value_state),
																'#default_value'	=> $district_code,
																'#attributes' 		=> $readonly_person_district,
																'#required' 		=> TRUE,
																'#ajax'				=> array(
																							'event'		=> 'change',
																							'effect' 	=> 'fade',
																							'callback'	=> 'get_info_form_ajax_callback',
																						)
														);
														
				/***************************** Sub Division block start **********************/
				$value_dis								= isset($form_state['values']['district'])?$form_state['values']['district'] : $district_code;
				
				if(!empty($value_dis) || $value_dis!='' || $value_dis!=0){
					$db_subdivision_value					= db_query("SELECT COUNT(id) FROM {sub_division} WHERE district_code ='".$value_dis."' AND is_active='Y' ")->fetchObject();
				}else{
					$db_subdivision_value					= 0;
				}
				
				
				
				if($db_subdivision_value->count>0){				
					if(($value_state!='' || $value_state!=0) && ($value_dis!="" || !empty($value_dis) || $value_dis!=0)){
						$form['dashboard_master']['subdivision']	= array(
																	'#prefix' 			=> '<div id="subdivisn_div"><div class="col-md-4"><label class="select">',
																	'#suffix' 			=> '</label></div></div>',
																	'#title'			=> t('Select Sub-Division'),
																	'#type' 			=> 'select',
																	'#options' 			=> common_custom_user_subdivisionlist_list_new($value_dis),
																	'#default_value'	=> $subdivision_code,
																	'#attributes' 		=> $readonly_person_subdiv,
																	'#required' 		=> TRUE,
																	'#ajax'				=> array(
																								'event'		=> 'change',
																								'effect' 	=> 'fade',
																								'callback'	=> 'get_info_form_ajax_callback',
																							)
															);
					}else{
						$form['dashboard_master']['subdivision']	= array(
																	'#prefix' 			=> '<div id="subdivisn_div"><div class="col-md-4"><label class="select">',
																	'#suffix' 			=> '</label></div></div>',
																	'#title'			=> t('Select Sub-Division'),
																	'#type' 			=> 'select',
																	'#options' 			=> common_custom_user_subdivisionlist_list_new($value_dis),
																	'#default_value'	=> $subdivision_code,
																	'#attributes' 		=> $readonly_person_subdiv,
																	'#required' 		=> TRUE,
																	'#ajax'				=> array(
																								'event'		=> 'change',
																								'effect' 	=> 'fade',
																								'callback'	=> 'get_info_form_ajax_callback',
																							)
															);
					
					
					}
					/***************************** Area Type block start **********************/				
					
					$value_subdiv						= isset($form_state['values']['subdivision']) ? $form_state['values']['subdivision'] : $subdivision_code;
					if(($value_dis!='' || !empty($value_dis) || $value_dis!=0) && (!empty($value_subdiv) || $value_subdiv!='' || $value_subdiv!=0)){
						$db_areatype_value					= db_query("SELECT COUNT(id) FROM {block_mun_master} WHERE distict_code ='".$value_dis."' AND sub_division_code=".$value_subdiv." AND is_active='Y' ")->fetchObject(); 
					}else{
						$db_areatype_value					= 0;
					}
					
					
					if($db_areatype_value->count>0){
					
						$areaTypeData 							= array();
						if($value_dis != "" && $value_subdiv != ""){	
							$areatypeOptions 					= common_custom_user_areatype_list_new($value_dis, $value_subdiv);
							if(empty($areatypeOptions)){
								$areatypeOptions	= array();
							}
							
							$form['dashboard_master']['areatype']		= array(
																		'#prefix' 			=> '<div id="areatype_div"><div class="col-md-4"><label class="select">',
																		'#suffix' 			=> '</label></div></div>',
																		'#title'			=> 'Select Area Type',
																		'#type' 			=> 'select',
																		'#options' 			=> $areatypeOptions,
																		'#default_value'	=> $areatype_code,
																		'#attributes' 		=> $readonly_person_areatype,
																		'#required' 		=> TRUE,
																		'#attributes' 		=> array('autocomplete' => 'off', 'placeholder'=>'Select Area Type'),
																		'#ajax'				=> array(
																								'event' 	=> 'change',
																								'effect' 	=> 'fade',			
																								'callback' 	=> 'get_info_form_ajax_callback',																								
																							)	
																);
							}else {
							$form['dashboard_master']['areatype']		= array(
																		'#prefix' 			=> '<div id="areatype_div"><div class="col-md-4"><label class="select">',
																		'#suffix' 			=> '</label></div></div>',
																		'#title'			=> 'Select Area Type',
																		'#type' 			=> 'select',
																		'#options' 			=> $areaTypeData,
																		'#default_value'	=> $areatype_code,
																		'#attributes' 		=> $readonly_person_areatype,
																		'#required' 		=> TRUE,
																		'#attributes' 		=> array('autocomplete' => 'off', 'placeholder'=>'Select Area Type'),
																		'#ajax'				=> array(
																								'event' 	=> 'change',
																								'effect' 	=> 'fade',			
																								'callback' 	=> 'get_info_form_ajax_callback',
																								'progress'	=>	'',
																							)	
																);
						}
						
						
				/***************************** Block/Municiple block start **********************/
					$value_areatype 						= isset($form_state['values']['areatype']) ? $form_state['values']['areatype'] : $areatype_code;
		
					$blockData 								= array();
					$blockData[] 							= 'Select';	
					
					$form['dashboard_master']['block_code'] 			= array(
																'#title'		=> 'Select Block',
																'#type' 		=> 'select',
																'#options' 		=> $blockData,
																'#default_value'=> $name_areatype_code,
																'#attributes' 	=> $readonly_person_areatype_code,
																'#required' 	=> TRUE,
																'#attributes' 	=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Block/Municipality/Corporation/SEZ/Notified Area'),
																'#ajax'			=> array(
																						'event' 	=> 'change',
																						'effect' 	=> 'fade',			
																						'callback' 	=> 'get_info_form_ajax_callback',
																						'progress'	=>	array('type' => 'throbber','message' => 'Loading...'),
																					),	
																'#prefix' 		=> '<div id="block_mun_div"><div class="col-md-4"><label class="select">',
																'#suffix' 		=> '</label></div></div>'
														);
				
					
						if($value_dis != 0 && $value_subdiv != 0){
									if($value_areatype != ""){
										if ( $value_areatype == 'B' ){
											$areatype_name	= 'Block';
										}elseif( $value_areatype == 'M'){
											$areatype_name	= 'Municipality';
										}elseif( $value_areatype == 'C'){
											$areatype_name	= 'Corporation';
										}elseif( $value_areatype == 'S'){
											$areatype_name	= 'SEZ';
										}elseif($value_areatype == 'N'){
											$areatype_name	= 'Notified Area';
										}
										
										$form['dashboard_master']['block_code'] 		= array(
																					'#title'		=> 'Name of '.$areatype_name,
																					'#type' 		=> 'select',
																					'#options' 		=> common_custom_user_block_list_new($value_subdiv,$value_areatype),
																					'#default_value'=> $name_areatype_code,
																					'#required' 	=>  TRUE,
																					'#attributes' 	=> $readonly_person_areatype_code,
																					'#ajax'			=> array(
																											'event' 	=> 'change',
																											'effect' 	=> 'fade',			
																											'callback' 	=> 'get_info_form_ajax_callback',
																											'progress'	=>	array('type' => 'throbber','message' => 'Loading...'),
																										),	
																					'#prefix' 		=> '<div id="block_mun_div"><div class="col-md-4"><label class="select">',
																					'#suffix' 		=> '</label></div></div>'
																			);
									}
							}	
							
				$value_block_mun 					= isset($form_state['values']['block_code']) ? $form_state['values']['block_code'] : $name_areatype_code;
				if(!empty($value_block_mun) || $value_block_mun!='' || $value_block_mun!=0){
					$db_vil_ward						= db_query("Select COUNT(id) FROM {village_ward_master} WHERE block_code=".$value_block_mun." ")->fetchObject();
				}else{
					$db_vil_ward						= 0;
				}			
				
				if($db_vil_ward->count>0){
							
					/***************************** GP/Ward block start **********************/
					
					$panchyatData 						= array();
					$panchyatData[] 					= 'Select';	
					
					$form['dashboard_master']['panchyat'] 		= array(
																'#title' 		=> 'Select Gram Panchayat/Ward',
																'#type' 		=> 'select',
																'#options' 		=> $panchyatData,
																'#default_value'=> $vill_ward_code,
																'#required' 	=>  TRUE,
																//'#attributes' 	=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Gram Panchayat/Ward'),
																'#attributes' 	=> $readonly_person_vill_ward,
																'#ajax'			=> array(
																						'event' 	=> 'change',
																						'effect' 	=> 'fade',			
																						'callback' 	=> 'get_info_form_ajax_callback',
																						'progress'	=>	array('type' => 'throbber','message' => 'Loading...'),
																					  ),	
																'#prefix' 		=> '<div id="gp_ward_div"><div class="col-md-4"><label class="select">',
																'#suffix' 		=> '</label></div></div>'
														);
					
					if($value_dis != "" && $value_subdiv != ""){
							if($value_areatype != ""){
								if($value_block_mun != ""){
									if($value_areatype == 'B'){
										$titleLable = 'Select Gram Panchayat Name';
										$funName	= common_custom_user_villward_list_new($value_block_mun,'V');
									}else if($value_areatype == 'M'){
										$titleLable = 'Ward Number';
										$funName	= common_custom_user_villward_list_new($value_block_mun,'W');
									}else if($value_areatype == 'C'){
										$titleLable = 'Ward Number';
										$funName	= common_custom_user_corporation_list_new($value_block_mun);
									}else if($value_areatype == 'S'){
										$titleLable = 'Sector';
										$funName	= common_custom_user_villward_list_new($value_block_mun,'S');
									}else if($value_areatype == 'N'){
										$titleLable = 'Notified Area';
										$funName	= common_custom_user_villward_list_new($value_block_mun,'N');
									}
									
									
									$form['dashboard_master']['panchyat'] = array(
																				'#type' 		=> 'select',
																				'#title' 		=> $titleLable,
																				'#options' 		=> $funName,
																				'#default_value'=> $vill_ward_code,
																				'#required' 	=>  TRUE,
																				//'#attributes' 	=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Gram Panchayat'),
																				'#attributes' 	=> $readonly_person_vill_ward,
																				'#ajax'			=> array(
																										  'event' 		=> 'change',
																										  'effect' 		=> 'fade',			
																										  'callback' 	=> 'get_info_form_ajax_callback',
																										  'progress'	=>	array('type' => 'throbber','message' => 'Loading...'),
																									),	
																				'#prefix' 		=> '<div id="gp_ward_div"><div class="col-md-4"><label class="select">',
																				'#suffix' 		=> '</label></div></div>'
																			);
							 }
						 }
					}
						
						/***************************** Police Station block start **********************/
					$value_gp_ward 								= isset($form_state['values']['panchyat']) ? $form_state['values']['panchyat'] : $vill_ward_code;
			
					$policeData 								= array();
					$policeData[] 								= '--Select--';		
					$form['dashboard_master']['ps'] 		= array(
																	'#title' 		=> 'Select Police Station',
																	'#type' 		=> 'select',
																	'#options' 		=> $policeData,
																	'#default_value'=> $ps_code,
																	'#required' 	=>  TRUE,
																	//'#attributes' 	=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Police Station'),
																	'#attributes' 	=> $readonly_person_ps,
																	'#prefix' 		=> '<div id="ps_div"><div class="col-md-4"><label class="select">',
																	'#suffix' 		=> '</label></div></div>'
																);
					
					if($value_dis != "" && $value_subdiv != ""){
						if($value_areatype != "" && $value_areatype != "0"){
							if($value_block_mun != "" && $value_block_mun != "0"){
								if($value_gp_ward != "" && $value_gp_ward!='0'){
									$form['dashboard_master']['ps'] 		= array(
																						'#title' 			=> 'Select Police Station',
																						'#type' 			=> 'select',
																						'#options' 			=> common_custom_user_ps_list_new($value_dis),
																						'#default_value'	=> $ps_code,
																						'#required' 		=> TRUE,
																						//'#attributes' 		=> array('id' => 'loc_e_ps_id','autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Police Station'),
																						'#attributes' 		=> $readonly_person_ps,
																						'#prefix' 			=> '<div id="ps_div"><div class="col-md-4"><label class="select">',
																						'#suffix' 			=> '</label></div></div>'
																				);
								}
							}
						}
					}
					
						/***************************** Pin Code block start **********************/
						$form['dashboard_master']['pin'] 			= array(
																		'#title' 			=> 'Pin Code',
																		'#type' 			=> 'textfield',
																		'#required' 		=> TRUE,
																		'#default_value'	=> $pin,
																		'#element_validate' => array('numeric_validation'),
																		//'#attributes' 		=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Pin Code','maxlength' => 6),
																		'#attributes' 		=> $readonly_person_pincode,
																		'#prefix' 			=> '<div id="pincode_div"><div class="col-md-4"><label class="input">',
																		'#suffix' 			=> '</label></div></div>'
																);
						}
					}
				}
			}												  
		}																		  
	}
	
		
		/****************************** end of establishment ***********************/
		$form['dashboard_master']['markup1']							=	array(
																'#type'		=> 'markup',
																'#markup'	=> '</div></div>'
																
																);
		
		
		
		
		/***** ESTABLISHMENT DETAILS STARTS   ****/
		
		
		
		$form['dashboard_master']['est_name'] = array(
			'#title' 				=> 'Name of Establishment',
			'#type' 				=> 'textfield',
			'#default_value'		=> $est_name,
			'#attributes' 			=> array('id' => '','autocomplete' => 'off','placeholder'=>'Name of the Establishment'),
			'#required' 			=>  TRUE,
			// '#element_validate' 	=> array('DASHBOARD_TEXTBOX_TEXT_VALIDATE'),
			'#prefix' 				=> '<div class="panel panel-default panel-form">
											<div class="panel-heading">ESTABLISHMENT DETAILS</div>
											<div class="panel-body">
												<div class="col-md-4"><label class="input">',
			'#suffix' => '</lable></div>'
		);
		
		$form['dashboard_master']['type_ofthe_est'] = array(
			'#title' => 'Type of The Establishment',
			'#type' => 'select',
			'#options' => array('micro' => 'Micro', 'small' => 'Small', 'medium' => 'Medium', 'large' => 'Large'),
			'#required' =>  TRUE,
			'#default_value' => $type_ofthe_est,
			'#attributes' => array('id' => '', 'class'=>array('form-control')),
			'#prefix' => '<div class="col-md-4"><label class="select">',
			'#suffix' => '</label></div>'
		);
		$form['dashboard_master']['est_location'] = array(
			'#title' 				=> 'Location of The Establishment',
			'#type' 				=> 'textfield',
			'#required' 			=>  TRUE,
			'#default_value'		=> $est_location,
			'#attributes' 			=> array('id' => '','autocomplete' => 'off','placeholder'=>'Location'),
			'#prefix' 				=> '<div class="col-md-4"><label class="input">',
			'#suffix' 				=> '</lable></div>'
		);
																
		$form['dashboard_master']['est_district'] = array(
			'#title' 				=> 'Select District',
			'#type' 				=> 'select',
			'#options' 				=> common_custom_user_district_list_new(),
			'#default_value'		=> !empty($est_district) ? $est_district : '',
			'#required' 			=>  TRUE,
			'#attributes' 			=> array(),
			'#ajax'					=> array(
											'event' 	=> 'change',
											'effect' 	=> 'fade',			
											'callback' 	=> 'get_estb_registration_info_form_ajax_callback',
											'progress' 	=> '',
									),
			'#prefix' 				=> '<div class="col-md-4"><label class="select">',
			'#suffix' 				=> '</label></div>'
		);
																
		$value_loc_e_dist 						= isset($form_state['values']['est_district']) ? $form_state['values']['est_district'] : $est_district;	
		
		if($value_loc_e_dist != "" || !empty($value_loc_e_dist) || is_numeric($value_loc_e_dist)){
		
						
			$form['dashboard_master']['est_subdivision'] = array(
				'#prefix' 				=> '<div id="estb_subdvsn_replace"><div class="col-md-4"><label class="select">',
				'#suffix' 				=> '</label></div></div>',
				'#title'				=> 'Select Subdivision',
				'#type' 				=> 'select',
				'#default_value'		=> !empty($est_subdivision) ? $est_subdivision :'',
				'#options' 				=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist),
				'#attributes' 			=> array(),
				'#required' 			=>  TRUE,
				'#ajax'					=> array(
												'event' 	=> 'change',
												'effect' 	=> 'fade',			
												'callback' 	=> 'get_estb_registration_info_form_ajax_callback',
												'progress' 	=> '',
											),		
				
				
			);
		}else {
		
			$form['dashboard_master']['est_subdivision'] = array(
				'#prefix' 				=> '<div id="estb_subdvsn_replace"><div class="col-md-4"><label class="select">',
				'#suffix' 				=> '</lable></div></div>',
				'#title'				=> 'Select Subdivision',
				'#attributes' 			=> array(),
				'#type' 				=> 'select',
				'#options' 				=> array(),
				'#required' 			=> TRUE,
				'#ajax'					=> array(
												'event' 	=> 'change',
												'effect' 	=> 'fade',			
												'callback' 	=> 'get_estb_registration_info_form_ajax_callback',
												'progress' 	=> '',
									),		
			);
	}
	
	$value_loc_e_subdivision 	= isset($form_state['values']['est_subdivision']) ? $form_state['values']['est_subdivision'] : $est_subdivision;
	
	if($value_loc_e_dist != "" && $value_loc_e_subdivision != ""){
		
		
		$areatypeOptions = common_custom_user_areatype_list_new($value_loc_e_dist, $value_loc_e_subdivision);
		
		$form['dashboard_master']['est_areatype'] = array(
			'#prefix' 				=> '<div id="estb_loc_e_areatype"><div class="col-md-4"><label class="select">',
			'#suffix' 				=> '</label></div></div>',
			'#title'				=> 'Select AreaType',
			'#attributes' 			=> array(),
			'#type' 				=> 'select',
			'#options' 				=> $areatypeOptions,
			'#required' 			=>  TRUE,
			'#default_value'		=> !empty($est_areatype) ? $est_areatype :'',
			'#ajax'					=> array(
											'event' 	=> 'change',
											'effect' 	=> 'fade',			
											'callback' 	=> 'get_estb_registration_info_form_ajax_callback',
											'progress'	=> '',
									),	
			
		);
	}else{
		
		$form['dashboard_master']['est_areatype'] = array(
			'#title'				=> 'Select Area Type',
			'#type' 				=> 'select',
			'#options' 				=> array(),
			'#required' 			=> TRUE,
			'#ajax'					=> array(
											'event' 	=> 'change',
											'effect' 	=> 'fade',			
											'callback' 	=> 'get_estb_registration_info_form_ajax_callback',
											'progress' 	=> '',
									),	
			'#prefix' 				=> '<div id="estb_loc_e_areatype"><div class="col-md-4"><label class="select">',
			'#suffix' 				=> '</label></div></div>',
			'#attributes' 			=> array(),
		);
	}
	
	$value_loc_e_areatype 		= isset($form_state['values']['est_areatype']) ? $form_state['values']['est_areatype'] : $est_areatype;
	
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
		
		$form['dashboard_master']['est_areatype_name'] = array(
								  '#prefix' 			=> '<div id="estb_common_1_replace"><div class="col-md-4"><label class="select">',
								  '#suffix' 			=> '</label></div></div>',
								  '#title'				=> 'Select '.$areatype_name,
								  '#type' 				=> 'select',
								  '#attributes' 		=> array(),
								  '#options' 			=> common_custom_user_block_list_new($value_loc_e_subdivision,$value_loc_e_areatype),
								  '#default_value' 		=> !empty($est_areatype_code) ? $est_areatype_code : '',
								  '#required' 			=> TRUE,
								  '#ajax'				=> array(
																'event' 			=> 'change',
																'effect' 			=> 'fade',			
																'callback'			=> 'get_estb_registration_info_form_ajax_callback',
																'progress'			=> '',
															  )
								);
		
	}else{
		
		$form['dashboard_master']['est_areatype_name'] = array(
								  '#prefix' 			=> '<div id="estb_common_1_replace"><div class="col-md-4"><label class="select">',
								  '#suffix' 			=> '</label></div></div>',
								  '#title'				=> 'Select Block/Municipality/Corporation/SEZ/Notified Area',
								  '#attributes' 		=> array(),
								  '#type' 				=> 'select',
								  '#options' 			=> array(),
								  '#required' 			=> TRUE,
								  '#ajax'				=> array(
																'event' 			=> 'change',
																'effect' 			=> 'fade',			
																'callback'			=> 'get_estb_registration_info_form_ajax_callback',
																'progress'			=> '',
															  )
								);
	}
	
	$value_est_areatype_code 	= isset($form_state['values']['est_areatype_name']) ? $form_state['values']['est_areatype_name'] : $est_areatype_code;
	
	if($value_loc_e_dist != "" && $value_loc_e_subdivision != "" && $value_loc_e_areatype != ""  && $value_est_areatype_code != "" ){
		
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
		
		$form['dashboard_master']['est_vill_ward'] = array(
		  '#prefix' 			=> '<div id="estb_common_2_replace"><div class="col-md-4"><label class="select">',
		  '#suffix' 			=> '</label></div></div>',
		  '#attributes' 		=> array(),
		  '#title'				=> 'Select '.$area_name,
		  '#type' 				=> 'select',
		  '#options'			=> common_custom_user_villward_list_new($value_est_areatype_code,$area_code),
		  '#required' 			=> TRUE,
		  '#default_value' 		=> !empty($est_loc_e_vill_ward) ? $est_loc_e_vill_ward : '',
		  '#ajax'				=> array(
										'event' 			=> 'change',
										'effect' 			=> 'fade',			
										'callback'			=> 'get_estb_registration_info_form_ajax_callback',
										'progress'			=> '',
									  )
		);
		
	}else{
		
		$form['dashboard_master']['est_vill_ward'] = array(
		  '#prefix' 			=> '<div id="estb_common_2_replace"><div class="col-md-4"><label class="select">',
		  '#suffix' 			=> '</label></div></div>',
		  '#attributes' 		=> array(),
		  '#title'				=> 'Select Gram Panchayat/Ward/Sector/Notified Area',
		  '#type' 				=> 'select',
		  '#options' 			=> array(),
		  '#required' 			=> TRUE,
		  '#ajax'				=> array(
										'event' 			=> 'change',
										'effect' 			=> 'fade',			
										'callback'			=> 'get_estb_registration_info_form_ajax_callback',
										'progress'			=> '',
									  )
		);
		
	}
	
	$value_loc_vill_ward 			= 	isset($form_state['values']['est_vill_ward']) ? $form_state['values']['est_vill_ward'] :$est_loc_e_vill_ward;
	
	if ($value_loc_e_dist != "" && $value_loc_e_subdivision != "" && $value_loc_e_areatype != ""  && $value_est_areatype_code != "" && $value_loc_vill_ward != "" ){
		
		$form['dashboard_master']['est_policestation'] = array(
			  '#prefix' 			=> '<div id="estb_ps_replace"><div class="col-md-4"><label class="select">',
			  '#suffix' 			=> '</lable></div></div>',
			  '#attributes' 		=> array(),
			  '#title'				=> 'Select Police Station',
			  '#type' 				=> 'select',
			  '#options' 			=> common_custom_user_ps_list_new($value_loc_e_dist),
			  '#default_value' 		=> !empty($est_ps) ? $est_ps: '',
			  '#required' 			=> TRUE,
				  
			); 	
		
	}else{
		
		$form['dashboard_master']['est_policestation'] = array(
		  '#prefix' 			=> '<div id="estb_ps_replace"><div class="col-md-4"><label class="select">',
		  '#suffix' 			=> '</lable></div></div>',
		  '#title'				=> 'Select Police Station',
		  '#attributes' 		=> array(),
		  '#type' 				=> 'select',
		  '#options' 			=> array(),
		  '#required' 			=> TRUE,
		); 
	}
	
	$form['dashboard_master']['est_pincode'] = array(
		'#title' 				=> 'Pin Code',
		'#type' 				=> 'textfield',
		'#attributes' 			=> array(),
		'#required' 			=>  TRUE,
		'#default_value' 		=> !empty($est_pin_number) ? $est_pin_number: '',
		'#prefix' 				=> '<div class="col-md-4"><label class="input">',
		'#suffix' 				=> '</label></div></div></div>'
	);

		
		/*************************************************************************************************************************************************************/
		
		
		
		$options_ownership 	= fetch_ownership_data();
		
		$form['dashboard_master']['ownership'] 					= array(
																	  '#prefix' 			=> '<div class="panel panel-default panel-form"><div class="panel-heading">OWNERSHIP DETAILS OF ESTABLISHMENT OF WHICH APPLICATION TO BE SUBMITED</div><div class="panel-body"><div class="col-md-4"><label class="select">',
																	  '#suffix' 			=> '</label></div>',
																	  '#title'				=> 'Type of Ownership',
																	  '#type' 				=> 'select',
																	  '#options' 			=> $options_ownership,
																	  '#required' 			=> TRUE,
																	  '#default_value' 		=> !empty($val_ownership) ? $val_ownership	:'',
																	  '#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_ajax_ownership',
																								'progress'			=> '',
																							  )
																	);
													
		$ownership 							= 	isset($form_state['values']['ownership']) ? $form_state['values']['ownership'] : $val_ownership;
		
		
		
		$form['dashboard_master']['ownership_undertaking'] 		 		= array(
																				'#prefix' 				=> '<div id="ownership_replace_1">',
																				'#suffix' 				=> '</div>',
																			  );	
		
		$form['dashboard_master']['ownership_undertaking_text']			= array(
																			  '#prefix' 			=> '<div id="ownership_replace_text">',
																			  '#suffix' 			=> '</div>'
																				);
		
		
		$form['dashboard_master']['other_ownership']					= array(
																				'#prefix' 		=> '<div id="ownership_replace_5">',
																				'#suffix' 		=> '</div>',
																				);
		
		$form['dashboard_master']['other_government_name'] 				= array(
																			  '#prefix' 			=> '<div id="ownership_replace_2">',
																			  '#suffix' 			=> '</div>'
																		  );
		
		
		
		
		if($ownership != "" || !empty($ownership) || is_numeric($ownership)){
			if($ownership == 1){
				
				$form['dashboard_master']['ownership_undertaking']= array(
																	  '#prefix' 			=> '<div id="ownership_replace_1"><div class="col-md-4"><label class="select">',
																	  '#suffix' 			=> '</label></div></div>',
																	  '#title'				=> 'Select Government Ownerships',
																	  '#type' 				=> 'select',
																	  '#options' 			=> government_ownership_list($ownership),
																	  '#default_value' 		=> !empty($val_ownership_undertaking) ? $val_ownership_undertaking	:'',
																	  '#required' 			=> TRUE,
																	  '#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_ajax_ownership',
																								'progress'			=> '',
																							  ),
																);
			}
			
			if($ownership == 2){
				
				$form['dashboard_master']['ownership_undertaking']= array(
																	  '#prefix' 			=> '<div id="ownership_replace_1"><div class="col-md-4"><label class="select">',
																	  '#suffix' 			=> '</label></div></div>',
																	  '#title'				=> 'Select Private Ownerships',
																	  '#type' 				=> 'select',
																	  '#options' 			=> private_ownership_list($ownership),
																	  '#default_value' 		=> !empty($val_ownership_undertaking) ? $val_ownership_undertaking	:'',
																	  '#required' 			=> TRUE,
																	  '#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_ajax_ownership',
																								'progress'			=> '',
																							  ),
																);
			}
			
			if($ownership == 3 || $ownership == 4 || $ownership == 5 || $ownership == 6){
				
				$form['dashboard_master']['ownership_undertaking']= array(
																	  '#prefix' 			=> '<div id="ownership_replace_1">',
																	  '#suffix' 			=> '</div>'
																		);
			}
			if($ownership == 7){
				$default_value_ownership	= '';
				if($val_other_ownership==''){
					$default_value_ownership = '';
				}else{
					$default_value_ownership = $val_other_ownership;
				}
				
				
				
				$form['dashboard_master']['ownership_undertaking_text']= array(
																	  '#prefix' 		=> '<div id="ownership_replace_text"><div class="col-md-4"><label class="input">',
																	  '#suffix' 		=> '</label></div></div>',
																	  '#title'			=> 'Other Ownership',
																	  '#type' 			=> 'textfield',
																	  '#default_value' 	=> $default_value_ownership,
																	  '#required' 		=> TRUE
																);
			}
		}
		
		
		
		$ownership_undertaking 							= 	isset($form_state['values']['ownership_undertaking']) ? $form_state['values']['ownership_undertaking'] : '';
		
		if(empty($ownership_undertaking) &&!empty($val_ownership_undertaking))
			$ownership_undertaking	=	$val_ownership_undertaking;
		
		
		
		if($ownership != "" || !empty($ownership) || is_numeric($ownership)){
			if($ownership == 1 && $ownership_undertaking == 15){
				
				$form['dashboard_master']['other_government_name']= array(
																	  '#prefix' 		=> '<div id="ownership_replace_2"><div class="col-md-4"><label class="input">',
																	  '#suffix' 		=> '</label></div></div>',
																	  '#title'			=> 'Other Government',
																	  '#type' 			=> 'textfield',
																	  '#default_value' 	=> !empty($val_other_government_name) ? $val_other_government_name	:'',
																	  '#required' 		=> TRUE
																);
			}
		}
		
		$form['dashboard_master']['other_private_name'] 			= array(
																	  '#prefix' 			=> '<div id="ownership_replace_6">',
																	  '#suffix' 			=> '</div>'
																  );
		
		if($ownership != "" || !empty($ownership) || is_numeric($ownership)){
			
			if($ownership == 2 && $ownership_undertaking == 19){
				$form['dashboard_master']['other_private_name'] = array(
																	  '#prefix' 			=> '<div id="ownership_replace_6"><div class="col-md-4"><label class="input">',
																	  '#suffix' 			=> '</label></div></div>',
																	  '#title'				=> 'Other Private',
																	  '#type' 				=> 'textfield',
																	  '#default_value' 		=> !empty($val_other_private_name) ? $val_other_private_name	:'',
																	  '#required' 			=> TRUE
																);
			}
		}
		
		$form['dashboard_master']['local_bodies'] 				= array(
																	  '#prefix' 			=> '<div id="ownership_replace_3">',
																	  '#suffix' 			=> '</div>'
																  );
		
		if($ownership != "" || !empty($ownership) || is_numeric($ownership)){
			if($ownership == 1 && $ownership_undertaking == 11){
				$form['dashboard_master']['local_bodies'] 		= array(
																	  '#prefix' 			=> '<div id="ownership_replace_3"><div class="col-md-4"><label class="select">',
																	  '#suffix' 			=> '</label></div></div>',
																	  '#title'				=> 'Select Local Bodies',
																	  '#type' 				=> 'select',
																	  '#options' 			=> local_bodies_ownership_list($ownership_undertaking),
																	  '#default_value' 		=> !empty($val_local_bodies) ? $val_local_bodies	:'',
																	  '#required' 			=> TRUE,
																	  '#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_ajax_ownership',
																								'progress'			=> '',
																							  ),
																);
			}
		}
		
		$local_bodies 										= 	isset($form_state['values']['local_bodies']) ? $form_state['values']['local_bodies'] : '';
		
		if(empty($local_bodies) &&!empty($val_local_bodies))
			$local_bodies	=	$val_local_bodies;
		
		$form['dashboard_master']['urban_local_bodies'] 		= array(
																  '#prefix' 			=> '<div id="ownership_replace_4">',
																  '#suffix' 			=> '</div>'
															  );
															  
		if($ownership != "" || !empty($ownership) || is_numeric($ownership)){
			if($ownership == 1 && $ownership_undertaking == 11){
				if($local_bodies == 23){
					
					$form['dashboard_master']['urban_local_bodies']= array(
																	  '#prefix' 		=> '<div id="ownership_replace_4"><div class="col-md-4"><label class="select">',
																	  '#suffix' 		=> '</label></div></div>',
																	  '#title'			=> 'Select Urban Local Bodies',
																	  '#type' 			=> 'select',
																	  '#options' 		=> urban_local_bodies_ownership_list($local_bodies),
																	  '#default_value' 	=> !empty($val_urban_local_bodies) ? $val_urban_local_bodies	:'',
																	  '#required' 		=> TRUE,
																	  '#ajax'			=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_ajax_ownership',
																								'progress'			=> '',
																							  ),
																);
				}
			}
			/*																	
			if($ownership != "" || !empty($ownership) || is_numeric($ownership)){
				if($ownership == 1 && $ownership_undertaking == 15){
					$form['dashboard_master']['other_ownership'] 		= array(
																		  '#prefix' 		=> '<div id="ownership_replace_5"><section class="col col-6"><label class="input">',
																		  '#suffix' 		=> '<i></i></label></section></div>',
																		  '#title'			=> 'Other Ownership',
																		  '#type' 			=> 'textfield',
																		  '#default_value' 	=> !empty($val_other_ownership) ? $val_other_ownership	:'',
																		  '#required' 		=> TRUE
																	);
				}else if($ownership == 2 && $ownership_undertaking == 19){
					$form['dashboard_master']['other_ownership'] 		= array(
																		  '#prefix' 		=> '<div id="ownership_replace_5"><section class="col col-6"><label class="input">',
																		  '#suffix' 		=> '<i></i></label></section></div>',
																		  '#title'			=> 'Other Ownership',
																		  '#type' 			=> 'textfield',
																		  '#default_value' 	=> !empty($val_other_ownership) ? $val_other_ownership	:'',
																		  '#required' 		=> TRUE
																	);
				}
			}	*/
		}
		$form['dashboard_master']['markup2']			= array(
																			'#type'	=> 'markup',
																			'#markup'	=> '</div></div>',
																			);
		
		
		
		
		
		
		/**********************************************************************************************/
	
	
	/***** PRINCIPAL EMPLOYER / CONTRACTOR DETAILS STARTS   ****/
		
		$form['dashboard_master']['pe_con_name'] = array(
			'#title' 				=> 'Name',
			'#type' 				=> 'textfield',
			'#default_value'		=> $pe_con_name,
			'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('form-control'),'placeholder'=>'Name of the Principal Employer / Contractor'),
			'#required' 			=>  TRUE,
			'#prefix' => '<div class="panel panel-default panel-form">
							<div class="panel-heading">PRINCIPAL EMPLOYER / CONTRACTOR DETAILS (As Per Aadhaar)</div>
							<div class="panel-body">
								<div class="col-md-4"><label class="input">',
			'#suffix' => '</lable></div>'
		);	
		
		$genderOptions 			=	array('M'=>t('Male'), 'F'=>t('Female'), 'O'=>t('Other'));
		
		$form['dashboard_master']['pe_con_gender'] = array(
		  '#prefix'				=> '<div class="col-md-3"><div class="inline-group">',
		  '#suffix'				=>'</div></div>',
		  '#type' 				=> 'radios',
		  '#title' 				=> t('Gender'),
		  '#options' 			=> $genderOptions,
		  '#default_value' 		=> isset($pe_con_gender) ? $pe_con_gender : 'M',
		  '#required' 			=> TRUE
		);
		
		
		
		if(empty($pe_con_dob)){
			$form['dashboard_master']['pe_con_dob'] = array(
				'#prefix'			=> '<div class="col-md-2"><label class="date">',
				'#suffix'			=> '</label></div>',
				'#type' 			=> 'date',
				'#title'			=>'Date Of Birth',
				'#required' 		=> TRUE,
				'#after_build' 		=> array("applicant_forms_format_dob"),
				'#element_validate' => array('applicant_dob_validate'),
			); 
		}else{
			$form['dashboard_master']['pe_con_dob'] = array(
				'#prefix'			=> '<div class="col-md-2"><label class="date">',
				'#suffix'			=> '</label></div>',
				'#type' 			=> 'date',
				'#title'			=> 'Date Of Birth',
				'#required' 		=> TRUE,
				'#after_build' 		=> array("applicant_forms_format_dob"),
				'#element_validate' => array('applicant_dob_validate'),
				'#default_value' 	=> array(
											 'month' => format_date(strtotime($pe_con_dob), 'custom',  'n'),
											 'day' => format_date(strtotime($pe_con_dob), 'custom',  'j'),
											 'year' => format_date(strtotime($pe_con_dob), 'custom',  'Y'),
											),
				'#attributes' 		=> array('class' => array('datefieldapp'))
			);
		}
		
		$form['dashboard_master']['pe_con_mobile'] = array(
		  '#prefix'				=> '<div class="col-md-3"><label class="input">',
		  '#suffix'				=> '</label></div>',
		  '#title'				=> 'Mobile',
		  '#type' 				=> 'textfield',
		  '#default_value' 		=>!empty($pe_con_mobile) ? $pe_con_mobile : '',
		  '#attributes' 		=> array('placeholder' => 'Enter your 10 digit Mobile Number', 'maxlength' => 10),
		  '#required' 			=> TRUE,
		  '#element_validate' 	=> array('DASHBOARD_TEXTBOX_CONTACT_NUMBER_VALIDATE')
		);
		
		
		$form['dashboard_master']['pe_con_address'] = array(
			'#prefix' 			=>	'<br class="clear"><div class="col-md-4"><label class="input">',
			'#suffix' 			=>	'</label></div>',
			'#title'			=>	t('Address Line1'),
			'#default_value'	=>	$pe_con_address,
			'#type'				=>	'textarea',
			'#required' 		=> 	TRUE,
			'#attributes' 		=> 	array('class' => array('form-control'))
		);
	
		$options_country 	= array(''=>'-Select-', 1 => 'India', 2 => 'Others'); 
		// country block start 	
		
		$form['dashboard_master']['pe_con_country'] = array(
				'#prefix' 				=> '<div class="col-md-4"><label class="select">',
				'#suffix' 				=> '</label></div>',
				'#title'				=> 'Country',
				'#type' 				=> 'select',
				'#options' 				=> $options_country,
				'#default_value'		=> $pe_con_country,
				'#attributes' 			=> array('class' => array('form-control')),
				'#required' 			=> TRUE,
				'#ajax'					=> array(
												'event' 			=> 'change',
												'effect' 			=> 'fade',			
												'callback'			=> 'get_pe_con_ajax_callback',
												'progress'			=>	'',
										)
		);
		
		// country block end
	
		// Default block start  									
		$value_pe_con_country = isset($form_state['values']['pe_con_country']) ? $form_state['values']['pe_con_country'] : $pe_con_country;
		
		$form['dashboard_master']['pe_con_state'] = array(
			'#prefix' => '<div id="pe_con_state_replace">',
			'#suffix' => '</div>',
		);
				
		$form['dashboard_master']['pe_con_district'] = array(
			'#prefix' => '<div id="pe_con_district_replace">',
			'#suffix' => '</div>',
		);
		$form['dashboard_master']['pe_con_subdivision'] = array(
			'#prefix' => '<div id="pe_con_subdivision_replace">',
			'#suffix' => '</div>',
		);
		$form['dashboard_master']['pe_con_areatype'] = array(
			'#prefix' => '<div id="pe_con_areatype_replace">',
			'#suffix' => '</div>',
		);
		$form['dashboard_master']['pe_con_block_code'] = array(
			'#prefix' => '<div id="pe_con_block_code_replace">',
			'#suffix' => '</div>',
		);
		$form['dashboard_master']['pe_con_gp'] = array(
			'#prefix' => '<div id="pe_con_gp_replace">',
			'#suffix' => '</div>',
		);
		$form['dashboard_master']['pe_con_ps'] = array(
			'#prefix' => '<div id="pe_con_ps_replace">',
			'#suffix' => '</div>',
		);
		$form['dashboard_master']['pe_con_pin'] = array(
			'#prefix' => '<div id="pe_con_pin_replace">',
			'#suffix' => '</div>',
		);
		
		// Default block end
	
 	if(!empty($value_pe_con_country) || $value_pe_con_country !=''){
		$db_pe_con_state_value = db_query("SELECT COUNT(id) count FROM {state_master} WHERE country_id = ".$value_pe_con_country."")->fetchObject();
	}else{
		$db_pe_con_state_value = array();
	}
	if(!empty($value_pe_con_country) && !empty($db_pe_con_state_value) && ($db_pe_con_state_value->count > 0)){
		// india section
			/***************************** State block start **********************/
		$options_pe_con_state = get_all_state1($value_pe_con_country);
		
        $form['dashboard_master']['pe_con_state'] = array(
				'#prefix' => '<div class="col-md-4" id="pe_con_state_replace"><label class="select">',
				'#suffix' => '</label></div>',
				'#title'			=> t('Select State').$value_pe_con_state,
				'#type' 			=> 'select',
				'#options' 			=> $options_pe_con_state,
				'#default_value'	=> $pe_con_state,
				'#required' 		=> TRUE,
				'#attributes' 		=> array('class' => array('form-control')),
				'#ajax'				=> array(
											'event'		=> 'change',
											'effect' 	=> 'fade',
											'callback'	=> 'get_pe_con_ajax_callback',
										)
		);
			/***************************** District block start **********************/
		
		if($form_state['values']['pe_con_state']!=''){
			$value_pe_con_state = $form_state['values']['pe_con_state'];
		}else{
			$value_pe_con_state = $pe_con_state;
		}
		
		$options_dist = array(''=>'-Select-');																  
		
		if(($value_pe_con_country!='' && (!empty($value_pe_con_state) || $value_pe_con_state!=0 || $value_pe_con_state!=''))){
			
			$db_district_value = db_query("SELECT COUNT(id) count FROM {district_master} WHERE state_id =".$value_pe_con_state." AND is_active='Y' ")->fetchObject();
			
			if(($db_district_value->count>0) && (!empty($value_pe_con_state) || $value_pe_con_state!=0)){
				
				$form['dashboard_master']['pe_con_district'] = array(
						'#prefix' 			=> '<div class="col-md-4" id="pe_con_district_replace"><label class="select">',
						'#suffix' 			=> '</label></div>',
						'#title'			=> t('Select District'),
						'#type' 			=> 'select',
						'#options' 			=> get_discrict_by_id($value_pe_con_state),
						'#default_value'	=> $pe_con_district,
						'#attributes' 		=> array('class' => array('form-control')),
						'#required' 		=> TRUE,
						'#ajax'				=> array(
													'event'		=> 'change',
													'effect' 	=> 'fade',
													'callback'	=> 'get_pe_con_ajax_callback',
												)
				);
														
				/***************************** Sub Division block start **********************/
				$value_pe_con_district								= isset($form_state['values']['pe_con_district'])?$form_state['values']['pe_con_district'] : $pe_con_district;
				
				if(!empty($value_pe_con_district) || $value_pe_con_district!='' || $value_pe_con_district!=0){
					$db_pe_con_subdivision_value					= db_query("SELECT COUNT(id) count FROM {sub_division} WHERE district_code ='".$value_pe_con_district."' AND is_active='Y' ")->fetchObject();
				}else{
					$db_pe_con_subdivision_value					= 0;
				}
				
				if($db_pe_con_subdivision_value->count > 0){
                				
					if(($value_pe_con_state!='' || $value_pe_con_state!=0) && ($value_pe_con_district!="" || !empty($value_pe_con_district) || $value_pe_con_district!=0)){
						$form['dashboard_master']['pe_con_subdivision']	= array(
								'#prefix' => '<div class="col-md-4" id="pe_con_subdivision_replace"><label class="select">',
								'#suffix' 			=> '</lable></div>',
								'#title'			=> t('Select Sub-Division'),
								'#type' 			=> 'select',
								'#options' 			=> common_custom_user_subdivisionlist_list_new($value_pe_con_district),
								'#default_value'	=> $pe_con_subdivision,
								'#attributes' 		=> array('class' => array('form-control')),
								'#required' 		=> TRUE,
								'#ajax'				=> array(
															'event'		=> 'change',
															'effect' 	=> 'fade',
															'callback'	=> 'get_pe_con_ajax_callback',
														)
						);
					}else{
						$form['dashboard_master']['pe_con_subdivision']	= array(
								'#prefix' => '<div class="col-md-4" id="pe_con_subdivision_replace"><label class="select">',
								'#suffix' 			=> '</lable></div>',
								'#title'			=> t('Select Sub-Division'),
								'#type' 			=> 'select',
								'#options' 			=> common_custom_user_subdivisionlist_list_new($value_pe_con_district),
								'#default_value'	=> $subdivision_code,
								'#attributes' 		=> array('class' => array('form-control')),
								'#required' 		=> TRUE,
								'#ajax'				=> array(
																'event'		=> 'change',
																'effect' 	=> 'fade',
																'callback'	=> 'get_pe_con_ajax_callback',
															)
							);
					
					
					}
					
					/***************************** Area Type block start **********************/				
					
					$value_pe_con_subdivision = isset($form_state['values']['pe_con_subdivision']) ? $form_state['values']['pe_con_subdivision'] : $pe_con_subdivision;
					
                    if(($value_pe_con_district!='' || !empty($value_pe_con_district) || $value_pe_con_district!=0) && (!empty($value_pe_con_subdivision) || $value_pe_con_subdivision!='' || $value_pe_con_subdivision!=0)){
						
                        $db_pe_con_areatype_value = db_query("SELECT COUNT(id) FROM {block_mun_master} WHERE distict_code ='".$value_pe_con_district."' AND sub_division_code=".$value_pe_con_subdivision." AND is_active='Y' ")->fetchObject(); 
					
                    }else{
						$db_pe_con_areatype_value = 0;
					}
					
					if($db_pe_con_areatype_value->count>0){
					
						$areaTypeData = array();
						if($value_pe_con_district != "" && $value_pe_con_subdivision != ""){	
							$areatypeOptions = common_custom_user_areatype_list_new($value_pe_con_district, $value_pe_con_subdivision);
							if(empty($areatypeOptions)){
								$areatypeOptions = array();
							}
							
							$form['dashboard_master']['pe_con_areatype'] = array(
									'#prefix' 			=> '<div class="col-md-4" id="pe_con_areatype_replace"><label class="select">',
									'#suffix' 			=> '</label></div>',
									'#title'			=> 'Select Area Type',
									'#type' 			=> 'select',
									'#options' 			=> $areatypeOptions,
									'#default_value'	=> $pe_con_areatype,
									'#required' 		=> TRUE,
									'#attributes' 		=> array('class' => array('form-control'), 'autocomplete' => 'off', 'placeholder'=>'Select Area Type'),
									'#ajax'				=> array(
															'event' 	=> 'change',
															'effect' 	=> 'fade',			
															'callback' 	=> 'get_pe_con_ajax_callback',																								
														)	
							);
						}else {
							$form['dashboard_master']['pe_con_areatype'] = array(
									'#prefix' 			=> '<div class="col-md-4" id="pe_con_areatype_replace"><label class="select">',
									'#suffix' 			=> '</lable></div>',
									'#title'			=> 'Select Area Type',
									'#type' 			=> 'select',
									'#options' 			=> $areaTypeData,
									'#default_value'	=> $areatype_code,
									'#attributes' 		=> array('class' => array('form-control')),
									'#required' 		=> TRUE,
									'#attributes' 		=> array('autocomplete' => 'off', 'placeholder'=>'Select Area Type'),
									'#ajax'				=> array(
															'event' 	=> 'change',
															'effect' 	=> 'fade',			
															'callback' 	=> 'get_pe_con_ajax_callback',
															'progress'	=>	'',
														)	
							);
						}
						
						
						/***************************** Block/Municiple block start **********************/
						$value_pe_con_areatype = isset($form_state['values']['pe_con_areatype']) ? $form_state['values']['pe_con_areatype'] : $pe_con_areatype;
			
						$blockData = array();
						$blockData[] = 'Select';	
						
						$form['dashboard_master']['pe_con_block_code'] = array(
							'#prefix' 		=> '<div class="col-md-4" id="pe_con_block_code_replace"><label class="select">',
							'#suffix' 		=> '</lable></div>',
							'#title'		=> 'Select Block',
							'#type' 		=> 'select',
							'#options' 		=> $blockData,
							'#default_value'=> $pe_con_block_code,
							'#required' 	=> TRUE,
							'#attributes' 	=> array('autocomplete' => 'off','placeholder'=>'Select Block/Municipality/Corporation/SEZ/Notified Area'),
							'#ajax'			=> array(
													'event' 	=> 'change',
													'effect' 	=> 'fade',			
													'callback' 	=> 'get_pe_con_ajax_callback',
													'progress'	=>	'',
												),	
								
						);
						
						if($value_pe_con_district != 0 && $value_pe_con_subdivision != 0){
							if($value_pe_con_areatype != ""){
								if ( $value_pe_con_areatype == 'B' ){
									$pe_con_areatype_name	= 'Block';
								}elseif( $value_pe_con_areatype == 'M'){
									$pe_con_areatype_name	= 'Municipality';
								}elseif( $value_pe_con_areatype == 'C'){
									$pe_con_areatype_name	= 'Corporation';
								}elseif( $value_pe_con_areatype == 'S'){
									$pe_con_areatype_name	= 'SEZ';
								}elseif($value_pe_con_areatype == 'N'){
									$pe_con_areatype_name	= 'Notified Area';
								}
								
								$form['dashboard_master']['pe_con_block_code'] = array(
									'#prefix' 		=> '<div class="col-md-4" id="pe_con_block_code_replace"><label class="select">',
									'#suffix' 		=> '</lable></div>',
									'#title'		=> 'Name of '.$pe_con_areatype_name,
									'#type' 		=> 'select',
									'#options' 		=> common_custom_user_block_list_new($value_pe_con_subdivision,$value_pe_con_areatype),
									'#default_value'=> $pe_con_block_code,
									'#required' 	=> TRUE,
									'#attributes' 	=> array('autocomplete' => 'off'),
									'#ajax'			=> array(
															'event' 	=> 'change',
															'effect' 	=> 'fade',			
															'callback' 	=> 'get_pe_con_ajax_callback',
															'progress'	=>	'',
														),	
										
								);
							}
						}
						
						
						$value_pe_con_block_code 					= isset($form_state['values']['pe_con_block_code']) ? $form_state['values']['pe_con_block_code'] : $pe_con_block_code;
						if(!empty($value_pe_con_block_code) || $value_pe_con_block_code!='' || $value_pe_con_block_code!=0){
							$db_pe_con_gp						= db_query("Select COUNT(id) FROM {village_ward_master} WHERE block_code=".$value_pe_con_block_code." ")->fetchObject();
						}else{
							$db_pe_con_gp						= 0;
						}	
						
						
						if($db_vil_ward->count>0){
							
							/***************************** GP/Ward block start **********************/
							
							$panchyatData 						= array();
							$panchyatData[] 					= 'Select';	
							
							$form['dashboard_master']['pe_con_gp'] = array(
								'#prefix' 		=> '<div class="col-md-4" id="pe_con_gp_replace"><label class="select">',
								'#suffix' 		=> '</label></div>',
								'#title' 		=> 'Select Gram Panchayat/Ward',
								'#type' 		=> 'select',
								'#options' 		=> $panchyatData,
								'#default_value'=> $pe_con_gp,
								'#required' 	=>  TRUE,
								'#attributes' 	=> array('class'=>array('form-control')),
								'#ajax'			=> array(
														'event' 	=> 'change',
														'effect' 	=> 'fade',			
														'callback' 	=> 'get_pe_con_ajax_callback',
														'progress'	=>	'',
													  ),	
							);
							
							if($value_pe_con_district != "" && $value_pe_con_subdivision != ""){
									if($value_areatype != ""){
										if($value_pe_con_block_code != ""){
											if($value_pe_con_areatype == 'B'){
												$titleLable = 'Select Gram Panchayat Name';
												$funName	= common_custom_user_villward_list_new($value_pe_con_block_code,'V');
											}else if($value_pe_con_areatype == 'M'){
												$titleLable = 'Ward Number';
												$funName	= common_custom_user_villward_list_new($value_pe_con_block_code,'W');
											}else if($value_pe_con_areatype == 'C'){
												$titleLable = 'Ward Number';
												$funName	= common_custom_user_corporation_list_new($value_pe_con_block_code);
											}else if($value_pe_con_areatype == 'S'){
												$titleLable = 'Sector';
												$funName	= common_custom_user_villward_list_new($value_pe_con_block_code,'S');
											}else if($value_pe_con_areatype == 'N'){
												$titleLable = 'Notified Area';
												$funName	= common_custom_user_villward_list_new($value_pe_con_block_code,'N');
											}
											
											
											$form['dashboard_master']['pe_con_gp'] = array(
												'#prefix' 		=> '<div class="col-md-4" id="pe_con_gp_replace"><label class="select">',
												'#suffix' 		=> '</label></div>',
												'#type' 		=> 'select',
												'#title' 		=> $titleLable,
												'#options' 		=> $funName,
												'#default_value'=> $pe_con_gp,
												'#required' 	=>  TRUE,
												'#attributes' 	=> array('class'=>array('form-control')),
												'#ajax'			=> array(
																		  'event' 		=> 'change',
																		  'effect' 		=> 'fade',			
																		  'callback' 	=> 'get_pe_con_ajax_callback',
																		  'progress'	=>	'',
																	),	
												
											);
									 }
								 }
							}
							
							/***************************** Police Station block start **********************/
							$value_pe_con_gp 								= isset($form_state['values']['pe_con_gp']) ? $form_state['values']['pe_con_gp'] : $pe_con_gp;
					
							$policeData 								= array();
							$policeData[] 								= '--Select--';		
							$form['dashboard_master']['pe_con_ps'] = array(
								'#prefix' 		=> '<div class="col-md-4" id="pe_con_ps_replace"><label class="select">',
								'#suffix' 		=> '</label></div>',
								'#title' 		=> 'Select Police Station',
								'#type' 		=> 'select',
								'#options' 		=> $policeData,
								'#default_value'=> $pe_con_ps,
								'#required' 	=>  TRUE,
								
							);
							
							if($value_pe_con_district != "" && $value_pe_con_subdivision != ""){
								if($value_areatype != "" && $value_areatype != "0"){
									if($value_pe_con_block_code != "" && $value_pe_con_block_code != "0"){
										if($value_pe_con_gp != "" && $value_pe_con_gp!='0'){
											$form['dashboard_master']['pe_con_ps'] = array(
												'#prefix' 			=> '<div class="col-md-4" id="pe_con_ps_replace"><label class="select">',
												'#suffix' 			=> '</label></div>',											
												'#title' 			=> 'Select Police Station',
												'#type' 			=> 'select',
												'#options' 			=> common_custom_user_ps_list_new($value_pe_con_district),
												'#default_value'	=> $pe_con_ps,
												'#required' 		=> TRUE,
												'#attributes' 	=> array('class'=>array('form-control')),										
											);
										}
									}
								}
							}
							
							/***************************** Pin Code block start **********************/
							$form['dashboard_master']['pe_con_pin'] = array(
								'#prefix' 			=> '<div class="col-md-4" id="pe_con_pin_replace"><label class="input">',
								'#suffix' 			=> '</label></div>',
								'#title' 			=> 'Pin Code',
								'#type' 			=> 'textfield',
								'#required' 		=> TRUE,
								'#default_value'	=> $pe_con_pin,
								'#element_validate' => array('numeric_validation'),
								'#attributes' 	=> array('maxlength' => 6),	
							);
							
						}					
					}
				}
				
			}
		}
		
	}
		
	
		
	$form['dashboard_master']['pe_con_markup'] = array(
		'#type' => 'markup',
		'#markup' => '</div></div></div>'
	);
	
	
	
	/*************************************************************************************************************************************************************/
		
		
	/***** WORKER DETAILS STARTS  ****/
	
	
		
	$form['dashboard_master']['work_man_master_role_male'] 	= array(
		'#title' 			=> 'Male',
		'#type' 			=> 'textfield',
		'#attributes' 		=> array('id' => '','autocomplete' => 'off','class'=>array(''),'placeholder'=>'Male'),
		'#required' 		=>  TRUE,
		'#default_value'	=>	$work_man_master_role_male,
		//'#element_validate' => array('DASHBOARD_TEXTBOX_TEXT_VALIDATE'),
		'#prefix' 			=> '<div class="panel panel-default panel-form">
									<div class="panel-heading" id="worker_details">WORKER DETAILS</div>
									<div class="panel-body">
										<div class="sky-form">
											<fieldset>
													<div class="col-md-3">
														<label>No of workman of master role / regular</label>
													</div>
													<div class="col-md-2"><label class="input">',
		'#suffix' 			=> '</label></div>'
	);
																
	$form['dashboard_master']['work_man_master_role_female']	= array(
		'#title' 			=> 'Female',
		'#type' 			=> 'textfield',
		'#attributes' 		=> array('id' => '','autocomplete' => 'off','class'=>array(''),'placeholder'=>'Female'),
		'#required' 		=>  TRUE,
		'#default_value'	=>	$work_man_master_role_female,
		//'#element_validate' => array('DASHBOARD_TEXTBOX_TEXT_VALIDATE'),
		'#prefix' 			=> '<div class="col-md-2"><label class="input">',
		'#suffix' 			=> '</label></div>'
	
	);	
																														
	 $form['dashboard_master']['master_role_adolescent_male']	= array(
		'#title'			=> 'Adolescent Male',
		'#type' 			=> 'textfield',
		'#attributes' 		=> array('id' => '','autocomplete' => 'off','class'=>array(''),'placeholder'=>'Adolescent Male'),
		'#required' 		=>  TRUE,
		'#default_value'	=>	$master_role_adolescent_male,
		//'#element_validate' => array('DASHBOARD_TEXTBOX_TEXT_VALIDATE'),
		'#prefix' 			=> '<div class="col-md-2"><label class="input">',
		'#suffix' 			=> '</label></div>'
	
	 );		
																
	 $form['dashboard_master']['master_role_adolescent_female']	= array(
		'#title'			=> 'Adolescent Female',
		'#type' 			=> 'textfield',
		'#attributes' 		=> array('id' => '','autocomplete' => 'off','class'=>array(''),'placeholder'=>'Adolescent Female'),
		'#required' 		=>  TRUE,
		'#default_value'	=>	$master_role_adolescent_female,
		//'#element_validate' => array('DASHBOARD_TEXTBOX_TEXT_VALIDATE'),
		'#prefix' 			=> '<div class="col-md-2"><label class="input">',
		'#suffix' 			=> '</label></div></fieldset>'
	
	 );	
	
	$form['dashboard_master']['contractual_labour_male'] 	= array(
		//'#title' 			=> 'No of workman of master role / regular',
		'#type' 			=> 'textfield',
		'#attributes' 		=> array('id' => '','autocomplete' => 'off','class'=>array(''),'placeholder'=>'No of contractual labour Male'),
		'#required' 		=>  TRUE,
		'#default_value'	=>	$contractual_labour_male,
		//'#element_validate' => array('DASHBOARD_TEXTBOX_TEXT_VALIDATE'),
		'#prefix' 			=> '<fieldset>
									<div class="col-md-3"><label>No of contractual labour</label></div>
									<div class="col-md-2"><label class="input">',
		'#suffix' 			=> '</label></div>'
	);
		
	$form['dashboard_master']['contractual_labour_female']	= array(
		'#type' 			=> 'textfield',
		'#attributes' 		=> array('id' => '','autocomplete' => 'off','class'=>array(''),'placeholder'=>'No of contractual labour Female'),
		'#required' 		=>  TRUE,
		'#default_value'	=>	$contractual_labour_female,
		//'#element_validate' => array('DASHBOARD_TEXTBOX_TEXT_VALIDATE'),
		'#prefix' 			=> '<div class="col-md-2"><label class="input">',
		'#suffix' 			=> '</label></div>'
	
	);	
																
																
	$form['dashboard_master']['contractual_labour_adolescent_male']	= array(
		'#type' 			=> 'textfield',
		'#attributes' 		=> array('id' => '','autocomplete' => 'off','class'=>array(''),'placeholder'=>'Adolescent Male'),
		'#required' 		=>  TRUE,
		'#default_value'	=>	$contractual_labour_adolescent_male,
		//'#element_validate' => array('DASHBOARD_TEXTBOX_TEXT_VALIDATE'),
		'#prefix' 			=> '<div class="col-md-2"><label class="input">',
		'#suffix' 			=> '</label></div>'
	
	);	
																														
	$form['dashboard_master']['contractual_labour_adolescent_female']	= array(
		'#type' 			=> 'textfield',
		'#attributes' 		=> array('id' => '','autocomplete' => 'off','class'=>array(''),'placeholder'=>'Adolescent Female'),
		'#required' 		=>  TRUE,
		'#default_value'	=>	$contractual_labour_adolescent_female,
		//'#element_validate' => array('DASHBOARD_TEXTBOX_TEXT_VALIDATE'),
		'#prefix' 			=> '<div class="col-md-2"><label class="input">',
		'#suffix' 			=> '</label></div></fieldset>'
	
	);													
		
	
	$form['dashboard_master']['other_labour_male'] 	= array(
		//'#title' 			=> 'No of workman of master role / regular',
		'#type' 			=> 'textfield',
		'#attributes' 		=> array('id' => '','autocomplete' => 'off','class'=>array(''),'placeholder'=>'No of other labour Male'),
		'#required' 		=>  TRUE,
		'#default_value'	=>	$child_labour_male,
		//'#element_validate' => array('DASHBOARD_TEXTBOX_TEXT_VALIDATE'),
		'#prefix' 			=> '<fieldset>
									<div class="col-md-3"><label>No of other worker engaged</label></div>
									<div class="col-md-2"><label class="input">',
		'#suffix' 			=> '</label></div>'
	);
																
																
	$form['dashboard_master']['other_labour_female'] = array(
		'#type' 			=> 'textfield',
		'#attributes' 		=> array('id' => '','autocomplete' => 'off','class'=>array(''),'placeholder'=>'No of contractual labour Female'),
		'#required' 		=>  TRUE,
		'#default_value'	=>	$child_labour_female,
		//'#element_validate' => array('DASHBOARD_TEXTBOX_TEXT_VALIDATE'),
		'#prefix' 			=> '<div class="col-md-2"><label class="input">',
		'#suffix' 			=> '</label></div>'
	
	);	
																														
	$form['dashboard_master']['other_labour_adolescent_male']	= array(
		'#type' 			=> 'textfield',
		'#attributes' 		=> array('id' => '','autocomplete' => 'off','class'=>array(''),'placeholder'=>'Adolescent Male'),
		'#required' 		=>  TRUE,
		'#default_value'	=>	$other_labour_adolescent_male,
		//'#element_validate' => array('DASHBOARD_TEXTBOX_TEXT_VALIDATE'),
		'#prefix' 			=> '<div class="col-md-2"><label class="input">',
		'#suffix' 			=> '</label></div>'
	
	);															
		
	$form['dashboard_master']['other_labour_adolescent_female']	= array(
		'#type' 			=> 'textfield',
		'#attributes' 		=> array('id' => '','autocomplete' => 'off','class'=>array(''),'placeholder'=>'Adolescent Female'),
		'#required' 		=>  TRUE,
		'#default_value'	=>	$other_labour_adolescent_female,
		//'#element_validate' => array('DASHBOARD_TEXTBOX_TEXT_VALIDATE'),
		'#prefix' 			=> '<div class="col-md-2"><label class="input">',
		'#suffix' 			=> '</label></div></fieldset></div></div></div>'
	
	);	
		
		
		
		
		/**********************************************************************************************/
		
								
		$form['dashboard_master']['submit'] = array (
		  '#prefix' 	=> '<div class="col-md-12"><div class="row">',  
		  '#suffix' 	=> '</div></div>', 
		  '#type' 		=> 'submit',
		  '#attributes' => array('class' => array('btn btn-primary')),
		  '#value' 		=> 'UPDATE'
	   );
		return $form;
	}
	

	
	function DASHBOARD_DIS_VALIDATE($element, &$form_state){
		$val 		= 	$form_state['values'];
		$district 	= 	$val['district'];
		if( $district == 0 ){
			form_error($element, t('District should not be blank.'));
		}
	}
	
	function DASHBOARD_BLCK_VALIDATE($element, &$form_state){
		$val 	= 	$form_state['values'];
		$block 	= 	$val['block_code'];
		if( $block == 0 ){
			form_error($element, t('Block should not be blank.'));
		}
	}
	
	function DASHBOARD_TEXTBOX_CONTACT_NUMBER_VALIDATE($element,&$form_state){
		 $val	= $form_state['values'];
		 $mob	= "/^[789][0-9]{9}$/";
		 if( strlen(trim($val['mobile'])) > 0 ){   
			if(!preg_match($mob, $val['mobile'])){ 
			  if (strlen(trim($val['mobile'])) < 10){
					form_error($element, t($element['#title'].' is less than 10 digit . '));
			  }else{
					form_error($element, t($element['#title'].' SHOULD BE VALID NUMBER'));
			  }
			}
		 }else{
			form_error($element, t($element['#title'].' CAN NOT BE LEFT BLANK'));
		 }
	}
	
	function DASHBOARD_TEXTBOX_EMAIL_VALIDATE($element,&$form_state){
		$val			=	$form_state['values'];
		$emailval 		= 	'/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,4})$/';
		if(!preg_match($emailval, $val['email'])){ 
			form_error($element, t($element['#title'].' SHOULD BE VALID'));
		}
	}
	
	function DASHBOARD_DISTRICT_VALIDATE($element,&$form_state){
		$val			=	$form_state['values'];
		$district		=	$val['district'];
		if($district == 0 ) {
			form_error($element, t($element['#title'].' SHOULD BE VALID'));
	    	$form_state['rebuild'] = TRUE;
		}
	}
	
	function DASHBOARD_BLOCK_VALIDATE($element,&$form_state){
		$val			=	$form_state['values'];
		$block			=	$val['block_code'];
		if($block == 0 ) {
			form_error($element, t($element['#title'].' SHOULD BE VALID'));
	    	$form_state['rebuild'] = TRUE;
		}
	}
	
	function DASHBOARD_CARD_TYPE_VALIDATE($element,&$form_state){
		$val			=	$form_state['values'];
		$card_type		=	$val['card_type'];
		if($card_type == 0 ) {
			form_error($element, t($element['#title'].' SHOULD BE VALID'));
	    	$form_state['rebuild'] = TRUE;
		}
	}
	
	function DASHBOARD_OWNERSHIP_TYPE_VALIDATE($element,&$form_state){
		$val					=	$form_state['values'];
		$type_of_ownership		=	$val['type_of_ownership'];
		if($type_of_ownership == 0 ) {
			form_error($element, t($element['#title'].' SHOULD BE VALID'));
	    	$form_state['rebuild'] = TRUE;
		}
	}
	
	function DASHBOARD_ESTABLISHMENT_TYPE_VALIDATE($element,&$form_state){
		$val					=	$form_state['values'];
		$est_type				=	$val['est_type'];
		if($est_type == 0 ) {
			form_error($element, t($element['#title'].' SHOULD BE VALID'));
	    	$form_state['rebuild'] = TRUE;
		}
	}
		
	function DASHBOARD_SPECIAL_CHAR_TEXT_VALIDATE($element,&$form_state){
		$txtValue		=	trim($element['#value']); 
		$match_pattern 	= 	'/[A-Za-z0-9]+/';
		if(!preg_match($match_pattern, $txtValue)) {
			form_error($element, t($element['#title'].' SHOULD BE TEXT ONLY'));
		}
	}
	
	function DASHBOARD_TEXTBOX_TEXT_VALIDATE($element,&$form_state){
		$txtValue		=	trim($element['#value']); 
		$match_pattern 	= 	'/^[ A-Za-z ]+$/';
		if(!preg_match($match_pattern, $txtValue)) {
			form_error($element, t($element['#title'].' SHOULD BE TEXT ONLY'));
		}
	}
	
	function DASHBOARD_TEXTBOX_OPTIONALTEXT_VALIDATE($element,&$form_state){
		$txtValue		=	trim($element['#value']); 
		$match_pattern 	= 	'/^[ A-Za-z ]+$/';
		
		if(!empty($txtValue)){
			if(!preg_match($match_pattern, $txtValue)) {
				form_error($element, t($element['#title'].' SHOULD BE TEXT ONLY'));
			}
		}
	}
	
	function DASHBOARD_TEXTBOX_NUMERIC_VALIDATE($element,&$form_state){
		$fieldValue=trim($element['#value']); 
		$match_pattern = '/^[0-9]+$/i';
		if(!empty($fieldValue)){
			if(!preg_match($match_pattern, $fieldValue)) {
				form_error($element, t($element['#title'].' SHOULD BE NUMBER ONLY'));
			}
		}
	}
	
	function DASHBOARD_TEXTBOX_NUMERIC_VALIDATE_PIN($element,&$form_state){
		$val 		=	$form_state['values'];
		$pin 		=	$val['pin'];
		if(strlen($pin) > 6){
			form_error($element, t($element['#title'].' SHOULD NOT BE MORE THAN 6 DIGITS.'));
		}
	}
	
	function DASHBOARD_TEXTBOX_EMPTY_TEXT_VALIDATE($element, &$form_state){
		$match_pattern 	= 	'/^[ A-Za-z ]+$/';
		$val 			= 	$form_state['values'];
		$middle_name 	= 	$val['m_name'];
		
		if(!empty($middle_name)){
			if(!preg_match($match_pattern, $middle_name)) {
				form_error($element, t($element['#title'].' SHOULD BE TEXT ONLY'));
			}
		}
	}
	
	function DASHBOARD_TEXTBOX_CARDNUMBER_TEXT_VALIDATE($element, &$form_state){
		$match_pattern 	= 	'/^[ A-Za-z0-9 ]+$/';
		$val 			= 	$form_state['values'];
		$card_number	=	$val['card_number'];
		
		if(!empty($card_number)){
			if(!preg_match($match_pattern, $card_number)) {
				form_error($element, t($element['#title'].' SHOULD BE TEXT ONLY'));
			}
		}
	}
	
	function applicant_profile_form_validate($form, &$form_state){
		if(isset($form['dashboard_master']) && !empty($form['dashboard_master'])) {
			$form_field_array1 = $form['dashboard_master'];
			foreach($form_field_array1 as $element){
				if(is_array($element)){
					if(isset($element['#type']) and ($element['#type'] == 'textfield')){
						chkbadchar($element['#value'], $element['#title'], $element['#name']);
					}
					if(isset($element['#type']) and ($element['#type'] == 'textarea')){
						chkbadchar($element['#value'], $element['#title'], $element['#name']);
					}
				}
			}
		}
	}
	
	/*function applicant_profile_form_submit($form, &$form_state){
		global $base_root, $base_path, $user;
		
		$val 					=	$form_state['values']; 
		
		$user_id				=	$user->uid;
		
		$f_name					=	$val['f_name'];
		$m_name					=	$val['m_name'];
		$l_name					=	$val['l_name'];
		$id						=	$val['id'];
		$dob					=	$val['dob']['year']."-".$val['dob']['month']."-".$val['dob']['day'];
		$email					=	$val['email'];
		$mobile					=	$val['mobile'];
		$gender					=	$val['gender'];
		
		
		$address				=	$val['address'];
		$country				=	$val['country'];
		$state					=	$val['state'];
		$dist_code				=	$val['district'];
		$subdivision			=	$val['subdivision'];
		$areatype				=	$val['areatype'];
		$name_areatype			=	$val['block_code'];
		$vill_ward_code			= 	$val['panchyat'];
		$ps						=	$val['ps'] ;
		$pin					=	$val['pin'];
		
		
		
		$card_type				=	$val['card_type'];
		$card_number			=	$val['card_number'];
		$status					=	1;
		
		///////////////  Ownership /////////////////////
				
		$ownership						=	$val['ownership'];
		
		$other_ownership				=	$val['other_ownership'];
		
		$ownership_undertaking			=	$val['ownership_undertaking'];
		
		$other_government_name			=	isset($val['other_government_name'])?$val['other_government_name']:'';
		$other_private_name				=	isset($val['other_private_name'])?$val['other_private_name']:'';
		
		$local_bodies					=	isset($val['local_bodies'])?$val['local_bodies']:0;
		$urban_local_bodies				=	isset($val['urban_local_bodies'])?$val['urban_local_bodies']:0;
		
		$ownership_undertaking_text		=	isset($val['ownership_undertaking_text'])?$val['ownership_undertaking_text']:'';
		
		///////////////  Establishment Info /////////////////////
		
		$est_name						= $val['est_name'];
		$est_location					= $val['est_location'];
		$est_district					= $val['est_district'];
		$est_subdivision				= $val['est_subdivision'];
		$est_areatype					= $val['est_areatype'];
		$est_block						= $val['est_areatype_name'];
		$est_panchyat					= $val['est_vill_ward'];
		$est_policestation				= $val['est_policestation'];
		$est_pincode					= $val['est_pincode'];
		
		///////////////  Workemen Info /////////////////////
		
		$work_man_master_role_male		=	$val['work_man_master_role_male'];
		$work_man_master_role_female	=	$val['work_man_master_role_female'];
		$contractual_labour_male		=	$val['contractual_labour_male'];
		$contractual_labour_female		=	$val['contractual_labour_female'];
		$other_labour_male				=	$val['other_labour_male'];
		$other_labour_female			=	$val['other_labour_female'];
		
		
		/////////////////// End ///////////////////////
		
		if( isset($user_id) ){	
			
			if( $email != "" && $mobile != "" ){
				
						
				$fieldsCommonAppInfo = array( 													
													'user_id' 				=>  $user_id,
													'f_name'				=>	$f_name,
													'm_name'				=>	$m_name, 
													'l_name'				=>	$l_name, 
													'dob'					=>	$dob, 
													'email'					=>	$email, 
													'mobile'				=>	$mobile,  
													'gender'				=>	$gender, 
													
													'address'				=>	$address, 
													'country'				=> 	$country,
													'state'					=>	$state,
													'dist_code'				=>	$dist_code, 
													'subdivision'			=>	$subdivision, 
													'areatype'				=>	$areatype,
													'name_areatype'			=>	$name_areatype,
													'vill_ward'				=>	$vill_ward_code,
													'ps'					=>	$ps,
													'pin'					=>	$pin, 
													'card_type'				=>	$card_type, 
													'card_number'			=>	$card_number ,
													'status'				=>	$status,
													
													
												  	'est_name'				=> $est_name,
												  	'est_loc' 				=> $est_location,
												  	'est_district' 			=> $est_district,
												  	'est_subdivision'		=> $est_subdivision,
												  	'est_loc_e_areatype'	=> $est_areatype,
												  	'est_name_areatype' 	=> $est_block,
												  	'est_loc_e_vill_ward'	=> $est_panchyat,
												 	'est_ps'				=> $est_policestation,
												  	'est_pin_number' 		=> $est_pincode,
													
													
													'ownership'				=>	$ownership,
													'other_ownership'		=>	$ownership_undertaking_text,
													
													
													'ownership_undertaking'	=>	$ownership_undertaking,
													'other_government_name'	=>	$other_government_name,
													'other_private_name'	=>	$other_private_name,
													'local_bodies'			=>	$local_bodies,
													'urban_local_bodies'	=>	$urban_local_bodies,
													
													'work_man_master_role_male' 	=> $work_man_master_role_male,
													'work_man_master_role_female'	=> $work_man_master_role_female,
													'contractual_labour_male'		=> $contractual_labour_male,
													'contractual_labour_female'		=> $contractual_labour_female,
													'child_labour_male'				=> $other_labour_male,
													'child_labour_female'			=> $other_labour_female
													
										   );
					
					//die(print_r($fieldsCommonAppInfo));	ok();
										   
					if(empty($id)){						   
					$last_id	= db_insert('l_common_application_master')->fields($fieldsCommonAppInfo)->execute();
					//drupal_goto('applicant-profile-update');
					$message = 'Your profile has been successfully captured. Please select the desired services from left hand panel.';
					drupal_set_message($message);
  					drupal_goto('applicant-dashboard');
					 
				}else{
					$query = db_update('l_common_application_master');
					$query->fields($fieldsCommonAppInfo);              	
					$query->condition('id', $id);
					$query->execute();
					// $message =	'Profile Data Saved Successfully';
					// drupal_set_message(t($message));
					$message = 'Your profile has been successfully updated. Please select the desired services from left hand panel.';
					drupal_set_message($message);
					drupal_goto('applicant-dashboard');					
				}
			}
			
		}
	}*/
	
	function applicant_profile_form_submit($form, &$form_state){
		global $base_root, $base_path, $user;
		//echo $_SESSION['service_id']; exit;
		$val 					=	$form_state['values']; //print_r($val);die;
		
		$user_id				=	$user->uid;
		
		$f_name					=	$val['f_name'];
		$m_name					=	$val['m_name'];
		$l_name					=	$val['l_name'];
		$id						=	$val['id'];
		$dob					=	$val['dob']['year']."-".$val['dob']['month']."-".$val['dob']['day'];
		$email					=	$val['email'];
		$mobile					=	$val['mobile'];
		$gender					=	$val['gender'];
		
		
		$address				=	$val['address'];
		$country				=	$val['country'];
		$state					=	$val['state'];
		$dist_code				=	$val['district'];
		$subdivision			=	$val['subdivision'];
		$areatype				=	$val['areatype'];
		$name_areatype			=	$val['block_code'];
		$vill_ward_code			= 	$val['panchyat'];
		$ps						=	$val['ps'] ;
		$pin					=	$val['pin'];
		
		
		
		$card_type				=	$val['card_type'];
		$card_number			=	$val['card_number'];
		$status					=	1;
		
		///////////////  Ownership /////////////////////
				
		$ownership						=	$val['ownership'];
		
		$other_ownership				=	$val['other_ownership'];
		
		$ownership_undertaking			=	$val['ownership_undertaking'];
		
		$other_government_name			=	isset($val['other_government_name'])?$val['other_government_name']:'';
		$other_private_name				=	isset($val['other_private_name'])?$val['other_private_name']:'';
		
		$local_bodies					=	isset($val['local_bodies'])?$val['local_bodies']:0;
		$urban_local_bodies				=	isset($val['urban_local_bodies'])?$val['urban_local_bodies']:0;
		
		$ownership_undertaking_text		=	isset($val['ownership_undertaking_text'])?$val['ownership_undertaking_text']:'';
		
		///////////////  Establishment Info /////////////////////
		
		$est_name = $val['est_name'];
		$type_ofthe_est = $val['type_ofthe_est'];
		$est_location = $val['est_location'];
		$est_district = $val['est_district'];
		$est_subdivision = $val['est_subdivision'];
		$est_areatype = $val['est_areatype'];
		$est_block = $val['est_areatype_name'];
		$est_panchyat = $val['est_vill_ward'];
		$est_policestation = $val['est_policestation'];
		$est_pincode = $val['est_pincode'];
		
		///////////////  Workemen Info /////////////////////
		
		$work_man_master_role_male		=	$val['work_man_master_role_male'];
		$work_man_master_role_female	=	$val['work_man_master_role_female'];
		$master_role_adolescent_male	=   $val['master_role_adolescent_male'];
		$master_role_adolescent_female	=   $val['master_role_adolescent_female'];
		
		$contractual_labour_male		=	$val['contractual_labour_male'];
		$contractual_labour_female		=	$val['contractual_labour_female'];
		$contractual_labour_adolescent_male		= $val['contractual_labour_adolescent_male'];
		$contractual_labour_adolescent_female	= $val['contractual_labour_adolescent_female'];
		
		$other_labour_male				=	$val['other_labour_male'];
		$other_labour_female			=	$val['other_labour_female'];
		$other_labour_adolescent_male	=   $val['other_labour_adolescent_male'];
		$other_labour_adolescent_female	= 	$val['other_labour_adolescent_female'];
		
		
		/////////////////// End ///////////////////////
		
		if( isset($user_id) ){	
			
			if( $email != "" && $mobile != "" ){
					$fieldsCommonAppInfo = array( 													
							'user_id' 				=>  $user_id,
							'f_name'				=>	$f_name,
							'm_name'				=>	$m_name, 
							'l_name'				=>	$l_name, 
							'dob'					=>	$dob, 
							'email'					=>	$email, 
							'mobile'				=>	$mobile,  
							'gender'				=>	$gender, 
							
							'address'				=>	$address, 
							'country'				=> 	$country,
							'state'					=>	$state,
							'dist_code'				=>	trim($dist_code), 
							'subdivision'			=>	$subdivision, 
							'areatype'				=>	trim($areatype),
							'name_areatype'			=>	$name_areatype,
							'vill_ward'				=>	$vill_ward_code,
							'ps'					=>	trim($ps),
							'pin'					=>	$pin, 
							'card_type'				=>	$card_type, 
							'card_number'			=>	$card_number ,
							'status'				=>	$status,
							
							
							'est_name' => $est_name,
							'est_type' => $type_ofthe_est,
							'est_loc' => $est_location,
							'est_district' 			=> trim($est_district),
							'est_subdivision'		=> $est_subdivision,
							'est_loc_e_areatype'	=> trim($est_areatype),
							'est_name_areatype' 	=> $est_block,
							'est_loc_e_vill_ward'	=> $est_panchyat,
							'est_ps'				=> trim($est_policestation),
							'est_pin_number' 		=> $est_pincode,
							
							
							'ownership'				=>	$ownership,
							'other_ownership'		=>	$ownership_undertaking_text,
							
							
							'ownership_undertaking'	=>	$ownership_undertaking,
							'other_government_name'	=>	$other_government_name,
							'other_private_name'	=>	$other_private_name,
							'local_bodies'			=>	$local_bodies,
							'urban_local_bodies'	=>	$urban_local_bodies,
							
							'work_man_master_role_male' 	=> $work_man_master_role_male,
							'work_man_master_role_female'	=> $work_man_master_role_female,
							'work_man_master_role_adolescent_male '	=> $master_role_adolescent_male,
							'work_man_master_role_adolescent_female' => $master_role_adolescent_female,
							
							
							'contractual_labour_male'		=> $contractual_labour_male,
							'contractual_labour_female'		=> $contractual_labour_female,
							'contractual_labour_adolescent_male' => $contractual_labour_adolescent_male,
							'contractual_labour_adolescent_female' => $contractual_labour_adolescent_female,
							
							'child_labour_male'				=> $other_labour_male,
							'child_labour_female'			=> $other_labour_female,
							'child_labour_adolescent_male'	=> $other_labour_adolescent_male,
							'child_labour_adolescent_female' => $other_labour_adolescent_female,
							
							
							
							'pe_con_name' 			=> $val['pe_con_name'],
							'pe_con_gender' 		=> $val['pe_con_gender'],
							'pe_con_address' 		=> $val['pe_con_address'],
							'pe_con_country' 		=> $val['pe_con_country'],
							'pe_con_state' 			=> $val['pe_con_state'],
							'pe_con_district' 		=> $val['pe_con_district'],
							'pe_con_subdivision' 	=> $val['pe_con_subdivision'],
							'pe_con_areatype' 		=> $val['pe_con_areatype'],
							'pe_con_block_code' 	=> $val['pe_con_block_code'],
							'pe_con_gp' 			=> $val['pe_con_gp'],
							'pe_con_ps' 			=> $val['pe_con_ps'],
							'pe_con_pin' 			=> $val['pe_con_pin'],
							'pe_con_dob' 			=> $val['pe_con_dob']['year']."-".$val['pe_con_dob']['month']."-".$val['pe_con_dob']['day'],
							'pe_con_mobile' 		=> $val['pe_con_mobile'],
				   );
					
					//die(print_r($fieldsCommonAppInfo));	
										   
					if(empty($id)){						   
					$last_id	= db_insert('l_common_application_master')->fields($fieldsCommonAppInfo)->execute();
					//drupal_goto('applicant-profile-update');
					$message = 'Your profile has been successfully captured. Please select the desired services from left hand panel.';
					drupal_set_message($message);
					if($_SESSION['service_id'] == 1){
						$fetch_clra_det 	= db_query("select * from l_clra_registration_master where user_id=:userId",array(':userId'=>$user_id));
						//if( $fetch_clra_det->rowCount() > 0 ){
							$obj1 	= 	$fetch_clra_det->fetchAssoc();
							$id     =   $obj1['id'];
							if($id != ""){
								drupal_goto('applicant-dashboard');
							}else{
								drupal_goto('applicant-profile/clra-registration/new');
							}
						//}
					}elseif($_SESSION['service_id'] == 2){
						$fetch_bocwa_det 	= db_query("select * from l_bocwa_registration_master where user_id=:userId",array(':userId'=>$user_id));
						//if( $fetch_bocwa_det->rowCount() > 0 ){
							$obj1 	= 	$fetch_bocwa_det->fetchAssoc();
							$id     =   $obj1['id'];
							if($id != ""){
								drupal_goto('applicant-dashboard');
							}else{
								drupal_goto('new-bocwa-application');
							}
						//}
					}elseif($_SESSION['service_id'] == 3){
						$fetch_mtw_det 	= db_query("select * from l_mtw_registration_master where user_id=:userId",array(':userId'=>$user_id));
						//if( $fetch_bocwa_det->rowCount() > 0 ){
							$obj1 	= 	$fetch_mtw_det->fetchAssoc();
							$id     =   $obj1['id'];
							if($id != ""){
								drupal_goto('applicant-dashboard');
							}else{
								drupal_goto('mtw-registration');
							}
						//}
					}elseif($_SESSION['service_id'] == 4){
						$fetch_ismw_det 	= db_query("select * from l_interstate_workman_master where user_id=:userId",array(':userId'=>$user_id));
						//if( $fetch_bocwa_det->rowCount() > 0 ){
							$obj1 	= 	$fetch_ismw_det->fetchAssoc();
							$id     =   $obj1['id'];
							if($id != ""){
								drupal_goto('applicant-dashboard');
							}else{
								drupal_goto('ismw_application');
							}
						//}
					}elseif($_SESSION['service_id'] == 12){
						$fetch_license_query = db_query("select count(id) as total_cl from l_particular_of_contract_labour where contractor_user_id=:userId", array(':userId' => $user_id));
						$fetch_license_result = $fetch_license_query->fetchAssoc();
							
						if($fetch_license_result['total_cl'] > 0){
							drupal_goto('applicant/license-renewal-amendment-list');
						}else{
							drupal_goto('check_fromv_no');
						}
					}else{			
  						drupal_goto('applicant-dashboard');
					}
				// }elseif($_SESSION['service_id'] == 12){  // licence
					// $fetch_license_query = db_query("select count(id) as total_cl from l_particular_of_contract_labour where contractor_user_id=:userId", array(':userId' => $user_id));
					// $fetch_license_result = $fetch_license_query->fetchAssoc();
						
					// if($fetch_license_result['total_cl'] > 0){
					// 	drupal_goto('applicant/license-renewal-amendment-list');
					// }else{
					// 	drupal_goto('check_fromv_no');
					// }
						 
				}else{
					$query = db_update('l_common_application_master');
					$query->fields($fieldsCommonAppInfo);              	
					$query->condition('id', $id);
					$query->execute();
					// $message =	'Profile Data Saved Successfully';
					// drupal_set_message(t($message));
					
					$query = db_update('users');
					$query->fields(array('mail' => $email));              	
					$query->condition('uid', $user_id);
					$query->execute();
					
					if($_SESSION['service_id'] == 1){
						//echo "hii"; exit;
						$fetch_clra_det 	= db_query("select * from l_clra_registration_master where user_id=:userId",array(':userId'=>$user_id));
						//if( $fetch_clra_det->rowCount() > 0 ){
							$obj1 	= 	$fetch_clra_det->fetchAssoc();
							$id     =   $obj1['id'];
							//echo "jjjiokojk-".$id; exit;
							if($id != ""){
								drupal_goto('applicant-dashboard');
							}else{
								drupal_goto('applicant-profile/clra-registration/new');
							}
						//}
					}elseif($_SESSION['service_id'] == 2){
						//echo "hii123"; exit;
						$fetch_bocwa_det 	= db_query("select * from l_bocwa_registration_master where user_id=:userId",array(':userId'=>$user_id));
						//if( $fetch_bocwa_det->rowCount() > 0 ){
							$obj1 	= 	$fetch_bocwa_det->fetchAssoc();
							$id     =   $obj1['id'];
							//echo "jjjiokojk123-".$id; exit;
							if($id != ""){
								drupal_goto('applicant-dashboard');
							}else{
								drupal_goto('new-bocwa-application');
							}
						//}
					}elseif($_SESSION['service_id'] == 3){
						$fetch_mtw_det 	= db_query("select * from l_mtw_registration_master where user_id=:userId",array(':userId'=>$user_id));
						//if( $fetch_bocwa_det->rowCount() > 0 ){
							$obj1 	= 	$fetch_mtw_det->fetchAssoc();
							$id     =   $obj1['id'];
							if($id != ""){
								drupal_goto('applicant-dashboard');
							}else{
								drupal_goto('mtw-registration');
							}
						//}
					}elseif($_SESSION['service_id'] == 4){
						$fetch_ismw_det 	= db_query("select * from l_interstate_workman_master where user_id=:userId",array(':userId'=>$user_id));
						//if( $fetch_bocwa_det->rowCount() > 0 ){
							$obj1 	= 	$fetch_ismw_det->fetchAssoc();
							$id     =   $obj1['id'];
							if($id != ""){
								drupal_goto('applicant-dashboard');
							}else{
								drupal_goto('ismw_application');
							}
						//}
					}else if($_SESSION['service_id'] == 12){
						$fetch_license_query = db_query("select count(id) as total_cl from l_particular_of_contract_labour where contractor_user_id=:userId", array(':userId' => $user_id));
						$fetch_license_result = $fetch_license_query->fetchAssoc();
							
						if($fetch_license_result['total_cl'] > 0){
							drupal_goto('applicant/license-renewal-amendment-list');
						}else{
							drupal_goto('check_fromv_no');
						}
					}else{
						// $message = 'Your profile has been successfully updated. Please select the desired services from left hand panel.';
						// drupal_set_message($message);
  						if($val['refer_url'] == 'sc'){
							drupal_goto('self-certification-application/particulars');
						}else{
							drupal_goto('applicant-dashboard');
						}
					}					
				}
			}
		}
	}
	
	function _load_district(){
		$district = array('- Select district -');
  		$query = db_select("district", "a");
  		$query->fields("a", array('district_id', 'dist_name'));
  		$result = $query->execute();
  		while($row = $result->fetchObject()){
    		$district[$row->district_id] = $row->dist_name;
  		}
  		return $district;
	}
	
	function _load_block($district_id){
		$block = array('- Select block -');
  		$query = db_select("dis_block", "b");
  		$query->fields("b", array('block_id', 'block_name'));
  		$query->condition("b.district_id", $district_id);
  		$result = $query->execute();
  		while($row = $result->fetchObject()){
    		$block[$row->block_id] = $row->block_name;
  		}
  		return $block;
	}
	
	function sandbox_ajax_dropdown_block($form, $form_state) {
		$commands	=	array();
		$commands[] = ajax_command_replace('#block-wrapper', drupal_render($form['dashboard_master']['block_code']));
		return array('#type' => 'ajax', '#commands' => $commands);
	}
	
	function applicant_forms_format_dob($form_element, &$form_state){
		unset($form_element['year']['#options']);
		$max_age = date('Y') - 100; // 100 years ago
		$min_age = date('Y');
		$form_element['year']['#options'] = array();
		foreach (range($max_age, $min_age) as $year){
			$form_element['year']['#options'][$year] = $year;
		}
		return $form_element;
	}
	
	function applicant_dob_validate($element, &$form_state){
		$employer_dob_value = $element['#value'];
		$employer_dob_timestamp = strtotime(trim($employer_dob_value['year']).'-'.trim($employer_dob_value['month']).'-'.trim($employer_dob_value['day']));
		$todays_date= strtotime(date("Y-m-d")) ; 
		if($employer_dob_timestamp >= $todays_date){
			form_error($element, t($element['#title'].' is not valid . '));
		}
	}
	
	function APPLICANT_PROFILE_AREATYPE_VALIDATE($element, &$form_state){
		$val				=	$form_state['values'];
		$applicant_prof_val	=	$val['areatype'];
		if( $applicant_prof_val == 'T' ){
			form_error($element, t($element['#title'].' cannot be empty.'));
	    	$form_state['rebuild'] = TRUE;
		}
	}
	
	function numeric_validation($element,&$form_state){
		$fieldValue=trim($element['#value']); 
		$match_pattern = '/^[0-9]+$/i';
		
		if(!empty($fieldValue)){
			if(!preg_match($match_pattern, $fieldValue)) {
				form_error($element, t($element['#title'].' SHOULD BE NUMBER ONLY'));
			}
			if(strlen($fieldValue)!=6){
				form_error($element, t($element['#title'].' SHOULD BE 6 DIGIT'));
			}
		}
	}
	
	function fetch_ownership_data(){
		$default_select 	= array(""=>"- Select Ownership -");
		$result 			= db_query("SELECT id, ownership_name FROM {ownership} where status = '1' order by id");
		$list 				= array();
		foreach ($result as $record) {
			$list[$record->id] = t('@ownership_name', array(
				'@ownership_name' => $record->ownership_name,
			));
		}
		return ($default_select+$list);
	}
	
	function government_ownership_list($ownership){
		$default_govtselect 	= array(""=>"- Select Government Ownerships -");
		$all_govt 				= array();
		$query_govt 			= db_select('ownership', 'ownshp');
		$query_govt				->fields('ownshp',array('id','ownership_name'));
		$query_govt				->orderBy('ownshp.id', 'ASC');
		$query_govt				->condition('ownshp.status','2');
		$query_govt				->condition('ownshp.parent_id', $ownership);
		$result_govt 			= $query_govt->execute(); 
		if(!empty($result_govt)) {      
			foreach($result_govt as $rows) {
				$all_govt[$rows->id] = t('@ownership_name', array('@ownership_name' => $rows->ownership_name));
			}    
		}
		if (isset($all_govt)) {			
			return ($default_govtselect + $all_govt);	
		} else {				
			return $default_govtselect;
		}
	}
	
	function private_ownership_list($ownership){
		$default_pvtselect 		= array(""=>"- Select Private Ownerships -");
		$all_pvt 				= array();
		$query_pvt 				= db_select('ownership', 'ownshp');
		$query_pvt				->fields('ownshp',array('id','ownership_name'));
		$query_pvt				->orderBy('ownshp.id', 'ASC');
		$query_pvt				->condition('ownshp.status','3');
		$query_pvt				->condition('ownshp.parent_id', $ownership);
		$result_pvt 			= $query_pvt->execute(); 
		if(!empty($result_pvt)) {      
			foreach($result_pvt as $rows) {
				$all_pvt[$rows->id] = t('@ownership_name', array('@ownership_name' => $rows->ownership_name));
			}    
		}
		if (isset($all_pvt)) {			
			return ($default_pvtselect + $all_pvt);	
		} else {				
			return $default_pvtselect;
		}
	}
	
	function local_bodies_ownership_list($ownership_undertaking){
		$default_lbselect 		= array(""=>"- Select Local Ownerships -");
		$all_lb 				= array();
		$query_lb 				= db_select('ownership', 'ownshp');
		$query_lb				->fields('ownshp',array('id','ownership_name'));
		$query_lb				->orderBy('ownshp.id', 'ASC');
		$query_lb				->condition('ownshp.status', '4');
		$query_lb				->condition('ownshp.parent_id', $ownership_undertaking);
		$result_lb 				= $query_lb->execute(); 
		if(!empty($result_lb)) {      
			foreach($result_lb as $rows) {
				$all_lb[$rows->id] = t('@ownership_name', array('@ownership_name' => $rows->ownership_name));
			}    
		}
		if (isset($all_lb)) {			
			return ($default_lbselect + $all_lb);	
		} else {				
			return $default_lbselect;
		}
	}
	
	function urban_local_bodies_ownership_list($local_bodies){
		$default_ulbselect 		= array(""=>"- Select Urban Ownerships -");
		$all_ulb 				= array();
		$query_ulb 				= db_select('ownership', 'ownshp');
		$query_ulb				->fields('ownshp',array('id','ownership_name'));
		$query_ulb				->orderBy('ownshp.id', 'ASC');
		$query_ulb				->condition('ownshp.status', '5');
		$query_ulb				->condition('ownshp.parent_id', $local_bodies);
		$result_ulb 			= $query_ulb->execute(); 
		if(!empty($result_ulb)) {      
			foreach($result_ulb as $rows) {
				$all_ulb[$rows->id] = t('@ownership_name', array('@ownership_name' => $rows->ownership_name));
			}    
		}
		if (isset($all_ulb)) {			
			return ($default_ulbselect + $all_ulb);	
		} else {				
			return $default_ulbselect;
		}
	}
	
	function get_ajax_ownership($form, $form_state){
		$commands = array();
		$commands[] = ajax_command_replace('#ownership_replace_1', drupal_render($form['dashboard_master']['ownership_undertaking']));
		
		
		$commands[] = ajax_command_replace('#ownership_replace_2', drupal_render($form['dashboard_master']['other_government_name']));
		$commands[] = ajax_command_replace('#ownership_replace_3', drupal_render($form['dashboard_master']['local_bodies']));
		$commands[] = ajax_command_replace('#ownership_replace_4', drupal_render($form['dashboard_master']['urban_local_bodies']));
		$commands[] = ajax_command_replace('#ownership_replace_5', drupal_render($form['dashboard_master']['other_ownership']));
		$commands[] = ajax_command_replace('#ownership_replace_6', drupal_render($form['dashboard_master']['other_private_name']));
		
		$commands[] = ajax_command_replace('#ownership_replace_text', drupal_render($form['dashboard_master']['ownership_undertaking_text']));
		
		return array('#type' => 'ajax', '#commands' => $commands);	
	}
	
	function get_estb_registration_info_form_ajax_callback($form, $form_state) {
		$commands = array();
		$commands[] = ajax_command_replace('#estb_subdvsn_replace', drupal_render($form['dashboard_master']['est_subdivision']));
		$commands[] = ajax_command_replace('#estb_loc_e_areatype', drupal_render($form['dashboard_master']['est_areatype']));
		$commands[] = ajax_command_replace('#estb_common_1_replace', drupal_render($form['dashboard_master']['est_areatype_name']));
		$commands[] = ajax_command_replace('#estb_common_2_replace', drupal_render($form['dashboard_master']['est_vill_ward']));
		$commands[] = ajax_command_replace('#estb_ps_replace', drupal_render($form['dashboard_master']['est_policestation']));
		return array('#type' => 'ajax', '#commands' => $commands);
	}
	
	
	function get_info_form_ajax_callback($form, $form_state){
		$data 	= array();
		$data[] = ajax_command_replace('#country_others_div', drupal_render($form['dashboard_master']['state']));
		$data[]	= ajax_command_replace('#distric_div', drupal_render($form['dashboard_master']['district']));
		$data[]	= ajax_command_replace('#subdivisn_div', drupal_render($form['dashboard_master']['subdivision']));
		$data[]	= ajax_command_replace('#areatype_div', drupal_render($form['dashboard_master']['areatype']));
		$data[]	= ajax_command_replace('#block_mun_div', drupal_render($form['dashboard_master']['block_code']));
		$data[]	= ajax_command_replace('#gp_ward_div', drupal_render($form['dashboard_master']['panchyat']));
		$data[]	= ajax_command_replace('#ps_div', drupal_render($form['dashboard_master']['ps']));
		$data[]	= ajax_command_replace('#pincode_div', drupal_render($form['dashboard_master']['pin']));
		return array('#type' => 'ajax', '#commands' => $data);
	}
	
	function get_all_state1($value_country=''){
		$default_select 	= array(""=>"- Select State -");
		$result 			= db_query("SELECT id,statename FROM {state_master} where country_id = $value_country order by statename");
		$list 				= array();
		foreach ($result as $record) {
			$list[$record->id] = t('@districtname', array(
				'@districtname' => $record->statename,
			));
		}
		return ($default_select+$list);
	}
	function get_discrict_by_id($value_state=''){
		$default_select 	= array(""=>"- Select District -");
		$result 			= db_query("SELECT district_code,district_name FROM {district_master} where is_active = 'Y' and state_id = $value_state order by district_name");
		$list 				= array();
		foreach ($result as $record) {
			$list[$record->district_code] = t('@districtname', array(
				'@districtname' => $record->district_name,
			));
		}
		return ($default_select+$list);
	}
	function REGISTRATION__TEXTBOX_NUMERIC_VALIDATE($element,&$form_state){
		$fieldValue=trim($element['#value']); 
		$match_pattern = '/^[0-9]+$/i';
		
		if(!empty($fieldValue)){
			if(!preg_match($match_pattern, $fieldValue)) {
				form_error($element, t($element['#title'].' should be number only'));
			}
			if(strlen($fieldValue)!=6){
				form_error($element, t($element['#title'].' should be 6 digit'));
			}
		}
	}
	
	function get_pe_con_ajax_callback($form, $form_state){
		$data 	= array();
        
        
        
		$data[]	= ajax_command_replace('#pe_con_state_replace', drupal_render($form['dashboard_master']['pe_con_state']));
        
		$data[]	= ajax_command_replace('#pe_con_district_replace', drupal_render($form['dashboard_master']['pe_con_district']));
		
		$data[]	= ajax_command_replace('#pe_con_subdivision_replace', drupal_render($form['dashboard_master']['pe_con_subdivision']));
        
		$data[]	= ajax_command_replace('#pe_con_areatype_replace', drupal_render($form['dashboard_master']['pe_con_areatype']));
        
		$data[]	= ajax_command_replace('#pe_con_block_code_replace', drupal_render($form['dashboard_master']['pe_con_block_code']));
        
		$data[]	= ajax_command_replace('#pe_con_gp_replace', drupal_render($form['dashboard_master']['pe_con_gp']));
        
		$data[]	= ajax_command_replace('#pe_con_ps_replace', drupal_render($form['dashboard_master']['pe_con_ps']));
        
		$data[]	= ajax_command_replace('#pe_con_pin_replace', drupal_render($form['dashboard_master']['pe_con_pin']));
        
		return array('#type' => 'ajax', '#commands' => $data);
	}
	
	/////////////////////////////////////////////////////////////////////////  End Of Functions For Ownership  //////////////////////////////////////////////////////////////////
	