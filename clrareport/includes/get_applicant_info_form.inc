<?php

function user_list_view($form, &$form_state){
	
	global $user, $base_root, $base_path;
	
	$uid			=	$user->uid;
	$query_dis_id	=	db_query('select distid from l_custom_user_detail where usr_id = :uid', array(':uid' => trim($uid) ));	
	$result			=	$query_dis_id->fetchAssoc();
	$district_id 	= 	$result['distid']; 
	
	
	$form['main_header'] = array(									   
		'#type' 		 => 'markup',
		'#markup' 		 => '<div class="row">',	    							
	);
	
	$form['header'] = array(									   
		'#type' 		 => 'markup',
		'#markup' 		 => '<div class="col-md-3"><div class="box box-primary"><div class="box-body">',	    							
	);
	
			
	//$options_services =	array('1' => 'P.E. Registration Under CLRA', '12'=>'Contractor License Under CLRA ', '2' => 'Est.Registration Under BOCWA','4' => 'Registration Under ISMW','5' => 'Annual Return User List');
	
	
	$options_services =	array('12'=>'Contractor License Under CLRA');
	
	$form['services'] = array(
		'#prefix' 	=> 	'',
		'#suffix' 	=> 	'',
		'#title'	=> 	'Select services',
		'#type' 	=> 	'select',
		'#options' 	=>	$options_services,
		'#attributes'	=> array('class' => array('form-control')),
		'#required'		=>	TRUE
	);
											
																			
	$options_dlc = report_district_list_new($uid, $district_id);			 
				 
	$form['district_id'] = array(
		'#prefix' 		=>	'',
		'#suffix'		=>	'',
		'#title'		=>	'Select District',
		'#type' 		=> 	'select',
		'#options' 		=>	$options_dlc,
		'#required'		=>	TRUE,
		'#id' 			=> 'district_id',
		'#attributes'	=> array('class' => array('form-control')),
		'#ajax'		  	=> array(
						'event' 			=> 'change',
						'effect' 			=> 'fade',			
						'callback'			=> 'get_ajax_callback',
						'progress'			=> array('type' => 'throbber'),
						)
	);

	
	$value_dist = isset($form_state['values']['district_id']) ? $form_state['values']['district_id'] : '';
	
	if($value_dist != "" && !empty($value_dist) && is_numeric($value_dist)){
		$form['subdivision_code']  		= array(
			'#prefix'		=> '<div id="subdvsn_code_replace">',
			'#suffix'		=> '</div>',								
			'#title'		=> 'Select Subdivision',
			'#type'			=> 'select',
			'#attributes'	=> array('class' => array('form-control')),
			'#options'		=> custom_user_subdivisionlist_list_new($value_dist),
			'#ajax'		  	=> array(
						'event' 			=> 'change',
						'effect' 			=> 'fade',			
						'callback'			=> 'get_ajax_callback',
						'progress'			=> array('type' => 'throbber'),
						)
		);
	}else{
		$form['subdivision_code'] 		= array(
			'#prefix' 		=> 	'<div id="subdvsn_code_replace">',
			'#suffix' 		=> 	'</div>',								
		);
	}
	
	$value_subdiv = isset($form_state['values']['subdivision_code']) ? $form_state['values']['subdivision_code'] : '';
	
	if($value_dist != "" && !empty($value_subdiv) && is_numeric($value_subdiv)){
		$form['blockmun_code']  		= array(
			'#prefix'		=> '<div id="blockmun_code_replace">',
			'#suffix'		=> '</div>',								
			'#title'		=> 'Select Block/Mun./Crop./SEZ/NA',
			'#type'			=> 'select',
			'#attributes'	=> array('class' => array('form-control')),
			'#options'		=> report_block_mun_list_new($value_dist, $value_subdiv)
		);
	}else{
		$form['blockmun_code'] 		= array(
			'#prefix' 		=> 	'<div id="blockmun_code_replace">',
			'#suffix' 		=> 	'</div>',								
		);
	}
	
	
	
	
	$form['submit'] = array(
		'#type' 		 => 'submit',
		'#prefix' 		 => '',
		'#suffix' 		 => '', 
		'#value' 		 => 'Search',
		'#attributes' 	 => array('class' => array('btn btn-primary')),
		'#ajax' 		 => array(
								'callback' => 'get_search_result',
								'wrapper'  => 'report_div',
								'method'   => 'replace',
								'effect'   => 'fade'
							)
	);	
	
	$form['footer'] = array(									   
		'#type' 		 => 'markup',
		'#markup' 		 => '</div></div></div>',	    							
	);
							
	
	$form['markup'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="col-md-9">
						<div class="box box-primary">
							<div id="report_div">
								<div class="box-body">Search Result...</div>
							</div>
						</div>
					</div>',
	); 	
	
	$form['main_footer'] = array(									   
		'#type' 		 => 'markup',
		'#markup' 		 => '</div>',	    							
	);
																				
	return $form;
}

