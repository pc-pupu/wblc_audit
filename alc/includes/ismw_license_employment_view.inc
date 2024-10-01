<?php

drupal_add_js(drupal_get_path('module', 'alc').'/js/mycustom.js');
function ismw_license_employment_view($form, &$form_state, $id = ''){
	
	global $base_root, $base_path, $user;
	
	$user_id 							= $user->uid;
	$license_id							= encryption_decryption_fun('decrypt', $id);
	
	/**** Fetch ALC SUBDIVISION Code ****/
	
	$get_alc_subdivision_details		=	db_select('l_customuser_relation_address', 'lcra');
	$get_alc_subdivision_details		->  leftJoin('users_roles', 'ur','ur.uid=lcra.user_id ');
	$get_alc_subdivision_details		->  leftJoin('role', 'ro', 'ro.rid = ur.rid');
	$get_alc_subdivision_details		->  fields('ro', array('name','rid'));
	$get_alc_subdivision_details		->	fields('lcra',array('sub_div_code'));
	$get_alc_subdivision_details		->	condition('lcra.user_id',$user_id);
	$get_alc_subdivision_details_result = 	$get_alc_subdivision_details->execute();
	
	if($get_alc_subdivision_details_result-> rowCount() > 0 ){
		
		$alc_data 						= $get_alc_subdivision_details_result->fetchAssoc();  
		$alc_subdivision_code 			= trim($alc_data['sub_div_code']);
		$alc_role_rid					= $alc_data['rid']; 
		$alc_role_name					= $alc_data['name']; 
		
	}
	
	/*** Get Applicant Block Code & Statuses ****/
	
	$get_licenseDetails					=   db_select('l_interstate_license_master', 'lilm');
	$get_licenseDetails	   				->  fields('lilm',array('worksite_subdiv','final_submit_status','status','license_number','qr_code','certificates_fid','identification_number','form_six','contractor_user_id','act_id','security_fees','reason_fees'));
	$get_licenseDetails					->	condition('lilm.worksite_subdiv',$alc_subdivision_code);
	$get_licenseDetails	   				->  condition('lilm.id', $license_id);
	$get_licenseDetails					->	condition('lilm.final_submit_status','P');
	$licenseDetails_result  			=   $get_licenseDetails->execute();
	
	if($licenseDetails_result-> rowCount() > 0 ){
		
		$applicant_data 				= $licenseDetails_result->fetchAssoc();
		$form_six						= $applicant_data['form_six'];
		$contractor_user_id 			= $applicant_data['contractor_user_id'];
		$worksite_subdivsion_code 		= trim($applicant_data['worksite_subdiv']);
		$ismw_license_number			= $applicant_data['license_number'];
		$iwm_license_qr_code			= $applicant_data['qr_code'];
		$applicant_final_submit_status	= $applicant_data['final_submit_status']; 
		$applicant_status				= $applicant_data['status'];
		$signed_certificate_fid			= $applicant_data['certificates_fid'];
		$identification_number			= $applicant_data['identification_number'];
		$securityFees					= ($applicant_data['security_fees']!='')|| $applicant_data['security_fees']!=0 ? $applicant_data['security_fees']: '';
		
	

		
		if($applicant_status == 'B'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i>CURRENT STATUS: Back for rectification</h4>
								<p>Application is sent back for rectification. After modification by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p>
								</div>';
		}
		if($applicant_status == 'BI'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i>CURRENT STATUS:  Back to inspector for rectification</h4><p>Application is sent back to Inspector for further verification. After verification, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
		}
		if($applicant_status == 'I'){
			$current_status = '<div class="callout callout-success"><h4>CURRENT STATUS: Certificate (FORM-VIII) is issued. </h4><p>Please check the License Certificate after uploading the Certificate.</div>';
		}
		if($applicant_status == 'F'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Forwarded by Inspector to ALC</h4><p>Application is Forwarded to ALC by Inspector for further verification. Any action can be taken for the application.</p></div>';
		}
		if($applicant_status == 'T'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Transaction successful</h4><p>Payment successful for this application. Form-V is not uploaded by the applicant. After submission of signed FORM-V by the applicant, the application can be further accessible.</p></div>';
		}
		if($applicant_status == 'V'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> 
									CURRENT STATUS: Verified and approved for fees submission. </h4><p>Application is approved and directed to pay fees. After fees payment and submission of signed FORM-V by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
		}
		if($applicant_status == 'R'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Rejected application</h4><p>If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
		}
		if($applicant_status == 'VA'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i>CURRENT STATUS: Application has been verified &amp; approved without fees.</h4><p>Application is approved without fees. After submission of signed FORM-V by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
		}
		if($applicant_status == 'S'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS:  FORM-V uploaded & application succesfully submitted</h4><p>FORM-V is submitted by the Applicant. After verification of uploaded FORM-V, Issue of License Certificate can be generated now or back to rectification FORM-V.</p></div>';
		} 
		if($applicant_status == 'U'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> 
									CURRENT STATUS: Back for Rectification(FORM-V)</h4><p>FORM-V is sent back for rectification. After modification by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p>
								</div>';
		} 
		if($applicant_status == '0'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Pending application</h4><p>Application is applied by the Applicant. Any action can be taken for the application.</p></div>';
		} 
	}
	
	
	/*** Get REMARK Details ****/
	
	$get_remark = db_select('l_interstate_remarks_license', 'lirm');
	$get_remark->fields('lirm', array('remark_field_text','remarks'));
	$get_remark->condition('lirm.license_id', $license_id);
	$get_remark->condition('lirm.remark_by', $contractor_user_id,'!=');
	$get_remark->condition('lirm.is_active', 1);
	$get_remark->orderBy('lirm.id','DESC');
	$get_remark->range(0, 1);
	$get_remark_result = $get_remark->execute();
			
	$remark_details = $get_remark_result->fetchAssoc(); 
	
	if($alc_subdivision_code == $worksite_subdivsion_code ){   /** Checking for If the Application can be viewed by the ALC of the Subdivision ****/
	
		$form['#attributes'] = array('enctype' => 'multipart/form-data');
		

		$form['license_employment']['markup_data']					= array(
																			'#markup' 		=> view_ismw_employment_license_alc($license_id,$contractor_user_id,$form_six),
																			'#type' 		=> 'markup',
																			);
																			
		$form['license_employment']['show_status'] 					= array(
																			'#type' 		=> 'markup',
																			'#markup'		=> $current_status,
																			);
																								
		if ($applicant_status != 'B' && $applicant_status != 'V' && $applicant_status != 'I' && $applicant_status != 'VA' && $applicant_status != 'T' && $applicant_status != 'U' ){
			
			$form['license_employment']['hidden_action_message']		= array(
																			'#prefix'		=> '<div class="box box-default box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">ACTIONS AND REMARKS</h3></div><div class="box-body"><span style="margin:0; width:100%;">',
																			'#suffix'		=> '</span>',
																			'#type' 		=> 'markup',
																			'#markup'		=> isset($form_state['values']['hidden_action_message']) ? $form_state['values']['hidden_action_message'] : ''
																		);
			$form['license_employment']['act_id']						= array(
																			'#type'			=> 'hidden',
																			'#default_value'=> !empty($applicant_data['act_id']) ? $applicant_data['act_id'] : '',
																			'#attributes'	=> array('readonly' => 'readonly'),
																		);
																		
			$form['license_employment']['fieldname']				= array(
																			'#title'		=> 'Field Name',
																			'#type'			=> 'hidden',
																			'#attributes'	=> array('id' => 'ismwlicenseEmp_fieldname_alc'),
																			'#default_value'=> !empty($remark_details['remark_field_text']) ? $remark_details['remark_field_text'] : '',
																		);
																										
			$form['license_employment']['contractor_user_id']	    = array(
																			'#type'			=> 'hidden',
																			'#attributes' 	=> array('readonly' => 'readonly'),
																			'#default_value'=> !empty($contractor_user_id) ? encryption_decryption_fun('encrypt',$contractor_user_id) : '',
																			);
																									
			$form['license_employment']['license_id']				= array(
																			'#type'			=> 'hidden',
																			'#default_value'=> !empty($license_id) ? encryption_decryption_fun('encrypt',$license_id ): '',
																			'#attributes' 	=> array('readonly' => 'readonly'),
																			);
																				
			$form['license_employment']['identification_number']	= array(
																			'#type'			=> 'hidden',
																			'#default_value'=> !empty($identification_number) ? encryption_decryption_fun('encrypt',$identification_number ): '',
																			'#attributes' 	=> array('readonly' => 'readonly'),
																			);
																				
			$form['license_employment']['remark_type']				 = array(
																			'#prefix' 		=> '<div class="col-md-3">&nbsp;</div><div class="form-custom col-md-6"><label class="input">',
																			'#suffix' 		=> '<i></i></label>',
																			'#title'		=> 'Please Select Actions',
																			'#type'			=> 'select',
																			'#required' 	=>  TRUE,
																			'#id'			=> 'remark_type_id',
																			'#options'		=> actions_dropdown_by_alc_act($applicant_status,'Y',42),
																			'#attributes'	=> array('onchange' => 'getRemarkTypeAction(this.value)', 'class'=>array('form-control')),
																			'#ajax'			=> array(
																									'event' 			=> 'change',
																									'method' 			=> 'append',
																									'wrapper' 			=> 'content-wrapper',
																									'effect' 			=> 'fade',			
																									'callback'			=> 'get_call_date_time_field',
																									'progress'			=> array('type'=> 'throbber'),
																								  ),
																			);
			$value_remark_type = isset($form_state['values']['remark_type']) ? $form_state['values']['remark_type'] : '';
			
																														
			$form['license_employment']['generate_certificate'] 	= array(
																		  '#prefix' 		=> '<div id="generate_certificate_block">',
																		  '#suffix' 		=> '</div>'
																		);
																							
																							
			$form['license_employment']['download_link'] 			= array(
																		  '#prefix' 		=> '<div id="download_link_block">',
																		  '#suffix' 		=> '</div>'
																		);
			
			$form['license_employment']['ismwemp_signed_license']  = array(
																		  '#prefix' 		=> '<div id="ismw_signed_certificate_block">',
																		  '#suffix' 		=> '</div>'
																		);
			$form['license_employment']['security_fees'] 			= array(
																		  '#prefix' 		=> '<div id="security_fees_block">',
																		  '#suffix' 		=> '</div>'
																		);	
																		
			$form['license_employment']['reason_fees'] 				= array(
																		  '#prefix' 		=> '<div id="reason_fees_block">',
																		  '#suffix' 		=> '</div>'
																		);															
			
			$form['license_employment']['remarks_text'] 			= array(
																		  '#prefix' 		=> '<div id="remarks_text_block">',
																		  '#suffix' 		=> '</div>'
																		);
																							
			$form['license_employment']['submit'] 					= array(
																		  '#prefix' 		=> '<div id="submit_block">',
																		  '#suffix' 		=> '</div>'
																		);
			if($value_remark_type == 'I'){
			
				if(!empty($ismw_license_number) && !empty($iwm_license_qr_code)){
					
				$form['license_employment']['download_link'] 		= array(
																		'#prefix'			=> '<div id="download_link_block">',
																		'#markup' 			=> l(t('<i class="fa fa-download" aria-hidden="true"></i>
 Download ISMW License Certificate'),'generate-ismwlicense-certificate/'.encryption_decryption_fun('encrypt',$iwm_license_qr_code).'/ISMW-EMPLOYMENT-LICENCE', array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn')))),
																		'#suffix' 			=> '</div><div class="clear"></div>',
																		
																		);
				
				 $form['license_employment']['ismwemp_signed_license'] = array(
																		'#title'			=> 'Upload Signed ISMW License Certificate :-',
																		'#type' 			=> 'managed_file',
																		'#upload_validators'=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																		'#upload_location' 	=> 'public://upload/encrypt/',
																		'#process' 			=> array('import_my_file_element_process'),
																		'#attributes' 		=> array('autocomplete' => 'off','class' => array('form-group')),
																		'#prefix'			=> '<div id="ismw_signed_certificate_block">',
																		'#suffix'			=> '</div>',
																		'#required' 		=>  TRUE,
																		);
																									
				$form['license_employment']['remarks_text']	= array(
																		'#prefix' 			=> '<div id="remarks_text_block"><label class="input">',
																		'#suffix' 			=> '</label></div>',
																		'#title'			=> 'Remarks',
																		'#type'				=> 'textarea',
																		'#id'				=> 'remarks_text_id',
																		'#rows'				=>  2,
																		'#required' 		=>  TRUE,
																		);
																									
				$form['license_employment']['submit'] 				= array (
																	  '#prefix' 			=> '<div id="submit_block">',
																	  '#suffix' 			=> '</div></div><div class="col-md-3">&nbsp;</div></div></div>',	
																	  '#type' 				=> 'submit',
																	  '#submit'				=> array('ismw_license_employment_view_submit'),
																	  '#id'					=> 'ismw_empLicSubmitId',
																	  '#attributes' 		=> array('class' => array('btn btn-primary pull-left acremark-btn')),
																	  '#value' 				=> 'Submit'
																  
																);
				
				}else{
					
				   $form['license_employment']['generate_certificate']= array (
																		  '#prefix' 		=> '<div id="generate_certificate_block">',
																		  '#suffix' 		=> '</div>',
																		  '#type' 			=> 'submit',
																		  '#attributes' 	=> array('class' => array('btn btn-primary pull-left acremark-btn')),
																		  '#value' 			=> 'Generate ISMW License Certificate',
																		  '#submit'			=> array('generation_ismw_license_number'),
																		);
				}
		    }
			
			if( $value_remark_type == 'B' || $value_remark_type == 'V' ){
				$form['license_employment']['security_fees'] = array(
										'#prefix' => '<div id="security_fees_block"><label class="input">',
										'#suffix' => '</label></div>',
										'#title' => 'Security Fees',
										'#type'	=> 'textfield',
										'#default_value' =>  $securityFees,
										'#required' =>  TRUE,
										'#element_validate'	=> array('SECURITY_FEES_NUMERIC_VALIDATE'),
										'#description' => '<i class="fa fa-warning" style="color:red;"> Note: Please enter only numeric value. (comma, dot, etc not allowed)</i>'
										);
																		
				$form['license_employment']['reason_fees']		= array(
																		'#prefix' 			=> '<div id="reason_fees_block"><label class="input">',
																		'#suffix' 			=> '</label></div>',
																		'#title'			=> 'Reason of Security Fees on the basis of &nbsp; ',
																		'#type'				=> 'textfield',
																		'#default_value'	=>  $applicant_data['reason_fees'],
																		'#required' 		=>  TRUE
																		);	
			}
		
			
			if($value_remark_type!='I'){	
							
				$form['license_employment']['remarks_text']			= array(
																			'#prefix' 		=> '<div id="remarks_text_block"><label class="input">',
																			'#suffix' 		=> '</label></div>',
																			'#title'		=> 'Remarks',
																			'#type'			=> 'textarea',
																			'#id'			=> 'remarks_text_id',
																			'#rows'			=>  2,
																			'#required' 	=>  TRUE,
																			);
																				
				$form['license_employment']['submit'] 				= array (
																		  '#prefix' 		=> '<div id="submit_block">',
																		  '#suffix' 		=> '</div></div><div class="col-md-3">&nbsp;</div></div></div>',
																		  '#type' 			=> 'submit',
																		  '#id'				=> 'ismw_empLicSubmitId',
																		  '#attributes' 	=> array('class' => array('btn btn-primary pull-left acremark-btn')),
																		  '#value' 			=> 'Submit'
																			);
			}
			
		}
		
		$form['license_employment']['markup_data_5']				= array(
																			'#prefix'		=>'<div class="box box-default box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">REMARK SUMMARY</h3></div><div class="box-body">',
																			'#markup' 	    => $certificateDownloadLink.get_ismw_license_employment_remark_details($license_id,$contractor_user_id),
																			'#suffix' 		=> '</div></div>',
																			);
																									
																									
			
			
			
		
																									
	}else{
		
		$message = 'You are not permitted to view this application.';
		drupal_set_message(t($message), 'error');
		
	}
	
	
	return $form;
	
}
function get_call_date_time_field($form, $form_state){
			
	$commands 	= array();
	$commands[] = ajax_command_replace('#download_link_block', drupal_render($form['license_employment']['download_link']));
	$commands[] = ajax_command_replace('#ismw_signed_certificate_block', drupal_render($form['license_employment']['ismwemp_signed_license']));
	$commands[] = ajax_command_replace('#generate_certificate_block', drupal_render($form['license_employment']['generate_certificate']));
	$commands[] = ajax_command_replace('#security_fees_block', drupal_render($form['license_employment']['security_fees']));
	$commands[] = ajax_command_replace('#reason_fees_block', drupal_render($form['license_employment']['reason_fees']));
	$commands[] = ajax_command_replace('#remarks_text_block', drupal_render($form['license_employment']['remarks_text']));
	$commands[] = ajax_command_replace('#submit_block', drupal_render($form['license_employment']['submit']));	
		
	return array('#type' => 'ajax', '#commands' => $commands);
	
}

