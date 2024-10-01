 <?php
drupal_add_js(drupal_get_path('module', 'alc').'/js/mycustom.js');

function view_bocwa_registration_form_amendment($form, &$form_state, $id = '',$app_user_id=''){
	global $base_root, $base_path, $user;
	
	$user_id 					= $user->uid;
	$application_id				= encryption_decryption_fun('decrypt', $id);
	
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
	
	$get_bocwa_application							=	db_select('l_bocwa_registration_master', 'lbrm');
	$get_bocwa_application							->	fields('lbrm',array('loc_e_subdivision','final_submit_status','status','registration_number','bocwa_qr_code','bocwa_registration_certificate','user_id','identification_number', 'finalfees','amendment_reference_no'));
	$get_bocwa_application							->	condition('lbrm.loc_e_subdivision',$alc_subdivision_code);
	$get_bocwa_application							->	condition('lbrm.id',$application_id);
	$get_bocwa_application							->	condition('lbrm.final_submit_status','P');
	$get_bocwa_application_result 					= 	$get_bocwa_application->execute();

	
	if($get_bocwa_application_result-> rowCount() > 0 ){
		
		$applicant_data 							= $get_bocwa_application_result->fetchAssoc();
		$applicant_subdivsion_code 					= trim($applicant_data['loc_e_subdivision']);
		$applicant_user_id							= $applicant_data['user_id'];
		$bocwa_registration_number					= $applicant_data['registration_number'];
		$bocwa_qr_code								= $applicant_data['bocwa_qr_code'];
		$applicant_final_submit_status				= $applicant_data['final_submit_status']; 
		$identification_number						= $applicant_data['identification_number']; 
		$applicant_status							= $applicant_data['status'];
		$uploaded_signed_certificate				= $applicant_data['bocwa_registration_certificate'];
		$amendment_reference_no						= $applicant_data['amendment_reference_no'];
		
		if($applicant_status == 'B'){
			$current_status = '<br><div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i>CURRENT STATUS: Back for rectification</h4>
								<p>Application is sent back for rectification. After modification by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p>
								</div>';
		}
		if($applicant_status == 'BI'){
			$current_status = '<br><div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i>CURRENT STATUS:  Back to inspector for rectification</h4><p>Application is sent back to Inspector for further verification. After verification, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
		}
		if($applicant_status == 'I'){
			$current_status = '<br><div class="callout callout-success"><h4>CURRENT STATUS: Certificate (FORM-II) is issued. </h4><p>Please check the Registration Certificate after uploading the Certificate.</div>';
		}
		if($applicant_status == 'F'){
			$current_status = '<br><div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Forwarded by Inspector to ALC</h4><p>Application is Forwarded to ALC by Inspector for further verification. Any action can be taken for the application.</p></div>';
		}
		if($applicant_status == 'T'){
			$current_status = '<br><div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Transaction successful</h4><p>Payment successful for this application. Amended application is not uploaded by the applicant. After submission of signed Amended application by the applicant, the application can be further accessible.</p></div>';
		}
		if($applicant_status == 'V'){
			$current_status = '<br><div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> 
									CURRENT STATUS: Verified and approved for fees submission. </h4><p>Application is approved and directed to pay fees. After fees payment and submission of signed Amended application by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
		}
		if($applicant_status == 'R'){
			$current_status = '<br><div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Rejected application</h4><p>If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
		}
		if($applicant_status == 'VA'){
			$current_status = '<br><div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i>CURRENT STATUS: Application has been verified &amp; approved without fees.</h4><p>Application is approved without fees. After submission of signed Amended application by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
		}
		if($applicant_status == 'S'){
			$current_status = '<br><div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS:  Amended application uploaded & application succesfully submitted</h4><p>Amended application is submitted by the Applicant. After verification of uploaded Amended application, Issue of License Certificate can be generated now or back to rectification Amended application.</p></div>';
		} 
		if($applicant_status == 'U'){
			$current_status = '<br><div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> 
									CURRENT STATUS: Back for Rectification(Amended application)</h4><p>Amended application is sent back for rectification. After modification by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p>
								</div>';
		} 
		if($applicant_status == '0'){
			$current_status = '<br><div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Pending application</h4><p>Application is applied by the Applicant. Any action can be taken for the application.</p></div>';
		} 
		
		/*** Get REMARK Details ****/
	
		$get_remark = db_select('l_bocwa_remark_master', 'lbkrm');
		$get_remark->fields('lbkrm', array('remark_field_title','remark_text','remark_type'));
		$get_remark->condition('lbkrm.remark_to', trim($applicant_user_id));
		$get_remark->condition('lbkrm.application_id', trim($application_id));
		$get_remark->condition('lbkrm.remark_by', trim($applicant_user_id), '!=');
		$get_remark->condition('lbkrm.is_active', 1);
		$get_remark->orderBy('lbkrm.id','DESC');
		$get_remark->range(0, 1);
		$get_remark_result = $get_remark->execute();
				
		$remark_details = $get_remark_result->fetchAssoc(); 
		
		if($alc_subdivision_code == $applicant_subdivsion_code ){   /** Checking for If the Application can be viewed by the ALC of the Subdivision ****/
			
			$form['#attributes'] 			= array('enctype' 		=> 'multipart/form-data');
	
			$form['amend_bocwa']['base_url'] 					= array(
																			  '#type' 	=> 'hidden',
																			  '#value' 	=> $base_root.$base_path,				  
																			);
			
			
			$form['amend_bocwa']['markup_data']					= array(
																			'#markup' 			=> view_bocwa_registration_form_func_alc($application_id,$applicant_user_id),
																			'#type' 			=> 'markup',
																			);
																									
			$form['amend_bocwa']['show_status'] 					= array(
																			'#type' 			=> 'markup',
																			'#markup'			=> $current_status,
																			);
																									
																									
			if ($applicant_status != 'B' && $applicant_status != 'V' && $applicant_status != 'I' && $applicant_status != 'VA' && $applicant_status != 'T' && $applicant_status != 'U' ){
				
				$form['amend_bocwa']['act_id']					= array(
																			'#type'				=> 'hidden',
																			'#default_value'	=> 2,
																			'#attributes' 		=> array('readonly' => 'readonly'),
																			);
																			
				
				
				$form['amend_bocwa']['hidden_action_message']	= array(
																			'#prefix'			=> '<div class="box box-default box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">ACTIONS AND REMARKS</h3></div><div class="box-body"><span style="margin:0; width:100%;">',
																			'#suffix'			=> '</span>',
																			'#type' 			=> 'markup',
																			'#markup'			=> isset($form_state['values']['hidden_action_message']) ? $form_state['values']['hidden_action_message'] : ''
																		);
				
				$form['amend_bocwa']['fieldname']				= array(
																			'#title'			=> 'Field Name',
																			'#type'				=> 'hidden',
																			'#attributes'		=> array('id' => 'bocwa_field_name_alc'),
																			'#default_value'	=> !empty($remark_details['remark_field_title']) ? $remark_details['remark_field_title'] : '',
																		);
																										
				$form['amend_bocwa']['applicant_user_id']		= array(
																			'#title'			=> 'User Id',
																			'#type'				=> 'hidden',
																			'#attributes' 		=> array('readonly' => 'readonly'),
																			'#default_value' 	=> !empty($applicant_user_id) ? encryption_decryption_fun('encrypt', $applicant_user_id) : '',
																			);
																			
				$form['amend_bocwa']['identification_number']		= array(
																			'#title'			=> 'User Id',
																			'#type'				=> 'hidden',
																			'#attributes' 		=> array('readonly' => 'readonly'),
																			'#default_value' 	=> !empty($identification_number) ? encryption_decryption_fun('encrypt', $identification_number) : '',
																			);
																										
				$form['amend_bocwa']['application_id']			= array(
																			'#title'			=> 'Application Id',
																			'#type'				=> 'hidden',
																			'#default_value' 	=> !empty($application_id) ? encryption_decryption_fun('encrypt', $application_id) : '',
																			'#attributes' 		=> array('readonly' => 'readonly'),
																			);
																										
				$pquery_result = db_query("select SUM(CAST(r.challanamount as INT)) as total_paid_amount from l_principle_epayments_receive_data as r left join l_principle_epayments_data as p on p.identification_no = r.transaction_id where r.banktransactionstatus='Success' and p.act_id = 2 and p.deposited_by = '".$applicant_user_id."'")->fetchObject();
			   
			   // print_r($pquery_result); die;
			  if($pquery_result->total_paid_amount < $applicant_data['finalfees']){
				  $amount_payable = 'Y';
			  }else{
				  $amount_payable = 'N';
			  }
			
				$form['amend_bocwa']['remark_type']				= array(
																			'#prefix' 			=> '<div class="col-md-3">&nbsp;</div><div class="form-custom col-md-6"><label class="input">',
																			'#suffix' 			=> '<i></i></label>',
																			'#title'			=> 'Please Select Actions',
																			'#type'				=> 'select',
																			'#required' 		=>  TRUE,
																			'#id'				=> 'remark_type_id',
																			'#options'			=> actions_dropdown_by_alc_act($applicant_status, $amount_payable,2),
																			'#attributes'		=> array('onchange' => 'getRemarkTypeAction(this.value)', 'class'=>array('form-control')),
																			'#ajax'				=> array(
																										'event' 		=> 'change',
																										'effect' 		=> 'fade',			
																										'callback'		=> 'get_call_date_time_field_amnd',
																										'progress'		=> '',
																									  ),
																			);
																											
					
																											
				$form['amend_bocwa']['generate_certificate'] 	= array(
																			  '#prefix' 		=> '<div id="generate_certificate_block">',
																			  '#suffix' 		=> '</div>'
																			);
																								
																								
				$form['amend_bocwa']['download_link'] 			= array(
																			  '#prefix' 		=> '<div id="download_link_block">',
																			  '#suffix' 		=> '</div>'
																			);
				
				$form['amend_bocwa']['bocwa_signed_certificate'] = array(
																			  '#prefix' 		=> '<div id="bocwa_signed_certificate_block">',
																			  '#suffix' 		=> '</div>'
																			);
																								
				$form['amend_bocwa']['remarks_text'] 			= array(
																			  '#prefix' 		=> '<div id="remarks_text_block">',
																			  '#suffix' 		=> '</div>'
																			);
																								
				$form['amend_bocwa']['submit'] 					= array(
																			  '#prefix' 		=> '<div id="submit_block">',
																			  '#suffix' 		=> '</div>'
																			);
																											
																											
				//if(isset($form_state['values']['remark_type'])){ $value_remark_type = $form_state['values']['remark_type']; }else{ $value_remark_type = '';}
				
				$value_remark_type = (isset($form_state['values']['remark_type'])) ? $form_state['values']['remark_type'] : '';	
				
				
					
				if(trim($value_remark_type) == 'I'){
						
					if(!empty($amendment_reference_no) && !empty($bocwa_qr_code)){ //echo $bocwa_registration_number.'--'.$bocwa_qr_code; exit;
						
						// $form['amend_bocwa']['download_link']	= array(
						// 													'#prefix'			=> '<div id="download_link_block">',
						// 													'#type'				=> 'markup',
						// 													'#markup' 			=> l(t('<i class="fa fa-download" aria-hidden="true"></i>&nbsp;Download BOCWA Registration Certificate'),'generate-bocwa-certificate/'.encryption_decryption_fun('encrypt', $bocwa_qr_code), array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn')))),
						// 													'#suffix' 			=> '</div><div class="clear"></div>',
						// 													);
																									
																									
						// $form['amend_bocwa']['bocwa_signed_certificate']	= array(
						// 													'#title'			=> 'Upload Signed BOCWA Registration Certificate :-',
						// 													'#type' 			=> 'managed_file',
						// 													'#upload_validators'=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
						// 													'#upload_location' 	=> 'public://upload/encrypt/',
						// 													'#process' 			=> array('import_my_file_element_process'),
						// 													'#attributes' 		=> array('id' => '','autocomplete' => 'off','class'=>array('')),
						// 													'#prefix'			=> '<div id="bocwa_signed_certificate_block">',
						// 													'#suffix'			=> '</div>',
						// 													'#required' 		=>  TRUE,
						// 													);	
																									
						$form['amend_bocwa']['remarks_text']		= array(
																			'#prefix' 			=> '<div id="remarks_text_block"><label class="input">',
																			'#suffix' 			=> '</label></div>',
																			'#title'			=> 'Remarks',
																			'#type'				=> 'textarea',
																			'#id'				=> 'remarks_text_id',
																			'#rows'				=>  2,
																			'#required' 		=>  TRUE,
																			);
																									
						$form['amend_bocwa']['submit'] 			= array(
																			  '#prefix' 		=> '<div id="submit_block">',
																			  '#suffix' 		=> '</div></div><div class="col-md-3">&nbsp;</div></div></div>',	
																			  '#type' 			=> 'submit',
																			  '#attributes' 	=> array('class' => array('btn btn-primary pull-left acremark-btn')),
																			  '#value' 			=> 'Submit',
																			  '#submit'			=> array('generation_bocwa_registration_number'),
																		  
																		);	
						
						
					}else{
					
						$form['amend_bocwa']['generate_certificate']= array (
																			  '#prefix' 		=> '<div id="submit_block">',
																			  '#suffix' 		=> '</div>',
																			  '#type' 			=> 'submit',
																			  '#attributes' 	=> array('class' => array('btn btn-primary pull-left acremark-btn')),
																			  '#value' 			=> 'Generate BOCWA Registration Certificate',
																			  '#submit'			=> array('generation_bocwa_registration_number'),
																			);
																									
																									
					}
																											
				}
					
					
				if($value_remark_type != 'I'){
																												
					$form['amend_bocwa']['remarks_text']			= array(
																			'#prefix' 			=> '<div id="remarks_text_block"><label class="input">',
																			'#suffix' 			=> '</label></div>',
																			'#title'			=> 'Remarks',
																			'#type'				=> 'textarea',
																			'#id'				=> 'remarks_text_id',
																			'#rows'				=>  2,
																			'#required' 		=>  TRUE,
																			);
					
					
					
					$form['amend_bocwa']['submit'] 				= array (
																			  '#prefix' 		=> '<div id="submit_block">',
																		 	  '#suffix' 		=> '</div></div><div class="col-md-3">&nbsp;</div></div></div>',
																			  '#type' 			=> 'submit',
																			  '#attributes' 	=> array('class' => array('btn btn-primary pull-left acremark-btn')),
																			  '#value' 			=> 'Submit'
																		  
																		);	
																										
				}
			}
			
			$form['amend_bocwa']['markup_data_1']				= array(	
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
	
	global $base_root, $base_path, $user;
	
	drupal_add_js(drupal_get_path('module', 'alc').'/js/mycustom.js');
	
	if(is_numeric($applicant_user_id) && is_numeric($application_id)){ 
		
		if(!empty($application_id)){
			
			
			/*$bocwa_application				=	db_select('l_bocwa_registration_master', 'lbrm');
			$bocwa_application				->	fields('lbrm',array('amendment_parent_id'));
			$bocwa_application				->	condition('lbrm.id',$application_id);
			$bocwa_application				->	condition('lbrm.user_id',$applicant_user_id);
			$bocwa_application_result 		= 	$bocwa_application->execute();  
			*/
			//if($bocwa_application_result-> rowCount() > 0 ){
				
				/*$applicant_data 		= 	$bocwa_application_result->fetchAssoc();
				$amendment_parent_id	=   $applicant_data['amendment_parent_id'];
				
				$bocwa_application_preview	=	db_select('l_bocwa_registration_master', 'lbrm');
				$bocwa_application_preview -> leftJoin('l_customuser_relation_address', 'lcra','lcra.sub_div_code=lbrm.loc_e_subdivision ');
				$bocwa_application_preview ->fields('lbrm',array('bocwa_registration_certificate','e_name','e_postal_address','e_permanent_address','full_name_manager','e_full_name','nature_of_build_const','max_no_of_building_workers_employed','est_date_of_commencement_building','est_date_of_completion_building','emp_address','emp_name','bocwa_registration_certificate','loc_e_name'));
				$bocwa_application_preview	->	fields('lcra',array('sub_div_code'));
				$bocwa_application_preview	->	condition('lbrm.id',$amendment_parent_id);
				$bocwa_application_preview	->	condition('lbrm.user_id',$applicant_user_id);
				$bocwa_application_result_preview 	= 	$bocwa_application_preview->execute();  */
				
				/*if($bocwa_application_result-> rowCount() > 0 ){
					
					$applicant_data_preview 		= 	$bocwa_application_result_preview->fetchAssoc();
					$bocwa_registration_certificate	=   $applicant_data_preview['bocwa_registration_certificate'];
					
					
		
					$table	 	 = 'l_bocwa_registration_master';
					$type	 	 = 'sub-table';
					$estArr	 	 = array('loc_e_dist','loc_e_subdivision','loc_e_areatype','loc_e_areatype_code','loc_e_vill_ward','loc_e_ps','loc_e_pin_number');
					$postalArr	 = array('e_postal_dist','e_postal_subdivision','e_postal_areatype','e_postal_areatype_code','e_postal_vill_ward','e_postal_ps','e_postal_pin_number');
					$empArr	 	 = array('emp_dist','emp_subdivision','emp_areatype','emp_areatype_code','emp_vill_ward','emp_ps','emp_pin_number');
					$managerArr	 = array('address_manager_dist','address_manager_subdivision','address_manager_areatype','address_manager_areatype_code','address_manager_vill_ward','address_manager_ps','address_manager_pin_number');
					$perAddArr	 = array('e_permanent_add_dist','e_permanent_add_subdivision','e_permanent_add_areatype','e_permanent_add_areatype_code','e_permanent_add_vill_ward','e_permanent_add_ps','e_permanent_add_pin_number');
					
					$parent_establishment_address 	= $applicant_data_preview['loc_e_name'].'<br/>'.get_full_address($table, $type, $amendment_parent_id, $estArr);
					$parent_postal_address 	 		= $applicant_data_preview['e_postal_address'].'<br/>'.get_full_address($table, $type, $amendment_parent_id, $postalArr);
					$parent_emp_address	 			= $applicant_data_preview['emp_address'].'<br/>'.get_full_address($table, $type, $amendment_parent_id, $empArr);
					$parent_manager_address	 		= $applicant_data_preview['address_manager'].'<br/>'.get_full_address($table, $type, $amendment_parent_id, $managerArr);
					$parent_permanent_address	 	= $applicant_data_preview['e_permanent_address'].'<br/>'.get_full_address($table, $type, $amendment_parent_id, $perAddArr);
					
					$parent_master_part_bocwa_docs = db_select('l_documents_upload', 'ldu');
					$parent_master_part_bocwa_docs->fields('ldu', array());
					$parent_master_part_bocwa_docs->condition('ldu.user_id', trim($applicant_user_id));
					$parent_master_part_bocwa_docs->condition('ldu.application_id', $content['amendment_parent_id']);
					$parent_master_part_bocwa_docs_result = $parent_master_part_bocwa_docs->execute();
					
					$parent_content_docs	=	$parent_master_part_bocwa_docs_result->fetchAssoc();
					
				}*/
			//}
			
			$master_part_bocwa_reg = db_select('l_bocwa_registration_master', 'lbrm');
			$master_part_bocwa_reg->fields('lbrm', array());		
			$master_part_bocwa_reg->condition('lbrm.user_id', $applicant_user_id);
			$master_part_bocwa_reg->condition('lbrm.id', $application_id);
			$master_part_bocwa_reg_result = $master_part_bocwa_reg->execute();
			
			if($master_part_bocwa_reg_result->rowCount() > 0){
				
				$content	= 	$master_part_bocwa_reg_result->fetchAssoc();
				$amendment_parent_id	=   $content['amendment_parent_id'];
				
				if($content['emp_gender']=='M'){
						$gender = "Male";
				}elseif($content['emp_gender']=='F'){
					$gender = "Female";
				}elseif($content['emp_gender']=='O'){
					$gender = "Transgender";
				}
				
				/** Previous Data ***/
				
				$bocwa_application_preview	=	db_select('l_bocwa_registration_master', 'lbrm');
				$bocwa_application_preview -> leftJoin('l_customuser_relation_address', 'lcra','lcra.sub_div_code=lbrm.loc_e_subdivision ');
				$bocwa_application_preview ->fields('lbrm',array('bocwa_registration_certificate','e_name','e_postal_address','e_permanent_address','full_name_manager','e_full_name','nature_of_build_const','max_no_of_building_workers_employed','est_date_of_commencement_building','est_date_of_completion_building','emp_address','emp_name','bocwa_registration_certificate','loc_e_name','address_manager','est_type','emp_gender'));
				$bocwa_application_preview	->	fields('lcra',array('sub_div_code'));
				$bocwa_application_preview	->	condition('lbrm.id',$amendment_parent_id);
				$bocwa_application_preview	->	condition('lbrm.user_id',$applicant_user_id);
				$bocwa_application_result_preview 	= 	$bocwa_application_preview->execute(); 
				
				if($bocwa_application_result_preview-> rowCount() > 0 ){
					
					$applicant_data_preview 		= 	$bocwa_application_result_preview->fetchAssoc();
					$bocwa_registration_certificate	=   $applicant_data_preview['bocwa_registration_certificate'];
					
					if($applicant_data_preview['emp_gender']=='M'){
							$previousgender = "Male";
					}elseif($applicant_data_preview['emp_gender']=='F'){
						$previousgender = "Female";
					}elseif($applicant_data_preview['emp_gender']=='O'){
						$previousgender = "Transgender";
					}
					
					/** Fetch Address from Miscellaneous Module ****/
		
					$table	 	 = 'l_bocwa_registration_master';
					$type	 	 = 'sub-table';
					$estArr	 	 = array('loc_e_dist','loc_e_subdivision','loc_e_areatype','loc_e_areatype_code','loc_e_vill_ward','loc_e_ps','loc_e_pin_number');
					$postalArr	 = array('e_postal_dist','e_postal_subdivision','e_postal_areatype','e_postal_areatype_code','e_postal_vill_ward','e_postal_ps','e_postal_pin_number');
					$empArr	 	 = array('emp_dist','emp_subdivision','emp_areatype','emp_areatype_code','emp_vill_ward','emp_ps','emp_pin_number');
					$managerArr	 = array('address_manager_dist','address_manager_subdivision','address_manager_areatype','address_manager_areatype_code','address_manager_vill_ward','address_manager_ps','address_manager_pin_number');
					$perAddArr	 = array('e_permanent_add_dist','e_permanent_add_subdivision','e_permanent_add_areatype','e_permanent_add_areatype_code','e_permanent_add_vill_ward','e_permanent_add_ps','e_permanent_add_pin_number');
					
					$parent_establishment_address 	= $applicant_data_preview['loc_e_name'].'<br/>'.get_full_address($table, $type, $amendment_parent_id, $estArr);
					$parent_postal_address 	 		= $applicant_data_preview['e_postal_address'].'<br/>'.get_full_address($table, $type, $amendment_parent_id, $postalArr);
					$parent_emp_address	 			= $applicant_data_preview['emp_address'].'<br/>'.get_full_address($table, $type, $amendment_parent_id, $empArr);
					$parent_manager_address	 		= $applicant_data_preview['address_manager'].'<br/>'.get_full_address($table, $type, $amendment_parent_id, $managerArr);
					$parent_permanent_address	 	= $applicant_data_preview['e_permanent_address'].'<br/>'.get_full_address($table, $type, $amendment_parent_id, $perAddArr);
					
					$parent_master_part_bocwa_docs = db_select('l_documents_upload', 'ldu');
					$parent_master_part_bocwa_docs->fields('ldu', array());
					$parent_master_part_bocwa_docs->condition('ldu.user_id', trim($applicant_user_id));
					$parent_master_part_bocwa_docs->condition('ldu.application_id', $content['amendment_parent_id']);
					$parent_master_part_bocwa_docs_result = $parent_master_part_bocwa_docs->execute();
					
					$parent_content_docs	=	$parent_master_part_bocwa_docs_result->fetchAssoc();
					
				}
				
				/** Fetch Address from Miscellaneous Module ****/

				$table	 	 = 'l_bocwa_registration_master';
				$type	 	 = 'sub-table';
				$estArr	 	 = array('loc_e_dist','loc_e_subdivision','loc_e_areatype','loc_e_areatype_code','loc_e_vill_ward','loc_e_ps','loc_e_pin_number');
				$postalArr	 = array('e_postal_dist','e_postal_subdivision','e_postal_areatype','e_postal_areatype_code','e_postal_vill_ward','e_postal_ps','e_postal_pin_number');
				$empArr	 	 = array('emp_dist','emp_subdivision','emp_areatype','emp_areatype_code','emp_vill_ward','emp_ps','emp_pin_number');
				$managerArr	 = array('address_manager_dist','address_manager_subdivision','address_manager_areatype','address_manager_areatype_code','address_manager_vill_ward','address_manager_ps','address_manager_pin_number');
				$perAddArr	 = array('e_permanent_add_dist','e_permanent_add_subdivision','e_permanent_add_areatype','e_permanent_add_areatype_code','e_permanent_add_vill_ward','e_permanent_add_ps','e_permanent_add_pin_number');
				
				$establishment_address 	= $content['loc_e_name'].'<br/>'.get_full_address($table, $type, $application_id, $estArr);
				$postal_address 	 	= $content['e_postal_address'].'<br/>'.get_full_address($table, $type, $application_id, $postalArr);
				$emp_address	 		= $content['emp_address'].'<br/>'.get_full_address($table, $type, $application_id, $empArr);
				$manager_address	 	= $content['address_manager'].'<br/>'.get_full_address($table, $type, $application_id, $managerArr);
				$permanent_address	 	= $content['e_permanent_address'].'<br/>'.get_full_address($table, $type, $application_id, $perAddArr);
				
				/**** Uploaded Documents ****/
			
				$master_part_bocwa_docs = db_select('l_documents_upload', 'ldu');
				$master_part_bocwa_docs->fields('ldu', array());
				$master_part_bocwa_docs->condition('ldu.user_id', trim($applicant_user_id));
				$master_part_bocwa_docs->condition('ldu.application_id', $application_id);
				$master_part_bocwa_docs_result = $master_part_bocwa_docs->execute();
				$content_docs	=	$master_part_bocwa_docs_result->fetchAssoc();
				
				$query = db_select('l_bocwa_amendment', 'lram')
				 ->	fields('lram', array('not_amended_fields'))
				 ->	condition('lram.application_id', $content['amendment_parent_id']);
				$amendment_field_text_array = $query->execute()->fetchAssoc();
				$notAmendedFields = explode(',', $amendment_field_text_array['not_amended_fields']);
				
				foreach ($notAmendedFields as $notToBeAmendedFields){ 
					if($notToBeAmendedFields == 'e_name'){ $amend_e_name_ck = "checked='checked'   disabled='disabled'";}
					if($notToBeAmendedFields == 'postal_address'){ $amend_postal_address_ck = "checked='checked' disabled='disabled' ";}	
					if($notToBeAmendedFields == 'e_permanent_address'){ $amend_e_permanent_address_ck = "checked='checked' disabled='disabled'";}	
					if($notToBeAmendedFields == 'manager_info'){ $amend_manager_info_ck = "checked='checked' disabled='disabled'";}	
					if($notToBeAmendedFields == 'e_nature_of_work'){ $amend_e_nature_of_work_ck = "checked='checked' disabled='disabled'";}	
					if($notToBeAmendedFields == 'max_num_of_workmen'){ $amend_max_num_of_workmen_ck = "checked='checked' disabled='disabled'";}	
					if($notToBeAmendedFields == 'est_date_comm'){ $amend_est_date_comm_ck = "checked='checked' disabled='disabled'";}	
					if($notToBeAmendedFields == 'est_date_completion'){ $amend_est_date_completion_ck = "checked='checked' disabled='disabled'";}
					if($notToBeAmendedFields == 'emp_info'){ $amend_emp_info_ck = "checked='checked' disabled='disabled'";}
				}
				
				$get_remark = db_select('l_bocwa_remark_master', 'lbkrm');
				$get_remark->fields('lbkrm', array('remark_field_title'));
				$get_remark->condition('lbkrm.remark_to', trim($applicant_user_id));
				$get_remark->condition('lbkrm.application_id', trim($application_id));
				$get_remark->condition('lbkrm.remark_by', (int)$applicant_user_id, '!=');
				$get_remark->condition('lbkrm.is_active', 1);
				$get_remark->orderBy('lbkrm.id','DESC');
				$get_remark->range(0, 1);
				$content_5 = $get_remark->execute()->fetchAssoc();
				$remarkVerifiedFields = explode(',', $content_5['remark_field_title']);
				
				foreach ($remarkVerifiedFields as $verifiedFields){ 
					if($verifiedFields == 'e_name'){ $e_name_ck = "checked='checked'";}
					if($verifiedFields == 'est_type'){ $est_type_ck = "checked='checked'";}
					if($verifiedFields == 'loc_e_name'){ $loc_e_name_ck = "checked='checked'";}
					if($verifiedFields == 'postal_address'){ $postal_address_ck = "checked='checked'";}	
					if($verifiedFields == 'e_permanent_address'){ $e_permanent_address_ck = "checked='checked'";}	
					if($verifiedFields == 'manager_info'){ $manager_info_ck = "checked='checked'";}	
					if($verifiedFields == 'e_nature_of_work'){ $e_nature_of_work_ck = "checked='checked'";}	
					if($verifiedFields == 'max_num_of_workmen'){ $max_num_of_workmen_ck = "checked='checked'";}	
					if($verifiedFields == 'est_date_comm'){ $est_date_comm_ck = "checked='checked'";}	
					if($verifiedFields == 'est_date_completion'){ $est_date_completion_ck = "checked='checked'";}
					if($verifiedFields == 'emp_gender'){ $emp_gender_ck = "checked='checked'";}
					if($verifiedFields == 'emp_info'){ $emp_info_ck = "checked='checked'";}
					
					if($verifiedFields == 'trade_license_file'){ $trade_license_file_ck = "checked='checked'";}	
					if($verifiedFields == 'article_of_assoc_file'){ $article_of_assoc_file_ck = "checked='checked'";}
					if($verifiedFields == 'memorandum_of_cert_file'){ $memorandum_of_cert_file_ck = "checked='checked'";}	
					if($verifiedFields == 'partnership_deed_file'){ $partnership_deed_file_ck = "checked='checked'";}
					if($verifiedFields == 'challan_file'){ $challan_file_ck = "checked='checked'";}
					if($verifiedFields == 'work_order_file'){ $work_order_file_ck = "checked='checked'";}	
					if($verifiedFields == 'form_one_asses_ses_file'){ $form_one_asses_ses_file_ck = "checked='checked'";}	
					if($verifiedFields == 'supp_asses_ses_file'){ $supp_asses_ses_file_ck = "checked='checked'";}
					if($verifiedFields == 'other_doc_file'){ $other_doc_file_ck = "checked='checked'";}	
					if($verifiedFields == 'bocwa_address_proof_file'){ $bocwa_address_proof_file_ck = "checked='checked'";}	
					if($verifiedFields == 'form_1_bocwa_signed_pdf_file') { $form_1_bocwa_signed_pdf_file_ck  = "checked='checked'";}
					
				}
			}

			
		}
		
		$output = '';
		
		/*<tr><td colspan="3" align="center" style="text-align:center;font-size:17px; padding-top: 8px;">
						<font color="#3366FF"><strong>APPLICATION OF REGISTRATION UNDER BOCWA</strong></font></td>
					</tr>*/
					
		// data from encrypted data table
	
	$enc_doc_array = array('TL'=> '','PD'=> '','AOA'=> '','MOA'=> '','MOC'=>'','FL'=> '','AC'=> '','PC'=> '','BB'	=> '','SC'	=> '','IC'	=> '','AP'=> '','ODSC'=> '','CR'=> '','WO'=> '','ORC'=> '','CH'=>'','FI'=>'');
	$pdficonlink = '<i class="fa fa-file fa-lg"></i>&nbsp;';
	$query 			= db_select('l_encrypted_uploaded_documents','leud')->fields('leud',array())->condition('status','1')->condition('act_id',2)->condition('user_id',$applicant_user_id)->condition('application_id',$application_id);
	$query_result 	= $query->execute();
	
	if($query_result->rowCount() > 0){
		foreach($query_result->fetchAll() as $data ){
			$enc_doc_array[$data->document_type_code]['content'] = 'view_documents/'.encryption_decryption_fun('encrypt',$data->id);
		}
	}
	
	$parent_enc_doc_array = array('TL'=> '','PD'=> '','AOA'=> '','MOA'=> '','MOC'=>'','FL'=> '','AC'=> '','PC'=> '','BB'	=> '','SC'	=> '','IC'	=> '','AP'=> '','ODSC'=> '','CR'=> '','WO'=> '','ORC'=> '','CH'=>'','FI'=>'');

	
	$parent_query 			= db_select('l_encrypted_uploaded_documents','leud')->fields('leud',array())->condition('status','1')->condition('act_id',2)->condition('user_id',$applicant_user_id)->condition('application_id',$content['amendment_parent_id']);
	$parent_query_query_result 	= $parent_query->execute();
	
	if($parent_query_query_result->rowCount() > 0){
		foreach($parent_query_query_result->fetchAll() as $data_parent ){
			$parent_enc_doc_array[$data_parent->document_type_code]['content'] = 'view_documents/'.encryption_decryption_fun('encrypt',$data_parent->id);
		}
	}
	
	$color = '<font style="color:red"> ** </font>';
	if($content['e_name']!=$applicant_data_preview['e_name']){$style_e_name="$color";}
	if($content['loc_e_name']!=$applicant_data_preview['loc_e_name'] || $establishment_address!=$parent_establishment_address){$style_loc_e_name="$color";}
	if($content['e_postal_address'] != $applicant_data_preview['e_postal_address'] || $postal_address != $parent_postal_address){$style_e_postal_address="$color";}
	if($content['e_full_name'] != $applicant_data_preview['e_full_name'] || $content['e_permanent_address'] != $applicant_data_preview['e_permanent_address'] || $permanent_address != $parent_permanent_address){$style_e_full_name="$color";}
	if($content['full_name_manager'] != $applicant_data_preview['full_name_manager'] || $manager_address != $parent_manager_address){$style_full_name_manager="$color";}
	
	if($content['nature_of_build_const']!=$applicant_data_preview['nature_of_build_const'] ){$style_nbuild_cost="$color";}
	if($content['max_no_of_building_workers_employed']!=$applicant_data_preview['max_no_of_building_workers_employed']){$style_max_no="$color";}
	
	if($content['est_date_of_commencement_building']!=$applicant_data_preview['est_date_of_commencement_building']){$style_est_date="$color";}
	if($content['est_date_of_completion_building']!=$applicant_data_preview['est_date_of_completion_building']){$style_est_date_of_completion="$color";}
	if($content['emp_name']!=$applicant_data_preview['emp_name'] || $content['emp_address']!= $applicant_data_preview['emp_address'] || $emp_address!=$parent_emp_address){$style_emp_name="$color";}
	
	$global_link = $base_root.$base_path.'sites/default/files/upload/';
	
	
	
	
		$output = '<div class="box box-primary box-solid">
    <div class="box-header with-border">
    	<div style="text-align:center;">NOTE:- All inputs are provided by Principal Employer <br/><input type="checkbox" id="check_symbol" class="note check2" checked="checked" /><label class="label2" for="check_symbol"></label> verified&nbsp;&nbsp; <input type="checkbox" id="uncheck_symbol" class="note check2" /><label class="label2" for="uncheck_symbol"></label>&nbsp;&nbsp; not verified   </div> 
    </div>
			   <table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-bordered table-responsive">					
					<tr>
						<th width="2%" style="text-align:center;font-size:17px; padding-top: 8px;" >Sl.No.</th>
						<th width="30%" style="text-align:center;font-size:17px; padding-top: 8px;" >Parameters</th>
						<th width="30%" style="text-align:center;font-size:17px; padding-top: 8px;">Inputs</th>
						<th width="30%" style="text-align:center;font-size:17px; padding-top: 8px;">Previous Inputs</th>
						<th width="8%" style="text-align:center;font-size:17px; padding-top: 8px;">Verified?</th>
					</tr>
					
					<tr>
						<td>1.</td>
						<td>Name of the Establishment'.$style_e_name.'</td>
						<td>'.$content['e_name'].'</td>
						<td>'.$applicant_data_preview['e_name'].'</td>
						<td><input type="checkbox" id="e_name" class="bocwa_verified_alc"  '.$e_name_ck.$amend_e_name_ck.' /></td>
					</tr>
					<tr>
						<td>1.a)</td>
						<td>Establishment Type'.'</td>
						<td>'.ucfirst($content['est_type']).'</td>
						<td>'.ucfirst($applicant_data_preview['est_type']).'</td>
						<td><input type="checkbox" id="est_type" class="bocwa_verified_alc"  '.$est_type_ck.' /></td>
					</tr>
					<tr >
						<td>1.b)</td>
						<td> Location(Work site) of the Establishment'.$style_loc_e_name.' </td>';
			$output .= '<td>'.$establishment_address.' </td><td>'.$parent_establishment_address.' </td>
 						<td><input type="checkbox" id="loc_e_name" class="bocwa_verified_alc" checked="checked" disabled="disabled"/></td>
				    </tr>
					<tr>
						<td>2.</td>
						<td> Postal Address(Work site) of the Establishment'.$style_e_postal_address.'</td>';
			$output .= '<td>'.$postal_address.'</td><td>'.$parent_postal_address.'</td>
 						<td><input type="checkbox" id="postal_address" class="bocwa_verified_alc" '.$postal_address_ck.$amend_postal_address_ck.' /></td>
					</tr>
					<tr>
						<td>3.</td>
						<td> Full Name and permanent address of the Establishment'.$style_style_e_full_name.'</td>
						<td>'.$content['e_full_name'].'<br>'.$permanent_address. '</td>
						<td>'.$applicant_data_preview['e_full_name'].'<br/>'.$parent_permanent_address. '</td>
						<td><input type="checkbox" id="e_permanent_address" class="bocwa_verified_alc" '.$e_permanent_address_ck.$amend_e_permanent_address_ck.'/></td>
					</tr>
					
					<tr>
						<td>4.</td>
						<td> Full name and address of the Manager or Person responsible for the supervision and control of the Establishment'.$style_full_name_manager.'</td>
						<td>'.$content['full_name_manager'].'<br>'.$manager_address.'</td>
						<td>'.$applicant_data_preview['full_name_manager'].'<br>'.$parent_manager_address.'</td>
						<td><input type="checkbox" id="manager_info" class="bocwa_verified_alc" '.$manager_info_ck.$amend_manager_info_ck.' /></td>
					</tr>
					
					<tr>
						<td>5.</td>
						<td>Nature of building or other construction work carried/is to be carried on in the Establishment'.$style_nbuild_cost.'</td>
						<td>'.$content['nature_of_build_const'] .'</td>
						<td>'.$applicant_data_preview['nature_of_build_const'] .'</td>
						<td><input type="checkbox" id="e_nature_of_work" class="bocwa_verified_alc" '.$e_nature_of_work_ck.$amend_e_nature_of_work_ck.'/></td>
					</tr>
					<tr>
						<td>6.</td>
						<td>Maximum Number of building workers to be employed on any day'.$style_max_no.'</td>
						<td>'.$content['max_no_of_building_workers_employed'].'</td>
						<td>'.$applicant_data_preview['max_no_of_building_workers_employed'].'</td>
						<td><input type="checkbox" id="max_num_of_workmen" class="bocwa_verified_alc" '.$max_num_of_workmen_ck.$amend_max_num_of_workmen_ck.' /></td>
					</tr>
					<tr>
						<td>7.</td>
						<td>Estimated date of commencement of building or other construction work'.$style_est_date.'</td>
						<td>'.date('dS M, Y',strtotime($content['est_date_of_commencement_building'])).'</td>
						<td>'.date('dS M, Y',strtotime($applicant_data_preview['est_date_of_commencement_building'])).'</td>
						<td><input type="checkbox" id="est_date_comm" class="bocwa_verified_alc" '.$est_date_comm_ck.$amend_est_date_comm_ck.' /></td>
					</tr>
					<tr>
						<td>8.</td>
						<td>Estimated date of completion of building or other construction work'.$style_est_date_of_completion.'</td>
						<td>'.date('dS M, Y',strtotime($content['est_date_of_completion_building'])).'</td>
						<td>'.date('dS M, Y',strtotime($applicant_data_preview['est_date_of_completion_building'])).'</td>
						<td><input type="checkbox" id="est_date_completion" class="bocwa_verified_alc" '.$est_date_completion_ck.$amend_est_date_completion_ck.'  /></td>
					</tr>
					<tr>
						<td>9.</td>
						<td>Full name and address of the employer'.$style_emp_nam.'</td>
						<td>'.$content['emp_name'].'<br>'.$emp_address.'</td>
						<td>'.$applicant_data_preview['emp_name'].'<br>'.$parent_emp_address.'</td>
						<td><input type="checkbox" id="emp_info" class="bocwa_verified_alc" '.$emp_info_ck.$amend_emp_info_ck.'  /></td>
					</tr>
					<tr>
						<td>9.a)</td>
						<td>Gender of the employer</td>
						<td>'.$gender.'</td>
						<td>'.$previousgender.'</td>
						<td><input type="checkbox" id="emp_gender" class="bocwa_verified_alc" '.$emp_gender_ck.'  /></td>
					</tr>
					
					
					<tr>
						<td>10.</td>
    					<td colspan="3" style="text-align:left;font-size:17px; padding-top: 8px;"><strong>Uploaded Documents</strong></td>
  					</tr>
					
					<tr>
						<td>11.a)</td>
						<td>Trade License</td>';
						
						if($enc_doc_array['TL']['content'] !=''){
								
								
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['TL']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['trade_license_file'])){
							 
							$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_trade_license/'.$content_docs['trade_license_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp; </td>';
							   
						}else {
							
							$output .=  '<td>No Document Uploaded</td>';
							
						}
						
						///previous
						
						if($parent_enc_doc_array['TL']['content'] !=''){
								
								$output .= '<td>'.l($pdficonlink,$parent_enc_doc_array['TL']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['trade_license_file'])){
							 
								$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_trade_license/'.$parent_content_docs['trade_license_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp; </td>';
							   
						}else {
							
							$output .=  '<td>No Document Uploaded</td>';
							
						}
							
							
						$output .=  '<td><input type="checkbox" id="trade_license_file" class="bocwa_verified_alc" '.$trade_license_file_ck.'/></td>
						
					</tr>
				
				
					<tr>
						<td>11.b)</td>
						<td>Articles of Association and Memorandum of Association/Partnership Deed</td>';
						
						if($enc_doc_array['AOA']['content'] !=''){
									
							$output .= '<td>'.l($pdficonlink,$enc_doc_array['AOA']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['article_of_assoc_file'])){ 
						
							$output .= ' <td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_article_of_assoc/'.$content_docs['article_of_assoc_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;</td>';
											
						}else{
							
								$output .=  '<td>No Document Uploaded</td>';
								
						}
						
						///privous
						
						if($parent_enc_doc_array['AOA']['content'] !=''){
					
					
							$output .= '<td>'.l($pdficonlink,$parent_enc_doc_array['AOA']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($parent_content_docs['article_of_assoc_file'])){ 
				
							$output .= ' <td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_article_of_assoc/'.$parent_content_docs['article_of_assoc_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;</td>';
							
						}else{
							
							$output .=  '<td>No Document Uploaded</td>';
								
						}
						
						$output .= '<td><input type="checkbox" id="article_of_assoc_file" class="bocwa_verified_alc" '.$article_of_assoc_file_ck.'/></td>
			  		</tr>


			  
			  		<tr>
						<td>11.c)</td>
						<td>Any other document in support of correctness of the particulars mentioned in the application if required</td>';
					
						if($enc_doc_array['MOC']['content'] !=''){
									
									
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['MOC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['memorandum_of_cert_file'])){ 
						
							$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_memorandum_of_cert/'.$content_docs['memorandum_of_cert_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;</td>';
						}else{
							
							$output .=  '<td>No Document Uploaded</td>';
						}
						
						if($parent_enc_doc_array['MOC']['content'] !=''){
									
									
								$output .= '<td>'.l($pdficonlink,$parent_enc_doc_array['MOC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($parent_content_docs['memorandum_of_cert_file'])){ 
						
							$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_memorandum_of_cert/'.$parent_content_docs['memorandum_of_cert_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;</td>';
						}else{
							
							$output .=  '<td>No Document Uploaded</td>';
						}
						
						$output .=  '<td><input type="checkbox" id="memorandum_of_cert_file" class="bocwa_verified_alc" '.$memorandum_of_cert_file_ck.'/></td>
						
				  </tr>
				
				  <tr>
				  		<td>11.d)</td>
						<td>Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc.</td>';
						
						 if($enc_doc_array['PD']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['PD']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['partnership_deed_file'])){
							  
								$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_partnership_deed/'.$content_docs['partnership_deed_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;</td>';
						}else{
							
							$output .=  '<td>No Document Uploaded</td>';
						}
						
						 if($parent_enc_doc_array['PD']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$parent_enc_doc_array['PD']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($parent_content_docs['partnership_deed_file'])){
							  
								$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_partnership_deed/'.$parent_content_docs['partnership_deed_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;</td>';
						}else{
							
							$output .=  '<td>No Document Uploaded</td>';
						}
									
						$output .= '<td><input type="checkbox" id="partnership_deed_file" class="bocwa_verified_alc" '.$partnership_deed_file_ck.'/></td>
					</tr>
					
					 <tr>
					 	<td>11.e)</td>
						<td>Challan</td>';
						
						 if($enc_doc_array['CH']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['CH']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['challan_file'])){
							  
								$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_challan/'.$content_docs['challan_file'].'">
												<i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;
											</td>';
						}else{
							
							$output .=  '<td>No Document Uploaded</td>';
						}
						
						 if($parent_enc_doc_array['CH']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$parent_enc_doc_array['CH']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($parent_content_docs['challan_file'])){
							  
								$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_challan/'.$parent_content_docs['challan_file'].'">
												<i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;
											</td>';
						}else{
							
							$output .=  '<td>No Document Uploaded</td>';
						}
									
						$output .= '<td><input type="checkbox" id="challan_file" class="bocwa_verified_alc" '.$challan_file_ck.' /></td>
					</tr>
					
					<tr>
						<td>11.f)</td>
						<td>Work Order</td>';
						
						 if($enc_doc_array['WO']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['WO']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['work_order_file'])){
							  
								$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_work_order/'.$content_docs['work_order_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;</td>';
						}else{
							
								$output .=  '<td>No Document Uploaded</td>';
						}
						
						 if($parent_enc_doc_array['WO']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$parent_enc_doc_array['WO']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['work_order_file'])){
							  
								$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_work_order/'.$parent_content_docs['work_order_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;</td>';
						}else{
							
								$output .=  '<td>No Document Uploaded</td>';
						}
									
						$output .= '<td><input type="checkbox" id="work_order_file" class="bocwa_verified_alc" '.$work_order_file_ck.'/></td>
					</tr>
					
					<tr>
						<td>11.g)</td>
						<td>Form I for assesment of CESS</td>';
						
						 if($enc_doc_array['AC']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['AC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						 }elseif(!empty($content_docs['form_one_asses_ses_file'])){
							  
								$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_form_one_asses_of_ses/'.$content_docs['form_one_asses_ses_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;</td>';
						}else{
							
							$output .=  '<td>No Document Uploaded</td>';
						}
						
						 if($parent_enc_doc_array['AC']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$parent_enc_doc_array['AC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						 }elseif(!empty($content_docs['form_one_asses_ses_file'])){
							  
								$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_form_one_asses_of_ses/'.$parent_content_docs['form_one_asses_ses_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;</td>';
						}else{
							
							$output .=  '<td>No Document Uploaded</td>';
						}
									
						$output .= '<td><input type="checkbox" id="form_one_asses_ses_file" class="bocwa_verified_alc" '.$form_one_asses_ses_file_ck.'/></td>
					</tr>
					
					<tr>
						<td>11.h)</td>
						<td>Documents in Support of Payment of CESS</td>';
						
						 if($enc_doc_array['PC']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['PC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['supp_asses_ses_file'])){
							  
								$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_support_payment_ses/'.$content_docs['supp_asses_ses_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;</td>';
						}else{
							
							$output .=  '<td>No Document Uploaded</td>';
						}
						
						if($parent_enc_doc_array['PC']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$parent_enc_doc_array['PC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['supp_asses_ses_file'])){
							  
								$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_support_payment_ses/'.$parent_content_docs['supp_asses_ses_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;</td>';
						}else{
							
							$output .=  '<td>No Document Uploaded</td>';
						}
									
						$output .= '<td><input type="checkbox" id="supp_asses_ses_file" class="bocwa_verified_alc"  '.$supp_asses_ses_file_ck.'/></td>
					</tr>
					
					<tr>
						<td>11.i)</td>
						<td>Documents in Support of Correctness of  Application</td>';
						
						 if($enc_doc_array['ODSC']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['ODSC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['other_doc_file'])){
							  
								$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_other_docs/'.$content_docs['other_doc_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;</td>';
						}else{
							
							$output .=  '<td>No Document Uploaded</td>';
						}
						
						 if($parent_enc_doc_array['ODSC']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$parent_enc_doc_array['ODSC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['other_doc_file'])){
							  
								$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_other_docs/'.$parent_content_docs['other_doc_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;</td>';
						}else{
							
							$output .=  '<td>No Document Uploaded</td>';
						}
									
						$output .= '<td><input type="checkbox" id="other_doc_file" class="bocwa_verified_alc" '.$other_doc_file_ck.'/></td>
					</tr>
					<tr>
						<td>11.j)</td>
						<td>Address Proof</td>';
						
						 if($enc_doc_array['AP']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['AP']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['address_proof_file'])){
							  
								$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_address_proof/'.$content_docs['address_proof_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;</td>';
						}else{
							
							$output .=  '<td>No Document Uploaded</td>';
						}
						
						if($parent_enc_doc_array['AP']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$parent_enc_doc_array['AP']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($parent_content_docs['address_proof_file'])){
							  
								$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_address_proof/'.$parent_content_docs['address_proof_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;</td>';
						}else{
							
							$output .=  '<td>No Document Uploaded</td>';
						}
									
						$output .= '<td><input type="checkbox" id="bocwa_address_proof_file" class="bocwa_verified_alc" '.$bocwa_address_proof_file_ck.'/></td>
					</tr>
					<tr>
						<td>11.k)</td>
						<td>Amended application</td>';
						
						 if($enc_doc_array['FI']['content'] !=''){
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['FI']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['form_1_bocwa_signed_pdf_file'])){
							  
								$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/BOCWA_Form-I/'.$content_docs['form_1_bocwa_signed_pdf_file'].'"></a>';
								if($content['status']=='S' || $content['status']=='U' || $content['status']=='I' ){
								
									$output .= '<i class="fa fa-file fa-lg"></i>&nbsp;&nbsp; | &nbsp; '.l(t('Generated Amended application &nbsp;<i class="fa fa-info-circle fa-lg" aria-hidden="true"></i> '),'bocwa-generate-pdf/'.encryption_decryption_fun('encrypt', $content['id']).'/'.encryption_decryption_fun('encrypt', $content['user_id']), array('html' => TRUE,'attributes'=> array('target'=>'_blank','style'=>'color:#337ab7')));
								}
							$output .= '</td>';
						}else{
							
							$output .=  '<td>No Document Uploaded</td>';
						}
						
						if($parent_enc_doc_array['FI']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$parent_enc_doc_array['FI']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($parent_content_docs['form_1_bocwa_signed_pdf_file'])){
							  
							  if($parent_content_docs['amendment_parent_id']!=''){ $generatedVar = 'Amended Application'; }else{ $generatedVar = 'FORM - I ';}
							  
								$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/BOCWA_Form-I/'.$parent_content_docs['form_1_bocwa_signed_pdf_file'].'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp; | &nbsp; '.l(t('Generated '.$generatedVar.' &nbsp; <i class="fa fa-info-circle fa-lg" aria-hidden="true"></i> '),'bocwa-generate-pdf/'.encryption_decryption_fun('encrypt', $content['id']).'/'.encryption_decryption_fun('encrypt', $content['user_id']), array('html' => TRUE,'attributes'=> array('target'=>'_blank','style'=>'color:#337ab7'))).'
											</td>';
						}else{
							
							$output .=  '<td>No Document Uploaded</td>';
						}
									
						$output .= '<td id="formVId"><input type="checkbox" id="form_1_bocwa_signed_pdf_file" class="bocwa_verified_alc" '.$form_1_bocwa_signed_pdf_file_ck.'/></td>
					</tr>';
					
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
						 $payment_details .= 'Challan Details:'.l('<i class="fa fa-file fa-lg"></i>', '/sites/default/files/upload/challan_upload/'.$govt_paydata['challan_file'], array('html' => TRUE, 'title' => 'Click here to view challan'));
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
						
						 $payment_details = '<u>Grips Payment[Online/Counter]</u><br>';
						 $payment_details .= 'IFSC Code:<span class="color_orange">'.$bank_code.'</span><br>';
						 $payment_details .= 'Bank Transaction Id:<span class="color_orange">'.$bankTransactionID.'</span><br>';
						 $payment_details .= 'Total Amount:<span class="color_orange">'.$total_amount.'</span><br>';
						 $payment_details .= 'Transaction Date:<span class="color_orange">'.$challan_fid_date.'</span><br>';
						 $payment_details .= 'Transaction Status: '.$payment_status;
					 }
					 ///previous payment details
					 
					 if($applicant_data_preview['payment_mode'] == 'G'){	 
						 $pre_govt_pay_query = db_select('l_govt_payment', 'lgp');
						 $pre_govt_pay_query->fields('lgp', array());
						 $pre_govt_pay_query->condition('lgp.application_id', $amendment_parent_id);
						 
						 $pre_govt_pay_query_result = $pre_govt_pay_query->execute();
						
					
						if($pre_govt_pay_query_result->rowCount() > 0 ){		
							$pre_govt_paydata = $pre_govt_pay_query_result->fetchAssoc();
									
							$pre_tv_number = 'Not available';
							$pre_challan_number = 'Not available';
							$pre_total_amount = 'Not available';
							$pre_challan_date = 'Not available';
									
							if(!empty($pre_govt_paydata['tv_number'])) $pre_tv_number = $pre_govt_paydata['tv_number'];
							if(!empty($pre_govt_paydata['challan_number'])) $pre_challan_number = $pre_govt_paydata['challan_number'];
							if(!empty($pre_govt_paydata['total_amount'])) $pre_total_amount = $pre_govt_paydata['total_amount'];
							if(!empty($pre_govt_paydata['challan_date'])) $pre_challan_date = $pre_govt_paydata['challan_date'];
						}
						
						 $pre_payment_details = '<u>Government Payment[Head to Head transfer through treasury]</u><br>';
						 $pre_payment_details .= 'Transaction Voucher Number:<span class="color_orange">'.$pre_tv_number.'</span><br>';
						 $pre_payment_details .= 'Challan Number:<span class="color_orange">'.$pre_challan_number.'</span><br>';
						 $pre_payment_details .= 'Challan Date:<span class="color_orange">'.date('dS M Y', strtotime($pre_challan_date)).'</span><br>';
						 $pre_payment_details .= 'Total amount:<span class="color_orange">'.number_format($pre_total_amount,2).'</span><br>';
						 $pre_payment_details .= 'Challan Details:'.l('<i class="fa fa-file fa-lg"></i>', '/sites/default/files/upload/challan_upload/'.$pre_govt_paydata['challan_file'], array('html' => TRUE, 'title' => 'Click here to view challan'));
					  }else{		
						 $pre_transaction_details  			= db_select('l_principle_epayments_receive_data', 'lpd');
						 $pre_transaction_details			->leftJoin('l_principle_epayments_data', 'lped', 'lped.identification_no = lpd.transaction_id');
						 $pre_transaction_details			->fields('lped', array('identification_no', 'application_id'));
						 $pre_transaction_details			->fields('lpd', array());
						 $pre_transaction_details			->condition('lped.act_id', '2');
						 $pre_transaction_details			->condition('lped.application_id', $amendment_parent_id);		
						 $pre_trans_details_result 			= $pre_transaction_details->execute();
						
						 $pre_bank_code = 'Not available';
						 $pre_bankTransactionID = 'Not available';
						 $pre_total_amount = 'Not available';
						 $pre_challan_fid_date = 'Not available';
						 $pre_payment_status = 'Not available';
							
						 if($pre_trans_details_result-> rowCount() > 0 ){			
							$pre_transaction_det	= $pre_trans_details_result->fetchAssoc();
							$pre_bankTransactionID	= $pre_transaction_det['transaction_id'];
							$pre_grn_number			= $pre_transaction_det['challanrefid'];
							$pre_challan_fid_date	= !empty($pre_transaction_det['challanrefid_date']) ? $pre_transaction_det['challanrefid_date'] : '';
							$pre_total_amount		= number_format($pre_transaction_det['challanamount'], 2);
							$pre_bank_code			= $pre_transaction_det['bank_cd'];
							if($pre_transaction_det['banktransactionstatus'] == 'Success') 
								$pre_payment_status = '<span class="color_green">'.$pre_transaction_det['banktransactionstatus'].'</span>';
							}else{
								$pre_payment_status = '<span class="color_red">'.$pre_transaction_det['banktransactionstatus'].'</span>';
							}
						
						 $pre_payment_details = '<u>Grips Payment[Online/Counter]</u><br>';
						 $pre_payment_details .= 'IFSC Code:<span class="color_orange">'.$pre_bank_code.'</span><br>';
						 $pre_payment_details .= 'Bank Transaction Id:<span class="color_orange">'.$pre_bankTransactionID.'</span><br>';
						 $pre_payment_details .= 'Total Amount:<span class="color_orange">'.$pre_total_amount.'</span><br>';
						 $pre_payment_details .= 'Transaction Date:<span class="color_orange">'.$pre_challan_fid_date.'</span><br>';
						 $pre_payment_details .= 'Transaction Status: '.$pre_payment_status;
					 }
 					/* Payment Details End */ 
 					
					 
				
					 if($content['status']=='I'){

						if($content['deemed'] == 'Y'){
							$issue_by = 'Deemed Approved';
						}else{
							$issue_by = 'Issued By ALC';
						}
					$output .='<tr>
							<td>11.l)</td>
							<td>Registration Certificate</td>';
							
						if($enc_doc_array['RC']['content'] !=''){
							
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['RC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}
						else if(!empty($content['bocwa_registration_certificate'])){ 
							 
								$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/BOCWA_Registration_Certificate/'.$content['bocwa_registration_certificate'].'"><i class="fa fa-file fa-lg"></i></a> &nbsp;|&nbsp; 
									<span class="viewfileins" id="certificates_fid_file">
										<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
									</span>
									
									<div id="fileins_certificates_fid_file" title="Registration Certificate" style="display:none;">
										<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/BOCWA_Registration_Certificate/'.$content['bocwa_registration_certificate'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
									</div>
								
								</td>';
							 }
							 else if($content['bocwa_registration_certificate'] =='' && $content['amendment_parent_id'] !=''){


								$generateDownloadLink 	= l(t('<i class="fa fa-download" aria-hidden="true"></i>&nbsp;  BOCW Registration Certificate'),$GLOBALS['base_url'].'/generate-bocwa-certificate/'.encryption_decryption_fun('encrypt', $content['bocwa_qr_code']), array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn'))));

								$output .= '<td>'.$generateDownloadLink.'&nbsp;&nbsp;&nbsp;<span class="viewfile_popup" id="signed_certificate"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
								<div id="view_signed_certificate"  title="BOCWA Registration Certificate" style="display:none;">
									<iframe src="'.$GLOBALS['base_url'].'/generate-bocwa-certificate/'.encryption_decryption_fun('encrypt', $content['bocwa_qr_code']).'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
								</div>
							</td>
							<td></td>
							<td>'.$issue_by.'</td>';
							}
							 
							//   if(!empty($bocwa_registration_certificate)){ 
							 	
							// 	$output .='<td colspan="2"><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/BOCWA_Registration_Certificate/'.$bocwa_registration_certificate.'"><i class="fa fa-file fa-lg"></i></a> &nbsp;|&nbsp; 
							// 		<span class="viewfileins" id="pri_certificates_fid_file">
							// 			<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
							// 		</span>
									
							// 		<div id="fileins_pri_certificates_fid_file" title="Registration Certificate" style="display:none;">
							// 			<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/BOCWA_Registration_Certificate/'.$bocwa_registration_certificate.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
							// 		</div>
								
							// 	</td>';
							//  } 
							 else { 
								$output .='<td>Under Process</td>';
							 }  
							 
							 
							 
				$output .='</tr>';
							}
 					$output .= '<tr>
								<td>11.m)</td>
								<td>Payment Details '.$color.'</td>
								<td>'.$payment_details.'</td>
								<td colspan="2">'.$pre_payment_details.'</td>  	
							  </tr>
					
					
					
				</table></fieldset>';
					
				$application_id_pre			= encryption_decryption_fun('encrypt', $application_id);
				$applicant_user_id_pre		= encryption_decryption_fun('encrypt', $applicant_user_id);
				$act_id						= encryption_decryption_fun('encrypt',2);
		
		$output .=  '</div><div class="row">
		<div class="col-md-2 col-xs-12">'.l('Back to list', 'alc-received-applications-bocwa', array('attributes' => array('class' => 'btn btn-block btn-info', 'title' => 'Back to List'))).'</div>
		<div class="col-md-4 col-xs-12"><i>'.l('How to digitally sign documents using USB Token', 'digitally-sign-process', array('attributes' => array('class' => 'btn btn-block btn-info', 'title' => 'How to digitally sign your documents using USB Token', 'target' => '_blank'))).'</i></div>
		
		
		<div class="col-md-4 col-xs-12"><i>'.l('Instructions to give remarks ', $base_root.$base_path.'sites/default/files/instructions-remarks.pdf', array('attributes' => array('class' => 'btn btn-block btn-info', 'title' => 'Instructions to give remarks', 'target' => '_blank'))).'</i></div>
		
		
		<div class="col-md-2 col-xs-12"><i>'.l('View Applicant Profile', 'view-applicant-profile/'.$application_id_pre.'/'.$applicant_user_id_pre.'/'.$act_id, array('attributes' => array('class' => 'btn btn-block btn-info', 'title' => 'Click here to view Applicant Profile', 'target' => '_blank'))).'</i></div>';
		
		
		
		$output .='</div>';	   
			
			   
		return $output;
		
		
	}else{
		
		drupal_goto('node/3');	
	}
}

