<?php
 drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
drupal_add_js(drupal_get_path('module', 'contractor') . '/js/contractor.js');
function renewal_particular_data($form, &$form_state, $serial_number){
	drupal_add_library('system','ui.datepicker');
	global $base_root, $base_path, $user;
	
	$contractor_id 		= 	$user->uid;
	if($contractor_id!=0){
	$flag='1';
	$action = 'decrypt';
	$serial = encryption_decryption_fun($action, $serial_number);

	$query = db_select('l_contractor_info_master', 'con_info_master');
 
  	$query->fields('con_info_master',array('id','user_id','act_id','est_date_of_work_of_each_labour_from_date','est_date_of_work_of_each_labour_to_date','contractor_max_no_of_labours_on_any_day','application_id'));
  	$query->condition('con_info_master.id ',$serial,'=');
   	$result = $query->execute(); 
 	$startend_date="";	
	if( $result->rowCount() > 0 ){
		foreach($result as $obj){			
			$startdate			= $obj->est_date_of_work_of_each_labour_from_date;
			$enddate			= $obj->est_date_of_work_of_each_labour_to_date; 
			$max_labour 		= $obj->contractor_max_no_of_labours_on_any_day;
			$application_id 	= $obj->application_id;
			$applicant_id 		= $obj->user_id;
			$con_info_id 		= $obj->id;
			$date1 				= strtotime($startdate);
			$date2 				= strtotime($enddate);
			$months 			= 0;
			$year 				= 0;
			while (($date1 = strtotime('+1 MONTH', $date1)) <= $date2){
    			$months++;
			if($months >= 12){ $year = abs($months/12);}
				if($months/12 != 0){ $months = $months - ($year*12);}
					if($year > 0){
						if($months > 0)
							$estimated_date=$year.'Year(s) '.$months.'Month(s)';
						else
							$estimated_date=$year.'Year(s)';
					}else
						$estimated_date=$months;
		}
	}
	}

	$start_date=date('dS F y', strtotime($startdate));
	$end_date=date('dS F y', strtotime($enddate));
	$startend_date=$start_date.' To '.$end_date;
	
	$previous_license_number='';
	
	$previous_license 					= 	db_select('l_contrcator_previous_license', 'previous_license');
  	$previous_license					->	fields('previous_license',array('perivious_license_no'));
  	$previous_license					->	condition('previous_license.serial_no_form_v',$serial,'=');
	$previous_license					->	condition('previous_license.created_by',$contractor_id,'=');
	$previous_license					->	condition('previous_license.perivious_license_no', 'NULL', '!=');

	$previous_license_no 				= 	$previous_license->execute()->fetchAll();
	
	
	if(!empty($previous_license_no)){
		foreach($previous_license_no as $row){
			$previous_license_number.=$row->perivious_license_no.',';
		}
		$previous_license_number_list = substr($previous_license_number, 0, -1);
	}
	
	
	
	
	$reg_no_principal_employeer_number='';
	
	$reg_no_principal 					= 	db_select('l_contrcator_previous_license', 'previous_license');
  	$reg_no_principal					->	fields('previous_license',array('reg_no_principal_employeer'));
  	$reg_no_principal					->	condition('previous_license.serial_no_form_v',$serial,'=');
	$reg_no_principal					->	condition('previous_license.created_by',$contractor_id,'=');
	$reg_no_principal					->	condition('previous_license.reg_no_principal_employeer', 'NULL', '!=');

	$reg_no_principal_no 				= 	$reg_no_principal->execute()->fetchAll();

	
	if(!empty($reg_no_principal_no)){
		foreach($reg_no_principal_no as $row){
			$reg_no_principal_employeer_number.=$row->reg_no_principal_employeer.',';
		}
		$reg_no_principal_employeer_number_list = substr($reg_no_principal_employeer_number, 0, -1);
	}
	
	
	$result_particular_labour 	= 	fetch_all_data_particular_of_contract_labour($serial,$contractor_id); 
	
	if(!empty($result_particular_labour)){ 
			$rowcount	=	1;
		
			$name_of_agent							=	$result_particular_labour['name_of_agent'];
			$address_of_manager						=	$result_particular_labour['address_of_manager']; 
			$contractor_convicted					=	$result_particular_labour['contractor_convicted'];
			$details_contractor_convicted			=	$result_particular_labour['details_contractor_convicted'];
			$contractor_revoking					=	$result_particular_labour['contractor_revoking'];
			$details_contractor_revoking			=	$result_particular_labour['details_contractor_revoking'];
			$contractor_previous_employer			=	$result_particular_labour['contractor_previous_employer'];
			$details_previous_employer				=	$result_particular_labour['details_previous_employer'];
			$amount_of_licence						=	$result_particular_labour['amount_of_licence'];
			$security_deposit						=	$result_particular_labour['amount_of_security_deposit'];
			$contractor_dist						=	$result_particular_labour['contractor_manager_dist'];
			$contractor_sub_division				=	$result_particular_labour['contractor_manager_subdivision'];
			$contractor_area_type 					=	$result_particular_labour['contractor_manager_areatype'];
			$contractorname_area_type				=	$result_particular_labour['contractor_manager_name_areatype'];
			$contractor_villward					=	$result_particular_labour['contractor_managerr_vill_ward'];
			$contractor_borough_name				=	$result_particular_labour['contarctor_manager_borough'];
			$contractor_police_station				=	$result_particular_labour['contractor_manager_ps'];
			$manager_pin							=	$result_particular_labour['manager_pin'];
			
			$contract_labour_id						=	$result_particular_labour['id'];
			$max_labour_particular					=	$result_particular_labour['contractor_max_no_of_labours_on_any_day'];
			
			$category_designation					=	$result_particular_labour['category_designation'];
			
			$unskilled_wages						=	$result_particular_labour['unskilled_wages'];
			$unskilled_rate_wages					=	$result_particular_labour['unskilled_rate_wages'];
		
		  	$semiskilled_wages						=	$result_particular_labour['semiskilled_wages'];
			$semiskilled_rate_wages					=	$result_particular_labour['semiskilled_rate_wages'];
			
			
			$skilled_wages							=	$result_particular_labour['skilled_wages'];
			$skilled_rate_wages						=	$result_particular_labour['skilled_rate_wages'];
			
			
			$highlyskilled_wages					=	$result_particular_labour['highlyskilled_wages'];
			$highlyskilled_rate_wages				=	$result_particular_labour['highlyskilled_rate_wages'];
			
			
		  	$hours_work								= 	$result_particular_labour['hours_work'];
		  	$overtime								= 	$result_particular_labour['overtime'];
		  	$overtime_wages							= 	$result_particular_labour['overtime_wages'];
		  	$weekly_holiday							= 	$result_particular_labour['weekly_holiday'];
		  	$no_holiday								= 	$result_particular_labour['no_holiday'];
	  
		  	$annual_leave							= 	$result_particular_labour['annual_leave'];
		  	$annual_leave_no						= 	$result_particular_labour['annual_leave_no'];
		  	$casual_leave							= 	$result_particular_labour['casual_leave'];
		  	$casual_leave_no						= 	$result_particular_labour['casual_leave_no'];
		  	$sick_leave								= 	$result_particular_labour['sick_leave'];
		 	$sick_leave_no							= 	$result_particular_labour['sick_leave_no'];
		  	$other_leave							= 	$result_particular_labour['other_leave'];
		 	$other_leave_no							= 	$result_particular_labour['other_leave_no'];
			$maternity_leave						= 	$result_particular_labour['maternity_leave'];
		 	$maternity_leave_no						= 	$result_particular_labour['maternity_leave_no'];
			$particulerid							=	$result_particular_labour['id'];
			
			
			$fetch_license_data 					= 	db_select('l_contractor_license_application', 'license');
  			$fetch_license_data						->	fields('license',array('id','backlog_license_date','backlog_license_no'));
  			$fetch_license_data						->	condition('license.contractor_particular_id',$particulerid,'=');
			$license_data 							= 	$fetch_license_data->execute()->fetchAssoc();	
			$license_contractor_id 					= 	$license_data['id'];
			$backlog_license_date 					= 	$license_data['backlog_license_date'];
			$backlog_license_no 					= 	$license_data['backlog_license_no'];	
			
		
		}else{
			$rowcount	=	0;
		}
		
	
	if(!empty($license_contractor_id)){
		
		$renewal_data						=	fetch_all_data_contractor_license_apply_renweal($license_contractor_id,$contractor_id);
		$license_reneal_fees				=	$renewal_data['license_reneal_fees'];
		$earned_leave_no					=	$renewal_data['earned_leave_no'];
		$spred_over							=	$renewal_data['spred_over'];
		$holiday_wages						=	$renewal_data['holiday_wages'];
		$special_benifites					=	$renewal_data['special_benifites'];
		$state_insurance					=	$renewal_data['state_insurance'];
		$miscellaneous_provisions			=	$renewal_data['miscellaneous_provisions'];
		$backlog_renewal_date				=	$renewal_data['backlog_renewal_date'];
		$backlog_renewal_fees				=	$renewal_data['backlog_renewal_fees'];
		$renewal_application_final_status	= 	$renewal_data['renewal_application_final_status'];
		$renewal_appliaction_status			= 	$renewal_data['renewal_appliaction_status'];
		$renewal_id							= 	$renewal_data['id'];	
		$is_extra_fees						= 	$renewal_data['is_extra_fees'];	
		$increnent_fees						= 	$renewal_data['increnent_fees'];
		$renewal_security_new				= 	$renewal_data['renewal_security_new'];
		$is_coparative						= 	$renewal_data['is_coparative'];	
		$recommand_date_expire				= 	$renewal_data['recommand_date_expire'];
		$license_reneal_fees				=	$renewal_data['license_reneal_fees'];
	}
	
	//print_r($renewal_data);die;
	
	if(empty($amount_of_security_deposit)){
		$amount_of_security_deposit=$max_labour*100;
	}
	
	$rest_holiday = substr($no_holiday, 0, -1);
	$no_holiday_list= explode(',', $rest_holiday);
	
	
	if(empty($max_labour_particular)){
		$max_labour=$max_labour;
	}
	else
	$max_labour=$max_labour_particular;
	
	$status_query_result 	= 	application_status($serial,$contractor_id);//-------call from miscellaneous module
	
	
	 $status					=	$status_query_result['status'];
	 $final_status				=	$status_query_result['final_status'];
	 $editablefield				=	fetch_editable_field_renewal($serial,$renewal_id);//---call from miscellaneous module
	
 	$readonly_backlog_renewal_date 			= array( 'class' => array('last_renewal_date_1'), 'onclick' => 'datepicker_lastrenewal();','readonly' => 'readonly',);
	$readonly_date_license 		        = array( 'class' => array('last_renewal_date_1'), 'onclick' => 'datepicker_lastrenewal();','readonly' => 'readonly',);
	if(!empty($editablefield)){
		
		$readonly_agent							= array('placeholder' => 'Name of the agent or manager of Contractor it the work site', 'class' => array('correction_view'));
		   $readonly_address_manager 				= array('placeholder' => 'Address of the agent or manager of Contractor it the work site', 'class' => array('correction_view'));
			$readonly_contract_manger_dist 			= array( 'class' => array('correction_view'));
			$readonly_contract_manger_subdivision 	= array('class' => array('correction_view'));
			$readonly_contract_manger_areatype 		= array('class' => array('correction_view'));
			$readonly_contract_manger_block 		= array( 'class' => array('correction_view'));
			$readonly_contract_manger_mun 			= array( 'class' => array('correction_view'));
			$readonly_contract_manger_corp 			= array( 'class' => array('correction_view'));
			$readonly_contract_manger_sez 			= array( 'class' => array('correction_view'));
			$readonly_contract_manger_not 			= array( 'class' => array('correction_view'));
			$readonly_contract_manger_vill 			= array( 'class' => array('correction_view'));
			$readonly_contract_manger_ward 			= array( 'class' => array('correction_view'));
			$readonly_contract_manger_bugh 			= array( 'class' => array('correction_view'));
			$readonly_contractor_manger_ps 			= array( 'class' => array('correction_view'));
			$readonly_contractor_manger_pin 		= array( 'class' => array('correction_view'));
			$readonly_category						= array('placeholder' => 'Category/designation/nomenclature of the contractor labour, namely,fitter,welder,carpenter,mazdor etc','class' => array('correction_view'));
			$readonly_unskilled_rate_wages 			= array( 'class' => array('correction_view'));
			$readonly_semiskilled_rate_wages 		= array( 'class' => array('correction_view'));
			$readonly_skilled_rate_wages 			= array( 'class' => array('correction_view'));
			$readonly_highlyskilled_rate_wages 		= array( 'class' => array('correction_view'));
			$readonly_hours_work 					= array( 'class' => array('correction_view'));
			$readonly_overtime 						= array( 'class' => array('correction_view'));
			$readonly_overtime_wages 				= array( 'class' => array('correction_view'));
			$readonly_weekly_holiday 				= array( 'class' => array('correction_view'));
			$readonly_annual_leave_no 				= array( 'class' => array('correction_view'));	
			$readonly_casual_leave_no				= array( 'class' => array('correction_view'));	
			$readonly_sick_leave_no 				= array( 'class' => array('correction_view'));
			$readonly_maternity_leave_no			= array( 'class' => array('correction_view'));	
			$readonly_other_leave_no 				= array( 'class' => array('correction_view'));
			$readonly_eraned_leave_no 				= array( 'class' => array('correction_view'));
			$readonly_backlog_licnse_no 			= array( 'class' => array('correction_view'));
			//$readonly_backlog_licnse_date 			= array( 'class' => array('correction_view'));
			$readonly_license_amount 				= array( 'class' => array('correction_view'));
			$readonly_security_amount				= array( 'class' => array('correction_view'));
			$readonly_backlog_renewal_amount 		= array( 'class' => array('correction_view'),'placeholder' => 'Total amount of previous Renewal of license');
			$readonly_backlog_renewal_date 			= array( 'class' => array('last_renewal_date'), 'onclick' => 'datepicker_lastrenewal();','readonly' => 'readonly');
			$readonly_special_benifites 			= array( 'class' => array('correction_view'));
			$readonly_state_insurance 				= array( 'class' => array('correction_view'));
			$readonly_miscellaneous 				= array( 'class' => array('correction_view'));
			$readonly_offence 						= array('');
			$readonly_offence_test					= 'correction_show';
			$readonly_revoking 						= array('');
			$readonly_revoking_test					='correction_show';
			$readonly_pervious_license 				= array('');
			$readonly_pervious_test					='correction_show';
			$readonly_pervious_license				=array();
			$readonly_pervious_test					='correction_show';
			
			
$readonly_date_license 			= array( 'class' => array('last_renewal_date'), 'onclick' => 'datepicker_lastrenewal();','readonly' => 'readonly');

				
	  $editablefield_array = explode(',', $editablefield['remark_field_title']);
	  
	  
	   foreach($editablefield_array as $row){
		  
		   
		
			switch($row){
					
				case "name_manager":
							$readonly_agent		= array('placeholder' => 'Name of the agent or manager of contractor it the work site','disabled' => 'disabled', 'class' => array('readonly_view'));
							
							break;
					  
				case "address_manager":
					$readonly_address_manager 				= array('placeholder' => 'Address of the agent or manager of contractor it the work site','disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contract_manger_dist 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contract_manger_subdivision 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contract_manger_areatype 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contract_manger_block 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contract_manger_mun 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contract_manger_corp 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contract_manger_sez 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contract_manger_not 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contract_manger_vill 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contract_manger_ward 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contract_manger_bugh 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contractor_manger_ps 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contractor_manger_pin 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					
					
							break;
					  
				case "category":
						$readonly_category				= array('placeholder' => 'Category/designation/nomenclature of the contractor labour, namely,fitter,welder,carpenter,mazdor etc','readonly' => 'readonly', 'class' => array('readonly_view'));
						
						break;
					  
				case "unskilled_wages":
							$readonly_unskilled_rate_wages 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							
							break;
					  
				case "semiskilled_wages":
							$readonly_semiskilled_rate_wages = array('disabled' => 'disabled', 'class' => array('readonly_view'));
							
							break;
					  
				case "skilled_wages":
							$readonly_skilled_rate_wages 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							
							 break;
					  
				case "highlysiskilled_wages":
						$readonly_highlyskilled_rate_wages = array('disabled' => 'disabled', 'class' => array('readonly_view'));
						
							break;
					  
				case "hour_details":
							$readonly_hours_work = array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_overtime 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_overtime_wages = array('disabled' => 'disabled', 'class' => array('readonly_view'));
							
							break;
					  
				case "weekly_holiday":
							$readonly_weekly_holiday 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly					=	'true';
							
							break;
							
				case "holiday_wages":
							$readonly_holiday_wages	=	array('disabled' => 'disabled', 'class' => array('readonly_view'));
							
							break;
					  
				case "annual_leave":
							$readonly_annual_leave_no = array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							
							break;
				case "casual_leave":
							$readonly_casual_leave_no = array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							
							break;
				case "eraned_leave":
							$readonly_eraned_leave_no = array('disabled' => 'disabled', 'class' => array('readonly_view'));
							
							break;
				case "sick_leave":
							$readonly_sick_leave_no = array('disabled' => 'disabled', 'class' => array('readonly_view'));
							
							break;
				case "maternity_leave":
							$readonly_maternity_leave_no = array('disabled' => 'disabled', 'class' => array('readonly_view'));
							
							break;
				
				case "other_leave":
							$readonly_other_leave_no = array('disabled' => 'disabled', 'class' => array('readonly_view'));
							
							
							break;
							
				case "backlog_licnse_date":
							$readonly_backlog_licnse_no 	= 	array('readonly' => true ,'class' => array('readonly_view'));
							//$readonly_backlog_licnse_date 	= 	array('readonly' => true);
							$readonly_date_license = array('readonly' => true,'class'=> array('readonly_view_date'),'readonly' => 'readonly');
							$flag_date						=	1;
			
							break;	
				case "license_amount":
							$readonly_license_amount = array('readonly' => true, 'class' => array('readonly_view'));
		
							break;		
				case "security_amount":
							$readonly_security_amount = array('readonly' => true, 'class' => array('readonly_view'));
							
		
							break;
				case "backlog_renewal_amount":
							$readonly_backlog_renewal_amount = array('readonly' => true, 'class' => array('readonly_view'),'placeholder' => 'Total amount of previous Renewal of license');
		
							break;	
				case "backlog_renewal_date":
							//$readonly_backlog_renewal_date = array('readonly' => true);
							$readonly_backlog_renewal_date = array('readonly' => true,'class'=> array('readonly_view_date'),'readonly' => 'readonly');
							
							$flag_renewal_date	=	1;
		
							break;
				case "special_benifites":
							$readonly_special_benifites = array('readonly' => true, 'class' => array('readonly_view'));
		
							break;	
				case "state_insurance":
							$readonly_state_insurance = array('readonly' => true, 'class' => array('readonly_view'));
		
							break;	
				case "miscellaneous":
							$readonly_miscellaneous = array('readonly' => true, 'class' => array('readonly_view'));
		
							break;	
				
				case "offence":
							$readonly_offence = array('disabled' => 'disabled');
							$readonly_offence_test='verified_show';
							
							break;	
				case "revoking":
							$readonly_revoking = array('disabled' => 'disabled',);
							$readonly_revoking_test='verified_show';
							
							break;	
				case "pervious_license":
							$readonly_pervious_license = array('disabled' => 'disabled',);
							$readonly_pervious_test	=	'verified_show';
							
							break;
				
				
					  
				default:
					 
				}
			
		}
				
	
	}//print_r($readonly_miscellaneous);
	//die();
	
	 
	$form['renewal_contractlaour'] = array(
							'#title' 		=>	 t('<div class="content"><div class="sky-form" style="width:100%"><header><h3 align="center"> APPLICATION FOR RENEWAL OF LICENSE</h3></header>'),
							'#type' 		=> 	'fieldset',
							'#attributes' 	=> 	array('class'=>array('')),														
							);
														
	$form['renewal_contractlaour']['applicant_id'] = array(			 
									   '#type' 			=> 'hidden',
									  '#default_value'	=> !empty($applicant_id) ? $applicant_id	:''
								);
	$form['renewal_contractlaour']['recommand_date_expire'] = array(			 
									   '#type' 			=> 'hidden',
									  '#default_value'	=> !empty($recommand_date_expire) ? $recommand_date_expire	:''
								);
										
	$form['renewal_contractlaour']['application_id'] = array(
									  '#type' 				=> 'hidden',
									  '#default_value'		=> !empty($application_id) ? $application_id	:''
								);
																	
	$form['renewal_contractlaour']['con_info_id'] =	 array(
										'#type' 			=> 'hidden',
										'#default_value'	=> !empty($con_info_id) ? $con_info_id	:''
								);
	$form['renewal_contractlaour']['editable_work'] = array(
									  '#type' 				=> 'hidden',
									  '#default_value'		=> !empty($editable_work) ? $editable_work	:''
								);
	$form['renewal_contractlaour']['editable_agent'] = array(
										'#type' 			=> 'hidden',
										'#default_value'	=> !empty($editable_agent) ? $editable_agent	:''
								);
	$form['renewal_contractlaour']['editable_address_manager'] = array(
										'#type' 			=> 'hidden',
										'#default_value'	=> !empty($editable_address_manager) ? $editable_address_manager	:''
							  );
	$form['renewal_contractlaour']['editable_max_no_day'] = array(
										'#type' 			=> 'hidden',
									    '#default_value'	=> !empty($editable_max_no_day) ? $editable_max_no_day	:''
							 );
    $form['renewal_contractlaour']['editable_convicted'] = array(
										'#type' 			=> 'hidden',
										'#default_value'	=> !empty($editable_convicted) ? $editable_convicted	:''
							);
	$form['renewal_contractlaour']['editable_revoking'] = array(
										'#type' 			=> 'hidden',
										'#default_value'	=> !empty($editable_revoking) ? $editable_revoking	:''
							);
	$form['renewal_contractlaour']['editable_previous_employer'] = array(
										'#type' 			=> 'hidden',
										'#default_value' 	=> !empty($editable_previous_employer) ? $editable_previous_employer	:''
							);
								
										
	$form['renewal_contractlaour']['contract_agnt_name_edit'] = array(
										'#type' 			=> 'hidden',
										'#default_value' 	=>!empty($name_of_agent) ? $name_of_agent : '',
												
							);
																		
	$form['renewal_contractlaour']['manager_add_edit'] = array(
										'#type' 			=> 'hidden',
										'#default_value' 	=>!empty($address_of_manager) ? $address_of_manager : '',
												
							);									
	$form['contractor_dashboard']['manager_dist_edit'] = array(									 
									  '#type' 			=> 	'hidden',
									  '#default_value'  =>	!empty($contractor_dist) ? $contractor_dist : '',
																			
							);
	$form['contractor_dashboard']['manager_subdivision_edit'] = array(									 
									  '#type' 			=> 	'hidden',
									  '#default_value'  =>	!empty($contractor_sub_division) ? $contractor_sub_division : '',
																			
							);
	$form['contractor_dashboard']['manager_areatype_edit'] = array(									 
									  '#type' 			=> 	'hidden',
									  '#default_value'  =>	!empty($contractor_area_type) ? $contractor_area_type : '',
																			
							);
	$form['contractor_dashboard']['name_manager_areatype_edit'] = array(									 
									  '#type' 			=> 	'hidden',
									  '#default_value'  =>	!empty($contractorname_area_type) ? $contractorname_area_type : '',
																			
							 );
	$form['contractor_dashboard']['manager_vill_ward_edit'] = array(									 
									  '#type' 			=> 	'hidden',
									  '#default_value'  =>	!empty($contractor_villward) ? $contractor_villward : '0',
																			
							);
									  	
	$form['contractor_dashboard']['manager_ps_edit'] = array(									 
									  '#type' 			=> 	'hidden',
									  '#default_value'  =>	!empty($contractor_police_station) ? $contractor_police_station : '',
																			
							);
	$form['contractor_dashboard']['convicted_edit'] = array(									 
									  '#type' 			=> 	'hidden',
									  '#default_value'  =>	!empty($contractor_convicted) ? $contractor_convicted : '0',
																			
							);	
	$form['contractor_dashboard']['convicted_details_edit'] = array(									 
									  '#type' 			=> 	'hidden',
									  '#default_value'  =>	!empty($details_contractor_convicted) ? $details_contractor_convicted : '',
																			
							);
	$form['contractor_dashboard']['revoking_edit'] = array(									 
									  '#type' 			=> 	'hidden',
									  '#default_value'  =>	!empty($contractor_revoking) ? $contractor_revoking : '0',
																			
							);
	$form['contractor_dashboard']['revoking_details_edit'] = array(									 
									  '#type' 			=> 	'hidden',
									  '#default_value'  =>	!empty($details_contractor_revoking) ? $details_contractor_revoking : '',
																			
							);
	$form['contractor_dashboard']['previous_employer_edit'] = array(									 
									  '#type' 			=> 	'hidden',
									  '#default_value'  =>	!empty($contractor_previous_employer) ? $contractor_previous_employer : '0',
																			
							);
    $form['contractor_dashboard']['previous_employer_details_edit'] = array(									 
									  '#type' 			=> 	'hidden',
									  '#default_value'  =>	!empty($details_previous_employer) ? $details_previous_employer : '',
																			
							 );
									  
									  
	$form['contractor_dashboard']['serial']=array(
								'#type' => 	'hidden',
								'#default_value'  =>	!empty($serial) ? $serial : '',
							);
	
		$form['renewal_contractlaour']['contract_labour_id'] = array(
													  '#title'				=>	'4.a) Nature of Work in which Contract Labour is Employed or is to be Employed ',
													  '#type' 				=> 'hidden',
													  '#default_value' 		=>	!empty($contract_labour_id) ? $contract_labour_id : '',
												
											  );

	//die();									  
	$form['renewal_contractlaour']['flag'] = array(
												'#type'						 => 'hidden',
												'#default_value' 			 =>	!empty($flag) ? $flag : '0',
												
											  );
	 $form['renewal_contractlaour']['serial_no'] = array(
	 											'#type' 			=> 'hidden',
												'#default_value' 	=>	!empty($serial) ? $serial : '',
												
											  );
	$form['renewal_contractlaour']['rowcount'] = array(
												
												'#type' 					=> 'hidden',
												'#default_value' 			=>	!empty($rowcount) ? $rowcount : '',
												
											  );
											  
	
	$form['contractor_dashboard']['renewal_appliaction_status']	=	array(
	        '#type'		=> 'hidden',
			'#default_value'	=>	!empty($renewal_appliaction_status) ? encryption_decryption_fun('encrypt', $renewal_appliaction_status) : '',
		);	
	
	
	
											  
 	$form['renewal_contractlaour']['contract_agnt_name'] = array(
												'#prefix' 					=>  '<fieldset><legend>5.Details of manager/agent of contractor at the worksite</legend><div class="row"><section class="col col-6" ><label class="input">',
												'#suffix' 					=>  '</label></section>',
												'#title'					=>	'A.Name of agent/manager :',
												'#type' 					=> 'textfield',
												'#default_value' 			=>	!empty($name_of_agent) ? $name_of_agent : '',
												'#attributes' 				=> 	$readonly_agent
											  );
																
	$form['renewal_contractlaour']['contract_agnt_add'] = array(
												'#prefix' 					=>  '<section class="col col-6" ><label class="input">',
												'#suffix' 					=>  '</label></section></div>',
												'#title'					=>	'B.Address Line1 of agent/manager:',
												'#type' 					=> 	'textfield',
												'#default_value' 			=>	!empty($address_of_manager) ? $address_of_manager : '',
												'#attributes' 				=> 	$readonly_address_manager,
												'#required' 				=> TRUE,
											 );
	$options_dist 	= get_district_contract_labour();
		
	$form['renewal_contractlaour']['manager_dist'] 	= array(
											  '#prefix' 			=> '<div class="row"><section class="col col-6"><label class="select">',
											  '#suffix' 			=> '</label></section>',
											  '#title'				=> 'C.Select District',
											  '#type' 				=> 'select',
											  '#options' 			=> $options_dist,
											  '#required' 			=> TRUE,
											  '#attributes' 		=>  $readonly_contract_manger_dist,
											  '#default_value' 		=> !empty($contractor_dist) ? $contractor_dist : '',
											  '#ajax'				=> array(
																		'event' 			=> 'change',
																		'effect' 			=> 'fade',			
																		'callback'			=> 'get_manager_address',
																		'progress'			=> '',
																	  )
										); 

	$value_manager_dist	= 	isset($form_state['values']['manager_dist']) ? $form_state['values']['manager_dist'] : $contractor_dist;
	
	if(empty($value_manager_dist)){
		
	$form['renewal_contractlaour']['manager_subdivision'] 	= array(
											  '#prefix' 			=> '<div id="manager_subdvsn_replace"><section class="col col-6"><label class="select">',
											  '#suffix' 			=> '</label></section></div>',
											  '#title'				=> 'D.Select Subdivision',
											  '#type' 				=> 'select',
											  '#options' 			=> array(),
											  '#required' 			=> TRUE,
											  '#attributes' 		=>  $readonly_contract_manger_subdivision,
											  '#ajax'				=> array(
																				'event' 			=> 'change',
																				'effect' 			=> 'fade',			
																				'callback'			=> 'get_manager_address',
																				'progress'			=> '',
																	  ),
										);
	}else{
		
		$form['renewal_contractlaour']['manager_subdivision'] 	= array(
												  '#prefix' 			=> '<div id="manager_subdvsn_replace"><section class="col col-6"><label class="select">',
												  '#suffix' 			=> '</label></section></div>',
												  '#title'				=> 'D.Select Subdivision',
												  '#type' 				=> 'select',
												  '#options' 			=> get_subdivisionlist_contract_labour($value_manager_dist),
												  '#required' 			=> TRUE,
												  '#attributes' 		=>  $readonly_contract_manger_subdivision,
												  '#default_value' 		=> !empty($contractor_sub_division) ? $contractor_sub_division : '',
												  '#ajax'				=> array(
																					'event' 			=> 'change',
																					'effect' 			=> 'fade',			
																					'callback'			=> 'get_manager_address',
																					'progress'			=> '',
																		  ),
										);
	}
		
	$value_manager_subdivision	= isset($form_state['values']['manager_subdivision']) ? $form_state['values']['manager_subdivision'] : $contractor_sub_division;
		
	$areatypeOptions = get_areatype_contract_labour($value_manager_dist, $value_manager_subdivision);
	if(empty($areatypeOptions)){
			$form['renewal_contractlaour']['manager_areatype'] = array(
											  '#prefix' 			=> '<div id="manager_areatype_replace"><section class="col col-6"><label class="select">',
											  '#suffix' 			=> '</label></section></div>',
											  '#title'				=> 'E.Select Areatype',
											  '#type' 				=> 'select',
											  '#options' 			=> array(),
											  '#attributes' 		=>  $readonly_contract_manger_areatype,
											  '#required' 			=> TRUE,
											  
											  '#ajax'				=> array(
																		'event' 			=> 'change',
																		'effect' 			=> 'fade',			
																		'callback'			=> 'get_manager_address',
																		'progress'			=> '',
																	  ),
												);
		}else{
		
		$form['renewal_contractlaour']['manager_areatype'] = array(
											  '#prefix' 			=> '<div id="manager_areatype_replace"><section class="col col-6"><label class="select">',
											  '#suffix' 			=> '</label></section></div>',
											  '#title'				=> 'E.Select Areatype',
											  '#type' 				=> 'select',
											  '#options' 			=> $areatypeOptions,
											  '#attributes' 		=>  $readonly_contract_manger_areatype,
											  '#required' 			=> TRUE,
											  '#default_value' 		=> !empty($contractor_area_type) ? $contractor_area_type : '',
											  '#ajax'				=> array(
																		'event' 			=> 'change',
																		'effect' 			=> 'fade',			
																		'callback'			=> 'get_manager_address',
																		'progress'			=> '',
																	  ),
												);
		}
			
		$value_manager_areatype = isset($form_state['values']['manager_areatype']) ? $form_state['values']['manager_areatype'] : $contractor_area_type;
			
				
				
		$form['renewal_contractlaour']['name_manager_areatype'] = array(
											 '#prefix' 				=> '<div id="name_manager_areatype_replace"><section class="col col-6"><label class="select">',
											 '#suffix' 				=> '</label></section></div>',
											 '#title'				=> 'F.Select Block/Municipality/Corporation/SEZ/Notified Area',
											 '#type' 				=> 'select',
											 '#options' 			=> array(),
											 '#attributes' 			=>  $readonly_contract_manger_areatype,
											 '#required' 			=> TRUE,
											 
											
											 '#ajax'				=> array(
																		'event' 			=> 'change',
																		'effect' 			=> 'fade',			
																		'callback'			=> 'get_manager_address',
																		'progress'			=> '',
																	  )
								 );
			
			
		if($value_manager_dist != "" && $value_manager_subdivision != ""){
			if($value_manager_areatype != ""){
				if($value_manager_areatype == 'B'){
					$form['renewal_contractlaour']['name_manager_areatype'] = array(
													  '#prefix' 			=>'<div id="name_manager_areatype_replace"><section class="col col-6"><label class="select">',
													  '#suffix' 			=> '</label></section></div>',
													  '#title'				=> 'F.Select Block',
													  '#type' 				=> 'select',
													  '#options' 			=> get_block_contract_labour($value_manager_subdivision, $value_manager_areatype),
													  '#required' 			=> TRUE,
													  '#attributes' 		=>  $readonly_contract_manger_block,
													  '#default_value' 		=> !empty($contractorname_area_type) ? $contractorname_area_type : '',
													  '#ajax'				=> array(
																				'event' 			=> 'change',
																				'effect' 			=> 'fade',			
																				'callback'			=> 'get_manager_address',
																				'progress'			=> '',
																			  )
													);
				}if($value_manager_areatype == 'M'){
						$form['renewal_contractlaour']['name_manager_areatype'] = array(
														  '#prefix' 			=>'<div id="name_manager_areatype_replace"><section class="col col-6"><label class="select">',
														  '#suffix' 			=> '</label></section></div>',
														  '#title'				=> 'F.Select Municipality',
														  '#type' 				=> 'select',
														  '#options' 			=> get_block_contract_labour($value_manager_subdivision, $value_manager_areatype),
														  '#required' 			=> TRUE,
														  '#attributes' 		=>  $readonly_contract_manger_mun,
														 '#default_value' 		=> !empty($contractorname_area_type) ? $contractorname_area_type : '',
														  '#ajax'				=> array(
																					'event' 			=> 'change',
																					'effect' 			=> 'fade',			
																					'callback'			=> 'get_manager_address',
																					'progress'			=> '',
																				  )
														);
				}if($value_manager_areatype == 'C'){
						$form['renewal_contractlaour']['name_manager_areatype'] = array(
														  '#prefix' 			=>'<div id="name_manager_areatype_replace"><section class="col col-6"><label class="select">',
														  '#suffix' 			=> '</label></section></div>',
														  '#title'				=> 'F.Select Corporation',
														  '#type' 				=> 'select',
														  '#options' 			=> get_block_contract_labour($value_manager_subdivision, $value_manager_areatype),
														  '#required' 			=> TRUE,
														  '#attributes' 		=>  $readonly_contract_manger_corp,
														  '#default_value' 		=> !empty($contractorname_area_type) ? $contractorname_area_type : '',
														  '#ajax'				=> array(
																					'event' 			=> 'change',
																					'effect' 			=> 'fade',			
																					'callback'			=> 'get_manager_address',
																					'progress'			=> '',
																				  )
														);
				}if($value_manager_areatype == 'S'){
						$form['renewal_contractlaour']['name_manager_areatype'] = array(
														  '#prefix' 			=>'<div id="name_manager_areatype_replace"><section class="col col-6"><label class="select">',
														  '#suffix' 			=> '</label></section></div>',
														  '#title'				=> 'F.Select SEZ',
														  '#type' 				=> 'select',
														  '#options' 			=> get_block_contract_labour($value_manager_subdivision, $value_manager_areatype),
														  '#required' 			=> TRUE,
														  '#attributes' 		=>  $readonly_contract_manger_sez,
														  '#default_value' 		=> !empty($contractorname_area_type) ? $contractorname_area_type : '',
														  '#ajax'				=> array(
																					'event' 			=> 'change',
																					'effect' 			=> 'fade',			
																					'callback'			=> 'get_manager_address',
																					'progress'			=> '',
																				  )
														);
				}if($value_manager_areatype == 'N'){
						$form['renewal_contractlaour']['name_manager_areatype'] = array(
														  '#prefix' 			=>'<div id="name_manager_areatype_replace"><section class="col col-6"><label class="select">',
														  '#suffix' 			=> '</label></section></div>',
														  '#title'				=> 'F.Select Notified Area',
														  '#type' 				=> 'select',
														  '#options' 			=> get_block_contract_labour($value_manager_subdivision, $value_manager_areatype),
														  '#required' 			=> TRUE,
														  '#attributes' 		=>  $readonly_contract_manger_not,
														 '#default_value' 		=> !empty($contractorname_area_type) ? $contractorname_area_type : '',
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
			
		$value_name_manager_areatype = isset($form_state['values']['name_manager_areatype']) ? $form_state['values']['name_manager_areatype'] : $contractorname_area_type;
		$form['renewal_contractlaour']['manager_vill_ward'] = array(
											 '#prefix' 				=> '<div id="manager_vill_ward_replace"><section class="col col-4"><label class="select">',
											 '#suffix' 				=> '</label></section></div>',
											 '#title'				=> 'G.Select Gram Panchayat/Ward',
											 '#type' 				=> 'select',
											 '#options' 			=> array(),
											 '#attributes' 			=>  $readonly_contract_manger_vill,
											 '#required' 			=> TRUE,											
											 '#ajax'				=> array(
																		'event' 			=> 'change',
																		'effect' 			=> 'fade',			
																		'callback'			=> 'get_manager_address',
																		'progress'			=> '',
																	  )
	
	
										  );
																				  
																				  
			if($value_manager_dist != "" && $value_manager_subdivision != ""){
				if($value_manager_areatype != ""){
					if($value_name_manager_areatype != ""){
						if($value_manager_areatype == 'B'){
								
								$form['renewal_contractlaour']['manager_vill_ward'] = array(
																  '#prefix' 			=> '<div id="manager_vill_ward_replace"><section class="col col-4"><label class="select">',
																  '#suffix' 			=> '</label></section></div>',
																  '#title'				=> 'G.Select Gram Panchayat',
																  '#type' 				=> 'select',
																  '#options'			=> get_villward_contract_labour($value_name_manager_areatype,'V'),
																  '#required' 			=> TRUE,
																  '#attributes' 		=>  $readonly_contract_manger_vill,
																 '#default_value' 		=> !empty($contractor_villward) ? $contractor_villward : '',
																  '#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_manager_address',
																								'progress'			=> '',
																							  )
																);	
						}if($value_manager_areatype == 'M'){
								
								$form['renewal_contractlaour']['manager_vill_ward'] = array(
																  '#prefix' 			=> '<div id="manager_vill_ward_replace"><section class="col col-4"><label class="select">',
																  '#suffix' 			=> '</label></section></div>',
																  '#title'				=> 'G.Select Ward',
																  '#type' 				=> 'select',
																  '#options' 			=> get_villward_contract_labour($value_name_manager_areatype,'W'),
																  '#required' 			=> TRUE,
																  '#attributes' 		=>  $readonly_contract_manger_ward,
																  '#default_value' 		=> !empty($contractor_villward) ? $contractor_villward : '',
																  '#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_manager_address',
																								'progress'			=> '',
																							  )
																);
																								
						}
							
						if($value_manager_areatype == 'C'){
								
						$form['renewal_contractlaour']['manager_vill_ward'] 	= array(
									  '#prefix' 			=> '<div id="manager_vill_ward_replace"><section class="col col-4"><label class="select">',
									  '#suffix' 			=> '</label></section></div>',
									  '#title'				=> 'G.Select Ward',
									  '#type' 				=> 'select',
									  '#options' 			=> get_corporation_contract_labour($value_name_manager_areatype),
									  '#required' 			=> TRUE,
									  '#attributes' 		=>  $readonly_contract_manger_ward,
									  '#default_value' 		=> !empty($contractor_villward) ? $contractor_villward : '',
									  '#ajax'				=> array(
																		'event' 			=> 'change',
																		'effect' 			=> 'fade',			
																		'callback'			=> 'get_manager_address',
																		'progress'			=> '',
															  		)
						);
				}if($value_manager_areatype == 'N'){
								
								$form['renewal_contractlaour']['manager_vill_ward'] = array(
																  '#prefix' 			=> '<div id="manager_vill_ward_replace"><section class="col col-4"><label class="select">',
																  '#suffix' 			=> '</label></section></div>',
																  '#title'				=> 'G.Select Notified Area',
																  '#type' 				=> 'select',
																  '#options' 			=> get_villward_contract_labour($value_name_manager_areatype,'N'),
																  '#required' 			=> TRUE,
																  '#attributes' 		=>  $readonly_contract_manger_ward,
																  '#default_value' 		=> !empty($contractor_villward) ? $contractor_villward : '',
																  '#ajax'				=> array(
																								'event' 			=> 'change',
																								'effect' 			=> 'fade',			
																								'callback'			=> 'get_manager_address',
																								'progress'			=> '',
																							  )
																);
																								
						}if($value_manager_areatype == 'S'){
								
								$form['renewal_contractlaour']['manager_vill_ward'] = array(
									  '#prefix' 			=> '<div id="manager_vill_ward_replace"><section class="col col-4"><label class="select">',
									  '#suffix' 			=> '</label></section></div>',
									  '#title'				=> 'G.Select Sector',
									  '#type' 				=> 'select',
									  '#options' 			=> get_villward_contract_labour($value_name_manager_areatype,'S'),
									  '#required' 			=> TRUE,
									  '#attributes' 		=>  $readonly_contract_manger_ward,
									  '#default_value' 		=> !empty($contractor_villward) ? $contractor_villward : '',
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
			
			$value_manager_vill_ward = isset($form_state['values']['manager_vill_ward']) ? $form_state['values']['manager_vill_ward'] : $contractor_villward;
			
			$form['renewal_contractlaour']['manager_ps'] = array(
												'#prefix' 				=> '<div id="manager_ps_replace"><section class="col col-4"><label class="select">',
												'#suffix' 				=> '</label></section></div>',
												'#title'				=> 'H.Select Police Station',
												'#type' 				=> 'select',
												'#options' 				=> array(),
												'#required' 			=> TRUE,
												'#attributes' 			=>  $readonly_contractor_manger_ps,
												'#ajax'					=> array(
																			'event' 			=> 'change',
																			'effect' 			=> 'fade',			
																			'callback'			=> 'get_manager_address',
																			'progress'			=> '',
																		  )
										  );
																								  
			if($value_manager_dist != "" && $value_manager_subdivision != ""){
				if($value_manager_areatype != ""){
					if($value_name_manager_areatype != ""){
						if($value_manager_vill_ward != ""){
							$form['renewal_contractlaour']['manager_ps'] = array(
															  '#prefix' 			=> '<div id="manager_ps_replace"><section class="col col-4"><label class="select">',
															  '#suffix' 			=> '</label></section></div>',
															  '#title'				=> 'H.Select Police Station',
															  '#type' 				=> 'select',
															  '#default_value' 		=> !empty($contractor_police_station) ? $contractor_police_station : '',
															  '#options' 			=> get_ps_contract_labour($value_manager_dist),
															  '#required' 			=> TRUE,
															  '#attributes' 		=>  $readonly_contractor_manger_ps,
															  );
						}
					}
				}
			}

			
			$form['renewal_contractlaour']['contractor_pin'] = array(
												  '#prefix' 			=> '<section class="col col-4"><label class="input">',
												  '#suffix' 			=> '</label></section></div></fieldset>',
												  '#title'				=>'I.PIN Code ',
												  '#type' 				=> 'textfield',
												
												  '#default_value' 		=> !empty($manager_pin) ? $manager_pin	:'',
												  '#required' 			=> TRUE,
												  '#element_validate'	=>  array('validate_integer') ,
												  '#maxlength'			=>'6',
												  '#attributes' 		=>  $readonly_contractor_manger_pin,
												);

	
	
						
			 $readonly_max_no	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
			 $readonly_fees		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
			 $form['renewal_contractlaour']['noof_contractlabour_edit'] = array(
												'#type' 		 => 'hidden',
												'#default_value' =>	!empty($max_labour) ? $max_labour : '',														
												);
			 $form['renewal_contractlaour']['fees_edit'] = array(
												'#type' 		 => 'hidden',
												'#default_value' =>!empty($amount_of_licence) ? $amount_of_licence : '',
												);
			 $form['renewal_contractlaour']['is_extra_fees'] = array(
												'#type' 		 => 'hidden',
												'#default_value' =>!empty($is_extra_fees) ? $is_extra_fees : '',
												);
		
	
		$form['renewal_contractlaour']['noof_contractlabour'] = array(
										'#type' 				=> 'hidden',
										'#default_value' 		=>!empty($max_labour) ? $max_labour : '',										
									    );
		$form['renewal_contractlaour']['contractlabour'] = array(
										'#prefix' 		=> '<fieldset><legend>6.Contract Labour Details</legend><div class="row"><section class="col col-4"><label class="input">',
										'#suffix' 		=> '</label></section>',
										'#title'		=> 'A.Maximum number of Contract Labour proposed to he employed in the establishment on any date',
										'#type'	 		=> 'textfield',
										'#default_value'=> !empty($max_labour) ? $max_labour : '',
										'#attributes' 	=> array('placeholder' => 'Maximum number of Contract Labour proposed .','readonly' => 'readonly', 'class' => array('readonly_view')),
										'#required' 	=> TRUE,
										);
	  $form['renewal_contractlaour']['fees'] = array(
											'#type' 			=> 'hidden',
											'#default_value'	=> !empty($amount_of_licence) ? $amount_of_licence : '',
								
							 			);
										
	if($is_extra_fees	==	1){
		$form['renewal_contractlaour']['increnent_fees'] = array(
														'#prefix' 			=> '<section class="col col-4"><label class="input">',
														'#suffix' 			=> '</label></section>',
														'#title'			=> 'B.Amount of Renewal Fees paid',
														'#type' 			=> 'textfield',
														'#default_value' 	=> !empty($increnent_fees) ? $increnent_fees : '',
														'#attributes'		=> array('placeholder' => 'Amount of Licence Fee paid.','readonly' => 'readonly' , 'class' => array('readonly_view')),
														'#required' 		=> TRUE,
														'#description'		=>'<font color="#FF0000"> Including 25% extra fees (according to R29(3))</font>'
												);
	}if($is_extra_fees !=1){
				$form['renewal_contractlaour']['fees_linces'] = array(
														'#prefix' 			=> '<section class="col col-4"><label class="input">',
														'#suffix' 			=> '</label></section>',
														'#title'			=> 'B.Amount of Renewal Fees paid',
														'#type' 			=> 'textfield',
														'#default_value' 	=> !empty($amount_of_licence) ? $amount_of_licence : '',
														'#attributes'		=> array('placeholder' => 'Amount of Licence Fee paid.','readonly' => 'readonly' , 'class' => array('readonly_view')),
														'#required' 		=> TRUE,
												);

	}
	if($is_coparative == 1){
		$form['renewal_contractlaour']['renewal_security_new'] = array(
														'#prefix' 			=> '<section class="col col-4"><label class="input">',
														'#suffix' 			=> '</label></section></div>',
														'#title'			=> 'C.Amount of Security fees paid for renewal',
														'#type' 			=> 'textfield',
														'#default_value' 	=> $max_labour*25,
														'#attributes'		=> array('placeholder' => 'Amount of Licence Fee paid.','readonly' => 'readonly' , 'class' => array('readonly_view')),
														'#required' 		=> TRUE,
														'#description'		=>'<font color="#FF0000"> For co-operative society security fees of Rs.25/- will be taken against each contract labour</font>'
														
												);
	}else{
		$form['renewal_contractlaour']['renewal_security_new'] = array(
														'#prefix' 			=> '<section class="col col-4"><label class="input">',
														'#suffix' 			=> '</label></section></div>',
														'#title'			=> 'C.Amount of Security fees paid for renewal',
														'#type' 			=> 'textfield',
														'#default_value' 	=>  $max_labour*100,
														'#attributes'		=> array('placeholder' => 'Amount of Licence Fee paid.','readonly' => 'readonly' , 'class' => array('readonly_view')),
														'#required' 		=> TRUE
														
												);
	}
	
	

	/*$form['renewal_contractlaour']['fees_linces'] = array(
											'#prefix' 			=> '<section class="col col-6"><label class="input">',
											'#suffix' 			=> '</label></section></div>',
											'#title'			=> 'B.Amount of Renewal Fees paid',
											'#type' 			=> 'textfield',
											'#default_value' 	=> !empty($amount_of_licence) ? $amount_of_licence : '',
											'#attributes'		=> array('placeholder' => 'Amount of Licence Fee paid.','readonly' => 'readonly' , 'class' => array('readonly_view')),
											'#required' 		=> TRUE,
									);
*/
	
	
	
	
	$form['renewal_contractlaour']['security_edit'] = array(
																					'#type' 			=> 'hidden',
																					'#default_value' 	=>!empty($amount_of_security_deposit) ? $amount_of_security_deposit : '100',
																					
															);
	
$form['renewal_contractlaour']['category_designation'] = array(
							'#prefix' 			=>  '<div class="row"><section><label class="textarea">',
							'#suffix' 			=>  '</label></section></div>',
							'#title'			=> 'D.Category/designation/nomenclature of the contractor labour, namely,fitter,welder,carpenter,mazdor etc',
							'#rows'				=> 2,	
							'#type' 			=> 'textarea',
							'#default_value' 	=> !empty($category_designation) ? $category_designation : '',
							'#required' 		=> TRUE,
							'#attributes'		=>	$readonly_category
																		);

											  
$form['renewal_contractlaour']['category_wages'] 	= array(
		'#prefix' 			=>  '<div class="row"><section ><label class="input">',
		'#suffix' 			=>  '</label></section><div>',
		'#type' 			=> 'markup',
		'#markup'			=> t('E.Rate of Wages,DA and other cash benifites paid/ to be paid to each category(i.e (a):Unskilled (b)Semi-Skilled (c)Skilled (d)Highly-Skilled etc.)of contract labour:'), 
);
								



$form['renewal_contractlaour']['unskilled_rate_wages'] = array(
			'#prefix' 					=>  '<div class="row"><section class ="col col-6"><label class="input">',
	
			'#suffix' 					=>  '</label></section>',
			'#title'					=> 	'E(i).Rate of Wages of Unskilled Category(including DA & Other & cash benifites)',
			'#type' 					=> 'textfield',
			'#default_value' 			=> !empty($unskilled_rate_wages) ? $unskilled_rate_wages : '0',
			'#element_validate'			=> array('isnumaric_valiadtion'),
			'#attributes'				=> $readonly_unskilled_rate_wages											  
);


											  

	
$form['renewal_contractlaour']['semiskilled_rate_wages'] = array(
	  '#prefix' 					=>  '<section class ="col col-6"><label class="input">',
	  '#suffix' 					=>  '</label></section></div>',
	  '#type' 						=> 'textfield',
	  '#title'						=> 	'E(ii).Rate of Wages of Semiskilled Category(including DA & Other & cash benifites)',
	  '#default_value' 				=> !empty($semiskilled_rate_wages) ? $semiskilled_rate_wages : '0',
	  '#element_validate'			=> array('isnumaric_valiadtion'),	
	  '#attributes'					=> $readonly_semiskilled_rate_wages											  
);
	

$form['renewal_contractlaour']['skilled_rate_wages'] = array(

		'#prefix' 						=>  '<div class="row"><section class ="col col-6"><label class="input">',
		'#suffix' 						=>  '</label></section>',
		'#type' 						=> 'textfield',
		'#title'						=> 	'E(iii).Rate of Wages of Skilled Category(including DA & Other & cash benifites)',
		'#default_value' 				=> !empty($skilled_rate_wages) ? $skilled_rate_wages : '0',
		'#element_validate'				=> array('isnumaric_valiadtion'),
		'#attributes'					=> $readonly_skilled_rate_wages												  
);						
								  


	
		
											  
	$form['renewal_contractlaour']['highlyskilled_rate_wages'] = array(

			'#prefix' 			=>  '<section class ="col col-6"><label class="input">',
			'#suffix' 			=>  '</label></section></div>',
			'#type' 			=> 'textfield',
			'#title'			=> 	'E(iv).Rate of Wages of Highlyskilled Category(including DA & Other & cash benifites)',
			'#default_value' 	=> !empty($highlyskilled_rate_wages) ? $highlyskilled_rate_wages : '0',
			'#element_validate'	=> array('isnumaric_valiadtion'),
			'#attributes'		=> $readonly_highlyskilled_rate_wages											  
	);	

			
	$form['renewal_contractlaour']['hours_work'] = array(

			  '#prefix' 			=>  '<div class="row"><section class ="col col-6"><label class="input">',
			  '#suffix' 			=>  '</label></section>',
			  '#type' 				=> 'textfield',
			  '#title' 				=> 	t('E(v).Hours of Work'),
			  '#default_value' 		=> !empty($hours_work) ? $hours_work : '0',
			  '#element_validate'	=> array('isnumaric_valiadtion') ,
			  '#required' 			=> TRUE,
			  '#attributes'			=> $readonly_hours_work
	);						 
																					
																						

$form['renewal_contractlaour']['spared_over'] = array(

				'#prefix' 			=>  '<section class ="col col-6"><label class="input">',
				'#suffix' 			=>  '</label></section></div>',
				'#type' 			=> 'textfield',
				'#title' 			=> 	t('E(vi)Spread over hour(s)'), 
				'#default_value' 	=> !empty($spred_over) ? $spred_over : '0',
			 '#element_validate'	=> array('isnumaric_valiadtion') ,
				'#required' 		=> TRUE,
				'#attributes'		=> $readonly_overtime      
																					
																				
																						
																		 );

	$form['renewal_contractlaour']['overtime'] = array(

				'#prefix' 			=>  '<div class="row"><section class ="col col-6"><label class="input">',
				'#suffix' 			=>  '</label></section>',
				'#type' 			=> 'textfield',
				'#title' 			=> 	t('E(vii).Overtime hour(s)'), 
				'#default_value' 	=> !empty($overtime) ? $overtime : '0',
				'#element_validate'	=> array('isnumaric_valiadtion'),
				'#required' 		=> TRUE,
				'#attributes'		=> $readonly_overtime      
																					
																				
																						
																		 );


			 $form['renewal_contractlaour']['overtime_wages'] = array(

																					'#prefix' 				=>  '<section class ="col col-6"><label class="input">',
																					'#suffix' 				=>  '</label></section></div>',
																					'#type' 				=> 'textfield',
																					'#title' 				=> 	t('E(viii).Overtime Wages/per hour'), 
																					'#default_value' 		=> !empty($overtime_wages) ? $overtime_wages : '0',
																					'#element_validate'		=> array('isnumaric_valiadtion') ,
																					'#required' 			=> TRUE,
																					'#attributes'			=> $readonly_overtime_wages          
											
										
												
											 							 );


	 $form['renewal_contractlaour']['weekly_holiday'] = array(

			'#prefix' 			=>  '<div class="row"><section class ="col col-6"><label class="checkboxe">',
			'#suffix' 			=>  '</label>',
			'#type' 			=> 'checkbox',
			'#title' 			=> 	'F.Whether weekly holiday observed and on which day ',  
			'#default_value' 	=> !empty($weekly_holiday) ? $weekly_holiday : '0',    
			'#attributes'		=> array( 'id' => 'weekly-holiday-id', 'onclick' => 'weekly_holiday_fun()' ,'readonly'=>$readonly) ,
			
																					
		 );
	$form['renewal_contractlaour']['no_holiday'] = array( 
			  '#prefix' 			=>  '<label class="select">',
			  '#suffix' 			=>  '</label></section>',
			  '#title'				=> 	'Select holiday(s):',
			  '#type' 			    => 	'select',
			  '#options' 			=> 	array(''=>t('Select'),'1'=>t('Sunday'),'2'=>t('Monday'),'3'=>t('Tuesday'),'4'=>t('Wednesday'),'5'=>t('Thursday'),'6'=>t('Friday'),'7'=>t('Saturday')),																					 			  '#attributes' 		=> 	array('id' => 'no-holiday-id','style'=>"width: 422px; height: 70px;" ),
			  '#multiple' 			=> 	TRUE,
			  '#default_value' 		=> 	!empty($no_holiday_list) ? $no_holiday_list : '',
			 '#attributes'			=> 	$readonly_weekly_holiday   
			);


			
									
 $form['renewal_contractlaour']['holidaywages'] = array(

				'#prefix' 				=>  '<section class ="col col-6"><label class="input">',
				'#suffix' 				=>  '</label></section></div>',
				'#type' 				=> 'textfield',
				'#title' 				=> 	t('G.holiday Wages/per hour'), 
				'#default_value' 		=> !empty($holiday_wages) ? $holiday_wages : '0',
				'#element_validate'		=> array('isnumaric_valiadtion') ,
				'#required' 			=> TRUE, 
				'#attributes'			=> $readonly_holiday_wages         
																					
				);				
									
									
 $form['renewal_contractlaour']['other_condition'] 	= array(
				'#prefix' 			=>  '<section><label class="input">',
				'#suffix' 			=>  '</label></section>',
				'#type' 			=> 	'markup',
			 	'#markup'			=> t('H.Other Condition of service like levave (annual leave,casual leave,sick leave,maternity leave etc)Holidays,hours of work etc.of the contract labour)'), 
													);


								
								

	
	
		$form['renewal_contractlaour']['annual_leave_no'] 	= array(
																				'#prefix' 			=>  '<div class="row"><section class ="col col-6"><label class="input">',
																				'#suffix' 			=>  '</label></section>',
																			  	'#type' 			=> 'textfield',
																			   	'#title'			=> t('Number of Annual Leave(s) in day(s)'), 
																			   	'#default_value' 	=> !empty($annual_leave_no) ? $annual_leave_no : '0',  
																				'#element_validate'	=>  array('isnumaric_valiadtion'),
																				'#attributes'		=>	$readonly_annual_leave_no	
																);

	
	$form['renewal_contractlaour']['casual_leave_no'] 	= array(
																				'#prefix' 			=>  '<section class ="col col-6"><label class="input">',
																				'#suffix' 			=>  '</label></section></div>',
																				'#type' 			=> 'textfield',
																			 	'#title'			=> t('Number of Casual Leave(s) in day(s)'), 
																				'#default_value' 	=> !empty($casual_leave_no) ? $casual_leave_no : '0',  
																				'#element_validate'	=> array('isnumaric_valiadtion'),
																				'#attributes'		=> $readonly_casual_leave_no
																	);

	
	$form['renewal_contractlaour']['sick_leave_no'] 	= array(
																				'#prefix' 			=>  '<div class="row"><section class ="col col-6"><label class="input">',
																				'#suffix' 			=>  '</label></section>',
																			  	'#type' 			=> 'textfield',
																			   	'#title'			=> t('Number of Sick Leave(s)'), 
																			  	'#default_value' 	=> !empty($sick_leave_no) ? $sick_leave_no : '0',
																			   	'#element_validate'	=> array('isnumaric_valiadtion'),
																				'#attributes'		=> $readonly_sick_leave_no
																				
																	);


	$form['renewal_contractlaour']['maternity_leave_no'] 	= array(
																				'#prefix' 			=>  '<section class ="col col-6"><label class="input">',
																				'#suffix' 			=>  '</label></section></div>',
																			  	'#type' 			=> 'textfield',
																			   	'#title'			=> t('Number of Mternity Leave(s) in day(s) '),
																				'#default_value' 	=> !empty($maternity_leave_no) ? $maternity_leave_no : '0' ,
																				'#element_validate'	=> array('isnumaric_valiadtion'),
																				'#attributes'		=> $readonly_maternity_leave_no
																	);


	$form['renewal_contractlaour']['other_leave_no'] 	= array(
																				'#prefix' 			=>  '<div class="row"><section class ="col col-6"><label class="input">',
																				'#suffix' 			=>  '</label></section>',
																			  	'#type' 			=> 'textfield',
																			   	'#title'			=> t('Number of other Leave(s) in day(s)'),
																			  	'#default_value' 	=> !empty($other_leave_no) ? $other_leave_no : '0' ,
																			    '#element_validate'	=> array('isnumaric_valiadtion') ,
																				'#attributes'		=> $readonly_other_leave_no
																);



$form['renewal_contractlaour']['earned_leave_no'] 	= array(
				'#prefix' 			=>  '<section class ="col col-6"><label class="input">',
				'#suffix' 			=>  '</label></section></div></fieldset>',
				'#type' 			=> 'textfield',
				'#title'			=> t('Number of Earned Leave(s) in day(s)'),
				'#default_value' 	=> !empty($earned_leave_no) ? $earned_leave_no : '0' ,
				'#element_validate'	=> array('isnumaric_valiadtion'),
				'#attributes'		=> $readonly_eraned_leave_no 
																	);
	 $form['renewal_contractlaour']['old_license_no'] = array(
				'#prefix' 		=> '<fieldset><legend>7.Other Information</legend><div class="row"><section class="col col-4"><label class="input">',
				'#suffix' 		=> '</label></section>',
				'#title'		=> 'A(i)License Number',
				'#type'	 		=> 'textfield',
				'#default_value'=> !empty($backlog_license_no) ? $backlog_license_no : '',
				'#attributes' 	=> array('placeholder' => 'License Number.'),
				'#required' 	=> TRUE,
				'#attributes'	=> $readonly_backlog_licnse_no
			);
	
		/*$form['renewal_contractlaour']['date_license'] = array(
								'#prefix'				=>'<section class="col col-4"><div id="license_date"><label class="date">',
								'#suffix'				=>'</label></div></section>',
								'#title'				=>'A(ii)Date of License <span class="verified_show con_dob"> </span>',
								'#type' 				=> 'date',
								'#after_build' 			=> array("contractor_forms_format_dob"),
								'#element_validate' 	=> array('contractor_dob_validate'),
								'#required'				=>	TRUE,
								//'#attributes'			=> $readonly_backlog_licnse_date
								//'#attributes' => array('readonly' => 'readonly')
								'#disabled'				=>	TRUE
							);*/	
							
							
		 $form['renewal_contractlaour']['date_license'] 	= array(
							'#prefix' 					=>'<section class="col col-4"><label class="input">',
							'#suffix' 					=> 	'</label></section>',
							'#type' 					=> 	'textfield',
							'#title'					=>	'A(ii)Date of License',
							'#attributes'				=>	$readonly_date_license,
							'#required' 				=> TRUE,
							'#default_value' 			=> 	!empty($backlog_license_date) ? $backlog_license_date : ''
						);								
	/*if($flag_date != 1){
			if(empty($backlog_license_date)){
						
						$form['renewal_contractlaour']['date_license'] = array(
								'#prefix'				=>'<section class="col col-4"><div id="license_date"><label class="date">',
								'#suffix'				=>'</label></div></section>',
								'#title'				=>'A(ii)Date of License <span class="correction_show con_dob"> </span>',
								'#type' 				=> 'date',
								'#after_build' 			=> array("contractor_forms_format_dob"),
								'#element_validate' 	=> array('contractor_dob_validate'),
								'#required'				=>	TRUE,
								
							);
			 }else{
						
						$form['renewal_contractlaour']['date_license'] = array(
								'#prefix'				=>	'<section class="col col-4"><div id="license_date"><label class="date">',
								'#suffix'				=>	'</label></div></section>',
								//'#title'				=>	'Date of License ',	
								'#title'				=>' A(ii)Date of License <span class="correction_show con_dob"> </span>',
								'#type'					=>	'date',
								'#after_build' 			=> 	array("contractor_forms_format_dob"),
								'#element_validate' 	=> 	array('contractor_dob_validate'),
								'#default_value' 		=> 	array(
																   'month' 	=> format_date(strtotime($backlog_license_date), 'custom',  'n'),
																   'day' 	=> format_date(strtotime($backlog_license_date), 'custom',  'j'),
																   'year'	=> format_date(strtotime($backlog_license_date), 'custom',  'Y'),
																),
								
								
							 );
				}
	}if($flag_date == 1){
		if(empty($backlog_license_date)){
						
						$form['renewal_contractlaour']['date_license'] = array(
								'#prefix'				=>'<section class="col col-4"><div id="license_date"><label class="date">',
								'#suffix'				=>'</label></div></section>',
								'#title'				=>'A(ii)Date of License <span class="verified_show con_dob"> </span>',
								'#type' 				=> 'date',
								'#after_build' 			=> array("contractor_forms_format_dob"),
								'#element_validate' 	=> array('contractor_dob_validate'),
								'#required'				=>	TRUE,
								//'#attributes'			=> $readonly_backlog_licnse_date
								//'#attributes' => array('readonly' => 'readonly')
								'#disabled'				=>	TRUE
							);
			 }else{
						
						$form['renewal_contractlaour']['date_license'] = array(
								'#prefix'				=>	'<section class="col col-4"><div id="license_date"><label class="date">',
								'#suffix'				=>	'</label></div></section>',
								//'#title'				=>	'Date of License ',	
								'#title'				=>' A(ii)Date of License <span class="verified_show con_dob"> </span>',
								'#type'					=>	'date',
								'#after_build' 			=> 	array("contractor_forms_format_dob"),
								'#element_validate' 	=> 	array('contractor_dob_validate'),
								'#default_value' 		=> 	array(
																   'month' 	=> format_date(strtotime($backlog_license_date), 'custom',  'n'),
																   'day' 	=> format_date(strtotime($backlog_license_date), 'custom',  'j'),
																   'year'	=> format_date(strtotime($backlog_license_date), 'custom',  'Y'),
																),
								//
								//'#attributes' => array('readonly' => 'readonly')
								'#disabled'				=>	TRUE
							 );
				}
		
		
	}*/
		
		$form['renewal_contractlaour']['license_fees'] = array(
							'#prefix' 			=> '<section class="col col-4"><label class="input">',
							'#suffix' 			=> '</label></section></div>',
							'#title'			=> 'B.License Fees',
							'#type'	 			=> 'textfield',
							'#default_value'	=> !empty($amount_of_licence) ? $amount_of_licence : '',
							'#element_validate' 	=> 	array('isnumaric_valiadtion'),
							
							'#required' 	=> TRUE,
							'#attributes'	=>	$readonly_license_amount
										);
										
	$form['renewal_contractlaour']['security_renewal'] = array(
										'#prefix' 			=> '<div class="row"><section class="col col-4"><label class="input">',
										'#suffix' 			=> '</label></section>',
										'#title'			=>	'C.Security fees of License',
										'#size'				=>	t('20'),	
										'#type' 			=> 'textfield',
										'#element_validate' 	=> 	array('isnumaric_valiadtion'),
										'#default_value'	=>	!empty($security_deposit) ? $security_deposit : '',
										'#required' 		=> TRUE,
										'#attributes'		=> $readonly_security_amount
					);	
	$form['renewal_contractlaour']['backlog_renewal_fees'] = array(
										'#prefix' 		=> '<section class="col col-4"><label class="input">',
										'#suffix' 		=> '</label></section>',
										'#title'		=> 'D.Fees of  Last Renewal',
										'#type'	 		=> 'textfield',
										'#default_value'=> !empty($backlog_renewal_fees) ? $backlog_renewal_fees : '0.00',
										'#attributes' 	=>	$readonly_backlog_renewal_amount ,
										'#element_validate' 	=> 	array('isnumaric_valiadtion'),
										'#required' 	=> TRUE,
									);
	$form['renewal_contractlaour']['last_renewal_date'] 	= array(
							'#prefix' 					=> 	'<section class="col col-4"><label class="input">',
							'#suffix' 					=> 	'</label></section></div>',
							'#type' 					=> 	'textfield',
							'#title'					=>	'E.Date of expiry of the previous license',
							'#attributes'				=>	$readonly_backlog_renewal_date,
							'#default_value' 			=> 	!empty($backlog_renewal_date) ? date('d-m-Y',strtotime($backlog_renewal_date)) : '',
							'#required' 				=> TRUE,
						);	
				
	
	
	

	
	
	
										
		
	$form['renewal_contractlaour']['special_benefits'] = array(
										'#prefix' 		=> '<section ><label class="textarea">',
										'#suffix' 		=> '</label></section>',
										'#title'		=> 'F.Special benefits provided,if any',
										'#rows'			=>	'2',
										'#type'	 		=> 'textarea',
										'#default_value'=> !empty($special_benifites) ? $special_benifites : '',
										'#attributes' 	=> $readonly_special_benifites,
										'#required' 	=> TRUE,
										);
//print_r($readonly_special_benifites);die();
	$form['renewal_contractlaour']['state_insurance'] = array(
										'#prefix' 		=> '<div class="row"><section class="col col-6"><label class="textarea">',
										'#suffix' 		=> '</label></section>',
										'#title'		=> 'G.Contribution made under the Employees Satae Insurance Act,1948',
										'#row'			=>	2,
										'#type'	 		=> 'textarea',
										'#default_value'=> !empty($state_insurance) ? $state_insurance : '',										
										//'#attributes' 	=> array('placeholder' => 'Maximum number of Contract Labour proposed .'),
										'#required' 	=> TRUE,
										//'#attributes' 	=> $readonly_special_benifites,
										'#attributes'	=>	$readonly_state_insurance
										);
	$form['renewal_contractlaour']['miscellaneous'] = array(
										'#prefix' 		=> '<section class="col col-6"><label class="textarea">',
										'#suffix' 		=> '</label></section></div>',
										'#title'		=> 'H.Contribution made under the Employees Provident Fund and Miscellaneous Provision Act,1952',
										'#row'			=>	2,
										'#type'	 		=> 'textarea',
										'#default_value'=> !empty($miscellaneous_provisions) ? $miscellaneous_provisions : '',
										'#attributes' 	=> array('placeholder' => 'Maximum number of Contract Labour proposed .'),
										'#required' 	=> TRUE,
										'#attributes'	=>	$readonly_miscellaneous
										);

$form['renewal_contractlaour']['convicted_markup'] = array(
											'#prefix' 		=>  '<section ><label class="input">',
											'#suffix' 		=>  '</label></section>', 
											'#markup' 		=>  t('I.Whether the contractor was convicted of any offence within the preceeding five years.If so, give details:<span class='.$readonly_offence_test.'></span>'), 
											'#type' 		=>  'markup',
											
								 
										);



	$form['renewal_contractlaour']['contractor_convicted'] = array(
											
											'#prefix' 				=> '<section><label class="inline-group">',
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
								
									
	$form['renewal_contractlaour']['details_contractor_convicted'] = array(
											'#prefix' 			=>  '<div id="add_convicted">',
											'#suffix' 			=>  '</div>',
												
									);
										
	$value_contractor_convicted 	= 	isset($form_state['values']['contractor_convicted']) ? $form_state['values']['contractor_convicted'] :$contractor_convicted;
	if(!empty($value_contractor_convicted)&&($value_contractor_convicted==1)){
											
	$form['renewal_contractlaour']['details_contractor_convicted'] = array(
											'#prefix' 			=>  '<div id="add_convicted"><section ><label class="textarea">',
											'#suffix' 			=>  '</label></section></div>',
											'#title'			=>	'Details contractor was convicted of any offence within the preceeding five years:',
											'#rows'				=>	2,	
											'#type' 			=> 	'textarea',
											'#default_value' 	=>	!empty($details_contractor_convicted) ? $details_contractor_convicted : '',
											
											
									);
	}
 
$form['renewal_contractlaour']['revoking_markup'] = array(
											'#prefix' 		=>  '<section><label class="input">',
											'#suffix' 		=>  '</label></section>', 
											'#markup' 		=>  t('J.Whether there was any order against the contract or revoking or suspending license or forfeiting security deposit in respect of an earlier contract. If so, the date of such order::<span class='.$readonly_revoking_test.'></span>'), 
											'#type' 		=>  'markup',
											
								 
										);
																		
	$form['renewal_contractlaour']['contractor_revoking'] = array(
										  '#prefix' 			=>  '<section><label class="inline-group">',
										  '#suffix' 			=>  '</label></section>',
									
										  '#type' 				=>  'radios',
										  '#options' 			=>  array('0'=>t('No'),'1'=>t('Yes')),
										  '#default_value' 		=> !empty($contractor_revoking) ? $contractor_revoking : 0,
										  '#attributes'			=>  $readonly_revoking,
										  '#ajax'				=>  array(
																			'event' 			=> 'change',
																			'effect' 			=> 'fade',			
																			'callback'			=> 'get_revoking',
																			'progress'			=> '',
																		 ) 

		     						);
																
	$form['renewal_contractlaour']['details_contractor_revoking'] = array(
											'#prefix' 			=>  '<div id="add_revoking">',
											'#suffix' 			=>  '</div>',
												
									);	
									
	
									
$value_contractor_revoking 	= 	isset($form_state['values']['contractor_revoking']) ? $form_state['values']['contractor_revoking'] :$contractor_revoking;
														
if(!empty($value_contractor_revoking) && ($value_contractor_revoking='1')){
  if(empty($details_contractor_revoking)){ 
	$form['renewal_contractlaour']['details_contractor_revoking'] = array(
											  '#prefix' 			=>  '<div id="add_revoking"><section><label class="date">',
											  '#suffix' 			=>  '</label></section></div>',
											  '#title'				=>	'The date of such order:',
											  '#type' 				=>  'date',
											  '#attributes'			=>	$readonly_contractor_revoking_date,
											  '#after_build' 		=> array("contractor_forms_format_dob"),
												
											);

  }
  else{
				
			$form['renewal_contractlaour']['details_contractor_revoking'] = array(
												'#prefix' 			=>  '<div id="add_revoking"><section><label class="date">',
											  	'#suffix' 			=>  '</label></section></div>',
												'#title'			=>	'The date of such order: ',
												'#type'				=>	'date',
												'#after_build' 		=> array("contractor_forms_format_dob"),
												
										  		'#default_value' 	=> array(
																			 'month' 	=> format_date(strtotime($details_contractor_revoking), 'custom',  'n'),
																			 'day' 		=> format_date(strtotime($details_contractor_revoking), 'custom',  'j'),
																			 'year' 	=> format_date(strtotime($details_contractor_revoking), 'custom',  'Y'),
																		),
												//'#attributes' 		=> array('class' => array('datefieldapp')),
												'#attributes'		=>	$readonly_contractor_revoking_date 
											  );
				}
				
}

	
$form['renewal_contractlaour']['previous_markup'] = array(
											'#prefix' 		=>  '<section><label class="input">',
											'#suffix' 		=>  '</label></section>', 
											'#markup' 		=>  t('K.Whether the contractor has worked in any other establishment within the past five years. If so. give details of the principal employer establishment and nature of work::<span class='.$readonly_pervious_test.'></span>'), 
											'#type' 		=>  'markup',
											
								 
										);
		
																		
	$form['renewal_contractlaour']['previous_employer'] = array(
												'#prefix' 			=>  '<section ><label class="inline-group">',
												'#suffix' 			=>  '</label></section>',
												
											   	'#type' 			=> 	'radios',
											    '#options'			=>	array('0'=>t('No'),'1'=>t('Yes')),
											   	'#default_value' 	=>	!empty($contractor_previous_employer) ? $contractor_previous_employer : 0,
											   	'#attributes'		=> 	$readonly_pervious_license,
											  	'#ajax'				=>  array(
																				'event' 			=> 'change',
																				'effect' 			=> 'fade',			
																				'callback'			=> 'get_liecnse',
																				'progress'			=> '',
																		) 
				
     									 );
															  
															  

															  
															  
$value_previous_employer 	= 	isset($form_state['values']['previous_employer']) ? $form_state['values']['previous_employer'] :$contractor_previous_employer;
$form['renewal_contractlaour']['add_license_number'] = array(
      									 '#prefix' 			=>  '<div id="add_license">',
										 '#suffix' 			=>  '</div>',
		
 									);	
$form['renewal_contractlaour']['add_reg_no_pe'] = array(
      								 	'#prefix' 		=>  '<div id="add_reg_no">',
										'#suffix' 		=>  '</div>',
  								 );														  
															  
if($value_previous_employer=='1'){														  
$form['renewal_contractlaour']['add_license_number'] = array(
										'#type' 			=> 'textfield',
										'#title' 			=> t('Give your Previous License Number'),
										'#prefix' 			=>  '<div id="add_license"><section class ="col col-6"><label class="input">',
										'#suffix' 			=>  '</label></section></div>',
										'#default_value' 	=> !empty($previous_license_number_list) ? $previous_license_number_list : '',
										'#attributes'		=> 	$readonly_previous_license,
										'#description'		=>'<font color="#FF0000"> If you have  multiple license number, provide your license number separated by comma(",")</font>'
										
									);
	
$form['renewal_contractlaour']['add_reg_no_pe'] = array(
										'#type' 			=> 'textfield',
										'#title' 			=> t('Give Registration Number of Principal Employer'),
										'#prefix' 			=>  '<div id="add_reg_no"><section class ="col col-6"><label class="input">',
										'#suffix' 			=>  '</label></section></div>',
										'#default_value' 	=>!empty($reg_no_principal_employeer_number_list) ? $reg_no_principal_employeer_number_list   : '',
										'#attributes'		=> 	$readonly_previous_reg,
										'#description'		=>'<font color="#FF0000"> If you have multiple Reg. No. of Principal Employer, provide Reg. No. separated by comma(",")</font>'
					
								);
}
																	
															
if(($renewal_application_final_status=='F' && $renewal_appliaction_status=='B')||($renewal_application_final_status=='' && $renewal_appliaction_status=='')){
	$form ['renewal_contractlaour']['submit'] = array (
							   '#prefix' 					=> '</fieldset><footer>',  
							  '#suffix' 					=> '</footer></div></div>', 
							  '#attributes' 				=> array('class' => array('button')),
							  '#type' 						=> 'submit',
							  '#value'						 => 'Save & Continue'
					  );
}
else{
	$form ['renewal_contractlaour']['end_div'] = array (
							  						'#prefix' 			=> '</fieldset>',  
							  						'#suffix' 			=> '</div></div>',							 
							  						'#type'				=> 'markup'
							 
					  );

}
														  
															  

	}else{
		drupal_goto('applicant-login');
	}
			
	return $form;
}




function get_manager_address($form, $form_state){
	$commands 	= array();
	
	$commands[] = ajax_command_replace('#manager_subdvsn_replace', drupal_render($form['renewal_contractlaour']['manager_subdivision']));
	$commands[] = ajax_command_replace('#manager_areatype_replace', drupal_render($form['renewal_contractlaour']['manager_areatype']));
	$commands[] = ajax_command_replace('#name_manager_areatype_replace', drupal_render($form['renewal_contractlaour']['name_manager_areatype']));
	$commands[] = ajax_command_replace('#manager_vill_ward_replace', drupal_render($form['renewal_contractlaour']['manager_vill_ward']));
	
	$commands[] = ajax_command_replace('#manager_ps_replace', drupal_render($form['renewal_contractlaour']['manager_ps']));
	
  	return array('#type' => 'ajax', '#commands' => $commands);
}

function get_liecnse($form, $form_state) {
		$commands = array();
		$commands[] = ajax_command_replace('#add_license', drupal_render($form['renewal_contractlaour']['add_license_number']));
		$commands[] = ajax_command_replace('#add_reg_no', drupal_render($form['renewal_contractlaour']['add_reg_no_pe']));
		
  		return array('#type' => 'ajax', '#commands' => $commands);
}

function get_revoking($form, $form_state) {
		$commands = array();
		$commands[] = ajax_command_replace('#add_revoking', drupal_render($form['renewal_contractlaour']['details_contractor_revoking']));
		return array('#type' => 'ajax', '#commands' => $commands);
}

function get_convicted($form, $form_state) {
		$commands 		= array();
		$commands[] 	= ajax_command_replace('#add_convicted', drupal_render($form['renewal_contractlaour']['details_contractor_convicted']));
		return array('#type' => 'ajax', '#commands' => $commands);
}



function custom_user_district_list_new() {
	$default_select 	= array(""=>"- Select District -");
	$result 			= db_query("SELECT district_code,district_name FROM {district_master} where is_active = 'Y' order by district_name");
	$list 				= array();
	foreach ($result as $record) {
		$list[$record->district_code] = t('@districtname', array(
			'@districtname' => $record->district_name,
		));
	}
	return ($default_select+$list);
}


function isnumaric_valiadtion($element, &$form_state){
	
  
  	$val			=	$form_state['values'];
	$txtValue		=	trim($element['#value']);
	if($txtValue != '' && !is_numeric($txtValue)) {
		form_error($element, t($element['#title'].' SHOULD BE VALID'));
		$form_state['rebuild'] = TRUE;
	}

	
}
function renewal_particular_data_validate($form, &$form_state) {
	
	$val										=	$form_state['values'];
	$match_pattern 								= 	'/[()<>#@\$\%\\_\:]+/';
	
	$unskilled_wages_value						=	$val['unskilled_wages']; 
	$unskilled_rate_wages						=	$val['unskilled_rate_wages'];
	$contractor_revoking						=	$val['contractor_revoking'];	
	$previous_employer							=	$val['previous_employer']; 
	$add_license_number							=	$val['add_license_number'];
	$add_reg_no_pe								=	$val['add_reg_no_pe'];
	
	if($previous_employer=='1'){
		if(empty($add_license_number)&& empty($add_reg_no_pe)){
			form_set_error('add_license_number',t('Please give vaild either License Number of Registration Number'));			
		}		
	}
	
	if(preg_match($match_pattern, $add_license_number)) {									
		form_set_error('add_license_number', t('License Number Should be valid'));
	}
	if(preg_match($match_pattern, $add_reg_no_pe)){
		form_set_error('add_reg_no_pe', t('Registration Number Should be valid'));
	}
	
	if($unskilled_wages_value=='1'){		
		if($unskilled_rate_wages==''){
			form_set_error($i, t('Please give value of Unskilled rate of wages'));
		}		
	}
	
	$semiskilled_wages_value = $val['semiskilled_wages']; 
	$semiskilled_rate_wages	 = $val['semiskilled_rate_wages'];
	
	if($semiskilled_wages_value=='1'){
		
	if($semiskilled_rate_wages==''){
			form_set_error('semiskilled_rate_wages', t('Please give value of Semiskilled Rate of wages'));
		}
		
		
	}
	
	$skilled_wages_value					=	$val['skilled_wages']; 
	$skilled_rate_wages						=	$val['skilled_rate_wages'];
	
	
	if($skilled_wages_value=='1'){
		
		if($skilled_rate_wages==''){
			form_set_error('skilled_rate_wages', t('Please give value of Skilled Rate of wages'));
		}
		
	}
	
		$highlyskilled_wages_value					=	$val['highlyskilled_wages']; 
		$highlyskilled_rate_wages				=	$val['highlyskilled_rate_wages'];
	
		
		if($highlyskilled_wages_value=='1'){
		
		if($highlyskilled_rate_wages==''){
			form_set_error('highlyskilled_daily_da_wages', t('Please give value of Highlyskilled Rate of wages'));
		}
		
		}
		
		$weekly_holiday_value					=	$val['weekly_holiday']; 
		$no_holiday_value						=	$val['no_holiday'];
		
		if($weekly_holiday_value=='1'){
		foreach($no_holiday_value as $rows){
			if(empty($rows)){
				form_set_error('no_holiday', t('Please Select Holiday(s)'));
				break;
			}
		}
		
		}
		$annual_leave_value							=	$val['annual_leave'];
		$annual_leave_no_value						=	$val['annual_leave_no']; 
		if($annual_leave_value=='1'){
		
		if($annual_leave_no_value=='0' || $annual_leave_no_value==""){
			form_set_error('annual_leave_no', t('Please give the Number of Anuual Leave'));
		}
	}
		$casual_leave_value							=	$val['casual_leave'];
		$casual_leave_no_value						=	$val['casual_leave_no']; 
		if($casual_leave_value=='1'){
		
		if($casual_leave_no_value=='0' || $casual_leave_no_value==""){
			form_set_error('casual_leave_no', t('Please give the Number of Casual Leave'));
		}
	}
		$sick_leave_value							=	$val['sick_leave'];
		$sick_leave_no_value						=	$val['sick_leave_no']; 
		if($sick_leave_value=='1'){
		
		if($sick_leave_no_value=='0' || $sick_leave_no_value==""){
			form_set_error('sick_leave_no', t('Please give the Number of Sick Leave'));
		}
	}
		$maternity_leave_value						=	$val['maternity_leave'];
		$maternity_leave_no_value					=	$val['maternity_leave_no']; 
		if($maternity_leave_value=='1'){
		
		if($maternity_leave_no_value=='0' || $maternity_leave_no_value==""){
			form_set_error('maternity_leave_value', t('Please give the Number of Maternity Leave'));
		}
	}
		$other_leave_value							=	$val['other_leave'];
		$other_leave_no_value					=	$val['other_leave_no']; 
		if($other_leave_value=='1'){
		
		if($other_leave_no_value=='0' || $other_leave_no_value==""){
			form_set_error('other_leave_no', t('Please give the Number of Other Leave'));
		}
	}
}


function renewal_particular_data_submit($form, &$form_state){
	 	global $base_root, $base_path, $user;	
	
		$contractor_id 					= $user->uid;		
		$val 							= $form_state['values'];		
		$contractor_id 					= $user->uid;
		$flag							= $val['flag'];
		$contract_natureofwork			= $val['contract_natureofwork'];
		$applicant_id					= $val['applicant_id'];
		$application_id					= $val['application_id'];
		$name_of_agent					= $val['contract_agnt_name'];
		$address_of_manager 			= $val['contract_agnt_add'];
		$manager_pin					= $val['contractor_pin'];
		$contractor_convicted 			= $val['contractor_convicted'];
		$details_contractor_convicted 	= $val['details_contractor_convicted'];
		$contractor_revoking			= $val['contractor_revoking'];
		$is_extra_fees					= $val['is_extra_fees'];
		
		if(!empty($contractor_revoking) && $contractor_revoking=='1'){		
			$details_contractor_revoking			= $val['details_contractor_revoking']['year']."-".$val['details_contractor_revoking']['month']."-".$val['details_contractor_revoking']['day'];
		}
		$previous_employer 							= $val['previous_employer'];	
		$user_id									= $applicant_id;
		$act_id 									= 1;
		$amount_of_renewal							= $val['fees'];
		$amount_of_security_deposit					= $val['security_renewal'];
		$fees_value									= $val['fees_value'];
		$rowcount									= $val['rowcount'];								
		$contract_labour_id							= $val['contract_labour_id'];
		$contractor_max_no_of_labours_on_any_day 	= $val['noof_contractlabour'];
		$contractor_dist 							= $val['manager_dist'];
		$contractor_subdivision 					= $val['manager_subdivision'];
		$contractor_areatype 						= $val['manager_areatype'];
		$contractor_name_areatype 					= $val['name_manager_areatype'];
		$contractor_vill_ward 						= $val['manager_vill_ward'];				
		$contractor_ps								= $val['manager_ps'];
		$serial_no									= $val['serial_no'];
		$con_info_id								= $val['con_info_id'];
		
		$editable_work								= $val['editable_work'];
		$editable_agent								= $val['editable_agent'];
		$editable_address_manager					= $val['editable_address_manager'];
		$editable_max_no_day						= $val['editable_max_no_day'];
		$editable_convicted							= $val['editable_convicted'];
		$editable_previous_employer					= $val['editable_previous_employer'];
		$editable_revoking							= $val['editable_revoking'];		
		$contract_agnt_name_edit					= $val['contract_agnt_name_edit'];
		$manager_add_edit							= $val['manager_add_edit'];
		$manager_dist_edit							= $val['manager_dist_edit'];
		$manager_subdivision_edit					= $val['manager_subdivision_edit'];
		$manager_areatype_edit						= $val['manager_areatype_edit'];
		$name_manager_areatype_edit					= $val['name_manager_areatype_edit'];
		$manager_vill_ward_edit						= $val['manager_vill_ward_edit'];
		
		$manager_ps_edit							= $val['manager_ps_edit'];
		
		
		$noof_contractlabour_edit					= $val['noof_contractlabour_edit'];
		$fees_edit									= $val['fees_edit'];
		$security_edit								= $val['security_edit'];
		$convicted_edit								= $val['convicted_edit'];
		$convicted_details_edit						= $val['convicted_details_edit'];
		$revoking_edit								= $val['revoking_edit'];
		$revoking_details_edit						= $val['revoking_details_edit'];
		$previous_employer_edit						= $val['previous_employer_edit'];		
		$natureofwork_edit							= $val['natureofwork_edit'];
		$fees_linces_test							= $val['fees_linces_test'];				
		$category_designation						= $val['category_designation'];		
		
		
		$unskilled_rate_wages						= $val['unskilled_rate_wages'];
				
		$semiskilled_rate_wages						= $val['semiskilled_rate_wages'];
		
			
		$skilled_rate_wages							= $val['skilled_rate_wages'];
		
						
		$highlyskilled_rate_wages					= $val['highlyskilled_rate_wages'];
		
	
		
		$hours_work									= $val['hours_work'];
		$overtime									= $val['overtime'];
		$overtime_wages								= $val['overtime_wages'];
		$weekly_holiday								= $val['weekly_holiday'];
		$no_holiday									= $val['no_holiday'];
		
		$annual_leave								= $val['annual_leave'];
		$annual_leave_no							= $val['annual_leave_no'];
		$casual_leave								= $val['casual_leave'];
		$casual_leave_no							= $val['casual_leave_no'];
		$sick_leave									= $val['sick_leave'];
		$sick_leave_no								= $val['sick_leave_no'];
		$maternity_leave							= $val['maternity_leave'];
		$maternity_leave_no							= $val['maternity_leave_no'];
		$other_leave								= $val['other_leave'];
		$other_leave_no								= $val['other_leave_no'];
		$add_license_number							= $val['add_license_number'];
		$add_reg_no_pe								= $val['add_reg_no_pe'];
		$serial										= $val['serial'];
		$old_license_no								= $val['old_license_no'];
		$date_license								= $val['date_license'];
		$last_renewal_date							= $val['last_renewal_date'];
		$license_fees								= $val['license_fees'];
		$backlog_renewal_fees						= $val['backlog_renewal_fees'];
		$special_benefits							= $val['special_benefits'];
		$contribution								= $val['contribution'];
		
		$holidaywages								= $val['holidaywages'];
		$spared_over								= $val['spared_over'];	
		$state_insurance							= $val['state_insurance'];	
		$miscellaneous								= $val['miscellaneous'];
		$earned_leave_no							= $val['earned_leave_no'];
		$renewal_security_new						= $val['renewal_security_new'];	
		$recommand_date_expire						= $val['recommand_date_expire'];
		$renewal_appliaction_status					= encryption_decryption_fun('decrypt', $val['renewal_appliaction_status']); 
		
		
		if($amount_of_security_deposit>$renewal_security_new){
			 $payble_security_ammount 	= 0.00;
             $balance_security_ammount = $amount_of_security_deposit-$renewal_security_new;
		}if($amount_of_security_deposit<$renewal_security_new){
			 $payble_security_ammount 	= $renewal_security_new-$amount_of_security_deposit;
             $balance_security_ammount = 0.00;
		}if($amount_of_security_deposit==$renewal_security_new){
			 $payble_security_ammount 	= 0.00;
             $balance_security_ammount = 0.00;
		}	
		
	if($license_fees>$amount_of_renewal)
		   $renewal_deposite_fees1 = $license_fees;
		if($license_fees<$amount_of_renewal)
		   $renewal_deposite_fees1 = $amount_of_renewal;
		if($license_fees == $amount_of_renewal)
		   $renewal_deposite_fees1 = $amount_of_renewal;
		   
		 if($renewal_deposite_fees1>$backlog_renewal_fees) 
		  		$renewal_deposite_fees = $renewal_deposite_fees1;
		 if($renewal_deposite_fees1<$backlog_renewal_fees) 
		  		$renewal_deposite_fees = $backlog_renewal_fees;
		if($renewal_deposite_fees1==$backlog_renewal_fees) 
		  		$renewal_deposite_fees = $backlog_renewal_fees;
		   
		   
		   
		if($amount_of_security_deposit>$renewal_security_new)
		   $deposited_max_security_fees = $amount_of_security_deposit;
		if($amount_of_security_deposit<$renewal_security_new)
		   $deposited_max_security_fees = $renewal_security_new;
		if($amount_of_security_deposit == $renewal_security_new)
		   $deposited_max_security_fees = $amount_of_security_deposit;	
				
		//print_r($backlog_renewal_fees);die();
	if(!empty($recommand_date_expire)){
	if(strtotime($recommand_date_expire)== strtotime($last_renewal_date)){

	if($previous_employer=='1'){		
		$previous_license 					= 	db_select('l_contrcator_previous_license', 'previous_license');
  		$previous_license					->	fields('previous_license',array('perivious_license_no'));
		$previous_license					->	condition('previous_license.serial_no_form_v',$serial,'=');
		$previous_license					->	condition('previous_license.created_by',$contractor_id,'='); 
		$previous_license					->	condition('previous_license.perivious_license_no', 'NULL', '!=');
		$previous_license_result			=	$previous_license->execute()->fetchAll();
		
		
		$reg_no_principal 					= 	db_select('l_contrcator_previous_license', 'previous_license');
		$reg_no_principal					->	fields('previous_license',array('reg_no_principal_employeer'));
		$reg_no_principal					->	condition('previous_license.serial_no_form_v',$serial,'=');
		$reg_no_principal					->	condition('previous_license.created_by',$contractor_id,'='); 
		$reg_no_principal					->	condition('previous_license.reg_no_principal_employeer', 'NULL', '!=');
	
		$reg_no_principal_result 			= 	$reg_no_principal->execute()->fetchAll();
		
		
		if(empty($previous_license_result)){
			if(!empty($add_license_number)){
				//$add_license_list = substr($add_license_number, 0, -1);
				$add_license_lists=explode(',', $add_license_number);					
				foreach($add_license_lists as $row){ 
					$add_more_license = array( 
											'perivious_license_no' => $row,
											'created_by'		   => $contractor_id,
											'created_date'		   => date("Y-m-d"),
											'serial_no_form_v'	   =>  $serial
											);														  
																			
					db_insert('l_contrcator_previous_license')->fields($add_more_license)->execute();
				}
			}
		}else{
			 $del_query			=		db_delete('l_contrcator_previous_license')->condition('serial_no_form_v', $serial);
			 $del_query			->		condition('created_by',$contractor_id,'=');
			 $del_query			->		isNotNull('perivious_license_no');
			 $del_query			->		execute();
			
			if(!empty($add_license_number)){
				//$add_license_list = substr($add_license_number, 0, -1);
				$add_license_lists=explode(',', $add_license_number);
					
				foreach($add_license_lists as $row){ 
					$add_more_license = array( 
											'perivious_license_no'		=>	$row,
											'created_by'				=>	$contractor_id,
											'created_date'				=>	date("Y-m-d"),
											'serial_no_form_v'			=>	$serial
											);
					db_insert('l_contrcator_previous_license')->fields($add_more_license)->execute();
				}
			}			
		}
		
		if(empty($reg_no_principal_result)){
			if(!empty($add_reg_no_pe)){
				//$add_reg_list = substr($add_reg_no_pe, 0, -1);
				$add_reg_lists=explode(',', $add_reg_no_pe);
				foreach($add_reg_lists as $row){ 
					$add_more_reg_pe_no = array( 
										'reg_no_principal_employeer'		=>	$row,
										'created_by'						=>	$contractor_id,
										'created_date'						=>	date("Y-m-d"),
										'serial_no_form_v'					=>	$serial
										);
					db_insert('l_contrcator_previous_license')->fields($add_more_reg_pe_no)->execute();	
				}
			}					
		}else{
			$del_query_reg		=	db_delete('l_contrcator_previous_license');   
			$del_query_reg		->	condition('serial_no_form_v', $serial);
			$del_query_reg		->	condition('created_by',$contractor_id,'=');
			$del_query_reg		->	isNotNull('reg_no_principal_employeer');
			$del_query_reg		->	execute();
			
			if(!empty($add_reg_no_pe)){
			//	$add_reg_list = substr($add_reg_no_pe, 0, -1);
				$add_reg_lists=explode(',', $add_reg_no_pe);
					
				foreach($add_reg_lists as $row){ 
					$add_more_reg_pe_no = array( 
											'reg_no_principal_employeer'		=>	$row,
											'created_by'						=>	$contractor_id,
											'created_date'						=>	date("Y-m-d"),
											'serial_no_form_v'					=>	$serial
											);
					db_insert('l_contrcator_previous_license')->fields($add_more_reg_pe_no)->execute();						
						
				}
			}
		}
	}
	 
	
	if(empty($previous_employer)){		
		$previous_license = db_select('l_contrcator_previous_license', 'previous_license');
  		$previous_license->fields('previous_license',array('perivious_license_no'));
		$previous_license->condition('previous_license.serial_no_form_v',$serial,'=');
		$previous_license->condition('previous_license.created_by',$contractor_id,'=');
		$previous_license->condition('previous_license.perivious_license_no', 'NULL', '!=');
		$previous_license_result = $previous_license->execute()->fetchAll();
		
		
		$reg_no_principal = 	db_select('l_contrcator_previous_license', 'previous_license');
		$reg_no_principal->fields('previous_license',array('reg_no_principal_employeer'));
		$reg_no_principal->condition('previous_license.serial_no_form_v',$serial,'=');
		$reg_no_principal->condition('previous_license.created_by',$contractor_id,'=');
		$reg_no_principal->condition('previous_license.reg_no_principal_employeer', 'NULL', '!=');
	
		$reg_no_principal_result = $reg_no_principal->execute()->fetchAll();
		
		
		if(!empty($previous_license_result)){
									db_delete('l_contrcator_previous_license')  
																 ->	condition('serial_no_form_v', $serial)
																 ->	condition('perivious_license_no', 'NULL', '!=')
																 ->	condition('created_by',$contractor_id,'=') 
																 -> execute();
					
		}
		if(!empty($reg_no_principal_result)){
			 					db_delete('l_contrcator_previous_license')   
																->	condition('serial_no_form_v', $serial)
																->	condition('reg_no_principal_employeer', 'NULL', '!=') 
																 ->	condition('created_by',$contractor_id,'=') 
																->	execute();
					
					

		}
		
	}
	
		
		if($editable_work=='1'){
			$contract_natureofwork	=	$natureofwork_edit;
			
			$contract_natureofwork = explode(',', $natureofwork_edit);
		}
		if($editable_agent=='1'){
			$name_of_agent			=	$contract_agnt_name_edit;
		}
		
		if($editable_address_manager=='1'){
			$address_of_manager			=	$manager_add_edit;			
			$contractor_dist			=	$manager_dist_edit;
			$contractor_subdivision		=	$manager_subdivision_edit;
			$contractor_areatype		=	$manager_areatype_edit;
			$contractor_name_areatype	=	$name_manager_areatype_edit;
			$contractor_vill_ward		=	$manager_vill_ward_edit;
			$contractor_ps				=	$manager_ps_edit;		
		}
	
		/*if($flag=='1'){
			if($editable_max_no_day=='1'){
				$contractor_max_no_of_labours_on_any_day	=	$noof_contractlabour_edit;
				$amount_of_licence							=	$fees_edit;
				
			}else{
				
				$amount_of_licence							=	$amount_of_licence;
			}
		}*/
		if($is_extra_fees	==	1){
						$amount_of_renewal		=	$amount_of_renewal;
						$increnent_fees			=	(125/100)*$amount_of_renewal;
				}else{
						$amount_of_renewal		=	$amount_of_renewal;
						$increnent_fees			=	$amount_of_renewal;
				}
				
		
		if($editable_convicted=='1'){
			$contractor_convicted			=	$convicted_edit;
			$details_contractor_convicted	=	$convicted_details_edit;
		}
		if($editable_revoking == '1'){
			$contractor_revoking			=	$revoking_edit;				
			if(!empty($revoking_details_edit)){
				$details_contractor_revoking = $revoking_details_edit;
			}
		}			
		
		if($editable_previous_employer=='1'){
			$contractor_previous_employer			=	$previous_employer_edit;	
			$details_previous_employer				=	$previous_employer_details_edit;
		}
		$no_holidays="";
		if(empty($no_holiday)){
			$no_holidays="";
		}else{
		foreach($no_holiday as $row){
			$no_holidays.=$row.',';
		}
		}

        $contract_labour=array(
								'contractor_user_id'						=>	$contractor_id,
								'name_of_agent'								=>	$name_of_agent,
								'address_of_manager'						=>	$address_of_manager, 
								'contractor_convicted'						=>	$contractor_convicted,
								'details_contractor_convicted'				=>	$details_contractor_convicted,
								'contractor_revoking'						=>	$contractor_revoking,
								'details_contractor_revoking'				=>	$details_contractor_revoking,
								'contractor_previous_employer'				=>	$previous_employer,
								'details_previous_employer'					=>	$previous_employer_details,
								'user_id'									=>	$user_id,
								'act_id'									=>	$act_id,
								'amount_of_licence'							=>	$license_fees,
								'amount_of_security_deposit'				=>	$amount_of_security_deposit,
								'contractor_manager_dist'					=>	$contractor_dist,
								'contractor_manager_subdivision'			=>	$contractor_subdivision ,
								'contractor_manager_areatype'				=>	$contractor_areatype,
								'contractor_manager_name_areatype'			=>	$contractor_name_areatype ,
								'contractor_managerr_vill_ward'				=>	$contractor_vill_ward,
								'contractor_manager_ps'						=>	$contractor_ps,
								'manager_pin'								=>	$manager_pin,
								'contractor_max_no_of_labours_on_any_day'	=>	$contractor_max_no_of_labours_on_any_day,								
								'category_designation'						=>	$category_designation,								
								'unskilled_wages'							=>	!empty($unskilled_rate_wages)?1:0,
								'unskilled_rate_wages'						=>	$unskilled_rate_wages,								
								'semiskilled_wages'							=>	!empty($semiskilled_rate_wages)?1:0,
								'semiskilled_rate_wages'					=>	$semiskilled_rate_wages,								
								'skilled_wages'								=>	!empty($skilled_rate_wages)?1:0,
								'skilled_rate_wages'						=>	$skilled_rate_wages,								
								'highlyskilled_wages'						=>	!empty($highlyskilled_rate_wages)?1:0,
								'highlyskilled_rate_wages'					=>	$highlyskilled_rate_wages,								
								'hours_work'								=> 	$hours_work,
								'overtime'									=> 	$overtime,
								'overtime_wages'							=> 	$overtime_wages,
								'weekly_holiday'							=> 	$weekly_holiday,
								'no_holiday'								=> 	$no_holidays,		
								'annual_leave'								=> 	!empty($annual_leave_no)?1:0,
								'annual_leave_no'							=> 	$annual_leave_no,
								'casual_leave'								=>	!empty($casual_leave_no)?1:0,
								'casual_leave_no'							=>	$casual_leave_no,
								'sick_leave'								=> 	!empty($sick_leave_no)?1:0,
								'sick_leave_no'								=> 	$sick_leave_no,
								'other_leave'								=> 	!empty($other_leave_no)?1:0,
								'other_leave_no'							=> 	$other_leave_no,
								'maternity_leave'							=>	!empty($maternity_leave_no)?1:0,
								'maternity_leave_no'						=> 	$maternity_leave_no	,
								
								
														
         					);						
							
		$fetch_clra_details		=	get_details_clra_master($application_id);//----miscellaneous module
	
					
	
		


	
if($rowcount>0){	

	$query_contract_labour 		= 	db_update('l_particular_of_contract_labour');
 	$query_contract_labour		->	fields($contract_labour);
 	$query_contract_labour		->	condition('contractor_user_id',trim($contractor_id));
	$query_contract_labour		->	condition('serial_no_from_v',trim($serial_no));
 	$query_contract_labour		->	execute();
	
	$last_particulart_id 		= 	db_query('SELECT MAX(id) FROM l_particular_of_contract_labour where contractor_user_id ='.$contractor_id.'and serial_no_from_v ='.$serial_no)->fetchField();
	
	$array_license	=	array(
								'contractor_user_id'		=>	$contractor_id,
								'user_id'					=>	$applicant_id,
								'act_id'					=>	1, 
								'serial_no_from_v'			=>	$serial_no,
								'backlog_license_no'		=>	$old_license_no,
								'is_backlog_license'		=>	1,
								'backlog_license_date'		=>	$date_license,
								'contractor_particular_id'	=>	$last_particulart_id,
								'final_status'				=>	'F',
								'status'					=>	'I',
													
         					);	
	$license_id = db_query('SELECT id FROM l_contractor_license_application where contractor_particular_id ='.$last_particulart_id)->fetchField();
	if(empty($license_id)){
		db_insert('l_contractor_license_application')->fields($array_license)->execute();
		
		$license_id = db_query('SELECT id FROM l_contractor_license_application where contractor_particular_id ='.$last_particulart_id)->fetchField();
	}else{
		$query_upadate_license 		= 	db_update('l_contractor_license_application');
 		$query_upadate_license		->	fields($array_license);
 		$query_upadate_license		->	condition('id',trim($license_id));
		$query_upadate_license		->	execute();
		
		$license_id = db_query('SELECT id FROM l_contractor_license_application where contractor_particular_id ='.$last_particulart_id)->fetchField();
	}
	
	 $array_renewal	=	array(
								'lincense_id'				=>	$license_id,
								'action'					=>	'R',
								'created_by'				=>	$contractor_id,
								'max_of_contract_labour'	=>	$contractor_max_no_of_labours_on_any_day,
								'licence_fee'				=>	$license_fees,
								'created_date'				=>	 date('Y-m-d'),
								'unskilled_wages'			=>	!empty($unskilled_rate_wages)?1:0,
								'unskilled_rate_wages'		=>	$unskilled_rate_wages,
							
								'semiskilled_wages'			=>	!empty($semiskilled_rate_wages)?1:0,
								'semiskilled_rate_wages'	=>	$semiskilled_rate_wages,
								'skilled_wages'				=>	!empty($skilled_rate_wages)?1:0,
								'skilled_rate_wages'		=>	$skilled_rate_wages,
								'highlyskilled_wages'		=>	!empty($highlyskilled_rate_wages)?1:0,
								'highlyskilled_rate_wages'	=>	$highlyskilled_rate_wages,
								
								'overtime_wages'			=>	$overtime_wages,
								'weekly_holiday'			=>	$weekly_holiday,
								'no_holiday'				=>	$no_holidays,
								'annual_leave'				=>	!empty($annual_leave_no)?1:0,
								'annual_leave_no'			=>	$annual_leave_no,
								'casual_leave'				=>	!empty($casual_leave_no)?1:0,
								'casual_leave_no'			=>	$casual_leave_no,
								'sick_leave'				=>	!empty($sick_leave_no)?1:0,
								'sick_leave_no'				=>	$sick_leave_no,
								'maternity_leave'			=>	!empty($maternity_leave_no)?1:0,	
								'maternity_leave_no'		=>	$maternity_leave_no,
								'earned_leave'				=>	!empty($earned_leave_no)?1:0,
								'earned_leave_no'			=>	$earned_leave_no,
								'other_leave'				=>	!empty($other_leave_no)?1:0,
								'other_leave_no'			=>	$other_leave_no,
								'hours_work'				=>	$hours_work,
								'spred_over'				=>	$spared_over,
								
								'holiday_wages'				=>	$holidaywages,
								'special_benifites'			=>	$special_benefits,
								'state_insurance'			=>	$state_insurance,
								'miscellaneous_provisions'	=>	$miscellaneous,
								'contractor_revoking'		=>	!empty($details_contractor_revoking)?1:0,
								'details_contractor_revoking'	=>	$details_contractor_revoking,
								'backlog_renewal_fees'		=>	$backlog_renewal_fees,
								'license_reneal_fees'		=>	$amount_of_renewal	,
								'backlog_renewal_date'		=>	$last_renewal_date,
								'is_renewal_particular'		=>	1,
								'increnent_fees'			=>	$increnent_fees,
								'renewal_security_new'		=>	$renewal_security_new,
								'payble_security_fees'		=>	$payble_security_ammount,
								'balance_securit_amount'	=>	$balance_security_ammount,
								'renewal_deposite_fees'		=>  $renewal_deposite_fees,
								'deposited_max_security_fees' => $deposited_max_security_fees
											
         					);
							
		$renewal_id = db_query('SELECT id FROM l_contractor_license_apply_renweal where lincense_id ='.$license_id)->fetchField();						
	if(empty($renewal_id)){
		db_insert('l_contractor_license_apply_renweal')->fields($array_renewal)->execute();
	}else{
		$query_upadate_renewal 		= 	db_update('l_contractor_license_apply_renweal');
 		$query_upadate_renewal		->	fields($array_renewal);
 		$query_upadate_renewal		->	condition('id',$renewal_id);
		$query_upadate_renewal		->	execute();
	}	

	$action = 'encrypt';

	$serial_number=encryption_decryption_fun($action, $serial_no);//----miscellaneous module

       if($renewal_appliaction_status == 'B'){
			contractor_activity_tag_insert_backlog_reneeal($serial,'R',$renewal_id,'B');
		}else{
			contractor_activity_tag_insert_backlog_reneeal($serial,'R',$renewal_id,NULL);
		}     
// drupal_goto('renewal/backlog-renewal/work_details/'.$serial_number);
	    
$message = 'Particular of contact labour has been saved successfully.';
	drupal_set_message(t($message));
	
	
	
}else{
	 	drupal_set_message('Please Fillup Previous tab'); 
 
	
}
	}
	else{
		$recommand_dateexpire = date("dS M, Y",strtotime($recommand_date_expire));
    drupal_set_message('"<b>Date of expiry of the previous license</b>" provided by you is not matched with ALC recommended date('.$recommand_dateexpire.'). Please correct the same','error'); 
}

	}
	if(empty($recommand_date_expire)){
	

	if($previous_employer=='1'){		
		$previous_license 					= 	db_select('l_contrcator_previous_license', 'previous_license');
  		$previous_license					->	fields('previous_license',array('perivious_license_no'));
		$previous_license					->	condition('previous_license.serial_no_form_v',$serial,'=');
		$previous_license					->	condition('previous_license.created_by',$contractor_id,'='); 
		$previous_license					->	condition('previous_license.perivious_license_no', 'NULL', '!=');
		$previous_license_result			=	$previous_license->execute()->fetchAll();
		
		
		$reg_no_principal 					= 	db_select('l_contrcator_previous_license', 'previous_license');
		$reg_no_principal					->	fields('previous_license',array('reg_no_principal_employeer'));
		$reg_no_principal					->	condition('previous_license.serial_no_form_v',$serial,'=');
		$reg_no_principal					->	condition('previous_license.created_by',$contractor_id,'='); 
		$reg_no_principal					->	condition('previous_license.reg_no_principal_employeer', 'NULL', '!=');
	
		$reg_no_principal_result 			= 	$reg_no_principal->execute()->fetchAll();
		
		
		if(empty($previous_license_result)){
			if(!empty($add_license_number)){
				//$add_license_list = substr($add_license_number, 0, -1);
				$add_license_lists=explode(',', $add_license_number);					
				foreach($add_license_lists as $row){ 
					$add_more_license = array( 
											'perivious_license_no' => $row,
											'created_by'		   => $contractor_id,
											'created_date'		   => date("Y-m-d"),
											'serial_no_form_v'	   =>  $serial
											);														  
																			
					db_insert('l_contrcator_previous_license')->fields($add_more_license)->execute();
				}
			}
		}else{
			 $del_query			=		db_delete('l_contrcator_previous_license')->condition('serial_no_form_v', $serial);
			 $del_query			->		condition('created_by',$contractor_id,'=');
			 $del_query			->		isNotNull('perivious_license_no');
			 $del_query			->		execute();
			
			if(!empty($add_license_number)){
				//$add_license_list = substr($add_license_number, 0, -1);
				$add_license_lists=explode(',', $add_license_number);
					
				foreach($add_license_lists as $row){ 
					$add_more_license = array( 
											'perivious_license_no'		=>	$row,
											'created_by'				=>	$contractor_id,
											'created_date'				=>	date("Y-m-d"),
											'serial_no_form_v'			=>	$serial
											);
					db_insert('l_contrcator_previous_license')->fields($add_more_license)->execute();
				}
			}			
		}
		
		if(empty($reg_no_principal_result)){
			if(!empty($add_reg_no_pe)){
				//$add_reg_list = substr($add_reg_no_pe, 0, -1);
				$add_reg_lists=explode(',', $add_reg_no_pe);
				foreach($add_reg_lists as $row){ 
					$add_more_reg_pe_no = array( 
										'reg_no_principal_employeer'		=>	$row,
										'created_by'						=>	$contractor_id,
										'created_date'						=>	date("Y-m-d"),
										'serial_no_form_v'					=>	$serial
										);
					db_insert('l_contrcator_previous_license')->fields($add_more_reg_pe_no)->execute();	
				}
			}					
		}else{
			$del_query_reg		=	db_delete('l_contrcator_previous_license');   
			$del_query_reg		->	condition('serial_no_form_v', $serial);
			$del_query_reg		->	condition('created_by',$contractor_id,'=');
			$del_query_reg		->	isNotNull('reg_no_principal_employeer');
			$del_query_reg		->	execute();
			
			if(!empty($add_reg_no_pe)){
			//	$add_reg_list = substr($add_reg_no_pe, 0, -1);
				$add_reg_lists=explode(',', $add_reg_no_pe);
					
				foreach($add_reg_lists as $row){ 
					$add_more_reg_pe_no = array( 
											'reg_no_principal_employeer'		=>	$row,
											'created_by'						=>	$contractor_id,
											'created_date'						=>	date("Y-m-d"),
											'serial_no_form_v'					=>	$serial
											);
					db_insert('l_contrcator_previous_license')->fields($add_more_reg_pe_no)->execute();						
						
				}
			}
		}
	}
	 
	
	if(empty($previous_employer)){		
		$previous_license = db_select('l_contrcator_previous_license', 'previous_license');
  		$previous_license->fields('previous_license',array('perivious_license_no'));
		$previous_license->condition('previous_license.serial_no_form_v',$serial,'=');
		$previous_license->condition('previous_license.created_by',$contractor_id,'=');
		$previous_license->condition('previous_license.perivious_license_no', 'NULL', '!=');
		$previous_license_result = $previous_license->execute()->fetchAll();
		
		
		$reg_no_principal = 	db_select('l_contrcator_previous_license', 'previous_license');
		$reg_no_principal->fields('previous_license',array('reg_no_principal_employeer'));
		$reg_no_principal->condition('previous_license.serial_no_form_v',$serial,'=');
		$reg_no_principal->condition('previous_license.created_by',$contractor_id,'=');
		$reg_no_principal->condition('previous_license.reg_no_principal_employeer', 'NULL', '!=');
	
		$reg_no_principal_result = $reg_no_principal->execute()->fetchAll();
		
		
		if(!empty($previous_license_result)){
									db_delete('l_contrcator_previous_license')  
																 ->	condition('serial_no_form_v', $serial)
																 ->	condition('perivious_license_no', 'NULL', '!=')
																 ->	condition('created_by',$contractor_id,'=') 
																 -> execute();
					
		}
		if(!empty($reg_no_principal_result)){
			 					db_delete('l_contrcator_previous_license')   
																->	condition('serial_no_form_v', $serial)
																->	condition('reg_no_principal_employeer', 'NULL', '!=') 
																 ->	condition('created_by',$contractor_id,'=') 
																->	execute();
					
					

		}
		
	}
	
		
		if($editable_work=='1'){
			$contract_natureofwork	=	$natureofwork_edit;
			
			$contract_natureofwork = explode(',', $natureofwork_edit);
		}
		if($editable_agent=='1'){
			$name_of_agent			=	$contract_agnt_name_edit;
		}
		
		if($editable_address_manager=='1'){
			$address_of_manager			=	$manager_add_edit;			
			$contractor_dist			=	$manager_dist_edit;
			$contractor_subdivision		=	$manager_subdivision_edit;
			$contractor_areatype		=	$manager_areatype_edit;
			$contractor_name_areatype	=	$name_manager_areatype_edit;
			$contractor_vill_ward		=	$manager_vill_ward_edit;
			$contractor_ps				=	$manager_ps_edit;		
		}
	
		/*if($flag=='1'){
			if($editable_max_no_day=='1'){
				$contractor_max_no_of_labours_on_any_day	=	$noof_contractlabour_edit;
				$amount_of_licence							=	$fees_edit;
				
			}else{
				
				$amount_of_licence							=	$amount_of_licence;
			}
		}*/
		if($is_extra_fees	==	1){
						$amount_of_renewal		=	$amount_of_renewal;
						$increnent_fees			=	(125/100)*$amount_of_renewal;
				}else{
						$amount_of_renewal		=	$amount_of_renewal;
						$increnent_fees			=	$amount_of_renewal;
				}
				
		
		if($editable_convicted=='1'){
			$contractor_convicted			=	$convicted_edit;
			$details_contractor_convicted	=	$convicted_details_edit;
		}
		if($editable_revoking == '1'){
			$contractor_revoking			=	$revoking_edit;				
			if(!empty($revoking_details_edit)){
				$details_contractor_revoking = $revoking_details_edit;
			}
		}			
		
		if($editable_previous_employer=='1'){
			$contractor_previous_employer			=	$previous_employer_edit;	
			$details_previous_employer				=	$previous_employer_details_edit;
		}
		$no_holidays="";
		if(empty($no_holiday)){
			$no_holidays="";
		}else{
		foreach($no_holiday as $row){
			$no_holidays.=$row.',';
		}
		}

        $contract_labour=array(
								'contractor_user_id'						=>	$contractor_id,
								'name_of_agent'								=>	$name_of_agent,
								'address_of_manager'						=>	$address_of_manager, 
								'contractor_convicted'						=>	$contractor_convicted,
								'details_contractor_convicted'				=>	$details_contractor_convicted,
								'contractor_revoking'						=>	$contractor_revoking,
								'details_contractor_revoking'				=>	$details_contractor_revoking,

								'contractor_previous_employer'				=>	$previous_employer,
								'details_previous_employer'					=>	$previous_employer_details,
								'user_id'									=>	$user_id,
								'act_id'									=>	$act_id,
								'amount_of_licence'							=>	$license_fees,
								'amount_of_security_deposit'				=>	$amount_of_security_deposit,
								'contractor_manager_dist'					=>	$contractor_dist,
								'contractor_manager_subdivision'			=>	$contractor_subdivision ,
								'contractor_manager_areatype'				=>	$contractor_areatype,
								'contractor_manager_name_areatype'			=>	$contractor_name_areatype ,
								'contractor_managerr_vill_ward'				=>	$contractor_vill_ward,
								'contractor_manager_ps'						=>	$contractor_ps,
								'manager_pin'								=>	$manager_pin,
								'contractor_max_no_of_labours_on_any_day'	=>	$contractor_max_no_of_labours_on_any_day,								
								'category_designation'						=>	$category_designation,								
								'unskilled_wages'							=>	!empty($unskilled_rate_wages)?1:0,
								'unskilled_rate_wages'						=>	$unskilled_rate_wages,								
								'semiskilled_wages'							=>	!empty($semiskilled_rate_wages)?1:0,
								'semiskilled_rate_wages'					=>	$semiskilled_rate_wages,								
								'skilled_wages'								=>	!empty($skilled_rate_wages)?1:0,
								'skilled_rate_wages'						=>	$skilled_rate_wages,								
								'highlyskilled_wages'						=>	!empty($highlyskilled_rate_wages)?1:0,
								'highlyskilled_rate_wages'					=>	$highlyskilled_rate_wages,								
								'hours_work'								=> 	$hours_work,
								'overtime'									=> 	$overtime,
								'overtime_wages'							=> 	$overtime_wages,
								'weekly_holiday'							=> 	$weekly_holiday,
								'no_holiday'								=> 	$no_holidays,		
								'annual_leave'								=> 	!empty($annual_leave_no)?1:0,
								'annual_leave_no'							=> 	$annual_leave_no,
								'casual_leave'								=>	!empty($casual_leave_no)?1:0,
								'casual_leave_no'							=>	$casual_leave_no,
								'sick_leave'								=> 	!empty($sick_leave_no)?1:0,
								'sick_leave_no'								=> 	$sick_leave_no,
								'other_leave'								=> 	!empty($other_leave_no)?1:0,
								'other_leave_no'							=> 	$other_leave_no,
								'maternity_leave'							=>	!empty($maternity_leave_no)?1:0,
								'maternity_leave_no'						=> 	$maternity_leave_no	,
								
								
														
         					);						
							
		$fetch_clra_details		=	get_details_clra_master($application_id);//----miscellaneous module
	
					
	
		


	
if($rowcount>0){	

	$query_contract_labour 		= 	db_update('l_particular_of_contract_labour');
 	$query_contract_labour		->	fields($contract_labour);
 	$query_contract_labour		->	condition('contractor_user_id',trim($contractor_id));
	$query_contract_labour		->	condition('serial_no_from_v',trim($serial_no));
 	$query_contract_labour		->	execute();
	
	$last_particulart_id 		= 	db_query('SELECT MAX(id) FROM l_particular_of_contract_labour where contractor_user_id ='.$contractor_id.'and serial_no_from_v ='.$serial_no)->fetchField();
	
	$array_license	=	array(
								'contractor_user_id'		=>	$contractor_id,
								'user_id'					=>	$applicant_id,
								'act_id'					=>	1, 
								'serial_no_from_v'			=>	$serial_no,
								'backlog_license_no'		=>	$old_license_no,
								'is_backlog_license'		=>	1,
								'backlog_license_date'		=>	$date_license,
								'contractor_particular_id'	=>	$last_particulart_id,
								'final_status'				=>	'F',
								'status'					=>	'I',
													
         					);	
	$license_id = db_query('SELECT id FROM l_contractor_license_application where contractor_particular_id ='.$last_particulart_id)->fetchField();
	if(empty($license_id)){
		db_insert('l_contractor_license_application')->fields($array_license)->execute();
		
		$license_id = db_query('SELECT id FROM l_contractor_license_application where contractor_particular_id ='.$last_particulart_id)->fetchField();
	}else{
		$query_upadate_license 		= 	db_update('l_contractor_license_application');
 		$query_upadate_license		->	fields($array_license);
 		$query_upadate_license		->	condition('id',trim($license_id));
		$query_upadate_license		->	execute();
		
		$license_id = db_query('SELECT id FROM l_contractor_license_application where contractor_particular_id ='.$last_particulart_id)->fetchField();
	}
	
	 $array_renewal	=	array(
								'lincense_id'				=>	$license_id,
								'action'					=>	'R',
								'created_by'				=>	$contractor_id,
								'max_of_contract_labour'	=>	$contractor_max_no_of_labours_on_any_day,
								'licence_fee'				=>	$license_fees,
								'created_date'				=>	 date('Y-m-d'),
								'unskilled_wages'			=>	!empty($unskilled_rate_wages)?1:0,
								'unskilled_rate_wages'		=>	$unskilled_rate_wages,
							
								'semiskilled_wages'			=>	!empty($semiskilled_rate_wages)?1:0,
								'semiskilled_rate_wages'	=>	$semiskilled_rate_wages,
								'skilled_wages'				=>	!empty($skilled_rate_wages)?1:0,
								'skilled_rate_wages'		=>	$skilled_rate_wages,
								'highlyskilled_wages'		=>	!empty($highlyskilled_rate_wages)?1:0,
								'highlyskilled_rate_wages'	=>	$highlyskilled_rate_wages,
								
								'overtime_wages'			=>	$overtime_wages,
								'weekly_holiday'			=>	$weekly_holiday,
								'no_holiday'				=>	$no_holidays,
								'annual_leave'				=>	!empty($annual_leave_no)?1:0,
								'annual_leave_no'			=>	$annual_leave_no,
								'casual_leave'				=>	!empty($casual_leave_no)?1:0,
								'casual_leave_no'			=>	$casual_leave_no,
								'sick_leave'				=>	!empty($sick_leave_no)?1:0,
								'sick_leave_no'				=>	$sick_leave_no,
								'maternity_leave'			=>	!empty($maternity_leave_no)?1:0,	
								'maternity_leave_no'		=>	$maternity_leave_no,
								'earned_leave'				=>	!empty($earned_leave_no)?1:0,
								'earned_leave_no'			=>	$earned_leave_no,
								'other_leave'				=>	!empty($other_leave_no)?1:0,
								'other_leave_no'			=>	$other_leave_no,
								'hours_work'				=>	$hours_work,
								'spred_over'				=>	$spared_over,
								
								'holiday_wages'				=>	$holidaywages,
								'special_benifites'			=>	$special_benefits,
								'state_insurance'			=>	$state_insurance,
								'miscellaneous_provisions'	=>	$miscellaneous,
								'contractor_revoking'		=>	!empty($details_contractor_revoking)?1:0,
								'details_contractor_revoking'	=>	$details_contractor_revoking,
								'backlog_renewal_fees'		=>	$backlog_renewal_fees,
								'license_reneal_fees'		=>	$amount_of_renewal	,
								'backlog_renewal_date'		=>	$last_renewal_date,
								'is_renewal_particular'		=>	1,
								'increnent_fees'			=>	$increnent_fees,
								'renewal_security_new'		=>	$renewal_security_new,
								'payble_security_fees'		=>	$payble_security_ammount,
								'balance_securit_amount'	=>	$balance_security_ammount,
								'renewal_deposite_fees'		=>  $renewal_deposite_fees,
								'deposited_max_security_fees' => $deposited_max_security_fees
											
         					);
							
							
		$renewal_id = db_query('SELECT id FROM l_contractor_license_apply_renweal where lincense_id ='.$license_id)->fetchField();	
						
	if(empty($renewal_id)){
		db_insert('l_contractor_license_apply_renweal')->fields($array_renewal)->execute();
	}else{
		$query_upadate_renewal 		= 	db_update('l_contractor_license_apply_renweal');
 		$query_upadate_renewal		->	fields($array_renewal);
 		$query_upadate_renewal		->	condition('id',$renewal_id);
		$query_upadate_renewal		->	execute();
	}	

	$action = 'encrypt';

	$serial_number=encryption_decryption_fun($action, $serial_no);//----miscellaneous module
 if($renewal_appliaction_status == 'B'){
			contractor_activity_tag_insert_backlog_reneeal($serial,'R',$renewal_id,'B');
		}else{
			contractor_activity_tag_insert_backlog_reneeal($serial,'R',$renewal_id,NULL);
		}     
      
// drupal_goto('renewal/backlog-renewal/work_details/'.$serial_number);
	    
$message = 'Particular of contact labour has been saved successfully.';
	drupal_set_message(t($message));
	
	
	
}else{
	 	drupal_set_message('Please Fillup Previous tab'); 
 
	
}
	
	
	}
		
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