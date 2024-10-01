<?php

function get_annual_return_report_form_public($form, &$form_state){
	
	global $user, $base_root, $base_path; 
  
	
	$form['insmonreports']['header'] = array(
									'#markup' => '<div class="row"><div class="col-md-12 analytic-report-box analytical-bg">
													  <div class="box box-primary">	 				
													      <div class="box-body">'
	);
			
	$options_services =	array( ""=> 'Select Service', '1' => 'P.E. Registration Under CLRA', '2' => 'Est.Registration Under BOCWA', '3' => 'Est.Registration Under MTW', '4' => 'License Under The acts of CLRA','5' => 'Est.Registration Under ISMW','6' => 'Maternity Benefit Act');
	
	$form['insmonreports']['services'] = array(
												'#prefix' 	=> 	'<div class="form-custom col-lg-6 col-md-6 col-sm-12 col-xs-12 form-full-label"><label>',
						  						'#suffix' 	=> 	'</label></div><div class="clear"></div>',
												'#title'	=> 	'<b>Select Services</b>',
												'#type' 	=> 	'select',
												'#options' 	=>	$options_services,
												'#attributes' 	=> array('class' => array('form-control')),
												'#required'		=>	TRUE
	);
												
	$value_services = isset($form_state['values']['services']) ? $form_state['values']['services'] : '';
	$options_first = common_custom_user_district_list_new(); 
  
    $value_dropdown_first = isset($form_state['values']['disname']) ? $form_state['values']['disname'] :key($options_first); 
	   
	$form['insmonreports']['boxheader'] = array(
		  						'#type' 	=> 'markup',
          						'#markup' 	=> '<div class="col-lg-6 col-md-6 col-sm-12 col-xs-12">',
	);	   
	    
     $form['insmonreports']['disname'] = array(		
									'#title'		 => 'Select District',	   
									'#type' 		 => 'select',		
									'#required' 	 => TRUE,
									'#options' 		 => $options_first,
									'#default_value' => $value_dropdown_first,
									'#prefix' 		 => '',
        							'#suffix'		 => '',
									'#attributes'    => array('class' => array('form-control')),
									'#ajax' 		 => array(
														'event' 	=> 'change',										
									   					'callback' 	=> 'inspection_monthly_rep_subdivsion_callback'
														),
									
	);
	 																		
	
	
	$options_subdiv_one = custom_user_subdivisionlist_list($form_state['values']['disname']);  
		
	$form['insmonreports']['inspection_dropdown_second'] = array(	 
	   								'#prefix' 		 => '<div id="inspection_month_subdivsion_replace">',
        							'#suffix'		 => '</div>',
									'#type' 		 => 'select',
        							'#title' 		 => 'Select Sub-division',   
         							'#options' 		 => $options_subdiv_one,
									'#attributes'    => array('class' => array('form-control')),
         							'#default_value' => isset($form_state['values']['inspection_dropdown_second']) ? $form_state['values']['inspection_dropdown_second'] :'' ,
									'#ajax' => array(
												'event' 	=> 'change',
												'callback' 	=> 'insreport_month_block_callback',
												'wrapper' 	=> 'inspection_monthly_dropdown_third_replace',
												'method' 	=> 'replace',
        									),
    ); 
	
	$options_block_one =  array(0 => "Block/Municipality") + insp_monthly_block_option($form_state['values']['inspection_dropdown_second']);
		 
	$form['insmonreports']['inspection_dropdown_third'] = array(
	     							'#prefix' 	 => '<div id="inspection_monthly_dropdown_third_replace">',
        							'#suffix' 	 => '</div>',
        							'#type' 	 => 'select',
        							'#title' 	 => 'Select Block/Municipality',
									'#attributes'    => array('class' => array('form-control')),	
									'#options'  	 => $options_block_one,
									'#default_value' => isset($form_state['values']['inspection_dropdown_third']) ? $form_state['values']['inspection_dropdown_third'] :'' ,
									
	);
	$form['insmonreports']['fodate'] = array(
				'#prefix' 	=> 	'<div class="row"><div class="form-custom col-md-6 col-sm-6 col-xs-12 form-full-label col-half-offset"><label class="input">',
				'#suffix' 	=> 	'</label></div>',
				'#title' 			=> 	t('<b>From date</b>'),
				'#type' 			=> 	'date',
				'#required'			=>	TRUE,
				'#default_value' 	=> 	array(
											'month' => format_date(time(), 'custom', 'n'),
											'day' 	=> format_date(time(), 'custom', 'j'),
											'year' 	=> format_date(time(), 'custom', 'Y')),
				'#after_build' => array('__set_year_range')
										
	); 


	$form['insmonreports']['todate'] = array(	
					'#prefix' 	=> 	'<div class="form-custom col-md-6 col-sm-6 col-xs-12 form-full-label col-half-offset"><label class="input">',
					'#suffix' 	=> 	'</label></div></div>',
					'#title' 			=> 	t('<b>To date</b>'),
					'#type' 			=> 	'date',
					'#required'			=>	TRUE,
					'#default_value' 	=> 	array(
												'month' => format_date(time(), 'custom', 'n'),
												'day' 	=> format_date(time(), 'custom', 'j'),
												'year' 	=> format_date(time(), 'custom', 'Y')),
					'#after_build' => array('__set_year_range')
	);
				
	
													
	$form['insmonreports']['ins_bt_srchbtn'] = array(     
	   							'#prefix' => '<div class="form-item form-type-select">',
       							'#suffix' => '</div></div></div></div>',
								'#attributes' => array('class' => array('labour-law-btn-pad btn-primary')),
								'#type' => 'submit', 
       							'#default_value' => 'SEARCH',
								'#ajax' => array(
											'callback' => 'annual_return_monthly_report_dis_subdis',
											'wrapper'  => 'lb_ins_month_reports_div',
											'method'   => 'html',
											'effect'   => 'fade'
										)					
	);	
	
	$form['insdiffstreports']['comrep'] = array(
		'#prefix' => '<div class="row"><div class="box box-primary"><div class="box-body"><div class="col-md-12" id="lb_ins_month_reports_div">',
		'#suffix' => '</div></div></div></div>',
	);													
														
	return $form;
}



