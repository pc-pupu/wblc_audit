<?php

function amendment_form_step_one($form, &$form_state){
	
	global $base_root, $base_path, $user;
	
	$user_id 			= $user->uid;
	
		 
	$form['amenmend_form']['regn_number'] 	= array(
												'#prefix' 			=> '<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">APPLICATION FOR AMENDMENT OF REGISTRATION CERTIFICATE FOR PRINCIPAL EMPLOYERS</div><div class="panel-body"><div class="col-md-8"><label class="input">',
												'#suffix' 			=> '</label></div></div></div>',
												'#title'			=> 'Enter your CLRA registration number:',
												'#type' 			=> 'textfield',
												'#required'			=>	TRUE,
												'#attributes'		=> array('autocomplete' => 'off'),
												'#description' 		=> '<font color="red"><strong>Note :- Enter the registration number of the certificate to be amended.</strong></font>',
											);
	
	$form['amenmend_form']['submit'] 		= array (
												'#prefix' 			=> '<div class="col-md-12"><div class="row">', 
												'#suffix' 			=> '</div></div></div>',
												'#type' 			=> 'submit',
												'#value' 			=> 'Continue ',
												'#attributes'		=> array('id' => 'submit_oneId' , 'class' => array('btn btn-primary pull-right form-submit'))
											);
															
							


	return $form;
}

function amendment_form_step_one_validate($form, &$form_state){
	$val=	$form_state['values'];
	
	foreach($form as $element){
		if(is_array($element)){
			if(isset($element['#type']) and ($element['#type'] == 'textfield')){
				chkbadchar($element['#value'], $element['#title'], $element['#name']);
			}
		}
 	}
	
	if( $val['regn_number'] == "" ){
		form_error($form['amenmend_form']['regn_number'], t('Registration Number cannot be blank.'));
	}
}


