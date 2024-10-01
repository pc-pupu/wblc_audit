<?php

function bocwa_upload_signed_application_form($form, &$form_state, $id=''){ 

	global $base_root, $base_path, $user;
	
	$user_id			= $user->uid;
    $application_id		= encryption_decryption_fun('decrypt',$id);
	
	
	
	$fetch_app_details 			= db_select('l_bocwa_registration_master', 'lbrm');
	$fetch_app_details			->fields('lbrm', array('final_submit_status','created','application_date','status','backlog_id','amendment_parent_id'));
	$fetch_app_details			->condition('lbrm.user_id', $user_id);
	$fetch_app_details			->condition('lbrm.id', $application_id);
	$fetch_app_details_result 	= $fetch_app_details->execute();
	
	
	
	if( $fetch_app_details_result->rowCount() > 0 ){
		
				$obj2 = $fetch_app_details_result->fetchAssoc();
				
				
				$final_submit_status					= $obj2['final_submit_status'];
				$status									= !empty($obj2['status']) ? $obj2['status']:'';
				$application_date						= !empty($obj2['application_date']) ? $obj2['application_date'] :'';
				$created								= !empty($obj2['created'])? $obj2['application_date'] :'';
				$backlog_id								= !empty($obj2['backlog_id']) ? $obj2['backlog_id'] : '' ;
			
	}
	if(!empty($obj2['amendment_parent_id'])){$form_1_name='Amended application';}else{$form_1_name='FORM I';}	
	if(trim($status == 'T') || trim($status == 'VA')|| trim($status == 'U' )){
		
		$form['bocwa_upload_signed_pdf']['header']			 					= array(
																			  '#type' 				=> 'markup',
																			  '#markup'		 		=> '<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">Upload Documents</div><div class="panel-body">',
																			);
																			
		$form['bocwa_upload_signed_pdf']['application_id'] 				= array(
																			  '#title'				=> 'Application Id',
																			  '#type' 				=> 'hidden',
																			  '#default_value'		=> !empty($application_id) ? $application_id : '',
																			  '#attributes'			=> array('readonly'=>'readonly')
																			);
																			
		$form['bocwa_upload_signed_pdf']['row1'] 						=  array(
																			  '#markup' 			=> '<div class="col-lg-12">',
																			  );
																			  
																			  
		$form['bocwa_upload_signed_pdf']['download_link']			    = array(
		
																		  '#markup' 				=> l(t('Download '.$form_1_name.''),'bocwa-generate-pdf/'.encryption_decryption_fun('encrypt', $application_id).'/'.encryption_decryption_fun('encrypt', $user_id), array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('link-btn')))),
																		
																				);	
																		
		$form['bocwa_upload_signed_pdf']['row2'] 						=  array(
																		   '#type'					 => 'markup',
																		   '#markup' 				 => '</div><br><br><div class="col-lg-12 col-md-12">',
																		);	
																		
																	
		
		$form['bocwa_upload_signed_pdf']['form_i_signed'] 				= array(
																			'#type' 				 => 'managed_file',
																			'#title'				 => 'Upload '.$form_1_name.' :-',
																			'#upload_validators' 	 => array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																			'#upload_location' 		 => 'public://upload/encrypt/',
																			'#process' 				 => array('import_my_file_element_process'),
																			'#attributes' 			 => array('id' => '','autocomplete' => 'off','class'=>array('')),
																			'#field_prefix' 		 => '<label class="input-file">',
																			'#field_suffix' 		 => '</label>',
																			'#required' 			 => TRUE
																			);
		
		
		
		$form['bocwa_upload_signed_pdf']['row3'] 						=  array(
																			'#markup' 				=> '</div>',
																			  );
																			  
		$form['bocwa_upload_signed_pdf']['row4'] 						=  array(
																		  	'#markup' 				=> '<div class="col-lg-12"><span class="red-note">NOTE :- '.$form_1_name.' should be duly signed and uploaded for successful submission of the Application.</span></div>',
																		  );
		
		$form['bocwa_upload_signed_pdf']['submit'] 					= array(
																			'#prefix' 				=> '</div></div>',  
																			'#suffix' 				=> '</div>',  
																			'#type' 				=> 'submit',
																			'#attributes' 			=> array('class' => array('btn btn-primary pull-right')),
																			'#value' 				=> 'FINAL SUBMIT'
																			  );
	}else {
		
		if(!empty($application_id)){								
			$get_form_1_pdf= get_readable_pdf_signed_form($application_id);
				$form['showmsg'] 			= array(
												'#markup' => ''.$get_form_1_pdf.'',
												); 
		}
		
	}
	return $form;
	
}



