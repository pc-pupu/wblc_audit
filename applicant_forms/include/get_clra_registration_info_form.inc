<?php 
drupal_add_js(drupal_get_path('module', 'applicant_forms') . '/js/upload_custom.js');

function get_clra_registration_info_form($form, &$form_state, $id='') {
	
	global $base_root, $base_path, $user;
	
	$user_id = $user->uid;	
	$action	= 'decrypt';
	$application_id = encryption_decryption_fun($action, $id);
	
	$fetch_reg_det = db_query("select * from l_clra_registration_master where user_id=:user_id and id=:application_id", array(':user_id' => $user_id ,':application_id' => $application_id));
			
				
	if($fetch_reg_det->rowCount() > 0 ){
				
		$obj2 = $fetch_reg_det->fetchAssoc();		
					
		$identification_number = $obj2['identification_number'];
		$act_id									=	$obj2['act_id'];
		
		$e_name									=	$obj2['e_name'];
		$est_type 						= 	$obj2['est_type'];
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
		$mobile_principal_emp					=	$obj2['mobile_principal_emp'];
		$address_principal_emp					=	$obj2['address_principal_emp'];
		
		$loc_emp_country						=   $obj2['loc_emp_country'];
		$loc_emp_state							=   $obj2['loc_emp_state'];
		$loc_emp_dist							=	$obj2['loc_emp_dist'];
		$loc_emp_subdivision					=	$obj2['loc_emp_subdivision'];
		$loc_emp_areatype 						=	$obj2['loc_emp_areatype'];
		$emp_name_areatype						=	$obj2['emp_name_areatype'];
		$loc_emp_vill_ward						=	$obj2['loc_emp_vill_ward'];
		$l_emp_ps								=	$obj2['l_emp_ps'];
		$loc_emp_pin_number						=   $obj2['loc_emp_pin_number'];
		$gender_pe 								= 	$obj2['gender_pe'];
		
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
			
		/**** DOC UPLOADED CODE STARTS*****/
						
		$fetch_reg_upload_files		=	db_query("select * from l_documents_upload where application_id=:application_id and user_id=:user_id",array(':user_id' => $user_id,':application_id' => $application_id ));
	
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
		
		$readonly_e_name 				= array('class' => array('form-control'));
		$readonly_loc_e_name 			= '';
		$readonly_e_postal_address 		= '';
		$readonly_e_postal_dist 		= '';
		$readonly_e_postal_subdivision 	= '';				
		$readonly_e_postal_areatype 	= '';
		$readonly_e_postal_areatype_code= '';
		$readonly_e_postal_vill_ward 	= '';
		$readonly_e_postal_ps 			= '';
		$readonly_address_principal_emp = '';
		$readonly_e_any_day_max_num_of_workmen =  array('id' => 'e_any_day_max_num_of_workmen_id');	
		
		if($backlog_id!=''){
			
			if(!empty($e_name)){ $readonly_e_name = array('disabled' => 'disabled', 'class' => array('readonly_view form-control'));}
			if(!empty($loc_e_name)){ $readonly_loc_e_name = array('disabled' => 'disabled', 'class' => array('readonly_view'));	}
			//if(!empty($e_postal_address)){ 				$readonly_e_postal_address				= array('readonly' => 'readonly', 'class' => array('readonly_view'));	}
			if(!empty($full_name_principal_emp)){ 			$readonly_full_name_principal_emp 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));  }
			// if(!empty($e_any_day_max_num_of_workmen)){ 	$readonly_e_any_day_max_num_of_workmen 	= array('disabled' => 'disabled', 'class' => array('readonly_view')); }
			if(!empty($address_principal_emp)){ 			$readonly_address_principal_emp 		= array('readonly' => 'readonly', 'class' => array('readonly_view')); }
			
			if ($loc_emp_country!='') { $loc_emp_country = $loc_emp_country ; } else{ $loc_emp_country = 1; }		
			if ($loc_emp_state!='') { $loc_emp_state = $loc_emp_state ; } else{ $loc_emp_state = 1; }
			if ($loc_manager_country!='') { $loc_manager_country = $loc_manager_country ; } else{ $loc_manager_country = 1; }
			if ($loc_manager_state!='') { $loc_manager_state = $loc_manager_state ; } else{ $loc_manager_state = 1; }
				
		}
		
		//if($application_id!='' ){
			
		 if($application_id!='' && ( $status =='B' || $final_submit_status =='')){
			
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
			
			$readonly_loc_e_pin_number 				= array('maxlength' => 6);
			$readonly_e_postal_pin_number			= array('maxlength' => 6);
			$readonly_loc_emp_pin_number			= array('maxlength' => 6);
			$readonly_loc_manager_pin_number		= array('maxlength' => 6);
			$readonly_e_nature_of_work 				= array('style'=>"height:120px;", 'onchange' => 'nature_wrk_others(this.value)');			
			$readonly_trade_license 				= array( 'id' => 'upload_check_in_1_id', 'onclick' => 'upload_check_fun_1()', $tlf => $tlf );
			$readonly_article_of_assoc 				= array( 'id' => 'upload_check_in_2_id', 'onclick' => 'upload_check_fun_2()', $asf => $asf );
			$readonly_memorandum_of_cert 			= array( 'id' => 'upload_check_in_3_id', 'onclick' => 'upload_check_fun_3()', $mcf => $mcf );
			$readonly_partnership_deed 				= array( 'id' => 'upload_check_in_4_id', 'onclick' => 'upload_check_fun_4()', $pdf => $pdf );
			$readonly_factory_license 				= array( 'id' => 'upload_check_in_5_id', 'onclick' => 'upload_check_fun_5()', $flf => $flf );
			$readonly_loc_e_subdivision 			= array('onkeyup' => 'getSpanText()', 'onblur' => 'getSpanText()', 'onchange' => 'getSpanText()');
			
			
			if($status == 'B'){
				
				
				$query 		= db_select('l_remarks_master', 'lrm')
						 	-> fields('lrm', array('remark_field_text'))
						 	-> condition('lrm.application_id', $application_id)
						 	-> orderBy('remark_id', 'DESC');
						 
				$remark_field_text_array =	$query->execute()->fetchAssoc();
				$field_arr 				 = explode(',', $remark_field_text_array['remark_field_text']);
				
				foreach($field_arr as $field_arr_val){
				
					if($field_arr_val == 'e_name'){	$readonly_e_name = array('disabled' => 'disabled', 'class' => array('readonly_view'));}
					if($field_arr_val == 'est_type'){	$readonly_estType = array('disabled' => 'disabled', 'class' => array('readonly_view'));}
				
					/*** Location Of the Establishment code for DISABLED fields ***/
				
					if($field_arr_val == 'loc_e_name'){
						$readonly_loc_e_name 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_loc_e_dist 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
						$readonly_loc_e_subdivision 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_loc_e_areatype 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_loc_e_areatype_code 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_loc_e_vill_ward 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
						$readonly_loc_e_ps 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_loc_e_pin_number 		= array('disabled' => 'disabled', 'maxlength' => 6, 'class' => array('readonly_view'));	
					}
				
					/*** Postal Address of the Establishment code for DISABLED fields ***/
					if($field_arr_val == 'e_postal_address'){
						$readonly_e_postal_address 		= array('readonly' => 'readonly', 'class' => array('readonly_view'));
						$readonly_e_postal_dist 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_e_postal_subdivision 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));				
						$readonly_e_postal_areatype 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_e_postal_areatype_code= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_e_postal_vill_ward 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_e_postal_ps 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_e_postal_pin_number	= array('disabled' => 'disabled', 'maxlength' => 6,  'class' => array('readonly_view'));
										
					}		
				
					if($field_arr_val == 'full_name_principal_emp'){	$readonly_full_name_principal_emp = array('disabled' => 'disabled', 'class' => array('readonly_view'));}
					if($field_arr_val == 'gender_pe'){	$readonly_gender_pe = array('disabled' => 'disabled', 'class' => array('readonly_view'));}
				
					/*** Address Of Principal Employer code for DISABLED fields ***/
				
					if($field_arr_val == 'address_principal_emp'){
					
						$readonly_address_principal_emp = array('readonly' => 'readonly', 'class' => array('readonly_view'),'placeholder' => 'Street Name/Landmark/Pin Code');
						$readonly_emp_country 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
						$readonly_emp_state 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
						$readonly_emp_dist 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
						$readonly_emp_subdv 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_emp_areatype 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_emp_areatype_name 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_emp_vill_ward 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_emp_ps 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_emp_pin_number		= array('disabled' => 'disabled','maxlength' => 6,  'class' => array('readonly_view'));						
					}
				
					if($field_arr_val == 'full_name_manager'){	$readonly_full_name_manager = array('disabled' => 'disabled', 'class' => array('readonly_view'));}	
				
					/*** Address Of Manager code for DISABLED fields ***/
					
					if($field_arr_val == 'address_manager'){
						$readonly_address_manager 			= array('readonly' => 'readonly', 'class' => array('readonly_view'), 'placeholder' => 'Street Name/Landmark/Pin Code');
						$readonly_manager_country 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_manager_state 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_manager_dist 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
						$readonly_loc_manager_subdv 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_loc_manager_areatype 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_loc_manager_areatype_code = array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_loc_manager_vill_ward 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
						$readonly_loc_manager_ps 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
						$readonly_loc_manager_pin_number 	= array('disabled' => 'disabled','maxlength' => 6,  'class' => array('readonly_view'));						
					}
				
					if($field_arr_val == 'e_nature_of_work'){ 
						$readonly_e_nature_of_work			 = array('disabed' => 'disabled','style' =>'height: 150px;', 'class' => array('readonly_view'));
						$readonly_other_nature_of_work_value = array('disabled' => 'disabled', 'class' => array('readonly_view'));
					}
					if($field_arr_val == 'max_num_wrkmen'){ $readonly_max_num_wrkmen = array('disabled' => 'disabled', 'class' => array('readonly_view'));} 
					if($field_arr_val == 'e_num_of_workmen_per_or_reg'){$readonly_e_num_of_workmen_per_or_reg = array('disabled' => 'disabled', 'class' => array('readonly_view'));}
					if($field_arr_val == 'e_num_of_workmen_temp_or_reg'){$readonly_e_num_of_workmen_temp_or_reg = array('disabled' => 'disabled', 'class' => array('readonly_view'));}	
					if($field_arr_val == 'workmen_if_same_similar_kind_of_work'){$readonly_workmen_if_same_similar_kind_of_work = array('disabled' => 'disabled', 'class' => array('readonly_view'));}	
					if($field_arr_val == 'con_lab_job_desc'){$readonly_con_lab_job_desc = array('readonly' => 'readonly', 'class' => array('readonly_view'));}	
					if($field_arr_val == 'con_lab_wage_rate_other_benefits'){$readonly_con_lab_wage_rate_other_benefits = array('disabled' => 'disabled', 'class' => array('readonly_view'));}	
					if($field_arr_val == 'con_lab_cat_desig_nom'){$readonly_con_lab_cat_desig_nom = array('disabled' => 'disabled', 'class' => array('readonly_view'));}	
					if($field_arr_val == 'e_settlement_award_judgement_min_wage'){$readonly_e_settlement_award_judgement_min_wage = array('disabled' => 'disabled', 'class' => array('readonly_view'));}
					if($field_arr_val == 'e_any_day_max_num_of_workmen'){ $readonly_e_any_day_max_num_of_workmen = array('disabled' => 'disabled', 'class' => array('readonly_view'));} 
					
					if($field_arr_val == 'trade_license_file'){ $readonly_trade_license = array('disabled' => 'disabled', 'class' => array());} 
					if($field_arr_val == 'article_of_assoc_file'){ $readonly_article_of_assoc = array('disabled' => 'disabled', 'class' => array());}
					if($field_arr_val == 'memorandum_of_cert_file'){ $readonly_memorandum_of_cert = array('disabled' => 'disabled', 'class' => array());}
					if($field_arr_val == 'partnership_deed_file'){ $readonly_partnership_deed = array('disabled' => 'disabled');} 	
					if($field_arr_val == 'factory_license_file'){ $readonly_factory_license = array('disabled' => 'disabled');} 			
				}
				
				
				
			}
			
			
			$options_dist 	= common_custom_user_district_list_new();
			
			$form['clra_reg_master_edit']['header']								 		= array(
																								'#markup' 			=>	 t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">1. Name and location of the Establishment <span id="rlo_location_id" class="form_note"></span></div><div class="panel-body">'),
																								);
																								
			$form['clra_reg_master_edit']['application_id'] 							= array(
																							  '#title'				=> 'Application Id',
																							  '#type' 				=> 'hidden',
																							  '#default_value'		=> !empty($application_id) ? encryption_decryption_fun('encrypt', $application_id) : '',
																							  '#attributes'			=> array('readonly'=>'readonly')
																							);
						
			$form['clra_reg_master_edit']['act_id'] 								    = array(
																							  '#title'				=> 'Act Id',
																							  '#type' 				=> 'hidden',
																							  '#default_value'		=> !empty($act_id) ? encryption_decryption_fun('encrypt', $act_id) : '',
																							  '#attributes' 		=> array('readonly'=>'readonly')
																							);
																	
			$form['clra_reg_master_edit']['identification_number'] 						= array(
																							  '#title'				=>'Identification Number',
																							  '#type' 				=> 'hidden',
																							  '#default_value'		=> !empty($identification_number) ? encryption_decryption_fun('encrypt', $identification_number) : '',
																							  '#attributes' 		=> array('readonly'=>'readonly')
																							 );
																							 
			$form['clra_reg_master_edit']['e_name'] 									= array(
																								'#prefix' 			=>  '<div class="col-md-4"><label class="input">',
																								'#suffix' 			=>  '</label></div>',
																								'#title'			=>  'Establishment Name',
																								'#type' 			=> 'textfield',
																								'#default_value' 	=> !empty($e_name) ? $e_name : '',
																								'#required' 		=> TRUE,
																								'#attributes' 		=> $readonly_e_name,
																								'#element_validate' => array('MISC_TEXTBOX_TEXT_ENAME_VALIDATE')
																								);
												
			$form['clra_reg_master_edit']['est_type'] 								= array(
																								'#title' 			=> 'Type of The Establishment',
																								'#type' 			=> 'select',
																								'#options' 			=> array('micro' => 'Micro', 'small' => 'Small', 'medium' => 'Medium', 'large' => 'Large'),
																								'#required' 		=>  TRUE,
																								'#default_value' 	=> $est_type,
																								'#attributes' 		=> $readonly_estType,
																								'#prefix' 			=> '<div class="col-md-4"><label class="select">',
																								'#suffix' 			=> '</label></div>'
																							);
			$form['clra_reg_master_edit']['loc_e_name'] 								= array(
																							  '#prefix' 			=>  '<div class="col-md-4"><label class="input">',
																							  '#suffix' 			=>  '</label></div>',
																							  '#title'				=> 'House No./Village Name/Street/Road',
																							  '#type' 				=> 'textfield',
																							  '#default_value' 		=> !empty($loc_e_name) ? $loc_e_name : '',
																							  '#required' 			=> TRUE,
																							  '#attributes' 		=> $readonly_loc_e_name,
																							  '#element_validate' 	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																							);
																							
			$form['clra_reg_master_edit']['loc_e_dist'] 								= array(
																							  '#prefix' 			=> '<div class="col-md-4"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select District',
																							  '#type' 				=> 'select',
																							  '#options' 			=> $options_dist,
																							  '#required' 			=> TRUE,
																							  '#attributes' 		=> $readonly_loc_e_dist,
																							  '#default_value' 		=> !empty($loc_e_dist) ? $loc_e_dist	:'',
																							  '#ajax'				=> array(
																														'event' 		=> 'change',
																														'effect' 		=> 'fade',			
																														'callback'		=> 'get_clra_registration_info_edit_form_ajax_callback',
																														'progress'		=> '',
																													  )
																							);
																							
			$value_loc_e_dist 		= 	isset($form_state['values']['loc_e_dist']) ? $form_state['values']['loc_e_dist'] : $loc_e_dist;
			
			if($value_loc_e_dist != "" || !empty($value_loc_e_dist) || is_numeric($value_loc_e_dist)){
				
				$form['clra_reg_master_edit']['loc_e_subdivision'] 						= array(
																							  '#prefix' 			=> '<div id="clra_subdvsn_replace" class="col-md-4"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Subdivision',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist),
																							  '#default_value' 		=> !empty($loc_e_subdivision) ? $loc_e_subdivision	:'',
																							  '#required' 			=> TRUE,
																							  '#id'					=> 'loc_e_subdivision_id',
																							  '#attributes' 		=> $readonly_loc_e_subdivision,
																							  '#ajax'				=> array(
																														'event' 	=> 'change',
																														'effect' 	=> 'fade',			
																														'callback'	=> 'get_clra_registration_info_edit_form_ajax_callback',
																														'progress'	=> '',
																													  ),
																							);
				
				
			}else{
				
				$form['clra_reg_master_edit']['loc_e_subdivision'] 						= array(
																							  '#prefix' 			=> '<div id="clra_subdvsn_replace" class="col-md-4"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Subdivision',
																							  '#type' 				=> 'select',
																							  '#options' 			=> array(),
																							  '#required' 			=> TRUE,
																							  '#id'					=> 'loc_e_subdivision_id',
																							  '#attributes' 		=> $readonly_loc_e_subdivision,
																							  '#ajax'				=> array(
																														'event' 	=> 'change',
																														'effect' 	=> 'fade',			
																														'callback'	=> 'get_clra_registration_info_edit_form_ajax_callback',
																														'progress'	=> '',
																													  ),
																							);
				
			}
			
			$value_loc_e_subdivision 				= isset($form_state['values']['loc_e_subdivision']) ? $form_state['values']['loc_e_subdivision'] : $loc_e_subdivision;
			
			if($value_loc_e_dist != "" && $value_loc_e_subdivision != ""){
					
				$areatypeOptions					= common_custom_user_areatype_list_new($value_loc_e_dist, $value_loc_e_subdivision);
				
				$form['clra_reg_master_edit']['loc_e_areatype']  						= array(
																							  '#prefix' 			=> '<div class="col-md-4" id="clra_loc_e_areatype"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Block/Municipality/Corporation/SEZ/Notified Area',
																							  '#type' 				=> 'select',
																							  '#options' 			=> $areatypeOptions,
																							  '#default_value' 		=> !empty($loc_e_areatype) ? $loc_e_areatype	:'',
																							  '#attributes' 		=> $readonly_loc_e_areatype,
																							  '#required' 			=> TRUE,
																							  '#id'					=> 'loc_e_areatype_id',
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_clra_registration_info_edit_form_ajax_callback',
																														'progress'			=> '',
																													  ),
																								);
				
			}else{
				
				$form['clra_reg_master_edit']['loc_e_areatype']  						= array(
																							  '#prefix' 			=> '<div class="col-md-4" id="clra_loc_e_areatype"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Block/Municipality/Corporation/SEZ/Notified Area',
																							  '#type' 				=> 'select',
																							  '#options' 			=> array(),
																							  '#required' 			=> TRUE,
																							  '#id'					=> 'loc_e_areatype_id',
																							  '#attributes' 		=> $readonly_loc_e_areatype,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_clra_registration_info_edit_form_ajax_callback',
																														'progress'			=> '',
																													  ),
																								);
			}
			
			$value_loc_e_areatype 					= isset($form_state['values']['loc_e_areatype']) ? $form_state['values']['loc_e_areatype'] : $loc_e_areatype;
			
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
				
				$form['clra_reg_master_edit']['loc_e_areatype_code'] 					= array(
																							'#prefix' 				=> '<div id="clra_code_replace" class="col-md-4"><label class="select">',
																							'#suffix' 				=> '</label></div>',
																							'#title'				=> 'Select '.$areatype_name,
																							'#type' 				=> 'select',
																							'#required' 			=> TRUE,
																							'#options' 				=> common_custom_user_block_list_new($value_loc_e_subdivision, $value_loc_e_areatype),
																							'#default_value'		=> !empty($loc_e_areatype_code) ? $loc_e_areatype_code	:'',
																							'#attributes' 			=> $readonly_loc_e_areatype_code,
																							'#ajax'					=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_clra_registration_info_edit_form_ajax_callback',
																														'progress'			=> '',
																													  ),
																						  );
					
			}else{
				
				$form['clra_reg_master_edit']['loc_e_areatype_code'] 					= array(
																							'#prefix' 				=> '<div id="clra_code_replace" class="col-md-4"><label class="select">',
																							'#suffix' 				=> '</label></div>',
																							'#title'				=> 'Select Block',
																							'#type' 				=> 'select',
																							'#options' 				=> array(),
																							'#attributes' 			=> $readonly_loc_e_areatype_code,
																							'#required' 			=> TRUE,
																							'#ajax'					=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_clra_registration_info_edit_form_ajax_callback',
																														'progress'			=> '',
																													  ),
																						  );
				
				
			}
			
			$value_loc_e_areatype_code 		= isset($form_state['values']['loc_e_areatype_code']) ? $form_state['values']['loc_e_areatype_code'] : $loc_e_areatype_code;
			
			if($value_loc_e_dist != "" && $value_loc_e_subdivision != "" && $value_loc_e_areatype != ""  && $value_loc_e_areatype_code != "" ){
					
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
				
				$form['clra_reg_master_edit']['loc_e_vill_ward'] 						= array(
																							  '#prefix' 			=> '<div class="col-md-4" id="clra_loc_vill_replace"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select '.$area_name,
																							  '#type' 				=> 'select',
																							  '#options'			=> common_custom_user_villward_list_new($value_loc_e_areatype_code,$area_code),
																							  '#default_value'		=> !empty($loc_e_vill_ward) ? $loc_e_vill_ward : '',
																							  '#attributes' 		=> $readonly_loc_e_vill_ward,
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_clra_registration_info_edit_form_ajax_callback',
																															'progress'			=> '',
																														  )
																							);
					
			}else{
				
				$form['clra_reg_master_edit']['loc_e_vill_ward'] 						= array(
																							  '#prefix' 			=> '<div class="col-md-4" id="clra_loc_vill_replace"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select '.$area_name,
																							  '#type' 				=> 'select',
																							  '#options' 			=> array(),
																							  '#attributes' 		=> $readonly_loc_e_vill_ward,
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'get_clra_registration_info_edit_form_ajax_callback',
																															'progress'			=> '',
																														  )
																							);
				
			}
			
			$value_loc_vill_ward 	= isset($form_state['values']['loc_e_vill_ward']) ? $form_state['values']['loc_e_vill_ward'] : $loc_e_vill_ward;
				
			if ($value_loc_e_dist != "" && $value_loc_e_subdivision != "" && $value_loc_e_areatype != ""  && $value_loc_e_areatype_code != "" && $value_loc_vill_ward != "" ){
				
				$form['clra_reg_master_edit']['loc_e_ps'] 								= array(
																							  '#prefix' 			=> '<div class="col-md-4" id="clra_ps_replace"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Police Station',
																							  '#type' 				=> 'select',
																							  '#id'					=> 'loc_e_ps_id',
																							  '#options' 			=> common_custom_user_ps_list_new($value_loc_e_dist),
																							  '#default_value' 	 	=> !empty($loc_e_ps) ? $loc_e_ps :'',
																							  '#attributes' 		=> $readonly_loc_e_ps,
																							  '#required' 			=> TRUE
																						  );
				
			}else{
				
				$form['clra_reg_master_edit']['loc_e_ps'] 								= array(
																							  '#prefix' 			=> '<div class="col-md-4" id="clra_ps_replace"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Police Station',
																							  '#type' 				=> 'select',
																							  '#id'					=> 'loc_e_ps_id',
																							  '#attributes' 		=> $readonly_loc_e_ps,
																							  '#options' 			=> array(),
																							  '#required' 			=> TRUE
																						  );
				
			}
			
			$form['clra_reg_master_edit']['loc_e_pin_number'] 							= array(
																							  '#prefix' 			=> '<div class="col-md-4"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'PIN Number',
																							  '#type' 				=> 'textfield',
																							  '#default_value' 	 	=> !empty($loc_e_pin_number) ? $loc_e_pin_number:'',
																							  '#attributes' 		=> $readonly_loc_e_pin_number,
																							  '#required' 			=> TRUE,
																							  '#element_validate' 	=> array('MISC_PIN_NUMBER'),																							  
																						  );
																						  
			$form['clra_reg_master_edit']['e_postal_address'] 							= array(
																							  '#prefix' 			=> '</div></div><div class="panel panel-default panel-form"><div class="panel-heading">2. Registered Office address of the Establishment</div><div class="panel-body"><div class="col-md-4"><label class="textarea">',
																							  '#suffix' 			=>  '</label></div>',
																							  '#title'				=>  'House No./Village Name/Street/Road:',
																							  '#type' 				=>  'textarea',
																							  '#default_value' 		=> !empty($e_postal_address) ? $e_postal_address : '',
																							  '#attributes'			=> $readonly_e_postal_address,
																							  '#required' 			=>  TRUE,
																							  '#rows'				=> t('4'),
																							  '#element_validate' 	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																							);
																							
			$form['clra_reg_master_edit']['e_postal_dist'] 								= array(
																							  '#prefix' 			=> '<div class="col-md-4"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select District',
																							  '#type' 				=> 'select',
																							  '#options' 			=> $options_dist,
																							  '#default_value' 		=> !empty($e_postal_dist) ? $e_postal_dist	:'',
																							  '#required' 			=> TRUE,
																							  '#attributes'			=> $readonly_e_postal_dist,
																							  '#ajax'				=> array(
																														'event' 	=> 'change',
																														'effect' 	=> 'fade',			
																														'callback'	=> 'get_clra_registration_e_postal_form_ajax_callback',
																														'progress'	=> '',
																													  )
																							);	
																							
			$value_e_postal_dist 	= 	isset($form_state['values']['e_postal_dist']) ? $form_state['values']['e_postal_dist'] : $e_postal_dist;
			
			if($value_e_postal_dist != "" || !empty($value_e_postal_dist) || is_numeric($value_e_postal_dist)){
			
				$form['clra_reg_master_edit']['e_postal_subdivision'] 					= array(
																							  '#prefix' 			=> '<div id="clra_e_postal_subdvsn_replace" class="col-md-4"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Subdivision',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_subdivisionlist_list_new($value_e_postal_dist),
																							  '#required' 			=> TRUE,
																							  '#attributes' 		=> $readonly_e_postal_subdivision,
																							  '#default_value' 		=> !empty($e_postal_subdivision) ? $e_postal_subdivision	:'',
																							  '#ajax'				=> array(
																														'event' 	=> 'change',
																														'effect' 	=> 'fade',			
																														'callback'	=> 'get_clra_registration_e_postal_form_ajax_callback',
																														'progress'	=> '',
																														  ),
																						);	
				
			}else{
				
				$form['clra_reg_master_edit']['e_postal_subdivision'] 					= array(
																							  '#prefix' 			=> '<div id="clra_e_postal_subdvsn_replace" class="col-md-4"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Subdivision',
																							  '#type' 				=> 'select',
																							  '#options' 			=> array(),
																							  '#required' 			=> TRUE,
																							  '#attributes' 		=> $readonly_e_postal_subdivision,
																							  '#ajax'				=> array(
																														'event' 	=> 'change',
																														'effect' 	=> 'fade',			
																														'callback'	=> 'get_clra_registration_e_postal_form_ajax_callback',
																														'progress'	=> '',
																														  ),
																						);
			}
			
			$value_e_postal_subdivision 	= isset($form_state['values']['e_postal_subdivision']) ? $form_state['values']['e_postal_subdivision'] : $e_postal_subdivision;
			
			if($value_e_postal_dist != "" && $value_e_postal_subdivision != ""){
					
				$areatypeOptions			= common_custom_user_areatype_list_new($value_e_postal_dist, $value_e_postal_subdivision);
				
				$form['clra_reg_master_edit']['e_postal_areatype'] 						= array(
																							  '#prefix' 			=> '<div class="col-md-4" id="e_postal_areatype_replace"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Block/Municipality/Corporation/SEZ/Notified Area',
																							  '#type' 				=> 'select',
																							  '#options' 			=> $areatypeOptions,
																							  '#attributes' 		=> $readonly_e_postal_areatype,
																							  '#default_value' 		=> !empty($e_postal_areatype) ? $e_postal_areatype	:'',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_clra_registration_e_postal_form_ajax_callback',
																														'progress'			=> '',
																													  ),
																								);
				
			}else{
				$form['clra_reg_master_edit']['e_postal_areatype'] 						= array(
																							  '#prefix' 			=> '<div class="col-md-4" id="e_postal_areatype_replace"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Block/Municipality/Corporation/SEZ/Notified Area',
																							  '#type' 				=> 'select',
																							  '#options' 			=> array(),
																							  '#attributes' 		=> $readonly_e_postal_areatype,
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_clra_registration_e_postal_form_ajax_callback',
																														'progress'			=> '',
																													  ),
																								);
			}
				
				
				
			
			$value_e_postal_areatype 		= isset($form_state['values']['e_postal_areatype']) ? $form_state['values']['e_postal_areatype'] : $e_postal_areatype;
			
			if($value_e_postal_dist != "" && $value_e_postal_subdivision != "" && $value_e_postal_areatype != ""){
					
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
				
				$form['clra_reg_master_edit']['e_postal_areatype_code'] 				= array(
																							  '#prefix' 			=> '<div class="col-md-4" id="e_postal_areatype_code_replace"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select '.$areatype_name,
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_e_postal_subdivision, $value_e_postal_areatype),
																							  '#default_value'		=> !empty($e_postal_name_areatype) ? $e_postal_name_areatype : '',
																							  '#attributes'			=> $readonly_e_postal_areatype_code,
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_clra_registration_e_postal_form_ajax_callback',
																														'progress'			=> '',
																													  )
																							);
				
				
				
			}else{
				$form['clra_reg_master_edit']['e_postal_areatype_code'] 				= array(
																							  '#prefix' 			=> '<div class="col-md-4" id="e_postal_areatype_code_replace"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Block',
																							  '#type' 				=> 'select',
																							  '#options' 			=> array(),
																							  '#attributes'			=> $readonly_e_postal_areatype_code,
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_clra_registration_e_postal_form_ajax_callback',
																														'progress'			=> '',
																													  )
																							);
			}
			
			$value_e_postal_areatype_code 	= isset($form_state['values']['e_postal_areatype_code']) ? $form_state['values']['e_postal_areatype_code'] : $e_postal_name_areatype;
			
			if($value_e_postal_dist != "" && $value_e_postal_subdivision != "" && $value_e_postal_areatype != "" && $value_e_postal_areatype_code != ""){
					
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
				
				$form['clra_reg_master_edit']['e_postal_vill_ward'] 					= array(
																							  '#prefix' 			=> '<div class="col-md-4" id="e_postal_vill_ward_replace"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select '.$area_name,
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_villward_list_new($value_e_postal_areatype_code,$area_code),
																							  '#default_value'		=> !empty($e_postal_vill_ward) ? $e_postal_vill_ward : '',
																							  '#attributes' 		=> $readonly_e_postal_vill_ward,
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_clra_registration_e_postal_form_ajax_callback',
																														'progress'			=> '',
																													  )
																							); 
				}else{
					
					$form['clra_reg_master_edit']['e_postal_vill_ward'] 					= array(
																							  '#prefix' 			=> '<div class="col-md-4" id="e_postal_vill_ward_replace"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select '.$area_name,
																							  '#type' 				=> 'select',
																							  '#options' 			=> array(),
																							  '#attributes' 		=> $readonly_e_postal_vill_ward,
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'get_clra_registration_e_postal_form_ajax_callback',
																														'progress'			=> '',
																													  )
																							);
					
				}
				
				$value_e_postal_vill_ward 						= isset($form_state['values']['e_postal_vill_ward']) ? $form_state['values']['e_postal_vill_ward'] : $e_postal_vill_ward;
				
				if($value_e_postal_dist != "" && $value_e_postal_subdivision != "" && $value_e_postal_areatype != "" && $value_e_postal_areatype_code != "" && $value_e_postal_vill_ward != "" ){
					
					$form['clra_reg_master_edit']['e_postal_ps'] 						= array(
																							  '#prefix' 			=> '<div class="col-md-4" id="clra_e_postal_ps_replace"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Police Station',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_ps_list_new($value_e_postal_dist),
																							  '#default_value' 		=> !empty($e_postal_ps) ? $e_postal_ps	:'',
																							  '#required' 			=> TRUE,
																							  '#attributes' 		=> $readonly_e_postal_ps,
																						  );
				}else{
					$form['clra_reg_master_edit']['e_postal_ps'] 						= array(
																							  '#prefix' 			=> '<div class="col-md-4" id="clra_e_postal_ps_replace"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Police Station',
																							  '#type' 				=> 'select',
																							  '#options' 			=> array(),
																							  '#required' 			=> TRUE,
																							  '#attributes' 		=> $readonly_e_postal_ps,
																						  );
				}
				
				$form['clra_reg_master_edit']['e_postal_pin_number'] 					= array(
																							  '#prefix' 			=> '<div class="col-md-4"><label class="input">',
																							  '#suffix' 			=> '</label></div></div></div>',
																							  '#title'				=> 'PIN Number',
																							  '#type' 				=> 'textfield',
																							  '#default_value' 	 	=> !empty($e_postal_pin_number) ? $e_postal_pin_number	:'',
																							  '#attributes' 		=> $readonly_e_postal_pin_number,
																							  '#required' 			=> TRUE,
																							  '#element_validate' 	=> array('MISC_PIN_NUMBER'),
																			  );
																			  
				$form['clra_reg_master_edit']['full_name_principal_emp'] 				= array(
																							  '#prefix' 			=> '<div class="panel panel-default panel-form"><div class="panel-heading">3. Full Name and address of the Principal Employer</div><div class="panel-body"><div class="col-md-3"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Principal Employer Name',
																							  '#type' 				=> 'textfield',
																							   '#default_value' 	=> !empty($full_name_principal_emp) ? $full_name_principal_emp : '',
																							  '#required' 			=> TRUE,
																							  '#element_validate' 	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																							);
				$form['clra_reg_master_edit']['gender_pe'] 								= array(
																							  '#prefix'				=> '<div class="col-md-3"><div class="inline-group">',
																							  '#suffix'				=>'</div></div>',
																							  '#type' 				=> 'radios',
																							  '#title' 				=> t('Gender'),
																							  '#options' 			=> array('M'=>t('Male'), 'F'=>t('Female'), 'O'=>t('Other')),
																							  '#default_value' 		=> isset($gender_pe) ? $gender_pe : NULL,
																							  '#attributes' 		=> $readonly_gender_pe,
																							  '#required' 			=> TRUE
																							);
																							
				$form['clra_reg_master_edit']['mobile_principal_emp'] = array(
											  '#prefix' => '<div class="col-md-3"><label class="input">',
											  '#suffix' => '</label></div>',
											  '#title' => 'Mobile No. of the Principal Employer',
											  '#type' => 'textfield',
											  '#default_value' => !empty($mobile_principal_emp) ? $mobile_principal_emp : '',
											  '#required' => TRUE,
											  '#attributes' => array('maxlength' => 10),
											  '#element_validate' => array('DASHBOARD_TEXTBOX_CONTACT_NUMBER_VALIDATE')
											);
				$options_country 	= array(''=>'-Select-', '1' => 'India', '2' => 'Others'); 
																										
				$form['clra_reg_master_edit']['emp_country'] 							= array(
																								'#prefix' 			=> '<div class="col-md-3"><label class="select">',
																								'#suffix' 			=> '</label></div>',
																								'#title'			=> 'Select Country',
																								'#type' 			=> 'select',
																								'#options' 			=> $options_country,
																								'#default_value'	=> (!empty($loc_emp_country)) ? $loc_emp_country : '',
																								'#required' 		=> TRUE,
																								'#attributes' 		=> $readonly_emp_country,
																								'#ajax'				=> array(
																																'event' 		=> 'change',
																																'effect' 		=> 'fade',			
																																'callback'		=> 'get_ajax_edit_country_emp',
																																'progress'		=>	'',
																														)
																						);
				$value_emp_country 									= isset($form_state['values']['emp_country']) ? $form_state['values']['emp_country'] : $loc_emp_country;
		
				$form['clra_reg_master_edit']['address_principal_emp']					= array(
																								'#prefix' 			=> '<div id="add_line_edit_replace">',
																								'#suffix' 			=> '</div>',
																								);
																									
				$form['clra_reg_master_edit']['emp_state']								= array(
																								'#prefix' 			=> '<div id="clra_edit_emp_state_replace">',
																								'#suffix' 			=> '</div>',
																								);
																									
				$form['clra_reg_master_edit']['emp_dist']								= array(
																								'#prefix' 			=> '<div id="clra_edit_emp_district_replace">',
																								'#suffix' 			=> '</div>',
																								);	
																									
				$form['clra_reg_master_edit']['loc_emp_subdv'] 							= array(
																							 '#prefix' 				=> '<div id="clra_empsubdv_replace">',
																							 '#suffix' 				=> '</div>'
																							);
																								
				$form['clra_reg_master_edit']['loc_emp_areatype'] 						= array(
																							 '#prefix' 				=> '<div id="clra_emp_areatype_replace">',
																							 '#suffix' 				=> '</div>'
																							);	
																								
				$form['clra_reg_master_edit']['loc_emp_areatype_name'] 					= array(
																							 '#prefix' 				=> '<div id="clra_empareatype_name_replace">',
																							 '#suffix' 				=> '</div>'
																							);	
																								
				$form['clra_reg_master_edit']['loc_emp_vill_ward'] 						= array(
																							 '#prefix' 				=> '<div id="clra_emp_vill_ward_replace">',
																							 '#suffix' 				=> '</div>'
																							);
																								
				$form['clra_reg_master_edit']['loc_emp_ps'] 							= array(
																								'#prefix' 			=> '<div id="clra_empps_replace">',
																								'#suffix' 			=> '</div>',
																								);																				
																									
				$form['clra_reg_master_edit']['loc_emp_pin_number'] 					= array(
																								'#prefix' 			=> '<div id="clra_emp_pin_replace">',
																								'#suffix' 			=> '</div>',
																								);
																								
				if( (!empty($value_emp_country) && $value_emp_country != '') && $value_emp_country ==1) {
			
			
					$form['clra_reg_master_edit']['address_principal_emp'] 				= array(
																							  '#prefix' 			=> '<div id="add_line_edit_replace" class="col-md-4"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'House No./Village Name/Street/Road',
																							  '#type' 				=> 'textfield',
																							  '#default_value' 		=>!empty($address_principal_emp) ? $address_principal_emp : '',
																							  '#attributes' 		=> $readonly_address_principal_emp,
																							  '#required' 			=> TRUE,
																							);
																								
					$form['clra_reg_master_edit']['emp_state']							= array(
																								'#prefix' 			=> '<div id="clra_edit_emp_state_replace" class="col-md-4"><label class="select">',
																								'#suffix' 			=> '</label></div>',
																								'#title'			=> t('Select State'),
																								'#type' 			=> 'select',
																								'#options' 			=> get_all_state($value_emp_country),
																								'#default_value'	=> $loc_emp_state,
																								'#required' 		=> TRUE,
																								'#attributes' 		=> $readonly_emp_state,
																								'#ajax'				=> array(
																															'event'		=> 'change',
																															'effect' 	=> 'fade',
																															'callback'	=> 'ajax_reload_edit_form',
																															'progress'	=>	'',
																															)
																								);	
																							
					$value_emp_state								= isset($form_state['values']['emp_state'])? $form_state['values']['emp_state'] : $loc_emp_state;
					
					if($value_emp_state != $loc_emp_state ){
								
						$loc_emp_dist 			= '';
						$loc_emp_subdivision 	= '';
						$loc_emp_areatype		= '';
						$emp_name_areatype		= '';
						$loc_emp_vill_ward		= '';
						$l_emp_ps				= '';
						$loc_emp_pin_number		= '';
						
					}
				
					if(($value_emp_country != '' && (!empty($value_emp_state) || $value_emp_state != '' )  )){
						
						
						$db_district_value							= db_query("SELECT COUNT(id) FROM {district_master} WHERE state_id = '".$value_emp_state."'")->fetchObject();
						
						
						if(!empty($db_district_value) && ($db_district_value->count>0) ) {
							
							$form['clra_reg_master_edit']['emp_dist'] 					= array(
																							  '#prefix' 			=> '<div id="clra_edit_emp_district_replace" class="col-md-4"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select District',
																							  '#type' 				=> 'select',
																							  '#options' 			=> get_district_by_id($value_emp_state),
																							  '#attributes' 		=> $readonly_emp_dist,
																							  '#default_value' 		=> !empty($loc_emp_dist) ? $loc_emp_dist : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'ajax_reload_edit_form',
																														'progress'			=> '',
																													  )
																							);	
																								
							$value_emp_dist 						= isset($form_state['values']['emp_dist']) ? $form_state['values']['emp_dist'] : $loc_emp_dist;
							
							if($value_emp_dist != $loc_emp_dist ){
								
								$loc_emp_subdivision 	= '';
								$loc_emp_areatype		= '';
								$emp_name_areatype		= '';
								$loc_emp_vill_ward		= '';
								$l_emp_ps				= '';
								$loc_emp_pin_number		= '';
								
							}
							
						}
						
					}
						
					if($value_emp_dist != ''  && !empty($value_emp_state)){
							
							$db_subdivision_value					= db_query("SELECT COUNT(id) as subdv_total FROM {sub_division} WHERE district_code = '".$value_emp_dist."'")->fetchObject();
							
							if(!empty($db_subdivision_value) && $db_subdivision_value != '' && $db_subdivision_value->subdv_total > 0) {
								
								
								$form['clra_reg_master_edit']['loc_emp_subdv'] 			= array(
																							  '#prefix' 			=> '<div id="clra_empsubdv_replace" class="col-md-4"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Subdivision',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_subdivisionlist_list_new($value_emp_dist),
																							  '#attributes' 		=> $readonly_emp_subdv,
																							  '#default_value' 		=> !empty($loc_emp_subdivision) ? $loc_emp_subdivision : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'ajax_reload_edit_form',
																														'progress'			=> '',
																													  )
																							);
																								
								$value_loc_emp_subdv 				= isset($form_state['values']['loc_emp_subdv']) ? $form_state['values']['loc_emp_subdv'] : $loc_emp_subdivision;
								
								if($value_loc_emp_subdv != $loc_emp_subdivision ){
										
									$loc_emp_areatype		= '';
									$emp_name_areatype		= '';
									$loc_emp_vill_ward		= '';
									$l_emp_ps				= '';
									$loc_emp_pin_number		= '';
								
								}
								
								
							}
							
					}
						
						
					if($value_emp_dist != "" && !empty($value_emp_state) && $value_loc_emp_subdv != ""){
							
							$db_block_mun_value						= db_query("SELECT COUNT(id) as areatype_total FROM {block_mun_master} WHERE distict_code = '".$value_emp_dist."' and sub_division_code  = '".$value_loc_emp_subdv."'")->fetchObject();
							
							if(!empty($db_block_mun_value) &&  $db_block_mun_value != '' && $db_block_mun_value->areatype_total > 0){
								
								$areatypeOptions					= common_custom_user_areatype_list_new($value_emp_dist, $value_loc_emp_subdv);
								
								
								if(!empty($areatypeOptions)){
									
									$form['clra_reg_master_edit']['loc_emp_areatype'] 	= array(
																							  '#prefix' 			=> '<div id="clra_emp_areatype_replace" class="col-md-4"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Area type',
																							  '#type' 				=> 'select',
																							  '#attributes' 		=> $readonly_emp_areatype,
																							  '#options' 			=> $areatypeOptions,
																							  '#default_value' 		=> !empty($loc_emp_areatype) ? $loc_emp_areatype : '',
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'ajax_reload_edit_form',
																														'progress'			=> '',
																													  )										  
																							);
																								
									$value_loc_emp_areatype 		= isset($form_state['values']['loc_emp_areatype']) ? $form_state['values']['loc_emp_areatype'] : $loc_emp_areatype;	
									
									if($value_loc_emp_areatype != $loc_emp_areatype){
									
										$emp_name_areatype		= '';
										$loc_emp_vill_ward		= '';
										$l_emp_ps				= '';
										$loc_emp_pin_number		= '';
									
									}
								}
								
								
							}
							
					}
					
					
					if ($value_emp_dist != "" && $value_loc_emp_subdv != "" && $value_loc_emp_areatype != "") {
						
						if ( $value_loc_emp_areatype == 'B' ){
							$areatype_name	= 'Block';
						}elseif( $value_loc_emp_areatype == 'M'){
							$areatype_name	= 'Municipality';
						}elseif( $value_loc_emp_areatype == 'C'){
							$areatype_name	= 'Corporation';
						}elseif( $value_loc_emp_areatype == 'S'){
							$areatype_name	= 'SEZ';
						}elseif($value_loc_emp_areatype == 'N'){
							$areatype_name	= 'Notified Area';
						}
						
						
						$form['clra_reg_master_edit']['loc_emp_areatype_name'] 			= array(
																							  '#prefix' 			=> '<div id="clra_empareatype_name_replace" class="col-md-4"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select '.$areatype_name,
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_loc_emp_subdv,$value_loc_emp_areatype),
																							  '#default_value' 		=> !empty($emp_name_areatype) ? $emp_name_areatype : '',
																							  '#attributes' 		=> $readonly_emp_areatype_name,
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'ajax_reload_edit_form',
																															'progress'			=> '',
																														  )
																							);
																								
						$value_loc_emp_areatype_code 				= 	isset($form_state['values']['loc_emp_areatype_name']) ? $form_state['values']['loc_emp_areatype_name'] : $emp_name_areatype;
						
						if($value_loc_emp_areatype_code != $emp_name_areatype){
									
							$loc_emp_vill_ward		= '';
							$l_emp_ps				= '';
							$loc_emp_pin_number		= '';
						
						}
						
					}
					
					if($value_emp_dist != "" && $value_loc_emp_subdv != "" && $value_loc_emp_areatype != ""  && $value_loc_emp_areatype_code != "" && !empty($value_emp_state) ){
						
						if ( $value_loc_emp_areatype == 'B' ){
							$area_name		= 'Gram Panchayat';
							$area_code 		= 'V';
						}elseif( $value_loc_emp_areatype == 'M'){
							$area_name		= 'Ward';
							$area_code 		= 'W';
						}elseif( $value_loc_emp_areatype == 'C'){
							$area_name		= 'Ward';
							$area_code 		= 'C';
						}elseif( $value_loc_emp_areatype == 'S'){
							$area_name		= 'Sector';
							$area_code 		= 'S';
						}elseif($value_loc_emp_areatype == 'N'){
							$area_name		= 'Notified Area';
							$area_code 		= 'N';
						}	
						
						
						$form['clra_reg_master_edit']['loc_emp_vill_ward'] 				= array(
																							  '#prefix' 			=> '<div id="clra_emp_vill_ward_replace" class="col-md-4"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select '.$area_name,
																							  '#type' 				=> 'select',
																							  '#options'			=> common_custom_user_villward_list_new($value_loc_emp_areatype_code,$area_code),
																							  '#required' 			=> TRUE,
																							  '#default_value' 		=> !empty($loc_emp_vill_ward) ? $loc_emp_vill_ward : '',
																							  '#attributes' 		=> $readonly_emp_vill_ward,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'ajax_reload_edit_form',
																															'progress'			=> '',
																														  )
																							);
																								
						$value_loc_emp_vill_ward 					= isset($form_state['values']['loc_emp_vill_ward']) ? $form_state['values']['loc_emp_vill_ward'] : $loc_emp_vill_ward;	
								
						if($value_loc_emp_vill_ward != $loc_emp_vill_ward){
							
							$l_emp_ps				= '';
							$loc_emp_pin_number		= '';
							
						}
						
					}
					
					if (!empty($value_emp_state) && $value_emp_dist != "" && $value_loc_emp_subdv != "" && $value_loc_emp_areatype && $value_loc_emp_areatype_code != "" && $value_loc_emp_vill_ward != "" ){
						
						
						$form['clra_reg_master_edit']['loc_emp_ps'] 					= array(
																							  '#prefix' 			=> '<div id="clra_empps_replace" class="col-md-4"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=>'Select Police Station',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_ps_list_new($value_emp_dist),
																							  '#default_value' 		=> !empty($l_emp_ps) ? $l_emp_ps: '',
																							  '#attributes' 		=> $readonly_emp_ps,
																							  '#required' 			=> TRUE,
																								  
																							); 	
																								
						$form['clra_reg_master_edit']['loc_emp_pin_number'] 			 = array(
																							  '#prefix' 			=> '<div id="clra_emp_pin_replace" class="col-md-4"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'PIN Number',
																							  '#type' 				=> 'textfield',
																							  '#default_value' 	 	=> !empty($loc_emp_pin_number) ? $loc_emp_pin_number	:'',
																							  '#attributes' 		=> $readonly_emp_pin_number,
																							  '#required' 			=> TRUE
																							  );
						
					}
						
				}else{
					
					$form['clra_reg_master_edit']['address_principal_emp'] 				= array(
																							  '#prefix' 			=> '<div id="add_line_edit_replace" class="col-md-12"><label class="textarea">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Address [ If other Country/State please provide detail address ]',
																							  '#rows'				=> t('2'),
																							  '#type' 				=> 'textarea',
																							  '#attributes' 		=> $readonly_address_principal_emp,
																							  '#default_value' 		=>!empty($address_principal_emp) ? $address_principal_emp : '',
																							  '#required' 			=> TRUE,
																							  '#element_validate' 	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																							);
																								
					$form['clra_reg_master_edit']['emp_state']							= array(
																								'#prefix' 			=> '<div id="clra_edit_emp_state_replace">',
																								'#suffix' 			=> '</div>',
																								);
																									
					$form['clra_reg_master_edit']['emp_dist']							= array(
																								'#prefix' 			=> '<div id="clra_edit_emp_district_replace">',
																								'#suffix' 			=> '</div>',
																								);
																									
					$form['clra_reg_master_edit']['loc_emp_subdv'] 						= array(
																							 '#prefix' 				=> '<div id="clra_empsubdv_replace">',
																							 '#suffix' 				=> '</div>'
																							);	
																								
					$form['clra_reg_master_edit']['loc_emp_areatype'] 					= array(
																							 '#prefix' 				=> '<div id="clra_emp_areatype_replace">',
																							 '#suffix' 				=> '</div>'
																							);
																								
					$form['clra_reg_master_edit']['loc_emp_areatype_name'] 				= array(
																							 '#prefix' 				=> '<div id="clra_empareatype_name_replace">',
																							 '#suffix' 				=> '</div>'
																							);	
																								
					$form['clra_reg_master_edit']['loc_emp_vill_ward'] 					= array(
																							 '#prefix' 				=> '<div id="clra_emp_vill_ward_replace">',
																							 '#suffix' 				=> '</div>'
																							);
																								
					$form['clra_reg_master_edit']['loc_emp_ps'] 						= array(
																								'#prefix' 			=> '<div id="clra_empps_replace">',
																								'#suffix' 			=> '</div>',
																								);
																									
					$form['clra_reg_master_edit']['loc_emp_pin_number'] 				= array(
																								'#prefix' 			=> '<div id="clra_emp_pin_replace">',
																								'#suffix' 			=> '</div>',
																								);
				
				}
				
				$form['clra_reg_master_edit']['full_name_manager'] 						= array(
																							  '#prefix' 			=> '</div></div><div class="panel panel-default panel-form"><div class="panel-heading">4. Full name and address of the Manager or Person responsible for the supervision  and control of the Establishment</div><div class="panel-body"><div class="col-md-6"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Full name of the Manager or Person Responsible',
																							  '#type' 				=> 'textfield',
																							  '#default_value' 		=> !empty($full_name_manager) ? $full_name_manager : '',
																							  '#attributes'			=> $readonly_full_name_manager, 
																							  '#required' 			=> TRUE,
																							  '#element_validate' 	=> array('MISC_TEXTBOX_TEXT_VALIDATE')
																							);
																							
		
																										
				$form['clra_reg_master_edit']['manager_country'] 						= array(
																								'#prefix' 			=> '<div class="col-md-6"><label class="select">',
																								'#suffix' 			=> '</label></div>',
																								'#title'			=> 'Select Country',
																								'#type' 			=> 'select',
																								'#options' 			=> $options_country,
																								'#default_value'	=> ($loc_manager_country != '') ? $loc_manager_country : '',
																								'#required' 		=> TRUE, 
																								'#attributes'		=> $readonly_manager_country, 
																								'#ajax'				=> array(
																																'event' 		=> 'change',
																																'effect' 		=> 'fade',			
																																'callback'		=> 'get_ajax_country_manager',
																																'progress'		=>	'',
																														)
																						);
																						
				$value_manager_country 									= isset($form_state['values']['manager_country']) ? $form_state['values']['manager_country'] : $loc_manager_country;
		
		
				if($value_manager_country != $loc_manager_country ){
					
					$loc_manager_state			= '';
					$loc_manager_dist 			= '';
					$loc_manager_subdivision 	= '';
					$loc_manager_areatype		= '';
					$manager_name_areatype		= '';
					$loc_manager_vill_ward		= '';
					$l_manager_ps				= '';
					$loc_manager_pin_number		= '';
					
				}
																							
				$form['clra_reg_master_edit']['address_manager']						= array(
																								'#prefix' 			=> '<div id="add_manager_replace">',
																								'#suffix'			=> '</div>',
																								);
																									
				$form['clra_reg_master_edit']['manager_state']							= array(
																								'#prefix' 			=> '<div id="clra_state_manager_replace">',
																								'#suffix' 			=> '</div>',
																								);
																									
				$form['clra_reg_master_edit']['manager_dist']							= array(
																								'#prefix' 			=> '<div id="clra_manager_district_replace">',
																								'#suffix' 			=> '</div>',
																								);
																									
				$form['clra_reg_master_edit']['loc_manager_subdv']						= array(
																								'#prefix' 			=> '<div id="clra_manager_subdv_replace">',
																								'#suffix' 			=> '</div>',
																								);
																									
				$form['clra_reg_master_edit']['loc_manager_areatype']					= array(
																								'#prefix' 			=> '<div id="clra_manager_areatype">',
																								'#suffix' 			=> '</div>',
																								);
																									
				$form['clra_reg_master_edit']['loc_manager_areatype_code']				= array(
																								'#prefix' 			=> '<div id="clra_manager_name_areatype">',
																								'#suffix' 			=> '</div>',
																								);
																									
				$form['clra_reg_master_edit']['loc_manager_vill_ward']					= array(
																								'#prefix' 			=> '<div id="clra_manager_vill_ward">',
																								'#suffix' 			=> '</div>',
																								);
																									
				$form['clra_reg_master_edit']['loc_manager_ps'] 						= array(
																								'#prefix' 			=> '<div id="clra_managerps_replace">',
																								'#suffix' 			=> '</div>',
																								);
																								
				$form['clra_reg_master_edit']['loc_manager_pin_number'] 				= array(
																								'#prefix' 			=> '<div id="clra_managerpin_replace">',
																								'#suffix' 			=> '</div>',
																								);			
																									
				
				if( (!empty($value_manager_country) && $value_manager_country != '') && $value_manager_country ==  1) {
					
					$form['clra_reg_master_edit']['address_manager'] 					= array(
																							  '#prefix' 			=> '<div id="add_manager_replace" class="col-md-4"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'House No./Village Name/Street/Road',
																							  '#type' 				=> 'textfield',
																							  '#required' 			=> TRUE,
																							  '#default_value' 		=>!empty($address_manager) ? $address_manager : '',
																							  '#attributes'			=> $readonly_address_manager, 
																							  '#element_validate' 	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																							);
																							
					$form['clra_reg_master_edit']['manager_state']						= array(
																								'#prefix' 			=> '<div id="clra_state_manager_replace" class="col-md-4"><label class="select">',
																								'#suffix' 			=> '</label></div>',
																								'#title'			=> t('Select State'),
																								'#type' 			=> 'select',
																								'#options' 			=> get_all_state($value_manager_country),
																								'#default_value'	=> $loc_manager_state,
																								'#attributes'		=> $readonly_manager_state, 
																								'#required' 		=> TRUE,
																								'#ajax'				=> array(
																															'event'		=> 'change',
																															'effect' 	=> 'fade',
																															'callback'	=> 'ajax_reload_data_edit_form',
																															'progress'	=>	'',
																															)
																							);
																							
					$value_manager_state								= isset($form_state['values']['manager_state'])? $form_state['values']['manager_state'] : $loc_manager_state;
			
			
					if($value_manager_state != $loc_manager_state ){
						
						$loc_manager_dist 			= '';
						$loc_manager_subdivision 	= '';
						$loc_manager_areatype		= '';
						$manager_name_areatype		= '';
						$loc_manager_vill_ward		= '';
						$l_manager_ps				= '';
						$loc_manager_pin_number		= '';
								
					}
					
					/****** MANAGER DISTRICT BLOCK STARTS ******/
			
					if(($value_manager_country != '' && (!empty($value_manager_state) || $value_manager_state != '' )  )){
						
						
						$db_manager_district_value						= db_query("SELECT COUNT(id) FROM {district_master} WHERE state_id = '".$value_manager_state."'")->fetchObject();
						
						
						if(!empty($db_manager_district_value) && ($db_manager_district_value->count>0) ) {
							
							
							$form['clra_reg_master_edit']['manager_dist']				= array(
																							'#prefix' 				=> '<div id="clra_manager_district_replace" class="col-md-4"><label class="select">',
																							'#suffix' 				=> '</label></div>',
																							'#title'				=> t('Select District'),
																							'#type' 				=> 'select',
																							'#options' 				=> get_district_by_id($value_manager_state),
																							'#default_value' 		=> !empty($loc_manager_dist) ? $loc_manager_dist : '',
																							'#attributes'			=> $readonly_manager_dist,
																							'#required' 			=> TRUE,
																							'#ajax'					=> array(
																															'event'		=> 'change',
																															'effect' 	=> 'fade',
																															'callback'	=> 'ajax_reload_data_edit_form',
																															'progress'	=>	'',
																															)
																							);
																										
							$value_manager_dist 						= 	isset($form_state['values']['manager_dist']) ? $form_state['values']['manager_dist'] : $loc_manager_dist;
							
							if($value_manager_dist != $loc_manager_dist){
								
								$loc_manager_subdivision 	= '';
								$loc_manager_areatype		= '';
								$manager_name_areatype		= '';
								$loc_manager_vill_ward		= '';
								$l_manager_ps				= '';
								$loc_manager_pin_number		= '';
							}
						}
					}
					
					/****** MANAGER SUBDIVISION BLOCK STARTS ******/	
			
					if($value_manager_dist != ''  && !empty($value_manager_state)){
						
						$db_manager_subdivision_value					= db_query("SELECT COUNT(id) as subdv_total_manager  FROM {sub_division} WHERE district_code = '".$value_manager_dist."'")->fetchObject();
						
						if(!empty($db_manager_subdivision_value) && $db_manager_subdivision_value != '' && $db_manager_subdivision_value->subdv_total_manager > 0) {
							
							$form['clra_reg_master_edit']['loc_manager_subdv'] 			= array(
																							  '#prefix' 			=> '<div id="clra_manager_subdv_replace" class="col-md-4"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Subdivision',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_subdivisionlist_list_new($value_manager_dist),
																							  '#default_value' 		=> !empty($loc_manager_subdivision) ? $loc_manager_subdivision	:'',
																							  '#required' 			=> TRUE,
																							  '#attributes'			=> $readonly_loc_manager_subdv,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'ajax_reload_data_edit_form',
																														'progress'			=> '',
																													  ),
																						);
																									
							$value_loc_manager_subdivision 				= isset($form_state['values']['loc_manager_subdv']) ? $form_state['values']['loc_manager_subdv'] : $loc_manager_subdivision;
								
							if($value_loc_manager_subdivision != $loc_manager_subdivision ){
										
								$loc_manager_areatype		= '';
								$manager_name_areatype		= '';
								$loc_manager_vill_ward		= '';
								$l_manager_ps				= '';
								$loc_manager_pin_number		= '';
								
							}
						}
					}
					
					if($value_manager_dist != "" && !empty($value_manager_state) && $value_loc_manager_subdivision != ""){
				
				
					 $db_block_mun_manager_value						= db_query("SELECT COUNT(id) as areatype_manager_total FROM {block_mun_master} WHERE distict_code = '".$value_manager_dist."' and sub_division_code  = '".$value_loc_manager_subdivision."'")->fetchObject();
					
					 if(!empty($db_block_mun_manager_value) &&  $db_block_mun_manager_value != '' && $db_block_mun_manager_value->areatype_manager_total > 0){
						
						$areatypeOptions							= common_custom_user_areatype_list_new($value_manager_dist, $value_loc_manager_subdivision);
						
						if(!empty($areatypeOptions)){
							
							$form['clra_reg_master_edit']['loc_manager_areatype'] 		= array(
																							  '#prefix' 			=> '<div id="clra_manager_areatype" class="col-md-4"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select Areatype',
																							  '#type' 				=> 'select',
																							  '#options' 			=> $areatypeOptions,
																							  '#default_value' 		=> !empty($loc_manager_areatype) ? $loc_manager_areatype : '',
																							  '#attributes'			=> $readonly_loc_manager_areatype,
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'ajax_reload_data_edit_form',
																														'progress'			=> '',
																													  ),
																								);
																										
							$value_loc_manager_areatype 			= isset($form_state['values']['loc_manager_areatype']) ? $form_state['values']['loc_manager_areatype'] : $loc_manager_areatype;
							
							if($value_loc_manager_subdivision != $loc_manager_subdivision && $value_loc_manager_areatype != $loc_manager_areatype){
								
								$manager_name_areatype		= '';
								$loc_manager_vill_ward		= '';
								$l_manager_ps				= '';
								$loc_manager_pin_number		= '';
							
							}
							
						}
						
					}
					}
					
					if ($value_manager_dist != "" && $value_loc_manager_subdivision != "" && $value_loc_manager_areatype != "") {
				
						if ( $value_loc_manager_areatype == 'B' ){
							$areatype_name	= 'Block';
						}elseif( $value_loc_manager_areatype == 'M'){
							$areatype_name	= 'Municipality';
						}elseif( $value_loc_manager_areatype == 'C'){
							$areatype_name	= 'Corporation';
						}elseif( $value_loc_manager_areatype == 'S'){
							$areatype_name	= 'SEZ';
						}elseif($value_loc_manager_areatype == 'N'){
							$areatype_name	= 'Notified Area';
						}
						
						$form['clra_reg_master_edit']['loc_manager_areatype_code'] 		= array(
																							  '#prefix' 			=> '<div id="clra_manager_name_areatype" class="col-md-4"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select '.$areatype_name,
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_block_list_new($value_loc_manager_subdivision, $value_loc_manager_areatype),
																							  '#default_value' 		=> !empty($manager_name_areatype) ? $manager_name_areatype : '',
																							  '#attributes'			=> $readonly_loc_manager_areatype_code,
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																														'event' 			=> 'change',
																														'effect' 			=> 'fade',			
																														'callback'			=> 'ajax_reload_data_edit_form',
																														'progress'			=> '',
																													  )
																							);
																									
																									
						$value_manager_name_areatype 					= isset($form_state['values']['loc_manager_areatype_code']) ? $form_state['values']['loc_manager_areatype_code'] : $manager_name_areatype;
						
						if($value_manager_name_areatype != $manager_name_areatype){
									
							$loc_manager_vill_ward		= '';
							$l_manager_ps				= '';
							$loc_manager_pin_number		= '';
						
						}
					}
					
					if($value_manager_dist != "" && $value_loc_manager_subdivision != "" && $value_loc_manager_areatype != ""  && $value_manager_name_areatype != "" && !empty($value_manager_state) ){
				
						if ( $value_loc_manager_areatype == 'B' ){
							$area_name		= 'Gram Panchayat';
							$area_code 		= 'V';
						}elseif( $value_loc_manager_areatype == 'M'){
							$area_name		= 'Ward';
							$area_code 		= 'W';
						}elseif( $value_loc_manager_areatype == 'C'){
							$area_name		= 'Ward';
							$area_code 		= 'C';
						}elseif( $value_loc_manager_areatype == 'S'){
							$area_name		= 'Sector';
							$area_code 		= 'S';
						}elseif($value_loc_manager_areatype == 'N'){
							$area_name		= 'Notified Area';
							$area_code 		= 'N';
						}
						
						
						$form['clra_reg_master_edit']['loc_manager_vill_ward'] 			= array(
																							  '#prefix' 			=> '<div id="clra_manager_vill_ward" class="col-md-4"><label class="select">',
																							  '#suffix' 			=> '<i></i></label></div>',
																							  '#title'				=> 'Select '.$area_name,
																							  '#type' 				=> 'select',
																							  '#options'			=> common_custom_user_villward_list_new($value_manager_name_areatype,$area_code),
																							  '#default_value' 		=> !empty($loc_manager_vill_ward) ? $loc_manager_vill_ward : '',
																							  '#attributes'			=> $readonly_loc_manager_vill_ward,
																							  '#required' 			=> TRUE,
																							  '#ajax'				=> array(
																															'event' 			=> 'change',
																															'effect' 			=> 'fade',			
																															'callback'			=> 'ajax_reload_data_edit_form',
																															'progress'			=> '',
																														  )
																							);	
																										
						$value_loc_manager_vill_ward 				= isset($form_state['values']['loc_manager_vill_ward']) ? $form_state['values']['loc_manager_vill_ward'] : $loc_manager_vill_ward;	
						
						if($value_loc_manager_vill_ward != $loc_manager_vill_ward){
									
							$l_manager_ps				= '';
							$loc_manager_pin_number		= '';
									
						}
					}
					
					if (!empty($value_manager_state) && $value_manager_dist != "" && $value_loc_manager_subdivision != "" && $value_loc_manager_areatype && $value_manager_name_areatype != "" && $value_loc_manager_vill_ward != "" ){
				
						$form['clra_reg_master_edit']['loc_manager_ps'] 				= array(
																							  '#prefix' 			=> '<div id="clra_managerps_replace" class="col-md-4"><label class="select">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Select Police Station',
																							  '#type' 				=> 'select',
																							  '#options' 			=> common_custom_user_ps_list_new($value_manager_dist),
																							  '#default_value' 		=> !empty($l_manager_ps) ? $l_manager_ps : '',
																							  '#attributes'			=> $readonly_loc_manager_ps,
																							  '#required' 			=> TRUE,
																							);
																										
						$form['clra_reg_master_edit']['loc_manager_pin_number'] 		= array(
																							  '#prefix' 			=> '<div id="clra_managerpin_replace" class="col-md-4"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'PIN Number',
																							  '#type' 				=> 'textfield',
																							  '#default_value' 	 	=> !empty($loc_manager_pin_number) ? $loc_manager_pin_number	:'',
																							  '#attributes' 		=> $readonly_loc_manager_pin_number,
																							  '#required' 			=> TRUE
																						  );
						
						
					}	
					
				}else{
				
					$form['clra_reg_master_edit']['address_manager'] 					= array(
																							  '#prefix' 			=> '<div id="add_manager_replace" class="col-md-12"><label class="textarea">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'Address[ If other Country/State please provide detail address ]',
																							  '#rows'				=> t('2'),	
																							  '#type' 				=> 'textarea',
																							  '#default_value' 		=> !empty($address_manager) ? $address_manager : '',
																							  '#attributes'			=> $readonly_address_manager, 
																							  '#required' 			=> TRUE,
																							  '#element_validate' 	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																							);
																							
					$form['clra_reg_master_edit']['manager_state']						= array(
																								'#prefix' 			=> '<div id="clra_state_manager_replace">',
																								'#suffix' 			=> '</div>',
																							);
																							
					$form['clra_reg_master_edit']['manager_dist']						= array(
																								'#prefix' 			=> '<div id="clra_manager_district_replace">',
																								'#suffix' 			=> '</div>',
																							);
																									
					$form['clra_reg_master_edit']['loc_manager_subdv']					= array(
																								'#prefix' 			=> '<div id="clra_manager_subdv_replace">',
																								'#suffix' 			=> '</div>',
																								);
																									
					$form['clra_reg_master_edit']['loc_manager_areatype']				= array(
																								'#prefix' 			=> '<div id="clra_manager_areatype">',
																								'#suffix' 			=> '</div>',
																							);
																									
					$form['clra_reg_master_edit']['loc_manager_areatype_code']			= array(
																								'#prefix' 			=> '<div id="clra_manager_name_areatype">',
																								'#suffix' 			=> '</div>',
																							);
																									
					$form['clra_reg_master_edit']['loc_manager_vill_ward']				= array(
																								'#prefix' 			=> '<div id="clra_manager_vill_ward">',
																								'#suffix' 			=> '</div>',
																							);
																							
					$form['clra_reg_master_edit']['loc_manager_ps'] 					= array(
																								'#prefix' 			=> '<div id="clra_managerps_replace">',
																								'#suffix' 			=> '</div>',
																							);
																									
					$form['clra_reg_master_edit']['loc_manager_pin_number'] 			= array(
																								'#prefix' 			=> '<div id="clra_managerpin_replace">',
																								'#suffix' 			=> '</div>',
																							);		
					
				}
				
				$options_cont 	= 	contractor_works_multiSelect_options();
				
				if(!empty($resultChunk)){
			
					$form['clra_reg_master_edit']['e_nature_of_work'] 					= array(
																								'#prefix' 			=> '</div></div><div class="panel panel-default panel-form"><div class="panel-heading">5. Nature of work carried on in the establishment</div><div class="panel-body"><div class="col-md-12"><label class="select">',
																								'#suffix' 			=> '</label></div>',
																								'#title'			=> 'Nature of Work Carried on in the Establishment',
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
				
					$form['clra_reg_master_edit']['e_nature_of_work'] 					= array(
																							'#prefix' 				=> '</div></div><div class="panel panel-default panel-form"><div class="panel-heading">5. Nature of work carried on in the establishment</div><div class="panel-body"><div class="col-md-12"><label class="select">',
																							'#suffix' 				=> '</label></div>',
																							'#title'				=> 'Nature of Work Carried on in the Establishment',
																							'#description'			=> '<span style="color:#ff0000;">Note: If multiple Nature of work to be selected, press Ctrl and select the options.</span>',
																							'#type' 				=> 'select',
																							'#id' 					=> 'e_nature_of_work_id',
																							'#options' 				=> $options_cont,
																							'#multiple' 			=> TRUE,
																							'#attributes' 			=> array('style'=>"height: 120px;", 'onchange' => 'nature_wrk_others(this.value)'),
																							'#required' 			=> TRUE,
																							'#attributes' 			=> $readonly_e_nature_of_work,
																							);	
				}
				
				$form['clra_reg_master_edit']['other_nature_of_work_value'] 			= array(
																							 '#prefix' 				=> '<div id ="edit_other_nature_work_id" class="col-md-12"><label class="input">',
																							 '#suffix' 				=> '</label></div>',
																							 '#title'				=> 'Other option for Nature of Work',
																							 '#default_value' 		=>  !empty($other_nature_wrk) ? $other_nature_wrk : '',
																							 '#id' 					=> 'other_value_nature_of_work_id',
																							 '#attributes' 			=> $readonly_other_nature_of_work_value,
																							 '#type' 				=> 'textfield',
																							);
																							
				$form['clra_reg_master_edit']['max_num_wrkmen']							= array(
																							 '#prefix' 				=> '</br></br><div class="col-md-4"><label class="input">',
																							 '#suffix' 				=> '</label></div>',
																							 '#title'				=> '(a) Maximum Number of Workmen Employed Directly on any day in the Establishment',
																							 '#type' 				=> 'textfield',
																							 '#default_value' 		=> ($max_num_wrkmen != '') ? $max_num_wrkmen : '',
																							 '#attributes' 			=> $readonly_max_num_wrkmen,
																							 '#required' 			=> TRUE,
																							 '#element_validate' 	=> array('MISC_TEXTBOX_NUMERIC_VALIDATE'),
																							);
																							
				$form['clra_reg_master_edit']['e_num_of_workmen_per_or_reg'] 			  = array(
																							  '#prefix' 			=> '<div class="col-md-4"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#required' 			=> TRUE,
																							  '#title'				=> '(b) Number of Workmen Engaged as Permanent / Regular Workmen',
																							  '#type' 				=> 'textfield',
																							  '#attributes'			=> $readonly_e_num_of_workmen_per_or_reg,
																							  '#default_value' 		=> ($e_num_of_workmen_per_or_reg!='') ? $e_num_of_workmen_per_or_reg : '',
																							  '#element_validate' 	=> array('MISC_TEXTBOX_NUMERIC_VALIDATE'),
																							);
																							
				$form['clra_reg_master_edit']['e_num_of_workmen_temp_or_reg'] 			 = array(
																							  '#prefix' 			=> '<div class="col-md-4"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> '(c) Number of Workmen Engaged as Temporary / Regular Workmen',
																							  '#type' 				=> 'textfield',
																							  '#default_value' 		=> ($e_num_of_workmen_temp_or_reg!='') ? $e_num_of_workmen_temp_or_reg : '',
																							  '#required' 			=> TRUE,
																							  '#element_validate' 	=> array('MISC_TEXTBOX_NUMERIC_VALIDATE'),
																							);
																							
				$workmenOptions			= array(0=> t('No'), 1=> t('Yes'));
				
				$form['clra_reg_master_edit']['workmen_if_same_similar_kind_of_work'] 	= array(
																							 '#prefix' 				=> '<div class="col-md-12 inline-group">',
																							 '#suffix' 				=> '</div>',
																							 '#type' 				=> 'radios',
																							 '#title' 				=> '(d) Whether the Workmen employed/intended to be Employment by the Contractor Perform the same or similar kind of work as the Workmen employed directly by the Principal Employer (if yes, please give here information as detailed below)',
																							 '#options' 			=> $workmenOptions,
																							 '#attributes' 			=> $readonly_workmen_if_same_similar_kind_of_work,
																							 '#default_value'		=> isset($workmen_if_same_similar_kind_of_work) ? $workmen_if_same_similar_kind_of_work : 0,
																							 '#required' 			=> TRUE,
																							);
																							
				$form['clra_reg_master_edit']['con_lab_job_desc'] 						= array(
																							  '#prefix' 			=> '<div class="col-md-4"><label class="textarea">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'd.i) a complete job description of the contract labour',
																							  '#type' 				=> 'textarea',
																							  '#default_value' 		=> !empty($con_lab_job_desc) ? $con_lab_job_desc : '',
																							  '#required' 			=> TRUE,
																							  '#attributes' 		=> $readonly_con_lab_job_desc,
																							  '#rows'				=> t('4'),	
																							  '#element_validate' 	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																							);
																							
				$form['clra_reg_master_edit']['con_lab_wage_rate_other_benefits']		= array(
																							  '#prefix' 			=> '<div class="col-md-4"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'd.ii) Wage rates and other cash benefits paid/to be paid',
																							  '#type' 				=> 'textfield',
																							  '#default_value' 		=> !empty($con_lab_wage_rate_other_benefits) ? $con_lab_wage_rate_other_benefits : '',
																							  '#attributes' 		=> $readonly_con_lab_wage_rate_other_benefits,
																							  '#element_validate' 	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																							);
																							
				$form['clra_reg_master_edit']['con_lab_cat_desig_nom']					= array(
																							 '#prefix' 				=> '<div class="col-md-4"><label class="input">',
																							  '#suffix' 			=> '</label></div>',
																							  '#title'				=> 'd.iii) Category/designation/nomenclature of the job',
																							  '#type' 				=> 'textfield',
																							  '#default_value' 		=> !empty($con_lab_cat_desig_nom) ? $con_lab_cat_desig_nom : '',
																							  '#attributes' 		=> $readonly_con_lab_cat_desig_nom,
																							  '#element_validate' 	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																						);
																						
				$form['clra_reg_master_edit']['e_settlement_award_judgement_min_wage'] 	= array(
																							  '#prefix' 			=> '<div class="col-md-8"><label class="input">',
																							  '#suffix' 			=> '</label></div></div></div>',
																							  '#title'				=> '(e) Settlement or award or judgement or minimum wages (if any applicable in the establishment)',
																							  '#type' 				=> 'textfield',
																							  '#default_value' 		=> !empty($e_settlement_award_judgement_min_wage) ? $e_settlement_award_judgement_min_wage : '',
																							  '#required' 			=> TRUE,
																							  '#attributes'			=> $readonly_e_settlement_award_judgement_min_wage,
																							  '#element_validate'	=> array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE')
																							);
																							
				$form['clra_reg_master_edit']['e_any_day_max_num_of_workmen']			= array(
																							 '#prefix' 				=> '<div class="panel panel-default panel-form"><div class="panel-heading">6. Maximum number of contract labour to be employed on any day through each contractor</div><div class="panel-body"><div class="col-md-8"><label class="input">',
																							  '#suffix' 		=> '</label></div></div></div>',
																							 '#title'			=> 'Maximum number of contract labour to be employed on any day through each contractor',
																							 '#type' 			=> 'textfield',
																							 '#default_value' 	=> !empty($e_any_day_max_num_of_workmen) ? $e_any_day_max_num_of_workmen : '',
																							 '#attributes' 		=> $readonly_e_any_day_max_num_of_workmen,
																							 '#required' 		=> TRUE,
																							 '#description' 	=> '<span style="display:none;" class="workernoten"><font color="red">Number of contract labour less than ten.</font> Note: It should be ten or more.</span>',
																							 '#element_validate'=> array('MISC_TEXTBOX_NUMERIC_VALIDATE'),
																							);
																							
				
				
				
				
				
				
				if ( $trade_license_file != "" ) {
					
					$trade_license_link 	= l(t('View Document &nbsp; <img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/lc-pdf.png">'),''.$GLOBALS['base_url'].'/sites/default/files/upload/trade_license/'.$trade_license_file, array('html' => TRUE,'attributes'=> array('target'=>'_blank'), 'attributes'=> array('class'=>' ')));
			
				}else{
						$trade_license_link = '';
					
				}
				
				$upload_value_1 = array( 'trade_license' => t('Trade License&nbsp;&nbsp;&nbsp;&nbsp;'.$trade_license_link ));
				
				if ( $article_of_assoc_file != "" ) {
					
					
					$article_of_assoc_link = l(t('View Document  <img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/lc-pdf.png">'),''.$GLOBALS['base_url'].'/sites/default/files/upload/article_of_assoc/'.$article_of_assoc_file, array('html' => TRUE,'attributes' => array('target'=>'_blank')));
			
				}else{
						$article_of_assoc_link = '';
					
				}
				
				$upload_value_2 = array( 'article_of_assoc' => t('Articles of Association and Memorandum of Association/Partnership Deed&nbsp;&nbsp;&nbsp;&nbsp;'.$article_of_assoc_link));
					
				
				if ( $memorandum_of_cert_file != "" ) {
					
					$memorandum_of_cert_link = l(t('View Document <img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/lc-pdf.png">'),''.$GLOBALS['base_url'].'/sites/default/files/upload/memorandum_of_cert/'.$memorandum_of_cert_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank')));
			
				}else{
						$memorandum_of_cert_link = '';
					
				}
				
				$upload_value_3 = array( 'memorandum_of_cert' => t('Any other document in support of correctness of the particulars mentioned in the application if required&nbsp;&nbsp;&nbsp;&nbsp;'.$memorandum_of_cert_link));
					
				
				
				if ( $partnership_deed_file != "" ) {
					
					$partnership_deed_link = l(t('View Document <img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/lc-pdf.png">'),''.$GLOBALS['base_url'].'/sites/default/files/upload/partnership_deed/'.$partnership_deed_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank')));
			
				}else{
					
					$partnership_deed_link = '';
					
				}
				
				$upload_value_4 = array( 'partnership_deed' => t('Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc.&nbsp;&nbsp;&nbsp;&nbsp;'.$partnership_deed_link));
					
				
				if ( $factory_license_file != "" ) {
					
					$factory_license_link = l(t('View Document <img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/lc-pdf.png">'),''.$GLOBALS['base_url'].'/sites/default/files/upload/FactoryLicense/'.$factory_license_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank')));
			
				}else{
					$factory_license_link = '';
				}
				
				$upload_value_5 = array( 'factory_license' => t('Factory License if any&nbsp;&nbsp;&nbsp;&nbsp;'.$factory_license_link));
					
				$form['clra_reg_master_edit']['upload_check_in_1'] 						= array(
																							  '#prefix' 			=> '<div class="panel panel-default panel-form"><div class="panel-heading">Upload Supporting Documents</div><div class="panel-body min-height-0"><section class="col-md-12"><label class="checkboxe">',
																							  '#suffix' 			=> '</label></section>',																		 
																							  '#type' 				=> 'checkboxes',
																							  '#options' 			=> $upload_value_1,
																							  '#attributes'			=> $readonly_trade_license
																							);
		
							
				$form['clra_reg_master_edit']['upload_check_in_2'] 						= array(
																							  '#prefix' 			=> '<section class="col-md-12"><label class="checkboxe">',
																							  '#suffix' 			=> '</label></section>',
																							  '#type' 				=> 'checkboxes',
																							  '#options' 			=> $upload_value_2,
																							  '#attributes'			=> $readonly_article_of_assoc
																							);
														
				$form['clra_reg_master_edit']['upload_check_in_3'] 						= array(
																							  '#prefix' 			=> '<section class="col-md-12"><label class="checkboxe">',
																							  '#suffix' 			=> '</label></section>',
																							  '#type' 				=> 'checkboxes',
																							  '#options' 			=> $upload_value_3,
																							  '#attributes'			=> $readonly_memorandum_of_cert
																							);
														
				$form['clra_reg_master_edit']['upload_check_in_4'] 						= array(
																							  '#prefix' 			=> '<section class="col-md-12"><label class="checkboxe">',
																							  '#suffix' 			=> '</label></section>',
																							  '#type' 				=> 'checkboxes',
																							  '#options' 			=> $upload_value_4,
																							  '#attributes'			=> $readonly_partnership_deed,
																							);
																				
				$form['clra_reg_master_edit']['upload_check_in_5'] 						= array(
																							   '#prefix' 			=> '<section class="col-md-12"><label class="checkboxe">',
																							  '#suffix' 			=> '</label></section>',
																							  '#type' 				=> 'checkboxes',
																							  '#options' 			=> $upload_value_5,
																							  '#attributes'			=> $readonly_factory_license,
																							);
																							
				$form['clra_reg_master_edit']['row1'] 									= array(
																							  '#markup' 			=> '<section class="col-md-6" id="trade_license_div">',
																							  ); 		
				$form['clra_reg_master_edit']['trade_license'] 							= array(
																							'#type' 				=> 'managed_file',
																							'#title'				=> '1. Upload Trade License',
																							'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																							'#upload_location' 		=> 'public://upload/trade_license/',
																							'#process' 				=> array('import_my_file_element_process'),
																							'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																							'#prefix' 				=> '<label class="input input-file">',
																							'#suffix' 				=> '</label>'
																							);
																							
				$form['clra_reg_master_edit']['row2'] 									= array(
																							  '#markup' 			=> '</section><section class="col-md-6" id="article_of_assoc_div">',
																							  ); 
							
	    		$form['clra_reg_master_edit']['article_of_assoc'] 						= array(
																							'#type' 				=> 'managed_file',
																							'#title'				=>'2. Upload Articles of Association and Memorandum of Association/Partnership Deed',
																							'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'),'file_validate_size' => array(50*1024*1024)),
																							'#upload_location' 		=> 'public://upload/article_of_assoc/',
																							'#process' 				=> array('import_my_file_element_process'),
																							'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																							'#prefix' 				=> '<label class="input input-file">',
																							'#suffix' 				=> '</label>',
																							);
				$form['clra_reg_master_edit']['row3'] 									= array(
																							  '#markup' 			=> '</section><section class="col-md-6" id="memorandum_of_cert_div">',
																							 );
	
				$form['clra_reg_master_edit']['memorandum_of_cert'] 					= array(
																							'#type' 				=> 'managed_file',
																							'#title'				=>'3. Upload any other document in support of correctness of the particulars mentioned in the application if required',
																							'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																							'#upload_location' 		=> 'public://upload/memorandum_of_cert/',
																							'#process' 				=> array('import_my_file_element_process'),
																							'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																							'#prefix' 				=> '<label class="input-file">',
																							'#suffix' 				=> '</label>'
																							);	
				
				
				
				$form['clra_reg_master_edit']['row4'] 									=  array(
																							  '#markup' 			=> '</section><section class="col-md-6" id="partnership_deed_div">',
																							  ); 
				
				$form['clra_reg_master_edit']['partnership_deed'] 						= array(
																							'#type' 				=> 'managed_file',
																							'#title'				=>'4. Upload other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc.',
																							'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																							'#upload_location' 		=> 'public://upload/partnership_deed/',
																							'#process' 				=> array('import_my_file_element_process'),
																							'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																							'#field_prefix' 		=> '<label class="input-file">',
																							'#field_suffix' 		=> '</label>'
																							);
																				
				$form['clra_reg_master_edit']['row5'] 									=  array(
																							  '#markup' 			=> '</section><section class="col-md-6" id="factory_license_div">',
																							  ); 
																				  
				$form['clra_reg_master_edit']['factory_license'] 						= array(
																							'#type' 				=> 'managed_file',
																							'#title'				=>'5. Upload Factory License',
																							'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
																							'#upload_location' 		=> 'public://upload/FactoryLicense/',
																							'#process' 				=> array('import_my_file_element_process'),
																							'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array('')),
																							'#field_prefix' 		=> '<label class="input-file">',
																							'#field_suffix' 		=> '</label>'
																							);
																							
				$form['clra_reg_master_edit']['row6'] 									= array(
																							 '#markup' 				=> '</section></div></div><div class="col-md-12"><div class="row">',  
																							 );
																							 
				$form['clra_reg_master_edit']['back_link'] 								= array(
																							  '#type' 				=> 'markup',  
																							  '#markup' 			=> l('<< BACK TO DASHBOARD', 'applicant-dashboard', array('attributes' => array('class' => 'link-btn')))
																							  );															
																	
				$form['clra_reg_master_edit']['submit'] 								= array(
																							 
																						  	  '#suffix' 			=> '</div></div></div>',  
																							  '#type' 				=> 'submit',
																							  '#attributes' 		=> array('class' => array('btn btn-primary pull-right form-submit')),
																							  '#value' 				=> 'SAVE'
																							   );
																									
																									
			
			
		}else{
			
			drupal_goto('applicant-dashboard');
		}
	}		
		
		

												
		
	
	
	
	return $form;
}

	
function get_clra_registration_info_form_submit($form, &$form_state){ 
	global $base_root, $base_path, $user;
	
	$user_id								= $user->uid;
	$val 									= $form_state['values'];
	$increment 								= 0;
	
	/* Upload For Trade License */
	
	$arr_1									= array();
	$trade_license 							= file_load($val['trade_license']);
	if( $trade_license != "" ){
		$arr_1 = explode("/", $trade_license->uri);
		$trade_license->status = FILE_STATUS_PERMANENT;
		file_save($trade_license);
		$trade_license_file = $arr_1[4];
	}
	
	
	/* Upload For Article of Association */
	
	$arr_2									= array();
	$article_of_assoc 						= file_load($val['article_of_assoc']);
	if( $article_of_assoc != "" ){
		$arr_2 = explode("/", $article_of_assoc->uri);
		$article_of_assoc->status = FILE_STATUS_PERMANENT;
		file_save($article_of_assoc);
		$article_of_assoc_file = $arr_2[4];
	}
	
	/* Upload For Memorandum of Certificate */
	
	$arr_3									= array();
	$memorandum_of_cert 					= file_load($val['memorandum_of_cert']);
	if( $memorandum_of_cert != "" ){
		$arr_3 = explode("/", $memorandum_of_cert->uri);
		$memorandum_of_cert->status = FILE_STATUS_PERMANENT;
		file_save($memorandum_of_cert);
		$memorandum_of_cert_file = $arr_3[4];
	}
	
	/* Upload For Partnership Deed */
	
	$arr_4									= array();
	$partnership_deed 						= file_load($val['partnership_deed']);
	if( $partnership_deed != "" ){
		$arr_4 = explode("/", $partnership_deed->uri);
		$partnership_deed->status = FILE_STATUS_PERMANENT;
		file_save($partnership_deed);
		$partnership_deed_file = $arr_4[4];
	}
	
	
	/* Upload For Factory License */
	
	$arr_5									= array();
	$factory_license 						= file_load($val['factory_license']);
	if( $factory_license != "" ){
		$arr_5 = explode("/", $factory_license->uri);
		$factory_license->status = FILE_STATUS_PERMANENT;
		file_save($factory_license);
		$factory_license_file = $arr_5[4];
	}
	
	
	
	$application_id							= encryption_decryption_fun('decrypt', $val['application_id']);
	$act_id 								= encryption_decryption_fun('decrypt', $val['act_id']);
	$identification_number					= encryption_decryption_fun('decrypt', $val['identification_number']);
	
				
	$e_name 								= $val['e_name'];
	$est_type = $val['est_type'];
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
	$e_postal_areatype_code 				= $val['e_postal_areatype_code'];
	$e_postal_vill_ward						= $val['e_postal_vill_ward'];
	$e_postal_ps							= $val['e_postal_ps'];
	$e_postal_pin_number					= $val['e_postal_pin_number'];
	
	$full_name_principal_emp 				= $val['full_name_principal_emp'];
	$mobile_principal_emp = $val['mobile_principal_emp'];
	$address_principal_emp 					= $val['address_principal_emp'];
	
	$emp_country							= $val['emp_country'];
	$emp_state								= $val['emp_state'];
	$loc_emp_dist 							= $val['emp_dist'];
	$loc_emp_subdivision 					= $val['loc_emp_subdv'];
	$loc_emp_areatype 						= $val['loc_emp_areatype'];
	$loc_emp_areatype_code					= $val['loc_emp_areatype_name'];
	$loc_emp_vill_ward						= $val['loc_emp_vill_ward'];
	$l_emp_ps								= $val['loc_emp_ps'];
	$loc_emp_pin_number						= $val['loc_emp_pin_number'];
	
	$full_name_manager 						= $val['full_name_manager'];
	$address_manager 						= $val['address_manager'];
	
	$loc_manager_country					= $val['manager_country'];
	$loc_manager_state						= $val['manager_state'];
	$loc_manager_dist 						= $val['manager_dist'];
	$loc_manager_subdivision 				= $val['loc_manager_subdv'];
	$loc_manager_areatype 					= $val['loc_manager_areatype'];
	$loc_manager_areatype_code 				= $val['loc_manager_areatype_code'];
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
	
	$fieldsRegInfo 							= array( 
												'user_id'									=>	$user->uid,
												'identification_number'						=>	$identification_number,
												'act_id'									=>  $act_id,
												
												'e_name'									=>	$e_name,
												'loc_e_name'								=>	$loc_e_name,
												'est_type' 									=> $est_type,
												
												'loc_e_dist'								=>	$loc_e_dist,
												'loc_e_subdivision'							=>	$loc_e_subdivision ,
												'loc_e_areatype'							=>	$loc_e_areatype ,
												'name_areatype'								=>	$loc_e_areatype_code ,
												'loc_e_vill_ward'							=>	$loc_e_vill_ward ,
												'l_e_ps'									=>	$l_e_ps,
												'loc_e_pin_number'							=>  $loc_e_pin_number,
												
												'e_postal_address'							=>	$e_postal_address,
												
												'e_postal_dist'								=>	$e_postal_dist,
												'e_postal_subdivision'						=>	$e_postal_subdivision ,
												'e_postal_areatype'							=>	$e_postal_areatype,
												'e_postal_name_areatype'					=>	$e_postal_areatype_code ,
												'e_postal_vill_ward'						=>	$e_postal_vill_ward,
												'e_postal_ps'								=>	$e_postal_ps,
												'e_postal_pin_number'						=>	$e_postal_pin_number,												 
												'full_name_principal_emp'					=>	$full_name_principal_emp,
												'mobile_principal_emp' 						=>  $mobile_principal_emp,
												'gender_pe'									=>  $val['gender_pe'], 
												'address_principal_emp'						=>	$address_principal_emp,												 
												'loc_emp_country'							=>	$emp_country,
												'loc_emp_state'								=>  $emp_state,
												'loc_emp_dist'								=>	$loc_emp_dist,
												'loc_emp_subdivision'						=>	$loc_emp_subdivision ,
												'loc_emp_areatype'							=>	$loc_emp_areatype,
												'emp_name_areatype'							=>	$loc_emp_areatype_code ,
												'loc_emp_vill_ward'							=>	$loc_emp_vill_ward,
												'l_emp_ps'									=>	$l_emp_ps,
												'loc_emp_pin_number'						=>  $loc_emp_pin_number,
												
												'full_name_manager'							=>	$full_name_manager, 
												'address_manager'							=>	$address_manager,
												
												'loc_manager_country'						=>  $loc_manager_country,
												'loc_manager_state'							=>  $loc_manager_state,
												'loc_manager_dist'							=>	$loc_manager_dist,
												'loc_manager_subdivision'					=>	$loc_manager_subdivision ,
												'loc_manager_areatype'						=>	$loc_manager_areatype,
												'manager_name_areatype'						=>	$loc_manager_areatype_code ,
												'loc_manager_vill_ward'						=>	$loc_manager_vill_ward,
												'l_manager_ps'								=>	$l_manager_ps,
												'loc_manager_pin_number'					=>  $loc_manager_pin_number,
												
												'max_num_wrkmen'							=>	$max_num_wrkmen, 
												'e_num_of_workmen_per_or_reg'				=>	$e_num_of_workmen_per_or_reg, 
												'e_num_of_workmen_temp_or_reg'				=>	$e_num_of_workmen_temp_or_reg, 
												'workmen_if_same_similar_kind_of_work'		=>	$workmen_if_same_similar_kind_of_work, 
												'con_lab_job_desc'							=>	$con_lab_job_desc, 
												'con_lab_wage_rate_other_benefits'			=>	$con_lab_wage_rate_other_benefits, 
												'con_lab_cat_desig_nom'						=>	$con_lab_cat_desig_nom,												
												'e_settlement_award_judgement_min_wage'		=>  $e_settlement_award_judgement_min_wage,
												'e_any_day_max_num_of_workmen'				=>	$e_any_day_max_num_of_workmen,
												
											);
	
								
	//*** Update Queries ***//
	
	if(!empty($fieldsRegInfo) && $application_id!=''){ 
		
		$query = db_update('l_clra_registration_master');
		$query->fields($fieldsRegInfo); 
		$query->condition('id',$application_id);
		$query->condition('user_id',$user_id);
		$query->execute();
			
	  /** For Nature Of work ***/
		db_delete('l_clra_reg_nature_of_work')
						->condition('user_id', trim($user_id),'=')
						->condition('application_id', trim($application_id),'=')
						->execute();
						
		db_delete('contractor_works')
						->condition('is_default',0)
						->condition('application_id', trim($application_id))
						->condition('act_id', trim($act_id))
						->execute();
						
			
		if(!empty($e_nature_of_work)){
			
			
			foreach($e_nature_of_work as $LoopValue){
				$fieldsEstContractorInfoNatureOfWork 				= 	array( 
																			'identification_number'	=>	$identification_number,
																			'user_id'				=>	$user_id,
																			'act_id'				=>  $act_id,
																			'application_id'		=>  $application_id,
																			'e_nature_of_work'		=>	$LoopValue
																			);
				db_insert('l_clra_reg_nature_of_work')->fields($fieldsEstContractorInfoNatureOfWork)->execute();
				$increment++;
			}
			
			
			
			if(!empty($other_nature_of_work_value)) {
				
							$ContractorOthersNatureOfWork 						= 	array( 
																							'application_id'		=>	$application_id,
																							'cont_work_name'		=>	ucwords($other_nature_of_work_value),
																							'act_id'				=>  $act_id,
																							'is_default'			=>  0
																							);
																						
							$last_insert_other_work_id = db_insert('contractor_works')->fields($ContractorOthersNatureOfWork)->execute();
			
			
			
		
		
							$fieldsEstContractorInfoNatureOfWork 				= 	array( 
																						'identification_number'	=>	$identification_number,
																						'user_id'				=>	$user->uid,
																						'act_id'				=>  $act_id,
																						'application_id'		=>	$application_id,
																						'e_nature_of_work'		=>	$last_insert_other_work_id
																						);
																							
							db_insert('l_clra_reg_nature_of_work')->fields($fieldsEstContractorInfoNatureOfWork)->execute();
				
				
			}
		}
		
		
		
		/**** DOC UPLOADED CODE *****/
				
		$fetch_reg_upload_files 	= db_query("select * from l_documents_upload where application_id=:application_id and user_id=:user_id",array(':application_id'=>$application_id, ':user_id'=>$user_id));
		
		if( $fetch_reg_upload_files->rowCount() > 0 ){
			
			foreach($fetch_reg_upload_files as $obj3){ 
				
				$identification_number_old					=	$obj3->identification_number;
				$trade_license_file_old						=	$obj3->trade_license_file;
				$article_of_assoc_file_old					=	$obj3->article_of_assoc_file;
				$memorandum_of_cert_file_old				=	$obj3->memorandum_of_cert_file;
				$partnership_deed_file_old					=	$obj3->partnership_deed_file;
				$factory_license_old						= 	$obj3->factory_license_file;
				$act_id_old									=	$obj3->act_id;
				$application_id								=   $obj3->application_id;
			}
			
			
			$queryForward = db_update('l_documents_upload');
			$queryForward->fields(array(
											'user_id'									=>	$user_id,
											'act_id'									=>  $act_id,
											'identification_number'						=>	$identification_number,
											'trade_license_file' 						=>  isset($trade_license_file)&&($trade_license_file_old!=$trade_license_file) ? $trade_license_file : $trade_license_file_old,
											'article_of_assoc_file' 					=>  isset($article_of_assoc_file)&&($article_of_assoc_file_old!=$article_of_assoc_file) ? $article_of_assoc_file : $article_of_assoc_file_old,
											'memorandum_of_cert_file' 					=>  isset($memorandum_of_cert_file)&&($memorandum_of_cert_file_old!=$memorandum_of_cert_file) ? $memorandum_of_cert_file : $memorandum_of_cert_file_old,
											'partnership_deed_file' 					=>  isset($partnership_deed_file)&&($partnership_deed_file_old!=$partnership_deed_file) ? $partnership_deed_file : $partnership_deed_file_old,
											'factory_license_file' 						=>  isset($factory_license_file)&&($factory_license_old!=$factory_license_file) ? $factory_license_file : $factory_license_old,
			 ));
			$queryForward->condition('user_id',$user_id);
			$queryForward->condition('application_id',$application_id);
			$queryForward->execute();
			
		}else{
			
			$fieldsDocumentsData 		= array( 
													'user_id'									=>	$user->uid,
													'act_id'									=>  $act_id,
													'identification_number'						=>	$identification_number,
													'trade_license_file' 						=>  $trade_license_file,
													'article_of_assoc_file' 					=>  $article_of_assoc_file,
													'memorandum_of_cert_file' 					=>  $memorandum_of_cert_file,
													'partnership_deed_file' 					=>  $partnership_deed_file,
													'factory_license_file'						=>  $factory_license_file,
													'application_id'							=>	$application_id,
												);
													
													
					if(!empty($fieldsDocumentsData)) {
													
						db_insert('l_documents_upload')->fields($fieldsDocumentsData)->execute();
					
					}
		}
		
		}
		
		$message = 'Application details successfully saved.';
		drupal_set_message(t($message));
}
function DASHBOARD_TEXTBOX_CONTACT_NUMBER_VALIDATE($element,&$form_state){
	 $val	= $form_state['values'];
	 $mob	= "/^[789][0-9]{9}$/";
	 if( strlen(trim($val['mobile_principal_emp'])) > 0 ){   
		if(!preg_match($mob, $val['mobile_principal_emp'])){ 
		  if (strlen(trim($val['mobile_principal_emp'])) < 10){
				form_error($element, t($element['#title'].' is less than 10 digit. Please enter valid mobile number.'));
		  }else{
				form_error($element, t($element['#title'].' should be valid number.')); 
		  }
		}
	 }else{
		form_error($element, t($element['#title'].' can not be left blank. Please enter valid mobile number.'));
	 }
}

