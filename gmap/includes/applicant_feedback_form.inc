<?php
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
function applicant_feedback_form($form, &$form_state){	  
					  
	$form['feedbackfrm']['top']=array(
		  '#type'		 => 'markup',
		  '#markup' 	 =>'<div class="panel panel-default panel-form">
								<div class="panel-heading">Enquiry / Grievance Form</div>												
								<div class="panel-body">'					  
					  					  				  
	);
	
	 
	$form['feedbackfrm']['subject']	= array(
			'#prefix' 		 	=>'<div class="row">
										<div class="col-md-6">
											<label class="select">',
			'#suffix' 	 		=> '		</label>
										</div>
									</div>',
			'#type'		 		=> 'select',
			'#options'    		=> get_all_enquiry_subject('ENQUIRY'),
			'#title'	 		=> t('Subject'),
			'#required'	 => TRUE					  
	);
						 
	$form['feedbackfrm']['comment']	= array(
			'#prefix' 			=> '<div class="row">
										<div class="col-md-6">
											<label class="textarea">',
			'#suffix' 			=> '		</label>
										</div>
									</div><br>',
			'#type'		 		=> 'textarea',
			'#title'	 		=> t('Comment'),
			'#element_validate' => array('MISC_TEXTBOX_TEXT_VALIDATE'),
			'#required'	 		=> TRUE,						
	);
												
	$form['feedbackfrm']['feedback_save'] = array(
			'#type'				=> 'submit',
			'#value'			=> 'Submit',
			'#attributes' 		=> array('class' => array('btn btn-primary')),
			'#prefix' 			=> '<div class="row">
										<div class="col-md-6">',  
			'#suffix' 			=> '	</div>
									</div>Note*:</div>
								</div>',						
	);
	
	$feedback_details = get_applicant_feedback_row();
	
	$form['feedbackfrm']['feedbacklist']=array(
			'#type'				=> 'markup',
			'#markup' 			=> '<div class="panel panel-default panel-form">
										<div class="panel-heading">Enquiry / Grievance Details</div>
										<div class="panel-body">
											<div class="sky-form">
												<div class="feedback-scroll">'.$feedback_details.'</div>
											</div>
										</div>
									</div>'
	);
	return $form;	
}

function get_applicant_feedback_row(){
	global $user;
	
	$header = array(					
					array('data' => 'Sl.No.', 'width' => '7%'),
					array('data' => 'Reference No.', 'width' => '15%'),
					array('data' => 'Date/Time', 'width' => '20%'),
					array('data' => 'Subject', 'width' => '25%'),
					array('data' => 'Comment'),
					array('data' => 'Status', 'width' => '15%'),					
					/* array('data' => 'Action', 'width' => '10%'), */   
				    );
	
	$fbrow_query = db_select('l_feedback', 'lfb');
	$fbrow_query->fields('lfb', array('id', 'subject', 'comment', 'submitted_date','status','status_details'));
	$fbrow_query->condition('lfb.user_id', $user->uid);
	$fbrow_query->orderBy('lfb.id', 'desc');
	$fbrow_query = $fbrow_query->execute(); 
	
	$counter = 0;
	
	if($fbrow_query->rowCount()){
		while($data = $fbrow_query->fetchAssoc()){
			$counter++;
			if($data['status'] == 'I'){$status = 'Initiated';}
			if($data['status'] == 'P'){$status = 'Under Process';}
			if($data['status'] == 'C'){$status = 'Completed';}
			$enc_id = encryption_decryption_fun('encrypt', $data['id']);	
			
			$rows[]  = 	array(
							array('data' =>	$counter, 'align' => 'left', 'class' => array($class)),
							array('data' =>	l('WBLCGREF'.$data['id'], 'applicant-feedback/details/'.$enc_id), 'align' => 'left', 'class' => array($class)),
							date('d/m/Y h:m A', $data['submitted_date']),
							array('data' =>	l($data['subject'], 'applicant-feedback/details/'.$enc_id), 'align' => 'left', 'class' => array($class)),
							array('data' =>	l($data['comment'], 'applicant-feedback/details/'.$enc_id), 'align' => 'left', 'class' => array($class)),
							$status,
							/* l('View', 'applicant-feedback/details/'.encryption_decryption_fun('encrypt', $data['id'])),	*/											
							);		
		}
	}
	$variables = array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('table table table-striped table-responsive dashboard-table')), 'empty' => t("No data found!"));						
	$output = theme('datatable', $variables);
	
	return $output;	
}

