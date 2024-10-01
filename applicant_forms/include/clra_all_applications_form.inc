<?php

function clra_all_applications_form($form, &$form_state){
	
	
	global $base_root, $base_path, $user;
	
	$_SESSION['uid'] 	= $user->uid;
	$login_id			= $user->uid;
	
	
	drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
	drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
	
	
	$form['clra_applications'] 									= array(
																  '#type' 				=> 'fieldset',
																  '#prefix' 			=> '<div class="content"><div class="sky-form">',
																 );
	
	$form['clra_applications']['markup_data_1']					= array(
													  				'#markup' 			=> l('New Application', 'clra_backlog', array( 'html' => TRUE, 'attributes'=> array('class' => array('button left-btn')))),
												  					);
																	
	
	$fetch_clra_applications_with_respect_to_id 				= fetch_clra_applications_with_respect_to_id();
	
	$form['clra_applications']['markup_data_2']					= array(
																	  '#markup' 		=> $fetch_clra_applications_with_respect_to_id,
																	  '#suffix' 		=> '</div></div>',
																  );
	return $form;
}

function fetch_clra_applications_with_respect_to_id(){
	
	global $base_root, $base_path, $user;
	
 	$userid		=	$user->uid;
	$action		= 'encrypt';
	$output 	= '';
	$header 	= array(
					  array('data' => 'SL. NO', 'field' => 'slno','width' => '5%'),
					  array('data' => 'IDENTIFICATION NO.<br/>APPLY DATE', 'field' => 'identification_number','width' => '20%'),
					  array('data' => 'REGISTRATION NO. <br/> ISSUE DATE', 'field' => 'registration_number','width' => '20%'),
					  array('data' => 'ESTABLISHMENT NAME', 'field' => 'e_name'),
					  array('data' => 'STATUS', 'field' => 'status','width' => '12%'),
					  array('data' => 'ACTION', 'field' => 'actions','width' => '20%'),
	  		 	  );
	if(isset($_GET['sort']) && isset($_GET['order'])) {
    	if($_GET['sort'] == 'asc')
      		$sort = 'ASC';
    	else
      		$sort = 'DESC';
 
		switch($_GET['order']) {
		  case 'Identification':
			$order = 'identification_number';
			break;
		  case 'Establishment':
			$order = 'e_name';
			break;
		  default:
			$order = 'full_name_principal_emp';
		}
	}else{
		$sort = 'ASC';
		$order = 'e_name';
	}
	
	$query_clra_reg_apps				=	db_select('l_clra_registration_master', 'lcrm');
	$query_clra_reg_apps				->	fields('lcrm',array());
	$query_clra_reg_apps				->	condition('lcrm.user_id',$userid);
	$query_clra_reg_apps				->	orderBy('lcrm.id','DESC');
	$query_clra_reg_apps_result 		= 	$query_clra_reg_apps->execute();
	

	if( $query_clra_reg_apps_result->rowCount() > 0 ){
		
		$counter			= 0;
		$operation_link		= '';
		
		while($data = $query_clra_reg_apps_result->fetchObject()) {
			
			$counter++;
			$view = '<img title="View Or Edit Information" alt="View Or Edit Information" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/view_details.png">';
			
			
			
			if(empty($data->status) && empty($data->final_submit_status)){ 
			         		
				$STATUS_IMG 				='<span class="incomplete">Incomplete</span>';				
				$view_link					= l($view, 'view-clra-application-details/view-clra-application/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
          	}
			
			if( trim($data->status) == "0" && !empty($data->final_submit_status)){ 
			         		
				$STATUS_IMG 				='<span class="applied">Applied</span>';				
				$view_link					= l($view, 'view-clra-application-details/view-clra-application/'.encryption_decryption_fun($action, $data->id) , array('html' => TRUE));
				
          	}
			
          	if(trim($data->status) == "A" &&!empty($data->final_submit_status) ){
				          		
				$STATUS_IMG 				='Approved';				
				$view_link					= l($view, 'view-clra-application-details/view-clra-application/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
				
          	}
           	if( trim($data->status) == "B" && !empty($data->final_submit_status) ){
				          		
				$STATUS_IMG 				='<span class="pending">Backed</span>';
				$view_link					= l($view, 'view-clra-application-details/view-clra-application/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
				
          	}
			
          	if(trim($data->status) == "V" && !empty($data->final_submit_status) ){
				
                $STATUS_IMG 				= '<span class="feespending">Fees Pending</span>';				
				$view_link					= l($view, 'view-clra-application-details/view-clra-application/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
				
          	}
          	if( $data->status == "BI" && !empty($data->final_submit_status) ){
				          		
				$STATUS_IMG 				='<span class="underprocess">Processing</span>';
				$view_link					= l($view, 'view-clra-application-details/view-clra-application/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
          	}
          	if( $data->status == "C" && !empty($data->final_submit_status)){ 
			         		
				$STATUS_IMG 				='<span class="call_applicant">Call Applicant</span>';				
				$view_link					= l($view, 'view-clra-application-details/view-clra-application/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
				
          	}
          	if( trim($data->status) == "I" && !empty($data->final_submit_status) ){	
					
				$STATUS_IMG 				= '<span class="issued">Issued</span>';				
				$view_link					= l($view, 'view-clra-application-details/view-clra-application/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
				
          	}
          	if( $data->status == "R" && !empty($data->final_submit_status)){
				          		
				$STATUS_IMG 				='<span class="reject">Reject</span>';				
				$view_link					= l($view, 'view-clra-application-details/view-clra-application/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
				
          	}
			
			if( $data->status == "F" && !empty($data->final_submit_status)){  
			        		
				$STATUS_IMG 				='<span class="underprocess">Processing</span>';				
				$view_link					= l($view, 'view-clra-application-details/view-clra-application/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
				
          	}
			if( $data->status == "T" && !empty($data->final_submit_status)){ 
			         		
				$STATUS_IMG 				='<span class="feespaid">Fees Paid</span>';				
				$view_link					= l($view, 'view-clra-application-details/view-clra-application/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
				
          	} 
			
			if( $data->status == "S" && !empty($data->final_submit_status)){ 
			         		
				$STATUS_IMG 				='<span class="final_submit">Final Submit</span>';				
				$view_link					= l($view, $act_wise_link.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
				
          	}
			
			if(!empty($data->registration_number) && trim($data->status) == "I"){
				
				$registration_number 		= '<strong>'.$data->registration_number.'</strong><br>'.date('dS M Y', strtotime($data->registration_date));
				$download_certificate		= l('Download Certificate','download-pdf-clra-registration/'.time().'/'.$data->id.'/'.$data->user_id, array('attributes' => array('target' => '_blank')));
				
			}else{
				
				$registration_number 		= 'UNDER PROCESS';
				$download_certificate		= '';
			}
			
			if(!empty($data->application_date)){
				
				$application_date 			= date('dS M Y', strtotime($data->application_date));	
			}else{
				$application_date 			= '';
			}
			
			$operation_link 				= $view_link.'<br/>'.$download_certificate;
			
			
			
			
			
			
			$rows[] = array(
					array('data' =>	$counter, 'align' => 'left', 'class' => array('')),
					array('data' => '<strong>'.$data->identification_number.'</strong><br/>'.$application_date, 'align' => 'left', 'class' => array('')),
					array('data' =>	$registration_number, 'align' => 'left', 'class' => array('')),
					array('data' =>	$data->e_name, 'align' => 'left', 'class' => array('')),
					array('data' =>	$STATUS_IMG, 'align' => 'left', 'class' => array('')),					
					array('data' =>	$view_link.'<br/>'.$download_certificate, 'align' => 'left', 'class' => array(''))
			  );
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

