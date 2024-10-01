<?php
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');

function view_activities($form,&$form_state,$view_person='',$areacode='', $status='', $which_application=''){ 

 	$area_code			=	encryption_decryption_fun('decrypt', $areacode)	;
	$form['officer_view_activities']    = array(
				'#type' 		=> 'fieldset',
				'#attributes'	=> array('class'=>array(''))
			);
										
								
	if($which_application	==	'C'	&& $view_person	==	'ALC'){
		$clra_data	=	clra_application_view_officer($area_code,$status);
		
		
			$form['officer_view_activities']['clra'] = array(		
						'#prefix' 				=> '<div class="content"><div class="sky-form"  style ="width:100%;">',
						'#type'					=>	'markup',
						'#markup' 			    => $clra_data	,
						'#suffix' 				=> '</div></div>',																		
																							
						);
		}
	if($which_application	==	'B'	&& $view_person	==	'ALC'){
		
		
		
			$form['officer_view_activities']['bocwa'] = array(		
						'#prefix' 				=> '<div class="content"><div class="sky-form"  style ="width:100%;">',
						'#type'					=>	'markup',
						'#markup' 			    => bocwa_application_view_officer($area_code,$status),
						'#suffix' 				=> '</div></div>',																		
																							
						);
		}
	if($which_application	==	'L'	&& $view_person	==	'ALC'){
		
		
		
			$form['officer_view_activities']['license'] = array(		
						'#prefix' 				=> '<div class="content"><div class="sky-form"  style ="width:100%;">',
						'#type'					=>	'markup',
						'#markup' 			    => contractor_license_view_officer($area_code,$status),
						'#suffix' 				=> '</div></div>',																		
																							
						);
		}
	return $form;
	
}

