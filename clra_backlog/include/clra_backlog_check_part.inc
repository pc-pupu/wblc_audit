<?php

function clra_backlog_check_part($form, &$form_state, $backlog_id = ''){
	
	global $base_root, $base_path, $user;
	
	
	$form['clra_backlog_master'] = array(
							'#prefix' => '<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">Offline / manual registration number</div><div class="panel-body">',
							'#attributes' => array('class'=>array('')),
						);
		 
	$form['clra_backlog_master']['regn_number'] = array(
								'#prefix' 		=> '<div class="col-md-12"><label class="input">',
								'#suffix' 		=> '</label></div>',
								'#title'		=> 'Enter offline / manual registration number',
								'#type' 		=> 'textfield',
								'#required'		=>	TRUE,
								'#attributes'	=> array('autocomplete' => 'off')
							);
	
	$form['clra_backlog_master']['note']				= array(
																'#prefix' 		=> '<div class="col-md-12">',
																'#suffix' 		=> '</div>',
																'#markup'		=> '<font color="red"><b>Note :- If you already have an Offline Registration Number Under Contract Labour (Regulation & Abolition) Act 1970, provide the Offline Registration Number and get a System Generated Registration Number.</b></font>',
																);
	
	$form['clra_backlog_master']['submit'] 				= array (
																'#prefix' 		=> '</div></div>',  
																'#type' 		=> 'submit',
																'#value' 		=> 'Continue ',
																'#attributes'	=> array('id' => 'submit_oneId' , 'class' => array('btn btn-primary pull-right'))
															);
															
	$form['clra_backlog_master']['link'] 				= array (
																'#suffix' 		=> '</div>', 
																'#type' 		=> 'markup',
																'#markup' 		=> l(t('<< Back'),'clra_backlog/clra-registration-type', array('html' => TRUE,'attributes'=>array('title' => '', 'class' => 'link-btn')))
															);													
							
	return $form;
}

function clra_backlog_check_part_validate($form, &$form_state){
	$val			=	$form_state['values'];
	
	foreach($form as $element){
		if(is_array($element)){
			if(isset($element['#type']) and ($element['#type'] == 'textfield')){
				chkbadchar($element['#value'], $element['#title'], $element['#name']);
			}
		}
 	}
	
	if( $val['regn_number'] == "" ){
		form_error($form['clra_backlog_master']['regn_number'], t('Registration Number cannot be blank.'));
	}
}

