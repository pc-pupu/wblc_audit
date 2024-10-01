<?php

function upload_doc_form_contractor($form, &$form_state, $id=''){
	
	global $base_root, $base_path, $user;
	
	$user_id 						= 	$user->uid;
	$action							=  'decrypt';
	$application_id					= encryption_decryption_fun($action, $id);
	
	$fetch_data_from_clra_master 	= 	db_query("select lcrm.identification_number from l_clra_registration_master lcrm where lcrm.user_id=:user_id and lcrm.id=:applicationid", array(':user_id' => $user_id, ':applicationid' =>$application_id));
		
	if( $fetch_data_from_clra_master->rowCount() > 0 ){
		
		$fetch_data					=	$fetch_data_from_clra_master->fetchAssoc();
		$identification_number		=	$fetch_data['identification_number'];
		
	}
	
	$path 							= drupal_get_path('module', 'applicant_forms');
	
	require_once("./$path/Classes/PHPExcel.php");
	require_once("./$path/Classes/PHPExcel/IOFactory.php");
	
	$form['application_id'] 	  	= array(
										  '#type' 				=> 'hidden',
										  '#default_value'		=> !empty($application_id) ? $application_id : '',
										  '#attributes' 		=> array('readonly'=>'readonly')
									  );
									
	$form['identification_number'] 	= array(
										  '#type' 				=> 'hidden',
										  '#default_value'		=> !empty($identification_number) ? $identification_number : '',
										  '#attributes' 		=> array('readonly'=>'readonly')
									  );
									  
	$form['post_procure_awbfile_markup'] = array(
										'#type' => 'markup',
										'#markup' => '<font color="red">In case of multiple contractors to be added, please click <a href="'.$base_root.$base_path.'/sites/all/modules/applicant_forms/download/CONTRACTOR-INFORMATION.xlsx">here</a> to download format for excel file. After filling of all contractors in the specified format, upload the document in the space provided.<br><br/>
										<strong>Note1:</strong> The field for "<i>Estimated start date of employment of each contract work under each contractor</i>&nbsp;&amp;&nbsp;<i>Estimated end date of employment of each contract work under each contractor</i>" in the excel sheet should be in any date format.<br><br/>										
										<strong>Note2:</strong> For the field name "<i>Nature of work in which contract labour is employed or is to be employed</i>" in the excel sheet, in case of multiple inputs for Nature of Work, you have to add nature of work code by giving "," (Comma). Click '.l('here', 'nature-of-work', array('html' => TRUE,'attributes'=>array('target' => '_blank'))).' to view the code for Nature of Work.<br><strong>Example</strong>,  Input given as " 1,2,3 " which means Code for 1 means -  <u>Security</u>, Code for 2 means -  <u>Water Treatment Operation</u>, Code for 3 means -  <u>Maintenance and Managerial Jobs</u>.<br/><br/>
										<strong>Note3:</strong> For the field name "<i>Police Station </i>" in the excel sheet, please search your respective police station starting with code number PS in sheet 1 and put it in your respective police station field in your excel sheet.<br><br/>
										<strong>Note4:</strong> Click '.l("here", "gp-and-ward-list", array('html' => TRUE,'attributes'=>array('target' => '_blank'))).' for view  Gram Panchayat or Ward code.</font><br>');
												
	$form['post_procure_awbfile'] = array(
										'#type' => 'file',
										'#title' => t('Select Contractor File'),
										'#description' => '<br/><font color="red"><i>.xlsx is only a valid extension</i></font>'
    								);
									
	
																
    $form['post_procure_paisareport'] = array(
											'#type' => 'submit', 
											'#value' => t('Upload')
    									);
	
	$form['back_to_contractor_list'] = array(
										'#type' => 'markup',
										'#markup' => '&nbsp;'.l('BACK', 'view-clra-application-details/clra-contractor-info/'.$id,  array('html' => TRUE))
									);
										
	return $form;
}

function upload_doc_form_contractor_validate($form, &$form_state){
	$file 								= 	file_save_upload('post_procure_awbfile', array('file_validate_extensions' => array('xlsx')));
	$path 								= 	drupal_get_path('module', 'applicant_forms');
	require_once("./$path/Classes/PHPExcel.php");
	require_once("./$path/Classes/PHPExcel/IOFactory.php");
        
    if ($file) {
      if ($file 						= 	file_move($file, 'public://upload/csv')) {
        $form_state['values']['file'] 	= 	$file;
			global $base_root, $base_path, $user;
			$file 									= 	$form_state['values']['file'];
			$file->status 							= 	FILE_STATUS_PERMANENT;
			$filepath 								= 	drupal_realpath($file->uri);
			$objReader 								= 	PHPExcel_IOFactory::createReader('Excel2007');
			$objReader->setReadDataOnly(true);
			$objPHPExcel 							= 	$objReader->load($filepath);
			$objWorksheet 							= 	$objPHPExcel->getActiveSheet();
			$maxCell 								= 	$objWorksheet->getHighestRowAndColumn();
			$data 									= 	$objWorksheet->rangeToArray('A2:' . $maxCell['column'] . $maxCell['row']);
			$data 									= 	array_map('array_filter', $data);
			$data 									= 	array_filter($data);
			$final_data['CONTRACTOR-INFORMATION']	=	$data;
			$application_id							=	$form_state['values']['application_id'];
			$identification_number					=	$form_state['values']['identification_number'];
								
			if(!empty($application_id) && is_numeric($application_id)){
				if(!empty($identification_number)){
					$result 								= 	count($final_data['CONTRACTOR-INFORMATION']); 
										
					$final_data = $final_data['CONTRACTOR-INFORMATION'];				
					
					$final_array_data = $final_data['CONTRACTOR-INFORMATION'];
										
					for($count=1; $count<=$result; $count++){						
						$row = 1;					
						foreach($final_data as $val){				
														
							$date_formated_from 						= 	date('Y-m-d', PHPExcel_Shared_Date::ExcelToPHP($val[8]));							
							$date_formated_to 							= 	date('Y-m-d', PHPExcel_Shared_Date::ExcelToPHP($val[9]));							
																					
							$name_of_contractor							=	$val[1]; 
							$email_of_contractor						=	$val[2];
							$con_loc_e_vill_ward						=	$val[5];
							$contractor_max_no_of_labours_on_any_day	=	$val[7];
							$police_station_code						=	$val[10]; // For Police station							
							
							$emailval 									= 	'/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,4})$/';
							$match_pattern 								= 	'/[@\$\%\,\_\:]+/';
							$date_pattern								=	'/^([0-9]{4,4})+\-([0-9]{2,2})+\-([0-9]{2,2})+$/';
						
							if( !preg_match($emailval, $email_of_contractor) ){ 
								form_error($form['post_procure_awbfile'], t('Valid email should be given on row number '.$row.'.'));
							}else if(empty($val[4]) && !is_numeric($val[4]) ){
								form_error($form['post_procure_awbfile'], 'Please enter pin number.');
							}else if( !is_numeric($con_loc_e_vill_ward) ){
								form_error($form['post_procure_awbfile'], t('Valid Gram Panchayat should be numeric on row number '.$row.'.'));
							}else if(!is_numeric($val[8])){
								form_error($form['post_procure_awbfile'], 'Please enter valid start date format(dd-mm-yyyy) on row number '.$row.'.');								
							}else if(!is_numeric($val[9]) ){
								form_error($form['post_procure_awbfile'], 'Please enter valid end date format(dd-mm-yyyy) on row number '.$row.'.');
							}else if( !is_numeric($contractor_max_no_of_labours_on_any_day) ){
								form_error($form['post_procure_awbfile'], t('Valid maximum number of contract labour to be employed on any day through each contractor should be numeric on row number '.$row.'.'));
							}else if(preg_match($match_pattern, $name_of_contractor) ) {
								form_error($form['post_procure_awbfile'], 'Name of the Contractor SHOULD BE TEXT ONLY on row number '.$row.'.');
							}else if( !preg_match($date_pattern, $date_formated_from) ) {
								form_error($form['post_procure_awbfile'], 'Please enter valid start date format(dd-mm-yyyy) on row number '.$row.'.');
							}
							else if( !preg_match($date_pattern, $date_formated_to) ) {
								form_error($form['post_procure_awbfile'], 'Please enter valid end date format(dd-mm-yyyy) on row number '.$row.'.');
							// }else if( !is_numeric($police_station_code) ){
								// form_error($form['post_procure_awbfile'], t('Police Station should be numeric on row number '.$row.'.'));
							}
							$row++;							
						}										
						break;
					}
				}else{
					form_error($form['post_procure_awbfile'], t( 'Identification Number is empty.'));
				}
			}else{
				form_error($form['post_procure_awbfile'], t( 'Application Number is empty.'));
			}
		}else {
        form_set_error('file', t('Failed to write the uploaded file the site\'s file folder.'));
      }
    }
}

function upload_doc_form_contractor_submit($form, &$form_state){
	
	global $base_root, $base_path, $user;
	$path 									= 	drupal_get_path('module', 'applicant_forms');
	require_once("./$path/Classes/PHPExcel.php");
	require_once("./$path/Classes/PHPExcel/IOFactory.php");
	$file 									= 	$form_state['values']['file'];
    $file->status 							= 	FILE_STATUS_PERMANENT;
    $filepath 								= 	drupal_realpath($file->uri);
	$objReader 								= 	PHPExcel_IOFactory::createReader('Excel2007');
	$objReader->setReadDataOnly(true);
	$objPHPExcel 							= 	$objReader->load($filepath);
	$objWorksheet 							= 	$objPHPExcel->getActiveSheet();
	$maxCell 								= 	$objWorksheet->getHighestRowAndColumn();
	$data 									= 	$objWorksheet->rangeToArray('A2:' . $maxCell['column'] . $maxCell['row']);
	$data 									= 	array_map('array_filter', $data);
	$data 									= 	array_filter($data);
	$final_data['CONTRACTOR-INFORMATION']	=	$data;
	$application_id							=	$form_state['values']['application_id'];
	$identification_number					=	$form_state['values']['identification_number'];
	
	$total_contractor_max_no_of_labours = 0;
	
	if(!empty($application_id) && is_numeric($application_id)){
		if(!empty($identification_number)){			
			for($count=1; $count<= count($final_data['CONTRACTOR-INFORMATION']); $count++){
				foreach($final_data['CONTRACTOR-INFORMATION'] as $val){
					$contractor_max_no_of_labours	=	$val[7];					
					$total_contractor_max_no_of_labours = $total_contractor_max_no_of_labours + $contractor_max_no_of_labours;
				}break;
				
			}
		}
	}
	$total_contractor_max_no_of_labours_pe = db_query("select e_any_day_max_num_of_workmen from l_clra_registration_master where id=:applicationid", array(':applicationid' => $application_id));
	$total_contractor_max_no_of_labours_pe_row 	= $total_contractor_max_no_of_labours_pe->fetchObject();
	$total_contractor_max_no_of_labours_pe = $total_contractor_max_no_of_labours_pe_row->e_any_day_max_num_of_workmen;
	
	if($total_contractor_max_no_of_labours_pe <= $total_contractor_max_no_of_labours ){
		form_error($form['post_procure_awbfile'], t( 'Total number of contract labour in the Excel Sheet exceeded '.$total_contractor_max_no_of_labours_pe.' If you want to upload this excel sheet, please increase the value for field "Maximum number of contract labour to be employed on any day through each contractor" in Application Details Tab.'));
	}else{	
	
	if(!empty($application_id) && is_numeric($application_id)){
		if(!empty($identification_number)){
			$result 								= 	count($final_data['CONTRACTOR-INFORMATION']);
			for($count=1; $count<=$result; $count++){
				foreach($final_data['CONTRACTOR-INFORMATION'] as $val){
					$date_formated_from 						= 	date('Y-m-d', PHPExcel_Shared_Date::ExcelToPHP($val[8]));
					$date_formated_to 							= 	date('Y-m-d', PHPExcel_Shared_Date::ExcelToPHP($val[9]));
					$name_of_contractor							=	$val[1]; 
					$email_of_contractor						=	$val[2];
					$con_loc_e_vill_ward						=	$val[5];
					$contractor_max_no_of_labours_on_any_day	=	$val[7];
					$police_station_code						=	$val[10];
					
					/*$date1										=	date_create($date_formated_from);
					$date2										=	date_create($date_formated_to);
					$date_diff_code								=	date_diff($date1,$date2);*/
					
										
					$query1 									= 	db_query("select * from village_ward_master where village_code=:vill_code", array( ":vill_code" => $con_loc_e_vill_ward ));
					if( $query1->rowCount() > 0 ){
						$query1_data							=	$query1->fetchAssoc();
						$block_code								=	$query1_data['block_code'];
						if( $block_code != "" ){
							$query2 							= 	db_query("select * from block_mun_master where block_code=:blck_code", array( ":blck_code" => $block_code ));
							if( $query2->rowCount() > 0 ){
								$query2_data					=	$query2->fetchAssoc();
								$district_code					=	$query2_data['district_code'];
								$sub_division_code				=	$query2_data['sub_division_code'];
								$type							=	$query2_data['type'];
								
								$fieldsarray					=	array(
																						'user_id'										=> $user->uid,
																						'contractor_user_id'							=> $user->uid,
																						'identification_number'							=> $identification_number,
																						'act_id'										=> 1,
																						'application_id'								=> $application_id,
																						'name_of_contractor' 							=> $name_of_contractor,
																						'email_of_contractor'							=> $email_of_contractor,
																						'address_of_contractor' 						=> $val[3],
																						'contractor_pin' 								=> (int)$val[4],
																						'contractor_max_no_of_labours_on_any_day' 		=> $contractor_max_no_of_labours_on_any_day,
																						'est_date_of_work_of_each_labour_from_date' 	=> $date_formated_from,
																						'est_date_of_work_of_each_labour_to_date' 		=> $date_formated_to,
																						'con_loc_e_dist'								=> $district_code,
																						'con_loc_e_subdivision'							=> $sub_division_code,
																						'con_l_e_ps'									=> $police_station_code,
																						'con_loc_e_areatype'							=> $type,
																						'con_name_areatype'								=> $block_code,
																						'con_loc_e_vill_ward' 							=> $con_loc_e_vill_ward,
																						'created_by'									=> 0,
																						'status'										=> 1,																						
																						'amendment_id'									=> 0,
																						'contractor_type'								=> 1,
																					);
								
								// print_r($fieldsarray); die;																
								$last_id 						= 	db_insert('l_contractor_info_master')->fields($fieldsarray)->execute();
								$x 								= 	explode(",", $val[6]);
								foreach($x as $data){
									$fieldsarray_more			=	array(
																						'user_id'										=> $user->uid,
																						'application_id'								=> $application_id,
																						'contractor_id' 								=> $last_id,
																						'identification_number'							=> $identification_number,
																						'contractor_nature_of_work'						=> $data,
																						'act_id'										=> 1
																					);
									db_insert('l_clra_con_nature_of_work')->fields($fieldsarray_more)->execute();
								}
							}
						}
					}else{
						$msg =	"Gram panchayat code invalid.";
						drupal_set_message($msg);
						break;
					}
				}
				break;
			}
			$msg			=	"Successfully Inserted";
			drupal_set_message($msg);
		}else{
			form_error($form['post_procure_awbfile'], t( 'Identification Number is empty.'));
		}
	}else{
		form_error($form['post_procure_awbfile'], t( 'Application Number is empty.'));
	}
	}
}
