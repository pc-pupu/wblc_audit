<?php
function establishment_randamization($r_type = '', $ins_id = ''){
		
	$user_query = db_select('l_custom_user_detail', 'lcud');	
	$user_query->join('l_customuser_relation_address', 'lcra', 'lcud.usr_id = lcra.user_id');
	
	$user_query->fields('lcud', array('fullname', 'mobile'));
	$user_query->fields('lcra', array('block_code'));	
	$user_query->condition('lcud.usr_id', $ins_id);		
	$user_query_result = $user_query->execute();
	
	$data_ins = $user_query_result->fetchObject();
	
	$total_resp_query = db_query("select count(*) totalest from msse_repsitory where lc_block_code = ".$data_ins->block_code);	
	$total_est_data = $total_resp_query->fetchObject();	
	
	
	$resp_query = db_query("select risk_category, count(*) totalriskest from msse_repsitory where lc_block_code = ".$data_ins->block_code."  group by risk_category");
	
	$high_risk   = 0;
	$medium_risk = 0;
	$low_risk	 = 0;
	
		
	while($data_risk = $resp_query->fetchObject()) {
		if($data_risk->risk_category=='HR'){
			$hight_risk = $data_risk->totalriskest;
		}else if($data_risk->risk_category=='MR'){
			$medium_risk = $data_risk->totalriskest;
		}else if($data_risk->risk_category=='LR'){
			$low_risk = $data_risk->totalriskest;
		}	
	}
	
	$total_risk_est = $high_risk+$medium_risk+$low_risk;
	
		
	$output = '<table width="100%" border="0" cellspacing="1" cellpadding="1">';
	$output .= '<tr>					
					<td width="25%">Inspector Name:</td>					
					<td>'.$data_ins->fullname.'</td>									
				</tr>
				<tr>					
					<td>Inspector mobile number:</td>					
					<td>'.$data_ins->mobile.'</td>									
				</tr>
				<tr>					
					<td valign="top">Total Establishment:</td>					
					<td>'.$total_est_data->totalest.'</td>									
				</tr>
				<tr>					
					<td valign="top">Available Establishment for randomization:</td>					
					<td>High Risk:'.$high_risk.'<br>Medium Risk:'.$medium_risk.'<br>Low Risk:'.$low_risk.'<br>Tatal:'.$total_risk_est.'</td>									
				</tr>';
	$output .= '</table>';
	
	$output .= '<table width="100%" border="0" cellspacing="1" cellpadding="1">';
	$output .= '<tr>					
					<td>
					
					Inspector Name:
					
					</td>													
				</tr>
				</table>';
				
	return $output;
	
}
?>