<?php 
drupal_add_js(drupal_get_path('module', 'alc').'/js/mycustom.js');

global $base_root, $base_path, $user;

$alcuserid = $user->uid;
/*if($alcuserid == 61){
echo $a = 'https://wblc.gov.in/download-pdf-clra-registration/1669875811/24057/83210';
$file_contents = file_get_contents($a); die;
}*/

/*if(trim($_POST['clra_alc_gen_certificates'])=='Generate Certificate'){
	if($_POST['alc_clra'] != '' ){
	$applicant_address="select loc_e_subdivision,name_areatype,clra_qr_code,registration_number from l_clra_registration_master where user_id='".$_POST['user_id']."' and id='".$_POST['var_id']."'";
$applicant_address_result=db_query($applicant_address); 
if($applicant_address_result->rowcount()>0){
	foreach($applicant_address_result as $row) {
			$subdivision =trim($row->loc_e_subdivision);
			$area =trim($row->name_areatype);
			$clra_qr_code=$row->clra_qr_code;
			$registration_number=$row->registration_number;
	
	
		
	}
}			
			
$reg_date = date('m/d/Y h:i:s a', time());			
			
//-----------------Reg Genaration
		
$query_Is_reg_code 		 		= 	db_select('l_clra_registration_master', 'lcrm');
$query_Is_reg_code				->	fields('lcrm',array('registration_number'));
$query_Is_reg_code				->	condition('lcrm.registration_number','NULL','!=')
				 				->	orderBy('lcrm.id', 'DESC')
				    			->	range(0,1);
$query_Is_reg_code_result 		= 	$query_Is_reg_code->execute()->fetchObject()->registration_number;	
		
$getShortName 				    =   custom_user_short_name_fun($subdivision);  // ---------come from custom_user module  
$reg_code						=	get_registration_code($area);//-------------miscellaneous module
$getBlockCodeRes 				=   substr($reg_code, -2);
			
if(empty($query_Is_reg_code_result)){
			$reg		=	$getShortName.$getBlockCodeRes.'/'.'CLR'.'/'.'000001';
}else{
		
			
			$reg_query		=	db_query("select  max (NULLIF(substr(registration_number,11,6),'') :: integer) as serial_num  from l_clra_registration_master where loc_e_subdivision='".$subdivision."'");
			$x				=	$reg_query->fetchAssoc();
			$reg_code 		=	$x['serial_num'];
			if(empty($reg_code)){
						$reg		=	$getShortName.$getBlockCodeRes.'/'.'CLR'.'/'.'000001';

			}else{
					  $reg_code_next			= 	$reg_code+1;
					  $reg_firts				=	$getShortName.$getBlockCodeRes.'/'.'CLR'.'/';
					  $reg_second				=	str_pad($reg_code_next, 6, "0", STR_PAD_LEFT);
					  $reg						=	$reg_firts.$reg_second;
			}
}




	$clra_qr_code = 'CLRA-REG'.$_POST['var_id'].'A1U'.$_POST['user_id'].'T'.time();
	
	$frm_v = array('is_from_v'=>1);
	$frm_v_gen = db_update('l_contractor_info_master');
	$frm_v_gen->fields($frm_v);
	$frm_v_gen->condition('application_id',$_POST['var_id']);
	$frm_v_gen->condition('user_id',$_POST['user_id']);
	$frm_v_gen->execute();
	
	$reg = array('registration_number'=>$reg,'registration_date'=>$reg_date,'clra_qr_code' =>$clra_qr_code,'status'=>$_POST['alc_clra']);
	$reg_update =  db_update('l_clra_registration_master');
	$reg_update->fields($reg);
	$reg_update->condition('user_id',$_POST['user_id']);
	$reg_update->condition('id',$_POST['var_id']);
	$reg_update->execute();	
	
	
	$user_type_query="select usr_id, usr_type, fullname, mobile, officenumber, employee_id from l_custom_user_detail where usr_id='".$alcuserid."'";
		
	$user_type_result=db_query($user_type_query); 
	if($user_type_result->rowcount()>0){
		foreach($user_type_result as $dataUser) {
			$fullname =trim($dataUser->fullname);
			$hrms_employee_id = trim($dataUser->employee_id);
		}
	}
	
	$remark_field_text = trim($_POST['field_name']);
		
	$remark_details  =  array(
						'remarks'   		=> 'Congratulations! Certificate is issued by the Registering Authority. You can download it from the dashboard.',
						'remark_by'   		=> trim($alcuserid), 
						'remark_to' 		=> trim($_POST['user_id']),
						'application_id' 	=> trim($_POST['var_id']),
						'remark_date'  		=> time(),
						'remark_type' 		=> $_POST['alc_clra'],
						'remark_by_name'    => $fullname,
						'remark_field_text' =>	$remark_field_text,
						'remark_by_role' 	=> '4',
						'hrms_employee_id'  => $hrms_employee_id
			);
	db_insert('l_remarks_master')->fields($remark_details)->execute();
	}
					
	
	
//}
drupal_goto('alc-visible-applications/'.encryption_decryption_fun('encrypt', trim($_POST['var_id'])).'/'.encryption_decryption_fun('encrypt', trim($_POST['user_id'])));
 	
}*/
//if($_POST['user_id']==352){
	//if(($_POST['clr_alc_submit']=='Submit' || $_POST['clra_alc_gen_certificates']=='Generate Certificate')){ 
//}else{
	//if(($_POST['clr_alc_submit'])=='Submit'){
