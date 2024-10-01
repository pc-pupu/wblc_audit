<?php

drupal_add_js(drupal_get_path('module', 'applicant_registration') . '/js/custom.js');
function find_user_details($form, &$form_state ){
	 global $base_root, $base_path;
	 
	 $form['act_id']=array(
						  '#prefix' => '<div class="row"><div class="col-md-3"><div class="search-form">',
	    				  '#suffix' => '',
						  '#type' => 'select',
						  '#options' => array( 
							""=> 'Select Service', 
							'1' => t('Registration of Principal Emp. under CLRA'), 
							'12'=> t('Contractor License under CLRA'), 
							'2' => t('Est.Registration under BOCWA'),
							'3' => t('Registration under MTW'),
							'4' => t('Registration under ISMW'),
							// '6'	=> t(''),
						),
						  '#title'=> t('Select service to get your user information'),
						  '#required'=> TRUE,
						  '#attributes' => array('class'=>array('form-control')),
					      '#ajax' => array(
											'event' => 'change',
											'effect' => 'fade',			
											'callback' => 'get_ajax_callback_title_change',
											'progress' => '',
											 )
						  );
						  
	$value_act_id = isset($form_state['values']['act_id']) ? $form_state['values']['act_id'] : 1;	
					  
	if($value_act_id==1||$value_act_id==2||$value_act_id==3||$value_act_id==4){			  
		$form['reg_license_no']=array(
							 '#prefix' => '<div id="text_change">',
							 '#suffix' => '</div>',
							 '#title'=> t('Registration Number'),
							 '#type' => 'textfield',
							 '#attributes' => array('class'=>array('form-control')),
						  );
						  
	}else if($value_act_id==6){
		$form['reg_license_no']=array(
							 '#prefix' => '<div id="text_change">',
							 '#suffix' => '</div>',
							 '#title'=> t('Previous Transaction Number'),
							 '#type' => 'textfield',
							 '#attributes' => array('class'=>array('form-control')),
						  );
	}else{
		
		$form['reg_license_no']=array(
							 '#prefix' => '<div id="text_change">',
							 '#suffix' => '</div>',
							 '#title'=> t('License Number'),
							 '#type' => 'textfield',
							 '#attributes' => array('class'=>array('form-control')),
						  );
	}
	
	$form['or_text']	= array(
				'#type' 			=> 'markup',
				'#markup' 			=> '<b>OR</b>'
		);
		
	if($value_act_id==1||$value_act_id==2||$value_act_id==3||$value_act_id==4){	
					  
		$form['identification_number']=array(
							 '#prefix' => '<div id="text_change_form_v">',
							 '#suffix' => '</div>',
							 '#title' => t('Identification Number'),
							 '#type'=> 'textfield',
							 '#attributes' => array('class'=>array('form-control')),
						  );		
					  
	}else if($value_act_id==6) {
		$form['identification_number']=array(
							 '#prefix' => '<div id="text_change_form_v">',
							 '#suffix' => '</div>',
							 '#title' => t('Bank Transaction ID'),
							 '#type'=> 'textfield',
							 '#attributes' => array('class'=>array('form-control')),
						  );	
	}else{
		$form['identification_number']=array(
							 '#prefix' => '<div id="text_change_form_v">',
							 '#suffix' => '</div>',
							 '#title' => t('Form-V Number / Reference Number'),
							 '#type'=> 'textfield',
							 '#attributes' => array('class'=>array('form-control')),
						  );	
		
	}
					  
	 $form['bt_srchbtn'] = array(
				'#type' => 'button',
				'#prefix' => '',
        		'#suffix' => '</div></div>',	
				'#default_value' => 'Search  ',
				'#ajax' 		 => array(
										'callback' => 'user_list_search',
										'wrapper' => 'user_info_div',
										'method' => 'html',
										'effect' => 'fade'
										)
   			);	
	
	$form['data_info'] = array(
	      '#prefix' => '<div class="col-md-9 content-box-inner"><h1>User Information</h1><div id="user_info_div">',
	      '#suffix' => '</div></div></div>',       
	); 	
			  
	return $form;	 	
	 
}

