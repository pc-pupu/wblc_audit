<?php
//drupal_add_css(drupal_get_path('module', 'alc') . '/css/customstyle.css');
drupal_add_js(drupal_get_path('module', 'alc').'/js/mycustom.js');
	
function alc_mtw_renewal_details($form, &$form_state, $id = '',$app_user_id=''){
	
	global $base_root, $base_path, $user;
	//$uri3						= explode('#',$app_user_id);
//	$app_user_id				= $uri3[0];
//	$uriID						= $uri3[1];
//	
	
	$user_id 					= $user->uid;
	$action	 					= 'decrypt';
	$action_encrypt				= 'encrypt';
	$application_id				= encryption_decryption_fun($action, $id); 
	$applicant_user_id			= encryption_decryption_fun($action, $app_user_id);
	$finalfees					= 0;
	$star						= '<span class="form-required" title="This field is required.">*</span>';
	$renewal_status				= 0;
	$amend_status				= 0;
	
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
	
	
	/*** Get Applicant Block Code & All Status ****/
	
	$get_mtw_application							=	db_select('l_mtw_registration_master', 'lmrm');
	$get_mtw_application							->	fields('lmrm',array());
	$get_mtw_application							->	condition('lmrm.mtw_loc_subdivision',$alc_subdivision_code);
	$get_mtw_application							->	condition('lmrm.id',$application_id);
	$get_mtw_application							->	condition('lmrm.user_id',$applicant_user_id);
	$get_mtw_application							->	condition('lmrm.final_submit_status','P');
	$get_mtw_application_result 					= 	$get_mtw_application->execute();

	
	if($get_mtw_application_result-> rowCount() > 0 ){
		
		$applicant_data 							= $get_mtw_application_result->fetchAssoc();
		
		$applicant_subdivsion_code 					= trim($applicant_data['mtw_loc_subdivision']);
		$mtw_registration_number					= $applicant_data['registration_number'];
		$mtw_registration_date						= $applicant_data['registration_date'];
		$mtw_qr_code								= $applicant_data['mtw_qr_code'];
		$applicant_final_submit_status				= $applicant_data['final_submit_status']; 
		$applicant_status							= $applicant_data['status'];
		$uploaded_signed_certificate				= $applicant_data['mtw_registration_certificate'];
		$fees										= $applicant_data['mtw_maxworkers'];
		$renewal_status								= $applicant_data['is_renew'];
		$amend_status								= $applicant_data['is_amend'];
		$backlog_id									= $applicant_data['backlog_id'];
		
		
		if($fees <= 5){
			$retFees	=	'10';
		}else if($fees > 5 && $fees <= 25  ){
			$retFees	=	'25';
		}else if($fees > 25 && $fees <= 50  ){
			$retFees	=	'50';
		}else if($fees > 50 && $fees <= 100  ){
			$retFees	=	'100';
		}else if($fees > 100 && $fees <= 250  ){
			$retFees	=	'250';
		}else if($fees >250 && $fees <= 500){
			$retFees	=	'500';
		}else if($fees > 500 && $fees <= 750  ){
			$retFees	=	'750';
		}else if($fees > 750){
			$retFees	=	'1000';
		}else{
			$retFees	= '0';
		}
		
		$originalFees									= $retFees;
		
		
	}
	
	/**** Get Renewal Details ****/
	$renewalQuery	= db_select('l_mtw_registration_renewal','lmrr');
	$renewalQuery->fields('lmrr',array());
	$renewalQuery->condition('lmrr.application_id',$application_id);
	$renewalQuery->condition('lmrr.status','I','!=');
	$renewalQuery->orderBy('lmrr.id','DESC');
	$renewalExecute	=	$renewalQuery->execute();
	if($renewalExecute->rowCount()>0){
		$renewalResult 					= $renewalExecute->fetchObject();
		$mtw_registration_certificate 	= $renewalResult->mtw_registration_certificate;
		$renewal_date					= $renewalResult->renewal_date;
		$uploaded_signed_certificate	= $renewalResult->mtw_registration_certificate;
		$ref_number						= $renewalResult->renewal_ref_number;	
		$renewal_id						= $renewalResult->id;
	}else{
		$renewalResult 					= array();
		$mtw_registration_certificate 	= '';
		$renewal_date					= '';
		$ref_number						= '';
		
		$renewalQuery2	= db_select('l_mtw_registration_renewal','lmrr');
		$renewalQuery2->fields('lmrr',array());
		$renewalQuery2->condition('lmrr.application_id',$application_id);
		$renewalQuery2->condition('lmrr.status','I','!=');
		$renewalQuery2->orderBy('lmrr.id','DESC');
		$renewalExecute2	=	$renewalQuery2->execute();
		$renewalResult2		= $renewalExecute2->fetchObject();
		$renewal_id			= $renewalResult2->id;
		
	}
	
	//die($uploaded_signed_certificate);
	/*** Get REMARK Details ****/
	
	$get_remark = db_select('l_mtw_remark_master', 'lmrkm');
	$get_remark->fields('lmrkm', array('remark_field_title','remark_text'));
	$get_remark->condition('lmrkm.remark_to', trim($applicant_user_id));
	$get_remark->condition('lmrkm.application_id', trim($application_id));
	$get_remark->condition('lmrkm.remark_by', trim($applicant_user_id), '!=');
	$get_remark->condition('lmrkm.is_active', 1);
	//if($applicant_status=='RN'){
		$get_remark->condition('lmrkm.ref_number', $ref_number);
	//}else{
		//$get_remark->condition('lmrkm.ref_number', $ref_number);
	//}
	$get_remark->orderBy('lmrkm.id','DESC');
	$get_remark->range(0, 1);
	$get_remark_result = $get_remark->execute();
			
	$remark_details = $get_remark_result->fetchAssoc(); 
	
	
	if(empty($remark_details)){
		// issued remark data
		$sqlmrk = "select remark_field_title,remark_text from l_mtw_remark_master where application_id =".trim($application_id)." and remark_to = ".trim($applicant_user_id)." and remark_by != ".trim($applicant_user_id)." and is_active = 1 and remark_type = 'I' " ;
		$chkr = db_query($sqlmrk);
		if($chkr->rowCount() > 0){
			$remark_details = $chkr->fetchAssoc();
		}
		
		//echo '<pre>'; print_r($remark_details); die;
	}
																									
	if($applicant_status == 'B'){
		$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> 
							CURRENT STATUS: Back for rectification</h4><p>Application is sent back for rectification. After modification by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p>
						</div>';
	}
	if($applicant_status == 'BI'){
		$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i>CURRENT STATUS:  Back to inspector for rectification</h4><p>Application is sent back to Inspector for further verification. After verification, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
	}
	if($applicant_status == 'I'){
		
		$certificateDownloadLink = l(t('<i class="fa fa-download" aria-hidden="true"></i>&nbsp; Signed ISMW License Certificate'),$GLOBALS['base_url'].'/sites/default/files/'.$upload_certificates_file_url, array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn')))).'<br/><br/>';
		$current_status = '<div class="callout callout-success"><h4>CURRENT STATUS: Renewal of Registration Certificate is issued. </h4><p>Please check the Registration Certificate after uploading the Certificate.</div>';
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
		$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS:  Form-I uploaded & application succesfully submitted</h4><p>FORM-I is submitted by the Applicant. After verification of uploaded FORM-I, Issue of Registration Certificate can be generated now or back to rectification FORM-I.</p></div>';
	} 
	if($applicant_status == 'U'){
		$current_status = 'CURRENT STATUS: <div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> 
								CURRENT STATUS: Back for Rectification(FORM-I)</h4><p>FORM-V is sent back for rectification. After modification by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p>
							</div>';
	} 
	if($applicant_status == '0'){
		$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Pending application</h4><p>Application is applied by the Applicant. Any action can be taken for the application.</p></div>';
	} 
	if($applicant_status == 'RN'){
		$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Pending application</h4><p>Application is applied by the Applicant. Any action can be taken for the application.</p></div>';
	}
	
	
																							
	if(trim($alc_subdivision_code) == trim($applicant_subdivsion_code) ){   /** Checking for If the Application can be viewed by the ALC of the Subdivision ****/
	
			$form['view_mtw_registration_form_alc']['markup_data']			= array(
				'#type' 		=> 'markup',
				'#markup' 		=> view_mtw_renewal_form_func_alc($application_id,$applicant_user_id)
			);
			
			$form['view_mtw_registration_form_alc']['current_status']		= array(
				'#type' 		=> 'markup',
				'#markup'		=> $current_status
			);																					
			
			
			/*$form['view_mtw_registration_form_alc']['show_status'] 		= array(
				'#prefix'				=> '<div class="comments" align="center">',
				'#suffix'				=> '</div>',
				'#type' 				=> 'markup',
				'#markup'				=> $current_status,
			);*/
			
			if ($applicant_status != 'B' && $applicant_status != 'V' && $applicant_status != 'I' && $applicant_status != 'VA' && $applicant_status != 'U' && $applicant_status != 'T'){
			
			$form['view_mtw_registration_form_alc']['markup_data_1'] = array(
				'#type'   => 'markup',
				'#markup' => '<div class="box box-primary box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i> Actions And Remarks</div><div class="box-body">',
				);
			
			
			$form['view_mtw_registration_form_alc']['hidden_action_message'] = array(
				'#type'					=> 'hidden',
			);	
			
			if($applicant_status == 'I'){
			
				$form['view_mtw_registration_form_alc']['view_uploaded_certificate'] = array(
						'#markup' 				=>	l(t('Uploaded Signed mtw Certificate  <img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/pdf.png" title="Click here to view & print">'),''.$GLOBALS['base_url'].'/sites/default/files/upload/MTW_Registration_Certificate/'.$uploaded_signed_certificate, array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('#pdf-img')))),
				
				);		
			}
		
			
		
			if ($applicant_status != 'B' && $applicant_status != 'V' && $applicant_status != 'I' && $applicant_status != 'VA' && $applicant_status != 'U' && $applicant_status != 'T'){
				
				$form['#attributes'] = array('enctype' => 'multipart/form-data');																				
				
				$form['view_mtw_registration_form_alc']['fieldname']						= array(
						'#title'				=> 'Field Name',
						'#type'					=> 'hidden',
						'#attributes'			=> array('id' => 'mtw_field_name_alc'),
						'#default_value' 		=> !empty($remark_details['remark_field_title']) ? $remark_details['remark_field_title'] : '',
				);
			
				$form['view_mtw_registration_form_alc']['applicant_user_id']				= array(
						'#title'				=> 'User Id',
						'#type'					=> 'hidden',
						'#value' 				=> !empty($applicant_user_id) ? $applicant_user_id : '',
				);
			
				$form['view_mtw_registration_form_alc']['application_id']					= array(
						'#title'				=> 'Application Id',
						'#type'					=> 'hidden',
						'#value' 				=> !empty($application_id) ? $application_id : '',
				);
				$form['view_mtw_registration_form_alc']['ref_number']					= array(
						'#title'				=> 'Reference number for renewal',
						'#type'					=> 'hidden',
						'#value' 				=> $ref_number
				);
				
				
				$form['view_mtw_registration_form_alc']['original_fees']					= array(
						'#title'				=> 'Renewal Fees',
						'#type'					=> 'hidden',
						'#value' 				=> !empty($originalFees) ? $originalFees : 0,
				);
			
								
				if($mtw_registration_number=='' && $backlog_id!='' && $applicant_status!="S"){
						$funcname = actions_dropdown_by_alc_mtw($applicant_status,$backlog_id);
				}else if($applicant_status=="S"){
						$funcname = actions_dropdown_by_alc_mtw("S");
				}else{
						$funcname = actions_dropdown_by_alc_mtw($applicant_status);
				}
			
				
				$form['view_mtw_registration_form_alc']['remark_type']					= array(
						//'#prefix' 				=> '<section><label class="select">',
						//'#suffix' 				=> '<i></i></label></section>',
						'#prefix' 				=> '<div class="col-md-3">&nbsp;</div><div class="form-custom col-md-6"><label class="input">',
						'#suffix' 				=> '<i></i></label>',
						'#title'				=> 'Please Select Actions',
						'#type'					=> 'select',
						'#required' 			=>  TRUE,
						'#options'				=>  $funcname,
						'#attributes'			=> array('class' => array('form-control')),
						'#ajax'					=> array(
							'event' 			=> 'change',
							'effect' 			=> 'fade',			
							'callback'			=> 'get_call_date_time_field',
							'progress'			=> '',
						),
				);
							
				
				
				$form['view_mtw_registration_form_alc']['call_applicant'] 				= array(
						'#prefix' 			=> '<div id="call_applicant_block">',
						'#suffix' 			=> '</div>'
				);
				
				$form['view_mtw_registration_form_alc']['generate_certificate'] 		= array(
						'#prefix' 			=> '<div id="generate_certificate_block">',
						'#suffix' 			=> '</div>'
				);
				
				$form['view_mtw_registration_form_alc']['regenerate_certificate'] 		= array (
						'#prefix' 			=> '<div id="regenerate_certificate_block">',
						'#suffix' 			=> '</div>',
				);																				
				$form['view_mtw_registration_form_alc']['download_link'] 				= array(
						'#prefix' 			=> '<div id="download_link_block">',
						'#suffix' 			=> '</div>'
				);
				
				$form['view_mtw_registration_form_alc']['mtw_signed_certificate'] 		= array(
						'#prefix' 			=> '<div id="mtw_signed_certificate_block">',
						'#suffix' 			=> '</div>'
				);
				
				$form['view_mtw_registration_form_alc']['remarks_text'] 				= array(
						'#prefix' 			=> '<div id="remarks_text_block">',
						'#suffix' 			=> '</div>'
				);
				
				$form['view_mtw_registration_form_alc']['submit'] = array(
						'#prefix' 			=> '<div id="submit_block">',
						'#suffix' 			=> '</div>'
				);
				
				// second time renewal
				$secondq = db_query("select id,mtw_qr_code,expiry_date from l_mtw_registration_renewal where id = ".$renewal_id."");
				$secondr = $secondq->fetchObject();
				
				$value_remark_type = isset($form_state['values']['remark_type']) ? $form_state['values']['remark_type'] : '';
				$requir_field	=	'';
				if($value_remark_type == 'I'){
				
					if($applicant_status =='S'){ 
						if($renewal_date =='' && $mtw_registration_number ==''){
							if($mtw_registration_number =='' && $mtw_registration_date =='' && $backlog_id !=''){
								$genrateFunction = 'generation_mtw_registration_number_backlog';
							}else{
								$genrateFunction = 'generation_mtw_registration_number';
							}
							if($value_remark_type== 'I'){						
								$requir_field		=	TRUE;
							}else{
								$requir_field		=	FALSE;
							}							
							// first time generate
							$form['view_mtw_registration_form_alc']['generate_certificate'] = array (
									'#prefix' 			=> '<div id="generate_certificate_block">',
									'#suffix' 			=> '</div>',
									'#type' 			=> 'submit',
									'#attributes' 		=> array('class' => array('btn btn-primary pull-left acremark-btn')),
									'#value' 			=> 'Generate MTW Registration Certificate',
									'#submit'			=> array($genrateFunction),
							);	
													
						}else if($secondr->mtw_qr_code == ''){
							$form['view_mtw_registration_form_alc']['regenerate_certificate'] 		= array (
										'#prefix' 			=> '<div id="regenerate_certificate_block">',
										'#suffix' 			=> '</div>',
										'#type' 			=> 'submit',
										'#attributes' 		=> array('class' => array('btn btn-primary pull-left acremark-btn')),
										'#value' 			=> 'Regenerate MTW Registration Certificate Renewal',
										'#submit'			=> array('generation_mtw_registration_number'),
								);
							
								$value_check = isset($form_state['values']['regenerate_certificate']) ? $form_state['values']['regenerate_certificate'] : '';
								if(!empty($value_check) && ($value_check=='Regenerate MTW Registration Certificate Renewal')){						
									$requir_field		=	FALSE;
								}else{
									$requir_field		=	FALSE;
								}
						
						}else{
							
							if(!empty($mtw_registration_certificate) || $mtw_registration_certificate ==''){
								
								// upload and submit									
								
								$form['view_mtw_registration_form_alc']['download_link']					= array(
										'#prefix'				=> '<div id="download_link_block"><section>',
										'#markup' 				=> l('<i class="fa fa-download"></i> '.t('Download MTW Registration Certificate'),'mtw-renewcertificate-generate/'.encryption_decryption_fun($action_encrypt, $mtw_qr_code), array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('btn btn-success')))),
										
										
										'#suffix' 				=> '</section></div>',
										
								);
								
								
								if(trim($applicant_status) == 'S' && trim($value_remark_type) =='I'){
									if($value_remark_type== 'I'){						
										$requir_field		=	TRUE;
									}else{
										$requir_field		=	FALSE;
									}						
									$form['view_mtw_registration_form_alc']['mtw_signed_certificate']	= array(
											'#title'				=> 'Upload Signed MTW Registration Certificate',
											'#type' 				=> 'managed_file',
											'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
											'#upload_location' 		=> 'public://upload/MTW_Registration_Certificate/',
											'#process' 				=> array('import_my_file_element_process'),
											'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
											'#prefix'				=> '<div id="mtw_signed_certificate_block"><section>',
											'#suffix'				=> '</section></div>',
											'#required' 			=>  $requir_field,
									);
								}else{
									$form['view_mtw_registration_form_alc']['mtw_signed_certificate']	= array(
											'#title'				=> 'Upload Signed MTW Registration Certificate',
											'#type' 				=> 'managed_file',
											'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
											'#upload_location' 		=> 'public://upload/MTW_Registration_Certificate/',
											'#process' 				=> array('import_my_file_element_process'),
											'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
											'#prefix'				=> '<div id="mtw_signed_certificate_block"><section>',
											'#suffix'				=> '</section></div>',
											'#required' 			=>  $requir_field,
									);
								}
								
								$form['view_mtw_registration_form_alc']['remarks_text']					= array(
										'#prefix' 				=> '<div id="remarks_text_block"><section><label class="input">',
										'#suffix' 				=> '</label></section></div>',
										'#title'				=> 'Remarks',
										'#type'					=> 'textarea',
										'#attributes'			=> array('class' => array('form-control')),
										'#rows'					=>  2,
										'#required' 			=> $requir_field,
										'#default_value'		=> 'MTW Renewal of Registration Certificate Issued'
								);
								
								$form['view_mtw_registration_form_alc']['submit'] 						= array (
										'#prefix' 				=> '<div id="submit_block">',
										'#suffix' 				=> '</div>',	
										'#type' 				=> 'submit',
										'#attributes' 			=> array('class' => array('btn btn-primary pull-left acremark-btn')),
										'#value' 				=> 'Submit'
								);
							}
						}
					}
				}
			
				//$selectedValue															=	isset($form_state['values']['remark_type'])?$form_state['values']['remark_type']:'';
				
				//echo $value_remark_type.'-----'.$applicant_status; die;
			
				if($value_remark_type != 'I'){
					if($value_remark_type		== 'B'){
						$placeholdertxt			= 'Back To Applicant For Correction';
					}elseif($value_remark_type	== 'BI'){
						$placeholdertxt			= 'Back To Inspector';
					}elseif($value_remark_type	=='VA'){
						$placeholdertxt			= 'Verified and Approve Without Fees';
					}elseif($value_remark_type	=='V'){
						$placeholdertxt			= 'Verified and Approve With Fees';
					}elseif($value_remark_type	=='R'){
						$placeholdertxt			= 'Reject Application';
					}elseif($value_remark_type	=='RN'){
						$placeholdertxt			= 'Apply For Renewal';
					}else{
						$placeholdertxt			= '';
					}					
					
					$form['view_mtw_registration_form_alc']['remarks_text']				= array(
						'#prefix' 				=> '<div id="remarks_text_block"><section><label class="input">',	
						'#suffix' 				=> '</label></section></div>',
						
						//'#prefix' 				=> '<div id="remarks_text_block" class="col-md-3">&nbsp;</div><div class="form-custom col-md-6"><label class="input">',
						//'#suffix' 				=> '<i></i></label></div>',
						'#attributes'			=> array('class' => array('form-control')),
						'#title'				=> 'Remarks',
						'#type'					=> 'textarea',
						'#rows'					=>  2,
						'#required' 			=>  $requir_field,																																																	
						'#default_value'		=> $placeholdertxt,
						//'#attributes'			=> array('placeholder'=>$placeholdertxt),																									
					);
					
					$form['view_mtw_registration_form_alc']['submit'] 						= array (
						'#prefix' 				=> '<div id="submit_block">',
						'#suffix' 				=> '</div></div>',	
						'#type' 				=> 'submit',
						'#attributes' 			=> array('class' => array('btn btn-primary pull-left acremark-btn')),
						'#value' 				=> 'Submit'
					);	
				}
			
			}
		
		
		
			$form['view_mtw_registration_form_alc']['markup_data_2']							= array(
				'#type' 				=> 'markup',
				'#markup' 				=> '</div></div>'
			);
		
		}
		
		
		$form['view_mtw_registration_form_alc']['markup_data_5']							= array(
			'#type' 				=> 'markup',
			'#markup' 				=> '
			<div class="box box-primary box-solid">
			<div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">Remark Summary</h3></div>
			<div class="box-body"><div class="feedback-scroll">'.get_mtw_remark_details($application_id,$applicant_user_id,$renewal_id).'</div></div>
			</div>',
		);																							
	}else{
		$message = 'You are not permitted to view this application.';
		drupal_set_message(t($message), 'error');	
	}
	
	return $form;
}

