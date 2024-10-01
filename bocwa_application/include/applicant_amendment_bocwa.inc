<?php
function amendment_form_step_one_for_bocwa($form, &$form_state){
	
	global $base_root, $base_path, $user;
	
	$user_id 			= $user->uid;
	
	$get_registration_number				=   db_select('l_bocwa_registration_master', 'lca');
	$get_registration_number	   			->  fields('lca', array('registration_number'));
	$get_registration_number	   			->  condition('lca.user_id', $user_id);
	$get_registration_number->orderBy('lca.id','DESC');
	$get_registration_number->range(0, 1);
	$amended_fields_result  =   $get_registration_number->execute()->fetchAssoc();
	$registration_number=$amended_fields_result['registration_number'];
	$form['amenmend_form']['regn_number'] 	= array(
												'#prefix' 			=> '<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">APPLICATION FOR AMENDMENT OF REGISTRATION CERTIFICATE FOR BOCWA REGISTRATION</div><div class="panel-body"><div class="col-md-8"><label class="input">',
												'#suffix' 			=> '</label>',
												'#title'			=> 'Enter your BOCWA registration number:',
												'#type' 			=> 'textfield',
												'#required'			=>	TRUE,
												'#default_value'		=>  $registration_number,
												'#attributes'		=> array('autocomplete' => 'off'),
												'#description' 		=> '<font color="red"><strong>Note :- Enter the registration number of the certificate to be amended.</strong></font>',
											);
	
	$form['amenmend_form']['submit'] 		= array (
												'#prefix' 			=> '', 
												'#suffix' 			=> '</div></div></div></div>',
												'#type' 			=> 'submit',
												'#value' 			=> 'Continue ',
												'#attributes'		=> array('id' => 'submit_oneId' , 'class' => array('btn btn-primary  form-submit'))
											);
															
							


	return $form;
}

function amendment_form_step_one_for_bocwa_validate($form, &$form_state){
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

function amendment_form_step_one_for_bocwa_submit($form, &$form_state){
	global $base_root, $base_path, $user;
	
	$exitrow = db_query("select id,amendment_parent_id,status FROM l_bocwa_registration_master WHERE user_id = '".$user->uid."' order by id desc limit 1")->fetchObject();		
	if($exitrow->status != 'I'){	// Yes
		$id = encryption_decryption_fun('encrypt', $exitrow->amendment_parent_id);

		//drupal_goto('apply-bocwa/'.$id);
		drupal_goto('apply-bocwa/'.$id);
	}else{
	
	$val 					=	$form_state['values'];
	$registration_no		=	trim($val['regn_number']);
	
	if(!empty($registration_no)){		
		$fetchExistingIdenNumber 			= db_select('l_bocwa_registration_master', 'lcrm');
		$fetchExistingIdenNumber			->fields('lcrm', array());
		$fetchExistingIdenNumber			->condition('lcrm.registration_number', trim($registration_no));
		$fetchExistingIdenNumber			->condition('lcrm.user_id', $user->uid);
		$fetchExistingIdenNumber			->condition('lcrm.status', 'I');
		$fetchExistingIdenNumber			->orderBy('lcrm.id','DESC');
		$fetchExistingIdenNumber			->range(0, 1);
		$fetchExistingIdenNumber_res	=	$fetchExistingIdenNumber->execute();
		$fetchExistingIdenNumber_data 	= 	$fetchExistingIdenNumber_res->fetchObject();
		
		if( $fetchExistingIdenNumber_res->rowCount() > 0 ){	// Yes					
			$edit_id = 	$fetchExistingIdenNumber_data->id;
			$edit_id = encryption_decryption_fun('encrypt', $edit_id);			
			drupal_goto('apply-bocwa/'.$edit_id);			
		}else{	// No					
			$get_bklg_tbl =	db_select('l_bocwa_principle_emp_backlog_data', 'lcpebd');
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
					drupal_goto('apply-bocwa/'.$id);									
				}
				
			}else{
				form_set_error('', t('Registration number does not exist. Please enter valid registration number.'));
			}	
		}	
	}
	}
}


