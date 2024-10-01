<?php
drupal_add_js(drupal_get_path('module', 'applicant_forms') . '/js/upload_custom.js');
function amendment_form($form, &$form_state) {
	
	global $base_root, $base_path, $user;
	
	$user_id			= $user->uid;
	
	//if($user_id == 17481){		
	
	$fetch_reg = db_select('l_clra_registration_master', 'lcrm');
	$fetch_reg->fields('lcrm', array());
	$fetch_reg->condition('lcrm.user_id', trim($user_id));
	$fetch_reg->condition('lcrm.status','I');
	$fetch_reg->orderBy('lcrm.id','DESC');
	$fetch_reg->range(0, 1);
	$fetch_reg_det = $fetch_reg->execute(); 
			
				
	if($fetch_reg_det->rowCount() > 0 ){
				
		$obj2 = $fetch_reg_det->fetchAssoc(); 
		
					
		$identification_number					=	$obj2['identification_number'];
		$act_id									=	$obj2['act_id'];
		
		$e_name									=	$obj2['e_name'];
		$loc_e_name								=	$obj2['loc_e_name'];
		$est_type 								= 	$obj2['est_type'];
		
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
		$gender_pe								=   $obj2['gender_pe'];
		$address_principal_emp					=	$obj2['address_principal_emp'];
		$loc_emp_country						=   $obj2['loc_emp_country'];
		$loc_emp_state							= 	$obj2['loc_emp_state'];
		$loc_emp_dist							=	$obj2['loc_emp_dist'];
		$loc_emp_subdivision					=	$obj2['loc_emp_subdivision'];
		$loc_emp_areatype 						=	$obj2['loc_emp_areatype'];
		$emp_name_areatype						=	$obj2['emp_name_areatype'];
		$loc_emp_vill_ward						=	$obj2['loc_emp_vill_ward'];
		$l_emp_ps								=	$obj2['l_emp_ps'];
		$loc_emp_pin_number						=   $obj2['loc_emp_pin_number'];
		
		$full_name_manager						=	$obj2['full_name_manager'];
		$address_manager						=	$obj2['address_manager'];
		$loc_manager_country					=   $obj2['loc_manager_country'];
		$loc_manager_state						=   $obj2['loc_manager_state'];
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
		$backlog_id								=	$obj2['backlog_id'];
		$application_id							=	$obj2['id'];
	}		
		
	
	/**** DOC UPLOADED CODE STARTS*****/
					
	$fetch_reg_upload_files	= db_query("select * from l_documents_upload where application_id=:application_id and user_id=:user_id",array(':user_id' => $user_id,':application_id' => $application_id ));

	if( $fetch_reg_upload_files->rowCount() > 0 ){
	
		foreach($fetch_reg_upload_files as $obj3){ 
		
			$trade_license_file						=	$obj3->trade_license_file;
			$article_of_assoc_file					=	$obj3->article_of_assoc_file;
			$memorandum_of_cert_file				=	$obj3->memorandum_of_cert_file;
			$partnership_deed_file					=	$obj3->partnership_deed_file;
			$factory_license_file					=	$obj3->factory_license_file;
		}
	}
		
	/**** DOC UPLOADED CODE ENDS *****/
	
	/**** NATURE OF WORK CODE STARTS*****/
	
	$query_get_each_nature_work  = 	db_query("select a.id,b.contractor_work_id from l_clra_reg_nature_of_work a left join contractor_works as b on a.e_nature_of_work = b.contractor_work_id where a.application_id =:application_id and a.user_id=:user_id and b.is_default =:is_default order by a.id desc", array( ':user_id' => $user_id , ':application_id' => $application_id , ':is_default' => '1')); 

		
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
	$get_other_nature_work		->condition('lcnw.application_id',$application_id);
	$other_nature_work_data 	= $get_other_nature_work->execute(); 
	
	if( $other_nature_work_data->rowCount() > 0 ){
		
		$obj3 = $other_nature_work_data->fetchAssoc(); 
		
		$other_nature_wrk_id 	= $obj3['contractor_work_id'];
		$other_nature_wrk 		= $obj3['cont_work_name'];
	}
	
	
	
	/**** NATURE OF WORK CODE ENDS *****/
	
	/**** AMENDMENT FIELD START *****/
	
	
		
	if(!empty($application_id)){		
		
		$readonly_e_name = $readonly_e_location = $readonly_e_loc_subdv =  $readonly_location =  $readonly_e_postal_address = $readonly_e_postal_location = $readonly_full_name_principal_emp = $readonly_address_principal_emp = $readonly_full_name_manager = $readonly_address_manager = $readonly_other_nature_of_work_value =  $readonly_max_num_wrkmen = $readonly_e_num_of_workmen_per_or_reg = $readonly_e_num_of_workmen_temp_or_reg = $readonly_workmen_if_same_similar_kind_of_work = $readonly_con_lab_job_desc = $readonly_con_lab_wage_rate_other_benefits = $readonly_con_lab_cat_desig_nom = $readonly_e_settlement_award_judgement_min_wage = $readonly_e_any_day_max_num_of_workmen = $readonly_estType = $readonly_gender_pe = array('class' => array('correction_view'));
		$readonly_e_loc_subdv =  array('onkeyup' => 'getSpanText()', 'onblur' => 'getSpanText()', 'onchange' => 'getSpanText()', 'class' => array('correction_view'));
		
		$readonly_e_postal_pin_number = $readonly_location_pin = $readonly_loc_emp_pin_number = $readonly_loc_manager_pin_number =array('maxlength' => 6,  'class' => array('correction_view'));
		
		$readonly_e_nature_of_work = array('style' =>'height: 150px;','class' => array('correction_view'),'onchange' => 'nature_wrk_others(this.value)');
		
		$readonly_address_principal_empTextarea = $readonly_address_manager_textarea = array('readonly' => 'readonly', 'class' => array('readonly_view'));
		
		$query = db_select('l_clra_amendment', 'lram')
					 ->	fields('lram', array('not_amended_fields'))
					 ->	condition('lram.application_id', $application_id)
					 ->condition('lram.status', 1)
					 ->	range(0, 1)
					 ->	orderBy('id', 'DESC');
					 
		$amendment_field_text_query = $query->execute();
		$amendment_field_text_array = $amendment_field_text_query->fetchAssoc();
		$amendment_field_arr = explode(',', $amendment_field_text_array['not_amended_fields']);
		
		$e_name_new = $e_location= $location = $e_postal_address_new =$pe_details =$man_details =$e_nature_of_work = $e_any_day_max_num_of_workmen_new =$e_num_of_workmen_per_or_reg_new =$e_num_of_workmen_temp_or_reg_new = $workmen_if_same_similar_kind_of_work_new =$con_lab_job_desc_new = $con_lab_wage_rate_other_benefits_new =$con_lab_cat_desig_nom_new =$e_settlement_award_judgement_min_wage_new =$max_num_wrkmen_new =0;
		
		foreach($amendment_field_arr as $val){ 
			if($val == 'e_name'){$e_name_new = 1; $readonly_e_name = array('disabled' => 'disabled', 'class' => array('readonly_view'));}
			if($val == 'e_location'){ 
				$e_location = 1 ; 
				$readonly_e_location  =  $readonly_e_loc_subdv = array('disabled' => 'disabled', 'class' => array('readonly_view'));
			}
			if($val == 'location') {
				$location = 1 ; 
				$readonly_location = $readonly_location_pin = array('disabled' => 'disabled', 'class' => array('readonly_view'));
			}
			if($val == 'e_postal_address'){ $e_postal_address_new = 1; $readonly_e_postal_address  = $readonly_e_postal_location = $readonly_e_postal_pin_number = array('disabled' => 'disabled', 'class' => array('readonly_view')); }
			
			if($val == 'pe_details'){ $pe_details = 1; $readonly_full_name_principal_emp = $readonly_address_principal_emp = $readonly_address_principal_empTextarea =  $readonly_loc_emp_pin_number =  array('disabled' => 'disabled', 'class' => array('readonly_view'));	}
			
			if($val == 'man_details') {$man_details = 1; $readonly_full_name_manager = $readonly_address_manager = $readonly_address_manager_textarea =   $readonly_loc_manager_pin_number = array('disabled' => 'disabled', 'class' => array('readonly_view'));	}
			
			if($val == 'e_nature_of_work') {$e_nature_of_work = 1; $readonly_other_nature_of_work_value = array('disabled' => 'disabled', 'class' => array('readonly_view')); $readonly_e_nature_of_work =   array('style' =>'height: 150px;','disabled' => 'disabled', 'class' => array('readonly_view'));}
			if($val == 'max_num_wrkmen'){ $max_num_wrkmen_new = 1; $readonly_max_num_wrkmen = array('disabled' => 'disabled', 'class' => array('readonly_view'));}
			
			if($val == 'e_num_of_workmen_per_or_reg') {$e_num_of_workmen_per_or_reg_new = 1; $readonly_e_num_of_workmen_per_or_reg = array('disabled' => 'disabled', 'class' => array('readonly_view'));}
			if($val == 'e_num_of_workmen_temp_or_reg') {$e_num_of_workmen_temp_or_reg_new = 1; $readonly_e_num_of_workmen_temp_or_reg = array('disabled' => 'disabled', 'class' => array('readonly_view'));}
			if($val == 'workmen_if_same_similar_kind_of_work') {$workmen_if_same_similar_kind_of_work_new = 1; $readonly_workmen_if_same_similar_kind_of_work = $readonly_con_lab_job_desc = $readonly_con_lab_wage_rate_other_benefits = $readonly_con_lab_cat_desig_nom = $readonly_e_settlement_award_judgement_min_wage = array('disabled' => 'disabled', 'class' => array('readonly_view')); }
			if($val == 'e_any_day_max_num_of_workmen') {$e_any_day_max_num_of_workmen_new = 1; $readonly_e_any_day_max_num_of_workmen =  array('disabled' => 'disabled', 'class' => array('readonly_view')); }
		}
	}
			
	if(!empty($application_id)){
				
		if($status == 'B'){
			$query = db_select('l_remarks_master', 'lrm')
						 ->	fields('lrm', array('remark_field_text'))
						 ->	condition('lrm.application_id', $application_id)
						 ->	orderBy('remark_id', 'DESC');
						 
			$remark_field_text_query = $query->execute();
			$remark_field_text_array = $remark_field_text_query->fetchAssoc();
			
			$field_arr = explode(',', $remark_field_text_array['remark_field_text']);
			
			
			$readonly_e_nature_of_work = array('style'=>"height:120px;", 'onchange' => 'nature_wrk_others(this.value)');			
			$readonly_trade_license = array( 'id' => 'upload_check_in_1_id', 'onclick' => 'upload_check_fun_1()', $tlf => $tlf );
			$readonly_article_of_assoc = array( 'id' => 'upload_check_in_2_id', 'onclick' => 'upload_check_fun_2()', $asf => $asf );
			$readonly_memorandum_of_cert = array( 'id' => 'upload_check_in_3_id', 'onclick' => 'upload_check_fun_3()', $mcf => $mcf );
			$readonly_partnership_deed = array( 'id' => 'upload_check_in_4_id', 'onclick' => 'upload_check_fun_4()', $pdf => $pdf );
			$readonly_factory_license = array( 'id' => 'upload_check_in_5_id', 'onclick' => 'upload_check_fun_5()', $flf => $flf );
			$readonly_loc_e_subdivision 	= array('onkeyup' => 'getSpanText()', 'onblur' => 'getSpanText()', 'onchange' => 'getSpanText()');
			foreach($field_arr as $field_arr_val){
			if($field_arr_val == 'e_name'){	$readonly_e_name = array('disabled' => 'disabled', 'class' => array('readonly_view'));}
			//if($field_arr_val == 'est_type'){	$readonly_estType = array('disabled' => 'disabled', 'class' => array('readonly_view'));}
			if($field_arr_val == 'e_location'){	$readonly_e_location = array('disabled' => 'disabled', 'class' => array('readonly_view'));}
			if($field_arr_val == 'location'){	
				$readonly_location = array('disabled' => 'disabled', 'class' => array('readonly_view'));
				$readonly_location_pin = array('disabled' => 'disabled', 'maxlength' => 6,  'class' => array('readonly_view'));
			}
			if($field_arr_val == 'e_postal_address'){
				$readonly_e_postal_address 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
				$readonly_e_postal_location 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
				$readonly_e_postal_pin_number	= array('disabled' => 'disabled', 'maxlength' => 6,  'class' => array('readonly_view'));
			}		
			if($field_arr_val == 'full_name_principal_emp'){	$readonly_full_name_principal_emp = array('disabled' => 'disabled', 'class' => array('readonly_view'));}
			if($field_arr_val == 'gender_pe'){	$readonly_gender_pe = array('disabled' => 'disabled', 'class' => array('readonly_view'));}
			if($field_arr_val == 'address_principal_emp'){
				$readonly_address_principal_emp = $readonly_loc_emp_pin_number	= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
				$readonly_address_principal_empTextarea = array('readonly' => 'readonly', 'class' => array('readonly_view'));
			}
			if($field_arr_val == 'full_name_manager'){	$readonly_full_name_manager = array('disabled' => 'disabled', 'class' => array('readonly_view'));}	
			if($field_arr_val == 'address_manager'){
				$readonly_address_manager 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
				$readonly_address_manager_textarea	= array('readonly' => 'readonly', 'class' => array('readonly_view'));
				$readonly_loc_manager_pin_number 	= array('disabled' => 'disabled','maxlength' => 6,  'class' => array('readonly_view'));						
			}
			if($field_arr_val == 'e_nature_of_work'){ 
				$readonly_e_nature_of_work			 = array('readonly' => 'readonly','style' =>'height: 150px;', 'class' => array('readonly_view'));
				$readonly_other_nature_of_work_value = array('disabled' => 'disabled', 'class' => array('readonly_view'));
			}
			if($field_arr_val == 'max_num_wrkmen'){ $readonly_max_num_wrkmen = array('disabled' => 'disabled', 'class' => array('readonly_view'));} 
			if($field_arr_val == 'e_num_of_workmen_per_or_reg'){$readonly_e_num_of_workmen_per_or_reg = array('disabled' => 'disabled', 'class' => array('readonly_view'));}
			if($field_arr_val == 'e_num_of_workmen_temp_or_reg'){$readonly_e_num_of_workmen_temp_or_reg = array('disabled' => 'disabled', 'class' => array('readonly_view'));}	
			if($field_arr_val == 'workmen_if_same_similar_kind_of_work'){$readonly_workmen_if_same_similar_kind_of_work = array('disabled' => 'disabled', 'class' => array());}	
			if($field_arr_val == 'con_lab_job_desc'){$readonly_con_lab_job_desc = array('readonly' => 'readonly', 'class' => array('readonly_view'));}	
			if($field_arr_val == 'con_lab_wage_rate_other_benefits'){$readonly_con_lab_wage_rate_other_benefits = array('disabled' => 'disabled', 'class' => array('readonly_view'));}	
			if($field_arr_val == 'con_lab_cat_desig_nom'){$readonly_con_lab_cat_desig_nom = array('disabled' => 'disabled', 'class' => array('readonly_view'));}	
			if($field_arr_val == 'e_settlement_award_judgement_min_wage'){$readonly_e_settlement_award_judgement_min_wage = array('disabled' => 'disabled', 'class' => array('readonly_view'));}
			if($field_arr_val == 'e_any_day_max_num_of_workmen'){ $readonly_e_any_day_max_num_of_workmen = array('disabled' => 'disabled', 'class' => array('readonly_view'));} 
		}
		
		}
		
		$form['clra_reg_master_edit']['header'] = array(
										  '#type'		=> 'markup',
										  '#markup' 	=> '<div class="content">',
										 
										);
																	
		$form['clra_reg_master_edit']['application_id'] = array(
									  '#prefix'				=> '<br class="clear">',
									  '#title'				=> 'Application Id',
									  '#type' 				=> 'hidden',
									  '#default_value'		=> !empty($application_id) ? encryption_decryption_fun('encrypt', $application_id) : '',
									  '#attributes'			=> array('readonly'=>'readonly')
									);
						
		$form['clra_reg_master_edit']['act_id'] = array(
									  '#title'				=> 'Act Id',
									  '#type' 				=> 'hidden',
									  '#default_value'		=> !empty($act_id) ? encryption_decryption_fun('encrypt', $act_id) : '',
									  '#attributes' 		=> array('readonly'=>'readonly')
									);
																
		$form['clra_reg_master_edit']['identification_number'] = array(
									  '#title'				=> 'Identification Number',
									  '#type' 				=> 'hidden',
									  '#default_value'		=> !empty($identification_number) ? encryption_decryption_fun('encrypt', $identification_number) : '',
									  '#attributes' 		=> array('readonly'=>'readonly')
									);
		$form['clra_reg_master_edit']['markup_link'] = array(
										'#type' 			=> 'markup',
										'#markup'			=> '<div class="reset-amd-fields" style="margin-top: 42px;">'.l('Reset Amendment Fields', 'apply-clra-reg-amendment/'.encryption_decryption_fun('encrypt', $obj2['id']), array('attributes' => array('html' => TRUE, 'class' => 'link-btn mtop-btm-15# pull-right btn-reset-amd-fields'))).'</div>',
												);

		if($e_name_new!=1 || $e_location !=1 || $location != 1){		
			$form['clra_reg_master_edit']['e_name'] = array(
										'#prefix' 			=> '<div class="panel panel-default panel-form"><div class="panel-heading">1. Name and location of the Establishment <span id="rlo_location_id" class="form_note"></span></div><div class="panel-body"><div class="col-md-6"><label class="input">',
										'#suffix' 			=>  '</label></div>',
										'#title'			=>  'Establishment Name.',
										'#type' 			=> 'textfield',
										'#default_value' 	=> !empty($e_name) ? $e_name : '',
										'#required' 		=> TRUE,
										'#attributes' 		=> $readonly_e_name,
										'#element_validate' => array('MISC_TEXTBOX_TEXT_ENAME_VALIDATE')
										);
			$form['clra_reg_master_edit']['est_type'] = array(
										'#title' 			=> 'Type of The Establishment',
										'#type' 			=> 'select',
										'#options' 			=> array('micro' => 'Micro', 'small' => 'Small', 'medium' => 'Medium', 'large' => 'Large'),
										'#required' 		=>  TRUE,
										'#default_value' 	=> $est_type,
										//'#attributes' 	=> $readonly_e_name,
										'#prefix' 			=> '<div class="col-md-6"><label class="select">',
										'#suffix' 			=> '</label></div>'
									);
				
												
			$form['clra_reg_master_edit']['loc_e_name'] = array(
										'#prefix' 			=> '<div class="col-md-8"><label class="input">',
										'#suffix' 			=> '</label></div>',
										'#title'			=> 'House/Flat No./Street No./Village/Locality Name etc.',
										'#type' 			=> 'textfield',
										'#default_value' 	=> !empty($loc_e_name) ? $loc_e_name : '',
										'#required' 		=> TRUE, 
										'#attributes' 		=> $readonly_e_location,
										'#element_validate'	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
										);
			$form['clra_reg_master_edit']['loc_e_dist'] = array(
									   '#prefix' 			=> '<div class="col-lg-4 col-md-4 min-height-80"><label class="select">',
									   '#suffix' 			=> '</label></div>',
									   '#title'				=> 'Select District',
									   '#type' 				=> 'select',
									   '#options' 			=> common_custom_user_district_list_new(),
									   '#attributes'		=>  $readonly_e_location,
									   '#default_value' 	=> !empty($loc_e_dist) ? $loc_e_dist: '',
									   '#required' 			=> TRUE,
									   '#ajax'				=> array(
																'event' 	=> 'change',
																'effect' 	=> 'fade',			
																'callback'	=> 'get_clra_registration_info_edit_form_ajax_callback',
																'progress'	=> array('type'=> 'throbber'),
															  )
									);
			$value_loc_e_dist 		= 	isset($form_state['values']['loc_e_dist']) ? $form_state['values']['loc_e_dist'] : $loc_e_dist;
			
			if($value_loc_e_dist != "" && !empty($value_loc_e_dist) && is_numeric($value_loc_e_dist)){
				$form['clra_reg_master_edit']['loc_e_subdivision']  = array(
																		  '#prefix' 		=> '<div class="col-lg-4 col-md-4 min-height-80" id="clra_subdvsn_replace"><label class="select">',
																		  '#suffix' 		=> '</label></div>',
																		  '#title'			=> 'Select Subdivision',
																		  '#type' 			=> 'select',
																		  '#options' 		=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist),
																		  '#required' 		=> TRUE,
																		  '#default_value' 	=> !empty($loc_e_subdivision) ? $loc_e_subdivision: '',
																		  '#attributes' 	=> $readonly_e_loc_subdv,
																		  '#id'				=> 'loc_e_subdivision_id',
																		  '#ajax'			=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_clra_registration_info_edit_form_ajax_callback',
																									'progress'	=> array('type'=> 'throbber'),
																								  ),
																	);	
			}else{
				$form['clra_reg_master_edit']['loc_e_subdivision'] = array(
																		  '#prefix' 		=> '<div class="col-lg-4 col-md-4 min-height-80" id="clra_subdvsn_replace"><label class="select">',
																		  '#suffix' 		=> '</label></div>',
																		  '#title'			=> 'Select Subdivision',
																		  '#type' 			=> 'select',
																		  '#options' 		=> array(),
																		  '#attributes' 	=> $readonly_e_loc_subdv,
																		  '#required' 		=> TRUE,
																		  '#ajax'			=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_clra_registration_info_edit_form_ajax_callback',
																									'progress'	=> array('type'=> 'throbber'),
																								  ),
																	);
			}
			$value_loc_e_subdivision = isset($form_state['values']['loc_e_subdivision']) ? $form_state['values']['loc_e_subdivision'] : $loc_e_subdivision;
			
			if($value_loc_e_dist != "" && $value_loc_e_subdivision != ""){
				$form['clra_reg_master_edit']['loc_e_areatype']	= array(
																		  '#prefix' 		=> '<div class="col-md-4 min-height-80" id="clra_loc_e_areatype"><label class="select">',
																		  '#suffix' 		=> '</label></div>',
																		  '#title'			=> 'Select Block/Municipality/Corporation/SEZ/Notified Area',
																		  '#type' 			=> 'select',
																		  '#options' 		=> common_custom_user_areatype_list_new($value_loc_e_dist, $value_loc_e_subdivision),
																		  '#default_value'	=> !empty($loc_e_areatype) ? $loc_e_areatype : '',
																		  '#attributes' 	=> $readonly_e_location,
																		  '#required' 		=> TRUE,
																		  '#ajax'			=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_clra_registration_info_edit_form_ajax_callback',
																									'progress'	=> array('type'=> 'throbber'),
																								  ),
																			);
			}else{
				$form['clra_reg_master_edit']['loc_e_areatype']	= array(
																		  '#prefix' 		=> '<div class="col-md-4 min-height-80" id="clra_loc_e_areatype"><label class="select">',
																		  '#suffix' 		=> '</label></div>',
																		  '#title'			=> 'Select Block/Municipality/Corporation/SEZ/Notified Area',
																		  '#type' 			=> 'select',
																		  '#options' 		=> array(),
																		  '#attributes' 	=> $readonly_e_location,
																		  '#required' 		=> TRUE,
																		  '#ajax'			=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_clra_registration_info_edit_form_ajax_callback',
																									'progress'	=> array('type'=> 'throbber'),
																								  ),
																			);
			}
			$value_loc_e_areatype = isset($form_state['values']['loc_e_areatype']) ? $form_state['values']['loc_e_areatype'] : $loc_e_areatype;
			
			if ($value_loc_e_dist != "" && $value_loc_e_subdivision != ""&& $value_loc_e_areatype != "") {
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
				$form['clra_reg_master_edit']['loc_e_areatype_code'] = array(
																	  '#prefix' 		=> '<div class="col-md-4 min-height-80" id="clra_common_1_replace"><label class="select">',
																	  '#suffix' 		=> '</label></div>',
																	  '#title'			=> 'Select '.$areatype_name,
																	  '#type' 			=> 'select',
																	  '#options' 		=> common_custom_user_block_list_new($value_loc_e_subdivision, $value_loc_e_areatype),
																	  '#required' 		=> TRUE,
																	  '#attributes' 	=> $readonly_e_location,
																	  '#default_value'	=> !empty($loc_e_areatype_code) ? $loc_e_areatype_code : '',
																	  '#ajax'			=> array(
																								'event' 	=> 'change',
																								'effect' 	=> 'fade',			
																								'callback'	=> 'get_clra_registration_info_edit_form_ajax_callback',
																								'progress'	=> array('type'=> 'throbber'),
																							  )
																 );	
			}else{
				$form['clra_reg_master_edit']['loc_e_areatype_code'] = array(
																	 '#prefix' 			=> '<div class="col-md-4 min-height-80" id="clra_common_1_replace"><label class="select">',
																	 '#suffix' 			=> '</label></div>',
																	 '#title'			=> 'Select Block',
																	 '#type' 			=> 'select',
																	 '#options' 		=> array(),
																	 '#attributes' 		=> $readonly_e_location,
																	 '#required' 		=> TRUE,
																	 '#ajax'			=> array(
																								'event' 	=> 'change',
																								'effect' 	=> 'fade',			
																								'callback'	=> 'get_clra_registration_info_edit_form_ajax_callback',
																								'progress'	=> array('type'=> 'throbber'),
																							  )
																  );
			}
			$value_loc_e_areacode = isset($form_state['values']['loc_e_areatype_code']) ? $form_state['values']['loc_e_areatype_code'] : $loc_e_areatype_code;
			
			if($value_loc_e_dist != "" && $value_loc_e_subdivision != "" && $value_loc_e_areatype != ""  && $value_loc_e_areacode != "" ){
				
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
				
				$form['clra_reg_master_edit']['loc_e_vill_ward']	= array(
																		  '#prefix' 		=> '<div class="col-md-4 min-height-80" id="clra_common_2_replace"><label class="select">',
																		  '#suffix' 		=> '</label></div>',
																		  '#title'			=> 'Select '.$area_name,
																		  '#type' 			=> 'select',
																		  '#options'		=> common_custom_user_villward_list_new($value_loc_e_areacode,$area_code),
																		  '#default_value'	=> !empty($loc_e_vill_ward) ? $loc_e_vill_ward : '',
																		  '#attributes' 	=> $readonly_location,
																		  '#required' 		=> TRUE,
																		  '#ajax'			=> array(
																								'event' 	=> 'change',
																								'effect' 	=> 'fade',			
																								'callback'	=> 'get_clra_registration_info_edit_form_ajax_callback',
																								'progress'	=> array('type'=> 'throbber'),
																							  )
																		);
			}else{
				$form['clra_reg_master_edit']['loc_e_vill_ward'] = array(
																		 '#prefix' 			=> '<div class="col-md-4 min-height-80" id="clra_common_2_replace"><label class="select">',
																		 '#suffix' 			=> '</label></div>',
																		 '#title'			=> 'Select Gram Panchayat/Ward/Sector/Notified Area',
																		 '#type' 			=> 'select',
																		 '#options' 		=> array(),
																		 '#required' 		=> TRUE,
																		 '#attributes' 		=> $readonly_location,
																		 '#ajax'			=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_clra_registration_info_edit_form_ajax_callback',
																									'progress'	=> array('type'=> 'throbber'),
																								  )
																	  );
			}
			$value_loc_e_vill_ward = isset($form_state['values']['loc_e_vill_ward']) ? $form_state['values']['loc_e_vill_ward'] :$loc_e_vill_ward;
			
			if ($value_loc_e_dist != "" && $value_loc_e_subdivision != "" && $value_loc_e_areatype != ""  && $value_loc_e_areacode != "" && $value_loc_e_vill_ward != "" ){
				$form['clra_reg_master_edit']['loc_e_ps'] 		= array(
																	  '#prefix' 		=> '<div class="col-md-4 min-height-80" id="clra_ps_replace"><label class="select">',
																	  '#suffix' 		=> '</label></div>',
																	  '#title'			=> 'Select Police Station',
																	  '#type' 			=> 'select',
																	  '#attributes' 	=> $readonly_location,
																	  '#options' 		=> common_custom_user_ps_list_new($value_loc_e_dist),
																	  '#default_value'	=> !empty($loc_e_ps) ? $loc_e_ps : '',
																	  '#required' 		=> TRUE
																  );
			}else{
				$form['clra_reg_master_edit']['loc_e_ps'] 		= array(
																	'#prefix' 			=> '<div class="col-md-4 min-height-80" id="clra_ps_replace"><label class="select">',
																	'#suffix' 			=> '</label></div>',
																	'#title'			=> 'Select Police Station',
																	'#type' 			=> 'select',
																	'#options' 			=> array(),
																	'#required' 		=> TRUE,
																	'#attributes' 		=> $readonly_location,
																	'#ajax'				=> array(
																								'event' 	=> 'change',
																								'effect' 	=> 'fade',			
																								'callback'	=> 'get_clra_registration_info_edit_form_ajax_callback',
																								'progress'	=> array('type'=> 'throbber'),
																							  )
															  );
			}
			$form['clra_reg_master_edit']['loc_e_pin_number'] = array(
																	  '#prefix' 		=> '<div class="col-md-4 min-height-80"><label class="input">',
																	  '#suffix' 		=> '</label></div></div></div>',
																	  '#title'			=> 'Pin Code',
																	  '#type' 			=> 'textfield',
																	  '#attributes' 	=> array('maxlength' => 6),
																	  '#default_value'	=> !empty($loc_e_pin_number) ? $loc_e_pin_number : '',
																	  '#required' 		=> TRUE,
																	  '#attributes' 	=> $readonly_location_pin,
																	  '#element_validate'=> array('MISC_PIN_NUMBER'),
																  );
		}

        if($e_postal_address_new != 1){
			$form['clra_reg_master_edit']['e_postal_address'] = array(
																	  '#prefix' 			=> '<div class="panel panel-default panel-form"><div class="panel-heading">2. Registered Office address of the Establishment</div><div class="panel-body"><div class="col-lg-8 col-md-8"><label class="input">',
																	  '#suffix' 			=> '</label></div>',
																	  '#title'				=> 'Postal Address of the Establishment',
																	  '#type' 				=> 'textfield',
																	  '#default_value' 		=> !empty($e_postal_address) ? $e_postal_address : '',
																	  '#attributes'			=> $readonly_e_postal_address,
																	  '#element_validate' 	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																	);	
			$form['clra_reg_master_edit']['e_postal_dist'] 	= array(
																	  '#prefix' 			=> '<div class="col-lg-4 col-md-4 min-height-80"><label class="select">',
																	  '#suffix' 			=> '</label></div>',
																	  '#title'				=> 'Select District',
																	  '#type' 				=> 'select',
																	  '#options' 			=> common_custom_user_district_list_new() ,
																	  '#default_value' 		=> !empty($e_postal_dist) ? $e_postal_dist	:'',
																	  '#required' 			=> TRUE,
																	  '#attributes'			=> $readonly_e_postal_location,
																	  '#ajax'				=> array(
																								'event' 	=> 'change',
																								'effect' 	=> 'fade',			
																								'callback'	=> 'get_clra_registration_e_postal_form_ajax_callback',
																								'progress'	=> '',
																							  )
																	);
			$value_e_postal_dist 	= 	isset($form_state['values']['e_postal_dist']) ? $form_state['values']['e_postal_dist'] : $e_postal_dist;
			
			if($value_e_postal_dist != "" && !empty($value_e_postal_dist) && is_numeric($value_e_postal_dist)){
				
				$form['clra_reg_master_edit']['e_postal_subdivision']  = array(
																		  '#prefix' 		=> '<div class="col-lg-4 col-md-4 min-height-80" id="clra_e_postal_subdvsn_replace"><label class="select">',
																		  '#suffix' 		=> '</label></div>',
																		  '#title'			=> 'Select Subdivision',
																		  '#type' 			=> 'select',
																		  '#options' 		=> common_custom_user_subdivisionlist_list_new($value_e_postal_dist),
																		  '#required' 		=> TRUE,
																		  '#default_value' 	=> !empty($e_postal_subdivision) ? $e_postal_subdivision: '',
																		  '#attributes' 	=> $readonly_e_postal_location,
																		  '#ajax'			=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_clra_registration_e_postal_form_ajax_callback',
																									'progress'	=> array('type'=> 'throbber'),
																								  ),
																	);	
			}else{
				$form['clra_reg_master_edit']['e_postal_subdivision'] = array(
																		  '#prefix' 		=> '<div class="col-lg-4 col-md-4 min-height-80" id="clra_e_postal_subdvsn_replace"><label class="select">',
																		  '#suffix' 		=> '</label></div>',
																		  '#title'			=> 'Select Subdivision',
																		  '#type' 			=> 'select',
																		  '#options' 		=> array(),
																		  '#attributes' 	=> $readonly_e_postal_location,
																		  '#required' 		=> TRUE,
																		  '#ajax'			=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_clra_registration_e_postal_form_ajax_callback',
																									'progress'	=> array('type'=> 'throbber'),
																								  ),
																	);
			}
			$value_e_postal_subdivision = isset($form_state['values']['e_postal_subdivision']) ? $form_state['values']['e_postal_subdivision'] : $e_postal_subdivision;
			
			if($value_e_postal_dist != "" && $value_e_postal_subdivision != ""){
				$form['clra_reg_master_edit']['e_postal_areatype']	= array(
																		  '#prefix' 		=> '<div class="col-md-4 min-height-80" id="postal_loc_e_areatype"><label class="select">',
																		  '#suffix' 		=> '</label></div>',
																		  '#title'			=> 'Select Block/Municipality/Corporation/SEZ/Notified Area',
																		  '#type' 			=> 'select',
																		  '#options' 		=> common_custom_user_areatype_list_new($value_e_postal_dist, $value_e_postal_subdivision),
																		  '#default_value'	=> !empty($e_postal_areatype) ? $e_postal_areatype : '',
																		  '#attributes' 	=> $readonly_e_postal_location,
																		  '#required' 		=> TRUE,
																		  '#ajax'			=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_clra_registration_e_postal_form_ajax_callback',
																									'progress'	=> array('type'=> 'throbber'),
																								  ),
																			);
			}else{
				$form['clra_reg_master_edit']['e_postal_areatype']	= array(
																		  '#prefix' 		=> '<div class="col-md-4 min-height-80" id="postal_loc_e_areatype"><label class="select">',
																		  '#suffix' 		=> '</label></div>',
																		  '#title'			=> 'Select Block/Municipality/Corporation/SEZ/Notified Area',
																		  '#type' 			=> 'select',
																		  '#options' 		=> array(),
																		  '#attributes' 	=> $readonly_e_postal_location,
																		  '#required' 		=> TRUE,
																		  '#ajax'			=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_clra_registration_e_postal_form_ajax_callback',
																									'progress'	=> array('type'=> 'throbber'),
																								  ),
																			);
			}
			$value_e_postal_areatype = isset($form_state['values']['e_postal_areatype']) ? $form_state['values']['e_postal_areatype'] : $e_postal_areatype;
			
			if ($value_e_postal_dist != "" && $value_e_postal_subdivision != ""&& $value_e_postal_areatype != "") {
				if ( $value_e_postal_areatype == 'B' ){
					$areatype_name	= 'Block';
				}elseif( $value_e_postal_areatype == 'M'){
					$areatype_name	= 'Municipality';
				}elseif( $value_e_postal_areatype == 'C'){
					$areatype_name	= 'Corporation';
				}elseif( $value_e_postal_areatype == 'S'){
					$areatype_name	= 'SEZ';
				}elseif($value_e_postal_areatype == 'N'){
					$areatype_name	= 'Notified Area';
				}
				$form['clra_reg_master_edit']['e_postal_code'] = array(
																	  '#prefix' 		=> '<div class="col-md-4 min-height-80" id="clra_e_postal_code_replace"><label class="select">',
																	  '#suffix' 		=> '</label></div>',
																	  '#title'			=> 'Select '.$areatype_name,
																	  '#type' 			=> 'select',
																	  '#options' 		=> common_custom_user_block_list_new($value_e_postal_subdivision, $value_e_postal_areatype),
																	  '#required' 		=> TRUE,
																	  '#attributes' 	=> $readonly_e_postal_location,
																	  '#default_value'	=> !empty($e_postal_name_areatype) ? $e_postal_name_areatype : '',
																	  '#ajax'			=> array(
																								'event' 	=> 'change',
																								'effect' 	=> 'fade',			
																								'callback'	=> 'get_clra_registration_e_postal_form_ajax_callback',
																								'progress'	=> array('type'=> 'throbber'),
																							  )
																 );	
			}else{
				$form['clra_reg_master_edit']['e_postal_code'] = array(
																	 '#prefix' 			=> '<div class="col-md-4 min-height-80" id="clra_e_postal_code_replace"><label class="select">',
																	 '#suffix' 			=> '</label></div>',
																	 '#title'			=> 'Select Block',
																	 '#type' 			=> 'select',
																	 '#options' 		=> array(),
																	 '#attributes' 		=> $readonly_e_postal_location,
																	 '#required' 		=> TRUE,
																	 '#ajax'			=> array(
																								'event' 	=> 'change',
																								'effect' 	=> 'fade',			
																								'callback'	=> 'get_clra_registration_e_postal_form_ajax_callback',
																								'progress'	=> array('type'=> 'throbber'),
																							  )
																  );
			}
			$e_postal_code = isset($form_state['values']['e_postal_code']) ? $form_state['values']['e_postal_code'] : $e_postal_name_areatype;
			
			if($value_e_postal_dist != "" && $value_e_postal_subdivision != "" && $value_e_postal_areatype != ""  && $e_postal_code != "" ){
				
				if ( $value_e_postal_areatype == 'B' ){
					$area_name		= 'Gram Panchayat';
					$area_code 		= 'V';
				}elseif( $value_e_postal_areatype == 'M'){
					$area_name		= 'Ward';
					$area_code 		= 'W';
				}elseif( $value_e_postal_areatype == 'C'){
					$area_name		= 'Ward';
					$area_code 		= 'C';
				}elseif( $value_e_postal_areatype == 'S'){
					$area_name		= 'Sector';
					$area_code 		= 'S';
				}elseif($value_e_postal_areatype == 'N'){
					$area_name		= 'Notified Area';
					$area_code 		= 'N';
				}
				
				$form['clra_reg_master_edit']['e_postal_vill_ward']	= array(
																		  '#prefix' 		=> '<div class="col-md-4 min-height-80" id="clra_e_postal_villward_replace"><label class="select">',
																		  '#suffix' 		=> '</label></div>',
																		  '#title'			=> 'Select '.$area_name,
																		  '#type' 			=> 'select',
																		  '#options'		=> common_custom_user_villward_list_new($e_postal_code,$area_code),
																		  '#default_value'	=> !empty($e_postal_vill_ward) ? $e_postal_vill_ward : '',
																		  '#attributes' 	=> $readonly_e_postal_location,
																		  '#required' 		=> TRUE,
																		  '#ajax'			=> array(
																								'event' 	=> 'change',
																								'effect' 	=> 'fade',			
																								'callback'	=> 'get_clra_registration_e_postal_form_ajax_callback',
																								'progress'	=> array('type'=> 'throbber'),
																							  )
																		);
			}else{
				$form['clra_reg_master_edit']['e_postal_vill_ward'] = array(
																		 '#prefix' 			=> '<div class="col-md-4 min-height-80" id="clra_e_postal_villward_replace"><label class="select">',
																		 '#suffix' 			=> '</label></div>',
																		 '#title'			=> 'Select Gram Panchayat/Ward/Sector/Notified Area',
																		 '#type' 			=> 'select',
																		 '#options' 		=> array(),
																		 '#required' 		=> TRUE,
																		 '#attributes' 		=> $readonly_e_postal_location,
																		 '#ajax'			=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_clra_registration_e_postal_form_ajax_callback',
																									'progress'	=> array('type'=> 'throbber'),
																								  )
																	  );
			}
			$value_e_postal_vill_ward = isset($form_state['values']['e_postal_vill_ward']) ? $form_state['values']['e_postal_vill_ward'] :$e_postal_vill_ward;
			
			if ($value_e_postal_dist != "" && $value_e_postal_subdivision != "" && $value_e_postal_areatype != ""  && $e_postal_code != "" && $value_e_postal_vill_ward != "" ){
				$form['clra_reg_master_edit']['e_postal_ps'] 		= array(
																	  '#prefix' 		=> '<div class="col-md-4 min-height-80" id="clra_e_postal_ps_replace"><label class="select">',
																	  '#suffix' 		=> '</label></div>',
																	  '#title'			=> 'Select Police Station',
																	  '#type' 			=> 'select',
																	  '#attributes' 	=> $readonly_e_postal_location,
																	  '#options' 		=> common_custom_user_ps_list_new($value_e_postal_dist),
																	  '#default_value'	=> !empty($e_postal_ps) ? $e_postal_ps : '',
																	  '#required' 		=> TRUE,
																	  '#ajax'				=> array(
																								'event' 	=> 'change',
																								'effect' 	=> 'fade',			
																								'callback'	=> 'get_clra_registration_e_postal_form_ajax_callback',
																								'progress'	=> array('type'=> 'throbber'),
																							  )
																  );
			}else{
				$form['clra_reg_master_edit']['e_postal_ps'] 		= array(
																	'#prefix' 			=> '<div class="col-md-4 min-height-80" id="clra_e_postal_ps_replace"><label class="select">',
																	'#suffix' 			=> '</label></div>',
																	'#title'			=> 'Select Police Station',
																	'#type' 			=> 'select',
																	'#options' 			=> array(),
																	'#required' 		=> TRUE,
																	'#attributes' 		=> $readonly_e_postal_location,
																	'#ajax'				=> array(
																								'event' 	=> 'change',
																								'effect' 	=> 'fade',			
																								'callback'	=> 'get_clra_registration_e_postal_form_ajax_callback',
																								'progress'	=> array('type'=> 'throbber'),
																							  )
															  );
			}
			$form['clra_reg_master_edit']['e_postal_pin_number'] = array(
																	  '#prefix' 		=> '<div class="col-md-4 min-height-80"><label class="input">',
																	  '#suffix' 		=> '</label></div></div></div>',
																	  '#title'			=> 'Pin Code',
																	  '#type' 			=> 'textfield',
																	  '#attributes' 	=> array('maxlength' => 6),
																	  '#default_value'	=> !empty($e_postal_pin_number) ? $e_postal_pin_number : '',
																	  '#required' 		=> TRUE,
																	  '#attributes' 	=> $readonly_e_postal_pin_number,
																	  '#element_validate'=> array('MISC_PIN_NUMBER'),
																  );
		}






		if($pe_details != 1){
			$form['clra_reg_master_edit']['full_name_principal_emp'] = array(
																		  '#prefix'			=> '<div class="panel panel-default panel-form"><div class="panel-heading">3. Full Name and address of the Principal Employer</div><div class="panel-body"><div class="col-md-4"><label class="input">',
																		   '#suffix' 		=> '</label></div>',
																		  '#title'			=> 'Principal Employer Name',
																		  '#type' 			=> 'textfield',
																		  '#default_value' 	=> !empty($full_name_principal_emp) ? $full_name_principal_emp : '',
																		  '#required' 		=> TRUE,
																		  '#attributes' 	=> $readonly_full_name_principal_emp,
																		 '#element_validate'=> array('MISC_TEXTBOX_TEXT_VALIDATE')
										);
			$form['clra_reg_master_edit']['gender_pe'] 				= array(
																		  '#prefix'				=> '<div class="col-md-4 inline-group">',
																		  '#suffix'				=>'</div>',
																		  '#type' 				=> 'radios',
																		  '#title' 				=> t('Gender'),
																		  '#options' 			=> array('M'=>t('Male'), 'F'=>t('Female'), 'O'=>t('Transgender')),
																		  '#default_value' 		=> isset($gender_pe) ? $gender_pe : 'M',
																		  '#required' 			=> TRUE
																		);
																		
			$form['clra_reg_master_edit']['emp_country'] 			= array(
																		'#prefix' 			=> '<div class="col-md-4"><label class="select">',
																		'#suffix' 			=> '</label></div>',
																		'#title'			=> 'Select Country',
																		'#type' 			=> 'select',
																		'#options' 			=> array('1' => 'India', '2' => 'Others'),
																		'#default_value'	=> !empty($loc_emp_country) ? $loc_emp_country: '',
																		'#required' 		=> TRUE,
																		'#attributes' 		=> $readonly_emp_address,
																		'#ajax'				=> array(
																			'event' 		=> 'change',
																			'effect' 		=> 'fade',			
																			'callback'		=> 'get_ajax_edit_country_emp',
																			'progress'		=> array('type'=> 'throbber'),
																	)
																);
															
		 	
			 $value_emp_country = isset($form_state['values']['emp_country']) ? $form_state['values']['emp_country'] :$loc_emp_country;
			
		
			 if($value_emp_country != $loc_emp_country ){
						
				$loc_emp_state			= '';
				$loc_emp_dist 			= '';
				$loc_emp_subdivision 	= '';
				$loc_emp_areatype		= '';
				$emp_name_areatype		= '';
				$loc_emp_vill_ward		= '';
				$l_emp_ps				= '';
				$loc_emp_pin_number		= '';
			
		   }
			$form['clra_reg_master_edit']['address_principal_emp']	= array(
																			'#prefix' 	=> '<div id="add_line_edit_replace">',
																			'#suffix' 	=> '</div>',
																			);
			$form['clra_reg_master_edit']['emp_state']				= array(
																			'#prefix' 	=> '<div id="clra_emp_state_replace">',
																			'#suffix' 	=> '</div>',
																			);
																																					
			$form['clra_reg_master_edit']['emp_dist']				= array(
																			'#prefix' 	=> '<div id="clra_emp_district_replace">',
																			'#suffix' 	=> '</div>',
																			);	
																		
			$form['clra_reg_master_edit']['loc_emp_subdv']			= array(
																			'#prefix' 	=> '<div id="clra_subdvsn_replace_2">',
																			'#suffix' 	=> '</div>',
																			);	
																		
			$form['clra_reg_master_edit']['loc_emp_areatype']		= array(
																			'#prefix' 	=> '<div id="clra_loc_e_areatype_2">',
																			'#suffix' 	=> '</div>',
																			);
																		
			$form['clra_reg_master_edit']['emp_name_areatype']		= array(
																			'#prefix' 	=> '<div id="clra_common_1_replace_2">',
																			'#suffix' 	=> '</div>',
																			);
																		
			$form['clra_reg_master_edit']['loc_emp_vill_ward']		= array(
																			'#prefix' 	=> '<div id="clra_common_2_replace_2">',
																			'#suffix' 	=> '</div>',
																			);
																		
			$form['clra_reg_master_edit']['loc_emp_ps']				= array(
																			'#prefix' 	=> '<div id="clra_ps_replace_2">',
																			'#suffix' 	=> '</div>',
																			);	
																		
			$form['clra_reg_master_edit']['loc_emp_pin_number']		= array(
																			'#prefix' 	=> '<div id="clra_pin_replace_2">',
																			'#suffix' 	=> '</div>',
																			);																																	
		
			if($value_emp_country != '' && $value_emp_country ==  1) { 
				
				$form['clra_reg_master_edit']['address_principal_emp'] 	= array(
																		'#prefix' 			=> '<div id="add_line_edit_replace" class="col-md-8 min-height-80"><label class="input">',
																		'#suffix' 			=> '</label></div>',
																		'#title'			=> 'House/Flat No./Street No./Village/Locality Name etc'.$value_emp_country,
																		'#type' 			=> 'textfield',
																		'#required' 		=> TRUE,
																		'#default_value' 	=> !empty($address_principal_emp) ? $address_principal_emp : '',
																		'#attributes'		=> $readonly_address_principal_emp,
																		'#element_validate' => array('MISC_TEXTBOX_LOCATION_VALIDATE')
																		);
				$form['clra_reg_master_edit']['emp_state']			= array(
																		'#prefix' 			=> '<div id="clra_emp_state_replace" class="col-lg-4 col-md-6"><label class="select">',
																		'#suffix' 			=> '</label></div>',
																		'#title'			=> t('Select State'),
																		'#type' 			=> 'select',
																		'#options' 			=> get_all_state($value_emp_country),
																		'#attributes'		=> $readonly_address_principal_emp,
																		'#default_value'	=> !empty($loc_emp_state) ? $loc_emp_state : '',
																		'#required' 		=> TRUE,
																		'#ajax'				=> array(
																								'event'		=> 'change',
																								'effect' 	=> 'fade',
																								'callback'	=> 'get_ajax_edit_country_emp',
																								'progress'	=> array('type'=> 'throbber'),
																								)
																		);	
			$value_emp_state	= isset($form_state['values']['emp_state'])? $form_state['values']['emp_state'] : $loc_emp_state;
				if($value_emp_state != $loc_emp_state ){
					$loc_emp_dist 			= '';
					$loc_emp_state 			= '';
					$loc_emp_subdivision	= '';
					$loc_emp_areatype		= '';
					$emp_name_areatype		= '';
					$loc_emp_vill_ward		= '';
					$l_emp_ps				= '';
					$loc_emp_pin_number		= '';
					
				}
				if(($value_emp_country != '' && (!empty($value_emp_state) || $value_emp_state != '' )  )){
			
					$db_district_value	= db_query("SELECT COUNT(id) FROM {district_master} WHERE state_id = '".$value_emp_state."'")->fetchObject();	
				
					if(!empty($db_district_value) && ($db_district_value->count>0) ) {
							
						$form['clra_reg_master_edit']['emp_dist']		= array(
																			'#prefix' 			=> '<div id="clra_emp_district_replace" class="col-lg-4 col-md-6"><label class="select">',
																			'#suffix' 			=> '</label></div>',
																			'#title'			=> t('Select District'),
																			'#type' 			=> 'select',
																			'#options' 			=> get_district_by_id($value_emp_state),
																			'#default_value' 	=> !empty($loc_emp_dist) ? $loc_emp_dist : '',
																			'#attributes'		=> $readonly_address_principal_emp,
																			'#required' 		=> TRUE,
																			'#ajax'				=> array(
																											'event'		=> 'change',
																											'effect' 	=> 'fade',
																											'callback'	=> 'get_ajax_edit_country_emp',
																											'progress'	=> array('type'=> 'throbber'),
																										)
																	);	
																												
						$value_loc_e_dist_2		= isset($form_state['values']['emp_dist']) ? $form_state['values']['emp_dist'] : $loc_emp_dist;
						
						if($value_loc_e_dist_2 != $loc_emp_dist ){
						
							$loc_emp_subdivision	= '';
							$loc_emp_areatype		= '';
							$emp_name_areatype		= '';
							$loc_emp_vill_ward		= '';
							$l_emp_ps				= '';
							$loc_emp_pin_number		= '';
						
						}
					}
				}
				
				if($value_loc_e_dist_2 != ''  && $value_emp_state!=''){
	
					$db_subdivision_value	= db_query("SELECT COUNT(id) as subdv_total FROM {sub_division} WHERE district_code = '".$value_loc_e_dist_2."'")->fetchObject();
						
					if(!empty($db_subdivision_value) && $db_subdivision_value != '' && $db_subdivision_value->subdv_total > 0) {
								
						$form['clra_reg_master_edit']['loc_emp_subdv'] = array(
																	  '#prefix' 				=> '<div id="clra_subdvsn_replace_2" class="col-lg-4 col-md-6"><label class="select">',
																	  '#suffix' 				=> '</label></div>',
																	  '#title'					=> 'Select Subdivision',
																	  '#type' 					=> 'select',
																	  '#default_value' 			=> !empty($loc_emp_subdivision) ? $loc_emp_subdivision : '',
																	  '#options' 				=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist_2),
																	  '#attributes'				=> $readonly_address_principal_emp,
																	  '#required' 				=> TRUE,
																	  '#ajax'					=> array(
																									'event' 	=> 'change',
																									'effect' 	=> 'fade',			
																									'callback'	=> 'get_ajax_edit_country_emp',
																									'progress'	=> array('type'=> 'throbber'),
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
			
				if($value_loc_e_dist_2 != "" && $value_emp_country != '' && $value_loc_e_subdivision_2 != ""){
							
			 		$db_block_mun_value = db_query("SELECT COUNT(id) as areatype_total FROM {block_mun_master} WHERE distict_code = '".$value_loc_e_dist_2."' and sub_division_code  = '".$value_loc_e_subdivision_2."'")->fetchObject();
			 		if(!empty($db_block_mun_value) &&  $db_block_mun_value != '' && $db_block_mun_value->areatype_total > 0){
							
				 		$areatypeOptions	 = common_custom_user_areatype_list_new($value_loc_e_dist_2, $value_loc_e_subdivision_2);
								
						 if(!empty($areatypeOptions)){
											
							$form['clra_reg_master_edit']['loc_emp_areatype'] = array(
																					  '#prefix' 				=> '<div id="clra_loc_e_areatype_2" class="col-lg-4 col-md-6"><label class="select">',
																					  '#suffix' 				=> '</label></div>',
																					  '#title'					=> 'Select Block / Municipality / Corporation / SEZ / Notified Area',
																					  '#type' 					=> 'select',
																					  '#options' 				=> $areatypeOptions,
																					  '#required' 				=> TRUE,
																					  '#attributes'				=> $readonly_address_principal_emp,
																					  '#default_value' 			=> !empty($loc_emp_areatype) ? $loc_emp_areatype : '',
																					  '#ajax'					=> array(
																													'event' 			=> 'change',
																													'effect' 			=> 'fade',			
																													'callback'			=> 'get_ajax_edit_country_emp',
																													'progress'			=> array('type'=> 'throbber'),
																												  ),
																						);
																													
							$value_loc_e_areatype_2 				= isset($form_state['values']['loc_emp_areatype']) ? $form_state['values']['loc_emp_areatype'] : $loc_emp_areatype;
							
							if($value_loc_e_subdivision_2 != $loc_emp_subdivision && $value_loc_e_areatype_2 != $loc_emp_areatype){
								$emp_name_areatype		= '';
								$loc_emp_vill_ward		= '';
								$l_emp_ps				= '';
								$loc_emp_pin_number		= '';
							}
						 }
			 		}
				}
			
				if ($value_loc_e_dist_2 != "" && $value_loc_e_subdivision_2 != "" && $value_loc_e_areatype_2 != "") {
							
					if ($value_loc_e_areatype_2 == 'B' ){
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
			
			
					$form['clra_reg_master_edit']['emp_name_areatype'] 	 = array(
																		  '#prefix' 				=> '<div id="clra_common_1_replace_2" class="col-lg-4 col-md-6"><label class="select">',
																		  '#suffix' 				=> '</label></div>',
																		  '#title'					=> 'Select '.$areatype_name,
																		  '#type' 					=> 'select',
																		  '#options' 				=> common_custom_user_block_list_new($value_loc_e_subdivision_2, $value_loc_e_areatype_2),
																		  '#required' 				=> TRUE,
																		  '#default_value' 			=> !empty($emp_name_areatype) ? $emp_name_areatype : '',
																		  '#attributes'				=> $readonly_address_principal_emp,
																		  '#ajax'					=> array(
																											'event' 			=> 'change',
																											'effect' 			=> 'fade',			
																											'callback'			=> 'get_ajax_edit_country_emp',
																											'progress'			=> array('type'=> 'throbber'),
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
								
							
					$form['clra_reg_master_edit']['loc_emp_vill_ward'] 	= array(
																			  '#prefix' 				=> '<div id="clra_common_2_replace_2" class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select '.$area_name,
																			  '#type' 					=> 'select',
																			  '#options'				=> common_custom_user_villward_list_new($value_common_selection_1_2,$area_code),
																			  '#attributes' 			=> $readonly_address_principal_emp,
																			  '#required' 				=> TRUE,
																			  '#default_value' 			=> !empty($loc_emp_vill_ward) ? $loc_emp_vill_ward : '',
																			  '#ajax'					=> array(
																												'event' 			=> 'change',
																												'effect' 			=> 'fade',			
																												'callback'			=> 'get_ajax_edit_country_emp',
																												'progress'			=> array('type'=> 'throbber'),
																									  )
																			);	
																										
					$value_common_selection_2_2 = isset($form_state['values']['loc_emp_vill_ward']) ? $form_state['values']['loc_emp_vill_ward'] : $loc_emp_vill_ward;	
						
					if($value_common_selection_2_2 != $loc_emp_vill_ward){
						$l_emp_ps				= '';
						$loc_emp_pin_number		= '';
					}
				}
				
				if (!empty($value_emp_state) && $value_loc_e_dist_2 != "" && $value_loc_e_subdivision_2 != "" && $value_loc_e_areatype_2 && $value_common_selection_1_2 != "" && $value_common_selection_2_2 != "" ){
					$form['clra_reg_master_edit']['loc_emp_ps'] 		= array(
																			  '#prefix' 				=> '<div id="clra_ps_replace_2" class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select Police Station',
																			  '#type' 					=> 'select',
																			  '#attributes'				=> $readonly_address_principal_emp,
																			  '#options' 				=> common_custom_user_ps_list_new($value_loc_e_dist_2),
																			  '#required' 				=> TRUE,
																			  '#default_value' 			=> !empty($l_emp_ps) ? $l_emp_ps : '',
																		  );
																						  
																						  
					$form['clra_reg_master_edit']['loc_emp_pin_number'] = array(
																			  '#prefix' 				=> '<div id="clra_pin_replace_2" class="col-lg-4 col-md-6"><label class="input">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Pin Code',
																			  '#type' 					=> 'textfield',
																			  '#default_value' 			=> !empty($loc_emp_pin_number) ? $loc_emp_pin_number : '',
																			  '#attributes' 			=> $readonly_loc_emp_pin_number,
																			  '#required' 				=> TRUE,
																			  '#element_validate' 		=> array('MISC_PIN_NUMBER')
																		  );	
				}														
																	
		
		}
			
			else{
				
				$form['clra_reg_master_edit']['address_principal_emp'] = array(
																	   '#prefix' 			=> '<div id="add_line_edit_replace" class="col-md-12"><label class="textarea">',
																	   '#suffix' 			=> '</label></div>',
																	   '#title'				=> 'Address [ If other Country/State please provide detail address ]',
																	   '#rows'				=> t('2'),	
																	   '#type' 				=> 'textarea',
																	   '#attributes'		=> $readonly_address_principal_emp,
																	   '#default_value' 	=> !empty($address_principal_emp) ? $address_principal_emp : '',
																	   '#required' 			=> TRUE,
																	   '#element_validate'	=> array('MISC_TEXTBOX_LOCATION_VALIDATE')
																	);
			}
			$form['clra_reg_master_edit']['markup3'] = array(
																			'#type' 	=> 'markup',
																			'#markup' 	=> '</div></div>',
																			);
																		
		
		}

		if($man_details != 1){		
			$form['clra_reg_master_edit']['full_name_manager'] = array(
													'#prefix' 		=> '<div class="panel panel-default panel-form"><div class="panel-heading">4. Full name and address of the Manager or Person responsible for the supervision and control of the Establishment</div><div class="panel-body"><div class="col-md-8"><label class="input">',
													'#suffix' 		=> '</label></div>',
													'#title'		=> 'Full name of the Manager or Person Responsible',
													'#type' 		=> 'textfield',
													'#default_value'=> !empty($full_name_manager) ? $full_name_manager : '',
													'#attributes'	=> $readonly_full_name_manager, 
													'#required' 	=> TRUE,
													'#element_validate'	=> array('MISC_TEXTBOX_TEXT_VALIDATE')
													);
			$form['clra_reg_master_edit']['manager_country'] = array(
													'#prefix' 		=> '<div class="col-md-4"><label class="select">',
													'#suffix' 		=> '</label></div>',
													'#title'		=> 'Select Country',
													'#type' 		=> 'select',
													'#options' 		=> array('1' => 'India', '2' => 'Others'),
													'#default_value'=> !empty($loc_manager_country) ? $loc_manager_country : '',
													'#attributes' 	=> $readonly_address_manager,
													'#required' 	=> TRUE,
													'#ajax'			=> array(
																		'event' 		=> 'change',
																		'effect' 		=> 'fade',			
																		'callback'		=> 'get_ajax_country_manager',
																		'progress'		=> array('type'=> 'throbber'),
																		)
													);
																	
			$value_manager_country 		= isset($form_state['values']['manager_country']) ? $form_state['values']['manager_country'] : $loc_manager_country;
			
			$form['clra_reg_master_edit']['address_manager']					= array(
																					'#prefix' 				=> '<div id="clra_manager_address_replace">',
																					'#suffix' 				=> '</div>',
																					);
		
			$form['clra_reg_master_edit']['manager_state']						= array(
																					'#prefix' 				=> '<div id="clra_manager_state_replace">',
																					'#suffix' 				=> '</div>',
																					);
																		
			$form['clra_reg_master_edit']['manager_dist']						= array(
																					'#prefix' 				=> '<div id="clra_manager_district_replace">',
																					'#suffix' 				=> '</div>',
																					);
																		
			$form['clra_reg_master_edit']['loc_manager_subdv']					= array(
																						'#prefix' 				=> '<div id="clra_subdvsn_replace_3">',
																						'#suffix' 				=> '</div>'
																						);
																		
			$form['clra_reg_master_edit']['loc_manager_areatype']				= array(
																						'#prefix' 				=> '<div id="clra_loc_e_areatype_3">',
																						'#suffix' 				=> '</div>',
																						);
																		
			$form['clra_reg_master_edit']['manager_name_areatype']				= array(
																						'#prefix' 				=> '<div id="clra_common_1_replace_3">',
																						'#suffix' 				=> '</div>',
																						);
																		
			$form['clra_reg_master_edit']['loc_manager_vill_ward']				= array(
																						'#prefix' 				=> '<div id="clra_common_2_replace_3">',
																						'#suffix' 				=> '</div>',
																						);
																		
			$form['clra_reg_master_edit']['loc_manager_ps']						= array(
																						'#prefix' 				=> '<div id="clra_ps_replace_3">',
																						'#suffix' 				=> '</div>',
																						);
																		
			$form['clra_reg_master_edit']['loc_manager_pin_number']				= array(
																						'#prefix' 				=> '<div id="clra_manager_pin_replace">',
																						'#suffix' 				=> '</div>',
																						);
																		
																		
			if( (!empty($value_manager_country) && $value_manager_country != '') && $value_manager_country ==  1) {
			$db_manager_state_value			= db_query("SELECT COUNT(id) FROM {state_master} WHERE country_id = '".$value_manager_country."'")->fetchObject();
			
			$form['clra_reg_master_edit']['address_manager'] 				= array(
																			  '#prefix' 				=> '<div id="clra_manager_address_replace" class="col-lg-8 col-md-8"><label class="input">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'House/Flat No./Street No./Village/Locality Name etc',
																			  '#type' 					=> 'textfield',
																			  '#required' 				=> TRUE,
																			  '#default_value' 			=> !empty($address_manager) ? $address_manager : '',
																			  '#attributes'				=> $readonly_address_manager,
																			  '#element_validate' 		=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																			);
																			
			$form['clra_reg_master_edit']['manager_state']					= array(
																			 '#prefix' 					=> '<div id="clra_manager_state_replace" class="col-lg-4 col-md-6"><label class="select">',
																			 '#suffix' 					=> '</label></div>',
																			 '#title'					=> t('Select State'),
																			 '#type' 					=> 'select',
																			 '#options' 				=> get_all_state($value_manager_country),
																			 '#default_value'			=> !empty($loc_manager_state) ? $loc_manager_state : '',
																			 '#required' 				=> TRUE,
																			 '#attributes'				=> $readonly_address_manager,
																			 '#ajax'					=> array(
																												'event'		=> 'change',
																												'effect' 	=> 'fade',
																												'callback'	=> 'get_ajax_manager_amendment',
																												'progress'	=>	'',
																												)
																			);
																			
			$value_manager_state	= isset($form_state['values']['manager_state'])? $form_state['values']['manager_state'] : $loc_manager_state;
				
				
			if($value_manager_state != $loc_manager_state ){
					
				$loc_manager_dist 			= '';
				$loc_manager_subdivision 	= '';
				$loc_manager_areatype		= '';
				$manager_name_areatype		= '';
				$loc_manager_vill_ward		= '';
				$l_manager_ps				= '';
				$loc_manager_pin_number		= '';
					
			}
			
			if(($value_manager_country != '' && (!empty($value_manager_state) || $value_manager_state != '' )  )){
	
				$db_manager_district_value					= db_query("SELECT COUNT(id) FROM {district_master} WHERE state_id = '".$value_manager_state."'")->fetchObject();	
		
				if(!empty($db_manager_district_value) && ($db_manager_district_value->count>0) ) {
					$form['clra_reg_master_edit']['manager_dist']			= array(
																				'#prefix' 				=> '<div id="clra_manager_district_replace" class="col-lg-4 col-md-6"><label class="select">',
																				'#suffix' 				=> '</label></div>',
																				'#title'				=> t('Select District'),
																				'#type' 				=> 'select',
																				'#options' 				=> get_district_by_id($value_manager_state),
																				'#default_value' 		=> !empty($loc_manager_dist) ? $loc_manager_dist : '',
																				'#required' 			=> TRUE,
																				'#attributes'			=> $readonly_address_manager,
																				'#ajax'					=> array(
																												'event'		=> 'change',
																												'effect' 	=> 'fade',
																												'callback'	=> 'get_ajax_manager_amendment',
																												'progress'	=>	'',
																												)
																				);	
					$value_loc_e_dist_3 	= 	isset($form_state['values']['manager_dist']) ? $form_state['values']['manager_dist'] : $loc_manager_dist;
					
					if($value_loc_e_dist_3 != $loc_manager_dist ){
					
						$loc_manager_subdivision 	= '';
						$loc_manager_areatype		= '';
						$manager_name_areatype		= '';
						$loc_manager_vill_ward		= '';
						$l_manager_ps				= '';
						$loc_manager_pin_number		= '';
	
					
					}
				}
			}
		
			if($value_loc_e_dist_3 != ''  && !empty($value_manager_state)){
				$db_manager_subdivision_value		= db_query("SELECT COUNT(id) as subdv_total_manager  FROM {sub_division} WHERE district_code = '".$value_loc_e_dist_3."'")->fetchObject();
				if(!empty($db_manager_subdivision_value) && $db_manager_subdivision_value != '' && $db_manager_subdivision_value->subdv_total_manager > 0) {
					$form['clra_reg_master_edit']['loc_manager_subdv'] 		= array(
																				  '#prefix' 				=> '<div id="clra_subdvsn_replace_3" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 				=> '</label></div>',
																				  '#title'					=> 'Select Subdivision',
																				  '#type' 					=> 'select',
																				  '#options' 				=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist_3),
																				  '#default_value' 			=> !empty($loc_manager_subdivision) ? $loc_manager_subdivision	: '',
																				  '#attributes'				=> $readonly_address_manager,
																				  '#required' 				=> TRUE,
																				  '#ajax'					=> array(
																													'event' 			=> 'change',
																													'effect' 			=> 'fade',			
																													'callback'			=> 'get_ajax_manager_amendment',
																													'progress'			=> '',
																												  ),
																	);
			
			
					$value_loc_e_subdivision_3 				= isset($form_state['values']['loc_manager_subdv']) ? $form_state['values']['loc_manager_subdv'] : $loc_manager_subdivision;
					
					if($value_loc_e_subdivision_3 != $loc_manager_subdivision ){
							
						$loc_manager_areatype		= '';
						$manager_name_areatype		= '';
						$loc_manager_vill_ward		= '';
						$l_manager_ps				= '';
						$loc_manager_pin_number		= '';
					
					}
				}
			}
			
			if($value_loc_e_dist_3 != "" && !empty($value_manager_state) && $value_loc_e_subdivision_3 != ""){
					
		  		$db_block_mun_manager_value	= db_query("SELECT COUNT(id) as areatype_manager_total FROM {block_mun_master} WHERE distict_code = '".$value_loc_e_dist_3."' and sub_division_code  = '".$value_loc_e_subdivision_3."'")->fetchObject();
					
		  		if(!empty($db_block_mun_manager_value) &&  $db_block_mun_manager_value != '' && $db_block_mun_manager_value->areatype_manager_total > 0){
						
		   			$areatypeOptions	= common_custom_user_areatype_list_new($value_loc_e_dist_3, $value_loc_e_subdivision_3);
						
		   			if(!empty($areatypeOptions)){
							
						$form['clra_reg_master_edit']['loc_manager_areatype']		= array(
																			  '#prefix' 				=> '<div id="clra_loc_e_areatype_3" class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select Block / Municipality / Corporation / SEZ / Notified Area',
																			  '#type' 					=> 'select',
																			  '#options' 				=> $areatypeOptions,
																			  '#default_value' 			=> !empty($loc_manager_areatype) ? $loc_manager_areatype : '',
																			  '#attributes'				=> $readonly_address_manager,
																			  '#required' 				=> TRUE,
																			  '#ajax'					=> array(
																											'event' 			=> 'change',
																											'effect' 			=> 'fade',			
																											'callback'			=> 'get_ajax_manager_amendment',
																											'progress'			=> '',
																										  ),
																				);	
																									
						$value_loc_e_areatype_3 			= isset($form_state['values']['loc_manager_areatype']) ? $form_state['values']['loc_manager_areatype'] : $loc_manager_areatype;
						
						if($value_loc_e_subdivision_3 != $loc_manager_subdivision && $value_loc_e_areatype_3 != $loc_manager_areatype){
										
							$manager_name_areatype		= '';
							$loc_manager_vill_ward		= '';
							$l_manager_ps				= '';
							$loc_manager_pin_number		= '';
						}
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
			
				$form['clra_reg_master_edit']['manager_name_areatype']		= array(
																				  '#prefix' 				=> '<div id="clra_common_1_replace_3" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 				=> '</label></div>',
																				  '#title'					=> 'Select '.$areatype_name,
																				  '#type' 					=> 'select',
																				  '#options' 				=> common_custom_user_block_list_new($value_loc_e_subdivision_3, $value_loc_e_areatype_3),
																				  '#default_value' 			=> !empty($manager_name_areatype) ? $manager_name_areatype : '',
																				  '#required' 				=> TRUE,
																				  '#attributes'				=> $readonly_address_manager,
																				  '#ajax'					=> array(
																												'event' 			=> 'change',
																												'effect' 			=> 'fade',			
																												'callback'			=> 'get_ajax_manager_amendment',
																												'progress'			=> '',
																											  )
																				);
																								
				$value_common_selection_1_3 				= isset($form_state['values']['manager_name_areatype']) ? $form_state['values']['manager_name_areatype'] : $manager_name_areatype;
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
						
				$form['clra_reg_master_edit']['loc_manager_vill_ward']		= array(
																				  '#prefix' 				=> '<div id="clra_common_2_replace_3" class="col-lg-4 col-md-6"><label class="select">',
																				  '#suffix' 				=> '</label></div>',
																				  '#title'					=> 'Select '.$area_name,
																				  '#type' 					=> 'select',
																				  '#options'				=> common_custom_user_villward_list_new($value_common_selection_1_3,$area_code),
																				  '#default_value' 			=> !empty($loc_manager_vill_ward) ? $loc_manager_vill_ward : '',
																				  '#required' 				=> TRUE,
																				  '#attributes'				=> $readonly_address_manager,
																				  '#ajax'					=> array(
																												'event' 			=> 'change',
																												'effect' 			=> 'fade',			
																												'callback'			=> 'get_ajax_manager_amendment',
																												'progress'			=> '',
																											  )
																				);	
																								
				$value_common_selection_2_3 				= isset($form_state['values']['loc_manager_vill_ward']) ? $form_state['values']['loc_manager_vill_ward'] : $loc_manager_vill_ward;	
				
				if($value_common_selection_2_3 != $loc_manager_vill_ward){
						
						$l_manager_ps				= '';
					$loc_manager_pin_number		= '';
				}
			}
		
			if (!empty($value_manager_state) && $value_loc_e_dist_3 != "" && $value_loc_e_subdivision_3 != "" && $value_loc_e_areatype_3 && $value_common_selection_1_3 != "" && $value_common_selection_2_3 != "" ){
				
				$form['clra_reg_master_edit']['loc_manager_ps'] 		= array(
																			  '#prefix' 				=> '<div id="clra_ps_replace_3" class="col-lg-4 col-md-6"><label class="select">',
																			  '#suffix' 				=> '</label></div>',
																			  '#title'					=> 'Select Police Station',
																			  '#type' 					=> 'select',
																			  '#id'						=> 'loc_e_ps_id_3',
																			  '#options' 				=> common_custom_user_ps_list_new($value_loc_e_dist_3),
																			  '#default_value' 			=> !empty($l_manager_ps) ? $l_manager_ps : '',
																			  '#attributes' 			=> $readonly_address_manager,
																			  '#required' 				=> TRUE
																		  );
																						  
				$form['clra_reg_master_edit']['loc_manager_pin_number']	 = array(
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
			$form['clra_reg_master_edit']['address_manager'] 				= array(
																				  '#prefix' 				=> '<div id="clra_manager_address_replace" class="col-md-12"><label class="textarea">',
																				  '#suffix' 				=> '</label></div>',
																				  '#title'					=> 'Address [ If other Country/State please provide detail address ]',
																				  '#rows'					=> t('2'),	 //array('placeholder' => 'Street Name/Landmark/Pin Code'),
																				  '#type' 					=> 'textarea',
																				  '#attributes' 			=> $readonly_address_manager_textarea,
																				  '#default_value' 			=>!empty($address_manager) ? $address_manager : '',
																				  '#required' 				=> TRUE,
																				  '#element_validate' 		=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																				);
			
		 }
		
		 $form['clra_reg_master_edit']['markup4']		= array(
																			'#type' 	=> 'markup',
																			'#markup' 	=> '</div></div>',
																			);
						
		
		}
		
		if($e_nature_of_work != 1 || $max_num_wrkmen_new !=1 || $e_num_of_workmen_per_or_reg_new != 1 || $e_num_of_workmen_temp_or_reg_new!=1 || $workmen_if_same_similar_kind_of_work_new !=  1 ){
			
			$form['clra_reg_master_edit']['markup8'] = array(
														'#type' 	=> 'markup',
														'#markup' 	=> '<div class="panel panel-default panel-form"><div class="panel-heading">5. Nature of work carried on in the establishment</div><div class="panel-body">',
														);	
			if($e_nature_of_work != 1){	
	
				$options_cont 	= 	contractor_works_multiSelect_options();
				$options_cont 	=  	array(0 => "Select Work") + $options_cont;
								
				if(!empty($resultChunk)){
					
					$form['clra_reg_master_edit']['e_nature_of_work'] 	= array(
																			'#prefix' 			=> '<div class="col-md-12"><label class="select">',
																			'#suffix' 			=> '</label></div>',
																			'#title'			=>'5.Nature of Work Carried on in the Establishment',
																			'#description'		=>'<span style="color:#ff0000;">Note: If multiple Nature of work to be selected, press Ctrl and select the options.</span>',
																			'#type' 			=> 'select',
																			'#id' 				=> 'e_nature_of_work_id',
																			'#options' 			=> $options_cont,
																			'#default_value' 	=> $resultChunk,
																			'#multiple' 		=> TRUE,
																			'#attributes' 		=> $readonly_e_nature_of_work,
																			'#required' 		=> TRUE,
													);
                                                                            
            }else{
                $form['clra_reg_master_edit']['e_nature_of_work'] = array(
																	'#prefix' 				=> '<div class="col-md-12"><label class="select">',
																	'#suffix' 				=> '</label></div>',
																	'#title'				=> '5.Nature of Work Carried on in the Establishment',
																	'#description'			=> '<span style="color:#ff0000;">Note: If multiple Nature of work to be selected, press Ctrl and select the options.</span>',
																	'#type' 				=> 'select',
																	'#id' 					=> 'e_nature_of_work_id',
																	'#options' 				=> $options_cont,
																	'#multiple' 			=> TRUE,
																	'#attributes' 			=> array('style'=>"", 'onchange' => 'nature_wrk_others(this.value)'),
																	'#required' 			=> TRUE
												);	
            }
    
            
            	$form['clra_reg_master_edit']['other_nature_of_work_value'] = array(
																	 '#prefix' 				=> '<div class="col-md-12" id ="edit_other_nature_work_id"><label class="input">',
																	 '#suffix' 				=> '</label></div>',
																	 '#title'				=> '5.i) Other Option for Nature of Work',
																	 '#default_value' 		=>  !empty($other_nature_wrk) ? $other_nature_wrk : '',
																	 '#id' 					=> 'other_value_nature_of_work_id',
																	 '#attributes' 			=> $readonly_other_nature_of_work_value,
																	 '#type' 				=> 'textfield',
																				
																			);
			}
		
			/*** Nature of Work Carried on in the Establishment end ***/
		
			if($max_num_wrkmen_new != 1){
																										
				$form['clra_reg_master_edit']['max_num_wrkmen']	= array(									
											'#prefix' 				=> '</br></br><div class="col-md-4"><label class="input">',
											'#suffix' 				=> '</label></div>',
											 '#title'				=> '5.a) Maximum Number of Workmen Employed Directly on any day in the Establishment',
											 '#type' 				=> 'textfield',
											 '#default_value' 		=> !empty($max_num_wrkmen) ? $max_num_wrkmen : '',
											 '#attributes' 			=> $readonly_max_num_wrkmen,
											 '#required' 			=> TRUE,
											 '#element_validate' 	=> array('MISC_TEXTBOX_NUMERIC_VALIDATE'),
											);
			}
																		
			if($e_num_of_workmen_per_or_reg_new != 1){													
				$form['clra_reg_master_edit']['e_num_of_workmen_per_or_reg'] = array(
											   '#prefix' 				=> '<div class="col-md-4"><label class="input">',
											  '#suffix' 				=> '</label></div>',
											  '#required' 			=> TRUE,
											  '#title'				=>'5.b) Number of Workmen Engaged as Permanent / Regular Workmen',
											  '#type' 				=> 'textfield',
											  '#attributes'			=> $readonly_e_num_of_workmen_per_or_reg,
											  '#default_value' 		=> !empty($e_num_of_workmen_per_or_reg) ? $e_num_of_workmen_per_or_reg : '',
											  '#element_validate' 	=> array('MISC_TEXTBOX_NUMERIC_VALIDATE'),
											);
			}
		
			if($e_num_of_workmen_temp_or_reg_new != 1){
																			
				$form['clra_reg_master_edit']['e_num_of_workmen_temp_or_reg'] = array(
											   '#prefix' 				=> '<div class="col-md-4"><label class="input">',
												'#suffix' 				=> '</label></div>',
											  '#title'				=>'5.c) Number of Workmen Engaged as Temporary / Casual Workmen',
											  '#type' 				=> 'textfield',
											  '#default_value' 		=> !empty($e_num_of_workmen_temp_or_reg) ? $e_num_of_workmen_temp_or_reg : '',
											  '#required' 			=> TRUE,
											  '#attributes' 		=> $readonly_e_num_of_workmen_temp_or_reg,
											  '#element_validate' 	=> array('MISC_TEXTBOX_NUMERIC_VALIDATE'),
											);
			}	
		
			if($workmen_if_same_similar_kind_of_work_new != 1){
																		
				$workmenOptions	= array(0=> t('No'), 1=> t('Yes'));
							
				$form['clra_reg_master_edit']['workmen_if_same_similar_kind_of_work'] = array(
										 '#prefix' 				=> '<div class="col-md-12 inline-group">',
										 '#suffix' 				=> '</div>',
										 '#type' 				=> 'radios',
										 '#title' 				=> '5.d) Whether the Workmen employed/intended to be Employment by the Contractor Perform the same or similar kind of work as the Workmen employed directly by the Principal Employer (if yes, please give here information as detailed below)',
										 '#options' 			=> $workmenOptions,
										 '#attributes' 			=> $readonly_workmen_if_same_similar_kind_of_work,
										 '#default_value'		=> isset($workmen_if_same_similar_kind_of_work) ? $workmen_if_same_similar_kind_of_work : 0,
										 '#required' 			=> TRUE,
										);
																				
			$form['clra_reg_master_edit']['con_lab_job_desc'] = array(
										 '#prefix' 			=> '<div class="col-md-4"><label class="textarea">',
										 '#suffix' 			=> '</label></div>',
										  '#title'				=>'5.d.i) a complete job description of the contractor labour',
										  '#type' 				=> 'textarea',
										  '#default_value' 		=> !empty($con_lab_job_desc) ? $con_lab_job_desc : '',
										  '#required' 			=> TRUE,
										  '#attributes' 		=> $readonly_con_lab_job_desc,
										  '#rows'				=>t('4'),	
										  '#element_validate' 	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
										);
																			
			$form['clra_reg_master_edit']['con_lab_wage_rate_other_benefits'] = array(
										  '#prefix' 			=> '<div class="col-md-4"><label class="input">',
										  '#suffix' 			=> '</label></div>',
										  '#title'				=>'5.d.ii) Wage rates and other cash benefits paid/to be paid',
										  '#type' 				=> 'textfield',
										  '#default_value' 		=> !empty($con_lab_wage_rate_other_benefits) ? $con_lab_wage_rate_other_benefits : '',
										  '#attributes' 		=> $readonly_con_lab_wage_rate_other_benefits,
										  '#rows'				=>t('2'),	
										  '#element_validate' 	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
										);
																			
			$form['clra_reg_master_edit']['con_lab_cat_desig_nom'] = array(
										  '#prefix' 			=> '<div class="col-md-4"><label class="input">',
										  '#suffix' 			=> '</label></div>',
										  '#title'				=>'5.d.iii) Category/designation/nomenclature of the job',
										  '#type' 				=> 'textfield',
										  '#default_value' 		=> !empty($con_lab_cat_desig_nom) ? $con_lab_cat_desig_nom : '',
										  '#attributes' 		=> $readonly_con_lab_cat_desig_nom,
										  '#rows'				=> t('2'),	
										  '#element_validate' 	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
									   );								
																			
			
			$form['clra_reg_master_edit']['e_settlement_award_judgement_min_wage'] = array(
										'#prefix' 			=> '<div class="col-md-8"><label class="input">',
										  '#suffix' 			=> '</label></div>',
										  '#title'				=>'5.e) Settlement or award or judgement or minimum wages (if any applicable in the establishment)',
										  '#type' 				=> 'textfield',
										  '#default_value' 		=> !empty($e_settlement_award_judgement_min_wage) ? $e_settlement_award_judgement_min_wage : '',
										  '#required' 			=> TRUE,
										  '#attributes'			=> $readonly_e_settlement_award_judgement_min_wage,
										  '#element_validate'	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
										);
			}
		
			$form['clra_reg_master_edit']['markup5']		= array(
																			'#type' 	=> 'markup',
																			'#markup' 	=> '</div></div>',
																			);
		}
		
		/**** Upload Documents Part ***/

	/** Trade License **/
		
		if ( $trade_license_file != "" ){
			
			$trade_license_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/trade_license/'.$trade_license_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
		}else{
			$trade_license_link = '';
		}
		$upload_value_1 = array( 'trade_license' => t('Trade License&nbsp;&nbsp;&nbsp;&nbsp;'.$trade_license_link ));
		
	
		/** Article of Association **/
		
		if ( $article_of_assoc_file != "" ){
			
			$article_of_assoc_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/article_of_assoc/'.$article_of_assoc_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
		}else{
			$article_of_assoc_link = '';
		}
		
		$upload_value_2 = array( 'article_of_assoc' => t('Articles of Association and Memorandum of Association/Partnership Deed&nbsp;&nbsp;&nbsp;&nbsp;'.$article_of_assoc_link));
		
		
		/**  Memorandum of Certificate **/
		
		if ( $memorandum_of_cert_file != "" ){
			
			$memorandum_of_cert_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/memorandum_of_cert/'.$memorandum_of_cert_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
		}else{
			$memorandum_of_cert_link = '';
		}
		
		$upload_value_3 = array( 'memorandum_of_cert' => t('Any other document in support of correctness of the particulars mentioned in the application if required&nbsp;&nbsp;&nbsp;&nbsp;'.$memorandum_of_cert_link));
		
		
		
		/**   Partnership Deed **/
		
		if ( $partnership_deed_file != "" ){
			
			$partnership_deed_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/partnership_deed/'.$partnership_deed_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
		}else{
			$partnership_deed_link = '';
		}
		
		$upload_value_4 = array( 'partnership_deed' => t('Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc.&nbsp;&nbsp;&nbsp;&nbsp;'.$partnership_deed_link));
		
				
		/**   Factory License  **/
		
		if ( $factory_license_file != "" ){
			
			$factory_license_link = l(t('View Document'),''.$GLOBALS['base_url'].'/sites/default/files/upload/FactoryLicense/'.$factory_license_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank' , 'class'=>array('#pdf-img'))));
			
		}else{
			$factory_license_link = '';
		}
		
		$upload_value_5 = array( 'factory_license' => t('Factory License if any&nbsp;&nbsp;&nbsp;&nbsp;'.$factory_license_link));
		
				
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
		
		$form['clra_reg_master_edit']['upload_check_in_1'] 	= array(
									   '#prefix' 	=> '<div class="panel panel-default panel-form"><div class="panel-heading">Upload Supporting Documents</div><div class="panel-body min-height-0"><section class="col-md-12"><label class="checkboxe">',
									  '#suffix' 	=> '</label></section>',																			 
									  '#type' 		=> 'checkboxes',
									  '#options' 	=> $upload_value_1,
									  '#attributes'	=> $readonly_trade_license
									);
		
							
		$form['clra_reg_master_edit']['upload_check_in_2'] 	= array(
									 '#prefix' 		=> '<section class="col-md-12"><label class="checkboxe">',
									  '#suffix' 	=> '</label></section>',
									  '#type' 		=> 'checkboxes',
									  '#options' 	=> $upload_value_2,
									  '#attributes'	=> $readonly_article_of_assoc
									);
												
		$form['clra_reg_master_edit']['upload_check_in_3'] 	= array(
									  '#prefix' 	=> '<section class="col-md-12"><label class="checkboxe">',
									  '#suffix' 	=> '</label></section>',
									  '#type' 		=> 'checkboxes',
									  '#options' 	=> $upload_value_3,
									  '#attributes'	=> $readonly_memorandum_of_cert
									);
												
		$form['clra_reg_master_edit']['upload_check_in_4'] 	= array(
										 '#prefix' 			=> '<section class="col-md-12"><label class="checkboxe">',
																					  '#suffix' 			=> '</label></section>',
									  '#type' 		=> 'checkboxes',
									  '#options' 	=> $upload_value_4,
									  '#attributes'	=> $readonly_partnership_deed,
									);
																		
		$form['clra_reg_master_edit']['upload_check_in_5'] 	= array(
		 '#prefix' 			=> '<section class="col-md-12"><label class="checkboxe">',
																					  '#suffix' 			=> '</label></section>',
									  '#type' 		=> 'checkboxes',
									  '#options' 	=> $upload_value_5,
									  '#attributes'	=> $readonly_factory_license,
									);
																		
		
		$form['clra_reg_master_edit']['row1'] = array(
									  '#markup' 	=> '<section class="col-md-6"  id="trade_license_div">',
									  ); 		
		$form['clra_reg_master_edit']['trade_license'] 	= array(
										'#type' 				=> 'managed_file',
										'#title'				=> '1. Upload Trade License',
										'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
										'#upload_location' 		=> 'public://upload/trade_license/',
										'#process' 				=> array('import_my_file_element_process'),
										'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
										'#prefix' 				=> '<label class="input input-file">',
										'#suffix' 				=> '</label>'										
										);
	
		$form['clra_reg_master_edit']['row2'] = array(
										'#markup' 			=> '</section><section class="col-md-6"  id="article_of_assoc_div">',
										); 
							
	    $form['clra_reg_master_edit']['article_of_assoc'] = array(
										'#type' 				=> 'managed_file',
										'#title'				=> '2. Upload Articles of Association and Memorandum of Association/Partnership Deed',
										'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
										'#upload_location' 		=> 'public://upload/article_of_assoc/',
										'#process' 				=> array('import_my_file_element_process'),
										'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
										'#prefix' 				=> '<label class="input input-file">',
										'#suffix' 				=> '</label>',
										);
							
							
		$form['clra_reg_master_edit']['row3'] = array(
										'#markup' 			=> '</section><section class="col-md-6"  id="memorandum_of_cert_div">',
										);
	
		$form['clra_reg_master_edit']['memorandum_of_cert'] = array(
										'#type' 				=> 'managed_file',
										'#title'				=> '3. Upload Any other document in support of correctness of the particulars mentioned in the application if required',
										'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
										'#upload_location' 		=> 'public://upload/memorandum_of_cert/',
										'#process' 				=> array('import_my_file_element_process'),
										'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
										'#prefix' 				=> '<label class="input-file">',
										'#suffix' 				=> '</label>'
										);		
		
		$form['clra_reg_master_edit']['row4'] =  array(
										'#markup' 			=> '</section><section class="col-md-6"  id="partnership_deed_div">',
										); 
		
		$form['clra_reg_master_edit']['partnership_deed'] = array(
										'#type' 				=> 'managed_file',
										'#title'				=>'4. Upload Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc.',
										'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
										'#upload_location' 		=> 'public://upload/partnership_deed/',
										'#process' 				=> array('import_my_file_element_process'),
										'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
										'#field_prefix' 		=> '<label class="input-file">',
										'#field_suffix' 		=> '</label>'
										);
																		
		$form['clra_reg_master_edit']['row5'] =  array(
										'#markup' 	=> '</section><section class="col-md-6" id="factory_license_div">',
										); 
																		  
		$form['clra_reg_master_edit']['factory_license'] = array(
										'#type' 				=> 'managed_file',
										'#title'				=>'5. Upload Factory License if any',
										'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
										'#upload_location' 		=> 'public://upload/FactoryLicense/',
										'#process' 				=> array('import_my_file_element_process'),
										'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
										'#field_prefix' 		=> '<label class="input-file">',
										'#field_suffix' 		=> '</label>'
										);

										$form['clra_reg_master_edit']['row6'] = array(
											'#markup' => '</section></div></div>');
            
	$form['clra_reg_master_edit']['submit'] = array(
			'#prefix' 			=> '', 
			'#type' 				=> 'submit',
			'#attributes' 		=> array('class' => array('btn btn-primary pull-right form-submit')),
			'#value' 				=> 'CONTINUE'
			);	

	$form['clra_reg_master_edit']['back_link'] 					= array(
										'#type' 			=> 'markup',
										'#suffix' 			=> '</div>', 
										'#markup'	    	=> l('<< BACK TO AMENDED FIELDS', 'apply-clra-reg-amendment/'.encryption_decryption_fun('encrypt', $application_id), array('attributes' => array('class' => 'link-btn'))));	

			
		return $form;
		
	//}else{
		//echo "Page Under Maintenance. Inconvenience regretted. Kindly do not make any changes in your Amendment Application."; die();
	//}
	}
}

function amendment_form_submit($form, &$form_state){ 
	global $base_root, $base_path, $user;
	 //print_r($form_state['values']);die;

	$exitrow = db_query("select id, status FROM l_clra_registration_master WHERE user_id = '".$user->uid."' order by id desc limit 1")->fetchObject();		
	
	if($exitrow->status != 'I'){	// Yes
		drupal_goto('applicant-dashboard');
	}else{
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
		//** For country and state values *****/
		$fetch_reg = db_select('l_clra_registration_master', 'lcrm');
		$fetch_reg->fields('lcrm', array('loc_emp_country','loc_emp_state','loc_manager_country','loc_manager_state'));
		$fetch_reg->condition('lcrm.user_id', trim($user_id));
		$fetch_reg->condition('lcrm.status','I');
		$fetch_reg->orderBy('lcrm.id','DESC');
		$fetch_reg->range(0, 1);
		$fetch_reg_det = $fetch_reg->execute()->fetchAssoc(); 	
				
		$last_master_application_id				= encryption_decryption_fun('decrypt', $val['application_id']);
		$act_id 								= encryption_decryption_fun('decrypt', $val['act_id']);
		$identification_number					= encryption_decryption_fun('decrypt', $val['identification_number']);
		
					
		$e_name 								= $val['e_name'];
		$est_type 								= $val['est_type'];
		$loc_e_name 							= $val['loc_e_name'];		
		$loc_e_dist 							= $val['loc_e_dist'];
		$loc_e_subdivision 						= $val['loc_e_subdivision'];
		$loc_e_areatype 						= $val['loc_e_areatype'];
		$loc_e_areatype_code 					= $val['loc_e_areatype_code'];
		$loc_e_vill_ward						= $val['loc_e_vill_ward'];
		$l_e_ps									= $val['loc_e_ps'];
		$loc_e_pin_number						= $val['loc_e_pin_number'];	
			
		$e_postal_address 						= $val['e_postal_address'];		
		$e_postal_dist 							= $val['e_postal_dist'];
		$e_postal_subdivision 					= $val['e_postal_subdivision'];
		$e_postal_areatype 						= $val['e_postal_areatype'];
		$e_postal_areatype_code 				= $val['e_postal_code'];
		$e_postal_vill_ward						= $val['e_postal_vill_ward'];
		$e_postal_ps							= $val['e_postal_ps'];
		$e_postal_pin_number					= $val['e_postal_pin_number'];	
			
		$full_name_principal_emp 				= $val['full_name_principal_emp'];
		$gender_pe								= $val['gender_pe'];
		$address_principal_emp 					= $val['address_principal_emp'];
		$loc_emp_country						= $val['emp_country'];
		$loc_emp_state							= $val['emp_state'];
		$loc_emp_dist 							= $val['emp_dist'];
		$loc_emp_subdivision 					= $val['loc_emp_subdv'];
		$loc_emp_areatype 						= $val['loc_emp_areatype'];
		$loc_emp_areatype_code 					= $val['emp_name_areatype'];
		$loc_emp_vill_ward 						= $val['loc_emp_vill_ward'];
		$l_emp_ps								= $val['loc_emp_ps'];
		$loc_emp_pin_number						= $val['loc_emp_pin_number'];
				
		$full_name_manager 						= $val['full_name_manager'];
		$address_manager 						= $val['address_manager'];
		$loc_manager_country					= $val['manager_country'];
		$loc_manager_state						= $val['manager_state'];	
		$loc_manager_dist 						= $val['manager_dist'];
		$loc_manager_subdivision 				= $val['loc_manager_subdv'];
		$loc_manager_areatype 					= $val['loc_manager_areatype'];
		$loc_manager_areatype_code 				= $val['manager_name_areatype'];
		$loc_manager_vill_ward 					= $val['loc_manager_vill_ward'];
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
		
		
		$fetch_reg_last_row	= db_query("select * from l_clra_registration_master where user_id =".$user->uid." and status = 'I' order by id desc limit 1");
		$obj3 = $fetch_reg_last_row->fetchAssoc();	
		
		
		
		$registration_number					= $obj3['registration_number'];
		$clra_qr_code							= $obj3['clra_qr_code'];
		$registration_date						= $obj3['registration_date'];
		//$certificates_fid						= $obj3['certificates_fid'];
		$amended_indentification_number			= 'CLRA-AMEND-'.$last_master_application_id.'-'.time();
		$backlog_id								= $obj3['backlog_id'];
		
		if(empty($e_name)) $e_name 				= $obj3['e_name'];
		if(empty($est_type)) $est_type 			= $obj3['est_type'];
		if(empty($loc_e_name)) $loc_e_name 		= $obj3['loc_e_name'];
		if(empty($loc_e_dist)) $loc_e_dist		= $obj3['loc_e_dist'];
		if(empty($loc_e_subdivision)) $loc_e_subdivision = $obj3['loc_e_subdivision'];
		if(empty($loc_e_areatype)) $loc_e_areatype 	= $obj3['loc_e_areatype'];
		if(empty($loc_e_areatype_code)) 
			$loc_e_areatype_code 				= $obj3['name_areatype'];
		if(empty($loc_e_vill_ward))
			 $loc_e_vill_ward 					= $obj3['loc_e_vill_ward'];
		if(empty($l_e_ps)) 
			$l_e_ps 							= $obj3['l_e_ps'];		
		if(empty($loc_e_pin_number)) 
			$loc_e_pin_number 					= $obj3['loc_e_pin_number'];
		if(empty($e_postal_address)) 
			$e_postal_address					= $obj3['e_postal_address'];
		if(empty($e_postal_dist)) 
			$e_postal_dist 						= $obj3['e_postal_dist'];
		if(empty($e_postal_subdivision)) 
			$e_postal_subdivision 				= $obj3['e_postal_subdivision'];
		if(empty($e_postal_areatype)) 
			$e_postal_areatype 					= $obj3['e_postal_areatype'];
		if(empty($e_postal_areatype_code)) 
			$e_postal_areatype_code 			= $obj3['e_postal_name_areatype'];
		if(empty($e_postal_vill_ward))
			 $e_postal_vill_ward 				= $obj3['e_postal_vill_ward'];
		if(empty($e_postal_ps))
			 $e_postal_ps 						= $obj3['e_postal_ps'];
		if(empty($e_postal_pin_number))
			 $e_postal_pin_number 				= $obj3['e_postal_pin_number'];
		if(empty($full_name_principal_emp)) 
			$full_name_principal_emp 			= $obj3['full_name_principal_emp'];
		if(empty($gender_pe)) 
			$gender_pe 							= $obj3['gender_pe'];
		if(empty($address_principal_emp)) 
			$address_principal_emp 				= $obj3['address_principal_emp'];
		if(empty($loc_emp_country)) 
			$loc_emp_country 					= $obj3['loc_emp_country'];
		if(empty($loc_emp_state)) 
			$loc_emp_state 						= $obj3['loc_emp_state'];
		if(empty($loc_emp_dist)) 
			$loc_emp_dist 						= $obj3['loc_emp_dist'];
		if(empty($loc_emp_subdivision)) 
			$loc_emp_subdivision 				= $obj3['loc_emp_subdivision'];
		if(empty($loc_emp_areatype)) 
			$loc_emp_areatype 					= $obj3['loc_emp_areatype'];
		if(empty($loc_emp_areatype_code)) 
			$loc_emp_areatype_code 				= $obj3['emp_name_areatype'];
		if(empty($loc_emp_vill_ward)) 
			$loc_emp_vill_ward 					= $obj3['loc_emp_vill_ward'];
		if(empty($l_emp_ps)) 
			$l_emp_ps 							= $obj3['l_emp_ps'];		
		if(empty($loc_emp_pin_number)) 
			$loc_emp_pin_number 				= $obj3['loc_emp_pin_number'];
		if(empty($full_name_manager))
			 $full_name_manager 				= $obj3['full_name_manager'];
		if(empty($address_manager)) 
			$address_manager 					= $obj3['address_manager'];
		if(empty($loc_manager_country)) 
			$loc_manager_country 			 	= $obj3['loc_manager_country'];
		if(empty($loc_manager_state)) 
			$loc_manager_state 			 		= $obj3['loc_manager_state'];
		if(empty($loc_manager_dist)) 
			$loc_manager_dist 					= $obj3['loc_manager_dist'];
		if(empty($loc_manager_subdivision)) 
			$loc_manager_subdivision 			= $obj3['loc_manager_subdivision'];
		if(empty($loc_manager_areatype)) 
			$loc_manager_areatype 				= $obj3['loc_manager_areatype'];
		if(empty($loc_manager_areatype_code)) 
			$loc_manager_areatype_code 			= $obj3['manager_name_areatype'];
		if(empty($loc_manager_vill_ward)) 
			$loc_manager_vill_ward 				= $obj3['loc_manager_vill_ward'];
		if(empty($l_manager_ps)) 
			$l_manager_ps 						= $obj3['l_manager_ps'];
		if(empty($loc_manager_pin_number)) 
			$loc_manager_pin_number 			= $obj3['loc_manager_pin_number'];
		if(empty($max_num_wrkmen)) 
			$max_num_wrkmen 					= $obj3['max_num_wrkmen'];
		if(empty($e_num_of_workmen_per_or_reg)) 
			$e_num_of_workmen_per_or_reg 		= $obj3['e_num_of_workmen_per_or_reg'];
		if(empty($e_num_of_workmen_temp_or_reg)) 
			$e_num_of_workmen_temp_or_reg 		= $obj3['e_num_of_workmen_temp_or_reg'];
		if(empty($workmen_if_same_similar_kind_of_work)) 
		$workmen_if_same_similar_kind_of_work 	= $obj3['workmen_if_same_similar_kind_of_work'];
		if(empty($con_lab_job_desc)) 
			$con_lab_job_desc 					= $obj3['con_lab_job_desc'];
		if(empty($con_lab_wage_rate_other_benefits)) 
			$con_lab_wage_rate_other_benefits 	= $obj3['con_lab_wage_rate_other_benefits'];
		if(empty($con_lab_cat_desig_nom)) 
			$con_lab_cat_desig_nom 				= $obj3['con_lab_cat_desig_nom'];
		if(empty($e_settlement_award_judgement_min_wage)) 
			$e_settlement_award_judgement_min_wage = $obj3['e_settlement_award_judgement_min_wage'];
		if(empty($e_any_day_max_num_of_workmen)) 
		$e_any_day_max_num_of_workmen 			 = $obj3['e_any_day_max_num_of_workmen'];
		
		$rloLwfsCode = get_rlo_lwfc_code($loc_e_vill_ward);
		$fieldsRegInfo 	= array( 
							'user_id' =>	$user->uid,
							'identification_number'	=>	$amended_indentification_number,
							'act_id' =>  $act_id,													
							'e_name' =>	$e_name,
							'est_type' =>  $est_type,
							'loc_e_name' =>	$loc_e_name,													
							'loc_e_dist' =>	$loc_e_dist,
							'loc_e_subdivision' => $loc_e_subdivision ,
							'loc_e_areatype' => $loc_e_areatype ,
							'name_areatype'	=>	$loc_e_areatype_code ,
							'loc_e_vill_ward' => $loc_e_vill_ward ,
							'l_e_ps' =>	$l_e_ps,
							'loc_e_pin_number' =>  $loc_e_pin_number,													
							'e_postal_address' => $e_postal_address,													
							'e_postal_dist'	=>	$e_postal_dist,
							'e_postal_subdivision' => $e_postal_subdivision ,
							'e_postal_areatype' => $e_postal_areatype,
							'e_postal_name_areatype' =>	$e_postal_areatype_code ,
							'e_postal_vill_ward' => $e_postal_vill_ward,
							'e_postal_ps' =>	$e_postal_ps,
							'e_postal_pin_number' => $e_postal_pin_number,									 
							'full_name_principal_emp' =>	$full_name_principal_emp,
							'gender_pe'	=>  $gender_pe, 
							'address_principal_emp' => $address_principal_emp,	
							'loc_emp_dist' =>	$loc_emp_dist,
							'loc_emp_subdivision' => $loc_emp_subdivision ,
							'loc_emp_areatype' => $loc_emp_areatype,
							'emp_name_areatype' => $loc_emp_areatype_code ,
							'loc_emp_vill_ward' => $loc_emp_vill_ward,
							'l_emp_ps' =>	$l_emp_ps,
							'loc_emp_pin_number' =>  $loc_emp_pin_number,													
							'full_name_manager' => $full_name_manager, 
							'address_manager' => $address_manager,													
							'loc_manager_dist' => $loc_manager_dist,
							'loc_manager_subdivision' =>	$loc_manager_subdivision ,
							'loc_manager_areatype' => $loc_manager_areatype,
							'manager_name_areatype' => $loc_manager_areatype_code ,
							'loc_manager_vill_ward' => $loc_manager_vill_ward,
							'l_manager_ps' =>	$l_manager_ps,
							'loc_manager_pin_number' =>  $loc_manager_pin_number,													
							'max_num_wrkmen' => $max_num_wrkmen, 
							'e_num_of_workmen_per_or_reg' => $e_num_of_workmen_per_or_reg, 
							'e_num_of_workmen_temp_or_reg' => $e_num_of_workmen_temp_or_reg, 
							'workmen_if_same_similar_kind_of_work' => $workmen_if_same_similar_kind_of_work, 
							'con_lab_job_desc' => $con_lab_job_desc, 
							'con_lab_wage_rate_other_benefits' => $con_lab_wage_rate_other_benefits, 
							'con_lab_cat_desig_nom' => $con_lab_cat_desig_nom ,													
							'e_settlement_award_judgement_min_wage'	=>  $e_settlement_award_judgement_min_wage,
							'e_any_day_max_num_of_workmen' =>	$e_any_day_max_num_of_workmen,
							'amendment_parent_id' => $last_master_application_id,	
							'registration_number' =>  $registration_number,
							'clra_qr_code' =>  $clra_qr_code,
							'registration_date'	=>  $registration_date,
							'backlog_id' =>	$backlog_id,
							'loc_emp_country' =>  $loc_emp_country,
							'loc_emp_state'	=>	$loc_emp_state,
							'loc_manager_country' => $loc_manager_country,
							'loc_manager_state'	=>  $loc_manager_state,
							'user_from_portal' =>  1,
							'rlo_code' => $rloLwfsCode->rlo_code,
							'lwfc_code' => $rloLwfsCode->lwfc_code																
						);
		
		
					
		//*** Update Queries ***//
		
				
		if(!empty($fieldsRegInfo) && $last_master_application_id!=''){
			
				$fetch_reg_det = db_query("select * from l_clra_registration_master where user_id =".$user->uid." and status != 'I' and status != 'R' order by id desc limit 1");				
				
				if($fetch_reg_det->rowCount() > 0 ){
					$query = db_update('l_clra_registration_master');
					$query->fields($fieldsRegInfo); 
					$query->condition('id',$last_master_application_id);
					$query->condition('user_id',$user_id);
					$query->condition('status', 'I', '!=');					
					$query->execute();
				
				}else{
					// insert	
					$new_application_id = db_insert('l_clra_registration_master')->fields($fieldsRegInfo)->execute();		
				}
				
				
				
				/** For Nature Of work ***/
				
				if(!empty($e_nature_of_work)){  
				
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
					
					
				}else{
					
					$get_reg_nature_work 		= db_select('l_clra_reg_nature_of_work', 'lcnw');
					$get_reg_nature_work		->leftJoin('contractor_works', 'cw', 'lcnw.e_nature_of_work=cw.contractor_work_id');
					$get_reg_nature_work		->fields('cw', array('contractor_work_id'));
					$get_reg_nature_work		->condition('lcnw.user_id',$user_id);
					//$get_reg_nature_work		->condition('cw.is_default','1');
					$get_reg_nature_work		->condition('lcnw.application_id',$last_master_application_id);
					$get_reg_nature_work_data 	= $get_reg_nature_work->execute(); 
					
					if(!empty($get_reg_nature_work_data )){
						
						foreach($get_reg_nature_work_data  as $listData){  
						
								$fieldsEstContractorInfoNatureOfWork 				= 	array( 
																											'identification_number'	=>	$amended_indentification_number,
																											'user_id'				=>	$user_id,
																											'act_id'				=>  $act_id,
																											'application_id'		=>  $new_application_id,
																											'e_nature_of_work'		=>	$listData->contractor_work_id,
																										);
																										
							db_insert('l_clra_reg_nature_of_work')->fields($fieldsEstContractorInfoNatureOfWork)->execute();
								
						}
					}
					
					
						
								
			

				}
				
				
				
				/**** DOC UPLOADED CODE *****/
				
				
				$fetch_reg_upload_files    = db_select('l_documents_upload','ldu');
				$fetch_reg_upload_files    ->fields('ldu', array('identification_number', 'trade_license_file', 'article_of_assoc_file', 'memorandum_of_cert_file', 'partnership_deed_file','factory_license_file'));
				$fetch_reg_upload_files    ->condition('ldu.application_id', $last_master_application_id);
				$fetch_reg_upload_files    ->condition('ldu.user_id',$user_id);
				$fetch_reg_upload_files    ->condition('ldu.act_id', $act_id);
				$fetch_upload_files_result = $fetch_reg_upload_files->execute();
				
				if( $fetch_upload_files_result->rowCount() > 0 ){
				
					$uploaded_files                             =   $fetch_upload_files_result->fetchAssoc();
					
					$trade_license_file_old						=	$uploaded_files['trade_license_file'];
					$article_of_assoc_file_old					=	$uploaded_files['article_of_assoc_file'];
					$memorandum_of_cert_file_old				=	$uploaded_files['memorandum_of_cert_file'];
					$partnership_deed_file_old					=	$uploaded_files['partnership_deed_file'];
					$factory_license_old						= 	$uploaded_files['factory_license_file'];
					
				
				}
				
				
				$fieldsDocumentsArray		= array(
												'user_id'				  		=>  $user_id,
												'act_id'				  		=>  $act_id,
												'identification_number'	  		=>  $amended_indentification_number,
												'trade_license_file' 	  		=>  isset($trade_license_file)&&($trade_license_file_old!=$trade_license_file) ? $trade_license_file : $trade_license_file_old,
												'article_of_assoc_file'   		=>  isset($article_of_assoc_file)&&($article_of_assoc_file_old!=$article_of_assoc_file) ? $article_of_assoc_file : $article_of_assoc_file_old,
												'memorandum_of_cert_file' 		=>  isset($memorandum_of_cert_file)&&($memorandum_of_cert_file_old!=$memorandum_of_cert_file) ? $memorandum_of_cert_file : $memorandum_of_cert_file_old,
												'partnership_deed_file'   		=>  isset($partnership_deed_file)&&($partnership_deed_file_old!=$partnership_deed_file) ? $partnership_deed_file : $partnership_deed_file_old,
												'factory_license_file' 	  		=>  isset($factory_license_file)&&($factory_license_old!=$factory_license_file) ? $factory_license_file : $factory_license_old,
												
												'application_id'		  		=>  $new_application_id,
				);
				
				db_insert('l_documents_upload')->fields($fieldsDocumentsArray)->execute();
				
				
				
				/* Trade Union Details ***/
				
				
				$get_trade_union = db_select('l_clra_reg_trade_union_master', 'lcrtu');
				$get_trade_union->fields('lcrtu', array());
				$get_trade_union->condition('lcrtu.user_id', trim($user->uid));
				$get_trade_union->condition('lcrtu.application_id', trim($last_master_application_id));
				
				$trade_union_details 	= $get_trade_union->execute();
				
				if( $trade_union_details->rowCount() > 0 ){
						
						while($trade_union = $trade_union_details->fetchObject()){
						
								$tradeUnionInsert		= array( 
														'user_id'				  =>  $user_id,
														'act_id'				  =>  $trade_union->act_id, 
														'e_trade_union_regn_no'	  =>  $trade_union->e_trade_union_regn_no,
														'e_trade_union_name'	  =>  $trade_union->e_trade_union_name,
														'e_trade_union_address'	  =>  $trade_union->e_trade_union_address,
														'application_id'		  =>  $new_application_id,
								);
								
								db_insert('l_clra_reg_trade_union_master')->fields($tradeUnionInsert)->execute();
						}
								
				}
			
				/*** Contractor Update & Insert Queries ****/
				
				$get_contractor_list = db_select('l_contractor_info_master','lcim');
				$get_contractor_list->fields('lcim', array());
				$get_contractor_list->condition('lcim.user_id', trim($user->uid));
				$get_contractor_list->condition('lcim.application_id', trim($last_master_application_id));
				$get_contractor_list->condition('lcim.identification_number', $identification_number);
				$contractor_details 	= $get_contractor_list->execute(); 
				
				if( $contractor_details->rowCount() > 0 ){ 
					
					while($contractor_details_row = $contractor_details->fetchAssoc()) {
						
						$contractor_insert =  array( 
												'application_id'								=>  $last_master_application_id,  // 95
												'user_id'										=>	$user->uid,
												'identification_number'							=>	$identification_number,
												'contractor_user_id'							=>	$contractor_details_row['contractor_user_id'],
												
												'name_of_contractor'							=>	$contractor_details_row['name_of_contractor'],
												'address_of_contractor'							=>	$contractor_details_row['address_of_contractor'],
												'email_of_contractor'							=>  $contractor_details_row['email_of_contractor'],
												
												'con_loc_e_dist'								=>	$contractor_details_row['con_loc_e_dist'],
												'con_loc_e_subdivision'							=>	$contractor_details_row['con_loc_e_subdivision'],
												'con_loc_e_areatype'							=>	$contractor_details_row['con_loc_e_areatype'],
												'con_name_areatype'								=>	$contractor_details_row['con_name_areatype'],
												'con_loc_e_vill_ward'							=>	$contractor_details_row['con_loc_e_vill_ward'],
												'con_l_e_ps'									=>	$contractor_details_row['con_l_e_ps'],
												'contractor_pin'								=>  $contractor_details_row['contractor_pin'],
												
												'contractor_max_no_of_labours_on_any_day'		=>	$contractor_details_row['contractor_max_no_of_labours_on_any_day'],
												'est_date_of_work_of_each_labour_from_date'		=>	$contractor_details_row['est_date_of_work_of_each_labour_from_date'], 
												'est_date_of_work_of_each_labour_to_date'		=>  $contractor_details_row['est_date_of_work_of_each_labour_to_date'],
												'est_date_of_work_of_each_labour_total_months'	=>  $contractor_details_row['est_date_of_work_of_each_labour_total_months'],
												'act_id'										=>	$contractor_details_row['act_id'], 
												'status'										=>	$contractor_details_row['status'],
												'contractor_type'								=>  $contractor_details_row['contractor_type'],
												'state_opts' 									=>  $contractor_details_row['state_opts'],
												'state_others'									=>	$contractor_details_row['state_others'],
												'contractor_parent_id'  						=>	$contractor_details_row['id'],
												
										); 
														
						
						$last_contractor_insert_id = db_insert('l_contractor_info_master')->fields($contractor_insert)->execute();	//INSERT Contractor	
																
						$contractor_update =  array( 
														'application_id'								=>  $new_application_id,  // 120
														'identification_number'							=>	$amended_indentification_number,
											    	);  
													
						$contractor_master_update = db_update('l_contractor_info_master');              //UPDATE Contractor
						$contractor_master_update->fields($contractor_update); 
						$contractor_master_update->condition('id',$contractor_details_row['id']);
						$contractor_master_update->condition('user_id',$user_id);
						$contractor_master_update->execute();
						
									
						
						$contractor_wrk 			= db_select('l_clra_con_nature_of_work', 'lccnw');
						$contractor_wrk				->fields('lccnw', array());
						$contractor_wrk				->condition('lccnw.user_id', trim($user->uid));
						$contractor_wrk				->condition('lccnw.contractor_id', $contractor_details_row['id']);
						$contractor_wrk				->condition('lccnw.identification_number', $identification_number);
						$contractor_wrk				->condition('lccnw.application_id', $last_master_application_id);
						$result_2 					= $contractor_wrk->execute();
						
						while($record_2 = $result_2->fetchAssoc()) {
							
															
								$con_nature_wrk_insert =  array( 
															'contractor_id'                 =>  $contractor_details_row['id'],
															'identification_number'			=>	$amended_indentification_number, 
															'user_id'						=>	$user->uid,
															'act_id'						=>	$act_id, 
															'application_id'				=>  $new_application_id,
															'contractor_nature_of_work'		=>	$record_2['contractor_nature_of_work'],
															);
															
								
													
								db_insert('l_clra_con_nature_of_work')->fields($con_nature_wrk_insert)->execute();
								
								
												
								$contractor_nature_wrk_update = db_update('l_clra_con_nature_of_work');              //UPDATE Contractor nature of work
								$contractor_nature_wrk_update->fields(array('contractor_id'   =>  $last_contractor_insert_id)); 
								$contractor_nature_wrk_update->condition('id',$record_2['id']);
								$contractor_nature_wrk_update->condition('user_id',$user_id);
								$contractor_nature_wrk_update->condition('application_id',$last_master_application_id);
								$contractor_nature_wrk_update->condition('identification_number',$identification_number);
								$contractor_nature_wrk_update->execute();
									
						}
					
					}
				}
		}
		
		
		
		
		if($new_application_id != ""){
			drupal_goto('clra-reg-amendment/view-clra-application/'.encryption_decryption_fun('encrypt', $new_application_id));
			
		}
		
		$message = 'Application Saved Successfully';
		drupal_set_message($message);
   }
}

function get_clra_registration_info_edit_form_ajax_callback($form, $form_state) {
	$commands = array();
	$commands[] = ajax_command_replace('#clra_subdvsn_replace', drupal_render($form['clra_reg_master_edit']['loc_e_subdivision']));
	$commands[] = ajax_command_replace('#clra_loc_e_areatype', drupal_render($form['clra_reg_master_edit']['loc_e_areatype']));
	$commands[] = ajax_command_replace('#clra_common_1_replace', drupal_render($form['clra_reg_master_edit']['loc_e_areatype_code']));
	$commands[] = ajax_command_replace('#clra_common_2_replace', drupal_render($form['clra_reg_master_edit']['loc_e_vill_ward']));
	$commands[] = ajax_command_replace('#clra_ps_replace', drupal_render($form['clra_reg_master_edit']['loc_e_ps']));
  	return array('#type' => 'ajax', '#commands' => $commands);
}

function get_clra_registration_e_postal_form_ajax_callback($form, $form_state){ 
	$commands = array(); 
	$commands[] = ajax_command_replace('#clra_e_postal_subdvsn_replace', drupal_render($form['clra_reg_master_edit']['e_postal_subdivision']));
	$commands[] = ajax_command_replace('#postal_loc_e_areatype', drupal_render($form['clra_reg_master_edit']['e_postal_areatype']));
	$commands[] = ajax_command_replace('#clra_e_postal_code_replace', drupal_render($form['clra_reg_master_edit']['e_postal_code']));
	$commands[] = ajax_command_replace('#clra_e_postal_villward_replace', drupal_render($form['clra_reg_master_edit']['e_postal_vill_ward']));
	$commands[] = ajax_command_replace('#clra_e_postal_ps_replace', drupal_render($form['clra_reg_master_edit']['e_postal_ps']));
  	return array('#type' => 'ajax', '#commands' => $commands);	
}



function get_ajax_edit_country_emp($form, $form_state){ 
	$commands = array();
	$commands[] = ajax_command_replace('#add_line_edit_replace', drupal_render($form['clra_reg_master_edit']['address_principal_emp']));
	$commands[] = ajax_command_replace('#clra_emp_state_replace', drupal_render($form['clra_reg_master_edit']['emp_state']));
	$commands[] = ajax_command_replace('#clra_emp_district_replace', drupal_render($form['clra_reg_master_edit']['emp_dist']));
	$commands[] = ajax_command_replace('#clra_subdvsn_replace_2', drupal_render($form['clra_reg_master_edit']['loc_emp_subdv']));
	$commands[] = ajax_command_replace('#clra_loc_e_areatype_2', drupal_render($form['clra_reg_master_edit']['loc_emp_areatype']));
	$commands[] = ajax_command_replace('#clra_common_1_replace_2', drupal_render($form['clra_reg_master_edit']['emp_name_areatype']));
	$commands[] = ajax_command_replace('#clra_common_2_replace_2', drupal_render($form['clra_reg_master_edit']['loc_emp_vill_ward']));
	$commands[] = ajax_command_replace('#clra_ps_replace_2', drupal_render($form['clra_reg_master_edit']['loc_emp_ps']));
	$commands[] = ajax_command_replace('#clra_pin_replace_2', drupal_render($form['clra_reg_master_edit']['loc_emp_pin_number']));
	
	return array('#type' => 'ajax', '#commands' => $commands);	
}

function get_ajax_country_manager ($form, $form_state){
	
	$commands[] = ajax_command_replace('#clra_manager_address_replace', drupal_render($form['clra_reg_master_edit']['address_manager']));
	$commands[] = ajax_command_replace('#clra_manager_state_replace', drupal_render($form['clra_reg_master_edit']['manager_state']));
	$commands[] = ajax_command_replace('#clra_manager_district_replace', drupal_render($form['clra_reg_master_edit']['manager_dist']));
	$commands[] = ajax_command_replace('#clra_subdvsn_replace_3', drupal_render($form['clra_reg_master_edit']['loc_manager_subdv']));
	$commands[] = ajax_command_replace('#clra_loc_e_areatype_3', drupal_render($form['clra_reg_master_edit']['loc_manager_areatype']));
	$commands[] = ajax_command_replace('#clra_common_1_replace_3', drupal_render($form['clra_reg_master_edit']['manager_name_areatype']));
	$commands[] = ajax_command_replace('#clra_common_2_replace_3', drupal_render($form['clra_reg_master_edit']['loc_manager_vill_ward']));
	$commands[] = ajax_command_replace('#clra_ps_replace_3', drupal_render($form['clra_reg_master_edit']['loc_manager_ps']));
	$commands[] = ajax_command_replace('#clra_manager_pin_replace', drupal_render($form['clra_reg_master_edit']['loc_manager_pin_number']));
	return array('#type' => 'ajax', '#commands' => $commands);
}

function get_ajax_manager_amendment($form, $form_state){
	$commands 	= array();
	$commands[] = ajax_command_replace('#clra_manager_state_replace', drupal_render($form['clra_reg_master_edit']['manager_state']));
	$commands[] = ajax_command_replace('#clra_manager_address_replace', drupal_render($form['clra_reg_master_edit']['address_manager']));
	$commands[] = ajax_command_replace('#clra_manager_district_replace', drupal_render($form['clra_reg_master_edit']['manager_dist']));
	$commands[] = ajax_command_replace('#clra_subdvsn_replace_3', drupal_render($form['clra_reg_master_edit']['loc_manager_subdv']));
	$commands[] = ajax_command_replace('#clra_loc_e_areatype_3', drupal_render($form['clra_reg_master_edit']['loc_manager_areatype']));
	$commands[] = ajax_command_replace('#clra_common_1_replace_3', drupal_render($form['clra_reg_master_edit']['manager_name_areatype']));
	$commands[] = ajax_command_replace('#clra_common_2_replace_3', drupal_render($form['clra_reg_master_edit']['loc_manager_vill_ward']));
	$commands[] = ajax_command_replace('#clra_ps_replace_3', drupal_render($form['clra_reg_master_edit']['loc_manager_ps']));
	$commands[] = ajax_command_replace('#clra_manager_pin_replace', drupal_render($form['clra_reg_master_edit']['loc_manager_pin_number']));
  	return array('#type' => 'ajax', '#commands' => $commands);
}