function user_list_search($form, &$form_state) {
	
	global $user;
	$val	= $form_state['values'];
	
	$act_id=$val['act_id'];
	$reg_license_no=$val['reg_license_no'];
	$identification_number=$val['identification_number'];
	
	if(!empty($reg_license_no)||!empty($identification_number)){
		
		
		if($act_id == 1){
			
			$clra_registration_details = db_select('l_clra_registration_master', 't');
			$clra_registration_details->leftjoin('users', 'u', 't.user_id = u.uid');
			$clra_registration_details->leftjoin('l_common_application_master', 'ca', 'ca.user_id = u.uid');
			$clra_registration_details->fields('u', array('name','mail'));
			$clra_registration_details->fields('ca', array('mobile'));
			if(!empty($reg_license_no)){
				$clra_registration_details->condition('t.registration_number', $reg_license_no);
			}else{
				$clra_registration_details->condition('t.identification_number', $reg_license_no);
			}
			$user_info = $clra_registration_details->execute()->fetchObject();
			
			$label_Text='Registration Number';
		}
		
		if($act_id == 2){
			
			$bocwa_registration_details = db_select('l_bocwa_registration_master', 't');
			$bocwa_registration_details->leftjoin('users', 'fd', 't.user_id = fd.uid');
			$bocwa_registration_details->leftjoin('l_common_application_master', 'ca', 'ca.user_id = fd.uid');
			$bocwa_registration_details->fields('fd', array('name','mail'));
			$bocwa_registration_details->fields('ca', array('mobile'));
			if(!empty($reg_license_no)){
				$bocwa_registration_details->condition('t.registration_number', $reg_license_no);
			}else{
				$bocwa_registration_details->condition('t.identification_number', $identification_number);
			}
			$user_info  = $bocwa_registration_details->execute()->fetchObject();
			
			$label_Text='Registration Number';
		}
		
		if($act_id == 12){
			
			$contractor_license_details = db_select('l_contractor_license_application', 't');
			$contractor_license_details->leftjoin('users', 'fd', 't.contractor_user_id = fd.uid');
			$contractor_license_details->leftjoin('l_common_application_master', 'ca', 'ca.user_id = t.contractor_user_id');
			$contractor_license_details->fields('fd', array('name','mail'));
			$contractor_license_details->fields('ca', array('mobile'));
			$contractor_license_details->fields('t', array('contractor_license_number'));
			if(!empty($reg_license_no)){
				$contractor_license_details->condition('t.contractor_license_number', $reg_license_no);
				$contractor_license_details->condition('t.status', 'R','!=');
			}else{
				$contractor_license_details->condition('t.serial_no_from_v', $identification_number);
			}
			
			$user_info = $contractor_license_details->execute()->fetchObject();
			 
			if($user_info->contractor_license_number !=''){
				$reg_license_no = $user_info->contractor_license_number;
			}
			
			$label_Text='License Number';
		}
		
		if($act_id == 3){
			
			$mtw_registration_details = db_select('l_mtw_registration_master', 't');
			$mtw_registration_details->leftjoin('users', 'fd', 't.user_id = fd.uid');
			$mtw_registration_details->leftjoin('l_common_application_master', 'ca', 'ca.user_id = fd.uid');
			$mtw_registration_details->fields('fd', array('name','mail'));
			$mtw_registration_details->fields('ca', array('mobile'));
			if(!empty($reg_license_no)){
				$mtw_registration_details->condition('t.registration_number', $reg_license_no);
			}else{
				$mtw_registration_details->condition('t.identification_number', $identification_number);
			}
			$user_info = $mtw_registration_details->execute()->fetchObject();
			
			$label_Text='Registration Number';
		}
		
		if($act_id == 4){
			
			$interstate_workman_details = db_select('l_interstate_workman_master', 't');
			$interstate_workman_details->leftjoin('users', 'fd', 't.user_id = fd.uid');
			$interstate_workman_details->leftjoin('l_common_application_master', 'ca', 'ca.user_id = fd.uid');
			$interstate_workman_details->fields('fd', array('name','mail'));
			$interstate_workman_details->fields('ca', array('mobile'));
			if(!empty($reg_license_no)){
				$interstate_workman_details->condition('t.registration_number', $reg_license_no);
			}else{
				$interstate_workman_details->condition('t.identification_number', $identification_number);
			}
			$user_info = 	$interstate_workman_details->execute()->fetchObject();
			
			$label_Text='Registration Number';
			
		}

		if($act_id == 6){
			$interstate_workman_details = db_select('l_cess_payment_details', 'pay');
			$interstate_workman_details->fields('pay', array('id','user_id','transaction_id','banktransaction_id'));


			$interstate_workman_details->leftjoin('users', 'u', 'u.uid = pay.user_id');
			$interstate_workman_details->fields('u', array('name','mail'));

			$interstate_workman_details->leftjoin('l_common_application_master', 'com', 'com.user_id = u.uid');
			$interstate_workman_details->fields('com', array('email','mobile'));
						
			if(!empty($reg_license_no)){
				$interstate_workman_details->condition('pay.transaction_id', $reg_license_no);
				$label_Text = 'Transaction ID';
				$reg_license_no = $reg_license_no;
			}else{
				$interstate_workman_details->condition('pay.banktransaction_id', $identification_number);

				$label_Text = 'Bank Transaction ID';
				$reg_license_no = $identification_number;
			}
			$user_info = 	$interstate_workman_details->execute()->fetchObject();
			
			
		}
		
		$phone1=substr($user_info->mobile,0,2);
		$phone2=substr($user_info->mobile,7,9);
		$phone=$phone1.'*****'.$phone2;
		if($user_info->mobile==''){
			$phone='';
		}else{
			$phone=$phone1.'******'.$phone2;
		}
		$emaillen=strlen($user_info->mail);
		$half_address=$emaillen/2;
		$str=strstr($user_info->mail,'@',true);
		$email2=substr($str,0,4);
		if($user_info->mail==''){
			$mail='';
		}else{
			$mail=$email2.'******'.strstr($user_info->mail,"@");
		}
		
		if(empty($reg_license_no)){
			$reg_license_no='Null';
		}else{
			$reg_license_no=$reg_license_no;
		}
	
	}
	$output.='<div class="col-sm-12 content-box-inner">		
              
        <style type="text/css">
					table, td, th {font-size:15px;font-family:Times New Roman;height:40px;margin-top:20px;}
					td{padding-left:13px !important;}
					th {background-color: #008BD1;color: white;}
					td:hover {background-color:#d4e3e5;}
					tr:nth-child(even) {background: #DBE5F0}
					tr:nth-child(odd) {background: #F1F1F1}
					.red-star{color:#FF0000;}
				</style>
				
				<table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table">					 
					 <tbody><tr>
						 <th width="30%" style="font-size:17px; padding-top: 8px;padding-left: 5px;">Parameters</th>
						 <th style="font-size:17px; padding-top: 8px;padding-left: 5px;">Values</th>
					 </tr>
					 <tr>
					 	  <td>'.$label_Text.'</td>
						  <td>'.$reg_license_no.'</td>
					 </tr>
					 <tr>
						  <td>User Name</td>
						  <td>'.$user_info->name.'</td>
					 </tr>
					 <tr>
						  <td>Email Address</td>
						  <td>'.$mail.'</td>
					 </tr>
					 <tr>
						  <td>Mobile Number</td>
						  <td>'.$phone.'</td>
					 </tr>
					
				</tbody></table></div>';
				
	$output1 .='<font color="#FF0000">Information Invalid</font>';			
				
	if(!empty($user_info->name)){
		return $output;
	}else{
		return $output1;
	}
	
}

function get_ajax_callback_title_change($form, $form_state) {
	$commands   = array();
	$commands[] = ajax_command_replace('#text_change', drupal_render($form['reg_license_no']));
	$commands[] = ajax_command_replace('#text_change_form_v', drupal_render($form['identification_number']));
	return array('#type' => 'ajax', '#commands' => $commands);
}

function email_address_change_request($form, &$form_state,$email_id='' ){
	 global $base_root, $base_path;
	
	 $form['header'] = array(
							'#markup' => t('<div class="content application-user"><div class="sky-form"><fieldset><legend>Application for change Email</legend>'),
							);
	
	$form['access']=array(
						  '#prefix' => '<div class="col-md-6">',
	    				  '#suffix' => '</div>',
						  '#type' => 'select',
						  '#options' => array( ""=> 'Select Service', '1' => t('Yes'), '2'=> t('No')),
						  '#title'=> t('Do you have access of you registared mobile number?'),
						  '#required'=> TRUE,
						  '#id' =>'access',
						  '#attributes' => array('class'=>array('form-control'),'onchange'=>'return others_access();')
						  );
						  
						  
	$form['markup_data1'] = array(	'#prefix' => '<div id="profile" class="col-md-12" style="display: none">',
									'#suffix' => '</div>',
									'#type'			=> 'markup',
									'#markup' 		=> '<h4><center>Please Log in your profile and change phone number from "My Profile" tab</center></h4>',
									);
						  
		
	
	$form['existingmail']=array(
							 '#prefix' => '<div id="text_change" class="col-md-6">',
							 '#suffix' => '</div>',
							 '#type' => 'hidden',
							 '#value' => encryption_decryption_fun('decrypt', $email_id)
							);
							
	$form['submit'] = array (
							  '#prefix' => '<div class="col-md-12" id="sub" style="display: none">',
							  '#suffix' => '</div>',							  
							  '#type' => 'submit',
							  '#value' => 'Submit',
							  '#suffix' => '</div>',
							);
	$form['footer'] = array(
							'#markup' => '</div></div>',
							);
	
			  
	return $form;	 	
	 
}

function email_address_change_request_submit($form, &$form_state){
	
	global $base_root, $base_path, $user;
	$user_id = $user->uid;
	$val = $form_state['values'];
	
	$existingmail = $val['existingmail'];
	
	$query = db_select('l_common_application_master', 'v');
	$query->fields('v',array('mobile','email','user_id'));
	$query->condition('v.email', $existingmail, '=');
	$result = $query->execute();
	$record = $result->rowCount();
	$data = $result->fetchObject();
	$existin_user_id = $data->user_id;
	$existin_email_id = $data->email;
	$existin_mobile_id = $data->mobile;	
	$fields = array( 
			'existin_gmail' => $existingmail,
			'exesting_user_id' => $existin_user_id,
			'exesting_mobile' => $existin_mobile_id,
		);
	
	db_insert('l_email_change_request')->fields($fields)->execute();
	drupal_goto('phone-number-varify/'.encryption_decryption_fun('encrypt',$existingmail));
	
	
	
}

function varify_mobile($form,&$form_state,$email_id=''){
	 
	 $email_id = encryption_decryption_fun('decrypt',$email_id);
	 
	 $form['mobile_varify']['header'] = array(
							'#markup' => t('<div class="content application-user"><div class="sky-form"><fieldset><legend>Application for change Email</legend>'),
							);
							
	$form['mobile_varify']['mobile_number'] =	array(						
				'#type'				=> 'textfield',
				'#title'			=> t('Enter your New mobile number'),
				'#attributes' 		=> array('placeholder'=>'760309XXXX', 'maxlength' => 255, 'class' => array('form-control'))						
				);
				
	$form['mobile_varify']['email_id'] =	array(						
				'#type'				=> 'textfield',
				'#title'			=> t('Enter your New Email'),
				'#attributes' 		=> array('placeholder'=>'ari@gmail.com','maxlength' => 255, 'class' => array('form-control'))						
				);
				
	$form['mobile_varify']['user_name'] =	array(						
				'#type'				=> 'textfield',
				'#title'			=> t('Enter your New User Name'),
				'#attributes' 		=> array('maxlength' => 255, 'class' => array('form-control'))						
				);
				
	$mobile_number = isset($form_state['values']['mobile_number']) ? $form_state['values']['mobile_number'] : '';
	$email_id = isset($form_state['values']['email_id']) ? $form_state['values']['email_id'] : '';	
	
	if(!empty($mobile_number)){
		$query = db_select('l_common_application_master', 'v');
		$query->fields('v',array('mobile','email','user_id'));
		$query->condition('v.mobile', $mobile_number, '=');
		$query->condition('v.email', $email_id, '=');
		$result = $query->execute();
		$record = $result->rowCount();
		$data = $result->fetchObject();
		$user_id = $data->user_id;
	}else{
		$record = 0;
	}
	
	if(!empty($mobile_number) && $record>0){
		echo $frdigit_no = substr(str_shuffle("0123456789"), 0, 4);
		echo $frdigit_email_no = substr(str_shuffle("0123456789"), 0, 4);
		//$frdigit_no = encryption_decryption_fun('encrypt',$frdigit_no);
		setcookie('frdigit_no',$frdigit_no, time() + (600), "/");
		//$msg = 'Use '.$frdigit_no.' as your login OTP. OTP is confidential. Sharing it with anyone gives them full access to your update information.';
	 	//send_sms_for_user_alert(trim($mobile_number), $msg);
		
					
		
					
		$form['mobile_varify']['phone_no'] =	array(
					'#type'			=> 'hidden',
					'#value'		=> $mobile_number
		);
		
		$form['mobile_varify']['email'] =	array(
					'#type'			=> 'hidden',
					'#value'		=> $email_id
		);
		
		$form['mobile_varify']['user_id'] =	array(
					'#type'			=> 'hidden',
					'#value'		=> $user_id
		);
		
		$form['mobile_varify']['secuirity_code'] =	array(						
					'#prefix'		=> '<div id="secuirity_code_dv">',
					'#suffix'		=> '</div>',
					'#type'			=> 'textfield',
					'#title'		=> t('Enter 4 digits secuirity code'),
					'#required'		=> TRUE,
					'#attributes' 	=> array( 'placeholder'=>'XXXX', 'maxlength' => 255, 'class' => array('form-control'))						
					);
					
		$form['mobile_varify']['msg'] =	array(						
					'#prefix'		=> '<div id="secuirity_code_msg">',
					'#suffix'		=> '</div>',
					'#type'			=> 'markup',
					'#markup'		=> '<font color="red">'.t('Please check your mobile number for 4 digit secuirity code.').'</font>'										
					);
					
		
		$form['mobile_varify']['secuirity_code_email'] =	array(						
					'#prefix'		=> '<div id="secuirity_code_div">',
					'#suffix'		=> '</div>',
					'#type'			=> 'textfield',
					'#title'		=> t('Enter 4 digits secuirity code'),
					'#required'		=> TRUE,
					'#attributes' 	=> array( 'placeholder'=>'XXXX', 'maxlength' => 255, 'class' => array('form-control'))						
					);
					
		$form['mobile_varify']['msg_email'] =	array(						
					'#prefix'		=> '<div id="secuirity_code_msg_email">',
					'#suffix'		=> '</div>',
					'#type'			=> 'markup',
					'#markup'		=> '<font color="red">'.t('Please check your email for 4 digit secuirity code.').'</font>'										
					);
					
		
		$form['mobile_varify']['submit'] = array(
					'#prefix'		=> '<div id="submit_btn_msg">',
					'#suffix'		=> '</div>',					
					'#attributes' 	=> array('class' => array('form-submit')),
					'#type'			=> 'submit',
					'#value'		=> t('SUBMIT')						
					);
	}else{
		
		if ($record ==0 && !empty($mobile_number)){
			$form['mobile_varify']['new_register_msg'] =	array(						
						'#prefix'		=> '<div id="secuirity_code_msg">',
						'#suffix'		=> '</div>',
						'#type'			=> 'markup',
						'#markup'		=> '<font color="red">'.t('This mobile number is not registered to your account.').'</font>'										
			);
		}
		$form['mobile_varify']['submit'] = array(					
					'#attributes' 	=> array('class' => array('form-submit')),
					'#type'			=> 'button',
					'#value'		=> t('Generate Otp'),
					'#ajax' 		=> 	array(
											'event' 	=> 'change',
											'effect' 	=> 'fade',			
											'callback'	=> 'get_ajax_callback_ofc_posting',
											'progress'	=> ''										
										)		
		);
		
		
	}
	
	$form['footer'] = array(
							'#markup' => '</div></div>',
							);
	
	return $form;
}


function varify_mobile_submit($form,&$form_state){
	 
	$val = $form_state['values'];
	$frdigit_no=$_COOKIE['frdigit_no'];
	//echo $frdigit_no=encryption_decryption_fun('decrypt',$_COOKIE['frdigit_no']);
	if ($frdigit_no==$val['secuirity_code']){
		$mobile_number = $val['phone_no'];
		$email = $val['email_id'];
		$email_ex = $val['email'];
		
		$user_name = $val['user_name'];
		$user_id = $val['user_id'];
		if(!empty($mobile_number)){
			
			// OTP verification with status change
			db_update('l_email_change_request')->fields(array('user_id'=>$user_id,'new_phone_number'=>$mobile_number,'new_email'=>$email))->condition('existin_gmail',$email_ex)->execute();
			$message = 'Email change request has been received and forwarded to the concerned person';
			drupal_goto('upload_documents/'.encryption_decryption_fun('encrypt',$user_id));
			drupal_set_message(t($message));
		}
	}else{
		$message="Entered code is  incorrect";
	}
}

function upload_documents($form, &$form_state,$user_id='' ){
	 global $base_root, $base_path;
	 
	 $form['header'] = array(
							'#markup' => t('<div class="content application-user"><div class="sky-form"><fieldset><legend>Application for change Email Upload Documents</legend>'),
							);
							
	$user_id=encryption_decryption_fun('decrypt',$user_id);
	
	$form['mobile_varify']['user_id'] =	array(
					'#type'			=> 'hidden',
					'#value'		=> $user_id
		);
	$form['registered_certificate'] 	= array(
						'#type' 				=> 'managed_file',
						'#title'				=>'1.Registration Certificate (Part - I and Part - II) of Desires Registered Unit',
						'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
						'#upload_location'		=> 'public://upload/encrypt/',
						'#process' 				=> array('import_my_file_element_process'),
						'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
						'#description'			=> '<i style="color:red;" class="fa fa-info-circle"> (Max size 300 KB, pdf file only) </i>',
						'#prefix' 				=> '<label class="input input-file">',
						'#suffix' 				=> '</label>'
						);
						
	$form['noc'] 	= array(
						'#type' 				=> 'managed_file',
						'#title'				=>'2.NOC Letters along with signature and stamp of the unit higher authority.',
						'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
						'#upload_location'		=> 'public://upload/encrypt/',
						'#process' 				=> array('import_my_file_element_process'),
						'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
						'#description'			=> '<i style="color:red;" class="fa fa-info-circle"> (Max size 300 KB, pdf file only) </i>',
						'#prefix' 				=> '<label class="input input-file">',
						'#suffix' 				=> '</label>'
						);
	
	
							
	$form['submit'] = array (
							  '#prefix' => '<div class="row"><div class="col-md-12">',
							  '#suffix' => '</div></div>',							  
							  '#type' => 'submit',
							  '#value' => 'Submit',
							  '#suffix' => '</div>',
							);
	$form['footer'] = array(
							'#markup' => '</div></div>',
							);
	
			  
	return $form;	 	
	 
}

function upload_documents_submit($form,&$form_state){
	 
	$val = $form_state['values'];
	$user_id = $val['user_id'];
	
	$registered_certificate = file_load($form_state['values']['registered_certificate']);
	$registered_certificate_name = $registered_certificate->filename;
	$registered_certificate_filesize = $registered_certificate->filesize;
	
	if($registered_certificate_filesize > 300000 ){
	
		drupal_set_message(t('Registration Certificate fileSize should be less then 300kb'),'error');
		return false;
	
	}
	
	$egistered_certificate_name_link=escapefile_url($base_root.$base_path.'sites/default/files/upload/encrypt/'.$registered_certificate_name);
	$egistered_certificate_link=file_get_contents($egistered_certificate_name_link);
	$egistered_certificate_encode_image_file = base64_encode($egistered_certificate_name_link);
	drupal_unlink('sites/default/files/upload/encrypt/'.$registered_certificate_name);
	
	$noc = file_load($form_state['values']['noc']);
	$noc_name = $noc->filename;
	$noc_filesize = $noc->filesize;
	
	if($noc_filesize > 300000 ){
	
		drupal_set_message(t('NOC Letters fileSize should be less then 300kb'),'error');
		return false;
	
	}
	
	$noc_name=escapefile_url($base_root.$base_path.'sites/default/files/upload/encrypt/'.$noc_name);
	$noc_name_link=file_get_contents($noc_name);
	$noc_name_link_image_file = base64_encode($noc_name_link);
	drupal_unlink('sites/default/files/upload/encrypt/'.$noc_name);
	
	$pdfData = array(
		
		'registered_certificate'=> $egistered_certificate_encode_image_file,
		'noc'=>$noc_name_link_image_file,
	);
	
	db_update('l_email_change_request')->fields($pdfData)->condition('user_id',$user_id)->execute();
	$message = 'Email change request has been already received by the concerned person';
	drupal_set_message(t($message));
	
}

function DASHBOARD_EMAIL_VALIDATE($element, &$form_state){
		 $email 	=	trim($form_state['values']['newemail']);
		 if( $email == "" ){
			form_error($element, t($element['#title'].' cannot be empty.')); 
		 }
		 if (!filter_var($email, FILTER_VALIDATE_EMAIL)) {
			form_error($element, t($element['#title'].' format invalid.'));
		 }
		 
	}

?>