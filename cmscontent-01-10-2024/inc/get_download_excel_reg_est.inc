<?php

function get_download_excel_reg_est(){

// Excel class file include
	$file = drupal_get_path('module', 'genfunction') . "/PHPExcel/Classes/PHPExcel.php";
	$file2 = drupal_get_path('module', 'genfunction') . "/PHPExcel/Classes/PHPExcel/IOFactory.php";
	require_once $file;
	require_once $file2;
	$objPHPExcel = new PHPExcel();
	$objPHPExcel->getProperties()->setCreator("Maarten Balliauw")
			->setLastModifiedBy("Maarten Balliauw")
			->setTitle("Office 2007 XLSX Test Document")
			->setSubject("Office 2007 XLSX Test Document")
			->setDescription("Test document for Office 2007 XLSX, generated using PHP classes.")
			->setKeywords("office 2007 openxml php")
			->setCategory("Test result file");
	$sheet = $objPHPExcel->getActiveSheet();
	// Header Section for excel	
	$sheet->setCellValue('A1','SL No');
	$sheet->setCellValue('B1','ID');
	$sheet->setCellValue('C1','Unit Id');
	$sheet->setCellValue('D1','Establishment Name');
	$sheet->setCellValue('E1','Address of Establishment');
	$sheet->setCellValue('F1','District');
	$sheet->setCellValue('G1','Subdivision');
	$sheet->setCellValue('H1','Block');
	$sheet->setCellValue('I1','Village');
	$sheet->setCellValue('J1','Polica Station');
	$sheet->setCellValue('K1','Pincode');


$query2 = db_query('SELECT em.id, em.unit_id, em.e_name, em.e_address_line, dm.district_name, sd.sub_div_name, bmm.block_mun_name, bmm.type, vw.village_name, ps.name_of_police_station, em.e_pin_code FROM l_establishment_unit_master AS em
LEFT JOIN district_master AS dm on dm.district_code = em.e_dist_code
LEFT JOIN sub_division AS sd on sd.sub_div_code = em.e_sub_div_code
LEFT JOIN block_mun_master AS bmm on bmm.block_code = em.e_block_mun_code
LEFT JOIN village_ward_master AS vw on vw.village_code = em.e_vill_ward_code
LEFT JOIN police_station AS ps on ps.police_station_code = em.e_ps_code
WHERE EM.applicant_user_id IS NOT NULL ORDER BY em.e_name');
    $query_result = db_query($query2)->fetchAll();
	$counter =0;
	$r = 2;
	foreach($query_result as  $key => $value) {
		if($value->type == 'M'){$gpward_name = 'Ward-'.$value->village_name;}else{$gpward_name = $value->village_name;}
		$counter++;			
		$sheet->setCellValue('A'.$r, $counter);
		$sheet->setCellValue('B'.$r,$value->id);
		$sheet->setCellValue('C'.$r,$value->unit_id);
		$sheet->setCellValue('D'.$r,$value->e_name);
		$sheet->setCellValue('E'.$r,$value->e_address_line);
		$sheet->setCellValue('F'.$r,$value->district_name);
		$sheet->setCellValue('G'.$r,$value->sub_div_name);
		$sheet->setCellValue('H'.$r,$value->block_mun_name);
		$sheet->setCellValue('I'.$r,$gpward_name);
		$sheet->setCellValue('J'.$r,$value->name_of_police_station);
		$sheet->setCellValue('K'.$r,$value->e_pin_code);
		$r++;
	}
	
	$objPHPExcel->setActiveSheetIndex(0);
	$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');	
	ob_end_clean();
	// Redirect output to a client’s web browser (Excel5)
	header('Content-Type: application/vnd.ms-excel;charset:UTF-8');

	$filename = 'list-of-establishment-'.date("d-m-Y").'.xls';
	header('Content-Disposition: attachment;filename='.basename(strtolower($filename)));
	
	header('Cache-Control: max-age=0');
	// If you're serving to IE 9, then the following may be needed
	header('Cache-Control: max-age=1');	
	$objWriter->save('php://output');
	exit;
}