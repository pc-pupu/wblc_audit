<?php
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
drupal_add_css(drupal_get_path('module', 'alc') . '/css/customstyle.css');
drupal_add_js(drupal_get_path('module', 'alc').'/js/mycustom.js');
	
function alc_application_details_mtw($form, &$form_state, $id = '',$app_user_id=''){
	
	global $base_root, $base_path, $user;
	$uri3						= explode('#',$app_user_id);
	$app_user_id				= $uri3[0];
	//$uriID						= $uri3[1];
	
	
	$user_id 					= $user->uid;
	$action	 					= 'decrypt';
	$action_encrypt				= 'encrypt';
	$application_id				= encryption_decryption_fun($action, $id);
	$applicant_user_id			= encryption_decryption_fun($action, $app_user_id);
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
	$get_mtw_application							->	fields('lmrm',array('mtw_loc_subdivision','final_submit_status','status','registration_number','mtw_qr_code','mtw_registration_certificate','is_renew','is_amend','backlog_id'));
	$get_mtw_application							->	condition('lmrm.mtw_loc_subdivision',$alc_subdivision_code);
	$get_mtw_application							->	condition('lmrm.id',$application_id);
	$get_mtw_application							->	condition('lmrm.user_id',$applicant_user_id);
	$get_mtw_application							->	condition('lmrm.final_submit_status','P');
	$get_mtw_application_result 					= 	$get_mtw_application->execute();

	
	if($get_mtw_application_result-> rowCount() > 0 ){
		
		$applicant_data 							= $get_mtw_application_result->fetchAssoc();
		$applicant_subdivsion_code 					= trim($applicant_data['mtw_loc_subdivision']);
		$mtw_registration_number					= $applicant_data['registration_number'];
		$mtw_qr_code								= $applicant_data['mtw_qr_code'];
		$applicant_final_submit_status				= $applicant_data['final_submit_status']; 
		$applicant_status							= $applicant_data['status'];
		$uploaded_signed_certificate				= $applicant_data['mtw_registration_certificate'];
		$renewal_status								= $applicant_data['is_renew'];
		$amend_status								= $applicant_data['is_amend'];
		$backlog_id									= $applicant_data['backlog_id'];
		
		
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
			$current_status = '<div class="callout callout-success"><h4>CURRENT STATUS: Registration Certificate is issued. </h4><p>Please check the Registration Certificate after uploading the Certificate.</div>';
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
	}
	
	
	
	/*** Get REMARK Details ****/
	
	$get_remark = db_select('l_mtw_remark_master', 'lmrkm');
	$get_remark->fields('lmrkm', array('remark_field_title','remark_text'));
	$get_remark->condition('lmrkm.remark_to', trim($applicant_user_id));
	$get_remark->condition('lmrkm.application_id', trim($application_id));
	$get_remark->condition('lmrkm.remark_by', trim($applicant_user_id), '!=');
	$get_remark->condition('lmrkm.is_active', 1);
	$get_remark->orderBy('lmrkm.id','DESC');
	$get_remark->range(0, 1);
	$get_remark_result = $get_remark->execute();
			
	$remark_details = $get_remark_result->fetchAssoc(); 

	$info = check_final_submit_data(encryption_decryption_fun('encrypt', 3), encryption_decryption_fun('encrypt', $application_id));
		
		if(!empty($info) && $info['remark_type']=='S'){
			$final_submit_date = $info['remark_date'];
			$today = time();						
			$datediff = $today - $final_submit_date;	
			$diff = round($datediff / (60 * 60 * 24));	
		}
																							
	if($alc_subdivision_code == $applicant_subdivsion_code ){   /** Checking for If the Application can be viewed by the ALC of the Subdivision ****/
	
	
	
			$form['view_mtw_registration_form_alc']	= array(
			  		'#type' 				=> 'fieldset',
			  		'#attributes'		 	=> array('class'=>array(''))
			);
																									
																									
		
			$form['view_mtw_registration_form_alc']['markup_data']		= array(
					'#prefix' 				=> '',
					'#markup' 				=> view_mtw_registration_form_func_alc($application_id,$applicant_user_id),
					'#suffix' 				=> '',
			);
																									
																									
																									
			$form['view_mtw_registration_form_alc']['show_status'] 		= array(
					'#type' 				=> 'markup',
					'#markup'				=> $current_status,
			);
																									
			if ($applicant_status != 'B' && $applicant_status != 'V' && $applicant_status != 'I' && $applicant_status != 'VA' && $applicant_status != 'U'){																						
																									
			$form['view_mtw_registration_form_alc']['markup_data_1']		= array(
					'#type' 				=> 'markup',
					'#markup' => '<div class="box box-primary box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i> Actions And Remarks</div><div class="box-body">',
			);
																									
			
			$form['view_mtw_registration_form_alc']['hidden_action_message']	= array(
				'#type'					=> 'hidden',
			);	
																								
			if($applicant_status == 'I' && !empty($uploaded_signed_certificate)){
				
				$form['view_mtw_registration_form_alc']['view_uploaded_certificate'] = array(
					'#markup' 				=>	l(t('Uploaded Signed mtw Certificate  <img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/pdf.png">'),''.$GLOBALS['base_url'].'/sites/default/files/upload/MTW_Registration_Certificate/'.$uploaded_signed_certificate, array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('#pdf-img')))),
				);		
				
			}

																							
			if ($applicant_status != 'B' && $applicant_status != 'V' && $applicant_status != 'I' && $applicant_status != 'VA' && $applicant_status != 'U'){
				if($diff <=30){	 /*** DEEMED **/	
					$form['view_mtw_registration_form_alc']['fieldname']	= array(
						'#title'				=> 'Field Name',
						'#type'					=> 'hidden',
						'#attributes'			=> array('id' => 'mtw_field_name_alc'),
						'#default_value' 		=> !empty($remark_details['remark_field_title']) ? $remark_details['remark_field_title'] : '',
					);
																											
					$form['view_mtw_registration_form_alc']['applicant_user_id']	= array(
						'#title'				=> 'User Id',
						'#type'					=> 'hidden',
						'#attributes' 			=> array('readonly' => 'readonly'),
						'#default_value' 		=> !empty($applicant_user_id) ? $applicant_user_id : '',
					);
																											
					$form['view_mtw_registration_form_alc']['application_id']		= array(
						'#title'				=> 'Application Id',
						'#type'					=> 'hidden',
						'#default_value' 		=> !empty($application_id) ? $application_id : '',
						'#attributes' 			=> array('readonly' => 'readonly'),
					);
																											
					
					
					if(empty($backlog_id) || $backlog_id=="0" || $backlog_id==""){
						$amount_payable = "Y";
					}else{
						$amount_payable = "N";
					}
					
					if($renewal_status != 1 && $amend_status != 1){
						$form['view_mtw_registration_form_alc']['remark_type']	= array(
								//'#prefix' 				=> '<section><label class="select">',
								//'#suffix' 				=> '<i></i></label></section>',
								'#prefix' 				=> '<div class="col-md-3">&nbsp;</div><div class="form-custom col-md-6"><label class="input">',
								'#suffix' 				=> '<i></i></label>',
								'#title'				=> 'Please Select Actions',
								'#type'					=> 'select',
								'#required' 			=>  TRUE,
								'#options'				=>  actions_amendment_dropdown_by_alc($applicant_status,$amount_payable),
								'#attributes'			=> array('class' => array('form-control')),
								'#ajax'					=> array(
																'event' 			=> 'change',
																'effect' 			=> 'fade',			
																'callback'			=> 'get_call_date_time_field',
																'progress'			=> '',
															  ),
								);
					}
				
					
					
					$form['view_mtw_registration_form_alc']['call_applicant'] 	= array(
						  '#prefix' 			=> '<div id="call_applicant_block">',
						  '#suffix' 			=> '</div>'
					);
																											
					$form['view_mtw_registration_form_alc']['generate_certificate'] = array(
						  '#prefix' 			=> '<div id="generate_certificate_block">',
						  '#suffix' 			=> '</div>'
					);
					$form['view_mtw_registration_form_alc']['re_generate_certificate'] 	= array (
						  '#prefix' 			=> '<div id="re_generate_certificate_block">',
						  '#suffix' 			=> '</div>'
					);
																									
					$form['view_mtw_registration_form_alc']['download_link'] 			= array(
						  '#prefix' 			=> '<div id="download_link_block">',
						  '#suffix' 			=> '</div>'
					);
					
					$form['view_mtw_registration_form_alc']['mtw_signed_certificate'] 	= array(
						  '#prefix' 			=> '<div id="mtw_signed_certificate_block">',
						  '#suffix' 			=> '</div>'
					);
																									
					$form['view_mtw_registration_form_alc']['remarks_text'] 			= array(
						  '#prefix' 			=> '<div id="remarks_text_block">',
						  '#suffix' 			=> '</div>'
					);
																									
																									
					$form['view_mtw_registration_form_alc']['submit'] 					= array(
						  '#prefix' 			=> '<div id="submit_block">',
						  '#suffix' 			=> '</div>'
					);
																											
																											
					$value_remark_type 		= isset($form_state['values']['remark_type']) ? $form_state['values']['remark_type'] : '';
					
					if($value_remark_type == 'I'){
						
						if(!empty($mtw_registration_number) && !empty($mtw_qr_code)){
						
							/**** regenerate mtw certificate start *****/ 
							$form['view_mtw_registration_form_alc']['re_generate_certificate'] 		= array (
									  '#prefix' 			=> '<div id="re_generate_certificate_block">',
									  '#suffix' 			=> '</div>',
									  '#type' 				=> 'submit',
									  '#attributes' 		=> array('class' => array('button')),
									  '#value' 				=> 'Regenerate MTW Registration Certificate',
									  '#submit'				=> array('re_generation_mtw_registration_number'),
							);
						
						
							/**** regenerate mtw certificate end *****/ 
							$value_check 			= isset($form_state['values']['regenerate_certificate']) ? $form_state['values']['regenerate_certificate'] : '';
							if(!empty($value_check) && ($value_check=='Regenerate MTW Registration Certificate')){						
								$requir_field		=	TRUE;
							}else{
								$requir_field		=	FALSE;
							}
							
							$form['view_mtw_registration_form_alc']['download_link']	= array(
										'#prefix'				=> '<div id="download_link_block"><section>',
										'#markup' 				=> l(t('Download MTW Registration Certificate<img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/pdf.png">'),'mtw-certificate-generate/'.encryption_decryption_fun($action_encrypt, $mtw_qr_code), array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('#pdf-img')))),
										
				
										'#suffix' 				=> '</section></div>',
							
							);
																											
																											
							$form['view_mtw_registration_form_alc']['mtw_signed_certificate']		= array(
										'#title'				=> 'Upload Signed mtw Registration Certificate ',
										'#type' 				=> 'managed_file',
										'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
										'#upload_location' 		=> 'public://upload/MTW_Registration_Certificate/',
										'#process' 				=> array('import_my_file_element_process'),
										'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
										'#prefix'				=> '<div id="mtw_signed_certificate_block"><section>',
										'#suffix'				=> '</section></div>',
										'#required' 			=>  $requir_field,
							);	
																											
							$form['view_mtw_registration_form_alc']['remarks_text']		= array(
										'#prefix' 				=> '<div id="remarks_text_block"><section><label class="input">',
										'#suffix' 				=> '</label></section></div>',
										'#title'				=> 'Remarks',
										'#type'					=> 'textfield',
										'#attributes'			=> array('class' => array('form-control')),
										'#rows'					=>  2,
										'#required' 			=>  $requir_field,
										'#default_value'		=> 'MTW Registration Certificate'
							);
																											
							$form['view_mtw_registration_form_alc']['submit'] 	= array (
										  '#prefix' 				=> '<div id="submit_block">',
										  '#suffix' 				=> '</div>',	
										  '#type' 					=> 'submit',
										  '#attributes' 			=> array('class' => array('btn btn-primary pull-left acremark-btn')),
										  '#value' 					=> 'Submit'
							);	
							
						}else{
					
							$form['view_mtw_registration_form_alc']['generate_certificate'] 		= array (
										  '#prefix' 			=> '<div id="generate_certificate_block">',
										  '#suffix' 			=> '</div>',
										  '#type' 				=> 'submit',
										  '#attributes' 			=> array('class' => array('btn btn-primary pull-left acremark-btn')),
										  '#value' 				=> 'Generate MTW Registration Certificate',
										  //'#submit'				=> array('generation_mtw_registration_number'),
							);																				
						}
					}
					}else{ 
						common_deemed_update_status(encryption_decryption_fun('encrypt', 3),encryption_decryption_fun('encrypt', $application_id),encryption_decryption_fun('encrypt', $user_id));			
					}
					//$selectedValue		=	isset($form_state['values']['remark_type'])?$form_state['values']['remark_type']:'';
					if($value_remark_type		!= 'I'){
						
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
						}else{
							$placeholdertxt			= '';
						}					
						
						if($renewal_status != 1 && $amend_status != 1){
							$form['view_mtw_registration_form_alc']['remarks_text']	= array(
									'#prefix' 				=> '<div id="remarks_text_block"><section><label class="input">',	
									'#suffix' 				=> '</label></section></div>',
									'#title'				=> 'Remarks',
									'#type'					=> 'textfield',
									'#attributes'			=> array('class' => array('form-control')),
									'#rows'					=>  2,
									'#required' 			=>  TRUE,																																																	
									'#default_value'		=> $placeholdertxt,
									'#attributes'			=> array('placeholder'=>$placeholdertxt),																									
							);
							
							
							
							$form['view_mtw_registration_form_alc']['submit'] 	= array (
								  '#prefix' 				=> '<div id="submit_block">',
								  '#suffix' 				=> '</div>',	
								  '#type' 					=> 'submit',
								  '#attributes' 			=> array('class' => array('btn btn-primary pull-left acremark-btn')),
								  '#value' 					=> 'Submit'
							);	
						
						}
					}
			
			}
			
		
			$form['view_mtw_registration_form_alc']['markup_data_2']		= array(
					'#type' 				=> 'markup',
					'#markup'				=> '</div></div></div>'	
			);
			
			}
			
			$form['view_mtw_registration_form_alc']['markup_data_5']	= array(
					'#type' 				=> 'markup',
					'#markup' 				=> '<div class="box box-primary box-solid">
													<div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">Remark Summary</h3></div>
													<div class="box-body"><div class="feedback-scroll">'.get_mtw_remark_details($application_id,$applicant_user_id).'</div></div>
												</div>',
					
			);
																										
	}else{
		$message = 'You are not permitted to view this application.';
		drupal_set_message(t($message), 'error');	
	}
	return $form;
}

