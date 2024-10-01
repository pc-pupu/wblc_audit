<?php 

function deemed_download_certificate($act_id_en = '', $application_id_en = '', $application_userid_en = ''){ 
	
	/*global $user;
	
	$applicant_user_id  = encryption_decryption_fun('decrypt', $application_userid_en); 
	$act_id				= encryption_decryption_fun('decrypt', $act_id_en); 
	$application_id		= encryption_decryption_fun('decrypt', $application_id_en);
	
	
	
	if($act_id == 1){ 
	
		$getLatestRemark  = check_final_submit_data(encryption_decryption_fun('encrypt', 1), encryption_decryption_fun('encrypt', $application_id));//miscellaneous.module	
		
													  
		if(!empty($getLatestRemark) && $getLatestRemark['remark_type']=='S'){
			
			$final_submit_date 	= $getLatestRemark['remark_date']; //echo date('Y-m-d h:i:s',$getLatestRemark['remark_date']).'<br/>';
			$today 				= time();
			$thirtydays 		= strtotime('+31 days',$final_submit_date);
			$dateThirtydays 	= date('Y-m-d h:i:s',$thirtydays); echo date('Y-m-d h:i:s',$thirtydays).'<br/>';
			
			
			$get_clra_application	=	db_select('l_clra_registration_master', 'clra');
			$get_clra_application	->  leftJoin('l_customuser_relation_address', 'lcra','lcra.sub_div_code=clra.loc_e_subdivision ');
			$get_clra_application	->	fields('clra',array('loc_e_subdivision','name_areatype','registration_number','clra_qr_code'));
			$get_clra_application	->	fields('lcra',array('sub_div_code'));
			$get_clra_application	->	condition('clra.id',$application_id);
			$get_clra_application	->	condition('clra.user_id',$applicant_user_id);

			$get_clra_application_result = 	$get_clra_application->execute();  

			if($get_clra_application_result-> rowCount() > 0 ){	
		
				$applicant_data_clra 	= $get_clra_application_result->fetchAssoc(); 
				$subdivision_code 		= $applicant_data_clra['loc_e_subdivision'];
				$area					= trim($row->name_areatype);
				$clra_qr_code			= $row->clra_qr_code;
				$registration_number	= $row->registration_number;


				$applicant_data_clra 		= 	$get_clra_application_result->fetchAssoc();
				$subdivision_code			=   $applicant_data_clra['loc_e_subdivision']; 
				$area_code					=   $applicant_data_clra['name_areatype'];
				$clra_registration_number	=   $applicant_data_clra['registration_number'];
				$clra_qr_code				=   $applicant_data_clra['clra_qr_code']; 
				$alc_subdiv_code			=   $applicant_data_clra['sub_div_code']; 

			}
	
			if (empty($clra_registration_number) && empty($clra_qr_code)){
				$clra_qr_code = 'CLRA-REG'.$application_id.'A1U'.$applicant_user_id.'T'.time();


				/****** Registration number create ******/

				/*$reg_date = date('m/d/Y h:i:s a', time());			

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

				$reg = array('registration_number'=>$reg,'registration_date'=>$dateThirtydays,'clra_qr_code' =>$clra_qr_code,'status'=>'I','deemed'=>'Y');
				$reg_update =  db_update('l_clra_registration_master');
				$reg_update->fields($reg);
				$reg_update->condition('user_id',$applicant_user_id);
				$reg_update->condition('id',$application_id);
				$reg_update->execute();


			}
			
			$query = db_select('l_remarks_master','rm')->fields('rm',array())->condition('application_id',$application_id)->condition('remark_type','V')->range(0,1)->orderBy('remark_id','desc');
			$result = $query->execute()->fetchAssoc();
			
			
			$alcuserId = $result['remark_by'];
			
			$getEmployeeid = db_select('l_custom_user_detail','cd')->fields('cd',array('employee_id','fullname'))->condition('usr_id',$alcuserId)->execute()->fetchAssoc();
			
			$employeeId = $getEmployeeid['employee_id'];
			
			
			
			
	
			$remark_details  =  array(
				'remarks'   		=> 'Deemed Approved',
				'remark_by'   		=> $alcuserId, 
				'remark_to' 		=> $applicant_user_id,
				'application_id' 	=> $application_id,
				'remark_date'  		=> $thirtydays,
				'remark_type' 		=> 'I',
				'remark_by_name'    => $getEmployeeid['fullname'],
				//'remark_field_text' => '',
				'remark_by_role' 	=> '4',
				'hrms_employee_id'  => $getEmployeeid['employee_id']
			);
	
			db_insert('l_remarks_master')->fields($remark_details)->execute();
		}
			
			
		$certificate_download_link = 'download-pdf-clra-registration/'.time().'/'.$application_id_en.'/'.encryption_decryption_fun('encrypt', $applicant_user_id);
		drupal_goto($certificate_download_link);		
			
	}*/
}