function clra_application_view_officer($area_code,$status){ 
	
	global $base_root, $base_path, $user;
	
 	$user_id	=	$user->uid;
	$output 	= '';
	$action		= 'encrypt';
	
	
  	// Table header
   $header = array(
		  array('data' => 'Sl. No', 'field' => 'slno','width' => '6%'),
		  array('data' => 'REGISTRATION NUMBER <br>REGISTRATION DATE', 'field' => 'reg_no','width' => '22%'),
		  array('data' => 'ESTABLISHMENT NAME', 'field' => 'aname','width' => '17%'),
		  array('data' => 'APPLY DATE', 'field' => 'applicationDate'),
		  array('data' => 'STATUS', 'field' => 'status'),
		  array('data' => 'ACTIONS', 'field' => 'actions'),
	  );
	  
 
	  
  /** Fetch All CLRA Application Details **/
  
  $get_clra_application_details 			= db_select('l_clra_registration_master', 'lcrm');
  $get_clra_application_details				->fields('lcrm',array());
  $get_clra_application_details				->condition('lcrm.name_areatype',$area_code);
  if($status	==	'P'){
 	 	$status_or				=	db_or();
		$status_or				->	condition('lcrm.status','0');
		$status_or				->	condition('lcrm.status','S');
		$get_clra_application_details			->	condition($status_or);
 } if($status	==	'In'){
 	 	$status_or				=	db_or();
		$status_or				->	condition('lcrm.status','B');
		$status_or				->	condition('lcrm.status','F');
		$status_or				->	condition('lcrm.status','BI');
		$status_or				->	condition('lcrm.status','V');
		$status_or				->	condition('lcrm.status','VA');
		$status_or				->	condition('lcrm.status','C');
		$status_or				->	condition('lcrm.status','T');
		$get_clra_application_details			->	condition($status_or);
 }if($status	==	'I'){
 	 	
		$get_clra_application_details			->	condition('lcrm.status','I');
 }if($status	==	'R'){
 	 	
		$get_clra_application_details			->	condition('lcrm.status','R');
 }
  $get_clra_application_details				->orderBy('lcrm.id','DESC');
  $get_clra_application_details_result 		= $get_clra_application_details->execute();
  
//print_r($get_clra_application_details_result->fetchAll());die();
  if($get_clra_application_details_result->rowCount() > 0 ){ 
  
  	$counter			= 0;  
	$view_img			= '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/view_details.png">';
		
		while($data = $get_clra_application_details_result->fetchObject()) { 
			$counter++;
			
			if(!empty($data->application_date)){
				$apply_date 		= date('dS M, Y', strtotime($data->application_date));
				
				/*** Escalation ****/
				
				$application_date 		= strtotime($data->application_date); 
				$last_week_date 		= strtotime('-7 day');
				
			}
			if(!empty($data->registration_date)){
				$issue_date 		= date('dS M, Y', strtotime($data->registration_date));
			}else {
				$issue_date			= '';
			}
			
			$registration_number = 'UNDER PROCESS';
			
			if( trim($data->status) == "0" ){
				$STATUS_IMG 		= '<span class="pending" title="Pending Application">Pending</span>';
				$registration_number = 'NEW APPLICATION';
          	}
          	
           	if( trim($data->status) == "B"){
				$STATUS_IMG 		= '<span class="pending" title="Back to Applicant by Inspector">Backed</span>';				
          	}
			
			if( trim($data->status) == "F"){
				$STATUS_IMG 		= '<span class="forward" title="Forwarded to ALC">Forwarded</span>';				
          	}
			
			if( trim($data->status) == "VA"){
				$STATUS_IMG 		= '<span class="approved">Approved</span>';				
          	}
          	if( trim($data->status) == "V" ){
				$STATUS_IMG 		= '<span class="feespending" title="Fees Pending">Fees Pending</span>';				
          	}
          	if( trim($data->status) == "BI" ){
				$STATUS_IMG 		= '<span class="backtoins" title="Back to Inspector">Back to Inspector</span>';				
          	}
          	if( trim($data->status) == "C"){
				$STATUS_IMG 		= '<span class="call_applicant" title="Call Applicant">Call</span>';				
          	}
          	if( trim($data->status) == "I" ){
				$STATUS_IMG 		= '<span class="issued" title="Certificate Issued">Issued</span>';				
          	}
			if( trim($data->status) == "T" ){
				$STATUS_IMG 		= '<span class="feespaid" title="Transaction Successful">Fees Paid</span>';				
          	}
          	if( trim($data->status) == "R" ){
				$STATUS_IMG 		= '<span class="reject" title="Reject Application">Reject</span>';
				$registration_number = 'REJECT';
          	}
			if( trim($data->status) == "S" ){
				$STATUS_IMG 		= '<span class="finalsubmit" title="Final Submit">Final Submit</span>';
				
          	}
			
			if($data->registration_number != ''){
				 $registration_number = $data->registration_number;
			}
			
			if( ( $application_date < $last_week_date ) && trim($data->status) == "0"){ 				 
				
				$rows[] = array(
						array('data' => $user_id.'|'.$counter, 'align' => 'left', 'class' => array('')),
						array('data' => $registration_number.'<br/>'.$issue_date, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => $data->e_name, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => $apply_date, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => $STATUS_IMG, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => l($view_img, 'official/view-list-of-pe/'.encryption_decryption_fun($action, $data->id).'/'.encryption_decryption_fun($action, $data->user_id).'/'.$data->status, array('html' => TRUE)), 'align' => 'left', 'class' => array('exp_pendng')),
						
				);
				
				
				
			}else{
				$rows[] = array(
						array('data' => $role_id.$counter, 'align' => 'left', 'class' => array('odd')),
						array('data' => '<strong>'.$registration_number.'</strong><br/>'.$issue_date, 'align' => 'left', 'class' => array('even')),
						array('data' => $data->e_name, 'align' => 'left', 'class' => array('odd')),
						array('data' => $apply_date, 'align' => 'left', 'class' => array('even')),
						array('data' => $STATUS_IMG, 'align' => 'left', 'class' => array('odd')),
						array('data' => l($view_img, 'official/view-list-of-pe/'.encryption_decryption_fun($action, $data->id).'/'.encryption_decryption_fun($action, $data->user_id).'/'.$data->status, array('html' => TRUE)), 'align' => 'left', 'class' => array('exp_pendng')),
						
				);
			
		}
  		
 		}
  
  }
  
  $variables = array(
	  		'attributes' 		=> array('class' => array('clra_applications')), 
	  		'header' 			=> $header,
	  		'rows'				=> $rows,
			'empty' 			=> t("No data found!")
	  );
	
	
  $output = theme('datatable', $variables);
	
  return $output;		
  
}

