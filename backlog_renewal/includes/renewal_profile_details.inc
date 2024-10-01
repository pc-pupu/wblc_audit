<?php
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
drupal_add_js(drupal_get_path('module', 'contractor') . '/js/contractor.js');
function renewal_profile_details($form, &$form_state,$serial_number=''){ 
	global $base_root, $base_path, $user; 
	$serial=encryption_decryption_fun('decrypt', $serial_number); 
	if($serial==6333 && $user->uid==12854 ){	
		$contractor_id 		= 	$user->uid;
		if($contractor_id!=0){ 
		
		$status_from='0';
		drupal_add_library('system','ui.datepicker');
		
		drupal_add_js('jQuery(document).ready(function(){jQuery( \'#dobId\' ).datepicker({
							  dateFormat  : "yy-mm-dd",
							  changeMonth : true,
							  changeYear  : true,
							  autoSize    : true,
							  yearRange	: "-100:+0",
							  });});', 'inline');
							  
				  
		$contractor_address	=	"";
		$query 	= 	db_select('l_contractor_info_master', 'info');
 		$query->fields('info',array())->condition('id',$serial,'=');
		$obj = $query->execute()->fetchObject();
		//$result = 	$query->execute();
	
		
		
		//if( $result->rowCount() > 0 ){
			//foreach($result as $obj){	
				  $name_of_contractor				=	$obj->name_of_contractor;
				  $address_of_contractor			=	$obj->address_of_contractor;
				  $id								=	$obj->id;
				  $applicant_id						=	$obj->user_id;
				  $_SESSION['applicant_id']			=	$applicant_id;
				  $areatype							=	$obj->con_loc_e_areatype;
				  $is_from_v						=	$obj->is_from_v;
				  $contractor_dist_con				=	$obj->con_loc_e_dist;
				  $contractor_sub_division_con		=	$obj->con_loc_e_subdivision;
				  $contractor_area_type_con 		=	$obj->con_loc_e_areatype;
				  $contractorname_area_type_con		=	$obj->con_name_areatype;
				  $contractor_villward_con			=	$obj->con_loc_e_vill_ward;			 
				  $contractor_police_station_con	=	$obj->con_l_e_ps;
				  $contractor_pin_con				=   $obj->contractor_pin;
				  $applicantion_id 					= 	$obj->application_id;
				  $startdate						=	$obj->est_date_of_work_of_each_labour_from_date;
				  $enddate							=	$obj->est_date_of_work_of_each_labour_to_date; 
				 $max_labour 						=   $obj->contractor_max_no_of_labours_on_any_day;
									  
			//}			
		//}
		//die($contractor_sub_division_con);
		$query_details 				= 	db_select('l_particular_of_contract_labour', 'lpcl');
		$query_details				->	fields('lpcl',array())->condition('serial_no_from_v',$serial,'=');
		$query_details				->	condition('contractor_user_id',$contractor_id,'=');
		$query_details				->	condition('act_id','1','=');
		$obj_details 			= 	$query_details->execute()->fetchAll();	
	//print_r($result_details);die();
		
		if(!empty($result_details)){
			//foreach($result_details as $obj_details){			
				$category_of_contractor			=	$obj_details->category_of_contractor;
				$father_contarctor_name			=	$obj_details->father_contarctor_name;
				$dob_contractor					=	$obj_details->dob_contractor;
				$age_contractor					=	$obj_details->age_contractor;
				$contractor_dist				=	$obj_details->contractor_dist;
				$contractor_sub_division		=	$obj_details->contractor_subdivision;
				$contractor_area_type 			=	$obj_details->contractor_areatype;
				$contractorname_area_type		=	$obj_details->contractor_name_areatype;
				$contractor_villward			=	$obj_details->contractor_vill_ward;
				$contractor_police_station		=	$obj_details->contractor_ps;
				$address_line1_of_contractor	=	$obj_details->address_of_contractor;
				$contractor_pin					=   $obj_details->contractor_pin;
				$worksite_address_line		  	=	$obj_details->worksite_address_line;
				$worksite_dist				  	=	$obj_details->worksite_dist; 
				$worksite_subdivision		  	=	$obj_details->worksite_subdivision;
				$work_site_areatype			  	=	$obj_details->work_site_areatype;
				$name_work_site_areatype	  	=	$obj_details->name_work_site_areatype;
				$work_site_vill_ward		  	=	$obj_details->work_site_vill_ward;
				$worksite_pin				  	=	$obj_details->worksite_pin;
				$worksite_ps				  	=	$obj_details->worksite_ps;
				$particulerid					=   $obj_details->id;
				$amount_of_security_deposit		=   $obj_details->amount_of_security_deposit;
			
								  
			//}			
		}
		if(!empty($particulerid)){
				$fetch_license_data 					= 	db_select('l_contractor_license_application', 'license');
				$fetch_license_data						->	fields('license',array('id'));
				$fetch_license_data						->	condition('license.contractor_particular_id',$particulerid,'=');
				$license_contractor_id 					= 	$fetch_license_data->execute()->fetchObject()->id;	
				
		}
		
		if(!empty($license_contractor_id)){
			
			$renewal_data						= fetch_all_data_contractor_license_apply_renweal($license_contractor_id,$contractor_id);
			$renewal_application_final_status	= $renewal_data['renewal_application_final_status'];
			$renewal_appliaction_status			= $renewal_data['renewal_appliaction_status'];	
			$renewal_id							= $renewal_data['id'];		
			$is_coparative_data					= $renewal_data['is_coparative'];		
		}
		
			
			$status_query_result 	= 	application_status($serial,$contractor_id);//-------call from miscellaneous module
			$status					=	$status_query_result['status'];
			$final_status			=	$status_query_result['final_status'];
			
			$contractor_address		='';	
			$contractor_address		=   miscellaneous_contractor_address_form($areatype,$serial);//---call from miscellaneous module
			$contractor_address		=   $address_of_contractor .'<br/>'.$contractor_address;
			
			$editablefield=fetch_editable_field_renewal($serial,$renewal_id);//---call from miscellaneous module
			
			//print_r($editablefield);die($renewal_id);
				$readonly_contarctor_dob 		= array( 'class' => array('last_renewal_date_1'), 'onclick' => 'datepicker_lastrenewal();','readonly' => 'readonly',);
			
			if(!empty($editablefield)){
				
					$readonly_contract_dist 		= array('class' => array('correction_show'));
					$readonly_contract_subdivision 	= array('class' => array('correction_show'));
					$readonly_contract_areatype 	= array('class' => array('correction_show'));
					$readonly_contract_block 		= array('class' => array('correction_show'));
					$readonly_contract_mun 			= array('class' => array('correction_show'));
					$readonly_contract_corp 		= array('class' => array('correction_show'));
					$readonly_contract_sez 			= array('class' => array('correction_show'));
					$readonly_contract_not 			= array('class' => array('correction_show'));
					$readonly_contract_vill 		= array('class' => array('correction_show'));
					$readonly_contract_ward 		= array('class' => array('correction_show'));
					$readonly_contract_bugh 		= array('class' => array('correction_show'));
					$readonly_contractor_ps 		= array('class' => array('correction_show'));
					$readonly_address_line1 		= array('class' => array('correction_show'));
					$readonly_pin 					= array('maxlength' => 6 ,'class' => array('correction_show'));
					$readonly_contarctor_age 		= array('class' => array('correction_show'));
					$readonly_category_of_contractor = array('class' => array('correction_show'));
					$readonly_father_contarctor_name = array('placeholder' => 'Enter your Father Name', 'class' => array('correction_show'));
					$readonly_worksite_address_line1 = array('class' => array('correction_show'));
					$readonly_worksite_dist 		= array('class' => array('correction_show'));
					$readonly_worksite_subdivision 	= array('class' => array('correction_show'));
					$readonly_worksite_areatype 	= array('class' => array('correction_show'));
					$readonly_worksite_block 		= array('class' => array('correction_show'));
					$readonly_worksite_mun 			= array('class' => array('correction_show'));
					$readonly_worksite_corp 		= array('class' => array('correction_show'));
					$readonly_worksite_sez 			= array('class' => array('correction_show'));
					$readonly_worksite_not 			= array('class' => array('correction_show'));
					$readonly_worksite_vill 		= array('class' => array('correction_show'));
					$readonly_worksite_ward 		= array('class' => array('correction_show'));
					$readonly_worksite_bugh 		= array('class' => array('correction_show'));
					$readonly_worksite_ps 			= array('class' => array('correction_show'));
					$readonly_worksite_pin 			= array('class' => array('correction_show'));
				
					$readonly_contarctor_dob 			= array( 'class' => array('last_renewal_date'), 'onclick' => 'datepicker_lastrenewal();','readonly' => 'readonly');
					
				$editablefield_array = explode(',', $editablefield['remark_field_title']);	
				
				foreach($editablefield_array as $row){
					
				switch($row){	
					case "address_of_contractor":
				
					$readonly_contract_dist 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contract_subdivision 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contract_areatype 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contract_block 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contract_mun 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contract_corp 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contract_sez 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contract_not 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contract_vill 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contract_ward 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contract_bugh 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_contractor_ps 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_address_line1 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_pin 					= array('disabled' => 'disabled', 'maxlength' => 6 , 'class' => array('readonly_view'));
					break;
					case "indivisual_company":
					//$readonly_contarctor_dob 		= array('disabled' => 'disabled');
					$readonly_contarctor_dob = array('readonly' => true,'class'=> array('readonly_view_date'),'readonly' => 'readonly');
					$readonly_contarctor_age 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_category_of_contractor = array('readonly' => 'readonly', 'class' => array('readonly_view'));
					$readonly_father_contarctor_name = array('placeholder' => 'Enter your Father Name', 'readonly' => 'readonly', 'class' => array('readonly_view'));
					$editable_category				=	1;
					break;
					case "worksite_address":
					
					$readonly_worksite_address_line1 = array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_worksite_dist 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_worksite_subdivision 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_worksite_areatype 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_worksite_block 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_worksite_mun 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_worksite_corp 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_worksite_sez 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_worksite_not 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_worksite_vill 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_worksite_ward 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_worksite_bugh 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_worksite_ps 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
					$readonly_worksite_pin 			= array('disabled' => 'disabled', 'maxlength' => 6 , 'class' => array('readonly_view'));
					break;
					case "is_co_oparative":	
						$readonly_cooparative 		= array('disabled' => 'disabled','class' => array('readonly_view'));
						$editable_cooparative		= 1;
								
						break;
								
					default:
					
					
							
				
			
				}
				
				
				
			}
		}
	
		$form['renewla_profile']		= array(
				'#title' 				=>	 t('<div class="content"><div class="sky-form" style = "width:100%"><header><h3 align="center"> APPLICATION FOR RENEWAL OF LICENSE  </h3></header>'),
				'#type' 				=> 	'fieldset',
				'#attributes' 			=> 	array('class'=>array('')),
				'#collapsed' 			=> 	TRUE,
				'#collapsable'			=>	TRUE
				);
										
		$form['renewla_profile']['max_labour'] = array(		 
				  '#type'			=>	'hidden',
				  '#default_value'	=>	!empty($max_labour) ? encryption_decryption_fun('encrypt', $max_labour) : 0,
				 
											
			);
		$form['renewla_profile']['max_deposit'] = array(		 
				  '#type'			=>	'hidden',
				  '#default_value'	=>	!empty($amount_of_security_deposit) ? encryption_decryption_fun('encrypt', $amount_of_security_deposit) : 0,
				 
											
			);								  
		
										  
			$form['renewla_profile']['id'] = array(		 
				  '#type'			=>	'hidden',
				  '#default_value'	=>	!empty($id) ?  $id: 0,
				 
											
			);							  
		
										  
		 $form['renewla_profile']['serial'] = array(									 
										  '#type' 			=> 	'hidden',
										  '#default_value'  =>	!empty($serial) ? $serial : '',
										
										  );
		 $form['renewla_profile']['status_from'] = array(									 
										  '#type' 			=> 	'hidden',
										  '#default_value'  =>	!empty($status_from) ? $status_from : '',
																				
										  );
		$form['renewla_profile']['editable_dist'] = array(									 
										  '#type' 			=> 	'hidden',
										  '#default_value'  =>	!empty($editable_dist) ? $editable_dist : '',
																				
										  );
		$form['renewla_profile']['editable_name'] = array(									 
										  '#type' 			=> 	'hidden',
										  '#default_value'  =>	!empty($editable_name) ? $editable_name : '',
																				
										  );
		 $form['renewla_profile']['editable_category'] = array(									 
										  '#type' 			=> 	'hidden',
										  '#default_value'  =>	!empty($editable_category) ? $editable_category : '',
																			
										  );
		$form['renewla_profile']['editable_dob'] = array(									 
										  '#type' 			=> 	'hidden',
										  '#default_value'  =>	!empty($editable_dob) ? $editable_dob : '',
																			
										  );
		 
		$form['renewla_profile']['applicant_id'] = array(									 
											  '#type' 			=> 'hidden',
											  '#default_value' 	=>	!empty($applicant_id) ? $applicant_id : '',
											  
											
										  );
		$form['renewla_profile']['reg_contractor'] = array(
										  '#type' 				=> 'hidden',
										  '#default_value' 		=>!empty($reg_contractor) ? $reg_contractor : '',
										
										  );
										  
	//-------------------------------------Back for correction for  field-------------------------------- 									  
			
			$form['renewla_profile']['contract_dist_edit'] = array(									 
										  '#type' 			=> 	'hidden',
										  '#default_value'  =>	!empty($contractor_dist) ? $contractor_dist : '',
																				
										  );
			$form['renewla_profile']['contract_subdivision_edit'] = array(									 
										  '#type' 			=> 	'hidden',
										  '#default_value'  =>	!empty($contractor_sub_division) ? $contractor_sub_division : '',
																				
										  );
			$form['renewla_profile']['contract_areatype_edit'] = array(									 
										  '#type' 			=> 	'hidden',
										  '#default_value'  =>	!empty($contractor_area_type) ? $contractor_area_type : '',
																				
										  );
			$form['renewla_profile']['name_contractor_areatype_edit']=array(
														'#type' 			=> 	'hidden',
														'#default_value'  =>	!empty($contractorname_area_type) ? $contractorname_area_type : '',
																				
										  );
			
			$form['renewla_profile']['contractor_vill_ward_edit'] = array(									 
										  '#type' 			=> 	'hidden',
										  '#default_value'  =>	!empty($contractor_villward) ? $contractor_villward : '0',
																				
										  );
			
			 $form['renewla_profile']['contractor_ps_edit'] = array(									 
										  '#type' 			=> 	'hidden',
										  '#default_value'  =>	!empty($contractor_police_station) ? $contractor_police_station : '',
																				
										  );
	
			 $form['renewla_profile']['contarctor_name_edit'] = array(									 
										  '#type' 			=> 	'hidden',
										  '#default_value'  =>	!empty($name_of_contractor) ? $name_of_contractor : '',
																				
										  );
	
		
			 $form['renewla_profile']['category_edit'] = array(									 
										  '#type' 			=> 	'hidden',
										  '#default_value'  =>	!empty($category_of_contractor) ? $category_of_contractor : '0',
																				
										  );
			 $form['renewla_profile']['father_edit'] = array(									 
										  '#type' 			=> 	'hidden',
										  '#default_value'  =>	!empty($father_contarctor_name) ? $father_contarctor_name : '',
																				
										  );
			$form['renewla_profile']['dob_edit'] = array(									 
										  '#type' 			=> 	'hidden',
										  '#default_value'  =>	!empty($dob_contractor) ? $dob_contractor : '',
																				
										  );
			$form['renewla_profile']['age_edit'] = array(									 
										  '#type' 			=> 	'hidden',
										  '#default_value'  =>	!empty($age_contractor) ? $age_contractor : '0',
																				
										  );
										  
										  
		$query_work = db_select('l_clra_reg_nature_of_work', 'c_reg_nature');
		$query_work->leftJoin('contractor_works', 'con_work','con_work.contractor_work_id = c_reg_nature.e_nature_of_work');  
		 
		// Adding fields
		$query_work->fields('c_reg_nature',array('identification_number','user_id','act_id'));
		$query_work->fields('con_work',array('cont_work_name'));
		$query_work->condition('c_reg_nature.user_id',$applicant_id,'=');
		$query_work->condition('c_reg_nature.application_id',$applicantion_id,'=');
		$query_work->condition('c_reg_nature.act_id','1','=');
		$result_work = $query_work->execute(); 
		$nature_work="";
		if( $result_work->rowCount() > 0 ){
			if($result_work->rowCount() > 0 ){
				foreach($result_work as $objwork){
					$nature_work_list=$nature_work_list.$objwork->cont_work_name.', ';
				}
			}
		}
		
		$nature_work= substr($nature_work_list, 0, -2);
		$e_name="";
		$query = db_select('l_clra_registration_master', 'c_r_m');
		$query->fields('c_r_m',array('id','e_name','e_postal_address','loc_e_areatype','registration_number','identification_number','registration_date','loc_emp_areatype','full_name_principal_emp','address_principal_emp','loc_e_name','loc_e_dist','loc_e_subdivision','name_areatype','loc_e_vill_ward','l_e_ps','loc_e_pin_number'))->condition('id',$applicantion_id,'=');
		$query->condition('act_id','1','=');
		$result = $query->execute()->fetchAll();
		//print_r($result);die();
		$result = $query->execute();
		
		
		$result = $query->execute();
		if( $result->rowCount() > 0 ){
			foreach($result as $obj){
				  $e_name					=	$obj->e_name;
				  $loc_e_name				=   $obj->loc_e_name;
				  $e_postal_address			=	$obj->e_postal_address;
				  $identification_number	=	$obj->identification_number;
				  $registration_number		=	$obj->registration_number;
				  $id						=	$obj->id;
				  $loc_e_areatype			=	$obj->loc_e_areatype;
				  $registration_date		=	$obj->registration_date;
				  $loc_emp_areatype			=	$obj->loc_emp_areatype;
				  $full_name_principal_emp	=	$obj->full_name_principal_emp;
				  $address_principal_emp	=	$obj->address_principal_emp;
				  
				   $loc_e_dist				=	$obj->loc_e_dist;
				 $loc_e_subdivision			=	$obj->loc_e_subdivision;
				  $loc_e_areatype			=	$obj->loc_e_areatype;
				  $name_areatype			=	$obj->name_areatype;
				  $loc_e_vill_ward			=	$obj->loc_e_vill_ward;
				  $l_e_ps					=	$obj->l_e_ps;
				  $loc_e_pin_number			=	$obj->loc_e_pin_number;
				  $loc_e_name				=	$obj->loc_e_name;
			}
		}
		
		$establishment_address="";
		$principal_address="";
		$establishment_address	=	miscellaneous_establishment_address_form($loc_e_areatype,$serial,$applicantion_id);//----miscellaneous module
		$principal_address		=	miscellaneous_principal_employer($loc_emp_areatype,$serial,$applicantion_id);//----miscellaneous module
		$principal_details		=   $full_name_principal_emp."<br/>".$address_principal_emp."<br/>".$principal_address;
	
		
		$registration_date_timestamp = strtotime($registration_date);
		$formatted_reg_date = date('m/d/Y', $registration_date_timestamp);
		
		$establish_details="";
		$establish_details=$establish_details."Registration Number: ".$registration_number."<br/>"."Date of Certificates: ".$formatted_reg_date."<br/> Under The acts of CLRA";	
		$name_address=  $e_name."<br/>".$loc_e_name."<br/>".$establishment_address;
		
		$result_work = get_nature_of_work_contrcator($applicantion_id,$serial); //---miscellaneous module
		if(!empty($result_work)) {      
			$nature_work="";
				foreach($result_work as $objwork){			
						$nature_work	=	$nature_work.$objwork->cont_work_name.', ';		  
					}
				$sub_con				=	substr($nature_work, 0, -2);
				$nature_work			=	$sub_con;	
			
			
			}
			
		$start_date		=	date('dS F y', strtotime($startdate));
		$end_date		=	date('dS F y', strtotime($enddate));
		$startend_date	=	$start_date.' To '.$end_date;
										  
		$output	.= '<style type="text/css">
						table, td, th {
								
								height:12px;
								
						}
						td{
							
							padding-left:20px;
							padding-right:20px;
						}
					</style>
					<table width="100%"  style=" border-width: thin;"  border="0px thin" bordercolor="#CCCCCC" cellspacing="0" cellpadding="10">
					<tr><td colspan="4"></td></tr>
						<tr>
							<td colspan="3">A.Name of the contractor</td>
							
							<td >'.$name_of_contractor.'</td>
						
						</tr>
						<tr><td colspan="4"></td></tr>
						
						<tr>
							<td colspan="3" valign="top">B.Address of the contractor </td>
							
							<td>'.$contractor_address.'</td>
						
							
						</tr>
					  
						<tr><td colspan="4"></td></tr>
	  
	  <tr>
		<td colspan="3" valign="top">C.Name and Address of the establishment</td>
		
		<td >'.$name_address.'</td>
		
	   
	   
	  </tr>
	  <tr><td colspan="4"></td></tr>
	  <tr>
		<td colspan="3" valign="top">D.Type of business, trade, industry, manufacture or occupation which is carried on in the establishment</td>
		
		<td  valign="top">'.$nature_work.'</td>
	  
		
	  </tr>
	  <tr><td colspan="4"></td></tr>
	  <tr>
		<td colspan="3"  valign="top">E.Establishment Registration Details Under Particular Act</td>
		
		<td valign="top" >'.$establish_details.'</td>
		
	   
	  </tr>
	  <tr><td colspan="4"></td></tr>
	  <tr>
		<td colspan="3" valign="top">F.Name and Address of the principal employer</td>
		
		<td>'.$principal_details.'</td>
		
	   
	  </tr>
	 <tr><td colspan="4"></td></tr>
	 
	  <tr>
		<td colspan="3">G.Nature of work in which contract labour is employed or is to be employed in the establishment</td>
	
		<td valign="top">'.$nature_work.'</td>
	  
		
	  </tr>
	  <tr><td colspan="4"></td></tr>
	  <tr>
		<td colspan="3">H.Duration of the proposed contract work (give particulars of proposed date of ending)
	</td>
		<td valign="top">'.$startend_date.'</td>
	  
	   
	  </tr>
	  </table>';	
			
			
			$string="1.Information provided by Principal Employer";
													 
			$form['renewla_profile']['contarctor_namesssss'] = array(
					'#prefix' 				=> '<fieldset><legend>'.$string.'</legend><div class="row"><section ><label class="input">',
					'#suffix' 				=> '</label></section></fieldset>',						
					'#type' 				=> 'markup',
					'#markup' 				=>  $output
												
				);							  
										  
										  
		///------------------------------------------------------------END---------------------------------------------
		
		$form['renewla_profile']['contractor_personal'] = array(
					'#prefix' 				=> '<fieldset><legend>2.Personal Information(Incase of Individuals)</legend>',
					'#suffix' 				=> '',						
					'#type' 				=> 'markup',
					
												
				);	
				
				
		 $form['renewla_profile']['contarctor_dob1'] 	= array(
								'#prefix' 					=>'<div class="row"><section class="col col-6"><label class="input">',
								'#suffix' 					=> 	'</label></section>',
								'#type' 					=> 	'textfield',
								'#title'					=>	'A.Date of Birth:',
								'#attributes'				=>	$readonly_contarctor_dob,
								'#default_value' 			=> 	!empty($dob_contractor) ? $dob_contractor : ''
							);		
			
				$form['renewla_profile']['contarctor_fathername'] = array(
					'#prefix' 			=>  '<section class="col col-6"><div id="add_fathername"><label class="input">',
					'#suffix' 			=>  '</label></div></section></div>',
					'#title'			=>	'B.Father name of Contractor :',
					'#size'				=>	t('20'),	
					'#type'				=> 	'textfield',
					'#default_value' 	=>	!empty($father_contarctor_name) ? $father_contarctor_name : '',
					'#attributes' 		=> 	$readonly_father_contarctor_name,
										
				);	
				$form['renewla_profile']['contarctor_age'] = array(
						'#prefix'				=> '<div class="row"><section class="col col-6"><label class="input">',
						'#suffix'				=>	'</label></section>',
						'#title'				=>	'C.Age:',
						'#type' 				=> 'textfield',
						'#attributes' 			=> 	array('placeholder' => 'Age'),
						'#default_value' 		=>	!empty($age_contractor) ? $age_contractor : '',
						'#attributes'			=>	$readonly_contarctor_age,
						//'#element_validate'		=>  array('agenumaric_valiadtion') 
					  );
					  
					if(empty($editable_cooparative)){
					$form['renewla_profile']['is_coparative'] = array(
												'#prefix' 			=> '<section class="col col-6"><div class="inline-group">',
												'#suffix' 			=> '</div></section></div></fieldset>',
												'#type' 			=> 'radios',
												'#title' 			=>  t('D.Co-oparative Society <span class="correction_show con_cate"></span>'),     
												'#options' 			=> array('1'=>t('Yes'),'0'=>t('No')),
												'#required' 		=> TRUE,
												'#default_value' 	=> !empty($is_coparative_data) ? $is_coparative_data : 0,
											//	'#attributes'		=>	$readonly_cooparative,
												
												
											
											 );
			}if($editable_cooparative == 1){
				
				
			 $form['renewla_profile']['editable_cooparative'] = array(									 
										  '#type' 			=> 	'hidden',
										  '#default_value'  =>	!empty($editable_cooparative) ? $editable_cooparative : '0',
																				
										  );
										  
				$form['renewla_profile']['is_coparative_data'] = array(									 
										  '#type' 			=> 	'hidden',
										  '#default_value'  =>	!empty($is_coparative_data) ? $is_coparative_data : '0',
																				
										  );
			
				$form['renewla_profile']['is_coparative_show'] = array(
										'#prefix' 			=> '<section class="col col-6"><label class="input">',
										'#suffix' 			=> '</label></section></div></fieldset>',
										'#type' 			=> 'textfield',
										'#title' 			=> t('D.Co-oparative Society'),  
										'#default_value' 	=> !empty($is_coparative_data) ? 'Yes':'No',
										'#attributes' 		=> 	$readonly_cooparative,
								);
			}
											  
		
											
				 $form['renewla_profile']['contarctor_address_line'] = array(
						'#prefix' 				=> '<fieldset><legend>3.Please Give Address Details</legend><div class="row"><section class="col col-14"><label class="input">',
						'#suffix' 				=> '</label></section></div>',
						'#title'				=>	'A.Address Line1 of contractor:',
						'#type' 				=> 	'textfield',
						'#size'					=>	120,
						'#default_value' 		=>	!empty($address_line1_of_contractor) ? ($address_line1_of_contractor) : '',
						'#attributes' 			=> 	array('placeholder' => 'Address Line1 the contractor',),
						'#required' 			=> 	TRUE,
						'#attributes'			=>	$readonly_address_line1,
					);
									
				$options_dist 	= get_district_dashboard();	
																	
				$form['renewla_profile']['contract_dist'] = array(			
					  '#prefix'				=>	'<div class="row"><section class="col col-6"><label class="select">',
					  '#suffix'				=>	'</label></section>',
					  '#title'				=>	'B.Select District ',
					  '#type' 			    => 	'select',
					  '#options' 			=> $options_dist,
					  '#default_value' 		=> !empty($contractor_dist) ? $contractor_dist : '',
					  '#required' 			=> TRUE,
					  '#attributes'			=> $readonly_contract_dist,
					  '#ajax'				=> array(
															'event' 			=> 'change',
															'effect' 			=> 'fade',			
															'callback'			=> 'get_contractor_address',
															'progress'			=> '',
														  )	
					
					 
											 
											);	
																			
			$value_contract_dist = 	isset($form_state['values']['contract_dist']) ? $form_state['values']['contract_dist'] : $contractor_dist;
			
			if(empty($value_contract_dist)){		
				$form['renewla_profile']['contract_subdivision'] = array(
					  '#prefix' 			=> '<div id="contractor_subdvsn_replace"><section class="col col-6"><label class="select">',
					  '#suffix' 			=> '</label></section></div>',
					  '#title'				=> 'C.Select Subdivision',
					  '#type' 				=> 'select',
					  '#options' 			=> array(),
					  '#required' 			=> TRUE,
					  '#attributes' 		=>  $readonly_contract_subdivision,
					  '#ajax'				=> array(
												'event' 			=> 'change',
												'effect' 			=> 'fade',			
												'callback'			=> 'get_contractor_address',
												'progress'			=> '',
											  ),
					);
		  }else{		
				$form['renewla_profile']['contract_subdivision'] = array(
					  '#prefix' 			=> '<div id="contractor_subdvsn_replace"><section class="col col-6"><label class="select">',
					  '#suffix' 			=> '</label></section></div>',
					  '#title'				=> 'C.Select Subdivision',
					  '#type' 				=> 'select',
					  '#options' 			=> get_subdivisionlist_dashboard($value_contract_dist),
					  '#required' 			=> TRUE,
					  '#attributes' 		=>  $readonly_contract_subdivision,
					  '#default_value' 		=> !empty($contractor_sub_division) ? $contractor_sub_division : '',
					  '#ajax'				=> array(
												'event' 			=> 'change',
												'effect' 			=> 'fade',			
												'callback'			=> 'get_contractor_address',
												'progress'			=> '',
											  ),
				);
		}
	
			$value_contract_subdivision	= isset($form_state['values']['contract_subdivision']) ? $form_state['values']['contract_subdivision'] : $contractor_sub_division;
						
			$areatypeOptions = get_areatype_dashboard($value_contract_dist, $value_contract_subdivision);			
			
			if(empty($areatypeOptions)){
				$form['renewla_profile']['contract_areatype']  = array(
					  '#prefix' 			=> '<div id="contractor_areatype_replace"><section class="col col-6"><label class="select">',
					  '#suffix' 			=> '</label></section></div>',
					  '#title'				=> 'D.Select Areatype',
					  '#type' 				=> 'select',
					  '#options' 			=> array(),
					  '#attributes' 		=>  $readonly_contract_areatype,
					  '#required' 			=> TRUE,
					  
					  '#ajax'				=> array(
												'event' 			=> 'change',
												'effect' 			=> 'fade',			
												'callback'			=> 'get_contractor_address',
												'progress'			=> '',
											  ),
						);
			}else{
			
				$form['renewla_profile']['contract_areatype']  = array(
					  '#prefix' 			=> '<div id="contractor_areatype_replace"><section class="col col-6"><label class="select">',
					  '#suffix' 			=> '</label></section></div>',
					  '#title'				=> 'D.Select Areatype',
					  '#type' 				=> 'select',
					  '#options' 			=> $areatypeOptions,
					  '#attributes' 		=>  $readonly_contract_areatype,
					  '#required' 			=> TRUE,
					  '#default_value' 		=> !empty($contractor_area_type) ? $contractor_area_type : '',
					  '#ajax'				=> array(
												'event' 			=> 'change',
												'effect' 			=> 'fade',			
												'callback'			=> 'get_contractor_address',
												'progress'			=> '',
											  ),
						);
			}			
			
			$value_contract_areatype = isset($form_state['values']['contract_areatype']) ? $form_state['values']['contract_areatype'] : $contractor_area_type;
			
			$form['renewla_profile']['name_contractor_areatype'] = array(
	
					 '#prefix' 				=> '<div id="name_contractor_areatype_replace"><section class="col col-6"><label class="select">',
					 '#suffix' 				=> '</label></section></div>',
					 '#title'				=> 'E.Select Block/Municipality/Corporation/SEZ/Notified Area',
					 '#type' 				=> 'select',
					 '#options' 			=> array(),
					 '#attributes' 			=>  $readonly_contract_areatype,
					 '#required' 			=> TRUE,
					 
					
					 '#ajax'				=> array(
												'event' 			=> 'change',
												'effect' 			=> 'fade',			
												'callback'			=> 'get_contractor_address',
												'progress'			=> '',
											  )
				);
	
			if($value_contract_dist != "" && $value_contract_subdivision != ""){
				if($value_contract_areatype != ""){
					if($value_contract_areatype == 'B'){
						$form['renewla_profile']['name_contractor_areatype'] = array(
							  '#prefix' 			=>'<div id="name_contractor_areatype_replace"><section class="col col-6"><label class="select">',
							  '#suffix' 			=> '</label></section></div>',
							  '#title'				=> 'E.Select Block',
							  '#type' 				=> 'select',
							  '#options' 			=> get_block_dashboard($value_contract_subdivision, $value_contract_areatype),
							  '#required' 			=> TRUE,
							  '#attributes' 		=>  $readonly_contract_block,
							  '#default_value' 		=> !empty($contractorname_area_type) ? $contractorname_area_type : '',
							  '#ajax'				=> array(
																'event' 			=> 'change',
																'effect' 			=> 'fade',			
																'callback'			=> 'get_contractor_address',
																'progress'			=> '',
													  )
							);
					}if($value_contract_areatype == 'M'){
						$form['renewla_profile']['name_contractor_areatype'] = array(
							  '#prefix' 			=>'<div id="name_contractor_areatype_replace"><section class="col col-6"><label class="select">',
							  '#suffix' 			=> '</label></section></div>',
							  '#title'				=> 'E.Select Municipality',
							  '#type' 				=> 'select',
							  '#options' 			=> get_block_dashboard($value_contract_subdivision, $value_contract_areatype),
							  '#required' 			=> TRUE,
							  '#attributes' 		=>  $readonly_contract_mun,
							  '#default_value' 		=> !empty($contractorname_area_type) ? $contractorname_area_type : '',
							  '#ajax'				=> array(
														'event' 			=> 'change',
														'effect' 			=> 'fade',			
														'callback'			=> 'get_contractor_address',
														'progress'			=> '',
													  )
						 );
					}if($value_contract_areatype == 'C'){
					$form['renewla_profile']['name_contractor_areatype'] = array(
						  '#prefix' 			=>'<div id="name_contractor_areatype_replace"><section class="col col-6"><label class="select">',
						  '#suffix' 			=> '</label></section></div>',
						  '#title'				=> 'E.Select Corporation',
						  '#type' 				=> 'select',
						  '#options' 			=> get_block_dashboard($value_contract_subdivision, $value_contract_areatype),
						  '#required' 			=> TRUE,
						  '#attributes' 		=>  $readonly_contract_corp,
						  '#default_value' 		=> !empty($contractorname_area_type) ? $contractorname_area_type : '',
						  '#ajax'				=> array(
															'event' 			=> 'change',
															'effect' 			=> 'fade',			
															'callback'			=> 'get_contractor_address',
															'progress'			=> '',
												  )
						);
					}if($value_contract_areatype == 'S'){
							$form['renewla_profile']['name_contractor_areatype'] = array(
								  '#prefix' 			=>'<div id="name_contractor_areatype_replace"><section class="col col-6"><label class="select">',
								  '#suffix' 			=> '</label></section></div>',
								  '#title'				=> 'E.Select SEZ',
								  '#type' 				=> 'select',
								  '#options' 			=> get_block_dashboard($value_contract_subdivision, $value_contract_areatype),
								  '#required' 			=> TRUE,
								  '#attributes' 		=>  $readonly_contract_sez,
								  '#default_value' 		=> !empty($contractorname_area_type) ? $contractorname_area_type : '',
								  '#ajax'				=> array(
																	'event' 			=> 'change',
																	'effect' 			=> 'fade',			
																	'callback'			=> 'get_contractor_address',
																	'progress'			=> '',
														  )
								);
						}if($value_contract_areatype == 'N'){
							$form['renewla_profile']['name_contractor_areatype'] = array(
								  '#prefix' 			=>'<div id="name_contractor_areatype_replace"><section class="col col-6"><label class="select">',
								  '#suffix' 			=> '</label></section></div>',
								  '#title'				=> 'E.Select Notified Area',
								  '#type' 				=> 'select',
								  '#options' 			=> get_block_dashboard($value_contract_subdivision, $value_contract_areatype),
								  '#required' 			=> TRUE,
								  '#attributes' 		=>  $readonly_contract_sez,
								  '#default_value' 		=> !empty($contractorname_area_type) ? $contractorname_area_type : '',
								  '#ajax'				=> array(
																	'event' 			=> 'change',
																	'effect' 			=> 'fade',			
																	'callback'			=> 'get_contractor_address',
																	'progress'			=> '',
														  )
								);
						}
				}
			}
	
												
			$value_name_contractor_areatype = isset($form_state['values']['name_contractor_areatype']) ? $form_state['values']['name_contractor_areatype'] : $contractorname_area_type;
				
			$form['renewla_profile']['contractor_vill_ward'] = array(
					 '#prefix' 				=> '<div id="contractor_vill_ward_replace"><section class="col col-4"><label class="select">',
					 '#suffix' 				=> '</label></section></div>',
					 '#title'				=> 'F.Select Gram Panchayat/Ward',
					 '#type' 				=> 'select',
					 '#options' 			=> array(),
					 '#attributes' 			=>  $readonly_contract_vill,
					 '#required' 			=> TRUE,
					
					 '#ajax'				=> array(
														'event' 			=> 'change',
														'effect' 			=> 'fade',			
														'callback'			=> 'get_contractor_address',
														'progress'			=> '',
												)		
					);
				if($value_contract_dist != "" && $value_contract_subdivision != ""){
					if($value_contract_areatype != ""){
						if($value_name_contractor_areatype != ""){
							if($value_contract_areatype == 'B'){
								$form['renewla_profile']['contractor_vill_ward'] 	= array(
										 '#prefix' 				=> '<div id="contractor_vill_ward_replace"><section class="col col-4"><label class="select">',
										 '#suffix' 				=> '</label></section></div>',
										 '#title'				=> 'F.Select Gram Panchayat',
										 '#type' 				=> 'select',
										 '#options'				=> get_villward_dashboard($value_name_contractor_areatype,'V'),
										 '#attributes' 			=>  $readonly_contract_vill,
										 '#default_value' 		=> !empty($contractor_villward) ? $contractor_villward : '',
										 '#required' 			=> TRUE,
										
										 '#ajax'				=> array(
																	'event' 			=> 'change',
																	'effect' 			=> 'fade',			
																	'callback'			=> 'get_contractor_address',
																	'progress'			=> '',
																  )
			
			
								 );
							}if($value_contract_areatype == 'M'){
									
									$form['renewla_profile']['contractor_vill_ward'] 	= array(
											  '#prefix' 			=> '<div id="contractor_vill_ward_replace"><section class="col col-4"><label class="select">',
											  '#suffix' 			=> '</label></section></div>',
											  '#title'				=> 'F.Select Ward',
											  '#type' 				=> 'select',
											  '#options' 			=> get_villward_dashboard($value_name_contractor_areatype,'W'),
											  '#required' 			=> TRUE,
											  '#attributes' 		=>  $readonly_contract_ward,
											  '#default_value' 		=> !empty($contractor_villward) ? $contractor_villward : '',
											  '#ajax'				=> array(
																				'event' 			=> 'change',
																				'effect' 			=> 'fade',			
																				'callback'			=> 'get_contractor_address',
																				'progress'			=> '',
																		  )
										);
																									
							}if($value_contract_areatype == 'C'){
									
									$form['renewla_profile']['contractor_vill_ward'] 	= array(
										  '#prefix' 			=> '<div id="contractor_vill_ward_replace"><section class="col col-4"><label class="select">',
										  '#suffix' 			=> '</label></section></div>',
										  '#title'				=> 'F.Select Ward',
										  '#type' 				=> 'select',
										  '#options' 			=> get_corporation_dasboard($value_name_contractor_areatype),
										  '#required' 			=> TRUE,
										  '#attributes' 		=>  $readonly_contract_ward,
										  '#default_value' 		=> !empty($contractor_villward) ? $contractor_villward : '',
										  '#ajax'				=> array(
																			'event' 			=> 'change',
																			'effect' 			=> 'fade',			
																			'callback'			=> 'get_contractor_address',
																			'progress'			=> '',
																	  )
										);
																									
							}if($value_contract_areatype == 'N'){
									
									$form['renewla_profile']['contractor_vill_ward'] 	= array(
											  '#prefix' 			=> '<div id="contractor_vill_ward_replace"><section class="col col-4"><label class="select">',
											  '#suffix' 			=> '</label></section></div>',
											  '#title'				=> 'F.Select Notified',
											  '#type' 				=> 'select',
											  '#options' 			=> get_villward_dashboard($value_name_contractor_areatype,'N'),
											  '#required' 			=> TRUE,
											  '#attributes' 		=>  $readonly_contract_ward,
											  '#default_value' 		=> !empty($contractor_villward) ? $contractor_villward : '',
											  '#ajax'				=> array(
																				'event' 			=> 'change',
																				'effect' 			=> 'fade',			
																				'callback'			=> 'get_contractor_address',
																				'progress'			=> '',
																		  )
										);
						}if($value_contract_areatype == 'S'){
									
									$form['renewla_profile']['contractor_vill_ward'] 	= array(
											  '#prefix' 			=> '<div id="contractor_vill_ward_replace"><section class="col col-4"><label class="select">',
											  '#suffix' 			=> '</label></section></div>',
											  '#title'				=> 'F.Select Sector',
											  '#type' 				=> 'select',
											  '#options' 			=> get_villward_dashboard($value_name_contractor_areatype,'S'),
											  '#required' 			=> TRUE,
											  '#attributes' 		=>  $readonly_contract_ward,
											  '#default_value' 		=> !empty($contractor_villward) ? $contractor_villward : '',
											  '#ajax'				=> array(
																				'event' 			=> 'change',
																				'effect' 			=> 'fade',			
																				'callback'			=> 'get_contractor_address',
																				'progress'			=> '',
																		  )
										);
						}
						
						}
						
					}
				}
				$value_contractor_villward 	= isset($form_state['values']['contractor_vill_ward']) ? $form_state['values']['contractor_vill_ward'] : $contractor_villward;
				$form['renewla_profile']['contractor_ps'] = array(
						'#prefix' 				=> '<div id="contractor_ps_replace"><section class="col col-4"><label class="select">',
						'#suffix' 				=> '</label></section></div>',
						'#title'				=> 'G.Select Police Station',
						'#type' 				=> 'select',
						'#options' 				=> array(),
						'#required' 			=> TRUE,
						'#attributes' 			=>  $readonly_contractor_ps,
						'#ajax'					=> array(
															'event' 			=> 'change',
															'effect' 			=> 'fade',			
															'callback'			=> 'get_contractor_address',
															'progress'			=> '',
												  )
				  );
																									  
																									  
																									  
				if($value_contract_dist != "" && $value_contract_subdivision != ""){
					if($value_contract_areatype != ""){
						if($value_name_contractor_areatype != ""){
							if($value_contractor_villward != ""){
							$form['renewla_profile']['contractor_ps'] 	= array(
									  '#prefix' 			=> '<div id="contractor_ps_replace"><section class="col col-4"><label class="select">',
									  '#suffix' 			=> '</label></section></div>',
									  '#title'				=> 'G.Select Police Station',
									  '#type' 				=> 'select',
									  '#default_value' 		=> !empty($contractor_police_station) ? $contractor_police_station : '',
									  '#options' 			=> get_ps_dashboard($value_contract_dist),
									  '#required' 			=> TRUE,
									  '#attributes' 		=>  $readonly_contractor_ps,
								);
							}
						}
					}
				}
				$form['renewla_profile']['contractor_pin'] = array(			
					  '#prefix' 		  => '<section class="col col-4"><label class="input">',
					  '#suffix' 		  => '</label></section></div></fieldset>',
					  '#title'			  => 'H.PIN Code ',
					  '#size'			  => t('20'),	
					  '#type' 			  => 'textfield',
					  '#attributes' 	  => $readonly_pin,												
					  '#default_value'	  => !empty($contractor_pin) ? $contractor_pin	:'',
					  '#required'		  => TRUE,
					  '#element_validate' => array('validate_integer')												 
					);					 
	
		  
		  
		  
		  
		  
		  //--------------------------------------------------------Worksite------------------------------------------------------------
			
			
			 $form['renewla_profile']['worksite_address_line'] = array(
						'#prefix' 				=> '<fieldset><legend>4.Please Give Worksite Address Details</legend><div class="row"><section class="col col-14"><label class="input">',
						'#suffix' 				=> '</label></section></div>',
						'#title'				=>	'A.Address Line1 of contractor:',
						'#type' 				=> 	'textfield',
						'#size'					=>	120,
						'#default_value' 		=>	!empty($worksite_address_line) ? ($worksite_address_line) : $loc_e_name,
						'#attributes' 			=> 	array('placeholder' => 'Address Line1 the contractor'),
						'#required' 			=> 	TRUE,
						'#attributes'			=>	$readonly_worksite_address_line1,
					);
					
			$form['renewla_profile']['worksite_dist'] = array(			
						  '#prefix'				=>	'<div class="row"><section class="col col-6"><label class="select">',
						  '#suffix'				=>	'</label></section>',
						  '#title'				=>	'B.Select District ',
						  '#type' 			    => 	'select',
						  '#options' 			=> $options_dist,
						  '#default_value' 		=> !empty($worksite_dist) ? $worksite_dist : $loc_e_dist,
						  '#required' 			=> TRUE,
						  '#attributes'			=> $readonly_worksite_dist,
						  '#ajax'				=> array(
																	'event' 			=> 'change',
																	'effect' 			=> 'fade',			
																	'callback'			=> 'get_worksite_address',
																	'progress'			=> '',
															 )	
					
					);	
																			
			//$value_worksite_dist 	= 	isset($form_state['values']['worksite_dist']) ? ($form_state['values']['worksite_dist']): ((!empty($worksite_dist) ? $worksite_dist :$loc_e_dist));
			
			$value_worksite_dist_1	= 	isset($form_state['values']['worksite_dist']) ? $form_state['values']['worksite_dist'] :(!empty($worksite_dist) ? $worksite_dist :$loc_e_dist) ;
			
			if(empty($value_worksite_dist_1)){		
				$form['renewla_profile']['worksite_subdivision'] = array(
					  '#prefix' 			=> '<div id="worksite_subdvsn_replace_1"><section class="col col-6"><label class="select">',
					  '#suffix' 			=> '</label></section></div>',
					  '#title'				=> 'C.Select Subdivision',
					  '#type' 				=> 'select',
					  '#options' 			=> array(),
					  '#required' 			=> TRUE,
					  '#attributes' 		=>  $readonly_worksite_subdivision,
					  '#ajax'				=> array(
												'event' 			=> 'change',
												'effect' 			=> 'fade',			
												'callback'			=> 'get_worksite_address',
												'progress'			=> '',
											  ),
					);
		  }else{	
				$form['renewla_profile']['worksite_subdivision'] = array(
					  '#prefix' 			=> '<div id="worksite_subdvsn_replace_1"><section class="col col-6"><label class="select">',
					  '#suffix' 			=> '</label></section></div>',
					  '#title'				=> 'C.Select Subdivision',
					  '#type' 				=> 'select',
					  '#options' 			=> get_subdivisionlist_dashboard($value_worksite_dist_1),
					  '#required' 			=> TRUE,
					  '#attributes' 		=>  $readonly_worksite_subdivision,
					  '#default_value' 		=> !empty($worksite_subdivision) ? $worksite_subdivision : $loc_e_subdivision,   
					  '#ajax'				=> array(
														'event' 			=> 'change',
														'effect' 			=> 'fade',			
														'callback'			=> 'get_worksite_address',
														'progress'			=> '',
											  ),
				);
		}
		
		$value_worksite_subdivision_1	= isset($form_state['values']['worksite_subdivision']) ? $form_state['values']['worksite_subdivision'] : (!empty($worksite_subdivision) ? $worksite_subdivision : $loc_e_subdivision);
		$areatypeOptions_wirksite_1 = get_areatype_dashboard($value_worksite_dist_1, $value_worksite_subdivision_1);
		
		
		if(empty($areatypeOptions_wirksite_1)){
				$form['renewla_profile']['worksite_areatype']  = array(
					  '#prefix' 			=> '<div id="worksite_areatype_replace"><section class="col col-6"><label class="select">',
					  '#suffix' 			=> '</label></section></div>',
					  '#title'				=> 'D.Select Areatype',
					  '#type' 				=> 'select',
					  '#options' 			=> array(),
					  '#attributes' 		=>  $readonly_worksite_areatype,
					  '#required' 			=> TRUE,
					  
					  '#ajax'				=> array(
												'event' 			=> 'change',
												'effect' 			=> 'fade',			
												'callback'			=> 'get_worksite_address',
												'progress'			=> '',
											  ),
						);
			}else{
			
				$form['renewla_profile']['worksite_areatype']  = array(
					  '#prefix' 			=> '<div id="worksite_areatype_replace"><section class="col col-6"><label class="select">',
					  '#suffix' 			=> '</label></section></div>',
					  '#title'				=> 'D.Select Areatype',
					  '#type' 				=> 'select',
					  '#options' 			=> $areatypeOptions_wirksite_1,
					  '#attributes' 		=>  $readonly_worksite_areatype,
					  '#required' 			=> TRUE,
					  '#default_value' 		=> !empty($work_site_areatype) ? $work_site_areatype : $loc_e_areatype,
					  '#ajax'				=> array(
												'event' 			=> 'change',
												'effect' 			=> 'fade',			
												'callback'			=> 'get_worksite_address',
												'progress'			=> '',
											  ),
						);
			}
			
		$value_worksite_areatype_1 = isset($form_state['values']['worksite_areatype']) ? $form_state['values']['worksite_areatype'] : (!empty($work_site_areatype) ? $work_site_areatype :$loc_e_areatype);
		
			
			$form['renewla_profile']['name_worksite_areatype'] = array(
					 '#prefix' 				=> '<div id="name_worksite_areatype_replace"><section class="col col-6"><label class="select">',
					 '#suffix' 				=> '</label></section></div>',
					 '#title'				=> 'E.Select Block/Municipality/Corporation/SEZ/Notified Area',
					 '#type' 				=> 'select',
					 '#options' 			=> array(),
					 '#attributes' 			=>  $readonly_worksite_areatype,
					 '#required' 			=> TRUE,
					 
					
					 '#ajax'				=> array(
												'event' 			=> 'change',
												'effect' 			=> 'fade',			
												'callback'			=> 'get_worksite_address',
												'progress'			=> '',
											  )
				);
	
			if($value_worksite_dist_1 != "" && $value_worksite_subdivision_1 != ""){
				if($value_worksite_areatype_1 != ""){
					if($value_worksite_areatype_1 == 'B'){
						$form['renewla_profile']['name_worksite_areatype'] = array(
							  '#prefix' 			=>'<div id="name_worksite_areatype_replace"><section class="col col-6"><label class="select">',
							  '#suffix' 			=> '</label></section></div>',
							  '#title'				=> 'E.Select Block',
							  '#type' 				=> 'select',
							  '#options' 			=> get_block_dashboard($value_worksite_subdivision_1, $value_worksite_areatype_1),
							  '#required' 			=> TRUE,
							  '#attributes' 		=>  $readonly_worksite_block,
							  '#default_value' 		=> !empty($name_work_site_areatype) ? $name_work_site_areatype : $name_areatype,
							  '#ajax'				=> array(
																'event' 			=> 'change',
																'effect' 			=> 'fade',			
																'callback'			=> 'get_worksite_address',
																'progress'			=> '',
													  )
							);
					}if($value_worksite_areatype_1 == 'M'){
						$form['renewla_profile']['name_worksite_areatype'] = array(
							  '#prefix' 			=>'<div id="name_worksite_areatype_replace"><section class="col col-6"><label class="select">',
							  '#suffix' 			=> '</label></section></div>',
							  '#title'				=> 'E.Select Municipality',
							  '#type' 				=> 'select',
							  '#options' 			=> get_block_dashboard($value_worksite_subdivision_1, $value_worksite_areatype_1),
							  '#required' 			=> TRUE,
							  '#attributes' 		=>  $readonly_worksite_mun,
							  '#default_value' 		=> !empty($name_work_site_areatype) ? $name_work_site_areatype : $name_areatype,
							  '#ajax'				=> array(
														'event' 			=> 'change',
														'effect' 			=> 'fade',			
														'callback'			=> 'get_worksite_address',
														'progress'			=> '',
													  )
						 );
					}if($value_worksite_areatype_1 == 'C'){
					$form['renewla_profile']['name_worksite_areatype'] = array(
						  '#prefix' 			=>'<div id="name_worksite_areatype_replace"><section class="col col-6"><label class="select">',
						  '#suffix' 			=> '</label></section></div>',
						  '#title'				=> 'E.Select Corporation',
						  '#type' 				=> 'select',
						  '#options' 			=> get_block_dashboard($value_worksite_subdivision_1, $value_worksite_areatype_1),
						  '#required' 			=> TRUE,
						  '#attributes' 		=>  $readonly_worksite_corp,
						  '#default_value' 		=> !empty($name_work_site_areatype) ? $name_work_site_areatype : $name_areatype,
						  '#ajax'				=> array(
															'event' 			=> 'change',
															'effect' 			=> 'fade',			
															'callback'			=> 'get_worksite_address',
															'progress'			=> '',
												  )
						);
					}if($value_worksite_areatype_1 == 'S'){
							$form['renewla_profile']['name_worksite_areatype'] = array(
								  '#prefix' 			=>'<div id="name_worksite_areatype_replace"><section class="col col-6"><label class="select">',
								  '#suffix' 			=> '</label></section></div>',
								  '#title'				=> 'E.Select SEZ',
								  '#type' 				=> 'select',
								  '#options' 			=> get_block_dashboard($value_worksite_subdivision_1, $value_worksite_areatype_1),
								  '#required' 			=> TRUE,
								  '#attributes' 		=>  $readonly_worksite_sez,
								  '#default_value' 		=> !empty($name_work_site_areatype) ? $name_work_site_areatype :$name_areatype ,
								  '#ajax'				=> array(
																	'event' 			=> 'change',
																	'effect' 			=> 'fade',			
																	'callback'			=> 'get_worksite_address',
																	'progress'			=> '',
														  )
								);
						}if($value_worksite_areatype_1 == 'N'){
							$form['renewla_profile']['name_worksite_areatype'] = array(
								  '#prefix' 			=>'<div id="name_worksite_areatype_replace"><section class="col col-6"><label class="select">',
								  '#suffix' 			=> '</label></section></div>',
								  '#title'				=> 'E.Select Notified Area',
								  '#type' 				=> 'select',
								  '#options' 			=> get_block_dashboard($value_worksite_subdivision_1, $value_worksite_areatype_1),
								  '#required' 			=> TRUE,
								  '#attributes' 		=>  $readonly_worksite_not,
								  '#default_value' 		=> !empty($name_work_site_areatype) ? $name_work_site_areatype : $name_areatype,
								  '#ajax'				=> array(
																	'event' 			=> 'change',
																	'effect' 			=> 'fade',			
																	'callback'			=> 'get_worksite_address',
																	'progress'			=> '',
														  )
								);
						}
				}
			}
	
												
			$value_name_worksite_areatype_1 = isset($form_state['values']['name_worksite_areatype']) ? $form_state['values']['name_worksite_areatype'] : (!empty($name_work_site_areatype) ? $name_work_site_areatype :$name_areatype);
				
			$form['renewla_profile']['worksite_vill_ward'] = array(
					 '#prefix' 				=> '<div id="worksite_vill_ward_replace"><section class="col col-4"><label class="select">',
					 '#suffix' 				=> '</label></section></div>',
					 '#title'				=> 'F.Select Gram Panchayat/Ward',
					 '#type' 				=> 'select',
					 '#options' 			=> array(),
					 '#attributes' 			=>  $readonly_worksite_vill,
					 '#required' 			=> TRUE,
					
					 '#ajax'				=> array(
														'event' 			=> 'change',
														'effect' 			=> 'fade',			
														'callback'			=> 'get_worksite_address',
														'progress'			=> '',
												)		
					);
				if($value_worksite_dist_1 != "" && $value_worksite_subdivision_1 != ""){
					if($value_worksite_areatype_1 != ""){
						if($value_name_worksite_areatype_1 != ""){
							if($value_worksite_areatype_1 == 'B'){
								$form['renewla_profile']['worksite_vill_ward'] 	= array(
										 '#prefix' 				=> '<div id="worksite_vill_ward_replace"><section class="col col-4"><label class="select">',
										 '#suffix' 				=> '</label></section></div>',
										 '#title'				=> 'F.Select Gram Panchayat',
										 '#type' 				=> 'select',
										 '#options'				=> get_villward_dashboard($value_name_worksite_areatype_1,'V'),
										 '#attributes' 			=>  $readonly_worksite_vill,
										 '#default_value' 		=> !empty($work_site_vill_ward) ? $work_site_vill_ward : $loc_e_vill_ward,
										 '#required' 			=> TRUE,
										
										 '#ajax'				=> array(
																	'event' 			=> 'change',
																	'effect' 			=> 'fade',			
																	'callback'			=> 'get_worksite_address',
																	'progress'			=> '',
																  )
			
			
								 );
							}if($value_worksite_areatype_1 == 'M'){
									
									$form['renewla_profile']['worksite_vill_ward'] 	= array(
											  '#prefix' 			=> '<div id="worksite_vill_ward_replace"><section class="col col-4"><label class="select">',
											  '#suffix' 			=> '</label></section></div>',
											  '#title'				=> 'F.Select Ward',
											  '#type' 				=> 'select',
											   '#options'			=> get_villward_dashboard($value_name_worksite_areatype_1,'W'),
											  '#required' 			=> TRUE,
											  '#attributes' 		=> $readonly_worksite_ward,
											 '#default_value' 		=> !empty($work_site_vill_ward) ? $work_site_vill_ward : $loc_e_vill_ward,
											  '#ajax'				=> array(
																				'event' 			=> 'change',
																				'effect' 			=> 'fade',			
																				'callback'			=> 'get_worksite_address',
																				'progress'			=> '',
																		  )
										);
																									
							}if($value_worksite_areatype_1 == 'C'){
									
									$form['renewla_profile']['worksite_vill_ward'] 	= array(
										  '#prefix' 			=> '<div id="worksite_vill_ward_replace"><section class="col col-4"><label class="select">',
										  '#suffix' 			=> '</label></section></div>',
										  '#title'				=> 'F.Select Ward',
										  '#type' 				=> 'select',
										  '#options' 			=> get_corporation_dasboard($value_name_worksite_areatype_1),
										  '#required' 			=> TRUE,
										  '#attributes' 		=>  $readonly_worksite_ward,
										 '#default_value' 		=> !empty($work_site_vill_ward) ? $work_site_vill_ward : $loc_e_vill_ward,
										  '#ajax'				=> array(
																			'event' 			=> 'change',
																			'effect' 			=> 'fade',			
																			'callback'			=> 'get_worksite_address',
																			'progress'			=> '',
																	  )
										);
																									
							}
							if($value_worksite_areatype_1 == 'N'){ //$p = get_villward_dashboard($value_name_worksite_areatype_1,'N');print_r($p);die($loc_e_vill_ward);
									
									$form['renewla_profile']['worksite_vill_ward'] 	= array(
										  '#prefix' 			=> '<div id="worksite_vill_ward_replace"><section class="col col-4"><label class="select">',
										  '#suffix' 			=> '</label></section></div>',
										  '#title'				=> 'F.Select Notified Area',
										  '#type' 				=> 'select',
										  '#options' 			=> get_villward_dashboard($value_name_worksite_areatype_1,'N'),
										  '#required' 			=> TRUE,
										  '#attributes' 		=>  $readonly_worksite_ward,
										 '#default_value' 		=> !empty($work_site_vill_ward) ? $work_site_vill_ward : $loc_e_vill_ward,
										  '#ajax'				=> array(
																			'event' 			=> 'change',
																			'effect' 			=> 'fade',			
																			'callback'			=> 'get_worksite_address',
																			'progress'			=> '',
																	  )
										);
																									
							}if($value_worksite_areatype_1 == 'S'){
									
									$form['renewla_profile']['worksite_vill_ward'] 	= array(
											  '#prefix' 			=> '<div id="worksite_vill_ward_replace"><section class="col col-4"><label class="select">',
											  '#suffix' 			=> '</label></section></div>',
											  '#title'				=> 'F.Select Sector',
											  '#type' 				=> 'select',
											  '#options' 			=> get_villward_dashboard($value_name_worksite_areatype_1,'S'),
											  '#required' 			=> TRUE,
											  '#attributes' 		=>  $readonly_worksite_ward,
											  '#default_value' 		=> !empty($work_site_vill_ward) ? $work_site_vill_ward : $loc_e_vill_ward,
											  '#ajax'				=> array(
																				'event' 			=> 'change',
																				'effect' 			=> 'fade',			
																				'callback'			=> 'get_worksite_address',
																				'progress'			=> '',
																		  )
										);
						}
							
						}
						
					}
				}
			$value_worksite_villward_1 	= isset($form_state['values']['worksite_vill_ward']) ? $form_state['values']['worksite_vill_ward'] : (!empty($work_site_vill_ward)?$work_site_vill_ward :$loc_e_vill_ward) ;
				
				$form['renewla_profile']['worksite_ps'] = array(
						'#prefix' 				=> '<div id="worksite_ps_replace"><section class="col col-4"><label class="select">',
						'#suffix' 				=> '</label></section></div>',
						'#title'				=> 'G.Select Police Station',
						'#type' 				=> 'select',
						'#options' 				=> array(),
						'#required' 			=> TRUE,
						'#attributes' 			=>  $readonly_worksite_ps,
						'#ajax'					=> array(
															'event' 			=> 'change',
															'effect' 			=> 'fade',			
															'callback'			=> 'get_worksite_address',
															'progress'			=> '',
												  )
				  );
																									  
																									  
																									  
			if($value_worksite_dist_1 != "" && $value_worksite_subdivision_1 != ""){
					
					if($value_worksite_areatype_1 != ""){
						
						if($value_name_worksite_areatype_1 != ""){
							
							
							if($value_worksite_villward_1 != ""){
								
								  $form['renewla_profile']['worksite_ps'] 	= array(
									  '#prefix' 			=> '<div id="worksite_ps_replace"><section class="col col-4"><label class="select">',
									  '#suffix' 			=> '</label></section></div>',
									  '#title'				=> 'G.Select Police Station',
									  '#type' 				=> 'select',
									  '#default_value' 	    => !empty($worksite_ps) ? $worksite_ps : $l_e_ps,
									  '#options' 			=> get_ps_dashboard($value_worksite_dist_1),
									  '#required' 			=> TRUE,
									  '#attributes' 		=>  $readonly_worksite_ps,
								);
							
							}
						}
					}
				}
				$form['renewla_profile']['worksite_pin'] = array(			
					  '#prefix' 		  => '<section class="col col-4"><label class="input">',
					  '#suffix' 		  => '</label></section></div>',
					  '#title'			  => 'H.PIN Code ',
					  '#size'			  => t('20'),	
					  '#type' 			  => 'textfield',
					  '#attributes' 	  => $readonly_worksite_pin,												
					   '#default_value' 		=> !empty($worksite_pin) ? $worksite_pin:$loc_e_pin_number,
					  '#required'		  => TRUE,
					  '#element_validate' => array('validate_integer')												 
					);	
			
			
			
			if($value_worksite_subdivision_1 == ""){
					$form['renewla_profile']['office_locatiuon'] = array(
								'#prefix' 			=> '<div id="office_locatiuon_replace"><section><label class="input">',
								'#suffix' 			=> '</label></section></div></fieldset>',
													 
								'#type' 			=> 'markup',
													
													  
								'#markup' 		=>  '',
													);
				}if($value_worksite_subdivision_1 != ""){
					$get_sub_division_name		= 	db_select('sub_division', 'subdivision');
					$get_sub_division_name		->	fields('subdivision',array('sub_div_name'));
					$get_sub_division_name		->	condition('subdivision.sub_div_code',$value_worksite_subdivision_1);
					$sub_division_name 			= 	$get_sub_division_name->execute()->fetchObject()->sub_div_name;
					$form['renewla_profile']['office_locatiuon'] = array(
													  '#prefix' 			=> '<div id="office_locatiuon_replace"><section><label class="input">',
													  '#suffix' 			=> '</label></section></div></fieldset>',
													 
													   '#type' 			=> 'markup',
													
													  
													  '#markup' 		=> '<center><font color="#FF0000">N.B:-Your application will be forwarded to RLO <strong>'.$sub_division_name .'</font></strong></center>',
													);
				}
			
			
									 
	
		  
		  
		  
		  
		  if(($renewal_application_final_status=='F' && $renewal_appliaction_status=='B')||($renewal_application_final_status=='' && $renewal_appliaction_status=='')){
			   $form['renewla_profile']['submit'] = array (
					  '#prefix' 		=> '<footer>',  
					  '#suffix' 		=> '</footer></div></div>',
					  '#type' 			=> 'submit',
					  '#attributes' 	=> array('class' => array('button')),
					  '#value' 			=> 'Save & Continue'										  
				   );
		 }
		 
			$form['renewla_profile']['renewal_appliaction_status'] = array(		 
				  '#type'			=>	'hidden',
				  '#default_value'	=>	!empty($renewal_appliaction_status) ? encryption_decryption_fun('encrypt', $renewal_appliaction_status) : '',
				 
											
			);		
		
		return $form;
		}else{
			drupal_goto('applicant-login');
		}
	}else{
		$form['backlog_renewal']['header']	= array(
			'#type' 				=> 	'markup',
			'#markup' 				=> 	'<div class="content"><div class="panel-body">The Reference Number is only applicable for renewal for those Contractors who has offline license. This service is no longer exists.</div></div>'
			);
			return $form;
			die;
	}
}


