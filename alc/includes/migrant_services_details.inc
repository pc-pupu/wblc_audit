<?php
function services_details($form, &$form_state, $user_role_en = '',$sostype = '', $par = ''){
	
	global $base_root, $base_path, $user;

	$fetch_users_role = db_select('users_roles', 'ur');
	$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role->fields('ro', array('rid', 'name'));
	$fetch_users_role->condition('ur.uid', $user->uid);
	$fetch_users_role_data = $fetch_users_role->execute()->fetchAssoc();
	$user_role = encryption_decryption_fun('decrypt',$user_role_en);
	$role_name = $fetch_users_role_data['name'];

	$activeSA = $activeS = '';
	
	if($par == 'list'){
		$activeSR == 'active';
		$activeSR == '';
		
	}else if($par == 'send-to-ins'){
		$activeSI == 'active';
		$activeSI == '';
		
	}else if($par == 'approved'){
		$activeSA == 'active';
		$activeSA == '';
		
	}else if($par == 'claim-pending'){
		$activeIR == 'active';
		$activeIR == '';
		
	}
 	
	
	if (($user_role == 4) || ($user_role == 5)) {
   
		$form['grievances_lists']['markup_data'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="nav-tabs-custom">
                                <ul class="nav nav-tabs">
                                    <li class="' . $activeSR . '">' . l('Service Claim List', 'services-details/' . $user_role_en . '/services/list') . '</li>
                                    <li class="' . $activeSI . '">' . l('Send to Inspector', 'services-details/' . $user_role_en . '/services/send-to-ins') . '</li>
									<li class="' . $activeSA . '">' . l('Claim Pending', 'services-details/' . $user_role_en . '/services/claim-pending') . '</li>
									<li class="' . $activeIR . '">' . l('Approved Claim', 'services-details/' . $user_role_en . '/services/approved') . '</li>
                                    
                                    
                                </ul>
                                <div class="tab-content">
                                    <div class="tab-pane active" id="fa-icons">
                                    <section id="new"><div class="feedback-scroll">' . services_list_claim($user_role,$sostype,$par) . '</div></section></div>'
        );
		
	}else if($user_role == 7){
		$form['grievances_lists']['markup_data'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="nav-tabs-custom">
							<ul class="nav nav-tabs">
								<li class="' . $activeSR . '">' . l('Service Claim List', 'services-details/' . $user_role_en . '/services/list') . '</li>
								<li class="' . $activeSI . '">' . l('Enquiry Pending', 'services-details/' . $user_role_en . '/services/send-to-ins') . '</li>
								<li class="' . $activeIR . '">' . l('Send to ALC', 'services-details/' . $user_role_en . '/services/claim-pending') . '</li>
							</ul>
                                <div class="tab-content">
                                    <div class="tab-pane active" id="fa-icons">
                                    <section id="new"><div class="feedback-scroll">' . services_list_claim($user_role,$sostype,$par) . '</div></section></div>'
        );
	} else {
		drupal_set_message('You are not authorize to access this page', error);
	}

	return $form;
			


}
function services_list_claim($user_role = '',$sostype = '', $par = ''){
	global $base_root, $base_path, $user;
	
	$table = $apply_category = $unit_name = $actionStatus = '';
	$user_id = $user->uid;

	$counter = 0;
	

	$fetch_users_role = db_select('users_roles', 'ur');
	$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role->fields('ro', array('rid', 'name'));
	$fetch_users_role->condition('ur.uid', $user_id);
	$fetch_users_role_data = $fetch_users_role->execute()->fetchAssoc();

	//print_r($fetch_users_role_data);die;
	$user_role = $fetch_users_role_data['rid'];
	$role_name = $fetch_users_role_data['name'];

	//echo $user_role;die;
	$get_area_codes = db_select('l_customuser_relation_address', 'lcra');
	$get_area_codes->fields('lcra', array('district_code', 'sub_div_code', 'block_code', 'custom_area_jurisdiction', 'is_custom_area_jurisdiction', 'area_type'));
	$get_area_codes->condition('lcra.user_id', $user_id);
	$get_area_codes_result = $get_area_codes->execute()->fetchAssoc();

	
		if (!empty($get_area_codes_result) && !empty($role_name)) {
			$district_code = $get_area_codes_result['district_code'];
			$subdivision_code = $get_area_codes_result['sub_div_code'];
			$block_code = $get_area_codes_result['block_code'];
			$area_type = $get_area_codes_result['area_type'];
			$customjurisdiction = $get_area_codes_result['custom_area_jurisdiction'];
			$iscustom_juris = $get_area_codes_result['is_custom_area_jurisdiction'];
		}
		
			/*$query = db_select('l_migrant_jobseeker_master', 'mjm');
			$query->leftJoin('block_mun_master', 'bmm', 'bmm.block_code=mjm.permanent_areacode');
			$query->fields('mjm', array());		
	
			if ($user_role == 5) {
				if ($customjurisdiction == 'SUBDIV' && $iscustom_juris == 'Y') {
					$subdv_arr = db_select('l_customuser_reletion_services_wise_user_settings', 'cra')->fields('cra', array('sub_div_code'))->condition('cra.user_id', $user_id)->execute()->fetchObject()->sub_div_code;
					$area_arr = explode(',', $subdv_arr);
					$query->condition('mjm.permanent_subdivision',$area_arr,'IN');
				} else {
					$area_arr = db_select('l_customuser_reletion_services_wise_user_settings', 'cra')->fields('cra', array('district_code'))->condition('cra.user_id', $user_id)->execute()->fetchAssoc();
					$query->condition('mjm.permanent_dist',$area_arr,'IN');
				}
	
	
			}else if ($user_role == 4) {
				if ($customjurisdiction == 'BMCNS' && $iscustom_juris == 'Y') {
					$block_arr = db_select('l_customuser_reletion_services_wise_user_settings', 'cra')->fields('cra', array('block_code'))->condition('cra.user_id', $user_id)->execute()->fetchObject()->block_code;
					$area_arr = explode(',', $block_arr);
					$query->condition('mjm.permanent_areacode',$area_arr,'IN');
				} else {
					
					$area_arr = db_select('l_customuser_reletion_services_wise_user_settings', 'cra')->fields('cra', array('sub_div_code'))->condition('cra.user_id', $user_id)->execute()->fetchAssoc();
					
					$query->condition('mjm.permanent_subdivision',$area_arr,'IN');
					
					
				}
	
			}else if ($user_role == 7) {
				$area_arr = db_select('l_customuser_reletion_services_wise_user_settings', 'cra')->fields('cra', array('block_code'))->condition('cra.user_id', $user_id)->execute()->fetchObject()->block_code;
				$query->condition('mjm.permanent_areacode',$area_arr,'=');
			}*/
			

			/*$query->leftJoin('l_migrant_jobseeker_action_master', 'mjam', 'mjam.application_id=mjm.id');
			$query->fields('mjam', array());	
			$query->condition('mjm.status', 'I', '!=');
			$query->condition('mjm.status', '', '!=');
            $query->condition('mjam.flag', 'S', '=');
			if($par == 'list'){
				$query->condition('mjam.status', 'SR');
			}elseif($par == 'send-to-ins'){
				$query->condition('mjam.status', 'SI');
			}else if($par == 'claim-pending'){
				$query->condition('mjam.status', 'IR');
			}else if($par == 'approved'){
				$query->condition('mjam.status', 'SA');
			}
			//if($par == 'list'){
				//$query->condition('mjam.status', 'SR');
				$query->orderBy('mjam.id','DESC');
				$result_help = $query->execute()->fetchAll();*/
				
			$query = db_select('l_migrant_jobseeker_claim_master', 'claim');
			$query->leftJoin('l_migrant_jobseeker_master', 'master', 'master.id=claim.application_id');
			$query->leftJoin('block_mun_master', 'bmm', 'bmm.block_code=master.permanent_areacode');
			$query->fields('master', array());
			$query->fields('claim', array());	
			
				
			if ($user_role == 4) {
				if ($customjurisdiction == 'BMCNS' && $iscustom_juris == 'Y') {
					$block_arr = db_select('l_customuser_reletion_services_wise_user_settings', 'cra')->fields('cra', array('block_code'))->condition('cra.user_id', $user_id)->execute()->fetchObject()->block_code;
					$area_arr = explode(',', $block_arr);
					$query->condition('master.permanent_areacode',$area_arr,'IN');
				} else {
					
					$area_arr = db_select('l_customuser_reletion_services_wise_user_settings', 'cra')->fields('cra', array('sub_div_code'))->condition('cra.user_id', $user_id)->execute()->fetchAssoc();					
					$query->condition('master.permanent_subdivision',$area_arr,'IN');
				}
	
			}else if ($user_role == 7) {
				$area_arr = db_select('l_customuser_reletion_services_wise_user_settings', 'cra')->fields('cra', array('block_code'))->condition('cra.user_id', $user_id)->execute()->fetchObject()->block_code;
				$query->condition('master.permanent_areacode',$area_arr,'=');
			}
			if($par == 'list'){
				$query->condition('claim.status', 'SR');
			}elseif($par == 'send-to-ins'){
				$query->condition('claim.status', 'SI');
			}else if($par == 'claim-pending'){
				$query->condition('claim.status', 'IR');
			}else if($par == 'approved'){
				$query->condition('claim.status', 'SA');
			}
			//$query->condition('claim.flag', 'S', '=');
			$query->orderBy('claim.id','DESC');
			$result_help = $query->execute()->fetchAll(); //echo '<pre>'; print_r($result_help);exit;
			
				
			$counter = 0;
			$header = array(
				array('data' => 'Sl. No.', 'width' => '6%', 'data-orderable' => false),
				array('data' => 'MWIN', 'width' => '13%'),
				array('data' => 'Worker Details', 'width' => '12%'),
				array('data' => 'Claim Type', 'width' => '12%'),
				array('data' => 'Ticket-Id', 'width' => '12%'),	
				array('data' => 'Date', 'width' => '15%'),	
				array('data' => 'Action', 'width' => '5%')
			);
			
			foreach ($result_help as $result_help_details) {
					$claim_details=db_select('l_migrant_jobseeker_claim_master','cm')->fields('cm', array('status'))->condition('cm.application_id',$result_help_details->application_id)->condition('cm.is_active',1)->condition('cm.ticket_no',$result_help_details->ticket_no)->execute()->fetchAssoc();

					//if($claim_details['status'] == 'SR'){
						$counter++;
					
						/*$latitude = $result_help_details->latitude;
						$longitude = $result_help_details->longitude;*/
						
						if($result_help_details->death_type == 'normal_death'){
							$claimType = 'Normal Death Claim';
						}else if($result_help_details->death_type == 'accidental_death'){
							$claimType = 'Accidental Death Claim';
						}
						
						$action_by_role	= $result_help_details->action_by_role; 
						$action_by_id = $result_help_details->action_by; 
						
						$action_by_name = action_official($action_by_id);
		
						if(!empty($result_help_details->action_to_hrms_id)){
							$action_by_hrms_id 				= '(HRMS-ID-'.$result_help_details->action_by_hrms_id.')';
						}else{
							$action_by_hrms_id = '';
						}
		
						$action_to_role					= $result_help_details->action_to_role; 
						$action_to_id					= $result_help_details->action_to; 
		
						if(!empty($result_help_details->action_to_hrms_id)){
							$action_to_hrms_id 				= '(HRMS-ID-'.$result_help_details->action_to_hrms_id.')';
						}else{
							$action_to_hrms_id = '';
						}
						
						$action_to_name = action_official($action_to_id);
						
						$applicant_details 	= $result_help_details->name.'<br>  ('.$result_help_details->mobile.')';
						$application_date 	= date('d-m-Y h:i A', strtotime($result_help_details->created_date));
		
						$query_present_location = db_select('l_migrant_jobseeker_work_details', 'mjwd');
						$query_present_location->fields('mjwd', array('id','present_country','present_state','present_address','present_dist','present_subdivision','present_blockmunname','present_villwardname','present_pin','present_ps','present_po'));
						$query_present_location->condition('mjwd.application_id',$result_help_details->application_id, '=');
						$query_present_location->orderBy('mjwd.id','DESC');
						$result_present_location = $query_present_location->execute()->fetchAssoc();
		
						if($result_present_location['present_country'] == 1){
		
							$present_country = 'India';
							$query_state 	 = db_select('state_master','sm');
							$query_state->fields('sm',array());
							$query_state->condition('sm.id',$result_present_location['present_state'],'=');
							$result_state 	 = $query_state->execute()->fetchAssoc();
		
							$location 		= $result_state['statename'].', '.$present_country;
								
								
						}else{
		
							$location = $result_present_location['present_address'];
							$present_country = '';
						}
		
						$getUserStatus = db_select('l_migrant_jobseeker_master','master')->fields('master', array('id','status'))->condition('id',$result_help_details->application_id)->execute()->fetchAssoc();
		
						if($getUserStatus['status'] == 'P' || $getUserStatus['status'] == 'A'){
							$mwin = $result_help_details->mwin_no.'(P)';
							$link = $base_root . $base_path . 'migrant-worker-details/' . encryption_decryption_fun('encrypt', $result_help_details->application_id) ;
							$mwin_details  = l(t('<span class="badge bg-blue">'.$mwin.'</span>'), $link, array('html' => TRUE));
						}elseif($getUserStatus['status'] == 'C'){
							$mwin_details = $result_help_details->mwin_no;
							
						}
						
						if($user_role == 4){
							if($claim_details['status'] == 'IR'){
								$link_action = $base_root . $base_path . 'migrant-services-action-taken/'.encryption_decryption_fun('encrypt', $result_help_details->application_id).'/'.encryption_decryption_fun('encrypt', $result_help_details->action_id);
							}else{
							$link_action = $base_root . $base_path . 'migrant-worker-services-details/'.encryption_decryption_fun('encrypt', $result_help_details->application_id).'/'.encryption_decryption_fun('encrypt', $result_help_details->action_id);
							}
						}else if($user_role == 7){
							if($claim_details['status'] == 'SI'){
								$link_action = $base_root . $base_path . 'migrant-services-action-taken/'.encryption_decryption_fun('encrypt', $result_help_details->application_id).'/'.encryption_decryption_fun('encrypt', $result_help_details->action_id);
							}else{
								$link_action = $base_root . $base_path . 'migrant-services-action-taken/'.encryption_decryption_fun('encrypt', $result_help_details->application_id).'/'.encryption_decryption_fun('encrypt', $result_help_details->action_id);
							}
						}
							
							
							$action  = l(t('<span class="badge bg-blue"><i class="fa fa-eye"> </i> View</span>'), $link_action, array('html' => TRUE));
		
							//$alertType = $result_help_details->flag_details;
							$ticket = $result_help_details->ticket_no;

							$rows[] = array(
								array('data' => $counter, 'align' => 'left', 'class' => array('even')),
								array('data' => $mwin_details, 'align' => 'left', 'class' => array('odd')),
								array('data' => $applicant_details, 'align' => 'left', 'class' => array('odd')),	
								array('data' => $claimType, 'align' => 'left', 'class' => array('odd')),
								array('data' => $ticket, 'align' => 'left', 'class' => array('even')),
								array('data' => $application_date, 'align' => 'left', 'class' => array('odd')),
								array('data' => $action, 'align' => 'left', 'class' => array('even')),		
								
							);
					//}
					
				}
				$variables = array(
					'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')),
					'header' => $header,
					'rows' => $rows,
					'empty' => t("No data found!")
				);
				
				$output = theme('datatable', $variables);
			/*}else if($par == 'action-taken'){
				$cond_or = db_or();
				$cond_or->condition('mjam.status','SI');
				$cond_or->condition('mjam.status','IR');
				$cond_or->condition('mjam.status','SA');
				$cond_or->condition('mjam.status','CR');
				$query->condition($cond_or);
				$query->orderBy('mjam.id','DESC');
				$result_help = $query->execute()->fetchAll();
				
				$counter = 0;
				$header = array(
					array('data' => 'Sl. No.', 'width' => '6%', 'data-orderable' => false),
					array('data' => 'MWIN', 'width' => '13%'),
					array('data' => 'Alert Type', 'width' => '12%'),
					array('data' => 'Ticket-Id', 'width' => '12%'),
					array('data' => 'Current Status', 'width' => '12%'),	
					array('data' => 'Action', 'width' => '5%')
				);
				
				foreach ($result_help as $result_help_details) {
					$claim_details=db_select('l_migrant_jobseeker_claim_master','cm')->fields('cm', array())->condition('cm.application_id',$result_help_details->application_id)->condition('cm.is_active',1)->condition('cm.ticket_no',$result_help_details->ticket_no)->condition('cm.action_id',$result_help_details->id)->execute()->fetchAssoc();

					if($claim_details['status'] == 'SI' || $claim_details['status'] == 'SR' || $claim_details['status'] == 'IR' || $claim_details['status'] == 'SA' || $claim_details['status'] == 'CR'){
						$counter++;
					
						$latitude = $result_help_details->latitude;
						$longitude = $result_help_details->longitude;
						$action_by_role	= $result_help_details->action_by_role; 
						$action_by_id = $result_help_details->action_by; 
						
						$action_by_name = action_official($action_by_id);
		
						if(!empty($result_help_details->action_to_hrms_id)){
							$action_by_hrms_id 				= '(HRMS-ID-'.$result_help_details->action_by_hrms_id.')';
						}else{
							$action_by_hrms_id = '';
						}
		
						$action_to_role					= $result_help_details->action_to_role; 
						$action_to_id					= $result_help_details->action_to; 
		
						if(!empty($result_help_details->action_to_hrms_id)){
							$action_to_hrms_id 				= '(HRMS-ID-'.$result_help_details->action_to_hrms_id.')';
						}else{
							$action_to_hrms_id = '';
						}
						
						$action_to_name = action_official($action_to_id);
						$application_date 	= date('d-m-Y h:i A', $result_help_details->action_date);
		
		
						$getUserStatus = db_select('l_migrant_jobseeker_master','master')->fields('master', array('id','status'))->condition('id',$result_help_details->application_id)->execute()->fetchAssoc();
		
						if($getUserStatus['status'] == 'P' || $getUserStatus['status'] == 'A'){
							$mwin = $result_help_details->mwin_no.'(P)';
							$link = $base_root . $base_path . 'migrant-worker-details/' . encryption_decryption_fun('encrypt', $result_help_details->application_id) ;
							$mwin_details  = l(t('<span class="badge bg-blue">'.$mwin.'</span>'), $link, array('html' => TRUE));
						}elseif($getUserStatus['status'] == 'C'){
							$mwin_details = $result_help_details->mwin_no;
							
						}
						
							$link_action = $base_root . $base_path . 'migrant-services-action-taken/'.encryption_decryption_fun('encrypt', $result_help_details->application_id).'/'.encryption_decryption_fun('encrypt', $result_help_details->id);
							$action  = l(t('<span class="badge bg-blue"><i class="fa fa-eye"> </i> View</span>'), $link_action, array('html' => TRUE));
		
							$alertType = $result_help_details->flag_details;
							if($result_help_details->status == 'SI'){
								$status = 'Send to Inspector';
							}else if($result_help_details->status == 'IR'){
								$status = 'Enquiry Report Submitted';
							}else if($result_help_details->status == 'SA'){
								$status = 'Approved';
							}else if($result_help_details->status == 'CR'){
								$status = 'Rejected';
							}
							
		
						$rows[] = array(
							array('data' => $counter, 'align' => 'left', 'class' => array('even')),
							array('data' => $mwin_details, 'align' => 'left', 'class' => array('odd')),	
							array('data' => $alertType, 'align' => 'left', 'class' => array('odd')),
							array('data' => $result_help_details->ticket_no, 'align' => 'left', 'class' => array('even')),
							array('data' => $status, 'align' => 'left', 'class' => array('odd')),
							array('data' => $action, 'align' => 'left', 'class' => array('even')),		
							
						);
					}
					
				}
				$variables = array(
					'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')),
					'header' => $header,
					'rows' => $rows,
					'empty' => t("No data found!")
				);
				
				$output = theme('datatable', $variables);
			}*/
			
		return $output;
			


}
function migrant_worker_services_view($form, &$form_state, $id_en = '',$action_id_en = ''){
	global $base_root, $base_path, $user;
	
	$user_id = $user->uid;
	$id = encryption_decryption_fun('decrypt', $id_en);
	$action_id = encryption_decryption_fun('decrypt', $action_id_en);
   
	$get_area_details		=	db_select('l_customuser_relation_address', 'lcra');
	$get_area_details		->  leftJoin('users_roles', 'ur','ur.uid=lcra.user_id ');
	$get_area_details		->  leftJoin('role', 'ro', 'ro.rid = ur.rid');
	$get_area_details       ->leftJoin('l_custom_user_detail', 'lcud', 'ur.uid=lcud.usr_id');
	$get_area_details		->  fields('ro', array('name','rid'));
	$get_area_details		->	fields('lcra',array('district_code','sub_div_code','block_code','custom_area_jurisdiction','is_custom_area_jurisdiction','user_id'));
	
	$get_area_details       ->fields('lcud', array('fullname','employee_id'));
	$get_area_details		->	condition('lcra.user_id',$user_id);
	$get_area_details_result = 	$get_area_details->execute();

	if($get_area_details_result-> rowCount() > 0 ){
		//print_r($get_area_details_result->fetchAssoc());die;
		$officer_data 					= $get_area_details_result->fetchAssoc();  
		$customjurisdiction 			= $officer_data['custom_area_jurisdiction']; 
		$iscustom_juris					= $officer_data['is_custom_area_jurisdiction'];
		$district_code 					= trim($officer_data['district_code']);
		$sub_div_code 					= trim($officer_data['sub_div_code']);
		$user_role						= $officer_data['rid']; 
		$role_name						= $officer_data['name']; 
		$user_role_id					= $officer_data['user_id']; 
		$full_name					    = $officer_data['fullname'];
		$hrms_employee_id 				= $officer_data['employee_id'];
	}

	$main_queary = db_select('l_migrant_jobseeker_master','master')->fields('master', array('name','identification_number','emergency_contact_no','mobile'))->condition('master.id',$id)->execute()->fetchAssoc();

	$geo_loc_queary=db_select('l_migrant_jobseeker_action_master','mjam')->fields('mjam', array())->condition('mjam.application_id',$id)->condition('mjam.id',$action_id)->condition('mjam.is_active',1)->execute()->fetchAssoc();
	
	$claim_details=db_select('l_migrant_jobseeker_claim_master','cm')->fields('cm', array())->condition('cm.application_id',$id)->condition('cm.ticket_no',$geo_loc_queary['ticket_no'])->condition('cm.is_active',1)->execute()->fetchAssoc(); 
	
	if($claim_details['death_type'] == 'normal_death'){
		$claimType = 'Normal Death Claim';
	}else if($claim_details['death_type'] == 'accidental_death'){
		$claimType = 'Accidental Death Claim';
	}
	
	
    $link = $base_root . $base_path . 'migrant-worker-details/' . encryption_decryption_fun('encrypt', $id) ;
	$identification_number  = l(t('<span class="badge bg-blue">'.$main_queary['identification_number'].'</span>'), $link, array('html' => TRUE));

	$output = '<div class="box box-primary">
	   					<div class="box-body">
						   <div class="row">';
	$output .= '<div class="col-md-6">

			<div class="panel panel-primary">
			<div class="panel-heading with-border">DETAILS OF THE MIGRANT WORKER</div>
			<div class="panel-body">
				<div class="col-md-12">
					<table class="table table-striped table-responsive admin-custom-table">
						<thead>
							<tr>
								<th width="60%">Parameters</th>
								<th>Inputs</th>
							</tr>
						</thead>
						<tbody>
						<tr>
							<td height="75"> Name of the worker </td>
							<td height="75">'.$main_queary['name'].'</td>
						</tr>
						<tr>
							<td height="75"> Mobile Number of the Applicant</td>
							<td height="75">'.$main_queary['mobile'].'</td>
						</tr>
						<tr>
							<td height="75"> MWIN number </td>
							<td height="75">'.$identification_number.'</td>
						</tr>
						<tr>
							<td height="75"> Emergency contact number </td>
							<td height="75">'.$main_queary['emergency_contact_no'].'</td>
						</tr>';
						

						$output.='</tbody>
					</table>
				</div>
			</div></div></div>
			<div class="col-md-6">
			<div class="panel panel-primary">
			<div class="panel-heading with-border">CLAIM DETAILS</div>
			<div class="panel-body">
				<div class="col-md-12">
					<table class="table table-striped table-responsive admin-custom-table">
						<thead>
						<tr>
							<th width="60%">Parameters</th>
							<th>Inputs</th>
						</tr>
						</thead>
					<tbody>
						<tr>
							<td height="75"> Claim Type</td>
							<td height="75">'.$claimType.'</td>
						</tr>
						<tr>
							<td height="75"> Claim Details</td>
							<td height="75">'.$geo_loc_queary['message'].'</td>
						</tr>
						<tr>
							<td height="75"> Ticket Details </td>
							<td height="75">'.$geo_loc_queary['ticket_no'].'</td>
						</tr>
						<tr>
							<td height="75"> Claim Raised Date </td>
							<td height="75">'.date('dS M, Y', strtotime($claim_details['created_date'])).'</td>
						</tr>
						';

					

					$output.='</tbody>
					</table>
				</div>

				</div></div></div></div></div></div>';

				$form['service']['markup_data'] = array(
					'#type' => 'markup',
					'#markup' => $output,
				);
				//print_r($claim_details); exit;($claim_details['status']!= 'SA' || $claim_details['status']!= 'SR' || $claim_details['status']!= 'SI')
				if ($user_role == 4 && ($claim_details['status']!='SA'|| $claim_details['status']!='SR' || $claim_details['status']!='SI' )){

					//if(($claim_details['status'] != 'SA' || $claim_details['status'] != 'SR' || $claim_details['status'] != 'SI')){
						$form['service']['markup_data_1'] 		= array(
							'#type' 		=> 'markup',
							'#markup'		=> '<div class="box box-primary box-solid"><div class="box-header with-border">
												<h3 class="box-title">Action Against This Claim</h3></div>
												<div class="box-body">',
						);
							$forward_option = array(
								'' => '- Select -',
								'SI' => 'Send to Inspector For Enquiry',
								'SA' => 'Approve',
								'CR' => 'Reject',
													
							);
						
						
					
							$form['service']['action_taken']		= array(
								'#prefix' 			=> '<div class="form-custom col-md-8" id="action_taken_replace"><label class="input">',
								'#suffix' 			=> '</label></div>',
								'#title'			=> 'Action Taken',
								'#type'				=> 'select',
								'#options'			=> $forward_option,
								'#attributes'		=> array('class' => array('form-control')),
								'#ajax' => array(
									'event' => 'change',
									'effect' => 'fade',			
									'callback' => 'get_ins_callback',
									'progress' => ''
									)
							);
							$form['service']['identification_number']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $main_queary['identification_number'],
							);
							$form['service']['ticket_no']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $geo_loc_queary['ticket_no'],
							);
							$form['service']['application_id']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $geo_loc_queary['application_id'],
							);
							$form['service']['flag_details']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $geo_loc_queary['flag_details'],
							);
							$form['service']['flag_code']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $geo_loc_queary['flag_code'],
							);
							$form['service']['death_type']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $claim_details['death_type'],
							);
							$form['service']['disability_type']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $claim_details['disability_type'],
							);
							$form['service']['worksite_id']	= array(
								'#type'		=> 'hidden',
								'#value' 	=>  $geo_loc_queary['worksite_id'],
							);
							$form['service_action']['raised_by']	= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $geo_loc_queary['raised_by'],
							);
							$form['service']['remark_by']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $user_role_id,
							);
							$form['service']['remark_by_role']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $user_role,
							);	
							$form['service']['remark_by_name']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $full_name,
							);
							$form['service']['remark_by_employee_id']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $hrms_employee_id,
							);
							
							$form['service']['forward_to']		= array(
								'#prefix' 			=> '<div id="forward_to_replace">',
								'#suffix' 			=> '</div>'  
							);
							$form['service']['message']		= array(
								'#prefix' 			=> '<div id="message_replace">',
								'#suffix' 			=> '</div>'  
							);
							$action_taken = isset($form_state['values']['action_taken']) ? $form_state['values']['action_taken'] : '';
							if($action_taken != "" && !empty($action_taken) && $action_taken == 'SI'){
								$form['service']['forward_to']		= array(
									'#prefix' 			=> '<div class="form-custom col-md-8" id="forward_to_replace"><label class="input">',
									'#suffix' 			=> '</label></div>',
									'#title'			=> 'Forward to Inspector',
									'#type'				=> 'select',
									'#options'			=> actions_dropdown_ins($user_role_id,$user_role),
									'#attributes'		=> array('class' => array('form-control')),
									'#ajax' => array(
										'event' => 'change',
										'effect' => 'fade',			
										'callback' => 'ins_callback',
										'progress' => ''
										)
								);
								$form['service']['message']		= array(
									'#prefix' 				=> '<div id="message_replace" class="form-custom col-md-8"><section><label class="input">',
									'#suffix' 				=> '</label></section></div>',
									'#title'				=> 'Remarks',
									'#type'					=> 'textarea',
									'#required' 			=> TRUE,
									'#attributes'			=> array('class' => array('form-control')),
									'#rows'					=>  4,
								);
								$imw_user_id = isset($form_state['values']['forward_to']) ? $form_state['values']['forward_to'] : '';
								if(!empty($imw_user_id)){
									$fetch_users_role		= db_select('users_roles', 'ur');
									$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
									$fetch_users_role->leftJoin('l_custom_user_detail', 'lcud', 'ur.uid=lcud.usr_id');
									$fetch_users_role->fields('ro', array('rid'));
									$fetch_users_role->fields('lcud', array('fullname','employee_id'));
									$fetch_users_role->condition('ur.uid', $imw_user_id);
									$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc();
				
									$remark_to_role			=	$fetch_users_role_data['rid'];
									$imw_full_name			=   $fetch_users_role_data['fullname'];
									$imw_employee_id 		= 	$fetch_users_role_data['employee_id'];
				
								}
								
								$form['service']['remark_to']		= array(
									'#type'					=> 'hidden',
									'#value' 				=>  $imw_user_id,
								);
								$form['service']['remark_to_role']		= array(
									'#type'					=> 'hidden',
									'#value' 				=>  $remark_to_role,
								);	
								$form['service']['remark_to_name']		= array(
									'#type'					=> 'hidden',
									'#value' 				=>  $imw_full_name,
								);	
								$form['service']['remark_to_employee_id']		= array(
									'#type'					=> 'hidden',
									'#value' 				=>  $imw_employee_id,
								);
								
							}else if($action_taken != "" && !empty($action_taken) && $action_taken == 'SA'){
								$form['service']['message']		= array(
									'#prefix' 				=> '<div id="message_replace">',
									'#suffix' 				=> '</div>',
									'#default_value'    	=> 'Claim has been Approved',
								);
							}else if($action_taken != "" && !empty($action_taken) && $action_taken == 'SR'){
								$form['service']['message']		= array(
									'#prefix' 				=> '<div id="message_replace" class="form-custom col-md-8"><section><label class="input">',
									'#suffix' 				=> '</label></section></div>',
									'#title'				=> 'Remarks',
									'#type'					=> 'textarea',
									//'#required' 			=> TRUE,
									'#attributes'			=> array('class' => array('form-control')),
									'#rows'					=>  4,
								);
							}
							$form['service']['submit'] 	= array (
								'#prefix' 			=> '<div class="form-custom col-md-8" id="submit_block">',
								'#suffix' 			=> '</div>',	
								'#type' 			=> 'submit',
								'#attributes' 		=> array('class' => array('btn btn-primary pull-left acremark-btn')),
								'#value' 			=> 'Submit'
							);
							$form['service']['markup_data_2']		= array(
								'#type' 				=> 'markup',
								'#markup'				=> '</div></div>'	
							);	
					
					//}
							
				}
				return $form;
}