function bocwa_application_view_officer($area_code,$status){ 
	
	global $base_root, $base_path, $user;
	
 	$user_id	=	$user->uid;
	$output 	= '';
	$action		= 'encrypt';
	
  	// Table header
   $header = array(
		  array('data' => 'Sl. No', 'field' => 'slno','width' => '6%'),
		
		  array('data' => 'REGISTRATION NUMBER <br>REGISTRATION DATE', 'field' => 'reg_no','width' => '22%'),
		  array('data' => 'ESTABLISHMENT NAME', 'field' => 'aname','width' => '17%'),
		  array('data' => 'APPLY DATE', 'field' => 'applicationDate'),
		  array('data' => 'STATUS', 'field' => 'status'),
		  array('data' => 'ACTIONS', 'field' => 'actions'),
	  );
	  
 

	/**** Fetch Inspector Block Code ****/
	
	
	    
  /** Fetch All BOCWA Application Details **/
  		
  $get_bocwa_application_details 				= db_select('l_bocwa_registration_master', 'bocwa_master');
  $get_bocwa_application_details				->fields('bocwa_master',array('id','e_name','application_date','registration_date','identification_number','registration_number','final_submit_status','status','user_id'));
  $get_bocwa_application_details				->condition('bocwa_master.loc_e_areatype_code',$area_code);
   if($status	==	'P'){
 	 	$status_or				=	db_or();
		$status_or				->	condition('bocwa_master.status','0');
		$status_or				->	condition('bocwa_master.status','S');
		$get_bocwa_application_details			->	condition($status_or);
 } if($status	==	'In'){
 	 	$status_or				=	db_or();
		$status_or				->	condition('bocwa_master.status','B');
		$status_or				->	condition('bocwa_master.status','F');
		$status_or				->	condition('bocwa_master.status','BI');
		$status_or				->	condition('bocwa_master.status','V');
		$status_or				->	condition('bocwa_master.status','VA');
		$status_or				->	condition('bocwa_master.status','C');
		$status_or				->	condition('bocwa_master.status','T');
		$get_bocwa_application_details			->	condition($status_or);
 }if($status	==	'I'){
 	 	
		$get_bocwa_application_details			->	condition('bocwa_master.status','I');
 }if($status	==	'R'){
 	 	
		$get_bocwa_application_details			->	condition('bocwa_master.status','R');
 }
 
  $get_bocwa_application_details				->orderBy('bocwa_master.id','DESC');
  $get_bocwa_application_details_result 		= $get_bocwa_application_details->execute();
 
//print_r($get_bocwa_application_details_result);die();
  
  if( $get_bocwa_application_details_result->rowCount() > 0 ){ 
  
  	$counter			= 0;  
	$view_img			= '<img src="'.$base_root.$base_path.'sites/default/files/view_i.png" height="26" width="80">';
		
		while($data = $get_bocwa_application_details_result->fetchObject()) {  
			$counter++;
			
			if(!empty($data->application_date)){
				$apply_date 		= date('dS M, Y', strtotime($data->application_date));
				
				/*** Escalation ****/
				
				$application_date 		= strtotime($data->application_date); 
				$last_week_date 		= strtotime('-7 day');
				
			}
			if(!empty($data->registration_date)){
				$issue_date 		= date('dS M, Y', strtotime($data->registration_date));
			}else {
				$issue_date			= '';
			}
			
			
			
			if( trim($data->status) == "0" ){
				$STATUS_IMG 		= '<span class="pending" title="Pending Application">Pending</span>';
				$registration_number = 'NEW APPLICATION';
          	}
          	
           	if( trim($data->status) == "B"){
				$STATUS_IMG 		= '<span class="pending" title="Back to Applicant by Inspector">Backed</span>';				
          	}
			
			if( trim($data->status) == "F"){
				$STATUS_IMG 		= '<span class="forward" title="Forwarded to ALC">Forwarded</span>';				
          	}
			
			if( trim($data->status) == "VA"){
				$STATUS_IMG 		= '<span class="approved">Approved</span>';				
          	}
          	if( trim($data->status) == "V" ){
				$STATUS_IMG 		= '<span class="feespending" title="Fees Pending">Fees Pending</span>';				
          	}
          	if( trim($data->status) == "BI" ){
				$STATUS_IMG 		= '<span class="backtoins" title="Back to Inspector">Back to Inspector</span>';				
          	}
          	if( trim($data->status) == "C"){
				$STATUS_IMG 		= '<span class="call_applicant" title="Call Applicant">Call</span>';				
          	}
          	if( trim($data->status) == "I" ){
				$STATUS_IMG 		= '<span class="issued" title="Certificate Issued">Issued</span>';				
          	}
          	if( trim($data->status) == "R" ){
				$STATUS_IMG 		= '<span class="reject" title="Reject Application">Reject</span>';
				$registration_number = 'REJECT';
          	}
			if( trim($data->status) == "T" ){
				$STATUS_IMG 		= '<span class="feespaid" title="Transaction Successful">Fees Paid</span>';				
          	}
			if( trim($data->status) == "R" ){
				$STATUS_IMG 		= '<span class="reject" title="Reject Application">Reject</span>';
				$registration_number = 'REJECT';
          	}
			if( trim($data->status) == "S" ){
				$STATUS_IMG 		= '<span class="finalsubmit" title="Final Submit">Final Submit</span>';
				
          	}
			
			if(!empty($data->registration_number)){
				 $registration_number = $data->registration_number;
			}else{
				$registration_number = 'UNDER PROCESS';	
			}
			
			if( ( $application_date < $last_week_date ) && trim($data->status) == "0"){ 				 
				
				$rows[] = array(
						array('data' => $counter, 'align' => 'left', 'class' => array('')),
					
						array('data' => $registration_number.'<br/>'.$issue_date, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => $data->e_name, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => $apply_date, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => $STATUS_IMG, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => l($view_img, 'official/view-list-of-bocwa/'.encryption_decryption_fun($action, $data->id).'/'.encryption_decryption_fun($action, $data->user_id).'/'.encryption_decryption_fun($action,$data->status), array('html' => TRUE)), 'align' => 'left', 'class' => array('exp_pendng')),
						
				);
				
				
				
			}else{
				$rows[] = array(
						array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
						
						array('data' => '<strong>'.$registration_number.'</strong><br/>'.$issue_date, 'align' => 'left', 'class' => array('odd')),
						array('data' => $data->e_name, 'align' => 'left', 'class' => array('even')),
						array('data' => $apply_date, 'align' => 'left', 'class' => array('odd')),
						array('data' => $STATUS_IMG, 'align' => 'left', 'class' => array('even')),
						array('data' => l($view_img, 'official/view-list-of-bocwa/'.encryption_decryption_fun($action, $data->id).'/'.encryption_decryption_fun($action, $data->user_id).'/'.encryption_decryption_fun($action,$data->status), array('html' => TRUE)), 'align' => 'left', 'class' => array('odd')),
						
				);
			
			}
  		
 		}
  
  }
  
  $variables = array(
	  		'attributes' 		=> array('class' => array('clra_applications')), 
	  		'header' 			=> $header,
	  		'rows'				=> $rows,
			'empty' 			=> t("No data found!")
	  );
	
	
  $output = theme('datatable', $variables);
	
  return $output;		
  
}