function fetch_clra_amended_applications($get_all_amendment_applications){
	
	global $base_root, $base_path, $user;
	
	$user_id 				= $user->uid;
	$counter				= 0;
	
	$action					=	'encrypt';
	$pay_now 				= 	'<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
	$pay_now 				.= 	'<span style="color:#aeaeae;">Pay Now</span>';
	$payment_details 		= 	'';
	$output 				= 	'';
	$view_link				=	'';
	$operation_link 		= 	'';
	$registration_number 	=   'UNDER PROCESS';
	$download_certificate	=   '';
	$form_v_link			=   '';
	
	$header 				= array(
								  array('data' => 'Sl. No', 'field' => 'slno', 'width' => '5%'),
								  array('data' => 'REGISTRATION NO. <br/> ISSUE DATE', 'field' => 'registration_number', 'width' => '18%'),
								  array('data' => 'ESTABLISHMENT NAME', 'field' => 'e_name', 'width' => '22%'),
								  array('data' => 'ACT', 'field' => 'act_id', 'width' => '10%'),
								  array('data' => 'STATUS', 'field' => 'status','width' => '10%', 'class' => array('not-sorting')),
								  array('data' => 'ACTION', 'field' => 'actions', 'width' => '20%', 'class' => array('not-sorting')),
							  );
				  
	while($data = $get_all_amendment_applications->fetchObject()) {
		
		$counter++;
		$view 							= '<img title="View Or Edit Information" alt="View Or Edit Information" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">'.'View Details';
		
		$act_name						= '';
		$act_wise_link					= '';
		$download_certificate_link 		= '';
		$remark_link					= '';
		$payment_details				= '';
		$upload_forms_link				= '';
		$pay_now						= '';
		$download_certificate			= '';
		$registration_number			= '';
		$form_v_link					= '';
		
		if(!empty($data->application_date)){
			$application_date 			= date('dS M Y', strtotime($data->application_date));	
		}else{
			$application_date 			= '';
		}
		
		$act_name					= 'CLRA REG AMENDMENT';
		$act_wise_link 				= 'clra-reg-amendment/view-clra-application/';
		$act_wise_view_link 		= 'clra-application/view/';
			
			
			
		if(empty($data->status) && empty($data->final_submit_status)){
										
			$STATUS_IMG 					='<img title="Incomplete" alt="Incomplete" width="32px" height="32px" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/incomplete.png">';			
			$view_link 						= l($view, $act_wise_link.encryption_decryption_fun($action, $data->id), array('html' => TRUE));				
			
			$download_certificate 			= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$download_certificate 	   	   .= '<span style="color:#aeaeae;">Download Certificate</span>';
			
			$remark_link 				    = '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$remark_link 			       .= '<span style="color:#aeaeae;">View Remarks</span>';				
			
		}
		
		if( trim($data->status) == "0" && !empty($data->final_submit_status)){	
								
			$STATUS_IMG 					='<img title="Applied" alt="Applied" width="32px" height="32px" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/applied.png">';				
			$view_link						= l($view, $act_wise_link.encryption_decryption_fun($action, $data->id) , array('html' => TRUE));
			
			
			$download_certificate			= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$download_certificate	   	   .= '<span style="color:#aeaeae;">Download Certificate</span>';
			
			$remark_link 					= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$remark_link 				   .= '<span style="color:#aeaeae;">View Remarks</span>';		
		}
		
	  
		if( trim($data->status) == "B" && !empty($data->final_submit_status) ){
							

			$STATUS_IMG 					='<img title="Back for correction" alt="Back for correction" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/back.png">';	
			$view_link						= l($view, $act_wise_link.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
							
			$remark_link					= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$remark_link	 			   .= l('View Remarks', 'view-remark/'.encryption_decryption_fun($action, $data->id).'/'.encryption_decryption_fun($action, $data->act_id), array('html' => TRUE));
			
			$download_certificate		    = '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$download_certificate	       .= '<span style="color:#aeaeae;">Download Certificate</span>';
		}
		
		if(trim($data->status) == "VA" && !empty($data->final_submit_status) ){				

			$STATUS_IMG 					='<img title="Approved" alt="Approved" width="32px" height="32px" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/approved-nw.png">';				
			$view_link						= l($view, $act_wise_link.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
			
			$download_certificate			= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$download_certificate	   	   .= '<span style="color:#aeaeae;">Download Certificate</span>';
			
			$remark_link 					= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$remark_link 				   .= l('View Remarks', 'view-remark/'.encryption_decryption_fun($action, $data->id).'/'.encryption_decryption_fun($action, $data->act_id), array('html' => TRUE));
			
			if($data->act_id == 1){
				
				$upload_forms_link 			= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
				$upload_forms_link 	       .= l('<span>Upload</span>', 'upload_signed_application_form/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));				
				
			}else if($data->act_id == 2){					
				
				$upload_forms_link 			= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
				$upload_forms_link 		   .= l('<span>Upload</span>', 'bocwa-upload-signed-application/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));					
								
			}				
		}
		
		if(trim($data->status) == "V" && !empty($data->final_submit_status) ){
			

			$STATUS_IMG 					='<img title="Fees Pending" alt="Fees Pending" width="32px" height="32px" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/fees_pending.png">';				
			$view_link						= l($view, $act_wise_link.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
			
			$pay_now 						= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$pay_now 				   	   .= l('<span>Pay Now</span>', 'epayments_preview/'.encryption_decryption_fun($action, $data->id).'/'.encryption_decryption_fun($action, $data->act_id), array('html' => TRUE));
			
			$download_certificate		    = '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$download_certificate	       .= '<span style="color:#aeaeae;">Download Certificate</span>';
			
			$remark_link 					= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$remark_link 				   .=  l('View Remarks', 'view-remark/'.encryption_decryption_fun($action, $data->id).'/'.encryption_decryption_fun($action, $data->act_id), array('html' => TRUE));
			
		}
		
		
		if( $data->status == "BI" && !empty($data->final_submit_status) ){
							

			$STATUS_IMG 					='<img title="In Process" alt="In Process" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/work-in-process.png">';
			$view_link						= l($view, $act_wise_link.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
			
			$download_certificate			= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$download_certificate	   	   .= '<span style="color:#aeaeae;">Download Certificate</span>';
			
			$remark_link 					= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$remark_link 				   .=  l('View Remarks', 'view-remark/'.encryption_decryption_fun($action, $data->id).'/'.encryption_decryption_fun($action, $data->act_id), array('html' => TRUE));
		}
		
		
		if( $data->status == "C" && !empty($data->final_submit_status)){ 			         		

			$STATUS_IMG 					='<img title="Call Applicant" alt="Call Applicant" width="32px" height="32px" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/phoneicon.png">';
			$view_link 						= l($view, $act_wise_link.encryption_decryption_fun($action, $data->id), array('html' => TRUE));			
			
			
			$remark_link 					= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$remark_link 				   .= l('View Remarks', 'view-remark/'.encryption_decryption_fun($action, $data->id).'/'.encryption_decryption_fun($action, $data->act_id), array('html' => TRUE));
			
			$download_certificate			= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$download_certificate	   	   .= '<span style="color:#aeaeae;">Download Certificate</span>';
			
		}
		
		
		if( trim($data->status) == "I" && !empty($data->final_submit_status) && !empty($data->registration_number) ){	
				

			$STATUS_IMG 					='<img title="Issued" alt="Issued" width="32px" height="32px" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/issue.png">';			
			$view_link						= l($view, $act_wise_link.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
			
			$download_certificate			= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$download_certificate	   	   .= l('Download Certificate',$download_certificate_link, array('attributes' => array('target' => '_blank')));
			
			$remark_link 					= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$remark_link 				   .= l('View Remarks', 'view-remark/'.encryption_decryption_fun($action, $data->id).'/'.encryption_decryption_fun($action, $data->act_id), array('html' => TRUE));
			
			$registration_number 			= '<strong>'.$data->registration_number.'</strong><br>'.date('dS M Y', strtotime($data->registration_date));
			
			
			if($data->act_id == 1){
				$form_v_link				= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
				$form_v_link			   .= l('<span>Download Form-V</span>', 'view-contractors-form-v/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
			}
			
		}else{
			
			
			$registration_number 			= 'UNDER PROCESS';
		}
		
		
		if( $data->status == "R" && !empty($data->final_submit_status)){
							

			$STATUS_IMG 					='<img title="Reject" alt="Reject" width="32px" height="32px" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/rejected.png">';			
			$view_link						= l($view, $act_wise_link.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
			
			$remark_link					= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$remark_link	 			   .= l('View Remarks', 'view-remark/'.encryption_decryption_fun($action, $data->id).'/'.encryption_decryption_fun($action, $data->act_id), array('html' => TRUE));
			
			$download_certificate		    = '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$download_certificate	       .= '<span style="color:#aeaeae;">Download Certificate</span>';
			
		}
		
		if( $data->status == "F" && !empty($data->final_submit_status)){  
						

			$STATUS_IMG  					='<img title="In Process" alt="In Process" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/work-in-process.png">';				
			$view_link	 					= l($view, $act_wise_link.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
			
			$download_certificate 			= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$download_certificate 		   .= '<span style="color:#aeaeae;">Download Certificate</span>';
			
			$remark_link 					= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$remark_link 				   .= l('View Remarks', 'view-remark/'.encryption_decryption_fun($action, $data->id).'/'.encryption_decryption_fun($action, $data->act_id), array('html' => TRUE));
			
		}
		
		if( $data->status == "T" && !empty($data->final_submit_status)){ 
						

			$STATUS_IMG 					= '<img title="Fees Paid" alt="Fees Paid" width="32px" height="32px" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/fees-received.png">';				
			$view_link						= l($view, $act_wise_link.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
			
			$payment_details				= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$payment_details			   .= l('Payment Details', 'payment-details/'.encryption_decryption_fun($action, $data->id).'/'.encryption_decryption_fun($action, $data->act_id), array('html' => TRUE));
			
			$download_certificate			= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$download_certificate	   	   .= '<span style="color:#aeaeae;">Download Certificate</span>';
			
			$remark_link 					= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$remark_link 				   .= l('View Remarks', 'view-remark/'.encryption_decryption_fun($action, $data->id).'/'.encryption_decryption_fun($action, $data->act_id), array('html' => TRUE));
			
			if($data->act_id == 1){
				
				$upload_forms_link			= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
				$upload_forms_link		   .= l('<span>Upload </span>', 'upload_signed_application_form/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
				
			}else if($data->act_id == 2){
				
				$upload_forms_link			= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
				$upload_forms_link		   .= l('<span>Upload</span>', 'bocwa-upload-signed-application/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));	
			}
			
		}
		
		if( $data->status == "S" && !empty($data->final_submit_status)){ 
						

			$STATUS_IMG 					= '<img title="final_submit" alt="final_submit" width="32px" height="32px" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/final-submit.png">';			
			$view_link						= l($view, $act_wise_link.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
			
			$download_certificate			= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$download_certificate	   	   .= '<span style="color:#aeaeae;">Download Certificate</span>';
			
			$remark_link 					= '<br><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
			$remark_link 				   .= l('View Remarks', 'view-remark/'.encryption_decryption_fun($action, $data->id).'/'.encryption_decryption_fun($action, $data->act_id), array('html' => TRUE));
			
		}
		
					
		
		if((!empty($data->final_submit_status)) && trim($data->status) != "B" && trim($data->status) != "C"){
							
			$view_link	= l($view, $act_wise_view_link.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
			
		}
			
			$rows[] = array(
					array('data' =>	$counter, 'align' => 'left', 'class' => array('')),
					array('data' => $registration_number, 'align' => 'left', 'class' => array('')),
					array('data' =>	$data->e_name, 'align' => 'left', 'class' => array('')),
					array('data' =>	'<strong>'.$act_name.'</strong>', 'align' => 'left', 'class' => array('')),
					array('data' =>	$STATUS_IMG, 'align' => 'left', 'style' => array('text-align:center;')),					
					array('data' =>	$view_link.$remark_link.$upload_forms_link.$pay_now.$download_certificate.$form_v_link.$payment_details, 'align' => 'left', 'class' => array(''))
			  );
		
	}
	
	$variables = array(
	  		'attributes' 		=> array('class' => array('clra_applications')), 
	  		'header' 			=> $header,
	  		'rows'				=> $rows,
			'empty' 			=> t("No data found!")
	  );
	
	$output = theme('datatable', $variables);
	$output.=   '<style type="text/css">
				.div-head{font-weight:bold;text-align:center;font-family:cursive;width: 100%;}
				</style>
					
				
				<table style="border:1px solid #000; width: 100%;" class="clra_applications sticky-enabled datatables-processed dataTable">
					<tr style="border:none;"><td colspan="12"><div class="div-head"><h3>Icon Reference</h3></div></td>
					<tr style="background:#3294D6;">
						<th>Icon</th><th>Description</th>
						<th>Icon</th><th>Description</th>
						<th>Icon</th><th>Description</th>
						<th>Icon</th><th>Description</th>
						<th>Icon</th><th>Description</th>
						<th>Icon</th><th>Description</th>
					</tr>
					<tr style="background: #fff;">
						<td><img title="Incomplete" alt="Incomplete" width="32px" height="32px" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/incomplete.png"></td><td>Incomplete</td>
						<td><img title="Applied" alt="Applied" width="32px" height="32px" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/applied.png"></td><td>Applied</td>
						<td><img title="Approved" alt="Approved" width="32px" height="32px" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/approved-nw.png"></td><td>Approved</td>
						<td><img title="Back for correction" alt="Back for correction" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/back.png"></td><td>Back for correction</td>
						<td><img title="Fees Pending" alt="Fees Pending" width="32px" height="32px" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/fees_pending.png"></td><td>Fees Pending</td>
						<td><img title="Final Submit" alt="Final Submit" width="32px" height="32px" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/final-submit.png"></td><td>Final Submit</td>
					</tr>
					<tr style="background: #fff;">
						<td><img title="In Process" alt="In Process" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/work-in-process.png"></td><td>In Process</td>
						<td><img title="Call Applicant" alt="Call Applicant" width="32px" height="32px" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/phoneicon.png"></td><td>Call Applicant</td>
						<td><img title="Issued" alt="Issued" width="32px" height="32px" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/issue.png"></td><td>Issued</td>
						<td><img title="Reject" alt="Reject" width="32px" height="32px" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/rejected.png"></td><td>Reject</td>
						<td><img title="Fees Paid" alt="Fees Paid" width="32px" height="32px" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/fees-received.png"></td><td>Fees Paid</td>
						<td>&nbsp;</td>
					</tr>
				</table>';

	
	return $output;
				  
	
	
	
}

function amendment_form_step_one_submit($form, &$form_state){
	global $base_root, $base_path, $user;
	
	$exitrow = db_query("select id, status FROM l_clra_registration_master WHERE user_id = '".$user->uid."' order by id desc limit 1")->fetchObject();		
	if($exitrow->status != 'I'){	// Yes
		drupal_goto('applicant-dashboard');
	}else{
	
	$val 					=	$form_state['values'];
	$registration_no		=	trim($val['regn_number']);
	
	if(!empty($registration_no)){		
		$fetchExistingIdenNumber = db_select('l_clra_registration_master', 'lcrm');
		$fetchExistingIdenNumber->fields('lcrm', array());
		$fetchExistingIdenNumber->condition('lcrm.registration_number', trim($registration_no));
		$fetchExistingIdenNumber->condition('lcrm.user_id', $user->uid);
		$fetchExistingIdenNumber->condition('lcrm.status', 'I');
		$fetchExistingIdenNumber->orderBy('lcrm.id','DESC');
		$fetchExistingIdenNumber->range(0, 1);
		$fetchExistingIdenNumber_res = $fetchExistingIdenNumber->execute();
		$fetchExistingIdenNumber_data = $fetchExistingIdenNumber_res->fetchObject();
		
		if( $fetchExistingIdenNumber_res->rowCount() > 0 ){	// Yes					
			$edit_id = 	$fetchExistingIdenNumber_data->id;
			$edit_id = encryption_decryption_fun('encrypt', $edit_id);			
			drupal_goto('apply-clra-reg-amendment/'.$edit_id);			
		}else{	// No					
			$get_bklg_tbl =	db_select('l_clra_principle_emp_backlog_data', 'lcpebd');
			$get_bklg_tbl->fields('lcpebd', array());
			$get_bklg_tbl->condition('lcpebd.registration_no', $registration_no);
			$get_bklg_tbl_res = $get_bklg_tbl->execute();
			$get_bklg_tbl_res_data = $get_bklg_tbl_res->fetchObject();
			
			if( $get_bklg_tbl_res->rowCount() > 0 ){
				if( $get_bklg_tbl_res_data->status == 1 ){					
					// invalid
					form_set_error('', t('Invalid registration number. Please enter valid registration number.'));
														
				}elseif($get_bklg_tbl_res_data->status != 1 ){  // if 0 or not 1									
					$id	= $get_bklg_tbl_res_data->id;
					$id = encryption_decryption_fun('encrypt', $id);
					drupal_goto('clra-backlog/'.$id);									
				}
				
			}else{
				form_set_error('', t('Registration number does not exist. Please enter valid registration number.'));
			}	
		}	
	}
	}
}