function migrant_worker_services_view_submit($form, &$form_state){
	global $base_root, $base_path, $user;
	$form_values = $form_state['values'];
	// echo "<pre>";
	// print_r($form_values);die;
	// if($form_values['supporting_doc'] != 0){
	// 	$doc_details             = file_load($form_values['supporting_doc']);
        
	// 	$doc_details_arr         = explode("/", $doc_details->uri);
	//     $file_mime        =  mime_content_type($doc_details->uri); // $ok_file = 1; $file_ext = '.pdf';
		
		
		
	// 	$file_ext = '';
	// 	$ok_file = 0;
	// 	if(($file_mime == 'application/pdf')){
	// 		$file_ext = '.pdf';
	// 		$ok_file = 1;
	// 		$file = 'imw_enquiry';
	// 	}
	// 	if (!empty($doc_details->uri) && $ok_file == 1){
	// 		$content         =  file_get_contents(escapefile_url($base_root . $base_path . 'sites/default/files/upload/migrantapp/web_temp/' . $doc_details_arr[5]));
                        
	// 		$doc_details_fname    = time() . '_' . $val['application_id'] . '_' .$file.$file_ext;
	// 		$filepathSave = 'sites/default/files/upload/migrantapp/';
	// 		if (!file_exists($filepathSave.$doc_details_fname)) {
				
	// 			$upload_array = array(
	// 				'application_id' => $form_values['application_id'],
	// 				'document_type' => $file,
	// 				'document_filename' => $doc_details_fname,
	// 				'is_active' => '1',
	// 				'ticket_id' 	=> $form_values['ticket_no'],
	// 				'created_date' => date('Y-m-d H:i:s'),
	// 				'flag' => 'IR',  // Enquiry Report
	// 			);

				
	// 			file_put_contents($filepathSave . $doc_details_fname, $content);
				  
	// 			$transaction = db_transaction();
    //                         try {
    //                             $insert_id = db_insert('l_migrant_jobseeker_documents')->fields($upload_array)->execute();
    //                         } catch (Exception $e) {
    //                             $transaction->rollback();
    //                             drupal_set_message(t('Unable to upload your file, please try again.'), 'error');
    //                             return false;
    //                         }
    //                         unset($transaction);
	// 		}
	// 	}else {
	// 		//$ok_file = 0;

	// 		drupal_set_message(t('Unable to upload your '.$file.' file, please try again.'), 'error');
	// 	   // return array('status_code' => 200, 'message' => 'Invalid File Type', 'doc_type' => $key, 'path' => NULL);
	// 	}
	// }
	

	$insarr = array(
		'application_id' => $form_values['application_id'],
		'mwin_no' 	=> $form_values['identification_number'],
		'status' 	=> $form_values['action_taken'],
		'message' 	=> $form_values['message'],
		'ticket_no' 	=> $form_values['ticket_no'],
		'flag_details' 	=> $form_values['flag_details'],
		'flag_web_app' 	=> 'W',
		'flag_code' 	=> $form_values['flag_code'],
		'flag' 			=> 'S',
		'action_date' 	=> time(),
		//  'present_location' => '',
		//  'longitude' => '',
		'raised_by'		 	=> $form_values['raised_by'],
		'worksite_id'		 	=> $form_values['worksite_id'],
		'action_by' => $form_values['remark_by'],
		'action_by_role' => $form_values['remark_by_role'],
		'action_by_hrms_id' => $form_values['remark_by_employee_id'],
		'action_to' => $form_values['remark_to'],
		'action_to_role' => $form_values['remark_to_role'],
		'action_to_hrms_id' => $form_values['remark_to_employee_id'],
		'is_active' => 1,
	 );
	 $remarkId = db_insert('l_migrant_jobseeker_action_master')->fields($insarr)->execute();

	 if($remarkId != ''){
		$claim_details=db_select('l_migrant_jobseeker_claim_master','cm')->fields('cm', array())->condition('cm.application_id',$form_values['application_id'])->condition('cm.ticket_no',$form_values['ticket_no'])->condition('cm.is_active',1)->execute();
		if($claim_details-> rowCount() > 0 ){
			$claim = $claim_details->fetchAssoc();
			$claimArray= array(
				'mwin_no' 				=>  $form_values['identification_number'],
				'application_id' 		=>  $form_values['application_id'],		
				'ticket_no'				=>  $form_values['ticket_no'],
				'claim_flag'			=>  $form_values['flag_code'],
				'death_type'			=>  $form_values['death_type'],
				'disability_type'		=>  $form_values['disability_type'],
				'status'				=>  $form_values['action_taken'],
				'action_id'				=>  $remarkId, // action master id
				'is_active'				=>  1,
				//'created_date'			=>  date('Y-m-d h:i:s'),
				'modified_date'			=>  date('Y-m-d h:i:s')
			);
	
			db_update('l_migrant_jobseeker_claim_master')->fields($claimArray)->condition('id',$claim['id'])->execute();
		}
		
	 }
	 drupal_goto('services-details/'.encryption_decryption_fun('encrypt', $form_values['remark_by_role']).'/services/list');
	 
}



