<?php 
// drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
function update_office_address($form, &$form_state, $usr_id_en){
	global $base_root, $base_path, $user;
	
	//$user_id = $user->uid; 
	$user_id_de = encryption_decryption_fun('decrypt',$usr_id_en);
	if($user_id_de != ''){
		$user_id = $user_id_de;
	}else{
		$user_id = $user->uid;
	}
	//echo $user_id;die;
	/*** Get User role **/
	
	$fetch_users_role		= db_select('users_roles', 'ur');
	$fetch_users_role		->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role		->fields('ro', array('rid'));
	$fetch_users_role		->condition('ur.uid', $user_id);
	$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc();

	$users_role	=	$fetch_users_role_data['rid'];
	//echo $users_role;die;
	

	if(trim($users_role)== 7){
		
		$get_block_code = db_select('l_customuser_relation_address', 'lcra');
		$get_block_code->InnerJoin('users_roles', 'ur','ur.uid= lcra.user_id'); 	
		$get_block_code->fields('lcra', array('block_code','district_code'));
		$get_block_code->condition('lcra.user_id', $user_id);
		$get_block_code->condition('ur.rid', $users_role);
		$get_block_code_result = $get_block_code->execute()->fetchAssoc();	
		
		$get_area_code_result = $get_block_code_result['block_code'];
		$get_district_code    = $get_block_code_result['district_code'];
		$back_link			  = 'inslink/lwfc-details';	
		
	}elseif(trim($users_role)== 4){
		
		$get_sub_div = db_select('l_customuser_relation_address', 'lcra');
		$get_sub_div->InnerJoin('users_roles', 'ur','ur.uid= lcra.user_id'); 	
		$get_sub_div->fields('lcra', array('sub_div_code','district_code'));
		$get_sub_div->condition('lcra.user_id', $user_id);
		$get_sub_div->condition('ur.rid', $users_role);
		$get_sub_div_result = $get_sub_div->execute()->fetchAssoc();
		//print_r($get_sub_div_result);die;
		$get_area_code_result = $get_sub_div_result['sub_div_code'];
		$get_district_code    = $get_sub_div_result['district_code'];
		$back_link			  = 'alcdashboard/rlo-details';	

	}
	//print_r($get_sub_div_result);die;
	$check_user_details = db_select('l_rlo_additional_info', 'lrai');
	$check_user_details->fields('lrai', array('id'));
	$check_user_details->condition('lrai.user_id', $user_id);
	$result_user_details = $check_user_details->execute();
	
	if( $result_user_details->rowCount() > 0 ){
		
		$clra_certificate = db_select('l_rlo_additional_info', 'lrai');
		$clra_certificate->fields('lrai', array('user_id','clra_certificate_address','office_name', 'about_office', 'office_mobile_number', 'office_email_address'));
		$clra_certificate->condition('lrai.sub_div_code', $get_area_code_result);
		$clra_certificate->condition('lrai.user_id', $user_id);
		$result = $clra_certificate->execute()->fetchAssoc();

	}else{
		$clra_certificate = db_select('l_rlo_additional_info', 'lrai');
		$clra_certificate->fields('lrai', array('user_id','clra_certificate_address','office_name', 'about_office', 'office_mobile_number', 'office_email_address'));
		$clra_certificate->condition('lrai.sub_div_code', $get_area_code_result);
		
		$result = $clra_certificate->execute()->fetchAssoc();
	}



// 	echo "<pre>";
//  print_r($result);die;

	if(empty($result['office_mobile_number']) || empty($result['office_email_address'])){
		$form['office_address']['css'] = array(	
							'#markup' => '<style>.main-sidebar{display:none;}</style>',
							'#type' => 'markup'
							);
		
		
	}
	
	$form['office_address']['addressboxstart'] = array(	
							'#markup' => '<div class="row">
											<div class="col-md-12">
												<div class="box box-primary">													
													<div class="box-body">',
							'#type' => 'markup'
							);
	
	$form['office_address']['user_id'] = array(
	'#type' 			=> 'hidden', 
	'#value'			=>  $user_id,
	);						
	
	$form['office_address']['office_name'] 	= array(
						  '#prefix' => '<div class="col-lg-8 col-md-8">',
					  	  '#suffix' => '</div>',
						  '#title' => t('Office Name'),
						  '#type' => 'select',
						  '#options' => office_name(),
						  '#required' =>  TRUE,
						  '#attributes' => array('class' => array('form-control')),
						  '#default_value' => $result['office_name']
						);	

			if($result['user_id'] == $user_id){


	$form['office_address']['office_mobile_number'] = array(
						  '#prefix' 		=> '<div class="col-lg-8 col-md-8">',
						  '#suffix' 		=> '</div>',
						  '#title' 			=> t('Enter Office Phone Number ').' <a href="#" class="small-box-footer" data-toggle="modal" data-target="#mobile_update"><strong> <i class="fa fa-edit"></i> </strong></a>',
						  '#type' 			=> 'textfield',
						  '#rows' => 2,						  
						  //'#required' 		=>  TRUE,
						  '#default_value' 	=> $result['office_mobile_number'],
						  '#attributes' => array('maxlength' => 10, 'class' => array('form-control'), 'readonly' => 'readonly'),
						  //'#element_validate'	=> array('TEXTBOX_NUMERIC_VALIDATE')
						);


	$form['office_address']['modal_open'] = array (		
		'#prefix'=>'<div class="modal fade" id="mobile_update" tabindex="-1" role="dialog" aria-labelledby="mobile_update_Label" aria-hidden="true">
						<div class="modal-dialog">
							<div class="modal-content">
								<div class="modal-header">
									<button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
										<h4 class="modal-title" id="mobile_update_Label">Mobile Number Update</h4>

										<p class="modal-title" id="desc"><font color="#FF0000"><b>*</b> </font> Please verify the old mobile number by otp</p>',
		'#suffix'=>'',
		);	
		
		$form['office_address']['old_mobile_number'] = array(
			'#prefix' 		=> '<div class="col-lg-8 col-md-8 mt-2">',
			'#suffix' 		=> '</div>',
			'#title' 			=> t('Old Mobile Number '),
			'#type' 			=> 'textfield',
			'#rows' => 2,	
			'#default_value' 	=> $result['office_mobile_number'],
			'#attributes' => array('maxlength' => 10, 'class' => array('form-control'), 'readonly' => 'readonly'),
			
		  );
		$form['office_address']['verify_btn'] = array(
				'#prefix' => '<div class="col-lg-8 col-md-8" id="verifybtn"><label class="input form-label-block">',
				'#suffix' => '</label></div>',
				'#type' => 'button',
				'#value' => t('Verify'),
				'#ajax' => array(
							'event' => 'click',
							'effect' => 'fade',			
							'callback' => 'get_otp_form_ajax_callback',
							'method' => 'replace',
							'progress' => array('type' => 'throbber','message' => t('Loading State...')),
							)
			);

			$val_verify = isset($form_state['values']['verify_btn']) ? $form_state['values']['verify_btn'] : '';
				
			$form['office_address']['linked_mobile_no_otp'] = array(
						'#prefix' => '<div id="otp1">',
						'#suffix' => '</div>',
						);
			$form['office_address']['verify_otp_btn'] = array(
						'#prefix' => '<div id="otp2">',
						'#suffix' => '</div>',
						);	
			$form['office_address']['update_mobile_number'] = array(
				'#prefix' => '<div class="col-lg-8 col-md-8" id="umn">',
				'#suffix' => '</div>',
			);
			if(!empty($val_verify) && $val_verify == 'Verify'){
				$form['office_address']['linked_mobile_no_otp'] = array(
					'#prefix' => '<div class="col-lg-8 col-md-8" id="otp1"><label class="input form-label-block">',
					'#suffix' => '</label></div>',
					'#title' => t('Enter OTP'),
					'#description' => t('Enter OTP send to Old Mobile Number'),
					'#type' => 'textfield',
					//'#required' => TRUE,
					'#attributes' => array('class' => array('form-control'),'autocomplete' => 'off','maxlength' => 6)
				);
				
				$form['office_address']['verify_otp_btn'] = array(
						'#prefix' => '<div class="col-lg-8 col-md-8" id="otp2"><label class="input form-label-block">',
						'#suffix' => '</label></div>',
						'#type' => 'button',
					//	'#attributes' => array('class' => array('form-control')),
						'#value' => t('Submit'),
						'#ajax' => array(
								'event' => 'click',
								'effect' => 'fade',			
								'callback' => 'otp_verify_ajax_callback',
								'method' => 'replace',
								'progress' => array('type' => 'throbber','message' => t('Loading State...')),
								)
				);

			$val_update = isset($form_state['values']['update_mobile_number']) ? $form_state['values']['update_mobile_number'] : '';
			if(!empty($val_update) && $val_update == 'Save and Continue'){
				$form['scheme']['linked_mobile_no_otp2'] = array(
					'#type' => 'markup',
					'#markup' => '<div class="col-md-6" id="otp1">Mobile Number updated</div>',
				);
				
				$form['scheme']['new_mobile_number'] = array(
					'#prefix' => '<div class="col-lg-8 col-md-8" id="otp2">',
					'#suffix' => '</div>',
				);
				
				$form['scheme']['update_mobile_number'] = array(
					'#prefix' => '<div class="col-lg-8 col-md-8" id="umn">',
					'#suffix' => '</div>',
				);
			}

				$mobile_no_otp = isset($form_state['values']['linked_mobile_no_otp']) ? $form_state['values']['linked_mobile_no_otp'] : '';	
				$form['scheme']['linked_mobile_no_otp2'] = array(
					'#type' => 'markup',
					'#markup' => '<div class="col-md-6" id="otp1"></div>',
				);
				
				$form['scheme']['new_mobile_number'] = array(
					'#prefix' => '<div class="col-lg-8 col-md-8" id="otp2">',
					'#suffix' => '</div>',
				);
				
				$form['scheme']['update_mobile_number'] = array(
					'#prefix' => '<div class="col-lg-8 col-md-8" id="umn">',
					'#suffix' => '</div>',
				);
				

				if(!empty($mobile_no_otp)){

					$queryckotp = db_select('l_otp_history', 'oh');
					$queryckotp->fields('oh',array('id'));
					$queryckotp->condition('oh.uid', $user_id, '=');
					$queryckotp->condition('oh.secuirity_code', $mobile_no_otp, '=');
					$queryckotp = $queryckotp->execute();
					if($queryckotp->rowCount() > 0){
						
					
				$form['scheme']['otp_verified'] = array(
					'#type'   		  => 'hidden',
					'#value' 		  => 'yes'
				);
				
				$form['scheme']['linked_mobile_no_otp2'] = array(
						'#type' => 'markup',
						'#markup' => '<div class="col-lg-8 col-md-8" id="otp1">Mobile Number verified</div>',
				);
						
				$form['scheme']['new_mobile_number'] = array(
					'#prefix' 		=> '<div class="col-lg-8 col-md-8" id="otp2">',
					'#suffix' 		=> '</div>',
					'#title' 			=> t('Enter New Mobile Number '),
					'#type' 			=> 'textfield',
					'#required' => TRUE,
					'#attributes' => array('maxlength' => 10, 'class' => array('form-control')),
					
				);	

				$form['scheme']['update_mobile_number'] = array(
					'#prefix' => '<div class="col-lg-8 col-md-8" id="umn">',
					'#suffix' => '</div>',
					'#type' => 'button',
				//	'#attributes' => array('class' => array('form-control')),
					'#value' => t('Save and Continue'),
					'#ajax' => array(
						'event' => 'click',
						'effect' => 'fade',			
						'callback' => 'update_mobile_ajax_callback',
						'method' => 'replace',
						'progress' => array('type' => 'throbber','message' => t('Loading State...')),
						)
					
			);
	

			}else{
				$form['scheme']['new_number_verify'] = array(
					'#type'   		  => 'hidden',
					'#value' 		  => 'no'
				);
				$form['scheme']['linked_mobile_no_otp2'] = array(
					'#type' => 'markup',
					'#markup' => '<div class="col-lg-8 col-md-8" id="otp1">Entered OTP is incorrect. Get New OTP click on "Verify" button</div>',
				);
				
				$form['scheme']['new_mobile_number'] = array(
					'#prefix' => '<div class="col-lg-8 col-md-8" id="otp2">',
					'#suffix' => '</div>',
				);
				$form['scheme']['update_mobile_number'] = array(
					'#prefix' => '<div class="col-lg-8 col-md-8" id="umn">',
					'#suffix' => '</div>',
				);
				
				
			}
			
		}
		
		
		
	}

	
		$form['office_address']['footer'] = array (		
		'#prefix'=>'</div><div class="modal-footer">',
		'#suffix'=>'</div>
		</div>
		</div>
		</div>',
		
					
		);								
							

	
		}else{
						$form['office_address']['office_mobile_number'] = array(
							'#prefix' 		=> '<div class="col-lg-8 col-md-8">',
							'#suffix' 		=> '</div>',
							'#title' 			=> t('Enter Office Phone Number ').' <a href="#" class="small-box-footer" data-toggle="modal" data-target="#mobile_update"></a>',
							'#type' 			=> 'textfield',
							'#rows' => 2,						  
							'#required' 		=>  TRUE,
							'#default_value' 	=> $result['office_mobile_number'],
							'#attributes' => array('maxlength' => 10, 'class' => array('form-control')),
							'#element_validate'	=> array('TEXTBOX_NUMERIC_VALIDATE')
							);	
					}
	$form['office_address']['office_email_address'] = array(
						  '#prefix' 		=> '<div class="col-lg-8 col-md-8">',
						  '#suffix' 		=> '</div>',
						  '#title' 			=> t('Enter Office email address'),
						  '#type' 			=> 'textfield',
						  // '#cols' 			=> 25, 
						  '#rows' 			=> 2, 
						  '#attributes' 	=> array('class' => array('form-control')),
						  '#required' 		=>  TRUE,
						  '#default_value' 	=> $result['office_email_address']
						);											
	$form['office_address']['clra_certificate_address'] = array(
						  '#prefix' 		=> '<div class="col-lg-8 col-md-8">',
						  '#suffix' 		=> '</div>',
						  '#title' 			=> t('Enter Office address'),
						  '#type' 			=> 'textarea',
						  // '#cols' 			=> 25, 
						  '#rows' 			=> 2, 
						  '#attributes' 	=> array('class' => array('form-control')),
						  '#required' 		=>  TRUE,
						  '#default_value' 	=> $result['clra_certificate_address']
						);
									
	
	$form['office_address']['about_office'] = array(
						  '#prefix' 		=> '<div class="col-lg-8 col-md-8">',
						  '#suffix' 		=> '</div></div></div></div>',
						  '#title' 			=> t('About Office'),
						  '#type' 			=> 'textarea',
						  //'#cols' 			=> 25, 
						  '#rows' 			=> 2, 
						  '#attributes' 	=> array('class' => array('form-control')),
						  '#required' 		=> TRUE,
						  '#default_value' 	=> $result['about_office']
						);															
	$form['office_address']['district_code'] = array(
						  '#type' 			=> 'hidden', 
						  '#value' 			=> encryption_decryption_fun('encrypt', $get_district_code)
					  );
																
	 $form['office_address']['sub_div_code'] = array(
						  '#type' 			=> 'hidden', 
						  '#value' 			=> encryption_decryption_fun('encrypt', $get_area_code_result)
						  );
								
	$form['office_address']['submit'] = array(
						  '#type' 			=> 'submit', 
						  '#value' 			=> t('Update'),
						  '#attributes' 	=> array('class' => array('btn btn-primary')),
						  '#prefix' 		=> '<div class="col-lg-3 col-md-3">',
						  '#suffix' 		=> '</div></div>',
						  );
																	
	/*$form['office_address']['markup'] = array(
						  '#markup' 		=> l('<span class="btn btn-primary">Back</span>',$back_link,array('html'=>TRUE)),
						  '#type' 			=> 'markup',
						  '#prefix' 		=> '<div class="col-lg-3 col-md-3">',
						  '#suffix' 		=> '</div>',
						  );*/
	return $form;
}

