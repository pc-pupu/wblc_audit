<?php 
//drupal_add_css(drupal_get_path('module', 'motor_transport') . '/css/sky-forms.css');
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');

function mtw_application_upload_docs($form, &$form_state, $id=''){ 
	global $base_root,$base_path,$user;
	
	drupal_add_js(drupal_get_path('module', 'motor_transport') . '/js/mtw_upload_custom.js');
	
	$user_id = $user->uid;
	
	$application_id	= encryption_decryption_fun('decrypt',$id);
	
	$enc_doc_array = array('TL'=> '','PD'=> '','AOA'=> '','MOA'=> '','FL'=> '','AC'=> '','PC'=> '','BB'	=> '','SC'	=> '','IC'	=> '','AP'=> '','ODSC'=> '','CR'=> '','WO'=> '','ORC'=> '');
	
	$pdficonlink = '<img title="View Documents" alt="View Documents" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/pdf.png"> &nbsp;View';
	
	// data from encrypted data table
	$query 			= db_select('l_encrypted_uploaded_documents','leud')->fields('leud',array())->condition('status','1')->condition('user_id',$user_id);
	$query_result 	= $query->execute();
	
	if($query_result->rowCount() > 0){
		foreach($query_result->fetchAll() as $data ){
			$enc_doc_array[$data->document_type_code]['content'] = 'view_documents/'.encryption_decryption_fun('encrypt',$data->id);
			$enc_doc_array[$data->document_type_code]['rowid'] = $data->id;
		}
	}
	
	$global_link = $base_root.$base_path.'sites/default/files/upload/';
	
	$trade_license_file			= '';
	$article_of_assoc_file		= '';
	$form_one_asses_ses_file	= '';
	$supp_asses_ses_file		= '';
	$mtw_address_proof_file		= '';
	$other_doc_file				= '';	
	
	$fetch_reg_mtw_upload_files	= db_select('l_documents_upload','ldu');
	$fetch_reg_mtw_upload_files->join('l_mtw_registration_master','lmrm','ldu.application_id=lmrm.id');
	$fetch_reg_mtw_upload_files->fields('ldu',array());
	$fetch_reg_mtw_upload_files->fields('lmrm',array('backlog_id','status'));
	$fetch_reg_mtw_upload_files->condition('ldu.application_id',$application_id,'=');
	$fetch_reg_mtw_upload_files->condition('ldu.user_id',$user_id,'=');
	$fetch_reg_mtw_upload_files->condition('ldu.act_id','3','=');
	$result = $fetch_reg_mtw_upload_files->execute();
		
	if( $result->rowCount() > 0 ){
		$obj3 = $result->fetchObject();
		$status 	= $obj3->status;
		$backlog_id	= $obj3->backlog_id;
		
		if($enc_doc_array['TL'] == ''){
			$enc_doc_array['TL']['content'] = ($obj3->trade_license_file != '')? $global_link.'trade_license/'.$obj3->trade_license_file : '';
		}
		if($enc_doc_array['AOA'] == ''){
			$enc_doc_array['AOA']['content'] = ($obj3->article_of_assoc_file != '')? $global_link.'article_of_assoc/'.$obj3->article_of_assoc_file : '';
		}
		if($enc_doc_array['PD'] == ''){
			$enc_doc_array['PD']['content'] = ($obj3->article_of_assoc_file != '')? $global_link.'article_of_assoc/'.$obj3->article_of_assoc_file : '';
		}
		if($enc_doc_array['BB'] == ''){
			$enc_doc_array['BB']['content'] = ($obj3->form_one_asses_ses_file != '')? $global_link.'blue_book/'.$obj3->form_one_asses_ses_file : '';
		}
		if($enc_doc_array['SC'] == ''){
			$enc_doc_array['SC']['content'] = ($obj3->form_one_asses_ses_file != '')? $global_link.'blue_book/'.$obj3->form_one_asses_ses_file : '';
		}
		if($enc_doc_array['IC'] == ''){
			$enc_doc_array['IC']['content'] = ($obj3->supp_asses_ses_file != '')? $global_link.'insurance/'.$obj3->supp_asses_ses_file : '';
		}
		if($enc_doc_array['AP'] == ''){
			$enc_doc_array['AP']['content'] = ($obj3->address_proof_file != '')? $global_link.'address_proof/'.$obj3->address_proof_file : '';
		}
		if($enc_doc_array['ODSC'] == ''){
			$enc_doc_array['ODSC']['content'] = ($obj3->other_doc_file != '')? $global_link.'OtherRelatedDocuments/'.$obj3->other_doc_file : '';
		}
	}else{
		$obj3 = array();
		$status 	= '';
		$backlog_id	= '';
		
		/*$enc_doc_array['TL']['content'] = '';
		$enc_doc_array['AOA']['content'] = '';
		$enc_doc_array['PD']['content'] = '';
		$enc_doc_array['BB']['content'] = '';
		$enc_doc_array['SC']['content'] = '';
		$enc_doc_array['IC']['content'] = '';
		$enc_doc_array['AP']['content'] = '';
		$enc_doc_array['ODSC']['content'] ='';*/
		
	}
	
	//echo '<pre>'; print_r($enc_doc_array); die;
	
	
		
	$form['mtw_upload_document']['header'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="panel panel-default panel-form">
						<div class="panel-heading"> Uoloaded Supportive Documents for Motor Transport Registration </div>
						<div class="panel-body">'
	);
	
	$form['mtw_upload_document']['mtw_application_id'] 		= array(
		'#title'		=>'Application ID',
		'#type' 		=> 'hidden',
		'#value'		=> !empty($application_id) ? $application_id : '',
		'#attributes' 	=> array('readonly'=>'readonly')
	);
	
	$form['mtw_upload_document']['backlog_id'] 			= array(
		'#title'		=>'Backlog Id',
		'#type' 		=> 'hidden',
		'#value'		=> !empty($backlog_id) ? $backlog_id :0,
		'#attributes' 	=> array('readonly'=>'readonly')
	);
	
	$form['mtw_upload_document']['mtw_act_id'] 			= array(
		'#title'		=>'Act Id',
		'#size'			=> t('20'),	
		'#type' 		=> 'hidden',
		'#default_value'	=> !empty($act_id) ? $act_id : '',
		'#attributes' 		=> array('readonly'=>'readonly')
	);
	
	$form['mtw_upload_document']['identification_number'] 	= array(
		'#title'		=>'Identification Number',
		'#size'			=> t('20'),	
		'#type' 		=> 'hidden',
		'#default_value'	=> !empty($identification_number) ? $identification_number : '',
		'#attributes' 		=> array('readonly'=>'readonly')
	);
	
	$tlf_class = '';
	$asf_class = '';
	$fsf_class = '';
	$sas_class = '';
	$odf_class = '';
	$adf_class = '';
	
	if ( $enc_doc_array['TL']['content'] != '' ) {
		$tlf = "checked"; 
	}else{
		$tlf = "";
	}
	if ( $enc_doc_array['AOA']['content'] != '' ) {
		$asf = "checked";
	}else if($enc_doc_array['PD']['content'] != ''){
		$asf = "checked";
	}else{
		$asf = "";
	}
	if ( $enc_doc_array['BB']['content'] != '' ) {
		$fsf = "checked";
	}else if ( $enc_doc_array['SC']['content'] != '' ) {
		$fsf = "checked";
	}else{
		$fsf = "";
	}
	if ( $enc_doc_array['IC']['content'] != '' ) {
		$sas = "checked";
	}else{
		$sas = "";
	}
	if ( $enc_doc_array['ODSC']['content'] != '' ) {
		$odf = "checked";
	}else{
		$odf = "";
	}
	if ( $enc_doc_array['AP']['content'] != '' ) {
		$adf = "checked";
	}else{
		$adf = "";
	}
	
	//echo '<pre>'; print_r($enc_doc_array); die;
	
	$readonly_trade_license = array( 'id' => 'mtw_upload_check_in_1_id', 'onclick' => 'mtw_upload_check_fun_1()', $tlf => $tlf, $tlf_class );
	$readonly_article_of_assoc = array( 'id' => 'mtw_upload_check_in_2_id', 'onclick' => 'mtw_upload_check_fun_2()', $asf => $asf );
	$readonly_form_one_asses_ses = array( 'id' => 'mtw_upload_check_in_3_id', 'onclick' => 'mtw_upload_check_fun_3()', $fsf => $fsf );
	$readonly_supp_asses_ses = array( 'id' => 'mtw_upload_check_in_4_id', 'onclick' => 'mtw_upload_check_fun_4()', $sas => $sas );
	$readonly_other_doc = array( 'id' => 'mtw_upload_check_in_5_id', 'onclick' => 'mtw_upload_check_fun_5()', $odf => $odf );
	$readonly_mtw_address_proof = array( 'id' => 'mtw_upload_check_in_6_id', 'onclick' => 'mtw_upload_check_fun_6()', $adf => $adf );
	
	
	if(!empty($application_id) && $status == 'B') {
		
		$query 		 =  db_select('l_mtw_remark_master', 'lbkrm')
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
								$tlf_class = "readonly_view";
								break;				
				case "article_of_assoc_file":
								$readonly_article_of_assoc 							= array('disabled' => 'disabled', 'class' => array());
								$asf_class = "readonly_view";
								break;		
				case "form_one_asses_ses_file":
								$readonly_form_one_asses_ses 						= array('disabled' => 'disabled', 'class' => array());
								$fsf_class = "readonly_view";
								break;				
				case "supp_asses_ses_file":
								$readonly_supp_asses_ses 							= array('disabled' => 'disabled', 'class' => array());
								$sas_class = "readonly_view";
								break;				
				case "other_doc_file":
								$readonly_other_doc 								= array('disabled' => 'disabled', 'class' => array());
								$odf_class = "readonly_view";
								break;
								
				case "mtw_address_proof_file":
								$readonly_mtw_address_proof 						= array('disabled' => 'disabled', 'class' => array());
								$adf_class = "readonly_view";
								break;
								
				default:
			}
		}
	}
	
	$mtw_fetch_reg_det		=	db_query("select identification_number, backlog_id, act_id, final_submit_status, status from l_mtw_registration_master where id=:application_id and user_id=:user_id", array( ':application_id' => $application_id, ':user_id' => $user_id ));
	
	if($mtw_fetch_reg_det->rowCount() > 0 ){ 
		foreach($mtw_fetch_reg_det as $obj2){ 
			$identification_number		= $obj2->identification_number;

			$act_id						= $obj2->act_id;
			$status						= $obj2->status;	
			$final_submit_status		= $obj2->final_submit_status;
			$backlog_id					= $obj2->backlog_id;
		}
	}
	
	/** Trade License **/																			
	$upload_value_1 = array( 'trade_license' => t('Trade License' ));
	/** Article of Association **/
	$upload_value_2 = array( 'article_of_assoc' => t('Article of Association / Partnership Deed'));
	/** Blue Book/ Smart Card Issued by Motor Vehicles **/
	$upload_value_3 = array( 'form_one_asses_ses' => t('Blue Book/ Smart Card Issued by Motor Vehicles'));
	/** Insurance Certificate of Motor Vehicles **/
	$upload_value_4 = array( 'supp_asses_ses' => t('Insurance Certificate of Motor Vehicles'));
	/**Documents in Support of Correctness of  Application **/
	$upload_value_5 = array( 'other_doc' => t('Other Documents in Support of Correctness of Particulars of Application'));
	/**Addresss Proof **/
	$upload_value_6 = array( 'mtw_address_proof_file' => t('Address Proof'));
	
	
	$form['mtw_upload_document']['enc_doc_array_hidden'] = array(
		'#type' => 'hidden',
		'#value' => $enc_doc_array
	);
			
	$viewLink_1	= '<span style="color:#bd0a0a">'.t('Not Yet Uploaded').'</span>';
	if($enc_doc_array['TL']['content'] !='' ){
		$viewLink_1	= l($pdficonlink,$enc_doc_array['TL']['content'], array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array(''))));
	}
	
	$form['mtw_upload_document']['upload_check_in_1'] = array(
		'#prefix'		=> '<div class="col-lg-6 col-md-6 '.$tlf_class.'"><label class="option">',
		'#suffix' 		=> '</label> '.$viewLink_1.'</div>',
		'#type' 		=> 'checkboxes',
		'#options' 		=> $upload_value_1,
		'#attributes'	=> $readonly_trade_license
	);
	
	$viewLink_2 = '<span style="color:#bd0a0a">'.t('Not Yet Uploaded').'</span>';
	if($enc_doc_array['AOA']['content'] != ''){
		$viewLink_2 = l($pdficonlink,$enc_doc_array['AOA']['content'], array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array(''))));
	}else if($enc_doc_array['PD']['content'] != ''){
		$viewLink_2 = l($pdficonlink,$enc_doc_array['PD']['content'], array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array(''))));
	}
	$form['mtw_upload_document']['upload_check_in_2'] 		= array(
		'#prefix'		=> '<div class="col-lg-6 col-md-6 '.$asf_class.'"><label class="option">',
		'#suffix' 		=> '</label> '.$viewLink_2.'</div>',
		'#type' 		=> 'checkboxes',
		'#options' 		=> $upload_value_2,
		'#attributes'	=> $readonly_article_of_assoc
	);
	
	$viewLink_3 = '<span style="color:#bd0a0a">'.t('Not Yet Uploaded').'</span>';
	if($enc_doc_array['BB']['content'] != ''){
		$viewLink_3 = l($pdficonlink,$enc_doc_array['BB']['content'], array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array(''))));
	}else if($enc_doc_array['SC']['content'] != ''){
		$viewLink_3 = l($pdficonlink,$enc_doc_array['SC']['content'], array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array(''))));
	}
	
	$form['mtw_upload_document']['upload_check_in_3'] 		= array(
		'#prefix'		=> '<div class="col-lg-6 col-md-6 '.$fsf_class.'"><label class="option">',
		'#suffix' 		=> '</label> '.$viewLink_3.'</div>',
		'#type' 		=> 'checkboxes',
		'#options' 		=> $upload_value_3,
		'#attributes'	=> $readonly_form_one_asses_ses
	); 
	
	$viewLink_4 = '<span style="color:#bd0a0a">'.t('Not Yet Uploaded').'</span>';
	if($enc_doc_array['IC']['content'] != ''){
		$viewLink_4 = l($pdficonlink,$enc_doc_array['IC']['content'], array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array(''))));
	}
	$form['mtw_upload_document']['upload_check_in_4'] 		= array(
		'#prefix'		=> '<div class="col-lg-6 col-md-6 '.$sas_class.'"><label class="option">',
		'#suffix' 		=> '</label> '.$viewLink_4.'</div>',
		'#type' 		=> 'checkboxes',
		'#options' 		=> $upload_value_4,
		'#attributes'	=> $readonly_supp_asses_ses
	);
	
	$viewLink_6 = '<span style="color:#bd0a0a">'.t('Not Yet Uploaded').'</span>'; 
	if($enc_doc_array['AP']['content'] != ''){
		$viewLink_6 = l($pdficonlink,$enc_doc_array['AP']['content'], array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array(''))));
	}
	$form['mtw_upload_document']['upload_check_in_6'] 		= array(
		'#prefix'		=> '<div class="col-lg-6 col-md-6 '.$adf_class.'"><label class="option">',
		'#suffix' 		=> '</label> '.$viewLink_6.'</div>',
		'#type' 		=> 'checkboxes',
		'#options' 		=> $upload_value_6,
		'#attributes'	=> $readonly_mtw_address_proof
	); 
	
	$viewLink_5 = '<span style="color:#bd0a0a">'.t('Not Yet Uploaded').'</span>';
	if($enc_doc_array['ODSC']['content'] != ''){
		$viewLink_5 = l($pdficonlink,$enc_doc_array['ODSC']['content'], array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array(''))));
	}
	$form['mtw_upload_document']['upload_check_in_5'] 		= array(
		'#prefix'		=> '<div class="col-lg-6 col-md-6 '.$odf_class.'"><label class="option">',
		'#suffix' 		=> '</label> '.$viewLink_5.'</div>',
		'#type' 		=> 'checkboxes',
		'#options' 		=> $upload_value_5,
		'#attributes'	=> $readonly_other_doc
	); 
	
	$form['mtw_upload_document']['footer'] = array(
		'#type' => 'markup',
		'#markup' => '</div></div>'
	);
	
	$form['mtw_upload_document']['header2'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="panel panel-default panel-form">
						<div class="panel-heading"> Uoload Documents Here </div>
						<div class="panel-body">'
	);
	
	$form['mtw_upload_document']['tlh'] = array(
		'#type' => 'markup',
		'#markup'  => '<div class="col-md-6" id="mtw_trade_license_div" style="display:none;">'
	);
	
	$form['mtw_upload_document']['trade_license'] = array(
		'#type' 				=> 'managed_file',
		'#title'				=> 'Upload Trade License',
		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
		'#upload_location'		=> 'public://upload/encrypt/',
		'#process' 				=> array('import_my_file_element_process'),
		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array(''))
	);
	
	$form['mtw_upload_document']['tlf'] = array(
		'#type' => 'markup',
		'#markup'  => '</div>'
	);
		
	$form['mtw_upload_document']['aoah'] = array(
		'#type' => 'markup',
		'#markup'  => '<div class="col-md-6" id="mtw_article_of_assoc_div" style="display:none;">'
	);
	
	$form['mtw_upload_document']['article_of_assoc'] = array(
		'#type' 				=> 'managed_file',
		'#title'				=> 'Upload Article of Association / Partnership Deed',
		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
		'#upload_location'		=> 'public://upload/encrypt/',
		'#process'				=> array('import_my_file_element_process'),
		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
		'#prefix' 				=> '<label class="input input-file">',
		'#suffix' 				=> '</label>',
	);
		
	$form['mtw_upload_document']['aoaf'] = array(
		'#type' => 'markup',
		'#markup'  => '</div>'
	);
		
	$form['mtw_upload_document']['bbh'] = array(
		'#type' => 'markup',
		'#markup'  => '<div class="col-md-6" id="mtw_form_one_asses_ses_div" style="display:none;">'
	);
	
	$form['mtw_upload_document']['form_one_asses_ses'] = array(
		'#type' 				=> 'managed_file',
		'#title'				=> 'Upload Blue Book/ Smart Card Issued by Motor Vehicles',
		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
		'#upload_location'		=> 'public://upload/encrypt/',
		'#process' 				=> array('import_my_file_element_process'),
		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array(''))
	);
	
	$form['mtw_upload_document']['bbf'] = array(
		'#type' => 'markup',
		'#markup'  => '</div>'
	);
	
	$form['mtw_upload_document']['ich'] = array(
		'#type' => 'markup',
		'#markup'  => '<div class="col-md-6" id="mtw_supp_asses_ses_div" style="display:none;">'
	);
	
	$form['mtw_upload_document']['supp_asses_ses'] = array(
		'#type' 				=> 'managed_file',
		'#title'				=> 'Upload Insurance Certificate of Motor Vehicles',
		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
		'#upload_location'		=> 'public://upload/encrypt/',
		'#process' 				=> array('import_my_file_element_process'),
		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array(''))
	);
		
	$form['mtw_upload_document']['icf'] = array(
		'#type' => 'markup',
		'#markup'  => '</div>'
	);
	
	$form['mtw_upload_document']['aph'] = array(
		'#type' => 'markup',
		'#markup'  => '<div class="col-md-6" id="mtw_address_proof_div" style="display:none;">'
	);
	
	$form['mtw_upload_document']['address_proof'] = array(
		'#type' 				=> 'managed_file',
		'#title'				=>'Address Proof',
		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
		'#upload_location'		=> 'public://upload/encrypt/',
		'#process' 				=> array('import_my_file_element_process'),
		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array(''))
	);
	
	$form['mtw_upload_document']['apf'] = array(
		'#type' => 'markup',
		'#markup'  => '</div>'
	);
		
	$form['mtw_upload_document']['odsch'] = array(
		'#type' => 'markup',
		'#markup'  => '<div class="col-md-6" id="mtw_other_doc_div" style="display:none;">'
	);
	
	$form['mtw_upload_document']['other_doc'] = array(
		'#type' 				=> 'managed_file',
		'#title'				=> 'Upload other documents in Support of Correctness of Particulars of Application',
		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
		'#upload_location'		=> 'public://upload/encrypt/',
		'#process' 				=> array('import_my_file_element_process'),
		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array(''))
	);
		
	$form['mtw_upload_document']['odscf'] = array(
		'#type' => 'markup',
		'#markup'  => '</div>'
	);
			
	$form['mtw_upload_document']['footer2'] = array(
		'#type' => 'markup',
		'#markup' =>'</div></div>'
	);
	
	
	$form['mtw_upload_document']['back_button'] = array (
		'#prefix' => '',  
		'#suffix' => '',  
		'#type'	  => 'markup',
		'#markup' => l(t('Back'),'mtw-registration-details/addmore/'.$id,array('attributes' => array('html'=>TRUE , 'title'=>'Click for Previous Tab', 'class' => array('btn btn-primary'))))
	);
	
	/*if(!empty($backlog_id) && $backlog_id!='' && $backlog_id!=0){
		$url = 'mtw-backlog-renewal/';
	}else{
		$url = 'mtw-registration-details/preview/';
	}*/
	$url = 'mtw-registration-details/preview/';
	
	$form['mtw_upload_document']['skip_button'] = array (
		'#prefix' 			=> '',  
		'#suffix' 			=> '',  
		'#type'				=>'markup',
		'#markup'				=>l(t('Next'),$url.$id,array('attributes' => array('html'=>TRUE ,'title'=>'Click for Next Tab', 'class' => array('btn btn-primary mleft-15'))))
	);
	
	
																				
	if( trim($status == 'B') || trim($final_submit_status) == '' || trim($final_submit_status) == 'S' ){
		$form['mtw_upload_document']['submit'] = array (
			'#prefix' 	=> '',  
			'#suffix' 	=> '',  
			'#type' 	=> 'submit',
			'#attributes'=> array('class' => array('btn btn-primary pull-right')),
			'#value' => t('Save & Continue')
		);
	}
	
	return $form;
}

