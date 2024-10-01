<?php

function amendment_form_step_two($form, &$form_state, $id=''){
	
	global $user;
	
	$application_id	=  encryption_decryption_fun('decrypt', $id);
	
	$clraDetails = db_select('l_clra_registration_master', 'lcrm');
	$clraDetails->fields('lcrm', array('loc_e_subdivision','name_areatype', 'registration_number', 'registration_date'));
	$clraDetails->condition('lcrm.id', $application_id);
	$clraDetails->condition('lcrm.act_id', '1');
	$clraDetailsResult = $clraDetails->execute()->fetchAssoc(); 
	
	$applicant_subdivision_code	= $clraDetailsResult['loc_e_subdivision'];
	$applicant_block_code		= $clraDetailsResult['name_areatype'];
	
	$get_amended_fields	=  db_select('l_clra_amendment', 'lca');
	$get_amended_fields	-> fields('lca', array());
	$get_amended_fields	-> condition('lca.application_id', $application_id);
	$get_amended_fields->condition('lca.status', 1);
	$get_amended_fields->orderBy('lca.id','DESC');
	$get_amended_fields->range(0, 1);
	$amended_fields_result  =   $get_amended_fields->execute()->fetchAssoc();
	
	$field_arr 	= explode(',', $amended_fields_result['amendment_fields']);//print_r($amended_fields_result);exit;
		
	foreach($field_arr as $field_arr_val){ 
		if($field_arr_val=='e_name') $readonly_e_name 	=  array('checked'=> 'checked'); 
		if($field_arr_val=='e_location') $readonly_e_location 	=  array('checked'=> 'checked');
		if($field_arr_val=='location') $readonly_location 	=  array('checked'=> 'checked'); 
		if($field_arr_val=='e_postal_address') $readonly_e_postal_address 	=  array('checked'=> 'checked'); 
		if($field_arr_val=='pe_details') $readonly_pe_details 	=  array('checked'=> 'checked');
		if($field_arr_val=='man_details') $readonly_man_details 	=  array('checked'=> 'checked');
		if($field_arr_val=='e_nature_of_work') $readonly_e_nature_of_work 	=  array('checked'=> 'checked');
		if($field_arr_val=='max_num_wrkmen') $readonly_max_num_wrkmen 	=  array('checked'=> 'checked');
		if($field_arr_val=='e_num_of_workmen_per_or_reg') $readonly_e_num_of_workmen_per_or_reg 	=  array('checked'=> 'checked');
		if($field_arr_val=='e_num_of_workmen_temp_or_reg') $readonly_e_num_of_workmen_temp_or_reg 	=  array('checked'=> 'checked');
		if($field_arr_val=='workmen_if_same_similar_kind_of_work') $readonly_workmen_if_same_similar_kind_of_work 	=  array('checked'=> 'checked');
		if($field_arr_val=='con_lab_job_desc') $readonly_con_lab_job_desc 	=  array('checked'=> 'checked');
		if($field_arr_val=='con_lab_wage_rate_other_benefits') $readonly_con_lab_wage_rate_other_benefits 	=  array('checked'=> 'checked');
		if($field_arr_val=='con_lab_cat_desig_nom') $readonly_con_lab_cat_desig_nom 	=  array('checked'=> 'checked');
		if($field_arr_val=='e_settlement_award_judgement_min_wage') $readonly_e_settlement_award_judgement_min_wage 	=  array('checked'=> 'checked');
		if($field_arr_val=='e_any_day_max_num_of_workmen') $readonly_e_any_day_max_num_of_workmen 	=  array('checked'=> 'checked');
		if($field_arr_val=='add_trade_union') $readonly_add_trade_union 	=  array('checked'=> 'checked');
		if($field_arr_val=='add_contractor') $readonly_add_contractor 	=  array('checked'=> 'checked');
	}
	
	
	$form['amend_form']['header'] 						= array(
															'#markup' 		=> t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">Fields For Amendment in Application of Registration under CLRA, Act</div><div class="panel-body min-height-0">'),
															'#type' 		=> 'markup'
															);
	
	$form['amend_form']['application_id'] 				= array(
															'#type' 		=> 'hidden',
															'#default_value'=> encryption_decryption_fun('encrypt', $application_id),
															'#attributes'	=> array('readonly' => 'readonly'), 								
															);
																						
	$form['amend_form']['applicant_subdivision_code'] 	= array(
															'#type' 		=> 'hidden',
															'#default_value'=> encryption_decryption_fun('encrypt', $applicant_subdivision_code),	
															'#attributes'	=> array('readonly' => 'readonly'),							
															);
																						
	$form['amend_form']['applicant_block_code'] 		= array(
															'#type' 		=> 'hidden',
															'#default_value'=> encryption_decryption_fun('encrypt', $applicant_block_code),
															'#attributes'	=> array('readonly' => 'readonly'),								
															);
	
		 
	$form['amend_form']['note'] 						= array(
															'#prefix' 		=> '<section><label>',
															'#suffix' 		=> '</label></section>',
															'#markup' 		=> t('Note : Tick the fields which is to be amended by the Principal Employer for Registration Number : 																	'.$get_app_location_details_result['registration_number'].'&nbsp;issued on Date: '.date('dS M, Y', strtotime($get_app_location_details_result['registration_date']))),				
															'#type' 		=> 'markup',														
															);
																						
	$form['amend_form']['e_name'] 						= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('1. Name of the Establishment'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> $description,
															'#attributes'	=> $readonly_e_name,								
															);
	
	$form['amend_form']['e_location'] 					= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('1a. Location of the Establishment<b> ( District, Subdivision, Block/Municipality/Corporation/SEZ/Notified Area )</span> NEED ALC&#39;s APPROVAL </b> '),				
															'#type' 		=> 'checkbox',
															'#description' 	=> $description,
															'#attributes'	=> $readonly_e_location,								
															);
															
	$form['amend_form']['location'] 					= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('1b. Location of the Establishment<b> ( Address Line , Gram Panchayat / Ward / Sector / Notified Area, Police Station, PIN Code )</b>'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> $description,
															'#attributes'	=> $readonly_location,								
															);
															
	$form['amend_form']['e_postal_address'] 			= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('2. Postal Address of the Establishment'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> $description,
															'#attributes'	=> $readonly_e_postal_address,								
															);
															
										
	$form['amend_form']['pe_details'] 					= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('3. Name and Address of Principal Employer'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> $description,
															'#attributes'	=> $readonly_pe_details,								
															);
													
	$form['amend_form']['man_details'] 					= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('4. Name and Address of Manager or person responsible'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> $description,	
															'#attributes'	=> $readonly_man_details,							
															);

	$form['amend_form']['e_nature_of_work'] 			= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('5. Nature of Work Carried on in the Establishment'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> $description,
															'#attributes'	=> $readonly_e_nature_of_work,	
																							
															);
													
	$form['amend_form']['max_num_wrkmen'] 				= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('5.a) Maximum Number of Workmen Employed Directly on any day in the Establishment'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> $description,	
															'#attributes'	=> $readonly_max_num_wrkmen,								
															);

	$form['amend_form']['e_num_of_workmen_per_or_reg'] 	= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('5.b) Number of Workmen Engaged as Permanent/Regular Workmen'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> $description,	
															'#attributes'	=> $readonly_e_num_of_workmen_per_or_reg,								
															);

	$form['amend_form']['e_num_of_workmen_temp_or_reg'] = array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('5.c) Number of Workmen Engaged as Temporary/Casual Workmen'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> $description,	
															'#attributes'	=> $readonly_e_num_of_workmen_temp_or_reg,								
															);

	$form['amend_form']['workmen_if_same_similar_kind_of_work'] = array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('5.d) Whether the Workmen employed/intended to be Employment by the Contractor Perform the same or similar kind of work as the Workmen employed directly by the Principal Employer (if yes, <br/>please give here information as detailed below'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> $description,
															'#attributes'	=> $readonly_workmen_if_same_similar_kind_of_work,		
																							
															);

	$form['amend_form']['con_lab_job_desc']		 		= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('5d.i) A complete job description of the contractor labour'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> $description,	
															'#attributes'	=> $readonly_con_lab_job_desc,								
															);

	$form['amend_form']['con_lab_wage_rate_other_benefits'] = array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('5d.ii) Wage rates and other cash benefits paid/to be paid'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> $description,
															'#attributes'	=> $readonly_con_lab_wage_rate_other_benefits,								
															);

	$form['amend_form']['con_lab_cat_desig_nom'] 		= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('5d.iii) Category/designation/nomenclature of the job'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> $description,	
															'#attributes'	=> $readonly_con_lab_cat_desig_nom,							
															);

	$form['amend_form']['e_settlement_award_judgement_min_wage'] = array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('5d.v) Settlement or award or judgement or minimum wages (if any applicable in the establishment'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> $description,
															'#attributes'	=> $readonly_e_settlement_award_judgement_min_wage,								
															);

	$form['amend_form']['e_any_day_max_num_of_workmen'] = array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('Maximum number of contract labour to be employed on any day through each contractor'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> $description_payable,	
															'#attributes'	=> $readonly_e_any_day_max_num_of_workmen,								
															);
						
	$form['amend_form']['add_trade_union'] 				= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
															'#suffix' 		=> '</label></section>',
															'#title' 		=> t('Add/Modify Trade Union Information'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> $description,
															'#attributes'	=> $readonly_add_trade_union,										
															);
						
	$form['amend_form']['add_contractor'] 				= array(
															'#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
															'#suffix' 		=> '</label></section></div></div>',
															'#title' 		=> t('Add/Modify Contractor Information.'),				
															'#type' 		=> 'checkbox',
															'#description' 	=> $description_payable,
															'#attributes'	=> $readonly_add_contractor,											
															);
						
	$form['amend_form']['submit'] 						= array (
															'#prefix'		=> '<div class="col-md-12"><div class="row">',
															'#suffix'		=> '</div></div></div>', 
															'#type' 		=> 'submit',
															'#value' 		=> 'Apply',
															'#attributes'	=> array('id' => 'submit_oneId' , 'class' => array('btn btn-primary pull-right form-submit'))
														);
	return $form;
}

