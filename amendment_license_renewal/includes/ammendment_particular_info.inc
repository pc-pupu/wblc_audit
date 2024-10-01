<?php
 drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');


function ammendment_particular_info($form, &$form_state,$serialno='' ,$renewallicenseammendment_id= '',$applicationflag= ''){
	global $user;
	
	$contractor_id							 = 	$user->uid;
	$serial_no								 =  encryption_decryption_fun('decrypt', $serialno);
	$renewal_license_ammendment_id			 =  encryption_decryption_fun('decrypt', $renewallicenseammendment_id);
	$application_flag						 =  encryption_decryption_fun('decrypt', $applicationflag);
	
	/*$form['particular_info'] = array(
			'#title' 		=>	 t('<div class="content"><div class="sky-form" style="width:100%">'),
			'#type' 		=> 	'fieldset',
			'#attributes' 	=> 	array('class'=>array('')),	
			);*/
			
		$form['particular_info']['header']	= 	array(
					'#markup' 	=>	t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading"><center>Particular of Contract Labour</center></div>'),
		);																	
	//$check_ammendment_table = check_ammendment_table($renewal_license_ammendment_id);			
	//$row_count 						= $check_ammendment_table ->rowCount();
	$contractor_regmaster_details 	= contractor_regmaster_details($serial_no);
	if(!empty($contractor_regmaster_details)){
			$application_id_data		 = 	$contractor_regmaster_details['id'];
			$pe_userid_data				 = 	$contractor_regmaster_details['user_id'];
	}

	if($application_flag == 'L'){
		
		$particular_data = particular_data($serial_no);
		if(!empty($particular_data)){
			$name_of_agent_license_renewal 							= $particular_data['name_of_agent'];
			$address_of_manager_license_renewal						= $particular_data['address_of_manager']; 
			$contractor_manager_dist_license_renewal				= $particular_data['contractor_manager_dist']; 
			$contractor_manager_subdivision_license_renewal			= $particular_data['contractor_manager_subdivision']; 
			$contractor_manager_name_areatype_license_renewal	    = $particular_data['contractor_manager_name_areatype'];
			$contractor_manager_areatype_license_renewal	    	= $particular_data['contractor_manager_areatype'];
			$contractor_managerr_vill_ward_license_renewal	    	= $particular_data['contractor_managerr_vill_ward'];
			$contractor_manager_ps_license_renewal	    			= $particular_data['contractor_manager_ps'];
			$manager_pin_license_renewal	    					= $particular_data['manager_pin'];
			$manager_state_license_renewal	    					= $particular_data['manager_state'];
			$manager_country_license_renewal	    				= $particular_data['manager_country'];
			$category_designation_license_renewal					= $particular_data['category_designation'];
			$unskilled_rate_wages_license_renewal					= $particular_data['unskilled_rate_wages'];
			$semiskilled_rate_wages_license_renewal					= $particular_data['semiskilled_rate_wages']; 
			$skilled_rate_wages_license_renewal						= $particular_data['skilled_rate_wages']; 
			$highlyskilled_rate_wages_license_renewal				= $particular_data['highlyskilled_rate_wages']; 
			$overtime_wages_license_renewal							= $particular_data['overtime_wages']; 
			$weekly_holiday_license_renewal							= $particular_data['weekly_holiday']; 
			$no_holiday_license_renewal								= $particular_data['no_holiday']; 
			$hours_work_license_renewal								= $particular_data['hours_work']; 
			$holiday_wages_license_renewal							= $particular_data['holiday_wages']; 
			$annual_leave_no_license_renewal						= $particular_data['annual_leave_no']; 
			$casual_leave_no_license_renewal						= $particular_data['casual_leave_no']; 
			$sick_leave_no_license_renewal							= $particular_data['sick_leave_no'];
			$maternity_leave_no_license_renewal						= $particular_data['maternity_leave_no'];
			$other_leave_no_license_renewal							= $particular_data['other_leave_no'];
			$contractor_convicted_license_renewal					= $particular_data['contractor_convicted'];
			$details_contractor_convicted_license_renewal			= $particular_data['details_contractor_convicted'];
			$contractor_revoking_license_renewal					= $particular_data['contractor_revoking'];
			$details_contractor_revoking_license_renewal			= $particular_data['details_contractor_revoking'];
			$contractor_previous_employer_license_renewal			= $particular_data['contractor_previous_employer'];
			$details_previous_employer_license_renewal				= $particular_data['details_previous_employer'];
			$overtime_license_renewal								= $particular_data['overtime'];
		}
		
	}
	if($application_flag == 'R'){
		
		$particular_data 		= 	particular_data($serial_no);
		$renewal_data 			= 	renewal_data_last($renewal_license_ammendment_id);
		if(!empty($particular_data)){
			$name_of_agent_license_renewal 							= 	$particular_data['name_of_agent'];
			$address_of_manager_license_renewal						= 	$particular_data['address_of_manager']; 
			$contractor_manager_dist_license_renewal				= 	$particular_data['contractor_manager_dist']; 
			$contractor_manager_subdivision_license_renewal			= 	$particular_data['contractor_manager_subdivision']; 
			$contractor_manager_name_areatype_license_renewal	    =	$particular_data['contractor_manager_name_areatype'];
			$contractor_manager_areatype_license_renewal	    	= 	$particular_data['contractor_manager_areatype'];
			$contractor_managerr_vill_ward_license_renewal	    	= 	$particular_data['contractor_managerr_vill_ward'];
			$contractor_manager_ps_license_renewal	    			= 	$particular_data['contractor_manager_ps'];
			$manager_pin_license_renewal	    					=	$particular_data['manager_pin'];
				$manager_state_license_renewal	    				=	$particular_data['manager_state'];
			$manager_country_license_renewal	    				=	$particular_data['manager_country'];
			$category_designation_license_renewal					= $particular_data['category_designation'];
			$details_contractor_convicted_license_renewal			=   $particular_data['details_contractor_convicted'];
			
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
			
			
			$special_benifites_license_renewal						=	$renewal_data['special_benifites'];
			$state_insurance_license_renewal						= 	$renewal_data['state_insurance'];
			$miscellaneous_provisions_license_renewal				=	$renewal_data['miscellaneous_provisions'];
			$contractor_revoking_license_renewal					=	$renewal_data['details_contractor_revoking'];
			
			
			
				
		
			
			
		}
	}
			
					
	     //	$details_ammendment_table 				=  $check_ammendment_table->fetchAssoc();
	$details_ammendment_table 	=  check_ammendment_table_new($renewal_license_ammendment_id,$serial_no);
	// if($contractor_id==61518){
	// 	echo $renewal_license_ammendment_id.'--'.$serial_no;
	// 	echo "<pre>";
	// 	print_r($details_ammendment_table);die;
	// }
	
		
	$pe_userid		= !empty($details_ammendment_table['user_id'])?$details_ammendment_table['user_id']:$pe_userid_data;
	$application_id			 				= !empty($details_ammendment_table['application_id'])?$details_ammendment_table['application_id']:$application_id_data;
	$name_of_agent 							= !empty($details_ammendment_table['name_of_agent'])?$details_ammendment_table['name_of_agent']:$name_of_agent_license_renewal;
	$address_of_manager						= !empty($details_ammendment_table['address_of_manager'])?$details_ammendment_table['address_of_manager']:$address_of_manager_license_renewal; 
	$contractor_manager_dist				= !empty($details_ammendment_table['contractor_manager_dist'])?$details_ammendment_table['contractor_manager_dist']:$contractor_manager_dist_license_renewal; 
	$contractor_manager_subdivision			= !empty($details_ammendment_table['contractor_manager_subdivision'])?$details_ammendment_table['contractor_manager_subdivision']:$contractor_manager_subdivision_license_renewal; 
	$contractor_manager_name_areatype	    = !empty($details_ammendment_table['contractor_manager_name_areatype'])?$details_ammendment_table['contractor_manager_name_areatype']:$contractor_manager_name_areatype_license_renewal;
	$contractor_manager_areatype	    	= !empty($details_ammendment_table['contractor_manager_areatype'])?$details_ammendment_table['contractor_manager_areatype']:$contractor_manager_areatype_license_renewal;
	$contractor_managerr_vill_ward	    	= !empty($details_ammendment_table['contractor_managerr_vill_ward'])?$details_ammendment_table['contractor_managerr_vill_ward']:$contractor_managerr_vill_ward_license_renewal;
	$contractor_manager_ps	    			= !empty($details_ammendment_table['contractor_manager_ps'])?$details_ammendment_table['contractor_manager_ps']:$contractor_manager_ps_license_renewal;
	$contractor_manager_pin	    					= !empty($details_ammendment_table['contractor_manager_pin'])?$details_ammendment_table['contractor_manager_pin']:$manager_pin_license_renewal;
	$manager_state	    					= !empty($details_ammendment_table['manager_state'])?$details_ammendment_table['manager_state']:$manager_state_license_renewal;
	$manager_country	    				= !empty($details_ammendment_table['manager_country'])?$details_ammendment_table['manager_country']:$manager_country_license_renewal;
	$category_designation					= !empty($details_ammendment_table['category_designation'])?$details_ammendment_table['category_designation']:$category_designation_license_renewal;
	$unskilled_rate_wages					= !empty($details_ammendment_table['unskilled_rate_wages'])?$details_ammendment_table['unskilled_rate_wages']:$unskilled_rate_wages_license_renewal;
	$semiskilled_rate_wages					= !empty($details_ammendment_table['semiskilled_rate_wages'])?$details_ammendment_table['semiskilled_rate_wages']:$semiskilled_rate_wages_license_renewal; 
	$skilled_rate_wages						= !empty($details_ammendment_table['skilled_rate_wages'])?$details_ammendment_table['skilled_rate_wages']:$skilled_rate_wages_license_renewal; 
	$highlyskilled_rate_wages				= !empty($details_ammendment_table['highlyskilled_rate_wages'])?$details_ammendment_table['highlyskilled_rate_wages']:$highlyskilled_rate_wages_license_renewal; 
	$overtime_wages							= !empty($details_ammendment_table['overtime_wages'])?$details_ammendment_table['overtime_wages']:$overtime_wages_license_renewal; 
	$weekly_holiday							= !empty($details_ammendment_table['weekly_holiday'])?$details_ammendment_table['weekly_holiday']:$weekly_holiday_license_renewal; 
	$no_holiday								= !empty($details_ammendment_table['no_holiday'])?$details_ammendment_table['no_holiday']:$no_holiday_license_renewal; 
	$hours_work								= !empty($details_ammendment_table['hours_work'])?$details_ammendment_table['hours_work']:$hours_work_license_renewal; 
	$spred_over								= !empty($details_ammendment_table['spred_over'])?$details_ammendment_table['spred_over']:$spred_over_license_renewal; 
	$holiday_wages							= !empty($details_ammendment_table['holiday_wages'])?$details_ammendment_table['holiday_wages']:$holiday_wages_license_renewal; 
	$annual_leave_no						= !empty($details_ammendment_table['annual_leave_no'])?$details_ammendment_table['annual_leave_no']:$annual_leave_no_license_renewal; 
	$casual_leave_no						= !empty($details_ammendment_table['casual_leave_no'])?$details_ammendment_table['casual_leave_no']:$casual_leave_no_license_renewal; 
	$sick_leave_no							= !empty($details_ammendment_table['sick_leave_no'])?$details_ammendment_table['sick_leave_no']:$sick_leave_no_license_renewal;
	$maternity_leave_no						= !empty($details_ammendment_table['maternity_leave_no'])?$details_ammendment_table['maternity_leave_no']:$maternity_leave_no_license_renewal;
	$earned_leave_no						= !empty($details_ammendment_table['earned_leave_no'])?$details_ammendment_table['earned_leave_no']:$earned_leave_no_license_renewal;
	$other_leave_no							= !empty($details_ammendment_table['other_leave_no'])?$details_ammendment_table['other_leave_no']:$other_leave_no_license_renewal;
	$special_benifites						= !empty($details_ammendment_table['special_benifites'])?$details_ammendment_table['special_benifites']:$special_benifites_license_renewal;
	$state_insurance						= !empty($details_ammendment_table['state_insurance'])?$details_ammendment_table['state_insurance']:$state_insurance_license_renewal;
	$miscellaneous_provisions				= !empty($details_ammendment_table['miscellaneous_provisions'])?$details_ammendment_table['miscellaneous_provisions']:$miscellaneous_provisions_license_renewal;
	$contractor_convicted					= !empty($details_ammendment_table['contractor_convicted'])?$details_ammendment_table['contractor_convicted']:$contractor_convicted_license_renewal;
	
	$details_contractor_convicted			= !empty($details_ammendment_table['details_contractor_convicted'])?$details_ammendment_table['details_contractor_convicted']:$details_contractor_convicted_license_renewal;
	$contractor_revoking					= !empty($details_ammendment_table['contractor_revoking'])?$details_ammendment_table['contractor_revoking']:$contractor_revoking_license_renewal;
	//echo $details_contractor_convicted;die;
	$details_contractor_revoking			= !empty($details_ammendment_table['details_contractor_revoking'])?$details_ammendment_table['details_contractor_revoking']:$details_contractor_revoking_license_renewal;
	$contractor_previous_employer			= !empty($details_ammendment_table['contractor_previous_employer'])?$details_ammendment_table['contractor_previous_employer']:$contractor_previous_employer_license_renewal;
	$details_previous_employer				= !empty($details_ammendment_table['details_previous_employer'])?$details_ammendment_table['details_previous_employer']:$details_previous_employer_license_renewal;
	//$reference_no							= !empty($details_previous_employer['reference_no'])?$record['reference_no']:'';
	$overtime								= !empty($details_ammendment_table['overtime'])?$record['overtime']:$overtime_license_renewal;
	$ammendment_status					    = $details_ammendment_table['ammendment_status'];

	$reference_no							= $details_ammendment_table['reference_no'];
	$last_remark_activity 					= last_remark_activity($reference_no);
	//print_r($reference_no);die();
	$remark_field_title 					= $last_remark_activity['remark_field_title'];
	$remark_type 							= $last_remark_activity['remark_type'];
		
	//echo $remark_field_title.'--<br>'.$remark_field_title;die;
	// if((empty($remark_field_title) && $remark_type =='B')|| (!empty($remark_field_title) && $remark_type =='B')){
	// 	$readonly_agent						= array('placeholder' => 'Enter your name','class' => array('correction_show'));
	// 	$readonly_address_manager			= array('placeholder' => 'Enter your Father Name', 'class' => array('correction_show'));
	// 	$readonly_contract_manger_dist 		= array('class' => array('correction_show'));
	// 	$readonly_unskilled_rate_wages 		= array('class' => array('correction_show'));
	// 	$readonly_semiskilled_rate_wages 	= array('class' => array('correction_show'));
	// 	$readonly_skilled_rate_wages 		= array('class' => array('correction_show'));
	// 	$readonly_highlyskilled_rate_wages 	= array('class' => array('correction_show'));
	// 	$readonly_annual_leave_no 			= array('class' => array('correction_show'));
	// 	$readonly_casual_leave_no 			= array('class' => array('correction_show'));
	// 	$readonly_sick_leave_no 			= array('class' => array('correction_show'));
	// 	$readonly_maternity_leave_no 		= array('class' => array('correction_show'));
	// 	$readonly_eraned_leave_no 			= array('class' => array('correction_show'));
	// 	$readonly_hours_work 				= array('class' => array('correction_show'));
	// 	$readonly_spared_over 				= array('class' => array('correction_show'));
	// 	$readonly_overtime 					= array('class' => array('correction_show'));
	// 	$readonly_overtime_wages 			= array('class' => array('correction_show'));
	// 	$readonly_weekly_holiday 			= array('class' => array('correction_show'));
	// 	$readonly_other_leave_no			    = array('class' => array('correction_show'));
	// 	$readonly_holiday_wages				= array('class' => array('correction_show'));
	// 	$readonly_category					= array('class' => array('correction_show'));
	// 	$readonly_state_insurance			= array('class' => array('correction_show'));	
	// 	$readonly_miscellaneous			    = array('class' => array('correction_show'));
	// 	$readonly_offence			   		= array('class' => array('correction_show'));
	// 	$readonly_offence_details			= array('class' => array('correction_show'));
	// 	$readonly_special_benifites			= array('class' => array('correction_show'));
	// 	$readonly_pervious_license			= array('class' => array('correction_show'));				
	// }
	if((!empty($remark_field_title) && $remark_type =='B')){
		$readonly_agent						= array('placeholder' => 'Enter your name','class' => array('correction_show'));
		$readonly_address_manager			= array('placeholder' => 'Enter your Father Name', 'class' => array('correction_show'));
		$readonly_contract_manger_dist 		= array('class' => array('correction_show'));
		$readonly_unskilled_rate_wages 		= array('class' => array('correction_show'));
		$readonly_semiskilled_rate_wages 	= array('class' => array('correction_show'));
		$readonly_skilled_rate_wages 		= array('class' => array('correction_show'));
		$readonly_highlyskilled_rate_wages 	= array('class' => array('correction_show'));
		$readonly_annual_leave_no 			= array('class' => array('correction_show'));
		$readonly_casual_leave_no 			= array('class' => array('correction_show'));
		$readonly_sick_leave_no 			= array('class' => array('correction_show'));
		$readonly_maternity_leave_no 		= array('class' => array('correction_show'));
		$readonly_eraned_leave_no 			= array('class' => array('correction_show'));
		$readonly_hours_work 				= array('class' => array('correction_show'));
		$readonly_spared_over 				= array('class' => array('correction_show'));
		$readonly_overtime 					= array('class' => array('correction_show'));
		$readonly_overtime_wages 			= array('class' => array('correction_show'));
		$readonly_weekly_holiday 			= array('class' => array('correction_show'));
		$readonly_other_leave_no			= array('class' => array('correction_show'));
		$readonly_holiday_wages				= array('class' => array('correction_show'));
		$readonly_category					= array('class' => array('correction_show'));
		$readonly_state_insurance			= array('class' => array('correction_show'));	
		$readonly_miscellaneous			    = array('class' => array('correction_show'));
		$readonly_offence			   		= array('class' => array('correction_show'));
		$readonly_offence_details			= array('class' => array('correction_show'));	
		$readonly_special_benifites			= array('class' => array('correction_show'));
		$readonly_pervious_license			= array('class' => array('correction_show'));			
	}
	
	if(!empty($remark_field_title) && $remark_type =='I'){
		$readonly_agent						= '';
		$readonly_address_manager			= '';
		$readonly_contract_manger_dist 		= '';
		$readonly_unskilled_rate_wages 		= '';
		$readonly_semiskilled_rate_wages 	= '';
		$readonly_skilled_rate_wages 		= '';
		$readonly_highlyskilled_rate_wages 	= '';
		$readonly_annual_leave_no 			= '';
		$readonly_casual_leave_no 			= '';
		$readonly_sick_leave_no 				= '';
		$readonly_maternity_leave_no 		= '';
		$readonly_eraned_leave_no 			= '';
		$readonly_hours_work 				= '';
		$readonly_spared_over 				= '';
		$readonly_overtime 					= '';
		$readonly_overtime_wages 			= '';
		$readonly_weekly_holiday 			= '';
		$readonly_other_leave_no			    = '';
		$readonly_holiday_wages				= '';
		$readonly_category					= '';
		$readonly_state_insurance			= '';	
		$readonly_miscellaneous			    = '';
		$readonly_offence			   		= '';
		$readonly_offence_details			= ''; 
		$readonly_special_benifites			= '';	
		$readonly_pervious_license			=  '';		
	}//print_r($remark_field_title);die();
	
	if(empty($remark_field_title) && $remark_type =='B'){
		
		$remark_field_title = explode(',', $remark_field_title);	
		//  
		foreach($remark_field_title as $row){//echo  $row.'<br>';
			switch($row){
				case  "address_of_manager":
						$readonly_agent				    = array('placeholder' => 'Enter name of agent','class' => array('readonly_view'),'readonly' => 'readonly');				$readonly_contract_manger_dist 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_address_manager 		= array('placeholder' => 'Enter your Father Name', 'readonly' => 'readonly', 'class' => array('readonly_view'));
		
							break;
				case  "category":
					$readonly_category 		= array('placeholder' => 'Enter your Father Name', 'readonly' => 'readonly', 'class' => array('readonly_view'));
					break;
				case  "unskilled_rate_wages":
						$readonly_unskilled_rate_wages = array('placeholder' => 'Enter your Father Name', 'readonly' => 'readonly', 'class' => array('readonly_view'));
					break;
				case   "semiskilled_rate_wages":
						$readonly_semiskilled_rate_wages  = array('readonly' => 'readonly', 'class' => array('readonly_view'));
					
					break;
				case   "skilled_rate_wages":
						$readonly_skilled_rate_wages  = array('readonly' => 'readonly', 'class' => array('readonly_view'));
					
					break;
				case   "highlyskilled_rate_wages":
						$readonly_highlyskilled_rate_wages  = array('readonly' => 'readonly', 'class' => array('readonly_view'));
					
					break;
				case   "hour_details":
						$readonly_hours_work  = array('readonly' => 'readonly', 'class' => array('readonly_view'));
					
					break;
				case   "spred_over":
						$readonly_spared_over  = array('readonly' => 'readonly', 'class' => array('readonly_view'));
					
					break;
				case   "Overtime":
						$readonly_overtime  = array('readonly' => 'readonly', 'class' => array('readonly_view'));
					
					break;
				case   "overtime_wages":
						$readonly_overtime_wages  = array('readonly' => 'readonly', 'class' => array('readonly_view'));
					
					break;
				case   "annual_leave":
						$readonly_annual_leave_no  = array('readonly' => 'readonly', 'class' => array('readonly_view'));
					
					break;
				case   "casual_leave":
						$readonly_casual_leave_no  = array('readonly' => 'readonly', 'class' => array('readonly_view'));
					
					break;
				case   "eraned_leave":
						$readonly_eraned_leave_no  = array('readonly' => 'readonly', 'class' => array('readonly_view'));
					
					break;
				case   "sick_leave":
						$readonly_sick_leave_no  = array('readonly' => 'readonly', 'class' => array('readonly_view'));
					
					break;
				case   "other_leave":
						$readonly_other_leave_no  = array('readonly' => 'readonly', 'class' => array('readonly_view'));
					
					break;
				case   "maternity_leave":
						$readonly_maternity_leave_no  = array('readonly' => 'readonly', 'class' => array('readonly_view'));
					
					break;
			case   "weekly_holiday":
						$readonly_weekly_holiday  = array('readonly' => 'readonly', 'class' => array('readonly_view'));
					
					break;
			case   "special_benifites":
						$readonly_special_benifites  = array('readonly' => 'readonly', 'class' => array('readonly_view'));
					
					break;
			
			case   "miscellaneous":
						$readonly_miscellaneous  = array('readonly' => 'readonly', 'class' => array('readonly_view'));
					
					break;
			case   "offence":
						$readonly_offence 		 = array('readonly' => 'readonly', 'class' => array('readonly_view'));
						$readonly_offence_details  = array('readonly' => 'readonly', 'class' => array('readonly_view'));
					
					break;
			case   "revoking":
						$readonly_offence_details  = array('readonly' => 'readonly', 'class' => array('readonly_view'));
					break;
			case   "pervious_license":
						$readonly_pervious_license  = array('readonly' => 'readonly', 'class' => array('readonly_view'));
					break;
			case   "state_insurance":
						$readonly_state_insurance  = array('readonly' => 'readonly', 'class' => array('readonly_view'));
					
					break;
			}
		}
	}
	//print_r($readonly_state_insurance);die();
	if(!empty($no_holiday)){
		$rest_holiday = substr($no_holiday, 0, -1);
		$no_holiday_list= explode(',', $rest_holiday);
	}
	//print_r($readonly_hours_work);die;
	$form['particular_info']['renewallicenseammendment_id'] = array(
			'#type' 			=> 'hidden',
			'#default_value' 	=> !empty($renewallicenseammendment_id) ? $renewallicenseammendment_id:'',
			
			
												
	);	
	$form['particular_info']['applicationflag'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($applicationflag) ? $applicationflag:'',
				
				
													
	);	
	$form['particular_info']['serialno'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($serialno) ? $serialno:'',
				
				
													
	);				 
	$form['particular_info']['application_id'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($application_id) ?  encryption_decryption_fun('encrypt', $application_id):'',
				
				
													
	);	
	$form['particular_info']['pe_userid'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($pe_userid) ?  encryption_decryption_fun('encrypt', $pe_userid):'',
				
				
													
	);	
	$form['particular_info']['reference_no'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($reference_no) ?  encryption_decryption_fun('encrypt', $reference_no):'',
				
				
													
	);	
	if($application_flag == 'L'){
		$license_id						=	$renewal_license_ammendment_id;//---kaj baki
	}if($application_flag == 'A'){
		$license_id						=	return_ammendement_license_id($renewal_license_ammendment_id);//---kaj baki
	} if($application_flag == 'R'){
		$license_id						=	return_renewal_license($renewal_license_ammendment_id);//---kaj baki
	} 					
							
	$ammendment = 	check_ammendment_fields($license_id,$serial_no)->fetchAssoc();
	 $form['particular_info']['start_markup'] = array(
				'#prefix' 		=>  '<div class="panel-body"><div class="sky-form" style="width:100%;">',
				'#type' 		=> 'markup',
						
	);
	if(!empty($ammendment)){
		
		$amendment_check_field 		= $ammendment['amendment_check_field'];
		$not_amendment_check_field	= $ammendment['not_amendment_check_field']; 
		if(!empty($amendment_check_field)){
			$form['particular_info']['amendment_check_field'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($amendment_check_field) ?  encryption_decryption_fun('encrypt', $amendment_check_field):'',
														
			);	
			$editable_field 		= explode(',', $amendment_check_field);
			foreach($editable_field as $row){
				if($row == 'address_of_manager'){
					$form['particular_info']['contract_agnt_name'] = array(
						'#prefix' 					=>  '<div class="col-sm-12"><h4 class="form-sub-title">Details of manager/agent of contractor at the worksite</h4></div><div class="col col-md-4"><label class="input">',					
						'#suffix' 					=>  '</label></div>',
						'#title'					=>	'A.Name of agent/manager :',
						'#type' 					=> 'textfield',
						'#default_value' 			=>	!empty($name_of_agent) ? $name_of_agent : '',
						'#attributes' 				=> 	$readonly_agent
					);
									
					$options_country 	= array(''=>'Select','1' => 'India','2' => 'Others'); 
					$form['particular_info']['manager_country'] = array(
			
						'#prefix' 			=> '<section class="col col-md-4"><label class="select">',
						'#suffix' 			=> '</label></section>',
						'#title'			=> 'B.Select Country',
						'#type' 			=> 'select',
						'#options' 			=> $options_country,
						'#default_value'	=> (!empty($manager_country)) ? $manager_country : '',
						'#required' 		=> TRUE,
						'#attributes' 		=> $readonly_contract_manger_dist,
						'#ajax'				=> array(
														'event' 		=> 'change',
														'effect' 		=> 'fade',			
														'callback'		=> 'get_manager_country',
														'progress'		=>	'',
												)
					);	
					$value_country 	=  isset($form_state['values']['manager_country'])?$form_state['values']['manager_country']:$manager_country;						
					$form['particular_info']['contract_agnt_add'] = array(
						'#prefix' 					=>  '<div id="manager_add_replace"><section class="col col-md-4"><label class="input">',
						'#suffix' 					=>  '</label></section></div>',
						'#title'					=>	'C.Address Line1 of agent/manager:',
						'#type' 					=> 	'textfield',
						'#default_value' 			=>	!empty($address_of_manager) ? $address_of_manager : '',
						'#attributes' 				=> 	$readonly_address_manager,
						'#required' 				=> TRUE,
					);
					$form['particular_info']['manager_state'] = array(
		
						'#prefix' 			=> '<div id="manager_state_replace"><section class="col col-md-4"><label class="select">',
						'#suffix' 			=> '</label></section></div>',
						'#title'			=> 'D.Select State',
						'#type' 			=> 'select',
						'#options' 			=>  get_all_state($value_country),
						'#default_value'	=> (!empty($manager_state)) ? $manager_state : '',
						'#required' 		=> TRUE,
						'#attributes' 		=> $readonly_contract_manger_dist,
					
					);
				
					$value_state 	=  isset($form_state['values']['manager_state'])?$form_state['values']['manager_state']:$manager_state;	
					$options_dist 	=  get_district_by_id($value_state);
				
				
					$form['particular_info']['manager_dist'] 	= array(
						'#prefix' 			=> '<div id="manager_dist_replace"><section class="col col-md-4"><label class="select">',
						'#suffix' 			=> '</label></section></div>',
						'#title'				=> 'E.Select District',
						'#type' 				=> 'select',
						'#options' 			=> $options_dist,
						'#required' 			=> TRUE,
						'#attributes' 		=>  $readonly_contract_manger_dist,
						'#default_value' 		=> !empty($contractor_manager_dist) ? $contractor_manager_dist : '',
										
					); 
				
				
					$form['particular_info']['manager_subdivision'] 	= array(
						'#prefix' 			=> '<div id="manager_subdvsn_replace"><section class="col col-md-4"><label class="select">',
						'#suffix' 			=> '</label></section></div>',
						'#title'				=> 'F.Select Subdivision',
						'#type' 				=> 'select',
						'#options' 			=> get_subdivisionlist_contract_labour($value_manager_dist),
						'#required' 			=> TRUE,
						'#attributes' 		=>  $readonly_contract_manger_dist,
						'#default_value' 		=> !empty($contractor_manager_subdivision) ? $contractor_manager_subdivision : '',
						
					);
				
				
								
				
				    $form['particular_info']['manager_areatype'] = array(
						'#prefix' 			=> '<div id="manager_areatype_replace"><section class="col col-md-4"><label class="select">',
						'#suffix' 			=> '</label></section></div>',
						'#title'				=> 'G.Select Areatype',
						'#type' 				=> 'select',
						'#options' 			=> array(),
						'#attributes' 		=>  $readonly_contract_manger_dist,
						'#required' 			=> TRUE,
						'#default_value' 		=> !empty($contractor_manager_areatype) ? $contractor_manager_areatype : '',
					);
							

					$form['particular_info']['name_manager_areatype'] = array(
						'#prefix' 				=> '<div id="name_manager_areatype_replace"><section class="col col-md-4"><label class="select">',
						'#suffix' 				=> '</label></section></div>',
						'#title'				=> 'H.Select Block/Municipality/Corporation/SEZ/Notified Area',
						'#type' 				=> 'select',
						'#options' 			=> array(),
						'#attributes' 			=>  $readonly_contract_manger_dist,
						'#required' 			=> TRUE,
						
					);
					
					$form['particular_info']['manager_vill_ward'] = array(
						'#prefix' 				=> '<div id="manager_vill_ward_replace"><section class="col col-md-4"><label class="select">',
						'#suffix' 				=> '</label></section></div>',
						'#title'				=> 'I.Select Gram Panchayat/Ward',
						'#type' 				=> 'select',
						'#options' 			=> array(),
						'#attributes' 			=>  $readonly_contract_manger_dist,
						'#required' 			=> TRUE,											
							

					);
					$form['particular_info']['manager_ps'] = array(
						'#prefix' 				=> '<div id="manager_ps_replace"><section class="col col-md-4"><label class="select">',
						'#suffix' 				=> '</label></section></div>',
						'#title'				=> 'J.Select Police Station',
						'#type' 				=> 'select',
						'#options' 				=> array(),
						'#required' 			=> TRUE,
						'#attributes' 			=>  $readonly_contract_manger_dist,
							
					);
										
					$form['particular_info']['contractor_manager_pin'] = array(
						'#prefix' 			=> '<div id="manager_pin_replace"><section class="col col-md-4"><label class="input">',
						'#suffix' 			=> '</label></section></div>',
						'#title'				=>'K.PIN Code ',
						'#type' 				=> 'textfield',
						'#default_value' 		=> !empty($contractor_manager_pin) ? $contractor_manager_pin	:'',
						'#required' 			=> TRUE,
						'#element_validate'	=>  array('validate_integer') ,
						'#maxlength'			=>'6',
						'#attributes' 		=>  $readonly_address_manager,
					);
								
								
					if($value_country == 1){
						$form['particular_info']['contract_agnt_add'] = array(
							'#prefix' 					=>  '<div id="manager_add_replace"><section class="col col-md-4"><label class="input">',
							'#suffix' 					=>  '</label></section></div>',
							'#title'					=>	'C.Address Line1 of agent/manager:',
							'#type' 					=> 	'textfield',
							'#default_value' 			=>	!empty($address_of_manager) ? $address_of_manager : '',
							'#attributes' 				=> 	$readonly_address_manager,
							'#required' 				=> TRUE,
						);
						$form['particular_info']['manager_state'] = array(
						
							'#prefix' 			=> '<div id="manager_state_replace"><section class="col col-md-4"><label class="select">',
							'#suffix' 			=> '</label></section></div>',
							'#title'			=> 'D.Select State',
							'#type' 			=> 'select',
							'#options' 			=>  get_all_state($value_country),
							'#default_value'	=> (!empty($manager_state)) ? $manager_state : '',
							'#required' 		=> TRUE,
							'#attributes' 		=> $readonly_contract_manger_dist,
							'#ajax'				=> array(
														'event' 			=> 'change',
														'effect' 			=> 'fade',			
														'callback'			=> 'get_manager_country',
														'progress'			=> '',
												)
							
						);
						$value_state 		=  isset($form_state['values']['manager_state'])?$form_state['values']['manager_state']:$manager_state;	
						$options_dist 		=  get_district_by_id($value_state);
						if(!empty($value_state)){ 
							$db_district_value	= db_query("SELECT count(id) as dis_id FROM {district_master} WHERE state_id = '".$value_state."'")->fetchObject()->dis_id;
							if($db_district_value>0){
								$form['particular_info']['manager_dist'] 	= array(
									'#prefix' 			=> '<div id="manager_dist_replace"><section class="col col-md-4"><label class="select">',
									'#suffix' 			=> '</label></section></div>',
									'#title'			=> 'E.Select District',
									'#type' 			=> 'select',
									'#options' 			=> $options_dist,
									'#required' 		=> TRUE,
									'#attributes' 		=>  $readonly_contract_manger_dist,
									'#default_value' 	=> !empty($contractor_manager_dist) ? $contractor_manager_dist : '',
									'#ajax'				=> array(
																'event' 			=> 'change',
																'effect' 			=> 'fade',			
																'callback'			=> 'get_manager_address',
																'progress'			=> '',
															)
								); 
								$value_manager_dist = 	isset($form_state['values']['manager_dist'])?$form_state['values']['manager_dist']:$contractor_manager_dist;	
								$form['particular_info']['manager_subdivision'] 	= array(
										'#prefix' 			=> '<div id="manager_subdvsn_replace"><section class="col col-md-4"><label class="select">',
										'#suffix' 			=> '</label></section></div>',
										
										);
								$form['particular_info']['manager_areatype'] = array(
											'#prefix' 			=> '<div id="manager_areatype_replace"><section class="col col-md-4"><label class="select">',
											'#suffix' 			=> '</label></section></div>',
											
										);
								$form['particular_info']['name_manager_areatype'] = array(
											'#prefix' 				=> '<div id="name_manager_areatype_replace"><section class="col col-md-4"><label class="select">',
											'#suffix' 				=> '</label></section></div>',
									
									);
								$form['particular_info']['manager_vill_ward'] = array(
										'#prefix' 				=> '<div id="manager_vill_ward_replace"><section class="col col-md-4"><label class="select">',
										'#suffix' 				=> '</label></section></div>',
										);
											
														
								$form['particular_info']['manager_ps'] = array(
											'#prefix' 				=> '<div id="manager_ps_replace"><section class="col col-md-4"><label class="select">',
											'#suffix' 				=> '</label></section></div>',
											
									);
								$form['particular_info']['contractor_manager_pin'] = array(
											'#prefix' 			=> '<div id="manager_pin_replace"><section class="col col-md-4"><label class="input">',
											'#suffix' 			=> '</label></section></div>',
											
									);
																	
							}else{
								$value_manager_dist = "";
								$form['particular_info']['manager_dist'] 	= array(
													'#prefix' 			=> '<div id="manager_dist_replace"><section class="col col-md-4"><label class="select">',
													'#suffix' 			=> '</label></section></div>',
													
												); 
									
								$form['particular_info']['manager_subdivision'] 	= array(
										'#prefix' 			=> '<div id="manager_subdvsn_replace"><section class="col col-md-4"><label class="select">',
										'#suffix' 			=> '</label></section></div>',
										
										);
								$form['particular_info']['manager_areatype'] = array(
											'#prefix' 			=> '<div id="manager_areatype_replace"><section class="col col-md-4"><label class="select">',
											'#suffix' 			=> '</label></section></div>',
											
										);
								$form['particular_info']['name_manager_areatype'] = array(
											'#prefix' 				=> '<div id="name_manager_areatype_replace"><section class="col col-md-4"><label class="select">',
											'#suffix' 				=> '</label></section></div>',
									
									);
								$form['particular_info']['manager_vill_ward'] = array(
										'#prefix' 				=> '<div id="manager_vill_ward_replace"><section class="col col-md-4"><label class="select">',
										'#suffix' 				=> '</label></section></div>',
										);
											
														
								$form['particular_info']['manager_ps'] = array(
											'#prefix' 				=> '<div id="manager_ps_replace"><section class="col col-md-4"><label class="select">',
											'#suffix' 				=> '</label></section></div>',
											
									);
								$form['particular_info']['contractor_manager_pin'] = array(
											'#prefix' 			=> '<div id="manager_pin_replace"><section class="col col-md-4"><label class="input">',
											'#suffix' 			=> '</label></section></div>',
											
											);
												
									
							}
						
						}else{
							$value_manager_dist = "";
							$form['particular_info']['manager_dist'] 	= array(
									'#prefix' 			=> '<div id="manager_dist_replace"><section class="col col-md-4"><label class="select">',
									'#suffix' 			=> '</label></section></div>',
									
							); 
						
							$form['particular_info']['manager_subdivision'] 	= array(
									'#prefix' 			=> '<div id="manager_subdvsn_replace"><section class="col col-md-4"><label class="select">',
									'#suffix' 			=> '</label></section></div>',
									
									);
							$form['particular_info']['manager_areatype'] = array(
										'#prefix' 			=> '<div id="manager_areatype_replace"><section class="col col-md-4"><label class="select">',
										'#suffix' 			=> '</label></section></div>',
										
									);
							$form['particular_info']['name_manager_areatype'] = array(
										'#prefix' 				=> '<div id="name_manager_areatype_replace"><section class="col col-md-4"><label class="select">',
										'#suffix' 				=> '</label></section></div>',
								
								);
							$form['particular_info']['manager_vill_ward'] = array(
									'#prefix' 				=> '<div id="manager_vill_ward_replace"><section class="col col-md-4"><label class="select">',
									'#suffix' 				=> '</label></section></div>',
									);
										
													
							$form['particular_info']['manager_ps'] = array(
										'#prefix' 				=> '<div id="manager_ps_replace"><section class="col col-md-4"><label class="select">',
										'#suffix' 				=> '</label></section></div>',
										
								);
							$form['particular_info']['contractor_manager_pin'] = array(
										'#prefix' 			=> '<div id="manager_pin_replace"><section class="col col-md-4"><label class="input">',
										'#suffix' 			=> '</label></section></div>',
										
								);
						
						}
						if(!empty($value_manager_dist)){
							$db_subdivision_value = db_query("SELECT count(id) as sub_id  FROM {sub_division} WHERE district_code = '".$value_manager_dist."'")->fetchObject()->sub_id;
							if($db_subdivision_value>0){
								$form['particular_info']['manager_subdivision'] 	= array(
									'#prefix' 			=> '<div id="manager_subdvsn_replace"><section class="col col-md-4"><label class="select">',
									'#suffix' 			=> '</label></section></div>',
									'#title'				=> 'F.Select Subdivision',
									'#type' 				=> 'select',
									'#options' 			=> get_subdivisionlist_contract_labour($value_manager_dist),
									'#required' 			=> TRUE,
									'#attributes' 		=>  $readonly_contract_manger_dist,
									'#default_value' 		=> !empty($contractor_manager_subdivision) ? $contractor_manager_subdivision : '',
									'#ajax'				=> array(
																	'event' 			=> 'change',
																	'effect' 			=> 'fade',			
																	'callback'			=> 'get_manager_address',
																	'progress'			=> '',
																),
								);
								$value_manager_subdivision	= isset($form_state['values']['manager_subdivision']) ? $form_state['values']['manager_subdivision'] : $contractor_manager_subdivision;
							}else{		
								$form['particular_info']['manager_subdivision'] 	= array(
											'#prefix' 			=> '<div id="manager_subdvsn_replace"><section class="col col-md-4"><label class="select">',
											'#suffix' 			=> '</label></section></div>',
											);
								$value_manager_subdivision = '';										
							}
						}else{		
							$form['particular_info']['manager_subdivision'] 	= array(
											'#prefix' 			=> '<div id="manager_subdvsn_replace"><section class="col col-md-4"><label class="select">',
											'#suffix' 			=> '</label></section></div>',
											);
							$value_manager_subdivision = '';										
						}
								
						$areatypeOptions = get_areatype_contract_labour($value_manager_dist, $value_manager_subdivision);
						if(!empty($value_manager_subdivision) && !empty($areatypeOptions)){
							$form['particular_info']['manager_areatype'] = array(
								'#prefix' 			=> '<div id="manager_areatype_replace"><section class="col col-md-4"><label class="select">',
								'#suffix' 			=> '</label></section></div>',
								'#title'				=> 'G.Select Areatype',
								'#type' 				=> 'select',
								'#options' 			=> $areatypeOptions,
								'#attributes' 		=>  $readonly_contract_manger_dist,
								'#required' 			=> TRUE,
								'#default_value' 		=> !empty($contractor_manager_areatype) ? $contractor_manager_areatype : '',
								'#ajax'				=> array(
														'event' 			=> 'change',
														'effect' 			=> 'fade',			
														'callback'			=> 'get_manager_address',
														'progress'			=> '',
														),
								);
						}else{
							$form['particular_info']['manager_areatype'] = array(
									'#prefix' 			=> '<div id="manager_areatype_replace"><section class="col col-md-4"><label class="select">',
									'#suffix' 			=> '</label></section></div>',
								
								);
						}
						$value_manager_areatype = isset($form_state['values']['manager_areatype']) ? $form_state['values']['manager_areatype'] : $contractor_manager_areatype;
						if($value_manager_areatype == ""){
							$$form['particular_info']['name_manager_areatype'] = array(
								'#prefix' 				=> '<div id="name_manager_areatype_replace"><section class="col col-6"><label class="select">',
								'#suffix' 				=> '</label></section></div>',
							);
							
						}	
						if($value_manager_dist != "" && $value_manager_subdivision != ""){
							if($value_manager_areatype != ""){
								if($value_manager_areatype == 'B'){
									$form['particular_info']['name_manager_areatype'] = array(
										'#prefix' 			=>'<div id="name_manager_areatype_replace"><section class="col col-md-4"><label class="select">',
										'#suffix' 			=> '</label></section></div>',
										'#title'				=> 'H.Select Block',
										'#type' 				=> 'select',
										'#options' 			=> get_block_contract_labour($value_manager_subdivision, $value_manager_areatype),
										'#required' 			=> TRUE,
										'#attributes' 		=>  $readonly_contract_manger_dist,
										'#default_value' 		=> !empty($contractor_manager_name_areatype) ? $contractor_manager_name_areatype : '',
										'#ajax'				=> array(
																'event' 			=> 'change',
																'effect' 			=> 'fade',			
																'callback'			=> 'get_manager_address',
																'progress'			=> '',
																)
									);
								}if($value_manager_areatype == 'M'){
										$form['particular_info']['name_manager_areatype'] = array(
											'#prefix' 			=>'<div id="name_manager_areatype_replace"><section class="col col-md-4"><label class="select">',
											'#suffix' 			=> '</label></section></div>',
											'#title'				=> 'H.Select Municipality',
											'#type' 				=> 'select',
											'#options' 			=> get_block_contract_labour($value_manager_subdivision, $value_manager_areatype),
											'#required' 			=> TRUE,
											'#attributes' 		=>  $readonly_contract_manger_dist,
											'#default_value' 		=> !empty($contractor_manager_name_areatype) ? $contractor_manager_name_areatype : '',
											'#ajax'				=> array(
																	'event' 			=> 'change',
																	'effect' 			=> 'fade',			
																	'callback'			=> 'get_manager_address',
																	'progress'			=> '',
																	)
										);
								}if($value_manager_areatype == 'C'){
									$form['particular_info']['name_manager_areatype'] = array(
										'#prefix' 			=>'<div id="name_manager_areatype_replace"><section class="col col-md-4"><label class="select">',
										'#suffix' 			=> '</label></section></div>',
										'#title'				=> 'H.Select Corporation',
										'#type' 				=> 'select',
										'#options' 			=> get_block_contract_labour($value_manager_subdivision, $value_manager_areatype),
										'#required' 			=> TRUE,
										'#attributes' 		=>  $readonly_contract_manger_dist,
										'#default_value' 		=> !empty($contractor_manager_name_areatype) ? $contractor_manager_name_areatype : '',
										'#ajax'				=> array(
																'event' 			=> 'change',
																'effect' 			=> 'fade',			
																'callback'			=> 'get_manager_address',
																'progress'			=> '',
																)
									);
								}if($value_manager_areatype == 'S'){
									$form['particular_info']['name_manager_areatype'] = array(
										'#prefix' 			=>'<div id="name_manager_areatype_replace"><section class="col col-md-4"><label class="select">',
										'#suffix' 			=> '</label></section></div>',
										'#title'				=> 'H.Select SEZ',
										'#type' 				=> 'select',
										'#options' 			=> get_block_contract_labour($value_manager_subdivision, $value_manager_areatype),
										'#required' 			=> TRUE,
										'#attributes' 		=>  $readonly_contract_manger_dist,
										'#default_value' 		=> !empty($contractor_manager_name_areatype) ? $contractor_manager_name_areatype : '',
										'#ajax'				=> array(
																'event' 			=> 'change',
																'effect' 			=> 'fade',			
																'callback'			=> 'get_manager_address',
																'progress'			=> '',
																)
									);
								}if($value_manager_areatype == 'N'){
									$form['particular_info']['name_manager_areatype'] = array(
										'#prefix' 			=>'<div id="name_manager_areatype_replace"><section class="col col-md-4"><label class="select">',
										'#suffix' 			=> '</label></section></div>',
										'#title'				=> 'H.Select Notified Area',
										'#type' 				=> 'select',
										'#options' 			=> get_block_contract_labour($value_manager_subdivision, $value_manager_areatype),
										'#required' 			=> TRUE,
										'#attributes' 		=>  $readonly_contract_manger_dist,
										'#default_value' 		=> !empty($contractor_manager_name_areatype) ? $contractor_manager_name_areatype : '',
										'#ajax'				=> array(
																'event' 			=> 'change',
																'effect' 			=> 'fade',			
																'callback'			=> 'get_manager_address',
																'progress'			=> '',
																)
									);
								}
							}
						}
								
						$value_name_manager_areatype = isset($form_state['values']['name_manager_areatype']) ? $form_state['values']['name_manager_areatype'] : $contractor_manager_name_areatype;
						if($value_manager_dist != "" && $value_manager_subdivision != ""){
								if($value_manager_areatype != ""){
									if($value_name_manager_areatype != ""){
										if($value_manager_areatype == 'B'){
												
											$form['particular_info']['manager_vill_ward'] = array(
												'#prefix' 			=> '<div id="manager_vill_ward_replace"><section class="col col-md-4"><label class="select">',
												'#suffix' 			=> '</label></section></div>',
												'#title'				=> 'I.Select Gram Panchayat',
												'#type' 				=> 'select',
												'#options'			=> get_villward_contract_labour($value_name_manager_areatype,'V'),
												'#required' 			=> TRUE,
												'#attributes' 		=>  $readonly_contract_manger_dist,
												'#default_value' 		=> !empty($contractor_managerr_vill_ward) ? $contractor_managerr_vill_ward : '',
												'#ajax'				=> array(
																				'event' 			=> 'change',
																				'effect' 			=> 'fade',			
																				'callback'			=> 'get_manager_address',
																				'progress'			=> '',
																			)
											);	
										}if($value_manager_areatype == 'M'){
												
											$form['particular_info']['manager_vill_ward'] = array(
												'#prefix' 			=> '<div id="manager_vill_ward_replace"><section class="col col-md-4"><label class="select">',
												'#suffix' 			=> '</label></section></div>',
												'#title'				=> 'I.Select Ward',
												'#type' 				=> 'select',
												'#options' 			=> get_villward_contract_labour($value_name_manager_areatype,'W'),
												'#required' 			=> TRUE,
												'#attributes' 		=>  $readonly_contract_manger_dist,
												'#default_value' 		=> !empty($contractor_managerr_vill_ward) ? $contractor_managerr_vill_ward : '',
												'#ajax'				=> array(
																				'event' 			=> 'change',
																				'effect' 			=> 'fade',			
																				'callback'			=> 'get_manager_address',
																				'progress'			=> '',
																			)
											);
																												
										}
										if($value_manager_areatype == 'C'){
												
											$form['particular_info']['manager_vill_ward'] 	= array(
												'#prefix' 			=> '<div id="manager_vill_ward_replace"><section class="col col-md-4"><label class="select">',
												'#suffix' 			=> '</label></section></div>',

												'#title'			=> 'I.Select Ward',
												'#type' 			=> 'select',
												'#options' 			=> get_corporation_contract_labour($value_name_manager_areatype),
												'#required' 		=> TRUE,
												'#attributes' 		=>  $readonly_contract_manger_dist,
												'#default_value' 	=> !empty($contractor_managerr_vill_ward) ? $contractor_managerr_vill_ward : '',
												'#ajax'				=> array(
																				'event' 			=> 'change',
																				'effect' 			=> 'fade',			
																				'callback'			=> 'get_manager_address',
																				'progress'			=> '',
																			)
											);
										}if($value_manager_areatype == 'N'){
												
											$form['particular_info']['manager_vill_ward'] = array(
												'#prefix' 			=> '<div id="manager_vill_ward_replace"><section class="col col-md-4"><label class="select">',
												'#suffix' 			=> '</label></section></div>',
												'#title'			=> 'I.Select Notified Area',
												'#type' 			=> 'select',
												'#options' 			=> get_villward_contract_labour($value_name_manager_areatype,'N'),
												'#required' 		=> TRUE,
												'#attributes' 		=>  $readonly_contract_manger_dist,
												'#default_value' 	=> !empty($contractor_managerr_vill_ward) ? $contractor_managerr_vill_ward : '',
												'#ajax'				=> array(
																				'event' 			=> 'change',
																				'effect' 			=> 'fade',			
																				'callback'			=> 'get_manager_address',
																				'progress'			=> '',
																			)
											);
																												
										}if($value_manager_areatype == 'S'){
												
											$form['particular_info']['manager_vill_ward'] = array(
												'#prefix' 			=> '<div id="manager_vill_ward_replace"><section class="col col-md-4"><label class="select">',
												'#suffix' 			=> '</label></section></div>',
												'#title'				=> 'I.Select Sector',
												'#type' 				=> 'select',
												'#options' 			=> get_villward_contract_labour($value_name_manager_areatype,'S'),
												'#required' 			=> TRUE,
												'#attributes' 		=>  $readonly_contract_manger_dist,
												'#default_value' 		=> !empty($contractor_managerr_vill_ward) ? $contractor_managerr_vill_ward : '',
												'#ajax'				=> array(
																					'event' 			=> 'change',
																					'effect' 			=> 'fade',			
																					'callback'			=> 'get_manager_address',
																					'progress'			=> '',
																			)
												);
																												
										}
									}
								}
						}
						$value_manager_vill_ward = isset($form_state['values']['manager_vill_ward']) ? $form_state['values']['manager_vill_ward'] : $contractor_managerr_vill_ward;
						if($value_manager_dist != "" && $value_manager_subdivision != ""){
								if($value_manager_areatype != ""){
									if($value_name_manager_areatype != ""){
										if($value_manager_vill_ward != ""){
											$form['particular_info']['manager_ps'] = array(
												'#prefix' 			=> '<div id="manager_ps_replace"><section class="col col-md-4"><label class="select">',
												'#suffix' 			=> '</label></section></div>',
												'#title'				=> 'H.Select Police Station',
												'#type' 				=> 'select',
												'#default_value' 		=> !empty($contractor_manager_ps) ? $contractor_manager_ps : '',
												'#options' 			=> get_ps_contract_labour($value_manager_dist),
												'#required' 			=> TRUE,
												'#attributes' 		=>  $readonly_contract_manger_dist,
											);
													
											$form['particular_info']['contractor_manager_pin'] = array(
												'#prefix' 			=> '<div id="manager_pin_replace"><section class="col col-md-4"><label class="input">',
												'#suffix' 			=> '</label></section></div>',
												'#title'				=>'K.PIN Code ',
												'#type' 				=> 'textfield',
												
												'#default_value' 		=> !empty($contractor_manager_pin) ? $contractor_manager_pin	:'',
												'#required' 			=> TRUE,
												'#element_validate'	=>  array('validate_integer') ,
												'#maxlength'			=>'6',
												'#attributes' 		=>  $readonly_address_manager,
											);			  
										}
									}
								}
						}		
					
					}
					if($value_country == 2){
						$form['particular_info']['contract_agnt_add'] = array(
							'#prefix' 					=>  '<div id="manager_add_replace"><section class="col col-md-4"><label class="input">',
							'#suffix' 					=>  '</label></section></div>',
							'#title'					=>	'C.Address Line1 of agent/manager:',
							'#type' 					=> 	'textfield',
							'#default_value' 			=>	!empty($address_of_manager) ? $address_of_manager : '',
							'#attributes' 				=> 	$readonly_address_manager,
							'#required' 				=> TRUE,
						);
											
						$form['particular_info']['manager_state'] = array(
							'#prefix' 			=> '<div id="manager_state_replace"><section class="col col-md-4"><label class="select">',
							'#suffix' 			=> '</label></section></div>',
						);
						$value_manager_dist = "";
						$form['particular_info']['manager_dist'] 	= array(
							'#prefix' 			=> '<div id="manager_dist_replace"><section class="col col-md-4"><label class="select">',
							'#suffix' 			=> '</label></section></div>',
							
						); 
						
						$form['particular_info']['manager_subdivision'] 	= array(
							'#prefix' 			=> '<div id="manager_subdvsn_replace"><section class="col col-md-4"><label class="select">',
							'#suffix' 			=> '</label></section></div>',
								
						);
						$form['particular_info']['manager_areatype'] = array(
							'#prefix' 			=> '<div id="manager_areatype_replace"><section class="col col-md-4"><label class="select">',
							'#suffix' 			=> '</label></section></div>',
									
						);
						$form['particular_info']['name_manager_areatype'] = array(
							'#prefix' 				=> '<div id="name_manager_areatype_replace"><section class="col col-md-4"><label class="select">',
							'#suffix' 				=> '</label></section></div>',
							
						);
						$form['particular_info']['manager_vill_ward'] = array(
							'#prefix' 				=> '<div id="manager_vill_ward_replace"><section class="col col-md-4"><label class="select">',
							'#suffix' 				=> '</label></section></div>',
						);
									
												
						$form['particular_info']['manager_ps'] = array(
							'#prefix' 				=> '<div id="manager_ps_replace"><section class="col col-md-4"><label class="select">',
							'#suffix' 				=> '</label></section></div>',
									
						);
						$form['particular_info']['contractor_manager_pin'] = array(
							'#prefix' 			=> '<div id="manager_pin_replace"><section class="col col-md-4"><label class="input">',
							'#suffix' 			=> '</label></section></div>',
									
						);
										
					}

							
				}
				if($row == 'unskilled_rate_wages'||$row == 'semiskilled_rate_wages'||$row == 'skilled_rate_wages'||$row == 'highlyskilled_rate_wages'){
					$form['particular_info']['unskilled_rate_wages'] = array(
						'#prefix' 					=>  '<div class="col col-sm-12"><h4 class="form-sub-title">Details of manager/agent of contractor at the worksite</h4></div><section class="col col-md-6"><label class="input">',	
					
							'#suffix' 				=>  '</label></section>',
							'#title'				=> 	'A.Rate of Wages of Unskilled Category(including DA & Other & cash benifites)',
							'#type' 				=> 'textfield',
							'#default_value' 		=> !empty($unskilled_rate_wages) ? $unskilled_rate_wages : '0',
							
							'#attributes'			=> $readonly_unskilled_rate_wages	
																			
					);

														
					$form['particular_info']['semiskilled_rate_wages'] = array(
						'#prefix' 					=>  '<section class ="col col-md-6"><label class="input">',
						'#suffix' 				=>  '</label></section>',
						'#type' 					=> 'textfield',
						'#title'					=> 	'B.Rate of Wages of Semiskilled Category(including DA & Other & cash benifites)',
						'#default_value' 			=> !empty($semiskilled_rate_wages) ? $semiskilled_rate_wages : '0',
						'#attributes'				=> $readonly_semiskilled_rate_wages											  
					);
					$form['particular_info']['skilled_rate_wages'] = array(

						'#prefix' 					=>  '<section class ="col col-md-6"><label class="input">',
						'#suffix' 					=>  '</label></section>',
						'#type' 					=>  'textfield',
						'#title'					=> 	'C.Rate of Wages of Skilled Category(including DA & Other & cash benifites)',
						'#default_value' 			=>  !empty($skilled_rate_wages) ? $skilled_rate_wages : '0',
						
						'#attributes'				=> $readonly_skilled_rate_wages												  
					);						
											

					$form['particular_info']['highlyskilled_rate_wages'] = array(

									'#prefix' 			=>  '<section class ="col col-md-6"><label class="input">',
									'#suffix' 			=>  '</label></section>',
									'#type' 			=> 'textfield',
									'#title'			=> 	'D.Rate of Wages of Highlyskilled Category(including DA & Other & cash benifites)',
									'#default_value' 	=> !empty($highlyskilled_rate_wages) ? $highlyskilled_rate_wages : '0',
									
									'#attributes'		=> $readonly_highlyskilled_rate_wages											  
					);	

				}
				if($row == 'casual_leave'||$row == 'eraned_leave'||$row == 'sick_leave'||$row == 'maternity_leave'){
					$form['particular_info']['annual_leave_no'] 	= array(
						'#prefix' 			=>  '<div class="col col-sm-12"><h4 class="form-sub-title">Other Condition of service like leave (annual leave,casual leave,sick leave,maternity leave etc.) Holidays etc.of the contract labour</h4></div><section class ="col col-md-6"><label class="input">',
						'#suffix' 			=>  '</label></section>',
						'#type' 			=> 'textfield',
						'#title'			=> t('A.Number of Annual Leave(s) in day(s)'), 
						'#default_value' 	=> !empty($annual_leave_no) ? $annual_leave_no : '0',  
						'#attributes'		=>	$readonly_annual_leave_no	
					);

				
					$form['particular_info']['casual_leave_no'] 	= array(
						'#prefix' 			=>  '<section class ="col col-md-6"><label class="input">',
						'#suffix' 			=>  '</label></section>',
						'#type' 			=> 'textfield',
						'#title'			=> t('B.Number of Casual Leave(s) in day(s)'), 
						'#default_value' 	=> !empty($casual_leave_no) ? $casual_leave_no : '0',  
						'#attributes'		=> $readonly_casual_leave_no
					);

				
					$form['particular_info']['sick_leave_no'] 	= array(
						'#prefix' 			=>  '<section class ="col col-md-6"><label class="input">',
						'#suffix' 			=>  '</label></section>',
						'#type' 			=> 'textfield',
						'#title'			=> t('C.Number of Sick Leave(s)'), 
						'#default_value' 	=> !empty($sick_leave_no) ? $sick_leave_no : '0',
						'#attributes'		=> $readonly_sick_leave_no
											
					);

					$form['particular_info']['maternity_leave_no'] 	= array(
						'#prefix' 			=>  '<section class ="col col-md-6"><label class="input">',
						'#suffix' 			=>  '</label></section>',
						'#type' 			=> 'textfield',
						'#title'			=> t('D.Number of Mternity Leave(s) in day(s) '),
						'#default_value' 	=> !empty($maternity_leave_no) ? $maternity_leave_no : '0' ,
						'#attributes'		=> $readonly_maternity_leave_no
					);


					$form['particular_info']['other_leave_no'] 	= array(
						'#prefix' 			=>  '<section class ="col col-md-6"><label class="input">',
						'#suffix' 			=>  '</label></section>',
						'#type' 			=> 'textfield',
						'#title'			=> t('E.Number of other Leave(s) in day(s)'),
						'#default_value' 	=> !empty($other_leave_no) ? $other_leave_no : '0' ,
						'#attributes'		=> $readonly_other_leave_no
					);



					$form['particular_info']['earned_leave_no'] 	= array(
							'#prefix' 			=>  '<section class ="col col-md-6"><label class="input">',
							'#suffix' 			=>  '</label></section>',
							'#type' 			=> 'textfield',
							'#title'			=> t('F.Number of Earned Leave(s) in day(s)'),
							'#default_value' 	=> !empty($earned_leave_no) ? $earned_leave_no : '0' ,
							'#attributes'		=> $readonly_eraned_leave_no 
					);

				}
				if($row == 'hour_details'){
					$form['particular_info']['hours_work'] = array(
						'#prefix' 			=>  '<div class="col col-lg-12 col-sm-12"><h4 class="form-sub-title">Daliy Hours of work, overtime,overtime wages and spread over time</h4></div><section class ="col col-md-3"><label class="input">',
						'#suffix' 			=>  '</label></section>',
						'#type' 				=> 'textfield',
						'#title' 				=> 	t('A.Hours of Work'),
						'#default_value' 		=> !empty($hours_work) ? $hours_work : '0',
						
						'#required' 			=> TRUE,
						'#attributes'			=> $readonly_hours_work
					);						 
																							
					$form['particular_info']['spared_over'] = array(
						'#prefix' 			=>  '<section class ="col col-md-3"><label class="input">',
						'#suffix' 			=>  '</label></section>',
						'#type' 			=> 'textfield',
						'#title' 			=> 	t('B.Spread over hour(s)'), 
						'#default_value' 	=> !empty($spred_over) ? $spred_over : '0',
						
						'#required' 		=> TRUE,
						'#attributes'		=> $readonly_spared_over  		
					);

					$form['particular_info']['overtime'] = array(
						'#prefix' 			=>  '<section class ="col col-md-3"><label class="input">',
						'#suffix' 			=>  '</label></section>',
						'#type' 			=> 'textfield',
						'#title' 			=> 	t('C.Overtime hour(s)'), 
						'#default_value' 	=> !empty($overtime) ? $overtime : '0',
						
						'#required' 		=> TRUE,
						'#attributes'		=> $readonly_overtime      
																					
					);	
																								
					$form['particular_info']['overtime_wages'] = array(
						'#prefix' 				=>  '<section class ="col col-md-3"><label class="input">',
						'#suffix' 				=>  '</label></section>',
						'#type' 				=> 'textfield',
						'#title' 				=> 	t('D.Overtime Wages/per hour'), 
						'#default_value' 		=> !empty($overtime_wages) ? $overtime_wages : '0',
						'#required' 			=> TRUE,
						'#attributes'			=> $readonly_overtime_wages   
					);						


					$form['particular_info']['weekly_holiday'] = array(

						'#prefix' 			=>  '<section class ="col col-md-6"><label class="checkboxe">',
						'#suffix' 			=>  '</label>',
						'#type' 			=> 'checkbox',
						'#title' 			=> 	'E.Whether weekly holiday observed and on which day ',  
						'#default_value' 	=> !empty($weekly_holiday) ? $weekly_holiday : '0',    
						'#attributes'		=> array( 'id' => 'weekly-holiday-id', 'onclick' => 'weekly_holiday_fun()' ,'readonly'=>$readonly) ,
						
																								
					);
					$form['particular_info']['no_holiday'] = array( 
						'#prefix' 			=>  '<label class="select">',
						'#suffix' 			=>  '</label></section>',
						'#title'				=> 	'Select holiday(s):',
						'#type' 			    => 	'select',
						'#options' 			=> 	array(''=>t('Select'),'1'=>t('Sunday'),'2'=>t('Monday'),'3'=>t('Tuesday'),'4'=>t('Wednesday'),'5'=>t('Thursday'),'6'=>t('Friday'),'7'=>t('Saturday')),																					 			  '#attributes' 		=> 	array('id' => 'no-holiday-id','style'=>"width: 422px; height: 70px;" ),
						'#multiple' 			=> 	TRUE,
						'#default_value' 		=> 	!empty($no_holiday_list) ? $no_holiday_list : '',
						'#attributes'			=> 	$readonly_weekly_holiday   
					);


						
												
					$form['particular_info']['holidaywages'] = array(

						'#prefix' 				=>  '<section class ="col col-md-6"><label class="input">',
						'#suffix' 				=>  '</label></section>',
						'#type' 				=> 'textfield',
						'#title' 				=> 	t('F.holiday Wages/per hour'), 
						'#default_value' 		=> !empty($holiday_wages) ? $holiday_wages : '0',
						
						'#required' 			=> TRUE, 
						'#attributes'			=> $readonly_weekly_holiday         
																								
					);	
				}
				if($row == 'category'){
					$form['particular_info']['category_designation'] = array(
						'#prefix' 			=>  '<div class="col col-md-12"><h4 class="form-sub-title">Category / designation / nomenclature of the contractor labour, namely, fitter, welder, carpenter etc.</h4></div><section class ="col col-md-12"><label class="textarea">',
						'#suffix' 			=>  '</label></section>',
						'#title'			=> 'Category/designation/nomenclature of the contractor labour, namely,fitter,welder,carpenter,mazdor etc',
						'#rows'				=> 2,	
						'#type' 			=> 'textarea',
						'#default_value' 	=> !empty($category_designation) ? $category_designation : '',
						'#required' 		=> TRUE,
						'#attributes'		=>	$readonly_category
					);

				}
				if($row == 'special_benifites'){
					$form['particular_info']['special_benefits'] = array(
							'#prefix' 			=>  '<div class="col col-md-12"><h4 class="form-sub-title">Special benefits provided,if any</h4></div><section class ="col col-md-12"><label class="textarea">',
							'#suffix' 			=>  '</label></section>',
							'#title'			=> 'Special benefits provided',
							'#rows'				=> 2,	
							'#type' 			=> 'textarea',
							'#default_value' 	=> !empty($special_benifites) ? $special_benifites : '',
							'#required' 		=> TRUE,
							'#attributes'		=>	$readonly_special_benifites
					);

				}
				if($row == 'state_insurance'){
					$form['particular_info']['state_insurance'] = array(
							'#prefix' 			=>  '<div class="col col-md-12"><h4 class="form-sub-title">Contribution made under the Employees State Insurance Act,1948</h4></div><section class ="col col-md-12"><label class="textarea">',
							'#suffix' 			=>  '</label></section>',
							'#title'			=> 'Contribution made under the Employees State Insurance Act,1948',
							'#rows'				=> 2,	
							'#type' 			=> 'textarea',
							'#default_value' 	=> !empty($state_insurance) ? $state_insurance : '',
							'#required' 		=> TRUE,
							'#attributes'		=>	$readonly_state_insurance
					);

				}
				if($row == 'miscellaneous'){
					$form['particular_info']['miscellaneous'] = array(
							'#prefix' 			=> '<div class="col-md-12"><h4 class="form-sub-title">Contribution made under the Employees Provident Fund and Miscellaneous Provision Act,1952</h4></div><section class ="col col-md-12"><label class="textarea">',
							'#suffix' 			=> '</label></section>',
							'#title'		=> 'Contribution made under the Employees Provident Fund and Miscellaneous Provision Act,1952',
							'#rows'			=>	2,
							'#type'	 		=> 'textarea',
							'#default_value'=> !empty($miscellaneous_provisions) ? $miscellaneous_provisions : '',
							'#attributes' 	=> array('placeholder' => 'Maximum number of Contract Labour proposed .'),
							'#required' 	=> TRUE,
							'#attributes'	=>	$readonly_miscellaneous
					);
				}
					
				if($row == 'offence'){
					$form['particular_info']['contractor_convicted'] = array(
											
						'#prefix' 				=> '<div class="col col-md-12"><h4 class="form-sub-title">Whether the contractor was convicted of any offence within the preceeding five years. If so, give details</h4></div><section class="col col-md-2 ammend-row"><label class="inline-group">',
						'#suffix' 				=> '</label></section>',
										
						'#type' 			=> 'radios',
						'#options' 			=> array('0'=>t('No'),'1'=>t('Yes')),	
						'#default_value' 	=> !empty($contractor_convicted) ? $contractor_convicted : '0',
						'#attributes'		=> $readonly_offence,
						'#ajax'				=>  array(
														'event' 			=> 'change',
														'effect' 			=> 'fade',			
														'callback'			=> 'get_convicted',
														'progress'			=> '',
													) 

									
										
					);
					$value_contractor_convicted 		= 	isset($form_state['values']['contractor_convicted']) ? $form_state['values']['contractor_convicted'] :$contractor_convicted;
					if($value_contractor_convicted == 1){
										
						$form['particular_info']['details_contractor_convicted'] = array(
							'#prefix' 			=>  '<div id="add_convicted"><section  class="col col-md-10" ><label class="input">',
							'#suffix' 			=>  '</label></section></div>',
							'#title'			=>	'If yes please give details',
							'#type' 			=> 	'textfield',
							'#default_value' 	=>	!empty($details_contractor_convicted) ? $details_contractor_convicted : '',
							'#required' 		=> TRUE,
							'#attributes'		=> $readonly_offence,
								
						);
					}else{
						$form['particular_info']['details_contractor_convicted'] = array(
							'#prefix' 			=>  '<div id="add_convicted"><section  class="col col-md-10" ><label class="input">',
							'#suffix' 			=>  '</label></section></div>',
							'#title'			=>	'If yes please give details',
							'#type' 			=> 	'textfield',
							'#default_value' 	=>	!empty($details_contractor_convicted) ? $details_contractor_convicted : '',
							'#attributes'		=>	$readonly_offence
												
												
												
						);
					}
							
				}
				if($row == 'pervious_license'){
					$previous_license_number_list 			= get_previous_license($serial_no,$contractor_id);
					$reg_no_principal_employeer_number_list = get_previous_reg($serial_no,$contractor_id);
					
					$form['particular_info']['add_license_number'] = array(
						'#type' 			=> 'textarea',
						'#rows'				=> 2,
						'#title' 			=> t('Give your Previous License Number'),
						'#prefix' 			=>  '<div class ="col col-md-12"><h4 class="form-sub-title">Whether the contractor has worked in any other establishment within the past five years. If so give details of the principal employer, establishment</h4><div class="row"><section class ="col col-md-6"><label class="textarea">',
						'#suffix' 			=>  '</label></section>',
						'#default_value' 	=> !empty($previous_license_number_list) ? $previous_license_number_list : '',
						'#attributes'		=> 	$readonly_pervious_license,
						'#description'		=>'<font color="#FF0000"> If you have  multiple license numbers, provide your license numbers separated by comma(",")</font>'
										
					);
				
					$form['particular_info']['add_reg_no_pe'] = array(
						'#type' 			=> 'textarea',
						'#title' 			=> t('Give Registration Number of Principal Employer'),
						'#rows'				=> 2,
						'#prefix' 			=> '<div class ="col col-md-6"><label class="textarea">',
						'#suffix' 			=> '</label></div></div></div>',
						'#default_value' 	=>!empty($reg_no_principal_employeer_number_list) ? $reg_no_principal_employeer_number_list   : '',
						'#attributes'		=> 	$readonly_pervious_license,
						'#description'		=>'<font color="#FF0000"> If you have multiple Reg. No. of Principal Employer, provide Reg. No. separated by comma(",")</font>'
											
					);
				}if($row == 'revoking'){
								
					$form['particular_info']['contractor_revoking'] = array(
						'#prefix' 			=>  '<div class="col col-sm-12"><h4 class="form-sub-title">Whether there was any order against the contract or revoking or suspending license or forfeiting security deposit in respect of an earlier contract. If so, the date of such order</h4></div><section class="col col-lg-2 col-sm-2 ammend-row"><label class="inline-group">',
						'#suffix' 			=>  '</label></section>',
						'#type' 			=>  'radios',
						'#options' 			=>  array('0'=>t('No'),'1'=>t('Yes')),
						'#default_value' 	=> !empty($contractor_revoking) ? $contractor_revoking : 0,
						'#attributes'		=>  $readonly_revoking,
						'#ajax'				=>  array(
														'event' 			=> 'change',
														'effect' 			=> 'fade',			
														'callback'			=> 'get_revoking',
														'progress'			=> '',
													) 

					);
					$value_contractor_revoking	= 	isset($form_state['values']['contractor_revoking']) ? $form_state['values']['contractor_revoking'] :$contractor_revoking;
					if($value_contractor_revoking == 1){
						if(!empty($details_contractor_revoking)){
							$form['particular_info']['details_contractor_revoking'] = array(
								'#prefix' 			=>  '<div id="add_revoking"><section class="col col-lg-6"><label class="date select">',
								'#suffix' 			=>  '</label></section></div>',
								'#title'				=>	'The date of such order:',
								'#type' 				=>  'date',
								'#attributes'			=>	$readonly_contractor_revoking_date,
								'#default_value' 		=> array(
																	'month' 	=> format_date(strtotime($details_contractor_revoking), 'custom',  'n'),
																	'day' 		=> format_date(strtotime($details_contractor_revoking), 'custom',  'j'),
																	'year'      => format_date(strtotime($details_contractor_revoking), 'custom',  'Y'),
																),
									
							);
						}else{
							$form['particular_info']['details_contractor_revoking'] = array(
								'#prefix' 			=>  '<div id="add_revoking"><section class="col col-lg-6"><label class="date select">',
								'#suffix' 			=>  '</label></section></div>',
								'#title'				=>	'The date of such order:',
								'#type' 				=>  'date',
								'#attributes'			=>	$readonly_contractor_revoking_date,
								
									
							);
						}
					}else{
						$form['particular_info']['details_contractor_revoking'] = array(
							'#prefix' 			=>  '<div id="add_revoking"><section><label class="date">',
							'#suffix' 			=>  '</label></section></div>',
						);
					}
								
				}	
			}
		}
		$form['particular_info']['ammendment_status'] = array(
			'#type' 			=> 'hidden',
			'#default_value' 	=> !empty($ammendment_status) ?  $ammendment_status:'',
														
		);			
		
		$form ['particular_info']['submit'] = array (
			'#prefix' 		=> '<div class="col col-md-12">',
			'#attributes' 	=> array('class' => array('button left-btn')),
			'#type' 			=> 'submit',
			'#value'			=> 'Save'
		);
		$form['particular_info']['back_link']	 = array(
			'#suffix'			=> '</div></div></div>',
			'#markup' 			=> l(t('Back'),'amendment_license_renewal/apply_amendment/'. $serialno.'/'.$renewallicenseammendment_id.'/'. $applicationflag, array('html' => TRUE,'attributes'=> array('class'=>array('button')))),
		);		
		return $form;
		
    }	
}

