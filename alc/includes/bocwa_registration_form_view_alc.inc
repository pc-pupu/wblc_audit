 <?php
 
drupal_add_js(drupal_get_path('module', 'alc').'/js/mycustom.js');

function view_bocwa_registration_form($form, &$form_state, $id = ''){
	global $base_root, $base_path, $user;
	
	$user_id = $user->uid;
	$application_id	= encryption_decryption_fun('decrypt', $id);
	
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
	
	$get_bocwa_application = db_select('l_bocwa_registration_master', 'lbrm');
	$get_bocwa_application->fields('lbrm',array('loc_e_subdivision','final_submit_status','status','registration_number', 'registration_date', 'bocwa_qr_code','bocwa_registration_certificate','user_id', 'identification_number', 'eodb_app_id', 'finalfees'));
	$get_bocwa_application->condition('lbrm.loc_e_subdivision',$alc_subdivision_code);
	$get_bocwa_application->condition('lbrm.id',$application_id);
	$get_bocwa_application->condition('lbrm.final_submit_status','P');
	$get_bocwa_application_result = $get_bocwa_application->execute();

	
	if($get_bocwa_application_result-> rowCount() > 0 ){
		
		$applicant_data = $get_bocwa_application_result->fetchAssoc();
		$applicant_subdivsion_code  = trim($applicant_data['loc_e_subdivision']);
		$applicant_user_id	 = $applicant_data['user_id'];
		$bocwa_registration_number = $applicant_data['registration_number'];
		$bocwa_qr_code = $applicant_data['bocwa_qr_code'];
		$applicant_final_submit_status = $applicant_data['final_submit_status']; 
		$applicant_status = $applicant_data['status'];
		$uploaded_signed_certificate = $applicant_data['bocwa_registration_certificate'];
		
				 
		if($applicant_status == 'B'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i>CURRENT STATUS: Back for rectification</h4>
								<p>Application is sent back for rectification. After modification by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p>
								</div>';
		}
		if($applicant_status == 'BI'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i>CURRENT STATUS:  Back to inspector for rectification</h4><p>Application is sent back to Inspector for further verification. After verification, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
		}
		if($applicant_status == 'I'){
			$current_status = '<div class="callout callout-success"><h4>CURRENT STATUS: Certificate (FORM-II) is issued. </h4><p>Please check the Registration Certificate after uploading the Certificate.</div>';
		}
		if($applicant_status == 'F'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Forwarded by Inspector to ALC</h4><p>Application is Forwarded to ALC by Inspector for further verification. Any action can be taken for the application.</p></div>';
		}
		if($applicant_status == 'T'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Transaction successful</h4><p>Payment successful for this application. Form-I is not uploaded by the applicant. After submission of signed FORM-I by the applicant, the application can be further accessible.</p></div>';
		}
		if($applicant_status == 'V'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> 
									CURRENT STATUS: Verified and approved for fees submission. </h4><p>Application is approved and directed to pay fees. After fees payment and submission of signed FORM-I by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
		}
		if($applicant_status == 'R'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Rejected application</h4><p>If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
		}
		if($applicant_status == 'VA'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i>CURRENT STATUS: Application has been verified &amp; approved without fees.</h4><p>Application is approved without fees. After submission of signed FORM-I by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
		}
		if($applicant_status == 'S'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS:  FORM-I uploaded & application succesfully submitted</h4><p>FORM-I is submitted by the Applicant. After verification of uploaded FORM-I, Issue of License Certificate can be generated now or back to rectification FORM-I.</p></div>';
		} 
		if($applicant_status == 'U'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> 
									CURRENT STATUS: Back for Rectification(FORM-I)</h4><p>FORM-I is sent back for rectification. After modification by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p>
								</div>';
		} 
		if($applicant_status == '0'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Pending application</h4><p>Application is applied by the Applicant. Any action can be taken for the application.</p></div>';
		} 
		
		/*** Get REMARK Details ****/
	
		$get_remark = db_select('l_bocwa_remark_master', 'lbkrm');
		$get_remark->fields('lbkrm', array('remark_field_title','remark_text'));
		$get_remark->condition('lbkrm.remark_to', trim($applicant_user_id));
		$get_remark->condition('lbkrm.application_id', trim($application_id));
		$get_remark->condition('lbkrm.remark_by', trim($applicant_user_id), '!=');
		$get_remark->condition('lbkrm.is_active', 1);
		$get_remark->orderBy('lbkrm.id','DESC');
		$get_remark->range(0, 1);
		$get_remark_result = $get_remark->execute();
				
		$remark_details = $get_remark_result->fetchAssoc(); 

		$info = check_final_submit_data(encryption_decryption_fun('encrypt', 2), encryption_decryption_fun('encrypt', $application_id));
		
		if(!empty($info) && $info['remark_type']=='S'){
			$final_submit_date = $info['remark_date'];
			$today = time();						
			$datediff = $today - $final_submit_date;	
			$diff = round($datediff / (60 * 60 * 24));	

		}
		
		if($alc_subdivision_code == $applicant_subdivsion_code){   /** Checking for If the Application can be viewed by the ALC of the Subdivision ****/
			
			$form['#attributes'] = array('enctype' 		=> 'multipart/form-data');
	
	
			$form['bocwa_registration']['markup_data'] = array(
						'#markup' => view_bocwa_registration_form_func_alc($application_id,$applicant_user_id),
						'#type' => 'markup',
						);
																			
			$form['bocwa_registration']['base_url'] = array(
					   '#type' => 'hidden',
					   '#value' => $base_root.$base_path,				  
						);
																									
			$form['bocwa_registration']['show_status'] = array(
						'#type' => 'markup',
						'#markup' => $current_status
						);																							
			
			$form['bocwa_registration']['caf_id_no'] = array(
						'#type' => 'hidden',
						'#value' => !empty($applicant_data['identification_number'])?$applicant_data['identification_number']:0,				  
						);
			$form['bocwa_registration']['eodb_app_id'] = array(
						'#type' => 'hidden',
						'#value' => !empty($applicant_data['eodb_app_id'])?$applicant_data['eodb_app_id']:0,				  
						);																						
			$form['bocwa_registration']['finalfees'] = array(
						'#type' => 'hidden',
						'#value' => !empty($applicant_data['finalfees'])?$applicant_data['finalfees']:0,				  
						);
			$form['bocwa_registration']['registration_number'] = array( 
						'#type' => 'hidden',
						'#value' => !empty($applicant_data['registration_number'])?$applicant_data['registration_number']:0,
						);
			$form['bocwa_registration']['registration_date'] = array(
						'#type' => 'hidden',
						'#value' => !empty($applicant_data['registration_date'])?$applicant_data['registration_date']:0,
						);
			$form['bocwa_registration']['bocwa_qr_code'] = array(
						'#type' => 'hidden',
						'#value' => !empty($applicant_data['bocwa_qr_code'])?$applicant_data['bocwa_qr_code']:0,
						);

			if($diff <=30){	 /*** DEEMED **/			
				if ($applicant_status != 'B' && $applicant_status != 'V' && $applicant_status != 'I' && $applicant_status != 'VA' && $applicant_status != 'T' && $applicant_status != 'U' ){
					
					$form['bocwa_registration']['act_id']					= array(
																				'#type'				=> 'hidden',
																				'#default_value'	=> 2,
																				'#attributes' 		=> array('readonly' => 'readonly'),
																				);	
					
					$form['bocwa_registration']['hidden_action_message']	= array(
																				'#prefix'			=> '<div class="box box-default box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">ACTIONS AND REMARKS</h3></div><div class="box-body"><span style="margin:0; width:100%;">',
																				'#suffix'			=> '</span>',
																				'#type' 			=> 'markup',
																				'#markup'			=> isset($form_state['values']['hidden_action_message']) ? $form_state['values']['hidden_action_message'] : ''
																			);
					
					$form['bocwa_registration']['fieldname']				= array(
																				'#title'			=> 'Field Name',
																				'#type'				=> 'hidden',
																				'#attributes'		=> array('id' => 'bocwa_field_name_alc'),
																				'#default_value'	=> !empty($remark_details['remark_field_title']) ? $remark_details['remark_field_title'] : '',
																			);
																											
					$form['bocwa_registration']['applicant_user_id']		= array(
																				'#title'			=> 'User Id',
																				'#type'				=> 'hidden',
																				'#attributes' 		=> array('readonly' => 'readonly'),
																				'#default_value' 	=> !empty($applicant_user_id) ? encryption_decryption_fun('encrypt', $applicant_user_id) : '',
																				);
																											
					$form['bocwa_registration']['application_id']			= array(
																				'#title'			=> 'Application Id',
																				'#type'				=> 'hidden',
																				'#default_value' 	=> !empty($application_id) ? encryption_decryption_fun('encrypt', $application_id) : '',
																				'#attributes' 		=> array('readonly' => 'readonly'),
																				);
																											
				
				
					$form['bocwa_registration']['remark_type']				= array(
																				'#prefix' 			=> '<div class="col-md-3">&nbsp;</div><div class="form-custom col-md-6"><label class="input">',
																				'#suffix' 			=> '<i></i></label>',
																				'#title'			=> 'Please Select Actions',
																				'#type'				=> 'select',
																				'#required' 		=>  TRUE,
																				'#id'				=> 'remark_type_id',
																				'#options'			=> actions_dropdown_by_alc_act($applicant_status,'Y',2),
																				'#attributes'		=> array('onchange' => 'getRemarkTypeAction(this.value)', 'class'=>array('form-control')),
																				'#ajax'				=> array(
																											'event' 		=> 'change',
																											'effect' 		=> 'fade',			
																											'callback'		=> 'get_call_date_time_field',
																											'progress'		=> '',
																										),
																				);
																												
						
																												
					$form['bocwa_registration']['generate_certificate'] 	= array(
																				'#prefix' 		=> '<div id="generate_certificate_block">',
																				'#suffix' 		=> '</div>'
																				);
																									
																									
					$form['bocwa_registration']['download_link'] 			= array(
																				'#prefix' 		=> '<div id="download_link_block">',
																				'#suffix' 		=> '</div>'
																				);
					
					$form['bocwa_registration']['bocwa_signed_certificate'] = array(
																				'#prefix' 		=> '<div id="bocwa_signed_certificate_block">',
																				'#suffix' 		=> '</div>'
																				);
																									
					$form['bocwa_registration']['remarks_text'] 			= array(
																				'#prefix' 		=> '<div id="remarks_text_block">',
																				'#suffix' 		=> '</div>'
																				);
																									
					$form['bocwa_registration']['submit'] 					= array(
																				'#prefix' 		=> '<div id="submit_block">',
																				'#suffix' 		=> '</div>'
																				);
																												
																												
					$value_remark_type 	= isset($form_state['values']['remark_type']) ? $form_state['values']['remark_type'] : '';
						
						
					if($value_remark_type== 'I'){
							
						if(!empty($bocwa_registration_number) && !empty($bocwa_qr_code)){
							

							// $form['bocwa_registration']['download_link']	= array(
							// 													'#prefix'			=> '<div id="download_link_block">',
							// 													'#markup' 			=> l(t('<i class="fa fa-download" aria-hidden="true"></i>&nbsp;Download BOCWA Registration Certificate'),'generate-bocwa-certificate/'.encryption_decryption_fun('encrypt', $bocwa_qr_code), array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn')))),
							// 													'#suffix' 			=> '</div><div class="clear"></div>',
							// 													);
																										
																										
							// $form['bocwa_registration']['bocwa_signed_certificate']	= array(
							// 													'#title'			=> 'Upload Signed BOCWA Registration Certificate :-',
							// 													'#type' 			=> 'managed_file',
							// 													'#upload_validators'=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
							// 													'#upload_location' 	=> 'public://upload/BOCWA_Registration_Certificate/',
							// 													'#process' 			=> array('import_my_file_element_process'),
							// 													'#attributes' 		=> array('id' => '','autocomplete' => 'off','class'=>array('')),
							// 													'#prefix'			=> '<div id="bocwa_signed_certificate_block">',
							// 													'#suffix'			=> '</div>',
							// 													'#required' 		=>  TRUE,
							// 													);	
																										
							// $form['bocwa_registration']['remarks_text']		= array(
							// 													'#prefix' 			=> '<div id="remarks_text_block"><label class="input">',
							// 													'#suffix' 			=> '</label></div>',
							// 													'#title'			=> 'Remarks',
							// 													'#type'				=> 'textarea',
							// 													'#id'				=> 'remarks_text_id',
							// 													'#rows'				=>  2,
							// 													'#required' 		=>  TRUE,
							// 													);
																										
							$form['bocwa_registration']['submit'] 			= array(
																				'#prefix' 		=> '<div id="submit_block">',
																				'#suffix' 		=> '</div></div><div class="col-md-3">&nbsp;</div></div></div>',	
																				'#type' 			=> 'submit',
																				'#attributes' 	=> array('class' => array('btn btn-primary pull-left acremark-btn')),
																				'#value' 			=> 'Submit'
																			
																			);	
							
						}else{
						
							$form['bocwa_registration']['submit']= array (
																				'#prefix' 		=> '<div id="generate_certificate_block">',
																				'#suffix' 		=> '</div>',
																				'#type' 			=> 'submit',
																				'#attributes' 	=> array('class' => array('btn btn-primary pull-left acremark-btn')),
																				'#value' 			=> 'Generate BOCWA Registration Certificate',
																				//'#submit'			=> array('generation_bocwa_registration_number'),
																				);
																										
																										
						}
																												
					}
						
						
					if($value_remark_type!= 'I'){
																													
						$form['bocwa_registration']['remarks_text']			= array(
																				'#prefix' 			=> '<div id="remarks_text_block"><label class="input">',
																				'#suffix' 			=> '</label></div>',
																				'#title'			=> 'Remarks',
																				'#type'				=> 'textarea',
																				'#id'				=> 'remarks_text_id',
																				'#rows'				=>  2,
																				'#required' 		=>  TRUE,
																				);
						
						
						
						$form['bocwa_registration']['submit'] 				= array (
																				'#prefix' 		=> '<div id="submit_block">',
																				'#suffix' 		=> '</div></div><div class="col-md-3">&nbsp;</div></div></div>',
																				'#type' 			=> 'submit',
																				'#attributes' 	=> array('class' => array('btn btn-primary pull-left acremark-btn')),
																				'#value' 			=> 'Submit'
																			
																			);	
																											
					}
				}
			}else{ 
					common_deemed_update_status(encryption_decryption_fun('encrypt', 2),encryption_decryption_fun('encrypt', $application_id),encryption_decryption_fun('encrypt', $user_id));
							
				}
			
			$form['bocwa_registration']['markup_data_1']				= array(	
																			'#prefix'			=>'<div class="box box-default box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">REMARK SUMMARY</h3></div><div class="box-body"><div class="feedback-scroll">',
																			'#markup' 			=> get_bocwa_remark_details($application_id, $applicant_user_id),
																			'#suffix' 			=> '</div></div></div>',
																			);
																									
		  										
		}else{
		
			$message = 'You are not permitted to view this application.';
			drupal_set_message(t($message), 'error');	
			
		}
	}
	
	return $form;
	
}