function renewal_profile_details_submit($form, &$form_state){
	global $base_root, $base_path, $user;
		
	 $contractor_id 		= 	$user->uid;
	
	
	$val										=	$form_state['values'];
	$id											= 	$val['id'];
	$status_from 								= 	$val['status_from'];
	
	$contarctor_address_line					=	$val['contarctor_address_line'];
	
	//$category 									= 	$val['category'];
	
	$contarctor_name							=	$val['contarctor_name'];
	$contarctor_fathername 						= 	$val['contarctor_fathername'];
	
/*	$contarctor_dob								=	$val['contarctor_dob']['year']."-".$val['contarctor_dob']['month']."-".$val['contarctor_dob']['day'];

*/

	
$contarctor_dob								=	!empty($val['contarctor_dob1'])?date('Y-m-d',strtotime($val['contarctor_dob1'])): NULL;


$category									=	!empty($contarctor_dob)?1:0;
	$contarctor_age 							= 	!empty($val['contarctor_age'])?$val['contarctor_age'] :0;
	$applicant_id 								= 	$val['applicant_id'];
	$serial										=	$val['serial'];
	
	$contractor_dist 							= 	$val['contract_dist'];
	$contractor_subdivision 					= 	$val['contract_subdivision'];
	$contractor_areatype 						= 	$val['contract_areatype'];
	$name_contractor_areatype					=	$val['name_contractor_areatype'];
	$contractor_vill_ward						=	$val['contractor_vill_ward'];
	$contractor_ps								= 	$val['contractor_ps'];
	$contractor_pin								=	$val['contractor_pin'];
	
	
	$editable_name								=	$val['editable_name'];
	$editable_dist								=	$val['editable_dist'];
	$editable_category							=	$val['editable_category'];
	$editable_dob								=	$val['editable_dob'];
	
	
	
	$contract_dist_edit							=	$val['contract_dist_edit'];
	$contract_subdivision_edit					=	$val['contract_subdivision_edit'];
	$contract_areatype_edit						=	$val['contract_areatype_edit'];
	$name_contractor_areatype_edit				=	$val['name_contractor_areatype_edit'];
	$contractor_vill_ward_edit					=	$val['contractor_vill_ward_edit'];
	
	$contractor_ps_edit							=	$val['contractor_ps_edit'];
	
	$contarctor_name_edit						=	$val['contarctor_name_edit'];
	
	$father_edit								=	$val['father_edit'];
	$category_edit								=	$val['category_edit'];
	
	$dob_edit									=	$val['dob_edit'];
	$age_edit									=	$val['age_edit'];
	
	
	
	$worksite_address_line 						= 	$val['worksite_address_line'];
	$worksite_dist 								= 	$val['worksite_dist'];
	$work_site_areatype 						= 	$val['worksite_areatype'];
	
	$worksite_subdivision 						= 	$val['worksite_subdivision'];
	
	$name_work_site_areatype					=	$val['name_worksite_areatype'];
	$work_site_vill_ward						= 	$val['worksite_vill_ward'];
	$worksite_pin								=	$val['worksite_pin'];
	$worksite_ps								=	$val['worksite_ps'];

	$editable_cooparative						=	$val['editable_cooparative'];
	$is_coparative_data							=	$val['is_coparative_data'];
	$max_labour									=	encryption_decryption_fun('decrypt', $val['max_labour']);
	$amount_of_security_deposit					=	encryption_decryption_fun('decrypt', $val['max_deposit']);
	$renewal_appliaction_status					=   encryption_decryption_fun('decrypt', $val['renewal_appliaction_status']); 

	
	if(!empty($max_labour)){
	if($editable_cooparative == 1){
		$is_coparative 		= $is_coparative_data;
		if($is_coparative == 1)
			$renewal_security_new = $max_labour*25;
		else
		    $renewal_security_new = $max_labour*100; 
			
		
	}else{
		$is_coparative		=	$val['is_coparative'];
		if($is_coparative == 1)
			$renewal_security_new = $max_labour*25;
		else
		    $renewal_security_new = $max_labour*100; 
	}
	}
	if(!empty($amount_of_security_deposit)){
	if($amount_of_security_deposit>$renewal_security_new){
			 $payble_security_ammount 	= 0.00;
             $balance_security_ammount = $amount_of_security_deposit-$renewal_security_new;
		}if($amount_of_security_deposit<$renewal_security_new){
			 $payble_security_ammount 	= $renewal_security_new-$renewal_security_new;
             $balance_security_ammount = 0.00;
		}if($amount_of_security_deposit==$renewal_security_new){
			 $payble_security_ammount 	= 0.00;
             $balance_security_ammount = 0.00;
		}
	}
	
   if($editable_dist=='1'){
		$contractor_dist 						= 	$contract_dist_edit	;
		$contractor_subdivision 				= 	$contract_subdivision_edit;
		$contractor_areatype 					= 	$contract_areatype_edit;
		$name_contractor_areatype 				= 	$name_contractor_areatype_edit;
		$contractor_vill_ward 					= 	$contractor_vill_ward_edit;
		$contractor_ps							= 	$contractor_ps_edit	;
	}
	if($editable_name=='1'){
		$contarctor_name						=	$contarctor_name_edit;
	}
	if($editable_category=='1'){
		$category								=	$category_edit;
		$contarctor_fathername					=	$father_edit;
	}

	if($editable_dob=='1'){
		$contarctor_dob 						= 	$dob_edit;
		$contarctor_age 						= 	$age_edit;
	}
	
	
	
	$details_array								= 	array(
					'category_of_contractor'			=>	$category,
					'father_contarctor_name'			=>	$contarctor_fathername,
					'dob_contractor'					=>	$contarctor_dob, 
					'age_contractor'					=>	$contarctor_age	,
					'serial_no_from_v'					=>	$serial	,
					'contractor_user_id'				=>	$contractor_id,
					'act_id'							=> '1',
					'contractor_dist'					=> $contractor_dist,
					'contractor_subdivision'			=> $contractor_subdivision,
					'contractor_areatype'				=> $contractor_areatype,
					'contractor_name_areatype'			=> $name_contractor_areatype,
					'contractor_vill_ward'				=> $contractor_vill_ward,
					'contractor_ps'                     => $contractor_ps,
					'address_of_contractor'				=> $contarctor_address_line,
					'contractor_pin'					=> $contractor_pin,
					
					'worksite_address_line'				=> 	$worksite_address_line,
					'worksite_dist'						=> 	$worksite_dist,
					'work_site_areatype'				=> 	$work_site_areatype,
					'worksite_subdivision'				=> 	$worksite_subdivision,
					'name_work_site_areatype'			=> 	$name_work_site_areatype,
					'work_site_vill_ward'               => 	$work_site_vill_ward,
					'worksite_pin'						=>  $worksite_pin,
					'worksite_ps'						=>  $worksite_ps	,
					'is_backlog_renewal'				=> 	'1'	,
					'is_coparative'						=> 	$is_coparative	,
														
			);
													
													
	$query 	= 	db_select('l_particular_of_contract_labour', 'lpcl');
 	$query->fields('lpcl',array())->condition('serial_no_from_v',$serial,'=');
	$query->condition('contractor_user_id',$contractor_id,'=');
	$result 			= 	$query->execute()->fetchAll();
	
	
	if(empty($result)){
		db_insert('l_particular_of_contract_labour')->fields($details_array)->execute();
	}
	
	else{
		
		$details_array	= 	array(
									'category_of_contractor'			=>	$category,
									'father_contarctor_name'			=>	$contarctor_fathername,
									'dob_contractor'					=>	$contarctor_dob, 
									'age_contractor'					=>	$contarctor_age	,
									'contractor_dist'					=> 	$contractor_dist,
									'contractor_subdivision'			=> 	$contractor_subdivision,
									'contractor_areatype'				=> 	$contractor_areatype,
									'contractor_name_areatype'			=> 	$name_contractor_areatype,
									'contractor_vill_ward'				=> 	$contractor_vill_ward,
									'contractor_ps'                     => 	$contractor_ps,
									'address_of_contractor'				=>  $contarctor_address_line,
									'contractor_pin'					=>  $contractor_pin	,
									
									'worksite_address_line'				=> 	$worksite_address_line,
									'worksite_dist'						=> 	$worksite_dist,
									'work_site_areatype'				=> 	$work_site_areatype,
									'worksite_subdivision'				=> 	$worksite_subdivision,
									'name_work_site_areatype'			=> 	$name_work_site_areatype,
									'work_site_vill_ward'               => 	$work_site_vill_ward,
									'worksite_pin'						=>  $worksite_pin,
									'worksite_ps'						=>  $worksite_ps	,
									'is_backlog_renewal'				=> 	'1'	,
									'is_coparative'						=> 	$is_coparative	,
									
														
																									
								);
								
			//print_r($details_array);
		
		//die();												
																								
		$queryabc 		 =	db_update('l_particular_of_contract_labour');
		$queryabc		->	fields($details_array);
		$queryabc		->	condition('serial_no_from_v',$serial,'=');
		$queryabc		->	condition('contractor_user_id',$contractor_id,'=');
		$queryabc		->	execute();
	}
	
	$last_particulart_id 		= 	db_query('SELECT MAX(id) FROM l_particular_of_contract_labour where contractor_user_id ='.$contractor_id.'and serial_no_from_v ='.$serial)->fetchField();
	
	$array_license	=	array(
								
								'contractor_particular_id'	=>	$last_particulart_id,
								'is_backlog_license'		=>	1,
								'final_status'				=>	'F',
								'status'					=>	'I',
								'serial_no_from_v'			=>  $serial
													
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
		
		$renewal_id = db_query('SELECT id FROM l_contractor_license_apply_renweal where lincense_id ='.$license_id)->fetchField();	
	}
	$array_renewal	=	array(
								'lincense_id'				=>	$license_id,
								'is_backlog_renewal'		=>	1,
								'created_by'				=>	$contractor_id,
								'action'					=>	'R',
								'is_coparative'				=> 	$is_coparative	,
								'renewal_security_new'		=>  $renewal_security_new,
								'payble_security_fees'		=>	$payble_security_ammount,
								'balance_securit_amount'	=>	$balance_security_ammount
									
								
											
         					);
							
							
	if(empty($renewal_id)){
		db_insert('l_contractor_license_apply_renweal')->fields($array_renewal)->execute();
	}else{
		$query_upadate_renewal 		= 	db_update('l_contractor_license_apply_renweal');
 		$query_upadate_renewal		->	fields($array_renewal);
 		$query_upadate_renewal		->	condition('id',$renewal_id);
		$query_upadate_renewal		->	execute();
	}	

if($renewal_appliaction_status == 'B')
			contractor_activity_tag_insert_backlog_reneeal($serial,'R',$renewal_id,'B');
	else
		  contractor_activity_tag_insert_backlog_reneeal($serial,'R',$renewal_id,'');	
	
	
	
	$message = 'Primary Information has been saved Successfully.';
	drupal_set_message(t($message));
	
}