function bocwa_upload_signed_application_form_submit($form, &$form_state){ 

	global $base_root, $base_path, $user;
	$user_id								= $user->uid;
	$val 									= $form_state['values'];
	$application_id							= $val['application_id'];
	/* Upload For Form -I */
	$arr_1									= array();
	$signed_form 							= file_load($val['form_i_signed']);
	if( !empty($val['form_i_signed'])){
		$form_i_signed 			= file_load($val['form_i_signed']);
		$form_i_signed_arr 		= explode("/", $form_i_signed->uri);
		$form_i_signed_fname	= time().'_'.$form_i_signed_arr[4]; 
		$filepathSave_tl 		= file_get_contents(escapefile_url($base_root.$base_path.'sites/default/files/upload/encrypt/'.$form_i_signed_arr[4]));
		$form_i_signed_content 	= base64_encode($filepathSave_tl);
		$insert_arr['FI']['fname'] = $form_i_signed_fname;
		$insert_arr['FI']['content'] = $form_i_signed_content;
	}
	
	
	$fetch_reg_det = db_query("select application_date, created,emp_name,amendment_parent_id from l_bocwa_registration_master where id=:application_id and user_id=:user_id", array( ':application_id' => $application_id, ':user_id' => $user_id ));
	
		if( $fetch_reg_det->rowCount() > 0 ){
			
			$obj2 									= $fetch_reg_det->fetchAssoc();
			$application_date						= !empty($obj2['application_date']) ? $obj2['application_date'] :'';
			$created_date							= !empty($obj2['created'])? $obj2['application_date'] :'';
			$emp_name								= $obj2['emp_name'];
				
		}
	
		$fetch_users_role		= db_select('users_roles', 'ur');
		$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
		$fetch_users_role->fields('ro', array('rid'));
		$fetch_users_role->condition('ur.uid', $user_id);
		$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc();
	
		$user_role		=	$fetch_users_role_data['rid']; 
		
		$fieldsRemarksData 	= array( 
									'remark_by'					=>  $user_id,
									'remark_to'					=>	$user_id,
									'remark_type' 				=>  'S',
									'remark_date' 				=>  time(),
									'remark_by_role'			=> 	$user_role,
									'remark_text'				=>	'Form I Successfully Submitted',
									'application_id'        	=>  $application_id,
									'remark_by_name'    		=>  $emp_name,
									'call_time_applicant'		=>  '00'
								);
	
	
	db_insert('l_bocwa_remark_master')->fields($fieldsRemarksData)->execute();
	
	if(!empty($insert_arr)){
		foreach($insert_arr as $key => $row){ 
			$arr = array(
				'user_id' => $user_id,
				'act_id' => 2,
				'application_id' => $application_id,
				'identification_number' => $identification_number,
				'document_type' => document_list($key),
				'document_type_code' => $key,
				'filename'		=> time().$insert_arr[$key]['fname'],
				'filecontent'	=> $insert_arr[$key]['content'],
				'update_date'	=> date('Y-m-d H:i:s'),
				'status'		=> '1'
			);
			
		// update previous data which are status = 1;
		$query 			= db_update('l_encrypted_uploaded_documents')->fields(array('status'=> '0'))->condition('document_type_code',$key)->condition('user_id',$user_id)->condition('status','1');
		$query_result 	= $query->execute();
		
		// insert data into table
		db_insert('l_encrypted_uploaded_documents')->fields($arr)->execute();
		
		//////////////////  BOCWA API SERVICE FOR LD  /////////////////////////////////	
	
		$api_q2 					= 	db_select('l_common_application_master', 'lcam');
		$api_q2						->	fields('lcam', array('ld_uid', 'ld_service_unique_id', 'ld_est_unique_id', 'bocwa_ld_service_id'));
		$api_q2						->	condition('lcam.user_id', $user_id);
		$api_q2_result 				= 	$api_q2->execute(); 
		$api_ap2_details			=	$api_q2_result->fetchAssoc();
		
		$ld_id						=	trim($api_ap2_details['ld_uid']);
		
		if(!empty($ld_id) || $ld_id != ""){
			$status_arr = array (
							'status' => '0',
							'id' => $api_ap2_details['bocwa_ld_service_id'],
							'ld_uid' => $api_ap2_details['ld_uid'],
							'ld_est_unique_id' => $api_ap2_details['ld_est_unique_id'],
							);
			$array_enc = encrypt_swold($status_arr);
			$curl_post_data = array ('message' => $array_enc, 'data' => $array_enc); 		
					
			$service_url  =   "https://wblabour.gov.in/restld/establishmentStatusUpdate";
			$ch       =   curl_init();
			$headers    = array();
			$headers[]    = 'Accept: application/json';
			$headers[]    = 'Content-Type: application/json';
			$headers[]    =   'charset=utf-8';
			curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
			curl_setopt($ch, CURLOPT_URL, $service_url);
			curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
			curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
			curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
			curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 0);
			curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode( $curl_post_data ) );
			curl_setopt($ch, CURLOPT_POST, true);
			$results    = curl_exec($ch);
			$errors     = curl_error($ch);
			curl_close($ch);
			$x = json_encode( $results );
			$tmp = json_decode(json_decode(trim($x)), TRUE);
	 
		   if($errors){
			//  echo "<br>";
			//  print_r(curl_errno());
			//  print_r($errors);
		   }else{
			// echo "<br>";
			// echo "Ok";
			// echo "<br>";
		   }
		   
		   // print_r($results); echo "<br>";
		   // $return_msg_encrypted = json_decode($results, TRUE);
		   // $return_msg = decrypt_swold($return_msg_encrypted['message']);
		   // print_r($return_msg);
			if($_SESSION['service_id'] == 2){
				drupal_goto('https://wblabour.gov.in/dashboard');
			}
		}
		//////////////////  END BOCWA API SERVICE FOR LD  /////////////////////////////////		
		}
		
		if(!empty($obj2['amendment_parent_id'])){$form_1_name='Amended application';}else{$form_1_name='FORM I';}
		$sql_query = db_update('l_bocwa_registration_master');
		$sql_query->fields(array(
				 'status'  				=> 'S',
		));
		$sql_query->condition('id',$application_id);
		$sql_query->condition('user_id',$user_id);
		$sql_query->execute();	
		
		$message = $form_1_name.' uploaded successfully';
		drupal_set_message(t($message));

	}
}