function view_bocwa_registration_form_amendment_submit($form,&$form_state){
	
	global $base_root, $base_path, $user;
	
	$alc_user_id 		= $user->uid;
	
	$val 				= $form_state['values'];
	$remark_type		= $val['remark_type'];
	$remarks_text		= $val['remarks_text'];
	$application_id 	= $val['application_id'];
	$fieldname			= $val['fieldname'];
	$remark_type		= $val['remark_type'];
	$remarks_text		= $val['remarks_text'];
	$call_date_time		= $val['call_applicant'];
	$identification_number	= $val['identification_number'];
	
	$applicant_user_id	= encryption_decryption_fun('decrypt', $val['applicant_user_id']);	
	$application_id	= encryption_decryption_fun('decrypt', $val['application_id']);
	
	/* Upload For BOCWA Signed Certificate*/
	
	$arr_1									= array();
	$signed_form_bocwa 						= file_load($val['bocwa_signed_certificate']);
	// if(!empty($val['bocwa_signed_certificate'])){
		
	// 	db_update('l_encrypted_uploaded_documents')->fields(array('status'=> '0'))->condition('document_type_code','RC')->condition('user_id',$applicant_user_id)->condition('status','1')->condition('act_id',2)->condition('application_id',$application_id)->execute();
		
	// 	$signed_certificate 	= file_load($val['bocwa_signed_certificate']);
	// 	$signed_certificate_arr = explode("/", $signed_certificate->uri);
	// 	$signed_certificate_fname	= time().'_'.$signed_certificate_arr[4]; 
	// 	$filepathSave_tl 		= file_get_contents(escapefile_url($base_root.$base_path.'sites/default/files/upload/encrypt/'.$signed_certificate_arr[4]));
	// 	$signed_certificate_content 	= base64_encode($filepathSave_tl);
	// 	$insert_arr['RC']['fname'] = $signed_certificate_fname;
	// 	$insert_arr['RC']['content'] = $signed_certificate_content;
	// 	$arr = array(
	// 		'user_id' 		=> $applicant_user_id,
	// 		'act_id' 		=> 2,
	// 		'identification_number' => encryption_decryption_fun('decrypt', $val['identification_number']),
	// 		'document_type' => 'BOCWA AMENDMENT CERTIFICATE',
	// 		'document_type_code' => 'RC',
	// 		'filename'		=> time().$insert_arr['RC']['fname'],
	// 		'filecontent'	=> $insert_arr['RC']['content'],
	// 		'update_date'	=> date('Y-m-d H:i:s'),
	// 		'status'		=> '1',
	// 		'application_id'=>$application_id,
			
	// 	);
	// 	$encryptedId = db_insert('l_encrypted_uploaded_documents')->fields($arr)->execute();
	// 	db_update('l_document_tag_master')->fields(array('certificate'=>$encryptedId))->condition('application_id',$application_id)->condition('act_id',2)->condition('user_id',$applicant_user_id)->execute();
	// }
	
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
	
	/*** Update Status of BOCWA Master Table ***/ 
	
	$update_status  =  db_update('l_bocwa_registration_master');
	
	// if(!empty($signed_certificate_fname)){
	// 	$update_status->fields(array(
	// 			 'status'						  => $remark_type,
	// 			 'bocwa_registration_certificate' => $signed_certificate_fname,
	// 	));
	// } else{
		
		$update_status->fields(array('status'	  => $remark_type));
	// }
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
		
	);
	
	db_insert('l_bocwa_remark_master')->fields($fieldsApplInfo2)->execute();
	//sms
	$applicant_mobile_no = get_mobile_number($applicant_user_id);
	if($remark_type == 'I'){
		$msg = 'Congratulations! Your BOCWA registration certificate is issued by ALC(Govt. of WB). Login to download the certificate from dashboard (wblc.gov.in)';	
	}else{
		$msg = 'Your BOCWA application is viewed and checked by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';
	}
	
	send_sms_for_user_alert($applicant_mobile_no, $msg);
	$message = 'Remark has been successfully saved.';
	drupal_set_message(t($message));
}