function amendment_form_step_two_bocwa($form, &$form_state, $id=''){
	
	$application_id										=  encryption_decryption_fun('decrypt', $id);
	
	$get_app_location_details 	= db_select('l_bocwa_registration_master', 'lcrm');
	$get_app_location_details->fields('lcrm', array('loc_e_subdivision', 'registration_number', 'registration_date','loc_e_areatype_code'));
	$get_app_location_details->condition('lcrm.id', $application_id);
	$get_app_location_details_result = $get_app_location_details->execute()->fetchAssoc(); 
	
	$applicant_subdivision_code		 = $get_app_location_details_result['loc_e_subdivision'];
	$applicant_block_code			 = $get_app_location_details_result['loc_e_areatype_code'];
	
	$get_amended_fields				=   db_select('l_bocwa_amendment', 'lca');
	$get_amended_fields	   			->  fields('lca', array('amendment_fields'));
	$get_amended_fields	   			->  condition('lca.application_id', $application_id);
	$get_amended_fields->condition('lca.status', 1);
	$get_amended_fields->orderBy('lca.id','DESC');
	$get_amended_fields->range(0, 1);
	$amended_fields_result  =   $get_amended_fields->execute()->fetchAssoc();
	
	$field_arr 	= explode(',', $amended_fields_result['amendment_fields']);
		
	
	foreach($field_arr as $field_arr_val){ 
		if($field_arr_val=='e_name') $readonly_e_name 	=  array('checked'=> 'checked'); 
		if($field_arr_val=='postal_address') $readonly_postal_address 	=  array('checked'=> 'checked');
		if($field_arr_val=='e_permanent_address') $readonly_e_permanent_address 	=  array('checked'=> 'checked');
		if($field_arr_val=='emp_info') $readonly_emp_info 	=  array('checked'=> 'checked');
		if($field_arr_val=='manager_info') $readonly_manager_info 	=  array('checked'=> 'checked');
		if($field_arr_val=='e_nature_of_work') $readonly_e_nature_of_work 	=  array('checked'=> 'checked');
		if($field_arr_val=='max_num_of_workmen') $readonly_max_num_of_workmen 	=  array('checked'=> 'checked');
		if($field_arr_val=='est_date_comm') $readonly_est_date_comm 	=  array('checked'=> 'checked');
		if($field_arr_val=='est_date_completion') $readonly_est_date_completion 	=  array('checked'=> 'checked'); 
		
		if($field_arr_val=='trade_license_file') $readonly_trade_license_file 	=  array('checked'=> 'checked');
		if($field_arr_val=='article_of_assoc_file') $readonly_article_of_assoc_file 	=  array('checked'=> 'checked'); 
		if($field_arr_val=='memorandum_of_cert_file') $readonly_memorandum_of_cert_file 	=  array('checked'=> 'checked');
		if($field_arr_val=='partnership_deed_file') $readonly_partnership_deed_file 	=  array('checked'=> 'checked');  
		if($field_arr_val=='challan_file') $readonly_challan_file 	=  array('checked'=> 'checked');  
		if($field_arr_val=='work_order_file') $readonly_work_order_file 	=  array('checked'=> 'checked');
		if($field_arr_val=='form_one_asses_ses_file') $readonly_form_one_asses_ses_file 	=  array('checked'=> 'checked');
		if($field_arr_val=='supp_asses_ses_file') $readonly_supp_asses_ses_file 	=  array('checked'=> 'checked'); 
		if($field_arr_val=='other_doc_file') $readonly_other_doc_file 	=  array('checked'=> 'checked');
		if($field_arr_val=='address_proof_file') $readonly_address_proof_file 	=  array('checked'=> 'checked');
		
		
		
	}
	
	
	$form['amend_form']['header'] 						= array(
															'#markup' 		=> t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">Fields For Amendment in Application of Registration under BOCWA, Act</div><div class="panel-body min-height-0">'),
															'#type' 		=> 'markup'
															);
	
	$form['amend_form']['application_id'] 				= array(
															'#type' 		=> 'hidden',
															'#default_value'=> encryption_decryption_fun('encrypt', $application_id),
															'#attributes'	=> array('readonly' => 'readonly'), 								
															);
															
	$form['amend_form']['applicant_subdivision_code'] 	= array(
															'#type' 		=> 'hidden',
															'#default_value'=> encryption_decryption_fun('encrypt', $applicant_subdivision_code),
															'#attributes'	=> array('readonly' => 'readonly'), 								
															);
																						
	$form['amend_form']['applicant_block_code'] 		= array(
															'#type' 		=> 'hidden',
															'#default_value'=> encryption_decryption_fun('encrypt', $applicant_block_code),
															'#attributes'	=> array('readonly' => 'readonly'), 								
															);
	
		 
	$form['amend_form']['note'] 						= array(
															'#prefix' 		=> '<section><label>',
															'#suffix' 		=> '</label></section>',
															'#markup' 		=> t('Note : Tick the fields which is to be amended by the Employer for Registration Number : 																	'.$get_app_location_details_result['registration_number'].'&nbsp;issued on Date: '.date('dS M, Y', strtotime($get_app_location_details_result['registration_date']))),				
															'#type' 		=> 'markup',														
															);
																						
	$form['amend_form']['e_name'] 						= array(
															/*'#prefix' 		=> '<section class="col-md-12"><label class="checkbox checkbox-primary">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('Name of the Establishment where building or other construction work is to carred on'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> !empty($description) ? $description : '',		
															'#attributes'	=> !empty($readonly_e_name) ? $readonly_e_name : '',*/								
															);
	
	
															


	$form['amend_form']['emp_info']		 				= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkbox checkbox-primary">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('Full name and address of the employer'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> !empty($description) ? $description : '',	
															'#attributes'	=> !empty($readonly_emp_info) ? $readonly_emp_info : '',						
															);

	$form['amend_form']['e_permanent_address'] = array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkbox checkbox-primary">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('Full Name and permanent address of the Establishment'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> !empty($description) ? $description : '',
															'#attributes'	=> !empty($readonly_e_permanent_address) ? $readonly_e_permanent_address : '',							
															);


						
	
															
	$form['amend_form']['postal_address'] 				= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkbox checkbox-primary">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('Registered Office address of the Establishment'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> !empty($description_payable) ? $description_payable : '',
															'#attributes'	=> !empty($readonly_postal_address) ? $readonly_postal_address : '',										
															);
															
	
															
	/*$form['amend_form']['emp_name'] 		= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('Employee Name'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> !empty($description_payable) ? $description_payable : '',
															'#attributes'	=> !empty($readonly_emp_name) ? $readonly_emp_name : '',										
															);
															
	$form['amend_form']['emp_address'] 		= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('Employee Address'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> !empty($description_payable) ? $description_payable : '',
															'#attributes'	=> !empty($readonly_emp_address) ? $readonly_emp_address : '',										
															);
															*/

															
	$form['amend_form']['manager_info'] 		= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkbox checkbox-primary">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('Full name and address of the Manager or Person responsible for the supervision  and control of the Establishment'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> !empty($description_payable) ? $description_payable : '',
															'#attributes'	=> !empty($readonly_manager_info) ? $readonly_manager_info : '',										
															);
															
	/*$form['amend_form']['address_manager'] 		= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('Manager Address'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> !empty($description_payable) ? $description_payable : '',
															'#attributes'	=> !empty($readonly_address_manager) ? $readonly_address_manager : '',										
															);*/
															
	
															
	$form['amend_form']['e_nature_of_work'] 		= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkbox checkbox-primary">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('Nature of building or other construction work is to be carried on'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> !empty($description_payable) ? $description_payable : '',
															'#attributes'	=> !empty($readonly_e_nature_of_work) ? $readonly_e_nature_of_work : '',										
															);
															
	$form['amend_form']['max_num_of_workmen'] 		= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkbox checkbox-primary">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('Maximum number of building workers to be employed on any day'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> !empty($description_payable) ? $description_payable : '',
															'#attributes'	=> !empty($readonly_max_num_of_workmen) ? $readonly_max_num_of_workmen : '',										
															);
															
	$form['amend_form']['est_date_comm'] 		= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkbox checkbox-primary">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('Estimated date of commencement and completion of building or other construction work'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> !empty($description_payable) ? $description_payable : '',
															'#attributes'	=> !empty($readonly_est_date_comm) ? $readonly_est_date_comm : '',										
															);
															
	/*$form['amend_form']['est_date_completion'] 		= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkbox checkbox-primary">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('Estimated date of the completion of building or other construction work'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> !empty($description_payable) ? $description_payable : '',
															'#attributes'	=> !empty($readonly_est_date_completion) ? $readonly_est_date_completion : '',										
															);*/
															

						
	$form['amend_form']['submit'] 						= array (
															'#prefix'		=> '<div class="col-md-12"><div class="row">',
															'#suffix'		=> '</div></div></div></div></div>', 
															'#type' 		=> 'submit',
															'#value' 		=> 'Apply',
															'#attributes'	=> array('id' => 'submit_oneId' , 'class' => array('btn btn-primary pull-right form-submit'))
														);
	return $form;
}