function ammendment_particular_info_submit($form, &$form_state){
	global $base_root, $base_path, $user;
	$contractor_id 		= 	$user->uid;
	$particular_info = array();
	$val										=	$form_state['values'];
	// echo "<pre>";
	// print_r($val);
	// die;
	$id											= 	$val['id'];
	$status_from 								= 	$val['status_from'];
	$renewallicenseammendment_id				=	encryption_decryption_fun('decrypt', $val['renewallicenseammendment_id']);
	$applicationflag							=	encryption_decryption_fun('decrypt', $val['applicationflag']);
	$serialno									=	encryption_decryption_fun('decrypt', $val['serialno']);
	$application_id								=	encryption_decryption_fun('decrypt', $val['application_id']);
	$pe_userid									=	encryption_decryption_fun('decrypt', $val['pe_userid']);
	$amendment_check_field						=	encryption_decryption_fun('decrypt', $val['amendment_check_field']);	
	$ammendment_status							=	$val['ammendment_status'];

	// if($applicationflag == 'L'){
	// 	$license_id								=	$renewallicenseammendment_id;//---kaj baki
	// }if($applicationflag == 'A'){
	// 	$license_id								=	return_ammendement_license_id($renewallicenseammendment_id);//---kaj baki
	// }if($applicationflag == 'R'){
	// 	$license_id								=	return_renewal_license($renewallicenseammendment_id);//---kaj baki
	// }

	$getLicenseId = db_query("SELECT id FROM l_contractor_license_application  where serial_no_from_v ='".$serialno."' and contractor_user_id ='".$contractor_id."' and status='I' ")->fetchAssoc();	
	$license_id    = $getLicenseId['id'];
    
	$editable_field 							=   explode(',', $amendment_check_field);
	//print_r($editable_field);die();
	
	foreach($editable_field as $row){
		if($row == 'address_of_manager'){
			$contract_agnt_name   			= $val['contract_agnt_name'];
			$contract_agnt_add    			= $val['contract_agnt_add'];
			$manager_dist   	 			= $val['manager_dist'];
			$manager_subdivision  			= $val['manager_subdivision'];
			$manager_areatype  				= $val['manager_areatype'];
			$name_manager_areatype			= $val['name_manager_areatype'];
			$manager_vill_ward   			= $val['manager_vill_ward'];
			$manager_ps   					= $val['manager_ps'];
			$contractor_manager_pin   		= $val['contractor_manager_pin'];
			$manager_country				= $val['manager_country'];
			$manager_state					= $val['manager_state'];
			$particular_info1 = array(
					'name_of_agent' 					=>	$contract_agnt_name,
					'address_of_manager' 				=> 	$contract_agnt_add,
					'contractor_manager_dist' 			=>	$manager_dist,
					'contractor_manager_subdivision' 	=>	$manager_subdivision,
					'contractor_manager_name_areatype' 	=>	$name_manager_areatype,
					'contractor_manager_areatype' 		=>	$manager_areatype,
					'contractor_managerr_vill_ward' 	=>	$manager_vill_ward,
					'contractor_manager_ps' 			=>	$manager_ps,
					'contractor_manager_pin' 			=>	$contractor_manager_pin,
					'manager_state'						=>	$manager_state,
					'manager_country'					=> $manager_country
										
			);
			$particular_info = (array_merge($particular_info,$particular_info1));
		 }
		if($row == 'unskilled_rate_wages'||$row == 'semiskilled_rate_wages'||$row == 'skilled_rate_wages'||$row == 'highlyskilled_rate_wages'){
			$unskilled_rate_wages   			= $val['unskilled_rate_wages'];
			$semiskilled_rate_wages    			= $val['semiskilled_rate_wages'];
			$skilled_rate_wages   	 			= $val['skilled_rate_wages'];
			$highlyskilled_rate_wages  			= $val['highlyskilled_rate_wages'];
			
			$particular_info2=array( 
										'unskilled_rate_wages' 		=>	$unskilled_rate_wages,
										'semiskilled_rate_wages' 	=> 	$semiskilled_rate_wages,
										'skilled_rate_wages' 		=>	$skilled_rate_wages,
										'highlyskilled_rate_wages' 	=>	$highlyskilled_rate_wages,
				
			);
			
			$particular_info = (array_merge($particular_info,$particular_info2));
				
		}
		if($row == 'casual_leave'||$row == 'eraned_leave'||$row == 'sick_leave'||$row == 'maternity_leave'){
			$annual_leave_no   				= $val['annual_leave_no'];
			$casual_leave_no    			= $val['casual_leave_no'];
			$sick_leave_no   	 			= $val['sick_leave_no'];
			$maternity_leave_no  			= $val['maternity_leave_no'];
			$other_leave_no  				= $val['other_leave_no'];
			$earned_leave_no  				= $val['earned_leave_no'];
			
			$particular_info3	=array( 
				'annual_leave_no' 		=>	$annual_leave_no,
				'casual_leave_no' 		=> 	$casual_leave_no,
				'sick_leave_no' 		=>	$sick_leave_no,
				'maternity_leave_no' 	=>	$maternity_leave_no,
				'earned_leave_no'		=>  $earned_leave_no,
				'other_leave_no'		=>  $other_leave_no
				
			);
			
			$particular_info = (array_merge($particular_info,$particular_info3));
		}
		
		if($row == 'hour_details'){
			$no_holidays	 ='';
			$hours_work		 = $val['hours_work'];
			$spared_over 	 = $val['spared_over'];
			$overtime		 = $val['overtime'];
			$weekly_holiday	 = $val['weekly_holiday'];
			$overtime_wages  = $val['overtime_wages'];
			$no_holiday  	 = $val['no_holiday'];
			foreach($no_holiday as $row){
				$no_holidays .= $row.',';
			}
			
			
			$holidaywages  	 = $val['holidaywages'];
			
			$particular_info4	=array( 
				'hours_work' 		=>	$hours_work,
				'spred_over' 		=> 	$spared_over,
				'overtime' 			=>	$overtime,
				'weekly_holiday' 	=>	$weekly_holiday,
				'overtime_wages'	=>  $overtime_wages,
				'no_holiday'		=>  $no_holidays,
				'holiday_wages'		=>  $holidaywages
					
			);
				
			$particular_info = (array_merge($particular_info,$particular_info4));
		}
		if($row == 'category'){
			$category_designation  	 = $val['category_designation'];
				$particular_info5	=array( 
						'category_designation' 		=>	$category_designation
				);
				
			$particular_info = (array_merge($particular_info,$particular_info5));
		}if($row == 'special_benifites'){
			$special_benefits  	 = $val['special_benefits'];
			$particular_info6	=array( 
						'special_benifites' 		=>	$special_benefits
				);
				
			$particular_info = (array_merge($particular_info,$particular_info6));
			
		}if($row == 'miscellaneous'){
				$miscellaneous  	 = $val['miscellaneous'];
				
				$particular_info7	=array( 
						'miscellaneous_provisions' 		=>	$miscellaneous
				);
				
			$particular_info = (array_merge($particular_info,$particular_info7));
			
		}if($row == 'offence'){
				$contractor_convicted  	 = $val['contractor_convicted'];
				$details_contractor_convicted  	 = $val['details_contractor_convicted'];
				$particular_info8	=array( 
						'contractor_convicted' 				=>	$contractor_convicted,
						'details_contractor_convicted' 		=>	$details_contractor_convicted
				);
				
			$particular_info = (array_merge($particular_info,$particular_info8));
		}
		if($row == 'revoking'){
			$contractor_revoking		 = $val['contractor_revoking'] ;
			if($contractor_revoking=='0' || empty($contractor_revoking)|| $contractor_revoking =='')
			$details_contractor_revoking = NULL;
			else
				$details_contractor_revoking = $val['details_contractor_revoking']['year']."-".$val['details_contractor_revoking']['month']."-".$val['details_contractor_revoking']['day'];
			$particular_info12	=array( 
						'contractor_revoking' 				=>	$contractor_revoking,
						'details_contractor_revoking' 		=>	$details_contractor_revoking
				);
				
			$particular_info = (array_merge($particular_info,$particular_info12));
		}
		if($row == 'state_insurance'){
			$state_insurance = $val['state_insurance'] ;
			$particular_info10	=array( 
					'state_insurance' 		=>	$state_insurance
			);
				
			$particular_info = (array_merge($particular_info,$particular_info10));
						
		}if($row == 'pervious_license'){
			$license_number 			= $val['add_license_number'] ;
			$reg_no_pe 					= $val['add_reg_no_pe'] ;
			$add_license_lists			= explode(',', $license_number);
			foreach($add_license_lists as $row){ 
				if(!empty($row) || $row != '' || $row != NULL){
					$pervious_data  	= check_pevious_license_reg($row,$contractor_id);
					$pervious_license	=	$pervious_data['perivious_license_no'];
					if(empty($pervious_license)|| $pervious_license == NULL || $pervious_license == ''){
						$add_more_license = array( 
									'perivious_license_no' => $row,
									'created_by'		   => $contractor_id,
									'created_date'		   => date("Y-m-d"),
									'serial_no_form_v'	   =>  $serialno
									);														  
																				
						db_insert('l_contrcator_previous_license')->fields($add_more_license)->execute();
					}
				}
			}
					
				
			$add_reg_lists = explode(',', $reg_no_pe);
			
			
			foreach($add_reg_lists as $row){ 
				if(!empty($row) || $row != '' || $row!=NULL){
					$pervious_data  = check_pevious_license_reg($row,$contractor_id);
					$pervious_reg	=	$pervious_data['reg_no_principal_employeer'];
					if(empty($pervious_reg) || $pervious_reg == NULL || $pervious_reg ==''){
						$add_more_reg_pe_no = array( 
								'reg_no_principal_employeer'		=>	$row,
								'created_by'						=>	$contractor_id,
								'created_date'						=>	date("Y-m-d"),
								'serial_no_form_v'					=>	$serialno
							);
						
						db_insert('l_contrcator_previous_license')->fields($add_more_reg_pe_no)->execute();		
					}
				}
			}
			
						
		}
    }

    $particular_info11 =array(
		'license_renewal_ammendment_id'		=>  $renewallicenseammendment_id,
		'application_id'					=>	$application_id,
		'user_id'							=>  $pe_userid,
		'license_id'						=>  $license_id,
		'contract_labour_flag'				=>	1,
		'created_by'						=>  $contractor_id,
	);
	$particular_info = (array_merge($particular_info,$particular_info11));
	// echo $ammendment_status;
	// echo "<pre>";
	// print_r($particular_info);
	// die;
	if(trim($ammendment_status)=='I'){

		$amendment_rows_check = db_select('l_amendment_of_contract_labour_license', 'amd')->fields('amd',array('id'))->condition('license_id',$license_id)->execute();
		if($amendment_rows_check->rowCount() > 1){
			$applicant_mailto  = 'wblc.nic@gmail.com';
			$subject = 'Amendment Issue';
			$msg = 'Multiple row for amendment for License Id -'.$license_id.' Form-V-No -'.$serialno.'. Place- labour worksite info';
			if($applicant_mailto){
				send_mail_for_user_alert($applicant_mailto, $subject, $msg);
			}
			form_set_error('reference_no' ,t('We apologize. There is a problem. Please try again later.'));
		}else{
			$ammendment_id = return_ammendement_id($license_id);

			$get_data = db_select('l_amendment_of_contract_labour_license', 'amd')->fields('amd',array('payable_amendment_fees','payable_amendment_security_fees','reference_no','deposited_max_security_fees','previous_highest_security'))->condition('id',$ammendment_id)->condition('license_id',$license_id)->execute()->fetchAssoc();
		}
		
		// echo $ammendment_id;
		// echo "<pre>";
		// print_r($particular_info);
		//  die;
		if(!empty($ammendment_id) || $ammendment_id!=''){
			$query = db_query("SELECT reference_no FROM l_amendment_of_contract_labour_license_log where previous_amendment_id ='".$ammendment_id."' ");	
			$reference_no = $query->fetchField();
			// if(empty($reference_no) || $reference_no == ''){
					
			// }
			amemd_log($ammendment_id);

			$get_payment_data_query = db_select('l_principle_epayments_data', 'pay')->fields('pay',array('total_amount','identification_no','payment_status'))->condition('application_id',$get_data['reference_no'])->condition('act_id',14)->condition('payment_status','Success')->execute();
			if($get_payment_data_query->rowCount() > 0){
				$get_payment_data = $get_payment_data_query->fetchAssoc();

				$total_amount = $get_data['payable_amendment_fees']+$get_data['payable_amendment_security_fees'];
				if($get_payment_data['total_amount'] >= $total_amount){
					$payable_amendment_security_fees = 0;
					$payable_amendment_fees = 0;
				}else{
					
					if(($get_payment_data['total_amount'] - $get_data['payable_amendment_security_fees']) == $get_data['payable_amendment_fees']){
						$payable_amendment_fees = $get_data['payable_amendment_fees'];
						$payable_amendment_security_fees = 0;
					}else{
						$payable_amendment_fees = 0;
						$payable_amendment_security_fees = $total_amount-$get_payment_data['total_amount'];
					}
					
				}
				$deposited_max_security_fees = $get_data['deposited_max_security_fees']+$get_payment_data['total_amount'];
			}else{
				$get_payment_data_query = db_query("SELECT id,challanamount from l_principle_epayments_receive_data where transaction_id IN (
					select identification_no from l_principle_epayments_data where application_id =".$get_data['reference_no']." and act_id = 14)");

				if($get_payment_data_query->rowCount() > 0){
					$get_payment_data = $get_payment_data_query->fetchAssoc();
					
					$total_amount = $get_data['payable_amendment_fees']+$get_data['payable_amendment_security_fees'];

					if($get_payment_data['challanamount'] >= $total_amount){
						$payable_amendment_security_fees = 0;
						$payable_amendment_fees = 0;
					}else{
						
						if(($get_payment_data['challanamount'] - $get_data['payable_amendment_security_fees']) == $get_data['payable_amendment_fees']){
							$payable_amendment_fees = $get_data['payable_amendment_fees'];
							$payable_amendment_security_fees = 0;
						}else{
							$payable_amendment_fees = 0;
							$payable_amendment_security_fees = $total_amount-$get_payment_data['challanamount'];
						}
						
					}
					$deposited_max_security_fees = $get_data['deposited_max_security_fees']+$get_payment_data['challanamount'];

				}else{
					$payable_amendment_security_fees = $get_data['payable_amendment_security_fees'];
					$payable_amendment_fees = $get_data['payable_amendment_fees'];
					$deposited_max_security_fees = $get_data['deposited_max_security_fees'];
				}

				


			}

			//echo 123;die;

			$upadte_array	=	 array(
				'ammendment_status'			=>	'',
				'ammendment_final_status'	=>	'',
				'created_date'				=>	NULL,
				'alc_inspector_action_date'	=>	NULL,
				'remark_by_userid'			=>	NULL,
				'remark_by_userid'			=>	NULL,
				'ammendment_certificate_id'	=>  NULL,
				'payable_amendment_security_fees' => $payable_amendment_security_fees,
				'payable_amendment_fees'	=> $payable_amendment_fees,
				'deposited_max_security_fees'	=> $deposited_max_security_fees,
				'previous_highest_security'	=> $deposited_max_security_fees,
			);
			// echo "<pre>";
			// print_r($upadte_array);die;
			// echo 123;die;
			//echo $ammendment_id;die;
			$update_ammendment_table 	= 	db_update('l_amendment_of_contract_labour_license');
			$update_ammendment_table	->	fields($upadte_array);
			$update_ammendment_table	->	condition('id',$ammendment_id);
			$update_ammendment_table	->	execute();
		}
			
	}
	
		/*if($ammendment_status=='I'){
				amemd_log($amend_id);
				$upadte_array	=	 array(
							'reference_no' 				=> 	NULL,
							'ammendment_status'			=>	NULL,
							'ammendment_final_status'	=>	NULL
					);
					
				$update_ammendment_table 	= 	db_update('l_amendment_of_contract_labour_license');
				$update_ammendment_table	->	fields($upadte_array);
				$update_ammendment_table	->	condition('id',$amend_id);
				$update_ammendment_table	->	execute();
			}*/

	//$check_ammendment_table        	 =  check_ammendment_table($renewallicenseammendment_id);
    $details_ammendment_table        				 =  check_ammendment_table_new($renewallicenseammendment_id,$serialno);

    if(empty($details_ammendment_table['id'])|| $details_ammendment_table['id'] ==NULL || $details_ammendment_table['id'] == ''){
		

		db_insert('l_amendment_of_contract_labour_license')->fields($particular_info)->execute();
		$ammendment_id 	= return_ammendement_id($license_id);
		if($ammendment_status == 'B'){
			contractor_activity_tag_insert($serialno,'A',$ammendment_id,$ammendment_status);
		}else{
			contractor_activity_tag_insert($serialno,'A',$ammendment_id,NULL);
		}
		contractor_activity_tag_insert($serialno,'A',$ammendment_id,NULL);
	}else{
		//echo $license_id.'--'.$details_ammendment_table['id'];die;
		
		$update_ammendment_table 	= 	db_update('l_amendment_of_contract_labour_license');
 		$update_ammendment_table	->	fields($particular_info);
 		$update_ammendment_table	->	condition('license_id',$license_id);
		$update_ammendment_table	->	condition('id',$details_ammendment_table['id']);
		$update_ammendment_table	->	execute();
		$ammendment_id 	= return_ammendement_id($license_id);
		if($ammendment_status == 'B'){
			contractor_activity_tag_insert($serialno,'A',$ammendment_id,$ammendment_status);
		}else{
			contractor_activity_tag_insert($serialno,'A',$ammendment_id,NULL);
		}
		
	}

	$message = 3;
	   drupal_goto('amendment_license_renewal/apply_amendment/'.$val['serialno'].'/'.$val['renewallicenseammendment_id'].'/'.$val['applicationflag'].'#'.$message, array('fragment' => 'particular')) ;	
	
}
	