function clra_backlog_check_part_submit($form, &$form_state){
	global $base_root, $base_path, $user;
	
	$exitrow = db_query("select count(id) as total_row FROM l_clra_registration_master WHERE user_id = '".$user->uid."' AND ((status is null) OR (status IN ('', '0', 'B', 'BI', 'F', 'V', 'VA', 'S', 'I')))")->fetchObject();		
	
	if($exitrow->total_row > 0 ){	// Yes
		drupal_goto('applicant-dashboard');
	}else{
		
	$val 					=	$form_state['values'];
	$registration_no		=	trim($val['regn_number']);
	
	if(!empty($registration_no)){
		
		$fetchExistingIdenNumber		=	db_select('l_clra_registration_master', 'lcrm');
		$fetchExistingIdenNumber->fields('lcrm', array());
		$fetchExistingIdenNumber->condition('lcrm.registration_number', trim($registration_no));
		$fetchExistingIdenNumber_res	=	$fetchExistingIdenNumber->execute();
		$fetchExistingIdenNumber_data 	= 	$fetchExistingIdenNumber_res->fetchObject();
		
		if( $fetchExistingIdenNumber_res->rowCount() > 0 ){	// Yes
			// echo "hii11"; exit;
			$edit_id 	= 	$fetchExistingIdenNumber_data->id;
			$edit_id	= encryption_decryption_fun('encrypt', $edit_id);
			drupal_goto('view-clra-application-details/view-clra-application/'.$edit_id);
		}else{	// No
			//echo "hii22"; exit;
			$get_bklg_tbl			=	db_select('l_clra_principle_emp_backlog_data', 'lcpebd');
			$get_bklg_tbl->fields('lcpebd', array());
			$get_bklg_tbl->condition('lcpebd.registration_no', $registration_no);
			$get_bklg_tbl_res		=	$get_bklg_tbl->execute();
			$get_bklg_tbl_res_data 	= 	$get_bklg_tbl_res->fetchObject();
			
			if( $get_bklg_tbl_res->rowCount() > 0 ){
				if( $get_bklg_tbl_res_data->status == 1 ){
					form_set_error('', t('Invalid registration number. Please enter valid registration number.'));
					// $edit_id 	= 	$fetchExistingIdenNumber_data->id;					
					// $edit_id	= encryption_decryption_fun('encrypt', $edit_id);
					// drupal_goto('view-clra-application-details/view-clra-application/'.$edit_id);
				}elseif( $get_bklg_tbl_res_data->status == 0 ){  // if 0
					//echo "hii44"; exit; //"230/R-25/01/ALC/DH"
					$id								=	$get_bklg_tbl_res_data->id;
					$registration_no				=	$get_bklg_tbl_res_data->registration_no;
					$registration_date				=	$get_bklg_tbl_res_data->registration_date;
					$name_of_the_establishment		=	$get_bklg_tbl_res_data->name_of_the_establishment;
					$address_of_the_establishment	=	$get_bklg_tbl_res_data->address_of_the_establishment;
					$name_of_the_principle_emp		=	$get_bklg_tbl_res_data->name_of_the_principle_emp;
					$address_of_the_principle_emp	=	$get_bklg_tbl_res_data->address_of_the_principle_emp;
					$maximum_no_of_contact_labour	=	$get_bklg_tbl_res_data->maximum_no_of_contact_labour;
					$finalfees						=	$get_bklg_tbl_res_data->fees;
		
					$fieldsBackLogRegInfo 			= 	array(
															'user_id'							=>	$user->uid,
															'identification_number'				=>	'CLRA'.time(),
															'e_name'							=>	$name_of_the_establishment,
															'e_postal_address'					=>	$address_of_the_establishment,
															'full_name_principal_emp'			=>	$name_of_the_principle_emp,
															'address_principal_emp'				=>	$address_of_the_principle_emp,
															'backlog_id'						=>  $id,
															// 'finalfees'							=>  $finalfees,
															'act_id'							=>  1,
															// 'status'							=>  '0',
															'e_any_day_max_num_of_workmen'		=>  $maximum_no_of_contact_labour
										    			);
					$last_id						=	db_insert('l_clra_registration_master')->fields($fieldsBackLogRegInfo)->execute();;
					$edit_id 						= 	$fetchExistingIdenNumber_data->id;
					if( $query != "" ){
						$fieldsBackLogUpdateInfo 	= 	array(
															'status'		=> '2',
															'date_time'		=> time()
														);
						
						$query = db_update('l_clra_principle_emp_backlog_data');
              			$query->fields($fieldsBackLogUpdateInfo); 
					  	$query->condition('id', $id);
              			$query->execute();
					}
					$last_id	= encryption_decryption_fun('encrypt', $last_id);
					drupal_goto('view-clra-application-details/view-clra-application/'.$last_id);
				}/*elseif( $get_bklg_tbl_res_data->status == 2 ){  // if 0
					form_set_error('', t('UNDER PROCESSING.CONTACT ALC.'));
				}*/
			}else{
				// form_set_error('', t('REGISTRATION NUMBER DOES NOT EXIST.PLEASE ENTER ORIGINAL REGISTRATION NUMBER ELSE YOU CAN CLICK SKIP BUTTON.'));
				drupal_goto('apply-clra-registration-backlog');
			}	
		}	
	}
	
   }
}

function clra_registration_type(){
	global $user;
	
	//if($user->uid == 64){
		$reg_type =	array('0'=>t('Already have a registration number of the establishment under The Contract Labour (Regulation & Abolition) Act, 1970'), '1'=>t('New registration ( You will be redirected to Shilpa Sathi Portal (https://silpasathi.wb.gov.in) to avail this service. Click on <strong>CONTINUE</strong> to proceed)'));
	/*}else{
		$reg_type =	array('0'=>t('Already have a registration number of the establishment under The Contract Labour (Regulation & Abolition) Act, 1970'), '1'=>t('New registration ()'));
	}*/
		 
	$form['clra_backlog_master']['reg_type'] = array(
									'#prefix' 		=> '<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">NEW REGISTRATION OF PRINCIPAL EMPLOYERS </div><div class="panel-body"><div class="radio-full-width">',
									'#suffix' 		=> '</div></div></div>',
									'#title'		=> 'Choose any option to continue for registration',
									'#options' 		=> $reg_type,
									'#type' 		=> 'radios',
									'#required'		=>	TRUE																
									);
										
	$form['clra_backlog_master']['submit'] = array (
									'#suffix' 		=> '</div>',  
									'#type' 		=> 'submit',
									'#value' 		=> 'Continue ',
									'#attributes'	=> array('id' => '' , 'class' => array('btn btn-primary pull-right'))
									);								
														
							
	return $form;
}
function clra_registration_type_submit($form, &$form_state){
	global $user;
	$val = $form_state['values'];
	
	if($val['reg_type'] == 0){
		drupal_goto('clra_backlog');
	}
	if($val['reg_type'] == 1){
		//if($user->uid == 64){
			drupal_goto('https://silpasathi.wb.gov.in');
		/*}else{
			drupal_goto('applicant-profile/clra-registration/new');
		}*/
	}
}

