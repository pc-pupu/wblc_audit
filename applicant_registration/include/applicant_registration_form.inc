<?php	
drupal_add_js(drupal_get_path('module', 'applicant_registration').'/js/reg_script.js');
drupal_add_css(drupal_get_path('module', 'applicant_registration').'/css/popup.css');
drupal_add_js(drupal_get_path('module','retrieve_password').'/js/crypto-js.min.js');

function applicant_registration_form($form, &$form_state) {
	global $base_root, $base_path;
	
	$form['#id'] = 'applicant-registration-form';
	
	$form['app_reg']['loginLink'] =	array(
		'#type'     		=> 'markup',
		'#markup'			=> '<div class="content">
									<div class="sky-form">
										<div class="row">'.l('CLICK HERE TO LOGIN', 'applicant-login', array('attributes' => array('html' => TRUE, 'class' => 'button right registration'))).'</div>',
	);		
	
	$form['app_reg']['per_info_h'] = 	array(
		'#type' => 'markup',
		'#markup' => '<fieldset><legend>Personal Information</legend>'
	);									
											
	$form['app_reg']['f_name'] = 	array(
		'#prefix'			=> '<div class="row"><section class="col col-4"><label class="input">',
		'#suffix'			=> '</label></section>',
		'#title'			=>  'First Name',
		'#type' 			=>  'textfield',
		'#attributes' 		=>  array('placeholder' => 'Enter your First Name'),
		'#required' 		=>  TRUE,
		'#element_validate'	=>  array('DASHBOARD_TEXTBOX_TEXT_VALIDATE')
	);
								
	$form['app_reg']['m_name'] = 	array(
		'#prefix'			=> '<section class="col col-4"><label class="input">',
		'#suffix'			=>'</label></section>',
		'#title'			=> 'Middle Name',
		'#type' 			=> 'textfield',
		'#attributes' 		=> array('placeholder' => 'Enter your Middle Name'),
	//'#element_validate'	=> array('DASHBOARD_TEXTBOX_CARDNUMBER_TEXT_VALIDATE')
	);
								
	$form['app_reg']['l_name'] = 	array(
		'#prefix'			=> '<section class="col col-4"><label class="input">',
		'#suffix'			=> '</label></section></div>',
		'#title'			=> 'Last Name',
		'#type' 			=> 'textfield',
		'#attributes' 		=> array('placeholder' => 'Enter your Last Name'),
		'#required' 		=> TRUE,
		'#element_validate'	=> array('DASHBOARD_TEXTBOX_TEXT_VALIDATE')
	);
	
	/*
	$form['app_reg']['dob'] =	array(
		'#prefix'			=> '<div class="row"><section class="col col-6"><label class="select">',
		'#suffix'			=> '</label></section>',
		'#type' 			=> 'date',
		'#title'			=> 'Date Of Birth',
		'#required' 		=> TRUE,
		'#after_build' 		=> array("applicant_forms_format_dob"),
		// '#element_validate' => array('applicant_dob_validate')
	);
	*/
	// $genderOptions =	array('0'=>t('Male'), '1'=>t('Female'), '2'=>t('Transgender'));
		
	/*
	$form['app_reg']['gender'] = 	array(
		'#prefix'			=> '<section class="col col-6"><label class="inline-group">',
		'#suffix'			=>'</label></section></div>',
		'#type' 			=> 'radios',
		'#title' 			=> t('Gender'),
		'#options' 			=> $genderOptions,
		'#required' 		=> TRUE
	);
	*/
	$cardTypeOptions = 	array( ""=>'Select Card Type', 1=>'AADHAR', 2=>'PAN', 3=>'TAN', 4=>'LIN', 5=>'EPIC' );
	$value_dropdown_cardType =	!empty($card_type) ? $card_type : key($cardTypeOptions);
	
	$form['app_reg']['card_type'] = 	array(
		'#prefix'			=> '<div class="row"><section class="col col-6"><label class="select">',
		'#suffix'			=> '</label></section>',
		'#title'			=> 'ID Card Type',
		'#type' 			=> 'select',
		'#options' 			=> $cardTypeOptions,
		'#required' 		=> TRUE,
		'#element_validate'	=> array('DASHBOARD_CARD_TYPE_VALIDATE')
	);
	
	$form['app_reg']['card_number'] = array(
		'#prefix'			=> '<section class="col col-6"><label class="input">',
		'#suffix'			=> '</label></section></div></fieldset>',
		'#title'			=> 'ID Card Number',
		'#type' 			=> 'password',
		'#attributes' 		=> array('placeholder' => 'Enter your Id Card Number'),
		'#id'				=>'card_number_id',
		'#required' 		=> TRUE,
		//'#element_validate' => array('DASHBOARD_SPECIAL_CHAR_TEXT_VALIDATE')
	);
	
	$form['app_reg']['per_info_f'] = 	array(
		'#type' => 'markup',
		'#markup' => '</fieldset>'
	);
	
	$form['app_reg']['con_info_h'] = 	array(
		'#type' => 'markup',
		'#markup' => '<fieldset><legend>Contact Information</legend>'
	);
	
	$form['app_reg']['email'] =	array(
		'#prefix'			=> '<div class="row"><section class="col col-4"><label class="input">',
		'#suffix'			=> '</label>',
		'#type'				=> 'textfield',
		'#title'			=> t('Email'),
		'#required'			=> TRUE,
		'#default_value'	=> (isset($form_state['values']['email'])) ? $form_state['values']['email'] : '',
		'#attributes' 		=> array( 'placeholder'=>'Enter valid email address', 'maxlength' => 255, 'onkeyup' => 'checkEmail(this.val);', 'onblur' => 'checkEmail(this.val);', 'class' => array('email_fld'), 'id' => 'email_id' ),
		'#element_validate' => array('DASHBOARD_EMAIL_VALIDATE')
	);
	
	$form['app_reg']['email_img_markup'] = array(
		'#type' 			=> 'markup', 
		'#markup' 			=> '<span id="common_email_img_div"></span>'
	);
	
	$form['app_reg']['email_markup'] = array(
		'#type' 			=> 'markup', 
		'#markup' 			=> '<span id="common_email_div"></span></section>'
	);
	
	
	$form['app_reg']['phone'] =	array(
		'#prefix'			=> '<section class="col col-4 contact-info-mob"><label class="input">',
		'#suffix'			=> '',
		'#type'				=> 'textfield',
		'#title'			=> t('Mobile Number'),
		'#required'			=> TRUE,
		'#attributes' 		=> array( 'id' => 'phoneId', 'placeholder'=>'Enter 10 digits mobile number', 'maxlength' => 10, 'accept' => 'number'),
		'#element_validate' => array('DASHBOARD_PHONE_VALIDATE')
	);
	
	$form['app_reg']['phone_markup'] = array(
		'#type' 			=> 'markup', 
		'#markup' 			=> '<span id="hint_phone"><img src="'.$base_root.$base_path.'sites/all/modules/applicant_registration/images/tooltip.png"></span>',
		'#suffix'			=> '</label></section>'
	);
	
	$form['app_reg']['contact_number'] =	array(
		'#prefix'			=> '<section class="col col-4"><label class="input">',
		'#suffix'			=> '</label></section></div>',
		'#type'				=> 'textfield',
		'#title'			=> t('Other Contact Number'),
		'#attributes' 		=> array( 'id' => 'phoneId', 'placeholder'=>'Enter any others phone number', 'maxlength' => 12, 'accept' => 'number'),
		'#element_validate' => array('DASHBOARD_CONTACT_NUMBER_VALIDATE')
	);
	
	$form['app_reg']['address'] = array(
		'#prefix'				=> '<section><label class="textarea">',
		'#suffix'				=> '</label></section>',
		'#title'  				=> 'House No./Village Name/Street/Road',
		'#type'   				=> 'textarea',
		'#attributes' 			=> array('placeholder' => 'Enter your Address'),
		'#rows' 				=> 2,
		'#required' 			=> TRUE,
		'#element_validate'		=> array('LOCATION_VALIDATE')
	);
		
	$options_dist = common_custom_user_district_list_new();
		
	$form['app_reg']['dist_code'] =  array(
		'#prefix'			=> '<div class="row"><section class="col col-6"><label class="select">',
		'#suffix'			=> '</label></section>',
		'#title'			=> 'Select District',
		'#type' 			=> 'select',
		'#options' 			=> $options_dist,
		'#required' 		=> TRUE,
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
			
		$form['app_reg']['subdivision'] = array(
			  '#prefix'				=> '<div id="clra_subdvsn_replace"><section class="col col-6"><label class="select">',
			  '#suffix'				=> '</label></section></div>',
			  '#title'				=> 'Select Subdivision',
			  '#type' 				=> 'select',
			  '#options' 			=> custom_user_subdivisionlist_list_new($value_loc_e_dist),
			  '#required' 			=> TRUE,
			  '#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
			  '#ajax'				=> array(
										'event' 			=> 'change',
										'effect' 			=> 'fade',			
										'callback'			=> 'get_ajax_callback_new_1',
										'progress'			=> '',
									  ),
		);
																									
	}else{
			
		$form['app_reg']['subdivision'] = array(
			  '#prefix'				=> '<div id="clra_subdvsn_replace"><section class="col col-6"><label class="select">',
			  '#suffix'				=> '</label></section></div>',
			  '#title'				=> 'Select Subdivision',
			  '#type' 				=> 'select',
			  '#options' 			=> array( '' => '--Select--' ),
			  '#required' 			=> TRUE,
			  '#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
			  '#ajax'				=> array(
										'event' 			=> 'change',
										'effect' 			=> 'fade',			
										'callback'			=> 'get_ajax_callback_new_1',
										'progress'			=> '',
									  ),
		);
	}
	
	$value_loc_e_subdivision = isset($form_state['values']['subdivision']) ? $form_state['values']['subdivision'] : '';
			
	if($value_loc_e_dist != "" && $value_loc_e_subdivision != ""){
		$areatypeOptions = custom_user_areatype_list_new($value_loc_e_dist, $value_loc_e_subdivision);
		$areaTypeData = array( '' => '--Select--' );
		
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
		
		
			
		$form['app_reg']['areatype'] = array(
			'#prefix'				=> '<div id="clra_loc_e_areatype"><section class="col col-6"><label class="select">',
			'#suffix'				=>'</label></section></div>',
			'#title'				=> 'Select Areatype',
			'#type' 				=> 'select',
			'#options' 			=> $areaTypeData,
			'#required' 			=> TRUE,
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
		$form['app_reg']['areatype'] = array(
			'#prefix'				=> '<div id="clra_loc_e_areatype"><section class="col col-6"><label class="select">',
			'#suffix'				=> '</label></section></div>',
			'#title'				=> 'Select Areatype',
			'#type' 				=> 'select',
			'#options' 			=> array( '' => '--Select--' ),
			'#required' 			=> TRUE,
			'#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
			'#ajax'				=> array(
									'event' 			=> 'change',
									'effect' 			=> 'fade',			
									'callback'			=> 'get_ajax_callback_new_1',
									'progress'			=> '',
								  ),
		);
	}
		
	$value_loc_e_areatype = isset($form_state['values']['areatype']) ? $form_state['values']['areatype'] : '';
			
	$form['app_reg']['name_areatype'] = array(
		'#prefix'				=> '<div id="clra_common_1_replace"><section class="col col-6"><label class="select">',
		'#suffix'				=> '</label></section></div>',
		'#title'				=> 'Select Block/Municipality/Corporation/SEZ/Notified Area',
		'#type' 				=> 'select',
		'#options' 			=> array( '' => '--Select--' ),
		'#required' 			=> TRUE,
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
				  '#suffix'				=> '</label></section></div>',
				  '#title'				=> 'Select Block',
				  '#type' 				=> 'select',
				  '#options' 			=> custom_user_block_list_new($value_loc_e_subdivision, $value_loc_e_areatype),
				  '#required' 			=> TRUE,
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
				  '#suffix'				=> '</label></section></div>',
				  '#title'				=> 'Select Municipality',
				  '#type' 				=> 'select',
				  '#options' 			=> custom_user_block_list_new($value_loc_e_subdivision, $value_loc_e_areatype),
				  '#required' 			=> TRUE,
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
					  '#suffix'				=> '</label></section></div>',
					  '#title'				=> 'Select Corporation',
					  '#type' 				=> 'select',
					  '#options' 			=> custom_user_block_list_new($value_loc_e_subdivision,$value_loc_e_areatype),
					  '#required' 			=> TRUE,
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
					'#suffix'				=> '</label></section></div>',
					'#title'				=> 'Select SEZ',
					'#type' 				=> 'select',
					'#options' 				=> custom_user_block_list_new($value_loc_e_subdivision, $value_loc_e_areatype),
					'#required' 			=> TRUE,
					'#element_validate'		=> array('REG_ADDRESS_VALIDATE'),
					//'#id'					=> 'common_selection_1_id',
					'#ajax'					=> array(
						'event' 			=> 'change',
						'effect' 			=> 'fade',			
						'callback'			=> 'get_ajax_callback_new_1',
						'progress'			=> '',
					 )
				);
			}else if($value_loc_e_areatype == 'N'){
			
				$form['app_reg']['name_areatype'] = array(
					'#prefix'				=> '<div id="clra_common_1_replace"><section class="col col-6"><label class="select">',
					'#suffix'				=> '</label></section></div>',
					'#title'				=> 'Select Notified Area',
					'#type' 				=> 'select',
					'#options' 				=> custom_user_block_list_new($value_loc_e_subdivision, $value_loc_e_areatype),
					'#required' 			=> TRUE,
					'#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
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
		
	$value_common_selection_1 = isset($form_state['values']['name_areatype']) ? $form_state['values']['name_areatype'] : '';
			
	$form['app_reg']['vill_ward'] = array(
		'#prefix'				=> '<div id="clra_common_2_replace"><section class="col col-4"><label class="select">',
		'#suffix'				=> '</label></section></div>',
		'#title'				=> 'Select Gram Panchayat/Ward/Sector',
		'#type' 				=> 'select',
		'#options' 			=> array( '' => '--Select--' ),
		'#required' 			=> TRUE,
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
					$form['app_reg']['vill_ward'] = array(
						'#prefix'				=> '<div id="clra_common_2_replace"><section class="col col-4"><label class="select">',
						'#suffix'				=> '</label></section></div>',
						'#title'				=> 'Select Gram Panchayat',
						'#type' 				=> 'select',
						'#options'			=> custom_user_villward_list_new($value_common_selection_1,'V'),
						'#required' 			=> TRUE,
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
							  '#suffix'				=> '</label></section></div>',
							  '#title'				=> 'Select Ward',
							  '#type' 				=> 'select',
							  '#options' 			=> custom_user_villward_list_new($value_common_selection_1,'W'),
							  '#required' 			=> TRUE,
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
						'#suffix'				=> '</label></section></div>',
						'#title'				=> 'Select Ward',
						'#type' 				=> 'select',
						'#options' 			=> custom_user_corporation_list_new($value_common_selection_1),
						'#required' 			=> TRUE,
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
						'#suffix'				=> '</label></section></div>',
						'#title'				=> 'Select SEZ Name',
						'#type' 				=> 'select',
						'#options' 			=> custom_user_villward_list_new($value_common_selection_1,'S'),
						'#required' 			=> TRUE,
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
						'#suffix'				=> '</label></section></div>',
						'#title'				=> 'Select Notified Area',
						'#type' 				=> 'select',
						'#options' 			=> custom_user_villward_list_new($value_common_selection_1,'N'),
						'#required' 			=> TRUE,
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
	
	$value_common_selection_2 = isset($form_state['values']['vill_ward']) ? $form_state['values']['vill_ward'] : '';
			
	$form['app_reg']['ps'] = array(
		'#prefix'				=> '<div id="clra_ps_replace"><section class="col col-4"><label class="select">',
		'#suffix'				=> '</label></section></div>',
		'#title'				=> 'Select Police Station',
		'#type' 				=> 'select',
		'#options' 				=> array( '' => '--Select--' ),
		'#required' 			=> TRUE,
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
					$form['app_reg']['ps'] = array(
					  '#prefix'				=> '<div id="clra_ps_replace"><section class="col col-4"><label class="select">',
					  '#suffix'				=> '</label></section></div>',
					  '#title'				=> 'Select Police Station',
					  '#type' 				=> 'select',
					  //'#id'					=> 'loc_e_ps_id',
					  '#options' 			=> custom_user_ps_list_new($value_loc_e_dist),
					  '#required' 			=> TRUE,
					  '#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
					);
				}
			}
		}
	}
	$form['app_reg']['pin'] = array(
		'#prefix'			=> '<section class="col col-4"><label class="input">',
		'#suffix'			=> '</label></section></div>',
		'#title'			=> 'PIN Code',
		'#type' 			=> 'textfield',
		'#attributes' 		=> array('placeholder' => 'Enter your Area Pin Code', 'maxlength' => 6),
		'#required' 		=> TRUE,
		'#element_validate'	=> array('DASHBOARD_TEXTBOX_NUMERIC_VALIDATE')
	);
	
	$form['app_reg']['con_info_f'] = 	array(
		'#type' => 'markup',
		'#markup' => '</fieldset>'
	);
	
	$form['app_reg']['est_info_h'] = 	array(
		'#type' => 'markup',
		'#markup' => '<fieldset><legend>Establishment Information</legend>'
	);
	$form['app_reg']['est_name'] = array(
		'#prefix' 			=>  '<div class="row"><section class="col col-4"><div id="est_name_check"><label class="input">',
		'#suffix' 			=>  '</label></div></section>',
		'#title'			=>  'Name',
		'#type' 			=>  'textfield',
		'#required' 		=>  TRUE,
		//'#element_validate'	=>	array('REG_EST_VALIDATE')
	);
	
	$form['app_reg']['est_type'] = array(
		'#title' => 'Establishment Type',
		'#type' => 'select',
		'#options' => array('micro' => 'Micro', 'small' => 'Small', 'medium' => 'Medium', 'large' => 'Large'),
		'#required' =>  TRUE,
		'#attributes' => array('id' => '', 'class'=>array('form-control')),
		'#prefix' => '<section class="col col-4">',
		'#suffix' => '</section>'
	);
	
	$form['app_reg']['est_loc'] = array(
		'#prefix' 			=>  '<section class="col col-4"><label class="input">',
		'#suffix' 			=>  '</label></section></div>',
		'#title'			=>  'House No./Village Name/Street/Road',
		'#type' 			=>  'textfield',
		'#required' 		=>  TRUE,
		//'#element_validate'		=> array('LOCATION_SECOND_VALIDATE')
	);
	
	$form['app_reg']['same_as'] = array(
		'#title' 				=> 'Same as Contact Information',
		'#type' 				=> 'checkbox',
		'#prefix' 				=>  '<section>',
		'#suffix' 				=>  '</section><div class="row">',
		'#ajax'					=> array(
				'callback' 	=> 'same_check',
				'wrapper' 	=> '',
				'method'	=> 'replace',
				'effect' 	=> 'fade',
	 	),
	);
														
	$checkbox = !empty($form_state['values']['same_as'])	?	1	:	0;
	
	if($checkbox==1){
			
			$form['app_reg']['est_district'] 				= array(
																	'#prefix' 			=> '<div id="sameas-contact-info-div">',
																	'#suffix'			=> '</div>'
																	);
			$form['app_reg']['est_subdivision'] 			= array(
																	'#prefix' 			=> '<div id="app_subdvsn_replace">',
																	'#suffix'			=> '</div>'
																	);
																	
			$form['app_reg']['est_loc_e_areatype']  		= array(
																	'#prefix' 			=> '<div id="app_loc_e_areatype">',
																	'#suffix'			=> '</div>'
																	);
																	
			$form['app_reg']['est_name_areatype'] 			= array(
																	'#prefix' 			=> '<div id="app_common_1_replace">',
																	'#suffix'			=> '</div>'
																	);
																	
			$form['app_reg']['est_loc_e_vill_ward'] 		= array(
																  '#prefix' 			=> '<div id="app_common_2_replace">',
																  '#suffix'				=> '</div>'
																	);
																	
			$form['app_reg']['est_ps'] 						= array(
																  '#prefix' 			=> '<div id="app_ps_replace">',
																  '#suffix'				=> '</div>'
																	);
																	
			$form['app_reg']['est_pin_number'] 				= array(
																  '#prefix' 			=> '<div id="app_pin_replace">',
																  '#suffix'				=> '</div>'
																);

	}
	if($checkbox == 0){
		$form['app_reg']['est_district'] = array(
			'#prefix' 			=> '<div id="sameas-contact-info-div"><section class="col col-6"><label class="select">',
			'#suffix' 			=> '</label></section></div>',
			'#title'			=> 'Select District' ,
			'#type' 			=> 'select',
			'#options' 			=> $options_dist,
			'#required'			=> TRUE,
			//'#element_validate'	=> array('REG_ESTABLISHMENT_DISTRICT_VALIDATE'),
			'#ajax'				=> array(
										'event' 			=> 'change',
										'effect' 			=> 'fade',
										'callback'			=> 'get_ajax_callback_new_2',
										'progress'			=> '',
									)
		);
														  
		
		$value_est_loc = isset($form_state['values']['est_district']) ? $form_state['values']['est_district'] : '';		
		
		if($value_est_loc != "" || !empty($value_est_loc) || is_numeric($value_est_loc)){
				
			$form['app_reg']['est_subdivision'] = array(
				'#prefix' 			=> '<div id="app_subdvsn_replace"><section class="col col-6"><label class="select">',
				'#suffix' 			=> '</label></section></div>',
				'#title'			=> 'Select Subdivision',
				'#type' 			=> 'select',
				'#options' 			=> custom_user_subdivisionlist_list_new($value_est_loc),
				'#required'			=> TRUE,
				//'#element_validate'	=> array('REG_ESTABLISHMENT_SUBDIVISION_VALIDATE'),
				'#ajax'				=> array(
											'event' 			=> 'change',
											'effect' 			=> 'fade',			
											'callback'			=> 'get_ajax_callback_new_2',
											'progress'			=> '',
									  ),
			);
																									
		}else{
			
			$form['app_reg']['est_subdivision'] = array(
				'#prefix' 			=> '<div id="app_subdvsn_replace"><section class="col col-6"><label class="select">',
				'#suffix' 			=> '<i></i></label></section></div>',
				'#title'				=> 'Select Subdivision',
				'#type' 				=> 'select',
				'#options' 			=> array( '' => '--Select--' ),
				'#required'			=> TRUE,
				//'#element_validate'	=> array('REG_ESTABLISHMENT_SUBDIVISION_VALIDATE'),
				'#ajax'				=> array(
										'event' 			=> 'change',
										'effect' 			=> 'fade',			
										'callback'			=> 'get_ajax_callback_new_2',
										'progress'			=> '',
									  ),
			);
				
		}
		
		$value_est_subdivision 	= isset($form_state['values']['est_subdivision']) ? $form_state['values']['est_subdivision'] : '';
		
		if($value_est_loc != "" && $value_est_subdivision != ""){
			
			$est_areatypeOptions = custom_user_areatype_list_new($value_est_loc, $value_est_subdivision);
			$est_areaTypeData = array( '' => '--Select--' );
			
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
			
			$form['app_reg']['est_loc_e_areatype'] = array(
							  '#prefix' 			=> '<div id="app_loc_e_areatype"><section class="col col-6"><label class="select">',
							  '#suffix' 			=> '<i></i></label></section></div>',
							  '#title'				=> 'Select area type',
							  '#type' 				=> 'select',
							  '#options' 			=> $est_areaTypeData,
							  '#required'			=> TRUE,
							  //'#element_validate'	=> array('REG_ESTABLISHMENT_LOC_E_AREATYPE_VALIDATE'),
							  '#ajax'				=> array(
														'event' 			=> 'change',
														'effect' 			=> 'fade',			
														'callback'			=> 'get_ajax_callback_new_2',
														'progress'			=> '',
													  ),
								);
																											
		}else{
			
			$form['app_reg']['est_loc_e_areatype'] = array(
							  '#prefix' 			=> '<div id="app_loc_e_areatype"><section class="col col-6"><label class="select">',
							  '#suffix' 			=> '<i></i></label></section></div>',
							  '#title'				=> 'Select area type',
							  '#type' 				=> 'select',
							  '#options' 			=> array( '' => '--Select--' ),
							  '#required'			=> TRUE,
							  //'#element_validate'	=> array('REG_ESTABLISHMENT_LOC_E_AREATYPE_VALIDATE'),
							  '#ajax'				=> array(
														'event' 			=> 'change',
														'effect' 			=> 'fade',			
														'callback'			=> 'get_ajax_callback_new_2',
														'progress'			=> '',
													  ),
								);
		}
		
		$value_est_loc_e_areatype = isset($form_state['values']['est_loc_e_areatype']) ? $form_state['values']['est_loc_e_areatype'] : '';
		
		$form['app_reg']['est_name_areatype'] = array(
												 '#prefix' 				=> '<div id="app_common_1_replace"><section class="col col-6"><label class="select">',
												 '#suffix' 				=> '<i></i></label></section></div>',
												 '#title'				=> 'Select Block/Municipality/Corporation/SEZ/Notified Area',
												 '#type' 				=> 'select',
												 '#options' 			=> array( '' => '--Select--' ),
												 '#required'			=> TRUE,
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
					$form['app_reg']['est_name_areatype'] = array(
									  '#prefix' 			=> '<div id="app_common_1_replace"><section class="col col-6"><label class="select">',
									  '#suffix' 			=> '<i></i></label></section></div>',
									  '#title'				=> 'Select Block',
									  '#type' 				=> 'select',
									  '#options' 			=> custom_user_block_list_new($value_est_subdivision, $value_est_loc_e_areatype),
									  '#required'			=> TRUE,
									  //'#element_validate'	=> array('REG_ESTABLISHMENT_NAME_AREATYPE_VALIDATE'),
									  '#ajax'				=> array(
																'event' 			=> 'change',
																'effect' 			=> 'fade',			
																'callback'			=> 'get_ajax_callback_new_2',
																'progress'			=> '',
															  )
									);
				}else if($value_est_loc_e_areatype == 'M'){
				
					$form['app_reg']['est_name_areatype'] 	= array(
									  '#prefix' 			=> '<div id="app_common_1_replace"><section class="col col-6"><label class="select">',
									  '#suffix' 			=> '<i></i></label></section></div>',
									  '#title'				=> 'Select Municipality',
									  '#type' 				=> 'select',
									  '#options' 			=> custom_user_block_list_new($value_est_subdivision, $value_est_loc_e_areatype),
									  '#required'			=> TRUE,
									  //'#element_validate'	=> array('REG_ESTABLISHMENT_NAME_AREATYPE_VALIDATE'),
									  '#ajax'				=> array(
																'event' 			=> 'change',
																'effect' 			=> 'fade',			
																'callback'			=> 'get_ajax_callback_new_2',
																'progress'			=> '',
															  )
									);
				}else if($value_est_loc_e_areatype == 'C'){
		
					$form['app_reg']['est_name_areatype'] 	= array(
									  '#prefix' 			=> '<div id="app_common_1_replace"><section class="col col-6"><label class="select">',
									  '#suffix' 			=> '<i></i></label></section></div>',
									  '#title'				=> 'Select Corporation',
									  '#type' 				=> 'select',
									  '#options' 			=> custom_user_block_list_new($value_est_subdivision, $value_est_loc_e_areatype),
									  '#required'			=> TRUE,
									  //'#element_validate'	=> array('REG_ESTABLISHMENT_NAME_AREATYPE_VALIDATE'),
									  '#ajax'				=> array(
																'event' 			=> 'change',
																'effect' 			=> 'fade',			
																'callback'			=> 'get_ajax_callback_new_2',
																'progress'			=> '',
															  )
								);
				}else if($value_est_loc_e_areatype == 'S'){
		
					$form['app_reg']['est_name_areatype'] 	= array(
																  '#prefix' 			=> '<div id="app_common_1_replace"><section class="col col-6"><label class="select">',
																  '#suffix' 			=> '<i></i></label></section></div>',
																  '#title'				=> 'Select SEZ',
																  '#type' 				=> 'select',
																  '#options' 			=> custom_user_block_list_new($value_est_subdivision, $value_est_loc_e_areatype),
																  '#required'			=> TRUE,
																  //'#element_validate'	=> array('REG_ESTABLISHMENT_NAME_AREATYPE_VALIDATE'),
																  '#ajax'				=> array(
																							'event' 			=> 'change',
																							'effect' 			=> 'fade',			
																							'callback'			=> 'get_ajax_callback_new_2',
																							'progress'			=> '',
																						  )
																);
				}else if($value_est_loc_e_areatype == 'N'){
				
					$form['app_reg']['est_name_areatype']  = array(
																  '#prefix' 			=> '<div id="app_common_1_replace"><section class="col col-6"><label class="select">',
																  '#suffix' 			=> '<i></i></label></section></div>',
																  '#title'				=> 'Select Notified Area',
																  '#type' 				=> 'select',
																  '#options' 			=> custom_user_block_list_new($value_est_subdivision, $value_est_loc_e_areatype),
																  '#required'			=> TRUE,
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
		
		$value_est_name_areatype = isset($form_state['values']['est_name_areatype']) ? $form_state['values']['est_name_areatype'] : '';
		
		$form['app_reg']['est_loc_e_vill_ward'] = array(
						 '#prefix' 				=> '<div id="app_common_2_replace"><section class="col col-4"><label class="select">',
						 '#suffix' 				=> '<i></i></label></section></div>',
						 '#title'				=> 'Select Gram Panchayat/Ward/Sector',
						 '#type' 				=> 'select',
						 '#options' 			=> array( '' => '--Select--' ),
						 '#required'			=> TRUE,
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
							
						$form['app_reg']['est_loc_e_vill_ward'] = array(
																	  '#prefix' 			=> '<div id="app_common_2_replace"><section class="col col-4"><label class="select">',
																	  '#suffix' 			=> '<i></i></label></section></div>',
																	  '#title'				=> 'Select Gram Panchayat '.$star,
																	  '#type' 				=> 'select',
																	  '#options'			=> custom_user_villward_list_new($value_est_name_areatype,'V'),
																	  '#required'			=> TRUE,
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
							
						$form['app_reg']['est_loc_e_vill_ward']  = array(
																	  '#prefix' 			=> '<div id="app_common_2_replace"><section class="col col-4"><label class="select">',
																	  '#suffix' 			=> '<i></i></label></section></div>',
																	  '#title'				=> 'Select Ward',
																	  '#type' 				=> 'select',
																	 // '#options' 			=> custom_user_villward_list_new($value_est_name_areatype,'W'),
																	  '#options'			=> custom_user_villward_list_new($value_est_name_areatype,'W'),
																	  '#required'			=> TRUE,
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
							
						$form['app_reg']['est_loc_e_vill_ward'] = array(
																	  '#prefix' 			=> '<div id="app_common_2_replace"><section class="col col-4"><label class="select">',
																	  '#suffix' 			=> '<i></i></label></section></div>',
																	  '#title'				=> 'Select Ward',
																	  '#type' 				=> 'select',
																	  '#options' 			=> custom_user_corporation_list_new($value_est_name_areatype),
																	  '#required'			=> TRUE,
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
							
						$form['app_reg']['est_loc_e_vill_ward'] = array(
																	  '#prefix' 			=> '<div id="app_common_2_replace"><section class="col col-4"><label class="select">',
																	  '#suffix' 			=> '<i></i></label></section></div>',
																	  '#title'				=> 'Select Sector',
																	  '#type' 				=> 'select',
																	  '#options' 			=> custom_user_villward_list_new($value_est_name_areatype,'S'),
																	  '#required'			=> TRUE,
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
																	  //'#element_validate'	=> array('REG_ADDRESS_VALIDATE'),
																	  '#required'			=> TRUE,
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
		
		$value_est_loc_e_vill_ward 			= isset($form_state['values']['est_loc_e_vill_ward']) ? $form_state['values']['est_loc_e_vill_ward'] : '';
		
		$form['app_reg']['est_ps'] 							= array(
																'#prefix' 				=> '<div id="app_ps_replace"><section class="col col-4"><label class="select">',
																'#suffix' 				=> '<i></i></label></section></div>',
																'#title'				=> 'Select Police Station',
																'#type' 				=> 'select',
																'#options' 				=> array( '' => '--Select--' ),
																'#required'				=> TRUE,
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
						$form['app_reg']['est_ps'] 			= array(
																  '#prefix' 			=> '<div id="app_ps_replace"><section class="col col-4"><label class="select">',
																  '#suffix' 			=> '<i></i></label></section></div>',
																  '#title'				=> 'Select Police Station',
																  '#type' 				=> 'select',
																  '#options' 			=> custom_user_ps_list_new($value_est_loc),
																  '#required'			=> TRUE,
																  //'#element_validate'	=> array('REG_ESTABLISHMENT_POLICE_VALIDATE')
															  );
					}
				}
			}
		}
		
		
		$form['app_reg']['est_pin_number'] = array(
		  '#prefix' 			=> '<div id="app_pin_replace"><section class="col col-4"><label class="input">',
		  '#suffix' 			=> '</label></section></div></div>',
		  '#title'				=> 'Pin Code',
		  '#type' 				=> 'textfield',
		  '#attributes' 		=> array('placeholder' => 'Enter your Area Pin Code', 'maxlength' => 6),
		  '#required' 			=> TRUE,
		  '#element_validate'	=> array('EST_TEXTBOX_NUMERIC_VALIDATE')
		);
	
	}
	
	$form['app_reg']['est_info_f'] = 	array(
		'#type' => 'markup',
		'#markup' => '</fieldset>'
	);
	
	$form['app_reg']['worker_info_h'] = 	array(
		'#type' => 'markup',
		'#markup' => '<fieldset><legend>Number of workers employed </legend>'
	);
	
	$form['app_reg']['work_man_master_role_male'] = array(
		 '#prefix'				=> '<table width="100%"><tr><td>No of workman of master role / regular</td><td>',
		 '#suffix'				=> '</td>',
		 '#type'				=> 'textfield',
		 '#title'				=> t('Male'),	
		 '#required' 			=>  TRUE,
		 '#default_value'		=> 0,																										
		 '#attributes' 			=> array('maxlength' => 20, 'class' => array('no_of_workmn')),
		 '#element_validate'	=> array('W_NUMERIC_VALIDATE')
	);
	$form['app_reg']['work_man_master_role_female'] = array(
		'#prefix'			=> '<td>',
		'#suffix'			=> '</td>',
		'#type'				=> 'textfield',
		'#title'			=> t('Female'),
		'#default_value'	=> 0,
		'#required' 		=>  TRUE,																													
		'#attributes' 		=> array('maxlength' => 20, 'class' => array('no_of_workmn')),
		'#element_validate'	=> array('W_NUMERIC_FEMALE_VALIDATE')
	);
	
	$form['app_reg']['work_man_master_role_adolescent_male'] = array(
		'#prefix'			=> '<td>',
		'#suffix'			=> '</td>',
		'#type'				=> 'textfield',
		'#title'			=> t('Adolescent Male'),
		'#default_value'	=> 0,
		'#required' 		=>  TRUE,																													
		'#attributes' 		=> array('maxlength' => 20, 'class' => array('no_of_workmn')),
		'#element_validate'	=> array('W_NUMERIC_FEMALE_VALIDATE')
	);
		
	$form['app_reg']['work_man_master_role_adolescent_female'] = array(
		'#prefix'			=> '<td>',
		'#suffix'			=> '</td></tr>',
		'#type'				=> 'textfield',
		'#title'			=> t('Adolescent Female'),
		'#default_value'	=> 0,
		'#required' 		=>  TRUE,																													
		'#attributes' 		=> array('maxlength' => 20, 'class' => array('no_of_workmn')),
		'#element_validate'	=> array('W_NUMERIC_FEMALE_VALIDATE')
	);
	
	$form['app_reg']['contractual_labour_male'] 		= array(
		'#prefix'			=> '<tr><td>No of contractual labour</td><td>',
		'#suffix'			=> '</td>',
		'#type'				=> 'textfield',
		'#title'			=> t('Male'),
		'#required' 		=>  TRUE,
		'#default_value'	=> 0,																													
		'#attributes' 		=> array('maxlength' => 20, 'class' => array('no_of_workmn')),
		'#element_validate'	=> array('CONTRACTUAL_NUMERIC_MALE_VALIDATE')
	);
	
	$form['app_reg']['contractual_labour_female'] 		= array(
		'#prefix'				=> '<td>',
		'#suffix'				=> '</td>',
		'#type'					=> 'textfield',
		'#title'				=> t('Female'),	
		'#default_value'		=> 0,
		'#required' 			=>  TRUE,																												
		'#attributes' 			=> array('maxlength' => 20, 'class' => array('no_of_workmn')),
		'#element_validate'		=> array('CONTRACTUAL_NUMERIC_FEMALE_VALIDATE')
	);
	
	$form['app_reg']['contractual_labour_adolescent_male'] = array(
		'#prefix'			=> '<td>',
		'#suffix'			=> '</td>',
		'#type'				=> 'textfield',
		'#title'			=> t('Adolescent Male'),
		'#default_value'	=> 0,
		'#required' 		=>  TRUE,																													
		'#attributes' 		=> array('maxlength' => 20, 'class' => array('no_of_workmn')),
		'#element_validate'	=> array('W_NUMERIC_FEMALE_VALIDATE')
	);
	
	$form['app_reg']['contractual_labour_adolescent_female'] = array(
		'#prefix'			=> '<td>',
		'#suffix'			=> '</td></tr>',
		'#type'				=> 'textfield',
		'#title'			=> t('Adolescent Female'),
		'#default_value'	=> 0,
		'#required' 		=>  TRUE,																													
		'#attributes' 		=> array('maxlength' => 20, 'class' => array('no_of_workmn')),
		'#element_validate'	=> array('W_NUMERIC_FEMALE_VALIDATE')
	);														
	$form['app_reg']['child_labour_male'] 				= array(
		'#prefix'				=> '<tr><td>No of other worker engaged</td><td>',
		'#suffix'				=> '</td>',
		'#type'					=> 'textfield',
		'#title'				=> t('Male'),
		'#required' 			=>  TRUE,
		'#default_value'		=> 0,																													
		'#attributes' 			=> array('maxlength' => 20, 'class' => array('no_of_workmn')),
		'#element_validate'		=> array('WORKER_OTHER_MALE_NUMERIC_VALIDATE')
	);
	
	$form['app_reg']['child_labour_female'] 			= array(
		'#prefix'				=> '<td>',
		'#suffix'				=> '</td>',
		'#type'					=> 'textfield',
		'#title'				=> t('Female'),	
		'#required' 			=> TRUE,
		'#default_value'		=> 0,																														
		'#attributes' 			=> array('maxlength' => 20, 'class' => array('no_of_workmn')),
		'#element_validate'		=> array('WORKER_OTHER_FEMALE_NUMERIC_VALIDATE')
	);		
	$form['app_reg']['child_labour_adolescent_male'] = array(
		'#prefix'			=> '<td>',
		'#suffix'			=> '</td>',
		'#type'				=> 'textfield',
		'#title'			=> t('Adolescent Male'),
		'#default_value'	=> 0,
		'#required' 		=>  TRUE,																													
		'#attributes' 		=> array('maxlength' => 20, 'class' => array('no_of_workmn')),
		'#element_validate'	=> array('W_NUMERIC_FEMALE_VALIDATE')
	);
	
	$form['app_reg']['child_labour_adolescent_female'] = array(
		'#prefix'			=> '<td>',
		'#suffix'			=> '</td></tr></table>',
		'#type'				=> 'textfield',
		'#title'			=> t('Adolescent Female'),
		'#default_value'	=> 0,
		'#required' 		=>  TRUE,																													
		'#attributes' 		=> array('maxlength' => 20, 'class' => array('no_of_workmn')),
		'#element_validate'	=> array('W_NUMERIC_FEMALE_VALIDATE')
	);	
	
	
	$form['app_reg']['worker_info_f'] = 	array(
		'#type' => 'markup',
		'#markup' => '</fieldset>'
	);	
		
	$form['app_reg']['login_info_h'] = 	array(
		'#type' => 'markup',
		'#markup' => ''
	);
	
	$form['app_reg']['username'] = array(
		'#prefix'				=> '<fieldset><legend>Login Information</legend><div class="row"><section class="col col-4"><label class="input">',
		'#suffix'				=> '</label>',
		'#type'					=> 'textfield',
		'#title'				=> t('Username'),
		'#required'				=> TRUE,
		'#element_validate'     => array('DASHBOARD_USERNAME_VALIDATE'),
		'#default_value'		=> (isset($form_state['values']['username'])) ? $form_state['values']['username'] : '',
		'#attributes' 			=> array( 'placeholder'=>'Enter username', 'maxlength' => 255, 'onkeyup' => 'checkUserName(this.val);', 'onblur' => 'checkUserName(this.val);', 'class' => array('username_fld'), 'id' => 'username_id')
	);
															
	$form['app_reg']['username_img_markup'] = array(
		'#type' 				=> 'markup', 
		'#markup' 				=> '<span id="common_username_img_div"></span>'
    );
														
	$form['app_reg']['username_markup'] = array(
		'#type' 			=> 'markup', 
		'#markup' 			=> '<span id="common_username_div"></span></section>'
    );
						
	$form['app_reg']['password'] = array(
		'#prefix'			=> '<section class="col col-4 login-info-pass"><label class="input">',
		/*'#suffix'			=> '</label></section>',*/
		'#type'				=> 'password',
		'#title'			=> t('Password'),
		'#size'				=> 20,
		'#required'			=> TRUE,
		'#attributes' 		=> array('id' => 'passwordId', 'placeholder'=>'Enter password', 'onkeyup' => 'setCheckPasswordStrength(this.val);', 'onblur' => 'setCheckPasswordStrength(this.val);', 'class' => array('pwd_fld') )
	);
	
	$form['app_reg']['markup_msg'] = array(
		'#type'				=> 'markup',
		'#markup'			=> '<div id="setPasswordMessage" style="display:none;"></div>'
	);
	
														
	$form['app_reg']['password_markup'] = array(
		'#type' 			=> 'markup', 
		'#markup' 			=> '<span id="hint"><img src="'.$base_root.$base_path.'sites/all/modules/applicant_registration/images/tooltip.png"></span>',
		'#suffix'			=> '</label></section>'
    );
						
	$form['app_reg']['conpassword'] = array(
		'#prefix'			=> '<section class="col col-4"><label class="input">',
		'#suffix'			=> '</label></section><br class="clear">',
		'#type'				=> 'password',
		'#title'			=> t('Confirm Password'),
		'#size'				=> 20,
		'#required'			=> TRUE,
		'#attributes' 		=> array('id' => 'conpasswordId', 'placeholder'=>'Enter password', 'onkeyup' => 'setCheckPasswordStrength(this.val);', 'onblur' => 'setCheckPasswordStrength(this.val);', 'class' => array('pwd_fld') )
	);
		
	$form['app_reg']['security_code'] = array(
			'#prefix'			=> '<section class="col col-12 custom-cap-design">',
			'#suffix'			=> '</section></div>',
			'#type'				=> 'captcha',
	);
		
	$form['app_reg']['s_val'] = array('#type' => 'hidden','#attributes' => array('id' => array('s_val')),
		'#size' => 60,
		'#maxlength' => 125,
		'#required' => TRUE,
	);
	$form['app_reg']['ct_val'] = array('#type' => 'hidden','#attributes' => array('id' => array('ct_val')),
		'#size' => 60,
		'#maxlength' => 125,
		'#required' => TRUE,
	); 
	
	
	$form['app_reg']['s_val_con'] = array('#type' => 'hidden','#attributes' => array('id' => array('s_val_con')),
		'#size' => 60,
		'#maxlength' => 125,
		'#required' => TRUE,
	);
	$form['app_reg']['ct_val_con'] = array('#type' => 'hidden','#attributes' => array('id' => array('ct_val_con')),
		'#size' => 60,
		'#maxlength' => 125,
		'#required' => TRUE,
	); 
	
	$form['app_reg']['s_val_cn'] = array('#type' => 'hidden','#attributes' => array('id' => array('s_val_cn')),
		'#size' => 60,
		'#maxlength' => 125,
		'#required' => TRUE,
	);
	$form['app_reg']['ct_val_cn'] = array('#type' => 'hidden','#attributes' => array('id' => array('ct_val_cn')),
		'#size' => 60,
		'#maxlength' => 125,
		'#required' => TRUE,
	); 
	
	$form['app_reg']['login_info_f'] = 	array(
		'#type' => 'markup',
		'#markup' => '</fieldset>'
	);		
	
													
	$form['app_reg']['submit_button'] = array(
		'#prefix'				=> '',
		'#suffix'				=>'</div></div>',
		'#attributes' 			=> array('class' => array('button right'),'OnClick' => 'return pwd_handler(this);'), 
		'#type'					=>'submit',
		'#value'				=>t('SUBMIT')
	);
	
	return $form;
}

function same_check($form, $form_state) {
		$commands 	= array();
		$commands[] = ajax_command_replace('#sameas-contact-info-div', drupal_render($form['app_reg']['est_district']));
		$commands[] = ajax_command_replace('#app_subdvsn_replace', drupal_render($form['app_reg']['est_subdivision']));
		$commands[] = ajax_command_replace('#app_loc_e_areatype', drupal_render($form['app_reg']['est_loc_e_areatype']));
		$commands[] = ajax_command_replace('#app_common_1_replace', drupal_render($form['app_reg']['est_name_areatype']));
		$commands[] = ajax_command_replace('#app_common_2_replace', drupal_render($form['app_reg']['est_loc_e_vill_ward']));
		$commands[] = ajax_command_replace('#app_ps_replace', drupal_render($form['app_reg']['est_ps']));
		$commands[] = ajax_command_replace('#app_pin_replace', drupal_render($form['app_reg']['est_pin_number']));
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
	
	$key = "Lc#Nic@1720";
	$card_number = cryptoJsAesDecrypt($key,$form_state['values']['card_number']);
	
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
	 
	 /*$validateEmail					=	db_query("select mail from users where mail=:mailval", array(':mailval'=> $form_state['values']['email']));
	 if( $validateEmail->rowCount() > 0 ){
		form_error($element, t($element['#title'].' already exists.'));
		$form_state['rebuild'] = TRUE;
	}*/
	//$form_state['rebuild']	=	true;
}

function DASHBOARD_USERNAME_VALIDATE($element, &$form_state){
 $username 	=	trim($form_state['values']['username']);
 if(strlen($username) >3){
	 $match_pattern = "/^[a-zA-Z0-9.@_-]*$/";
	 if( $username == "" ){
		form_error($element, t($element['#title'].' cannot be empty.')); 
	 }
	 if (!preg_match($match_pattern, $username)) {
		form_error($element, t($element['#title'].' format invalid. Username will allow only Character, Number, @(at), .(dot), -(hyphen), and _(underscore).'));
	 }
	
	 }else{
		form_error($element, t($element['#title'].' must be longer than 4 characters'));
 }
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

function W_NUMERIC_VALIDATE($element, &$form_state){
	if(!is_numeric($form_state['values']['work_man_master_role_male'])){
		form_error($element, 'Workman master role '.t($element['#title'].' should be numeric.'));
	}
}

function W_NUMERIC_FEMALE_VALIDATE($element, &$form_state){	
	if(!is_numeric($form_state['values']['work_man_master_role_female'])){
		form_error($element, 'Workman master role '.t($element['#title'].' should be numeric.'));
	}
}

function CONTRACTUAL_NUMERIC_MALE_VALIDATE($element, &$form_state){	
	if(!is_numeric($form_state['values']['contractual_labour_male'])){
		form_error($element, 'Contractual Labour '.t($element['#title'].' should be numeric.'));
	}
}

function CONTRACTUAL_NUMERIC_FEMALE_VALIDATE($element, &$form_state){	
	if(!is_numeric($form_state['values']['contractual_labour_female'])){
		form_error($element, 'Contractual Labour '.t($element['#title'].' should be numeric.'));
	}
	
	if($form_state['values']['contractual_labour_female']==''){
		form_error($element, 'Contractual Labour '.t($element['#title'].' should not be empty.'));
	}
}

function WORKER_OTHER_MALE_NUMERIC_VALIDATE($element, &$form_state){	
	if(!is_numeric($form_state['values']['child_labour_male'])){
		form_error($element, 'Other Worker '.t($element['#title'].' should be numeric.'));
	}
}

function WORKER_OTHER_FEMALE_NUMERIC_VALIDATE($element, &$form_state){	
	if(!is_numeric($form_state['values']['child_labour_female'])){
		form_error($element, 'Other Worker '.t($element['#title'].' should be numeric.'));
	}
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
	if( $form_state['values']['contact_number'] != "" && is_numeric($form_state['values']['contact_number']) ){
		$contact_number	=	$form_state['values']['contact_number'];
	}else{
		$contact_number	=	0;
	}
		
	$time						=	time();
	$date						=	date('Y-m-d');
	$tempAckNumber				=	$time."-".$date;
	$est_name					= $form_state['values']['est_name'];
	$est_loc					= $form_state['values']['est_loc'];
	$est_type					= $form_state['values']['est_type'];
	
	
	$s_val				= $form_state['values']['s_val'];
	$ct_val				= $form_state['values']['ct_val'];
	
	$s_val_con			= $form_state['values']['s_val_con'];
	$ct_val_con			= $form_state['values']['ct_val_con'];
	
	$s_val_cn			= $form_state['values']['s_val_cn'];
	$ct_val_cn			= $form_state['values']['ct_val_cn'];
		
	$password_en = '{"ct":"'.$ct_val.'","iv":"'.trim($form_state['values']['password']).'","s":"'.$s_val.'"}';
	$confirm_password_en = '{"ct":"'.$ct_val_con.'","iv":"'.trim($form_state['values']['conpassword']).'","s":"'.$s_val_con.'"}';
	
	$card_number_en = '{"ct":"'.$ct_val_cn.'","iv":"'.trim($form_state['values']['card_number']).'","s":"'.$s_val_cn.'"}';
	
	$key = "Lc#Nic@1720";
	$password = cryptoJsAesDecrypt($key,$password_en); // cryptoJsAesDecrypt from user.module file 
	
	$confirm_password = cryptoJsAesDecrypt($key,$confirm_password_en);
	
	$card_number = cryptoJsAesDecrypt($key,$card_number_en);
	
	
	if($password != $confirm_password){
		form_error($form['password'], t('Password and Confirm Password did not match.Please re-enter.'));
	}
	
		
	if($form_state['values']['same_as']== 1){
		
		$contact_info_district 	= $form_state['values']['dist_code'];
		$contact_info_subdiv 	= $form_state['values']['subdivision'];
		$contact_info_areatype 	= $form_state['values']['areatype'];
		$contact_info_areacode 	= $form_state['values']['name_areatype'];
		$contact_info_gpward 	= $form_state['values']['vill_ward'];
		$contact_info_ps 		= $form_state['values']['ps'];
		$contact_info_pin 		= $form_state['values']['pin'];
		
	}else{
		
		$contact_info_district 	= $form_state['values']['est_district'];
		$contact_info_subdiv 	= $form_state['values']['est_subdivision'];
		$contact_info_areatype 	= $form_state['values']['est_loc_e_areatype'];
		$contact_info_areacode 	= $form_state['values']['est_name_areatype'];
		$contact_info_gpward 	= $form_state['values']['est_loc_e_vill_ward'];
		$contact_info_ps 		= $form_state['values']['est_ps'];
		$contact_info_pin 		= $form_state['values']['est_pin_number']; 
	}
	

	
	$sql		=	db_query('select * from l_clra_registration_master where e_name=:e_name and est_type=:est_type and loc_e_name=:loc_e_name and loc_e_dist=:loc_e_dist and loc_e_subdivision=:loc_e_subdivision and name_areatype=:name_areatype and loc_e_areatype=:loc_e_areatype and loc_e_vill_ward=:loc_e_vill_ward and l_e_ps=:l_e_ps and loc_e_pin_number=:loc_e_pin_number', array(':e_name'=> $est_name,':est_type'=> $est_type,':loc_e_name'=> $est_loc,':loc_e_dist'=> $contact_info_district,':loc_e_subdivision'=> $contact_info_subdiv,':loc_e_areatype'=> $contact_info_areatype,':name_areatype'=> $contact_info_areacode,':loc_e_vill_ward'=> $contact_info_gpward,':l_e_ps'=> $contact_info_ps,':loc_e_pin_number'=> $contact_info_pin));
	
	$result = $sql->fetchObject();
	
	if (empty($result)){
		
		$user_name 			=   $form_state['values']['username'];
		if(!empty($user_name)) {
			$check_usernme_res = checkScriptNameExist($user_name);
		}
		
		if ($check_usernme_res == "DoesNotExist"){
			
			$newUser 					= 	array(
										'name' 		=> trim($form_state['values']['username']),
										'pass' 		=> $password, 
										'mail' 		=> trim($form_state['values']['email']),
										'status' 	=> 1,
										'init' 		=> trim($form_state['values']['email']),
									);
			user_save(null, $newUser);
			
			$txtValue					=	trim($form_state['values']['username']); 
			$fetch_info_querys 			= 	db_select('users', 'U');
			$fetch_info_querys			->	fields('U',  array('uid'));
			$fetch_info_querys			->	condition('U.name',$txtValue);
			$fetch_info_querys_result 	= 	$fetch_info_querys->execute()->fetchObject();
			$lastId						=	$fetch_info_querys_result->uid;  
			$userInfo 					= 	array(
												'uid' 			=> $lastId,
												'fullname'		=> $form_state['values']['f_name']." ".$form_state['values']['l_name'],
												'uid_phone' 	=> $form_state['values']['phone'],
												'pass_wd' 		=> $password, 
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
														// 'dob' =>	$form_state['values']['dob']['year']."-".$form_state['values']['dob']['month']."-".$form_state['values']['dob']['day'],
														'dob'							=>	date('1111-11-11'),
														'email'							=>	$form_state['values']['email'], 
														'mobile'						=>	$form_state['values']['phone'],
														  
														// 'gender'						=>	$form_state['values']['gender'], 
														'gender'						=>	0, 
														'address'						=>	$form_state['values']['address'],
														
														'dist_code'						=>	!empty($form_state['values']['dist_code']) ? $form_state['values']['dist_code'] : '',
														'subdivision'					=>	!empty($form_state['values']['subdivision']) ? $form_state['values']['subdivision'] : 0,
														'areatype'						=>	!empty($form_state['values']['areatype']) ? $form_state['values']['areatype']: '' ,
														'name_areatype'					=>	!empty($form_state['values']['name_areatype']) ? $form_state['values']['name_areatype']: 0 ,
														'vill_ward'						=>	!empty($form_state['values']['vill_ward']) ? $form_state['values']['vill_ward'] : 0 ,
														'ps'							=>	!empty($form_state['values']['ps']) ? $form_state['values']['ps'] : '' ,
														'pin'							=>	!empty($form_state['values']['pin']) ? $form_state['values']['pin'] : 0,
														
														'est_name'						=>	!empty($est_name) ? $est_name : '',
														'est_type'						=>	!empty($est_type) ? $est_type : '',
														'est_loc'						=>	!empty($est_loc) ? $est_loc : '',
														'est_district'					=>	!empty($contact_info_district) ? $contact_info_district : '',
														'est_subdivision'				=>	!empty($contact_info_subdiv) ? $contact_info_subdiv : 0,
														'est_loc_e_areatype'			=>	!empty($contact_info_areatype) ? $contact_info_areatype : '',
														'est_name_areatype'				=>	!empty($contact_info_areacode) ? $contact_info_areacode : 0,
														'est_loc_e_vill_ward'			=>	!empty($contact_info_gpward) ? $contact_info_gpward : 0,
														'est_ps'						=>	!empty($contact_info_ps) ? $contact_info_ps : '',
														'est_pin_number'				=>	!empty($contact_info_pin) ? $contact_info_pin : 0,
														
														'card_type'						=>	$form_state['values']['card_type'],
														'card_number'					=>	$card_number,
														'status'						=> 1,
														'oswicsapplicationflag'			=> 5, /* START DATE 07-01-2019 */
														'contact_number'				=>  $contact_number,
														
														'work_man_master_role_male'		=>	!empty($form_state['values']['work_man_master_role_male']) ? $form_state['values']['work_man_master_role_male'] : 0,
														'work_man_master_role_female'	=>	!empty($form_state['values']['work_man_master_role_female']) ? $form_state['values']['work_man_master_role_female'] : 0,
														'contractual_labour_male'		=>	!empty($form_state['values']['contractual_labour_male']) ? $form_state['values']['contractual_labour_male'] : 0,
														'contractual_labour_female'		=>	!empty($form_state['values']['contractual_labour_female']) ? $form_state['values']['contractual_labour_female'] : 0,
														'child_labour_male'				=>	!empty($form_state['values']['child_labour_male']) ? $form_state['values']['child_labour_male'] : 0,
														'child_labour_female' 			=> !empty($form_state['values']['child_labour_female']) ? $form_state['values']['child_labour_female'] : 0,															
														'work_man_master_role_adolescent_male' => !empty($form_state['values']['work_man_master_role_adolescent_male']) ? $form_state['values']['work_man_master_role_adolescent_male'] : 0,
														'work_man_master_role_adolescent_female' =>	!empty($form_state['values']['work_man_master_role_adolescent_female']) ? $form_state['values']['work_man_master_role_adolescent_female'] : 0,
														'contractual_labour_adolescent_male' =>	!empty($form_state['values']['contractual_labour_adolescent_male']) ? $form_state['values']['contractual_labour_adolescent_male'] : 0,
														'contractual_labour_adolescent_female' =>	!empty($form_state['values']['contractual_labour_adolescent_female']) ? $form_state['values']['contractual_labour_adolescent_female'] : 0,
														'child_labour_adolescent_male' =>	!empty($form_state['values']['child_labour_adolescent_male']) ? $form_state['values']['child_labour_adolescent_male'] : 0,
														'child_labour_adolescent_female' =>	!empty($form_state['values']['child_labour_adolescent_female']) ? $form_state['values']['child_labour_adolescent_female'] : 0
													);
	
			db_insert('l_common_application_master')->fields($fieldsCommonAppInfo)->execute();				
			
			$phone	=	trim($form_state['values']['phone']);
			if(!empty($phone)){
				
				$template_id = 1107161520430394545;	
				$msg	=	"Thank you ".str_replace('&', 'and', trim($form_state['values']['f_name'])." ".trim($form_state['values']['l_name']))." for registering on Labour Commissionerate. Please keep the username safely and change your password at regular intervals. Your Username- ".trim($form_state['values']['username'])." Labour Commissionerate, WB";
				send_sms_for_user_alert($phone, $msg, $template_id);
				send_sms_for_user_alert('7603091500', $msg);
			}
			
			// send mail start
			
			$mailto  = trim($form_state['values']['email']);
			$subject = 'Welcome to Labour Commissionerate, Govt. Of West Bengal';
			
			$message = 'Hello <strong>'.trim($form_state['values']['f_name']).'</strong>,<br><br>You have successfully registered for an account at wblc.gov.in. Please keep the username safely and change your password at regular intervals.<br><br><br> <u>Here is your information</u><br><br><strong>Username</strong>  '.trim($form_state['values']['username']).'<br> You can access this link to login wblc.gov.in/applicant-login<br><br><br>Thank you';
			
			send_mail_for_user_alert($mailto, $subject, $message);
			drupal_set_message('Registration Successfully. Now enter your username and password for login.');
			drupal_goto('applicant-login');	
		}else{
			drupal_set_message('Username already exists', 'error');
		}
	}else{
		drupal_set_message('Establishment already registered in the same location', 'error'); exit;
	}
				
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
	$query_areatype				->condition('bmm.is_active', 'Y');
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
	$query_block			->condition('blm.is_active','Y');
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
	$default_villwardselect = array(""=>"- Select -");
	$all_villwards = array();
	$query_villwardkname = db_select('village_ward_master', 'vwm');
	$query_villwardkname->fields('vwm',array('village_code','village_name', 'category'));
	
	if($value_villwardtype == 'W'){
		$query_villwardkname->orderBy('vwm.ord', 'ASC');
	}else{
		$query_villwardkname->orderBy('vwm.village_name', 'ASC');
	}

	$query_villwardkname->condition('vwm.block_code', trim($value_villwared),'=');
	$query_villwardkname->condition('vwm.category', trim($value_villwardtype), '=');		
	
	$result_villwardkname = $query_villwardkname->execute(); 
	if(!empty($result_villwardkname)) {      
		 foreach($result_villwardkname as $rows) {
			if($rows->category == 'W'){
				$all_villwards[$rows->village_code] = t('@villwardkname', array('@villwardkname' => 'Ward-'.$rows->village_name));	
			}else{
				$all_villwards[$rows->village_code] = t('@villwardkname', array('@villwardkname' => ucwords($rows->village_name)));
			}
			/*if($rows->category == 'V'){
				$all_villwards[$rows->village_code] = t('@villwardkname', array('@villwardkname' => $rows->village_name));	
			}else{
				$all_villwards[$rows->village_code] = t('@villwardkname', array('@villwardkname' => $rows->village_name));
			}*/
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