<?php
function get_clra_contractor_info_form_print($id=''){
		$application_id	= encryption_decryption_fun('decrypt', $id);
		$user_id = $user->uid;
		$output = '<style>			
				.clra_report tr td{border: 1px solid #d1d1d1;padding: 6px; border-top:none;border-right:none; font-size:12px;}			
				.clra_report tr th{background: #dddddd none repeat scroll 0 0;border: 1px solid #d1d1d1; padding: 5px; font-size:12px;}
				.note_text{padding-top: 6px; font-size:8px;}
			</style>'
			;
		$output .=	GET_INSERTED_CONTRACTOR_LIST_TABLE_RESULT_Print($application_id); 
		
		return pdf_using_mpdf_api($output, 'all-registered-applicant-user-in-west-bengal-'.date("d-m-Y-h-m-s"));

		}
	
function GET_INSERTED_CONTRACTOR_LIST_TABLE_RESULT_Print($application_id){
	global $user;
	
	ctools_include('modal');
	ctools_modal_add_js();
	$registration_number = '';
	$e_name	= '';
	
	$dataClraMaster = db_query("select lcrm.id, lcrm.final_submit_status, lcrm.identification_number, lcrm.status, lcrm.amendment_parent_id , lcrm.e_name from l_clra_registration_master lcrm where lcrm.user_id=:user_id and lcrm.id=:applicationid", array(':user_id' => $user->uid, ':applicationid' =>$application_id))->fetchAssoc();
	
	if( !empty($dataClraMaster)){
		$identification_number = $dataClraMaster['identification_number'];
		$final_submit_status = $dataClraMaster['final_submit_status'];
		$status	= $dataClraMaster['status'];
		$amendment_parent_id = $dataClraMaster['amendment_parent_id'];
			if(!empty($dataClraMaster['e_name'])){
				$e_name	= $dataClraMaster['e_name'];
			}
			if(!empty($dataClraMaster['registration_number'])){
				$registration_number = '['.$dataClraMaster['registration_number'].']';
			}
	}
	
	$uid = $user->uid;
	$output = '';
	$application_status	= '';
	$service_name = '';
	$license_file_url = '';
			
	if($amendment_parent_id!='' || $amendment_parent_id!=0){
		
		$license_number = '';
		
		$output = '<style>			
				.clra_report tr td{border: 1px solid #d1d1d1;padding: 6px; border-top:none;border-right:none; font-size:12px;}			
				.clra_report tr th{background: #dddddd none repeat scroll 0 0;border: 1px solid #d1d1d1; padding: 5px; font-size:12px;}
				.note_text{padding-top: 6px; font-size:8px;}
			</style>
			<table width="100%" border="0" cellspacing="0" cellpadding="0" class="clra_report">
				<thead>
					 <tr><td colspan="5" align="center" style="font-size:15px; border:0px;"><strong>LABOUR COMMISSIONERATE</strong></td></tr>
					 <tr><td align="center" colspan="5" align="center" style="font-size:12px; border:0px;"><strong>GOVERNMENT OF WEST BENGAL</strong></td></tr>
					 <tr><td align="center" colspan="5" align="center" style="font-size:12px; border:0px;"><i>List of contractorss('.$e_name.$registration_number.')</i></td></tr>
					 <tr>
						<th align="left"><strong>SL.No.</strong></th>
						<th width="30%" align="left"><strong>Contractor Name<br/>License Number</strong></th>
						<th align="left"><strong>Nature of Work</strong></th>
						<th align="left"><strong>Estimated Date of Employment of Each Contract Work Under Each Contractor</strong></th>
						<th align="left"><strong>No. of Contract Labour</strong></th>
					</tr>
				</thead>
				<tbody>';						
	}else{ 
	
		$output .= '<style>			
				.clra_report tr td{border: 1px solid #d1d1d1;padding: 6px; border-top:none;border-right:none; font-size:12px;}			
				.clra_report tr th{background: #dddddd none repeat scroll 0 0;border: 1px solid #d1d1d1; padding: 5px; font-size:12px;}
				.note_text{padding-top: 6px; font-size:8px;}
			</style>
			<table width="100%" border="0" cellspacing="0" cellpadding="0" class="clra_report">
				<thead>
					 <tr><td colspan="5" align="center" style="font-size:15px; border:0px;"><strong>LABOUR COMMISSIONERATE</strong></td></tr>
					 <tr><td align="center" colspan="5" align="center" style="font-size:12px; border:0px;"><strong>GOVERNMENT OF WEST BENGAL</strong></td></tr>
					 <tr><td align="center" colspan="5" align="center" style="font-size:12px; border:0px;"><i>List of contractors ('.$e_name.$registration_number.')</i></td></tr>
					 <tr>
						<th align="left"><strong>SL.No.</strong></th>
						<th width="30%" align="left"><strong>Contractor Name</strong></th>
						<th align="left"><strong>Nature of Work</strong></th>
						<th align="left"><strong>Estimated Date of Employment of Each Contract Work Under Each Contractor</strong></th>
						<th align="left"> <strong>No. of Contract Labour</strong></th>
					</tr>
				</thead>
				<tbody>';						
	}

	/** Contractor Details ***/
	
	$counter = 0;
			
	$query = db_select('l_contractor_info_master', 'lcim');
	$query->fields('lcim', array());
	$query->condition('lcim.application_id', $application_id);
	$query->condition('lcim.user_id', $user->uid);
	$query->orderBy('lcim.id','DESC');
	$contractors_list = $query->execute()->fetchAll();
	
	if(!empty($contractors_list)){
	$Download_From_V = '';
		$reasonStatus = '';
		foreach($contractors_list as  $data){ 
			$counter++;
			$license_file_url = '';
			$license_res = '';
			$license_certificate_id = '';
			$download_license_certificate = '';
		
			if($data->is_from_v == 1 && $data->contractor_max_no_of_labours_on_any_day > 9){ 				
							
				/*** CONTRACTOR PRESENT ACTIVITY ***/
				
				$contractor_last_activity = db_select('l_contractor_activity_tag','lcat');
				$contractor_last_activity->fields('lcat', array('id', 'application_status', 'flag','from_v_serial_no','created_by'));
				$contractor_last_activity->condition('lcat.from_v_serial_no', $data->id);
				$contractor_last_activity->orderBy('lcat.id', 'DESC');
				$contractor_last_activity->range(0,1);						
				$activity_result = $contractor_last_activity->execute()->fetchAssoc();
					
				if(!empty($activity_result)){
					
					if($activity_result['application_status'] == 'I'){
						$classname='status-issued'; 
						$application_status = 'ISSUED</div>';
						$service_flag = $activity_result['flag'];
						
					}elseif($activity_result['application_status'] == 'R'){
						$classname='status-rejected'; 
						$application_status = 'REJECTED</div>';
						$service_flag = $activity_result['flag'];
						
					}else{ 
						$classname='status-in-process'; 
						$application_status = 'IN PROCESS</div>';
						$service_flag = $activity_result['flag'];
					}
					if($service_flag == 'L'){
						$service_name = '<div class="status-btm '.$classname.'">LICENSE&nbsp;';
					}elseif($service_flag == 'R'){
						$service_name = '<div class="status-btm '.$classname.'">RENEWAL&nbsp;';
					}elseif($service_flag == 'A'){
						$service_name = '<div class="status-btm '.$classname.'">AMENDMENT&nbsp;';
					}
					
				}else{
					$service_name = '';
					$application_status	= '<div class="status-btm status-not-applied">NOT APPLIED</div>';
				}
				
				if($data->contractor_type == 2){
					$Download_From_V = l('<i class="fa fa-chevron-circle-right"></i> Download Form-V','pdf_from_v_download/'.$data->id,  array('html' => TRUE,'attributes' => array('target' => '_blank')));
				}else{
					$Download_From_V = '<span style="color:#337ab7;"><i class="fa fa-chevron-circle-right"></i> <strong>REF. NO- 00'.$data->id.'</strong></span>';
				}
				
				
				if($data->contractor_user_id!='' || $data->contractor_user_id!=0){
					$license_res = basic_information_for_ammendment($data->id, $data->contractor_user_id);
					if(!empty($license_res)){
						$license_number = '<br/><strong>'.$license_res['contractor_license_number'].'</strong>';
					}
				}else{
						$license_number = '';
				}
			}else{
				if($status == 'I' && $data->is_from_v == 0){
					$application_status	= '<div class="status-btm status-not-reqd">DEACTIVATED</div>';	
				}elseif($status != 'I' && $data->is_from_v == 0){
					$application_status	= '<div class="status-btm status-not-applied" style="font-size:11px; width:160px;">APPLICATION NOT ISSUED</div>';	
				}else{
					$application_status	= '<div class="status-btm status-not-reqd">LICENSE NOT REQD.</div>';
				}
				$Download_From_V = '<i class="fa fa-chevron-circle-right"></i> License Not Applicable';
				$service_name = '';
				$download_license_certificate = '';
				$license_number	= '';
			}
		
			$contractor_wrk_name = db_select('l_clra_con_nature_of_work', 'lccnw');
			$contractor_wrk_name->leftJoin('contractor_works', 'cw', 'cw.contractor_work_id=lccnw.contractor_nature_of_work');
			$contractor_wrk_name->fields('lccnw', array('contractor_nature_of_work'));
			$contractor_wrk_name->fields('cw', array('cont_work_name'));
			$contractor_wrk_name->condition('lccnw.user_id', $user->uid);
			$contractor_wrk_name->condition('lccnw.contractor_id', $data->id);
			$contractor_wrk_name->condition('lccnw.application_id', $application_id);
			$result_2 = $contractor_wrk_name->execute()->fetchAll(); 
			
			$cont_work_name	= '';
			$prefix	= '';
			
			foreach ($result_2 as  $con_nature_of_wrk){
				
				if($con_nature_of_wrk->contractor_nature_of_work == 28){
					$cont_work_name .= 'Others';
				}else{
					$cont_work_name .= $prefix.$con_nature_of_wrk->cont_work_name;
					$prefix = ', ';
				}
			}
			
			if($activity_result['application_status'] == 'R'){
				$Download_From_V	= '<i class="fa fa-chevron-circle-right"></i> <font style="color:#ff0;">FORM-V Rejected </font>';
			}
										
			if($amendment_parent_id!='' || $amendment_parent_id!=0){
				$output .= "<tr>";		
				$output .= "<td align='left'>".$counter."</td>";			
				$output .= "<td align='left'>".ucfirst($data->name_of_contractor).$license_number."</td>";
				$output .= "<td align='left'>".$cont_work_name."</td>";
				$output .= "<td align='left'>".date('dS M, Y',strtotime($data->est_date_of_work_of_each_labour_from_date))."<br/>
							<b>To</b><br/>".date('dS M, Y',strtotime($data->est_date_of_work_of_each_labour_to_date))."'</center></td>";
				$output .= "<td style='border-right: 1px solid #d1d1d1;' align='left'>".$data->contractor_max_no_of_labours_on_any_day."</center></td>";	
				$output .= "</tr>";	
				
			}else{
				$output .= "<tr>";		
				$output .= "<td align='left'>".$counter."</td>";			
				$output .= "<td align='left'>".ucfirst($data->name_of_contractor)."</td>";
				$output .= "<td align='left'>".$cont_work_name."</td>";
				$output .= "<td align='left'>".date('dS M, Y',strtotime($data->est_date_of_work_of_each_labour_from_date))."<br/>
							<b>To</b><br/>".date('dS M, Y',strtotime($data->est_date_of_work_of_each_labour_to_date))."</center></td>";
				$output .= "<td style='border-right: 1px solid #d1d1d1;' align='left'>".$data->contractor_max_no_of_labours_on_any_day."</center></td>";	
				$output .= "</tr>";				
			}							
		}
				$output .= "</tbody></table>";
	}
	return pdf_using_mpdf_api($output, 'contractor-list-'.date("d-m-Y-h-m-s"));

}
	
function get_clra_contractor_info_form_excel($id=''){
	global $user;
	$application_id		= encryption_decryption_fun('decrypt', $id);
	$user_id 			= $user->uid;
	$registration_number = '';
	$e_name	= '';
	$dataClraMaster = db_query("select lcrm.id, lcrm.final_submit_status, lcrm.identification_number, lcrm.registration_number, lcrm.status, lcrm.amendment_parent_id, lcrm.e_name from l_clra_registration_master lcrm where lcrm.user_id=:user_id and lcrm.id=:applicationid", array(':user_id' => $user_id, ':applicationid' =>$application_id))->fetchAssoc();
	
	if(!empty($dataClraMaster)){
		$identification_number = $dataClraMaster['identification_number'];
		$final_submit_status = $dataClraMaster['final_submit_status'];
		$status	= $dataClraMaster['status'];
		$amendment_parent_id = $dataClraMaster['amendment_parent_id'];
			if(!empty($dataClraMaster['e_name'])){
				$e_name	= $dataClraMaster['e_name'];
			}
			if(!empty($dataClraMaster['registration_number'])){
				$registration_number = '['.$dataClraMaster['registration_number'].']';
			}
	}
	
	$filename = str_replace(" ", "-", trim($e_name)).'-'.date("d-m-Y").'.xls';
	$heading_text = 'List of contractors ('.$e_name.$registration_number.')';
	
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
    					'borders' 	=> array(
											'bottom' => $default_border,
											'left'   => $default_border,
											'top'	 => $default_border,
											'right'  => $default_border,
    									),
    					'fill' 	=> array(
        						'type' 	=> PHPExcel_Style_Fill::FILL_SOLID,
        						'color' => array('rgb'=>'FFFF00'),
    							),
    					'font' => array('bold' => true,)
					);
	
	$starting_pos = ord('A1');	
	$sheet->setCellValue('A1', strtoupper($heading_text));
	$sheet->setCellValue('D2', 'Address of Contractor');
	$sheet->setCellValue('I2', 'Estimated Date of Employment of Each Contract Work Under Each Contractor');
	$sheet->setCellValue('A2', 'Sl.No.');
	if($amendment_parent_id!='' || $amendment_parent_id!=0){
		$sheet->getCell('B2')->setValue("Contractor Name \n License Number");
		$sheet->getStyle('B2')->getAlignment()->setWrapText(true);
	} else {
		$sheet->setCellValue('B2', 'Contractor Name');
	}
	$sheet->setCellValue('C2', 'Email');
	$sheet->setCellValue('D3', 'Address Line 1');
	$sheet->setCellValue('E3', 'Sub-division');
	$sheet->setCellValue('F3', 'District');
	$sheet->setCellValue('G3', 'State');
	$sheet->setCellValue('H2', 'Nature of Work');
	$sheet->setCellValue('I3', 'Start Date of Contract');
	$sheet->setCellValue('J3', 'End Date of Contract');
	$sheet->setCellValue('K2', 'No. of Contract Labour');
	
	$sheet->mergeCells('A1:K1');
	$sheet->mergeCells('D2:G2');
	$sheet->mergeCells('I2:J2');
	$sheet->mergeCells('A2:A3');
	$sheet->mergeCells('B2:B3');
	$sheet->mergeCells('C2:C3');
	$sheet->mergeCells('H2:H3');
	$sheet->mergeCells('K2:K3');

	$objPHPExcel->getActiveSheet()->getRowDimension('1')->setRowHeight(25);
	$objPHPExcel->getActiveSheet()->getRowDimension('2')->setRowHeight(25);
    $objPHPExcel->getActiveSheet()->getColumnDimension('A')->setWidth(10);	
	$objPHPExcel->getActiveSheet()->getColumnDimension('B')->setWidth(20);
	
	$objPHPExcel->getActiveSheet()->getColumnDimension('C')->setWidth(25);
	$objPHPExcel->getActiveSheet()->getColumnDimension('D')->setWidth(35);
	$objPHPExcel->getActiveSheet()->getColumnDimension('E')->setWidth(30);
	$objPHPExcel->getActiveSheet()->getColumnDimension('F')->setWidth(30);
	$objPHPExcel->getActiveSheet()->getColumnDimension('G')->setWidth(30);
	$objPHPExcel->getActiveSheet()->getColumnDimension('H')->setWidth(30);
	$objPHPExcel->getActiveSheet()->getColumnDimension('I')->setWidth(30);
	$objPHPExcel->getActiveSheet()->getColumnDimension('J')->setWidth(30);
	$objPHPExcel->getActiveSheet()->getColumnDimension('K')->setWidth(30);
	

	$sheet->getStyle('A1:K1')->applyFromArray($style_header);
	$sheet->getStyle('A2:A3')->applyFromArray($style_header);
	$sheet->getStyle('B2:B3')->applyFromArray($style_header);
	$sheet->getStyle('C2')->applyFromArray($style_header);
	$sheet->getStyle('D2:G2')->applyFromArray($style_header);
	$sheet->getStyle('I2')->applyFromArray($style_header);
	$sheet->getStyle('I3')->applyFromArray($style_header);
	$sheet->getStyle('J3')->applyFromArray($style_header);
	$sheet->getStyle('H2')->applyFromArray($style_header);
	$sheet->getStyle('K2')->applyFromArray($style_header);
	$sheet->getStyle('D3')->applyFromArray($style_header);
	$sheet->getStyle('E3:G3')->applyFromArray($style_header);

	$sheet->getStyle('A:K')->applyFromArray(
							array(								
								'alignment' => 
										array(
											'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_LEFT, 
											'vertical' => PHPExcel_Style_Alignment::VERTICAL_CENTER),
    							)
							);
					
	$sheet->getStyle('A1:K3')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER)->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);	
	$sheet->getStyle('K2')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER)->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);	
	$sheet->getStyle('H2')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER)->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);	
		
	$sheet->getStyle('A1:K3')->applyFromArray(
								array(								
									'borders' => array(
												'bottom'  => $default_border,
												'left' 	  => $default_border,
												'top'	  => $default_border,
												'right'   => $default_border,
											),
									
									)		
								);
	
	/** Contractor Details ***/
	
	$counter = 0;
	$r = 4;		
	$query = db_select('l_contractor_info_master', 'lcim');
	$query->fields('lcim', array());
	$query->condition('lcim.application_id', $application_id);
	$query->condition('lcim.user_id', $user->uid);
	$query->orderBy('lcim.id','DESC');
	$contractors_list = $query->execute()->fetchAll(); 
	
	if(!empty($contractors_list)){
	$Download_From_V = '';
		$reasonStatus = '';
		
		foreach($contractors_list as  $data) {
			$counter++;
			$license_file_url = '';
			$license_res = '';
			$license_certificate_id = '';
			$download_license_certificate = '';
		
			if($data->is_from_v == 1 && $data->contractor_max_no_of_labours_on_any_day > 9){ 				
							
				/*** CONTRACTOR PRESENT ACTIVITY ***/
				
				$contractor_last_activity = db_select('l_contractor_activity_tag','lcat');
				$contractor_last_activity->fields('lcat', array('id', 'application_status', 'flag','from_v_serial_no','created_by'));
				$contractor_last_activity->condition('lcat.from_v_serial_no', $data->id);
				$contractor_last_activity->orderBy('lcat.id', 'DESC');
				$contractor_last_activity->range(0,1);						
				$activity_result = $contractor_last_activity->execute()->fetchAssoc();
					
				if(!empty($activity_result)){
					
					if($activity_result['application_status'] == 'I'){
						$classname='status-issued'; 
						$application_status = 'ISSUED</div>';
						$service_flag = $activity_result['flag'];
						
					}elseif($activity_result['application_status'] == 'R'){
						$classname='status-rejected'; 
						$application_status = 'REJECTED</div>';
						$service_flag = $activity_result['flag'];
						
					}else{ 
						$classname='status-in-process'; 
						$application_status = 'IN PROCESS</div>';
						$service_flag = $activity_result['flag'];
					}
					if($service_flag == 'L'){
						$service_name = '<div class="status-btm '.$classname.'">LICENSE&nbsp;';
					}elseif($service_flag == 'R'){
						$service_name = '<div class="status-btm '.$classname.'">RENEWAL&nbsp;';
					}elseif($service_flag == 'A'){
						$service_name = '<div class="status-btm '.$classname.'">AMENDMENT&nbsp;';
					}
					
				}else{
					$service_name = '';
					$application_status	= '<div class="status-btm status-not-applied">NOT APPLIED</div>';
				}
				
				if($data->contractor_type == 2){
					$Download_From_V = l('<i class="fa fa-chevron-circle-right"></i> Download Form-V','pdf_from_v_download/'.$data->id,  array('html' => TRUE,'attributes' => array('target' => '_blank')));
				}else{
					$Download_From_V = '<span style="color:#337ab7;"><i class="fa fa-chevron-circle-right"></i> <strong>REF. NO- 00'.$data->id.'</strong></span>';
				}
				
				
				if($data->contractor_user_id!='' || $data->contractor_user_id!=0){
					
					$license_res = basic_information_for_ammendment($data->id, $data->contractor_user_id);
					
					if(!empty($license_res)){
						$license_number = $license_res['contractor_license_number'];
					}
					
				}else{
						$license_number = '';
				}
				
			}else{
				
				if($status == 'I' && $data->is_from_v == 0){
					$application_status = '<div class="status-btm status-not-reqd">DEACTIVATED</div>';	
				}elseif($status != 'I' && $data->is_from_v == 0){
					$application_status = '<div class="status-btm status-not-applied" style="font-size:11px; width:160px;">APPLICATION NOT ISSUED</div>';	
				}else{
					$application_status = '<div class="status-btm status-not-reqd">LICENSE NOT REQD.</div>';
				}
				$Download_From_V = '<i class="fa fa-chevron-circle-right"></i> License Not Applicable';
				$service_name = '';
				$download_license_certificate = '';
				$license_number = '';
			}
		
			$contractor_wrk_name = db_select('l_clra_con_nature_of_work', 'lccnw');
			$contractor_wrk_name->leftJoin('contractor_works', 'cw', 'cw.contractor_work_id=lccnw.contractor_nature_of_work');
			$contractor_wrk_name->fields('lccnw', array('contractor_nature_of_work'));
			$contractor_wrk_name->fields('cw', array('cont_work_name'));
			$contractor_wrk_name->condition('lccnw.user_id', $user->uid);
			$contractor_wrk_name->condition('lccnw.contractor_id', $data->id);
			$contractor_wrk_name->condition('lccnw.application_id', $application_id);
			$result_2 = $contractor_wrk_name->execute()->fetchAll(); 
			
			$cont_work_name	= '';
			$prefix	= '';
			
			foreach ($result_2 as  $con_nature_of_wrk){
				
				if($con_nature_of_wrk->contractor_nature_of_work == 28){
					$cont_work_name .= 'Others';
				}else{
					$cont_work_name .= $prefix.$con_nature_of_wrk->cont_work_name;
					$prefix = ', ';
				}
				
			}							
			if($activity_result['application_status'] == 'R'){
				$Download_From_V = '<i class="fa fa-chevron-circle-right"></i> <font style="color:#ff0;">FORM-V Rejected </font>';
			}
			if(!empty($data->con_loc_e_dist)){
				$query = db_select('district_master', 'd');
				$query->fields('d', array('district_name'));
				$query->condition('d.district_code', $data->con_loc_e_dist); 
				$query = $query->execute()->fetchAssoc();
				$cont_dist = $query['district_name'];
			}else{
				$cont_dist = 'N/A';
			}
			if(!empty($data->con_loc_e_subdivision)){
				$query	=	db_select('sub_division', 's');
				$query->fields('s', array('sub_div_name'));
				$query->condition('s.sub_div_code', $data->con_loc_e_subdivision); 
				$query = $query->execute()->fetchAssoc();
				$cont_sub_div = $query['sub_div_name'];
			}else{
				$cont_sub_div = 'N/A';
			}
			
			if(!empty($data->state)){
				$query = db_select('state_master', 'sm');
				$query->fields('sm', array('statename'));
				$query->condition('sm.id', $data->state_opts); 
				$query = $query->execute()->fetchAssoc();
				$cont_state = $query['statename'];
			}
			
		
				$sheet->setCellValue('A'.$r, $counter);
			if($amendment_parent_id!='' || $amendment_parent_id!=0){
				$objRichText = new PHPExcel_RichText();
				$objRichText->createText(ucfirst($data->name_of_contractor)."\n");
				
				$objBold = $objRichText->createTextRun($license_number);
				$objBold->getFont()->setBold(true);
				
				$sheet->getCell('B'.$r)->setValue($objRichText);
				$sheet->getStyle('B'.$r )->getAlignment()->setWrapText(true);
				//$sheet->setCellValue('B'.$r, $name_of_contractor.$license_number);
				$sheet->setCellValue('C'.$r, $data->email_of_contractor);
				$sheet->setCellValue('D'.$r, $data->address_of_contractor);
				$sheet->setCellValue('E'.$r, $cont_sub_div);
				$sheet->setCellValue('F'.$r, $cont_dist);
				$sheet->setCellValue('G'.$r, $cont_state);
				$sheet->setCellValue('H'.$r, $cont_work_name);
				$sheet->setCellValue('I'.$r, date('dS M, Y',strtotime($data->est_date_of_work_of_each_labour_from_date)));
				$sheet->setCellValue('J'.$r, date('dS M, Y',strtotime($data->est_date_of_work_of_each_labour_to_date)));
				$sheet->setCellValue('K'.$r, $data->contractor_max_no_of_labours_on_any_day);
				$r++;		
			}else{
				$sheet->setCellValue('B'.$r, ucfirst($data->name_of_contractor));	
				$sheet->setCellValue('C'.$r, $data->email_of_contractor);
				$sheet->setCellValue('D'.$r, $data->address_of_contractor);
				$sheet->setCellValue('E'.$r, $cont_sub_div);
				$sheet->setCellValue('F'.$r, $cont_dist);
				$sheet->setCellValue('G'.$r, $cont_state);
				$sheet->setCellValue('H'.$r, $cont_work_name);
				$sheet->setCellValue('I'.$r, date('dS M, Y',strtotime($data->est_date_of_work_of_each_labour_from_date)));
				$sheet->setCellValue('J'.$r, date('dS M, Y',strtotime($data->est_date_of_work_of_each_labour_to_date)));
				$sheet->setCellValue('K'.$r, $data->contractor_max_no_of_labours_on_any_day);
				$r++;	  
			}	
			} 
		
	}
		$objPHPExcel->setActiveSheetIndex(0);
		$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');	
		ob_end_clean();
		// Redirect output to a client’s web browser (Excel5)
		header('Content-Type: application/vnd.ms-excel;charset:UTF-8');
		header('Content-Disposition: attachment;filename='.basename(strtolower($filename)));
		header('Cache-Control: max-age=0');
		// If you're serving to IE 9, then the following may be needed
		header('Cache-Control: max-age=1');	
		$objWriter->save('php://output');
	   	exit;						
}

?>