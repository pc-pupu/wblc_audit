<?php

function clra_backlog_details($form, &$form_state){
	$get_all_clra_backlog_table					=	get_all_clra_backlog_table();
	$form['get_clra_backlog_alc_markup']		=	array( '#type' => 'markup', '#markup' => $get_all_clra_backlog_table );
	return $form;
}

function get_all_clra_backlog_table(){
	global $base_root, $base_path, $user;
	$uid										=	$user->uid;
	$user_role = $user->roles;
	drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
	
	$output 									= 	'';
	$header 									= 	array(
														array('data' => 'Sl. No', 'field' => 'slno', 'width' => '6%'),
														array('data'=> 'Reg. Number & Date', 'field' => 'registration_no', 'width' => '15%'),
														array('data'=> 'Name of the Est.', 'field' => 'name_of_the_establishment'),
														// array('data'=> 'Address<br> of the Est.', 'field' => 'address_of_the_establishment'),
														array('data'=> 'Name of the P.E.', 'field' => 'name_of_the_principle_emp', 'width' => '20%'),
														// array('data'=> 'Address<br> of the P.E.', 'field' => 'address_of_the_principle_emp'),
														array('data'=> 'Labour(CL)', 'field' => 'maximum_no_of_contact_labour', 'width' => '10%'),
														array('data'=> 'Fees', 'field' => 'maximum_no_of_contact_labour', 'width' => '8%'),
														array('data'=> 'Action', 'field' => '', 'width' => '12%'),
													);
	$sd_query = db_select('l_customuser_relation_address', 'lcrd');
	$sd_query->	fields('lcrd', array('sub_div_code'));
	$sd_query->condition('user_id',$uid);
	//$sd_query->groupBy('lcrd.registration_no'); 
	$sd_result = $sd_query->execute();
	$sd_row = $sd_result->fetchAssoc();		
													
	$query = db_select('l_clra_principle_emp_backlog_data', 'lcpebd');
	
	$query->fields('lcpebd', array());
	if($user_role['4'] == 'ALC'){
		$query->condition('subdivision_id_establishment',$sd_row['sub_div_code']);
	}
	
	$query->orderBy('lcpebd.id', 'DESC');
	$result= $query->execute();
	
	if( $result->rowCount() > 0 ){
		$count 						= 	1;
		while($data = $result->fetchObject()) {
			$counter = 	$count++;
			$action	=	'encrypt';
			$edit = '<i class="fa fa-edit"></i> Edit';
			
			if($data->status != 1){
				$edit_link = l($edit, 'edit-data/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
				$status = '<span class="badge bg-green"><i class="fa fa-check" aria-hidden="true"></i>Valid</span>';
			}else{
				// $edit_link = 'New Number Generated';
				$edit_link = l($edit, 'edit-data/'.encryption_decryption_fun($action, $data->id), array('html' => TRUE));
				$status = '<span class="badge bg-red"><i class="fa fa-close"></i> Invalid</span> (Already generated a new number)';
			}
						
			$view = '<button type="button" class="btn btn-info pull-right" data-toggle="modal" data-target="#estPeInfo_'.$data->id.'"><i class="fa fa-info-circle"></i>&nbsp;More</button>';
			$view .= '<div class="modal fade" id="estPeInfo_'.$data->id.'" role="dialog">
    					<div class="modal-dialog">
							<div class="box box-primary box-solid">
								<div class="box-header">
									<button type="button" class="close" data-dismiss="modal">&times;</button>
									<h3 class="box-title">'.$data->name_of_the_establishment.'</h3>
								</div>        
          						<div class="modal-body">
         						<table cellpadding="0" cellspacing="0" border="0" class="table table-striped table-responsive admin-custom-table">
									 <tr>										
										<td>Registration Number</td>
										<td>'.$data->registration_no.'</td>
									 </tr>
									 <tr>										 
										 <td>Registration Date</td>
										 <td>'.date('d/m/Y', strtotime($data->registration_date)).'</td>
									 </tr>
									 <tr>										
										 <td>Name of the Establishment </td>
										 <td>'.$data->name_of_the_establishment.'</td>
									 </tr>
									 <tr>										
										 <td>Address of the Establishment</td>
										 <td>'.$data->address_of_the_establishment.'</td>
									 </tr>
									 <tr>										
										 <td>Name of the Principal Employer</td>
										 <td>'.$data->name_of_the_principle_emp.'</td>
									 </tr>
									 <tr>										 
										 <td>Address of the Principal Employer</td>
										 <td>'.$data->address_of_the_principle_emp.'</td>
									 </tr>
									 <tr>									 
										 <td>Maximum Number of Contract Labours</td>
										 <td>'.$data->maximum_no_of_contact_labour.'</td>
									 </tr>
									 <tr>										 
										 <td>Fees</td>
										 <td>₹'.$data->fees.'</td>
									 </tr>
									 <tr>										 
										 <td>Status</td>
										 <td>'.$status.'</td>
									 </tr>
								  </table>
								</div>
							<div class="modal-footer">
								<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
							</div>
						</div>      
					</div>
				</div>';
			
			
			$address_of_est	= smart_wordwrap($data->address_of_the_establishment, 11);
			$address_of_pe = smart_wordwrap($data->address_of_the_principle_emp, 11);
			
			$name_of_the_establishment = smart_wordwrap($data->name_of_the_establishment, 11);
			$name_of_the_principle_emp = smart_wordwrap($data->name_of_the_principle_emp, 11);
						
			$rows[] = array( 
							$counter,
							'<b>'.$data->registration_no.'</b><br>'.date('d/m/Y', strtotime($data->registration_date)), 
							$name_of_the_establishment, 
							// $address_of_est,
							$name_of_the_principle_emp,
							// $address_of_pe,
							$data->maximum_no_of_contact_labour,
							'₹'.$data->fees,
							$edit_link.' '.$view
						);
			}
		}
		$variables 	= 	array( 'header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')), 'empty' => t("No data found!"));						
		$output 	= 	theme('datatable', $variables);
		return $output;
	}	
