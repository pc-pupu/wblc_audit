<?php

function mtw_acknowledge_pdf_generator($application_id=''){
	global $base_root, $base_path, $user;
		
		$user_id 			= $user->uid;
		$application_id		= encryption_decryption_fun('decrypt', $application_id);
				
		$master_part_mtw_reg = db_select('l_mtw_registration_master', 'lmrm');
		$master_part_mtw_reg->fields('lmrm', array('id','user_id','is_renew','is_amend','application_date'));
		$master_part_mtw_reg->condition('lmrm.id',$application_id);
		$master_part_mtw_reg_result = $master_part_mtw_reg->execute();  
		if($master_part_mtw_reg_result->rowCount() >0 ){
			$content			=	$master_part_mtw_reg_result->fetchObject();
			$applicant_user_id	=	$content->user_id;
			$application_date	=	$content->application_date;
			$is_renew			=	$content->is_renew;
			$is_amend			=	$content->is_amend;
		}else{
			$content			=	array();
			$applicant_user_id	=	$content->user_id;
			$is_renew			=	0;
			$is_amend			=	0;
		}
		// renewal table data
		/*if($is_renew ==1){
			$renewalQuery		=	db_select('l_mtw_registration_renewal','lmrr');
			$renewalQuery		->	fields('lmrr',array('application_date'));
			$renewalQuery		->	condition('lmrr.application_id',);
			$renewalQuery		->	condition('lmrr.status','1');
			$renewalResult		=	$renewalQuery->	
		}*/
		
		
		
		
		/*** FETCH ADDRESS OF ALC RLO ADDRESS  ****/
		
		$last_remarked_issued_alc  			= 	db_select('l_mtw_remark_master', 'lbrm');
		$last_remarked_issued_alc			->	fields('lbrm',array('remark_by'));
		$last_remarked_issued_alc			->  condition('lbrm.remark_to', $applicant_user_id);
		$last_remarked_issued_alc			->  condition('lbrm.remark_by', $applicant_user_id,'!=');
		$last_remarked_issued_alc			->  condition('lbrm.application_id', $application_id);
		$last_remarked_issued_alc			->	range(0, 1);
		$issued_by_user_id_alc 				=	$last_remarked_issued_alc->execute()->fetchAssoc();
		
		if(!empty($issued_by_user_id_alc)){
			$last_remarked_alc 				= $issued_by_user_id_alc['remark_by'];
		}
		
		
		/**** Fetch ALC SUBDIVISION Code ****/
		
		$get_alc_subdivision_details					=	db_select('l_customuser_relation_address', 'lcra');
		$get_alc_subdivision_details					->  leftJoin('users_roles', 'ur','ur.uid=lcra.user_id ');
		$get_alc_subdivision_details					->  leftJoin('role', 'ro', 'ro.rid = ur.rid');
		$get_alc_subdivision_details					->  fields('ro', array('name','rid'));
		$get_alc_subdivision_details					->	fields('lcra',array('sub_div_code'));
		$get_alc_subdivision_details					->	condition('lcra.user_id',$last_remarked_alc);
		$get_alc_subdivision_details_result 			= 	$get_alc_subdivision_details->execute();
		
		if($get_alc_subdivision_details_result->rowCount() > 0 ){
			
			$alc_data 					= $get_alc_subdivision_details_result->fetchAssoc();  
			$alc_subdivision_code 		= trim($alc_data['sub_div_code']);
			$alc_role_rid				= $alc_data['rid']; 
			$alc_role_name				= $alc_data['name']; 
			
			$alc_address				= $alc_role_name.'<br>'.miscellaneous_alc($last_remarked_alc);
		}else{
			$alc_address				= '';
		}
		
		
		
		/******** Fetch applicant name and address *******/
		
		$applicantData			= db_select('l_common_application_master','lcam');
		$applicantData			->fields('lcam',array('f_name','l_name','m_name','address'));
		$applicantData			->condition('lcam.user_id',$applicant_user_id);
		$applicantresult		= $applicantData->execute();
		
		
		if($applicantresult->rowCount() >0){
			$applicantArr		= $applicantresult->fetchObject();
			$name				= $applicantArr->f_name.' '.$applicantArr->m_name.' '.$applicantArr->l_name;
			$address			= $applicantArr->address;
			
			$table				= 'l_common_application_master';
			$type				= 'custom';
			$id					= $application_id;
			$fieldArr			= array('dist_code','subdivision','areatype','name_areatype','vill_ward','ps','pin');
			
			$applicant_address 	= $name.'<br>'.$address.'<br>'.get_full_address($table,$type,$applicant_user_id,$fieldArr);
		}else{
			$name				= '';
			$address			= '';
			$applicant_address	= '';
		}
		
		
		$output ='';			
		$output .= '<style>
						table, td, th , tr{font-size:15px;font-family:Times New Roman;height:10px;color:#000;background:#fff;padding:2px;}
						.div-head{font-weight:bold;font-size:12px;text-align:center;font-family:Times New Roman;padding:8px;}
					</style>
					
					<table width="100%" border="0" cellspacing="0" cellpadding="0" style="font-family:Times New Roman;  margin-top:5px;">
					  <tr>
						<td align="center" style="font-size:18px;"><strong>FORM - I </strong></td>
					  </tr>
					  <tr>
						<td align="center" style="font-size:16px;"><strong>( SEE RULE 4 )</strong></td>
					  </tr>
					  <tr>
						<td align="center" style="font-size:15px;">ACKNOWLEDGEMENT<br>(West Bengal right to public services act, 2013)</td>
					  </tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr>
						<td>From : '.$alc_address.'</td>
					  </tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr>
						<td>To : '.$applicant_address.'</td>
					  </tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr>
						<td>Subject : The West Bengal Right to Public Services Act, 2013 --- Acknowledgement of application</td>
					  </tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr><td></td></tr>
					  <tr>
						<td>Ref. : Your Application dated <u>'.date("d.m.Y").'</u><br><br><p>I hereby acknowledge your application cited. Due date of service to be provided is <u>'.date("d.m.Y").'</u></p></td>
					  </tr>
					  <tr><td></td></tr>
					 <!-- <tr>
						<td align="center">Or</td>
					  </tr>
					  <tr><td>The following defects in the application may be rectified, urgently:<br>(Specify defects, if any)<br><ol><li></li></ol></td></tr>-->';
					  

				
				$output .='	<tr><td></td></tr>
							<tr><td></td></tr>
							<tr><td></td></tr>
							<tr><td></td></tr>
							<tr><td></td></tr>
							<tr><td></td></tr>
							<tr><td></td></tr>
							<tr><td></td></tr>
							<tr><td></td></tr>
							<tr><td></td></tr>
							<tr><td colspan="2" align="left">Date of receipt of application : &nbsp; '.date("dS M Y").'</td></tr>
					   </table>';
							
						
			//print_r($output);die;
			return pdf_using_mpdf_api($output, 'MTW-FINAL-SUBMIT-ACKNOWLEDGEMENT'.time());

}
