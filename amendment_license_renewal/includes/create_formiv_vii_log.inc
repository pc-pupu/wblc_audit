<?php
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');

function create_formiv_vii_log($from_v,$renewallicenseammendmentid,$licenseid,$x='Application Form'){
	
	$amendlog_id 	= encryption_decryption_fun('decrypt', $renewallicenseammendmentid);
	 $from_v_no 						= encryption_decryption_fun('decrypt', $from_v);
	$license_id =encryption_decryption_fun('decrypt', $licenseid);
	 create_application_from($amendlog_id,$license_id,$from_v_no);
	
}
function create_application_from($amendlog_id,$license_id,$from_v_no){
	global $user;
	$contractor_id 				=  	$user->uid;	
	//echo 'amend--'.$amendlog_id.'<br>license--'.$license_id.'<br>form V--'.$from_v_no;die;
	$details_ammendment_table 	=	fetch_amendment_log_details($amendlog_id);
	$reference_no 				= 	$details_ammendment_table['reference_no'];
	$application_id				=	$details_ammendment_table['application_id'];
	$is_flag					= 	$details_ammendment_table['is_flag'];
	$parent_id					= 	$details_ammendment_table['license_renewal_ammendment_id'];
	
	if(!empty($reference_no)){
		$remark_details = last_remark_activity($reference_no);
		
		if(!empty($remark_details)){
			$amendment_check_field = $remark_details['amendment_check_field'];
			if(!empty($amendment_check_field)){
					$editable_field 	= explode(',', $amendment_check_field);
			
					foreach($editable_field as $row){
				switch($row){//echo $row;
					case ( $row == "name_of_contractor"||$row =="contractor_dist"||$row =="father_name_of_contarctor"||$row =="category_of_contractor"||$row =="date_of_birth_age"):
							 $ammend_name  = '<font color="#FF0000">**</font>';
							break;
						       
					case  "worksite_address":
					        $ammend_worksite  = '<font color="#FF0000">**</font>';
							break;
					case ( $row == "contractor_max_no_of_labours_on_any_day"||$row =="co_oparative"):
					
					        $max_no_of_labours_chk  =  '<font color="#FF0000">**</font>';
							break;
									
					case  "address_of_manager":
					        $address_of_manager_chk  = '<font color="#FF0000">**</font>';
							break;
					case ($row == "unskilled_rate_wages"||$row =="semiskilled_rate_wages"||$row =="skilled_rate_wages"||$row =="highlyskilled_rate_wages"):
					        $wages_info  = '<font color="#FF0000">**</font>';
							break;
					case ( $row == "hour_details"||$row =="spred_over"||$row =="Overtime"||$row =="overtime_wages"):
					        $hour_details_info  = '<font color="#FF0000">**</font>';
							break;
					
					case ($row == "annual_leave"||$row =="casual_leave"||$row =="eraned_leave"||$row =="sick_leave"||$row =="maternity_leave" || $row=="other_leave"||$row =="weekly_holiday"):
					        $other_condition_info  = '<font color="#FF0000">**</font>';
							break;
					
					case "special_benifites" :
					       $special_benifites_chk  = '<font color="#FF0000">**</font>';
						  break;
				 	case "state_insurance" :
					       $state_insurance_chk  = '<font color="#FF0000">**</font>';
						  break;
					case "miscellaneous" :
					       $miscellaneous_chk  = '<font color="#FF0000">**</font>';
						  break;
					case "offence" :
					       $offence_chk  = '<font color="#FF0000">**</font>';
						  break;
				
					case "revoking" :
					       $revoking_chk  = '<font color="#FF0000">**</font>';
						  break;	
					
				}
			}
			}
		}
	}
	
	$details 	 						= 	contractor_regmaster_details_issued_license($application_id,$from_v_no);;
	$establishment_address				=	"";
	$establishment_address				=	miscellaneous_establishment_address_form($details['loc_e_areatype'],$serial_no,$details['id']);
	$establishment_address				=	$details['e_name'].'<br>'.$establishment_address;
	$nature_of_work_pe 					= 	'';
	$nature_of_work_pe 					= 	nature_of_work_pe($details['id']);
	$principal_address					=	miscellaneous_principal_employer($loc_emp_areatype,$serial_no,$details['id']);//----miscellaneous module
	$registration_date_timestamp	 	=  strtotime($details['registration_date']);
	$formatted_reg_date 				= 	date('dS M Y',$registration_date_timestamp);
	$establish_details_reg				=	"";
	$establish_details_reg				=  $establish_details_reg."Registration Number: ".$details['registration_number']."<br/>"."Date of Certificates: ".$formatted_reg_date."\nUnder The acts of CLRA";	
	$principal_address					=	miscellaneous_principal_employer($loc_emp_areatype,$serial_no,$details['id']);//----miscellaneous module
	$principal_details					=	$details['full_name_principal_emp']."<br>".$details['address_principal_emp']."<br>".$principal_address;
	$labour_from_date					=	$details['est_date_of_work_of_each_labour_from_date'];
	$labour_to_date						=	$details['est_date_of_work_of_each_labour_to_date'];
	$name_of_contractor_pervious	 	= 	$details['name_of_contractor'];
	$contractor_max_no_of_labours_pe	=	$details['contractor_max_no_of_labours_on_any_day'];
	$state_opts							=	$details['state_opts'];
	//$pe_contractor_address				=   $details['address_of_contractor'].'<br/>'. miscellaneous_contractor_address_form($details['con_loc_e_areatype'],$from_v_no);
	if($state_opts == 1){
		$pe_contractor_address			=   $details['address_of_contractor'].'<br/>'. miscellaneous_contractor_address_form($details['con_loc_e_areatype'],$serial_no);
	}if($state_opts == 2){
		$pe_contractor_address			=  $details['address_of_contractor'].'<br/>'. $details['state_others'];
	}
	$result_work 						= 	get_nature_of_work_contrcator($details['id'],$from_v_no); //---miscellaneous module
	if($result_work->rowCount() > 0 ){
				foreach($result_work as $objwork){			
					$nature_work=$nature_work.$objwork->cont_work_name.', ';		  
				}
			}
	$sub_con		=	substr($nature_work, 0, -2);
	$nature_work	=	$sub_con;	
    $estimated_date						=	date_calculation($labour_from_date,$labour_to_date);
	
		$record = fetch_vaild_date($contractor_id,$parent_id,$is_flag);
	if(!empty($record)){
			$license_no 		= !empty($record['backlog_license_no'])?$record['backlog_license_no'].'<br/>Ref No.'.$record['contractor_license_number']:$record['contractor_license_number'];
			$license_date 		= !empty($record['backlog_license_date'])?$record['backlog_license_date']:$record['license_date'];
			$validated_date 	= !empty($record['next_renweal_date'])?date('dS M Y', strtotime($record['next_renweal_date'])):'';
			$license_renewal_ammendment_certificate_id = !empty($record['certificateid'])?$record['certificateid']:'';
	}
	
	
	
	$name_of_contractor 			 = $details_ammendment_table['name_of_contractor'];
	$contractor_father_name 		 = !empty($details_ammendment_table['father_contarctor_name'])?$details_ammendment_table['father_contarctor_name']:'Nil';
	//$address_line1_of_contractor	 = !empty($details_ammendment_table['contractor_areatype'])? ammendment_address($license_id,'ammendment_contrcator_address') :'';
	$address_of_contractor			= $details_ammendment_table['address_of_contractor'];
	$fieldArr_contractor 			 =  array('contractor_dist','contractor_subdivision','contractor_areatype','contractor_name_areatype','contractor_vill_ward','contractor_ps','contractor_pin','contractor_state');
    $address_line1_of_contractor	 =	$address_of_contractor.'<br>'.get_full_address('l_amendment_of_contract_labour_license_log','sub-table',$amendlog_id,$fieldArr_contractor); 
	$age_contractor					 = !empty($details_ammendment_table['age_contractor'])?$details_ammendment_table['age_contractor']:'Nil';
	$dob_contractor					 = !empty($details_ammendment_table['dob_contractor'])?date('dS M Y', strtotime($details_ammendment_table['dob_contractor'])):'Nil';
	$name_of_agent					 = !empty($details_ammendment_table['name_of_agent'])?$details_ammendment_table['name_of_agent'] :'Nil'; 
	//$address_line1_of_manager		 = !empty($details_ammendment_table['contractor_manager_areatype'])? ammendment_address($license_id,'ammendment_manager_address'):'Nil'; 
	$address_line_of_manager		= $details_ammendment_table['address_of_manager'];
	 
	$fieldArr_manager_amm			=  	array('contractor_manager_dist','contractor_manager_subdivision','contractor_manager_areatype','contractor_manager_name_areatype','contractor_managerr_vill_ward','contractor_manager_ps','contractor_manager_pin','manager_state');
  $address_line1_of_manager			= $address_line_of_manager.'<br>'.get_full_address('l_amendment_of_contract_labour_license_log','sub-table',$amendlog_id,$fieldArr_manager_amm);
	$max_of_contract_labour 		 = $details_ammendment_table['max_of_contract_labour'];
	$category_designation 			 = $details_ammendment_table['category_designation'];
	$unskilled_rate_wages 			 = !empty($details_ammendment_table['unskilled_rate_wages'])?$details_ammendment_table['unskilled_rate_wages']:'Nil';
	$semiskilled_rate_wages 		 = !empty($details_ammendment_table['semiskilled_rate_wages'])?$details_ammendment_table['semiskilled_rate_wages']:'Nil';
	$skilled_rate_wages 			 = !empty($details_ammendment_table['skilled_rate_wages'])?$details_ammendment_table['skilled_rate_wages']:'Nil';
	$highlyskilled_rate_wages 		 = !empty($details_ammendment_table['highlyskilled_rate_wages'])?$details_ammendment_table['highlyskilled_rate_wages']:'Nil';
	$hours_work						 = !empty($details_ammendment_table['hours_work'])?$details_ammendment_table['hours_work']:'Nil';
    $spred_over 					 = !empty($details_ammendment_table['spred_over'])?$details_ammendment_table['spred_over']:'Nil';
	$overtime_wages					 = !empty($details_ammendment_table['overtime_wages'])?$details_ammendment_table['overtime_wages']:'Nil';
	$overtime						 = !empty($details_ammendment_table['overtime'])?$details_ammendment_table['overtime']:'Nil';
	$annual_leave_no 				 = !empty($details_ammendment_table['annual_leave_no'])?$details_ammendment_table['annual_leave_no']:'Nil';
	$casual_leave_no 				 = !empty($details_ammendment_table['casual_leave_no'])?$details_ammendment_table['casual_leave_no']:'Nil';
	$sick_leave_no 					 = !empty($details_ammendment_table['sick_leave_no'])?$details_ammendment_table['sick_leave_no']:'Nil';
	$maternity_leave_no 			 = !empty($details_ammendment_table['maternity_leave_no'])?$details_ammendment_table['maternity_leave_no']:'Nil';
	$earned_leave_no 				 = !empty($details_ammendment_table['earned_leave_no'])?$details_ammendment_table['earned_leave_no']:'Nil';
	$other_leave_no 				 = !empty($details_ammendment_table['other_leave_no'])?$details_ammendment_table['other_leave_no']:'Nil';
	$no_holiday						 = !empty($details_ammendment_table['no_holiday'])?$details_ammendment_table['no_holiday']:'';	
	$details_contractor_convicted	= !empty($details_ammendment_table['details_contractor_convicted'])?$details_ammendment_table['details_contractor_convicted']:'Nil';
	$details_contractor_revoking	= !empty($details_ammendment_table['details_contractor_revoking'])?date('dS M Y', strtotime($details_ammendment_table['details_contractor_revoking'])):'Nil';
	$category_of_contractor			= $details_ammendment_table['category_of_contractor'];	
	$work_site_address 				= ammendment_address($license_id,'ammendment_worksite_address');
	$reason_changing_worksite 		= !empty($details_ammendment_table['reason_changing_worksite'])?$details_ammendment_table['reason_changing_worksite']:'Nil';
	$earned_leave_no 				= $details_ammendment_table['earned_leave_no'];
	$holiday_wages					= !empty($details_ammendment_table['holiday_wages'])?$details_ammendment_table['holiday_wages']:'Nil';
	$special_benifites				= !empty($details_ammendment_table['special_benifites'])?$details_ammendment_table['special_benifites']:'Nil';
	$miscellaneous_provisions		= !empty($details_ammendment_table['miscellaneous_provisions'])?$details_ammendment_table['miscellaneous_provisions']:'Nil';
	$state_insurance				= !empty($details_ammendment_table['state_insurance'])?$details_ammendment_table['state_insurance']:'Nil';
	$is_coparative					=  !empty($details_ammendment_table['is_coparative'])?'Yes':'No';
	$applicable_security_fees   	= $details_ammendment_table['applicable_security_fess'];
	$previous_highest_security		= !empty($details_ammendment_table['previous_highest_security'])?$details_ammendment_table['previous_highest_security']:0.00;
	$due_security_fees				= !empty($details_ammendment_table['due_security_fees'])?$details_ammendment_table['due_security_fees']:0.00;	
	$payable_amendment_security_fees = !empty($details_ammendment_table['payable_amendment_security_fees'])?$details_ammendment_table['payable_amendment_security_fees']:0.00;
	$applicable_amendment_fee		= !empty($details_ammendment_table['applicable_amendment_fee'])?$details_ammendment_table['applicable_amendment_fee']:0.00;	
	$previous_highest_deposit_fees	= !empty($details_ammendment_table['previous_highest_deposit_fees'])?$details_ammendment_table['previous_highest_deposit_fees']:0.00;
	$payable_amendment_fees			=	!empty($details_ammendment_table['payable_amendment_fees'])?$details_ammendment_table['payable_amendment_fees']:0.00;		
	$total_payble_amount 			= 	$payable_amendment_fees+$payable_amendment_security_fees;
	if(!empty($no_holiday)){
	$rest_holiday = substr($no_holiday, 0, -1);
	
	$no_holiday_list= explode(',', $rest_holiday);
	
	$name_holiday="";
	$total_holiday=0;
    foreach($no_holiday_list as $row){
		$total_holiday=$total_holiday+1;
		if($row=='1'){
			$name_holiday.='Sunday'.',';
		}
		if($row=='2'){
			$name_holiday.='Monday'.',';
		}
		if($row=='3'){
			$name_holiday.='Tuesday'.',';
		}
		if($row=='4'){
			$name_holiday.='Wednesday'.',';
		}
		if($row=='5'){
			$name_holiday.='Thursday'.',';
		}
		if($row=='6'){
			$name_holiday.='Friday'.',';
		}
		if($row=='7'){
			$name_holiday.='Saturday'.',';
		}
		
	}
	$name_holidays = substr($name_holiday, 0, -1);
	if(empty($name_holidays)){
		$name_holidays='NIL';
	}
	$weekly_days = $total_holiday.' day(s)('.$name_holidays.').';
	}else{
		$weekly_days = 'Nil';
	}
		if(($category_of_contractor=='1')){
		    $type = 'Company';
			$title = 'Date of commencement of business:-'.date('d F, Y',strtotime($dob_contractor));
			//$age_contractor='';
	}else{
		    $type = 'Individuals';
			$title = 'Date of Birth:-'.date('d F, Y',strtotime($dob_contractor)).'<br>Age: '.$age_contractor;
			//$age_contractor= 'Age: '.$age_contractor;
	}
	
	$pervious_license_detais = pe_regmaster_details_amend($from_v_no,$contractor_id);
	if(empty($pervious_license_detais))
	     $pervious_license_detais = "Nil";
	
	$page_output = '';
	$page_output = '<style type="text/css">
								table, td, th , tr{font-size:14px;font-family:Times New Roman;height:25px;color:#000;margin-top:15px;background:#fff;padding:5px;}
					</style>
						<table width="100%" border="0" cellspacing="0" cellpadding="0" >
						<tr>
							<td colspan="2" align="center" style="font-size:19px;"><strong>APPLICATION FOR AMENDMENT OF LICENSE UNDER <br>CONTRACT LABOUR (R&A) ACT,1970</strong><br>License No.-'.$license_no.'<br> Issued Date.-'.$license_date.' &nbsp; &nbsp;Valid upto.-'.$validated_date.'<br><span style="font-size:15px;"> <strong>(Serial No. of FORM-V or Reference No.-00'.$from_v_no.')</strong> </span></td>
						</tr>
						 <tr><td colspan="2"  align="center" style="border-bottom:none;border-top:none;">Fields marked(<font color="#FF0000">**</font>)are to be amended.</td></tr>	
						<tr><td colspan="2" style="border-bottom:none;"></td></tr>					
						<tr><td colspan ="2"  style="border-top:none;"><strong>1.Information is given by Principal Employer</strong></td></tr>
						<tr>
							<td valign="top">(A)Name & address of the establishment</td>
							<td style="padding-left:40px;">'.$establishment_address.'</td>
						</tr>
						 <tr><td colspan="2"></td></tr>
						 <tr>
							<td valign="top">(B)Type of Business, trade, industry, manufacture or occupation which is carried on in the establishment</td>
							<td style="padding-left:40px;">'.$nature_of_work_pe.'</td>
						</tr>   
						<tr><td colspan="2"></td></tr>
						<tr>
							<td valign="top">(C)Number and date of Certificate</td>
							<td style="padding-left:40px;">'.$establish_details_reg.'</td>
						</tr>
						<tr><td colspan="2"></td></tr>
						<tr>
							<td valign="top">(D)Name & Address of the Principal Employer</td>
							<td style="padding-left:40px;">'.$principal_details.'</td>
						</tr>
						<tr><td  colspan="2"></td></tr>
						<tr>
							<td valign="top">(E)Address of the contractor(provided by principal employer)</td>
							<td style="padding-left:40px;">'.$pe_contractor_address.'</td>
						</tr>
						<tr><td  colspan="2"></td></tr>
						<tr>
							<td valign="top">(F)Maximum number of Contract Labour proposed to he employed in the establishment on any date</td>
							<td style="padding-left:40px;">'.$contractor_max_no_of_labours_pe.'</td>
						</tr>
							<tr><td colspan="2"></td></tr>
						<tr>
							<td valign="top">(G)Nature of work in which Contract Labour is employed or is to be employed in the establishment</td>
							<td style="padding-left:40px;">'.$nature_work.'</td>
						</tr>
						
						<tr><td colspan="2"></td></tr>
						<tr>
							<td valign="top">(H)Duration of the proposed contract work (give particulars of proposed date of ending)</td>
							<td style="padding-left:40px;">'.$estimated_date.'</td>
						</tr>
						<tr><td  colspan="2"></td></tr>
  	<tr><td colspan="2"><strong>2.Name & address of the contractor (Official Information)</strong></td></tr>
						<tr>
							<td valign="top" width="50%"> (A)Name of the contractor &nbsp; '.$ammend_name.'</td>
							<td style="padding-left:40px;">'.$name_of_contractor.'</td>
						</tr>
						<tr><td  colspan="2"></td></tr>
						<tr>
							<td valign="top" width="50%"> (B)Addres of the contractor&nbsp; '.$ammend_name.'</td>
							<td style="padding-left:40px;">'.$address_line1_of_contractor.'</td>
						</tr>
						<tr><td colspan="2"></td></tr>
						<tr>
							<td valign="top" width="50%"> (C)Father name of contractor &nbsp;'.$ammend_name.'</td>
							<td style="padding-left:40px;">'.$contractor_father_name.'</td>
						</tr>
						<tr><td colspan="2"></td></tr>
						
						
						<tr>
							<td valign="top">(D)Category of Contractor &nbsp;'.$ammend_name.' </td>
							<td style="padding-left:40px;">'.$type.'<br>'.$title.'</td>
						</tr>
						
						<tr><td colspan="2"></td></tr>
						
						<tr><td colspan ="2"><strong>3.Contractor Labour and Worksite Details</strong></td></tr>
						<tr>
							<td valign="top">(A)Work site Address &nbsp;'.$ammend_worksite.'</td>
							<td style="padding-left:40px;">'.$work_site_address.'</td>
						</tr>
						
							
						
						<tr><td colspan="2"></td></tr>
						
						<tr>
							<td valign="top">(B)Maximum number of Contract Labour proposed to be employed in the establishment on any date.&nbsp;'.$max_no_of_labours_chk.'</td>
							<td style="padding-left:40px;">'.$max_of_contract_labour.'</td>
						</tr>
						<tr><td colspan="2"></td></tr>
						
						<tr>
							<td valign="top">(C)Co-oparative Society. &nbsp;'.$max_no_of_labours_chk.'</td>
							<td style="padding-left:40px;">'.$is_coparative.'</td>
						</tr>
						<tr><td colspan="2"></td></tr>
						<tr>
							<td valign="top">(D)Applicable Amendment Fees. &nbsp;'.$max_no_of_labours_chk.'</td>
							<td style="padding-left:40px;">'.$applicable_amendment_fee.' /-</td>
						</tr>
							<tr><td colspan="2"></td></tr>
						<tr>
							<td valign="top">(E)Highest Security Fees paid during license/subsequent amendments. &nbsp;'.$max_no_of_labours_chk.'</td>
							<td style="padding-left:40px;">'.$previous_highest_deposit_fees.' /-</td>
						</tr>
						<tr><td colspan="2"></td></tr>
						<tr>
							<td valign="top">(F)Payable Amendment Fees. &nbsp;'.$max_no_of_labours_chk.'</td>
							<td style="padding-left:40px;">'.$payable_amendment_fees.' /-</td>
						</tr> 
						<tr><td colspan="2"></td></tr>
						<tr>
							<td valign="top">(G)Applicable Security Fees. &nbsp;'.$max_no_of_labours_chk.'</td>
							<td style="padding-left:40px;">'.$applicable_security_fees.' /-</td>
						</tr>
						<tr><td colspan="2"></td></tr>
						<tr>
							<td valign="top">(H)Highest Security Fees paid during license/subsequent amendments. &nbsp;'.$max_no_of_labours_chk.'</td>
							<td style="padding-left:40px;">'.$previous_highest_security.' /-</td>
						</tr> 
						<tr><td colspan="2"></td></tr>
						<tr>
							<td valign="top">(I)Due Security Fees. &nbsp;'.$max_no_of_labours_chk.'</td>
							<td style="padding-left:40px;">'.$due_security_fees.' /-</td>
						</tr>
						<tr><td colspan="2"></td></tr>
						<tr>
							<td valign="top">(J)Payable Amendment security Fees. &nbsp;'.$max_no_of_labours_chk.'</td>
							<td style="padding-left:40px;">'.$payable_amendment_security_fees.' /-</td>
						</tr>
						<tr><td colspan="2"></td></tr>
						<tr>
							<td valign="top">(K)Total payable amount is. &nbsp;'.$max_no_of_labours_chk.'</td>
							<td style="padding-left:40px;">'.$total_payble_amount.' /-</td>
						</tr>
						<tr><td colspan="2"></td></tr>
						<tr><td colspan ="2"><strong>4.Particular of Contract Labour</strong></td></tr>
						<tr>
						<td valign="top">(A)Name & address of the agent or Manager of Contractor at the work site. &nbsp; '.$address_of_manager_chk.'</td>
							<td style="padding-left:40px;">'.$name_of_agent.'<br /> '.$address_line1_of_manager.'</td>
						</tr>
						<tr><td colspan="2"></td></tr>
						<tr>
							<td valign="top">(B)Category/designation/<br>nomenclature of the contractor labour, namely,fitter,welder,carpenter,mazdor etc</td>
							<td style="padding-left:40px;">'.$category_designation.'</td>
						</tr>
						<tr><td colspan="2"></td></tr>
						<tr>
    						<td colspan="2"><strong>(C)(i)Rate of Wages, DA and other cash benefits paid/ to be paid to each category(i.e (a):Unskilled (b)SemiSkilled (c)Skilled (d)HighlySkilled etc.)of contract labour:</strong></td>
  						</tr>
   
   		 <tr><td colspan="2"></td></tr>   
		
		<tr>
							<td valign="top">(A)Rate of Wages,DA and other cash benefits paid/ to be paid to Unskilled Category of contract labour &nbsp; '.$wages_info.'</td>
							<td style="padding-left:40px;">'.$unskilled_rate_wages.'</td>
						</tr>
						<tr><td colspan="2"></td></tr>
						<tr>
							<td valign="top">(B)Rate of Wages,DA and other cash benefits paid/ to be paid to Semi-skilled Category of contract labour &nbsp;'.$wages_info.'</td>
							<td style="padding-left:40px;">'.$semiskilled_rate_wages.'</td>
						</tr>
						<tr><td colspan="2"></td></tr>
						<tr>
							<td valign="top">(C)Rate of Wages,DA and other cash benefits paid/ to be paid to Skilled Category of contract labour &nbsp; '.$wages_info.'</td>
							<td style="padding-left:40px;">'.$skilled_rate_wages.'</td>
						</tr>
   						<tr><td colspan="2"></td></tr>
						<tr>
							<td valign="top">(D)Rate of Wages,DA and other cash benefits paid/ to be paid to Highly_Skilled Category of contract labour &nbsp; '.$wages_info.'</td>
							<td style="padding-left:40px;">'.$highlyskilled_rate_wages.'</td>
						</tr>
						<tr><td colspan="2"></td></tr>
						<tr>
							<td valign="top">(C)(ii)Hours of Work,Overtime and Overtime Wages &nbsp; '.$hour_details_info.'</td>
							<td style="padding-left:40px;">Hours of Work- '.$hours_work.' hr(s),&nbsp;Overtime- '.$overtime.' hr(s),&nbsp;Overtime Wages- &#8377; '.$overtime_wages.'(per hour)</td>
						</tr>
   						<tr><td colspan="2"></td></tr>
						<tr>
    						<td colspan="2"><strong>(C)(iii)Other Condition of service like leave (annual leave,casual leave,sick leave,maternity leave etc)Holidays etc.of the contract labour:</strong></td>
  						</tr>
   						<tr>
						<td valign="top"(A)>Annual leave &nbsp;'.$other_condition_info.'</td>
						<td style="padding-left:40px;">'.$annual_leave_no.'	</td>
						</tr>
						<tr>
						<td valign="top">(B)Casual leave &nbsp;'.$other_condition_info.'</td>
						<td style="padding-left:40px;">'.$casual_leave_no.'	</td>
						</tr>
						<tr>
						<td valign="top">(C)Sick leave &nbsp;'.$other_condition_info.'</td>
						<td style="padding-left:40px;">'.$sick_leave_no.'	</td>
						</tr>
						<tr>
						<td valign="top">(D)Maternity leave &nbsp;'.$other_condition_info.'</td>
						<td style="padding-left:40px;">'.$maternity_leave_no.'	</td>
						</tr>
						<tr>
						<td valign="top">(E)Earned leave &nbsp;'.$other_condition_info.'</td>
						<td style="padding-left:40px;">'.$earned_leave_no.'	</td>
						</tr>
						<tr>
						<td valign="top">(F)Other leave &nbsp;'.$other_condition_info.'</td>
						<td style="padding-left:40px;">'.$other_leave_no.'	</td>
						</tr>
						<tr>
						<td valign="top">(G)Whether weekly holiday(s) observed and on which day &nbsp;'.$other_condition_info.'</td>
						<td style="padding-left:40px;">'.$weekly_days .'</td>
						</tr>
						<tr>
						<td valign="top">(H)Whether weekly holiday(s) so observed was paid holiday(s)  &nbsp;'.$other_condition_info.'</td>
						<td style="padding-left:40px;">'.$holiday_wages.'</td>
						</tr>
						
						<tr><td colspan="2"></td></tr>
						<tr>
							<td valign="top">5. Whether the contractor was convicted of any offence within the preceding five years. If so, give details</td>
							<td style="padding-left:40px;">'.$details_contractor_convicted.'</td>
						</tr>
						<tr><td  colspan="2"></td></tr>
						<tr>
						 <td  valign="top">6.Whether there was any order against the contract or revoking or suspending license or forfeiting security deposit in respect of an earlier contract. If so, the date of such order. &nbsp;'.$revoking_chk.' </td>
						 <td style="padding-left:40px;">'.$details_contractor_revoking.'</td>
						</tr>
						<tr><td  colspan="2"></td></tr>
						<tr>
							<td valign="top">7.Whether the contractor has worked in any other establishment within the past five years. If so. give details of the principal employer, establishment and nature of work</td>
							<td style="padding-left:40px;">'.$pervious_license_detais.'</td>
						</tr>
						<tr><td  colspan="2"></td></tr>
						<tr>
							<td valign="top">8.Special benefits provided, if any  &nbsp;'.$special_benifites_chk.'</td>
							<td style="padding-left:40px;">'.$special_benifites.'</td>
						</tr>
						<tr><td  colspan="2"></td></tr>
						<tr>
							<td valign="top">9.Contribution made under the Employees Provident Fund and Miscellaneous Provision Act,1952&nbsp;'.$miscellaneous_chk.' </td>
							<td style="padding-left:40px;">'.$miscellaneous_provisions.'</td>
						</tr>
						<tr><td  colspan="2"></td></tr>
						<tr>
							<td valign="top">10.Contribution made under the Employees State Insurance Act,1948 &nbsp;'.$state_insurance_chk.' </td>
							<td style="padding-left:40px;">'.$state_insurance.'</td>
						</tr>
						<tr><td  colspan="2"></td></tr>
						
					   <tr><td colspan="2"></td></tr>';
					$security_fees_payment = !empty($payable_amendment_security_fees)?'&#8377; '.$payable_amendment_security_fees.' ( Rupees '.convertNumberToWord($amount_of_security_deposit):'Nil';
					   $transaction_details  			= db_select('l_principle_epayments_receive_data', 'lpd');
		$transaction_details			->leftJoin('l_principle_epayments_data', 'lped', 'lpd.transaction_id=lped.identification_no');
		$transaction_details			->fields('lped', array('identification_no', 'application_id'));
		$transaction_details			->fields('lpd', array());
		$transaction_details			->condition('lped.act_id', '14');
		$transaction_details			->condition('lped.application_id', $reference_no);		
		$trans_details_result 			= $transaction_details->execute()->fetchAll();
		
		if(!empty($trans_details_result)){
			foreach($trans_details_result as $row){
					$bankTransactionID	= !empty($row->transaction_id)?$row->transaction_id:"Payment Not required" ;
					$grn_number			= !empty($row->challanrefid)?$row->challanrefid:"Payment Not required";
					$challan_fid_date	= !empty($row->challanrefid_date) ? substr($row->challanrefid_date,0,10) : "Payment Not required";
					$total_amount		= !empty($row->challanamount)?number_format($row->challanamount, 2):"Payment Not required";
					$bank_code			= !empty($row->bank_cd)?$row->bank_cd:"Payment Not required";
		
					   
					$page_output .= '   <tr>
							<td valign="top">11.Amount of Licence Fee paid-no. of treasury challan and date</td>
							<td style="padding-left:40px;">Bank Code:-'.$bank_code.',<br/>GRN:- '.$grn_number.',<br/>Amendment Fees:- &#8377; '.$payable_amendment_fees.' ( Rupees '.convertNumberToWord($payable_amendment_fees). ',<br/>Transaction Date:- '.$challan_fid_date.'</td>
					  </tr>
					  <tr><td  colspan="2"></td></tr>
					   <tr>
							<td valign="top">12.Amount of security deposit-treasury receipt no. and date</td>
							<td style="padding-left:40px;">Bank Code:-'.$bank_code.',<br/>GRN:- '.$grn_number.',<br/>Security Fees:- '.$security_fees_payment. ',<br/>Transaction Date:- '.$challan_fid_date.'</td>
						 </tr>
						  <tr><td colspan="2" style="border-bottom:none;border-top:none;"></td></tr>';
			}
		}
						 
						$page_output .= '   <tr>
						  	<td colspan="2" style="border-bottom:none;border-top:none;">Note:-The application shall be accompanied by the amount of fees as applicable for amendment </td>
						  </tr>
						  <tr><td colspan="2" style="border-bottom:none;border-top:none;"></td></tr>						  
						  <tr><td colspan="2" style="border-bottom:none;border-top:none;"><strong>Declaration:</strong> I hereby declare that the details given above are correct to the best of my knowledge and belief.</td></tr>
						  <tr><td colspan="2" style="border-bottom:none;border-top:none;"></td></tr>
						  <tr><td colspan="2" style="border-bottom:none;border-top:none;"></td></tr>
						  <tr><td colspan="2" style="border-bottom:none;border-top:none;"></td></tr>
						  <tr><td colspan="2" style="border-bottom:none;border-top:none;"></td></tr>
						  <tr><td colspan="2" style="border-bottom:none;border-top:none;"></td></tr>						  
						  <tr>
								<td align="left" style="border-bottom:none;border-top:none;border-right:none;"><strong><i>Place :'.$work_site_address.' </strong></i></td>
								<td align="right" style="border-bottom:none;border-top:none; border-left:none;"><strong>Signature of the Applicant</strong></td>
							</tr>
							<tr></tr>
							<tr>
								<td align="left" style="border-bottom:none;border-top:none; border-right:none;"><strong>Date : </strong>'.date("dS M, Y").'</td>
								<td align="right" style="border-bottom:none;border-top:none; border-left:none;"><strong>(Contractor)<strong></td>
							</tr>
						   <tr><td colspan="2" style="border-bottom:none;border-top:none;"></td></tr>
						  <tr><td colspan="2" style="border-bottom:none;border-top:none;"></td></tr>
						  <tr><td colspan="2" style="border-bottom:none;border-top:none;"></td></tr>
						  
						 </table>';
						  
	
	

	return pdf_using_mpdf_api($page_output, 'Form-IV-'.time());

}

?>