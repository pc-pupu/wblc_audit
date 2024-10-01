<?php
function view_payment_information($applicationid='',$actid=''){
	global $user;
	$applicationId = encryption_decryption_fun('decrypt', $applicationid);
	$actId = encryption_decryption_fun('decrypt', $actid);
	
	
	$query1 = db_select('l_clra_registration_master', 'cr');
	$query1->leftJoin('l_clra_principle_emp_backlog_data', 'bcr', 'bcr.id = cr.backlog_id');
	$query1->fields('cr', array('id'));
	$query1->fields('bcr', array('fees'));
	$query1->condition('cr.user_id',$user->uid);
	$query1->isNotNull('cr.backlog_id');
	$fees = $query1->execute()->fetchObject()->fees;	
	
	/*$query = db_select('l_principle_epayments_receive_data', 'r');
	$query->leftJoin('l_principle_epayments_data', 'pr', 'pr.identification_no = r.transaction_id');
	$query->fields('r', array('challanamount', 'challanrefid_date'));
	$query->fields('pr', array('act_id'));
	$query->condition('pr.deposited_by',$user->uid);
	$query->condition('r.banktransactionstatus', 'Success');
	$query = $query->execute();*/
	
	$query = db_select('l_principle_epayments_data', 'pr');	
	$query->fields('pr',array('id', 'act_id', 'total_amount', 'payment_mode', 'payment_status', 'identification_no', 'depositor_mobile_no', 'rtn_prd_frm')); 	
	$query->condition('pr.deposited_by',$user->uid);
	if($applicationId!=0 && $actId!=0 ){
	$query->condition('pr.application_id',$applicationId);
	$query->condition('pr.act_id',$actId);	
	}
	$query->orderBy('pr.payment_status', 'desc');
	$query->orderBy('pr.id', 'desc');	
	
	$query = $query->execute();
	
	$output = '<div class="panel panel-default panel-form">
				  <div class="panel-body">
					<div class="row">
						<div class="col-lg-12 col-md-12">';
	
	$header 	= array(
					  array('data' => 'SL. NO', 'width' => '10%'),
					  array('data' => 'Purpose'),
					  array('data' => 'Amount', 'width' => '15%'),
					  array('data' => 'Date',  'width' => '10%'),
					  array('data' => 'STATUS', 'width' => '10%'),
					  array('data' => 'ACTION', 'width' => '10%'),
	  		 	  );
	$counter = 1;						
	if(!empty($fees)){
		/*$output .= '<tr>
						<td>Registration of Principal Employer Under the Contract Labour (Regulation and Abolition) Act, 1970</td>
						<td>'.number_format($fees, 2).'</td><td>Manual Proces</td><td>Registration Fees</td>
					</tr>';*/
		$rows[] = array(
					array('data' =>	$counter++, 'align' => 'left', 'class' => array('')),
					array('data' => 'Registration of Principal Employer Under the Contract Labour (Regulation and Abolition) Act, 1970'),
					array('data' =>	number_format($fees, 2)),
					array('data' =>	'Manual Proces'),
					array('data' =>	'Success', 'align' => 'left'),					
					array('data' =>	'NA')
			  );
		$manual_payment = 1;
	}
	if($query->rowCount() > 0 ){		
		$total = 0;
		foreach($query as $result){
			if(trim($result->act_id) == 1){
				$purpose = 'Registration of Principal Employer Under the Contract Labour (Regulation and Abolition) Act, 1970';
				$type = 'Registration Fees';
			}else if($result->act_id == 2){
				$purpose = 'Registration of Establishments Employing Under Building And Other Construction Workers Act, 1996';
				$type = 'Registration Fees';
			}else if($result->act_id == 3){
				$purpose = 'Registration of Motor Transport Workers Undertaking';
				$type = 'Registration Fees';
			}else if($result->act_id == 4){
				$purpose = 'Registration of Establishment Under the Inter-State Migrant Workers Act, 1979';
				$type = 'Registration Fees';
			}else if($result->act_id == 6){
				$purpose = 'CESS Under The Building and Other Construction Workers&prime; Welfare Cess Act, 1996';
				$type = 'CESS Value';
			}else if($result->act_id == 12){
				$purpose = 'License of Contractor Under the Contract Labour (Regulation and Abolition) Act, 1970';
				$type = 'License & Security Fees';
			}else if($result->act_id == 13){
				$purpose = 'Renewal of Contractor License Under the Contract Labour (Regulation and Abolition) Act, 1970';
				$type = 'Renewal Fees';
			}else if($result->act_id == 14){
				$purpose = 'Amendment of Contractor License Under the Contract Labour (Regulation and Abolition) Act, 1970';
				$type = 'Amendment/Security Fees';
			}else if($result->act_id == 42){
				$purpose = 'License (Employment) of Contractor Under the Inter-State Migrant Workers Act, 1979';
				$type = 'License & Security Fees';
			}else if($result->act_id == 43){
				$purpose = 'License (Recruitment) of Contractor Under the Inter-State Migrant Workers Act, 1979';
				$type = 'License & Security Fees';
			}else if($result->act_id == 100){
				$purpose = 'Registration/Continuation of Registration under Self Certification Scheme, 2016';
				$type = 'Self Certification Fees';
			}
			
			$total = $total + $result->total_amount;
			
			if($result->payment_status == 'N'){
				$status = '<span class=" badge bg-red">Pending</span>';
			}else{
				$status = '<span class=" badge bg-green">'.$result->payment_status.'</span>';
			}					 
						
			$rows[] = array(
					array('data' =>	$counter++, 'align' => 'left', 'class' => array('')),
					array('data' => $purpose, 'align' => 'left', 'class' => array('')),
					array('data' =>	number_format($result->total_amount, 2), 'align' => 'left', 'class' => array('')),
					array('data' =>	date('d/m/Y', strtotime($result->rtn_prd_frm)), 'align' => 'left', 'class' => array('')),
					array('data' =>	$status, 'align' => 'left', 'class' => array('')),					
					array('data' =>	l('<span class=" badge bg-blue">Update Status</span>', 'epayments-info/verification/'.encryption_decryption_fun('encrypt', $result->identification_no), array('html' => TRUE)))
				);
		}
	}
	
	$variables = array(
	  		'attributes' 		=> array('class' => array('table table-striped table-responsive dashboard-table sub-table-dashboard')), 
	  		'header' 			=> $header,
	  		'rows'				=> $rows,
			'empty' 			=> t("No data found!")
	  );
	
	$output .= theme('datatable', $variables);
	
	$output .= '</tbody></table></div></div></div></div>';
	
	return $output;
}
?>