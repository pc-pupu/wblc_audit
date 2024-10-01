 <?php
 
drupal_add_js(drupal_get_path('module', 'alc').'/js/mycustom.js');

function view_ismw_application_alc($form, &$form_state, $id = '',$app_user_id=''){
	
	global $base_root, $base_path, $user;
	
	$act_id = 4;
	$user_id = $user->uid;
	$application_id = encryption_decryption_fun('decrypt', $id);
	$applicant_user_id = encryption_decryption_fun('decrypt', $app_user_id);
	
	/**** Fetch ALC SUBDIVISION Code ****/
	
	$get_alc_subdivision_details = db_select('l_customuser_relation_address', 'lcra');
	$get_alc_subdivision_details -> leftJoin('users_roles', 'ur','ur.uid=lcra.user_id ');
	$get_alc_subdivision_details -> leftJoin('role', 'ro', 'ro.rid = ur.rid');
	$get_alc_subdivision_details -> fields('ro', array('name','rid'));
	$get_alc_subdivision_details ->	fields('lcra',array('sub_div_code'));
	$get_alc_subdivision_details ->	condition('lcra.user_id',$user_id);
	$get_alc_subdivision_details_result = 	$get_alc_subdivision_details->execute();
	
	if($get_alc_subdivision_details_result-> rowCount() > 0 ){
		$alc_data = $get_alc_subdivision_details_result->fetchAssoc();  
		$alc_subdivision_code = trim($alc_data['sub_div_code']);
		$alc_role_rid = $alc_data['rid']; 
		$alc_role_name = $alc_data['name']; 
	}
	
	
	/*** Get Applicant Block Code & Statuses ****/
	
	$get_ismw_application =	db_select('l_interstate_workman_master', 'liwm');
	$get_ismw_application -> fields('liwm',array('loc_e_subdivision','final_submit_status','status','registration_number', 'registration_date','iwm_qr_code','iwm_certificates_fid','identification_number', 'eodb_app_id', 'finalfees'));
	$get_ismw_application -> condition('liwm.loc_e_subdivision',$alc_subdivision_code);
	$get_ismw_application -> condition('liwm.id',$application_id);
	$get_ismw_application -> condition('liwm.user_id',$applicant_user_id);
	$get_ismw_application -> condition('liwm.final_submit_status','P');
	$get_ismw_application_result = 	$get_ismw_application->execute();

	
	if($get_ismw_application_result-> rowCount() > 0 ){
		
		$applicant_data = $get_ismw_application_result->fetchAssoc();
		$applicant_subdivsion_code = trim($applicant_data['loc_e_subdivision']);
		$ismw_registration_number = $applicant_data['registration_number'];
		$ismw_qr_code = $applicant_data['iwm_qr_code'];
		$applicant_final_submit_status = $applicant_data['final_submit_status']; 
		$applicant_status = $applicant_data['status'];
		$signed_certificate_fid = $applicant_data['iwm_certificates_fid'];
		$identification_number = $applicant_data['identification_number'];
		
		if(!empty($signed_certificate_fid)){
			
			$upload_certificates_id = db_select('file_managed','fm');
			$upload_certificates_id -> fields('fm',array());
			$upload_certificates_id -> condition('fid',$signed_certificate_fid);
			$upload_certificates_file = $upload_certificates_id->execute()->fetchAssoc();
			
			if(!empty($upload_certificates_file)){ 
				
					  $url = explode('//',$upload_certificates_file['uri']);
					  $upload_certificates_file_url = $url[1];
					  $upload_certificates_file_name = $upload_certificates_file['filename'];
				}
			
		}
		
		if($applicant_status == 'B'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i>CURRENT STATUS: Back for rectification</h4>
								<p>Application is sent back for rectification. After modification by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p>
								</div>';
		}
		if($applicant_status == 'BI'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i>CURRENT STATUS:  Back to inspector for rectification</h4><p>Application is sent back to Inspector for further verification. After verification, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
		}
		if($applicant_status == 'I'){
			
			$certificateDownloadLink = l(t('<i class="fa fa-download" aria-hidden="true"></i>&nbsp; Signed ISMW Registrtation Certificate'),$GLOBALS['base_url'].'/sites/default/files/'.$upload_certificates_file_url, array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn')))).'<br/><br/>';
			$current_status = '<div class="callout callout-success"><h4>CURRENT STATUS: Certificate (FORM-II) is issued. </h4><p>Please recheck the Registration Certificate after Generation.</div>';
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
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS:  FORM-I uploaded & application successfully submitted</h4><p>FORM-I is submitted by the Applicant. After verification of uploaded FORM-I, Registration Certificate can be generated now or FORM-I can be sent for back to rectification.</p></div>';
		} 
		if($applicant_status == 'U'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> 
									CURRENT STATUS: Back for Rectification(FORM-I)</h4><p>FORM-I is sent back for rectification. After modification by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p>
								</div>';
		} 
		
	}
	
	/*** Get REMARK Details ****/
	
	$get_remark = db_select('l_interstate_remark_master', 'lirm');
	$get_remark->fields('lirm', array('remark_field_text','remark_text'));
	$get_remark->condition('lirm.remark_to', trim($applicant_user_id));
	$get_remark->condition('lirm.application_id', trim($application_id));
	$get_remark->condition('lirm.remark_by', trim($applicant_user_id), '!=');
	$get_remark->condition('lirm.is_active', 1);
	$get_remark->orderBy('lirm.id','DESC');
	$get_remark->range(0, 1);
	$get_remark_result = $get_remark->execute();
			
	$remark_details = $get_remark_result->fetchAssoc(); 
	
	
	$info = check_final_submit_data(encryption_decryption_fun('encrypt', 4), encryption_decryption_fun('encrypt', $application_id));	
	if(!empty($info) && $info['remark_type']=='S'){
		$final_submit_date = $info['remark_date'];
		$today = time();						
		$datediff = $today - $final_submit_date;	
		$diff = round($datediff / (60 * 60 * 24));		
	}

	
	if($alc_subdivision_code == $applicant_subdivsion_code || $user->uid == 1094){   /** Checking for If the Application can be viewed by the ALC of the Subdivision ****/
	
		$form['#attributes'] = array('enctype' => 'multipart/form-data');
		
		$form['ismw_reg']['markup_data'] = array(
					'#markup' => view_ismw_registration_form_func_alc($application_id,$applicant_user_id),
					'#type' => 'markup',
					);											
		$form['ismw_reg']['base_url'] 	= array(
					  '#type' 	=> 'hidden',
					  '#value' 	=> $base_root.$base_path,				  
					);																			
		$form['ismw_reg']['show_status'] = array(
					'#type' => 'markup',
					'#markup' => $current_status,
					);
		$form['ismw_reg']['act_id'] = array(
					'#type' => 'hidden',
					'#default_value' => !empty($act_id) ? $act_id: '',
					'#attributes' => array('readonly' => 'readonly'),
					);			
		$form['ismw_reg']['caf_id_no'] = array(
					'#type' => 'hidden',
					'#value' => !empty($applicant_data['identification_number'])?$applicant_data['identification_number']:0,				  
					);
		$form['ismw_reg']['eodb_app_id'] = array(
					'#type' => 'hidden',
					'#value' => !empty($applicant_data['eodb_app_id'])?$applicant_data['eodb_app_id']:0,				  
					);																						
		$form['ismw_reg']['finalfees'] = array(
					'#type' => 'hidden',
					'#value' => !empty($applicant_data['finalfees'])?$applicant_data['finalfees']:0,				  
					);
		$form['ismw_reg']['registration_number'] = array( 
					'#type' => 'hidden',
					'#value' => !empty($applicant_data['registration_number'])?$applicant_data['registration_number']:0,
					);
		$form['ismw_reg']['registration_date'] = array(
					'#type' => 'hidden',
					'#value' => !empty($applicant_data['registration_date'])?$applicant_data['registration_date']:0,
					);
		$form['ismw_reg']['ismw_qr_code'] = array(
					'#type' => 'hidden',
					'#value' => !empty($applicant_data['iwm_qr_code'])?$applicant_data['iwm_qr_code']:0,
					);	
					
		if($diff <=30){	 /*** DEEMED **/	
			if ($applicant_status != 'B' && $applicant_status != 'V' && $applicant_status != 'I' && $applicant_status != 'VA' && $applicant_status != 'T' && $applicant_status != 'U' ){
			
			$form['ismw_reg']['hidden_action_message'] = array(
						'#prefix' => '<div class="box box-default box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">ACTIONS AND REMARKS</h3></div><div class="box-body"><span style="margin:0; width:100%;">',
						'#suffix' => '</span>',
						'#type' => 'markup',
						'#markup' => isset($form_state['values']['hidden_action_message']) ? $form_state['values']['hidden_action_message'] : ''
						);
																
			$form['ismw_reg']['fieldname'] = array(
						'#title' => 'Field Name',
						'#type' => 'hidden',
						'#attributes' => array('id' => 'ismw_field_name_alc'),
						'#default_value' => !empty($remark_details['remark_field_text']) ? $remark_details['remark_field_text'] : '',
						);
																
			$form['ismw_reg']['applicant_user_id'] = array(
						'#type' => 'hidden',
						'#attributes' => array('readonly' => 'readonly'),
						'#default_value' => ($applicant_user_id!='') ? encryption_decryption_fun('encrypt',$applicant_user_id): '',
						);
																	
			$form['ismw_reg']['application_id'] = array(
						'#type' => 'hidden',
						'#default_value' => ($application_id!='') ? encryption_decryption_fun('encrypt',$application_id ): '',
						'#attributes' => array('readonly' => 'readonly'),
						);
																
			$form['ismw_reg']['identification_number'] = array(
						'#type' => 'hidden',
						'#default_value' => ($identification_number!='') ? encryption_decryption_fun('encrypt',$identification_number ): '',
						'#attributes' => array('readonly' => 'readonly'),
						);
																	
			$form['ismw_reg']['remark_type'] = array(
						'#prefix' => '<div class="col-md-3">&nbsp;</div><div class="form-custom col-md-6"><label class="input">',
						'#suffix' => '<i></i></label>',
						'#title' => 'Please Select Actions',
						'#type' => 'select',
						'#required' =>  TRUE,
						'#id' => 'remark_type_id',
						'#options' => actions_dropdown_by_alc_act($applicant_status,'Y',$act_id),
						'#attributes' => array('onchange' => 'getRemarkTypeAction(this.value)', 'class'=>array('form-control')),
						'#ajax' => array(
										'event' => 'change',
										'method' => 'append',
										'wrapper' => 'content-wrapper',
										'effect' => 'fade',			
										'callback' => 'get_call_date_time_field',
										'progress' => array('type'=> 'throbber'),
									  ),
						);
			
			
					
			$value_remark_type = isset($form_state['values']['remark_type']) ? $form_state['values']['remark_type'] : '';
																									
			$form['ismw_reg']['generate_certificate'] = array(
						'#prefix' => '<div id="generate_certificate_block">',
						'#suffix' => '</div>'
						);
																							
																							
			$form['ismw_reg']['download_link'] = array(
						'#prefix' => '<div id="download_link_block">',
						'#suffix' => '</div>'
						);
			
			$form['ismw_reg']['ismw_signed_certificate'] = array(
					  '#prefix' => '<div id="ismw_signed_certificate_block">',
					  '#suffix' => '</div>'
					);																
			
			$form['ismw_reg']['remarks_text'] = array(
					  '#prefix' => '<div id="remarks_text_block">',
					  '#suffix' => '</div>'
					);
																		
			$form['ismw_reg']['submit'] = array(
					  '#prefix' => '<div id="submit_block">',
					  '#suffix' => '</div>'
					);	
																
			if($value_remark_type == 'I'){
			
				if(!empty($ismw_registration_number) && !empty($ismw_qr_code)){
					
					/*$form['ismw_reg']['download_link'] = array(
											'#prefix' => '<div id="download_link_block">',
											'#markup' => l(t('<i class="fa fa-download" aria-hidden="true"></i>
Download ISMW Registration Certificate'),'generate-ismw-certificate/'.encryption_decryption_fun('encrypt',$ismw_qr_code), array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn')))),
											'#suffix' => '</div><div class="clear"></div>',
											
											);
				
				 	$form['ismw_reg']['ismw_signed_certificate'] = array(
																'#title'			=> 'Upload Signed ISMW Registration Certificate :-',
																'#type' 			=> 'managed_file',
																'#upload_validators'=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																'#upload_location' 	=> 'public://upload/ISMW_Registration_Certificate/',
																'#process' 			=> array('import_my_file_element_process'),
																'#attributes' 		=> array('autocomplete' => 'off','class' => array('form-group')),
																'#prefix'			=> '<div id="ismw_signed_certificate_block">',
																'#suffix'			=> '</div>',
																'#required' 		=>  TRUE,
																);
																									
					$form['ismw_reg']['remarks_text']		= array(
																'#prefix' 			=> '<div id="remarks_text_block"><label class="input">',
																'#suffix' 			=> '</label></div>',
																'#title'			=> 'Remarks',
																'#type'				=> 'textarea',
																'#id'				=> 'remarks_text_id',
																'#rows'				=>  2,
																'#required' 		=>  TRUE,
																);*/
																									
					$form['ismw_reg']['submit'] 			= array (
															  '#prefix' 			=> '<div id="submit_block">',
															  '#suffix' 			=> '</div></div><div class="col-md-3">&nbsp;</div></div></div>',	
															  '#type' 				=> 'submit',
															  '#id'					=> 'ismw_empLicSubmitId',
															  '#attributes' 		=> array('class' => array('btn btn-primary pull-left acremark-btn')),
															  '#value' 				=> 'Submit'
														  
														);
				
				}else{
					
				   $form['ismw_reg']['generate_certificate']= array (
																		  '#prefix' 		=> '<div id="generate_certificate_block">',
																		  '#suffix' 		=> '</div>',
																		  '#type' 			=> 'submit',
																		  '#attributes' 	=> array('class' => array('btn btn-primary pull-left acremark-btn')),
																		  '#value' 			=> 'Generate ISMW Registration Certificate',
																		  //'#submit'			=> array('generation_ismw_registration_number'),
																		);
				}
		    }																					
			
				if($value_remark_type!= 'I'){
																													
					$form['ismw_reg']['remarks_text']			= array(
																	'#prefix' 			=> '<div id="remarks_text_block"><label class="input">',
																	'#suffix' 			=> '</label></div>',
																	'#title'			=> 'Remarks',
																	'#type'				=> 'textarea',
																	'#id'				=> 'remarks_text_id',
																	'#rows'				=>  2,
																	'#required' 		=>  TRUE,
																	);
					
					
					
					$form['ismw_reg']['submit'] 				= array (
																	'#prefix' 		=> '<div id="submit_block">',
																	'#suffix' 		=> '</div></div><div class="col-md-3">&nbsp;</div></div></div>',
																	'#type' 			=> 'submit',
																	'#attributes' 	=> array('class' => array('btn btn-primary pull-left acremark-btn')),
																	'#value' 			=> 'Submit'
																		
																		);	
																											
				}
		    }
		}else{
			common_deemed_update_status(encryption_decryption_fun('encrypt', 4),encryption_decryption_fun('encrypt', $application_id),encryption_decryption_fun('encrypt', $applicant_user_id));
					
		}
			
		$form['ismw_reg']['markup_data_1']				= array(	
															'#prefix'			=>'<div class="box box-default box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">REMARK SUMMARY</h3></div><div class="box-body"><div class="feedback-scroll">',
															'#markup' 			=> get_ismw_remark_details($application_id,$applicant_user_id),
															'#suffix' 			=> '</div></div></div>',
															);
																									
	}else{
		
		$message = 'You are not permitted to view this application.';
		drupal_set_message(t($message), 'error');	
		
	}
	
	return $form;
	
}

function view_ismw_registration_form_func_alc($application_id='' ,$applicant_user_id= ''){ 

	global $base_root, $base_path, $user;
	
	if(is_numeric($applicant_user_id) && is_numeric($application_id)){ 
		
		if(!empty($application_id)){
			
			global $base_root, $base_path, $user;
	
			$master_part_ismw_reg = db_select('l_interstate_workman_master', 'liwm');
			$master_part_ismw_reg->fields('liwm', array());		
			$master_part_ismw_reg->condition('liwm.user_id', $applicant_user_id);
			$master_part_ismw_reg->condition('liwm.id', $application_id);
			$master_part_ismw_reg_result = $master_part_ismw_reg->execute();
			
			if($master_part_ismw_reg_result->rowCount() > 0){
				
				$content					= 	$master_part_ismw_reg_result->fetchAssoc();
				
				if($content['iwm_certificates_fid']!='' && $content['status'] == 'I'){
					$upload_certificates_id 		= db_select('file_managed','fm');
					$upload_certificates_id			-> fields('fm',array());
					$upload_certificates_id			-> condition('fid',$content['iwm_certificates_fid']);
					$upload_certificates_file		= $upload_certificates_id-> execute()->fetchAssoc();
		
					if(!empty($upload_certificates_file)){ 
					  $url									= explode('//',$upload_certificates_file['uri']);
					  $upload_certificates_file_url			= $url[1];
					  $upload_certificates_file_name		= $upload_certificates_file_name['filename'];
					  $certificateDownloadLink				= l(t('<i class="fa fa-download" aria-hidden="true"></i>&nbsp; Signed ISMW Registration Certificate'),$GLOBALS['base_url'].'/sites/default/files/'.$upload_certificates_file_url, array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn'))));
					}
				}
				
							
				/** Fetch CLRA Registration Details ***/
			
				$getCLRADetails = db_select('l_clra_registration_master', 'master');
				$getCLRADetails	->fields('master', array('user_id', 'id'));	
				$getCLRADetails	->condition('master.registration_number',$content['clra_registration_number']);
				$getCLRADetails->orderBy('master.id','DESC');
				$getCLRAData	= $getCLRADetails->execute()->fetchAssoc();
				
				$clra_registration_date 	= !empty($content['clra_registration_date']) ? date('dS M, Y',strtotime($content['clra_registration_date'])) : "Not Applicable";
				$clra_registration_number	= !empty($content['clra_registration_number'])? $content['clra_registration_number'] : '';
				$clraRegLink				= !empty($content['clra_registration_number']) && !empty($getCLRAData) ? '&nbsp;&nbsp;'.l('<span class="badge bg-blue"><i class="fa fa-eye"></i> View More of Registration</span>', 'alc-visible-applications/'.encryption_decryption_fun('encrypt', $getCLRAData['id']).'/'.encryption_decryption_fun('encrypt', $getCLRAData['user_id']), array('html' => TRUE, 'target' => '_blank')) : "<span class='badge bg-blue'>Not Available in wblc.gov.in</span>";
			}
			
			/** Fetch Address from Miscellaneous Module ****/
			
			$table	 				= 'l_interstate_workman_master';
			$type	 				= 'ismw';
			$estArr	 				= array('loc_e_dist','loc_e_subdivision','loc_e_areatype','loc_e_areatype_code','loc_e_vill_ward','loc_e_ps','loc_e_pin_number');
			$postalArr	 			= array('e_postal_dist','e_postal_subdivision','e_postal_areatype','e_postal_areatype_code','e_postal_vill_ward','e_postal_ps','e_postal_pin_number');
			
			$establishment_address 	= get_full_address($table, $type, $application_id, $estArr);
			$postal_address 		= get_full_address($table, $type, $application_id, $postalArr);
			
			/*** Principal Employer Details ****/
		
			$ismw_act_emp 		= 	db_select('l_interstate_employer_master', 'liem');
			$ismw_act_emp		->	fields('liem', array());
			$ismw_act_emp		-> 	condition('liem.designation', 'PrincipalEmployer');
			$ismw_act_emp		->  condition('liem.application_id', $application_id);
			$ismw_act_emp		->	condition('liem.user_id', $applicant_user_id);	
			$ismw_act_emp		->	condition('liem.is_active', 1);
			$ismw_act_emp_result = 	$ismw_act_emp->execute()->fetchAssoc(); 
			
			$principal_emp_guardian_name	= !empty($ismw_act_emp_result['guardian_name']) ? $ismw_act_emp_result['guardian_name'] : 'Not Applicable';
			
			if(!empty($ismw_act_emp_result) && $ismw_act_emp_result['country_code'] == 1){
				
				$table	 				= 'l_interstate_employer_master';
				$type	 				= 'sub-table';
				$emp_address			= '<br/>'.get_full_address('l_interstate_employer_master', 'sub-table', $ismw_act_emp_result['id'], array('district_code','subdivision_code','area_type','area_type_code','village_code','police_station','pin','state_code'));
				if($ismw_act_emp_result['gender']=='M'){
					$gender = "Male";
				}elseif($ismw_act_emp_result['gender']=='F'){
					$gender = "Female";
				}elseif($ismw_act_emp_result['gender']=='O'){
					$gender = "Transgender";
				} 
				
			}
			
		/*** DIRECTOR / PARTNER DETAILS ****/
	
		$ismw_dir_partner = db_select('l_interstate_employer_master', 'lidpm');	
		$ismw_dir_partner -> fields('lidpm', array('id'));
		$ismw_dir_partner -> condition('lidpm.application_id', $application_id);	
		$ismw_dir_partner -> condition('lidpm.user_id', $applicant_user_id);
		$ismw_dir_partner -> condition('lidpm.is_active', 1);
		$db_or = db_or();
		$db_or -> condition('lidpm.designation', 'director', '=');
		$db_or -> condition('lidpm.designation', 'partner', '=');
		$ismw_dir_partner -> condition($db_or);
		$ismw_dir_partner -> orderBy('lidpm.id', 'DESC');
		$ownership_details = $ismw_dir_partner->execute()->fetchAll(); 
		
		$ResultDirectorPartner 	= count($ownership_details);
	
		if($ResultDirectorPartner!='' && $ResultDirectorPartner>0){
			$NumberOfDPs	= 'Number of Director / Partner:  &nbsp;'.$ResultDirectorPartner .'&nbsp;&nbsp;'.l('<span class="badge bg-olive margin"><i class="fa fa-eye"></i> View Details</span>','view-details/directorpartner-info/'.encryption_decryption_fun('encrypt', $application_id).'/'.encryption_decryption_fun('encrypt', 4),array('html'=> TRUE,'attributes'=> array('target'=>'_blank')));	
		}else{
			$NumberOfDPs	= 'No Director/Partner Added';
		}
			
			/*** MANAGER DETAILS ***/
		
		$ismw_manager 				= 	db_select('l_interstate_employer_master', 'lidpm');	
		$ismw_manager				->	fields('lidpm', array());
		$ismw_manager				->  condition('lidpm.application_id', $application_id);	
		$ismw_manager				->	condition('lidpm.user_id', $applicant_user_id);
		$ismw_manager				->	condition('lidpm.is_active', 1);
		$ismw_manager				->	condition('lidpm.designation', 'manager', '=');
		$ismw_manager				->	orderBy('lidpm.id', 'DESC');
		$ismw_manager_details		= 	$ismw_manager->execute()->fetchAll(); 
		
		$ResultManager 		= count($ismw_manager_details);
		
		if($ResultManager!='' && $ResultManager>0){
			$NumberOfManager	= 'Number of Manager / Person Responsible:  &nbsp;'.$ResultManager .'&nbsp;&nbsp;'.l('<span class="badge bg-olive margin"><i class="fa fa-eye"></i> View Details</span>','view-details/incharge-info/'.encryption_decryption_fun('encrypt', $application_id).'/'.encryption_decryption_fun('encrypt', 4),array('html'=> TRUE,'attributes'=> array('target'=>'_blank')));	
		}else{
			$NumberOfManager	= 'No Manager / Person Responsible Added';
		}
			
			/**** NATURE OF WORK CODE STARTS*****/
		
			$get_nature_work = 	db_select('l_interstate_nature_of_work', 'lcnw');
			$get_nature_work ->	leftJoin('contractor_works', 'cw', 'lcnw.e_nature_of_work=cw.contractor_work_id');
			$get_nature_work ->	fields('cw',array('cont_work_name','contractor_work_id'));
			$get_nature_work ->	condition('lcnw.user_id',$applicant_user_id);
			$get_nature_work ->	condition('lcnw.act_id',4);
			$get_nature_work ->	condition('lcnw.application_id',$application_id);
			$get_nature_work ->	condition('cw.is_default',1);
			$query_get_each_nature_work = $get_nature_work->execute(); 
		
			$prefix						= '';	
			$natureOfWrkString 			= '';
			$lastsuffix 				= '';
			
			if(!empty($query_get_each_nature_work)){
				$total = $query_get_each_nature_work->rowCount();
				
				if($total > 1 ) $lastsuffix 	= ', '; else $lastsuffix 	= '';
				
				foreach($query_get_each_nature_work as $listData){ 
				
						
						if($listData->contractor_work_id == 28){
							/** FOR FIELD FOR OTHER OPTION OF NATURE WORK ****/
			
							$get_other_nature_work 		= db_select('l_interstate_nature_of_work', 'lcnw');
							$get_other_nature_work		->leftJoin('contractor_works', 'cw', 'lcnw.e_nature_of_work=cw.contractor_work_id');
							$get_other_nature_work		->fields('cw',array('cont_work_name','contractor_work_id'));
							$get_other_nature_work		->condition('cw.is_default',0);
							$get_other_nature_work		->condition('lcnw.user_id',$applicant_user_id);
							$get_other_nature_work		->condition('cw.act_id',4);
							$get_other_nature_work		->condition('lcnw.application_id',$application_id);
							$other_nature_work_data 	= $get_other_nature_work->execute(); 
							
							if( $other_nature_work_data->rowCount() > 0 ){ 
								
								$obj3 = $other_nature_work_data->fetchAssoc(); 
								
								$other_nature_wrk_id 	= $obj3['contractor_work_id'];
								$other_nature_wrk 		= $obj3['cont_work_name'];
								
								$natureOfWrkString 		.= $lastsuffix.$other_nature_wrk;
								
							}
							
						}else{ 								
							$natureOfWrkString 			.= $prefix.$listData->cont_work_name;
							$prefix  	 				 = ', ';
						}
							
				}
			}
			
			/**** CONTRACTOR SECTION ****/
			
			$ismw_con 				= 	db_select('l_interstate_contractor_master', 'licm');
			$ismw_con				->	fields('licm', array());
			$ismw_con				->  condition('licm.application_id', $application_id);	
			$ismw_con				->	condition('licm.user_id', $applicant_user_id);
			$ismw_con				->	condition('licm.contractor_status', 1);
			$ismw_con				->	orderBy('licm.id', 'DESC');
			$ismw_con_res			= $ismw_con->execute()->fetchAll();
			
			$contractors			=	array();
			foreach ($ismw_con_res as $contractors_row) {
				$contractors[]	=	$contractors_row;
			}
			
			$ResultContractor = count($ismw_con_res);
			
			if($ResultContractor!='' && $ResultContractor>0){
				$NumberOfContractor	= 'Number of Contractor / Person Responsible:  &nbsp;'.$ResultContractor .'&nbsp;&nbsp;'.l('<span class="badge bg-olive margin"><i class="fa fa-eye"></i> View Details</span>','view-details/contractor-info/'.encryption_decryption_fun('encrypt', $application_id).'/'.encryption_decryption_fun('encrypt', 4),array('html'=> TRUE,'attributes'=> array('target'=>'_blank')));	
			}else{
				$NumberOfContractor	= 'No Contractor Information Added';
			}
			
			/**** Uploaded Documents ****/
				
			$ismw_docs = db_select('l_documents_upload', 'ldu');
			$ismw_docs->fields('ldu', array());
			$ismw_docs->condition('ldu.user_id', trim($applicant_user_id)); 
			$ismw_docs->condition('ldu.application_id', trim($application_id));
			$ismw_docs->condition('ldu.act_id', 4);
			$ismw_docs_result = $ismw_docs->execute();
			
			$content_docs	=	$ismw_docs_result->fetchAssoc();
			
			$master_part_ismw_encrypted_docs = db_select('l_encrypted_uploaded_documents', 'leud');
			$master_part_ismw_encrypted_docs->fields('leud', array());
			$master_part_ismw_encrypted_docs->condition('leud.user_id', $user_id);
			$master_part_ismw_encrypted_docs->condition('leud.act_id', '4');
			$master_part_ismw_encrypted_docs->condition('leud.application_id', $application_id);
			$master_part_ismw_encrypted_docs_result = $master_part_ismw_encrypted_docs->execute();
			
			$content_encrypted_docs	=	$master_part_ismw_encrypted_docs_result->fetchAssoc();
			$enc_doc_array = array('TL'=> '','PD'=> '','AOA'=> '','MOA'=> '','MOC'=>'','FL'=> '','AC'=> '','PC'=> '','BB'	=> '','SC'	=> '','IC'	=> '','AP'=> '','ODSC'=> '','CR'=> '','WO'=> '','ORC'=> '','CH'=>'','AMP'=>'','CIA'=>'','DSC'=>'','FI'=>'');
			$pdficonlink = '<img title="View Documents" alt="View Documents" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/pdf.png"> &nbsp;';
		
			$query 	= db_select('l_encrypted_uploaded_documents','leud')->fields('leud',array())->condition('application_id',$application_id)->condition('act_id','4')->condition('status','1')->condition('user_id',$applicant_user_id);
			$query_result 	= $query->execute();
			$document_type_arr=array();
			if($query_result->rowCount() > 0){
				foreach($query_result->fetchAll() as $data ){
					$enc_doc_array[$data->document_type_code]['content'] = 'view_documents/'.encryption_decryption_fun('encrypt',$data->id);
				}
			}
	
			$global_link = $base_root.$base_path.'sites/default/files/upload/';
			
			$get_remark = db_select('l_interstate_remark_master', 'lirm');
			$get_remark->fields('lirm', array('remark_field_text'));
			$get_remark->condition('lirm.remark_to', trim($applicant_user_id));
			$get_remark->condition('lirm.application_id', trim($application_id));
			$get_remark->condition('lirm.remark_by', trim($applicant_user_id), '!=');
			$get_remark->condition('lirm.is_active', 1);
			$get_remark->orderBy('lirm.id','DESC');
			$get_remark->range(0, 1);
			$get_remark_result = $get_remark->execute();
			
			$content_5 = $get_remark_result->fetchAssoc(); 
			
					
			$remark_field_text_arr = explode(',', $content_5['remark_field_text']);
			
			foreach ($remark_field_text_arr as $fieldname){ 
				if($fieldname == 'e_name'){ $e_name_ck = "checked='checked'";}
				if($fieldname == 'est_type'){ $est_type_ck = "checked='checked'";}
				if($fieldname == 'loc_e_name'){ $loc_e_name_ck = "checked='checked'";}
				if($fieldname == 'e_postal_address'){ $e_postal_address_ck = "checked='checked'";}
				if($fieldname == 'emp_name'){ $emp_name_ck = "checked='checked'";}
				if($fieldname == 'gender'){ $gender_ck = "checked='checked'";}
				if($fieldname == 'emp_guardian_name'){ $emp_guardian_name_ck = "checked='checked'";}
				if($fieldname == 'emp_address'){ $emp_address_ck = "checked='checked'";}	
				if($fieldname == 'modify_manager'){ $modify_manager_ck = "checked='checked'";}	
				if($fieldname == 'modify_contractors'){ $modify_contractors_ck = "checked='checked'";}
				if($fieldname == 'modify_director_partner'){ $modify_director_partner_ck = "checked='checked'";}	
				if($fieldname == 'e_nature_of_work'){ $e_nature_of_work_ck = "checked='checked'";}
				if($fieldname == 'clra_registration_number'){ $clra_registration_number_ck = "checked='checked'";}
				if($fieldname == 'clra_registration_date'){ $clra_registration_date_ck = "checked='checked'";}	
				if($fieldname == 'max_num_migrant_wrkmen'){ $max_num_migrant_wrkmen_ck = "checked='checked'";}	
					
				if($fieldname == 'trade_license_file'){ $trade_license_file_ck = "checked='checked'";}	
				if($fieldname == 'article_of_assoc_file'){ $article_of_assoc_file_ck = "checked='checked'";}
				if($fieldname == 'factory_license_file'){ $factory_license_file_ck = "checked='checked'";}	
				if($fieldname == 'certificate_other_states'){ $certificate_other_states_file_ck = "checked='checked'";}
				if($fieldname == 'other_related_documents'){ $other_related_documents_ck = "checked='checked'";}
				if($fieldname == 'signed_pdf_file'){ $form_1_signed_pdf_file_ck = "checked='checked'";}
				if($fieldname == 'backlog_certificate'){ $backlog_certificate_ck = "checked='checked'";}
			}
			
		}
		if($content['gender_pe'] == 'M'){
			$gender = 'Male';
		}elseif($content['gender_pe'] == 'F'){
			$gender = 'Female';
		}elseif($content['gender_pe'] == 'O'){
			$gender = 'Others';
		}
		$output = '';
	
		$output .= '<div class="box box-primary box-solid">
					<div class="box-header with-border">
					  <h3 class="box-title">Inputs are provided by Contractor [For Verification]</h3>
					</div>
					<div class="box-body">
						<div class="feedback-scroll">
							<table class="table table-bordered">
								<tbody>
								<tr>
									<th style="width:6%">Sl. No.</th>
									<th style="width:40%">Parameters</th>
									<th>Inputs</th>
									<th style="width:10%">Verified?</th>
								</tr>
								<tr>
									<td>1.(a)</td>
									<td>Establishment Name</td>
									<td>'.$content['e_name'].'</td>
									<td><input type="checkbox" id="e_name" class="ismw_verified_alc"  '.$e_name_ck.' /></td>
								</tr>
								<tr>
									<td>1.(b)</td>
									<td>Establishment Type</td>
									<td>'.ucfirst($content['est_type']).'</td>
									<td><input type="checkbox" id="est_type" class="ismw_verified_alc"  '.$est_type_ck.' /></td>
								</tr>
								<tr>
									<td>1.(c)</td>
									<td>Location of the Establishment</td><td>'.$content['loc_e_name'].'<br/>'.$establishment_address.' </td>
									<td><input type="checkbox" id="loc_e_name" class="ismw_verified_alc" '.$loc_e_name_ck.' /></td>
								</tr>
								<tr>
									<td>2. </td>
									<td>Postal Address of the Establishment </td>
									<td>'.$content['e_postal_address'].'<br/>'.$postal_address.'</td>
									<td><input type="checkbox" id="e_postal_address" class="ismw_verified_alc" '.$e_postal_address_ck.' /></td>
								</tr>
								<tr>
									<td>3.(a)</td>
									<td>Name of the Principal Employer <br/> Contact Number </td>
									<td>'.$ismw_act_emp_result['name'].'<br/> '.$ismw_act_emp_result['contact_number'].'</td>
									<td><input type="checkbox" id="emp_name" class="ismw_verified_alc" '.$emp_name_ck.'  /></td>
								</tr>
								<tr>
									<td>3.(b)</td>
									<td>Gender of the Principal Employer</td>
									<td>'.$gender.'</td>
									<td><input type="checkbox" id="gender" class="ismw_verified_alc" '.$gender_ck.'  /></td>
								</tr>
								<tr>
									<td>3.(c)</td>
									<td>Father/Husband name of the Principal Employer </td>
									<td>'.$principal_emp_guardian_name .'</td>
									<td><input type="checkbox" id="emp_guardian_name" class="ismw_verified_alc" '.$emp_guardian_name_ck.'  /></td>
								</tr>
								<tr>
									<td>3.(d)</td>
									<td>Address of the Principal Employer</td>
									<td>'.$ismw_act_emp_result['address_line'].'<br/>'.$emp_address.'</td>
									<td><input type="checkbox" id="emp_address" class="ismw_verified_alc" '.$emp_address_ck.'  /></td>
								</tr>
								
								<tr>
								  <td>4. </td>
								  <td>Director&#146;s / Partner&#146;s Information </td>
								  <td style="width:40%">'.$NumberOfDPs.'</td>
								  <td><input type="checkbox" id="modify_director_partner" class="ismw_verified_alc" '.$modify_director_partner_ck.' /></td>
								</tr> 
								<tr>
								  <td>5.</td>
								  <td>Name(s) and Address(es) of the manager(s) or person(s) responsible for the supervision and control of the establishment </td>
								  <td style="width:40%">'.$NumberOfManager.'</td>
								  <td><input type="checkbox" id="modify_manager" class="ismw_verified_alc" '.$modify_manager_ck.' /></td>
								</tr>
								<tr>
									<td colspan="4" style="font-weight:700; text-transform:uppercase; font-size:16px;"><strong>6. Number and Date of Registration of the establishment Under the Contract Labour (R&A) Act,1970</strong></td>
								</tr>
								<tr>
									<td>6.(a)</td>
									<td>Registration Number</td>
									<td>'.$clra_registration_number.'&nbsp;&nbsp;'.$clraRegLink.'</td>
									<td><input type="checkbox" id="clra_registration_number" class="ismw_verified_alc" '.$clra_registration_number_ck.' /></td>
								</tr>
								<tr>
									<td>6.(b)</td>
									<td>Date of Registration</td>
									<td>'.$clra_registration_date.'</td>
									<td><input type="checkbox" id="clra_registration_date" class="ismw_verified_alc" '.$clra_registration_date_ck.' /></td>
								</tr>
								
								<tr>
									<td>7.</td>
									<td>Nature of Work</td>
									<td>'.$natureOfWrkString .'</td>
									<td><input type="checkbox" id="e_nature_of_work" class="ismw_verified_alc" '.$e_nature_of_work_ck.'/></td>
								</tr>
								
								<tr>
								  <td>8. </td>
								  <td>Particulars of Contractors and Migrant Workmen</td>
								  <td style="width:40%">'.$NumberOfContractor.'</td>
								  <td><input type="checkbox" id="modify_contractors" class="ismw_verified_alc" '.$modify_contractors_ck.' /></td>
								</tr>';
					$output	.= ' <tr>
									<td>9.</td>
									<td>Maximum number of migrant workmen are to be employed on any day through each contractor</td>
									<td><font color="#F00">**&nbsp;</font>'.$content ['max_num_migrant_wrkmen'].'
										&nbsp;&nbsp; <font color="#006600"> <b>[ FEES IS CALCULATED BASED ON THIS VALUE ] </b></font>
									</td>
									<td><input type="checkbox" id="max_num_migrant_wrkmen" class="ismw_verified_alc" '.$max_num_migrant_wrkmen_ck.' /></td>
								</tr>
								<tr>
									<td colspan="4"><strong>10. DOCUMENTS UPLOAD</strong></td>
								</tr>
								<tr>
									<td>10.(a)</td>
									<td>Trade License</td>';
									
							if($enc_doc_array['TL']['content'] !=''){
									
									
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['TL']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['trade_license_file'])){
											 
											$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/trade_license/'.$content_docs['trade_license_file'].'">
															<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png">&nbsp;&nbsp;
															<span class="viewfileins" id="trade_license_file">
															<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
															</span>
															<div id="fileins_trade_license_file"  title="Articles of Association and Memorandum of Association/Partnership Deed" style="display:none;">
															<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/trade_license/'.$content_docs['trade_license_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
															</div>
														</a>
															
														</td>';
											   
										}else {
											
											$output .=  '<td>No Document Uploaded</td>';
											
										}
									$output .=  '<td><input type="checkbox" id="trade_license_file" class="ismw_verified_alc" '.$trade_license_file_ck.' /></td>
									
								</tr>
							
							
								<tr>
									<td>10.(b)</td>
									<td>Articles of Association and Memorandum of Association/Partnership Deed</td>';
									
									if($enc_doc_array['AMP']['content'] !=''){
									
									
									$output .= '<td>'.l($pdficonlink,$enc_doc_array['AMP']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
								}elseif(!empty($content_docs['article_of_assoc_file'])){ 
									
											$output .= ' <td>
															<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/article_of_assoc/'.$content_docs['article_of_assoc_file'].'">
															<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png">&nbsp;&nbsp;
															<span class="viewfileins" id="article_of_assoc_file">
															<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
															</span>
															<div id="fileins_article_of_assoc_file"  title="Articles of Association and Memorandum of Association/Partnership Deed" style="display:none;">
															<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/article_of_assoc/'.$content_docs['article_of_assoc_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
															</div>
														</a>
														</td>';
													
									}else{
										
											$output .=  '<td>No Document Uploaded</td>';
											
									}
									$output .= '<td><input type="checkbox" id="article_of_assoc_file" class="ismw_verified_alc"'.$article_of_assoc_file_ck.' /></td>
								</tr>
			
								<tr>
									<td>10.(c)</td>
									<td>Factory License</td>';
									if($enc_doc_array['FL']['content'] !=''){
									
									
									$output .= '<td>'.l($pdficonlink,$enc_doc_array['FL']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
								}elseif(!empty($content_docs['factory_license_file'])){
										  
											$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/FactoryLicense/'.$content_docs['factory_license_file'].'">
															<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png">&nbsp;&nbsp;
															<span class="viewfileins" id="factory_license_file">
																<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
															</span>
															<div id="fileins_factory_license_file"  title="Factory License" style="display:none;">
																<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/FactoryLicense/'.$content_docs['factory_license_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
															</div>
														</a>
														</td>';
									}else{
										
										$output .=  '<td>No Document Uploaded</td>';
									}
												
									$output .= '<td><input type="checkbox" id="factory_license_file" class="ismw_verified_alc" '.$factory_license_file_ck.' /></td>
								</tr>
						  
								<tr>
									<td>10.(d)</td>
									<td>Certificate issued by Authorities from other state</td>';
								
									if($enc_doc_array['CIA']['content'] !=''){
									
									
									$output .= '<td>'.l($pdficonlink,$enc_doc_array['CIA']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
								}elseif(!empty($content_docs['certificate_other_states'])){ 
									
										$output .= '<td>
														<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/CertificateOtherStates/'.$content_docs['certificate_other_states'].'">
															<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png">&nbsp;&nbsp;
															<span class="viewfileins" id="certificate_other_states">
																<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
															</span>
															<div id="fileins_certificate_other_states"  title="Certificate issued by Authorities from other state" style="display:none;">
																<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/CertificateOtherStates/'.$content_docs['certificate_other_states'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
															</div>
														</a>
													</td>';
									}else{
										
										$output .=  '<td>No Document Uploaded</td>';
									}
									
									$output .=  '<td><input type="checkbox" id="certificate_other_states" class="ismw_verified_alc"'.$certificate_other_states_file_ck.' /></td>
									
								 </tr>
							
								 <tr>
									<td>10.(e)</td>
									<td>Documents substantiating correctness of particulars mentioned in the application</td>';
									
									 if($enc_doc_array['DSC']['content'] !=''){
									
									
									$output .= '<td>'.l($pdficonlink,$enc_doc_array['DSC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
								}elseif(!empty($content_docs['other_doc_file'])){
										  
											$output .= '<td>
															<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/OtherRelatedDocuments/'.$content_docs['other_doc_file'].'">
																<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png">&nbsp;&nbsp;
																<span class="viewfileins" id="other_doc_file">
																	<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
																</span>
																<div id="fileins_other_doc_file"  title="Other Document" style="display:none;">
																	<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/OtherRelatedDocuments/'.$content_docs['other_doc_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
																</div>
															</a>
															
														</td>';
									}else{
										
										$output .=  '<td>No Document Uploaded</td>';
									}
												
									$output .= '<td><input type="checkbox" id="other_related_documents" class="ismw_verified_alc" '.$other_related_documents_ck.' /></td>
								</tr>
								
								
								<tr>
									<td>10.(f)</td>
									<td>FORM-I</td>';
									
									 if($enc_doc_array['FI']['content'] !=''){
									
									
									$output .= '<td>'.l($pdficonlink,$enc_doc_array['FI']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
								}elseif(!empty($content_docs['form_1_signed_pdf_file'])){
										  
											$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/FORM-I/'.$content_docs['form_1_signed_pdf_file'].'">
															<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png">&nbsp;&nbsp;
															<span class="viewfileins" id="form_1_signed_pdf_file">
															<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
															</span>
															<div id="fileins_form_1_signed_pdf_file"  title="Form-I" style="display:none;">
															<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/FORM-I/'.$content_docs['form_1_signed_pdf_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
															</div>&nbsp;&nbsp;&nbsp;'.l(t('Generated FORM - I'),'ismw-generate-pdf/'.encryption_decryption_fun('encrypt', $content['id']).'/'.encryption_decryption_fun('encrypt', $content['user_id']), array('html' => TRUE,'attributes'=> array('target'=>'_blank'))).'
															
														</td>';
									}else{
										
										$output .=  '<td>FORM-I will be uploaded by the Applicant after payment of fees</td>';
									}
												
									$output .= '<td id="formVId"><input type="checkbox" id="signed_pdf_file" class="ismw_verified_alc" '.$form_1_signed_pdf_file_ck.' /></td>
								</tr>
								<tr>
									<td>10.(g)</td>
									<td>Previous Registration Certificate</td>';
									
									  if($enc_doc_array['ORC']['content'] !=''){
									
									
									$output .= '<td>'.l($pdficonlink,$enc_doc_array['ORC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
								}elseif(!empty($content_docs['backlog_certificate'])){
										  
											$output .= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/backlog_certificate/'.$content_docs['backlog_certificate'].'">
															<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png">&nbsp;&nbsp;
															<span class="viewfileins" id="backlog_certificate"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
															<div id="fileins_backlog_certificate"  title="Previous Registration Certificate" style="display:none;">
															<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/FORM-I/'.$content_docs['backlog_certificate'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
															</div></a>&nbsp;&nbsp;&nbsp;'.l(t('Generated FORM - I'),'ismw-generate-pdf/'.encryption_decryption_fun('encrypt', $content['id']), array('html' => TRUE,'attributes'=> array('target'=>'_blank'))).'
														</td>';
									}else{
										
										$output .=  '<td>Previous Registration Certificate will be uploaded by the applicant after fees payment in case, of Manually registered applicant</td>';
									}
												
									$output .= '<td><input type="checkbox" id="backlog_certificate" class="ismw_verified_alc" '.$backlog_certificate_ck.' /></td>
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
									 $payment_details .= 'Total amount:<span class="color_orange"> &#8377;'.number_format($total_amount,2).'</span><br>';
									 $payment_details .= 'Challan Details:'.l('<img src='.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png>', '/sites/default/files/upload/challan_upload/'.$govt_paydata['challan_file'], array('html' => TRUE, 'title' => 'Click here to view challan'));
									 
					  			}else{	
									
								 $transaction_details  			= db_select('l_principle_epayments_receive_data', 'lpd');
								 $transaction_details			->leftJoin('l_principle_epayments_data', 'lped', 'lped.identification_no = lpd.transaction_id');
								 $transaction_details			->fields('lped', array('identification_no', 'application_id'));
								 $transaction_details			->fields('lpd', array());
								 $transaction_details			->condition('lped.act_id', '4');
								 $transaction_details			->condition('lped.application_id', $content['id']);		
								 $trans_details_result 			= $transaction_details->execute();
								
								 $bank_code 					= 'Not available';
								 $bankTransactionID 			= 'Not available';
								 $total_amount 					= 'Not available';
								 $challan_fid_date 				= 'Not available';
								 $payment_status 				= 'Not available';
							
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
								
								 
								 $payment_details = '<u> GRIPS PAYMENT [ Online / Counter ] </u> <br>';
								 $payment_details .= 'IFSC Code : <font color="#FF6600">'.$bank_code.'</font><br>';
								 $payment_details .= 'Bank Transaction Id : <font color="#FF6600">'.$bankTransactionID.'</font><br>';
								 $payment_details .= 'Total Amount:<font color="#FF6600"> &#8377;'.$total_amount.'</font><br>';
								 $payment_details .= 'Transaction Date : <font color="#FF6600">'.$challan_fid_date.'</font><br>';
								 $payment_details .= 'Transaction Status : '.$payment_status;
							 }
							 
 							 /* Payment Details End */ 
 
					$output .= '<tr>
									<td colspan="4" style="font-weight:700; text-transform:uppercase; font-size:16px;">11. Payment Details</td>
								</tr>
								<tr>
									<td width="10%">11.a.) </td>
									<td>Total Fees</td>
									<td width="50%"> &#8377; ' .$content['finalfees'].'.00  &nbsp;&nbsp;&nbsp;&nbsp;
										<button type="button" class="btn btn-info" data-toggle="modal" data-target="#myModalFeeschart">
											<i class="fa fa-info-circle"></i>&nbsp;Fees Chart
										</button>
									</td>
								</tr>
								<tr>
									<td>11.(b)</td>
									<td>Payment Details</td>
									<td colspan="2">'.$payment_details.'</td>  	
								</tr>';
								if($content['status']=='I' ){
									if($content['deemed'] == 'Y'){
										$issue_by = 'Deemed Approved';
									}else{
										$issue_by = 'Issued By ALC';
									}
					$output .= '<tr>
									<td width="10%">12.</td>
									<td><strong>ISMW REGISTRATION CERTIFICATE</strong></td>';
									if($content['iwm_certificates_fid']!=''){
								$output .= '<td>'.$certificateDownloadLink.'
												<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$upload_certificates_file_url.'">&nbsp;&nbsp;
													<span class="viewfileins" id="signed_certificate"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
													<div id="fileins_signed_certificate"  title="Signed ISMW Registration Certificate" style="display:none;">
														<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$upload_certificates_file_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
													</div>
												</a>
											</td>';
									}else{
										 $generatecertificateDownload	= l(t('<i class="fa fa-download" aria-hidden="true"></i>&nbsp; ISMW Registration Certificate'),$GLOBALS['base_url'].'/generate-ismw-certificate/'.encryption_decryption_fun('encrypt', $content['iwm_qr_code']), array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn'))));										 
										$output .= '<td>'.$generatecertificateDownload.'
												<a target="_blank" href="'.$GLOBALS['base_url'].'/generate-ismw-certificate/'.encryption_decryption_fun('encrypt', $content['iwm_qr_code']).'">&nbsp;&nbsp;
													<span class="viewfileins" id="signed_certificate"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
													<div id="fileins_signed_certificate"  title="ISMW Registration Certificate" style="display:none;">
														<iframe src="'.$GLOBALS['base_url'].'/generate-ismw-certificate/'.encryption_decryption_fun('encrypt', $content['iwm_qr_code']).'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
													</div>
												</a>
											</td>
											<td>'.$issue_by.'</td>';
										
									}
					$output .= '</tr>';
								}
					$output .= '</table>
						</div>
					</br>
				
					<div class="modal fade" id="myModalFeeschart" role="dialog">
						<div class="modal-dialog">
							<div class="box box-primary box-solid">
								<div class="box-header">
									<button type="button" class="close" data-dismiss="modal">&times;</button>
									<h3 class="box-title">FEES CHART</h3>
									<!--<p><strong>[ Registration of Establishment Employing Migrant Workmen Under Inter-State Migrant Workmen (RE & CS) Act, 1971 ]</strong></p>-->
								</div>
								
								<div class="modal-body">
									<table cellpadding="0" cellspacing="0" border="0" class="table table-striped table-responsive admin-custom-table">
										<tr>
											<td colspan="3">If the Number of Migrant Workmen proposed to be employed in the establishment on any day</td>
										 </tr>
										 <tr>
											<th>Sl.No.</th>
											<th>#</th>
											<th>Fees</th>
										 </tr>
										 <tr>
											 <td>1</td>
											 <td>Is 5 or more but does not exceed 20 </td>
											 <td>₹30</td>
										 </tr>
										 <tr>
											 <td>2</td>
											 <td>Exceeds 20 but does not exceed 50</td>
											 <td>₹75</td>
										 </tr>
										 <tr>
											 <td>3</td>
											 <td>Exceeds 50 but does not exceed 100</td>
											 <td>₹150</td>
										 </tr>
										 <tr>
											 <td>4</td>
											 <td>Exceeds 100 but does not exceed 200</td>
											 <td>₹300</td>
										 </tr>
										 <tr>
											 <td>5</td>
											 <td>Exceeds 200 but does not exceed 400</td>
											 <td>₹600</td>
										 </tr>
										 <tr>
											 <td>6</td>
											 <td>Exceeds 400 but does not exceed 800</td>
											 <td>₹900</td>
										 </tr>
										 <tr>
											 <td>7</td>
											 <td>Exceeds 800 but does not exceed 1000</td>
											 <td>₹1000</td>
										 </tr>
										 <tr>
											 <td>8</td>
											 <td>Exceeds 1000</td>
											 <td>₹1500</td>
										 </tr>
									  </table>
								</div>
								<div class="modal-footer">
									<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
								</div>
							</div>      
						</div>
					</div>';
				
				
					
		$output .= '<div class="row"><ul style="padding:0;"><li class="col-md-2 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-arrow-left"></i>&nbsp;Back to list</div>', 'ismwapplicationslist', array('attributes' => array('title' => 'Click here back to applications list'), 'html' => TRUE)).'</li>';
	
		$output .= '<li class="col-md-2 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-user"></i>&nbsp;View Applicant Profile</div>', 'view-applicant-profile/'.encryption_decryption_fun('encrypt', $application_id).'/'.encryption_decryption_fun('encrypt', $applicant_user_id).'/'.encryption_decryption_fun('encrypt', 4), array('attributes' => array('title' => 'Click here view Applicant Profile'), 'html' => TRUE)).'</li>';
	
		$output .= '<li class="col-md-4 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-info-circle"></i>&nbsp;Instructions to give remarks</div>', $base_root.$base_path.'sites/default/files/instructions-remarks.pdf', array('attributes' => array('title' => 'Instructions to give remarks'), 'html' => TRUE)).'</li>';
	
		$output .= '<li class="col-md-4 col-xs-10" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-question-circle" aria-hidden="true"></i>&nbsp;How to digitally sign your documents using USB Token</div>', 'digitally-sign-process', array('attributes' => array('title' => 'How to digitally sign your documents using USB Token'), 'html' => TRUE)).'</li></ul></div></br>';
		return $output;
		
		
	}else{
		
		drupal_goto('node/3');	
	}
}

function view_ismw_application_alc_submit($form,&$form_state){ 
	
	
	global $base_root, $base_path, $user;
	
	$alc_user_id 			= $user->uid;
	
	$val 					= $form_state['values'];
	$identification_number	= encryption_decryption_fun('decrypt',$val['identification_number']);
	$applicant_user_id 		= encryption_decryption_fun('decrypt',$val['applicant_user_id']);
	$application_id 		= encryption_decryption_fun('decrypt',$val['application_id']);
	$fieldname				= $val['fieldname'];
	$remark_type			= $val['remark_type'];
	$remarks_text			= $val['remarks_text'];
	$act_id				    = 4;

	/* Upload For ISMW Signed Certificate*/
	
	//$arr_1									= array();
	//$signed_form_ismw 						= file_load($val['ismw_signed_certificate']);
	// $target_dir 							= $base_root.'/sites/default/files/upload/ISMW_Registration_Certificate/';
	// if( $signed_form_ismw != "" ){
		
	// 	$arr_1 = explode("/", $signed_form_ismw->uri); 
	// 	$signed_form_ismw->status = FILE_STATUS_PERMANENT;
	// 	file_save($signed_form_ismw);
		
	// 	$signed_form_ismw_file = $arr_1[4];
	// 	$signed_form_ismw_fid  = $signed_form_ismw->fid;
	// 	$signedCertificateName = $signed_form_ismw->filename;
	// 	$signedFile_link	   = escapefile_url($target_dir.$signed_form_ismw->filename);
	// 	$fileImageLink		   = file_get_contents($signedFile_link); 
	// 	$encryptedFile 		   = base64_encode($fileImageLink);
	// }
	
	//     *******    Generation of Registation Number     *******     //

	     /*** Get Applicant Block Code & Statuses & ALC Details ****/
    if($remark_type == 'I'){
		$ismw_application =	db_select('l_interstate_workman_master', 'lbrm');
   		$ismw_application->leftJoin('l_customuser_relation_address', 'lcra','lcra.sub_div_code=lbrm.loc_e_subdivision ');
   		$ismw_application->fields('lbrm',array('loc_e_subdivision','loc_e_areatype_code','registration_number','iwm_qr_code','identification_number'));
   		$ismw_application->fields('lcra',array('sub_div_code'));
   		$ismw_application->condition('lbrm.id',$application_id);
   		$ismw_application->condition('lbrm.user_id',$applicant_user_id);   
   		$ismw_application_result = $ismw_application->execute();
   
   		if($ismw_application_result->rowCount() > 0 ){	   
			$applicant_data = $ismw_application_result->fetchAssoc();
			$subdivision_code = $applicant_data['loc_e_subdivision'];
			$area_code = $applicant_data['loc_e_areatype_code'];
			$ismw_registration_number = $applicant_data['registration_number'];
			$ismw_qr_code = $applicant_data['iwm_qr_code']; 
			$alc_subdiv_code = $applicant_data['sub_div_code'];
			$identification_number = $applicant_data['identification_number'];
		}
   
   		if (empty($ismw_registration_number) && empty($ismw_qr_code)){	   
		   /*** GENERATION OF QR CODE ****/			   
			$ismw_qr_code = 'ISMW-REG'.$application_id.'A4U'.$applicant_user_id.'T'.time();		
	   
		   /*** GENERATION OF ISMW REGISTRATION NUMBER ***/ 
		   
		   $getShortNameSubdivision = custom_user_short_name_fun($subdivision_code);  // ---------come from custom_user module  
		   $getRegistrarCode = get_registration_code($area_code);              //-------------miscellaneous module
		   $registrarCode = substr($getRegistrarCode, -2);
		   
		   $reg_query =	db_query("select  max (NULLIF(substr(registration_number,11,6),'') :: integer) as serial_num  from l_interstate_workman_master where loc_e_subdivision='".$alc_subdiv_code."'");
		   $reg_query_result = $reg_query->fetchAssoc();
		   $reg_code = $reg_query_result['serial_num'];
		   
		   if(empty($reg_code)){ 
			   $registration_number	= $getShortNameSubdivision.$registrarCode.'/'.'IMR'.'/'.'000001';
		   }else{
			   $reg_code_next = $reg_code+1; 
			   $reg_first =	$getShortNameSubdivision.$registrarCode.'/'.'IMR'.'/';
			   $reg_second = str_pad($reg_code_next, 6, "0", STR_PAD_LEFT);
			   $registration_number	= $reg_first.$reg_second;
		   }
		   $update_status = db_update('l_interstate_workman_master');
		   $update_status->fields(array('iwm_qr_code' => $ismw_qr_code, 'registration_number' =>	$registration_number, 'registration_date' => date("Y-m-d")));
		   $update_status->condition('id',$application_id);
		   $update_status->condition('user_id',$applicant_user_id);
		   $update_status->execute();
		   
		   $remarks_text = 'Congratulations! Certificate is issued by the Registering Authority. You can download it from the dashboard.';
		}
	}


	/** FOR SHILPA SATHI ***/
	
	$serviceResponse = 1;	
	$fetchCommonId = db_select('l_common_application_master', 'cam');
	$fetchCommonId->fields('cam', array('id', 'oswicsapplicationflag', 'ldapplicationflag',  'ld_service_unique_id', 'ld_uid', 'ld_est_unique_id'));
	$fetchCommonId->condition('cam.user_id', $applicant_user_id);
	// $fetchCommonId->condition('cam.oswicsapplicationflag', 7);
	$commonId = $fetchCommonId->execute()->fetchAssoc();
	
	if($commonId['oswicsapplicationflag'] == 888){  // SILPASATHI	
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
		
		if($remark_type == 'V'){	
			$paymentdetails[] = array(
						'payableamt' => $val['finalfees'],
						'hoa' => '0230-00-101-002-16',
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
			echo $pdf_gen_url =  'https://wblc.gov.in/generate-ismw-certificate/'.$ismw_qr_code.'/plan'; die;
			$pdf_gen_url =  'https://wblc.gov.in/generate-ismw-certificate/123/plan';
			
			// $pdf_gen_url =  'https://wblc.gov.in/sites/default/files/upload/ISMW_Registration_Certificate/ISMW-REG-1665482171.pdf';		
			
			echo '<br>';	
			
			echo $fileImageLink = file_get_contents($pdf_gen_url);
			die;
			
			$data_file[] = array(
						'file_id' => $val['registration_number'],
						'file_date' => $val['registration_date'],
						'file_category' => 'CERTIFICATE',
						'file_name' => str_replace('/', '_',$val['registration_number']).'_'.$application_id.'.pdf',
						'file_type' => 'application/pdf',
						'file_content' => base64_encode($fileImageLink),
						'valid_upto' => '',
						);
			
			$cquery = db_select('l_interstate_contractor_master', 'cm');
			$cquery->fields('cm', array('eodb_con_id', 'form_six', 'name_of_contractor', 'contractor_status', 'form_six'));
			$cquery->condition('cm.application_id', $application_id);
			// $cquery->condition('cm.eodb_con_id', 0, '!=');
			$cquery = $cquery->execute(); 
			
			if($cquery->rowCount()>0){
				foreach($cquery as $cdata) {
					$contractorlist[] = array(
									'form_six' => $cdata->form_six,
									'contractor_name' => $cdata->name_of_contractor,
									'eodb_con_id' => $cdata->eodb_con_id,									
									'status' => trim($cdata->contractor_status)
								 );
				}
			}
			
			$additionalParamDtls[] = array(
						'paramName' => 'CONTRACTORLIST',
						'paramVal' => $contractorlist					
						);				
			$curl_post_data = array (
 						'source' => 'WBLC',
						'taskid' => 'STATUSDATA',
						'eodb_app_id' => $val['eodb_app_id'],
						'caf_id_no' => $val['caf_id_no'],									
						'status_code' => $remark_type,
						'status_text' => $status_text,
						'status_date' => date('Y-m-d H:m:s'),
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
				
		// $service_url = "http://172.20.176.12/eodbosw/eservices/deptpostData";
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
		$results = curl_exec($ch);
		$errors = curl_error($ch); // print_r($errors); die;
		curl_close($ch);
		// $curlRes = json_encode($results); 
		$results = json_decode(trim($results), TRUE); // echo '<pre>'; print_r($results); die;
 
	   if($results['code'] == 200){
			$serviceResponse = 1;
			// $res_array = $result['content'];
	   }	  
	}
	if($commonId['oswicsapplicationflag'] == 7){		
		$serviceResponse = 0;
		// $service_url = 'http://203.163.247.155:8443/api/Generic/Post'; // Staging 
		$service_url = 'http://202.61.117.163:8443/api/Generic/Post'; // producttion
		$properties_ss 	 = 'Qqq7HUBItKukq0aG0HNkdscTZdVMXJGyAMOGswKAFqkdTtuXFggJgfS56G3rjn5gSGtPuRqwg23X6IqZhSONs8BAwfX6LzdweLb0NqB1RRQGQnrC0kUt6GM98XuWRxYJeSXNkhbiSdj5ZDerECaG6ck1wLo4Q1xFPmhipUXwD/B860tsRnpKlbrmuyz1ByUUSVHY+Kx9ijXUX4eZsRZHnwnDY9Vwm6+8QoKq5gXsYi64dJxczd+kUNXSIbRv+ET9gAYG3aE375xOuhTaPV85szRQz7QKE1ycEXEAmM1zdsQw1yEWI4sVXOhCUKlVe2LIJABq8TzLgi0Dq+CSlOivbEsSUJqt/k/Eu/WizWc2G9t9MvSLlutR4/TmhYQWZ7weWzpcQytZkJgVzTuvdFyhLF7n1a2BSHuKXMHkUUrMfXUh2NwgrXDjFafCn5ubuCBwDEs/N7hTgVCNSTz9KVL+E2AJ73OWF2o8+bjT/nFnePROjlNaXtAkOXqVw248pqlLh4cAm7hn7+E+ihpdAynuBm4lK731u3N21bUux8pOKRGyUrh1MuwZLcQ599wuHiZ6CIQpcpTJ7JJeH0+mT322x46kYu3hbDGsPPygkC102eS9sAJ/yERENzJOn0w1p9mpQz3e3WBFWVXIoiCW3/3SBYfQ26IthNMNMH0ufS4SCJ5cxOwCPfCeIoBQLQYZR70JHA7Mq+aseyXNM6wlKHysVRqUPR1nAuzS2wupYXDTMvs8wstOkdqkyTwUcJrNwCDZaVwbWgai628q9lUAszPdqveiX2bzGyMISxJ6mzk3jVk6osSo9EHHXFBEhyzuprlEhkW4LnCQwmVOyu+SXeKbbh1Jumv50iUs2ZoTE7jbF3MD50TPXRD+YQo6KzZwnBDuv14+uNQ2vC22x060SgkWCqw+YrTFhHrrEpdxw3gGjK0u7AjbPtj8oiznhetBaw9kT7maHt4gKFhUrvGEJp71Igr32lO8AdVUfOtnlkoqdphkw33VATY9+KIeADVa+6Fp2PcW7BOcfSWHhSydxuj6lQ==';
		
		$ismwApplication =	db_select('l_interstate_workman_master', 'liwm');
		$ismwApplication -> fields('liwm',array('status','registration_number','iwm_qr_code','iwm_certificates_fid','record_id'));
		$ismwApplication -> condition('liwm.id',$application_id);
		$ismwApplication -> condition('liwm.user_id',$applicant_user_id);
		$ismwApplicationResult = $ismwApplication->execute()->fetchAssoc();
		
		if($ismwApplicationResult['registration_number'] != ''){
			$registration_number = $ismwApplicationResult['registration_number'];
			$registration_date = date('d/m/Y', strtotime($ismwApplicationResult['registration_date']));
		}
		
		/** For Fetching Director/Partner Data to Shilpa Sathi ***/
		
		$ismw_dir_partner = db_select('l_interstate_employer_master', 'lidpm');	
		$ismw_dir_partner -> fields('lidpm', array('id','name','designation'));
		$ismw_dir_partner -> condition('lidpm.application_id', $application_id);	
		$ismw_dir_partner -> condition('lidpm.user_id', $applicant_user_id);
		$ismw_dir_partner -> condition('lidpm.is_active', 1);
		$db_or = db_or();
		$db_or -> condition('lidpm.designation', 'director', '=');
		$db_or -> condition('lidpm.designation', 'partner', '=');
		$ismw_dir_partner -> condition($db_or);
		$ismw_dir_partner -> orderBy('lidpm.id', 'DESC');
		$ownership_details = $ismw_dir_partner->execute()->fetchAll();
		
		foreach ($ownership_details as $ownership_row){
			$ownerDataArray[]		=array(
									'director_id'			=> $ownership_row->id,
									'director_name'		=> $ownership_row->name,
									//'lisma_dir_desig'		=> $ownership_row->designation,
									'director_cafno' 		=> $identification_number,
									'director_tid'			=> 'lisma',
								 );	
		}
		
		/** For Fetching Manager Data to Shilpa Sathi ***/
		
		$ismw_manager 				= 	db_select('l_interstate_employer_master', 'lidpm');	
		$ismw_manager				->	fields('lidpm', array());
		$ismw_manager				->  condition('lidpm.application_id', $application_id);	
		$ismw_manager				->	condition('lidpm.user_id', $applicant_user_id);
		$ismw_manager				->	condition('lidpm.is_active', 1);
		$ismw_manager				->	condition('lidpm.designation', 'manager', '=');
		$ismw_manager				->	orderBy('lidpm.id', 'DESC');
		$ismw_manager_details		= 	$ismw_manager->execute()->fetchAll();
		
		foreach ($ismw_manager_details as $manager_row){
			
			$managerDataArray[]		=array(
									'manager_id'			=> $manager_row->id,
									'manager_name'		=> $manager_row->name,
									//'lisma_man_desig'		=> $manager_row->designation,
									'manager_cafno' 		=> $identification_number,
									'manager_tid'			=> 'lisma'
								 );	
		}
		 
		/** For Fetching Contractor Data to Shilpa Sathi ***/
		
		$ismw_con 				= 	db_select('l_interstate_contractor_master', 'licm');
		$ismw_con				->	fields('licm', array('id','name_of_contractor'));
		$ismw_con				->  condition('licm.application_id', $application_id);	
		$ismw_con				->	condition('licm.user_id', $applicant_user_id);
		$ismw_con				->	condition('licm.contractor_status', 1);
		$ismw_con				->	orderBy('licm.id', 'DESC');
		$ismw_con_res			= 	$ismw_con->execute()->fetchAll(); 
		
		$getRemark 				= db_select('l_interstate_remark_master', 'lrm');
		$getRemark->fields('lrm', array('record_id','remark_type','remark_by_role'));
		$getRemark->condition('lrm.remark_to', $applicant_user_id);
		$getRemark->condition('lrm.application_id', $application_id);
		$getRemark->orderBy('lrm.id', 'DESC');
   		$getRemark->range(0, 1);
		$remark = $getRemark->execute()->fetchAssoc();
		
		/*$ismw_con 				= db_query('select name_of_contractor,id from l_interstate_contractor_master where application_id="'.$application_id.'" and user_id = "'.$applicant_user_id.'" and contractor_status = 1 order by id DESC');
		$ismw_con_res			= 	$ismw_con->execute()->fetchAll(); */
		
		
		foreach ($ismw_con_res as $con_row){
			
			$conDataArray[]		=array(
									'contractor_id'	=> $con_row->id,
									'contractor_name'	=> $con_row->name_of_contractor,
									'contractor_tid' 		=> 'lisma',
									'eodbcafno' 			=> $identification_number
								 );
		}
		
		
		
		$curl_post_data 	= array(
								'transid' 		=> 'lcsts',
								'properties'  	=> $properties_ss,
								'dc1' 			=> array(
													'no' 	=> '1',
													'type' 	=> '0',
													'data' 	=> array(
																  'application_id' 		=> $application_id,
																  'remark_type' 		=> $remark_type,   
																  'eodbcafid' 			=> $identification_number,
																  'registration_date' 	=> $registration_date,
																  'registration_number' => $registration_number,
																  'insfilename' 		=> $signedCertificateName,
																  'file_contents' 		=> $encryptedFile,
																  'remarks' 			=> $remarks_text,
																  'remark_by' 			=> 'ALC',
																  'remark_date' 		=> date('d/m/Y'),
																  'remark_to' 			=> 'Applicant',
																  'record_id' 			=> $remark['record_id'],
																  'form_tid' 			=> 'lisma'
																)
														),
								'Contractor Details' => array(
													'no' 	=> '2',
													'type' 	=> '1',
													'data' 	=>  $conDataArray													
													),
													
								'Director Details' => array(
													'no' 	=> '4',
													'type' 	=> '1',
													'data' 	=>  $ownerDataArray
													
													),
								'Manager Details' => array(
													'no' 	=> '5',
													'type' 	=> '1',
													'data' 	=>  $managerDataArray
													),
								 );
		
		//print_r($curl_post_data); die;
		//echo json_encode( $curl_post_data); die;
								 
		$ch = curl_init();
		curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-Type: application/json; charset=utf-8'));
		curl_setopt($ch, CURLOPT_URL, $service_url);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0); 
		curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 0);
		curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($curl_post_data));
		curl_setopt($ch, CURLOPT_POST, true);
		$results = curl_exec($ch);
		
		curl_close($ch);
					
		$tmp = json_decode($results);//echo $tmp;die;
		
		$data = explode('*$*', $tmp);// print_r($data); exit;
		
		$a = json_decode($data[1])->message;
		$b = json_decode($data[2])->result;  //print_r($b); die;

		if($b[0]->save == 'success'){
			$serviceResponse = 1;
		}
	}
	if($commonId['ldapplicationflag'] == 1){		
		$serviceResponse = 0;
		
		$status_arr = array (
 						'status' => $remark_type,						
 						'id' => $commonId['imw_ld_service_id'],
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
	
	if($serviceResponse == 1){
		
		/*** Get User role of ALC **/
		
		$fetch_users_role = db_select('users_roles', 'ur');
		$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
		$fetch_users_role->leftJoin('l_custom_user_detail', 'lcud', 'ur.uid=lcud.usr_id');
		$fetch_users_role->fields('ro', array('rid'));
		$fetch_users_role->fields('lcud', array('fullname','employee_id'));
		$fetch_users_role->condition('ur.uid', $alc_user_id);
		$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc();
	
		$remark_by_role	= $fetch_users_role_data['rid'];
		$alc_full_name = $fetch_users_role_data['fullname'];
		$employee_id   = $fetch_users_role_data['employee_id'];
		
		/*** Update Status of ISMW Master Table ***/ 
	
		$update_status = db_update('l_interstate_workman_master');
		
		//if(!empty($signed_form_ismw_file)){
			// $update_status->fields(array(
			// 		 'status' => $remark_type,
			// 		 'iwm_certificates_fid' => $signed_form_ismw_fid,
			// ));
		//} else{
			
			$update_status->fields(array('status' => $remark_type));
		//}
		
		$update_status->condition('id',$application_id);
		$update_status->condition('user_id',$applicant_user_id);
		$update_status->execute();
	
		if($remark_type == 'I'){
			// db_insert('l_encrypted_uploaded_documents')->fields(
			// 											array(
			// 												'user_id' 			 => $applicant_user_id,
			// 												'document_type' 	 => 'ISMW Reg Certificate' ,
			// 												'document_type_code' => 'ISMWREG',
			// 												'filename'			 => $signedCertificateName,
			// 												'filecontent'		 => $encryptedFile,
			// 												'act_id'			 => 4,
			// 												'application_id'	 => $application_id,
			// 												'created_date'		 => date('Y-m-d'),
			// 												'status'			 => 1
			// 												))->execute(); 	
	
	
		 /*** FORM VI GENERATION ***/
		 $form_six				= NULL;
	
		
		
		    $update_tag  =  db_update('l_interstate_tag_master')
						->fields(array('registration_number' => $registration_number, 'issued_date' => date("Y-m-d"),'status' => $remark_type))
						->condition('application_id', $application_id)	
						->condition('identification_number', $identification_number)
						->condition('pe_user_id', $applicant_user_id)
						->condition('applied_for', 'REG')
						->condition('act_id', 4)
						->execute();
				
			
			$ismw_con 				= 	db_select('l_interstate_contractor_master', 'licm');
			$ismw_con				->	fields('licm', array('id'));
			$ismw_con				->  condition('licm.application_id', $application_id);	
			$ismw_con				->	condition('licm.user_id', $applicant_user_id);
			$ismw_con				->	condition('licm.contractor_status', 1);
			$ismw_con				->	orderBy('licm.id', 'DESC');
			$ismw_con_res			= 	$ismw_con->execute()->fetchAll(); 
		
			foreach ($ismw_con_res as $con_row){
			
				$form_six 			=   $con_row->id;
				
				$update_form_six 	= 	db_update('l_interstate_contractor_master');
				$update_form_six	->	fields(array('form_six'	  => $form_six));
				$update_form_six	->	condition('id',$con_row->id);
				$update_form_six	->	condition('user_id',$applicant_user_id);
				$update_form_six	->	condition('contractor_status', 1);
				$update_form_six	->	execute();
		
			}
			

			if($commonId['ldapplicationflag'] == 8 && $remark_type == 'I'){ 
				$applicant_mailto  = get_email($applicant_user_id);
				$applicant_mobile_no = get_mobile_number($applicant_user_id);
				
				db_update('users')->fields(array('status'=>1))->condition('status',0)->condition('uid',$applicant_user_id)->execute();
				$msg = 'Your ISMW PE registration certificate is issued by ALC(Govt. of WB). Login to download the certificate from dashboard.<br>You may avail other services from <strong><a href="'.$base_root.$base_path.'">wblc.gov.in</a></strong><br/><br/> Username :'.$identification_number.'<br/>Email :'.$applicant_mailto.'<br/>Mobile :'.$applicant_mobile_no.'<br/>Also, you may find your user credentials from <a href="'.$base_root.$base_path.'find-user-details">CLICK HERE</a> with your application information. You can generate your password from <a href="'.$base_root.$base_path.'forgot-password">Forgot your password</a> ';
	
					
				$subject = 'Welcome to Labour Commissionerate, Govt. Of West Bengal';
				if(!empty($applicant_mailto)){
					send_mail_for_user_alert($applicant_mailto, $subject, $msg);
					send_mail_for_user_alert('wblc.nic@gmail.com', $subject, $msg);
				}
				
			}
		}
	
		/***Insert In ISMW Remarks Master Table ***/ 
	
		
		$fieldsApplInfo2  =  array(
			'remark_text'   		=> $remarks_text, 
			'remark_by'   			=> $alc_user_id, 
			'remark_to' 			=> $applicant_user_id,
			'application_id' 		=> $application_id,
			'remark_date'  			=> time(),
			'remark_type' 			=> $remark_type,
			'remark_by_name'    	=> $alc_full_name,
			'remark_field_text' 	=> $fieldname,
			'remark_by_role' 		=> $remark_by_role,
			'call_time_applicant'	=> !empty($call_date_time)? strtotime($call_date_time): 0,
			'employee_id'			=> $employee_id
			
		);
	
		db_insert('l_interstate_remark_master')->fields($fieldsApplInfo2)->execute(); 

		drupal_goto('ismwapplications-view/'.$val['application_id'].'/'.$val['applicant_user_id']);
		$message = 'Registration Certificate has been successfully generated.';
		drupal_set_message(t($message));
	}
	
}


function generation_ismw_registration_number($form, &$form_state){
	
	 global $user;
	 $val  					= $form_state['values'];
	 $applicant_user_id		= encryption_decryption_fun('decrypt',$val['applicant_user_id']);
	 $application_id		= encryption_decryption_fun('decrypt',$val['application_id']);
	 $identification_number	= encryption_decryption_fun('decrypt',$val['identification_number']);
	 $act_id				= 4;
	 $fieldname				= $val['fieldname'];
	 $remark_type			= $val['remark_type'];
	 $alc_user_id			= $user->uid;
	 	 
	 /*** Get Applicant Block Code & Statuses & ALC Details ****/
	
	$ismw_application							=	db_select('l_interstate_workman_master', 'lbrm');
	$ismw_application							->  leftJoin('l_customuser_relation_address', 'lcra','lcra.sub_div_code=lbrm.loc_e_subdivision ');
	$ismw_application							->	fields('lbrm',array('loc_e_subdivision','loc_e_areatype_code','registration_number','iwm_qr_code','identification_number'));
	$ismw_application							->	fields('lcra',array('sub_div_code'));
	$ismw_application							->	condition('lbrm.id',$application_id);
	$ismw_application							->	condition('lbrm.user_id',$applicant_user_id);
	
	$ismw_application_result 					= 	$ismw_application->execute();
	
	if($ismw_application_result-> rowCount() > 0 ){
		
		$applicant_data 							= 	$ismw_application_result->fetchAssoc();
		$subdivision_code							=   $applicant_data['loc_e_subdivision'];
		$area_code									=   $applicant_data['loc_e_areatype_code'];
		$ismw_registration_number					=   $applicant_data['registration_number'];
		$ismw_qr_code								=   $applicant_data['iwm_qr_code']; 
		$alc_subdiv_code							=   $applicant_data['sub_div_code'];
		$identification_number						=   $applicant_data['identification_number']; 		
	}
	
	if (empty($ismw_registration_number) && empty($ismw_qr_code)){
		
			/*** GENERATION OF QR CODE ****/			
			
			 $ismw_qr_code = 'ISMW-REG'.$application_id.'A4U'.$applicant_user_id.'T'.time();		
		
			/*** GENERATION OF ISMW REGISTRATION NUMBER ***/
			
			
			$getShortNameSubdivision 			=   custom_user_short_name_fun($subdivision_code);  // ---------come from custom_user module  
			$getRegistrarCode					=	get_registration_code($area_code);              //-------------miscellaneous module
			$registrarCode 						=   substr($getRegistrarCode, -2);
			
			$reg_query							=	db_query("select  max (NULLIF(substr(registration_number,11,6),'') :: integer) as serial_num  from l_interstate_workman_master where loc_e_subdivision='".$alc_subdiv_code."'");
			$reg_query_result					=	$reg_query->fetchAssoc();
			$reg_code							=	$reg_query_result['serial_num'];
			
			if(empty($reg_code)){ 
				$registration_number	=	$getShortNameSubdivision.$registrarCode.'/'.'IMR'.'/'.'000001';
			}else{
				$reg_code_next			= 	$reg_code+1; 
				$reg_first				=	$getShortNameSubdivision.$registrarCode.'/'.'IMR'.'/';
				$reg_second				=	str_pad($reg_code_next, 6, "0", STR_PAD_LEFT);
				$registration_number	=	$reg_first.$reg_second;
			}
			
			
					
			/*** Update Status of ISMW Master Table ***/ 
			
			//if($applicant_user_id == 352){
				$update_status  =  db_update('l_interstate_workman_master');
				$update_status->fields(array('iwm_qr_code' => $ismw_qr_code, 'registration_number' =>	$registration_number, 'registration_date' => date("Y-m-d"),'status'=>'I'));
				$update_status->condition('id',$application_id);
				$update_status->condition('act_id',$act_id);
				$update_status->condition('user_id',$applicant_user_id);
				$update_status->execute();
				
				
				$ismw_con = db_select('l_interstate_contractor_master', 'licm');
				$ismw_con->fields('licm', array('id'));
				$ismw_con->condition('licm.application_id', $application_id);	
				$ismw_con->condition('licm.user_id', $applicant_user_id);
				$ismw_con->condition('licm.contractor_status', 1);
				$ismw_con->orderBy('licm.id', 'DESC');
				$ismw_con_res = $ismw_con->execute()->fetchAll(); 
			
				foreach ($ismw_con_res as $con_row){
				
					$form_six 			=   $con_row->id;
					
					$update_form_six 	= 	db_update('l_interstate_contractor_master');
					$update_form_six	->	fields(array('form_six'	  => $form_six));
					$update_form_six	->	condition('id',$con_row->id);
					$update_form_six	->	condition('user_id',$applicant_user_id);
					$update_form_six	->	condition('contractor_status', 1);
					$update_form_six	->	execute();
			
				}
				
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
				
				$fieldsApplInfo2  =  array(
								'remark_text'   		=> 'Congratulations! Certificate is issued by the Registering Authority. You can download it from the dashboard.',  
								'remark_by'   			=> $alc_user_id, 
								'remark_to' 			=> $applicant_user_id,
								'application_id' 		=> $application_id,
								'remark_date'  			=> time(),
								'remark_type' 			=> $remark_type,
								'remark_by_name'    	=> $alc_full_name,
								'remark_field_text' 	=> $fieldname,
								'remark_by_role' 		=> $remark_by_role,
								'employee_id'			=> $employee_id
								
								
							);
	
				db_insert('l_interstate_remark_master')->fields($fieldsApplInfo2)->execute();
				
				$update_tag  =  db_update('l_interstate_tag_master')
							   ->fields(array('registration_number' => $registration_number, 'issued_date' => date("Y-m-d"),'status' => 'I'))
							   ->condition('application_id', $application_id)	
							   ->condition('identification_number', $identification_number)
							   ->condition('pe_user_id', $applicant_user_id)
							   ->condition('applied_for', 'REG')
							   ->condition('act_id', 4)
							   ->execute();
				
			/*}else{
			$update_status  					=  db_update('l_interstate_workman_master');
			$update_status->fields(array('iwm_qr_code' => $generated_qr_code, 'registration_number' =>	$registration_number, 'registration_date' => date("Y-m-d"),));
			$update_status->condition('id',$application_id);
			$update_status->condition('act_id',$act_id);
			$update_status->condition('user_id',$applicant_user_id);
			$update_status->execute();
			
			$update_tag  					=  db_update('l_interstate_tag_master');
			$update_tag->fields(array('registration_number' => $registration_number, 'issued_date' => date("Y-m-d")));
			$update_tag->condition('application_id',$application_id);
			$update_tag->condition('pe_user_id',$applicant_user_id);
			$update_tag->condition('identification_number',$identification_number);
			$update_tag->condition('act_id', 4);
			$update_tag->execute();
			}
			
						
			$message									   = drupal_set_message_custom(t('Registration Certificate has been successfully generated. Please Sign and Upload Certificate to complete this process'));
 			$form_state['values']['hidden_action_message'] = $message;*/
			
			// $msg = 'Congratulations! Certificate is issued by the Registering Authority. You can download it from the dashboard. <br>You may avail the other services from <strong>wblc.gov.in</strong> with the user credentials of <strong>Shilpa Sathi</strong> (<strong>silpasathi.wb.gov.in</strong>)';
				// $applicant_mailto  = get_email($applicant_user_id);
				// $subject = 'Welcome to Labour Commissionerate, Govt. Of West Bengal';
				// if(!empty($applicant_mailto)){
				// 	send_mail_for_user_alert($applicant_mailto, $subject, $msg);
				// }
			$form_state['rebuild']							= TRUE;
	
	}
	 
	 
	
	
}


function get_ismw_remark_details($application_id,$applicant_user_id){
	
	global $base_root, $base_path, $user;
	
	$user_id 		= $user->uid;
	$loadingImg 	= $base_root.'/sites/all/themes/jackson/images/lodinggif.gif';
	$counter 		= 0;
	
	$header = array(
					array('data' => 'Sl', 'field' => 'slno','width' => '5%'),
					array('data' => 'Date - Time', 'field' => 'date','width' => '15%'),
					array('data' => 'Remark', 'field' => 'remrk'),
					array('data' => 'Remark Status', 'field' => 'remark type','width' => '15%'),
					array('data' => 'Remark By', 'field' => 'licenseno','width' => '20%'),
					array('data' => 'Action','width' => '10%')
				); 
				
	
	$get_remark = db_select('l_interstate_remark_master', 'lirm');
	$get_remark	->leftJoin('role', 'ro', 'ro.rid = lirm.remark_by_role');
	$get_remark->fields('lirm', array());
	$get_remark->fields('ro', array('name'));
	$get_remark->condition('lirm.remark_to', trim($applicant_user_id));
	$get_remark->condition('lirm.remark_by_role', 8,'!=');
	$get_remark->condition('lirm.application_id', trim($application_id));
	$get_remark->condition('lirm.is_active', 1);
	$get_remark->orderBy('lirm.id','DESC');
	$get_remark_result = $get_remark->execute()->fetchAll(); 
	
	
	foreach($get_remark_result as $row){ 
		
		$counter 		= $counter+1;
		$remark_by		= $row->remark_by_name.'&nbsp;('.$row->name.')';
		
		// get latest remark given by session user // get payment info (checking  for data insert into l_principle_epayments_data table)
			
		$getPayInfo		= db_query("SELECT COUNT(*) AS total_row FROM l_principle_epayments_data WHERE act_id=4 AND application_id = ".$application_id."")->fetchObject();
		$getId			= $getPayInfo->total_row;
		
		$getMaxData		= db_query("SELECT MAX(id) AS MAXID FROM l_interstate_remark_master WHERE is_active=1 and application_id = ".$application_id."")->fetchObject();
		$maxId			= $getMaxData->maxid;
				
		/*if($maxId>0){
			if($row->remark_type=='U'){
				$url = 'delete-remarks/'.encryption_decryption_fun('encrypt',$row->id).'/'.encryption_decryption_fun('encrypt',$application_id).'/'.encryption_decryption_fun('encrypt',$applicant_user_id).'/'.encryption_decryption_fun('encrypt',4);	
				$delete	= l('<div id="delBtn" class="delete-btn active-del">&nbsp;</div>',$url,array('html' => TRUE));
					
			}else{ 
				if(($row->remark_by==$user_id) &&($maxId==$row->id) && ($getId!=0 || $getId!='')){
					$url = 'delete-remarks/'.encryption_decryption_fun('encrypt',$row->id).'/'.encryption_decryption_fun('encrypt',$application_id).'/'.encryption_decryption_fun('encrypt',$applicant_user_id).'/'.encryption_decryption_fun('encrypt',4);	
					$delete	= l('<div id="delBtn" class="delete-btn active-del">&nbsp;</div>',$url,array('html' => TRUE));	
				}else{
					$delete	= '<div id="delBtn" class="delete-btn disable-del">&nbsp;</div>';
				}	
			}
		}*/
		
		if($maxId>0){
			if($row->remark_type=='U'){				
				$delete	= '<span id="delBtn" class="delete-btn active-del" data-id ='.encryption_decryption_fun('encrypt',$row->id).' data-uid='.encryption_decryption_fun('encrypt',$user_id).' data-act='.encryption_decryption_fun('encrypt',4).'>&nbsp;<img style="display:none;margin-left:30px;" src="'.$loadingImg.'" alt="loading" id="loading_'.encryption_decryption_fun('encrypt',$row->id).'"></span>';
					
			}else{ 
				if(($row->remark_by==$user_id) &&($maxId==$row->id) && ($getId!=0 || $getId!='')){						
					$delete	= '<span id="delBtn" class="delete-btn active-del" data-id ='.encryption_decryption_fun('encrypt',$row->id).' data-uid='.encryption_decryption_fun('encrypt',$user_id).' data-act='.encryption_decryption_fun('encrypt',4).'>&nbsp;<img style="display:none;margin-left:30px;" src="'.$loadingImg.'" alt="loading" id="loading_'.encryption_decryption_fun('encrypt',$row->id).'"></span>';
				}else{
					$delete	= '<div class="delete-btn disable-del">&nbsp;</div>';
				}	
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

function get_call_date_time_field($form, $form_state){
	$commands 	= array();
	$commands[] = ajax_command_replace('#download_link_block', drupal_render($form['ismw_reg']['download_link']));
	$commands[] = ajax_command_replace('#ismw_signed_certificate_block', drupal_render($form['ismw_reg']['ismw_signed_certificate']));
	$commands[] = ajax_command_replace('#generate_certificate_block', drupal_render($form['ismw_reg']['generate_certificate']));
	$commands[] = ajax_command_replace('#remarks_text_block', drupal_render($form['ismw_reg']['remarks_text']));
	$commands[] = ajax_command_replace('#submit_block', drupal_render($form['ismw_reg']['submit']));
	return array('#type' => 'ajax', '#commands' => $commands);
	
}

