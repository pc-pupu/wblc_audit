<?php

drupal_add_js(drupal_get_path('module', 'alc').'/js/mycustom.js');

function alc_view_contractor_license_renewal($form, &$form_state, $renewalId='',$licenseid=''){
	
	global $base_root, $base_path, $user;
	
	$user_id			= $user->uid;
    $renewal_id 		= encryption_decryption_fun('decrypt', $renewalId);
	$license_id			= encryption_decryption_fun('decrypt', $licenseid);
	$act_id				= 13;
	$current_status		= '';	
	
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
	
	
	/*** Get Applicant Subdivision Code & Statuses ****/
	
	$get_renewalDetails	 = db_select('l_contractor_license_application', 'license');
	$get_renewalDetails	 ->leftJoin('l_contractor_license_apply_renweal', 'renewal','renewal.lincense_id = license.id and license.contractor_user_id = renewal.created_by');
	$get_renewalDetails	 ->leftJoin('l_particular_of_contract_labour', 'particular','particular.serial_no_from_v = license.serial_no_from_v and particular.contractor_user_id = license.contractor_user_id');
	$get_renewalDetails	 ->fields('license',array('serial_no_from_v','id'));
	$get_renewalDetails	 ->fields('renewal',array('renewal_appliaction_status','remark_by_userid','renewal_certificate_id','created_by','id','is_flag','parent_id','license_reneal_fees','renewal_application_final_status','is_autorenewal'));
	$get_renewalDetails	 ->fields('particular',array('worksite_subdivision','id'));
	$get_renewalDetails	 ->condition('particular.worksite_subdivision',$alc_subdivision_code);
	$get_renewalDetails	 ->condition('license.id',$license_id);
	$get_renewalDetails	 ->condition('renewal.id',$renewal_id);
	$renewalDetails_result = $get_renewalDetails->execute(); 
	
	if($renewalDetails_result-> rowCount() > 0 ){
		
		$applicant_data 			= $renewalDetails_result->fetchAssoc(); 
		$applicant_status			= $applicant_data['renewal_appliaction_status'];
		$worksite_subdivsion_code	= $applicant_data['worksite_subdivision'];
		$serialFormV				= $applicant_data['serial_no_from_v'];
		$particular_id				= $applicant_data['particular_id'];
		
		
		/*** Get REMARK Details ****/
	
		$get_remark = db_select('l_remark_license', 'remark_license');
		$get_remark->fields('remark_license', array('remark_field_title','remark_by','remark_by_role'));
		$get_remark->condition('remark_license.particular_id', $renewal_id);
		$get_remark->condition('remark_license.flag', 'R');
		$get_remark->condition('remark_license.remark_to', $serialFormV);
		$get_remark->condition('remark_license.is_active', 1);
		$get_remark->where('((remark_license.remark_by_role = 4) OR (remark_license.remark_by_role = 7))');
		$get_remark->orderBy('remark_license.id','DESC');
		$get_remark->range(0, 1);
		$get_remark_result = $get_remark->execute()->fetchAssoc();
		
		if(!empty($applicant_status)){
			
			if($applicant_status == "F"){
				$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Pending application</h4><p>Application is applied by the Applicant. Any action can be taken for the application.</p></div>';
			}
			
			if($applicant_status == "B"){
				$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i>CURRENT STATUS: Back for rectification</h4><p>Application is sent back for rectification. After modification by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
			}
			
			if( $applicant_status == "FW"){
				$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Forwarded to ALC by Inspector</h4><p>Application is Forwarded to ALC by Inspector for further verification. Any action can be taken for the application.</p></div>CURRENT STATUS: Application has been FORWARDED TO ALC';
			
			}	
			
			if($applicant_status == "A") { 
				$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Verified and approved for fees submission. </h4><p>Application is approved and directed to pay fees. After fees payment and submission of signed FORM - VII by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
			}
			
			if( $applicant_status == "I"){
				$current_status = '<div class="callout callout-success"><h4>CURRENT STATUS: Certificate (FORM-VI) is issued. </h4><p>Please check the Renewal Certificate after uploading the Certificate.</div>';
			}
			
			if( $applicant_status == "P" && $get_remark_result['remark_by_role'] == 8 ){
				$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Transaction successful</h4><p>Payment successful for this application. FORM-VII is not uploaded by the applicant. After submission of signed FORM-VII by the applicant, the application can be further accessible.</p></div>';
			}
			
			if( $applicant_status == "U"){
				 $current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS:  FORM-VII uploaded & application succesfully submitted</h4><p>FORM-VII is submitted by the Applicant. After verification of uploaded FORM-VII, Renewal Certificate can be generated now or can be sent back for rectification of FORM-VII.</p></div>';
			
			}
			
			if($applicant_status == "R"){
				$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Rejected application</h4><p>If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
			}
			
			if( $applicant_status == "BI" ){
				$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i>CURRENT STATUS:  Back to inspector for rectification</h4><p>Application is sent back to Inspector for further verification. After verification, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
			}
			
			if( $applicant_status == "P" && $get_remark_result['remark_by_role'] == $alc_role_rid){
				 $current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i>CURRENT STATUS: Back for Rectification(FORM-VII)</h4><p>FORM-VII is sent back for rectification. After modification by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
			
			}
		}
		
		if($alc_subdivision_code == $worksite_subdivsion_code && $renewal_id == $applicant_data['renewal_id']){   /** Checking for If the Application can be viewed by the ALC of the Subdivision ****/
			
			$form['#attributes'] 							= array('enctype' 		=> 'multipart/form-data');
			
			$form['view_renewal']['markup_data']			= array(
																'#markup' 			=> view_renewal_details($serialFormV,$renewal_id,$particular_id,$license_id),
																'#type' 			=> 'markup'
															);
															
			$form['amend_bocwa']['base_url'] 				= array(
																  '#type' 			=> 'hidden',
																  '#value' 			=> $base_root.$base_path,				  
																);
															
			$form['view_renewal']['show_status'] 			= array(
																'#type' 			=> 'markup',
																'#markup'			=> $current_status,
																);
																
			$form['view_renewal']['act_id']					= array(
																'#type'				=> 'hidden',
																'#default_value'	=> !empty($act_id) ? $act_id: '',
																'#attributes' 		=> array('readonly' => 'readonly'),
																);
																
			if ($applicant_status != 'B' && $applicant_status != 'A' && $applicant_status != 'I' && $applicant_status != 'P' && $applicant_status != 'R'){
				
				$form['view_renewal']['hidden_message']		= array(
																'#prefix'			=> '<div class="box box-default box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">ACTIONS AND REMARKS</h3></div><div class="box-body"><span style="margin:0; width:100%;">',
																'#suffix'			=> '</span>',
																'#type' 			=> 'markup',
																'#markup'			=> isset($form_state['values']['hidden_message']) ? $form_state['values']['hidden_message'] : ''
															);
														
				$form['view_renewal']['renewal_fieldname']	= array(
																'#type'				=> 'hidden',
																'#attributes'		=> array('id' => 'renewal_fieldname', 'readonly'=>'readonly' ),
																'#default_value'	=> !empty($get_remark_result['remark_field_title']) ? $get_remark_result['remark_field_title'] : '',
																);
																
				$form['view_renewal']['renewal_id'] 		= array(
																'#type' 			=> 'hidden',
																'#default_value'	=> $renewal_id!='' ? $renewalId : '',
																'#attributes'		=> array('readonly'=>'readonly'),
																);
				$form['view_renewal']['formV'] 				= array(
																'#type' 			=> 'hidden',
																'#default_value'	=> $serialFormV!='' ? encryption_decryption_fun('encrypt', $serialFormV) : '',
																'#attributes'		=> array('readonly'=>'readonly'),
																);												
				$form['view_renewal']['created_by'] 		= array(
																'#type' 			=> 'hidden',
																'#default_value'	=> $applicant_data['created_by']!='' ? encryption_decryption_fun('encrypt', $applicant_data['created_by']) : '',
																'#attributes'		=> array('readonly'=>'readonly'),
																);	
																
				$form['view_renewal']['remark_type']		= array(
																'#prefix' 			=> '<div class="col-md-3">&nbsp;</div><div class="form-custom col-md-6"><label class="input">',
																'#suffix' 			=> '<i></i></label>',
																'#title'			=> 'Please Select Actions',
																'#type'				=> 'select',
																'#required' 		=>  TRUE,
																'#id'				=> 'remark_type_id',
																'#options'			=> actions_dropdown_by_alc_act($applicant_status,'Y',13),
																'#attributes'		=> array('onchange' => 'getRemarkTypeAction(this.value)', 'class'=>array('form-control')),
																'#ajax'				=> array(
																						'event' 			=> 'change',
																						'method' 			=> 'append',
																						'wrapper' 			=> 'content-wrapper',
																						'effect' 			=> 'fade',			
																						'callback'			=> 'get_call_date_time_field',
																						'progress'			=> array('type'=> 'throbber'),
																					  ),
																);
																
				$value_remark_type = isset($form_state['values']['remark_type']) ? $form_state['values']['remark_type'] : '';
				
									
				$form['view_renewal']['download_link'] 		= array(
																  '#prefix' 		=> '<div id="download_link_block">',
																  '#suffix' 		=> '</div>'
																);
				
				$form['view_renewal']['signed_renewal']  	= array(
																  '#prefix' 		=> '<div id="signed_license_certificate_block">',
																  '#suffix' 		=> '</div>'
																);																
				
				$form['view_renewal']['remarks_text'] 		= array(
																  '#prefix' 		=> '<div id="remarks_text_block">',
																  '#suffix' 		=> '</div>'
																);
																								
				$form['view_renewal']['submit'] 			= array(
																  '#prefix' 		=> '<div id="submit_block">',
																  '#suffix' 		=> '</div>'
																);
				
				if($value_remark_type == 'I'){
				
					$form['view_renewal']['download_link'] 	= array(
																'#prefix'			=> '<div id="download_link_block">',
																'#markup' 			=> l(t('<i class="fa fa-download" aria-hidden="true"></i>
	Download Renewal Certificate'),'clra-license-amendment-renewal-certificates/'.encryption_decryption_fun('encrypt', $serialFormV ).'/'.encryption_decryption_fun('encrypt',$renewal_id).'/'.encryption_decryption_fun('encrypt', $applicant_data['created_by']).'/'.$licenseid.'/'.encryption_decryption_fun('encrypt','R').'/Renewal Certificates'.time(), array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn')))),
																'#suffix' 			=> '</div>',
																);
															
															
					$form['view_renewal']['signed_renewal']	= array(
																'#title'			=> 'Upload Signed Renewal Certificate :-',
																'#type' 			=> 'managed_file',
																'#upload_validators'=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																'#upload_location' 	=> 'public://upload/alc_upload_certificates_renewal_license/',
																'#process' 			=> array('import_my_file_element_process'),
																'#attributes' 		=> array('autocomplete' => 'off','class' => array('form-group')),
																'#prefix'			=> '<div id="signed_license_certificate_block">',
																'#suffix'			=> '</div>',
																'#required' 		=>  TRUE,
																);
																
					$form['view_renewal']['remarks_text']	= array(
																'#prefix' 			=> '<div id="remarks_text_block"><label class="input">',
																'#suffix' 			=> '</label></div>',
																'#title'			=> 'Remarks',
																'#type'				=> 'textarea',
																'#id'				=> 'remarks_text_id',
																'#rows'				=>  2,
																'#required' 		=>  TRUE,
																);
																
					$form['view_renewal']['submit'] 		= array (
															  '#prefix' 			=> '<div id="submit_block">',
															  '#suffix' 			=> '</div></div><div class="col-md-3">&nbsp;</div></div></div>',	
															  '#type' 				=> 'submit',
															  '#attributes' 		=> array('class' => array('btn btn-primary pull-left acremark-btn')),
															  '#value' 				=> 'Submit'
														);
				
				}
				
				if($value_remark_type!='I'){	
							
					$form['view_renewal']['remarks_text']	= array(
																	'#prefix' 		=> '<div id="remarks_text_block"><label class="input">',
																	'#suffix' 		=> '</label></div>',
																	'#title'		=> 'Remarks',
																	'#type'			=> 'textarea',
																	'#id'			=> 'remarks_text_id',
																	'#rows'			=>  2,
																	'#required' 	=>  TRUE,
																	);
																					
					$form['view_renewal']['submit'] 		= array(
																  '#prefix' 		=> '<div id="submit_block">',
																  '#suffix' 		=> '</div></div><div class="col-md-3">&nbsp;</div></div></div>',
																  '#type' 			=> 'submit',
																  '#id'				=> 'ismw_empLicSubmitId',
																  '#attributes' 	=> array('class' => array('btn btn-primary pull-left acremark-btn')),
																  '#value' 			=> 'Submit'
																	);
				}
			}
			
			$form['view_renewal']['show_remark_details'] 	= array(
																'#prefix'			=>'<div class="box box-default box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">REMARK SUMMARY</h3></div><div class="box-body"><div class="feedback-scroll">',
																'#type' 			=> 'markup',
																'#markup'			=> renewal_remark_details($serialFormV,$renewal_id,$applicant_data['created_by'],$license_id,$applicant_data['is_autorenewal']),
																'#suffix' 			=> '</div></div></div>',
									 						 );
		}else{
			drupal_set_message(t('You are not permitted to view this application.'), 'error');
		}
	}
	
	return $form;
}

