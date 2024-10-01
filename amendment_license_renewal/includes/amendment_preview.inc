<?php

drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');


function amendment_preview($form, &$form_state,$serialno='' ,$ammendid= ''){
	global $user;
	$contractor_id 				=  $user->uid;	
	$serial_no					=  encryption_decryption_fun('decrypt', $serialno);
	$ammend_id				=  encryption_decryption_fun('decrypt', $ammendid); 
	
	$form['amendment_preview']['serial_no'] = array(
			'#type' 		=> 'hidden',
			'#default_value'=>  $serialno,
			'#attributes'	=> array('readonly' => 'readonly'), 								
			); 
	$form['amendment_preview']['ammend_id'] = array(
			'#type' 		=> 'hidden',
			'#default_value'=>  $ammendid,
			'#attributes'	=> array('readonly' => 'readonly'), 								
			);
	
	$record = basic_information_for_ammendment($serial_no);
	if(!empty($record)){
			$license_no 		= !empty($record['backlog_license_no'])?$record['backlog_license_no'].'<br/>Ref No.'.$record['contractor_license_number']:$record['contractor_license_number'];
			$license_date 		= !empty($record['backlog_license_date'])?$record['backlog_license_date']:$record['license_date'];
			$validated_date 	= !empty($record['next_renweal_date'])?date('dS M Y', strtotime($record['next_renweal_date'])):'';
			$license_renewal_ammendment_certificate_id = !empty($record['certificateid'])?$record['certificateid']:'';
	}
	
	$form['amendment_preview']['note'] = array(
			'#prefix' 		=> '<div class="content"><div class="sky-form" style="width:100%;"><header><center>
								<div style="font-size:18px;">
									<span>Form-V/Reference Number: <font color="#0F0106"><strong>00'.$serial_no.'</strong></font></span><br/>
									<span> License Number:  <font color="#0F0106"><strong>'.$license_no.'</strong></font></span>
									<br/>
									<span>Issue Date: <font color="#0F0106"><strong>'.date('dS M Y', strtotime($license_date)).'</strong></font>&nbsp;&nbsp;
										Valid Upto: <font color="#0F0106"><strong>'.$validated_date.'</strong></font></span>
								</div></center></header><fieldset>',
			'#suffix' 		=> '</label></section>',
			'#markup' 		=> 	details_preview($serial_no,$ammend_id,$license_renewal_ammendment_certificate_id),		
			'#type' 		=> 'markup',														
		);
	$form ['amend_form']['submit'] = array (
			  '#prefix' 		   => '</fieldset><footer>',  
			 '#suffix' 				=> '</footer>',
			  '#attributes' 		=> array('class' => array('button middle-align')),
			  '#type' 				=> 'submit',  
			  '#submit' 			=> array('print_preview'),
			  '#value'				=> 'PRINT PREVIEW'
	  );
		return $form;
}
function details_preview($serial_no,$ammend_id,$license_renewal_ammendment_certificate_id){
	
	global $base_root, $base_path, $user;
	
	$contractor_id 				= 	$user->uid;
	$edit_personal_details  	= '';
	$details 					= 	contractor_regmaster_details($serial_no);
	$license_id					=	return_ammendement_license_id($ammend_id);//---kaj baki
	
	
	
	
	$details_principal_employer 	 	= '';
	$registration_date_timestamp	 	= strtotime($details['registration_date']);
	$formatted_reg_date 				= date('dS M Y', $registration_date_timestamp);
	
	$establish_details_reg				=	"";
	$establish_details_reg				=	$establish_details_reg."Registration Number: ".$details['registration_number']."<br/>"."Date of Certificates: ".$formatted_reg_date."\nUnder The acts of CLRA";	
	
	$establishment_address				=	"";
	$establishment_address				=	miscellaneous_establishment_address_form($details['loc_e_areatype'],$serial_no,$details['id']);
	$establishment_address				=	$details['e_name'].'<br/>'.$establishment_address;
	$nature_of_work_pe 					= 	'';
	$nature_of_work_pe 					= 	nature_of_work_pe($details['id']);
	$principal_address					=	miscellaneous_principal_employer($loc_emp_areatype,$serial_no,$details['id']);//----miscellaneous module
	$principal_details					=	$details['full_name_principal_emp']."<br>".$details['address_principal_emp']."<br>".$principal_address;
	$labour_from_date					=	$details['est_date_of_work_of_each_labour_from_date'];
	$labour_to_date						=	$details['est_date_of_work_of_each_labour_to_date'];
	$name_of_contractor_pervious	 	= 	$details['name_of_contractor'];
	$state_opts							=	$details['state_opts'];
	//$pe_contractor_address				=   $details['address_of_contractor'].'<br/>'. miscellaneous_contractor_address_form($details['con_loc_e_areatype'],$serial_no);
	if($state_opts == 1){
		$pe_contractor_address			=   $details['address_of_contractor'].'<br/>'. miscellaneous_contractor_address_form($details['con_loc_e_areatype'],$serial_no);
	}if($state_opts == 2){
		$pe_contractor_address			=  $details['address_of_contractor'].'<br/>'. $details['state_others'];
	}
	$contractor_max_no_of_labours_pe	=	$details['contractor_max_no_of_labours_on_any_day'];
    $estimated_date						=	date_calculation($labour_from_date,$labour_to_date);
	
	
	$result_work 						= get_nature_of_work_contrcator($details['id'],$serial_no); //---miscellaneous module
	
	if($result_work->rowCount() > 0 ){
				foreach($result_work as $objwork){			
					$nature_work=$nature_work.$objwork->cont_work_name.', ';		  
				}
			}
			$sub_con=substr($nature_work, 0, -2);
			$nature_work=$sub_con;	
	
		  $result					= 	particular_file($serial,$a);
		  $frm_v_file_id			=	$result['frm_v_file_id'];
		  $residential_file_id		=	$result['residential_file_id'];

	
	$check_ammendment_table 		= check_ammendment_table_with_id($ammend_id);	
	$details_ammendment_table 		=  $check_ammendment_table->fetchAssoc();
	
	
    $name_of_contractor 			= $details_ammendment_table['name_of_contractor'];
	$father_contarctor_name 		= $details_ammendment_table['father_contarctor_name'];
	//$contractor_convicted			= date('dS M, Y',strtotime($details_ammendment_table['details_contractor_convicted'];
	$contractor_convicted			= !empty($details_ammendment_table['details_contractor_convicted'])?$details_ammendment_table['details_contractor_convicted']:'Nil';
	
	$contractor_revoking			= $details_ammendment_table['details_contractor_revoking'];
	//$address_line1_of_contractor	= ammendment_address($license_id,'ammendment_contrcator_address') ;
	$address_of_contractor			= $details_ammendment_table['address_of_contractor'];
	$fieldArr_contractor 			 =  array('contractor_dist','contractor_subdivision','contractor_areatype','contractor_name_areatype','contractor_vill_ward','contractor_ps','contractor_pin','contractor_state');
    $address_line1_of_contractor	 =	$address_of_contractor.'<br>'.get_full_address('l_amendment_of_contract_labour_license','sub-table',$ammend_id,$fieldArr_contractor);
	
	
	
	$name_of_agent				    = $details_ammendment_table['name_of_agent'];
	//$address_line1_of_manager		= ammendment_address($license_id,'ammendment_manager_address');
	$address_line_of_manager		= $details_ammendment_table['address_of_manager'];
	 
	$fieldArr_manager_amm			=  	array('contractor_manager_dist','contractor_manager_subdivision','contractor_manager_areatype','contractor_manager_name_areatype','contractor_managerr_vill_ward','contractor_manager_ps','contractor_manager_pin','manager_state');
  $address_line1_of_manager			= $address_line_of_manager.'<br>'.get_full_address('l_amendment_of_contract_labour_license','sub-table',$ammend_id,$fieldArr_manager_amm);
	$category_of_contractor			= $details_ammendment_table['category_of_contractor'];
	$dob_contractor					= $details_ammendment_table['dob_contractor'];
	$age_contractor					= $details_ammendment_table['age_contractor'];
	$pe_userid						= $details_ammendment_table['user_id'];
	$application_id					= $details_ammendment_table['application_id'];
	$reference_no					= $details_ammendment_table['reference_no'];
	$age_contractor					= $details_ammendment_table['age_contractor'];
	if(!empty($dob_contractor)){
		$dob_contractor_new = date('dS M, Y',strtotime($dob_contractor));
	}else{
		$dob_contractor_new = 'Nil';
	}if(!empty($age_contractor)){
		$age_contractor_new = $age_contractor.'Years';
	}else{
		$age_contractor_new = 'Nil';
	}
	if(($category_of_contractor=='1' && $category_of_contractor)){
		    $type = 'Company';
			$title = 'Date of commencement of business';
			$age_contractor='';
	}else{
		    $type = 'Individuals';
			$title = 'Date of Birth and age (incase of individuals)';
			$age_contractor= 'Age: '.$age_contractor_new;
	}
	$unskilled_rate_wages 						= !empty($details_ammendment_table['unskilled_rate_wages'])?$details_ammendment_table['unskilled_rate_wages']:'Nil';
	$semiskilled_rate_wages 					= !empty($details_ammendment_table['semiskilled_rate_wages'])?$details_ammendment_table['semiskilled_rate_wages']:'Nil';
	$skilled_rate_wages 					 	= !empty($details_ammendment_table['skilled_rate_wages'])?$details_ammendment_table['skilled_rate_wages']:'Nil';
	$highlyskilled_rate_wages 					= !empty($details_ammendment_table['highlyskilled_rate_wages'])?$details_ammendment_table['highlyskilled_rate_wages']:'Nil';
	$hours_work									= !empty($details_ammendment_table['hours_work'])?$details_ammendment_table['hours_work']:'Nil';
    $spred_over 								= !empty($details_ammendment_table['spred_over'])?$details_ammendment_table['spred_over']:'Nil';
	$overtime_wages								= !empty($details_ammendment_table['overtime_wages'])?$details_ammendment_table['overtime_wages']:'Nil';
	$overtime								    = !empty($details_ammendment_table['overtime'])?$details_ammendment_table['overtime']:'Nil';	
	$annual_leave_no 							= !empty($details_ammendment_table['annual_leave_no'])?$details_ammendment_table['annual_leave_no']:'Nil';
	$casual_leave_no 							= !empty($details_ammendment_table['casual_leave_no'])?$details_ammendment_table['casual_leave_no']:'Nil';
	$sick_leave_no 								= !empty($details_ammendment_table['sick_leave_no'])?$details_ammendment_table['sick_leave_no']:'Nil';
	$maternity_leave_no 						= !empty($details_ammendment_table['maternity_leave_no'])?$details_ammendment_table['maternity_leave_no']:'Nil';
	$earned_leave_no 							= !empty($details_ammendment_table['earned_leave_no'])?$details_ammendment_table['earned_leave_no']:'Nil';
	$other_leave_no 							= !empty($details_ammendment_table['other_leave_no'])?$details_ammendment_table['other_leave_no']:'Nil';
	$category_designation 						= !empty($details_ammendment_table['category_designation'])?$details_ammendment_table['category_designation']:'Nil';
	$work_site_address 							= ammendment_address($license_id,'ammendment_worksite_address');
	$max_of_contract_labour 					= $details_ammendment_table['max_of_contract_labour'];
	
	$holiday_wages								=!empty($details_ammendment_table['holiday_wages'])?$details_ammendment_table['holiday_wages']:'Nil';
	$no_holiday									=$details_ammendment_table['no_holiday'];
	
	$upload_singed_from_id 						= $details_ammendment_table['upload_singed_from'];
	
	
	
	
	$miscellaneous_provisions					=!empty($details_ammendment_table['miscellaneous_provisions'])?$details_ammendment_table['miscellaneous_provisions']:'Nil';
	$state_insurance							= !empty($details_ammendment_table['state_insurance'])?$details_ammendment_table['state_insurance']:'Nil';
	$special_benifites							= !empty($details_ammendment_table['special_benifites'])?$details_ammendment_table['special_benifites']:'Nil';
	$due_security_fees							= !empty($details_ammendment_table['due_security_fees'])?$details_ammendment_table['due_security_fees']:0.00;	
	$reason_changing_worksite					= !empty($details_ammendment_table['reason_changing_worksite'])?$details_ammendment_table['reason_changing_worksite']:'Not Avalible'; 
	$applicable_security_fees   				= $details_ammendment_table['applicable_security_fess'];
	$applicable_amendment_fee   				= $details_ammendment_table['applicable_amendment_fee'];
	$previous_highest_deposit_fees				= $details_ammendment_table['previous_highest_deposit_fees'];
	$payable_amendment_fees   					= !empty($details_ammendment_table['payable_amendment_fees'])?$details_ammendment_table['payable_amendment_fees']:'0.00';
	$previous_highest_security					= !empty($details_ammendment_table['previous_highest_security'])?$details_ammendment_table['previous_highest_security']:0.00;
	$payable_amendment_security_fees			= !empty($details_ammendment_table['payable_amendment_security_fees'])?$details_ammendment_table['payable_amendment_security_fees']:0.00;
	$license_renewal_ammendment_id				= $details_ammendment_table['license_renewal_ammendment_id'];
	$is_coparative								= !empty($details_ammendment_table['is_coparative'])?'YES':'NO';
	$worksite_subdivision						= $details_ammendment_table['worksite_subdivision'];
	$subdivnam 									= get_subdivname($worksite_subdivision);	
	
	$result 									= particular_data($serial_no);
	$work_order_file_id_pre						= $result['work_order_file_id'];
	$frm_v_file_id								= $result['frm_v_file_id'];
	$residential_file_id						= $result['residential_file_id'];
	$other_file_id_pre							= $result['other_file_id'];
	
	$is_flag									= $details_ammendment_table['is_flag'];
	
	if($is_flag == 'R'){
		$renewal_data 							= renewal_data_last($license_renewal_ammendment_id);
	
		$form_iv_vii_id							= $renewal_data['upload_signed_form'];
		$certificate_id							= $renewal_data['renewal_certificate_id'];
	}
	
	if($is_flag == 'L'){
		
		$form_iv_vii_id							=	$result['signed_license_id'];
		$certificate_id 						=   $license_renewal_ammendment_certificate_id;
	}
	if($is_flag == 'A'){
		
		$form_iv_vii_id							=	$details_ammendment_table['upload_singed_from'];
		//$certificate_id							=	!empty($details_ammendment_table['ammendment_certificate_id'])?$details_ammendment_table['ammendment_certificate_id']:$certificate_id_renewal_licesne;
		  $amendment_log_details 				= 	contract_labour_amendment_log($ammend_id);
		  $form_iv_vii_id						=	$amendment_log_details['upload_singed_from'];
		  $certificate_id						=	$amendment_log_details['ammendment_certificate_id'];
		  $other_file_id_pre					=   $amendment_log_details['other_file_id'];	
		
	}
	$other_file_id 								= !empty($details_ammendment_table['other_file_id'])?$details_ammendment_table['other_file_id']:$other_file_id_pre;
	$work_order_file_id							= !empty($details_ammendment_table['work_order_file_id'])?$details_ammendment_table['work_order_file_id']:$work_order_file_id_pre;

	if($work_order_file_id!=''){
		$uploaded_workorder_file = fetch_uplaoded_file_details($work_order_file_id);
		if(!empty($uploaded_workorder_file)){ 
				$url			 	= 	explode('//',$uploaded_workorder_file['uri']);
				$work_order_url		= 	$url[1];
				$workorder_file		=	$uploaded_workorder_file['filename'];
		}
	}
	if($frm_v_file_id!=''){
		$uploaded_frmv_file = fetch_uplaoded_file_details($frm_v_file_id);
		if(!empty($uploaded_frmv_file)){ 
				$url 				= explode('//',$uploaded_frmv_file['uri']);
				$frmv_url			= $url[1];
				$frmv_file			= $uploaded_frmv_file['filename'];
		}
	}
	if($residential_file_id!=''){
		$uploaded_residential_file = fetch_uplaoded_file_details($residential_file_id);
		if(!empty($uploaded_residential_file)){ 
				$url				= explode('//',$uploaded_residential_file['uri']);
				$residential_url	= $url[1];
				$residential_file	= $uploaded_residential_file['filename'];
		}
	}
	if($form_iv_vii_id!=''){
		$upload_form_iv_vii_file = fetch_uplaoded_file_details($form_iv_vii_id);
		if(!empty($upload_form_iv_vii_file)){ 
				$url				= explode('//',$upload_form_iv_vii_file['uri']);
				$form_iv_vii_url	= $url[1];
				$form_iv_vii_file	= $upload_form_iv_vii_file['filename'];
		}
	}
	if($other_file_id!=''){
		$upload_other_file = fetch_uplaoded_file_details($other_file_id);
		if(!empty($upload_other_file)){ 
				$url				= explode('//',$upload_other_file['uri']);
				$other_url			= $url[1];
				$other_file			= $upload_other_file['filename'];
		}
	}
	if($certificate_id!=''){
		$upload_certificate_file = fetch_uplaoded_file_details($certificate_id);
		if(!empty($upload_certificate_file)){ 
				$url						= explode('//',$upload_certificate_file['uri']);
				$certificate_url			= $url[1];
				$certificate_file			= $upload_certificate_file['filename'];
		}
	}
	if($upload_singed_from_id!=''){
		$upload_singed_from_file = fetch_uplaoded_file_details($upload_singed_from_id);
		if(!empty($upload_singed_from_file)){ 
				$url									= explode('//',$upload_singed_from_file['uri']);
				$documents_upload_singed_from_url		= $url[1];
				$documents_upload_singed_from			= $upload_singed_from_file['filename'];
		}
	}
	
	// echo $work_order_file_id;die();	
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
	if(empty($name_holidays))
		$name_holidays='NIL';
		$weekly_days = $total_holiday.' day(s)('.$name_holidays.').';
	}else{
		$weekly_days = 'Nil';
	}	

	$output			= '';
	if(!empty($due_security_fees)){
		$due_statement = '<font color="#FF0000"><strong> Your security fees '.$due_security_fees.' is due till now.</strong></font>';
	}else{
		$due_statement = "";
	}	
	$total_payble_amount = $payable_amendment_fees+$payable_amendment_security_fees;
	$pe_details_reg 	= 'View Details';
	$pe_details_reg 	= l($pe_details_reg, 'amendment_license_renewal/view_pe_details/'.encryption_decryption_fun('encrypt', trim($serial_no)).'/'.encryption_decryption_fun('encrypt', trim($contractor_id)), array('html' => TRUE));
	$contractor_revoking_date = !empty($contractor_revoking)?date('dS M, Y',strtotime($contractor_revoking)):'Not Avaliable';
	$output	 = '<div class="con-dtls-view-main">
					<h3>1.Information given by Principal Employer</h3>
					<div class="con-dtls-view-contains">
						<section class="odd">
							<label>Name & address of the establishment</label><span>'.$establishment_address.'</span>
						</section>
									
						<section class="even">
							<label>Type of Business, trade, industry, manufacture or occupation which is carried on in the establishment</label><span>'.$nature_of_work_pe.' </span>
						</section>
									
						<section class="odd">
							 <label>Number and date of Certificate</label><span>'.$establish_details_reg.'</span>
						</section>
									
						<section class="even">
							<label>Name & Address of the Principal Employer</label><span>'.$principal_details.'</span>
						</section>
						<section class="odd">
							<label>Address of the contractor(provided by principal employer)</label><span>'.$pe_contractor_address.'</span>
						</section>
						<section class="even">
							<label>Maximum number of Contract Labour proposed to he employed in the establishment on any date	</label><span>'.$contractor_max_no_of_labours_pe.' </span>
						</section>
						<section class="odd">
							<label>Nature of work in which Contract Labour is employed or is to be employed in the establishment</label><span>'.$nature_work.' </span>
						</section>
								
						<section class="even">
							<label>Duration of the proposed contract work(give particulars of proposed date of ending) </label><span>'.$estimated_date.' </span>
						</section>
									
						
					</div>
			</div>
			<div class="con-dtls-view-main" id="Personal Information"><span>'.$message1.'</span>
			
				<h3>2.Name & address of the contractor (Official Information) <span class="edit">'.$edit_personal_details.'</span></h3>
				<div class="con-dtls-view-contains">
					<section class="odd">
							<label>Name</label><span>'.$name_of_contractor.'</span>
					</section>
									
					<section class="even">
							<label>Address of Contractor</label><span>'.$address_line1_of_contractor.' <br />'. $contractor_address.'</span>
					</section>
									
					<section class="odd">
							 <label>Father name of contarctor</label><span>'.$father_contarctor_name.'</span>
					</section>
									
					<section class="even">
							 <label>Category of Contractor</label><span>'.$type.'</span>
					</section>
					<section class="odd">
							<label>'.$title.'</label><span>'.$dob_contractor_new.'<br />'.$age_contractor.'</span>
					</section>
									
									
				</div>
			</div>
			
			<div class="con-dtls-view-main" id="Labour and Worksite"><span>'.$message2.'</span>
				<h3>3.Contractor Labour and Worksite Details<span class="edit">'.$edit_contractor_labour.'</span></h3>
				<div class="con-dtls-view-contains">
					<section class="even">
							<label>Work site Address</label><span>'.$work_site_address.'</span>
					</section>
					<!--<section class="odd">
							<label>Reason for changeing of worksite details</label><span>'.$reason_changing_worksite1.'</span>
					</section>	-->
					<section class="odd">
							<label>Co-oparative Society</label><span>'.$is_coparative.'</span>
					</section>				
					<section class="even">
							<label>Maximum number of Contract Labour proposed to he employed in the establishment on any date</label><span>'.$max_of_contract_labour.' ('.convertNumberToWord($max_of_contract_labour).')</span>
					</section>
					<section class="odd">
							<label>Applicable Ammendment Fees<font color="#FF0000">**</font></label><span>'.$applicable_amendment_fee.' /-</span>
					</section>
					<section class="even">
							<label>Highest Fees paid during license/subsequent amendments<font color="#FF0000">**</font></label><span>'.$previous_highest_deposit_fees.' /- </span>
					</section>	
					<section class="odd">
							<label>Payble Ammendment Fees<font color="#FF0000">**</font></label><span>'.$payable_amendment_fees.' /-</span>
					</section>	
					<section class="even">
							<label>Applicable Security Fees<font color="#FF0000">**</font></label><span>'.$applicable_security_fees.' /- </span>
					</section>
					<section class="odd">
							<label>Highest Security Fees paid during license/subsequent amendments<font color="#FF0000">**</font></label><span>'.$previous_highest_security.' /-</span>
					</section>								
					<section class="even">
							<label>Due Security Fees<font color="#FF0000">**</font></label><span>'.$due_security_fees.' /-</span>
					</section>
				<section class="odd">
							<label>Payble Ammendment security Fees<font color="#FF0000">**</font></label><span>'.$payable_amendment_security_fees.' /- </span>
					</section>	
				<section class="even">
							<label><strong>Total payble amount is</strong> <font color="#FF0000">**</font></label><span>'.$total_payble_amount.' /- </span>
					</section>									
				</div>
			</div>
			
			
						   
			<div class="con-dtls-view-main" id ="particular"><span>'.$message3.'</span>
				<h3>4. Particular of Contractor Labour <span class="edit">'.$edit_particular_details.'</span></h3>
								
				<div class="con-dtls-view-contains">
					<section class="even">
							<label>Name & address of the agent or Manager of Contractor at the work site</label><span>'.$name_of_agent.'<br /> '.$address_line1_of_manager.'</span>
					</section>
									
					
									
					
									
					<section class="even">
						     <label>Category/designation/nomenclature of the contractor labour, namely, fitter, welder, carpenter, mazdor etc</label><span>'.$category_designation.'</span>
					</section>
										
					<div style="background:#3c4e5a; border:3px solid #e8e8e8;padding:5px;">
										
						<span style="font-weight:bold;font-size: 15px;color: #FFF;"><center>Rate of Wages,DA and other cash benefits paid/ to be paid to each category of contract labour i.e</center><center>  (a):Unskilled (b)Semi Skilled (c)Skilled (d)Highly Skilled etc.)</center></span>							
										
					</div>
					<section class="odd">
							<label>(a)Rate of Wages,DA and other cash benefits paid/ to be paid to Unskilled of contract labour</label><span>'.$unskilled_rate_wages.'</span>
					</section>
					<section class="even">
							<label>(b)Rate of Wages,DA and other cash benefits paid/ to be paid to Semi-skilled of contract labour</label><span>'.$semiskilled_rate_wages.'</span>
					</section>
					<section class="odd">
							<label>(c)Rate of Wages,DA and other cash benefits paid/ to be paid to Skilled of contract labour</label><span>'.$skilled_rate_wages.'</span>
					</section>
					<section class="even">
							<label>(d)Rate of Wages,DA and other cash benefits paid/ to be paid to Highly-skilled of contract labour</label><span>'.$highlyskilled_rate_wages.'</span>
					</section>
									
					<section class="odd">
							<label>(e)Hours of Work, Spread over time, Overtime and Overtime Wages </label><span>Hours of Work:-'.$hours_work.' hr(s)<br> Spread over time:- '.$spred_over.' hr(s) <br> Overtime:- '.$overtime.' hr(s) <br>Overtime Wages:- <del>&#2352;</del> '.$overtime_wages.'(per hour)</span>
					</section>
					<div style="background:#3c4e5a; border:3px solid #e8e8e8;padding:5px;">
						  <span style="font-weight:bold;font-size: 15px;color: #FFF;"><center>Other Condition of service like leave (annual leave,casual leave,sick leave,maternity leave etc.) Holidays etc.of the contract labour</center></span>							
										
					</div>
					<section class="odd">
							<label>(a)Number of Annual leave</label><span>'.$annual_leave_no.'</span>
					</section>
					<section class="even">
							<label>(b)Number of Casual leave</label><span>'.$casual_leave_no.'</span>
					</section>
					<section class="odd">
							<label>(c)Number of Earned leave</label><span>'.$earned_leave_no.'</span>
					</section>
					<section class="even">
							<label>(d)Number of Sick leave</label><span>'.$sick_leave_no.'</span>
					</section>
					<section class="odd">
							<label>(e)Number of Maternity leave</label><span>'.$maternity_leave_no.'</span>
					</section>
					
					 <section class="even">
							<label>(f)Number of Other leave</label><span>'.$other_leave_no.'</span>
					 </section>
					<section class="odd">
							<label>Whether weekly holiday(s) observed and on which day</label><span>'.$weekly_days.'</span>
					</section>
					<section class="even">
							<label>Whether weekly holiday(s) so observed was paid holiday(s)</label><span>'.$holiday_wages.'</span>
					</section>
					<section class="odd">
							<label>Whether the contractor was convicted of any offence within the preceding five years. If so, give details</label><span>'.$contractor_convicted.'</span>
					</section>
					<section class="even">
							<label>Whether there was any order against the contract or revoking or suspending license or forfeiting security deposit in respect of an earlier contract. If so, the date of such order</label><span>Revoking Date :- '.$contractor_revoking_date.'<span style="float:right;">'.$readonly_contractor_revoking.'</span>
					</section>
					<section class="odd">
							<label>Whether the contractor has worked in any other establishment within the past five years. If so. give details of the principal employer, establishment and nature of work</label><span>'.$pe_details_reg.'</span>
					</section>
					<section class="even">
							<label>Special benefits provided, if any </label><span>'.$special_benifites.'</span>
					</section>
					<section class="odd">
							<label>Contribution made under the Employees Provident Fund and Miscellaneous Provision Act,1952 </label><span>'.$miscellaneous_provisions.'</span>
					</section>
					<section class="even">
							<label>Contribution made under the Employees State Insurance Act,1948</label><span>'.$state_insurance.'</span>
					</section>
					<div style="background:#3c4e5a; border:3px solid #e8e8e8;padding:5px;">
							<span style="font-weight:bold;font-size: 15px;color: #FFF;"><center>Uploaded Documents</center></span>							
					</div>
					<section class="odd">
							 <label>Work Order</label>';
								if(!empty($work_order_file_id)){
									$output.='<span><a title = "Click here to view Work Order" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$work_order_url.'" target="_blank">View work order file</a></span>';
			 					}else{
			   						$output.='<span>No Document uploaded</span>';
			  					}
				$output.='</section>
						  <section class="even">
							  <label>Form V</label>';
									if(!empty($frm_v_file_id)){
											$output.='<span><a title = "Click here to view Form V" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$frmv_url.'" target="_blank">View Form V</a></span>';
			  						}else{
			   								$output.='<span>No Document uploaded</span>';
			  						}
										
					$output.='</section>
							  <section class="odd">
										<label>Residential Certificate/Tread License</label>';
											if(!empty($residential_file_id)){
													$output.='<span><a title = "Click here to view Residential Certificate" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$residential_url.'" target="_blank">View Certificate</a></span>';
			  								}else{
			   										$output.='<span>No Document uploaded</span>';
			  								}	
					$output.='</section>
							  <section class="even">
										<label>Form IV/ VII/ Amendment Application Form (subsequently)</label>';
											if(!empty($form_iv_vii_id)){
												$output.='<span><a title = "Click here to view Form IV" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$form_iv_vii_url.'" target="_blank">View Form</a></span>';
			  						}else{
			   								$output.='<span>No Document uploaded</span>';
			  						}	
														
										
					$output.='</section>
							  <section class="odd">
										<label>License or Renewal or Ammendment Certificate (subsequently)</label>';
											if(!empty($certificate_id)){
												$output.='<span><a title = "Click here to view Certificate" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$certificate_url.'" target="_blank">View certificate</a></span>';
			  								}else{
			   									$output.='<span>No Document uploaded</span>';
			  								}	
										
										
					
					$output.='</section>
							 <section class="even">
									<label>Other Document</label>';
										if(!empty($other_file_id)){
												$output.='<span><a title = "Click here to view Other Document" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$other_url.'" target="_blank">View Other File</a></span>';
			  							}else{
			   								$output.='<span>No Document uploaded</span>';
			  							}
					                   
					$output.='	</section>
					
					
							 <section class="odd">
									<label>Amendment Application Form</label>';
										if(!empty($upload_singed_from_id)){
												$output.='<span><a title = "Click here to view amendment application form" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$documents_upload_singed_from_url.'" target="_blank">View amendment application form</a></span>';
			  							}else{
			   								$output.='<span>No Document uploaded</span>';
			  							}
					                   
					$output.='	</section>
					
					
					
								
								
			</div>
	 </div><font color="#FF0000">Note:-Your application will be forwarded to RLO  <strong><u>'.$subdivnam .' </strong></u> with respect to your given worksite address.</font>';
						   
	return $output;
}
function print_preview($form, &$form_state){
	
		$val			=	$form_state['values'];
		$serial_no		=	$val['serial_no'];
		$ammend_id		=	$val['ammend_id'];
		$x				= 	'Appliaction From';
		drupal_goto('amendment_license_renewal/create_formiv_vii/'.$serial_no.'/'.$ammend_id.'/'.$x) ;
}
	

	
	
	

	
	


