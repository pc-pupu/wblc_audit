<?php
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');

function check_reference_no($form, &$form_state){
	global $base_root, $base_path, $user;
	$contractor_id 				= 	$user->uid;
		
	$form['check_service']['header'] = array(
		'#markup' 			=>	 t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading"> You can check which service you are eligible to apply by entering your FORM-V Number or reference Number.</div><div class="panel-body">'),
		'#type' 			=> 	'markup',
	);
	
	$form['check_service']['reference_no'] 	= 	array(
		'#prefix' 			=> 	'<div class="col-md-12"><label class="input">',
		'#suffix' 			=> 	'</div></label>',
		'#type' 			=> 	'textfield',
		'#title'			=> 	'Enter your reference number / serial number of FORM-V (provided by principal employer).:',
		'#required'			=>	TRUE,
		'#description' 		=> 	l('Click here to view sample FORM V', '/sites/default/files/upload/contractor_license_from_v/sample_form_V.pdf', array('html' => TRUE,'attributes'=>array('target' => '_blank', 'title' => 'Click here to view sample FORM V'))),
		'#attributes'		=> 	array('autocomplete' => 'off')
	);
			
	$form['check_service']['license_no'] 	= 	array(   
		'#prefix' 			=> '<div id="pensionstatusapp1">',
		'#suffix' 			=> '</div>'
	);
			
	$form['check_service']['license_no'] 		= 	array(
		'#prefix' 			=> 	'<div class="col-md-12" id="pensionstatusapp1"><label class="input">',
		'#suffix' 			=> 	'</label></div>',
		'#title'			=> 	'Enter your License Number:',
		'#type' 			=> 	'textfield',
		'#default_value' 	=>	!empty($license_no) ? $license_no : '',
		'#required'			=>	TRUE,
		'#attributes'		=> 	array('autocomplete' => 'off')
	);
	
	$form['check_service']['pensionstbt'] 		= 	array(	   
		'#prefix' 			=> '<div class="col-md-12" id="pensionstbtid">',
		'#suffix' 			=> '</div>', 
		'#type' 			=> 	'button',
		'#attributes' 		=> array('class' => array('btn btn-primary pull-right')),
		'#default_value' 	=> 	'Check Availability',
		'#ajax' 			=> 	array(
								'callback' 	=> 'check_availability_submit',
								'wrapper' 	=> '',
								'method' 	=> 'html',
								'effect' 	=> 'fade'
							)
			);
	
	$value_button 			= 	isset($form_state['values']['pensionstbt']) ? $form_state['values']['pensionstbt'] : '';							
	$value_reference_no 	= 	isset($form_state['values']['reference_no']) ? $form_state['values']['reference_no'] : '';
	
	if(!empty($value_reference_no)){
		$form['check_service']['pensionstbt'] = array(	   
			'#prefix' 	=> '<div id="pensionstbtid">',
			'#suffix' 	=> '</div>', 
			);
	}
	
	$form['check_service']['application_message'] = array(   
			'#prefix' 	=> '<div id="message_id">',
			'#suffix' 	=> '</div>'
			);
										
	$form['check_service']['application_type'] = array(   
			'#prefix'  => '<div id="pensionstatusapp2">',
			'#suffix'  => '</div>'
			);
										
	$form['check_service']['submit'] 	= 	array(   
			'#prefix'  => '<div id="pensionstatusapp3">',
			'#suffix'  => '</div>'
			);
	
	if(!empty($value_reference_no)){
		$record = basic_information_for_ammendment($value_reference_no); 
		$form['check_service']['pensionstbt'] = array(   
			'#prefix' 	=> '<div id="pensionstbtid">',
			'#suffix' 	=> '</div>'
			);	
		
	if(!empty($record)){
		// if($contractor_id == 6760){
		// 	echo "<pre>";
		// 	print_r($record);die;
		// }
		
		$license_no = !empty($record['backlog_license_no'])?$record['backlog_license_no']:$record['contractor_license_number'];
		$form['check_service']['license_no'] = 	array(
			'#prefix' 		=> 	'<div class="col-md-12" id="pensionstatusapp1"><label class="input">',
			'#suffix' 		=> 	'</label></div>',
			'#title'		=> 	'Enter your License Number:',
			'#type' 		=> 	'textfield',
			'#value' 		=>	!empty($license_no) ? $license_no : '',
			'#required'		=>	TRUE,
			'#attributes'	=> 	array('autocomplete' => 'off')
		);
	}
	$query_contractor_type 		= 	db_select('l_contractor_info_master', 'infomaster');
	$query_contractor_type		->	fields('infomaster',array('contractor_type'))->condition('id',(int)$value_reference_no,'=');
	$contractor_type 			= 	$query_contractor_type->execute()->fetchObject()->contractor_type;
	
	if($contractor_type == 2){
		$application_status			=	'Click here to continue';
		$application_status_1		=	'check_fromv_no/';
		$application_status_show	=	l($application_status,$application_status_1,array('html' => TRUE));
		$application_type			=	application_type($value_reference_no,$contractor_id);
			
		if($application_type['BR'] == 'Renewal of License'){
			$form['check_service']['application_message'] 	= 	array(   
					'#prefix' 		=> '<div id="message_id"><section><label class="input">',
					'#type' 		=> 	'markup',
					'#suffix' 		=> '</div></section></label>',
					'#markup'		=> '<font color="#FF0000"><b>You are required to apply for License</b></font>  '.$application_status_show
					);
		}else{
			$application_type		= application_type($value_reference_no,$contractor_id);
	
			$form['check_service']['application_type'] 	= 	array(
				'#prefix' 			=> 	'<div class="col-md-12" id="pensionstatusapp2"><label class="select">',
				'#suffix' 			=> 	'</label></div>',
				'#title'			=>	'Select Service',
				'#type' 			=> 	'select',
				'#options' 			=> $application_type,
				'#required'			=>	TRUE
			);
													
			$form['check_service']['submit']  			= 	array(
				'#prefix' 			=> 	'<div id="pensionstatusapp3">',
				'#suffix' 			=> 	'</div>',
				'#attributes' 		=> array('class' => array('btn btn-primary pull-right')),
				'#type' 			=> 	'submit',
				'#value' 			=> 	'Submit',
				'#attributes'		=> 	array('id' => 'submit_oneId' , 'class' => array('button'))
				);
			}
	}
	
				 
	if($contractor_type == 1){
		$application_type	=	application_type($value_reference_no,$contractor_id);
	
		$form['check_service']['application_type'] 	= 	array(
				'#prefix' 			=> 	'<div class="col-md-12" id="pensionstatusapp2"><label class="select">',
				'#suffix' 			=> 	'</label></div>',
				'#title'			=>	'Select Service',
				 '#type' 			=> 	'select',
				 '#options' 		=> $application_type,
				 '#required'		=>	TRUE
		);
										
		$form['check_service']['submit']  			= 	array(
				'#prefix' 			=> 	'<div id="pensionstatusapp3">',
				'#suffix' 			=> 	'</div>',
				'#attributes' 		=> array('class' => array('btn btn-primary pull-right')),
				'#type' 			=> 	'submit',
				'#value' 			=> 	'Submit',
				'#attributes'		=> 	array('id' => 'submit_oneId' , 'class' => array('btn btn-primary pull-right'))
			);
		 }
	}
	$form['check_service']['footer'] = 	array(
				'#markup' 			=> '</div></div></div>', 
				'#type' 			=> 	'markup',
	);
	return $form;
}

