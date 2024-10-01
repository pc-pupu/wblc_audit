<?php
drupal_add_library('system','ui.datepicker');
drupal_add_js('jQuery(document).ready(function(){
	jQuery("#start_date").datepicker({
	dateFormat: "dd-mm-yy",
	minDate: "01-01-2015",
	maxDate: 0,
	changeMonth: true,
	changeYear: true,
	});
});', 'inline');
drupal_add_js('jQuery(document).ready(function(){
	jQuery("#end_date, #to_date_of_inspection").datepicker({
	dateFormat: "dd-mm-yy",
	minDate: "01-02-2015",
	maxDate: 0,
	changeMonth: true,
	changeYear: true,
	// yearRange: "-60:+10",
	});
});', 'inline');
function trade_union_report_of_clra($form, $form_state, $id=''){
	global $user;
	
	
	
	$form['clra_report']['markup_start'] = array(
						'#type' => 'markup',
						'#markup'=> '<div class="row">
										<div class="col-md-12">
											<div class="box box-primary">
												<div class="box-body">'
						);
						
	$options_dlc = common_custom_user_district_list_new();			 
				 
	$form['clra_report']['district_id'] = array(
									'#prefix' 	=> 	'<div class="form-custom col-md-3 col-sm-3 col-xs-3 form-full-label col-half-offset"><label class="input">',
									'#suffix' 	=> 	'</label></div>',
									'#title'			=>	'Select district',
									'#type' 			=> 	'select',
									'#options' 			=>	$options_dlc,
									'#attributes' => array('class' => array('form-control')),
									'#default_value'	=>	!empty($district_code) ? $district_code : '',
									'#required'			=>	TRUE,
									'#ajax'				=> 	array(
																'event' 			=> 'change',
																'effect' 			=> 'fade',			
																'callback'			=> 'get_clra_tu_subdivision_ajax_callback',
																'progress'			=> '',
															 )											
									
								);
								
	$value_dist = isset($form_state['values']['district_id']) ? $form_state['values']['district_id'] : $district_code;
	
	if($value_dist != "" || !empty($value_dist) || is_numeric($value_dist)){
		$form['clra_report']['subdivision_code'] = array(
							'#prefix' 	=> 	'<div class="form-custom col-md-3 col-sm-3 col-xs-3 form-full-label col-half-offset" id="subdvsn_code_replace"><label class="input">',
							'#suffix' => '</label></div>',																	
							'#title' => 'Select subdivision',
							'#type'  => 'select',
							'#attributes' => array('class' => array('form-control')),
							'#default_value' => !empty($subdivision_code) ? $subdivision_code : '',
							'#options' => common_custom_user_subdivisionlist_list_new($value_dist)
						);
	}else{
		$form['clra_report']['subdivision_code'] = array(
							'#prefix' 	=> 	'<div class="form-custom col-md-3 col-sm-3 col-xs-3 form-full-label col-half-offset" id="subdvsn_code_replace"><label class="input">',
							'#suffix' 	=> 	'</label></div>',																	
							'#title'				=> 'Select subdivision',
							'#type' 				=> 'select',
							'#attributes' => array('class' => array('form-control')),
							'#options' 				=> array( '' => '--Select--' )
						);
	}

	
	$form['clra_report']['date_of_start'] = array(
						'#prefix'=> '<div class="col-md-2 #col-md-offset-right-8">',
						'#suffix'=> '</div>',
						'#type'=> 'textfield',
						'#title'=> t('Start Date'),
						'#required'=> TRUE,
						'#attributes'=> array('id' => 'start_date','class' => array('form-control'),'readonly' => 'readonly'),
						);
						
	$form['clra_report']['date_of_end'] = array(
						'#prefix'=> '<div class="col-md-2 #col-md-offset-right-8">',
						'#suffix'=> '</div>',
						'#type'=> 'textfield',
						'#title'=> t('End Date'),
						'#required'=> TRUE,
						'#attributes'=> array('id' => 'end_date','class' => array('form-control'),'readonly' => 'readonly'),
						);
		
	
	$form['clra_report']['search'] = array(
						'#prefix' => '<div class="col-md-2">',
						'#suffix' => '</div></div></div></div>',
						'#type' => 'button',
						'#value' => 'GENERATE',
						'#attributes' => array('class' => array('btn btn-primary pull-left')),
						'#ajax' => array(
								'callback' => 'clra_trade_union_report_callback',
								'wrapper'  => 'clra_trade_union_report_callback_div',
								'method'   => 'html',
								'effect'   => 'fade'
								)
						);
						
	$form['clra_report']['markup_start2'] = array(
						'#type' => 'markup',
						'#markup'=> '<div class="col-md-12">
										<div class="box box-primary">
											<div class="box-body">'
						);
						
	$form['clra_report']['download'] = array(
						'#prefix'=> '<div class="col-md-12" id="clra_trade_union_report_callback_div">',
						'#suffix'=> '</div>'
						);						
	
	$form['clra_report']['footer'] = array(
						'#markup' => '</div></div></div></div>',
						'#type' => 'markup'
						);
																																											
	return $form;						
}

