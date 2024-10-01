<?php
function mtw_registration_pdf_generator($mtw_registration_encoded_details){
	global $base_root, $base_path, $user; 
	
	$action								= 'decrypt';
	$generated_qr_code 					= encryption_decryption_fun($action,$mtw_registration_encoded_details);
	$mtw_registration_details			= $generated_qr_code;
	
	$qr_arr								= explode('-',$mtw_registration_details);
	$qr1								= $qr_arr[0];//echo '<br>';
	$rest								= $qr_arr[1];		
	$get_application_id 				= substr($rest,strpos($rest, "G") + 1);
	$application_id     				= substr($get_application_id, 0, strpos($get_application_id, 'A'));
	
	$get_act_id 						= substr($get_application_id,strpos($get_application_id, "A") + 1);
	$act_id     						= substr($get_act_id, 0, strpos($get_act_id, 'U'));
	
	$get_applicant_user_id 				= substr($get_act_id,strpos($get_act_id, "U") + 1);
	$applicant_user_id     				= substr($get_applicant_user_id, 0, strpos($get_applicant_user_id, 'T'));
	
	$created_timestamp					= substr($rest,strpos($rest, "T") + 1);
	
	$mtw_name							= 'Name';
	$office_name						= $office_address	= $office_place = $office_subdiv_name = '';
	$backlog_id							= 0;
	
	
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
		$backlog_id				= $res['backlog_id'];
		
	}
	
	/////////////////////////  Address Fetch ////////////////////////////////
	
	
	$table					= 'l_mtw_registration_master';
	$type					= 'mtw';
	$id						= $application_id;
	$fieldArr				= array('mtw_loc_dist','mtw_loc_subdivision','mtw_loc_areatype','mtw_loc_areatype_code','mtw_loc_vill_ward','mtw_loc_ps','mtw_loc_pincode');
	$establishment_address 	= get_full_address($table, $type, $id, $fieldArr);
	
	/******* Backlog Data *******/
	
	if($backlog_id !=0){
		$backlogQuery	= db_select('l_mtw_registration_backlog','lmrb');
		$backlogQuery	->fields('lmrb',array('registration_number','registration_date','expiry_date'));
		$backlogQuery	->condition('lmrb.id',$backlog_id);
		$backlogExe		= $backlogQuery->execute();
		if($backlogExe->rowCount() >0 ){
			$backlogResult				= $backlogExe->fetchObject();
			$old_registration_number	= '<br>Old Registration Number : '.$backlogResult->registration_number;			
		}
	}else{
		$old_registration_number	= '';
	}
	
	//////////////////////////////  End Master Data /////////////////////////////////////
	
	
	/** Fetch Issuing Authority Details ****/
	
	$get_remark = db_select('l_mtw_remark_master', 'lbrm');
	$get_remark	->leftJoin('role', 'ro', 'ro.rid = lbrm.remark_by_role');
	$get_remark->fields('lbrm', array('remark_by_name'));
	$get_remark->fields('ro', array('name'));
	$get_remark->condition('lbrm.remark_to', trim($applicant_user_id));
	$get_remark->condition('lbrm.application_id', trim($application_id));
	 $get_remark->condition('lbrm.remark_by', trim($applicant_user_id), '!=');
	//$get_remark->condition('lbrm.remark_type', 'I');
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
	
	$qr_content 			= 'SL.NO. : '.$res['certificate_serial']."\nREG. NO. : ".$res['registration_number']."\nMTW. NAME : ".$res['mtw_name']."\nTOTAL WORKERS : ".$res['mtw_maxworkers']."\nISSUED BY : ".$issued_by .' on '. date("dS M, Y", strtotime($res["registration_date"]));
	
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
	
	$emblem_img			= '<img src="'.drupal_get_path('module', 'pdf_generator').'/images/emblemofindia.png"  height="120">';
	$content 			= '';
	$signature_footer	= '';
	$page_break_start 	= '<p style="page-break-after:always">'; 
	$page_break_end 	= '</p>';
	$signature_footer 	= '<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center">								
								<tr><td align="center" style="text-align:justify;font-size:12px;"><i>[Signature & Seal of Registering Officer]<br></i></td><tr/>							
								<tr><td align="center" style="text-align:justify;font-size:12px;"><i>Chief Inspector/Inspectors</i></td></tr>
								<tr><td align="center" style="text-align:justify;font-size:12px;"><i>'.$signature_posting_address.'</i></td></tr>
							</table></p>';
							 
	$content 			= '<style type="text/css">
								table, td, th {font-size:18px;font-family:Times New Roman;height:25px;color:#000;margin-top:20px;background:#fff;}
								th {background-color: #008BD1;color: white;}
								td:hover {background-color:#d4e3e5;}
							
								#css3table {width: 100%;color: #FFFFFF;font-family:Times New Roman;padding:0px;display: table;border: 1px solid black;}
								#row  {display: table-row;border-bottom:1px solid #000;	}
								#left {width:5%;padding:5px;color: #000;font-size:12px;float:left;display: table-cell;border-right:1px solid #000;}
								#right {padding: 5px;width: 50%;color: #000;font-style:italic;font-size:12px;float:right;display: table-cell;}
								#middle {padding: 5px;width: 36%;color: #000;font-style:italic;font-size:12px;float: left;display: table-cell;border-right:1px solid #000;}
								.div-head{font-weight:bold;font-size:12px;text-align:center;font-family:Times New Roman;padding:10px;}
						  </style>';
				
	
	   
	$content 			.=  '<table width="1044px" border="0" cellspacing="0" cellpadding="0">
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
									<td align="left" style="font-size:12px;text-align:left;"><strong><i>Registration No. : '.$res['registration_number'].' </i><strong>'.$old_registration_number.'</td>
									<td style="font-size:12px;text-align:right;"><strong><i>Date : '.date("dS M, Y",$created_timestamp).' </i><strong></td>
								</tr>';
								
	$content					.= '<tr>
									<td align="left" style="font-size:12px;text-align:left;"><strong><i>Serial No. : '.$res['certificate_serial'].' </i><strong></td>
									<td style="font-size:12px;text-align:right;"><strong><i>Fee &#8377; : '.$res['finalfees'].' </i><strong></td>
								</tr>
								<tr><td colspan="2">&nbsp;<br><br><br><br><br><br></td></tr>
								<tr>
									<td colspan="2" style="text-align:justify;font-size:12px;line-height:20px;"><i>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A Certificate of Registration is hereby granted to <u><strong>M/s '.trim($res["mtw_name"]).'</strong></u>, to operate motor transport undertaking employing not more than <strong><u>'.$res['mtw_maxworkers'].'</u></strong> person(s) on any one day during the year subject to the provisions of the Motor Transport Workers Act, 1961, and the Rules, made thereunder.
									<br><br>
									&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The Certificate of Registration shall remain in force till the 31st day of December '.date('Y').'.
									</i>
									</td>
								</tr>
								<tr><td colspan="2">&nbsp;<br><br><br><br><br><br></td></tr>
								<tr>
									<td></td>
									<td style="test-align:center;">'.$signature_footer.'</td>
								</tr>	
							</table>
							
							<table width="100%" border="0" cellspacing="0" cellpadding="0">
								
															
							</table>';				
							
	
	// echo $content; die;
	return pdf_using_mpdf_api($content, 'MTW-Certificate-'.time());
	
	
}