function __set_year_range($form_element, $form_values) {
    $form_element['year']['#options'] = drupal_map_assoc(range(2014, date("Y")));
    return $form_element;
}

function annual_return_monthly_report_dis_subdis($form, $form_states) {
	 global $base_root, $base_path;
		
	$val = $form_states['values'];
	$disid = trim($val['disname']);
	$services = trim($val['services']);
	$sub_div_id = trim($val['inspection_dropdown_second']);
	$block_id = trim($val['inspection_dropdown_third']);
	$fodate = trim($val['fodate']);
	$todate = trim($val['todate']);
	$fdt = $val['fodate']['year'].'-'.$val['fodate']['month'].'-'.$val['fodate']['day'];
	$tdt = $val['todate']['year'].'-'.$val['todate']['month'].'-'.$val['todate']['day'];
	
	if(empty($disid)){
		
		return "Please select District!";
		
	}
		
	if($services==1){
		$service_name="<b>Results for P.E. Registration Under CLRA</b>";
	}elseif($services==2){
		$service_name="<b>Results for Est.Registration Under BOCWA</b>";
	}elseif($services==3){
		$service_name="<b>Results for Est.Registration Under MTW</b>";
	}elseif($services==4){
		$service_name="<b>Results for License Under The acts of CLRA</b>";
	}elseif($services==5){
		$service_name="<b>Results for Est.Registration Under ISMW</b>";
	}elseif($services==6){
		$service_name="<b>Results for Maternity Benefit Act</b>";
	}
	
	$output = '<style type="text/css">
							
							
			.tab {
				border: 1px solid #d4d4d1;
				background-color: #fff;
				float: left;
				margin-bottom: 20px;
				width: 100%;
			}
			
			.tab-content {
				width: 100%;
				padding: 20px;
				display: none;
			}
			
			#tab-1 {
			 display: block;   
			}
			#tab-3 #container444 { margin-bottom:15px; }
			#tab-3 #container444 #highcharts-3 { width:100%!important;border:1px solid #d4d4d4; }

	</style>';
	
	$output .= '<table width="100%" border="0" cellspacing="0" cellpadding="0" class="table view-act-rules-table">
					  <tr>
						<th width="6%">Sl. No</th>
						<th width="25%">NAME OF THE ESTABLISHMENT</th>
						<th>ESTABLISHMENT ADDRESS</th>
						<th width="10%">APPLY YEAR</th>
						
					  </tr>';

	
	
	
	$query_annual_return = db_select('l_annual_return_wizard_info', 'larwi');
	$query_annual_return->LeftJoin('l_annual_return_common_data', 'larcam', 'larcam.user_id=larwi.user_id');
	$query_annual_return->fields('larcam',array('est_name','establishment_address'));
	
	$counter=0;
	
	if($services==1){
		$query_annual_return->condition('larwi.clra_act',1);
	}
	if($services==2){
		$query_annual_return->condition('larwi.bocwa_act',1);
	}
	if($services==3){
		$query_annual_return->condition('larwi.mtw_act',1);
	}
	if($services==4){
		$query_annual_return->condition('larwi.license_act',1);
	}
	if($services==5){
		$query_annual_return->condition('larwi.ismw_act',1);
	}
	if($services==6){
		$query_annual_return->condition('larwi.maternity_benefit',1);
	}
	
	$query_annual_return->condition('larcam.est_district', $disid);
	if(!empty($sub_div_id)){
		$query_annual_return->condition('larcam.est_subdivision', $sub_div_id);
	}
	if(!empty($block_id)){
		$query_annual_return->condition('larcam.est_name_areatype', $block_id);
	}
	$query_annual_return->condition('larcam.final_submit_datetime', $fdt, '>=');
	$query_annual_return->condition('larcam.final_submit_datetime', $tdt, '<=');
	$query_annual_return->fields('larwi',array('current_yr'));  
	$data = $query_annual_return->execute()->fetchAll();
	if(count($data)>0){
	foreach($data as $row){
		$counter++;
		
		if(!empty($row->est_name)){	
		
			$output .= '<tr>
							<td>'.$counter.'</td>
							<td>'.$row->est_name.'</td>
							<td>'.$row->establishment_address.'</td>
							<td>'.$row->current_yr.'</td>
						  </tr>';
		}
					
		
	}
	}else{
		
		$output .= '<tr>
						<td colspan="4" align="center">No data found!</td>
						
					  </tr>';
	
	}
	
	
	
	return '<div class="col-md-12">'.$service_name.'<br/>'.$output.'</div>'; 
		
}