function view_mtw_renewal_form_func_alc($application_id='' ,$applicant_user_id= ''){  
	
	global $base_root, $base_path, $user;
	
	$imgurl = '<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>'; //die($application_id);
	
	if(is_numeric($applicant_user_id) && is_numeric($application_id)){ 
		
		if(!empty($application_id)){
			
			global $base_root, $base_path, $user;
			
			// mtw master table data
			$master_part_mtw_reg = db_select('l_mtw_registration_master', 'lmrm');
			$master_part_mtw_reg->fields('lmrm', array('id','user_id','final_submit_status','status','registration_number','registration_date','mtw_registration_certificate','mtw_name','mtw_nature','total_routes','total_mtw_vehicle','mtw_maxworkers','payment_mode','mtw_route_details','finalfees','expiry_date','backlog_id','mtw_loc_address','act_id'));		
			$master_part_mtw_reg->condition('lmrm.user_id', $applicant_user_id);
			$master_part_mtw_reg->condition('lmrm.id', $application_id);
			$master_part_mtw_reg_result = $master_part_mtw_reg->execute();
			
			if($master_part_mtw_reg_result->rowCount() > 0){
				$content	= 	$master_part_mtw_reg_result->fetchAssoc(); 
				
				//echo '<pre>'; print_r($content); die;
				
				$fees		=	$content['mtw_maxworkers'];
				$status		=	$content['status'];
				$backlog_id	=	$content['backlog_id'];
		
				if($fees <= 5){
					$retFees	=	'10';
				}else if($fees > 5 && $fees <= 25  ){
					$retFees	=	'25';
				}else if($fees > 25 && $fees <= 50  ){
					$retFees	=	'50';
				}else if($fees > 50 && $fees <= 100  ){
					$retFees	=	'100';
				}else if($fees > 100 && $fees <= 250  ){
					$retFees	=	'250';
				}else if($fees >250 && $fees <= 500){
					$retFees	=	'500';
				}else if($fees > 500 && $fees <= 750  ){
					$retFees	=	'750';
				}else if($fees > 750){
					$retFees	=	'1000';
				}else{
					$retFees	= '0';
				}
				$originalFees	= $retFees;
			}else{
				$content	= 	array();
			}
			
			$table			= 'l_mtw_registration_master';
			$type			= 'mtw';
			$id				= $application_id;
			$fieldArr		= array('mtw_loc_dist','mtw_loc_subdivision','mtw_loc_areatype','mtw_loc_areatype_code','mtw_loc_vill_ward','mtw_loc_ps','mtw_loc_pincode');
			$establishment_address 	= get_full_address($table, $type, $id, $fieldArr);
			
			// mtw renewal table data
			$renewal_part_mtw = db_select('l_mtw_registration_renewal','lmrr');
			$renewal_part_mtw->fields('lmrr',array());
			$renewal_part_mtw->condition('lmrr.application_id',trim($application_id));
			$renewal_part_mtw->condition('lmrr.status','I','!=');
			$renewal_part_mtw_fetch	=	$renewal_part_mtw->execute();
			if($renewal_part_mtw_fetch->rowCount()>0){
				$dataArr			=	$renewal_part_mtw_fetch->fetchObject(); 
				$ref_number			=	$dataArr->renewal_ref_number;	
				$renewalId 			=   $dataArr->id;
			}else{
				$ref_number			=	0;
			}
			
					
			/**** Uploaded Documents ****/
			
			$trade_license_file			= '';
			$article_of_assoc_file		= '';
			$form_one_asses_ses_file	= '';
			$supp_asses_ses_file		= '';
			$mtw_address_proof_file		= '';
			$other_doc_file				= '';			
			
			$enc_doc_array = array('TL'=> '','PD'=> '','AOA'=> '','MOA'=> '','FL'=> '','AC'=> '','PC'=> '','BB'	=> '','SC'	=> '','IC'	=> '','AP'=> '','ODSC'=> '','CR'=> '','WO'=> '','ORC'=> '', 'SFI' => '');
	
			$pdficonlink = '<img title="View Documents" alt="View Documents" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/pdf.png">';
			
			// data from encrypted data table
			$query 			= db_select('l_encrypted_uploaded_documents','leud')->fields('leud',array())->condition('status','1')->condition('user_id',trim($applicant_user_id));
			$query_result 	= $query->execute();
			
			if($query_result->rowCount() > 0){
				foreach($query_result->fetchAll() as $data ){
					$enc_doc_array[$data->document_type_code]['content'] = 'view_documents/'.encryption_decryption_fun('encrypt',$data->id);
					$enc_doc_array[$data->document_type_code]['rowid'] = $data->id;
				}
			}
			
				
			$master_part_mtw_docs = db_select('l_documents_upload', 'ldu');
			$master_part_mtw_docs->fields('ldu', array());
			$master_part_mtw_docs->condition('ldu.user_id', trim($applicant_user_id));
			$master_part_mtw_docs->condition('ldu.application_id', trim($application_id));
			$master_part_mtw_docs->condition('ldu.act_id', 3);
			//$master_part_mtw_docs->condition('ldu.identification_number', trim($ref_number));
			
			$master_part_mtw_docs_result = $master_part_mtw_docs->execute();
			
			$global_link = $base_root.$base_path.'sites/default/files/upload/';
			
			if( $master_part_mtw_docs_result->rowCount() > 0 ){
				$content_docs = $master_part_mtw_docs_result->fetchObject();
				if($enc_doc_array['TL'] == ''){
					$enc_doc_array['TL']['content'] = ($content_docs->trade_license_file != '')? $global_link.'trade_license/'.$content_docs->trade_license_file : '';
				}
				if($enc_doc_array['AOA'] == ''){
					$enc_doc_array['AOA']['content'] = ($content_docs->article_of_assoc_file != '')? $global_link.'article_of_assoc/'.$content_docs->article_of_assoc_file : '';
				}
				if($enc_doc_array['PD'] == ''){
					$enc_doc_array['PD']['content'] = ($content_docs->article_of_assoc_file != '')? $global_link.'article_of_assoc/'.$content_docs->article_of_assoc_file : '';
				}
				if($enc_doc_array['BB'] == ''){
					$enc_doc_array['BB']['content'] = ($content_docs->form_one_asses_ses_file != '')? $global_link.'blue_book/'.$content_docs->form_one_asses_ses_file : '';
				}
				if($enc_doc_array['SC'] == ''){
					$enc_doc_array['SC']['content'] = ($content_docs->form_one_asses_ses_file != '')? $global_link.'blue_book/'.$content_docs->form_one_asses_ses_file : '';
				}
				if($enc_doc_array['IC'] == ''){
					$enc_doc_array['IC']['content'] = ($content_docs->supp_asses_ses_file != '')? $global_link.'insurance/'.$content_docs->supp_asses_ses_file : '';
				}
				if($enc_doc_array['AP'] == ''){
					$enc_doc_array['AP']['content'] = ($content_docs->address_proof_file != '')? $global_link.'address_proof/'.$content_docs->address_proof_file : '';
				}
				if($enc_doc_array['ODSC'] == ''){
					$enc_doc_array['ODSC']['content'] = ($content_docs->other_doc_file != '')? $global_link.'OtherRelatedDocuments/'.$content_docs->other_doc_file : '';
				}
			}
			// else{
			// 	$enc_doc_array['TL']['content'] = '';
			// 	$enc_doc_array['AOA']['content'] = '';
			// 	$enc_doc_array['PD']['content'] = '';
			// 	$enc_doc_array['BB']['content'] = '';
			// 	$enc_doc_array['SC']['content'] = '';
			// 	$enc_doc_array['IC']['content'] = '';
			// 	$enc_doc_array['AP']['content'] = '';
			// 	$enc_doc_array['ODSC']['content'] ='';
			// }
						
			//echo '<pre>'; print_r($enc_doc_array); die($applicant_user_id.'----'.$application_id);
			
			
			/***** Uploaded Form - I at renewal *******/
			
			$getQuery 	=	db_query("select identification_number, form_1_signed_pdf_file from l_documents_upload as ldu left join l_mtw_registration_renewal as lmrr on lmrr.renewal_ref_number= ldu.identification_number where lmrr.application_id = ".$application_id." and ldu.application_id=".$application_id." and lmrr.status !='I' and lmrr.purpose='renewal'");
			
			if($getQuery->rowCount()>0){
				$getPrvExe	=	$getQuery->fetchObject();				
			}else{
				$getPrvExe	=	array();
			}
			
			/******* Remarks table data *********/
			$get_remark = db_select('l_mtw_remark_master', 'lmrkm');
			$get_remark->fields('lmrkm', array('id','remark_field_title'));
			$get_remark->condition('lmrkm.remark_to', trim($applicant_user_id));
			$get_remark->condition('lmrkm.application_id', trim($application_id));
			$get_remark->condition('lmrkm.remark_by', trim($applicant_user_id), '!=');
			$get_remark->condition('lmrkm.renewal_id', $renewalId);
			$get_remark->condition('lmrkm.is_active', 1);
			/*if($status=='RN'){
				$get_remark->condition('lmrkm.ref_number', '0');
			}else{
				$get_remark->condition('lmrkm.ref_number', $ref_number);
			}*/			
			$get_remark->orderBy('lmrkm.id','DESC');
			$get_remark->range(0, 1);
			$get_remark_result = $get_remark->execute();
			$content_5 = $get_remark_result->fetchAssoc(); 
			
			
			$remark_field_text_arr = explode(',', $content_5['remark_field_title']);
			
			//echo '<pre>'; print_r($get_remark_result); echo $applicant_user_id.'----'.$application_id.'----'.$ref_number; die;
			

			$mtw_name_ck = '';
			$mtw_loc_address_ck = '';
			$mtw_nature_ck = '';
			$total_routes_ck = '';
			$total_route_milage_ck = '';
			$total_mtw_vehicle_ck = '';
			$mtw_maxworkers_ck = '';
			$mtw_route_details_ck = '';
								
			$trade_license_file_ck = '';	
			$article_of_assoc_file_ck = '';
			$memorandum_of_cert_file_ck = '';	
			$partnership_deed_file_ck = '';
			$challan_file_ck = '';
			$work_order_file_ck = '';	
				
			$form_one_asses_ses_file_ck = '';	
				
			$supp_asses_ses_file_ck = '';
			$other_doc_file_ck = '';	
			$mtw_address_proof_file_ck = '';
			$form_1_mtw_signed_pdf_file_ck = '';
			$old_reg_no_ck = '';
			$old_reg_dt_ck = '';
			$old_reg_expdt_ck = '';
			$old_reg_certificate_ck = '';
			$old_reg_fees_ck = '';
			$form_1_ren_mtw_signed_pdf_file_ck = '';
			$finalfees_ck = '';
			$finefees_ck = '';

			
			
			if(!empty($content_5['remark_field_title'])){
				foreach ($remark_field_text_arr as $fieldname){
					
					$mtw_name_ck =  (in_array('mtw_name',$remark_field_text_arr)) ? "checked='checked'" : '';
					$mtw_loc_address_ck = (in_array('mtw_loc_address',$remark_field_text_arr)) ? "checked='checked'" : '';
					$mtw_nature_ck = (in_array('mtw_nature',$remark_field_text_arr)) ? "checked='checked'" : '';
					
					$total_routes_ck = (in_array('total_routes',$remark_field_text_arr)) ? "checked='checked'" : '';
					$total_route_milage_ck = (in_array('total_route_milage',$remark_field_text_arr)) ? "checked='checked'" : '';
					$total_mtw_vehicle_ck = (in_array('total_mtw_vehicle',$remark_field_text_arr)) ? "checked='checked'" : '';
					$mtw_maxworkers_ck = (in_array('mtw_maxworkers',$remark_field_text_arr)) ? "checked='checked'" : '';
					$mtw_route_details_ck = (in_array('mtw_route_details',$remark_field_text_arr)) ? "checked='checked'" : '';
					$mtw_maxworkers_ck = (in_array('mtw_maxworkers',$remark_field_text_arr)) ? "checked='checked'" : '';
					
					
					$trade_license_file_ck = (in_array('trade_license_file',$remark_field_text_arr)) ? "checked='checked'" : '';
					$article_of_assoc_file_ck = (in_array('article_of_assoc_file',$remark_field_text_arr)) ? "checked='checked'" : '';
					$memorandum_of_cert_file_ck = (in_array('memorandum_of_cert_file',$remark_field_text_arr)) ? "checked='checked'" : '';
					$partnership_deed_file_ck = (in_array('partnership_deed_file',$remark_field_text_arr)) ? "checked='checked'" : '';
					$challan_file_ck = (in_array('challan_file',$remark_field_text_arr)) ? "checked='checked'" : '';
					$work_order_file_ck = (in_array('work_order_file',$remark_field_text_arr)) ? "checked='checked'" : '';
					$form_one_asses_ses_file_ck = (in_array('form_one_asses_ses_file',$remark_field_text_arr)) ? "checked='checked'" : '';
					$supp_asses_ses_file_ck = (in_array('supp_asses_ses_file',$remark_field_text_arr)) ? "checked='checked'" : '';
					$other_doc_file_ck = (in_array('other_doc_file',$remark_field_text_arr)) ? "checked='checked'" : '';
					$mtw_address_proof_file_ck = (in_array('mtw_address_proof_file',$remark_field_text_arr)) ? "checked='checked'" : '';
					$finalfees_ck = (in_array('finalfees',$remark_field_text_arr)) ? "checked='checked'" : '';
					$form_1_mtw_signed_pdf_file_ck = (in_array('form_1_mtw_signed_pdf_file',$remark_field_text_arr)) ? "checked='checked'" : '';
					
					$finefees_ck = (in_array('finefees',$remark_field_text_arr)) ? "checked='checked'" : '';
					$old_reg_no_ck = (in_array('old_reg_no',$remark_field_text_arr)) ? "checked='checked'" : '';
					$old_reg_dt_ck = (in_array('old_reg_dt',$remark_field_text_arr)) ? "checked='checked'" : '';
					
					$old_reg_expdt_ck = (in_array('old_reg_expdt',$remark_field_text_arr)) ? "checked='checked'" : '';
					
					$old_reg_certificate_ck = (in_array('old_reg_certificate',$remark_field_text_arr)) ? "checked='checked'" : '';
					
					$old_reg_fees_ck = (in_array('old_reg_fees',$remark_field_text_arr)) ? "checked='checked'" : '';
					
				}
			}else{
				if($backlog_id=='' || $backlog_id==0){
					$mtw_name_ck = "checked='checked' readonly='readonly' onclick='return false'";
					$mtw_loc_address_ck = "checked='checked' readonly='readonly' onclick='return false'";
					$mtw_nature_ck = "checked='checked' readonly='readonly' onclick='return false'";
					$total_routes_ck = "checked='checked' readonly='readonly' onclick='return false'";
					$total_route_milage_ck = "checked='checked' readonly='readonly' onclick='return false'";
					$total_mtw_vehicle_ck = "checked='checked' readonly='readonly' onclick='return false'";
					$mtw_maxworkers_ck = "checked='checked' readonly='readonly' onclick='return false'";
					$mtw_route_details_ck = "checked='checked' readonly='readonly' onclick='return false'";
										
					$trade_license_file_ck = "checked='checked' readonly='readonly' onclick='return false'";	
					$article_of_assoc_file_ck = "checked='checked' readonly='readonly' onclick='return false'";
					$memorandum_of_cert_file_ck = "checked='checked' readonly='readonly' onclick='return false'";	
					$partnership_deed_file_ck = "checked='checked' readonly='readonly' onclick='return false'";
					$challan_file_ck = "checked='checked' readonly='readonly' onclick='return false'";
					$work_order_file_ck = "checked='checked' readonly='readonly' onclick='return false'";	
						
					$form_one_asses_ses_file_ck = "checked='checked' readonly='readonly' onclick='return false'";	
						
					$supp_asses_ses_file_ck = "checked='checked' readonly='readonly' onclick='return false'";
					$other_doc_file_ck = "checked='checked' readonly='readonly' onclick='return false'";	
					$mtw_address_proof_file_ck = "checked='checked' readonly='readonly' onclick='return false'";
					$form_1_mtw_signed_pdf_file_ck = "checked='checked' readonly='readonly' onclick='return false'";
					
					$owner_id_ck = "checked='checked' readonly='readonly' onclick='return false'";
					
					$finalfees_ck = "checked='checked' readonly='readonly' onclick='return false'";
					$finefees_ck = "";
				} 
			}
			/*************** Fetching Backlog Data ****************/
			$getBacklog					= db_select('l_mtw_registration_backlog','lbmtw');
			$getBacklog					->fields('lbmtw',array());
			$getBacklog					->condition('id',$backlog_id);
			$getBacklogEx				= $getBacklog->execute();
			if($getBacklogEx->rowCount() >0){
				$backlogArray			= 	$getBacklogEx->fetchObject();
			}else{
				$backlogArray			=	array();
			}
			
			
			/*********Fetching Person Data*******/
			// common veriable
			$tablePr					=	'l_mtw_registration_master_extra';
			$fieldArrPr					=	array('person_district','person_subdivision','person_areatype','person_areatype_code','person_vill_ward','person_ps','person_pincode');
			$selectArr					=	array('id','person_name','person_address');
			
			$getAllQuery				=	db_select('l_mtw_registration_master_extra','lmrme');
			$getAllQuery->fields('lmrme',array());
			$getAllQuery->condition('application_id',$application_id);
			$getAllQuery->orderby('id','DESC');
			$getData 					= $getAllQuery->execute();
			
			$personArray				= array();
			if($getData->rowCount()>0){
				$slno =1;
				foreach($getData->fetchAll() as $single){
					$label				= '';
					$designation		= $single->designation;
					
					if(($designation == "proprietor") || ($designation == "partner")) {
						$label								= "Name and Address of the ".ucwords($designation)." in case of firm not registered under the Companies Act,1956";
					}else if($designation =="general_manager"){
						$label								= "Name and Address of the General Manager in the case or a public sector undertaking";
					}else if($designation =="director"){
						$label								= "Name and Address of the Director in case of company registered under the Companies Act,1956";
					}else{
						$label								= "Name and Address of the ".ucwords($designation)." ";
					}
					// verified person data
					
					if(in_array("owner_id_".$single->id,$remark_field_text_arr)){ $owner_id_ck = "checked='checked'"; }else{ $owner_id_ck = ''; }
					
					// person out of country code
					$personCountry	= $single->person_country;
					if($personCountry=='1'){
						$pr_country = 'India';						
					}elseif($personCountry=='2'){
						$pr_country = 'Others';	
					}
					
					
					
					// persona out of wb state code
					$personState	= $single->person_state;
					$getStateQuery	= db_select('state_master','sm')->fields('sm',array())->condition('sm.id',$personState,'=')->execute();
					$getStateData	= $getStateQuery->fetchObject();
					if(!empty($getStateData)){
						$pr_state	= $getStateData->statename.', ';
					}else{
						$pr_state	= '';
					}
					
					
					$personArray[]		= '<tr>
											<td>'.$slno++.'</td>
											<td>'.$single->person_name.'<br>('.$label.')</td>
											<td>'.ucwords($designation).'</td>
											<td>'.$single->person_address.'<br>'.get_full_address($tablePr, $single->designation, $single->id, $fieldArrPr).'<br>'.$pr_state.$pr_country.'</td>
											<td><input type="checkbox" id="owner_id_'.$single->id.'" class="mtw_verified_alc" '. $owner_id_ck .'/></td>
										   </tr>';
				}
			}else{
				$personArray[]		= '<tr><td colspan="4" align="center">Data not available</td></tr>';
			}
		}
		$output = '';
	
		
		$output = '<div class="box box-primary box-solid">
						<div class="box-header with-border"><i class="ion ion-clipboard"></i>Establishment Details<br></div>
						<div class="box-body">
							<table width="100%" border="0" class="table table-bordered table-responsive">
								<tr>
									<td colspan="4"><strong>NOTE:</strong> All inputs are provided by Applicant  . <input type="checkbox" id="check_symbol" class="note check2" checked="checked" /><label class="label2" for="check_symbol"></label> <!--symbol means input has been--> verified&nbsp;&nbsp; <input type="checkbox" id="uncheck_symbol" class="note check2" /><label class="label2" for="uncheck_symbol"></label>&nbsp;&nbsp;<!--symbol means input has been--> not verified</td>
								</tr>
								<tr>
									<th style="width:10%">Sl. No.</th>
									<th style="width:40%">Parameters</th>
									<th style="width:40%">Inputs</th>
									<th style="width:10%">Verified?</th>
								</tr>';
		if( ($master_part_mtw_reg_result->rowCount() > 0) && ($content['registration_number']!='')){ 
					
		$output .= '		<tr>
									<td>#</td>
									<td>Registration Number : '.$content['registration_number'].'<br>Registration Date : '.date('dS M, Y', strtotime($content['registration_date'])).'</td>
									<td>Ref. No. : '.$ref_number.'<br>Expiry Date : '.date('dS M, Y', strtotime($content['expiry_date'])).'</td>
									<td><input type="checkbox" class="mtw_verified_alc" checked="checked" readonly="readonly" /></td>
								</tr>';	
					
		}
		$output .= '			<tr>
									<td>1. </td>
									<td>Name of Motor Transport Undertaking </td>
									<td>'.$content['mtw_name'].'</td>
									<td><input type="checkbox" id="mtw_name" class="mtw_verified_alc"  '.$mtw_name_ck.' /></td>
								</tr>';
		
		$output .= '			<tr>
									<td>2.</td>
									<td>Full Address to which communications relating to the Motor Transport undertaking should be sent  </td>
									<td>'.$content['mtw_loc_address'].'<br/>'.$establishment_address.' </td>
									<td><input type="checkbox" id="mtw_loc_address" class="mtw_verified_alc" '.$mtw_loc_address_ck.' /></td>
								</tr>';
					
		if($content['mtw_nature']=='city_service'){
			$mtw_nature_contrnt		=	'City Service';
		}elseif($content['mtw_nature']=='long_distance'){
			$mtw_nature_contrnt		=	'Long Distance';
		}elseif($content['mtw_nature']=='passenger_service'){
			$mtw_nature_contrnt		=	'Passenger Service';
		}elseif($content['mtw_nature']=='long_distance_freight_service'){
			$mtw_nature_contrnt		=	'Long Distance Freight Service';
		}elseif(!empty($content['mtw_nature']) && $content['mtw_nature']!='city_service' && $content['mtw_nature']!='long_distance' && $content['mtw_nature']!='passenger_service' && $content['mtw_nature']!='long_distance_freight_service'){
			$mtw_nature_contrnt		=	$content['mtw_nature'];
		}
		
			$output .= '		<tr>
									<td>3.</td>
									<td>Nature of Motor transport Service </td>
									<td>'.$mtw_nature_contrnt.'</td>
									<td><input type="checkbox" id="mtw_nature" class="mtw_verified_alc" '.$mtw_nature_ck.' /></td>
								</tr>';
		
		
		$output .= ' 			<tr>
									<td>4.</td>
									<td>Total number of routes</td>
									<td>'.$content['total_routes'].'</td>
									<td><input type="checkbox" id="total_routes" class="mtw_verified_alc"  '.$total_routes_ck.' /></td>
								</tr>';
		if($content['total_routes'] != '' && $content['total_routes'] != 0){
			$total_milage_add 	= 0;
			$total_no		  	= 0;
			$routeDetails1	=	db_select('l_mtw_registration_extended','mtwe');
			$routeDetails1->fields('mtwe',array());
			$routeDetails1->condition('mtwe.application_id',$application_id);
			$fetdata = $routeDetails1->execute();
			
			
			$allDetails 	=	$fetdata->fetchAll();
			if(!empty($allDetails)){
				foreach($allDetails as $datum1){
					$total_no++;
					$total_milage_add =  $total_milage_add+$datum1->milage;
				}
			}
			$output2 = $total_no.'-'.$total_milage_add.'km';
		}else{
			$output2 = 'N/A';
		}
			$output .= '		<tr>
									<td>5.</td>
									<td>Route Details - Milage</td>
									<td>'.$output2.'</td>
									<td><input type="checkbox" id="total_route_milage" class="mtw_verified_alc" '.$total_route_milage_ck.' /></td>
								</tr>';		
		$output .= '			<tr>
									<td>6.</td>
									<td>Total number of motor transport vehicles on the last date or the preceeding year </td>
									<td>'.$content['total_mtw_vehicle'].'</td>
									<td><input type="checkbox" id="total_mtw_vehicle" class="mtw_verified_alc" '.$total_mtw_vehicle_ck.' /></td>
								</tr>';
		
		$output .= '			<tr>
									<td>7.</td>
									<td>Maximum number of motor transport workers employed on any day during the preceeding year </td>
									<td>'.$content['mtw_maxworkers'].'<div class="btn btn-info pull-right" data-toggle="modal" data-target="#myModalFeeschat"><i class="fa fa-info-circle"></i> Fees Chart</div></td>
									<td><input type="checkbox" id="mtw_maxworkers" class="mtw_verified_alc" '.$mtw_maxworkers_ck.' /></td>
								</tr>';
		
		$output .= '		</table>
						</div>
					</div>';
		
		
		$output	.= '<div class="modal fade" id="myModalFeeschat" role="dialog">
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
											<th># Number of Workers</th>
											<th>Fees</th>
										 </tr>
										 <tr>
											 <td>1</td>
											 <td>Does not exceed less or equal to 5</td>
											 <td>₹10</td>
										 </tr>
										 <tr>
											 <td>2</td>
											 <td>Exceeds 5 but does not exceed 25</td>
											 <td>₹25</td>
										 </tr>
										 <tr>
											 <td>3</td>
											 <td>Exceeds 25 but does not exceed 50</td>
											 <td>₹50</td>
										 </tr>
										 <tr>
											 <td>4</td>
											 <td>Exceeds 50 but does not exceed 100</td>
											 <td>₹100</td>
										 </tr>
										 <tr>
											 <td>5</td>
											 <td>Exceeds 100 but does not exceed 250</td>
											 <td>₹250</td>
										 </tr>
										 <tr>
											 <td>6</td>
											 <td>Exceeds 250 but does not exceed 500</td>
											 <td>₹500</td>
										 </tr>
										 <tr>
											 <td>7</td>
											 <td>Exceeds 500 but does not exceed 750</td>
											 <td>₹750</td>
										 </tr>
										 <tr>
											 <td>8</td>
											 <td>Exceeds 750 but does not exceed 999</td>
											 <td>₹1000</td>
										 </tr>
										 <tr>
											 <td>9</td>
											 <td>Exceeds 1000 </td>
											 <td>₹1000</td>
										 </tr>
										 
									  </table>
									</div>
								<div class="modal-footer">
									<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
								</div>
							</div>      
						</div>
					</div>';
		
		
		
		$output .= '<div class="box box-primary box-solid">
						<div class="box-header with-border"><i class="ion ion-clipboard"></i>  Previous(Offline) Registration Certificate Details </div>
						<div class="box-body">';		
     	
		if(($content['backlog_id']!='' && $content['backlog_id']!=0 && !empty($backlogArray))/* && $content['registration_number']==''*/){
		// get backlog data
				
			$output .= '	<table width="100%" border="0" class="table table-bordered table-responsive">
								<tr>
									<th style="width:10%">Sl</th>
									<th style="width:40%">Parameters</th>
									<th style="width:40%">Inputs</th>
									<th style="width:10%">Verified?</th>
								</tr>
								<tr>
									<td>(i)</td>
									<td>Registration Number</td>
									<td>'.$backlogArray->registration_number.'</td>
									<td><input type="checkbox" id="old_reg_no" class="mtw_verified_alc"  '.$old_reg_no_ck.' /></td>
								</tr>
								<tr>
									<td>(ii)</td>
									<td>Registration Date</td>
									<td>'.date('dS M, Y', strtotime($backlogArray->registration_date)).'</td>
									<td><input type="checkbox" id="old_reg_dt" class="mtw_verified_alc"  '.$old_reg_dt_ck.' /></td>
								</tr>
								<tr>
									<td>(iii)</td>
									<td>Expiry Date</td>
									<td>'.date('dS M, Y', strtotime($backlogArray->expiry_date)).'</td>
									<td><input type="checkbox" id="old_reg_expdt" class="mtw_verified_alc"  '.$old_reg_expdt_ck.' /></td>
								</tr>
								<tr>
									<td>(iv)</td>
									<td>Registration Certificate</td>
									
									<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/MTW-Backlog-RC/'.$backlogArray->registration_certificate.'">
										<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png" title="Click here to view & print"></a>&nbsp;&nbsp;&nbsp;
										<span class="viewfile_popup" id="old_reg_certificate">'.$imgurl.'</span>
										<div id="view_old_reg_certificate"  title="Old Registration Certificate" style="display:none;">
											<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/MTW-Backlog-RC/'.$backlogArray->registration_certificate.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
										</div>
									</td>
									<td><input type="checkbox" id="old_reg_certificate" class="mtw_verified_alc"  '.$old_reg_certificate_ck.' /></td>
								</tr>
								<tr>
									<td>(v)</td>
									<td>Registration Fees</td>
									<td><b> &#8377;'.$originalFees.'(Rupees '.convertNumberToWord($backlogArray->paidfees).')</b></td>
									<td><input type="checkbox" id="old_reg_fees" class="mtw_verified_alc"  '.$old_reg_fees_ck.' /></td>
								</tr>
								</table>
								</td>
							</tr>
							</table>
						
							';
		}else{
			$output .= 'Information not available';	
		}
		
		$output .= '	</div>
					</div>';
		
		
		
		$output .= '<div class="box box-primary box-solid">
						<div class="box-header with-border"><i class="ion ion-clipboard"></i> Ownership Details </div>
						<div class="box-body">
							<table width="100%" border="0" class="table table-bordered table-responsive">
								<tr>
									<th>Sl. No.</th>
									<th width="40%">Name</th>
									<th>Designation</th>
									<th>Residential</th>
									<th width="10%">Verified?</th>
								</tr>';			
		$output .='				<tr>';
		if(!empty($personArray)){
			foreach($personArray as $row){
				$output 			.=$row;
			}
		}								
		
		$output .='				</tr>
							</table>
						</div>
					</div>';
					
		$output .= '<div class="box box-primary box-solid">
						<div class="box-header with-border"><i class="ion ion-clipboard"></i> Uploaded Documents </div>
						<div class="box-body">
							<table width="100%" border="0" class="table table-bordered table-responsive">
								<tr>
									<th>Sl. No.</th>
									<th width="34%">Parameters</th>
									<th>Inputs</th>
									<th width="10%">Verified?</th>
								</tr>';	
		
		
		
		$viewlink1 = 'No Document Uploaded';
		if($enc_doc_array['TL']['content'] !=''){
			$viewlink1 = l($pdficonlink,$enc_doc_array['TL']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}
		
		$output .= 	'<tr>
							<td>(i)</td>
							<td>Trade License</td>
							<td>'.$viewlink1.'</td>
							<td><input type="checkbox" id="trade_license_file" class="mtw_verified_alc" '.$trade_license_file_ck.' /></td>		
						</tr>';
		
		/*if(!empty($content_docs['trade_license_file'])){
								 
			$output .= 			'<tr>
									<td>(i)</td>
									<td>Trade License</td>
									<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/trade_license/'.$content_docs['trade_license_file'].'">
										<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png" title="Click here to view & print"></a>&nbsp;&nbsp;&nbsp;
										<span class="viewfile_popup" id="trade_license">'.$imgurl.'</span>
										<div id="view_trade_license"  title="Trade License" style="display:none;">
										<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/trade_license/'.$content_docs['trade_license_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
										</div>
									</td>
									<td><input type="checkbox" id="trade_license_file" class="mtw_verified_alc" '.$trade_license_file_ck.' /></td>
								</tr>';
			   
		}else {
			
			$output .=  '<tr><td>(i)</td><td>Trade License</td><td>No Document Uploaded</td><td><input type="checkbox" id="trade_license_file" class="mtw_verified_alc" '.$trade_license_file_ck.' /></td></tr>';
			
		}*/
		
		$viewlink2 = 'No Document Uploaded';
		if($enc_doc_array['AOA']['content'] !=''){
			$viewlink2 = l($pdficonlink,$enc_doc_array['AOA']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}else if($enc_doc_array['PD']['content'] !=''){
			$viewlink2 = l($pdficonlink,$enc_doc_array['PD']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}
		
		$output .= 	'<tr>
							<td>(ii)</td>
							<td>Articles of Association / Partnership Deed</td>
							<td>'.$viewlink2.'</td>
							<td><input type="checkbox" id="article_of_assoc_file" class="mtw_verified_alc"'.$article_of_assoc_file_ck.' /></td>	
						</tr>';
		
		
		
		/*if(!empty($content_docs['article_of_assoc_file'])){ 
						
				$output .= '<tr>
								<td>(ii)</td>
								<td>Articles of Association / Partnership Deed</td>
								<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/article_of_assoc/'.$content_docs['article_of_assoc_file'].'">
									<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png" title="Click here to view & print"></a>&nbsp;&nbsp;&nbsp;
									<span class="viewfile_popup" id="article_of_assoc_file_1">'.$imgurl.'</span>
									<div id="view_article_of_assoc_file_1"  title="Articles of Association and Memorandum of Association/Partnership Deed"  style="display:none;">
									<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/article_of_assoc/'.$content_docs['article_of_assoc_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
									</div>
								</td>
								<td><input type="checkbox" id="article_of_assoc_file" class="mtw_verified_alc" '.$article_of_assoc_file_ck.' /></td>
							</tr>';
						
		}else{
			
				$output .=  '<tr><td>(ii)</td><td>Articles of Association / Partnership Deed</td><td>No Document Uploaded</td><td><input type="checkbox" id="article_of_assoc_file" class="mtw_verified_alc" '.$article_of_assoc_file_ck.' /></td></tr>';
				
		}*/
		
		$viewlink3 = 'No Document Uploaded';
		if($enc_doc_array['BB']['content'] !=''){
			$viewlink3 = l($pdficonlink,$enc_doc_array['BB']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}else if($enc_doc_array['SC']['content'] !=''){
			$viewlink3 = l($pdficonlink,$enc_doc_array['SC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}
		
		$output .= 	'<tr>
							<td>(iii)</td>
							<td>Blue Book/ Smart Card Issued by Motor Vehicles</td>
							<td>'.$viewlink3.'</td>
							<td><input type="checkbox" id="form_one_asses_ses_file" class="mtw_verified_alc" '.$form_one_asses_ses_file_ck.' /></td>
						</tr>';
		
		/*
		if(!empty($content_docs['form_one_asses_ses_file'])){
							  
				$output .= '<tr>
								<td>(iii)</td>
								<td>Blue Book/ Smart Card Issued by Motor Vehicles</td>
								<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/blue_book/'.$content_docs['form_one_asses_ses_file'].'">
								<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png" title="Click here to view & print"></a>&nbsp;&nbsp;&nbsp;
								<span class="viewfile_popup" id="form_one_asses_ses_file">'.$imgurl.'</span>
								<div id="view_form_one_asses_ses_file"  title="Form I for assesment of SES"  style="display:none;">
								<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/blue_book/'.$content_docs['form_one_asses_ses_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
								</div></td>
								<td><input type="checkbox" id="form_one_asses_ses_file" class="mtw_verified_alc" '.$form_one_asses_ses_file_ck.' /></td>
							
							</tr>';
		}else{
			
			$output .=  '<tr><td>(iii)</td><td>Blue Book/ Smart Card Issued by Motor Vehicles</td><td>No Document Uploaded</td></td><td><input type="checkbox" id="form_one_asses_ses_file" class="mtw_verified_alc" '.$form_one_asses_ses_file_ck.' /></td></tr>';
		}
		*/
		$viewlink4 = 'No Document Uploaded';
		if($enc_doc_array['IC']['content'] !=''){
			$viewlink4 = l($pdficonlink,$enc_doc_array['IC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}
		
		$output .= 	'<tr>
							<td>(iv)</td>
							<td>Insurance Certificate of Motor Vehicles</td>
							<td>'.$viewlink4.'</td>
							<td><input type="checkbox" id="supp_asses_ses_file" class="mtw_verified_alc" '.$supp_asses_ses_file_ck.' /></td>
						</tr>';
		
		/*
		if(!empty($content_docs['supp_asses_ses_file'])){
							  
				$output .= '<tr>
								<td>(iv)</td>
								<td>Insurance Certificate of Motor Vehicles</td>
								<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/insurance/'.$content_docs['supp_asses_ses_file'].'">
								<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png" title="Click here to view & print" ></a>&nbsp;&nbsp;&nbsp;
								<span class="viewfile_popup" id="supp_asses_ses_file">'.$imgurl.'</span>
								<div id="view_supp_asses_ses_file"  title="Documents in Support of Payment of SES"  style="display:none;">
								<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/insurance/'.$content_docs['supp_asses_ses_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
								</div></td>
								<td><input type="checkbox" id="supp_asses_ses_file" class="mtw_verified_alc" '.$supp_asses_ses_file_ck.' /></td>
							</tr>';
		}else{
			
			$output .=  '<tr><td>(iv)</td><td>Insurance Certificate of Motor Vehicles</td><td>No Document Uploaded</td><td><input type="checkbox" id="supp_asses_ses_file" class="mtw_verified_alc" '.$supp_asses_ses_file_ck.' /></td></tr>';
		}
		*/
		
		$viewlink5 = 'No Document Uploaded';
		if($enc_doc_array['ODSC']['content'] !=''){
			$viewlink5 = l($pdficonlink,$enc_doc_array['ODSC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}
		
		$output .= 	'<tr>
							<td>(v)</td>
							<td>Documents in support of correctness of the application</td>
							<td>'.$viewlink5.'</td>
							<td><input type="checkbox" id="other_doc_file" class="mtw_verified_alc" '.$other_doc_file_ck.' /></td>
						</tr>';
						
		/*
		
		if(!empty($content_docs['other_doc_file'])){
							  
				$output .= '<tr>
								<td>(v)</td>
								<td>Documents in support of correctness of the application</td>
								<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/OtherRelatedDocuments/'.$content_docs['other_doc_file'].'">
								<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png" title="Click here to view & print"></a>&nbsp;&nbsp;&nbsp;
								<span class="viewfile_popup" id="other_doc_file">'.$imgurl.'</span>
								<div id="view_other_doc_file"  title="Documents in Support of Correctness of  Application"  style="display:none;">
								<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/OtherRelatedDocuments/'.$content_docs['other_doc_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
								</div></td>
								<td><input type="checkbox" id="other_doc_file" class="mtw_verified_alc" '.$other_doc_file_ck.' /></td>
							</tr>';
		}else{
			
			$output .=  '<tr><td>(v)</td><td>Documents in support of correctness of the application</td><td>No Document Uploaded</td><td><input type="checkbox" id="other_doc_file" class="mtw_verified_alc" '.$other_doc_file_ck.' /></td></tr>';
		}
		*/
		$viewlink6 = 'No Document Uploaded';
		if($enc_doc_array['AP']['content'] !=''){
			$viewlink6 = l($pdficonlink,$enc_doc_array['AP']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}
		
		$output .= 	'<tr>
							<td>(vi)</td>
							<td>Address Proof</td>
							<td>'.$viewlink6.'</td>
							<td><input type="checkbox" id="mtw_address_proof_file" class="mtw_verified_alc" '.$mtw_address_proof_file_ck.' /></td>
						</tr>';
		
		/*
		
		if(!empty($content_docs['address_proof_file'])){
							  
				$output .= '<tr>
								<td>(vi)</td>
								<td>Address Proof</td>
								<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/address_proof/'.$content_docs['address_proof_file'].'">
								<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png" title="Click here to view & print"></a>&nbsp;&nbsp;&nbsp;
								<span class="viewfile_popup" id="mtw_address_proof_file_id">'.$imgurl.'</span>
								<div id="view_mtw_address_proof_file_id"  title="Documents in Support of Address Proof"  style="display:none;">
								<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/address_proof/'.$content_docs['address_proof_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
								</div></td>
								<td><input type="checkbox" id="mtw_address_proof_file" class="mtw_verified_alc" '.$mtw_address_proof_file_ck.' /></td>
							</tr>';
		}else{
			
			$output .=  '<tr><td>(vi)</td><td>Address Proof</td><td>No Document Uploaded</td><td><input type="checkbox" id="mtw_address_proof_file" class="mtw_verified_alc" '.$mtw_address_proof_file_ck.' /></td></tr>';
		}
		*/
		
		//print_r($content_docs);die('jjj');
		
		
		if(!empty($content['mtw_registration_certificate'])){
							  
				$output .= '<tr>
								<td>(vii)</td>
								<td>Old Registration (Registration Certificate) </td>
								<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/MTW_Registration_Certificate/'.$content['mtw_registration_certificate'].'">
								<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png" title="Click here to view & print"></a>&nbsp;&nbsp;&nbsp;
								<span class="viewfile_popup" id="form_1_file">'.$imgurl.'</span>
								<div id="view_form_1_file"  title="Form - I Application"  style="display:none;">
								<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/MTW_Registration_Certificate/'.$content['mtw_registration_certificate'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
								</div></td>
								<td><input type="checkbox" id="form_1_mtw_signed_pdf_file" class="mtw_verified_alc" '.$form_1_mtw_signed_pdf_file_ck.' /></td>
							</tr>';
		}else{
			
			$output .=  '<tr><td>(vii)</td><td>Old Registration (Registration Certificate) </td><td>No Document Uploaded</td><td><input type="checkbox" id="form_1_mtw_signed_pdf_file" class="mtw_verified_alc" '.$form_1_mtw_signed_pdf_file_ck.' /></td></tr>';
		}
		
		
		
		if(!empty($getPrvExe) || $getPrvExe!=''){
			if(!empty($getPrvExe->form_1_signed_pdf_file)){
			  
				$output .= '<tr>
								<td>(viii)</td>
								<td>Form-I (Renewal Application)</td>
								<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/mtw_Form-I/'.$getPrvExe->form_1_signed_pdf_file.'">
								<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png" title="Click here to view & print">
								</a>&nbsp;
								<span class="viewfile_popup" id="form_1_file"> '.$imgurl.'</span>
								<div id="view_form_1_file"  title="Form - I Application"  style="display:none;">
								<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/mtw_Form-I/'.$getPrvExe->form_1_signed_pdf_file.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
								</div>&nbsp;&nbsp;&nbsp; '.l(t('Generated FORM - I'),'mtw-renewgenerate-pdf/'.encryption_decryption_fun('encrypt', $content['id']).'/'.encryption_decryption_fun('encrypt', $content['user_id']), array('html' => TRUE,'attributes'=> array('target'=>'_blank'))).'</td>
								<td><input type="checkbox" id="form_1_mtw_signed_pdf_file" class="mtw_verified_alc" '.$form_1_ren_mtw_signed_pdf_file_ck.' /></td>
							</tr>';
								
			}else if($enc_doc_array['SFI']['content'] !=''){
				$output .= '<tr><td>(viii)</td><td>Form-I (Renewal Application)</td><td>'.l($pdficonlink,$enc_doc_array['SFI']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td><td><input type="checkbox" id="form_1_mtw_signed_pdf_file" class="mtw_verified_alc" '.$form_1_ren_mtw_signed_pdf_file_ck.' /></td></tr>';
			}else{	
				$output .=  '<tr><td>(viii)</td><td>Form-I (Renewal Application)</td><td>No Document Uploaded</td><td><input type="checkbox" id="form_1_mtw_signed_pdf_file" class="mtw_verified_alc" '.$form_1_ren_mtw_signed_pdf_file_ck.' /></td></tr>';
			}
		}	
					
		$output .= '		</table>
						</div>
					</div>';			
	/* Payment Details Start */
	$bank_code = '';
	$payment_status = '';
	$grn_number = '';
	$bankTransactionID = '';
	$total_amount = '';
	$challan_fid_date = '';
					
	if(trim($content['status'])== 'T' || trim($content['status'])== 'S' || trim($content['status'])== 'I'){						
		// $renwal_tdata = db_select('l_mtw_registration_renewal', 'mr');
		// $renwal_tdata->fields('mr', array('id'));
		// $renwal_tdata->condition('mr.application_id', $content['id']);
		// //$renwal_tdata->condition('mr.status', 'T');	
		// $renwal_tdata = $renwal_tdata->execute()->fetchObject(); 
		
						
		$transaction_details = db_select('l_principle_epayments_receive_data', 'lpd');
		$transaction_details->leftJoin('l_principle_epayments_data', 'lped', 'lpd.transaction_id=lped.identification_no');
		$transaction_details->fields('lped', array('identification_no', 'application_id'));
		$transaction_details->fields('lpd', array());
		$transaction_details->condition('lped.act_id', '3');
		$transaction_details->condition('lped.application_id', $renewalId);		
		$trans_details_result = $transaction_details->execute();
		
		if($trans_details_result-> rowCount() > 0 ){
			
			$transaction_det	= $trans_details_result->fetchAssoc();
			$bankTransactionID	= $transaction_det['transaction_id'];
			$grn_number			= $transaction_det['challanrefid'];
			$challan_fid_date	= !empty($transaction_det['challanrefid_date']) ? $transaction_det['challanrefid_date'] : '';
			$total_amount		= number_format($transaction_det['challanamount'], 2);
			$bank_code			= $transaction_det['bank_cd'];
			$payment_status		= $transaction_det['banktransactionstatus'];	
		}
	 }
	 
	 $payment_details = '<u>Grips Payment[Online/Counter]</u><br>';
	 $payment_details .= 'IFSC Code: <span class="color_orange">'.$bank_code.'</span><br>';
	 $payment_details .= 'GRN Number: <span class="color_orange">'.$grn_number.'</span><br>';
	 $payment_details .= 'Bank Transaction Id: <span class="color_orange">'.$bankTransactionID.'</span><br>';
	 $payment_details .= 'Total Amount: <span class="color_orange">'.$total_amount.'</span><br>';
	 $payment_details .= 'Transaction Date: <span class="color_orange">'.$challan_fid_date.'</span><br>';
	 $payment_details .= 'Transaction Status: '.$payment_status;
	 
	/* Payment Details End */										
		
		
			
	$output .= '<div class="box box-primary box-solid">
						<div class="box-header with-border"><i class="ion ion-clipboard"></i>  Payment Details </div>
						<div class="box-body">
							<table width="100%" border="0" class="table table-bordered table-responsive">';								
	
	$output .= '			<tr>
								<td> Amount fees</td>
								<td colspan="2"><b> &#8377;'.$originalFees.'(Rupees '.convertNumberToWord($originalFees).')</b></td>
								<td width="10%"><input type="checkbox" id="finalfees" class="mtw_verified_alc" '.$finalfees_ck.'  /></td>
							</tr>
							<tr>
								<td colspan="3" style="color:#ff0000;"><strong>Note:-</strong>Provided further that in cases where the Chief Inspector or an Inspector duly authorised by him in this behalf is satisfied that the delay in making the application was due to circumstances over which the employer had no control, he may reduce or remit, as he thanks fit, such excess fee <span style="color:#ff0000; font-weight:700;">(Must Checked if you want to add fine)</span> </td>
								<td><input type="checkbox" id="finefees" name="fine_field" class="mtw_verified_alc" '.$finefees_ck.'/></td>
							</tr>
							<tr>
								<td>Payment Details</td>
								<td colspan="3">'.$payment_details.'</td>  	
							</tr>
					';
	
						
	$output .= '		</table>
						</div>
					</div>';					
					
					
				$action	 					= 'encrypt';
		$application_id_pre			= encryption_decryption_fun($action, $application_id);
		$applicant_user_id_pre		= encryption_decryption_fun($action, $applicant_user_id);
		$act_id						= encryption_decryption_fun($action,$content['act_id']);
		
		
		
		$output .= '
		<ul class="row" style="padding:0;">
			<li class="col-md-2 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-arrow-left"></i> Back to list</div>','alc-mtw-renewal-list',array('attributes' =>array('class' => array('backapplication_view'), 'title' => 'Click here back to applications list'), 'html' => TRUE)).'</li>
			
			<li class="col-md-3 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-user"></i> View Applicant Profile</div>','view-applicant-profile/'.$application_id_pre.'/'.$applicant_user_id_pre.'/'.$act_id,array('attributes' =>array('class' => array('backapplication_view'), 'title' => 'Click here back to applications list'), 'html' => TRUE)).'</li>
			
			<li class="col-md-3 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-info-circle"></i> Instructions to give remarks</div>',$base_root.$base_path.'sites/default/files/instructions-remarks.pdf',array('attributes' =>array('class' => array('backapplication_view'), 'title' => 'Click here back to applications list'), 'html' => TRUE)).'</li>
			
			<li class="col-md-4 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-question-circle"></i> How to digitally sign documents using USB Token</div>','digitally-sign-process',array('attributes' =>array('class' => array('backapplication_view'), 'title' => 'Click here back to applications list'), 'html' => TRUE)).'</li>
		</ul>';
			
			   
		return $output;
		
		
	}else{
		
		drupal_goto('node/3');	
	}
}
/*function REGISTRATION_TEXTBOX_TEXT_VALIDATE($element,&$form_state){
	$txtValue		=	trim($element['#value']); 
	$match_pattern 	= 	'/^[a-zA-Z0-9. ]+$/';
	if(!preg_match($match_pattern, $txtValue)) {
		form_error($element, t($element['#title'].' should be text only'));
	}
}*/

function alc_mtw_renewal_details_submit($form,&$form_state,$id='',$app_user_id=''){
	
	global $base_root, $base_path, $user;
	
	$alc_user_id 		= $user->uid;
	
	$val 	= $form_state['values']; 
	$fineVal = '';
	if(isset($_POST['fine_field'])){
		$fineVal			= $_POST['fine_field'];
	}
	$original_fees		= $val['original_fees'];
	
	//print_r($val);die('ppp');
	
	/* Upload For mtw Signed Certificate*/
	
	if(isset($val['mtw_signed_certificate'])){
		$arr_1					= array();
		$mtw_signed_certificate = file_load($val['mtw_signed_certificate']);
		if( $mtw_signed_certificate != "" ){
			$arr_1 = explode("/", $mtw_signed_certificate->uri);
			$mtw_signed_certificate->status = FILE_STATUS_PERMANENT;
			file_save($mtw_signed_certificate);
			$mtw_signed_certificate_file = $arr_1[4];
		}
	}
	
	$applicant_user_id 	= $val['applicant_user_id'];
	$application_id 	= $val['application_id'];
	$fieldname			= $val['fieldname'];
	$remark_type		= $val['remark_type'];
	$remarks_text		= $val['remarks_text'];
	//$call_date_time		= $val['call_applicant'];
	
	/*** Get User role of ALC **/
	
	$fetch_users_role		= db_select('users_roles', 'ur');
	$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role->leftJoin('l_custom_user_detail', 'lcud', 'ur.uid=lcud.usr_id');
	$fetch_users_role->fields('ro', array('rid'));
	$fetch_users_role->fields('lcud', array('fullname','employee_id'));
	$fetch_users_role->condition('ur.uid', $alc_user_id);
	$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc();

	$remark_by_role			= $fetch_users_role_data['rid'];
	$alc_full_name			= $fetch_users_role_data['fullname'];
	$employee_id			= $fetch_users_role_data['employee_id'];
	
	
	
	/**** final fees *****/
	$getPrvValue			= 	db_select('l_mtw_registration_renewal','lmrr');
	$getPrvValue->fields('lmrr',array('previous_fees','renewal_ref_number','id'));
	$getPrvValue->condition('lmrr.application_id',$application_id);
	$getPrvValue->condition('lmrr.status','I','!=');
	$getPrvData	= $getPrvValue->execute();
	if($getPrvData->rowCount() > 0){
		$prvdata			=	$getPrvData->fetchObject();
		$finalPaidAmount 	= 	$prvdata->previous_fees;
		$renewal_ref_number	=	$prvdata->renewal_ref_number;
		$renewal_id			=	$prvdata->id;
	}else{
		$finalPaidAmount 	=	0;
		$renewal_ref_number	=	0;
		$renewal_id			=	0;
	}
		
	/*** fine calculation ***/
	if(!empty($fineVal) || $fineVal!=''){
		$currentAmount = $original_fees;
		$fine_amount = ($original_fees*25)/100;
		$finalAmount = ceil($original_fees+$fine_amount);
		$fine_fees = $fine_amount;
		$paid_fees = ceil($original_fees+$fine_amount);
	}else{
		$currentAmount = $original_fees;
		$fine_amount = 0;
		$finalAmount = $original_fees;
		$fine_fees = 0;
		$paid_fees = $original_fees;
	}
	
	//echo $currentAmount.'--'.$fine_amount.'---'.$finalAmount.'-----'.$fine_fees.'-----'.$paid_fees;
	//die;
	
	$master_exp = '';
	// check exp date null or not null
	$chkrnw = db_query("select expiry_date, renewal_date from l_mtw_registration_renewal where id = ".$renewal_id." and application_id = ".$application_id."");
	if($chkrnw->rowCount() > 0){
		$data = $chkrnw->fetchObject();
		$expiry_date = $data->expiry_date;
		
		if($expiry_date == ''){
		/********************************** Check Previous Exp Date ************************************/
			$reg_masterq	= db_select('l_mtw_registration_master','lmrm')->fields('lmrm',array('backlog_id','expiry_date'))->condition('lmrm.id',$application_id)->execute();
			if($reg_masterq->rowCount() > 0){	
				$reg_masterr	= $reg_masterq->fetchObject();
				$expiry_date 	= $reg_masterr->expiry_date;
				if($expiry_date == '' || $expiry_date == NULL){
					$sql = "select expiry_date from l_mtw_registration_backlog where id = ".$reg_masterr->backlog_id."";
					$query = db_query($sql);
					$result = $query->fetchObject();
					if($result->expiry_date !='' ){
						$date = strtotime($result->expiry_date); 
						$new_date = strtotime('+ 1 year', $date); 
						$expiry_date_show = date('dS M, Y', $new_date);
						$expiry_date = date('Y-m-d', $new_date);
						$master_exp = $expiry_date;
					}
				}else{
					$sql = "select id,expiry_date from l_mtw_registration_renewal where id = (select max(id) from l_mtw_registration_renewal where application_id = ".$application_id." )";
					$query = db_query($sql);
					$result = $query->fetchObject();
					if($result->expiry_date !='' ){
						$date = strtotime($result->expiry_date); 
						$new_date = strtotime('+ 1 year', $date); 
						$expiry_date_show = date('dS M, Y', $new_date);
						$expiry_date = date('Y-m-d', $new_date);
					}
				}
			}
		}
	}
	
	/*** Update Renewal Table ***/
	
	if($remark_type=='B'){
		$update_arr	= array('status' => '1');
		db_update('l_mtw_registration_renewal')->fields($update_arr)->condition('application_id',$application_id)->condition('status','1')->execute();
		$master_array	= array('finalfees' => $finalAmount, 'status' => $remark_type);
	}else if($remark_type=='V'){
		$update_arr	= array('current_fees' => $currentAmount, 'final_fees' => $finalAmount, 'paid_fees' => $paid_fees, 'fine_fees' => $fine_fees, 'status' => $remark_type);
		$master_array	= array('finalfees' => $paid_fees, 'status' => $remark_type);
		
		
		if($result->expiry_date == ''){
			$update_arr['expiry_date'] = $expiry_date;
		}
		if($master_exp == ''){
			$master_array['expiry_date'] = $expiry_date;
		}
		
		$or = db_or()->condition('status','1')->condition('status','V');
			
		$chk = db_update('l_mtw_registration_renewal')->fields($update_arr)->condition('application_id',$application_id)->condition($or)->execute();
		
	}else if($remark_type=='I'){
		
		$update_arr	= array('status' => $remark_type, 'renewal_date' => date('Y-m-d'), 'mtw_registration_certificate' => $mtw_signed_certificate_file );
		
		$or = db_or()->condition('status','V')->condition('status','T');
		db_update('l_mtw_registration_renewal')->fields($update_arr)->condition('application_id',$application_id)->condition($or)->execute();
		
		$master_array	= array('finalfees'=> $finalAmount,'mtw_registration_certificate'=> $mtw_signed_certificate_file,'status'=> $remark_type);
	}else{
		$master_array	= array('status' => $remark_type);
	}
	
	/*** Update Status of mtw Master Table ***/ 
	
	//echo '<pre>'; print_r($update_arr);  print_r($master_array); 
	
	//die;
	
	db_update('l_mtw_registration_master')->fields($master_array)->condition('id',$application_id)->execute();
	
	/***Insert In mtw Remarks Master Table ***/ 
		
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
		'ref_number'			=> $renewal_ref_number,
		'is_active'				=> 1,
		'renewal_id'			=> $renewal_id,
		'employee_id'			=> $employee_id
	);

	db_insert('l_mtw_remark_master')->fields($fieldsApplInfo2)->execute(); 	
	
	$message = 'Remark has been successfully saved.';
	drupal_set_message(t($message));
	
	if($remark_type == 'B'){	
		$msg = 'Your MTW application is sent back for rectification by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';
	}else if($remark_type == 'V'){	
		$msg = 'Your MTW application is approved for fees submission by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';
	}else if($remark_type == 'I'){	
		$msg = 'Congratulations! Your MTW registration certificate is issued by ALC(Govt. of WB). Login to download the certificate from dashboard (wblc.gov.in)';
	}else if($remark_type == 'R'){	
		$msg = 'Your MTW application is rejected by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';	
	}else if($remark_type == 'VA'){	
		$msg = 'Your MTW application is approved by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';		
	}
	
	$applicant_mobile_no = get_mobile_number($applicant_user_id); 
	if(!empty($applicant_mobile_no)){ 
		send_sms_for_user_alert($applicant_mobile_no, $msg);
		//send_sms_for_user_alert('8017090753', $msg);
	}else{
		$msg1	=	"Message not sent.Please try again later.";
		drupal_set_message(t($msg1));
	}
	
	// send mail start		
	$applicant_mailto  = get_email($applicant_user_id);
	$subject = 'Welcome to Labour Commissionerate, Govt. Of West Bengal';
	if($applicant_mailto){
		send_mail_for_user_alert($applicant_mailto, $subject, $msg);
	}
	
}


function generation_mtw_registration_number($form, &$form_state){
	
	 $val  					= $form_state['values']; //print_r($val); die('jjj');
	 $applicant_user_id		= $val['applicant_user_id'];
	 $application_id		= $val['application_id'];
	 $act_id				= 3;
	 
	 /*** Get Applicant Block Code & Statuses & ALC Details ****/
	
	$get_mtw_application							=	db_select('l_mtw_registration_master', 'lbrm');
	$get_mtw_application							->  leftJoin('l_customuser_relation_address', 'lcra','lcra.sub_div_code=lbrm.mtw_loc_subdivision ');
	$get_mtw_application							->	fields('lbrm',array('mtw_loc_subdivision','mtw_loc_areatype_code','registration_number','registration_date','backlog_id','mtw_qr_code','is_renew'));
	$get_mtw_application							->	fields('lcra',array('sub_div_code'));
	$get_mtw_application							->	condition('lbrm.id',$application_id);
	$get_mtw_application							->	condition('lbrm.user_id',$applicant_user_id);
	
	$get_mtw_application_result 					= 	$get_mtw_application->execute();
	
	if($get_mtw_application_result-> rowCount() > 0 ){
		
		$application_data 							= 	$get_mtw_application_result->fetchAssoc();
		$subdivision_code							=   $application_data['mtw_loc_subdivision'];
		$area_code									=   $application_data['mtw_loc_areatype_code'];
		$mtw_registration_number					=   $application_data['registration_number'];
		$mtw_registration_date						=	$application_data['registration_date'];
		$mtw_qr_code								=   $application_data['mtw_qr_code']; 
		$alc_subdiv_code							=   $application_data['sub_div_code']; 
		$is_renew									=	$application_data['is_renew'];
		$backlog_id									=	$application_data['backlog_id'];
	}
	
	if (!empty($is_renew) && $is_renew==1){
			/*** GENERATION OF QR CODE ****/
			
			$generated_qr_code					= 'MTW-REN'.$application_id.'A3U'.$applicant_user_id.'T'.time(); 
			
			/*** GENERATION OF MTW REGISTRATION NUMBER FOR BACKLOG DATA *******/			
			
			/******* Renewal table data *******/
			$getRenewalQuery		=	db_select('l_mtw_registration_renewal','lmrr');
			$getRenewalQuery->fields('lmrr',array());
			$getRenewalQuery->condition('lmrr.application_id',$application_id);
			$getRenewalQuery->condition('lmrr.status','T');
			$getRenewalExecute 		= 	$getRenewalQuery->execute();
			if($getRenewalExecute->rowCount()>0){
				$getRenewalResult	=	$getRenewalExecute->fetchObject();
				$previous_fees 		=	$getRenewalResult->previous_fees;
				$current_fees		=	$getRenewalResult->current_fees;
				$final_amount 		=	$previous_fees+$current_fees;
			}else{
				$final_amount		=	0;
			}
			
			/* Renewal section start */
			$dataQuery			=	db_select('l_mtw_registration_master','lmrm');
			$dataQuery->fields('lmrm',array('registration_date','mtw_registration_certificate','finalfees','application_date','expiry_date'));
			$dataQuery->condition('id',$application_id);
			$dataEx 			=	$dataQuery->execute();
			
			if($dataEx->rowCount()>0){
				$dataArr			=	$dataEx->fetchObject();
				
				$application_date	=	$dataArr->application_date;
				$registration_date	= 	$dataArr->registration_date;
				$previous_fees		=	0;
				$prvCertificate		=	$dataArr->mtw_registration_certificate;
				$expiry_date		=	$dataArr->expiry_date;
								
				// store data in renewal table when alc approve application
				$insertNewalPrvData	=	array(
					'application_date'		=> $application_date,
					'expiry_date' 			=>  $expiry_date,
					'mtw_qr_code'			=>	$generated_qr_code,
					'renewal_date'			=>	date('Y-m-d'),
				); 
				
				
				db_update('l_mtw_registration_renewal')->fields($insertNewalPrvData)->condition('application_id',$application_id)->condition('renewal_ref_number',$val['ref_number'])->execute();
				
				
				/*** Update Status of mtw Master Table ***/ 
			
				$update_status  					=  db_update('l_mtw_registration_master');
				$update_status->fields(array('mtw_qr_code' => $generated_qr_code, 'registration_date' => date("Y-m-d")));
				$update_status->condition('id',$application_id);
				$update_status->condition('act_id',$act_id);
				$update_status->condition('user_id',$applicant_user_id);
				$update_status->execute();
				
				
			}
			
			/* Renewal table end */
			
			$message							= drupal_set_message_custom(t('Registration Certificate has been successfully generated. Please Sign and Upload Certificate to complete this process'));//-------------miscellaneous module

 			$form_state['values']['hidden_action_message'] = $message;
			
			$form_state['rebuild']				= TRUE;
	
	}
}

function generation_mtw_registration_number_backlog($form, &$form_state){
	
	 $val  					= $form_state['values']; //print_r($val); exit;
	 $applicant_user_id		= $val['applicant_user_id'];
	 $application_id		= $val['application_id'];
	 $act_id				= 3;
	 
	 /*** Get Applicant Block Code & Statuses & ALC Details ****/
	
	$get_mtw_application							=	db_select('l_mtw_registration_master', 'lbrm');
	$get_mtw_application							->  leftJoin('l_customuser_relation_address', 'lcra','lcra.sub_div_code=lbrm.mtw_loc_subdivision ');
	$get_mtw_application							->	fields('lbrm',array('mtw_loc_subdivision','mtw_loc_areatype_code','registration_number','mtw_qr_code'));
	$get_mtw_application							->	fields('lcra',array('sub_div_code'));
	$get_mtw_application							->	condition('lbrm.id',$application_id);
	$get_mtw_application							->	condition('lbrm.user_id',$applicant_user_id);
	
	$get_mtw_application_result 					= 	$get_mtw_application->execute();
	
	if($get_mtw_application_result->rowCount() > 0 ){
		
		$applicant_data 							= 	$get_mtw_application_result->fetchAssoc();
		$subdivision_code							=   $applicant_data['mtw_loc_subdivision'];
		$area_code									=   $applicant_data['mtw_loc_areatype_code'];
		$mtw_registration_number					=   $applicant_data['registration_number'];
		$mtw_qr_code								=   $applicant_data['mtw_qr_code']; 
		$alc_subdiv_code							=   $applicant_data['sub_div_code']; 
		
		
	}
	
	if (empty($mtw_registration_number) && empty($mtw_qr_code)){
			/*** GENERATION OF QR CODE ****/
			
			 $generated_qr_code					= 'MTW-REN'.$application_id.'A3U'.$applicant_user_id.'T'.time();
		
			/*** GENERATION OF MTW REGISTRATION NUMBER ***/			
			
			
			$getShortNameSubdivision 			=   custom_user_short_name_fun($subdivision_code);  // ---------come from custom_user module  
			$getRegistrarCode					=	get_registration_code($area_code);              //-------------miscellaneous module
			$registrarCode 						=   substr($getRegistrarCode, -2);
			
			
			$chkdata							=	db_query("select registration_number from l_mtw_registration_master where id=(select MAX(id) from l_mtw_registration_master where registration_number!='');");				
			
			/*$chkdata							=	db_query("select registration_number from l_mtw_registration_master");	*/
			
			if($chkdata->rowCount()>0){
				$chkquery						=	$chkdata->fetchObject();
				$reg_data						=	$chkquery->registration_number;
			
			}else{
				$reg_data						= 	'';
			}
			
			
			if(empty($reg_data)){ 
				
						$registration_number	=	$getShortNameSubdivision.$registrarCode.'/'.'MTW'.'/'.'000001';
						$certificate_serial_no	=	'C/'.$alc_subdiv_code.'/'.date('Y').'/1';	

			}else{
						$reg_query				=	db_query("select  max(NULLIF(substr(registration_number,11,6),'') :: integer) as serial_num  from l_mtw_registration_master where mtw_loc_subdivision='".$alc_subdiv_code."'");
			
						$reg_query_result		=	$reg_query->fetchAssoc();
						
						$reg_code				=	$reg_query_result['serial_num'];
					  	$reg_code_next			= 	$reg_code+1; 
					  	$reg_first				=	$getShortNameSubdivision.$registrarCode.'/'.'MTW'.'/';
					  	$reg_second				=	str_pad($reg_code_next, 6, "0", STR_PAD_LEFT);
					  	$registration_number	=	$reg_first.$reg_second;
						$certificate_serial_no	=	'C/'.$alc_subdiv_code.'/'.date('Y').'/'.$reg_code_next;	
			}
			
			// get exp date from renewal table
			$or = db_or()->condition('status','V')->condition('status','T');
			$chk = db_select('l_mtw_registration_renewal','renmtw')->fields('renmtw',array('id','renewal_ref_number','expiry_date'))->condition('application_id',$application_id)->condition($or)->execute();
			
			if($chk->rowCount() > 0){
				$renewal_data = $chk->fetchObject();
				$renewal_ref_number = $renewal_data->renewal_ref_number;
				$expiry_date = $renewal_data->expiry_date;
				$renewal_id = $renewal_data->id;
			}			
			
			/*** Update Status of mtw Master Table ***/ 
			
			$update_status =  db_update('l_mtw_registration_master');
			$update_status->fields(array('expiry_date' => $expiry_date, 'mtw_qr_code' => $generated_qr_code, 'registration_number' => $registration_number, 'registration_date' => date("Y-m-d"), 'certificate_serial' => $certificate_serial_no));
			$update_status->condition('id',$application_id);
			$update_status->condition('act_id',$act_id);
			$update_status->condition('user_id',$applicant_user_id);
			$update_status->execute();
			
			$message							= drupal_set_message_custom(t('Registration Certificate has been successfully generated. Please Sign and Upload Certificate to complete this process'));//-------------miscellaneous module

 			$form_state['values']['hidden_action_message'] = $message;
			
			$form_state['rebuild']				= TRUE;
	
	}
}

function re_generation_mtw_registration_number_backlog($form, &$form_state){
	
	// $val  					= $form_state['values']; //print_r($val); exit;
//	 $applicant_user_id		= $val['applicant_user_id'];
//	 $application_id		= $val['application_id'];
//	 $act_id				= 3;
//	 
//	 /*** Get Applicant Block Code & Statuses & ALC Details ****/
//	
//	$get_mtw_application							=	db_select('l_mtw_registration_master', 'lbrm');
//	$get_mtw_application							->  leftJoin('l_customuser_relation_address', 'lcra','lcra.sub_div_code=lbrm.mtw_loc_subdivision ');
//	$get_mtw_application							->	fields('lbrm',array('mtw_loc_subdivision','mtw_loc_areatype_code','registration_number','mtw_qr_code'));
//	$get_mtw_application							->	fields('lcra',array('sub_div_code'));
//	$get_mtw_application							->	condition('lbrm.id',$application_id);
//	$get_mtw_application							->	condition('lbrm.user_id',$applicant_user_id);
//	
//	$get_mtw_application_result 					= 	$get_mtw_application->execute();
//	
//	if($get_mtw_application_result-> rowCount() > 0 ){
//		
//		$applicant_data 							= 	$get_mtw_application_result->fetchAssoc();
//		$subdivision_code							=   $applicant_data['mtw_loc_subdivision'];
//		$area_code									=   $applicant_data['mtw_loc_areatype_code'];
//		$mtw_registration_number					=   $applicant_data['registration_number'];
//		$mtw_qr_code								=   $applicant_data['mtw_qr_code']; 
//		$alc_subdiv_code							=   $applicant_data['sub_div_code']; 
//		
//		
//	}
//	
//	//if (empty($mtw_registration_number) && empty($mtw_qr_code)){
//			/*** GENERATION OF QR CODE ****/
//			
//			 $generated_qr_code					= 'MTW-REG'.$application_id.'A3U'.$applicant_user_id.'T'.time();
//		
//			/*** GENERATION OF MTW REGISTRATION NUMBER ***/			
//			
//			
//			$getShortNameSubdivision 			=   custom_user_short_name_fun($subdivision_code);  // ---------come from custom_user module  
//			$getRegistrarCode					=	get_registration_code($area_code);              //-------------miscellaneous module
//			$registrarCode 						=   substr($getRegistrarCode, -2);
//			
//			
//			$chkdata							=	db_query("select registration_number from l_mtw_registration_master where id=(select MAX(id) from l_mtw_registration_master where registration_number!='');");	
//			
//			if($chkdata->rowCount()>0){
//				$chkquery						=	$chkdata->fetchObject();
//				$reg_data						=	$chkquery->registration_number;
//			}else{
//				$reg_data						= 	'';
//			}
//			
//			
//			if(empty($reg_data)){ 
//				
//						$registration_number	=	$getShortNameSubdivision.$registrarCode.'/'.'MTW'.'/'.'000001';
//						$certificate_serial_no	=	'C/'.$alc_subdiv_code.'/'.date('Y').'/1';	
//
//			}else{
//						$reg_query				=	db_query("select  max (NULLIF(substr(registration_number,11,6),'') :: integer) as serial_num  from l_mtw_registration_master where mtw_loc_subdivision='".$alc_subdiv_code."'");
//			
//						$reg_query_result		=	$reg_query->fetchAssoc();
//						
//						$reg_code				=	$reg_query_result['serial_num'];
//					  	$reg_code_next			= 	$reg_code+1; 
//					  	$reg_first				=	$getShortNameSubdivision.$registrarCode.'/'.'MTW'.'/';
//					  	$reg_second				=	str_pad($reg_code_next, 6, "0", STR_PAD_LEFT);
//					  	$registration_number	=	$reg_first.$reg_second;
//						$certificate_serial_no	=	'C/'.$alc_subdiv_code.'/'.date('Y').'/'.$reg_code_next;	
//			}
//			
//			
//			/*** Update Status of mtw Master Table ***/ 
//			
//			$update_status  					=  db_update('l_mtw_registration_master');
//			$update_status->fields(array('mtw_qr_code' => $generated_qr_code, /*'registration_number' =>	$registration_number,*/ 'registration_date' => date("Y-m-d") /*'certificate_serial' => $certificate_serial_no ,*//*'expiry_date'=>date("Y").'-12-31'*/));
//			$update_status->condition('id',$application_id);
//			$update_status->condition('act_id',$act_id);
//			$update_status->condition('user_id',$applicant_user_id);
//			$update_status->execute();
//			
//			/* Renewal section start*/
//			$dataQuery			=	db_select('l_mtw_registration_master','lmrm');
//			$dataQuery->fields('lmrm',array('registration_date','mtw_registration_certificate','finalfees','application_date'));
//			$dataQuery->condition('id',$application_id);
//			$dataEx 			=	$dataQuery->execute();
//			
//			if($dataEx->rowCount()>0){
//				$dataArr			=	$dataEx->fetchObject();
//				
//				$application_date	=	$dataArr->application_date;
//				$registration_date	= 	$dataArr->registration_date;
//				$previous_fees		=	$dataArr->finalfees;
//				$prvCertificate		=	$dataArr->mtw_registration_certificate;
//				
//				
//				/********************************** Check Previous Exp Date ************************************/
//				$reg_masterq	= db_select('l_mtw_registration_master','lmrm')->fields('lmrm',array('backlog_id'))->condition('lmrm.application_id',$application_id)->execute();
//				
//				$reg_masterr	= $reg_masterq->fetchObject();
//				if($reg_masterr->backlog_id !='' ){
//					$sql = "select expiry_date as maxdt from l_mtw_registration_backlog where id = ".$reg_masterr->backlog_id."";
//					$query = db_query($sql);
//					$result = $query->fetchObject();
//					if($result->maxdt !='' ){
//						$StaringDate = date("Y").'-12-31';
//						$expiry_date	= date("Y-m-d", strtotime(date("Y-m-d", strtotime($StaringDate)) . " + 1 year"));
//					}		
//				}else{
//					$sql = "select max(expiry_date) as maxdt from l_mtw_registration_renewal where application_id = ".$application_id."";
//					$query = db_query($sql);
//					$result = $query->fetchObject();
//					
//					if($result->maxdt !='' ){
//						$StaringDate = date("Y").'-12-31';
//						$expiry_date	= date("Y-m-d", strtotime(date("Y-m-d", strtotime($StaringDate)) . " + 1 year"));
//					}else{
//						$expiry_date	= date("Y").'-12-31';
//					}
//				}
//				
//				// store data first time in renewal table
//				$updateRenewalPrvData	=	array(
//											'previous_fees' 		=>	0,
//											'current_fees' 			=>	$previous_fees,
//											'final_fees' 			=>	$previous_fees,
//											'mtw_registration_certificate'=> trim($prvCertificate),
//											'application_date'		=> 	$application_date, 		// Applicant apply date
//											'renewal_date'			=>	NULL,
//											'expiry_date' 			=>  $expiry_date,
//											//'status'				=>	'I'
//										); 
//				db_update('l_mtw_registration_renewal')->fields($updateRenewalPrvData)->condition('application_id',$application_id)->condition('status','V')->execute();
//			}
//			
//			/* Renewal table end */
//			
//			
//			
//			
//			$message							= drupal_set_message_custom(t('Registration Certificate has been successfully generated. Please Sign and Upload Certificate to complete this process'));//-------------miscellaneous module
//
// 			$form_state['values']['hidden_action_message'] = $message;
//			
//			$form_state['rebuild']				= TRUE;
//	
//	//}
	
	
}


function get_mtw_remark_details($application_id,$applicant_user_id,$renewal_id){
	
	global $base_root, $base_path, $user;
	
	$user_id = $user->uid;
	
	$counter					= 0;
	
	$header = array(
		array('data' => 'Sl. No', 'field' => 'slno'),
		array('data' => 'Date / Time', 'field' => 'date'),
		array('data' => 'Remark', 'field' => 'remrk'),
		array('data' => 'Remark Status', 'field' => 'remark type', 'width' => '10%'),
		array('data' => 'Remark By', 'field' => 'licenseno'),
		array('data' => 'Action')
	);
	
	$get_remark = db_select('l_mtw_remark_master', 'lbrm');
	$get_remark	->leftJoin('role', 'ro', 'ro.rid = lbrm.remark_by_role');
	$get_remark->fields('lbrm', array());
	$get_remark->fields('ro', array('name'));
	$get_remark->condition('lbrm.remark_to', trim($applicant_user_id));
	$get_remark->condition('lbrm.application_id', trim($application_id));
	$get_remark->condition('lbrm.renewal_id', trim($renewal_id));
	$get_remark->condition('lbrm.is_active', 1);
	$get_remark->orderBy('lbrm.id','DESC');
	$get_remark_result = $get_remark->execute()->fetchAll(); 
	
	
	if(!empty($get_remark_result)){
		foreach($get_remark_result as $row){ 
			
			$counter 		= $counter+1;
			$remark_by		= $row->remark_by_name.'('.$row->name.')';
			//$remark_by		= $row->name;
			
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
			}else if(trim($row->remark_type) == 'U'){ 
				$STATUS_IMG =  '<span class="pending">Back For Correction (Form-I)</span>'; 
			}else if(trim($row->remark_type) == 'VA'){ 
				$STATUS_IMG =  '<span class="approved">Approved</span>'; 
			}else if(trim($row->remark_type) == 'RN'){ 
				$STATUS_IMG =  '<span class="applied">Renewal Applied</span>'; 
			}
			
			if (!empty($row->call_time_applicant)){
				$call_time_applicant =  ' ( '.date('dS M, Y - g:i A',$row->call_time_applicant).' )';
			}else{
				$call_time_applicant ='';
			}
			
			// get renewal id
			$ridQ = db_query("select MAX(id) as renid from l_mtw_registration_renewal where application_id = ".$application_id." and status !='I'")->fetchObject();
			$renewal_id = $ridQ->renid; 
			
			// get latest remark given by session user
			$getMaxData				= db_query("SELECT MAX(id) AS MAXID FROM l_mtw_remark_master WHERE is_active=1 and application_id = ".$application_id."")->fetchObject();
			$maxId					= $getMaxData->maxid;
			if($maxId>0){
				if(($row->remark_by==$user->uid) &&($maxId==$row->id)){
					if($row->remark_type == 'I'){
						$delete			= '<div id="delBtn" class="delete-btn disable-del">'.t('Delete').'</div>';
					}else{										
						$url 			= 'remarks-delete/'.encryption_decryption_fun('encrypt',$row->id).'/'.encryption_decryption_fun('encrypt',$application_id).'/'.encryption_decryption_fun('encrypt',$applicant_user_id).'/renewal/'.$renewal_id;			
						$delete			= l('<div id="delBtn" class="delete-btn active-del">'.t('Delete').'</div>',$url,array('html' => TRUE));	
					}
				}else{
					$delete			= '<div id="delBtn" class="delete-btn disable-del">'.t('Delete').'</div>';
				}
			}
			$rows[] 	= array(
				array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
				array('data' => date('dS M, Y - g:i A', $row->remark_date), 'align' => 'left', 'class' => array('odd')),
				array('data' => $row->remark_text.$call_time_applicant, 'align' => 'left', 'class' => array('odd')),
				array('data' => $STATUS_IMG, 'align' => 'left', 'class' => array('odd')),
				array('data' => $row->remark_by_name.'&nbsp;( ' .$row->name.' )', 'align' => 'left', 'class' => array('odd')),
				array('data' => $delete)
			);
		}
	
	}else{
		$rows = array();
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
	$commands[] = ajax_command_replace('#call_applicant_block', drupal_render($form['view_mtw_registration_form_alc']['call_applicant']));
	$commands[] = ajax_command_replace('#download_link_block', drupal_render($form['view_mtw_registration_form_alc']['download_link']));
	$commands[] = ajax_command_replace('#mtw_signed_certificate_block', drupal_render($form['view_mtw_registration_form_alc']['mtw_signed_certificate']));
	$commands[] = ajax_command_replace('#generate_certificate_block', drupal_render($form['view_mtw_registration_form_alc']['generate_certificate']));
	$commands[] = ajax_command_replace('#regenerate_certificate_block', drupal_render($form['view_mtw_registration_form_alc']['regenerate_certificate']));
	
	$commands[] = ajax_command_replace('#remarks_text_block', drupal_render($form['view_mtw_registration_form_alc']['remarks_text']));
	$commands[] = ajax_command_replace('#submit_block', drupal_render($form['view_mtw_registration_form_alc']['submit']));
	return array('#type' => 'ajax', '#commands' => $commands);
	
}

	