function contractor_license_view_officer($area_code,$status){
		
	global $base_root, $base_path, $user;
	
 	$userid	=	$user->uid;
	$output 	= '';
	
	$header 	= array(
					  array('data' => 'Sl. No', 'field' => 'slno','width' =>'6%'),
					  array('data' => 'License<br>Number &amp; Date', 'field' => 'l_num','width' => '16%'),
					  array('data' => 'Name of Contractor', 'field' => 'c_name','width' => '20%'),					  				 
					  array('data' => 'Apply Date', 'field' => 'applLic_date','width' => '15%'),
					  array('data' => 'PE Reg. No.<br>Form V Serial No.', 'field' => 'form_v','width' => '20%'),					  
					  array('data' => 'Status', 'field' => 'status'),
					  array('data' => 'ACTIONS', 'field' => 'actions'),
	  		 	  );
	
	
	
	/**** Fetch License Lists with Respect to Block code ****/
	
	
	$get_license_data					= db_select('l_contractor_info_master', 'infomaster');
  	$get_license_data					->InnerJoin('l_particular_of_contract_labour', 'particular','particular.serial_no_from_v= infomaster.id'); 
 	$get_license_data					->InnerJoin('l_contractor_license_application', 'license','particular.id= license.contractor_particular_id'); 
  	$get_license_data					->InnerJoin('l_clra_registration_master', 'clra_master','clra_master.id= infomaster.application_id');
	

	$get_license_data					->	fields('infomaster',array('name_of_contractor','application_id'));
	$get_license_data					->	fields('clra_master',array('name_areatype'));
	$get_license_data					->	fields('license',array('user_id','serial_no_from_v','created_date','license_date','principal_reg_no','contractor_license_number','final_status','status'));
	$get_license_data					->	fields('particular',array('id'));
	$get_license_data					->	condition('clra_master.name_areatype',$area_code);
	if($status	==	'P'){
 	 	$status_or				=	db_or();
		$status_or				->	condition('license.status','F');
		$status_or				->	condition('license.status','U');
		$get_license_data		->	condition($status_or);
 } if($status	==	'In'){
 	 	$status_or				=	db_or();
		$status_or				->	condition('license.status','B');
		$status_or				->	condition('license.status','FW');
		$status_or				->	condition('license.status','BI');
		$status_or				->	condition('license.status','A');
		$status_or				->	condition('license.status','C');
		$status_or				->	condition('license.status','T');
		$get_license_data		->	condition($status_or);
 }if($status	==	'I'){
 	 	
		$get_license_data			->	condition('license.status','I');
 }if($status	==	'R'){
 	 	
		$get_license_data			->	license('lcrm.status','R');
 }

	$get_license_data_result 			= 	$get_license_data->execute();
	
	
	
	if( $get_license_data_result->rowCount() > 0 ){ 
		
		$counter			= 0;             
		$view_img			= '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/view_details.png" height="26" width="80">';
		
		while($data = $get_license_data_result->fetchObject()) { 
			$counter++;
			
			if(!empty($data->created_date)){
				$license_apply_date 	= date('dS M, Y', strtotime($data->created_date));
				
				/*** Escalation ****/
				
				$application_date 		= strtotime($data->created_date); 
				$last_week_date 		= strtotime('-7 day');
			}
			
			if(!empty($data->license_date)){
				$license_issue_date 	= date('dS M, Y', strtotime($data->license_date));
			}else {
				$license_issue_date		= '';
			}
			
			if( trim($data->status) == "F" && trim($data->final_status) == "F" ){
				$STATUS_IMG 		= '<span class="pending">Pending</span>';
          	}
          	if( trim($data->status) == "A" && trim($data->final_status) == "F" ){
				$STATUS_IMG 		= '<span class="feespending" title="Fees Pending">Fees Pending</span>';
          	}
           	if( trim($data->status) == "B" && trim($data->final_status) == "F" ){
				$STATUS_IMG 		= '<span class="pending" title="Back to Applicant">Backed</span>';
          	}
          	if( trim($data->status) == "P" && trim($data->final_status) == "F" ){
				$STATUS_IMG 		= '<span class="feespaid">Fees Paid</span>';
          	}
          	if( trim($data->status) == "BI" && trim($data->final_status) == "F" ){
				$STATUS_IMG 		= '<span class="pending" title="Back to Inspector by ALC">Backed to Inspector</span>';
          	}
          	if( trim($data->status) == "C" && trim($data->final_status) == "F" ){
				$STATUS_IMG 		= '<span class="call_applicant">Call Applicant</span>';
          	}
          	if( trim($data->status) == "I" && trim($data->final_status) == "F" ){
				$STATUS_IMG 		= '<span class="issued">License Issued</span>';
          	}
          	if( trim($data->status) == "R" && trim($data->final_status) == "F" ){
				$STATUS_IMG 		= '<span class="reject">Reject</span>';
          	}
			if( trim($data->status) == "FW" && trim($data->final_status) == "F" ){
				$STATUS_IMG 		= '<span class="forward" title="Forwarded to ALC">Forwarded</span>';
          	}
			if( trim($data->status) == "U" && trim($data->final_status) == "F" ){
				$STATUS_IMG 		= '<span class="finalsubmit" title="Final submit">Final Submit</span>';
          	}
			
			
			$particular_table_pk_id = encryption_decryption_fun('encrypt',$data->id);
			$serial_no_from_v 		= encryption_decryption_fun('encrypt', $data->serial_no_from_v); 
			
			if( ( $application_date < $last_week_date ) && trim($data->status) == "F" ){

		
				$rows[] = array(
							array('data' => $counter, 'align' => 'left', 'class' => array('')),
							array('data' =>	!empty($data->contractor_license_number)? $data->contractor_license_number :'In Process', 'align' => 'left', 'class' => array('pendng')),
							array('data' => $data->name_of_contractor, 'align' => 'left', 'class' => array('pendng')),
							array('data' => $license_apply_date, 'align' => 'left', 'class' => array('pendng')),
							array('data' => '<strong>'.$data->principal_reg_no.'<br/><font color="#fff">00'.$data->serial_no_from_v.'</font><strong>', 'align' => 'left', 'class' => array('pendng')),							
							array('data' =>	$STATUS_IMG, 'align' => 'left', 'class' => array('pendng')),
							array('data' =>	l($view_img,'official/view-license/'.$serial_no_from_v.'/'.$particular_table_pk_id, array('html' => TRUE)),'align' => 'left', 'class' => array(''))
							
					  );
			}else {
			
				$rows[] = array(
						array('data' =>	$counter, 'align' => 'left', 'class' => array('')),
						array('data' =>	!empty($data->contractor_license_number) ? $data->contractor_license_number :'In Process', 'align' => 'left', 'class' => array('')),
						array('data' =>	$data->name_of_contractor, 'align' => 'left', 'class' => array('')),
						array('data' =>	$license_apply_date, 'align' => 'left', 'class' => array('')),
						array('data' => '<strong>'.$data->principal_reg_no.'<br/><font color="#FF6600">00'.$data->serial_no_from_v.'</font><strong>', 'align' => 'left', 'class' => array('')),											
						array('data' =>	$STATUS_IMG, 'align' => 'left', 'class' => array('')),
						array('data' =>	l($view_img,'official/view-license/'.$serial_no_from_v.'/'.$particular_table_pk_id, array('html' => TRUE)), 'align' => 'left', 'class' => array(''))
				  );
			}
		}
	}
	
	$variables = array(
	  		'attributes' 		=> array('class' => array('clra_applications')), 
	  		'header' 			=> $header,
	  		'rows'				=> $rows,
			'empty' 			=> t("No data found!")
	  );
	
	
	$output = theme('datatable', $variables);
	
	return $output;
}