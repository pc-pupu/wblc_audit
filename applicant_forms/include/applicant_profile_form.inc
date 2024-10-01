<?php

drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicant_dashboard.css');

function applicant_profile_form($form, &$form_state){
		global $base_root, $base_path, $user;
			
		$_SESSION['uid'] 	= $user->uid;
		
		
		$get_final_ack_number= db_query("select final_ack_number from l_common_application_master where user_id=:user_id",array(':user_id'=>$_SESSION['uid']));
		
		if( $get_final_ack_number->rowCount() > 0 ){
			foreach($get_final_ack_number as $obj1){ 
				$_SESSION['final_ack_number']		=	$obj1->final_ack_number;
			}
		}
	
		$fetch_common_det 	= db_query("select f_name, m_name, l_name, dob, email, mobile, gender, address, district, block, vil_ward_borough, pin, card_type, card_number, status from l_common_application_master where user_id=:userId",array(':userId'=>$_SESSION['uid']));
		if( $fetch_common_det->rowCount() > 0 ){
			foreach($fetch_common_det as $obj){
				$f_name					=	$obj->f_name;
				$m_name					=	$obj->m_name;
				$l_name					=	$obj->l_name;
				$dob					=	$obj->dob;
				$email					=	$obj->email;
				$mobile					=	$obj->mobile;
				$gender					=	$obj->gender;
				$address				=	$obj->address;
				$district				=	$obj->district;
				$block					=	$obj->block;
				$vil_ward_borough		=	$obj->vil_ward_borough;
				$pin					=	$obj->pin;
				$card_type				=	$obj->card_type;
				$card_number			=	$obj->card_number;
				$_SESSION['tabStatus']	=	isset($obj->status) ? $obj->status : 0;
			}
		}
	
		drupal_add_library('system','ui.datepicker');
		
		drupal_add_js('jQuery(document).ready(function(){jQuery( \'#applicant_dobId\' ).datepicker({
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
								
								
		$form['dashboard_master']['final_ack_number'] = array(
								  '#prefix'=> '<table class="applicant_dashboard" class="" border="0" width="100%"><tr><td>',
								  '#suffix'=>'</td></tr>',
								  '#title'=>'<span class="loginbox">Final Acknowlegment Number</span>',
								  '#size'=>t('20'),	
								  '#type' => 'hidden',
								  '#default_value' =>isset($_SESSION['final_ack_number']) ? $_SESSION['final_ack_number'] : '',
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
								  '#attributes' => array('id' => 'applicant_dobId', 'placeholder' => 'Enter your Date of Birth', 'onclick' => 'applicant_prof_datepicker();', 'class' =>  array('applicant_dob_class')),
								  '#default_value' =>!empty($dob) ? $dob : '',
								  '#required' => TRUE,
								  //'#element_validate' => array('getDatePickerFunc')
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
								
		$genderOptions 			=	array('0'=>t('Male'), '1'=>t('Female'), '2'=>t('Transgender'));
		
		$form['dashboard_master']['gender'] = array(
								  '#prefix'=> '<tr><td>',
								  '#suffix'=>'</td></tr>',
								  '#type' => 'radios',
								  '#title' => t('Gender'),
								  '#options' => $genderOptions,
								  '#default_value' => isset($gender) ? $gender : 0,
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
		
		$options_dist			=	applicant_prof_dist_dropdown_options();
		
		$form['dashboard_master']['district'] = array(
											  '#prefix'=> '<tr><td>',
								  			  '#suffix'=>'</td></tr>',
											  '#title'=>'District of the Applicant',
											  '#type' => 'select',
											  '#options' =>$options_dist,
											  '#default_value' => !empty($district) ? $district	:'',
											  '#required' => TRUE,
											  '#ajax'		=> array(
																'event' 			=> 'change',
																'effect' 			=> 'fade',			
																'callback'			=> 'get_app_prof_info_form_ajax_callback',
																'progress'			=> '',
															  )
											);
		
		$value_dist 	= 	isset($form_state['values']['district']) ? $form_state['values']['district'] : '';
								
		$form['dashboard_master']['vil_ward_borough'] = array(
								  '#prefix'=> '<tr><td>',
								  '#suffix'=>'</td></tr>',
								  '#title'=>'<span class="">Vill./Ward/Borough</span>',
								  '#size'=>t('20'),	
								  '#type' => 'textfield',
								  '#default_value' =>!empty($vil_ward_borough) ? $vil_ward_borough : '',
								  '#attributes' => array('placeholder' => 'Enter your Gram Panchayat/Ward/Borough'),
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
		
		$cardTypeOptions 			= 	array(0=>'Select Card Type', 1=>'AADHAR', 2=>'PAN', 3=>'TAN', 4=>'LIN', 5=>'VOTER');
		$value_dropdown_cardType	=	!empty($card_type) ? $card_type : key($cardTypeOptions);
		
		
		$form['dashboard_master']['card_type'] = array(
								  '#prefix'=> '<tr><td>',
								  '#suffix'=>'</td></tr>',
								  '#title'=>'<span class="">ID Card Type</span>',
								  '#type' => 'select',
								  '#options' => $cardTypeOptions,
								  '#default_value'=> $value_dropdown_cardType,
								  '#required' => TRUE,
								  '#element_validate'=> array('DASHBOARD_CARD_TYPE_VALIDATE')
					   			);
		
		$form['dashboard_master']['card_number'] = array(
		                          '#prefix'=> '<tr><td>',
								  '#suffix'=>'</td></tr>',
								  '#title'=>'<span class="">ID Card Number</span>',
								  '#type' => 'textfield',
								  '#size'=>t('20'),
								  '#default_value' =>!empty($card_number) ? $card_number : '',
								  '#attributes' => array('placeholder' => 'Enter your Id Card Number', 'id' => 'card_number_id', 'maxlength' => 20),
								  '#required' => TRUE,
								  '#element_validate' => array('DASHBOARD_SPECIAL_CHAR_TEXT_VALIDATE')
					   			);
							
	$form['dashboard_master']['submit'] = array (
								  '#prefix'=> '<tr><td>',
								  '#suffix'=>'</td></tr></table>',
								  '#type' => 'submit',
								  '#value' => 'SAVE'
							   );
	return $form;
}

function final_ack_number_detail(){
		global $user;
		$date = date('Y-m-d H:i:s');
		$final_number = strtotime($date);
		$final_ack_number = $final_number.'-'.$user->uid;
		return $final_ack_number;
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
	/*$validateEmail = db_query("select u.mail as email_id from users u where u.uid=:user_id", array(':user_id' => $_SESSION['uid']));
	if( $validateEmail->rowCount() > 0 ){
		$fetched_email =  $validateEmail->fetchObject()->email_id;
		if( $val['email'] == $fetched_email ){
			form_error($element, t($element['#title']. ' already exists.'));
			$form_state['rebuild'] = TRUE;
		}
	}*/
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

function applicant_profile_form_submit($form, &$form_state){
		global $base_root,$base_path,$user;
		
		$val 					=	$form_state['values'];
		$user_id				=	$user->uid;
		$f_name					=	$val['f_name'];
		$m_name					=	$val['m_name'];
		$l_name					=	$val['l_name'];
		$dob_1					=	$val['dob'];
		$dob_expl				=	explode("-", $dob_1);
		$dob_af_exp				=	$dob_expl[2]."-".$dob_expl[1]."-".$dob_expl[0];
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
		$final_ack_number		=   isset($_SESSION['final_ack_number']) ? $_SESSION['final_ack_number'] : final_ack_number_detail();
		
		if( isset($user_id) ){	
			
			if( $email != "" && $mobile != "" ){
				
				db_delete('l_common_application_master')
						->condition('user_id', trim($user->uid),'=')
						->execute();
						
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
													'district'				=>	$district, 
													'block'					=>	$block, 
													'vil_ward_borough'		=>	$vil_ward_borough, 
													'pin'					=>	$pin, 
													'card_type'				=>	$card_type, 
													'card_number'			=>	$card_number ,
													'status'				=>	$status,
													'final_ack_number'      =>  $final_ack_number,
										   );
										   
				 db_insert('l_common_application_master')->fields($fieldsCommonAppInfo)->execute();
			}
			$message							=	'Profile Data Saved Successfully';
			drupal_set_message(t($message));
		}
	}
	
	function applicant_prof_dist_dropdown_options(){
		/*$default_select = array( "" => "- Select District -" );
		$result 		= db_query("SELECT * FROM {district} order by district_id");
		$list 			= array();
		foreach ($result as $record) {
			$list[$record->district_id] = t('@dist_name', array('@dist_name' => $record->dist_name));
		}
		return ($default_select+$list);*/
		
		$country_query=db_query("SELECT * FROM {district} order by district_id");  
 	 	foreach ($country_query as $result){
       		$res[$result->district_id]=$result->dist_name;       
     	}
     	return $res;
	}
	
	function applicant_prof_block_municipality_dropdown_function($key=''){
		if( $key != "" ){
			$default_blockselect 	= array(""=>"- Select -");
			$all_blocks 			= array();
			$query_block 			= db_select('dis_block', 'db');
			$query_block->fields('db',array('district_id', 'block_id', 'block_name'));
			$query_block->condition('db.district_id',$key,'=');
			//print_r($query_block->execute()); exit;
			$result_block 			= $query_block->execute(); 
			
			if(!empty($result_block)) {      
				  foreach($result_block as $rows) {
					$all_blocks[$rows->block_id] = t('@block_name', array('@block_name' => $rows->block_name));
				  }    
			}
	
			if (isset($all_blocks)) {
				return ($default_blockselect + $all_blocks);
			} else {
				return $default_blockselect;
			}
		}else{
			$default_blockselect 	= array(""=>"- Select -");
			return $default_blockselect;
		}
	}
	
	
	function get_app_prof_info_form_ajax_callback($form, $form_state){
		$commands	=	array();
		$commands[] = ajax_command_replace('#app_prof_block_replace', drupal_render($form['dashboard_master']['block']));
		return array('#type' => 'ajax', '#commands' => $commands);
	}