<?php
function get_inspection_dlc_list($form, &$form_state){
	global $base_root, $base_path, $user;
	
	$created_by		=	$user->uid;
	
	$form['created_by']	=	array(
									'#type' 			=> 'hidden',
									'#default_value' 	=> !empty($created_by) ? $created_by : '',
								);
	
	$markup_output	=	getOutputTable($created_by);
	
	$form['markup_output']	=	array(
									'#type' => 'markup',
									'#markup' => $markup_output."<br>"
								);							
	
	$form['submit']			=	array(
			'#type'	=>	'submit',
			'#value'	=>	'Randomization'
	);
	
	
	$query				=	db_query('select id from l_randamization_results where date=:date and dlc_id=:dlc_id', array(':date' => date('Y-m-d'), ':dlc_id' => $created_by));
	
	if($query->rowCount() > 0){
		$markup_output_2	=	getOutputTable_2($created_by);
	
		$form['markup_output_2']	=	array(
										'#type' 	=> 'markup',
										'#markup' 	=> $markup_output_2."<br>"
									);	
	}
								
	return $form;
}

function getOutputTable_2($created_by=''){
	$output			=	'';
	$today_date		=	date('Y-m-d');
	$query_1		=	db_query('select id from l_randamization_results where dlc_id=:dlc_id and date=:today_date', array(':dlc_id' => $created_by, ':today_date' => $today_date));
	$query_1_res	=	$query_1->fetchAssoc();
	$usr_id			=	$query_1_res['id'];
	
	$query_2		=	db_query('select a.em1_file_no, b.fullname, b.usr_id, a.est_category from l_randamization_result_details a join l_custom_user_detail b on b.usr_id=a.inspector_id where a.ran_master_id=:usr_id', array(':usr_id' => $usr_id));
	$output 		= 	'<div style="overflow-y:scroll; height:400px;"><font color="#00FFFF"><b>Randomization Result</b></font><table width="100%" border="1" cellspacing="1" cellpadding="1">';
	$output 		.= 	'<tr>
							<th>Sl. No</th>
							<th>Name/Mobile of the Inspector</th>					
							<th>Details of the Establishment</th>
							<th>Establishment Category</th>
						</tr>';				
	$counter		= 	0;
	if($query_2->rowCount() > 0){
		$ins_id     = 0;
		while($data=$query_2->fetchObject()){
			$ins_id = $data->usr_id;
			$counter++;
			if($data->est_category=="R"){
				$est_category_status	=	"Routine Inspection";	
			}else{
				$est_category_status	=	"Self Certificate Inspection";
			}
			$output .= '<tr bgcolor="'.$color.'">
							<td>'.$counter.'</td>
							<td>'.$data->fullname.'</td>
							<td>'.get_establishment_name($data->em1_file_no).'&nbsp;'.l('View Details','view-establishment-details/'.$data->em1_file_no).'</td>
							<td>'.$est_category_status.'</td>
						</tr>';
			
		}
	}
	$output .= '</table>';
	return $output;	
}

function get_establishment_name($em1_file_no=''){
	$query				=	db_query('select name_of_enterprise from msse_repsitory where em1_file_no=:em1_file_no', array(':em1_file_no' => $em1_file_no));
	$query_res			=	$query->fetchAssoc();
	$name_of_enterprise	=	$query_res['name_of_enterprise'];
	return $name_of_enterprise;
}