if(($_POST['clr_alc_submit']=='Submit' || $_POST['clra_alc_gen_certificates']=='Generate Certificate')){ 	//}
	global $base_root , $base_path;
	if($_POST['alc_clra']=='I'){ 
	
		$get_application		=	db_select('l_clra_registration_master', 'lbrm');
		$get_application->	fields('lbrm',array('loc_e_subdivision','name_areatype','registration_number','clra_qr_code','identification_number'));
		$get_application->	condition('lbrm.id',trim($_POST['var_id']));
		$get_application->	condition('lbrm.user_id',trim($_POST['user_id']));
		
		$get_application_result  = 	$get_application->execute(); 
		
		if($get_application_result-> rowCount() > 0 ){
			
			$applicant_data 							= 	$get_application_result->fetchAssoc();
			$subdivision_code							=   $applicant_data['loc_e_subdivision'];
			$area_code									=   $applicant_data['name_areatype'];
			$registration_number						=   $applicant_data['registration_number'];
			$clra_qr_code								=   $applicant_data['clra_qr_code'];
			$identification_number						=   $applicant_data['identification_number'];
		}

		if (empty($registration_number) && empty($clra_qr_code)){
			
			$clra_qr_code = 'CLRA-REG'.$_POST['var_id'].'A1U'.$_POST['user_id'].'T'.time();
			
			/*** GENERATION OF CLRA REGISTRATION NUMBER ***/
			
			$getShortNameSubdivision 			=   custom_user_short_name_fun($subdivision_code);  // ---------custom_user module  
			$getRegistrarCode					=	get_registration_code($area_code);              //-------------miscellaneous module
			$registrarCode 						=   substr($getRegistrarCode, -2);
			
			$reg_query	=	db_query("select max (NULLIF(substr(registration_number,11,6), '') :: integer) as serial_num  from l_clra_registration_master where loc_e_subdivision='".$subdivision_code."'");
			$reg_query_result					=	$reg_query->fetchAssoc();
			$reg_code							=	(int)$reg_query_result['serial_num'];
			
			if(empty($reg_code)){ 				
				$registration_number	=	$getShortNameSubdivision.$registrarCode.'/'.'CLR'.'/'.'000001';
			}else{
				$reg_code_next			= 	$reg_code+1;
				$reg_first				=	$getShortNameSubdivision.$registrarCode.'/'.'CLR'.'/';
				$reg_second				=	str_pad($reg_code_next, 6, "0", STR_PAD_LEFT);
				$registration_number	=	$reg_first.$reg_second;
			}
			
			$reg_date = date('m/d/Y h:i:s a', time());			
			$reg = array('registration_number'=>$registration_number,'registration_date'=>$reg_date,'clra_qr_code' =>$clra_qr_code,'status'=>$_POST['alc_clra']);
			$reg_update =  db_update('l_clra_registration_master');
			$reg_update->fields($reg);
			$reg_update->condition('user_id',$_POST['user_id']);
			$reg_update->condition('id',$_POST['var_id']);
			$reg_update->execute();	
			
			$frm_v = array('is_from_v'=>1);
			$frm_v_gen = db_update('l_contractor_info_master');
			$frm_v_gen->fields($frm_v);
			$frm_v_gen->condition('application_id',$_POST['var_id']);
			$frm_v_gen->condition('user_id',$_POST['user_id']);
			$frm_v_gen->execute();
	
				
		}
		
		
	}
	
	/******OSW******/
	$serviceResponse 	= 1;	
	$fetchCommonId = db_select('l_common_application_master', 'cam');
	$fetchCommonId->fields('cam', array('id', 'oswicsapplicationflag', 'ldapplicationflag', 'clra_ld_service_id', 'ld_service_unique_id', 'ld_uid', 'ld_est_unique_id'));
	$fetchCommonId->condition('cam.user_id', trim($_POST['user_id']));
	// $fetchCommonId->condition('cam.oswicsapplicationflag', 7);
	$commonId = $fetchCommonId->execute()->fetchAssoc(); 
	
	if($commonId['ldapplicationflag'] == 88888){  // SILPASATHI
		$serviceResponse = 0;
		$remark_type = $_POST['alc_clra'];
		if($remark_type == 'B'){
			$status_text = 'Back for Correction';
		}elseif($remark_type == 'V'){
			$status_text = 'Approved';
		}elseif($remark_type == 'I'){
			$status_text = 'Issued';
		}
		$eodb_app_id = $_POST['eodb_app_id'];
		$caf_id_no = $_POST['caf_id_no'];		
		
		if($remark_type == 'B'){
			$curl_post_data = array (
 						'source' => 'WBLC',
						'taskid' => 'STATUSDATA',
						'eodb_app_id' => $eodb_app_id,
						'caf_id_no' => $caf_id_no,								
						'status_code' => $remark_type,
						'status_text' => $status_text,
						'status_date' => date('Y-m-d H:m:s'),
						'application_id' => $_POST['var_id'],
						'status_remarks' => $_POST['clra_alc_remarks']				
						);	
		}elseif($remark_type == 'V'){	
			$paymentdetails[] = array(
						'payableamt' => $_POST['finalfees'],
						'hoa' => '0230-00-106-001-27',
						'purpose' => 'Registration Fees',
						);			
			$curl_post_data = array (
 						'source' => 'WBLC',
						'taskid' => 'PAYMENTINFO',
						'eodb_app_id' => $eodb_app_id,
						'caf_id_no' => $caf_id_no,
						'deptCode' => '050',
						'svcCode' => '301',
						'payabletotalamt' => $_POST['finalfees'],
						'periodFrom' => date('Y-m-d'),
						'periodTo' => date('Y-m-d'),
						'paymentdetails' => $paymentdetails,			
						'status_code' => $remark_type,
						'status_text' => $status_text,
						'status_date' => date('Y-m-d H:m:s'),
						'application_id' => $_POST['var_id'],
						'status_remarks' => $_POST['clra_alc_remarks']				
						);	
		}elseif($remark_type == 'I'){ 			
			// $file_link = 'https://wblc.gov.in/download-pdf-clra-registration/'.time().'/'.encryption_decryption_fun('encrypt', $_POST['var_id']).'/'.encryption_decryption_fun('encrypt', $_POST['user_id']); 
			
			// $file_link = 'https://wblc.gov.in/sites/default/files/CLRA-REG-1669879822.pdf'; 
			
			$file_contents = file_get_contents($file_link);
						
			$data_file[] = array(
						'file_id' => $_POST['registration_number'],
						'file_date' => $_POST['registration_date'],
						'file_category' => 'CERTIFICATE',
						'file_name' => str_replace('/', '-', $_POST['registration_number']).'-'.$_POST['var_id'].'.pdf',
						'file_type' => 'application/pdf',
						'file_content' => base64_encode($file_contents),
						'valid_upto' => '',
						);
			$cquery = db_select('l_contractor_info_master', 'cm');
			$cquery->fields('cm', array('eodb_con_id', 'id', 'name_of_contractor', 'status', 'is_from_v'));
			$cquery->condition('cm.application_id', trim($_POST['var_id']));
			// $cquery->condition('cm.eodb_con_id', 0, '!=');
			$cquery = $cquery->execute(); 
			
			if($cquery->rowCount()>0){
				foreach($cquery as $cdata) {
					$contractorlist[] = array(
									'formv_no' => $cdata->id,
									'contractor_name' => $cdata->name_of_contractor,
									'eodb_con_id' => $cdata->eodb_con_id,
									'is_form_v' => trim($cdata->is_from_v),
									'status' => trim($cdata->status)
								 );
				}
			}
			
			$additionalParamDtls[] = array(
						'paramName' => 'CONTRACTORLIST',
						'paramVal' => $contractorlist					
						);					
			$curl_post_data = array (
 						'source' => 'WBLC',
						'taskid' => 'STATUSDATA',
						'eodb_app_id' => $eodb_app_id,
						'caf_id_no' => $caf_id_no,									
						'status_code' => $remark_type,
						'status_text' => $status_text,
						'status_date' => date('Y-m-d H:m:s'),
						'status_remarks' => $status_text,
						'status_remarks' => $_POST['clra_alc_remarks'],
						'application_id' => $_POST['var_id'],
						'data_file' => $data_file,
						'additionalParamDtls' => $additionalParamDtls			
						);		  
			
		}
		// echo '<pre>'; print_r($curl_post_data); die;	
		// echo json_encode($curl_post_data); die;
		// $array_enc = encrypt_swold($status_arr);
		// $curl_post_data = array ('message' => $array_enc, 'status_val' => $array_enc); 
		$service_url = "https://silpasathi.wb.gov.in/eservices/deptpostData";
		$ch = curl_init();
		$headers = array();
		$headers[] = 'Accept: application/json';
		$headers[] = 'Content-Type: application/json';
		$headers[] = 'charset=utf-8';
		curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
		curl_setopt($ch, CURLOPT_URL, $service_url);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
		curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 0);
		curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($curl_post_data));
		curl_setopt($ch, CURLOPT_POST, true);
		$results    = curl_exec($ch);
		$errors     = curl_error($ch); // print_r($errors); die;
		curl_close($ch);
		// $curlRes = json_encode($results); 
		$results = json_decode(trim($results), TRUE); // echo '<pre>'; print_r($results); die;
 
	   if($results['code'] == 200){
			$serviceResponse = 1;
			// $res_array = $result['content'];
	   }	
	}
	if($commonId['oswicsapplicationflag'] == 7){  
		
		$svcrespostatus = 0;
		$service_url = 'http://202.61.117.163:8443/api/Generic/Post';
		$properties_ss = 'Qqq7HUBItKukq0aG0HNkdscTZdVMXJGyAMOGswKAFqkdTtuXFggJgfS56G3rjn5gSGtPuRqwg23X6IqZhSONs8BAwfX6LzdweLb0NqB1RRQGQnrC0kUt6GM98XuWRxYJeSXNkhbiSdj5ZDerECaG6ck1wLo4Q1xFPmhipUXwD/B860tsRnpKlbrmuyz1ByUUSVHY+Kx9ijXUX4eZsRZHnwnDY9Vwm6+8QoKq5gXsYi64dJxczd+kUNXSIbRv+ET9gAYG3aE375xOuhTaPV85szRQz7QKE1ycEXEAmM1zdsQw1yEWI4sVXOhCUKlVe2LIJABq8TzLgi0Dq+CSlOivbEsSUJqt/k/Eu/WizWc2G9t9MvSLlutR4/TmhYQWZ7weWzpcQytZkJgVzTuvdFyhLF7n1a2BSHuKXMHkUUrMfXUh2NwgrXDjFafCn5ubuCBwDEs/N7hTgVCNSTz9KVL+E2AJ73OWF2o8+bjT/nFnePROjlNaXtAkOXqVw248pqlLh4cAm7hn7+E+ihpdAynuBm4lK731u3N21bUux8pOKRGyUrh1MuwZLcQ599wuHiZ6CIQpcpTJ7JJeH0+mT322x46kYu3hbDGsPPygkC102eS9sAJ/yERENzJOn0w1p9mpQz3e3WBFWVXIoiCW3/3SBYfQ26IthNMNMH0ufS4SCJ5cxOwCPfCeIoBQLQYZR70JHA7Mq+aseyXNM6wlKHysVRqUPR1nAuzS2wupYXDTMvs8wstOkdqkyTwUcJrNwCDZaVwbWgai628q9lUAszPdqveiX2bzGyMISxJ6mzk3jVk6osSo9EHHXFBEhyzuprlEhkW4LnCQwmVOyu+SXeKbbh1Jumv50iUs2ZoTE7jbF3MD50TPXRD+YQo6KzZwnBDuv14+uNQ2vC22x060SgkWCqw+YrTFhHrrEpdxw3gGjK0u7AjbPtj8oiznhetBaw9kT7maHt4gKFhUrvGEJp71Igr32lO8AdVUfOtnlkoqdphkw33VATY9+KIeADVa+6Fp2PcW7BOcfSWHhSydxuj6lQ==';
		
		$registration_number = '';
		$registration_date = '';
		if(trim($_POST['alc_clra']) == 'I' ){	
			$redistration_number=$_POST['registration_number'];
			$target_dir = 'upload/alc_upload_certificates_clra/';
			$extension = end(explode(".", $_FILES["fileToUpload"]["name"]));
			$target_file = $target_dir;
			$destination = $target_file;
			
			$swap_file = file_save_upload('clra_signed_certificate', array(), file_build_uri($target_file), $replace=FILE_EXISTS_RENAME);  
			
			if ($file = file_save_upload('clra_signed_certificate',$replace = FILE_EXISTS_RENAME)){
				$uri=trim($file->uri);
				$file->status = FILE_STATUS_PERMANENT;
				file_save($file);
				$certificates_fid = $file->fid;
				$certificates_file_name = $file->filename;
				$dir=$target_dir.$file->filename;					
								
				$file_link=escapefile_url($uri);					
								
				$file_link_image_link=file_get_contents($file_link);				
				$encript_form1 = base64_encode($file_link_image_link);				
			}
		}
			
		$clra_registration = db_select('l_clra_registration_master', 'lrm');
		$clra_registration->fields('lrm', array('identification_number', 'registration_date', 'registration_number'));
		$clra_registration->condition('user_id', $_POST['user_id']);
		$clra_registration->condition('id', $_POST['var_id']);
		$clra_regresult = $clra_registration->execute()->fetchObject();
		$identification_number = $clra_regresult->identification_number;
		
		if($clra_regresult->registration_number != ''){
			$registration_number = $clra_regresult->registration_number;
			$registration_date = date('d/m/Y', strtotime($clra_regresult->registration_date));
			$insfilename = $certificates_file_name;
			$file_contents = $encript_form1;
		}
		
		$user_type_query_contractor="select name_of_contractor,id from l_contractor_info_master where application_id='".$_POST['var_id']."' and user_id='".$_POST['user_id']."'";
		$user_type_result_contractor=db_query($user_type_query_contractor); 
		$array_val_con=array();
		if($user_type_result_contractor->rowcount()>0){
		foreach($user_type_result_contractor as $contractor_result) {
			$array_val_con[]=array("contractor_id"=> trim($contractor_result->id),
									"contractor_name"=> trim($contractor_result->name_of_contractor),
									'contractor_tid' => 'lrpea',
									'eodbcafno' => $identification_number
								 );
			}
		}
		
		
		///remark value
		
		$clra_remarks_recordid = db_select('l_remarks_master', 'lrm');
		$clra_remarks_recordid->fields('lrm', array('recordid','remark_type','remark_by_role'));
		$clra_remarks_recordid->condition('remark_to', $_POST['user_id']);
		$clra_remarks_recordid->condition('application_id', $_POST['var_id']);
		$clra_remarks_recordid->orderBy('remark_id', 'DESC');
   		$clra_remarks_recordid->range(0, 1);
		$clra_remarks_recordid_regresult = $clra_remarks_recordid->execute()->fetchObject();
		$clra_remarks_recordid_number = $clra_remarks_recordid_regresult->recordid;
		$remark_type = $clra_remarks_recordid_regresult->remark_type;
		$remark_by_role = $clra_remarks_recordid_regresult->remark_by_role;
		
		if($remark_by_role==4 && $remark_type=='T'){
			
			$remark_types='U';
		}else{
			
			$remark_types=$_POST['alc_clra'];
		}
		$remark_types = trim($_POST['alc_clra']);
		$user_type_query_trade_union="select e_trade_union_name,e_trade_union_regn_no,id from l_clra_reg_trade_union_master where application_id='".$_POST['var_id']."' and user_id='".$_POST['user_id']."'";
		$user_type_query_trade_union_result=db_query($user_type_query_trade_union); 
		$array_val_trade=array();
		if($user_type_query_trade_union_result->rowcount()>0){
		foreach($user_type_query_trade_union_result as $tradeunion) {
			$array_val_trade[] =array(										
										 'union_id'=>$tradeunion->id,
										 'union_name' =>$tradeunion->e_trade_union_name,
										 'union_tid' => 'lrpea',
										 'eodb_caf_no' => $identification_number
									);
			}
		}
		
		
		$curl_post_data = array(
					'transid' => 'lcsts',
					'properties'  => $properties_ss,
					'dc1' => array(
								'no' => '1',
								'type' => '0',
								'data' => array(
											  'application_id' => trim($_POST['var_id']),
											  'remark_type' => trim($remark_types),   
											  'eodbcafid' => $identification_number,
											  'registration_date' => $registration_date,
											  'registration_number' => $registration_number,
											  'insfilename' => $insfilename,
											  'file_contents' => $file_contents,
											  'remarks' => $_POST['clra_alc_remarks'],
											  'remark_by' => 'ALC',
											  'remark_date' => date('d/m/Y'),
											  'remark_to' => 'Applicant',
											  'record_id' => $clra_remarks_recordid_number,
											  'form_tid' => 'lrpea'
											)
											
					),
					"dc2" => array(
							"no"=> "2",
							"type"=> "1",
							"data"=> $array_val_con
							),
								
														
						
				    "dc3"=> array(
							"no"=> "3",
							"type"=> "1",
							"data"=> $array_val_trade
							)
								
					);
					
		// echo json_encode($curl_post_data); die;
		// print_r($curl_post_data);exit;
					
		$ch = curl_init();
		curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-Type: application/json; charset=utf-8'));
		curl_setopt($ch, CURLOPT_URL, $service_url);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0); 
		curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 0);
		curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($curl_post_data));
		curl_setopt($ch, CURLOPT_POST, true);
		$results = curl_exec($ch);

		curl_close($ch);
					
		$tmp = json_decode($results);
		
		$data = explode('*$*', $tmp);
		
		$a = json_decode($data[1])->message;
		$b = json_decode($data[2])->result; // print_r($b); die;

		if($b[0]->save == 'success'){
			$svcrespostatus = 1;
		}
	} // echo $svcrespostatus; echo 9999; die;	
	if($commonId['ldapplicationflag'] == 1){
				
		$serviceResponse = 0;		
		$status_arr = array (
 						'status' => trim($_POST['alc_clra']),						
 						'id' => $commonId['clra_ld_service_id'],
						'ld_uid' => $commonId['ld_uid'],
						'ld_est_unique_id' => $commonId['ld_est_unique_id'],
						);					
		$array_enc = encrypt_swold($status_arr);
		$curl_post_data = array ('message' => $array_enc, 'status_val' => $array_enc);				
		$service_url  =   "https://wblabour.gov.in/restld/establishmentStatusUpdate";
		$ch       =   curl_init();
		$headers    = array();
		$headers[]    = 'Accept: application/json';
		$headers[]    = 'Content-Type: application/json';
		$headers[]    = 'charset=utf-8';
		curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
		curl_setopt($ch, CURLOPT_URL, $service_url);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
		curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 0);
		curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode( $curl_post_data ) );
		curl_setopt($ch, CURLOPT_POST, true);
		$results    = curl_exec($ch);
		$errors     = curl_error($ch);
		curl_close($ch);
		$x = json_encode($results);
		$tmp = json_decode(json_decode(trim($x)), TRUE);
 
	    if($errors){
			//  echo "<br>";
			//  print_r(curl_errno());
			//  print_r($errors);
	    }else{
			// echo "<br>";
			// echo "Ok";
			// echo "<br>";
		}
	   
	   	// print_r($results); echo "<br>";
	    $return_msg_encrypted = json_decode($results, TRUE);
	    $return_msg = decrypt_swold($return_msg_encrypted['message']);
	    //print_r($return_msg); echo $return_msg['status']; die;
	   
	    if(trim($return_msg['status']) == 1){
		   $serviceResponse = 1;
	    }
		
	}
	
	/******OSW******/
	if($serviceResponse == 1){	
		
		$_SESSION['error_msg'] = '';
		if(empty($_POST['clra_alc_remarks'])){
			
			drupal_set_message(t('Remark can not be empty.'));
			$_SESSION['error_msg'] = 'Remark can not be empty.';
		
		}else if($_POST['alc_clra'] != ''){
			//if($_POST['user_id']!=352){
			//if(trim($_POST['alc_clra']) == 'I' && empty($_FILES["files"]["name"]["clra_signed_certificate"])  	){//&& empty($_FILES["files"]["name"]["clra_signed_certificate"])  			
				//drupal_set_message(t('Please select certificate file.'));
       		//				$_SESSION['error_msg'] = 'Please select certificate file.';
			//}else{
				
			$status_remark 	= trim($_POST['alc_clra'])	;
			$status = array('status'=>$status_remark);
			$uid = $user->uid;
			
			$user_type_query="select usr_id, usr_type, fullname, mobile, officenumber, employee_id from l_custom_user_detail where usr_id='".$user->uid."'";
			
			$user_type_result=db_query($user_type_query); 
			if($user_type_result->rowcount()>0){
				foreach($user_type_result as $dataUser) {
					$fullname =trim($dataUser->fullname);
					$hrms_employee_id = trim($dataUser->employee_id);
				}
			}
			if( trim($_POST['alc_clra']) == 'C' ){
				$remarks = trim($_POST['clra_alc_remarks']).' on '.trim($_POST['clra_alc_call_time']);
			}else if($_POST['alc_clra'] == 'I'){
				$remarks ='Congratulations! Certificate is issued by the Registering Authority. You can download it from the dashboard.';
			}else{
				$remarks = trim($_POST['clra_alc_remarks']);
			}
			
			if(empty($remarks)){
				$message= drupal_set_message_custom(t('Remark must be filled out'),'error');  //-------------miscellaneous module
				echo $message;
		
			}
			//print_r($status).'---'.echo $_POST['var_id'];die;
			$remark_field_text = trim($_POST['field_name']);
			
			$remark_details  =  array(
								'remarks'   		=> $remarks,
								'remark_by'   		=> trim($uid), 
								'remark_to' 		=> trim($_POST['user_id']),
								'application_id' 	=> trim($_POST['var_id']),
								'remark_date'  		=> time(),
								'remark_type' 		=> $status_remark,
								'remark_by_name'    => $fullname,
								'remark_field_text' => trim($remark_field_text),
								'remark_by_role' 	=> '4',
								'hrms_employee_id'  => $hrms_employee_id
					);
					
			db_insert('l_remarks_master')->fields($remark_details)->execute();
			// echo '<pre>';
			// print_r($remark_details);die;
			$QryTestInfoUpdt = db_update('l_clra_registration_master');
			$QryTestInfoUpdt->fields($status);
			$QryTestInfoUpdt->condition('id',$_POST['var_id']);
			$QryTestInfoUpdt->execute();
			
			/*if($_POST['user_id']!=352){
			if(trim($_POST['alc_clra']) == 'I' ){ 		
				$redistration_number=$_POST['registration_number'];
				$target_dir = 'upload/alc_upload_certificates_clra/';
				$extension = end(explode(".", $_FILES["fileToUpload"]["name"]));
				$target_file = $target_dir;
				$destination = $target_file;
				
				$swap_file = file_save_upload('clra_signed_certificate', array(), file_build_uri($target_file), $replace=FILE_EXISTS_RENAME);  
				
				if ($file = file_save_upload('clra_signed_certificate',$replace = FILE_EXISTS_RENAME)){	   
					$uri=trim($file->uri);		
					$file->status = FILE_STATUS_PERMANENT;
					file_save($file);
					$certificates_fid = $file->fid;
					$certificates_file_name = $file->filename;
				}
				
				$file_uplod_array  =  array('certificates_fid'=>$certificates_fid);
				$file_uplod_update =  db_update('l_clra_registration_master');
				$file_uplod_update->fields($file_uplod_array);
				$file_uplod_update->condition('user_id', $_POST['user_id']);
				$file_uplod_update->condition('id', $_POST['var_id']);
				$file_uplod_update->execute();	
			}
			}*/
			
			
			// FOR SMS ALERT START
			
			$applicant_mobile_no = get_mobile_number($_POST['user_id']);
			
			if($status_remark == 'B'){					
				$msg = 'Your CLRA application is backed for rectification by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';
				send_sms_for_user_alert($applicant_mobile_no, $msg);			
			}else if($status_remark == 'V'){			
				$msg = 'Your CLRA application is approved for fees submission by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';
				send_sms_for_user_alert($applicant_mobile_no, $msg);
			}else if($status_remark == 'I'){
				if($commonId['ldapplicationflag'] == 8 && $status_remark == 'I'){
					$applicant_mailto  = get_email($_POST['user_id']);
					db_update('users')->fields(array('status'=>1))->condition('status',0)->condition('uid',$_POST['user_id'])->execute();
					$msg = 'Congratulations! Your CLRA registration certificate is issued by ALC(Govt. of WB). Login to download the certificate from dashboard. <br>You may avail other services from <strong><a href="'.$base_root.$base_path.'">wblc.gov.in</a></strong><br/><br/> <strong></strong>Username :- '.$identification_number.'<br/>Email : - '.$applicant_mailto.'<br/>Mobile : - '.$applicant_mobile_no.'<br/>Also, you may find your user credentials from <a href="'.$base_root.$base_path.'find-user-details">CLICK HERE</a> with your application information. You can generate your password from <a href="'.$base_root.$base_path.'forgot-password">Forgot your password</a> ';
					$subject = 'Welcome to Labour Commissionerate, Govt. Of West Bengal';										
				}else{
					$msg = 'Congratulations! Your CLRA registration certificate is issued by ALC(Govt. of WB). Login to download the certificate from dashboard (wblc.gov.in)';
					send_sms_for_user_alert($applicant_mobile_no, $msg);
				}
				if(!empty($applicant_mailto)){
					send_mail_for_user_alert($applicant_mailto, $subject, $msg);					
				}
			}else if($status_remark == 'R'){			
				$msg = 'Your CLRA application is rejected by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';
				send_sms_for_user_alert($applicant_mobile_no, $msg);			
			}else if($status_remark == 'VA'){						
				$msg = 'Your CLRA application is approved by ALC(Govt. of WB). Please check your dashboard for further process.(wblc.gov.in)';
				send_sms_for_user_alert($applicant_mobile_no, $msg);		
			}
			// send_sms_for_user_alert('7603091500', $msg);
					
			// FOR SMS ALERT END
			
			drupal_set_message(t('Action and remark successfully submitted for this application.'));	
	
			drupal_goto('alc-visible-applications/'.encryption_decryption_fun('encrypt', trim($_POST['var_id'])).'/'.encryption_decryption_fun('encrypt', trim($_POST['user_id'])));
			//}
		}
	}else{
		drupal_set_message(t('Please select any type actions'));	
	}
}
?>