function get_clra_registration_info_edit_form_ajax_callback($form, $form_state) {
	$commands = array();
	$commands[] = ajax_command_replace('#clra_subdvsn_replace', drupal_render($form['clra_reg_master_edit']['loc_e_subdivision']));
	$commands[] = ajax_command_replace('#clra_loc_e_areatype', drupal_render($form['clra_reg_master_edit']['loc_e_areatype']));
	$commands[] = ajax_command_replace('#clra_code_replace', drupal_render($form['clra_reg_master_edit']['loc_e_areatype_code']));
	$commands[] = ajax_command_replace('#clra_loc_vill_replace', drupal_render($form['clra_reg_master_edit']['loc_e_vill_ward']));
	$commands[] = ajax_command_replace('#clra_ps_replace', drupal_render($form['clra_reg_master_edit']['loc_e_ps']));
  	return array('#type' => 'ajax', '#commands' => $commands);
}

function get_clra_registration_e_postal_form_ajax_callback($form, $form_state){ 
	$commands = array(); 
	$commands[] = ajax_command_replace('#clra_e_postal_subdvsn_replace', drupal_render($form['clra_reg_master_edit']['e_postal_subdivision']));
	$commands[] = ajax_command_replace('#e_postal_areatype_replace', drupal_render($form['clra_reg_master_edit']['e_postal_areatype']));
	$commands[] = ajax_command_replace('#e_postal_areatype_code_replace', drupal_render($form['clra_reg_master_edit']['e_postal_areatype_code']));
	$commands[] = ajax_command_replace('#e_postal_vill_ward_replace', drupal_render($form['clra_reg_master_edit']['e_postal_vill_ward']));
	$commands[] = ajax_command_replace('#clra_e_postal_ps_replace', drupal_render($form['clra_reg_master_edit']['e_postal_ps']));
  	return array('#type' => 'ajax', '#commands' => $commands);	
}

