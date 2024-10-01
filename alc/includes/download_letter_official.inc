<?php

function download_official_letter($form, &$form_state, $id,$ticket_no=''){
	
	$application_id	= 	encryption_decryption_fun('decrypt', $id);
	$ticket_id		= 	encryption_decryption_fun('decrypt', $ticket_no);
	
	$getData = 	db_select('l_migrant_jobseeker_action_master', 'action');
	$getData ->	leftJoin('l_migrant_jobseeker_master', 'master', 'master.id=action.application_id');
	$getData ->fields('master', array('name','aadhar_no','permanent_pin','permanent_areatype','permanent_address','permanent_areatype'));	
	$getData ->fields('action', array('flag_code'));
	
	$getData ->fields('dm', array('district_name'));
	$getData ->leftJoin('district_master', 'dm', 'dm.district_code=master.permanent_dist');
	
	$getData ->fields('sd', array('sub_div_name'));
	$getData ->leftJoin('sub_division', 'sd', 'sd.sub_div_code=master.permanent_subdivision');
	
	$getData->fields('bmm', array('block_mun_name'));
	$getData->leftJoin('block_mun_master', 'bmm', 'bmm.block_code=master.permanent_areacode');
	
	$getData->fields('vwm', array('village_name'));
	$getData->leftJoin('village_ward_master', 'vwm', 'vwm.village_code=master.permanent_villward');
	
	$getData->fields('ps', array('name_of_police_station'));
	$getData->leftJoin('police_station', 'ps', 'ps.police_station_code=master.permanent_ps');
	
	$getData -> condition('action.application_id', $application_id);	
	$getData ->	condition('action.ticket_no', $ticket_id);
	$getData -> orderBy('action.ticket_no','DESC');
	$getData->range(0, 1);	
	$getData_res = $getData->execute()->fetchAssoc(); 
	
	
	if($getData_res['permanent_areatype']=="B"){
		$text_AreaType_loc = "Block - ";
		if($getData_res['permanent_villward']!=''){
			$areatype_loc_name = ucwords($getData_res['village_name']).',&nbsp;';
		}else{
			$areatype_loc_name ='';
		}
	}elseif($getData_res['permanent_areatype']=="M"){
		if($getData_res['permanent_villward']!=''){
			$areatype_loc_name = 'Ward-'.$getData_res['village_name'].',&nbsp;'; 
		}else{
			$areatype_loc_name ='';
		}
	}elseif($getData_res['permanent_areatype']=="C"){
		if($getData_res['permanent_villward']!=''){
			$areatype_loc_name = $getData_res['village_name'].',&nbsp;';
		}else{
			$areatype_loc_name ='';
		}
	}elseif($getData_res['permanent_areatype']=="N"){
		if($getData_res['permanent_villward']!=''){
			$areatype_loc_name = $getData_res['village_name'].',&nbsp;';
		}else{
			$areatype_loc_name ='';
		}
	}elseif($getData_res['permanent_areatype']=="S"){
		if($getData_res['permanent_villward']!=''){
			$areatype_loc_name = $getData_res['village_name'].',&nbsp;';
		}else{
			$areatype_loc_name ='';
		}
	}
	
	if($getData_res['permanent_dist'] == 17){
		$permanent_address = $getData_res['permanent_address'].',&nbsp;'.$areatype_loc_name.',&nbsp;PS - '.$getData_res['name_of_police_station'].',&nbsp;'.$getData_res['district_name'].'- '.$getData_res['permanent_pin'];			
	}else{
		$permanent_address = $getData_res['permanent_address'].',&nbsp;'.$areatype_loc_name.$text_AreaType_loc.$getData_res['block_mun_name'].',&nbsp;'.$getData_res['sub_div_name'].' - '.$getData_res['permanent_pin'].',&nbsp;PS - '.$getData_res['name_of_police_station'].',&nbsp;'.$getData_res['district_name'];
	}
	
	if($getData_res['flag_code'] == 'PL'){
		$name = 'Pandemic/Lockdown like situation';
	}else if($getData_res['flag_code'] == 'DT'){
		$name = 'Death';
	}else if($getData_res['flag_code'] == 'FA'){
		$name = 'Fatal Accident';
	}else if($getData_res['flag_code'] == 'CF'){
		$name = 'Confinement';
	}
	
	
	
	$getWorkDetails	 =	db_select('l_migrant_jobseeker_work_details','wrksite');	
	$getWorkDetails  ->	fields('wrksite',array());
	$getWorkDetails  ->fields('dm', array('statename'));
	$getWorkDetails  ->leftJoin('state_master', 'dm', 'dm.id=wrksite.present_state');
	$getWorkDetails	 ->	condition('wrksite.application_id',$application_id,'=');
	$getWorkDetails	 ->	orderBy('wrksite.id','DESC');
	$workDetails	 = 	$getWorkDetails->execute()->fetchAssoc(); 
	
	if($workDetails['present_country'] ==1){
		$statename = $workDetails['statename'];
	}else{
		$statename = $workDetails['present_country_name'].'. Passport No.' .$workDetails['passport_no'];
	}
	
						
	
	
	$page_output = '';
	$page_output = '<style type="text/css">
							table, td {font-size:14px;font-family:Times New Roman;height:25px;color:#000;margin-top:20px;background:#fff;}
							th{font-size:13px;font-family:Times New Roman;}
							.td_border{border:1px solid #000;font-size:12px;padding: 8px;font-style:italic; border-right:none;border-bottom:none;"}
							.div-head{font-weight:bold;font-size:15px;text-align:center;font-family:Times New Roman;padding:8px;}
					    </style>';
	$page_output = '<table width="100%" border="0" cellspacing="0" cellpadding="0">
					<tr><td colspan="3">&nbsp;</td></tr>
					<tr><td>To, </td><td></td><td></td></tr>
					<tr><td colspan="3">&nbsp;</td></tr>
					<tr><td>&nbsp;&nbsp;The District Magistrate </td><td></td><td></td></tr>
					<tr><td colspan="3">&nbsp;</td></tr>
					<tr><td colspan="3">&nbsp;&nbsp;Copy to The Labour Commissionerate </td></tr>
					<tr><td colspan="3">&nbsp;</td></tr>
					<tr><td colspan="3">&nbsp;</td></tr>
					<tr><td colspan="3">&nbsp;</td></tr>
					<tr><td colspan="3">&nbsp;</td></tr>
					<tr><td>&nbsp;&nbsp;Dear Sir, &nbsp;&nbsp;</td><td></td><td></td></tr>
					<tr><td colspan="3">&nbsp;</td></tr>
					<tr><td colspan="3">&nbsp;</td></tr>
					<tr><td colspan="3" style="text-align:justify; font-size:13px; line-height:25px; padding:0 10px 0 10px;">&nbsp;&nbsp;It has been informed that an emeregency has been raised by <strong><i>'.$getData_res['name'].'</i></strong> resident of  <strong><i> '.$permanent_address.'</i></strong> with Aadhar No.'.$getData_res['aadhar_no'].' working in '.$workDetails['statename'].'. As per emeregency record he is type of emergency in <strong>'.$name.' </strong></td></tr>
					<tr><td colspan="3">&nbsp;</td></tr>
					<tr><td colspan="3">&nbsp;</td></tr>
					<tr><td colspan="3">&nbsp;</td></tr>
					<tr><td colspan="3">&nbsp;</td></tr>
					</table>
					<table width="96%" border="0" cellspacing="0" cellpadding="0" align="center">
						<tr><td></td></tr>
						<tr><td></td></tr>
						<tr><td></td></tr>
						<tr>
							<td width="60%"></td>
							<td valign="bottom" style="font-size:13px;text-align:center; line-height:20px;padding-top:80px;">Signature and Seal <br/>of<br/>Assiatant Labour Commissioner<br/>'.$getData_res['sub_div_name'].'</td>
						</tr>
					</table>';
	
	return pdf_using_mpdf_api($page_output, 'OFFICIAL-LETTER-'.$ticket_id.'-'.time());	
}
?>