function get_contractor_address($form, $form_state){
	$commands 	= array();
	
	$commands[] = ajax_command_replace('#contractor_subdvsn_replace', drupal_render($form['renewla_profile']['contract_subdivision']));
	$commands[] = ajax_command_replace('#contractor_areatype_replace', drupal_render($form['renewla_profile']['contract_areatype']));
	$commands[] = ajax_command_replace('#name_contractor_areatype_replace', drupal_render($form['renewla_profile']['name_contractor_areatype']));
	$commands[] = ajax_command_replace('#contractor_vill_ward_replace', drupal_render($form['renewla_profile']['contractor_vill_ward']));
	
	$commands[] = ajax_command_replace('#contractor_ps_replace', drupal_render($form['renewla_profile']['contractor_ps']));
	
  	return array('#type' => 'ajax', '#commands' => $commands);
}

function get_category($form, $form_state){
	$commands 	= array();
	
	$commands[] = ajax_command_replace('#add_fathername', drupal_render($form['renewla_profile']['contarctor_fathername']));
	$commands[] = ajax_command_replace('#add_dob', drupal_render($form['renewla_profile']['contarctor_dob']));
	$commands[] = ajax_command_replace('#add_age', drupal_render($form['renewla_profile']['contarctor_age']));
	
	
  	return array('#type' => 'ajax', '#commands' => $commands);
}


