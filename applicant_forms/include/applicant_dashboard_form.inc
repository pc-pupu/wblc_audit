<?php

	drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicant_dashboard.css');
	
	function applicant_dashboard_form($form, &$form_state) {
		
		global $base_root,$base_path,$user;
		
		$_SESSION['uid'] 	= $user->uid;
		$fetch_user_det 	= db_query("select tempacknumber from l_user_additional_reg_details where uid=:user_id",array(':user_id'=>$_SESSION['uid']));
		if( $fetch_user_det->rowCount() > 0 ){
			foreach($fetch_user_det as $obj1){
				$_SESSION['tempacknumber']		=	$obj1->tempacknumber;
			}
		}
		
		$fetch_common_det 	= db_query("select f_name, m_name, l_name, dob, email, mobile, gender, address, district, block, vil_ward_borough, pin, card_type, card_number from l_common_application_master where user_id=:userId",array(':userId'=>$_SESSION['uid']));
		if( $fetch_common_det->rowCount() > 0 ){
			foreach($fetch_common_det as $obj){
				$f_name				=	$obj->f_name;
				$m_name				=	$obj->m_name;
				$l_name				=	$obj->l_name;
				$dob				=	$obj->dob;
				$email				=	$obj->email;
				$mobile				=	$obj->mobile;
				$gender				=	$obj->gender;
				$address			=	$obj->address;
				$district			=	$obj->district;
				$block				=	$obj->block;
				$vil_ward_borough	=	$obj->vil_ward_borough;
				$pin				=	$obj->pin;
				$card_type			=	$obj->card_type;
				$card_number		=	$obj->card_number;
			}
		}

		
		drupal_add_library('system','ui.datepicker');
		
		drupal_add_js('jQuery(document).ready(function(){jQuery( \'#dobId\' ).datepicker({
     			 				dateFormat  : "yy-mm-dd",
   								changeMonth : true,
     			 				changeYear  : true,
     			 				autoSize    : true,
								yearRange	: "-100:+0",
								maxDate		: new Date()
   			                });});', 'inline');
						
		$form['dashboard_master'] = array(
								  '#title' => t('<div class="heading">My Application Profile</div>'),
								  '#type' => 'fieldset',
								  '#attributes' => array('class'=>array('hello')),
								  '#collapsed' => TRUE,
								  '#collapsable'=>TRUE
          						);
								
		$form['dashboard_master']['common_app_number'] = array(
								  '#prefix'=> '<table class="applicant_dashboard" class="" border="0" width="100%"><tr><td>',
								  '#suffix'=>'</td></tr>',
								  '#title'=>'<span class="loginbox">Common App Number</span>',
								  '#size'=>t('20'),	
								  '#type' => 'hidden',
								  '#default_value' =>isset($_SESSION['tempacknumber']) ? $_SESSION['tempacknumber'] : '',
								  '#attributes' => array('readonly'=>'readonly'),
								  '#disabled' => TRUE
					   			);
								
		$form['dashboard_master']['f_name'] = array(
								  '#prefix'=> '<tr><td>',
								  '#suffix'=>'</td></tr>',
								  '#title'=>'<span class="">First Name</span>',
								  '#size'=>t('20'),	
								  '#type' => 'textfield',
								  '#default_value' =>!empty($f_name) ? $f_name : '',
								  '#attributes' => array('placeholder' => 'Enter your First Name'),
								  '#required' => TRUE,
								  '#element_validate'=> array('DASHBOARD_TEXTBOX_TEXT_VALIDATE')
					   			);
								
		$form['dashboard_master']['m_name'] = array(
								  '#prefix'=> '<tr><td>',
								  '#suffix'=>'</td></tr>',
								  '#title'=>'<span class="">Middle Name</span>',
								  '#size'=>t('20'),	
								  '#type' => 'textfield',
								  '#default_value' =>!empty($m_name) ? $m_name : '',
								  '#attributes' => array('placeholder' => 'Enter your Middle Name'),
								  '#element_validate'=> array('DASHBOARD_TEXTBOX_EMPTY_TEXT_VALIDATE')
					   			);
								
		$form['dashboard_master']['l_name'] = array(
								  '#prefix'=> '<tr><td>',
								  '#suffix'=>'</td></tr>',
								  '#title'=>'<span class="">Last Name</span>',
								  '#size'=>t('20'),	
								  '#type' => 'textfield',
								  '#default_value' =>!empty($l_name) ? $l_name : '',
								  '#attributes' => array('placeholder' => 'Enter your Last Name'),
								  '#required' => TRUE,
								  '#element_validate'=> array('DASHBOARD_TEXTBOX_TEXT_VALIDATE')
					   			);
								
		$form['dashboard_master']['dob'] = array(
								  '#prefix'=> '<tr><td>',
								  '#suffix'=>'</td></tr>',
								  '#title'=>'<span class="">Date Of Birth</span>',
								  '#size'=>t('20'),	
								  '#type' => 'textfield',
								  '#attributes' => array('id' => 'dobId', 'readonly'=>'readonly', 'placeholder' => 'Enter your Date of Birth'),
								  '#default_value' =>!empty($dob) ? $dob : '',
								  '#required' => TRUE
					   			);
								
		$form['dashboard_master']['email'] = array(
								  '#prefix'=> '<tr><td>',
								  '#suffix'=>'</td></tr>',
								  '#title'=>'<span class="">Email</span>',
								  '#size'=>t('20'),	
								  '#type' => 'textfield',
								  '#default_value' =>!empty($email) ? $email : '',
								  '#attributes' => array('placeholder' => 'Enter your Email'),
								  '#required' => TRUE,
								  '#element_validate'=> array('DASHBOARD_TEXTBOX_EMAIL_VALIDATE')
					   			);
								
		$form['dashboard_master']['mobile'] = array(
								  '#prefix'=> '<tr><td>',
								  '#suffix'=>'</td></tr>',
								  '#title'=>'<span class="">Mobile</span>',
								  '#size'=>t('20'),	
								  '#type' => 'textfield',
								  '#default_value' =>!empty($mobile) ? $mobile : '',
								  '#attributes' => array('placeholder' => 'Enter your 10 digit Mobile Number', 'maxlength' => 10),
								  '#required' => TRUE,
								  '#element_validate' => array('DASHBOARD_TEXTBOX_CONTACT_NUMBER_VALIDATE')
					   			);
								
		$genderOptions 			=	array('0'=>t('Male'),'1'=>t('Female'));
		
		$form['dashboard_master']['gender'] = array(
								  '#prefix'=> '<tr><td>',
								  '#suffix'=>'</td></tr>',
								  '#type' => 'radios',
								  '#title' => t('Gender'),
								  '#options' => $genderOptions,
								  '#default_value' => isset($gender) ? $gender : 1,
								  '#required' => TRUE
								);
								
		$form['dashboard_master']['address'] = array(
								  '#prefix' => '<tr><td>',
								  '#suffix' => '</td></tr>',
								  '#title'  => '<span class="">Address</span>',
								  '#size'   => t('20'),	
								  '#type'   => 'textarea',
								  '#default_value' =>!empty($address) ? $address : '',
								  '#attributes' => array('placeholder' => 'Enter your Address'),
								  '#required' => TRUE
					   			);
								
		$options_dist 			= 	ecrc_dist_dropdown_options();
	  	$options_dist 			=  	array(0 => "Select District") + $options_dist ;
	  	$value_dropdown_dist 	= 	isset($form_state['values']['district']) ? $form_state['values']['district'] : key($options_dist);
	    
	   	$form['dashboard_master']['district'] = array(
								  '#prefix'=> '<tr><td>',
								  '#suffix'=>'</td></tr>',
								  '#type' => 'select',
								  '#title' => 'District',
								  '#options' => $options_dist,
								  '#disabled' => TRUE,
								  '#required' => TRUE,
								  '#default_value' => !empty($district) || !empty($district) ? $district : 17, 
								  '#ajax' => array(
										'event' => 'change',
										'callback' => 'establishment_dist_ajax_callback',
										'wrapper' => 'establishment_block_div',
										'method' => 'replace',
										'keypress' => TRUE,
										),
								  '#element_validate'=> array('DASHBOARD_DISTRICT_VALIDATE')
         						);
								
		$value_dropdown_dist	=	17;
		$options_block_one 		=  	array(0 => "--Select--",9999 => "--NONE--");
		$options_block_one		=	$options_block_one+block_municipality_dropdown_function($value_dropdown_dist);
		$value_dropdown_block	=	isset($block) ? $block : key($options_block_one);
		
		$form['dashboard_master']['block'] = array(
								  '#prefix'=> '<tr><td>',
								  '#suffix'=>'</td></tr>',
								  '#type' => 'select',
								  '#title' => 'Block/Muni/Corp.',
								  '#required' => TRUE,
								  '#options' => $options_block_one,
								  '#default_value' => $value_dropdown_block,
								  '#element_validate'=> array('DASHBOARD_BLOCK_VALIDATE')
	  							);
								
		$form['dashboard_master']['vil_ward_borough'] = array(
								  '#prefix'=> '<tr><td>',
								  '#suffix'=>'</td></tr>',
								  '#title'=>'<span class="">Vill./Ward/Borough</span>',
								  '#size'=>t('20'),	
								  '#type' => 'textfield',
								  '#default_value' =>!empty($vil_ward_borough) ? $vil_ward_borough : '',
								  '#attributes' => array('placeholder' => 'Enter your Village/Ward/Borough'),
								  '#required' => TRUE
					   			);
								
		$form['dashboard_master']['pin'] = array(
								  '#prefix'=> '<tr><td>',
								  '#suffix'=>'</td></tr>',
								  '#title'=>'<span class="">Pin</span>',
								  '#size'=>t('20'),	
								  '#type' => 'textfield',
								  '#default_value' =>!empty($pin) ? $pin : '',
								  '#attributes' => array('placeholder' => 'Enter your Area Pin Code', 'maxlength' => 6),
								  '#required' => TRUE,
								  '#element_validate'=> array('DASHBOARD_TEXTBOX_NUMERIC_VALIDATE', 'DASHBOARD_TEXTBOX_NUMERIC_VALIDATE_PIN')
					   			);
		
		$cardTypeOptions 			= 	array(0=>'Select Card Type', 1=>'Aadhar Card', 2=>'Pan Card', 3=>'Tan Card', 4=>'Lin Card', 5=>'Voter Card');
		$value_dropdown_cardType	=	!empty($card_type) ? $card_type : key($cardTypeOptions);
		
		
		$form['dashboard_master']['card_type'] = array(
								  '#prefix'=> '<tr><td>',
								  '#suffix'=>'</td></tr>',
								  '#title'=>'<span class="">Card Type</span>',
								  '#type' => 'select',
								  '#options' => $cardTypeOptions,
								  '#default_value'=> $value_dropdown_cardType,
								  '#required' => TRUE,
								  '#element_validate'=> array('DASHBOARD_CARD_TYPE_VALIDATE')
					   			);
		
		$form['dashboard_master']['card_number'] = array(
		                          '#prefix'=> '<tr><td>',
								  '#suffix'=>'</td></tr>',
								  '#title'=>'<span class="">Card Number</span>',
								  '#type' => 'textfield',
								  '#size'=>t('20'),
								  '#default_value' =>!empty($card_number) ? $card_number : '',
								  '#attributes' => array('placeholder' => 'Enter your Card Number', 'id' => 'card_number_id', 'maxlength' => 20),
								  '#required' => TRUE,
								  '#element_validate' => array('DASHBOARD_SPECIAL_CHAR_TEXT_VALIDATE')
					   			);
								
		$form['submit'] = array (
								  '#prefix'=> '<tr><td>',
								  '#suffix'=>'</td></tr></table>',
								  '#type' => 'submit',
								  '#value' => 'SAVE'
							   );
		return $form; 
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
		$block			=	$val['block'];
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
	
	function DASHBOARD_TEXTBOX_EMPTY_TEXT_VALIDATE($element,&$form_state){
		$match_pattern 	= 	'/^[ A-Za-z ]+$/';
		$val = $form_state['values'];
		$middle_name = $val['m_name']	;
		if(!empty($middle_name)){
			if(!preg_match($match_pattern, $middle_name)) {
				form_error($element, t($element['#title'].' SHOULD BE TEXT ONLY'));
			}
		}
		
	}
	
	function applicant_dashboard_form_validate($form, &$form_state) {
		
		
	}
	
	function applicant_dashboard_form_submit($form, &$form_state){
		global $base_root,$base_path,$user;
		
		$val 					=	$form_state['values'];
		$user_id				=	$user->uid;
		$f_name					=	$val['f_name'];
		$m_name					=	$val['m_name'];
		$l_name					=	$val['l_name'];
		$dob					=	$val['dob'];
		$email					=	$val['email'];
		$mobile					=	$val['mobile'];
		$gender					=	$val['gender'];
		$address				=	$val['address'];
		$district				=	$val['district'];
		$block					=	$val['block'];
		$vil_ward_borough		=	$val['vil_ward_borough'];
		$pin					=	$val['pin'];
		$card_type				=	$val['card_type'];
		$card_number			=	$val['card_number'];
		$status					=	1;
		$common_app_number		=	$_SESSION['tempacknumber'];
		
		if( isset($_SESSION['tempacknumber']) ){	
			
			if( $email != "" && $mobile != "" ){
				
				db_delete('l_common_application_master')
						->condition('user_id', trim($user->uid),'=')
						->execute();
						
				$fieldsCommonAppInfo = array( 
													'user_id'				=>	$user_id,
													'f_name'				=>	$f_name,
													'm_name'				=>	$m_name, 
													'l_name'				=>	$l_name, 
													'dob'					=>	$dob, 
													'email'					=>	$email, 
													'mobile'				=>	$mobile,  
													'gender'				=>	$gender, 
													'address'				=>	$address, 
													'district'				=>	$district, 
													'block'					=>	$block, 
													'vil_ward_borough'		=>	$vil_ward_borough, 
													'pin'					=>	$pin, 
													'card_type'				=>	$card_type, 
													'card_number'			=>	$card_number ,
													'common_app_number'		=>  $common_app_number,
													'status'				=>	$status,
													);
				db_insert('l_common_application_master')->fields($fieldsCommonAppInfo)->execute();
			}
			$_SESSION['tempacknumber']			=	$common_app_number;
			$message							=	'Application Saved Successfully';
			drupal_set_message(t($message));
		}
	}
	
	