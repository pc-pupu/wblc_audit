<?php
drupal_add_js(drupal_get_path('module', 'alc').'/js/mycustom.js');
function alc_view_backlog_contractor_license_renewal($form, &$form_state, $licenserenewalid='' , $serial_no_fromv='' , $contractor_particularid='', $licenseid_hash=''){
	
	
	global $base_root, $base_path, $user;
	$is_backlog_renewal_date_exists = 0;
	$uri3							= explode('#',$licenseid_hash);
	$licenseid						= $uri3[0];
	$uriID							= $uri3[1];
	if(empty($licenseid)){
		$licenseid					= $licenseid_hash;
		$uriID	 					= "";
	}
    $license_renewal_id 			= encryption_decryption_fun('decrypt', $licenserenewalid);
	$serial_no_from_v 				= encryption_decryption_fun('decrypt', $serial_no_fromv);
	$contractor_particular_id		= encryption_decryption_fun('decrypt', $contractor_particularid);
	$license_id						= encryption_decryption_fun('decrypt', $licenseid);	
	
	
	
	$get_remark 					= 	db_select('l_remark_license', 'lrl');
	$get_remark						->	fields('lrl', array('remark_field_title'));
	$get_remark						->	condition('lrl.particular_id', $license_renewal_id);
	$get_remark						->	condition('lrl.flag','R');
	
	$get_remark						->	condition('lrl.is_active',1);
	
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
	 $created_by 					= 	$status_renewal->created_by;	

	if(!empty($renewal_certificate_id)){
		$upload_certificates_id 		= db_select('file_managed','fm');
		$upload_certificates_id			-> fields('fm',array());
		$upload_certificates_id			-> condition('fid',$renewal_certificate_id);
		
		$upload_certificates_file		= $upload_certificates_id	-> execute()->fetchAssoc();
						
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
	
	$get_remark->condition('remark_license.is_active', 1);
	
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
		}if($row != "backlog_renewal_date" && empty($recommended_date_expire)){
			$is_backlog_renewal_date_exists = 0;
			break;
		}if($row != "backlog_renewal_date" && !empty($recommended_date_expire)){
			$is_backlog_renewal_date_exists = 1;
			break;
		}
	}
	
	$get_remark_type = 	db_select('l_remark_license', 'lrl');
	$get_remark_type->fields('lrl', array());
	$get_remark_type->condition('lrl.particular_id', $license_renewal_id);
	$get_remark_type->condition('lrl.remark_to', $serial_no_from_v);
	$get_remark_type->condition('lrl.flag','R');
	$get_remark_type->condition('lrl.is_active',1);
	$get_remark_type->orderBy('lrl.id','DESC');
	$get_remark_type->range(0, 1);
	$remark_result	=	$get_remark_type->execute()->fetchAssoc();
	$applicant_status 	= 	$remark_result['remark_type'];	
	
	if(!empty($applicant_status)){
			
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
			
			if( $applicant_status == "P" && $remark_result['remark_by_role'] == 8 ){
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
				
			if( $applicant_status == "P" && $remark_result['remark_by_role'] == 4){
				 $current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i>CURRENT STATUS: Back for Rectification(FORM-VII)</h4><p>FORM-VII is sent back for rectification. After modification by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
			
			}
		}
		
	
	$form['#attributes'] 								= array('enctype' 		=> 'multipart/form-data');
	
	$form['backlog_renewal']['markup_data']				= array(
															'#markup' 			=> getRenewalTable($serial_no_from_v,$license_renewal_id,$contractor_particular_id,$license_id),
															'#type' 			=> 'markup'
														);
													
	$form['backlog_renewal']['show_status'] 			= array(
															'#type' 			=> 'markup',
															'#markup'			=> $current_status,
															);
				
	$form['backlog_renewal']['formV']					= array(		
															'#type'				=> 'hidden',
															'#default_value' 	=> !empty($serial_no_fromv) ? $serial_no_fromv : '',
															);
				
	$form['backlog_renewal']['renewal_id']				= array(		
															'#type'				=> 'hidden',
															'#default_value' 	=> !empty($licenserenewalid) ? $licenserenewalid : '',
															);
				
	$form['backlog_renewal']['remark_alc_text_renewal']	= array(		
															'#type'				=> 'hidden',
															'#default_value' 	=> !empty($get_remark_result_fields) ? $get_remark_result_fields : '',
															);
														
	$form['backlog_renewal']['renewal_fees']			= array(		
															'#type'				=> 'hidden',
															'#default_value'	=> !empty($reneal_fees) ? $reneal_fees : '',
															);
															
	$form['backlog_renewal']['contractor_particularid']	= array(		
															'#type'				=> 'hidden',
															'#default_value' 	=> !empty($contractor_particularid) ? $contractor_particularid : '',
															);
				
	$form['backlog_renewal']['licenseid']				= array(		
															'#type'				=> 'hidden',
															'#default_value' 	=> !empty($licenseid) ? $licenseid : '',
															);
				
	$form['backlog_renewal']['backlog_renewal_date']	= array(		
															'#type'				=> 'hidden',
															'#default_value' 	=> !empty($backlog_renewal_date) ? $backlog_renewal_date : '',
															);
														
	if ($get_status_renewal != 'B' && $get_status_renewal != 'A' && $get_status_renewal != 'I' /*&& $get_status_renewal != 'P'*/ && $applicant_status != 'R'){
		
		$form['backlog_renewal']['markup1']		= array(
															'#prefix'			=> '<div class="box box-default box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">ACTIONS AND REMARKS</h3></div><div class="box-body">',
															'#type' 			=> 'markup',
														);
		if(!empty($uriID)){
		if($uriID == "HI"){
		
				$message= drupal_set_message_custom(t('Remark is successfully deleted'));
		}if($uriID == "hello"){
				$message= drupal_set_message_custom(t('Remark is successfully saved'));
		}if($uriID == "fromv"){
			$message= drupal_set_message_custom(t('Please verify the <strong>"Form VII" by checking  check box in <strong>"Is Verified?"</strong> column then you can proceed for Issue Certificate of application remark.'),'error');
		}
				$form['backlog_renewal']['MARKUP_1']	= array(
										'#prefix' 			=> '<div id="remark_view">',		
										'#type'				=> 'markup',
										'#markup'			=>	$message,
										 '#suffix' 			=> '</div>'
										
						);
		}
		
		$form['backlog_renewal']['remark_type']		= array(
														'#prefix' 			=> '<div class="col-md-3">&nbsp;</div><div class="form-custom col-md-6"><label class="input">',
														'#suffix' 			=> '<i></i></label>',	
														'#title'			=> 'Please Select Actions',
														'#type'				=> 'select',
														'#required' 		=>  TRUE,
														'#options'			=>  renewal_actions_dropdown_by_alc($get_status_renewal),
														'#ajax'				=>  array(
																					'event' 			=> 'change',
																					'effect' 			=> 'fade',			
																					'callback'			=> 'action_field',
																					'progress'			=> '',
																				),
																);
		$value_action_type		= isset($form_state['values']['remark_type']) ? $form_state['values']['remark_type'] : '';	
		
		$form['backlog_renewal']['recomented_date'] 	= array(
														  '#prefix' 		=> '<div id="alc_recomented_date">',
														  '#suffix' 		=> '</div>'
														);
														
		$form['backlog_renewal']['download_certificates']= array(
														  '#prefix' 		=> '<div id="download_certificates">',
														  '#suffix' 		=> '</div>'
														);

	     $form['backlog_renewal']['certificates'] 		= array(
														  '#prefix' 		=> '<div id="edit-certificates-ajax-wrapper">',
														  '#suffix' 		=> '</div>'
														);	
														
		 $form['backlog_renewal']['check_declaration'] 	= array(
														  '#prefix' 		=> '<div id="check_declaration_id">',
														  '#suffix' 		=> '</div>'
														);	
														
		$form['backlog_renewal']['hidden_action'] 		= array(
														  '#prefix' 		=> '<div id="renewal_hidden_action_div">',
														  '#suffix' 		=> '</div>'
														);	
														
		$form['backlog_renewal']['remarks_text'] 		= array(
														  '#prefix' 		=> '<div id="renewal_remarks_text_div">',
														  '#suffix' 		=> '</div>'
														);
														
		if(!empty($value_action_type) && $value_action_type == 'I'){
			
			if($is_backlog_renewal_date_exists == 0 || empty($is_backlog_renewal_date_exists)){
				
				$form['backlog_renewal']['recomented_date']= array(
															'#prefix'		=>'<div id="alc_recomented_date"><label class="date">',
															'#suffix'		=>'</label></div>',
															'#title'		=>'Verify and recommend date of expiry of the previous license',
															'#type' 		=>'date',
															'#default_value'=>!empty($recommended_date_expire) ? $recommended_date_expire : '',
															'#required'		=>	TRUE,
														);
														
				$form['backlog_renewal']['check_declaration']= array(
															'#prefix' 		=> '<div id="check_declaration_id"><span style="color:#FF0000">',
															'#suffix' 		=> '</span></div>',
															'#type' 		=> 'checkbox',
															'#title' 		=> t('<strong>Declaration: Please be informed that the date being recommended by you shall be full and final and the renewal of license will be generated from the date as recommended, once the renewal certificate is issued no changes can not be done further.</strong>'),
															'#required' 	=> TRUE,
															'#attributes'	=> array('onclick' => 'return checkFinalAppCheckBox()'),
														);
														
				$form['backlog_renewal']['is_recommanden_date']= array(		
															'#type'			=> 'hidden',
															'#default_value' => '1',
															);	
				
			}
			
			
														
			if($is_backlog_renewal_date_exists == 1){
				
			  $form['backlog_renewal']['download_certificates']	= array(
															'#prefix'		=> '<div id="download_certificates">',
															'#markup' 		=> l(t('<i class="fa fa-download" aria-hidden="true"></i>Download Renewal Certificate'),'renewal-certificates/'.$serial_no_fromv.'/'.$contractor_particularid.'/'.$licenseid.'/'.$licenserenewalid, array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn')))),
															'#suffix' 		=> '</div>',
													 );
													 
			 $uri='public://upload/alc_upload_certificates_renewal_license/';
			 $target_file=$uri.$value_frm_v;
			 if (!file_exists($target_file)) {
				 if( mkdir($target_file, 0777, true)){
			
		 		 }
			}													
			$form['backlog_renewal']['certificates'] 	= array(
															'#type' 		=> 'managed_file',
															'#title'		=>	'Upload Signed Certificate of Renewal of Licence :-',
															'#upload_validators'=> array('file_validate_extensions' => array('pdf')),
															'#upload_location' => $target_file,
															'#process' 		=> array('import_my_file_element_process'),
															'#attributes' 	=> array('id' => '','autocomplete' => 'off','class'=>array(''),'file_validate_size' => array(50*1024*1024)),
															'#prefix'		=> '<div id="edit-certificates-ajax-wrapper"><label class="input input-file">',
															'#suffix' 		=> '</label></div>',
															'#required' 	=>  TRUE,
																				  
														  );
										  
										  
			$form['backlog_renewal']['remarks_text']	= array(
															'#prefix' 		=> '<div id="renewal_remarks_text_div"><label class="textarea">',
															'#suffix' 		=> '</label></div>',
															'#title'		=> 'Remarks',
															'#type'			=> 'textarea',
															'#rows'			=>  2,
															'#required' 	=>  TRUE,
													);
			}
														
			
				
			
												
			
				
		}
														
		if(!empty($value_action_type) && ( $value_action_type == 'B' || $value_action_type == 'A' ) ){
		
			if(!empty($recommended_date_expire)){
				$form['backlog_renewal']['recomented_date'] = array(
															'#prefix'			=>'<div id="alc_recomented_date"><label class="date">',
															'#suffix'			=>'</label></div>',
															'#title'			=>'Verify and recommend date of expiry of the previous license',
															'#type' 			=>'date',
															'#required'			=> TRUE,
															'#default_value' 	=> array(
																					   'month' 	=> format_date(strtotime($recommended_date_expire), 'custom',  'n'),
																					   'day' 	=> format_date(strtotime($recommended_date_expire), 'custom',  'j'),
																					   'year' 	=> format_date(strtotime($recommended_date_expire), 'custom',  'Y'),
																						),
															);
			}else{
				
				$form['backlog_renewal']['recomented_date'] = array(
															'#prefix'			=>'<div id="alc_recomented_date"><label class="date">',
															'#suffix'			=>'</label></div>',
															'#title'			=>'Verify and recommend date of expiry of the previous license',
															'#type' 			=> 'date',
															'#required'			=>	TRUE,
															);
			}
			
			$form['backlog_renewal']['remarks_text']		= array(
																'#prefix' 		=> '<div id="renewal_remarks_text_div"><label class="textarea">',
																'#suffix' 		=> '</label></div>',
																'#title'		=> 'Remarks',
																'#type'			=> 'textarea',
																'#rows'			=>  2,
																'#required' 	=>  TRUE,
														);
		}
		
		if(!empty($value_action_type) && ( $value_action_type == 'BI' || $value_action_type == 'R' ) ){
			
			$form['backlog_renewal']['remarks_text']	= array(
									'#prefix' 				=> '<div id="renewal_remarks_text_div"><label class="textarea">',
									'#suffix' 				=> '</label></div>',
									'#title'				=> 'Remarks',
									'#type'					=> 'textarea',
									'#rows'					=>  2,
									'#required' 			=>  TRUE,
							);
		}
		
		$form['backlog_renewal']['submit'] 				= array (
															  '#suffix' 		=> '</div><div class="col-md-3">&nbsp;</div></div></div>',
															  '#type' 			=> 'submit',
															  '#attributes' 	=> array('class' => array('btn btn-primary pull-left acremark-btn')),
															  '#value' 			=> 'Submit'
															);										
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
				$form['backlog_renewal']['MARKUP_1']	= array(
							 '#prefix' 			=> '<div id="remark_view">',		
							 '#type'			=> 'markup',
							 '#markup'			=>	$message,
							 '#suffix' 			=> '</div>'
										
						);
		}
		
		$form['backlog_renewal']['show_certificates'] 	= array(
															'#prefix' 			=> '<div id="show_certificates_id">',
															'#suffix'		    =>'</div>',
															'#type' 			=> 'markup',
															'#markup' 			=> '<strong>View Certificate of Renewal of Licence</strong> <a href="'.$GLOBALS['base_url'].'/sites/default/files/'.$upload_certificates_file_url.'" target="_blank"><img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>'
														  );
	}

	$form['backlog_renewal']['show_remark_details'] 	= array(
															  '#prefix'			=>'<div class="box box-default box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">REMARK SUMMARY</h3></div><div class="box-body">',
															  '#type' 			=> 'markup',
															  '#markup'			=> renewal_remark_details($serial_no_from_v,$license_renewal_id,$license_id,$contractor_particular_id,$created_by),
															  '#suffix' 		=> '</div></div>',
									 						 );				   
	return $form;
	
}

