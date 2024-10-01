<?php
function generate_monthly_minimum_wage($form, $form_state){
	$fixationPoint = 243;
	
	$avg_cpi_value_6_month = 291;
	$rise_cpi = $avg_cpi_value_6_month - $fixationPoint;
	$neutralization_rate_unskill = 28.69;
	$neutralization_rate_semiskill = 31.56;
	$neutralization_rate_skill = 34.72;
	$neutralization_rate_highskill = 38.19;
	
	$initial_fixed_minwage_unskill = 6972.00;
	$initial_fixed_minwage_semiskill = 7670.00;
	$initial_fixed_minwage_skill = 8437.00;
	$initial_fixed_minwage_highskill = 9280.00;

	$mw_query = db_select('l_min_wages_documents', 'mw');	
	$mw_query->fields('mw',array('year', 'month', 'node_type_name', 'file'));
	$mw_query->condition('mw.month', 'January', '=');
	$mw_query->condition('mw.year', 2018, '=');
	$mw_query->condition('mw.is_active', 1, '=');		
	$mw_query->orderby('mw.node_type_name', 'asc');	
	$mw_result = $mw_query->execute();
	
	if($mw_result->rowCount() > 0){
		$count = 1;
		$mnwlist = '';
		foreach($mw_result as $mw_data){
			$mnwlist .='<tr><td width="6%">'.$count++.'</td><td width="6%" align="center"><input type="checkbox" name=""></td><td>'.strtoupper($mw_data->node_type_name).'</td></tr>';
		}
	}	
	
	$form['min_wage']['header'] = array( 	
		'#prefix'=>'<div class="box box-primary box-solid"><div class="box-body feedback scroll">
					<table class="table table-bordered">
						<thead>
							<tr>
								<th>Sl. No.</th>
								<th>Parameter</th>
								<th>Values</th>
							</tr></thead>
							<tbody><tr>
								<td>1. </td>
								<td>Fixation Point</td>
								<td>'.$fixationPoint.' Note: As declared.</td>
							</tr>
							<tr>
								<td>2. </td>
								<td>Average CPI value of last 6 months</td>
								<td>'.$avg_cpi_value_6_month.'[Note: April,2019-290, May,2019-289, june,2019-290, July,2019-291, Aug,2019-292, Sept.2019-292]<br>Actual Value = 290.66</td>
							</tr>
							<tr>
								<td>3. </td>
								<td>Rise in CPI</td>
								<td>'.$rise_cpi.'</td>
							</tr>
						</tbody>
					</table>
					<table class="table table-bordered">
						<tbody>
							<tr>
								<th colspan="4">Factors of Category wise Minimum wages</th>
							</tr>
							<tr>
								<th rowspan="2" valign="middle">Category </th>
								<th rowspan="2" valign="middle">Initially Fixed Rate</th>
								<th rowspan="2" valign="middle">Calculated CPI Rate</th>
								<th colspan="2" align="center" valign="middle">Minimum wages</th>
							</tr>
							<tr>								
								<th>Zone A</th>
								<th>Zone B</th>
							</tr>
							<tr>
								<td>Unskilled</td>
								<td>'.number_format($initial_fixed_minwage_unskill, 2).'</td>
								<td>'.$neutralization_rate_unskill.' X (291-243) = '.$neutralization_rate_unskill*$rise_cpi.'</td>
								<td>'.number_format(($initial_fixed_minwage_unskill + ($neutralization_rate_unskill*$rise_cpi)), 2).'</td>
								<td>'.number_format(($initial_fixed_minwage_unskill_1 + ($neutralization_rate_unskill*$rise_cpi_1)), 2).'</td>
							</tr>
							<tr>
								<td>Semi-Skilled</td>
								<td>'.number_format($initial_fixed_minwage_semiskill, 2).'</td>
								<td>'.$neutralization_rate_semiskill.' X (291-243) = '.$neutralization_rate_semiskill*$rise_cpi.'</td>
								<td>'.number_format(($initial_fixed_minwage_semiskill + ($neutralization_rate_semiskill*$rise_cpi)), 2).'</td>
								<td>'.number_format(($initial_fixed_minwage_semiskill_1 + ($neutralization_rate_semiskill*$rise_cpi_1)), 2).'</td>
							</tr>
							<tr>
								<td>Skilled</td>
								<td>'.number_format($initial_fixed_minwage_skill, 2).'</td>
								<td>'.$neutralization_rate_skill.' X (291-243) = '.$neutralization_rate_skill*$rise_cpi.'</td>
								<td>'.number_format(($initial_fixed_minwage_skill + ($neutralization_rate_skill*$rise_cpi)), 2).' </td>
								<td>'.number_format(($initial_fixed_minwage_semiskill_1 + ($neutralization_rate_semiskill*$rise_cpi_1)), 2).'</td>
							</tr>
							<tr>
								<td>Highly Skilled </td>
								<td>'.number_format($initial_fixed_minwage_highskill, 2).'</td>
								<td>'.$neutralization_rate_highskill.' X (291-243) = '.$neutralization_rate_highskill*$rise_cpi.'</td>
								<td>'.number_format(($initial_fixed_minwage_highskill + ($neutralization_rate_highskill*$rise_cpi)), 2).'</td>
								<td>'.number_format(($initial_fixed_minwage_semiskill_1 + ($neutralization_rate_semiskill*$rise_cpi_1)), 2).'</td>
							</tr>							
						</tbody>
					</table>
					</div></div>
					<div class="box box-primary box-solid" style="overflow:auto; overflow-y:scroll; max-height:400px;"><div class="box-body feedback scroll">
					<table class="table table-bordered table-fixed table-striped">
						<thead>
							<tr>
								<th width="6%">Sl. No.</th>
								<th align="center" width="6%">Select</th>
								<th>Title</th>
							</tr></thead><tbody>'.$mnwlist
		);	
	
	$form['min_wage']['header2'] = array( 	
		'#type' => 'markup',
		'#markup' => '</tbody></table></div></div>'
		);
	$form['min_wage']['avg_cpi'] = array( 		
		'#type'=>'hidden',
		'#required'=>TRUE,
		'#attributes'=>array('class' => array('form-control')),	
		);
	$form['min_wage']['avg_cpi'] = array( 		
		'#type'=>'hidden',
		'#required'=>TRUE		
		);
	$form['min_wage']['generate']= array(
		'#type'=>'submit',
		'#value'=>'Generate',
		'#attributes'=>array('class' => array('btn btn-primary'),'style'=>'padding-right: 20px;padding-left: 20px;'),
		); 	
	
	return $form;
}
?>