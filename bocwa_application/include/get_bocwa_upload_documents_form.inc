<?php 

function get_bocwa_upload_document_form($form, &$form_state, $id=''){ 
	drupal_add_library('system','ui.datepicker');
	global $base_root,$base_path,$user;
	
	$user_id 				= $user->uid;
	$application_id			= encryption_decryption_fun('decrypt',$id);
	
	
	drupal_add_js(drupal_get_path('module', 'bocwa_application') . '/js/bocwa_upload_custom.js');
	
	
	/**** DOC UPLOADED CODE *****/
					
	$fetch_reg_bocwa_upload_files	=	db_query("select * from l_documents_upload where application_id =:application_id and user_id=:user_id",array( ':application_id' => $application_id, ':user_id' => $user_id ));
		
		if( $fetch_reg_bocwa_upload_files->rowCount() > 0 ){
			
			foreach($fetch_reg_bocwa_upload_files as $obj3){ 
				
				$trade_license_file						=	$obj3->trade_license_file;
				$article_of_assoc_file					=	$obj3->article_of_assoc_file;
				$memorandum_of_cert_file				=	$obj3->memorandum_of_cert_file;
				$partnership_deed_file					=	$obj3->partnership_deed_file;
				$challan_file							=   $obj3->challan_file;
				$work_order_file						=	$obj3->work_order_file;
				$form_one_asses_ses_file				=   $obj3->form_one_asses_ses_file;
				$supp_asses_ses_file					=	$obj3->supp_asses_ses_file;
				$other_doc_file							=	$obj3->other_doc_file;
				$bocwa_address_proof_file				=	$obj3->address_proof_file;
			}
		}
					
					
	/**** DOC UPLOADED CODE *****/
	
	$bocwa_fetch_reg_det		=	db_query("select identification_number, act_id, final_submit_status, status from l_bocwa_registration_master where id=:application_id and user_id=:user_id", array( ':application_id' => $application_id, ':user_id' => $user_id ));
	
	if($bocwa_fetch_reg_det->rowCount() > 0 ){ 
		
				foreach($bocwa_fetch_reg_det as $obj2){ 
					$identification_number					=	$obj2->identification_number;
					$act_id									=   $obj2->act_id;
					$status									= 	$obj2->status;	
					$final_submit_status					= 	$obj2->final_submit_status;
				}
				
	}
	
	
	
	
	$readonly_trade_license 							= array( 'id' => 'bocwa_upload_check_in_1_id', 'onclick' => 'bocwa_upload_check_fun_1()', $tlf => $tlf );
	$readonly_article_of_assoc 							= array( 'id' => 'bocwa_upload_check_in_2_id', 'onclick' => 'bocwa_upload_check_fun_2()', $asf => $asf );
	$readonly_memorandum_of_cert 						= array( 'id' => 'bocwa_upload_check_in_3_id', 'onclick' => 'bocwa_upload_check_fun_3()', $mcf => $mcf );
	$readonly_partnership_deed 							= array( 'id' => 'bocwa_upload_check_in_4_id', 'onclick' => 'bocwa_upload_check_fun_4()', $pdf => $pdf );
	$readonly_challan									= array( 'id' => 'bocwa_upload_check_in_5_id', 'onclick' => 'bocwa_upload_check_fun_5()', $chf => $chf );
	$readonly_work_order								= array( 'id' => 'bocwa_upload_check_in_6_id', 'onclick' => 'bocwa_upload_check_fun_6()', $wof => $wof );
	$readonly_form_one_asses_ses						= array( 'id' => 'bocwa_upload_check_in_7_id', 'onclick' => 'bocwa_upload_check_fun_7()', $fsf => $fsf );
	$readonly_supp_asses_ses 							= array( 'id' => 'bocwa_upload_check_in_8_id', 'onclick' => 'bocwa_upload_check_fun_8()', $sas => $sas );
	$readonly_other_doc									= array( 'id' => 'bocwa_upload_check_in_9_id', 'onclick' => 'bocwa_upload_check_fun_9()', $odf => $odf );
	$readonly_bocwa_address_proof						= array( 'id' => 'bocwa_upload_check_in_10_id','onclick' => 'bocwa_upload_check_fun_10()',$adf => $adf );
	
	if(!empty($application_id) && $status == 'B') {
		$query 		 =  db_select('l_bocwa_remark_master', 'lbkrm')
					 ->	fields('lbkrm', array('remark_field_title'))
					 ->	condition('lbkrm.remark_to', trim($user_id))
					 -> condition('lbkrm.application_id', $application_id)
					 ->	orderBy('lbkrm.id','DESC');
		$remark_field_text_query = $query->execute()->fetchAssoc(); 
					 
		$remark_arr  = explode(',', $remark_field_text_query['remark_field_title']);
		
		
		foreach($remark_arr as $field_arr_val){				
					
					switch($field_arr_val){	
					
						case "trade_license_file":
										$readonly_trade_license 							= array('disabled' => 'disabled', 'class' => array());
										break;
										
						case "article_of_assoc_file":
										$readonly_article_of_assoc 							= array('disabled' => 'disabled', 'class' => array());
										break;
								
						case "memorandum_of_cert_file":
										$readonly_memorandum_of_cert 						= array('disabled' => 'disabled', 'class' => array());
										break;
										
						case "partnership_deed_file":
										$readonly_partnership_deed 							= array('disabled' => 'disabled', 'class' => array());
										break;
										
						case "challan_file":
										$readonly_challan 									= array('disabled' => 'disabled', 'class' => array());
										break;
										
						case "work_order_file":
										$readonly_work_order 								= array('disabled' => 'disabled', 'class' => array());
										break;	
													
						case "form_one_asses_ses_file":
										$readonly_form_one_asses_ses 						= array('disabled' => 'disabled', 'class' => array());
										break;
										
						case "supp_asses_ses_file":
										$readonly_supp_asses_ses 							= array('disabled' => 'disabled', 'class' => array());
										break;
																			
										
						case "other_doc_file":
										$readonly_other_doc 								= array('disabled' => 'disabled', 'class' => array());
										break;
										
						case "bocwa_address_proof_file":
										$readonly_bocwa_address_proof 						= array('disabled' => 'disabled', 'class' => array());
										break;
										
						default:
					}
					
		}
	}
	
	/** Trade License **/																			
	
	
	if ( $trade_license_file != "" ) {
		
		$trade_license_link 	= l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_trade_license/'.$trade_license_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
	}else{
			$trade_license_link = '';
		
	}
		
	$upload_value_1 = array( 'trade_license' => t('Trade License&nbsp;&nbsp;&nbsp;&nbsp;'.$trade_license_link ));
	
	
	
	/** Article of Association **/
	
	
	if ( $article_of_assoc_file != "" ) {
		
		
		$article_of_assoc_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_article_of_assoc/'.$article_of_assoc_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
	}else{
			$article_of_assoc_link = '';
		
	}
	
	$upload_value_2 = array( 'article_of_assoc' => t('Articles of Association and Memorandum of Association/Partnership Deed&nbsp;&nbsp;&nbsp;&nbsp;'.$article_of_assoc_link));
	
	
	
	/**  Memorandum of Certificate **/
	
	
	
	
	if ( $memorandum_of_cert_file != "" ) {
		
		$memorandum_of_cert_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_memorandum_of_cert/'.$memorandum_of_cert_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
	}else{
			$memorandum_of_cert_link = '';
		
	}
	
	
	$upload_value_3 = array( 'memorandum_of_cert' => t('Any other licenses/registrations taken/applied to run the establishment &nbsp;&nbsp;&nbsp;&nbsp;'.$memorandum_of_cert_link));
	
	
	/**   Partnership Deed **/
	
	
	if ( $partnership_deed_file != "" ) {
		
		$partnership_deed_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_partnership_deed/'.$partnership_deed_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
	}else{
		
		$partnership_deed_link = '';
		
	}
	
	$upload_value_4 = array( 'partnership_deed' => t('Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc.&nbsp;&nbsp;&nbsp;&nbsp;'.$partnership_deed_link));
	
	/** Challan **/
	
	
	if ( $challan_file != "" ) {
		
		$challan_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_challan/'.$challan_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
	}else{
		
		$challan_link = '';
		
	}
	
	$upload_value_5 = array( 'challan' => t('Challan&nbsp;&nbsp;&nbsp;&nbsp;'.$challan_link));
	
	
	/** Work Order **/
	
	
	
	if ( $work_order_file != "" ) {
		
		$work_order_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_work_order/'.$work_order_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
	}else{
		
		$work_order_link = '';
		
	}
	
	$upload_value_6 = array( 'work_order' => t('Work Order&nbsp;&nbsp;&nbsp;&nbsp;'.$work_order_link));
	
	
	/** Form I for assesment of SES **/
	
	
	if ( $form_one_asses_ses_file != "" ) {
		
		$form_one_asses_ses_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_form_one_asses_of_ses/'.$form_one_asses_ses_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
	}else{
		
		$form_one_asses_ses_link = '';
		
	}
	
	$upload_value_7 = array( 'form_one_asses_ses' => t('Form I for assesment of CESS&nbsp;&nbsp;&nbsp;&nbsp;'.$form_one_asses_ses_link));
	
	
	/** Documents in Support of Payment of SES **/
	
	
	
	
	if ( $supp_asses_ses_file != "" ) {
		
		$supp_asses_ses_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_support_payment_ses/'.$supp_asses_ses_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
	}else{
		
		$supp_asses_ses_link = '';
		
	}
	
	$upload_value_8 = array( 'supp_asses_ses' => t('Documents in Support of Payment of CESS&nbsp;&nbsp;&nbsp;&nbsp;'.$supp_asses_ses_link));
	
	
	
	/**Documents in Support of Correctness of  Application **/
	
	
	
	
	if ( $other_doc_file != "" ) {
		
		$other_doc_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_other_docs/'.$other_doc_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
	}else{
		
		$other_doc_link = '';
		
	}
	
	$upload_value_9 = array( 'other_doc' => t('Documents in Support of Correctness of  Application&nbsp;&nbsp;&nbsp;&nbsp;'.$other_doc_link));
	
	
	/**Addresss Proof **/
	
	
	if ( $bocwa_address_proof_file != "" ) {
		
		$bocwa_address_proof_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_address_proof/'.$bocwa_address_proof_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
	}else{
		
		$bocwa_address_proof_link = '';
		
	}
	
	$upload_value_10 = array( 'bocwa_address_proof_file' => t('Address Proof&nbsp;&nbsp;&nbsp;&nbsp;'.$bocwa_address_proof_link));
	
	
	
	if ( $trade_license_file != "" ) {
		$tlf = "checked";
	}else{
		$tlf = "";
	}
	if ( $article_of_assoc_file != "" ) {
		$asf = "checked";
	}else{
		$asf = "";
	}
	if ( $memorandum_of_cert_file != "" ) {
		$mcf = "checked";
	}else{
		$mcf = "";
	}
	if ( $partnership_deed_file != "" ) {
		$pdf = "checked";
	}else{
		$pdf = "";
	}
	if ( $challan_file != "" ) {
		$chf = "checked";
	}else{
		$chf = "";
	}
	if ( $work_order_file != "" ) {
		$wof = "checked";
	}else{
		$wof = "";
	}
	if ( $form_one_asses_ses_file != "" ) {
		$fsf = "checked";
	}else{
		$fsf = "";
	}
	if ( $supp_asses_ses_file != "" ) {
		$sas = "checked";
	}else{
		$sas = "";
	}
	if ( $other_doc_file != "" ) {
		$odf = "checked";
	}else{
		$odf = "";
	}
	if ( $bocwa_address_proof_file != "" ) {
		$adf = "checked";
	}else{
		$adf = "";
	}
	
	$form['bocwa_upload_document']['bocwa_application_id'] 					= array(
																				  '#title'				=>'Application ID',
																				  '#size'				=>t('20'),	
																				  '#type' 				=> 'hidden',
																				  '#default_value'		=> !empty($application_id) ? $application_id : '',
																				  '#attributes' 		=> array('readonly'=>'readonly')
																				);
																				
	$form['bocwa_upload_document']['bocwa_act_id'] 							= array(
																				  '#title'				=>'Act Id',
																				  '#size'				=> t('20'),	
																				  '#type' 				=> 'hidden',
																				  '#default_value'		=> !empty($act_id) ? $act_id : '',
																				  '#attributes' 		=> array('readonly'=>'readonly')
																				);
																																
	$form['bocwa_upload_document']['identification_number'] 				= array(
																				  '#title'				=>'Identification Number',
																				  '#size'				=> t('20'),	
																				  '#type' 				=> 'hidden',
																				  '#default_value'		=> !empty($identification_number) ? $identification_number : '',
																				  '#attributes' 		=> array('readonly'=>'readonly')
																				);
																				
	
	$form['bocwa_upload_document']['upload_check_in_1'] 					= array(
																				  '#prefix' 			=> '<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">UPLOAD SUPPORTING DOCUMENTS</div><div class="panel-body"><table width="100%" border="0" cellspacing="0" cellpadding="0" class="admin-table table-font table-border"><tbody><tr><td width="45%"><label class="label2" for="upload_check_in_1">',
																				  '#suffix' 			=> '</label></td>',
																				  '#type' 				=> 'checkboxes',
																				  '#options' 			=> $upload_value_1,
																				  '#attributes'			=> $readonly_trade_license
																				);
			
	$form['bocwa_upload_document']['upload_check_in_2'] 					= array(
																				  '#prefix'				=> '<td><label class="label2" for="upload_check_in_2">',
																				  '#suffix' 			=> '</label></td></tr>',
																				  '#type' 				=> 'checkboxes',
																				  '#options' 			=> $upload_value_2,
																				  '#attributes'			=> $readonly_article_of_assoc
																				);
													
	$form['bocwa_upload_document']['upload_check_in_3'] 					= array(
																				  '#prefix'				=> '<tr><td><label class="label2" for="upload_check_in_3">',
																				  '#suffix' 			=> '</label></td>',
																				  '#type' 				=> 'checkboxes',
																				  '#options' 			=> $upload_value_3,
																				  '#attributes'			=> $readonly_memorandum_of_cert
																				);
													
	$form['bocwa_upload_document']['upload_check_in_4'] 					= array(
																				  '#prefix'				=> '<td><label class="label2">',
																				  '#suffix' 			=> '</label></td></tr>',
																				  '#type' 				=> 'checkboxes',
																				  '#options' 			=> $upload_value_4,
																				  '#attributes'			=> $readonly_partnership_deed 
																				);
													
	/*$form['bocwa_upload_document']['upload_check_in_5'] 					= array(
																				  '#prefix'				=> '<tr><td><label class="label2">',
																				  '#suffix' 			=> '</label></td>',
																				  '#type' 				=> 'checkboxes',
																				  '#options' 			=> $upload_value_5,
																				  '#attributes'			=> $readonly_challan
																				); 
	*/												
	$form['bocwa_upload_document']['upload_check_in_6'] 					= array(
																				  '#prefix'				=> '<tr><td><label class="label2">',
																				  '#suffix' 			=> '</label></td>',
																				  '#type' 				=> 'checkboxes',
																				  '#options' 			=> $upload_value_6,
																				  '#attributes'			=> $readonly_work_order
																				); 
													
	$form['bocwa_upload_document']['upload_check_in_7'] 					= array(
																				  '#prefix'				=> '<td><label class="label2">',
																				  '#suffix' 			=> '</label></td></tr>',
																				  '#type' 				=> 'checkboxes',
																				  '#options' 			=> $upload_value_7,
																				  '#attributes'			=> $readonly_form_one_asses_ses
																				); 
													
	$form['bocwa_upload_document']['upload_check_in_8'] 					= array(
																				  '#prefix'				=> '<tr><td><label class="label2">',
																				  '#suffix' 			=> '</label></td>',
																				  '#type' 				=> 'checkboxes',
																				  '#options' 			=> $upload_value_8,
																				  '#attributes'			=> $readonly_supp_asses_ses 
																				); 
													
	/*$form['bocwa_upload_document']['upload_check_in_9'] 					= array(
																				  '#prefix'				=> '<td><label class="label2">',
																				  '#suffix' 			=> '</label></td>',
																				  '#type' 				=> 'checkboxes',
																				  '#options' 			=> $upload_value_9,
																				  '#attributes'			=> $readonly_other_doc
																				); 
	*/
	$form['bocwa_upload_document']['upload_check_in_10'] = array(
	  '#prefix'				=> '<td><label class="label2">',
	  '#suffix' 			=> '</label></td></tr>',
	  '#type' 				=> 'checkboxes',
	  '#options' 			=> $upload_value_10,
	  '#attributes'			=> $readonly_bocwa_address_proof
	); 
													
	
	
	$form['bocwa_upload_document']['row1'] = array(
		'#type' 	=> 'markup',
		'#markup' 	=> '<tr><td id="bocwa_trade_license_div">'
	); 	
										  
	 	
	$form['bocwa_upload_document']['trade_license'] = array(
		'#type' 				=> 'managed_file',
		'#title'				=>'1. Upload Trade License',
		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
		'#upload_location' 		=> 'public://upload/bocwa_trade_license/',
		'#process' 				=> array('import_my_file_element_process'),
		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
		'#prefix' 				=> '<label class="input input-file">',
		'#suffix' 				=> '</label>'
	);
							
	$form['bocwa_upload_document']['row2'] = array(
		'#type' 	=> 'markup',
		'#markup' 	=> '</td><td id="bocwa_article_of_assoc_div">',
	); 
	
	$form['bocwa_upload_document']['article_of_assoc'] = array(
		'#type' 				=> 'managed_file',
		'#title'				=>'2. Upload Articles of Association and Memorandum of Association/Partnership Deed',
		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
		'#upload_location' 		=> 'public://upload/bocwa_article_of_assoc/',
		'#process'				=> array('import_my_file_element_process'),
		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
		'#prefix' 				=> '<label class="input input-file">',
		'#suffix' 				=> '</label>',
	);
							
							
	$form['bocwa_upload_document']['row3'] = array(
		'#type' 	=> 'markup',
		'#markup' 	=> '</td></tr><tr><td id="bocwa_memorandum_of_cert_div">',
	);

	$form['bocwa_upload_document']['memorandum_of_cert'] = array(
		'#type' 				=> 'managed_file',
		'#title'				=> '3. Upload any other licenses/registrations taken/applied to run the establishment ',
		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
		'#upload_location' 		=> 'public://upload/bocwa_memorandum_of_cert/',
		'#process' 				=> array('import_my_file_element_process'),
		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
		'#prefix' 				=> '<label class="input-file">',
		'#suffix' 				=> '</label>'
	);	
	
	
	
	$form['bocwa_upload_document']['row4'] =  array(
		'#type' 	=> 'markup',
		'#markup' 			=> '</td><td id="bocwa_partnership_deed_div">',
	); 
	
	$form['bocwa_upload_document']['partnership_deed'] = array(
		'#type' 				=> 'managed_file',
		'#title'				=>'4. Upload Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc.',
		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
		'#upload_location' 		=> 'public://upload/bocwa_partnership_deed/',
		'#process' 				=> array('import_my_file_element_process'),
		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
		'#field_prefix' 		=> '<label class="input-file">',
		'#field_suffix' 		=> '</label>'
	);
		
	/*$form['bocwa_upload_document']['row5'] = array(
		'#type' 	=> 'markup',
		'#markup' 			=> '</td></tr><tr><td id="bocwa_challan_div">',    
	);
	
	$form['bocwa_upload_document']['challan'] = array(
		'#type' 				=> 'managed_file',
		'#title'				=>'5. Upload Challan',
		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
		'#upload_location' 		=> 'public://upload/bocwa_challan/',
		'#process' 				=> array('import_my_file_element_process'),
		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
		'#field_prefix' 		=> '<label class="input-file">',
		'#field_suffix' 		=> '</label>'
	);
	*/
	$form['bocwa_upload_document']['row6'] = array(
		'#type' 	=> 'markup',
		'#markup' 	=> '</td></tr><tr><td class="col-lg-6 col-md-6" id="bocwa_work_order_div">',    
	);
	 
	$form['bocwa_upload_document']['work_order'] = array(
		'#type' 				=> 'managed_file',
		'#title'				=>'5. Upload Work Order',
		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
		'#upload_location' 		=> 'public://upload/bocwa_work_order/',
		'#process' 				=> array('import_my_file_element_process'),
		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
		'#field_prefix' 		=> '<label class="input-file">',
		'#field_suffix' 		=> '</label>'
	);
	
	$form['bocwa_upload_document']['row7'] = array(
		'#type' 	=> 'markup',
		'#markup' 	=> '</td><td id="bocwa_form_one_asses_ses_div">',    
	);
	 
	$form['bocwa_upload_document']['form_one_asses_ses'] = array(
		'#type' 				=> 'managed_file',
		'#title'				=>'6. Upload Form I for Assesment of CESS',
		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
		'#upload_location' 		=> 'public://upload/bocwa_form_one_asses_of_ses/',
		'#process' 				=> array('import_my_file_element_process'),
		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
		'#field_prefix' 		=> '<label class="input-file">',
		'#field_suffix' 		=> '</label>'
	);
	
	$form['bocwa_upload_document']['row8'] = array(
		'#type' 	=> 'markup',
	 	'#markup' 	=> '</td></tr><tr><td id="bocwa_supp_asses_ses_div">',    
	);			
	 
	$form['bocwa_upload_document']['supp_asses_ses'] = array(
		'#type' 				=> 'managed_file',
		'#title'				=>'7. Upload Documents in support of Payment of welfare CESS',
		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
		'#upload_location' 		=> 'public://upload/bocwa_support_payment_ses/',
		'#process' 				=> array('import_my_file_element_process'),
		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
		'#field_prefix' 		=> '<label class="input-file">',
		'#field_suffix' 		=> '</label>'
	);
	
	/*$form['bocwa_upload_document']['row9'] = array(
		'#type' 	=> 'markup',
		'#markup' 	=> '</td></tr><tr><td id="bocwa_other_doc_div">',    
	);
	 
	$form['bocwa_upload_document']['other_doc'] = array(
		'#type' 				=> 'managed_file',
		'#title'				=>'9. Upload Documents in support of correctness of the Application',
		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
		'#upload_location' 		=> 'public://upload/bocwa_other_docs/',
		'#process' 				=> array('import_my_file_element_process'),
		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
		'#field_prefix' 		=> '<label class="input-file">',
		'#field_suffix' 		=> '</label>'
	);*/
	
	$form['bocwa_upload_document']['row10'] = array(
		'#type' 	=> 'markup',
		'#markup' 	=> '</td><td id="bocwa_address_proof_div">',    
	);
																				
	$form['bocwa_upload_document']['address_proof'] = array(
		'#type' 				=> 'managed_file',
		'#title'				=>'8. Address Proof',
		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
		'#upload_location' 		=> 'public://upload/bocwa_address_proof/',
		'#process' 				=> array('import_my_file_element_process'),
		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
		'#field_prefix' 		=> '<label class="input-file">',
		'#field_suffix' 		=> '</label>'
	);
																				
	$form['bocwa_upload_document']['row11'] = array(
		'#type' 	=> 'markup',
		'#markup' 	=> '</td></tr></table>', 
	);
	 
	$form['bocwa_upload_document']['endmarkup'] = array(
		'#type' 	=> 'markup',
		'#markup' 	=> '<br></div></div></div>', 
	);	
																			
	if( trim($status == 'B') || trim($final_submit_status) == '' || trim($final_submit_status) == 'S' ){
		
		$form['bocwa_upload_document']['submit'] = array (
			'#prefix' 			=> '',  
			'#suffix' 			=> '',  
			'#type' 			=> 'submit',
			'#attributes' 		=> array('class' => array('btn btn-primary pull-right')),
			'#value' 			=> 'SAVE'
		);
	}
	 
	
	return $form;
	
}



