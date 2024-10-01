 <?php

drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
drupal_add_js(drupal_get_path('module', 'applicant_forms').'/js/custom.js');

function apply_amendment($form, &$form_state, $serialno='' ,$renewallicenseammendment_id= '',$applicationflag_hash= ''){
	
	global $user;
	$userid								= 	$user->uid;	
	
	
	
	$serial_no							=  encryption_decryption_fun('decrypt', $serialno);
	$renewal_license_ammendment_id		=  encryption_decryption_fun('decrypt', $renewallicenseammendment_id);
	
	
	$uri								= explode('#',$applicationflag_hash);
	$applicationflag					= $uri[0];
	$application_flag					= encryption_decryption_fun('decrypt', $uri[0]);
	$sms								= $uri[1];

	/*$get_application_type				= get_Application_type($userid);
	foreach($get_application_type as $row){
			$application_status  = $row->statusapplication;
			$flag 				 = $row->flagapplication;
	}*/
	
	$record = basic_information_for_ammendment($serial_no);
	
	if(!empty($record)){
			$license_no = !empty($record['backlog_license_no'])?$record['backlog_license_no'].'<br/>Ref No.'.$record['contractor_license_number']:$record['contractor_license_number'];
			$license_date = !empty($record['backlog_license_date'])?$record['backlog_license_date']:$record['license_date'];
			$validated_date = !empty($record['next_renweal_date'])?date('dS M Y', strtotime($record['next_renweal_date'])):'';
			$license_renewal_ammendment_certificate_id = !empty($record['certificateid'])?$record['certificateid']:'';
	}
	
	$contractor_regmaster_details 	= contractor_regmaster_details($serial_no);
	if(!empty($contractor_regmaster_details)){
			$application_id_data		 = 	$contractor_regmaster_details['id'];
			$pe_userid_data				 = 	$contractor_regmaster_details['user_id'];
	}
	if($application_flag == 'L'){
		$license_id						 =	$renewal_license_ammendment_id;
	}if($application_flag == 'A'){
		//$license_id						 =	return_ammendement_license_id($renewal_license_ammendment_id);
		// edited by jyoti 23/02/2023
		$license_id					     =	get_amd_license_id($renewal_license_ammendment_id);  // module
	}if($application_flag == 'R'){
		$license_id						 =	return_ammendement_license_id($renewal_license_ammendment_id);
	}
	
	//echo $serial_no.'--'.$userid.'--'.$renewal_license_ammendment_id.'--'.$application_flag.'---'.$license_id; exit;
	//$check_ammendment_table 	= check_ammendment_table($renewal_license_ammendment_id);	
	//$details_ammendment_table 	=  $check_ammendment_table->fetchAssoc();

	
	//$details_ammendment_table 	= check_ammendment_table_license_renewal_ammendment_id($renewal_license_ammendment_id);
	$details_ammendment_table 	= check_ammendment_table_new($renewal_license_ammendment_id,$serial_no); 
	$pe_userid					= !empty($details_ammendment_table['user_id'])?$details_ammendment_table['user_id']:$pe_userid_data;
	$application_id				= !empty($details_ammendment_table['application_id'])?$details_ammendment_table['application_id']:$application_id_data;
	$application_status			= $details_ammendment_table['ammendment_status'];
	
	
	$reference_no	= $details_ammendment_table['reference_no'];
	
	
	$form['amend_form']['serialno'] = array(
			'#type' 		=> 'hidden',
			'#default_value'=> $serialno,
			'#attributes'	=> array('readonly' => 'readonly'), 								
			); 
	$form['amend_form']['renewal_licenseammendment_id'] = array(
			'#type' 		=> 'hidden',
			'#default_value'=> $renewallicenseammendment_id,
			'#attributes'	=> array('readonly' => 'readonly'), 								
			); 
	$form['amend_form']['applicationflag'] = array(
			'#type' 		=> 'hidden',
			'#default_value'=> $applicationflag,
			'#attributes'	=> array('readonly' => 'readonly'), 								
			); 
	$form['amend_form']['application_id'] = array(
			'#type' 		=> 'hidden',
			'#default_value'=>  encryption_decryption_fun('encrypt', $application_id),
			'#attributes'	=> array('readonly' => 'readonly'), 								
			); 
	$form['amend_form']['pe_userid'] = array(
			'#type' 		=> 'hidden',
			'#default_value'=>  encryption_decryption_fun('encrypt', $pe_userid),
			'#attributes'	=> array('readonly' => 'readonly'), 								
			);
			
	/*$form['amend_form']['note'] = array(
			'#prefix' 		=> '<div class="content"><div class="sky-form" style="width:100%;"><header><center>
								<div style="font-size:18px;">
									<span>Form-V/Reference Number: <font color="#0F0106"><strong>00'.$serial_no.'</strong></font></span><br/>
									<span> License Number:  <font color="#0F0106"><strong>'.$license_no.'</strong></font></span>
									<br/>
									<span>Issue Date: <font color="#0F0106"><strong>'.date('dS M Y', strtotime($license_date)).'</strong></font>&nbsp;&nbsp;
										Valid Upto: <font color="#0F0106"><strong>'.$validated_date.'</strong></font></span>
								</div></center></header><fieldset>',
			'#suffix' 		=> '</label></section></fieldset><fieldset>',
			'#markup' 		=> 	details_preview($serial_no,$renewal_license_ammendment_id,$application_flag,$license_renewal_ammendment_certificate_id,$sms),		
			'#type' 		=> 'markup',														
		);*/
	$form['amend_form']['note'] = array(
				'#prefix' 									=> '<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading"><center>
								<span>Form-V/Reference Number: <font color="#d3e1ec"><strong>00'.$serial_no.'</strong></font></span><br/>
								<span> License Number:  <font color="#d3e1ec"><strong>'.$license_no.'</strong></font></span>
								<br/>
								<span>Issue Date: <font color="#d3e1ec"><strong>'.date('dS M Y', strtotime($license_date)).'</strong></font>&nbsp;&nbsp;
								Valid Upto: <font color="#d3e1ec"><strong>'.$validated_date.'</strong></font></span>
								</center></div>',
				'#suffix' 									=> '',
				'#markup' 									=> 	details_preview($serial_no,$renewal_license_ammendment_id,$application_flag,$license_renewal_ammendment_certificate_id,$sms),		
				'#type' 									=> 'markup',														
	);	
	
	$form['amend_form']['test'] = array(
				'#type' 									=> 'markup',
				//'#prefix' 									=> '<div class="col-md-12">',
				'#prefix' 									=> 	'<div class="panel-body"><div class="sky-form" style="width:100%;">',
																	  
			);
	
	$form['amend_form']['work_order'] = array(
	
				'#type' 									=> 'managed_file',
				'#title'									=> 'Work-order Document',
				'#upload_validators' 						=> array('file_validate_extensions' => array('pdf')),
				'#upload_location' 							=> 'public://upload/ammendment_workorder_doc',
				'#process' 									=> array('import_my_file_element_process'),
				'#attributes' 								=> array('id' => '','autocomplete' => 'off','class'=>array(''),'file_validate_size' => array(50*1024*1024)),
				'#prefix' 									=> '<div class="row"><section class="col col-6"><label class="input input-file">',
				'#suffix' 									=> '</label></section></div><br/>',
																	  
			);
			
		$form['amend_form']['other_document'] = array(
				'#type' 									=> 'managed_file',
				'#title'									=> 'Other Document',
				'#upload_validators' 						=> array('file_validate_extensions' => array('pdf')),
				'#upload_location' 							=> 'public://upload/ammendment_worksite_supportive_doc',
				'#process' 									=> array('import_my_file_element_process'),
				'#attributes' 								=> array('id' => '','autocomplete' => 'off','class'=>array(''),'file_validate_size' => array(50*1024*1024)),
				'#prefix' 									=> '<div class="row"><section class="col col-6"><label class="input input-file">',
				'#suffix' 									=> '</label></section></div><br/>',
																	  
			);
		
		$form['amend_form']['self_declaration'] = array(
		
				'#type' 			=> 'checkbox',
				
				'#title' 			=> t('<b>Declaration: I hereby declare that the details given above are correct to the best of my knowledge and belief.</b>'),
				'#required' 		=> TRUE,
				'#attributes'	    => array('id' => 'finalAgreeId','onchange' => 'return checkFinalAppCheckBox()'),
		 );
		  $form['amend_form']['test1'] = array(
				'#type' 									=> 'markup',
				
				'#suffix' 									=> '<br>',
				
																	  
			);
	
	if(trim($application_status) == NULL || trim($application_status) == 'B'){	
		$form['amend_form']['submit'] = array (
				  '#prefix' 								=> '',
				  '#id'	 									=> 'submitBtn1',
				  '#attributes' 							=> array('class' => array('button left-btn')),
				  '#type' 									=> 'submit',
				  '#value'									=> 'Save'
		  );
	 }else{
		 $form['amend_form']['submit'] = array (
				 
				  '#id'	 									=> 'submitBtn1',
				  '#attributes' 							=> array('class' => array('button left-btn')),
				  '#type' 									=> 'submit',
				  '#value'									=> 'Save'
		  );
	 }
	
	  if(trim($application_status) == NULL || trim($application_status) == 'I'){
			  $form['amend_form']['back_link']	 = array(
					
				'#markup' 		=> l(t('Back'),'amendment_license_renewal/check_for_amendment/'. $serialno, array('html' => TRUE,'attributes'=> array('class'=>array('button ')))),
				
					
																
				);	
	  }else{
		   $form['amend_form']['back_link']	 = array(
					
					'#markup' 								=> '',
				 
					
																			
				);	
	
	  }
	  
	  
	   $form['amend_form']['test3'] = array(
				'#type' 									=> 'markup',
				'#suffix' 									=> '</div></div></div></div>',
				
																	  
			);
	  
	return $form;
						

}
function details_preview($serial_no,$renewal_license_ammendment_id,$application_flag,$license_renewal_ammendment_certificate_id,$sms){
	global $base_root, $base_path, $user;

	$contractor_id 				= 	$user->uid;
	$edit_personal_details  	= '';
	$details 					= 	contractor_regmaster_details($serial_no);
	// if($contractor_id==61518){
	// 	echo $renewal_license_ammendment_id;
	// }die;
	
	if($application_flag == 'L'){
		$license_id						=	$renewal_license_ammendment_id;
	}if($application_flag == 'A'){
		//$license_id						=	return_ammendement_license_id($renewal_license_ammendment_id);
		$license_id                     =   get_amd_license_id($renewal_license_ammendment_id);  // module
	}if($application_flag == 'R'){
		$license_id						=	return_renewal_license($renewal_license_ammendment_id);
	}
	$ammendment 			 			= 	check_ammendment_fields($license_id,$serial_no)->fetchAssoc();
	
	if(!empty($ammendment)){
		$amendment_check_field 	= $ammendment['amendment_check_field'];
		if(!empty($amendment_check_field)){
			$amendment_check_fields = substr($amendment_check_field, 0, -1);
			$editable_field 	= explode(',', $amendment_check_fields);
	
			foreach($editable_field as $row){
				if($row =='name_of_contractor' || $row == 'contractor_dist'){ 
					
					$edit_personal_details 		= '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/edit.png"> ';
					//$edit_personal_details	='edit';
					$edit_personal_details 	    = l($edit_personal_details, 'amendment_license_renewal/ammendment-contractor-information/'.encryption_decryption_fun('encrypt', trim($serial_no)).'/'.encryption_decryption_fun('encrypt', trim($renewal_license_ammendment_id)).'/'.encryption_decryption_fun('encrypt', trim($application_flag)), array('html' => TRUE));
					
							
				}if($row !='name_of_contractor' && $row != 'contractor_dist' && $row!='contractor_max_no_of_labours_on_any_day' && $row!='father_name_of_contarctor' && $row!='category_of_contractor' && $row!='date_of_birth_age' && $row!='co_oparative' && $row!='worksite_address' && $row!='change_reason'){
					
					$edit_particular_details 	= '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/edit.png"> ';
					$edit_particular_details 	= l($edit_particular_details, 'amendment_license_renewal/ammendment-particular-info/'.encryption_decryption_fun('encrypt', trim($serial_no)).'/'.encryption_decryption_fun('encrypt', trim($renewal_license_ammendment_id)).'/'.encryption_decryption_fun('encrypt', trim($application_flag)), array('html' => TRUE));
					
				}if($row =='contractor_max_no_of_labours_on_any_day' || $row=='worksite_address'){
					
					$edit_contractor_labour 	= '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/edit.png"> ';
					$edit_contractor_labour 	= l($edit_contractor_labour, 'amendment_license_renewal/labour-worksite-info/'.encryption_decryption_fun('encrypt', trim($serial_no)).'/'.encryption_decryption_fun('encrypt', trim($renewal_license_ammendment_id)).'/'.encryption_decryption_fun('encrypt', trim($application_flag)), array('html' => TRUE));
				
				}
			}
        }
    }
	
	
	$details_principal_employer 	 	= '';
	$registration_date_timestamp	 	= strtotime($details['registration_date']);
	$formatted_reg_date 				= date('dS M Y', $registration_date_timestamp);
	
	$establish_details_reg				=	"";
	$establish_details_reg				=	$establish_details_reg."Registration Number: ".$details['registration_number']."<br/>"."Date of Certificates: ".$formatted_reg_date."\nUnder The CLRA acts";	
	
	$establishment_address				=	"";
	$establishment_address				=	miscellaneous_establishment_address_form($details['loc_e_areatype'],$serial_no,$details['id']);
	$establishment_address				=	$details['e_name'].'<br/>'.$details['loc_e_name'].'<br>'.$establishment_address;
	$nature_of_work_pe 					= 	'';
	$nature_of_work_pe 					= 	nature_of_work_pe($details['id']);
	//$principal_address					=	miscellaneous_principal_employer($loc_emp_areatype,$serial_no,$details['id']);//----miscellaneous module
	$fieldAr_pe							=  array('loc_emp_dist','loc_emp_subdivision','loc_emp_areatype','emp_name_areatype','loc_emp_vill_ward','l_emp_ps','loc_manager_pin_number','loc_manager_state');
    $principal_address	 				=	get_full_address('l_clra_registration_master','sub-table',$details['id'],$fieldAr_pe);
	$principal_details					=	$details['full_name_principal_emp']."<br>".$details['address_principal_emp']."<br>".$principal_address;
	$labour_from_date					=	$details['est_date_of_work_of_each_labour_from_date'];
	$labour_to_date						=	$details['est_date_of_work_of_each_labour_to_date'];
	$contractor_max_no_of_labours_pe	=	$details['contractor_max_no_of_labours_on_any_day'];
	$name_of_contractor_pervious	 	= 	$details['name_of_contractor'];
	$state_opts							=	$details['state_opts'];
	if($state_opts == 1){
		$pe_contractor_address			=   $details['address_of_contractor'].'<br/>'. miscellaneous_contractor_address_form($details['con_loc_e_areatype'],$serial_no);
	}if($state_opts == 2){
		$pe_contractor_address			=  $details['address_of_contractor'].'<br/>'. $details['state_others'];
	}
    $estimated_date						=	date_calculation($labour_from_date,$labour_to_date);
	
	
	$result_work 						= get_nature_of_work_contrcator($details['id'],$serial_no); //---miscellaneous module
	if($result_work->rowCount() > 0 ){
				foreach($result_work as $objwork){			
					$nature_work=$nature_work.$objwork->cont_work_name.', ';		  
				}
			}
			$sub_con=substr($nature_work, 0, -2);
			$nature_work=$sub_con;	
	
	
	
	if($application_flag == 'L' ){ 
		$result = particular_data($serial_no);
		
		if(!empty($result)){
			
			$particularid									= $result['id'];
			$father_contarctor_name_license_renewal 		= !empty($result['father_contarctor_name'])?$result['father_contarctor_name'] : 'Nil';
			//$contractor_address_license_renewal				= !empty($result['contractor_areatype'])?miscellaneous_contractor_update_address_form($result['contractor_areatype'],$serial_no,$contractor_id):''; 
			
			
			
			$category_of_contractor_license_renewal			= !empty($result['category_of_contractor'])?'Company' :'Individuals' ;
			$dob_contractor_license_renewal					= !empty($result['dob_contractor'])?$result['dob_contractor'] :'';
			$dob_age_contractor_license_renewal				= $result['age_contractor'];
			$contractor_manager_areatype					= !empty($result['contractor_manager_areatype'])?miscellaneous_contractor_manager_address_form($result['contractor_areatype'],$serial_no,$contractor_id):'';
			
			$work_site_address_license_renewal				= !empty($result['worksite_address_line']) ? miscellaneous_worksite_address($serial_no,$particularid):'';
			$name_of_agent_license_renewal					= $result['name_of_agent'];
			$address_of_manager_license_renewal			  	= $result['address_of_manager'].'<br/>'.$contractor_manager_areatype; 
			
			$contractor_max_labours_license_renewal			= $result['contractor_max_no_of_labours_on_any_day'];
			
			$category_designation_license_renewal	 		= $result['category_designation']; 
			$unskilled_rate_wages_license_renewal			= !empty($result['unskilled_rate_wages'])?'<del>&#2352;</del>'.$result['unskilled_rate_wages'].' ( Rupees '.convertNumberToWord($result['unskilled_rate_wages']).' only)':'Nil';
			$semiskilled_rate_wages_license_renewal			=  !empty($result['semiskilled_rate_wages'])?'<del>&#2352;</del>'.$result['semiskilled_rate_wages'].' ( Rupees '.convertNumberToWord($result['semiskilled_rate_wages']).' only)':'Nil';
			$skilled_rate_wages_license_renewal				=	!empty($result['skilled_rate_wages'])?'<del>&#2352;</del>'.$result['skilled_rate_wages'].' ( Rupees '.convertNumberToWord($result['semiskilled_rate_wages']).' only)':'Nil';
			$highlyskilled_rate_wages_license_renewal		=	!empty($result['highlyskilled_rate_wages'])?'<del>&#2352;</del>'.$result['highlyskilled_rate_wages'].' ( Rupees '.convertNumberToWord($result['highlyskilled_rate_wages']).' only)':'Nil';
			$hours_work_license_renewal						= 	$result['hours_work'];
			$spred_over_license_renewal						=	'0';
			$overtime_license_renewal						= 	$result['overtime'];
			$overtime_wages_license_renewal					= 	$result['overtime_wages'];
			$annual_leave_no_license_renewal				= 	!empty($result['annual_leave_no'])?$result['annual_leave_no']:'Nil';
			$casual_leave_no_license_renewal				= 	!empty($result['casual_leave_no'])?$result['casual_leave_no']:'Nil';
			$sick_leave_no_license_renewal					= 	!empty($result['sick_leave_no'])?$result['sick_leave_no']:'Nil';
			$other_leave_no_license_renewal					= 	!empty($result['other_leave_no'])?$result['other_leave_no']:'Nil';
			$maternity_leave_no_license_renewal				=	!empty($result['maternity_leave_no'])?$result['maternity_leave_no']:'Nil';
			$weekly_holiday_license_renewal					= 	$result['weekly_holiday'];
			$no_holiday_license_renewal						= 	$result['no_holiday'];
			//$work_order_file_id_license_renewal				=	$result['work_order_file_id'];
			$work_order_file_license_renewal_id				=	$result['work_order_file_id'];
			$frm_v_file_id									=	$result['frm_v_file_id'];
			$residential_file_id							=	$result['residential_file_id'];
			$other_file_id_license_renewal					=	$result['other_file_id'];
			$form_iv_vii_id_license_renewal					=	$result['signed_license_id'];
			
			$contractor_convicted_license_renewal			=	$result['details_contractor_convicted'];
			$contractor_revoking_license_renewal			=	$result['details_contractor_revoking'];
			$holiday_wages_license_renewal					=	0;
			$work_subdivision_license_renewal				=   $result['worksite_subdivision'];
			$address_line1_of_contractor_license_renewal	= !empty($result['address_of_contractor'])?$result['address_of_contractor'] : ''; 
		$fieldArr_license_renewal 						=  	array('contractor_dist','contractor_subdivision','contractor_areatype','contractor_name_areatype','contractor_vill_ward','contractor_ps','contractor_pin','contractor_state');
				$address_of_renewal_license				 	=	$address_line1_of_contractor_license_renewal.'<br>'.get_full_address('l_particular_of_contract_labour','sub-table',$particularid,$fieldArr_license_renewal);
				
			$address_line1_of_manager_license_renewal		= 	$result['address_of_manager'];
			$fieldAr_manager_license_renewal 				=  	array('contractor_manager_dist','contractor_manager_subdivision','contractor_manager_areatype','contractor_manager_name_areatype','contractor_managerr_vill_ward','contractor_manager_ps','manager_pin','manager_state');
			$address_of_manager_renewal_licenae				=	$address_line1_of_manager_license_renewal.'<br>'.get_full_address('l_particular_of_contract_labour','sub-table',$particularid,$fieldAr_manager_license_renewal);		
				
		}
	
	}
	
	
	 if($application_flag == 'R'){
		
			$particular_data 										= 	particular_data($serial_no);
	
			$renewal_data 											= 	renewal_data_last($renewal_license_ammendment_id);
			$work_subdivision_license_renewal						=   $particular_data['worksite_subdivision'];
			$particularid											= 	$particular_data['id'];
			$name_of_agent_license_renewal 							= 	$particular_data['name_of_agent'];
			$category_designation_license_renewal	 				= 	$particular_data['category_designation']; 
			$father_contarctor_name_license_renewal 				=   !empty($particular_data['father_contarctor_name'])?$particular_data['father_contarctor_name'] : 'Nil';
			$category_of_contractor_license_renewal					=   $particular_data['category_of_contractor'];
			$dob_contractor_license_renewal							= 	!empty($particular_data['dob_contractor'])?$particular_data['dob_contractor'] :'';
			$dob_age_contractor_license_renewal						= 	$particular_data['age_contractor'];
			$contractor_manager_areatype							=   !empty($particular_data['contractor_manager_areatype'])?miscellaneous_contractor_manager_address_form($particular_data['contractor_areatype'],$serial_no,$contractor_id):'';
			$work_site_address_license_renewal						=   !empty($particular_data['work_site_areatype'])?$particular_data['worksite_address_line'].'<br/>' .miscellaneous_worksite_address($serial_no,$particularid):'';
			//$contractor_address_license_renewal						=   !empty($particular_data['contractor_areatype'])?miscellaneous_contractor_update_address_form($particular_data['contractor_areatype'],$serial_no,$contractor_id):''; 
			$address_of_contractor_license_renewal	    			= 	$particular_data['address_of_contractor'];
			$fieldArr_license_renewal 						 		=  	array('contractor_dist','contractor_subdivision','contractor_areatype','contractor_name_areatype','contractor_vill_ward','contractor_ps','contractor_pin','contractor_state');
   			$address_of_renewal_license				 				=	$address_of_contractor_license_renewal.'<br>'.get_full_address('l_particular_of_contract_labour','sub-table',$particularid,$fieldArr_license_renewal);
			
			
			$address_of_manager_license_renewal			  			= 	$particular_data['address_of_manager'];
			$work_order_file_license_renewal_id						=	$particular_data['work_order_file_id'];
			$frm_v_file_id											=	$particular_data['frm_v_file_id'];
			$residential_file_id									=	$particular_data['residential_file_id'];
			$other_file_id_license_renewal							=	$particular_data['other_file_id'];
			$contractor_convicted_license_renewal					=	$particular_data['details_contractor_convicted'];	
			
			$fieldAr_manager_license_renewal 						=  	array('contractor_manager_dist','contractor_manager_subdivision','contractor_manager_areatype','contractor_manager_name_areatype','contractor_managerr_vill_ward','contractor_manager_ps','manager_pin','manager_state');
   		$address_of_manager_renewal_licenae				 			=	$address_of_manager_license_renewal.'<br>'.get_full_address('l_particular_of_contract_labour','sub-table',$particularid,$fieldAr_manager_license_renewal);	
			
			//$father_contarctor_name_license_renewal 				=  !empty($particular_data['father_contarctor_name'])?$particular_data['father_contarctor_name'] : 'Nil';
			
			
			$contractor_max_labours_license_renewal					=   $renewal_data['max_of_contract_labour'];
			
			$unskilled_rate_wages_license_renewal					= 	!empty($renewal_data['unskilled_rate_wages'])?'<del>&#2352;</del>'.$renewal_data['unskilled_rate_wages'].' ( Rupees '.convertNumberToWord($renewal_data['unskilled_rate_wages']).' only)':'Nil';
			$semiskilled_rate_wages_license_renewal					= 	!empty($renewal_data['semiskilled_rate_wages'])?'<del>&#2352;</del>'.$renewal_data['semiskilled_rate_wages'].' ( Rupees '.convertNumberToWord($renewal_data['semiskilled_rate_wages']).' only)':'Nil';
			$skilled_rate_wages_license_renewal						= 	!empty($renewal_data['skilled_rate_wages'])?'<del>&#2352;</del>'.$renewal_data['skilled_rate_wages'].' ( Rupees '.convertNumberToWord($renewal_data['skilled_rate_wages']).' only)':'Nil';
			$highlyskilled_rate_wages_license_renewal				= 	!empty($renewal_data['highlyskilled_rate_wages'])?'<del>&#2352;</del>'.$renewal_data['highlyskilled_rate_wages'].' ( Rupees '.convertNumberToWord($renewal_data['highlyskilled_rate_wages']).' only)':'Nil';
			
			
		
		   $annual_leave_no_license_renewal							= 	!empty($renewal_data['annual_leave_no'])?$renewal_data['annual_leave_no']:'Nil';
		   $casual_leave_no_license_renewal							= 	!empty($renewal_data['casual_leave_no'])?$renewal_data['casual_leave_no']:'Nil';
		   $sick_leave_no_license_renewal							= 	!empty($renewal_data['sick_leave_no'])?$renewal_data['sick_leave_no']:'Nil';
		   $other_leave_no_license_renewal							= 	!empty($renewal_data['other_leave_no'])?$renewal_data['other_leave_no']:'Nil';
		   $maternity_leave_no_license_renewal						=	!empty($renewal_data['maternity_leave_no'])?$renewal_data['maternity_leave_no']:'Nil';
		   $earned_leave_no_license_renewal							=	!empty($renewal_data['earned_leave_no'])?$renewal_data['earned_leave_no']:'Nil';
			
			
			
		
			$hours_work_license_renewal								=	$renewal_data['hours_work'];
			$spred_over_license_renewal								=	$renewal_data['spred_over'];
			$holiday_wages_license_renewal							=	$renewal_data['holiday_wages'];
			$weekly_holiday_license_renewal							= 	$renewal_data['weekly_holiday'];
		  	$no_holiday_license_renewal								= 	$renewal_data['no_holiday'];
			$overtime_license_renewal								= 	$renewal_data['overtime'];
			$overtime_wages_license_renewal							= 	$renewal_data['overtime_wages'];
			
			
			$special_benifites_license_renewal						=	!empty($renewal_data['special_benifites'])?$renewal_data['special_benifites']:'Nil';
			$state_insurance_license_renewal						= 	!empty($renewal_data['state_insurance'])?($renewal_data['state_insurance']):'Nil';
			$miscellaneous_provisions_license_renewal				=	!empty($renewal_data['miscellaneous_provisions'])?$renewal_data['miscellaneous_provisions']:'Nil';
			$contractor_revoking_license_renewal					=	$renewal_data['details_contractor_revoking'];
			
			$form_iv_vii_id_license_renewal							=	$renewal_data['upload_signed_form'];
			$license_renewal_ammendment_certificate_id				=	$renewal_data['renewal_certificate_id'];
			$due_security_fees_amount_license_renewal				=	$renewal_data['due_security_fees'];	
	 }

	
	  if($application_flag == 'A'){
		  $result								= 	particular_file($serial_no,$contractor_id);
		  $frm_v_file_id						=	$result['frm_v_file_id'];
		  $residential_file_id					=	$result['residential_file_id'];
		  $work_order_file_license_renewal_id	=	$result['work_order_file_id'];
		  $amendment_log_details 				= 	contract_labour_amendment_log($renewal_license_ammendment_id);
		  $form_iv_vii_id_license_renewal		=	$amendment_log_details['upload_singed_from'];
		  $license_renewal_ammendment_certificate_id	=	$amendment_log_details['ammendment_certificate_id'];
		  $other_file_id_license_renewal		= $amendment_log_details['other_file_id'];	
		  $details_ammendment_table				=	db_query('select  * from l_amendment_of_contract_labour_license where  id=:license_renewal_ammendmentid ', array(':license_renewal_ammendmentid' =>$renewal_license_ammendment_id))->fetchAssoc();
		 // $details_ammendment_table 			= check_ammendment_table_license_renewal_ammendment_id($renewal_license_ammendment_id);
		  $amand_id								= $details_ammendment_table['id'];
	
	  }
	 
	
	$contractor_convicted				 		 = !empty($details_ammendment_table['details_contractor_convicted'])?$details_ammendment_table['details_contractor_convicted']:(!empty($contractor_convicted_license_renewal)?$contractor_convicted_license_renewal:'Nil');
	
	$contractor_revoking				 		 = !empty($details_ammendment_table['details_contractor_revoking'])?$details_ammendment_table['details_contractor_revoking']:$contractor_revoking_license_renewal;

	 
	
	$address_line_of_manager					 = $details_ammendment_table['address_of_manager'];
	 
	$fieldArr_manager_amm						 =  	array('contractor_manager_dist','contractor_manager_subdivision','contractor_manager_areatype','contractor_manager_name_areatype','contractor_managerr_vill_ward','contractor_manager_ps','contractor_manager_pin','manager_state');
  	$amand_manager_address			 		     =	$address_line_of_manager.'<br>'.get_full_address('l_amendment_of_contract_labour_license','sub-table',$amand_id,$fieldArr_manager_amm);
	
	$dob_contractor								 = !empty($details_ammendment_table['dob_contractor'])?$details_ammendment_table['dob_contractor']:$dob_contractor_license_renewal;
	$age_contractor								 = !empty($details_ammendment_table['age_contractor'])?$details_ammendment_table['age_contractor']:$dob_age_contractor_license_renewal;
	$pe_userid									 = !empty($details_ammendment_table['user_id'])?$details_ammendment_table['user_id']:$pe_userid;
	$application_id								 = !empty($details_ammendment_table['application_id'])?$details_ammendment_table['application_id']:$application_id;
	$reference_no								 = !empty($details_ammendment_table['reference_no'])?$details_ammendment_table['reference_no']:'';
	$address_of_contractor						 = !empty($details_ammendment_table['address_of_contractor'])?$details_ammendment_table['address_of_contractor']:$address_of_contractor_license_renewal;
	$fieldArr_contractor 						 =  array('contractor_dist','contractor_subdivision','contractor_areatype','contractor_name_areatype','contractor_vill_ward','contractor_ps','contractor_pin','contractor_state');
    $address_of_amand				 			 =	$address_of_contractor.'<br>'.get_full_address('l_amendment_of_contract_labour_license','sub-table',$amand_id,$fieldArr_contractor);
	
 
	//$age_contractor								 = !empty($details_ammendment_table['age_contractor'])?$details_ammendment_table['age_contractor']:$age_contractor_license_renewals; 
	
	
	
	$contractor_revoking_date = !empty($contractor_revoking)?date('dS M, Y',strtotime($contractor_revoking)):'Not Avaliable';
	if(!empty($dob_contractor)){
		$dob_contractor_new = date('dS M, Y',strtotime($dob_contractor));
	}else{
		$dob_contractor_new = 'Nil';
		
	}if(!empty($age_contractor)){
		$age_contractor_new = $age_contractor.'Years';
	}else{
		$age_contractor_new = 'Nil';
	}
	
	$unskilled_rate_wages 						= !empty($details_ammendment_table['unskilled_rate_wages'])?$details_ammendment_table['unskilled_rate_wages']:(!empty($unskilled_rate_wages_license_renewal)?$unskilled_rate_wages_license_renewal:'Nil');
	$semiskilled_rate_wages 					= !empty($details_ammendment_table['semiskilled_rate_wages'])?$details_ammendment_table['semiskilled_rate_wages']:(!empty($semiskilled_rate_wages_license_renewal)?$semiskilled_rate_wages_license_renewal:'Nil');
	$skilled_rate_wages 					 	= !empty($details_ammendment_table['skilled_rate_wages'])?$details_ammendment_table['skilled_rate_wages']:(!empty($skilled_rate_wages_license_renewal)?$skilled_rate_wages_license_renewal:'Nil');
	$highlyskilled_rate_wages 					= !empty($details_ammendment_table['highlyskilled_rate_wages'])?$details_ammendment_table['highlyskilled_rate_wages']:(!empty($highlyskilled_rate_wages_license_renewal)?$highlyskilled_rate_wages_license_renewal:'Nil');
	$hours_work									= !empty($details_ammendment_table['hours_work'])?$details_ammendment_table['hours_work']:$hours_work_license_renewal;
    $spred_over 								= !empty($details_ammendment_table['spred_over'])?$details_ammendment_table['spred_over']:$spred_over_license_renewal;
	$overtime_wages								= !empty($details_ammendment_table['overtime_wages'])?$details_ammendment_table['overtime_wages']:$overtime_wages_license_renewal;
	$overtime								    = !empty($details_ammendment_table['overtime'])?$details_ammendment_table['overtime']:$overtime_license_renewal;	
	$annual_leave_no 							= !empty($details_ammendment_table['annual_leave_no'])?$details_ammendment_table['annual_leave_no']:(!empty($annual_leave_no_license_renewal)?$annual_leave_no_license_renewal:'Nil');
	$casual_leave_no 							= !empty($details_ammendment_table['casual_leave_no'])?$details_ammendment_table['casual_leave_no']:(!empty($casual_leave_no_license_renewal)?$casual_leave_no_license_renewal:'Nil');
	$sick_leave_no 								= !empty($details_ammendment_table['sick_leave_no'])?$details_ammendment_table['sick_leave_no']:(!empty($sick_leave_no_license_renewal)?$sick_leave_no_license_renewal:'Nil');
	$maternity_leave_no 						= !empty($details_ammendment_table['maternity_leave_no'])?$details_ammendment_table['maternity_leave_no']:(!empty($maternity_leave_no_license_renewal)?$maternity_leave_no_license_renewal:'Nil');
	$earned_leave_no 							= !empty($details_ammendment_table['earned_leave_no'])?$details_ammendment_table['earned_leave_no']:(!empty($earned_leave_no_license_renewal)?$earned_leave_no_license_renewal:'Nil');
	$other_leave_no 							= !empty($details_ammendment_table['other_leave_no'])?$details_ammendment_table['other_leave_no']:(!empty($other_leave_no_license_renewal)?$other_leave_no_license_renewal:'Nil');
	
	

	$query_all 	= db_select('l_amendment_of_contract_labour_license', 'amendment');
	$query_all->fields('amendment',array())->condition('license_id',$license_id,'=');
	$result_all = $query_all->execute();
	if($result_all->rowCount() > 0){
		$result_all_Details = $result_all->fetchAssoc();
		$max_of_contract_labour	= $result_all_Details['max_of_contract_labour'];

		$max_of_contract_labour 					= !empty($result_all_Details['max_of_contract_labour'])?$result_all_Details['max_of_contract_labour']:$contractor_max_labours_license_renewal;	
		$work_site_address 							= !empty($result_all_Details['worksite_address_line'])?ammendment_address($license_id,'ammendment_worksite_address',$reference_no):$work_site_address_license_renewal;
		$name_of_contractor 						 = !empty($result_all_Details['name_of_contractor'])?$result_all_Details['name_of_contractor']:$name_of_contractor_pervious;
		$address_line1_of_contractor				 = !empty($result_all_Details['address_of_contractor'])? $address_of_amand :$address_of_renewal_license; 
		$father_contarctor_name 					 = !empty($result_all_Details['father_contarctor_name'])?$result_all_Details['father_contarctor_name']:(!empty($father_contarctor_name_license_renewal)?$father_contarctor_name_license_renewal:'Nil');

		$category_of_contractor_ammend				 = $result_all_Details['category_of_contractor'];
		if($category_of_contractor_ammend != 0 && $category_of_contractor_ammend != 1){
			$category_of_contractor = $category_of_contractor_license_renewal;
		}if($category_of_contractor_ammend == 0 || $category_of_contractor_ammend == 1){
			$category_of_contractor = $category_of_contractor_ammend;
		}
		if($category_of_contractor=='1' && !empty($category_of_contractor)){
			$type = 'Company';
			$title = 'Date of commencement of business';
			$age_contractor='';
		}else{
				$type = 'Individuals';
				$title = 'Date of Birth and age (incase of individuals)';
				$age_contractor= 'Age: '.$age_contractor_new;
		}

		$name_of_agent = !empty($result_all_Details['name_of_agent'])?$result_all_Details['name_of_agent'] :$name_of_agent_license_renewal;
		$address_line1_of_manager					 = !empty($result_all_Details['address_of_manager'])? $amand_manager_address:$address_of_manager_renewal_licenae; 
		$category_designation 						= !empty($result_all_Details['category_designation'])?$result_all_Details['category_designation']:(!empty($category_designation_license_renewal)?$category_designation_license_renewal:'Nil');


	}else{
		$max_of_contract_labour 					= !empty($details_ammendment_table['max_of_contract_labour'])?$details_ammendment_table['max_of_contract_labour']:$contractor_max_labours_license_renewal;	
		$work_site_address 							= !empty($details_ammendment_table['worksite_address_line'])?ammendment_address($license_id,'ammendment_worksite_address',$reference_no):$work_site_address_license_renewal;
		$name_of_contractor 						 = !empty($details_ammendment_table['name_of_contractor'])?$details_ammendment_table['name_of_contractor']:$name_of_contractor_pervious;
		$address_line1_of_contractor				 = !empty($details_ammendment_table['address_of_contractor'])? $address_of_amand :$address_of_renewal_license; 
		$father_contarctor_name 					 = !empty($details_ammendment_table['father_contarctor_name'])?$details_ammendment_table['father_contarctor_name']:(!empty($father_contarctor_name_license_renewal)?$father_contarctor_name_license_renewal:'Nil');

		$category_of_contractor_ammend				 = $details_ammendment_table['category_of_contractor'];
		if($category_of_contractor_ammend != 0 && $category_of_contractor_ammend != 1){
			$category_of_contractor = $category_of_contractor_license_renewal;
		}if($category_of_contractor_ammend == 0 || $category_of_contractor_ammend == 1){
			$category_of_contractor = $category_of_contractor_ammend;
		}
		if($category_of_contractor=='1' && !empty($category_of_contractor)){
			$type = 'Company';
			$title = 'Date of commencement of business';
			$age_contractor='';
		}else{
				$type = 'Individuals';
				$title = 'Date of Birth and age (incase of individuals)';
				$age_contractor= 'Age: '.$age_contractor_new;
		}

		$name_of_agent = !empty($details_ammendment_table['name_of_agent'])?$details_ammendment_table['name_of_agent'] :$name_of_agent_license_renewal;
		$address_line1_of_manager					 = !empty($details_ammendment_table['address_of_manager'])? $amand_manager_address:$address_of_manager_renewal_licenae; 
		$category_designation 						= !empty($details_ammendment_table['category_designation'])?$details_ammendment_table['category_designation']:(!empty($category_designation_license_renewal)?$category_designation_license_renewal:'Nil');

	}
	
	
	$holiday_wages								=!empty($details_ammendment_table['holiday_wages'])?$details_ammendment_table['holiday_wages']:(!empty($holiday_wages_license_renewal)?$holiday_wages_license_renewal:'Nil');
	$no_holiday									=!empty($details_ammendment_table['no_holiday'])?$details_ammendment_table['no_holiday']:(!empty($no_holiday_license_renewal)?$no_holiday_license_renewal:'Nil');
	
	$supportive_documents_worksite_id 			= $details_ammendment_table['supportive_documents_worksite'];
	
	$payment_recipt_file_id 					= $details_ammendment_table['payment_recipt_file'];
	
	$work_order_file_id							= !empty($details_ammendment_table['work_order_file_id'])?$details_ammendment_table['work_order_file_id']:$work_order_file_license_renewal_id;
	
	
	$other_file_id 								= !empty($details_ammendment_table['other_file_id'])?$details_ammendment_table['other_file_id']:$other_file_id_license_renewal;
	$form_iv_vii_id								= !empty($details_ammendment_table['upload_singed_from'])?($details_ammendment_table['upload_singed_from']):$form_iv_vii_id_license_renewal;
	$certificate_id								= !empty($details_ammendment_table['ammendment_certificate_id'])?($details_ammendment_table['ammendment_certificate_id']):$license_renewal_ammendment_certificate_id;
	

	$miscellaneous_provisions					= !empty($details_ammendment_table['miscellaneous_provisions'])?$details_ammendment_table['miscellaneous_provisions']:(!empty($miscellaneous_provisions_license_renewal)?$miscellaneous_provisions_license_renewal:'Nil');
	$state_insurance							= !empty($details_ammendment_table['state_insurance'])?($details_ammendment_table['state_insurance']):(!empty($state_insurance_license_renewal)?$state_insurance_license_renewal:'Nil');
	$special_benifites							= !empty($details_ammendment_table['special_benifites'])?($details_ammendment_table['special_benifites']):(!empty($special_benifites_license_renewal)?$special_benifites_license_renewal:'Nil');
	$work_subdivision_license_renewal			= !empty($details_ammendment_table['worksite_subdivision'])?($details_ammendment_table['worksite_subdivision']):$work_subdivision_license_renewal;
	$subdivnam 									= get_subdivname($work_subdivision_license_renewal);
	$reference_no_data							= $details_ammendment_table['reference_no'];
	$reason_changing_worksite					= !empty($details_ammendment_table['reason_changing_worksite'])?$details_ammendment_table['reason_changing_worksite']:'Not Avalible';

		
	
	
  
  
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
				$certificate_url			= '';
				$url						= explode('//',$upload_certificate_file['uri']);
				$certificate_url			= $url[1];
				$certificate_file			= $upload_certificate_file['filename'];
		}
	}
	if($supportive_documents_worksite_id!=''){
		$supportive_documents_worksite_file = fetch_uplaoded_file_details($supportive_documents_worksite_id);
		if(!empty($supportive_documents_worksite_file)){ 
				$url									= explode('//',$supportive_documents_worksite_file['uri']);
				$documents_worksite_file_url			= $url[1];
				$documents_worksite_file_file			= $supportive_documents_worksite_file['filename'];
		}
	}
	
	


	if(!empty($no_holiday)){
		$rest_holiday = substr($no_holiday, 0, -1);
		$no_holiday_list= explode(',', $rest_holiday);
		
		$name_holiday="";
		$total_holiday=0;
		foreach($no_holiday_list as $row){
			if(!empty($row)){
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
		}
		$name_holidays = substr($name_holiday, 0, -1);
		if(empty($name_holidays))
			$name_holidays='NIL';
	}	
	if(!empty($sms)){
	if($sms == "1"){
			$message1= drupal_set_message_custom(t('official Information are successfully updated'));
	}if($sms == "2"){
			$message2= drupal_set_message_custom(t('Contractor Labour and Worksite Details'));
	}if($sms == "3"){
			$message3= drupal_set_message_custom(t('Particular of Contract Labour '));
	}
	}
	
	if(!empty($reference_no_data)){
		$last_remark_activity = last_remark_activity($reference_no_data);
	
		 $remark_field_title = $last_remark_activity['remark_field_title'];
		 $remark_type 		 = $last_remark_activity['remark_type'];
	}
	 //print_r($remark_field_title);die();
	
	if((empty($remark_field_title) && $remark_type =='B')){
			   $readonly_agent	= $readonly_address_manager = $readonly_contract_manger_dist=
			   $readonly_unskilled_rate_wages=$readonly_semiskilled_rate_wages=$readonly_skilled_rate_wages=
			   $readonly_highlyskilled_rate_wages=$readonly_annual_leave_no=$readonly_casual_leave_no=$readonly_sick_leave_no=
			   $readonly_maternity_leave_no=$readonly_eraned_leave_no=$readonly_other_leave_no=
			   $readonly_hours_work=$readonly_weekly_holiday=$readonly_holiday_wages=$readonly_offence=
			   $readonly_contractor_revoking=$readonly_special_benifites=$readonly_state_insurance=
			   $readonly_miscellaneous=$readonly_hours_work=$readonly_spared_over=$readonly_overtime=$readonly_overtime_wages=
			   $readonly_category=$readonly_father_contarctor_name=
			   $readonly_address_line1=$readonly_contract_dist=$readonly_contarctor_age=
			   $readonly_contarctor_dob=$readonly_category_of_contractor=$readonly_other_document=$readonly_supportive_documents=
			   $raedonly_payment_recipt= $readonly_max_no_of_labours =$readonly_workorder=
			   '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/Correct-It.png">';
			   $previous_docu=$readonly_contract_name =  '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
			  $readonly_worksite	=	'<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
				
	}if(!empty($remark_field_title) && $remark_type =='I'){
			   $readonly_agent	= $readonly_address_manager = $readonly_contract_manger_dist=
			   $readonly_unskilled_rate_wages=$readonly_semiskilled_rate_wages=$readonly_skilled_rate_wages=
			   $readonly_highlyskilled_rate_wages=$readonly_annual_leave_no=$readonly_casual_leave_no=$readonly_sick_leave_no=
			   $readonly_maternity_leave_no=$readonly_eraned_leave_no=$readonly_other_leave_no=
			   $readonly_hours_work=$readonly_weekly_holiday=$readonly_holiday_wages=$readonly_offence=
			   $readonly_contractor_revoking=$readonly_special_benifites=$readonly_state_insurance=
			   $readonly_miscellaneous=$readonly_hours_work=$readonly_spared_over=$readonly_overtime=$readonly_overtime_wages=
			   $readonly_category=$readonly_contract_name=$readonly_father_contarctor_name=
			   $readonly_address_line1=$readonly_contract_dist=$readonly_contarctor_age=
			   $readonly_contarctor_dob=$readonly_category_of_contractor=$readonly_other_document=$readonly_supportive_documents=
			   $raedonly_payment_recipt=$readonly_max_no_of_labours=$readonly_worksite=$readonly_workorder='' ;
			   
		   }
	if((!empty($remark_field_title) && $remark_type =='B')){
			   $readonly_agent	= $readonly_address_manager = $readonly_contract_manger_dist=
			   $readonly_unskilled_rate_wages=$readonly_semiskilled_rate_wages=$readonly_skilled_rate_wages=
			   $readonly_highlyskilled_rate_wages=$readonly_annual_leave_no=$readonly_casual_leave_no=$readonly_sick_leave_no=
			   $readonly_maternity_leave_no=$readonly_eraned_leave_no=$readonly_other_leave_no=
			   $readonly_hours_work=$readonly_weekly_holiday=$readonly_holiday_wages=$readonly_offence=
			   $readonly_contractor_revoking=$readonly_special_benifites=$readonly_state_insurance=
			   $readonly_miscellaneous=$readonly_hours_work=$readonly_spared_over=$readonly_overtime=$readonly_overtime_wages=
			   $readonly_category=$readonly_father_contarctor_name=
			   $readonly_address_line1=$readonly_contract_dist=$readonly_contarctor_age=
			   $readonly_contarctor_dob=$readonly_category_of_contractor=$readonly_other_document=$readonly_supportive_documents=
			   $raedonly_payment_recipt=$readonly_max_no_of_labours=$readonly_workorder=
			   '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/Correct-It.png">';
			   $previous_docu=$readonly_contract_name =  '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
			   $readonly_worksite	=	'<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
			  
				
	}
	// print_r($remark_field_title);	 
	if((!empty($remark_field_title) && $remark_type =='B')){
		 $previous_docu =  '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
		 $readonly_worksite	=	'<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
		   $remark_field_title = explode(',', $remark_field_title);	
		  
			 foreach($remark_field_title as $row){
				switch($row){
					case  "address_of_manager":
								$readonly_agent	= $readonly_contract_manger_dist=$readonly_address_manager ='<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
						 	 break;
					case  "category":
							$readonly_category=$readonly_contarctor_dob=$readonly_contarctor_age = '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							break;
					case  "unskilled_rate_wages":
							 $readonly_unskilled_rate_wages = '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							break;
					case   "semiskilled_rate_wages":
							 $readonly_semiskilled_rate_wages  ='<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							
						 	break;
					case   "skilled_rate_wages":
							 $readonly_skilled_rate_wages  = '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							
						 	break;
					case   "highlyskilled_rate_wages":
							 $readonly_highlyskilled_rate_wages  = '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							
						 	break;
					case   "hour_details":
							 $readonly_hours_work  = '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							
						 	break;
					case   "spred_over":
							 $readonly_hours_work  ='<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							
						 	break;
					case   "overtime":
							 $readonly_hours_work  = '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							
						 	break;
					case   "overtime_wages":
							 $readonly_overtime_wages  = '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							
						 	break;
					case   "annual_leave":
							 $readonly_annual_leave_no  ='<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							
						 	break;
						case   "casual_leave":
							 $readonly_casual_leave_no  ='<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							
						 	break;
						case   "eraned_leave":
							 $readonly_eraned_leave_no  = '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							
						 	break;
						case   "sick_leave":
							 $readonly_sick_leave_no  = '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							
						 	break;
						case   "other_leave":
							 $readonly_other_leave_no  = '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							
						 	break;
						case   "maternity_leave":
							 $readonly_maternity_leave_no  = '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							
						 	break;
					case   "weekly_holiday":
							 $readonly_weekly_holiday  = '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							
						 	break;
					case   "special_benifites":
							 $readonly_special_benifites  = '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							
						 	break;
					case   "state_insurance":
							 $readonly_state_insurance  = '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							
						 	break;
					case   "miscellaneous":
							 $readonly_miscellaneous  = '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							
						 	break;
					case   "offence":
							  $readonly_offence 	= '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							 
							
						 	break;
					case   "revoking":
							 $readonly_contractor_revoking  = '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							 
							
						 	break;
							
					case  "name_of_contractor":
							 $readonly_contract_name	='<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							 break;
					case  "contractor_dist":
							$readonly_contract_dist 	= $readonly_address_line1 = '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
						 break;
						case  "father_name_of_contarctor":
							 $readonly_father_contarctor_name = '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							break;
						case   "category_of_contractor":
							 $readonly_category_of_contractor  =  '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							$readonly_contarctor_dob =$readonly_contarctor_dob =  '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							break;
						case   "other_doc_id":
							 $readonly_other_document  =  '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							 break;
						case   "supportive_documents":
							 $readonly_supportive_documents  =  '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							 break;
						case   "payment_recipt":
							 $raedonly_payment_recipt  =  '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							 break;
						case   "contractor_max_no_of_labours_on_any_day":
							 $readonly_max_no_of_labours  =  '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							 break;
						case   "work_order_id":
							 $readonly_workorder  =  '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/verified.png">';
							 break;
						
					}
				}
		   }
		//echo $readonly_offence;die();
	//$pe_details_reg = pe_regmaster_details($serial_no,$contractor_id);
	//print_r($pe_details_reg);die();
	if(!empty($due_security_fees_amount_license_renewal)){
		$due_statement = '<font color="#FF0000"><strong> Your security fees '.$due_security_fees_amount_license_renewal.' is due till now.</strong></font>';
	}else{
		$due_statement = "";
	}
	$pe_details_reg 	= 'View Details';
	$pe_details_reg 	= l($pe_details_reg, 'amendment_license_renewal/view_pe_details/'.encryption_decryption_fun('encrypt', trim($serial_no)).'/'.encryption_decryption_fun('encrypt', trim($contractor_id)), array('html' => TRUE));


	$output			= '';
		

	$output	 = '<div class="con-dtls-view-main">
					<h3>1.Information Given by Principal Employeer.</h3>
					<div class="con-dtls-view-contains">
						<section class="odd">
							<label>Name & address of the establishment of Principal Employer</label><span>'.$establishment_address.'</span>
						</section>
									
						<section class="even">
							<label>Type of Business, trade, industry, manufacture or occupation which is carried on in the establishment(P.E.)</label><span>'.trim($nature_of_work_pe,',').' </span>
						</section>
									
						<section class="odd">
							 <label>Number and date of Certificate</label><span>'.$establish_details_reg.'</span>
						</section>
									
						<section class="even">
							<label>Name & Address of the Principal Employer</label><span>'.$principal_details.'</span>
						</section>
				        <section class="odd">
							<label>Name & Address of Contractor</label><span>'.$name_of_contractor_pervious.'<br>'.$pe_contractor_address.'</span>
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
							<label>Name</label><span>'.$name_of_contractor.' <span style="float:right;"> '.$readonly_contract_name.'</span>	</span>			
					</section>
									
					<section class="even">
							<label>Address of Contractor</label><span>'.$address_line1_of_contractor.' <br />'. $contractor_address.'<span style="float:right;">'.$readonly_contract_dist.'</span></span>
					</section>
									
					<section class="odd">
							 <label>Father name of contarctor</label><span>'.$father_contarctor_name.'<span style="float:right;">'.$readonly_father_contarctor_name.'</span></span>
					</section>
									
					<section class="even">
							 <label>Category of Contractor</label><span>'.$type.'<span style="float:right;">'.$readonly_category_of_contractor.'</span></span>
					</section>
					<section class="odd">
							<label>'.$title.'</label><span>'.$dob_contractor_new.'<br />'.$age_contractor.'<span style="float:right;">'.$readonly_contarctor_age.'</span></span>
					</section>
									
									
				</div>
			</div>
			
			<div class="con-dtls-view-main" id="Labour and Worksite"><span>'.$message2.'</span>';
			
			
			 $output	 .= '<h3>3.Contractor Labour and Worksite Details<span class="edit">'.$edit_contractor_labour.'</span></h3>';
			
			 
				$output	 .='<div class="con-dtls-view-contains">
					<section class="odd">
							<label>Work site Address</label><span>'.$work_site_address.'<span style="float:right;">'.$readonly_worksite.'</span></span>
					</section>
					<!--<section class="even">
							<label>Reason for changeing of worksite details</label><span><'.$reason_changing_worksite1.'</span>
					</section>-->
									
					<section class="even">
							<label>Maximum number of Contract Labour proposed to he employed in the establishment on any date</label><span>'.$max_of_contract_labour.' ('.convertNumberToWord($max_of_contract_labour).')<br>'.$due_statement.'<span style="float:right;">'.$readonly_max_no_of_labours.'</span></span>
					</section>
									
					
					<section class="odd">
						<font color="#FF0000"><strong>Note:-Your application will be forwarded to RLO <b>'.$subdivnam .' </b>with respect to your given worksite address</font></strong>
					</section>				
														
				</div>
			</div>
			
			
						   
			<div class="con-dtls-view-main" id ="particular"><span>'.$message3.'</span>
				<h3>4. Particular of Contract Labour <span class="edit">'.$edit_particular_details.'</span></h3>
								
				<div class="con-dtls-view-contains">
					<section class="odd">
							<label>Name & address of the agent or Manager of Contractor at the work site</label><span>'.$name_of_agent.'<br /> '.$address_line1_of_manager.'  <span style="float:right;">'.$readonly_agent.'</span></span>
					</section>
					<section class="even">
						     <label>Category/designation/ nomenclature of the contractor labour, namely, fitter,welder,carpenter, mazdor etc</label><span>'.$category_designation.'<span style="float:right;">'.$readonly_category.'</span></span>
					</section>
										
					<div style="background:#3c4e5a; border:3px solid #e8e8e8;padding:5px;">
										
						<span style="font-weight:bold;font-size: 15px;color: #FFF;"><center>Rate of Wages,DA and other cash benefits paid/ to be paid to each category of contract labour i.e</center><center>  (a):Unskilled (b)Semi Skilled (c)Skilled (d)Highly Skilled etc.)</center></span>							
										
					</div>
					<section class="odd">
							<label>(a)Rate of Wages,DA and other cash benefits paid/ to be paid to Unskilled of contract labour</label><span>'.$unskilled_rate_wages.'<span style="float:right;">'.$readonly_unskilled_rate_wages.'</span></span>
					</section>
					<section class="even">
							<label>(b)Rate of Wages,DA and other cash benefits paid/ to be paid to Semi-skilled of contract labour</label><span>'.$semiskilled_rate_wages.'<span style="float:right;">'.$readonly_semiskilled_rate_wages.'</span></span>
					</section>
					<section class="odd">
							<label>(c)Rate of Wages,DA and other cash benefits paid/ to be paid to Skilled of contract labour</label><span>'.$skilled_rate_wages.'<span style="float:right;">'.$readonly_skilled_rate_wages.'</span></span>
					</section>
					<section class="even">
							<label>(d)Rate of Wages,DA and other cash benefits paid/ to be paid to Highly-skilled of contract labour</label><span>'.$highlyskilled_rate_wages.'<span style="float:right;">'.$readonly_highlyskilled_rate_wages.'</span></span>
					</section>
									
					<section class="odd">
							<label>(e)Hours of Work, Spread over time, Overtime and Overtime Wages </label><span>Hours of Work:-'.$hours_work.' hr(s)<br> Spread over time:- '.$spred_over.' hr(s) <br> Overtime:- '.$overtime.' hr(s) <br>Overtime Wages:- <del>&#2352;</del> '.$overtime_wages.'(per hour)<span style="float:right;">'.$readonly_hours_work.'</span></span>
					</section>
					<div style="background:#3c4e5a; border:3px solid #e8e8e8;padding:5px;">
						  <span style="font-weight:bold;font-size: 15px;color: #FFF;"><center>Other Condition of service like leave (annual leave,casual leave,sick leave,maternity leave etc.) Holidays etc.of the contract labour</center></span>							
										
					</div>
				 
  
					<section class="odd">
							<label>(a)Number of Annual leave</label><span>'.$annual_leave_no.'<span style="float:right; position:relative; top:-64px;right:-94px;">'.$readonly_annual_leave_no.'</span></span>
					</section>
					<section class="even">
							<label>(b)Number of Casual leave</label><span>'.$casual_leave_no.'<span style="float:right;">'.$readonly_casual_leave_no.'</span></span>
					</section>
					<section class="odd">
							<label>(c)Number of Earned leave</label><span>'.$earned_leave_no.'<span style="float:right;">'.$readonly_eraned_leave_no.'</span></span>
					</section>
					<section class="even">
							<label>(d)Number of Sick leave</label><span>'.$sick_leave_no.'<span style="float:right;">'.$readonly_sick_leave_no.'</span></span>
					</section>
					<section class="odd">
							<label>(e)Number of Maternity leave</label><span>'.$maternity_leave_no.'<span style="float:right;">'.$readonly_maternity_leave_no.'</span></span>
					</section>
					
					 <section class="odd">
							<label>(f)Number of Other leave</label><span>'.$other_leave_no.'<span style="float:right;">'.$readonly_other_leave_no.'</span></span>
					 </section>
					<section class="odd">
							<label>Whether weekly holiday(s) observed and on which day</label><span>'.$total_holiday.' day(s)('.$name_holidays.')<span style="float:right;">'.$readonly_weekly_holiday.'</span></span>
					</section>
					<section class="even">
							<label>Whether weekly holiday(s) so observed was paid holiday(s)</label><span>'.$holiday_wages.'<span style="float:right;">'.$readonly_weekly_holiday.'</span></span>
					</section>
					<section class="odd">
							<label>Whether the contractor was convicted of any offence within the preceding five years. If so, give details</label><span>'.$contractor_convicted.'<span style="float:right;"> '.$readonly_offence.'</span></span>
					</section>
					<section class="even">
							<label>Whether there was any order against the contract or revoking or suspending license or forfeiting security deposit in respect of an earlier contract. If so, the date of such order11</label><span>Revoking Date :- '.$contractor_revoking_date.'<span style="float:right;">'.$readonly_contractor_revoking.'</span></span>
					</section>
					<section class="odd">
							<label>Whether the contractor has worked in any other establishment within the past five years. If so. give details of the principal employer, establishment and nature of work</label><span>'.$pe_details_reg.'<span style="float:right;">'.$readonly_contractor_revoking.'</span></span>
					</section>
					<section class="even">
							<label>Special benefits provided, if any </label><span>'.$special_benifites.'<span style="float:right;">'.$readonly_special_benifites.'</span></span>
					</section>
					<section class="odd">
							<label>Contribution made under the Employees Provident Fund and Miscellaneous Provision Act,1952 </label><span>'.$miscellaneous_provisions.'<span style="float:right;">'.$readonly_miscellaneous.'</span></span>
					</section>
					<section class="even">
							<label>Contribution made under the Employees State Insurance Act,1948</label><span>'.$state_insurance.'<span style="float:right;">'.$readonly_state_insurance.'</span></span>
					</section>
					<div style="background:#3c4e5a; border:3px solid #e8e8e8;padding:5px;">
							<span style="font-weight:bold;font-size: 15px;color: #FFF;"><center>Uploaded Documents</center></span>							
					</div>
					<section class="odd">
							 <label>Work Order</label>';
								if(!empty($work_order_file_id)){ 
									$output.='<span><a title = "Click here to view Work Order" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$work_order_url.'" target="_blank"><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/icon-dn-pdf.png"> View Work Order </a><span style="float:right;">'.$readonly_workorder.'</span></span>';
			 					}else{
			   						$output.='<span>No Document uploaded<span style="float:right;">'.$readonly_workorder.'</span></span>';
			  					}
				$output.='</section>
						  <section class="even">
							  <label>Form V</label>';
									if(!empty($frm_v_file_id)){
											$output.='<span><a title = "Click here to view Form V" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$frmv_url.'" target="_blank"><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/icon-dn-pdf.png"> View Form V </a><span style="float:right;">'.$previous_docu.'</span></span>';
			  						}else{
			   								$output.='<span>No Document uploaded<span style="float:right;"> '.$previous_docu.'</span></span>';
			  						}
										
					$output.='</section>
							  <section class="odd">
										<label>Residential Certificate/Tread License</label>';
											if(!empty($residential_file_id)){
													$output.='<span><a title = "Click here to view Residential Certificate" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$residential_url.'" target="_blank"><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/icon-dn-pdf.png"> View Residential Certificate/Tread License </a><span style="float:right;">'.$previous_docu.'</span></span>';
			  								}else{
			   										$output.='<span>No Document uploaded <span style="float:right;">'.$previous_docu.'</span></span>';
			  								}	
					$output.='</section>
							  <section class="even">
										<label>Previous Form IV or VII or Ammendment Application Form</label>';
											if(!empty($form_iv_vii_id)){
												  $output.='<span><a title = "Click here to view Form" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$form_iv_vii_url.'" target="_blank"><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/icon-dn-pdf.png"> View Form</a><span style="float:right;">'.$previous_docu.'</span></span>';
			  						}else{
			   								$output.='<span>No Document uploaded<span style="float:right;"> '.$previous_docu.'</span></span>';
									}
										
					$output.='</section>
					
							  <section class="odd">
										<label>Previous License or Renewal or Ammendment Certificate</label>';
											if(!empty($certificate_id)){
												$output.='<span><a title = "Click here to view Certificate" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$certificate_url.'" target="_blank"><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/icon-dn-pdf.png"> View Certificate</a> <span style="float:right;">'.$previous_docu.'</span></span>';
												
			  								}else{
			   									$output.='<span>No Document uploaded<span style="float:right;"> '.$previous_docu.'</span></span>';
			  								}	
										
										
					
					$output.='</section>
							 <section class="even">
									<label>Other Document</label>';
										if(!empty($other_file_id)){
												$output.='<span><a title = "Click here to view Other Document" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$other_url.'" target="_blank"><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/icon-dn-pdf.png"> View Other Document</a><span style="float:right;">'.$readonly_other_document.'</span></span>';
			  							}else{
			   								$output.='<span>No Document uploaded <span style="float:right;"></span></span>';
			  							}
					                   
					$output.='	</section>
					
					
					
								
			</div>
	 </div>';
						   
	return $output;
}
function apply_amendment_submit($form, &$form_state){ 
	
	global $base_root, $base_path, $user;
	$ammendment_info 							= array();
	$val										=	$form_state['values']; 
	$contractor_id 								= 	$user->uid;
	
		// echo "<pre>";
		// print_r($val);die;

		// echo "Surver under maintenance";
		// die;
		
	$id											= 	$val['id'];
	$status_from 								= 	$val['status_from'];
	$serialno_1									=	$val['serialno'];
	$renewallicenseammendment_id_1				=	$val['renewallicenseammendment_id'];
	$applicationflag_1							=	$val['applicationflag'];
	$renewallicenseammendment_id				=	encryption_decryption_fun('decrypt', $val['renewal_licenseammendment_id']);
	$applicationflag							=	encryption_decryption_fun('decrypt', $val['applicationflag']);	
	$serialno									=	encryption_decryption_fun('decrypt', $val['serialno']);
	$application_id								=	encryption_decryption_fun('decrypt', $val['application_id']);
	$pe_userid									=	encryption_decryption_fun('decrypt', $val['pe_userid']);
	
	
	 //-------------------------Work order Doc-----------------------------------------
	$work_order_file_details 	= fileupload($val['work_order']);
	 if ($work_order_file_details!="")  {
		$work_order_file_fid	=	$work_order_file_details->fid;
	  
	}
	 //-------------------------Other Doc-----------------------------------------
	$other_document_details 	= fileupload($val['other_document']);
	 if ($other_document_details!="")  {
		$other_file_fid			=	$other_document_details->fid;
	  
	}
	
	if($applicationflag == 'L'){
		$license_id		=	$renewallicenseammendment_id;//---kaj baki		
	}if($applicationflag == 'A'){
		//$license_id								=	return_ammendement_license_id($renewallicenseammendment_id);//---kaj baki
		 $license_id	=	get_amd_license_id($renewallicenseammendment_id);		 
	}if($applicationflag == 'R'){
		$license_id		=	return_renewal_license($renewallicenseammendment_id);//---kaj baki
	}
	
	//$check_ammendment_table 					=  check_ammendment_table($renewallicenseammendment_id);	
	//$details_ammendment_table 					=  $check_ammendment_table->fetchAssoc();
	//$details_ammendment_table 					=  check_ammendment_table_license_renewal_ammendment_id($renewallicenseammendment_id);

	$details_ammendment_table                   = check_ammendment_table_new($renewallicenseammendment_id,$serialno);	
	$ammendment 								= check_ammendment_fields($license_id,$serialno)->fetchAssoc();	 //exit;
	 
	 $personal_info_flag1 						=  $details_ammendment_table['personal_info_flag'];
	 $worksite_details_flag						=  $details_ammendment_table['worksite_details_flag'];
	 $contract_labour_flag 						=  $details_ammendment_table['contract_labour_flag'];
	 $ammendment_status 						=  $details_ammendment_table['ammendment_status'];
	 $ammendment_final_status 					=  $details_ammendment_table['ammendment_final_status'];	
	 $reference_no 								=  $details_ammendment_table['reference_no'];	
	
	
	$not_amendment_check_field	= $ammendment['not_amendment_check_field'];
	if(!empty($not_amendment_check_field)){
		$details 							= 	contractor_regmaster_details($serialno);
		$name_of_contractor_pervious	 	= 	$details['name_of_contractor'];
		$particular_data 					= particular_data($serialno);
		
		if($applicationflag == 'L'){			
			if(!empty($particular_data)){
				$particularid									= $particular_data['id'];
				$father_contarctor_name_license_renewal 		= $particular_data['father_contarctor_name'];
				$address_line1_of_contractor_license_renewal	= $particular_data['address_of_contractor']; 
				$contractor_dist_license_renewal				= $particular_data['contractor_dist']; 
				$contractor_subdivision_license_renewal			= $particular_data['contractor_subdivision']; 
				$contractor_name_areatype_license_renewal	    = $particular_data['contractor_name_areatype'];
				$contractor_areatype_license_renewal	    	= $particular_data['contractor_areatype'];
				$contractor_vill_ward_license_renewal	    	= $particular_data['contractor_vill_ward'];
				$contractor_ps_license_renewal	    			= $particular_data['contractor_ps'];
				$contractor_pin_license_renewal	    			= $particular_data['contractor_pin'];
				$category_of_contractor_license_renewal			= $particular_data['category_of_contractor'];
				$dob_contractor_license_renewal					= $particular_data['dob_contractor'];
				$age_contractor_license_renewal					= $particular_data['age_contractor'];
				$unskilled_rate_wages_license_renewal			= $particular_data['unskilled_rate_wages'];
				$semiskilled_rate_wages_license_renewal			= $particular_data['semiskilled_rate_wages']; 
				$skilled_rate_wages_license_renewal				= $particular_data['skilled_rate_wages']; 
				$highlyskilled_rate_wages_license_renewal		= $particular_data['highlyskilled_rate_wages']; 
				$annual_leave_no_license_renewal				= $particular_data['annual_leave_no']; 
				$casual_leave_no_license_renewal			    = $particular_data['casual_leave_no']; 
				$sick_leave_no_license_renewal					= $particular_data['sick_leave_no'];
				$maternity_leave_no_license_renewal				= $particular_data['maternity_leave_no'];
				$earned_leave_no_license_renewal				= $particular_data['earned_leave_no'];
				$other_leave_no_license_renewal					= $particular_data['other_leave_no']; 
				$overtime_wages_license_renewal					= $particular_data['overtime_wages']; 
				$weekly_holiday_license_renewal					= $particular_data['weekly_holiday']; 
				$no_holiday_license_renewal						= $particular_data['no_holiday']; 
				$hours_work_license_renewal						= $particular_data['hours_work']; 
				$spred_over_license_renewal						= $particular_data['spred_over']; 
				$holiday_wages_license_renewal					= $particular_data['holiday_wages'];
				$category_designation_license_renewal			= $particular_data['category_designation'];
				$special_benifites_license_renewal				= $particular_data['special_benifites'];
				$miscellaneous_provisions_license_renewal		= $particular_data['miscellaneous_provisions'];
				$contractor_convicted_license_renewal			= $particular_data['contractor_convicted'];
				$details_contractor_convicted_license_renewal	= $particular_data['details_contractor_convicted'];
				$state_insurance_license_renewal				= $particular_data['state_insurance'];
				$worksite_address_line_license_renewal			= $particular_data['worksite_address_line'];
				$worksite_dist_license_renewal					= $particular_data['worksite_dist'];
				$work_site_areatype_license_renewal				= $particular_data['work_site_areatype'];
				$worksite_subdivision_license_renewal			= $particular_data['worksite_subdivision'];
				$name_work_site_areatype_license_renewal		= $particular_data['name_work_site_areatype'];
				$work_site_vill_ward_license_renewal			= $particular_data['work_site_vill_ward'];
				$worksite_pin_license_renewal					= $particular_data['worksite_pin'];
				$worksite_ps_license_renewal					= $particular_data['worksite_ps'];
				$max_no_of_labours_license_renewal				= $particular_data['contractor_max_no_of_labours_on_any_day'];
				$is_coparative_license_renewal					= $particular_data['is_coparative'];
				$amount_of_licence_renewal						= $particular_data['amount_of_licence'];
				$amount_of_security_deposit_licence_renewal		= $particular_data['amount_of_security_deposit'];

				$name_of_agent_license_renewal 					= $particular_data['name_of_agent'];
				$address_of_manager_license_renewal				= $particular_data['address_of_manager']; 
				$contractor_manager_dist_license_renewal		= $particular_data['contractor_manager_dist']; 
				$contractor_manager_subdivision_license_renewal	= $particular_data['contractor_manager_subdivision']; 
				$contractor_manager_name_areatype_license_renewal= $particular_data['contractor_manager_name_areatype'];
				$contractor_manager_areatype_license_renewal	 = $particular_data['contractor_manager_areatype'];
				$contractor_managerr_vill_ward_license_renewal	 = $particular_data['contractor_managerr_vill_ward'];
				$contractor_manager_ps_license_renewal	    	 = $particular_data['contractor_manager_ps'];
				$manager_pin_license_renewal	    			 = $particular_data['manager_pin'];
				$overtime_license_renewal						 = $particular_data['overtime'];
				$category_designation_license_renewal			 = $particular_data['category_designation'];
				$contractor_revoking_license_renewal			=	$particular_data['details_contractor_revoking'];
			}
		}
		
		if($applicationflag == 'R'){
			$renewal_data = 	renewal_data_last($renewallicenseammendment_id);
			$name_of_agent_license_renewal 						= 	$particular_data['name_of_agent'];
			$address_of_manager_license_renewal						= 	$particular_data['address_of_manager']; 
			$contractor_manager_dist_license_renewal				= 	$particular_data['contractor_manager_dist']; 
			$contractor_manager_subdivision_license_renewal			= 	$particular_data['contractor_manager_subdivision']; 
			$contractor_manager_name_areatype_license_renewal	    =	$particular_data['contractor_manager_name_areatype'];
			$contractor_manager_areatype_license_renewal	    	= 	$particular_data['contractor_manager_areatype'];
			$contractor_managerr_vill_ward_license_renewal	    	= 	$particular_data['contractor_managerr_vill_ward'];
			$contractor_manager_ps_license_renewal	    			= 	$particular_data['contractor_manager_ps'];
			$manager_pin_license_renewal	    					=	$particular_data['manager_pin'];
			$worksite_address_line_license_renewal					= 	$particular_data['worksite_address_line'];
			$worksite_dist_license_renewal							= 	$particular_data['worksite_dist'];
			$work_site_areatype_license_renewal						= 	$particular_data['work_site_areatype'];
			$worksite_subdivision_license_renewal					= 	$particular_data['worksite_subdivision'];
			$name_work_site_areatype_license_renewal				= 	$particular_data['name_work_site_areatype'];
			$work_site_vill_ward_license_renewal					= 	$particular_data['work_site_vill_ward'];
			$worksite_pin_license_renewal							= 	$particular_data['worksite_pin'];
			$worksite_ps_license_renewal							= 	$particular_data['worksite_ps'];
			$category_designation_license_renewal					= $particular_data['category_designation'];
		
			$father_contarctor_name_license_renewal 				= $particular_data['father_contarctor_name'];
			$address_line1_of_contractor_license_renewal			= $particular_data['address_of_contractor']; 
			$contractor_dist_license_renewal						= $particular_data['contractor_dist']; 
			$contractor_subdivision_license_renewal					= $particular_data['contractor_subdivision']; 
			$contractor_name_areatype_license_renewal	    		= $particular_data['contractor_name_areatype'];
			$contractor_areatype_license_renewal	    			= $particular_data['contractor_areatype'];
			$contractor_vill_ward_license_renewal	    			= $particular_data['contractor_vill_ward'];
			$contractor_ps_license_renewal	    					= $particular_data['contractor_ps'];
			$contractor_pin_license_renewal	    					= $particular_data['contractor_pin'];
			$category_of_contractor_license_renewal					= $particular_data['category_of_contractor'];
			$dob_contractor_license_renewal							= $particular_data['dob_contractor'];
			$age_contractor_license_renewal							= $particular_data['age_contractor'];
			
			
			
			$unskilled_rate_wages_license_renewal					= 	$renewal_data['unskilled_rate_wages'];
			$semiskilled_rate_wages_license_renewal					= 	$renewal_data['semiskilled_rate_wages']; 
			$skilled_rate_wages_license_renewal						= 	$renewal_data['skilled_rate_wages']; 
			$highlyskilled_rate_wages_license_renewal				= 	$renewal_data['highlyskilled_rate_wages']; 
			
			$annual_leave_no_license_renewal						=	$renewal_data['annual_leave_no'];
			$casual_leave_no_license_renewal						=	$renewal_data['casual_leave_no'];
			$sick_leave_no_license_renewal							=	$renewal_data['sick_leave_no'];
			$maternity_leave_no_license_renewal						=	$renewal_data['maternity_leave_no'];
			$other_leave_no_license_renewal							=	$renewal_data['other_leave_no'];
			$earned_leave_no_license_renewal						=	$renewal_data['earned_leave_no'];
			
			$hours_work_license_renewal								=	$renewal_data['hours_work'];
			$spred_over_license_renewal								=	$renewal_data['spred_over'];
			$holiday_wages_license_renewal							=	$renewal_data['holiday_wages'];
			$weekly_holiday_license_renewal							= 	$renewal_data['weekly_holiday'];
			$no_holiday_license_renewal								= 	$renewal_data['no_holiday'];
			$max_no_of_labours_license_renewal						= 	$renewal_data['max_of_contract_labour'];
			
			
			$special_benifites_license_renewal						=	$renewal_data['special_benifites'];
			$state_insurance_license_renewal						= 	$renewal_data['state_insurance'];
			$miscellaneous_provisions_license_renewal				=	$renewal_data['miscellaneous_provisions'];
			$contractor_revoking_license_renewal					=	$renewal_data['details_contractor_revoking'];
			$contractor_convicted_license_renewal			   		 =	$particular_data['details_contractor_convicted'];
		}
	}
	$name_of_contractor				= !empty($details_ammendment_table['name_of_contractor'])?$details_ammendment_table['name_of_contractor']:$name_of_contractor_pervious; 
	$father_contarctor_name			= !empty($details_ammendment_table['father_contarctor_name'])?$details_ammendment_table['father_contarctor_name']:$father_contarctor_name_license_renewal;
	$category_of_contractor			= !empty($details_ammendment_table['category_of_contractor'])?$details_ammendment_table['category_of_contractor']:$category_of_contractor_license_renewal;
	$dob_contractor					= !empty($details_ammendment_table['dob_contractor'])?$details_ammendment_table['dob_contractor']:$dob_contractor_license_renewal;
	$age_contractor					= !empty($details_ammendment_table['age_contractor'])?$details_ammendment_table['age_contractor']:$age_contractor_license_renewal; 
	$contarctor_address_line		= !empty($details_ammendment_table['contarctor_address_line'])?$details_ammendment_table['contarctor_address_line']:$address_line1_of_contractor_license_renewal; 
	$contractor_dist				= !empty($details_ammendment_table['contractor_dist'])?$details_ammendment_table['contractor_dist']:$contractor_dist_license_renewal; 
	$contractor_subdivision			= !empty($details_ammendment_table['contractor_subdivision'])?$details_ammendment_table['contractor_subdivision']:$contractor_subdivision_license_renewal; 
	$contractor_name_areatype	    = !empty($details_ammendment_table['contractor_name_areatype'])?$details_ammendment_table['contractor_name_areatype']:$contractor_name_areatype_license_renewal;
	$contractor_areatype    		= !empty($details_ammendment_table['contractor_areatype'])?$details_ammendment_table['contractor_areatype']:$contractor_areatype_license_renewal;
    $contractor_vill_ward    		= !empty($details_ammendment_table['contractor_vill_ward'])?$details_ammendment_table['contractor_vill_ward']:$contractor_vill_ward_license_renewal;
	$contractor_ps	    			= !empty($details_ammendment_table['contractor_ps'])?$details_ammendment_table['contractor_ps']:$contractor_ps_license_renewal;
	$contractor_pin	    			= !empty($details_ammendment_table['contractor_pin'])?$details_ammendment_table['contractor_pin']:$contractor_pin_license_renewal;
	$unskilled_rate_wages   		= !empty($details_ammendment_table['unskilled_rate_wages'])?$details_ammendment_table['unskilled_rate_wages']:$unskilled_rate_wages_license_renewal;
	$semiskilled_rate_wages    		= !empty($details_ammendment_table['semiskilled_rate_wages'])?$details_ammendment_table['unskilled_rate_wages']:$semiskilled_rate_wages_license_renewal;
	$skilled_rate_wages   	 		= !empty($details_ammendment_table['skilled_rate_wages'])?$details_ammendment_table['unskilled_rate_wages']:$skilled_rate_wages_license_renewal;
	$highlyskilled_rate_wages  		= !empty($details_ammendment_table['highlyskilled_rate_wages'])?$details_ammendment_table['unskilled_rate_wages']:$highlyskilled_rate_wages_license_renewal;
	$annual_leave_no				= !empty($details_ammendment_table['annual_leave_no'])?$details_ammendment_table['annual_leave_no']:$annual_leave_no_license_renewal; 
	$casual_leave_no				= !empty($details_ammendment_table['casual_leave_no'])?$details_ammendment_table['casual_leave_no']:$casual_leave_no_license_renewal; 
	$sick_leave_no					= !empty($details_ammendment_table['sick_leave_no'])?$details_ammendment_table['sick_leave_no']:$sick_leave_no_license_renewal;
	$maternity_leave_no				= !empty($details_ammendment_table['maternity_leave_no'])?$details_ammendment_table['maternity_leave_no']:$maternity_leave_no_license_renewal;
	$earned_leave_no				= !empty($details_ammendment_table['earned_leave_no'])?$details_ammendment_table['earned_leave_no']:(!empty($earned_leave_no_license_renewal)?$earned_leave_no_license_renewal:'0');
	$other_leave_no					= !empty($details_ammendment_table['other_leave_no'])?$details_ammendment_table['other_leave_no']:$other_leave_no_license_renewal;
	$overtime_wages					= !empty($details_ammendment_table['overtime_wages'])?$details_ammendment_table['overtime_wages']:$overtime_wages_license_renewal; 
    $overtime_wages					= !empty($details_ammendment_table['overtime_wages'])?$details_ammendment_table['overtime_wages']:$overtime_wages_license_renewal; 
	$weekly_holiday					= !empty($details_ammendment_table['weekly_holiday'])?$details_ammendment_table['weekly_holiday']:$weekly_holiday_license_renewal; 
	$no_holiday						= !empty($details_ammendment_table['no_holiday'])?$details_ammendment_table['no_holiday']:$no_holiday_license_renewal; 
	$hours_work						= !empty($details_ammendment_table['hours_work'])?$details_ammendment_table['hours_work']:$hours_work_license_renewal; 
	$spred_over						= !empty($details_ammendment_table['spred_over'])?$details_ammendment_table['spred_over']:$spred_over_license_renewal; 
	$holiday_wages					= !empty($details_ammendment_table['holiday_wages'])?$details_ammendment_table['holiday_wages']:$holiday_wages_license_renewal; 
	$category_designation			= !empty($details_ammendment_table['category_designation'])?$details_ammendment_table['category_designation']:$category_designation_license_renewal;
	$special_benifites				= !empty($details_ammendment_table['special_benifites'])?$details_ammendment_table['special_benifites']:$special_benifites_license_renewal; 
	$miscellaneous_provisions 		= !empty($details_ammendment_table['miscellaneous_provisions'])?$details_ammendment_table['miscellaneous_provisions']:$miscellaneous_provisions_license_renewal;
	$contractor_convicted			= !empty($details_ammendment_table['details_contractor_convicted'])?$details_ammendment_table['details_contractor_convicted']:$contractor_convicted_license_renewal; 
	$contractor_revoking			= !empty($details_ammendment_table['details_contractor_revoking'])?$details_ammendment_table['details_contractor_revoking']:$contractor_revoking_license_renewal;
	$state_insurance				= !empty($details_ammendment_table['state_insurance'])?$details_ammendment_table['state_insurance']:$state_insurance_license_renewal;
	$worksite_address_line			= !empty($details_ammendment_table['worksite_address_line'])?$details_ammendment_table['worksite_address_line']:$worksite_address_line_license_renewal;
	$worksite_dist					= !empty($details_ammendment_table['worksite_dist'])?$details_ammendment_table['worksite_dist']:$worksite_dist_license_renewal; 
	$work_site_areatype				= !empty($details_ammendment_table['work_site_areatype'])?$details_ammendment_table['work_site_areatype']:$work_site_areatype_license_renewal;
	$worksite_subdivision			= !empty($details_ammendment_table['worksite_subdivision'])?($details_ammendment_table['worksite_subdivision']):$worksite_subdivision_license_renewal;
	$name_work_site_areatype 		= !empty($details_ammendment_table['name_work_site_areatype'])?$details_ammendment_table['name_work_site_areatype']:$name_work_site_areatype_license_renewal;
	$work_site_vill_ward 			= !empty($details_ammendment_table['work_site_vill_ward'])?$details_ammendment_table['work_site_vill_ward']:$work_site_vill_ward_license_renewal;
	$worksite_pin					= !empty($details_ammendment_table['worksite_pin'])?$details_ammendment_table['worksite_pin']:$worksite_pin_license_renewal;
	$worksite_ps					= !empty($details_ammendment_table['worksite_ps'])?$details_ammendment_table['worksite_ps']:$worksite_ps_license_renewal;
	$max_of_contract_labour 		= !empty($details_ammendment_table['max_of_contract_labour'])?$details_ammendment_table['max_of_contract_labour']:$max_no_of_labours_license_renewal;
	$is_coparative					= !empty($details_ammendment_table['is_coparative'])?$details_ammendment_table['is_coparative']:!empty($is_coparative_license_renewal)?$is_coparative_license_renewal:0;
	//$max_of_contract_labour			= !empty($details_ammendment_table['max_of_contract_labour'])?$details_ammendment_table['max_of_contract_labour']:!empty($max_no_of_labours_license_renewal)?$max_no_of_labours_license_renewal:$max_no_of_labours_pe;
	$license_renewal_fees			= !empty($details_ammendment_table['license_renewal_fees'])?$details_ammendment_table['license_renewal_fees']:$amount_of_licence_renewal;
	$license_renewal_security_fees  = !empty($details_ammendment_table['license_renewal_security_fees'])?$details_ammendment_table['license_renewal_security_fees']:$amount_of_security_deposit_licence_renewal;
	
	$ammend_deposit_fees			= $details_ammendment_table['ammend_deposit_fees'];
	$deposited_max_security_fees	= $details_ammendment_table['deposited_max_security_fees'];	
	$ammendment_status				= $details_ammendment_table['ammendment_status'];
	
			//----------------fees calcution for ammendment-------------------
			

	
	$max_renewal_fees 	=  return_maximum_renewal_fees($license_id,$contractor_id);
	$max_ammend_fees 	=  return_maximum_ammendment_fees($license_id,$contractor_id);
	$max_license_fees	=  return_maximum_license_fees($serialno,$contractor_id);
	
	if($max_renewal_fees>$max_ammend_fees)
	    $max_fees1	= $max_renewal_fees;
    if($max_renewal_fees<$max_ammend_fees)
	    $max_fees1	= $max_ammend_fees;	 
	if($max_renewal_fees == $max_ammend_fees)
	    $max_fees1	= $max_ammend_fees;	 
	if($max_fees1>$max_license_fees)
		$max_fees2 = $max_fees1;
	if($max_fees1<$max_license_fees)
		$max_fees2 = $max_license_fees;
	if($max_fees1 == $max_license_fees)
		$max_fees2 = $max_license_fees;
		
		
	 $maximum_renewal_labour = return_maximum_renewal_labour($license_id,$contractor_id);
	 $maximum_ammend_labour	 = return_maximum_ammendment_labour($license_id,$contractor_id);
	 $maximum_license_labour = return_maximum_license_labour($serialno,$contractor_id);
		
	 if($maximum_renewal_labour>$maximum_ammend_labour)
	    $max_labour1	= $maximum_renewal_labour;
    if($maximum_renewal_labour<$maximum_ammend_labour)
	    $max_labour1	= $maximum_renewal_labour;	 
	if($maximum_renewal_labour == $maximum_ammend_labour)
	    $max_labour1	= $maximum_ammend_labour;	 
	if($max_labour1>$maximum_license_labour)
		$$max_labour2 = $max_labour1;
	if($max_labour1<$maximum_license_labour)
		$max_labour2 = $maximum_license_labour;
	if($max_labour1 == $maximum_license_labour)
		$max_labour2 = $maximum_license_labour;
		
		//---------------------------------------------fees calcution for ammendment------------------------------------------------
	if($max_labour2>$max_of_contract_labour)
				$highest_labour = $max_labour2;
	if($max_labour2<$max_of_contract_labour)
			$highest_labour = $max_of_contract_labour;
	if($max_labour2 == $max_of_contract_labour)
			$highest_labour = $max_of_contract_labour;
			
			
	$ammendent_fees		=   applicable_ammendment_fees_calculation($max_of_contract_labour);
	
	if($max_fees2>$ammend_deposit_fees){
		$currecnt_deposit_fees = $max_fees2;
	}if($max_fees2<$ammend_deposit_fees){
		$currecnt_deposit_fees = $ammend_deposit_fees;
	}if($max_fees2 == $ammend_deposit_fees){
		$currecnt_deposit_fees = $ammend_deposit_fees;
	}
	if($highest_labour>400){
		$paymantable_ammenedent_fees 	= 0.00;
		$deposit_fees_value				= $currecnt_deposit_fees;
	}if($highest_labour<=400){
		if($currecnt_deposit_fees>$ammendent_fees){
			$paymantable_ammenedent_fees 	= 0.00;
			$deposit_fees_value				= $currecnt_deposit_fees;
		}if($currecnt_deposit_fees<$ammendent_fees){
			  $paymantable_ammenedent_fees 	= $ammendent_fees-$currecnt_deposit_fees;
			  $deposit_fees_value			= $ammendent_fees;
		}if($currecnt_deposit_fees==$ammendent_fees){
			 $paymantable_ammenedent_fees 	= 0.00;
			 $deposit_fees_value			= $ammendent_fees;
		}
	}


 	$applicable_amendment_fee   	= !empty($details_ammendment_table['applicable_amendment_fee'])?$details_ammendment_table['applicable_amendment_fee']:!empty($ammendent_fees)?$ammendent_fees:'0';
 
	$payable_amendment_fees   		= !empty($details_ammendment_table['payable_amendment_fees'])?$details_ammendment_table['payable_amendment_fees']:!empty($paymantable_ammenedent_fees)?$paymantable_ammenedent_fees:'0';
	
	if(empty($ammendment_status) || $ammendment_status == 'I')
		$previous_deposit_fees   		= $currecnt_deposit_fees;
	else
		$previous_deposit_fees   		= !empty($details_ammendment_table['previous_highest_deposit_fees'])?$details_ammendment_table['previous_highest_deposit_fees']:$currecnt_deposit_fees;
			
		//---------------------------------------------fees calcution for security------------------------------------------------
	$max_license_security_fees 			= return_maximum_license_security_fees($serialno,$contractor_id);
	$max_ammendment_security_fess       = return_maximum_ammendment_security_fess($license_id,$contractor_id);
	$max_renewal_security_fess			= return_maximum_renewal_security($license_id,$contractor_id);

	
	if($max_license_security_fees>$max_ammendment_security_fess){
		$max_security_fees1	=$max_license_security_fees;	 
		
	}if($max_license_security_fees<$max_ammendment_security_fess){
		$max_security_fees1	=$max_ammendment_security_fess;	 
		
	}if($max_license_security_fees==$max_ammendment_security_fess){
		$max_security_fees1	=$max_ammendment_security_fess;	 
		
	}if($max_security_fees1>$max_renewal_security_fess){
		$max_security_fees2 = $max_security_fees1;
	}
	if($max_security_fees1<$max_renewal_security_fess){
		$max_security_fees2 = $max_renewal_security_fess;
	}
	if($max_security_fees1==$max_renewal_security_fess){
		$max_security_fees2 = $max_renewal_security_fess;
	}
	if($max_security_fees2>$deposited_max_security_fees){
		$current_security_deposit = $max_security_fees2;
	}

	if($max_security_fees2<$deposited_max_security_fees){
		$current_security_deposit = $deposited_max_security_fees;
	}
	if($max_security_fees2 == $deposited_max_security_fees){
		$current_security_deposit = $deposited_max_security_fees;
	}
	
	
	if(empty($ammendment_status) || $ammendment_status == 'I'){
		$previous_max_security 		= $current_security_deposit;
		
	}
	else{
	 $previous_max_security   	= !empty($details_ammendment_table['previous_highest_security'])?$details_ammendment_table['previous_highest_security']:$current_security_deposit;
	
	}
	$ammendent_security_fees 	=  applicable_ammendment_security_fees_calculation($max_of_contract_labour,$value_is_coparative);
	
	if($ammendent_security_fees>$previous_max_security){
		$paymantable_sucurity_ammenedent_fees = $ammendent_security_fees-$previous_max_security;
		if(!empty($due_security_fees_amount_license_renewal) && $due_security_fees_amount_license_renewal!=0){
			
			$paymantable_sucurity_ammenedent_fees 		= $paymantable_sucurity_ammenedent_fees+$due_security_fees_amount_license_renewal;
			$deposited_security_fees					= $ammendent_security_fees;
		}else{
			$deposited_security_fees					= $ammendent_security_fees;
		}
	}if($ammendent_security_fees<$previous_max_security){
		$paymantable_sucurity_ammenedent_fees = 0.00;
		if(!empty($due_security_fees_amount_license_renewal) && $due_security_fees_amount_license_renewal!=0){
			
			$paymantable_sucurity_ammenedent_fees 		= $due_security_fees_amount_license_renewal;
			$deposited_security_fees					= $previous_max_security;
		}else{
			
			$deposited_security_fees					= $previous_max_security;
		}
	}if($ammendent_security_fees==$previous_max_security){
		$paymantable_sucurity_ammenedent_fees = 0.00;
		if(!empty($due_security_fees_amount_license_renewal) && $due_security_fees_amount_license_renewal!=0){
			
			$paymantable_sucurity_ammenedent_fees 		= $due_security_fees_amount_license_renewal;
			$deposited_security_fees					= $previous_max_security;
		}else{
			
			$deposited_security_fees					= $previous_max_security;
		}
	}
		
		
	$applicable_security_fess   = !empty($details_ammendment_table['applicable_security_fess'])?$details_ammendment_table['applicable_security_fess']:!empty($ammendent_security_fees)?$ammendent_security_fees:'0';
		
	$payable_amendment_security_fees  =  !empty($details_ammendment_table['payable_amendment_security_fees'])?$details_ammendment_table['payable_amendment_security_fees']:$paymantable_sucurity_ammenedent_fees;
	
	
	$name_of_agent 					 = !empty($details_ammendment_table['name_of_agent'])?$details_ammendment_table['name_of_agent']:$name_of_agent_license_renewal;
	$address_of_manager				 = !empty($details_ammendment_table['address_of_manager'])?$details_ammendment_table['address_of_manager']:$address_of_manager_license_renewal; 
	$contractor_manager_dist		 = !empty($details_ammendment_table['contractor_manager_dist'])?$details_ammendment_table['contractor_manager_dist']:$contractor_manager_dist_license_renewal; 
	$contractor_manager_subdivision	 = !empty($details_ammendment_table['contractor_manager_subdivision'])?$details_ammendment_table['contractor_manager_subdivision']:$contractor_manager_subdivision_license_renewal; 
	$contractor_manager_name_areatype = !empty($details_ammendment_table['contractor_manager_name_areatype'])?$details_ammendment_table['contractor_manager_name_areatype']:$contractor_manager_name_areatype_license_renewal;
	$contractor_manager_areatype	  = !empty($details_ammendment_table['contractor_manager_areatype'])?$details_ammendment_table['contractor_manager_areatype']:$contractor_manager_areatype_license_renewal;
	$contractor_managerr_vill_ward	  = !empty($details_ammendment_table['contractor_managerr_vill_ward'])?$details_ammendment_table['contractor_managerr_vill_ward']:$contractor_managerr_vill_ward_license_renewal;
	$contractor_manager_ps	    	  = !empty($details_ammendment_table['contractor_manager_ps'])?$details_ammendment_table['contractor_manager_ps']:$contractor_manager_ps_license_renewal;
	$contractor_manager_pin	    	  = !empty($details_ammendment_table['contractor_manager_pin'])?$details_ammendment_table['contractor_manager_pin']:$manager_pin_license_renewal;
	$overtime						 = !empty($details_previous_employer['overtime'])?$record['overtime']:$overtime_license_renewal;
	$amend_id						 =  $details_ammendment_table['id'];
	//$ammendment_status1				 =  $details_ammendment_table['ammendment_final_status'];
	//$ammendment_status				 =  $details_ammendment_table['ammendment_final_status'];	
	
			$editable_field 		= explode(',', $not_amendment_check_field);
			
			
				foreach($editable_field as $row){
					if($row == 'contractor_dist'){
						$ammendment_info1							= 	array(
							
								'category_of_contractor'			=>	$category_of_contractor,
								'father_contarctor_name'			=>	$father_contarctor_name,
								'dob_contractor'					=>	$dob_contractor, 
								'age_contractor'					=>	$age_contractor	,
								'contractor_dist'					=> $contractor_dist,
								'contractor_subdivision'			=> $contractor_subdivision,
								'contractor_areatype'				=> $contractor_areatype,
								'contractor_name_areatype'			=> $contractor_name_areatype,
								'contractor_vill_ward'				=> $contractor_vill_ward,
								'contractor_ps'                     => $contractor_ps,
								'address_of_contractor'				=> $contarctor_address_line,
								'contractor_pin'					=> $contractor_pin,
								'personal_info_flag'				=> 1,
						);
							$ammendment_info = (array_merge($ammendment_info,$ammendment_info1));
				
					
					}if($row == 'unskilled_rate_wages'||$row == 'semiskilled_rate_wages'||$row == 'skilled_rate_wages'||$row == 'highlyskilled_rate_wages'){
							$ammendment_info2=array( 
									'unskilled_rate_wages' 		=>	$unskilled_rate_wages,
									'semiskilled_rate_wages' 	=> 	$semiskilled_rate_wages,
									'skilled_rate_wages' 		=>	$skilled_rate_wages,
									'highlyskilled_rate_wages' 	=>	$highlyskilled_rate_wages,
									'contract_labour_flag'		=>	1
								
							);
							
							$ammendment_info = (array_merge($ammendment_info,$ammendment_info2));
							
					
					}if($row == 'casual_leave'||$row == 'eraned_leave'||$row == 'sick_leave'||$row == 'maternity_leave'){
							$ammendment_info3=array( 
									'annual_leave_no' 			=>	$annual_leave_no,
									'casual_leave_no' 			=> 	$casual_leave_no,
									'sick_leave_no' 			=>	$sick_leave_no,
									'maternity_leave_no' 		=>	$maternity_leave_no,
									'earned_leave_no'			=>  $earned_leave_no,
									'other_leave_no'			=>  $other_leave_no,
									'contract_labour_flag'		=>	1
						
							);
							
							$ammendment_info = (array_merge($ammendment_info,$ammendment_info3));
							
					
					}if($row == 'hour_details'){
						$ammendment_info4	=array( 
								'hours_work' 			=>	$hours_work,
								'spred_over' 			=> 	$spred_over,

								'overtime' 				=>	$overtime,
								'weekly_holiday' 		=>	$weekly_holiday,
								'overtime_wages'		=>  $overtime_wages,
								'no_holiday'			=>  $no_holiday,
								'holiday_wages'			=>  $holiday_wages,
								'contract_labour_flag'	=>	1
									
							);
						$ammendment_info = (array_merge($ammendment_info,$ammendment_info4));
					}if($row == 'category'){
						$ammendment_info5	=array( 
								'category_designation' 		=>	$category_designation,
								'contract_labour_flag'		=>	1
							);
						$ammendment_info = (array_merge($ammendment_info,$ammendment_info5));
					}if($row == 'special_benifites'){
						$ammendment_info6	=array( 
								'special_benifites' 		=>	$special_benifites,
								'contract_labour_flag'		=>	1
							);
						$ammendment_info = (array_merge($ammendment_info,$ammendment_info6));
					
					}if($row == 'miscellaneous'){
						$ammendment_info7	=array( 
							'miscellaneous_provisions' 		=>	$miscellaneous_provisions,
							'contract_labour_flag'			=>	1
						);
				
						$ammendment_info = (array_merge($ammendment_info,$ammendment_info7));
				
					}if($row == 'offence'){
							$ammendment_info8	=array( 
									'contractor_convicted' 				=>	1,
									'details_contractor_convicted' 		=>	$contractor_convicted,
									'contract_labour_flag'				=>	1
							);
							
						$ammendment_info = (array_merge($ammendment_info,$ammendment_info8));
					}
					if($row == 'revoking'){
						$ammendment_info9	=array( 
									'contractor_revoking' 				=>	1,
									'details_contractor_revoking' 		=>	$contractor_revoking,
									'contract_labour_flag'				=>	1
							);
							
						$ammendment_info = (array_merge($ammendment_info,$ammendment_info9));
						
					}
		
					/*/*if($row == 'worksite_address'){*/
							
									
					//}
					if($row == 'state_insurance'){
							$ammendment_info10	=array( 
									'state_insurance' 		=>	$state_insurance,
									'contract_labour_flag'	=>	1
							);
							
						$ammendment_info = (array_merge($ammendment_info,$ammendment_info10));
									
					}
					
					// if($row == 'contractor_max_no_of_labours_on_any_day'){ 
					// 		$ammendment_info12	=array( 
									
									
					// 			'max_of_contract_labour'				=> $max_of_contract_labour,
					// 			'applicable_security_fess'				=> $applicable_security_fess,
					// 			'payable_amendment_security_fees'		=> $payable_amendment_security_fees,
					// 			'payable_amendment_fees'				=> $payable_amendment_fees,
					// 			'ammend_deposit_fees'					=> $deposit_fees_value,
					// 			'previous_highest_deposit_fees'			=> $previous_deposit_fees,
								
								
								
					// 			'deposited_max_security_fees'			=> $previous_max_security,
								
					// 			'previous_highest_security'				=> $previous_max_security,
					// 			'is_coparative'							=> $is_coparative,
					// 			'due_security_fees'						=> $due_security_fees_amount_license_renewal,
					// 			'worksite_details_flag'					=> 1,
					// 			'applicable_amendment_fee'				=> $applicable_amendment_fee
									
									
					// 		);
							
						
					// 	$ammendment_info = (array_merge($ammendment_info,$ammendment_info12)); 
						
								
					// }
					if($row == 'address_of_manager'){
							$ammendment_info14	=array( 
									'name_of_agent' 					=>	$name_of_agent,
									'address_of_manager' 				=> 	$address_of_manager,
									'contractor_manager_dist' 			=>	$contractor_manager_dist,
									'contractor_manager_subdivision' 	=>	$contractor_manager_subdivision,
									'contractor_manager_name_areatype' 	=>	$contractor_manager_name_areatype,
									'contractor_manager_areatype' 		=>	$contractor_manager_areatype,
									'contractor_managerr_vill_ward' 	=>	$contractor_managerr_vill_ward,
									'contractor_manager_ps' 			=>	$contractor_manager_ps,
									'contractor_manager_pin' 			=>	$contractor_manager_pin,
							);
							
						$ammendment_info = (array_merge($ammendment_info,$ammendment_info14));
									
					}
				
			
			
	
			
				}

	
	if(empty($ammendment_status)|| $ammendment_status=='I'){ 
		//$reference_no = mt_rand(100000, 999999);
		$reference_no = create_reference_no();
			$ammendment_info13 =array(
						'reference_no'	=>	$reference_no,
						'ain_amendment' => 'AMEND-LIC-00'.$license_id.'-00'.$reference_no.'-'.$contractor_id.'-'.date('Y')
				);

		$ammendment_info = (array_merge($ammendment_info,$ammendment_info13));
	}

	$ammendment_info12	=array( 
									
									
		'max_of_contract_labour'				=> $max_of_contract_labour,
		'applicable_security_fess'				=> $applicable_security_fess,
		'payable_amendment_security_fees'		=> $payable_amendment_security_fees,
		'payable_amendment_fees'				=> $payable_amendment_fees,
		'ammend_deposit_fees'					=> $deposit_fees_value,
		'previous_highest_deposit_fees'			=> $previous_deposit_fees,
		
		
		
		'deposited_max_security_fees'			=> $previous_max_security,
		
		'previous_highest_security'				=> $previous_max_security,
		'is_coparative'							=> $is_coparative,
		'due_security_fees'						=> $due_security_fees_amount_license_renewal,
		'worksite_details_flag'					=> 1,
		'applicable_amendment_fee'				=> $applicable_amendment_fee
			
			
	);
	

	$ammendment_info = (array_merge($ammendment_info,$ammendment_info12)); 
	
	$ammendment_info11 =array(
			'license_renewal_ammendment_id'		=>  $renewallicenseammendment_id,
			'application_id'					=>	$application_id,
			'user_id'							=>  $pe_userid,
			'license_id'						=>  $license_id,
			'ammendment_status'					=>  'F',
			'ammendment_final_status'			=>  'F',
			'is_flag'                           =>  $applicationflag,
			'created_date'                      =>  date('Y-m-d'),
			'created_by'						=>  $contractor_id,
			'other_file_id'                     =>  $other_file_fid,
			'work_order_file_id'                =>  $work_order_file_fid,
			'worksite_address_line'				=>	$worksite_address_line,
			'worksite_dist'						=>	$worksite_dist, 
			'work_site_areatype'				=>	$work_site_areatype,
			'name_work_site_areatype'			=>	$name_work_site_areatype,
			'work_site_vill_ward'				=>  $work_site_vill_ward,
			'worksite_pin'						=>  $worksite_pin,
			'worksite_ps'						=>  $worksite_ps,
			'worksite_details_flag'				=>  1,
			'worksite_subdivision'				=>	$worksite_subdivision,
			'name_of_contractor'				=>  $name_of_contractor, 
			'modified_date_time'				=> date('Y-m-d H:i:s')
	);

	
	$ammendment_info = (array_merge($ammendment_info,$ammendment_info11)); 
	
	$amendment_check_field 	= $ammendment['amendment_check_field'];
	//print_r($amendment_check_field);die;
	$flag = "";
	if(empty($amendment_check_field)){
		$flag = "All_tab_successfully_update";
	}else{

		if(!empty($amendment_check_field)){
			$amendmen_field 	= explode(',', $amendment_check_field);
			//print_r($amendmen_field);
			if (in_array('name_of_contractor' || 'contractor_dist' || 'category_of_contractor' || 'date_of_birth_age' || 'father_name_of_contarctor' ||'contractor_max_no_of_labours_on_any_day'||'co_oparative' || 'category' || 'unskilled_rate_wages' || 'semiskilled_rate_wages' || 'skilled_rate_wages' ||  'highlyskilled_rate_wages' || 'date_of_birth_age' || 'hour_details' || 'spred_over' || 'overtime' || 'overtime_wages' ||  'annual_leave' || 'casual_leave' || 'eraned_leave' || 'sick_leave' || 'maternity_leave' || 'other_leave' || 'weekly_holiday' || 'special_benifites' || 'state_insurance' || 'miscellaneous' || 'offence' || 'pervious_license' || 'revoking' || 'address_of_manager',array_filter($amendmen_field))) {
				if($ammendment_status == 'I' && (empty($contract_labour_flag) || ($contract_labour_flag != 1))){
					$flag = "";	
				}else{
					$flag = "All_tab_successfully_update";
				}
			}

			foreach($amendmen_field as $row){
				if($row =='name_of_contractor' || $row == 'contractor_dist' ||  $row == 'category_of_contractor' ||  $row == 'date_of_birth_age' || $row == 'father_name_of_contarctor'){
					if($ammendment_status == 'I'){
						drupal_set_message(('Please update "Name & address of the contractor (Personal Information)".Then you can procces to finally submit your application'),'error');
					}else{	
						if(empty($personal_info_flag1) || ($personal_info_flag1 != 1)){
							
						    drupal_set_message(('Please update "Name & address of the contractor (Personal Information)".Then you can procces to finally submit your application'),'error');
							break;	
						}
					}
				}if($row =='contractor_max_no_of_labours_on_any_day' || $row =='co_oparative'){
					if($ammendment_status == 'I'){
						drupal_set_message(('Please update "Contractor Labour and Worksite Details".Then you can procces to finally submit your application'),'error');
					}else{
						if(empty($worksite_details_flag) || ($worksite_details_flag != 1)){
							drupal_set_message(('Please update "Contractor Labour and Worksite Details".Then you can procces to finally submit your application'),'error');
							break;		
						}
					}
				}
				if($row =='category' || $row == 'unskilled_rate_wages' || $row=='semiskilled_rate_wages' || $row=='skilled_rate_wages' || $row == 'highlyskilled_rate_wages'  || $row == 'date_of_birth_age' || $row == 'hour_details'  || $row == 'spred_over' || $row == 'overtime' || $row =='overtime_wages' || $row == 'annual_leave' || $row == 'casual_leave' || $row == 'eraned_leave' || $row == 'sick_leave' || $row == 'maternity_leave' || $row == 'other_leave' || $row == 'weekly_holiday' || $row=='special_benifites' || $row == 'state_insurance' || $row == 'miscellaneous' || $row == 'offence' || $row == 'pervious_license' || $row == 'revoking' || $row =='address_of_manager'){
					
					if($ammendment_status == 'I'){
						drupal_set_message(('Please update  Particular of Contract Labour".Then you can procces to finally submit your application'),'error');
					}else{
						if(empty($contract_labour_flag) || ($contract_labour_flag != 1)){
							drupal_set_message(('Please update  Particular of Contract Labour".Then you can procces to finally submit your application'),'error');
							break;
						}
					}
				}
			}
		}
	}
	
	
    if(trim($flag) == "All_tab_successfully_update"){
		
	    if(empty($details_ammendment_table['id'])|| $details_ammendment_table['id'] == NULL || $details_ammendment_table['id'] == ''){
			
			//db_insert('l_amendment_of_contract_labour_license')->fields($ammendment_info)->execute();
				$ammendment_id = return_ammendement_id($license_id);
				contractor_activity_tag_insert($serialno,'A',$ammendment_id,'F');
				$last_remark_activity = last_remark_activity($reference_no);
				if(!empty($last_remark_activity['remark_field_title'])){
					$remark_field_title 		=	$last_remark_activity['remark_field_title'];
				}else{
					$remark_field_title 		= $not_amendment_check_field;
				}
					$linces_remark = array(							
									'remark_by'			 		=>	$contractor_id,
									'remark_to'		 			=>  $serial ,
									'remark_type'	 			=>	'F',
									'remark_date'	 			=>	time(),
									'remark_by_role' 			=>	'8'	,	
									'remark_text'	 			=>  'Application has been fully submited',
									'particular_id'	 			=> 	$reference_no,
									'not_amendment_check_field'	=>	$not_amendment_check_field,
									'amendment_check_field' 	=>	$amendment_check_field,
									'remark_field_title'  		=>	$remark_field_title,
									'flag'						=>	'A'			
									);
									
			db_insert('l_remark_license')->fields($linces_remark)->execute();
				
		}else{
			

				$update_ammendment_table 	= 	db_update('l_amendment_of_contract_labour_license');
				$update_ammendment_table	->	fields($ammendment_info);
				$update_ammendment_table	->	condition('id',$details_ammendment_table['id']);
				$update_ammendment_table	->	execute();
				$ammendment_id 				= return_ammendement_id($license_id);
				
				contractor_activity_tag_insert($serialno,'A',$ammendment_id,'F');
				 $last_remark_activity = last_remark_activity($reference_no);
				
					if(!empty($last_remark_activity['remark_field_title'])){
						$remark_field_title 		=	$last_remark_activity['remark_field_title'];
					}else{
						$remark_field_title 		= $not_amendment_check_field;
					}
				
					$linces_remark = array(							
								'remark_by'			 		=>	$contractor_id,
								'remark_to'		 			=>  $serialno,
								'remark_type'	 			=>	'F',
								'remark_date'	 			=>	time(),
								'remark_by_role' 			=>	'8'	,	
								'remark_text'	 			=>  'Application has been fully submited',
								'particular_id'	 			=> 	$reference_no,
								'not_amendment_check_field'	=>	$not_amendment_check_field,
								'amendment_check_field' 	=>	$amendment_check_field,
								'remark_field_title'  		=>	$remark_field_title,
								'flag'						=>	'A'			
								);
							
			db_insert('l_remark_license')->fields($linces_remark)->execute();
				
		}
			
			 drupal_goto('amendment_license_renewal/amendment-perview/'.encryption_decryption_fun('encrypt', $serialno).'/'.encryption_decryption_fun('encrypt', $ammendment_id)) ;
			
	}
	 	
	
}