function getRenewalTable($serial_no_from_v='',$license_renewal_id='' ,$contractor_particular_id='', $license_id =''){
	
	drupal_add_js(drupal_get_path('module', 'alc').'/js/mycustom.js');
	
	global $base_root, $base_path, $user;
	
   
	$query 				= 	db_select('l_contractor_info_master', 'c_info_master');
 	$query				->	fields('c_info_master',array())->condition('id',$serial_no_from_v,'=');
	$query				->	condition('act_id','1','=');
	$query_result		=	$query->execute();
	
	if( $query_result->rowCount() > 0 ){
		
		foreach($query_result as $row){
			$application_id 				= 	$row->application_id;
			$applicant_id 					= 	$row->user_id;
			$name_of_contractor				=	$row->name_of_contractor;
			$address_of_contractor_pe		=	$row->address_of_contractor;
			
		}
	}
	
	/** Establishment Nature of work  ***/
	
	
	$query_work 							= 	db_select('l_clra_reg_nature_of_work', 'c_reg_nature');
	$query_work								->	leftJoin('contractor_works', 'con_work','con_work.contractor_work_id = c_reg_nature.e_nature_of_work');  
	$query_work								->	fields('c_reg_nature',array('identification_number','user_id','act_id'));
	$query_work								->	fields('con_work',array('cont_work_name'));
	$query_work								->	condition('c_reg_nature.application_id',$application_id,'=');
	$query_work								->	condition('c_reg_nature.act_id','1','=');
	$result_work 							= 	$query_work->execute();
	 
	$nature_work							=	"";
	
	if( $result_work->rowCount() > 0 ){
		foreach($result_work as $objwork){
			$nature_work_list				=	$nature_work_list.$objwork->cont_work_name.', ';				  
		}
	}
	$nature_work							= 	substr($nature_work_list, 0, -2);
	
	/** Establishment Details    ***/

	$establishment_details_query 			= 	db_select('l_clra_registration_master', 'c_r_m');
	$establishment_details_query			->	fields('c_r_m',array('id','e_name','loc_e_name','registration_number','act_id','loc_e_areatype','registration_date','loc_emp_areatype','full_name_principal_emp','address_principal_emp','certificates_fid'))->condition('id',$application_id,'=');
	$establishment_details_query			->	condition('c_r_m.act_id','1','=');
	$establishment_details_result 			= 	$establishment_details_query->execute();
	
	if( $establishment_details_result->rowCount() > 0 ){
		
		foreach($establishment_details_result as $obj){
			
 			$id							=	$obj->id;
			$registration_number		=	$obj->registration_number;
 			$loc_e_name					=	$obj->loc_e_name;	
			$loc_e_areatype				=	$obj->loc_e_areatype;
			$e_name						=	$obj->e_name;
			$registration_date			=	$obj->registration_date;
			$loc_emp_areatype			=	$obj->loc_emp_areatype;
			$full_name_principal_emp	=	$obj->full_name_principal_emp;
			$address_principal_emp		=	$obj->address_principal_emp;
			$certificates_fid			=	$obj->certificates_fid;	
		}
	}
	$pe_certificates			=	db_select('file_managed','fm');
	$pe_certificates 			->	fields('fm',array());
	$pe_certificates			->	condition('fm.fid',$certificates_fid,'=');
	$uploaded_pe_certificates	=	$pe_certificates -> execute()->fetchAssoc();
	
	if(!empty($uploaded_pe_certificates)){ 
			$url			 	= 	explode('//',$uploaded_pe_certificates['uri']);
   			$certificates_url		= 	$url[1];
			
	}
					
	$establishment_address				=	"";
	$establishment_address				=	miscellaneous_establishment_address_form($loc_e_areatype,$serial_no_from_v,$application_id);// call from miscellaneous module		
	$establishment_address				=	$e_name.'<br/>'.$loc_e_name.'<br>'.$establishment_address;
	
	
	$principal_address					=	miscellaneous_principal_employer($loc_emp_areatype,$serial_no_from_v,$application_id);//----miscellaneous module
	$principal_details					=	$full_name_principal_emp."<br>".$address_principal_emp."<br>".$principal_address;

	
	$registration_date_timestamp 		= 	strtotime($registration_date);
	$formatted_reg_date 				= 	date('m/d/Y', $registration_date_timestamp);
	
	$establish_details_reg				=	"";
	$establish_details_reg				=	$establish_details_reg."Registration Number: ".$registration_number."<br/>"."Date of Certificates: ".$formatted_reg_date."\nUnder The acts of CLRA";	
	
	
	//** Contract Labour Nature of work **/
	
	
	if(!empty($serial_no_from_v)){
		$result_labourwork 				= 	get_nature_of_work_contrcator($application_id,$serial_no_from_v); //---miscellaneous module	
  		$labour_nature_work				=	"";
		if($result_labourwork->rowCount() > 0 ){
			foreach($result_labourwork as $objwork){
				$labour_nature_work		=	$labour_nature_work.$objwork->cont_work_name.', ';			  
			}
			$labour_nature_work			=	substr($labour_nature_work, 0, -2);
		}
	}
	//-----------------------------------END work---------------------------------------------------------

	$query 								= 	db_select('l_contractor_info_master', 'con_info_master'); 
 	$query								->	fields('con_info_master',array('id','user_id','act_id','est_date_of_work_of_each_labour_from_date','est_date_of_work_of_each_labour_to_date'));
  	$query								->	condition('con_info_master.id',$serial_no_from_v,'=');
   	$result 							= 	$query->execute()->fetchAssoc(); 
	
	
  	$startend_date						=	"";

	if( !empty($result) ){
		
		
			
			
			$startdate				=	$result['est_date_of_work_of_each_labour_from_date'];
			$enddate				=	$result['est_date_of_work_of_each_labour_to_date']; 
			
    
	 		$diff=round(abs(strtotime($startdate)-strtotime($enddate))/86400).' day(s)';
	
	 

			
			
		
	}

	$start_date							=	date('dS F y', strtotime($startdate));
	$end_date							=	date('dS F y', strtotime($enddate));
	$startend_date						=	$start_date.' To '.$end_date;
	//--------------------------------Fetch uploaded documents------------------------------
	
	$uploaded_doc			=	db_select('file_managed','fm');
	$uploaded_doc 			->	fields('fm',array());
	$uploaded_doc			->	condition('fm.fid',array($work_order_file_id,$frm_v_file_id,$residential_file_id),'IN');
	$uploaded_doc_result	=	$uploaded_doc -> execute()->fetchAll();

	
	//---------------------------Details of contractor------------------------------------
	
	$contractor_address					=	"";
	$query 								= 	db_select('l_particular_of_contract_labour', 'lpcl');
 	$query								->	fields('lpcl',array())->condition('id',$contractor_particular_id,'=');
	$result 							= 	$query->execute()->fetchAssoc(); 
	if(!empty($result)){
	
			$contractor_areatype						=	$result['contractor_areatype']; 
			$contractor_manager_areatype				=	$result['contractor_manager_areatype'];
			$work_order_file_id							=	$result['work_order_file_id'];
			$frm_v_file_id								=	$result['frm_v_file_id'];
			$residential_file_id						=	$result['residential_file_id'];
		
			
			$father_contarctor_name						=	$result['father_contarctor_name'];	
			$dob_contractor								=	$result['dob_contractor'];
			$dob_age_contractor							=	$result['age_contractor'];
			$address_of_manager							=	$result['address_of_manager']; 
			$contractor_max_no_of_labours_on_any_day	=	$result['contractor_max_no_of_labours_on_any_day']; 
			$address_line1_of_contractor				=	$result['address_of_contractor']; 
			$category_designation						=	$result['category_designation'];
			$manager_pin								=	$result['manager_pin'];
			$contractor_pin								=	$result['contractor_pin'];
			
			
			$unskilled_wages							=	$result['unskilled_wages'];
			$unskilled_rate_wages						=	$result['unskilled_rate_wages'];
		
		  	$semiskilled_wages							=	$result['semiskilled_wages'];
			$semiskilled_rate_wages						=	$result['semiskilled_rate_wages'];
			
			
			$skilled_wages								=	$result['skilled_wages'];
			$skilled_rate_wages							=	$result['skilled_rate_wages'];
			
			
			$highlyskilled_wages						=	$result['highlyskilled_wages'];
			$highlyskilled_rate_wages					=	$result['highlyskilled_rate_wages'];
			
			
		  	$hours_work									= 	$result['hours_work'];
		  	$overtime									= 	$result['overtime'];
		  	$overtime_wages								= 	$result['overtime_wages'];
		  	$weekly_holiday								= 	$result['weekly_holiday'];
		  	$no_holiday									= 	$result['no_holiday'];
	  
		  	$annual_leave								= 	$result['annual_leave'];
		  	$annual_leave_no							= 	$result['annual_leave_no'];
		  	$casual_leave								= 	$result['casual_leave'];
		  	$casual_leave_no							= 	$result['casual_leave_no'];
		  	$sick_leave									= 	$result['sick_leave'];
		 	$sick_leave_no								= 	$result['sick_leave_no'];
		  	$other_leave								= 	$result['other_leave'];
		 	$other_leave_no								= 	$result['other_leave_no'];
			$maternity_leave_no							=	$result['maternity_leave_no'];
			$signed_license_id							=	$result['signed_license_id'];
			
			$particularid								=	$result['id'];
			$name_of_agent								=	$result['name_of_agent'];
			$address_of_manager							=	$result['address_of_manager'];
			$contractor_convicted						=	$result['contractor_convicted'];
			$category_of_contractor						=	$result['category_of_contractor'];
			$worksite_address_line						=	$result['worksite_address_line'];
			
			
			
			$fetch_license_data 						= 	db_select('l_contractor_license_application', 'license');
  			$fetch_license_data							->	fields('license',array('id','backlog_license_date','backlog_license_no','contractor_user_id','is_backlog_license','contractor_license_number','application_id','user_id','application_id'));
  			$fetch_license_data							->	condition('license.contractor_particular_id',$particularid,'=');
			$license_data 								= 	$fetch_license_data->execute()->fetchAssoc();
			//print_r($license_data);	
			$license_contractor_id 						= 	$license_data['id'];
			$backlog_license_date 						= 	$license_data['backlog_license_date'];
			$backlog_license_no 						= 	$license_data['backlog_license_no'];
			$contractor_user_id							=	$license_data['contractor_user_id'];
			$is_backlog_license							=	$license_data['is_backlog_license'];
			$contractor_license_number					=	$license_data['contractor_license_number'];
			$application_id								=	$license_data['application_id'];
			$user_id									=	$license_data['user_id'];
			$application_id									=	$license_data['application_id'];
			if(!empty($license_contractor_id)){
		
					$renewal_data						=	fetch_all_data_contractor_license_apply_renweal($license_contractor_id,$contractor_user_id);
					//print_r($renewal_data);die;
					//echo 'hi';die();
					$license_reneal_fees				=	$renewal_data['license_reneal_fees'];
					$earned_leave_no					=	$renewal_data['earned_leave_no'];
					$spred_over							=	$renewal_data['spred_over'];
					$holiday_wages						=	$renewal_data['holiday_wages'];
					$special_benifites					=	$renewal_data['special_benifites'];
					$state_insurance					=	$renewal_data['state_insurance'];
					$miscellaneous_provisions			=	$renewal_data['miscellaneous_provisions'];
					$backlog_renewal_date				=	$renewal_data['backlog_renewal_date'];
					$backlog_renewal_fees				=	$renewal_data['backlog_renewal_fees'];
					$renewal_application_final_status	= 	$renewal_data['renewal_application_final_status'];
					$renewal_appliaction_status			= 	$renewal_data['renewal_appliaction_status'];	
					$upload_backlog_certificate_id		= 	$renewal_data['upload_backlog_certificate'];
					$upload_signed_form_id				= 	$renewal_data['upload_signed_form'];
					$increnent_fees						= 	$renewal_data['increnent_fees'];
					$renewal_fees						= 	$renewal_data['license_reneal_fees'];	
					$renewal_security_new				= 	$renewal_data['renewal_security_new'];	
					$created_date						= 	$renewal_data['created_date'];
					$is_coparative						= 	!empty($renewal_data['is_coparative']) ? 'Yes' : 'No';	
					
			}
	
		
		$pervious_license_detais			=	'';

		$pervious_license_detais			=	license_details($serial_no_from_v,$contractor_user_id);

		//print_r($pervious_license_detais);die();
			
		$special_benifites_show	=	empty($special_benifites)?' Not Applicable':$special_benifites;
		$state_insurance_show	=	empty($state_insurance)?' Not Applicable':$state_insurance;
		$miscellaneous_provisions_show	=	empty($miscellaneous_provisions)?'Not Applicable' : $miscellaneous_provisions;
			
		/*if($category_of_contractor==1 && !empty($category_of_contractor)){
			$title = 'Commencement date of business';
			$age_contractor='';
		}else{
			$title = 'Date of Birth and age (incase of individuals)';
			$age_contractor= 'Age: '.$dob_age_contractor.' Years';
		}*/
			$title = 'Date of Birth and age (incase of individuals)';
			$age_contractor= !empty($dob_contractor)?'DOB:-'.date('dS M, Y',strtotime($dob_contractor)).'<br>Age:-'.(!empty($dob_age_contractor)?$dob_age_contractor:'Nil') :'DOB:-Nil<br> Age:-'.(!empty($dob_age_contractor)?$dob_age_contractor:'Nil');
			
		if($contractor_convicted=='1')
			$details_contractor_convicted=	$result['details_contractor_convicted'];
		else
			$details_contractor_convicted=	'NO';

		$contractor_revoking			=	$result['contractor_revoking'];
		$details_contractor_revoking		=	$result['details_contractor_revoking'];	
	
		

		$contractor_previous_employer	=	$result['contractor_previous_employer'];
		
		if($contractor_previous_employer=='1')
			$details_previous_employer= $result['details_previous_employer']; 
		else
			$details_previous_employer='NO';
		
		$amount_of_licence				=	$result['amount_of_licence'];
		$amount_of_security_deposit		=	$result['amount_of_security_deposit'];
			
		
	}
	
	$contractor_address				=	miscellaneous_contractor_certificate_address_form($contractor_areatype='',$serial_no_from_v,$contractor_particular_id);//call from miscellaneous
		$contractor_manager_address		=	miscellaneous_contractor_manager_address_form($contractor_manager_areatype='',$serial_no_from_v,$contractor_user_id)	;
	
	if($father_contarctor_name != ''){
		$father_contarctor_name = $father_contarctor_name.'<br/>';
	}else{
		$father_contarctor_name	= '';
	}
	//---------------------------------------End contractor Address-----------------------------------	
	$output								=	'';
	
	
	// Establishment details


	//Labour
	
	$rest_holiday = substr($no_holiday, 0, -1);
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

	//--------------------------------Fetch uploaded documents------------------------------
	
	$uploaded_workorder			=	db_select('file_managed','fm');
	$uploaded_workorder 		->	fields('fm',array());
	$uploaded_workorder			->	condition('fm.fid',$work_order_file_id,'=');
	$uploaded_workorder_file	=	$uploaded_workorder -> execute()->fetchAssoc();
	
	if(!empty($uploaded_workorder_file)){ 
			$url			 	= 	explode('//',$uploaded_workorder_file['uri']);
   			$work_order_url		= 	$url[1];
			$workorder_file		=	$uploaded_workorder_file['filename'];
	}
	
	
	$uploaded_frmv				=	db_select('file_managed','fm');
	$uploaded_frmv 				->	fields('fm',array());
	$uploaded_frmv				->	condition('fm.fid',$frm_v_file_id,'=');
	$uploaded_frmv_file			=	$uploaded_frmv -> execute()->fetchAssoc();
	
	if(!empty($uploaded_frmv_file)){ 
			$url 				= explode('//',$uploaded_frmv_file['uri']);
   			$frmv_url			= $url[1];
			$frmv_file			= $uploaded_frmv_file['filename'];
	}
	
	$uploaded_residential		=	db_select('file_managed','fm');
	$uploaded_residential 		->	fields('fm',array());
	$uploaded_residential		->	condition('fm.fid',$residential_file_id,'=');
	$uploaded_residential_file	=	$uploaded_residential -> execute()->fetchAssoc();
	
	if(!empty($uploaded_residential_file)){ 
			$url				= explode('//',$uploaded_residential_file['uri']);
   			$residential_url	= $url[1];
			$residential_file	= $uploaded_residential_file['filename'];
	}
	
	
	$upload_formiv				=	db_select('file_managed','fm');
	$upload_formiv 				->	fields('fm',array());
	$upload_formiv				->	condition('fm.fid',$signed_license_id,'=');
	$upload_formiv_file			=	$upload_formiv -> execute()->fetchAssoc();
	
	if(!empty($upload_formiv_file)){ 
			$url				= explode('//',$upload_formiv_file['uri']);
   			$formiv_url			= $url[1];
			$formiv_file		= $upload_formiv_file['filename'];
	}
		

	
	
	$upload_backlog_certificates				=	db_select('file_managed','fm');
	$upload_backlog_certificates 				->	fields('fm',array());
	$upload_backlog_certificates				->	condition('fm.fid',$upload_backlog_certificate_id,'=');
	$upload_backlog_certificates_file			=	$upload_backlog_certificates -> execute()->fetchAssoc();
	
	if(!empty($upload_backlog_certificates_file)){ 
			$url								= explode('//',$upload_backlog_certificates_file['uri']);
   			$backlog_certificates_url			= $url[1];
			$backlog_certificates_file			= $upload_backlog_certificates_file['filename'];
	}
	
	
	$upload_signed_form_vii				=	db_select('file_managed','fm');
	$upload_signed_form_vii 				->	fields('fm',array());
	$upload_signed_form_vii				->	condition('fm.fid',$upload_signed_form_id,'=');
	$upload_signed_form_vii_file			=	$upload_signed_form_vii -> execute()->fetchAssoc();
	
	if(!empty($upload_signed_form_vii_file)){ 
			$url								= explode('//',$upload_signed_form_vii_file['uri']);
   			$upload_signed_form_vii_url			= $url[1];
			$upload_signed_form_vii_file_name			= $upload_signed_form_vii_file['filename'];
	}
	
	$father_contarctor_name=!empty($father_contarctor_name)?$father_contarctor_name : 'Nil';
	$get_remark = db_select('l_remark_license', 'remark_license');
		$get_remark->fields('remark_license', array('remark_field_title'));
		$get_remark->condition('remark_license.remark_to', $serial_no_from_v);
		$get_remark->condition('remark_license.particular_id', $license_renewal_id);
		$get_remark->condition('remark_license.is_active', 1);
		$get_remark->where("((remark_license.remark_by_role = 4) OR (remark_license.remark_by_role = 7))");
		$get_remark->orderBy('remark_license.id','DESC');
		$get_remark->range(0, 1);
		$get_remark_result = $get_remark->execute();

		

		$editable_data = $get_remark_result->fetchAssoc();//print_r($get_remark_result);die($serial_no_from_v);
		
		$remark_field_text_arr = explode(',', $editable_data['remark_field_title']);

		for($i=0; $i<count($remark_field_text_arr); $i++){
					$fieldname = $remark_field_text_arr[$i];
						
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
	if($backlog_license_date<='2015-11-27'){
			if(empty($backlog_renewal_date)){
				$appliyyear					= date('Y', strtotime($created_date ));
				$backlog_license_year		= date('Y', strtotime($backlog_license_date));
				$backlog_license_month		= date('m', strtotime($backlog_license_date));
				$backlog_license_day		= date('d', strtotime($backlog_license_date));
				$next_renweal_date			= ($appliyyear+1).'-12-31';
				$renewal_expaire 			=  '31st Dec,'.$backlog_license_year;;	
			}else{
				$renewal_expaire			= date("dS M, Y",strtotime($backlog_renewal_date));	
			}
			
		}else{
			
				if(empty($backlog_renewal_date)){
					$backlog_license_year		= date('Y', strtotime($backlog_license_date));
					$backlog_license_month		= date('m', strtotime($backlog_license_date));
					$backlog_license_day		= date('d', strtotime($backlog_license_date));
					$next_renweal_date			= ($backlog_license_year+1).'-'.$backlog_license_month.'-'.($backlog_license_day-1);
					
					$renewal_expaire			= date("dS M, Y",strtotime($next_renweal_date));
				}else{
				$renewal_expaire			= date("dS M, Y",strtotime($backlog_renewal_date));	
			}
			
					
		}
	

	
	
	if($renewal_security_new>$amount_of_security_deposit){
		$new_security_amount = $renewal_security_new -  $amount_of_security_deposit;
	}else{
		$new_security_amount = 0;
	}
	
	$worksite_address								=	$worksite_address_line.'<br>'.miscellaneous_worksite_address($serial_no_from_v,$contractor_particular_id);
	
	if(empty($annual_leave_no)){
						$annual_leave_no_show='Nil';
					}else{
						//$annual_leave_no_show=$annual_leave_no.' '.'day(s)';
						
						$annual_leave_no_show=$annual_leave_no ;
					}
					
					if(empty($casual_leave_no)){
						$casual_leave_no_show='Nil';
					}else{
						//$casual_leave_no_show=$casual_leave_no.' '.'day(s)';
						$casual_leave_no_show=$casual_leave_no ;
					}
					
					if(empty($sick_leave_no)){
						$sick_leave_no_show='Nil';
					}else{
						//$sick_leave_no_show=$sick_leave_no.' '.'day(s)';
						$sick_leave_no_show=$sick_leave_no;
					}
					
					if(empty($maternity_leave_no)){
						$maternity_leave_no_show='Nil';
					}else{
						//$maternity_leave_no_show=$maternity_leave_no.' '.'day(s)';
						$maternity_leave_no_show=$maternity_leave_no;
					}
					
					if(empty($other_leave_no)){
						$other_leave_no_show	=	'Nil';
					}else{
						// $other_leave_no_show	=	$other_leave_no.' '.'day(s)';
						 $other_leave_no_show	=	$other_leave_no;
					}
					if(empty($earned_leave_no)){
						$earned_leave_no_show	=	'Nil';
					}else{
						//$earned_leave_no_show=$earned_leave_no.' '.'day(s)';
						$earned_leave_no_show=$earned_leave_no;
					}
					if(empty($holiday_wages)){
						$holiday_wages_show='Nil';
					}else{
						$holiday_wages_show	=	$holiday_wages;
					}
	
	$extrafeesmsg = '';
	
				if($renewal_fees < $increnent_fees){ $extrafeesmsg = '[<i style="color:#f00;">including 25% extra</i>]';}	
				
	$contractor_pe		=	'';	
	$contractor_pe		=   $address_of_contractor_pe .'<br>'. miscellaneous_contractor_address_form($areatype='',$serial_no_from_v);//---call from miscellaneous module
	
	$transaction_details  			= 	db_select('l_principle_epayments_receive_data', 'lpd');
	$transaction_details			->	leftJoin('l_principle_epayments_data', 'lped', 'lpd.transaction_id = lped.identification_no');
	$transaction_details			->  fields('lped', array('identification_no', 'application_id'));
	$transaction_details			->  fields('lpd', array());
	$transaction_details			->	condition('lped.act_id', '13');
	$transaction_details			->	condition('lped.application_id',$license_renewal_id);
	$transaction_details			->	condition('lped.deposited_by',$contractor_user_id); 
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
	 
	/* $output	.= '<div class="box box-primary collapsed-box box-solid">
						<div class="box-header with-border">
						  <h3 class="box-title">1. Inputs are provided by Principal Employer [ Verified ]</h3>
						</div>
						<div class="box-body">
							 <div class="feedback-scroll">
								 <table class="table table-bordered">
									 <tr>
									  <th style="width:10%">Sl</th>
									  <th style="width:40%">Parameters</th>
									  <th style="width:40%">Inputs</th>
									</tr>
									<tr>
										<td colspan="3" style="font-size:15px; font-weight:600;">1. Particulars of Establishment where Contract Labour is to be employed</td>
									</tr>
									<tr>
										<td>1.(a)</td>
										<td width="40%">Name and address of establishment</td>
										<td>'.$establishment_address.'</td>
									</tr>
									<tr>
										<td>1.(b)</td>
										<td>Type of business, trade,industry, manufacture or occupation, which is carried on in the establishment</td>
										<td>'.$nature_work.'</td>
									</tr>
									<tr>
										<td>1.(c)</td>
										<td>Establishment Registration Details Under Particular Act</td>
										<td>'.$establish_details_reg.'</td>
									</tr>
									<tr>
										<td>1.(d)</td>
										<td>Name and address of the Principal Employer</td>
										<td>'.$principal_details.'</td>
									</tr>
									<tr>
										<td colspan="3" style="font-size:15px; font-weight:600;">2. Contractor Details</td>
									</tr>
									<tr>
										<td>2.(a)</td>
										<td>Name and address of Contractor</td>
										<td>'.$name_of_contractor.'<br/>'.$contractor_pe.'</td>
									</tr>
									<tr>
										<td>2.(b)</td>
										<td>Nature of work in which contract labour is employed or is to be employed in the establishment</td>
										<td>'.$labour_nature_work.'</td>
									</tr>
									<tr>
										<td>2.(c)</td>
										<td>Duration of the proposed contract work (give particulars of proposed date of ending) </td>
										<td>'.$startend_date.'<br /> Estimated Duration:  '.$diff.'</td>
									</tr>
									<tr>
										<td>2.(d)</td>
										<td>Maximum number of Contract Labour proposed to he employed in the establishment on any date. </td>
										<td>'.$contractor_max_no_of_labours_on_any_day.' ('.convertNumberToWord($contractor_max_no_of_labours_on_any_day).').'&nbsp;&nbsp; <font color="#006600"> <b>[ FEES IS CALCULATED BASED ON THIS VALUE ] </b></font></td>
									</tr>
								</table>
							 </div>
						</div>
					</div>
					
					<div class="box box-primary box-solid">
						<div class="box-header with-border">
						  <h3 class="box-title">3. Inputs are provided by Contractor for Renewal  [For Verification]</h3>
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
										<td>3. a)</td>
										<td>'.$title.'</td>
										<td>'.$age_contractor.'</td>
										<td><input type="checkbox" id="indivisual_company" class="renewal_alc_verified" '.$indivisual_company_check.' /></td>
									</tr>
									<tr>
										<td>3. b)</td>
										<td>Father&#13;s name of contractor (incase of individuals) </td>
										<td>'.$father_contarctor_name.'</td>
										<td><input type="checkbox" id="father_name"  class="renewal_alc_verified" '.$father_name_check.' /></td>
									</tr>
									<tr>
										<td>3. c)</td>
										<td><strong>C.Co-oparative Society</strong><font color="red">**</font></td>
										<td><strong>'.$is_coparative.'</strong></td>
										<td><input type="checkbox" id="is_co_oparative"  class="renewal_alc_verified" '.$is_co_oparative_check.' /></td>
									</tr>
									<tr>
										<td>3. d)</td>
										<td>Address of contractor</strong></td>
										<td>'.$address_line1_of_contractor.' <br />'. $contractor_address.'</td>
										<td><input type="checkbox" id="address_of_contractor" class="renewal_alc_verified" '.$address_of_contractor_check.' /></td>
									</tr>
								</table>
							 </div>
						</div>
					</div>*/
							
							
							
							
							
	 $output	.= '
	 <style type="text/css">
						table, td, th {border: 1px solid #9e8774;font-size:15px;font-family:Times New Roman;height:40px;margin-top:20px;}
						td{padding-left:13px;}
						th {background-color: #008BD1;color: white;}
						td:hover {background-color:#d4e3e5;}
						tr:nth-child(even) {background: #DBE5F0}
						tr:nth-child(odd) {background: #F1F1F1}
					 </style>
			   
			   
				 <table class="table table-bordered">
				
				<tr><td colspan="3" class="sub-title"><strong> Application preview of Renewal of license</strong>
					<br> N.B.:- All inputs are provided by Applicant
					<br>(Fields marked <font color="#FF0000">(**)</font> are very sensitive data. Please verify all information properly by checking all check boxes)</td>
				</tr>
				
 				<tr>
					<th width="35%">Parameters</th>
					<th>Inputs</th>
					<th width="15%">Is Verified? </th>
				</tr>
				 <tr>
				<td colspan="3" align="center"><strong>1.Information provided by Principal Employer</strong><br>(All information of this section have been provided by Principal Employer during Registration or Ammendment application,already verified by ALC)</td>
			 </tr>
  					
				<tr>
					<td>A.Name of the contrcator</td>
					<td>'.$name_of_contractor.'</td>
					<td><input type="checkbox" checked="checked" disabled="disabled" title="Already Verified" /><span style="color:#006600;">&nbsp;Verified</span></td>
				</tr>
				<tr>
					<td>B.Address of the contractor</td>
					<td>'.$contractor_pe.'</td>
					<td><input type="checkbox" checked="checked" disabled="disabled" title="Already Verified" /><span style="color:#006600;">&nbsp;Verified</span></td>
				</tr>
				<tr>
					<td colspan="3" align="center" class="sub-title"><strong>Particular of Establishment where Contract Labour is to be employed</strong</td>
  					</tr>
				<tr>
					<td>C.Name & address of the establishment</td>
					<td>'.$establishment_address.'</td>
					<td><input type="checkbox"   checked="checked" disabled="disabled" title="Already Verified" /><span style="color:#006600;">&nbsp;Verified</span> </td>
  				</tr>
				<tr>
					<td>D.Type of Business, trade, industry, manufacture or occupation which is carried on in the establishment</td>
					<td>'.$nature_work.'</td>
					<td><input type="checkbox"   checked="checked" disabled="disabled" title="Already Verified" /><span style="color:#006600;">&nbsp;Verified</span> </td>
				</tr>
				 <tr>
					<td>E.Establishment Registration Details Under Particular Act</td>
					<td>'.$establish_details_reg.'</td>
					<td><input type="checkbox"   checked="checked" disabled="disabled" title="Already Verified" /><span style="color:#006600;">&nbsp;Verified</span> </td>
			    </tr> 
				  <tr>
					<td>F.Name & Address of the Principal Employer </td>
					<td>'.$principal_details.'</td>
					<td><input type="checkbox"   checked="checked" disabled="disabled" title="Already Verified" /><span style="color:#006600;">&nbsp;Verified</span> </td>
			  </tr>
			  <tr>
    			<td  colspan="3" align="center"><a target="_blank" href= '.$GLOBALS['base_url'].'/alc-visible-applications/'.encryption_decryption_fun('encrypt', trim($application_id)).'/'.encryption_decryption_fun('encrypt', trim($user_id)).'>CLICK HERE TO VIEW MORE ESTABLISHMENT DETAILS</a> <a title = "Click here to view & print Registration Certificate of Principal Employer" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$certificates_url.'" target="_blank"><img src="'.$GLOBALS['base_url'].'/'. drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>&nbsp;&nbsp;
	
	
				<span class="popup_renewal" id='.$certificates_fid.'><img src="'.$GLOBALS['base_url'].'/'. drupal_get_path('theme', 'jackson').'/images/zoom.png"></span>
				<div id="view_'.$certificates_fid.'" title="Registration Certificate of Principal Employer" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$certificates_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>
	
	
	</td> </tr>
			   <tr>
				<td>G.Nature of work in which Contract Labour is employed or is to be employed in the establishment</td>
				<td>'.$labour_nature_work.'</td>
				<td><input type="checkbox"   checked="checked" disabled="disabled" title="Already Verified" /><span style="color:#006600;">&nbsp;Verified</span> </td>
			  </tr>
			   <tr>
				<td>H.Duration of the proposed contract work <br />(give particulars of proposed date of ending) </td>
				<td>'.$startend_date.'<br /> Estimated Duration:  '.$diff.'</td>
				<td><input type="checkbox"   checked="checked" disabled="disabled" title="Already Verified" /><span style="color:#006600;">&nbsp;Verified</span> </td>
			  </tr>
			   <tr>
				<td colspan="3" class="sub-title"><strong>2.Personal Information(Incase of Individuals)</strong></td>
			   </tr>
			 <tr>
					<td>A.'.$title.'</td>
					<td>'.$age_contractor.'</td>
					<td><input type="checkbox" id="indivisual_company" class="renewal_alc_verified" '.$indivisual_company_check.' /></td>
				</tr>
			  
				<tr>
					<td>B.Father&#13;s name of contractor (incase of individuals) </td>
					<td>'.$father_contarctor_name.'</td>
					<td><input type="checkbox" id="father_name"  class="renewal_alc_verified" '.$father_name_check.' /></td>
				</tr>
				<tr>
					<td><strong>C.Co-oparative Society</strong><font color="red">**</font></td>
					<td><strong>'.$is_coparative.'</strong></td>
					<td><input type="checkbox" id="is_co_oparative"  class="renewal_alc_verified" '.$is_co_oparative_check.' /></td>
				</tr>
				
				 <tr>
				<td colspan="3" class="sub-title"><strong>3.Contractor Address Details </strong></td>
			 </tr>
			 
			 <tr>
					<td><strong>A.Address of contractor</strong>><font color="red">**</font> </td>
					<td>'.$address_line1_of_contractor.' <br />'. $contractor_address.'</td>
					<td><input type="checkbox" id="address_of_contractor" class="renewal_alc_verified" '.$address_of_contractor_check.' /></td>
				</tr>
				
				   <tr>
				<td colspan="3" class="sub-title"><strong>4.Worksite Address Details </strong></td>
			 </tr>
			 <tr>
					<td><strong>A.Worksite Address</strong><font color="red">**</font></td>
					<td>'.$worksite_address.' </td>
					<td><input type="checkbox" id="worksite_address"  class="renewal_alc_verified" '.$worksite_address_check.' /></td>
				</tr>				
			  
  			  <tr><td colspan="3" class="sub-title"><strong>5.Details of manager/agent of contractor at the worksite </strong></td></tr>			 
			 
			   <tr>
				<td><strong>A.Name of the agent or Manager of Contractor at the work site</strong> <font color="red">**</font></td>
				<td>'.$name_of_agent.'</td>
				<td><input type="checkbox" id="name_manager"  class="renewal_alc_verified"  '.$name_manager_check.' /></td>
			  </tr>
			  <tr>
				<td><strong>B.Address of the agent or Manager of Contractor at the work site </strong><font color="red">**</font></td>
				<td> '.$address_of_manager.'<br /> '.$contractor_manager_address.'</td>
				<td><input type="checkbox" id="address_manager"  class="renewal_alc_verified" '.$address_manager_check.'  /></td>
			  </tr>
			   <tr>
				<td colspan="3" class="sub-title"><strong>6.Contract Labour Details</strong></td>
			 </tr>
			  <tr>
				<td><strong>A.Maximum number of Contract Labour proposed to he employed in the establishment on any date.</strong><font color="red">**</font></td>
				<td><strong>'.$contractor_max_no_of_labours_on_any_day.' ('.convertNumberToWord($contractor_max_no_of_labours_on_any_day).')</strong></td>
				<td><input type="checkbox"   checked="checked" disabled="disabled" title="Already Verified" /><span style="color:#006600;">&nbsp;Verified</span> </td>
			  </tr>
			  	<tr>
				<td><strong>B.Amount of Renewal of Licence Fee</strong> <font color="red">**</font></td>
				<td><strong>₹' .$increnent_fees.' ( Rupees '.convertNumberToWord($increnent_fees). ' only)</strong>'.$extrafeesmsg.'</td>
				<td><input type="checkbox"   checked="checked" disabled="disabled" title="Already Verified" /><span style="color:#006600;">&nbsp;Verified</span> </td>
			</tr>
				<tr>
				<td><strong>C.Applicable security fees for Renewal</strong> <font color="red">**</font></td>';
				if($is_coparative == 'Yes'){$alert = '[<font color="#FF0000">Reduced 25% of security fees per head of contract labour,applicable for Co-oparative Society</font>]';}
				$output .= '<td><strong>₹' .$renewal_security_new.' ( Rupees '.convertNumberToWord($renewal_security_new).' only)</strong>'.$alert.'</td>
				<td><input type="checkbox"   checked="checked" disabled="disabled" title="Already Verified" /><span style="color:#006600;">&nbsp;Verified</span> </td>
			</tr>
			 
			</tr>
				
			   <tr>
				<td><strong>D.Category/designation/nomenclature of the contractor labour, namely,fitter,welder,carpenter,mazdor etc.</strong><font color="red">**</font></td>
				<td>'.$category_designation.'</td>
				<td><input type="checkbox" id="category"  class="renewal_alc_verified"  '.$category_check.'/></td>
			  </tr>
			  <tr>
				<td colspan="3">
						<b>E.Rate of Wages,DA and other cash benefits paid/ to be paid to each category(i.e (a):Unskilled (b)Semi-Skilled (c)Skilled (d)Highly-Skilled etc.)of contract labour</b>:</td>
			  </tr>
   			  <tr>
    			<td>E(i)Rate of Wages,DA and other cash benefits paid/ to be paid to Unskilled of contract labour:</td>';
					if(empty($unskilled_rate_wages)){
						$unskilled_rate_wages_show='Nil';
					}else{
						//$unskilled_rate_wages_show='<del>&#2352;</del>'.$unskilled_rate_wages.' ( Rupees '.convertNumberToWord($unskilled_rate_wages). 'only)';
						$unskilled_rate_wages_show= $unskilled_rate_wages ;
					}
					$output	.='<td>'.$unskilled_rate_wages_show.'</td>
							<td><input type="checkbox" id="unskilled_wages"    class="renewal_alc_verified" '.$unskilled_wages_check.' /></td>
				</tr>';
	
					if(empty($semiskilled_rate_wages)){
						$semiskilled_rate_wages_show='Nil';
					}else{
						$semiskilled_rate_wages_show= $semiskilled_rate_wages;
					}
   
  		$output.='<tr>
    				<td>E(ii)Rate of Wages,DA and other cash benefits paid/ to be paid to Semi-skilled of contract labour:</td>
					<td>'.$semiskilled_rate_wages_show.'</td>
					<td><input type="checkbox" id="semiskilled_wages"  class="renewal_alc_verified"  '.$semiskilled_wages_check.' /></td>
				  </tr>';
	
	
				   if(empty($skilled_rate_wages)){
						$skilled_rate_wages_show='Nil';
					}else{
						//$skilled_rate_wages_show='<del>&#2352;</del>'.$skilled_rate_wages.' (Rupees '.convertNumberToWord($skilled_rate_wages). 'only)';
						$skilled_rate_wages_show= $skilled_rate_wages;
					}
   
 		 $output.='<tr>
					<td>E(iii)Rate of Wages,DA and other cash benefits paid/ to be paid to Skilled of contract labour:</td>
					<td>'.$skilled_rate_wages_show.'</td>
					<td><input type="checkbox" id="skilled_wages" class="renewal_alc_verified"  '.$skilled_wages_check.' /></td>
				  </tr>';
	
					if(empty($highlyskilled_rate_wages)){
						$highlyskilled_rate_wages_show='Nil';
					}else{
						//$highlyskilled_rate_wages_show='<del>&#2352;</del>'.$highlyskilled_rate_wages.' ( Rupees '.convertNumberToWord($highlyskilled_rate_wages). ' only)';
						$highlyskilled_rate_wages_show= $highlyskilled_rate_wages;
					}
    
   
	  $output	.='<tr>
					<td>E(iv)Rate of Wages,DA and other cash benefits paid/ to be paid to Highly-skilled of contract labour:</td>
					<td>'.$highlyskilled_rate_wages_show.'</td>
					<td><input type="checkbox" id="highlysiskilled_wages" class="renewal_alc_verified" '.$highlysiskilled_wages_check.' /></td>
    			  </tr>
				  <tr>
					<td>E(v).Hours of Work,Spread over time,Overtime and Overtime Wages</td>
					<td>(v)Hours of Work:-'.$hours_work.' <br>(vi) Spread over time:- '.$spred_over.' <br> (vii)Overtime:- '.$overtime.'  <br>(viii)Overtime Wages:-  '.$overtime_wages.'</td>
					<td><input type="checkbox" id="hour_details" class="renewal_alc_verified"  '.$hour_details_check. ' /></td>
    			  </tr>
				  <tr>
					<td>F.Whether weekly holiday(s) observed and on which day</td>
					<td>'.$total_holiday.' day(s) ('.$name_holidays.')</td>
					<td><input type="checkbox" id="weekly_holiday" class="renewal_alc_verified" '.$weekly_holiday_check. '  /></td>
				</tr>
				<tr>
					<td>G.Whether weekly holiday(s) so observed was paid holiday(s)</td>
					<td>'.$holiday_wages.'</td>
					<td><input type="checkbox" id="holiday_wages" class="renewal_alc_verified"  '.$holiday_wages_check.' /></td>
				</tr>
				
				<td colspan="3">
						<b>H.Other Condition of service like leave (annual leave,casual leave,sick leave,maternity leave etc.) Holidays etc.of the contract labour</b>:</td>
			  </tr>
  				
				  <tr>
					
			<td>Number of Annual leave</td>
					  <td>'.$annual_leave_no_show.'</td>
					  <td><input type="checkbox" id="annual_leave" class="renewal_alc_verified" '.$annual_leave_check. '  /></td>
				  </tr>
				  <tr>
					<td>Number of Casual leave</td>
					<td>'.$casual_leave_no_show.'</td>
					 <td><input type="checkbox" id="casual_leave" class="renewal_alc_verified" '.$casual_leave_check. '  /></td>
				  </tr>
				  <tr>
					<td>Number of Earned leave</td>
					<td>'.$earned_leave_no_show.'</td>
					<td><input type="checkbox" id="eraned_leave" class="renewal_alc_verified" '.$eraned_leave_check.' /></td>
				</tr>
				  <tr>
					  <td>Number of Sick leave</td>
					  <td>'.$sick_leave_no_show.'</td>
					  <td><input type="checkbox" id="sick_leave" class="renewal_alc_verified" '.$sick_leave_check. ' /></td>
				  </tr>
				<tr>
					<td>Number of Maternity leave</td>
					<td>'.$maternity_leave_no_show.'</td>
					<td><input type="checkbox" id="maternity_leave" class="renewal_alc_verified"  '.$maternity_leave_check.' /></td>
				</tr>
				<tr>
					<td>Number of Other leave</td>
					<td>'.$other_leave_no_show.'</td>
					<td><input type="checkbox" id="other_leave" class="renewal_alc_verified"  '.$other_leave_check.' /></td>
				</tr>
				
				 <tr>
				<td colspan="3" class="sub-title"><strong>7.Other Information</strong></td>
			 </tr>
			 <tr>
				<td><strong>A.Number and date of the license</strong><font color="red">**</font></td>
				<td><strong>A(i).NO.: '.$backlog_license_no.'<br>A(ii)Date:'.date('dS M, Y',strtotime($backlog_license_date)).'</strong> </td>
				<td><input type="checkbox" id="backlog_licnse_date"  class="renewal_alc_verified" '.$backlog_licnse_date_check.' /></td>
			</tr>
				 	
			<tr>
				<td><strong>B.Amount of Licence Fee paid</strong><font color="red">**</font></td>
				<td><strong> ₹ '.$amount_of_licence.' ( Rupees '.convertNumberToWord($amount_of_licence). ' only)</strong></td>
				<td><input type="checkbox" id="license_amount"  class="renewal_alc_verified" '.$license_amount_check.' /></td>
				<tr>
				<tr>
				<td><strong>C.Amount of security deposit</strong><font color="red">**</font></td>
				<td><strong>₹ ' .$amount_of_security_deposit.' ( Rupees '.convertNumberToWord($amount_of_security_deposit). ' only)</strong></td>
				<td><input type="checkbox" id="security_amount" class="renewal_alc_verified" '.$seurity_amount_check.' /></td>
			</tr>	
			<tr>
				<td><strong>D.Amount of last Renewal of Licence Fee paid</strong><font color="red">**</font></td>
				<td><strong>₹ ' .$backlog_renewal_fees.' ( Rupees '.convertNumberToWord($backlog_renewal_fees). ' only)</td>
				<td><input type="checkbox" id="backlog_renewal_amount" class="renewal_alc_verified"  '.$backlog_renewal_amount_check.'/></td>
			</tr>
			<tr>
				<td><font color ="red"><strong>E.Date of expiry of the previous license<font color="red">**</font></strong></font></td>
				<td><font color ="red"><b>' .$renewal_expaire.'</b></font></td>
				<td><input type="checkbox" id="backlog_renewal_date" class="renewal_alc_verified"  '.$backlog_renewal_date_check.' /></td>
			</tr>
			<tr>
				<td><strong>F.Special benefits provided, if any</strong><font color="red">**</font></td>
				<td>'.$special_benifites_show.'</td>
				<td><input type="checkbox" id="special_benifites" class="renewal_alc_verified" '.$special_benifites_check.' /></td>
			  </tr>
			  <tr>
				<td><strong>G.Contribution made under the Employees State Insurance Act,1948</strong><font color="red">**</font></td>
				<td>'.$state_insurance_show.'</td>
				<td><input type="checkbox" id="state_insurance" class="renewal_alc_verified"  '.$state_insurance_check.' /></td>
			  </tr>
			   <tr>
				<td><strong>H.Contribution made under the Employees Provident Fund and Miscellaneous Provision Act,1952</strong> <font color="red">**</font></td>
				<td>'.$miscellaneous_provisions_show.'</td>
				<td><input type="checkbox" id="miscellaneous" class="renewal_alc_verified"  '.$miscellaneous_check.' /></td>
			  </tr>
			<tr>
					<td><strong>I.Whether the contractor was convicted of any offence within the preceding five years. If so, give details</strong><font color="red">**</font></td>
   					<td>'.$details_contractor_convicted.'</td>
					<td><input type="checkbox" id="offence" class="renewal_alc_verified"  '.$offence_check.' /></td>
  				</tr>
				<tr>';
			   
				  if(!empty($details_contractor_revoking)){
					  $revoking_date	= date('dS M, Y', strtotime($details_contractor_revoking));
				  }else{
					  $revoking_date='Not Applicable';
				  }
	   $output.=' <td><strong>J.Whether there was any order against the contract or revoking or suspending license or forfeiting security deposit in respect of an earlier contract. If so, the date of such order </strong><font color="red">**</font></td>
				  <td>'.$revoking_date.'</td>
				  <td><input type="checkbox" id="revoking" class="renewal_alc_verified"  '.$revoking_check. ' /></td>
			  </tr>
			     
			  <tr>
				<td><strong>K.Whether the contractor has worked in any other establishment within the past five years. If so. give details of the principal employer, establishment and nature of work</strong> <font color="red">**</font></td>
				<td>'.$pervious_license_detais.'</td>
				<td><input type="checkbox" id="pervious_license" class="renewal_alc_verified"  '.$pervious_license_check.'/></td>
			  </tr>			  
			   
			  <tr><td colspan="3" class="sub-title"><strong>8.Uploaded Documents</strong></td></tr>			 
			
			  <tr>
			  	<td><strong>A.Work Order </strong><font color="red">**</font></td>';
			  		if(!empty($work_order_file_id)){ 
        		$output.='<td><a title = "Click here to view work order" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$work_order_url.'" target="_blank"><img src="'.$GLOBALS['base_url'].'/'. drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>
					
					&nbsp;&nbsp;
				<span class="popup_renewal" id='.$work_order_file_id.'><img src="'.$GLOBALS['base_url'].'/'. drupal_get_path('theme', 'jackson').'/images/zoom.png"></span>
				<div id="view_'.$work_order_file_id.'" title="Registration Certificate of Principal Employer" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$work_order_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>
					
				
				</td>
				
				<td><input type="checkbox" id="work_order" class="renewal_alc_verified"  '.$work_order_check.'  /></td>';
				
			
				
			  		}else{
			   	$output.='<td>No Document uploaded</td><td><input type="checkbox" id="work_order" class="renewal_alc_verified"  '.$work_order_check.'/></td>';
			  		}
	$output.='</tr>
			  <tr>
			  	<td><strong>B.Form V </strong><font color="red">**</font></td>';
			  		if(!empty($frm_v_file_id)){ 
        		$output.='<td><a title = "Click here to view Form V" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$frmv_url.'" target="_blank"><img src="'.$GLOBALS['base_url'].'/'. drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>
				
				&nbsp;&nbsp;
				<span class="popup_renewal" id='.$frm_v_file_id.'><img src="'.$GLOBALS['base_url'].'/'. drupal_get_path('theme', 'jackson').'/images/zoom.png"></span>
				<div id="view_'.$frm_v_file_id.'" title="Registration Certificate of Principal Employer" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$frmv_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>
				
				
				&nbsp;&nbsp; '.l(t('Generated FORM - V'),'alc-view-formV/'.$application_id.'/'.$serial_no_from_v.'/Form-VII'.time(), array('html' => TRUE,'attributes'=> array('target'=>'_blank'))).'
				</td>
				<td><input type="checkbox" id="frm_v" class="renewal_alc_verified"  '.$frm_v_check.' /></td>';
			  		}else{
			   	$output.='<td>No Document uploaded</td><td><input type="checkbox" id="frm_v" class="renewal_alc_verified"  '.$frm_v_check.' /></td>';
			  		}
	$output.='</tr>
			  <tr>
			  	<td><strong>C.Residential Certificate</strong> <font color="red">**</font></td>';
			  		if(!empty($residential_file_id)){ 
        		$output.='<td><a title = "Click here to view residential certificate" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$residential_url.'" target="_blank"><img src="'.$GLOBALS['base_url'].'/'. drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>
				
				&nbsp;&nbsp;
				<span class="popup_renewal" id='.$residential_file_id.'><img src="'.$GLOBALS['base_url'].'/'. drupal_get_path('theme', 'jackson').'/images/zoom.png"></span>
				<div id="view_'.$residential_file_id.'" title="Registration Certificate of Principal Employer" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$residential_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>
				</td> <td><input type="checkbox" id="residential" class="renewal_alc_verified"  '.$residential_check.' /> 
				
				
				</td>';
			  		}else{
			   	$output.='<td>No Document uploaded</td> <td input type="residential" id="frm_v" class="renewal_alc_verified" '.$residential_check.' /></td>';
			  		}
					
			  		
	$output.='</tr>
			  <tr>
			  	<td><strong>D.Pervious certificates of license or renewal</strong> <font color="red">**</font></td>';
			  		if(!empty($upload_backlog_certificate_id)){ 
        		$output.='<td><a title = "Click here to view periviuos certifiactes for renewal" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$backlog_certificates_url.'" target="_blank"><img src="'.$GLOBALS['base_url'].'/'. drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>
				&nbsp;&nbsp;
				<span class="popup_renewal" id='.$upload_backlog_certificate_id.'><img src="'.$GLOBALS['base_url'].'/'. drupal_get_path('theme', 'jackson').'/images/zoom.png"></span>
				<div id="view_'.$upload_backlog_certificate_id.'" title="Registration Certificate of Principal Employer" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$backlog_certificates_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>
				</td> 
				
				<td><input type="checkbox" id="perivious" class="renewal_alc_verified"   '.$perivious_check.' /></td>';
			  		}else{
			   	$output.='<td>No Document uploaded</td> <td><input type="checkbox" id="perivious" class="renewal_alc_verified"  '.$perivious_check.' /></td>';
			  		}
					
	$output.='</tr><tr>
	
			  	<td><strong>E.Form VII</strong> <font color="red">**</font></td>';
			  		if(!empty($upload_signed_form_id)){ 
        				$output.='<td><a title = "Click here to view form VI" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$upload_signed_form_vii_url.'" target="_blank"><img src="'.$GLOBALS['base_url'].'/'. drupal_get_path('theme', 'jackson').'/images/pdf.png"></a>
						
						&nbsp;&nbsp;
				<span class="popup_renewal" id='.$upload_signed_form_id.'><img src="'.$GLOBALS['base_url'].'/'. drupal_get_path('theme', 'jackson').'/images/zoom.png"></span>
				<div id="view_'.$upload_signed_form_id.'" title="Registration Certificate of Principal Employer" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$upload_signed_form_vii_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>
						
						&nbsp;&nbsp; '.l(t('Generated FORM - VII'),'pdf-form-vii/'.encryption_decryption_fun('encrypt', $license_renewal_id).'/'.encryption_decryption_fun('encrypt', $contractor_user_id).'/Form-VII'.time(), array('html' => TRUE,'attributes'=> array('target'=>'_blank'))).'</td>
						 <td><input type="checkbox" id="form_vii" class="renewal_alc_verified"  '.$form_vii_check.' /></td>';
			  		}else{
			   			$output.='<td>Form -VII to be uploaded with duly signed by the Applicant after fees submission &nbsp;&nbsp; '.l(t('Generated FORM - VII'),'pdf-form-vii/'.encryption_decryption_fun('encrypt', $license_renewal_id).'/'.encryption_decryption_fun('encrypt', $contractor_user_id).'/Form-VII'.time(), array('html' => TRUE,'attributes'=> array('target'=>'_blank'))).'</td>
						 <td><input type="checkbox" id="form_vii" class="renewal_alc_verified"  '.$form_vii_check.'/></td>';
			  		}
	
				
	
	$output.=' </tr> 
	 <tr>
				<td colspan="3" class="sub-title"><strong>9.Important Information</strong></td>
			 </tr>
			 
			  <tr>
				<td>A.Whether a certificate by the principal employer in Form V is Enclosed</td>
				<td>YES</td>
				<td><input type="checkbox" checked="checked" disabled="disabled" title="Already Verified" /><span style="color:#006600;">&nbsp;Verified</span> </td>
			  </tr>		
		
			<tr>
				<td><strong>B.Payble amount of security fees</strong><font color="red">**</font></td>
				<td><strong>₹' .$new_security_amount.' ( Rupees '.convertNumberToWord($new_security_amount). ' only)</strong></td>
				<td><input type="checkbox"   checked="checked" disabled="disabled" title="Already Verified" /><span style="color:#006600;">&nbsp;Verified</span> </td>
			</tr>
			<tr>
				<td colspan="2"><font color="#FF0000">D.Provided that if the application for renewal is not received with in the time specified in R29(2)[<i>Amendment on 27th Nov. 2015</i>], a fee of 25%, in excess of the fee ordinarily payable for the licence shall be payable for such renewal: Provided further that in case where the licensing officer is satisfied that the delay in submission of the application is due to unavoidable circumstances beyond the control of the contractor, he may reduce or remit as he thinks fit the payment of such excess fee.[see R29(3)]</font></td>
			
				<td><input type="checkbox" id="backlog_renewal_extra_amount" onclick="return Confirmcheck()" class="renewal_alc_verified"  '.$backlog_renewal_extra_check.'/><br><i style="color:#f00;">Must be select for 25% additional fees</i></td>
			</tr>
			
			<tr><td>PAYMENT DETAILS</td><td colspan="2"> '.$payment_details.'</td></tr></table><br class="clear" />
			
	
		</table>';
		
		
	$output .= '<div class="row"><ul style="padding:0;"><li class="col-md-2 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-arrow-left"></i>&nbsp;Back to list</div>', 'alc/alc-received-renewal-details', array('attributes' => array('title' => 'Click here back to applications list'), 'html' => TRUE)).'</li>';
	
	$output .= '<li class="col-md-2 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-user"></i>&nbsp;View Applicant Profile</div>', 'view-applicant-profile/'.encryption_decryption_fun('encrypt', $serial_no_from_v).'/'.encryption_decryption_fun('encrypt', $contractor_user_id).'/'.encryption_decryption_fun('encrypt', '12'), array('attributes' => array('title' => 'Click here view Applicant Profile'), 'html' => TRUE)).'</li>';
	
	$output .= '<li class="col-md-4 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-info-circle"></i>&nbsp;Instructions to give remarks</div>', $base_root.$base_path.'sites/default/files/instructions-remarks.pdf', array('attributes' => array('title' => 'Instructions to give remarks'), 'html' => TRUE)).'</li>';
	
	$output .= '<li class="col-md-4 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-question-circle" aria-hidden="true"></i>&nbsp;How to digitally sign your documents using USB Token</div>', 'digitally-sign-process', array('attributes' => array('title' => 'How to digitally sign your documents using USB Token'), 'html' => TRUE)).'</li></ul></div>';		
	
	return $output;
	
}

function action_field($form, $form_state){
	$commands 	= array();	
	$commands[] = ajax_command_replace('#renewal_call_applicants', drupal_render($form['backlog_renewal']['call_applicant']));
	$commands[] = ajax_command_replace('#renewal_remarks_text_div', drupal_render($form['backlog_renewal']['remarks_text']));
	$commands[] = ajax_command_replace('#gen_pdf_div', drupal_render($form['backlog_renewal']['gen_pdf']));
	$commands[] = ajax_command_replace('#renewal_hidden_action_div', drupal_render($form['backlog_renewal']['hidden_action']));
	$commands[] = ajax_command_replace('#edit-certificates-ajax-wrapper', drupal_render($form['backlog_renewal']['certificates']));
	$commands[] = ajax_command_replace('#download_certificates', drupal_render($form['backlog_renewal']['download_certificates']));
	$commands[] = ajax_command_replace('#alc_recomented_date', drupal_render($form['backlog_renewal']['recomented_date']));
	$commands[] = ajax_command_replace('#check_declaration_id', drupal_render($form['backlog_renewal']['check_declaration']));

		
	return array('#type' => 'ajax', '#commands' => $commands);	
}

function renewal_remark_details($serialFormV,$license_renewal_id,$license_id,$contractor_particular_id,$created_by){
	
	$user_id 		= $user->uid;
	$counter		= 0;
	$status			= '';
	
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
					array('data' => 'Remark', 'field' => 'remrk'),
					array('data' => 'Date - Time', 'field' => 'date','width' => '15%'),
					array('data' => 'Remark Status', 'field' => 'remark type','width' => '15%'),
					array('data' => 'Remark By', 'field' => 'licenseno','width' => '20%'),
					array('data' => 'Action','width' => '10%')
				);

	$remark_details			=	db_select('l_remark_license','Rm');
	
	$remark_details			->	InnerJoin('role', 'r','r.rid = Rm.remark_by_role');
	$remark_details 		->	fields('Rm',array('remark_text','remark_type','remark_date','remark_by','remark_by_name','id','remark_by_role'));
	
	$remark_details 		->	fields('r',array('name'));
	$remark_details			->	condition('Rm.remark_to',$serialFormV,'=');
	$remark_details			->	condition('Rm.particular_id',$license_renewal_id,'=');
	$remark_details			->	condition('Rm.flag','R','=');
	$remark_details			->	condition('Rm.is_active',1,'=');
	$remark_or				=	db_or();
	$remark_or				->	condition('Rm.remark_by_role',4);
	$remark_or				->	condition('Rm.remark_by_role',7);
	$remark_details			->	condition($remark_or);
	$remark_details			->	orderBy('Rm.id','DESC');
	$remark_details_result	=	$remark_details -> execute()->fetchAll();


	foreach($remark_details_result as $row){
		
		$counter++;
		$remark_by		= $row->remark_by_name.'&nbsp;('.$row->name.')';
		
		if($row->remark_type == "P" && $row->remark_by_role == 4){
			$status 	= '<span class="backed" title=" Rectify FORM-VII"></span>';
		}else{
			$status 	= get_user_license_status($row->remark_type);
			
		}
		
				
		$query 	= 	db_select('l_remark_license', 'lrl')
				->	fields('lrl', array('remark_by','id'))
				->	condition('lrl.remark_to', $serialFormV)
				->	condition('lrl.particular_id', $license_renewal_id)
				->	condition('lrl.flag', 'R')
				->	condition('lrl.is_active', 1)
				->	orderBy('id', 'DESC')
			    ->  extend('PagerDefault')->limit(1);
		$remark_result = $query->execute()->fetchAssoc();
						 
	 	$remark_by_id = $remark_result['remark_by']; 
		$remark_id = $remark_result['id']; 
		$application_details	= contractor_activity_tag_del($serialFormV,$created_by);
		
		$flag_application		= $application_details['flagapplication'];
		//  $user_id;
		if($flag_application == 'R'){
		if($user->uid == $remark_by_id && $row->id == $remark_id ){
			$url 				= 'renewal-remarks-delete/'.encryption_decryption_fun('encrypt',$row->id).'/'.encryption_decryption_fun('encrypt',$license_renewal_id).'/'.encryption_decryption_fun('encrypt',$license_id).'/'.encryption_decryption_fun('encrypt',$serialFormV).'/'.encryption_decryption_fun('encrypt',$contractor_particular_id);			
			$delete				= l('<div id="delBtnrenewalremark" class="delete-btn active-del" >'.t('Delete').'</div>',$url,array('html' => TRUE));	
		}else{
			$delete = '<div id="delBtn" class="delete-btn disable-del">'.t('Delete').'</div>';
			
		}
		}else{
			$delete = '<div id="delBtn" class="delete-btn disable-del">'.t('Delete').'</div>';
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
					'empty'		 => t('No Remark found')
				);
	$output = theme('datatable', $variables);
	
	return $output;			
}

function alc_view_backlog_contractor_license_renewal_submit($form,&$form_state){
	$license_issued=0;	
	$message_identifier = '0';
	$is_backlog_renewal_date_exists = 0;
	global $user;
	 $uid = $user->uid;	
	

	$val = $form_state['values'];
	
	$status  								= $val['remark_type'];
	$status1  								= $val['remark_type'];
	$remark_text 							= $val['remarks_text'];
	$calltime 								= $val['call_applicant'];
	$renewal_fees 							= $val['renewal_fees'];
	$contractor_particularid				= $val['contractor_particularid'];
	$licenseid								=  $val['licenseid'];
	$is_recommanden_date					=  $val['is_recommanden_date'];
	$licenseid_my							= encryption_decryption_fun('decrypt',$val['licenseid']);
	
	$renewal_expaire						= $val['backlog_renewal_date'];
	$recomented_date					    = $val['recomented_date']['year']."-".$val['recomented_date']['month']."-".$val['recomented_date']['day'];
	//print_r($recomented_date);
	//die();
	$remark_by  							= $uid;
	$remark_to  							= encryption_decryption_fun('decrypt',$val['formV']);
	$remark_field_title						= $val['remark_alc_text_renewal'];
	$editablefield_array 					= explode(',', $remark_field_title);
	if(!empty($editablefield_array)){	
	foreach($editablefield_array as $row){
			if($row == "backlog_renewal_date"){
				$is_backlog_renewal_date_exists = 0;
				break;
			}if($row != "backlog_renewal_date"){
				$is_backlog_renewal_date_exists = 1;
				break;
			}
		}
	}
	if(!empty($editablefield_array)){
	foreach($editablefield_array as $row){
	
			
					
		if($row == "backlog_renewal_extra_amount"){
				$is_extra_fees		=	 1 ;
				$increnent_fees		=	(125/100)*$renewal_fees;
				break;
		}else{
			$is_extra_fees		=	0 ;
			$increnent_fees		=	$renewal_fees;
		}
	}
	}
	$particularid				= encryption_decryption_fun('decrypt',$val['renewal_id']);
	$details_renewal 			= contractor_license_apply_renweal($particularid);
	$created_by					= $details_renewal['created_by'];
	//die();
	
	$remark_by_role 			= 	db_select('l_custom_user_detail', 'user_details');
	$remark_by_role				->	fields('user_details', array('fullname'))->condition('user_details.usr_id',$uid,'=');
	$remark_by_name				=	$remark_by_role->execute()->fetchObject()->fullname;
	
	if($status == 'C' ){	
		$remark_text =$remark_text .' on '.$calltime;
		
	}if($status	==	'BI'){
		$update_status_1   =  		array('renewal_appliaction_status'=>$status,'remark_by_userid'=> $uid,'is_extra_fees' =>$is_extra_fees,'increnent_fees' =>$increnent_fees,'modification_date'=>time());
				 
				$apply_status  		=  	db_update('l_contractor_license_apply_renweal');
				$apply_status		->	fields($update_status_1);
										
				$apply_status		->	condition('id',$particularid);
				$apply_status		->  condition('action','R','=');
				$apply_status		->	execute();
								
		$renewal_remark_license = array(
					'remark_text'   		=> $remark_text, 
					'remark_by'   			=> $remark_by, 
					'remark_to' 			=> $remark_to,
					'remark_type' 			=> 'BI',
					'remark_date'  			=> time(),
					'remark_field_title' 	=> $remark_field_title,
					'particular_id'			=>	$particularid,
					'remark_by_role' 		=> '4',
					'flag'					=> 'R',
					'remark_by_name'		=> $remark_by_name
				);						
				
		db_insert('l_remark_license')->fields($renewal_remark_license)->execute();
								
		contractor_activity_tag_insert_backlog_reneeal($remark_to,'R',$particularid,'BI',$created_by);
		
	}if($status	==	'P'){
		$update_status_1   =  	array('renewal_appliaction_status'=>'P','remark_by_userid'=> $uid,'is_extra_fees' =>$is_extra_fees,'increnent_fees' =>$increnent_fees,'modification_date'=>time());
				 
				$apply_status  		=  	db_update('l_contractor_license_apply_renweal');
				$apply_status		->	fields($update_status_1);
										
				$apply_status		->	condition('id',$particularid);
				$apply_status		->  condition('action','R','=');
				$apply_status		->	execute();
								
		$renewal_remark_license = array(
					'remark_text'   		=> $remark_text, 
					'remark_by'   			=> $remark_by, 
					'remark_to' 			=> $remark_to,
					'remark_type' 			=> 'B',
					'remark_date'  			=> time(),
					'remark_field_title' 	=> $remark_field_title,
					'particular_id'			=>	$particularid,
					'remark_by_role' 		=> '4',
					'flag'					=> 'R',
					'remark_by_name'		=> $remark_by_name
				);						
				
		db_insert('l_remark_license')->fields($renewal_remark_license)->execute();
								
	contractor_activity_tag_insert_backlog_reneeal($remark_to,'R',$particularid,'P',$created_by);	
		
	}	
	
	 
	 if($status == 'R'){
		   
 	
			$update_status_1   	=  	array('renewal_appliaction_status'=>$status,'remark_by_userid'=> $uid,'is_extra_fees' =>$is_extra_fees,'increnent_fees' =>$increnent_fees,'modification_date'=>time());
			$apply_status  		=  	db_update('l_contractor_license_apply_renweal');
			$apply_status		->	fields($update_status_1);
							
			$apply_status		->	condition('id',$particularid);
			$apply_status		->  condition('action','R','=');
			$apply_status		->	execute();
			
		   $renewal_remark_license = array(
									'remark_text'   		=> $remark_text, 
									'remark_by'   			=> $remark_by, 
									'remark_to' 			=> $remark_to,
									'remark_type' 			=> $status,
									'remark_date'  			=> time(),
									'remark_field_title' 	=> $remark_field_title,
									'particular_id'			=>	$particularid,
									'remark_by_role' 		=> '4',
									'flag'					=> 'R',
									'remark_by_name'		=> $remark_by_name
								);
								
		
		    db_insert('l_remark_license')->fields($renewal_remark_license)->execute();
			contractor_activity_tag_insert_backlog_reneeal($remark_to,'R',$particularid,'R',$created_by);
			/*$record=db_query("SELECT * from rejected_renewal ('".$particularid."','".$licenseid_my."','1')");	
		 	if( $record->rowCount() > 0 ){
		
						$obj 				= 	$record->fetchAssoc();
						$testparticular_id 	= $obj['testparticular_id'];	
						$testlicense_id 	= $obj['testlicense_id'];
						$testrenewal_id 	= $obj['testrenewal_id'];
				drupal_goto('alc/alc_view_backlog_rejected_license_renewal/'.encryption_decryption_fun('encrypt',$testrenewal_id).'/'.$val['formV'].'/'.encryption_decryption_fun('encrypt',$testparticular_id).'/'.encryption_decryption_fun('encrypt',$testlicense_id)) ;
			}*/
			
	
				
	
			
	 }
	 if($status == 'B'){
		 //die($is_backlog_renewal_date_exists);
		   if(!empty($is_backlog_renewal_date_exists)){
			   	 $update_status_1   =  	array('renewal_appliaction_status'=>$status1,'remark_by_userid'=> $uid,'is_extra_fees' =>$is_extra_fees,'increnent_fees' =>$increnent_fees , 'recommand_date_expire' =>$recomented_date,'modification_date'=>time());
				 
				$apply_status  		=  	db_update('l_contractor_license_apply_renweal');
				$apply_status		->	fields($update_status_1);
										
				$apply_status		->	condition('id',$particularid);
				$apply_status		->  condition('action','R','=');
				$apply_status		->	execute();
								
		$renewal_remark_license = array(
					'remark_text'   		=> $remark_text, 
					'remark_by'   			=> $remark_by, 
					'remark_to' 			=> $remark_to,
					'remark_type' 			=> $status,
					'remark_date'  			=> time(),
					'remark_field_title' 	=> $remark_field_title,
					'particular_id'			=>	$particularid,
					'remark_by_role' 		=> '4',
					'flag'					=> 'R',
					'remark_by_name'		=> $remark_by_name
				);
										
				
		db_insert('l_remark_license')->fields($renewal_remark_license)->execute();
		contractor_activity_tag_insert_backlog_reneeal($remark_to,'R',$particularid,'B',$created_by);
	
		   }
		if((strtotime($renewal_expaire) != strtotime($recomented_date)) && (empty($is_backlog_renewal_date_exists))){
			$message_identifier = 4;
		}if((strtotime($renewal_expaire) == strtotime($recomented_date)) && (empty($is_backlog_renewal_date_exists))){
			 $update_status_1   =  	array('renewal_appliaction_status'=>$status1,'remark_by_userid'=> $uid,'is_extra_fees' =>$is_extra_fees,'increnent_fees' =>$increnent_fees , 'recommand_date_expire' =>$recomented_date);
				 
				$apply_status  		=  	db_update('l_contractor_license_apply_renweal');
				$apply_status		->	fields($update_status_1);
										
				$apply_status		->	condition('id',$particularid);
				$apply_status		->  condition('action','R','=');
				$apply_status		->	execute();
								
		$renewal_remark_license = array(
					'remark_text'   		=> $remark_text, 
					'remark_by'   			=> $remark_by, 
					'remark_to' 			=> $remark_to,
					'remark_type' 			=> $status,
					'remark_date'  			=> time(),
					'remark_field_title' 	=> $remark_field_title,
					'particular_id'			=>	$particularid,
					'remark_by_role' 		=> '4',
					'flag'					=> 'R',
					'remark_by_name'		=> $remark_by_name
				);						
				
		db_insert('l_remark_license')->fields($renewal_remark_license)->execute();
		contractor_activity_tag_insert_backlog_reneeal($remark_to,'R',$particularid,'B',$created_by);
	
	}
		
		
								
	 }
	 if($status == 'A'){
		 
		   // $array2 = array("father_name","address_of_contractor","indivisual_company","backlog_licnse_date","backlog_renewal_date","name_manager","address_manager","unskilled_wages","semiskilled_wages","skilled_wages","highlysiskilled_wages","annual_leave","casual_leave","eraned_leave","sick_leave","maternity_leave","other_leave","weekly_holiday","holiday_wages","offence","revoking","pervious_license","special_benifites","state_insurance","miscellaneous","work_order","frm_v","residential","perivious","form_vii","license_amount","security_amount","backlog_renewal_amount","category","hour_details","worksite_address","is_co_oparative","backlog_renewal_extra_amount");
			
			 $array2 = array("address_of_contractor","backlog_licnse_date","backlog_renewal_date","name_manager","address_manager","offence","revoking","pervious_license","special_benifites","state_insurance","miscellaneous","work_order","frm_v","residential","perivious","license_amount","security_amount","backlog_renewal_amount","category","worksite_address","is_co_oparative","backlog_renewal_extra_amount");
$result1 = array_diff($array2, $editablefield_array);


	if(!empty($result1)){
		if(count($result1)>1){
			$message_identifier = '1';
			
		}if(count($result1)==1){
			
			foreach($result1 as $row){
				if($row == 'backlog_renewal_extra_amount'){
					
					   if(strtotime($renewal_expaire) == strtotime($recomented_date)){
						   
								$update_status_1   	=  	array('renewal_appliaction_status'=>$status1,'remark_by_userid'=> $uid,'is_extra_fees' =>$is_extra_fees,'increnent_fees' =>$increnent_fees,'recommand_date_expire' => $recomented_date,'modification_date'=>time());
								$apply_status  		=  	db_update('l_contractor_license_apply_renweal');
								$apply_status		->	fields($update_status_1);
								$apply_status		->	condition('id',$particularid);
								$apply_status		->  condition('action','R','=');
								$apply_status		->	execute();
								
								$renewal_remark_license = array(
												'remark_text'   		=> $remark_text, 
												'remark_by'   			=> $remark_by, 
												'remark_to' 			=> $remark_to,
												'remark_type' 			=> $status,
												'remark_date'  			=> time(),
												'remark_field_title' 	=> $remark_field_title,
												'particular_id'			=>	$particularid,
												'remark_by_role' 		=> '4',
												'flag'					=> 'R',
												'remark_by_name'		=> $remark_by_name
											);						
				
								db_insert('l_remark_license')->fields($renewal_remark_license)->execute();
								contractor_activity_tag_insert_backlog_reneeal($remark_to,'R',$particularid,'A',$created_by);	
								
							
								//print_r($result1);die();
					   } 
					   if(strtotime($renewal_expaire) != strtotime($recomented_date)){
						   $message_identifier = '2';
					   }
				}else{
					$message_identifier = '1';
				}
			}
			
		}
		
	
	}if(empty($result1)){
		 if(strtotime($renewal_expaire) == strtotime($recomented_date)){
						   
								$update_status_1   	=  	array('renewal_appliaction_status'=>$status1,'remark_by_userid'=> $uid,'is_extra_fees' =>$is_extra_fees,'increnent_fees' =>$increnent_fees,'recommand_date_expire' => $recomented_date,'modification_date'=>time());
								$apply_status  		=  	db_update('l_contractor_license_apply_renweal');
								$apply_status		->	fields($update_status_1);
								$apply_status		->	condition('id',$particularid);
								$apply_status		->  condition('action','R','=');
								$apply_status		->	execute();
								
								$renewal_remark_license = array(
												'remark_text'   		=> $remark_text, 
												'remark_by'   			=> $remark_by, 
												'remark_to' 			=> $remark_to,
												'remark_type' 			=> $status,
												'remark_date'  			=> time(),
												'remark_field_title' 	=> $remark_field_title,
												'particular_id'			=>	$particularid,
												'remark_by_role' 		=> '4',
												'flag'					=> 'R',
												'remark_by_name'		=> $remark_by_name
											);						
				
								db_insert('l_remark_license')->fields($renewal_remark_license)->execute();
								
							contractor_activity_tag_insert_backlog_reneeal($remark_to,'R',$particularid,'A',$created_by);	
							
								//print_r($result1);die();
					   }
			
	} if(strtotime($renewal_expaire) != strtotime($recomented_date)){
						   $message_identifier = '2';
	   }
	
	
	
			
	 }
	
	 
	if($status	==	'I'){
		 $arr_1					= array();
		
		 if(!empty($editablefield_array)){
			 foreach($editablefield_array as $rows){
				
				 if($rows == 'form_vii'){ 
					 
					 $renewal_certificates 	= file_load($val['certificates']);
					 if($renewal_certificates != "" ){
					           	$arr_1 = explode("/", $renewal_certificates->uri);
								$renewal_certificates->status = FILE_STATUS_PERMANENT;
								file_save($renewal_certificates);
		 						$renewal_certificates_fid=$renewal_certificates->fid;
					
					$renewal_remark_license = array(
									'remark_text'   		=> $remark_text, 
									'remark_by'   			=> $remark_by, 
									'remark_to' 			=> $remark_to,
									'remark_type' 			=> $status,
									'remark_date'  			=> time(),
									'remark_field_title' 	=> $remark_field_title,
									'particular_id'			=>	$particularid,
									'remark_by_role' 		=> '4',
									'flag'					=> 'R',
									'remark_by_name'		=> $remark_by_name
								);
					db_insert('l_remark_license')->fields($renewal_remark_license)->execute();
					contractor_activity_tag_insert_backlog_reneeal($remark_to,'R',$particularid,'I',$created_by);	
					
					$update_status   	=  	array('renewal_appliaction_status'=>$status1,'remark_by_userid'=> $uid ,'renewal_certificate_id' => $renewal_certificates_fid,'is_extra_fees' =>$is_extra_fees,'increnent_fees' =>$increnent_fees,'modification_date'=>time());
					 $apply_status  	=  	db_update('l_contractor_license_apply_renweal');
					 $apply_status		->	fields($update_status);
				
	$apply_status		->	condition('id',$particularid);
	$apply_status		->  condition('action','R','=');
	$apply_status		->	execute() ;				
					  $message_identifier = '';
					  break;
					
					 }
					 if($is_recommanden_date == '1'){
					
					$message_identifier = 3;
								$appliyyear					= date('Y', strtotime($recomented_date));
								$backlog_license_year		= date('Y', strtotime($recomented_date));
								$backlog_license_month		= date('m', strtotime($recomented_date));
								$backlog_license_day		= date('d', strtotime($recomented_date));
								$next_renweal_date			= ($appliyyear+1).'-'.$backlog_license_month.'-'.($backlog_license_day);
								
			 					$update		= db_update('l_contractor_license_application')->fields(array(
																		'licence_qr_code'				=>	''	,
																		'contractor_license_number'		=> NULL,
																		'next_renweal_date'				=> $next_renweal_date,
																		'block_next_renwal_date'		=> $next_renweal_date
									
									
								 							))
							
													->condition('id',$licenseid_my)
													->execute();
			
			$update_status   			=  	array('recommand_date_expire' => $recomented_date, 'backlog_renewal_date' =>$recomented_date,'next_renweal_date' => $next_renweal_date,'modification_date'=>time());
			 $apply_status  	=  	db_update('l_contractor_license_apply_renweal');
	 $apply_status		->	fields($update_status);
				
	$apply_status		->	condition('id',$particularid);
	$apply_status		->  condition('action','R','=');
	$apply_status		->	execute() ;	
	break;	
				
				}	
				 }
		
				if($rows != 'form_vii'){			
				    $message_identifier = 5;
				if($is_recommanden_date == '1'){
					
				
								$appliyyear					= date('Y', strtotime($recomented_date));
								$backlog_license_year		= date('Y', strtotime($recomented_date));
								$backlog_license_month		= date('m', strtotime($recomented_date));

								$backlog_license_day		= date('d', strtotime($recomented_date));
								$next_renweal_date			= ($appliyyear+1).'-'.$backlog_license_month.'-'.($backlog_license_day);
								
			 					$update				= db_update('l_contractor_license_application')
														->fields(array(
																		'licence_qr_code'				=>	''	,
																		'contractor_license_number'		=> NULL,
																		'next_renweal_date'				=> $next_renweal_date,
																		'block_next_renwal_date'		=> $next_renweal_date
									
									
								 								))
							
													->condition('id',$licenseid_my)
													->execute();
			
			$update_status   			=  	array('recommand_date_expire' => $recomented_date, 'backlog_renewal_date' =>$recomented_date,'next_renweal_date' => $next_renweal_date,'modification_date'=>time());
			 $apply_status  	=  	db_update('l_contractor_license_apply_renweal');
	 $apply_status		->	fields($update_status);
				
	$apply_status		->	condition('id',$particularid);
	$apply_status		->  condition('action','R','=');
	$apply_status		->	execute() ;	
		$message_identifier = 3;
	break;	
				
				}
						
					
				}
		
		 		
			       
			 }
		 }
					 
			 
		 
	}
		
					
	$array = array('fragment' => '');
	$message = '';
	if(empty($message_identifier)){
		$array = array('fragment' => 'remark_view');
	
	      $message = 'hello';
		  
		    drupal_goto('alc/alc_view_backlog_contractor_license_renewal/'.$val['renewal_id'].'/'.$val['formV'].'/'.$contractor_particularid.'/'.$licenseid.'#'.$message, array('fragment' => 'remark_view')) ;
	}
    if($message_identifier == '1'){
		//echo $dd = $_POST['remark_alc_text_renewal'];die();
		//print_r();;
	
	     drupal_set_message(t('Please verify all data properly by checking all check box in <strong>"Is Verified?"</strong> column then you can proceed for Approval of application remark. <br>N.B.:-"<strong>Must be select for 25% additional fees,2.Personal Information(Incase of Individuals),6(E).Rate of Wages,6(H).Other Condition of service</strong>" are optional'),'error');
		  	// $form_state['values']['remark_alc_text_renewal_1'] = $dd;

  //$form_state['rebuild'] =TRUE;
   drupal_goto('alc_view_backlog_contractor_license_renewal/'.$val['renewal_id'].'/'.$val['formV'].'/'.$contractor_particularid.'/'.$licenseid.'/1') ;
	}
    if($message_identifier == '2'){
		
	      drupal_set_message(t('Input provided by applicant in "<b>E.Date of expiry of the previous license</b>" is not matched with ALC recommended date. Please back the application to applicant to correct the same.'),'error');
		   drupal_goto('alc/alc_view_backlog_contractor_license_renewal/'.$val['renewal_id'].'/'.$val['formV'].'/'.$contractor_particularid.'/'.$licenseid) ;
	}
     if($message_identifier == '3'){
		
	     drupal_set_message(t('Your recommended date is successfully submitted.If you want to issue the certificate, please verify <strong>"Form VII" by checking check box in "Is Verified?" column</strong>.'));
		  drupal_goto('alc/alc_view_backlog_contractor_license_renewal/'.$val['renewal_id'].'/'.$val['formV'].'/'.$contractor_particularid.'/'.$licenseid) ;
	 }
	if($message_identifier == '4'){
		
		drupal_set_message(t('Input provided by applicant in "<b>E.Date of expiry of the previous license</b>" is not matched with ALC recommended date. Please uncheked the check box and back the application to applicant to correct the same.'),'error');
		   drupal_goto('alc/alc_view_backlog_contractor_license_renewal/'.$val['renewal_id'].'/'.$val['formV'].'/'.$contractor_particularid.'/'.$licenseid) ;
	}
	      
	if($message_identifier == '5'){
		$message = '';
		$message = 'fromv';
		$array = array('fragment' => 'remark_view');
		   drupal_goto('alc_view_backlog_contractor_license_renewal/'.$val['renewal_id'].'/'.$val['formV'].'/'.$contractor_particularid.'/'.$licenseid.'#'.$message, $array) ;
		
	}	

}

function renewal_remarks_delete ($remarkid,$licenserenewalid,$licenseid,$serialnofromv,$contractorparticularid){
		global $user;
	 $uid = $user->uid;	
	
	$remark_id 			= encryption_decryption_fun('decrypt',$remarkid);
	$renewal_id 		= encryption_decryption_fun('decrypt',$licenserenewalid);
	$license_id 		= encryption_decryption_fun('decrypt',$licenseid);
	$serial_no_fromv    = encryption_decryption_fun('decrypt',$serialnofromv);
	
	$remark_by_role 	= 	db_select('l_custom_user_detail', 'user_details');
	$remark_by_role		->	fields('user_details', array('fullname'))->condition('user_details.usr_id',$uid,'=');
	$remark_by_name		=	$remark_by_role->execute()->fetchObject()->fullname;	
	
	$query_expire 		= 	db_select('l_contractor_license_apply_renweal', 'renewal')
						->	fields('renewal', array('recommand_date_expire','backlog_renewal_date'))
						->	condition('renewal.id', $renewal_id)
						->	orderBy('id', 'DESC')
			            ->  extend('PagerDefault')->limit(1);
						 
   $query_expire_date	 = $query_expire->execute()->fetchAssoc();
   $recommand_date_expire = $query_expire_date['recommand_date_expire'];
   $backlog_renewal_date = $query_expire_date['backlog_renewal_date'];
   
   db_query("SELECT fun_del_log_remark ('".$remark_id."','".$remark_by_name."')");
   
   $tag_query 	= 	db_select('l_contractor_activity_tag', 'tag')
				->	fields('tag', array('id'))
				->	condition('tag.from_v_serial_no', $serial_no_fromv)
				->	condition('tag.renewal_license_ammendment_id', $renewal_id)
				->	condition('tag.flag', 'R')
				->	orderBy('id', 'DESC')
			    ->  extend('PagerDefault')->limit(1);
	$activity_tag_details	= $tag_query->execute()->fetchAssoc();
  		
	$delquery			= db_delete('l_remark_license')->condition('id', $remark_id)->execute();
	$deltag		= db_delete('l_contractor_activity_tag')->condition('id', $activity_tag_details['id'])->execute();	

	
	$query 	= 	db_select('l_remark_license', 'lrl')
				->	fields('lrl', array('remark_type','remark_by'))
				->	condition('lrl.remark_to', $serial_no_fromv)
				->	condition('lrl.particular_id', $renewal_id)
				->	condition('lrl.flag', 'R')
				->	orderBy('id', 'DESC')
			    ->  extend('PagerDefault')->limit(1);
						 
 $query_details		= $query->execute()->fetchAssoc();
 $remark_type		= $query_details['remark_type'];	
 
 $remark_by			= $query_details['remark_by'];

 if(empty($remark_type)){
		$remark_type = 'F';
	}
				 
	
	if($remark_type	==	'U'){
		$update				= db_update('l_contractor_license_apply_renweal')
							->fields(array(
											'renewal_appliaction_status'  	=> $remark_type,
											'renewal_certificate_id'		=>	0	,
											'remark_by_userid'				=> $remark_by	
									
									
								 	))
							->condition('id',$renewal_id)
							->execute();
							
							
							
	 $update				= db_update('l_contractor_license_application')
							->fields(array(
											'licence_qr_code'				=>	''	,
											'contractor_license_number'		=> NULL,
									
									
								 	))
							
							->condition('id',$license_id)
							->execute();
	}else{
	
	// update mtw master table
	$update				= db_update('l_contractor_license_apply_renweal')
							->fields(array(
									'renewal_appliaction_status' => $remark_type,
									'remark_by_userid'			=> $remark_by
								  ))
							->condition('id',$renewal_id)
							->execute();
	}
	
$message = 'HI';
	drupal_goto('alc/alc_view_backlog_contractor_license_renewal/'.$licenserenewalid.'/'.$serialnofromv.'/'.$contractorparticularid.'/'.$licenseid.'#'.$message, array('fragment' => 'remark_view')) ;
	
	
	drupal_set_message(t('Remark has been successfully saved.'));

	return $form;
}