function get_annual_return_report_est_count($form, &$form_state){
	
	global $user, $base_root, $base_path; 
  
	
	$form['insmonreports']['header'] = array(
									'#markup' => '<div class="row"><div class="analytic-report-box analytical-bg">
													  <div class="box box-primary">	 				
													      <div class="box-body">'
	);
			
	$options_services =	array( ""=> 'Select Service', '1' => 'P.E. Registration Under CLRA', '2' => 'Est.Registration Under BOCWA', '3' => 'Est.Registration Under MTW', '4' => 'License Under The acts of CLRA','5' => 'Est.Registration Under ISMW','6' => 'Maternity Benefit Act');
	
	
												
	$value_services = isset($form_state['values']['services']) ? $form_state['values']['services'] : '';
	$options_first = common_custom_user_district_list_new()+array( "All"=> 'All'); 
  
    $value_dropdown_first = isset($form_state['values']['disname']) ? $form_state['values']['disname'] :key($options_first); 
	   
	$form['insmonreports']['boxheader'] = array(
		  						'#type' 	=> 'markup',
          						'#markup' 	=> '<div class="col-md-6"><div class="box box-primary"><div class="box-body">',
	);	   
	    
     $form['insmonreports']['disname'] = array(		
									'#title'		 => 'Select District',	   
									'#type' 		 => 'select',		
									'#required' 	 => TRUE,
									'#options' 		 => $options_first,
									'#default_value' => $value_dropdown_first,
									'#prefix' 		 => '<div class="col-md-12">',
        							'#suffix'		 => '</div>',
									'#attributes'    => array('class' => array('form-control')),
									'#ajax' 		 => array(
														'event' 	=> 'change',										
									   					'callback' 	=> 'inspection_monthly_rep_subdivsion_callback'
														),
									
	);
	 																		
	
	
	$options_subdiv_one = custom_user_subdivisionlist_list($form_state['values']['disname']);  
		
	$form['insmonreports']['inspection_dropdown_second'] = array(	 
	   								'#prefix' 		 => '<div id="inspection_month_subdivsion_replace" class="col-md-12">',
        							'#suffix'		 => '</div>',
									'#type' 		 => 'select',
        							'#title' 		 => 'Select Sub-division',    
         							'#options' 		 => $options_subdiv_one,
									'#attributes'    => array('class' => array('form-control')),
         							'#default_value' => isset($form_state['values']['inspection_dropdown_second']) ? $form_state['values']['inspection_dropdown_second'] :'' ,
									'#ajax' => array(
												'event' 	=> 'change',
												'callback' 	=> 'insreport_month_block_callback',
												'wrapper' 	=> 'inspection_monthly_dropdown_third_replace',
												'method' 	=> 'replace',
        									),
    ); 
	
	$options_block_one =  array(0 => "Block/Municipality") + insp_monthly_block_option($form_state['values']['inspection_dropdown_second']);
		 
	$form['insmonreports']['inspection_dropdown_third'] = array(
	     							'#prefix' 	 => '<div class="col-md-12" id="inspection_monthly_dropdown_third_replace">',
        							'#suffix' 	 => '</div>',
        							'#type' 	 => 'select',
        							'#title' 	 => 'Select Block/Municipality',
									'#attributes'    => array('class' => array('form-control')),	
									'#options'  	 => $options_block_one,
									'#default_value' => isset($form_state['values']['inspection_dropdown_third']) ? $form_state['values']['inspection_dropdown_third'] :'' ,
									
	);
	$form['insmonreports']['fodate'] = array(
				'#prefix' 	=> 	'<div class="form-custom col-md-6 col-sm-6 col-xs-12 form-full-label col-half-offset"><label class="input">',
				'#suffix' 	=> 	'</label></div>',
				'#title' 			=> 	t('<b>From date</b>'),
				'#type' 			=> 	'date',
				'#required'			=>	TRUE,
				'#default_value' 	=> 	array(
											'month' => format_date(time(), 'custom', 'n'),
											'day' 	=> format_date(time(), 'custom', 'j'),
											'year' 	=> format_date(time(), 'custom', 'Y')),
				'#after_build' => array('__set_year_range')
										
	); 


	$form['insmonreports']['todate'] = array(	
					'#prefix' 	=> 	'<div class="form-custom col-md-6 col-sm-6 col-xs-12 form-full-label col-half-offset"><label class="input">',
					'#suffix' 	=> 	'</label></div>',
					'#title' 			=> 	t('<b>To date</b>'),
					'#type' 			=> 	'date',
					'#required'			=>	TRUE,
					'#default_value' 	=> 	array(
												'month' => format_date(time(), 'custom', 'n'),
												'day' 	=> format_date(time(), 'custom', 'j'),
												'year' 	=> format_date(time(), 'custom', 'Y')),
					'#after_build' => array('__set_year_range')
	);
				
	
													
	$form['insmonreports']['ins_bt_srchbtn'] = array(     
	   							'#prefix' => '<div class="form-item form-type-select col-md-12">',
       							'#suffix' => '</div></div></div></div><div class="col-md-12"><div class="box box-primary"><div class="box-body">',
								'#attributes' => array('class' => array('labour-law-btn-pad btn-primary')),
								'#type' => 'button', 
       							'#default_value' => 'SEARCH',
								'#ajax' => array(
											'callback' => 'annual_return_monthly_report_est_count_div',
											'wrapper'  => 'annual_return_reports_est_count_div',
											'method'   => 'html',
											'effect'   => 'fade'
										)					
	);	
	
	$form['insdiffstreports']['comrep'] = array(
		'#prefix' => '<div class="row"><div class="box box-primary"><div class="box-body"><div class="col-md-12" id="annual_return_reports_est_count_div">',
		'#suffix' => '</div></div></div></div>',
	);													
														
	return $form;
}

