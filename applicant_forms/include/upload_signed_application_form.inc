<?php 

function upload_signed_application_form($form, &$form_state, $id=''){
	
	global $base_root, $base_path, $user;
	
	$user_id					= $user->uid;
	$application_id				= encryption_decryption_fun('decrypt', $id);
	
	
	$fetch_app_details 			= db_select('l_clra_registration_master', 'lcrm');
	$fetch_app_details			->fields('lcrm', array('final_submit_status','created','application_date','status','backlog_id','amendment_parent_id'));
	$fetch_app_details			->condition('lcrm.user_id', trim($user_id));
	$fetch_app_details			->condition('lcrm.id', trim($application_id));
	$fetch_app_details_result 	= $fetch_app_details->execute();
			
	if( $fetch_app_details_result->rowCount() > 0 ){
		
				$application_details 	    			= $fetch_app_details_result->fetchAssoc();
				
				
				$final_submit_status					= $application_details['final_submit_status'];
				$status									= !empty($application_details['status']) ? $application_details['status']:'';
				$application_date						= !empty($application_details['application_date']) ? $application_details['application_date'] :'';
				$created								= !empty($application_details['created'])? $application_details['application_date'] :'';
				$backlog_id								= !empty($application_details['backlog_id']) ? $application_details['backlog_id'] : '' ;
				$amendment_parent_id					= $application_details['amendment_parent_id'];
			
	}
		
	if(trim($status == 'T') || trim($status == 'VA')|| trim($status == 'U' )){
		
	$form['upload_signed_pdf']['header']			 				= array(
																		  '#type' 				=> 'markup',
																		  //'#markup' 			=> t('<div class="content"><div class="sky-form" style="width:100%;"><header><h3><b>Upload Documents</b></h3></header>'),
																		  '#markup' 			=> t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">Upload Documents</div><div class="panel-body">'),
																		  
																	);
																		
	$form['upload_signed_pdf']['application_id'] 					= array(
																		  '#title'				=> 'Application Id',
																		  '#type' 				=> 'hidden',
																		  '#default_value'		=> !empty($application_id) ? $application_id : '',
																		  '#attributes'			=> array('readonly'=>'readonly')
																	);
																		
	$form['upload_signed_pdf']['row1'] 								=  array(
																		 '#markup' 			=> '<div class="col-lg-12 col-md-12">',
																		);
																		
																		
																		  
	$form['upload_signed_pdf']['download_link']						= array(
																		'#markup' 				=> l(t('Download FORM - I'),'generate-pdf-applicant/'.encryption_decryption_fun('encrypt', $application_id), array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('link-btn')))),
																		
																		);	
																		
																															  
	$form['upload_signed_pdf']['row2'] 								=  array(
																		 '#type'				=> 'markup',
																		 '#markup' 				=> '</div><div class="col-lg-6 col-md-12">',
																		);																	  

	$form['upload_signed_pdf']['form_i_signed'] 					= array(
																		'#type' 				=> 'managed_file',
																		'#title'				=>'Upload Signed FORM I ',
																		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																		'#upload_location' 		=> 'public://upload/FORM-I/',
																		'#process' 				=> array('import_my_file_element_process'),
																		'#required' 			=> TRUE,
																		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																		'#field_prefix' 		=> '<label class="input-file">',
																		'#field_suffix' 		=> '</label>'
																		);
																		
																		
	$form['upload_signed_pdf']['row3'] 								=  array(
																		  '#type'				=> 'markup',
																		  '#markup' 			=> '</div>',
																		  );
																		
																		  
	
	if ( (!empty($backlog_id) && $backlog_id!='')&& $amendment_parent_id=='') {																  
																			
	$form['upload_signed_pdf']['row4'] 								=  array(
																		  '#type'				=> 'markup',
																		  '#markup' 			=> '<div class="col-lg-6 col-md-12">',
																		  );																	  																	
																		
																		
	$form['upload_signed_pdf']['clra_backlog_certificate']  		= array(
																		'#type' 				=> 'managed_file',
																		'#title'				=>'Upload Previous Registration Certificate',
																		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																		'#upload_location' 		=> 'public://upload/backlog_clra_registration_certificate/',
																		'#process' 				=> array('import_my_file_element_process'),
																		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																		'#field_prefix' 		=> '<label class="input-file">',
																		'#field_suffix' 		=> '</label>',
																		'#required' 			=> TRUE,
																		);
	
	
	$form['upload_signed_pdf']['row5'] 								=  array(
																		'#type'				=> 'markup',
																		'#markup' 			=> '</div>',
															
																		  );
																		  
																		  
	}
	
	
	$form['upload_signed_pdf']['row6'] 								=  array(
																		'#markup' 			=> '<div class="col-lg-12 col-md-12"><span style="color:red;"><b><u>NOTE</u> :-&nbsp;  FORM-I </b>should be duly signed and uploaded in order to obtain <b>Registration Certificate (FORM-II)</b>.</span></div>',
																		);																	
	
	
	$form['upload_signed_pdf']['submit'] 							= array(
																		'#prefix' 			=> '</div></div>',  
																		'#suffix' 			=> '</div>',  
																		'#type' 				=> 'submit',
																		'#attributes' 		=> array('class' => array('btn btn-primary pull-right')),
																		'#value' 				=> 'SUBMIT'
																		   );
	}else {
		
		if(!empty($application_id)){								
			$get_form_1_pdf= get_readable_pdf_signed_form($application_id,$backlog_id);
				$form['showmsg'] 			= array(
												'#markup' => ''.$get_form_1_pdf.'',
												); 
		}
		
	}
	return $form;
	
}