function get_ajax_edit_country_emp($form, $form_state){
	$commands = array();
	$commands[] = ajax_command_replace('#add_line_edit_replace', drupal_render($form['clra_reg_master_edit']['address_principal_emp']));
	$commands[] = ajax_command_replace('#clra_edit_emp_state_replace', drupal_render($form['clra_reg_master_edit']['emp_state']));
	$commands[] = ajax_command_replace('#clra_edit_emp_district_replace', drupal_render($form['clra_reg_master_edit']['emp_dist']));
	$commands[] = ajax_command_replace('#clra_empsubdv_replace', drupal_render($form['clra_reg_master_edit']['loc_emp_subdv']));
	$commands[] = ajax_command_replace('#clra_emp_areatype_replace', drupal_render($form['clra_reg_master_edit']['loc_emp_areatype']));
	$commands[] = ajax_command_replace('#clra_empareatype_name_replace', drupal_render($form['clra_reg_master_edit']['loc_emp_areatype_name']));
	$commands[] = ajax_command_replace('#clra_emp_vill_ward_replace', drupal_render($form['clra_reg_master_edit']['loc_emp_vill_ward']));
	$commands[] = ajax_command_replace('#clra_empps_replace', drupal_render($form['clra_reg_master_edit']['loc_emp_ps']));
	$commands[] = ajax_command_replace('#clra_emp_pin_replace', drupal_render($form['clra_reg_master_edit']['loc_emp_pin_number']));
	return array('#type' => 'ajax', '#commands' => $commands);
	
}

