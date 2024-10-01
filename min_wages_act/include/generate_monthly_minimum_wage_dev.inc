<?php
function generate_monthly_minimum_wage_dev($form, &$form_state){
	$mnw_fix_neut_initial_value = db_select('l_mnw_fixation_neutralization_initial_fixed_point_master','l')-> fields('l', array())-> condition('l.is_active',1)->execute()->fetchAssoc();
	
	$sum_cpi_zone_A = 0;
	$sum_cpi_zone_B = 0;
		
	$current_month = date('n',time());  //6; 7;
	$current_year =  date('Y',time()); //2019;2019;
		
		if(($current_month >= 1) && ($current_month <= 6)){
			/*** MNW for July Session (Previus(Oct,Nov,Dec),Jan,Feb,Mar) --------- Therefore MNW should generate from March to June ***/
			$month_sl_1 = array(1,2,3);
			$month_sl_2 = array(10,11,12);
			
			$db_or = db_or();
			$db_or->condition(
						db_and()->condition('l.cpiyear',$current_year-1)->condition('l.month_srl', ($month_sl_2), 'IN')
					);
			$db_or->condition(
						db_and()->condition('l.cpiyear',$current_year)->condition('l.month_srl', ($month_sl_1), 'IN')
					);
			
			$cpi_value_pre_Oct_Dec = db_select('l_mnw_cpi_number_master','l');
			$cpi_value_pre_Oct_Dec->fields('l', array());
			$cpi_value_pre_Oct_Dec->condition('l.is_active',1);
			$cpi_value_pre_Oct_Dec->condition($db_or);
			$cpi_value_pre_Oct_Dec = $cpi_value_pre_Oct_Dec->execute(); 
			
			foreach ($cpi_value_pre_Oct_Dec as $data) {
 				$sum_cpi_zone_A = $sum_cpi_zone_A + $data->iwvalue;
 				$sum_cpi_zone_B = $sum_cpi_zone_B + $data->alvalue;
			}
			
		}else{
			/*** MNW for Janaury Session (Apr, May, June, July, Aug, Sept)  --------- Therefore MNW should generate from July to Dec ***/
			$session = '1';
			$year = $current_year + 1;
			
			$month_sl = array(4,5,6,7,8,9);
			
			$cpi_value_Apr_Sept_Zone_A = db_select('l_mnw_cpi_number_master','l');
			$cpi_value_Apr_Sept_Zone_A->fields('l', array());
			$cpi_value_Apr_Sept_Zone_A->condition('l.is_active',1);
			$cpi_value_Apr_Sept_Zone_A->condition('l.cpiyear',$current_year);
			$cpi_value_Apr_Sept_Zone_A = $cpi_value_Apr_Sept_Zone_A->condition('l.month_srl', ($month_sl), 'IN')->execute(); 
			
			foreach ($cpi_value_Apr_Sept_Zone_A as $data) {
 				$sum_cpi_zone_A = $sum_cpi_zone_A + $data->iwvalue;
				$sum_cpi_zone_B = $sum_cpi_zone_B + $data->alvalue;
			}
		}

	/*** Avg CPI of 6 months ***/ 	
	$avg_cpi_value_Zone_A = round($sum_cpi_zone_A / 6);
	$avg_cpi_value_Zone_B = round($sum_cpi_zone_B / 6); 
	
	/**** FIxation Point ****/
	$fixation_value_IW_Zone_A_new = $mnw_fix_neut_initial_value['fix_iw_a_value'];
	$fixation_value_IW_Zone_B_new = $mnw_fix_neut_initial_value['fix_iw_b_value'];
	
	/**** Rise in CPI ****/
	$rise_cpi_zone_A = $avg_cpi_value_Zone_A - $mnw_fix_neut_initial_value['fix_iw_a_value']; 
	$rise_cpi_zone_B = $avg_cpi_value_Zone_B - $mnw_fix_neut_initial_value['fix_iw_b_value']; 
		
		
	$mw_query1 = db_select('l_min_wages_schedule_employment_master', 'mw');	
	$mw_query1->fields('mw',array());
	$mw_query1->condition('mw.is_active', 1, '=');	
	$mw_query1->orderby('mw.node_type_name', 'asc');	
	$mw_result_table1 = $mw_query1->condition('mw.table_category', 1, '=')->execute(); 
	
	if($mw_result_table1->rowCount() > 0){
		$count = 1;
		$mnwlist_table1 = '';
		foreach($mw_result_table1 as $data1){
			$mnwlist_table1 .='<tr><td width="6%">'.$count++.'</td><td>'.strtoupper($data1->node_type_name).'</td></tr>';
		}
	}	
	$result1 = $mw_query1->condition('mw.table_category', 1, '=')->execute()->fetchCol('id');
	
	$mw_query2 = db_select('l_min_wages_schedule_employment_master', 'mw');	
	$mw_query2->fields('mw',array());
	$mw_query2->condition('mw.is_active', 1, '=');	
	$mw_query2->orderby('mw.node_type_name', 'asc');	
	$mw_result_table2 = $mw_query2->condition('mw.table_category', 2, '=')->execute(); 
	

	if($mw_result_table2->rowCount() > 0){
		$count = 1;
		$mnwlist_table2 = '';
		foreach($mw_result_table2 as $data2){
			$mnwlist_table2 .='<tr><td width="6%">'.$count++.'</td><td>'.strtoupper($data2->node_type_name).'</td></tr>';
		}
	}	
	$result2 = $mw_query2->condition('mw.table_category', 2, '=')->execute()->fetchCol('id');
	
	$mw_query3 = db_select('l_min_wages_schedule_employment_master', 'mw');	
	$mw_query3->fields('mw',array());
	$mw_query3->condition('mw.is_active', 1, '=');	
	$mw_query3->orderby('mw.node_type_name', 'asc');	
	$mw_result_table3 = $mw_query3->condition('mw.table_category', 3, '=')->execute(); 
	

	if($mw_result_table3->rowCount() > 0){
		$count = 1;
		$mnwlist_table3 = '';
		foreach($mw_result_table3 as $data3){
			$mnwlist_table3 .='<tr><td width="6%">'.$count++.'</td><td>'.strtoupper($data3->node_type_name).'</td></tr>';
		} 
	}	
	$result3 = $mw_query3->condition('mw.table_category', 3, '=')->execute()->fetchCol('id');
	
	$form['min_wage']['year'] = array(
		'#type'=>'hidden',
		'#value'=>$current_year,
		);
	$form['min_wage']['month'] = array(
		'#type'=>'hidden',
		'#value'=>$current_month,
		);	
	$form['min_wage']['emp_id_table1'] = array(
		'#type'=>'hidden',
		'#value'=>$result1,
		);
	$form['min_wage']['emp_id_table2'] = array(
		'#type'=>'hidden',
		'#value'=>$result2,
		);	
	$form['min_wage']['emp_id_table3'] = array(
		'#type'=>'hidden',
		'#value'=>$result3,
		);		
	
	$form['min_wage']['table_1_info'] = array( 	
		'#prefix'=>'<div class="box box-primary box-solid"><div class="box-header">Information Of Table - 1</div>
					<div class="box-body feedback scroll">
					<table width="100%" class="table table-bordered">
						<thead>
							<tr>
								<th>Sl. No.</th>
								<th>Parameter (Zone A)</th>
								<th>Values (Zone A)</th>
								<th>Values (Zone B)</th>
							</tr>
						</thead>
							<tbody><tr>
								<td>1. </td>
								<td>Fixation Point</td>
								<td>'.$mnw_fix_neut_initial_value['fix_iw_a_value'].' Note: As declared.</td>
								<td>'.$mnw_fix_neut_initial_value['fix_iw_b_value'].' Note: As declared.</td>
							</tr>
							<tr>
								<td>2. </td>
								<td>Average CPI value </td>
								<td>'.$avg_cpi_value_Zone_A.'</td>
								<td>'.$avg_cpi_value_Zone_B.'</td>
							</tr>
							<tr>
								<td>3. </td>
								<td>Rise in CPI</td>
								<td>'.$rise_cpi_zone_A.'</td>
								<td>'.$rise_cpi_zone_B.'</td>

							</tr>
						</tbody>
					</table>
					<table class="table table-bordered">
						<tbody>
							<tr>
								<th colspan="8">Factors of Category wise Minimum wages</th>
							</tr>
							<tr>
								<th rowspan="2" valign="middle">Category </th>
								<th colspan="2" valign="middle">Initially Fixed Rate</th>
								<th colspan="2" valign="middle">Calculated CPI Rate</th>
								<th colspan="2" align="center" valign="middle">Minimum wages</th>
							</tr>
							<tr>
								<th>Zone A</th>
								<th>Zone B</th>
								<th>Zone A</th>
								<th>Zone B</th>
								<th>Zone A</th>
								<th>Zone B</th>
							</tr>
							<tr>
								<td>Unskilled</td>
								<td>'.number_format($mnw_fix_neut_initial_value['initial_iw_a_u_value'], 2).'</td>
								<td>'.number_format($mnw_fix_neut_initial_value['initial_iw_b_u_value'], 2).'</td>
								<td>'.$mnw_fix_neut_initial_value['neut_iw_a_u_value'].' X ('.$avg_cpi_value_Zone_A.'-'.$mnw_fix_neut_initial_value['fix_iw_a_value'].') = '.($mnw_fix_neut_initial_value['neut_iw_a_u_value']*$rise_cpi_zone_A).'</td>
								<td>'.$mnw_fix_neut_initial_value['neut_iw_b_u_value'].'X ('.$avg_cpi_value_Zone_B.'-'.$mnw_fix_neut_initial_value['fix_iw_b_value'].') = '.$mnw_fix_neut_initial_value['neut_iw_b_u_value']*$rise_cpi_zone_B.'</td>
								<td>'.number_format(($mnw_fix_neut_initial_value['initial_iw_a_u_value'] + ($mnw_fix_neut_initial_value['neut_iw_a_u_value']*$rise_cpi_zone_A)), 2).'</td>
								<td>'.number_format(($mnw_fix_neut_initial_value['initial_iw_b_u_value'] + ($mnw_fix_neut_initial_value['neut_iw_b_u_value']*$rise_cpi_zone_B)), 2).'</td>
							</tr>
							<tr>
								<td>Semi-Skilled</td>
								<td>'.number_format($mnw_fix_neut_initial_value['initial_iw_a_sm_value'], 2).'</td>
								<td>'.number_format($mnw_fix_neut_initial_value['initial_iw_b_sm_value'], 2).'</td>
								<td>'.$mnw_fix_neut_initial_value['neut_iw_a_sm_value'].' X ('.$avg_cpi_value_Zone_A.'-'.$mnw_fix_neut_initial_value['fix_iw_a_value'].') = '.($mnw_fix_neut_initial_value['neut_iw_a_sm_value']*$rise_cpi_zone_A).'</td>
								<td>'.$mnw_fix_neut_initial_value['neut_iw_b_sm_value'].' X ('.$avg_cpi_value_Zone_B.'-'.$mnw_fix_neut_initial_value['fix_iw_b_value'].') = '.($mnw_fix_neut_initial_value['neut_iw_b_sm_value']*$rise_cpi_zone_B).'</td>
								<td>'.number_format(($mnw_fix_neut_initial_value['initial_iw_a_sm_value'] + ($mnw_fix_neut_initial_value['neut_iw_a_sm_value']*$rise_cpi_zone_A)), 2).'</td>
								<td>'.number_format(($mnw_fix_neut_initial_value['initial_iw_b_sm_value'] + ($mnw_fix_neut_initial_value['neut_iw_b_sm_value']*$rise_cpi_zone_B)), 2).'</td>
							</tr>
							<tr>
								<td>Skilled</td>
								<td>'.number_format($mnw_fix_neut_initial_value['initial_iw_a_s_value'], 2).'</td>
								<td>'.number_format($mnw_fix_neut_initial_value['initial_iw_b_s_value'], 2).'</td>
								<td>'.$mnw_fix_neut_initial_value['neut_iw_a_s_value'].' X ('.$avg_cpi_value_Zone_A.'-'.$mnw_fix_neut_initial_value['fix_iw_a_value'].') = '.($mnw_fix_neut_initial_value['neut_iw_a_s_value']*$rise_cpi_zone_A).'</td>
								<td>'.$mnw_fix_neut_initial_value['neut_iw_b_s_value'].' X ('.$avg_cpi_value_Zone_B.'-'.$mnw_fix_neut_initial_value['fix_iw_b_value'].') = '.($mnw_fix_neut_initial_value['neut_iw_b_s_value']*$rise_cpi_zone_B).'</td>
								<td>'.number_format(($mnw_fix_neut_initial_value['initial_iw_a_s_value'] + ($mnw_fix_neut_initial_value['neut_iw_a_s_value']*$rise_cpi_zone_A)), 2).'</td>
								<td>'.number_format(($mnw_fix_neut_initial_value['initial_iw_b_s_value'] + ($mnw_fix_neut_initial_value['neut_iw_b_s_value']*$rise_cpi_zone_B)), 2).'</td>
							</tr>
							<tr>
								<td>Highly Skilled </td>
								<td>'.number_format($mnw_fix_neut_initial_value['initial_iw_a_h_value'], 2).'</td>
								<td>'.number_format($mnw_fix_neut_initial_value['initial_iw_b_h_value'], 2).'</td>
								<td>'.$mnw_fix_neut_initial_value['neut_iw_a_h_value'].' X ('.$avg_cpi_value_Zone_A.'-'.$mnw_fix_neut_initial_value['fix_iw_a_value'].') = '.($mnw_fix_neut_initial_value['neut_iw_a_h_value']*$rise_cpi_zone_A).'</td>
								<td>'.$mnw_fix_neut_initial_value['neut_iw_b_h_value'].' X ('.$avg_cpi_value_Zone_B.'-'.$mnw_fix_neut_initial_value['fix_iw_b_value'].') = '.($mnw_fix_neut_initial_value['neut_iw_b_h_value']*$rise_cpi_zone_B).'</td>
								<td>'.number_format(($mnw_fix_neut_initial_value['initial_iw_a_h_value'] + ($mnw_fix_neut_initial_value['neut_iw_a_h_value']*$rise_cpi_zone_A)), 2).'</td>
								<td>'.number_format(($mnw_fix_neut_initial_value['initial_iw_b_h_value'] + ($mnw_fix_neut_initial_value['neut_iw_b_h_value']*$rise_cpi_zone_B)), 2).'</td>
							</tr>							
						</tbody>
					</table>
					</div></div>'
					);
					
	$form['min_wage']['header_table1'] = array( 	
			'#prefix' =>'<div class="box box-primary box-solid" style="overflow:auto; overflow-y:scroll; max-height:400px;"><div class="box-body feedback scroll">
						<table class="table table-bordered table-fixed table-striped" width="100%">
							<thead>	<tr><th> Table 1</th></tr></thead>'
							);
							
	$form['min_wage']['select_table1'] = array(
			'#prefix' => '<tr><td>',
			'#suffix' => '</td></tr>',
			'#title' => 'Select All Employment in Table 1',
			'#type' => 'checkbox',
			);
							
							
	$form['min_wage']['header_table1_markup'] = array(
			'#prefix' => '<tr>
							<td>
							<table class="table table-bordered table-fixed table-striped">
								<thead>
									<tr>
										<th width="6%">Sl. No.</th>
										<th>Title</th>
									</tr></thead><tbody>'.$mnwlist_table1.'</tbody>
							</table>
							</td>
						<tr>
					</table>
					</div></div>'
					);
	
	/**** Rise in CPI ****/
	$rise_old_cpi_zone_A = $avg_cpi_value_Zone_A - $mnw_fix_neut_initial_value['fix_old_iw_a_value']; 
	$rise_old_cpi_zone_B = $avg_cpi_value_Zone_B - $mnw_fix_neut_initial_value['fix_old_iw_b_value']; 
					
	$form['min_wage']['table_2_info'] = array( 	
		'#prefix'=>'<div class="box box-primary box-solid"><div class="box-header">Information Of Table - 2</div>
					<div class="box-body feedback scroll">
					<table width="100%" class="table table-bordered">
						<thead>
							<tr>
								<th>Sl. No.</th>
								<th>Parameter (Zone A)</th>
								<th>Values (Zone A)</th>
								<th>Values (Zone B)</th>
							</tr>
						</thead>
							<tbody><tr>
								<td>1. </td>
								<td>Fixation Point</td>
								<td>'. $mnw_fix_neut_initial_value['fix_old_iw_a_value'].' Note: As declared.</td>
								<td>'. $mnw_fix_neut_initial_value['fix_old_iw_b_value'].' Note: As declared.</td>
							</tr>
							<tr>
								<td>2. </td>
								<td>Average CPI value </td>
								<td>'.$avg_cpi_value_Zone_A.'</td>
								<td>'.$avg_cpi_value_Zone_B.'</td>
							</tr>
							<tr>
								<td>3. </td>
								<td>Rise in CPI</td>
								<td>'.$rise_old_cpi_zone_A.'</td>
								<td>'.$rise_old_cpi_zone_B.'</td>
							</tr>
						</tbody>
					</table>
					<table class="table table-bordered">
						<tbody>
							<tr>
								<th colspan="8">Factors of Category wise Minimum wages</th>
							</tr>
							<tr>
								<th rowspan="2" valign="middle">Category </th>
								<th colspan="2" valign="middle">Initially Fixed Rate</th>
								<th colspan="2" valign="middle">Calculated CPI Rate</th>
								<th colspan="2" align="center" valign="middle">Minimum wages</th>
							</tr>
							<tr>
								<th>Zone A</th>
								<th>Zone B</th>
								<th>Zone A</th>
								<th>Zone B</th>
								<th>Zone A</th>
								<th>Zone B</th>
							</tr>
							<tr>
								<td>Unskilled</td>
								<td>'.number_format($mnw_fix_neut_initial_value['initial_old_iw_a_u_value'], 2).'</td>
								<td>'.number_format($mnw_fix_neut_initial_value['initial_old_iw_b_u_value'], 2).'</td>
								<td>'.$mnw_fix_neut_initial_value['neut_old_iw_a_u_value'].' X ('.$avg_cpi_value_Zone_A.'-'.$mnw_fix_neut_initial_value['fix_old_iw_a_value'].') = '.($mnw_fix_neut_initial_value['neut_old_iw_a_u_value']*$rise_old_cpi_zone_A).'</td>
								<td>'.$mnw_fix_neut_initial_value['neut_old_iw_b_u_value'].' X ('.$avg_cpi_value_Zone_B.'-'.$mnw_fix_neut_initial_value['fix_old_iw_b_value'].') = '.($mnw_fix_neut_initial_value['neut_old_iw_b_u_value']*$rise_old_cpi_zone_B).'</td>
								<td>'.number_format(($mnw_fix_neut_initial_value['initial_old_iw_a_u_value'] + ($mnw_fix_neut_initial_value['neut_old_iw_a_u_value']*$rise_old_cpi_zone_A)), 2).'</td>
								<td>'.number_format(($mnw_fix_neut_initial_value['initial_old_iw_b_u_value'] + ($mnw_fix_neut_initial_value['neut_old_iw_b_u_value']*$rise_old_cpi_zone_B)), 2).'</td>
							</tr>
							<tr>
								<td>Semi-Skilled</td>
								<td>'.number_format($mnw_fix_neut_initial_value['initial_old_iw_a_sm_value'], 2).'</td>
								<td>'.number_format($mnw_fix_neut_initial_value['initial_old_iw_b_sm_value'], 2).'</td>
								<td>'.$mnw_fix_neut_initial_value['neut_old_iw_a_sm_value'].' X ('.$avg_cpi_value_Zone_A.'-'.$mnw_fix_neut_initial_value['fix_old_iw_a_value'].') = '.($mnw_fix_neut_initial_value['neut_old_iw_a_sm_value']*$rise_old_cpi_zone_A).'</td>
								<td>'.$mnw_fix_neut_initial_value['neut_old_iw_b_sm_value'].' X ('.$avg_cpi_value_Zone_B.'-'.$mnw_fix_neut_initial_value['fix_old_iw_b_value'].') = '.($mnw_fix_neut_initial_value['neut_old_iw_b_sm_value']*$rise_old_cpi_zone_B).'</td>
								<td>'.number_format(($mnw_fix_neut_initial_value['initial_old_iw_a_sm_value'] + ($mnw_fix_neut_initial_value['neut_old_iw_a_sm_value']*$rise_old_cpi_zone_A)), 2).'</td>
								<td>'.number_format(($mnw_fix_neut_initial_value['initial_old_iw_b_sm_value'] + ($mnw_fix_neut_initial_value['neut_old_iw_b_sm_value']*$rise_old_cpi_zone_B)), 2).'</td>
							</tr>
							<tr>
								<td>Skilled</td>
								<td>'.number_format($mnw_fix_neut_initial_value['initial_old_iw_a_s_value'], 2).'</td>
								<td>'.number_format($mnw_fix_neut_initial_value['initial_old_iw_b_s_value'], 2).'</td>
								<td>'.$mnw_fix_neut_initial_value['neut_old_iw_a_s_value'].' X ('.$avg_cpi_value_Zone_A.'-'.$mnw_fix_neut_initial_value['fix_old_iw_a_value'].') = '.($mnw_fix_neut_initial_value['neut_old_iw_a_s_value']*$rise_old_cpi_zone_A).'</td>
								<td>'.$mnw_fix_neut_initial_value['neut_old_iw_b_s_value'].' X ('.$avg_cpi_value_Zone_B.'-'.$mnw_fix_neut_initial_value['fix_old_iw_b_value'].') = '.($mnw_fix_neut_initial_value['neut_old_iw_b_s_value']*$rise_old_cpi_zone_B).'</td>
								<td>'.number_format(($mnw_fix_neut_initial_value['initial_old_iw_a_s_value'] + ($mnw_fix_neut_initial_value['neut_old_iw_a_s_value']*$rise_old_cpi_zone_A)), 2).'</td>
								<td>'.number_format(($mnw_fix_neut_initial_value['initial_old_iw_b_s_value'] + ($mnw_fix_neut_initial_value['neut_old_iw_b_s_value']*$rise_old_cpi_zone_B)), 2).'</td>
							</tr>
							<tr>
								<td>Highly Skilled </td>
								<td>'.number_format($mnw_fix_neut_initial_value['initial_old_iw_a_h_value'], 2).'</td>
								<td>'.number_format($mnw_fix_neut_initial_value['initial_old_iw_b_h_value'], 2).'</td>
								<td>'.$mnw_fix_neut_initial_value['neut_old_iw_a_h_value'].' X ('.$avg_cpi_value_Zone_A.'-'.$mnw_fix_neut_initial_value['fix_old_iw_a_value'].') = '.($mnw_fix_neut_initial_value['neut_old_iw_a_h_value']*$rise_old_cpi_zone_A).'</td>
								<td>'.$mnw_fix_neut_initial_value['neut_old_iw_b_h_value'].' X ('.$avg_cpi_value_Zone_B.'-'.$mnw_fix_neut_initial_value['fix_old_iw_b_value'].') = '.($mnw_fix_neut_initial_value['neut_old_iw_b_h_value']*$rise_old_cpi_zone_B).'</td>
								<td>'.number_format(($mnw_fix_neut_initial_value['initial_old_iw_a_h_value'] + ($mnw_fix_neut_initial_value['neut_old_iw_a_h_value']*$rise_old_cpi_zone_A)), 2).'</td>
								<td>'.number_format(($mnw_fix_neut_initial_value['initial_old_iw_b_h_value'] + ($mnw_fix_neut_initial_value['neut_old_iw_b_h_value']*$rise_old_cpi_zone_B)), 2).'</td>
							</tr>							
						</tbody>
					</table>
					</div></div>'
					);
									
	$form['min_wage']['header_table2'] = array(
			'#prefix' => '<div class="box box-primary box-solid" style="overflow:auto; overflow-y:scroll; max-height:400px;"><div class="box-body feedback scroll">
							<table class="table table-bordered table-fixed table-striped" width="100%">
							<thead>	<tr><th> Table 2</th></tr></thead>'
							);
	$form['min_wage']['select_table2'] = array(
			'#prefix' => '<tr><td>',
			'#suffix' => '</td></tr>',
			'#title' => 'Select All Employment in Table 2',
			'#type' => 'checkbox',
			);	
	$form['min_wage']['header_table2_markup'] = array(
			'#prefix' => '<tr>
							<td>
							<table class="table table-bordered table-fixed table-striped">
								<thead>
									<tr>
										<th width="6%">Sl. No.</th>
										<th>Title</th>
									</tr></thead><tbody>'.$mnwlist_table2.'</tbody></table></td><tr></table></div></div>'
		);	
		
		
	$avg_cpi_value_AL = $avg_cpi_value_Zone_B;
	$rise_cpi_zone_AL = $avg_cpi_value_AL - $mnw_fix_neut_initial_value['fix_al_value'];
	
	$form['min_wage']['table_3_info'] = array( 	
		'#prefix'=>'<div class="box box-primary box-solid"><div class="box-header">Information Of Table - 3 ( Agriculture )</div>
					<div class="box-body feedback scroll">
					<table width="100%" class="table table-bordered">
						<thead>
							<tr>
								<th>Sl. No.</th>
								<th>Parameter (Zone A)</th>
								<th>Values (Zone A)</th>
							</tr>
						</thead>
							<tbody><tr>
								<td>1. </td>
								<td>Fixation Point</td>
								<td>'. $mnw_fix_neut_initial_value['fix_al_value'].' Note: As declared.</td>
							</tr>
							<tr>
								<td>2. </td>
								<td>Average CPI value </td>
								<td>'.$avg_cpi_value_AL.'</td>
							</tr>
							<tr>
								<td>3. </td>
								<td>Rise in CPI</td>
								<td>('.$avg_cpi_value_AL.'-'.$mnw_fix_neut_initial_value['fix_al_value'].') = '.$rise_cpi_zone_AL.'</td>
							</tr>
						</tbody>
					</table>
					<table class="table table-bordered">
						<tbody>
							<tr>
								<th colspan="8">Factors of Category wise Minimum wages</th>
							</tr>
							<tr>
								<th valign="middle">Category </th>
								<th valign="middle">Initially Fixed Rate</th>
								<th valign="middle">Calculated CPI Rate</th>
								<th align="center" valign="middle">Minimum wages</th>
							</tr>
							
							<tr>
								<td>Unskilled</td>
								<td>'.number_format($mnw_fix_neut_initial_value['initial_al_u_value'], 2).'</td>
								<td>'.$mnw_fix_neut_initial_value['neut_al_u_value'].' X ('.$avg_cpi_value_AL.'-'.$mnw_fix_neut_initial_value['fix_al_value'].') = '.($mnw_fix_neut_initial_value['neut_al_u_value']*$rise_cpi_zone_AL).'</td>
								<td>'.number_format(($mnw_fix_neut_initial_value['initial_al_u_value'] + ($mnw_fix_neut_initial_value['neut_al_u_value']*$rise_cpi_zone_AL)), 2).'</td>
							</tr>
							<tr>
								<td>Semi-Skilled</td>
								<td>'.number_format($mnw_fix_neut_initial_value['initial_al_sm_value'], 2).'</td>
								<td>'.$mnw_fix_neut_initial_value['neut_al_sm_value'].' X ('.$avg_cpi_value_AL.'-'.$mnw_fix_neut_initial_value['fix_al_value'].') = '.($mnw_fix_neut_initial_value['neut_al_sm_value']*$rise_cpi_zone_AL).'</td>
								<td>'.number_format(($mnw_fix_neut_initial_value['initial_al_sm_value'] + ($mnw_fix_neut_initial_value['neut_al_sm_value']*$rise_cpi_zone_AL)), 2).'</td>
							</tr>
							<tr>
								<td>Skilled</td>
								<td>'.number_format($mnw_fix_neut_initial_value['initial_al_s_value'], 2).'</td>
								<td>'.$mnw_fix_neut_initial_value['neut_al_s_value'].' X ('.$avg_cpi_value_AL.'-'.$mnw_fix_neut_initial_value['fix_al_value'].') = '.($mnw_fix_neut_initial_value['neut_al_s_value']*$rise_cpi_zone_AL).'</td>
								
								<td>'.number_format(($mnw_fix_neut_initial_value['initial_al_s_value'] + ($mnw_fix_neut_initial_value['neut_al_s_value']*$rise_cpi_zone_AL)), 2).'</td>
							</tr>
							<tr>
								<td>Highly Skilled </td>
								<td>'.number_format($mnw_fix_neut_initial_value['initial_al_h_value'], 2).'</td>
								<td>'.$mnw_fix_neut_initial_value['neut_al_h_value'].' X ('.$avg_cpi_value_AL.'-'.$mnw_fix_neut_initial_value['fix_al_value'].') = '.($mnw_fix_neut_initial_value['neut_al_h_value']*$rise_cpi_zone_AL).'</td>
								<td>'.number_format(($mnw_fix_neut_initial_value['initial_al_h_value'] + ($mnw_fix_neut_initial_value['neut_al_h_value']*$rise_cpi_zone_AL)), 2).'</td>
							</tr>							
						</tbody>
					</table>
					</div></div>'
					);
						
	$form['min_wage']['header_table3'] = array(
			'#prefix' => '<div class="box box-primary box-solid" style="overflow:auto; overflow-y:scroll; max-height:400px;"><div class="box-body feedback scroll">
							<table class="table table-bordered table-fixed table-striped" width="100%">
							<thead>	<tr><th> Table 3</th></tr></thead>'
							);
	$form['min_wage']['select_table3'] = array(
			'#prefix' => '<tr><td>',
			'#suffix' => '</td></tr>',
			'#title' => 'Select All Employment in Table 3',
			'#type' => 'checkbox',
			);	
	$form['min_wage']['header_table3_markup'] = array(
			'#prefix' => '<tr>
							<td>
							<table class="table table-bordered table-fixed table-striped">
								<thead>
									<tr>
										<th width="6%">Sl. No.</th>
										<th>Title</th>
									</tr></thead><tbody>'.$mnwlist_table3.'</tbody></table></td><tr></table></div></div>'
		);					
		
	$form['min_wage']['submit'] = array(
		'#type'=>'submit',
		'#value'=>'Generate',
		'#attributes'=>array('class' => array('btn btn-primary')),
		); 
		
	return $form;
}

