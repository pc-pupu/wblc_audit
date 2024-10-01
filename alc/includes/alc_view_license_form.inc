<?php
drupal_add_js(drupal_get_path('module', 'alc').'/js/mycustom.js');
function alc_view_license_form($form,&$form_state,$frmv_no='',$particular_id=''){ 	
	global $base_root, $base_path, $user;
	
	$alc_id 			= $user->uid;
	$view_status 		= '1';
	$act_id				= 12;
	$serial				= encryption_decryption_fun('decrypt', $frmv_no);
	$particularid		= encryption_decryption_fun('decrypt', $particular_id);
	
	/**** Fetch ALC SUBDIVISION Code ****/
	
	$get_alc_subdivision_details		=	db_select('l_customuser_relation_address', 'lcra');
	$get_alc_subdivision_details		->  leftJoin('users_roles', 'ur','ur.uid=lcra.user_id ');
	$get_alc_subdivision_details		->  leftJoin('role', 'ro', 'ro.rid = ur.rid');
	$get_alc_subdivision_details		->  fields('ro', array('name','rid'));
	$get_alc_subdivision_details		->	fields('lcra',array('sub_div_code','block_code','custom_area_jurisdiction','is_custom_area_jurisdiction','rlo_code','lwfc_code'));
	$get_alc_subdivision_details		->	condition('lcra.user_id',$alc_id);
	$get_alc_subdivision_details_result = 	$get_alc_subdivision_details->execute();
	
	if($get_alc_subdivision_details_result-> rowCount() > 0 ){
		
		$alc_data 						= $get_alc_subdivision_details_result->fetchAssoc();  
		

		$alc_subdivision_code 			= trim($alc_data['sub_div_code']);
		$alc_rlo_code 					= trim($alc_data['rlo_code']);
		$alc_role_rid					= $alc_data['rid']; 
		$alc_role_name					= $alc_data['name'];
		$customjurisdiction 			= $alc_data['custom_area_jurisdiction']; 
		$iscustom_juris					= $alc_data['is_custom_area_jurisdiction'];	
	}

	if($customjurisdiction=='BMCNS' && $iscustom_juris == 'Y'){
		$block_code_arr = db_select('l_customuser_reletion_services_wise_user_settings','cra')
		->fields('cra',array('block_code'))
		->condition('cra.user_id', $alc_id)
		->execute()->fetchObject()->block_code;
		$block_code_arr = explode(',', $block_code_arr); 
	}
	
	/*** Get Applicant Subdivision Code & Statuses ****/
	
	$get_licenseDetails	 = db_select('l_particular_of_contract_labour', 'pcl');
	$get_licenseDetails	 ->leftJoin('l_contractor_license_application', 'cla','pcl.id = cla.contractor_particular_id');
	$get_licenseDetails	 ->fields('pcl',array('worksite_subdivision', 'amount_of_licence', 'amount_of_security_deposit','rlo_code','lwfc_code'));
	$get_licenseDetails	 ->fields('cla',array('eodb_app_id', 'caf_id_no', 'status','licence_qr_code','contractor_license_number','certificates_id','id','contractor_user_id', 'license_date'));
	if(!empty($alc_rlo_code) || $alc_rlo_code !=''){
		$get_licenseDetails	 ->condition('pcl.rlo_code',$alc_rlo_code);
	}else{
		$get_licenseDetails	 ->condition('pcl.worksite_subdivision',$alc_subdivision_code);
	}
	// $get_licenseDetails	 ->condition('pcl.worksite_subdivision',$alc_subdivision_code);
	
	
	$get_licenseDetails	 ->condition('pcl.serial_no_from_v',$serial);
	$get_licenseDetails	 ->condition('pcl.id',$particularid);
	$licenseDetails_result = $get_licenseDetails->execute();
	
	if($licenseDetails_result-> rowCount() > 0 ){
		
		$applicant_data = $licenseDetails_result->fetchAssoc(); 
		// echo "<pre>";
		// print_r($applicant_data);die;
		$applicant_status = $applicant_data['status'];
		$worksite_subdivsion_code = $applicant_data['worksite_subdivision'];
		$worksite_rlo_code = $applicant_data['rlo_code'];
		$license_id = $applicant_data['id'];
		
		/*** Get REMARK Details ****/
	
		$get_remark = db_select('l_remark_license', 'remark_license');
		$get_remark->fields('remark_license', array('remark_field_title','remark_by','remark_by_role'));
		$get_remark->condition('remark_license.remark_to', $serial);
		$get_remark->condition('remark_license.particular_id', $particularid);
		$get_remark->condition('remark_license.flag', 'L');
		$get_remark->where('((remark_license.remark_by_role = 4) OR (remark_license.remark_by_role = 7))');
		$get_remark->orderBy('remark_license.id','DESC');
		$get_remark->range(0, 1);
		$get_remark_result = $get_remark->execute()->fetchAssoc();
		
		if(!empty($applicant_status)){
			if($applicant_status == 'F'){
				$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Pending application</h4><p>Application is applied by the Applicant. Any action can be taken for the application.</p></div>';
			} 
			if($applicant_status == "A") { 
				$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Verified and approved for fees submission. </h4><p>Application is approved and directed to pay fees. After fees payment and submission of signed FORM - IV by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
			}
			if($applicant_status == "B"){
				$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i>CURRENT STATUS: Back for rectification</h4><p>Application is sent back for rectification. After modification by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
			}
			if( $applicant_status == "P" ){
				$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Transaction successful</h4><p>Payment successful for this application. FORM-IV is not uploaded by the applicant. After submission of signed FORM-IV by the applicant, the application can be further accessible.</p></div>';
			}
			if( $applicant_status == "BI" ){
				$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i>CURRENT STATUS:  Back to inspector for rectification</h4><p>Application is sent back to Inspector for further verification. After verification, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
			}
			if( $applicant_status == "I"){
				$current_status = '<div class="callout callout-success"><h4>CURRENT STATUS: Certificate (FORM-VI) is issued. </h4><p>Please check the License Certificate after uploading the Certificate.</div>';
			}
			if($applicant_status == "R"){
				$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Rejected application</h4><p>If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
			} 
			if( $applicant_status == "FW"){
				$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Forwarded to ALC by Inspector</h4><p>Application is Forwarded to ALC by Inspector for further verification. Any action can be taken for the application.</p></div>CURRENT STATUS: Application has been FORWARDED TO ALC';
			
			}
			if( $applicant_status == "U"){
				 $current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS:  Form-IV uploaded & application succesfully submitted</h4><p>FORM-IV is submitted by the Applicant. After verification of uploaded FORM-IV, License Certificate can be generated now or can be sent back for rectification of FORM-IV.</p></div>';
			
			}
			if( $applicant_status == "P" && $get_remark_result['remark_by_role'] == $alc_role_rid){
				 $current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i>CURRENT STATUS: Back for Rectification(FORM-IV)</h4><p>FORM-IV is sent back for rectification. After modification by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
			
			}
		}
	}
	
	$info = check_final_submit_data(encryption_decryption_fun('encrypt', 12), encryption_decryption_fun('encrypt', $serial)); 
	//if($applicant_data['contractor_user_id']== 352){
	
		if(!empty($info) && $info['remark_type']=='U'){ 
			$final_submit_date = $info['remark_date'];
			$today = time();						
			$datediff = $today - $final_submit_date;	
			$diff = round($datediff / (60 * 60 * 24));		
		}
	//}


	
	//if(($alc_subdivision_code == $worksite_subdivsion_code )|| ($customjurisdiction=='BMCNS' && $iscustom_juris == 'Y')){   /** Checking for If the Application can be viewed by the ALC of the Subdivision ****/
	
		//if(($alc_rlo_code == $worksite_rlo_code )){   /** Checking for If the Application can be viewed by the ALC of the RLO ****/

	
		$form['#attributes'] = array('enctype' => 'multipart/form-data');
		
		$form['view_license']['markup_data'] = array(
					'#markup' => view_fromIV_details($serial,$particularid,$license_id),
					'#type' => 'markup'
					);															
		$form['view_license']['show_status'] = array(
					'#type' => 'markup',
					'#markup' => $current_status,
					);
															
		$form['view_license']['act_id'] = array(
					'#type' => 'hidden',
					'#default_value' => !empty($act_id) ? $act_id: '',
					'#attributes' => array('readonly' => 'readonly'),
					);															
		$form['view_license']['caf_id_no'] = array(
					'#type' => 'hidden',
					'#value' => !empty($applicant_data['caf_id_no'])?$applicant_data['caf_id_no']:0,				  
					);
		$form['view_license']['eodb_app_id'] = array(
					'#type' => 'hidden',
					'#value' => !empty($applicant_data['eodb_app_id'])?$applicant_data['eodb_app_id']:0,				  
					);
		$form['view_license']['amount_of_licence'] = array(
					'#type' => 'hidden',
					'#value' => !empty($applicant_data['amount_of_licence'])?$applicant_data['amount_of_licence']:0,				  
					);
		$form['view_license']['amount_of_security_deposit'] = array(
					'#type' => 'hidden',
					'#value' => !empty($applicant_data['amount_of_security_deposit'])?$applicant_data['amount_of_security_deposit']:0,				  
					);
		$form['view_license']['contractor_license_number'] = array( 
					'#type' => 'hidden',
					'#value' => !empty($applicant_data['contractor_license_number'])?$applicant_data['contractor_license_number']:0,
					);
		$form['view_license']['license_date'] = array(
					'#type' => 'hidden',
					'#value' => !empty($applicant_data['license_date'])?$applicant_data['license_date']:0,
					);
					
		if($diff <=30){	 /*** NOT DEEMED **/	
			
			if ($applicant_status != 'B' && $applicant_status != 'A' && $applicant_status != 'I' && $applicant_status != 'P' && $applicant_status != 'R'){
		
			$form['view_license']['hidden_message'] = array(
					'#prefix' => '<div class="box box-default box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">ACTIONS AND REMARKS</h3></div><div class="box-body"><span style="margin:0; width:100%;">',
					'#suffix' => '</span>',
					'#type' => 'markup',
					'#markup' => isset($form_state['values']['hidden_message']) ? $form_state['values']['hidden_message'] : ''
					);
														
			$form['view_license']['license_fieldname'] = array(
					'#type' => 'hidden',
					'#attributes' => array('id' => 'license_fieldname', 'readonly'=>'readonly' ),
					'#default_value' => !empty($get_remark_result['remark_field_title']) ? $get_remark_result['remark_field_title'] : '',
					);
														
			$form['view_license']['license_id'] = array(
					'#type' => 	'hidden',
					'#default_value' => $applicant_data['id']!='' ? encryption_decryption_fun('encrypt', $applicant_data['id']) : ''	,
					'#attributes' => array('readonly'=>'readonly' ),
					);															
			$form['view_license']['serial'] = array(
					'#type' => 'hidden',
					'#default_value' => ($serial!='') ? encryption_decryption_fun('encrypt',$serial): '',				
					'#attributes' => array('readonly'=>'readonly')
					);	
															
			$form['view_license']['particularid'] = array(
					'#type' 			=> 	'hidden',
					'#default_value'	=> 	($particularid!='') ? encryption_decryption_fun('encrypt',$particularid) : '',
					'#attributes'		=> 	array('readonly'=>'readonly')
					);
															
			$form['view_license']['contractor_user_id']	= array(
															'#type'				=> 'hidden',
															'#attributes' 		=> array('readonly' => 'readonly'),
															'#default_value'	=> $applicant_data['contractor_user_id']!='' ? encryption_decryption_fun('encrypt',$applicant_data['contractor_user_id']) : '',
															);
														
			$form['view_license']['remark_type']		= array(
															'#prefix' 			=> '<div class="col-md-3">&nbsp;</div><div class="form-custom col-md-6"><label class="input">',
															'#suffix' 			=> '<i></i></label>',
															'#title'			=> 'Please Select Actions',
															'#type'				=> 'select',
															'#required' 		=>  TRUE,
															'#id'				=> 'remark_type_id',
															'#options'			=> actions_dropdown_by_alc_act($applicant_status,'Y',$act_id),
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
			
			$form['view_license']['generate_certificate']= array(
															  '#prefix' 		=> '<div id="generate_certificate_block">',
															  '#suffix' 		=> '</div>'
															);									
																							
			$form['view_license']['download_link'] 		= array(
															  '#prefix' 		=> '<div id="download_link_block">',
															  '#suffix' 		=> '</div>'
															);
			
			$form['view_license']['signed_license']  	= array(
															  '#prefix' 		=> '<div id="signed_license_certificate_block">',
															  '#suffix' 		=> '</div>'
															);																
			
			$form['view_license']['remarks_text'] 		= array(
															  '#prefix' 		=> '<div id="remarks_text_block">',
															  '#suffix' 		=> '</div>'
															);
																							
			$form['view_license']['submit'] 			= array(
															  '#prefix' 		=> '<div id="submit_block">',
															  '#suffix' 		=> '</div>'
															);
															
			if($value_remark_type == 'I'){
				
				if(!empty($applicant_data['licence_qr_code'])){
			
					// $form['view_license']['download_link'] = array(
					// 										'#prefix'			=> '<div id="download_link_block">',
					// 										'#markup' 			=> l(t('<i class="fa fa-download" aria-hidden="true"></i>
					// 	Download License Certificate'),'clra-license/'.encryption_decryption_fun('encrypt', $serial).'/'.encryption_decryption_fun('encrypt',$particularid).'/CLRA-LICENSE-CERTIFICATE', array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn')))),
					// 										'#suffix' 			=> '</div><div class="clear"></div>',
															
					// 										);
															
															
					// $form['view_license']['signed_license']= array(
					// 											'#title'			=> 'Upload Signed License Certificate :-',
					// 											'#type' 			=> 'managed_file',
					// 											'#upload_validators'=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
					// 											'#upload_location' 	=> 'public://upload/alc_upload_certificates_license/',
					// 											'#process' 			=> array('import_my_file_element_process'),
					// 											'#attributes' 		=> array('autocomplete' => 'off','class' => array('form-group')),
					// 											'#prefix'			=> '<div id="signed_license_certificate_block">',
					// 											'#suffix'			=> '</div>',
					// 											'#required' 		=>  TRUE,
					// 											);
																
					// $form['view_license']['remarks_text']= array(
					// 											'#prefix' 		=> '<div id="remarks_text_block"><label class="input">',
					// 											'#suffix' 		=> '</label></div>',
					// 											'#title'		=> 'Remarks',
					// 											'#type'			=> 'textarea',
					// 											'#id'			=> 'remarks_text_id',
					// 											'#rows'			=>  2,
					// 											'#required' 	=>  TRUE,
					// 											);
																									
					$form['view_license']['submit'] 	= array (
														  '#prefix' 			=> '<div id="submit_block">',
														  '#suffix' 			=> '</div></div><div class="col-md-3">&nbsp;</div></div></div>',	
														  '#type' 				=> 'submit',
														  '#submit'				=> array('alc_view_license_form_submit'),
														  '#id'					=> 'ismw_empLicSubmitId',
														  '#attributes' 		=> array('class' => array('btn btn-primary pull-left acremark-btn')),
														  '#value' 				=> 'Submit'
													  
													);
				}else{
					
					$form['view_license']['generate_certificate']= array (
																		  '#prefix' 		=> '<div id="generate_certificate_block">',
																		  '#suffix' 		=> '</div>',
																		  '#type' 			=> 'submit',
																		  '#attributes' 	=> array('class' => array('btn btn-primary pull-left acremark-btn')),
																		  '#value' 			=> 'Generate License Certificate',
																		 // '#submit'			=> array('generation_license_certificate'),
																		);
					
				}
				
			}
															
			if($value_remark_type!='I'){	
							
				$form['view_license']['remarks_text']	= array(
																'#prefix' 		=> '<div id="remarks_text_block"><label class="input">',
																'#suffix' 		=> '</label></div>',
																'#title'		=> 'Remarks',
																'#type'			=> 'textarea',
																'#id'			=> 'remarks_text_id',
																'#rows'			=>  2,
																'#required' 	=>  TRUE,
																);
																				
				$form['view_license']['submit'] 		= array(
															  '#prefix' 		=> '<div id="submit_block">',
															  '#suffix' 		=> '</div></div><div class="col-md-3">&nbsp;</div></div></div>',
															  '#type' 			=> 'submit',
															  '#id'				=> 'ismw_empLicSubmitId',
															  '#attributes' 	=> array('class' => array('btn btn-primary pull-left acremark-btn')),
															  '#value' 			=> 'Submit'
																);
			}
			
		}
		
	  }
		
		$form['view_license']['show_remark_details'] 	= array(
															'#prefix'			=>'<div class="box box-default box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">REMARK SUMMARY</h3></div><div class="box-body">',
															'#type' 			=> 'markup',
															'#markup'			=> show_remark_details($serial,$particularid,$applicant_data['id'],$applicant_data['contractor_user_id']),
															'#suffix' 			=> '</div></div>',
								  );

	// }else{
	// 	$message = 'You are not permitted to view this application.';
	// 	drupal_set_message(t($message), 'error');
	// }

	

	
	return $form;
	
}

function get_call_date_time_field($form, $form_state){
			
	$commands 	= array();
	$commands[] = ajax_command_replace('#download_link_block', drupal_render($form['view_license']['download_link']));
	$commands[] = ajax_command_replace('#signed_license_certificate_block', drupal_render($form['view_license']['signed_license']));
	$commands[] = ajax_command_replace('#generate_certificate_block', drupal_render($form['view_license']['generate_certificate']));
	$commands[] = ajax_command_replace('#remarks_text_block', drupal_render($form['view_license']['remarks_text']));
	$commands[] = ajax_command_replace('#submit_block', drupal_render($form['view_license']['submit']));	
		
	return array('#type' => 'ajax', '#commands' => $commands);
	
}
function view_fromIV_details($serial='',$particularid='',$license_id=''){
	
	drupal_add_js(drupal_get_path('module', 'alc').'/js/mycustom.js'); 
	
	global $base_root, $base_path, $user;
	
	
	/** Contractor Details **/
	
	$fetchContractorData 	= db_select('l_contractor_info_master', 'con_master');
	$fetchContractorData 	->fields('con_master',array());
	$fetchContractorData	->condition('id',$serial);
	$fetchContractorData	->condition('act_id',1);
	$fetchContractorData	= $fetchContractorData->execute();
	
	if($fetchContractorData->rowCount() > 0 ){
		
		$contractorData 		= $fetchContractorData->fetchAssoc();
		$contractor_name		= $contractorData['name_of_contractor'];
		$application_id 		= $contractorData['application_id'];
		$applicant_id 			= $contractorData['user_id'];
		
		
		$conNameAddress = $contractor_name.'<br/>'.$contractorData['address_of_contractor'].'<br/>'.get_full_address('l_contractor_info_master', 'sub-table', $serial, array('con_loc_e_dist','con_loc_e_subdivision','con_loc_e_areatype','con_name_areatype','con_loc_e_vill_ward','con_l_e_ps','contractor_pin'));
		$contractor_type = ($contractorData['contractor_type'] == 1 ) ? 'Offline' : 'New Contractor';
		if($contractorData['contractor_type'] == 1) $formVUploading = 'FORM-V : Not Applicable <br/>REFERENCE NUMBER : 00'.$serial; else $formVUploading = 'YES';
		$active = '<span class="badge bg-green"><i class="fa fa-check" aria-hidden="true"></i> Active</span>';
		$deactive = '<span class="badge bg-red"><i class="fa fa-close"></i> Deactive</span>';
		$contractor_status = ($contractorData['status'] == 1 ) ? $active : $deactive;		
		$con_nature_work = getConNatureOfWrk($serial,$application_id,1 );
		
		$particularData = db_select('l_particular_of_contract_labour', 'particular');
		$particularData->fields('particular',array());
		$particularData->condition('particular.serial_no_from_v',$serial,'=');
		$particularData->condition('particular.id',$particularid,'=');
		$particularData->condition('particular.act_id','1','=');
		$particularLabourRes = $particularData->execute()->fetchAssoc();
	
		if(!empty($particularLabourRes)){		
		
			$contractor_user_id							=	$particularLabourRes['contractor_user_id'];
			$contractor_max_no_of_labours_on_any_day	=	$particularLabourRes['contractor_max_no_of_labours_on_any_day'];
			$unskilled_rate_wages						=   ($particularLabourRes['unskilled_rate_wages']!='' && $particularLabourRes['unskilled_rate_wages']!=0) ?  '&#x20B9; '. $particularLabourRes['unskilled_rate_wages'].' ( Rupees  '.convertNumberToWord($particularLabourRes['unskilled_rate_wages']). 'only)' : 'NIL';
			$semiskilled_rate_wages						=	($particularLabourRes['semiskilled_rate_wages']!='' && $particularLabourRes['semiskilled_rate_wages']!=0) ?  '&#x20B9; '. $particularLabourRes['semiskilled_rate_wages'].' ( Rupees  '.convertNumberToWord($particularLabourRes['semiskilled_rate_wages']). 'only)' : 'NIL';
			$skilled_rate_wages							=	($particularLabourRes['skilled_rate_wages']!='' && $particularLabourRes['skilled_rate_wages']!=0 ) ? '&#x20B9; '. $particularLabourRes['skilled_rate_wages'].' ( Rupees  '.convertNumberToWord($particularLabourRes['skilled_rate_wages']). 'only)' : 'NIL';  
			$highlyskilled_rate_wages					=	($particularLabourRes['highlyskilled_rate_wages']!='' && $particularLabourRes['highlyskilled_rate_wages']!=0) ? '&#x20B9;'. $particularLabourRes['highlyskilled_rate_wages'].' ( Rupees  '.convertNumberToWord($particularLabourRes['highlyskilled_rate_wages']). 'only)' : 'NIL';
			$annual_leave								= 	($particularLabourRes['annual_leave_no']!='' && $particularLabourRes['annual_leave_no']!=0) ? $particularLabourRes['annual_leave_no'].' day(s)' : 'NIL'; 
		  	$casual_leave								= 	($particularLabourRes['casual_leave_no']!='' && $particularLabourRes['casual_leave_no']!=0) ? $particularLabourRes['casual_leave_no'].' day(s)' : 'NIL'; 
		 	$sick_leave									= 	($particularLabourRes['sick_leave_no']!='' && $particularLabourRes['sick_leave_no']!=0) ? $particularLabourRes['sick_leave_no'].' day(s)' : 'NIL';
			$maternity_leave							=	($particularLabourRes['maternity_leave_no']!='' && $particularLabourRes['maternity_leave_no']!=0) ? $particularLabourRes['maternity_leave_no'].' day(s)' : 'NIL'; 
		 	$other_leave								= 	($particularLabourRes['other_leave_no']!='' && $particularLabourRes['other_leave_no']!=0) ? $particularLabourRes['other_leave_no'].' day(s)' : 'NIL';  
			$weekly_holiday								= 	$particularLabourRes['weekly_holiday'];
		  	$no_holiday									= 	$particularLabourRes['no_holiday'];
			$contractor_convicted						=   !empty($particularLabourRes['details_contractor_convicted']) ? $particularLabourRes['details_contractor_convicted'] : 'NIL';
			$contractor_revoking						=   !empty($particularLabourRes['details_contractor_revoking'])!='' && $particularLabourRes['details_contractor_revoking']!='' ? date('dS M, Y',strtotime($particularLabourRes['details_contractor_revoking'])) : 'NIL'; 
			$cooperative								=	($particularLabourRes['is_coparative'] == 1) ?  'Yes' : 'No';
			
			if($particularLabourRes['is_coparative'] == 1){
				$alert = '<font color="#FF0000">[ Reduced 25% of security fees per head of labour, applicable for Co-operative Society ]</font>';
			}else{
				$alert ='';
			}
			
			$rest_holiday 		= substr($no_holiday, 0, -1);
			$no_holiday_list	= explode(',', $rest_holiday);
			$name_holiday		= "";
			$total_holiday		= 0;
			foreach($no_holiday_list as $row){
				$total_holiday = $total_holiday+1;
				if($row=='1'){
					$name_holiday.='Sunday'.',';
				}
				if($row=='2'){
					$name_holiday.='Monday'.',';
				}
				if($row=='3'){
					$name_holiday.='Tuesday'.',';
				}
				if($row=='4'){
					$name_holiday.='Wednesday'.',';
				}
				if($row=='5'){
					$name_holiday.='Thursday'.',';
				}
				if($row=='6'){
					$name_holiday.='Friday'.',';
				}
				if($row=='7'){
					$name_holiday.='Saturday'.',';
				}
				if($row=='8'){
					$name_holiday.='By Rotation'.',';
				}
				
			}
			$name_holidays = substr($name_holiday, 0, -1);
			if(empty($name_holidays))
				$name_holidays	=	'NIL';
			
			if($particularLabourRes!='' && $particularLabourRes['category_of_contractor'] == 1){
				$contractor_type_title 	= 'Commencement date of business';
				$father_husband_name	= 'Not Applicable';
				$contractor_type_value 	= !empty($particularLabourRes['dob_contractor']) ? date('dS M, Y',strtotime($particularLabourRes['dob_contractor'])) : "";
			}else{
				$contractor_type_title = 'Date of Birth and age [in case of individuals]';
				$father_husband_name   = !empty($particularLabourRes['father_contarctor_name']) ? $particularLabourRes['father_contarctor_name'] : "Not Applicable";  
				$contractor_type_value = !empty($particularLabourRes['dob_contractor']) ?date('dS M, Y',strtotime($particularLabourRes['dob_contractor'])).'&nbsp;&nbsp;Age- '.$particularLabourRes['age_contractor'] .' years' : "";
			}
			
			/** Address Fields **/
	
			$worksite_address	= $particularLabourRes['worksite_address_line'].'<br/>'.get_full_address('l_particular_of_contract_labour', 'sub-table', $particularid, array('worksite_dist','worksite_subdivision','work_site_areatype','name_work_site_areatype','work_site_vill_ward','worksite_ps','worksite_pin'));
			
			$conUpdatedAddress	= $particularLabourRes['address_of_contractor'].'<br/>'.get_full_address('l_particular_of_contract_labour', 'sub-table', $particularid, array('contractor_dist','contractor_subdivision','contractor_areatype','contractor_name_areatype','contractor_vill_ward','contractor_ps','contractor_pin'));	
			
			$agentManagerAddress= $particularLabourRes['address_of_manager'].'<br/>'.get_full_address('l_particular_of_contract_labour', 'sub-table', $particularid, array('contractor_manager_dist','contractor_manager_subdivision','contractor_manager_areatype','contractor_manager_name_areatype','contractor_managerr_vill_ward','contractor_manager_ps','manager_pin'));
			
			
			$license_details= license_details($serial,$contractor_user_id);	
			
			/*** Document Section **/
			
			/*$uploaded_workorder			=	db_select('file_managed','fm');
			$uploaded_workorder 		->	fields('fm',array());
			$uploaded_workorder			->	condition('fm.fid',$particularLabourRes['work_order_file_id'],'=');
			$uploaded_workorder_file	=	$uploaded_workorder -> execute()->fetchAssoc();
			
			if(!empty($uploaded_workorder_file)){ 
				$url			 		= 	explode('//',$uploaded_workorder_file['uri']);
				$work_order_url			= 	$url[1];
				$workorder_file			=	$uploaded_workorder_file['filename'];
			}
			
			
			$uploaded_frmv				=	db_select('file_managed','fm');
			$uploaded_frmv 				->	fields('fm',array());
			$uploaded_frmv				->	condition('fm.fid',$particularLabourRes['frm_v_file_id'],'=');
			$uploaded_frmv_file			=	$uploaded_frmv -> execute()->fetchAssoc();
			
			if(!empty($uploaded_frmv_file)){ 
				$url 					= explode('//',$uploaded_frmv_file['uri']);
				$frmv_url				= $url[1];
				$frmv_file				= $uploaded_frmv_file['filename'];
			}
			
			$uploaded_residential		=	db_select('file_managed','fm');
			$uploaded_residential 		->	fields('fm',array());
			$uploaded_residential		->	condition('fm.fid',$particularLabourRes['residential_file_id'],'=');
			$uploaded_residential_file	=	$uploaded_residential -> execute()->fetchAssoc();
			
			if(!empty($uploaded_residential_file)){ 
				$url					= explode('//',$uploaded_residential_file['uri']);
				$residential_url		= $url[1];
				$residential_file		= $uploaded_residential_file['filename'];
			}
			
			$upload_other				=	db_select('file_managed','fm');
			$upload_other 				->	fields('fm',array());
			$upload_other				->	condition('fm.fid',$particularLabourRes['other_file_id'],'=');
			$upload_other_file			=	$upload_other -> execute()->fetchAssoc();
			
			if(!empty($upload_other_file)){ 
				$url					= explode('//',$upload_other_file['uri']);
				$other_url				= $url[1];
				$other_file				= $upload_otheriv_file['filename'];
			}*/
			
			$upload_formiv				=	db_select('file_managed','fm');
			$upload_formiv 				->	fields('fm',array());
			$upload_formiv				->	condition('fm.fid',$particularLabourRes['signed_license_id'],'=');
			$upload_formiv_file			=	$upload_formiv -> execute()->fetchAssoc();
			
			if(!empty($upload_formiv_file)){ 
					$url				= explode('//',$upload_formiv_file['uri']);
					$formiv_url			= $url[1];
					$formiv_file		= $upload_formiv_file['filename'];
			}
		}
		
		/*** Establishment Details **/
		
		$fetchEstablishment 	= 	db_select('l_clra_registration_master', 'est_master');
		$fetchEstablishment->fields('est_master',array('id', 'user_id', 'e_name','loc_e_name','registration_number','act_id','registration_date','full_name_principal_emp','address_principal_emp','loc_emp_country','certificates_fid'))->condition('user_id',$applicant_id,'=');
		$fetchEstablishment	->condition('est_master.act_id','1','=');
		$fetchEstablishment	->condition('est_master.status','I','=');
		$fetchEstablishment->orderBy('est_master.id','DESC');
		$fetchEstablishment->range(0, 1);
		$establishmentData 		= 	$fetchEstablishment->execute()->fetchAssoc();
		
		
		if(!empty($establishmentData)){
				
			$establishmentInfo 	=  $establishmentData['e_name'].'<br/>'.$establishmentData['loc_e_name'].'<br/>'.get_full_address('l_clra_registration_master', 'sub-table', $application_id, array('loc_e_dist','loc_e_subdivision','loc_e_areatype','name_areatype','loc_e_vill_ward','l_e_ps','loc_e_pin_number'));
			
			$get_peNatureOfWrk = pe_natureOfWrk($establishmentData['id'],$establishmentData['user_id'],1);
			
			
			if($establishmentData['loc_emp_country'] == 1){
				
				$peInfo	= $establishmentData['full_name_principal_emp'].'<br/>'.$establishmentData['address_principal_emp'].'<br/>'.get_full_address('l_clra_registration_master', 'sub-table', $application_id, array('loc_emp_dist','loc_emp_subdivision','loc_emp_areatype','emp_name_areatype','loc_emp_vill_ward','l_emp_ps','loc_emp_pin_number','loc_emp_state'));
				
			}else{
				
				$peInfo	= $establishmentData['full_name_principal_emp'].'<br/>'.$establishmentData['address_principal_emp'];
				
			}
			
			$reg_certificate	=	db_select('file_managed','fm');
			$reg_certificate 	->	fields('fm',array());
			$reg_certificate	->	condition('fm.fid',$establishmentData['certificates_fid']);
			$regFile			=	$reg_certificate ->execute()->fetchAssoc();
			if(!empty($regFile)){ 
				$certificates_url	= '';
				$url			 	= 	explode('//',$regFile['uri']);
				$certificates_url	= 	$url[1];
					
			}
		}
		
		
		/** Payment Info ***/
		
		$check_payment_mod  		= 	db_select('l_contractor_license_application', 'license');	
		$check_payment_mod			->  fields('license', array('payment_mode', 'status','is_backlog_license','backlog_license_date','certificates_id','license_date','deemed', 'eodb_app_id', 'caf_id_no'));	
		$check_payment_mod			->	condition('license.serial_no_from_v', $serial);	
		$check_payment_mod_result	= 	$check_payment_mod->execute()->fetchAssoc();
		
		$payment_mod 				= 	$check_payment_mod_result['payment_mode'];
		$status 					= 	$check_payment_mod_result['status'];
		$is_backlog_license			=	$check_payment_mod_result['is_backlog_license'];
		$backlog_license_date		=	date('dS M, Y', strtotime($check_payment_mod_result['backlog_license_date']));
		
		if($is_backlog_license	==	1){
			$text = '<font color=#FF0000><strong>( Manually Issued License on '.$backlog_license_date.' )</strong></font>';
		}else{
			$text =	'';
		}
		
		if(!empty($check_payment_mod_result['certificates_id']) && $status == 'I'){
			$upload_certificates_id 		= db_select('file_managed','fm');
			$upload_certificates_id			-> fields('fm',array());
			$upload_certificates_id			-> condition('fid',$check_payment_mod_result['certificates_id']);
			$upload_certificates_file		= $upload_certificates_id-> execute()->fetchAssoc();
		
			if(!empty($upload_certificates_file)){ 
			  $url									= explode('//',$upload_certificates_file['uri']);
			  $upload_certificates_file_url			= $url[1];
			  $upload_certificates_file_name		= $upload_certificates_file_name['filename'];
			  $certificateDownloadLink				= l(t('<i class="fa fa-download" aria-hidden="true"></i>&nbsp; Signed FORM-VI License Certificate'),$GLOBALS['base_url'].'/sites/default/files/'.$upload_certificates_file_url, array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn'))));
			}
		}
			
		if(trim($payment_mod)	== 'O' && ($status == 'P' || $status == 'U' || $status == 'I')){  /** Offline Payment **/
		
			$offline_pay_query 	= db_select('l_offline_payment_info', 'lopi');								
			$offline_pay_query	->fields('lopi', array('grn_number', 'approved_by', 'challan_file_name'));
			$offline_pay_query	->condition('lopi.application_id', $serial);
			$offline_pay_query	->condition('lopi.act_id', 12);								
			$offline_pay_result = $offline_pay_query->execute()->fetchObject();
	  
		   $payment_details 	 = '<u>Manual Payment</u><br>';
		   $payment_details 	.= 'GRN NO.:<span class="color_orange">'.$offline_pay_result->grn_number.'&nbsp;'.l('Click here view details', 'https://www.wbifms.gov.in/GRIPS/grn_status.do', array('attributes' => array('title' =>'Click here to view details', 'target' => '_blank'))).'</span><br>';
		   $payment_details 	.= 'Total Amount:<span class="color_orange">  &#8377; ' .($particularLabourRes['amount_of_licence']+$particularLabourRes['amount_of_security_deposit']).'</span><br>';
		   $payment_details 	.= 'Challan:<span class="color_orange">&nbsp;'.l('Click here view Challan', '/sites/default/files/upload/challan_upload/offline/'.$offline_pay_result->challan_file_name, array('attributes' => array('title' =>'Click here to view details', 'target' => '_blank'))).'</span><br>';	
		   
		}else{  /** Online Payment **/
			
			$transaction_details  			= 	db_select('l_principle_epayments_receive_data', 'lpd');
			$transaction_details			->	leftJoin('l_principle_epayments_data', 'lped', 'lpd.transaction_id = lped.identification_no');
			$transaction_details			->  fields('lped', array('identification_no', 'application_id'));
			$transaction_details			->  fields('lpd', array());
			$transaction_details			->	condition('lped.act_id', '12');
			$transaction_details			->	condition('lpd.banktransactionstatus', 'Success');
			$transaction_details			->	condition('lped.application_id',$serial);
			$transaction_details			->	condition('lped.deposited_by',$contractor_user_id); 
			$transaction_det 				= 	$transaction_details->execute()->fetchAssoc();
		
			$grn_number						= 	'Not available';
			$bank_code 						= 	'Not available';
			$bankTransactionID 				= 	'Not available';
			$total_amount 					= 	'Not available';
			$challan_fid_date 				= 	'Not available';
			$payment_status 				=   'Not available';
			
			if(!empty($transaction_det)){
				
				$bankTransactionID				= $transaction_det['transaction_id'];
				$grn_number						= $transaction_det['challanrefid'];
				$challan_fid_date				= !empty($transaction_det['challanrefid_date']) ? $transaction_det['challanrefid_date'] : '';
				$total_amount					= $transaction_det['challanamount']!='' ? '&#8377; '. number_format($transaction_det['challanamount'], 2): 'Not available';
				$bank_code						= $transaction_det['bank_cd'];
				
				if($transaction_det['banktransactionstatus'] == 'Success') {
					$payment_status = '<font color="#060">'.$transaction_det['banktransactionstatus'].'</font>';
				}else{
					$payment_status = '<font color="#CC0000">'.$transaction_det['banktransactionstatus'].'</font>';
				}
			}
			
			$payment_details = '<u>GRIPS PAYMENT</u><br>';
			// $payment_details .= 'Identification Number: <span>'.l($bankTransactionID.' <i class="fa fa-info-circle" aria-hidden="true"></i>', 'epayments-info/verification/'.encryption_decryption_fun('encrypt', $bankTransactionID), array('html' => TRUE, 'attributes'=> array('target'=>'_blank'))).'</span><br>';
			$payment_details .= 'GRN: <span class="color_orange">'.l($grn_number.' <i class="fa fa-info-circle" aria-hidden="true"></i>', 'epayments-info/verification/'.encryption_decryption_fun('encrypt', $bankTransactionID), array('html' => TRUE, 'attributes'=> array('target'=>'_blank'))).'</span><br>';				
			$payment_details .= 'Total Amount: <span class="color_orange">'.$total_amount.'</span><br>';
			$payment_details .= 'Transaction Date: <span class="color_orange">'.$challan_fid_date.'</span><br>';
			// $payment_details .= 'IFSC Code: <span class="color_orange">'.$bank_code.'</span><br>';		
			$payment_details .= 'Transaction Status: '.$payment_status;
		}
		
		
		/** Not Required Presently ***/
		
		
		if($user->name == 'alc1howlcwb' && $serial == 206080){
			$get_status_query = db_select('l_contractor_license_application', 'lcla');	
			$get_status_query->fields('lcla', array('status'));	
			$get_status_query->condition('lcla.serial_no_from_v', $serial);	
			$get_status_query_result = $get_status_query->execute()->fetchAssoc();
			if(trim($get_status_query_result['status']) == 'A'){
				$payment_details.= '<br>'.l('Click here to update payment status', 'offilne-payment-mode/'. encryption_decryption_fun('encrypt', $serial).'/'.encryption_decryption_fun('encrypt', 12));				
			}
	 	}
		
		/*** Get Remark Data For License ***/
		
		
		$get_remark = db_select('l_remark_license', 'remark_license');
		$get_remark->fields('remark_license', array('remark_field_title'));
		$get_remark->condition('remark_license.remark_to', $serial);
		$get_remark->condition('remark_license.particular_id', $particularid);
		$get_remark->condition('remark_license.flag', 'L');
		$get_remark->where("((remark_license.remark_by_role = 4) OR (remark_license.remark_by_role = 7))");
		$get_remark->orderBy('remark_license.id','DESC');
		$get_remark->range(0, 1);
		$get_remark_result = $get_remark->execute()->fetchAssoc();
		
		$category_of_contractor = $contractor_dist_ck = $dob_contractor_ck= $name_of_agent_ck = $address_of_manager_ck = $address_of_manager_ck = $category_designation_ck = $unskilled_rate_wages_ck = $semiskilled_rate_wages_ck = $highlyskilled_rate_wages_ck = $hours_work_ck = $overtime_ck = $overtime_wages_ck = $annual_leave_no_ck = $casual_leave_no_ck = $sick_leave_no_ck = $maternity_leave_no_ck = $other_leave_no_ck = $holiday_ck = $contractor_convicted_ck = $contractor_revoking_ck = $contractor_previous_employer_ck = $worksite_add_ck = $frm_v_ck = $frm_v_file_id_ck = $work_order_file_id_ck = $residential_file_id_ck = $other_doc_id_ck =$form_iv_id_ck = '';
		if(!empty($get_remark_result['remark_field_title'])){
			$remark_field_text_arr = explode(',', $get_remark_result['remark_field_title']);
			
			foreach ($remark_field_text_arr as $fieldname){
				
				if(trim($fieldname) == 'category_of_contractor'){ $category_of_contractor = "checked='checked'";}
				if($fieldname == 'contractor_dist'){ $contractor_dist_ck = "checked='checked'";}
				if($fieldname == 'is_cooparative'){ $is_cooparative_ck = "checked='checked'";}	
				if($fieldname == 'dob_contractor'){ $dob_contractor_ck = "checked='checked'";}
				if($fieldname == 'name_of_agent'){ $name_of_agent_ck = "checked='checked'";}
				if($fieldname == 'address_of_manager'){ $address_of_manager_ck = "checked='checked'";}	
				if($fieldname == 'category_designation'){ $category_designation_ck = "checked='checked'";}
				if($fieldname == 'unskilled_rate_wages'){ $unskilled_rate_wages_ck = "checked='checked'";}	
				if($fieldname == 'semiskilled_rate_wages'){ $semiskilled_rate_wages_ck = "checked='checked'";}
				if($fieldname == 'skilled_rate_wages'){ $skilled_rate_wages_ck = "checked='checked'";}	
				if($fieldname == 'highlyskilled_rate_wages'){ $highlyskilled_rate_wages_ck = "checked='checked'";}
				if($fieldname == 'hours_work'){ $hours_work_ck = "checked='checked'";}
				if($fieldname == 'overtime'){ $overtime_ck = "checked='checked'";}
				if($fieldname == 'overtime_wages'){ $overtime_wages_ck = "checked='checked'";}
				if($fieldname == 'annual_leave_no'){ $annual_leave_no_ck = "checked='checked'";}
				if($fieldname == 'casual_leave_no'){ $casual_leave_no_ck = "checked='checked'";}
				if($fieldname == 'sick_leave_no'){ $sick_leave_no_ck = "checked='checked'";}
				if($fieldname == 'maternity_leave_no'){ $maternity_leave_no_ck = "checked='checked'";}
				if($fieldname == 'other_leave_no'){ $other_leave_no_ck = "checked='checked'";}
				if($fieldname == 'holiday'){ $holiday_ck = "checked='checked'";}
				if($fieldname == 'contractor_convicted'){ $contractor_convicted_ck = "checked='checked'";}
				if($fieldname == 'contractor_revoking'){ $contractor_revoking_ck = "checked='checked'";}
				if($fieldname == 'contractor_previous_employer'){ $contractor_previous_employer_ck = "checked='checked'";}
				if($fieldname == 'worksite_add'){ $worksite_add_ck = "checked='checked'";}
				if($fieldname == 'frm_v'){ $frm_v_ck = "checked='checked'";}
				if($fieldname == 'frm_v_file_id'){ $frm_v_file_id_ck = "checked='checked'";}
				if($fieldname == 'work_order_file_id'){ $work_order_file_id_ck = "checked='checked'";}
				if($fieldname == 'residential_file_id'){ $residential_file_id_ck = "checked='checked'";}
				if($fieldname == 'other_doc_id'){ $other_doc_id_ck = "checked='checked'";}
				if($fieldname == 'form_iv_id'){ $form_iv_id_ck = "checked='checked'";}
			}
		}
	
    if(!empty($check_payment_mod_result['caf_id_no'])){
		$application_details = '<tr><td></td><td>CAF Number</td><td>'.$check_payment_mod_result['caf_id_no'].'</td><td></td> </tr>
							    <tr><td></td><td>Form V Serial Number</td><td>'.$serial.'</td><td></td></tr>';
	}
	$output = '';				
	$output	.= '<div class="row equal-panels">
					<div class="col-md-6 col-sm-12">
						<div class="box box-primary box-solid">
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
										<td>'.$establishmentData['registration_number'].' issued on '.date('dS M, Y',strtotime($establishmentData ['registration_date'])).'<br/>'.l('Registration Certificate',$GLOBALS['base_url'].'/sites/default/files/'.$certificates_url,array('attributes' => array('target' => '_blank'), 'html' => TRUE)).'&nbsp;
										<span class="viewfile_popup" id="regCertificate">
												<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
											</span>
											<div id="view_regCertificate"  title="Registration Certificate" style="display:none;">
												<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$certificates_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
											</div>
											
											&nbsp;&nbsp;&nbsp;'.
											l('<span class="badge bg-blue"><i class="fa fa-eye"></i> View More of Registration<div>', 'alc-visible-applications/'.encryption_decryption_fun('encrypt', $application_id).'/'.encryption_decryption_fun('encrypt', $applicant_id), array('html' => TRUE, 'target' => '_blank')).'
										
										
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
						<div class="box box-primary box-solid">
							<div class="box-header with-border">
							  <h3 class="box-title">2. Contractor Details provided by Principal Employer [ Verified ]</h3>
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
										<td width="40%">Name and address of Contractor</td>
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
										<td>'.date('dS M, Y',strtotime($contractorData['est_date_of_work_of_each_labour_from_date'])).' TO '.date('dS M, Y',strtotime($contractorData['est_date_of_work_of_each_labour_to_date'])).'</td>
									</tr>
									<tr>
										<td>2.(e)</td>
										<td>Maximum number of contract labour proposed to be employed in the establishment on any date</td>
										<td>'.$contractorData['contractor_max_no_of_labours_on_any_day'].'</td>
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
					  <h3 class="box-title">3. Inputs are provided by Contractor [For Verification]</h3>
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
							'.$application_details.'
							<tr>
								<td>1.(a)</td>
								<td>Father&#13;s name of the contractor (including his father&#13;s name in case of individuals) </td>
								<td>'.$father_husband_name.'</td>
								<td><input type="checkbox" id="category_of_contractor" class="yn_license_verified"  '.$category_of_contractor.' </td> 
							</tr>
							<tr>
								<td>1.(b)</td>
								<td>Address of the contractor </td>
								<td>'.$conUpdatedAddress.'</td>
								<td><input type="checkbox" id="contractor_dist" class="yn_license_verified"  '.$contractor_dist_ck.'</td>
							</tr>
							<tr>
								<td>2.(a)</td>
								<td><strong>Co-operative Society</strong>  <font color="red">**</font></td>
								<td>'.$cooperative.'</td>
								<td><input type="checkbox" id="is_cooparative" class="yn_license_verified"  '.$is_cooparative_ck.'</td>
							</tr>
							<tr>
								<td>2.(b)</td>
								<td>'.$contractor_type_title.'</td>
								<td>'.$contractor_type_value.'</td>
								<td><input type="checkbox" id="dob_contractor" class="yn_license_verified" '.$dob_contractor_ck.' />  </td>
							</tr>
							<tr>
								<td>3.(a)</td>
								<td>Name of the agent or manager of contractor at the work site </td>
								<td>'.$particularLabourRes['name_of_agent'].'</td>
								<td><input type="checkbox" id="name_of_agent" class="yn_license_verified" '.$name_of_agent_ck.' />  </td>
							</tr>
							<tr>
								<td>3.(b)</td>
								<td>Address of the agent or manager of contractor at the work site</td>
								<td>'.$agentManagerAddress.'</td>
								<td><input type="checkbox" id="address_of_manager" class="yn_license_verified" '.$address_of_manager_ck.' />  </td>
							</tr>
							<tr>
								<td>4. </td>
								<td>Category/designation/nomenclature of the contractor labour, namely,fitter,welder,carpenter,mazdor etc.</td>
								<td>'.$particularLabourRes['category_designation'].'</td>
								<td><input type="checkbox" id="category_designation" class="yn_license_verified" '.$category_designation_ck.' />  </td>
							</tr>
							<tr>
								<td colspan="4" style="font-size:15px; font-weight:600;">
									5.&nbsp;&nbsp;&nbsp;Rate of Wages, DA and other cash benefits paid / to be paid to each category (i.e (a) Unskilled (b) Semi-Skilled (c) Skilled (d) Highly-Skilled etc.) of contract labour</td>
							</tr>
							<tr>
								<td>5.(a)</td>
								<td>Rate of wages,DA and Other cash benefits paid/ to be paid to Unskilled of contract labour:</td>
								<td>'.$unskilled_rate_wages.'</td>
								<td><input type="checkbox" id="unskilled_rate_wages" class="yn_license_verified"  '.$unskilled_rate_wages_ck.'  </td>
							</tr>
							<tr>
								<td>5.(b)</td>
								<td>Rate of wages,DA and Other cash benefits paid/ to be paid to Semi-skilled of contract labour:</td>
								<td>'.$semiskilled_rate_wages.'</td>
								<td><input type="checkbox" id="semiskilled_rate_wages" class="yn_license_verified" '.$semiskilled_rate_wages_ck.'  </td>
							</tr>
							<tr>
								<td>5.(c)</td>
								<td>Rate of Wages,DA and Other cash benefits paid/ to be paid to Skilled of contract labour:</td>
								<td>'.$skilled_rate_wages.'</td>
								<td><input type="checkbox" id="skilled_rate_wages" class="yn_license_verified"  '.$skilled_rate_wages_ck.'</td>
							</tr>
							<tr>
							 	<td>5.(d)</td>
								<td>Rate of Wages,DA and Other cash benefits paid/ to be paid to Highly-skilled of contract labour:</td>
								<td>'.$highlyskilled_rate_wages.'</td>
								<td><input type="checkbox" id="highlyskilled_rate_wages" class="yn_license_verified" '.$highlyskilled_rate_wages_ck.' </td>
						  	<tr>
							<tr>
								<td colspan="4" style="font-size:15px; font-weight:600;">6.&nbsp;&nbsp;&nbsp;Hours of Work, Overtime and Overtime Wages</td>
							</tr>
							<tr>
								<td>6.(a)</td>
								<td>Hours of Work</td>
								<td>'.$particularLabourRes['hours_work'].' hr(s)</td>
								<td><input type="checkbox" id="hours_work" class="yn_license_verified"  '.$hours_work_ck.' </td>
							</tr>
							<tr>
								<td>6.(b)</td>
								<td>Overtime</td>
								<td>'.$particularLabourRes['overtime'].' hr(s)</td>
								<td><input type="checkbox" id="overtime" class="yn_license_verified"  '.$overtime_ck.'</td>
							</tr>
							<tr>
								<td>6.(c)</td>
								<td>Overtime Wages</td>
								<td>&#x20B9; '.$particularLabourRes['overtime_wages'].' ( per hour )</td>
								<td><input type="checkbox" id="overtime_wages" class="yn_license_verified" '.$overtime_wages_ck.' </td> 
							</tr>
							<tr>
								<td colspan="4" style="font-size:15px; font-weight:600;">
								7.&nbsp;&nbsp;&nbsp;Other Condition of service like leave ( annual leave, casual leave,sick leave, maternity leave etc.) Holidays etc. of the contract labour</td>
							</tr>
							<tr>
								<td>7.(a)</td>
								<td>Number of annual leave</td>
								<td>'.$annual_leave.'</td>
								<td><input type="checkbox" id="annual_leave_no" class="yn_license_verified" '.$annual_leave_no_ck.' </td>
							</tr>
							<tr>
								<td>7.(b)</td>
								<td>Number of casual leave</td>
								<td>'.$casual_leave.'</td>
								<td><input type="checkbox" id="casual_leave_no" class="yn_license_verified" '.$casual_leave_no_ck.'  </td>
							</tr>
							<tr>
								<td>7.(c)</td>
								<td>Number of sick leave</td>
								<td>'.$sick_leave.'</td>
								<td><input type="checkbox" id="sick_leave_no" class="yn_license_verified"  '.$sick_leave_no_ck.' </td>
							</tr>
							<tr>
								<td>7.(d)</td>
								<td>Number of maternity leave</td>
								<td>'.$maternity_leave.'</td>
								<td><input type="checkbox" id="maternity_leave_no" class="yn_license_verified" '.$maternity_leave_no_ck.'</td>
							</tr>
							<tr>
								<td>7.(e)</td>
								<td>Number of other leave</td>
								<td>'.$other_leave.'</td>
								<td><input type="checkbox" id="other_leave_no" class="yn_license_verified"  '.$other_leave_no_ck.' </td>
							</tr>
							<tr>
								<td>7.(f)</td>
								<td>Holiday(s)</td>
								<td>'.$total_holiday.' day(s) ('.$name_holidays.')</td>
								<td><input type="checkbox" id="holiday" class="yn_license_verified"  '.$holiday_ck.' </td>
							</tr>
							<tr>
								<td>8. </td>
								<td>Whether the contractor was convicted of any offence within the preceeding five years. If so, give details</td>
								<td>'.$contractor_convicted.'</td>
								<td><input type="checkbox" id="contractor_convicted" class="yn_license_verified" '.$contractor_convicted_ck.'  </td>
							</tr>
							<tr>
								<td>9. </td>
    							<td>Whether there was any order against the contract or revoking or suspending license or forfeiting security deposit in respect of an earlier contract. If so, the date of such order.</td>
    							<td>'.$contractor_revoking.'</td>
    							<td><input type="checkbox" id="contractor_revoking" class="yn_license_verified"   '.$contractor_revoking_ck.' </td>
  							</tr>
							<tr>
								<td>10. </td>
    							<td>Whether the contractor has worked in any other establishment within the past five years. If so. give details of the principal employer, establishment and nature of work</td>
    							<td>'.$license_details.'</td>
    							<td><input type="checkbox" id="contractor_previous_employer" class="yn_license_verified"  '.$contractor_previous_employer_ck.' </td>
  							</tr>
							<tr>
								<td>11. </td>
    							<td><strong>Work Site Address <strong><font color="red">**</font> </td>
    							<td>'.$worksite_address.'</td>
    							<td><input type="checkbox" id="worksite_add" class="yn_license_verified" '.$worksite_add_ck.' </td>
  							</tr>	
							<tr>
								<td>12. </td>
    							<td>Whether a certificate by the principal employer in FORM-V is enclosed</td>
    							<td>'.$formVUploading.'</td>
    							<td><input type="checkbox" id="frm_v" class="yn_license_verified"  '.$frm_v_ck.'  </td>
  							</tr>
							<tr>
								<td>13. </td>
    							<td>Maximum number of contract labour proposed to be employed in the establishment on any date</td>
    							<td>'.$contractor_max_no_of_labours_on_any_day.'&nbsp;&nbsp; <font color="#006600"> <b>[ FEES IS CALCULATED BASED ON THIS VALUE] </b></font></td>
    							<td><input type="checkbox" id="" checked="checked"  disabled="disabled" class="yn_license_verified"</td>
  							</tr>
							 <tr>
								<td colspan="3" style="font-weight:700; text-transform:uppercase; font-size:15px;">14. Documents Uploaded</td>
							 </tr>
							 <tr>
								<td>i.)</td>
								<td>FORM - V</td>';
								$encfilearr = db_select('l_encrypted_uploaded_documents','leud')->fields('leud',array('document_type_code'))->condition('user_id',$contractor_user_id)->condition('application_id', $serial)->condition('status', 1)->orderBy('id', 'desc')->execute()->fetchAll();
								foreach($encfilearr as $arr){ 
									$arr1[] = $arr->document_type_code;
								} // echo '<pre>'; print_r($arr1); die;							   
									if(!empty($particularLabourRes['frm_v_file_id'])){
										$file_status_FV = fetch_documents_status_or_url($contractor_user_id,'12',$license_id,'FV',$particularLabourRes['frm_v_file_id']); 
										if($file_status_FV != '1'){
											$file_url = $file_status_FV;
											$output.='<td><a title = "Click here to view Form V" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$file_url.'" target="_blank"><img src="'.$GLOBALS['base_url'].'/'. drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
										}else{
											$view_form_v = l('<i class="fa fa-file fa-lg"></i>','license-doc-view/FV/'.encryption_decryption_fun('encrypt', $serial).'/'.encryption_decryption_fun('encrypt', $license_id),array('html' => TRUE,'attributes'=>array('target'=>'_blank','class'=>array('#pdf-img')))); 
										$output .= '<td>'.$view_form_v;
										}
										/*$uploaded_FV_filename = get_license_filename($serial, $license_id, 'FV');
										$output .= '<span class="viewfile_popup" id="form_five">
														<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
													</span>
													<div id="view_form_five"  title="FORM-V" style="display:none;">
														<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/common/'.$uploaded_FV_filename.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
													</div>';*/
									    $output .= '</td>';
									
									}elseif(in_array('FV', $arr1)){ 
										$output .=  '<td>'.l('<i class="fa fa-file fa-lg"></i>', 'documents-view/'.encryption_decryption_fun('encrypt', $contractor_user_id).'/'.encryption_decryption_fun('encrypt', $serial).'/'.encryption_decryption_fun('encrypt', 'FV'), array('html' => TRUE,'attributes'=>array('target'=>'_blank','class'=>array('#pdf-img')))).'</td>';
									}else {
										
										$output .=  '<td>No Document Uploaded</td>';
										
									}
									$output .=  '<td><input type="checkbox" id="frm_v_file_id" class="yn_license_verified" '.$frm_v_file_id_ck.' </td>
							</tr>	
							 <tr>
								<td>ii.)</td>
								<td>Work Order</td>';
									if(!empty($particularLabourRes['work_order_file_id'])){
										$file_status_WO = fetch_documents_status_or_url($contractor_user_id,'12',$license_id,'WO',$particularLabourRes['work_order_file_id']); 
										if($file_status_WO != '1'){
											$file_url = $file_status_WO;
											$output.='<td><a title = "Click here to view work order" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$file_url.'" target="_blank"><img src="'.$GLOBALS['base_url'].'/'. drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
										}else{
										$view_work_order = l('<img src =  "'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png">','license-doc-view/WO/'.encryption_decryption_fun('encrypt', $serial).'/'.encryption_decryption_fun('encrypt', $license_id),array('html' => TRUE,'attributes'=>array('target'=>'_blank','class'=>array('#pdf-img'))));
										$output .= '<td>'.$view_work_order;
										}
										/*$uploaded_WO_filename = get_license_filename($serial, $license_id, 'WO');
										$output .= '<span class="viewfile_popup" id="workorder">
														<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
													</span>
													<div id="view_workorder"  title="Work Order" style="display:none;">
														<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/common/'.$uploaded_WO_filename.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
													</div>';*/
										$output .= '</td>';
									}elseif(in_array('WO', $arr1)){ 
										$output .=  '<td>'.l('<i class="fa fa-file fa-lg"></i>', 'documents-view/'.encryption_decryption_fun('encrypt', $contractor_user_id).'/'.encryption_decryption_fun('encrypt', $serial).'/'.encryption_decryption_fun('encrypt', 'WO'), array('html' => TRUE,'attributes'=>array('target'=>'_blank','class'=>array('#pdf-img')))).'</td>';
									}else {
										
										$output .=  '<td>No Document Uploaded</td>';
										
									}
									$output .=  '<td><input type="checkbox" id="work_order_file_id" class="yn_license_verified" '.$work_order_file_id_ck.' </td>
							</tr>
							<tr>
								<td>iii.)</td>
								<td>Residential Certificate / Trade Licence</td>';
									if(!empty($particularLabourRes['residential_file_id'])){
										$file_status_TL = fetch_documents_status_or_url($contractor_user_id,'12',$license_id,'TL',$particularLabourRes['residential_file_id']); 
										if($file_status_TL != '1'){
											$file_url = $file_status_TL;
											$output.='<td><a title = "Click here to view residential certificate" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$file_url.'" target="_blank"><img src="'.$GLOBALS['base_url'].'/'. drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
										}else{
										$view_residential_file = l('<img src = "'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png">','license-doc-view/TL/'.encryption_decryption_fun('encrypt', $serial).'/'.encryption_decryption_fun('encrypt', $license_id),array('html' => TRUE,'attributes'=>array('target'=>'_blank','class'=>array('#pdf-img'))));
										$output .= '<td>'.$view_residential_file;
										}
										/*$uploaded_TL_filename = get_license_filename($serial, $license_id, 'TL');
										$output .= '<span class="viewfile_popup" id="res_cert">
														<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
													</span>
													<div id="view_res_cert"  title="Residential Certificate / Trade Licence" style="display:none;">
														<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/common/'.$uploaded_TL_filename.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
													</div>';*/
										$output .= '</td>';
									}elseif(in_array('AP', $arr1)){ 
										$output .=  '<td>'.l('<i class="fa fa-file fa-lg"></i>', 'documents-view/'.encryption_decryption_fun('encrypt', $contractor_user_id).'/'.encryption_decryption_fun('encrypt', $serial).'/'.encryption_decryption_fun('encrypt', 'AP'), array('html' => TRUE,'attributes'=>array('target'=>'_blank','class'=>array('#pdf-img')))).'</td>';
									}else {
										
										$output .=  '<td>No Document Uploaded</td>';
										
									}
									$output .=  '<td><input type="checkbox" id="residential_file_id" class="yn_license_verified"   '.$residential_file_id_ck.' </td>
							</tr>
							<tr>
								<td>iv.)</td>
								<td>Other Document</td>'; 
									if(!empty($particularLabourRes['other_file_id'])){
										$file_status_OD = fetch_documents_status_or_url($contractor_user_id,'12',$license_id,'OD',$particularLabourRes['other_file_id']); 
										if($file_status_OD != '1'){
											$file_url = $file_status_OD;
											$output.='<td><a title = "Click here to view otehr document" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$other_url.'" target="_blank"><img src="'.$GLOBALS['base_url'].'/'. drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
										}else{
										$view_other_file = l('<img src = "'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png">','license-doc-view/OD/'.encryption_decryption_fun('encrypt', $serial).'/'.encryption_decryption_fun('encrypt', $license_id),array('html' => TRUE,'attributes'=>array('target'=>'_blank','class'=>array('#pdf-img'))));
										$output .= '<td>'.$view_other_file;
										}
										/*$uploaded_OD_filename = get_license_filename($serial, $license_id, 'OD');
										$output .= '<span class="viewfile_popup" id="form_six">
															<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
													</span>
													<div id="view_form_six"  title="FORM-VI" style="display:none;">
														<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/common/'.$uploaded_OD_filename.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
													</div>';*/
										$output .= '</td>';
									}elseif(in_array('OD', $arr1)){ 
										$output .=  '<td>'.l('<i class="fa fa-file fa-lg"></i>', 'documents-view/'.encryption_decryption_fun('encrypt', $contractor_user_id).'/'.encryption_decryption_fun('encrypt', $serial).'/'.encryption_decryption_fun('encrypt', 'OD'), array('html' => TRUE,'attributes'=>array('target'=>'_blank','class'=>array('#pdf-img')))).'</td>';
									}else {
										
										$output .=  '<td>No Document Uploaded</td>';
										
									}
									$output .=  '<td><input type="checkbox" id="other_doc_id" class="yn_license_verified"'.$other_doc_id_ck.'</td>
							</tr>
							<tr>
								<td>vi.)</td>
								<td>FORM-IV</td>';
								 if(!empty($particularLabourRes['signed_license_id'])){
									 
									 $output .= '<td>
													<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$formiv_url.'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;
													<span class="viewfile_popup" id="signed_pdf_file">
														<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
													</span>
													<div id="view_signed_pdf_file"  title="Signed FORM-IV" style="display:none;">
														<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$formiv_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
													</div>';
													if($status =='P' || $status=='U' || $status=='I' ){
										 $output .= '&nbsp;&nbsp;&nbsp;
													'.l(t('Generated FORM - IV'),'alc-view-formiv/'.encryption_decryption_fun('encrypt', $serial).'/'.encryption_decryption_fun('encrypt', $particularid).'/'.encryption_decryption_fun('encrypt', $contractor_user_id), array('html' => TRUE,'attributes'=> array('target'=>'_blank')));
								 					}
									$output .= '</td>';
									  
								}elseif(in_array('FIV', $arr1)){ 
										$output .=  '<td>'.l('<i class="fa fa-file fa-lg"></i> View', 'documents-view/'.encryption_decryption_fun('encrypt', $contractor_user_id).'/'.encryption_decryption_fun('encrypt', $serial).'/'.encryption_decryption_fun('encrypt', 'FIV'), array('html' => TRUE, 'attributes'=> array('target'=>'_blank'))).'
										<span class="viewfile_popup" id="signed_pdf_file"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
										<div id="view_signed_pdf_file"  title="Signed FORM-IV" style="display:none;"><iframe src="'.$base_root.'/documents-view/'.encryption_decryption_fun('encrypt', $contractor_user_id).'/'.encryption_decryption_fun('encrypt', $serial).'/'.encryption_decryption_fun('encrypt', 'FIV').'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>
										</td>';	
								}else{
									
									$output .=  '<td>Signed FORM-IV will be uploaded by the Applicant after payment of fees</td>';
								}
								
								if($status == 'U'){
									$disabled = '';	
								}else{
									$disabled = 'disabled="disabled"';
								}
										
								$output .= '<td id="formVId"><input type="checkbox" id="form_iv_id" '.$disabled.' class="yn_license_verified"   '.$form_iv_id_ck.'> </td>
							</tr>';
							if($status=='I' ){
				$output .=	'<tr>
								<td>vii.)</td>
								<td>License </td>';
								 if(!empty($check_payment_mod_result['certificates_id'])){
									 
									 $output .= '<td>
									 				'.$certificateDownloadLink.'&nbsp;&nbsp;&nbsp;
													<span class="viewfile_popup" id="signed_license">
														<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
													</span>
													<div id="view_signed_license"  title="Signed License" style="display:none;">
														<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$upload_certificates_file_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
													</div>';
													
									$output .= '</td>';
								}else if($check_payment_mod_result['certificates_id']=='' && $check_payment_mod_result['license_date']!=''){
									
									$generatedcertificate = l(t('<i class="fa fa-download" aria-hidden="true"></i>&nbsp; License Certificate'),$GLOBALS['base_url'].'/clra-license/'.encryption_decryption_fun('encrypt', $serial).'/'.encryption_decryption_fun('encrypt', $particularid).'/CLRA-LICENSE-CERTIFICATE', array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn'))));
									
									 $output .= '<td>
									 				'.$generatedcertificate.'&nbsp;&nbsp;&nbsp;
													<span class="viewfile_popup" id="signed_license">
														<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
													</span>
													<div id="view_signed_license"  title="License" style="display:none;">
														<iframe src="'.$GLOBALS['base_url'].'/clra-license/'.encryption_decryption_fun('encrypt', $serial).'/'.encryption_decryption_fun('encrypt', $particularid).'/CLRA-LICENSE-CERTIFICATE" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
													</div>';
													
									$output .= '</td>';
									if($check_payment_mod_result['deemed'] == 'Y'){
										$output .= '<td>Deemed Approval</td>';
									}else{
										$output .= '<td>Issued by ALC</td>';
									}
									
								}
					$output .= '</tr>';
							}
			$output .= '</table>
						<table class="table table-bordered">
						   <tr>
							<td colspan="3" style="font-weight:700; text-transform:uppercase; font-size:16px;">15. Payment Details</td>
						   </tr>
						   <tr>
							  <td width="10%">15.i.) </td>
							  <td>License Fees</td>
							  <td width="50%"> &#8377; ' .number_format($particularLabourRes['amount_of_licence'],2).'  &nbsp;&nbsp;&nbsp;&nbsp;
							  	<button type="button" class="btn btn-info" data-toggle="modal" data-target="#myModalFeeschart"><i class="fa fa-info-circle"></i>&nbsp;Fees Chart</button>
							  </td>
							</tr>
							<tr>
							  <td>15.ii.) </td>
							  <td>Security Fees  '.$alert.'</td>
							  <td> &#8377; ' .number_format($particularLabourRes['amount_of_security_deposit'], 2).'.00</td>
							</tr>
							<tr>
							  <td>15.iii.) </td>
							  <td>Payment Details</td>
							  <td>' .$payment_details.'</td>
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
				
	}else{
		drupal_set_message(t('Invalid FORM-V / Reference Number for this application.'), 'error');
		drupal_set_message(t($message), 'error');
	}
	
 	$output .= '<div class="row"><ul style="padding:0;"><li class="col-md-2 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-arrow-left"></i>&nbsp;Back to list</div>', 'alc-received-license', array('attributes' => array('title' => 'Click here back to applications list'), 'html' => TRUE)).'</li>';
	
	$output .= '<li class="col-md-2 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-user"></i>&nbsp;View Applicant Profile</div>', 'view-applicant-profile/'.encryption_decryption_fun('encrypt', $serial).'/'.encryption_decryption_fun('encrypt', $contractor_user_id).'/'.encryption_decryption_fun('encrypt', 12), array('attributes' => array('title' => 'Click here view Applicant Profile'), 'html' => TRUE)).'</li>';
	
	$output .= '<li class="col-md-4 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-info-circle"></i>&nbsp;Instructions to give remarks</div>', $base_root.$base_path.'sites/default/files/instructions-remarks.pdf', array('attributes' => array('title' => 'Instructions to give remarks'), 'html' => TRUE)).'</li>';
	
	$output .= '<li class="col-md-4 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-question-circle" aria-hidden="true"></i>&nbsp;How to digitally sign your documents using USB Token</div>', 'digitally-sign-process', array('attributes' => array('title' => 'How to digitally sign your documents using USB Token'), 'html' => TRUE)).'</li></ul></div>';
	
	return $output;	

}
function show_remark_details($serial='',$particularid='', $license_id='',$contractor_user_id='' ){
		
	 
    global $user;
	 
	$user_id 		= $user->uid;
	$counter		= 0;
	$status			= '';
	
	$query_frmv 	= db_select('l_contractor_info_master', 'cim');
	$query_frmv 	->fields('cim',array('is_from_v','contractor_type'));
	$query_frmv		->condition('cim.id',$serial);
	$formV 			= $query_frmv->execute()->fetchAssoc();
	
	if($formV['contractor_type']=='1'){
		$formVSerial	= 'REF No.: 00'.$serial;
	}else{
		$formVSerial	= 'FORM-V : 00'.$serial;
	}
	
	$header = array(
					array('data' => 'Sl', 'field' => 'slno','width' => '5%'),
					array('data' => 'FORM-V / REF NO.', 'field' => 'serial','width' => '10%'),
					array('data' => 'Remark', 'field' => 'remrk'),
					array('data' => 'Date - Time', 'field' => 'date','width' => '15%'),
					array('data' => 'Remark Status', 'field' => 'remark type','width' => '15%'),
					array('data' => 'Remark By', 'field' => 'licenseno','width' => '20%'),
					array('data' => 'Action','width' => '10%')
				);

	$remark_details			=	db_select('l_remark_license','remark');
	$remark_details			->	InnerJoin('l_custom_user_detail', 'cus_detl','cus_detl.usr_id = remark.remark_by');
	$remark_details			->	InnerJoin('role', 'r','r.rid = remark.remark_by_role');
	$remark_details 		->	fields('remark',array());
	$remark_details 		->	fields('cus_detl',array('fullname','employee_id'));
	$remark_details 		->	fields('r',array('name'));
	$remark_details			->	condition('remark.remark_to',$serial,'=');
	$remark_details			->	condition('remark.particular_id',$particularid,'=');
	$remark_details			->	condition('remark.flag','L','=');
	$remark_details			->	condition('remark.is_active','1');
	$remark_or				=	db_or();
	$remark_or				->	condition('remark.remark_by_role',4);
	$remark_or				->	condition('remark.remark_by_role',7);
	$remark_details			->	condition($remark_or);
	$remark_details			->	orderBy('remark.id','DESC');
	$remark_details_result	=	$remark_details -> execute()->fetchAll();
	
	
	foreach($remark_details_result as $row){
		
		$counter++;
		$remark_by				= $row->remark_by_name.'&nbsp;('.$row->name.')';
		
		$application_details	= contractor_activity_tag_del($serial,$contractor_user_id);
		$flag_application		= $application_details['flagapplication'];
		
		if($row->remark_type == "P" && $row->remark_by_role == 4){
			$status 			= '<span class="backed" title=" Rectify FORM-IV"></span>';
		}else{
			$status 			= get_user_license_status($row->remark_type);
			
		}
		
		if($flag_application == 'L'){
			
			$getPayInfo	 	= db_query("SELECT COUNT(*) AS total_row FROM l_principle_epayments_data WHERE act_id=12 AND application_id = ".$serial."")->fetchObject();
			$getId		 	= $getPayInfo->total_row; 
			
			$getMaxData		= db_query("SELECT MAX(id) AS MAXID FROM l_remark_license WHERE is_active=1 and remark_to= ".$serial." and flag= 'L' and particular_id=".$particularid."")->fetchObject();
			$maxId			= $getMaxData->maxid;
			
			if($maxId>0){
				if($row->remark_type!='I'){
					if($row->remark_type=='U'){
						$url 			= 'delete-remarks/'.encryption_decryption_fun('encrypt',$row->id).'/'.encryption_decryption_fun('encrypt',$serial).'/'.encryption_decryption_fun('encrypt',$particularid).'/'.encryption_decryption_fun('encrypt',12);	
						$delete			= l('<div id="delBtn" class="delete-btn active-del">&nbsp;</div>',$url,array('html' => TRUE));
						
						}else{  //$getId =0;
						if(($row->remark_by==$user_id) &&($maxId==$row->id) && ($getId==0)){
							$url 	= 'delete-remarks/'.encryption_decryption_fun('encrypt',$row->id).'/'.encryption_decryption_fun('encrypt',$serial).'/'.encryption_decryption_fun('encrypt',$particularid).'/'.encryption_decryption_fun('encrypt',12);	
							$delete	= l('<div id="delBtn" class="delete-btn active-del">&nbsp;</div>',$url,array('html' => TRUE));	
						}else{
							$delete	= '<div class="delete-btn disable-del">&nbsp;</div>';
						}	
					}
				}else{
					$delete	= '<div class="delete-btn disable-del">&nbsp;</div>';	
				}
			}
		}
		
		$rows[] = array(
					array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
					array('data' => $formVSerial, 'align' => 'left', 'class' => array('odd')),
					array('data' => $row->remark_text, 'align' => 'left', 'class' => array('odd')),
					array('data' => date('dS M, Y - g:i a', $row->remark_date), 'align' => 'left', 'class' => array('odd')),
					array('data' => $status, 'align' => 'left', 'class' => array('odd')),
					array('data' =>$remark_by, 'align' => 'left', 'class' => array('odd')),
					array('data' =>$delete, 'align' => 'left', 'class' => array('odd'))
				);
	}


	$variables = array(
					'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')), 
					'header' 	 => $header,
					'rows'		 => $rows,
					'empty'		 => 'No data found', 
				);
	$output = theme('datatable', $variables);
	return '<div class="feedback-scroll">'.$output.'</div>';			
}
function get_action_field($form, $form_state){
	$commands 	= array();	
	$commands[] = ajax_command_replace('#call_applicants', drupal_render($form['view_license']['call_applicant']));
	$commands[] = ajax_command_replace('#genarate_certificates', drupal_render($form['view_license']['download_certificates']));
	$commands[] = ajax_command_replace('#edit-upload-certificates-ajax-wrapper', drupal_render($form['view_license']['certificates']));
	$commands[] = ajax_command_replace('#remarks_text_div', drupal_render($form['view_license']['remarks_text']));
	$commands[] = ajax_command_replace('#gen_pdf_div', drupal_render($form['view_license']['gen_pdf']));
	$commands[] = ajax_command_replace('#submit_div', drupal_render($form['view_license']['submit']));
	$commands[] = ajax_command_replace('#hidden_action_div', drupal_render($form['view_license']['hidden_action']));	
	return array('#type' => 'ajax', '#commands' => $commands);	
}