function ajax_reload_edit_form($form, $form_state) {
	$commands = array();
	$commands[] = ajax_command_replace('#clra_edit_emp_state_replace', drupal_render($form['clra_reg_master_edit']['emp_state']));
	$commands[] = ajax_command_replace('#clra_edit_emp_district_replace', drupal_render($form['clra_reg_master_edit']['emp_dist']));
	$commands[] = ajax_command_replace('#clra_empsubdv_replace', drupal_render($form['clra_reg_master_edit']['loc_emp_subdv']));
	$commands[] = ajax_command_replace('#clra_emp_areatype_replace', drupal_render($form['clra_reg_master_edit']['loc_emp_areatype']));
	$commands[] = ajax_command_replace('#clra_empareatype_name_replace', drupal_render($form['clra_reg_master_edit']['loc_emp_areatype_name']));
	$commands[] = ajax_command_replace('#clra_emp_vill_ward_replace', drupal_render($form['clra_reg_master_edit']['loc_emp_vill_ward']));
	$commands[] = ajax_command_replace('#clra_empps_replace', drupal_render($form['clra_reg_master_edit']['loc_emp_ps']));
	$commands[] = ajax_command_replace('#clra_emp_pin_replace', drupal_render($form['clra_reg_master_edit']['loc_emp_pin_number']));
    return array('#type' => 'ajax', '#commands' => $commands);
}