<script type="text/javascript">

jQuery(document).ready(function() {		
	jQuery("#back_for_correction_alc").click(function(e){		
		var field_name = jQuery("#field_name").val();
		if(confirm('want to continue')){ 
			jQuery("#chech_data").val(field_name);
		}
	});
	
	
	jQuery(".viewfile_popup").click(function(e){
		var id = jQuery(this).attr('id');
        e.preventDefault(); // this will prevent the browser to redirect to the href
		jQuery( "#view_"+id ).dialog();
	});		


	jQuery('.verified_alc_clra_yn').on('ifChanged', function (e){		
		var fieldIdt = jQuery("input[name=field_name]").val();		
		if (jQuery(this).is(':checked')) {					
			var fieldId 	= jQuery(this).attr('id');
				
			if (fieldIdt == ''){
				var fieldIdt 	= fieldId;
			}else{
				var fieldIdt 	= fieldIdt+','+fieldId;
			}			
			jQuery("input[name=field_name]").val(fieldIdt);
		}
		
		var field_name = jQuery("input[name=field_name]").val();
		
		if (!jQuery(this).is(':checked')) {	
			var fieldIdt = '';			
			var fieldId = jQuery(this).attr('id');
			//jQuery("#editable_fields_license").val('');
			var field_name_arr = field_name.split(',');
				
			for(var i=0;i<field_name_arr.length;i++){
				if(field_name_arr[i] != fieldId){
					var fieldIdt = fieldIdt+','+field_name_arr[i];
					fieldIdt = fieldIdt.replace(',,',',');
					jQuery("input[name=field_name]").val(fieldIdt);					
					var t =	jQuery("input[name=field_name]").val();						
					if(t==','){						
						jQuery("input[name=field_name]").val(null);
					}
				}
   			}    			
		}		
		
	});
	
	jQuery('#alc_clra_id').change(function (e){
		var option_val = jQuery('#alc_clra_id').val();
		var clra_qr_code = jQuery("input[name=clra_qr_code]").val();
		var clra_reg_no = jQuery("input[name=clra_reg_no]").val();
		var clra_reg_date = jQuery("input[name=clra_reg_date]").val();
		
		jQuery("#clra_alc_remarks_id").val('');
		
		if(option_val == 'V' || option_val == 'I' || option_val == 'VA'){
			if(confirm("All information are checked and verified. Do you want to continue?")){
				jQuery(".icheckbox_flat-blue").addClass('checked');
				jQuery(".icheckbox_flat-blue").eq(1).removeClass('checked');
				jQuery(".icheckbox_flat-blue").attr('aria-checked','true');
				
				var alflstring = 'full_name_manager,address_manager,e_name,loc_e_name,e_postal_address,e_nature_of_work,max_num_wrkmen,e_num_of_workmen_per_or_reg,e_num_of_workmen_temp_or_reg,workmen_if_same_similar_kind_of_work,con_lab_job_desc,con_lab_wage_rate_other_benefits,con_lab_cat_desig_nom,e_settlement_award_judgement_min_wage,e_any_day_max_num_of_workmen,trade_license_file,article_of_assoc_file,memorandum_of_cert_file,partnership_deed_file,factory_license_file,form_1_clra_signed_pdf_file,backlog_clra_registration_certificate_file,address_principal_emp,full_name_principal_emp,est_type,gender_pe';
				
				jQuery("input[name=field_name]").val(alflstring);
				jQuery('#alc_clra_id').attr('selected','selected');
				
				if(option_val == 'I'){
					if(clra_qr_code != '' && clra_reg_no != '' && clra_reg_date != ''){ 
						document.getElementById('action_div').style.display = 'block'; 
						document.getElementById('genarate_pdf').style.display = 'none';
						// document.getElementById('action_call_time').style.display = 'none';
						document.getElementById('submit_remark').style.display = 'block';
						jQuery('#clr_alc_submit_id').show();						
					}else{
						document.getElementById('genarate_pdf').style.display = 'block';
						// document.getElementById('action_call_time').style.display = 'none';
						document.getElementById('action_div').style.display = 'none';
						document.getElementById('submit_remark').style.display = 'none';
						jQuery('#clr_alc_submit_id').hide();
					}
					 
					jQuery("#clra_alc_remarks_id").val('Congratulations! Certificate is issued by the Registering Authority. You can download it from the dashboard.');					
				}
				if(option_val == 'V'){
					jQuery("#clra_alc_remarks_id").val('Application is verified and approved by Registering Authority and directed to pay fees. After successful payment download FORM-I from the dashboard and upload it after signing the downloaded FORM-I.');
				}
				if(option_val == 'VA'){
					jQuery("#clra_alc_remarks_id").val('Application is verified and approved by Registering Authority. Download FORM-I from the dashboard and upload it after signing the downloaded FORM-I.');
				}
																	
			}else{				
				jQuery('#alc_clra_id').val('');
				jQuery("#clra_alc_remarks_id").val('');
			}
		// }else if(option_val == ''){
		}else{  
			// if(confirm("Want to change selected option?")){			
				// window.location.reload(true);
			// }
			// document.getElementById('submit_remark').style.display = 'none';			
			
			var field_nameorg = jQuery("input[name=field_nameorg]").val();	
			jQuery("input[name=field_name]").val(field_nameorg);
			
			jQuery('#submit_remark').show();
			jQuery('#clr_alc_submit_id').show();					
		}
		
		
		
		if(option_val == 'B'){ // RECTIFICATION INFORMATION
			jQuery("#clra_alc_remarks_id").val('Application is sent back for rectification. Kindly modify disapproved fields and re-submit the application.');
		}
		if(option_val == 'T'){ // RECTIFICATION FORM I
			jQuery("#clra_alc_remarks_id").val('Application is sent back for FORM-I rectification. Kindly modify disapproved fields and re-submit the application.');
		}
		if(option_val == 'F'){
			jQuery("#clra_alc_remarks_id").val('Application is sent back for further verification. Kindly verify and forward to the Registering Authority.');
		}		
		
		if(option_val != 'I'){
			document.getElementById('genarate_pdf').style.display = 'none';			
			document.getElementById('action_div').style.display = 'none';
			// document.getElementById('submit_remark').style.display = 'none';			
		}
	});
	
	

var action = jQuery("#alc_clra_id").val();

if(action == 'I'){
	jQuery("#clra_alc_remarks_id").val('Congratulations! Certificate is issued by the Registering Authority. You can download it from the dashboard.');		
}

});

</script>

<?php
 drupal_add_js(drupal_get_path('module', 'alc').'/js/view_doc_js.js');


$get_remark = db_select('l_remarks_master', 'lrm');
$get_remark->fields('lrm', array('remark_field_text','remark_by_role'));
$get_remark->condition('lrm.remark_to', $userid);
$get_remark->condition('lrm.application_id', $var_id);
$get_remark->condition('lrm.remark_by', $userid, '!=');
$get_remark->condition('lrm.is_active', 1);
$get_remark->orderBy('lrm.remark_id','DESC');
$get_remark->range(0, 1);
$get_remark_result = $get_remark->execute();

$editable_data = $get_remark_result->fetchAssoc();

$remark_by_role=$editable_data['remark_by_role'];



$remark_field_text_arr = explode(',', $editable_data['remark_field_text']);

for($i=0; $i<count($remark_field_text_arr); $i++){
	$fieldname = $remark_field_text_arr[$i];
		
	if($fieldname == 'e_name'){ $e_name_ck = "checked='checked'";}
	if($fieldname == 'est_type'){	$estType_ck = "checked='checked'";}
	if($fieldname == 'loc_e_name'){ $loc_e_name_ck = "checked='checked'";}
	if($fieldname == 'e_postal_address'){ $e_postal_address_ck = "checked='checked'";}	
	if($fieldname == 'full_name_principal_emp'){ $full_name_principal_emp_ck = "checked='checked'";}
	if($fieldname == 'gender_pe'){	$gender_pe_ck = "checked='checked'";}
	if($fieldname == 'address_principal_emp'){ $address_principal_emp_ck = "checked='checked'";}	
	if($fieldname == 'full_name_manager'){ $full_name_manager_ck = "checked='checked'";}	
	if($fieldname == 'address_manager'){ $address_manager_ck = "checked='checked'";}	
	if($fieldname == 'e_nature_of_work'){ $e_nature_of_work_ck = "checked='checked'";}	
	if($fieldname == 'e_any_day_max_num_of_workmen'){ $e_any_day_max_num_of_workmen_ck = "checked='checked'";}	
	if($fieldname == 'e_num_of_workmen_per_or_reg'){ $e_num_of_workmen_per_or_reg_ck = "checked='checked'";}	
	if($fieldname == 'e_num_of_workmen_temp_or_reg'){ $e_num_of_workmen_temp_or_reg_ck = "checked='checked'";}	
	if($fieldname == 'workmen_if_same_similar_kind_of_work'){ $workmen_if_same_similar_kind_of_work_ck = "checked='checked'";}
	if($fieldname == 'con_lab_job_desc'){ $con_lab_job_desc_ck = "checked='checked'";}	
	if($fieldname == 'con_lab_wage_rate_other_benefits'){ $con_lab_wage_rate_other_benefits_ck = "checked='checked'";}	
	if($fieldname == 'con_lab_cat_desig_nom'){ $con_lab_cat_desig_nom_ck = "checked='checked'";}	
	if($fieldname == 'e_trade_union_name'){ $e_trade_union_name_ck = "checked='checked'";}
	if($fieldname == 'e_trade_union_address'){ $e_trade_union_address_ck = "checked='checked'";}	
	if($fieldname == 'e_settlement_award_judgement_min_wage'){ $e_settlement_award_judgement_min_wage_ck = "checked='checked'";}
	
	if($fieldname == 'backlog_clra_registration_certificate_file'){ $backlog_clra_registration_certificate_file_ck = "checked='checked'";}
	if($fieldname == 'form_1_clra_signed_pdf_file'){ $form_1_clra_signed_pdf_file_ck = "checked='checked'";}
	if($fieldname == 'max_num_wrkmen'){ $max_num_wrkmen_ck = "checked='checked'";}
	if($fieldname == 'trade_license_file'){ $trade_license_file_ck = "checked='checked'";}
	if($fieldname == 'article_of_assoc_file'){ $article_of_assoc_file_ck = "checked='checked'";}
	if($fieldname == 'memorandum_of_cert_file'){ $memorandum_of_cert_file_ck = "checked='checked'";}
	if($fieldname == 'partnership_deed_file'){ $partnership_deed_file_ck = "checked='checked'";}
	if($fieldname == 'factory_license_file'){ $factory_license_file_ck = "checked='checked'";}
	
	
	
	
}

$establishment_address 	= miscellaneous_clra_pe_location_address($var_id);
$postal_address 		= miscellaneous_postal_add_pe($var_id);
$pe_address 			= miscellaneous_clra_pe_address($var_id);
$manager_address	 	= miscellaneous_manager_pe($var_id);


/**** NATURE OF WORK CODE STARTS*****/

$get_nature_work = 	db_select('l_clra_reg_nature_of_work', 'lcnw');
$get_nature_work ->	leftJoin('contractor_works', 'cw', 'lcnw.e_nature_of_work=cw.contractor_work_id');
$get_nature_work ->	fields('cw',array('cont_work_name','contractor_work_id'));
$get_nature_work ->	condition('lcnw.user_id',$content['user_id']);
$get_nature_work ->	condition('lcnw.is_active',1);
$get_nature_work ->	condition('lcnw.application_id',$var_id);
$get_nature_work ->	orderBy('lcnw.id');
// $get_nature_work ->	condition('cw.is_default',1);
$query_get_each_nature_work = $get_nature_work->execute(); 

$prefix						= '';	
$natureOfWrkString 			= '';
$lastsuffix 				= '';