function alc_view_license_form_submit($form,&$form_state){	
	global $base_root, $base_path, $user;
	$alc_user_id 		= $user->uid;	
	$val 				= $form_state['values'];
	$license_issued		= 0;
	$particularid 		= encryption_decryption_fun('decrypt', $val['particularid']);
	$contractor_user_id	= encryption_decryption_fun('decrypt', $val['contractor_user_id']);
	$license_id			= encryption_decryption_fun('decrypt', $val['license_id']);
	$formVSerial 		= encryption_decryption_fun('decrypt', $val['serial']);
	$actId 				= $val['act_id'];
	$status 			= $val['remark_type'];
	$remark_text 		= $val['remarks_text'];
	$remark_field_title	= $val['license_fieldname'];
	
	$serviceResponse = 1;
	
	if($actId == 12 && $actId!=0 && $actId!='' && !empty($actId) && $formVSerial!='' && $formVSerial!='0'){	
	
		/** Get User role of ALC ***/
	
		$fetch_users_role = db_select('users_roles', 'ur');
		$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
		$fetch_users_role->leftJoin('l_custom_user_detail', 'lcud', 'ur.uid=lcud.usr_id');
		$fetch_users_role->fields('ro', array('rid'));
		$fetch_users_role->fields('lcud', array('fullname','employee_id'));
		$fetch_users_role->condition('ur.uid', $alc_user_id);
		$fetch_users_role_data = $fetch_users_role->execute()->fetchAssoc(); 
	
		$remark_by_role	= $fetch_users_role_data['rid'];
		$alc_full_name	= $fetch_users_role_data['fullname'];
		$employeeId	= $fetch_users_role_data['employee_id'];	
	
		/** Generation of License Number**/
		if($status == 'I'){
			 
			/*** Get Applicant Block Code & Statuses  ****/
	 
			$license				=	db_select('l_contractor_license_application', 'license_master');
			$license				->  leftJoin('l_particular_of_contract_labour', 'particular','particular.id=license_master.contractor_particular_id ');
			$license				->  leftJoin('l_contractor_info_master', 'contractor','contractor.id=particular.serial_no_from_v ');
			$license				->	fields('particular',array('contractor_user_id','worksite_subdivision','name_work_site_areatype'));
			$license				->	fields('contractor',array('name_of_contractor'));
			$license				->	fields('license_master',array('contractor_license_number','licence_qr_code','caf_id_no'));
			$license				->	condition('license_master.id',$license_id);
			$license				->	condition('particular.id',$particularid);
			$license				->	condition('particular.contractor_user_id',$contractor_user_id);
			$license				->  condition('particular.serial_no_from_v',$formVSerial);
			$license_result 		= $license->execute();
			
			if($license_result-> rowCount() > 0 ){
				$applicant_data 	= 	$license_result->fetchAssoc();
		
				$subdivision_code	=   $applicant_data['worksite_subdivision'];
				$area_code			=   $applicant_data['name_work_site_areatype'];
				$license_number		=   $applicant_data['contractor_license_number'];
				$license_qr_code	=   $applicant_data['licence_qr_code']; 
				$contractorName		= 	$applicant_data['name_of_contractor'];
				$contractor_user_id	= 	$applicant_data['contractor_user_id'];
				$cafId 				= 	$applicant_data['caf_id_no'];
				
				
				if (empty($license_number) && empty($license_qr_code)){
				
					/*** GENERATION OF LICENSE NUMBER ***/

					//$getShortNameSubdivision =  custom_user_short_name_fun($subdivision_code);
					$getRloShortCode 		=  get_rlo_short_code($area_code); // RLO code added on 19/03/2024
					$getRegistrarCode		=  get_registration_code($area_code);  
					$registrarCode 			=  substr($getRegistrarCode, -2); 
					
					$reg_query				=	db_query("select  max (NULLIF(substr(contractor_license_number,11,6),'') :: integer) as serial_num  from l_contractor_license_application where substr(contractor_license_number,0,4) ='".$getRloShortCode."'");
					$reg_query_result		=	$reg_query->fetchAssoc();
					$reg_code				=	$reg_query_result['serial_num'];
					
					if(empty($reg_code)){ 
						$license_number		=	$getRloShortCode.$registrarCode.'/'.'CLL'.'/'.'000001';
					}else{
						$reg_code_next		= 	$reg_code+1; 
						$reg_first			=	$getRloShortCode.$registrarCode.'/'.'CLL'.'/';
						$reg_second			=	str_pad($reg_code_next, 6, "0", STR_PAD_LEFT);
						$license_number		=	$reg_first.$reg_second;
						
					}
					
					$renewalDate 			= date("Y-m-d", strtotime("+364 days", strtotime(date('Y-m-d'))));
					$SLNo					= 'CLL'.$formVSerial.'V'.'T'.time();
					$licence_qr_code		= encryption_decryption_fun('encrypt',"License No:".$license_number."\nIssue Date:". date("dS M, Y")."\nName of Contractor:".$contractorName."\nValid upto:".date("dS M, Y",strtotime($renewalDate))."\nIssued By:".$alc_full_name."\nSL.No:".$SLNo);
					
					/*** Update Status of Master License Table ***/ 
					$update_status  		=  db_update('l_contractor_license_application');
					$update_status->fields(array('licence_qr_code' => $licence_qr_code, 'contractor_license_number'=> $license_number, 'license_date' => date("Y-m-d"),'next_renweal_date' => $renewalDate ,  'block_next_renwal_date' =>$renewalDate));
					$update_status->condition('id',$license_id);
					$update_status->condition('serial_no_from_v',$formVSerial);
					$update_status->condition('contractor_particular_id',$particularid);
					$update_status->condition('contractor_user_id',$contractor_user_id);
					$update_status->execute();
					
					/*** Update Contractor Master Table ***/ 
			
					$update_contractor  	= db_update('l_contractor_info_master');
					$update_contractor->fields(array('contractor_user_id'=> $contractor_user_id));
					$update_contractor->condition('id',$formVSerial);
					$update_contractor->execute();
					
					$remark_text = 'Congratulations! Certificate is issued by the Registering Authority. You can download it from the dashboard.';
					
				}
				
			}
			
		}
		
		$fetchCommonId = db_select('l_common_application_master', 'cam');
		$fetchCommonId->fields('cam', array('id', 'oswicsapplicationflag', 'ldapplicationflag', 'bocwa_ld_service_id', 'ld_service_unique_id', 'ld_uid', 'ld_est_unique_id','email'));
		$fetchCommonId->condition('cam.user_id', $contractor_user_id);		
		$commonId = $fetchCommonId->execute()->fetchAssoc();
		
		if($commonId['ldapplicationflag'] == 8888){  // 8 // SILPASATHI				
			$serviceResponse = 0;
			if($status == 'B'){
				$status_text = 'Back for Correction';
			}elseif($status == 'A'){
				$status_text = 'Approved';
			}elseif($status == 'I'){
				$status_text = 'Issued';			
			}elseif($status == 'R'){
				$status_text = 'Rejected';
			}
			$eodb_app_id = $val['eodb_app_id'];
			$caf_id_no = $val['caf_id_no'];	
			
			if($status == 'A'){	
				$paymentdetails[] = array(
						'payableamt' => $val['amount_of_licence'],
						'hoa' => '0230-00-106-001-16',
						'purpose' => 'Licence Fees',
						);
				$paymentdetails[] = array(
						'payableamt' => $val['amount_of_security_deposit'],
						'hoa' => '8443-00-103-001-07',
						'purpose' => 'Security Fees',
						);			
				$curl_post_data = array (
 						'source' => 'WBLC',
						'taskid' => 'PAYMENTINFO',
						'eodb_app_id' => $val['eodb_app_id'],
						'caf_id_no' => $val['caf_id_no'],
						'deptCode' => '050',
						'svcCode' => '301',
						'payabletotalamt' => $val['amount_of_licence']+$val['amount_of_security_deposit'],
						'periodFrom' => date('Y-m-d'),
						'periodTo' => date('Y-m-d'),
						'paymentdetails' => $paymentdetails,			
						'status_code' => $status,
						'status_text' => $status_text,
						'status_date' => date('Y-m-d H:m:s'),
						'license_id' => $license_id,
						'particuler_id' => $particularid,
						'contractor_user_id' => $contractor_user_id,
						'status_remarks' => $remark_text					
						);	
			}elseif($status == 'I'){				
				$pdf_gen_url = 'https://wblc.gov.in/clra-license/'.encryption_decryption_fun('encrypt', $formVSerial).'/'.encryption_decryption_fun('encrypt', $particularid).'/CLRA-LICENSE-CERTIFICATE';							
				$file_contents = file_get_contents($pdf_gen_url);
							
				$data_file[] = array(
						'file_id' => $val['contractor_license_number'],
						'file_date' => $val['license_date'],
						'file_category' => 'CERTIFICATE',
						'file_name' => str_replace('/', '_',$val['contractor_license_number']).'_'.$license_id.'.pdf',
						'file_type' => 'application/pdf',
						'file_content' =>  base64_encode($file_contents),
						'valid_upto' =>  date('Y-m-d', strtotime('+1 years'))
						);
				$curl_post_data = array (
 						'source' => 'WBLC',
						'taskid' => 'STATUSDATA',
						'eodb_app_id' => $val['eodb_app_id'],
						'caf_id_no' => $val['caf_id_no'],									
						'status_code' => $status,
						'status_text' => $status_text,
						'status_date' => date('Y-m-d H:m:s'),
						'status_remarks' => 'Congratulations! Certificate is issued by the Registering Authority. You can download it from the dashboard.',
						'application_id' => $license_id,
						'license_id' => $license_id,
						'data_file' => $data_file,
						'additionalParamDtls' => $additionalParamDtls			
						);			
			}else{
				$curl_post_data = array (
 						'source' => 'WBLC',
						'taskid' => 'STATUSDATA',
						'eodb_app_id' => $val['eodb_app_id'],
						'caf_id_no' => $val['caf_id_no'],									
						'status_code' => $status,
						'status_text' => $status_text,
						'status_date' => date('Y-m-d H:m:s'),
						'status_remarks' => $remark_text,
						'application_id' => $license_id,
						'license_id' => $license_id,
						'particuler_id' => $particularid,
						'contractor_user_id' => $contractor_user_id						
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
			$errors     = curl_error($ch); // print_r($errors); die;
			curl_close($ch);			
			$results = json_decode(trim($results), TRUE); // echo '<pre>'; print_r($results); die;
	 
		   if($results['code'] == 200){
				$serviceResponse = 1;				
		   }
		}
		
		// API		
		if($serviceResponse == 1){
			/** UPDATE LICENSE MASTER TABLE **/
		
			$apply_status  =  db_update('l_contractor_license_application');
			//$apply_status->fields(array('status'=>$status,'modification_date' =>date("Y-m-d"),'remark_by_userid'=> $alc_user_id,'certificates_id' =>$certificateFid,'license_issued'=>$license_issued));
			$apply_status->fields(array('status'=>$status,'modification_date' =>date("Y-m-d"),'remark_by_userid'=> $alc_user_id,'license_issued'=>$license_issued));
			$apply_status->condition('serial_no_from_v',$formVSerial);
			$apply_status->condition('contractor_particular_id',$particularid);
			$apply_status->execute();		
		
			/** Insert In LICENSE Remarks Master Table **/
		
			if($status == 'A'){
				$remark_field_title ='category_of_contractor, contractor_dist,is_cooparative, dob_contractor, name_of_agent, address_of_manager, category_designation, unskilled_rate_wages, semiskilled_rate_wages,skilled_rate_wages,highlyskilled_rate_wages,hours_work, overtime, overtime_wages,annual_leave_no, casual_leave_no,sick_leave_no, maternity_leave_no, other_leave_no,holiday, contractor_convicted, contractor_revoking, contractor_previous_employer, worksite_add,frm_v,frm_v_file_id, work_order_file_id, residential_file_id, other_doc_id';
			}
		
			$licenseRemarks 	= array(
									'remark_text'   		=> $remark_text, 
									'remark_by'   			=> $alc_user_id, 
									'remark_to' 			=> $formVSerial,
									'remark_type' 			=> $status,
									'remark_date'  			=> time(),
									'remark_field_title' 	=> $remark_field_title,
									'particular_id'			=> $particularid,
									'remark_by_role' 		=> '4',
									'remark_by_name' 		=>  $alc_full_name,
									'flag' 					=>  'L',
									'employee_id'			=> $employeeId
									);
									
			db_insert('l_remark_license')->fields($licenseRemarks)->execute();	
		
			contractor_activity_tag_insert($formVSerial,'L',$license_id,$status,$contractor_user_id);
		
			// FOR SMS ALERT START
		
			$applicant_mobile_no = get_mobile_number($contractor_user_id);
			
			if($status == 'B'){					
				$msg = 'Your License for (00'.$formVSerial.') application is backed for rectification by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';
				send_sms_for_user_alert($applicant_mobile_no, $msg);			
			}else if($status == 'A'){			
				$msg = 'Your License for (00'.$formVSerial.') application is approved for fees submission by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';
				send_sms_for_user_alert($applicant_mobile_no, $msg);
			}else if($status == 'I'){			
				$msg = 'Congratulations! Your License for (00'.$formVSerial.') is issued by ALC(Govt. of WB). Login to download the certificate from dashboard (wblc.gov.in)';
				send_sms_for_user_alert($applicant_mobile_no, $msg);								
			}else if($status == 'R'){			
				$msg = 'Your License for (00'.$formVSerial.') application is rejected by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';
				send_sms_for_user_alert($applicant_mobile_no, $msg);						
			}	
			
			if($commonId['ldapplicationflag'] == 8 && $status == 'I'){ 
				$applicant_mailto  = get_email($contractor_user_id);

				db_update('users')->fields(array('status'=>1))->condition('status',0)->condition('uid',$contractor_user_id)->execute();
				$msg = 'Your License for (00'.$formVSerial.') is issued by ALC(Govt. of WB). Login to download the certificate from dashboard.<br>You may avail other services from <strong><a href="'.$base_root.$base_path.'">wblc.gov.in</a></strong><br/><br/> <strong></strong>Username :- '.$cafId.'<br/>Email : - '.$applicant_mailto.'<br/>Mobile : - '.$applicant_mobile_no.'<br/>Also, you may find your user credentials from <a href="'.$base_root.$base_path.'find-user-details">CLICK HERE</a> with your application information. You can generate your password from <a href="'.$base_root.$base_path.'forgot-password">Forgot your password</a> ';
	
				 
				$subject = 'Welcome to Labour Commissionerate, Govt. Of West Bengal';
				if(!empty($applicant_mailto)){
					send_mail_for_user_alert($applicant_mailto, $subject, $msg);
					//send_mail_for_user_alert('wblc.nic@gmail.com', $subject, $msg);
				}
				
		    }
			$message = 'Remark has been successfully saved.';
			drupal_set_message(t($message));
		}		
	}
}
function generation_license_certificate($form, &$form_state) {
	
	 global $user;
	 
	 $val   				= $form_state['values'];
	 $alcUserId 			= $user->uid;
 	 $serialFormV  			= encryption_decryption_fun('decrypt',$val['serial']);
  	 $particular_id			= encryption_decryption_fun('decrypt',$val['particularid']);
	 $license_id			= encryption_decryption_fun('decrypt',$val['license_id']);
	 $contractor_user_id	= encryption_decryption_fun('decrypt',$val['contractor_user_id']);
	 $license_fieldname		= $val['license_fieldname'];
	 $remarktype 			= $val['remark_type'];
	 
	 /** Get User role of ALC ***/
	
	$fetch_users_role		= db_select('users_roles', 'ur');
	$fetch_users_role		->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role		->leftJoin('l_custom_user_detail', 'lcud', 'ur.uid=lcud.usr_id');
	$fetch_users_role		->fields('ro', array('rid'));
	$fetch_users_role		->fields('lcud', array('fullname','employee_id'));
	$fetch_users_role		->condition('ur.uid', $alcUserId);
	$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc(); 

	$remark_by_role			=	$fetch_users_role_data['rid'];
	$alc_full_name			=   $fetch_users_role_data['fullname'];
	$employeeId				= 	$fetch_users_role_data['employee_id'];
	 
	 /*** Get Applicant Block Code & Statuses  ****/
	 
	$license				=	db_select('l_contractor_license_application', 'license_master');
	$license				->  leftJoin('l_particular_of_contract_labour', 'particular','particular.id=license_master.contractor_particular_id ');
	$license				->  leftJoin('l_contractor_info_master', 'contractor','contractor.id=particular.serial_no_from_v ');
	$license				->	fields('particular',array('contractor_user_id','worksite_subdivision','name_work_site_areatype'));
	$license				->	fields('contractor',array('name_of_contractor'));
	$license				->	fields('license_master',array('contractor_license_number','licence_qr_code'));
	$license				->	condition('license_master.id',$license_id);
	$license				->	condition('particular.id',$particular_id);
	$license				->	condition('particular.contractor_user_id',$contractor_user_id);
	$license				->  condition('particular.serial_no_from_v',$serialFormV);
	$license_result 		= $license->execute();
	
	if($license_result-> rowCount() > 0 ){
		
		$applicant_data 	= 	$license_result->fetchAssoc();
		
		$subdivision_code	=   $applicant_data['worksite_subdivision'];
		$area_code			=   $applicant_data['name_work_site_areatype'];
		$license_number		=   $applicant_data['contractor_license_number'];
		$license_qr_code	=   $applicant_data['licence_qr_code']; 
		$contractorName		= 	$applicant_data['name_of_contractor'];
		$contractor_user_id	= 	$applicant_data['contractor_user_id'];
		
		if (empty($license_number) && empty($license_qr_code)){
			
			/*** GENERATION OF LICENSE NUMBER ***/
			
			$getShortNameSubdivision =  custom_user_short_name_fun($subdivision_code); 
			$getRegistrarCode		 =	get_registration_code($area_code);  
			$registrarCode 			 =  substr($getRegistrarCode, -2); 
			
			$reg_query				=	db_query("select  max (NULLIF(substr(contractor_license_number,11,6),'') :: integer) as serial_num  from l_contractor_license_application where substr(contractor_license_number,0,4) ='".$getShortNameSubdivision."'");
			$reg_query_result		=	$reg_query->fetchAssoc();
			$reg_code				=	$reg_query_result['serial_num'];
			
			if(empty($reg_code)){ 
				
				$license_number			=	$getShortNameSubdivision.$registrarCode.'/'.'CLL'.'/'.'000001';

			}else{
				$reg_code_next			= 	$reg_code+1; 
				$reg_first				=	$getShortNameSubdivision.$registrarCode.'/'.'CLL'.'/';
				$reg_second				=	str_pad($reg_code_next, 6, "0", STR_PAD_LEFT);
				$license_number			=	$reg_first.$reg_second;
			}
			
        	$renewalDate 				= date("Y-m-d", strtotime("+364 days", strtotime(date('Y-m-d'))));
			$SLNo						= 'CLL'.$serialFormV.'V'.'T'.time();
			$licence_qr_code			= encryption_decryption_fun('encrypt',"License No:".$license_number."\nIssue Date:". date("dS M, Y")."\nName of Contractor:".$contractorName."\nValid upto:".date("dS M, Y",strtotime($renewalDate))."\nIssued By:".$alc_full_name."\nSL.No:".$SLNo);		
			
			
			/*** Update Status of Master License Table ***/ 
			
			//if($contractor_user_id==352){
				
				$update_status  			=  db_update('l_contractor_license_application');
				$update_status->fields(array('licence_qr_code' => $licence_qr_code, 'contractor_license_number' =>	$license_number, 'license_date' => date("Y-m-d"),'next_renweal_date' => $renewalDate ,'modification_date'=> date("Y-m-d"), 'remark_by_userid'=> $alcUserId , 'block_next_renwal_date' =>$renewalDate,'status'=>'I'));
				$update_status->condition('id',$license_id);
				$update_status->condition('serial_no_from_v',$serialFormV);
				$update_status->condition('contractor_particular_id',$particular_id);
				$update_status->condition('contractor_user_id',$contractor_user_id);
				$update_status->execute();
				
				$licenseRemarks 	= array(
									'remark_text'   		=> 'Congratulations! Certificate is issued by the Registering Authority. You can download it from the dashboard.', 
									'remark_by'   			=> $alcUserId, 
									'remark_to' 			=> $serialFormV,
									'remark_type' 			=> $remarktype,
									'remark_date'  			=> time(),
									'remark_field_title' 	=> $license_fieldname,
									'particular_id'			=> $particular_id,
									'remark_by_role' 		=> '4',
									'remark_by_name' 		=>  $alc_full_name,
									'flag' 					=>  'L',
									'employee_id'			=> $employeeId
									);
									
			db_insert('l_remark_license')->fields($licenseRemarks)->execute();	
		
			contractor_activity_tag_insert($serialFormV,'L',$license_id,$remarktype,$contractor_user_id);
			
		    /*** Update Contractor Master Table ***/ 
			
			$update_contractor  	=  	db_update('l_contractor_info_master');
			$update_contractor->fields(array('contractor_user_id'=> $contractor_user_id));
			$update_contractor->condition('id',$serialFormV);
			$update_contractor->execute();
		
			// FOR SMS ALERT START
		
			$applicant_mobile_no = get_mobile_number($contractor_user_id);
			
			if($status == 'B'){					
				$msg = 'Your License for (00'.$serialFormV.') application is backed for rectification by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';
				send_sms_for_user_alert($applicant_mobile_no, $msg);			
			}else if($status == 'A'){			
				$msg = 'Your License for (00'.$serialFormV.') application is approved for fees submission by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';
				send_sms_for_user_alert($applicant_mobile_no, $msg);
			}else if($status == 'I'){			
				$msg = 'Congratulations! Your License for (00'.$serialFormV.') is issued by ALC(Govt. of WB). Login to download the certificate from dashboard (wblc.gov.in)';
				send_sms_for_user_alert('7278989577', $msg);
			}else if($status == 'R'){			
				$msg = 'Your License for (00'.$serialFormV.') application is rejected by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';
				send_sms_for_user_alert($applicant_mobile_no, $msg);			
			}		
				
			// FOR SMS ALERT END
				
			/*}else{
				$update_status  			=  db_update('l_contractor_license_application');
				$update_status->fields(array('licence_qr_code' => $licence_qr_code, 'contractor_license_number' =>	$license_number, 'license_date' => date("Y-m-d"),'next_renweal_date' => $renewalDate ,'modification_date'=> date("Y-m-d"), 'remark_by_userid'=> $alcUserId , 'block_next_renwal_date' =>$renewalDate));
				$update_status->condition('id',$license_id);
				$update_status->condition('serial_no_from_v',$serialFormV);
				$update_status->condition('contractor_particular_id',$particular_id);
				$update_status->condition('contractor_user_id',$contractor_user_id);
				$update_status->execute();
			}*/
			
			
		
			
			$message								= drupal_set_message_custom(t('License Certificate has been successfully generated. Please Sign and Upload Certificate to complete this process'));
 			//$form_state['values']['hidden_message'] = $message;
			$form_state['rebuild']					= TRUE;

		}
	}
}

function get_license_filename($serial, $license_id,$doc_type){
	$uploaded_file = db_select('l_encrypted_uploaded_documents','l');
	$uploaded_file->fields('l',array('filename'));
	$uploaded_file->condition('application_id',$license_id);
	$uploaded_file->condition('form_v',$serial);
	$uploaded_file->condition('document_type_code',$doc_type);
	$uploaded_file->condition('status','1');
	$uploaded_file = $uploaded_file->execute()->fetchAssoc();
	$uploaded_filename = 'contractor_license_'.$doc_type.'_'.$uploaded_file['filename'];
	
	return $uploaded_filename; 
}
	