function view_mtw_registration_form_func_alc($application_id='' ,$applicant_user_id= ''){ 
	
	global $base_root, $base_path, $user;
	
	$imgurl = $GLOBALS["base_url"].'/'.drupal_get_path('theme', 'jackson').'/images/zoom.png';
	
	if(is_numeric($applicant_user_id) && is_numeric($application_id)){ 
		
		if(!empty($application_id)){
			
			global $base_root, $base_path, $user;
	
			$master_part_mtw_reg = db_select('l_mtw_registration_master', 'lmrm');
			$master_part_mtw_reg->fields('lmrm', array());		
			$master_part_mtw_reg->condition('lmrm.user_id', $applicant_user_id);
			$master_part_mtw_reg->condition('lmrm.id', $application_id);
			$master_part_mtw_reg_result = $master_part_mtw_reg->execute();
			
			if($master_part_mtw_reg_result->rowCount() > 0){
				
				$content	= 	$master_part_mtw_reg_result->fetchAssoc();
				$fees 		= $content['mtw_maxworkers'];
				$upload_renewal_certificates_file = $content['mtw_registration_certificate'];
				if($fees <= 5){
					$retFees	=	'10.00';
				}else if($fees > 5 && $fees <= 25  ){
					$retFees	=	'25.00';
				}else if($fees > 25 && $fees <= 50  ){
					$retFees	=	'50.00';
				}else if($fees > 50 && $fees <= 100  ){
					$retFees	=	'100.00';
				}else if($fees > 100 && $fees <= 250  ){
					$retFees	=	'250.00';
				}else if($fees >250 && $fees <= 500){
					$retFees	=	'500.00';
				}else if($fees > 500 && $fees <= 750  ){
					$retFees	=	'750.00';
				}else if($fees > 750 && $fees <= 1000  ){
					$retFees	=	'1000.00';
				}else{
					$retFees	= '0.00';
				}
			}
			
			$table			= 'l_mtw_registration_master';
			$type			= 'mtw';
			$id				= $application_id;
			$fieldArr		= array('mtw_loc_dist','mtw_loc_subdivision','mtw_loc_areatype','mtw_loc_areatype_code','mtw_loc_vill_ward','mtw_loc_ps','mtw_loc_pincode');
			$establishment_address 	= get_full_address($table, $type, $id, $fieldArr);
			
			/**** Uploaded Documents ****/




			
			$enc_doc_array = array('TL'=> '','PD'=> '','AOA'=> '','MOA'=> '','FL'=> '','AC'=> '','PC'=> '','BB'	=> '','SC'	=> '','IC'	=> '','AP'=> '','ODSC'=> '','CR'=> '','WO'=> '','ORC'=> '');
	
			$pdficonlink = '<img title="View Documents" alt="View Documents" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/pdf.png">';


			// data from old format
			$master_part_mtw_docs = db_select('l_documents_upload', 'ldu');
			$master_part_mtw_docs->fields('ldu', array());
			$master_part_mtw_docs->condition('ldu.user_id', trim($applicant_user_id));
			$master_part_mtw_docs->condition('ldu.application_id', trim($application_id));
			$master_part_mtw_docs->condition('ldu.act_id', 3);
			$master_part_mtw_docs_result = $master_part_mtw_docs->execute();
			
			// if( $master_part_mtw_docs_result->rowCount() > 0 ){
			// 	$content_docs = $master_part_mtw_docs_result->fetchAssoc();
			// }else{
			// 	$content_docs = $enc_doc_array;
			// }
			
			// if($application_id == 223){
			// echo '<pre>'; print_r($master_part_mtw_docs_result->fetchAssoc()); die;
			// }
						
			$trade_license_file			= '';
			$article_of_assoc_file		= '';
			$form_one_asses_ses_file	= '';
			$supp_asses_ses_file		= '';
			$mtw_address_proof_file		= '';
			$other_doc_file				= '';
			
			
			$global_link = $base_root.$base_path.'sites/default/files/upload/';
			
			if( $master_part_mtw_docs_result->rowCount() > 0 ){
				$content_docs = $master_part_mtw_docs_result->fetchObject();

				if($content_docs->trade_license_fid !=''){
					// data from encrypted data table
					$query_en 			= db_select('l_encrypted_uploaded_documents','leud')->fields('leud',array())->condition('status','1')->condition('user_id',$applicant_user_id);
					$query_en_result 	= $query_en->execute();
					
					if($query_en_result->rowCount() > 0){
						foreach($query_en_result->fetchAll() as $data_en ){
							$enc_doc_array[$data_en->document_type_code]['content'] = 'view_documents/'.encryption_decryption_fun('encrypt',$data_en->id);
						}
					}
				}else{
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


			}else{
				$enc_doc_array['TL']['content'] = '';
				$enc_doc_array['AOA']['content'] = '';
				$enc_doc_array['PD']['content'] = '';
				$enc_doc_array['BB']['content'] = '';
				$enc_doc_array['SC']['content'] = '';
				$enc_doc_array['IC']['content'] = '';
				$enc_doc_array['AP']['content'] = '';
				$enc_doc_array['ODSC']['content'] ='';


				// data from encrypted data table
				$query 			= db_select('l_encrypted_uploaded_documents','leud')->fields('leud',array())->condition('status','1')->condition('user_id',trim($applicant_user_id));
				$query_result 	= $query->execute();
				// echo $query_result->rowCount();
				if($query_result->rowCount() > 0){
					$endocarr = $query_result->fetchAll();
					foreach($endocarr as $data ){ //print_r($data); 
						$enc_doc_array[$data->document_type_code]['content'] = 'view_documents/'.encryption_decryption_fun('encrypt',$data->id);
						$enc_doc_array[$data->document_type_code]['rowid'] = $data->id;
					}
				}

			}
			
			
			
			
			/********* Remaks Data ********/
			$get_remark = db_select('l_mtw_remark_master', 'lmrkm');
			$get_remark->fields('lmrkm', array('remark_field_title'));
			$get_remark->condition('lmrkm.remark_to', trim($applicant_user_id));
			$get_remark->condition('lmrkm.application_id', trim($application_id));
			$get_remark->condition('lmrkm.remark_by', trim($applicant_user_id), '!=');
			$get_remark->condition('lmrkm.is_active', 1);
			$get_remark->orderBy('lmrkm.id','DESC');
			$get_remark->range(0, 1);
			$get_remark_result = $get_remark->execute();
					
			$content_5 = $get_remark_result->fetchAssoc(); 
					
			$remark_field_text_arr = explode(',', $content_5['remark_field_title']);
			
			$finefees_ck = '';
			
			$mtw_name_ck = '';
			$mtw_loc_address_ck = '';
			$mtw_nature_ck  = '';
			$total_routes_ck  = '';
			$total_route_milage_ck  = '';
			$total_mtw_vehicle_ck  = '';
			$mtw_maxworkers_ck  = '';
			$mtw_route_details_ck  = '';
			$trade_license_file_ck  = '';
			$article_of_assoc_file_ck  = '';
			$memorandum_of_cert_file_ck  = '';
			$partnership_deed_file_ck  = '';
			$challan_file_ck  = '';
			$work_order_file_ck  = '';
			$form_one_asses_ses_file_ck  = '';
			$supp_asses_ses_file_ck  = '';
			$other_doc_file_ck  = '';
			$mtw_address_proof_file_ck  = '';
			$finalfees_ck  = '';
			$form_1_mtw_signed_pdf_file_ck  = '';
			
			if(!empty($remark_field_text_arr)){ 
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
					
					// person out of wb state code
					$personState	= $single->person_state;
					$getStateQuery	= db_select('state_master','sm')->fields('sm',array())->condition('sm.id',$personState,'=')->execute();
					$getStateData	= $getStateQuery->fetchObject();
					if(!empty($getStateData)){
						$pr_state	= $getStateData->statename.', ';
					}else{
						$pr_state	= '';
					}
					
					
					$personArray[]		= '<tr>
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
		
		
		
		
		$output1 = '<div class="box box-primary box-solid">
						<div class="box-header with-border"><i class="ion ion-clipboard"></i>Establishment Details<br></div>
						<div class="box-body"><div class="feedback-scroll">
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
		
		
		$output1 .= '			<tr>
									<td>1. </td>
									<td>Name of Motor Transport Undertaking </td>
									<td>'.$content['mtw_name'].'</td>
									<td><input type="checkbox" id="mtw_name" class="mtw_verified_alc"  '.$mtw_name_ck.' /></td>
								</tr>';
		
		$output1 .= '			<tr>
									<td>2. </td>
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
		
		
		$output1 .= '			<tr>
									<td>3. </td>
									<td>Nature of motor transport service</td>
									<td>'.$mtw_nature_contrnt.'</td>
									<td><input type="checkbox" id="mtw_nature" class="mtw_verified_alc" '.$mtw_nature_ck.' /></td>								
								</tr>';
		
		$output1 .= '			<tr>
									<td>4. </td>
									<td>Total number of routes</td>
									<td>'.$content['total_routes'].'</td>
									<td><input type="checkbox" id="total_routes" class="mtw_verified_alc"  '.$total_routes_ck.' /></td>								
								</tr>';
		
		if($content['total_routes'] != '' && $content['total_routes'] != 0){
			$total_milage_add 	= 0;
			$total_no		  	= 0;
			$routeDetails1		=	db_select('l_mtw_registration_extended','mtwe');
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
		
		
		$output1 .= '			<tr>
									<td>5. </td>
									<td>Total no of Route - Total Route Milage</td>
									<td>'.$output2.'</td>
									<td><input type="checkbox" id="total_route_milage" class="mtw_verified_alc" '.$total_route_milage_ck.' /></td>					
								</tr>';
		
		$output1 .= '			<tr>
									<td>6. </td>
									<td>Total number of motor transport vehicles on the last date or the preceeding year</td>
									<td>'.$content['total_mtw_vehicle'].'</td>
									<td><input type="checkbox" id="total_mtw_vehicle" class="mtw_verified_alc" '.$total_mtw_vehicle_ck.' /></td>				
								</tr>';
		
		$output1 .= '			<tr>
									<td>7. </td>
									<td>Maximum number of motor transport workers employed on any day during the preceeding year</td>
									<td>'.$content['mtw_maxworkers'].'<div class="btn btn-info pull-right" data-toggle="modal" data-target="#myModalFeeschat"><i class="fa fa-info-circle"></i> Fees Chart</div></td>
									<td><input type="checkbox" id="mtw_maxworkers" class="mtw_verified_alc" '.$mtw_maxworkers_ck.' /></td>				
								</tr>';
		
		$output1 .= 		'</table></div>
						</div>
					</div>';
		
		
		$output1	.= '<div class="modal fade" id="myModalFeeschat" role="dialog">
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
		
		
		
		$output1 .= '<div class="box box-primary box-solid">
						<div class="box-header with-border"><i class="ion ion-clipboard"></i>Ownership Details<br></div>
						<div class="box-body"><div class="feedback-scroll">
							<table width="100%" border="0" class="table table-bordered table-responsive">
								<tr>
									<th style="text-align:center;font-size:17px; padding-top: 0px;"> Name </th>
									<th style="text-align:center;font-size:17px; padding-top: 0px;"> Designation </th>
									<th style="text-align:center;font-size:17px; padding-top: 0px;"> Residential </th>
									<th style="text-align:center;font-size:17px; padding-top: 0px;"> Verified? </th>
								</tr>';					
							
		if(!empty($personArray)){
			foreach($personArray as $row){
				$output1 .=$row;
			}
		}
		
		
			
     	$output1 .= 		'</table></div>
						</div>
					</div>';
					
		
		
		
		
		$output1 .= '<div class="box box-primary box-solid">
						<div class="box-header with-border"><i class="ion ion-clipboard"></i> Uploaded Documents </div>
						<div class="box-body"><div class="feedback-scroll">
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
		
		$output1 .= 	'<tr>
							<td>(i)</td>
							<td>Trade License</td>
							<td>'.$viewlink1.'</td>
							<td><input type="checkbox" id="trade_license_file" class="mtw_verified_alc" '.$trade_license_file_ck.' /></td>		
						</tr>';
		
		/*if(!empty($content_docs['trade_license_file'])){
				$output1 .= 	'<tr>
									<td>(i)</td>
									<td>Trade License</td>
									<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/trade_license/'.$content_docs['trade_license_file'].'">
										<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>&nbsp;&nbsp;&nbsp;
										<span class="viewfile_popup" id="trade_license"><img src="'.$imgurl.'" title="Click here to view Trade license" alt="trade-license"></span>
										<div id="view_trade_license"  title="Trade License" style="display:none;">
										<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/trade_license/'.$content_docs['trade_license_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
										</div>
									</td>
									<td><input type="checkbox" id="trade_license_file" class="mtw_verified_alc" '.$trade_license_file_ck.' /></td>									
								</tr>';
		
		}else{
			$output1 .=  '<tr><td>(i)</td><td>Trade License</td><td></td><td><input type="checkbox" id="trade_license_file" class="mtw_verified_alc" '.$trade_license_file_ck.' /></td></tr>';
		}*/
		
		$viewlink2 = 'No Document Uploaded';
		if($enc_doc_array['AOA']['content'] !=''){
			$viewlink2 = l($pdficonlink,$enc_doc_array['AOA']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}else if($enc_doc_array['PD']['content'] !=''){
			$viewlink2 = l($pdficonlink,$enc_doc_array['PD']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}
		
		$output1 .= 	'<tr>
							<td>(ii)</td>
							<td>Articles of Association / Partnership Deed</td>
							<td>'.$viewlink2.'</td>
							<td><input type="checkbox" id="article_of_assoc_file" class="mtw_verified_alc"'.$article_of_assoc_file_ck.' /></td>	
						</tr>';
		
		
		/*
		if(!empty($content_docs['article_of_assoc_file'])){ 
				$output1 .= 	'<tr>
									<td>(ii)</td>
									<td>Articles of Association / Partnership Deed</td>
									<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/article_of_assoc/'.$content_docs['article_of_assoc_file'].'">
										<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>&nbsp;&nbsp;&nbsp;
										<span class="viewfile_popup" id="article_of_assoc_file_1"><img src="'.$imgurl.'" title="Click here to view Articles of Association and Memorandum of Association/Partnership Deed" alt="trade-license"></span>
										<div id="view_article_of_assoc_file_1"  title="Articles of Association and Memorandum of Association/Partnership Deed"  style="display:none;">
										<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/article_of_assoc/'.$content_docs['article_of_assoc_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
										</div>
									</td>
									<td><input type="checkbox" id="article_of_assoc_file" class="mtw_verified_alc"'.$article_of_assoc_file_ck.' /></td>									
								</tr>';
		}else{
				$output1 .=  '<tr><td>(ii)</td><td>Articles of Association / Partnership Deed</td><td>No Document Uploaded</td><td><input type="checkbox" id="article_of_assoc_file" class="mtw_verified_alc"'.$article_of_assoc_file_ck.' /></td></tr>';
		}
		*/
		
		$viewlink3 = 'No Document Uploaded';
		if($enc_doc_array['BB']['content'] !=''){
			$viewlink3 = l($pdficonlink,$enc_doc_array['BB']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}else if($enc_doc_array['SC']['content'] !=''){
			$viewlink3 = l($pdficonlink,$enc_doc_array['SC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}
		
		$output1 .= 	'<tr>
							<td>(iii)</td>
							<td>Blue Book/ Smart Card Issued by Motor Vehicles</td>
							<td>'.$viewlink3.'</td>
							<td><input type="checkbox" id="form_one_asses_ses_file" class="mtw_verified_alc" '.$form_one_asses_ses_file_ck.' /></td>
						</tr>';
		
		/*
		if(!empty($content_docs['form_one_asses_ses_file'])){ 
				$output1 .= 	'<tr>
									<td>(iii)</td>
									<td>Blue Book/ Smart Card Issued by Motor Vehicles</td>
									<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/blue_book/'.$content_docs['form_one_asses_ses_file'].'">
										<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>&nbsp;&nbsp;&nbsp;
										<span class="viewfile_popup" id="form_one_asses_ses_file"><img src="'.$imgurl.'" title="Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc."></span>
										<div id="view_form_one_asses_ses_file"  title="Form I for assesment of SES"  style="display:none;">
										<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/blue_book/'.$content_docs['form_one_asses_ses_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
										</div>
									</td>
									<td><input type="checkbox" id="form_one_asses_ses_file" class="mtw_verified_alc" '.$form_one_asses_ses_file_ck.' /></td>								
								</tr>';
		}else{
				$output1 .=  '<tr><td>(iii)</td><td>Blue Book/ Smart Card Issued by Motor Vehicles</td><td>No Document Uploaded</td><td><input type="checkbox" id="article_of_assoc_file" class="mtw_verified_alc"'.$article_of_assoc_file_ck.' /></td></tr>';
		}
		*/
		
		$viewlink4 = 'No Document Uploaded';
		if($enc_doc_array['IC']['content'] !=''){
			$viewlink4 = l($pdficonlink,$enc_doc_array['IC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}
		
		$output1 .= 	'<tr>
							<td>(iv)</td>
							<td>Insurance Certificate of Motor Vehicles</td>
							<td>'.$viewlink4.'</td>
							<td><input type="checkbox" id="supp_asses_ses_file" class="mtw_verified_alc" '.$supp_asses_ses_file_ck.' /></td>
						</tr>';
		/*
		if(!empty($content_docs['supp_asses_ses_file'])){
				$output1 .= 	'<tr>
									<td>(iv)</td>
									<td>Insurance Certificate of Motor Vehicles</td>
									<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/insurance/'.$content_docs['supp_asses_ses_file'].'">
										<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>&nbsp;&nbsp;&nbsp;
										<span class="viewfile_popup" id="supp_asses_ses_file"><img src="'.$imgurl.'" title="Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc."></span>
										<div id="view_supp_asses_ses_file"  title="Documents in Support of Payment of SES"  style="display:none;">
										<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/insurance/'.$content_docs['supp_asses_ses_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
										</div>
									</td>
									<td><input type="checkbox" id="supp_asses_ses_file" class="mtw_verified_alc" '.$supp_asses_ses_file_ck.' /></td>
								</tr>';
		}else{
				$output1 .= 	'<tr>
									<td>(iv)</td><td>Insurance Certificate of Motor Vehicles</td><td>No Document Uploaded</td><td><input type="checkbox" id="other_doc_file" class="mtw_verified_alc" '.$other_doc_file_ck.' /></td>
								
								</tr>';
		}
		*/
		
		$viewlink5 = 'No Document Uploaded';
		if($enc_doc_array['ODSC']['content'] !=''){
			$viewlink5 = l($pdficonlink,$enc_doc_array['ODSC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}
		
		$output1 .= 	'<tr>
							<td>(v)</td>
							<td>Documents in support of correctness of the application</td>
							<td>'.$viewlink5.'</td>
							<td><input type="checkbox" id="other_doc_file" class="mtw_verified_alc" '.$other_doc_file_ck.' /></td>
						</tr>';
		
		
		/*
		if(!empty($content_docs['other_doc_file'])){
			$output1 .= 	'<tr>
								<td>(v)</td>
								<td>Documents in support of correctness of the application</td>
								<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/OtherRelatedDocuments/'.$content_docs['other_doc_file'].'">
									<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>&nbsp;&nbsp;&nbsp;
									<span class="viewfile_popup" id="other_doc_file"><img src="'.$imgurl.'" title="Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc."></span>
									<div id="view_other_doc_file"  title="Documents in Support of Correctness of  Application"  style="display:none;">
									<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/OtherRelatedDocuments/'.$content_docs['other_doc_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
									</div>
								</td>
								<td><input type="checkbox" id="other_doc_file" class="mtw_verified_alc" '.$other_doc_file_ck.' /></td>
							</tr>';
		}else{
			$output1 .= 	'<tr><td>(v)</td><td>Documents in support of correctness of the application</td><td>No Document Uploaded</td><td><input type="checkbox" id="other_doc_file" class="mtw_verified_alc" '.$other_doc_file_ck.' /></td></tr>';
								
		}
		*/
		
		$viewlink6 = 'No Document Uploaded';
		if($enc_doc_array['AP']['content'] !=''){
			$viewlink6 = l($pdficonlink,$enc_doc_array['AP']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}

		

		$output1 .= 	'<tr>
							<td>(vi)</td>
							<td>Address Proof</td>
							<td>'.$viewlink6.'</td>
							<td><input type="checkbox" id="mtw_address_proof_file" class="mtw_verified_alc" '.$mtw_address_proof_file_ck.' /></td>
						</tr>
						';
						if($content['status']=='I'){
							if($content['deemed'] == 'Y'){
								$issue_by = 'Deemed Approved';
							}else{
								$issue_by = 'Issued By ALC';
							}
			$output1 .=	'<tr>
							<td>(vii)</td>
							<td>Registration Certificate</td>';
					if(!empty($content['mtw_registration_certificate'])){
						
						$output1 .= '<td>'.$generateDownloadLink.'&nbsp;&nbsp;&nbsp;<span class="viewfile_popup" id="signed_certificate"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
						<div id="view_signed_certificate" title="Signed MTW Registration Certificate" style="display:none;">
							<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/MTW_Registration_Certificate/'.$upload_renewal_certificates_file.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
						</div>
					</td>';
					}else{
						$generateDownloadLink 	= l(t('<i class="fa fa-download" aria-hidden="true"></i>&nbsp;  MTW Registration Certificate'),$GLOBALS['base_url'].'/mtw-certificate-generate/'.encryption_decryption_fun('encrypt', $content['mtw_qr_code']), array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn'))));

						$output1 .= '<td>'.$generateDownloadLink.'&nbsp;&nbsp;&nbsp;<span class="viewfile_popup" id="signed_certificate"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
						<div id="view_signed_certificate"  title="MTW Registration Certificate" style="display:none;">
							<iframe src="'.$GLOBALS['base_url'].'/mtw-certificate-generate/'.encryption_decryption_fun('encrypt', $content['mtw_qr_code']).'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
						</div>
					</td>
					<td>'.$issue_by.'</td>';
					}
			$output1 .= '</tr>';
						}
		/*
		if(!empty($content_docs['address_proof_file'])){
			$output1 .= 	'<tr>
								<td>(vi)</td>
								<td>Address Proof</td>
								<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/address_proof/'.$content_docs['address_proof_file'].'">
										<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>&nbsp;&nbsp;&nbsp;
										<span class="viewfile_popup" id="mtw_address_proof_file_id"><img src="'.$imgurl.'" title="Address Proof"></span>
										<div id="view_mtw_address_proof_file_id"  title="Documents in Support of Address Proof"  style="display:none;">
										<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/address_proof/'.$content_docs['address_proof_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
										</div>
									</td>
								<td><input type="checkbox" id="mtw_address_proof_file" class="mtw_verified_alc" '.$mtw_address_proof_file_ck.' /></td>
							</tr>';
		}else{
			$output1 .= 	'<tr><td>(vi)</td><td>Address Proof</td><td>No Document Uploaded</td><td><input type="checkbox" id="mtw_address_proof_file" class="mtw_verified_alc" '.$mtw_address_proof_file_ck.' /></td></tr>';
		}
		
		if(!empty($content_docs['form_1_signed_pdf_file'])){
			$output1 .= '<tr>
							<td>(vii)</td>
							<td>Form-I</td>
							<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/mtw_Form-I/'.$content_docs['form_1_signed_pdf_file'].'">
								<img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png">
								</a>
								<span class="viewfile_popup" id="form_1_file"><img src="'.$imgurl.'" title="Uploaded Application(Form-I) of Registration"></span>
								<div id="view_form_1_file"  title="Form - I Application"  style="display:none;">
								<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/mtw_Form-I/'.$content_docs['form_1_signed_pdf_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
								</div>
								&nbsp;&nbsp;&nbsp; '.l(t('Generated FORM - I'),'mtw-generate-pdf/'.encryption_decryption_fun('encrypt', $content['id']).'/'.encryption_decryption_fun('encrypt', $content['user_id']), array('html' => TRUE,'attributes'=> array('target'=>'_blank'))).'
							</td>
							<td><input type="checkbox" id="form_1_mtw_signed_pdf_file" class="mtw_verified_alc" '.$form_1_mtw_signed_pdf_file_ck.' /></td>
						</tr>';
		}else{
			$output1 .= '<tr><td>(vii)</td><td>Form-I</td><td>No Document Uploaded</td><td><input type="checkbox" id="form_1_mtw_signed_pdf_file" class="mtw_verified_alc" '.$form_1_mtw_signed_pdf_file_ck.' /></td></tr>';
		}*/
		
						
		$output1 .= '		</table>
						</div></div>
					</div>';	
				
		
		
		/* Payment Details Start */ // echo '<pre>'; print_r($content); die;
					
		 if(trim($content['status'])== 'T' || trim($content['status'])== 'S' || trim($content['status'])== 'I'){						
			$renwal_tdata = db_select('l_mtw_registration_renewal', 'mr');
			$renwal_tdata->fields('mr', array('id'));
			$renwal_tdata->condition('mr.application_id', $content['id']);
			//$renwal_tdata->condition('mr.status', 'T');	
			$renwal_tdata = $renwal_tdata->execute()->fetchObject();
			
							
			$transaction_details = db_select('l_principle_epayments_receive_data', 'lpd');
			$transaction_details->leftJoin('l_principle_epayments_data', 'lped', 'lpd.transaction_id=lped.identification_no');
			$transaction_details->fields('lped', array('identification_no', 'application_id'));
			$transaction_details->fields('lpd', array());
			$transaction_details->condition('lped.act_id', '3');
			$transaction_details->condition('lped.application_id', $renwal_tdata->id);		
			$trans_details_result = $transaction_details->execute(); 
			
			if($trans_details_result-> rowCount() > 0 ){			
				$transaction_det = $trans_details_result->fetchAssoc();
				$bankTransactionID	= $transaction_det['transaction_id'];
				$grn_number	= $transaction_det['challanrefid'];
				$challan_fid_date = $transaction_det['challanrefid_date']; // !empty($transaction_det['challanrefid_date']) ? date('d/m/Y',strtotime($transaction_det['challanrefid_date'])) : '';
				$total_amount = number_format($transaction_det['challanamount'], 2);
				$bank_code = $transaction_det['bank_cd'];
				$payment_status	= $transaction_det['banktransactionstatus'];	
			}
		 }else{
			$bank_code = '';
			$bankTransactionID = '';
			$total_amount = '';
			$challan_fid_date = '';
			$payment_status = '';
			$grn_number = '';
		 }
		 
		 $payment_details = '<u>Grips Payment[Online/Counter]</u><br>';
		 $payment_details .= 'IFSC Code: <span class="color_orange">'.$bank_code.'</span><br>';
		 $payment_details .= 'GRN Number: <span class="color_orange">'.$grn_number.'</span><br>';
		 $payment_details .= 'Bank Transaction Id: <span class="color_orange">'.$bankTransactionID.'</span><br>';
		 $payment_details .= 'Total Amount: <span class="color_orange">'.$total_amount.'</span><br>';
		 $payment_details .= 'Transaction Date: <span class="color_orange">'.$challan_fid_date.'</span><br>';
		 $payment_details .= 'Transaction Status: '.$payment_status; 					
		
		$output1 .= '<div class="box box-primary box-solid">
						<div class="box-header with-border"><i class="ion ion-clipboard"></i>  Payment Details </div>
						<div class="box-body"><div class="feedback-scroll">
							<table width="100%" border="0" class="table table-bordered table-responsive">';								
	
		$output1 .= '			<tr>
									<td> Amount fees</td>
									<td colspan="2"><b> &#8377;'.$retFees.'(Rupees '.convertNumberToWord($retFees).')</b></td>
									<td width="10%"><input type="checkbox" id="finalfees" class="mtw_verified_alc" '.$finalfees_ck.'  /></td>
								</tr>
								
								<tr>
									<td>Payment Details</td>
									<td colspan="3">'.$payment_details.'</td>  	
								</tr>
					';
	
						
	$output1 .= '		</table></div>
						</div>
					</div>';
		
	
	/*
	<tr>
									<td colspan="3" style="color:#ff0000;"><strong>Note:-</strong>Provided further that in cases where the Chief Inspector or an Inspector duly authorised by him in this behalf is satisfied that the delay in making the application was due to circumstances over which the employer had no control, he may reduce or remit, as he thanks fit, such excess fee <span style="color:#ff0000; font-weight:700;">(Must Checked if you want to add fine)</span> </td>
									<td><input type="checkbox" id="finefees" name="fine_field" class="mtw_verified_alc" '.$finefees_ck.'/></td>
								</tr>
	
	*/
	
	
		/* Payment Details End */ 
								
		$action	 					= 'encrypt';
		$application_id_pre			= encryption_decryption_fun($action, $application_id);
		$applicant_user_id_pre		= encryption_decryption_fun($action, $applicant_user_id);
		$act_id						= encryption_decryption_fun($action,$content['act_id']);
		
		   
		
		$output1 .= '
		<ul class="row" style="padding:0;">
			<li class="col-md-2 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-arrow-left"></i> Back to list</div>','alc-receive-application-mtw',array('attributes' =>array('class' => array('backapplication_view'), 'title' => 'Click here back to applications list'), 'html' => TRUE)).'</li>
			
			<li class="col-md-3 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-user"></i> View Applicant Profile</div>','view-applicant-profile/'.$application_id_pre.'/'.$applicant_user_id_pre.'/'.$act_id,array('attributes' =>array('class' => array('backapplication_view'), 'title' => 'Click here back to applications list'), 'html' => TRUE)).'</li>
			
			<li class="col-md-3 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-info-circle"></i> Instructions to give remarks</div>',$base_root.$base_path.'sites/default/files/instructions-remarks.pdf',array('attributes' =>array('class' => array('backapplication_view'), 'title' => 'Click here back to applications list'), 'html' => TRUE)).'</li>
			
			<li class="col-md-4 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-question-circle"></i> How to digitally sign documents using USB Token</div>','digitally-sign-process',array('attributes' =>array('class' => array('backapplication_view'), 'title' => 'Click here back to applications list'), 'html' => TRUE)).'</li>
		</ul>';
		
					   
		return $output1;
		
		
	}else{
		
		drupal_goto('node/3');	
	}
}


function alc_application_details_mtw_submit($form,&$form_state,$id='',$app_user_id=''){ 
	
	global $base_root, $base_path, $user;
	
	$alc_user_id 		= $user->uid;
	
	$val 				= $form_state['values']; 
	
	/* Upload For mtw Signed Certificate*/
	
	// $arr_1									= array();
	// $signed_form_mtw 						= file_load($val['mtw_signed_certificate']);
	// if( $signed_form_mtw != "" ){
	// 	$arr_1 = explode("/", $signed_form_mtw->uri);
	// 	$signed_form_mtw->status = FILE_STATUS_PERMANENT;
	// 	file_save($signed_form_mtw);
	// 	$signed_form_mtw_file = $arr_1[4];
	// }
	
	$applicant_user_id 	= $val['applicant_user_id'];
	$application_id 	= $val['application_id'];
	$fieldname			= $val['fieldname'];
	$remark_type		= $val['remark_type'];
	$remarks_text		= $val['remarks_text'];
	$call_date_time		= $val['call_applicant'];
	$act_id				= 3;
	
   
	//////  ************ Generation of Registation number Start  *****************   ///////
	
	/*** Get Applicant Block Code & Statuses & ALC Details ****/
	if($remark_type == 'I'){
	   $get_mtw_application							=	db_select('l_mtw_registration_master', 'lbrm');
	   $get_mtw_application							->  leftJoin('l_customuser_relation_address', 'lcra','lcra.sub_div_code=lbrm.mtw_loc_subdivision ');
	   $get_mtw_application							->	fields('lbrm',array('mtw_loc_subdivision','mtw_loc_areatype_code','registration_number','mtw_qr_code'));
	   $get_mtw_application							->	fields('lcra',array('sub_div_code'));
	   $get_mtw_application							->	condition('lbrm.id',$application_id);
	   $get_mtw_application							->	condition('lbrm.user_id',$applicant_user_id);
	   
	   $get_mtw_application_result 					= 	$get_mtw_application->execute();
	   
	   if($get_mtw_application_result-> rowCount() > 0 ){
		   
		   $applicant_data 							= 	$get_mtw_application_result->fetchAssoc();
		   $subdivision_code						=   $applicant_data['mtw_loc_subdivision'];
		   $area_code								=   $applicant_data['mtw_loc_areatype_code'];
		   $mtw_registration_number					=   $applicant_data['registration_number'];
		   $mtw_qr_code								=   $applicant_data['mtw_qr_code']; 
		   $alc_subdiv_code							=   $applicant_data['sub_div_code']; 
		   
		   
	   }
	   
	   if (empty($mtw_registration_number) && empty($mtw_qr_code)){
			   /*** GENERATION OF QR CODE ****/
			   
				$generated_qr_code					= 'MTW-REG'.$application_id.'A3U'.$applicant_user_id.'T'.time();
		   
			   /*** GENERATION OF MTW REGISTRATION NUMBER ***/			
			   
			   
			   $getShortNameSubdivision 			=   custom_user_short_name_fun($subdivision_code);  // ---------come from custom_user module  
			   $getRegistrarCode					=	get_registration_code($area_code);              //-------------miscellaneous module
			   $registrarCode 						=   substr($getRegistrarCode, -2);
			   
			   
			   //$chkdata							=	db_query("select registration_number from l_mtw_registration_master where id='".$application_id."'");	
			   $chkdata							=	db_query("select registration_number from l_mtw_registration_master where id=(select MAX(id) from l_mtw_registration_master where registration_number!='');");
			   
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
						   $reg_query				=	db_query("select  max (NULLIF(substr(registration_number,11,6),'') :: integer) as serial_num  from l_mtw_registration_master where mtw_loc_subdivision='".$alc_subdiv_code."'");
						   
						   $reg_query_result		=	$reg_query->fetchAssoc();
						   
						   $reg_code				=	$reg_query_result['serial_num'];
							 $reg_code_next			= 	$reg_code+1; 
							 $reg_first				=	$getShortNameSubdivision.$registrarCode.'/'.'MTW'.'/';
							 $reg_second				=	str_pad($reg_code_next, 6, "0", STR_PAD_LEFT);
							 $registration_number	=	$reg_first.$reg_second;
						   $certificate_serial_no	=	'C/'.$alc_subdiv_code.'/'.date('Y').'/'.$reg_code_next;	
			   }
			   
			   
			   /*** Update Status of mtw Master Table ***/ 
			   
			   $update_status  					=  db_update('l_mtw_registration_master');
			   $update_status->fields(array('mtw_qr_code' => $generated_qr_code, 'registration_number' =>	$registration_number, 'registration_date' => date("Y-m-d"), 'certificate_serial' => $certificate_serial_no,'expiry_date'=>date("Y").'-12-31', 'reg_mtw_qr_code'=> $generated_qr_code, 'reg_registration_date'=>date("Y-m-d"), 'reg_expire_date'=> date("Y").'-12-31'));
			   $update_status->condition('id',$application_id);
			   $update_status->condition('act_id',$act_id);
			   $update_status->condition('user_id',$applicant_user_id);
			   $update_status->execute();
			   
			   /* Renewal section start*/
			   $dataQuery			=	db_select('l_mtw_registration_master','lmrm');
			   $dataQuery->fields('lmrm',array('registration_date','mtw_registration_certificate','finalfees','application_date'));
			   $dataQuery->condition('id',$application_id);
			   $dataEx 			=	$dataQuery->execute();
			   
			   if($dataEx->rowCount()>0){
				   $dataArr			=	$dataEx->fetchObject();
				   
				   $application_date	=	$dataArr->application_date;
				   $registration_date	= 	$dataArr->registration_date;
				   $previous_fees		=	$dataArr->finalfees;
				   $prvCertificate		=	$dataArr->mtw_registration_certificate;
				   
				   // store data first time in renewal table
				   $upNewalPrvData	=	array(
											   'previous_fees' 		=>	0,
											   'current_fees' 			=>	$previous_fees,
											   'final_fees' 			=>	$previous_fees,
											   'purpose'				=>  'registration',
											   'mtw_registration_certificate'=> trim($prvCertificate),
											   'application_id'	 		=>  $application_id,
											   'application_date'		=> 	$application_date, 		// Applicant apply date
											   'renewal_ref_number' 	=>  'MTW-REG-'.$application_id.'-'.time(),
											   'renewal_date'			=>	NULL,
											   'expiry_date' 			=>  date("Y").'-12-31',
											   'status'				=>	'I'
										   ); 
				   //print_r($insertNewalPrvData);
				   $or = db_or()->condition('status','V')->condition('status','T')->condition('status','S');
				   db_update('l_mtw_registration_renewal')->fields($upNewalPrvData)->condition('application_id',$application_id)->condition($or)->execute();
				   
				   
			   }
			   
			   /* Renewal table end */
			   $remarks_text = 'Congratulations! Certificate is issued by the Registering Authority. You can download it from the dashboard.';
	   }
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


	$remark_by_role			= $fetch_users_role_data['rid'];
	$alc_full_name			= $fetch_users_role_data['fullname'];
	$employee_id			= $fetch_users_role_data['employee_id'];
	
	
	$master_part_mtw_reg = db_select('l_mtw_registration_master', 'lmrm');
	$master_part_mtw_reg->fields('lmrm', array());		
	$master_part_mtw_reg->condition('lmrm.user_id', $applicant_user_id);
	$master_part_mtw_reg->condition('lmrm.id', $application_id);
	$master_part_mtw_reg_result = $master_part_mtw_reg->execute();
	
	if($master_part_mtw_reg_result->rowCount() > 0){
		$content	= $master_part_mtw_reg_result->fetchAssoc();
		$fees 		= $content['mtw_maxworkers'];
		$mtw_qr_code = $content['mtw_qr_code'];	
		$application_date = $content['application_date'];
		$renewal_date = $content['registration_date'];
	}else{
		$fees 		= 0;
		$mtw_qr_code = '';
		$application_date = '';
		$renewal_date = '';
	}
		
	$getPrvValue			= 	db_select('l_mtw_registration_renewal','lmrr');
	$getPrvValue->fields('lmrr',array('previous_fees','renewal_ref_number','id'));
	$getPrvValue->condition('lmrr.application_id',$application_id);
	$getPrvValue->condition('lmrr.purpose','registration','=');
	$getPrvValue->condition('lmrr.status','I','!=');
	$getPrvData	= $getPrvValue->execute();
	if($getPrvData->rowCount() > 0){
		$prvdata			=	$getPrvData->fetchObject();
		$finalPaidAmount 	= 	$prvdata->previous_fees;
		$renewal_ref_number	=	$prvdata->renewal_ref_number;
		$renewal_id			=	$prvdata->id;
	}else{
		$query		= db_select('l_mtw_remark_master', 'rm');
		$query		->fields('rm',array());
		$query		->condition('rm.application_id', $application_id);
		$db_or 		= db_or();
		$db_or		->condition('rm.remark_type', 'V');
		$db_or		->condition('rm.remark_type', 'VA');
		$query		->condition($db_or);
		$query		->range(0,1)	;		
		$query		->orderBy('rm.id','DESC');
		$result 	= $query->execute()->fetchAssoc();
		
		$alc_user_id 			= ($result['remark_by']!='')?$result['remark_by']:$alc_user_id;	
		$renewal_ref_number		= $result['ref_number'];	
		$renewal_id				= $result['renewal_id'];
		$getEmployeeid 			= db_select('l_custom_user_detail','cd')->fields('cd',array('employee_id','fullname'))->condition('usr_id',$alc_user_id)->execute()->fetchAssoc();
		$alc_full_name			= $getEmployeeid['fullname'];	
		$employee_id			= $getEmployeeid['employee_id'];
	}
	
	
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
		'renewal_id'			=> $renewal_id,
		'employee_id'			=> $employee_id
		
	);

	// echo '<pre>';print_r($fieldsApplInfo2);die;


	if($fees <= 5){
		$retFees	=	'10.00';
	}else if($fees > 5 && $fees <= 25  ){
		$retFees	=	'25.00';
	}else if($fees > 25 && $fees <= 50  ){
		$retFees	=	'50.00';
	}else if($fees > 50 && $fees <= 100  ){
		$retFees	=	'100.00';
	}else if($fees > 100 && $fees <= 250  ){
		$retFees	=	'250.00';
	}else if($fees >250 && $fees <= 500){
		$retFees	=	'500.00';
	}else if($fees > 500 && $fees <= 750  ){
		$retFees	=	'750.00';
	}else if($fees > 750 && $fees <= 1000  ){
		$retFees	=	'1000.00';
	}else{
		$retFees	= '0.00';
	}
	
	// checking for all fields will be checked before approve
	
	if($remark_type=="VA" || $remark_type=="V"){
		
		$array2	= array('id', 'status', 'mtw_name','mtw_loc_address','mtw_nature','total_routes','total_route_milage','total_mtw_vehicle','mtw_maxworkers','trade_license_file','article_of_assoc_file','form_one_asses_ses_file','supp_asses_ses_file','other_doc_file','mtw_address_proof_file','finalfees');
		
		$getAllQuery				=	db_select('l_mtw_registration_master_extra','lmrme');
		$getAllQuery->fields('lmrme',array('id'));
		$getAllQuery->condition('application_id',$application_id);
		$getAllQuery->orderby('id','DESC');
		$getData 					= $getAllQuery->execute();
		if($getData->rowCount()>0){
			foreach($getData->fetchAll() as $single){
				$array2[]	= "owner_id_".$single->id;
			}
		}
		$array1	= explode(',',$fieldname);
		$result = array_diff($array2, $array1);
		
			
		/*** Update Status of mtw Master Table ***/ 

		 $update_status  =  db_update('l_mtw_registration_master');
		// if(!empty($signed_form_mtw_file)){
		// 	$update_status->fields(array(
		// 			 'status'						  => $remark_type,
		// 			 'mtw_registration_certificate' => trim($signed_form_mtw_file),
		// 	));
		// } else{
			
			$update_status->fields(array('status'=> $remark_type, 'reg_status' => $remark_type , 'reg_finalfees' => $retFees));
		//}
		$update_status->condition('id',$application_id);
		$update_status->condition('user_id',$applicant_user_id);
		$update_status->execute();
		
		db_insert('l_mtw_remark_master')->fields($fieldsApplInfo2)->execute(); 	
			
		
		
		$insertNewalPrvData	=	array(
			'previous_fees' 		=> 0,
			'current_fees' 			=> $retFees,
			'fine_fees'				=> 0,
			'paid_fees'				=> ($retFees+0),
			'final_fees' 			=> (0+$retFees),
			'purpose'				=> 'registration',
			//'mtw_registration_certificate'=> trim($prvCertificate),
			'application_id'	 	=> $application_id,
			'application_date'		=> 	$application_date, 		// Applicant apply date
			'renewal_date'			=> NULL,
			'expiry_date' 			=> date("Y").'-12-31',
			'status'				=> 'V',
			'user_id'				=> $applicant_user_id
		); 
				//print_r($insertNewalPrvData);
				
				// check already inserted or not
				
		$chk = db_query("select id,renewal_ref_number from l_mtw_registration_renewal where application_id = ".$application_id." and purpose = 'registration'");
		if($chk->rowCount() > 0){
			$renewal_data = $chk->fetchObject();
			$renewal_insert_id = $renewal_data->id;
			$renewal_ref_number = $renewal_data->renewal_ref_number;			
			db_update('l_mtw_registration_renewal')->fields($insertNewalPrvData+array('status'=>$remark_type))->condition('application_id',$application_id)->condition('purpose','registration')->execute();
		}else{
			$renewal_ref_number = 'MTW-REG-'.$application_id.'-'.time();
			$insertNewalPrvData['renewal_ref_number'] = $renewal_ref_number; 
			$renewal_insert_id = db_insert('l_mtw_registration_renewal')->fields($insertNewalPrvData)->execute();
		}
		if($renewal_insert_id !=0 && $renewal_insert_id != '' ){
			db_update('l_mtw_remark_master')->fields(array('ref_number' => $renewal_ref_number,'renewal_id'=> $renewal_insert_id ))->condition('application_id',$application_id)->execute();
		}
		
		$message = 'Remark has been successfully saved.';
		drupal_set_message(t($message));
		
	}else{
		/*** Update Status of mtw Master Table ***/ 
		
		$update_status  =  db_update('l_mtw_registration_master');
		// if(!empty($signed_form_mtw_file)){
		// 	$update_status->fields(array(
		// 			 'status'						  => $remark_type,
		// 			 'mtw_registration_certificate' => trim($signed_form_mtw_file),
		// 	));
		// }else{
			$update_status->fields(array('status' => $remark_type,'reg_status' => $remark_type, 'reg_finalfees' => $retFees));
		// }
		$update_status->condition('id',$application_id);
		$update_status->condition('user_id',$applicant_user_id);
		$update_status->execute();
				
		if($remark_type == 'I'){
			$or = db_or()->condition('status','V')->condition('status','T');
			$qu = db_update('l_mtw_registration_renewal')->fields(array('renewal_date' =>date('Y-m-d H:i:s'),'mtw_registration_certificate'=>trim($signed_form_mtw_file),'mtw_qr_code'=> $mtw_qr_code,'status'=>'I'))->condition('application_id',$application_id)->condition($or)->execute();

			db_update('users')->fields(array('status'=>1))->condition('status',0)->condition('uid',$applicant_user_id)->execute(); 
			
		}
		
		db_insert('l_mtw_remark_master')->fields($fieldsApplInfo2)->execute(); 	
			
		$message = 'Remark has been successfully saved.';
		drupal_set_message(t($message));
		
	}
	// send sms start
	
	if($remark_type == 'B'){	
		$msg = 'Your CLRA application is sent back for rectification by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';
	}else if($remark_type == 'V'){	
		$msg = 'Your CLRA application is approved for fees submission by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';
	}else if($remark_type == 'I'){	
		$msg = 'Congratulations! Your CLRA registration certificate is issued by ALC(Govt. of WB). Login to download the certificate from dashboard (wblc.gov.in)';
	}else if($remark_type == 'R'){	
		$msg = 'Your CLRA application is rejected by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';	
	}else if($remark_type == 'VA'){	
		$msg = 'Your CLRA application is approved by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';		
	}
	$applicant_mobile_no = get_mobile_number($applicant_user_id); 
	if(!empty($applicant_mobile_no)){ 
		send_sms_for_user_alert($applicant_mobile_no, $msg);		
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
	 			// $applicant_mailto  = get_email($applicant_user_id);
				// $subject = 'Welcome to Labour Commissionerate, Govt. Of West Bengal';
				// if(!empty($applicant_mailto)){
				// 	send_mail_for_user_alert($applicant_mailto, $subject, $msg);
				// }  
	$message = drupal_set_message_custom(t('Registration Certificate has been successfully generated.'));//-------------miscellaneous module
	$form_state['values']['hidden_action_message'] = $message;
    $form_state['rebuild']				= TRUE;
	
}


function generation_mtw_registration_number($form, &$form_state){
	
	 $val  					= $form_state['values']; 
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
	
	if($get_mtw_application_result-> rowCount() > 0 ){
		
		$applicant_data 							= 	$get_mtw_application_result->fetchAssoc();
		$subdivision_code							=   $applicant_data['mtw_loc_subdivision'];
		$area_code									=   $applicant_data['mtw_loc_areatype_code'];
		$mtw_registration_number					=   $applicant_data['registration_number'];
		$mtw_qr_code								=   $applicant_data['mtw_qr_code']; 
		$alc_subdiv_code							=   $applicant_data['sub_div_code']; 
		
		
	}
	
	if (empty($mtw_registration_number) && empty($mtw_qr_code)){
			/*** GENERATION OF QR CODE ****/
			
			 $generated_qr_code					= 'MTW-REG'.$application_id.'A3U'.$applicant_user_id.'T'.time();
		
			/*** GENERATION OF MTW REGISTRATION NUMBER ***/			
			
			
			$getShortNameSubdivision 			=   custom_user_short_name_fun($subdivision_code);  // ---------come from custom_user module  
			$getRegistrarCode					=	get_registration_code($area_code);              //-------------miscellaneous module
			$registrarCode 						=   substr($getRegistrarCode, -2);
			
			
			//$chkdata							=	db_query("select registration_number from l_mtw_registration_master where id='".$application_id."'");	
			$chkdata							=	db_query("select registration_number from l_mtw_registration_master where id=(select MAX(id) from l_mtw_registration_master where registration_number!='');");
			
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
						$reg_query				=	db_query("select  max (NULLIF(substr(registration_number,11,6),'') :: integer) as serial_num  from l_mtw_registration_master where mtw_loc_subdivision='".$alc_subdiv_code."'");
						
						$reg_query_result		=	$reg_query->fetchAssoc();
						
						$reg_code				=	$reg_query_result['serial_num'];
					  	$reg_code_next			= 	$reg_code+1; 
					  	$reg_first				=	$getShortNameSubdivision.$registrarCode.'/'.'MTW'.'/';
					  	$reg_second				=	str_pad($reg_code_next, 6, "0", STR_PAD_LEFT);
					  	$registration_number	=	$reg_first.$reg_second;
						$certificate_serial_no	=	'C/'.$alc_subdiv_code.'/'.date('Y').'/'.$reg_code_next;	
			}
			
			
			/*** Update Status of mtw Master Table ***/ 
			
			/*$update_status  					=  db_update('l_mtw_registration_master');
			$update_status->fields(array('mtw_qr_code' => $generated_qr_code, 'registration_number' =>	$registration_number, 'registration_date' => date("Y-m-d"), 'certificate_serial' => $certificate_serial_no,'expiry_date'=>date("Y").'-12-31'));
			$update_status->condition('id',$application_id);
			$update_status->condition('act_id',$act_id);
			$update_status->condition('user_id',$applicant_user_id);
			$update_status->execute();*/
			
			/* Renewal section start*/
			$dataQuery			=	db_select('l_mtw_registration_master','lmrm');
			$dataQuery->fields('lmrm',array('registration_date','mtw_registration_certificate','finalfees','application_date'));
			$dataQuery->condition('id',$application_id);
			$dataEx 			=	$dataQuery->execute()->fetchObject();
			
			if($dataEx->rowCount()>0){ 
				$dataArr			=	$dataEx->fetchObject(); 
				
				$application_date	=	$dataArr->application_date;
				$registration_date	= 	$dataArr->registration_date;
				$previous_fees		=	$dataArr->finalfees;
				$prvCertificate		=	$dataArr->mtw_registration_certificate;
				
				// store data first time in renewal table
				$upNewalPrvData	=	array(
											'previous_fees' 		=>	0,
											'current_fees' 			=>	$previous_fees,
											'final_fees' 			=>	$previous_fees,
											'purpose'				=>  'registration',
											'mtw_registration_certificate'=> trim($prvCertificate),
											'application_id'	 	=>  $application_id,
											'application_date'		=> 	$application_date, 		// Applicant apply date
											'renewal_ref_number' 	=>  'MTW-REG-'.$application_id.'-'.time(),
											'renewal_date'			=>	NULL,
											'expiry_date' 			=>  date("Y").'-12-31',
											'status'				=>	'I'
										); 
				//print_r($insertNewalPrvData);
				
				
				$or = db_or()->condition('status','V')->condition('status','T')->condition('status','S');
				db_update('l_mtw_registration_renewal')->fields($upNewalPrvData)->condition('application_id',$application_id)->condition($or)->execute();
				//db_insert('l_mtw_registration_renewal')->fields($insertNewalPrvData)->execute();
			}
			
			/* Renewal table end */
			
			
			$message							= drupal_set_message_custom(t('Registration Certificate has been successfully generated. Please Sign and Upload Certificate to complete this process'));//-------------miscellaneous module

 			$form_state['values']['hidden_action_message'] = $message;
			
			$form_state['rebuild']				= TRUE;
	
	}
}

function re_generation_mtw_registration_number($form, &$form_state){
	// 
	//	
	//	 $val  					= $form_state['values']; 
	//	
	//	 $applicant_user_id		= $val['applicant_user_id'];
	//	 $application_id		= $val['application_id'];
	//	 $act_id				= 3;
	//	 
	//	
	//	 
	//	 
	//	 ** Get Applicant Block Code & Statuses & ALC Details ***
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
	//	if (empty($mtw_registration_number) && empty($mtw_qr_code)){
	//			** GENERATION OF QR CODE ***
	//			
	//			 $generated_qr_code					= 'MTW-REG'.$application_id.'A3U'.$applicant_user_id.'T'.time();
	//		
	//			** GENERATION OF MTW REGISTRATION NUMBER **			
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
	//			
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
	//			echo  'Mloumita'; die;
	//			** Update Status of mtw Master Table ** 
	//			
	//			$update_status  					=  db_update('l_mtw_registration_master');
	//			$update_status->fields(array('mtw_qr_code' => $generated_qr_code, 'registration_date' => date("Y-m-d")));
	//			$update_status->condition('id',$application_id);
	//			$update_status->condition('act_id',$act_id);
	//			$update_status->condition('user_id',$applicant_user_id);
	//			$update_status->execute();
	//			
	//			 Renewal section start
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
	//				 store data first time in renewal table
	//				$updateRenewalPrvData	=	array(
	//											previous_fees' 		=>	0,
	//											current_fees' 			=>	$previous_fees,
	//											final_fees' 			=>	$previous_fees,
	//											purpose'				=>  'registration',
	//											mtw_registration_certificate'=> trim($prvCertificate),
	//											application_id'	 	=>  $application_id,
	//											application_date'		=> 	$application_date, 		// Applicant apply date
	//											renewal_ref_number' 	=>  'MTW-REG-'.$application_id.'-'.time(),
	//											renewal_date'			=>	NULL,
	//											expiry_date' 			=>  date("Y").'-12-31',
	//											status'				=>	'I'
	//										);
	//										
	//				$or = db_or()->condition('status','V')->condition('status','T')->condition('status','S');
	//				db_update('l_mtw_registration_renewal')->fields($upNewalPrvData)->condition('application_id',$application_id)->condition($or)->execute();
	//				
	//										 
	//				db_update('l_mtw_registration_renewal')->fields($updateRenewalPrvData)->condition('application_id',$application_id)->condition('status','0')->execute();
	//			}
	//			
	//			 Renewal table end */
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
	//
}


function get_mtw_remark_details($application_id,$applicant_user_id){
	
	global $base_root, $base_path, $user;
	
	$user_id = $user->uid;
	
	$counter					= 0;
	
	$header = array(
		array('data' => 'Sl. No', 'field' => 'slno'),
		array('data' => 'Date / Time', 'field' => 'date'),
		array('data' => 'Remark', 'field' => 'remrk'),
		array('data' => 'Remark Status', 'field' => 'remark type'),
		array('data' => 'Remark By', 'field' => 'licenseno'),
		array('data' => 'Action')
	);
	
	$get_remark = db_select('l_mtw_remark_master', 'lbrm');
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
			$remark_by		= $row->remark_by_name.'('.$row->name.')';
			
			if(trim($row->remark_type== 'I' )){
				$STATUS_IMG = '<span class="issued">Issued</span>';		
			}else if(trim($row->remark_type) == 'B'){ 
				$STATUS_IMG =  '<span class="backed">Back to Applicant by Officer</span>';
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
				$STATUS_IMG =  '<span class="approved">Renewal Applied</span>'; 
			}
			
			
			if (!empty($row->call_time_applicant)){
				$call_time_applicant =  ' ( '.date('dS M, Y - g:i a',$row->call_time_applicant).' )';
			}else{
				$call_time_applicant ='';
			}
			
			
			// get data for checking renewal or amend
			$chkRnAm	=	db_query("SELECT id, is_renew, is_amend,deemed FROM l_mtw_registration_master WHERE id = ".$application_id."")->fetchObject();
			if(!empty($chkRnAm)){
				$renewal_status = $chkRnAm->is_renew ;
				$amend_status 	= $chkRnAm->is_amend;
				$deemed 		= $chkRnAm->deemed;
			}
			
			// get latest remark given by session user
			$getMaxData				= db_query("SELECT MAX(id) AS MAXID FROM l_mtw_remark_master WHERE is_active=1 and application_id = ".$application_id."")->fetchObject();
			$maxId					= $getMaxData->maxid;
			if($maxId>0){
				if(($row->remark_by==$user->uid) &&($maxId==$row->id) && $renewal_status==0 && $amend_status==0){
					if($row->remark_type == 'I'){
						$delete			= '<div id="delBtn" class="delete-btn disable-del">'.t('Delete').'</div>';
					}else{
						$url 			= 'remarks-delete/'.encryption_decryption_fun('encrypt',$row->id).'/'.encryption_decryption_fun('encrypt',$application_id).'/'.encryption_decryption_fun('encrypt',$applicant_user_id);			
						$delete			= l('<div id="delBtn" class="delete-btn active-del">'.t('Delete').'</div>',$url,array('html' => TRUE));	
					}
				}else{
					$delete			= '<div id="delBtn" class="delete-btn disable-del">'.t('Delete').'</div>';
				}
			}
				$rows[] 	= array(
									array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
									array('data' => date('dS M, Y - g:i a', $row->remark_date), 'align' => 'left', 'class' => array('odd')),
									array('data' => $row->remark_text.$call_time_applicant, 'align' => 'left', 'class' => array('odd')),
									array('data' => $STATUS_IMG, 'align' => 'left', 'class' => array('odd')),
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
	$commands[] = ajax_command_replace('#call_applicant_block', drupal_render($form['view_mtw_registration_form_alc']['call_applicant']));
	$commands[] = ajax_command_replace('#download_link_block', drupal_render($form['view_mtw_registration_form_alc']['download_link']));
	$commands[] = ajax_command_replace('#mtw_signed_certificate_block', drupal_render($form['view_mtw_registration_form_alc']['mtw_signed_certificate']));
	$commands[] = ajax_command_replace('#generate_certificate_block', drupal_render($form['view_mtw_registration_form_alc']['generate_certificate']));
	$commands[] = ajax_command_replace('#re_generate_certificate_block', drupal_render($form['view_mtw_registration_form_alc']['re_generate_certificate']));
	$commands[] = ajax_command_replace('#remarks_text_block', drupal_render($form['view_mtw_registration_form_alc']['remarks_text']));
	$commands[] = ajax_command_replace('#submit_block', drupal_render($form['view_mtw_registration_form_alc']['submit']));
	return array('#type' => 'ajax', '#commands' => $commands);
	
}

	