function __set_year_range($form_element, $form_values) {
    $form_element['year']['#options'] = drupal_map_assoc(range(2014, date("Y")));
    return $form_element;
}

function report_district_list_new($dlc_id = '', $district_id = '') {
	$default_select 	= array(""=>"- Select District -");
	global $base_root, $base_path, $user;
	
	$pre_role = db_query("SELECT r.rid FROM users_roles ur LEFT JOIN role r ON r.rid=ur.rid WHERE ur.uid=:user_id LIMIT 1", array( ':user_id' => $user->uid));
	$frole = $pre_role->fetchObject();
	$role_id = $frole->rid;
	if($role_id == 12){
		$result 	 = db_query("SELECT district_code,district_name FROM {district_master} where is_active = 'Y' order by district_name");
		$list 	 = array();
		
		if(count($result) > 1){
			$default_select 	= array("all"=>"All");
		}
		if(count($result) > 0){
			foreach ($result as $record) {
				$list[$record->district_code] = t('@districtname', array(
						'@districtname' => $record->district_name,
				));
			}
		}
	}elseif($role_id == 5){
		$result 	 = db_query("SELECT district_code,district_name FROM {district_master} where district_code in (select distict_code from  l_inspector_random_master where user_id='$dlc_id' group by distict_code) and is_active = 'Y' order by district_name");
		$list 	 = array();
		if(count($result) > 1){
			$default_select 	= array("all"=>"All");
		}
		if(count($result) > 0){
			foreach ($result as $record) {
				$list[$record->district_code] = t('@districtname', array(
						'@districtname' => $record->district_name,
				));
			}
		}
	}else{
		$result 	 = db_query("SELECT district_code,district_name FROM {district_master} where is_active = 'Y' and district_code ='".$district_id."' order by district_name");
		$list 	 = array();
		
		if($result->rowCount() > 1){
			$default_select 	= array("all"=>"All");
		}
		if($result->rowCount() > 0){
			foreach ($result as $record) {
				$list[$record->district_code] = t('@districtname', array(
						'@districtname' => $record->district_name,
				));
			}
		}
	}
	return ($default_select+$list);
}

function report_block_mun_list_new($district = '', $subdivision = ''){
	$result = db_query("select block_code, block_mun_name from block_mun_master where is_active = 'Y' and distict_code = '".$district."' and sub_division_code = '".$subdivision."'");
	$list 	 = array();
	$default_select = array(''=>'-Select-');
	
	/*if($result->rowCount() > 1){
		$default_select 	= array("all"=>"All");
	}*/
	if($result->rowCount() > 0){
		foreach ($result as $record) {
			$list[$record->block_code] = t('@districtname', array(
					'@districtname' => $record->block_mun_name,
			));
		}
	}
	return ($default_select+$list);
}

function custom_user_subdivisionlist_list_new($value_dist){
	$default_subdvselect 	= array(""=>"- Select Sub-division -");
	$all_subdvs 			= array();
	$query_subdv 			= db_select('sub_division', 'subdv');
	$query_subdv			->fields('subdv',array('sub_div_code','sub_div_name'));
	$query_subdv			->orderBy('subdv.sub_div_name', 'ASC');
	$query_subdv			->condition('subdv.is_active','Y');
	$query_subdv			->condition('subdv.district_code',(int)$value_dist);
	$result_subdv 			= $query_subdv->execute(); 
	if(!empty($result_subdv)) {      
		foreach($result_subdv as $rows) {
			$all_subdvs[$rows->sub_div_code] = t('@subdvsname', array('@subdvsname' => $rows->sub_div_name));
		}    
	}
	if (isset($all_subdvs)) {			
		return ($default_subdvselect + $all_subdvs);	
	} else {				
		return $default_subdvselect;
	}
}