function clra_backlog_check_part_amnd($form, &$form_state, $backlog_id = ''){
	
	global $base_root, $base_path, $user;
	
	$get_bklg_tbl =	db_select('l_clra_principle_emp_backlog_data', 'lcpebd');
	$get_bklg_tbl->fields('lcpebd', array());
	$get_bklg_tbl->condition('lcpebd.id', encryption_decryption_fun("decrypt", $backlog_id));
	$get_bklg_tbl->condition('lcpebd.status', 0);
	$get_bklg_tbl_res =	$get_bklg_tbl->execute();	
			
	if( $get_bklg_tbl_res->rowCount() > 0 ){	
		$get_bklg_tbl_res_data = $get_bklg_tbl_res->fetchObject();
	
		$form['clra_backlog_master'] = array(
							'#prefix' => '<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">Manual/Offline registration number</div><div class="panel-body">',
							'#attributes' => array('class'=>array('')),
						);
		 
	$form['clra_backlog_master']['regn_number'] = array(							
								'#type' 		 => 'hidden',
								'#required'		 =>	TRUE,
								'#default_value' => encryption_decryption_fun("encrypt", $get_bklg_tbl_res_data->registration_no),								
							  );
	$form['clra_backlog_master']['regn_number_display'] = array(
								'#prefix' 		 => '<div class="col-md-6"><label class="input">',
								'#suffix' 		 => '</label></div>',
								// '#title'		 => 'Manual/Offline registration number',
								'#type' 		 => 'textfield',
								'#required'		 =>	TRUE,
								'#default_value' => $get_bklg_tbl_res_data->registration_no,
								'#attributes'	 => array('autocomplete' => 'off', 'readonly' => 'readonly')
							);
	
	$form['clra_backlog_master']['note'] = array(
								'#prefix' 		=> '<div class="col-md-12">',
								'#suffix' 		=> '</div>',
								// '#markup'		=> '<font color="red"><b>Note :- If you already have an Offline Registration Number Under Contract Labour (Regulation & Abolition) Act 1970, provide the Offline Registration Number and get a System Generated Registration Number.</b></font>',
								);
	
	$form['clra_backlog_master']['submit'] = array (
								'#prefix' 		=> '</div></div>',  
								'#type' 		=> 'submit',
								'#value' 		=> 'Continue ',
								'#attributes'	=> array('id' => 'submit_oneId' , 'class' => array('btn btn-primary pull-right'))
							);
															
	$form['clra_backlog_master']['link'] = array (
								'#suffix' 		=> '</div>', 
								'#type' 		=> 'markup',
								'#markup' 		=> l(t('<< Back'),'clra_backlog/clra-registration-type', array('html' => TRUE,'attributes'=>array('title' => '', 'class' => 'link-btn')))
								);	
															
	}else{
		form_set_error('', t('Invalid registration number. Please enter valid registration number.'));
	}
							
	return $form;
}

