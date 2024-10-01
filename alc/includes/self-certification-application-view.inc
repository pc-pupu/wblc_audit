<?php
function self_certification_application_view($id = ''){
	// echo  $id; die;
	$id = encryption_decryption_fun('decrypt', $id);
	
	$sc_query3 = db_select('l_self_certification_registration_master', 'lscrm');
	$sc_query3->leftJoin('district_master', 'dm_con', 'dm_con.district_code = lscrm.est_dist_code');
	$sc_query3->leftJoin('sub_division', 'sd_con', 'sd_con.sub_div_code = lscrm.est_subdiv_code');
	$sc_query3->leftJoin('block_mun_master', 'bmm_con', 'bmm_con.block_code = lscrm.est_block_code');	
	$sc_query3->leftJoin('village_ward_master', 'vwm_con', 'vwm_con.village_code = lscrm.gp_ward_code');	
	
	$sc_query3->fields('lscrm', array('id', 'user_id', 'status', 'est_name', 'est_location', 'phone_no', 'email_address', 'applicable_field_data', 'apply_status', 'regular_male_worker', 'regular_female_worker', 'regular_total_worker', 'contract_male_worker', 'contract_female_worker', 'contract_total_worker', 'contract_total_worker', 'others_male_worker', 'others_female_worker', 'others_total_worker', 'total_male_worker', 'total_female_worker'));
	$sc_query3->fields('dm_con', array('district_name'));
	$sc_query3->fields('sd_con', array('sub_div_name'));
	$sc_query3->fields('bmm_con', array('block_mun_name', 'type'));
	$sc_query3->fields('vwm_con', array('village_name'));
	$sc_query3->condition('lscrm.id', $id);	
	
	$result = $sc_query3->execute()->fetchAssoc();	// print_r($result); die;
	
	$dir_proprietor = db_select('l_common_employee_info', 'lidpm');	
	$dir_proprietor->fields('lidpm', array());
	$dir_proprietor->condition('lidpm.user_id', $result['user_id']);
	$dir_proprietor->condition('lidpm.is_active', 1);
	// $dir_proprietor->condition('lidpm.designation', 'Proprietor', '=');
	$dir_proprietor->condition('lidpm.deleted', 'N');
	$dir_proprietor->orderBy('lidpm.id', 'DESC');
	$ownership_details1	= $dir_proprietor->execute();
	
	$principalEmployer_name = '';
	$proprietor_name = '';
	$director_name = '';
	
	if(!empty($ownership_details1)){
		foreach($ownership_details1 as $listData1){ 
			if($listData1->designation == 'PrincipalEmployer'){
				$principalEmployer_name = $principalEmployer_name.', '.$listData1->name;
			}
			if($listData1->designation == 'Proprietor'){
				$proprietor_name = $proprietor_name.', '.$listData1->name;
			}
			if($listData1->designation == 'director' || $listData1->designation == 'partner'){
				$director_name = $director_name.', '.$listData1->name;
			}
		}
	}
	
	$transaction_details = db_select('l_principle_epayments_receive_data', 'lpd');
	$transaction_details->leftJoin('l_principle_epayments_data', 'lped', 'lped.identification_no = lpd.transaction_id');
	$transaction_details->fields('lped', array('identification_no', 'application_id'));
	$transaction_details->fields('lpd', array());
	$transaction_details->condition('lped.act_id', '100');
	$transaction_details->condition('lped.application_id', $id);		
	$trans_details_result = $transaction_details->execute();
	
	$bank_code = 'Not available';
	$bankTransactionID = 'Not available';
	$total_amount = 'Not available';
	$challan_fid_date = 'Not available';
	$payment_status = 'Not available';
	$grn_number = 'Not available';
	$payment_status = 'Payment Not Made';
		
	 if($trans_details_result-> rowCount() > 0 ){			
		$transaction_det	= $trans_details_result->fetchAssoc();
		$bankTransactionID	= $transaction_det['transaction_id'];
		$grn_number			= $transaction_det['challanrefid'];
		$challan_fid_date	= !empty($transaction_det['challanrefid_date']) ? $transaction_det['challanrefid_date'] : '';
		$total_amount		= number_format($transaction_det['challanamount'], 2);
		$bank_code			= $transaction_det['bank_cd'];
		if($transaction_det['banktransactionstatus'] == 'Success'){
			$payment_status = '<span class="color_green">'.$transaction_det['banktransactionstatus'].'</span>';
	    }else{
			$payment_status = '<span class="color_red">'.$transaction_det['banktransactionstatus'].'</span>';
	    }
	 }
			
	$payment_details = '<u><strong>Grips Payment Details[Online/Counter]</strong></u><br>';
	$payment_details .= 'IFSC Code:<span class="color_orange">'.$bank_code.'</span><br>';
	$payment_details .= 'GRN:<span class="color_orange">'.$grn_number.'</span><br>';
	$payment_details .= 'Total Amount:<span class="color_orange"> &#8377;'.$total_amount.'</span><br>';
	$payment_details .= 'Transaction Date:<span class="color_orange">'.$challan_fid_date.'</span><br>';
	$payment_details .= 'Transaction Status: '.$payment_status;
	
	
	$output = '<div class="box box-primary collapsed-box">
                   <div class="box-header ui-sortable-handle" style="cursor: move;"> <i class="ion ion-clipboard"></i>
                        <h3 class="box-title"> A. Particulars Of Establishment</h3>
                            <div class="pull-right box-tools">
                             	<button class="btn btn-info btn-sm" data-widget="collapse"><i class="fa fa-plus"></i></button>
                            </div>    			  
				  <div class="box-body">
					 <table width="100%" border="0" class="table table-bordered table-responsive">
						<tr>							
							<th style="width:40%">Parameters</th>
							<th style="width:40%">Inputs</th>							
						</tr>
						<tr>
							<td>Name of the establishment</td>
							<td>'.$result['est_name'].'</td>															
						</tr>
						<tr>
							<td>Address/Location of the establishment</td>
							<td>'.$result['est_location'].'<br>'.ucfirst($result['village_name']).'&nbsp;'.$locatypeType.', '.$result['block_mun_name'].'&nbsp;'.$areaType.',<br>'.$result['sub_div_name'].','.$result['district_name'].',<br>Pin-'.$result['est_pin_number'].', West Bengal.</td>															
						</tr>
						<tr>
							<td>Phone Number</td>
							<td>'.$result['mobile'].'</td>							
						</tr>
						<tr>
							<td>Email Address</td>
							<td>'.$result['email'].'</td>							
						</tr>
						<tr>
							<td>Name of the Employer</td>
							<td>'.substr($principalEmployer_name, 2).'</td>							
						</tr>
						<tr>
							<td>Name of the Proprietor<br>(if applicable)</td>
							<td>'.substr($proprietor_name, 2).'</td>							
						</tr>
						<tr>
							<td>List of partners/Directors</td>
							<td>'.substr($director_name, 2).'</td>							
						</tr>';
	
	$output .= '</table></div></div></div>';
	
	$output .= '<div class="box box-primary collapsed-box">
                   <div class="box-header ui-sortable-handle" style="cursor: move;"> <i class="ion ion-clipboard"></i>
                        <h3 class="box-title"> B. No. of Workers Employed</h3>
						<div class="pull-right box-tools">
							<button class="btn btn-info btn-sm" data-widget="collapse"><i class="fa fa-plus"></i></button>
						</div>
						<div class="box-body">
							<table width="100%" border="0" class="table table-bordered table-responsive">
								<tr>
									<th >TYPE OF WORKER</td>
									<th >MALE</th>
									<th >FEMALE</th>
									<th >TOTAL</th>								
								</tr>
								<tr>
									<td>Regular</td>
									<td>'.$result['regular_male_worker'].'</td>
									<td>'.$result['regular_female_worker'].'</td>
									<td>'.$result['regular_total_worker'].'</td>								
								</tr>
								<tr>
									<td>Contract</td>
									<td>'.$result['contract_male_worker'].'</td>
									<td>'.$result['contract_female_worker'].'</td>
									<td>'.$result['contract_total_worker'].'</td>								
								</tr>
								<tr>
									<td>Others</td>
									<td>'.$result['others_male_worker'].'</td>
									<td>'.$result['others_female_worker'].'</td>
									<td>'.$result['others_total_worker'].'</td>								
								</tr>
								<tr>
									<td>Total</td>
									<td>'.$result['total_male_worker'].'</td>
									<td>'.$result['total_female_worker'].'</td>
									<td>'.($result['total_male_worker'] + $result['total_female_worker']).'</td>								
								</tr>
							</table>
						</div>
					</div>
				</div>';
	
	$output .= '<div class="box box-primary collapsed-box">
                   <div class="box-header ui-sortable-handle" style="cursor: move;"> <i class="ion ion-clipboard"></i>
                        <h3 class="box-title"> C. List of Acts applicable to the establishment</h3>
                            <div class="pull-right box-tools">
                             	<button class="btn btn-info btn-sm" data-widget="collapse"><i class="fa fa-plus"></i></button>
                            </div>
							<div class="box-body">
					 		<table width="100%" border="0" class="table table-bordered table-responsive">';
	
	$sc_info_query  = db_select('l_self_certification_form_master ', 'lscfm');	
	$sc_info_query->fields('lscfm', array());
	$sc_info_query->condition('lscfm.is_active', 'Y');
	$sc_info_query->isNull('lscfm.field_type');
	$sc_info_query->orderBy('ord', 'asc');
	$sc_info_query->range(0, 18);			
	$sc_info_query = $sc_info_query->execute();
	
	if($sc_info_query-> rowCount() > 0 ){		
		foreach($sc_info_query as $othersdata){
			
			if($othersdata->input_field == 'Y'){				
				
				$genaratefield_id = 'edit-sc-field-'.$othersdata->id.'-end';				
				
				$is_available = substr_count($othersdata->applicable_field_data, $genaratefield_id);
				
				if($is_available == 1){				
					$checkbox = 'Applicable';
				}else{
					$checkbox = 'Not Applicable';
				}								
			}
			$output .= '<tr><td>'.strtoupper($othersdata->field_name).'</td><td>'.$othersdata->field_title_text.'</td><td>'.$checkbox.'</td></tr>';	
		}
	}
	
	$output .= '</table></div></div></div>';
	
	$output .= '<div class="box box-primary collapsed-box">
                   <div class="box-header ui-sortable-handle" style="cursor: move;"> <i class="ion ion-clipboard"></i>
                        <h3 class="box-title"> D. Submitted Documents and Information [whichever is applicable]</h3>
                            <div class="pull-right box-tools">
                             	<button class="btn btn-info btn-sm" data-widget="collapse"><i class="fa fa-plus"></i></button>
                            </div>
							<div class="box-body">
					 			<table width="100%" border="0" class="table table-bordered table-responsive">';
	
	$sc_info_query  = db_select('l_self_certification_form_master ', 'lscfm');	
	$sc_info_query->fields('lscfm', array());
	$sc_info_query->condition('lscfm.is_active', 'Y');
	$sc_info_query->isNull('lscfm.field_type');
	$sc_info_query->orderBy('ord', 'asc');
	$sc_info_query->range(18, 7);			
	$sc_info_query = $sc_info_query->execute();
	
	if($sc_info_query-> rowCount() > 0 ){		
		foreach($sc_info_query as $othersdata){
			
			if($othersdata->input_field == 'Y'){				
				
				$genaratefield_id = 'edit-sc-field-'.$othersdata->id.'-end';				
				
				$is_available = substr_count($othersdata->applicable_field_data, $genaratefield_id);
				
				if($is_available == 1){				
					$checkbox = 'Applicable';
				}else{
					$checkbox = 'Not Applicable';
				}								
			}
			
			$sc_info_uplad  = db_select('l_self_certification_file_upload ', 'lscfm');	
			$sc_info_uplad	->fields('lscfm', array('file_name'));
			$sc_info_uplad	->condition('lscfm.master_id', $othersdata->id);
			$sc_info_uplad	->condition('lscfm.application_id', $id);
			$sc_info_uplad  ->orderBy('lscfm.id', 'DESC');
			$sc_info_uplad  ->range(0,1);
			$sc_info_result_file = $sc_info_uplad->execute()->fetchAssoc();
			
			$files = $sc_info_result_file['file_name'];
			
			$download_doc = '';
			
			if(file_exists($_SERVER['DOCUMENT_ROOT'] .'/sites/default/files/upload/self_certificate_file/'.$files)){				
				$download_doc = l('<i class="fa fa-file fa-lg"></i>', $GLOBALS['base_url'].'/sites/default/files/upload/self_certificate_file/'.$files, array('html' => true, 'target' => '_blank'));
			}else{
				$download_doc = '';
			}
						
			$output .= '<tr><td>'.strtoupper($othersdata->field_name).'</td><td>'.$othersdata->field_title_text.'</td><td align="center">'.$download_doc.'</td><td>'.$checkbox.'</td></tr>';	
		}
	}
	
	$output .= '</table></div></div></div>';
	
	$output .= '<div class="box box-primary collapsed-box">
                   <div class="box-header ui-sortable-handle" style="cursor: move;"> <i class="ion ion-clipboard"></i>
                        <h3 class="box-title"> Others Information</h3>
                            <div class="pull-right box-tools">
                             	<button class="btn btn-info btn-sm" data-widget="collapse"><i class="fa fa-plus"></i></button>
                            </div>
							<div class="box-body">
					 			<table width="100%" border="0" class="table table-bordered table-responsive">';
	
	$sc_info_query  = db_select('l_self_certification_form_master ', 'lscfm');	
	$sc_info_query->fields('lscfm', array());
	$sc_info_query->condition('lscfm.is_active', 'Y');
	$sc_info_query->isNull('lscfm.field_type');
	$sc_info_query->orderBy('ord', 'asc');
	$sc_info_query->range(25, 9);			
	$sc_info_query = $sc_info_query->execute();
	
	if($sc_info_query-> rowCount() > 0 ){		
		foreach($sc_info_query as $othersdata){
			
			if($othersdata->input_field == 'Y'){				
				
				$genaratefield_id = 'edit-sc-field-'.$othersdata->id.'-end';				
				
				$is_available = substr_count($othersdata->applicable_field_data, $genaratefield_id);
				
				if($is_available == 1){				
					$checkbox = 'Yes';
				}else{
					$checkbox = 'No';
				}								
			}
			$output .= '<tr><td>'.strtoupper($othersdata->field_name).'</td><td>'.$othersdata->field_title_text.'</td><td>'.$checkbox.'</td></tr>';	
		}
	}
	
	$output .= '</table></div></div></div>';
	
	$output .= '<div class="box box-primary collapsed-box">
                   <div class="box-header ui-sortable-handle" style="cursor: move;"> <i class="ion ion-clipboard"></i>
                        <h3 class="box-title"> Payment Information</h3>
                            <div class="pull-right box-tools">
                             	<button class="btn btn-info btn-sm" data-widget="collapse"><i class="fa fa-plus"></i></button>
                            </div>
							<div class="box-body">
					    		<table width="100%" border="0" class="table table-bordered table-responsive">
					 	      		<tr><td width="40%">GRN Number/Others Information</td><td>'.$payment_details.'</td><td width="15%">'.$payment_status.'</td></tr>
					    		</table>
				    		</div>
						</div>';	
		
	$output .= '</div>';	
	
	
	$form['sc']['view_application'] = array(
								'#markup' 	=> $output,
								'#type' 	=> 'markup'
								);
								
    if($result['status'] != 'V' && $result['status'] != 'B'){
   		$form['sc']['application_id'] =	array(						
					'#type'			 => 'hidden',				
					'#default_value' => (isset($result['id'])) ? $result['id']: ''				
					);
		$form['sc']['remark_to'] =	array(						
					'#type'			 => 'hidden',				
					'#default_value' => (isset($result['user_id'])) ? $result['user_id']: ''				
					);
					
   		$action_remark = array('B' => t('Back for correction'), 'V' => t('Verified & Approved'));
											
   		$form['sc']['action'] = array( 
					'#prefix'		=> '<div class="content">
											<div class="panel panel-default panel-form">
											<div class="panel-heading">Action & Remark</div>
												<div class="panel-body">
													<div class="row">														
														<div class="col-md-6 col-md-offset-3"><label class="select label-block">',
					'#suffix'		=> '<i></i></label></div>',
					'#title'		=> 'Select action type',
					'#type' 		=> 'select',
					'#options' 		=> $action_remark,
					'#required' 	=> TRUE,					
					'#attributes' 	=> array('class' => array('form-control')),										
					);
					
		$form['sc']['remark'] = array( 
					'#prefix'		=> '<div class="col-md-6 col-md-offset-3"><label class="input label-block">',
					'#suffix'		=> '<i></i></label></div>',
					'#title'		=> 'Remark',
					'#type' 		=> 'textarea',					
					'#required' 	=> TRUE,
					'#rows'			=>  2,
					'#attributes' 	=> array('class' => array('form-control')),										
					);
					
   		$form['sc']['submit'] = array(
					'#prefix'		=> '<div class="col-md-6 col-md-offset-3">',
					'#suffix'		=> '</div></div></div></div></div>',
					'#attributes' 	=> array('class' => array('btn btn-primary')),
					'#type'			=> 'submit',
					'#value'		=> t('Submit')
					);
	}
					
	$sc_remark = db_select('l_sc_remarks_master', 'rm');
	$sc_remark->fields('rm', array());	
	$sc_remark->condition('rm.application_id', $id);	
	
	$sc_remarkresult = $sc_remark->execute();
	
	$rmdata_arr = '';
	$slno = 1;
	
	foreach($sc_remarkresult as $rmdata){
		if($rmdata->remark_type == '0'){ $current_status = '<span class="backed" title="Back for Rectification"></span>';}
		if($rmdata->remark_type == 'B'){ $current_status = '<span class="backed" title="Back for Rectification"></span>';}
		if($rmdata->remark_type == 'V'){ $current_status = '<span class="approved">Approved</span>';}
	
		$rmdata_arr .= '<tr>
							<td width="7%">'.$slno++.'</td>
							<td width="12%">'.date("d/m/Y h:m A", $rmdata->remark_date).'</td>
							<td>'.$rmdata->remarks.'</td>
							<td width="10%">'.$current_status.'</td>
							<td width="20%">'.$rmdata->remark_by_name.'</td>
							<td width="5%">'.l('<i class="fa fa-remove" title="Delete"></i>', '#', array('html' => TRUE)).'</td>
						</tr>';
	}
   
   $form['sc']['action_remark'] = array(
					'#markup' 	=> '<div class="box box-primary box-solid">
    								   <div class="box-header with-border"> <i class="ion ion-clipboard"></i> Action & Remark Details</div>									   
									   <div class="box-body">
									   	  <table cellpadding="0" cellspacing="0" border="0" width="100%" class="table table-bordered table-responsive">
										  	 <thead>
                							 	<tr>
													<th width="7%">Sl. No.</th>
													<th>Date & Time</th>
													<th>Remark</th>
													<th>Status</th>
													<th>Remark by</th>
													<th width="10%">Action</th>
												</tr>
											</thead>
										    <tbody>
											'.$rmdata_arr.'	
											</tbody>
										  </table>
									   </div>
								   </div>',
					'#type' 	=> 'markup'
					);   
   return $form;	
}