function get_ajax_callback($form, $form_state){
	 $commands 	= array();
	 $commands[] = ajax_command_replace('#subdvsn_code_replace', drupal_render($form['subdivision_code']));
	 $commands[] = ajax_command_replace('#blockmun_code_replace', drupal_render($form['blockmun_code']));
	 
	 return array('#type' => 'ajax', '#commands' => $commands);
}

function get_search_result($form, &$form_state){
	
	$val = $form_state['values'];
	
	$services = $val['services'];
	$district = $val['district_id'];
	$subdivision = isset($val['subdivision_code']) ? $val['subdivision_code'] : '0';
	$blockmun = isset($val['blockmun_code']) ? $val['blockmun_code'] : '0';
	
	
	$getdata	= '';
	if($district !=''){
		$getdata = get_result_data($services,$district,$subdivision,$blockmun);
	}
	
	$link = $services.'/'.encryption_decryption_fun('encrypt', $district).'/'.encryption_decryption_fun('encrypt', $subdivision).'/'.encryption_decryption_fun('encrypt', $blockmun);
	
	$downloadlink = l('<i class="fa fa-download"></i> Download Excel','user-list-data/download/'.$link,array('html'=>TRUE,'attributes' => array('class' => array('btn btn-default pull-right'))));
	
	
	$pdflink = l('<i class="fa fa-print"></i> Print Result','user-list-data/print/'.$link,array('html'=>TRUE,'attributes' => array('class' => array('btn btn-default pull-right'), 'target' => '_blank')));
	
	
	$form['markup']	=	array(
		'#type'		=>	'markup',
		'#markup'	=>	'<div id="report_div">
							<div class="box-header">
								<div class="box-title">Search Result...</div>
								'.$downloadlink.$pdflink.'
							</div>
							<div class="box-body">'.$getdata.'</div>
						</div>'	
	);
	
	
	$commands[] = ajax_command_html('#report_div', render($form['markup']));
 
	return array('#type' => 'ajax', '#commands' => $commands);
	
}

function get_result_data($services = 0,$district = '',$subdivision = '', $blockmun= ''){
	if( $services == 1 ){
		$getdata	= array();	
	}else if($services == 12){
		$getdata	= getLicenceData($services,$district,$subdivision,$blockmun);
	}
	
	//echo $services.'<br>'.$district.'<br>'.$subdivision.'<br>'.$blockmun; die;
	
	$header = array(
		array('data' => 'SL.No.', 'width' => '5%'),
		array('data' => 'ESTABLISHMENT'),
		array('data' => 'NAME OF CONTRACTOR'),
		array('data' => 'ADDRESS'),
		array('data' => 'BLOCK/MUN/SEZ', 'width' => '15%'),
		array('data' => 'CONTACT INFO', 'width' => '15%'),
		
		//array('data' => 'MOBILE', 'width' => '15%'),
		//array('data' => 'EMAIL', 'width' => '5%')
	);
	
	if(!empty($getdata)){$counter=1;
		foreach($getdata as $data){
			
			$full_address = $data->address.'<br>'.$data->district.', '.$data->subdivision.'<br>'.$data->block.', '.$data->village.'<br>'.$data->ps.', '.$data->pin;
			
			$rows[] = array(
				array('data' =>	$counter++, 'align' => 'left', 'class' => array('')),
				array('data' =>	strtoupper($data->estname), 'align' => 'left', 'class' => array('')),
				array('data' =>	strtoupper($data->name), 'align' => 'left', 'class' => array('')),
				array('data' =>	strtoupper($full_address), 'align' => 'left', 'class' => array('')),
				array('data' => $data->worksiteblockmun, 'align' => 'left', 'class' => array('')),
				array('data' => $data->mobile.'<br>'.$data->email, 'align' => 'left', 'class' => array(''))
		  	);
		}
	}
	
	$variables = array('attributes' => array('class' => array('box-body table-responsive no-padding table table-hover admin-custom-table')), 'header' => $header, 'rows' => $rows, 'empty' => t("No data found!"));
	$output = theme('datatable', $variables);
	
	
	return $output;
			
}