function generate_monthly_minimum_wage_dev_submit($form, &$form_state){ 
	$val = $form_state['values']; 
	$mnw_fix_neut_initial_value = db_select('l_mnw_fixation_neutralization_initial_fixed_point_master','l')-> fields('l', array())-> condition('l.is_active',1)->execute()->fetchAssoc();
	
	$sum_cpi_zone_A = 0;
	$sum_cpi_zone_B = 0;
		
	$current_month = $val['month']; // 6;7;
	$current_year = $val['year'];//2019;2019;
		
		if(($current_month >= 1) && ($current_month <= 6)){
			/*** MNW for July Session (Previus(Oct,Nov,Dec),Jan,Feb,Mar) --------- Therefore MNW should generate from March to June ***/
			$session = '2';
			$year = $current_year;
			
			$month_sl_1 = array(1,2,3);
			$month_sl_2 = array(10,11,12);
			
			$db_or = db_or();
			$db_or->condition(
						db_and()->condition('l.cpiyear',$current_year-1)->condition('l.month_srl', ($month_sl_2), 'IN')
					);
			$db_or->condition(
						db_and()->condition('l.cpiyear',$current_year)->condition('l.month_srl', ($month_sl_1), 'IN')
					);
			
			$cpi_value_pre_Oct_Dec = db_select('l_mnw_cpi_number_master','l');
			$cpi_value_pre_Oct_Dec->fields('l', array());
			$cpi_value_pre_Oct_Dec->condition('l.is_active',1);
			$cpi_value_pre_Oct_Dec->condition($db_or);
			$cpi_value_pre_Oct_Dec = $cpi_value_pre_Oct_Dec->execute(); 
			
			foreach ($cpi_value_pre_Oct_Dec as $data) {
 				$sum_cpi_zone_A = $sum_cpi_zone_A + $data->iwvalue;
 				$sum_cpi_zone_B = $sum_cpi_zone_B + $data->alvalue;
			}
			
		}else{
			/*** MNW for Janaury Session (Apr, May, June, July, Aug, Sept)  --------- Therefore MNW should generate from July to Dec ***/
			$session = '1';
			$year = $current_year + 1;
			
			$month_sl = array(4,5,6,7,8,9);
			
			$cpi_value_Apr_Sept_Zone_A = db_select('l_mnw_cpi_number_master','l');
			$cpi_value_Apr_Sept_Zone_A->fields('l', array());
			$cpi_value_Apr_Sept_Zone_A->condition('l.is_active',1);
			$cpi_value_Apr_Sept_Zone_A->condition('l.cpiyear',$current_year);
			$cpi_value_Apr_Sept_Zone_A = $cpi_value_Apr_Sept_Zone_A->condition('l.month_srl', ($month_sl), 'IN')->execute(); 
			
			foreach ($cpi_value_Apr_Sept_Zone_A as $data) {
 				$sum_cpi_zone_A = $sum_cpi_zone_A + $data->iwvalue;
				$sum_cpi_zone_B = $sum_cpi_zone_B + $data->alvalue;
			}
		}

		/*** Avg CPI of 6 months ***/ 	
		$avg_cpi_value_Zone_A = round($sum_cpi_zone_A / 6); 
		$avg_cpi_value_Zone_B = round($sum_cpi_zone_B / 6); 
		
	if($val['select_table1']){
		
		/**** FIxation Point ****/
		$fixation_value_IW_Zone_A = $mnw_fix_neut_initial_value['fix_iw_a_value'];
		$fixation_value_IW_Zone_B = $mnw_fix_neut_initial_value['fix_iw_b_value'];
		
		
		/**** Rise in CPI ****/
		$rise_cpi_zone_A = $avg_cpi_value_Zone_A - $fixation_value_IW_Zone_A; 
		$rise_cpi_zone_B = $avg_cpi_value_Zone_B - $fixation_value_IW_Zone_B; 
		
		/**** Neutralization Point ****/
		$neutralization_point_unskill_zoneA = $mnw_fix_neut_initial_value['neut_iw_a_u_value'];
		$neutralization_point_semiskill_zoneA = $mnw_fix_neut_initial_value['neut_iw_a_sm_value'];
		$neutralization_point_skill_zoneA = $mnw_fix_neut_initial_value['neut_iw_a_s_value'];
		$neutralization_point_highskill_zoneA = $mnw_fix_neut_initial_value['neut_iw_a_h_value'];
		
		$neutralization_point_unskill_zoneB = $mnw_fix_neut_initial_value['neut_iw_b_u_value'];
		$neutralization_point_semiskill_zoneB = $mnw_fix_neut_initial_value['neut_iw_b_sm_value'];
		$neutralization_point_skill_zoneB = $mnw_fix_neut_initial_value['neut_iw_b_s_value'];
		$neutralization_point_highskill_zoneB = $mnw_fix_neut_initial_value['neut_iw_b_h_value'];
		
		/**** Initial Fixed Value ****/
		
		$initial_fixed_minwage_unskill_zoneA = $mnw_fix_neut_initial_value['initial_iw_a_u_value'];
		$initial_fixed_minwage_semiskill_zoneA = $mnw_fix_neut_initial_value['initial_iw_a_sm_value'];
		$initial_fixed_minwage_skill_zoneA = $mnw_fix_neut_initial_value['initial_iw_a_s_value'];
		$initial_fixed_minwage_highskill_zoneA = $mnw_fix_neut_initial_value['initial_iw_a_h_value'];
		
		$initial_fixed_minwage_unskill_zoneB = $mnw_fix_neut_initial_value['initial_iw_b_u_value'];
		$initial_fixed_minwage_semiskill_zoneB = $mnw_fix_neut_initial_value['initial_iw_b_sm_value'];
		$initial_fixed_minwage_skill_zoneB = $mnw_fix_neut_initial_value['initial_iw_b_s_value'];
		$initial_fixed_minwage_highskill_zoneB = $mnw_fix_neut_initial_value['initial_iw_b_h_value'];
		
		/*** Minimum Wage Worker & Zone Wise ***/
		$unskilled_wage_A = ($initial_fixed_minwage_unskill_zoneA + ($neutralization_point_unskill_zoneA*$rise_cpi_zone_A));
		$semiskilled_wage_A = ($initial_fixed_minwage_semiskill_zoneA + ($neutralization_point_semiskill_zoneA*$rise_cpi_zone_A)); 
	 	$skilled_wage_A = ($initial_fixed_minwage_skill_zoneA + ($neutralization_point_skill_zoneA*$rise_cpi_zone_A)); 
	 	$highskilled_wage_A = ($initial_fixed_minwage_highskill_zoneA + ($neutralization_point_highskill_zoneA*$rise_cpi_zone_A));  
	
		$unskilled_wage_B = ($initial_fixed_minwage_unskill_zoneB + ($neutralization_point_unskill_zoneB*$rise_cpi_zone_B));
	 	$semiskilled_wage_B = ($initial_fixed_minwage_semiskill_zoneB + ($neutralization_point_semiskill_zoneB*$rise_cpi_zone_B)); 
	 	$skilled_wage_B = ($initial_fixed_minwage_skill_zoneB + ($neutralization_point_skill_zoneB*$rise_cpi_zone_B)); 
	 	$highskilled_wage_B = ($initial_fixed_minwage_highskill_zoneB + ($neutralization_point_highskill_zoneB*$rise_cpi_zone_B));  

		foreach($val['emp_id_table1'] as $data1){
			$employment_wise_area_worker_factor =  db_select('l_min_wages_schedule_employment_master','l')-> fields('l', array('id','node_type_name','area_factor','worker_factor'))->condition('l.id',$data1)->execute()->fetchAssoc();
			$result_1 = array();
			if($employment_wise_area_worker_factor['worker_factor'] == 1 || $employment_wise_area_worker_factor['worker_factor'] == 2 || $employment_wise_area_worker_factor['worker_factor'] == 3 || $employment_wise_area_worker_factor['worker_factor'] == 4){
				$fields1 = array(
								'unskilled_rate_zone_a' => $unskilled_wage_A,
								);
					$result_1 = $fields1;	
				if($employment_wise_area_worker_factor['area_factor'] == 2){
					$fields1_a = array(
								'unskilled_rate_zone_b' => $unskilled_wage_B,
								);
					$result_1 = array_merge($result_1, $fields1_a);
				}
			}
			if($employment_wise_area_worker_factor['worker_factor'] == 2 || $employment_wise_area_worker_factor['worker_factor'] == 3 || $employment_wise_area_worker_factor['worker_factor'] == 4){
				$fields2 = array(
								'semiskilled_rate_zone_a' => $semiskilled_wage_A,
								);
					$result_1 = array_merge($result_1, $fields2);				
				if($employment_wise_area_worker_factor['area_factor'] == 2){
					$fields2_a = array(
								'semiskilled_rate_zone_b' => $semiskilled_wage_B,
								);
					$result_1 = array_merge($result_1, $fields2_a);				
				}
			}
			if($employment_wise_area_worker_factor['worker_factor'] == 3 || $employment_wise_area_worker_factor['worker_factor'] == 4){
				$fields3 = array(
								'skilled_rate_zone_a' => $skilled_wage_A,
								);
					$result_1 = array_merge($result_1, $fields3);							
				if($employment_wise_area_worker_factor['area_factor'] == 2){				
					$fields3_a = array(
									'skilled_rate_zone_b' => $skilled_wage_B,
									);	
					$result_1 = array_merge($result_1, $fields3_a);							
				}
			}
			if($employment_wise_area_worker_factor['worker_factor'] == 4){
				$fields4 = array(
								'highskilled_rate_zone_a' => $highskilled_wage_A,
								);
				$result_1 = array_merge($result_1, $fields4);
				if($employment_wise_area_worker_factor['area_factor'] == 2){		
					$fields4_a = array(
									'highskilled_rate_zone_b' => $highskilled_wage_B,
									);	
					$result_1 = array_merge($result_1, $fields4_a);
				}
			}
					$other_data = array(
										'year' => $year,
										'session' => $session,
										'employment_id' => $employment_wise_area_worker_factor['id'],
										'employment_title' => $employment_wise_area_worker_factor['node_type_name'],
										'cpi_value_zone_a' => $avg_cpi_value_Zone_A,
										'cpi_value_zone_b' => $avg_cpi_value_Zone_B
									);
					$result_1 = array_merge($result_1, $other_data);	
			$data_exist1 = db_select('l_min_wages_schedule_employment_details_info_dev','l')->fields('l',array())->condition('l.employment_id',$data1)->condition('l.year',$year)->condition('l.session',$session)->execute();
			if($data_exist1->rowCount()>0){
				db_update('l_min_wages_schedule_employment_details_info_dev')->fields($result_1)->condition('employment_id',$data1)->condition('year',$year)->condition('session',$session)->execute();
			}else{
				db_insert('l_min_wages_schedule_employment_details_info_dev')->fields($result_1)->execute(); 
			}   
		}
	}
	 
	if($val['select_table2']){ 
 		
		/**** FIxation Point ****/
 		$fixation_value_OLD_IW_Zone_A = $mnw_fix_neut_initial_value['fix_old_iw_a_value'];
		$fixation_value_OLD_IW_Zone_B = $mnw_fix_neut_initial_value['fix_old_iw_b_value'];
		
		/**** Rise in CPI ****/
		$rise_cpi_zone_A = $avg_cpi_value_Zone_A - $fixation_value_OLD_IW_Zone_A; 
		$rise_cpi_zone_B = $avg_cpi_value_Zone_B - $fixation_value_OLD_IW_Zone_B; 
		
		/**** Neutralization Point ****/
		$neutralization_point_OLD_unskill_zoneA = $mnw_fix_neut_initial_value['neut_old_iw_a_u_value'];
		$neutralization_point_OLD_semiskill_zoneA = $mnw_fix_neut_initial_value['neut_old_iw_a_sm_value'];
		$neutralization_point_OLD_skill_zoneA = $mnw_fix_neut_initial_value['neut_old_iw_a_s_value'];
		$neutralization_point_OLD_highskill_zoneA = $mnw_fix_neut_initial_value['neut_old_iw_a_h_value'];
		
		$neutralization_point_OLD_unskill_zoneB = $mnw_fix_neut_initial_value['neut_old_iw_b_u_value'];
		$neutralization_point_OLD_semiskill_zoneB = $mnw_fix_neut_initial_value['neut_old_iw_b_sm_value'];
		$neutralization_point_OLD_skill_zoneB = $mnw_fix_neut_initial_value['neut_old_iw_b_s_value'];
		$neutralization_point_OLD_highskill_zoneB = $mnw_fix_neut_initial_value['neut_old_iw_b_h_value'];
		
		/**** Initial Fixed Value ****/
		
		$initial_fixed_minwage_OLD_unskill_zoneA = $mnw_fix_neut_initial_value['initial_old_iw_a_u_value'];
		$initial_fixed_minwage_OLD_semiskill_zoneA = $mnw_fix_neut_initial_value['initial_old_iw_a_sm_value'];
		$initial_fixed_minwage_OLD_skill_zoneA = $mnw_fix_neut_initial_value['initial_old_iw_a_s_value'];
		$initial_fixed_minwage_OLD_highskill_zoneA = $mnw_fix_neut_initial_value['initial_old_iw_a_h_value'];
		
		$initial_fixed_minwage_OLD_unskill_zoneB = $mnw_fix_neut_initial_value['initial_old_iw_b_u_value'];
		$initial_fixed_minwage_OLD_semiskill_zoneB = $mnw_fix_neut_initial_value['initial_old_iw_b_sm_value'];
		$initial_fixed_minwage_OLD_skill_zoneB = $mnw_fix_neut_initial_value['initial_old_iw_b_s_value'];
		$initial_fixed_minwage_OLD_highskill_zoneB = $mnw_fix_neut_initial_value['initial_old_iw_b_h_value'];
		
		/*** Minimum Wage Worker & Zone Wise ***/
		
		$unskilled_OLD_wage_A = ($initial_fixed_minwage_OLD_unskill_zoneA + ($neutralization_point_OLD_unskill_zoneA*$rise_cpi_zone_A));
		$semiskilled_OLD_wage_A = ($initial_fixed_minwage_OLD_semiskill_zoneA + ($neutralization_point_OLD_semiskill_zoneA*$rise_cpi_zone_A)); 
	 	$skilled_OLD_wage_A = ($initial_fixed_minwage_OLD_skill_zoneA + ($neutralization_point_OLD_skill_zoneA*$rise_cpi_zone_A)); 
	 	$highskilled_OLD_wage_A = ($initial_fixed_minwage_OLD_highskill_zoneA + ($neutralization_point_OLD_highskill_zoneA*$rise_cpi_zone_A));  
	
		$unskilled_OLD_wage_B = ($initial_fixed_minwage_OLD_unskill_zoneB + ($neutralization_point_OLD_unskill_zoneB*$rise_cpi_zone_B));
	 	$semiskilled_OLD_wage_B = ($initial_fixed_minwage_OLD_semiskill_zoneB + ($neutralization_point_OLD_semiskill_zoneB*$rise_cpi_zone_B)); 
	 	$skilled_OLD_wage_B = ($initial_fixed_minwage_OLD_skill_zoneB + ($neutralization_point_OLD_skill_zoneB*$rise_cpi_zone_B)); 
	 	$highskilled_OLD_wage_B = ($initial_fixed_minwage_OLD_highskill_zoneB + ($neutralization_point_OLD_highskill_zoneB*$rise_cpi_zone_B));

		foreach($val['emp_id_table2'] as $data2){
			$employment_wise_area_worker_factor =  db_select('l_min_wages_schedule_employment_master','l')-> fields('l', array('id','node_type_name','area_factor','worker_factor'))->condition('l.id',$data2)->execute()->fetchAssoc();
			$result_old = array();
			
			if($employment_wise_area_worker_factor['worker_factor'] == 1 || $employment_wise_area_worker_factor['worker_factor'] == 2 || $employment_wise_area_worker_factor['worker_factor'] == 3 || $employment_wise_area_worker_factor['worker_factor'] == 4){
				$fields1 = array(
								'unskilled_rate_zone_a' => $unskilled_OLD_wage_A,
								);
					$result_old = $fields1;	
				if($employment_wise_area_worker_factor['area_factor'] == 2){
					$fields1_a = array(
								'unskilled_rate_zone_b' => $unskilled_OLD_wage_B,
								);
					$result_old = array_merge($result_old, $fields1_a);
				}
			}
			if($employment_wise_area_worker_factor['worker_factor'] == 2 || $employment_wise_area_worker_factor['worker_factor'] == 3 || $employment_wise_area_worker_factor['worker_factor'] == 4){
				$fields2 = array(
								'semiskilled_rate_zone_a' => $semiskilled_OLD_wage_A,
								);
					$result_old = array_merge($result_old, $fields2);				
				if($employment_wise_area_worker_factor['area_factor'] == 2){
					$fields2_a = array(
								'semiskilled_rate_zone_b' => $semiskilled_OLD_wage_B,
								);
					$result_old = array_merge($result_old, $fields2_a);				
				}
			}
			if($employment_wise_area_worker_factor['worker_factor'] == 3 || $employment_wise_area_worker_factor['worker_factor'] == 4){
				$fields3 = array(
								'skilled_rate_zone_a' => $skilled_OLD_wage_A,
								);
					$result_old = array_merge($result_old, $fields3);							
				if($employment_wise_area_worker_factor['area_factor'] == 2){				
					$fields3_a = array(
									'skilled_rate_zone_b' => $skilled_OLD_wage_B,
									);	
					$result_old = array_merge($result_old, $fields3_a);							
				}
			}
			if($employment_wise_area_worker_factor['worker_factor'] == 4){
				$fields4 = array(
								'highskilled_rate_zone_a' => $highskilled_OLD_wage_A,
								);
				$result_old = array_merge($result_old, $fields4);
				if($employment_wise_area_worker_factor['area_factor'] == 2){		
					$fields4_a = array(
									'highskilled_rate_zone_b' => $highskilled_OLD_wage_B,
									);	
					$result_old = array_merge($result_old, $fields4_a);
				}
			}
					$other_data = array(
										'year' => $year,
										'session' => $session,
										'employment_id' => $employment_wise_area_worker_factor['id'],
										'employment_title' => $employment_wise_area_worker_factor['node_type_name'],
										'cpi_value_zone_a' => $avg_cpi_value_Zone_A,
										'cpi_value_zone_b' => $avg_cpi_value_Zone_B									);
					$result_old = array_merge($result_old, $other_data);	
			$data_exist2 = db_select('l_min_wages_schedule_employment_details_info_dev','l')->fields('l',array('id'))->condition('l.employment_id',$data2)->condition('l.year',$year)->condition('l.session',$session)->execute();
			if($data_exist2->rowCount()>0){
				db_update('l_min_wages_schedule_employment_details_info_dev')->fields($result_old)->condition('employment_id',$data2)->condition('year',$year)->condition('session',$session)->execute();
			}else{ 
				db_insert('l_min_wages_schedule_employment_details_info_dev')->fields($result_old)->execute(); 
			}   
		}  
	} 
	
	if($val['select_table3']){ 
		
		/**** FIxation Point ****/
		$fixation_value_AL = $mnw_fix_neut_initial_value['fix_al_value'];
		
		/**** Rise in CPI ****/
		$avg_cpi_value_AL = $avg_cpi_value_Zone_B;
		$rise_cpi_zone_AL = $avg_cpi_value_AL - $mnw_fix_neut_initial_value['fix_al_value']; 
		
		/**** Neutralization Point ****/
		$neutralization_point_unskill = $mnw_fix_neut_initial_value['neut_al_u_value'];
		$neutralization_point_semiskill = $mnw_fix_neut_initial_value['neut_al_sm_value'];
		$neutralization_point_skill = $mnw_fix_neut_initial_value['neut_al_s_value'];
		$neutralization_point_highskill = $mnw_fix_neut_initial_value['neut_al_h_value'];
	
		/**** Initial Fixed Value ****/
		$initial_fixed_minwage_unskill = $mnw_fix_neut_initial_value['initial_al_u_value'];
		$initial_fixed_minwage_semiskill = $mnw_fix_neut_initial_value['initial_al_sm_value'];
		$initial_fixed_minwage_skill = $mnw_fix_neut_initial_value['initial_al_s_value'];
		$initial_fixed_minwage_highskill = $mnw_fix_neut_initial_value['initial_al_h_value'];
		
		/*** Minimum Wage Worker & Zone Wise ***/
		$unskilled_wage_AL = ($initial_fixed_minwage_unskill + ($neutralization_point_unskill*$rise_cpi_zone_AL));
		$semiskilled_wage_AL = ($initial_fixed_minwage_semiskill + ($neutralization_point_semiskill*$rise_cpi_zone_AL)); 
	 	$skilled_wage_AL = ($initial_fixed_minwage_skill + ($neutralization_point_skill*$rise_cpi_zone_AL)); 
	 	$highskilled_wage_AL = ($initial_fixed_minwage_highskill + ($neutralization_point_highskill*$rise_cpi_zone_AL));  
		 
		foreach($val['emp_id_table3'] as $data3){ 
			$employment_wise_area_worker_factor =  db_select('l_min_wages_schedule_employment_master','l')-> fields('l', array('id','node_type_name','area_factor','worker_factor'))->condition('l.id',$data3)->execute()->fetchAssoc();
			$result_3 = array();
			if($employment_wise_area_worker_factor['worker_factor'] == 1 || $employment_wise_area_worker_factor['worker_factor'] == 2 || $employment_wise_area_worker_factor['worker_factor'] == 3 || $employment_wise_area_worker_factor['worker_factor'] == 4){
				$fields1 = array(
								'unskilled_rate_zone_a' => $unskilled_wage_AL,
								);
					$result_3 = $fields1;	
			}
			if($employment_wise_area_worker_factor['worker_factor'] == 2 || $employment_wise_area_worker_factor['worker_factor'] == 3 || $employment_wise_area_worker_factor['worker_factor'] == 4){
				$fields2 = array(
								'semiskilled_rate_zone_a' => $semiskilled_wage_AL,
								);
					$result_3 = array_merge($result_3, $fields2);				
			}
			if($employment_wise_area_worker_factor['worker_factor'] == 3 || $employment_wise_area_worker_factor['worker_factor'] == 4){
				$fields3 = array(
								'skilled_rate_zone_a' => $skilled_wage_AL,
								);
					$result_3 = array_merge($result_3, $fields3);							
			}
			if($employment_wise_area_worker_factor['worker_factor'] == 4){
				$fields4 = array(
								'highskilled_rate_zone_a' => $highskilled_wage_AL,
								);
				$result_3 = array_merge($result_3, $fields4);
			}		
					$other_data = array(
										'year' => $year,
										'session' => $session,
										'employment_id' => $employment_wise_area_worker_factor['id'],
										'employment_title' => $employment_wise_area_worker_factor['node_type_name'],
										'cpi_value_zone_b' => $avg_cpi_value_AL
									);
					$result_3 = array_merge($result_3, $other_data);	
			
			$data_exist3 = db_select('l_min_wages_schedule_employment_details_info_dev','l')->fields('l',array())->condition('l.employment_id',$data3)->condition('l.year',$year)->condition('l.session',$session)->execute();
			if($data_exist3->rowCount()>0){  
				db_update('l_min_wages_schedule_employment_details_info_dev')->fields($result_3)->condition('employment_id',$data3)->condition('year',$year)->condition('session',$session)->execute();
			}else{  
				db_insert('l_min_wages_schedule_employment_details_info_dev')->fields($result_3)->execute(); 
			}  
		}
	}
	
	drupal_goto('list-of-minimum-wage-scheduled-employment-dev'); 
}

