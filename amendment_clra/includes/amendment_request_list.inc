<?php
function amendment_request_list(){
	
	global $base_root, $base_path, $user;
		
	drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
	
	$count	=	1;
	$uid	=	$user->uid;
	
	$query 	= 	db_select('l_clra_amendment', 'lcam');
				$query->leftJoin('l_clra_registration_master', 'lcrm','lcrm.id = lcam.application_id');  
				$query->fields('lcam', array());
				$query->fields('lcrm', array('e_name', 'registration_number'));
				//$query->condition('lcam.applicant_subdivision_code', 191001);
				//$query->condition('lcam.status', 0);
				$query->orderBy('lcam.id', 'DESC');
				// $query->groupBy('lcam.applicant_uid');
	$result = 	$query->execute();
	
	$output = 	'';	
	
	$header = 	array(
				array('data' => 'Sl. No', 'field' => 'e_trade_union_regn_no', 'width' => '10%'),
				array('data' => 'Establishment Name', 'field' => 'e_name', 'width' => '35%', 'sort' => 'ASC'),
				array('data' => 'Registration Number', 'field' => 'registration_number', 'width' => '30%'),
				array('data' => 'Status', 'field' => 'status', 'width' => '15%'),					
				array('data' => 'Operation', 'field' => '', 'width' => '10%')
			);
						
	if( $result->rowCount() > 0 ){
		while($data = $result->fetchObject()) {
			$counter	=	$count++;
			
			if(trim($data->status) == 1) $status = '<span style="font-weight:700;color:#093;">ACCEPTED</span>'; else $status = '<span style="font-weight:700;color:#f00;">PENDING</span>';
			$view = '<img title="View Or Edit Information" alt="View Or Edit Information" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/view_details.png">';
			
			$enc_applicant_uid = encryption_decryption_fun('encrypt', $data->applicant_uid);	
			
			$rows[] 	= 	array(
								$counter,
								$data->e_name,
								$data->registration_number,								
								$status,								
								l($view, 'clra-amendment-request/details/'.$enc_applicant_uid, array('html' => TRUE))
							);
		}	
	}
	
	$variables = array( 
						'header' 		=> $header, 
						'rows' 			=> $rows, 
						'attributes' 	=> array('class' => array('zui-table clra_applications'), 
						'style' 		=> 'color:#f00;'), 'empty' => t("No data found!")
					);
						
	$output 						= 	theme('datatable', $variables);
	return $output;
	
}

function amendment_request_details($form, &$form_state, $id=''){
	
	$applicant_uid = encryption_decryption_fun('decrypt', $id);
		
	$query = db_select('l_clra_amendment', 'lram')
					 ->	fields('lram', array('id', 'amendment_fields', 'status', 'amend_apply_date', 'amend_accept_date'))
					 ->	condition('lram.applicant_uid', $applicant_uid)
					 ->	orderBy('id', 'DESC');
					 
	$am_result = $query->execute();
	$am_result_arr = $am_result->fetchAll();
	
	$form['amendment']['accept_head_text'.$am_result->id] = array(
									  '#prefix' 			=> '',  
									  '#suffix' 			=> '',  
									  '#type' 				=> 'markup',									  
									  '#markup' 			=> 'I want to amend some fields.........<br><br>'
									   );
	
		
	foreach($am_result_arr as $am_result){
		$amendment_field_arr = explode(',', $am_result->amendment_fields);
		
		$fields = '';
		$i = 1;
	
		foreach($amendment_field_arr as $val){
			if($val == 'e_name') $fields .= $i++.'. Name and Location of the Establishment.<br>';
			if($val == 'e_postal_address') $fields .= $i++.'. Postal Address of the Establishment.<br>';
			if($val == 'pe_details') $fields .= $i++.'. Name and Address of Principal Employer.<br>';
			if($val == 'man_details') $fields .= $i++.'. Name and Address of Manager or person responsible.<br>';
			if($val == 'e_nature_of_work') $fields .= $i++.'. Nature of Work Carried on in the Establishment.<br>';
			if($val == 'e_any_day_max_num_of_workmen') $fields .= $i++.'. Maximum Number of Workmen Employed Directly on any day in the Establishment.<br>';
			if($val == 'e_num_of_workmen_per_or_reg') $fields .= $i++.'. Number of Workmen Engaged as Permanent/Regular Workmen.<br>';
			if($val == 'e_num_of_workmen_temp_or_reg') $fields .= $i++.'. Number of Workmen Engaged as Temporary/Casual Workmen.<br>';
			if($val == 'workmen_if_same_similar_kind_of_work') $fields .= $i++.'. Whether the Workmen employed/intended to be Employment by the Contractor Perform the same or similar kind of work as the Workmen employed directly by the Principal Employer (if yes, please give here information as detailed below.<br>';			
			if($val == 'max_num_con_emp') $fields .= $i++.'. Maximum number of contract labour to be employed on any day through each contractor.<br>';	
		}
		
		$amend_apply_date = date("d, M Y", strtotime($am_result->amend_apply_date));
		if(empty($am_result->amend_accept_date)){
			$amend_accept_date = 'Not available';
		}else{
			$amend_accept_date = date("d, M Y", strtotime($am_result->amend_accept_date));
		}
		
		$form['amendment']['accept_date'.$am_result->id] = array(
									  '#prefix' 			=> '',  
									  '#suffix' 			=> '',  
									  '#type' 				=> 'markup',									  
									  '#markup' 			=> 'Apply date:'.$amend_apply_date.'<br>Accept date:'.$amend_accept_date.'<br><br>'
									   );
	
		$form['amendment']['accept_text'.$am_result->id] = array(
									  '#prefix' 			=> '',  
									  '#suffix' 			=> '',  
									  '#type' 				=> 'markup',									  
									  '#markup' 			=> $fields
									   );		
		if($am_result->status == 1){
			$form['amendment']['accepted'.$am_result->id] = array(
									  '#prefix' 			=> '<style>.already_accept{color:#093;font-weight:700;font-size:14px;}</style>',  
									  '#suffix' 			=> '',  
									  '#type' 				=> 'markup',									  
									  '#markup' 			=> '<br><span class="already_accept">STATUS: ACCEPTED</span><br>'
									 );
		}else{
			$form['amendment']['accept_id'] = array(
									  '#prefix' 			=> '',  
									  '#suffix' 			=> '',  
									  '#type' 				=> 'hidden',									  
									  '#value' 				=> $am_result->id
									   );		
			$form['amendment']['accept'] = array(
									  '#prefix' 			=> '',  
									  '#suffix' 			=> '<br>',  
									  '#type' 				=> 'submit',
									  '#attributes' 		=> array('class' => array('button')),
									  '#value' 				=> 'ACCEPT'
									   );	
		}
		$form['amendment']['border'.$am_result->id] = array(
									  '#prefix' 			=> '',  
									  '#suffix' 			=> '',  
									  '#type' 				=> 'markup',									  
									  '#markup' 			=> '<hr><br>'
									   );
	}
	return $form;
}

function amendment_request_details_submit($form, &$form_state){
	$val = $form_state['values'];
	
	$fields 	= array('status' => 1, 'amend_accept_date' => date("Y-m-d"));
	$query = db_update('l_clra_amendment');
	$query->fields($fields); 
	$query->condition('id',$val['accept_id']);	
	$query->execute();
	$message = 'Amendment request accepted.';
	drupal_set_message(t($message));
}
?>