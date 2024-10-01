<?php
function alcviewbacklogcontractorlicenserenewal($form, &$form_state, $licenserenewalid='' , $serial_no_fromv='' , $contractor_particularid='', $licenseid_hash=''){
	global $base_root, $base_path, $user;
	$is_backlog_renewal_date_exists = 0;
	$uri3							= explode('#',$licenseid_hash);
	$licenseid						= $uri3[0];
	$uriID							= $uri3[1];
	
	if(empty($licenseid)){
		$licenseid= $licenseid_hash;
		$uriID	 ="";
	}
    $license_renewal_id 			= encryption_decryption_fun('decrypt', $licenserenewalid);
	$serial_no_from_v 				= encryption_decryption_fun('decrypt', $serial_no_fromv);
	$contractor_particular_id		= encryption_decryption_fun('decrypt', $contractor_particularid);
	$license_id						= encryption_decryption_fun('decrypt', $licenseid);	
	

	
	$getRenewalTableInfo			= getRenewalTable($serial_no_from_v,$license_renewal_id,$contractor_particular_id,$license_id);
	 $renewal_data					=	fetch_all_data_contractor_license_apply_renweal($license_contractor_id,$contractor_user_id);
	$get_remark 					= 	db_select('l_remark_license', 'lrl');
	$get_remark						->	fields('lrl', array('remark_field_title'));
	$get_remark						->	condition('lrl.particular_id', $license_renewal_id);
	$get_remark						->	condition('lrl.flag','R');
	$get_remark						->	where('((lrl.remark_by_role = 4) OR (lrl.remark_by_role = 7))');
	$get_remark						->	orderBy('lrl.id','DESC');
	$get_remark						->	range(0, 1);
	$get_remark_result_fields 		 = 	$get_remark->execute()->fetchObject()->remark_field_title;
	
	$get_status 					= 	db_select('l_contractor_license_apply_renweal', 'renewal');
	$get_status						->	fields('renewal', array('renewal_appliaction_status','remark_by_userid','renewal_certificate_id','license_reneal_fees','backlog_renewal_date','recommand_date_expire','created_by'))->condition('renewal.action','R','=');
	$get_status						->	condition('renewal.id', $license_renewal_id);

	$status_renewal 		 		= 	$get_status->execute()->fetchObject();
	$get_status_renewal				=	$status_renewal->renewal_appliaction_status;
	$renewal_certificate_id 		= 	$status_renewal->renewal_certificate_id;
	$reneal_fees 					= 	$status_renewal->license_reneal_fees;	
	$backlog_renewal_date 			= 	$status_renewal->backlog_renewal_date;	
	$recommended_date_expire 		= 	$status_renewal->recommand_date_expire;	
	 $created_by 				= 	$status_renewal->created_by;	

	if(!empty($renewal_certificate_id)){
		$upload_certificates_id 		= db_select('file_managed','fm');
		$upload_certificates_id			-> fields('fm',array());
		$upload_certificates_id			-> condition('fid',$renewal_certificate_id);
		
		$upload_certificates_file		= $upload_certificates_id	-> execute()->fetchAssoc();
						
						//exit;
		  if(!empty($upload_certificates_file)){ 
			  $url									= explode('//',$upload_certificates_file['uri']);
			  $upload_certificates_file_url			= $url[1];
			  $upload_certificates_file_name		= $upload_certificates_file['filename'];
		  }
		}
		
		
		$get_remark = db_select('l_remark_license', 'remark_license');
		$get_remark->fields('remark_license', array('remark_field_title'));
		$get_remark->condition('remark_license.remark_to', $serial_no_from_v);
		$get_remark->condition('remark_license.particular_id', $license_renewal_id);
		$get_remark->where("((remark_license.remark_by_role = 4) OR (remark_license.remark_by_role = 7))");
		$get_remark->orderBy('remark_license.id','DESC');
		$get_remark->range(0, 1);
		$get_remark_result = $get_remark->execute();

		

		$editable_data = $get_remark_result->fetchAssoc();
		
		$remark_field_text_arr = explode(',', $editable_data['remark_field_title']);
	    
		foreach($remark_field_text_arr as $row){
			if($row == "backlog_renewal_date" && !empty($recommended_date_expire)){
				$is_backlog_renewal_date_exists = 1;
				break;
			}
			if($row != "backlog_renewal_date" && empty($recommended_date_expire)){
				$is_backlog_renewal_date_exists = 0;
				break;
			}
			if($row != "backlog_renewal_date" && !empty($recommended_date_expire)){
				$is_backlog_renewal_date_exists = 1;				
				break;
			}
		}
		
		
			
			
		$get_remark_type 				= 	db_select('l_remark_license', 'lrl');
		$get_remark_type				->	fields('lrl', array('remark_type'));
		$get_remark_type				->	condition('lrl.particular_id', $license_renewal_id);
		$get_remark_type				->	condition('lrl.flag','R');
		$get_remark_type				->	orderBy('lrl.id','DESC');
		$get_remark_type				->	range(0, 1);
		$remark_type 		 			= 	$get_remark_type->execute()->fetchObject()->remark_type;	
		
		if(!empty($remark_type)){
		if($remark_type == 'B'){
			$current_status = 'CURRENT STATUS: Application has been BACKED to Applicant for rectification';
		}
		if($remark_type == 'FW'){
			$current_status = 'CURRENT STATUS: Application has been FORWARDED TO ALC';
		}
		if($remark_type == 'A'){
			$current_status = 'CURRENT STATUS: Application has been VERIFIED & GIVEN APPROVAL FOR FEES SUBMISSION';
		}
		if($remark_type == 'C'){
			$current_status = 'CURRENT STATUS: Applicant has been CALLED BY ALC';
		}
		if($remark_type == 'I'){
			$current_status = 'CURRENT STATUS: LICENCE CERTIFICATE has been ISSUED';
		}
		if($remark_type == 'P'){
			$current_status = 'CURRENT STATUS: Payment Successful for this Application';
		}
		if($remark_type == 'U'){
			$current_status = 'CURRENT STATUS: FORM-VII is uploaded successfully';
		}
		if($remark_type == 'R'){
			$current_status = 'CURRENT STATUS: Application has been REJECTED';
		}
		
		if($remark_type == 'BI'){
			$current_status = 'CURRENT STATUS: Application has been BACKED TO INSPECTOR';
		}		
	}
	
	$form['backlog_renewal_alc']['mark_up'] 	= array(
							'#type' 				=>	'markup',
			);
				
	$form['backlog_renewal_alc']['mark_up_renewal_show'] = array(
				  			'#type' 			=> 	'markup',
				  			'#markup' 			=> 	$getRenewalTableInfo,
				  			'#suffix' 			=> '', 
			   );
			   
			 
			   
			   
	$form['backlog_renewal_alc']['show_status'] = array(
									  '#prefix'				=> '<div class="comments" align="center">',
									  '#suffix'				=> '</div>',
									  '#type' 				=> 'markup',
									  '#markup'				=> $current_status
									);
	
	$form['backlog_renewal_alc']['formV']	= array(		
							'#type'				=> 'hidden',
							'#default_value' 	=> !empty($serial_no_fromv) ? $serial_no_fromv : '',
				);
				
	$form['backlog_renewal_alc']['renewal_id']	= array(		
							'#type'				=> 'hidden',
							'#default_value' 	=> !empty($licenserenewalid) ? $licenserenewalid : '',
				);
				
	$form['backlog_renewal_alc']['remark_alc_text_renewal']	= array(		
								'#type'				=> 'hidden',
								'#default_value' 	=> !empty($get_remark_result_fields) ? $get_remark_result_fields : '',
				);
	$form['backlog_renewal_alc']['renewal_fees']	= array(		
								'#type'				=> 'hidden',
								'#default_value' 	=> !empty($reneal_fees) ? $reneal_fees : '',
				);
	$form['backlog_renewal_alc']['contractor_particularid']	= array(		
								'#type'				=> 'hidden',
								'#default_value' 	=> !empty($contractor_particularid) ? $contractor_particularid : '',
				);
				
	$form['backlog_renewal_alc']['licenseid']	= array(		
								'#type'				=> 'hidden',
								'#default_value' 	=> !empty($licenseid) ? $licenseid : '',
				);
				
				
	$form['backlog_renewal_alc']['backlog_renewal_date']	= array(		
								'#type'				=> 'hidden',
								'#default_value' 	=> !empty($backlog_renewal_date) ? $backlog_renewal_date : '',
				);
				
				
	
//print_r($get_status_renewal);die();			
	if($get_status_renewal!='R'){			
	if($get_status_renewal!='P'){														  
	if($get_status_renewal!='I'){
	if($get_status_renewal!='A'){
	if($get_status_renewal!='B'){
	if(!empty($uriID)){
		if($uriID == "HI"){
		
				$message= drupal_set_message_custom(t('Remark is successfully deleted'));
		}if($uriID == "hello"){
				$message= drupal_set_message_custom(t('Remark is successfully saved'));
		}if($uriID == "fromv"){
			$message= drupal_set_message_custom(t('Please verify the <strong>"Form VII" by checking  check box in <strong>"Is Verified?"</strong> column then you can proceed for Issue Certificate of application remark.'),'error');
		}
				$form['backlog_renewal_alc']['MARKUP_1']	= array(
										'#prefix' 			=> '<div id="remark_view">',		
										'#type'				=> 'markup',
										'#markup'			=>	$message,
										 '#suffix' 			=> '</div>'
										
						);
		}
			 
		
		$form['backlog_renewal_alc']['remark_type']	= array(
					'#prefix' 				=> '<div class="content"><div class="sky-form"  style="width:100%;"><header><h3><b>ACTIONS AND REMARK</b></h3></header><fieldset><section><label class="select">',	
					'#suffix' 				=> '<i></i></label></section>',
					'#title'				=> 'Please Select Actions',
					'#type'					=> 'select',
					'#required' 			=>  TRUE,
					'#options'				=>  renewal_actions_dropdown_by_alc($get_status_renewal),
					'#ajax'					=>  array(
														'event' 			=> 'change',
														'effect' 			=> 'fade',			
														'callback'			=> 'action_field',
														'progress'			=> '',
													),
							);
			
		$form['backlog_renewal_alc']['recomented_date'] 	= array(
								  '#prefix' 			=> '<div id="alc_recomented_date">',
								  '#suffix' 			=> '</div>'
							);
		$form['backlog_renewal_alc']['download_certificates'] 		= array(
					  '#prefix' 			=> '<div id="download_certificates">',
					  '#suffix' 			=> '</div>'
				);

	     $form['backlog_renewal_alc']['certificates'] 				= array(
					  '#prefix' 			=> '<div id="edit-certificates-ajax-wrapper">',
					  '#suffix' 			=> '</div>'
					);	
		 $form['backlog_renewal_alc']['check_declaration'] 				= array(
					  '#prefix' 			=> '<div id="check_declaration_id">',
					  '#suffix' 			=> '</div>'
					);		
		$form['backlog_renewal_alc']['remarks_text'] 		= array(
					  '#prefix' 			=> '<div id="renewal_remarks_text_div">',
					  '#suffix' 			=> '</div>'
				);	
	   $form['backlog_renewal_alc']['remarks_text']	= array(
									'#prefix' 				=> '<div id="renewal_remarks_text_div"><section><label class="textarea">',
									'#suffix' 				=> '</label></section></div>',
									'#title'				=> 'Remarks',
									'#type'					=> 'textarea',
									'#rows'					=>  2,
									'#required' 			=>  TRUE,
							);	
					
				
		
	 
		$form['backlog_renewal_alc']['call_applicant'] 	= array(
								  '#prefix' 			=> '<div id="renewal_call_applicants">',
								  '#suffix' 			=> '</div>'
							);
		
		
		
							
		
				
		
	
		$form['backlog_renewal_alc']['hidden_action'] 		= array(
					  '#prefix' 			=> '<div id="renewal_hidden_action_div">',
					  '#suffix' 			=> '</div>'
				);
															
															
	   
															
															
		$value_action_type		= isset($form_state['values']['remark_type']) ? $form_state['values']['remark_type'] : '';
		
		if(!empty($value_action_type) && trim($value_action_type)== 'C'){
			
					drupal_add_css(drupal_get_path('module', 'alc').'/js/jquery.datetimepicker.css');
					drupal_add_js(drupal_get_path('module', 'alc').'/js/jquery.datetimepicker.js');	
					drupal_add_js(drupal_get_path('module', 'alc').'/js/custom_datepicker.js');
					$form['backlog_renewal_alc']['recomented_date'] 	= array(
								  '#prefix' 			=> '<div id="alc_recomented_date">',
								  '#suffix' 			=> '</div>'
							);	
																										
					$form['backlog_renewal_alc']['call_applicant']	 = array(
												'#prefix' 				=> '<div id="renewal_call_applicants"><section><label class="input">',
												'#suffix' 				=> '</label></section></div>',
												'#title'				=> 'Date Time',
												'#type'					=> 'textfield',
												'#attributes' 			=> array('id' => 'calltime',  'placeholder' => 'Enter Date and Time', 'onclick' => 'showdate()','onkeyup' => 'showdate()', 'onblur' => 'showdate()',),
												'#required' 			=>  TRUE,
											);	
											
				$form['backlog_renewal_alc']['remarks_text']	= array(
									'#prefix' 				=> '<div id="renewal_remarks_text_div"><section><label class="textarea">',
									'#suffix' 				=> '</label></section></div>',
									'#title'				=> 'Remarks',
									'#type'					=> 'textarea',
									'#rows'					=>  2,
									'#required' 			=>  TRUE,
							);	
																										
																									
		}if((!empty($value_action_type) )&& trim($value_action_type)== 'I'){
			
			if($is_backlog_renewal_date_exists == 0 || empty($is_backlog_renewal_date_exists)){
			 	$form['backlog_renewal_alc']['recomented_date'] = array(
											'#prefix'				=>'<div id="alc_recomented_date"><section><label class="date">',
											'#suffix'				=>'</label></section></div>',
											'#title'				=>'Verify and recommend date of expiry of the previous license',
											'#type' 				=> 'date',
											//'#element_validate' 	=> 	array('contractor_dob_validate'),
											'#default_value' 		=>	!empty($recommended_date_expire) ? $recommended_date_expire : '',
											'#required'				=>	TRUE,
										);
				$form['backlog_renewal_alc']['remarks_text'] 		= array(
					  						'#prefix' 				=> '<div id="renewal_remarks_text_div">',
					  						'#suffix' 				=> '</div>'
										);
				$form['backlog_renewal_alc']['check_declaration'] 	= array(
					  						'#prefix' 				=> '<div id="check_declaration_id"><span style="color:#FF0000">',
					  						'#suffix' 				=> '</span></div>',
					  						'#type' 				=> 'checkbox',
					   						'#title' 				=> t('<strong>Declaration: Please be informed that the date being recommended by you shall be full and final and the renewal of license will be generated from the date as recommended, once the renewal certificate is issued no changes can not be done further.</strong>'),
								 			'#required' 			=> TRUE,
											'#attributes'			=> array('onclick' => 'return checkFinalAppCheckBox()'),
					  
										);		
				$form['backlog_renewal_alc']['download_certificates']	= array(
											  '#prefix' 			=> '<div id="download_certificates">',
											  '#suffix' 			=> '</div>'
										);
				$form['backlog_renewal_alc']['is_recommanden_date']	= array(		
								'#type'				=> 'hidden',
								'#default_value' 	=> '1',
				);
				}
		
		    if($is_backlog_renewal_date_exists == 1){
			          $form['backlog_renewal_alc']['download_certificates']		= array(
								'#prefix'		=> '<div id="download_certificates"><section>',
								'#markup'     	=> l('Download Certificate of Renewal of Licence','renewal-certificates/'.$serial_no_fromv.'/'.$contractor_particularid.'/'.$licenseid.'/'.$licenserenewalid, array('attributes' => array('target' => '_blank'))),
								'#suffix' 		=> '</section></div>',
																									
					     );	
					 $uri='public://upload/alc_upload_certificates_renewal_license/';
			         $target_file=$uri.$value_frm_v;
			 
			 if (!file_exists($target_file)) {
				 if( mkdir($target_file, 0777, true)){
			
		 		 }
			}													
																								
																									
																									
				$form['backlog_renewal_alc']['certificates'] 	 = array(
											'#type' 				=> 'managed_file',
											'#title'				=>	'Upload Signed Certificate of Renewal of Licence :-',
											'#upload_validators' 	=> array('file_validate_extensions' => array('pdf')),
											'#upload_location' 		=> $target_file,
											'#process' 				=> array('import_my_file_element_process'),
											'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array(''),'file_validate_size' => array(50*1024*1024)),
											'#prefix'				=> '<div id="edit-certificates-ajax-wrapper"><section><label class="input input-file">',
											'#suffix' 				=> '</label></section></div>',
											'#required'		    	=> 'TRUE'
																  
										  );
										  
										  
				$form['backlog_renewal_alc']['remarks_text']	= array(
									'#prefix' 				=> '<div id="renewal_remarks_text_div"><section><label class="textarea">',
									'#suffix' 				=> '</label></section></div>',
									'#title'				=> 'Remarks',
									'#type'					=> 'textarea',
									'#rows'					=>  2,
									'#required' 			=>  TRUE,
							);
					
				
										  
										  
			
		}
	
			
			
			
		
		
		}
		
		if(!empty($value_action_type) && trim($value_action_type)== 'B'){
				if(empty($recommended_date_expire)){
				$form['backlog_renewal_alc']['recomented_date'] = array(
											'#prefix'				=>'<div id="alc_recomented_date"><section><label class="date">',
											'#suffix'				=>'</label></section></div>',
											'#title'				=>'Verify and recommend date of expiry of the previous license',
											'#type' 				=> 'date',
										//	'#element_validate' 	=> 	array('contractor_dob_validate'),
											
											'#required'				=>	TRUE,
											
								);
		  }else{
				$form['backlog_renewal_alc']['recomented_date'] = array(
											'#prefix'				=>'<div id="alc_recomented_date"><section><label class="date">',
											'#suffix'				=>'</label></section></div>',
											'#title'				=>'Verify and recommend date of expiry of the previous license',
											'#type' 				=> 'date',
											//'#element_validate' 	=> 	array('contractor_dob_validate'),
											'#default_value' 		=> array(
																			   'month' => format_date(strtotime($recommended_date_expire), 'custom',  'n'),
																			   'day' => format_date(strtotime($recommended_date_expire), 'custom',  'j'),
																			   'year' => format_date(strtotime($recommended_date_expire), 'custom',  'Y'),
																			),
											'#required'				=>	TRUE,
											
								);
				
		  }
			
			$form['backlog_renewal_alc']['remarks_text']	= array(
									'#prefix' 				=> '<div id="renewal_remarks_text_div"><section><label class="textarea">',
									'#suffix' 				=> '</label></section></div>',
									'#title'				=> 'Remarks',
									'#type'					=> 'textarea',
									'#rows'					=>  2,
									'#required' 			=>  TRUE,
							);
		}if(!empty($value_action_type) && trim($value_action_type)== 'BI'){
			$form['backlog_renewal_alc']['recomented_date'] 	= array(
								  '#prefix' 			=> '<div id="alc_recomented_date">',
								  '#suffix' 			=> '</div>'
							);	
			
				$form['backlog_renewal_alc']['remarks_text']	= array(
									'#prefix' 				=> '<div id="renewal_remarks_text_div"><section><label class="textarea">',
									'#suffix' 				=> '</label></section></div>',
									'#title'				=> 'Remarks',
									'#type'					=> 'textarea',
									'#rows'					=>  2,
									'#required' 			=>  TRUE,
							);
		}if(!empty($value_action_type) && trim($value_action_type)== 'R'){
			$form['backlog_renewal_alc']['recomented_date'] 	= array(
								  '#prefix' 			=> '<div id="alc_recomented_date">',
								  '#suffix' 			=> '</div>'
							);	
			
				$form['backlog_renewal_alc']['remarks_text']	= array(
									'#prefix' 				=> '<div id="renewal_remarks_text_div"><section><label class="textarea">',
									'#suffix' 				=> '</label></section></div>',
									'#title'				=> 'Remarks',
									'#type'					=> 'textarea',
									'#rows'					=>  2,
									'#required' 			=>  TRUE,
							);
		}if(!empty($value_action_type) && trim($value_action_type)== 'P'){
			$form['backlog_renewal_alc']['recomented_date'] 	= array(
								  '#prefix' 			=> '<div id="alc_recomented_date">',
								  '#suffix' 			=> '</div>'
							);
							
							
			
					
			}
			if(!empty($value_action_type) && trim($value_action_type)== 'A'){
				if(empty($recommended_date_expire)){
				$form['backlog_renewal_alc']['recomented_date'] = array(
											'#prefix'				=>'<div id="alc_recomented_date"><section><label class="date">',
											'#suffix'				=>'</label></section></div>',
											'#title'				=>'Verify and recommend date of expiry of the previous license',
											'#type' 				=> 'date',
											//'#element_validate' 	=> 	array('contractor_dob_validate'),
											
											'#required'				=>	TRUE,
											
								);
		  }else{
				$form['backlog_renewal_alc']['recomented_date'] = array(
											'#prefix'				=>'<div id="alc_recomented_date"><section><label class="date">',
											'#suffix'				=>'</label></section></div>',
											'#title'				=>'Verify and recommend date of expiry of the previous license',
											'#type' 				=> 'date',
											//'#element_validate' 	=> 	array('contractor_dob_validate'),
											'#default_value' 		=> array(
																			   'month' => format_date(strtotime($recommended_date_expire), 'custom',  'n'),
																			   'day' => format_date(strtotime($recommended_date_expire), 'custom',  'j'),
																			   'year' => format_date(strtotime($recommended_date_expire), 'custom',  'Y'),
																			),
											'#required'				=>	TRUE,
											
								);
				
		  }
			
			
			$form['backlog_renewal_alc']['remarks_text']	= array(
									'#prefix' 				=> '<div id="renewal_remarks_text_div"><section><label class="textarea">',
									'#suffix' 				=> '</label></section></div>',
									'#title'				=> 'Remarks',
									'#type'					=> 'textarea',
									'#rows'					=>  2,
									'#required' 			=>  TRUE,
							);
			
	
			}
															
			
				
		/*$form['backlog_renewal_alc']['submit'] 		= array (
								  '#prefix' 				=> '</fieldset><footer>',
								  '#type' 					=> 'submit',
								  '#attributes' 			=> array('class' => array('button')),
								  '#value' 					=> 'Submit'
					  
				);					*/	
	}
	}
	}
	}
	}
	 if($get_status_renewal == 'I'){
	 
	    if(!empty($uriID)){
		if($uriID == "HI"){
				$message= drupal_set_message_custom(t('Remark is successfully deleted'));
		}if($uriID == "hello"){
				$message= drupal_set_message_custom(t('Remark is successfully saved'));
		}if($uriID == "fromv"){
			$message= drupal_set_message_custom(t('Please verify the <strong>"Form VII" by checking  check box in <strong>"Is Verified?"</strong> column then you can proceed for Issue Certificate of application remark.'),'error');
		}
				$form['backlog_renewal_alc']['MARKUP_1']	= array(
							 '#prefix' 			=> '<div id="remark_view">',		
							 '#type'			=> 'markup',
							 '#markup'			=>	$message,
							 '#suffix' 			=> '</div>'
										
						);
		}
		
			$form['backlog_renewal_alc']['show_certificates'] 		= array(
						'#prefix' 			=> '<fieldset><div id="show_certificates_id">',
						'#suffix'		    =>'</div>',
						'#type' 			=> 'markup',
						'#markup' 			=> '<strong>View Certificate of Renewal of Licence</strong> <a href="'.$GLOBALS['base_url'].'/sites/default/files/'.$upload_certificates_file_url.'" target="_blank"><img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>'
					  );
	
		}
	if($get_status_renewal != 'A' && $get_status_renewal != 'I' && $get_status_renewal != 'C' && $get_status_renewal != 'R'  && $get_status_renewal != 'B' && $get_status_renewal != 'P' ){
		$form['backlog_renewal_alc']['remark_details'] 		= array(
									
										'#type' 				=> 'markup',
										'#markup'				=> renewal_remark_details($serial_no_from_v,$license_renewal_id,$license_id,$contractor_particular_id,$created_by),
										'#suffix' 				=> '</fieldset></footer></div></div>',
									);		
}else{
	
	if(!empty($uriID)){
		if($uriID == "HI"){
		
				$message= drupal_set_message_custom(t('Remark is successfully deleted'));
		}if($uriID == "hello"){
				$message= drupal_set_message_custom(t('Remark is successfully saved'));
		}if($uriID == "fromv"){
			$message= drupal_set_message_custom(t('Please verify the <strong>"Form VII" by checking  check box in <strong>"Is Verified?"</strong> column then you can proceed for Issue Certificate of application remark.'),'error');
		}
				$form['backlog_renewal_alc']['MARKUP_1']	= array(
										'#prefix' 			=> '<div id="remark_view">',		
										'#type'				=> 'markup',
										'#markup'			=>	$message,
										 '#suffix' 			=> '</div>'
										
						);
		}
				
									
		$form['backlog_renewal_alc']['remark_details'] 		= array(
		  							
									'#type' 				=> 'markup',
									'#markup'				=> renewal_remark_details($serial_no_from_v,$license_renewal_id,$license_id,$contractor_particular_id,$created_by),
									'#suffix' 				=> '</fieldset></footer></div></div>',
								  );				
}	
		return $form;
}

