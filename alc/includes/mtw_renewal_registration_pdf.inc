<?php
function mtw_renewal_registration_pdf($mtw_registration_encoded_details){
	global $base_root, $base_path, $user; 
	
	$action								= 'decrypt';
	$generated_qr_code 					= encryption_decryption_fun($action,$mtw_registration_encoded_details);
	$mtw_registration_details			= $generated_qr_code;
	
	$qr_arr								= explode('-',$mtw_registration_details);
	$qr1								= $qr_arr[0];//echo '<br>';
	$rest								= $qr_arr[1];		
	$get_application_id 				= substr($rest,strpos($rest, "N") + 1);
	$application_id     				= substr($get_application_id, 0, strpos($get_application_id, 'A'));
	
	$get_act_id 						= substr($get_application_id,strpos($get_application_id, "A") + 1);
	$act_id     						= substr($get_act_id, 0, strpos($get_act_id, 'U'));
	
	$get_applicant_user_id 				= substr($get_act_id,strpos($get_act_id, "U") + 1);
	$applicant_user_id     				= substr($get_applicant_user_id, 0, strpos($get_applicant_user_id, 'T'));
	
	$created_timestamp					= substr($rest,strpos($rest, "T") + 1);
	
	$mtw_name							= 'Name';
	$office_name						= $office_address	= $office_place = $office_subdiv_name = '';
	
	/*** Get Applicant Details ****/
	
	$master_part_mtw_reg = db_select('l_mtw_registration_master', 'lbrm');
	$master_part_mtw_reg->fields('lbrm', array());	
	
	$master_part_mtw_reg->fields('sd', array('sub_div_name'));
	$master_part_mtw_reg->leftJoin('sub_division', 'sd', 'sd.sub_div_code=lbrm.mtw_loc_subdivision');
	
	$master_part_mtw_reg->leftJoin('l_rlo_additional_info', 'rai', 'rai.sub_div_code = lbrm.mtw_loc_subdivision');
	$master_part_mtw_reg->fields('rai', array('clra_certificate_address', 'office_name'));
	
	$master_part_mtw_reg->condition('lbrm.user_id', $applicant_user_id);
	$master_part_mtw_reg->condition('lbrm.mtw_qr_code', $generated_qr_code);
	$master_part_mtw_reg->condition('lbrm.id', $application_id);
	$master_part_mtw_reg_result = $master_part_mtw_reg->execute();
	
	if($master_part_mtw_reg_result->rowCount() > 0){
		
		$res		 			= $master_part_mtw_reg_result->fetchAssoc();
		$mtw_name				= $res['mtw_name'];
		$office_name 			= $res['office_name'];
		$office_place			= get_certificate_office_name($office_name);
		$office_subdiv_name		= $res["sub_div_name"];
		$office_address 		= $res["clra_certificate_address"];
		
		//$date = strtotime($res['expiry_date']); 
		//$new_date = strtotime('+ 1 year', $date); 
		//$expiry_date = date('dS M, Y', $new_date);
		
		$expiry_date = date('dS M, Y', strtotime($res['expiry_date']));
	}
	
	/////////////////////////  Address Fetch ////////////////////////////////
	
	
	$table					= 'l_mtw_registration_master';
	$type					= 'mtw';
	$id						= $application_id;
	$fieldArr				= array('mtw_loc_dist','mtw_loc_subdivision','mtw_loc_areatype','mtw_loc_areatype_code','mtw_loc_vill_ward','mtw_loc_ps','mtw_loc_pincode');
	$establishment_address 	= get_full_address($table, $type, $id, $fieldArr);
	
	//////////////////////////////  End 1 Location of Establishment/////////////////////////////////////
	
	
	/***** Get Renewal All Data ******/
	
	$getRenewalQuery	= db_select('l_mtw_registration_renewal','lmrr');		
	$getRenewalQuery->fields('lmrr',array('renewal_ref_number','renewal_date','expiry_date','status'));
	$getRenewalQuery->condition('lmrr.application_id',trim($application_id));
	$getRenewalQuery->condition('lmrr.purpose','renewal');
	//$getRenewalQuery->condition('lmrr.status','I');
	$getRenewalQuery->orderBy('lmrr.id','ASC');
	$getRenewal = $getRenewalQuery->execute();
	
	
	/***** Get Active Renewal Data ******/
	/*$getActiveQuery	= db_select('l_mtw_registration_renewal','lmrr');		
	$getActiveQuery->fields('lmrr',array('expiry_date'));
	$getActiveQuery->condition('lmrr.application_id',trim($application_id));
	$getActiveQuery->condition('lmrr.purpose','renewal');
	$getActiveQuery->condition('lmrr.status','T');
	$getActive = $getActiveQuery->execute();
	if($getActive->rowCount()>0){
		$getActiveResult 	=	$getActive->fetchObject();
		$expiry_date			=	' valid upto '.date("dS M, Y", strtotime($getActiveResult->expiry_date));
	}else{
		$expiry_date			=	'';
		
	}*/
	//echo '<pre>'; print_r($getActiveResult); die;
	
	
	/** Fetch Issuing Authority Details ****/
	
	$get_remark = db_select('l_mtw_remark_master', 'lbrm');
	$get_remark	->leftJoin('role', 'ro', 'ro.rid = lbrm.remark_by_role');
	$get_remark->fields('lbrm', array('remark_by_name'));
	$get_remark->fields('ro', array('name'));
	$get_remark->condition('lbrm.remark_to', trim($applicant_user_id));
	$get_remark->condition('lbrm.application_id', trim($application_id));
	$get_remark->condition('lbrm.remark_by', trim($applicant_user_id), '!=');
	$get_remark->orderBy('lbrm.id','DESC');
	$get_remark->range(0, 1);
	$get_remark_result = $get_remark->execute()->fetchAssoc();
	
	$issued_by = $get_remark_result['remark_by_name'].'('.$get_remark_result['name'].' '.$res['sub_div_name'].')';
	
	/***** Generation of QR Code **************/
	
	$PNG_TEMP_DIR 			= drupal_get_path('module', 'pdf_generator').'/upload/mtw_reg_qr_code/';
	$PNG_WEB_DIR 			= drupal_get_path('module', 'pdf_generator').'/upload/mtw_reg_qr_code/';
	
	include "./sites/all/modules/pdf_generator/phpqrcode/qrlib.php";
	
	if (!file_exists($PNG_TEMP_DIR))
		mkdir($PNG_TEMP_DIR);
		
	$filename 				= $PNG_TEMP_DIR.'test.png';
	$errorCorrectionLevel 	= 'H';
	$matrixPointSize 		= 2;
	
	$qr_content 			= 'SL.NO. : '.$res['certificate_serial']."\nREG. NO. : ".$res['registration_number']."\nMTW. NAME : ".$res['mtw_name']."\nTOTAL WORKERS : ".$res['mtw_maxworkers']."\nISSUED BY : ".$issued_by .' on '. date("dS M, Y", strtotime($res["registration_date"])).$expiry_date;
	
	$filename				= $PNG_TEMP_DIR.encryption_decryption_fun('encrypt', 'MTW'.time()).'.png';
	
	QRcode::png($qr_content, $filename, $errorCorrectionLevel, $matrixPointSize, 4); 
	
	$generate_qr_image 		= $PNG_WEB_DIR.basename($filename);
	
	$qr_image				= '<img src="'.drupal_get_path('module', 'pdf_generator').'/upload/mtw_reg_qr_code/'.basename($filename).'" style="margin-top:20px;">';
	
	/***** Generation of QR Code Ends**************/
	
	
	/*** FETCH ADDRESS OF ALC RLO ADDRESS  ****/
		
	$last_remarked_issued_alc  			= 	db_select('l_mtw_remark_master', 'lbrm');
	$last_remarked_issued_alc			->	fields('lbrm',array('remark_by'));
	$last_remarked_issued_alc			->  condition('lbrm.remark_to', $applicant_user_id);
	$last_remarked_issued_alc			->  condition('lbrm.application_id', $application_id);
	$last_remarked_issued_alc			->  condition('lbrm.remark_type', 'I');
	$last_remarked_issued_alc			->	range(0, 1);
	$issued_by_user_id_alc 				=	$last_remarked_issued_alc->execute()->fetchAssoc();
	
	if(!empty($issued_by_user_id_alc)){
		$last_remarked_alc 				= $issued_by_user_id_alc['remark_by'];
	}
	//echo $application_id;
	
	/**** Fetch ALC SUBDIVISION Code ****/
	
	$get_alc_subdivision_details					=	db_select('l_customuser_relation_address', 'lcra');
	$get_alc_subdivision_details					->  leftJoin('users_roles', 'ur','ur.uid=lcra.user_id ');
	$get_alc_subdivision_details					->  leftJoin('role', 'ro', 'ro.rid = ur.rid');
	$get_alc_subdivision_details					->  fields('ro', array('name','rid'));
	$get_alc_subdivision_details					->	fields('lcra',array('sub_div_code'));
	$get_alc_subdivision_details					->	condition('lcra.user_id',$user_id);
	$get_alc_subdivision_details_result 			= 	$get_alc_subdivision_details->execute();
	
	if($get_alc_subdivision_details_result-> rowCount() > 0 ){
		
		$alc_data 					= $get_alc_subdivision_details_result->fetchAssoc();  
		$alc_subdivision_code 		= trim($alc_data['sub_div_code']);
		$alc_role_rid				= $alc_data['rid']; 
		$alc_role_name				= $alc_data['name']; 
		
	}
	
	$alc_address		=	miscellaneous_alc($last_remarked_alc);
	
	
	/*** Transaction Details ***/
	
	
	$transaction_details  			= db_select('l_principle_epayments_data', 'lped');
	$transaction_details			->leftJoin('l_principle_epayments_receive_data', 'lpd', 'lpd.transaction_id=lped.identification_no');
	$transaction_details			->fields('lped', array());
	$transaction_details			->fields('lpd', array());
	$transaction_details			->condition('lped.act_id', '2');
	$transaction_details			->condition('lped.deposited_by', trim($applicant_user_id));
	$trans_details_result 			= $transaction_details->execute();
	
	if($trans_details_result-> rowCount() > 0 ){
		
		$transaction_det	= $trans_details_result->fetchAssoc(); 
		$bankTransactionID	= $transaction_det['banktransactionid'];
		if(!empty($transaction_det['challanrefid_date'])){
			$challan_fid_date	= $transaction_det['banktranstimestamp'];
		}else{
			$challan_fid_date	= '';
		}
		$total_amount		= $transaction_det['total_amount'];
		$bank_code			= $transaction_det['bank_cd'];
	}
	$signature_posting_address = $office_subdiv_name;
	
	$emblem_img		= '<img src="'.drupal_get_path('module', 'pdf_generator').'/images/Gazette_Notification_of_State_Emblem.png" width="130" height="150">';
	$emblem_img2	= '<img src="'.drupal_get_path('module', 'pdf_generator').'/images/text-registration.png" width="146" height="43">';
	
	$content 			= '';
	$signature_footer	= '';
	$page_break_start 	= '<p style="page-break-after:always">'; 
	$page_break_end 	= '</p>';
		
	$signature_footer 	= '<table width="96%" border="0" align="center" cellspacing="0" cellpadding="0" style="font-family: Times New Roman, Times, serif;text-align:justify;">
								<tr>
									<td>'.$qr_image.'</td>
									<td valign="bottom" style="font-size:13px;text-align:center; color:#0b3f88;line-height:20px;padding-top:50px;">
									<br/>Chief Inspector/Inspectors<br/>'.$signature_posting_address.'</td>
								</tr>
							</table>';
	
	/*$content 			.=  '<table width="1044px" border="0" cellspacing="0" cellpadding="0">
							 <tr><td align="right" width="55%">'.$emblem_img.'</td><td align="right">'.$qr_image.'</td></tr>
							 <tr><td colspan="2" align="center" style="font-size:25px;padding-bottom:5px;"><strong>GOVERNMENT OF WEST BENGAL</strong></td></tr>	
							 <tr><td colspan="2" align="center" style="font-size:22px;padding-bottom:5px;">'.$office_place.', '.strtoupper($office_subdiv_name).'</td></tr>				 			 
							 <tr><td colspan="2" align="center" style="font-size:20px;"><i>'.$office_address.'</i></td></tr>
							 <tr><td colspan="2"></td></tr>
							 <tr><td colspan="2"></td></tr>
							 <tr><td colspan="2"></td></tr>
					 		 <tr><td colspan="2" align="center" style="font-size:30px;"><strong>FORM II</strong></td></tr>
							<tr><td colspan="2" align="center" style="font-size:15px;">[ See Rule 5 ]</td></tr>
							<tr><td colspan="2"></td></tr>					 
							<tr><td colspan="2" align="center" style="font-size:24px;"><strong><u>CERTIFICATE OF REGISTRATION TO WORK A MOTOR TRANSPORT UNDERTAKING</u></strong></td></tr>
							<tr><td colspan="2"></td></tr>	
							<tr><td colspan="2"></td></tr>	 					 
						 </table>
							
							
							<table width="100%" border="0" cellspacing="0" cellpadding="0">
								<tr><td colspan="2">&nbsp;<br><br></td></tr>
								<tr>
									<td align="left" style="font-size:12px;text-align:left;"><strong><i>Registration No. : '.$res['registration_number'].' </i><strong></td>
									<td style="font-size:12px;text-align:right;"><strong><i>Date : '.date("dS M, Y",$created_timestamp).' </i><strong></td>
								</tr>
								
								<tr>
									<td align="left" style="font-size:12px;text-align:left;"><strong><i>Serial No. : '.$res['certificate_serial'].' </i><strong></td>
									<td style="font-size:12px;text-align:right;"><strong><i>Fee &#8377; : '.$res['finalfees'].' </i><strong></td>
								</tr>
								<tr><td colspan="2">&nbsp;<br><br><br><br><br><br></td></tr>
								<tr>
									<td colspan="2" style="text-align:justify;font-size:12px;line-height:20px;"><i>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A Certificate of Registration is hereby granted to <u><strong>M/s '.trim($res["mtw_name"]).'</strong></u>, to operate motor transport undertaking employing not more than <strong><u>'.$res['mtw_maxworkers'].'</u></strong> person(s) on any one day during the year subject to the provisions of the Motor Transport Workers Act, 1961, and the Rules, made thereunder.
									<br><br>
									&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The Certificate of Registration shall remain in force till the valid upto '.$expiry_date.'
									</i>
									</td>
								</tr>
								<tr><td colspan="2">&nbsp;<br><br><br><br><br><br></td></tr>
								<tr>
									<td></td>
									<td style="test-align:center;">'.$signature_footer.'</td>
								</tr>	
							</table>';				
	
	*/
	
	$content 			= '';
	$content .= '<div style="width:100%; background:url(sites/all/modules/pdf_generator/images/text-gov-of-wb-lc.png) repeat; ">
					<div style="width:100%; background:url(sites/all/modules/pdf_generator/images/frame-md-up2.png) no-repeat left top; padding:16px; height:976px;">
					
					<table width="96%" border="0" align="center" cellspacing="0" cellpadding="0" style="font-family: Times New Roman, Times, serif;text-align:justify;">
						<tr><td colspan="2" align="center" valign="top" width="55%">'.$emblem_img.'</td></tr>
						<tr><td colspan="2" align="center" style="color:#0b3f88;margin:15px 0;padding: 15px 0;  font-weight:700; font-size:22px;line-height:22px; text-transform:uppercase;"><strong>GOVERNMENT OF WEST BENGAL</strong></td></tr>
						<tr><td colspan="2" align="center" style="font-size:15px;color:#3f3f3f;font-weight:700;line-height:20px;"><b>'.$office_place.'<br> '.strtoupper($office_subdiv_name).'</b></td></tr><tr><td colspan="2" align="center" style="font-size:13px;color:#0b3f88;padding:10px 0;"><b><i>'.$office_address.'</i></b></td></tr>
						<tr><td colspan="2"></td></tr>
						<tr><td colspan="2" align="center" style="font-size:16px;color:#0b3f88;padding-top:20px;"><strong>FORM - II<br></strong></td></tr>
						<tr><td colspan="2" align="center" style="font-size:12px; font-weight: normal;color:#0b3f88;">[ See Rule 5 ]</td></tr>
						<tr><td colspan="2">&nbsp;</td></tr>
						<tr><td colspan="2">&nbsp;</td></tr>
						<tr><td colspan="2" align="center" style="font-size:13px;color:#3f3f3f;font-weight:700;line-height:20px;"><strong>CERTIFICATE OF REGISTRATION TO WORK A MOTOR TRANSPORT UNDERTAKING</strong></td></tr>
						<tr>
							<td width="50%" style="font-size:14px;text-align:left; font-style:italic; color:#0b3f88;"><strong><br>Registration No. : '.$res['registration_number'].'<strong>'.$old_registration_number.'</td>
							<td width="50%" style="font-size:14px;text-align:right;font-style:italic; color:#0b3f88;"><strong>Date: '.date("dS M, Y",$created_timestamp).'<strong></td>
						</tr>
						<tr>
							<td width="50%" style="font-size:14px;text-align:left; font-style:italic; color:#0b3f88;"><strong><i>Serial No. : '.$res['certificate_serial'].' </i><strong></td>
							<td width="50%" style="font-size:14px;text-align:right;font-style:italic; color:#0b3f88;"><strong><i>Fees: &#8377; '.$res['finalfees'].' </i><strong><br><br><br></td>
						</tr>
						<tr><td colspan="2"></td></tr>
						<tr><td colspan="2"></td></tr>					
					</table>
					
					<table width="96%" border="0" align="center" cellspacing="0" cellpadding="0" style="font-family: Times New Roman, Times, serif;text-align:justify;">
						 <tr> 
							<td width="50%" style="font-size:12px;text-align:left; font-style:italic; color:#0b3f88;padding-top:15px; line-height:20px;"><br><p>&nbsp; &nbsp; &nbsp; &nbsp; Certificate of Registration is hereby granted to <font="1"><strong>M/s '.trim($res["mtw_name"]).'</strong></font>, to operate motor transport undertaking employing not more than <strong>'.$res['mtw_maxworkers'].'</strong> person(s) on any one day during the year subject to the provisions of the Motor Transport Workers Act, 1961, and the Rules, made thereunder.
							<br><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The Certificate of Registration shall remain in force till the valid upto '.$expiry_date.'.</p></td>
						</tr><br><br>
						<tr><td></td></tr>
						
					</table>
					
					'.$signature_footer.'
					</div>
					</div>';
	if($getRenewal->rowCount() > 0){
	$content 			.=  '<table width="96%" border="1" align="center" cellspacing="0" cellpadding="0" style="font-family: Times New Roman, Times, serif;text-align:justify;">
							<tr><th>Renewal Reference Number</th><th>Date of renewal</th><th>Date of Expiry</th></tr>';	
		foreach($getRenewal->fetchAll() as $datum){
			if($datum->status == 'V' || $datum->status == 'T' || $datum->status == 'I'){
				$content 		.=	'<tr><td>'.$datum->renewal_ref_number.'</td><td>'.date('dS M, Y',strtotime($datum->renewal_date)).'</td><td>'.date('dS M, Y',strtotime($datum->expiry_date)).'</td></tr>';
			}		
		}
	$content 			.=	'</table>';
	}
	//$content		.= '<tr><td>'.$datum->renewal_ref_number.'</td><td>'.date('dS M, Y').'</td><td>'.$expiry_date.'</td></tr>';
														
	
	
	// echo $content; die;
	return pdf_using_mpdf_api($content, 'MTW-Renewal-Certificate-'.time());
	
	
}