function pe_regmaster_details($serialno,$contractorid){
	$serial_no					=  encryption_decryption_fun('decrypt', $serialno);
	$contractor_id				=  encryption_decryption_fun('decrypt', $contractorid);
	$i = 0;
		$reg_no_principal_lists ='';
		$reg_no_principal 					= 	db_select('l_contrcator_previous_license', 'previous_license');
		$reg_no_principal					->	fields('previous_license',array('reg_no_principal_employeer'));
		$reg_no_principal					->	condition('previous_license.serial_no_form_v',$serial_no,'=');
		$reg_no_principal					->	condition('previous_license.created_by',$contractor_id,'=');
		$reg_no_principal					->	condition('previous_license.reg_no_principal_employeer', 'NULL', '!=');
		$reg_no_principal_no 				= 	$reg_no_principal->execute()->fetchAll();
		
	
	foreach($reg_no_principal_no as $row){ 
			$record = db_query("select name_of_contractor, serial_no_from_v, address_of_contractor, 
			   con_loc_e_dist, con_loc_e_subdivision, con_loc_e_borough, con_name_areatype, 

			   con_loc_e_areatype, con_loc_e_vill_ward, con_l_e_ps, contractor_pin, 
			   state_opts, state_others, id, e_name, loc_e_name, registration_number, 
			   act_id, loc_e_areatype, registration_date, loc_emp_areatype, 
			   full_name_principal_emp, address_principal_emp,user_id,contractor_max_no_of_labours_on_any_day,est_date_of_work_of_each_labour_from_date,est_date_of_work_of_each_labour_to_date
		  FROM contractor_registrationmaster_details where registration_number ='".trim($row->reg_no_principal_employeer)."'")->fetchAssoc();
		
		  $reg = $row->reg_no_principal_employeer.',';
		  $nature_work= '';
		
		  $result_work 						= nature_of_work_pe($record['id']); //---miscellaneous module
		
	
			
		  if(!empty($record['registration_date'])){
			  $registration_date_timestamp	 	= strtotime($record['registration_date']);
			  $formatted_reg_date 				=  date('m/d/Y', $registration_date_timestamp);
			   $reg_no_principal_lists ='<strong>Name:-</strong>'.$record['e_name'].'<br><strong>Reg No.:-</strong>'.$record['registration_number'].'<br><strong>Address:-</strong>'.$record['loc_e_name'].'<br>'.miscellaneous_establishment_address_form($record['loc_e_areatype'],$serial_no,$record['id'])."<br/>"."<strong>Date of Certificates:-</strong> ".$formatted_reg_date."\nUnder The CLRA  acts<br/><strong>Nature of work:-</strong>".$result_work;
		$i= $i+1;
		  
		 	$output	 .= '<div class="con-dtls-view-main">
					<h3>'.$i.'.Details of the principal employer, establishment and nature of work,where contractor has worked within the past five years.</h3>
					<div class="con-dtls-view-contains">
						<section class="odd">
							<label>'.$row->reg_no_principal_employeer.'</label><span>'.$reg_no_principal_lists.'</span>
						</section>
					
					</div>
			</div>';
		 
			}
			
	
		}
		
		$previous_license_number='';
		$previous_license 					= 	db_select('l_contrcator_previous_license', 'previous_license');
		$previous_license					->	fields('previous_license',array('perivious_license_no'));
		$previous_license					->	condition('previous_license.serial_no_form_v',$serial_no,'=');
		$previous_license					->	condition('previous_license.created_by',$contractor_id,'=');
		$previous_license					->	condition('previous_license.perivious_license_no', 'NULL', '!=');
	
		$previous_license_no 				= 	$previous_license->execute()->fetchAll();
		if(!empty($previous_license_no)){
				foreach($previous_license_no as $row){
					  $license = $row->perivious_license_no.',';
					
					$pe_id 		= 	db_select('l_contractor_license_application', 'license');
					$pe_id		->	fields('license',array('user_id'));
					$pe_id		->	condition('license.contractor_license_number',$row->perivious_license_no,'=');
					$get_pe_id 	= 	$pe_id->execute()->fetchAll();
					if(!empty($get_pe_id)){
						foreach($get_pe_id as $rows){	 
						$record = db_query("select name_of_contractor, serial_no_from_v, address_of_contractor, 
						   con_loc_e_dist, con_loc_e_subdivision, con_loc_e_borough, con_name_areatype, 
						   con_loc_e_areatype, con_loc_e_vill_ward, con_l_e_ps, contractor_pin, 
						   state_opts, state_others, id, e_name, loc_e_name, registration_number, 
						   act_id, loc_e_areatype, registration_date, loc_emp_areatype, 
						   full_name_principal_emp, address_principal_emp,user_id,contractor_max_no_of_labours_on_any_day,est_date_of_work_of_each_labour_from_date,est_date_of_work_of_each_labour_to_date
					  FROM contractor_registrationmaster_details where user_id ='".$rows->user_id."'")->fetchAssoc();
					  
					
					  $nature_work= '';
					
					  $result_work 						= nature_of_work_pe($record['id']); //---miscellaneous module
					
				
						
					  if(!empty($record['registration_date'])){
						  $registration_date_timestamp	 	= strtotime($record['registration_date']);
						  $formatted_reg_date 				=  date('m/d/Y', $registration_date_timestamp);
						   $reg_no_principal_lists ='<strong>Name:-</strong>'.$record['e_name'].'<br><strong>Reg No.:-</strong>'.$record['registration_number'].'<br><strong>Address:-</strong>'.$record['loc_e_name'].'<br>'.miscellaneous_establishment_address_form($record['loc_e_areatype'],$serial_no,$record['id'])."<br/>"."<strong>Date of Certificates:-</strong> ".$formatted_reg_date."\nUnder The CLRA acts <br/><strong>Nature of work:-</strong>".$result_work;
					$i= $i+1;
					  
						$output	 .= '<div class="con-dtls-view-main">
								<h3>'.$i.'.Details of the principal employer, establishment and nature of work,where contractor has worked within the past five years.</h3>
								<div class="con-dtls-view-contains">
									<section class="odd">
										<label>'.$row->perivious_license_no.'</label><span>'.$reg_no_principal_lists.'</span>
									</section>
								
								</div>
						</div>';
		 
			}
			
	
		
						}
			
			}
		}
	
		}
		
		if($output == ''){
		$output	 = '<div class="con-dtls-view-main">
					<h3>1.Information Given by Principal Employeer.</h3>
					<div class="con-dtls-view-contains">
						<section class="odd">
							<label>Details of the principal employer, establishment and nature of work,where contractor has worked within the past five years </label><span>Not Availble</span>
						</section>
						<section class="even">
							<label>Given Reg. No of PE</label><span>'.$reg.'</span>
						</section>
						<section class="odd">
							<label>Given License No. </label><span>'.$license.'</span>
						</section>
					
					</div>
			</div>';
	}
	
	
	return $output;
}
	
	


