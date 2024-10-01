<?php

drupal_add_js(drupal_get_path('module', 'alc') . '/js/mycustom.js');
drupal_add_css(drupal_get_path('module', 'addlc') . '/css/style.css');


function karmasathi_migrant_list($usertype = '', $par = ''){
	//die('ok');
	global $base_root, $base_path, $user;
	
	$table = $apply_category = $unit_name = $actionStatus = '';
	$user_id = $user->uid;

	$counter = 0;

	$fetch_users_role = db_select('users_roles', 'ur');
	$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role->fields('ro', array('rid', 'name'));
	$fetch_users_role->condition('ur.uid', $user_id);
	$fetch_users_role_data = $fetch_users_role->execute()->fetchAssoc();

	//print_r($fetch_users_role_data);die;
	$user_role = $fetch_users_role_data['rid'];
	$role_name = $fetch_users_role_data['name'];

	
	$get_area_codes = db_select('l_customuser_relation_address', 'lcra');
	$get_area_codes->fields('lcra', array('district_code', 'sub_div_code', 'block_code', 'custom_area_jurisdiction', 'is_custom_area_jurisdiction', 'area_type'));
	$get_area_codes->condition('lcra.user_id', $user_id);
	$get_area_codes_result = $get_area_codes->execute()->fetchAssoc();

	
	if (!empty($get_area_codes_result) && !empty($role_name)) {
		$district_code 		= $get_area_codes_result['district_code'];
		$subdivision_code 	= $get_area_codes_result['sub_div_code'];
		$block_code 		= $get_area_codes_result['block_code'];
		$area_type 			= $get_area_codes_result['area_type'];
		$customjurisdiction = $get_area_codes_result['custom_area_jurisdiction'];
		$iscustom_juris 	= $get_area_codes_result['is_custom_area_jurisdiction'];
	}


		
	
	$query = db_select('block_mun_master', 'bmm');
	//$query->leftJoin('block_mun_master', 'bmm', 'bmm.block_code=mjm.permanent_areacode');
	$query->fields('bmm', array('block_mun_name'));	
	if ($user_role == 5) { 
		if ($customjurisdiction == 'SUBDIV' && $iscustom_juris == 'Y') {
			$subdv_arr = db_select('l_customuser_reletion_services_wise_user_settings', 'cra')->fields('cra', array('sub_div_code'))->condition('cra.user_id', $user_id)->execute()->fetchObject()->sub_div_code;
			$area_arr = explode(',', $subdv_arr);

		}else {
			$area_arr = db_select('l_customuser_relation_address', 'cra')->fields('cra', array('district_code'))->condition('cra.user_id', $user_id)->execute()->fetchAssoc();
			
		}

	}else if ($user_role == 4) {
		if ($customjurisdiction == 'BMCNS' && $iscustom_juris == 'Y') {
			$block_arr = db_select('l_customuser_reletion_services_wise_user_settings', 'cra')->fields('cra', array('block_code'))->condition('cra.user_id', $user_id)->execute()->fetchObject()->block_code;
			$area_arr = explode(',', $block_arr);
				
		}else{				
			$area_arr = db_select('l_customuser_relation_address', 'cra')->fields('cra', array('sub_div_code'))->condition('cra.user_id', $user_id)->execute()->fetchAssoc();				
			
		}

	}else if ($user_role == 7) {
		$area_arr = db_select('l_customuser_relation_address', 'cra')->fields('cra', array('block_code'))->condition('cra.user_id', $user_id)->execute()->fetchObject()->block_code;			
		
	}
	
	//print_r($area_arr);die;
	$query_details = $query->execute()->fetchAll();
	
	if($par == 'approved'){
		$status = array('C');
	}elseif ($par == 'pending') {
		$status = array('A','BA');
	}elseif ($par == 'provisional') {
		$status =  array('P','BP');
	}elseif ($par ==  'back') {
		$status =  array('B');
	}elseif ($par == 'reject') {
		$status =  array('R');
	}
	// $curl_post_data = array (
	// 	'source' 		=> 'WBLC',
	// 	'taskid' 		=> 'LCFETCHAPPLICATION',
	// 	'user_role' 	=> $user_role,
	// 	'block_subdiv' 	=> $area_arr,									
	// 	'user_id' 		=> $user_id,
	// 	'status' 		=> $status,							
	// );
	$curl_post_data = array (
		'source' 		=> 'WBLC',
		'taskid' 		=> 'LCFETCHAPPLICATION',
		'user_role' 	=> $user_role,
		'block_subdiv' 	=> $area_arr,									
		'user_id' 		=> $user_id,
		'status' 		=> $status						
	);
	//print_r(json_encode($curl_post_data));die;
		//$service_url = "http://202.61.117.237:80/karmasathips/rest/wblcofficial";

		$service_url = "https://164.100.199.8:443/rest/wblcofficial";
		$ch = curl_init();
		$headers = array();
		$headers[] = 'Accept: application/json';
		$headers[] = 'Content-Type: application/json';
		$headers[] = 'charset=utf-8';
		curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
		curl_setopt($ch, CURLOPT_URL, $service_url);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
		curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 0);
		curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($curl_post_data));
		curl_setopt($ch, CURLOPT_POST, true);
		$results    = curl_exec($ch);
		$errors     = curl_error($ch); 
		//print_r($errors); die;
		curl_close($ch);			
		$results = json_decode($results, TRUE); 
		//echo '<pre>'; print_r($results); die('ok');

		//print_r($results['content']);die;
	if($results['status_code'] == 200){
	 
		$file = drupal_get_path('module', 'genfunction') . "/PHPExcel/Classes/PHPExcel.php";
	$file2 = drupal_get_path('module', 'genfunction') . "/PHPExcel/Classes/PHPExcel/IOFactory.php";
  
	require_once $file;
	require_once $file2;
	//$heading_text = get_the_table();

    
    $objPHPExcel = new PHPExcel();
    $objPHPExcel->getProperties()->setCreator("NIC")
      ->setLastModifiedBy("WBLC")
      ->setTitle("Office 2007 XLSX Test Document")
      ->setSubject("Office 2007 XLSX Test Document")
      ->setDescription("Test document for Office 2007 XLSX, generated using PHP classes.")
      ->setKeywords("office 2007 openxml php")
      ->setCategory("Test result file");

    $sheet = $objPHPExcel->getActiveSheet();
    $default_border = array(
      'style' => PHPExcel_Style_Border::BORDER_THIN,
      'color' => array('rgb' => '000000')
    );
    $style_header = array(
      'borders' => array(
        'bottom' => $default_border,
        'left' => $default_border,
        'top' => $default_border,
        'right' => $default_border,
      ),
      'fill' => array(
        'type' => PHPExcel_Style_Fill::FILL_SOLID,
        'color' => array('rgb' => 'FFFF00'),
      ),
      'font' => array(
        'bold' => true,
      )
    );
    $style = array(
      'alignment' => array(
        'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
        'bold' => true,
        'style' => PHPExcel_Style_Border::BORDER_MEDIUM,
        'color' => array('rgb' => '1006A3')
      ),
      'borders' => array(
        'bottom' => $default_border,
        'left' => $default_border,
        'top' => $default_border,
        'right' => $default_border,
      ),
    );
	$sheet->mergeCells('A1:F1');
	
	$starting_pos = ord('A1');
	$sheet->setCellValue('A1', strtoupper(' karmasathi application ('.$par.')'));
    $sheet->setCellValue('A2', 'SL.No.');
	if($type=='DS'){
    $sheet->setCellValue('B2', 'DS ID');
	}
    $sheet->setCellValue('B2', 'MWIN');
    $sheet->setCellValue('C2', 'WORKER NAME');
    $sheet->setCellValue('D2', 'MOBILE NUMBER');
    if($user_role==7){
		$sheet->setCellValue('E2', 'ADDRESS');
		$sheet->setCellValue('F2', 'BLOCK');
		$sheet->setCellValue('G2', 'VILL / WARD');
		$sheet->setCellValue('H2', 'POLICE STATION');
		$sheet->setCellValue('I2', 'PIN CODE');
		$sheet->setCellValue('J2', 'STATUS');
	}else{
		$sheet->setCellValue('E2', 'STATUS');
	}
	
	if($user_role==7){
		$objPHPExcel->getActiveSheet()->getRowDimension('1')->setRowHeight(25);
		$objPHPExcel->getActiveSheet()->getRowDimension('2')->setRowHeight(25);
		$objPHPExcel->getActiveSheet()->getColumnDimension('A')->setWidth(10);
		$objPHPExcel->getActiveSheet()->getColumnDimension('B')->setWidth(25);
		$objPHPExcel->getActiveSheet()->getColumnDimension('C')->setWidth(10);
		$objPHPExcel->getActiveSheet()->getColumnDimension('D')->setWidth(15);
		$objPHPExcel->getActiveSheet()->getColumnDimension('E')->setWidth(30);
		$objPHPExcel->getActiveSheet()->getColumnDimension('F')->setWidth(40);
		$objPHPExcel->getActiveSheet()->getColumnDimension('G')->setWidth(30);
		$objPHPExcel->getActiveSheet()->getColumnDimension('H')->setWidth(30);
		$objPHPExcel->getActiveSheet()->getColumnDimension('I')->setWidth(30);
		$objPHPExcel->getActiveSheet()->getColumnDimension('J')->setWidth(30);
		

	}else{
		$objPHPExcel->getActiveSheet()->getRowDimension('1')->setRowHeight(25);
		$objPHPExcel->getActiveSheet()->getRowDimension('2')->setRowHeight(25);
		$objPHPExcel->getActiveSheet()->getColumnDimension('A')->setWidth(10);
		$objPHPExcel->getActiveSheet()->getColumnDimension('B')->setWidth(25);
		$objPHPExcel->getActiveSheet()->getColumnDimension('C')->setWidth(10);
		$objPHPExcel->getActiveSheet()->getColumnDimension('D')->setWidth(15);
		$objPHPExcel->getActiveSheet()->getColumnDimension('E')->setWidth(30);
	}
   
    //$objPHPExcel->getActiveSheet()->getColumnDimension('F')->setWidth(40);
	

	$objPHPExcel->getActiveSheet()->getStyle('B')->getNumberFormat()->setFormatCode('0');

    $sheet->getStyle('A1')->applyFromArray($style_header);
    $sheet->getStyle('B1')->applyFromArray($style_header);
    $sheet->getStyle('C1')->applyFromArray($style_header);
    $sheet->getStyle('D1')->applyFromArray($style_header);
	if($user_role==7){
		$sheet->getStyle('E1')->applyFromArray($style_header);
		$sheet->getStyle('F1')->applyFromArray($style_header);
		$sheet->getStyle('G1')->applyFromArray($style_header);
		$sheet->getStyle('H1')->applyFromArray($style_header);
		$sheet->getStyle('I1')->applyFromArray($style_header);
		$sheet->getStyle('J1')->applyFromArray($style_header);

	}else{
		$sheet->getStyle('E1')->applyFromArray($style_header);
	}

    $sheet->getStyle('A1')->applyFromArray(
      array(
        'alignment' => array(
          'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
          'vertical' => PHPExcel_Style_Alignment::VERTICAL_CENTER
        ),

      )
    );
    $sheet->getStyle('A')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_LEFT);
    $sheet->getStyle('I')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_LEFT);
    $sheet->getStyle('A2:K2')->applyFromArray(
      array(
        'borders' => array(
          'bottom' => $default_border,
          'left' => $default_border,
          'top' => $default_border,
          'right' => $default_border,
        ),
        'alignment' => array(
          'vertical' => PHPExcel_Style_Alignment::VERTICAL_CENTER
        ),
        'font' => array('bold' => true)
      )
    );
	$r = 3;
	$counter = 0;
	foreach ($results['content'] as $jobseeker_data) {
		
		$counter++;
		$application_id 		= $jobseeker_data['id'];
			
			$name 					= $jobseeker_data['name'];	
			$mobile 				= $jobseeker_data['mobile'];	
			$status 				= $jobseeker_data['status'];

			if($jobseeker_data['status'] == 'C'){
				$status_show = 'Approved';
				$identification_number 	= $jobseeker_data['identification_number'];
			}elseif ($jobseeker_data['status'] == 'P' || $jobseeker_data['status'] == 'BP') {
				$status_show = 'Provisional';
				$identification_number 	= $jobseeker_data['identification_number'].'(P)';
			}elseif ($jobseeker_data['status'] == 'A' || $jobseeker_data['status'] == 'BA') {
				$status_show = 'Pending for approval';
				$identification_number 	= $jobseeker_data['identification_number'];
			}elseif ($jobseeker_data['status'] == 'B') {
				$status_show = 'Back for correction';
				$identification_number 	= $jobseeker_data['identification_number'];
			}elseif ($jobseeker_data['status'] == 'R') {
				$status_show = 'Reject';
				$identification_number 	= $jobseeker_data['identification_number'];
			}




		$sheet->setCellValue('A' . $r, $counter);
		
		$sheet->setCellValue('B' . $r, $identification_number);
		$sheet->setCellValue('C' . $r, $name);
		$sheet->setCellValue('D' . $r, $mobile);
		if($user_role==7){
			$sheet->setCellValue('E' . $r,  ucwords($jobseeker_data['permanent_address']));
			$sheet->setCellValue('F' . $r,  ucwords($jobseeker_data['block_mun_name']));
			$sheet->setCellValue('G' . $r,  ucwords($jobseeker_data['village_ward_name']));
			$sheet->setCellValue('H' . $r,  ucwords($jobseeker_data['ps_name']));
			$sheet->setCellValue('I' . $r,  $jobseeker_data['permanent_pin']);
			$sheet->setCellValue('J' . $r,  ucwords($status_show));
			
		}else{
			$sheet->setCellValue('E' . $r,  $status_show);
		}
	
		$r++;
  
	  }
	  if($user_role==7){
		$sheet->getStyle('A' . $r . ':J' . $r . '')->applyFromArray($style);
	  }else{
		$sheet->getStyle('A' . $r . ':E' . $r . '')->applyFromArray($style);
	  }
	  $objPHPExcel->setActiveSheetIndex(0);
	  $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');
	  ob_end_clean();
	  // Redirect output to a client’s web browser (Excel5)
	  header('Content-Type: application/vnd.ms-excel;charset:UTF-8');
	  header('Content-Disposition: attachment;filename=karmasathi'.time().'.xls');
	  header('Cache-Control: max-age=0');
	  // If you're serving to IE 9, then the following may be needed
	  header('Cache-Control: max-age=1');
	  $objWriter->save('php://output');
	  exit;




   
		


	} else {
		drupal_set_message('You are not authorize to access this page', error);
	}

	return $form;
		

		

		return $output;

	

}