if(!empty($query_get_each_nature_work)){
	$total = $query_get_each_nature_work->rowCount(); 
	
	if($total > 1 ) $lastsuffix 	= ', '; else $lastsuffix 	= '';
	
	foreach($query_get_each_nature_work as $listData){ 
	
			$natureOfWrkString 			.= $prefix.$listData->cont_work_name;
			$prefix  	 				 = ', ';
		// if($listData->contractor_work_id == 28){

		// 	/** FOR FIELD FOR OTHER OPTION OF NATURE WORK ****/

		// 	$get_other_nature_work 		= db_select('l_clra_reg_nature_of_work', 'lcnw');
		// 	$get_other_nature_work		->leftJoin('contractor_works', 'cw', 'lcnw.e_nature_of_work=cw.contractor_work_id');
		// 	$get_other_nature_work		->fields('cw',array('cont_work_name','contractor_work_id'));
		// 	// $get_other_nature_work		->condition('cw.is_default',0);
		// 	$get_other_nature_work		->condition('lcnw.user_id',$content['user_id']);
		// 	$get_other_nature_work		->condition('cw.act_id',1);
		// 	$get_other_nature_work		->condition('lcnw.application_id',$var_id);
		// 	$other_nature_work_data 	= $get_other_nature_work->execute(); 
			
		// 	if( $other_nature_work_data->rowCount() > 0 ){ 
				
		// 		$obj3 = $other_nature_work_data->fetchAssoc(); 
				
		// 		$other_nature_wrk_id 	= $obj3['contractor_work_id'];
		// 		$other_nature_wrk 		= $obj3['cont_work_name'];
				
		// 		// $natureOfWrkString 		.= $lastsuffix.$other_nature_wrk;
				
		// 	}
			
		// }else{ 								
		// 	$natureOfWrkString 			.= $prefix.$listData->cont_work_name;
		// 	$prefix  	 				 = ', ';
		// }
				
	} 
}

?>