function upload_signed_application_form_submit($form, &$form_state){ 
	global $base_root, $base_path, $user;
	
	
	$user_id								= $user->uid;
	$val 									= $form_state['values'];
	
	$application_id							= $val['application_id'];
	
	
	/* Upload For Form -I */
	
	$arr_1									= array();
	$signed_form 							= file_load($val['form_i_signed']);
	if( $signed_form != "" ){
		$arr_1 = explode("/", $signed_form->uri);
		$signed_form->status = FILE_STATUS_PERMANENT;
		file_save($signed_form);
		$signed_form_file = $arr_1[4];
	}
	
	
	/* Upload For Backlog Registration Certificate */
	
	$arr_3									= array();
	$backlog_cetificate 					= file_load($val['clra_backlog_certificate']);
	if( $backlog_cetificate != "" ){
		$arr_3 = explode("/", $backlog_cetificate->uri);
		$backlog_cetificate->status = FILE_STATUS_PERMANENT;
		file_save($backlog_cetificate);
		$backlog_cetificate_file = $arr_3[4];
	}
	
	
	$fetch_reg_det		=	db_query("select application_date, created,full_name_principal_emp from l_clra_registration_master where id=:application_id and user_id=:user_id", array( ':application_id' => $application_id, ':user_id' => $user_id ));
	
		if( $fetch_reg_det->rowCount() > 0 ){
			
				 	$obj2 									= $fetch_reg_det->fetchAssoc();
					$application_date						= !empty($obj2['application_date']) ? $obj2['application_date'] :'';
					$created_date							= !empty($obj2['created'])? $obj2['application_date'] :'';
					$principal_emp_name						= $obj2['full_name_principal_emp'];
				
		}
		
		$fetch_users_role		= db_select('users_roles', 'ur');
		$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
		$fetch_users_role->fields('ro', array('rid'));
		$fetch_users_role->condition('ur.uid', $user_id);
		$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc();
	
		$user_role		=	$fetch_users_role_data['rid']; 
		
		$fieldsRemarksData 		= 	array( 
									'remarks'				=>	'Form I Successfully Submitted.',
									'remark_by'				=>  $user_id,
									'remark_to'				=>	$user_id,
									'application_id'        =>  $application_id,
									'remark_type' 			=>  'S',
									'remark_date' 			=>  time(),
									'remark_by_name'		=> 	$principal_emp_name,
									'remark_by_role'		=> 	$user_role
								);
				
	db_insert('l_remarks_master')->fields($fieldsRemarksData)->execute();
	
	$queryForward = db_update('l_documents_upload');
	$queryForward->fields(array('form_1_clra_signed_pdf_file'=>	$signed_form_file,'backlog_clra_registration_certificate_file' => $backlog_cetificate_file));
	$queryForward->condition('user_id',$user_id);
	$queryForward->condition('application_id',$application_id);
	$queryForward->execute();
		
	
	$sql_query = db_update('l_clra_registration_master');
	$sql_query->fields(array(
			 'status'  => 'S',
	));
	$sql_query->condition('id',$application_id);
	$sql_query->condition('user_id',$user_id);
	$sql_query->execute();
	
	//////////////////  CLRA API SERVICE FOR LD  /////////////////////////////////	
	
	$api_q2 = db_select('l_common_application_master', 'lcam');
	$api_q2->fields('lcam', array('ld_uid', 'ld_service_unique_id', 'ld_est_unique_id', 'clra_ld_service_id'));
	$api_q2->condition('lcam.user_id', $user_id);
	$api_q2_result = $api_q2->execute(); 
	$api_ap2_details = $api_q2_result->fetchAssoc();	
	$ld_id = trim($api_ap2_details['ld_uid']);
	
	if(!empty($ld_id) || $ld_id != ""){
		$status_arr = array (
 						'status' => '0',
 						'id' => $api_ap2_details['clra_ld_service_id'],
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
	   
		if($_SESSION['service_id'] == 1){
			drupal_goto('https://wblabour.gov.in/dashboard');
		}
	}
	//////////////////  END CLRA API SERVICE FOR LD  /////////////////////////////////	
		
	$message = 'Documents is/are successfully uploaded. You will be further informed by ALC ';
	drupal_set_message(t($message));


}

function get_readable_pdf_signed_form($application_id='',$backlog_id=''){
	
	global $base_root, $base_parh, $user;
	
	$user_id	= $user->uid;
	
	$fetch_reg_upload_files 	= db_query("select form_1_clra_signed_pdf_file, backlog_clra_registration_certificate_file from l_documents_upload where application_id=:application_id and user_id=:user_id",array(':application_id'=>$application_id, ':user_id'=>$user_id));
		
		if( $fetch_reg_upload_files->rowCount() > 0 ){
			
				$obj3 = $fetch_reg_upload_files->fetchAssoc(); 
				
				$form_1_clra_signed_pdf_file					=	$obj3['form_1_clra_signed_pdf_file'];
				$backlog_clra_registration_certificate_file		=	$obj3['backlog_clra_registration_certificate_file'];
				
		}
		
		
		$output='';
		$output.= '<div class="content">
					<div class="panel panel-default panel-form">
						<div class="panel-heading">UPLOADED DOCUMENTS :</div>
							<div class="panel-body">';
								if(!empty($form_1_clra_signed_pdf_file)){
									$output .= '<div class="col-lg-6">
												<label class="input"> Uploaded Signed Form I&nbsp;&nbsp;&nbsp;&nbsp;
												<a href="'.$GLOBALS['base_url'].'/sites/default/files/upload/FORM-I/'.$form_1_clra_signed_pdf_file.'" target="_blank"><img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/lc-pdf.png"></a>
												</label>
											</div>';
								}
													
								
							
							
							if(!empty($backlog_id) && $backlog_id!='')	 {		
				
								if(!empty($backlog_clra_registration_certificate_file)){
									$output .= '<div class="col-lg-6">
												<label class="input"> Upload Previous Registration Certificate&nbsp;&nbsp;&nbsp;&nbsp;
												<a href="'.$GLOBALS['base_url'].'/sites/default/files/upload/backlog_clra_registration_certificate/'.$backlog_clra_registration_certificate_file.'" target="_blank"><img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/lc-pdf.png"></a>
												</label>
											</div>';
								
								}
							
							
							}

							
				$output.= 	'<div class="row">&nbsp;&nbsp;</div><div class="row">&nbsp;&nbsp;</div>
							</div>
						</div>
				 </div>';
				 
				 return $output; 
}