function getLicenceData($services = 0,$district = '',$subdivision = '',$blockmun = ''){
	
	//echo $district.'--'.$subdivision; 
	
	$district_query = '';
	$subdivision_query = '';
	$blockmun_query = '';
	
	if($district !='' && $district != 'all'){
		$district_query = " and pa.worksite_dist = '".$district."' ";
	}
	
	if($district !='' && ($subdivision !='' || !empty($subdivision))){
		$subdivision_query = " and pa.worksite_subdivision = '".$subdivision."' ";
	}
	
	if($district !='' && ($subdivision !='' || !empty($subdivision)) && ($blockmun != '')){
		$blockmun_query = " and pa.name_work_site_areatype = '".$blockmun."' ";
	}
	
	//echo '<pre>';
	$sql = "
		select cm.name_of_contractor as name, caf.mobile as mobile, caf.email as email,pa.address_of_contractor as address, vm.village_name as village, bmm.block_mun_name as block, sd.sub_div_name as subdivision, dm.district_name as district, psm.name_of_police_station as ps, pa.contractor_pin as pin, pem.e_name as estname, sdw.sub_div_name as worksitesubdiv,bmmw.block_mun_name as worksiteblockmun 
		from l_particular_of_contract_labour as pa
		left join l_contractor_info_master as cm on cm.id = pa.serial_no_from_v
		left join l_clra_registration_master as pem on pem.id = cm.application_id and pem.user_id = cm.user_id
		left join l_common_application_master as caf on caf.user_id = pa.contractor_user_id
		left join district_master as dm on dm.district_code = pa.contractor_dist
		left join sub_division as sd on sd.sub_div_code = pa.contractor_subdivision and sd.district_code = pa.contractor_dist
		left join block_mun_master as bmm on bmm.block_code = pa.contractor_name_areatype and bmm.sub_division_code = pa.contractor_subdivision
		left join village_ward_master as vm on vm.village_code = pa.contractor_vill_ward
		left join police_station as psm on psm.police_station_code = pa.contractor_ps
		left join l_contractor_license_application as cl on cl.contractor_particular_id = pa.id
		
		left join sub_division as sdw on sdw.sub_div_code = pa.worksite_subdivision and sdw.district_code = pa.worksite_dist
		left join block_mun_master as bmmw on bmmw.block_code = pa.name_work_site_areatype and bmmw.sub_division_code = pa.worksite_subdivision
		
		
		
		where cl.status = 'I' ".$district_query.$subdivision_query.$blockmun_query."
		order by pem.e_name asc
		";
		
		$query = db_query($sql);
		if($query->rowCount() > 0){
			$result = $query->fetchAll();
		}else{
			$result = array();
		}
		
		return $result;
}