function check_availability_submit($form, &$form_state){

	$commands 	= array();
	$commands[] = ajax_command_replace('#pensionstatusapp1', drupal_render($form['check_service']['license_no']));
	$commands[] = ajax_command_replace('#message_id', drupal_render($form['check_service']['application_message']));
	$commands[] = ajax_command_replace('#pensionstatusapp2', drupal_render($form['check_service']['application_type']));
	$commands[] = ajax_command_replace('#pensionstatusapp3', drupal_render($form['check_service']['submit']));
	$commands[] = ajax_command_replace('#pensionstbtid', drupal_render($form['check_service']['pensionstbt']));
  	return array('#type' => 'ajax', '#commands' => $commands);
}

function check_reference_no_submit($form, &$form_state){
	global $base_root, $base_path, $user;
	
	$contractor_id 				= 	$user->uid;
	$val						=	$form_state['values']; 
	$serial_number				= 	$val['reference_no']; 
	$license_no					= 	$val['license_no'];
	$application_type			= 	trim($val['application_type']);
	
	
	/** Payal **/
	$fetch_remark		 = 	db_select('l_contractor_activity_tag', 'tag');
  	$fetch_remark		->	fields('tag',array('created_by','application_status'));
	$fetch_remark		->	condition('tag.from_v_serial_no',(int)$serial_number,'=');
	$fetch_remark		->	range(0,1);
	$fetch_remark		->	orderBy('tag.id', 'DESC');
	$recordCreatedBy	= 	$fetch_remark->execute()->fetchAssoc();
	 
	if($recordCreatedBy['created_by']== '' || $contractor_id == $recordCreatedBy['created_by']){
		$record 					= 	db_query( "select * from fun_contrcator_activity('".$serial_number."','".$contractor_id."')")->fetchAssoc();
		$previous_application_flag  =  $record['flagapplication'];
		$previous_application_id    =  $record['renewallicenseammendmentid'];
		
		$query 						= 	db_select('l_contractor_license_application', 'license');
		$query						->	fields('license',array('serial_no_from_v','contractor_license_number','id','contractor_particular_id','next_renweal_date','backlog_license_no','caf_id_no'))->condition('serial_no_from_v',(int)$serial_number,'=')->condition('contractor_user_id',$contractor_id,'=');
		$result 					= 	$query->execute()->fetchAssoc();
		$serial_no_from_v			=	$result['serial_no_from_v'];
		$contractor_license_number	=	$result['contractor_license_number'];
		$backlog_license_no			=	$result['backlog_license_no'];
		$contractor_particular_id	=	$result['contractor_particular_id'];
		$next_renweal_date			=	$result['next_renweal_date'];
		$license_id 				= 	$result['id'];
		$cafId 						= 	$result['caf_id_no'];

		

		// $contractor_regmaster 	= contractor_regmaster_details($serial_number);  
		//$labour_to_date		 	= $contractor_regmaster['est_date_of_work_of_each_labour_to_date'];
		 //$status		 			= $contractor_regmaster['status'];
		//$is_from_v				= $contractor_regmaster['is_from_v'];


		$query_info 				= 	db_select('l_contractor_info_master', 'info');
		$query_info					->	fields('info',array('is_from_v','status','est_date_of_work_of_each_labour_to_date'))->condition('id',(int)$serial_number,'=');
		$result_info				= $query_info->execute()->fetchAssoc();
		$is_from_v					= $result_info['is_from_v'];
		$status		 				= $result_info['status'];
		$labour_to_date		 		= $result_info['est_date_of_work_of_each_labour_to_date'];
	
		if($application_type == 'BR'){
			$renewal_data	=	fetch_all_data_contractor_license_apply_renweal($license_id,$contractor_id);
			if(!empty($renewal_data)){
		 		$renewal_appliaction_status	=	$renewal_data['renewal_appliaction_status'];
			}
			$query_frmv  				= 	db_select('l_contractor_info_master', 'c_info_master');
			$query_frmv					->	fields('c_info_master',array('is_from_v','contractor_type'))->condition('id',(int)$serial_number,'=');
			$query_frmv					->	condition('is_from_v','1','=');
			$query_data					=   $query_frmv->execute()->fetchAssoc();	
			$is_from_v 					= 	$query_data['is_from_v'];
			$contractor_type			=	$query_data['contractor_type'];
			if($is_from_v != 1){
				form_set_error('reference_no' ,t('Please enter vaild Reference Number or Form-V Number'));
			}
			if($contractor_type == '1'){
		 		$particula_data = fetch_all_data_particular_of_contract_labour($serial_number,$contractor_id);
		 		if(($is_from_v == 1) && empty($particula_data)){
				 drupal_goto('renewal/backlog-renewal/default/'.encryption_decryption_fun('encrypt', trim($serial_number)));
		 	    }
		       if(!empty($particula_data)){
				 $is_backlog_renewal = $particula_data['is_backlog_renewal'];
		  		 if($is_backlog_renewal == 1){
		  			if($renewal_appliaction_status == 'I'){
			   			drupal_goto('renewal/preview-renewal/'.encryption_decryption_fun('encrypt', trim($serial_number)).'/'.encryption_decryption_fun('encrypt', trim($renewal_appliaction_status)));
		  			}
					if($renewal_appliaction_status == 'U' ||$renewal_appliaction_status == 'A' || $renewal_appliaction_status == 'F' ||  $renewal_appliaction_status == 'P' || $renewal_appliaction_status == 'C' || $renewal_appliaction_status == 'FW'||$renewal_appliaction_status == 'BI' || $renewal_appliaction_status == 'R'){
			         drupal_goto('renewal/preview-renewal/'.encryption_decryption_fun('encrypt', trim($serial_number)).'/'.encryption_decryption_fun('encrypt', trim($renewal_appliaction_status)));
		  			}
					if($renewal_appliaction_status == 'B' || empty($renewal_appliaction_status)){
			         drupal_goto('renewal/backlog-renewal/default/'.encryption_decryption_fun('encrypt', trim($serial_number)));
		  			}
	      		}
          	   } 
			   if(empty($is_backlog_renewal) || $is_backlog_renewal == 0){
			   	drupal_goto('license-lists');
		 	   }
			}
			if($contractor_type == '2'){
		 		form_set_error('reference_no' ,t('You are applicable to apply for License '));
			}
		}
		
		if($application_type == 'A'){
			$record_details = db_query( "select  * from fun_basic_information1('".$contractor_id."','".$previous_application_id."','".$previous_application_flag."')")->fetchAssoc();
			$to_day = date('Y-m-d');
			if($labour_to_date>$record_details['next_renweal_date'])
		    	$min_date = $record_details['next_renweal_date'];
			else
		   		$min_date = $labour_to_date;
		    //if($labour_to_date>$record_details['next_renweal_date'] && $status == 1 ){
			//echo $to_day.'---'.	$min_date.'--'.$status;die;
			if(($to_day<=$min_date) && $status== 1 ){ 
				$query_all 	= db_select('l_amendment_of_contract_labour_license', 'amendment');
 				$query_all->fields('amendment',array('id','ammendment_status','license_renewal_ammendment_id'))->condition('license_id',$license_id,'=');
				$result_all = $query_all->execute();
				
				if($result_all->rowCount() > 1){
					$applicant_mailto  = 'wblc.nic@gmail.com';
					$subject = 'Amendment Issue';
					$msg = 'Multiple row for amendment for License Id -'.$license_id.' Form-V-No -'.$serial_number;
					if($applicant_mailto){
						send_mail_for_user_alert($applicant_mailto, $subject, $msg);
					}
					form_set_error('reference_no' ,t('Contractor have been suspended/deactivated or your work period has been end or your license has been expired '));
				}else{
					$query 	= db_select('l_amendment_of_contract_labour_license', 'amendment');
					$query->fields('amendment',array('id','ammendment_status','license_renewal_ammendment_id'))->condition('license_id',$license_id,'=');
				   $result = $query->execute()->fetchAssoc();
				   
				   $ammendment_status		=	$result['ammendment_status'];
				   $renewallicenseammendmentid	=	$result['license_renewal_ammendment_id'];
				   
				   if(empty($ammendment_status)|| $ammendment_status == 'I')
						drupal_goto('amendment_license_renewal/check_for_amendment/'.encryption_decryption_fun('encrypt', trim($serial_number)));
					   if($ammendment_status == 'B'){
						   $activity_tag 				= contractor_activity_tag($serial_number);
						   $renewallicenseammendmentid = $activity_tag['renewallicenseammendmentid'];
							$application_flag 			= $activity_tag['flagapplication'];
						   drupal_goto('amendment_license_renewal/apply_amendment/'.encryption_decryption_fun('encrypt', $serial_number).'/'. encryption_decryption_fun('encrypt', $renewallicenseammendmentid).'/'.encryption_decryption_fun('encrypt', $application_flag));
					   }
					   if(!empty($ammendment_status)||$ammendment_status != 'B' || $ammendment_status != 'I')
							drupal_goto('amendment_license_renewal/amendment-perview/'.encryption_decryption_fun('encrypt', trim($serial_number)).'/'.encryption_decryption_fun('encrypt', trim($renewallicenseammendmentid)), array('html' => TRUE));
				}

				
			}else{
				form_set_error('reference_no' ,t('You have been suspended/deactivated by your Principal Employer or your work period has been end or your license has been expired .For any inconvenience, contact your Principal Employer'));
			}
		}		
		
		if($application_type == 'R'){
			$record_details = db_query( "select  * from fun_basic_information1('".$contractor_id."','".$previous_application_id."','".$previous_application_flag."')")->fetchAssoc();
			$todaysDate = date('Y-m-d');
			//if(($labour_to_date > $record_details['next_renweal_date'])/*&&($labour_to_date >= date('Y-m-d'))*/ && $status == 1 ){
				 // echo $labour_to_date.'--'.$todaysDate ; 						
				// echo "<br>";
				// echo $labour_to_date.'--'.$record_details['next_renweal_date'].'--status'.$status.'<br>';die;
				//&& $is_from_v == 1 &&  
			if(( strtotime($labour_to_date) >= strtotime($todaysDate)) && $status == 1 && (strtotime($labour_to_date) >= strtotime($record_details['next_renweal_date']))){
				
				
				if($is_from_v == 1){							
					$contractor_license_apply_renweal = fetch_all_data_contractor_license_apply_renweal($license_id,$contractor_id);
					$appliaction_status				  =	$contractor_license_apply_renweal['renewal_appliaction_status'];
					drupal_goto('applicant/license-renewal/'.encryption_decryption_fun('encrypt', trim($serial_number)).'/'.encryption_decryption_fun('encrypt', trim($license_id)).'/'.encryption_decryption_fun('encrypt', trim($contractor_id)));
				}else{
					form_set_error('reference_no' ,t('Your Principal Employer is in the process of Amendment. You can apply only after your Principal Employer completed its amendment process.'));
				}						
			}else{
				form_set_error('reference_no' ,t('You have been suspended/deactivated by your Principal Employer or your work period has been end .For any inconvenience, contact your Principal Employer'));					
			}
		}	
			
	}else{
		drupal_set_message('You have applied services of FORM-V '.str_pad($serial_number, 6, "0", STR_PAD_LEFT).' with different login details. Please apply the service with the correct login details.','error');
	}
}