function get_ajax_country_manager($form, $form_state){
	$commands = array();
	$commands[] = ajax_command_replace('#add_manager_replace', drupal_render($form['clra_reg_master_edit']['address_manager']));
	$commands[] = ajax_command_replace('#clra_state_manager_replace', drupal_render($form['clra_reg_master_edit']['manager_state']));
	$commands[] = ajax_command_replace('#clra_manager_district_replace', drupal_render($form['clra_reg_master_edit']['manager_dist']));
	$commands[] = ajax_command_replace('#clra_manager_subdv_replace', drupal_render($form['clra_reg_master_edit']['loc_manager_subdv']));
	$commands[] = ajax_command_replace('#clra_manager_areatype', drupal_render($form['clra_reg_master_edit']['loc_manager_areatype']));
	$commands[] = ajax_command_replace('#clra_manager_name_areatype', drupal_render($form['clra_reg_master_edit']['loc_manager_areatype_code']));
	$commands[] = ajax_command_replace('#clra_manager_vill_ward', drupal_render($form['clra_reg_master_edit']['loc_manager_vill_ward']));
	$commands[] = ajax_command_replace('#clra_managerps_replace', drupal_render($form['clra_reg_master_edit']['loc_manager_ps']));
	$commands[] = ajax_command_replace('#clra_managerpin_replace', drupal_render($form['clra_reg_master_edit']['loc_manager_pin_number']));
	return array('#type' => 'ajax', '#commands' => $commands);
	
}

