<?php
function emergency_details($form, &$form_state, $user_role_en = '', $sostype = '', $par = ''){
	global $base_root, $base_path, $user;

	$fetch_users_role = db_select('users_roles', 'ur');
	$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role->fields('ro', array('rid', 'name'));
	$fetch_users_role->condition('ur.uid', $user->uid);
	$fetch_users_role_data = $fetch_users_role->execute()->fetchAssoc();
	$user_role = encryption_decryption_fun('decrypt',$user_role_en);
	$role_name = $fetch_users_role_data['name'];

	$activeEA = $activeE = $activeEP = '';
	
	if($par == 'list'){
		$activeE = 'active';
		$activeEP = '';
		$activeEA == '';
	}else if($par == 'action-taken'){
		$activeEA == 'active';
		$activeE = '';
		$activeEP = '';
	}else{
		$activeE = '';
		$activeEA == '';
		$activeEP = 'active';
	}
 	
	
	if ((($user_role == 4) || ($user_role == 5) || ($user_role == 7))) {
  
		$form['grievances_lists']['markup_data'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="nav-tabs-custom">
                                <ul class="nav nav-tabs">
                                    <li class="' . $activeE . '">' . l('Emergency Raised List', 'emergency-details/' . $user_role_en . '/emergency/list') . '</li>
                                    <li class="' . $activeEA . '">' . l('Action Taken', 'emergency-details/' . $user_role_en . '/emergency/action-taken') . '</li>
                                </ul>
                                <div class="tab-content">
                                    <div class="tab-pane active" id="fa-icons">
                                    <section id="new"><div class="feedback-scroll">' . emergency_list_claim($user_role,$sostype,$par) . '</div></section></div>'
        );


	} else {
		drupal_set_message('You are not authorize to access this page', error);
	}

	return $form;
			


}
function emergency_list_claim($user_role = '',$sostype = '', $par = ''){
	global $base_root, $base_path, $user;
	
	$table = $apply_category = $unit_name = $actionStatus = '';
	$user_id = $user->uid;

	$counter = 0;
	

	$fetch_users_role = db_select('users_roles', 'ur');
	$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role->fields('ro', array('rid', 'name'));
	$fetch_users_role->condition('ur.uid', $user_id);
	$fetch_users_role_data = $fetch_users_role->execute()->fetchAssoc();
	
	$user_role = $fetch_users_role_data['rid'];
	$role_name = $fetch_users_role_data['name'];

	
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
		
		$query = db_select('l_migrant_jobseeker_master', 'mjm');
		$query->leftJoin('block_mun_master', 'bmm', 'bmm.block_code=mjm.permanent_areacode');
		$query->fields('mjm', array('identification_number','mobile','status','name','emergency_contact_no'));			
			
	
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
				

			}
			

			$query->leftJoin('l_migrant_jobseeker_action_master', 'mjam', 'mjam.application_id=mjm.id');
			$query->fields('mjam', array());	
			$query->condition('mjm.status', 'I', '!=');
			$query->condition('mjm.status', '', '!=');
            $query->condition('mjam.flag', 'E', '=');
			if($par == 'list'){
				$query->condition('mjam.status', 'ER', '=');
			}else if ($par == 'action-taken'){
				$query->condition('mjam.status', 'AT', '=');
			}
			$query->condition('mjam.is_active', 1);			
			$query->orderBy('mjam.id','DESC');
			$result_help = $query->execute()->fetchAll();
			
			$counter = 0;
			$header = array(
				array('data' => 'Sl. No.', 'width' => '6%', 'data-orderable' => false),
				array('data' => 'MWIN', 'width' => '13%'),
				array('data' => 'Worker Details', 'width' => '12%'),
				array('data' => 'Alert Type', 'width' => '12%'),
				array('data' => 'Ticket-Id', 'width' => '12%'),	
				array('data' => 'Date', 'width' => '15%'),	
				array('data' => 'Action', 'width' => '5%')
			);
				
				foreach ($result_help as $result_help_details) {
					
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
					
					$applicant_details 	= $result_help_details->name.'<br>  ('.$result_help_details->mobile.')';
					$application_date 	= date('d-m-Y h:i A', $result_help_details->action_date);
	
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
					//echo $result_help_details->application_id.'----'.$result_help_details->id;die;
						$link_action = $base_root . $base_path . 'migrant-worker-emergency-details/'.encryption_decryption_fun('encrypt', $result_help_details->application_id).'/'.encryption_decryption_fun('encrypt', $result_help_details->id);
						$action  = l(t('<span class="badge bg-blue"><i class="fa fa-eye"> </i> View</span>'), $link_action, array('html' => TRUE));
	
						$alertType = $result_help_details->flag_details;
						
	
					$rows[] = array(
						array('data' => $counter, 'align' => 'left', 'class' => array('even')),
						array('data' => $mwin_details, 'align' => 'left', 'class' => array('odd')),
						array('data' => $applicant_details, 'align' => 'left', 'class' => array('odd')),	
						array('data' => $alertType, 'align' => 'left', 'class' => array('odd')),
						array('data' => $result_help_details->ticket_no, 'align' => 'left', 'class' => array('even')),
						array('data' => $application_date, 'align' => 'left', 'class' => array('odd')),
						array('data' => $action, 'align' => 'left', 'class' => array('even')),		
						
					);
	
				}
				$variables = array(
					'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')),
					'header' => $header,
					'rows' => $rows,
					'empty' => t("No data found!")
				);
				
				$output = theme('datatable', $variables);
			/*}else if($par == 'action-taken'){
				
			}*/
			
		return $output;
			


}
function migrant_worker_emergency_view($form, &$form_state, $id_en = '',$action_id_en = ''){
	
	global $base_root, $base_path, $user;
	
	$user_id   = $user->uid;
	
	$id 		= encryption_decryption_fun('decrypt', $id_en);
	$action_id  = encryption_decryption_fun('decrypt', $action_id_en);
	
	$fetch_users_role = db_select('users_roles', 'ur');
	$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role->fields('ro', array('rid', 'name'));
	$fetch_users_role->condition('ur.uid', $user_id);
	$fetch_users_role_data = $fetch_users_role->execute()->fetchAssoc();

	
	$user_role = $fetch_users_role_data['rid'];
	$role_name = $fetch_users_role_data['name'];
  
    $main_queary = db_select('l_migrant_jobseeker_master','master')->fields('master', array('name','identification_number','emergency_contact_no','mobile'))->condition('master.id',$id)->execute()->fetchAssoc();

	$geo_loc_queary = db_select('l_migrant_jobseeker_action_master','mjam')->fields('mjam', array())->condition('mjam.application_id',$id)->condition('mjam.id',$action_id)->condition('mjam.is_active',1)->execute()->fetchAssoc();
	
    $document_queary=db_select('l_migrant_jobseeker_documents','document')->fields('document', array())->condition('document.application_id',$id)->condition('document.document_type','audio')->condition('document.flag','E')->condition('document.is_active',1)->execute()->fetchAssoc();
	
    if($geo_loc_queary['raised_by'] == 'O'){
        
        $raised_by_queary = db_select('l_migrant_jobseeker_alertbyothers_info','alertbyothers')->fields('alertbyothers', array())->condition('alertbyothers.ticket_no',$geo_loc_queary['ticket_no'])->condition('alertbyothers.application_id',$id)->execute()->fetchAssoc();
        $raised_by = $raised_by_queary['relationship'].' ('.$raised_by_queary['mobile'].')';
    }else{
        $raised_by = 'Self';
    }

    $link 					= $base_root . $base_path . 'migrant-worker-details/' . encryption_decryption_fun('encrypt', $id) ;
	$identification_number  = l(t('<span class="badge bg-blue">'.$main_queary['identification_number'].'</span>'), $link, array('html' => TRUE));	
			
			
	$link_action 			= $base_root . $base_path . 'download-audio/'.$document_queary['document_filename'];
	$action  				= l(t('<span class="badge bg-blue"><i class="fa fa-download"> </i> Download</span>'), $link_action, array('html' => TRUE));	
			
	$output = '<div class="box box-primary">
	   					<div class="box-body">
						   <div class="row">';
	$output .= '<div class="col-md-6">
				<div class="panel panel-primary">
				<div class="panel-heading with-border">DETAILS OF THE MIGRANT WORKER</div>
				<div class="panel-body">
					<div class="col-md-12">
						<table class="table table-bordered table-responsive">						
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
			<div class="panel-heading with-border">EMERGENCY DETAILS - Ticket ID : '.$geo_loc_queary['ticket_no'].'</div>
			<div class="panel-body">
				<div class="col-md-12">
					<table class="table table-bordered table-responsive">							
					<tbody>
						<tr>
							<td height="75"> Emergency Details </td>
							<td height="75">'.$geo_loc_queary['flag_details'].'<br>'.$geo_loc_queary['message'].'</td>
						</tr>
						<tr>
							<td height="75"> Emergency Raised By </td>
							<td height="75">'.$raised_by.'</td>
						</tr>
						<tr>
							<td height="75"> Emergency Raised Location </td>
							<td height="75">'.$geo_loc_queary['present_location'].'</td>
						</tr>
						
						
						<tr>
							<td height="75"> Emergency  voice message</td>
							<td height="75">
							<p>'.$action.'</p>
							</td>
						</tr>
						';
						
					

		$output.='</tbody></table></div></div></div></div></div></div></div>';

	$form['emergency']['markup_data'] = array(
										'#type' 	 => 'markup',
										'#markup' 	 => $output,
										);
	// if (($user_role == 4) && $geo_loc_queary['status'] =='ER'){
		
	// 	$form['emergency']['identification_number']	= array(
	// 									'#type'		 => 'hidden',
	// 									'#value' 	 =>  $geo_loc_queary['identification_number'],
	// 								);
	// 	$form['emergency']['ticket_no']	= array(
	// 									'#type'		 => 'hidden',
	// 									'#value' 	 =>  $geo_loc_queary['ticket_no'],
	// 								);
	// 	$form['emergency']['application_id']  = array(
	// 									'#type'		=> 'hidden',
	// 									'#value' 	=>  $geo_loc_queary['application_id'],
	// 								);
	// 	$form['emergency']['flag_details']	= array(
	// 									'#type'		=> 'hidden',
	// 									'#value' 	=>  $geo_loc_queary['flag_details'],
	// 								);
	// 	$form['emergency']['flag_code']	= array(
	// 									'#type'		=> 'hidden',
	// 									'#value' 	=>  $geo_loc_queary['flag_code'],
	// 								);
	// 	$form['emergency']['raised_by']	= array(
	// 									'#type'		=> 'hidden',
	// 									'#value' 	=>  $geo_loc_queary['raised_by'],
	// 								);
	// 	$form['emergency']['worksite_id']	= array(
	// 									'#type'		=> 'hidden',
	// 									'#value' 	=>  $geo_loc_queary['worksite_id'],
	// 								);
	// 	$form['emergency']['markup_data_1'] = array(
	// 									'#type' 	  => 'markup',
	// 									'#markup'	  => '<div class="box box-default box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">ACTIONS AND REMARKS</h3></div><div class="box-body"><span style="margin:0; width:100%;">',
	// 								);
				
	// 	$form['emergency']['action_taken']	= array(
	// 									'#prefix' 	  => '<div class="col-md-3">&nbsp;</div><div class="form-custom col-md-6" id="action_taken_replace"><label class="input">',
	// 									'#suffix' 	  => '<i></i></label>',
	// 									'#title'	  => 'Please Select Actions',
	// 									'#type'		  => 'select',
	// 									'#required'   =>  TRUE,
	// 									'#options'	  => array('' => '- Select -','AT' => 'Action Taken'), //'AI' => 'Action Initiated',
	// 									'#attributes' => array('class' => array('form-control')),
	// 									'#ajax' 	  => array(
	// 														'event' 	=> 'change',
	// 														'effect' 	=> 'fade',			
	// 														'callback'  => 'action_ini_tak',
	// 														'progress'  => array('type'=> 'throbber'),
	// 														)
	// 									);
	// 	$action_taken = isset($form_state['values']['action_taken']) ? $form_state['values']['action_taken'] : '';
		
	// 	$form['emergency']['letter']	= array(
	// 									'#prefix' 	=> '<div id="letter_replace">',
	// 									'#suffix' 	=> '</div>'  
	// 								);
		
	// 	$form['emergency']['message']	= array(
	// 									'#prefix' 	=> '<div id="message_replace">',
	// 									'#suffix' 	=> '</div>'  
	// 								);
						
	// 	if($action_taken != "" && !empty($action_taken) && $action_taken == 'AT'){
			
	// 		$form['emergency']['letter'] = array(
	// 				 					'#prefix'	=> '<div id="letter_replace">',
	// 				 					'#markup' 	=> l(t('<i class="fa fa-download" aria-hidden="true"></i>
	// 				 	Download Official Letter'),'download-letter/'.encryption_decryption_fun('encrypt', $id).'/'.encryption_decryption_fun('encrypt',$geo_loc_queary['ticket_no']), array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn')))),
	// 				 					'#suffix' 	=> '</div><div class="clear"></div>',
	// 									);
			
			
	// 		$form['emergency']['message']		= array(
	// 									'#prefix' 		=> '<div id="message_replace" class="form-custom"><label class="input">',
	// 									'#suffix' 		=> '</label></div>',
	// 									'#title'		=> 'Remarks',
	// 									'#type'			=> 'textarea',
	// 									'#required' 	=> TRUE,
	// 									'#attributes'	=> array('class' => array('form-control')),
	// 									'#rows'			=>  2,
	// 								);
	// 	}/*else if($action_taken != "" && !empty($action_taken) && $action_taken == 'AI'){
	// 		$form['emergency']['message']		= array(
	// 			'#prefix' 				=> '<div id="message_replace">',
	// 			'#suffix' 				=> '</div>',
	// 			'#default_value'    	=> 'Action Initiated For Your Emergency',
	// 		);
	// 	}*/
	// 	$form['emergency']['submit'] 	= array (
	// 									'#prefix' 		=> '<div id="submit_block">',
	// 									'#suffix' 		=> '</div></div><div class="col-md-3">&nbsp;</div></div></div>',	
	// 									'#type' 		=> 'submit',
	// 									'#attributes' 	=> array('class' => array('btn btn-primary pull-left acremark-btn')),
	// 									'#value' 		=> 'Submit'
	// 								);
	// 	$form['emergency']['markup_data_2']		= array(
	// 									'#type' 		=> 'markup',
	// 									'#markup'		=> '</div></div>'	
	// 								);	
		
	// } 
	$form['emergency']['markup_data_3'] = array(
										'#type' 	 => 'markup',
										'#markup' 	 => '<div class="callout callout-warning"><h4>CURRENT STATUS: Pending </h4><p>'.$geo_loc_queary['message'].'.</div>',
										);
	
	
	return $form;

}
function migrant_worker_emergency_view_submit($form, &$form_state){
	
	global $base_root, $base_path, $user;
	
	$user_id 	 = $user->uid;
	$form_values = $form_state['values']; 
	
	/** Get User role of ALC ***/
	
	$fetch_users_role = db_select('users_roles', 'ur');
	$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role->leftJoin('l_custom_user_detail', 'lcud', 'ur.uid=lcud.usr_id');
	$fetch_users_role->fields('ro', array('rid'));
	$fetch_users_role->fields('lcud', array('fullname','employee_id'));
	$fetch_users_role->condition('ur.uid', $user_id);
	$fetch_users_role_data = $fetch_users_role->execute()->fetchAssoc(); 

	$remark_by_role	= $fetch_users_role_data['rid'];
	$alc_full_name	= $fetch_users_role_data['fullname'];
	$employeeId	= $fetch_users_role_data['employee_id'];

	
	$insarr = array(
		'application_id' 	=> $form_values['application_id'],
		'mwin_no' 		 	=> $form_values['identification_number'],
		'ticket_no' 	 	=> $form_values['ticket_no'],
		'status' 		 	=> $form_values['action_taken'],
		'message' 			=> $form_values['message'],
		'flag' 			 	=> 'E',
		'flag_details' 		=> $form_values['flag_details'],
		'raised_by'		 	=> $form_values['raised_by'],
		'action_date' 		=> time(),
		'flag_web_app' 		=> 'W',
		'flag_code' 		=> $form_values['flag_code'],
		'action_by' 		=> $user_id,
		'action_by_role'	=> $remark_by_role,
		'action_by_hrms_id' => $employeeId,
		'worksite_id'		=> $form_values['worksite_id'],
		/*'action_to' 		=> 0,
		'action_to_role' 	=> 0,
		'action_to_hrms_id' => 0,*/
		'is_active' 		=> 1,
	 );
	
	 db_update('l_migrant_jobseeker_action_master')->fields(array('is_active'=>0))->condition('ticket_no', $form_values['ticket_no'])->condition('is_active', 1)->execute();
	 db_insert('l_migrant_jobseeker_action_master')->fields($insarr)->execute();
	 drupal_goto('emergency-details/' . encryption_decryption_fun('encrypt',$remark_by_role) . '/emergency/action-taken');
}
function download_audio($file){
	$filepath = $base_root.$base_path.'sites/default/files/upload/migrantapp/audio/'.($file);
	if (file_exists($filepath)) {
		header('Content-Description: File Transfer');
		header('Content-Type: application/octet-stream');
		header('Content-Disposition: attachment; filename="' . basename($filepath) . '"');
		header('Expires: 0');
		header('Cache-Control: must-revalidate');
		header('Pragma: public');
		header('Content-Length: ' . filesize($filepath));
		flush(); // Flush system output buffer
		readfile($filepath);
		die();
	  } else {
		http_response_code(404);
		die();
	  }
}
function action_ini_tak($form, $form_state) {
	$commands = array();
	$commands[] = ajax_command_replace('#letter_replace', drupal_render($form['emergency']['letter']));
	$commands[] = ajax_command_replace('#message_replace', drupal_render($form['emergency']['message']));
	return array('#type' => 'ajax', '#commands' => $commands);
}