function get_call_date_time_field($form, $form_state){
			
	$commands 	= array();
	$commands[] = ajax_command_replace('#download_link_block', drupal_render($form['view_renewal']['download_link']));
	$commands[] = ajax_command_replace('#signed_license_certificate_block', drupal_render($form['view_renewal']['signed_renewal']));
	$commands[] = ajax_command_replace('#remarks_text_block', drupal_render($form['view_renewal']['remarks_text']));
	$commands[] = ajax_command_replace('#submit_block', drupal_render($form['view_renewal']['submit']));	
		
	return array('#type' => 'ajax', '#commands' => $commands);
	
}

function view_renewal_details($serialFormV='',$renewal_id='' ,$particular_id='', $license_id =''){
	
	drupal_add_js(drupal_get_path('module', 'alc').'/js/mycustom.js');
	//echo $serialFormV.'&&&&&'.$renewal_id.'****'.$particular_id.'****'.$license_id;
	//exit;
	
	global $base_root, $base_path, $user; 
	
	$contractorAddress	= $work_site_address = $licenseInfo = $validTill = $cooperative = $weekly_holidayValue = $total_holiday = $holidayName = $pre_work_order = $lastCertificateName = $unskilled_rate_wages_ck = $semiskilled_rate_wages_ck = $skilled_rate_wages_ck = $highlyskilled_rate_wages_ck = $extended_wo_check = $backlog_renewal_extra_ck = $form_vii_check = $certificates_url = $renewalCertificateUrl = ''; 
	
	if($serialFormV!='' && $serialFormV!=0){
		
		/** RENEWAL DETAILS ***/ // echo $renewal_id; die;
		
		$renewalDetails = renewal_data($renewal_id); 
		
		if(!empty($renewalDetails)){
			
			$peContractorInfo 		= contractor_regmaster_details($serialFormV); //print_r($peContractorInfo);
		
			if(!empty($peContractorInfo)){
				
				/**PE  Establishment Details ***/
				
				$establishmentInfo 	=  $peContractorInfo['e_name'].'<br/>'.$peContractorInfo['loc_e_name'].'<br/>'.get_full_address('l_clra_registration_master', 'sub-table', $peContractorInfo['id'], array('loc_e_dist','loc_e_subdivision','loc_e_areatype','name_areatype','loc_e_vill_ward','l_e_ps','loc_e_pin_number'));
				
				$get_peNatureOfWrk 	= pe_natureOfWrk($peContractorInfo['id'],$peContractorInfo['user_id'],1);
				
				if($peContractorInfo['state_opts'] == 1){
					
					$peInfo			= $peContractorInfo['full_name_principal_emp'].'<br/>'.$peContractorInfo['address_principal_emp'].'<br/>'.get_full_address('l_clra_registration_master', 'sub-table',$peContractorInfo['id'], array('loc_emp_dist','loc_emp_subdivision','loc_emp_areatype','emp_name_areatype','loc_emp_vill_ward','l_emp_ps','loc_emp_pin_number','loc_emp_state'));
					
				}else{
					
					$peInfo			= $peContractorInfo['full_name_principal_emp'].'<br/>'.$peContractorInfo['address_principal_emp'];
				}
				
				$con_nature_work	= getConNatureOfWrk($peContractorInfo['serial_no_from_v'],$peContractorInfo['id'],1 );
			
				/** Registration & Contractor & License Details **/
				
				$licenseConData 	= db_select('l_contractor_license_application', 'license');
				$licenseConData 	->leftJoin('l_particular_of_contract_labour', 'particular','particular.serial_no_from_v = license.serial_no_from_v and license.contractor_particular_id = particular.id and particular.contractor_user_id = '.$renewalDetails['created_by']);
				$licenseConData 	->leftJoin('l_contractor_info_master', 'con_master','con_master.id = license.serial_no_from_v');
				$licenseConData 	->leftJoin('l_clra_registration_master', 'reg_master','reg_master.id = con_master.application_id and reg_master.id ='.$peContractorInfo['id']);
				$licenseConData 	->fields('license',array('contractor_license_number','id','certificates_id','license_date','backlog_license_no','backlog_license_date','next_renweal_date'));
				$licenseConData 	->fields('particular',array('address_of_contractor','id','worksite_address_line','work_order_file_id','frm_v_file_id','residential_file_id'));
				$licenseConData 	->fields('reg_master',array('certificates_fid','id','status'));
				$licenseConData 	->fields('con_master',array('contractor_type','name_of_contractor','id'));
				$licenseConData		->condition('particular.id',$particular_id);
				$licenseConData		->condition('license.serial_no_from_v',$serialFormV);
				$licenseConData		->condition('license.contractor_user_id',$renewalDetails['created_by']);
				$licenseConData		->condition('particular.contractor_user_id',$renewalDetails['created_by']);
				$licenseConData		->condition('particular.serial_no_from_v',$serialFormV);
				//$licenseConData		->condition('reg_master.status','I');
				$licenseConData		= $licenseConData->execute();
				
				if($licenseConData->rowCount() > 0 ){
					
					$RegConLicenseInfo 		= $licenseConData->fetchAssoc(); //print_r($RegConLicenseInfo);exit;
					
					$conNameAddress 		= $RegConLicenseInfo['name_of_contractor'].'<br/>'.$peContractorInfo['address_of_contractor'].'<br/>'.get_full_address('l_contractor_info_master', 'sub-table', $serialFormV, array('con_loc_e_dist','con_loc_e_subdivision','con_loc_e_areatype','con_name_areatype','con_loc_e_vill_ward','con_l_e_ps','contractor_pin'));  // Provided by PE
					
					$contractor_type 		= ($RegConLicenseInfo['contractor_type'] == 1 ) ? 'Offline' : 'New Contractor';
					if($RegConLicenseInfo['contractor_type'] == 1) $formVUploading = 'FORM-V : &nbsp;&nbsp;Not Applicable <br/>REFERENCE NUMBER : 00'.$serialFormV; else $formVUploading = 'FORM-V Serial :00'.$serialFormV;
					$active					= '<span class="badge bg-green"><i class="fa fa-check" aria-hidden="true"></i> Active</span>';
					$deactive				= '<span class="badge bg-red"><i class="fa fa-close"></i> Deactive</span>';
					$contractor_status 		= ($peContractorInfo['status'] == 1 ) ? $active : $deactive;
					
					$regFile				=	fetch_uplaoded_file_details($RegConLicenseInfo['certificates_fid']);
					if(!empty($regFile)){ 
						$url			 	= 	explode('//',$regFile['uri']);
						$certificates_url	= 	$url[1];
							
					}
					
					$amendedContractorAddress	= ammendment_address($license_id,'ammendment_contrcator_address'); // If Amended Contractor Address provided by contractor
				
					if(!empty($amendedContractorAddress)){
						$contractorAddress	= $amendedContractorAddress;
					}else{
						$contractorAddress	=  $RegConLicenseInfo['address_of_contractor'].'<br/>'.get_full_address('l_particular_of_contract_labour', 'sub-table', $particular_id, array('contractor_dist','contractor_subdivision','contractor_areatype','contractor_name_areatype','contractor_vill_ward','contractor_ps','contractor_pin')); // Provided by Contractor 
					}
					
					if($renewalDetails['is_flag'] == 'L'){
				 
						 $work_site_address		= $RegConLicenseInfo['worksite_address_line'].'<br/>'.get_full_address('l_particular_of_contract_labour', 'sub-table', $particular_id, array('worksite_dist','worksite_subdivision','work_site_areatype','name_work_site_areatype','work_site_vill_ward','worksite_ps','worksite_pin'));
						 $lastCertificateName	= 'License Certificate';
						 $pre_work_order		= $RegConLicenseInfo['work_order_file_id'];
						 $frmVFile_id			= $RegConLicenseInfo['frm_v_file_id'];	
						 $residentialFile_id	= $RegConLicenseInfo['residential_file_id'];
						 $pre_certificate_id	= $RegConLicenseInfo['certificates_id']; 
						 
						 
					 }else if($renewalDetails['is_flag'] == 'A'){
						 
						 $work_site_address		= ammendment_address($renewalDetails['lincense_id'],'ammendment_worksite_address');
						 
						 $amendment_table		= check_ammendment_table($renewalDetails['parent_id']);	
						 $amendmentData 		= $amendment_table->fetchAssoc();
						 
						 $lastCertificateName	= 'Amendment Certificate';
						 $pre_certificate_id	= $amendmentData['ammendment_certificate_id'];
						 $pre_work_order		= !empty($amendmentData['work_order_file_id'])? $amendmentData['work_order_file_id']: $RegConLicenseInfo['work_order_file_id'];
						 $frmVFile_id			= $RegConLicenseInfo['frm_v_file_id'];	
						 $residentialFile_id	= $RegConLicenseInfo['residential_file_id'];
						 
							 
					 }else if($renewalDetails['is_flag'] == 'R'){
						
						$work_site_address		= $RegConLicenseInfo['worksite_address_line'].'<br/>'.get_full_address('l_particular_of_contract_labour', 'sub-table', $particular_id, array('worksite_dist','worksite_subdivision','work_site_areatype','name_work_site_areatype','work_site_vill_ward','worksite_ps','worksite_pin'));
						
						 $lastRenewal 			= renewal_data_last($renewalDetails['parent_id']); 
						 $lastCertificateName	= 'Previous Renewal Certificate';
						 if($lastRenewal['is_backlog_renewal'] == 1){
							$pre_work_order		= $RegConLicenseInfo['work_order_file_id']; 
						 }else{
							$pre_work_order		= $lastRenewal['extended_work_order'];
						 }
						 $pre_certificate_id	= $lastRenewal['renewal_certificate_id']; 
						 $frmVFile_id			= $RegConLicenseInfo['frm_v_file_id'];	
						 $residentialFile_id	= $RegConLicenseInfo['residential_file_id'];
						 
						
						 
						
					 }
					 
					$record_details  = db_query( "select  * from fun_basic_information1('".$renewalDetails['created_by']."','".$renewalDetails['parent_id']."','".$renewalDetails['is_flag']."')")->fetchAssoc();
					
					if(!empty($record_details)){
						
						if($record_details['backlog_license_no']!=''){
								$licenseInfo	= 'Ref. License No.:'.$record_details['contractor_license_number'].'<br>Date:'.date('d/m/Y',strtotime($record_details['license_date'])).'<hr/>Offline License No: '.$record_details['backlog_license_no'].'<br>Date:'.date('d/m/Y',strtotime($record_details['backlog_license_date']));
						}else{
								$licenseInfo	=   $record_details['contractor_license_number'].' - '.date('dS M, Y',strtotime($record_details['license_date']))  ;
						}
						
						if($record_details['next_renweal_date']!=''){
							$validTill 			= 'VALID TILL : '.date('d/m/Y', strtotime($record_details['next_renweal_date']));	
						}else{
							$validTill 			= '';
						}	
						
					}
					
					$cooperative	=	!empty($renewalDetails['is_coparative']) ?  'Yes' : 'No';
					$extraFees 		=   !empty($renewalDetails['is_extra_fees'])?' <i style="color:#f00;">[ including 25% extra ]</i> ': '';
					$workHours		=	!empty($renewalDetails['hours_work'])? $renewalDetails['hours_work'].' hr(s)':'NIL';
					$spread_over	= 	!empty($renewalDetails['spred_over'])? $renewalDetails['spred_over'].' hr(s)':'NIL';
					$weekly_holiday	= 	!empty($renewalDetails['weekly_holiday'])? $renewalDetails['weekly_holiday']:'0';
					$holiday_wages	=	!empty($renewalDetails['holiday_wages'])? $renewalDetails['holiday_wages']: 'NIL';
					$annual_leave	= 	$renewalDetails['annual_leave_no']!=''? $renewalDetails['annual_leave_no']:'NIL';
					$casual_leave	= 	!empty($renewalDetails['casual_leave_no'])? $renewalDetails['casual_leave_no']: 'NIL';
					$earned_leave	= 	!empty($renewalDetails['earned_leave_no'])? $renewalDetails['earned_leave_no']: 'NIL';
					$sick_leave		= 	!empty($renewalDetails['sick_leave_no'])? $renewalDetails['sick_leave_no']: 'NIL';
					$maternityLeave	=	!empty($renewalDetails['maternity_leave_no'])?$renewalDetails['maternity_leave_no']:'NIL';
					$otherLeave		=	!empty($renewalDetails['other_leave_no'])? $renewalDetails['other_leave_no']:'NIL';	
					$specialBenifits=	!empty($renewalDetails['special_benifites'])?$renewalDetails['special_benifites']:'NIL';
					$stateInsurance	= 	!empty($renewalDetails['state_insurance'])?$renewalDetails['state_insurance']:'NIL';
					$miscProvisions	=	!empty($renewalDetails['miscellaneous_provisions'])?$renewalDetails['miscellaneous_provisions']:'NIL';	
					$conRevoking	= 	!empty($renewalDetails['details_contractor_revoking'])? date('dS M, Y', strtotime($renewalDetails['details_contractor_revoking'])):'NIL';	
					$unskilledRate	=	!empty($renewalDetails['unskilled_rate_wages'])?$renewalDetails['unskilled_rate_wages']:'NIL';
					$semiSkilledRate=	!empty($renewalDetails['semiskilled_rate_wages'])?$renewalDetails['semiskilled_rate_wages']:'NIL';
					$skilledRate	=	!empty($renewalDetails['skilled_rate_wages'])?$renewalDetails['skilled_rate_wages']:'NIL';				
					$highSkilledRate=	!empty($renewalDetails['highlyskilled_rate_wages'])?$renewalDetails['highlyskilled_rate_wages']:'NIL';					
					
					if($weekly_holiday!='' && $weekly_holiday =='0') {
						
						$rest_holiday 		= 	substr($renewalDetails['no_holiday'], 0, -1);
						$holiday_list		= 	explode(',', $rest_holiday); 
						$total_holiday		=	0;
						
						foreach($holiday_list as $row){
							
							$total_holiday	=	$total_holiday + 1;
							
							if($row=='1'){
								$holidayName.='Sunday'.',';
							}
							if($row=='2'){
								$holidayName.='Monday'.',';
							}
							if($row=='3'){
								$holidayName.='Tuesday'.',';
							}
							if($row=='4'){
								$holidayName.='Wednesday'.',';
							}
							if($row=='5'){
								$holidayName.='Thursday'.',';
							}
							if($row=='6'){
								$holidayName.='Friday'.',';
							}
							if($row=='7'){
								$holidayName.='Saturday'.',';
							}
							
						}
					
						$name_holidays 			= substr($holidayName, 0, -1);
						$weekly_holidayValue	= $total_holiday .'day(s)'.'&nbsp;[ '.$name_holidays.' ]';
		
					}else{
						$weekly_holidayValue		=	'Not Applicable';
					}
					
					$preWorkOrderFile = fetch_uplaoded_file_details($pre_work_order);
					if(!empty($preWorkOrderFile)){ 
						$url			 		= 	explode('//',$preWorkOrderFile['uri']);
						$pre_work_order_url		= 	$url[1];
						$pre_workorder_file		=	$preWorkOrderFile['filename'];
					}
					
					$workOrderFile = fetch_uplaoded_file_details($renewalDetails['extended_work_order']);
					if(!empty($workOrderFile)){ 
						$url			 	= 	explode('//',$workOrderFile['uri']);
						$work_order_url		= 	$url[1];
						$workorder_file		=	$workOrderFile['filename'];
					}
					
					$frmVFile = fetch_uplaoded_file_details($frmVFile_id);
					if(!empty($frmVFile)){ 
						$url 				= explode('//',$frmVFile['uri']);
						$frmv_url			= $url[1];
						$frmv_file			= $frmVFile['filename'];
					}
					
					$residentialFile = fetch_uplaoded_file_details($residentialFile_id);
					if(!empty($residentialFile)){ 
						$url					= explode('//',$residentialFile['uri']);
						$residential_url		= $url[1];
						$residential_file		= $residentialFile['filename'];
					}
	
					$form7File 					= fetch_uplaoded_file_details($renewalDetails['upload_signed_form']);
					if(!empty($form7File)){ 
						$url		  			= explode('//', $form7File['uri']);
						$form_vii_url 			= $url[1];
						
					}
					$preCertificateFile 		= fetch_uplaoded_file_details($pre_certificate_id);
					if(!empty($preCertificateFile)){ 
						$url					= explode('//',$preCertificateFile['uri']);
						$preCertificatesUrl		= $url[1];
					}
					
					if($renewalDetails['is_autorenewal'] == 'Y'){ 
					
						//$certificateDownloadLink =  l(t('<i class="fa fa-download" aria-hidden="true"></i>&nbsp; Signed Renewal Certificate'),'', array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn'))));
						$certificateDownloadLink= l(t('<i class="fa fa-download" aria-hidden="true"></i>&nbsp; Auto-Generated Renewal Certificate'),'clra-amendment-renewal-certificates/'.encryption_decryption_fun('encrypt', $serialFormV).'/'.encryption_decryption_fun('encrypt',$renewal_id).'/'.encryption_decryption_fun('encrypt', $renewalDetails['created_by']).'/'.encryption_decryption_fun('encrypt','R'), array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn'))));
						
						$link = 'clra-amendment-renewal-certificates/'.encryption_decryption_fun('encrypt', $serialFormV).'/'.encryption_decryption_fun('encrypt',$renewal_id).'/'.encryption_decryption_fun('encrypt', $renewalDetails['created_by']).'/'.encryption_decryption_fun('encrypt','R');
						
						/*'clra-license-amendment-renewal-certificates/'.encryption_decryption_fun('encrypt', $serialFormV ).'/'.encryption_decryption_fun('encrypt',$renewalDetails['id']).'/'.encryption_decryption_fun('encrypt', $renewalDetails['created_by']).'/'.encryption_decryption_fun('encrypt', $renewalDetails['lincense_id']).'/'.encryption_decryption_fun('encrypt','R').'/Renewal Certificates'.time()*/
						
					}else{
						$renewalCertificateFile 	= fetch_uplaoded_file_details($renewalDetails['renewal_certificate_id']);
						if(!empty($renewalCertificateFile)){ 
							$url					= explode('//',$renewalCertificateFile['uri']);
							$renewalCertificateUrl	= $url[1];
							$certificateDownloadLink= l(t('<i class="fa fa-download" aria-hidden="true"></i>&nbsp; Signed Renewal Certificate'),$GLOBALS['base_url'].'/sites/default/files/'.$renewalCertificateUrl, array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn'))));
							$link = $GLOBALS['base_url'].'/sites/default/files/'.$renewalCertificateUrl;
							
						}
					} 
				}
			}
			
			$transaction_details  			= 	db_select('l_principle_epayments_receive_data', 'lpd');
			$transaction_details			->	leftJoin('l_principle_epayments_data', 'lped', 'lpd.transaction_id = lped.identification_no');
			$transaction_details			->  fields('lped', array('identification_no', 'application_id'));
			$transaction_details			->  fields('lpd', array());
			$transaction_details			->	condition('lped.act_id', '13');
			$transaction_details			->	condition('lped.application_id',$renewal_id);
			$transaction_details			->	condition('lped.deposited_by',$renewalDetails['created_by']); 
			$transaction_det 				= 	$transaction_details->execute()->fetchAssoc();
			
			
			$bank_code 						= 	'Not available';
			$bankTransactionID 				= 	'Not available';
			$total_amount 					= 	'Not available';
			$challan_fid_date 				= 	'Not available';
			$payment_status 				=   'Not available';
			
			if(!empty($transaction_det)){
				$bankTransactionID				= $transaction_det['transaction_id'];
				$grn_number						= $transaction_det['challanrefid'];
				$challan_fid_date				= !empty($transaction_det['challanrefid_date']) ? $transaction_det['challanrefid_date'] : '';
				$total_amount					= number_format($transaction_det['challanamount'], 2);
				$bank_code						= $transaction_det['bank_cd'];
				
				if($transaction_det['banktransactionstatus'] == 'Success') {
					$payment_status = '<font color="#060">'.$transaction_det['banktransactionstatus'].'</font>';
				}else{
					$payment_status = '<font color="#CC0000">'.$transaction_det['banktransactionstatus'].'</font>';
				}
			}
			
			$payment_details = '<u> GRIPS PAYMENT [ Online / Counter ] </u> <br>';
			$payment_details .= 'IFSC Code : <font color="#FF6600">'.$bank_code.'</font><br>';
			$payment_details .= 'Bank Transaction Id : <font color="#FF6600">'.$bankTransactionID.'</font><br>';
			$payment_details .= 'Total Amount : <font color="#FF6600">'.$total_amount.'</font><br>';
			$payment_details .= 'Transaction Date : <font color="#FF6600">'.$challan_fid_date.'</font><br>';
			$payment_details .= 'Transaction Status : '.$payment_status;
			
			$get_remark = db_select('l_remark_license', 'remark_license');
			$get_remark->fields('remark_license', array('remark_field_title'));
			$get_remark->condition('remark_license.remark_to', $serialFormV);
			$get_remark->condition('remark_license.particular_id', $renewal_id);
			$get_remark->condition('remark_license.is_active', 1);
			$get_remark->where("((remark_license.remark_by_role = 4) OR (remark_license.remark_by_role = 7))");
			$get_remark->orderBy('remark_license.id','DESC');
			$get_remark->range(0, 1);
			$get_remark_result = $get_remark->execute()->fetchAssoc();
			
			if(!empty($get_remark_result['remark_field_title'])){
				$remark_field_title = explode(',', $get_remark_result['remark_field_title']);
				
				foreach ($remark_field_title as $fieldname){
				
					if($fieldname == 'extended_wo'){ $extended_wo_check = "checked='checked'";}
					if($fieldname == 'form_vii'){ $form_vii_check = "checked='checked'";}
					if($fieldname == 'backlog_renewal_extra_amount'){ $backlog_renewal_extra_ck = "checked='checked'";}
					if($fieldname == 'unskilled'){ $unskilled_rate_wages_ck = "checked='checked'";}
					if($fieldname == 'semiskilled'){ $semiskilled_rate_wages_ck = "checked='checked'";}
					if($fieldname == 'skilled'){ $skilled_rate_wages_ck = "checked='checked'";}
					if($fieldname == 'highly'){ $highlyskilled_rate_wages_ck = "checked='checked'";}
				
				}
			}
		}
	
		$output = '';				
		$output	.= '<div class="row equal-panels">
		
						<div class="col-md-6 col-sm-12">
							<div class="box box-primary collapsed-box box-solid">
								<div class="box-header with-border">
								  <h3 class="box-title">1. Establishment Details [ Verified ]</h3>
								  <div class="box-tools pull-right">
									<button type="button" class="btn btn-box-tool" data-widget="collapse"><i class="fa fa-minus"></i></button>
								  </div>
								</div>
								<div class="box-body">
									<div class="feedback-scroll">
									  <table class="table table-bordered">
										<tbody>
											<tr>
											  <th width="8%">Sl</th>
											  <th>Parameters</th>
											  <th>Inputs</th>
											</tr>
											<tr>
												<td>1.(a)</td>
												<td width="40%">Name and address of establishment</td>
												<td>'.$establishmentInfo.'</td>
											</tr>
											<tr>
												<td>1.(b)</td>
												<td>Type of business, trade,industry, manufacture or occupation, which is carried on in the establishment</td>
												<td>'.$get_peNatureOfWrk.'</td>
											</tr>
											<tr>
												<td>1.(c)</td>
												<td>Number and date of certificate</td>
												<td>'.$peContractorInfo['registration_number'].' issued on '.date('dS M, Y',strtotime($peContractorInfo ['registration_date'])).'<br/>'.l('Registration Certificate',$GLOBALS['base_url'].'/sites/default/files/'.$certificates_url,array('attributes' => array('target' => '_blank'), 'html' => TRUE)).'&nbsp;
												<span class="viewfile_popup" id="regCertificate">
														<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
													</span>
													<div id="view_regCertificate"  title="Registration Certificate" style="display:none;">
														<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$certificates_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
													</div>
													
													&nbsp;&nbsp;&nbsp;'.
													l('<span class="badge bg-blue"><i class="fa fa-eye"></i> View More of Registration<div>', 'alc-visible-applications/'.encryption_decryption_fun('encrypt', $peContractorInfo['id']).'/'.encryption_decryption_fun('encrypt', $peContractorInfo['user_id']), array('html' => TRUE, 'target' => '_blank')).'
												</td>
											</tr>
											<tr>
												<td>1.(d)</td>
												<td>Name and address of the Principal Employer</td>
												<td>'.$peInfo.'</td>
											</tr>
										</tbody>
									  </table>
									</div>
								</div>
							</div>
						</div>
						
						<div class="col-md-6 col-sm-12">
							<div class="box box-primary collapsed-box box-solid">
								<div class="box-header with-border">
								  <h3 class="box-title">2. Contractor Details Provided by Principal Employer [ Verified ]</h3>
								  <div class="box-tools pull-right">
									<button type="button" class="btn btn-box-tool" data-widget="collapse"><i class="fa fa-minus"></i></button>
								  </div>
								</div>
								<div class="box-body">
								  <div class="feedback-scroll">
									<table class="table table-bordered">
									<tbody>
										<tr>
										  <th width="8%">Sl</th>
										  <th>Parameters</th>
										  <th>Inputs</th>
										</tr>
										<tr>
											<td>2.(a)</td>
											<td width="50%">Name and address of Contractor <br/> ( Address provided by principal employer)</td>
											<td>'.$conNameAddress.'</td>
										</tr>
										<tr>
											<td>2.(b)</td>
											<td>Contractor Status</td>
											<td>'.$contractor_type.'&nbsp;&nbsp;'.$contractor_status.'</td>
										</tr>
										<tr>
											<td>2.(c)</td>
											<td>Nature of work in which contract labour is employed or is to be employed in the establishment</td>
											<td>'.$con_nature_work.'</td>
										</tr>
										<tr>
											<td>2.(d)</td>
											<td>Duration of the proposed contract work( give particulars of proposed date of commencing and ending )</td>
											<td>'.date('dS M, Y',strtotime($peContractorInfo['est_date_of_work_of_each_labour_from_date'])).' TO '.date('dS M, Y',strtotime($peContractorInfo['est_date_of_work_of_each_labour_to_date'])).'</td>
										</tr>
										<tr>
											<td>2.(e)</td>
											<td>Maximum number of contract labour proposed to be employed in the establishment on any date</td>
											<td>'.$peContractorInfo['contractor_max_no_of_labours_on_any_day'].'&nbsp;&nbsp; <font color="#006600"> <b>[ FEES IS CALCULATED BASED ON THIS VALUE ] </b></font></td>
										</tr>
									</tbody>
								  </table>
								  </div>
								</div>
							</div>
						</div>	
						
					</div>
					
					<div class="box box-primary box-solid">
						<div class="box-header with-border">
						  <h3 class="box-title">3. License Details Provided by Contractor [ Verified ] </h3>
						  <div class="box-tools pull-right">
							<button type="button" class="btn btn-box-tool" data-widget="collapse"><i class="fa fa-minus"></i></button>
						  </div>
						</div>
						<div class="box-body">
							 <div class="feedback-scroll">
								 <div class="col-md-6 col-sm-12">
								 	<table class="table table-bordered">
										<tr>
										  <th width="8%">Sl</th>
										  <th>Parameters</th>
										  <th>Inputs</th>
										</tr>
										<tr>
											<td>1. </td>
											<td width="45%">Name and Address of the Contractor</td>
											<td>'.$peContractorInfo['name_of_contractor'].'<br/>'.$contractorAddress.'</td>
										</tr>
										<tr>
											<td>2. </td>
											<td>Co-operative Society</td>
											<td>'.$cooperative.'</td>
										</tr>
										<tr>
											<td>3. </td>
											<td>License Number and Date of Issue</td>
											<td>'.$licenseInfo.'</td>
										</tr>
										<tr>
											<td>4.</td>
											<td>Date of expiry</td>
											<td>'.$validTill.'</td>
										</tr>
										<tr>
											<td>5.</td>
											<td>Maximum no. of contract labour employed by the contractor on any day</td>
											<td>'.$renewalDetails['max_of_contract_labour'].' ('. number_to_word($renewalDetails['max_of_contract_labour']).') head(s)</td>
										</tr>
										
										<tr>
											<td>6. </td>
											<td>Special benifits provided, if any</td>
											<td>'.$specialBenifits.' </td>
										</tr>
										<tr>
											<td>7. </td>
											<td>Contribution made under the Employees State Insurance Act,1984</td>
											<td>'.$stateInsurance.' </td>
										</tr>
										<tr>
											<td>8. </td>
											<td>Whether the license of the contractor was suspended or revoked</td>
											<td>'.$conRevoking.' </td>
										</tr>
										<tr>
											<td>9. </td>
											<td>Conribution made under the Employees Provident Fund and Miscellaneous Provisions Act,1952</td>
											<td>'.$miscProvisions.' </td>
										</tr>
									</table>
								 </div>
								 <div class="col-md-6 col-sm-12">
								 	<table class="table table-bordered">
								 		<tr>
										  <th width="8%">Sl</th>
										  <th>Parameters</th>
										  <th>Inputs</th>
										</tr>
										<tr>
											<td width="8%">10. </td>
											<td width="50%"><strong>Worksite Address <strong><font color="red">**</font></td>
											<td>'.$work_site_address.'</td>
										</tr>
									
										<tr>
											<td>11.</td>
											<td>Daily hours of work and spread over</td>
											<td>'.$workHours.' and  '.$spread_over.'</td>
										</tr>
										<tr>
											<td>12.</td>
											<td>Whether weekly holiday observed was on which day</td>
											<td>'.$weekly_holidayValue.'</td>
										</tr>
										<tr>
											<td>13.</td>
											<td>Whether weekly holiday so observed was paid holiday</td>
											<td>'.$holiday_wages.'</td>
										</tr>
										<tr>
											<td>14. </td>
											<td>Number of Annual leave</td>
											<td>'.$annual_leave.'</td>
										</tr>
										<tr>
											<td>15. </td>
											<td>Number of Casual leave</td>
											<td>'.$casual_leave.' </td>
										</tr>
										<tr>
											<td>16. </td>
											<td>Number of Earned leave</td>
											<td>'.$earned_leave.' </td>
										</tr>
										<tr>
											<td>17. </td>
											<td>Number of Sick leave</td>
											<td>'.$sick_leave.' </td
										</tr>
										<tr>
											<td>18. </td>
											<td>Number of Maternity leave</td>
											<td>'.$maternityLeave.' </td>
										</tr>
										<tr>
											<td>19. </td>
											<td>Number of Other leave</td>
											<td>'.$otherLeave.' </td>
										</tr>
										<tr>
											<td>20. </td>
											<td>FORM-V / REF. NO.</td>';
											if($RegConLicenseInfo['contractor_type'] == 1){
								$output	.= '<td>'.$formVUploading.'</td>';
											}else{
												if($frmVFile_id!=''){
								$output	.= '<td>
												<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$frmv_url.'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;
												<span class="viewfile_popup" id="pre_formV">
													<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
												</span>
												<div id="view_formV"  title="FORM -V" style="display:none;">
													<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$frmv_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
												</div>
											</td>';
												}else{
													$output .=  '<td>No Document Uploaded</td>';
												}
											}
							$output	.= '</tr>
										<tr>
											<td>21. </td>
											<td>Previous Work Order</td>';
											if($pre_work_order!=''){
								$output	.= '<td>
												<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$pre_work_order_url.'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;
												<span class="viewfile_popup" id="pre_work_order">
													<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
												</span>
												<div id="view_pre_work_order"  title="Previous Work Order" style="display:none;">
													<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$pre_work_order_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
												</div>
											</td>';
											}else{
												$output .=  '<td>No Document Uploaded</td>';
											}
						   $output .=  '</tr>
						   				<tr>
											<td>22. </td>
											<td>Residential Certificate / Trade License</td>';
											if($residentialFile_id!=''){
											
								$output	.= '<td>
												<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$residential_url.'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;
												<span class="viewfile_popup" id="residential_tradeLic">
													<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
												</span>
												<div id="view_residential_tradeLic"  title="Residential Certificate / Trade License" style="display:none;">
													<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$residential_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
												</div>
											</td>';
											}else{
												$output .=  '<td>No Document Uploaded</td>';
											}
						   $output .=  '</tr>
						   				<tr>
											<td>23. </td>
											<td>Previous License / Renewal / Amendment Certificate</td>';
											if($pre_certificate_id!=''){
											
								$output	.= '<td>
												<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$preCertificatesUrl.'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;
												<span class="viewfile_popup" id="previousCertificate">
													<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
												</span>
												<div id="view_previousCertificate"  title="'.$lastCertificateName.'" style="display:none;">
													<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$preCertificatesUrl.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
												</div>
											</td>';
											}else{
												$output .=  '<td>No Document Uploaded</td>';
											}
						   $output .=  '</tr>
									</table>
								 </div>
								
							 
							 	 
							 </div>
						</div>
					</div>
					
					<div class="box box-primary box-solid">
						<div class="box-header with-border">
						  <h3 class="box-title">4. Inputs are provided by Contractor for Renewal  [For Verification]</h3>
						</div>
						<div class="box-body">
							 <div class="feedback-scroll">
								 <table class="table table-bordered">
									 <tr>
									  <th style="width:10%">Sl</th>
									  <th style="width:40%">Parameters</th>
									  <th style="width:40%">Inputs</th>
									  <th style="width:10%">Verified?</th>
									</tr>
									<tr>
										<td colspan="4" style="font-size:15px; font-weight:600;">1.&nbsp;&nbsp;&nbsp;Rate of Wages, DA and other cash benefits paid / to be paid to each category (i.e (a) Unskilled (b) Semi-Skilled (c) Skilled (d) Highly-Skilled etc.) of contract labour</td>
									</tr>
									<tr>
										<td>1.(a)</td>
										<td>Rate of wages,DA and Other cash benefits paid/ to be paid to Unskilled of contract labour:</td>
										<td>'.$unskilledRate.'</td>
										<td><input type="checkbox" id="unskilled" class="alc_renewal_verified"  '.$unskilled_rate_wages_ck.'  </td>
									</tr>
									<tr>
										<td>1.(b)</td>
										<td>Rate of wages,DA and Other cash benefits paid/ to be paid to Semi-skilled of contract labour:</td>
										<td>'.$semiSkilledRate.'</td>
										<td><input type="checkbox" id="semiskilled" class="alc_renewal_verified" '.$semiskilled_rate_wages_ck.'  </td>
									</tr>
									<tr>
										<td>1.(c)</td>
										<td>Rate of Wages,DA and Other cash benefits paid/ to be paid to Skilled of contract labour:</td>
										<td>'.$skilledRate.'</td>
										<td><input type="checkbox" id="skilled" class="alc_renewal_verified"  '.$skilled_rate_wages_ck.'</td>
									</tr>
									<tr>
										<td>1.(d)</td>
										<td>Rate of Wages,DA and Other cash benefits paid/ to be paid to Highly-skilled of contract labour:</td>
										<td>'.$highSkilledRate.'</td>
										<td><input type="checkbox" id="highly" class="alc_renewal_verified" '.$highlyskilled_rate_wages_ck.' </td>
									<tr>
									<tr>
										<td colspan="4" style="font-size:15px; font-weight:600">2. Documents Uploaded</td>
								    </tr>
									<tr>
										<td>i.) </td>
										<td>Extended Work Order</td>';
										
										if($renewalDetails['wo_file_encryption'] != ''){
											$renewal_file = base64_decode($renewalDetails['wo_file_encryption']);
											$renewal_file_name='work_order_renewal_license_clra'.'_'.$renewal_id.'.pdf';
											file_put_contents('sites/default/files/upload/common/'.$renewal_file_name, $renewal_file);
											$output	.= '<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/common/'.$renewal_file_name.'"><i class="fa fa-file fa-lg"></i></a></td>';
											
										}else if($renewalDetails['extended_work_order']!=''){
										
							$output	.= '<td>
											<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$work_order_url.'"><i class="fa fa-file fa-lg"></i>'.$renewalDetails['wo_file_encryption'].'</a>&nbsp;&nbsp;&nbsp;
											<span class="viewfile_popup" id="extendedWO">
												<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
											</span>
											<div id="view_extendedWO"  title="Extended Work Order" style="display:none;">
												<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$work_order_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
											</div>
										</td>';
										}else{
											$output .=  '<td>No Document Uploaded</td>';
										}
					   		$output .=  '<td><input type="checkbox" id="extended_wo" class="alc_renewal_verified" '.$extended_wo_check.' </td>
					   				</tr>
									<tr>
										<td>ii.)</td>
										<td>FORM-VII</td>';
										 if(!empty($renewalDetails['upload_signed_form'])){
											 
											 $output .= '<td>
															<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$form_vii_url.'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;
															<span class="viewfile_popup" id="signed_pdf_file">
																<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
															</span>
															<div id="view_signed_pdf_file"  title="Signed FORM-VII" style="display:none;">
																<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$form_vii_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
															</div>';
															if($renewalDetails['renewal_appliaction_status'] =='P' || $renewalDetails['renewal_appliaction_status']=='U' || $renewalDetails['renewal_appliaction_status']=='I' ){
												 $output .= '&nbsp;&nbsp;&nbsp;
															'.l(t('Generated FORM - VII'),'pdf-form-vii/'.encryption_decryption_fun('encrypt', $renewal_id).'/'.encryption_decryption_fun('encrypt', 0).'/'.encryption_decryption_fun('encrypt', 'FORM-VII'), array('html' => TRUE,'attributes'=> array('target'=>'_blank')));
															}
											$output .= '</td>';
											  
											
										}else{
											
											$output .=  '<td>Signed FORM-VII will be uploaded by the Applicant after payment of fees</td>';
										}
										if($renewalDetails['renewal_appliaction_status'] == 'U'){
											$disabled = '';	
										}else{
											$disabled = 'disabled="disabled"';
										}
												
										$output .= '<td id="formVId"><input type="checkbox" id="form_vii" '.$disabled.' class="alc_renewal_verified"   '.$form_vii_check.'> </td>
									</tr>';
									if($renewalDetails['renewal_appliaction_status'] =='I' ){
						$output .= '<tr>
										<td>iii.)</td>
										<td>Renewal Certificate </td>';
											 $output .= '<td colspan="2">'.$certificateDownloadLink.'</td>';
						$output .= '</tr>';
									 }
						$output .= '<tr>
										<td colspan="4" style="font-size:15px; font-weight:600">3. Payment Details</td>
								   </tr>
								   <tr>
								    	<td>3.(a)</td>
										<td colspan="2"><font color="#FF0000">Note: Provided that if the application for renewal is not received with in the time specified in R29(2)[<i>Amendment on 27th Nov. 2015</i>], a fee of 25%, in excess of the fee ordinarily payable for the licence shall be payable for such renewal : Provided further that in case where the licensing officer is satisfied that the delay in submission of the application is due to unavoidable circumstances beyond the control of the contractor, he may reduce or remit as he thinks fit the payment of such excess fee [ see R29 (3) ] </font></td>
										<td id="extraAmt"><input type="checkbox" id="backlog_renewal_extra_amount" onclick="return Confirmcheck();" class="alc_renewal_verified" '.$backlog_renewal_extra_ck.' <br/><div><i style="color:#f00;">Must select for 25% additional fees</i> </div></td>
									</tr>
									<tr>
										<td>3.(b)</td>
										<td>Renewal fees</td>
										<td colspan="2">&#8377; '.$renewalDetails['increnent_fees'].'&nbsp;&nbsp; '.$extraFees.'&nbsp;&nbsp;<button type="button" class="btn btn-info" data-toggle="modal" data-target="#myModalFeeschart"><i class="fa fa-info-circle"></i>&nbsp;Fees Chart</button></td>
									</tr>
									<tr>
										<td>3.(c)</td>
										<td>Payment Details</td>
										<td colspan="2">'.$payment_details.'</td>
									</tr>
								</table>
							 </div>
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
											 <td>Is 10 but does not exceed 20 </td>
											 <td>₹50.00</td>
										 </tr>
										 <tr>
											 <td>2</td>
											 <td>Exceeds 20 but does exceed 50 </td>
											 <td>₹125.00</td>
										 </tr>
										 <tr>
											 <td>2</td>
											 <td>Exceeds 50 but does not exceed 100</td>
											 <td>₹250.00</td>
										 </tr>
										 <tr>
											 <td>3</td>
											 <td>Exceeds 100 but does not exceed 200</td>
											 <td>₹500.00</td>
										 </tr>
										 <tr>
											 <td>4</td>
											 <td>Exceeds 200 but does not exceed 400</td>
											 <td>₹1000.00</td>
										 </tr>
										 <tr>
											 <td>9</td>
											 <td>Exceeds 400</td>
											 <td>₹1250.00</td>
										 </tr>
									  </table>
									</div>
								<div class="modal-footer">
									<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
								</div>
							</div>      
						 </div>
					  </div>';
	
	$output .= '<div class="row"><ul style="padding:0;"><li class="col-md-2 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-arrow-left"></i>&nbsp;Back to list</div>', 'alc-received-renewal-details/pending', array('attributes' => array('title' => 'Click here back to applications list'), 'html' => TRUE)).'</li>';
	
	$output .= '<li class="col-md-2 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-user"></i>&nbsp;View Applicant Profile</div>', 'view-applicant-profile/'.encryption_decryption_fun('encrypt', $serialFormV).'/'.encryption_decryption_fun('encrypt', $renewalDetails['created_by']).'/'.encryption_decryption_fun('encrypt', 12), array('attributes' => array('title' => 'Click here view Applicant Profile'), 'html' => TRUE)).'</li>';
	
	$output .= '<li class="col-md-4 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-info-circle"></i>&nbsp;Instructions to give remarks</div>', $base_root.$base_path.'sites/default/files/instructions-remarks.pdf', array('attributes' => array('title' => 'Instructions to give remarks'), 'html' => TRUE)).'</li>';
	
	$output .= '<li class="col-md-4 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-question-circle" aria-hidden="true"></i>&nbsp;How to digitally sign your documents using USB Token</div>', 'digitally-sign-process', array('attributes' => array('title' => 'How to digitally sign your documents using USB Token'), 'html' => TRUE)).'</li></ul></div>';
	}
	
	
	return $output;
}