function annual_return_monthly_report_est_count_div($form, $form_states) {
	global $base_root, $base_path;
		
	$val = $form_states['values'];
	$disid = trim($val['disname']);
	$sub_div_id = trim($val['inspection_dropdown_second']);
	$block_id = trim($val['inspection_dropdown_third']);
	$fodate = trim($val['fodate']);
	$todate = trim($val['todate']);
	$from_dt = $val['fodate']['year'].'-'.$val['fodate']['month'].'-'.$val['fodate']['day'];
	$to_dt = $val['todate']['year'].'-'.$val['todate']['month'].'-'.$val['todate']['day'];
		
	
	
	
	$output = '
			<table id="datatable" width="100%" border="0" cellspacing="0" cellpadding="0" class="table view-act-rules-table" >
			<thead>
				  <tr>
					<th><b>Sevices/District</b></th>
					<th width="10%">CLRA</th>
					<th width="10%">CLRA License</th>
					<th width="10%">BOCWA</th>
					<th width="10%">MTW</th>
					<th width="10%">ISWM</th>
					<th width="10%">Maternity Benefit</th>
				  </tr>
				  </thead>
				  </tbody>';
	
	
	
	if ($disid=='All'){
			$a1 = db_select('district_master', 't');
			$a1->fields('t', array('district_name','district_code'));
			$a2 	= 	$a1->execute();
			$flag=4;
	}
	else if ($disid!='All'  && $sub_div_id=='' && $block_id==0){
			$a1 = db_select('sub_division', 't');
			$a1->fields('t', array('district_name','district_code'));
			$a1->condition('t.district_code', $disid, '=');
			$a2 	= 	$a1->execute();
			$flag=1;
	}else if($disid!='All' && $sub_div_id!='' && $block_id==0){
			$flag=0;
			$a1 = db_select('sub_division', 't');
			$a1->fields('t', array('sub_div_name','sub_div_code'));
			$a1->condition('t.sub_div_code', $sub_div_id, '=');
			$a2 	= 	$a1->execute();
			
	}else if($disid!='All' && $sub_div_id!='' && $block_id!=0){
			$flag=3;
			$a1 = db_select('block_mun_master', 't');
			$a1->fields('t', array('block_mun_name'));
			$a1->condition('t.block_code', $block_id, '=');
			$a1->orderby('t.block_code');
			$a2 	= 	$a1->execute();
		
			
	}
	
	
	if($a2->rowCount()>0){
		
		
		foreach($a2 as $vl1){
			$clra = db_select('l_annual_return_wizard_info', 'larwi');
			$clra->LeftJoin('l_annual_return_common_data', 'lcpm', 'larwi.user_id=lcpm.user_id');
			$clra->fields('lcpm', array('id'));
			if($flag==1){
				$clra->condition('lcpm.est_district', $disid, '=');
			}
			if($flag==0){
				$clra->condition('lcpm.est_subdivision', $sub_div_id, '=');
			}
			if($flag==3){
				$clra->condition('lcpm.est_name_areatype', $block_id, '=');
			}
			if($flag==4){
				$clra->condition('lcpm.est_district', $vl1->district_code, '=');
			}
			$clra->condition('lcpm.final_submit_datetime', $from_dt, '>=');
			$clra->condition('lcpm.final_submit_datetime', $to_dt, '<=');
			$clra->condition('lcpm.status', '1', '=');
			$clra->condition('larwi.clra_act', '1', '=');
			
			$clra2 = 	$clra->execute();
			$clra_no	=	$clra2->rowCount();
			
			$bocwa = db_select('l_annual_return_wizard_info', 'larwi');
			$bocwa->LeftJoin('l_annual_return_common_data', 'lcpm', 'larwi.user_id=lcpm.user_id');
			$bocwa->fields('lcpm', array('id'));
			if($flag==1){
				$bocwa->condition('lcpm.est_district', $disid, '=');
			}
			if($flag==0){
				$bocwa->condition('lcpm.est_subdivision', $sub_div_id, '=');
			}
			if($flag==3){
				$bocwa->condition('lcpm.est_name_areatype', $block_id, '=');
			}
			if($flag==4){
				$bocwa->condition('lcpm.est_district', $vl1->district_code, '=');
			}
			$bocwa->condition('lcpm.final_submit_datetime', $from_dt, '>=');
			$bocwa->condition('lcpm.final_submit_datetime', $to_dt, '<=');
			$bocwa->condition('lcpm.status', '1', '=');
			$bocwa->condition('larwi.bocwa_act', '1', '=');
			
			$bocwa2 = 	$bocwa->execute();
			$bocwa_no	=	$bocwa2->rowCount();
			
			$mtw = db_select('l_annual_return_wizard_info', 'larwi');
			$mtw->LeftJoin('l_annual_return_common_data', 'lcpm', 'larwi.user_id=lcpm.user_id');
			$mtw->fields('lcpm', array('id'));
			if($flag==1){
				$mtw->condition('lcpm.est_district', $disid, '=');
			}
			if($flag==0){
				$mtw->condition('lcpm.est_subdivision', $sub_div_id, '=');
			}
			if($flag==3){
				$mtw->condition('lcpm.est_name_areatype', $block_id, '=');
			}
			if($flag==4){
				$mtw->condition('lcpm.est_district', $vl1->district_code, '=');
			}
			$mtw->condition('lcpm.final_submit_datetime', $from_dt, '>=');
			$mtw->condition('lcpm.final_submit_datetime', $to_dt, '<=');
			$mtw->condition('lcpm.status', '1', '=');
			$mtw->condition('larwi.mtw_act', '1', '=');
			
			$mtw2 = 	$mtw->execute();
			$mtw_no	=	$mtw2->rowCount();
			
			$ismw = db_select('l_annual_return_wizard_info', 'larwi');
			$ismw->LeftJoin('l_annual_return_common_data', 'lcpm', 'larwi.user_id=lcpm.user_id');
			$ismw->fields('lcpm', array('id'));
			if($flag==1){
				$ismw->condition('lcpm.est_district', $disid, '=');
			}
			if($flag==0){
				$ismw->condition('lcpm.est_subdivision', $sub_div_id, '=');
			}
			if($flag==3){
				$ismw->condition('lcpm.est_name_areatype', $block_id, '=');
			}
			if($flag==4){
				$ismw->condition('lcpm.est_district', $vl1->district_code, '=');
			}
			$ismw->condition('lcpm.final_submit_datetime', $from_dt, '>=');
			$ismw->condition('lcpm.final_submit_datetime', $to_dt, '<=');
			$ismw->condition('lcpm.status', '1', '=');
			$ismw->condition('larwi.mtw_act', '1', '=');
			
			$ismw2 = 	$ismw->execute();
			$ismw_no	=	$ismw2->rowCount();
			
			
			$clra_license = db_select('l_annual_return_wizard_info', 'larwi');
			$clra_license->LeftJoin('l_annual_return_common_data', 'lcpm', 'larwi.user_id=lcpm.user_id');
			$clra_license->fields('lcpm', array('id'));
			if($flag==1){
				$clra_license->condition('lcpm.est_district', $disid, '=');
			}
			if($flag==0){
				$clra_license->condition('lcpm.est_subdivision', $sub_div_id, '=');
			}
			if($flag==3){
				$clra_license->condition('lcpm.est_name_areatype', $block_id, '=');
			}
			if($flag==4){
				$clra_license->condition('lcpm.est_district', $vl1->district_code, '=');
			}
			$clra_license->condition('lcpm.final_submit_datetime', $from_dt, '>=');
			$clra_license->condition('lcpm.final_submit_datetime', $to_dt, '<=');
			$clra_license->condition('lcpm.status', '1', '=');
			$clra_license->condition('larwi.license_act', '1', '=');
			
			$clra_license2 = 	$clra_license->execute();
			$clra_license_no	=	$clra_license2->rowCount();
			
			$maternity_benefit = db_select('l_annual_return_wizard_info', 'larwi');
			$maternity_benefit->LeftJoin('l_annual_return_common_data', 'lcpm', 'larwi.user_id=lcpm.user_id');
			$maternity_benefit->fields('lcpm', array('id'));
			if($flag==1){
				$maternity_benefit->condition('lcpm.est_district', $disid, '=');
			}
			if($flag==0){
				$maternity_benefit->condition('lcpm.est_subdivision', $sub_div_id, '=');
			}
			if($flag==3){
				$maternity_benefit->condition('lcpm.est_name_areatype', $block_id, '=');
			}
			if($flag==4){
				$maternity_benefit->condition('lcpm.est_district', $vl1->district_code, '=');
			}
			$maternity_benefit->condition('lcpm.final_submit_datetime', $from_dt, '>=');
			$maternity_benefit->condition('lcpm.final_submit_datetime', $to_dt, '<=');
			$maternity_benefit->condition('lcpm.status', '1', '=');
			$maternity_benefit->condition('larwi.maternity_benefit', '1', '=');
			
			$maternity_benefit2 = 	$maternity_benefit->execute();
			$maternity_benefit_no	=	$maternity_benefit2->rowCount();
			
			
			
			if($flag==1 || $flag==2 || $flag==4){
				$myarr1[$vl1->district_name]	=	array('SubDivName' => $vl1->district_name,'clra_submit' =>$clra_no,'bocwa_submit'=>$bocwa_no,'mtw_submit'=>$mtw_no,'ismw_submit'=>$ismw_no,'clra_lisence_submit'=>$clra_license_no,'maternity_benefit_submit'=>$maternity_benefit_no );
			}else if($flag==0){
				$myarr1[$vl1->sub_div_name]	=	array('SubDivName' => $vl1->sub_div_name,'clra_submit' =>$clra_no,'bocwa_submit'=>$bocwa_no,'mtw_submit'=>$mtw_no,'ismw_submit'=>$ismw_no,'clra_lisence_submit'=>$clra_license_no,'maternity_benefit_submit'=>$maternity_benefit_no );
			}else if($flag==3){
				$myarr1[$vl1->block_mun_name]	=	array('SubDivName' => $vl1->block_mun_name,'clra_submit' =>$clra_no,'bocwa_submit'=>$bocwa_no,'mtw_submit'=>$mtw_no,'ismw_submit'=>$ismw_no,'clra_lisence_submit'=>$clra_license_no,'maternity_benefit_submit'=>$maternity_benefit_no );
			}
				
			
		}
	}
	foreach($myarr1 as $key=>$vl2){
							
					
		
		$district_name =$vl2['SubDivName'];
		$clra_no = $vl2['clra_submit'];
		$bocwa_no = $vl2['bocwa_submit'];
		$mtw_no = $vl2['mtw_submit'];
		$ismw_no = $vl2['ismw_submit'];
		$clra_lisence_no = $vl2['clra_lisence_submit'];
		$maternity_benefit_no=$vl2['maternity_benefit_submit'];
		
			$output .= '<tr>
						<td><b>'.$district_name.'</b></td>
						<td><b>'.$clra_no.'</b></td>
						<td><b>'.$clra_lisence_no.'</b></td>
						<td><b>'.$bocwa_no.'</b></td>
						<td><b>'.$mtw_no.'</b></td>
						<td><b>'.$ismw_no.'</b></td>
						<td><b>'.$maternity_benefit_no.'</b></td>
						
					  </tr>';
				$clra_total+=$clra_no;
				$clra_lisence_total+=$clra_lisence_no;
				$bocwa_no_total+=$bocwa_no;
				$mtw_no_total+=$mtw_no;
				$ismw_no_total+=$ismw_no;
				$maternity_benefit_no_total+=$maternity_benefit_no;	  
					  
		
		}
		
		if($disid=='All'){
		$output .='<tr>
							<td width="10%"><b>Total</b></td>';
						
		$output .='
					<td width="10%"><b>'.$clra_total.'</b></td>
					<td width="10%"><b>'.$clra_lisence_total.'</b></td>
					<td width="10%"><b>'.$bocwa_no_total.'</b></td>
					<td width="10%"><b>'.$mtw_no_total.'</b></td>
					<td width="10%"><b>'.$ismw_no_total.'</b></td>
					<td width="10%"><b>'.$maternity_benefit_no_total.'</b></td>
					';	
		
		}
			
		$output .='</tbody></table>';
	
	return '<div class="col-md-12">'.$service_name.'<br/>'.$output.'</div>'; 
		
}

