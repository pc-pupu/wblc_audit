<?php

function add_backlog_pe_data($form, &$form_state){
	
	global $base_root, $base_path, $user;
	
	$user_id  =  $user->uid;
	
	$form['post_procure_awbfile_markup'] = array(
										'#type' => 'markup',
										'#markup' => '<font color="red">Before the uploading, please click <a href="'.$base_root.$base_path.'sites/all/modules/alc/download/data.xlsx">here</a> and download the excel file. After filling up the establishment details in the specified format then upload the this file.<br>');
												
	$form['post_procure_awbfile'] = array(
										'#type' => 'file',
										'#title' => t('Select Excel File'),
										'#size' => 22,
										'#description' => '<br/><font color="red">#Note : .xlsx is only a valid extension</font>'
    								);									
																	
    $form['submit'] = array(
										'#type' => 'submit', 
										'#value' => t('Upload')
    								);
	return $form;	
}

function add_backlog_pe_data_validate($form, &$form_state){
	$file = file_save_upload('post_procure_awbfile', array('file_validate_extensions' => array('xlsx')));
	$path = drupal_get_path('module', 'applicant_forms');
	require_once("./$path/Classes/PHPExcel.php");
	require_once("./$path/Classes/PHPExcel/IOFactory.php");
        
    if($file){
       if($file = file_move($file, 'public://upload/csv')){
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
			$final_data['BACK_LOG_DATA']			=	$data;
			
			$result 								= 	count($final_data['BACK_LOG_DATA']);
			
			$final_validate							=	$final_data['BACK_LOG_DATA'];
			$row 									= 	1;
			for($count=1; $count<=$result; $count++){
				
				foreach($final_validate as $val){
					$rowNumber						=	$row++;
					$registration_no				= $val[1];
					$registration_date				= date('Y-m-d', PHPExcel_Shared_Date::ExcelToPHP($val[2]));
					$name_of_the_establishment		= $val[3];
					$address_of_the_establishment	= $val[4];
		
					$block_mun_id_establishment		= $val[5];
					$name_of_the_principle_emp		= $val[6];
					$address_of_the_principle_emp	= $val[7];
					
					$block_mun_id_pe				= $val[8];
					$maximum_no_of_contact_labour	= $val[9];
					$fees							= $val[10];
					
					if( $registration_no == "" ){
						form_error($form['post_procure_awbfile'], t('Valid registration number should be given on row number '.$rowNumber.'.'));
					}else if( $registration_date == "" ){
						form_error($form['post_procure_awbfile'], t('Valid registration date should be given on row number '.$rowNumber.'.'));
					}else if( $name_of_the_establishment == "" ){
						form_error($form['post_procure_awbfile'], t('Valid name of establishment should be given on row number '.$rowNumber.'.'));
					}else if( $address_of_the_establishment == "" ){
						form_error($form['post_procure_awbfile'], t('Valid address of establishment should be given on row number '.$rowNumber.'.'));
					}else if(empty($block_mun_id_establishment)){
						form_error($form['post_procure_awbfile'], t('Valid block/Municipality number should be given on row number '.$rowNumber.'.'));
					}else if( $block_mun_id_pe == "" ){
						form_error($form['post_procure_awbfile'], t('Valid block/Municipality number should be given on row number '.$rowNumber.'.'));
					}else if( $maximum_no_of_contact_labour == "" && !is_numeric($maximum_no_of_contact_labour) ){
						form_error($form['post_procure_awbfile'], t('Maximum Number of Contract labour is not number on row number '.$rowNumber.'.'));
					}else if( $fees == "" && !is_numeric($fees) ){
						form_error($form['post_procure_awbfile'], t('Fees is not number on row number '.$rowNumber.'.'));
					}
					
					$pe_name_pattern 		= 	'/[@\$\%\,\_\:]+/';
					$pe_address_pattern 	= 	'/[@\$\%\_\:]+/';
					if( $name_of_the_principle_emp == "" ){
						form_error($form['post_procure_awbfile'], t('Name of the Principle Employer is empty on row number '.$rowNumber.'.'));
					}else if( preg_match($pe_name_pattern, $name_of_the_principle_emp) ){
						form_error($form['post_procure_awbfile'], t('Name of the Principle Employer contains special characters on row number '.$rowNumber.'.'));
					}else if( $address_of_the_principle_emp == "" ){
						form_error($form['post_procure_awbfile'], t('Address of the Principle Employer is empty on row number '.$rowNumber.'.'));
					}else if( preg_match($pe_address_pattern, $address_of_the_principle_emp) ){
						form_error($form['post_procure_awbfile'], t('Address of the Principle Employer contains special characters on row number '.$rowNumber.'.'));
					}
					
				}
			}
	   }
	}
	
}