function view_ismw_employment_license_alc($license_id='' ,$contractor_user_id= '', $form_six=''){
	 
	drupal_add_js(drupal_get_path('module', 'alc').'/js/mycustom.js'); 
	
	global $base_root, $base_path, $user;
	
	if(is_numeric($contractor_user_id) && is_numeric($license_id) && is_numeric($form_six)){ 
		
		if($license_id!='' && $form_six!=''){
			
			global $base_root, $base_path, $user;
	
			$master_license 	= db_select('l_interstate_license_master', 'liwm');
			$master_license		->  leftJoin('l_interstate_contractor_master', 'con_master', 'con_master.form_six=liwm.form_six');
			$master_license		->  leftJoin('state_master', 'rec_state', 'rec_state.id=liwm.recruited_state');
			$master_license		->  leftJoin('district_master', 'rec_district', 'rec_district.state_id=liwm.recruited_state');
			$master_license 	->  fields('liwm', array());
			$master_license	    ->  fields('rec_state', array('statename'));
			$master_license	    ->  fields('rec_district', array('district_name'));	
			$master_license	    ->  fields('con_master', array('contractor_status','other_nature_of_work','est_date_commencement','est_date_termination'));		
			$master_license 	->condition('liwm.contractor_user_id', $contractor_user_id);
			$master_license  	->condition('liwm.form_six', $form_six);
			$master_license 	 ->condition('liwm.id', $license_id);
			$master_license_result = $master_license->execute();
			
			if($master_license_result->rowCount() > 0){
				
				$licenseDetails_result		= $master_license_result->fetchAssoc(); 
				$securityFees 				= ($licenseDetails_result['security_fees']!='') ? '&#8377;' .$licenseDetails_result['security_fees'].'.00' : '&#8377; 0.00';
				$reasonFees					=  !empty($licenseDetails_result['reason_fees']) ? $licenseDetails_result['reason_fees']: '';
				
				$contractor_address 		= $licenseDetails_result['con_address'].'<br/>'.get_full_address('ismw_license_employment_view', 'sub-table', $license_id, array('con_dist','con_subdivision','con_areatype','con_areatype_code','con_vill_ward','con_ps','con_pin','state_value'));
				
				$conNatureOfWrk 			= getConNatureOfWrk($form_six,$licenseDetails_result['pe_application_id'],4); 
				
				if(!empty($conNatureOfWrk) && $conNatureOfWrk['contractor_work_id']!=''){					
					if($conNatureOfWrk['contractor_work_id']==28){
						$con_nature_work		= $licenseDetails_result['other_nature_of_work'];
					}else{
						$con_nature_work 		= $conNatureOfWrk['cont_work_name'];
					}	
				}
				
				$est_address 			   = $licenseDetails_result['pe_est_address'].'<br/>'.get_full_address('l_interstate_license_master', 'sub-table', $license_id, array('pe_est_dist','pe_est_subdiv','pe_est_areatype','pe_est_areacode','pe_est_vill_ward','pe_est_ps','pe_est_pin_number'));
				$emp_result 				= get_peDetails($licenseDetails_result['pe_application_id'],$licenseDetails_result['pe_user_id']); 
				
				if(!empty($emp_result) && $emp_result['country_code'] == 1){
					
					$emp_address	= $emp_result['address_line'].'<br/>'.get_full_address('l_interstate_employer_master', 'sub-table', $emp_result['id'], array('district_code','subdivision_code','area_type','area_type_code','village_code','police_station','pin','state_code'));
				}else{
					$emp_address	= $emp_result['address_line'];
				}
				$get_peNatureOfWrk 			= pe_natureOfWrk($licenseDetails_result['pe_application_id'],$licenseDetails_result['pe_user_id'],4);
				$father_husband_name		= !empty($licenseDetails_result['father_husband_name']) ? $licenseDetails_result['father_husband_name'] : "Not Applicable";
				
				$contractor_type 			= ($licenseDetails_result['contractor_type'] == 1 ) ? 'Offline' : 'New Contractor';
				$active						= '<span class="badge bg-green"><i class="fa fa-check" aria-hidden="true"></i> Active</span>';
				$deactive					= '<span class="badge bg-red"><i class="fa fa-close"></i> Deactive</span>';
				$contractor_status 			= ($licenseDetails_result['contractor_status'] == 1 ) ? $active : $deactive;
				
				if($licenseDetails_result['agentmanager_country']== 1){
					$agentCountry		= 'India';	
				}else{
					$agentCountry		= '' ;
				}
				
				$agentManagerAddress 	= $licenseDetails_result['address_agentmanager'].'<br/>'.get_full_address('ismw_license_employment_view', 'sub-table', $licenseDetails_result['id'], array('agentmanager_dist','agentmanager_subdv','agentmanager_areatype','agentmanager_areatype_code','agentmanager_vill_ward','agentmanager_ps','agentmanager_pin_number','agentmanager_state'));
				
				$worksite_address		= $licenseDetails_result['worksite_location'].'<br/>'.get_full_address('ismw_license_employment_view', 'sub-table', $licenseDetails_result['id'], array('worksite_dist','worksite_subdiv','worksite_areatype','worksite_areacode','worksite_vill_ward','worksite_ps','worksite_pin_number'));
					
			}
			if( (!empty($licenseDetails_result['licenceno_clra'])) && $licenseDetails_result['licenceno_clra']!='NIL'){ 
				$licenseClraViewLink = '&nbsp;&nbsp;'.l('View Details', 'view-details/licence-clra/'.encryption_decryption_fun('encrypt',$licenseDetails_result['licenceno_clra']).'/'.encryption_decryption_fun('encrypt',42), array('attributes' => array('class' => 'view-link','target' => '_blank'), 'html' => TRUE));
				
				$licenseArray			= explode(',',$licenseDetails_result['licenceno_clra']); 
				$licenseArrayCount		= count($licenseArray);
				
				 for($i=0; $i<=$licenseArrayCount-1; $i++){
					 $REG				= substr($licenseArray[$i], 0, strpos($licenseArray[$i], '-')); 
					 $date				= strtotime(substr($licenseArray[$i],strpos($licenseArray[$i], "-") + 1)); 
					 if(date!='' && !empty($date)){
						$licenseClra 	.= $REG .' on '. date('dS M, Y',$date).'<br/>'; 
					 }else{
						 $licenseClra 		.= $licenseArray[$i];
						 if($licenseArrayCount != 1){ 
						 	$licenseClra 		.= '<br/>'; 
						 }
					 }
				 }
			}else{ 
				$licenseClra = 'NIL';
			}
			$contractor_revoking_date  = !empty($licenseDetails_result['contractor_revoking_date']) ? date('dS M, Y',strtotime($licenseDetails_result['contractor_revoking_date'])) : "NIL";
			if(!empty($licenseDetails_result['recruited_villward_other']) && !empty($licenseDetails_result['recruited_subdiv_other'])) 
				$recruitedVillWardSubdv = $licenseDetails_result['recruited_villward_other'].',&nbsp;'.$licenseDetails_result['recruited_subdiv_other']; 
			elseif(!empty($licenseDetails_result['recruited_villward_other'])&& empty($licenseDetails_result['recruited_subdiv_other']))
				$recruitedVillWardSubdv = $licenseDetails_result['recruited_villward_other'].','; 
			elseif(!empty($licenseDetails_result['recruited_subdiv_other'])&& empty($licenseDetails_result['recruited_villward_other']))
				$recruitedVillWardSubdv = $licenseDetails_result['recruited_subdiv_other'].',';
			else
				$recruitedVillWardSubdv ='';
				
			if(!empty($licenseDetails_result['district_name'])){
				$recruitedDistrict = $licenseDetails_result['district_name'].', ';
			}else{
				$recruitedDistrict = '';
			}
			
			/** DOCUMENTS SECTION **/
			
			$regFORMI 	= db_select('l_documents_upload','ldu')->fields('ldu',array('form_1_signed_pdf_file'))->condition('act_id',4)->condition('user_id',$licenseDetails_result['pe_user_id'])->condition('application_id',$licenseDetails_result['pe_application_id'])->execute()->fetchAssoc();
			
			$docsEncryptedRes = db_select('l_document_tag_master','leud')->fields('leud',array())->condition('act_id',42)->condition('status','1')->condition('user_id',$licenseDetails_result['contractor_user_id'])->condition('application_id',$licenseDetails_result['id'])->execute()->fetchAssoc();
			
			
			/*if($docsEncryptedRes->rowCount() > 0){
				foreach($docsEncryptedRes->fetchObject() as $data)	{ 
					$docs_array[$data->document_type_code]['content'] = 'view_documents/'.encryption_decryption_fun('encrypt',$data->id);
				}
			}*/
			/** Payment Info ***/
			
			if($licenseDetails_result['payment_mode'] == 'G'){	 
				 $govt_pay_query = db_select('l_govt_payment', 'lgp');
				 $govt_pay_query->fields('lgp', array());
				 $govt_pay_query->condition('lgp.application_id', $licenseDetails_result['id']);
				 
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
				 $payment_details .= 'Transaction Voucher Number : <span class="color_orange">'.$tv_number.'</span><br>';
				 $payment_details .= 'Challan Number : <span class="color_orange">'.$challan_number.'</span><br>';
				 $payment_details .= 'Challan Date : <span class="color_orange">'.date('dS M Y', strtotime($challan_date)).'</span><br>';
				 $payment_details .= 'Total amount : <span class="color_orange"> &#8377;'.number_format($total_amount,2).'</span><br>';
				 $payment_details .= 'Challan Details : '.l('<img src='.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png>', '/sites/default/files/upload/challan_upload/'.$govt_paydata['challan_file'], array('html' => TRUE, 'title' => 'Click here to view challan'));
			  }else{		
				 $transaction_details  			= db_select('l_principle_epayments_receive_data', 'lpd');
				 $transaction_details			->leftJoin('l_principle_epayments_data', 'lped', 'lped.identification_no = lpd.transaction_id');
				 $transaction_details			->fields('lped', array('identification_no', 'application_id'));
				 $transaction_details			->fields('lpd', array());
				 $transaction_details			->condition('lped.act_id', '42');
				 $transaction_details			->condition('lped.application_id', $licenseDetails_result['id']);		
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
				
				 $payment_details  = '<b><u> Grips Payment [ Online/Counter ]</u> </b><br/>';
				 $payment_details .= 'Total Amount : <span class="color_orange">&#8377;'.$total_amount.'</span><br>';
				 $payment_details .= 'GRN Number : <span class="color_orange">'.$grn_number.'</span><br>';
				 $payment_details .= 'IFSC Code : <span class="color_orange">'.$bank_code.'</span><br>';
				 $payment_details .= 'Bank Transaction Id : <span class="color_orange">'.$bankTransactionID.'</span><br>';
				 $payment_details .= 'Transaction Date : <span class="color_orange">'.$challan_fid_date.'</span><br>';
				 $payment_details .= 'Transaction Status: '.$payment_status;
			 }
			 
			$get_remark = db_select('l_interstate_remarks_license', 'lirm');
			$get_remark->fields('lirm', array('remark_field_text'));
			$get_remark->condition('lirm.remark_to', trim($contractor_user_id));
			$get_remark->condition('lirm.license_id', trim($license_id));
			$get_remark->condition('lirm.remark_by', trim($contractor_user_id), '!=');
			$get_remark->condition('lirm.is_active', 1);
			$get_remark->orderBy('lirm.id','DESC');
			$get_remark->range(0, 1);
			$get_remark_result = $get_remark->execute()->fetchAssoc();
			$remark_field_text_arr = explode(',', $get_remark_result['remark_field_text']);
			
			foreach ($remark_field_text_arr as $fieldname){
				if($fieldname == 'ownership_type'){ 
					$ownership_type_ck = "checked='checked'";
					/*$est_reg_no_ck = "checked='checked'";
					$dob_ck = "checked='checked'";
					$contractor_name_ck = "checked='checked'";
					$guardian_name_ck = "checked='checked'";
					$designation_ck = "checked='checked'";*/
					
				} 
				/*if($fieldname == 'dob'){ $dob_ck = "checked='checked'";}
				if($fieldname == 'guardian_name'){ $guardian_name_ck = "checked='checked'";}*/
				if($fieldname == 'licenceno_clra'){ $licenceno_clra_ck = "checked='checked'";}
				if($fieldname == 'nameAgentmanager'){ $nameAgentmanager_ck = "checked='checked'";}
				if($fieldname == 'addressAgentmanager'){ $addressAgentmanager_ck = "checked='checked'";}
				if($fieldname == 'worksite_address'){ $worksite_address_ck = "checked='checked'";}	
				if($fieldname == 'convictedReason'){ $convictedReason_ck = "checked='checked'";}	
				if($fieldname == 'revoking_date'){ $revoking_date_ck = "checked='checked'";}
				if($fieldname == 'fiveYears_info'){ $fiveYears_info_ck = "checked='checked'";}	
				if($fieldname == 'workmen_details'){ $workmen_details_ck = "checked='checked'";}
				if($fieldname == 'dp_details'){ $dp_details_ck = "checked='checked'";}
				if($fieldname == 'incharge_details'){ $incharge_details_ck = "checked='checked'";}
				if($fieldname == 'address_recruited') { $address_recruited_ck = "checked='checked'";}
				if($fieldname == 'contractor_migrant_wrkmen') { $contractor_migrant_wrkmen_ck = "checked='checked'";}		
					
				if($fieldname == 'form_six_file'){ $form_six_file_ck = "checked='checked'";}
				if($fieldname == 'workOrder_file'){ $workOrder_file_ck = "checked='checked'";}			
				if($fieldname == 'trade_license_file'){ $trade_license_file_ck = "checked='checked'";}
				if($fieldname == 'addressProof_file'){ $addressProof_file_ck = "checked='checked'";}
				if($fieldname == 'formV_signed_pdf_file'){ $formV_signed_pdf_file_ck = "checked='checked'";}
				if($fieldname == 'other_related_documents'){ $other_related_documents_ck = "checked='checked'";}
				if($fieldname == 'contractLabourLicense_file'){ $contractLabourLicense_file_ck = "checked='checked'";}
			}
		
			if($licenseDetails_result['ownership_type'] == 1 ){ 
		 	
			$contractor_type_value = !empty($licenseDetails_result['ownership_date_value']) ? date('dS M, Y',strtotime($licenseDetails_result['ownership_date_value'])) : "";
		 	$ownershipType 	 = 'Company';
			$rowspan =5;
			$ownershipBlock  = '<tr>
								  <td>1.(b)</td>
								  <td>Company Registration Number </td>
								  <td>'.$licenseDetails_result['est_reg_no'].'</td>
								</tr>
								<tr>
								  <td>1.(c)</td>
								  <td>Date of Company Registration</td>
								  <td>'.$contractor_type_value.'</td>
								</tr>
								<tr>
								  <td>1.(d)</td>
								  <td>Name of Contractor / Applicant </td>
								  <td>'.$licenseDetails_result['contractor_name'].'</td>
								</tr>
								<tr>
								  <td>1.(e)</td>
								  <td>Applicant Designation</td>
								  <td>'.$licenseDetails_result['ownership_designation'].'</td>
								</tr>';
		 }elseif($licenseDetails_result['ownership_type'] == 2 ){
			  $ownershipType 	= 'Individual';
			  $rowspan =4;
			  $contractor_type_value = !empty($licenseDetails_result['ownership_date_value']) ? date('dS M, Y',strtotime($licenseDetails_result['ownership_date_value'])) : "";
			  $ownershipBlock = '<tr>
								  <td>1.(b)</td>
								  <td>Date of Birth and age [in case of individuals]</td>
								  <td>'.$contractor_type_value.'</td>
								</tr>
								<tr>
								  <td>1.(c)</td>
								  <td>Name of Contractor/Applicant  </td>
								  <td>'.$licenseDetails_result['contractor_name'].'</td>
								</tr>
								<tr>
								  <td>1.(d)</td>
								  <td>Fathers / husband&#39;s name [in case of individuals]</td>
								  <td>'.$father_husband_name.'</&#39;td>
								</tr>';
		 }else{
			 $ownershipType = 'Others';
			 $rowspan =3;
			 $ownershipBlock = '<tr>
								  <td>1.(b)</td>
								  <td>Name of the Contractor / Applicant  </td>
								  <td>'.$licenseDetails_result['contractor_name'].'</td>
								</tr>
								<tr>
								  <td>1.(c)</td>
								  <td>Applicant Designation</td>
								  <td>'.$licenseDetails_result['ownership_designation'].'</td>
								</tr>';
		 }
			
										 /* <td><input type="checkbox" id="ownership_type" class="ismw_EMP_verified_alc" '.$ownership_type_ck.' /></td>*/
$output = '';
		
			$output .= '<div class="row equal-panels">
							<div class="col-md-6">
								<div class="box box-primary box-solid">
									<div class="box-header with-border">
									  <h3 class="box-title">1. Establishment Details [ Verified ]</h3>
									  <div class="box-tools pull-right">
										<button type="button" class="btn btn-box-tool" data-widget="collapse"><i class="fa fa-minus"></i>
										</button>
									  </div>
									</div>
									<div class="box-body">
									  <table class="table table-bordered">
										<tbody>
											<tr>
											   <th style="width: 15px">Sl</th>
											  <th>Parameters</th>
											  <th>Inputs</th>
											</tr>
											<tr>
												<td>1.(a)</td>
												<td width="50%">Name and address of establishment</td>
												<td>'.$licenseDetails_result['pe_est_name'].'<br/>'.$est_address.'</td>
											</tr>
											<tr>
												<td>1.(b)</td>
												<td>Type of business, trade,industry, manufacture or occupation, which is carried on in the establishment</td>
												<td>'.$get_peNatureOfWrk.'</td>
											</tr>
											<tr>
												<td>1.(c)</td>
												<td>Number and date of certificate of registration of the establishment under the Act, along with a certified copy thereof and the particulars in FORM-I</td>
												<td>'.$licenseDetails_result['pe_registration_number'].' issued on '.date('dS M, Y',strtotime($licenseDetails_result ['pe_reg_date'])).'&nbsp;&nbsp;'.l('FORM-I',$GLOBALS['base_url'].'/sites/default/files/upload/FORM-I/'.$regFORMI['form_1_signed_pdf_file'],array('attributes' => array('target' => '_blank'), 'html' => TRUE)).'</td>
											</tr>
											<tr>
												<td>1.(d)</td>
												<td>Name and address of the Principal Employer</td>
												<td>'.$emp_result['name'].'<br/>'.$emp_address.'</td>
											</tr>
										</tbody>
									  </table>
									</div>
								</div>
							</div>	
							
							<div class="col-md-6">
								<div class="box box-primary box-solid">
									<div class="box-header with-border">
									  <h3 class="box-title">2. Contractor Details [ Verified ]</h3>
									  <div class="box-tools pull-right">
										<button type="button" class="btn btn-box-tool" data-widget="collapse"><i class="fa fa-minus"></i></button>
									  </div>
									</div>
									<div class="box-body">
									  <table class="table table-bordered">
										<tbody>
											<tr>
											  <th style="width: 15px">Sl</th>
											  <th>Parameters</th>
											  <th>Inputs</th>
											</tr>
											<tr>
												<td>2.(a)</td>
												<td width="50%">Name and address of Contractor&#39;s Establishment</td>
												<td>'.$licenseDetails_result['est_name'].'<br/>'.$contractor_address.'</td>
											</tr>
											<tr>
												<td>2.(b)</td>
												<td>Contractor Status</td>
												<td>'.$contractor_type.'&nbsp;&nbsp;'.$contractor_status.'</td>
											</tr>
											<tr>
												<td>2.(c)</td>
												<td>Nature of work in which migrant workmen are employed or are to be employed in the establishment</td>
												<td>'.$con_nature_work.'</td>
											</tr>
											<tr>
												<td>2.(d)</td>
												<td>Duration of the proposed contract work( give particulars of proposed date of commencing and ending )</td>
												<td>'.date('dS M, Y',strtotime($licenseDetails_result['est_date_commencement'])).' TO '.date('dS M, Y',strtotime($licenseDetails_result['est_date_termination'])).'</td>
											</tr>
											<tr>
												<td>2.(e)</td>
												<td>Maximum number of migrant workmen proposed to be employed in the establishment on any date</td>
												<td>'.$licenseDetails_result['max_num_migrant_wrkmen'].'</td>
											</tr>
										</tbody>
									  </table>
									</div>
								</div>
							</div>	
					  </div>
							
					  
					  <div class="box box-primary box-solid">
						<div class="box-header with-border">
						  <h3 class="box-title">3. Inputs are provided by Contractor [For Verification]</h3>
						</div>
						<div class="box-body">
							 <table class="table table-bordered ismw-table-marge">
							 <tr>
							  <th style="width:10%">Sl</th>
							  <th style="width:40%">Parameters</th>
							  <th style="width:40%">Inputs</th>
							  <th style="width:10%">Verified?</th>
							</tr>
							<tr>
							  <td>1.(a)</td>
							  <td>Ownership Type</td>
							  <td>'.$ownershipType.'</td>
							  <td rowspan="'.$rowspan.'" valign="center" style="vertical-align: middle!important;">
							  <input type="checkbox" id="ownership_type" class="ismw_EMP_verified_alc" '.$ownership_type_ck.' /></td>
							</tr>
							'.$ownershipBlock.'
							<tr>
							  <td>2.</td>
							  <td>Work site address where migrant workmen will be engaged</td>
							  <td>'.$worksite_address.'</td>
							  <td><input type="checkbox"  id="worksite_address" class="ismw_EMP_verified_alc" '.$worksite_address_ck.'/></td>
							</tr>
							<tr>
							  <td>3.(a)</td>
							  <td>Name of Agent / Manager </td>
							  <td>'.$licenseDetails_result['name_agentmanager'].'</td>
							  <td><input type="checkbox" id="nameAgentmanager" class="ismw_EMP_verified_alc" '.$nameAgentmanager_ck.' /></td>
							</tr>
							<tr>
							  <td>3.(b)</td>
							  <td>Address of Agent / Manager </td>
							  <td>'.$agentManagerAddress.'</td>
							  <td><input type="checkbox" id="addressAgentmanager" class="ismw_EMP_verified_alc" '.$addressAgentmanager_ck.' /></td>
							</tr>
							<tr>
							  <td>4. </td>
							  <td><strong>Maximum number of migrant workmen proposed to be employed in the establishment on any date :</strong></td>
							  <td>'.$licenseDetails_result['contractor_migrant_wrkmen'].'<font color="#006600"> <b>[ FEES IS CALCULATED BASED ON THIS VALUE ] </b></font>
							  <button type="button" class="btn btn-info pull-right" data-toggle="modal" data-target="#myModalFeeschat"><i class="fa fa-info-circle"></i>&nbsp;Fees Chart</button></td>
							  <td><input type="checkbox"  id="contractor_migrant_wrkmen" class="ismw_EMP_verified_alc" '.$contractor_migrant_wrkmen_ck.'/></td>
							</tr>
							<tr>
							  <td>5. (a)</td>
							  <td> Whether there was any order against the contractor revoking or suspending licence or forfeiting security deposits in respect of an earlier contract. If so, the date of such order</td>
							  <td>'.$contractor_revoking_date.'</td>
							  <td><input type="checkbox"  id="revoking_date" class="ismw_EMP_verified_alc" '.$revoking_date_ck.'/></td>
							</tr>
							<tr>
							  <td>5.(b)</td>
							  <td>Whether the contractor was convicted of any offence within the preceeding five years. If so, give details:</td>
							  <td>'.$licenseDetails_result['contractor_convicted_reason'].'</td>
							  <td><input type="checkbox"  id="convictedReason" class="ismw_EMP_verified_alc" '.$convictedReason_ck.'/></td>
							</tr>
							<tr>
							  <td>5.(c)</td>
							  <td>Whether the contractor has obtained Licence under the Contract Labour (Regulation & Abolition) Act,1970 pertaining to the establishment where migrant workmen to be employed</td>
							  <td>'.$licenseClra.'<span>'.$licenseClraViewLink.'</span></td>
							  <td><input type="checkbox" id="licenceno_clra" class="ismw_EMP_verified_alc" '.$licenceno_clra_ck.' /></td>
							</tr>
							<tr>
							  <td>5.(d)</td>
							  <td> Whether the contractor has worked in any other establishment within the past five years. If so, give details of the principal employer, establishment and nature of work</td>
							  <td>'.$licenseDetails_result['past_five_years_workdetails'].'</td>
							  <td><input type="checkbox"  id="fiveYears_info" class="ismw_EMP_verified_alc" '.$fiveYears_info_ck.'/></td>
							</tr>
							<tr>
							  <td>6. </td>
							  <td>Address from where migrant workmen proposed to be recruited from</td>
							  <td>'.$licenseDetails_result['recruited_address'].',<br/>'.$recruitedVillWardSubdv.'<br/>'.$recruitedDistrict.'PS- '.$licenseDetails_result['recruited_ps'].',<br/>'.$licenseDetails_result['statename'].',&nbsp;PIN-'.$licenseDetails_result['recruited_pin'].'</td>
							  <td><input type="checkbox"  id="address_recruited" class="ismw_EMP_verified_alc" '.$address_recruited_ck.'/></td>
							</tr>
							</table>';
							$MigrantWorkmen			= db_select('l_interstate_license_employee_master','limwm');
							$MigrantWorkmen			->condition('limwm.license_id', $license_id);
							$MigrantWorkmen			->condition('limwm.user_id', $contractor_user_id);
							$MigrantWorkmen			->condition('limwm.type', 'W');
							$MigrantWorkmen			->condition('limwm.is_active', 1);
							$MigrantWorkmen			-> orderBy('limwm.id', 'DESC');
							$MigrantWorkmen_result 	= $MigrantWorkmen->execute()->fetchAll();
							
							$ResultWorkmen 			= count($MigrantWorkmen_result);
							
							if($ResultWorkmen!='' && $ResultWorkmen>0){
								$NumberOfWorkmen	= 'Number of Workmen :  &nbsp;'.$ResultWorkmen .'&nbsp;&nbsp;'.l('<span class="badge bg-olive margin"><i class="fa fa-eye"></i> View Details</span>','view-details/workmen-info/'.encryption_decryption_fun('encrypt', $license_id).'/'.encryption_decryption_fun('encrypt', 42),array('html'=> TRUE ,'attributes' => array('target' => '_blank')));	
							}else{
								$NumberOfWorkmen	= 'No Workmen Information Added';
							}
							
							$DirectorPartner		= db_select('l_interstate_license_employee_master','limwm');
							$DirectorPartner		->fields('limwm', array('id'));
							$DirectorPartner		->condition('limwm.license_id', $license_id);
							$DirectorPartner		->condition('limwm.type', 'D');
							$DirectorPartner		->condition('limwm.user_id', $contractor_user_id);
							$DirectorPartner		->condition('limwm.is_active', 1);
							$DirectorPartner		->orderBy('limwm.id', 'DESC');
							$DirectorPartner_result = $DirectorPartner->execute()->fetchAll();
							
							$ResultDirectorPartner 	= count($DirectorPartner_result);
							
							if($ResultDirectorPartner!='' && $ResultDirectorPartner>0){
								$NumberOfDPs	= 'Number of Director / Partner:  &nbsp;'.$ResultDirectorPartner .'&nbsp;&nbsp;'.l('<span class="badge bg-olive margin"><i class="fa fa-eye"></i> View Details</span>','view-details/directorpartner-info/'.encryption_decryption_fun('encrypt', $license_id).'/'.encryption_decryption_fun('encrypt', 42),array('html'=> TRUE ,'attributes' => array('target' => '_blank')));	
							}else{
								$NumberOfDPs	= 'No Director/Partner Added';
							}
							
							$getIncharge			= db_select('l_interstate_license_employee_master','lmrme');
							$getIncharge			->fields('lmrme', array());
							$getIncharge			->condition('lmrme.license_id', $license_id);
							$getIncharge			->condition('lmrme.type', 'M');
							$getIncharge			->condition('lmrme.user_id', $contractor_user_id);
							$getIncharge			->condition('lmrme.is_active', 1);
							$getIncharge			->orderBy('lmrme.id', 'DESC');
							$getInchargeResult 		= $getIncharge->execute()->fetchAll();
							
							$ResultIncharge 		= count($getInchargeResult);
							
							if($ResultIncharge!='' && $ResultIncharge>0){
								$NumberOfIncharge	= 'Number of Incharge / Person Responsible:  &nbsp;'.$ResultIncharge .'&nbsp;&nbsp;'.l('<span class="badge bg-olive margin"><i class="fa fa-eye"></i> View Details</span>','view-details/incharge-info/'.encryption_decryption_fun('encrypt', $license_id).'/'.encryption_decryption_fun('encrypt', 42),array('html'=> TRUE ,'attributes' => array('target' => '_blank')));	
							}else{
								$NumberOfIncharge	= 'No Incharge / Person Responsible Added';
							}
				$output  .='<table class="table table-bordered">
							<tr>
							  <td colspan="4" style="font-weight:700; text-transform:uppercase; font-size:15px;">6. Contractor Particulars</td>
							</tr>
							<tr>
								<td style="width:10%">i.) </td>
								<td style="width:40%">Workmen Information </td>
								<td style="width:40%">'.$NumberOfWorkmen.'</td>
								<td style="width:10%"><input type="checkbox" id="workmen_details" class="ismw_EMP_verified_alc" '.$workmen_details_ck.' /></td>
							</tr>
							<tr>
								<td style="width:10%">ii.) </td>
								<td>Directors / Partner&#146;s Information </td>
								<td style="width:40%">'.$NumberOfDPs.'</td>
								<td><input type="checkbox" id="dp_details" class="ismw_EMP_verified_alc" '.$dp_details_ck.' /></td>
							</tr>
							<tr>
								<td style="width:10%">iii.) </td>
								<td>Name(s) and Address(es) of the person(s) in charge of and responsible to the company/firm </td>
								<td style="width:40%">'.$NumberOfIncharge.'</td>
								<td><input type="checkbox" id="incharge_details" class="ismw_EMP_verified_alc" '.$incharge_details_ck.' /></td>
							</tr>		 
							</table>
							<table class="table table-bordered">
							<tr>
							  <td colspan="4" style="font-weight:700; text-transform:uppercase; font-size:15px;">7. License Details for Recruitment Issued by Licensing Officer of the State/ Union Territory from where migrant workers are to be recruited</td>
							</tr>
							<tr>
								<td style="width:10%">i.) </td>
								<td style="width:40%">Recruitment License Number and Date and Validity</td>
								<td style="width:40%">'.$licenseDetails_result['rec_license_no'].' issued on '.date('dS M, Y',strtotime($licenseDetails_result['rec_license_date'])).'. Valid Till : '.date('dS M, Y',strtotime($licenseDetails_result['rec_license_expiry_date'])).'</td>
								<td style="width:10%"></td>
							</tr>
							<tr>
								<td style="width:10%">ii.) </td>
								<td style="width:40%">Recruitment Certificate</td>';
								if($docs_array['ISMW_REC']['content']!=''){
						$output .= '<td style="width:40%">'.l('<i class="fa fa-file fa-lg"></i>',$docs_array['ISMW_REC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'&nbsp;
									<span class="viewfile_popup" id="rec">
										<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
									</span>
									<div id="view_rec"  title="Recruitment Certificate" style="display:none;">
										<iframe src="'.$docs_array['ISMW_REC']['content'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
									</div></td>';
								}else{
									$output .=  '<td style="width:40%">No Document Uploaded</td>';
								}
						$output .=  '<td style="width:10%"></td>
							</tr>
							</table>
							<table class="table table-bordered">
							 <tr>
								<td colspan="4" style="font-weight:700; text-transform:uppercase; font-size:15px;">8. Documents Uploaded</td>
							 </tr>
							 <tr>
								<td style="width:10%">i.)</td>
								<td>FORM-VI</td>';
									if($docsEncryptedRes['form_serial_file']!=''){
										$output .= '<td style="width:40%">'.l(t('<i class="fa fa-file fa-lg">'),'view_documents/'.encryption_decryption_fun('encrypt',$docsEncryptedRes['form_serial_file']), array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('')))).'&nbsp;
														<!--<span class="viewfile_popup" id="form_six">
															<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
														</span>
														<div id="view_form_six"  title="FORM-VI" style="display:none;">
															<iframe src="'.$docs_array['FVI']['content'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
														</div>-->
													</td>';
									}else {
										
										$output .=  '<td style="width:40%">No Document Uploaded</td>';
										
									}
									$output .=  '<td style="width:10%"><input type="checkbox" id="form_six_file" class="ismw_EMP_verified_alc" '.$form_six_file_ck.' /></td>
							</tr>
							<tr>
								<td>ii.)</td>
								<td>Work Order</td>';
									if($docsEncryptedRes['work_order']!=''){
									$output .= '<td style="width:40%">'.l(t('<i class="fa fa-file fa-lg">'),'view_documents/'.encryption_decryption_fun('encrypt',$docsEncryptedRes['work_order']), array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('')))).'&nbsp;
													<!--<span class="viewfile_popup" id="workOrder">
														<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
													</span>
													<div id="view_workOrder"  title="Work Order" style="display:none;">
														<iframe src="'.$docs_array['WO']['content'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
													</div>-->
												</td>';
									}else {
										
										$output .=  '<td>No Document Uploaded</td>';
										
									}
									$output .=  '<td><input type="checkbox" id="workOrder_file" class="ismw_EMP_verified_alc" '.$workOrder_file_ck.' /></td>
							
							</tr>
							<tr>
								<td>iii.)</td>
								<td>Trade License</td>';
									if($docsEncryptedRes['trade_license']!=''){
										$output .= '<td style="width:40%">'.l(t('<i class="fa fa-file fa-lg">'),'view_documents/'.encryption_decryption_fun('encrypt',$docsEncryptedRes['trade_license']), array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('')))).'&nbsp;
													<!--<span class="viewfile_popup" id="trade_license">
														<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
													</span>
													<div id="view_trade_license"  title="Trade License" style="display:none;">
														<iframe src="'.$docs_array['TL']['content'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
													</div>-->
												</td>';
									}else {
										
										$output .=  '<td>No Document Uploaded</td>';
										
									}
									$output .=  '<td><input type="checkbox" id="trade_license_file" class="ismw_EMP_verified_alc" '.$trade_license_file_ck.' /></td>
							</tr>
							<tr>
								<td>iv.)</td>
								<td>Address Proof</td>';
									if($docsEncryptedRes['address_proof']!=''){
									$output .= '<td style="width:40%">'.l(t('<i class="fa fa-file fa-lg">'),'view_documents/'.encryption_decryption_fun('encrypt',$docsEncryptedRes['address_proof']), array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('')))).'&nbsp;
											</td>'; 
									}else {
										
										$output .=  '<td>No Document Uploaded</td>';
										
									}
									$output .=  '<td><input type="checkbox" id="addressProof_file" class="ismw_EMP_verified_alc" '.$addressProof_file_ck.' /></td>
							
							</tr>
							<tr>
								<td>v.)</td>
								<td>Other Document</td>';
								if($docsEncryptedRes['other_document']!=''){
									$output .= '<td style="width:40%">'.l(t('<i class="fa fa-file fa-lg">'),'view_documents/'.encryption_decryption_fun('encrypt',$docsEncryptedRes['other_document']), array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('')))).'&nbsp;
											</td>'; 
									}else {
										
										$output .=  '<td>No Document Uploaded</td>';
										
									}
									$output .=  '<td><input type="checkbox" id="other_related_documents" class="ismw_EMP_verified_alc" '.$other_related_documents_ck.' /></td>
							
							</tr>
							<tr>
								<td>vi.)</td>
								<td>FORM-V</td>';
								 if($docsEncryptedRes['signed_application']!=''){
									 $output .= '<td style="width:40%">'.l(t('<i class="fa fa-file fa-lg">'),'view_documents/'.encryption_decryption_fun('encrypt',$docsEncryptedRes['signed_application']), array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('')))).'&nbsp;';
													if($licenseDetails_result['status']=='T' || $licenseDetails_result['status']=='S' || $licenseDetails_result['status']=='U' || $licenseDetails_result['status']=='I' ){
										 $output .= '&nbsp;&nbsp;&nbsp;
													'.l(t('Generated FORM - V'),'generate-pdf-ismwlicense-employment/'.encryption_decryption_fun('encrypt', $licenseDetails_result['id']).'/'.encryption_decryption_fun('encrypt', $licenseDetails_result['form_six']).'/ISMW-LICENCE-FORM-V', array('html' => TRUE,'attributes'=> array('target'=>'_blank')));
								 					}
									$output .= '</td>';
									  
									
								}else{
									
									$output .=  '<td>Signed Form-V will be uploaded by the Applicant after payment of fees</td>';
								}
										
								$output .= '<td id="formVId"><input type="checkbox" id="formV_signed_pdf_file" class="ismw_EMP_verified_alc" '.$formV_signed_pdf_file_ck.' /></td>
							</tr>';
							if($licenseDetails_result['status'] == 'I'){
				$output .= '<tr>
								<td>vii.)</td>
								<td>ISMW SIGNED LICENSE CERTIFICATE</td>
								<td>'.l(t('<i class="fa fa-download" aria-hidden="true"></i>&nbsp; Signed ISMW License Certificate'),'view_documents/'.encryption_decryption_fun('encrypt',$docsEncryptedRes['certificate']) , array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn')))).'</td>
							</tr>';
							}
				$output .= '</table>	
						    <table class="table table-bordered">
						   <tr>
							<td colspan="3" style="font-weight:700; text-transform:uppercase; font-size:16px;">9. Payment Details</td>
						   </tr>
						   <tr>
							  <td style="width:10%">i.) </td>
							  <td>Total License Fees</td>
							  <td style="width:50%"> &#8377;' .$licenseDetails_result['finalfees'].'.00</td>
							</tr>
							<tr>
							  <td>ii)</td>
							  <td>Total Security Fees <button type="button" class="btn btn-info pull-right" data-toggle="modal" data-target="#myModalDisclaimer"><i class="fa fa-info-circle"></i>&nbsp;Disclaimer</button></td>
							  <td>'.$securityFees.'</td>
							</tr>
							<tr>
								<td>iii.) </td>
								<td>Reason of Security Fees  on the basis of &nbsp; </td>
								<td>'.$reasonFees.'</td>
							</tr>
							<tr>
							<td>iv.) </td>
							<td>Payment Information</td>
							<td>'.$payment_details.'</td>
							</tr>	
						   </table>
						</div>
					  </div> 
					  
					  <div class="modal fade" id="myModalFeeschat" role="dialog">
						<div class="modal-dialog">
							<div class="box box-primary box-solid">
								<div class="box-header">
									<button type="button" class="close" data-dismiss="modal">&times;</button>
									<h3 class="box-title">FEES CHART</h3>
								</div>
								<!--<p>If the Number of Employees proposed to be employed on contract on any day</p>
								<p><strong>[Registration Under Contract Labour (R & A) Act, 1970]</strong></p>-->
							
								<div class="modal-body">
									<table cellpadding="0" cellspacing="0" border="0" class="table table-striped table-responsive admin-custom-table">
										 <tr>
											<th>Sl.No.</th>
											<th>#</th>
											<th>Fees</th>
										 </tr>
										 <tr>
											 <td>1</td>
											 <td>Is 5 or more but does not exceed 20 </td>
											 <td>₹10</td>
										 </tr>
										 <tr>
											 <td>2</td>
											 <td>Exceeds 20 but does not exceed 50</td>
											 <td>₹20</td>
										 </tr>
										 <tr>
											 <td>3</td>
											 <td>Exceeds 50 but does not exceed 100</td>
											 <td>₹40</td>
										 </tr>
										 <tr>
											 <td>4</td>
											 <td>Exceeds 100 but does not exceed 200</td>
											 <td>₹80</td>
										 </tr>
										 <tr>
											 <td>5</td>
											 <td>Exceeds 200 but does not exceed 400</td>
											 <td>₹160</td>
										 </tr>
										 <tr>
											 <td>6</td>
											 <td>Exceeds 400 but does not exceed 750</td>
											 <td>₹200</td>
										 </tr>
										 <tr>
											 <td>7</td>
											 <td>Exceeds 750 but does not exceed 1000</td>
											 <td>₹250</td>
										 </tr>
										 <tr>
											 <td>8</td>
											 <td>Exceeds 1000 but does not exceed 2000</td>
											 <td>₹350</td>
										 </tr>
										 <tr>
											 <td>9</td>
											 <td>Exceeds 2000</td>
											 <td>₹500</td>
										 </tr>
									  </table>
									</div>
								<div class="modal-footer">
									<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
								</div>
							</div>      
						</div>
					</div>
					
					<div class="modal fade" id="myModalDisclaimer" role="dialog">
						<div class="modal-dialog">
							<div class="box box-primary box-solid">
								<div class="box-header">
									<button type="button" class="close" data-dismiss="modal">&times;</button>
									<h3 class="box-title">DISCLAIMER</h3>
								</div>
								<div class="modal-body">
									<p>Estimate for security may be prepared on the amount  needed  to  provide  for  employment of migrant section on the basis of the number of workmen employed, the wages payable to them, the facilities which shall be afforded to them and other relevant factors.</p>
								</div>
								<div class="modal-footer">
									<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
								</div>
							</div>      
						</div>
					</div>';
		}
	}else{
		
		drupal_set_message(t('Invalid FORM-VI for this application.'), 'error');
		drupal_set_message(t($message), 'error');
	}
		return $output;
}