function clra_backlog_check_part_amnd_submit($form, &$form_state){
	global $base_root, $base_path, $user;
	$exitrow = db_query("select count(id) as total_row FROM l_clra_registration_master WHERE user_id = '".$user->uid."' AND ((status is null) OR (status IN ('', '0', 'B', 'BI', 'F', 'V', 'VA', 'S', 'I')))")->fetchObject();		
	
	if($exitrow->total_row > 0 ){	// Yes
		drupal_goto('applicant-dashboard');
	}else{				
		
		$val =	$form_state['values'];
		$registration_no = encryption_decryption_fun("decrypt", trim($val['regn_number']));
		
		if(!empty($registration_no)){
			
			$fetchExistingIdenNumber		=	db_select('l_clra_registration_master', 'lcrm');
			$fetchExistingIdenNumber->fields('lcrm', array());
			$fetchExistingIdenNumber->condition('lcrm.registration_number', trim($registration_no));
			$fetchExistingIdenNumber->condition('lcrm.user_id', $user->uid);
			$fetchExistingIdenNumber_res	=	$fetchExistingIdenNumber->execute();
			$fetchExistingIdenNumber_data 	= 	$fetchExistingIdenNumber_res->fetchObject();
			
			if( $fetchExistingIdenNumber_res->rowCount() > 0 ){	// Yes			
				// $edit_id 	= 	$fetchExistingIdenNumber_data->id;
				// $edit_id	= encryption_decryption_fun('encrypt', $edit_id);
				// drupal_goto('view-clra-application-details/view-clra-application/'.$edit_id);
				drupal_goto('applicant-dashboard');
			}else{	// No			
				$get_bklg_tbl =	db_select('l_clra_principle_emp_backlog_data', 'lcpebd');
				$get_bklg_tbl->fields('lcpebd', array());
				$get_bklg_tbl->condition('lcpebd.registration_no', $registration_no);
				$get_bklg_tbl_res		=	$get_bklg_tbl->execute();
				$get_bklg_tbl_res_data 	= 	$get_bklg_tbl_res->fetchObject();
				
				if( $get_bklg_tbl_res->rowCount() > 0 ){
					if( $get_bklg_tbl_res_data->status == 1 ){
						form_set_error('', t('Invalid registration number. Please enter valid registration number.'));					
					}elseif( $get_bklg_tbl_res_data->status == 0 ){  // if 0					
						$id								=	$get_bklg_tbl_res_data->id;
						$registration_no				=	$get_bklg_tbl_res_data->registration_no;
						$registration_date				=	$get_bklg_tbl_res_data->registration_date;
						$name_of_the_establishment		=	$get_bklg_tbl_res_data->name_of_the_establishment;
						$address_of_the_establishment	=	$get_bklg_tbl_res_data->address_of_the_establishment;
						$name_of_the_principle_emp		=	$get_bklg_tbl_res_data->name_of_the_principle_emp;
						$address_of_the_principle_emp	=	$get_bklg_tbl_res_data->address_of_the_principle_emp;
						$maximum_no_of_contact_labour	=	$get_bklg_tbl_res_data->maximum_no_of_contact_labour;
						$finalfees						=	$get_bklg_tbl_res_data->fees;
			
						$fieldsBackLogRegInfo = array(
												'user_id'							=>	$user->uid,
												'identification_number'				=>	'CLRA'.time(),
												'e_name'							=>	$name_of_the_establishment,
												'e_postal_address'					=>	$address_of_the_establishment,
												'full_name_principal_emp'			=>	$name_of_the_principle_emp,
												'address_principal_emp'				=>	$address_of_the_principle_emp,
												'backlog_id'						=>  $id,
												// 'finalfees'							=>  $finalfees,
												'act_id'							=>  1,
												// 'status'							=>  '0',
												'e_any_day_max_num_of_workmen'		=>  $maximum_no_of_contact_labour
											);
						$last_id =	db_insert('l_clra_registration_master')->fields($fieldsBackLogRegInfo)->execute();;
						$edit_id = 	$fetchExistingIdenNumber_data->id;
						if( $query != "" ){
							$fieldsBackLogUpdateInfo = array(
														'status'		=> '2',
														'date_time'		=> time()
													);
							
							$query = db_update('l_clra_principle_emp_backlog_data');
							$query->fields($fieldsBackLogUpdateInfo); 
							$query->condition('id', $id);
							$query->execute();
						}
						$last_id	= encryption_decryption_fun('encrypt', $last_id);
						drupal_goto('view-clra-application-details/view-clra-application/'.$last_id);
					}/*elseif( $get_bklg_tbl_res_data->status == 2 ){  // if 0
						form_set_error('', t('UNDER PROCESSING.CONTACT ALC.'));
					}*/
				}else{
					// form_set_error('', t('REGISTRATION NUMBER DOES NOT EXIST.PLEASE ENTER ORIGINAL REGISTRATION NUMBER ELSE YOU CAN CLICK SKIP BUTTON.'));
					drupal_goto('apply-clra-registration-backlog');
				}	
			}	
		}
	}
}