function view_bocwa_registration_form_func_alc($application_id='' ,$applicant_user_id= ''){
	 
	drupal_add_js(drupal_get_path('module', 'alc').'/js/mycustom.js');
	
	global $base_root, $base_path, $user;
	
	$output = $e_name_ck = $loc_e_name_ck =  $e_postal_address_ck = $e_full_name_ck = $per_address_est_ck = $full_name_manager_ck = $address_manager_ck = $e_nature_of_work_ck = $max_num_of_workmen_ck = $est_date_comm_ck = $est_date_completion_ck = $emp_name_ck = $emp_address_ck = $trade_license_file_ck = $article_of_assoc_file_ck = $memorandum_of_cert_file_ck = $partnership_deed_file_ck = $challan_file_ck = $work_order_file_ck = $form_one_asses_ses_file_ck = $supp_asses_ses_file_ck = $other_doc_file_ck = $bocwa_address_proof_file_ck = $form_1_bocwa_signed_pdf_file_ck ='';
	
	$enc_doc_array = array('TL'=> '','PD'=> '','AOA'=> '','MOA'=> '','MOC'=>'','FL'=> '','AC'=> '','PC'=> '','BB'	=> '','SC'	=> '','IC'	=> '','AP'=> '','ODSC'=> '','CR'=> '','WO'=> '','ORC'=> '','CH'=>'','FI'=>'');

	$pdficonlink 	= '<i class="fa fa-file fa-lg"></i>&nbsp;';
	$query 			= db_select('l_encrypted_uploaded_documents','leud')->fields('leud',array())->condition('status','1')->condition('act_id',2)->condition('application_id',$application_id);
	$query_result 	= $query->execute();
	
	if($query_result->rowCount() > 0){
		foreach($query_result->fetchAll() as $data ){
			
			$enc_doc_array[$data->document_type_code]['content'] = 'view_documents/'.encryption_decryption_fun('encrypt',$data->id);
			
		}
	}
	
	if(is_numeric($applicant_user_id) && is_numeric($application_id) && $application_id!=''){ 
		
			
		global $base_root, $base_path, $user;

		$master_part_bocwa_reg = db_select('l_bocwa_registration_master', 'lbrm');
		$master_part_bocwa_reg->fields('lbrm', array());		
		$master_part_bocwa_reg->condition('lbrm.user_id', $applicant_user_id);
		$master_part_bocwa_reg->condition('lbrm.id', $application_id);
		$master_part_bocwa_reg_result = $master_part_bocwa_reg->execute();
		
		if($master_part_bocwa_reg_result->rowCount() > 0){
			
			$content					= 	$master_part_bocwa_reg_result->fetchAssoc();
			if($content['emp_gender']=='M'){
					$gender = "Male";
			}elseif($content['emp_gender']=='F'){
				$gender = "Female";
			}elseif($content['emp_gender']=='O'){
				$gender = "Transgender";
			}




			$certificateDownloadLink 	= l(t('<i class="fa fa-download" aria-hidden="true"></i>&nbsp; Signed BOCW Registration Certificate'),$GLOBALS['base_url'].'/sites/default/files/upload/BOCWA_Registration_Certificate/'.$content['bocwa_registration_certificate'], array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn'))));
			
			/** Fetch Address from Miscellaneous Module ****/

			$establishment_address 	= $content['loc_e_name'].'<br/>'.get_full_address('l_bocwa_registration_master', 'sub-table', $application_id, array('loc_e_dist','loc_e_subdivision','loc_e_areatype','loc_e_areatype_code','loc_e_vill_ward','loc_e_ps','loc_e_pin_number')); 
			$postal_address = $content['e_postal_address'].'<br/>'.get_full_address('l_bocwa_registration_master', 'sub-table', $application_id, array('e_postal_dist','e_postal_subdivision','e_postal_areatype','e_postal_areatype_code','e_postal_vill_ward','e_postal_ps','e_postal_pin_number')); 
			$permanent_address 		= $content['e_permanent_address'].'<br/>'.get_full_address('l_bocwa_registration_master', 'sub-table', $application_id, array('e_permanent_add_dist','e_permanent_add_subdivision','e_permanent_add_areatype','e_permanent_add_areatype_code','e_permanent_add_vill_ward','e_permanent_add_ps','e_permanent_add_pin_number')); 
			$manager_address	 	= $content['address_manager'].'<br/>'.get_full_address('l_bocwa_registration_master', 'sub-table', $application_id, array('address_manager_dist','address_manager_subdivision','address_manager_areatype','address_manager_areatype_code','address_manager_vill_ward','address_manager_ps','address_manager_pin_number'));
			$emp_address			= $content['emp_address'].'<br/>'.get_full_address('l_bocwa_registration_master', 'sub-table', $application_id, array('emp_dist','emp_subdivision','emp_areatype','emp_areatype_code','emp_vill_ward','emp_ps','emp_pin_number'));
			
			/**** Uploaded Documents ****/
			
			$master_part_bocwa_docs = db_select('l_documents_upload', 'ldu');
			$master_part_bocwa_docs->fields('ldu', array());
			$master_part_bocwa_docs->condition('ldu.user_id', trim($applicant_user_id));
			$master_part_bocwa_docs->condition('ldu.application_id', trim($application_id));
			$master_part_bocwa_docs_result = $master_part_bocwa_docs->execute();
			
			$content_docs	=	$master_part_bocwa_docs_result->fetchAssoc();
			
			$get_remark = db_select('l_bocwa_remark_master', 'lbkrm');
			$get_remark->fields('lbkrm', array('remark_field_title'));
			$get_remark->condition('lbkrm.remark_to', trim($applicant_user_id));
			$get_remark->condition('lbkrm.application_id', trim($application_id));
			$get_remark->condition('lbkrm.remark_by', trim($applicant_user_id), '!=');
			$get_remark->condition('lbkrm.is_active', 1);
			$get_remark->orderBy('lbkrm.id','DESC');
			$get_remark->range(0, 1);
			$get_remark_result = $get_remark->execute();
					
			$content_5 = $get_remark_result->fetchAssoc(); 
			
					
			$remark_field_text_arr = explode(',', $content_5['remark_field_title']);
			
			foreach ($remark_field_text_arr as $fieldname){ 
				if($fieldname == 'e_name'){ $e_name_ck = "checked='checked'";}
				if($fieldname == 'est_type'){ $est_type_ck = "checked='checked'";}
				if($fieldname == 'loc_e_name'){ $loc_e_name_ck = "checked='checked'";}
				if($fieldname == 'e_postal_address'){ $e_postal_address_ck = "checked='checked'";}	
				if($fieldname == 'e_full_name'){ $e_full_name_ck = "checked='checked'";}
				if($fieldname == 'per_address_est'){ $per_address_est_ck = "checked='checked'";}	
				if($fieldname == 'full_name_manager'){ $full_name_manager_ck = "checked='checked'";}	
				if($fieldname == 'address_manager'){ $address_manager_ck = "checked='checked'";}	
				if($fieldname == 'e_nature_of_work'){ $e_nature_of_work_ck = "checked='checked'";}	
				if($fieldname == 'max_num_of_workmen'){ $max_num_of_workmen_ck = "checked='checked'";}	
				if($fieldname == 'est_date_comm'){ $est_date_comm_ck = "checked='checked'";}	
				if($fieldname == 'est_date_completion'){ $est_date_completion_ck = "checked='checked'";}
				if($fieldname == 'emp_name'){ $emp_name_ck = "checked='checked'";}
				if($fieldname == 'emp_gender'){ $emp_gender_ck = "checked='checked'";}
				if($fieldname == 'emp_address'){ $emp_address_ck = "checked='checked'";}
				
				if($fieldname == 'trade_license_file'){ $trade_license_file_ck = "checked='checked'";}	
				if($fieldname == 'article_of_assoc_file'){ $article_of_assoc_file_ck = "checked='checked'";}
				if($fieldname == 'memorandum_of_cert_file'){ $memorandum_of_cert_file_ck = "checked='checked'";}	
				if($fieldname == 'partnership_deed_file'){ $partnership_deed_file_ck = "checked='checked'";}
				if($fieldname == 'challan_file'){ $challan_file_ck = "checked='checked'";}
				if($fieldname == 'work_order_file'){ $work_order_file_ck = "checked='checked'";}	
				if($fieldname == 'form_one_asses_ses_file'){ $form_one_asses_ses_file_ck = "checked='checked'";}	
				if($fieldname == 'supp_asses_ses_file'){ $supp_asses_ses_file_ck = "checked='checked'";}
				if($fieldname == 'other_doc_file'){ $other_doc_file_ck = "checked='checked'";}	
				if($fieldname == 'bocwa_address_proof_file'){ $bocwa_address_proof_file_ck = "checked='checked'";}	
				if($fieldname == 'form_1_bocwa_signed_pdf_file') { $form_1_bocwa_signed_pdf_file_ck  = "checked='checked'";}
			}
			
			/* Payment Details Start */
				
			 if($content['payment_mode'] == 'G'){	 
				 $govt_pay_query = db_select('l_govt_payment', 'lgp');
				 $govt_pay_query->fields('lgp', array());
				 $govt_pay_query->condition('lgp.application_id', $content['id']);
				 
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
				 $payment_details .= 'Total amount:<span class="color_orange">'.number_format($total_amount,2).'</span><br>';
				 $payment_details .= 'Challan Details:'.l('<img src='.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png>', '/sites/default/files/upload/challan_upload/'.$govt_paydata['challan_file'], array('html' => TRUE, 'title' => 'Click here to view challan'));
			  }else{		
				 $transaction_details  			= db_select('l_principle_epayments_receive_data', 'lpd');
				 $transaction_details			->leftJoin('l_principle_epayments_data', 'lped', 'lped.identification_no = lpd.transaction_id');
				 $transaction_details			->fields('lped', array('identification_no', 'application_id'));
				 $transaction_details			->fields('lpd', array());
				 $transaction_details			->condition('lped.act_id', '2');
				 $transaction_details			->condition('lped.application_id', $content['id']);		
				 $trans_details_result 			= $transaction_details->execute();
				
				 $bank_code = 'Not available';
				 $bankTransactionID = 'Not available';
				 $total_amount = 'Not available';
				 $challan_fid_date = 'Not available';
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
				
				 $payment_details = '<b><u> Grips Payment [ Online/Counter ]</u> </b><br/>';
				 $payment_details .= 'GRN : <span class="color_orange">'.$grn_number.'</span><br>';
				 $payment_details .= 'Bank Transaction Id : <span class="color_orange">'.$bankTransactionID.'</span><br>';
				 $payment_details .= 'Total Amount : <span class="color_orange">&#8377;'.$total_amount.'</span><br>';
				 $payment_details .= 'Transaction Date : <span class="color_orange">'.$challan_fid_date.'</span><br>';
				 $payment_details .= 'Transaction Status: '.$payment_status;
			 	 }
			 
				 
			/* Payment Details End */ 
			
			
				$output = '<div class="box box-primary box-solid">
								<div class="box-header with-border">
									  <h3 class="box-title">1. Inputs are provided by Employer [For Verification]1</h3>
								</div>
								<div class="box-body"><div class="feedback-scroll">
									<table class="table table-bordered">
										<tr>
										  <th style="width:10%">Sl</th>
										  <th style="width:40%">Parameters</th>
										  <th style="width:40%">Inputs</th>
										  <th style="width:10%">Verified?</th>
										</tr>
										<tr>
										  <td>1.(a)</td>
										  <td>Name of the Establishment</td>
										  <td>'.$content['e_name'].'</td>
										  <td><input type="checkbox" id="e_name" class="bocwa_verified_alc"  '.$e_name_ck.' /></td>
										</tr>
										<tr>
										  <td>1.(b)</td>
										  <td>Establishment Type</td>
										  <td>'.ucfirst($content['est_type']).'</td>
										  <td><input type="checkbox" id="est_type" class="bocwa_verified_alc"  '.$est_type_ck.' /></td>
										</tr>
										<tr>
										  <td>1.(c)</td>
										  <td>Location ( Work site) of the Establishment </td>
										  <td>'.$establishment_address.' </td>
										  <td><input type="checkbox" id="loc_e_name" class="bocwa_verified_alc" '.$loc_e_name_ck.' /></td>
										</tr>
										<tr>
										  <td>2. </td>
										  <td>Postal Address( Work site ) of the Establishment</td>
										  <td>'.$postal_address.'</td>
										  <td><input type="checkbox" id="e_postal_address" class="bocwa_verified_alc" '.$e_postal_address_ck.' /></td>
										</tr>
										<tr>
										   <td>3.a) </td>
										   <td>Full Name of the Establishment </td>
										   <td>'.$content['e_full_name'].'</td>
										   <td><input type="checkbox" id="full_name_est" class="bocwa_verified_alc"  '.$e_full_name_ck.' /></td>
										</tr>
										<tr>
											<td>3.b) </td>
											<td>Permanent Address Of the Establishment</td>
											<td>'.$permanent_address. '</td>
											<td><input type="checkbox" id="per_address_est" class="bocwa_verified_alc" '.$per_address_est_ck.' /></td>
										</tr>
										<tr>
											<td>4.a) </td>
											<td>Full name of the Manager or Person Responsible for the Supervision and control of the Establishment</td>
											<td>'.$content['full_name_manager'].'</td>
											<td><input type="checkbox" id="full_name_manager" class="bocwa_verified_alc" '.$full_name_manager_ck.' /></td>
										</tr>
										<tr>
											<td>4.b) </td>
											<td>Address of the Manager or Person Responsible for the Supervision  and control of the Establishment </td>
											<td>'.$manager_address.'</td>
											<td><input type="checkbox" id="address_manager" class="bocwa_verified_alc" '.$address_manager_ck.' /></td>
										</tr>
										<tr>
											<td>5.</td>
											<td>Nature of building or other construction work carried/is to be carried on in the Establishment</td>
											<td>'.$content['nature_of_build_const'] .'</td>
											<td><input type="checkbox" id="e_nature_of_work" class="bocwa_verified_alc" '.$e_nature_of_work_ck.'/></td>
										</tr>
										<tr>
											<td>6.</td>
											<td>Maximum Number of building workers to be employed on any day</td>
											<td>'.$content['max_no_of_building_workers_employed'].'&nbsp;&nbsp; <font color="#006600"> <b>[ FEES IS CALCULATED BASED ON THIS VALUE ] </b></font></td>
											<td><input type="checkbox" id="max_num_of_workmen" class="bocwa_verified_alc" '.$max_num_of_workmen_ck.' /></td>
										</tr>
										<tr>
											<td>7.</td>
											<td>Estimated date of commencement of building or other construction work</td>
											<td>'.date('dS M, Y',strtotime($content['est_date_of_commencement_building'])).'</td>
											<td><input type="checkbox" id="est_date_comm" class="bocwa_verified_alc" '.$est_date_comm_ck.' /></td>
										</tr>
										<tr>
											<td>8.</td>
											<td>Estimated date of commencement of building or other construction work</td>
											<td>'.date('dS M, Y',strtotime($content['est_date_of_completion_building'])).'</td>
											<td><input type="checkbox" id="est_date_completion" class="bocwa_verified_alc" '.$est_date_completion_ck.'  /></td>
										</tr>
										<tr>
											<td>9.a)</td>
											<td>Full Name of the Employer</td>
											<td>'.$content['emp_name'].'</td>
											<td><input type="checkbox" id="emp_name" class="bocwa_verified_alc" '.$emp_name_ck.'  /></td>
										</tr>
										<tr>
											<td>9.b)</td>
											<td>Gender</td>
											<td>'.$gender.'</td>
											<td><input type="checkbox" id="emp_gender" class="bocwa_verified_alc" '.$emp_gender_ck.'  /></td>
										</tr>
										<tr>
											<td>9.c)</td>
											<td>Address of the Employer </td>
											<td>'.$emp_address.'</td>
											<td><input type="checkbox" id="emp_address" class="bocwa_verified_alc" '.$emp_address_ck.'  /></td>
										</tr>
										 <tr>
											<td colspan="3" style="font-weight:700; text-transform:uppercase; font-size:15px;">10. Documents Uploaded</td>
										 </tr>
										 <tr>
											<td>i.)</td>
											<td>Trade License</td>';
											if(!empty($content_docs['trade_license_file'])){
														$output .= '<td>
																		<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_trade_license/'.$content_docs['trade_license_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;
																		<span class="viewfile_popup" id="trade_license">
																			<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
																		</span>
																		<div id="view_trade_license"  title="Trade License" style="display:none;">
																			<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_trade_license/'.$content_docs['trade_license_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
																		</div>
																	</td>';
													}elseif($content['eodb_app_id'] != 0){
														
														$output .=  '<td>'.l('View', 'documents-view/'.encryption_decryption_fun('encrypt', $content['user_id']).'/'.encryption_decryption_fun('encrypt', $content['id']).'/'.encryption_decryption_fun('encrypt', 'TL'), array('attributes' => array('target' => '_blank'),'html' => TRUE)).'</td>';
													}else {
														
														$output .=  '<td>No Document Uploaded</td>';
														
													}
													$output .=  '<td><input type="checkbox" id="trade_license_file" class="bocwa_verified_alc" '.$trade_license_file_ck.' /></td>
										</tr>
										<tr>
											<td>ii.)</td>
											<td>Articles of Association and Memorandum of Association/Partnership Deed</td>';
												if(!empty($content_docs['article_of_assoc_file'])){
														$output .= '<td>
																		<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_article_of_assoc/'.$content_docs['article_of_assoc_file'].'">
																		<i class="fa fa-file fa-lg"></i></a>&nbsp;
																		<span class="viewfile_popup" id="article">
																			<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
																		</span>
																		<div id="view_article" title="Articles of Association and Memorandum of Association/Partnership Deed" style="display:none;">
																			<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_article_of_assoc/'.$content_docs['article_of_assoc_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
																		</div>
																	</td>';
													}elseif($content['eodb_app_id'] != 0){
														$output .=  '<td>'.l('View', 'documents-view/'.encryption_decryption_fun('encrypt', $content['user_id']).'/'.encryption_decryption_fun('encrypt', $content['id']).'/'.encryption_decryption_fun('encrypt', 'AOA'), array('attributes' => array('target' => '_blank'), 'html' => TRUE)).'</td>';
													}else {
														
														$output .=  '<td>No Document Uploaded</td>';
														
													}
												$output .=  '<td><input type="checkbox" id="article_of_assoc_file" class="bocwa_verified_alc"'.$article_of_assoc_file_ck.' /></td>
										</tr>
										<tr>
											<td>iii.)</td>
											<td>Any other document in support of correctness of the particulars mentioned in the application if required</td>';
												if(!empty($content_docs['memorandum_of_cert_file'])){
													
													$output .= '<td>
																<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_memorandum_of_cert/'.$content_docs['memorandum_of_cert_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;
																<span class="viewfile_popup" id="support_doc">
																	<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
																</span>
																<div id="view_support_doc"  title="Any other document in support of correctness of the particulars mentioned in the application" style="display:none;">
																	<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_memorandum_of_cert/'.$content_docs['memorandum_of_cert_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
																</div>
															</td>';
												}elseif($content['eodb_app_id'] != 0){
													$output .=  '<td>'.l('View', 'documents-view/'.encryption_decryption_fun('encrypt', $content['user_id']).'/'.encryption_decryption_fun('encrypt', $content['id']).'/'.encryption_decryption_fun('encrypt', 'ODSC'), array('attributes' => array('target' => '_blank'),'html' => TRUE)).'</td>';
												}else {
													
													$output .=  '<td>No Document Uploaded</td>';
													
												}
												$output .=  '<td><input type="checkbox" id="memorandum_of_cert_file" class="bocwa_verified_alc"'.$memorandum_of_cert_file_ck.' /></td>
											</tr>
											<tr>
												<td>iv.)</td>
												<td>Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc.</td>';
													if(!empty($content_docs['partnership_deed_file'])){
														
														$output .= '<td>
																	<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_partnership_deed/'.$content_docs['partnership_deed_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;
																	<span class="viewfile_popup" id="other_certificate">
																		<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
																	</span>
																	<div id="view_other_certificate"  title="Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc." style="display:none;">
																		<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_partnership_deed/'.$content_docs['partnership_deed_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
																	</div>
																</td>';
													}elseif($content['eodb_app_id'] != 0){
														$output .=  '<td>'.l('View', 'documents-view/'.encryption_decryption_fun('encrypt', $content['user_id']).'/'.encryption_decryption_fun('encrypt', $content['id']).'/'.encryption_decryption_fun('encrypt', 'CR'), array('attributes' => array('target' => '_blank'),'html' => TRUE)).'</td>';
													}else {
														
														$output .=  '<td>No Document Uploaded</td>';
														
													}
													$output .=  '<td><input type="checkbox" id="partnership_deed_file" class="bocwa_verified_alc" '.$partnership_deed_file_ck.' /></td>
											</tr>
											<tr>
												<td>v.)</td>
												<td>Challan</td>';
													if(!empty($content_docs['challan_file'])){
														
														$output .= '<td>
																	<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_challan/'.$content_docs['challan_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;
																	<span class="viewfile_popup" id="challan">
																		<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
																	</span>
																	<div id="view_challan"  title="Challan" style="display:none;">
																		<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_challan/'.$content_docs['challan_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
																	</div>
																</td>';
													}elseif($content['eodb_app_id'] != 0){
														// $output .=  '<td>'.l('View', 'documents-view/'.encryption_decryption_fun('encrypt', $content['user_id']).'/'.encryption_decryption_fun('encrypt', $content['id']).'/'.encryption_decryption_fun('encrypt', 'CH'), array('attributes' => array('target' => '_blank'),'html' => TRUE)).'</td>';
														$output .=  '<td>No Document Uploaded</td>';
													}else {
														
														$output .=  '<td>No Document Uploaded</td>';
														
													}
													$output .=  '<td><input type="checkbox" id="challan_file" class="bocwa_verified_alc" '.$challan_file_ck.' /></td>
											</tr>
											<tr>
												<td>vi.)</td>
												<td>Work Order</td>';
													if(!empty($content_docs['work_order_file'])){
														
														$output .= '<td>
																	<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_work_order/'.$content_docs['work_order_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;
																	<span class="viewfile_popup" id="workorder">
																		<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
																	</span>
																	<div id="view_workorder"  title="Work Order" style="display:none;">
																		<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_work_order/'.$content_docs['work_order_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
																	</div>
																</td>';
													}elseif($content['eodb_app_id'] != 0){
														$output .=  '<td>'.l('View', 'documents-view/'.encryption_decryption_fun('encrypt', $content['user_id']).'/'.encryption_decryption_fun('encrypt', $content['id']).'/'.encryption_decryption_fun('encrypt', 'WO'), array('attributes' => array('target' => '_blank'),'html' => TRUE)).'</td>';
													}else {
														
														$output .=  '<td>No Document Uploaded</td>';
														
													}
													$output .=  '<td><input type="checkbox" id="work_order_file" class="bocwa_verified_alc" '.$work_order_file_ck.' /></td>
											</tr>
											<tr>
												<td>vii.)</td>
												<td>FORM-I for assesment of CESS</td>';
													if(!empty($content_docs['form_one_asses_ses_file'])){
														
														$output .= '<td>
																	<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_form_one_asses_of_ses/'.$content_docs['form_one_asses_ses_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;
																	<span class="viewfile_popup" id="form1cess">
																		<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
																	</span>
																	<div id="view_form1cess"  title="FORM-I for assesment of CESS" style="display:none;">
																		<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_form_one_asses_of_ses/'.$content_docs['form_one_asses_ses_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
																	</div>
																</td>';
													}elseif($content['eodb_app_id'] != 0){
														$output .=  '<td>'.l('View', 'documents-view/'.encryption_decryption_fun('encrypt', $content['user_id']).'/'.encryption_decryption_fun('encrypt', $content['id']).'/'.encryption_decryption_fun('encrypt', 'AC'), array('attributes' => array('target' => '_blank'),'html' => TRUE)).'</td>';
													}else {
														
														$output .=  '<td>No Document Uploaded</td>';
														
													}
													$output .=  '<td><input type="checkbox" id="form_one_asses_ses_file" class="bocwa_verified_alc" '.$form_one_asses_ses_file_ck.' /></td>
											</tr>
											<tr>
												<td>viii.)</td>
												<td>Documents in Support of Payment of CESS</td>';
													if(!empty($content_docs['supp_asses_ses_file'])){
														
														$output .= '<td>
																	<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_support_payment_ses/'.$content_docs['supp_asses_ses_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;
																	<span class="viewfile_popup" id="docsupportcess">
																		<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
																	</span>
																	<div id="view_docsupportcess"  title="Documents in Support of Payment of CESS" style="display:none;">
																		<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_support_payment_ses/'.$content_docs['supp_asses_ses_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
																	</div>
																</td>';
													}elseif($content['eodb_app_id'] != 0){
														$output .=  '<td>'.l('View', 'documents-view/'.encryption_decryption_fun('encrypt', $content['user_id']).'/'.encryption_decryption_fun('encrypt', $content['id']).'/'.encryption_decryption_fun('encrypt', 'PC'), array('attributes' => array('target' => '_blank'),'html' => TRUE)).'</td>';
													}else {
														
														$output .=  '<td>No Document Uploaded</td>';
														
													}
													$output .=  '<td><input type="checkbox" id="supp_asses_ses_file" class="bocwa_verified_alc" '.$supp_asses_ses_file_ck.' /></td>
											</tr>
											<tr>
												<td>ix.)</td>
												<td>Documents in Support of Correctness of  Application</td>';
													if(!empty($content_docs['other_doc_file'])){
														
														$output .= '<td>
																	<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_other_docs/'.$content_docs['other_doc_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;
																	<span class="viewfile_popup" id="docsupportcorrectness">
																		<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
																	</span>
																	<div id="view_docsupportcorrectness" title="Documents in Support of Correctness of  Application" style="display:none;">
																		<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_other_docs/'.$content_docs['other_doc_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
																	</div>
																</td>';
													}elseif($content['eodb_app_id'] != 0){
														$output .=  '<td>'.l('View', 'documents-view/'.encryption_decryption_fun('encrypt', $content['user_id']).'/'.encryption_decryption_fun('encrypt', $content['id']).'/'.encryption_decryption_fun('encrypt', 'DSC'), array('attributes' => array('target' => '_blank'),'html' => TRUE)).'</td>';
													}else {
														
														$output .=  '<td>No Document Uploaded</td>';
														
													}
													$output .=  '<td><input type="checkbox" id="other_doc_file" class="bocwa_verified_alc" '.$other_doc_file_ck.' /></td>
											</tr>
											<tr>
												<td>x.)</td>
												<td>Address Proof</td>';
													if(!empty($content_docs['address_proof_file'])){
														
														$output .= '<td>
																	<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_address_proof/'.$content_docs['address_proof_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;
																	<span class="viewfile_popup" id="addressproof">
																		<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
																	</span>
																	<div id="view_addressproof" title="Address Proof" style="display:none;">
																		<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_address_proof/'.$content_docs['address_proof_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
																	</div>
																</td>';
													}elseif($content['eodb_app_id'] != 0){
														$output .=  '<td>'.l('View', 'documents-view/'.encryption_decryption_fun('encrypt', $content['user_id']).'/'.encryption_decryption_fun('encrypt', $content['id']).'/'.encryption_decryption_fun('encrypt', 'AP'), array('attributes' => array('target' => '_blank'),'html' => TRUE)).'</td>';
													}else {
														
														$output .=  '<td>No Document Uploaded</td>';
														
													}
													$output .=  '<td><input type="checkbox" id="bocwa_address_proof_file" class="bocwa_verified_alc" '.$bocwa_address_proof_file_ck.' /></td>
											</tr>
											<tr>
												<td>xi.)</td>
												<td>FORM-I</td>';
												
						if($enc_doc_array['FI']['content'] !=''){
								
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['FI']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'
								&nbsp;&nbsp; | &nbsp; '.l(t('Generated FORM-I&nbsp;<i class="fa fa-info-circle fa-lg" aria-hidden="true"></i> '),'bocwa-generate-pdf/'.encryption_decryption_fun('encrypt', $content['id']).'/'.encryption_decryption_fun('encrypt', $content['user_id']), array('html' => TRUE,'attributes'=> array('target'=>'_blank','style'=>'color:#337ab7'))).'</td>';
								
						}elseif(!empty($content_docs['form_1_bocwa_signed_pdf_file'])){
							  
							 $output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/BOCWA_Form-I/'.$content_docs['form_1_bocwa_signed_pdf_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp; | &nbsp; '.l(t('Generated FORM-I &nbsp; <i class="fa fa-info-circle fa-lg" aria-hidden="true"></i> '),'bocwa-generate-pdf/'.encryption_decryption_fun('encrypt', $content['id']).'/'.encryption_decryption_fun('encrypt', $content['user_id']), array('html' => TRUE,'attributes'=> array('target'=>'_blank','style'=>'color:#337ab7'))).'
											</td>';
						}else{
							
							$output .=  '<td>Signed Form-I will be uploaded by the Applicant after payment of fees</td>';
						}
						
						
												 
											
												 $output .= '<td id="formVId"><input type="checkbox" id="form_1_bocwa_signed_pdf_file" class="bocwa_verified_alc" '.$form_1_bocwa_signed_pdf_file_ck.' /></td>
											</tr>';
											if($content['status']=='I'){
												if($content['deemed'] == 'Y'){
													$issue_by = 'Deemed Approved';
												}else{
													$issue_by = 'Issued By ALC';
												}
								$output .=	'<tr>
												<td>x.)</td>
												<td>Registration Certificate BOCWA ( FORM-II )</td>';
												if(!empty($content['bocwa_registration_certificate'])){
										 
											$output .= '<td>'.$certificateDownloadLink.'&nbsp;&nbsp;&nbsp;<span class="viewfile_popup" id="signed_certificate"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
															<div id="view_signed_certificate"  title="Signed BOCWA Registration Certificate" style="display:none;">
																<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/BOCWA_Registration_Certificate/'.$content['bocwa_registration_certificate'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
															</div>
														</td>';
												}else if($content['bocwa_registration_certificate'] ==''){


													$generateDownloadLink 	= l(t('<i class="fa fa-download" aria-hidden="true"></i>&nbsp;  BOCW Registration Certificate'),$GLOBALS['base_url'].'/generate-bocwa-certificate/'.encryption_decryption_fun('encrypt', $content['bocwa_qr_code']), array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn'))));

													$output .= '<td>'.$generateDownloadLink.'&nbsp;&nbsp;&nbsp;<span class="viewfile_popup" id="signed_certificate"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
													<div id="view_signed_certificate"  title="BOCWA Registration Certificate" style="display:none;">
														<iframe src="'.$GLOBALS['base_url'].'/generate-bocwa-certificate/'.encryption_decryption_fun('encrypt', $content['bocwa_qr_code']).'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
													</div>
												</td>
												<td>'.$issue_by.'</td>';
												}
								$output .= '</tr>';
										}
						$output .= '</table>
									<table class="table table-bordered">
									   <tr>
										<td colspan="3" style="font-weight:700; text-transform:uppercase; font-size:16px;">11. Payment Details</td>
									   </tr>
									   <tr>
										  <td width="10%">11.i.) </td>
										  <td>Registration Fees</td>
										  <td width="50%"> &#8377; ' .$content['finalfees'].'.00  &nbsp;&nbsp;&nbsp;&nbsp;
											<button type="button" class="btn btn-info" data-toggle="modal" data-target="#myModalFeeschart"><i class="fa fa-info-circle"></i>&nbsp;Fees Chart</button>
										  </td>
										</tr>
										<tr>
										  <td>11.ii.) </td>
										  <td>Payment Details</td>
										  <td>'.$payment_details.'</td>
										</tr>
									 </table></div>
								</div>
						  </div>
						
						<div class="modal fade" id="myModalFeeschart" role="dialog">
							<div class="modal-dialog">
								<div class="box box-primary box-solid">
									<div class="box-header">
										<button type="button" class="close" data-dismiss="modal" style="color:#fff;">&times;</button>
										<h3 class="box-title">FEES CHART</h3>
									</div>						
									<div class="modal-body">
										<table cellpadding="0" cellspacing="0" border="0" class="table table-striped table-responsive admin-custom-table">
											 <tr>
												<th>Sl.No.</th>
												<th>#</th>
												<th>Fees</th>
											 </tr>
											 <tr>
												 <td>1</td>
												 <td>Is upto 100 </td>
												 <td>₹500.00</td>
											 </tr>
											 <tr>
												 <td>2</td>
												 <td>Exceeds 100 but does not exceed 500 </td>
												 <td>₹2000.00</td>
											 </tr>
											 <tr>
												 <td>3</td>
												 <td>Exceeds 500</td>
												 <td>₹10000.00</td>
											 </tr>
										</table>
									</div>
									<div class="modal-footer">
										<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
									</div>
								</div>      
							</div>
						</div>';
		}
			
			
	}else{
		
		drupal_set_message(t('Invalid Response'), 'error');
		
	}
	
	$output .= '<div class="row"><ul style="padding:0;"><li class="col-md-2 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-arrow-left"></i>&nbsp;Back to list</div>', 'receivedapplications/'.encryption_decryption_fun('encrypt',$content['act_id']), array('attributes' => array('title' => 'Click here back to applications list'), 'html' => TRUE)).'</li>';
	
	$output .= '<li class="col-md-2 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-user"></i>&nbsp;View Applicant Profile</div>', 'view-applicant-profile/'.encryption_decryption_fun('encrypt', $content['id']).'/'.encryption_decryption_fun('encrypt', $content['user_id']).'/'.encryption_decryption_fun('encrypt', 12), array('attributes' => array('title' => 'Click here view Applicant Profile'), 'html' => TRUE)).'</li>';
	
	$output .= '<li class="col-md-4 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-info-circle"></i>&nbsp;Instructions to give remarks</div>', $base_root.$base_path.'sites/default/files/instructions-remarks.pdf', array('attributes' => array('title' => 'Instructions to give remarks'), 'html' => TRUE)).'</li>';
	
	$output .= '<li class="col-md-4 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-question-circle" aria-hidden="true"></i>&nbsp;How to digitally sign your documents using USB Token</div>', 'digitally-sign-process', array('attributes' => array('title' => 'How to digitally sign your documents using USB Token'), 'html' => TRUE)).'</li></ul></div>';
		
	return $output;
		
}