function get_readable_pdf_signed_form($application_id){
	
	global $base_root, $base_parh, $user;
	
	$user_id	= $user->uid;
	
	$fetch_reg_upload_files 	= db_query("select form_1_bocwa_signed_pdf_file from l_documents_upload where application_id=:application_id and user_id=:user_id",array(':application_id'=>$application_id, ':user_id'=>$user_id));
		
		if( $fetch_reg_upload_files->rowCount() > 0 ){
			
			$obj3 = $fetch_reg_upload_files->fetchAssoc(); 
			
			$form_1_bocwa_signed_pdf_file =	$obj3['form_1_bocwa_signed_pdf_file'];
				
		}
		
		$fetch_reg_det = db_query("select amendment_parent_id from l_bocwa_registration_master where id=:application_id and user_id=:user_id", array( ':application_id' => $application_id, ':user_id' => $user_id ));
	
		if( $fetch_reg_det->rowCount() > 0 ){
			
			$obj2 									= $fetch_reg_det->fetchAssoc();
		}
		if(!empty($obj2['amendment_parent_id'])){$form_1_name='Amended application';}else{$form_1_name='FORM I';}
		
		$output='';
		$output.= '<div class="content">
						<div class="panel panel-default panel-form">
							<div class="panel-heading">UPLOADED SIGNED '.$form_1_name.' :</div>
								<div class="panel-body">';
								if(!empty($form_1_bocwa_signed_pdf_file)){
									$output .= '<div class="col-lg-6 col-md-12">
												<label class="input"> Uploaded Signed Form I&nbsp;&nbsp;&nbsp;&nbsp;
												<a href="'.$GLOBALS['base_url'].'/sites/default/files/upload/BOCWA_Form-I/'.$form_1_bocwa_signed_pdf_file.'" target="_blank"><img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>
												</label>
											</div>';
								}
								
								$output .= '</div>
								
							</div>
				 		</div>';
				 
				 return $output; 
}


	
	