function get_district_contract_labour() {
		$default_select 	= array(""=>"- Select District -");
		$result 			= db_query("SELECT district_code, district_name FROM {district_master} where is_active = 'Y' order by district_name");
		$list 				= array();
		foreach ($result as $record) {
			$list[$record->district_code] = t('@districtname', array(
				'@districtname' => $record->district_name,
			));
		}
		return ($default_select+$list);
}
	
function get_subdivisionlist_contract_labour($value_dist=''){
	
	if(!empty($value_dist)){
		$default_subdvselect 	= array(""=>"- Select Sub-division -");
		$all_subdvs 			= array();
		$query_subdv 			= db_select('sub_division', 'subdv');
		$query_subdv			->fields('subdv',array('sub_div_code','sub_div_name'));
		$query_subdv			->orderBy('subdv.sub_div_name', 'ASC');
		$query_subdv			->condition('subdv.is_active','Y');
		$query_subdv			->condition('subdv.district_code',(int)$value_dist);
		$result_subdv 			= $query_subdv->execute(); 
		if(!empty($result_subdv)) {      
			foreach($result_subdv as $rows) {
				$all_subdvs[$rows->sub_div_code] = t('@subdvsname', array('@subdvsname' => $rows->sub_div_name));
			}    
		}
		
		if (isset($all_subdvs)) {			
			return ($default_subdvselect + $all_subdvs);	
		} else {				
			return $default_subdvselect;
		}
	
	}else{
		$default_subdvselect 	= array(""=>"- Select Sub-division -");
		return $default_subdvselect;
	}
	
}
function get_areatype_contract_labour($value_loc_e_dist, $value_loc_e_subdivision){
	if(!empty($value_loc_e_dist)&& !empty($value_loc_e_subdivision) ){
		$default_areatypeselect 	= array(""=>"- Select AreaType -");
		$all_areatype 				= array();
		$query_areatype 			= db_select('block_mun_master', 'bmm');
		$query_areatype				->fields('bmm',array('type'));
		$query_areatype				->condition('bmm.distict_code', trim($value_loc_e_dist));
		$query_areatype				->condition('bmm.sub_division_code', trim($value_loc_e_subdivision));
		$query_areatype				->groupBy('bmm.type');
		$result_areatype 			= $query_areatype->execute(); 
		if(!empty($result_areatype)) {      
			foreach($result_areatype as $rows) {
				if($rows->type=='B'){
					$all_areatype[$rows->type] = t('@areatype', array('@areatype' =>'Block'));
				}if($rows->type=='M'){
					$all_areatype[$rows->type] = t('@areatype', array('@areatype' =>'Municipality'));
				}if($rows->type=='C'){
					$all_areatype[$rows->type] = t('@areatype', array('@areatype' =>'Corporation'));
				}if($rows->type=='S'){
					$all_areatype[$rows->type] = t('@areatype', array('@areatype' =>'SEZ'));
				}if($rows->type=='N'){
					$all_areatype[$rows->type] = t('@areatype', array('@areatype' =>'Notified Area'));
				}
				
			}    
		}
		
		if (isset($all_areatype)) {			
			return ($default_areatypeselect + $all_areatype);	
		} else {				
			return $default_areatypeselect;
		}
	}else{
		$default_areatypeselect 	= array(""=>"- Select AreaType -");
		return $default_areatypeselect;
	}
}
function get_block_contract_labour($value_subdv, $value_areatype){
	if(!empty($value_subdv)&& !empty($value_areatype) ){
		$default_blockselect 	= array(""=>"- Select -");
		$all_blocks 			= array();
		$query_block 			= db_select('block_mun_master', 'blm');
		$query_block			->fields('blm',array('block_code','block_mun_name'));
		$query_block			->orderBy('blm.block_mun_name', 'ASC');
		$query_block			->condition('blm.sub_division_code',$value_subdv,'=');
		$query_block			->condition('blm.type',$value_areatype,'=');
		$result_block 			= $query_block->execute(); 
		if(!empty($result_block)) {      
			foreach($result_block as $rows) {
				$all_blocks[$rows->block_code] = t('@blockname', array('@blockname' => $rows->block_mun_name));
			}    
		}

		if (isset($all_blocks)) {
			return ($default_blockselect + $all_blocks);
		} else {
			return $default_blockselect;
		}
	}else{
		$default_blockselect 	= array(""=>"- Select -");
		return $default_blockselect;

	}
}

