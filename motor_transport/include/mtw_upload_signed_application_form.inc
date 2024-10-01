<?php

function mtw_upload_signed_application_form($form, &$form_state, $id=''){ 
	// die('Service Under Maintenance. Sorry for the inconvenience');
	
	global $base_root, $base_path, $user;
	
	$user_id = $user->uid;
    $application_id	= encryption_decryption_fun('decrypt', $id);	
		
	$fetch_app_details = db_select('l_mtw_registration_master', 'lbrm');
	$fetch_app_details->fields('lbrm', array('final_submit_status','created','application_date','status','backlog_id','is_renew','is_amend'));
	$fetch_app_details->condition('lbrm.user_id', $user_id);
	$fetch_app_details->condition('lbrm.id', $application_id);
	$fetch_app_details_result 	= $fetch_app_details->execute();	
	
	if( $fetch_app_details_result->rowCount() > 0 ){		
		$obj2 = $fetch_app_details_result->fetchAssoc();				
		$final_submit_status = $obj2['final_submit_status'];
		$status				 = !empty($obj2['status']) ? $obj2['status']:'';
		$application_date	 = !empty($obj2['application_date']) ? $obj2['application_date'] :'';
		$created			 = !empty($obj2['created'])? $obj2['application_date'] :'';
		$backlog_id			 = !empty($obj2['backlog_id']) ? $obj2['backlog_id'] : '' ;
		$is_renew			 = $obj2['is_renew'];
	}else{
		$is_renew =	0;
	}
		
	if( trim($status == 'T')  || trim($status == 'VA') || trim($status == 'U')){		
		
		$form['mtw_upload_signed_pdf']['header'] = array(
			'#type' => 'markup',
			'#markup' => '<div class="panel panel-default panel-form">
				<div class="panel-heading"> UPLOADED SIGNED FORM - I : </div>
				<div class="panel-body">'
		);
		
			
			
		if($is_renew == 1){
			$rquery = db_select('l_mtw_registration_renewal', 'rn');
			$rquery->fields('rn', array('id','renewal_ref_number'));
			$rquery->condition('rn.application_id', $application_id);
			$rquery->condition('rn.purpose', 'renewal');
			$db_or = db_or();
			$db_or->condition('rn.status', 'T');
			$db_or->condition('rn.status', 'U');
			$db_or->condition('rn.status', 'V');			
			$rquery->condition($db_or);			
			$renewal_id = $rquery->execute()->fetchObject()->id;
			$renewal_ref_number = $rquery->execute()->fetchObject()->renewal_ref_number;
			
			$downllink = 'mtw-renewgenerate-pdf/'.encryption_decryption_fun('encrypt', $application_id).'/'.encryption_decryption_fun('encrypt', 'self').'/'.encryption_decryption_fun('encrypt', $renewal_id);
		}else{
			$renewal_id = '';
			$renewal_ref_number = '';
			
			$rquery = db_select('l_mtw_registration_renewal', 'rn');
			$rquery->fields('rn', array('id','renewal_ref_number'));
			$rquery->condition('rn.application_id', $application_id);
			$rquery->condition('rn.purpose', 'registration');
			$db_or = db_or();
			$db_or->condition('rn.status', 'T');
			$db_or->condition('rn.status', 'U');
			$db_or->condition('rn.status', 'V');			
			$rquery->condition($db_or);			
			$renewal_id = $rquery->execute()->fetchObject()->id;
			$renewal_ref_number = $rquery->execute()->fetchObject()->renewal_ref_number;		
			
			$downllink = 'mtw-generate-pdf/'.encryption_decryption_fun('encrypt', $application_id).'/'.encryption_decryption_fun('encrypt', 'self');
		}
		
					
		$form['mtw_upload_signed_pdf']['download_link']	= array(
			'#type' => 'markup',
			'#markup' => '<div class="col-lg-6 col-md-12">'.l(t('Download FORM - I'),$downllink, array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('link-btn')))).'</div>'
		);	
		
		$form['mtw_upload_signed_pdf']['application_id'] = array(
			'#title'				 => 'Application Id',
			'#type' 				 => 'hidden',
			'#default_value'		 => !empty($application_id) ? $application_id : '',
			'#attributes'			 => array('readonly'=>'readonly')
		);
		
		$form['mtw_upload_signed_pdf']['renewal_id'] = array(
			'#title'				 => 'Application Id',
			'#type' 				 => 'hidden',
			'#default_value'		 => !empty($renewal_id) ? $renewal_id : 0,
			'#attributes'			 => array('readonly'=>'readonly')
		);
		$form['mtw_upload_signed_pdf']['renewal_ref_number'] = array(
			'#title'				 => 'Application Id',
			'#type' 				 => 'hidden',
			'#default_value'		 => !empty($renewal_ref_number) ? $renewal_ref_number : 0,
			'#attributes'			 => array('readonly'=>'readonly')
		);
				
		$form['mtw_upload_signed_pdf']['markup1'] =  array(
			'#type' => 'markup',
			'#markup' => '<div class="col-lg-6 col-md-12">'
		);
			
		$form['mtw_upload_signed_pdf']['form_i_signed'] = array(
			'#type'				=> 'managed_file',
			'#title'			=> 'Upload FORM I :-',
			'#upload_validators'=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
			'#upload_location'	=> 'public://upload/encrypt/',
			'#process' 			=> array('import_my_file_element_process'),
			'#attributes' 		=> array('id' => '','autocomplete' => 'off','class'=>array('')),
			'#required'			=> TRUE
		);
		
		$form['mtw_upload_signed_pdf']['markup2'] =  array(
			'#type' => 'markup',
			'#markup' => '</div>'
		);
		
		$form['mtw_upload_signed_pdf']['markup3'] =  array(
			'#type' => 'markup',
			'#markup' => '<div class="col-lg-12 col-md-12"><span style="color:red;"><b><u>NOTE</u> :-&nbsp;  FORM-I </b>should be duly signed and uploaded in order to obtain <b>Registration Certificate (FORM-II)</b>.</span></div>',
		);		
		
		$form['mtw_upload_signed_pdf']['submit'] = array(
			'#prefix' 				=> '<div class="col-lg-12 col-md-12">',  
			'#suffix' 				=> '</div>',  
			'#type' 				=> 'submit',
			'#attributes' 			=> array('class' => array('button pull-right')),
			'#value' 				=> 'FINAL SUBMIT'
		);
		
		$form['mtw_upload_signed_pdf']['footer'] = array(
			'#type'	=> 'markup',
			'#markup' => '</div>'
		);
		
	}else{
		if(!empty($application_id)){								
			$get_form_1_pdf= get_readable_pdf_signed_form($application_id);
			$form['showmsg'] = array(
				'#markup' => ''.$get_form_1_pdf.'',
			); 
		}
	}
	return $form;
}



