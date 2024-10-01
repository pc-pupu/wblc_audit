<?php

drupal_add_js(drupal_get_path('module', 'applicant_forms') . '/js/upload_custom.js');
 
function amendment_form_clra_registration($form, &$form_state,$id=''){ 

	global $base_root, $base_path, $user;

	$user_id					= $user->uid;
	$amend_application_id		= encryption_decryption_fun('decrypt', $id);
	
	$fetch_reg_det				= db_query("select * from l_clra_registration_master where user_id=:user_id and id=:application_id", array(':user_id' => $user_id ,':application_id' => $amend_application_id));
	
	
	if($fetch_reg_det->rowCount() > 0 ){
	
		$obj2 									= $fetch_reg_det->fetchAssoc(); 
		
		$identification_number					=	$obj2['identification_number'];
		$act_id									=	$obj2['act_id'];
		
		$e_name									=	$obj2['e_name'];
		$est_type 								= 	$obj2['est_type'];
		$loc_e_name								=	$obj2['loc_e_name'];
		$loc_e_dist								=	$obj2['loc_e_dist'];
		$loc_e_subdivision						=	$obj2['loc_e_subdivision'];
		$loc_e_areatype							=	$obj2['loc_e_areatype'];
		$loc_e_areatype_code					=	$obj2['name_areatype'];
		$loc_e_vill_ward						=	$obj2['loc_e_vill_ward'];
		$loc_e_ps								=	$obj2['l_e_ps'];
		$loc_e_pin_number						= 	$obj2['loc_e_pin_number'];
		
		$e_postal_address						=	$obj2['e_postal_address'];
		$e_postal_dist							=   $obj2['e_postal_dist'];
		$e_postal_subdivision					=   $obj2['e_postal_subdivision'];
		$e_postal_areatype						=   $obj2['e_postal_areatype'];	
		$e_postal_name_areatype					=   $obj2['e_postal_name_areatype'];	
		$e_postal_vill_ward 					=   $obj2['e_postal_vill_ward'];
		$e_postal_ps							=   $obj2['e_postal_ps'];
		$e_postal_pin_number					= 	$obj2['e_postal_pin_number'];	
				
		$full_name_principal_emp				=	$obj2['full_name_principal_emp'];
		$gender_pe								=	$obj2['gender_pe'];
		$loc_emp_country						=	$obj2['loc_emp_country'];
		$loc_emp_state							=	$obj2['loc_emp_state'];
		$address_principal_emp					=	$obj2['address_principal_emp'];
		$loc_emp_dist							=	$obj2['loc_emp_dist'];
		$loc_emp_subdivision					=	$obj2['loc_emp_subdivision'];
		$loc_emp_areatype 						=	$obj2['loc_emp_areatype'];
		$emp_name_areatype						=	$obj2['emp_name_areatype'];
		$loc_emp_vill_ward						=	$obj2['loc_emp_vill_ward'];
		$l_emp_ps								=	$obj2['l_emp_ps'];
		$loc_emp_pin_number						=   $obj2['loc_emp_pin_number'];
		
		$full_name_manager						=	$obj2['full_name_manager'];
		$gender_pe								=	$obj2['gender_pe'];
		$manager_address						=	$obj2['address_manager'];
		$loc_manager_country					=	$obj2['loc_manager_country'];
		$loc_manager_state						=	$obj2['loc_manager_state'];
		$loc_manager_dist						=	$obj2['loc_manager_dist'];
		$loc_manager_subdivision				=	$obj2['loc_manager_subdivision'];
		$loc_manager_areatype					=	$obj2['loc_manager_areatype'];
		$manager_name_areatype					=	$obj2['manager_name_areatype'];
		$loc_manager_vill_ward					=	$obj2['loc_manager_vill_ward'];
		$l_manager_ps							=	$obj2['l_manager_ps'];
		$loc_manager_pin_number					= 	$obj2['loc_manager_pin_number'];
		
		$max_num_wrkmen							=	$obj2['max_num_wrkmen'];
		$e_num_of_workmen_per_or_reg			=	$obj2['e_num_of_workmen_per_or_reg'];
		$e_num_of_workmen_temp_or_reg			=	$obj2['e_num_of_workmen_temp_or_reg'];
		$workmen_if_same_similar_kind_of_work	=	$obj2['workmen_if_same_similar_kind_of_work'];
		$con_lab_job_desc						=	$obj2['con_lab_job_desc'];
		$con_lab_wage_rate_other_benefits		=	$obj2['con_lab_wage_rate_other_benefits'];
		$con_lab_cat_desig_nom					=	$obj2['con_lab_cat_desig_nom'];
		$e_settlement_award_judgement_min_wage	=	$obj2['e_settlement_award_judgement_min_wage'];
		$e_any_day_max_num_of_workmen			=	$obj2['e_any_day_max_num_of_workmen'];
		
		$final_submit_status					=	$obj2['final_submit_status'];
		$status									=	$obj2['status'];
		
		$amendment_parent_id					=	$obj2['amendment_parent_id'];
		
	}	

	/**** DOC UPLOADED CODE STARTS*****/
		
	$fetch_reg_upload_files		=	db_query("select * from l_documents_upload where application_id=:application_id and user_id=:user_id",array(':user_id' => $user_id,':application_id' => $amend_application_id ));
	
	if( $fetch_reg_upload_files->rowCount() > 0 ){
		foreach($fetch_reg_upload_files as $obj3){ 
			$trade_license_file						=	$obj3->trade_license_file;
			$article_of_assoc_file					=	$obj3->article_of_assoc_file;
			$memorandum_of_cert_file				=	$obj3->memorandum_of_cert_file;
			$partnership_deed_file					=	$obj3->partnership_deed_file;
			$factory_license_file					=	$obj3->factory_license_file;
		}
	}

	/**** NATURE OF WORK CODE STARTS*****/

	$query_get_each_nature_work  = 	db_query("select a.id,b.contractor_work_id from l_clra_reg_nature_of_work a left join contractor_works as b on a.e_nature_of_work = b.contractor_work_id where a.application_id 		=:application_id and a.user_id=:user_id and b.is_default =:is_default order by a.id desc", array( ':user_id' => $user_id , ':application_id' => $amend_application_id , ':is_default' => '1')); 


	$resultChunk	=	 array();		
	if(!empty($query_get_each_nature_work)){
		foreach($query_get_each_nature_work as $listData){ 
			$resultChunk[] = $listData->contractor_work_id;
		}
	}
	/** FOR FIELD FOR OTHER OPTION OF NATURE WORK ****/
	
	$get_other_nature_work 		= db_select('l_clra_reg_nature_of_work', 'lcnw');
	$get_other_nature_work		->leftJoin('contractor_works', 'cw', 'lcnw.e_nature_of_work=cw.contractor_work_id');
	$get_other_nature_work		->fields('cw',array('cont_work_name','contractor_work_id'));
	$get_other_nature_work		->condition('cw.is_default',0);
	$get_other_nature_work		->condition('lcnw.user_id',$user_id);
	$get_other_nature_work		->condition('lcnw.application_id',$amend_application_id);
	$other_nature_work_data 	= $get_other_nature_work->execute(); 
	
	if( $other_nature_work_data->rowCount() > 0 ){
	
		$obj3 = $other_nature_work_data->fetchAssoc();  
		
		$other_nature_wrk_id 	= $obj3['contractor_work_id'];
		$other_nature_wrk 		= $obj3['cont_work_name'];
	}

	/**** NATURE OF WORK CODE ENDS *****/	


	if(!empty($amend_application_id) && ($status == 'B' || $status == '' )){
		
		if ( $trade_license_file != "" ) {
			$tlf = "checked";
		}else{
			$tlf = "";
		}
		if ( $article_of_assoc_file != "" ) {
			$asf = "checked";
		}else{
			$asf = "";
		}
		if ( $memorandum_of_cert_file != "" ) {
			$mcf = "checked";
		}else{
			$mcf = "";
		}
		if ( $partnership_deed_file != "" ) {
			$pdf = "checked";
		}else{
			$pdf = "";
		}
		if ( $factory_license_file != "" ) {
			$flf = "checked";
		}else{
			$flf = "";
		}
		$readonly_trade_license 		= array('id' => 'upload_check_in_1_id', 'onclick' => 'upload_check_fun_1()', $tlf => $tlf); 
		$readonly_article_of_assoc 		= array('id' => 'upload_check_in_2_id', 'onclick' => 'upload_check_fun_2()', $asf => $asf); 
		$readonly_memorandum_of_cert 	= array('id' => 'upload_check_in_3_id', 'onclick' => 'upload_check_fun_3()', $mcf => $mcf); 
		$readonly_partnership_deed 		= array('id' => 'upload_check_in_4_id', 'onclick' => 'upload_check_fun_4()', $pdf => $pdf); 
		$readonly_factory_license 		= array('id' => 'upload_check_in_5_id', 'onclick' => 'upload_check_fun_5()', $flf => $flf); 
		
		$readonly_e_name = $readonly_e_location = $readonly_addressLine = $readonly_location = $readonly_e_postal_address = $readonly_e_postal_dist = $readonly_e_postal_subdivision = $readonly_e_postal_areatype = $readonly_e_postal_areatype_code = $readonly_e_postal_vill_ward =$readonly_e_postal_ps = $readonly_e_postal_ps = $readonly_full_name_principal_emp= $readonly_address_principal_emp = $readonly_address_principal_emp_Textarea = $readonly_emp_country = $readonly_emp_state = $readonly_emp_dist = $readonly_loc_emp_subdv = $readonly_loc_emp_areatype= $readonly_loc_emp_areatype_code = $readonly_loc_emp_vill_ward= $readonly_loc_emp_ps = $readonly_full_name_manager = $readonly_address_manager = $readonly_address_manager_Textarea = $readonly_manager_country = $readonly_manager_state = $readonly_manager_dist = $readonly_loc_manager_subdv = $readonly_loc_manager_areatype = $readonly_loc_manager_areatype_code = $readonly_loc_manager_vill_ward = $readonly_loc_manager_ps =   $readonly_other_nature_of_work_value = $readonly_max_num_wrkmen = $readonly_e_num_of_workmen_per_or_reg = $readonly_e_num_of_workmen_temp_or_reg =  $readonly_workmen_if_same_similar_kind_of_work = $readonly_con_lab_job_desc = $readonly_con_lab_wage_rate_other_benefits = $readonly_con_lab_cat_desig_nom = $readonly_e_settlement_award_judgement_min_wage = $readonly_e_any_day_max_num_of_workmen = $readonly_estType = $readonly_gender_pe=  array('class' => array('correction_view'));
		$readonly_e_loc_subdv 	   = array('onkeyup' => 'getSpanText()', 'onblur' => 'getSpanText()', 'onchange' => 'getSpanText()', 'class' => array('correction_view'));
		$readonly_e_nature_of_work = array('readonly' => 'readonly','style' =>'height: 150px;', 'class' => array('correction_view'),'onchange' => 'nature_wrk_others(this.value)');
		$readonly_loc_e_pin_number = $readonly_e_postal_pin_number = $readonly_loc_emp_pin_number = $readonly_loc_manager_pin_number = array('maxlength' => 6,  'class' => array('correction_view'));
		
		/*** GET AMEND FIELDS FROM CLRA_AMENDMENT TABLE ****/
		
		$query 						= db_select('l_clra_registration_master', 'lcrm')
									->fields('lcrm', array('id'))
									->condition('lcrm.user_id', $user_id)
									->condition('lcrm.status', 'I')
									->orderBy('id', 'DESC')
									->range(0, 1);
		$get_last_master_application  = $query->execute()->fetchAssoc();
		
		
		$amended_fields				= db_select('l_clra_amendment', 'lca')
									->fields('lca', array('not_amended_fields'))
									->condition('lca.application_id', $get_last_master_application['id'])
									->condition('lca.status', 1)
									->orderBy('id', 'DESC')
									->range(0, 1);
		$notAmendedField 			= $amended_fields->execute()->fetchAssoc();
		$field_arr 					= explode(',', $notAmendedField['not_amended_fields']);
		
		
		
		foreach($field_arr as $field_arr_val){ 
			switch(true){	
			
				case ($field_arr_val == "e_name"):
							$readonly_e_name 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_estType 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
							
				case ($field_arr_val ==  "e_location"):	
							$readonly_e_location 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_loc_subdv 	   			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
							
				case ($field_arr_val ==  "location"):
							$readonly_addressLine 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_location 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_loc_e_pin_number 			= array('disabled' => 'disabled', 'maxlength' => 6,  'class' => array('readonly_view'));
							break;
				case ($field_arr_val == "e_postal_address"):
							$readonly_e_postal_address 			= array('readonly' => 'readonly', 'class' => array('readonly_view'));
							$readonly_e_postal_dist 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_postal_subdivision 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));			
							$readonly_e_postal_areatype 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_postal_areatype_code 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_postal_vill_ward 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_postal_ps 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_postal_pin_number		= array('disabled' => 'disabled', 'maxlength' => 6,  'class' => array('readonly_view'));
							break;
							
				case ($field_arr_val == "pe_details"):
							$readonly_full_name_principal_emp 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_gender_pe 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_address_principal_emp	 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_address_principal_emp_Textarea = array('readonly' => 'readonly', 'class' => array('readonly_view'));
							$readonly_emp_country 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_emp_state 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_emp_dist 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_loc_emp_subdv 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_loc_emp_areatype 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_loc_emp_areatype_code		= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_loc_emp_vill_ward 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_loc_emp_ps 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_loc_emp_pin_number		= array('disabled' => 'disabled', 'maxlength' => 6,  'class' => array('readonly_view'));
							break;
							
				case ($field_arr_val == "man_details"):
							$readonly_full_name_manager 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_address_manager 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_address_manager_Textarea  = array('readonly' => 'readonly', 'class' => array('readonly_view'));	
							$readonly_manager_country 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_manager_state 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_manager_dist 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_loc_manager_subdv 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_loc_manager_areatype 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_loc_manager_areatype_code	= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_loc_manager_vill_ward 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_loc_manager_ps 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							$readonly_loc_manager_pin_number 	= array('disabled' => 'disabled', 'maxlength' => 6,  'class' => array('readonly_view'));
							break;
							
				case ($field_arr_val == "e_nature_of_work"):
							$readonly_other_nature_of_work_value				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_e_nature_of_work							= array('readonly' => 'readonly','style'=>"height:150px;", 'onchange' => 'nature_wrk_others(this.value)','class' => array('readonly_view'));			
							break;
							
				case ($field_arr_val == "max_num_wrkmen"):
							$readonly_max_num_wrkmen							= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
							
				case ($field_arr_val == "e_num_of_workmen_per_or_reg"):
							$readonly_e_num_of_workmen_per_or_reg				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
							
				case ($field_arr_val == "e_num_of_workmen_temp_or_reg"):
							$readonly_e_num_of_workmen_temp_or_reg				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							break;
							
				case ($field_arr_val == "workmen_if_same_similar_kind_of_work"):
							$readonly_workmen_if_same_similar_kind_of_work 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
							break;
							
				case ($field_arr_val == "con_lab_job_desc"):
							$readonly_con_lab_job_desc							= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
							
				case ($field_arr_val == "con_lab_wage_rate_other_benefits"):
							$readonly_con_lab_wage_rate_other_benefits 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;	
									
				case ($field_arr_val == "con_lab_cat_desig_nom"):
							$readonly_con_lab_cat_desig_nom						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;	
									
				case ($field_arr_val == "e_settlement_award_judgement_min_wage"):
							$readonly_e_settlement_award_judgement_min_wage 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;	
									
				case ($field_arr_val == "e_any_day_max_num_of_workmen"):
							$readonly_e_any_day_max_num_of_workmen 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
							
				case ($field_arr_val == "trade_license_file"):
							$readonly_trade_license 							= array('class' => array('readonly_view'),'id' => 'upload_check_in_1_id', 'onclick' => 'upload_check_fun_1()', $tlf => $tlf);
							break;
							
				case ($field_arr_val == "article_of_assoc_file"):
							$readonly_article_of_assoc 							= array('class' => array('readonly_view'), 'id' => 'upload_check_in_2_id', 'onclick' => 'upload_check_fun_2()', $asf => $asf);
							break;
							
				case ($field_arr_val == "memorandum_of_cert_file"):
							$readonly_memorandum_of_cert 						= array('class' => array('readonly_view'),'id' => 'upload_check_in_3_id', 'onclick' => 'upload_check_fun_3()', $mcf => $mcf);
							break;
							
				case ($field_arr_val == "partnership_deed_file"):
							$readonly_partnership_deed 							= array('class' => array('readonly_view'),'id' => 'upload_check_in_4_id', 'onclick' => 'upload_check_fun_4()', $pdf => $pdf);
							break;
							
							
				case ($field_arr_val == "factory_license_file"):
							$readonly_factory_license 							= array('class' => array('readonly_view'),'id' => 'upload_check_in_5_id', 'onclick' => 'upload_check_fun_5()', $flf => $flf );
							break;
							
				default:
			}
		}
		if($status == 'B'){
			
			$readonly_trade_license 	 = array('class' => array('correction_view'),'id' => 'upload_check_in_1_id', 'onclick' => 'upload_check_fun_1()', $tlf => $tlf);
			$readonly_article_of_assoc 	 = array('class' => array('correction_view'), 'id' => 'upload_check_in_2_id', 'onclick' => 'upload_check_fun_2()', $asf => $asf);
			$readonly_memorandum_of_cert = array('class' => array('correction_view'),'id' => 'upload_check_in_3_id', 'onclick' => 'upload_check_fun_3()', $mcf => $mcf);
			$readonly_partnership_deed 	 = array('class' => array('correction_view'),'id' => 'upload_check_in_4_id', 'onclick' => 'upload_check_fun_4()', $pdf => $pdf);
			$readonly_factory_license 	 = array('class' => array('correction_view'),'id' => 'upload_check_in_5_id', 'onclick' => 'upload_check_fun_5()', $flf => $flf );
			
			/*** Get REMARK Details ****/
		
			$query_remark 		  		= db_select('l_remarks_master', 'lrm');
			$query_remark		 		->	fields('lrm', array('remark_field_text'));
			$query_remark		 		->	condition('lrm.application_id', $amend_application_id);
			$query_remark		 		->	orderBy('remark_id', 'DESC');
			$remark_field_text_array 	= $query_remark->execute()->fetchAssoc();
			$remark_arr 				= explode(',', $remark_field_text_array['remark_field_text']);
			
			foreach($remark_arr as $field_arr_val1){				
				
				switch($field_arr_val1){	
				
				case "e_name":
						$readonly_e_name 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						break;
						
				case "est_type":
						$readonly_estType 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						break;
							
				case  "e_location":	
						$readonly_e_location 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_e_loc_subdv					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						break;
						
				case  "location":
						
						$readonly_location 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_loc_e_pin_number 				= array('disabled' => 'disabled', 'maxlength' => 6, 'class' => array('readonly_view'));
						break;
				
				case "e_postal_address":
						$readonly_e_postal_address 				= array('readonly' => 'readonly', 'class' => array('readonly_view'));
						$readonly_e_postal_dist 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_e_postal_subdivision 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));				
						$readonly_e_postal_areatype 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_e_postal_areatype_code 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_e_postal_vill_ward 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_e_postal_ps 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_e_postal_pin_number			= array('disabled' => 'disabled', 'maxlength' => 6,  'class' => array('readonly_view'));
						break;
						
				case "pe_details":
						$readonly_full_name_principal_emp 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_gender_pe 								= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
						$readonly_address_principal_emp	 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_address_principal_emp_Textarea 			= array('readonly' => 'readonly', 'class' => array('readonly_view'));
						$readonly_emp_country 								= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_emp_state 								= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_emp_dist 									= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
						$readonly_loc_emp_subdv 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_loc_emp_areatype 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_loc_emp_areatype_code 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_loc_emp_vill_ward 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
						$readonly_loc_emp_ps 								= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_loc_emp_pin_number						= array('disabled' => 'disabled','maxlength' => 6,  'class' => array('readonly_view'));
						break;
						
				case "man_details":
						$readonly_full_name_manager 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_address_manager 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_address_manager_Textarea  				= array('readonly' => 'readonly', 'class' => array('readonly_view'));	
						$readonly_manager_country 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_manager_state 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_manager_dist 								= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
						$readonly_loc_manager_subdv 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_loc_manager_areatype 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_loc_manager_areatype_code					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_loc_manager_vill_ward 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
						$readonly_loc_manager_ps 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
						$readonly_loc_manager_pin_number 					= array('disabled' => 'disabled','maxlength' => 6,  'class' => array('readonly_view'));
						break;
						
				case "e_nature_of_work":
						$readonly_e_nature_of_work			 				= array('readonly' => 'readonly','style' =>'height: 150px;', 'class' => array('readonly_view'));
						$readonly_other_nature_of_work_value 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						break;
						
				case "max_num_wrkmen":
						$readonly_max_num_wrkmen 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						break;
						
				case "e_num_of_workmen_per_or_reg":
						$readonly_e_num_of_workmen_per_or_reg 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						break;
						
				case "e_num_of_workmen_temp_or_reg":
						$readonly_e_num_of_workmen_temp_or_reg 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						break;
						
				case "workmen_if_same_similar_kind_of_work":
						$readonly_workmen_if_same_similar_kind_of_work 		= array('disabled' => 'disabled', 'class' => array());
						break;
						
				case "con_lab_job_desc":
						$readonly_con_lab_job_desc 							= array('readonly' => 'readonly', 'class' => array('readonly_view'));
						break;
						
				case "con_lab_wage_rate_other_benefits":
						$readonly_con_lab_wage_rate_other_benefits 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						break;
											
				case "con_lab_cat_desig_nom":
						$readonly_con_lab_cat_desig_nom 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						break;	
								
				case "e_settlement_award_judgement_min_wage":
						$readonly_e_settlement_award_judgement_min_wage 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						break;	
						
				case "e_any_day_max_num_of_workmen":
						$readonly_e_any_day_max_num_of_workmen 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
						break;
						
				case "trade_license_file":
						$readonly_trade_license 							= array('disabled' => 'disabled', 'class' => array(),$tlf => $tlf);
						break;
						
				case "article_of_assoc_file":
						$readonly_article_of_assoc 							= array('disabled' => 'disabled', 'class' => array(), $asf => $asf);
						break;
						
				case "memorandum_of_cert_file":
						$readonly_memorandum_of_cert 						= array('disabled' => 'disabled', 'class' => array(), $mcf => $mcf);
						break;
						
				case "partnership_deed_file":
						$readonly_partnership_deed 							= array('disabled' => 'disabled', 'class' => array(), $pdf => $pdf);
						break;
						
						
				case "factory_license_file":
						$readonly_factory_license 							= array('disabled' => 'disabled', 'class' => array(), $flf => $flf );
						break;
						
				default:
				}
			}
		}
		$form['clra_amendment_edit']['application_id'] 				= array(
																			'#title'			=> 'Application Id',
																			'#type' 			=> 'hidden',
																			'#default_value'	=> !empty($amend_application_id) ? encryption_decryption_fun('encrypt', $amend_application_id) : '',
																			'#attributes'		=> array('readonly'=>'readonly')
																		);
		
		$form['clra_amendment_edit']['act_id'] 						= array(
																			'#title'			=> 'Act Id',
																			'#type' 			=> 'hidden',
																			'#default_value'	=> !empty($act_id) ? encryption_decryption_fun('encrypt', $act_id) : '',
																			'#attributes' 		=> array('readonly'=>'readonly')
																		);
												
		$form['clra_amendment_edit']['identification_number'] 		= array(
																			'#title'			=>'Identification Number',
																			'#type' 			=> 'hidden',
																			'#default_value'	=> !empty($identification_number) ? encryption_decryption_fun('encrypt', $identification_number) : '',
																			'#attributes' 		=> array('readonly'=>'readonly')
																		);
		
		$form['clra_amendment_edit']['amendment_parent_id'] 		 = array(
																			'#title'			=> 'Identification Number',
																			'#type' 			=> 'hidden',
																			'#default_value'	=> !empty($amendment_parent_id) ? encryption_decryption_fun('encrypt', $amendment_parent_id) : '',
																			'#attributes' 		=> array('readonly'=>'readonly')
																		);
			
		$form['clra_amendment_edit']['markup1'] 						= array(
																		  '#type'				=> 'markup',
																		  '#markup' 			=> '<div class="content"><br class="clear"><div class="reset-amd-fields">',
																		);
										
		$form['clra_amendment_edit']['markup2']						=	array(
																'#type' 			=> 'markup',
																'#markup'			=> l('Reset Amendment Fields', 'apply-clra-reg-amendment/'.encryption_decryption_fun('encrypt', $get_last_master_application['id']), array('attributes' => array('html' => TRUE, 'class' => 'link-btn pull-right btn-reset-amd-fields')))
																		);	
		$form['clra_amendment_edit']['e_name'] 						= array(
																		  '#prefix' 			=>  '<div class="panel panel-default panel-form"><div class="panel-heading">1. Name and location of the Establishment <span id="rlo_location_id" class="form_note"></span></div><div class="panel-body"><div class="col-lg-4 col-md-6"><label class="input">',
																		  '#suffix' 			=>  '</label></div>',
																		  '#title'				=>  'Establishment Name.',
																		  '#type' 				=>  'textfield',
																		  '#required' 			=>  TRUE,
																		  '#default_value' 		=>  !empty($e_name) ? $e_name : '',
																		  '#attributes'			=>  $readonly_e_name,
																		  //'#element_validate' 	=>  array('MISC_TEXTBOX_TEXT_ENAME_VALIDATE')
																		);
		$form['clra_amendment_edit']['est_type'] 				= array(
																			'#title' 			=> 'Type of The Establishment',
																			'#type' 			=> 'select',
																			'#options' 			=> array('micro' => 'Micro', 'small' => 'Small', 'medium' => 'Medium', 'large' => 'Large'),
																			'#required' 		=>  TRUE,
																			'#default_value' 	=> $est_type,
																			//'#attributes' 		=> $readonly_estType,
																			'#prefix' 			=> '<div class="col-md-4"><label class="select">',
																			'#suffix' 			=> '</label></div>'
																	);																	  
		
		$form['clra_amendment_edit']['loc_e_name'] 					= array(
																		  '#prefix' 			=>  '<div class="col-lg-4 col-md-6"><label class="input">',
																		  '#suffix' 			=>  '</label></div>',
																		  '#title'				=>  'Location',
																		  '#type' 				=>  'textfield',
																		  '#required' 			=>  TRUE,
																		  '#default_value' 		=>  !empty($loc_e_name) ? $loc_e_name : '',
																		  '#attributes'			=>  $readonly_addressLine,
																		  '#element_validate' 	=>  array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																		);																	  
		$form['clra_amendment_edit']['loc_e_dist'] 					= array(
																		  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="select">',
																		  '#suffix' 			=> '</label></div>',
																		  '#title'				=> 'Select District',
																		  '#type' 				=> 'select',
																		  '#options' 			=> common_custom_user_district_list_new(),
																		  '#default_value' 		=> !empty($loc_e_dist) ? $loc_e_dist : '',
																		  '#attributes'			=>  $readonly_e_location,
																		  '#required' 			=> TRUE,
																		  '#ajax'				=> array(
																										'event' 	=> 'change',
																										'effect' 	=> 'fade',			
																										'callback'	=> 'get_ajax_location_amendment',
																										'progress'	=> array('type' => 'throbber'),
																									  )
																		);
		$value_loc_e_dist 	= isset($form_state['values']['loc_e_dist']) ? $form_state['values']['loc_e_dist'] : $loc_e_dist;
		
		if($value_loc_e_dist != "" || !empty($value_loc_e_dist) || is_numeric($value_loc_e_dist)){
					
			$form['clra_amendment_edit']['loc_e_subdivision'] 		= array(
																		  '#prefix' 			=> '<div class="col-lg-4 col-md-6" id="clra_subdvsn_replace"><label class="select">',
																		  '#suffix' 			=> '</label></div>',
																		  '#title'				=> 'Select Subdivision',
																		  '#type' 				=> 'select',
																		  '#options' 			=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist),
																		  '#required' 			=> TRUE,
																		  '#default_value' 		=> !empty($loc_e_subdivision) ? $loc_e_subdivision : '',
																		  '#attributes'			=> $readonly_e_loc_subdv,
																		  '#id'					=> 'loc_e_subdivision_id',
																		  '#ajax'				=> array(
																										'event' 	=> 'change',
																										'effect' 	=> 'fade',			
																										'callback'	=> 'get_ajax_location_amendment',
																										'progress'	=> array('type' => 'throbber'),
																									  ),
																	);
																											
		}else{
					
			$form['clra_amendment_edit']['loc_e_subdivision'] 	= array(
																	  '#prefix' 				=> '<div class="col-lg-4 col-md-6" id="clra_subdvsn_replace"><label class="select">',
																	  '#suffix' 				=> '</label></div>',
																	  '#title'					=> 'Select Subdivision',
																	  '#type' 					=> 'select',
																	  '#options' 				=> array(),
																	  '#attributes'				=> $readonly_e_loc_subdv,
																	  '#id'						=> 'loc_e_subdivision_id',
																	  '#required' 				=> TRUE,
																	  '#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_ajax_location_amendment',
																									'progress'	=> array('type' => 'throbber'),
																								  ),
																);
		}
		
		$value_loc_e_subdivision = isset($form_state['values']['loc_e_subdivision']) ? $form_state['values']['loc_e_subdivision'] : $loc_e_subdivision;
		
		if($value_loc_e_dist != "" && $value_loc_e_subdivision != ""){
					
			$form['clra_amendment_edit']['loc_e_areatype']  	= array(
																	  '#prefix' 				=> '<div class="col-lg-4 col-md-6" id="clra_loc_e_areatype"><label class="select">',
																	  '#suffix' 				=> '</label></div>',
																	  '#title'					=> 'Select Block / Municipality / Corporation / SEZ / Notified Area',
																	  '#type' 					=> 'select',
																	  '#options' 				=> common_custom_user_areatype_list_new($value_loc_e_dist, $value_loc_e_subdivision),
																	  '#default_value'			=> !empty($loc_e_areatype) ? $loc_e_areatype : '',
																	  '#required' 				=> TRUE,
																	  '#attributes'				=> $readonly_e_location,
																	  '#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_ajax_location_amendment',
																									'progress'	=> array('type' => 'throbber'),
																								  ),
																		);
																													
		}else{
					
			$form['clra_amendment_edit']['loc_e_areatype'] 		= array(
																	  '#prefix' 				=> '<div class="col-lg-4 col-md-6" id="clra_loc_e_areatype"><label class="select">',
																	  '#suffix' 				=> '</label></div>',
																	  '#title'					=> 'Select Block / Municipality / Corporation / SEZ / Notified Area',
																	  '#type' 					=> 'select',
																	  '#options' 				=> array(),
																	  '#required' 				=> TRUE,
																	  '#attributes'				=> $readonly_e_location,
																	  '#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_ajax_location_amendment',
																									'progress'	=> array('type' => 'throbber'),
																								  ),
																);
		}
		
		$value_loc_e_areatype 	= isset($form_state['values']['loc_e_areatype']) ? $form_state['values']['loc_e_areatype'] : $loc_e_areatype;
			
		if ($value_loc_e_dist != "" && $value_loc_e_subdivision != "" && $value_loc_e_areatype != "") {
				
			if ( $value_loc_e_areatype == 'B' ){
				$areatype_name	= 'Block';
			}elseif( $value_loc_e_areatype == 'M'){
				$areatype_name	= 'Municipality';
			}elseif( $value_loc_e_areatype == 'C'){
				$areatype_name	= 'Corporation';
			}elseif( $value_loc_e_areatype == 'S'){
				$areatype_name	= 'SEZ';
			}elseif($value_loc_e_areatype == 'N'){
				$areatype_name	= 'Notified Area';
			}
			
			$form['clra_amendment_edit']['name_areatype'] 		= array(
																	  '#prefix' 				=> '<div class="col-lg-4 col-md-6" id="clra_common_1_replace"><label class="select">',
																	  '#suffix' 				=> '</label></div>',
																	  '#title'					=> 'Select '.$areatype_name,
																	  '#type' 					=> 'select',
																	  '#options' 				=> common_custom_user_block_list_new($value_loc_e_subdivision, $value_loc_e_areatype),
																	  '#required' 				=> TRUE,
																	  '#attributes'				=> $readonly_e_location,
																	  '#default_value'			=> !empty($loc_e_areatype_code) ? $loc_e_areatype_code : '',
																	  '#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_ajax_location_amendment',
																									'progress'	=> array('type' => 'throbber'),
																								  )
															 );	
					
		}else{
					
			$form['clra_amendment_edit']['name_areatype'] 		= array(
																	 '#prefix' 					=> '<div class="col-lg-4 col-md-6" id="clra_common_1_replace"><label class="select">',
																	 '#suffix' 					=> '</label></div>',
																	 '#title'					=> 'Select Block',
																	 '#type' 					=> 'select',
																	 '#options' 				=> array(),
																	 '#required' 				=> TRUE,
																	 '#attributes'				=> $readonly_e_location,
																	 '#id'						=> 'common_selection_1_id',
																	 '#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_ajax_location_amendment',
																									'progress'	=> array('type' => 'throbber'),
																								  )
																  );
					
		}	
		$value_loc_areatype_code  = isset($form_state['values']['name_areatype']) ? $form_state['values']['name_areatype'] : $loc_e_areatype_code;
		
		if($value_loc_e_dist != "" && $value_loc_e_subdivision != "" && $value_loc_e_areatype != ""  && $value_loc_areatype_code != "" ){
					
			if ( $value_loc_e_areatype == 'B' ){
				$area_name		= 'Gram Panchayat';
				$area_code 		= 'V';
			}elseif( $value_loc_e_areatype == 'M'){
				$area_name		= 'Ward';
				$area_code 		= 'W';
			}elseif( $value_loc_e_areatype == 'C'){
				$area_name		= 'Ward';
				$area_code 		= 'C';
			}elseif( $value_loc_e_areatype == 'S'){
				$area_name		= 'Sector';
				$area_code 		= 'S';
			}elseif($value_loc_e_areatype == 'N'){
				$area_name		= 'Notified Area';
				$area_code 		= 'N';
			}
			
					
			$form['clra_amendment_edit']['loc_e_vill_ward'] 	= array(
																	  '#prefix' 				=> '<div class="col-lg-4 col-md-6" id="clra_common_2_replace"><label class="select">',
																	  '#suffix' 				=> '</label></div>',
																	  '#title'					=> 'Select '.$area_name,
																	  '#type' 					=> 'select',
																	  '#options'				=> common_custom_user_villward_list_new($value_loc_areatype_code,$area_code),
																	  '#attributes'				=> $readonly_location,
																	  '#default_value'			=> !empty($loc_e_vill_ward) ? $loc_e_vill_ward : '',
																	  '#required' 				=> TRUE,
																	  '#ajax'					=> array(
																										'event' 	=> 'change',
																										'effect' 	=> 'fade',			
																										'callback'	=> 'get_ajax_location_amendment',
																										'progress'	=> array('type' => 'throbber'),
																									  )
																	);
					
		}else{
					
			$form['clra_amendment_edit']['loc_e_vill_ward'] 	= array(
																	 '#prefix' 					=> '<div class="col-lg-4 col-md-6" id="clra_common_2_replace"><label class="select">',
																	 '#suffix' 					=> '</label></div>',
																	 '#title'					=> 'Select Gram Panchayat / Ward / Sector / Notified Area',
																	 '#type' 					=> 'select',
																	 '#options' 				=> array(),
																	 '#attributes'				=> $readonly_location,
																	 '#required' 				=> TRUE,
																	 '#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_ajax_location_amendment',
																									'progress'	=> array('type' => 'throbber'),
																								  )
																  );
		}
		$value_loc_vill_ward = isset($form_state['values']['loc_e_vill_ward']) ? $form_state['values']['loc_e_vill_ward'] : $loc_e_vill_ward;
		if ($value_loc_e_dist != "" && $value_loc_e_subdivision != "" && $value_loc_e_areatype != ""  && $value_loc_areatype_code != "" && $value_loc_vill_ward != "" ){
					
			$form['clra_amendment_edit']['loc_e_ps'] 				= array(
																	  '#prefix' 				=> '<div class="col-lg-4 col-md-6" id="clra_ps_replace"><label class="select">',
																	  '#suffix' 				=> '</label></div>',
																	  '#title'					=> 'Select Police Station',
																	  '#type' 					=> 'select',
																	  '#options' 				=> common_custom_user_ps_list_new($value_loc_e_dist), 
																	  '#attributes'				=> $readonly_location,
																	  '#default_value'			=> !empty($loc_e_ps) ? $loc_e_ps : '',
																	  '#required' 				=> TRUE
																  );
					
		}else{
					
			$form['clra_amendment_edit']['loc_e_ps'] 				= array(
																		'#prefix' 				=> '<div class="col-lg-4 col-md-6" id="clra_ps_replace"><label class="select">',
																		'#suffix' 				=> '</label></div>',
																		'#title'				=> 'Select Police Station',
																		'#type' 				=> 'select',
																		'#options' 				=> array(),
																		'#required' 			=> TRUE,
																		'#attributes'			=> $readonly_location,
																		'#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_ajax_location_amendment',
																									'progress'	=> array('type' => 'throbber'),
																								  )
																  );
					
		}	
		$form['clra_amendment_edit']['loc_e_pin_number'] 			= array(
																	  '#prefix' 				=> '<div class="col-lg-4 col-md-6"><label class="input">',
																	  '#suffix' 				=> '</label></div>',
																	  '#title'					=> 'Pin Code',
																	  '#type' 					=> 'textfield',
																	  '#attributes' 			=> $readonly_loc_e_pin_number,
																	  '#default_value'			=> !empty($loc_e_pin_number) ? $loc_e_pin_number : '',
																	  '#required' 				=> TRUE,
																	  '#element_validate' 		=> array('MISC_PIN_NUMBER'),
																	);	
		$form['clra_amendment_edit']['e_postal_address'] 			= array(
																	  '#prefix' 				=> '</div></div><div class="panel panel-default panel-form"><div class="panel-heading">2. Registered Office address of the Establishment</div><div class="panel-body"><div class="col-lg-4 col-md-6"><label class="textarea">',
																	  '#suffix' 				=> '</label></div>',
																	  '#title'					=> 'Address Line 1:',
																	  '#type' 					=> 'textarea',
																	  '#default_value' 			=> !empty($e_postal_address) ? $e_postal_address : '',
																	  '#required' 				=> TRUE,
																	  '#rows'					=> t('4'),
																	  '#attributes' 			=> $readonly_e_postal_address,
																	  '#element_validate' 		=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																	);																		
																						
			
				
		$form['clra_amendment_edit']['e_postal_dist'] 				= array(
																	  '#prefix' 				=> '<div class="col-lg-4 col-md-6"><label class="select">',
																	  '#suffix' 				=> '</label></div>',
																	  '#title'					=> 'Select District',
																	  '#type' 					=> 'select',
																	  '#options' 				=> common_custom_user_district_list_new(),
																	  '#default_value' 			=> !empty($e_postal_dist) ? $e_postal_dist:'',
																	  '#attributes' 			=> $readonly_e_postal_dist,
																	  '#required' 				=> TRUE,
																	  '#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect'	=> 'fade',			
																									'callback'	=> 'get_ajax_postal_amendment',
																									'progress'	=> array('type' => 'throbber'),
																								  )
																	);
				
		$value_loc_e_dist_1 = 	isset($form_state['values']['e_postal_dist']) ? $form_state['values']['e_postal_dist'] : $e_postal_dist;
		
		if($value_loc_e_dist_1 != "" || !empty($value_loc_e_dist_1) || is_numeric($value_loc_e_dist_1)){
					
			$form['clra_amendment_edit']['e_postal_subdivision'] = array(
																	  '#prefix' 				=> '<div class="col-lg-4 col-md-6" id="clra_subdvsn_replace_1"><label class="select">',
																	  '#suffix' 				=> '</label></div>',
																	  '#title'					=> 'Select Subdivision',
																	  '#type' 					=> 'select',
																	  '#options' 				=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist_1),
																	  '#default_value' 			=>!empty($e_postal_subdivision) ? $e_postal_subdivision : '',
																	  '#attributes' 			=> $readonly_e_postal_subdivision,
																	  '#required' 				=> TRUE,
																	  '#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_ajax_postal_amendment',
																									'progress'	=> array('type' => 'throbber'),
																								  ),
																);
		}else{
					
			$form['clra_amendment_edit']['e_postal_subdivision'] = array(
																	  '#prefix' 				=> '<div class="col-lg-4 col-md-6" id="clra_subdvsn_replace_1"><label class="select">',
																	  '#suffix' 				=> '</label></div>',
																	  '#title'					=> 'Select Subdivision',
																	  '#type' 					=> 'select',
																	  '#options' 				=> array(),
																	  '#attributes' 			=> $readonly_e_postal_subdivision,
																	  '#required' 				=> TRUE,
																	  '#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_ajax_postal_amendment',
																									'progress'	=> array('type' => 'throbber'),
																								  ),
																		);
		}
		$value_loc_e_subdivision_1 	= isset($form_state['values']['e_postal_subdivision']) ? $form_state['values']['e_postal_subdivision'] : $e_postal_subdivision;
		
		if($value_loc_e_dist_1 != "" && $value_loc_e_subdivision_1 != ""){
			
			$form['clra_amendment_edit']['e_postal_areatype'] 	= array(
																	  '#prefix' 				=> '<div class="col-lg-4 col-md-6" id="clra_loc_e_areatype_1"><label class="select">',
																	  '#suffix' 				=> '</label></div>',
																	  '#title'					=> 'Select Block / Municipality / Corporation / SEZ / Notified Area',
																	  '#type' 					=> 'select',
																	  '#options' 				=> common_custom_user_areatype_list_new($value_loc_e_dist_1, $value_loc_e_subdivision_1),
																	  '#default_value'			=> !empty($e_postal_areatype) ? $e_postal_areatype : '',
																	  '#attributes' 			=> $readonly_e_postal_areatype,
																	  '#required' 				=> TRUE,
																	  '#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_ajax_postal_amendment',
																									'progress'	=> array('type' => 'throbber'),
																								  ),
																		);
		}else{
					
			$form['clra_amendment_edit']['e_postal_areatype'] 	= array(
																	  '#prefix' 				=> '<div class="col-lg-4 col-md-6" id="clra_loc_e_areatype_1"><label class="select">',
																	  '#suffix' 				=> '</label></div>',
																	  '#title'					=> 'Select Block / Municipality / Corporation / SEZ / Notified Area',
																	  '#type' 					=> 'select',
																	  '#options' 				=> array(),
																	  '#required' 				=> TRUE,
																	  '#attributes' 			=> $readonly_e_postal_areatype,
																	  '#ajax'					=> array(
																										'event' 	=> 'change',
																										'effect' 	=> 'fade',			
																										'callback'	=> 'get_ajax_postal_amendment',
																										'progress'	=> array('type' => 'throbber'),
																									  ),
																);
		}
		$value_loc_e_areatype_1 = isset($form_state['values']['e_postal_areatype']) ? $form_state['values']['e_postal_areatype'] : $e_postal_areatype;
		
		if($value_loc_e_dist_1 != "" && $value_loc_e_subdivision_1 != "" && $value_loc_e_areatype_1 != ""){
			
			if ( $value_loc_e_areatype_1 == 'B' ){
				$areatype_name	= 'Block';
			}elseif( $value_loc_e_areatype_1 == 'M'){
				$areatype_name	= 'Municipality';
			}elseif( $value_loc_e_areatype_1 == 'C'){
				$areatype_name	= 'Corporation';
			}elseif( $value_loc_e_areatype_1 == 'S'){
				$areatype_name	= 'SEZ';
			}elseif($value_loc_e_areatype_1 == 'N'){
				$areatype_name	= 'Notified Area';
			}
			
			$form['clra_amendment_edit']['e_postal_name_areatype'] = array(
																	  '#prefix' 				=> '<div class="col-lg-4 col-md-6" id="clra_common_1_replace_1"><label class="select">',
																	  '#suffix' 				=> '</label></div>',
																	  '#title'					=> 'Select '.$areatype_name,
																	  '#type' 					=> 'select',
																	  '#options' 				=> common_custom_user_block_list_new($value_loc_e_subdivision_1, $value_loc_e_areatype_1),
																	  '#default_value'			=> !empty($e_postal_name_areatype) ? $e_postal_name_areatype : '',
																	  '#required' 				=> TRUE,
																	  '#attributes' 			=> $readonly_e_postal_areatype_code,
																	  '#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_ajax_postal_amendment',
																									'progress'	=> array('type' => 'throbber'),
																								  )
																	);
					
		}else{
					
			$form['clra_amendment_edit']['e_postal_name_areatype']= array(
																	 '#prefix' 					=> '<div class="col-lg-4 col-md-6" id="clra_common_1_replace_1"><label class="select">',
																	 '#suffix' 					=> '</label></div>',
																	 '#title'					=> 'Select Block',
																	 '#type' 					=> 'select',
																	 '#options' 				=> array(),
																	 '#required' 				=> TRUE,
																	 '#attributes' 				=> $readonly_e_postal_areatype_code,
																	 '#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_ajax_postal_amendment',
																									'progress'	=> array('type' => 'throbber'),
																								  )
														  );
		}	
		$value_common_selection_1_1 	= isset($form_state['values']['e_postal_name_areatype']) ? $form_state['values']['e_postal_name_areatype'] : $e_postal_name_areatype;
		if($value_loc_e_dist_1 != "" && $value_loc_e_subdivision_1 != "" && $value_loc_e_areatype_1 != "" && $value_common_selection_1_1 != ""){
			
			if ( $value_loc_e_areatype_1 == 'B' ){
				$area_name		= 'Gram Panchayat';
				$area_code 		= 'V';
			}elseif( $value_loc_e_areatype_1 == 'M'){
				$area_name		= 'Ward';
				$area_code 		= 'W';
			}elseif( $value_loc_e_areatype_1 == 'C'){
				$area_name		= 'Ward';
				$area_code 		= 'C';
			}elseif( $value_loc_e_areatype_1 == 'S'){
				$area_name		= 'Sector';
				$area_code 		= 'S';
			}elseif($value_loc_e_areatype_1 == 'N'){
				$area_name		= 'Notified Area';
				$area_code 		= 'N';
			}
					
			$form['clra_amendment_edit']['e_postal_vill_ward'] 		= array(
																	  '#prefix' 				=> '<div class="col-lg-4 col-md-6" id="clra_common_2_replace_1"><label class="select">',
																	  '#suffix' 				=> '</label></div>',
																	  '#title'					=> 'Select '.$area_name,
																	  '#type' 					=> 'select',
																	  '#options' 				=> common_custom_user_villward_list_new($value_common_selection_1_1,$area_code),
																	  '#default_value'			=> !empty($e_postal_vill_ward) ? $e_postal_vill_ward : '',
																	  '#required' 				=> TRUE,
																	  '#attributes' 			=> $readonly_e_postal_vill_ward,
																	  '#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_ajax_postal_amendment',
																									'progress'	=> array('type' => 'throbber'),
																								  )
																	); 
		}else{
				
			$form['clra_amendment_edit']['e_postal_vill_ward'] 		= array(
																	 '#prefix' 					=> '<div class="col-lg-4 col-md-6" id="clra_common_2_replace_1"><label class="select">',
																	 '#suffix' 					=> '</label></div>',
																	 '#title'					=> 'Select Gram Panchayat / Ward / Sector / Notified Area',
																	 '#type' 					=> 'select',
																	 '#options' 				=> array(),
																	 '#required' 				=> TRUE,
																	 '#attributes' 				=> $readonly_e_postal_vill_ward,
																	 '#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_ajax_postal_amendment',
																									'progress'	=> array('type' => 'throbber'),
																								  )
																  );
		}
		$value_common_selection_2_1 = isset($form_state['values']['e_postal_vill_ward']) ? $form_state['values']['e_postal_vill_ward'] : $e_postal_vill_ward;
		if ($value_loc_e_dist_1 != "" && $value_loc_e_subdivision_1 != "" && $value_loc_e_areatype_1 != "" && $value_common_selection_1_1 != "" && $value_common_selection_2_1 != "" ){
			
			$form['clra_amendment_edit']['e_postal_ps'] 		= array(
																	  '#prefix' 				=> '<div class="col-lg-4 col-md-6" id="clra_ps_replace_1"><label class="select">',
																	  '#suffix' 				=> '</label></div>',
																	  '#title'					=> 'Select Police Station',
																	  '#type' 					=> 'select',
																	  '#attributes' 			=> $readonly_e_postal_ps,
																	  '#options' 				=> common_custom_user_ps_list_new($value_loc_e_dist_1),
																	  '#default_value'			=> !empty($e_postal_ps) ? $e_postal_ps : '',
																	  '#required' 				=> TRUE
																  );
		}else{
				
			$form['clra_amendment_edit']['e_postal_ps'] 		= array(
																		'#prefix' 				=> '<div class="col-lg-4 col-md-6" id="clra_ps_replace_1"><label class="select">',
																		'#suffix' 				=> '</label></div>',
																		'#title'				=> 'Select Police Station',
																		'#type' 				=> 'select',
																		'#options' 				=> array(),
																		'#required' 			=> TRUE,
																		 '#attributes' 			=> $readonly_e_postal_ps,
																		'#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_ajax_postal_amendment',
																									'progress'	=> array('type' => 'throbber'),
																								  )
																  );
		}
		
		$form['clra_amendment_edit']['e_postal_pin_number'] 	= array(
																	  '#prefix' 				=> '<div class="col-lg-4 col-md-6"><label class="input">',
																	  '#suffix' 				=> '</label></div>',
																	  '#title'					=> 'Pin Code',
																	  '#type' 					=> 'textfield',
																	  '#attributes' 			=> $readonly_e_postal_pin_number,
																	  '#default_value'			=> !empty($e_postal_pin_number) ? $e_postal_pin_number : '',
																	  '#required' 				=> TRUE,
																	  '#element_validate' 		=> array('MISC_PIN_NUMBER'),	
																  );
																  
		$form['clra_amendment_edit']['full_name_principal_emp']	= array(
																	  '#prefix' 			=> '</div></div><div class="panel panel-default panel-form"><div class="panel-heading">3. Full Name and address of the Principal Employer</div><div class="panel-body"><div class="col-md-4"><label class="input">',
																	  '#suffix' 			=> '</label></div>',
																	  '#title'				=> 'Principal Employer Name',
																	  '#type' 				=> 'textfield',
																	  '#default_value' 		=> $full_name_principal_emp,
																	  '#attributes' 		=> $readonly_full_name_principal_emp,
																	  '#required' 			=> TRUE,
																	  '#element_validate'	=> array('MISC_TEXTBOX_TEXT_ENAME_VALIDATE')
																	);
		$form['clra_amendment_edit']['gender_pe'] 				= array(
																	  '#prefix'				=> '<div class="col-md-4"><div class="inline-group">',
																	  '#suffix'				=>'</div></div>',
																	  '#type' 				=> 'radios',
																	  '#title' 				=> t('Gender'),
																	  '#options' 			=> array('M'=>t('Male'), 'F'=>t('Female'), 'O'=>t('Transgender')),
																	  '#default_value' 		=> isset($gender_pe) ? $gender_pe : NULL,
																	  //'#attributes' 		=> $readonly_gender_pe,
																	  '#required' 			=> TRUE
																	);
		$form['clra_amendment_edit']['emp_country'] 			= array(
																	'#prefix' 				=> '<div id ="countrydiv" class="col-md-4"><label class="select">',
																	'#suffix' 				=> '</label></div>',
																	'#title'				=> 'Select Country',
																	'#type' 				=> 'select',
																	'#options' 				=> array('1' => 'India', '2' => 'Others'),
																	'#default_value'		=> !empty($loc_emp_country) ? $loc_emp_country : 0,
																	'#attributes' 			=> $readonly_emp_country,
																	'#required' 			=> TRUE,
																	'#ajax'					=> array(
																								'event' 	=> 'change',
																								'effect' 	=> 'fade',			
																								'callback'	=> 'get_ajax_pe_country',
																								'progress'	=> array('type' => 'throbber'),
																								)
																	);
		$value_emp_country 	= isset($form_state['values']['emp_country']) ? $form_state['values']['emp_country'] : $loc_emp_country;
		
		$form['clra_amendment_edit']['emp_state']					= array(
																		'#prefix' 				=> '<div id="clra_emp_state_replace">',
																		'#suffix' 				=> '</div>',
																		);	
		$form['clra_amendment_edit']['address_principal_emp']		= array(
																		'#prefix' 				=> '<div id="add_line_replace">',
																		'#suffix' 				=> '</div>',
																		);
		$form['clra_amendment_edit']['emp_dist']					= array(
																		'#prefix' 				=> '<div id="clra_emp_district_replace">',
																		'#suffix' 				=> '</div>',
																		);	
																							
		$form['clra_amendment_edit']['loc_emp_subdv']				= array(
																		'#prefix' 				=> '<div id="clra_subdvsn_replace_2">',
																		'#suffix' 				=> '</div>',
																		);	
																						
		$form['clra_amendment_edit']['loc_emp_areatype']			= array(
																		'#prefix' 				=> '<div id="clra_loc_e_areatype_2">',
																		'#suffix' 				=> '</div>',
																		);
																						
		$form['clra_amendment_edit']['emp_name_areatype']			= array(
																		'#prefix' 				=> '<div id="clra_common_1_replace_2">',
																		'#suffix' 				=> '</div>',
																		);
																						
		$form['clra_amendment_edit']['loc_emp_vill_ward']			= array(
																		'#prefix' 				=> '<div id="clra_common_2_replace_2">',
																		'#suffix' 				=> '</div>',
																		);
																
		$form['clra_amendment_edit']['loc_emp_ps']					= array(
																		'#prefix' 				=> '<div id="clra_ps_replace_2">',
																		'#suffix' 				=> '</div>',
																		);	
																	
		$form['clra_amendment_edit']['loc_emp_pin_number']			= array(
																		'#prefix' 				=> '<div id="clra_pin_replace_2">',
																		'#suffix' 				=> '</div>',
																		);	
																		
		if( (!empty($value_emp_country) && $value_emp_country != '') && $value_emp_country ==  1) {
			$form['clra_amendment_edit']['emp_state']				= array(
																		'#prefix' 				=> '<div id="clra_emp_state_replace" class="col-lg-4 col-md-4"><label class="select">',
																		'#suffix' 				=> '</label></div>',
																		'#title'				=> t('Select State'),
																		'#type' 				=> 'select',
																		'#options' 				=> get_all_state($value_emp_country),
																		'#attributes'			=> $readonly_emp_state,
																		'#default_value'		=> !empty($loc_emp_state) ? $loc_emp_state : '',
																		'#required' 			=> TRUE,
																		'#ajax'					=> array(
																										'event'		=> 'change',
																										'effect' 	=> 'fade',
																										'callback'	=> 'get_ajax_pe_country',
																										'progress'	=> array('type' => 'throbber'),
																										)
																		);	
																											
			$value_emp_state	= isset($form_state['values']['emp_state'])? $form_state['values']['emp_state'] : $loc_emp_state;
			if($value_emp_state != $loc_emp_state ){
				$loc_emp_dist 			= '';
				$loc_emp_subdivision 	= '';
				$loc_emp_areatype		= '';
				$emp_name_areatype		= '';
				$loc_emp_vill_ward		= '';
				$l_emp_ps				= '';
				$loc_emp_pin_number		= '';
			}
			$form['clra_amendment_edit']['address_principal_emp'] 	= array(
																		  '#prefix' 			=> '<div id="add_line_replace" class="col-lg-8 col-md-8"><label class="input">',
																		  '#suffix' 			=> '</label></div>',
																		  '#title'				=> 'Address Line1',
																		  '#type' 				=> 'textfield',
																		  '#default_value' 		=> !empty($address_principal_emp) ? $address_principal_emp : '',
																		  '#required' 			=> TRUE,
																		  '#attributes'			=> $readonly_address_principal_emp,
																		  '#element_validate' 	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																		);
																		
			if(($value_emp_country != '' && (!empty($value_emp_state) || $value_emp_state != '' )  )){
							
				$db_district_value	= db_query("SELECT COUNT(id) FROM {district_master} WHERE state_id = '".$value_emp_state."'")->fetchObject();
							
				if(!empty($db_district_value) && ($db_district_value->count>0) ) {
							
					$form['clra_amendment_edit']['emp_dist']		= array(
																		'#prefix' 				=> '<div id="clra_emp_district_replace" class="col-lg-4 col-md-6"><label class="select">',
																		'#suffix' 				=> '</label></div>',
																		'#title'				=> t('Select District'),
																		'#type' 				=> 'select',
																		'#options' 				=> get_district_by_id($value_emp_state),
																		'#default_value' 		=> !empty($loc_emp_dist) ? $loc_emp_dist : '',
																		'#attributes'			=> $readonly_emp_dist,
																		'#required' 			=> TRUE,
																		'#ajax'					=> array(
																										'event'		=> 'change',
																										'effect' 	=> 'fade',
																										'callback'	=> 'get_ajax_pe_country',
																										'progress'	=> array('type' => 'throbber'),
																									)
																);	
																											
					$value_loc_e_dist_2		= isset($form_state['values']['emp_dist']) ? $form_state['values']['emp_dist'] : $loc_emp_dist;
					
					if($value_loc_e_dist_2 != $loc_emp_dist ){
						$loc_emp_subdivision 	= '';
						$loc_emp_areatype		= '';
						$emp_name_areatype		= '';
						$loc_emp_vill_ward		= '';
						$l_emp_ps				= '';
						$loc_emp_pin_number		= '';
					}
				}
			}
			if($value_loc_e_dist_2 != ''  && !empty($value_emp_state)){
	
				$db_subdivision_value = db_query("SELECT COUNT(id) as subdv_total FROM {sub_division} WHERE district_code = '".$value_loc_e_dist_2."'")->fetchObject();
						
				if(!empty($db_subdivision_value) && $db_subdivision_value != '' && $db_subdivision_value->subdv_total > 0) {
								
					$form['clra_amendment_edit']['loc_emp_subdv'] 	= array(
																		  '#prefix' 			=> '<div id="clra_subdvsn_replace_2" class="col-lg-4 col-md-6"><label class="select">',
																		  '#suffix' 			=> '</label></div>',
																		  '#title'				=> 'Select Subdivision',
																		  '#type' 				=> 'select',
																		  '#default_value' 		=> !empty($loc_emp_subdivision) ? $loc_emp_subdivision : '',
																		  '#options' 			=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist_2),
																		  '#attributes'			=> $readonly_loc_emp_subdv,
																		  '#required' 			=> TRUE,
																		  '#ajax'				=> array(
																										'event' 	=> 'change',
																										'effect' 	=> 'fade',			
																										'callback'	=> 'get_ajax_pe_country',
																										'progress'	=> array('type' => 'throbber'),
																									  ),
																		);	
																												
					$value_loc_e_subdivision_2	= isset($form_state['values']['loc_emp_subdv']) ? $form_state['values']['loc_emp_subdv'] : $loc_emp_subdivision;
					if($value_loc_e_subdivision_2 != $loc_emp_subdivision ){
						$loc_emp_areatype		= '';
						$emp_name_areatype		= '';
						$loc_emp_vill_ward		= '';
						$l_emp_ps				= '';
						$loc_emp_pin_number		= '';
					}
				}
			}
			if($value_loc_e_dist_2 != "" && !empty($value_emp_state) && $value_loc_e_subdivision_2 != ""){
							
			 $db_block_mun_value = db_query("SELECT COUNT(id) as areatype_total FROM {block_mun_master} WHERE distict_code = '".$value_loc_e_dist_2."' and sub_division_code  = '".$value_loc_e_subdivision_2."'")->fetchObject();
			 if(!empty($db_block_mun_value) &&  $db_block_mun_value != '' && $db_block_mun_value->areatype_total > 0){
									
				$form['clra_amendment_edit']['loc_emp_areatype'] 	= array(
																		  '#prefix' 			=> '<div id="clra_loc_e_areatype_2" class="col-lg-4 col-md-6"><label class="select">',
																		  '#suffix' 			=> '</label></div>',
																		  '#title'				=> 'Select Block / Municipality / Corporation / SEZ / Notified Area',
																		  '#type' 				=> 'select',
																		  '#options' 			=> common_custom_user_areatype_list_new($value_loc_e_dist_2, $value_loc_e_subdivision_2),
																		  '#required' 			=> TRUE,
																		  '#attributes'			=> $readonly_loc_emp_areatype,
																		  '#default_value' 		=> !empty($loc_emp_areatype) ? $loc_emp_areatype : '',
																		  '#ajax'				=> array(
																										'event' 	=> 'change',
																										'effect' 	=> 'fade',			
																										'callback'	=> 'get_ajax_pe_country',
																										'progress'	=> array('type' => 'throbber'),
																									  ),
																			);
				$value_loc_e_areatype_2 = isset($form_state['values']['loc_emp_areatype']) ? $form_state['values']['loc_emp_areatype'] : $loc_emp_areatype;
				if($value_loc_e_subdivision_2 != $loc_emp_subdivision && $value_loc_e_areatype_2 != $loc_emp_areatype){
					$emp_name_areatype		= '';
					$loc_emp_vill_ward		= '';
					$l_emp_ps				= '';
					$loc_emp_pin_number		= '';
				 }
			 }
			}
			if ($value_loc_e_dist_2 != "" && $value_loc_e_subdivision_2 != "" && $value_loc_e_areatype_2 != "") {
							
				if ( $value_loc_e_areatype_2 == 'B' ){
					$areatype_name	= 'Block';
				}elseif( $value_loc_e_areatype_2 == 'M'){
					$areatype_name	= 'Municipality';
				}elseif( $value_loc_e_areatype_2 == 'C'){
					$areatype_name	= 'Corporation';
				}elseif( $value_loc_e_areatype_2 == 'S'){
					$areatype_name	= 'SEZ';
				}elseif($value_loc_e_areatype_2 == 'N'){
					$areatype_name	= 'Notified Area';
				}
			$form['clra_amendment_edit']['emp_name_areatype'] 	 	= array(
																		  '#prefix' 			=> '<div id="clra_common_1_replace_2" class="col-lg-4 col-md-6"><label class="select">',
																		  '#suffix' 			=> '</label></div>',
																		  '#title'				=> 'Select '.$areatype_name,
																		  '#type' 				=> 'select',
																		  '#options' 			=> common_custom_user_block_list_new($value_loc_e_subdivision_2, $value_loc_e_areatype_2),
																		  '#required' 			=> TRUE,
																		  '#default_value' 		=> !empty($emp_name_areatype) ? $emp_name_areatype : '',
																		  '#attributes'			=> $readonly_loc_emp_areatype_code,
																		  '#ajax'				=> array(
																										'event' 	=> 'change',
																										'effect' 	=> 'fade',			
																										'callback'	=> 'get_ajax_pe_country',
																										'progress'	=> array('type' => 'throbber'),
																										  )
																 );
																								 
																								 
			$value_common_selection_1_2 = isset($form_state['values']['emp_name_areatype']) ? $form_state['values']['emp_name_areatype'] : $emp_name_areatype;
			if($value_common_selection_1_2 != $emp_name_areatype){
				$loc_emp_vill_ward		= '';
				$l_emp_ps				= '';
				$loc_emp_pin_number		= '';
			}
		}
			if($value_loc_e_dist_2 != "" && $value_loc_e_subdivision_2 != "" && $value_loc_e_areatype_2 != ""  && $value_common_selection_1_2 != "" && !empty($value_emp_state) ){
				if ( $value_loc_e_areatype_2 == 'B' ){
					$area_name		= 'Gram Panchayat';
					$area_code 		= 'V';
				}elseif( $value_loc_e_areatype_2 == 'M'){
					$area_name		= 'Ward';
					$area_code 		= 'W';
				}elseif( $value_loc_e_areatype_2 == 'C'){
					$area_name		= 'Ward';
					$area_code 		= 'C';
				}elseif( $value_loc_e_areatype_2 == 'S'){
					$area_name		= 'Sector';
					$area_code 		= 'S';
				}elseif($value_loc_e_areatype_2 == 'N'){
					$area_name		= 'Notified Area';
					$area_code 		= 'N';
				}
				$form['clra_amendment_edit']['loc_emp_vill_ward'] 		= array(
																			  '#prefix' 			=> '<div id="clra_common_2_replace_2" class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 			=> '</label></div>',
																			  '#title'				=> 'Select '.$area_name,
																			  '#type' 				=> 'select',
																			  '#options'			=> common_custom_user_villward_list_new($value_common_selection_1_2,$area_code),
																			  '#attributes' 		=> $readonly_loc_emp_vill_ward,
																			  '#required' 			=> TRUE,
																			  '#default_value' 		=> !empty($loc_emp_vill_ward) ? $loc_emp_vill_ward : '',
																			  '#ajax'				=> array(
																											'event' 	=> 'change',
																											'effect' 	=> 'fade',			
																											'callback'	=> 'get_ajax_pe_country',
																											'progress'	=> array('type' => 'throbber'),
																									  )
																			);	
																											
				$value_common_selection_2_2 = isset($form_state['values']['loc_emp_vill_ward']) ? $form_state['values']['loc_emp_vill_ward'] : $loc_emp_vill_ward;	
				if($value_common_selection_2_2 != $loc_emp_vill_ward){
					$l_emp_ps				= '';
					$loc_emp_pin_number		= '';
				}
			}
			if (!empty($value_emp_state) && $value_loc_e_dist_2 != "" && $value_loc_e_subdivision_2 != "" && $value_loc_e_areatype_2 && $value_common_selection_1_2 != "" && $value_common_selection_2_2 != "" ){
				
				$form['clra_amendment_edit']['loc_emp_ps'] 			= array(
																		  '#prefix' 			=> '<div id="clra_ps_replace_2" class="col-lg-4 col-md-6"><label class="select">',
																		  '#suffix' 			=> '</label></div>',
																		  '#title'				=> 'Select Police Station',
																		  '#type' 				=> 'select',
																		  '#attributes'			=> $readonly_loc_emp_ps,
																		  '#options' 			=> common_custom_user_ps_list_new($value_loc_e_dist_2),
																		  '#required' 			=> TRUE,
																		  '#default_value' 		=> !empty($l_emp_ps) ? $l_emp_ps : '',
																	  );
																					  
																					  
				$form['clra_amendment_edit']['loc_emp_pin_number'] 	= array(
																		  '#prefix' 			=> '<div id="clra_pin_replace_2" class="col-lg-4 col-md-6"><label class="input">',
																		  '#suffix' 			=> '</label></div>',
																		  '#title'				=> 'Pin Code',
																		  '#type' 				=> 'textfield',
																		  '#default_value' 		=> !empty($loc_emp_pin_number) ? $loc_emp_pin_number : '',
																		  '#attributes' 		=> $readonly_loc_emp_pin_number,
																		  '#required' 			=> TRUE,
																		  '#element_validate' 	=> array('MISC_PIN_NUMBER')
																	  );	
								
			}
		}else{
			$form['clra_amendment_edit']['address_principal_emp']   = array(
																	  '#prefix' 				=> '<div class="col-md-12" id="add_line_replace"><label class="textarea">',
																	  '#suffix' 				=> '</label></div>',
																	  '#title'					=> 'Address Line1 [ If other Country/State please provide detail address ]',
																	  '#rows'					=> t('2'),
																	  '#type' 					=> 'textarea',
																	  '#attributes'				=> $readonly_address_principal_emp_Textarea,
																	  '#default_value' 			=>!empty($address_principal_emp) ? $address_principal_emp : '',
																	  '#required' 				=> TRUE,
																	  '#element_validate' 		=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																	);	
																	
		}
		$form['clra_amendment_edit']['full_name_manager'] 			= array(
																	  '#prefix' 				=> '</div></div><div class="panel panel-default panel-form"><div class="panel-heading">4. Full name and address of the Manager or Person responsible for the supervision  and control of the Establishment</div><div class="panel-body"><div class="col-md-4"><label class="input">',
																	  '#suffix' 				=> '</label></div>',
																	  '#title'					=> 'Name',
																	  '#type' 					=> 'textfield',
																	  '#required' 				=> TRUE,
																	  '#default_value' 			=> $full_name_manager,
																	  '#attributes'				=> $readonly_full_name_manager,
																	  '#element_validate'		=> array('MISC_TEXTBOX_TEXT_VALIDATE')
																	);
																												
																												
																											
		$form['clra_amendment_edit']['manager_country'] 			= array(
																		 '#prefix' 				=> '<div id="managerCountry" class="col-md-4"><label class="select">',
																		 '#suffix' 				=> '</label></div>',
																		 '#title'				=> 'Select Country',
																		 '#type' 				=> 'select',
																		 '#options' 			=> array('1' => 'India', '2' => 'Others'),
																		 '#attributes'			=> $readonly_manager_country,
																		 '#default_value'		=> !empty($loc_manager_country) ? $loc_manager_country : '',
																		 '#required' 			=> TRUE,
																		 '#ajax'				=> array(
																									'event' 		=> 'change',
																									'effect' 		=> 'fade',			
																									'callback'		=> 'get_ajax_country_manager',
																									'progress'		=> array('type' => 'throbber'),
																									)
																);
		$value_manager_country 	= isset($form_state['values']['manager_country']) ? $form_state['values']['manager_country'] : $loc_manager_country;
		$form['clra_amendment_edit']['manager_state']				= array(
																		'#prefix' 				=> '<div id="clra_manager_state_replace">',
																		'#suffix' 				=> '</div>',
																		);		
				
		$form['clra_amendment_edit']['address_manager']				= array(
																		'#prefix' 				=> '<div id="clra_manager_address_replace">',
																		'#suffix' 				=> '</div>',
																		);
		$form['clra_amendment_edit']['manager_dist']				= array(
																		'#prefix' 				=> '<div id="clra_manager_district_replace">',
																		'#suffix' 				=> '</div>',
																		);
																	
		$form['clra_amendment_edit']['loc_manager_subdv']			= array(
																		'#prefix' 				=> '<div id="clra_subdvsn_replace_3">',
																		'#suffix' 				=> '</div>',
																		);
																	
		$form['clra_amendment_edit']['loc_manager_areatype']		= array(
																		'#prefix' 				=> '<div id="clra_loc_e_areatype_3">',
																		'#suffix' 				=> '</div>',
																		);
																	
		$form['clra_amendment_edit']['manager_name_areatype']		= array(
																		'#prefix' 				=> '<div id="clra_common_1_replace_3">',
																		'#suffix' 				=> '</div>',
																		);
																	
		$form['clra_amendment_edit']['loc_manager_vill_ward']		= array(
																		'#prefix' 				=> '<div id="clra_common_2_replace_3">',
																		'#suffix' 				=> '</div>',
																		);
																	
		$form['clra_amendment_edit']['loc_manager_ps']				= array(
																		'#prefix' 				=> '<div id="clra_ps_replace_3">',
																		'#suffix' 				=> '</div>',
																		);
																	
		$form['clra_amendment_edit']['loc_manager_pin_number']		= array(
																		'#prefix' 				=> '<div id="clra_manager_pin_replace">',
																		'#suffix' 				=> '</div>',
																		);
		if( (!empty($value_manager_country) && $value_manager_country != '') && $value_manager_country ==  1) {
			$db_manager_state_value	= db_query("SELECT COUNT(id) FROM {state_master} WHERE country_id = '".$value_manager_country."'")->fetchObject();
			$form['clra_amendment_edit']['manager_state']			= array(
																		 '#prefix' 				=> '<div id="clra_manager_state_replace" class="col-lg-4 col-md-6"><label class="select">',
																		 '#suffix' 				=> '</label></div>',
																		 '#title'				=> t('Select State'),
																		 '#type' 				=> 'select',
																		 '#options' 			=> get_all_state($value_manager_country),
																		 '#default_value'		=> !empty($loc_manager_state) ? $loc_manager_state : '',
																		 '#required' 			=> TRUE,
																		 '#attributes'			=> $readonly_manager_state,
																		 '#ajax'				=> array(
																										'event'		=> 'change',
																										'effect' 	=> 'fade',
																										'callback'	=> 'get_ajax_country_manager',
																										'progress'	=> array('type' => 'throbber'),
																										)
																		);	
			$value_manager_state	= isset($form_state['values']['manager_state'])? $form_state['values']['manager_state'] : $loc_manager_state;
			if($value_manager_state != $loc_manager_state ){
				$loc_manager_dist 		= '';
				$loc_manager_subdivision= '';
				$loc_manager_areatype	= '';
				$manager_name_areatype	= '';
				$loc_manager_vill_ward	= '';
				$l_manager_ps			= '';
				$loc_manager_pin_number	= '';
			}
			$form['clra_amendment_edit']['address_manager'] 		= array(
																		  '#prefix' 			=> '<div id="clra_manager_address_replace" class="col-lg-8 col-md-8"><label class="input">',
																		  '#suffix' 			=> '</label></div>',
																		  '#title'				=> 'Address Line1',
																		  '#type' 				=> 'textfield',
																		  '#required' 			=> TRUE,
																		  '#default_value' 		=> !empty($manager_address) ? $manager_address : '',
																		  '#attributes'			=> $readonly_address_manager,
																		  '#element_validate' 	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																		);
			if(($value_manager_country != '' && (!empty($value_manager_state) || $value_manager_state != '' )  )){
		
				$db_manager_district_value	= db_query("SELECT COUNT(id) FROM {district_master} WHERE state_id = '".$value_manager_state."'")->fetchObject();	
			
				if(!empty($db_manager_district_value) && ($db_manager_district_value->count>0) ) {
					$form['clra_amendment_edit']['manager_dist']	= array(
																		'#prefix' 				=> '<div id="clra_manager_district_replace" class="col-lg-4 col-md-6"><label class="select">',
																		'#suffix' 				=> '</label></div>',
																		'#title'				=> t('Select District'),
																		'#type' 				=> 'select',
																		'#options' 				=> get_district_by_id($value_manager_state),
																		'#default_value' 		=> !empty($loc_manager_dist) ? $loc_manager_dist : '',
																		'#required' 			=> TRUE,
																		'#attributes'			=> $readonly_manager_dist,
																		'#ajax'					=> array(
																										'event'		=> 'change',
																										'effect' 	=> 'fade',
																										'callback'	=> 'get_ajax_country_manager',
																										'progress'	=> array('type' => 'throbber'),
																										)
																		);	
					
					
				
					$value_loc_e_dist_3 = 	isset($form_state['values']['manager_dist']) ? $form_state['values']['manager_dist'] : $loc_manager_dist;
					if($value_loc_e_dist_3 != $loc_manager_dist ){
						$loc_manager_subdivision= '';
						$loc_manager_areatype	= '';
						$manager_name_areatype	= '';
						$loc_manager_vill_ward	= '';
						$l_manager_ps			= '';
						$loc_manager_pin_number	= '';
					}
				}
			}
			if($value_loc_e_dist_3 != ''  && !empty($value_manager_state)){
				
				$db_manager_subdivision_value = db_query("SELECT COUNT(id) as subdv_total_manager  FROM {sub_division} WHERE district_code = '".$value_loc_e_dist_3."'")->fetchObject();
				
				if(!empty($db_manager_subdivision_value) && $db_manager_subdivision_value != '' && $db_manager_subdivision_value->subdv_total_manager > 0) {
				
					$form['clra_amendment_edit']['loc_manager_subdv'] = array(
																		  '#prefix' 			=> '<div id="clra_subdvsn_replace_3" class="col-lg-4 col-md-6"><label class="select">',
																		  '#suffix' 			=> '</label></div>',
																		  '#title'				=> 'Select Subdivision',
																		  '#type' 				=> 'select',
																		  '#options' 			=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist_3),
																		  '#default_value' 		=> !empty($loc_manager_subdivision) ? $loc_manager_subdivision	: '',
																		  '#attributes'			=> $readonly_loc_manager_subdv,
																		  '#required' 			=> TRUE,
																		  '#ajax'				=> array(
																										'event'		=> 'change',
																										'effect' 	=> 'fade',
																										'callback'	=> 'get_ajax_country_manager',
																										'progress'	=> array('type' => 'throbber'),
																										  ),
															);
			
			
					$value_loc_e_subdivision_3 				= isset($form_state['values']['loc_manager_subdv']) ? $form_state['values']['loc_manager_subdv'] : $loc_manager_subdivision;
					if($value_loc_e_subdivision_3 != $loc_manager_subdivision ){
						$loc_manager_areatype	= '';
						$manager_name_areatype	= '';
						$loc_manager_vill_ward	= '';
						$l_manager_ps			= '';
						$loc_manager_pin_number	= '';
					}
				}
			}
			if($value_loc_e_dist_3 != "" && !empty($value_manager_state) && $value_loc_e_subdivision_3 != ""){
						
			  $db_block_mun_manager_value= db_query("SELECT COUNT(id) as areatype_manager_total FROM {block_mun_master} WHERE distict_code = '".$value_loc_e_dist_3."' and sub_division_code  = '".$value_loc_e_subdivision_3."'")->fetchObject();
						
			  if(!empty($db_block_mun_manager_value) &&  $db_block_mun_manager_value != '' && $db_block_mun_manager_value->areatype_manager_total > 0){
								
				$form['clra_amendment_edit']['loc_manager_areatype'] = array(
																		  '#prefix' 				=> '<div id="clra_loc_e_areatype_3" class="col-lg-4 col-md-6"><label class="select">',
																		  '#suffix' 				=> '</label></div>',
																		  '#title'					=> 'Select Block / Municipality / Corporation / SEZ / Notified Area',
																		  '#type' 					=> 'select',
																		  '#options' 				=> common_custom_user_areatype_list_new($value_loc_e_dist_3, $value_loc_e_subdivision_3),
																		  '#default_value' 			=> !empty($loc_manager_areatype) ? $loc_manager_areatype : '',
																		  '#attributes'				=> $readonly_loc_manager_areatype,
																		  '#required' 				=> TRUE,
																		  '#ajax'					=> array(
																										'event'		=> 'change',
																										'effect' 	=> 'fade',
																										'callback'	=> 'get_ajax_country_manager',
																										'progress'	=> array('type' => 'throbber'),
																									  ),
																			);	
																										
				$value_loc_e_areatype_3 = isset($form_state['values']['loc_manager_areatype']) ? $form_state['values']['loc_manager_areatype'] : $loc_manager_areatype;
				if($value_loc_e_subdivision_3 != $loc_manager_subdivision && $value_loc_e_areatype_3 != $loc_manager_areatype){
					$manager_name_areatype	= '';
					$loc_manager_vill_ward	= '';
					$l_manager_ps			= '';
					$loc_manager_pin_number	= '';
				}
			  }
		   }
		   if ($value_loc_e_dist_3 != "" && $value_loc_e_subdivision_3 != "" && $value_loc_e_areatype_3 != "") {
						
				if ( $value_loc_e_areatype_3 == 'B' ){
					$areatype_name	= 'Block';
				}elseif( $value_loc_e_areatype_3 == 'M'){
					$areatype_name	= 'Municipality';
				}elseif( $value_loc_e_areatype_3 == 'C'){
					$areatype_name	= 'Corporation';
				}elseif( $value_loc_e_areatype_3 == 'S'){
					$areatype_name	= 'SEZ';
				}elseif($value_loc_e_areatype_3 == 'N'){
					$areatype_name	= 'Notified Area';
				}
				
				$form['clra_amendment_edit']['manager_name_areatype'] = array(
																		  '#prefix' 				=> '<div id="clra_common_1_replace_3" class="col-lg-4 col-md-6"><label class="select">',
																		  '#suffix' 				=> '</label></div>',
																		  '#title'					=> 'Select '.$areatype_name,
																		  '#type' 					=> 'select',
																		  '#options' 				=> common_custom_user_block_list_new($value_loc_e_subdivision_3, $value_loc_e_areatype_3),
																		  '#default_value' 			=> !empty($manager_name_areatype) ? $manager_name_areatype : '',
																		  '#required' 				=> TRUE,
																		  '#attributes'				=> $readonly_loc_manager_areatype_code,
																		  '#ajax'					=> array(
																										'event' 	=> 'change',
																										'effect' 	=> 'fade',			
																										'callback'	=> 'get_ajax_country_manager',
																										'progress'	=> array('type' => 'throbber'),
																									  )
																		);
																									
				$value_common_selection_1_3 = isset($form_state['values']['manager_name_areatype']) ? $form_state['values']['manager_name_areatype'] : $manager_name_areatype;
				if($value_common_selection_1_3 != $manager_name_areatype){
					$loc_manager_vill_ward		= '';
					$l_manager_ps				= '';
					$loc_manager_pin_number		= '';
				}
			}
			if($value_loc_e_dist_3 != "" && $value_loc_e_subdivision_3 != "" && $value_loc_e_areatype_3 != ""  && $value_common_selection_1_3 != "" && !empty($value_manager_state) ){
					
				if ( $value_loc_e_areatype_3 == 'B' ){
					$area_name		= 'Gram Panchayat';
					$area_code 		= 'V';
				}elseif( $value_loc_e_areatype_3 == 'M'){
					$area_name		= 'Ward';
					$area_code 		= 'W';
				}elseif( $value_loc_e_areatype_3 == 'C'){
					$area_name		= 'Ward';
					$area_code 		= 'C';
				}elseif( $value_loc_e_areatype_3 == 'S'){
					$area_name		= 'Sector';
					$area_code 		= 'S';
				}elseif($value_loc_e_areatype_3 == 'N'){
					$area_name		= 'Notified Area';
					$area_code 		= 'N';
				}
						
				$form['clra_amendment_edit']['loc_manager_vill_ward']	= array(
																		  '#prefix' 				=> '<div id="clra_common_2_replace_3" class="col-lg-4 col-md-6"><label class="select">',
																		  '#suffix' 				=> '</label></div>',
																		  '#title'					=> 'Select '.$area_name,
																		  '#type' 					=> 'select',
																		  '#options'				=> common_custom_user_villward_list_new($value_common_selection_1_3,$area_code),
																		  '#default_value' 			=> !empty($loc_manager_vill_ward) ? $loc_manager_vill_ward : '',
																		  '#required' 				=> TRUE,
																		  '#attributes'				=> $readonly_loc_manager_vill_ward,
																		  '#ajax'					=> array(
																										'event' 	=> 'change',
																										'effect' 	=> 'fade',			
																										'callback'	=> 'get_ajax_country_manager',
																										'progress'	=> array('type' => 'throbber'),
																									  )
																		);	
																									
				$value_common_selection_2_3 = isset($form_state['values']['loc_manager_vill_ward']) ? $form_state['values']['loc_manager_vill_ward'] : $loc_manager_vill_ward;	
				if($value_common_selection_2_3 != $loc_manager_vill_ward){
					$l_manager_ps				= '';
					$loc_manager_pin_number		= '';
				}
			}
			if (!empty($value_manager_state) && $value_loc_e_dist_3 != "" && $value_loc_e_subdivision_3 != "" && $value_loc_e_areatype_3 && $value_common_selection_1_3 != "" && $value_common_selection_2_3 != "" ){
					
				$form['clra_amendment_edit']['loc_manager_ps'] 			= array(
																		  '#prefix' 				=> '<div id="clra_ps_replace_3" class="col-lg-4 col-md-6"><label class="select">',
																		  '#suffix' 				=> '</label></div>',
																		  '#title'					=> 'Select Police Station',
																		  '#type' 					=> 'select',
																		  '#id'						=> 'loc_e_ps_id_3',
																		  '#options' 				=> common_custom_user_ps_list_new($value_loc_e_dist_3),
																		  '#default_value' 			=> !empty($l_manager_ps) ? $l_manager_ps : '',
																		  '#attributes' 			=> $readonly_loc_manager_ps,
																		  '#required' 				=> TRUE
																	  );
																							  
				$form['clra_amendment_edit']['loc_manager_pin_number']   = array(
																			'#prefix' 				=> '<div id="clra_manager_pin_replace" class="col-lg-4 col-md-6"><label class="input">',
																			'#suffix' 				=> '</label></div>',
																			'#title'				=> 'Pin Code',
																			'#type' 				=> 'textfield',
																			'#default_value' 		=>!empty($loc_manager_pin_number) ? $loc_manager_pin_number : '',
																			'#attributes' 			=> $readonly_loc_manager_pin_number,
																			'#element_validate' 	=> array('MISC_PIN_NUMBER'),
																			'#required' 			=> TRUE
																			);	
						
						
			}
		}else{
			$form['clra_amendment_edit']['address_manager'] 		= array(
																		  '#prefix' 			=> '<div id="clra_manager_address_replace" class="col-md-12"><label class="textarea">',
																		  '#suffix' 			=> '</label></div>',
																		  '#title'				=> 'Address Line1 [ If other Country/State please provide detail address ]',
																		  '#rows'				=> t('2'),	
																		  '#type' 				=> 'textarea',
																		  '#attributes' 		=> $readonly_address_manager_Textarea,
																		  '#default_value' 		=>!empty($manager_address) ? $manager_address : '',
																		  '#required' 			=> TRUE,
																		  '#element_validate' 	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																		);
		}
		$form['clra_amendment_edit']['e_nature_of_work'] 			= array(
																		 '#prefix' 				=> '</div></div><div class="panel panel-default panel-form"><div class="panel-heading">5. Nature of work carried on in the establishment</div><div class="panel-body"><div class="col-md-12"><label class="select">',
																		 '#suffix' 				=> '</label></div>',
																		 '#title'				=> 'Select Nature of Work',
																		 '#description'			=> '<span style="color:#f00;">Note: If multiple Nature of work to be selected, press Ctrl and select the options.<br/><br/></span>',
																		 '#type' 				=> 'select',
																		 '#id' 					=> 'e_nature_of_work_id',
																		 '#options' 			=> contractor_works_multiSelect_options(),
																		 '#multiple' 			=> TRUE,
																		 '#default_value' 		=> $resultChunk,
																		 '#attributes' 			=> $readonly_e_nature_of_work,
																		 '#required' 			=> TRUE
																	);	
		$form['clra_amendment_edit']['other_nature_of_work_value'] 	= array(
																		 '#prefix' 				=> '<div class="col-md-12" id ="edit_other_nature_work_id"><label class="input">',
																		 '#suffix' 				=> '</label></div>',
																		 '#title'				=> 'Other Option for Nature of Work',
																		 '#type' 				=> 'textfield',
																		 '#id' 					=> 'other_value_nature_of_work_id',
																		 '#default_value' 		=>  !empty($other_nature_wrk) ? $other_nature_wrk : '',
																		 '#attributes' 			=> $readonly_other_nature_of_work_value,
																		// '#element_validate'	=> array('MISC_OTHERNATURE_TEXTBOX_TEXT_VALIDATE')
																		);
																								
		$form['clra_amendment_edit']['max_num_wrkmen'] 				= array(
																		  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																		  '#suffix' 			=> '</label></div>',
																		  '#title'				=> '(a) Maximum Number of Workmen Employed Directly on any day in the Establishment',
																		  '#type' 				=> 'textfield',
																		  '#default_value' 		=> ($max_num_wrkmen != '') ? $max_num_wrkmen : '',
																		  '#attributes' 		=> $readonly_max_num_wrkmen,
																		  '#required' 			=> TRUE,
																		  '#element_validate' 	=> array('MISC_TEXTBOX_NUMERIC_VALIDATE'),	
																		);
																					
		$form['clra_amendment_edit']['e_num_of_workmen_per_or_reg'] = array(
																		  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																		  '#suffix' 			=> '</label></div>',
																		  '#required' 			=> TRUE,
																		  '#title'				=> '(b) Number of Workmen Engaged as Permanent/Regular Workmen',
																		  '#type' 				=> 'textfield',
																		  '#default_value' 		=> ($e_num_of_workmen_per_or_reg!='') ? $e_num_of_workmen_per_or_reg : '', 
																		  '#attributes' 		=> $readonly_e_num_of_workmen_per_or_reg,
																		  '#element_validate' 	=> array('MISC_TEXTBOX_NUMERIC_VALIDATE'),
																		);
																				
		$form['clra_amendment_edit']['e_num_of_workmen_temp_or_reg'] = array(
																		  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																		  '#suffix' 			=> '</label></div>',
																		  '#title'				=> '(c) Number of Workmen Engaged as Temporary/Regular Workmen',
																		  '#type' 				=> 'textfield',
																		  '#default_value' 		=> ($e_num_of_workmen_temp_or_reg!='') ? $e_num_of_workmen_temp_or_reg : '',
																		  '#required' 			=> TRUE,
																		  '#attributes' 		=> $readonly_e_num_of_workmen_temp_or_reg,
																		  '#element_validate' 	=> array('MISC_TEXTBOX_NUMERIC_VALIDATE'),
																		);
		$form['clra_amendment_edit']['workmen_if_same_similar_kind_of_work']= array(
																			'#prefix' 			=> '<div class="col-md-12 inline-group">',
																			'#suffix' 			=> '</div>',
																			'#type' 			=> 'radios',
																			'#title' 			=> '(d) Whether the Workmen employed/intended to be Employment by the Contractor Perform the same or similar kind of work as the Workmen employed directly by the Principal Employer (if yes, please give here information as detailed below)',
																			'#options' 			=>array(0=> t('No'), 1=> t('Yes')),
																			'#required' 		=> TRUE, 
																			'#default_value'	=> isset($workmen_if_same_similar_kind_of_work) ? $workmen_if_same_similar_kind_of_work : 0,
																			'#attributes' 		=> $readonly_workmen_if_same_similar_kind_of_work,
																			);	
								
		$form['clra_amendment_edit']['con_lab_job_desc'] 			= array(
																		  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="textarea">',
																		  '#suffix' 			=> '</label></div>',
																		  '#title'				=> 'i. A complete job description of the contract labour',
																		  '#type' 				=> 'textarea',
																		  '#rows'				=> t('4'),	
																		  '#required' 			=> TRUE,
																		  '#default_value' 		=> !empty($con_lab_job_desc) ? $con_lab_job_desc : '',
																		  '#attributes'			=> $readonly_con_lab_job_desc,
																		  '#element_validate' 	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																		);
		$form['clra_amendment_edit']['con_lab_wage_rate_other_benefits'] = array(
																		  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																		  '#suffix' 			=> '</label></div>',
																		  '#title'				=> 'ii. Wage rates and other cash benefits paid/to be paid',
																		  '#type' 				=> 'textfield',
																		  '#default_value' 		=> !empty($con_lab_wage_rate_other_benefits) ? $con_lab_wage_rate_other_benefits : '',
																		  '#attributes'			=> $readonly_con_lab_wage_rate_other_benefits,
																		  '#required' 			=> TRUE,
																		  '#element_validate'   => array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																	);
													
		$form['clra_amendment_edit']['con_lab_cat_desig_nom'] 		= array(
																		  '#prefix' 			=> '<div class="col-lg-4 col-md-6"><label class="input">',
																		  '#suffix' 			=> '</label></div>',
																		  '#title'				=> 'iii. Category/designation/nomenclature of the job',
																		  '#type' 				=> 'textfield',
																		  '#required' 			=> TRUE,
																		  '#default_value'		=> !empty($con_lab_cat_desig_nom) ? $con_lab_cat_desig_nom : '',
																		  '#attributes'			=> $readonly_con_lab_cat_desig_nom,
																		  '#element_validate' 	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																	);
		$form['clra_amendment_edit']['e_settlement_award_judgement_min_wage'] = array(
																		  '#prefix' 			=> '<div class="col-md-8"><label class="input">',
																		  '#suffix' 			=> '</label></div></div></div></div>',
																		  '#title'				=> '(e) Settlement or award or judgement or minimum wages ( if any applicable in the establishment )',
																		  '#type' 				=> 'textfield',
																		  '#required' 			=> TRUE,
																		  '#default_value' 		=> !empty($e_settlement_award_judgement_min_wage) ? $e_settlement_award_judgement_min_wage : '',
																		  '#attributes'			=> $readonly_e_settlement_award_judgement_min_wage,
																		  '#element_validate' 	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																			);
		$form['clra_amendment_edit']['e_any_day_max_num_of_workmen'] = array(
																		  '#prefix' 			=> '<div class="panel panel-default panel-form"><div class="panel-heading">6. Maximum number of contract labour to be employed on any day through each contractor</div><div class="panel-body"><div class="col-md-8"><label class="input">',
																		  '#suffix' 			=> '</label></div></div></div>',
																		  '#title'				=> 'Maximum number of contract labour to be employed on any day through each contractor',
																		  '#type' 				=> 'textfield',
																		  '#required' 			=> TRUE,
																		  '#default_value' 		=> ($e_any_day_max_num_of_workmen!='') ? $e_any_day_max_num_of_workmen : '',
																		  '#attributes' 		=> $readonly_e_any_day_max_num_of_workmen,
																		  '#element_validate' 	=> array('MISC_TEXTBOX_NUMERIC_VALIDATE')
																		);
		if ( $trade_license_file != "" ) {
					
			$trade_license_link 	= l(t('View Document &nbsp; <img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/lc-pdf.png">'),''.$GLOBALS['base_url'].'/sites/default/files/upload/trade_license/'.$trade_license_file, array('html' => TRUE,'attributes'=> array('target'=>'_blank'), 'attributes'=> array('class'=>' ')));
	
		}else{
				$trade_license_link = '';
			
		}
		
		$upload_value_1 			= array( 'trade_license' => t('Trade License&nbsp;&nbsp;&nbsp;&nbsp;'.$trade_license_link ));
		
		/** Article of Association **/
		
		if ( $article_of_assoc_file != "" ){
			
			$article_of_assoc_link = l(t('View Document &nbsp; <img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/lc-pdf.png">'),''.$GLOBALS['base_url'].'/sites/default/files/upload/article_of_assoc/'.$article_of_assoc_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
		}else{
			$article_of_assoc_link = '';
		}
		
		$upload_value_2 = array( 'article_of_assoc' => t('Articles of Association and Memorandum of Association/Partnership Deed&nbsp;&nbsp;&nbsp;&nbsp;'.$article_of_assoc_link));
		
		
		/**  Memorandum of Certificate **/
		
		if ( $memorandum_of_cert_file != "" ){
			
			$memorandum_of_cert_link = l(t('View Document &nbsp; <img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/lc-pdf.png">'),''.$GLOBALS['base_url'].'/sites/default/files/upload/memorandum_of_cert/'.$memorandum_of_cert_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
		}else{
			$memorandum_of_cert_link = '';
		}
		
		$upload_value_3 = array( 'memorandum_of_cert' => t('Any other document in support of correctness of the particulars mentioned in the application if required&nbsp;&nbsp;&nbsp;&nbsp;'.$memorandum_of_cert_link));
		
				
		/**   Partnership Deed **/
		
		if ( $partnership_deed_file != "" ){
			
			$partnership_deed_link = l(t('View Document &nbsp; <img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/lc-pdf.png">'),''.$GLOBALS['base_url'].'/sites/default/files/upload/partnership_deed/'.$partnership_deed_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
		}else{
			$partnership_deed_link = '';
		}
		
		$upload_value_4 = array( 'partnership_deed' => t('Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc.&nbsp;&nbsp;&nbsp;&nbsp;'.$partnership_deed_link));
		
		
		
		/**   Factory License  **/
		
		if ( $factory_license_file != "" ){
			
			$factory_license_link = l(t('View Document &nbsp; <img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/lc-pdf.png">'),''.$GLOBALS['base_url'].'/sites/default/files/upload/FactoryLicense/'.$factory_license_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
		}else{
			$factory_license_link = '';
		}
																  
		$upload_value_5 = array( 'factory_license' => t('Factory License if any&nbsp;&nbsp;&nbsp;&nbsp;'.$factory_license_link));
		
		$form['clra_amendment_edit']['upload_check_in_1'] 			= array(
																		  '#prefix' 			=> '<div class="panel panel-default panel-form"><div class="panel-heading">Upload Supporting Documents</div><div class="panel-body min-height-0"><section class="col-md-12"><label class="checkboxe">',
																		  '#suffix' 			=> '</label></section>',																		 
																		  '#type' 				=> 'checkboxes',
																		  '#options' 			=> $upload_value_1,
																		  '#attributes'			=> $readonly_trade_license
																		);
		
							
		$form['clra_amendment_edit']['upload_check_in_2'] 			= array(
																		  '#prefix' 			=> '<section class="col-md-12"><label class="checkboxe">',
																		  '#suffix' 			=> '</label></section>',
																		  '#type' 				=> 'checkboxes',
																		  '#options' 			=> $upload_value_2,
																		  '#attributes'			=> $readonly_article_of_assoc
																		);
												
		$form['clra_amendment_edit']['upload_check_in_3'] 			= array(
																		  '#prefix' 			=> '<section class="col-md-12"><label class="checkboxe">',
																		  '#suffix' 			=> '</label></section>',
																		  '#type' 				=> 'checkboxes',
																		  '#options' 			=> $upload_value_3,
																		  '#attributes'			=> $readonly_memorandum_of_cert
																		);
												
		$form['clra_amendment_edit']['upload_check_in_4'] 			= array(
																		  '#prefix' 			=> '<section class="col-md-12"><label class="checkboxe">',
																		  '#suffix' 			=> '</label></section>',
																		  '#type' 				=> 'checkboxes',
																		  '#options' 			=> $upload_value_4,
																		  '#attributes'			=> $readonly_partnership_deed,
																		);
																		
		$form['clra_amendment_edit']['upload_check_in_5'] 			= array(
																		  '#prefix' 			=> '<section class="col-md-12"><label class="checkboxe">',
																		  '#suffix' 			=> '</label></section>',
																		  '#type' 				=> 'checkboxes',
																		  '#options' 			=> $upload_value_5,
																		  '#attributes'			=> $readonly_factory_license,
																		);
		$form['clra_amendment_edit']['row1'] 						= array(
																		 '#markup' 				=> '<section class="col-md-6" id="trade_license_div">',
																		  ); 		
		$form['clra_amendment_edit']['trade_license'] 				= array(
																		'#type' 				=> 'managed_file',
																		'#title'				=> '1. Upload Trade License',
																		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																		'#upload_location' 		=> 'public://upload/trade_license/',
																		'#process' 				=> array('import_my_file_element_process'),
																		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																		'#prefix' 				=> '<label class="input input-file">',
																		'#suffix' 				=> '</label>'
																		);
		$form['clra_amendment_edit']['row2'] 						= array(
																		  '#markup' 			=> '</section><section class="col-md-6" id="article_of_assoc_div">',
																		  ); 
							
	    $form['clra_amendment_edit']['article_of_assoc'] 			= array(
																		'#type' 				=> 'managed_file',
																		'#title'				=>'2. Upload Articles of Association and Memorandum of Association/Partnership Deed',
																		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																		'#upload_location' 		=> 'public://upload/article_of_assoc/',
																		'#process' 				=> array('import_my_file_element_process'),
																		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																		'#prefix' 				=> '<label class="input input-file">',
																		'#suffix' 				=> '</label>',
																		);
		$form['clra_amendment_edit']['row3'] 						= array(
																		  '#markup' 			=> '</section><section class="col-md-6" id="memorandum_of_cert_div">',
																		 );
	
		$form['clra_amendment_edit']['memorandum_of_cert'] 			= array(
																		'#type' 				=> 'managed_file',
																		'#title'				=>'3. Upload any other document in support of correctness of the particulars mentioned in the application if required',
																		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																		'#upload_location' 		=> 'public://upload/memorandum_of_cert/',
																		'#process' 				=> array('import_my_file_element_process'),
																		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																		'#prefix' 				=> '<label class="input-file">',
																		'#suffix' 				=> '</label>'
																		);
		$form['clra_amendment_edit']['row4'] 						=  array(
																		  '#markup' 			=> '</section><section class="col-md-6" id="partnership_deed_div">',
																		  ); 
		
		$form['clra_amendment_edit']['partnership_deed'] 			= array(
																		'#type' 				=> 'managed_file',
																		'#title'				=>'4. Upload other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc.',
																		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																		'#upload_location' 		=> 'public://upload/partnership_deed/',
																		'#process' 				=> array('import_my_file_element_process'),
																		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																		'#field_prefix' 		=> '<label class="input-file">',
																		'#field_suffix' 		=> '</label>'
																		);
		$form['clra_amendment_edit']['row5'] 						=  array(
																		  '#markup' 			=> '</section><section class="col-md-6" id="factory_license_div">',
																		  ); 
																		  
		$form['clra_amendment_edit']['factory_license'] 			= array(
																		'#type' 				=> 'managed_file',
																		'#title'				=>'5. Upload Factory License',
																		'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																		'#upload_location' 		=> 'public://upload/FactoryLicense/',
																		'#process' 				=> array('import_my_file_element_process'),
																		'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																		'#field_prefix' 		=> '<label class="input-file">',
																		'#field_suffix' 		=> '</label>'
																		);
		$form['clra_amendment_edit']['row6'] 						= array(
																		'#markup' 				=> '</section></div></div></div>'
																		);
																					
		
		$form['clra_amendment_edit']['submit'] 						= array(														 
																		 '#type' 				=> 'submit',
																		 '#attributes' 			=> array('class' => array('btn btn-primary pull-right form-submit')),
																		 '#value' 				=> 'SAVE'
																		);
		/*$form['clra_amendment_edit']['markup1']						= array(
																		'#type' 				=> 'markup',
																		'#markup' 				=> l('Reset Amendment Fields', 'apply-clra-reg-amendment/'.encryption_decryption_fun('encrypt', $amendment_parent_id), array('attributes' => array('html' => TRUE, 'class' => 'link-btn')))																			
																		);
		$form['clra_amendment_edit']['row7'] 						= array(
																			'#markup' 			=> '</div>'
																			);*/											   
		return $form;
														   


	}else{

		
		drupal_set_message('You are not authorized to access this page','error');
	}

}


function get_ajax_location_amendment($form, $form_state){
	$commands 	= array();
	$commands[] = ajax_command_replace('#clra_subdvsn_replace', drupal_render($form['clra_amendment_edit']['loc_e_subdivision']));
	$commands[] = ajax_command_replace('#clra_loc_e_areatype', drupal_render($form['clra_amendment_edit']['loc_e_areatype']));
	$commands[] = ajax_command_replace('#clra_common_1_replace', drupal_render($form['clra_amendment_edit']['name_areatype']));
	$commands[] = ajax_command_replace('#clra_common_2_replace', drupal_render($form['clra_amendment_edit']['loc_e_vill_ward']));
	$commands[] = ajax_command_replace('#clra_ps_replace', drupal_render($form['clra_amendment_edit']['loc_e_ps']));
  	return array('#type' => 'ajax', '#commands' => $commands);
}

function get_ajax_postal_amendment($form, $form_state){
	$commands 	= array();
	$commands[] = ajax_command_replace('#clra_subdvsn_replace_1', drupal_render($form['clra_amendment_edit']['e_postal_subdivision']));
	$commands[] = ajax_command_replace('#clra_loc_e_areatype_1', drupal_render($form['clra_amendment_edit']['e_postal_areatype']));
	$commands[] = ajax_command_replace('#clra_common_1_replace_1', drupal_render($form['clra_amendment_edit']['e_postal_name_areatype']));
	$commands[] = ajax_command_replace('#clra_common_2_replace_1', drupal_render($form['clra_amendment_edit']['e_postal_vill_ward']));
	$commands[] = ajax_command_replace('#clra_ps_replace_1', drupal_render($form['clra_amendment_edit']['e_postal_ps']));
  	return array('#type' => 'ajax', '#commands' => $commands);
}

function get_ajax_pe_country ($form, $form_state){
	$commands 	= array();
	$commands[] = ajax_command_replace('#countrydiv', drupal_render($form['clra_amendment_edit']['emp_country']));
	$commands[] = ajax_command_replace('#clra_emp_state_replace', drupal_render($form['clra_amendment_edit']['emp_state']));
	$commands[] = ajax_command_replace('#add_line_replace', drupal_render($form['clra_amendment_edit']['address_principal_emp']));
	$commands[] = ajax_command_replace('#clra_emp_district_replace', drupal_render($form['clra_amendment_edit']['emp_dist']));
	$commands[] = ajax_command_replace('#clra_subdvsn_replace_2', drupal_render($form['clra_amendment_edit']['loc_emp_subdv']));
	$commands[] = ajax_command_replace('#clra_loc_e_areatype_2', drupal_render($form['clra_amendment_edit']['loc_emp_areatype']));
	$commands[] = ajax_command_replace('#clra_common_1_replace_2', drupal_render($form['clra_amendment_edit']['emp_name_areatype']));
	$commands[] = ajax_command_replace('#clra_common_2_replace_2', drupal_render($form['clra_amendment_edit']['loc_emp_vill_ward']));
	$commands[] = ajax_command_replace('#clra_ps_replace_2', drupal_render($form['clra_amendment_edit']['loc_emp_ps']));
	$commands[] = ajax_command_replace('#clra_pin_replace_2', drupal_render($form['clra_amendment_edit']['loc_emp_pin_number']));
	return array('#type' => 'ajax', '#commands' => $commands);
}


function get_ajax_country_manager ($form, $form_state){
	$commands[] = ajax_command_replace('#managerCountry', drupal_render($form['clra_amendment_edit']['manager_country']));
	$commands[] = ajax_command_replace('#clra_manager_state_replace', drupal_render($form['clra_amendment_edit']['manager_state']));
	$commands[] = ajax_command_replace('#clra_manager_address_replace', drupal_render($form['clra_amendment_edit']['address_manager']));
	$commands[] = ajax_command_replace('#clra_manager_district_replace', drupal_render($form['clra_amendment_edit']['manager_dist']));
	$commands[] = ajax_command_replace('#clra_subdvsn_replace_3', drupal_render($form['clra_amendment_edit']['loc_manager_subdv']));
	$commands[] = ajax_command_replace('#clra_loc_e_areatype_3', drupal_render($form['clra_amendment_edit']['loc_manager_areatype']));
	$commands[] = ajax_command_replace('#clra_common_1_replace_3', drupal_render($form['clra_amendment_edit']['manager_name_areatype']));
	$commands[] = ajax_command_replace('#clra_common_2_replace_3', drupal_render($form['clra_amendment_edit']['loc_manager_vill_ward']));
	$commands[] = ajax_command_replace('#clra_ps_replace_3', drupal_render($form['clra_amendment_edit']['loc_manager_ps']));
	$commands[] = ajax_command_replace('#clra_manager_pin_replace', drupal_render($form['clra_amendment_edit']['loc_manager_pin_number']));
	return array('#type' => 'ajax', '#commands' => $commands);
}


function amendment_form_clra_registration_submit($form, &$form_state){ 

	global $base_root, $base_path, $user;
	$finalAmendedFields = $finalNotAmendedFields = $trimFinalAmendedFields= '';
		
		$user_id	= $user->uid;
		$val 		= $form_state['values'];   
		$increment 	= 0;
		
		/* Upload For Trade License */
		
		$arr_1			= array();
		$trade_license 	= file_load($val['trade_license']);
		
		if( $trade_license != "" ){
			$arr_1 = explode("/", $trade_license->uri);
			$trade_license->status = FILE_STATUS_PERMANENT;
			file_save($trade_license);
			$trade_license_file = $arr_1[4];
		}
		
		
		/* Upload For Article of Association */
		
		$arr_2			  = array();
		$article_of_assoc = file_load($val['article_of_assoc']);
		
		if( $article_of_assoc != "" ){
			$arr_2 = explode("/", $article_of_assoc->uri);
			$article_of_assoc->status = FILE_STATUS_PERMANENT;
			file_save($article_of_assoc);
			$article_of_assoc_file = $arr_2[4];
		}
		
		/* Upload For Memorandum of Certificate */
		
		$arr_3				= array();
		$memorandum_of_cert = file_load($val['memorandum_of_cert']);
		
		if( $memorandum_of_cert != "" ){
			$arr_3 = explode("/", $memorandum_of_cert->uri);
			$memorandum_of_cert->status = FILE_STATUS_PERMANENT;
			file_save($memorandum_of_cert);
			$memorandum_of_cert_file = $arr_3[4];
		}
		
		/* Upload For Partnership Deed */
		
		$arr_4			  = array();
		$partnership_deed = file_load($val['partnership_deed']);
		
		if( $partnership_deed != "" ){
			$arr_4 = explode("/", $partnership_deed->uri);
			$partnership_deed->status = FILE_STATUS_PERMANENT;
			file_save($partnership_deed);
			$partnership_deed_file = $arr_4[4];
		}
		
		
		/* Upload For Factory License */
		
		$arr_5			 = array();
		$factory_license = file_load($val['factory_license']);
		
		if( $factory_license != "" ){
			$arr_5 = explode("/", $factory_license->uri);
			$factory_license->status = FILE_STATUS_PERMANENT;
			file_save($factory_license);
			$factory_license_file = $arr_5[4];
		}	
		
		$new_application_id					= encryption_decryption_fun('decrypt', $val['application_id']);
		$act_id 							= encryption_decryption_fun('decrypt', $val['act_id']);
		$amended_indentification_number		= encryption_decryption_fun('decrypt', $val['identification_number']);
		$amendment_parent_id				= encryption_decryption_fun('decrypt', $val['amendment_parent_id']);
		
		
		$fetch_reg_det 						= db_query("select * from l_clra_registration_master where user_id =".$user->uid." and status = 'I' order by id desc limit 1");
		$obj3 								= $fetch_reg_det->fetchAssoc();
		
		$registration_number					= $obj3['registration_number'];
		$clra_qr_code							= $obj3['clra_qr_code'];
		$registration_date						= $obj3['registration_date'];
		//$certificates_fid						= $obj3['certificates_fid'];
				
		$e_name 								= $val['e_name'];
		$est_type 								= $val['est_type'];
		$loc_e_name 							= $val['loc_e_name'];		
		$loc_e_dist 							= $val['loc_e_dist'];
		$loc_e_subdivision 						= $val['loc_e_subdivision'];
		$loc_e_areatype 						= $val['loc_e_areatype'];
		$loc_e_areatype_code 					= $val['name_areatype'];
		$loc_e_vill_ward						= $val['loc_e_vill_ward'];
		$l_e_ps									= $val['loc_e_ps'];
		$loc_e_pin_number						= $val['loc_e_pin_number'];	
			
		$e_postal_address 						= $val['e_postal_address'];		
		$e_postal_dist 							= $val['e_postal_dist'];
		$e_postal_subdivision 					= $val['e_postal_subdivision'];
		$e_postal_areatype 						= $val['e_postal_areatype'];
		$e_postal_areatype_code 				= $val['e_postal_name_areatype'];
		$e_postal_vill_ward						= $val['e_postal_vill_ward'];
		$e_postal_ps							= $val['e_postal_ps'];
		$e_postal_pin_number					= $val['e_postal_pin_number'];	
			
		$full_name_principal_emp 				= $val['full_name_principal_emp'];
		$address_principal_emp 					= $val['address_principal_emp'];
		$loc_emp_country						= $val['emp_country'];
		$loc_emp_state							= $val['emp_state'];		
		$loc_emp_dist 							= $val['emp_dist'];
		$loc_emp_subdivision 					= $val['loc_emp_subdv'];
		$loc_emp_areatype 						= $val['loc_emp_areatype'];
		$loc_emp_areatype_code 					= $val['emp_name_areatype'];
		$loc_emp_vill_ward						= $val['loc_emp_vill_ward'];
		$l_emp_ps								= $val['loc_emp_ps'];
		$loc_emp_pin_number						= $val['loc_emp_pin_number'];
				
		$full_name_manager 						= $val['full_name_manager'];
		$gender_pe 								= $val['gender_pe'];
		$address_manager 						= $val['address_manager'];
		$loc_manager_country					= $val['manager_country'];
		$loc_manager_state 						= $val['manager_state'];		
		$loc_manager_dist 						= $val['manager_dist'];
		$loc_manager_subdivision 				= $val['loc_manager_subdv'];
		$loc_manager_areatype 					= $val['loc_manager_areatype'];
		$loc_manager_areatype_code 				= $val['manager_name_areatype'];
		$loc_manager_vill_ward					= $val['loc_manager_vill_ward'];
		$l_manager_ps							= $val['loc_manager_ps'];
		$loc_manager_pin_number					= $val['loc_manager_pin_number'];
		
		$e_nature_of_work 						= $val['e_nature_of_work'];
		$other_nature_of_work_value				= ucwords($val['other_nature_of_work_value']);		
		$max_num_wrkmen 						= $val['max_num_wrkmen'];
		$e_num_of_workmen_per_or_reg 			= $val['e_num_of_workmen_per_or_reg'];
		$e_num_of_workmen_temp_or_reg 			= $val['e_num_of_workmen_temp_or_reg'];
		$workmen_if_same_similar_kind_of_work 	= $val['workmen_if_same_similar_kind_of_work'];
		$con_lab_job_desc 						= $val['con_lab_job_desc'];
		$con_lab_wage_rate_other_benefits 		= $val['con_lab_wage_rate_other_benefits'];
		$con_lab_cat_desig_nom 					= $val['con_lab_cat_desig_nom'];								
		$e_settlement_award_judgement_min_wage  = $val['e_settlement_award_judgement_min_wage'];
		$e_any_day_max_num_of_workmen 			= $val['e_any_day_max_num_of_workmen'];
		
		$amended_fields	= db_select('l_clra_amendment', 'lca')
						->fields('lca', array('amendment_fields','not_amended_fields'))
						->condition('lca.application_id', $obj3['id'])
						->condition('lca.status', 1)
						->orderBy('id', 'DESC')
						->range(0, 1);
		$amendedField 			= $amended_fields->execute()->fetchAssoc();
		$amendment_fields 		= $amendedField['amendment_fields'];
		$notAmendment_fields 	= $amendedField['not_amended_fields'];
		$amendedAddition 		= explode(',', $amendedField['amendment_fields']);
		
		$rloLwfsCode = get_rlo_lwfc_code($loc_e_vill_ward);
		
		$fieldsRegInfo = array( 
							'user_id' => $user->uid,
							'identification_number'	 => $amended_indentification_number,
							'act_id' => $act_id,													
							'e_name' => $e_name,
							'est_type' => $est_type, 
							'loc_e_name' => $loc_e_name,													
							'loc_e_dist' => $loc_e_dist,
							'loc_e_subdivision' => $loc_e_subdivision ,
							'loc_e_areatype' => $loc_e_areatype ,
							'name_areatype' => $loc_e_areatype_code ,
							'loc_e_vill_ward' => $loc_e_vill_ward,
							'l_e_ps' => $l_e_ps,
							'loc_e_pin_number' => $loc_e_pin_number,													
							'e_postal_address' => $e_postal_address,													
							'e_postal_dist' => $e_postal_dist,
							'e_postal_subdivision' => $e_postal_subdivision ,
							'e_postal_areatype' => $e_postal_areatype,
							'e_postal_name_areatype' => $e_postal_areatype_code ,
							'e_postal_vill_ward' => $e_postal_vill_ward,
							'e_postal_ps' => $e_postal_ps,
							'e_postal_pin_number' => $e_postal_pin_number,									 
							'full_name_principal_emp' => $full_name_principal_emp,
							'gender_pe' => $gender_pe,
							'address_principal_emp'	=> $address_principal_emp,
							'loc_emp_country' =>  $loc_emp_country,
							'loc_emp_state'	=>  $loc_emp_state,													
							'loc_emp_dist' =>$loc_emp_dist,
							'loc_emp_subdivision' => $loc_emp_subdivision ,
							'loc_emp_areatype' => $loc_emp_areatype,
							'emp_name_areatype' => $loc_emp_areatype_code ,
							'loc_emp_vill_ward' => $loc_emp_vill_ward,
							'l_emp_ps' => $l_emp_ps,
							'loc_emp_pin_number' =>  $loc_emp_pin_number,													
							'full_name_manager' => $full_name_manager,
							'address_manager' => $address_manager,
							'loc_manager_country' =>  $loc_manager_country,
							'loc_manager_state'	=>  $loc_manager_state,													
							'loc_manager_dist' => $loc_manager_dist,
							'loc_manager_subdivision' => $loc_manager_subdivision ,
							'loc_manager_areatype'	 => $loc_manager_areatype,
							'manager_name_areatype'	 => $loc_manager_areatype_code ,
							'loc_manager_vill_ward'	 => $loc_manager_vill_ward,
							'l_manager_ps' =>$l_manager_ps,
							'loc_manager_pin_number' =>  $loc_manager_pin_number,													
							'max_num_wrkmen' => $max_num_wrkmen, 
							'e_num_of_workmen_per_or_reg' => $e_num_of_workmen_per_or_reg, 
							'e_num_of_workmen_temp_or_reg' =>	$e_num_of_workmen_temp_or_reg, 
							'workmen_if_same_similar_kind_of_work'	=>	$workmen_if_same_similar_kind_of_work, 
							'con_lab_job_desc' => $con_lab_job_desc, 
							'con_lab_wage_rate_other_benefits' =>	$con_lab_wage_rate_other_benefits, 
							'con_lab_cat_desig_nom'	 => $con_lab_cat_desig_nom ,													
							'e_settlement_award_judgement_min_wage'	=>  $e_settlement_award_judgement_min_wage,
							'e_any_day_max_num_of_workmen' => $e_any_day_max_num_of_workmen,
							'amendment_parent_id' => $amendment_parent_id,	
							'registration_number' =>  $registration_number,
							'clra_qr_code' =>  $clra_qr_code,
							'registration_date' =>  $registration_date,
							//'certificates_fid' =>  $certificates_fid,
							'rlo_code' => $rloLwfsCode->rlo_code,
							'lwfc_code' => $rloLwfsCode->lwfc_code									
						);
		//*** Update Queries ***//
		
		if(!empty($fieldsRegInfo) && $new_application_id!=''){
			
			    /** Check Amendment fields with location ***/
				
				if($val['loc_e_name'] == $obj3['loc_e_name'] && $val['loc_e_dist'] == $obj3['loc_e_dist'] && $val['loc_e_subdivision'] == $obj3['loc_e_subdivision'] && $val['loc_e_areatype'] == $obj3['loc_e_areatype'] && $val['name_areatype'] == $obj3['name_areatype'] && $val['loc_e_vill_ward'] == $obj3['loc_e_vill_ward'] && $val['l_e_ps'] == $obj3['l_e_ps'] && $val['loc_e_pin_number'] == $obj3['loc_e_pin_number']){ // data same  -- not amended fields ("e_location", "location")
					//echo 666;exit;
					if(in_array("e_location", $amendedAddition)) {
						$findVariable 		= array("e_location", "location");
						$madeAmendedFields 	= str_replace($findVariable, "", $amendment_fields);
						$trimFinalAmendedFields = str_replace(',,', ",", $madeAmendedFields);
						$finalAmendedFields = str_replace(',,', ",", $trimFinalAmendedFields);
						
						$findVariable = array("e_location", "location");
						$madeNotAmendedFields = str_replace($findVariable, "", $notAmendment_fields);
						$addedNotAmended_fields = 'e_location,location,'.$madeNotAmendedFields;
						$finalNotAmendedFields = str_replace(',,', ",", $addedNotAmended_fields);
					}
					
				}elseif( ($val['loc_e_name'] != $obj3['loc_e_name'] || $val['loc_e_name'] == $obj3['loc_e_name'] || $val['loc_e_vill_ward'] != $obj3['loc_e_vill_ward'] || $val['l_e_ps'] != $obj3['l_e_ps'] || $val['loc_e_pin_number'] != $obj3['loc_e_pin_number']) && ($val['loc_e_dist'] == $obj3['loc_e_dist'] && $val['loc_e_subdivision'] == $obj3['loc_e_subdivision'] && $val['loc_e_areatype'] == $obj3['loc_e_areatype'] && $val['name_areatype'] == $obj3['name_areatype'])){ // Upto block same -- not amended fields ("e_location")/ amended ("location")
					//echo 333;exit;
					if(in_array("location", $amendedAddition)) { 
					
						$findVariable 		= array("e_location", "location");
						$madeAmendedFields 	= str_replace($findVariable, "", $amendment_fields);
						$addedAmended_fields = 'location,'.$madeAmendedFields;
						$trimFinalAmendedFields = str_replace(',,', ",", $addedAmended_fields);
						$finalAmendedFields = str_replace(',,', ",", $trimFinalAmendedFields);
						
						$findVariable = array("e_location", "location");
						$madeNotAmendedFields = str_replace($findVariable, "", $notAmendment_fields);
						$addedNotAmended_fields = 'e_location,'.$madeNotAmendedFields;
						$finalNotAmendedFields = str_replace(',,', ",", $addedNotAmended_fields);
						
					}
				}elseif($val['loc_e_name'] != $obj3['loc_e_name'] && $val['loc_e_dist'] != $obj3['loc_e_dist'] && $val['loc_e_subdivision'] != $obj3['loc_e_subdivision'] && $val['loc_e_areatype'] != $obj3['loc_e_areatype'] && $val['name_areatype'] != $obj3['name_areatype'] && $val['loc_e_vill_ward'] != $obj3['loc_e_vill_ward'] && $val['l_e_ps'] != $obj3['l_e_ps'] && $val['loc_e_pin_number'] != $obj3['loc_e_pin_number']){ // all data changed -- amended ("location",e_location)
					//echo 555;exit;
					if(!in_array("e_location", "location", $amendedAddition)) { 
					
						$findVariable 		= array("e_location", "location");
						$madeAmendedFields 	= str_replace($findVariable, "", $amendment_fields);
						$addedAmended_fields = 'e_location,location,'.$madeAmendedFields;
						$trimFinalAmendedFields = str_replace(',,', ",", $addedAmended_fields);
						$finalAmendedFields = str_replace(',,', ",", $trimFinalAmendedFields);
						
						$findVariable = array("e_location", "location");
						$madeNotAmendedFields = str_replace($findVariable, "", $notAmendment_fields);
						$finalNotAmendedFields = str_replace(',,', ",", $madeNotAmendedFields);
						
					}
				}elseif($val['loc_e_name'] != $obj3['loc_e_name'] && $val['loc_e_dist'] == $obj3['loc_e_dist'] && $val['loc_e_subdivision'] != $obj3['loc_e_subdivision'] && $val['loc_e_areatype'] != $obj3['loc_e_areatype'] && $val['name_areatype'] != $obj3['name_areatype'] && $val['loc_e_vill_ward'] != $obj3['loc_e_vill_ward'] && $val['l_e_ps'] != $obj3['l_e_ps'] && $val['loc_e_pin_number'] != $obj3['loc_e_pin_number']){ // Only district not changed  -- amended ("location",e_location)
					
					if(!in_array("e_location", "location", $amendedAddition)) {
					
						$findVariable 		= array("e_location", "location");
						$madeAmendedFields 	= str_replace($findVariable, "", $amendment_fields);
						$addedAmended_fields = 'e_location,location,'.$madeAmendedFields;
						$trimFinalAmendedFields = str_replace(",,", ",", $addedAmended_fields);
						$finalAmendedFields = str_replace(',,', ",", $trimFinalAmendedFields);
						
						$findVariable = array("e_location", "location");
						$madeNotAmendedFields = str_replace($findVariable, "", $notAmendment_fields);
						$finalNotAmendedFields = str_replace(',,', ",", $madeNotAmendedFields);
						
					}
				}
			
			db_update('l_clra_amendment')->fields(array('amendment_fields'=>$finalAmendedFields,'not_amended_fields'=>$finalNotAmendedFields))->condition('application_id',$obj3['id'])->condition('status', 1)->condition('applicant_uid',$user->uid)->execute();
				
			/*** Master table Update ****/
				
			$query = db_update('l_clra_registration_master');
			$query->fields($fieldsRegInfo); 
			$query->condition('id',$new_application_id);
			$query->condition('user_id',$user_id);
			$query->execute();
				
			/** For Nature Of work ***/
				
			if(!empty($e_nature_of_work)){ 
					
					db_delete('l_clra_reg_nature_of_work')
							->condition('user_id', trim($user_id),'=')
							->condition('application_id', trim($new_application_id),'=')
							->execute();
				
					db_delete('contractor_works')
							->condition('is_default',0)
							->condition('application_id', trim($new_application_id))
							->condition('act_id', trim($act_id))
							->execute();
					
				
				
					foreach($e_nature_of_work as $LoopValue){
												$fieldsEstContractorInfoNatureOfWork 				= 	array( 
																											'identification_number'	=>	$amended_indentification_number,
																											'user_id'				=>	$user_id,
																											'act_id'				=>  $act_id,
																											'application_id'		=>  $new_application_id,
																											'e_nature_of_work'		=>	$LoopValue
																										);
						db_insert('l_clra_reg_nature_of_work')->fields($fieldsEstContractorInfoNatureOfWork)->execute();
						$increment++;
					}
				
					
					/** For Other Field Nature of Work ****/
				
					if(!empty($other_nature_of_work_value)) {
				
						$ContractorOthersNatureOfWork 												= 	array( 
																											'application_id'		=>	$new_application_id,
																											'cont_work_name'		=>	ucwords($other_nature_of_work_value),
																											'act_id'				=>  $act_id,
																											'is_default'			=>  0
																											);
					
						$last_insert_other_work_id = db_insert('contractor_works')->fields($ContractorOthersNatureOfWork)->execute();
					
					
						$fieldsEstContractorInfoNatureOfWork 										= 	array( 
																											'identification_number'	=>	$amended_indentification_number,
																											'user_id'				=>	$user->uid,
																											'act_id'				=>  $act_id,
																											'application_id'		=>	$new_application_id,
																											'e_nature_of_work'		=>	$last_insert_other_work_id
																											);
					
						db_insert('l_clra_reg_nature_of_work')->fields($fieldsEstContractorInfoNatureOfWork)->execute();
				
				
					}
				}
				
			/**** DOC UPLOADED CODE *****/
				
			$fetch_reg_amended_files 	= db_select('l_documents_upload','ldu');
			$fetch_reg_amended_files 	->fields('ldu', array('trade_license_file', 'article_of_assoc_file', 'memorandum_of_cert_file', 'partnership_deed_file','factory_license_file'));
			$fetch_reg_amended_files 	->condition('ldu.application_id', $new_application_id);
			$fetch_reg_amended_files 	->condition('ldu.act_id', $act_id);
			$fetch_upload_amend_result 	= $fetch_reg_amended_files->execute();
			
			if( $fetch_upload_amend_result->rowCount() > 0 ){
			
				$amended_uploaded_files 						= 	$fetch_upload_amend_result->fetchAssoc(); 
				
				$trade_license_file_amend						=	$amended_uploaded_files['trade_license_file'];
				$article_of_assoc_file_amend					=	$amended_uploaded_files['article_of_assoc_file'];
				$memorandum_of_cert_file_amend					=	$amended_uploaded_files['memorandum_of_cert_file'];
				$partnership_deed_file_amend					=	$amended_uploaded_files['partnership_deed_file'];
				$factory_license_amend							= 	$amended_uploaded_files['factory_license_file'];
				
				
				$queryForward = db_update('l_documents_upload');
				$queryForward->fields(array(
										'identification_number'						=>	$amended_indentification_number,
										'trade_license_file' 						=>  isset($trade_license_file)&&($trade_license_file_amend!=$trade_license_file) ? $trade_license_file : $trade_license_file_amend,
										'article_of_assoc_file' 					=>  isset($article_of_assoc_file)&&($article_of_assoc_file_amend!=$article_of_assoc_file) ? $article_of_assoc_file : $article_of_assoc_file_amend,
										'memorandum_of_cert_file' 					=>  isset($memorandum_of_cert_file)&&($memorandum_of_cert_file_amend!=$memorandum_of_cert_file) ? $memorandum_of_cert_file : $memorandum_of_cert_file_amend,
										'partnership_deed_file' 					=>  isset($partnership_deed_file)&&($partnership_deed_file_amend!=$partnership_deed_file) ? $partnership_deed_file : $partnership_deed_file_amend,
										'factory_license_file' 						=>  isset($factory_license_file)&&($factory_license_amend!=$factory_license_file) ? $factory_license_file : $factory_license_amend,
				));
				$queryForward->condition('user_id',$user_id);
				$queryForward->condition('application_id',$new_application_id);
				$queryForward->execute();
			}
		}
		
		$message = 'Application updated successfully.';
		drupal_set_message(t($message));
}