function ajax_reload_data_edit_form($form, $form_state) {
	$commands = array();
	$commands[] = ajax_command_replace('#clra_state_manager_replace', drupal_render($form['clra_reg_master_edit']['manager_state']));
	$commands[] = ajax_command_replace('#clra_manager_district_replace', drupal_render($form['clra_reg_master_edit']['manager_dist']));
	$commands[] = ajax_command_replace('#clra_manager_subdv_replace', drupal_render($form['clra_reg_master_edit']['loc_manager_subdv']));
	$commands[] = ajax_command_replace('#clra_manager_areatype', drupal_render($form['clra_reg_master_edit']['loc_manager_areatype']));
	$commands[] = ajax_command_replace('#clra_manager_name_areatype', drupal_render($form['clra_reg_master_edit']['loc_manager_areatype_code']));
	$commands[] = ajax_command_replace('#clra_manager_vill_ward', drupal_render($form['clra_reg_master_edit']['loc_manager_vill_ward']));
	$commands[] = ajax_command_replace('#clra_managerps_replace', drupal_render($form['clra_reg_master_edit']['loc_manager_ps']));
	$commands[] = ajax_command_replace('#clra_managerpin_replace', drupal_render($form['clra_reg_master_edit']['loc_manager_pin_number']));
  	return array('#type' => 'ajax', '#commands' => $commands);
}

