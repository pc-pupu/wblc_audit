<?php

drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');

function check_amendment($form, &$form_state,$serialno = ''){
	
	

	$serial_no											 =  encryption_decryption_fun('decrypt', $serialno);

	/*$description 										= t('<span class="amend-des"><font color="#009900"> Free</font><span>');
	$description_payable 								= t('<span class="amend-des"><font color="#f00"> Payable</font><span>');*/
	
	
	//$activity_tag = contractor_activity_tag($serial_no);
	
	// $fetch_remark		 = 	db_select('l_contractor_activity_tag', 'tag');
  	// $fetch_remark		->	fields('tag',array());
	// $fetch_remark		->	condition('application_status','I');
	// $fetch_remark		->	condition('from_v_serial_no',$serial_no);
	// $fetch_remark		->	orderBy('id','DESC');
	// $fetch_remark		->	range(0, 1);
	// $activity_tag 		= 	$fetch_remark->execute()->fetchAssoc();
	
	$activity_tag = db_query("select * from l_contractor_activity_tag where trim(application_status) ='I' and from_v_serial_no = ".$serial_no." order by id desc limit 1")->fetchAssoc();
	// echo "<pre>";
	// 	print_r($activity_tag);die;
	
	if(!empty($activity_tag)){
		
		 $renewallicenseammendmentid = $activity_tag['renewal_license_ammendment_id'];
		 $application_flag 			= $activity_tag['flag'];
	}
	
	$record = basic_information_for_ammendment($serial_no);
	
	
	
	if(!empty($record)){
	$license_no = !empty($record['backlog_license_no'])?$record['backlog_license_no'].'<br/>Ref No.'.$record['contractor_license_number']:$record['contractor_license_number'];
	$license_date = !empty($record['backlog_license_date'])?$record['backlog_license_date']:$record['license_date'];
	$validated_date = !empty($record['next_renweal_date'])?date('dS M Y', strtotime($record['next_renweal_date'])):'';
	}
	
	if($application_flag == 'L'){
		$license_id								=	$renewallicenseammendmentid;//---kaj baki
	}if($application_flag == 'A'){
		//$license_id								=	return_ammendement_license_id($renewallicenseammendmentid);//---kaj baki
		
		 // edited by jyoti 23/02/2023
		 $license_id								=	get_amd_license_id($renewallicenseammendmentid);
		
	}if($application_flag == 'R'){
		$license_id								=	return_renewal_license($renewallicenseammendmentid);
	}
  
	$check_ammendment = check_ammendment_fields($license_id,$serial_no)->fetchAssoc();
	
	// echo $renewallicenseammendmentid.'--'.$serial_no;
	// 		echo "<pre>";
	// 	print_r($activity_tag);die();
		
	

	if(!empty($check_ammendment['amendment_check_field'])){
		$editable_fields = substr($check_ammendment['amendment_check_field'], 0, -1);
		$editable_field 			= explode(',', $editable_fields);
		
			foreach($editable_field as $row){
				switch($row){ 
					case ($row == "name_of_contractor"||$row == "contractor_dist" ||$row == "father_name_of_contarctor" ||$row == "category_of_contractor" ||$row == "date_of_birth_age"):
					        $personal_info  = array('checked' => 'checked');
							break;
					case  "worksite_address":
					        $worksite_info  = array('checked' => 'checked');
							break;
					case  "contractor_max_no_of_labours_on_any_day":
					        $max_no_of_labours  = array('checked' => 'checked');
							break;
					case  "address_of_manager":
					        $address_of_manager  = array('checked' => 'checked');
							break;
					case  "category":
					        $category_info  = array('checked' => 'checked');
							break;
					case ($row == "unskilled_rate_wages"||$row =="semiskilled_rate_wages"||$row =="skilled_rate_wages"||$row =="highlyskilled_rate_wages"):
					        $wages_info  = array('checked' => 'checked');
							break;
					case ( $row == "hour_details"||$row =="spred_over"||$row =="Overtime"||$row =="overtime_wages"):
					        $hour_details_info  = array('checked' => 'checked');
							break;
					case ($row == "annual_leave"||$row =="casual_leave"||$row =="eraned_leave"||$row =="sick_leave"||$row =="maternity_leave" || $row=="other_leave"||$row =="weekly_holiday"):
					        $other_condition_info  = array('checked' => 'checked');
							break;
					case  "special_benifites":
					        $special_benifites  = array('checked' => 'checked');
							break;
					case  "state_insurance":
					        $state_insurance  = array('checked' => 'checked');
							break;
					case  "miscellaneous":
					        $miscellaneous  = array('checked' => 'checked');
							break;
					case  "offence":
					        $offence  = array('checked' => 'checked');
							break;
					case  "pervious_license":
					        $pervious_license  = array('checked' => 'checked');
							break;
					case  "revoking":
					        $revoking  = array('checked' => 'checked');
							break;
					
					
					
				}
			}
	}
	
	
		
	/*$form['amend_form']['header'] 	= array(
			'#markup' 		=> t('<div class="content">
				<div class="sky-form" style="width:100%;">
					<header>
						<center>
							<div style="font-size:18px;">
									<span>Form-V/Reference Number: <font color="#0F0106"><strong>00'.$serial_no.'</strong></font></span><br/>
									<span> License Number:  <font color="#0F0106"><strong>'.$license_no.'</strong></font></span>
									<br/>
									<span>Issue Date: <font color="#0F0106"><strong>'.date('dS M Y', strtotime($license_date)).'</strong></font>&nbsp;&nbsp;
									Valid Upto: <font color="#0F0106"><strong>'.$validated_date.'</strong></font></span>
							</div></center></header>'),
					'#type' 		=> 'markup'
			);
	*/
	
	$form['amend_form']['header']			= array(
				'#markup' 			=>	 t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading"><center><span>Form-V/Reference Number: <font color="#d3e1ec"><strong>00'.$serial_no.'</strong></font></span><br/>
	<span> License Number:  <font color="#d3e1ec"><strong>'.$license_no.'</strong></font></span>
	<br/>
	<span>Issue Date: <font color="#d3e1ec"><strong>'.date('dS M Y', strtotime($license_date)).'</strong></font>&nbsp;&nbsp;
	Valid Upto: <font color="#d3e1ec"><strong>'.$validated_date.'</strong></font></span></center>									
	</div>') );
	
	$form['amend_form']['reference_no'] 		= array(
			'#prefix' 			=> 	'<div class="panel-body"><div class="sky-form"><div class="cus-amment-check-box">',
			
			'#required'			=>	TRUE,
			'#description' 		=> 	$link_form_V,
			'#attributes'		=> 	array('autocomplete' => 'off')
			);
			
	$form['amend_form']['serial_no'] = array(
						'#type' 		=> 'hidden',
						'#default_value'=> $serialno,
						'#attributes'	=> array('readonly' => 'readonly'), 								
						);
																						
	$form['amend_form']['applicationflag'] = array(
						'#type' 		=> 'hidden',
						'#default_value'=> encryption_decryption_fun('encrypt', $application_flag),
						'#attributes'	=> array('readonly' => 'readonly'), 								
						);
							
	$form['amend_form']['renewallicenseammendmentid'] = array(
						'#type' 		=> 'hidden',
						'#default_value'=> encryption_decryption_fun('encrypt', $renewallicenseammendmentid),
						'#attributes'	=> array('readonly' => 'readonly'), 								
						);
		 
	$form['amend_form']['note'] = array(
						'#prefix' 		=> '<section><label class="checkbox"> ',
						'#suffix' 		=> '</label></section>',
						'#markup' 		=> t('Note : Tick the fields which is to be amended by the Contractor. '),				
						'#type' 		=> 'markup',														
						);
	$slno = 1;					
	$form['amend_form']['worksite_address'] = array(
						'#prefix' 		=> '<section><label class="checkbox">',
						'#suffix' 		=> '</label></section>',
						'#title' 		=> t($slno++.'.Address Line1 of the worksite'),				
						'#type' 		=> 'checkbox',
						'#attributes'	=> $worksite_info,
						// '#description' 	=> $description,								
					);
					
	$form['amend_form']['contrctor_details'] = array(
						'#prefix' 		=> '<section><label class="checkbox">',
						'#suffix' 		=> '</label></section>',
						'#title' 		=> t($slno++.'.Address of the contractor'),				
						'#type' 		=> 'checkbox',
						'#attributes'	=>	$personal_info,
						'#description' 	=> $description,								
					);
																										
	$form['amend_form']['manager_details'] = array(
						'#prefix' 		=> '<section><label class="checkbox">',
						'#suffix' 		=> '</label></section>',
						'#title' 		=> t($slno++.'.Name and address of the agent or manager of contractor at worksite '),				
						'#type' 		=> 'checkbox',
						'#attributes'	=>	$address_of_manager,
						'#description' 	=> $description,								
						);
	
	$form['amend_form']['max_labour'] = array(
						'#prefix' 		=> '<section><label class="checkbox">',
						'#suffix' 		=> '</label></section>',
						'#title' 		=> t($slno++.'.Maximum number of Contrct Labour proposed to be employed in the establishment on any day'),				
						'#type' 		=> 'checkbox',
						'#attributes'	=>	$max_no_of_labours,
						'#description' 	=> $description_payable,								
						);

										
	$form['amend_form']['category'] = array(
						'#prefix' 		=> '<section><label class="checkbox">',
						'#suffix' 		=> '</label></section>',
						'#title' 		=> t($slno++.'.Category/designation/nomenclature of the contrctor labour, namely,filtter,welder,carpanter,mazdor etc.'),				
						'#type' 		=> 'checkbox',
						'#attributes'	=>	$category_info,
						'#description' 	=> $description,								
						);
													
	$form['amend_form']['rate_wages'] 	= array(
						'#prefix' 		=> '<section><label class="checkbox">',
						'#suffix' 		=> '</label></section>',
						'#title' 		=> t($slno++.'.(a).Rate of wages,DA and other cash benefites paid,to be paid to each category(i.e.(a):Unskilled(b)Semi-skilled(c)Skilled(d)Highly-Skilledetc.) of contrct labour'),				
						'#type' 		=> 'checkbox',
						'#attributes'	=>	$wages_info,
						'#description' 	=> $description,								
						);

	$form['amend_form']['work_wages'] = array(
						'#prefix' 		=> '<section><label class="checkbox">',
						'#suffix' 		=> '</label></section>',
						'#title' 		=> t($slno.'.(b).Daliy Hours of work, overtime,overtime wages and spread over time'),				
						'#type' 		=> 'checkbox',
						'#attributes'	=>	$hour_details_info,
						'#description' 	=> $description,								
						);
													
	

	$form['amend_form']['leave_details'] = array(
						'#prefix' 		=> '<section><label class="checkbox">',
						'#suffix' 		=> '</label></section>',
						'#title' 		=> t($slno.'.(c).Other condition of service like leave (annual leave,casual leave,sick leave, maternity leave etc) holiday etc. of the contrct labour)'),				
						'#type' 		=> 'checkbox',
						'#attributes'	=>	$other_condition_info,
						'#description' 	=> $description,								
						);
	$form['amend_form']['special_benifites'] = array(
						'#prefix' 		=> '<section><label class="checkbox">',
						'#suffix' 		=> '</label></section>',
						'#title' 		=> t($slno++.'.Special benifites provide, if any'),				
						'#type' 		=> 'checkbox',
						'#attributes'	=>	$special_benifites,
						'#description' 	=> $description,								
						);
	
	$form['amend_form']['state_insurance'] = array(
						'#prefix' 		=> '<section><label class="checkbox">',
						'#suffix' 		=> '</label></section>',
						'#title' 		=> t($slno++.'.Contribution made under the Employees State Insurance Act,1984'),				
						'#type' 		=> 'checkbox',
						'#attributes'	=>	$state_insurance,
						'#description' 	=> $description,								
						);
	$form['amend_form']['miscellaneous'] = array(
						'#prefix' 		=> '<section><label class="checkbox">',
						'#suffix' 		=> '</label></section>',
						'#title' 		=> t($slno++.'.Contribution made under the Employees Provident Fund and Miscellaneous Provision Act,1952'),				
						'#type' 		=> 'checkbox',
						'#attributes'	=>	$miscellaneous,
						'#description' 	=> $description,								
						);
	$form['amend_form']['convicted'] = array(
						'#prefix' 		=> '<section><label class="checkbox">',
						'#suffix' 		=> '</label></section>',
						'#title' 		=> t($slno++.'.Whether the contrctor was convicted of any offence within the preceding five yeras. If so, give details'),				
						'#type' 		=> 'checkbox',
						'#attributes'	=>	$offence,
						'#description' 	=> $description,								
						);

	$form['amend_form']['past_five'] = array(
						'#prefix' 		=> '<section><label class="checkbox">',
						'#suffix' 		=> '</label></section>',
						'#title' 		=> t($slno++.'.Whether the contrctor has worked in any other establishment within the past five years. If so give details of the parincipal employer, establishment'),				
						'#type' 		=> 'checkbox',
						'#attributes'	=>	$pervious_license,
						'#description' 	=> $description,								
						);
	$form['amend_form']['revoking'] = array(
						'#prefix' 		=> '<section><label class="checkbox">',
						'#suffix' 		=> '</label></section><br/>',
						'#title' 		=> t($slno++.'.Whether there was any order against the contract or revoking or suspending license or forfeiting security deposit in respect of an earlier contract. If so, the date of such order'),				
						'#type' 		=> 'checkbox',
						'#attributes'	=>	$revoking,
						'#description' 	=> $description,								
						);

	

	$form['amend_form']['name_data'] = array(
			
			'#markup' 		=>'<font color="#FF0000">Note:-The Principal Employer may be requested to amend the contrctor list in his certificate of registration in case of change of Name of the contractor</font>' 	,		
			'#type' 		=> 'markup',														
		);
	//--left according to sudhansu					
	$form['amend_form']['submit'] 			= array (
						'#prefix' 		=> '<br><br><hr><br>',
						'#suffix' 		=> '</div></div></div></div></div>',  
						'#type' 		=> 'submit',
						'#value' 		=> 'Apply',
						'#attributes'	=> array('id' => 'submit_oneId' , 'class' => array('button left'))
					);
	return $form;
}

function check_amendment_submit($form, &$form_state){
	
	global $user;
	
	$user_id						= 	$user->uid;
	
	$val 							= 	$form_state['values'];


 	$serial_no						= encryption_decryption_fun('decrypt', $val['serial_no']);
	$renewallicenseammendment_id	= encryption_decryption_fun('decrypt', $val['renewallicenseammendmentid']);
	$application_flag				= encryption_decryption_fun('decrypt', $val['applicationflag']);
	$renewallicenseammendmentid		= $val['renewallicenseammendmentid'];
	
	//$record = contractor_activity_tag($serial_no);
	
	/*if(!empty($record)){
		
		$renewallicenseammendmentid = $record['renewallicenseammendmentid'];
		$application_flag 			= $record['flagapplication'];
	}*/
	if($application_flag == 'L'){
		$license_id								=	$renewallicenseammendment_id;//---kaj baki
	}if($application_flag == 'A'){
		//$license_id								=	return_ammendement_license_id($renewallicenseammendment_id);//---kaj baki
		 // edited by jyoti 23/02/2023
		 $license_id								=	get_amd_license_id($renewallicenseammendment_id);
	}if($application_flag == 'R'){
		$license_id								=	return_renewal_license($renewallicenseammendment_id);//---kaj baki
	}
	
	$check_filed = '';
	$uncheck_filed ='';
	
	//$contrctor_details 	= !empty($val['contrctor_details'])?$check_filed .='name_of_contractor,contractor_dist,father_name_of_contarctor,category_of_contractor,date_of_birth_age,':$uncheck_filed .='name_of_contractor,contractor_dist,father_name_of_contarctor,category_of_contractor,date_of_birth_age,';
	$contrctor_details 	= !empty($val['contrctor_details'])?$check_filed .='contractor_dist,father_name_of_contarctor,category_of_contractor,date_of_birth_age,':$uncheck_filed .='name_of_contractor,contractor_dist,father_name_of_contarctor,category_of_contractor,date_of_birth_age,';
	$manager_details 	= !empty($val['manager_details'])?$check_filed .='address_of_manager,':$uncheck_filed .='address_of_manager,';
	$max_labour 		= !empty($val['max_labour'])?$check_filed .='contractor_max_no_of_labours_on_any_day,co_oparative,':$uncheck_filed .='contractor_max_no_of_labours_on_any_day,co_oparative,';
	$worksite_address	= !empty($val['worksite_address'])?$check_filed .='worksite_address,change_reason,':$uncheck_filed .='worksite_address,change_reason,';
	$category 			= !empty($val['category'])?$check_filed .='category,':$uncheck_filed .='category,';
	$rate_wages 		= !empty($val['rate_wages'])?$check_filed .='unskilled_rate_wages,semiskilled_rate_wages,skilled_rate_wages,highlyskilled_rate_wages,':$uncheck_filed .='unskilled_rate_wages,semiskilled_rate_wages,skilled_rate_wages,highlyskilled_rate_wages,';
	$work_wages 		= !empty($val['work_wages'])?$check_filed .='hour_details,spred_over,overtime,overtime_wages,':$uncheck_filed .='hour_details,spred_over,overtime,overtime_wages,';
	$leave_details 		= !empty($val['leave_details'])?$check_filed .='annual_leave,casual_leave,eraned_leave,sick_leave,maternity_leave,other_leave,weekly_holiday,':$uncheck_filed .='annual_leave,casual_leave,eraned_leave,sick_leave,maternity_leave,other_leave,weekly_holiday,';
	$special_benifites 	= !empty($val['special_benifites'])?$check_filed .='special_benifites,':$uncheck_filed .='special_benifites,';
	$state_insurance 	= !empty($val['state_insurance'])?$check_filed .= 'state_insurance,':$uncheck_filed .= 'state_insurance,';
	$miscellaneous 	    = !empty($val['miscellaneous'])?$check_filed .= 'miscellaneous,':$uncheck_filed .= 'miscellaneous,';
	$convicted 	        = !empty($val['convicted'])?$check_filed .= 'offence,':$uncheck_filed .= 'offence,';
	$state_insurance 	= !empty($val['past_five'])?$check_filed .= 'pervious_license,':$uncheck_filed .= 'pervious_license,';
	$revoking			= !empty($val['revoking'])?$check_filed .= 'revoking,':$uncheck_filed .= 'revoking,';
	
	$array_check_ammendment = array('amendment_check_field' => $check_filed ,'conrtactor_user_id'=>$user_id,'license_id' => $license_id,'not_amendment_check_field' => $uncheck_filed,'from_v_serial_no' => $serial_no);
	
	$result = check_ammendment_fields($license_id,$serial_no);
	
	if(empty($check_filed)){
		$message = 'cont procced';
		drupal_set_message(t($message));

	}else{
	$count	= $result->rowcount();

	if($count>=1){	
		
			$query_contract_labour 		= 	db_update('l_amendment_check_table');
			$query_contract_labour		->	fields($array_check_ammendment);
			$query_contract_labour		->	condition('license_id',$license_id);
			$query_contract_labour		->	execute();
			}else{
					db_insert('l_amendment_check_table')->fields($array_check_ammendment)->execute();
			}
			
			
	drupal_goto('amendment_license_renewal/apply_amendment/'.encryption_decryption_fun('encrypt', $serial_no).'/'. $renewallicenseammendmentid.'/'.encryption_decryption_fun('encrypt', $application_flag));
	
}
			
		
	

}
?>