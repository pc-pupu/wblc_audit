<?php

function alc_clra_amended_applications($form, &$form_state){ 
	
	global $base_root, $base_path, $user;
	
	$_SESSION['uid'] 	= $user->uid;
	$user_id			= $user->uid;
	
	drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
	
	$form['clra_amended_application_lists_alc'] 					= array(
																			'#type' 				=> 'fieldset',
																			'#attributes'		 	=> array('class'=>array(''))
																			);
		
	$get_all_clra_amended_application_lists 						=  clra_amended_application_lists_alc();
		
		
	$form['clra_amended_application_lists_alc']['markup_data_1'] 	= array(
																			'#prefix' 				=> '',
																			'#markup' 				=> $get_all_clra_amended_application_lists,
																			'#suffix' 				=> '',
																);
		
	
		
		
	return $form;
}

function clra_amended_application_lists_alc(){
	
	global $base_root, $base_path, $user;
	
 	$user_id	=	$user->uid;	 
	
	/**** Fetch ALC SUBDIVISION Code ****/
	
	$query_subdivision			= db_select('l_customuser_relation_address', 'lcra');
	$query_subdivision			->fields('lcra',array('sub_div_code','district_code'));
	$query_subdivision			->condition('lcra.user_id',$user_id,'=');
	$result_query_subdivision 	= $query_subdivision->execute();
	
	
	if($result_query_subdivision-> rowCount() > 0 ){		
		$subdivision_code_result = $result_query_subdivision->fetchAssoc();		
		$subdivision_alc 		 = $subdivision_code_result['sub_div_code'];		
	}
	
	
     /** Fetch All CLRA Amended Application Details **/
  		
	  $get_clra_amendment_application 			= db_select('l_clra_registration_master', 'clra_master');
	  $get_clra_amendment_application			->fields('clra_master',array('id','e_name','application_date','registration_date','identification_number','registration_number','final_submit_status','status','user_id','amendment_parent_id'));
	  $get_clra_amendment_application			->condition('clra_master.loc_e_subdivision',$subdivision_alc);
	  $get_clra_amendment_application			->condition('clra_master.final_submit_status','P');
	  $get_clra_amendment_application			->isNotNull('clra_master.amendment_parent_id');
	  
	  
	  $get_clra_amendment_application			->orderBy('clra_master.id','DESC');
	  $get_clra_amendment_application_result	= $get_clra_amendment_application->execute();
	  
	  
	  $header = array(
			  array('data' => 'Sl. No', 'field' => 'slno','width' => '6%', 'data-orderable' => false),
			  array('data' => 'IDENTIFICATION NUMBER', 'field' => 'reg_no','width' => '16%'),
			  array('data' => 'REGISTRATION NUMBER <br>REGISTRATION DATE', 'field' => 'reg_no','width' => '22%'),
			  array('data' => 'ESTABLISHMENT NAME', 'field' => 'aname','width' => '17%'),
			  array('data' => 'APPLY DATE', 'field' => 'applicationDate'),
			  array('data' => 'STATUS', 'field' => 'status'),
			  array('data' => 'ACTIONS', 'field' => 'actions'),
	  );
	  
	  if( $get_clra_amendment_application_result->rowCount() > 0 ){ 
	  
		$counter			= 0;  
		$view_img			= '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/view_details.png">';
			
			while($data = $get_clra_amendment_application_result->fetchObject()) { 
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
					$STATUS_IMG 		= '<span class="pending" title="Pending Application"></span>';
					$registration_number = 'NEW APPLICATION';
				}
				
				elseif( trim($data->status) == "B"){
					$STATUS_IMG 		=  '<span class="backed" title="Back for Rectification"></span>';			
				}
				
				elseif( trim($data->status) == "F"){
					$STATUS_IMG 		= '<span class="forward" title="Forwarded"></span>';			
				}
				
				elseif( trim($data->status) == "VA"){
					$STATUS_IMG 		= '<span class="issued">Approved</span>';				
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
					$STATUS_IMG 		= '<span class="feespaid" title="Fees Paid"></span>';
					
				}elseif( trim($data->status) == "S"){ 
					$STATUS_IMG 				='<span class="finalsubmit" title="Final Submit"></span>';			
				}else{
					$STATUS_IMG='';
				}
				
				if($data->registration_number != ''){
					 $registration_number = $data->registration_number;
				}
			
				if( ( $application_date < $last_week_date ) && trim($data->status) == "0"){ 				 
					
					$rows[] = array(
							array('data' => $counter, 'align' => 'left', 'class' => array('')),
							array('data' => $data->identification_number, 'align' => 'left', 'class' => array('exp_pendng')),
							array('data' => '<strong>'.$registration_number.'</strong><br/>'.$issue_date, 'align' => 'left', 'class' => array('exp_pendng')),
							array('data' => $data->e_name, 'align' => 'left', 'class' => array('exp_pendng')),
							array('data' => $apply_date, 'align' => 'left', 'class' => array('exp_pendng')),
							array('data' => $STATUS_IMG, 'align' => 'left', 'class' => array('exp_pendng')),
							array('data' => l($view_img, 'alc-view-amended-application/'.encryption_decryption_fun('encrypt', trim($data->id)).'/'.encryption_decryption_fun('encrypt', trim($data->user_id)), array('html' => TRUE)), 'align' => 'left', 'class' => array('exp_pendng ')),
							
					);
					
					
					
				}else{
					$rows[] = array(
							array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
							array('data' => $data->identification_number, 'align' => 'left', 'class' => array('even')),
							array('data' => '<strong>'.$registration_number.'</strong><br/>'.$issue_date, 'align' => 'left', 'class' => array('even')),
							array('data' => $data->e_name, 'align' => 'left', 'class' => array('odd')),
							array('data' => $apply_date, 'align' => 'left', 'class' => array('even')),
							array('data' => $STATUS_IMG, 'align' => 'left', 'class' => array('odd')),
							array('data' => l($view_img, 'alc-view-amended-application/'.encryption_decryption_fun('encrypt', trim($data->id)).'/'.encryption_decryption_fun('encrypt', trim($data->user_id)),array('html' => TRUE)), 'align' => 'left', 'class' => array('even')),
							
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