function get_district_dashboard() {
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
	
	function get_subdivisionlist_dashboard($value_dist=''){

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
	
	function get_areatype_dashboard($value_loc_e_dist, $value_loc_e_subdivision){
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
		//print_r($all_areatype); //exit;
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
	
	function get_block_dashboard($value_subdv, $value_areatype){
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
	
	function get_villward_dashboard($value_villwared,$value_villwardtype){
		
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

function get_ps_dashboard($value_dist){
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

function get_corporation_dasboard($code){
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


function get_worksite_address($form, $form_state){
	$commands 	= array();
	
	$commands[] = ajax_command_replace('#worksite_subdvsn_replace_1', drupal_render($form['renewla_profile']['worksite_subdivision']));
	$commands[] = ajax_command_replace('#worksite_areatype_replace', drupal_render($form['renewla_profile']['worksite_areatype']));
	$commands[] = ajax_command_replace('#name_worksite_areatype_replace', drupal_render($form['renewla_profile']['name_worksite_areatype']));
	$commands[] = ajax_command_replace('#worksite_vill_ward_replace', drupal_render($form['renewla_profile']['worksite_vill_ward']));
	
	$commands[] = ajax_command_replace('#worksite_ps_replace', drupal_render($form['renewla_profile']['worksite_ps']));
		$commands[] = ajax_command_replace('#office_locatiuon_replace', drupal_render($form['renewla_profile']['office_locatiuon']));
	
  	return array('#type' => 'ajax', '#commands' => $commands);
}
