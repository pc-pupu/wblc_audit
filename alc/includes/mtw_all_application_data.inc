<?php
function mtw_all_application_data($form, &$form_state){
	global $base_root, $base_path, $user;
	$mainq = db_select('l_mtw_registration_master','mtw');
	$mainq->fields('mtw',array());
	$mainq->condition('mtw.final_submit_status','P');
	//$mainq->condition('mtw.status','0');
	//$mainq->condition('mtw.is_renew','0');
	//$mainq->condition('mtw.is_renew','1');
	$mainq->orderby('mtw.id','asc');
	$mainr = $mainq->execute();
	
	$output = '<table border="1">
				<tr>
					<th>SL No</th>
					<th>MTW Name</th>
					<th>Reg DT</th>
					<th>Exp DT</th>
					<th>Reg No</th>
					<th>Certi</th>
					<th>Generate</th>
					<th>Worker</th>
					<th>Reg / Renew</th>
					<th>Status</th>
					<th>Paid Fees</th>
					<th>Details</th>
				</tr>';
	
	if($mainr->rowCount() > 0){
		foreach($mainr->fetchAll() as $row){
			
			if(trim($row->is_renew) != 0 ){
				$url = 'mtw-renewcertificate-generate/';
				
			}else{
				$url = 'mtw-certificate-generate/';
			}
			
			$output .= '<tr>
							<td>'.$row->id.'</td>
							<td>'.$row->mtw_name.'</td>
							<td>'.$row->registration_date.'</td>
							<td>'.$row->expiry_date.'</td>
							<td>'.$row->registration_number.'</td>
							<td><a href = "'.$base_root.$base_path.'/sites/default/files/upload/MTW_Registration_Certificate/'.$row->mtw_registration_certificate.'">View</a></td>
							<td><a href="'.$base_root.$base_path.$url.encryption_decryption_fun('encrypt', $row->mtw_qr_code).'">Genrate</a></td>
							<td>'.$row->mtw_maxworkers.'</td>
							<td>'.$row->is_renew.'</td>
							<td>'.$row->status.'</td>
							<td>'.$row->finalfees.'</td>
							<td><a href = "'.$base_root.$base_path.'mtw-all-application-details/'.encryption_decryption_fun('encrypt', $row->id).'">Details</a></td>
						</tr>';
		}
	}
	$output .= '</table>';
	
	
	echo $output;
}

function mtw_all_application_details($form, &$form_state, $id_en = ''){
	
	$application_id = encryption_decryption_fun('decrypt', $id_en);
	
	$renewalq = db_select('l_mtw_registration_renewal','rmtw');
	$renewalq->fields('rmtw',array());
	$renewalq->leftJoin('l_mtw_registration_master','mtw','mtw.id = rmtw.application_id');
	$renewalq->fields('mtw',array('mtw_name','mtw_registration_certificate','mtw_maxworkers'));
	$renewalq->addexpression('mtw.registration_date','reg_ren_dt');
	$renewalq->addexpression('rmtw.status','re_status');
	$renewalq->condition('rmtw.application_id',$application_id);
	$renewalq->orderby('rmtw.id','asc');
	$renewalr = $renewalq->execute();
	
	$output = '<table border="1">
				<tr>
					<th>SL No</th>
					<th>Application ID</th>
					<th>MTW Name</th>
					<th>Reg DT</th>
					<th>Exp DT</th>
					<th>Reg No</th>
					<th>Certi</th>
					<th>Worker</th>
					<th>Ref Number</th>
					<th>Status</th>
					<th>Renewal Status</th>
					<th>Current Fees Original</th>
					<th>Current Fees Payment</th>
					<th>Paid Fees</th>
					<th>Details</th>
				</tr>';
	
	if($renewalr->rowCount() > 0){
		foreach($renewalr->fetchAll() as $row2){
			
			$fees 		= $row2->mtw_maxworkers;
			if($fees <= 5){
				$retFees	=	'10.00';
			}else if($fees > 5 && $fees <= 25  ){
				$retFees	=	'25.00';
			}else if($fees > 25 && $fees <= 50  ){
				$retFees	=	'50.00';
			}else if($fees > 50 && $fees <= 100  ){
				$retFees	=	'100.00';
			}else if($fees > 100 && $fees <= 250  ){
				$retFees	=	'250.00';
			}else if($fees >250 && $fees <= 500){
				$retFees	=	'500.00';
			}else if($fees > 500 && $fees <= 750  ){
				$retFees	=	'750.00';
			}else if($fees > 750 && $fees <= 1000  ){
				$retFees	=	'1000.00';
			}else{
				$retFees	= '0.00';
			}
			
			$output .= '<tr>
							<td>'.$row2->id.'</td>
							<th>'.$row2->application_id.'</th>
							<td>'.$row2->mtw_name.'</td>
							<td>'.$row2->reg_ren_dt.'</td>
							<td>'.$row2->expiry_date.'</td>
							<td>'.$row2->registration_number.'</td>
							<td>'.$row2->mtw_registration_certificate.'</td>
							<td>'.$row2->mtw_maxworkers.'</td>
							<td>'.$row2->renewal_ref_number.'</td>
							<td>'.$row2->status.'</td>
							<td>'.$row2->re_status.'</td>
							<td>'.$retFees.'</td>
							<td>'.$row2->current_fees.'</td>
							<td>'.$row2->paid_fees.'</td>
							<td><a href = "'.$base_root.$base_path.'/sites/default/files/upload/MTW_Registration_Certificate/'.$row2->mtw_registration_certificate.'">View</a></td>
						</tr>';
		}
	}
	
	$output .= '</table>';
	echo $output;
	die;
}