function get_corporation_contract_labour($code){
	if(!empty($code)){
		$default_corporationselect 		= array(""=>"- Select -");
		$all_corporation 				= array();
		$query_corporationname 			= db_select('village_ward_master', 'vwm');
		$query_corporationname			->fields('vwm',array('village_code', 'village_name'));
		$query_corporationname			->condition('vwm.block_code', $code, '=');
		$query_corporationname			->condition('vwm.category', 'C', '=');
		$query_corporationname			->orderBy('vwm.ord', 'ASC');
		$result_corporationname 		= $query_corporationname->execute(); 		
		if(!empty($result_corporationname)) {      
			foreach($result_corporationname as $rows) {
				$all_corporation[$rows->village_code] = t('@village_name', array('@village_name' => $rows->village_name));
			}    
		}
		if (isset($all_corporation)) {
			return ($default_corporationselect + $all_corporation);
		} else {
			return $default_corporationselect;
		}
	}else{
		$default_corporationselect 		= array(""=>"- Select -");
		return $default_corporationselect;
	}
}

function get_villward_contract_labour($value_villwared,$value_villwardtype){
	if(!empty($value_villwared) && !empty($value_villwardtype)){
		$default_villwardselect 	= array(""=>"- Select -");
		$all_villwards 				= array();
		$query_villwardkname 		= db_select('village_ward_master', 'vwm');
		$query_villwardkname		->fields('vwm',array('village_code','village_name'));
		$query_villwardkname		->orderBy('vwm.village_name', 'ASC');
		$query_villwardkname		->condition('vwm.block_code',$value_villwared,'=');
		$query_villwardkname		->condition('vwm.category',$value_villwardtype,'=');
		$result_villwardkname 		= $query_villwardkname->execute(); 
		if(!empty($result_villwardkname)) {      
			foreach($result_villwardkname as $rows) {
				if($value_villwardtype == 'W')
					$all_villwards[$rows->village_code] = t('@villwardkname', array('@villwardkname' => 'Ward.'.$rows->village_name));	
				else				
					$all_villwards[$rows->village_code] = t('@villwardkname', array('@villwardkname' => ucwords($rows->village_name)));
			}    
		}

		if (isset($all_villwards)) {
			return ($default_villwardselect + $all_villwards);
		} else {
			return $default_villwardselect;
		}
	}else{
		$default_villwardselect 	= array(""=>"- Select -");
		return $default_villwardselect;
	}
}