function mtw_upload_signed_application_form_submit($form, &$form_state){ 

	global $base_root, $base_path, $user;
	
	
	$user_id	= $user->uid;
	$val 		= $form_state['values'];
	
	$application_id	= $val['application_id'];
	$renewal_id		= $val['renewal_id'];
	$renewal_ref_number = $val['renewal_ref_number'];
	
	// master table dada
	
	$info = db_select('l_mtw_registration_master','mtw')->fields('mtw',array('mtw_name'))->condition('mtw.id',$application_id)->execute();
	if($info->rowCount() > 0){
		$data = $info->fetchObject();
		$mtw_name = $data->mtw_name;
	}else{
		$mtw_name = '';
	}
	
	$file = file_load($val['form_i_signed']);
	$filename_arr = explode("/", $file->uri);
	$filename	= time().'_'.$filename_arr[4]; 
	
	
		
	$filepathSave 	= file_get_contents(escapefile_url($base_root.$base_path.'/sites/default/files/upload/encrypt/'.$filename_arr[4]));
	$content 		= base64_encode($filepathSave);
	$document_type_code	 = 'SFI';
	// store in db
	$arr = array(
		'user_id' => $user_id,
		'document_type' => 'Signed Form - I',
		'document_type_code' => $document_type_code, // Signed Form I
		'filename'		=> time().$filename,
		'filecontent'	=> $content,
		'update_date'	=> date('Y-m-d H:i:s'),
		'status'		=> '1',
		'act_id'		=> 3,
		'application_id' => $application_id
	);
	
	// update previous data which are status = 1;
	$query 			= db_update('l_encrypted_uploaded_documents')->fields(array('status'=> '0'))->condition('document_type_code',$document_type_code)->condition('user_id',$user_id)->condition('status','1');
	$query_result 	= $query->execute();
	// insert data into table
	$insert_id = db_insert('l_encrypted_uploaded_documents')->fields($arr)->execute();
	
	// update renewal table data
	
	$or = db_or()->condition('status','V')->condition('status','T')->condition('status','S');
	db_update('l_mtw_registration_renewal')->fields(array('signed_form_i' => $insert_id,'status'=>'S'))->condition('application_id',$application_id)->condition('user_id',$user_id)->condition($or)->execute();
	
	
	
			
		// remarks section 
		
		$fetch_users_role		= db_select('users_roles', 'ur');
		$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
		$fetch_users_role->fields('ro', array('rid'));
		$fetch_users_role->condition('ur.uid', $user_id);
		$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc();
	
		$user_role		=	$fetch_users_role_data['rid']; 
		
		$fieldsRemarksData 		= 	array( 
									'remark_by'					=> $user_id,
									'remark_to'					=> $user_id,
									'remark_type' 				=> 'S',
									'remark_date' 				=> time(),
									'remark_by_role'			=> $user_role,
									'remark_text'				=> 'Form I Successfully Submitted',
									'application_id'        	=> $application_id,
									'remark_by_name'    		=> $mtw_name,
									'call_time_applicant'		=> '00',
									'ref_number'				=> $renewal_ref_number,
									'renewal_id'				=> $renewal_id
								);
		db_insert('l_mtw_remark_master')->fields($fieldsRemarksData)->execute();
		
		// master dada update
		
		$sql_query = db_update('l_mtw_registration_master');
		$sql_query->fields(array(
				 			'status'  	=> 'S',
							));
		$sql_query->condition('id',$application_id);
		$sql_query->condition('user_id',$user_id);
		$sql_query->execute();	
		
		/////////////////  MTW API SERVICE FOR LD  /////////////////////////////////
		
		$api_q2 = 	db_select('l_common_application_master', 'lcam');
		$api_q2	->	fields('lcam', array('ld_uid'));
		$api_q2	->	condition('lcam.user_id', $user_id);
		$api_q2_result = $api_q2->execute(); 
		$api_ap2_details = $api_q2_result->fetchAssoc();
		
		$ld_id = trim($api_ap2_details['ld_uid']);
		
		if(!empty($ld_id) || $ld_id != ""){
			if($_SESSION['service_id'] == 3){
				drupal_goto('https://wblabour.gov.in/dashboard');
			}
		}
		//////////////////  END MTW API SERVICE FOR LD  /////////////////////////////////	
		
		$message = 'Documents is/are successfully uploaded. You will be further informed by ALC.';
		drupal_set_message(t($message));


}