function generation_bocwa_registration_number($form, &$form_state){
	drupal_add_js(drupal_get_path('module', 'alc').'/js/mycustom.js');

	global $base_root, $base_path, $user;
	
	$alc_user_id 		= $user->uid;
	
	 $val  					= $form_state['values'];
	 $fieldname			= $val['fieldname'];
	 $applicant_user_id		= encryption_decryption_fun('decrypt',$val['applicant_user_id']);
	 $application_id		= encryption_decryption_fun('decrypt',$val['application_id']);
	 $act_id				= 2;
	 
	 /*** Get Applicant Block Code & Statuses & ALC Details ****/
	
	$get_bocwa_application = db_select('l_bocwa_registration_master', 'lbrm');
	$get_bocwa_application->leftJoin('l_customuser_relation_address', 'lcra','lcra.sub_div_code=lbrm.loc_e_subdivision ');
	$get_bocwa_application->fields('lbrm',array('loc_e_subdivision','loc_e_areatype_code','registration_number','bocwa_qr_code','amendment_parent_id'));
	$get_bocwa_application->fields('lcra',array('sub_div_code'));
	$get_bocwa_application->condition('lbrm.id',$application_id);
	$get_bocwa_application->condition('lbrm.user_id',$applicant_user_id);
	
	$get_bocwa_application_result = 	$get_bocwa_application->execute();  
	if($get_bocwa_application_result-> rowCount() > 0 ){
		$applicant_data 			= 	$get_bocwa_application_result->fetchAssoc();
		$subdivision_code			=   $applicant_data['loc_e_subdivision'];
		$area_code					=   $applicant_data['loc_e_areatype_code'];
		$bocwa_registration_number	=   $applicant_data['registration_number'];
		$bocwa_qr_code				=   $applicant_data['bocwa_qr_code']; 
		$alc_subdiv_code			=   $applicant_data['sub_div_code'];
		$amendment_parent_id		=   $applicant_data['amendment_parent_id']; 
	}
	
	$bocwa_application=	db_select('l_bocwa_registration_master', 'lbrm');
	$bocwa_application->leftJoin('l_customuser_relation_address', 'lcra','lcra.sub_div_code=lbrm.loc_e_subdivision ');
	$bocwa_application->fields('lbrm',array('loc_e_subdivision','loc_e_areatype_code','registration_number','bocwa_qr_code','bocwa_registration_certificate'));
	$bocwa_application->fields('lcra',array('sub_div_code'));
	$bocwa_application->condition('lbrm.id',$amendment_parent_id);
	$bocwa_application->condition('lbrm.user_id',$applicant_user_id);
	$bocwa_application_result 	= 	$bocwa_application->execute();  
	
	if($bocwa_application_result-> rowCount() > 0 ){
		$applicant_data 				= 	$bocwa_application_result->fetchAssoc();
		$bocwa_registration_number		=   $applicant_data['registration_number'];
		$bocwa_registration_certificate	=   $applicant_data['bocwa_registration_certificate'];
	}
	/*** GENERATION OF QR CODE ****/
	
	$generated_qr_code		 = 'BOCWA-REG'.$application_id.'A2U'.$applicant_user_id.'T'.time();
	
	/*** GENERATION OF BOCWA REGISTRATION NUMBER ***/
	
	// $getShortNameSubdivision =  custom_user_short_name_fun($subdivision_code);  // ---------come from custom_user module  
	// $getRegistrarCode		 =	get_registration_code($area_code);              //-------------miscellaneous module
	// $registrarCode 			 =  substr($getRegistrarCode, -2);
	// $reg_query	=	db_query("select max (NULLIF(substr(registration_number,11,6), '') :: integer) as serial_num  from l_bocwa_registration_master where loc_e_subdivision='".$alc_subdiv_code."'");
	// $reg_query_result		= $reg_query->fetchAssoc();
	
	// $reg_code				= (int)$reg_query_result['serial_num'];
			
	
	


	/*** Update Status of BOCWA Master Table ***/ 
	$bocwa_amendment_number='AMEND-'.$amendment_parent_id.'-00'.$application_id.'-'.$applicant_user_id;

	$update_status  =  db_update('l_bocwa_registration_master');
	$update_status->fields(array('bocwa_qr_code' => $generated_qr_code, 'amendment_date' => date("Y-m-d"),'amendment_reference_no' =>	$bocwa_amendment_number, 'status' => 'I'));
	$update_status->condition('id',$application_id);
	$update_status->condition('act_id',$act_id);
	$update_status->condition('user_id',$applicant_user_id);
	$update_status->isNotNull('amendment_parent_id');
	$update_status->execute();
	

	$fetch_users_role		= db_select('users_roles', 'ur');
	$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role->leftJoin('l_custom_user_detail', 'lcud', 'ur.uid=lcud.usr_id');
	$fetch_users_role->fields('ro', array('rid'));
	$fetch_users_role->fields('lcud', array('fullname'));
	$fetch_users_role->condition('ur.uid', $alc_user_id);
	$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc();

	$remark_by_role			=	$fetch_users_role_data['rid'];
	$alc_full_name			=   $fetch_users_role_data['fullname'];	
	
	
	/***Insert In BOCWA Remarks Master Table ***/ 
	
		
	$fieldsApplInfo2  =  array(
		'remark_text'   		=> 'Congratulations! Amendment Certificate is issued by the Registering Authority. You can download it from the dashboard.', 
		'remark_by'   			=> $alc_user_id, 
		'remark_to' 			=> $applicant_user_id,
		'application_id' 		=> $application_id,
		'remark_date'  			=> time(),
		'remark_type' 			=> 'I',
		'remark_by_name'    	=> $alc_full_name,
		'remark_field_title' 	=> $fieldname,
		'remark_by_role' 		=> $remark_by_role,
		
	);
	
	db_insert('l_bocwa_remark_master')->fields($fieldsApplInfo2)->execute();
	//sms
	$applicant_mobile_no = get_mobile_number($applicant_user_id);
	if($remark_type == 'I'){
		$msg = 'Congratulations! Your BOCWA registration certificate is issued by ALC(Govt. of WB). Login to download the certificate from dashboard (wblc.gov.in)';	
	}else{
		$msg = 'Your BOCWA application is viewed and checked by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';
	}
	
	send_sms_for_user_alert($applicant_mobile_no, $msg);

	$message = drupal_set_message_custom(t('BOCWA Registration Certificate has been successfully generated'));
	$form_state['values']['hidden_action_message'] = $message;
	$form_state['rebuild']= TRUE;

}