function view_bocwa_registration_form_submit($form,&$form_state){
	
	global $base_root, $base_path, $user;
	
	
	$val = $form_state['values'];
	$alc_user_id 		= $user->uid;
	$applicant_user_id 	= encryption_decryption_fun('decrypt', $val['applicant_user_id']);
	$application_id 	= encryption_decryption_fun('decrypt', $val['application_id']);
	$fieldname			= $val['fieldname'];
	$remark_type		= $val['remark_type'];
	$remarks_text		= $val['remarks_text'];
	
	
	/* Upload For BOCWA Signed Certificate*/
	
	// $arr_1 = array();
	// $signed_form_bocwa = file_load($val['bocwa_signed_certificate']);
	// if( $signed_form_bocwa != "" ){
	// 	$arr_1 = explode("/", $signed_form_bocwa->uri);
	// 	$signed_form_bocwa->status = FILE_STATUS_PERMANENT;
	// 	file_save($signed_form_bocwa);
	// 	$signed_form_bocwa_file = $arr_1[4];
	// 	$file_contents = file_get_contents($signed_form_bocwa->uri);
	// }
	

	//////  ************ Generation of Registation number Start  *****************   ///////

	/*** Get Applicant Block Code & Statuses & ALC Details ****/
	if($remark_type == 'I'){
				
		$get_bocwa_application							=	db_select('l_bocwa_registration_master', 'lbrm');
		$get_bocwa_application							->  leftJoin('l_customuser_relation_address', 'lcra','lcra.sub_div_code=lbrm.loc_e_subdivision ');
		$get_bocwa_application							->	fields('lbrm',array('loc_e_subdivision','loc_e_areatype_code','registration_number','bocwa_qr_code','identification_number'));
		$get_bocwa_application							->	fields('lcra',array('sub_div_code'));
		$get_bocwa_application							->	condition('lbrm.id',$application_id);
		$get_bocwa_application							->	condition('lbrm.user_id',$applicant_user_id);
		
		$get_bocwa_application_result 					= 	$get_bocwa_application->execute();  
		
		if($get_bocwa_application_result-> rowCount() > 0 ){
			
			$applicant_data 							= 	$get_bocwa_application_result->fetchAssoc(); 
			// print_r($applicant_data);exit;
			$subdivision_code							=   $applicant_data['loc_e_subdivision'];
			$area_code									=   $applicant_data['loc_e_areatype_code'];
			$bocwa_registration_number					=   $applicant_data['registration_number'];
			$bocwa_qr_code								=   $applicant_data['bocwa_qr_code']; 
			$alc_subdiv_code							=   $applicant_data['sub_div_code']; 
			$identificationNo							=   $applicant_data['identification_number'];
			
			
		}
	
		if (empty($bocwa_registration_number) && empty($bocwa_qr_code)){ 
			/*** GENERATION OF QR CODE ****/
			
			 $generated_qr_code					= 'BOCWA-REG'.$application_id.'A2U'.$applicant_user_id.'T'.time();
		
			/*** GENERATION OF BOCWA REGISTRATION NUMBER ***/
			
			$getShortNameSubdivision 			=   custom_user_short_name_fun($subdivision_code);  // ---------custom_user module  
			$getRegistrarCode					=	get_registration_code($area_code);              //-------------miscellaneous module
			$registrarCode 						=   substr($getRegistrarCode, -2);
			
			$reg_query	=	db_query("select max (NULLIF(substr(registration_number,11,6), '') :: integer) as serial_num  from l_bocwa_registration_master where loc_e_subdivision='".$alc_subdiv_code."'");
			$reg_query_result					=	$reg_query->fetchAssoc();
			$reg_code							=	(int)$reg_query_result['serial_num'];
			
			if(empty($reg_code)){ 
				
						$registration_number	=	$getShortNameSubdivision.$registrarCode.'/'.'BCR'.'/'.'000001';

			}else{
					  	$reg_code_next			= 	$reg_code+1;
					  	$reg_first				=	$getShortNameSubdivision.$registrarCode.'/'.'BCR'.'/';
					  	$reg_second				=	str_pad($reg_code_next, 6, "0", STR_PAD_LEFT);
					  	$registration_number	=	$reg_first.$reg_second;
			}
			
         	$registration_date =  date("Y-m-d");
			
			$update_status  =  db_update('l_bocwa_registration_master');
			$update_status->fields(array('bocwa_qr_code' => $generated_qr_code, 'registration_number' =>$registration_number, 'registration_date' => date("Y-m-d")));
			$update_status->condition('id',$application_id);
			$update_status->condition('user_id',$applicant_user_id);
			$update_status->execute();

		}else{
			$bocwa_reg = db_select('l_bocwa_registration_master', 'br');
			$bocwa_reg->fields('br', array('bocwa_qr_code', 'registration_number', 'registration_date','identification_number'));			
			$bocwa_reg->condition('br.user_id', $applicant_user_id);
			$bocwa_reg->condition('br.id', $application_id);
			$bocwa_reg = $bocwa_reg->execute()->fetchObject();
			$registration_number= $bocwa_reg->registration_number; 
			$generated_qr_code 	= $bocwa_reg->bocwa_qr_code;
			$registration_date 	= $bocwa_reg->registration_date;	
			$identificationNo	= $bocwa_reg->identification_number;		
		}
		
		$remarks_text =  'Congratulations! Certificate is issued by the Registering Authority. You can download it from the dashboard.';
	}
	//////  ************ Generation of Registation number End  *****************   ///////


	/*** Get User role of ALC **/
	
	$fetch_users_role		= db_select('users_roles', 'ur');
	$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role->leftJoin('l_custom_user_detail', 'lcud', 'ur.uid=lcud.usr_id');
	$fetch_users_role->fields('ro', array('rid'));
	$fetch_users_role->fields('lcud', array('fullname','employee_id'));
	$fetch_users_role->condition('ur.uid', $alc_user_id);
	$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc();

	$remark_by_role			=	$fetch_users_role_data['rid'];
	$alc_full_name			=   $fetch_users_role_data['fullname'];	
	$employee_id			= 	$fetch_users_role_data['employee_id'];
	
	/******OSW******/
	$serviceResponse 	= 1;
	
	$fetchCommonId = db_select('l_common_application_master', 'cam');
	$fetchCommonId->fields('cam', array('id', 'oswicsapplicationflag', 'ldapplicationflag', 'bocwa_ld_service_id', 'ld_service_unique_id', 'ld_uid', 'ld_est_unique_id','email'));
	$fetchCommonId->condition('cam.user_id', $applicant_user_id);
	// $fetchCommonId->condition('cam.oswicsapplicationflag', 7);
	$commonId = $fetchCommonId->execute()->fetchAssoc();
	
	if($commonId['ldapplicationflag'] == 8888){  // 88 SILPASATHI	
		$serviceResponse = 0;
		if($remark_type == 'B'){
			$status_text = 'Back for Correction';
		}elseif($remark_type == 'V'){
			$status_text = 'Approved';
		}elseif($remark_type == 'I'){
			$status_text = 'Issued';
		}
		$eodb_app_id = $val['eodb_app_id'];
		$caf_id_no = $val['caf_id_no'];		
		if($remark_type == 'B'){
			$curl_post_data = array (
 						'source' => 'WBLC',
						'taskid' => 'STATUSDATA',
						'eodb_app_id' => $val['eodb_app_id'],
						'caf_id_no' => $val['caf_id_no'],								
						'status_code' => $remark_type,
						'status_text' => $status_text,
						'status_date' => date('Y-m-d H:m:s'),
						'application_id' => $application_id,
						'remarks' => $remarks_text					
						);	
		}elseif($remark_type == 'V'){	
			$paymentdetails[] = array(
						'payableamt' => $val['finalfees'],
						'hoa' => '0230-00-101-004-12',
						'purpose' => 'Registration Fees',
						);			
			$curl_post_data = array (
 						'source' => 'WBLC',
						'taskid' => 'PAYMENTINFO',
						'eodb_app_id' => $val['eodb_app_id'],
						'caf_id_no' => $val['caf_id_no'],
						'deptCode' => '050',
						'svcCode' => '301',
						'payabletotalamt' => $val['finalfees'],
						'periodFrom' => date('Y-m-d'),
						'periodTo' => date('Y-m-d'),
						'paymentdetails' => $paymentdetails,			
						'status_code' => $remark_type,
						'status_text' => $status_text,
						'status_date' => date('Y-m-d H:m:s'),
						'application_id' => $application_id,
						'remarks' => $remarks_text					
						);	
		}elseif($remark_type == 'I'){ 
			// echo 899; die;			
			$file_link = 'https://wblc.gov.in/generate-bocwa-certificate/'.encryption_decryption_fun('encrypt', $generated_qr_code);
			$file_contents = file_get_contents($file_link);
			
			$data_file[] = array( 
						'file_id' => $registration_number,
						'file_date' => $registration_date,
						'file_category' => 'CERTIFICATE',
						'file_name' => str_replace('/', '_',$registration_number).'_'.$application_id.'.pdf',
						'file_type' => 'application/pdf',
						'file_content' => base64_encode($file_contents),
						'valid_upto' => '',
						); 
			$additionalParamDtls[] = array(
						'paramName' => 'bocwa_qr_code',
						'paramVal' => $val['bocwa_qr_code']					
						);			
			$curl_post_data = array (
 						'source' => 'WBLC',
						'taskid' => 'STATUSDATA',
						'eodb_app_id' => $val['eodb_app_id'],
						'caf_id_no' => $val['caf_id_no'],									
						'status_code' => $remark_type,
						'status_text' => $status_text,
						'status_date' => date('Y-m-d H:m:s'),
						'status_remarks' => $status_text,
						'remarks' => $remarks_text,		
						'application_id' => $application_id,
						'data_file' => $data_file,
						'additionalParamDtls' => $additionalParamDtls			
						);		  
			
		}	
		
		// echo '<pre>'; print_r($curl_post_data); die;	
		// echo json_encode($curl_post_data); die;
		// $array_enc = encrypt_swold($status_arr);
		// $curl_post_data = array ('message' => $array_enc, 'status_val' => $array_enc); 		
				
		$service_url = "https://silpasathi.wb.gov.in/eservices/deptpostData";
		$ch = curl_init();
		$headers = array();
		$headers[] = 'Accept: application/json';
		$headers[] = 'Content-Type: application/json';
		$headers[] = 'charset=utf-8';
		curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
		curl_setopt($ch, CURLOPT_URL, $service_url);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
		curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 0);
		curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($curl_post_data));
		curl_setopt($ch, CURLOPT_POST, true);
		$results    = curl_exec($ch);
		$errors     = curl_error($ch); //print_r($errors); die;
		curl_close($ch);
		// $curlRes = json_encode($results); 
		$results = json_decode(trim($results), TRUE); // print_r($results); die;
 
	   if($results['code'] == 200){
			$serviceResponse = 1;
			// $res_array = $result['content'];
	   }	  
	}
	if($commonId['ldapplicationflag'] == 1){		
		$serviceResponse = 0;
		
		$status_arr = array (
 						'status' => $remark_type,						
 						'id' => $commonId['bocwa_ld_service_id'],
						'ld_uid' => $commonId['ld_uid'],
						'ld_est_unique_id' => $commonId['ld_est_unique_id'],
						);					
				
		$array_enc = encrypt_swold($status_arr);
		$curl_post_data = array ('message' => $array_enc, 'status_val' => $array_enc); 		
				
		$service_url  =   "https://wblabour.gov.in/restld/establishmentStatusUpdate";
		$ch       =   curl_init();
		$headers    = array();
		$headers[]    = 'Accept: application/json';
		$headers[]    = 'Content-Type: application/json';
		$headers[]    = 'charset=utf-8';
		curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
		curl_setopt($ch, CURLOPT_URL, $service_url);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
		curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 0);
		curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode( $curl_post_data ) );
		curl_setopt($ch, CURLOPT_POST, true);
		$results    = curl_exec($ch);
		$errors     = curl_error($ch);
		curl_close($ch);
		$x = json_encode($results);
		$tmp = json_decode(json_decode(trim($x)), TRUE);
 
	   if($errors){
		//  echo "<br>";
		//  print_r(curl_errno()); 
		//  print_r($errors);
	   }else{
		// echo "<br>";
		// echo "Ok";
		// echo "<br>";
	   }
	   
	   // print_r($results); echo "<br>";
	   $return_msg_encrypted = json_decode($results, TRUE);
	   $return_msg = decrypt_swold($return_msg_encrypted['message']);
	   // print_r($return_msg); echo $return_msg['status']; die;
	   
	   if(trim($return_msg['status']) == 1){
		   $serviceResponse = 1;
	   }
	}
	
	/******OSW******/
	
	if($serviceResponse == 1){	
		/*** Update Status of BOCWA Master Table ***/ 
	
		$update_status  =  db_update('l_bocwa_registration_master');
	
		// if(!empty($signed_form_bocwa_file)){
		// 	$update_status->fields(array(
		// 		 					'status' => $remark_type,
		// 		 					//'bocwa_registration_certificate' => $signed_form_bocwa_file,
		// 							));
		// }else{		
			$update_status->fields(array('status' => $remark_type));
		//}
	
		$update_status->condition('id',$application_id);
		$update_status->condition('user_id',$applicant_user_id);
		$update_status->execute();
	
		/***Insert In BOCWA Remarks Master Table ***/ 
		
		$fieldsApplInfo2  =  array(
			'remark_text'   		=> $remarks_text, 
			'remark_by'   			=> $alc_user_id, 
			'remark_to' 			=> $applicant_user_id,
			'application_id' 		=> $application_id,
			'remark_date'  			=> time(),
			'remark_type' 			=> $remark_type,
			'remark_by_name'    	=> $alc_full_name,
			'remark_field_title' 	=> $fieldname,
			'remark_by_role' 		=> $remark_by_role,
			'call_time_applicant'	=> !empty($call_date_time)? strtotime($call_date_time): 0,
			'emp_id'				=> $employee_id		
			);
	
		db_insert('l_bocwa_remark_master')->fields($fieldsApplInfo2)->execute();
	
		/*********************************************/
		//sms
		/*********************************************/
		
		$applicant_mobile_no = get_mobile_number($applicant_user_id);
		
		// $remark_for = get_user_application_status($remark_type);
		
		if($remark_type == 'I'){
			$template_id = 1107161520461049092;	
			$msg = 'Congratulations! Your BOCWA registration certificate is issued by ALC(Govt. of WB). Login to download the certificate from dashboard (wblc.gov.in). Labour Commissionerate, WB';
		}elseif($remark_type == 'V' || $remark_type == 'VA'){
			$template_id = 1107161520445385323;
			$msg= 'Your BOCWA application is viewed and checked by ALC (Govt. of WB). Please check your dashboard for further process (wblc.gov.in).Labour Commissionerate, WB';
		}elseif($remark_type == 'B' ){
			$template_id = 1107161520445385323;
			$msg = 'Your BOCWA application is sent back for rectification by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in).Labour Commissionerate, WB';
		}elseif($remark_type == 'R' ){
			$template_id = 1107161520445385323;
			$msg = 'Your BOCWA application is rejected by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in). Labour Commissionerate, WB';
		}
		
		send_sms_for_user_alert($applicant_mobile_no, $msg,$template_id);
		
		
		$mailto  = trim($commonId['email']);
		$subject = 'Welcome to Labour Commissionerate, Govt. Of West Bengal';
		send_mail_for_user_alert($mailto, $subject, $msg);	

		if($commonId['ldapplicationflag'] == 8 && $remark_type == 'I'){ 
			db_update('users')->fields(array('status'=>1))->condition('status',0)->condition('uid',$applicant_user_id)->execute(); 
			$msg = 'Congratulations! Your BOCWA registration certificate is issued by ALC(Govt. of WB). Login to download the certificate from dashboard.<br>You may avail other services from <strong><a href="'.$base_root.$base_path.'">wblc.gov.in</a></strong><br/><br/> Username :- '.$identificationNo.'<br/>Email :- '.$mailto.'<br/>Mobile :- '.$applicant_mobile_no.'<br/>Also, you may find your user credentials from <a href="'.$base_root.$base_path.'find-user-details">CLICK HERE</a> with your application information. You can generate your password from <a href="'.$base_root.$base_path.'forgot-password">Forgot your password</a> ';

			$applicant_mailto  = get_email($applicant_user_id);
			$subject = 'Welcome to Labour Commissionerate, Govt. Of West Bengal';
			if(!empty($applicant_mailto)){
				send_mail_for_user_alert($applicant_mailto, $subject, $msg);
				//send_mail_for_user_alert('wblc.nic@gmail.com', $subject, $msg);
			}
			
		}	

		$message = drupal_set_message_custom(t('Registration Certificate has been successfully generated. Please Sign and Upload Certificate to complete this process'));//-------------miscellaneous module
		$form_state['values']['hidden_action_message'] = $message;
		$form_state['rebuild']				= TRUE;
	}	
}

