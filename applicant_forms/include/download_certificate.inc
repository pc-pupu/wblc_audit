<?php


function download_certificate($form, &$form_state, $act_id_en = 0, $application_id_en = 0){
	global $user;
	
	$applicant_user_id		= $user->uid;
	
	$act_id		= encryption_decryption_fun('decrypt', $act_id_en); 
	$application_id		= encryption_decryption_fun('decrypt', $application_id_en); 
	
	if($act_id == 1){
		$get_clra_application							=	db_select('l_clra_registration_master', 'clra'); 
		$get_clra_application							->	fields('clra',array('loc_e_subdivision','name_areatype','registration_number','clra_qr_code','status'));
		$get_clra_application							->	condition('clra.id',$application_id);
		$get_clra_application							->	condition('clra.user_id',$applicant_user_id);
		
		$get_clra_application_result 					= 	$get_clra_application->execute();  
		
		if($get_clra_application_result-> rowCount() > 0 ){
			
			$applicant_data 		= $get_clra_application_result->fetchAssoc();
			$registration_number	= $applicant_data['registration_number'];
			$qr_code				= $applicant_data['clra_qr_code']; 
			$status					= $applicant_data['status'];
		}
	
		$function_name = 'generation_clra_registration_number';
				
		$certificate_download_link = 'download-pdf-clra-registration/'.time().'/'.$application_id_en.'/'.encryption_decryption_fun('encrypt', $applicant_user_id);
	
	}else if($act_id == 2){
	
		$get_bocwa_application							=	db_select('l_bocwa_registration_master', 'lbrm');
		$get_bocwa_application							->	fields('lbrm',array('loc_e_subdivision','loc_e_areatype_code','registration_number','bocwa_qr_code','status'));
		$get_bocwa_application							->	condition('lbrm.id',$application_id);
		$get_bocwa_application							->	condition('lbrm.user_id',$applicant_user_id);
		
		$get_bocwa_application_result 					= 	$get_bocwa_application->execute();  
		
		if($get_bocwa_application_result-> rowCount() > 0 ){
			
			$applicant_data 		= $get_bocwa_application_result->fetchAssoc();
			$registration_number	= $applicant_data['registration_number'];
			$qr_code				= $applicant_data['bocwa_qr_code']; 
			$status					= $applicant_data['status'];
			
		}
		$function_name = 'generation_bocwa_registration_number';
		
		$certificate_download_link = 'generate-bocwa-certificate/'.encryption_decryption_fun('encrypt', $qr_code);
		
	}else if($act_id == 3){
	
		$get_mtw_application							=	db_select('l_mtw_registration_master', 'lmtw');
		$get_mtw_application							->	fields('lmtw',array('mtw_loc_dist','mtw_loc_subdivision','mtw_loc_areatype','mtw_loc_areatype_code','registration_number','mtw_qr_code','status'));
		$get_mtw_application							->	condition('lmtw.id',$application_id);
		$get_mtw_application							->	condition('lmtw.user_id',$applicant_user_id);
		
		$get_mtw_application_result 					= 	$get_mtw_application->execute();  
		
		if($get_mtw_application_result-> rowCount() > 0 ){
			
			$applicant_data 		= $get_mtw_application_result->fetchAssoc();
			$registration_number	= $applicant_data['registration_number'];
			$qr_code				= $applicant_data['mtw_qr_code']; 
			$status					= $applicant_data['status'];
			
		}
		$function_name = 'generation_mtw_registration_number';
		
		$certificate_download_link = 'mtw-certificate-generate/'.encryption_decryption_fun('encrypt', $qr_code);
		
	}else if($act_id == 4){
	
		$get_mtw_application							=	db_select('l_interstate_workman_master', 'liwm');
		$get_mtw_application							->	fields('liwm',array('loc_e_subdivision','loc_e_areatype_code','registration_number','iwm_qr_code','status','identification_number'));
		$get_mtw_application							->	condition('liwm.id',$application_id);
		$get_mtw_application							->	condition('liwm.user_id',$applicant_user_id);
		
		$get_mtw_application_result 					= 	$get_mtw_application->execute();  
		
		if($get_mtw_application_result-> rowCount() > 0 ){
			
			$applicant_data 		= $get_mtw_application_result->fetchAssoc();
			$registration_number	= $applicant_data['registration_number'];
			$qr_code				= $applicant_data['iwm_qr_code']; 
			$status					= $applicant_data['status'];
			
		}
		$function_name = 'generation_ismw_registration_number';
		
		$certificate_download_link = 'generate-ismw-certificate/'.encryption_decryption_fun('encrypt', $qr_code);
		
	}
	
	$form['applicant_user_id']		= array(
		'#type'			=> 'hidden',
		'#attributes' 	=> array('readonly' => 'readonly'),
		'#value' 		=> encryption_decryption_fun('encrypt', $user->uid),
	);	
	$form['application_id']			= array(
		'#type'		=> 'hidden',
		'#value' 	=> !empty($application_id) ? $application_id_en : '',
		'#attributes' => array('readonly' => 'readonly'),
	);
	
	
	if($status =='I'){
		$form['download_link']	= array(
			'#prefix'			=> '<div id="download_link_block">',
			'#markup' 			=> l(t('Click to Download Registration Certificate'),$certificate_download_link, array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('btn btn-primary pull-left acremark-btn form-submit')))),
			'#suffix' 			=> '</div><div class="clear"></div>',
		);
	}else{
		$form['generate_certificate']= array (
		  '#prefix' 		=> '<div id="generate_certificate_block">',
		  '#suffix' 		=> '</div>',
		  '#type' 			=> 'submit',
		  '#attributes' 	=> array('class' => array('btn btn-primary pull-left acremark-btn')),
		  '#value' 			=> 'Click to Download Registration Certificate',
		  '#submit'			=> array($function_name),
		);
	}
	
	
	
	
	return $form;
}