function get_bocwa_remark_details($application_id,$applicant_user_id){
	
	drupal_add_js(drupal_get_path('module', 'alc').'/js/mycustom.js');
	global $base_root, $base_path, $user;
	
	$loadingImg 	= $base_root.'/sites/all/themes/jackson/images/lodinggif.gif';
	$user_id 		= $user->uid;
	$counter		= 0;
	
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
			
		$counter 		= $counter+1;
		$remark_by		= $row->remark_by_name.'&nbsp;('.$row->name.')';
		
		// get latest remark given by session user // get payment info (checking  for data insert into l_principle_epayments_data table)
			
		$getPayInfo		= db_query("SELECT COUNT(*) AS total_row FROM l_principle_epayments_data WHERE act_id=2 AND application_id = ".$application_id."")->fetchObject();
		$getId			= $getPayInfo->total_row;
		
		$getMaxData		= db_query("SELECT MAX(id) AS MAXID FROM l_bocwa_remark_master WHERE is_active=1 and application_id = ".$application_id."")->fetchObject();
		$maxId			= $getMaxData->maxid;
		if($maxId>0){
			if(($row->remark_by==$user_id) &&($maxId==$row->id) && ($getId!=0 || $getId!='')){						
				$delete	= '<span id="delBtn" class="delete-btn active-del" data-id ='.encryption_decryption_fun('encrypt',$row->id).' data-uid='.encryption_decryption_fun('encrypt',$user_id).' data-act='.encryption_decryption_fun('encrypt',2).'>&nbsp;<img style="display:none;margin-left:30px;" src="'.$loadingImg.'" alt="loading" id="loading_'.encryption_decryption_fun('encrypt',$row->id).'"></span>';
			}else{
				$delete	= '<div id="delBtn" class="delete-btn disable-del">&nbsp;</div>';
			}	
		}
		$rows[] 	= array(
			array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
			array('data' => date('dS M, Y - g:i a', $row->remark_date), 'align' => 'left', 'class' => array('odd')),
			array('data' => $row->remark_text.$call_time_applicant, 'align' => 'left', 'class' => array('odd')),
			array('data' => get_user_application_status($row->remark_type), 'align' => 'left', 'class' => array('odd')),
			array('data' => $row->remark_by_name.'&nbsp;( ' .$row->name.' )', 'align' => 'left', 'class' => array('odd')),
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
function get_call_date_time_field_amnd($form, $form_state){
	$commands 	= array();
	
	$commands[] = ajax_command_replace('#download_link_block', drupal_render($form['amend_bocwa']['download_link']));
	$commands[] = ajax_command_replace('#upload_text_block', drupal_render($form['amend_bocwa']['upload_text']));
	$commands[] = ajax_command_replace('#bocwa_signed_certificate_block', drupal_render($form['amend_bocwa']['bocwa_signed_certificate']));
	$commands[] = ajax_command_replace('#generate_certificate_block', drupal_render($form['amend_bocwa']['generate_certificate']));
	$commands[] = ajax_command_replace('#remarks_text_block', drupal_render($form['amend_bocwa']['remarks_text']));
	$commands[] = ajax_command_replace('#submit_block', drupal_render($form['amend_bocwa']['submit']));
	return array('#type' => 'ajax', '#commands' => $commands);
	
}