function amendment_form_step_two_submit($form, &$form_state){
	
	global $user;
	
	$user_id					= $user->uid;
	$val 						= $form_state['values'];
	$application_id				= encryption_decryption_fun('decrypt', $val['application_id']);	
	$applicant_subdivision_code	= encryption_decryption_fun('decrypt', $val['applicant_subdivision_code']);
	$applicant_block_code		= encryption_decryption_fun('decrypt', $val['applicant_block_code']);
	
	$all_fields	= array('e_name','e_location','location','e_postal_address','pe_details','man_details','e_nature_of_work','max_num_wrkmen','e_num_of_workmen_per_or_reg','e_num_of_workmen_temp_or_reg','workmen_if_same_similar_kind_of_work','con_lab_job_desc','con_lab_wage_rate_other_benefits','con_lab_cat_desig_nom','e_settlement_award_judgement_min_wage','e_any_day_max_num_of_workmen','add_trade_union','add_contractor');
	
	$field_name 				= array_keys($val,1);
	$amendment_fields 			= implode(",",$field_name);
	$amended_fields_arr 		= explode(",",$amendment_fields);
	$not_amended_field          = '';
	
	
	foreach($all_fields as $get_notamended_fields){
		switch($get_notamended_fields){
			
			case "e_name":
				  if (!in_array("e_name", $amended_fields_arr))
				  $value .= 'e_name,';
				  break;
				  
			case "e_location":
				  if (!in_array("e_location", $amended_fields_arr))
				  $value .= 'e_location,';
				  break;
				  
			case "location":
				  if (!in_array("location", $amended_fields_arr))
				  $value .= 'location,';
				  break;
				  
			case "e_postal_address":
				  if (!in_array("e_postal_address", $amended_fields_arr))
				  $value .= 'e_postal_address,';
				  break;
			  
			case "pe_details":
				  if (!in_array("pe_details", $amended_fields_arr))
				  $value .= 'pe_details,';
				  break;
				  
			case "man_details":
				  if (!in_array("man_details", $amended_fields_arr))
				  $value .= 'man_details,';
				  break;
				  
			case "e_nature_of_work":
				  if (!in_array("e_nature_of_work", $amended_fields_arr))
				  $value .= 'e_nature_of_work,';
				  break;
				  
			case "max_num_wrkmen":
				  if (!in_array("max_num_wrkmen", $amended_fields_arr))
				  $value .= 'max_num_wrkmen,';
				  break;
				  
			case "e_num_of_workmen_per_or_reg":
				  if (!in_array("e_num_of_workmen_per_or_reg", $amended_fields_arr))
				  $value .= 'e_num_of_workmen_per_or_reg,';
				  break;
				  
			case "e_num_of_workmen_temp_or_reg":
				  if (!in_array("e_num_of_workmen_temp_or_reg", $amended_fields_arr))
				  $value .= 'e_num_of_workmen_temp_or_reg,';
				  break;
				  
			case "workmen_if_same_similar_kind_of_work":
				  if (!in_array("workmen_if_same_similar_kind_of_work", $amended_fields_arr))
				  $value .= 'workmen_if_same_similar_kind_of_work,';
				  break;
				  
			case "con_lab_job_desc":
				  if (!in_array("con_lab_job_desc", $amended_fields_arr))
				  $value .= 'con_lab_job_desc,';
				  break;
				  
			case "con_lab_wage_rate_other_benefits":
				  if (!in_array("con_lab_wage_rate_other_benefits", $amended_fields_arr))
				  $value .= 'con_lab_wage_rate_other_benefits,';
				  break;
				  
			case "con_lab_cat_desig_nom":
				  if (!in_array("con_lab_cat_desig_nom", $amended_fields_arr))
				  $value .= 'con_lab_cat_desig_nom,';
				  break;
				  
			case "e_settlement_award_judgement_min_wage":
				  if (!in_array("e_settlement_award_judgement_min_wage", $amended_fields_arr))
				  $value .= 'e_settlement_award_judgement_min_wage,';
				  break;
				  
			case "e_any_day_max_num_of_workmen":
				  if (!in_array("e_any_day_max_num_of_workmen", $amended_fields_arr))
				  $value .= 'e_any_day_max_num_of_workmen,';
				  break;
				  
			case "add_trade_union":
				  if (!in_array("add_trade_union", $amended_fields_arr))
				  $value .= 'add_trade_union,';
				  break;
				  
			case "add_contractor":
				  if (!in_array("add_contractor", $amended_fields_arr))
				  $value .= 'add_contractor,';
				  break;
				  
			default:
				 $value .= '';
				  
		}
	}
	
	$amendedAddition = explode(",",$amendment_fields);
		 
	if(in_array("e_location", $amendedAddition)) {
		$findVariable = array("e_location", "location");
		$madeAmendedFields = str_replace($findVariable, "", $amendment_fields);
		$addedAmended_fields = 'e_location,location,'.$madeAmendedFields;
		$finalAmendedFields = str_replace(',,', ",", $addedAmended_fields);
		
		$findVariable = array("e_location", "location");
		$madeNotAmendedFields = str_replace($findVariable, "", $value);
		$finalNotAmendedFields = str_replace(',,', ",", $madeNotAmendedFields);
	}else{
		
		$finalAmendedFields = str_replace(',,', ",", $amendment_fields);
		$finalNotAmendedFields = str_replace(',,', ",", $value);
	}
	
	if(count($field_name) == 0){
			
		 drupal_set_message('Please select minimum one point.', 'error');
		   
   	}else{
			
		$exitrow = db_query("select id, status, amendment_parent_id, loc_e_dist,loc_e_subdivision,loc_e_areatype,name_areatype FROM l_clra_registration_master WHERE user_id = '".$user->uid."' order by id desc limit 1")->fetchObject();	
		
		$get_amended_fields	= db_select('l_clra_amendment', 'lca');
		$get_amended_fields	->fields('lca', array());
		$get_amended_fields	->condition('lca.application_id', $application_id);
		$get_amended_fields->condition('lca.status', 1);
		$get_amended_fields->orderBy('lca.id','DESC');
		$get_amended_fields->range(0, 1);
		$amended_fields_result  = $get_amended_fields->execute()->fetchAssoc();
		
		if(!empty($amended_fields_result) ){ 
		
			if($exitrow->status == 'B'){
				$remark = db_select('l_remarks_master', 'lrm');
				$remark->fields('lrm', array('remark_field_text','remark_id'));
				$remark->condition('lrm.application_id', $exitrow->id);
				$remark->orderBy('is_active', 1);
				$remark->orderBy('remark_id', 'DESC');
				$remarkData = $remark->execute()->fetchAssoc();
				$remark_arr = explode(',', $remarkData['remark_field_text']);
				
				$getAmended = explode(",",$finalAmendedFields);
				foreach($getAmended as $amendData){
					if(in_array($amendData ,$remark_arr)){
						$madeRemarkTextValue = str_replace($amendData, "", $remarkData['remark_field_text']);
						$finalRemarkTextValue = str_replace(',,', ",", $madeRemarkTextValue);
					}
				}
				db_update('l_remarks_master')->fields(array('remark_field_text' => $finalRemarkTextValue))->condition('application_id',$exitrow->id)->condition('remark_id',$remarkData['remark_id'])->execute();
			}
			
			$updatedArray 	= array(
								'amendment_fields'				=>  $finalAmendedFields,
								'applicant_subdivision_code' 	=>  $applicant_subdivision_code,
								'applicant_block_code' 			=>  $applicant_block_code,
								'not_amended_fields' 			=>  $finalNotAmendedFields,
								'amend_apply_date' 				=>  date("Y-m-d"),
								'status' 						=> 	1,
							);
			
			$queryForward = db_update('l_clra_amendment');
			$queryForward->fields($updatedArray);
			$queryForward->condition('id',$amended_fields_result['id']);
			$queryForward->condition('status', 1);
			$queryForward->condition('applicant_uid',$user_id);
			$queryForward->condition('application_id',$application_id);
			$queryForward->execute();
				
			drupal_set_message('You have chosen your fields for amendment successfully.');	
			if($exitrow->id !='' && $exitrow->amendment_parent_id == $application_id ){	
				drupal_goto('clra-reg-amendment/view-clra-application/'.encryption_decryption_fun('encrypt', $exitrow->id));
			}else{
				drupal_goto('clra-amendment-submit');
			}
				
		}else{ 
	
			$amendment_array	=	array(								
									'application_id' 				=> $application_id,
									'status' 						=> 1,
									'amend_apply_date' 				=> date("Y-m-d"),
									'amendment_fields' 				=> $finalAmendedFields,	
									'applicant_subdivision_code' 	=> $applicant_subdivision_code,
									'applicant_block_code' 			=> $applicant_block_code,
									'not_amended_fields'			=> $finalNotAmendedFields,
									'applicant_uid'					=> $user_id,
								);	
			db_insert('l_clra_amendment')->fields($amendment_array)->execute();
			drupal_set_message('You have chosen your fields for amendment successfully.');	
			drupal_goto('clra-amendment-submit');
		}
		
   }
}
?>