function generation_clra_registration_number($form, &$form_state){
	
	$val  				= $form_state['values']; //print_r($val); exit;
	$applicant_user_id	= encryption_decryption_fun('decrypt', $val['applicant_user_id']);
	$application_id		= encryption_decryption_fun('decrypt', $val['application_id']); 
	$act_id				= 1;
	
	
	/*** Get Applicant Block Code & Statuses ****/
	
	$get_clra_application							=	db_select('l_clra_registration_master', 'clra');
	$get_clra_application							->  leftJoin('l_customuser_relation_address', 'lcra','lcra.sub_div_code=clra.loc_e_subdivision ');
	$get_clra_application							->	fields('clra',array('loc_e_subdivision','name_areatype','registration_number','clra_qr_code'));
	$get_clra_application							->	fields('lcra',array('sub_div_code'));
	$get_clra_application							->	condition('clra.id',$application_id);
	$get_clra_application							->	condition('clra.user_id',$applicant_user_id);
	
	$get_clra_application_result 					= 	$get_clra_application->execute();  
	
	if($get_clra_application_result-> rowCount() > 0 ){	
		
		$applicant_data_clra 	= $get_clra_application_result->fetchAssoc(); 
		$subdivision_code 		= $applicant_data_clra['loc_e_subdivision'];
		$area					= trim($row->name_areatype);
		$clra_qr_code			= $row->clra_qr_code;
		$registration_number	= $row->registration_number;
		
		
		$applicant_data_clra 						= 	$get_clra_application_result->fetchAssoc();
		$subdivision_code							=   $applicant_data_clra['loc_e_subdivision']; 
		$area_code									=   $applicant_data_clra['name_areatype'];
		$clra_registration_number					=   $applicant_data_clra['registration_number'];
		$clra_qr_code								=   $applicant_data_clra['clra_qr_code']; 
		$alc_subdiv_code							=   $applicant_data_clra['sub_div_code']; 
				
	}
	
	if (empty($clra_registration_number) && empty($clra_qr_code)){
		$clra_qr_code = 'CLRA-REG'.$application_id.'A1U'.$applicant_user_id.'T'.time();
		
		
		/****** Registration number create ******/
		
		$reg_date = date('m/d/Y h:i:s a', time());			
					
		$query_Is_reg_code 		 		= 	db_select('l_clra_registration_master', 'lcrm');
		$query_Is_reg_code				->	fields('lcrm',array('registration_number'));
		$query_Is_reg_code				->	condition('lcrm.registration_number','NULL','!=')
										->	orderBy('lcrm.id', 'DESC')
										->	range(0,1);
		$query_Is_reg_code_result 		= 	$query_Is_reg_code->execute()->fetchObject()->registration_number;	
				
		$getShortName 				    =   custom_user_short_name_fun($subdivision_code);  // ---------come from custom_user module  
		$reg_code						=	get_registration_code($area_code);//-------------miscellaneous module
		$getBlockCodeRes 				=   substr($reg_code, -2);
					
		if(empty($query_Is_reg_code_result)){
			$reg		=	$getShortName.$getBlockCodeRes.'/'.'CLR'.'/'.'000001';
		}else{		
			
			$reg_query		=	db_query("select  max (NULLIF(substr(registration_number,11,6),'') :: integer) as serial_num from l_clra_registration_master where loc_e_subdivision=".$subdivision_code."");
			$x				=	$reg_query->fetchAssoc();
			
			if(empty($reg_code)){
						$reg		=	$getShortName.$getBlockCodeRes.'/'.'CLR'.'/'.'000001';
	
			}else{
					  $reg_code_next			= 	$reg_code+1;
					  $reg_firts				=	$getShortName.$getBlockCodeRes.'/'.'CLR'.'/';
					  $reg_second				=	str_pad($reg_code_next, 6, "0", STR_PAD_LEFT);
					  $reg						=	$reg_firts.$reg_second;
			}
		}
		
		
		
		$frm_v = array('is_from_v'=>1);
		$frm_v_gen = db_update('l_contractor_info_master');
		$frm_v_gen->fields($frm_v);
		$frm_v_gen->condition('application_id',$application_id);
		$frm_v_gen->condition('user_id',$applicant_user_id);
		$frm_v_gen->execute();
		
		$reg = array('registration_number'=>$reg,'registration_date'=>$reg_date,'clra_qr_code' =>$clra_qr_code,'status'=>'I');
		$reg_update =  db_update('l_clra_registration_master');
		$reg_update->fields($reg);
		$reg_update->condition('user_id',$applicant_user_id);
		$reg_update->condition('id',$application_id);
		$reg_update->execute();
		
		
	}
	
	$remark_details  =  array(
		'remarks'   		=> 'Certificates is Deemed Approved',
		'remark_by'   		=> $applicant_user_id, 
		'remark_to' 		=> $applicant_user_id,
		'application_id' 	=> $application_id,
		'remark_date'  		=> time(),
		'remark_type' 		=> 'I',
		'remark_by_name'    => '',
		'remark_field_text' =>	'',
		'remark_by_role' 	=> '8',
		'hrms_employee_id'  => 0
	);
	
	db_insert('l_remarks_master')->fields($remark_details)->execute();
	
}