function applicant_feedback_form_submit($form, &$form_state){
	global $user;
	
	$val =	$form_state['values']; 
	
	$user_info = db_select('l_common_application_master', 'cm');
	$user_info->fields('cm', array('email', 'f_name', 'm_name', 'l_name', 'mobile', 'est_district', 'est_subdivision'));
	$user_info->condition('user_id', $user->uid);
	$user_info = $user_info->execute()->fetchAssoc();	
	
		
	if(!empty($user_info['m_name'])){
		$name = $user_info['f_name'].' '.$user_info['m_name'].' '.$user_info['l_name'];
	}else{
		$name = $user_info['f_name'].' '.$user_info['l_name'];
	}
	
	date_default_timezone_set("Asia/Kolkata");
	date_default_timezone_get();
	
	$query = db_select('l_feedback');
	$query->addExpression('MAX(id)');
	$query->condition('user_id',$user->uid);
	$result = $query->execute();
	$result = $result->fetchField();
	
	$subject_array = get_all_enquiry_subject('ENQUIRY');
	$subject = $subject_array[$val['subject']];
	
	$farray = array( 													
					'submitted_date'	=> time(),
					'name'				=> $name,
					'email'				=> $user_info['email'],
					'contact_no'  		=> $user_info['mobile'],
					'district'  		=> $user_info['est_district'],
					'subdivision' 		=> $user_info['est_subdivision'],								
					'subject' 			=> strip_tags($subject),
					'subject_id'		=> $val['subject'],
					'comment' 			=> strip_tags($val['comment']),
					'feedback_type'		=> 'S',
					'category'			=> 0,
					'user_id'	  		=> $user->uid,
					'is_active' 		=> 'Y',
					'is_read'   		=> 'N',
					'deleted'   		=> 'N',
					'is_published'   	=> 'N',
					'status'			=> 'I'
					);					
															   
	if(!empty($farray)){						   
		db_insert('l_feedback')->fields($farray)->execute();
		
		drupal_set_message(t('Your Enquiry successfully submitted with Reference No.- WBLCGREF'.$result));
		
		// SMS Alert
		$phone = trim($user_info['mobile']);
		
		$sms_msg = 'Your Enquiry has successfully sent to the concern official with Reference No.- WBLCGREF'.$result;
		send_sms_for_user_alert($phone, $sms_msg);
		
		// Mail Alert
		$mailto = trim($user_info['email']);
		$subject = 'Enquiry submited: wblc.gov.in';
		$message = 'Hi,<br>'.$name.', </br>Your Enquiry has successfully sent to the concern official with Reference No.- WBLCGREF'.$result;		
		send_mail_for_user_alert($mailto, $subject, $message, $name);
	}
}

function applicant_feedback_details_form($id = ''){
	$id	 =	encryption_decryption_fun('decrypt', $id);
		$header = array(					
					array('data' => 'Sl.No.', 'width' => '7%'),
					array('data' => 'Date/Time', 'width' => '20%'),
					array('data' => 'Subject', 'width' => '25%'),
					array('data' => 'Comment'),
				    );
	
	$fbrow_query = db_select('l_feedback', 'lfb');
	$fbrow_query->fields('lfb', array('id', 'subject', 'comment', 'submitted_date','status','status_details'));
		$db_or = db_or();
		$db_or->condition('lfb.master_id', $id);
		$db_or->condition('lfb.id', $id);
	$fbrow_query->condition($db_or);
	$fbrow_query->orderBy('lfb.id', 'desc');
	$fbrow_query = $fbrow_query->execute(); 
	
	$counter = 0;
	
	if($fbrow_query->rowCount()){
		while($data = $fbrow_query->fetchAssoc()){
			$counter++;
			if($data['status'] == 'I'){$status = 'Initiated';}
			if($data['status'] == 'P'){$status = 'Under Process';}
			if($data['status'] == 'C'){$status = 'Completed';}
			$enc_id = encryption_decryption_fun('encrypt', $data['id']);	
			
			$rows[]  = 	array(
							array('data' =>	$counter, 'align' => 'left', 'class' => array($class)),
							date('d/m/Y h:m A', $data['submitted_date']),
							array('data' =>	$data['subject']),
							array('data' =>	$data['comment']),
							//$status,
							);		
		}
	}
	
	$variables = array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('table table table-striped table-responsive dashboard-table')), 'empty' => t("No data found!"));
	$output = '<div class="panel panel-default panel-form">
					<div class="panel-heading">Enquiry / Grievance Details</div>
					<div class="panel-body">
						<div class="sky-form">
							<div class="feedback-scroll">';						
	$output .= theme('datatable', $variables);
	$output .= '			</div>
						</div>
					</div>
				</div>';
	return $output .'</br></br>'. l('<< BACK', 'applicant-feedback', array('attributes' => array('class' => 'link-btn btn btn-primary')));									
		
}


function get_all_enquiry_subject($type = ''){ 
	if(!empty($type)){
		$subj_query = db_select('l_grievance_category', 'lgc');
		$subj_query->fields('lgc', array('category','id'));
		$subj_query->condition('category_for',$type);
		$subj_query->condition('is_active',1);
		$subj_result 	= $subj_query->execute()->fetchAll(); 
		if(!empty($subj_result)){
			foreach($subj_result as $rw){
				$return[$rw->id] = $rw->category;				
			}
			return $return;
		}else{
			return array();
		}
	}else{
		return array();
	}
}