function update_office_address_submit($form, &$form_state){
	
	global $base_root, $base_path, $user;
	
	
	$val 				= $form_state['values'];
	if($val['user_id'] == ''){
		$user_id   			= $user->uid;
	}
	else{
		$user_id = $val['user_id'];
	}
	//print_r($val);die;
	/*** Get User role **/
	
	$fetch_users_role		= db_select('users_roles', 'ur');
	$fetch_users_role		->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role		->fields('ro', array('rid'));
	$fetch_users_role		->condition('ur.uid', $user_id);
	$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc();

	$users_role			=	$fetch_users_role_data['rid'];	
	
	
	$district_code 		= encryption_decryption_fun('decrypt', $val['district_code']);
	$area_code			= encryption_decryption_fun('decrypt', $val['sub_div_code']);
	
	$check_details = db_select('l_rlo_additional_info', 'lrai');
	$check_details->fields('lrai', array('clra_certificate_address','office_name'));
	$check_details->condition('lrai.sub_div_code', $area_code);
	$check_details->condition('lrai.user_id', $user_id);
	$result = $check_details->execute();
	
	if( $result->rowCount() > 0 ){
		$fields_Array		=	array(								
								'clra_certificate_address' 	=> $val['clra_certificate_address'],
								'office_email_address' 		=> trim($val['office_email_address']),
								'office_name' 				=> $val['office_name'],
								'sub_div_code'				=> $area_code,
								'district_code'				=> $district_code,
								'user_id'					=> $user_id,
								'about_office'				=> $val['about_office'],
								'modified_date'				=> time()
						   );
						   
		$query = db_update('l_rlo_additional_info');
		$query->fields($fields_Array); 
		$query->condition('sub_div_code', $area_code);
		$query->condition('user_id', $user_id);
		$query->execute();
		
	}else{
		$fields_Array		=	array(								
								'clra_certificate_address' 	=> $val['clra_certificate_address'],
								'office_mobile_number' 		=> trim($val['office_mobile_number']),
								'office_email_address' 		=> trim($val['office_email_address']),
								'office_name' 				=> $val['office_name'],
								'sub_div_code'				=> $area_code,
								'district_code'				=> $district_code,
								'user_id'					=> $user_id,
								'about_office'				=> $val['about_office'],
								'create_date'				=> time()
						   );		
		db_insert('l_rlo_additional_info')->fields($fields_Array)->execute();
	}
	
	if($users_role == 7){
		drupal_goto('inslink/lwfc-details');
	}elseif($users_role == 4){
		drupal_goto('alcdashboard/rlo-details');
	}	
}