function generation_bocwa_registration_number($form, &$form_state){
	
	 $val  					= $form_state['values']; //print_r($val); exit;
	 $applicant_user_id		= encryption_decryption_fun('decrypt', $val['applicant_user_id']);
	 $application_id		= encryption_decryption_fun('decrypt', $val['application_id']);
	 $act_id				= 2;
	 
	 
	 /*** Get Applicant Block Code & Statuses & ALC Details ****/
	
	$get_bocwa_application							=	db_select('l_bocwa_registration_master', 'lbrm');
	$get_bocwa_application							->  leftJoin('l_customuser_relation_address', 'lcra','lcra.sub_div_code=lbrm.loc_e_subdivision ');
	$get_bocwa_application							->	fields('lbrm',array('loc_e_subdivision','loc_e_areatype_code','registration_number','bocwa_qr_code'));
	$get_bocwa_application							->	fields('lcra',array('sub_div_code'));
	$get_bocwa_application							->	condition('lbrm.id',$application_id);
	$get_bocwa_application							->	condition('lbrm.user_id',$applicant_user_id);
	
	$get_bocwa_application_result 					= 	$get_bocwa_application->execute();  
	
	if($get_bocwa_application_result-> rowCount() > 0 ){
		
		$applicant_data 							= 	$get_bocwa_application_result->fetchAssoc();
		$subdivision_code							=   $applicant_data['loc_e_subdivision'];
		$area_code									=   $applicant_data['loc_e_areatype_code'];
		$bocwa_registration_number					=   $applicant_data['registration_number'];
		$bocwa_qr_code								=   $applicant_data['bocwa_qr_code']; 
		$alc_subdiv_code							=   $applicant_data['sub_div_code']; 

	}
	
	
	if (empty($bocwa_registration_number) && empty($bocwa_qr_code)){
		
		/*** GENERATION OF QR CODE ****/
		
		 $generated_qr_code					= 'BOCWA-REG'.$application_id.'A2U'.$applicant_user_id.'T'.time();
	
		/*** GENERATION OF BOCWA REGISTRATION NUMBER ***/
		
		
		$getShortNameSubdivision 			=   custom_user_short_name_fun($subdivision_code);  // ---------come from custom_user module  
		$getRegistrarCode					=	get_registration_code($area_code);              //-------------miscellaneous module
		$registrarCode 						=   substr($getRegistrarCode, -2);
		
		$reg_query	=	db_query("select max (NULLIF(substr(registration_number,11,6), '') :: integer) as serial_num  from l_bocwa_registration_master where loc_e_subdivision='".$alc_subdiv_code."'");
			
					
		$reg_query_result					=	$reg_query->fetchAssoc();
		
		$reg_code							=	(int)$reg_query_result['serial_num'];
		
		if(empty($reg_code)){ 
			
			$registration_number	=	$getShortNameSubdivision.$registrarCode.'/'.'BCR'.'/'.'000001';
	
	}else{
			$reg_code_next			= 	$reg_code+1;
			$reg_first				=	$getShortNameSubdivision.$registrarCode.'/'.'BCR'.'/';
			$reg_second				=	str_pad($reg_code_next, 6, "0", STR_PAD_LEFT);
			$registration_number	=	$reg_first.$reg_second;
		}
				
		/*** Update Status of BOCWA Master Table ***/ 
		
		$update_status  					=  db_update('l_bocwa_registration_master');
		$update_status->fields(array('bocwa_qr_code' => $generated_qr_code, 'registration_number' =>	$registration_number, 'registration_date' => date("Y-m-d"),'status'=>'I'));
		$update_status->condition('id',$application_id);
		$update_status->condition('act_id',$act_id);
		$update_status->condition('user_id',$applicant_user_id);
		$update_status->execute();
				
	}
	 
	 
	$fieldsApplInfo2  =  array(
		'remark_text'   		=> 'Certificates is Deemed Approved', 
		'remark_by'   			=> $applicant_user_id, 
		'remark_to' 			=> $applicant_user_id,
		'application_id' 		=> $application_id,
		'remark_date'  			=> time(),
		'remark_type' 			=> 'I',
		'remark_by_name'    	=> '',
		'remark_field_title' 	=> '',
		'remark_by_role' 		=> 8,
		'call_time_applicant'	=> 0,
		'emp_id'				=> 0		
		);

	db_insert('l_bocwa_remark_master')->fields($fieldsApplInfo2)->execute();
	
	
}