function clra_trade_union_report_callback($form, $form_states){
	$district_id = $form_states['values']['district_id'];
	$subdivision_code = $form_states['values']['subdivision_code'];
	$fromdate = $form_states['values']['date_of_start'];
	$todate = $form_states['values']['date_of_end'];
	
	
	if($subdivision_code == ''){$subdivision_code = 0;}
	
	$output = '<div class="col-md-4">'.l('<span class="btn btn-warning"> DOWNLOAD EXCEL <i class="fa fa-download"></i></span>', 'clra/clra-tu-report-excel/'.$district_id.'/'.$subdivision_code.'/'.strtotime($fromdate).'/'.strtotime($todate), array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</div>';  	
	return $output;
}



function get_clra_tu_subdivision_ajax_callback($form, $form_state){
	 $commands 	= array();
	 $commands[] = ajax_command_replace('#subdvsn_code_replace', drupal_render($form['clra_report']['subdivision_code']));
	 return array('#type' => 'ajax', '#commands' => $commands);
}

/*function clr_tu_report_xls($district, $subdivision, $fdt, $tdt){
	//echo $district.'<>'.$subdivision.'<>'.$fdt.'<>'.$tdt; die;
	global $user;
	$clra_report	= db_query("select clra.registration_number as Registration_number,clra.registration_date, clra.e_name as establishment_name,clra.max_num_wrkmen,
clra.e_num_of_workmen_per_or_reg as workmen_permanent_or_regular,clra.e_num_of_workmen_temp_or_reg as temporary_or_regular,clra.e_any_day_max_num_of_workmen,
t.e_trade_union_regn_no, t.e_trade_union_name
	from l_clra_reg_trade_union_master t
	join l_clra_registration_master clra on clra.user_id = t.user_id and clra.id = t.application_id
		where amendment_parent_id is null and status='I' and loc_e_subdivision=$subdivision
		order by clra.id")->fetchAssoc();	
	$clra_report['establishment_name'];

	
	if($clra_report->rowCount()>0){
		$sl_no = 1;		
		foreach ($clra_report as $data) {
			
			$rows = '<tr>
							<td>'.$sl_no++.'</td>
							<td>'.$data->registration_number.'</td>
							<td align="center">'.date('d/m/y', strtotime($data->registration_date)).'</td>
							<td>'.$data->establishment_name.'</td>
							<td>'.$data->max_num_wrkmen.'</td>
							<td>'.$data->workmen_permanent_or_regular.'</td>
							<td>'.$data->e_any_day_max_num_of_workmen.'</td>
							<td>'.$data->e_trade_union_regn_no.'</td>
							<td>'.$data->e_trade_union_name.'</td>
						</tr>'; 
		
		}
	}else{
		$rows = '<tr><td colspan="5">Data not found</td></tr>';
	}
	$dist_name=districtname($district);
	$subdivision_name=subdistrictname($subdivision);
	$report_heading = 'Clra Trade Union Report of District:'.$dist_name.'and Subdivision'.$subdivision_name;
	
	
	$output = '<style type="text/css">
				table, td, th {font-size:12px;font-family:Times New Roman;height:35px;color:#000;margin-top:20px;background:#fff;border:1px solid #cccccc;}
				td, th { color: black;padding-left:5px;}		
				</style>
				<div class="box box-primary">
				   <div class="box-body">					    				
					  <table width="100%" border="0" cellspacing="0" cellpadding="0">
						  <tr><td colspan="10" align="center" style="font-size:16px;"><strong>COVID-19 Monitoring Report In Factories</strong></td>
						  '.$report_heading.'							  
						  <tr>
							<td width="5%" style=""><strong>S.N.</strong></td>
							<td width="8%"><strong>Registration Number</strong></td>
							<td width="8%"><strong>Registration Date</strong></td>
							<td width="8%"><strong>Establishment Name</strong></td>
							<td width="13%"><strong>Maximum Number Wrkmen</strong></td>
							<td width="15%"><strong>workmen_permanent_or_regular</strong></td>
							<td width="15%"><strong>Remarks on Worker&prime;s welfare/ wages</strong></td>
							<td width="9%"><strong>Whether COVID-19 protocol is maintained </strong></td>
							<td width="10%"><strong>Whether provision of worker′s welfare/ wages are maintained</strong></td>
							<td width="9%"><strong>Status</strong></td>								
						</tr>							
						'.$rows.'							
					  </table>
				   </div>
			   </div>';

	
	return pdf_using_mpdf_api($output, 'INS-'.time(), 'A4-L');
	// return pdf_using_mpdf_api($output, 'INS-'.time());
}*/

function districtname($dlc_id) {
	
	global $base_root, $base_path, $user;
	$query_dis_nm=db_query(' select   district_name   from district_master where  district_code = :discd  ', array(':discd' => trim($dlc_id) ));	
		
	$result_dis=$query_dis_nm->fetchAssoc();
	$district_name = $result_dis['district_name'];
		
	return ($district_name );
}

function subdistrictname($dlc_id) {
	
	global $base_root, $base_path, $user;
	$query_dis_nm=db_query(' select sub_div_name from sub_division where  sub_div_code = :sdiscd  ', array(':sdiscd' => trim($dlc_id) ));	
		
	$result_dis=$query_dis_nm->fetchAssoc();
	$sub_div_name = $result_dis['sub_div_name'];
		
	return ($sub_div_name );
}

function clr_tu_report_xls($district, $subdivision, $fdt, $tdt){
	global $user;
	$toda=date('Y-m-d',$tdt);
	$toda=$toda.' '.'23:59:59';
	$fodt=date('Y-m-d',$fdt);
	$fodt=$fodt.' '.'00:00:00';
	if($subdivision!=0){
		$clra_report	= db_query("select clra.registration_number as Registration_number,clra.registration_date, clra.e_name as establishment_name,clra.max_num_wrkmen,
	clra.e_num_of_workmen_per_or_reg as workmen_permanent_or_regular,clra.e_num_of_workmen_temp_or_reg as temporary_or_regular,clra.e_any_day_max_num_of_workmen,
	t.e_trade_union_regn_no, t.e_trade_union_name,clra.full_name_principal_emp,clra.full_name_manager,ps.name_of_police_station,clra.loc_e_dist,clra.loc_e_subdivision,clra.name_areatype,clra.loc_e_areatype,clra.loc_e_vill_ward,clra.loc_e_borough,clra.l_e_ps,clra.loc_e_name,lc.mobile,lc.email,clra.id,clra.loc_e_pin_number,clra.address_principal_emp,clra.address_manager
	from l_clra_reg_trade_union_master t
	right join l_clra_registration_master clra on clra.user_id = t.user_id and clra.id = t.application_id
	right join police_station ps on clra.l_e_ps = ps.police_station_code
	right join l_common_application_master lc on clra.user_id = lc.user_id
		where amendment_parent_id is null and clra.status='I' and loc_e_dist='".$district."' and loc_e_subdivision=".$subdivision." and registration_date BETWEEN  '".$fodt."' AND '".$toda."'
		order by clra.id");
	}else{
		
				$clra_report	= db_query("select clra.registration_number as Registration_number,clra.registration_date, clra.e_name as establishment_name,clra.max_num_wrkmen,
	clra.e_num_of_workmen_per_or_reg as workmen_permanent_or_regular,clra.e_num_of_workmen_temp_or_reg as temporary_or_regular,clra.e_any_day_max_num_of_workmen,
	t.e_trade_union_regn_no, t.e_trade_union_name,clra.full_name_principal_emp,clra.full_name_manager,ps.name_of_police_station,clra.loc_e_dist,clra.loc_e_subdivision,clra.name_areatype,clra.loc_e_areatype,clra.loc_e_vill_ward,clra.loc_e_borough,clra.l_e_ps,clra.loc_e_name,lc.mobile,lc.email,clra.id,clra.loc_e_pin_number,clra.address_principal_emp,clra.address_manager
	from l_clra_reg_trade_union_master t
	right join l_clra_registration_master clra on clra.user_id = t.user_id and clra.id = t.application_id
	right join police_station ps on clra.l_e_ps = ps.police_station_code
	right join l_common_application_master lc on clra.user_id = lc.user_id
		where amendment_parent_id is null and clra.status='I' and loc_e_dist='".$district."' and registration_date BETWEEN   '".$fodt."' AND '".$toda."'
		order by clra.id");

	}
	 $table_value=db_query($clra_report);
	
	$dist_name=districtname($district);
	$subdivision_name=subdistrictname($subdivision);
	if($subdivision!=0){
		$report_heading = 'Clra Trade Union Report of District: '.$dist_name.' and Subdivision '.$subdivision_name.' From '.date('d/m/Y',$fdt).' To '.date('d/m/Y',$tdt);
	}else{
		$report_heading = 'Clra Trade Union Report of District: '.$dist_name.' From '.date('d/m/Y',$fdt).' To '.date('d/m/Y',$tdt);
	}
	
	$report_heading = '';
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
	$default_border = array(
		'style' => PHPExcel_Style_Border::BORDER_THIN,
		'color' => array('rgb'=>'1006A3')
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
			'color' => array('rgb'=>'1006A3'),
		),
		'font' => array(
						'bold' => true,
		),
		'alignment' => array(
            'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
			'bold' => true,
			'style' => PHPExcel_Style_Border::BORDER_NONE,
			'color' => array('rgb'=>'FFFFFF')
        ),			
	);
	$style = array(
        'alignment' => array(
            'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
			'bold' => true,
			'style' => PHPExcel_Style_Border::BORDER_MEDIUM,
			'color' => array('rgb'=>'1006A3')
        ),
		'borders' => array(
			'bottom' => $default_border,
			'left' => $default_border,
			'top' => $default_border,
			'right' => $default_border,
		),
    );
	
	$starting_pos = ord('A');
	$sheet->mergeCells('A1:R1');
	$sheet->setCellValue('A1', $report_heading);
	$sheet->getStyle("A1:R1")->applyFromArray($style_header);
	$sheet->getStyle("A2:R2")->applyFromArray($style);
	$starting_pos = ord('A');

	$starting_pos = ord('A');	
	$sheet->setCellValue('A2', $areaname);
	$sheet->getStyle("A2:R2")->applyFromArray( array(
		'alignment' => array(
            'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
			'vertical'   => PHPExcel_Style_Alignment::VERTICAL_CENTER,
			'bold' => true,
			'style' => PHPExcel_Style_Border::BORDER_MEDIUM,
			'color' => array('rgb'=>'1006A3'),
			'rotation' => 0,
            'wrap' => true
        ),
		'borders' => array(
			'bottom' => $default_border,
			'left' => $default_border,
			'top' => $default_border,
			'right' => $default_border,
		),
		'font' => array(
						'bold' => true,
		),
	)
	);
	
	$sheet->getColumnDimension('A')->setAutoSize(false);
	$sheet->getColumnDimension('B')->setAutoSize(false);
	$sheet->getColumnDimension('C')->setAutoSize(false);
	$sheet->getColumnDimension('D')->setAutoSize(false);
	$sheet->getColumnDimension('E')->setAutoSize(false);	
	$sheet->getColumnDimension('F')->setAutoSize(false);	
	$sheet->getColumnDimension('G')->setAutoSize(false);	
	$sheet->getColumnDimension('H')->setAutoSize(false);
	$sheet->getColumnDimension('I')->setAutoSize(false);
	$sheet->getColumnDimension('J')->setAutoSize(false); 
	$sheet->getColumnDimension('K')->setAutoSize(false); 
	$sheet->getColumnDimension('L')->setAutoSize(false);
	$sheet->getColumnDimension('M')->setAutoSize(false); 
	$sheet->getColumnDimension('N')->setAutoSize(false);
	$sheet->getColumnDimension('O')->setAutoSize(false); 
	$sheet->getColumnDimension('P')->setAutoSize(false);
	$sheet->getColumnDimension('Q')->setAutoSize(false); 
	$sheet->getColumnDimension('R')->setAutoSize(false); 
	
	$sheet->getColumnDimension('A')->setWidth(10);
	$sheet->getColumnDimension('B')->setWidth(18);
	$sheet->getColumnDimension('C')->setWidth(18);
	$sheet->getColumnDimension('D')->setWidth(35);
	$sheet->getColumnDimension('E')->setWidth(35);
	$sheet->getColumnDimension('F')->setWidth(35);
	$sheet->getColumnDimension('G')->setWidth(35);
	$sheet->getColumnDimension('H')->setWidth(10);
	$sheet->getColumnDimension('I')->setWidth(15);
	$sheet->getColumnDimension('H')->setWidth(15);
	$sheet->getColumnDimension('J')->setWidth(12);
	$sheet->getColumnDimension('K')->setWidth(30);
	$sheet->getColumnDimension('L')->setWidth(16);
	$sheet->getColumnDimension('M')->setWidth(40);
	$sheet->getColumnDimension('N')->setWidth(40);
	$sheet->getColumnDimension('O')->setWidth(40);
	$sheet->getColumnDimension('P')->setWidth(40);
	$sheet->getColumnDimension('Q')->setWidth(40);
	$sheet->getColumnDimension('R')->setWidth(40);
	
	// }
	
	// Header Section for excel	
	$sheet->setCellValue('A2','SL No');
	$sheet->setCellValue('B2','REG. NUMBER');
	$sheet->setCellValue('C2','REG. DATE');
	$sheet->setCellValue('D2','NAME OF THE ESTABLISHMENT');
	$sheet->setCellValue('E2','ADDRESS OF THE ESTABLISHMENT');
	$sheet->setCellValue('F2','POLICE STATION OF THE EST.');
	$sheet->setCellValue('G2','NAME OF THE PRINCIPAL EMPLOYER');
	$sheet->setCellValue('H2','ADDRESS OF THE PRINCIPAL EMPLOYER');
	$sheet->setCellValue('I2','NAME OF THE MANAGER');
	$sheet->setCellValue('J2','ADDRESS OF THE MANAGER');
	$sheet->setCellValue('K2','CONTACT NUMBER');
	$sheet->setCellValue('L2','CONTACT EMAIL');
	$sheet->setCellValue('M2','WORKMEN EMPLOYED DIRECTLY ON ANY DAY');
	$sheet->setCellValue('N2','WORKMEN ENGAGED AS PERMANENT/REGULAR');
	$sheet->setCellValue('O2','WORKMEN ENGAGED AS TEMPORARY/REGULAR');
	$sheet->setCellValue('P2','MAXIMUM NUMBER OF CONTRACT LABOUR TO BE EMPLOYED ON ANY DAY THROUGH EACH CONTRACTOR');
	$sheet->setCellValue('Q2','TRADE UNION REGISTRATION NUMBER ');
	$sheet->setCellValue('R2', 'TRADE UNION NAME');
	
	$sheet->getStyle("R")->applyFromArray(array(
        'alignment' => array(
            'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
			'vertical'   => PHPExcel_Style_Alignment::VERTICAL_CENTER,
			'bold' => true,
			'style' => PHPExcel_Style_Border::BORDER_MEDIUM,
			'color' => array('rgb'=>'1006A3'),
        )));
		
	$sheet->getStyle("A2:R2")->applyFromArray(array(
        'alignment' => array(
            'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
			'vertical'   => PHPExcel_Style_Alignment::VERTICAL_CENTER,
			'bold' => true,
			'style' => PHPExcel_Style_Border::BORDER_MEDIUM,
			'color' => array('rgb'=>'1006A3')
        ),
		'borders' => array(
			'bottom' => $default_border,
			'left' => $default_border,
			'top' => $default_border,
			'right' => $default_border,
		),
		'fill' => array(
			// 'type' => PHPExcel_Style_Fill::FILL_SOLID,
			'color' => array('rgb'=>'CCCCCC'),
		),
		'font' => array(
			'bold' => true,
		),
    ));

		
	
	$counter = 0;
	$r = 3;		
	foreach ($table_value as $data) {
		$application_id=$data->id;
		$address=$data->loc_e_name.'<br/>'.get_full_address('l_clra_registration_master', 'sub-table', $application_id, array('loc_e_dist','loc_e_subdivision','loc_e_areatype','name_areatype','loc_e_vill_ward','l_e_ps','loc_e_pin_number')); 
		
		$pe_address=$data->address_principal_emp.'<br/>'.get_full_address('l_clra_registration_master', 'sub-table', $application_id, array('loc_emp_dist','loc_emp_subdivision','loc_emp_areatype','emp_name_areatype','loc_emp_vill_ward','l_emp_ps','loc_emp_pin_number'));
		
		$manager_address=$data->address_manager.'<br/>'.get_full_address('l_clra_registration_master', 'sub-table', $application_id, array('loc_manager_dist','loc_manager_subdivision','loc_manager_areatype','manager_name_areatype','loc_manager_vill_ward','l_manager_ps','loc_manager_pin_number')); 
		
		$counter++;			
		$sheet->setCellValue('A'.$r, $counter);
		$sheet->setCellValue('B'.$r, $data->registration_number);
		$sheet->setCellValue('C'.$r, date('d/m/y', strtotime($data->registration_date)));
		$sheet->setCellValue('D'.$r, $data->establishment_name);
		$sheet->setCellValue('E'.$r, strip_tags($address));
		$sheet->setCellValue('F'.$r, $data->name_of_police_station);
		$sheet->setCellValue('G'.$r, $data->full_name_principal_emp);
		$sheet->setCellValue('H'.$r, strip_tags($pe_address));
		$sheet->setCellValue('I'.$r, $data->full_name_manager);
		$sheet->setCellValue('J'.$r, strip_tags($manager_address));
		$sheet->setCellValue('K'.$r, $data->mobile);
		$sheet->setCellValue('L'.$r, $data->email);
		$sheet->setCellValue('M'.$r, $data->max_num_wrkmen);
		$sheet->setCellValue('N'.$r, $data->workmen_permanent_or_regular);
		$sheet->setCellValue('O'.$r, $data->temporary_or_regular);
		$sheet->setCellValue('P'.$r, $data->e_any_day_max_num_of_workmen);
		$sheet->setCellValue('Q'.$r, $data->e_trade_union_regn_no);
		$sheet->setCellValue('R'.$r, $data->e_trade_union_name);
		$cr = 'A'.$r;
		$cl = 'A'.$r;
		
		$sheet->getStyle("".$cr.":".$cl."")->applyFromArray($style);
		$r++;
	} 
	
	$objPHPExcel->setActiveSheetIndex(0);
	$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');	
	ob_end_clean();
	// Redirect output to a client’s web browser (Excel5)
	//header('Content-Type: application/vnd.ms-excel;charset:UTF-8');
	$filename = 'Clra-Trade-Union'.date("d-m-Y").'.xls';
	header('Content-Disposition: attachment;filename='.basename(strtolower($filename)));
	

	header('Cache-Control: max-age=0');
	// If you're serving to IE 9, then the following may be needed
	header('Cache-Control: max-age=1');	
	$objWriter->save('php://output');
}