function get_otp_form_ajax_callback($form, &$form_state){
	
	$val = $form_state['values'];
		
	//$mobile_no = $val['reg_mob_no'];
	//$sixdigit_no = 123456;
	//$sixdigit_no = substr(str_shuffle("0123456789"), 0, 6);

			//$sixdigit_no_en = encryption_decryption_fun('encrypt',$sixdigit_no);

			$secuirity_code = substr(str_shuffle("1234567890"), 0, 6);
			$secuirity_code_en = encryption_decryption_fun('encrypt',$secuirity_code);
			db_insert('l_otp_history')->fields(array('mobile_number' => $val['old_mobile_number'], 'secuirity_code' => $secuirity_code, 'is_active' => 1, 'uid' => $val['user_id'], 'created_date' => time()))->execute();

		//setcookie('sixdigit_no',$secuirity_code, time() + (600), "/");
		//$msg = 'Use '.encryption_decryption_fun('decrypt',$sixdigit_no).' as your login OTP. OTP is confidential. Sharing it with anyone gives them full access to your update information.';
	 	
		//send_sms_for_user_alert(trim($mobile_number), $msg);


	$otp_data = array(); 
	$otp_data[] = ajax_command_replace('#otp1', drupal_render($form['office_address']['linked_mobile_no_otp']));
	$otp_data[] = ajax_command_replace('#otp2', drupal_render($form['office_address']['verify_otp_btn']));
	$otp_data[] = ajax_command_replace('#umn', drupal_render($form['office_address']['update_mobile_number']));
	return array('#type' => 'ajax', '#commands' => $otp_data);
}
function otp_verify_ajax_callback($form, &$form_state){


	$render = array();
	$render[] = ajax_command_replace('#otp1', drupal_render($form['scheme']['linked_mobile_no_otp2']));
	$render[] = ajax_command_replace('#otp2', drupal_render($form['scheme']['new_mobile_number']));
	$render[] = ajax_command_replace('#umn', drupal_render($form['scheme']['update_mobile_number']));
	
	$form_state['rebuild']=TRUE;
	
	return array('#type' => 'ajax', '#commands' => $render);
}

function update_mobile_ajax_callback($form, &$form_state){
	
	$val = $form_state['values'];
		
	$mobile_no = $val['new_mobile_number'];
	
	//print_r($mobile_no);die;
	$inst = array(
		'changed_date'		 	=> date('Y-m-d'),
		'old_mobile_number'     => $val['old_mobile_number'],
		'office_mobile_number'  => $mobile_no,
		);

	db_update('l_rlo_additional_info')->fields($inst)->condition('user_id',$val['user_id'])->execute();
	
	$render = array();
	$render[] = ajax_command_replace('#otp1', drupal_render($form['scheme']['linked_mobile_no_otp2']));
	$render[] = ajax_command_replace('#otp2', drupal_render($form['scheme']['new_mobile_number']));
	$render[] = ajax_command_replace('#umn', drupal_render($form['scheme']['update_mobile_number']));
	
	$form_state['rebuild']=TRUE;
	
	return array('#type' => 'ajax', '#commands' => $render);
}

function TEXTBOX_NUMERIC_VALIDATE($element, &$form_state){
	if(!is_numeric($form_state['values']['office_mobile_number'])){
		form_error($element, t($element['#title'].' should be numeric.'));
	}
}