function ismw_license_employment_view_submit($form,&$form_state){ 
	
	global $base_root, $base_path, $user;
	
	$alc_user_id 			= $user->uid; 
	
	$val 					= $form_state['values'];
	$identification_number	= encryption_decryption_fun('decrypt',$val['identification_number']);
	$contractor_user_id 	= encryption_decryption_fun('decrypt',$val['contractor_user_id']);
	$license_id 			= encryption_decryption_fun('decrypt',$val['license_id']);
	$security_fees 			= $val['security_fees'];
	$reason_fees 			= $val['reason_fees'];
	$fieldname				= $val['fieldname'];
	$remark_type			= $val['remark_type'];
	$remarks_text			= $val['remarks_text']; 
	
	$get_licenseDetails					=   db_select('l_interstate_license_master', 'lilm');
	$get_licenseDetails	   				->  fields('lilm',array('form_six','licence_type','contractor_user_id','license_number','license_date','license_expiry_date'));
	$get_licenseDetails	   				->  condition('lilm.id', $license_id);
	$get_licenseDetails	   				->  condition('lilm.contractor_user_id', $contractor_user_id);
	$licenseDetails_result  			=   $get_licenseDetails->execute()->fetchAssoc();
	
	
	if(!empty($licenseDetails_result) && $licenseDetails_result['form_six']!=''){
		
		if(!empty($val['ismwemp_signed_license'])){
			
			db_update('l_encrypted_uploaded_documents')->fields(array('status'=> '0'))->condition('document_type_code','ISMWEMPLC')->condition('user_id',$contractor_user_id)->condition('status','1')->condition('act_id',42)->condition('application_id',$license_id)->execute();
			
			
			$upload_doc			= file_load($val['ismwemp_signed_license']); 
			$upload_doc_arr 	= explode("/", $upload_doc->uri); 
			$upload_doc_fname	= time().'_'.$upload_doc_arr[4]; 
			$filepathSave_ap	= file_get_contents(escapefile_url($base_root.$base_path.'sites/default/files/upload/encrypt/'.$upload_doc_arr[4]));
			$upload_doc_content = base64_encode($filepathSave_ap);
			$insert_arr['ISMWEMPLC']['fname'] = $upload_doc_fname;
			$insert_arr['ISMWEMPLC']['content'] = $upload_doc_content;
			$arr = array(
				'user_id' 		=> $contractor_user_id,
				'act_id' 		=> 42,
				'document_type' => 'LICENSE CERTIFICATE EMPLOYMENT ISMW',
				'document_type_code' => 'ISMWEMPLC',
				'filename'		=> time().$insert_arr['ISMWEMPLC']['fname'],
				'filecontent'	=> $insert_arr['ISMWEMPLC']['content'],
				'update_date'	=> date('Y-m-d H:i:s'),
				'status'		=> '1',
				'application_id'=>$license_id,
				
			);
		
		$encryptedId = db_insert('l_encrypted_uploaded_documents')->fields($arr)->execute();
		db_update('l_document_tag_master')->fields(array('certificate'=>$encryptedId))->condition('application_id',$license_id)->condition('act_id',42)->condition('user_id',$contractor_user_id)->execute();
		}
		
	
		/** Get User role of ALC ***/
		
		$fetch_users_role		= db_select('users_roles', 'ur');
		$fetch_users_role		->leftJoin('role', 'ro', 'ro.rid=ur.rid');
		$fetch_users_role		->leftJoin('l_custom_user_detail', 'lcud', 'ur.uid=lcud.usr_id');
		$fetch_users_role		->fields('ro', array('rid'));
		$fetch_users_role		->fields('lcud', array('fullname','employee_id'));
		$fetch_users_role		->condition('ur.uid', $alc_user_id);
		$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc(); 
	
		$remark_by_role			=	$fetch_users_role_data['rid'];
		$alc_full_name			=   $fetch_users_role_data['fullname'];
		$employee_id			= 	$fetch_users_role_data['employee_id'];
	
		/** UPDATE LICENSE MASTER **/
		$update_status  =  db_update('l_interstate_license_master');
		if($remark_type == 'B' || $remark_type == 'V'){
			$update_status->fields(array('security_fees' => $security_fees, 'reason_fees' =>$reason_fees,'status' => $remark_type ));
		}else{
			$update_status->fields(array('status' => $remark_type));
		}
		$update_status->condition('id',$license_id);
		$update_status->condition('contractor_user_id',$contractor_user_id);
		$update_status->execute();
		
	
	if(!empty($val['ismwemp_signed_license']) && $remark_type == 'I'){
		
		db_update('l_interstate_contractor_master')->fields(array('contractor_user_id' =>$contractor_user_id))->condition('form_six',$licenseDetails_result['form_six'])->execute();
		
		$tag_data = db_update('l_interstate_tag_master');
		$tag_data->fields(array('status' => 'I','license_number' => $licenseDetails_result['license_number'],'license_date' => $licenseDetails_result['license_date'],'license_validity_date' => $licenseDetails_result['license_expiry_date']));
		$tag_data->condition('license_id',$license_id);
		$tag_data->condition('contractor_user_id',$contractor_user_id);
		$tag_data->condition('form_six',$licenseDetails_result['form_six']);
		$tag_data->condition('identification_number', $identification_number);
		$tag_data->condition('act_id', 42);
		$tag_data->execute();

		// $msg = 'Congratulations! Certificate is issued by the Registering Authority. You can download it from the dashboard. <br>You may avail the Amendment and the other services from <strong>wblc.gov.in</strong> with the user credentials of <strong>Shilpa Sathi</strong> (<strong>silpasathi.wb.gov.in</strong>)';
				// $applicant_mailto  = get_email($contractor_user_id);
				// $subject = 'Welcome to Labour Commissionerate, Govt. Of West Bengal';
				// if(!empty($applicant_mailto)){
				// 	send_mail_for_user_alert($applicant_mailto, $subject, $msg);
				// }
	}
	
	/** Insert In ISMW LICENSE Remarks Master Table **/
	
		
	$fieldsApplInfo2  =  array(
		'remark_by'   			=> $alc_user_id,
		'remark_to' 			=> $contractor_user_id, 
		'remark_type' 			=> $remark_type,
		'remark_field_text' 	=> $fieldname,
		'remark_by_role' 		=> $remark_by_role,
		'remarks' 				=> $remarks_text,
		'license_id' 			=> $license_id,
		'remark_by_name'    	=> $alc_full_name,
		'emp_id'				=> $employee_id,
		'form_six'				=> $licenseDetails_result['form_six'],
		'remark_date'  			=> time(), //date("Y-m-d h:i:sa"),
		'licence_type' 			=> $licenseDetails_result['licence_type'],
		'flag'					=>  'L'
	);
	
	db_insert('l_interstate_remarks_license')->fields($fieldsApplInfo2)->execute(); 	
		
	$message = 'Remark has been successfully saved.';
	drupal_set_message(t($message));
	}	
}

