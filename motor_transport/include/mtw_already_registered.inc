<?php
drupal_add_css(drupal_get_path('module', 'motor_transport') . '/css/sky-forms.css');
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
drupal_add_css(drupal_get_path('module', 'motor_transport').'/css/popup.css');
drupal_add_js(drupal_get_path('module', 'motor_transport') . '/js/mtw_upload_custom.js');
drupal_add_library('system','ui.datepicker');
		


	
function mtw_already_registered($form, &$form_state){
	global $base_root, $base_path;
	$form['form_begin'] = array(
		'#type' 	=> 'markup',
		'#markup' 	=> '<div class="panel panel-default panel-form">
							<div class="panel-heading">Renewal of Motor Transport Undertaking Registration Certificate </div>
							<div class="panel-body">',
	);
	
	
	$form['pre_reg_number'] = array(
		'#title' 		=> 'Enter Your Previous Registration Number',
		'#type'  		=> 'textfield',
		'#attributes'	=> array('id' => '','autocomplete' => 'off','class'=>array(''),'placeholder'=>'Previous Registration Number'),
		'#required' 	=>  TRUE,
		'#prefix' 		=> '<div class="row"><div class="col-lg-6 col-md-6"><label class="input">',
		'#suffix' 		=> '</label></div>',
	);
	
	$form['paidfees'] = array(
		'#title' 			=> 'Enter Your Previous Paid Registration Fees',
		'#type' 			=> 'textfield',
		'#attributes' 		=> array('id' => '','autocomplete' => 'off','class'=>array(''),'placeholder'=>'Previous Paid Registration Fees'),
		'#required' 		=>  TRUE,
		'#prefix' 			=> '<div class="col-lg-6 col-md-6"><label class="input">',
		'#suffix' 			=> '</label></div></div>'
	);
	
	$form['registration_date'] = array(
		'#title' 			=> 'Select Your Previous Registration Date (Date Format - MM/DD/YY)',
		'#type' 			=> 'textfield',
		'#attributes' 		=> array('id' => 'registration_date','autocomplete' => 'off','class'=>array(''),'placeholder'=>'Previous Registration Date','readonly'=>'readonly'),
		'#required' 		=>  TRUE,
		'#prefix' 			=> '<div class="row"><div class="col-lg-6 col-md-6"><label class="input">',
		'#suffix' 			=> '</label></div>'
	);
	$form['expiry_date'] = array(
		'#title' 			=> 'Select Your Previous Registration Expiry Date (Date Format - MM/DD/YY)',
		'#type' 			=> 'textfield',
		'#attributes' 		=> array('id' => 'expiry_date','autocomplete' => 'off','class'=>array(''),'placeholder'=>'Previous Registration Expiry Date','readonly'=>'readonly'),
		'#required' 		=>  TRUE,
		'#prefix' 			=> '<div class="col-lg-6 col-md-6"><label class="input">',
		'#suffix' 			=> '</label></div></div>'
	);
	
	$form['upload_rc_label_1'] = array(
		'#type'				=> 'markup',
		'#markup'			=> '<div class="row"><div class="col-lg-6 col-md-6"><label class="">',									
	);
	
	
	$form['upload_rc']			= array(
		'#type' 			=> 'managed_file',
		'#default_value'	=> $value,
		'#title' 			=> t('Upload Previous Registration Certificate (.pdf)'),
		'#required' 		=>  TRUE,
		'#process' 			=> array('import_my_file_element_process'),
		'#upload_location' 	=> 'public://upload/MTW-Backlog-RC/',
		'#upload_validators'=> array(
									'file_validate_extensions' => array('pdf'),
									'file_validate_image_resolution' => array(50*1024*1024),
								 ),						 
	);	
	
	$form['upload_rc_label_2'] = array(
		'#type'			=> 'markup',
		'#markup'		=> '</label></div>',								
	);
	
	$form['payments_doc_label_1'] = array(
		'#type'			=> 'markup',
		'#markup'		=> '<div class="col-lg-6 col-md-6"><label class="">',									
	);
	
	$form['payments_doc'] = array(
		'#type' 			=> 'managed_file',
		'#default_value'	=> $value,
		'#title' 			=> t('Upload Previous Payment Documents (.pdf)'),
		'#process' 			=> array('import_my_file_element_process'),
		'#upload_location' 	=> 'public://upload/MTW-Prv-Pay_Do/',
		'#upload_validators'=> array(
									'file_validate_extensions' => array('pdf'),
									'file_validate_image_resolution' => array(50*1024*1024),
								 ),				 
	);	
	
	$form['payments_doc_label_2'] = array(
		'#type'			=> 'markup',
		'#markup'		=> '</label></div></div>',									
	);
	
	
	$form['note'] = array(
		'#type'				=> 'markup',
		'#markup'			=> '<div class="row"><div class="col-lg-12 col-md-12"><span>Or &nbsp;</span>Click '.l(t('here'),'mtw-reg-checking/').' to back and re-enter your correct registration number.</div></div></div></div>',
	);
	
	$form['submit_button'] 		= array(
		'#type'				=>'submit',
		'#value'			=>t('Continue'),
		'#attributes' 		=> array('class' => array('button form-submit')),
	);
		
	return $form;
}