function getRenewalTable($serialFormV='',$renewal_id='' ,$particular_id='', $license_id =''){
	
	global $base_root, $base_path, $user;
	
	//echo $serialFormV.'--'.$renewal_id.'--'.$particular_id.'--'.$license_id;
	$conInfo	=  db_select('l_contractor_info_master', 'info_master');
	$conInfo	-> fields('info_master', array());
	$conInfo	-> condition('info_master.id',$serialFormV);
	$conInfoRes	= $conInfo->execute()->fetchAssoc();
	
	if(!empty($conInfoRes)){
		
		$contractorName		= $conInfoRes['name_of_contractor'];
		$conAddress			= $conInfoRes['address_of_contractor'];
		$peApplicationId 	= $conInfoRes['application_id'];
		$peUser_id 			= $conInfoRes['user_id'];
		
		$conNameAddress 	= $contractorName.'<br/>'.$conInfoRes['address_of_contractor'].'<br/>'.get_full_address('l_contractor_info_master', 'sub-table', $serialFormV, array('con_loc_e_dist','con_loc_e_subdivision','con_loc_e_areatype','con_name_areatype','con_loc_e_vill_ward','con_l_e_ps','contractor_pin'));
		
		$contractor_type 	= ($conInfoRes['contractor_type'] == 1 ) ? 'Offline' : 'New Contractor';
		
		$con_nature_work 	= getConNatureOfWrk($serialFormV,$peApplicationId,1 );
		
		
		if($conInfoRes['contractor_type'] == 1) $formVUploading = 'FORM-V : Not Applicable <br/>REFERENCE NUMBER : 00'.$serialFormV; else $formVUploading = 'YES';
		$active					= '<span class="badge bg-green"><i class="fa fa-check" aria-hidden="true"></i> Active</span>';
		$deactive				= '<span class="badge bg-red"><i class="fa fa-close"></i> Deactive</span>';
		$contractor_status 		= ($conInfoRes['status'] == 1 ) ? $active : $deactive;
		
		/*** Establishment Details **/
		
		$fetchEstablishment = 	db_select('l_clra_registration_master', 'est_master');
		$fetchEstablishment	-> fields('est_master', array('id','e_name','loc_e_name','registration_number','act_id','registration_date','full_name_principal_emp','address_principal_emp','loc_emp_country','certificates_fid'));
		$fetchEstablishment ->condition('user_id',$peUser_id,'=');
		$fetchEstablishment	->condition('est_master.id',$peApplicationId,'=');
		$fetchEstablishment	->orderBy('est_master.id','DESC');
		$establishmentData 	= 	$fetchEstablishment->execute()->fetchAssoc();
		
		if(!empty($establishmentData)){
			
			$establishmentInfo 	=  $establishmentData['e_name'].'<br/>'.$establishmentData['loc_e_name'].'<br/>'.get_full_address('l_clra_registration_master', 'sub-table', $peApplicationId, array('loc_e_dist','loc_e_subdivision','loc_e_areatype','name_areatype','loc_e_vill_ward','l_e_ps','loc_e_pin_number'));
			
			if($establishmentData['loc_emp_country'] == 1){
				
				$peInfo	= $establishmentData['full_name_principal_emp'].'<br/>'.$establishmentData['address_principal_emp'].'<br/>'.get_full_address('l_clra_registration_master', 'sub-table', $peApplicationId, array('loc_emp_dist','loc_emp_subdivision','loc_emp_areatype','emp_name_areatype','loc_emp_vill_ward','l_emp_ps','loc_emp_pin_number','loc_emp_state'));
				
			}else{
				
				$peInfo	= $establishmentData['full_name_principal_emp'].'<br/>'.$establishmentData['address_principal_emp'];
				
			}
			
			$peNatureOfWrk 		= pe_natureOfWrk($peApplicationId,$peUser_id,1);
			
			$reg_certificate	=	db_select('file_managed','fm');
			$reg_certificate 	->	fields('fm',array());
			$reg_certificate	->	condition('fm.fid',$establishmentData['certificates_fid']);
			$regFile			=	$reg_certificate ->execute()->fetchAssoc();
			if(!empty($regFile)){ 
				$certificates_url	= '';
				$url			 	= 	explode('//',$regFile['uri']);
				$certificates_url	= 	$url[1];
					
			}
			
			$contractorLicenseData 	= 	db_select('l_particular_of_contract_labour', 'particular');
			$contractorLicenseData	->  leftJoin('l_contractor_license_application', 'license','license.serial_no_from_v= particular.serial_no_from_v and license.contractor_user_id=particular.contractor_user_id ');
			//$contractorLicenseData	->  leftJoin('l_contractor_license_apply_renweal', 'renewal','renewal.lincense_id= license.id and renewal.created_by=license.contractor_user_id ');
			$contractorLicenseData	->	fields('particular',array());
			$contractorLicenseData	->	fields('license',array('id','backlog_license_date','backlog_license_no','contractor_user_id','is_backlog_license','contractor_license_number','application_id','user_id','application_id'));
			//$contractorLicenseData	->	fields('renewal',array());
			//$contractorLicenseData	->	condition('renewal.action','R','=');
			//$contractorLicenseData	->	condition('renewal.id',$renewal_id,'=');
			$contractorLicenseData	->	condition('particular.id',$particular_id,'=');
			$contractorLicenseData	->	condition('license.id',$license_id,'=');
			$licenseResult 			= 	$contractorLicenseData->execute()->fetchAssoc(); //print_r($licenseResult);exit;
			
			if(!empty($licenseResult)){
				$worksite_address			= $licenseResult['worksite_address_line'].'<br/>'.get_full_address('l_particular_of_contract_labour', 'sub-table', $particular_id, array('worksite_dist','worksite_subdivision','work_site_areatype','name_work_site_areatype','work_site_vill_ward','worksite_ps','worksite_pin'));
				$conUpdatedAddress			= $licenseResult['address_of_contractor'].'<br/>'.get_full_address('l_particular_of_contract_labour', 'sub-table', $particular_id, array('contractor_dist','contractor_subdivision','contractor_areatype','contractor_name_areatype','contractor_vill_ward','contractor_ps','contractor_pin'));
				
				$agentManagerAddress		= get_full_address('l_particular_of_contract_labour', 'sub-table', $particular_id, array('contractor_manager_dist','contractor_manager_subdivision','contractor_manager_areatype','contractor_manager_name_areatype','contractor_managerr_vill_ward','contractor_manager_ps','manager_pin'));
				$cooperative				=	($licenseResult['is_coparative'] == 1) ?  'Yes' : 'No';
				if($licenseResult['is_coparative'] == 1){
						$alert = '[<font color="#FF0000">Reduced 25% of security fees per head of contract labour,applicable for Co-oparative Society</font>]';
				}else{
					$alert = '';	
				}
				
				if($licenseResult!='' && $licenseResult['category_of_contractor'] == 1){
					$contractor_type_title 	= 'Commencement date of business';
					$father_husband_name	= 'Not Applicable';
					$contractor_type_value 	= !empty($licenseResult['dob_contractor']) ? date('dS M, Y',strtotime($licenseResult['dob_contractor'])) : "";
				}else{
					$contractor_type_title = 'Date of Birth and age [in case of individuals]';
					$father_husband_name   = !empty($licenseResult['father_contarctor_name']) ? $licenseResult['father_contarctor_name'] : "Not Applicable";  
					$contractor_type_value = !empty($licenseResult['dob_contractor']) ?date('dS M, Y',strtotime($licenseResult['dob_contractor'])).'&nbsp;&nbsp;Age- '.$licenseResult['age_contractor'] .' years' : "";
				}
				
				$unskilled_rate_wages		=   ($licenseResult['unskilled_rate_wages']!='' && $licenseResult['unskilled_rate_wages']!=0) ?  '&#x20B9; '. $licenseResult['unskilled_rate_wages'].' ( Rupees  '.convertNumberToWord($licenseResult['unskilled_rate_wages']). 'only)' : 'NIL';
				$semiskilled_rate_wages		=	($licenseResult['semiskilled_rate_wages']!='' && $licenseResult['semiskilled_rate_wages']!=0) ?  '&#x20B9; '. $licenseResult['semiskilled_rate_wages'].' ( Rupees  '.convertNumberToWord($licenseResult['semiskilled_rate_wages']). 'only)' : 'NIL';
				$skilled_rate_wages			=	($licenseResult['skilled_rate_wages']!='' && $licenseResult['skilled_rate_wages']!=0 ) ? '&#x20B9; '. $licenseResult['skilled_rate_wages'].' ( Rupees  '.convertNumberToWord($licenseResult['skilled_rate_wages']). 'only)' : 'NIL';  
				$highlyskilled_rate_wages	=	($licenseResult['highlyskilled_rate_wages']!='' && $licenseResult['highlyskilled_rate_wages']!=0) ? '&#x20B9;'. $licenseResult['highlyskilled_rate_wages'].' ( Rupees  '.convertNumberToWord($licenseResult['highlyskilled_rate_wages']). 'only)' : 'NIL';
				
				$annual_leave				= 	($licenseResult['annual_leave_no']!='' && $licenseResult['annual_leave_no']!=0) ? $licenseResult['annual_leave_no'].' day(s)' : 'NIL'; 
				$casual_leave				= 	($licenseResult['casual_leave_no']!='' && $licenseResult['casual_leave_no']!=0) ? $licenseResult['casual_leave_no'].' day(s)' : 'NIL'; 
				$sick_leave					= 	($licenseResult['sick_leave_no']!='' && $licenseResult['sick_leave_no']!=0) ? $licenseResult['sick_leave_no'].' day(s)' : 'NIL';
				$maternity_leave			=	($licenseResult['maternity_leave_no']!='' && $licenseResult['maternity_leave_no']!=0) ? $licenseResult['maternity_leave_no'].' day(s)' : 'NIL'; 
				$other_leave				= 	($licenseResult['other_leave_no']!='' && $licenseResult['other_leave_no']!=0) ? $licenseResult['other_leave_no'].' day(s)' : 'NIL';  				$earned_leave				= 	($licenseResult['earned_leave_no']!='' && $licenseResult['earned_leave_no']!=0) ? $licenseResult['earned_leave_no'].' day(s)' : 'NIL';
				$contractor_convicted		=  ($licenseResult['contractor_convicted']== 1 && !empty($licenseResult['details_contractor_convicted'])) ? $licenseResult['details_contractor_convicted']  : 'NIL';
				
				$contractor_revoking		=   !empty($licenseResult['details_contractor_revoking'])!='' && $licenseResult['details_contractor_revoking']!='' ? date('dS M, Y',strtotime($licenseResult['details_contractor_revoking'])) : 'NIL'; 
				
				
				$amount_of_security_deposit	=	$licenseResult['amount_of_security_deposit'];
				
				
				
				
				$rest_holiday = substr($licenseResult['no_holiday'], 0, -1);
				$no_holiday_list= explode(',', $rest_holiday);
				$name_holiday="";
				$total_holiday=0;
				foreach($no_holiday_list as $row){
					$total_holiday=$total_holiday+1;
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
					
				}
				$name_holidays = substr($name_holiday, 0, -1);
				if(empty($name_holidays))
					$name_holidays='Nil';
				
				/** Renewal data **/
				
				$renewalDetails 			= contractor_license_apply_renweal($renewal_id);
				
				$workOrderFile = fetch_uplaoded_file_details($licenseResult['work_order_file_id']);
				if(!empty($workOrderFile)){ 
					$url			 	= 	explode('//',$workOrderFile['uri']);
					$work_order_url		= 	$url[1];
					$workorder_file		=	$workOrderFile['filename'];
				}
				
				$frmVFile = fetch_uplaoded_file_details($licenseResult['frm_v_file_id']);
				if(!empty($frmVFile)){ 
					$url 				= explode('//',$frmVFile['uri']);
					$frmv_url			= $url[1];
					$frmv_file			= $frmVFile['filename'];
				}
				
				$residentialFile = fetch_uplaoded_file_details($licenseResult['residential_file_id']);
				if(!empty($residentialFile)){ 
					$url					= explode('//',$residentialFile['uri']);
					$residential_url		= $url[1];
					$residential_file		= $residentialFile['filename'];
				}
				
				$backlogCertificate 		= fetch_uplaoded_file_details($renewalDetails['upload_backlog_certificate']);
				if(!empty($backlogCertificate)){ 
					$url					= explode('//',$backlogCertificate['uri']);
					$preCertificatesUrl		= $url[1];
				}
				
				$formIVFile = fetch_uplaoded_file_details($licenseResult['signed_license_id']);
				if(!empty($formIVFile)){ 
					$url		  			= explode('//', $formIVFile['uri']);
					$form_vii_url 			= $url[1];
					
				}
				$form7File 					= fetch_uplaoded_file_details($renewalDetails['upload_signed_form']);
				if(!empty($form7File)){ 
					$url		  			= explode('//', $form7File['uri']);
					$form_vii_url 			= $url[1];
					
				}	
				
				$perviousLicenseDetais		= license_details($serialFormV,$licenseResult['contractor_user_id']); //print_r($perviousLicenseDetais);exit;
				
				$get_remark = db_select('l_remark_license', 'remark_license');
				$get_remark->fields('remark_license', array('remark_field_title'));
				$get_remark->condition('remark_license.remark_to', $serialFormV);
				$get_remark->condition('remark_license.particular_id', $renewal_id);
				$get_remark->where("((remark_license.remark_by_role = 4) OR (remark_license.remark_by_role = 7))");
				$get_remark->orderBy('remark_license.id','DESC');
				$get_remark->range(0, 1);
				$get_remark_result = $get_remark->execute()->fetchAssoc();
				
				$remark_field_arr = explode(',', $get_remark_result['remark_field_title']);
				
				foreach ($remark_field_arr as $fieldname){
					
					if($fieldname == 'father_name'){ $father_name_check = "checked='checked'";}
					if($fieldname == 'address_of_contractor'){ $address_of_contractor_check = "checked='checked'";}
					if($fieldname == 'indivisual_company'){ $indivisual_company_check = "checked='checked'";}	
					if($fieldname == 'backlog_licnse_date'){ $backlog_licnse_date_check = "checked='checked'";}
					if($fieldname == 'backlog_renewal_date'){ $backlog_renewal_date_check = "checked='checked'";}	
					if($fieldname == 'name_manager'){ $name_manager_check = "checked='checked'";}	
					if($fieldname == 'address_manager'){ $address_manager_check = "checked='checked'";}	
					if($fieldname == 'unskilled_wages'){ $unskilled_wages_check = "checked='checked'";}	
					if($fieldname == 'semiskilled_wages'){ $semiskilled_wages_check = "checked='checked'";}	
					if($fieldname == 'skilled_wages'){ $skilled_wages_check = "checked='checked'";}
					if($fieldname == 'highlysiskilled_wages'){ $highlysiskilled_wages_check = "checked='checked'";}	
					if($fieldname == 'annual_leave'){ $annual_leave_check = "checked='checked'";}	
					if($fieldname == 'casual_leave'){ $casual_leave_check = "checked='checked'";}	
					if($fieldname == 'eraned_leave'){ $eraned_leave_check = "checked='checked'";}	
					if($fieldname == 'sick_leave'){ $sick_leave_check = "checked='checked'";}
					if($fieldname == 'maternity_leave'){ $maternity_leave_check = "checked='checked'";}	
					if($fieldname == 'other_leave'){ $other_leave_check = "checked='checked'";}
					if($fieldname == 'weekly_holiday'){ $weekly_holiday_check = "checked='checked'";}
					if($fieldname == 'holiday_wages'){ $holiday_wages_check = "checked='checked'";}
					if($fieldname == 'offence'){ $offence_check = "checked='checked'";}
					if($fieldname == 'revoking'){ $revoking_check = "checked='checked'";}
					if($fieldname == 'pervious_license'){ $pervious_license_check = "checked='checked'";}
					if($fieldname == 'special_benifites'){ $special_benifites_check = "checked='checked'";}
					if($fieldname == 'state_insurance'){ $state_insurance_check = "checked='checked'";}
					if($fieldname == 'miscellaneous'){ $miscellaneous_check = "checked='checked'";}
					if($fieldname == 'work_order'){ $work_order_check = "checked='checked'";}
					if($fieldname == 'frm_v'){ $frm_v_check = "checked='checked'";}
					if($fieldname == 'residential'){ $residential_check = "checked='checked'";}
					if($fieldname == 'perivious'){ $perivious_check = "checked='checked'";}
					if($fieldname == 'form_vii'){ $form_vii_check= "checked='checked'";}
					if($fieldname == 'license_amount'){ $license_amount_check = "checked='checked'";}
					if($fieldname == 'security_amount'){ $seurity_amount_check = "checked='checked'";}
					if($fieldname == 'backlog_renewal_amount'){ $backlog_renewal_amount_check = "checked='checked'";}
					if($fieldname == 'category'){ $category_check = "checked='checked'";}
					if($fieldname == 'hour_details'){ $hour_details_check = "checked='checked'";}
					if($fieldname =='worksite_address'){$worksite_address_check = "checked='checked'";}
					if($fieldname =='backlog_renewal_extra_amount'){$backlog_renewal_extra_check = "checked='checked'";}
					if($fieldname =='is_co_oparative'){$is_co_oparative_check = "checked='checked'";}	
				}
				
				
				if($licenseResult['backlog_license_date'] <= '2015-11-27'){
					
					if(empty($renewalDetails['backlog_renewal_date'])){
						
						$appliedyear				= date('Y', strtotime($renewalDetails['created_date']));
						$backlog_license_year		= date('Y', strtotime($licenseResult['backlog_license_date']));
						$backlog_license_month		= date('m', strtotime($licenseResult['backlog_license_date']));
						$backlog_license_day		= date('d', strtotime($licenseResult['backlog_license_date']));
						$next_renweal_date			= ($appliedyear+1).'-12-31';
						$renewal_expire 			= '31st Dec,'.$backlog_license_year;;	
					}else{
						$renewal_expire				= date("dS M, Y",strtotime($renewalDetails['backlog_renewal_date']));	
					}
			
				}else{
			
					if(empty($renewalDetails['backlog_renewal_date'])){
						$backlog_license_year		= date('Y', strtotime($licenseResult['backlog_license_date']));
						$backlog_license_month		= date('m', strtotime($licenseResult['backlog_license_date']));
						$backlog_license_day		= date('d', strtotime($licenseResult['backlog_license_date']));
						$next_renewal_date			= ($backlog_license_year+1).'-'.$backlog_license_month.'-'.($backlog_license_day-1);
						
						$renewal_expire				= date("dS M, Y",strtotime($next_renewal_date));
					}else{
						$renewal_expire				= date("dS M, Y",strtotime($renewalDetails['backlog_renewal_date']));	
					}
				}
				
				if($renewalDetails['renewal_security_new'] > $amount_of_security_deposit){
					$new_security_amount = $renewalDetails['renewal_security_new'] -  $amount_of_security_deposit;
				}else{
					$new_security_amount = 0;
				}
				
				
				if($renewalDetails['license_reneal_fees'] < $renewalDetails['increnent_fees']){
					$extraFees 				=  '<i style="color:#f00;">[ including 25% extra ]</i> ';
				}else{
					$extraFees 				=  '';
				}
				
				
				
				$transaction_details  			= 	db_select('l_principle_epayments_receive_data', 'lpd');
				$transaction_details			->	leftJoin('l_principle_epayments_data', 'lped', 'lpd.transaction_id = lped.identification_no');
				$transaction_details			->  fields('lped', array('identification_no', 'application_id'));
				$transaction_details			->  fields('lpd', array());
				$transaction_details			->	condition('lped.act_id', '13');
				$transaction_details			->	condition('lped.application_id',$renewal_id);
				$transaction_details			->	condition('lped.deposited_by',$licenseResult['contractor_user_id']); 
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
				$payment_details = '<u>Grips Payment[Online/Counter]</u><br>';
				$payment_details .= 'IFSC Code:<font color="#FF6600">'.$bank_code.'</font><br>';
				$payment_details .= 'Bank Transaction Id:<font color="#FF6600">'.$bankTransactionID.'</font><br>';
				$payment_details .= 'Total Amount:<font color="#FF6600">'.$total_amount.'</font><br>';
				$payment_details .= 'Transaction Date:<font color="#FF6600">'.$challan_fid_date.'</font><br>';
				$payment_details .= 'Transaction Status: '.$payment_status.trim($remark_type);	
				
				
				$output	='<div class="row equal-panels">
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
													<td>'.$peNatureOfWrk.'</td>
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
														l('<span class="badge bg-blue"><i class="fa fa-eye"></i> View More of Registration<div>', 'alc-visible-applications/'.encryption_decryption_fun('encrypt', $peApplicationId).'/'.encryption_decryption_fun('encrypt', $peUser_id), array('html' => TRUE, 'target' => '_blank')).'
													
													
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
									  <h3 class="box-title">2. Contractor Details provided by Principal Employer[ Verified ]</h3>
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
												<td>'.date('dS M, Y',strtotime($conInfoRes['est_date_of_work_of_each_labour_from_date'])).' TO '.date('dS M, Y',strtotime($conInfoRes['est_date_of_work_of_each_labour_to_date'])).'</td>
											</tr>
											<tr>
												<td>2.(e)</td>
												<td>Maximum number of contract labour proposed to be employed in the establishment on any date</td>
												<td>'.$conInfoRes['contractor_max_no_of_labours_on_any_day'].'&nbsp;&nbsp; <font color="#006600"> <b>[ FEES IS CALCULATED BASED ON THIS VALUE ] </b></font></td>
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
										<tr>
											<td>1.(a)</td>
											<td width="50%">Father&#13;s name of the contractor (including his father&#13;s name in case of individuals) </td>
											<td>'.$father_husband_name.'</td>
											<td><input type="checkbox" id="father_name" class="renewal_alc_verified" '.$father_name_check.' </td> 
										</tr>
										<tr>
											<td>1.(b)</td>
											<td>Address of the contractor </td>
											<td>'.$conUpdatedAddress.'</td>
											<td><input type="checkbox" id="address_of_contractor" class="renewal_alc_verified"  '.$address_of_contractor_check.'</td>
										</tr>
										<tr>
											<td>2.(a)</td>
											<td><strong>Co-operative Society</strong>  <font color="red">**</font></td>
											<td>'.$cooperative.'</td>
											<td><input type="checkbox" id="is_co_oparative" class="renewal_alc_verified"  '.$is_co_oparative_check.'</td>
										</tr>
										<tr>
											<td>2.(b)</td>
											<td>'.$contractor_type_title.'</td>
											<td>'.$contractor_type_value.'</td>
											<td><input type="checkbox" id="indivisual_company" class="renewal_alc_verified" '.$indivisual_company_check.' />  </td>
										</tr>
										<tr>
											<td>3.(a)</td>
											<td>Name of the agent or manager of contractor at the work site </td>
											<td>'.$licenseResult['name_of_agent'].'</td>
											<td><input type="checkbox" id="name_manager" class="renewal_alc_verified" '.$name_manager_check.' />  </td>
										</tr>
										<tr>
											<td>3.(b)</td>
											<td>Address of the agent or manager of contractor at the work site</td>
											<td>'.$agentManagerAddress.'</td>
											<td><input type="checkbox" id="address_of_manager" class="renewal_alc_verified" '.$address_manager_check.' />  </td>
										</tr>
										<tr>
											<td>4. </td>
											<td>Category/designation/nomenclature of the contractor labour, namely,fitter,welder,carpenter,mazdor etc.</td>
											<td>'.$licenseResult['category_designation'].'</td>
											<td><input type="checkbox" id="category" class="renewal_alc_verified" '.$category_check.' />  </td>
										</tr>
										<tr>
											<td colspan="4" style="font-size:15px; font-weight:600;">
												5.&nbsp;&nbsp;&nbsp;Rate of Wages, DA and other cash benefits paid / to be paid to each category (i.e (a) Unskilled (b) Semi-Skilled (c) Skilled (d) Highly-Skilled etc.) of contract labour</td>
										</tr>
										<tr>
											<td>5.(a)</td>
											<td>Rate of wages,DA and Other cash benefits paid/ to be paid to Unskilled of contract labour:</td>
											<td>'.$unskilled_rate_wages.'</td>
											<td><input type="checkbox" id="unskilled_wages" class="renewal_alc_verified"  '.$unskilled_wages_check.'  </td>
										</tr>
										<tr>
											<td>5.(b)</td>
											<td>Rate of wages,DA and Other cash benefits paid/ to be paid to Semi-skilled of contract labour:</td>
											<td>'.$semiskilled_rate_wages.'</td>
											<td><input type="checkbox" id="semiskilled_wages" class="renewal_alc_verified" '.$semiskilled_wages_check.'  </td>
										</tr>
										<tr>
											<td>5.(c)</td>
											<td>Rate of Wages,DA and Other cash benefits paid/ to be paid to Skilled of contract labour:</td>
											<td>'.$skilled_rate_wages.'</td>
											<td><input type="checkbox" id="skilled_wages" class="renewal_alc_verified"  '.$skilled_wages_check.'</td>
										</tr>
										<tr>
											<td>5.(d)</td>
											<td>Rate of Wages,DA and Other cash benefits paid/ to be paid to Highly-skilled of contract labour:</td>
											<td>'.$highlyskilled_rate_wages.'</td>
											<td><input type="checkbox" id="highlysiskilled_wages" class="renewal_alc_verified" '.$highlysiskilled_wages_check.' </td>
										<tr>
										<tr>
											<td colspan="4" style="font-size:15px; font-weight:600;">6.&nbsp;&nbsp;&nbsp;Hours of Work, Overtime and Overtime Wages</td>
										</tr>
										<tr>
											<td>6.(a)</td>
											<td>Hours of Work,Spread over time,Overtime and Overtime Wages</td>
											<td>Hours of Work : '.$licenseResult['hours_work'].' hr(s)<br/> Spread over time : '.$licenseResult['spred_over'].'  hr(s)<br/>Overtime: '.$licenseResult['overtime'].' hr(s)<br/>Overtime Wages: &#x20B9; '.$licenseResult['overtime_wages'].' </td>
											<td><input type="checkbox" id="hour_details" class="renewal_alc_verified"  '.$hour_details_check.' </td>
										</tr>
										<tr>
											<td colspan="4" style="font-size:15px; font-weight:600;">
											7.&nbsp;&nbsp;&nbsp;Other Condition of service like leave ( annual leave, casual leave,sick leave, maternity leave etc.) Holidays etc. of the contract labour</td>
										</tr>
										<tr>
											<td>7.(a)</td>
											<td>Number of annual leave</td>
											<td>'.$annual_leave.'</td>
											<td><input type="checkbox" id="annual_leave" class="renewal_alc_verified" '.$annual_leave_check.' </td>
										</tr>
										<tr>
											<td>7.(b)</td>
											<td>Number of casual leave</td>
											<td>'.$casual_leave.'</td>
											<td><input type="checkbox" id="casual_leave" class="renewal_alc_verified" '.$casual_leave_check.'  </td>
										</tr>
										<tr>
											<td>7.(c)</td>
											<td>Number of sick leave</td>
											<td>'.$sick_leave.'</td>
											<td><input type="checkbox" id="sick_leave" class="renewal_alc_verified"  '.$sick_leave_check.' </td>
										</tr>
										<tr>
											<td>7.(d)</td>
											<td>Number of maternity leave</td>
											<td>'.$maternity_leave.'</td>
											<td><input type="checkbox" id="maternity_leave" class="renewal_alc_verified" '.$maternity_leave_check.'</td>
										</tr>
										<tr>
											<td>7.(e)</td>
											<td>Number of other leave</td>
											<td>'.$other_leave.'</td>
											<td><input type="checkbox" id="other_leave" class="renewal_alc_verified"  '.$other_leave_check.' </td>
										</tr>
										<tr>
											<td>7.(f)</td>
											<td>Number of earned leave</td>
											<td>'.$earned_leave.'</td>
											<td><input type="checkbox" id="eraned_leave" class="renewal_alc_verified"  '.$eraned_leave_check.' </td>
										</tr>
										<tr>
											<td>7.(g)</td>
											<td>Whether weekly holiday(s) observed and on which day</td>
											<td>'.$total_holiday.' day(s) ('.$name_holidays.')</td>
											<td><input type="checkbox" id="weekly_holiday" class="renewal_alc_verified"  '.$weekly_holiday_check.' </td>
										</tr>
										<tr>
											<td>7.(h)</td>
											<td>Whether weekly holiday(s) so observed was paid holiday(s)</td>
											<td>'.$renewalDetails['holiday_wages'].' day(s)</td>
											<td><input type="checkbox" id="holiday_wages" class="renewal_alc_verified"  '.$holiday_wages_check.' </td>
										</tr>
										<tr>
											<td>8. </td>
											<td>Whether the contractor was convicted of any offence within the preceeding five years. If so, give details</td>
											<td>'.$contractor_convicted.'</td>
											<td><input type="checkbox" id="offence" class="renewal_alc_verified" '.$offence_check.'  </td>
										</tr>
										<tr>
											<td>9. </td>
											<td>Whether there was any order against the contract or revoking or suspending license or forfeiting security deposit in respect of an earlier contract. If so, the date of such order.</td>
											<td>'.$contractor_revoking.'</td>
											<td><input type="checkbox" id="revoking" class="renewal_alc_verified" '.$revoking_check.' </td>
										</tr>
										<tr>
											<td>10. </td>
											<td>Whether the contractor has worked in any other establishment within the past five years. If so. give details of the principal employer, establishment and nature of work</td>
											<td>'.$perviousLicenseDetais.'</td>
											<td><input type="checkbox" id="pervious_license" class="renewal_alc_verified"  '.$pervious_license_check.'/></td>
										</tr>
										<tr>
											<td>11. </td>
											<td>Special benifits provided, if any</td>
											<td>'.$renewalDetails['special_benifites'].'</td>
											<td><input type="checkbox" id="special_benifites" class="renewal_alc_verified" '.$special_benifites_check.' </td>
										</tr>
										<tr>
											<td>12. </td>
											<td>Contribution made under the Employees State Insurance Act,1948</td>
											<td>'.$renewalDetails['state_insurance'].'</td>
											<td><input type="checkbox" id="state_insurance" class="renewal_alc_verified"  '.$state_insurance_check.' /></td>
										  </tr>
										  <tr>
										   <td>13. </td>
											<td>Contribution made under the Employees Provident Fund and Miscellaneous Provision Act,1952</td>
											<td>'.$renewalDetails['miscellaneous_provisions'].'</td>
											<td><input type="checkbox" id="miscellaneous" class="renewal_alc_verified"  '.$miscellaneous_check.' /></td>
										  </tr>
										  <tr>
										   <td>14. </td>
											<td>Whether a certificate by the principal employer in Form V is Enclosed</td>
											<td colspan="2">FORM-V Not Applicable (Legacy Data)<br/> REF NO: 00'.$serialFormV.'</td>
										  </tr>
										  <tr>
											<td colspan="4" style="font-size:15px; font-weight:600;">15.&nbsp;&nbsp;&nbsp;Other Important Information</td>
										  </tr>
										  <tr>
											<td>15.(a)</td>
											<td><strong>Work Site Address <font color="red">**</font></strong> </td>
											<td>'.$worksite_address.'</td>
											<td><input type="checkbox" id="worksite_address" class="renewal_alc_verified" '.$worksite_address_check.' </td>
										</tr>
										<tr>
											<td>15.(b)</td>
											<td><strong>Number and date of the license<font color="red">**</font></strong></td>
											<td>'.$licenseResult['backlog_license_no'].' issued on  '.date('dS M, Y',strtotime($licenseResult['backlog_license_date'])).' </td>
											<td><input type="checkbox" id="backlog_licnse_date"  class="renewal_alc_verified" '.$backlog_licnse_date_check.' /></td>
										</tr>
										<tr>
											<td>15.(c)</td>
											<td><strong>Date of expiry of the previous license<font color="red">**</font></strong></td>
											<td>' .$renewal_expire.'</td>
											<td><input type="checkbox" id="backlog_renewal_date" class="renewal_alc_verified"  '.$backlog_renewal_date_check.' /></td>
										</tr>
										<tr>
											<td>15.(d)</td>
											<td><strong>Amount of Licence Fee paid<font color="red">**</font></strong></td>
											<td>₹ '.$licenseResult['amount_of_licence'].' ( Rupees '.convertNumberToWord($licenseResult['amount_of_licence']). ' only)</td>
											<td><input type="checkbox" id="license_amount"  class="renewal_alc_verified" '.$license_amount_check.' /></td>
										<tr>
										<tr>
											<td>15.(e)</td>
											<td><strong>Amount of Security Deposit paid <font color="red">**</font></strong></td>
											<td>₹ ' .$amount_of_security_deposit.' ( Rupees '.convertNumberToWord($amount_of_security_deposit). ' only)</td>
											<td><input type="checkbox" id="security_amount" class="renewal_alc_verified" '.$seurity_amount_check.' /></td>
										</tr>
										<tr>
											<td>15.(f)</td>
											<td><strong>Amount of last Renewal of Licence Fee paid<font color="red">**</font></strong></td>
											<td>₹ ' .$renewalDetails['backlog_renewal_fees'].' ( Rupees '.convertNumberToWord($renewalDetails['backlog_renewal_fees']). ' only)</td>
											<td><input type="checkbox" id="backlog_renewal_amount" class="renewal_alc_verified"  '.$backlog_renewal_amount_check.'/></td>
										</tr>	
										<tr>
											<td colspan="4" style="font-weight:700; text-transform:uppercase; font-size:15px;">16. Documents Uploaded</td>
										 </tr> 
										 <tr>
											<td>ii.)</td>
											<td>Work Order</td>';
												if(!empty($licenseResult['work_order_file_id'])){
													$output .= '<td>
																	<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$work_order_url.'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;
																	<span class="viewfile_popup" id="workorder">
																		<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
																	</span>
																	<div id="view_workorder"  title="Work Order" style="display:none;">
																		<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$work_order_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
																	</div>
																</td>';
												}else {
													
													$output .=  '<td>No Document Uploaded</td>';
													
												}
												$output .=  '<td><input type="checkbox" id="work_order" class="renewal_alc_verified" '.$work_order_check.' </td>
										</tr>
										<tr>
											<td>ii.)</td>
											<td>Residential Certificate</td>';
												if(!empty($licenseResult['residential_file_id'])){
													$output .= '<td>
																	<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$residential_url.'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;
																	<span class="viewfile_popup" id="res_cert">
																		<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
																	</span>
																	<div id="view_res_cert"  title="Residential Certificate" style="display:none;">
																		<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$residential_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
																	</div>
																</td>';
												}else {
													
													$output .=  '<td>No Document Uploaded</td>';
													
												}
												$output .=  '<td><input type="checkbox" id="residential" class="renewal_alc_verified" '.$residential_check.' </td>
										</tr>
										<tr>
											<td>iii.)</td>
											<td>Previous License / Renewal Certificate</td>';
											if($renewalDetails['upload_backlog_certificate']!=''){
											
								$output	.= '<td>
												<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$preCertificatesUrl.'"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;&nbsp;
												<span class="viewfile_popup" id="previousCertificate">
													<i class="fa fa-search-plus fa-lg" aria-hidden="true"></i>
												</span>
												<div id="view_previousCertificate"  title="License / Renewal Certificate" style="display:none;">
													<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$preCertificatesUrl.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
												</div>
											</td>';
											}else{
												$output .=  '<td>No Document Uploaded</td>';
											}
											$output .=  '<td><input type="checkbox" id="perivious" class="renewal_alc_verified" '.$perivious_check.' </td>
										</tr>
										<tr>
											<td>iv.)</td>
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
																'.l(t('Generated FORM - VII'),'pdf-form-vii/'.encryption_decryption_fun('encrypt', $renewal_id).'/'.encryption_decryption_fun('encrypt', $licenseResult['contractor_user_id']).'/Form-VII'.time(), array('html' => TRUE,'attributes'=> array('target'=>'_blank')));
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
												
											$output .= '<td id="formVId"><input type="checkbox" id="form_vii" '.$disabled.' class="renewal_alc_verified"   '.$form_vii_check.'> </td>
										</tr>
										<tr>
											<td colspan="4" style="font-size:15px; font-weight:600">17. Payment Details</td>
									   </tr>
									   <tr>
								    	<td>17.(a)</td>
										<td>Renewal Fees</strong></td>
										<td colspan="2">₹' .$renewalDetails['increnent_fees'].' ( Rupees '.convertNumberToWord($renewalDetails['increnent_fees']). ' only)&nbsp;&nbsp; '.$extraFees.'</td>
										</tr>
									    <tr>
											<td>17.(b)</td>
											<td>Applicable security fees for Renewal</strong></td>
											<td colspan="2">₹' .$renewalDetails['renewal_security_new'].' ( Rupees '.convertNumberToWord($renewalDetails['renewal_security_new']). ' only)&nbsp;&nbsp; '.$alert.'</td>
										</tr>
										<tr>
											<td>17.(c)</td>
											<td>Payable security fees for Renewal</strong></td>
											<td colspan="2">₹' .$new_security_amount.' ( Rupees '.convertNumberToWord($new_security_amount). ' only)</td>
										</tr>
										<tr>
											<td colspan="3"><font color="#FF0000">Provided that if the application for renewal is not received with in the time specified in R29(2)[<i>Amendment on 27th Nov. 2015</i>], a fee of 25%, in excess of the fee ordinarily payable for the licence shall be payable for such renewal: Provided further that in case where the licensing officer is satisfied that the delay in submission of the application is due to unavoidable circumstances beyond the control of the contractor, he may reduce or remit as he thinks fit the payment of such excess fee.[see R29(3)]</font></td>
										
											<td><input type="checkbox" id="backlog_renewal_extra_amount" onclick="return Confirmcheck()" class="renewal_alc_verified"  '.$backlog_renewal_extra_check.'/><br><i style="color:#f00;">Must be select for 25% additional fees</i></td>
										</tr>
										<tr>
											<td>17.(d)</td>
											<td>Payment Details</td>
											<td colspan="2">'.$payment_details.'</td>
										</tr>
									</table>
								</div>
							</div>
						 </div>';
		

		
			}
		}
	}
	
 	$output .= '<div class="row"><ul style="padding:0;"><li class="col-md-2 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-arrow-left"></i>&nbsp;Back to list</div>', 'alc/alc-received-renewal-details', array('attributes' => array('title' => 'Click here back to applications list'), 'html' => TRUE)).'</li>';
	
	$output .= '<li class="col-md-2 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-user"></i>&nbsp;View Applicant Profile</div>', 'view-applicant-profile/'.encryption_decryption_fun('encrypt', $serialFormV).'/'.encryption_decryption_fun('encrypt', $contractor_user_id).'/'.encryption_decryption_fun('encrypt', 12), array('attributes' => array('title' => 'Click here view Applicant Profile'), 'html' => TRUE)).'</li>';
	
	$output .= '<li class="col-md-4 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-info-circle"></i>&nbsp;Instructions to give remarks</div>', $base_root.$base_path.'sites/default/files/instructions-remarks.pdf', array('attributes' => array('title' => 'Instructions to give remarks'), 'html' => TRUE)).'</li>';
	
	$output .= '<li class="col-md-4 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-question-circle" aria-hidden="true"></i>&nbsp;How to digitally sign your documents using USB Token</div>', 'digitally-sign-process', array('attributes' => array('title' => 'How to digitally sign your documents using USB Token'), 'html' => TRUE)).'</li></ul></div>';
	return $output; 
	//die('<h1>Work is in progress. Inconvenience Regretted</h1>');
}

