<?php
function grievance_list($form, &$form_state, $user_role_en = '', $sostype = '', $par = ''){
	global $base_root, $base_path, $user;

	$fetch_users_role = db_select('users_roles', 'ur');
	$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role->fields('ro', array('rid', 'name'));
	$fetch_users_role->condition('ur.uid', $user->uid);
	$fetch_users_role_data = $fetch_users_role->execute()->fetchAssoc();
	$user_role = encryption_decryption_fun('decrypt',$user_role_en);
	$role_name = $fetch_users_role_data['name'];

	$activeGA = $activeG = $activeGP = '';
	
	if($par == 'list'){
		$activeG = 'active';
		$activeGP = '';
		$activeGA == '';
	}else if($par == 'action-taken'){
		$activeGA == 'active';
		$activeG = '';
		$activeGP = '';
	}else if($par == 'inprocess'){
		$activeG = '';
		$activeGA == '';
		$activeGP = 'active';
	}
	
	if ((($user_role == 4) || ($user_role == 5) || ($user_role == 7))) {
   
		$form['grievances_lists']['markup_data'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="nav-tabs-custom">
                                <ul class="nav nav-tabs">
                                    <li class="' . $activeG . '">' . l('Grievance Raised List', 'grievance-details/' . $user_role_en . '/grievance/list') . '</li>
                                    <li class="' . $activeGA . '">' . l('Action Taken', 'grievance-details/' . $user_role_en . '/grievance/action-taken') . '</li>
                                    <li class="' . $activeGP . '">' . l('Inprocess', 'grievance-details/' . $user_role_en . '/grievance/inprocess') . '</li>
                                    
                                </ul>
                                <div class="tab-content">
                                    <div class="tab-pane active" id="fa-icons">
                                    <section id="new"><div class="feedback-scroll">' . grievance_list_details($user_role,$sostype,$par) . '</div></section></div>'
        );


	} else {
		drupal_set_message('You are not authorize to access this page', error);
	}

	return $form;
			


}
function grievance_list_details($user_role = '',$sostype = '', $par = ''){
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
		
		
	$query = db_select('block_mun_master', 'bmm');
	//$query->leftJoin('block_mun_master', 'bmm', 'bmm.block_code=mjm.permanent_areacode');
	$query->fields('bmm', array('block_mun_name'));	
	if ($user_role == 5) { 
		if ($customjurisdiction == 'SUBDIV' && $iscustom_juris == 'Y') {
			$subdv_arr = db_select('l_customuser_reletion_services_wise_user_settings', 'cra')->fields('cra', array('sub_div_code'))->condition('cra.user_id', $user_id)->execute()->fetchObject()->sub_div_code;
			$area_arr = explode(',', $subdv_arr);

		}else {
			$area_arr = db_select('l_customuser_relation_address', 'cra')->fields('cra', array('district_code'))->condition('cra.user_id', $user_id)->execute()->fetchAssoc();
			
		}

	}else if ($user_role == 4) {
		if ($customjurisdiction == 'BMCNS' && $iscustom_juris == 'Y') {
			$block_arr = db_select('l_customuser_reletion_services_wise_user_settings', 'cra')->fields('cra', array('block_code'))->condition('cra.user_id', $user_id)->execute()->fetchObject()->block_code;
			$area_arr = explode(',', $block_arr);
				
		}else{				
			$area_arr = db_select('l_customuser_relation_address', 'cra')->fields('cra', array('sub_div_code'))->condition('cra.user_id', $user_id)->execute()->fetchAssoc();				
			
		}

	}else if ($user_role == 7) {
		$area_arr = db_select('l_customuser_relation_address', 'cra')->fields('cra', array('block_code'))->condition('cra.user_id', $user_id)->execute()->fetchObject()->block_code;			
		
	}
	$query_details = $query->execute()->fetchAll();


	// $curl_post_data = array (
	// 	'source' 		=> 'WBLC',
	// 	'taskid' 		=> 'LCFETCHAPPLICATION',
	// 	'user_role' 	=> $user_role,
	// 	'block_subdiv' 	=> $area_arr,									
	// 	'user_id' 		=> $user_id,
	// 	'status' 		=> $status,							
	// );
	
	
	// 	//$service_url = "http://202.61.117.237:80/karmasathips/rest/wblcofficial";

	// 	$service_url = "https://164.100.199.8:443/rest/wblcofficial";
	// 	$ch = curl_init();
	// 	$headers = array();
	// 	$headers[] = 'Accept: application/json';
	// 	$headers[] = 'Content-Type: application/json';
	// 	$headers[] = 'charset=utf-8';
	// 	curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
	// 	curl_setopt($ch, CURLOPT_URL, $service_url);
	// 	curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
	// 	curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
	// 	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
	// 	curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 0);
	// 	curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($curl_post_data));
	// 	curl_setopt($ch, CURLOPT_POST, true);
	// 	$results    = curl_exec($ch);
	// 	$errors     = curl_error($ch); 
	// 	 //print_r($errors); die;
	// 	curl_close($ch);			
	// 	$results = json_decode(trim($results), TRUE);  //echo '<pre>'; print_r($results); die;	

		
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
	
	// foreach ($result_help as $result_help_details) {
		
	// 	$counter++;
		
	// 	$latitude = $result_help_details->latitude;
	// 	$longitude = $result_help_details->longitude;
	// 	$action_by_role	= $result_help_details->action_by_role; 
	// 	$action_by_id = $result_help_details->action_by; 
		
	// 	$action_by_name = action_official($action_by_id);

	// 	if(!empty($result_help_details->action_to_hrms_id)){
	// 		$action_by_hrms_id 				= '(HRMS-ID-'.$result_help_details->action_by_hrms_id.')';
	// 	}else{
	// 		$action_by_hrms_id = '';
	// 	}

	// 	$action_to_role					= $result_help_details->action_to_role; 
	// 	$action_to_id					= $result_help_details->action_to; 

	// 	if(!empty($result_help_details->action_to_hrms_id)){
	// 		$action_to_hrms_id 				= '(HRMS-ID-'.$result_help_details->action_to_hrms_id.')';
	// 	}else{
	// 		$action_to_hrms_id = '';
	// 	}
		
	// 	$action_to_name = action_official($action_to_id);
		
	// 	$applicant_details 	= $result_help_details->name.'<br>  ('.$result_help_details->mobile.')';
	// 	$application_date 	= date('d-m-Y h:i A', $result_help_details->action_date);

	// 	$query_present_location = db_select('l_migrant_jobseeker_work_details', 'mjwd');
	// 	$query_present_location->fields('mjwd', array('id','present_country','present_state','present_address','present_dist','present_subdivision','present_blockmunname','present_villwardname','present_pin','present_ps','present_po'));
	// 	$query_present_location->condition('mjwd.application_id',$result_help_details->application_id, '=');
	// 	$query_present_location->orderBy('mjwd.id','DESC');
	// 	$result_present_location = $query_present_location->execute()->fetchAssoc();

	// 	if($result_present_location['present_country'] == 1){

	// 		$present_country = 'India';
	// 		$query_state 	 = db_select('state_master','sm');
	// 		$query_state->fields('sm',array());
	// 		$query_state->condition('sm.id',$result_present_location['present_state'],'=');
	// 		$result_state 	 = $query_state->execute()->fetchAssoc();

	// 		$location 		= $result_state['statename'].', '.$present_country;
				
				
	// 	}else{

	// 		$location = $result_present_location['present_address'];
	// 		$present_country = '';
	// 	}

	// 	$getUserStatus = db_select('l_migrant_jobseeker_master','master')->fields('master', array('id','status'))->condition('id',$result_help_details->application_id)->execute()->fetchAssoc();

	// 	if($getUserStatus['status'] == 'P' || $getUserStatus['status'] == 'A'){
	// 		$mwin = $result_help_details->mwin_no.'(P)';
	// 		$link = $base_root . $base_path . 'migrant-worker-details/'.encryption_decryption_fun('encrypt', $result_help_details->application_id) ;
	// 		$mwin_details  = l(t('<span class="badge bg-blue">'.$mwin.'</span>'), $link, array('html' => TRUE));
	// 	}elseif($getUserStatus['status'] == 'C'){
	// 		$mwin_details = $result_help_details->mwin_no;
			
	// 	}
		
	// 		$link_action = $base_root . $base_path . 'migrant-worker-grievance-details/'.encryption_decryption_fun('encrypt', $result_help_details->application_id).'/'.encryption_decryption_fun('encrypt', $result_help_details->id);
	// 		$action  = l(t('<span class="badge bg-blue"><i class="fa fa-eye"> </i> View</span>'), $link_action, array('html' => TRUE));

	// 		$alertType = $result_help_details->flag_details;
			

	// 	$rows[] = array(
	// 		array('data' => $counter, 'align' => 'left', 'class' => array('even')),
	// 		array('data' => $mwin_details, 'align' => 'left', 'class' => array('odd')),
	// 		array('data' => $applicant_details, 'align' => 'left', 'class' => array('odd')),	
	// 		array('data' => $alertType, 'align' => 'left', 'class' => array('odd')),
	// 		array('data' => $result_help_details->ticket_no, 'align' => 'left', 'class' => array('even')),
	// 		array('data' => $application_date, 'align' => 'left', 'class' => array('odd')),
	// 		array('data' => $action, 'align' => 'left', 'class' => array('even')),		
			
	// 	);

		

	// }
	$variables = array(
		'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')),
		'header' => $header,
		'rows' => $rows,
		'empty' => t("No data found!")
	);
	
	$output = theme('datatable', $variables);
		
		
	return $output;
			


}
function karmasathi_grievance_view($form, &$form_state, $id_en = '',$action_id_en = ''){
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
			<div class="panel-heading with-border">GRIEVANCE DETAILS</div>
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
							<td height="75"> Grievance </td>
							<td height="75">'.$geo_loc_queary['flag_details'].'</td>
						</tr>
						<tr>
							<td height="75"> Grievance Details</td>
							<td height="75">'.$geo_loc_queary['message'].'</td>
						</tr>
						<tr>
							<td height="75"> Ticket Details </td>
							<td height="75">'.$geo_loc_queary['ticket_no'].'</td>
						</tr>
						<tr>
							<td height="75"> Grievance Raised Date </td>
							<td height="75">'.date('dS M, Y', $geo_loc_queary['action_date']).'</td>
						</tr>
						';

					

					$output.='</tbody>
					</table>
				</div>

				</div></div></div></div></div></div>';

				$form['grievance']['markup_data'] = array(
					'#type' => 'markup',
					'#markup' => $output,
				);
				// if (($user_role == 4)){
				// 	if($geo_loc_queary['status'] == 'GR'){
				// 		$form['grievance']['markup_data_1'] 		= array(
				// 			'#type' 		=> 'markup',
				// 			'#markup'		=> '<div class="box box-primary box-solid"><div class="box-header with-border">
				// 								<h3 class="box-title">Action Against This Grievance</h3></div>
				// 								<div class="box-body">',
				// 		);
				// 			$forward_option = array(
				// 				'' => '- Select -',
				// 				'AI' => 'Action Initiated',
				// 				'AT' => 'Action Taken',
													
				// 			);
						
						
					
				// 			$form['grievance']['action_taken']		= array(
				// 				'#prefix' 			=> '<div class="form-custom col-md-8" id="action_taken_replace"><label class="input">',
				// 				'#suffix' 			=> '</label></div>',
				// 				'#title'			=> 'Action Taken',
				// 				'#type'				=> 'select',
				// 				'#options'			=> $forward_option,
				// 				'#attributes'		=> array('class' => array('form-control')),
				// 				'#ajax' => array(
				// 					'event' => 'change',
				// 					'effect' => 'fade',			
				// 					'callback' => 'action_ini_tak',
				// 					'progress' => ''
				// 					)
				// 			);
				// 			$form['grievance']['identification_number']		= array(
				// 				'#type'					=> 'hidden',
				// 				'#value' 				=>  $geo_loc_queary['identification_number'],
				// 			);
				// 			$form['grievance']['ticket_no']		= array(
				// 				'#type'					=> 'hidden',
				// 				'#value' 				=>  $geo_loc_queary['ticket_no'],
				// 			);
				// 			$form['grievance']['application_id']		= array(
				// 				'#type'					=> 'hidden',
				// 				'#value' 				=>  $geo_loc_queary['application_id'],
				// 			);
				// 			$form['grievance']['flag_details']		= array(
				// 				'#type'					=> 'hidden',
				// 				'#value' 				=>  $geo_loc_queary['flag_details'],
				// 			);
				// 			$form['grievance']['flag_code']		= array(
				// 				'#type'					=> 'hidden',
				// 				'#value' 				=>  $geo_loc_queary['flag_code'],
				// 			);
				// 			$form['grievance']['raised_by']	= array(
				// 				'#type'					=> 'hidden',
				// 				'#value' 				=>  $geo_loc_queary['raised_by'],
				// 			);
				// 			$form['grievance']['worksite_id']	= array(
				// 				'#type'					=> 'hidden',
				// 				'#value' 				=>  $geo_loc_queary['worksite_id'],
				// 			);
				// 			$form['grievance']['remark_by']		= array(
				// 				'#type'					=> 'hidden',
				// 				'#value' 				=>  $user_role_id,
				// 			);
				// 			$form['grievance']['remark_by_role']		= array(
				// 				'#type'					=> 'hidden',
				// 				'#value' 				=>  $user_role,
				// 			);	
				// 			$form['grievance']['remark_by_name']		= array(
				// 				'#type'					=> 'hidden',
				// 				'#value' 				=>  $full_name,
				// 			);
				// 			$form['grievance']['remark_by_employee_id']		= array(
				// 				'#type'					=> 'hidden',
				// 				'#value' 				=>  $hrms_employee_id,
				// 			);
				// 			$form['grievance']['message']		= array(
				// 				'#prefix' 			=> '<div id="message_replace">',
				// 				'#suffix' 			=> '</div>'  
				// 			);
				// 			$action_taken = isset($form_state['values']['action_taken']) ? $form_state['values']['action_taken'] : '';
				// 			if($action_taken != "" && !empty($action_taken) && $action_taken == 'AT'){
				// 				$form['grievance']['message']		= array(
				// 					'#prefix' 				=> '<div id="message_replace" class="form-custom col-md-8"><section><label class="input">',
				// 					'#suffix' 				=> '</label></section></div>',
				// 					'#title'				=> 'Remarks',
				// 					'#type'					=> 'textarea',
				// 					'#required' 			=> TRUE,
				// 					'#attributes'			=> array('class' => array('form-control')),
				// 					'#rows'					=>  4,
				// 				);
				// 			}else if($action_taken != "" && !empty($action_taken) && $action_taken == 'AI'){
				// 				$form['grievance']['message']		= array(
				// 					'#prefix' 				=> '<div id="message_replace">',
				// 					'#suffix' 				=> '</div>',
				// 					'#default_value'    	=> 'Action Initiated For Your Grievance',
				// 				);
				// 			}
				// 			$form['grievance']['submit'] 	= array (
				// 				'#prefix' 			=> '<div class="form-custom col-md-8" id="submit_block">',
				// 				'#suffix' 			=> '</div>',	
				// 				'#type' 			=> 'submit',
				// 				'#attributes' 		=> array('class' => array('btn btn-primary pull-left acremark-btn')),
				// 				'#value' 			=> 'Submit'
				// 			);
				// 			$form['grievance']['markup_data_2']		= array(
				// 				'#type' 				=> 'markup',
				// 				'#markup'				=> '</div></div>'	
				// 			);	
				// 	}
				// } 
				return $form;

}