function service_action_view($form, &$form_state, $id_en = '',$action_id_en = ''){ 
	global $base_root, $base_path, $user;
	
	$user_id = $user->uid;
	$id = encryption_decryption_fun('decrypt', $id_en);
	$action_id = encryption_decryption_fun('decrypt', $action_id_en);
   
	$get_area_details		=	db_select('l_customuser_relation_address', 'lcra');
	$get_area_details		->  leftJoin('users_roles', 'ur','ur.uid=lcra.user_id ');
	$get_area_details		->  leftJoin('role', 'ro', 'ro.rid = ur.rid');
	$get_area_details       ->leftJoin('l_custom_user_detail', 'lcud', 'ur.uid=lcud.usr_id');
	$get_area_details		->  fields('ro', array('name','rid'));
	$get_area_details		->	fields('lcra',array('district_code','sub_div_code','block_code','custom_area_jurisdiction','is_custom_area_jurisdiction','user_id'));
	
	$get_area_details       ->fields('lcud', array('fullname','employee_id'));
	$get_area_details		->	condition('lcra.user_id',$user_id);
	$get_area_details_result = 	$get_area_details->execute();

	if($get_area_details_result-> rowCount() > 0 ){
		//print_r($get_area_details_result->fetchAssoc());die;
		$officer_data 					= $get_area_details_result->fetchAssoc();  
		$customjurisdiction 			= $officer_data['custom_area_jurisdiction']; 
		$iscustom_juris					= $officer_data['is_custom_area_jurisdiction'];
		$district_code 					= trim($officer_data['district_code']);
		$sub_div_code 					= trim($officer_data['sub_div_code']);
		$user_role						= $officer_data['rid']; 
		$role_name						= $officer_data['name']; 
		$user_role_id					= $officer_data['user_id']; 
		$full_name					    = $officer_data['fullname'];
		$hrms_employee_id 				= $officer_data['employee_id'];
	}

	$main_queary = db_select('l_migrant_jobseeker_master','master')->fields('master', array('name','identification_number','emergency_contact_no','mobile'))->condition('master.id',$id)->execute()->fetchAssoc();

	$geo_loc_queary=db_select('l_migrant_jobseeker_action_master','mjam')->fields('mjam', array())->condition('mjam.application_id',$id)->condition('mjam.id',$action_id)->condition('mjam.is_active',1)->execute()->fetchAssoc();

	$claim_details=db_select('l_migrant_jobseeker_claim_master','cm')->fields('cm', array())->condition('cm.application_id',$id)->condition('cm.ticket_no',$geo_loc_queary['ticket_no'])->condition('cm.is_active',1)->execute()->fetchAssoc();

    $link = $base_root . $base_path . 'migrant-worker-details/' . encryption_decryption_fun('encrypt', $id) ;
	$identification_number  = l(t('<span class="badge bg-blue">'.$main_queary['identification_number'].'</span>'), $link, array('html' => TRUE));
 			
	
	$output = '<div class="box box-primary">
	   					<div class="box-body">
						   <div class="row">';
	$output .= '<div class="col-md-6">

			<div class="panel panel-primary">
			<div class="panel-heading with-border">DETAILS OF THE MIGRANT WORKERs</div>
			<div class="panel-body">
				<div class="col-md-12">
					<table class="table table-striped table-responsive admin-custom-table">
						<thead>
							<tr>
								<th width="60%">Parameters</th>
								<th>Inputs</th>
							</tr>
						</thead>
						<tbody>
						<tr>
							<td height="75"> Name of the worker </td>
							<td height="75">'.$main_queary['name'].'</td>
						</tr>
						<tr>
							<td height="75"> Mobile Number of the Applicant</td>
							<td height="75">'.$main_queary['mobile'].'</td>
						</tr>
						<tr>
							<td height="75"> MWIN number </td>
							<td height="75">'.$identification_number.'</td>
						</tr>
						<tr>
							<td height="75"> Emergency contact number </td>
							<td height="75">'.$main_queary['emergency_contact_no'].'</td>
						</tr>';
						

						$output.='</tbody>
					</table>
				</div>
			</div></div></div>
			<div class="col-md-6">
			<div class="panel panel-primary">
			<div class="panel-heading with-border">CLAIM DETAILS</div>
			<div class="panel-body">
				<div class="col-md-12">
					<table class="table table-striped table-responsive admin-custom-table">
						<thead>
						<tr>
							<th width="60%">Parameters</th>
							<th>Inputs</th>
						</tr>
						</thead>
					<tbody>
						<tr>
							<td height="75"> Claim For</td>
							<td height="75">'.$geo_loc_queary['flag_details'].'</td>
						</tr>
						<tr>
							<td height="75"> Claim Details</td>
							<td height="75">'.$geo_loc_queary['message'].'</td>
						</tr>
						<tr>
							<td height="75"> Ticket Details </td>
							<td height="75">'.$geo_loc_queary['ticket_no'].'</td>
						</tr>
						<tr>
							<td height="75"> Claim Raised Date </td>
							<td height="75">'.date('dS M, Y', strtotime($claim_details['created_date'])).'</td>
						</tr>
						';

					

					$output.='</tbody>
					</table>
				</div>

				</div></div></div></div></div></div>';
				$form['service_action']['markup_data'] = array(
					'#type' => 'markup',
					'#markup' => $output,
				);
				
					$migrant_jobseeker_doc 	= db_select('l_migrant_jobseeker_documents', 'mjd');
					$migrant_jobseeker_doc->fields('mjd', array());
					$migrant_jobseeker_doc->condition('mjd.application_id', $id);
					$migrant_jobseeker_doc->condition('mjd.is_active', 1);
					$migrant_jobseeker_doc->condition('mjd.flag', 'IR');
					$doc_details = $migrant_jobseeker_doc->execute()->fetchAll();
          
   
			foreach ($doc_details as $value) {
				if($value->document_type == 'imw_enquiry'){  
					$doc_file_name_imw_enquiry = $value->document_filename;
					$mj_doc_imw_enquiry = '<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/migrantapp/'.$doc_file_name_imw_enquiry.'"><i class="fa fa-file fa-lg"></i></a>';
					//echo $GLOBALS['base_url'].'/sites/default/files/upload/migrantapp/'.$doc_file_name_imw_enquiry; die;
				}
			}
				$enquiry_details=db_select('l_migrant_jobseeker_action_master','mjam')->fields('mjam', array())->condition('mjam.application_id',$id)->condition('mjam.id',$action_id)->condition('mjam.is_active',1)->orderBy('mjam.id','DESC')->execute()->fetchAssoc();
				
				if($user_role == 7){
					
					
					 //$claim_details=db_select('l_migrant_jobseeker_claim_master','cm')->fields('cm', array())->condition('cm.application_id',$id)->orderBy('cm.id','DESC')->condition('cm.is_active',1)->execute()->fetchAssoc();
					
					if($enquiry_details['action_to'] == $user_role_id && $claim_details['status'] == 'SI'){
						$form['service_action']['action_to_data'] 		= array(
							'#type' 		=> 'markup',
							'#markup'		=> '<div class="box box-primary box-solid"><div class="box-header with-border">
												<h3 class="box-title">Enquiry Report</h3></div>
												<div class="box-body">',
						);
						$form['service_action']['action_taken']		= array(
							'#type'					=> 'hidden',
							'#value' 				=>  'IR',
						);
						$form['service_action']['identification_number']		= array(
							'#type'					=> 'hidden',
							'#value' 				=>  $main_queary['identification_number'],
						);
						$form['service_action']['ticket_no']		= array(
							'#type'					=> 'hidden',
							'#value' 				=>  $geo_loc_queary['ticket_no'],
						);
						$form['service_action']['application_id']		= array(
							'#type'					=> 'hidden',
							'#value' 				=>  $geo_loc_queary['application_id'],
						);
						$form['service_action']['flag_details']		= array(
							'#type'					=> 'hidden',
							'#value' 				=>  $geo_loc_queary['flag_details'],
						);
						$form['service_action']['flag_code']		= array(
							'#type'					=> 'hidden',
							'#value' 				=>  $geo_loc_queary['flag_code'],
						);
						$form['service_action']['death_type']		= array(
							'#type'					=> 'hidden',
							'#value' 				=>  $claim_details['death_type'],
						);
						$form['service_action']['disability_type']		= array(
							'#type'					=> 'hidden',
							'#value' 				=>  $claim_details['disability_type'],
						);
						$form['service_action']['raised_by']	= array(
							'#type'					=> 'hidden',
							'#value' 				=>  $geo_loc_queary['raised_by'],
						);
						$form['service_action']['worksite_id']	= array(
							'#type'		=> 'hidden',
							'#value' 	=>  $geo_loc_queary['worksite_id'],
						);
						$form['service_action']['remark_by']		= array(
							'#type'					=> 'hidden',
							'#value' 				=>  $user_role_id,
						);
						$form['service_action']['remark_by_role']		= array(
							'#type'					=> 'hidden',
							'#value' 				=>  $user_role,
						);	
						$form['service_action']['remark_by_name']		= array(
							'#type'					=> 'hidden',
							'#value' 				=>  $full_name,
						);
						$form['service_action']['remark_by_employee_id']		= array(
							'#type'					=> 'hidden',
							'#value' 				=>  $hrms_employee_id,
						);
						$form['service_action']['supporting_doc'] = array(
							'#title' => t('Upload Enquiry Report'),
							'#description' => t('Upload Enquiry Report for this claim. (PDF only)'),
							'#type'                 => 'managed_file',
                    		'#upload_validators'     => array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(250 * 1024)),
							'#upload_location'        => 'public://upload/migrantapp/web_temp/',
							'#process'                 => array('import_my_file_element_process'),
							'#attributes'             => array('id' => 'supporting_doc', 'autocomplete' => 'off', 'class' => array('')),
							'#required'             => TRUE,
							'#file_check'           => TRUE
						);
						$form['service_action']['message'] = array(
							'#prefix' => '<div class="col-md-8">',
							'#suffix' => '</div>',
							'#title' => t('Emergency Message'),
							'#type' => 'textarea',
						   // '#required' => TRUE,
							'#attributes' => array('class' => array('form-control'),'maxlength' => 250,'autocomplete' => 'off'),  
						);
						$form['service_action']['submit'] 	= array (
							'#prefix' 			=> '<div class="form-custom col-md-8" id="submit_block">',
							'#suffix' 			=> '</div>',	
							'#type' 			=> 'submit',
							'#attributes' 		=> array('class' => array('btn btn-primary pull-left acremark-btn')),
							'#value' 			=> 'Submit'
						);

						$form['service_action']['action_to_data_2']		= array(
							'#type' 				=> 'markup',
							'#markup'				=> '</div></div>'	
						);

					}else if($claim_details['status'] == 'IR'){
						$form['service_action']['status_data'] 		= array(
							'#type' 		=> 'markup',
							'#markup'		=> '<div class="box box-primary box-solid"><div class="box-header with-border">
												<h3 class="box-title">Action Against This Claim</h3></div>
												<div class="box-body">',
						);
						$form['service_action']['status_markup']		= array(
							'#type' 				=> 'markup',
							'#markup'				=> '<div class="alert alert-info alert-dismissable"><h4><i class="icon fa fa-success"></i> CURRENT STATUS: Enquiry Report Submitted</h4></div>'	
						);
						$form['service_action']['status_data_2']		= array(
							'#type' 				=> 'markup',
							'#markup'				=> '</div></div>'	
						);
					}else if($claim_details['status'] == 'SA'){
						$form['service_action']['status_data'] 		= array(
							'#type' 		=> 'markup',
							'#markup'		=> '<div class="box box-primary box-solid"><div class="box-header with-border">
												<h3 class="box-title">Action Against This Claim</h3></div>
												<div class="box-body">',
						);
						$form['service_action']['status_markup']		= array(
							'#type' 				=> 'markup',
							'#markup'				=> '<div class="alert alert-success alert-dismissable"><h4><i class="icon fa fa-success"></i> CURRENT STATUS: Claim Approved</h4></div>'	
						);
						$form['service_action']['status_data_2']		= array(
							'#type' 				=> 'markup',
							'#markup'				=> '</div></div>'	
						);
					}else if($claim_details['status'] == 'CR'){
						$form['service_action']['status_data'] 		= array(
							'#type' 		=> 'markup',
							'#markup'		=> '<div class="box box-primary box-solid"><div class="box-header with-border">
												<h3 class="box-title">Action Against This Claim</h3></div>
												<div class="box-body">',
						);
						$form['service_action']['status_markup']		= array(
							'#type' 				=> 'markup',
							'#markup'				=> '<div class="alert alert-danger alert-dismissable"><h4><i class="icon fa fa-success"></i> CURRENT STATUS: Claim Rejected</h4></div>'	
						);
						$form['service_action']['status_data_2']		= array(
							'#type' 				=> 'markup',
							'#markup'				=> '</div></div>'	
						);
					}
					
				}else if($user_role == 4){
					
					if($claim_details['status'] == 'IR'){
						$output2 = '<div class="box box-primary">
									<div class="box-body">
									<div class="row">';
						$output2 .= '<div class="col-md-12">

								<div class="panel panel-primary">
								<div class="panel-heading with-border">DETAILS OF THE MIGRANT WORKER</div>
								<div class="panel-body">
									<div class="col-md-12">
									<table class="table table-striped table-responsive admin-custom-table">
										<thead>
											<tr>
												<th width="60%">Enquiry Report submitted by the Inspector</th>
												<th>View Enquiry Report</th>
											</tr>
										</thead>
										<tbody>
										<tr>
											<td height="75">'.$enquiry_details['message'].'</td>
											<td height="75">'.$mj_doc_imw_enquiry.'</td>
										</tr>';
										

										$output2.='</tbody>
									</table>
								</div>
							</div></div></div></div></div></div>';
							
						
							$form['service_action']['markup_data_1'] 		= array(
								'#type' 		=> 'markup',
								'#markup'		=> '<div class="box box-primary box-solid"><div class="box-header with-border">
													<h3 class="box-title">Action Against This Claim</h3></div>
													<div class="box-body">',
							);
							$forward_option = array(
								'' => '- Select -',
								'SI' => 'Send to Inspector For Enquiry',
								'SA' => 'Approve',
								'CR' => 'Reject',
													
							);
						
							$form['service_action']['action_taken']		= array(
								'#prefix' 			=> '<div class="form-custom col-md-8" id="action_taken_replace"><label class="input">',
								'#suffix' 			=> '</label></div>',
								'#title'			=> 'Action Taken',
								'#type'				=> 'select',
								'#options'			=> $forward_option,
								'#attributes'		=> array('class' => array('form-control')),
								'#ajax' => array(
									'event' => 'change',
									'effect' => 'fade',			
									'callback' => 'get_ins_callback',
									'progress' => ''
									)
							);
							$form['service_action']['identification_number']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $main_queary['identification_number'],
							);
							$form['service_action']['ticket_no']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $geo_loc_queary['ticket_no'],
							);
							$form['service_action']['application_id']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $geo_loc_queary['application_id'],
							);
							$form['service_action']['flag_details']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $geo_loc_queary['flag_details'],
							);
							$form['service_action']['flag_code']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $geo_loc_queary['flag_code'],
							);
							$form['service_action']['death_type']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $claim_details['death_type'],
							);
							$form['service_action']['disability_type']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $claim_details['disability_type'],
							);
							$form['service_action']['worksite_id']	= array(
								'#type'		=> 'hidden',
								'#value' 	=>  $geo_loc_queary['worksite_id'],
							);
							$form['service_action']['raised_by']	= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $geo_loc_queary['raised_by'],
							);
							$form['service_action']['remark_by']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $user_role_id,
							);
							$form['service_action']['remark_by_role']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $user_role,
							);	
							$form['service_action']['remark_by_name']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $full_name,
							);
							$form['service_action']['remark_by_employee_id']		= array(
								'#type'					=> 'hidden',
								'#value' 				=>  $hrms_employee_id,
							);
							
							$form['service_action']['forward_to']		= array(
								'#prefix' 			=> '<div id="forward_to_replace">',
								'#suffix' 			=> '</div>'  
							);
							$form['service_action']['message']		= array(
								'#prefix' 			=> '<div id="message_replace">',
								'#suffix' 			=> '</div>'  
							);
							$action_taken = isset($form_state['values']['action_taken']) ? $form_state['values']['action_taken'] : '';
							if($action_taken != "" && !empty($action_taken) && $action_taken == 'SI'){
								$form['service_action']['forward_to']		= array(
									'#prefix' 			=> '<div class="form-custom col-md-8" id="forward_to_replace"><label class="input">',
									'#suffix' 			=> '</label></div>',
									'#title'			=> 'Forward to Inspector',
									'#type'				=> 'select',
									'#options'			=> actions_dropdown_ins($user_role_id,$user_role),
									'#attributes'		=> array('class' => array('form-control')),
									'#ajax' => array(
										'event' => 'change',
										'effect' => 'fade',			
										'callback' => 'ins_callback',
										'progress' => ''
										)
								);
								$form['service_action']['message']		= array(
									'#prefix' 				=> '<div id="message_replace" class="form-custom col-md-8"><section><label class="input">',
									'#suffix' 				=> '</label></section></div>',
									'#title'				=> 'Remarks',
									'#type'					=> 'textarea',
									'#required' 			=> TRUE,
									'#attributes'			=> array('class' => array('form-control')),
									'#rows'					=>  4,
								);
								$imw_user_id = isset($form_state['values']['forward_to']) ? $form_state['values']['forward_to'] : '';
								if(!empty($imw_user_id)){
									$fetch_users_role		= db_select('users_roles', 'ur');
									$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
									$fetch_users_role->leftJoin('l_custom_user_detail', 'lcud', 'ur.uid=lcud.usr_id');
									$fetch_users_role->fields('ro', array('rid'));
									$fetch_users_role->fields('lcud', array('fullname','employee_id'));
									$fetch_users_role->condition('ur.uid', $imw_user_id);
									$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc();
				
									$remark_to_role			=	$fetch_users_role_data['rid'];
									$imw_full_name			=   $fetch_users_role_data['fullname'];
									$imw_employee_id 		= 	$fetch_users_role_data['employee_id'];
				
								}
								
								$form['service_action']['remark_to']		= array(
									'#type'					=> 'hidden',
									'#value' 				=>  $imw_user_id,
								);
								$form['service_action']['remark_to_role']		= array(
									'#type'					=> 'hidden',
									'#value' 				=>  $remark_to_role,
								);	
								$form['service_action']['remark_to_name']		= array(
									'#type'					=> 'hidden',
									'#value' 				=>  $imw_full_name,
								);	
								$form['service_action']['remark_to_employee_id']		= array(
									'#type'					=> 'hidden',
									'#value' 				=>  $imw_employee_id,
								);
								
							}else if($action_taken != "" && !empty($action_taken) && $action_taken == 'SA'){
								$form['service_action']['message']		= array(
									'#prefix' 				=> '<div id="message_replace">',
									'#suffix' 				=> '</div>',
									'#value'    			=> 'Claim has been Approved',
								);
							}else if($action_taken != "" && !empty($action_taken) && $action_taken == 'SR'){
								$form['service_action']['message']		= array(
									'#prefix' 				=> '<div id="message_replace" class="form-custom col-md-8"><section><label class="input">',
									'#suffix' 				=> '</label></section></div>',
									'#title'				=> 'Remarks',
									'#type'					=> 'textarea',
									//'#required' 			=> TRUE,
									'#attributes'			=> array('class' => array('form-control')),
									'#rows'					=>  4,
								);
							}
							$form['service_action']['submit'] 	= array (
								'#prefix' 			=> '<div class="form-custom col-md-8" id="submit_block">',
								'#suffix' 			=> '</div>',	
								'#type' 			=> 'submit',
								'#attributes' 		=> array('class' => array('btn btn-primary pull-left acremark-btn')),
								'#value' 			=> 'Submit'
							);
							$form['service_action']['markup_data_2']		= array(
								'#type' 				=> 'markup',
								'#markup'				=> '</div></div>'	
							);	
					
  
					}else if($claim_details['status'] == 'SI'){

						$form['service_action']['status_data'] 		= array(
							'#type' 		=> 'markup',
							'#markup'		=> '<div class="box box-primary box-solid"><div class="box-header with-border">
												<h3 class="box-title">Action Against This Claim</h3></div>
												<div class="box-body">',
						);
						$form['service_action']['status_markup']		= array(
							'#type' 				=> 'markup',
							'#markup'				=> '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-success"></i> CURRENT STATUS: Send to Inspector</h4></div>'	
						);
						$form['service_action']['status_data_2']		= array(
							'#type' 				=> 'markup',
							'#markup'				=> '</div></div>'	
						);
					}else if($claim_details['status'] == 'SA'){
						$form['service_action']['status_data'] 		= array(
							'#type' 		=> 'markup',
							'#markup'		=> '<div class="box box-primary box-solid"><div class="box-header with-border">
												<h3 class="box-title">Action Against This Claim</h3></div>
												<div class="box-body">',
						);
						$form['service_action']['status_markup']		= array(
							'#type' 				=> 'markup',
							'#markup'				=> '<div class="alert alert-success alert-dismissable"><h4><i class="icon fa fa-success"></i> CURRENT STATUS: Claim Approved</h4></div>'	
						);
						$form['service_action']['status_data_2']		= array(
							'#type' 				=> 'markup',
							'#markup'				=> '</div></div>'	
						);
					}else if($claim_details['status'] == 'CR'){
						$form['service_action']['status_data'] 		= array(
							'#type' 		=> 'markup',
							'#markup'		=> '<div class="box box-primary box-solid"><div class="box-header with-border">
												<h3 class="box-title">Action Against This Claim</h3></div>
												<div class="box-body">',
						);
						$form['service_action']['status_markup']		= array(
							'#type' 				=> 'markup',
							'#markup'				=> '<div class="alert alert-danger alert-dismissable"><h4><i class="icon fa fa-success"></i> CURRENT STATUS: Claim Rejected</h4></div>'	
						);
						$form['service_action']['status_data_2']		= array(
							'#type' 				=> 'markup',
							'#markup'				=> '</div></div>'	
						);
					}
				}
				$form['service_action']['markup_data_3'] = array(
					'#type' => 'markup',
					'#markup' => $output2,
				);
				return $form;
				
}
function service_action_view_submit($form, &$form_state){
	global $base_root, $base_path, $user;
	$form_values = $form_state['values'];
	// echo "<pre>";
	// print_r($form_values);die;
	if($form_values['supporting_doc'] != 0){
		$doc_details             = file_load($form_values['supporting_doc']);
        
		$doc_details_arr         = explode("/", $doc_details->uri);
	    $file_mime        =  mime_content_type($doc_details->uri); // $ok_file = 1; $file_ext = '.pdf';
		
		
		
		$file_ext = '';
		$ok_file = 0;
		if(($file_mime == 'application/pdf')){
			$file_ext = '.pdf';
			$ok_file = 1;
			$file = 'imw_enquiry';
		}
		if (!empty($doc_details->uri) && $ok_file == 1){
			$content         =  file_get_contents(escapefile_url($base_root . $base_path . 'sites/default/files/upload/migrantapp/web_temp/' . $doc_details_arr[5]));
                        
			$doc_details_fname    = time() . '_' . $form_values['application_id'] . '_' .$file.$file_ext;
			$filepathSave = 'sites/default/files/upload/migrantapp/';
			if (!file_exists($filepathSave.$doc_details_fname)) {
				
				$upload_array = array(
					'application_id' => $form_values['application_id'],
					'document_type' => $file,
					'document_filename' => $doc_details_fname,
					'is_active' => '1',
					'ticket_id' 	=> $form_values['ticket_no'],
					'created_date' => date('Y-m-d H:i:s'),
					'flag' => 'IR',  // Enquiry Report
				);

				
				file_put_contents($filepathSave . $doc_details_fname, $content);
				  
				$transaction = db_transaction();
                            try {
                                $insert_id = db_insert('l_migrant_jobseeker_documents')->fields($upload_array)->execute();
                            } catch (Exception $e) {
                                $transaction->rollback();
                                drupal_set_message(t('Unable to upload your file, please try again.'), 'error');
                                return false;
                            }
                            unset($transaction);
			}
		}else {
			//$ok_file = 0;

			drupal_set_message(t('Unable to upload your '.$file.' file, please try again.'), 'error');
		   // return array('status_code' => 200, 'message' => 'Invalid File Type', 'doc_type' => $key, 'path' => NULL);
		}
	}
	

	$insarr = array(
		'application_id' => $form_values['application_id'],
		'mwin_no' 	=> $form_values['identification_number'],
		'status' 	=> $form_values['action_taken'],
		'message' 	=> $form_values['message'],
		'ticket_no' 	=> $form_values['ticket_no'],
		'flag_details' 	=> $form_values['flag_details'],
		'flag_web_app' 	=> 'W',
		'flag_code' 	=> $form_values['flag_code'],
		'flag' 			=> 'S',
		'action_date' 	=> time(),
		//  'present_location' => '',
		//  'longitude' => '',
		'raised_by'		 	=> $form_values['raised_by'],
		'worksite_id'		=> $form_values['worksite_id'],
		'action_by' => $form_values['remark_by'],
		'action_by_role' => $form_values['remark_by_role'],
		'action_by_hrms_id' => $form_values['remark_by_employee_id'],
		'action_to' => $form_values['remark_to'],
		'action_to_role' => $form_values['remark_to_role'],
		'action_to_hrms_id' => $form_values['remark_to_employee_id'],
		'is_active' => 1,
	 );
	 $remarkId = db_insert('l_migrant_jobseeker_action_master')->fields($insarr)->execute();

	 if($remarkId != ''){
		$claim_details=db_select('l_migrant_jobseeker_claim_master','cm')->fields('cm', array())->condition('cm.application_id',$form_values['application_id'])->condition('cm.ticket_no',$form_values['ticket_no'])->condition('cm.is_active',1)->execute();
		if($claim_details-> rowCount() > 0 ){
			$claim = $claim_details->fetchAssoc();
			$claimArray= array(
				'mwin_no' 				=>  $form_values['identification_number'],
				'application_id' 		=>  $form_values['application_id'],		
				'ticket_no'				=>  $form_values['ticket_no'],
				'claim_flag'			=>  $form_values['flag_code'],
				'death_type'			=>  $form_values['death_type'],
				'disability_type'		=>  $form_values['disability_type'],
				'status'				=>  $form_values['action_taken'],
				'action_id'				=>  $remarkId, // action master id
				'is_active'				=>  1,
				//'created_date'			=>  date('Y-m-d h:i:s'),
				'modified_date'			=>  date('Y-m-d h:i:s')
			);
	
			db_update('l_migrant_jobseeker_claim_master')->fields($claimArray)->condition('id',$claim['id'])->execute();
		}
		
	 }
	 drupal_goto('services-details/'.encryption_decryption_fun('encrypt', $form_values['remark_by_role']).'/services/list');
}