function get_inspection_dlc_list_submit($form, &$form_state){
	$array_1	=	array(
		'date'	=>	date('Y-m-d'),
		'dlc_id'	=>	$form_state['values']['created_by']
	);
	$last_insert_id	=	db_insert('l_randamization_results')->fields($array_1)->execute();
	
	$created_by		=	$form_state['values']['created_by']; // 50
	
	$query_1		=	db_query('select usr_id from l_custom_user_detail where created_by=:created_by', array(':created_by' => $created_by));
	$query_1_res	=	$query_1->fetchAssoc();
	$usr_id			=	$query_1_res['usr_id']; // 66
	//echo $created_by; exit;
	$query_2		=	db_query('select a.usr_id, b.block_code from l_custom_user_detail a join l_customuser_relation_address b on a.usr_id = b.user_id where a.created_by=:usr_id and a.usr_type=:usr_type', array(':usr_id' => $usr_id, ':usr_type' => 'INSPECTOR'));
	
	
	if($query_2->rowCount() > 0){
		while($data=$query_2->fetchObject()){
			print_r($data->block_code."<br>");
			$a1				=	db_select('msse_repsitory','mr');
			$a1				->	fields('mr', array( 'em1_file_no' ));
			$a1				->	condition('mr.lc_block_code', $data->block_code, '=');
			$a1				->	condition('mr.is_rnmized', '1', '!=');
			$a1				->	condition('mr.risk_category', 'LR', '!=');
			$a1				->	condition('mr.active', 'Yes', '=');
			$query_3 		= 	$a1->execute();
			if($query_3->rowCount() > 0 ){
				while($data_2	=	$query_3->fetchObject()){
					$array_2	=	array( 
						'em1_file_no'	=>	$data_2->em1_file_no,
						'inspector_id'	=>	$data->usr_id,
						'ran_master_id'	=>	$last_insert_id,
						'est_category'	=>	'R'
					);
					
					db_insert('l_randamization_result_details')->fields($array_2)->execute();
					
					$array_3	=	array(
						'is_rnmized' =>	1,
						'rnd_date'	 =>	date('Y-m-d')					
					);
					
					$queryabc 			= db_update('msse_repsitory');
					$queryabc->fields($array_3);
										
					$queryabc->condition('em1_file_no', trim($data_2->em1_file_no));
					$queryabc->execute();
						
				}
			}
			
			$a2				=	db_select('msse_repsitory','mr');
			$a2				->	fields('mr', array( 'em1_file_no' ));
			$a2				->	condition('mr.lc_block_code', $data->block_code, '=');
			$a2				->	condition('mr.is_rnmized', '1', '!=');
			$a2				->	condition('mr.risk_category', 'HR', '=');
			$a2				->	condition('mr.active', 'Yes', '=');
			$query_5 		= 	$a2->execute();
			
			
			$query_5_res	=	$query_5->rowCount();
			
			$total_HR		=	$query_5_res; // 10
			//$total_HR_10	=	$total_HR/10;
			$total_HR_res	=	round($total_HR/10);
			
			$a3				=	db_select('msse_repsitory','mr');
			$a3				->	fields('mr', array( 'em1_file_no' ));
			$a3				->	condition('mr.lc_block_code', $data->block_code, '=');
			$a3				->	condition('mr.is_rnmized', '1', '!=');
			$a3				->	condition('mr.risk_category', 'MR', '=');
			$a3				->	condition('mr.active', 'Yes', '=');
			$query_6 		= 	$a3->execute();
			
			$query_6_res	=	$query_6->rowCount();
			
			$total_MR		=	$query_6_res; // 5
			$total_MR_5		=	$total_MR/20;
			$total_MR_res	=	round($total_MR/20);
			
			$limit			=	round($total_MR_res+$total_HR_res);
			
			$a4				=	db_select('msse_repsitory','mr');
			$a4				->	fields('mr', array( 'em1_file_no' ));
			$a4				->	condition('mr.lc_block_code', $data->block_code, '=');
			$a4				->	condition('mr.is_rnmized', '1', '!=');
			$a4				->	condition('mr.risk_category', 'LR', '!=');
			$a4				->	condition('mr.active', 'Yes', '=');
			$a4				->	range(0, $limit);
			$query_7 		= 	$a4->execute();
			//print_r($query_7);
			while($data_3	=	$query_7->fetchObject()){
				$array_4	=	array( 
					'em1_file_no'	=>	$data_3->em1_file_no,
					'inspector_id'	=>	$data->usr_id,
					'ran_master_id'	=>	$last_insert_id,
					'est_category'	=>	'C'
				);

				db_insert('l_randamization_result_details')->fields($array_4)->execute();
				
				$array_5	=	array(
					'is_rnmized' =>	1,
					'rnd_date'	 =>	date('Y-m-d')					
				);
				
				$queryabc 			= db_update('msse_repsitory');
				$queryabc->fields($array_5);
									
				$queryabc->condition('em1_file_no', trim($data_3->em1_file_no));
				$queryabc->execute();
					
			}
		}
	}
	
	$message		=	"Randomization successfully done.";
	drupal_set_message($message);
	
}

