<?php
	
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
drupal_add_js(drupal_get_path('module', 'contractor') . '/js/contractor.js');
		
function renewal_doc_upload($form, &$form_state,$serial_number=''){
	global $base_root, $base_path, $user;
	
	$contractor_id = $user->uid;
	
	$action = 'decrypt';
	 $serial = encryption_decryption_fun($action, $serial_number);  //----miscellaneous module
	
	
	$particular		 		= 	db_select('l_particular_of_contract_labour', 'l_particular_con_labour');
  	$particular				->	fields('l_particular_con_labour',array('id'));
  	$particular				->	condition('l_particular_con_labour.serial_no_from_v',$serial,'=');
	$particular				->	condition('l_particular_con_labour.contractor_user_id',$contractor_id,'=');
	$particular				->	condition('l_particular_con_labour.act_id','1','=');
	$particulerid 			= 	$particular->execute()->fetchObject()->id; 
	
	if(!empty($particulerid))
		$license_id = db_query('SELECT id FROM l_contractor_license_application where contractor_particular_id ='.$particulerid)->fetchField();	
	
	$fetch_renewal_sttaus				=	fetch_all_data_contractor_license_apply_renweal($license_id,$contractor_id);//print_r($fetch_renewal_sttaus);die();
	$renewal_application_final_status	= 	$fetch_renewal_sttaus['renewal_application_final_status'];
	$renewal_appliaction_status			= 	$fetch_renewal_sttaus['renewal_appliaction_status'];	
	$renewal_id							= 	$fetch_renewal_sttaus['id'];
	$amount_of_renewal					=	$fetch_renewal_sttaus['license_reneal_fees'];
	$backlog_renewal_fees				=	$fetch_renewal_sttaus['backlog_renewal_fees'];
	$renewal_security_new				=	$fetch_renewal_sttaus['renewal_security_new'];
	$result_particular_labour 			= 	fetch_all_data_particular_of_contract_labour($serial,$contractor_id); 	
	$license_fees						=	$result_particular_labour['amount_of_licence'];
	$amount_of_security_deposit			=	$result_particular_labour['amount_of_security_deposit'];
	
	if($license_fees>$amount_of_renewal)
		   $renewal_deposite_fees1 = $license_fees;
		if($license_fees<$amount_of_renewal)
		   $renewal_deposite_fees1 = $amount_of_renewal;
		if($license_fees == $amount_of_renewal)
		   $renewal_deposite_fees1 = $amount_of_renewal;
		   
		 if($renewal_deposite_fees1>$backlog_renewal_fees) 
		  		$renewal_deposite_fees = $renewal_deposite_fees1;
		 if($renewal_deposite_fees1<$backlog_renewal_fees) 
		  		$renewal_deposite_fees = $backlog_renewal_fees;
		if($renewal_deposite_fees1==$backlog_renewal_fees) 
		  		$renewal_deposite_fees = $backlog_renewal_fees;
		   
		   
		   
		if($amount_of_security_deposit>$renewal_security_new)
		   $deposited_max_security_fees = $amount_of_security_deposit;
		if($amount_of_security_deposit<$renewal_security_new)
		   $deposited_max_security_fees = $renewal_security_new;
		if($amount_of_security_deposit == $renewal_security_new)
		   $deposited_max_security_fees = $amount_of_security_deposit;		
	
	$editablefield=fetch_editable_field_renewal($serial,$renewal_id); // print_r($editablefield); die;//---call from miscellaneous module
	
	if(!empty($editablefield)){ 
		$editablefield_array = explode(',', $editablefield['remark_field_title']);
		foreach($editablefield_array as $row){
			 if($row == 'work_order'){	
				$edit_work_order = 1;
			 }
			
			 if($row == 'frm_v'){	
				$edit_frm_v	= 1;
			 }
			
			 if($row == 'residential'){	
				$edit_residential_file	= 1;						
			 }
			  if($row == 'perivious'){	
				$edit_perivious_file	= 1;						
			 }
		}
	}	
	
	$contractor_id = $user->uid;
	$action = 'decrypt';
	$serial	= encryption_decryption_fun($action, $serial_number);	//---miscellaneous module	
	
	/*$form['renewal_doc_upload'] = array(
			'#markup' 		=> t('<div class="content"><div class="sky-form" style="width:100%;">'),
			'#type'			=>'markup' ,
			//'#prefix' 		=> '<fieldset>',
			//'#attributes' 	=> 	array('class'=>array('')),		
			);
			*/
	$form['renewal_doc_upload'] = array(
							'#title' 		=>	 t('<div class="content"><div class="sky-form" style="width:100%"><header><h3 align="center"> Upload Necessary Documents</h3></header>'),
							'#type' 		=> 	'fieldset',
							'#attributes' 	=> 	array('class'=>array('')),														
							);										
										
	$form['renewal_doc_upload']['view_1'] = array(
				'#prefix' 	   	=>  '<fieldset><legend>8.Uploaded Documents</legend>',
				'#suffix' 		=>  '</label></section>',
			 	'#type' 		=>  'markup',
												
		 );			
								
		$form['renewal_doc_upload']['renewal_deposite_fees'] 	= array(
							
								'#type' 		 => 'hidden',
								'#default_value' =>  encryption_decryption_fun('encrypt', $renewal_deposite_fees),
								'#attributes'	 => array('readonly'=>'readonly')
								);	
	$form['renewal_doc_upload']['deposited_max_security_fees'] 	= array(
							
								'#type' 		 => 'hidden',
								'#default_value' =>  encryption_decryption_fun('encrypt', $deposited_max_security_fees),
								'#attributes'	 => array('readonly'=>'readonly')
								);												
	$form['renewal_doc_upload']['serial'] 	= array(
								'#title'		 =>	'serial number',
								'#size'			 =>	t('20'),	
								'#type' 		 => 'hidden',
								'#default_value' => $serial,
								'#attributes'	 => array('readonly'=>'readonly')
								);
	$form['contractor_doc_upload']['editable_work_order'] = array(			 
							 	'#type' 		 => 'hidden',
							 	'#default_value' => !empty($edit_work_order) ? $edit_work_order	:''
								);
	$form['renewal_doc_upload']['editable_frm_v'] = array(			 
							 	'#type' 		 => 'hidden',
							 	'#default_value' => !empty($edit_frm_v) ? $edit_frm_v	:''
								);
	$form['renewal_doc_upload']['editable_residential_file'] = array(			 
							 	'#type' 		 => 'hidden',
							 	'#default_value' => !empty($edit_residential_file) ? $edit_residential_file	:''
								);
	$form['renewal_doc_upload']['edit_perivious_file'] = array(			 
							 	'#type' 		 => 'hidden',
							 	'#default_value' => !empty($edit_perivious_file) ? $edit_perivious_file	:''
								);
								
	$form['renewal_doc_upload']['license_id'] = array(			 
							 	'#type' 		 => 'hidden',
							 	'#default_value' => !empty($license_id) ? $license_id	:''
								);
								
								
	$markup_uploaded_documents = get_uploaded_document($serial, $contractor_id,$license_id);
	
	$form['renewal_doc_upload']['markup_uploaded_documents'] = array(
								'#type' 	=> 'markup',
								'#markup' 	=> $markup_uploaded_documents
								);			
	
	/*$status_query_result = application_status($serial,$contractor_id);	//-------call from miscellaneous module
	$status				 = $status_query_result['status'];
	$final_status		 = $status_query_result['final_status'];*/
	
	if($edit_work_order!='1'){
		$form['renewal_doc_upload']['work_order'] = array(
				'#type' 									=> 'managed_file',
				'#title'									=> 'A.Work Order',
				'#upload_validators' 						=> array('file_validate_extensions' => array('pdf')),
				'#upload_location' 							=> 'public://upload/contractor_license_work_order',
				'#process' 									=> array('import_my_file_element_process'),
				'#attributes' 								=> array('id' => '','autocomplete' => 'off','class'=>array(''),'file_validate_size' => array(50*1024*1024)),
				'#prefix' 									=> '<div class="row"><section class="col col-6"><label class="input input-file">',
				'#suffix' 									=> '</label></section></div>',
				'#required'									=> TRUE,															  
			);
		}else{
			$form['renewal_doc_upload']['work_order'] 	= array(
					'#type' 										=> 'managed_file',
					'#title'										=> 'A.Work Order',
					'#upload_validators' 							=> array('file_validate_extensions' => array('pdf')),
					'#upload_location' 								=> 'public://upload/contractor_license_work_order',
					'#process' 										=> array('import_my_file_element_process'),
					'#attributes' 									=> array('id' => '','autocomplete' => 'off','class'=>array(''),'file_validate_size' => array(50*1024*1024)),
					'#prefix' 										=> '<label class="input input-file">',
					'#suffix' 										=> '</label>',															  
				);
		}
	//	if($edit_frm_v!='1'){																
			$form['renewal_doc_upload']['from_v'] 	= array(
					'#type' 										=> 'managed_file',
					'#title'										=> 'B.Form V',
					'#upload_validators' 							=> array('file_validate_extensions' => array('pdf')),
					'#upload_location' 								=> 'public://upload/contractor_license_from_v',
					'#process' 										=> array('import_my_file_element_process'),
					'#attributes' 									=> array('id' => '','autocomplete' => 'off','class'=>array(''),'file_validate_size' => array(50*1024*1024)),
					'#prefix' 										=> '<label class="input input-file">',
					'#suffix' 										=> '</label>',
					//'#required'										=>	TRUE
				);
		/*}else{
			$form['renewal_doc_upload']['from_v'] 	= array(
					'#type' 									=> 'managed_file',
					'#title'									=> '2. Upload Form V',
					'#upload_validators' 						=> array('file_validate_extensions' => array('pdf')),
					'#upload_location' 							=> 'public://upload/contractor_license_from_v',
					'#process' 									=> array('import_my_file_element_process'),
					'#attributes' 								=> array('id' => '','autocomplete' => 'off','class'=>array(''),'file_validate_size' => array(50*1024*1024)),
					'#prefix' 									=> '<label class="input input-file">',
					'#suffix' 									=> '</label>',
				);
		}*/
		if($edit_residential_file!='1'){
			$form['renewal_doc_upload']['residential'] = array(
					'#type' 									 => 'managed_file',
					'#title'			 						 =>	'C.Address proof Certificate ',
					'#upload_validators'						 => array('file_validate_extensions' => array('pdf')),
					'#upload_location' 	 						 => 'public://upload/contractor_license_residential',
					'#process' 			 						 => array('import_my_file_element_process'),
					'#attributes' 		 						 => array('id' => '','autocomplete' => 'off','class'=>array(''),'file_validate_size' => array(50*1024*1024)),
					'#prefix' 									 => '<label class="input input-file">',
					'#suffix' 			 						 => '</label>',
					'#required'		     						 => TRUE
				);
		}else{
			$form['renewal_doc_upload']['residential'] 	 = array(
					'#type' 									 => 'managed_file',
					'#title'			 						 =>	'C.Address proof Certificate ',
					'#upload_validators' 						 => array('file_validate_extensions' => array('pdf')),
					'#upload_location' 	 						 => 'public://upload/contractor_license_residential',
					'#process' 			 						 => array('import_my_file_element_process'),
					'#attributes' 								 => array('id' => '','autocomplete' => 'off','class'=>array(''),'file_validate_size' => array(50*1024*1024)),
					'#prefix' 			 						 => '<label class="input input-file">',
					'#suffix' 			 						 => '</label>',
					);
		}
		if($edit_perivious_file!='1'){
					$form['renewal_doc_upload']['perivious_certificate'] 	 = array(
						'#type' 												  	=> 'managed_file',
						'#title'			 									    =>	'D.License Certificate or Last Renewal Certificate ',
						'#upload_validators' 									 	=> array('file_validate_extensions' => array('pdf')),
						'#upload_location' 	 										=> 'public://upload/upload_backlog_certificates_renewal',
						'#process' 													=> array('import_my_file_element_process'),
						'#attributes' 		 										=> array('id' => '','autocomplete' => 'off','class'=>array(''),'file_validate_size' => array(50*1024*1024)),
						'#prefix' 			 										=> '<label class="input input-file">',
						'#suffix' 													=> '</label>',
						'#required'													=> TRUE,
						);
		}else{
			$form['renewal_doc_upload']['perivious_certificate'] 	 = array(
				'#type' 												  	=> 'managed_file',
				'#title'			 									    =>	'D.License Certificate or Last Renewal Certificate ',
				'#upload_validators' 									 	=> array('file_validate_extensions' => array('pdf')),
				'#upload_location' 	 										=> 'public://upload/upload_backlog_certificates_renewal',
				'#process' 													=> array('import_my_file_element_process'),
				'#attributes' 		 										=> array('id' => '','autocomplete' => 'off','class'=>array(''),'file_validate_size' => array(50*1024*1024)),
				'#prefix' 			 										=> '<label class="input input-file">',
				'#suffix' 													=> '</label>',
			
				);
		}
		
			
		
		
		$form['renewal_doc_upload']['self_declaration'] = array(
								  '#type' 				=> 'checkbox',
								  '#title' 				=> t('<b>Declaration: I hereby declare that the details given above are correct to the best of my knowledge and belief.</b>'),
								  '#required' 			=> TRUE,
								  '#attributes'			=> array( 'id' => 'self_declaration_id', 'onclick' => 'checkCheckBox()'),
				  				  );
	
	
	
	$form['renewal_doc_upload']['view_2'] = array(
			
				'#suffix' 		=>  '</fieldset>',
			 	'#type' 		=>  'markup',
												
		 );
	
	if(($renewal_application_final_status=='F' && $renewal_appliaction_status=='B')||($renewal_application_final_status	=='' && $renewal_appliaction_status	=='')){
		$form['renewal_doc_upload']['submit'] = array(
									'#prefix' 		=> '<footer>',  
									'#suffix' 		=> '</footer></div></div>', 
									'#type' 		=> 'submit',
									'#attributes' 	=> array('class' => array('button'), 'style' => 'float:none;'),
									'#value' 		=> 'Final Submit'
									);
	}
			
	return $form;
}
function renewal_doc_upload_submit($form, &$form_state){
	
	global $base_root,$base_path,$user;
	
	$contractor_id = $user->uid;
	
	$val = $form_state['values'];	
	$serial	= $val['serial'];	
	$license_id	=	$val['license_id'];
	
	$status_query_result 	= application_status($serial,$contractor_id);    //-------call from miscellaneous module
	$status					= $status_query_result['status'];
	$final_status			= $status_query_result['final_status']; 
	$renewal_deposite_fees			=	encryption_decryption_fun('decrypt',$val['renewal_deposite_fees']); 
	$deposited_max_security_fees	=	encryption_decryption_fun('decrypt',$val['deposited_max_security_fees']); 
	
	$editable_work_order 		= $val['editable_work_order'];
	$editable_frm_v				= $val['editable_frm_v'];
	$editable_residential_file	= $val['editable_residential_file'];	
	$edit_perivious_file		= $val['edit_perivious_file'];
	
	$query_file 				= db_select('l_particular_of_contract_labour', 'lpcl');
 	$query_file					->fields('lpcl',array('work_order_file_id','frm_v_file_id','residential_file_id','id'))->condition('serial_no_from_v',$serial,'=')->condition('contractor_user_id',$contractor_id,'=')->condition('act_id','1','=');
	$result_file 				= 	$query_file->execute()->fetchAssoc();
	
			
	$work_order_file_id_edit	= $result_file['work_order_file_id'];
	$frm_v_file_id_edit			= $result_file['frm_v_file_id'];
	$residential_file_id_edit	= $result_file['residential_file_id'];	
	$particulerid				= $result_file['id'];
	
	$renewal_data=fetch_all_data_contractor_license_apply_renweal($license_id,$contractor_id);
	$upload_backlog_certificate				= $renewal_data['upload_backlog_certificate'];
	$recommand_date_expire					= $renewal_data['recommand_date_expire'];
	$backlog_renewal_date					= $renewal_data['backlog_renewal_date'];
	
	/*** Work Order Upload ***/
	if(!empty($recommand_date_expire)){
	if(strtotime($backlog_renewal_date) == strtotime($recommand_date_expire)){
	if($editable_work_order==1){
	 	$work_order_fid=$work_order_file_id_edit;
	}else{
		
			 $arr_1			= array();
			 $work_order 	= file_load($val['work_order']);
			 if( $work_order != "" ){
					$arr_1 				= explode("/", $work_order->uri);
					$work_order->status = FILE_STATUS_PERMANENT;
					file_save($work_order);
					$work_order_fid=$work_order->fid;
					 //$work_order_fid=$arr_1[4];
				}
	}
	
	echo $work_order_fid;echo '-----'.exit;
	//-------------------------from V upload-----------------------------------------
	/*if($editable_frm_v==1){	
	 $frm_v_file_id=$frm_v_file_id_edit;
	}else{
		
		 $arr_2			= array();
		 $from_v 		= file_load($val['from_v']);
	
		if($from_v != "" ){
				$arr_2 			= explode("/", $from_v->uri);
				$from_v->status = FILE_STATUS_PERMANENT;
				file_save($from_v);
			 	$frm_v_file_id=$from_v->fid;
			 
		}
	}*/
	
	$from_v 	= file_load($val['from_v']);
	 if(!empty($from_v)){
		 		$arr_2 					= 	explode("/", $from_v->uri);
				$from_v->status 		= 	FILE_STATUS_PERMANENT;
				file_save($from_v);
			 	$frm_v_file_id			=	$from_v->fid;
				
	 }else{		 
	 	 $frm_v_file_id				=	$frm_v_file_id_edit;
	 }

	
	//-------------------------residential upload-----------------------------------------
	if($editable_residential_file==1){ 
	$residential_fid=$residential_file_id_edit;
	}
	else{
		
		$file_name_residential 	= file_load($val['residential']);
		
	}
	if($file_name_residential){   
   
		   if ($file_name_residential!="")
		   {
				$uri						   =	trim($file_name_residential->uri); 
				$file_name_residential->status = 	FILE_STATUS_PERMANENT;
				file_save($file_name_residential);
				$residential_fid=$file_name_residential->fid;
				$residential_file_name=$file->filename;
		   
		  
		   }
	
	}
	
	
	
	
	//---------------------------------------previous certificates----------------------
	if($edit_perivious_file==1){
	 	$perivious_certificate_fid=$upload_backlog_certificate;
	}else{
		
			 $arr_1						= array();
			 $perivious_certificate 	= file_load($val['perivious_certificate']);
			 if( $perivious_certificate != "" ){
					$arr_1 				= explode("/", $perivious_certificate->uri);
					$perivious_certificate->status = FILE_STATUS_PERMANENT;
					file_save($perivious_certificate);
					$perivious_certificate_fid	=	$perivious_certificate->fid;
					 //$work_order_fid=$arr_1[4];
				}
	}
		
	/***UPLOAD END D0C **/				
				
	$get_application_id 	=	db_select('l_contractor_info_master', 'con_info_master');
	$get_application_id		->	fields('con_info_master',array('id','act_id','application_id'));
	$get_application_id		->	condition('con_info_master.id',$serial,'=');
	$get_application_id		->	condition('con_info_master.act_id','1','=');
	$result_application_id 	= $get_application_id->execute()->fetchAll(); 
	if(!empty($result_application_id)){
		foreach($result_application_id as $rows){
			$application_id = $rows->application_id;
		}
	}

	$query_particular 		=	db_select('l_particular_of_contract_labour','lpcl');
	$query_particular		->	fields('lpcl',array('id','contractor_max_no_of_labours_on_any_day','amount_of_licence'));
	$query_particular		->	condition('lpcl.serial_no_from_v',$serial,'=');
	$query_particular		->	condition('lpcl.contractor_user_id',$contractor_id,'=');
	$quer_particular_data	=	$query_particular->execute()->fetchAssoc();
	
	$contractor_particular_id =$quer_particular_data['id'] ; 
	$contractor_max_no_of_labours_on_any_day=$quer_particular_data['contractor_max_no_of_labours_on_any_day'];
	$amount_of_licence = $quer_particular_data['amount_of_licence'];
	
	

	$query = db_select('l_clra_registration_master','lcrm');
	$query->fields('lcrm',array('user_id','registration_number',));
	$query->condition('id',$application_id,'=');
	$query->condition('act_id','1','=');
	$result = $query->execute()->fetchAssoc();


	$registration_number=$result['registration_number']; 
	$user_id=$result['user_id']; 

	$created_date = date('m/d/Y h:i:s a', time());
	
	if(!empty($contractor_particular_id)){
			$license_id 			= db_query('SELECT id FROM l_contractor_license_application where contractor_particular_id ='.$contractor_particular_id)->fetchField();	
			$is_renewal_particular = db_query('SELECT is_renewal_particular FROM l_contractor_license_apply_renweal where lincense_id ='.$license_id)->fetchField();
	}
	if($is_renewal_particular == 1){
		$renewal_data			    = array('upload_backlog_certificate'=>$perivious_certificate_fid,'is_backlog_renewal'=>1,'renewal_application_final_status'=>'F','renewal_appliaction_status'=>'F','modification_date' => time(),'renewal_deposite_fees' =>$renewal_deposite_fees,'deposited_max_security_fees' =>$deposited_max_security_fees,'parent_id' =>$license_id,'is_flag' =>'L');
		//print_r($renewal_data);die();
		$query_upadate_renewal 		= 	db_update('l_contractor_license_apply_renweal');
 		$query_upadate_renewal		->	fields($renewal_data);
 		$query_upadate_renewal		->	condition('lincense_id',$license_id);
		$result = $query_upadate_renewal		->	execute();
			
		$renewal_id = db_query('SELECT id FROM l_contractor_license_apply_renweal where lincense_id ='.$license_id)->fetchField();	
	
						
	/*$linces_submit = array(							
							'user_id'							=>	$user_id,
							'act_id'							=>	'1', 
							'contractor_user_id'				=>	$contractor_id,
							'principal_reg_no'					=>	$registration_number,
							'final_status'						=>	'F',		//--------Finally Submitted this application
							'created_date'						=>	$created_date,
							'serial_no_from_v'					=>	$serial,
							'status'							=>	'I',
							'application_id'					=>	$application_id,
							'contractor_particular_id'			=>	$contractor_particular_id,
							'modification_date'					=>	$created_date,
							
					);
					
	
		$query_license_update =	db_update('l_contractor_license_application');
		$query_license_update->fields($linces_submit);
		$query_license_update->condition('contractor_user_id',trim($contractor_id));
		$query_license_update->condition('serial_no_from_v',trim($serial));
		$query_license_update->execute();	*/
			
		

	
	$apply_status 			= array('is_apply_status'=>1,'work_order_file_id'=>$work_order_fid,'frm_v_file_id'=>$frm_v_file_id,'residential_file_id'=>$residential_fid);
	print_r($apply_status);exit;
	$query_contract_labour 	= db_update('l_particular_of_contract_labour');
 	$query_contract_labour	->	fields($apply_status);
 	$query_contract_labour	->condition('contractor_user_id',trim($contractor_id));
	$query_contract_labour	->condition('serial_no_from_v',trim($serial));
 	$query_contract_labour	->execute();
	
	$linces_remark = array(							
						'remark_by'		 =>	$contractor_id,
						'remark_to'		 =>  $serial ,
						'remark_type'	 =>	'F',
						'remark_date'	 =>	time(),
						'remark_by_role' =>	'8'	,	
						'remark_text'	 =>  'Application has been fully submited',
						'particular_id'	 => $renewal_id,
						'flag'			=>	'R'			
						);
						
	db_insert('l_remark_license')->fields($linces_remark)->execute();
	 contractor_activity_tag_insert_backlog_reneeal($serial,'R',$renewal_id,'F');
	drupal_set_message('Application is applied successfully'); 
	$testing_status = 0;
	drupal_goto('renewal/preview-renewal/'.encryption_decryption_fun('encrypt', $serial).'/'.$testing_status);	
	}else{
		drupal_set_message('Please Fillup Previous tab'); 
	}
	}else{
		$recommand_dateexpire = date("dS M, Y",strtotime($recommand_date_expire));
		drupal_set_message('"<b>Date of expiry of the previous license</b>" provided by you is not matched with ALC recommended date('.$recommand_dateexpire.'). Please go to pervious tab and correct the same','error'); 
	}
	}
	if(empty($recommand_date_expire)){
	
	if($editable_work_order==1){
	 	$work_order_fid=$work_order_file_id_edit;
	}else{
		
			 $arr_1			= array();
			 $work_order 	= file_load($val['work_order']);
			 if( $work_order != "" ){
					$arr_1 				= explode("/", $work_order->uri);
					$work_order->status = FILE_STATUS_PERMANENT;
					file_save($work_order);
					$work_order_fid=$work_order->fid;
					 //$work_order_fid=$arr_1[4];
				}
	}
	
	
	//-------------------------from V upload-----------------------------------------
	/*if($editable_frm_v==1){	
	 $frm_v_file_id=$frm_v_file_id_edit;
	}else{
		
		 $arr_2			= array();
		 $from_v 		= file_load($val['from_v']);
	
		if($from_v != "" ){
				$arr_2 			= explode("/", $from_v->uri);
				$from_v->status = FILE_STATUS_PERMANENT;
				file_save($from_v);
			 	$frm_v_file_id=$from_v->fid;
			 
		}
	}*/
	
	$from_v 	= file_load($val['from_v']);
	 if(!empty($from_v)){
		 		$arr_2 					= 	explode("/", $from_v->uri);
				$from_v->status 		= 	FILE_STATUS_PERMANENT;
				file_save($from_v);
			 	$frm_v_file_id			=	$from_v->fid;
				
	 }else{		 
	 	 $frm_v_file_id				=	$frm_v_file_id_edit;
	 }

	
	//-------------------------residential upload-----------------------------------------
	if($editable_residential_file==1){ 
	$residential_fid=$residential_file_id_edit;
	}
	else{
		
		$file_name_residential 	= file_load($val['residential']);
		
	}
	if($file_name_residential){   
   
		   if ($file_name_residential!="")
		   {
				$uri						   =	trim($file_name_residential->uri); 
				$file_name_residential->status = 	FILE_STATUS_PERMANENT;
				file_save($file_name_residential);
				$residential_fid=$file_name_residential->fid;
				$residential_file_name=$file->filename;
		   
		  
		   }
	
	}
	
	
	
	
	//---------------------------------------previous certificates----------------------
	if($edit_perivious_file==1){
	 	$perivious_certificate_fid=$upload_backlog_certificate;
	}else{
		
			 $arr_1						= array();
			 $perivious_certificate 	= file_load($val['perivious_certificate']);
			 if( $perivious_certificate != "" ){
					$arr_1 				= explode("/", $perivious_certificate->uri);
					$perivious_certificate->status = FILE_STATUS_PERMANENT;
					file_save($perivious_certificate);
					$perivious_certificate_fid	=	$perivious_certificate->fid;
					 //$work_order_fid=$arr_1[4];
				}
	}
		
	/***UPLOAD END D0C **/				
				
	$get_application_id 	=	db_select('l_contractor_info_master', 'con_info_master');
	$get_application_id		->	fields('con_info_master',array('id','act_id','application_id'));
	$get_application_id		->	condition('con_info_master.id',$serial,'=');
	$get_application_id		->	condition('con_info_master.act_id','1','=');
	$result_application_id 	= $get_application_id->execute()->fetchAll(); 
	if(!empty($result_application_id)){
		foreach($result_application_id as $rows){
			$application_id = $rows->application_id;
		}
	}

	$query_particular 		=	db_select('l_particular_of_contract_labour','lpcl');
	$query_particular		->	fields('lpcl',array('id','contractor_max_no_of_labours_on_any_day','amount_of_licence'));
	$query_particular		->	condition('lpcl.serial_no_from_v',$serial,'=');
	$query_particular		->	condition('lpcl.contractor_user_id',$contractor_id,'=');
	$quer_particular_data	=	$query_particular->execute()->fetchAssoc();
	
	$contractor_particular_id =$quer_particular_data['id'] ; 
	$contractor_max_no_of_labours_on_any_day=$quer_particular_data['contractor_max_no_of_labours_on_any_day'];
	$amount_of_licence = $quer_particular_data['amount_of_licence'];
	
	

	$query = db_select('l_clra_registration_master','lcrm');
	$query->fields('lcrm',array('user_id','registration_number',));
	$query->condition('id',$application_id,'=');
	$query->condition('act_id','1','=');
	$result = $query->execute()->fetchAssoc();


	$registration_number=$result['registration_number']; 
	$user_id=$result['user_id']; 

	$created_date = date('m/d/Y h:i:s a', time());
	
	if(!empty($contractor_particular_id)){
			$license_id 			= db_query('SELECT id FROM l_contractor_license_application where contractor_particular_id ='.$contractor_particular_id)->fetchField();	
			$is_renewal_particular = db_query('SELECT is_renewal_particular FROM l_contractor_license_apply_renweal where lincense_id ='.$license_id)->fetchField();
	}
	if($is_renewal_particular == 1){
		$renewal_data			    = array('upload_backlog_certificate'=>$perivious_certificate_fid,'is_backlog_renewal'=>1,'renewal_application_final_status'=>'F','renewal_appliaction_status'=>'F','modification_date' => time(),'renewal_deposite_fees' =>$renewal_deposite_fees,'deposited_max_security_fees' =>$deposited_max_security_fees,'parent_id' =>$license_id,'is_flag' =>'L');
		//print_r($renewal_data);die();
		$query_upadate_renewal 		= 	db_update('l_contractor_license_apply_renweal');
 		$query_upadate_renewal		->	fields($renewal_data);
 		$query_upadate_renewal		->	condition('lincense_id',$license_id);
		$result = $query_upadate_renewal		->	execute();
			
		$renewal_id = db_query('SELECT id FROM l_contractor_license_apply_renweal where lincense_id ='.$license_id)->fetchField();	
	
						
	$linces_submit = array(							
							'user_id'							=>	$user_id,
							'act_id'							=>	'1', 
							'contractor_user_id'				=>	$contractor_id,
							'principal_reg_no'					=>	$registration_number,
							'final_status'						=>	'F',		//--------Finally Submitted this application
							'created_date'						=>	$created_date,
							'serial_no_from_v'					=>	$serial,
							'status'							=>	'I',
							'application_id'					=>	$application_id,
							'contractor_particular_id'			=>	$contractor_particular_id,
							'modification_date'					=>	$created_date,
							
					);
					
	
		$query_license_update =	db_update('l_contractor_license_application');
		$query_license_update->fields($linces_submit);
		$query_license_update->condition('contractor_user_id',trim($contractor_id));
		$query_license_update->condition('serial_no_from_v',trim($serial));
		$query_license_update->execute();	
			
		

	
	$apply_status 			= array('is_apply_status'=>1,'work_order_file_id'=>$work_order_fid,'frm_v_file_id'=>$frm_v_file_id,'residential_file_id'=>$residential_fid);
	
	$query_contract_labour 	= db_update('l_particular_of_contract_labour');
 	$query_contract_labour	->	fields($apply_status);
 	$query_contract_labour	->condition('contractor_user_id',trim($contractor_id));
	$query_contract_labour	->condition('serial_no_from_v',trim($serial));
 	$query_contract_labour	->execute();
	
	$linces_remark = array(							
						'remark_by'		 =>	$contractor_id,
						'remark_to'		 =>  $serial ,
						'remark_type'	 =>	'F',
						'remark_date'	 =>	time(),
						'remark_by_role' =>	'8'	,	
						'remark_text'	 =>  'Application has been fully submited',
						'particular_id'	 => $renewal_id,
						'flag'			=>	'R'			
						);
						
	db_insert('l_remark_license')->fields($linces_remark)->execute();
	 contractor_activity_tag_insert_backlog_reneeal($serial,'R',$renewal_id,'F');
	drupal_set_message('Application is applied successfully'); 
	$testing_status = 0;
	drupal_goto('renewal/preview-renewal/'.encryption_decryption_fun('encrypt', $serial).'/'.$testing_status);	
	}else{
		drupal_set_message('Please Fillup Previous tab'); 
	}
	
	}
	
}
	
	
function get_uploaded_document($serial, $contractor_id,$license_id){
	$file_details =	fetch_all_data_particular_of_contract_labour($serial,$contractor_id); // miscellanous
	
	$fetch_renewal_sttaus			=	fetch_all_data_contractor_license_apply_renweal($license_id,$contractor_id);//print_r($fetch_renewal_sttaus);die();
	$upload_backlog_certificate_id	= $fetch_renewal_sttaus['upload_backlog_certificate'];
	$renewal_id						= $fetch_renewal_sttaus['id'];
	
	if(!empty($file_details)){
		$work_order_file_id	= $file_details['work_order_file_id'];
		$frm_v_file_id	= $file_details['frm_v_file_id'];
		$residential_file_id = $file_details['residential_file_id'];
	}
	if(empty($work_order_file_id)&& empty($frm_v_file_id) && empty($residential_file_id) && empty($upload_backlog_certificate_id)){
		$output = '';
	}else{
		$upload_work_order_file_id 	= db_select('file_managed','fm');
		$upload_work_order_file_id->fields('fm',array());
		$upload_work_order_file_id->condition('fid',$work_order_file_id);
								
		$upload_work_order_file	= $upload_work_order_file_id->execute()->fetchAssoc();
		
		if(!empty($upload_work_order_file)){ 
			$url = explode('//',$upload_work_order_file['uri']);
   			$upload_work_order_file_url	 = $url[1];
			$upload_work_order_file_name = $upload_work_order_file['filename'];
		}
		
		$upload_frm_v_file_id = db_select('file_managed','fm');
		$upload_frm_v_file_id-> fields('fm',array());
		$upload_frm_v_file_id-> condition('fid',$frm_v_file_id);
						
		$upload_frm_v_file = $upload_frm_v_file_id-> execute()->fetchAssoc();
		
		if(!empty($upload_frm_v_file)){ 
			$url					= explode('//',$upload_frm_v_file['uri']);
   			$upload_frm_v_file_url	= $url[1];
			$upload_frm_v_file_name	= $upload_frm_v_file['filename'];
		}
		
		$upload_residential_file_id = db_select('file_managed','fm');
		$upload_residential_file_id-> fields('fm',array());
		$upload_residential_file_id-> condition('fid',$residential_file_id);
						
		$upload_residential_file = $upload_residential_file_id	-> execute()->fetchAssoc();
	
		if(!empty($upload_residential_file)){ 
			$url							= explode('//',$upload_residential_file['uri']);
   			$upload_residential_file_url	= $url[1];
			$upload_residential_file_name	= $upload_residential_file['filename'];
		}
		
		if(!empty($upload_backlog_certificate_id)){
				$perivious_certificate_id 	= db_select('file_managed','fm');
				$perivious_certificate_id->fields('fm',array());
				$perivious_certificate_id->condition('fid',$upload_backlog_certificate_id);
								
				$perivious_certificate_file	= $perivious_certificate_id->execute()->fetchAssoc();
		
				if(!empty($perivious_certificate_file)){ 
					$url = explode('//',$perivious_certificate_file['uri']);
					$perivious_certificate_file_url	 = $url[1];
					$perivious_certificate_file_name = $perivious_certificate_file['filename'];
				}
		}
		
	}
	//echo $perivious_certificate_file_name;die();	
	
	$particuler_id 				= 	db_select('l_particular_of_contract_labour', 'lpcl');
 	$particuler_id				->	fields('lpcl',array('id'))->condition('serial_no_from_v',$serial,'=')->condition('contractor_user_id',$contractor_id,'=')->condition('act_id','1','=');
	$particulerid 				= 	$particuler_id->execute()->fetchObject()->id;
	
	$editablefield = fetch_editable_field_renewal($serial,$renewal_id);	//---call from miscellaneous module	
	
	if(!empty($editablefield)){
		$work_order_file_verified		= 'correction_show';
		$frm_v_file_verified		= 'correction_show';
		$residential_file_verified		= 'correction_show';
		$perivious_file_verified		= 'correction_show';
				
	 	 $editablefield_array = explode(',', $editablefield['remark_field_title']);
		 foreach($editablefield_array as $row){
			 switch($row){
				 case "work_order":
							$work_order_file_verified		= 'verified_show';
							break;
				 case "frm_v":
							$frm_v_file_verified		= 'verified_show';
							break;
				case "residential":
							$residential_file_verified		= 'verified_show';
							break;
				case "perivious":
							$perivious_file_verified		= 'verified_show';
							break;
			 }
		 }
		
	}//die($perivious_file_verified);
			  
		  
		/*  if($row == 'work_order_file_id'){
			  $flag_order=1;
			  $work_order_file_verified = 'verified_show';
			
		  }if(empty($flag_order)){
			  $work_order_file_verified='correction_show';
		  }
		  if($row == 'frm_v_file_id'){
			  $flag_file=1;
			  $frm_v_file_verified = 'verified_show';
			  
		  }if(empty($flag_file)){
			  $frm_v_file_verified='correction_show';
		  }
		  if($row == 'residential_file_id'){
			  $flag_residential=1;
			  $residential_file_verified = 'verified_show';
			  
		  }if(empty($flag_residential)){
			  $residential_file_verified='correction_show';
		  }*/
	
	//echo $work_order_file_verified;echo 'hi';die();
	$output .= '<table width="100%" border="0" class="application_view">
				<tr>
				<td colspan="2"><center><b>
				Uploaded Documents</b></center>
				</td></tr>
				<tr><td width="40%">Work order</td>';
	 if(empty($upload_work_order_file_name)){
		 $output .='<td>N/A</td>';
	  }else{
		$output.= '<td><a href="'.$GLOBALS['base_url'].'/sites/default/files/'.$upload_work_order_file_url.'" target="_blank" class="ttt"><img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/pdf.png"></a><span class="'.$work_order_file_verified.'">&nbsp;</span></td>';
	  }
	  					
	  $output.='</tr><tr><td>Form V</td>';
	  
	  if(empty($upload_frm_v_file_name)){
		 $output .='<td>N/A</td>';
	  }else{
		 $output.=  '<td><a href="'.$GLOBALS['base_url'].'/sites/default/files/'.$upload_frm_v_file_url.'" target="_blank"><img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/pdf.png"></a><span class="'.$frm_v_file_verified.'">&nbsp;</span></td>';
	  }
						
	  $output.='</tr><tr><td>Address proof Certificate</td>';
	  
	  if(empty($upload_residential_file_name)){
		 $output .='<td>N/A</td>';
	  }else{
		 $output.='<td '.$class.'><a href="'.$GLOBALS['base_url'].'/sites/default/files/'.$upload_residential_file_url.'" target="_blank"><img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/pdf.png"></a><span class="'.$residential_file_verified.'">&nbsp;</span></td>';
	  }
	   $output.='</tr><tr><td>License Certificate or last Renewal Certificate</td>';
	  
	  if(empty($upload_backlog_certificate_id)){
		 $output .='<td>N/A</td>';
	  }else{
		 $output.='<td '.$class.'><a href="'.$GLOBALS['base_url'].'/sites/default/files/'.$perivious_certificate_file_url.'" target="_blank"><img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/pdf.png"></a><span class="'.$perivious_file_verified.'">&nbsp;</span></td>';
	  }
	  
	  $output.='</tr></table><br>';
	  
	 return $output;
}
	
?>