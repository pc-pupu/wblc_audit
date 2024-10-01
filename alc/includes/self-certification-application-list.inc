<?php
function get_self_certification_application(){
	global $user;
	$get_alc_subdivision_details = db_select('l_customuser_relation_address', 'lcra');
	$get_alc_subdivision_details->leftJoin('users_roles', 'ur','ur.uid = lcra.user_id');	
	$get_alc_subdivision_details->fields('lcra',array('sub_div_code'));
	$get_alc_subdivision_details->fields('ur',array('rid'));
	$get_alc_subdivision_details->condition('lcra.user_id', $user->uid);
	$get_alc_result = $get_alc_subdivision_details->execute()->fetchAssoc();
	
		
	$header = array(
				  array('data' => 'Sl.No', 'field' => 'slno','width' => '7%', 'data-orderable' => false),
				  array('data' => 'Identification No.', 'field' => 'aname','width' => '15%'),
				  array('data' => 'Establishment Name', 'field' => 'status','width' => ''),
				  array('data' => 'Apply Date', 'field' => 'reg_no','width' => '10%'),				 
				  array('data' => 'Status', 'field' => 'status','width' => '10%'),				  
				  array('data' => 'Action', 'field' => 'actions','width' => '10%')
				);
	 
	 $sc_query3 = db_select('l_self_certification_registration_master', 'lscrm');
	 $sc_query3->fields('lscrm', array('id', 'user_id', 'est_name', 'creat_date', 'apply_status', 'status',  'apply_date', 'issue_date', 'contract_total_worker', 'est_subdiv_code'));	
	 $sc_query3->condition('lscrm.est_subdiv_code', $get_alc_result['sub_div_code']);	
	 $sc_query3->orderBy('id', 'desc');
	 $sc_query3 = $sc_query3->execute();	 
	 
	 $slno = 1;
	 
	 $view_img = '<span class="badge bg-blue"><i class="fa fa-eye"></i> View Details<div>';
	 
	 if($sc_query3->rowCount() > 0){
	 	foreach($sc_query3 as $data){
			if($data->status == 'N'){ $status = 'INCOMPLETE';}
		 	if($data->status == 'A'){ $status = 'PAYMENT NOT MADE';}
		 	if($data->status == 'P'){ $status = 'PAYMENT MADE';}
		 	if($data->status == 'S'){ $status = '<span class="finalsubmit" title="Final Submit"></span>';}
		 	if($data->status == 'I'){ $status = '<span class="issued" title="Certificate Issued"></span>';}
		 
			$rows[] = array(
						array('data' => $slno++, 'align' => 'left', 'class' => array('')),
					  	array('data' => $data->user_id.'/SCS/'.$data->id.'/WB'),
					  	array('data' => $data->est_name),
					  	array('data' => date("d/m/Y", strtotime($data->apply_date))),
					  	array('data' => $status),					  
					  	array('data' => l($view_img, 'self-certification-application-view/'.encryption_decryption_fun('encrypt', $data->id), array('html' => TRUE)), 'align' => 'left')
					);
		}
	}
	$variables = array(
	  		'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')), 
	  		'header' 	 => $header,
	  		'rows'		 => $rows,
			'empty' 	 => t("No data found!")
	  		);
	
	$output = theme('datatable', $variables);
	return '<div class="row"><div class="col-md-12"><div class="box box-primary"><div class="box-body">'.$output.'</div></div></div></div>';
}
?>