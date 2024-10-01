<?php
	
	drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
	drupal_add_css(drupal_get_path('module', 'applicant_registration') . '/css/reg_css.css');
	drupal_add_js(drupal_get_path('module', 'applicant_registration').'/js/reg_script.js');
	drupal_add_css(drupal_get_path('module', 'applicant_registration').'/css/popup.css');

	function applicant_registration_form($form, &$form_state) {

		global $base_root, $base_path;

		$form['app_reg']['loginLink'] 					=	array(
																'#prefix'			=> '<div class="content"><div class="sky-form"><div class="row">',
																'#type'     		=> 'markup',
																'#markup'			=> l('CLICK HERE TO LOGIN', 'applicant-login', array('attributes' => array('html' => TRUE, 'class' => 'button right registration'))),
																'#suffix'			=> '</div>',
															);		
											
											
		$form['app_reg']['f_name']						= 	array(
																'#prefix'			=> '<fieldset><legend>Personal Information</legend><div class="row"><section class="col col-4"><label class="input">',
																'#suffix'			=> '</label></section>',
																'#title'			=>  'First Name',
																'#type' 			=>  'textfield',
																'#attributes' 		=>  array('placeholder' => 'Enter your First Name'),
																//'#required' 		=>  TRUE,
																'#element_validate'	=>  array('DASHBOARD_TEXTBOX_TEXT_VALIDATE')
															);
															
															
								
		$form['app_reg']['m_name'] 						= 	array(
																'#prefix'			=> '<section class="col col-4"><label class="input">',
																'#suffix'			=>'</label></section>',
																'#title'			=> 'Middle Name',
																'#type' 			=> 'textfield',
																'#attributes' 		=> array('placeholder' => 'Enter your Middle Name'),
																//'#element_validate'	=> array('DASHBOARD_TEXTBOX_CARDNUMBER_TEXT_VALIDATE')
															);
															
															
								
		$form['app_reg']['l_name'] 						= 	array(
																'#prefix'			=> '<section class="col col-4"><label class="input">',
																'#suffix'			=> '</label></section></div>',
																'#title'			=> 'Last Name',
																'#type' 			=> 'textfield',
																'#attributes' 		=> array('placeholder' => 'Enter your Last Name'),
																//'#required' 		=> TRUE,
																'#element_validate'	=> array('DASHBOARD_TEXTBOX_TEXT_VALIDATE')
														 );
														 
														 
														 
		$form['app_reg']['dob'] 						=	array(
																'#prefix'			=> '<div class="row"><section class="col col-6"><label class="select">',
																'#suffix'			=> '</label></section>',
																'#type' 			=> 'date',
																'#title'			=> 'Date Of Birth',
																'#required' 		=> TRUE,
																'#after_build' 		=> array("applicant_forms_format_dob"),
																// '#element_validate' => array('applicant_dob_validate')
															);
															
		
		$genderOptions 									=	array('0'=>t('Male'), '1'=>t('Female'), '2'=>t('Transgender'));
		
		$form['app_reg']['gender'] 						= 	array(
																'#prefix'			=> '<section class="col col-6"><label class="inline-group">',
																'#suffix'			=>'</label></section></div>',
																'#type' 			=> 'radios',
																'#title' 			=> t('Gender'),
																'#options' 			=> $genderOptions,
																'#required' 		=> TRUE
															);
															
															
															
		$cardTypeOptions 								= 	array( ""=>'Select Card Type', 1=>'AADHAR', 2=>'PAN', 3=>'TAN', 4=>'LIN', 5=>'EPIC' );
		$value_dropdown_cardType						=	!empty($card_type) ? $card_type : key($cardTypeOptions);
		
		$form['app_reg']['card_type'] 					= 	array(
																'#prefix'			=> '<div class="row"><section class="col col-6"><label class="select">',
																'#suffix'			=> '<i></i></label></section>',
																'#title'			=> 'ID Card Type',
																'#type' 			=> 'select',
																'#options' 			=> $cardTypeOptions,
																//'#required' 		=> TRUE,
																'#element_validate'	=> array('DASHBOARD_CARD_TYPE_VALIDATE')
															);
															
															
		$form['app_reg']['card_number'] 				= 	array(
																'#prefix'			=> '<section class="col col-6"><label class="input">',
																'#suffix'			=> '</label></section></div></fieldset>',
																'#title'			=> 'ID Card Number',
																'#type' 			=> 'textfield',
																'#attributes' 		=> array('placeholder' => 'Enter your Id Card Number', 'id' => 'card_number_id', 'maxlength' => 20),
																//'#required' 		=> TRUE,
																'#element_validate' => array('DASHBOARD_SPECIAL_CHAR_TEXT_VALIDATE')
															);
		
																
		$form['app_reg']['email'] 						=	array(
																'#prefix'			=> '<fieldset><legend>Contact Information</legend><div class="row"><section class="col col-4"><label class="input">',
																'#suffix'			=> '</label>',
																'#type'				=> 'textfield',
																'#title'			=> t('Email'),
																//'#required'		=> TRUE,
																'#default_value'	=> (isset($form_state['values']['email'])) ? $form_state['values']['email'] : '',
																'#attributes' 		=> array( 'placeholder'=>'Enter valid email address', 'maxlength' => 255, 'onkeyup' => 'checkEmail(this.val);', 'onblur' => 'checkEmail(this.val);', 'class' => array('email_fld'), 'id' => 'email_id' ),
																'#element_validate' => array('DASHBOARD_EMAIL_VALIDATE')
															);
															
		$form['app_reg']['email_img_markup'] 		= array(
																'#type' 			=> 'markup', 
																'#markup' 			=> '<span id="common_email_img_div"></span>'
														   );
															
		
															
		$form['app_reg']['email_markup'] 			= array(
																'#type' 			=> 'markup', 
																'#markup' 			=> '<span id="common_email_div"></span></section>'
														   );
															
															
		$form['app_reg']['phone']						=	array(
																'#prefix'			=> '<section class="col col-4"><label class="input">',
																'#suffix'			=> '</label></section>',
																'#type'				=> 'textfield',
																'#title'			=> t('Mobile Number'),
																//'#required'			=> TRUE,
																'#attributes' 		=> array( 'id' => 'phoneId', 'placeholder'=>'Enter 10 digits mobile number', 'maxlength' => 10, 'accept' => 'number'),
																'#element_validate' => array('DASHBOARD_PHONE_VALIDATE')
															);
															
		$form['app_reg']['phone_markup'] 				= array(
																'#type' 			=> 'markup', 
																'#markup' 			=> '<span id="hint_phone"><img src="'.$base_root.$base_path.'sites/all/modules/applicant_registration/images/tooltip.jpg"></span>'
														   );
		
		$form['app_reg']['contact_number']				=	array(
																'#prefix'			=> '<section class="col col-4"><label class="input">',
																'#suffix'			=> '</label></section></div>',
																'#type'				=> 'textfield',
																'#title'			=> t('Other Contact Number'),
																'#attributes' 		=> array( 'id' => 'phoneId', 'placeholder'=>'Enter any others phone number', 'maxlength' => 12, 'accept' => 'number'),
																'#element_validate' => array('DASHBOARD_CONTACT_NUMBER_VALIDATE')
															);
							
		$form['app_reg']['address'] 					= array(
															'#prefix'				=> '<section><label class="textarea">',
															'#suffix'				=> '</label></section>',
															'#title'  				=> 'Address Line 1',
															'#type'   				=> 'textarea',
															'#attributes' 			=> array('placeholder' => 'Enter your Address'),
															'#rows' 				=> 2,
															//'#required' 			=> TRUE
															'#element_validate'		=> array('LOCATION_VALIDATE')
														);
		
		$options_dist 									= common_custom_user_district_list_new();
		
		$form['app_reg']['dist_code']   				=  array(
																'#prefix'			=> '<div class="row"><section class="col col-6"><label class="select">',
																'#suffix'			=> '<i></i></label></section>',
																'#title'			=> 'Select District',
																'#type' 			=> 'select',
																'#options' 			=> $options_dist,
																//'#required' 		=> TRUE,
																'#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
																'#ajax'				=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_ajax_callback_new_1',
																						'progress'			=> '',
																					 )
															);
					
		$value_loc_e_dist = isset($form_state['values']['dist_code']) ? $form_state['values']['dist_code'] : '';
		
			
		if($value_loc_e_dist != "" || !empty($value_loc_e_dist) || is_numeric($value_loc_e_dist)){				
				
			$form['app_reg']['subdivision']  			= array(
															  '#prefix'				=> '<div id="clra_subdvsn_replace"><section class="col col-6"><label class="select">',
															  '#suffix'				=> '<i></i></label></section></div>',
															  '#title'				=> 'Select Subdivision',
															  '#type' 				=> 'select',
															  '#options' 			=> custom_user_subdivisionlist_list_new($value_loc_e_dist),
															  //'#required' 			=> TRUE,
															  '#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
															  '#ajax'				=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_ajax_callback_new_1',
																						'progress'			=> '',
																					  ),
														);
																										
		}else{
				
			$form['app_reg']['subdivision'] 			= array(
															  '#prefix'				=> '<div id="clra_subdvsn_replace"><section class="col col-6"><label class="select">',
															  '#suffix'				=> '<i></i></label></section></div>',
															  '#title'				=> 'Select Subdivision',
															  '#type' 				=> 'select',
															  '#options' 			=> array( '' => '--Select--' ),
															  //'#required' 			=> TRUE,
															  '#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
															  '#ajax'				=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_ajax_callback_new_1',
																						'progress'			=> '',
																					  ),
														);
			}
			
		$value_loc_e_subdivision 						= isset($form_state['values']['subdivision']) ? $form_state['values']['subdivision'] : '';
			
		if($value_loc_e_dist != "" && $value_loc_e_subdivision != ""){
				
			$areatypeOptions							 = custom_user_areatype_list_new($value_loc_e_dist, $value_loc_e_subdivision);
			$areaTypeData								 = array( '' => '--Select--' );
			
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
				
			$form['app_reg']['areatype']  				= array(
															 '#prefix'				=> '<div id="clra_loc_e_areatype"><section class="col col-6"><label class="select">',
															 '#suffix'				=>'<i></i></label></section></div>',
															  '#title'				=> 'Select Areatype',
															  '#type' 				=> 'select',
															  '#options' 			=> $areaTypeData,
															  //'#required' 			=> TRUE,
															  '#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
															  //'#id'					=> 'loc_e_areatype_id',
															  '#ajax'				=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_ajax_callback_new_1',
																						'progress'			=> '',
																					  ),
																);
																												
			}else{
				
				$form['app_reg']['areatype'] 			= array(
															  '#prefix'				=> '<div id="clra_loc_e_areatype"><section class="col col-6"><label class="select">',
															  '#suffix'				=>'<i></i></label></section></div>',
															  '#title'				=> 'Select Areatype',
															  '#type' 				=> 'select',
															  '#options' 			=> array( '' => '--Select--' ),
															  //'#required' 			=> TRUE,
															  '#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
															  '#ajax'				=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_ajax_callback_new_1',
																						'progress'			=> '',
																					  ),
																);
			}
			
			$value_loc_e_areatype 						= isset($form_state['values']['areatype']) ? $form_state['values']['areatype'] : '';
			
			$form['app_reg']['name_areatype'] 			= array(
															 '#prefix'				=> '<div id="clra_common_1_replace"><section class="col col-6"><label class="select">',
															 '#suffix'				=>'<i></i></label></section></div>',
															 '#title'				=> 'Select Block/Municipality/Corporation/SEZ/Notified Area',
															 '#type' 				=> 'select',
															 '#options' 			=> array( '' => '--Select--' ),
															 //'#required' 			=> TRUE,
															 '#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
															 '#ajax'				=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_ajax_callback_new_1',
																						'progress'			=> '',
																					  )
														  );
			
			if($value_loc_e_dist != "" && $value_loc_e_subdivision != ""){
				if($value_loc_e_areatype != ""){
					if($value_loc_e_areatype == 'B'){
						$form['app_reg']['name_areatype'] = array(
															  '#prefix'				=> '<div id="clra_common_1_replace"><section class="col col-6"><label class="select">',
															  '#suffix'				=> '<i></i></label></section></div>',
															  '#title'				=> 'Select Block',
															  '#type' 				=> 'select',
															  '#options' 			=> custom_user_block_list_new($value_loc_e_subdivision, $value_loc_e_areatype),
															  //'#required' 			=> TRUE,
															  '#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
															  //'#id'					=> 'common_selection_1_id',
															  '#ajax'				=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_ajax_callback_new_1',
																						'progress'			=> '',
																					  )
															);
					}else if($value_loc_e_areatype == 'M'){
					
						$form['app_reg']['name_areatype'] = array(
															  '#prefix'				=> '<div id="clra_common_1_replace"><section class="col col-6"><label class="select">',
															  '#suffix'				=> '<i></i></label></section></div>',
															  '#title'				=> 'Select Municipality',
															  '#type' 				=> 'select',
															  '#options' 			=> custom_user_block_list_new($value_loc_e_subdivision, $value_loc_e_areatype),
															  //'#required' 			=> TRUE,
															  '#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
															  //'#id'					=> 'common_selection_1_id',
															  '#ajax'				=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_ajax_callback_new_1',
																						'progress'			=> '',
																					  )
															);
					}else if($value_loc_e_areatype == 'C'){
			
						$form['app_reg']['name_areatype'] = array(
															  '#prefix'				=> '<div id="clra_common_1_replace"><section class="col col-6"><label class="select">',
															  '#suffix'				=> '<i></i></label></section></div>',
															  '#title'				=> 'Select Corporation',
															  '#type' 				=> 'select',
															  '#options' 			=> custom_user_block_list_new($value_loc_e_subdivision,$value_loc_e_areatype),
															  //'#required' 			=> TRUE,
															  '#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
															  //'#id'					=> 'common_selection_1_id',
															  '#ajax'				=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_ajax_callback_new_1',
																						'progress'			=> '',
																					  )
														);
					}else if($value_loc_e_areatype == 'S'){
			
						$form['app_reg']['name_areatype'] = array(
															 '#prefix'				=> '<div id="clra_common_1_replace"><section class="col col-6"><label class="select">',
															 '#suffix'				=> '<i></i></label></section></div>',
															  '#title'				=> 'Select SEZ',
															  '#type' 				=> 'select',
															  '#options' 			=> custom_user_block_list_new($value_loc_e_subdivision, $value_loc_e_areatype),
															  //'#required' 			=> TRUE,
															  '#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
															  //'#id'					=> 'common_selection_1_id',
															  '#ajax'				=> array(
																						'event' 			=> 'change',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_ajax_callback_new_1',
																						'progress'			=> '',
																					  )
															);
					}else if($value_loc_e_areatype == 'N'){
					
						$form['app_reg']['name_areatype'] = array(
															  '#prefix'				=> '<div id="clra_common_1_replace"><section class="col col-6"><label class="select">',
															  '#suffix'				=> '<i></i></label></section></div>',
															  '#title'				=> 'Select Notified Area',
															  '#type' 				=> 'select',
															  '#options' 			=> custom_user_block_list_new($value_loc_e_subdivision, $value_loc_e_areatype),
															  //'#required' 			=> TRUE,
															  '#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
															  //'#id'					=> 'common_selection_1_id',
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
			
			$value_common_selection_1 					    = isset($form_state['values']['name_areatype']) ? $form_state['values']['name_areatype'] : '';
			
			$form['app_reg']['vill_ward'] 					= array(
																 '#prefix'				=> '<div id="clra_common_2_replace"><section class="col col-4"><label class="select">',
																 '#suffix'				=> '<i></i></label></section></div>',
																 '#title'				=> 'Select Gram Panchayat/Ward/Sector',
																 '#type' 				=> 'select',
																 '#options' 			=> array( '' => '--Select--' ),
																 //'#required' 			=> TRUE,
																 '#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
																 //'#id'					=> 'common_selection_2_id',
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
								
							$form['app_reg']['vill_ward'] 	= array(
																  '#prefix'				=> '<div id="clra_common_2_replace"><section class="col col-4"><label class="select">',
																  '#suffix'				=> '<i></i></label></section></div>',
																  '#title'				=> 'Select Gram Panchayat',
																  '#type' 				=> 'select',
																  '#options'			=> custom_user_villward_list_new($value_common_selection_1,'V'),
																  //'#required' 			=> TRUE,
																  '#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
																  //'#id'					=> 'common_selection_2_id',
																  '#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_ajax_callback_new_1',
																								'progress'			=> '',
																							  )
																);	
						}
							
						if($value_loc_e_areatype == 'M'){
								
								$form['app_reg']['vill_ward'] = array(
																  '#prefix'				=> '<div id="clra_common_2_replace"><section class="col col-4"><label class="select">',
																  '#suffix'				=> '<i></i></label></section></div>',
																  '#title'				=> 'Select Ward',
																  '#type' 				=> 'select',
																  '#options' 			=> custom_user_villward_list_new($value_common_selection_1,'W'),
																  //'#required' 			=> TRUE,
																  '#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
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
								
								$form['app_reg']['vill_ward'] = array(
																  '#prefix'				=> '<div id="clra_common_2_replace"><section class="col col-4"><label class="select">',
																  '#suffix'				=> '<i></i></label></section></div>',
																  '#title'				=> 'Select Ward',
																  '#type' 				=> 'select',
																  '#options' 			=> custom_user_corporation_list_new($value_common_selection_1),
																  //'#required' 			=> TRUE,
																  '#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
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
								
								$form['app_reg']['vill_ward'] = array(
																  '#prefix'				=> '<div id="clra_common_2_replace"><section class="col col-4"><label class="select">',
																  '#suffix'				=> '<i></i></label></section></div>',
																  '#title'				=> 'Select SEZ Name',
																  '#type' 				=> 'select',
																  '#options' 			=> custom_user_villward_list_new($value_common_selection_1,'S'),
																  //'#required' 			=> TRUE,
																  '#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
																  //'#id'					=> 'common_selection_2_id',
																  '#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_ajax_callback_new_1',
																								'progress'			=> '',
																							  )
																);
						}
						
						if($value_loc_e_areatype == 'N'){
								
								$form['app_reg']['vill_ward'] = array(
																  '#prefix'				=> '<div id="clra_common_2_replace"><section class="col col-4"><label class="select">',
																  '#suffix'				=> '<i></i></label></section></div>',
																  '#title'				=> 'Select Notified Area',
																  '#type' 				=> 'select',
																  '#options' 			=> custom_user_villward_list_new($value_common_selection_1,'N'),
																  //'#required' 			=> TRUE,
																  '#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
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
			
			$value_common_selection_2 						= isset($form_state['values']['vill_ward']) ? $form_state['values']['vill_ward'] : '';
			
			$form['app_reg']['ps'] 							= array(
																'#prefix'				=> '<div id="clra_ps_replace"><section class="col col-4"><label class="select">',
																'#suffix'				=> '<i></i></label></section></div>',
																'#title'				=> 'Select Police Station',
																'#type' 				=> 'select',
																'#options' 				=> array( '' => '--Select--' ),
																//'#required' 			=> TRUE,
																'#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
																//'#id'					=> 'loc_e_ps_id',
																'#ajax'					=> array(
																							'event' 			=> 'change',
																							'effect' 			=> 'fade',			
																							'callback'			=> 'get_ajax_callback_new_1',
																							'progress'			=> '',
																						  )
														  );
			
			if($value_loc_e_dist != "" && $value_loc_e_subdivision != ""){
				if($value_loc_e_areatype != ""){
					if($value_common_selection_1 != ""){
						if($value_common_selection_2 != ""){
							$form['app_reg']['ps'] 			= array(
																  '#prefix'				=> '<div id="clra_ps_replace"><section class="col col-4"><label class="select">',
																  '#suffix'				=> '<i></i></label></section></div>',
																  '#title'				=> 'Select Police Station',
																  '#type' 				=> 'select',
																  //'#id'					=> 'loc_e_ps_id',
																  '#options' 			=> custom_user_ps_list_new($value_loc_e_dist),
																  //'#required' 			=> TRUE
																  '#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
															  );
						}
					}
				}
			}							
											
		
											
		$form['app_reg']['pin'] 							= array(
																	'#prefix'			=> '<section class="col col-4"><label class="input">',
																	'#suffix'			=> '</label></section></fieldset>',
																	'#title'			=> 'PIN Code',
																	'#type' 			=> 'textfield',
																	'#attributes' 		=> array('placeholder' => 'Enter your Area Pin Code', 'maxlength' => 6),
																	//'#required' 		=> TRUE,
																	'#element_validate'	=> array('DASHBOARD_TEXTBOX_NUMERIC_VALIDATE')
																);
		
		
		
												
		
		/*$pre_reg_est_check_options						= 	array( '1' => t('Please register your establishment here if you are a Principal Employer, Establishment, Employer, Association etc. <font color="#FF0000">( <b>Note :</b> Not Applicable for Contractor )</font>' ));
																
		$form['app_reg']['pre_reg_est_check']			=	array(
																'#prefix'			=> '<fieldset><legend>Establishment Information</legend><div class="row">',
																'#suffix'			=> '',
																'#type' 			=> 'checkboxes',
															    '#options' 			=> $pre_reg_est_check_options,
															    '#attributes'		=> array( 'id' => 'pre_reg_est_check_id')
															);*/
		
		$form['app_reg']['est_name'] 					= array(
																'#prefix' 			=>  '<fieldset><legend>Establishment Information (Optional)</legend><div class="row"><div id="show_estbalishment_div"><div class="row"><section class="col col-6"><label class="input">Establishment Name',
																'#suffix' 			=>  '</label></section>',
																'#type' 			=>  'textfield',
																//'#element_validate'	=>	array('REG_EST_VALIDATE')
														  );
														  
														  
		$form['app_reg']['est_loc'] 					= array(
																'#prefix' 			=>  '<section class="col col-6"><label class="input">',
																'#suffix' 			=>  '</label></section></div>',
																'#title'			=>  'Establishment Location',
																'#type' 			=>  'textfield',
																//'#required' 		=>  TRUE,
																//'#element_validate'		=> array('LOCATION_SECOND_VALIDATE')
															  );
			
		$form['app_reg']['est_district'] 				= array(
																'#prefix' 			=> '<div class="row"><section class="col col-6"><label class="select">',
																'#suffix' 			=> '<i></i></label></section>',
																'#title'			=> 'Select Establishment District',
																'#type' 			=> 'select',
																'#options' 			=> $options_dist,
																//'#required' 		=> TRUE,
																//'#element_validate'	=> array('REG_ESTABLISHMENT_DISTRICT_VALIDATE'),
																'#ajax'				=> array(
																							'event' 			=> 'change',
																							'effect' 			=> 'fade',			
																							'callback'			=> 'get_ajax_callback_new_2',
																							'progress'			=> '',
																						)
														  );
				
		$value_est_loc 						= 	isset($form_state['values']['est_district']) ? $form_state['values']['est_district'] : '';
		
		if($value_est_loc != "" || !empty($value_est_loc) || is_numeric($value_est_loc)){
				
			$form['app_reg']['est_subdivision'] 		= array(
																'#prefix' 			=> '<div id="app_subdvsn_replace"><section class="col col-6"><label class="select">',
																'#suffix' 			=> '<i></i></label></section></div>',
																'#title'			=> 'Select Subdivision',
																'#type' 			=> 'select',
																'#options' 			=> custom_user_subdivisionlist_list_new($value_est_loc),
																//'#required' 		=> TRUE,
																//'#element_validate'	=> array('REG_ESTABLISHMENT_SUBDIVISION_VALIDATE'),
																'#ajax'				=> array(
																							'event' 			=> 'change',
																							'effect' 			=> 'fade',			
																							'callback'			=> 'get_ajax_callback_new_2',
																							'progress'			=> '',
																					  ),
															);
																									
		}else{
			
			$form['app_reg']['est_subdivision'] 		= array(
																  '#prefix' 			=> '<div id="app_subdvsn_replace"><section class="col col-6"><label class="select">',
																  '#suffix' 			=> '<i></i></label></section></div>',
																  '#title'				=> 'Select Subdivision',
																  '#type' 				=> 'select',
																  '#options' 			=> array( '' => '--Select--' ),
																  //'#required' 			=> TRUE,
																  //'#element_validate'	=> array('REG_ESTABLISHMENT_SUBDIVISION_VALIDATE'),
																  '#ajax'				=> array(
																							'event' 			=> 'change',
																							'effect' 			=> 'fade',			
																							'callback'			=> 'get_ajax_callback_new_2',
																							'progress'			=> '',
																						  ),
																	);
		}
		
		$value_est_subdivision 				= isset($form_state['values']['est_subdivision']) ? $form_state['values']['est_subdivision'] : '';
		
		if($value_est_loc != "" && $value_est_subdivision != ""){
			
			$est_areatypeOptions								   = custom_user_areatype_list_new($value_est_loc, $value_est_subdivision);
			$est_areaTypeData									   = array( '' => '--Select--' );
			
			if(!empty($est_areatypeOptions)){
				
				foreach($est_areatypeOptions as $key=>$val){
					
					if($val == 'B'){
						$est_areaTypeData[$key]	= 'Block';
					}
					if($val == 'M'){
						$est_areaTypeData[$key]	= 'Municipality';
					}
					if($val == 'C'){
						$est_areaTypeData[$key]	= 'Corporation';
					}
					if($val == 'S'){
						$est_areaTypeData[$key]	= 'SEZ';
					}
					if($val == 'N'){
						$est_areaTypeData[$key]	= 'Notified Area';
					}
				}
			}
			
			$form['app_reg']['est_loc_e_areatype']  						   				= array(
																									  '#prefix' 			=> '<div id="app_loc_e_areatype"><section class="col col-6"><label class="select">',
																									  '#suffix' 			=> '<i></i></label></section></div>',
																									  '#title'				=> 'Select Areatype',
																									  '#type' 				=> 'select',
																									  '#options' 			=> $est_areaTypeData,
																									  //'#required' 			=> TRUE,
																									  //'#element_validate'	=> array('REG_ESTABLISHMENT_LOC_E_AREATYPE_VALIDATE'),
																									  '#ajax'				=> array(
																																'event' 			=> 'change',
																																'effect' 			=> 'fade',			
																																'callback'			=> 'get_ajax_callback_new_2',
																																'progress'			=> '',
																															  ),
																										);
																											
		}else{
			
			$form['app_reg']['est_loc_e_areatype'] 						   					= array(
																									  '#prefix' 			=> '<div id="app_loc_e_areatype"><section class="col col-6"><label class="select">',
																									  '#suffix' 			=> '<i></i></label></section></div>',
																									  '#title'				=> 'Select Areatype',
																									  '#type' 				=> 'select',
																									  '#options' 			=> array( '' => '--Select--' ),
																									  //'#required' 			=> TRUE,
																									  //'#element_validate'	=> array('REG_ESTABLISHMENT_LOC_E_AREATYPE_VALIDATE'),
																									  '#ajax'				=> array(
																																'event' 			=> 'change',
																																'effect' 			=> 'fade',			
																																'callback'			=> 'get_ajax_callback_new_2',
																																'progress'			=> '',
																															  ),
																										);
		}
		
		$value_est_loc_e_areatype 															= isset($form_state['values']['est_loc_e_areatype']) ? $form_state['values']['est_loc_e_areatype'] : '';
		
		$form['app_reg']['est_name_areatype'] 												= array(
																									 '#prefix' 				=> '<div id="app_common_1_replace"><section class="col col-6"><label class="select">',
																									 '#suffix' 				=> '<i></i></label></section></div>',
																									 '#title'				=> 'Select Block/Municipality/Corporation/SEZ/Notified Area',
																									 '#type' 				=> 'select',
																									 '#options' 			=> array( '' => '--Select--' ),
																									 //'#required' 			=> TRUE,
																									 //'#element_validate'	=> array('REG_ESTABLISHMENT_NAME_AREATYPE_VALIDATE'),
																									 '#ajax'				=> array(
																																'event' 			=> 'change',
																																'effect' 			=> 'fade',			
																																'callback'			=> 'get_ajax_callback_new_2',
																																'progress'			=> '',
																															  )
																								  );
		
		if($value_est_loc != "" && $value_est_subdivision != ""){
			if($value_est_loc_e_areatype != ""){
				if($value_est_loc_e_areatype == 'B'){
					$form['app_reg']['est_name_areatype'] 									= array(
																									  '#prefix' 			=>'<div id="app_common_1_replace"><section class="col col-6"><label class="select">',
																									  '#suffix' 			=> '<i></i></label></section></div>',
																									  '#title'				=> 'Select Block',
																									  '#type' 				=> 'select',
																									  '#options' 			=> custom_user_block_list_new($value_est_subdivision, $value_est_loc_e_areatype),
																									  //'#required' 			=> TRUE,
																									  //'#element_validate'	=> array('REG_ESTABLISHMENT_NAME_AREATYPE_VALIDATE'),
																									  '#ajax'				=> array(
																																'event' 			=> 'change',
																																'effect' 			=> 'fade',			
																																'callback'			=> 'get_ajax_callback_new_2',
																																'progress'			=> '',
																															  )
																									);
				}else if($value_est_loc_e_areatype == 'M'){
				
					$form['app_reg']['est_name_areatype'] 									= array(
																									  '#prefix' 			=> '<div id="app_common_1_replace"><section class="col col-6"><label class="select">',
																									  '#suffix' 			=> '<i></i></label></section></div>',
																									  '#title'				=> 'Select Municipality',
																									  '#type' 				=> 'select',
																									  '#options' 			=> custom_user_block_list_new($value_est_subdivision, $value_est_loc_e_areatype),
																									  //'#required' 			=> TRUE,
																									  //'#element_validate'	=> array('REG_ESTABLISHMENT_NAME_AREATYPE_VALIDATE'),
																									  '#ajax'				=> array(
																																'event' 			=> 'change',
																																'effect' 			=> 'fade',			
																																'callback'			=> 'get_ajax_callback_new_2',
																																'progress'			=> '',
																															  )
																									);
				}else if($value_est_loc_e_areatype == 'C'){
		
					$form['app_reg']['est_name_areatype'] 									= array(
																									  '#prefix' 			=> '<div id="app_common_1_replace"><section class="col col-6"><label class="select">',
																									  '#suffix' 			=> '<i></i></label></section></div>',
																									  '#title'				=> 'Select Corporation',
																									  '#type' 				=> 'select',
																									  '#options' 			=> custom_user_block_list_new($value_est_subdivision, $value_est_loc_e_areatype),
																									  //'#required' 			=> TRUE,
																									  //'#element_validate'	=> array('REG_ESTABLISHMENT_NAME_AREATYPE_VALIDATE'),
																									  '#ajax'				=> array(
																																'event' 			=> 'change',
																																'effect' 			=> 'fade',			
																																'callback'			=> 'get_ajax_callback_new_2',
																																'progress'			=> '',
																															  )
																								);
				}else if($value_est_loc_e_areatype == 'S'){
		
					$form['app_reg']['est_name_areatype'] 									= array(
																									  '#prefix' 			=> '<div id="app_common_1_replace"><section class="col col-6"><label class="select">',
																									  '#suffix' 			=> '<i></i></label></section></div>',
																									  '#title'				=> 'Select SEZ',
																									  '#type' 				=> 'select',
																									  '#options' 			=> custom_user_block_list_new($value_est_subdivision, $value_est_loc_e_areatype),
																									  //'#required' 			=> TRUE,
																									  //'#element_validate'	=> array('REG_ESTABLISHMENT_NAME_AREATYPE_VALIDATE'),
																									  '#ajax'				=> array(
																																'event' 			=> 'change',
																																'effect' 			=> 'fade',			
																																'callback'			=> 'get_ajax_callback_new_2',
																																'progress'			=> '',
																															  )
																									);
				}else if($value_est_loc_e_areatype == 'N'){
				
					$form['app_reg']['est_name_areatype'] 									= array(
																									  '#prefix' 			=> '<div id="app_common_1_replace"><section class="col col-6"><label class="select">',
																									  '#suffix' 			=> '<i></i></label></section></div>',
																									  '#title'				=> 'Select Notified Area',
																									  '#type' 				=> 'select',
																									  '#options' 			=> custom_user_block_list_new($value_est_subdivision, $value_est_loc_e_areatype),
																									  //'#required' 			=> TRUE,
																									  //'#element_validate'	=> array('REG_ESTABLISHMENT_NAME_AREATYPE_VALIDATE'),
																									  '#ajax'				=> array(
																																'event' 			=> 'change',
																																'effect' 			=> 'fade',			
																																'callback'			=> 'get_ajax_callback_new_2',
																																'progress'			=> '',
																															  )
																									);
				}
			}
		}
		
		$value_est_name_areatype 																= isset($form_state['values']['est_name_areatype']) ? $form_state['values']['est_name_areatype'] : '';
		
		$form['app_reg']['est_loc_e_vill_ward'] 											= array(
																									 '#prefix' 				=> '<div id="app_common_2_replace"><section class="col col-4"><label class="select">',
																									 '#suffix' 				=> '<i></i></label></section></div>',
																									 '#title'				=> 'Select Gram Panchayat/Ward/Sector',
																									 '#type' 				=> 'select',
																									 '#options' 			=> array( '' => '--Select--' ),
																									 //'#required' 			=> TRUE,
																									 //'#element_validate'	=> array('REG_ESTABLISHMENT_LOC_VILL_WARD_VALIDATE'),
																									 '#ajax'				=> array(
																																'event' 			=> 'change',
																																'effect' 			=> 'fade',			
																																'callback'			=> 'get_ajax_callback_new_2',
																																'progress'			=> '',
																															  )
																								  );
		
		if($value_est_loc != "" && $value_est_subdivision != ""){
			if($value_est_loc_e_areatype != ""){
				if($value_est_name_areatype != ""){
					if($value_est_loc_e_areatype == 'B'){
							
							$form['app_reg']['est_loc_e_vill_ward'] 						= array(
																									  '#prefix' 			=> '<div id="app_common_2_replace"><section class="col col-4"><label class="select">',
																									  '#suffix' 			=> '<i></i></label></section></div>',
																									  '#title'				=> 'Select Gram Panchayat',
																									  '#type' 				=> 'select',
																									  '#options'			=> custom_user_villward_list_new($value_est_name_areatype,'V'),
																									  //'#required' 			=> TRUE,
																									  //'#element_validate'	=> array('REG_ESTABLISHMENT_LOC_VILL_WARD_VALIDATE'),
																									  '#ajax'				=> array(
																																	'event' 			=> 'change',
																																	'effect' 			=> 'fade',			
																																	'callback'			=> 'get_ajax_callback_new_2',
																																	'progress'			=> '',
																																  )
																									);	
					}
						
					if($value_est_loc_e_areatype == 'M'){
							
							$form['app_reg']['est_loc_e_vill_ward'] 						= array(
																									  '#prefix' 			=> '<div id="app_common_2_replace"><section class="col col-4"><label class="select">',
																									  '#suffix' 			=> '<i></i></label></section></div>',
																									  '#title'				=> 'Select Ward',
																									  '#type' 				=> 'select',
																									  '#options' 			=> custom_user_villward_list_new($value_est_name_areatype,'W'),
																									  //'#required' 			=> TRUE,
																									  //'#element_validate'	=> array('REG_ESTABLISHMENT_LOC_VILL_WARD_VALIDATE'),
																									  '#ajax'				=> array(
																																	'event' 			=> 'change',
																																	'effect' 			=> 'fade',			
																																	'callback'			=> 'get_ajax_callback_new_2',
																																	'progress'			=> '',
																																  )
																									);
																							
					}
						
					if($value_est_loc_e_areatype == 'C'){
							
							$form['app_reg']['est_loc_e_vill_ward'] 						= array(
																									  '#prefix' 			=> '<div id="app_common_2_replace"><section class="col col-4"><label class="select">',
																									  '#suffix' 			=> '<i></i></label></section></div>',
																									  '#title'				=> 'Select Ward',
																									  '#type' 				=> 'select',
																									  '#options' 			=> custom_user_corporation_list_new($value_est_name_areatype),
																									  //'#required' 			=> TRUE,
																									  //'#element_validate'	=> array('REG_ESTABLISHMENT_LOC_VILL_WARD_VALIDATE'),
																									  '#ajax'				=> array(
																																'event' 			=> 'change',
																																'effect' 			=> 'fade',			
																																'callback'			=> 'get_ajax_callback_new_2',
																																'progress'			=> '',
																															  )
																									);
					}
					
					if($value_est_loc_e_areatype == 'S'){
							
							$form['app_reg']['est_loc_e_vill_ward'] 						= array(
																									  '#prefix' 			=> '<div id="app_common_2_replace"><section class="col col-4"><label class="select">',
																									  '#suffix' 			=> '<i></i></label></section></div>',
																									  '#title'				=> 'Select Sector',
																									  '#type' 				=> 'select',
																									  '#options' 			=> custom_user_villward_list_new($value_est_name_areatype,'S'),
																									  //'#required' 			=> TRUE,
																									  //'#element_validate'	=> array('REG_ESTABLISHMENT_LOC_VILL_WARD_VALIDATE'),
																									  '#ajax'				=> array(
																																	'event' 			=> 'change',
																																	'effect' 			=> 'fade',			
																																	'callback'			=> 'get_ajax_callback_new_2',
																																	'progress'			=> '',
																																  )
																									);
																							
					}
					
					if($value_est_loc_e_areatype == 'N'){
								
								$form['app_reg']['est_loc_e_vill_ward'] = array(
																  '#prefix'				=> '<div id="app_common_2_replace"><section class="col col-4"><label class="select">',
																  '#suffix'				=> '<i></i></label></section></div>',
																  '#title'				=> 'Select Notified Area',
																  '#type' 				=> 'select',
																  '#options' 			=> custom_user_villward_list_new($value_est_name_areatype,'N'),
																  //'#required' 			=> TRUE,
																  //'#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
																  //'#id'					=> 'common_selection_2_id',
																  '#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_ajax_callback_new_2',
																								'progress'			=> '',
																							  )
																);
						}
				}
			}
		}
		
		$value_est_loc_e_vill_ward 	= isset($form_state['values']['est_loc_e_vill_ward']) ? $form_state['values']['est_loc_e_vill_ward'] : '';
		
		$form['app_reg']['est_ps'] 	= array(
																'#prefix' 				=> '<div id="app_ps_replace"><section class="col col-4"><label class="select">',
																'#suffix' 				=> '<i></i></label></section></div>',
																'#title'				=> 'Select Police Station',
																'#type' 				=> 'select',
																'#options' 				=> array( '' => '--Select--' ),
																//'#required' 			=> TRUE,
																//'#element_validate'		=> array('REG_ESTABLISHMENT_POLICE_VALIDATE'),
																'#ajax'					=> array(
																							'event' 			=> 'change',
																							'effect' 			=> 'fade',			
																							'callback'			=> 'get_ajax_callback_new_2',
																							'progress'			=> '',
																						  )
														  );
		
		if($value_est_loc != "" && $value_est_subdivision != ""){
			if($value_est_loc_e_areatype != ""){
				if($value_est_name_areatype != ""){
					if($value_est_loc_e_vill_ward != ""){
						$form['app_reg']['est_ps'] 									= array(
																									  '#prefix' 			=> '<div id="app_ps_replace"><section class="col col-4"><label class="select">',
																									  '#suffix' 			=> '<i></i></label></section></div>',
																									  '#title'				=> 'Select Police Station',
																									  '#type' 				=> 'select',
																									  '#options' 			=> custom_user_ps_list_new($value_est_loc),
																									  //'#required' 			=> TRUE
																									  //'#element_validate'	=> array('REG_ESTABLISHMENT_POLICE_VALIDATE')
																								  );
					}
				}
			}
		}
		
		
		$form['app_reg']['est_pin_number'] 											= array(
																									  '#prefix' 			=> '<section class="col col-4"><label class="input">',
																									  '#suffix' 			=> '</label></section></div></div></fieldset>',
																									  '#title'				=> 'Pin Code',
																									  '#type' 				=> 'textfield',
																									  '#attributes' 		=> array('maxlength' => 6),
																									  //'#required' 			=> TRUE
																									  //'#element_validate'	=> array('EST_TEXTBOX_NUMERIC_VALIDATE')
																								  );
		
		
		$form['app_reg']['work_man_master_role_male'] =	array(
												'#prefix'			=> '<fieldset><legend>Number of workers employed (Optional)</legend><table width="100%"><tr><td>No of workman of master role</td><td>',
												'#suffix'			=> '</td>',
												'#type'				=> 'textfield',
												'#title'			=> t('Male'),																												
												'#attributes' 		=> array('maxlength' => 20, 'class' => array('no_of_workmn'))
											   );
		$form['app_reg']['work_man_master_role_female'] =	array(
												'#prefix'		=> '<td>',
												'#suffix'		=> '</td></tr>',
												'#type'			=> 'textfield',
												'#title'		=> t('Female'),																													
												'#attributes' 	=> array('maxlength' => 20, 'class' => array('no_of_workmn'))
												);
		$form['app_reg']['contractual_labour_male'] =	array(
												'#prefix'		=> '<tr><td>No of contractual labour</td><td>',
												'#suffix'		=> '</td>',
												'#type'			=> 'textfield',
												'#title'		=> t('Male'),																													
												'#attributes' 	=> array('maxlength' => 20, 'class' => array('no_of_workmn'))
												);
		
		$form['app_reg']['contractual_labour_female'] =	array(
												'#prefix'		=> '<td>',
												'#suffix'		=> '</td></tr>',
												'#type'			=> 'textfield',
												'#title'		=> t('Female'),																													
												'#attributes' 	=> array('maxlength' => 20, 'class' => array('no_of_workmn'))
												);
		$form['app_reg']['child_labour_male'] =	array(
												'#prefix'		=> '<tr><td>No of other worker engaged</td><td>',
												'#suffix'		=> '</td>',
												'#type'			=> 'textfield',
												'#title'		=> t('Male'),																													
												'#attributes' 	=> array('maxlength' => 20, 'class' => array('no_of_workmn'))
												);
		
		$form['app_reg']['child_labour_female'] =	array(
												'#prefix'		=> '<td>',
												'#suffix'		=> '</td></tr></table></fieldset>',
												'#type'			=> 'textfield',
												'#title'		=> t('Female'),																													
												'#attributes' 	=> array('maxlength' => 20, 'class' => array('no_of_workmn'))
												);		
																
		$form['app_reg']['username'] 		=	array(
												'#prefix'			=> '<fieldset><legend>Login Information</legend><div class="row"><section class="col col-4"><label class="input">',
												'#suffix'			=> '</label>',
												'#type'				=> 'textfield',
												'#title'			=> t('Username'),
												'#required'			=> TRUE,
												'#default_value'	=> (isset($form_state['values']['username'])) ? $form_state['values']['username'] : '',
												'#attributes' 		=> array( 'placeholder'=>'Enter username', 'maxlength' => 255, 'onkeyup' => 'checkUserName(this.val);', 'onblur' => 'checkUserName(this.val);', 'class' => array('username_fld'), 'id' => 'username_id')
												);
															
		$form['app_reg']['username_img_markup'] 		= array(
																'#type' 			=> 'markup', 
																'#markup' 			=> '<span id="common_username_img_div"></span>'
														   );
															
		$form['app_reg']['username_markup'] 			= array(
																'#type' 			=> 'markup', 
																'#markup' 			=> '<span id="common_username_div"></span></section>'
														   );
							
		$form['app_reg']['password'] 					=	array(
																'#prefix'			=> '<section class="col col-4"><label class="input">',
																'#suffix'			=> '</label></section>',
																'#type'				=> 'password',
																'#title'			=> t('Password'),
																'#size'				=> 20,
																'#required'			=> TRUE,
																'#attributes' 		=> array('id' => 'passwordId', 'placeholder'=>'Enter password', 'onkeyup' => 'setCheckPasswordStrength(this.val);', 'onblur' => 'setCheckPasswordStrength(this.val);', 'class' => array('pwd_fld') )
															);
															
		$form['app_reg']['password_markup'] 			= array(
																'#type' 			=> 'markup', 
																'#markup' 			=> '<span id="hint"><img src="'.$base_root.$base_path.'sites/all/modules/applicant_registration/images/tooltip.jpg"></span>'
														   );
							
		$form['app_reg']['conpassword'] 				=	array(
																'#prefix'			=> '<section class="col col-4"><label class="input">',
																'#suffix'			=> '</label></section></div></fieldset>',
																'#type'				=> 'password',
																'#title'			=> t('Confirm Password'),
																'#size'				=> 20,
																'#required'			=> TRUE,
																'#attributes' 		=> array('id' => 'conpasswordId', 'placeholder'=>'Enter password', 'onkeyup' => 'setCheckPasswordStrength(this.val);', 'onblur' => 'setCheckPasswordStrength(this.val);', 'class' => array('pwd_fld') )
															);
		
		
		$form['app_reg']['markup_msg'] 					=	array(
																'#type'				=> 'markup',
																'#markup'			=> '<div id="setPasswordMessage" style="display:none;"></div>'
															);
		
				
		$form['app_reg']['submit_button']					= array(
																'#prefix'				=> '<footer>',
																'#suffix'				=>'</footer></div></div>',
																'#attributes' 			=> array('class' => array('button left-btn')),
																'#type'					=>'submit',
																'#value'				=>t('SUBMIT')
																);		
		
		return $form; 
	}
	
	function applicant_forms_format_dob($form_element, &$form_state){
		unset($form_element['year']['#options']);
		$max_age = date('Y') - 100; // 100 years ago
		
		$min_age = date('Y');
		//echo $max_age."--".$min_age; exit;
		$form_element['year']['#options'] = array();
		foreach (range($max_age, $min_age) as $year){
			$form_element['year']['#options'][$year] = $year;
		}
		return $form_element;
	}
	
	function DASHBOARD_TEXTBOX_TEXT_VALIDATE($element, &$form_state){
		$txtValue		=	trim($element['#value']); 
		$match_pattern 	= 	'/[A-Za-z]+/';
		if( $txtValue == "" ){
			form_error($element, t($element['#title'].' cannot be empty.'));
		}
		if(!preg_match($match_pattern, $txtValue)) {
			form_error($element, t($element['#title'].' should be text only.'));
		}
		//$form_state['rebuild']	=	true;
	}
	
	function DASHBOARD_CARD_TYPE_VALIDATE($element, &$form_state){
		if( $form_state['values']['card_type'] == "" ){
			form_error($element, t($element['#title'].' cannot be empty.'));
		}
		//$form_state['rebuild']	=	true;
	}
	
	function DASHBOARD_SPECIAL_CHAR_TEXT_VALIDATE($element, &$form_state){
		$card_type		=	$form_state['values']['card_type'];
		$card_number	=	$form_state['values']['card_number'];
		
		if( $card_number == "" ){
			form_error($element, t($element['#title'].' cannot be empty.'));
		}
		if( $card_type == 1 ){
			if (strlen(trim($card_number)) != 12 ){
				form_error($element, t('Aadhar number should be 12 digits.'));
			}else if( !is_numeric($card_number) ){
				form_error($element, t('Aadhar number should be numeric.'));
			}
		}else if( $card_type == 2 ){
			if (strlen(trim($card_number)) != 10 ){
				form_error($element, t('PAN number should be 10 digits.'));
			}else if (!preg_match("/^([a-zA-Z]){5}([0-9]){4}([a-zA-Z]){1}?$/", $card_number)) {
  				form_error($element, t('PAN number should be valid.'));
			}
		}
		//$form_state['rebuild']	=	true;
	}
	
	function DASHBOARD_EMAIL_VALIDATE($element, &$form_state){
		 $email 	=	trim($form_state['values']['email']);
		 if( $email == "" ){
			form_error($element, t($element['#title'].' cannot be empty.')); 
		 }
		 if (!filter_var($email, FILTER_VALIDATE_EMAIL)) {
			form_error($element, t($element['#title'].' format invalid.'));
		 }
		 
		 $validateEmail					=	db_query("select mail from users where mail=:mailval", array(':mailval'=> $form_state['values']['email']));
		 if( $validateEmail->rowCount() > 0 ){
			form_error($element, t($element['#title'].' already exists.'));
			$form_state['rebuild'] = TRUE;
		}
		//$form_state['rebuild']	=	true;
	}
	
	function DASHBOARD_PHONE_VALIDATE($element, &$form_state){
		$mob	= "/^[789][0-9]{9}$/";
		if(strlen(trim($form_state['values']['phone']))>0){
			if(!preg_match($mob, $form_state['values']['phone'])){ 
			  if (strlen(trim($form_state['values']['phone'])) < 10){
				 form_error($element, t($element['#title'].' is less than 10 digit.'));
			  }else{
				form_error($element, t($element['#title'].' should be valid.'));
			  }
			}
		 }else{
			form_error($element, t($element['#title'].' cannot be blank.'));
		 }
		 //$form_state['rebuild']	=	true;
	}
	
	function DASHBOARD_CONTACT_NUMBER_VALIDATE($element, &$form_state){
		if( !empty($form_state['values']['contact_number']) ){
			if(strlen(trim($form_state['values']['contact_number']))>0){
			  	if (strlen(trim($form_state['values']['contact_number'])) < 8){
					form_error($element, t($element['#title'].' is less than 8 digit.'));
			  	}
			}
		}
		//$form_state['rebuild']	=	true;
	}
	
	function REG_ADDRESS_VALIDATE($element, &$form_state){
		if( $form_state['values']['dist_code'] == "" ){
			form_error($element, t($element['#title'].' cannot be empty.'));
		}
		if( $form_state['values']['subdivision'] == "" ){
			form_error($element, t($element['#title'].' cannot be empty.'));
		}
		if( $form_state['values']['areatype'] == "" ){
			form_error($element, t($element['#title'].' cannot be empty.'));
		}
		if( $form_state['values']['name_areatype'] == "" ){
			form_error($element, t($element['#title'].' cannot be empty.'));
		}
		if( $form_state['values']['vill_ward'] == "" ){
			form_error($element, t($element['#title'].' cannot be empty.'));
		}
		if( $form_state['values']['ps'] == "" ){
			form_error($element, t($element['#title'].' cannot be empty.'));
		}
		//$form_state['rebuild']	=	true;
	}
	
	function REG_ESTABLISHMENT_DISTRICT_VALIDATE($element, &$form_state){
		//if($form_state['values']['pre_reg_est_check'][1]==1){
			if( $form_state['values']['est_district'] == "" ){
				form_error($element, t('Establishment district cannot be empty.'));
			}
		//}
		//$form_state['rebuild']	=	true;
	}
	
	function REG_ESTABLISHMENT_SUBDIVISION_VALIDATE($element, &$form_state){
		//if($form_state['values']['pre_reg_est_check'][1]==1){
			if( $form_state['values']['est_subdivision'] == "" ){
				form_error($element, t('Establishment subdivision cannot be empty.'));
			}
		//}
		//$form_state['rebuild']	=	true;
	}
	
	function REG_ESTABLISHMENT_LOC_E_AREATYPE_VALIDATE($element, &$form_state){
		//if($form_state['values']['pre_reg_est_check'][1]==1){
			if( $form_state['values']['est_loc_e_areatype'] == "" ){
				form_error($element, t('Establishment areatype cannot be empty.'));
			}
		//}
		//$form_state['rebuild']	=	true;
	}
	
	function REG_ESTABLISHMENT_NAME_AREATYPE_VALIDATE($element, &$form_state){
		//if($form_state['values']['pre_reg_est_check'][1]==1){
			if( $form_state['values']['est_name_areatype'] == "" ){
				form_error($element, t('Establishment Block/Municipality/Corporation/SEZ/Notified Area cannot be empty.'));
			}
		//}
		//$form_state['rebuild']	=	true;
	}
	
	function REG_ESTABLISHMENT_LOC_VILL_WARD_VALIDATE($element, &$form_state){
		//if($form_state['values']['pre_reg_est_check'][1]==1){
			if( $form_state['values']['est_loc_e_vill_ward'] == "" ){
				form_error($element, t('Establishment Gram Panchayat/Ward cannot be empty.'));
			}
		//}
		//$form_state['rebuild']	=	true;
	}
	
	function REG_ESTABLISHMENT_POLICE_VALIDATE($element, &$form_state){
		//if($form_state['values']['pre_reg_est_check'][1]==1){
			if( $form_state['values']['est_ps'] == "" ){
				form_error($element, t('Establishment Police Station cannot be empty.'));
			}
		//}
		//$form_state['rebuild']	=	true;
	}
	
	function DASHBOARD_TEXTBOX_NUMERIC_VALIDATE($element, &$form_state){
		if($form_state['values']['pin'] == ""){
			form_error($element, t($element['#title'].' cannot be empty.'));
		}
		if(strlen(trim($form_state['values']['pin']))!=6){
			form_error($element, t($element['#title'].' should be 6 digits.'));
		}
		if(!is_numeric($form_state['values']['pin'])){
			form_error($element, t($element['#title'].' should be numeric.'));
		}
		//$form_state['rebuild']	=	true;
	}
	
	function EST_TEXTBOX_NUMERIC_VALIDATE($element, &$form_state){
		//if($form_state['values']['pre_reg_est_check'][1]==1){
			if($form_state['values']['est_pin_number'] == ""){
				form_error($element, t('Establishment Pin cannot be empty.'));
			}
			if(strlen(trim($form_state['values']['est_pin_number']))!=6){
				form_error($element, t('Establishment Pin should be 6 digits.'));
			}
			if(!is_numeric($form_state['values']['est_pin_number'])){
				form_error($element, t('Establishment Pin should be numeric.'));
			}
		//}
		//$form_state['rebuild']	=	true;
	}
	
	function REG_EST_VALIDATE($element, &$form_state){
		//if($form_state['values']['pre_reg_est_check'][1]==1){
			if($form_state['values']['est_name']==""){
				form_error($element, t('Establishment name cannot be empty.'));	
			}
		//}
		//$form_state['rebuild']	=	true;
	}
	
	function LOCATION_VALIDATE($element, &$form_state){
		if($form_state['values']['address']==""){
			form_error($element, t($element['#title'].' cannot be empty.'));	
		}
		//$form_state['rebuild']	=	true;
	}
	
	function LOCATION_SECOND_VALIDATE($element, &$form_state){
		//if($form_state['values']['pre_reg_est_check'][1]==1){
			if($form_state['values']['est_loc']==""){
				form_error($element, t('Establishment location cannot be empty.'));	
			}
		//}
		//$form_state['rebuild']	=	true;
	}
	
	function applicant_registration_form_submit($form, &$form_state) {
		global $base_root, $base_path, $user;
		//print_r($form_state['values']); exit;
		if( $form_state['values']['contact_number'] != "" && is_numeric($form_state['values']['contact_number']) ){
			$contact_number	=	$form_state['values']['contact_number'];
		}else{
			$contact_number	=	0;
		}
		
			$time						=	time();
			$date						=	date('Y-m-d');
			$tempAckNumber				=	$time."-".$date;
			$newUser 					= 	array(
												'name' 		=> trim($form_state['values']['username']),
												'pass' 		=> trim($form_state['values']['password']), 
												'mail' 		=> trim($form_state['values']['email']),
												'status' 	=> 1,
												'init' 		=> trim($form_state['values']['email']),
											);
			user_save(null, $newUser);
			$txtValue					=	trim($form_state['values']['username']); 
			$fetch_info_querys 			= 	db_select('users', 'U');
			$fetch_info_querys->fields('U',  array('uid'));
			$fetch_info_querys->condition('U.name',$txtValue);
			$fetch_info_querys_result 	= 	$fetch_info_querys->execute()->fetchObject();
			$lastId						=	$fetch_info_querys_result->uid;  
			$userInfo 					= 	array(
												'uid' 			=> $lastId,
												'fullname'		=> $form_state['values']['f_name']." ".$form_state['values']['l_name'],
												'uid_phone' 	=> $form_state['values']['phone'],
												'passwd' 		=> trim($form_state['values']['password']), 
												'tempacknumber' => $tempAckNumber  
											);
		   db_insert('l_user_additional_reg_details')->fields($userInfo)->execute(); 
		   $userRole 					= 	array(
												'uid' => $lastId,
												'rid' => '8'
											);
			db_insert('users_roles')->fields($userRole)->execute();
			
			$fieldsCommonAppInfo	=	array(
												'user_id'						=> 	$lastId,
												'f_name'						=>	$form_state['values']['f_name'],
												'm_name'						=>	$form_state['values']['m_name'], 
												'l_name'						=>	$form_state['values']['l_name'], 
												'dob'							=>	$form_state['values']['dob']['year']."-".$form_state['values']['dob']['month']."-".$form_state['values']['dob']['day'],
												'email'							=>	$form_state['values']['email'], 
												'mobile'						=>	$form_state['values']['phone'],
												  
												'gender'						=>	$form_state['values']['gender'], 
												'address'						=>	$form_state['values']['address'],
												
												'dist_code'						=>	$form_state['values']['dist_code'],
												'subdivision'					=>	$form_state['values']['subdivision'],
												'areatype'						=>	$form_state['values']['areatype'],
												'name_areatype'					=>	$form_state['values']['name_areatype'],
												'vill_ward'						=>	$form_state['values']['vill_ward'],
												'ps'							=>	$form_state['values']['ps'],
												'pin'							=>	!empty($form_state['values']['pin']) ? $form_state['values']['pin'] : 0,
												
												'est_name'						=>	!empty($form_state['values']['est_name']) ? $form_state['values']['est_name'] : '',
												'est_loc'						=>	!empty($form_state['values']['est_loc']) ? $form_state['values']['est_loc'] : '',
												'est_district'					=>	!empty($form_state['values']['est_district']) ? $form_state['values']['est_district'] : '',
												'est_subdivision'				=>	!empty($form_state['values']['est_subdivision']) ? $form_state['values']['est_subdivision'] : '',
												'est_loc_e_areatype'			=>	!empty($form_state['values']['est_loc_e_areatype']) ? $form_state['values']['est_loc_e_areatype'] : '',
												'est_name_areatype'				=>	!empty($form_state['values']['est_name_areatype']) ? $form_state['values']['est_name_areatype'] : 0,
												'est_loc_e_vill_ward'			=>	!empty($form_state['values']['est_loc_e_vill_ward']) ? $form_state['values']['est_loc_e_vill_ward'] : 0,
												'est_ps'						=>	!empty($form_state['values']['est_ps']) ? $form_state['values']['est_ps'] : 0,
												'est_pin_number'				=>	!empty($form_state['values']['est_pin_number']) ? $form_state['values']['est_pin_number'] : 0,
												
												'card_type'						=>	$form_state['values']['card_type'],
												'card_number'					=>	$form_state['values']['card_number'],
												'status'						=>	1,
												'contact_number'				=>  $contact_number,
												
												'work_man_master_role_male'		=>	!empty($form_state['values']['work_man_master_role_male']) ? $form_state['values']['work_man_master_role_male'] : 0,
												'work_man_master_role_female'	=>	!empty($form_state['values']['work_man_master_role_female']) ? $form_state['values']['work_man_master_role_female'] : 0,
												'contractual_labour_male'		=>	!empty($form_state['values']['contractual_labour_male']) ? $form_state['values']['contractual_labour_male'] : 0,
												'contractual_labour_female'		=>	!empty($form_state['values']['contractual_labour_female']) ? $form_state['values']['contractual_labour_female'] : 0,
												'child_labour_male'				=>	!empty($form_state['values']['child_labour_male']) ? $form_state['values']['child_labour_male'] : 0,
												'child_labour_female'			=>	!empty($form_state['values']['child_labour_female']) ? $form_state['values']['child_labour_female'] : 0
											);
			//print_r($fieldsCommonAppInfo); exit;
			// !empty($amendment_parent_id) ? $amendment_parent_id : '',
			db_insert('l_common_application_master')->fields($fieldsCommonAppInfo)->execute();
			
			
			$phone	=	trim($form_state['values']['phone']);
			if(!empty($phone)){
				$msg	=	"Thank you ".str_replace('&', 'and', trim($form_state['values']['f_name'])." ".trim($form_state['values']['l_name']))." for registering on Labour Commissionerate .Please keep the username safely and change your password at regular intervals. Your Username - ".trim($form_state['values']['username'])." and Password - ".trim($form_state['values']['password']);
				module_load_include('inc', 'common_encryption_form', 'include/test_cryptography_form');
				send_sms($phone, $msg);
				send_sms('9749689171', $msg);
			}
			
			drupal_set_message('Registration Successfully. Now enter your username and password for login.');
			drupal_goto('applicant-login');			
	}
	
	function get_ajax_callback_new_1($form, $form_state) {
		$commands 	= array();
		$commands[] = ajax_command_replace('#clra_subdvsn_replace', drupal_render($form['app_reg']['subdivision']));
		$commands[] = ajax_command_replace('#clra_loc_e_areatype', drupal_render($form['app_reg']['areatype']));
		$commands[] = ajax_command_replace('#clra_common_1_replace', drupal_render($form['app_reg']['name_areatype']));
		$commands[] = ajax_command_replace('#clra_common_2_replace', drupal_render($form['app_reg']['vill_ward']));
		$commands[] = ajax_command_replace('#clra_ps_replace', drupal_render($form['app_reg']['ps']));
		return array('#type' => 'ajax', '#commands' => $commands);
	}
	
	function get_ajax_callback_new_2($form, $form_state){
		$commands 	= array();
		$commands[] = ajax_command_replace('#app_subdvsn_replace', drupal_render($form['app_reg']['est_subdivision']));
		$commands[] = ajax_command_replace('#app_loc_e_areatype', drupal_render($form['app_reg']['est_loc_e_areatype']));
		$commands[] = ajax_command_replace('#app_common_1_replace', drupal_render($form['app_reg']['est_name_areatype']));
		$commands[] = ajax_command_replace('#app_common_2_replace', drupal_render($form['app_reg']['est_loc_e_vill_ward']));
		$commands[] = ajax_command_replace('#app_ps_replace', drupal_render($form['app_reg']['est_ps']));
		return array('#type' => 'ajax', '#commands' => $commands);
	}
	
	function custom_user_district_list_new() {
		$default_select 	= array(""=>"- Select District -");
		$result 			= db_query("SELECT district_code,district_name FROM {district_master} where is_active = 'Y' order by district_name");
		$list 				= array();
		foreach ($result as $record) {
			$list[$record->district_code] = t('@districtname', array(
				'@districtname' => $record->district_name,
			));
		}
		return ($default_select+$list);
	}
	
	function custom_user_subdivisionlist_list_new($value_dist){
		$default_subdvselect 	= array(""=>"- Select Sub-division -");
		$all_subdvs 			= array();
		$query_subdv 			= db_select('sub_division', 'subdv');
		$query_subdv			->fields('subdv',array('sub_div_code','sub_div_name'));
		$query_subdv			->orderBy('subdv.sub_div_name', 'ASC');
		$query_subdv			->condition('subdv.is_active','Y');
		$query_subdv			->condition('subdv.district_code',(int)$value_dist);
		$result_subdv 			= $query_subdv->execute(); 
		if(!empty($result_subdv)) {      
			foreach($result_subdv as $rows) {
				$all_subdvs[$rows->sub_div_code] = t('@subdvsname', array('@subdvsname' => $rows->sub_div_name));
			}    
		}
		if (isset($all_subdvs)) {			
			return ($default_subdvselect + $all_subdvs);	
		} else {				
			return $default_subdvselect;
		}
	}
	
	function custom_user_areatype_list_new($value_loc_e_dist, $value_loc_e_subdivision){
		$default_areatypeselect 	= array(""=>"- Select AreaType -");
		$all_areatype 				= array();
		$query_areatype 			= db_select('block_mun_master', 'bmm');
		$query_areatype				->fields('bmm',array('type'));
		$query_areatype				->condition('bmm.distict_code', trim($value_loc_e_dist));
		$query_areatype				->condition('bmm.sub_division_code', trim($value_loc_e_subdivision));
		$query_areatype				->groupBy('bmm.type');
		$result_areatype 			= $query_areatype->execute(); 
		if(!empty($result_areatype)) {      
			foreach($result_areatype as $rows) {
				$all_areatype[$rows->type] = t('@areatype', array('@areatype' => $rows->type));
			}    
		}
		if (isset($all_areatype)) {			
			return ($default_areatypeselect + $all_areatype);	
		} else {				
			return $default_areatypeselect;
		}
	}
	
	function custom_user_block_list_new($value_subdv, $value_areatype){
		$default_blockselect 	= array(""=>"- Select -");
		$all_blocks 			= array();
		$query_block 			= db_select('block_mun_master', 'blm');
		$query_block			->fields('blm',array('block_code','block_mun_name'));
		$query_block			->orderBy('blm.block_mun_name', 'ASC');
		$query_block			->condition('blm.sub_division_code',$value_subdv,'=');
		$query_block			->condition('blm.type',$value_areatype,'=');
		$result_block 			= $query_block->execute(); 
		if(!empty($result_block)) {      
			foreach($result_block as $rows) {
				$all_blocks[$rows->block_code] = t('@blockname', array('@blockname' => $rows->block_mun_name));
			}    
		}
	
		if (isset($all_blocks)) {
			return ($default_blockselect + $all_blocks);
		} else {
			return $default_blockselect;
		}
	}
	
	function custom_user_villward_list_new($value_villwared,$value_villwardtype){
		//echo $value_villwardtype."---".$value_villwared; die();
		$default_villwardselect 	= array(""=>"- Select -");
		$all_villwards 				= array();
		$query_villwardkname 		= db_select('village_ward_master', 'vwm');
		$query_villwardkname		->fields('vwm',array('village_code','village_name'));
		
		if($value_villwardtype == 'W'){
			$query_villwardkname		->orderBy('vwm.ord', 'ASC');
		}else{
			$query_villwardkname		->orderBy('vwm.village_name', 'ASC');
		}
	
		$query_villwardkname		->condition('vwm.block_code',$value_villwared,'=');
		$query_villwardkname		->condition('vwm.category',$value_villwardtype,'=');
		$result_villwardkname 		= $query_villwardkname->execute(); 
		if(!empty($result_villwardkname)) {      
			 foreach($result_villwardkname as $rows) {
				if($value_villwardtype == 'W'){
					$all_villwards[$rows->village_code] = t('@villwardkname', array('@villwardkname' => 'Ward-'.$rows->village_name));	
				}else{
					$all_villwards[$rows->village_code] = t('@villwardkname', array('@villwardkname' => ucwords($rows->village_name)));
				}
				if($value_villwardtype=='S'){
					$all_villwards[$rows->village_code] = t('@villwardkname', array('@villwardkname' => $rows->village_name));	
				}else{
					$all_villwards[$rows->village_code] = t('@villwardkname', array('@villwardkname' => $rows->village_name));
				}
			 }    
		}
	
		if (isset($all_villwards)) {
			//print_r($default_villwardselect + $all_villwards); exit;
			return ($default_villwardselect + $all_villwards);
		} else {
			return $default_villwardselect;
		}
	}
	
	function custom_user_corporation_list_new($code){
		$default_corporationselect 		= array(""=>"- Select -");
		$all_corporation 				= array();
		$query_corporationname 			= db_select('village_ward_master', 'vwm');
		$query_corporationname			->fields('vwm',array('village_code', 'village_name'));
		$query_corporationname			->condition('vwm.block_code', $code, '=');
		$query_corporationname			->condition('vwm.category', 'C', '=');
		$query_corporationname			->orderBy('vwm.ord', 'ASC');
		$result_corporationname 		= $query_corporationname->execute(); 		
		if(!empty($result_corporationname)) {      
			foreach($result_corporationname as $rows) {
				$all_corporation[$rows->village_code] = t('@village_name', array('@village_name' => $rows->village_name));
			}    
		}
		if (isset($all_corporation)) {
			return ($default_corporationselect + $all_corporation);
		} else {
			return $default_corporationselect;
		}
	}
	
	function custom_user_ps_list_new($value_dist){
		$default_pselect 	= array(""=>"- Select -");
		$all_ps 			= array();
		$query_psname 		= db_select('police_station', 'ps');
		$query_psname		->fields('ps',array('police_station_code','name_of_police_station'));
		$query_psname		->orderBy('ps.name_of_police_station', 'ASC');
		$query_psname		->condition('ps.dristrict_code',$value_dist,'=');
		$result_psname 		= $query_psname->execute(); 
		if(!empty($result_psname)) {      
			foreach($result_psname as $rows) {
				$default_pselect[$rows->police_station_code] = t('@psname', array('@psname' => $rows->name_of_police_station));
			}    
		}
		if (isset($all_ps)) {
			return ($default_pselect + $all_ps);
		} else {
			return $default_pselect;
		}
	}