function add_backlog_pe_data_submit($form, &$form_state){
	//$file = file_save_upload('post_procure_awbfile', array('file_validate_extensions' => array('xlsx')));
	$path = drupal_get_path('module', 'applicant_forms');
	require_once("./$path/Classes/PHPExcel.php");
	require_once("./$path/Classes/PHPExcel/IOFactory.php");
        
    //if($file){
       //if($file = file_move($file, 'public://upload/csv')){
      		//$form_state['values']['file'] 	= 	$file;
			global $base_root, $base_path, $user;
			$data = array();
			$val  = array();
			
			
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
			$final_data['BACK_LOG_DATA']			=	$data;
			
			
			$data = $final_data['BACK_LOG_DATA'];
			
			$total_row = 	count($data);	
			
			for($count=0; $count<=$total_row; $count++){
				$val = $data[$count];
				if(!empty($val[1])){	
				$query 	= db_select('l_clra_principle_emp_backlog_data', 'cpebd');	
						  $query->fields('cpebd', array('id'));
						  $query->condition('cpebd.registration_no',trim($val[1]),'=');

				$query_result = $query->execute();
				
				
				//
				
				$query_bmm 	= db_select('block_mun_master', 'bmm');	
				$query_bmm->fields('bmm', array('id'));
				$query_bmm->condition('bmm.block_code',trim($val[5]),'=');

				$query_result_bmm = $query_bmm->execute();		
				
				if( $query_result_bmm->rowCount() < 1){					
					drupal_set_message('Invalid BLOCK, MUNICIPILITY, N/A AND SEZ CODE (FROM BLOCK NAME AND CODE SHEETS) of the Establishment on sl. no. '.$val[0]);
					break;
				}else{	
					$query_bmm1 	= db_select('block_mun_master', 'bmm');	
					$query_bmm1->fields('bmm', array('id'));
					$query_bmm1->condition('bmm.block_code',trim($val[8]),'='); 

					$query_result_bmm1 = $query_bmm1->execute();
					
					if( $query_result_bmm1->rowCount() < 1){					
						drupal_set_message('Invalid BLOCK, MUNICIPILITY, N/A AND SEZ CODE (FROM BLOCK NAME AND CODE SHEETS) of the Principal Employer on sl. no. '.$val[0]);
						break;
					}else{ 
										
				if($query_result->rowCount() < 1){
					if(!empty($val[5])){  					
					$subdivision_code_establishment = get_area_code('block_mun_master', 'block_code',trim($val[5]), 'sub_division_code');
					$district_code_establishment 	= get_area_code('sub_division', 'sub_div_code', $subdivision_code_establishment, 'district_code');
					
					$subdivision_code_pe = get_area_code('block_mun_master', 'block_code',trim($val[8]), 'sub_division_code'); 
					$district_code_pe 	 = get_area_code('sub_division', 'sub_div_code', $subdivision_code_pe, 'district_code');
					
					$backlog_pe_data	=	array(
											'registration_no'				=> trim($val[1]),
											'registration_date'				=> date('Y-m-d', PHPExcel_Shared_Date::ExcelToPHP($val[2])),
											'name_of_the_establishment'		=> $val[3],
											'address_of_the_establishment'	=> $val[4],
											'district_code_establishment'	=> $district_code_establishment,
											'subdivision_id_establishment'	=> $subdivision_code_establishment,
											'block_mun_id_establishment'	=> trim($val[5]),
											'name_of_the_principle_emp'		=> $val[6],
											'address_of_the_principle_emp'	=> $val[7],
											'district_code_pe'				=> $district_code_pe,
											'subdivision_id_pe'				=> $subdivision_code_pe,
											'block_mun_id_pe'				=> trim($val[8]),
											'maximum_no_of_contact_labour'	=> $val[9],
											'fees'							=> trim($val[10]),
											'user_id'						=> $user->uid,
											'status'						=> '0',
											'created_date'					=> time()																				
										);						
					
					//print_r($backlog_pe_data);									
					$last_id  =  db_insert('l_clra_principle_emp_backlog_data')->fields($backlog_pe_data)->execute();											
				}
				}
				}
				}			
			} 			
		}
		$message = 'Principal Employer data successfully updated.';
		drupal_set_message($message);
		drupal_goto('clra-backlog-details');
}

function get_area_code($table_name, $field_name, $value, $return_field){	

	$query 	= db_select($table_name, 'tbl');	
			  $query->fields('tbl', array($return_field));
			  $query->condition('tbl.'.$field_name,trim($value),'=');
	
	$query_result = $query->execute();	
		
	$data		  = $query_result->fetchObject();
	if(count($data) > 0){
		return trim($data->$return_field);
	}else{
		return NULL;
	}
	
}

/*
function update_office_addres($form, &$form_state){
	global $base_root, $base_path, $user;
	
	$user_id = $user->uid;
	
	$clra_certificate = db_select('l_rlo_additional_info', 'lrai');
	$clra_certificate->InnerJoin('l_customuser_relation_address', 'lcra','lcra.sub_div_code= lrai.sub_div_code'); 	
	$clra_certificate->fields('lrai', array('clra_certificate_address'));
	$clra_certificate->fields('lcra', array('sub_div_code'));
	$clra_certificate->condition('lcra.user_id', $user_id,'=');
	
	$result = $clra_certificate->execute()->fetchAssoc();	
												
	$form['office_addres']['clra_certificate_address'] = array(
									  '#title' 			=> t('Enter Office address'),
									  '#type' 			=> 'textarea',
									  '#cols' 			=> 35, 
									  '#rows' 			=> 5, 
									  '#attributes' 	=> array('placeholder' => 'Enter Office address'),
									  '#required' 		=>  TRUE,
									  '#default_value' 	=> $result['clra_certificate_address']
    								);																
    
	 $form['office_addres']['sub_div_code'] = array(
									  '#type' 		=> 'hidden', 
									  '#value' 		=> $result['sub_div_code']
    								);
	
	$form['office_addres']['submit'] = array(
									  '#type' 		=> 'submit', 
									  '#value' 		=> t('Update')
    								);
	return $form;
}

function update_office_addres_submit($form, &$form_state){
	global $base_root, $base_path, $user;
	
	$val = $form_state['values'];	
	
	$fields_Array	=	array(								
							'clra_certificate_address' => $val['clra_certificate_address']							
						   );	
	
	$query = db_update('l_rlo_additional_info');
	$query->fields($fields_Array); 
	$query->condition('sub_div_code', $val['sub_div_code']);
	$query->execute();
	
	$message = 'Update successfully.';
	drupal_set_message(t($message));
	
	drupal_goto('alcdashboard/rlo-details');
}*/