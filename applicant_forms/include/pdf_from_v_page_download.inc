<?php

function pdf_from_v_page_download($form, &$form_state, $application_id, $serial_no=''){
	
	// $serial_no = encryption_decryption_fun('decrypt', $serial_no);
	
	if(empty($serial_no)){	
		global $base_root,$base_path,$user;
		$uid = $user->uid;	
		
		$query = db_select('l_contractor_info_master', 'c_info_master');
     	$query->fields('c_info_master',array('name_of_contractor', 'id', 'application_id', 'email_of_contractor'));
		$query->condition('c_info_master.id', $application_id,'=');
		$query->condition('c_info_master.contractor_type', 2);
     	$result2 = $query->execute()->fetchObject();
				
		$name_of_contractor  = $result2->name_of_contractor;
		$contractor_id  = $result2->id;
		$applicant_id  = $result2->application_id;
		
		
		$query = db_select('l_clra_registration_master', 'lcrm');
     	$query->fields('lcrm',array('registration_number', 'registration_date', 'e_name', 'loc_e_pin_number','loc_e_name'));		
		$query->condition('lcrm.user_id', $uid,'='); 
		$query->condition('lcrm.id', $applicant_id,'=');
				
		$query->leftJoin('district_master', 'dm', 'dm.district_code = lcrm.loc_e_dist');
		$query->fields('dm', array('district_name'));	
	
		$query->leftJoin('sub_division', 'sd', 'sd.sub_div_code=lcrm.loc_e_subdivision');
		$query->fields('sd', array('sub_div_name'));	
	
		$query->leftJoin('block_mun_master', 'bmm', 'bmm.block_code=lcrm.name_areatype');
		$query->fields('bmm', array('block_mun_name'));	
		
		$query->leftJoin('village_ward_master', 'vwm', 'vwm.village_code = lcrm.loc_e_vill_ward');
		$query->fields('vwm', array('village_name'));		
		
		$query->leftJoin('police_station', 'ps', 'ps.police_station_code=lcrm.l_e_ps');
		$query->fields('ps', array('name_of_police_station'));
					
     	$result = $query->execute()->fetchObject();	
			
		$registration_number = $result->registration_number;
		$full_name_principal_emp = $result->full_name_principal_emp;
		
		$e_name_address = '<u>Name and address of establishment</u><br><span style="word-break: break-all;font-size:12px;line-height:30px;font-family:Comic Sans MS;"><u>Name</u>: '.$result->e_name.'<br><u>Address</u>: '.$result->loc_e_name.'<br/>'.ucfirst($result->village_name).', '.$result->block_mun_name.', '.$result->sub_div_name.',<br>'.$result->district_name.', Pin-'.$result->loc_e_pin_number.'<br>West Bengal.</span>';

	}else{
		
		
		$query = db_select('l_contractor_info_master', 'c_info_master');
     	$query->fields('c_info_master',array('id', 'application_id', 'name_of_contractor', 'email_of_contractor'));
		$query->condition('c_info_master.id', $serial_no,'=');
		$query->condition('c_info_master.contractor_type', 2);
     	$result2 = $query->execute()->fetchObject();		
				
		$name_of_contractor  = $result2->name_of_contractor;
		$contractor_id  = $result2->id;
		$applicant_id  = $result2->application_id;
		
		
		$query = db_select('l_clra_registration_master', 'lcrm');
     	$query->fields('lcrm',array('registration_number', 'registration_date', 'e_name', 'loc_e_pin_number','loc_e_name'));		
		$query->condition('lcrm.id', $application_id,'=');
				
		$query->leftJoin('district_master', 'dm', 'dm.district_code = lcrm.loc_e_dist');
		$query->fields('dm', array('district_name'));	
	
		$query->leftJoin('sub_division', 'sd', 'sd.sub_div_code=lcrm.loc_e_subdivision');
		$query->fields('sd', array('sub_div_name'));	
	
		$query->leftJoin('block_mun_master', 'bmm', 'bmm.block_code=lcrm.name_areatype');
		$query->fields('bmm', array('block_mun_name'));	
		
		$query->leftJoin('village_ward_master', 'vwm', 'vwm.village_code = lcrm.loc_e_vill_ward');
		$query->fields('vwm', array('village_name'));		
		
		$query->leftJoin('police_station', 'ps', 'ps.police_station_code=lcrm.l_e_ps');
		$query->fields('ps', array('name_of_police_station'));
					
     	$result = $query->execute()->fetchObject();	
			
		$registration_number = $result->registration_number;
		$full_name_principal_emp = $result->full_name_principal_emp;
		
	
		$e_name_address = '<u>Name and address of establishment</u><br><span style="word-break: break-all;font-size:12px;line-height:30px;font-family:Comic Sans MS;"><u>Name</u>: '.$result->e_name.'<br><u>Address</u>: '.$result->loc_e_name.'<br/>'.ucfirst($result->village_name).', '.$result->block_mun_name.', '.$result->sub_div_name.',<br>'.$result->district_name.', Pin-'.$result->loc_e_pin_number.'<br>West Bengal.</span>';
				
	}
	
	/***** Generation of QR Code **************/
	
	$PNG_TEMP_DIR 			= drupal_get_path('module', 'pdf_generator').'/upload/clra_reg_qr/formV';
	$PNG_WEB_DIR 			= drupal_get_path('module', 'pdf_generator').'/upload/clra_reg_qr/formV';
	
	include "./sites/all/modules/pdf_generator/phpqrcode/qrlib.php";
	
	if (!file_exists($PNG_TEMP_DIR))
		mkdir($PNG_TEMP_DIR);
		
		$filename 				= $PNG_TEMP_DIR.'test.png';
		$errorCorrectionLevel 	= 'H';
		$matrixPointSize 		= 2;
	
		$qr_content = 'Serial Number: 00'.$contractor_id."\nReg. No.: ".$registration_number."\nReg Date: ".date("dS M, Y", strtotime($result->registration_date))."\nEmail:".$result2->email_of_contractor;
	
		$filename =	$PNG_TEMP_DIR.encryption_decryption_fun('encrypt', time()).'.png';
	
		QRcode::png($qr_content, $filename, $errorCorrectionLevel, $matrixPointSize, 4); 
		$generate_qr_image 		= $PNG_WEB_DIR.basename($filename);
	
		$qr_image = '<img src="'.drupal_get_path('module', 'pdf_generator').'/upload/clra_reg_qr/'.basename($filename).'">';
	
	/***** Generation of QR Code Ends**************/
	
	
		
	$page_output = '';
	$page_output = '<style type="text/css">
							table, td, th {height:25px;color:#000;margin-top:20px;background:#fff;}
							th {background-color: #008BD1;color: white;}
							td:hover {background-color:#d4e3e5;}
					    </style>					
					
						<table width="100%" border="0" cellspacing="0" cellpadding="0">
							<tr><td colspan="2">&nbsp;</td></tr>
							<tr><td colspan="2">&nbsp;</td></tr>
							<tr><td colspan="2">&nbsp;</td></tr>
							<tr><td colspan="2" align="center" style="font-size:22px;font-family:Times New Roman;"><strong>FORM V</strong></td></tr>
							<tr><td colspan="2" align="center" style="font-size:13px;font-family:Times New Roman;">[ See Rule 21(2) ]</td></tr>
							<tr><td colspan="2"></td></tr>							
							<tr><td colspan="2" align="center" style="font-size:18px;font-family:Comic Sans MS;">Form of Certificate by Principal Employer</td></tr>							
							<tr><td colspan="2" style="font-size:16px;text-align:center;font-family:Times New Roman;">Serial number : '.str_pad($contractor_id, 7, "0", STR_PAD_LEFT).'</td>
							<tr><td colspan="2"></td></tr>							
							<tr><td colspan="2" style="word-break: break-all;font-size:13px;line-height:25px;font-family:Comic Sans MS;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Certified that I have engaged the applicant <span style="text-transform:uppercase; font-weight:bold;">'.$name_of_contractor.'</span> as a contractor in my establishment. I undertake to be bound by all the provisions of the Contract Labour (Regulation &amp; Abolition) Act,1970, and the West Bengal Contract Labour (Regulation and Abolition) Rules, 1972, in so far as the provisions are applicable to me in respect of the employment of contract labour by the applicant in my establishment.</td>
							</tr>						
							<tr><td colspan="2">&nbsp;</td></tr>
							<tr><td colspan="2">&nbsp;</td></tr>														
							<tr>
							    <td width="56%" style="font-size:14px;line-height:20px;font-family:Comic Sans MS; font-weight:700;">Place: <span style="font-size:11px; font-weight:700;">'.$result->sub_div_name.'</span></td>
								<td align="center" style="font-size:12px;line-height:20px;font-family:Comic Sans MS;text-transform:uppercase;"><span style="color:#dddddd;">'.$full_name_principal_emp.'</span><br>Signature of Principal Employer</td>
							<tr/>							
							<tr>
							    <td width="56%" style="font-size:14px;line-height:20px;font-family:Comic Sans MS;font-weight:700;">Date: <span style="font-size:11px;font-weight:400;">'.date("dS M, Y").'</span></td>
								<td align="center" style="font-size:14px;line-height:20px;font-family:Comic Sans MS;">'.$registration_number.'</td>
							<tr/>
							<tr><td colspan="2">&nbsp;</td></tr>
							<tr>
							    <td width="56%" style="font-size:12px;line-height:30px;font-family:Comic Sans MS;">'.$qr_image.'</td>
								<td align="left" style="font-size:14px;line-height:30px;font-family:Comic Sans MS;">'.$e_name_address.'</td>
							<tr/>
								
						</table>
						';
		return pdf_using_mpdf_api($page_output, $file_name);	
}

function contractor_pdf_from_v_page_download($form, &$form_state, $serial_no=''){
	 
	 global $base_root,$base_path,$user;
	 $serial_no = encryption_decryption_fun('decrypt', $serial_no);
	 $uid = $user->uid;	
	
	$contractorAddress ='';
	if(empty($serial_no)){	
		
		
		$query = db_select('l_contractor_info_master', 'c_info_master');
     	$query->fields('c_info_master',array('name_of_contractor', 'id', 'application_id', 'email_of_contractor'));
		$query->condition('c_info_master.id', $application_id,'=');
		$query->condition('c_info_master.contractor_type', 2);
     	$result2 = $query->execute()->fetchObject();
				
		$name_of_contractor  = $result2->name_of_contractor;
		$contractor_id  = $result2->id;
		$application_id  = $result2->application_id;
		
		
		$table	 	 = 'l_clra_registration_master';
		$type	 	 = 'sub-table';
		$estArr	 	 = array('loc_e_dist','loc_e_subdivision','loc_e_areatype','name_areatype','loc_e_vill_ward','l_e_ps','loc_e_pin_number');
		$establishment_address 	= $result2['loc_e_name'].'<br/>'.get_full_address($table, $type, $application_id, $estArr);
		
		
		$query = db_select('l_clra_registration_master', 'lcrm');
     	$query->fields('lcrm',array('registration_number', 'registration_date', 'e_name', 'loc_e_pin_number','loc_e_name'));		
		$query->condition('lcrm.user_id', $uid,'='); 
		$query->condition('lcrm.id', $applicant_id,'=');
				
		$query->leftJoin('district_master', 'dm', 'dm.district_code = lcrm.loc_e_dist');
		$query->fields('dm', array('district_name'));	
	
		$query->leftJoin('sub_division', 'sd', 'sd.sub_div_code=lcrm.loc_e_subdivision');
		$query->fields('sd', array('sub_div_name'));	
	
		$query->leftJoin('block_mun_master', 'bmm', 'bmm.block_code=lcrm.name_areatype');
		$query->fields('bmm', array('block_mun_name'));	
		
		$query->leftJoin('village_ward_master', 'vwm', 'vwm.village_code = lcrm.loc_e_vill_ward');
		$query->fields('vwm', array('village_name'));		
		
		$query->leftJoin('police_station', 'ps', 'ps.police_station_code=lcrm.l_e_ps');
		$query->fields('ps', array('name_of_police_station'));
					
     	$result = $query->execute()->fetchObject();	
			
		$registration_number = $result->registration_number;
		$full_name_principal_emp = $result->full_name_principal_emp;
		
		$e_name_address = '<u>Name and address of establishment</u><br><span style="word-break: break-all;font-size:12px;line-height:30px;font-family:Comic Sans MS;"><u>Name</u>: '.$result->e_name.'<br><u>Address</u>: '.$result->loc_e_name.'<br/>'.ucfirst($result->village_name).', '.$result->block_mun_name.', '.$result->sub_div_name.',<br>'.$result->district_name.', Pin-'.$result->loc_e_pin_number.'<br>West Bengal.</span>';

	}else{
		
		
		$query 		 = db_select('l_contractor_info_master', 'c_info_master');
		$query		 ->leftJoin('l_clra_registration_master', 'reg', 'reg.id=c_info_master.application_id');
		$query		 ->leftJoin('sub_division', 'sd', 'sd.sub_div_code = reg.loc_e_subdivision');
		
		$query		 ->leftJoin('district_master', 'con_dis', 'con_dis.district_code = c_info_master.con_loc_e_dist');
		$query		 ->leftJoin('sub_division', 'con_subdv', 'con_subdv.sub_div_code = c_info_master.con_loc_e_subdivision');
		$query		 ->leftJoin('block_mun_master', 'con_bmm', 'con_bmm.block_code	=	c_info_master.con_name_areatype');
		$query		 ->leftJoin('village_ward_master', 'con_vwm', 'con_vwm.village_code = c_info_master.con_loc_e_vill_ward');
		$query		 ->leftJoin('police_station', 'con_ps', 'con_ps.police_station_code = c_info_master.con_l_e_ps');
		
		$query		 ->fields('sd', array('sub_div_name'));
		
		$query		 ->fields('con_dis', array('district_name'));
		$query		 ->fields('con_subdv', array('sub_div_name'));
		$query		 ->fields('con_bmm', array('block_mun_name'));	
		$query		 ->fields('con_vwm', array('village_name'));
		$query		 ->fields('con_ps', array('name_of_police_station'));
		
     	$query		 ->fields('c_info_master',array('id', 'application_id', 'name_of_contractor', 'email_of_contractor','con_loc_e_areatype','address_of_contractor','state_opts','con_loc_e_dist','contractor_pin','contractor_max_no_of_labours_on_any_day'));
		$query		 ->fields('reg',array('id','registration_number', 'registration_date', 'e_name', 'loc_e_pin_number','loc_e_name','full_name_principal_emp','loc_e_subdivision'));
		$query		 ->condition('c_info_master.id', $serial_no,'=');
		$query		 ->condition('c_info_master.contractor_type', 2);
     	$result2	 = $query->execute()->fetchAssoc();		//print_r($result2);exit;
		
		if($result2['con_loc_e_areatype']=="B"){
			$text_AreaType_loc = "Block- ";
			$areatype_loc_name = ucwords($result2['village_name']).',&nbsp;';
		}elseif($result2['con_loc_e_areatype']=="M"){
			$areatype_loc_name = 'Ward-'.$result2['village_name'].',&nbsp;'; 
		}elseif($result2['con_loc_e_areatype']=="C"){
			$areatype_loc_name = $result2['village_name'].',&nbsp;';
		}elseif($result2['con_loc_e_areatype']=="N"){
			$areatype_loc_name = $result2['village_name'].',&nbsp;';
		}elseif($result2['con_loc_e_areatype']=="S"){
			$areatype_loc_name = $result2['village_name'].',&nbsp;';
		}
				
		$establishment_address 	= $result2['loc_e_name'].'<br/>'.get_full_address('l_clra_registration_master',  'sub-table', $result2['application_id'], array('loc_e_dist','loc_e_subdivision','loc_e_areatype','name_areatype','loc_e_vill_ward','l_e_ps','loc_e_pin_number'));
		
		if($result2['state_opts'] == 1 || $result2['state_opts'] == ''){
			
			if($result2['con_loc_e_dist'] == 17){
				
				$contractorAddress = $result2['address_of_contractor'].',&nbsp;'.$areatype_loc_name.',&nbsp;PS - '.$result2['name_of_police_station'].',&nbsp;'.$result2['district_name'].'- '.$result2['contractor_pin'];		
				
			}else{
				
				$contractorAddress = $result2['address_of_contractor'].',&nbsp;'.$areatype_loc_name.$text_AreaType_loc.$result2['block_mun_name'].',&nbsp;'.$result2['con_subdv_sub_div_name'].',&nbsp;PS - '.$result2['name_of_police_station'].',&nbsp;'.$result2['district_name'].',&nbsp;PIN- '.$result2['contractor_pin'];;
			}
			
		}else{
			$contractorAddress = $result2['address_of_contractor'];
		}
	}
	
	/** Contractor NAture of Work **/
	
	$cont_work_name = "";
	$prefix 		= '';
	
	$contractor_wrk_name 		= db_select('l_clra_con_nature_of_work', 'lccnw');
	$contractor_wrk_name		->leftJoin('contractor_works', 'cw', 'cw.contractor_work_id=lccnw.contractor_nature_of_work');
	$contractor_wrk_name		->fields('lccnw', array('contractor_nature_of_work'));
	$contractor_wrk_name		->fields('cw', array('cont_work_name'));
	$contractor_wrk_name		->condition('lccnw.user_id', $uid);
	$contractor_wrk_name		->condition('lccnw.contractor_id', $serial_no);
	$contractor_wrk_name		->condition('lccnw.application_id', $result2['application_id']);
	$result_2 					= $contractor_wrk_name->execute()->fetchAll(); //print_r($result_2);exit;
	
	foreach ($result_2 as  $con_nature_of_wrk){
		
		if($con_nature_of_wrk->contractor_nature_of_work == 28){
			if(!empty($result2['other_nature_work'])){
				$cont_work_name  .= $result2['other_nature_work'];
			}else{
				$cont_work_name  .= 'Others';
			}
		}else{
			$cont_work_name  	.= $prefix.$con_nature_of_wrk->cont_work_name;
			$prefix  	 = ', ';
		}
		
	}
	
	/***** Generation of QR Code **************/
	
	$PNG_TEMP_DIR 			= drupal_get_path('module', 'pdf_generator').'/upload/clra_reg_qr/formV';
	$PNG_WEB_DIR 			= drupal_get_path('module', 'pdf_generator').'/upload/clra_reg_qr/formV';
	
	include "./sites/all/modules/pdf_generator/phpqrcode/qrlib.php";
	
	if (!file_exists($PNG_TEMP_DIR))
		mkdir($PNG_TEMP_DIR);
		
		$filename 				= $PNG_TEMP_DIR.'test.png';
		$errorCorrectionLevel 	= 'H';
		$matrixPointSize 		= 2;
	
		$qr_content = 'Serial Number: 00'.$result2['id']."\nReg. No.: ".$result2['registration_number']."\nReg Date: ".date("dS M, Y", strtotime($result2['registration_date']))."\nNature of Work :- ".$cont_work_name."\nWorkers : ".$result2['contractor_max_no_of_labours_on_any_day'];
	
		$filename =	$PNG_TEMP_DIR.encryption_decryption_fun('encrypt', time()).'.png';
	
		QRcode::png($qr_content, $filename, $errorCorrectionLevel, $matrixPointSize, 4); 
		$generate_qr_image 		= $PNG_WEB_DIR.basename($filename);
	
		$qr_image = '<img src="'.drupal_get_path('module', 'pdf_generator').'/upload/clra_reg_qr/'.basename($filename).'">';
	
	/***** Generation of QR Code Ends**************/
	
	
		
	$page_output = '';
	$page_output = '<style type="text/css">
							table, td, th {height:25px;color:#000;margin-top:20px;background:#fff;}
							th {background-color: #008BD1;color: white;}
							td:hover {background-color:#d4e3e5;}
					    </style>					
					
						<table width="100%" border="0" cellspacing="0" cellpadding="0">
							<tr><td colspan="2">&nbsp;</td></tr>
							<tr><td colspan="2">&nbsp;</td></tr>
							<tr><td colspan="2">&nbsp;</td></tr>
							<tr><td colspan="2" align="center" style="font-size:22px;font-family:Times New Roman;"><strong>FORM V</strong></td></tr>
							<tr><td colspan="2" align="center" style="font-size:13px;font-family:Times New Roman;">[ See Rule 21(2) ]</td></tr>
							<tr><td colspan="2"></td></tr>							
							<tr><td colspan="2" align="center" style="font-size:18px;font-family:Comic Sans MS;">Form of Certificate by Principal Employer</td></tr>							
							<tr><td colspan="2" style="font-size:16px;text-align:center;font-family:Times New Roman;">Serial number : '.str_pad($result2['id'], 7, "0", STR_PAD_LEFT).'</td>
							<tr><td colspan="2"></td></tr>							
							<tr><td colspan="2" style="word-break: break-all;font-size:13px;line-height:25px;font-family: Times New Roman, Times, serif;text-align:justify;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Certified that I have engaged the applicant <span style="text-transform:uppercase; font-weight:bold;">'.$result2['name_of_contractor'].',&nbsp;'.$contractorAddress.'</span> as a contractor in my establishment. I undertake to be bound by all the provisions of the Contract Labour (Regulation &amp; Abolition) Act,1970, and the West Bengal Contract Labour (Regulation and Abolition) Rules, 1972, in so far as the provisions are applicable to me in respect of the employment of contract labour by the applicant in my establishment.</td>
							</tr>
							<tr><td colspan="2">&nbsp;</td></tr>
							<tr><td colspan="2">&nbsp;</td></tr>
							<tr><td colspan="2">&nbsp;</td></tr>
							<tr><td colspan="2">&nbsp;</td></tr>
							<tr><td colspan="2">&nbsp;</td></tr>
							<tr>
								<td width="50%" style="font-family: Times New Roman, Times, serif;">Place : '.$result2['sub_div_name'].'</td>
								<td width="48%" style="font-family: Times New Roman, Times, serif;">'.$result2['full_name_principal_emp'].'</td>
							</tr>
							<tr>
								<td width="50%" style="font-family: Times New Roman, Times, serif;">Date : '.date("dS M, Y").'</td>
								<td width="48%" align="center" style="font-family: Times New Roman, Times, serif;">Signature of Principal Employer<br/><b>'.$result2['registration_number'].'</b></td>
							</tr>
							<tr><td colspan="2">&nbsp;</td></tr>
							</table>
							
							<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center">
							<tr>
							  <td width="52%">'.$qr_image.'</td>
							  <td width="40%" style="font-size:11px;line-height:20px;padding-top:30px;"><strong>NAME AND ADDRESS OF ESTABLISHMENT</strong><br/>'.$result2['e_name'].'<br/>'.$establishment_address.', West Bengal</td>
							</tr>
							</table>
							<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center">
								<tr><td colspan="2">&nbsp;</td></tr>
								<tr><td colspan="2">&nbsp;</td></tr>
								<tr><td colspan="2">&nbsp;</td></tr>
								<tr><td colspan="2">&nbsp;</td></tr>
								<tr><td colspan="2">&nbsp;</td></tr>
								<tr><td style="font-family: Times New Roman, Times, serif;font-size:11px;"> Maximum Number of Contractor Labour to be Employed on any day Through Each Contractor to be engaged :- '.$result2['contractor_max_no_of_labours_on_any_day'].'</td></tr>
								<tr><td style="font-family: Times New Roman, Times, serif;font-size:11px;"> Nature of Work :- '.$cont_work_name.'</td></tr>
							</table>
							
							
							
																				
							
						</table>
						';
		return pdf_using_mpdf_api($page_output, $file_name);	
}
?>