function mtw_application_upload_docs_submit($form, &$form_state){ 
		global $base_root, $base_path, $user;
		$val = $form_state['values']; //echo '<pre>'; print_r($val); die;
		$user_id = $user->uid;
		$act_id	= $val['mtw_act_id'];
		$mtw_application_id	= $val['mtw_application_id'];
		$identification_number = $val['identification_number'];
		$backlog_id = $val['backlog_id'];
		
		/* Upload For Trade License */
		
		if(!empty($val['trade_license'])){
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
		
		/* Upload For Blue Book/ Smart Card Issued by Motor Vehicles */
		if(!empty($val['form_one_asses_ses'])){
			$blue_book		= file_load($val['form_one_asses_ses']);
			$blue_book_arr 	= explode("/", $blue_book->uri);
			$blue_book_fname	= time().'_'.$blue_book_arr[4]; 
			$filepathSave_bb 	= file_get_contents(escapefile_url($base_root.$base_path.'sites/default/files/upload/encrypt/'.$blue_book_arr[4]));
			$blue_book_content 	= base64_encode($filepathSave_bb);
			$insert_arr['BB']['fname'] = $blue_book_fname;
			$insert_arr['BB']['content'] = $blue_book_content;
		}
		
		/* Upload For Insurance Certificate of Motor Vehicles */
		if(!empty($val['supp_asses_ses'])){
			$insr_certi		= file_load($val['supp_asses_ses']);
			$insr_certi_arr 	= explode("/", $insr_certi->uri);
			$insr_certi_fname	= time().'_'.$insr_certi_arr[4]; 
			$filepathSave_ic 	= file_get_contents(escapefile_url($base_root.$base_path.'sites/default/files/upload/encrypt/'.$insr_certi_arr[4]));
			$insr_certi_content = base64_encode($filepathSave_ic);
			$insert_arr['IC']['fname'] = $insr_certi_fname;
			$insert_arr['IC']['content'] = $insr_certi_content;
		}
		
		/* Upload For Other Document in support of Correctness of Application */
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
		
		// store in db
		
		if(!empty($insert_arr)){
			foreach($insert_arr as $key => $row){ 
				$arr = array(
					'user_id' => $user_id,
					'act_id' => 3,
					'application_id' => $mtw_application_id,
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
		}
		// update in documents table
		
		$doc_arr = array(
			'user_id' => $user_id,
			'application_id' => $mtw_application_id,
			'act_id' => 3,
			'trade_license_fid' => ($val['enc_doc_array_hidden']['TL']['rowid'] !='') ? $val['enc_doc_array_hidden']['TL']['rowid'] : 0,
			'article_of_assoc_fid' => ($val['enc_doc_array_hidden']['AOA']['rowid'] !='' ) ? $val['enc_doc_array_hidden']['AOA']['rowid'] : 0,
			'partnership_deed_fid' => ($val['enc_doc_array_hidden']['PD']['rowid'] !='' ) ? $val['enc_doc_array_hidden']['PD']['rowid'] : 0,
			'form_one_asses_ses_fid' => ($val['enc_doc_array_hidden']['BB']['rowid'] != '' ) ? $val['enc_doc_array_hidden']['BB']['rowid'] : (($val['enc_doc_array_hidden']['SC']['rowid'] !='' ) ? $val['enc_doc_array_hidden']['SC']['rowid'] : 0),
			'supp_asses_ses_fid' => ($val['enc_doc_array_hidden']['IC']['rowid'] != '' ) ? $val['enc_doc_array_hidden']['IC']['rowid'] : 0,
			'other_doc_fid' => ($val['enc_doc_array_hidden']['ODSC']['rowid'] != '' ) ? $val['enc_doc_array_hidden']['ODSC']['rowid'] : 0,
			'address_proof_fid' => ($val['enc_doc_array_hidden']['AP']['rowid'] !='' ) ? $val['enc_doc_array_hidden']['AP']['rowid'] : 0,	
		);
		// check for update or insert
		$checkq = db_select('l_documents_upload','ldu')->fields('ldu',array('documents_id'))->condition('act_id',3)->condition('user_id',$user_id)->condition('application_id',$mtw_application_id);
		$checkr = $checkq->execute();
		
		if($checkr->rowCount() > 0){
			db_update('l_documents_upload')->fields($doc_arr)->condition('act_id',3)->condition('user_id',$user_id)->condition('application_id',$mtw_application_id)->execute();
		}else{
			db_insert('l_documents_upload')->fields($doc_arr)->execute();
		}
		
		$url = 'mtw-registration-details/preview/';
		/*if($backlog_id!='' || $backlog_id!=0){
			$url = 'mtw-backlog-renewal/';
		}else{
			$url = 'mtw-registration-details/preview/';
		}*/
		
		drupal_goto($url.encryption_decryption_fun('encrypt', $mtw_application_id));
}