<div class="box box-primary box-solid">
    <div class="box-header with-border">
    	<div style="text-align:center;">NOTE:- All inputs are provided by Principal Employer OR on behalf of Principal Employer<br/><input type="checkbox" id="check_symbol" class="note check2" checked="checked" /><label class="label2" for="check_symbol"></label> <!--symbol means input has been--> verified&nbsp;&nbsp; <input type="checkbox" id="uncheck_symbol" class="note check2" /><label class="label2" for="uncheck_symbol"></label>&nbsp;&nbsp;<!--symbol means input has been--> not verified   </div> 
    </div>
    <div class="box-body"><div class="feedback-scroll">
    	<table width="100%" border="0" class="table table-bordered table-responsive">
        	<tr>
              <th style="width:10%">Sl</th>
              <th style="width:40%">Parameters</th>
              <th style="width:40%">Inputs</th>
              <th style="width:10%">Verified?</th>
            </tr>
    <?php
	if($content['gender_pe']=='M'){
		$gender = 'Male';	
	}elseif($content['gender_pe']=='F'){
		$gender = 'Female';	
	}elseif($content['gender_pe']=='O'){
		$gender = 'Transgender';
	}
	if(!empty($content['backlog_id'])){
		$get_bklg_tbl = db_select('l_clra_principle_emp_backlog_data', 'lcpebd');
		$get_bklg_tbl->fields('lcpebd', array('registration_no', 'registration_date', 'fees'));
		$get_bklg_tbl->condition('lcpebd.id', $content['backlog_id']);
												
		$get_bklg_tbl_res =	$get_bklg_tbl->execute();
		$get_bklg_tbl_res_data = $get_bklg_tbl_res->fetchObject();
		?>
            <tr>
            	<td>#</td>
                <td>Registration number/date(Offline)</td>
                <td><?php echo '<strong>Reg. No.:- </strong>'.$get_bklg_tbl_res_data->registration_no.'&nbsp;&nbsp;<strong>Date:- </strong>'.date('d/m/Y',strtotime($get_bklg_tbl_res_data->registration_date));?></td>
                <td align="center">
                <input type="checkbox" id="e_name_bl" class="verified_alc_clra_yn check2" checked="checked" disabled="disabled" />
                <label class="label2" for="e_name_bl"></label>
                </td>
            </tr>
		<?php
	}
    ?>
            <tr>
           		<td colspan="4" style="font-weight:700; text-transform:uppercase; font-size:15px;">1. Full Name and address of the Principal Employer</td>
            </tr>
            <tr>
            	<td>1.(a)</td>
                <td width="40%">Full Name of the principal employer</td>
                <td><?php echo $content['full_name_principal_emp'];?></td>
                <td align="center"><input type="checkbox" name="full_name_principal_emp" id="full_name_principal_emp" class="verified_alc_clra_yn check2"  <?php echo $full_name_principal_emp_ck; ?> />
                <label class="label2" for="full_name_principal_emp"></label>
                </td>
    		</tr>
            <tr>
					<td>1.(b)</td>
					<td>Gender</td>
					<td><?php echo $gender;?></td>
                    <td align="center"><input type="checkbox" name="gender_pe" id="gender_pe" class="verified_alc_clra_yn check2"  <?php echo $gender_pe_ck; ?> />
                <label class="label2" for="full_name_principal_emp"></label>
				</tr>
				
				<tr>
					<td>1.(c)</td>
                <td>Address Of the principal employer </td>
                <td><?php echo $content['address_principal_emp'].'<br/>'.$pe_address;?></td>
                <td align="center"><input type="checkbox" id="address_principal_emp" name="address_principal_emp" class="verified_alc_clra_yn check2" <?php echo $address_principal_emp_ck;?> />
                <label class="label2" for="address_principal_emp"></label>
                </td>
            </tr>
            <tr>
            	<td colspan="4" style="font-weight:700; text-transform:uppercase; font-size:15px;">2. Full name of the Manager or Person Responsible for the Supervision and control of the Establishment</td>
            </tr>
            <tr>
                <td>2.(a)</td>
                <td>Full name of the manager or person responsible for the supervision and control of the establishment </td>
                <td><?php echo $content['full_name_manager'];?></td>
                <td align="center"><input type="checkbox" id="full_name_manager" class="verified_alc_clra_yn check2"  <?php echo $full_name_manager_ck; ?> />
                <label class="label2" for="full_name_manager"></label>
                </td>
            </tr>
            <tr>
                <td>2.(b)</td>
                <td>Address of the manager or person responsible for the supervision  and control of the establishment </td>
                <td><?php echo $content['address_manager'].'<br/>'.$manager_address;?></td>
                <td align="center"><input type="checkbox" id="address_manager" class="verified_alc_clra_yn check2" <?php echo $address_manager_ck;?> />
                <label class="label2" for="address_manager"></label>
                </td>
            </tr>
            <tr>
            	<td colspan="4" style="font-weight:700; text-transform:uppercase; font-size:15px;">3. Name and location of the Establishment</td>
            </tr>
            <tr>
                <td>3.(a)</td>
                <td >Name of the Establishment </td>
                <td><?php echo $content['e_name'];?></td>
                <td align="center">
                <input type="checkbox" id="e_name" class="verified_alc_clra_yn check2"  <?php echo $e_name_ck; ?> /> 
                <label class="label2" for="e_name"></label>
                </td> 
            </tr>
            <tr>
                <td>3.(b)</td>
                <td>Establishment Type </td>
                <td><?php echo ucfirst($content['est_type']);?></td>
                <td align="center">
                <input type="checkbox" id="est_type" class="verified_alc_clra_yn check2" <?php echo $estType_ck;?> />
                <label class="label2" for="est_type"></label>
            </tr>
            <tr>
                <td>3.(b)</td>
                <td>Location of the establishment</td>
                <td><?php echo $content['loc_e_name'].'<br/>'.$establishment_address;?></td>
                <td align="center">
                <input type="checkbox" id="loc_e_name" class="verified_alc_clra_yn check2" <?php echo $loc_e_name_ck;?> />
                <label class="label2" for="loc_e_name"></label>
                </td>
            </tr>
            <tr>
            	<td colspan="4" style="font-weight:700; text-transform:uppercase; font-size:15px;">4. Registered Office address of the Establishment</td>
            </tr>
            <tr>
                <td>4.(a)</td>
                <td>Postal address of the establishment </td>
                <td> <?php echo $content['e_postal_address'].'<br/>'.$postal_address;?></td>
                <td align="center"><input type="checkbox" id="e_postal_address" class="verified_alc_clra_yn check2" <?php echo $e_postal_address_ck;?> />
                <label class="label2" for="e_postal_address"></label>
                </td>
            </tr>
            <tr>
                <td colspan="4" style="font-weight:700; text-transform:uppercase; font-size:15px;">5. Nature of Work Carried on in the Establishment</td>
            </tr>
            <tr>
                <td>5.</td>
                <td>Nature of work carried on in the establishment </td>
                <td><?php echo $natureOfWrkString;?></td>
                <td align="center"><input type="checkbox" id="e_nature_of_work" class="verified_alc_clra_yn check2" <?php echo $e_nature_of_work_ck;?> />
                <label class="label2" for="e_nature_of_work"></label>
                </td>
            </tr>
            <tr>
                <td>5.(a)</td>
                <td>Maximum Number of workmen employed directly on any day in the establishment</td>
                <td><?php echo $content['max_num_wrkmen'];?></td>
                <td align="center"><input type="checkbox" id="max_num_wrkmen" class="verified_alc_clra_yn check2" <?php echo $max_num_wrkmen_ck;?> />
                <label class="label2" for="max_num_wrkmen"></label>
                </td>
            </tr>
            <tr>
                <td>5.(b)</td>
                <td>Number of workmen engaged as permanent/regular Workmen </td>
                <td><?php echo $content['e_num_of_workmen_per_or_reg'];?></td>
                <td align="center"><input type="checkbox" id="e_num_of_workmen_per_or_reg" class="verified_alc_clra_yn check2" <?php echo $e_num_of_workmen_per_or_reg_ck;?> />
                <label class="label2" for="e_num_of_workmen_per_or_reg"></label>
                </td>
            </tr>
    <tr>
     <td>5.(c)</td>
    <td>Number of workmen engaged as temporary/regular workmen </td>
    <td><?php echo $content['e_num_of_workmen_temp_or_reg'];?></td>
    <td align="center"><input type="checkbox" id="e_num_of_workmen_temp_or_reg" class="verified_alc_clra_yn check2" <?php echo $e_num_of_workmen_temp_or_reg_ck;?>  />
    <label class="label2" for="e_num_of_workmen_temp_or_reg"></label>
    </td>
    </tr>
    <tr>
    <td>5.(d)</td>
    <td>Whether the workmen employed/intended to be employment by the contractor perform the same or similar kind of work as the workmen employed directly by the principal employer (if yes, please give here information as detailed below) </td>
    <td><?php if($content['workmen_if_same_similar_kind_of_work']=="0"){ echo "No";}else{ echo "Yes";} ?>
    </td>
    <td align="center"><input type="checkbox" id="workmen_if_same_similar_kind_of_work" class="verified_alc_clra_yn check2" <?php echo $workmen_if_same_similar_kind_of_work_ck;?> />
    <label class="label2" for="workmen_if_same_similar_kind_of_work"></label>
    </td>
    </tr>
    <tr>
    <td>5.d.i)</td>
    <td>A complete job description of the contract labour </td>
    <td><?php echo $content['con_lab_job_desc'];?></td>
    <td align="center"><input type="checkbox" id="con_lab_job_desc" class="verified_alc_clra_yn check2" <?php echo $con_lab_job_desc_ck;?> />
    <label class="label2" for="con_lab_job_desc"></label>
    </td>
    </tr>
    <tr>
    <td>5.d.ii)</td>
    <td>Wage rates and other cash benefits paid/to be paid </td>
    <td><?php echo $content['con_lab_wage_rate_other_benefits'];?></td>
    <td align="center"><input type="checkbox" id="con_lab_wage_rate_other_benefits" class="verified_alc_clra_yn check2" <?php echo $con_lab_wage_rate_other_benefits_ck;?> />
    <label class="label2" for="con_lab_wage_rate_other_benefits"></label>
    </td>
    </tr>
    <tr>
    <td>5.d.iii)</td>
    <td>Category/designation/nomenclature of the job </td>
    <td><?php echo $content['con_lab_cat_desig_nom'];?></td>
    <td align="center"><input type="checkbox" id="con_lab_cat_desig_nom" class="verified_alc_clra_yn check2" <?php echo $con_lab_cat_desig_nom_ck;?> />
    <label class="label2" for="con_lab_cat_desig_nom"></label>
    </td>
    </tr>
    <tr>
    <td>5.e)</td>
    <td>Settlement or award or judgement or minimum wages (if any applicable in the establishment) </td>
    <td><?php echo $content['e_settlement_award_judgement_min_wage'];?></td>
    <td align="center"><input type="checkbox" id="e_settlement_award_judgement_min_wage" class="verified_alc_clra_yn check2" <?php echo $e_settlement_award_judgement_min_wage_ck;?> />
    <label class="label2" for="e_settlement_award_judgement_min_wage"></label>
    </td>
    </tr>
    <tr>
    <td>6.</td>
    <td style="font-weight:700; font-size:16px;">Maximum number of contract labour to be employed on any day through each contractor </td>
    <td>
    <?php echo '<strong>'.$content['e_any_day_max_num_of_workmen'].'&nbsp; <font color="#006600">[FEES IS CALCULATED BASED ON THIS VALUE]</font></strong>';?>
    <button type="button" class="btn btn-info pull-right" data-toggle="modal" data-target="#myModalFeeschat"><i class="fa fa-info-circle"></i>&nbsp;Fees Chart</button>
    </td>
    <td align="center"><input type="checkbox" id="e_any_day_max_num_of_workmen" class="verified_alc_clra_yn check2" <?php echo $e_any_day_max_num_of_workmen_ck;?> />
    <label class="label2" for="e_any_day_max_num_of_workmen"></label>
    </td>
    </tr>
    
    <tr>
    <td colspan="4" style="font-weight:700; text-transform:uppercase; font-size:15px;">7. Uploaded documents </td>
    </tr>
   <?php
   if($content['eodb_app_id'] != 0){
   		$enc_doc_array = array('TL'=> '','PD'=> '','AOA'=> '','MOA'=> '','MOC'=>'','FL'=> '','AC'=> '','PC'=> '','BB'	=> '','SC'	=> '','IC'	=> '','AP'=> '','ODSC'=> '','CR'=> '','WO'=> '','ORC'=> '','CH'=>'');
		$pdficonlink = '<img title="View Documents" alt="View Documents" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/pdf.png"> &nbsp;';
		$query_result = db_select('l_encrypted_uploaded_documents','leud')->fields('leud',array())->condition('application_id',$var_id)->condition('act_id','1')->condition('status','1')->execute();
		$document_type_arr=array();
		if($query_result->rowCount() > 0){
			foreach($query_result->fetchAll() as $data ){			
				$enc_doc_array[$data->document_type_code]['content'] = 'view_documents/'.encryption_decryption_fun('encrypt',$data->id);
			}
		}
   }
    $doc_id='1';
    $file='file_'.$doc_id; 
	// print_r($content_docs); echo $content_docs[0]->trade_license_file; die;
	$docs = $content_docs[0];
    // foreach($content_docs as $docs){    
    ?>
    <tr>
     <?php	
	if($enc_doc_array['TL']['content'] !=''){ ?>
    <td>i.)</td>
    <td>Trade license</td>	
	<td>
    <?php // $output .=  '<td>'.l('<i class="fa fa-file fa-lg"></i>', 'documents-view/'.encryption_decryption_fun('encrypt', $contractor_user_id).'/'.encryption_decryption_fun('encrypt', $serial).'/'.encryption_decryption_fun('encrypt', 'AP'), array('html' => TRUE,'attributes'=>array('target'=>'_blank','class'=>array('#pdf-img')))).'</td>'; ?>    
    
    
    <a target="_blank" title="Click here to Trade License document." href="<?php echo  $GLOBALS['base_url'];?>/documents-view/<?php echo encryption_decryption_fun('encrypt', $content['user_id']);?>/<?php echo encryption_decryption_fun('encrypt', $var_id);?>/<?php echo encryption_decryption_fun('encrypt', 'TL');?>"><i class="fa fa-file fa-lg"></i></a>&nbsp;
    
    <!--<a target="_blank" title="Click here to Trade License document." href="<?php // echo  $GLOBALS['base_url'];?>/<?php // echo $enc_doc_array['TL']['content']; ?>"><i class="fa fa-file fa-lg"></i></a>&nbsp;-->
    
    <span class="viewfile_popup" id="trade_license" title="Click here to view Trade License"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
    
    <div id="view_trade_license" style="display:none;" title="Trade License"><iframe src="<?php echo  $GLOBALS['base_url'];?>/<?php echo $enc_doc_array['TL']['content'];?>" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>&nbsp;&nbsp;
    </td>
    <td align="center">
    <input type="checkbox" id="form_1_clra_signed_pdf_file" class="verified_alc_clra_yn check2" <?php echo $form_1_clra_signed_pdf_file_ck;?> />
    <label class="label2" for="form_1_clra_signed_pdf_file"></label>
    </td>
	<?php
    }elseif(!empty($docs->trade_license_file)){
    ?>
    <tr>
    <td>i.)</td>
    <td>Trade license</td>
    <td>
    <a target="_blank" href="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/trade_license/<?php echo $docs->trade_license_file; ?>"><i class="fa fa-file fa-lg"></i></a>&nbsp;
    
    <span class="viewfile_popup" id="trade_license"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
    
    <div id="view_trade_license"  title="Trade License" style="display:none;"><iframe src="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/trade_license/<?php echo $docs->trade_license_file;?>" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>
    </td>
    <td align="center">
   <input type="checkbox" id="trade_license_file" class="verified_alc_clra_yn check2" <?php echo $trade_license_file_ck;?> />
    <label class="label2" for="trade_license_file"></label>
    </td>
    
    </tr>
    <?php }else{ ?>    
    <tr>
    <td>i.)</td>
    <td>Trade license</td>
    <td>No document uploaded</td>
    <td align="center">
    <input type="checkbox" id="trade_license_file" class="verified_alc_clra_yn check2" <?php echo $trade_license_file_ck;?> />
    <label class="label2" for="trade_license_file"></label>
    </td>
    </tr>
    <?php  
	}
	
    if($enc_doc_array['AOA']['content'] !=''){ // AMA?>
    <tr>
	 <td>ii.)</td>
    <td>Articles of Association and Memorandum of Association/Partnership Deed</td>
	<td>
    <a target="_blank" title="Click here to Trade License document." href="<?php echo  $GLOBALS['base_url'];?>/documents-view/<?php echo encryption_decryption_fun('encrypt', $content['user_id']);?>/<?php echo encryption_decryption_fun('encrypt', $var_id);?>/<?php echo encryption_decryption_fun('encrypt', 'AOA');?>"><i class="fa fa-file fa-lg"></i></a>&nbsp;
    
    <!--<a target="_blank" title="Click here to download documents" href="<?php // echo  $GLOBALS['base_url'];?>/<?php // echo $enc_doc_array['AOA']['content']; ?>"><i class="fa fa-file fa-lg"></i></a>&nbsp;-->
    
    <span class="viewfile_popup" id="article_of_assoc_file_1" title="Click here to view Trade License"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
    
    <div id="view_article_of_assoc_file_1" style="display:none;" title="Articles of Association and Memorandum of Association/Partnership Deed"><iframe src="<?php echo  $GLOBALS['base_url'];?>/<?php echo $enc_doc_array['AOA']['content'];?>" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>&nbsp;&nbsp;
    </td>
    <td align="center">
    <input type="checkbox" id="article_of_assoc_file" class="verified_alc_clra_yn check2" <?php echo $article_of_assoc_file_ck;?> />
    <label class="label2" for="article_of_assoc_file"></label>
    </td>
	<?php
    }elseif(!empty($docs->article_of_assoc_file)){
    $doc_id = $doc_id+1; 
    $file='file_'.$doc_id;
    ?>
    <tr>
    <td>ii.)</td>
    <td>Articles of Association and Memorandum of Association/Partnership Deed</td>
    <td><a target="_blank" title="Click here to download documents" href="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/article_of_assoc/<?php echo $docs->article_of_assoc_file; ?>"><i class="fa fa-file fa-lg"></i></a>&nbsp;
    
    <span class="viewfile_popup" id="article_of_assoc_file_1"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
    
    <div id="view_article_of_assoc_file_1"  title="Articles of Association and Memorandum of Association/Partnership Deed"  style="display:none;"><iframe src="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/article_of_assoc/<?php echo $docs->article_of_assoc_file;?>" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div></td>
    <td align="center">
    <input type="checkbox" id="article_of_assoc_file" class="verified_alc_clra_yn check2" <?php echo $article_of_assoc_file_ck;?> />
    <label class="label2" for="article_of_assoc_file"></label>
    </td>
    </tr>
    <?php  }
	else { ?>
    <tr>
    <td>ii.)</td>
    <td>Articles of Association and Memorandum of Association/Partnership Deed</td>
    <td>No document uploaded</td>
    <td align="center">
    <input type="checkbox" id="article_of_assoc_file" class="verified_alc_clra_yn check2" <?php echo $article_of_assoc_file_ck;?> />
    <label class="label2" for="article_of_assoc_file"></label>
    </td>
    </tr> 
    <?php  }
	
    if(isset($enc_doc_array['ODSC']['content']) && $enc_doc_array['ODSC']['content'] !=''){ ?>
    <tr>
	 <td>iii.)</td>
    <td>Any other document in support of correctness of the particulars mentioned in the application(if required)</td>
	<td>
    <!--<a target="_blank" title="Click here to download documents" href="<?php // echo  $GLOBALS['base_url'];?>/<?php // echo $enc_doc_array['ODSC']['content']; ?>"><i class="fa fa-file fa-lg"></i></a>&nbsp;-->
    
    <a target="_blank" title="Click here to Trade License document." href="<?php echo  $GLOBALS['base_url'];?>/documents-view/<?php echo encryption_decryption_fun('encrypt', $content['user_id']);?>/<?php echo encryption_decryption_fun('encrypt', $var_id);?>/<?php echo encryption_decryption_fun('encrypt', 'ODSC');?>"><i class="fa fa-file fa-lg"></i></a>&nbsp;
    
    <span class="viewfile_popup" id="abcd" title="Click here to view document in support of correctness of the application"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
    <div id="view_abcd" style="display:none;" title="Any other document in support of correctness of the particulars mentioned in the application if required"><iframe src="<?php echo  $GLOBALS['base_url'];?>/<?php echo $enc_doc_array['ODSC']['content'];?>" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>&nbsp;&nbsp;
    </td>
    <td align="center">
   <input type="checkbox" id="memorandum_of_cert_file" class="verified_alc_clra_yn check2" <?php echo $memorandum_of_cert_file_ck;?> />
    <label class="label2" for="memorandum_of_cert_file"></label>
    </td>
	
    <?php
    
	}
	elseif(!empty($docs->memorandum_of_cert_file)){
    $doc_id=  $doc_id+1; 
    $file='file_'.$doc_id;
    ?>
    <tr>
    <td>iii.)</td>
    <td>Any other document in support of correctness of the particulars mentioned in the application if required</td>
    <td><a target="_blank" title="Click here to download documents." href="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/memorandum_of_cert/<?php echo $docs->memorandum_of_cert_file; ?>"><i class="fa fa-file fa-lg"></i></a>&nbsp;
    
    <span class="viewfile_popup" id="abcd"  title="Click here to view document in support of correctness of the application"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
    
    <div id="view_abcd" style="display:none;"  title="Any other document in support of correctness of the particulars mentioned in the application if required"><iframe src="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/memorandum_of_cert/<?php echo $docs->memorandum_of_cert_file; ?>" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div></td>
    <td align="center">
    <input type="checkbox" id="memorandum_of_cert_file" class="verified_alc_clra_yn check2" <?php echo $memorandum_of_cert_file_ck;?> />
    <label class="label2" for="memorandum_of_cert_file"></label>
    </td>
    </tr>
    <?php }
	else{ ?>
    <tr>
    <td>iii.)</td>
    <td>Any other document in support of correctness of the particulars mentioned in the application if required</td>
    <td>No document uploaded</td>
    <td align="center">
    <input type="checkbox" id="memorandum_of_cert_file" class="verified_alc_clra_yn check2" <?php echo $memorandum_of_cert_file_ck;?> />
    <label class="label2" for="memorandum_of_cert_file"></label>
    </td>
    </tr> 
    
    <?php }  
	
    if(isset($enc_doc_array['CR']['content']) && $enc_doc_array['CR']['content'] !=''){ ?>
    <tr>
	 <td>iv.)</td>
    <td>Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc.</td>
	<td>
    
   <!-- <a target="_blank" title="Click here to view other certificates of registration" href="<?php // echo  $GLOBALS['base_url'];?>/<?php // echo $enc_doc_array['CR']['content']; ?>"><i class="fa fa-file fa-lg"></i></a>&nbsp;-->
   
   <a target="_blank" title="Click here to Trade License document." href="<?php echo  $GLOBALS['base_url'];?>/documents-view/<?php echo encryption_decryption_fun('encrypt', $content['user_id']);?>/<?php echo encryption_decryption_fun('encrypt', $var_id);?>/<?php echo encryption_decryption_fun('encrypt', 'CR');?>"><i class="fa fa-file fa-lg"></i></a>&nbsp;
    
    <span class="viewfile_popup" id="abcd" title="Click here to view document in support of correctness of the application"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
    
    <div id="view_abcd" style="display:none;" title="Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc."><iframe src="<?php echo  $GLOBALS['base_url'];?>/<?php echo $enc_doc_array['CR']['content'];?>" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>&nbsp;&nbsp;
     </td>
    <td align="center">
  <input type="checkbox" id="partnership_deed_file" class="verified_alc_clra_yn check2" <?php echo $partnership_deed_file_ck;?> />
    <label class="label2" for="partnership_deed_file"></label>
    </td>
	 <?php
    }
	elseif(!empty($docs->partnership_deed_file)){
    $doc_id=  $doc_id+1; 
    $file='file_'.$doc_id;
    ?>
    <tr>
    <td>iv.)</td>
    <td>Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc.</td>
    <td><a target="_blank" title="Click here to download documents." href="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/partnership_deed/<?php echo $docs->partnership_deed_file; ?>"><i class="fa fa-file fa-lg"></i></a>&nbsp;
    
    <span class="viewfile_popup" id="abcd"  title="Click here to view other certificates of registration"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>    
    <div id="view_abcd" style="display:none;" title="Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc."><iframe src="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/partnership_deed/<?php echo $docs->partnership_deed_file; ?>" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div></td>
    <td align="center">
    <input type="checkbox" id="partnership_deed_file" class="verified_alc_clra_yn check2" <?php echo $partnership_deed_file_ck;?> />
    <label class="label2" for="partnership_deed_file"></label>
    </td>
    </tr>
    <?php } 
	else{ ?>
    <tr>
    <td>iv.)</td>
    <td>Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc.</td>
    <td>No document uploaded</td>
    <td align="center">
    <input type="checkbox" id="partnership_deed_file" class="verified_alc_clra_yn check2" <?php echo $partnership_deed_file_ck;?> />
    <label class="label2" for="partnership_deed_file"></label>
    </td>
    </tr>
    <?php	  
    } 
    
    if(isset($enc_doc_array['FL']['content']) && $enc_doc_array['FL']['content'] !=''){ ?>
    <tr>
	<td>v.)</td>
    <td>Factory License if any</td>
	<td>
    <!--<a target="_blank" title="Click here to view other certificates of registration" href="<?php // echo  $GLOBALS['base_url'];?>/<?php // echo $enc_doc_array['FL']['content']; ?>"><i class="fa fa-file fa-lg"></i></a>&nbsp;-->
    
    <a target="_blank" title="Click here to Trade License document." href="<?php echo  $GLOBALS['base_url'];?>/documents-view/<?php echo encryption_decryption_fun('encrypt', $content['user_id']);?>/<?php echo encryption_decryption_fun('encrypt', $var_id);?>/<?php echo encryption_decryption_fun('encrypt', 'FL');?>"><i class="fa fa-file fa-lg"></i></a>&nbsp;
    
    <span class="viewfile_popup" id="factory_license_file_pdf" title="Click here to view Factory License"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
     <div id="view_factory_license_file_pdf" style="display:none;" title="Factory License"><iframe src="<?php echo  $GLOBALS['base_url'];?>/<?php echo $enc_doc_array['FL']['content'];?>" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>&nbsp;&nbsp;
    </td>
    <td align="center">
  <input type="checkbox" id="partnership_deed_file" class="verified_alc_clra_yn check2" <?php echo $partnership_deed_file_ck;?> />
    <label class="label2" for="partnership_deed_file"></label>
    </td>
	<?php
    }
	elseif(!empty($docs->factory_license_file)){
    $doc_id=  $doc_id+1; 
    $file='file_'.$doc_id;
    ?>
    <tr>
    <td>v.)</td>
    <td>Factory License if any</td>
    <td>    
    <a target="_blank" title="Click here to download documents." href="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/FactoryLicense/<?php echo $docs->factory_license_file;?>"><i class="fa fa-file fa-lg"></i></a>&nbsp;
    <span class="viewfile_popup" id="factory_license_file_pdf" title="Click here to view Factory License"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
    <div id="view_factory_license_file_pdf" style="display:none;" title="Factory License" ><iframe src="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/FactoryLicense/<?php echo $docs->factory_license_file;?>" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>
     </td>
    <td align="center">
    <input type="checkbox" id="factory_license_file" class="verified_alc_clra_yn check2" <?php echo $factory_license_file_ck;?> />
    <label class="label2" for="factory_license_file"></label>
    </td>
    </tr>
     <?php } 
	 else{?>
    <tr>
    <td>v.)</td>
    <td>Factory License if any</td>
    <td>No document uploaded</td>
    <td align="center">
    <input type="checkbox" id="factory_license_file" class="verified_alc_clra_yn check2" <?php echo $factory_license_file_ck;?> />
    <label class="label2" for="factory_license_file"></label>
    </td>
    </tr>
    <?php }
	
    if(isset($enc_doc_array['FI']['content']) && $enc_doc_array['FI']['content'] !=''){ ?>
    <tr>
	<td>vi.)</td>
    <td>FORM-I</td>
	<td>
    <?php /*?><a target="_blank" title="Click here to view other certificates of registration" href="<?php echo  $GLOBALS['base_url'];?>/<?php echo $enc_doc_array['FI']['content']; ?>"><i class="fa fa-file fa-lg"></i></a>&nbsp;<?php */?>
    
    <a target="_blank" title="Click here to Trade License document." href="<?php echo  $GLOBALS['base_url'];?>/documents-view/<?php echo encryption_decryption_fun('encrypt', $content['user_id']);?>/<?php echo encryption_decryption_fun('encrypt', $var_id);?>/<?php echo encryption_decryption_fun('encrypt', 'FI');?>"><i class="fa fa-file fa-lg"></i></a>&nbsp;
    
    <span class="viewfile_popup" id="factory_license_file_pdf" title="Click here to view Factory License"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
     <div id="view_factory_license_file_pdf" style="display:none;" title="Factory License"><iframe src="<?php echo  $GLOBALS['base_url'];?>/<?php echo $enc_doc_array['FL']['content'];?>" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>&nbsp;&nbsp;
    </td>
    <td align="center">
  <input type="checkbox" id="partnership_deed_file" class="verified_alc_clra_yn check2" <?php echo $partnership_deed_file_ck;?> />
    <label class="label2" for="partnership_deed_file"></label>
    </td>
	<?php
    }
	elseif(!empty($docs->form_1_clra_signed_pdf_file)){
    $doc_id=  $doc_id+1; 
    $file='file_'.$doc_id;
    ?>
    
    <tr> 
    <td>vi.)</td>
    <td>FORM-I</td>
    <td>
    <a target="_blank" title="Click here to signed form-I document." href="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/FORM-I/<?php echo $docs->form_1_clra_signed_pdf_file; ?>"><i class="fa fa-file fa-lg"></i></a>&nbsp;
    
    <span class="viewfile_popup" id="form_1_clra_signed_pdf_file_1" title="Click here to view FORM-I"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
    
    <div id="view_form_1_clra_signed_pdf_file_1" style="display:none;" title="FORM-I"><iframe src="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/FORM-I/<?php echo $docs->form_1_clra_signed_pdf_file;?>" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>&nbsp;&nbsp;
    
    <?php echo l(t('<span class="badge bg-aqua">&nbsp;Click here to view & download system generated FORM-I&nbsp;<i class="fa fa-download" aria-hidden="true"></i></span>'),'generate-pdf-applicant/'.encryption_decryption_fun('encrypt', $content['id']).'/'.encryption_decryption_fun('encrypt', $content['user_id']), array('html' => TRUE,'attributes'=> array('target'=>'_blank'))); ?>
    </td>
    <td align="center">
    <input type="checkbox" id="form_1_clra_signed_pdf_file" class="verified_alc_clra_yn check2" <?php echo $form_1_clra_signed_pdf_file_ck;?> />
    <label class="label2" for="form_1_clra_signed_pdf_file"></label>
    </td>
    
    </tr>
    <?php }
	else{?>
    <tr> 
    <td>vi.)</td>
    <td>FORM-I</td>
    <td>Form I will be uploaded after fees payment</td>
    <td align="center">
    <input type="checkbox" id="form_1_clra_signed_pdf_file" class="verified_alc_clra_yn check2" <?php echo $form_1_clra_signed_pdf_file_ck;?> />
    <label class="label2" for="form_1_clra_signed_pdf_file"></label>
    </td>
    </tr>
    <?php }?> 
    
    <?php 
    if(!empty($docs->backlog_clra_registration_certificate_file)){
    $doc_id=  $doc_id+1; 
    $file='file_'.$doc_id;
    ?>
    <tr>
    <td>vii.)</td>
    <td>Previous Registration Certificate</td>
    <td><a target="_blank" title="Click here to view old registration certificate." href="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/backlog_clra_registration_certificate/<?php echo $docs->backlog_clra_registration_certificate_file; ?>"><i class="fa fa-file fa-lg"></i></a>&nbsp;
    
    
    <span class="viewfile_popup" id="backlog_clra_registration_certificate_file_1" title="Click here to view Previous Registration Certificate"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
    
    <div id="view_backlog_clra_registration_certificate_file_1" style="display:none;" title="Previous Registration Certificate"><iframe src="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/backlog_clra_registration_certificate/<?php echo $docs->backlog_clra_registration_certificate_file;?>" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>
    
    </td>
    <td align="center">
    <input type="checkbox" id="backlog_clra_registration_certificate_file" class="verified_alc_clra_yn check2" <?php echo $backlog_clra_registration_certificate_file_ck;?> />
    <label class="label2" for="backlog_clra_registration_certificate_file"></label>
    </td>
    
    </tr>
    <?php }
	else{?>
    <tr>
    <td>vii.)</td>
    <td>Previous Registration Certificate</td>
    <td>For Previous Registered Applicant, this certificate should be uploaded</td> 
    <td align="center">
    <input type="checkbox" id="backlog_clra_registration_certificate_file" class="verified_alc_clra_yn check2" <?php echo $backlog_clra_registration_certificate_file_ck;?> />
    <label class="label2" for="backlog_clra_registration_certificate_file"></label>
    </td></tr>
    <?php 
    }   
     
    if(!empty($content['certificates_fid'])){
		$upload_certificates_id = db_select('file_managed','fm');
		$upload_certificates_id-> fields('fm',array());
		$upload_certificates_id-> condition('fid', $content['certificates_fid']);		
		$upload_certificates_file = $upload_certificates_id->execute()->fetchAssoc();
    
		if(!empty($upload_certificates_file)){ 
			$url							 = explode('//',$upload_certificates_file['uri']);
			$upload_certificates_file_url	 = $url[1];
			$upload_certificates_file_name = $upload_certificates_file['filename'];
		}
    	$download =	'<a title="Click here to download Certificate" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$upload_certificates_file_url.'" target="_blank"><i class="fa fa-file fa-lg"></i></a>&nbsp;';
    ?>		
    <tr>
        <td>viii.)</td>
        <td>Registration Certificate</td>
        <td><?php echo $download;?>  &nbsp;&nbsp;<strong>[<?php echo $content['registration_number'];?>]</strong></td> 
        <td align="center">Issued By ALC</td>
    </tr>
    <?php
    }
	
	if($content['status']=='I' && $content['certificates_fid']=='' && $content['registration_number']!=''){
		
		//download-pdf-clra-registration/'.time().'/'.encryption_decryption_fun('encrypt', $data->id).'/'.encryption_decryption_fun('encrypt', $data->user_id)
		
		$download =	'<a title="Click here to download Certificate" href="'.$GLOBALS['base_url'].'/download-pdf-clra-registration/'.time().'/'.encryption_decryption_fun('encrypt', $content['id']).'/'.encryption_decryption_fun('encrypt', $content['user_id']).'" target="_blank"><i class="fa fa-file fa-lg"></i></a>&nbsp;';?>
    	<tr>
            <td>viii.)</td>
            <td>Registration Certificate</td>
            <td><?php echo $download;?>  &nbsp;&nbsp;<strong>[<?php echo $content['registration_number'];?>]</strong></td> 
            <?php if($content['deemed']=='Y'){?>
            <td align="center">Deemed Approved</td>
            <?php }else{?>
            <td align="center">Issued By ALC</td>
            <?php } ?>
    	</tr>
		
	<?php }

    
    /* Payment Details Start */
    if($content['payment_mode'] == 'G'){	 
		$govt_pay_query = db_select('l_govt_payment', 'lgp');
		$govt_pay_query->fields('lgp', array());
		$govt_pay_query->condition('lgp.application_id', $content['id']);		
		$govt_pay_query_result = $govt_pay_query->execute();
    
    
		if($govt_pay_query_result->rowCount() > 0 ){		
			$govt_paydata = $govt_pay_query_result->fetchAssoc();
				
			$tv_number = 'Not available';
			$challan_number = 'Not available';
			$total_amount = 'Not available';
			$challan_date = 'Not available';
				
			if(!empty($govt_paydata['tv_number'])) $tv_number = $govt_paydata['tv_number'];
			if(!empty($govt_paydata['challan_number'])) $challan_number = $govt_paydata['challan_number'];
			if(!empty($govt_paydata['total_amount'])) $total_amount = $govt_paydata['total_amount'];
			if(!empty($govt_paydata['challan_date'])) $challan_date = $govt_paydata['challan_date'];
		}
    
		$payment_details = '<u>Government Payment[Head to Head transfer through treasury]</u><br>';
		$payment_details .= 'Transaction Voucher Number:<span class="color_orange">'.$tv_number.'</span><br>';
		$payment_details .= 'Challan Number:<span class="color_orange">'.$challan_number.'</span><br>';
		$payment_details .= 'Challan Date:<span class="color_orange">'.date('dS M Y', strtotime($challan_date)).'</span><br>';
		$payment_details .= 'Total amount:<span class="color_orange">'.number_format($total_amount,2).'</span><br>';
		$payment_details .= 'Challan Details:'.l('<i class="fa fa-file fa-lg"></i>', '/sites/default/files/upload/challan_upload/'.$govt_paydata['challan_file'], array('html' => TRUE, 'title' => 'Click here to view challan'));

    }else if($content['payment_mode'] == 'O'){

		$offline_pay_query = db_select('l_offline_payment_info', 'lopi');								
		$offline_pay_query->fields('lopi', array('grn_number', 'approved_by'));
		$offline_pay_query->condition('lopi.application_id', $content['id']);
		$offline_pay_query->condition('lopi.act_id', 1);								
		$offline_pay_query = $offline_pay_query->execute();
		
		$offline_pay_result = $offline_pay_query->fetchObject();
		
		$payment_details = '<u>Manual Payment</u><br>';
		$payment_details .= 'GRN NO.:<span class="color_orange">'.$offline_pay_result->grn_number.'&nbsp;'.l('Click here view details', 'https://wbfin.wb.nic.in/GRIPS/grn_status.do', array('attributes' => array('title' =>'Click here to view details', 'target' => '_blank'))).'</span><br>';
		$payment_details .= 'Total Amount:<span class="color_orange"> &#8377; '.$content['finalfees'].'</span><br>';
		$payment_details .= 'Challan:<span class="color_orange">No available</span><br>';

    }else{		

		$transaction_details  			= db_select('l_principle_epayments_receive_data', 'lpd');
		$transaction_details			->leftJoin('l_principle_epayments_data', 'lped', 'lped.identification_no = lpd.transaction_id');
		$transaction_details			->fields('lped', array('identification_no', 'application_id'));
		$transaction_details			->fields('lpd', array());
		$transaction_details			->condition('lped.act_id', '1');
		$transaction_details			->condition('lped.application_id', $content['id']);		
		$trans_details_result 			= $transaction_details->execute();
    
		$bank_code = 'Not available';
		$bankTransactionID = 'Not available';
		$total_amount = 'Not available';
		$challan_fid_date = 'Not available';
		$payment_status = 'Not available';
    
		if($trans_details_result-> rowCount() > 0 ){			
			$transaction_det	= $trans_details_result->fetchAssoc();
			$bankTransactionID	= $transaction_det['transaction_id'];
			$grn_number			= $transaction_det['challanrefid'];
			$challan_fid_date	= !empty($transaction_det['challanrefid_date']) ? $transaction_det['challanrefid_date'] : '';
			$total_amount		= number_format($transaction_det['challanamount'], 2);
			$bank_code			= $transaction_det['bank_cd'];
			if(!empty($transaction_det['banktransactionstatus']) && $transaction_det['banktransactionstatus'] == 'Success') 
				$payment_status = '<span class="color_green">'.$transaction_det['banktransactionstatus'].'</span>';
		}else if(!empty($transaction_det['banktransactionstatus'])) {
			$payment_status 	= '<span class="color_red">'.$transaction_det['banktransactionstatus'].'</span>';
		}
    
		$payment_details = ' <b><u> Grips Payment [ Online/Counter ]</u> </b><br/>';
		$payment_details .= 'GRN: <span class="color_orange">'.$grn_number.'</span><br>';
		$payment_details .= 'IFSC Code: <span class="color_orange">'.$bank_code.'</span><br>';
		$payment_details .= 'Bank Transaction Id: <span class="color_orange">'.$bankTransactionID.'</span><br>';
		$payment_details .= 'Total Amount: <span class="color_orange"> &#8377;'.$total_amount.'</span><br>';
		$payment_details .= 'Transaction Date: <span class="color_orange">'.$challan_fid_date.'</span><br>';
		$payment_details .= 'Transaction Status: '.$payment_status;
    }
    
    /* Payment Details End */ 
    
    if(!empty($content['backlog_id'])){
		if($get_bklg_tbl_res_data->fees < $content['finalfees']){
		$present_payble_amount = $content['finalfees'] - $get_bklg_tbl_res_data->fees;
    }else{
    	$present_payble_amount = 0;
    }
    }else{
    	$present_payble_amount = $content['finalfees'];
    }
    
    
    
    ?> 
    <tr>
        <td colspan="4" style="font-weight:700; text-transform:uppercase; font-size:15px;">8. Payment Details</td>
    </tr>
    <tr>
     <td>8.i)</td>
    <td><span style="color:#f00;">Payable Amount:&nbsp;₹<?php echo $present_payble_amount;?></span>
    <?php if(!empty($content['backlog_id'])){?>
    <br /><font color="#f00"><i>[As it's already manual registrar application so fees may be exempted.]</i></font>
    <?php } ?>
    
    </td>
    <td colspan="2"><?php echo $payment_details;?></td>  	
    </tr>
    
    </table></div>
    </div>
</div>

<div class="box box-primary box-solid">
	<div class="box-header with-border"><i class="ion ion-clipboard"></i>  Trade Union Information
    </div>
    <div class="box-body"><div class="feedback-scroll">
			<table cellpadding="0" cellspacing="0" border="0" width="100%" class="table table-bordered table-responsive">
				<thead>
                <tr>
					<th width="7%">Sl. No.</th>
                    <th width="12%">Registration No.</th>
                    <th>Name</th>
                    <th width="10%">Action</th>
				</tr>
                </thead>
                <tbody>
				<?php
                if(!empty($param_trad)){
                    $x = 0;	
                    foreach($param_trad as $trade_union_data){ 
                        $x++;
                        ?>							 
                        <tr>
                            <td><?php echo $x;?></td>
                            <td><?php echo $trade_union_data->e_trade_union_regn_no;?></td>
                            <td><?php echo $trade_union_data->e_trade_union_name;?></td>
                            <td width="10%"><button type="button" class="btn btn-info pull-center" data-toggle="modal" data-target="#tuMore_<?php echo $trade_union_data->id;?>"><i class="fa fa-info-circle"></i>&nbsp;More</button>
                                <div class="modal fade" id="tuMore_<?php echo $trade_union_data->id;?>" role="dialog">
                                    <div class="modal-dialog">
                                        <div class="box box-primary box-solid">
                                            <div class="box-header">
                                            <button type="button" class="close" data-dismiss="modal">&times;</button>
                                            <h3 class="box-title">Trade Union Details:<?php echo $trade_union_data->e_trade_union_regn_no;?></h3>
                                            </div>                                                    
                                            <div class="modal-body">
                                            <table cellpadding="0" cellspacing="0" border="0" class="table table-striped table-responsive admin-custom-table">
                                                <tr>
                                                    <td width="50%"><strong>Registration of the Trade Union</strong></td>
                                                    <td><?php echo $trade_union_data->e_trade_union_regn_no;?></td>                                         
                                                </tr>                                     
                                                 <tr>
                                                     <td width="50%"><strong>Name of the Trade Union</strong></td>
                                                     <td><?php echo $trade_union_data->e_trade_union_name;?></td>                                         
                                                 </tr>
                                                 <tr>                                         
                                                     <td><strong>Address of the Trade Union</strong></td>
                                                     <td><?php echo $trade_union_data->e_trade_union_address;?></td>
                                                 </tr>
                                             </table>
                                        </div>
                                        <div class="modal-footer">
                                            <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
                                        </div>
                                     </div>      
                                  </div>
                               </div>
                            </td>
                        </tr>			
					<?php
                    }
                }else{ 
                 ?>
                    <tr><td colspan="4" style="text-align:center"> <center>Trade union not added</center></td></tr>
          <?php } ?>
          		</tbody>
				</table> </div>
    </div>               
</div>

<!-- CONTRACTOR INFORMATION START -->

<div class="box box-primary box-solid">
	<div class="box-header with-border"><i class="ion ion-clipboard"></i>  Particulars of Contractors and Contract Labour</h3>
    </div>
	<div class="box-body"><div class="feedback-scroll">
       <table width="100%" border="0" class="table table-bordered table-responsive">
			 <thead>
             <tr>
				<th width="7%">Sl. No.</th>
                <th>Contractor Name</th>
                <!--<th width="10%">Form V</th>-->
                <th width="7%">Contract Labour No.</th>
                <th width="25%">Nature of Work</th>
                <th width="10%">Status</th>
                <th width="10%">Action</th>
			 </tr>
             </thead>
             <tbody>
			<?php
			if(!empty($contractor_info)){
				$y = 0;	
				foreach($contractor_info as $delta_clra_cont => $datafetch_clra_cont){ 
					$y++;
					$nature_of_wrk = "";
					$prefix = '';
					foreach($contractor_nature_wrk as $key=>$val){
						if($datafetch_clra_cont->id == $key){ 
							foreach($val as $value){ 
								$nature_of_wrk .= $prefix.$value;
								$prefix = ', ';
							}
						} 
					}
					
					$contractor_info_address	= miscellaneous_contractor_info_address($application_id, $datafetch_clra_cont->id);
					
					if($datafetch_clra_cont->status != 1){
						$contractor_status = '<span class="badge bg-red"><i class="fa fa-close"></i> Inactive</span>';
					}else{
						$contractor_status = '<span class="badge bg-green"><i class="fa fa-check" aria-hidden="true"></i> Active</span>';
					}
					
					if($datafetch_clra_cont->other_nature_work!= ''){$nature_of_wrk = $datafetch_clra_cont->other_nature_work.', '.$nature_of_wrk;}
  			?>
  			<tr>
    			<td width="7%"><?php echo $y;?></td>
                <td><?php echo $datafetch_clra_cont->name_of_contractor;?></td>
               <!-- <td width="12%"><?php // echo str_pad( $datafetch_clra_cont->id, 7, "0", STR_PAD_LEFT);?></td>-->
                <td width="7%"><?php echo $datafetch_clra_cont->contractor_max_no_of_labours_on_any_day; ?></td>
                <td width="25%"><?php echo $nature_of_wrk; ?></td>                
                <td width="10%"><?php echo $contractor_status;?></td>
                <td width="10%">
                	<button type="button" class="btn btn-info pull-center" data-toggle="modal" data-target="#ContractorMore_<?php echo $datafetch_clra_cont->id;?>"><i class="fa fa-info-circle"></i>&nbsp;More</button>
                    <div class="modal fade" id="ContractorMore_<?php echo $datafetch_clra_cont->id;?>" role="dialog">
                        <div class="modal-dialog">
                        <div class="box box-primary box-solid">
                            <div class="box-header">
                                <button type="button" class="close" data-dismiss="modal">&times;</button>
                                <h3 class="box-title">Particulars of Contractors and Contract Labour:<?php echo $datafetch_clra_cont->name_of_contractor;?></h3>
                            </div>                                                    
                            <div class="modal-body">
                                <table cellpadding="0" cellspacing="0" border="0" class="table table-striped table-responsive admin-custom-table">                                     
                                     <?php
                                     if($content['status'] == 'I'){
										 $serial_no = $datafetch_clra_cont->id;
									 }else{
										 $serial_no = 'Under Process';
									 }
									 if($datafetch_clra_cont->is_from_v == 1){
									 ?>
									<tr>
                                         <td width="50%"><strong>Serial No. of Form-V</strong></td>
                                         <td><?php echo $serial_no; ?></td>                                         
                                     </tr>
                                     <?php
									 }
									 ?>
                                     <tr>
                                         <td width="50%"><strong>Name of the Contractor</strong></td>
                                         <td><?php echo $datafetch_clra_cont->name_of_contractor; ?></td>                                         
                                     </tr>
                                     <tr>                                         
                                         <td><strong>Email of Contractor</strong></td>
                                         <td><?php echo $datafetch_clra_cont->email_of_contractor; ?></td>
                                     </tr>
                                     <tr>                                         
                                         <td><strong>Address of the Contractor</strong></td>
                                         <td><?php echo $datafetch_clra_cont->address_of_contractor.'<br/>'.$contractor_info_address; ?></td>
                                     </tr>
                                     <tr>
                                         <td><strong>Nature of Work in which Contract Labour is Employed or is to be Employed</strong></td>                                         
                                         <td><?php echo $nature_of_wrk; ?></td>
                                     </tr>
                                     <tr>
                                         <td><strong>Maximum Number of Contractor Labour to be Employed on any day Through Each Contractor</strong></td>                                         
                                         <td><?php echo $datafetch_clra_cont->contractor_max_no_of_labours_on_any_day; ?></td>
                                     </tr>
                                     <tr>
                                         <td><strong>Estimated Date of Employment of Each Contract Work Under Each Contractor</strong></td>
                                         <td><?php echo date('dS M, Y',strtotime($datafetch_clra_cont->est_date_of_work_of_each_labour_from_date)).' to '. date('dS M, Y',strtotime($datafetch_clra_cont->est_date_of_work_of_each_labour_to_date)) ?></td>
                                     </tr>
                                     <tr>
                                         <td><strong>Status</strong></td>
                                         <td><?php echo $contractor_status;?></td>
                                     </tr>
                                  </table>
                                </div>
                            <div class="modal-footer">
                                <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
                            </div>
                        </div>      
                        </div>
                        </div>
                </td>
			</tr>
             
		<?php  
		} 
	}else{
 	?>
    <tr><td colspan="6" style="text-align:center"><center>Contractors Not Available</center></td></tr>
 	<?php 
	}
	?>
    </tbody>
</table></div>
	</div>
</div>
<!-- CONTRACTOR INFORMATION END -->




<div class="row">
<?php
	$action	 = 'encrypt';
	$application_id_pre		= encryption_decryption_fun($action, $content['id']);
	$applicant_user_id_pre	= encryption_decryption_fun($action, $content['user_id']);
	$act_id					= encryption_decryption_fun($action, $content['act_id']);
	?>
    <!--<div class="col-md-3 col-xs-12">
      <div class="info-box">
        <span class="info-box-icon bg-aqua"><i class="fa fa-info"></i></span>
        <div class="info-box-content">
          <span class="info-box-text">Instructions to digitally sign documents using USB Token</span>
          <span class="info-box-number"><i class="fa fa-view"></i></span>
        </div>
      </div>
    </div>
    
    <div class="col-md-2 col-xs-12" style="list-style:none;margin-bottom:5px;"> 
        		<button class="btn btn-block btn-info">
    				<span class="bg-aqua"><i class="fa fa-arrow-left"></i>  Back to list </span> 
    			</button>
                <div class="info-box-content">Back to list</div>
        </div>-->
    
	<ul style="padding:0;">
		<li class="col-md-2 col-xs-12" style="list-style:none;margin-bottom:5px;">
			<?php echo l('<button class="btn btn-block btn-info"><i class="fa fa-arrow-left"></i> Back to list</button>', 'alc_receivedapplications', array('attributes' => array('title' => 'Click here back to applications list'), 'html' => TRUE))?></li>		
<li class="col-md-2 col-xs-12" style="list-style:none;margin-bottom:5px;"><?php echo l('<button class="btn btn-block btn-info"><i class="fa fa-user"></i> View applicant profile</button>', 'view-applicant-profile/'.$application_id_pre.'/'.$applicant_user_id_pre.'/'.$act_id, array('attributes' => array('title' => 'Click here view Applicant Profile ', 'target' => '_blank'), 'html' => TRUE))?></li>

<li class="col-md-4 col-xs-12" style="list-style:none;margin-bottom:5px;"><?php echo l('<button class="btn btn-block btn-info"><i class="fa fa-info-circle"></i> Instructions to give remarks</button>', $base_root.$base_path.'sites/default/files/instructions-remarks.pdf', array('attributes' => array('title' => 'Instructions to give remarks', 'target' => '_blank'), 'html' => TRUE))?></li> 

<li class="col-md-4 col-xs-12" style="list-style:none;margin-bottom:5px;"><?php echo l('<button class="btn btn-block btn-info" style="white-space:normal;"><i class="fa fa-question-circle" aria-hidden="true"></i>
 How to digitally sign documents using USB token</button>', 'digitally-sign-process', array('attributes' => array('title' => 'How to digitally sign your documents using USB Token', 'target' => '_blank'), 'html' => TRUE))?></li>
 </ul>
</div>

<?php if($content['status'] == 'B'){					
			$current_status .= '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> Current status: Back for rectification</h4><p>Application is sent back for rectification. After modification by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
			
		}
		else if($content['status'] == 'V'){			
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> Current status: Verified and approved for fees submission</h4><p>Application is approved and directed to pay fees. After fees payment and submission of signed FORM-I by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
			
		}
		else if($content['status'] == 'C'){
			$current_status = 'CURRENT STATUS: Applicant has been CALLED BY ALC';
			// $current_status = '<div class="callout callout-warning"><h4>Current status: Back for rectification</h4><p>If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
			
		}
		else if($content['status'] == 'I'){			
			$current_status = '<div class="callout callout-success"><h4>Current status: Certificate issued</h4><p>Certificate is issued. For any changes in the FORM-II(Certificate), Applicant can opt for Amendment of Registration Certificate. If you want to get back to the previous remark and re-upload FORM-II(Certificate), delete the current remark by clicking the delete option.</p></div>';
			
		}
		else if($content['status'] == 'F'){			
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> Current status: Forwarded by Inspector to ALC</h4><p>Application is Forwarded to ALC by Inspector for further verification. Any action can be taken for the application.</p></div>';
			
		}
		else if($content['status'] == 'T'){			
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> Current status: Transaction successful</h4><p>Payment successful for this application. Form-I is not uploaded by the applicant. After submission of signed FORM-I by the applicant, the application can be further accessible .</p></div>';
			
		}
		else if($content['status'] == 'T' && ($remark_by_role == '4' ||$remark_by_role == '7') ){			
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> Current status: Back for rectification</h4><p>Payment successful for this application. Form-I is not uploaded by the applicant. After submission of signed FORM-I by the applicant, the application can be further accessible.</p></div>';
			
		}		
		else if($content['status'] == 'R'){			
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> Current status: Rejected application</h4><p>If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
			
		}
		
		else if($content['status'] == 'BI'){			
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> Current status: Back to inspector for rectification</h4><p>Application is sent back to Inspector for further verification. After verification, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
			
		}
		
		else if($content['status'] == 'S'){			
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> Current status: Form-I uploaded & application submission completed</h4><p>FORM-I is submitted by the Applicant. After verification of uploaded FORM-I, Issue of Registration Certificate can be generated now or back to rectification FORM-I.</p></div>';			
		}
		
		else if($content['status'] == 'VA'){			
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> Current status: Application has been verified &amp; approved without fees.</h4><p>Application is approved without fees. After submission of signed FORM-I by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
			
		}else if($content['status'] == '0'){			
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> Current status: Pending application</h4><p>Application is applied by the Applicant. Any action can be taken for the application.</p></div>';
			
		}?>

<br class="clear"  />
<?php echo $current_status;?>
<br class="clear"/>

<input type="hidden" name="clra_reg_no" value="<?php echo $content['registration_number'];?>" />
<input type="hidden" name="clra_reg_date" value="<?php echo $content['registration_date'];?>" />
<input type="hidden" name="base_url" value="<?php echo $base_root.$base_path;?>" />

<?php if($content['status'] != 'V' && $content['status'] != 'I' && $content['status'] != 'C' && $content['status'] != 'R' && $content['status'] != 'S' && $content['status'] != 'B' && $content['status'] != 'T' && $content['status'] != 'VA'){?>

	<div class="box box-primary">
		<div class="box-header with-border">
			<h3 class="box-title"><i class="ion ion-clipboard"></i> &nbsp; ACTIONS AND REMARK</h3>
		</div>
		<div class="box-body" id="clra_alc">
			<div id="message">
			<?php
			if(!empty($content['clra_qr_code'])&& empty($content['certificates_fid'])){					
				echo $message = drupal_set_message_custom(t('CLRA Form-II(Certificate) has been successfully generated. Please Download it and Upload the Form-II(Certificate) after signing. Then click on submit button to complete this process.'));					
			}else if (!empty($content['clra_qr_code'])&& (!empty($content['certificates_fid']))){					 
				echo $message='';					
			}else{					
				echo $message='';
			}
			
			if($_SESSION['error_msg'] != ''){
				echo drupal_set_message_custom(t($_SESSION['error_msg']), 'error');
			}		
			?>
			</div>        
		<form name="clra_alc_action" id="clra_alc_action" action="" method="post" >
					<div class="row"><div class="col-md-3"></div>
					<div class="form-custom col-md-6">
						<label class="input">
						<div class="form-item form-type-select">
						<label for="Actions">Please Select Actions  <font color="#FF0000">*</font></label>
							<select name="alc_clra" class="form-control" id="alc_clra_id" required>
								<option value="">Select process type</option>
								<option value="B">Back for correction</option>
								<option value="BI">Back to inspector</option>
								<option value="R">Reject</option>
								<?php if(!empty($content['backlog_id']) && $content['backlog_id']!=0) { ?>
								<option value="VA">Verified & Approval without fees submission</option>
								<option value="V">Verified & Approval for fees submission</option> 
								<?php } else {?>
								<option value="V">Verified & Approval for fees submission</option>
								<?php } ?>                                       
							</select>
						</div>
						</label> 
					</div>
					<div class="col-md-3"></div>
					</div>
					<div class="row">
					<div class="col-md-3"></div>
					<div class="form-custom col-md-6">
					<label class="input">
						<div class="form-item form-type-select">                       
							<label for="Remark">Remark <font color="#FF0000">*</font></label>
							<textarea name="clra_alc_remarks" id="clra_alc_remarks_id" class="form-control" rows="2" required></textarea></label>
						</div>
					</label>
					</div>
					<div class="col-md-3"></div>
					</div>
					<input type="hidden" size="300" id="field_name" name="field_name" value="<?php echo $editable_data['remark_field_text'];?>" />
					<input type="hidden" size="300" id="field_nameorg" name="field_nameorg" value="<?php echo $editable_data['remark_field_text'];?>" />
					<input type="hidden" id="var_id" name="var_id" value="<?php echo $var_id; ?>" /> <!--var_id = application_id-->
					<input type="hidden" id="user_id" name="user_id" value="<?php echo $userid; ?>" />
					<input type="hidden" id="caf_id_no" name="caf_id_no" value="<?php echo $content['identification_number']; ?>" />
					<input type="hidden" id="eodb_app_id" name="eodb_app_id" value="<?php echo $content['eodb_app_id']; ?>" />  
					<input type="hidden" id="" name="finalfees" value="<?php echo $content['finalfees']; ?>" />
					<div class="row">
					<div class="col-md-3"></div>
					<div class="col-md-6">   
						<div class="box-footer">           
							<input type="submit" name="clr_alc_submit" value="Submit" id="clr_alc_submit_id" class="btn btn-primary pull-left form-submit" />
						</div>
					</div>
					</div>                   
		</form>
		</div>
	</div>

<?php }elseif($content['status'] == 'S' || $content['status'] == 'C'){ 
	
	$info = check_final_submit_data(encryption_decryption_fun('encrypt', 1), encryption_decryption_fun('encrypt', $content['id']));	
	if(!empty($info) && $info['remark_type']=='S'){
		$final_submit_date = $info['remark_date'];  
		//echo 'final_submit_date--'.date('Y-m-d h:i:s',$final_submit_date);
		$today = time();
		//echo 'today--'.date('Y-m-d h:i:s',$today).'<br/>';						
		$datediff = $today - $final_submit_date;	  
		$diff = round($datediff / (60 * 60 * 24));
	}
	
	if($diff <=30){ /** For Deemed ***/ 
	
		$remark_status	=	fetch_second_remarkrow($var_id);  
		if($remark_status == 'V')
			$remark_status = 'T';
		else
			$remark_status = $remark_status;?>

		<div class="box box-primary">
			<div class="box-header with-border">
				<h3 class="box-title"><i class="ion ion-clipboard"></i> &nbsp;ACTIONS AND REMARK</h3>
			</div>
			<div class="box-body" id="clra_alc">
				<div id="message">
					<?php
					if(!empty($content['clra_qr_code'])&& empty($content['certificates_fid'])){					
						echo $message = drupal_set_message_custom(t('CLRA Certificate has been successfully generated.Please Download it and Upload this Registration Certificate.Then click on submit button to complete this process')); //-------------miscellaneous module					
					}else if (!empty($content['clra_qr_code'])&& (!empty($content['certificates_fid']))){					 
						echo $message='';					
					}else{					
						echo $message='';
					}
					
					if($_SESSION['error_msg'] != ''){
						echo drupal_set_message_custom(t($_SESSION['error_msg']), 'error');
					}
				?>
				</div>
				<form name="clra_alc_action" id="clra_alc_action" action="" method="post"  enctype="multipart/form-data">
					<div class="row">
						<div class="col-md-3"></div>
						<div class="form-custom col-md-6">
							<label class="input">
								<div class="form-item form-type-select">
								<label for="Actions">Please Select Actions <font color="#FF0000">*</font></label>       
								<?php if(!empty($content['clra_qr_code'])&& empty($content['certificates_fid'])){?>    
									<select name="alc_clra" id="alc_clra_id" class="form-control" onchange="return alc_clra_action();" required>
										<option value="">Select Process Type</option>              
										<!--<option value="C" >Call Applicant</option>-->
										<option value="I" selected="selected">Issue Certificate</option>              
										<!--<option value="R" >Reject</option>-->
									</select>
									<?php }else if (!empty($content['clra_qr_code'])&& (!empty($content['certificates_fid']))){?>
									<select name="alc_clra" id="alc_clra_id" class="form-control" onchange="return alc_clra_action();" required>
										<option value="">Select Process Type</option>
										<!--<option value="C" >Call Applicant</option>-->
										<option value="I" >Issue Certificate</option>              
										<option value="R" >Reject</option>
									</select>
									<?php }else{?> 
									<select name="alc_clra" id="alc_clra_id" class="form-control" onchange="return alc_clra_action();" required>
										<option value="">Select Process Type</option>
										<option value=<?php echo $remark_status;?>>Back for correction</option>
										<!--<option value="C" >Call Applicant</option>-->
										<option value="I" >Issue Certificate</option>              
										<option value="R" >Reject</option>
									</select>
								<?php }?>
								</div>
							</label>
						</div>
						<div class="col-md-3"></div>
					</div>
					<div id="genarate_pdf" style="display:none">
						<div class="row">        
							<div class="col-md-3"></div>           
							<div class="col-md-3">
							<?php //if($content['user_id']==352){?>
							<input type="submit" name="clra_alc_gen_certificates" value="Generate Certificate" id="clra_alc_gen_certificates" class="btn btn-primary pull-left form-submit"/> 
							<?php //}else{?>            
							<!--<input type="submit" name="clra_alc_gen_certificates" value="Generate Certificate" id="clra_alc_gen_certificates" class="btn btn-primary pull-left form-submit"  onclick="generate_certificates();" /> -->
							<?php //}?>
							</div>
							<div class="col-md-6"></div> 
						</div>
					</div> 
					<?php
						if (!empty($content['clra_qr_code']) && (!empty($content['registration_number']))){
							$display = 'block';
						}else{
							$display = 'none';
						}
					?>
					<div id="action_div" style="display:<?php echo $display;?>;">
						<div class="row">        
						<div class="col-md-3"></div>           
						<div class="col-md-3"><a title="Click here to download Original Certificate" href="/download-pdf-clra-registration/<?php echo time();?>/<?php echo encryption_decryption_fun('encrypt', $var_id); ?>/<?php echo encryption_decryption_fun('encrypt', $userid); ?>" target="_blank"><span class="badge bg-green"><h5><i class="fa fa-download fa-lg"></i>&nbsp;Click to download certificate</h5></span></a></div>
						</div>
					</div>
					<div class="row" style="display:<?php echo $display;?>;">        
						<div class="col-md-3"></div>           
						<div class="col-md-3">               
							<label class="input">
								<div class="#form-item #form-type-select">
								<label for="upload"><!--Choose signed Form-II(Certificates)<font color="#FF0000">*</font>--></label>
								<div id="edit-clra-signed-certificate-upload" class=" form-managed-file">
								<div class="form-group">
								<div class="btn btn-default btn-file" style="white-space:normal;"><i class="fa fa-paperclip"></i> Click to choose signed Form-II(Certificates)
								<input id="edit-clra-signed-certificate-upload" class="form-file" type="file" size="22" name="files[clra_signed_certificate]">
								<input type="hidden" value="0" name="clra_signed_certificate[fid]">
								</div>
								</div>
								</div>
								</div>
							</label>
						</div>
					</div>
					<div class="row">        
						<div class="col-md-3"></div>           
						<div class="form-custom col-md-6">
					<label class="input">
						<div id="submit_remark" style="display:block" class="form-item form-type-select"> 
							<label for="Remark">Remark <font color="#FF0000">*</font></label>             		
							<textarea name="clra_alc_remarks" id="clra_alc_remarks_id" rows="2" class="form-control"></textarea>
						</div>
					</label>
					</div>
						<div class="col-md-3"></div>
					</div>
					<input type="hidden" id="field_name" name="field_name" value="<?php echo $editable_data['remark_field_text'];?>" />
					<input type="hidden" id="var_id" name="var_id" value="<?php echo $var_id; ?>" /> <!--var_id = application_id-->
					<input type="hidden" id="user_id" name="user_id" value="<?php echo $userid; ?>" />
					<input type="hidden" id="clra_qr_code" name="clra_qr_code" value="<?php echo $content['clra_qr_code']; ?>" />
					<input type="hidden" id="registration_number" name="registration_number" value="<?php echo $content['registration_number']; ?>" />
					<input type="hidden" id="registration_date" name="registration_date" value="<?php echo $content['registration_date']; ?>" />
					<input type="hidden" id="caf_id_no" name="caf_id_no" value="<?php echo $content['identification_number']; ?>" />
					<input type="hidden" id="eodb_app_id" name="eodb_app_id" value="<?php echo $content['eodb_app_id']; ?>" /> 
					<div class="row">
						<div class="col-md-3"></div>
						<div class="col-md-6">   
							<div class="box-footer">
							<input type="submit" name="clr_alc_submit" value="Submit" id="clr_alc_submit_id" class="btn btn-primary pull-left form-submit" onclick=" return submit_form();" />
							</div>
						</div>
					</div> 
				</form>
			</div>
		</div>
<?php
	}
}
?>

 <div class="box box-default box-solid">
 	<div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">REMARK SUMMARY</h3></div>
    	<div class="box-body">
        	<div class="feedback-scroll">
				<?php 
				$user_id 	= $user->uid;
				$counter 	= 0;
				$loadingImg = $base_root.'/sites/all/themes/jackson/images/ajax-loader.gif';
				$header 	= array(
                              array('data' => 'SL.NO.', 'field' => '','width' => '6%'),
                              array('data' => 'Date', 'field' => 'date','width' => '10%'),
                              array('data' => 'Remark', 'field' => 'remrk'),
                              array('data' => 'Status', 'field' => 'remark type','width' => '7%'),		  
                              array('data' => 'Remark By', 'field' => '','width' => '15%'),
                              array('data' => 'Action','width' => '6%'),
                          );
	
				foreach($param_remark as $row){
					
					$counter 		= $counter+1;
					$remark_by		= $row->remark_by_name.'&nbsp;('.$row->name.')';
					
					$getPayInfo		= db_query("SELECT COUNT(*) AS total_row FROM l_principle_epayments_data WHERE act_id=1 AND application_id = ".$row->application_id."")->fetchObject();
					$getId			= $getPayInfo->total_row;
					
					$getMaxData		= db_query("SELECT MAX(remark_id) AS MAXID FROM l_remarks_master WHERE is_active=1 and application_id = ".$row->application_id."")->fetchObject();
					$maxId			= $getMaxData->maxid;
					
					if($maxId>0){
						if(($row->remark_by==$user_id) &&($maxId==$row->remark_id) && ($getId!=0 || $getId!='')&& $row->remark_type != 'I'){					
							$delete	= '<span id="delBtn" class="delete-btn active-del" data-id ='.encryption_decryption_fun('encrypt',$row->remark_id).' data-uid='.encryption_decryption_fun('encrypt',$user_id).' data-act='.encryption_decryption_fun('encrypt',1).'>&nbsp;<img style="display:none;margin-left:30px;" src="'.$loadingImg.'" alt="loading" id="loading_'.encryption_decryption_fun('encrypt',$row->id).'"></span>';
						}else{
							$delete	= '<div class="delete-btn disable-del">&nbsp;</div>';
						}	
						
					}
					
			/*if($maxId>0){
				if(($row->remark_by==$user->uid) &&($maxId==$row->remark_id) && empty($getId)){
					$url = 'clra-remarks-delete/'.encryption_decryption_fun('encrypt',$row->remark_id).'/'.encryption_decryption_fun('encrypt',$row->application_id).'/'.encryption_decryption_fun('encrypt',$userid);			
					$actionItem	 = l('<div id="delBtn" class="delete-btn active-del">'.t('Delete').'</div>',$url,array('html' => TRUE));	
				}else{
					$actionItem	  = '<div id="delBtn" class="delete-btn disable-del">'.t('Delete').'</div>';
				}
			}
			*/
	
						
		
			if($row->remark_type=='T' && ($row->remark_by_role == '4' || $row->remark_by_role == '7')){
				 $remarkStatus ='<span class="backed" title="Back for Form-I Rectification"></span>';
			}else{
				$remarkStatus = get_user_application_status($row->remark_type);
			}
			$rows[] = array(
						array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
						array('data' =>date('dS M, Y - g:i a', $row->remark_date), 'align' => 'left', 'class' => array('odd')),
						array('data' => nl2br($row->remarks), 'align' => 'left', 'class' => array('odd')),
						array('data' => $remarkStatus, 'align' => 'left', 'class' => array('odd')),					
						array('data' =>$row->remark_by_name, 'align' => 'left', 'class' => array('odd')),
						array('data' =>$delete, 'align' => 'left', 'class' => array('odd'))		
			);
		}
		$variables = array(
			'attributes' 		=> array('class' => array('table table-striped table-responsive admin-custom-table')), 
			'header' 			=> $header,
			'rows'				=> $rows,
			'empty' 	 => t("Remark not available!")
	  );

	
		$output = theme('datatable', $variables);
	
		if($content['status'] != 'V' && $content['status'] != 'I' && $content['status'] != 'C' && $content['status'] != 'R' && $content['status'] != 'S' && $content['status'] != 'B' && $content['status'] != 'T'){
 		echo $output.'</div></div></div>' ;
}else{
	$uri = $GLOBALS['base_url'].'/sites/default/files/'.$upload_certificates_file_url;	
	echo $output.'' ;	
}

$_SESSION['error_msg'] = '';
	
?>


<div class="modal fade" id="myModalFeeschat" role="dialog">
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
                        <th>#</th>
                        <th>Fees</th>
                     </tr>
                     <tr>
                         <td>1</td>
                         <td>Does not exceed less or equal to 20</td>
                         <td>₹200</td>
                     </tr>
                     <tr>
                         <td>2</td>
                         <td>Exceeds 20 but does not exceed 50</td>
                         <td>₹500</td>
                     </tr>
                     <tr>
                         <td>3</td>
                         <td>Exceeds 50 but does not exceed 100</td>
                         <td>₹1000</td>
                     </tr>
                     <tr>
                         <td>4</td>
                         <td>Exceeds 100 but does not exceed 200</td>
                         <td>₹2000</td>
                     </tr>
                     <tr>
                         <td>5</td>
                         <td>Exceeds 200 but does not exceed 400</td>
                         <td>₹4000</td>
                     </tr>
                     <tr>
                         <td>6</td>
                         <td>Exceeds 400</td>
                         <td>₹5000</td>
                     </tr>
                  </table>
        		</div>
        	<div class="modal-footer">
          		<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
        	</div>
      	</div>      
    </div>
</div>



