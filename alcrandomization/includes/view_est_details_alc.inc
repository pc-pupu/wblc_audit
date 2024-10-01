<?php

function view_est_details_alc($form, &$form_state, $em1_file_no=''){
	
	$get_output_data = get_output_data($em1_file_no);
	
	$form['markup_view']	=	array(
									'#type'=>'markup',
									'#markup' => $get_output_data
								);
	
	return $form;
}

function get_output_data($em1_file_no=''){
	
	$query_villwardkname2 		= db_select('msse_repsitory', 'msr');
	$query_villwardkname2		->fields('msr',array('name_of_enterprise','name_of_applicant', 'per_res_address','permanent_contactno','category_of_enterprise','comment_date','total_annual_turnover','nic_activity_name','assic_code_desc'));		
	$query_villwardkname2		->condition('msr.em1_file_no',trim($em1_file_no),'=');
	//$query_villwardkname		->orderBy('vwm.ord', 'DESC');
	//die($query_villwardkname."@".$form_state['build_info']['args'][0]);
	$get_gp_ward_list2 			= $query_villwardkname2->execute();
	$data = $get_gp_ward_list2 -> fetchAssoc();
	
	$output = '';
		
	$output .= '</style><table width="100%"><tr><td>Name Of Enterprise</td><td> &nbsp;</td><td>'.$data['name_of_enterprise'].'</td></tr>
		  <tr><td>Name Of Applicant</td><td> &nbsp;</td><td>'.$data['name_of_applicant'].'</td></tr>
		  <tr><td>Permanent Address</td><td> &nbsp;</td><td>'.$data['per_res_address'].'</td></tr>
		  <tr><td>Contact Number</td><td> &nbsp;</td><td>'.$data['permanent_contactno'].'</td></tr>
		  <tr><td>Category Of Enterprise</td><td> &nbsp;</td><td>'.$data['category_of_enterprise'].'</td></tr>
		  <tr><td>Date of Commencement</td><td> &nbsp;</td><td>'.$data['comment_date'].'</td></tr>
		  <tr><td>Total Annual Turnover</td><td> &nbsp;</td><td>'.$data['total_annual_turnover'].'</td></tr>
		  </table>';
	return $output;
	/*$form['testdata']   = array(
		  '#title'				=> 'ID Data',
		  '#type' 				=> 'markup',
		  '#markup' 			=> '<style type="text/css">
									table, td, th {border: 1px solid #006595;font-size:15px;font-family:Times New Roman;height:40px;margin-top:20px;}
									td{padding-left:13px !important;}
									th {background-color: #008BD1;color: white;}
									td:hover {background-color:#d4e3e5;}
									tr:nth-child(even) {background: #DBE5F0}
									tr:nth-child(odd) {background: #F1F1F1}
									.red-star{color:#FF0000;}
								</style><table width="100%"><tr><td>Name Of Enterprise</td><td> &nbsp;</td><td>'.$data['name_of_enterprise'].'</td></tr>
		  <tr><td>Name Of Applicant</td><td> &nbsp;</td><td>'.$data['name_of_applicant'].'</td></tr>
		  <tr><td>Permanent Address</td><td> &nbsp;</td><td>'.$data['per_res_address'].'</td></tr>
		  <tr><td>Contact Number</td><td> &nbsp;</td><td>'.$data['permanent_contactno'].'</td></tr>
		  <tr><td>Category Of Enterprise</td><td> &nbsp;</td><td>'.$data['category_of_enterprise'].'</td></tr>
		  <tr><td>Date of Commencement</td><td> &nbsp;</td><td>'.$data['comment_date'].'</td></tr>
		  <tr><td>Total Annual Turnover</td><td> &nbsp;</td><td>'.$data['total_annual_turnover'].'</td></tr>
		  </table>'
	);*/
}