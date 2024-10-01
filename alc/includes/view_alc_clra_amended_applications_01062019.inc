<?php

function view_alc_clra_amended_applications($form, &$form_state, $id = '',$app_user_id=''){
	
	drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
	drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
	drupal_add_js(drupal_get_path('module', 'alc').'/js/mycustom.js');
	
	global $base_root, $base_path, $user;
	
	$user_id 					= $user->uid;
	$action	 					= 'decrypt';
	$action_encrypt				= 'encrypt';
	$application_id				= encryption_decryption_fun($action, $id);
	$applicant_user_id			= encryption_decryption_fun($action, $app_user_id);
	
		
	/**** Fetch ALC SUBDIVISION Code ****/
	
	$get_alc_subdivision_details					=	db_select('l_customuser_relation_address', 'lcra');
	$get_alc_subdivision_details					->  leftJoin('users_roles', 'ur','ur.uid=lcra.user_id ');
	$get_alc_subdivision_details					->  leftJoin('role', 'ro', 'ro.rid = ur.rid');
	$get_alc_subdivision_details					->  fields('ro', array('name','rid'));
	$get_alc_subdivision_details					->	fields('lcra',array('sub_div_code'));
	$get_alc_subdivision_details					->	condition('lcra.user_id',$user_id);
	$get_alc_subdivision_details_result 			= 	$get_alc_subdivision_details->execute();
	
	if($get_alc_subdivision_details_result-> rowCount() > 0 ){
		
		$alc_data 					= $get_alc_subdivision_details_result->fetchAssoc();  
		$alc_subdivision_code 		= trim($alc_data['sub_div_code']);
		$alc_role_rid				= $alc_data['rid']; 
		$alc_role_name				= $alc_data['name']; 
		
	}
	
	/*** Get Applicant Block Code & Statuses ****/
	
	$get_clra_amended_application							=	db_select('l_clra_registration_master', 'lcrm');
	$get_clra_amended_application							->	fields('lcrm',array('loc_e_subdivision','final_submit_status','status','registration_number','amendment_parent_id','e_any_day_max_num_of_workmen','amendment_reference_no','certificates_fid'));
	$get_clra_amended_application							->	condition('lcrm.loc_e_subdivision',$alc_subdivision_code);
	$get_clra_amended_application							->	condition('lcrm.id',$application_id);
	$get_clra_amended_application							->	condition('lcrm.user_id',$applicant_user_id);
	$get_clra_amended_application							->	condition('lcrm.final_submit_status','P');
	$get_clra_amended_application_result 					= 	$get_clra_amended_application->execute();
	
	
	 
	if($get_clra_amended_application_result-> rowCount() > 0 ){
		
		$applicant_data 							= $get_clra_amended_application_result->fetchAssoc(); 
		$applicant_subdivsion_code 					= trim($applicant_data['loc_e_subdivision']);
		$clra_registration_number					= $applicant_data['registration_number'];
		$applicant_final_submit_status				= $applicant_data['final_submit_status']; 
		$applicant_status							= $applicant_data['status'];
		$amendment_reference_no						= $applicant_data['amendment_reference_no'];
		$amendment_parent_id						= $applicant_data['amendment_parent_id'];
		$certificates_fid							= $applicant_data['certificates_fid'];
		$fees 										= $applicant_data['e_any_day_max_num_of_workmen'];
		
		
		if(!empty($certificates_fid)){
			$upload_certificates_id 		= db_select('file_managed','fm');
			$upload_certificates_id			-> fields('fm',array());
			$upload_certificates_id			-> condition('fid',$certificates_fid);
			$upload_certificates_file		= $upload_certificates_id-> execute()->fetchAssoc();
			
			if(!empty($upload_certificates_file)){ 
			  $url									= explode('//',$upload_certificates_file['uri']);
			  $upload_certificates_file_url			= $url[1];
			  $upload_certificates_file_name		= $upload_certificates_file['filename'];
		  }
			
		}
		
		
		/** Fees Calculation Part ****/ 
		
		if($fees <= 20 ){
			$fees_payable	=	'200';
		}else if($fees > 20 && $fees <= 50  ){
			$fees_payable	=	'500';
		}else if($fees > 50 && $fees <= 100  ){
			$fees_payable	=	'1000';
		}else if($fees > 100 && $fees <= 200  ){
			$fees_payable	=	'2000';
		}else if($fees > 200 && $fees <= 400  ){
			$fees_payable	=	'4000';
		}else if($fees > 400){
			$fees_payable	=	'5000';
		}
		
		/*** Fees Already Paid(not needed) ****/
		
		$get_last_submitted_fees                = db_select('l_clra_registration_master', 'lcrm_old');
		$get_last_submitted_fees				->fields('lcrm_old',array('backlog_id'));
		$get_last_submitted_fees				->condition('lcrm_old.id',$amendment_parent_id);
		$get_last_submitted_fees				->condition('lcrm_old.status','I');
		$last_submitted_fees_result 			= $get_last_submitted_fees->execute()->fetchAssoc();
		
		// $fees_paid                         		= $last_submitted_fees_result['finalfees']; 
		
		$fees_paid = get_clra_pe_total_paid_amount($applicant_user_id, $application_id);
		
		// if($application_id == 5356 && $applicant_user_id = 4545){
		//	$fees_paidAmnt_details = get_clra_pe_total_paid_amount_details($applicant_user_id, $application_id);		
		//	echo $fees_paid = $fees_paidAmnt_details['total_amount']; echo $fees_paidAmnt_details['amount_payable_status']; die;
		// }
		
		
		/*if(!empty($last_submitted_fees_result['backlog_id'])){
			$get_backlog_fees = db_select('l_clra_principle_emp_backlog_data', 'lcpebd');
			$get_backlog_fees->fields('lcpebd',array('name_of_the_establishment', 'fees'));
			$get_backlog_fees->condition('lcpebd.id', $last_submitted_fees_result['backlog_id']);		
			$get_backlog_fees_result = $get_backlog_fees->execute()->fetchAssoc();
		
			if($get_backlog_fees_result['fees'] > $fees_paid){		
				$fees_paid = $get_backlog_fees_result['fees'];
			}
		}*/
		
		if($fees_payable > $fees_paid){
			$amount_payable_status  = 'Y';  
		}else{
			$amount_payable_status = 'N';
		}
		
		if(!empty($last_submitted_fees_result['backlog_id'])){
			$get_backlog_fees = db_select('l_clra_principle_emp_backlog_data', 'lcpebd');
			$get_backlog_fees->fields('lcpebd',array('maximum_no_of_contact_labour', 'fees'));
			$get_backlog_fees->condition('lcpebd.id', $last_submitted_fees_result['backlog_id']);		
			$get_backlog_fees_result = $get_backlog_fees->execute()->fetchAssoc();
		
			if(($amount_payable_status  == 'Y') && $get_backlog_fees_result['maximum_no_of_contact_labour'] >= $applicant_data['e_any_day_max_num_of_workmen']){		
				$amount_payable_status  = 'YN';
			}
		}
		
		
				
		
		if($applicant_status == 'B'){
			$current_status = 'CURRENT STATUS: Application is BACKED to Applicant';
		}
		if($applicant_status == 'BI'){
			$current_status = 'CURRENT STATUS: Application is BACKED TO INSPECTOR';
		}
		if($applicant_status == 'C'){
			$current_status = 'CURRENT STATUS: Applicant is CALLED BY ALC';
		}
		if($applicant_status == 'I'){
			$current_status = 'CURRENT STATUS: REGISTRATION CERTIFICATE IS ISSUED';
		}
		if($applicant_status == 'F'){
			$current_status = 'CURRENT STATUS: Application is FORWARDED TO ALC';
		}
		if($applicant_status == 'T'){
			$current_status = 'CURRENT STATUS: TRANSACTION IS SUCCESSFULL for this Application';
		}
		if($applicant_status == 'V'){
			$current_status = 'CURRENT STATUS: Application is VERIFIED & GIVEN APPROVAL FOR FEES SUBMISSION';
		}
		if($applicant_status == 'R'){
			$current_status = 'CURRENT STATUS: Application is REJECTED';
		}
		if($applicant_status == 'VA'){
			$current_status = 'CURRENT STATUS: Application is APPROVED & does not require FEES SUBMISSION';
		}
		if($applicant_status == 'S'){
			$current_status = 'CURRENT STATUS: Form-I is uploaded & Application has been FINALLY SUBMITTED';
		} 
		if($applicant_status == 'U'){
			$current_status = 'CURRENT STATUS: Form-I is sent BACK FOR CORRECTION';
		} 
		
	}
		
	/*** Get REMARK Details ****/
	
	$get_remark = db_select('l_remarks_master', 'lrm');
	$get_remark->fields('lrm', array('remark_field_text','remark_type'));
	$get_remark->condition('lrm.remark_to', trim($applicant_user_id));
	$get_remark->condition('lrm.application_id', trim($application_id));
	$get_remark->condition('lrm.remark_by', trim($applicant_user_id), '!=');
	$get_remark->orderBy('lrm.remark_id','DESC');
	$get_remark->range(0, 1);
	$get_remark_result = $get_remark->execute(); 
			
	$remark_details = $get_remark_result->fetchAssoc();
	
	
	
	if($alc_subdivision_code == $applicant_subdivsion_code ){ 
	
		$form['view_clra_amended_form'] = array(
								  '#type' 		=> 'fieldset',
								  '#attributes'	=> array('class'=>array(''))
								  );
																									
		$form['view_clra_amended_form']['markup_data'] = array(
									'#prefix' 			=> '',
									'#markup' 			=> view_clra_amended_form_func_alc($application_id,$applicant_user_id,$fees_paid,$fees_payable),
									'#suffix' 			=> '',
									);																										
		
																										
		$form['view_clra_amended_form']['show_status'] 	= array(
									'#prefix'			=> '<div class="comments" align="center">',
									'#suffix'			=> '</div>',
									'#type' 			=> 'markup',
									'#markup'			=> $current_status,
									);
																									
		$form['view_clra_amended_form']['markup_data_1'] = array(
									'#type' 				=> 'markup',
									'#markup' 				=> '<div class="content"><div class="sky-form" style="width:100%"><header><h3><b>ACTIONS AND REMARK</b></h3></header><fieldset>',
									);
																									
																									
		$form['view_clra_amended_form']['hidden_action_message'] = array(
									'#prefix'				=> '<span style="margin:0; width:100%;">',
									'#suffix'				=> '</span>',
									'#type' 				=> 'markup',
									'#markup'				=> isset($form_state['values']['hidden_action_message']) ? $form_state['values']['hidden_action_message'] : ''
									);
																								
		if($applicant_status == 'I'){
				
			$form['view_clra_amended_form']['view_uploaded_certificate'] = array(
									'#markup' =>	l(t('Uploaded Signed Amended Certificate  <img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/pdf.png">'),''.$GLOBALS['base_url'].'/sites/default/files/'.$upload_certificates_file_url, array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('#pdf-img')))));		
				
		}																									
																									
		if ($applicant_status != 'B' && $applicant_status != 'V' && $applicant_status != 'I' && $applicant_status != 'VA' && $applicant_status != 'T' && $applicant_status != 'U' ){																									
																									
				$form['view_clra_amended_form']['fieldname'] = array(
										'#title'		 => 'Field Name',
										'#type'			 => 'hidden',
										'#attributes'	 => array('id' => 'amended_field_name_alc'),
										'#default_value' => !empty($remark_details['remark_field_text']) ? $remark_details['remark_field_text'] : '',
										);
																								
				$form['view_clra_amended_form']['applicant_user_id'] = array(
										'#title'				=> 'User Id',
										'#type'					=> 'hidden',
										'#attributes' 			=> array('readonly' => 'readonly'),
										'#default_value' 		=> !empty($applicant_user_id) ? encryption_decryption_fun('encrypt',$applicant_user_id) : '',
										);
																									
				$form['view_clra_amended_form']['amendment_id']	= array(
										'#title'				=> 'Amendment Id',
										'#type'					=> 'hidden',
										'#default_value' 		=> !empty($amendment_parent_id) ? encryption_decryption_fun('encrypt',$amendment_parent_id) : '',
										'#attributes' 			=> array('readonly' => 'readonly'),
										);
																											
				$form['view_clra_amended_form']['application_id'] = array(
										'#title'				=> 'Application Id',
										'#type'					=> 'hidden',
										'#default_value' 		=> !empty($application_id) ? encryption_decryption_fun('encrypt',$application_id) : '',
										'#attributes' 			=> array('readonly' => 'readonly'),
										);
																									
				$form['view_clra_amended_form']['remark_type'] = array(
										'#prefix' 				=> '<section><label class="select">',
										'#suffix' 				=> '<i></i></label></section>',
										'#title'				=> 'Please Select Action',
										'#type'					=> 'select',
										'#required' 			=>  TRUE,
										'#options'				=>  actions_amendment_dropdown_by_alc($applicant_status,$amount_payable_status),
										'#ajax'					=> array(
																		'event' 			=> 'change',
																		'effect' 			=> 'fade',			
																		'callback'			=> 'get_call_date_time_field_amendment',
																		'progress'			=> array('type' => 'throbber'),
																	  ),
										);
																									
																								
				$form['view_clra_amended_form']['call_applicant'] = array(
										  '#prefix' => '<div id="call_applicant_block">',
										  '#suffix' => '</div>'
										);
																									
				$form['view_clra_amended_form']['generate_certificate'] = array(
										  '#prefix' => '<div id="generate_certificate_block">',
										  '#suffix' => '</div>'
										);
																									
				$form['view_clra_amended_form']['download_link'] = array(
										  '#prefix' => '<div id="download_link_block">',
										  '#suffix' => '</div>'
										);
																									
				$form['view_clra_amended_form']['amended_clra_signed_certificate'] = array(
										  '#prefix' => '<div id="clra_amend_signed_certificate_block">',
										  '#suffix' => '</div>'
										);
																									
				$form['view_clra_amended_form']['remarks_text'] = array(
										  '#prefix' => '<div id="remarks_text_block">',
										  '#suffix' => '</div>'
										);
																									
																									
				$form['view_clra_amended_form']['submit'] = array(
										  '#prefix' => '<div id="submit_block">',
										  '#suffix' => '</div>'
										);
																									
				$value_remark_type = isset($form_state['values']['remark_type']) ? $form_state['values']['remark_type'] : '';
		
		
		
			/**** For CALL APPLICANT SECTION *******/
						
						
			if(!empty($value_remark_type) && trim($value_remark_type)== 'C'){	
			
				drupal_add_css(drupal_get_path('module', 'alc').'/js/jquery.datetimepicker.css');
				drupal_add_js(drupal_get_path('module', 'alc').'/js/jquery.datetimepicker.js');	
				drupal_add_js(drupal_get_path('module', 'alc').'/js/custom_datepicker.js');
																									
				$form['view_clra_amended_form']['call_applicant'] = array(
											'#prefix' 		=> '<div id="call_applicant_block"><section><label class="input">',
											'#suffix' 		=> '</label></section></div>',
											'#title'		=> 'Date Time',
											'#type'			=> 'textfield',
											'#attributes' 	=> array('placeholder' => 'Enter Date and Time', 'onclick' => 'showdate()'),
											'#id'			=> 'calltime', 
											'#required' 	=> TRUE,
											);																						
			}
						
			/**** For CALL APPLICANT SECTION ENDS*******/		
		
			if($value_remark_type== 'I'){			
				if(!empty($amendment_reference_no)){			
					$form['view_clra_amended_form']['download_link'] = array(
											'#prefix' => '<div id="download_link_block"><section>',
											'#markup' => l(t('Download Amended Registration Certificate<img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/pdf.png">'),'generate-clra_amended-certificate/'.encryption_decryption_fun('encrypt',$application_id).'/'.encryption_decryption_fun('encrypt',$amendment_reference_no).'/AMENDED-CERTIFICATE', array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('#pdf-img')))),				
											'#suffix' => '</section></div>',										
											);
																									
																									
					$form['view_clra_amended_form']['amended_clra_signed_certificate'] = array(
											'#title'			 => 'Upload Signed CLRA Amended Registration Certificate :-',
											'#type' 			 => 'managed_file',
											'#upload_validators' => array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
											'#upload_location' 	 => 'public://upload/alc_upload_certificates_clra/',
											'#process' 			 => array('import_my_file_element_process'),
											'#attributes' 		 => array('id' => '','autocomplete' => 'off','class'=>array('')),
											'#prefix'			 => '<div id="clra_amend_signed_certificate_block"><section>',
											'#suffix'			 => '</section></div>',
											'#required' 		 =>  TRUE,
											);	
																									
					$form['view_clra_amended_form']['remarks_text']	= array(
											'#prefix' 	=> '<div id="remarks_text_block"><section><label class="textarea">',
											'#suffix' 	=> '</label></section></div>',
											'#title'	=> 'Remarks',
											'#type'		=> 'textarea',
											'#rows'		=>  2,
											'#required' =>  TRUE,
											);
																									
																									
			  		$form['view_clra_amended_form']['submit'] = array(
											  '#prefix' 	=> '<div id="submit_block">',
											  '#suffix' 	=> '</div>',	
											  '#type' 		=> 'submit',
											  '#attributes' => array('class' => array('button')),
											  '#value' 		=> 'Submit'									  
											  );	
				
				}else{
				
					$form['view_clra_amended_form']['generate_certificate'] = array (
											  '#prefix' 	=> '<div id="generate_certificate_block">',
											  '#suffix' 	=> '</div>',
											  '#type' 		=> 'submit',
											  '#attributes' => array('class' => array('button')),
											  '#value' 		=> 'Generate Amendment Certificate',
											  '#submit'		=> array('generate_amended_reference_number'),
											);
				
				}
			
			}
																									
		
			if($value_remark_type != 'I'){		
																									
				$form['view_clra_amended_form']['remarks_text']	= array(
											'#prefix' 		=> '<div id="remarks_text_block"><section><label class="textarea">',
											'#suffix' 		=> '</label></section></div>',
											'#title'		=> 'Remarks',
											'#type'			=> 'textarea',
											'#rows'			=>  2,
											'#required' 	=>  TRUE,
											);
																										
				$form['view_clra_amended_form']['submit'] = array (
											  '#prefix' 	=> '<div id="submit_block">',
											  '#suffix' 	=> '</div>',	
											  '#type' 		=> 'submit',
											  '#attributes' => array('class' => array('button')),
											  '#value' 		=> 'Submit'														  
											);
			}
		}
																							
																							
		$form['view_clra_amended_form']['markup_data_5'] = array(
												'#prefix'	=> '<section style="padding-top:20px;">',
												'#markup' 	=> get_clra_amend_remark_details($application_id,$applicant_user_id),
												'#suffix' 	=> '</section></div></div>',
												);
		
	}else{		
		$message = 'You are not permitted to view this application.';
		drupal_set_message(t($message), 'error');		
	}
	
	return $form;

}