function get_bocwa_upload_document_form_submit($form, &$form_state){ 

		global $base_root, $base_path, $user;
		$val 									= $form_state['values'];
		$user_id								= $user->uid;
		$act_id									= $val['bocwa_act_id'];
		$bocwa_application_id					= $val['bocwa_application_id'];
		$identification_number					= $val['identification_number'];
		
		
		

		/* Upload For Trade License */
		
		$arr_1									= array();
		$trade_license 							= file_load($val['trade_license']);
		$trade_license_filetype					= 'BOCWA TRADE LICENSE';
		
		if( $trade_license != "" ){
			$arr_1 = explode("/", $trade_license->uri);
			$trade_license->status = FILE_STATUS_PERMANENT;
			file_save($trade_license);
			$trade_license_file = $arr_1[4];
		}
		
		
		/* Upload For Article of Association */
		
		$arr_2									= array();
		$article_of_assoc 						= file_load($val['article_of_assoc']);
		$article_of_assoc_filetype				= 'BOCWA ARTICLE OF ASSOCIATION';
		
		if( $article_of_assoc != "" ){
			$arr_2 = explode("/", $article_of_assoc->uri);
			$article_of_assoc->status = FILE_STATUS_PERMANENT;
			file_save($article_of_assoc);
			$article_of_assoc_file = $arr_2[4];
		}
		
		/* Upload For Memorandum of Certificate */
		
		$arr_3									= array();
		$memorandum_of_cert 					= file_load($val['memorandum_of_cert']);
		$memorandum_of_cert_filetype			= 'BOCWA MEMORANDUM OF CERTIFICATE';
		
		
		if( $memorandum_of_cert != "" ){
			$arr_3 = explode("/", $memorandum_of_cert->uri);
			$memorandum_of_cert->status = FILE_STATUS_PERMANENT;
			file_save($memorandum_of_cert);
			$memorandum_of_cert_file = $arr_3[4];
		}
		
		/* Upload For Partnership Deed */
		
		$arr_4								= array();
		$partnership_deed 					= file_load($val['partnership_deed']);
		$partnership_deed_filetype			= 'BOCWA PARTNERSHIP DEED';
		
		if( $partnership_deed != "" ){
			$arr_4 = explode("/", $partnership_deed->uri);
			$partnership_deed->status = FILE_STATUS_PERMANENT;
			file_save($partnership_deed);
			$partnership_deed_file = $arr_4[4];
		}
		
		/* Upload For Challan */
		
		$arr_5						= array();
		$challan 					= file_load($val['challan']);
		$challan_filetype			= 'BOCWA CHALLAN';
		
		if( $challan != "" ){
			$arr_5 = explode("/", $challan->uri);
			$challan->status = FILE_STATUS_PERMANENT;
			file_save($challan);
			$challan_file = $arr_5[4];
		}
		
		/* Upload For Work Order */
		
		$arr_6							= array();
		$work_order 					= file_load($val['work_order']);
		if( $work_order != "" ){
			$arr_6 = explode("/", $work_order->uri);
			$work_order->status = FILE_STATUS_PERMANENT;
			file_save($work_order);
			$work_order_file = $arr_6[4];
		}
		
		/* Upload For Form I for Assesment of CESS */
		
		$arr_7							= array();
		$form_one_asses_ses 			= file_load($val['form_one_asses_ses']);
		if( $form_one_asses_ses != "" ){
			$arr_7 = explode("/", $form_one_asses_ses->uri);
			$form_one_asses_ses->status = FILE_STATUS_PERMANENT;
			file_save($form_one_asses_ses);
			$form_one_asses_ses_file = $arr_7[4];
		}
		
		/* Upload For Document in support of Payment of CESS */
		
		$arr_8							= array();
		$supp_asses_ses 				= file_load($val['supp_asses_ses']);
		if( $supp_asses_ses != "" ){
			$arr_8 = explode("/", $supp_asses_ses->uri);
			$supp_asses_ses->status = FILE_STATUS_PERMANENT;
			file_save($supp_asses_ses);
			$supp_asses_ses_file = $arr_8[4];
		}
		
		/* Upload For Other Document in support of Correctness of Application */
		
		$arr_9							= array();
		$other_doc 				= file_load($val['other_doc']);
		if( $other_doc != "" ){
			$arr_9 = explode("/", $other_doc->uri);
			$other_doc->status = FILE_STATUS_PERMANENT;
			file_save($other_doc);
			$other_doc_file = $arr_9[4];
		}
		
		
		/* Upload For Address Proof */
		
		$arr_10							= array();
		$address_proof 				= file_load($val['address_proof']);
		if( $address_proof != "" ){
			$arr_10 = explode("/", $address_proof->uri);
			$address_proof->status = FILE_STATUS_PERMANENT;
			file_save($address_proof);
			$address_proof_file = $arr_10[4];
		}
		$uploaded_files =  array();
		
		
		$fetch_reg_bocwa_upload_files = db_query("select * from l_documents_upload where application_id=:application_id and user_id=:user_id ",array(':application_id'=>$bocwa_application_id, ':user_id'=>$user_id));
		
		
		if( $fetch_reg_bocwa_upload_files->rowCount() > 0 ){
			
			foreach($fetch_reg_bocwa_upload_files as $obj3){ 
				
				$identification_number_old					=	$obj3->identification_number;
				$trade_license_file_old						=	$obj3->trade_license_file;
				$article_of_assoc_file_old					=	$obj3->article_of_assoc_file;
				$memorandum_of_cert_file_old				=	$obj3->memorandum_of_cert_file;
				$partnership_deed_file_old					=	$obj3->partnership_deed_file;
				$challan_file_old							=   $obj3->challan_file;
				$work_order_file_old						=	$obj3->work_order_file;
				$form_one_asses_ses_file_old				=   $obj3->form_one_asses_ses_file;
				$supp_asses_ses_file_old					=	$obj3->supp_asses_ses_file;
				$other_doc_file_old							=	$obj3->other_doc_file;
				$bocwa_address_proof_file_old				=	$obj3->address_proof_file;
				$act_id_old									=	$obj3->act_id;
			}
			
			$queryForward = db_update('l_documents_upload');
			$queryForward->fields(array(
											'trade_license_file' 						=>  isset($trade_license_file)&&($trade_license_file_old!=$trade_license_file) ? $trade_license_file : $trade_license_file_old,
											'article_of_assoc_file' 					=>  isset($article_of_assoc_file)&&($article_of_assoc_file_old!=$article_of_assoc_file) ? $article_of_assoc_file : $article_of_assoc_file_old,
											'memorandum_of_cert_file' 					=>  isset($memorandum_of_cert_file)&&($memorandum_of_cert_file_old!=$memorandum_of_cert_file) ? $memorandum_of_cert_file : $memorandum_of_cert_file_old,
											'partnership_deed_file' 					=>  isset($partnership_deed_file)&&($partnership_deed_file_old!=$partnership_deed_file) ? $partnership_deed_file : $partnership_deed_file_old,
											'challan_file'								=>  isset($challan_file)&&($challan_file_old!=$challan_file) ? $challan_file : $challan_file_old,
											'work_order_file'							=>	isset($work_order_file)&&($work_order_file_old!=$work_order_file) ? $work_order_file : $work_order_file_old,
											'form_one_asses_ses_file'					=>  isset($form_one_asses_ses_file)&&($form_one_asses_ses_file_old!=$form_one_asses_ses_file) ? $form_one_asses_ses_file : $form_one_asses_ses_file_old,
											'supp_asses_ses_file'						=> 	isset($supp_asses_ses_file)&&($supp_asses_ses_file_old!=$supp_asses_ses_file) ? $supp_asses_ses_file : $supp_asses_ses_file_old, 
											'other_doc_file'							=>  isset($other_doc_file)&&($other_doc_file_old!=$other_doc_file) ? $other_doc_file : $other_doc_file_old,
											 
											'address_proof_file'					=>	isset($address_proof_file)&&($bocwa_address_proof_file_old!=$address_proof_file) ? $address_proof_file : $bocwa_address_proof_file_old,
			 ));
			$queryForward->condition('application_id',$bocwa_application_id);
			$queryForward->condition('user_id',$user_id);
			$queryForward->execute();
			
			$message													=	"Documents Successfully Uploaded";
			drupal_set_message($message);
			
		}else{
		
			$fieldsBocwaDocumentsData 			= array( 
															'user_id'									=>	$user->uid,
															'act_id'									=>  $act_id,
															'application_id'							=>  $bocwa_application_id,
															'identification_number'						=>	$identification_number,
															'trade_license_file' 						=>  $trade_license_file,
															'article_of_assoc_file' 					=>  $article_of_assoc_file,
															'memorandum_of_cert_file' 					=>  $memorandum_of_cert_file,
															'partnership_deed_file' 					=>  $partnership_deed_file,
															'challan_file'								=>  $challan_file,
															'work_order_file'							=>  $work_order_file,
															'form_one_asses_ses_file'					=>  $form_one_asses_ses_file,
															'supp_asses_ses_file'						=>  $supp_asses_ses_file,
															'other_doc_file'							=>  $other_doc_file,
															'address_proof_file'					=>	$address_proof_file,
														);
														
					
			db_insert('l_documents_upload')->fields($fieldsBocwaDocumentsData)->execute();
					
			$message													=	"Documents Successfully Uploaded";
			drupal_set_message($message);
		}
		
}