function get_ps_contract_labour($value_dist){
	if(!empty($value_dist)){
		$default_pselect 	= array(""=>"- Select -");
		$all_ps 			= array();
		$query_psname 		= db_select('police_station', 'ps');
		$query_psname		->fields('ps',array('police_station_code','name_of_police_station'));
		$query_psname		->orderBy('ps.name_of_police_station', 'ASC');
		$query_psname		->condition('ps.dristrict_code',$value_dist,'=');
		$result_psname 		= $query_psname->execute(); 
		if(!empty($result_psname)) {      
			foreach($result_psname as $rows) {
				$default_pselect[$rows->police_station_code] = t('@psname', array('@psname' => $rows->name_of_police_station));
			}    
		}
		if (isset($all_ps)) {
			return ($default_pselect + $all_ps);
		} else {
			return $default_pselect;
		}
	}else{
		$default_pselect 	= array(""=>"- Select -");
		return $default_pselect;
	}
}


function get_manager_address($form, $form_state){
	$commands 	= array();
	
	$commands[] = ajax_command_replace('#manager_subdvsn_replace', drupal_render($form['particular_info']['manager_subdivision']));
	$commands[] = ajax_command_replace('#manager_areatype_replace', drupal_render($form['particular_info']['manager_areatype']));
	$commands[] = ajax_command_replace('#name_manager_areatype_replace', drupal_render($form['particular_info']['name_manager_areatype']));
	$commands[] = ajax_command_replace('#manager_vill_ward_replace', drupal_render($form['particular_info']['manager_vill_ward']));
	
	$commands[] = ajax_command_replace('#manager_ps_replace', drupal_render($form['particular_info']['manager_ps']));
	$commands[] = ajax_command_replace('#manager_pin_replace', drupal_render($form['particular_info']['contractor_manager_pin']));
	
  	return array('#type' => 'ajax', '#commands' => $commands);
}

