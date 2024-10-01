<?php
drupal_add_js(drupal_get_path('module', 'alc') . '/js/mycustom.js');
drupal_add_css(drupal_get_path('module', 'addlc') . '/css/style.css');

function karmasathi_migrant_list($form, &$form_state, $user_role_en = '', $par = '')
{


	// http://202.61.117.237/wblc/official-karmasathi-list/dXhyN2pCbmNDUTBld2VDWW1JczZJdz09/pending
	global $base_root, $base_path, $user;
	$fetch_users_role = db_select('users_roles', 'ur');
	$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role->fields('ro', array('rid', 'name'));
	$fetch_users_role->condition('ur.uid', $user->uid);
	$fetch_users_role_data = $fetch_users_role->execute()->fetchAssoc();
	$user_role = encryption_decryption_fun('decrypt', $user_role_en);
	//$user_role = $user_role_en;

	$role_name = $fetch_users_role_data['name'];

	$active = $activeIncomplete = $activePen = '';

	if ($par == 'approved') {
		$active = 'active';
		$activeIncomplete = '';
		$activeBack = '';
		$activeRej = '';
		$exlName = 'APPROVED APPLICATION';
	} else if ($par == 'provisional') {
		$activeIncomplete = 'active';
		$active = '';
		$activeBack = '';
		$activeRej = '';
		$exlName = 'PROVISIONAL APPLICATION';
	} else if ($par == 'pending') {
		$activePen = 'active';
		$active = '';
		$activeIncomplete = '';
		$activeBack = '';
		$activeRej = '';
		$exlName = 'PENDING APPLICATION';
	} else if ($par == 'back') {
		$activePen = '';
		$active = '';
		$activeIncomplete = '';
		$activeBack = 'active';
		$activeRej = '';
		$exlName = 'BACK FOR CORRECTION APPLICATION';
	} else if ($par == 'reject') {
		$activePen = '';
		$active = '';
		$activeIncomplete = '';
		$activeBack = '';
		$activeRej = 'active';
		$exlName = 'REJECTED APPLICATION';
	}


	$back_link = $base_root . $base_path . 'official-karmasathi-list-xls/' . $user_role_en . '/' . $par;
	$back = l(t('<span class="btn btn-success pull-right"><i class="fa fa-file-excel-o"></i> DOWNLOAD LIST OF ' . $exlName . '</span>'), $back_link, array('html' => TRUE));


	if ((($user_role == 4) || ($user_role == 12) || ($user_role == 7) || ($user_role == 5))) {

		$form['application_lists']['markup_data1'] = array(
			'#type' => 'markup',
			'#markup' => '<div class="nav-tabs-custom">
								<ul class="nav nav-tabs">
									<li class="' . $active . '">' . l('Approved Worker List', 'official-karmasathi-list/' . $user_role_en . '/approved') . '</li>
									<li class="' . $activePen . '">' . l('Pending for Approval', 'official-karmasathi-list/' . $user_role_en . '/pending') . '</li>
									<li class="' . $activeIncomplete . '">' . l('Provisional Application', 'official-karmasathi-list/' . $user_role_en . '/provisional') . '</li>
									<li class="' . $activeBack . '">' . l('Back for Correction', 'official-karmasathi-list/' . $user_role_en . '/back') . '</li>
									<li class="' . $activeRej . '">' . l('Rejected Application', 'official-karmasathi-list/' . $user_role_en . '/reject') . '</li>
								</ul>
								<div class="tab-content">
									<div class="tab-pane active" id="fa-icons">
									<section id="new"><div class="feedback-scroll"><br><br>' . $back . '<br><br>' . get_karmasathi_migrant_lists($user_role, $par) . '</div></section></div>'
		);


	} else {
		drupal_set_message('You are not authorize to access this page', error);
	}

	return $form;

}
function get_karmasathi_migrant_lists($usertype = '', $par = '')
{
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

		} else {
			$area_arr = db_select('l_customuser_relation_address', 'cra')->fields('cra', array('district_code'))->condition('cra.user_id', $user_id)->execute()->fetchAssoc();

		}

	} else if ($user_role == 4) {
		if ($customjurisdiction == 'BMCNS' && $iscustom_juris == 'Y') {
			$block_arr = db_select('l_customuser_reletion_services_wise_user_settings', 'cra')->fields('cra', array('block_code'))->condition('cra.user_id', $user_id)->execute()->fetchObject()->block_code;
			$area_arr = explode(',', $block_arr);

		} else {
			$area_arr = db_select('l_customuser_relation_address', 'cra')->fields('cra', array('sub_div_code'))->condition('cra.user_id', $user_id)->execute()->fetchAssoc();

		}

	} else if ($user_role == 7) {
		$area_arr = db_select('l_customuser_relation_address', 'cra')->fields('cra', array('block_code'))->condition('cra.user_id', $user_id)->execute()->fetchObject()->block_code;

	}

	//print_r($area_arr);die;
	$query_details = $query->execute()->fetchAll();

	if ($par == 'approved') {
		$status = array('C');
	} elseif ($par == 'pending') {
		$status = array('A', 'BA');
	} elseif ($par == 'provisional') {
		$status = array('P', 'BP');
	} elseif ($par == 'back') {
		$status = array('B');
	} elseif ($par == 'reject') {
		$status = array('R');
	}
	// $curl_post_data = array (
	// 	'source' 		=> 'WBLC',
	// 	'taskid' 		=> 'LCFETCHAPPLICATION',
	// 	'user_role' 	=> $user_role,
	// 	'block_subdiv' 	=> $area_arr,									
	// 	'user_id' 		=> $user_id,
	// 	'status' 		=> $status,							
	// );
	$curl_post_data = array(
		'source' => 'WBLC',
		'taskid' => 'LCFETCHAPPLICATION',
		'user_role' => $user_role,
		'block_subdiv' => $area_arr,
		'user_id' => $user_id,
		'status' => $status
	);
	//print_r(json_encode($curl_post_data));die;
	//$service_url = "http://202.61.117.237:80/karmasathips/rest/wblcofficial";

	$service_url = "https://164.100.199.8:443/rest/wblcofficial";
	$ch = curl_init();
	$headers = array();
	$headers[] = 'Accept: application/json';
	$headers[] = 'Content-Type: application/json';
	$headers[] = 'charset=utf-8';
	curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
	curl_setopt($ch, CURLOPT_URL, $service_url);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
	curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
	curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 0);
	curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($curl_post_data));
	curl_setopt($ch, CURLOPT_POST, true);
	$results = curl_exec($ch);
	$errors = curl_error($ch);
	//print_r($errors); die;
	curl_close($ch);
	$results = json_decode($results, TRUE);
	//echo '<pre>'; print_r($results); die('ok');

	//print_r($results['content']);die;
	if ($results['status_code'] == 200) {

		$counter = 0;
		$header = array(
			array('data' => 'Sl. No.', 'width' => '6%', 'data-orderable' => false),
			array('data' => 'MWIN', 'width' => '10%'),
			array('data' => 'Worker Name', 'width' => '15%'),
			array('data' => 'Mobile Number', 'width' => '10%'),
			array('data' => 'Status', 'width' => '8%'),
			array('data' => 'action', 'width' => '8%'),
		);

		foreach ($results['content'] as $jobseeker_data) {
			$application_id = $jobseeker_data['id'];

			$name = $jobseeker_data['name'];
			$mobile = $jobseeker_data['mobile'];
			$status = $jobseeker_data['status'];

			if ($jobseeker_data['status'] == 'C') {
				$status_show = 'Approved';
				$identification_number = $jobseeker_data['identification_number'];
			} elseif ($jobseeker_data['status'] == 'P' || $jobseeker_data['status'] == 'BP') {
				$status_show = 'Provisional';
				$identification_number = $jobseeker_data['identification_number'] . '(P)';
			} elseif ($jobseeker_data['status'] == 'A' || $jobseeker_data['status'] == 'BA') {
				$status_show = 'Pending for approval';
				$identification_number = $jobseeker_data['identification_number'];
			} elseif ($jobseeker_data['status'] == 'B') {
				$status_show = 'Back for correction';
				$identification_number = $jobseeker_data['identification_number'];
			} elseif ($jobseeker_data['status'] == 'R') {
				$status_show = 'Reject';
				$identification_number = $jobseeker_data['identification_number'];
			}

			$link = $base_root . $base_path . 'karmasathi-migrant-details/' . encryption_decryption_fun('encrypt', $application_id);
			$view_details = l(t('<span class="badge bg-blue"><i class="fa fa-eye"></i> View Details </span>'), $link, array('html' => TRUE));
			// $apply_date = date('dS M, Y', strtotime($jobseeker_data->modified_date));
			// $migrated_on = date('dS M, Y', strtotime($jobseeker_data->migrated_on));

			$counter++;
			$rows[] = array(
				array('data' => $counter, 'align' => 'left', 'class' => array('even')),
				array('data' => $identification_number, 'align' => 'left', 'class' => array('odd')),
				array('data' => $name, 'align' => 'left', 'class' => array('odd')),
				//array('data' => , 'align' => 'left', 'class' => array('even')),
				array('data' => $mobile, 'align' => 'left', 'class' => array('odd')),
				array('data' => $status_show, 'align' => 'left', 'class' => array('even')),
				array('data' => $view_details, 'align' => 'left', 'class' => array('odd')),
			);
		}

		$variables = array(
			'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')),
			'header' => $header,
			'rows' => $rows,
			'empty' => t("No data found!")
		);


		$output = theme('datatable', $variables);
	}





	return $output;



}






