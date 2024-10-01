<?php
function bocwa_all_applications_form($form, &$form_state){
	
	global $base_root, $base_path, $user;
	
	$_SESSION['uid'] 	= $user->uid;
	$user_id			= $user->uid;
	
	drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
	drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
	
	$form['bocwa_applications'] 					= array(
														  '#type' 				=> 'fieldset',
														  '#prefix' 			=> '<div class="content"><div class="sky-form">',
												 			);
	
	$form['bocwa_applications']['markup_data_1']	= array(
															'#markup' 			=> l('New Application', 'new-bocwa-application', array( 'html' => TRUE, 'attributes'=> array('class' => array('button left-btn')))),
															);
	
	$fetch_bocwa_applications_with_respect_to_id 	= fetch_bocwa_applications_with_respect_to_id();
	
	$form['bocwa_applications']['markup_data_2']	= array(
														'#markup' 				=> ''.$fetch_bocwa_applications_with_respect_to_id.'',
														'#suffix' 				=> '</div></div>',
														);
	return $form;
}

function fetch_bocwa_applications_with_respect_to_id(){
	
	global $base_root, $base_path, $user;
	
 	$userid			=	$user->uid;
	$output 		= '';
	
	$header 		= array(
						  array('data' => 'SL. NO', 'field' => 'slno','width' => '7%'),
						  array('data' => 'IDENTIFICATION NO.<br/>APPLY DATE', 'field' => 'identification_number','width' => '20%'),
						  array('data' => 'REGISTRATION NO. <br/> ISSUE DATE', 'field' => 'registration_number','width' => '20%'),
						  array('data' => 'ESTABLISHMENT NAME', 'field' => 'e_name'),
						  array('data' => 'STATUS', 'field' => 'status','width' => '16%'),
						  array('data' => 'OPERATIONS', 'field' => 'actions'),
					  );
	if(isset($_GET['sort']) && isset($_GET['order'])) {
    	if($_GET['sort'] 	== 'asc')
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
	
	$query_lbrm_reg_apps				=	db_select('l_bocwa_registration_master', 'lbrm');
	$query_lbrm_reg_apps				->	fields('lbrm',array());
	$query_lbrm_reg_apps				->	condition('lbrm.user_id',$userid);
	$query_lbrm_reg_apps_result 		= 	$query_lbrm_reg_apps->execute();
	

	if( $query_lbrm_reg_apps_result->rowCount() > 0 ){
		
		$counter			= 0;
		$operation_link		= '';
		
		$view = '<img title="View Or Edit Information" alt="View Or Edit Information" src="'.$base_root.$base_path.'/sites/all/themes/jackson/images/view_details.png">';
		
		while($data = $query_lbrm_reg_apps_result->fetchObject()) {
			
			$counter++;
			
			if(empty($data->status) && empty($data->final_submit_status)){ 
			         		
				$STATUS_IMG 				='<span class="incomplete">Incomplete</span>';				
				$view_link					= l($view, 'bocwa-application/default/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
          	}
			
			if( trim($data->status) == "0" && !empty($data->final_submit_status)){ 
			         		
				$STATUS_IMG 				='<span class="applied">Applied</span>';				
				$view_link					= l($view, 'bocwa-application/default/'.encryption_decryption_fun($action, $data->id) , array('html' => TRUE));
				
          	}
			
          	if(trim($data->status) == "A" &&!empty($data->final_submit_status) ){
				          		
				$STATUS_IMG 				='Approved';				
				$view_link					= l($view, 'bocwa-application/default/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
				
          	}
           	if( trim($data->status) == "B" && !empty($data->final_submit_status) ){
				          		
				$STATUS_IMG 				='<span class="pending">Backed</span>';
				$view_link					= l($view, 'bocwa-application/default/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
				
          	}
			
          	if(trim($data->status) == "V" && !empty($data->final_submit_status) ){
				
                $STATUS_IMG 				= '<span class="feespending">Fees Pending</span>';				
				$view_link					= l($view, 'bocwa-application/default/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
				
          	}
          	if( $data->status == "BI" && !empty($data->final_submit_status) ){
				          		
				$STATUS_IMG 				='<span class="underprocess">Processing</span>';
				$view_link					= l($view, 'bocwa-application/default/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
          	}
          	if( $data->status == "C" && !empty($data->final_submit_status)){ 
			         		
				$STATUS_IMG 				='<span class="call_applicant">Call Applicant</span>';				
				$view_link					= l($view, 'bocwa-application/default/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
				
          	}
          	if( trim($data->status) == "I" && !empty($data->final_submit_status) ){	
					
				$STATUS_IMG 				= '<span class="issued">Issued</span>';				
				$view_link					= l($view, 'bocwa-application/default/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
				
          	}
          	if( $data->status == "R" && !empty($data->final_submit_status)){
				          		
				$STATUS_IMG 				='<span class="reject">Reject</span>';				
				$view_link					= l($view, 'bocwa-application/default/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
				
          	}
			
			if( $data->status == "F" && !empty($data->final_submit_status)){  
			        		
				$STATUS_IMG 				='<span class="underprocess">Processing</span>';				
				$view_link					= l($view, 'bocwa-application/default/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
				
          	}
			if( $data->status == "T" && !empty($data->final_submit_status)){ 
			         		
				$STATUS_IMG 				='<span class="feespaid">Fees Paid</span>';				
				$view_link					= l($view, 'bocwa-application/default/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
				
          	} 
			
			
			if(!empty($data->registration_number) && trim($data->status) == "I"){
				
				$registration_number 		= '<strong>'.$data->registration_number.'</strong><br>'.date('dS M Y', strtotime($data->registration_date));
				$download_certificate		= l('Download Certificate','generate-bocwa-certificate/'.encryption_decryption_fun('encrypt', $data->bocwa_qr_code), array('attributes' => array('target' => '_blank')));
				
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
					array('data' =>	$operation_link.$pay_now, 'align' => 'left', 'class' => array(''))
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

/*function encryption_decryption_fun($action, $string) {
    $output 		= false;
    $encrypt_method = "AES-256-CBC";
    
    $secret_key 	= 'labour';
    $secret_iv 		= 'Lc#Nic@2015';
	
    $key = hash('sha512', $secret_key);
	
    $iv = substr(hash('sha512', $secret_iv), 0, 16);

    if( $action == 'encrypt' ) {
        $output = openssl_encrypt($string, $encrypt_method, $key, 0, $iv);
        $output = base64_encode($output);
    }else if( $action == 'decrypt' ){
        $output = openssl_decrypt(base64_decode($string), $encrypt_method, $key, 0, $iv);
    }
    return $output;
}*/