function karmasathi_grievance_view_submit($form, &$form_state){
	$form_values = $form_state['values'];
	// echo "<pre>";
	// print_r($form_values);die;
	$insarr = array(
		'application_id' => $form_values['application_id'],
		'mwin_no' 	=> $form_values['identification_number'],
		'status' 	=> $form_values['action_taken'],
		'message' 	=> $form_values['message'],
		'ticket_no' 	=> $form_values['ticket_no'],
		'flag_details' 	=> $form_values['flag_details'],
		'flag_web_app' 	=> 'W',
		'flag_code' 	=> $form_values['flag_code'],
		'flag' 			=> 'G',
		'action_date' 	=> time(),
		//  'present_location' => '',
		//  'longitude' => '',
		'raised_by'		 	=> $form_values['raised_by'],
		'worksite_id'		=> $form_values['worksite_id'],
		'action_by' => $form_values['remark_by'],
		'action_by_role' => $form_values['remark_by_role'],
		'action_by_hrms_id' => $form_values['remark_by_employee_id'],
		'action_to' => 0,
		'action_to_role' => 0,
		'action_to_hrms_id' => 0,
		'is_active' => 1,
	 );
	 db_insert('l_migrant_jobseeker_action_master')->fields($insarr)->execute();
}
function action_ini_tak($form, $form_state) {
	$commands = array();
	$commands[] = ajax_command_replace('#message_replace', drupal_render($form['grievance']['message']));
	return array('#type' => 'ajax', '#commands' => $commands);
}