function generation_ismw_license_number($form, &$form_state){
	
	$val  				= $form_state['values'];
	$contractor_user_id	= encryption_decryption_fun('decrypt',$val['contractor_user_id']);
	$license_id			= encryption_decryption_fun('decrypt',$val['license_id']);
	$identification_no	= encryption_decryption_fun('decrypt',$val['identification_number']);
	$fieldname			= $val['fieldname'];
	$act_id				= 42;
	
	/*** Get Applicant Block Code & Statuses & ALC Details ****/
	
	$ismw_emp_license							=	db_select('l_interstate_license_master', 'lbrm');
	$ismw_emp_license							->  leftJoin('l_customuser_relation_address', 'lcra','lcra.sub_div_code=lbrm.worksite_subdiv ');
	$ismw_emp_license							->	fields('lbrm',array('worksite_subdiv','worksite_areacode','license_number','qr_code','pe_application_id','form_six','pe_user_id','identification_number'));
	$ismw_emp_license							->	fields('lcra',array('sub_div_code'));
	$ismw_emp_license							->	condition('lbrm.id',$license_id);
	$ismw_emp_license							->	condition('lbrm.contractor_user_id',$contractor_user_id);
	$ismw_emp_license							->	condition('lbrm.identification_number',$identification_no);
	
	$ismw_emp_license_result 					= 	$ismw_emp_license->execute();
	if($ismw_emp_license_result-> rowCount() > 0 ){
		
		$applicant_data 							= 	$ismw_emp_license_result->fetchAssoc();
		$subdivision_code							=   $applicant_data['worksite_subdiv'];
		$area_code									=   $applicant_data['worksite_areacode'];
		$license_number								=   $applicant_data['license_number'];
		$iwm_license_qr_code						=   $applicant_data['qr_code']; 
		$alc_subdiv_code							=   $applicant_data['sub_div_code'];
		$pe_application_id							=   $applicant_data['pe_application_id'];
		$form_six									=   $applicant_data['form_six'];
		$pe_user_id									=   $applicant_data['pe_user_id'];
		$identification_number						=   $applicant_data['identification_number'];
		
		if (empty($license_number) && empty($iwm_license_qr_code)){
		
			/*** GENERATION OF QR CODE ****/
			
			 $generated_qr_code					= 'ISMW-E-LIC'.$license_id.'A42U'.$contractor_user_id.'T'.time();
		
		
			/*** GENERATION OF ISMW LICENSE NUMBER ***/
			
			
			$getShortNameSubdivision 			=   custom_user_short_name_fun($subdivision_code);   
			$getRegistrarCode					=	get_registration_code($area_code);              
			$registrarCode 						=   substr($getRegistrarCode, -2);
			
			$reg_query							=	db_query("select  max (NULLIF(substr(license_number,13,6),'') :: integer) as serial_num  from l_interstate_license_master where worksite_subdiv='".$alc_subdiv_code."'");
			
			$reg_query_result					=	$reg_query->fetchAssoc();
			
			$reg_code							=	$reg_query_result['serial_num'];
			
			
			
			
			if(empty($reg_code)){ 
				
						$license_number			=	$getShortNameSubdivision.$registrarCode.'/E/'.'IML'.'/'.'000001';

			}else{
					  	$reg_code_next			= 	$reg_code+1; 
					  	$reg_first				=	$getShortNameSubdivision.$registrarCode.'/E/'.'IML'.'/';
					  	$reg_second				=	str_pad($reg_code_next, 6, "0", STR_PAD_LEFT);
					  	$license_number			=	$reg_first.$reg_second;
			}
			
			
					
			/*** Update Status of ISMW Master License Table ***/ 
			
			$update_status  	=  db_update('l_interstate_license_master');
			$update_status->fields(array('qr_code' => $generated_qr_code, 'license_number' =>	$license_number, 'license_date' => date("Y-m-d"),'license_expiry_date' => date('Y').'-12-31' ,'modified_date'=> date("Y-m-d h:i:sa"))); //date('Y-m-d', strtotime(' + 364 days'))
			$update_status->condition('id',$license_id);
			$update_status->condition('act_id',$act_id);
			$update_status->condition('contractor_user_id',$contractor_user_id);
			$update_status->condition('identification_number',$identification_no);
			$update_status->execute();
			
			$sql 	=  db_query("UPDATE l_interstate_remarks_license SET remark_field_text ='".$fieldname."' WHERE id IN (SELECT id FROM l_interstate_remarks_license WHERE license_id =".$license_id." AND form_six = ".$form_six." AND remark_by!=".$contractor_user_id." order by id DESC LIMIT 1)");
						
			$message									   = drupal_set_message_custom(t('License Certificate has been successfully generated. Please Sign and Upload Certificate to complete this process'));
 			$form_state['values']['hidden_action_message'] = $message;
			$form_state['rebuild']							= TRUE;
	
	}
	}
	
}