function user_list_data_print($type = '',$services = 0,$district = '',$subdivision = '',$blockmun = ''){
	
	$district = encryption_decryption_fun('decrypt', $district); 
	$subdivision = encryption_decryption_fun('decrypt', $subdivision);
	$blockmun = encryption_decryption_fun('decrypt', $blockmun);
	
	$result = array();
	$district_name = $sub_div_name = $block_mun_name = '';
	
	if($services == 12){$act_name = 'License of Contractor Under CLRA';}
	
	$district_name = db_query("select district_name from district_master where district_code ='".$district."'")->fetchObject()->district_name;
	
	if($subdivision != '' || $subdivision != 0 || !empty($subdivision)){		
		$sub_div_name = db_query("select sub_div_name from sub_division where sub_div_code ='".$subdivision."'")->fetchObject()->sub_div_name.', ';	
	}
	
	if($blockmun != '' && $blockmun != 0 && !empty($blockmun)){		
		$block_mun_name = db_query("select block_mun_name from block_mun_master where block_code ='".$blockmun."'")->fetchObject()->block_mun_name.', ';	
	}
	
	if($district !='' || $district !=0){
		if($services == 12){
			$result = getLicenceData($services,$district,$subdivision,$blockmun);
		}
	}
	
	//echo 'Block -'.$blockmun.'====='.$block_mun_name.'SUB -'.$sub_div_name.'Dis -'.$district_name;
	
	//echo '<pre>'; print_r($result);die;
	
	
	$output = '<style>			
					.clra_report tr td{border: 1px solid #d1d1d1;padding: 6px; border-top:none;border-right:none; font-size:12px;}			
					.clra_report tr th{background: #dddddd none repeat scroll 0 0;border: 1px solid #d1d1d1; padding: 5px; font-size:12px;}
					.note_text{padding-top: 6px; font-size:8px;}
				</style>
				<table width="100%" border="0" cellspacing="0" cellpadding="0" class="clra_report">
					<thead>
						 <tr><td colspan="6" align="center" style="font-size:15px; border:0px;"><strong>LABOUR COMMISSIONERATE</strong></td></tr>
						 <tr><td align="center" colspan="6" align="center" style="font-size:12px; border:0px;"><strong>GOVERNMENT OF WEST BENGAL</strong></td></tr>
						 <tr><td align="center" colspan="6" align="center" style="font-size:12px; border:0px;"><i>All Registered Applicant User in West Bengal </i><br>'.$block_mun_name.$sub_div_name.$district_name.'</td></tr>
						 <tr>
							<th><strong>SL.No.</strong></th>
							<th align="center"><strong>ESTABLISHMENT NAME</strong></th>
							<th align="center"><strong>CONTRACTOR NAME</strong></th>
							<th width="30%"><strong>ADDRESS</strong></th>
							<th><strong>CONTACT NUMBER</strong></th>
							<th width="12%"><strong>EMAIL</strong></th>
							
						</tr>
					</thead>
					<tbody>';
	
	
	
	if(!empty($result)){ $count = 1;
		foreach($result as $data){
			$output .= '<tr>';
			$output .= '<td>'.$count++.'</td>
						<td>'.$data->estname.'</td>
						<td>'.$data->name.'</td>
						<td>'.$data->address.'</td>
						<td>'.$data->mobile.'</td>
						<td style="border-right: 1px solid #d1d1d1;">'.$data->email.'</td>';			
			$output .= '</tr>';
		}
	}
	$output .= "</tbody></table>";
	
	//echo $output;
	
	
	return pdf_using_mpdf_api($output, 'all-registered-applicant-user-in-west-bengal-'.date("d-m-Y-h-m-s"));	
	
}


function user_list_data_download($type = '',$services = 0,$district = '',$subdivision = '', $blockmun = ''){
	
	$district = encryption_decryption_fun('decrypt', $district);
	$subdivision = encryption_decryption_fun('decrypt', $subdivision); 
	$blockmun = encryption_decryption_fun('decrypt', $blockmun);
	
	if($services == 12){$act_name = 'License of Contractor Under CLRA';}
	
	if($district !='' || $district !=0){
		$result = getLicenceData($services,$district,$subdivision,$blockmun);
	}else{
		$result = array();
	}
	
	if($blockmun != ''){		
		$block_mun_name = db_query("select block_mun_name from block_mun_master where block_code ='".$blockmun."'")->fetchObject()->block_mun_name.', ';	
	}
	
	//echo '<pre>'; print_r($result);die;
	
	
	$heading_text = '';
	$sub_div_name = '';
	
	
	$district_name = db_query("select district_name from district_master where district_code ='".$district."'")->fetchObject()->district_name;
	$district_name2 = str_replace(" ", "-", trim($district_name));

	if($subdivision != '' || !empty($subdivision) || $subdivision != ''){		
		$sub_div_name = db_query("select sub_div_name from sub_division where sub_div_code ='".$subdivision."'")->fetchObject()->sub_div_name;
		$sub_div_name2 = str_replace(" ", "-", trim($sub_div_name));
	}
	if(!empty($sub_div_name2)){
		$filename = $sub_div_name2.'-'.$district_name2.'-'.date("d-m-Y").'.xls';
		$heading_text = 'All Certified Applicant Of '.$act_name.' In '.$sub_div_name.', '.$district_name.' West Bengal';
	} else {
		$filename = 'All District'.date("d-m-Y").'.xls';
		// $heading_text = 'All Registered User in '.$district_name.' West Bengal';
		$heading_text = 'All Certified Applicant Of '.$act_name.' In '.$sub_div_name.', West Bengal';
	}
	
	
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
			'bottom'  => $default_border,
			'left' 	  => $default_border,
			'top'	  => $default_border,
			'right'   => $default_border,
		),
		'fill' 	=> array(
			'type' 	=> PHPExcel_Style_Fill::FILL_SOLID,
			'color' => array('rgb'=>'FFFF00'),
		),
		'font' => array(
			'bold' => true
		)
	);
	
	$starting_pos = ord('A1');	
	$sheet->setCellValue('A1', strtoupper($heading_text));	
	$sheet->setCellValue('A2', 'Sl.No.');
	
	
	if($services==12){
		$sheet->setCellValue('B2', 'Eshtablishment Name');
		$sheet->setCellValue('C2', 'Name of Contractor');
		$sheet->setCellValue('D2', 'Address');
		$sheet->setCellValue('E2', 'Subdivision Name');
		$sheet->setCellValue('F2', 'Block-Mun-Cor-SEZ-NA Name');
		$sheet->setCellValue('G2', 'Mobile');
		$sheet->setCellValue('H2', 'Email');
		
		$sheet->mergeCells('A1:H1');
		
	}
	
	
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
		

		
	$sheet->getStyle('A1')->applyFromArray($style_header);
	$sheet->getStyle('B1')->applyFromArray($style_header);
	$sheet->getStyle('C1')->applyFromArray($style_header);
	$sheet->getStyle('D1')->applyFromArray($style_header);
	$sheet->getStyle('E1')->applyFromArray($style_header);
	$sheet->getStyle('F1')->applyFromArray($style_header);
	$sheet->getStyle('G1')->applyFromArray($style_header);
	$sheet->getStyle('H1')->applyFromArray($style_header);
	
					
	$sheet->getStyle('A1')->applyFromArray(
		array(								
			'alignment' => 
				array(
					'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER, 
					'vertical' => PHPExcel_Style_Alignment::VERTICAL_CENTER
				),	
    		)
	);
	
	$sheet->getStyle('A')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_LEFT);				
	$sheet->getStyle('E')->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_LEFT);				
	$sheet->getStyle('A2:H2')->applyFromArray(
		array(								
			'borders' => array(
				'bottom'  => $default_border,
				'left' 	  => $default_border,
				'top'	  => $default_border,
				'right'   => $default_border,
	    	),
			'alignment' => array(	
			'vertical' => PHPExcel_Style_Alignment::VERTICAL_CENTER),
			'font' => array('bold' => true)
		)
	);
	$r = 3;	
	if(!empty($result)){ $counter = 0;
		foreach($result as $data){ 
			$r++; $counter++;
			
			//$full_address = $data->address.', '.$data->district.', '.$data->subdivision.', '.$data->block.', '.$data->village.', '.$data->ps.', '.$data->pin;
			
			$sheet->setCellValue('A'.$r, $counter);
			$sheet->setCellValue('B'.$r, $data->estname);
			$sheet->setCellValue('C'.$r, $data->name);
			$sheet->setCellValue('D'.$r, $data->address);
			$sheet->setCellValue('E'.$r, $data->worksitesubdiv);
			$sheet->setCellValue('F'.$r, $data->worksiteblockmun);
			
			$sheet->setCellValue('G'.$r, $data->mobile);
			$sheet->setCellValue('H'.$r, $data->email);
			
		}
	}
	
	$objPHPExcel->setActiveSheetIndex(0);
	$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');	
	ob_end_clean();
	
	header('Content-Type: application/vnd.ms-excel;charset:UTF-8');
	header('Content-Disposition: attachment;filename='.basename(strtolower($filename)));
	header('Cache-Control: max-age=0');
	
	header('Cache-Control: max-age=1');	
	$objWriter->save('php://output');
	exit;
	
	
}