function self_certification_application_view_submit($form, &$form_state){
	global $user;
	$val = $form_state['values'];
	
	$sc_user_query3 = db_select('l_custom_user_detail', 'lscrm');
	$sc_user_query3->fields('lscrm', array('usr_id', 'usr_type', 'fullname', 'mobile', 'officenumber', 'employee_id'));	
	$sc_user_query3->condition('lscrm.usr_id', $user->uid);	
	
	$sc_user_result3 = $sc_user_query3->execute()->fetchAssoc();	
		
	$remark_details  =  array(
							'remarks'   		=> $val['remark'],
							'remark_by'   		=> $user->uid, 
							'remark_to' 		=> $val['remark_to'],
							'application_id' 	=> $val['application_id'],
							'remark_date'  		=> time(),
							'remark_type' 		=> $val['action'],
							'remark_by_name'    => $sc_user_result3['fullname'],
							'remark_field_text' =>	'',
							'remark_by_role' 	=> '4',
							'is_active'			=> 1,
							'hrms_employee_id'  => $sc_user_result3['employee_id']
				);
				
    db_insert('l_sc_remarks_master')->fields($remark_details)->execute();		
	
	$scarr =  array('status' => $val['action']);	
	$query = db_update('l_self_certification_registration_master');
	$query->fields($scarr); 
	$query->condition('id',$val['application_id']);		
	$query->execute();
}
?>