function getOutputTable($created_by=''){
	$query_1		=	db_query('select usr_id from l_custom_user_detail where created_by=:created_by', array(':created_by' => $created_by));
	$query_1_res	=	$query_1->fetchAssoc();
	$usr_id			=	$query_1_res['usr_id'];
	
	$query_2		=	db_query('select a.fullname, a.mobile, b.block_code from l_custom_user_detail a join l_customuser_relation_address b on a.usr_id = b.user_id where a.created_by=:usr_id and a.usr_type=:usr_type', array(':usr_id' => $usr_id, ':usr_type' => 'INSPECTOR'));

	$output = '<div style="overflow-y:scroll; height:400px;"><table width="100%" border="1" cellspacing="1" cellpadding="1">';
	$output .= '<tr>
					<th>Sl. No</th>
					<th>Name/Mobile of the Inspector</th>					
					<th>Details of the Establishment <br> [Routine Inspection] </th>
					<th>Details of the Establishment <br> [Self Certificate Inspection] </th>
				</tr>';				
	$counter		= 0;				
	if($query_2->rowCount() > 0){
		while($data=$query_2->fetchObject()){
			$total_resp_query = db_query("select count(*) totalest from msse_repsitory where lc_block_code = ".$data->block_code);	
			$total_est_data = $total_resp_query->fetchObject();	
			
			
			$resp_query = db_query("select risk_category, count(*) totalriskest from msse_repsitory where lc_block_code = ".$data->block_code." and is_rnmized != 1 group by risk_category");
			
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
				
			$est_details = '<table width="100%" border="0" cellspacing="1" cellpadding="1">';
			$est_details .= '<tr>					
							<td valign="top" width="30%">Total :</td>					
							<td>'.$total_est_data->totalest.'</td>									
							</tr>
							<tr>					
								<td valign="top">Available for randomization:</td>					
								<td>High Risk:'.$high_risk.'<br>Medium Risk:'.$medium_risk.'<br>Low Risk:'.$low_risk.'</td>									
							</tr>';
			$est_details .= '</table>';
				
			$counter++;
			
			$total_resp_query_2 = db_query("select count(*) totalest from msse_repsitory where sc_status ='1' and lc_block_code = ".$data->block_code);	
			$total_est_data_2 = $total_resp_query_2->fetchObject();	
			
			
			$resp_query_2 = db_query("select risk_category, count(*) totalriskest from msse_repsitory where lc_block_code = ".$data->block_code." and sc_status ='1' group by risk_category");
			
			$high_risk_2   = 0;
			$medium_risk_2 = 0;
			$low_risk_2	   = 0;
			
				
			while($data_risk_2 = $resp_query_2->fetchObject()) {
				if($data_risk_2->risk_category=='HR'){
					$hight_risk_2 = $data_risk_2->totalriskest;
				}else if($data_risk_2->risk_category=='MR'){
					$medium_risk_2 = $data_risk_2->totalriskest;
				}else if($data_risk_2->risk_category=='LR'){
					$low_risk_2 = $data_risk_2->totalriskest;
				}	
			}
			
			$total_risk_est_2 = $high_risk_2+$medium_risk_2+$low_risk_2;
				
			$est_details_2 = '<table width="100%" border="0" cellspacing="1" cellpadding="1">';
			$est_details_2 .= '<tr>					
							<td valign="top" width="30%">Total :</td>					
							<td>'.$total_est_data_2->totalest.'</td>									
							</tr>
							<tr>					
								<td valign="top">Available for randomization:</td>					
								<td>High Risk:'.$high_risk_2.'<br>Medium Risk:'.$medium_risk_2.'</td>									
							</tr>';
			$est_details_2 .= '</table>';
			
			$output .= '<tr>
							<td>'.$counter.'</td>
							<td><b>'.$data->fullname.'</b><br>'.$data->mobile.'<br> Area Name: <b>'.get_area_name($data->block_code).'</b></td>
							<td>'.$est_details.'</td>
							<td>'.$est_details_2.'</td>
						</tr>';
		}
	}
	
	$output .= '</table></div>';
	$output .= '<table>';
	
	$output .= '</table>';	
	return $output;
}

function get_area_name($block_code=''){

              $block_code_query_name = db_select('block_mun_master', 'd');
			 $block_code_query_name->fields('d', array('block_mun_name'));
			 $block_code_query_name->condition('d.block_code', array($block_code),'=');

                  $block_code_name = $block_code_query_name->execute();
                  $result=$block_code_name->fetchAssoc();
	                 $block_mun_name = $result['block_mun_name']; 
					 
					 return $block_mun_name;
}

?>