function amendment_form_step_two_bocwa_submit($form, &$form_state){
	
	global $user;
	
	$exitrow = db_query("select id, status FROM l_bocwa_registration_master WHERE user_id = '".$user->uid."' order by id desc limit 1")->fetchObject();		
	
	if($exitrow->status != 'I'){	// Yes
		$app_id=encryption_decryption_fun('encrypt', $exitrow->id);
		drupal_goto('amendment-bocwa/bocwa-amendment-submit/'.$app_id);
		
	}else{
	
		$user_id					= $user->uid;
		$val 						= $form_state['values'];
		$application_id				= encryption_decryption_fun('decrypt', $val['application_id']);	
		$applicant_subdivision_code	= encryption_decryption_fun('decrypt', $val['applicant_subdivision_code']);
		$applicant_block_code		= encryption_decryption_fun('decrypt', $val['applicant_block_code']);
		
		$lastrow 	= db_query("select id, status FROM l_bocwa_registration_master WHERE user_id = '".$user->uid."' and status='I' order by id desc limit 1")->fetchObject();	
		$app_id		= encryption_decryption_fun('encrypt', $lastrow->id);
		$all_fields	= array('e_name','postal_address','emp_info','e_permanent_address','manager_info','e_nature_of_work','max_num_of_workmen','est_date_comm'/*'trade_license_file','article_of_assoc_file','memorandum_of_cert_file','partnership_deed_file','challan_file','work_order_file','form_one_asses_ses_file','supp_asses_ses_file','other_doc_file','address_proof_file'*/);
		
		$field_name 				= array_keys($val,1);
		$amendment_fields 			= implode(",",$field_name); 
		if(in_array("est_date_comm", $field_name)){
			$amendment_fields 	=   $amendment_fields.',est_date_completion';
		}
		$amended_fields_arr 		= explode(",",$amendment_fields);
		
		$not_amended_field          = '';
	
	
		foreach($all_fields as $get_notamended_fields){
		
			switch($get_notamended_fields){
			
					 case "e_name":
						  if (!in_array("e_name", $amended_fields_arr))
						  $value .= 'e_name,';
						  break;
					case "postal_address":
						  if (!in_array("postal_address", $amended_fields_arr))
						  $value .= 'postal_address,';
						  break;
					case "emp_info":
						  if (!in_array("emp_info", $amended_fields_arr))
						  $value .= 'emp_info,';
						  break;
					case "e_permanent_address":
						  if (!in_array("e_permanent_address", $amended_fields_arr))
						  $value .= 'e_permanent_address,';
						  break;
					case "manager_info":
						  if (!in_array("manager_info", $amended_fields_arr))
						  $value .= 'manager_info,';
						  break;
					case "e_nature_of_work":
						  if (!in_array("e_nature_of_work", $amended_fields_arr))
						  $value .= 'e_nature_of_work,';
						  break;
						  
					case "max_num_of_workmen":
						  if (!in_array("max_num_of_workmen", $amended_fields_arr))
						  $value .= 'max_num_of_workmen,';
						  break;
						  
					case "est_date_comm":
						  if (!in_array("est_date_comm", $amended_fields_arr))
						  $value .= 'est_date_comm,est_date_completion,';
						  break;
						  
					/*case "est_date_comm":
						  if (!in_array("est_date_comm", $amended_fields_arr))
						  $value .= 'est_date_completion,';
						  break;*/
						  
					/*case "est_date_completion":
						  if (!in_array("est_date_completion", $amended_fields_arr))
						  $value .= 'est_date_completion,';
						  break;*/
						  
					/*case "trade_license_file":
						  if (!in_array("trade_license_file", $amended_fields_arr))
						  $value .= 'trade_license_file,';
						  break;
						  
					case "article_of_assoc_file":
						  if (!in_array("article_of_assoc_file", $amended_fields_arr))
						  $value .= 'article_of_assoc_file,';
						  break;
						  
					case "memorandum_of_cert_file":
						  if (!in_array("memorandum_of_cert_file", $amended_fields_arr))
						  $value .= 'memorandum_of_cert_file,';
						  break;
						  
					case "partnership_deed_file":
						  if (!in_array("partnership_deed_file", $amended_fields_arr))
						  $value .= 'partnership_deed_file,';
						  break;
						  
					case "challan_file":
						  if (!in_array("challan_file", $amended_fields_arr))
						  $value .= 'challan_file,';
						  break;
						  
					case "work_order_file":
						  if (!in_array("work_order_file", $amended_fields_arr))
						  $value .= 'work_order_file,';
						  break;
						  
					case "form_one_asses_ses_file":
						  if (!in_array("form_one_asses_ses_file", $amended_fields_arr))
						  $value .= 'form_one_asses_ses_file,';
						  break;
						  
					case "supp_asses_ses_file":
						  if (!in_array("supp_asses_ses_file", $amended_fields_arr))
						  $value .= 'supp_asses_ses_file,';
						  break;
						  
					case "other_doc_file":
						  if (!in_array("other_doc_file", $amended_fields_arr))
						  $value .= 'other_doc_file,';
						  break;
						  
					case "address_proof_file":
						  if (!in_array("address_proof_file", $amended_fields_arr))
						  $value .= 'address_proof_file,';
						  break;
						  */
					default:
						 $value .= '';
				  
			}
		}
		
   		if(count($field_name) == 0){
			
		   $message = "Please select minimum one point.";
		   drupal_set_message($message, 'error');
		   
   		}else{	
		
			$get_amended_fields	= db_select('l_bocwa_amendment', 'lca');
			$get_amended_fields	->fields('lca', array('id','amendment_fields'));
			$get_amended_fields	->condition('lca.application_id', $application_id);
			$get_amended_fields->condition('lca.status', 1);
			$get_amended_fields->orderBy('lca.id','DESC');
			$get_amended_fields->range(0, 1);
			$amended_fields_result  = $get_amended_fields->execute()->fetchAssoc();
			
			
			if(!empty($amended_fields_result)){
				
				$updatedArray 	= array(
										'amendment_fields'				=>  $amendment_fields.',trade_license_file,article_of_assoc_file,memorandum_of_cert_file,partnership_deed_file,challan_file,work_order_file,form_one_asses_ses_file,supp_asses_ses_file,other_doc_file,address_proof_file',
										'applicant_subdivision_code' 	=>  $applicant_subdivision_code,
										'applicant_block_code' 			=>  $applicant_block_code,
										'not_amended_fields' 			=>  $value,
										'amend_apply_date' 				=>  date("Y-m-d"),
										'status' 						=> 	1,
									);
				
				
				$queryForward = db_update('l_bocwa_amendment');
				$queryForward->fields($updatedArray);
				$queryForward->condition('id',$amended_fields_result['id']);
				//$queryForward->condition('applicant_uid',$user_id);
				$queryForward->condition('application_id',$application_id);
				$queryForward->execute();
				
			}else{
				
				$amendment_array	=	array(								
											'application_id' 				=> $application_id,
											'status' 						=> 1,
											'amend_apply_date' 				=> date("Y-m-d"),
											'amendment_fields' 				=> $amendment_fields.',trade_license_file,article_of_assoc_file,memorandum_of_cert_file,partnership_deed_file,challan_file,work_order_file,form_one_asses_ses_file,supp_asses_ses_file,other_doc_file,address_proof_file',	
											'applicant_subdivision_code' 	=> $applicant_subdivision_code,
											'applicant_block_code' 			=> $applicant_block_code,
											'not_amended_fields'			=> $value,
											'applicant_uid'					=> $user_id,
										);
												
				db_insert('l_bocwa_amendment')->fields($amendment_array)->execute();
			}
		
		
		$message =	"You have chosen your fields for amendment successfully.";		
		//drupal_goto('amendment-bocwa/bocwa-amendment-submit/'.$app_id);
		drupal_goto('amendment-bocwa-application/'.$app_id);
			
		drupal_set_message($message);
   }
   
		}	
}

?>