function mtw_already_registered_submit($form, &$form_state){
	global $base_root, $base_path;
	$val				=	$form_state['values'];
	
	$pre_reg_number		=	$val['pre_reg_number'];
	$paidfees			=	$val['paidfees'];
	$registration_date	=	$val['registration_date'];
	$expiry_date		=	$val['expiry_date'];
	
	
	$arr_1				= array();
	$upload_rc 			= file_load($val['upload_rc']);
	$upload_rc_filetype	= 'PREVIOUS REG CERTIFICATE';
	
	if( $upload_rc != "" ){
		$arr_1 = explode("/", $upload_rc->uri);
		$upload_rc->status = FILE_STATUS_PERMANENT;
		file_save($upload_rc);
		$upload_rc_file = $arr_1[4];
	}
	
	$arr_2				= array();
	$upload_pay 		= file_load($val['payments_doc']);
	$upload_pay_filetype	= 'PREVIOUS PAYMENT DOCS';
	
	if( $upload_pay != "" ){
		$arr_2 = explode("/", $upload_pay->uri);
		$upload_pay->status = FILE_STATUS_PERMANENT;
		file_save($upload_pay);
		$upload_pay_file = $arr_2[4];
	}
	
	$insertArray		=	array(
								'registration_number'		=>	$pre_reg_number,
								'registration_date'			=>	$registration_date,
								'expiry_date'				=>	$expiry_date,
								'registration_certificate'	=>	$upload_rc_file,
								'payments_doc'				=>	$upload_pay_file,
								'paidfees'					=>	$paidfees,
							);	
	
	$chkPrv				=	db_select('l_mtw_registration_backlog','lmrb');
	$chkPrv				->fields('lmrb',array('id'));
	$chkPrv				->condition('lmrb.registration_number',$pre_reg_number);
	$chkPrvResult		=	$chkPrv->execute();
	if($chkPrvResult->rowCount() == 0){
		$inserBacklog_Id	=	db_insert('l_mtw_registration_backlog')->fields($insertArray)->execute();
		drupal_goto('mtw-registration/'.encryption_decryption_fun('encrypt', $inserBacklog_Id));
	}else{
		$backlogData	= $chkPrvResult->fetchObject();
		$backlogId		= $backlogData->id;
		
		// update backlog information
		
		$upArray		=	array(
								'registration_date'			=>	$registration_date,
								'expiry_date'				=>	$expiry_date,
								'registration_certificate'	=>	$upload_rc_file,
								'payments_doc'				=>	$upload_pay_file,
								'paidfees'					=>	$paidfees,
							);
		
		db_update('l_mtw_registration_backlog')->fields($upArray)->condition('registration_number',$pre_reg_number)->execute();
		
		
		
		// checking for half draft data for registration
		$chkHalfDraft	= db_select('l_mtw_registration_master','lmrm');
		$chkHalfDraft	->fields('lmrm',array('id'));
		$chkHalfDraft	->condition('lmrm.backlog_id',$backlogId);
		$chkHalfDraftResult	= $chkHalfDraft->execute();
		if($chkHalfDraftResult->rowCount() >0 ){
			$applicationArr 	=	$chkHalfDraftResult->fetchObject();
			$application_id		=	$applicationArr->id;
			
			$message			=	"System Registration already done. Please complete the form to submit.";
			drupal_set_message($message);
			drupal_goto('mtw-registration-details/default/'.encryption_decryption_fun('encrypt', $application_id));
		}else{
			$message			=	"Please fillup the form for System Registration.";
			drupal_set_message($message);
			drupal_goto('mtw-registration/'.encryption_decryption_fun('encrypt', $inserBacklog_Id));
		}
	}
}