function generation_mtw_registration_number($form, &$form_state){
	
	 $val  					= $form_state['values']; //print_r($val); exit;
	 $applicant_user_id		= encryption_decryption_fun('decrypt', $val['applicant_user_id']);
	 $application_id		= encryption_decryption_fun('decrypt', $val['application_id']);
	 $act_id				= 3;
	 
	
	 
	 
	 /*** Get Applicant Block Code & Statuses & ALC Details ****/
	
	$get_mtw_application =	db_select('l_mtw_registration_master', 'lbrm');
	$get_mtw_application->  leftJoin('l_customuser_relation_address', 'lcra','lcra.sub_div_code=lbrm.mtw_loc_subdivision ');
	$get_mtw_application->	fields('lbrm',array('mtw_loc_subdivision','mtw_loc_areatype_code','registration_number','mtw_qr_code'));
	$get_mtw_application->	fields('lcra',array('sub_div_code'));
	$get_mtw_application->	condition('lbrm.id',$application_id);
	$get_mtw_application->	condition('lbrm.user_id',$applicant_user_id);
	
	$get_mtw_application_result = 	$get_mtw_application->execute();
	
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
			
			$update_status =  db_update('l_mtw_registration_master');
			$update_status->fields(array('mtw_qr_code' => $generated_qr_code, 'registration_number' =>	$registration_number, 'registration_date' => date("Y-m-d"), 'certificate_serial' => $certificate_serial_no,'expiry_date'=>date("Y").'-12-31','status'=>'I'));
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
					'application_id'	 	=>  $application_id,
					'application_date'		=> 	$application_date, 		// Applicant apply date
					'renewal_ref_number' 	=>  'MTW-REG-'.$application_id.'-'.time(),
					'renewal_date'			=>	NULL,
					'expiry_date' 			=>  date("Y").'-12-31',
					'status'				=>	'I'
				); 
				//print_r($insertNewalPrvData);
				db_update('l_mtw_registration_renewal')->fields($upNewalPrvData)->condition('application_id',$application_id)->condition('status','V')->execute();
				//db_insert('l_mtw_registration_renewal')->fields($insertNewalPrvData)->execute();
			}
			
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
				
				$alc_user_id 			= $result['remark_by'];			
				$getEmployeeid 			= db_select('l_custom_user_detail','cd')->fields('cd',array('employee_id','fullname'))->condition('usr_id',$alc_user_id)->execute()->fetchAssoc();
				$alc_full_name			= $getEmployeeid['fullname'];	
				$employee_id			= $getEmployeeid['employee_id'];
				
				$fieldsApplInfo2  =  array(
								'remark_text'   		=> 'Congratulations! Certificate is issued by the Registering Authority. You can download it from the dashboard.',  
								'remark_by'   			=> $alc_user_id, 
								'remark_to' 			=> $applicant_user_id,
								'application_id' 		=> $application_id,
								'remark_date'  			=> $thirtydays,
								'remark_type' 			=> 'I',
								'remark_by_name'    	=> $alc_full_name,
								'remark_field_title' 	=> $result['remark_field_text'],
								'remark_by_role' 		=> 4,
								'employee_id'			=> $employee_id,
								'deemed_on'				=> date('Y-m-d')
							);
	
				db_insert('l_mtw_remark_master')->fields($fieldsApplInfo2)->execute();
	
	}
}