function generation_bocwa_registration_number($form, &$form_state){
	global $base_root, $base_path, $user;
	
	
	 $alc_user_id 		= $user->uid;
	 $val  			    = $form_state['values'];

 	 $applicant_user_id		= encryption_decryption_fun('decrypt', $val['applicant_user_id']);
	 $application_id		= encryption_decryption_fun('decrypt', $val['application_id']);
	 $act_id				= 2;
	 
	 $fieldname				= $val['fieldname'];
	 $remark_type			= $val['remark_type'];
	
	 
	 
	 /*** Get Applicant Block Code & Statuses & ALC Details ****/
	
	$get_bocwa_application							=	db_select('l_bocwa_registration_master', 'lbrm');
	$get_bocwa_application							->  leftJoin('l_customuser_relation_address', 'lcra','lcra.sub_div_code=lbrm.loc_e_subdivision ');
	$get_bocwa_application							->	fields('lbrm',array('loc_e_subdivision','loc_e_areatype_code','registration_number','bocwa_qr_code'));
	$get_bocwa_application							->	fields('lcra',array('sub_div_code'));
	$get_bocwa_application							->	condition('lbrm.id',$application_id);
	$get_bocwa_application							->	condition('lbrm.user_id',$applicant_user_id);
	
	$get_bocwa_application_result 					= 	$get_bocwa_application->execute();  
	
	if($get_bocwa_application_result-> rowCount() > 0 ){
		
		$applicant_data 							= 	$get_bocwa_application_result->fetchAssoc();
		$subdivision_code							=   $applicant_data['loc_e_subdivision'];
		$area_code									=   $applicant_data['loc_e_areatype_code'];
		$bocwa_registration_number					=   $applicant_data['registration_number'];
		$bocwa_qr_code								=   $applicant_data['bocwa_qr_code']; 
		$alc_subdiv_code							=   $applicant_data['sub_div_code']; 
		
		
	}
	
	if (empty($bocwa_registration_number) && empty($bocwa_qr_code)){
			/*** GENERATION OF QR CODE ****/
			
			 $generated_qr_code					= 'BOCWA-REG'.$application_id.'A2U'.$applicant_user_id.'T'.time();
		
			/*** GENERATION OF BOCWA REGISTRATION NUMBER ***/
			
			$getShortNameSubdivision 			=   custom_user_short_name_fun($subdivision_code);  // ---------custom_user module  
			$getRegistrarCode					=	get_registration_code($area_code);              //-------------miscellaneous module
			$registrarCode 						=   substr($getRegistrarCode, -2);
			
			$reg_query	=	db_query("select max (NULLIF(substr(registration_number,11,6), '') :: integer) as serial_num  from l_bocwa_registration_master where loc_e_subdivision='".$alc_subdiv_code."'");
			$reg_query_result					=	$reg_query->fetchAssoc();
			$reg_code							=	(int)$reg_query_result['serial_num'];
			
			if(empty($reg_code)){ 
				
						$registration_number	=	$getShortNameSubdivision.$registrarCode.'/'.'BCR'.'/'.'000001';

			}else{
					  	$reg_code_next			= 	$reg_code+1;
					  	$reg_first				=	$getShortNameSubdivision.$registrarCode.'/'.'BCR'.'/';
					  	$reg_second				=	str_pad($reg_code_next, 6, "0", STR_PAD_LEFT);
					  	$registration_number	=	$reg_first.$reg_second;
			}
			
			
					
			/*** Update Status of BOCWA Master Table ***/ 
			// if($applicant_user_id == 352){
				$update_status  =  db_update('l_bocwa_registration_master');
				$update_status->fields(array('bocwa_qr_code' => $generated_qr_code, 'registration_number' =>	$registration_number, 'registration_date' => date("Y-m-d"),'status' =>'I'));
				$update_status->condition('id',$application_id);
				$update_status->condition('act_id',$act_id);
				$update_status->condition('user_id',$applicant_user_id);
				$update_status->execute();

				
				$fetch_users_role		= db_select('users_roles', 'ur');
				$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
				$fetch_users_role->leftJoin('l_custom_user_detail', 'lcud', 'ur.uid=lcud.usr_id');
				$fetch_users_role->fields('ro', array('rid'));
				$fetch_users_role->fields('lcud', array('fullname','employee_id'));
				$fetch_users_role->condition('ur.uid', $alc_user_id);
				$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc();
			
				$remark_by_role			=	$fetch_users_role_data['rid'];
				$alc_full_name			=   $fetch_users_role_data['fullname'];	
				$employee_id			= 	$fetch_users_role_data['employee_id'];		
	
				/***Insert In BOCWA Remarks Master Table ***/ 
				
				$fieldsApplInfo2  =  array(
					'remark_text'   		=> 'Congratulations! Certificate is issued by the Registering Authority. You can download it from the dashboard.', 
					'remark_by'   			=> $alc_user_id, 
					'remark_to' 			=> $applicant_user_id,
					'application_id' 		=> $application_id,
					'remark_date'  			=> time(),
					'remark_type' 			=> $remark_type,
					'remark_by_name'    	=> $alc_full_name,
					'remark_field_title' 	=> $fieldname,
					'remark_by_role' 		=> $remark_by_role,
					'emp_id'				=> $employee_id		
					);
			
				db_insert('l_bocwa_remark_master')->fields($fieldsApplInfo2)->execute();

				$message = drupal_set_message_custom(t('Registration Certificate has been successfully generated. Please Sign and Upload Certificate to complete this process'));//-------------miscellaneous module
				$form_state['values']['hidden_action_message'] = $message;
				$form_state['rebuild']				= TRUE;


			// }else{

			// 	$update_status  =  db_update('l_bocwa_registration_master');
			// 	$update_status->fields(array('bocwa_qr_code' => $generated_qr_code, 'registration_number' =>	$registration_number, 'registration_date' => date("Y-m-d"),));
			// 	$update_status->condition('id',$application_id);
			// 	$update_status->condition('act_id',$act_id);
			// 	$update_status->condition('user_id',$applicant_user_id);
			// 	$update_status->execute();

			// 	$message							= drupal_set_message_custom(t('Registration Certificate has been successfully generated. Please Sign and Upload Certificate to complete this process'));//-------------miscellaneous module
			// 	$form_state['values']['hidden_action_message'] = $message;
			// 	$form_state['rebuild']				= TRUE;

			// }
			
			
			
		
	}
	 
	 
	
	
}
function get_bocwa_remark_details($application_id,$applicant_user_id){
	
	global $base_root, $base_path, $user;
	
	$user_id = $user->uid;
	$counter = 0;
	$loadingImg = $base_root.'/sites/all/themes/jackson/images/lodinggif.gif';
	
	$header = array(
					array('data' => 'Sl', 'field' => 'slno','width' => '5%'),
					array('data' => 'Date - Time', 'field' => 'date','width' => '15%'),
					array('data' => 'Remark', 'field' => 'remrk'),
					array('data' => 'Remark Status', 'field' => 'remark type','width' => '15%'),
					array('data' => 'Remark By', 'field' => 'licenseno','width' => '20%'),
					array('data' => 'Action','width' => '10%')
				);
	
	
	$get_remark = db_select('l_bocwa_remark_master', 'lbrm');
	$get_remark	->leftJoin('role', 'ro', 'ro.rid = lbrm.remark_by_role');
	$get_remark->fields('lbrm', array());
	$get_remark->fields('ro', array('name'));
	$get_remark->condition('lbrm.remark_to', trim($applicant_user_id));
	$get_remark->condition('lbrm.application_id', trim($application_id));
	$get_remark->condition('lbrm.is_active', 1);
	$get_remark->orderBy('lbrm.id','DESC');
	$get_remark_result = $get_remark->execute()->fetchAll(); 
	
	
	foreach($get_remark_result as $row){ 
			
		$counter 				= $counter+1;
		$remark_by				= $row->remark_by_name.'&nbsp;( '.$row->name.' )';
		
		// get payment info (checking  for data intsert into l_principle_epayments_data table)
		$getPayInfo				= db_query("SELECT COUNT(*) AS total_row FROM l_principle_epayments_data WHERE act_id=2 AND application_id = ".$application_id."")->fetchObject();
		$getId					= $getPayInfo->total_row; 
		
		// get latest remark given by session user
		$getMaxData				= db_query("SELECT MAX(id) AS MAXID FROM l_bocwa_remark_master WHERE is_active=1 and application_id = ".$application_id."")->fetchObject();
		$maxId					= $getMaxData->maxid;
		
		if($maxId>0){
			
			if(($row->remark_by == $user_id) &&($maxId == $row->id) && ($getId!=0 || $getId!='') && $row->remark_type != 'I'){		
				$delete	= '<span id="delBtn" class="delete-btn active-del" data-id ='.encryption_decryption_fun('encrypt',$row->id).' data-uid='.encryption_decryption_fun('encrypt',$user_id).' data-act='.encryption_decryption_fun('encrypt',2).'>&nbsp;<img style="display:none;margin-left:30px;" src="'.$loadingImg.'" alt="loading" id="loading_'.encryption_decryption_fun('encrypt',$row->id).'"></span>';
			}else{
				$delete	= '<div class="delete-btn disable-del">&nbsp;</div>';
			}	
		}
		$rows[] = array(
					array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
					array('data' => date('dS M, Y - g:i a', $row->remark_date), 'align' => 'left', 'class' => array('odd')),
					array('data' => $row->remark_text, 'align' => 'left', 'class' => array('odd')),
					array('data' => get_user_application_status($row->remark_type), 'align' => 'left', 'class' => array('odd')),
					array('data' => $remark_by, 'align' => 'left', 'class' => array('odd')),
					array('data' => $delete)
		  );
	}
		
	$variables = array(
	  		'attributes' 		=> array('class' => array('table table-striped table-responsive admin-custom-table')), 
	  		'header' 			=> $header,
	  		'rows'				=> $rows,
			'empty' 			=> t("No data found!")
	  );
	  
	$output = theme('datatable', $variables);
	  
   	return $output;	
}
function get_call_date_time_field($form, $form_state){
	$commands 	= array();
	$commands[] = ajax_command_replace('#download_link_block', drupal_render($form['bocwa_registration']['download_link']));
	$commands[] = ajax_command_replace('#bocwa_signed_certificate_block', drupal_render($form['bocwa_registration']['bocwa_signed_certificate']));
	$commands[] = ajax_command_replace('#generate_certificate_block', drupal_render($form['bocwa_registration']['generate_certificate']));
	$commands[] = ajax_command_replace('#remarks_text_block', drupal_render($form['bocwa_registration']['remarks_text']));
	$commands[] = ajax_command_replace('#submit_block', drupal_render($form['bocwa_registration']['submit']));
	return array('#type' => 'ajax', '#commands' => $commands);
	
}