function renewal_remark_details($serial_no_from_v,$license_renewal_id,$license_id,$contractor_particular_id,$created_by){
		
	$sl_no='0';
	 global $user;
	$user_id 					= $user->uid;
	
	$query_frmv = db_select('l_contractor_info_master', 'cim');
	$query_frmv = $query_frmv->fields('cim',array('is_from_v'));
	$query_frmv->condition('cim.id',$serial_no_from_v);
	$is_frm_v = $query_frmv->execute()->fetchObject();
	$is_frm_v=$is_frm_v->is_from_v;
	if($is_frm_v=='0'){
		$from_v_no='N/A';
	}else{
		$from_v_no='00'.$serial_no_from_v;
	}
	$header = array(
					array('data' => 'Sl. No', 'field' => 'slno'),
					array('data' => 'Remark', 'field' => 'remrk'),
					array('data' => 'Remark On', 'field' => 'remark type'),
					array('data' => 'Serial/Ref. No.', 'field' => 'serial'),
					array('data' => 'Date', 'field' => 'date'),
					array('data' => 'Remark By', 'field' => 'licenseno'),
					array('data' => 'Action', 'field' => 'action'),
				);

	$remark_details			=	db_select('l_remark_license','Rm');
	
	$remark_details			->	InnerJoin('role', 'r','r.rid = Rm.remark_by_role');
	$remark_details 		->	fields('Rm',array('remark_text','remark_type','remark_date','remark_by','remark_by_name','id'));
	
	$remark_details 		->	fields('r',array('name'));
	$remark_details			->	condition('Rm.remark_to',$serial_no_from_v,'=');
	$remark_details			->	condition('Rm.particular_id',$license_renewal_id,'=');
	$remark_details			->	condition('Rm.flag','R','=');
	$remark_or				=	db_or();
	$remark_or				->	condition('Rm.remark_by_role',4);
	$remark_or				->	condition('Rm.remark_by_role',7);
	$remark_details			->	condition($remark_or);
	$remark_details			->	orderBy('Rm.id','DESC');
	$remark_details_result	=	$remark_details -> execute()->fetchAll();


	foreach($remark_details_result as $row){
		$sl_no=$sl_no+1;
		$remark_by=$row->remark_by_name.'('.$row->name.')';
		
		if($row->remark_type == 'B'){
				$current_status = '<span class="backed" title="Back for Rectification"></span>';
			}
			if($row->remark_type == 'FW'){
				$current_status = '<span class="forward">Forwarded</span>';
			}
			if($row->remark_type == 'A'){
				$current_status = '<span class="feespending" title="Fees Pending"></span>';
			}
			if($row->remark_type == 'C'){
				$current_status = '<span class="callapplicant" title="Call Applicant"></span>';
			}
			if($row->remark_type == 'I'){
				$current_status = '<span class="issued" title="Certificate Issued"></span>';
			}
			if($row->remark_type == 'P'){
				$current_status = '<span class="feespaid" title="Fees Paid"></span>';
			}
			if($row->remark_type == 'U'){
				$current_status = '<span class="finalsubmit" title="Final Submit"></span>';
			}
			if($row->remark_type == 'R'){
				$current_status = '<span class="reject" title="Rejected"></span>';
			}
			if($row->remark_type == 'BI'){
				$current_status = '<span class="backtoins" title="Back to Inspector"></span>';
			}
			
		$query 	= 	db_select('l_remark_license', 'lrl')
				->	fields('lrl', array('remark_by','id'))
				->	condition('lrl.remark_to', $serial_no_from_v)
				->	condition('lrl.particular_id', $license_renewal_id)
				->	condition('lrl.flag', 'R')
				->	orderBy('id', 'DESC')
			    ->  extend('PagerDefault')->limit(1);
		$remark_result = $query->execute()->fetchAssoc();
						 
	 	$remark_by_id = $remark_result['remark_by']; 
		$remark_id = $remark_result['id']; 
		$application_details	= contractor_activity_tag_del($serial_no_from_v,$created_by);
		//print_r($application_details);die($created_by);
		$flag_application		= $application_details['flagapplication'];
		//  $user_id;
		if($flag_application == 'R'){
		if($user->uid == $remark_by_id && $row->id == $remark_id ){
			$url 				= 'renewal-remarks-delete/'.encryption_decryption_fun('encrypt',$row->id).'/'.encryption_decryption_fun('encrypt',$license_renewal_id).'/'.encryption_decryption_fun('encrypt',$license_id).'/'.encryption_decryption_fun('encrypt',$serial_no_from_v).'/'.encryption_decryption_fun('encrypt',$contractor_particular_id);			
			$delete				= l('<div id="delBtnrenewalremark" class="delete-btn active-del" >'.t('Delete').'</div>',$url,array('html' => TRUE));	
		}else{
			$delete = '<div id="delBtn" class="delete-btn disable-del">'.t('Delete').'</div>';
			
		}
		}else{
			$delete = '<div id="delBtn" class="delete-btn disable-del">'.t('Delete').'</div>';
		}
	
		$rows[] = array(
					array('data' => $sl_no, 'width' => '8%', 'class' => array('center') ),
					array('data' => $row->remark_text, 'align' => 'left' ),
					array('data' => $current_status, 'width' => '12%' ),
					array('data' => $from_v_no, 'width' => '17%' ),
					array('data' =>date('dS M, Y', $row->remark_date), 'width' => '13%' ),
					array('data' =>$remark_by, 'width' => '13%' ),
					array('data' =>$delete, 'width' => '9%', 'class' => array('center'))
				);
	}
	
	$variables = array(
					'attributes' => array('class' => array('clra_applications','view-act-rules-table')), 
					'header' 	 => $header,
					'rows'		 => $rows,
					'empty'		 => t('No Remark found')
				);
	$output = theme('datatable', $variables);
	/*$output.='<style type="text/css">.div-head{font-weight:bold;font-size:16px;text-align:center;font-family:Times New Roman;padding:10px; margin-top: 50px;}</style>
				<br><br>
				<table cellpadding="0" cellspacing="0" border="0" class="view-act-rules-table">
				<tr><td colspan="3" class="center"><strong>STATUS AND ITS DESCRIPTIONS</strong></td></tr>
				<tr>
				<td class="center"><span class="finalsubmit">Final Submit</span></br>Final Submit By Applicant</td>
				<td class="center"><span class="pending">Backed</span></br>Back for correction</td>
				<td class="center"><span class="forward">Forwarded</span></br>Forward to ALC by Inspector</td>
				</tr>
				<tr>
				<td class="center"><span class="backtoins">Back to Inspector</span></br>Back to Inspector</td>
				<td class="center"><span class="feespending">Fees Pending</span></br>Fees Pending</td>
				<td class="center"><span class="call_applicant">Call Applicant</span></br>Call applicant to ALC office</td>
				</tr>
				<tr>
				<td class="center"><span class="feespaid">Fees Paid</span></br>Transaction Successful</td>
				<td class="center"><span class="issued">Issued</span></br>Issued Registration Certificate</td>
				<td class="center"><span class="reject">Reject</span></br>Rejected Application</td>
				</tr>
				</table>';*/
	 return $output;			
}

?>