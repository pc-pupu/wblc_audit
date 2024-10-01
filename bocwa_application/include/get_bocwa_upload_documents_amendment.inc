<?php 

function get_bocwa_upload_document_amendment($form, &$form_state, $id=''){ 
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
				
				
	$master_part_bocwa_encrypted_docs = db_select('l_encrypted_uploaded_documents', 'leud');
	$master_part_bocwa_encrypted_docs->fields('leud', array());
	$master_part_bocwa_encrypted_docs->condition('leud.user_id', $user_id);
	$master_part_bocwa_encrypted_docs->condition('leud.act_id', '2');
	$master_part_bocwa_encrypted_docs->condition('leud.application_id', $application_id);
	$master_part_bocwa_encrypted_docs_result = $master_part_bocwa_encrypted_docs->execute();
	
	$content_encrypted_docs	=	$master_part_bocwa_encrypted_docs_result->fetchAssoc();
		
	$enc_doc_array = array('TL'=> '','PD'=> '','AOA'=> '','MOA'=> '','MOC'=>'','FL'=> '','AC'=> '','PC'=> '','BB'	=> '','SC'	=> '','IC'	=> '','AP'=> '','ODSC'=> '','CR'=> '','WO'=> '','ORC'=> '','CH'=>'');
	
	$pdficonlink = '<img title="View Documents" alt="View Documents" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/pdf.png"> &nbsp;';
	
	$query 			= db_select('l_encrypted_uploaded_documents','leud')->fields('leud',array())->condition('application_id',$application_id)->condition('act_id','2')->condition('status','1')->condition('user_id',$user_id);
	$query_result 	= $query->execute();
	$document_type_arr=array();
	if($query_result->rowCount() > 0){
		foreach($query_result->fetchAll() as $data ){
			$enc_doc_array[$data->document_type_code]['content'] = 'view_documents/'.encryption_decryption_fun('encrypt',$data->id);
		}
	}
	
	$global_link = $base_root.$base_path.'sites/default/files/upload/';
					
	/**** DOC UPLOADED CODE *****/
	
	$bocwa_fetch_reg_det		=	db_query("select identification_number, act_id, final_submit_status, status from l_bocwa_registration_master where id=:application_id and user_id=:user_id", array( ':application_id' => $application_id, ':user_id' => $user_id ));
	
	if($bocwa_fetch_reg_det->rowCount() > 0 ){ 
		
				foreach($bocwa_fetch_reg_det as $obj2){ 
					$identification_number					=	$obj2->identification_number;
					$act_id									=   $obj2->act_id;
					$status									= 	'B';	
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
	
	
	
	
	/** Trade License **/																			
	
	
	if($enc_doc_array['TL']['content'] !=''){
									
									
				$trade_license_link .= l(t('View Document'),$enc_doc_array['TL']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
			}elseif ( $trade_license_file != "" ) {
		
		$trade_license_link 	= l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_trade_license/'.$trade_license_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
	}else{
			$trade_license_link = '';
		
	}
		
	$upload_value_1 = array( 'trade_license' => t('Trade License&nbsp;&nbsp;&nbsp;&nbsp;'.$trade_license_link ));
	
	
	
	/** Article of Association **/
	
	
	if($enc_doc_array['AOA']['content'] !=''){
									
									
			$article_of_assoc_link .= l(t('View Document'),$enc_doc_array['AOA']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}elseif ( $article_of_assoc_file != "" ) {

		
		$article_of_assoc_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_article_of_assoc/'.$article_of_assoc_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
	}else{
			$article_of_assoc_link = '';
		
	}
	
	$upload_value_2 = array( 'article_of_assoc' => t('Articles of Association and Memorandum of Association/Partnership Deed&nbsp;&nbsp;&nbsp;&nbsp;'.$article_of_assoc_link));
	
	
	
	/**  Memorandum of Certificate **/
	
	
	
	
	if($enc_doc_array['MOC']['content'] !=''){
									
									
				$memorandum_of_cert_link .= l(t('View Document'),$enc_doc_array['MOC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}elseif ( $memorandum_of_cert_file != "" ) {
		
		$memorandum_of_cert_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_memorandum_of_cert/'.$memorandum_of_cert_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
	}else{
			$memorandum_of_cert_link = '';
		
	}
	
	
	$upload_value_3 = array( 'memorandum_of_cert' => t('Any other document in support of correctness of the particulars mentioned in the application if required&nbsp;&nbsp;&nbsp;&nbsp;'.$memorandum_of_cert_link));
	
	
	/**   Partnership Deed **/
	
	
	if($enc_doc_array['PD']['content'] !=''){
									
					$partnership_deed_link .= l(t('View Document'),$enc_doc_array['PD']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
			}elseif ( $partnership_deed_file != "" ) {
		
		$partnership_deed_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_partnership_deed/'.$partnership_deed_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
	}else{
		
		$partnership_deed_link = '';
		
	}
	
	$upload_value_4 = array( 'partnership_deed' => t('Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc.&nbsp;&nbsp;&nbsp;&nbsp;'.$partnership_deed_link));
	
	/** Challan **/
	
	
	 if($enc_doc_array['CH']['content'] !=''){
									
				$challan_link .= l(t('View Document'),$enc_doc_array['CH']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}elseif ( $challan_file != "" ) {

		$challan_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_challan/'.$challan_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
	}else{
		
		$challan_link = '';
		
	}
	
	$upload_value_5 = array( 'challan' => t('Challan&nbsp;&nbsp;&nbsp;&nbsp;'.$challan_link));
	
	
	/** Work Order **/
	
	
	
	 if($enc_doc_array['WO']['content'] !=''){
									
				$work_order_link .= l(t('View Document'),$enc_doc_array['WO']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}elseif ( $work_order_file != "" ) {
		
		$work_order_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_work_order/'.$work_order_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
	}else{
		
		$work_order_link = '';
		
	}
	
	$upload_value_6 = array( 'work_order' => t('Work Order&nbsp;&nbsp;&nbsp;&nbsp;'.$work_order_link));
	
	
	/** Form I for assesment of SES **/
	
	
	if($enc_doc_array['AC']['content'] !=''){
									
				$form_one_asses_ses_link .= l(t('View Document'),$enc_doc_array['AC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}elseif ( $form_one_asses_ses_file != "" ) {
		
		$form_one_asses_ses_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_form_one_asses_of_ses/'.$form_one_asses_ses_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
	}else{
		
		$form_one_asses_ses_link = '';
		
	}
	
	$upload_value_7 = array( 'form_one_asses_ses' => t('Form I for assesment of CESS&nbsp;&nbsp;&nbsp;&nbsp;'.$form_one_asses_ses_link));
	
	
	/** Documents in Support of Payment of SES **/
	
	
	
	
	if($enc_doc_array['PC']['content'] !=''){
									
				$supp_asses_ses_link .= l(t('View Document'),$enc_doc_array['PC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}elseif ( $supp_asses_ses_file != "" ) {
		
		$supp_asses_ses_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_support_payment_ses/'.$supp_asses_ses_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
	}else{
		
		$supp_asses_ses_link = '';
		
	}
	
	$upload_value_8 = array( 'supp_asses_ses' => t('Documents in Support of Payment of CESS&nbsp;&nbsp;&nbsp;&nbsp;'.$supp_asses_ses_link));
	
	
	
	/**Documents in Support of Correctness of  Application **/
	
	
	
	
	 if($enc_doc_array['ODSC']['content'] !=''){
									
				$other_doc_link .= l(t('View Document'),$enc_doc_array['ODSC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
		}elseif ( $other_doc_file != "" ) {
		
		$other_doc_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_other_docs/'.$other_doc_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
	}else{
		
		$other_doc_link = '';
		
	}
	
	$upload_value_9 = array( 'other_doc' => t('Documents in Support of Correctness of Application&nbsp;&nbsp;&nbsp;&nbsp;'.$other_doc_link));
	
	
	/**Addresss Proof **/
	
	
	if($enc_doc_array['AP']['content'] !=''){
									
					$bocwa_address_proof_link .= l(t('View Document'),$enc_doc_array['AP']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank')));
			}elseif ( $bocwa_address_proof_file != "" ) {
		
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
													
	$form['bocwa_upload_document']['upload_check_in_5'] 					= array(
																				  '#prefix'				=> '<tr><td><label class="label2">',
																				  '#suffix' 			=> '</label></td>',
																				  '#type' 				=> 'checkboxes',
																				  '#options' 			=> $upload_value_5,
																				  '#attributes'			=> $readonly_challan
																				); 
													
	$form['bocwa_upload_document']['upload_check_in_6'] 					= array(
																				  '#prefix'				=> '<td><label class="label2">',
																				  '#suffix' 			=> '</label></td></tr>',
																				  '#type' 				=> 'checkboxes',
																				  '#options' 			=> $upload_value_6,
																				  '#attributes'			=> $readonly_work_order
																				); 
													
	$form['bocwa_upload_document']['upload_check_in_7'] 					= array(
																				  '#prefix'				=> '<tr><td><label class="label2">',
																				  '#suffix' 			=> '</label></td>',
																				  '#type' 				=> 'checkboxes',
																				  '#options' 			=> $upload_value_7,
																				  '#attributes'			=> $readonly_form_one_asses_ses
																				); 
													
	$form['bocwa_upload_document']['upload_check_in_8'] 					= array(
																				  '#prefix'				=> '<td><label class="label2">',
																				  '#suffix' 			=> '</label></td></tr>',
																				  '#type' 				=> 'checkboxes',
																				  '#options' 			=> $upload_value_8,
																				  '#attributes'			=> $readonly_supp_asses_ses 
																				); 
													
	$form['bocwa_upload_document']['upload_check_in_9'] 					= array(
																				  '#prefix'				=> '<tr><td><label class="label2">',
																				  '#suffix' 			=> '</label></td>',
																				  '#type' 				=> 'checkboxes',
																				  '#options' 			=> $upload_value_9,
																				  '#attributes'			=> $readonly_other_doc
																				); 
																				
	$form['bocwa_upload_document']['upload_check_in_10'] 					= array(
																				  '#prefix'				=> '<td><label class="label2">',
																				  '#suffix' 			=> '</label></td></tr>',
																				  '#type' 				=> 'checkboxes',
																				  '#options' 			=> $upload_value_10,
																				  '#attributes'			=> $readonly_bocwa_address_proof
																				); 
													
	
	
	$form['bocwa_upload_document']['row1'] 									= array(
																				  //'#markup' 			=> '<br><div class="col-lg-6 col-md-6" id="bocwa_trade_license_div">'
																				  '#markup' 			=> '<tr><td id="bocwa_trade_license_div">'
																				 
																				  ); 	
										  
	 	
	$form['bocwa_upload_document']['trade_license'] 						= array(
																				'#type' 				=> 'managed_file',
																				'#title'				=>'1. Upload Trade License',
																				'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																				'#upload_location'		=> 'public://upload/encrypt/',
																				'#process' 				=> array('import_my_file_element_process'),
																				'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																				'#prefix' 				=> '<label class="input input-file">',
																				'#suffix' 				=> '</label>'
																				);
							
	$form['bocwa_upload_document']['row2'] 									= array(
																				  '#markup' 			=> '</td><td id="bocwa_article_of_assoc_div">',
																				  ); 
	
	$form['bocwa_upload_document']['article_of_assoc'] 						= array(
																				'#type' 				=> 'managed_file',
																				'#title'				=>'2. Upload Articles of Association and Memorandum of Association/Partnership Deed',
																				'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																				'#upload_location' 		=> 'public://upload/encrypt/',
																				'#process'				=> array('import_my_file_element_process'),
																				'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																				'#prefix' 				=> '<label class="input input-file">',
																				'#suffix' 				=> '</label>',
																		);
							
							
	$form['bocwa_upload_document']['row3'] 									= array(
																				'#markup' 				=> '</td></tr><tr><td id="bocwa_memorandum_of_cert_div">',
																			 );

	$form['bocwa_upload_document']['memorandum_of_cert'] 					= array(
																				'#type' 				=> 'managed_file',
																				'#title'				=> '3. Upload Any other document in support of correctness of the particulars mentioned in the application if required',
																				'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																				'#upload_location' 		=> 'public://upload/encrypt/',
																				'#process' 				=> array('import_my_file_element_process'),
																				'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																				'#prefix' 				=> '<label class="input-file">',
																				'#suffix' 				=> '</label>'
																		);	
	
	
	
	$form['bocwa_upload_document']['row4'] 									=  array(
																				  '#markup' 			=> '</td><td id="bocwa_partnership_deed_div">',
																				  ); 
	
	$form['bocwa_upload_document']['partnership_deed'] 						= array(
																				'#type' 				=> 'managed_file',
																				'#title'				=>'4. Upload Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc.',
																				'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																				'#upload_location' 		=> 'public://upload/encrypt/',
																				'#process' 				=> array('import_my_file_element_process'),
																				'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																				'#field_prefix' 		=> '<label class="input-file">',
																				'#field_suffix' 		=> '</label>'
																		);
	
	
	
	$form['bocwa_upload_document']['row5'] 									= array(
																					'#markup' 			=> '</td></tr><tr><td id="bocwa_challan_div">',    
																		 	  );
	
	$form['bocwa_upload_document']['challan'] 							 	= array(
																				'#type' 				=> 'managed_file',
																				'#title'				=>'5. Upload Challan',
																				'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																				'#upload_location' 		=> 'public://upload/encrypt/',
																				'#process' 				=> array('import_my_file_element_process'),
																				'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																				'#field_prefix' 		=> '<label class="input-file">',
																				'#field_suffix' 		=> '</label>'
																				);
	
	$form['bocwa_upload_document']['row6'] 								 	= array(
																			  '#markup' 				=> '</td><td class="col-lg-6 col-md-6" id="bocwa_work_order_div">',    
																 			);
	 
	 $form['bocwa_upload_document']['work_order'] 							= array(
																				'#type' 				=> 'managed_file',
																				'#title'				=>'6. Upload Work Order',
																				'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																				'#upload_location' 		=> 'public://upload/encrypt/',
																				'#process' 				=> array('import_my_file_element_process'),
																				'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																				'#field_prefix' 		=> '<label class="input-file">',
																				'#field_suffix' 		=> '</label>'
																				);
	
	$form['bocwa_upload_document']['row7'] 									= array(
																				  '#markup' 			=> '</td></tr><tr><td id="bocwa_form_one_asses_ses_div">',    
																	 			);
	 
	  $form['bocwa_upload_document']['form_one_asses_ses'] 					= array(
																				'#type' 				=> 'managed_file',
																				'#title'				=>'7. Upload Form I for Assesment of CESS',
																				'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																				'#upload_location' 		=> 'public://upload/encrypt/',
																				'#process' 				=> array('import_my_file_element_process'),
																				'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																				'#field_prefix' 		=> '<label class="input-file">',
																				'#field_suffix' 		=> '</label>'
																				);
	
	$form['bocwa_upload_document']['row8'] 									= array(
																				  '#markup' 			=> '</td><td id="bocwa_supp_asses_ses_div">',    
																	 			);			
	 
	  $form['bocwa_upload_document']['supp_asses_ses'] 						= array(
																				'#type' 				=> 'managed_file',
																				'#title'				=>'8. Upload Documents in support of Payment of welfare CESS',
																				'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																				'#upload_location' 		=> 'public://upload/encrypt/',
																				'#process' 				=> array('import_my_file_element_process'),
																				'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																				'#field_prefix' 		=> '<label class="input-file">',
																				'#field_suffix' 		=> '</label>'
																				);
	
	$form['bocwa_upload_document']['row9'] 									= array(
																				  '#markup' 			=> '</td></tr><tr><td id="bocwa_other_doc_div">',    
																	 			);
	 
	  $form['bocwa_upload_document']['other_doc'] 							= array(
																				'#type' 				=> 'managed_file',
																				'#title'				=>'9. Upload Documents in support of correctness of the Application',
																				'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																				'#upload_location' 		=> 'public://upload/encrypt/',
																				'#process' 				=> array('import_my_file_element_process'),
																				'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																				'#field_prefix' 		=> '<label class="input-file">',
																				'#field_suffix' 		=> '</label>'
																				);
	
	$form['bocwa_upload_document']['row10'] 								= array(
																				  '#markup' 			=> '</td><td id="bocwa_address_proof_div">',    
																	 			);
																				
	$form['bocwa_upload_document']['address_proof'] 							= array(
																				'#type' 				=> 'managed_file',
																				'#title'				=>'10. Address Proof',
																				'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																				'#upload_location' 		=> 'public://upload/encrypt/',
																				'#process' 				=> array('import_my_file_element_process'),
																				'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																				'#field_prefix' 		=> '<label class="input-file">',
																				'#field_suffix' 		=> '</label>'
																				);
																				
	$form['bocwa_upload_document']['row11'] 								= array(
																				  '#markup' 			=> '</td></tr></table>', 
																				  ); 
																				
	if( trim($status == 'B') || trim($final_submit_status) == '' || trim($final_submit_status) == 'S' ){
		
	$form['bocwa_upload_document']['submit'] 								= array (
																				  '#prefix' 			=> '<br></div></div></div>',  
																				  '#suffix' 			=> '',  
																				  '#type' 				=> 'submit',
																				  '#attributes' 		=> array('class' => array('btn btn-primary pull-right')),
																				  '#value' 				=> 'SAVE'
																				   );
	}
	return $form;
	
}



function get_bocwa_upload_document_amendment_submit($form, &$form_state){ 

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
		
		if( !empty($val['trade_license'])){
			$trade_license 			= file_load($val['trade_license']);
			$trade_license_arr 		= explode("/", $trade_license->uri);
			$trade_license_fname	= time().'_'.$trade_license_arr[4]; 
			$filepathSave_tl 		= file_get_contents(escapefile_url($base_root.$base_path.'sites/default/files/upload/encrypt/'.$trade_license_arr[4]));
			$trade_license_content 	= base64_encode($filepathSave_tl);
			$insert_arr['TL']['fname'] = $trade_license_fname;
			$insert_arr['TL']['content'] = $trade_license_content;
		}
		
		/* Upload For Article of Association */
		if(!empty($val['article_of_assoc'])){
			$article_of_assoc		= file_load($val['article_of_assoc']);
			$article_of_assoc_arr 	= explode("/", $article_of_assoc->uri);
			$article_of_assoc_fname	= time().'_'.$article_of_assoc_arr[4]; 
			$filepathSave_aoa 		= file_get_contents(escapefile_url($base_root.$base_path.'sites/default/files/upload/encrypt/'.$article_of_assoc_arr[4]));
			$article_of_assoc_content 	= base64_encode($filepathSave_aoa);		
			$insert_arr['AOA']['fname'] = $article_of_assoc_fname;
			$insert_arr['AOA']['content'] = $article_of_assoc_content;
		}
		
		if(!empty($val['memorandum_of_cert'])){
			$memorandum_of_cert		= file_load($val['memorandum_of_cert']);
			$article_of_assoc_arr 	= explode("/", $memorandum_of_cert->uri);
			$memorandum_of_cert_fname	= time().'_'.$article_of_assoc_arr[4]; 
			$filepathSave_aoa 		= file_get_contents(escapefile_url($base_root.$base_path.'sites/default/files/upload/encrypt/'.$article_of_assoc_arr[4]));
			$memorandum_of_cert_content 	= base64_encode($filepathSave_aoa);		
			$insert_arr['MOC']['fname'] = $memorandum_of_cert_fname;
			$insert_arr['MOC']['content'] = $memorandum_of_cert_content;
		}
		
		if(!empty($val['partnership_deed'])){
			$partnership_deed		= file_load($val['partnership_deed']);
			$partnership_deed_arr 	= explode("/", $partnership_deed->uri);
			$partnership_deed_fname	= time().'_'.$partnership_deed_arr[4]; 
			$filepathSave_aoa 		= file_get_contents(escapefile_url($base_root.$base_path.'sites/default/files/upload/encrypt/'.$partnership_deed_arr[4]));
			$memorandum_of_cert_content 	= base64_encode($filepathSave_aoa);		
			$insert_arr['PD']['fname'] = $partnership_deed_fname;
			$insert_arr['PD']['content'] = $partnership_deed_fname_content;
		}
		
		if(!empty($val['challan'])){
			$challan		= file_load($val['challan']);
			$challan_arr 	= explode("/", $challan->uri);
			$challan_fname	= time().'_'.$challan_arr[4]; 
			$filepathSave_aoa 		= file_get_contents(escapefile_url($base_root.$base_path.'sites/default/files/upload/encrypt/'.$challan_arr[4]));
			$challan_fname_content 	= base64_encode($filepathSave_aoa);		
			$insert_arr['CH']['fname'] = $challan_fname;
			$insert_arr['CH']['content'] = $challan_fname_content;
		}
		
		if(!empty($val['work_order'])){
			$work_order		= file_load($val['work_order']);
			$work_order_arr 	= explode("/", $work_order->uri);
			$work_order_fname	= time().'_'.$work_order_arr[4]; 
			$filepathSave_aoa 		= file_get_contents(escapefile_url($base_root.$base_path.'sites/default/files/upload/encrypt/'.$work_order_arr[4]));
			$work_order_content 	= base64_encode($filepathSave_aoa);		
			$insert_arr['WO']['fname'] = $work_order_fname;
			$insert_arr['WO']['content'] = $work_order_content;
		}
		
		if(!empty($val['form_one_asses_ses'])){
			$form_one_asses_ses_order		= file_load($val['form_one_asses_ses']);
			$form_one_asses_ses_order_arr 	= explode("/", $form_one_asses_ses_order->uri);
			$form_one_asses_ses_order_fname	= time().'_'.$form_one_asses_ses_order_arr[4]; 
			$filepathSave_aoa 		= file_get_contents(escapefile_url($base_root.$base_path.'sites/default/files/upload/encrypt/'.$form_one_asses_ses_order_arr[4]));
			$form_one_asses_content 	= base64_encode($filepathSave_aoa);		
			$insert_arr['AC']['fname'] = $form_one_asses_ses_order_fname;
			$insert_arr['AC']['content'] = $form_one_asses_content;
		}
		
		/* Upload For Insurance Certificate of Motor Vehicles */
		if(!empty($val['supp_asses_ses'])){
			$insr_certi		= file_load($val['supp_asses_ses']);
			$insr_certi_arr 	= explode("/", $insr_certi->uri);
			$insr_certi_fname	= time().'_'.$insr_certi_arr[4]; 
			$filepathSave_ic 	= file_get_contents(escapefile_url($base_root.$base_path.'sites/default/files/upload/encrypt/'.$insr_certi_arr[4]));
			$insr_certi_content = base64_encode($filepathSave_ic);
			$insert_arr['PC']['fname'] = $insr_certi_fname;
			$insert_arr['PC']['content'] = $insr_certi_content;
		}
		
		if(!empty($val['other_doc'])){
			$support_doc		= file_load($val['other_doc']);
			$support_doc_arr 	= explode("/", $support_doc->uri);
			$support_doc_fname	= time().'_'.$support_doc_arr[4]; 
			$filepathSave_odsc	= file_get_contents(escapefile_url($base_root.$base_path.'sites/default/files/upload/encrypt/'.$support_doc_arr[4]));
			$support_doc_content = base64_encode($filepathSave_odsc);
			$insert_arr['ODSC']['fname'] = $support_doc_fname;
			$insert_arr['ODSC']['content'] = $support_doc_content;
		}
		
		/* Upload For Address Proof */
		if(!empty($val['address_proof'])){
			$address_proof		= file_load($val['address_proof']);
			$address_proof_arr 	= explode("/", $address_proof->uri);
			$address_proof_fname	= time().'_'.$address_proof_arr[4]; 
			$filepathSave_ap	= file_get_contents(escapefile_url($base_root.$base_path.'sites/default/files/upload/encrypt/'.$address_proof_arr[4]));
			$address_proof_content = base64_encode($filepathSave_ap);
			$insert_arr['AP']['fname'] = $address_proof_fname;
			$insert_arr['AP']['content'] = $address_proof_content;
		}
		
		if(!empty($insert_arr)){
			foreach($insert_arr as $key => $row){ 
				$arr = array(
					'user_id' => $user_id,
					'act_id' => $act_id,
					'application_id' => $bocwa_application_id,
					'identification_number' => $identification_number,
					'document_type' => document_list($key),
					'document_type_code' => $key,
					'filename'		=> time().$insert_arr[$key]['fname'],
					'filecontent'	=> $insert_arr[$key]['content'],
					'update_date'	=> date('Y-m-d H:i:s'),
					'status'		=> '1'
				);
				
				// update previous data which are status = 1;
				$query 			= db_update('l_encrypted_uploaded_documents')->fields(array('status'=> '0'))->condition('document_type_code',$key)->condition('user_id',$user_id)->condition('status','1');
				$query_result 	= $query->execute();
				
				// insert data into table
				db_insert('l_encrypted_uploaded_documents')->fields($arr)->execute();
			}
		
			$message =	"Documents Successfully Uploaded";
			drupal_set_message($message);
		}
		
}

