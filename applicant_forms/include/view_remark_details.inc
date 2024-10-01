<?php

function view_remark_details($application_en_id, $en_act_id){	
	global $base_root, $base_path, $user;	
	$user_id    = $user->uid;	
	$counter	= 0;
	
	$action	 		= 'decrypt';
	$application_id = encryption_decryption_fun($action, $application_en_id);
	$act_id 		= encryption_decryption_fun($action, $en_act_id);
	
	if($act_id!='' && $act_id!=0 && $application_id!='' && $application_id!=0 && $user_id!='' && $user_id!=0){
	
	$header = array(
		array('data' => 'Sl. No', 'width' => '7%'),
		array('data' => 'Date - Time', 'width' => '10%'),
		array('data' => 'Remark', 'field' => 'remrk'),
		array('data' => 'Remark Status', 'width' => '15%'),
		array('data' => 'Remark By', 'width' => '10%'),
		// array('data' => 'Actions', 'field' => 'licenseno'),
	);
	
	if ($act_id == 1) {		
		$get_remark = db_select('l_remarks_master', 'lrm');
		$get_remark	->leftJoin('role', 'ro', 'ro.rid = lrm.remark_by_role');
		$get_remark ->leftJoin('l_clra_registration_master', 'lcrm', 'lcrm.id=lrm.application_id ');
		$get_remark->fields('lrm', array());
		$get_remark->fields('ro', array('name'));
		$get_remark->fields('lcrm', array('status', 'final_submit_status', 'amendment_parent_id'));
		$get_remark->condition('lrm.application_id', trim($application_id));
		$get_remark->condition('lrm.remark_to',$user_id);
		$get_remark->orderBy('lrm.remark_id','DESC');
		$get_remark->condition('lrm.is_active', 1);		
		/* $db_or = db_or();
		$db_or->condition('lrm.remark_type', 'B' , '=');
		$db_or->condition('lrm.remark_type', 'C' , '=');
		$db_or->condition('lrm.remark_type', 'R' , '=');
		$db_or->condition('lrm.remark_type', 'U' , '=');			
		$get_remark->condition($db_or); */
		
		$get_remark_result = $get_remark->execute()->fetchAll(); 	
	}else if($act_id == 2){
		
			$get_remark = db_select('l_bocwa_remark_master', 'lrm');
			$get_remark	->leftJoin('role', 'ro', 'ro.rid = lrm.remark_by_role');
			$get_remark ->leftJoin('l_bocwa_registration_master', 'lbrm', 'lbrm.id=lrm.application_id ');
			$get_remark->fields('lrm', array());
			$get_remark->fields('ro', array('name'));
			$get_remark->fields('lbrm', array('status', 'final_submit_status'));
			$get_remark->condition('lrm.application_id', trim($application_id));
			$get_remark->condition('lrm.remark_to',$user_id);
			$get_remark->orderBy('lrm.id','DESC');
			
			
			// $db_or = db_or();
			// $db_or->condition('lrm.remark_type', 'B' , '=');
			// $db_or->condition('lrm.remark_type', 'C' , '=');
			// $db_or->condition('lrm.remark_type', 'R' , '=');
			// $db_or->condition('lrm.remark_type', 'U' , '=');
			
			
			// $get_remark->condition($db_or);

			$get_remark_result = $get_remark->execute()->fetchAll(); 
			
			// if($user_id == 352){
			// 	$ackquery = db_select('l_bocwa_remark_master','r')->fields('r', array('id'))->condition('r.application_id',4523)->condition('r.remark_to',$user_id)->execute()->fetchAll();	
			// 	print_r($get_remark_result);
			// 	die;}

	}else if($act_id == 3){
		
			$get_remark = db_select('l_mtw_remark_master', 'lmrm');
			$get_remark	->leftJoin('role', 'ro', 'ro.rid = lmrm.remark_by_role');
			$get_remark ->leftJoin('l_mtw_registration_master', 'lmtrm', 'lmtrm.id=lmrm.application_id ');
			$get_remark->fields('lmrm', array());
			$get_remark->fields('ro', array('name'));
			$get_remark->fields('lmtrm', array('status', 'final_submit_status'));
			$get_remark->condition('lmrm.application_id', trim($application_id));
			$get_remark->condition('lmrm.remark_to',$user_id);
			$get_remark->orderBy('lmrm.id','DESC');
			
			
			$db_or = db_or();
			$db_or->condition('lmrm.remark_type', 'B' , '=');
			$db_or->condition('lmrm.remark_type', 'C' , '=');
			$db_or->condition('lmrm.remark_type', 'R' , '=');
			$db_or->condition('lmrm.remark_type', 'V' , '=');
			$db_or->condition('lmrm.remark_type', 'VA' , '=');
			
			$get_remark->condition($db_or);
			$get_remark_result = $get_remark->execute()->fetchAll(); 
			
	}else if($act_id == 4){
		
			$get_remark = db_select('l_interstate_remark_master', 'lirm');
			$get_remark	->leftJoin('role', 'ro', 'ro.rid = lirm.remark_by_role');
			$get_remark ->leftJoin('l_interstate_workman_master', 'liwm', 'liwm.id=lirm.application_id ');
			$get_remark->fields('lirm', array());
			$get_remark->fields('ro', array('name'));
			$get_remark->fields('liwm', array('status', 'final_submit_status'));
			$get_remark->condition('lirm.application_id', trim($application_id));
			$get_remark->condition('lirm.remark_to',$user_id);
			$get_remark->orderBy('lirm.id','DESC');
			
			
			$db_or = db_or();
			$db_or->condition('lirm.remark_type', 'B' , '=');
			$db_or->condition('lirm.remark_type', 'C' , '=');
			$db_or->condition('lirm.remark_type', 'R' , '=');
			$db_or->condition('lirm.remark_type', 'U' , '=');
			
			
			$get_remark->condition($db_or);
			$get_remark_result = $get_remark->execute()->fetchAll(); 
	}else if($act_id == 42 || $act_id == 43 || $act_id == 44 || $act_id == 45){
		
		$get_remark = db_select('l_interstate_remarks_license', 'lirm');
		$get_remark	->leftJoin('role', 'ro', 'ro.rid = lirm.remark_by_role');
		$get_remark ->leftJoin('l_interstate_license_master', 'liwm', 'liwm.id=lirm.license_id');
		$get_remark->fields('lirm', array());
		$get_remark->fields('liwm', array('status', 'final_submit_status'));
		$get_remark->fields('ro', array('name'));
		$get_remark->condition('lirm.remark_to', $user_id);
		$get_remark->condition('lirm.remark_by', $user_id,'!=');
		$get_remark->condition('liwm.act_id', $act_id);
		$get_remark->condition('lirm.license_id', $application_id);		
		$db_or = db_or();
		$db_or->condition('lirm.remark_type', 'B' , '=');
		$db_or->condition('lirm.remark_type', 'V' , '=');
		$db_or->condition('lirm.remark_type', 'R' , '=');
		$db_or->condition('lirm.remark_type', 'U' , '=');
		$db_or->condition('lirm.remark_type', 'I' , '=');
		$get_remark->condition($db_or);	
		$get_remark->orderBy('lirm.id','DESC');
		$get_remark_result = $get_remark->execute()->fetchAll(); 
	}
	foreach($get_remark_result as $row){ 
			
			$counter 		= $counter+1;
			$remark_by		= $row->remark_by_name.'('.$row->name.')';
			
			if($act_id == 1) {
				
				if($row->remark_type == 'B')  {
					    if(!empty($row->amendment_parent_id))  {
							$remark_details_link = l('CLICK HERE TO RESOLVE APPLICATION', 'clra-reg-amendment/view-clra-application/'.$application_en_id, array('attributes' => array('target' => '_blank')));
						}else{
							$remark_details_link = l('CLICK HERE TO RESOLVE APPLICATION', 'view-clra-application-details/view-clra-application/'.$application_en_id, array('attributes' => array('target' => '_blank')));
						}
						$upload_forms_link	 = ''; 
				} else if($row->remark_type == 'C') {
					
						$remark_details_link = l('BACK TO DASHBOARD', 'applicant-dashboard', array('attributes' => array('html' => TRUE)));
						$upload_forms_link	 = '';
						 
				}else if($row->remark_type == 'R') {
					
						$remark_details_link = l('BACK TO DASHBOARD', 'applicant-dashboard', array('attributes' => array('html' => TRUE)));
						$upload_forms_link   = '';
						 
				}else if($row->remark_type == 'U') {
					
						$remark_details_link = l('BACK TO DASHBOARD', 'applicant-dashboard', array('attributes' => array('html' => TRUE)));
						$upload_forms_link   = l('UPLOAD DOCUMENTS', 'upload_signed_application_form/'.$application_en_id, array('html' => TRUE)); 
				}
				
				$remark_text = $row->remarks;
					
			}else if ($act_id == 2){
				
				if($row->remark_type == 'B') {
		
						$remark_details_link = l('CLICK HERE TO RESOLVE APPLICATION', 'bocwa-application/default/'.$application_en_id, array('attributes' => array('target' => '_blank'))); 
						$upload_forms_link	 = '';
						
				}else if($row->remark_type == 'C') {
		
						$remark_details_link = l('BACK TO DASHBOARD', 'applicant-dashboard', array('attributes' => array('html' => TRUE))); 
						$upload_forms_link	 = '';
				}else if($row->remark_type == 'R') {
		
						$remark_details_link = l('BACK TO DASHBOARD', 'applicant-dashboard', array('attributes' => array('html' => TRUE))); 
						$upload_forms_link	 = '';
				}
				$remark_text = $row->remark_text;
			}else if ($act_id == 3){
				if($row->remark_type == 'B') {
						$remark_details_link = l('CLICK HERE TO RESOLVE APPLICATION', 'mtw-registration-details/default/'.$application_en_id, array('attributes' => array('target' => '_blank'))); 
						$upload_forms_link	 = '';
				}else if($row->remark_type == 'C') {
						$remark_details_link = l('BACK TO DASHBOARD', 'applicant-dashboard', array('attributes' => array('html' => TRUE))); 
						$upload_forms_link	 = '';
				}else if($row->remark_type == 'R') {
						$remark_details_link = l('BACK TO DASHBOARD', 'applicant-dashboard', array('attributes' => array('html' => TRUE))); 
						$upload_forms_link	 = '';
				}
				$remark_text = $row->remark_text;
				
			}else if ($act_id == 4){
				if($row->remark_type == 'B') {
						$remark_details_link = l('CLICK HERE TO RESOLVE APPLICATION', 'ismw_application/default/'.$application_en_id, array('attributes' => array('target' => '_blank'))); 
						$upload_forms_link	 = '';
				}else if($row->remark_type == 'C') {
						$remark_details_link = l('BACK TO DASHBOARD', 'applicant-dashboard', array('attributes' => array('html' => TRUE))); 
						$upload_forms_link	 = '';
				}else if($row->remark_type == 'R') {
						$remark_details_link = l('BACK TO DASHBOARD', 'applicant-dashboard', array('attributes' => array('html' => TRUE))); 
						$upload_forms_link	 = '';
				}else if($row->remark_type == 'U') {
					
						$remark_details_link = l('BACK TO DASHBOARD', 'applicant-dashboard', array('attributes' => array('html' => TRUE)));
						$upload_forms_link   = l('UPLOAD DOCUMENTS', 'upload_signed_application_form/'.$application_en_id, array('html' => TRUE)); 
				}
				$remark_text = $row->remark_text;
			}else if ($act_id == 42){
				if($row->remark_type == 'B') {
						$remark_details_link = l('CLICK HERE TO RESOLVE APPLICATION', 'ismw-license/establishment-details/'.$application_en_id, array('attributes' => array('target' => '_blank'))); 
						$upload_forms_link	 = '';
				}else if($row->remark_type == 'R') {
						$remark_details_link = l('BACK TO DASHBOARD', 'ismw-license-list/'.encryption_decryption_fun('encrypt', $act_id), array('attributes' => array('html' => TRUE))); 
						$upload_forms_link	 = '';
				}else if($row->remark_type == 'U') {
					
						$remark_details_link = l('BACK TO DASHBOARD', 'ismw-license-list/'.encryption_decryption_fun('encrypt', $act_id), array('attributes' => array('html' => TRUE)));
						$upload_forms_link   = l('UPLOAD DOCUMENTS', 'ismwlicense-signed-application/'.$application_en_id, array('html' => TRUE)); 
				}
				$remark_text = $row->remarks;
			}else if ($act_id == 43){
				if($row->remark_type == 'B') {
						$remark_details_link = l('CLICK HERE TO RESOLVE APPLICATION', 'ismw-license-recruitment/contractor-info/'.$application_en_id, array('attributes' => array('target' => '_blank'))); 
						$upload_forms_link	 = '';
				}else if($row->remark_type == 'R') {
						$remark_details_link = l('BACK TO DASHBOARD', 'ismw-license-list/'.encryption_decryption_fun('encrypt', $act_id), array('attributes' => array('html' => TRUE))); 
						$upload_forms_link	 = '';
				}else if($row->remark_type == 'U') {
					
						$remark_details_link = l('BACK TO DASHBOARD', 'ismw-license-list/'.encryption_decryption_fun('encrypt', $act_id), array('attributes' => array('html' => TRUE)));
						$upload_forms_link   = l('UPLOAD DOCUMENTS', 'ismwlicense-signed-application/'.$application_en_id, array('html' => TRUE)); 
				}
				$remark_text = $row->remarks;
			}
			
			if($data->status == 'F'){
					$user_status	= '<span class="inprocess" title="In Process"></span>';
				}else{
					$user_status 	= get_user_application_status($row->remark_type,$row->act_id);
				}
			
			if ($row->status != 'B' ){
				
				$remark_details_link = l('BACK TO DASHBOARD', 'applicant-dashboard', array('attributes' => array('html' => TRUE)));
			}
			
			$rows[] 	= array(				
					array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
					array('data' => date('d/m/Y', $row->remark_date), 'align' => 'left', 'class' => array('odd')),
					array('data' => $remark_text, 'align' => 'left', 'class' => array('odd')),
					array('data' => $user_status, 'align' => 'left', 'class' => array('odd')),
					array('data' => /*$row->remark_by_name.'&nbsp;( ' .*/$row->name/*.' )'*/, 'align' => 'left', 'class' => array('odd')),
					// array('data' => $remark_details_link.'<br/>'.$upload_forms_link, 'align' => 'left', 'class' => array('even')),
					
											
			  );	
		}	
		
	$variables = array(
	  		'attributes' 		=> array('class' => array('table table table-striped table-responsive dashboard-table')), 
	  		'header' 			=> $header,
	  		'rows'				=> $rows,
			'empty' 			=> t("No data found!")
	  );
	  
	$output = theme('datatable', $variables);
		
	return $output;
	}else{
		drupal_set_message('Invalid Response','error');	
	}
}
?>
	
	