function get_ismw_license_employment_remark_details($license_id,$contractor_user_id){
	
	global $base_root, $base_path, $user;
	
	$user_id = $user->uid;
	
	$counter					= 0;
	
	$header = array(
		array('data' => 'Sl', 'field' => 'slno','width' => '5%'),
		array('data' => 'Date - Time', 'field' => 'date','width' => '15%'),
		array('data' => 'Remark', 'field' => 'remrk'),
		array('data' => 'Remark Status', 'field' => 'remark type','width' => '15%'),
		array('data' => 'Remark By', 'field' => 'licenseno','width' => '20%'),
		array('data' => 'Action','width' => '10%')
	);
	
	$get_remark = db_select('l_interstate_remarks_license', 'lirm');
	$get_remark	->leftJoin('role', 'ro', 'ro.rid = lirm.remark_by_role');
	$get_remark->fields('lirm', array());
	$get_remark->fields('ro', array('name'));
	$get_remark->condition('lirm.remark_to', trim($contractor_user_id));
	$get_remark->condition('lirm.license_id', trim($license_id));
	$get_remark->condition('lirm.remark_by', trim($contractor_user_id),'!=');
	$get_remark->condition('lirm.is_active', 1);
	$get_remark->orderBy('lirm.id','DESC');
	$get_remark_result = $get_remark->execute()->fetchAll(); 
	
	
	foreach($get_remark_result as $row){ 
			
			$counter 		= $counter+1;
			$remark_by		= $row->remark_by_name.'&nbsp;('.$row->name.')';
			
			// get latest remark given by session user
			
			$getPayInfo	 	= db_query("SELECT COUNT(*) AS total_row FROM l_principle_epayments_data WHERE act_id=42 AND application_id = ".$license_id."")->fetchObject();
			$getId		 	= $getPayInfo->total_row;
			
			$getMaxData		= db_query("SELECT MAX(id) AS MAXID FROM l_interstate_remarks_license WHERE is_active=1 and license_id = ".$license_id." and remark_to=".$contractor_user_id."")->fetchObject();
			$maxId			= $getMaxData->maxid;
			
			if($maxId>0){
				if($row->remark_type=='U'){
					$url 			= 'delete-remarks/'.encryption_decryption_fun('encrypt',$row->id).'/'.encryption_decryption_fun('encrypt',$license_id).'/'.encryption_decryption_fun('encrypt',$contractor_user_id).'/'.encryption_decryption_fun('encrypt',42);	
					$delete			= l('<div id="delBtn" class="delete-btn active-del">&nbsp;</div>',$url,array('html' => TRUE));
					
				}else{ 
					if(($row->remark_by==$user_id) &&($maxId==$row->id) && ($getId!=0 || $getId!='')){
						$url 	= 'delete-remarks/'.encryption_decryption_fun('encrypt',$row->id).'/'.encryption_decryption_fun('encrypt',$license_id).'/'.encryption_decryption_fun('encrypt',$contractor_user_id).'/'.encryption_decryption_fun('encrypt',42);	
						$delete	= l('<div id="delBtn" class="delete-btn active-del">&nbsp;</div>',$url,array('html' => TRUE));	
					}else{
						$delete	= '<div id="delBtn" class="delete-btn disable-del">&nbsp;</div>';
					}	
				}
				
			}
			$rows[] 	= array(
				
					array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
					array('data' => date('dS M, Y - g:i a', $row->remark_date), 'align' => 'left', 'class' => array('odd')),
					array('data' => $row->remarks, 'align' => 'left', 'class' => array('odd')),
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

function SECURITY_FEES_NUMERIC_VALIDATE($element, &$form_state){
	if(!is_numeric($form_state['values']['security_fees'])){
		form_error($element, t($element['#title'].' should be numeric (comma, dot, etc not allowed).'));
	}
}