function generation_ismw_registration_number($form, &$form_state){
	
	 $val  					= $form_state['values'];
	 $applicant_user_id		= encryption_decryption_fun('decrypt',$val['applicant_user_id']);
	 $application_id		= encryption_decryption_fun('decrypt',$val['application_id']);
	 $act_id				= 4;
	 
	
	 
	 
	 /*** Get Applicant Block Code & Statuses & ALC Details ****/
	
	$ismw_application							=	db_select('l_interstate_workman_master', 'lbrm');
	$ismw_application							->  leftJoin('l_customuser_relation_address', 'lcra','lcra.sub_div_code=lbrm.loc_e_subdivision ');
	$ismw_application							->	fields('lbrm',array('loc_e_subdivision','loc_e_areatype_code','registration_number','iwm_qr_code','identification_number'));
	$ismw_application							->	fields('lcra',array('sub_div_code'));
	$ismw_application							->	condition('lbrm.id',$application_id);
	$ismw_application							->	condition('lbrm.user_id',$applicant_user_id);
	
	$ismw_application_result 					= 	$ismw_application->execute();
	
	if($ismw_application_result-> rowCount() > 0 ){
		
		$applicant_data 							= 	$ismw_application_result->fetchAssoc();
		$subdivision_code							=   $applicant_data['loc_e_subdivision'];
		$area_code									=   $applicant_data['loc_e_areatype_code'];
		$ismw_registration_number					=   $applicant_data['registration_number'];
		$ismw_qr_code								=   $applicant_data['iwm_qr_code']; 
		$alc_subdiv_code							=   $applicant_data['sub_div_code'];
		$identification_number						=   $applicant_data['identification_number']; 
		
		
	}
	
	if (empty($ismw_registration_number) && empty($ismw_qr_code)){
		
		
		
			/*** GENERATION OF QR CODE ****/
			
			
			 $generated_qr_code					= 'ISMW-REG'.$application_id.'A4U'.$applicant_user_id.'T'.time();
		
		
			/*** GENERATION OF ISMW REGISTRATION NUMBER ***/
			
			
			$getShortNameSubdivision 			=   custom_user_short_name_fun($subdivision_code);  // ---------come from custom_user module  
			$getRegistrarCode					=	get_registration_code($area_code);              //-------------miscellaneous module
			$registrarCode 						=   substr($getRegistrarCode, -2);
			
			$reg_query							=	db_query("select  max (NULLIF(substr(registration_number,11,6),'') :: integer) as serial_num  from l_interstate_workman_master where loc_e_subdivision='".$alc_subdiv_code."'");
			
			$reg_query_result					=	$reg_query->fetchAssoc();
			
			$reg_code							=	$reg_query_result['serial_num'];
			
			
			
			
			if(empty($reg_code)){ 
				
						$registration_number	=	$getShortNameSubdivision.$registrarCode.'/'.'IMR'.'/'.'000001';

			}else{
					  	$reg_code_next			= 	$reg_code+1; 
					  	$reg_first				=	$getShortNameSubdivision.$registrarCode.'/'.'IMR'.'/';
					  	$reg_second				=	str_pad($reg_code_next, 6, "0", STR_PAD_LEFT);
					  	$registration_number	=	$reg_first.$reg_second;
			}
			
			
					
			/*** Update Status of ISMW Master Table ***/ 
			
			$update_status  					=  db_update('l_interstate_workman_master');
			$update_status->fields(array('iwm_qr_code' => $generated_qr_code, 'registration_number' =>	$registration_number, 'registration_date' => date("Y-m-d"),'status' => 'I'));
			$update_status->condition('id',$application_id);
			$update_status->condition('act_id',$act_id);
			$update_status->condition('user_id',$applicant_user_id);
			$update_status->execute();
			
			$update_tag  					=  db_update('l_interstate_tag_master');
			$update_tag->fields(array('registration_number' => $registration_number, 'issued_date' => date("Y-m-d")));
			$update_tag->condition('application_id',$application_id);
			$update_tag->condition('pe_user_id',$applicant_user_id);
			$update_tag->condition('identification_number',$identification_number);
			$update_tag->condition('act_id', 4);
			$update_tag->execute();
			
						
			$fieldsApplInfo2  =  array(
				'remark_text'   		=> 'Certificates is Deemed Approved', 
				'remark_by'   			=> $applicant_user_id, 
				'remark_to' 			=> $applicant_user_id,
				'application_id' 		=> $application_id,
				'remark_date'  			=> time(),
				'remark_type' 			=> 'I',
				'remark_by_name'    	=> '',
				'remark_field_text' 	=> '',
				'remark_by_role' 		=> 8,
				'call_time_applicant'	=> 0,
				//'employee_id'			=> 0		
			);
		
			db_insert('l_interstate_remark_master')->fields($fieldsApplInfo2)->execute();
	
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
			
			$update_status  			=  db_update('l_contractor_license_application');
			$update_status->fields(array('licence_qr_code' => $licence_qr_code, 'contractor_license_number' =>	$license_number, 'license_date' => date("Y-m-d"),'next_renweal_date' => $renewalDate ,'modification_date'=> date("Y-m-d"), 'remark_by_userid'=> $alcUserId , 'block_next_renwal_date' =>$renewalDate ));
			$update_status->condition('id',$license_id);
			$update_status->condition('serial_no_from_v',$serialFormV);
			$update_status->condition('contractor_particular_id',$particular_id);
			$update_status->condition('contractor_user_id',$contractor_user_id);
			$update_status->execute();
			
			/*** Update Contractor Master Table ***/ 
			
			$update_contractor  	=  	db_update('l_contractor_info_master');
			$update_contractor->fields(array('contractor_user_id'=> $contractor_user_id));
			$update_contractor->condition('id',$serialFormV);
			$update_contractor->execute();
			
			$message								= drupal_set_message_custom(t('License Certificate has been successfully generated. Please Sign and Upload Certificate to complete this process'));
 			$form_state['values']['hidden_message'] = $message;
			$form_state['rebuild']					= TRUE;

		}
	}
}