function get_readable_pdf_signed_form($application_id){
	
	global $base_root, $base_parh, $user;
	
	$user_id	= $user->uid;
	// first check from encrypted table else old upload doc table
	$fetch_reg_upload_files = db_query("select * from l_encrypted_uploaded_documents where application_id=:application_id and user_id=:user_id and status = :status",array(':application_id'=>$application_id, ':user_id'=>$user_id, ':status' => 1));
	
	if($fetch_reg_upload_files->rowCount() > 0 ){
		$obj3 = $fetch_reg_upload_files->fetchAssoc(); 
		$row_id = $obj3['id'];
		
		$view_link = 'view_documents/'.encryption_decryption_fun('encrypt',$row_id);
		$pdficonlink = '<img title="View Documents" alt="View Documents" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/pdf.png">';
		
		
		$view_part = '';
		$view_part .= '<div class="col-lg-6 col-md-12"> Uploaded Signed Form I&nbsp;&nbsp;&nbsp;&nbsp; '.l($pdficonlink,$view_link,array('html' => TRUE, 'attributes' => array('class' => array(), 'target'=> '_blank' ))).'</div>';
		$view_part .= '<div class="col-lg-6 col-md-12">'.l('Download Acknowledgement','service-ack/'.encryption_decryption_fun('encrypt', 3).'/'.encryption_decryption_fun('encrypt', $application_id), array('html' => TRUE, 'attributes' => array('target' => '_blank', 'class' => array('')))).'</div>';
		
	}else{
		$fetch_reg_upload_files 	= db_query("select form_1_signed_pdf_file from l_documents_upload where application_id=:application_id and user_id=:user_id order by documents_id DESC limit 1 offset 0",array(':application_id'=>$application_id, ':user_id'=>$user_id));
		$form_1_signed_pdf_file =	$obj3['form_1_signed_pdf_file'];
		
		
		$view_part = '';
		
		
			
	}
	$output = '';	
	$output .='<div class="panel panel-default panel-form">
				<div class="panel-heading"> UPLOADED SIGNED FORM - I : </div>
				<div class="panel-body">'.$view_part.'Please note: This is a system generated Acknowledgement and does not need any signature.</div>
			</div>';
						
						
			 
	return $output; 
}


	
	