function get_clra_registration_trade_union_form_ajax_callback($form, $form_state) {
	$commands = array();
	$commands[] = ajax_command_replace('#clra_trade_union_subdv_replace', drupal_render($form['clra_reg_master_edit']['e_trade_union_subdv']));
	$commands[] = ajax_command_replace('#clra_trade_union_mun_replace', drupal_render($form['clra_reg_master_edit']['e_trade_union_mun']));
	$commands[] = ajax_command_replace('#clra_trade_union_block_replace', drupal_render($form['clra_reg_master_edit']['e_trade_union_block']));
	$commands[] = ajax_command_replace('#clra_trade_union_other_replace', drupal_render($form['clra_reg_master_edit']['e_trade_union_other']));
	$commands[] = ajax_command_replace('#clra_trade_union_corp_replace', drupal_render($form['clra_reg_master_edit']['e_trade_union_corp']));
	$commands[] = ajax_command_replace('#clra_trade_union_sez_replace', drupal_render($form['clra_reg_master_edit']['e_trade_union_sez']));
	$commands[] = ajax_command_replace('#clra_trade_union_not_replace', drupal_render($form['clra_reg_master_edit']['e_trade_union_not']));
	$commands[] = ajax_command_replace('#clra_trade_union_vill_replace', drupal_render($form['clra_reg_master_edit']['e_trade_union_vill']));
	$commands[] = ajax_command_replace('#clra_trade_union_ward_replace', drupal_render($form['clra_reg_master_edit']['e_trade_union_ward']));
	$commands[] = ajax_command_replace('#clra_trade_union_ps_replace', drupal_render($form['clra_reg_master_edit']['e_trade_union_ps']));
	$commands[] = ajax_command_replace('#clra_trade_union_bugh_replace', drupal_render($form['clra_reg_master_edit']['e_trade_union_bugh']));
	$commands[] = ajax_command_replace('#clra_trade_union_code_replace', drupal_render($form['clra_reg_master_edit']['e_trade_union_code']));
  return array('#type' => 'ajax', '#commands' => $commands);
}