function renewal_remark_details($serialFormV,$renewal_id,$created_by,$license_id,$isautoRenewal){
	global $base_root, $base_path, $user;
	$user_id 		= $user->uid;
	$counter		= 0;
	$status			= '';
	$loadingImg 	= $base_root.'/sites/all/themes/jackson/images/lodinggif.gif';
	
	$query_frmv 	= db_select('l_contractor_info_master', 'cim');
	$query_frmv 	->fields('cim',array('is_from_v','contractor_type'));
	$query_frmv		->condition('cim.id',$serialFormV);
	$formV 			= $query_frmv->execute()->fetchAssoc();
	
	if($formV['contractor_type']=='1'){
		$formVSerial	= 'REF NO : 00'.$serialFormV;
	}else{
		$formVSerial	= 'FORM-V : 00'.$serialFormV;
	}
	
	$header = array(
					array('data' => 'Sl', 'field' => 'slno','width' => '5%'),
					array('data' => 'FORM-V / REF NO.', 'field' => 'serial','width' => '10%'),
					array('data' => 'Date - Time', 'field' => 'date','width' => '15%'),
					array('data' => 'Remark', 'field' => 'remrk'),
					array('data' => 'Remark Status', 'field' => 'remark type','width' => '15%'),
					array('data' => 'Remark By', 'field' => 'licenseno','width' => '20%'),
					array('data' => 'Action','width' => '10%')
				);
	
	$remark_details			=	db_select('l_remark_license','remark');
	//$remark_details			->	InnerJoin('l_custom_user_detail', 'cus_detl','cus_detl.usr_id = remark.remark_by');
	$remark_details			->	InnerJoin('role', 'r','r.rid = remark.remark_by_role');
	$remark_details 		->	fields('remark',array());
	//$remark_details 		->	fields('cus_detl',array('employee_id'));
	$remark_details 		->	fields('r',array('name'));
	$remark_details			->	condition('remark.remark_to',$serialFormV,'=');
	$remark_details			->	condition('remark.particular_id',$renewal_id,'=');
	$remark_details			->	condition('remark.flag','R','=');
	$remark_details			->	condition('remark.is_active','1');
	$remark_or				=	db_or();
	$remark_or				->	condition('remark.remark_by_role',4);
	$remark_or				->	condition('remark.remark_by_role',7);
	$remark_details			->	condition($remark_or);
	$remark_details			->  orderBy('remark.id','DESC');
	$remark_details_result	=	$remark_details->execute()->fetchAll(); 
	
	
	foreach($remark_details_result as $row){
		
		$counter++;
		if($isautoRenewal == 'Y'){
			$remark_byName		= 'Auto-renewed';
		}else{
			$remark_byName		= $row->remark_by_name.'&nbsp;('.$row->name.')';
		}
		
		if($row->remark_type == "P" && $row->remark_by_role == 4){
			$status 	= '<span class="backed" title=" Rectify FORM-VII"></span>';
		}else{
			$status 	= get_user_license_status($row->remark_type);
			
		}
		
		$getPayInfo	 	= db_query("SELECT COUNT(*) AS total_row FROM l_principle_epayments_data WHERE act_id=13 AND application_id = ".$serialFormV."")->fetchObject();
		$getId		 	= $getPayInfo->total_row; //echo $getId.'nnnnnn';
		
		$getMaxData		= db_query("SELECT MAX(id) AS MAXID FROM l_remark_license WHERE is_active=1 and remark_to= ".$serialFormV." and flag= 'R' and particular_id=".$renewal_id."")->fetchObject();
		$maxId			= $getMaxData->maxid; 
		
		if($maxId>0){
			
			if(($row->remark_by == $user_id) &&($maxId == $row->id) && ($getId!=0 || $getId!='')&& $row->remark_type != 'I'){		
				$delete	= '<span id="delBtn" class="delete-btn active-del" data-id ='.encryption_decryption_fun('encrypt',$row->id).' data-uid='.encryption_decryption_fun('encrypt',$user_id).' data-act='.encryption_decryption_fun('encrypt',13).'>&nbsp;<img style="display:none;margin-left:30px;" src="'.$loadingImg.'" alt="loading" id="loading_'.encryption_decryption_fun('encrypt',$row->id).'"></span>';
			}else{
				$delete	= '<div id="delBtn" class="delete-btn disable-del">&nbsp;</div>';
			}	
		}

		
		
		$rows[] = array(
					array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
					array('data' => $formVSerial, 'align' => 'left', 'class' => array('odd')),
					array('data' => date('dS M, Y - g:i a', $row->remark_date), 'align' => 'left', 'class' => array('odd')),
					array('data' => $row->remark_text, 'align' => 'left', 'class' => array('odd')),
					array('data' => $status, 'align' => 'left', 'class' => array('odd')),
					array('data' =>$remark_byName, 'align' => 'left', 'class' => array('odd')),
					array('data' =>$delete, 'align' => 'left', 'class' => array('odd'))
				);
		
		}
//exit;
 
	$variables = array(
					'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')), 
					'header' 	 => $header,
					'rows'		 => $rows,
					'empty'		 => 'No data found',
				);
				
	$output = theme('datatable', $variables);
	
	return '<div class="feedback-scroll">'.$output.'</div>';		
}

