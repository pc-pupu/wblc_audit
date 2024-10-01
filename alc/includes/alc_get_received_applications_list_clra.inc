<?php 

function alc_received_applications_list_clra($form, &$form_state){	
	global $base_root, $base_path, $user;
	
	$user_id = $user->uid;
	
	
	
	$form['clra_application_lists_alc'] = array(
							'#type' 		=> 'fieldset',
							'#attributes'	=> array('class'=>array(''))
							);
		
	$get_all_clra_application_lists =  clra_application_lists_alc();
		
		
	$form['clra_application_lists_alc']['markup_data_1'] = array(							
							'#prefix' => '<div class="feedback-scroll">',
							'#suffix' => '</div>', 
							'#markup' => '<div class="feedback-scroll">'.$get_all_clra_application_lists.'</div>'							
							);		
	return $form;
}

function clra_application_lists_alc(){
	
	global $base_root, $base_path, $user;
	
 	$user_id = $user->uid;	 
	
	/**** Fetch ALC SUBDIVISION Code ****/
	
	$query_subdivision = db_select('l_customuser_relation_address', 'lcra');
	$query_subdivision->fields('lcra',array('sub_div_code','district_code'));
	$query_subdivision->condition('lcra.user_id',$user_id,'=');
	$result_query_subdivision = $query_subdivision->execute();	
	
	if($result_query_subdivision-> rowCount() > 0 ){		
		$subdivision_code_result = $result_query_subdivision->fetchAssoc();		
		$subdivision_alc = $subdivision_code_result['sub_div_code'];		
	}	
	
	 /** Fetch All CLRA Application Details **/
  		
	$get_clra_application_details = db_select('l_clra_registration_master', 'clra_master');
	$get_clra_application_details->fields('clra_master',array('id','e_name','application_date','registration_date','identification_number','registration_number','final_submit_status','status','user_id', 'amendment_parent_id', 'backlog_id'));
	$get_clra_application_details->condition('clra_master.loc_e_subdivision',$subdivision_alc);
	$get_clra_application_details->condition('clra_master.final_submit_status','P');
	$get_clra_application_details->condition('clra_master.status','I','!=');
	$get_clra_application_details->orderBy('clra_master.id','DESC');
	//$get_clra_application_details->isNull('clra_master.amendment_parent_id');/**15062017**/
	$get_clra_application_details_result = $get_clra_application_details->execute();
  
  $header = array(
		  array('data' => 'Sl. No.', 'field' => 'slno','width' => '8%', 'data-orderable' => false),
		  array('data' => 'Identification Number', 'field' => 'reg_no','width' => '17%'),
		  array('data' => 'Registration No. & Date', 'field' => 'reg_no','width' => '15%'),
		  array('data' => 'Establishment Name', 'field' => 'e_name'),
		  array('data' => 'Apply Date', 'field' => 'applicationDate', 'width' => '10%'),
		  array('data' => 'Status', 'field' => 'status', 'width' => '7%'),
		  array('data' => 'action', 'field' => 'actions', 'width' => '8%'),
  		);
  
  if( $get_clra_application_details_result->rowCount() > 0 ){  
  	$counter = 0;  
	$view_img = '<span class="badge bg-blue"><i class="fa fa-eye"></i> View Details<div>';
		
	while($data = $get_clra_application_details_result->fetchObject()) { 
		$counter++;
			
		if(!empty($data->application_date)){
			$apply_date = date('d/m/Y', strtotime($data->application_date));
			
			/*** Escalation ****/
				
			$application_date = strtotime($data->application_date); 
			$last_week_date = strtotime('-7 day');
				
		}
		if(!empty($data->registration_date)){
			$issue_date 		= date('d/m/Y', strtotime($data->registration_date));
		}else {
			$issue_date			= '';
		}
		
		$registration_number = 'UNDER PROCESS';
		
		if( trim($data->status) == "0" ){
			$STATUS_IMG 		= '<span class="pending" title="Pending Application"></span>';
			$registration_number = 'NEW APPLICATION';
		}
		
		elseif( trim($data->status) == "B"){
			$STATUS_IMG 		= '<span class="backed" title="Back for Rectification"></span>';				
		}
		
		elseif( trim($data->status) == "F"){
			$STATUS_IMG 		= '<span class="forward" title="Forwarded"></span>';				
		}
		
		elseif( trim($data->status) == "A"){
			$STATUS_IMG 		= '<span class="approved" title="Approved"></span>';				
		}
		elseif( trim($data->status) == "VA"){
			$STATUS_IMG 		= '<span class="approved" title="Approved"></span>';				
		}
		elseif( trim($data->status) == "V" ){
			$STATUS_IMG 		= '<span class="feespending" title="Fees Pending"></span>';				
		}
		elseif( trim($data->status) == "BI" ){
			$STATUS_IMG 		= '<span class="backtoins" title="Back to Inspector"></span>';				
		}
		elseif( trim($data->status) == "C"){
			$STATUS_IMG 		= '<span class="callapplicant" title="Call Applicant"></span>';				
		}
		elseif( trim($data->status) == "I" ){
			$STATUS_IMG 		= '<span class="issued" title="Certificate Issued"></span>';				
		}
		elseif( trim($data->status) == "R" ){
			$STATUS_IMG 		= '<span class="reject" title="Rejected"></span>';
			$registration_number = 'REJECT';
			
		}elseif( trim($data->status) == "T" ){
			$STATUS_IMG 				= '<span class="feespaid" title="Fees Paid"></span>';	
			
		}elseif( trim($data->status) == "S"){ 
			$STATUS_IMG 				='<span class="finalsubmit" title="Final Submit"></span>';
			
		}elseif( trim($data->status) == "U"){ 
		
			$STATUS_IMG ='<span class="backed" title="Back for Form-I Rectification"></span>';
								
		}else{
			$STATUS_IMG='';
		}
		
		if(!empty($data->registration_number)){
			 $registration_number = $data->registration_number;
		}
					
		$apply_category = '';
		
		if(!empty($data->amendment_parent_id)){
			 $apply_category = '<span style="font-size:14px; color:#E08E0B">[AMENDMENT APPLICATION]</span>';
		}			
		
		if( ( $application_date < $last_week_date ) && trim($data->status) == "0"){ 				 
				
			$view_details  = array('data' => l($view_img, 'alc-visible-applications/'.encryption_decryption_fun('encrypt', trim($data->id)).'/'.encryption_decryption_fun('encrypt', trim($data->user_id)), array('html' => TRUE)), 'align' => 'left', 'class' => array('exp_pendng '));
				
			if(!empty($data->amendment_parent_id)){
				$view_details = array('data' => l($view_img, 'alc-view-amended-application/'.encryption_decryption_fun('encrypt', trim($data->id)).'/'.encryption_decryption_fun('encrypt', trim($data->user_id)), array('html' => TRUE)), 'align' => 'left', 'class' => array('exp_pendng '));
			}else{
				  $backlog_rc_upload = db_select('l_documents_upload', 'ldu');
				  $backlog_rc_upload->fields('ldu',array('backlog_clra_registration_certificate_file'));					  
				  $backlog_rc_upload->condition('ldu.application_id', $data->id);
				  $backlog_rc_upload->condition('ldu.act_id', 1);  
				  $backlog_rc_upload = $backlog_rc_upload->execute();
				  $backlog_rc_upload_result = $backlog_rc_upload->fetchObject();					  
					  
				  if(!empty($backlog_rc_upload_result->backlog_clra_registration_certificate_file) && empty($data->backlog_id)){
					   $view_details = array('data' => l($view_img, 'alc-approve-backlog-applications/'.encryption_decryption_fun('encrypt', trim($data->id)).'/'.encryption_decryption_fun('encrypt', trim($data->user_id)), array('html' => TRUE)), 'align' => 'left', 'class' => array('exp_pendng '));
				 }
			}
				
				
				
		$rows[] = array(
				array('data' => $counter, 'align' => 'left', 'class' => array('')),
				array('data' => $data->identification_number.'<br/>'.$apply_category, 'align' => 'left', 'class' => array('exp_pendng')),
				array('data' => '<strong>'.$registration_number.'</strong><br/>'.$issue_date, 'align' => 'left', 'class' => array('exp_pendng')),
				array('data' => $data->e_name, 'align' => 'left', 'class' => array('exp_pendng')),
				array('data' => $apply_date, 'align' => 'left', 'class' => array('exp_pendng')),
				array('data' => $STATUS_IMG, 'align' => 'left', 'class' => array('exp_pendng')),
				$view_details						
				);
							
		}else{				
			$view_details  = array('data' => l($view_img, 'alc-visible-applications/'.encryption_decryption_fun('encrypt', trim($data->id)).'/'.encryption_decryption_fun('encrypt', trim($data->user_id)),array('html' => TRUE)), 'align' => 'left', 'class' => array('even'));
			
			if(!empty($data->amendment_parent_id)){
				$view_details = array('data' => l($view_img, 'alc-view-amended-application/'.encryption_decryption_fun('encrypt', trim($data->id)).'/'.encryption_decryption_fun('encrypt', trim($data->user_id)), array('html' => TRUE)), 'align' => 'left', 'class' => array(''));
			}else{
				  $backlog_rc_upload = db_select('l_documents_upload', 'ldu');
				  $backlog_rc_upload->fields('ldu',array('backlog_clra_registration_certificate_file'));					  
				  $backlog_rc_upload->condition('ldu.application_id', $data->id);
				  $backlog_rc_upload->condition('ldu.act_id', 1);  
				  $backlog_rc_upload = $backlog_rc_upload->execute();
				  $backlog_rc_upload_result = $backlog_rc_upload->fetchObject();				  
				  
				if(!empty($backlog_rc_upload_result->backlog_clra_registration_certificate_file) && empty($data->backlog_id)){
					 $view_details = array('data' => l($view_img, 'alc-approve-backlog-applications/'.encryption_decryption_fun('encrypt', trim($data->id)).'/'.encryption_decryption_fun('encrypt', trim($data->user_id)), array('html' => TRUE)), 'align' => 'left', 'class' => array('exp_pendng '));
				 }
			}
		
			$rows[] = array(
					array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
					array('data' => $data->identification_number.'<br/>'.$apply_category, 'align' => 'left', 'class' => array('even')),
					array('data' => '<strong>'.$registration_number.'</strong><br/>'.$issue_date, 'align' => 'left', 'class' => array('even')),
					array('data' => $data->e_name, 'align' => 'left', 'class' => array('odd')),
					array('data' => $apply_date, 'align' => 'left', 'class' => array('even')),
					array('data' => $STATUS_IMG, 'align' => 'left', 'class' => array('odd')),
					$view_details,					
					);
			
				}  		
 			}
  
  		}
  
  		$variables = array(
						'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')), 
						'header' 	 => $header,
						'rows'		 => $rows,
						'empty' 	 => t("No data found!"));
	
	
  $output = theme('datatable', $variables);
	
  return $output;		
  
}