function actions_dropdown_ins($user_role_id = '', $user_role = ''){   // *********** inspector list
	global $user, $base_root, $base_path;
		
	$getSubdivision = db_select('l_customuser_relation_address','rel')->fields('rel',array('sub_div_code'))->condition('rel.user_id', $user_role_id)->execute()->fetchObject()->sub_div_code;				
	$getAllAlcUserIds = db_select('l_customuser_relation_address','rel')->fields('rel',array('user_id'))->condition('rel.sub_div_code', $getSubdivision)->condition('rel.block_code',0)->execute();
		if ($getAllAlcUserIds->rowCount() >1){					
			foreach ($getAllAlcUserIds->fetchAll() as $alcuserid){						
				$created_by[] = $alcuserid->user_id;
			} 
		}else{
			$created_by[] = $user_role_id;
			
		}

		$user_query = db_select('users', 'u');
		$user_query->join('l_custom_user_detail', 'cud', 'cud.usr_id = u.uid');
		$user_query->fields('u', array());
		$user_query->fields('cud', array());
		$user_query->orderBy('cud.fullname','ASC');
		$user_query->condition('cud.usr_rid', 7);
		$user_query->condition('cud.created_by', $created_by, 'IN');
		$user_query_result = $user_query->execute()->fetchAll();
	
		foreach ($user_query_result as $ins_details) {
		
			$options[$ins_details->uid] = $ins_details->fullname.' ('.$ins_details->user_place.')';
	
			$default_select = array("" => "- Select -");
			$options = $default_select + $options;
		}
			
	
	return $options;
}
function get_ins_callback($form, $form_state) {
	$commands = array();
	$commands[] = ajax_command_replace('#forward_to_replace', drupal_render($form['service']['forward_to']));
	$commands[] = ajax_command_replace('#message_replace', drupal_render($form['service']['message']));
	return array('#type' => 'ajax', '#commands' => $commands);
}
function ins_callback($form, $form_state) {	

	return $form['service']['remark_to'];				
}

