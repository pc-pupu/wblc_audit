<?php 
function cpi_number_for_worker_print_excel(){
	global $user, $base_root, $base_path;
	
	$filename = strtoupper(str_replace(" ", "-",'Consumer Price Index Numbers(Monthly)')).'.xls';
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
	
	$default_border = array(
	 'allborders' => array(
		'style' => PHPExcel_Style_Border::BORDER_THIN,
		'color' => array('rgb'=>'1006A3')
		)
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
			'color' => array('rgb'=>'4BA0F4'),
		),
		'font' => array(
					'bold' => true,
					),
		'alignment' => array(
            'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
        )
	);
	$styleArray = array(
		  'borders' => array(
			  'allborders' => array(
				  'style' => PHPExcel_Style_Border::BORDER_THIN
			  )
		  )
	  );
	
	 $style = array(
        'alignment' => array(
            'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
        ),
		'font' => array(
					'bold' => true,
					),
    );
	
	$starting_pos = ord('A');
	$sheet->mergeCells('A1:Z1');	
	$sheet->setCellValue('A1', 'Consumer Price Index Numbers(Monthly)');
	$sheet->getStyle('A1'.$merge_cell)->applyFromArray($style_header);
	
	$sheet->setCellValue('A2', ' Year ');
	$sheet->setCellValue('C2', ' January ');
	$sheet->setCellValue('E2', ' February ');
	$sheet->setCellValue('G2', ' March ');
	$sheet->setCellValue('I2', ' April ');
	$sheet->setCellValue('K2', ' May ');
	$sheet->setCellValue('M2', ' June ');
	$sheet->setCellValue('O2', ' July ');
	$sheet->setCellValue('Q2', ' August ');
	$sheet->setCellValue('S2', ' September ');
	$sheet->setCellValue('U2', ' October ');
	$sheet->setCellValue('W2', ' November ');
	$sheet->setCellValue('Y2', ' December ');
	
	$sheet->getStyle('A2')->applyFromArray($style);
	$sheet->getStyle('C2')->applyFromArray($style);
	$sheet->getStyle('E2')->applyFromArray($style);
	$sheet->getStyle('G2')->applyFromArray($style);
	$sheet->getStyle('I2')->applyFromArray($style);
	$sheet->getStyle('K2')->applyFromArray($style);
	$sheet->getStyle('M2')->applyFromArray($style);
	$sheet->getStyle('O2')->applyFromArray($style);
	$sheet->getStyle('Q2')->applyFromArray($style);
	$sheet->getStyle('S2')->applyFromArray($style);
	$sheet->getStyle('U2')->applyFromArray($style);
	$sheet->getStyle('W2')->applyFromArray($style);
	$sheet->getStyle('Y2')->applyFromArray($style);
	
	$objPHPExcel->getActiveSheet()->getColumnDimension('A')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('C')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('D')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('E')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('F')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('G')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('D')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('H')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('I')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('J')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('K')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('L')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('M')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('N')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('O')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('P')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('Q')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('R')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('S')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('T')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('U')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('V')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('W')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('X')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('Y')->setWidth("5");
	$objPHPExcel->getActiveSheet()->getColumnDimension('Z')->setWidth("5");
	
	
	$sheet->mergeCells('C2:D2');
	$sheet->mergeCells('E2:F2');
	$sheet->mergeCells('G2:H2');
	$sheet->mergeCells('I2:J2');
	$sheet->mergeCells('K2:L2');
	$sheet->mergeCells('M2:N2');
	$sheet->mergeCells('O2:P2');
	$sheet->mergeCells('Q2:R2');
	$sheet->mergeCells('S2:T2');
	$sheet->mergeCells('U2:V2');
	$sheet->mergeCells('W2:X2');
	$sheet->mergeCells('Y2:Z2');
	
	$sheet->setCellValue('C3', 'IW');
	$sheet->setCellValue('D3', 'AL');
	$sheet->setCellValue('E3', 'IW');
	$sheet->setCellValue('F3', 'AL');
	$sheet->setCellValue('G3', 'IW');
	$sheet->setCellValue('H3', 'AL');
	$sheet->setCellValue('I3', 'IW');
	$sheet->setCellValue('J3', 'AL');
	$sheet->setCellValue('K3', 'IW');
	$sheet->setCellValue('L3', 'AL');
	$sheet->setCellValue('M3', 'IW');
	$sheet->setCellValue('N3', 'AL');
	$sheet->setCellValue('O3', 'IW');
	$sheet->setCellValue('P3', 'AL');
	$sheet->setCellValue('Q3', 'IW');
	$sheet->setCellValue('R3', 'AL');
	$sheet->setCellValue('S3', 'IW');
	$sheet->setCellValue('T3', 'AL');
	$sheet->setCellValue('U3', 'IW');
	$sheet->setCellValue('V3', 'AL');
	$sheet->setCellValue('W3', 'IW');
	$sheet->setCellValue('X3', 'AL');
	$sheet->setCellValue('Y3', 'IW');
	$sheet->setCellValue('Z3', 'AL');
	
	$sheet->getStyle('C3')->applyFromArray($style);
	$sheet->getStyle('D3')->applyFromArray($style);
	$sheet->getStyle('E3')->applyFromArray($style);
	$sheet->getStyle('F3')->applyFromArray($style);
	$sheet->getStyle('G3')->applyFromArray($style);
	$sheet->getStyle('H3')->applyFromArray($style);
	$sheet->getStyle('I3')->applyFromArray($style);
	$sheet->getStyle('J3')->applyFromArray($style);
	$sheet->getStyle('K3')->applyFromArray($style);
	$sheet->getStyle('L3')->applyFromArray($style);
	$sheet->getStyle('M3')->applyFromArray($style);
	$sheet->getStyle('N3')->applyFromArray($style);
	$sheet->getStyle('O3')->applyFromArray($style);
	$sheet->getStyle('P3')->applyFromArray($style);
	$sheet->getStyle('Q3')->applyFromArray($style);
	$sheet->getStyle('R3')->applyFromArray($style);
	$sheet->getStyle('S3')->applyFromArray($style);
	$sheet->getStyle('T3')->applyFromArray($style);
	$sheet->getStyle('U3')->applyFromArray($style);
	$sheet->getStyle('V3')->applyFromArray($style);
	$sheet->getStyle('W3')->applyFromArray($style);
	$sheet->getStyle('X3')->applyFromArray($style);
	$sheet->getStyle('Y3')->applyFromArray($style);
	$sheet->getStyle('Z3')->applyFromArray($style);
	
	
	$centre_desc = 1;
	
	$cpiresult = db_query('select cpi.cpiyear, janiw, febiw, mrciw, apriw, mayiw, juneiw, julyiw, augiw, septiw, octiw, noviw, deciw, janal, febal, mrcal, apral, mayal, juneal, julyal, augal, septal, octal, noval, decal from (select cpiyear, centre_desc from l_mnw_cpi_number_master group by cpiyear, centre_desc) as cpi
left join(select iwvalue as janiw, alvalue as janal, cpiyear from l_mnw_cpi_number_master where month_srl=1 and is_active =1) as j on cpi.cpiyear = j.cpiyear
left join(select iwvalue as febiw, alvalue as febal, cpiyear from l_mnw_cpi_number_master where month_srl=2 and is_active =1) as f on cpi.cpiyear = f.cpiyear
left join(select iwvalue as mrciw, alvalue as mrcal, cpiyear from l_mnw_cpi_number_master where month_srl=3 and is_active =1) as m on cpi.cpiyear = m.cpiyear
left join(select iwvalue as apriw, alvalue as apral, cpiyear from l_mnw_cpi_number_master where month_srl=4 and is_active =1) as a on cpi.cpiyear = a.cpiyear
left join(select iwvalue as mayiw, alvalue as mayal, cpiyear from l_mnw_cpi_number_master where month_srl=5 and is_active =1) as my on cpi.cpiyear = my.cpiyear
left join(select iwvalue as juneiw, alvalue as juneal, cpiyear from l_mnw_cpi_number_master where month_srl=6 and is_active =1) as jn on cpi.cpiyear = jn.cpiyear
left join(select iwvalue as julyiw, alvalue as julyal, cpiyear from l_mnw_cpi_number_master where month_srl=7 and is_active =1) as jl on cpi.cpiyear = jl.cpiyear
left join(select iwvalue as augiw, alvalue as augal, cpiyear from l_mnw_cpi_number_master where month_srl=8 and is_active =1) as ag on cpi.cpiyear = ag.cpiyear
left join(select iwvalue as septiw, alvalue as septal, cpiyear from l_mnw_cpi_number_master where month_srl=9 and is_active =1) as s on cpi.cpiyear = s.cpiyear
left join(select iwvalue as octiw, alvalue as octal, cpiyear from l_mnw_cpi_number_master where month_srl=10 and is_active =1) as o on cpi.cpiyear = o.cpiyear
left join(select iwvalue as noviw, alvalue as noval, cpiyear from l_mnw_cpi_number_master where month_srl=11 and is_active =1) as n on cpi.cpiyear = n.cpiyear
left join(select iwvalue as deciw, alvalue as decal, cpiyear from l_mnw_cpi_number_master where month_srl=12 and is_active =1) as d on cpi.cpiyear = d.cpiyear where cpi.centre_desc = '.$centre_desc.' order by cpi.cpiyear desc');
	$count=4;
	foreach($cpiresult as $data) {
		$sheet->setCellValue('A'.$count, (!empty($data->cpiyear) ? $data->cpiyear : '-'));
		$sheet->setCellValue('C'.$count, (!empty($data->janiw) ? $data->janiw : '-'));
		$sheet->setCellValue('D'.$count, (!empty($data->janal) ? $data->janal : '-'));
		$sheet->setCellValue('E'.$count, (!empty($data->febiw) ? $data->febiw : '-'));
		$sheet->setCellValue('F'.$count, (!empty($data->febal) ? $data->febal : '-'));
		$sheet->setCellValue('G'.$count, (!empty($data->mrciw) ? $data->mrciw : '-'));
		$sheet->setCellValue('H'.$count, (!empty($data->mrcal) ? $data->mrcal : '-'));
		$sheet->setCellValue('I'.$count, (!empty($data->apriw) ? $data->apriw : '-'));
		$sheet->setCellValue('J'.$count, (!empty($data->apral) ? $data->apral : '-'));
		$sheet->setCellValue('K'.$count, (!empty($data->mayiw) ? $data->mayiw : '-'));
		$sheet->setCellValue('L'.$count, (!empty($data->mayal) ? $data->mayal : '-'));
		$sheet->setCellValue('M'.$count, (!empty($data->juneiw) ? $data->juneiw : '-'));
		$sheet->setCellValue('N'.$count, (!empty($data->juneal) ? $data->juneal : '-'));
		$sheet->setCellValue('O'.$count, (!empty($data->julyiw) ? $data->julyiw : '-'));
		$sheet->setCellValue('P'.$count, (!empty($data->julyal) ? $data->julyal : '-'));
		$sheet->setCellValue('Q'.$count, (!empty($data->augiw) ? $data->augiw : '-'));
		$sheet->setCellValue('R'.$count, (!empty($data->augal) ? $data->augal : '-'));
		$sheet->setCellValue('S'.$count, (!empty($data->septiw) ? $data->septiw : '-'));
		$sheet->setCellValue('T'.$count, (!empty($data->septal) ? $data->septal : '-'));
		$sheet->setCellValue('U'.$count, (!empty($data->octiw) ? $data->octiw : '-'));
		$sheet->setCellValue('V'.$count, (!empty($data->octal) ? $data->octal : '-'));
		$sheet->setCellValue('W'.$count, (!empty($data->noviw) ? $data->noviw : '-'));
		$sheet->setCellValue('X'.$count, (!empty($data->noval) ? $data->noval : '-'));
		$sheet->setCellValue('Y'.$count, (!empty($data->deciw) ? $data->deciw : '-'));
		$sheet->setCellValue('Z'.$count, (!empty($data->decal) ? $data->decal : '-'));
		
		
		
		/*$sheet->getStyle('A'.$count)->applyFromArray($style);
		$sheet->getStyle('C'.$count)->applyFromArray($style);
		$sheet->getStyle('D'.$count)->applyFromArray($style);
		$sheet->getStyle('E'.$count)->applyFromArray($style);
		$sheet->getStyle('F'.$count)->applyFromArray($style);
		$sheet->getStyle('G'.$count)->applyFromArray($style);
		$sheet->getStyle('H'.$count)->applyFromArray($style);
		$sheet->getStyle('I'.$count)->applyFromArray($style);
		$sheet->getStyle('J'.$count)->applyFromArray($style);
		$sheet->getStyle('K'.$count)->applyFromArray($style);
		$sheet->getStyle('L'.$count)->applyFromArray($style);
		$sheet->getStyle('M'.$count)->applyFromArray($style);
		$sheet->getStyle('N'.$count)->applyFromArray($style);
		$sheet->getStyle('O'.$count)->applyFromArray($style);
		$sheet->getStyle('P'.$count)->applyFromArray($style);
		$sheet->getStyle('Q'.$count)->applyFromArray($style);
		$sheet->getStyle('R'.$count)->applyFromArray($style);
		$sheet->getStyle('S'.$count)->applyFromArray($style);
		$sheet->getStyle('T'.$count)->applyFromArray($style);
		$sheet->getStyle('U'.$count)->applyFromArray($style);
		$sheet->getStyle('V'.$count)->applyFromArray($style);
		$sheet->getStyle('W'.$count)->applyFromArray($style);
		$sheet->getStyle('X'.$count)->applyFromArray($style);
		$sheet->getStyle('Y'.$count)->applyFromArray($style);
		$sheet->getStyle('Z'.$count)->applyFromArray($style);*/
		$count++;
		}
		$sheet->getStyle("A2:Z".($count-1))->applyFromArray($styleArray);
		$sheet->removeColumn('B');
		$sheet->mergeCells('A2:A3');

	//-------------------
	$objPHPExcel->getActiveSheet()->setTitle("CPI Number");
	$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');	
	ob_end_clean();
	// Redirect output to a client’s web browser (Excel5)
	header('Content-Type: application/vnd.ms-excel;charset:UTF-8');
	header('Content-Disposition: attachment;filename='.basename($filename));
	header('Cache-Control: max-age=0');
	// If you're serving to IE 9, then the following may be needed
	header('Cache-Control: max-age=1');
	// If you're serving to IE over SSL, then the following may be needed
	
	$objWriter->save('php://output');
	exit;

}