function view_clra_amended_form_func_alc($application_id,$applicant_user_id,$fees_paid,$fees_payable){
	
	global $base_root, $base_path, $user;
	
	if(!empty($application_id) && $applicant_user_id){
			
		global $base_root, $base_path, $user;
		
		$clra_reg_amendment = db_select('l_clra_registration_master', 'lcrm');
		$clra_reg_amendment	->fields('lcrm', array());
		$clra_reg_amendment->condition('lcrm.user_id', $applicant_user_id);
		$clra_reg_amendment->condition('lcrm.id', $application_id);
		$clra_reg_amendment_result = $clra_reg_amendment->execute();
		
		if($clra_reg_amendment_result->rowCount() > 0){
			
			$content	= 	$clra_reg_amendment_result->fetchAssoc();
			
			if(!empty($content['amendment_parent_id'])){
				$last_amendment_id			= $content['amendment_parent_id'];
			}
		}
		
		
		
		if($content['workmen_if_same_similar_kind_of_work']== 0){
			$output_similar_wrk  = "No";
		}else{
			$output_similar_wrk = "Yes";
		}
		/** Fetch Registration Certificate ****/
		
		if(!empty($content['certificates_fid'])){
			$upload_certificates_id 		= db_select('file_managed','fm');
			$upload_certificates_id			-> fields('fm',array());
			$upload_certificates_id			-> condition('fid',$content['certificates_fid']);
			
			$upload_certificates_file		= $upload_certificates_id-> execute()->fetchAssoc();
						
					
		  if(!empty($upload_certificates_file)){ 
			  $url									= explode('//',$upload_certificates_file['uri']);
			  $upload_certificates_file_url			= $url[1];
			  $upload_certificates_file_name		= $upload_certificates_file['filename'];
		  }
		}

		
		/** Fetch Address from Miscellaneous Module ****/
	
		$establishment_address 	= miscellaneous_clra_pe_location_address($application_id);
		$postal_address 		= miscellaneous_postal_add_pe($application_id);
		$pe_address 			= miscellaneous_clra_pe_address($application_id);
		$manager_address	 	= miscellaneous_manager_pe($application_id);
		
		/*** Fetch Principal Employer Nature of Work ****/
		
		$fetch_nature_data	= db_select('l_clra_reg_nature_of_work', 'lcrnow');
		$fetch_nature_data->leftJoin('contractor_works', 'cw', 'cw.contractor_work_id=lcrnow.e_nature_of_work');
		$fetch_nature_data->fields('cw', array('cont_work_name'));
		$fetch_nature_data->fields('lcrnow', array('e_nature_of_work'));
		$fetch_nature_data->condition('lcrnow.user_id', $applicant_user_id);
		$fetch_nature_data->condition('lcrnow.application_id', $application_id);
		$fetch_nature_data_result = $fetch_nature_data->execute();
		
		$content_2	=	array();
		foreach ($fetch_nature_data_result as $master_part_clra_reg_nature_row) {
			$content_2[]	=	$master_part_clra_reg_nature_row;
		}
		
		$prefix	=	'';
		$result_arr	=	'';
		foreach ($content_2 as  $res_nature_of_wrk){
			
			if($res_nature_of_wrk->e_nature_of_work == 28){
				$result_arr  	.= '';
			}else{		
				$result_arr  	.= $prefix.$res_nature_of_wrk->cont_work_name;
				$prefix  	 	 = ', ';
			}
		}
		
		/***Contractor Details **/
		
		$amendment_clra_cont = db_select('l_contractor_info_master', 'lcim');
		$amendment_clra_cont->fields('lcim', array());
		$amendment_clra_cont->condition('lcim.user_id', trim($applicant_user_id));
		$amendment_clra_cont->condition('lcim.application_id', trim($application_id));
		$amendment_clra_cont->condition('lcim.status', 1);
		$amendment_clra_cont->orderBy('lcim.id', 'DESC');
		
		$amendment_clra_cont_result = $amendment_clra_cont->execute();
		
		$content_3	=	array();
		foreach ($amendment_clra_cont_result as $amendment_clra_cont_result_row) {
			$content_3[]	=	$amendment_clra_cont_result_row;
		}
		
		
		
				
		$amendment_clra_docs = db_select('l_documents_upload', 'ldu');
		$amendment_clra_docs->fields('ldu', array());
		$amendment_clra_docs->condition('ldu.user_id', $applicant_user_id);
		$amendment_clra_docs->condition('ldu.application_id', trim($application_id));
		$amendment_clra_docs_result = $amendment_clra_docs->execute();
		
		$content_docs		= $amendment_clra_docs_result->fetchAssoc();
		
		$master_part_trade_union = db_select('l_clra_reg_trade_union_master', 'lcrtu');
		$master_part_trade_union->fields('lcrtu', array());
		$master_part_trade_union->condition('lcrtu.user_id', $applicant_user_id);
		$master_part_trade_union->condition('lcrtu.application_id', trim($application_id));
		$master_part_trade_union_result = $master_part_trade_union->execute();
		
		$trade_union_result	=	$master_part_trade_union_result->fetchAll();
		
		
		/** Get Amended Fields ****/
		
		$fetch_not_amendment_fields 			= db_select('l_clra_amendment', 'lca');
		$fetch_not_amendment_fields				->fields('lca', array('not_amended_fields','id'));
		$fetch_not_amendment_fields				->condition('lca.application_id', $content['amendment_parent_id']);
		$fetch_not_amendment_fields				->orderBy('lca.id','DESC');
		$fetch_not_amendment_fields				->range(0, 1);
		$fetch_not_amendment_fields_result 		= $fetch_not_amendment_fields->execute()->fetchAssoc();
		
		$verified_fields_arr 					= explode(',', $fetch_not_amendment_fields_result['not_amended_fields']);
		
		
		foreach($verified_fields_arr as $not_amend_field){ 
		
			switch($not_amend_field){
					
				case "e_name":
							$amended_e_name 									= 'Not Applicable' ;
							$amend_e_name_ck 									= "checked='checked'   disabled='disabled'" ;
							break;
					  
				case "e_postal_address":
							$amended_e_postal_address 							= 'Not Applicable' ;
							$amend_e_postal_address_ck 							= "checked='checked'   disabled='disabled'" ;
							break;
					  
				case "pe_details":
							$amended_pe_details 								= 'Not Applicable' ;
							$amend_pe_details_ck 						    	= "checked='checked'   disabled='disabled'" ;
							break;
					  
				case "man_details":
							$amended_man_details 								= 'Not Applicable' ;
							$amend_man_details_ck 								= "checked='checked'   disabled='disabled'" ;
							break;
					  
				case "e_nature_of_work":
							$amended_e_nature_of_work 							= 'Not Applicable' ;
							$amend_e_nature_of_work_ck 							= "checked='checked'   disabled='disabled'" ;
							break;
					  
				case "max_num_wrkmen":
							 $amended_max_num_wrkmen 							= 'Not Applicable' ;
							 $amend_max_num_wrkmen_ck 							= "checked='checked'   disabled='disabled'" ;
							 break;
					  
				case "e_num_of_workmen_per_or_reg":
							$amended_per_or_reg 								= 'Not Applicable' ;
							$amend_e_num_of_workmen_per_or_reg_ck 				= "checked='checked'   disabled='disabled'" ;
							break;
					  
				case "e_num_of_workmen_temp_or_reg":
							$amended_temp_or_reg 								= 'Not Applicable' ;
							$amend_e_num_of_workmen_temp_or_reg_ck 				= "checked='checked'   disabled='disabled'" ;
							break;
					  
				case "workmen_if_same_similar_kind_of_work":
							$amended_similar_kind_of_work 						= 'Not Applicable' ;
							$amend_workmen_if_same_similar_kind_of_work_ck 		= "checked='checked'   disabled='disabled'" ;
							break;
							
				case "e_settlement_award_judgement_min_wage":
							$amended_settlement_award_judgement 				= 'Not Applicable' ;
							$amend_e_settlement_award_judgement_min_wage_ck 	= "checked='checked'   disabled='disabled'" ;
							break;
					  
				case "e_any_day_max_num_of_workmen":
							$amended_e_any_day_max_num_of_workmen 				= 'Not Applicable' ;
							$amend_e_any_day_max_num_of_workmen_ck 				= "checked='checked'   disabled='disabled'" ;
							break;
							
				/*case "add_trade_union":
					  if (!in_array("add_trade_union", $amended_fields_arr))
					  $value .= 'add_trade_union,';
					  break;
					  
				case "add_contractor":
						
					  break;*/
					  
				default:
					 
				}
			
		}
		
		/** Get Remark data ****/
		
		$get_remark = db_select('l_remarks_master', 'lbkrm');
		$get_remark->fields('lbkrm', array('remark_field_text'));
		$get_remark->condition('lbkrm.remark_to', trim($applicant_user_id));
		$get_remark->condition('lbkrm.application_id', trim($application_id));
		$get_remark->condition('lbkrm.remark_by', trim($applicant_user_id), '!=');
		$get_remark->where("((lbkrm.remark_by_role = 4) OR (lbkrm.remark_by_role = 7))");
		$get_remark->orderBy('lbkrm.remark_id','DESC');
		$get_remark->range(0, 1);
		$get_remark_result = $get_remark->execute()->fetchAssoc();
		$remark_field_text_arr = explode(',', $get_remark_result['remark_field_text']);
		
		foreach ($remark_field_text_arr as $fieldname){ 
			if($fieldname == 'e_name'){ $e_name_ck = "checked='checked'";}
			if($fieldname == 'e_postal_address'){ $e_postal_address_ck = "checked='checked'";}	
			if($fieldname == 'pe_details'){ $pe_details_ck = "checked='checked'";}
			if($fieldname == 'man_details'){ $man_details_ck = "checked='checked'";}	
			if($fieldname == 'e_nature_of_work'){ $e_nature_of_work_ck = "checked='checked'";}	
			if($fieldname == 'max_num_wrkmen'){ $max_num_wrkmen_ck = "checked='checked'";}	
			if($fieldname == 'e_any_day_max_num_of_workmen'){ $e_any_day_max_num_of_workmen_ck = "checked='checked'";}	
			if($fieldname == 'e_num_of_workmen_per_or_reg'){ $e_num_of_workmen_per_or_reg_ck = "checked='checked'";}	
			if($fieldname == 'e_num_of_workmen_temp_or_reg'){ $e_num_of_workmen_temp_or_reg_ck = "checked='checked'";}	
			if($fieldname == 'workmen_if_same_similar_kind_of_work'){ $workmen_if_same_similar_kind_of_work_ck = "checked='checked'";}
			if($fieldname == 'con_lab_cat_desig_nom'){ $con_lab_cat_desig_nom_ck = "checked='checked'";}
			if($fieldname == 'con_lab_job_desc'){ $con_lab_job_desc_ck = "checked='checked'";}
			if($fieldname == 'con_lab_wage_rate_other_benefits'){ $con_lab_wage_rate_other_benefits_ck = "checked='checked'";}
			if($fieldname == 'e_settlement_award_judgement_min_wage'){ $e_settlement_award_judgement_min_wage_ck = "checked='checked'";}
			
			if($fieldname == 'trade_license_file'){ $trade_license_file_ck = "checked='checked'";}
			if($fieldname == 'article_of_assoc_file'){ $article_of_assoc_file_ck = "checked='checked'";}
			if($fieldname == 'memorandum_of_cert_file'){ $memorandum_of_cert_file_ck = "checked='checked'";}
			if($fieldname == 'partnership_deed_file'){ $partnership_deed_file_ck = "checked='checked'";}
			if($fieldname == 'factory_license_file'){ $factory_license_file_ck = "checked='checked'";}
			if($fieldname == 'backlog_clra_registration_certificate_file'){ $backlog_clra_registration_certificate_file_ck = "checked='checked'";}
			if($fieldname == 'form_1_clra_signed_pdf_file'){ $form_1_clra_signed_pdf_file_ck = "checked='checked'";}
		}
		
		/* Payment Details Start */
 
 if($content['payment_mode'] == 'G'){	 
	 $govt_pay_query = db_select('l_govt_payment', 'lgp');
	 $govt_pay_query->fields('lgp', array());
	 $govt_pay_query->condition('lgp.application_id', $application_id);
	 
	 $govt_pay_query_result = $govt_pay_query->execute();
	

	if($govt_pay_query_result->rowCount() > 0 ){		
		$govt_paydata = $govt_pay_query_result->fetchAssoc();
				
		$tv_number = 'Not available';
		$challan_number = 'Not available';
		$total_amount = 'Not available';
		$challan_date = 'Not available';
				
		if(!empty($govt_paydata['tv_number'])) $tv_number = $govt_paydata['tv_number'];
		if(!empty($govt_paydata['challan_number'])) $challan_number = $govt_paydata['challan_number'];
		if(!empty($govt_paydata['total_amount'])) $total_amount = $govt_paydata['total_amount'];
		if(!empty($govt_paydata['challan_date'])) $challan_date = $govt_paydata['challan_date'];
	}
	
	 $payment_details = '<u>Government Payment[Head to Head transfer through treasury]</u><br>';
	 $payment_details .= 'Transaction Voucher Number:<span class="color_orange">'.$tv_number.'</span><br>';
	 $payment_details .= 'Challan Number:<span class="color_orange">'.$challan_number.'</span><br>';
	 $payment_details .= 'Challan Date:<span class="color_orange">'.date('dS M Y', strtotime($challan_date)).'</span><br>';
	 // $payment_details .= 'Total amount:<span class="color_orange">'.number_format($total_amount,2).'</span><br>';
	 $payment_details .= 'Challan Details:'.l('<img src='.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png>', '/sites/default/files/upload/challan_upload/'.$govt_paydata['challan_file'], array('html' => TRUE, 'title' => 'Click here to view challan'));
  }else if($content['payment_mode'] == 'O'){
	  $offline_pay_query = db_select('l_offline_payment_info', 'lopi');								
	  $offline_pay_query->fields('lopi', array('grn_number', 'approved_by'));
	  $offline_pay_query->condition('lopi.application_id', $application_id);
	  $offline_pay_query->condition('lopi.act_id', 1);								
	  $offline_pay_query = $offline_pay_query->execute();
	
	  $offline_pay_result = $offline_pay_query->fetchObject();
	  
	  $payment_details = '<u>Manual Payment</u><br>';
	  $payment_details .= 'GRN NO.:<span class="color_orange">'.$offline_pay_result->grn_number.'&nbsp;'.l('Click here view details', 'https://wbfin.wb.nic.in/GRIPS/grn_status.do', array('attributes' => array('title' =>'Click here to view details', 'target' => '_blank'))).'</span><br>';
	  // $payment_details .= 'Total Amount:<span class="color_orange"> &#8377; '.$content['finalfees'].'</span><br>';
	  $payment_details .= 'Challan:<span class="color_orange">No available</span><br>';	 
  }else{		
	 $transaction_details  			= db_select('l_principle_epayments_receive_data', 'lpd');
	 $transaction_details			->leftJoin('l_principle_epayments_data', 'lped', 'lped.identification_no = lpd.transaction_id');
	 $transaction_details			->fields('lped', array('identification_no', 'application_id'));
	 $transaction_details			->fields('lpd', array());
	 $transaction_details			->condition('lped.act_id', '1');
	 $transaction_details			->condition('lped.application_id', $application_id);		
	 $trans_details_result 			= $transaction_details->execute();
	
	 $bank_code = 'Not available';
	 $bankTransactionID = 'Not available';
	 $total_amount = 'Not available';
	 $challan_fid_date = 'Not available';
	 $payment_status = 'Not available';
	 $grn_number = 'Not available';
	 $payment_status = 'Not available';
		
	 if($trans_details_result-> rowCount() > 0 ){			
		$transaction_det	= $trans_details_result->fetchAssoc();
		$bankTransactionID	= $transaction_det['transaction_id'];
		$grn_number			= $transaction_det['challanrefid'];
		$challan_fid_date	= !empty($transaction_det['challanrefid_date']) ? $transaction_det['challanrefid_date'] : '';
		$total_amount		= number_format($transaction_det['challanamount'], 2);
		$bank_code			= $transaction_det['bank_cd'];
		if($transaction_det['banktransactionstatus'] == 'Success') 
			$payment_status = '<span class="color_green">'.$transaction_det['banktransactionstatus'].'</span>';
	    }else{
			$payment_status = '<span class="color_red">'.$transaction_det['banktransactionstatus'].'</span>';
	    }
		
		if(empty($transaction_det['banktransactionstatus']) == 'Success'){ $payment_status = 'Not available';}
	
	 $payment_details = '<u><strong>Grips Payment Details[Online/Counter]</strong></u><br>';
	 $payment_details .= 'IFSC Code:<span class="color_orange">'.$bank_code.'</span><br>';
	 $payment_details .= 'GRN:<span class="color_orange">'.$grn_number.'</span><br>';
	 // $payment_details .= 'Total Amount:<span class="color_orange"> &#8377;'.$total_amount.'</span><br>';
	 $payment_details .= 'Transaction Date:<span class="color_orange">'.$challan_fid_date.'</span><br>';
	 $payment_details .= 'Transaction Status: '.$payment_status;
 } 
 
 /* Payment Details End */ 	
		
		$reg_query = db_select('l_clra_registration_master', 'lcrm');
		$reg_query	->fields('lcrm', array('id'));
		$reg_query->condition('lcrm.user_id', $applicant_user_id);
		// $reg_query->condition('lcrm.id', $application_id);
		$reg_query->condition('lcrm.amendment_parent_id', NULL);
		$reg_result = $reg_query->execute()->fetchAssoc();
		
		
			
			
	
		$output ='';
		$output = '<style type="text/css">
						table, td, th {border: 1px solid #006595;font-size:15px;font-family:Times New Roman;height:40px;margin-top:20px;}
						td{padding-left:13px !important;}
						th {background-color: #008BD1;color: white;}
						td:hover {background-color:#d4e3e5;}
						tr:nth-child(even) {background: #DBE5F0}
						tr:nth-child(odd) {background: #F1F1F1}
						.red-star{color:#FF0000;}
					</style>
					
					<table width="100%" border="0" cellspacing="0" cellpadding="0">
					 <tr>
						<td colspan="3" style="text-align:center">NOTE:- All inputs are provided by Principal Employer <br/><input type="checkbox" checked="checked" /> Amendment not required &nbsp;&nbsp; <input type="checkbox" />&nbsp;&nbsp;Amendment Requested </td>    
					</tr>
					   <tr>
						<td colspan="3" style="text-align:center;"><strong>REGISTRATION NUMBER : '.$content['registration_number'].'<br/>ISSUE DATE : '.date('dS M, Y', strtotime($content['registration_date'])).' </strong><br/><a target="_blank" href="'.$GLOBALS['base_url'].'/alc-visible-applications/'.encryption_decryption_fun('encrypt', $reg_result['id']).'/'.encryption_decryption_fun('encrypt', $content['user_id']).'">View Registration Details</a></td>
					  </tr>
					  <tr>
						<th width="40%" style="text-align:center;font-size:17px; padding-top: 8px;" >Parameters</th>
						<th width="45%" style="text-align:center;font-size:17px; padding-top: 8px;">Inputs</th>
						<th style="text-align:center;font-size:17px; padding-top: 8px;"><strong>Verified ?</strong></th>
					  </tr>
					  <tr>
						<td> 1. Name and location of the establishment </td>
						<td>'.$content['e_name'].'<br/>'.$content['loc_e_name'].'<br/>'.$establishment_address.'</td>
						<td><input type="checkbox" readonly="readonly" id="e_name" class="clra_amend_alc" '.$e_name_ck.$amend_e_name_ck.' />&nbsp;&nbsp;'.$amended_e_name.'</td>
					  </tr>
					  <tr>
						<td> 2. Postal Address of the establishment</td>
						<td>'.$content['e_postal_address'].'<br/>'.$postal_address.'</td>
						<td><input type="checkbox" id="e_postal_address" class="clra_amend_alc"  '.$e_postal_address_ck.$amend_e_postal_address_ck.' />&nbsp;&nbsp;'.$amended_e_postal_address.'</td>
					  </tr>
					   <tr>
						<td> 3. Full name and address of the Principal Employer[furnish father &#769;s name in the  <br/>case of individuals]</td>
						<td>'.$content['full_name_principal_emp'].'<br/>'.$content['address_principal_emp'].'<br/>'.$pe_address.'</td>
						<td><input type="checkbox" id="pe_details" class="clra_amend_alc"  '.$pe_details_ck. $amend_pe_details_ck.' />&nbsp;&nbsp;'.$amended_pe_details.'</td>
					  </tr>
					  <tr>
						<td> 4. Full name and address of the Manager or person responsible for the supervision and control of the establishment</td>
						<td>'.$content['full_name_manager'].'<br/>'.$content['address_manager'].'<br/>'.$manager_address.'</td>
						<td><input type="checkbox" id="man_details" class="clra_amend_alc"  '.$man_details_ck.$amend_man_details_ck.  ' />&nbsp;&nbsp;'.$amended_man_details.'</td>
					  </tr>
					  <tr>
						<td> 5. Nature of work carried on in the establishment</td>
						<td>'.$result_arr.'</td>
						<td><input type="checkbox" id="e_nature_of_work" class="clra_amend_alc" '.$e_nature_of_work_ck. $amend_e_nature_of_work_ck.' />&nbsp;&nbsp;'.$amended_e_nature_of_work.'</td>
					  </tr>
					  <tr>
						<td>(a) Maximum number of workmen employed directly on any day in the establishment</td>
						<td>'.$content['max_num_wrkmen'].'</td>
						<td><input type="checkbox" id="max_num_wrkmen" class="clra_amend_alc"  '.$max_num_wrkmen_ck.$amend_max_num_wrkmen_ck.' />&nbsp;&nbsp;'.$amended_max_num_wrkmen.'</td>
					  </tr>
					  <tr>
						<td>(b) Number of workmen engaged as permanent/regular workmen</td>
						<td>'.$content['e_num_of_workmen_per_or_reg'].'</td>
						<td><input type="checkbox" id="e_num_of_workmen_per_or_reg" class="clra_amend_alc"  '.$e_num_of_workmen_per_or_reg_ck.$amend_e_num_of_workmen_per_or_reg_ck.' />&nbsp;&nbsp;'.$amended_per_or_reg.'</td>
					   </tr>
					   <tr>
						<td>(c) Number of workmen engaged as temporary/regular workmen</td>
						<td>'.$content['e_num_of_workmen_temp_or_reg'].'</td>
						<td><input type="checkbox" id="e_num_of_workmen_temp_or_reg" class="clra_amend_alc"  '.$e_num_of_workmen_temp_or_reg_ck. $amend_e_num_of_workmen_temp_or_reg_ck. ' />&nbsp;&nbsp;'.$amended_temp_or_reg.'</td>
					   </tr>
					   <tr>
						<td>(d) Whether the workmen employed/intended to be employment by the contractor perform the same or similar kind of work as the workmen employed directly  by the Principal Employer (if yes, please give here information as detailed below:)</td>
						<td>'.$output_similar_wrk.'</td>
						<td><input type="checkbox" id="workmen_if_same_similar_kind_of_work" class="clra_amend_alc"  '.$workmen_if_same_similar_kind_of_work_ck.$amend_workmen_if_same_similar_kind_of_work_ck.' />&nbsp;&nbsp;'.$amended_similar_kind_of_work.'</td>
					   </tr>
					   <tr>
						<td>i. a complete job description of the contractor labour </td>
						<td>'.$content['con_lab_job_desc'].'</td>
						<td><input type="checkbox" id="con_lab_job_desc" class="clra_amend_alc"  '.$workmen_if_same_similar_kind_of_work_ck.$amend_workmen_if_same_similar_kind_of_work_ck.' />&nbsp;&nbsp;'.$amended_similar_kind_of_work.'</td>
					   </tr>
					   <tr>
						<td>ii. wage rates and other cash benefits paid/to be paid </td>
						<td>'.$content['con_lab_wage_rate_other_benefits'].'</td>
						<td><input type="checkbox" id="con_lab_wage_rate_other_benefits" class="clra_amend_alc"  '.$workmen_if_same_similar_kind_of_work_ck.$amend_workmen_if_same_similar_kind_of_work_ck.' />&nbsp;&nbsp;'.$amended_similar_kind_of_work.'</td>
					   </tr>
					    <tr>
						<td>iii. category/designation/nomenclature of the job </td>
						<td>'.$content['con_lab_cat_desig_nom'].'</td>
						<td><input type="checkbox" id="con_lab_cat_desig_nom" class="clra_amend_alc"  '.$workmen_if_same_similar_kind_of_work_ck.$amend_workmen_if_same_similar_kind_of_work_ck.' />&nbsp;&nbsp;'.$amended_similar_kind_of_work.'</td>
					   </tr>
					   <tr>
						<td>(f)  Settlement or award or judgement or minimum wages (if any applicable in the establishment) </td>
						<td>'.$content['e_settlement_award_judgement_min_wage'].'</td>
						<td><input type="checkbox" id="e_name" class="e_settlement_award_judgement_min_wage"  '.$e_settlement_award_judgement_min_wage_ck. $amend_e_settlement_award_judgement_min_wage_ck.' />&nbsp;&nbsp;'.$amended_settlement_award_judgement.'</td>
					   </tr>
					   <tr>
						<td> 6. Maximum number of contract labour to be employed on any day through each contractor</td>
						<td>'.$content['e_any_day_max_num_of_workmen'].'</td>
						<td><input type="checkbox" id="e_any_day_max_num_of_workmen" class="clra_amend_alc"  '.$e_any_day_max_num_of_workmen_ck.$amend_e_any_day_max_num_of_workmen_ck.' />&nbsp;&nbsp;'.$amended_e_any_day_max_num_of_workmen.'</td>
					   </tr>
					    <tr>
						<td colspan="3" align="center" style="text-align:center;font-size:17px; padding-top: 8px;"><font color="#3366FF"><strong>Documents Uploaded</strong></font></td>
					  </tr>';
					  
					  if(!empty($content_docs)){ 
			$output .='<tr>
							<td>Trade License</td>';
							
						   if(!empty($content_docs['trade_license_file'])){ 
							$output .=' <td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/trade_license/'.$content_docs['trade_license_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>
							
								<span class="viewfileins" id="trade_license_file">
									<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/zoom.png">
								</span>
								
								<div id="fileins_trade_license_file" title="Trade License" style="display:none;">
									<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/trade_license/'.$content_docs['trade_license_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
								</div>
							</td>
							
							';
						   } else { 
							$output .=' <td>No Document Uploaded</td>';
						  } 
				$output .='<td><input type="checkbox" id="trade_license_file" class="clra_amend_alc"  '.$trade_license_file_ck.' />&nbsp;&nbsp;'.$amended_trade_license_file.'</td>
					  </tr>
			
						<tr>
							<td>Articles of Association and Memorandum of Association/Partnership Deed</td>';
							 if(!empty($content_docs['article_of_assoc_file'])){ 
					
						 		$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/article_of_assoc/'.$content_docs['article_of_assoc_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>
								
								<span class="viewfileins" id="article_of_assoc_file">
									<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/zoom.png">
							    </span>
								
								<div id="fileins_article_of_assoc_file" title="Articles of Association and Memorandum of Association/Partnership Deed" style="display:none;">
										<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/article_of_assoc/'.$content_docs['article_of_assoc_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
							   </div>
								
								</td>';
					 		} else { 
								$output .='<td>No Document Uploaded</td>';
						 	} 
				$output .='<td><input type="checkbox" id="article_of_assoc_file" class="clra_amend_alc"  '.$article_of_assoc_file_ck.' />&nbsp;&nbsp;'.$amended_article_of_assoc_file.'</td>
					  </tr>
							<tr>
								<td>Any other document in support of correctness of the particulars mentioned in the application if required</td>';
								if(!empty($content_docs['memorandum_of_cert_file'])){ 
					
								$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/memorandum_of_cert/'.$content_docs['memorandum_of_cert_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>
								
								<span class="viewfileins" id="memorandum_of_cert_file">
									<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/zoom.png">
								</span>
								
								<div id="fileins_memorandum_of_cert_file" title="Any other document in support of correctness of the particulars mentioned in the application" style="display:none;">
									<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/memorandum_of_cert/'.$content_docs['memorandum_of_cert_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
									</div>
								
								</td>';
								 } else { 
								$output .='<td>No Document Uploaded</td>';
								 } 
				$output .='<td><input type="checkbox" id="memorandum_of_cert_file" class="clra_amend_alc"  '.$memorandum_of_cert_file_ck.' />&nbsp;&nbsp;'.$amended_memorandum_of_cert_file.'</td>
					  </tr>
    
					<tr>
						<td>Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc.</td>';
						
						 if(!empty($content_docs['partnership_deed_file'])){ 
						 
							$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/partnership_deed/'. $content_docs['partnership_deed_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>
							
								<span class="viewfileins" id="partnership_deed_file">
									<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/zoom.png">
								</span>
								<div id="fileins_partnership_deed_file" title="Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc" style="display:none;">
									<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/partnership_deed/'.$content_docs['partnership_deed_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
								</div>
							
							</td>';
						 } else { 
							$output .='<td>No Document Uploaded</td>';
						 } 
      		$output .='<td><input type="checkbox" id="partnership_deed_file" class="clra_amend_alc"  '.$partnership_deed_file_ck.' />&nbsp;&nbsp;'.$amended_partnership_deed_file.'</td>
					  </tr>
				
							<tr>
								<td>Factory License if any</td>';
								
								 if(!empty($content_docs['factory_license_file'])){ 
								 
									$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/FactoryLicense/'. $content_docs['factory_license_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>
									
									<span class="viewfileins" id="factory_license_file">
										<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/zoom.png">
									</span>
									
									<div id="fileins_factory_license_file" title="Factory License" style="display:none;">
										<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/FactoryLicense/'.$content_docs['factory_license_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
									</div>
									
									</td>';
								 } else { 
									$output .='<td>No Document Uploaded</td>';
								 } 
					$output .='<td><input type="checkbox" id="factory_license_file" class="clra_amend_alc"  '.$factory_license_file_ck.' />&nbsp;&nbsp;'.$amended_factory_license_file.'</td>
					  </tr>
					  <tr>
							<td>Form -I </td>';
							if(!empty($content_docs['form_1_clra_signed_pdf_file'])){
								
								$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/FORM-I/'. $content_docs['form_1_clra_signed_pdf_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>
								
									<span class="viewfileins" id="form_1_clra_signed_pdf_file">
										<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/zoom.png">
									</span>
									
									<div id="fileins_form_1_clra_signed_pdf_file" title="Uploaded Form -I" style="display:none;">
										<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/FORM-I/'.$content_docs['form_1_clra_signed_pdf_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
									</div>  
									&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
									'. l(t('Generated FORM - I'),'generate-pdf-applicant/'.encryption_decryption_fun('encrypt', $application_id).'/'.encryption_decryption_fun('encrypt', $applicant_user_id), array('html' => TRUE,'attributes'=> array('target'=>'_blank'))).' 
								
								</td>';
							} else { 
								$output .='<td>No Document Uploaded</td>';
							} 
					$output .='<td><input type="checkbox" id="form_1_clra_signed_pdf_file" class="clra_amend_alc"  '.$form_1_clra_signed_pdf_file_ck.' /></td>
					  </tr>
					  <tr>
							<td>Registration Certificate</td>';
							
							 if(!empty($content['certificates_fid'])){ 
							 
								$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$upload_certificates_file_url.'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>
									<span class="viewfileins" id="certificates_fid_file">
										<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/zoom.png">
									</span>
									
									<div id="fileins_certificates_fid_file" title="Registration Certificate" style="display:none;">
										<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$upload_certificates_file_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
									</div>
								
								</td>';
							 } else { 
								$output .='<td>Under Process</td>';
							 } 
				$output .='<td></td>
					  </tr>';
				
    				 }
					 if($fees_payable > $fees_paid){
						$amount_payable  = '&#8377; '.number_format($fees_payable - $fees_paid, 2);  
					 }else{
						 $amount_payable = 'Not Applicable';
					 }
					 
					 
			$output .='<tr>
						<td>Total Fees</td>
						<td>Total Amount : &#8377;'.number_format($fees_payable, 2).'<br/>Fees Paid : &#8377; '.number_format($fees_paid,2).'<br/> Amount Payable : '.$amount_payable.'<br>'.$payment_details.'<input type="hidden" name="retFees" id="retFees" value="'.$retFees.'" /></td> 
						<td><span class="viewfile_popup view_fees_details" id="fees_details"> VIEW FEES CHART</span><div id="view_fees_details" title="FEES CHART" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/fees_chat/CLRA-FEES-STRUCTURE.pdf" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div></td>   
					  </tr>

					</table>
					
					
					
					<div class="contractor_details" style="padding:12px 0; text-align:center; font-size:15px;"><strong>Trade Union Details</strong></div>
					   <div style="overflow-y:scroll;max-height:392px;">
							<table style="border:1px solid #000;width:885px;">';
							if(!empty($trade_union_result)){
								$x = 0;	
								foreach($trade_union_result as $trade_union_data){ 
									$x++;
									$output .= '<tr>
													<td colspan="2" align="center" style="text-align:center;font-size:17px; padding-top: 8px;">
														<font color="#3366FF"><strong>Trade Union('.$x.')</strong></font>				
													</td>
												</tr>
												<tr>
													<th width="51%" style="text-align:center;font-size:17px; padding-top: 8px;">Parameters</th>
													<th width="49%" style="text-align:center;font-size:17px; padding-top: 8px;">Values</th>
												</tr>
												 <tr>
													<td>Registration of the Trade Union</td>
													<td>'.$trade_union_data->e_trade_union_regn_no.'</td>
												  </tr>
												  <tr>
													<td>Name of the Trade Union</td>
													<td>'.$trade_union_data->e_trade_union_name.'</td>
												  </tr>
												  <tr>
													<td>Address of the Trade Union</td>
													<td>'.$trade_union_data->e_trade_union_address.'</td>
												  </tr>';
								}
						 	}else{ 
								$output .= '<tr><td style="text-align:center" colspan="2"> No Trade Union Added</td></tr>';
						 	} 
					$output .='</table>
			  </div>
			  
			  <div class="contractor_details" style="padding:12px 0; text-align:center; font-size:15px;"><strong>Contractors Details</strong></div>
			  	<div style="overflow-y:scroll;max-height:392px;">
					<table style="border:1px solid #000;width:885px;">';
					
						if(!empty($content_3)){ 
								$i = 0;
								foreach($content_3 as $delta_clra_cont => $datafetch_clra_cont){ 
									$i++;
									
									$cont_work_name = "";
									$prefix = '';
									
									
									$contractor_wrk_name 		= db_select('l_clra_con_nature_of_work', 'lccnw');
									$contractor_wrk_name		->leftJoin('contractor_works', 'cw', 'cw.contractor_work_id=lccnw.contractor_nature_of_work');
									$contractor_wrk_name		->fields('lccnw', array('contractor_nature_of_work'));
									$contractor_wrk_name		->fields('cw', array('cont_work_name'));
									$contractor_wrk_name		->condition('lccnw.user_id', $applicant_user_id);
									$contractor_wrk_name		->condition('lccnw.contractor_id', $datafetch_clra_cont->id);
									$contractor_wrk_name		->condition('lccnw.application_id', $application_id);
									$result_2 					= $contractor_wrk_name->execute()->fetchAll(); 
									
									foreach ($result_2 as  $con_nature_of_wrk){
										
										if($con_nature_of_wrk->contractor_nature_of_work == 28){
											$cont_work_name  	.= 'Others';
										}else{
											$cont_work_name  	.= $prefix.$con_nature_of_wrk->cont_work_name;
											$prefix  	 = ', ';
										}
										
									}
									if  (!empty($datafetch_clra_cont->amendment_id ) &&  $last_amendment_id == $datafetch_clra_cont->amendment_id){
										$add_contractor_amended = '<font color="red"><strong><i>Newly Added in Amendment</i></strong></font>';
									}else{
										$add_contractor_amended='';
									}
									
									
									if ($datafetch_clra_cont->state_opts == 1){
										$table	 				= 'l_contractor_info_master';
										$type_con	 			= 'sub-table';
										$con_Arr	 			= array('con_loc_e_dist','con_loc_e_subdivision','con_loc_e_areatype','con_name_areatype','con_loc_e_vill_ward','con_l_e_ps','contractor_pin','state_opts');
										$contractor_info_address 	= '<br/>'.get_full_address($table, $type_con, $datafetch_clra_cont->id, $con_Arr);
									}else{
										
										$contractor_info_address		='<br/>'.$datafetch_clra_cont->state_others;
									}
									
									
									$output .= '<tr>
													<td colspan="2" align="center" style="text-align:center;font-size:17px; padding-top: 8px;">
														<font color="#3366FF"><strong>CLRA Particulars of Contractors and Contract Labour('.$i.')</strong><br/>'.$add_contractor_amended.'</font>				
													</td>
												</tr>
												<tr>
													<th width="51%" style="text-align:center;font-size:17px; padding-top: 8px;">Parameters</th>
													<th width="49%" style="text-align:center;font-size:17px; padding-top: 8px;">Values</th>
												</tr>
												<tr>
													<td>Name of the Contractor</td>
													<td>'.$datafetch_clra_cont->name_of_contractor.'</td>
												</tr>
												<tr>
													<td>Address of the Contractor</td>
													<td>'.$datafetch_clra_cont->address_of_contractor.$contractor_info_address.'</td>
												</tr>
												<tr>
													<td>Email of Contractor</td>
													<td>'.$datafetch_clra_cont->email_of_contractor.'</td>
												</tr>
												<tr>
													<td>Nature of Work in which Contract Labour is Employed or is to be Employed</td>
													<td>'.$cont_work_name.'</td>
												</tr>
												<tr>
													<td>Maximum Number of Contractor Labour to be Employed on any day Through Each Contractor</td>
													<td>'.$datafetch_clra_cont->contractor_max_no_of_labours_on_any_day.'</td>
												</tr> 
												<tr>
													<td>Estimated Date of Employment of Each Contract Work Under Each Contractor</td>
													<td>'.date('dS M, Y',strtotime($datafetch_clra_cont->est_date_of_work_of_each_labour_from_date)).' to '.date('dS M, Y',strtotime($datafetch_clra_cont->est_date_of_work_of_each_labour_to_date)).'</td>
												</tr> ';
												
									}
						} else{  
				$output .= '<tr>
								<td colspan="2" align="center" style="text-align:center;font-size:10px; padding-top: 8px;">Contractors Details Not Found</td>
							</tr>';
						}
	$output .= '</table>
			    </div>';
					
			
			return $output;
			
	}else{
		drupal_goto('node/3');	
	}
}                                                                                       
	
	

function get_call_date_time_field_amendment($form, $form_state){
	$commands 	= array();
	$commands[] = ajax_command_replace('#call_applicant_block', drupal_render($form['view_clra_amended_form']['call_applicant']));
	$commands[] = ajax_command_replace('#generate_certificate_block', drupal_render($form['view_clra_amended_form']['generate_certificate']));
	$commands[] = ajax_command_replace('#remarks_text_block', drupal_render($form['view_clra_amended_form']['remarks_text']));
	$commands[] = ajax_command_replace('#submit_block', drupal_render($form['view_clra_amended_form']['submit']));
	$commands[] = ajax_command_replace('#download_link_block', drupal_render($form['view_clra_amended_form']['download_link']));
	$commands[] = ajax_command_replace('#clra_amend_signed_certificate_block', drupal_render($form['view_clra_amended_form']['amended_clra_signed_certificate']));
	
	
	return array('#type' => 'ajax', '#commands' => $commands);
	
}

function get_clra_amend_remark_details($application_id,$applicant_user_id){
	
	global $base_root, $base_path, $user;
	
	$user_id = $user->uid;
	
	$counter					= 0;
	
	$header = array(
		array('data' => 'Sl. No', 'field' => 'slno'),
		array('data' => 'Date - Time', 'field' => 'date'),
		array('data' => 'Remark', 'field' => 'remrk'),
		array('data' => 'Remark Status', 'field' => 'remark type'),
		array('data' => 'Remark By', 'field' => 'licenseno'),
	);
	
	$get_remark = db_select('l_remarks_master', 'lrm');
	$get_remark	->leftJoin('role', 'ro', 'ro.rid = lrm.remark_by_role');
	$get_remark->fields('lrm', array());
	$get_remark->fields('ro', array('name'));
	$get_remark->condition('lrm.remark_to', trim($applicant_user_id));
	$get_remark->condition('lrm.application_id', trim($application_id));
	//$get_remark->condition('lrm.remark_by', trim($applicant_user_id), '!=');
	$get_remark->orderBy('lrm.remark_id','DESC');
	$get_remark_result = $get_remark->execute()->fetchAll(); 
	
	
	foreach($get_remark_result as $row){ 
			
			$counter 		= $counter+1;
			$remark_by		= $row->fullname.'('.$row->name.')';
			
			if(trim($row->remark_type== 'I' )){
				$STATUS_IMG = '<span class="issued">Issued</span>';		
			}else if(trim($row->remark_type) == 'B'){ 
				$STATUS_IMG =  '<span class="pending">Backed</span>';
			}else if(trim($row->remark_type) == 'V'){ 
				$STATUS_IMG =  '<span class="feespending">Fees Pending</span>';
			}else if(trim($row->remark_type) == 'BI'){
				$STATUS_IMG =  '<span class="backtoins">Back to Inspector</span>'; 
       		 }else if(trim($row->remark_type) == 'C'){ 
				$STATUS_IMG = '<span class="call_applicant">Call Applicant</span>';
			}else if(trim($row->remark_type) == 'R'){ 
				$STATUS_IMG =  '<span class="reject">Rejected</span>';
			}else if(trim($row->remark_type) == 'F'){ 
				$STATUS_IMG =  '<span class="forward">Forwarded</span>'; 
			}else if(trim($row->remark_type) == '0'){ 
				$STATUS_IMG =  '<span class="applied">Applied</span>'; 
			}else if(trim($row->remark_type) == 'T'){ 
				$STATUS_IMG =  '<span class="feespaid">Fees Paid</span>'; 
			}else if(trim($row->remark_type) == 'S'){ 
				$STATUS_IMG =  '<span class="finalsubmit">Final Submit</span>'; 
			}else if(trim($row->remark_type) == 'VA'){ 
				$STATUS_IMG =  '<span class="approved">Approved</span>'; 
			}else if(trim($row->remark_type) == 'U'){ 
				$STATUS_IMG =  '<span class="pending">FORM-I Backed</span>'; 
			}
			
			if (!empty($row->call_time_applicant)){
				$call_time_applicant =  ' ( '.date('dS M, Y - g:i a',$row->call_time_applicant).' )';
			}else{
				$call_time_applicant ='';
			}
			
			
			
				$rows[] 	= array(
				
					array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
					array('data' => date('dS M, Y - g:i a', $row->remark_date), 'align' => 'left', 'class' => array('odd')),
					array('data' => $row->remarks.$call_time_applicant, 'align' => 'left', 'class' => array('odd')),
					array('data' => $STATUS_IMG, 'align' => 'left', 'class' => array('odd')),
					array('data' => $row->remark_by_name.'&nbsp;( ' .$row->name.' )', 'align' => 'left', 'class' => array('odd')),
					
							
			  );
			
			
			
		}
		
	$variables = array(
	  		'attributes' 		=> array('class' => array('clra_applications')), 
	  		'header' 			=> $header,
	  		'rows'				=> $rows,
			'empty' 			=> t("No data found!")
	  );
	  
	$output = theme('datatable', $variables);
	  
	/*$output.=   '<style type="text/css">
				.div-head{font-weight:bold;font-size:16px;text-align:center;font-family:Times New Roman;padding:10px; margin-top: 50px;width: 100%;}
				</style>
					
				<h3 class="div-head"><u>STATUS AND ITS DESCRIPTIONS</u></h3>	
				<table style="border:1px solid #000; width: 100%;">
					<tr style="background: #fff;">
						<td style="padding-top: 15px;font-size:12px; text-align:center;"><span class="finalsubmit" style="margin-right: 10px;">Final Submit</span>Final Submit By Applicant</td>
						<td style="padding-top: 15px;font-size:12px;text-align:center;"><span class="pending" style="margin-right: 10px;">Backed</span> Back for correction</td>
						<td style="padding-top: 15px;font-size:12px;text-align:center;"><span class="forward" style="margin-right: 10px;">Forwarded</span>  Forward to ALC by Inspector</td>
					</tr>
					<tr style="background: #fff;">
						<td style="padding-top: 15px;font-size:12px; text-align:center;"><span class="backtoins" style="margin-right: 10px;">Back to Inspector</span> Back to Inspector</td>
						<td style="padding-top: 15px;font-size:12px; text-align:center;"><span class="feespending" style="margin-right: 10px;">Fees Pending</span> Fees Pending</td>
						<td style="padding-top: 15px;font-size:12px; text-align:center;"><span class="call_applicant" style="margin-right: 10px;">Call Applicant</span> Call applicant to ALC office</td>
					</tr>
					<tr style="background:#fff;">
						<td style="padding-top: 15px;font-size:12px; text-align:center;"><span class="feespaid" style="margin-right: 10px;">Fees Paid</span>Transaction Successful</td>
						<td style="padding-top: 15px;font-size:12px; text-align:center;"><span class="issued" style="margin-right: 10px;">Issued</span> Issued Registration Certificate</td>
						<td style="padding-top: 15px;font-size:12px; text-align:center;"><span class="reject" style="margin-right: 10px;">Reject</span>  Rejected Application</td>
					</tr>
				</table>';*/
	 
		 
	  
   return $output;	
	
}

function view_alc_clra_amended_applications_submit($form,&$form_state){
	
	global $base_root, $base_path, $user;
	
	$alc_user_id 		= $user->uid;
	$total_fieldname	= '';
	
	$val 				= $form_state['values'];
	
	$applicant_user_id 	= encryption_decryption_fun('decrypt',$val['applicant_user_id']);
	$application_id 	= encryption_decryption_fun('decrypt',$val['application_id']);
	$amendment_id		= encryption_decryption_fun('decrypt',$val['amendment_id']);
	$fieldname			= $val['fieldname'];
	$remark_type		= $val['remark_type'];
	$remarks_text		= $val['remarks_text'];
	
	
	/* Upload For CLRA Amended Signed Certificate*/
	
	$arr_1									= array();
	$signed_form_clra_amendment 		    = file_load($val['amended_clra_signed_certificate']);
	if( $signed_form_clra_amendment != "" ){
		$arr_1 = explode("/", $signed_form_clra_amendment->uri);
		$signed_form_clra_amendment->status = FILE_STATUS_PERMANENT;
		file_save($signed_form_clra_amendment);
		$signed_clra_amendment_file=$signed_form_clra_amendment->fid;
	}
	
	
	
	/** Get Not Amended Fields & Verified Fields ****/
		
	$fetch_amendment_fields 		= db_select('l_clra_amendment', 'lca');
	$fetch_amendment_fields			->fields('lca', array('not_amended_fields'));
	$fetch_amendment_fields			->condition('lca.application_id', $amendment_id);
	$fetch_amendment_fields			->orderBy('lca.id','DESC');
	$fetch_amendment_fields			->range(0, 1);
	$verified_amendment_fields_result 	= $fetch_amendment_fields->execute()->fetchAssoc();
	
	
	/** Get Remark Data ****/
	
	$get_remark = db_select('l_remarks_master', 'lrm');
	$get_remark->fields('lrm', array('remark_field_text','remark_type','call_time_applicant'));
	$get_remark->condition('lrm.remark_to', trim($applicant_user_id));
	$get_remark->condition('lrm.application_id', trim($application_id));
	$get_remark->condition('lrm.remark_by', trim($applicant_user_id), '!=');
	$get_remark->condition('lrm.remark_type', 'B');
	$get_remark->orderBy('lrm.remark_id','DESC');
	$get_remark->range(0, 1);
	$get_remark_result = $get_remark->execute()->fetchAssoc();
			

	if(!empty($get_remark_result) ){
		
		$total_fieldname = $fieldname;
	}else{
		
		$total_fieldname = $verified_amendment_fields_result['not_amended_fields'].$fieldname;
	}
	
	
	
	/*** Get User role of ALC **/
	
	$fetch_users_role		= db_select('users_roles', 'ur');
	$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role->leftJoin('l_custom_user_detail', 'lcud', 'ur.uid=lcud.usr_id');
	$fetch_users_role->fields('ro', array('rid'));
	$fetch_users_role->fields('lcud', array('fullname'));
	$fetch_users_role->condition('ur.uid', $alc_user_id);
	$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc();

	$remark_by_role			=	$fetch_users_role_data['rid'];
	$alc_full_name			=   $fetch_users_role_data['fullname'];
	
	
	
	/*** Update Status of CLRA Master Table ***/ 
	
	$update_status  =  db_update('l_clra_registration_master');
	
	if(!empty($signed_clra_amendment_file)){
		
		$frm_v_gen 					=  	db_update('l_contractor_info_master');
		$frm_v_gen					->	fields(array('is_from_v'  => 1));
		$frm_v_gen					->	condition('application_id',$application_id);
		$frm_v_gen					->	condition('user_id',$applicant_user_id);
		$frm_v_gen					->	execute();
		
		
		$update_status->fields(array(
				 'status'						  => $remark_type,
				 'certificates_fid' 			  => $signed_clra_amendment_file,
		));
		
		
		
		
		
		
	} else{
		
		$update_status->fields(array('status'	  => $remark_type));
	}
	
	$update_status->condition('id',$application_id);
	$update_status->condition('user_id',$applicant_user_id);
	$update_status->execute();
	
	/***Insert In CLRA Remarks Master Table ***/ 
	
	$fieldsApplInfo2  =  array(
		'remarks'   			=> $remarks_text, 
		'remark_by'   			=> $alc_user_id, 
		'remark_to' 			=> $applicant_user_id,
		'application_id' 		=> $application_id,
		'remark_date'  			=> time(),
		'remark_type' 			=> $remark_type,
		'remark_by_name'    	=> $alc_full_name,
		'remark_field_text' 	=> $total_fieldname,
		'remark_by_role' 		=> $remark_by_role,
		'call_time_applicant'	=> !empty($call_date_time)? strtotime($call_date_time): 0,
		
	);
	
	db_insert('l_remarks_master')->fields($fieldsApplInfo2)->execute(); 	
		
	$message = 'Remark has been successfully saved.';
	drupal_set_message(t($message));
	
}


function generate_amended_reference_number($form, &$form_state){
	
	 $val  					= $form_state['values']; 
	 $applicant_user_id		= encryption_decryption_fun('decrypt',$val['applicant_user_id']);
	 $amendment_id			= encryption_decryption_fun('decrypt',$val['amendment_id']);
	 $application_id		= encryption_decryption_fun('decrypt',$val['application_id']);
	 
	 
	 /*** GENERATION OF AMENDMENT REFERENCE NUMBER *****/
	 
	 $amendment_reference_no  = 'AMEND-'.$application_id.'-00'.$amendment_id.'-'.$applicant_user_id;
	 
	 
	 /*** Update Status of CLRA Master Table ***/ 
			
	$update_status  					=  db_update('l_clra_registration_master');
	$update_status->fields(array('amendment_reference_no' => $amendment_reference_no,'amendment_date' => date("Y-m-d"),));
	$update_status->condition('id',$application_id);
	$update_status->condition('user_id',$applicant_user_id);
	$update_status->execute();
	
	$message										= drupal_set_message_custom(t('Amended Registration Certificate has been successfully generated. Please Sign and Upload Certificate to complete this process'));//---miscellaneous module

	$form_state['values']['hidden_action_message'] 	= $message;
	
	$form_state['rebuild']							= TRUE;
}
	
	 
	
