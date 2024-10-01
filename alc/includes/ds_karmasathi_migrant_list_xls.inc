<?php
function ds_karmasathi_migrant_list($form, &$form_state, $user_role_en = '', $par = '', $type='',$next=0,$p=1)
{
	global $user;
	$datashow=100;
	//echo $next.'----------'.$p;die;
	$par1=$p;
	$table = $apply_category = $unit_name = $actionStatus = '';
	$user_id = $user->uid;

	$i = $next;
	// http://202.61.117.237/wblc/official-karmasathi-list/dXhyN2pCbmNDUTBld2VDWW1JczZJdz09/pending
	global $base_root, $base_path, $user;
	$fetch_users_role = db_select('users_roles', 'ur');
	$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role->fields('ro', array('rid', 'name'));
	$fetch_users_role->condition('ur.uid', $user->uid);
	$fetch_users_role_data = $fetch_users_role->execute()->fetchAssoc();
    $user_role = encryption_decryption_fun('decrypt',$user_role_en);
	//$user_role = $user_role_en;

	$role_name = $fetch_users_role_data['name'];

	$active = $activeIncomplete = $activePen = '';
	$back_link  = $base_root . $base_path . 'duare-sarkar-user-list';
		

	if ($par == 'approved') {
		$active = 'active';
		$activeIncomplete = '';
		$activeBack = '';
		$activeRej = '';
	} else if ($par == 'provisional') {
		$activeIncomplete = 'active';
		$active = '';
		$activeBack = '';
		$activeRej = '';
	}else if ($par == 'pending') {
		$activePen = 'active';
		$active = '';
		$activeIncomplete = '';
		$activeBack = '';
		$activeRej = '';
	}else if ($par == 'back') {
		$activePen = '';
		$active = '';
		$activeIncomplete = '';
		$activeBack = 'active';
		$activeRej = '';
	}else if ($par == 'reject') {
		$activePen = '';
		$active = '';
		$activeIncomplete = '';
		$activeBack = '';
		$activeRej = 'active';
	}
	$back = l(t('<span class="badge bg-yellow pull-right"><i class="fa fa-file-excel-o"></i> EXCEL</span>'), $back_link, array('html' => TRUE));
	
 
		
 	
	if ((($user_role == 4) || ($user_role == 12) || ($user_role == 7) || ($user_role == 5))) {

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
	
	$query_details = $query->execute()->fetchAll();
	
	if($par == 'approved'){
		$status = 'C';
	}elseif ($par == 'pending') {
		$status = array('A','BA');
	}elseif ($par == 'provisional') {
		$status = array('P','BP');
	}elseif ($par == 'back') {
		$status = 'B';
	}elseif ($par == 'reject') {
		$status = 'R';
	}
	
	$curl_post_data = array (
		'source' 		=> 'WBLC',
		'taskid' 		=> 'DSLCFETCHAPPLICATION',
		'user_role' 	=> $user_role,
		'block_subdiv' 	=> $area_arr,									
		'user_id' 		=> $user_id,
		'iscustom_juris'=> $iscustom_juris,
		'status' 		=> $status,
		'datashow'      => $datashow,
		'next'           => $next,
		//SSSSSS'excel'         =>'yes',
		'type'	       =>$type,							
	);
	if($user_role==7){
	$curl_post_data['excel']='yes';
	}else{
		$curl_post_data['excel']='no';
	}		
    //print_r($curl_post_data);
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
    //print_r($errors); echo 777; die;
	curl_close($ch);
			
	$results = json_decode(trim($results), TRUE);  
	//echo '<pre>'; print_r($results);


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
	$sheet->setCellValue('A1', strtoupper('duare sarkar karmasathi application ('.$par.')'));
    $sheet->setCellValue('A2', 'SL.No.');
	if($type=='DS'){
    $sheet->setCellValue('B2', 'DS ID');
	}
	
    $sheet->setCellValue('C2', 'MWIN');
    $sheet->setCellValue('D2', 'WORKER NAME');
    $sheet->setCellValue('E2', 'MOBILE NUMBER');
	if($user_role==7){
		$sheet->setCellValue('F2', 'ADDRESS');
		$sheet->setCellValue('G2', 'BLOCK');
		$sheet->setCellValue('H2', 'VILL / WARD');
		$sheet->setCellValue('I2', 'POLICE STATION');
		$sheet->setCellValue('J2', 'PIN CODE');
		$sheet->setCellValue('K2', 'STATUS');
	}else{
		$sheet->setCellValue('F2', 'STATUS');
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
		$objPHPExcel->getActiveSheet()->getColumnDimension('K')->setWidth(30);

	}else{
		$objPHPExcel->getActiveSheet()->getRowDimension('1')->setRowHeight(25);
		$objPHPExcel->getActiveSheet()->getRowDimension('2')->setRowHeight(25);
		$objPHPExcel->getActiveSheet()->getColumnDimension('A')->setWidth(10);
		$objPHPExcel->getActiveSheet()->getColumnDimension('B')->setWidth(25);
		$objPHPExcel->getActiveSheet()->getColumnDimension('C')->setWidth(10);
		$objPHPExcel->getActiveSheet()->getColumnDimension('D')->setWidth(15);
		$objPHPExcel->getActiveSheet()->getColumnDimension('E')->setWidth(30);
		$objPHPExcel->getActiveSheet()->getColumnDimension('F')->setWidth(40);
	}
    
	

	$objPHPExcel->getActiveSheet()->getStyle('B')->getNumberFormat()->setFormatCode('0');

    $sheet->getStyle('A1')->applyFromArray($style_header);
    $sheet->getStyle('B1')->applyFromArray($style_header);
    $sheet->getStyle('C1')->applyFromArray($style_header);
    $sheet->getStyle('D1')->applyFromArray($style_header);
    $sheet->getStyle('E1')->applyFromArray($style_header);
    
	if($user_role==7){
		$sheet->getStyle('F1')->applyFromArray($style_header);
		$sheet->getStyle('G1')->applyFromArray($style_header);
		$sheet->getStyle('H1')->applyFromArray($style_header);
		$sheet->getStyle('I1')->applyFromArray($style_header);
		$sheet->getStyle('J1')->applyFromArray($style_header);
		$sheet->getStyle('K1')->applyFromArray($style_header);
	}else{
		$sheet->getStyle('F1')->applyFromArray($style_header);
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
    $sheet->getStyle('J')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_LEFT);
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
		// echo"<pre>";
		// print_r($jobseeker_data['permanent_pin']);die;
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
		if($type=='DS'){
		$sheet->setCellValue('B' . $r, $jobseeker_data['duaresarkar_id'] ,PHPExcel_Cell_DataType::TYPE_STRING);
		}
		$sheet->setCellValue('C' . $r, $identification_number);
		$sheet->setCellValue('D' . $r, $name);
		$sheet->setCellValue('E' . $r, $mobile);
		
		if($user_role==7){
			$sheet->setCellValue('F' . $r,  $jobseeker_data['permanent_address']);
			$sheet->setCellValue('G' . $r,  ucwords($jobseeker_data['block_mun_name']));
			$sheet->setCellValue('H' . $r,  ucwords($jobseeker_data['village_ward_name']));
			$sheet->setCellValue('I' . $r,  ucwords($jobseeker_data['ps_name']));
			$sheet->setCellValue('J' . $r,  ucwords($jobseeker_data['permanent_pin']));
			$sheet->setCellValue('K' . $r,  $status_show);
		}else{
			$sheet->setCellValue('F' . $r,  $status_show);
		}
	
		$r++;
  
	  }
	  if($user_role==7){
		$sheet->getStyle('A' . $r . ':K' . $r . '')->applyFromArray($style);
	  }else{
		$sheet->getStyle('A' . $r . ':F' . $r . '')->applyFromArray($style);
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

}