function get_convicted($form, $form_state) {
		$commands 		= array();
		$commands[] 	= ajax_command_replace('#add_convicted', drupal_render($form['particular_info']['details_contractor_convicted']));
		return array('#type' => 'ajax', '#commands' => $commands);
}
function get_revoking($form, $form_state) {
		$commands 		= array();
		$commands[] 	= ajax_command_replace('#add_revoking', drupal_render($form['particular_info']['details_contractor_revoking']));
		return array('#type' => 'ajax', '#commands' => $commands);
}


function get_manager_country($form, $form_state){
	$commands 	= array();
	$commands[] = ajax_command_replace('#manager_add_replace', drupal_render($form['particular_info']['contract_agnt_add']));
	$commands[] = ajax_command_replace('#manager_state_replace', drupal_render($form['particular_info']['manager_state']));
	$commands[] = ajax_command_replace('#manager_dist_replace', drupal_render($form['particular_info']['manager_dist']));
	$commands[] = ajax_command_replace('#manager_subdvsn_replace', drupal_render($form['particular_info']['manager_subdivision']));
	$commands[] = ajax_command_replace('#manager_areatype_replace', drupal_render($form['particular_info']['manager_areatype']));
	$commands[] = ajax_command_replace('#name_manager_areatype_replace', drupal_render($form['particular_info']['name_manager_areatype']));
	$commands[] = ajax_command_replace('#manager_vill_ward_replace', drupal_render($form['particular_info']['manager_vill_ward']));
	
	$commands[] = ajax_command_replace('#manager_ps_replace', drupal_render($form['particular_info']['manager_ps']));
	$commands[] = ajax_command_replace('#manager_pin_replace', drupal_render($form['particular_info']['contractor_manager_pin']));
	
  	return array('#type' => 'ajax', '#commands' => $commands);
}