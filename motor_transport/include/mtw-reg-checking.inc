<?php

function mtw_reg_checking($form, &$form_state){
	
	global $base_root, $base_path, $user;
	
	drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
	
															  
	$form['renewal_reference_no']['registration_no'] 	= array(
		'#prefix' => '<div class="panel panel-default panel-form">
						<div class="panel-heading">Apply for Renewal of Motor Transport Undertaking Registration Certificate</div>
						<div class="panel-body">
							<div class="row"><div class="col-lg-4 col-md-6"><label class="input">',
		'#suffix' => '</label></div></div>',
		
		
		'#title'		=> 'Enter your registration number:',
		'#type' 		=> 'textfield',
		'#required'		=>	TRUE,
		'#attributes'	=> array('autocomplete' => 'off')
	);
																	
	$form['renewal_reference_no']['submit'] = array (
		'#prefix' 		=> '',
		'#suffix' 		=> '</div></div>', 
		'#type' 		=> 'submit',
		'#value' 		=> 'Submit',
		'#attributes'	=> array('id' => 'submit_oneId' , 'class' => array('button left-btn'))
	);												
							
	return $form;
}

function mtw_reg_checking_submit($form, &$form_state){
	global $base_root, $base_path, $user;
	$val =	$form_state['values'];
	$registration_no = 	$val['registration_no']; 
	
	
	$mtw_reg = db_select('l_mtw_registration_master','lmtw');		
	$mtw_reg ->fields('lmtw',array('registration_number','id'));
	$mtw_reg ->condition('lmtw.registration_number',$registration_no);
	$ex	 = $mtw_reg->execute();

	if($ex->rowCount()>0){
		$result =	$ex->fetchObject();
		$application_id =	$result->id;
		// second time online renewal
		drupal_goto('mtw-renewal/'.encryption_decryption_fun('encrypt',$application_id));
	}else{
		$mtw_regb = db_select('l_mtw_registration_backlog','lmtw');		
		$mtw_regb ->fields('lmtw',array('registration_number','id'));
		$mtw_regb ->condition('lmtw.registration_number',$registration_no);
		$exb = $mtw_regb->execute();
		if($exb->rowCount()>0){
			$application_id	= $exb->fetchObject()->id;
			// backlog renewal all process
			//drupal_goto('mtw-backlog-renewal/'.encryption_decryption_fun('encrypt',$application_id));
			drupal_goto('mtw-registration/'.encryption_decryption_fun('encrypt',$application_id));
		}else{
			// no data found please enter valid registration number or contact to alc
			$message			=	"No data found please enter valid registration number or give all details.";
			drupal_set_message($message,'error');
			drupal_goto('mtw-already-registered/');
		}	
	}
}