function alc_view_contractor_license_renewal_submit($form,&$form_state){
	
	global $user;
	
	$is_extra_fees	= $increment_fees = '';
	
	$alc_user_id		= $user->uid;	 
	$val 				= $form_state['values'];
	$actId				= $val['act_id'];
	$remark_field_title	= $val['renewal_fieldname'];
	$status  			= $val['remark_type'];
	$remark_text 		= $val['remarks_text'];
	$serialFormV		= encryption_decryption_fun('decrypt', $val['formV']);
	$renewal_id			= encryption_decryption_fun('decrypt',$val['renewal_id']);
	$created_by			= encryption_decryption_fun('decrypt',$val['created_by']);
	
	$getRenewalData		= db_query("SELECT license_reneal_fees, created_by FROM l_contractor_license_apply_renweal WHERE id= ".$renewal_id)->fetchAssoc();
	
	if($actId == 13 && $actId!=0 && $actId!='' && !empty($actId) && $renewal_id!='' && $renewal_id!='0' && $created_by == $getRenewalData['created_by'] ){
		
		/** Get User role of ALC ***/
	
		$fetch_users_role		= db_select('users_roles', 'ur');
		$fetch_users_role		->leftJoin('role', 'ro', 'ro.rid=ur.rid');
		$fetch_users_role		->leftJoin('l_custom_user_detail', 'lcud', 'ur.uid=lcud.usr_id');
		$fetch_users_role		->fields('ro', array('rid'));
		$fetch_users_role		->fields('lcud', array('fullname','employee_id'));
		$fetch_users_role		->condition('ur.uid', $alc_user_id);
		$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc(); 
	
		$remark_by_role			= $fetch_users_role_data['rid'];
		$alc_full_name			= $fetch_users_role_data['fullname'];
		$employeeId				= $fetch_users_role_data['employee_id'];
		
		/** Fees Calculation For Renewal with extra Fees ***/
		
		if (in_array("backlog_renewal_extra_amount", explode(',', $remark_field_title))){
			$is_extra_fees		=	 1 ;
			$increment_fees		=	(125/100)*$getRenewalData['license_reneal_fees'];
			$updateFees			=   db_update('l_contractor_license_apply_renweal')->fields(array('is_extra_fees' =>$is_extra_fees,'increnent_fees' =>$increment_fees))->condition('id',$renewal_id)->condition('created_by',$getRenewalData['created_by'])->execute();
		}else{
			$is_extra_fees		=	0 ;
			$increment_fees		=	$getRenewalData['license_reneal_fees'];
			$updateFees			=   db_update('l_contractor_license_apply_renweal')->fields(array('is_extra_fees' =>$is_extra_fees,'increnent_fees' =>$increment_fees))->condition('id',$renewal_id)->condition('created_by',$getRenewalData['created_by'])->execute();
		}
		
		if($status == 'I'){
			
			$file 					= file_load($val['signed_renewal']);   
			if(!empty($file)){	
				$uri 				= trim($file->uri); 
				$file->status 		= FILE_STATUS_PERMANENT;
				file_save($file);
				$certificateFid		= $file->fid;
				$certificateName	= $file->filename;
			}
		}	
		
		/** UPDATE RENEWAL MASTER TABLE **/
		
		$apply_status  		=  	db_update('l_contractor_license_apply_renweal');
		$apply_status		->	fields(array('renewal_appliaction_status' => $status,'remark_by_userid'=> $alc_user_id ,'renewal_certificate_id' => $certificateFid));
		$apply_status		->	condition('id',$renewal_id);
		$apply_status		->	condition('created_by',$getRenewalData['created_by']);
		$apply_status		->  condition('action','R','=');
		$apply_status		->	execute();
		
		$renewalRemarks 	= array(
									'remark_text'   		=> $remark_text, 
									'remark_by'   			=> $alc_user_id, 
									'remark_to' 			=> $serialFormV,
									'remark_type' 			=> $status,
									'remark_date'  			=> time(),
									'remark_field_title' 	=> $remark_field_title,
									'particular_id'			=> $renewal_id,
									'remark_by_role' 		=> '4',
									'remark_by_name' 		=>  $alc_full_name,
									'flag' 					=> 'R',
									'employee_id'			=> $employeeId
									);
							
		db_insert('l_remark_license')->fields($renewalRemarks)->execute();	
		contractor_activity_tag_insert($serialFormV,'R',$renewal_id,$status,$created_by);	
		
		// FOR SMS ALERT START
		
		$applicant_mobile_no = get_mobile_number($getRenewalData['created_by']);
		
		if($status == 'B'){					
			$msg = 'Your Renewal for (00'.$serialFormV.') application is backed for rectification by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';
			send_sms_for_user_alert($applicant_mobile_no, $msg);			
		}else if($status == 'A'){			
			$msg = 'Your Renewal for (00'.$serialFormV.') application is approved for fees submission by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';
			send_sms_for_user_alert($applicant_mobile_no, $msg);
		}else if($status == 'I'){			
			$msg = 'Congratulations! Your Renewal for (00'.$serialFormV.') is issued by ALC(Govt. of WB). Login to download the certificate from dashboard (wblc.gov.in)';
			send_sms_for_user_alert($serialFormV, $msg);
		}else if($status == 'R'){			
			$msg = 'Your Renewal for (00'.$formVSerial.') application is rejected by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';
			send_sms_for_user_alert($serialFormV, $msg);			
		}
		// send_sms_for_user_alert('7603091500', $msg);
				
		// FOR SMS ALERT END
		
		$message = 'Remark has been successfully saved.';
		drupal_set_message(t($message));
		
	}else{
		
	}
}


