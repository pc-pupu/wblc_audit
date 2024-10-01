<?php
drupal_add_js(drupal_get_path('module', 'alc') . '/js/mycustom.js');
drupal_add_css(drupal_get_path('module', 'addlc') . '/css/style.css');

function ds_karmasathi_migrant_list($form, &$form_state, $user_role_en = '', $par = '', $type = '', $next = 0, $p = 1)
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

	// if($user->uid==788){
	// 	echo $type;die;
	// }



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


	if ($user_role == 7) {
		$back_link = $base_root . $base_path . 'duare-sarkar-official-karmasathi-list-xls/' . $user_role_en . '/' . $par . '/' . $type . '/' . $next . '/' . $p;
		$back = l(t('<span class="btn btn-success pull-right"><i class="fa fa-file-excel-o"></i> DOWNLOAD LIST OF ' . $exlName . ' </span>'), $back_link, array('html' => TRUE));

	} else {
		$back_link = $base_root . $base_path . 'duare-sarkar-official-karmasathi-list-xls/' . $user_role_en . '/' . $par . '/' . $type . '/' . $next . '/' . $p;
		$back = l(t('<span class="btn btn-success pull-right"><i class="fa fa-file-excel-o"></i>DOWNLOAD THE BELOW LIST</span>'), $back_link, array('html' => TRUE));
	}



	if ((($user_role == 4) || ($user_role == 12) || ($user_role == 7) || ($user_role == 5))) {
		if (empty($type)) {
			$form['application_lists']['markup_data1'] = array(
				'#type' => 'markup',
				'#markup' => '<div class="box box-info"><div class="box-header"><div class="nav-tabs-custom">
								<ul class="nav nav-tabs">
									<li class="' . $active . '">' . l('Approved Worker List', 'duare-sarkar-official-karmasathi-list/' . $user_role_en . '/approved') . '</li>
									<li class="' . $activePen . '">' . l('Pending for Approval', 'duare-sarkar-official-karmasathi-list/' . $user_role_en . '/pending') . '</li>
									<li class="' . $activeIncomplete . '">' . l('Provisional Application', 'duare-sarkar-official-karmasathi-list/' . $user_role_en . '/provisional') . '</li>
									<li class="' . $activeBack . '">' . l('Back for Correction', 'duare-sarkar-official-karmasathi-list/' . $user_role_en . '/back') . '</li>
									<li class="' . $activeRej . '">' . l('Rejected Application', 'duare-sarkar-official-karmasathi-list/' . $user_role_en . '/reject') . '</li>
								</ul>
								<div class="tab-content">
									<div class="tab-pane active" id="fa-icons">
									<section id="new"><div class="feedback-scroll"><br><br>' . $back . '<br><br>'
			);
		} else {
			$form['application_lists']['markup_data1'] = array(
				'#type' => 'markup',
				'#markup' => '<div class="box box-info"><div class="box-header"><div class="nav-tabs-custom">
								<ul class="nav nav-tabs">
									<li class="' . $active . '">' . l('Approved Worker List', 'duare-sarkar-official-karmasathi-list/' . $user_role_en . '/approved' . '/' . $type) . '</li>
									<li class="' . $activePen . '">' . l('Pending for Approval', 'duare-sarkar-official-karmasathi-list/' . $user_role_en . '/pending' . '/' . $type) . '</li>
									<li class="' . $activeIncomplete . '">' . l('Provisional Application', 'duare-sarkar-official-karmasathi-list/' . $user_role_en . '/provisional' . '/' . $type) . '</li>
									<li class="' . $activeBack . '">' . l('Back for Correction', 'duare-sarkar-official-karmasathi-list/' . $user_role_en . '/back' . '/' . $type) . '</li>
									<li class="' . $activeRej . '">' . l('Rejected Application', 'duare-sarkar-official-karmasathi-list/' . $user_role_en . '/reject' . '/' . $type) . '</li>
								</ul>
								<div class="tab-content">
									<div class="tab-pane active" id="fa-icons">
									<section id="new"><div class="feedback-scroll"><br><br>' . $back . '<br><br>'
			);

		}
		$form['application_lists']['user_role'] = array(
			'#type' => 'hidden',
			'#value' => $user_role,
		);
		$form['application_lists']['par'] = array(
			'#type' => 'hidden',
			'#value' => $par,
		);
		$form['application_lists']['type'] = array(
			'#type' => 'hidden',
			'#value' => $type,
		);
		if ($type == "DS") {
			$form['application_lists']['ds_id'] = array(
				'#prefix' => '<div class="row"><div class="col-md-2">',
				'#suffix' => '</div>',
				'#type' => 'textfield',
				'#title' => 'DS ID',
				'#attributes' => array('class' => array('form-control')),


			);
		}

		$form['application_lists']['mobile'] = array(
			'#prefix' => '<div class="col-md-3">',
			'#suffix' => '</div>',
			//'#element_validate' => array('MISC_PHONE_VALIDATE'),
			'#type' => 'textfield',
			'#title' => 'MOBILE',
			'#attributes' => array('class' => array('form-control')),


		);
		$form['application_lists']['mwin'] = array(
			'#prefix' => '<div class="col-md-3">',
			'#suffix' => '</div>',
			'#type' => 'textfield',
			'#title' => 'MWIN',
			'#attributes' => array('class' => array('form-control')),


		);
		$form['application_lists']['name'] = array(
			'#prefix' => '<div class="col-md-3">',
			'#suffix' => '</div>',
			'#type' => 'textfield',
			'#title' => 'WORKER NAME',
			'#attributes' => array('class' => array('form-control')),


		);
		$form['application_lists']['submit'] = array(
			'#id' => 'submitBtn',
			'#value' => 'Search',
			'#attributes' => array('class' => array('btn btn-primary')),
			'#prefix' => '<div class="col-md-1"><div class="form-item"><label>&nbsp;</label><br>',
			'#suffix' => '</div></div></div>',
			'#type' => 'button',
			'#ajax' => array(
				'callback' => 'reports_by_search',
				'wrapper' => 'reports_by_search_div',
				'method' => 'html',
				'effect' => 'fade'
			)
		);
		$form['application_lists']['markup_data2'] = array(
			'#type' => 'markup',
			'#markup' => '<div id="reports_by_search_div">' . get_karmasathi_migrant_lists($user_role, $par, $type, $next, $p) . '</div>'
		);

		$form['application_lists']['markup_dataend'] = array(
			'#type' => 'markup',
			'#markup' => '</div></section></div></div></div>'

		);


	} else {
		drupal_set_message('You are not authorize to access this page', error);
	}

	return $form;

}
function get_karmasathi_migrant_lists($usertype = '', $par = '', $type = '', $next = 0, $pgage_for = 1)
{
	//echo $usertype . '<br>' . $par . '<br>' . $type;
	//die;
	//die('ok');
	global $base_root, $base_path, $user;
	$par1 = $par;
	$table = $apply_category = $unit_name = $actionStatus = '';
	$user_id = $user->uid;

	$i = $next;

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

	$query_details = $query->execute()->fetchAll();

	if ($par == 'approved') {
		$status = 'C';
	} elseif ($par == 'pending') {
		$status = array('A', 'BA');
	} elseif ($par == 'provisional') {
		$status = array('P', 'BP');
	} elseif ($par == 'back') {
		$status = 'B';
	} elseif ($par == 'reject') {
		$status = 'R';
	}
	$datashow = 100;

	$curl_post_data = array(
		'source' => 'WBLC',
		'taskid' => 'DSLCFETCHAPPLICATION',
		'user_role' => $user_role,
		'block_subdiv' => $area_arr,
		'user_id' => $user_id,
		'iscustom_juris' => $iscustom_juris,
		'status' => $status,
		'datashow' => $datashow,
		'next' => $next,
		'excel' => 'no',
		'type' => $type
	);
	// if($user_id==788)	{
	// 	echo "<pre>";
	// print_r($curl_post_data);die;
	// }



	//  $service_url = "https://164.100.199.8:443/rest/wblcofficial";
	 $service_url = "https://karmasathips.wblabour.gov.in/rest/wblcofficial";
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
	//print_r($errors); echo 777; die;
	curl_close($ch);

	$results = json_decode(trim($results), TRUE);  //echo '<pre>'; print_r($results); die;

	//print_r($results['content']);die;
	if ($results['status_code'] == 200) {
		$total_all = $results['total_all'];
		$i = $next;
		if ($type == 'DS') {
			$header = array(
				array('data' => 'Sl. No.', 'width' => '6%', 'data-orderable' => false),
				array('data' => 'DS ID', 'width' => ''),
				array('data' => 'MWIN', 'width' => '10%'),
				array('data' => 'Worker Name', 'width' => '15%'),
				array('data' => 'Mobile Number', 'width' => '10%'),
				array('data' => 'Status', 'width' => '15%'),
				array('data' => 'action', 'width' => '8%'),
			);
		} else {
			$header = array(
				array('data' => 'Sl. No.', 'width' => '6%', 'data-orderable' => false),
				//array('data' => 'DS ID', 'width' => ''),
				array('data' => 'MWIN', 'width' => '10%'),
				array('data' => 'Worker Name', 'width' => '15%'),
				array('data' => 'Mobile Number', 'width' => '10%'),
				array('data' => 'Status', 'width' => '15%'),
				array('data' => 'action', 'width' => '8%'),
			);
		}


		foreach ($results['content'] as $jobseeker_data) {
			$i++;
			$next++;
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

			if ($i > 0) {
				if ($type == 'DS') {
					$rows[] = array(
						array('data' => $i, 'align' => 'left', 'class' => array('even')),
						array('data' => $jobseeker_data['duaresarkar_id'], 'align' => 'left', 'class' => array('odd')),
						array('data' => $identification_number, 'align' => 'left', 'class' => array('odd')),
						array('data' => $name, 'align' => 'left', 'class' => array('odd')),
						//array('data' => , 'align' => 'left', 'class' => array('even')),
						array('data' => $mobile, 'align' => 'left', 'class' => array('odd')),
						array('data' => $status_show, 'align' => 'left', 'class' => array('even')),
						array('data' => $view_details, 'align' => 'left', 'class' => array('odd')),
					);
				} else {
					$rows[] = array(
						array('data' => $i, 'align' => 'left', 'class' => array('even')),
						//array('data' => $jobseeker_data['duaresarkar_id'], 'align' => 'left', 'class' => array('odd')),
						array('data' => $identification_number, 'align' => 'left', 'class' => array('odd')),
						array('data' => $name, 'align' => 'left', 'class' => array('odd')),
						//array('data' => , 'align' => 'left', 'class' => array('even')),
						array('data' => $mobile, 'align' => 'left', 'class' => array('odd')),
						array('data' => $status_show, 'align' => 'left', 'class' => array('even')),
						array('data' => $view_details, 'align' => 'left', 'class' => array('odd')),
					);

				}
			}
		}

		if ($i == $total_all) {
			$next1 = "Next &raquo;";
		} else {
			$next1 = '<a href=' . $base_root . $base_path . 'duare-sarkar-official-karmasathi-list/' . encryption_decryption_fun('encrypt', $usertype) . '/' . $par1 . '/' . $type . '/' . ($next) . '>Next &raquo;</a>';

		}
		if ($next - $datashow > 0) {
			$privious = '<a href=' . $base_root . $base_path . 'duare-sarkar-official-karmasathi-list/' . encryption_decryption_fun('encrypt', $usertype) . '/' . $par1 . '/' . $type . '/' . ($next - $datashow - $datashow) . '>&laquo; Previous</a>';
		} else {
			$privious = '&laquo; Previous';
		}

		$foot = '';
		$foot .= '<nav aria-label="...">
	<ul class="pagination pagination-sm justify-content-center cus-pagination-number">';
		//$page1 = $page + 5;
		$remaining_page = $total_all % $datashow;

		$totalpage = $total_all / $datashow | bc;
		if (!empty($remaining_page)) {
			$totalpage = $totalpage + 1;
		}


		if ($pgage_for >= 6) {
			$foot .= '<li class="page-item"><a class="page-link first" href=' . $base_root . $base_path . 'duare-sarkar-official-karmasathi-list/' . encryption_decryption_fun('encrypt', $usertype) . '/' . $par1 . '/' . $type . '/' . '>First</a></li>';
			$foot .= '<li class="page-item"><a class="page-link" href=' . $base_root . $base_path . 'duare-sarkar-official-karmasathi-list/' . encryption_decryption_fun('encrypt', $usertype) . '/' . $par1 . '/' . $type . '/' . ((($pgage_for - 1) - 5) * $datashow) . '/' . ($pgage_for - 5) . '>' . ($pgage_for - 5) . '</a></li>';
			$foot .= '<li class="page-item"><a class="page-link" href=' . $base_root . $base_path . 'duare-sarkar-official-karmasathi-list/' . encryption_decryption_fun('encrypt', $usertype) . '/' . $par1 . '/' . $type . '/' . ((($pgage_for - 1) - 4) * $datashow) . '/' . ($pgage_for - 4) . '>' . ($pgage_for - 4) . '</a></li>';
			$foot .= '<li class="page-item"><a class="page-link" href=' . $base_root . $base_path . 'duare-sarkar-official-karmasathi-list/' . encryption_decryption_fun('encrypt', $usertype) . '/' . $par1 . '/' . $type . '/' . ((($pgage_for - 1) - 2) * $datashow) . '/' . ($pgage_for - 2) . '>' . ($pgage_for - 2) . '</a></li>';
			$foot .= '<li class="page-item"><a class="page-link" href=' . $base_root . $base_path . 'duare-sarkar-official-karmasathi-list/' . encryption_decryption_fun('encrypt', $usertype) . '/' . $par1 . '/' . $type . '/' . ((($pgage_for - 1) - 1) * $datashow) . '/' . ($pgage_for - 1) . '>' . ($pgage_for - 1) . '</a></li>';
			$image = '<li><img src="' . $base_root . $base_path . drupal_get_path('module', 'alc') . '/images/pagination-2way.png" >';
			$foot .= $image . '</li>';

		}

		for ($page = $pgage_for; $page <= $pgage_for + 5; $page++) {
			if ($totalpage < $page) {

				break;
			}
			if ($page == 1) {
				$foot .= '<li class="page-item"><a class="page-link" href=' . $base_root . $base_path . 'duare-sarkar-official-karmasathi-list/' . encryption_decryption_fun('encrypt', $usertype) . '/' . $par1 . '/' . '>1</a></li>';
			} else {


				$foot .= '<li class="page-item"><a class="page-link" href=' . $base_root . $base_path . 'duare-sarkar-official-karmasathi-list/' . encryption_decryption_fun('encrypt', $usertype) . '/' . $par1 . '/' . (($page - 1) * $datashow) . '/' . $page . '>' . $page . '</a></li>';

			}
		}
		$foot .= '<li class="page-item"><a class="page-link last" href=' . $base_root . $base_path . 'duare-sarkar-official-karmasathi-list/' . encryption_decryption_fun('encrypt', $usertype) . '/' . $par1 . '/' . (($totalpage - 1) * $datashow) . '/' . ($totalpage - 1) . '>Last</a></li>';
		$foot .= ' </ul>
	</nav>';

		//   $footer[] = array(
		// 	'<tr><td colspan="7"><div class="row"><div class="custom-table-footer">
		//  <div  class="col-md-3 pull-left"><strong>TOTAL ' . $total_all . ' DATA FOUND.</strong></div>  
		//  <div class="col-md-7 pull-left"><center>' . $foot . '</center></div>
		//  <div class="col-md-2 pull-right text-right">' . $privious . ' || ' . $next1 . '</div> 
		//  </div></div></td></tr>',


		//   );
		$footer[] = array(
			'<tr><td colspan="7"><div class="row"><div class="custom-table-footer">
	 <div  class="col-md-3 pull-left"><strong>TOTAL ' . $total_all . ' DATA FOUND.</strong></div>  
	 <div class="col-md-7 pull-left"><center></center></div>
	 <div class="col-md-2 pull-right text-right">' . $privious . ' || ' . $next1 . '</div> 
	 </div></div></td></tr>',


		);

		$variables = array(
			'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')),
			'header' => $header,
			'rows' => $rows,
			'footer' => $footer,
			'empty' => t("No data found!")
		);


		$output = theme('table', $variables);
	}

	return $output;
}

function reports_by_search($form, &$form_state)
{
	$val = $form_state['values'];
	if (empty($val['name']) && empty($val['ds_id']) && empty($val['mwin']) && empty($val['mobile'])) {
		return '<span style:"color:red;">Select minimium one parameter for search</span>';
	}

	$user_role = $val['user_role'];
	$par = $val['par'];
	global $base_root, $base_path, $user;
	$par1 = $par;
	$table = $apply_category = $unit_name = $actionStatus = '';
	$user_id = $user->uid;

	$i = $next;

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

	$query_details = $query->execute()->fetchAll();

	if ($par == 'approved') {
		$status = 'C';
	} elseif ($par == 'pending') {
		$status = array('A', 'BA');
	} elseif ($par == 'provisional') {
		$status = array('P', 'BP');
	} elseif ($par == 'back') {
		$status = 'B';
	} elseif ($par == 'reject') {
		$status = 'R';
	}
	$datashow = 100;

	$curl_post_data = array(
		'source' => 'WBLC',
		'taskid' => 'DSLCFETCHAPPLICATION',
		'user_role' => $user_role,
		'block_subdiv' => $area_arr,
		'user_id' => $user_id,
		'iscustom_juris' => $iscustom_juris,
		'status' => $status,
		'datashow' => $datashow,
		'next' => $next,
		'excel' => 'no',
		'search' => 'search',
		'name' => $val['name'],
		'ds_id' => $val['ds_id'],
		'mwin' => $val['mwin'],
		'mobile' => $val['mobile'],
		'type' => $val['type'],


	);
	//print_r($curl_post_data);
	// $service_url = "https://164.100.199.8:443/rest/wblcofficial";
	 $service_url = "https://karmasathips.wblabour.gov.in/rest/wblcofficial";
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
	//print_r($errors); echo 777; die;
	curl_close($ch);

	$results = json_decode(trim($results), TRUE); // echo '<pre>'; print_r($results); die;

	//print_r($results['content']);die;
	if ($results['status_code'] == 200) {
		$total_all = $results['total_all'];
		$i = $next;
		if ($val['type'] == "DS") {
			$header = array(
				array('data' => 'Sl. No.', 'width' => '6%', 'data-orderable' => false),

				array('data' => 'DS ID', 'width' => ''),

				array('data' => 'MWIN', 'width' => '10%'),
				array('data' => 'Worker Name', 'width' => '15%'),
				array('data' => 'Mobile Number', 'width' => '10%'),
				array('data' => 'Status', 'width' => '8%'),
				array('data' => 'action', 'width' => '8%'),
			);
		} else {
			$header = array(
				array('data' => 'Sl. No.', 'width' => '6%', 'data-orderable' => false),

				//array('data' => 'DS ID', 'width' => ''),

				array('data' => 'MWIN', 'width' => '10%'),
				array('data' => 'Worker Name', 'width' => '15%'),
				array('data' => 'Mobile Number', 'width' => '10%'),
				array('data' => 'Status', 'width' => '8%'),
				array('data' => 'action', 'width' => '8%'),
			);

		}


		foreach ($results['content'] as $jobseeker_data) {
			$i++;
			$next++;
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

			if ($i > 0) {
				if ($val['type'] == "DS") {
					$rows[] = array(
						array('data' => $i, 'align' => 'left', 'class' => array('even')),
						array('data' => $jobseeker_data['duaresarkar_id'], 'align' => 'left', 'class' => array('odd')),
						array('data' => $identification_number, 'align' => 'left', 'class' => array('odd')),
						array('data' => $name, 'align' => 'left', 'class' => array('odd')),
						//array('data' => , 'align' => 'left', 'class' => array('even')),
						array('data' => $mobile, 'align' => 'left', 'class' => array('odd')),
						array('data' => $status_show, 'align' => 'left', 'class' => array('even')),
						array('data' => $view_details, 'align' => 'left', 'class' => array('odd')),
					);
				} else {
					$rows[] = array(
						array('data' => $i, 'align' => 'left', 'class' => array('even')),
						//array('data' => $jobseeker_data['duaresarkar_id'], 'align' => 'left', 'class' => array('odd')),
						array('data' => $identification_number, 'align' => 'left', 'class' => array('odd')),
						array('data' => $name, 'align' => 'left', 'class' => array('odd')),
						//array('data' => , 'align' => 'left', 'class' => array('even')),
						array('data' => $mobile, 'align' => 'left', 'class' => array('odd')),
						array('data' => $status_show, 'align' => 'left', 'class' => array('even')),
						array('data' => $view_details, 'align' => 'left', 'class' => array('odd')),
					);

				}
			}
		}

		if ($i == $total_all) {
			$next1 = "Next &raquo;";
		} else {
			$next1 = '<a href=' . $base_root . $base_path . 'duare-sarkar-official-karmasathi-list/' . encryption_decryption_fun('encrypt', $usertype) . '/' . $par1 . '/' . ($next) . '>Next &raquo;</a>';

		}
		if ($next - $datashow > 0) {
			$privious = '<a href=' . $base_root . $base_path . 'duare-sarkar-official-karmasathi-list/' . encryption_decryption_fun('encrypt', $usertype) . '/' . $par1 . '/' . ($next - $datashow - $datashow) . '>&laquo; Previous</a>';
		} else {
			$privious = '&laquo; Previous';
		}

		$foot = '';
		$foot .= '<nav aria-label="...">
	<ul class="pagination pagination-sm justify-content-center cus-pagination-number">';
		//$page1 = $page + 5;
		$remaining_page = $total_all % $datashow;

		$totalpage = $total_all / $datashow | bc;
		if (!empty($remaining_page)) {
			$totalpage = $totalpage + 1;
		}


		if ($pgage_for >= 6) {
			$foot .= '<li class="page-item"><a class="page-link first" href=' . $base_root . $base_path . 'duare-sarkar-official-karmasathi-list/' . encryption_decryption_fun('encrypt', $usertype) . '/' . $par1 . '>First</a></li>';
			$foot .= '<li class="page-item"><a class="page-link" href=' . $base_root . $base_path . 'duare-sarkar-official-karmasathi-list/' . encryption_decryption_fun('encrypt', $usertype) . '/' . $par1 . '/' . ((($pgage_for - 1) - 5) * $datashow) . '/' . ($pgage_for - 5) . '>' . ($pgage_for - 5) . '</a></li>';
			$foot .= '<li class="page-item"><a class="page-link" href=' . $base_root . $base_path . 'duare-sarkar-official-karmasathi-list/' . encryption_decryption_fun('encrypt', $usertype) . '/' . $par1 . '/' . ((($pgage_for - 1) - 4) * $datashow) . '/' . ($pgage_for - 4) . '>' . ($pgage_for - 4) . '</a></li>';
			$foot .= '<li class="page-item"><a class="page-link" href=' . $base_root . $base_path . 'duare-sarkar-official-karmasathi-list/' . encryption_decryption_fun('encrypt', $usertype) . '/' . $par1 . '/' . ((($pgage_for - 1) - 2) * $datashow) . '/' . ($pgage_for - 2) . '>' . ($pgage_for - 2) . '</a></li>';
			$foot .= '<li class="page-item"><a class="page-link" href=' . $base_root . $base_path . 'duare-sarkar-official-karmasathi-list/' . encryption_decryption_fun('encrypt', $usertype) . '/' . $par1 . '/' . ((($pgage_for - 1) - 1) * $datashow) . '/' . ($pgage_for - 1) . '>' . ($pgage_for - 1) . '</a></li>';
			$image = '<li><img src="' . $base_root . $base_path . drupal_get_path('module', 'alc') . '/images/pagination-2way.png" >';
			$foot .= $image . '</li>';

		}

		for ($page = $pgage_for; $page <= $pgage_for + 5; $page++) {
			if ($totalpage < $page) {

				break;
			}
			if ($page == 1) {
				$foot .= '<li class="page-item"><a class="page-link" href=' . $base_root . $base_path . 'duare-sarkar-official-karmasathi-list/' . encryption_decryption_fun('encrypt', $usertype) . '/' . $par1 . '/' . '>1</a></li>';
			} else {


				$foot .= '<li class="page-item"><a class="page-link" href=' . $base_root . $base_path . 'duare-sarkar-official-karmasathi-list/' . encryption_decryption_fun('encrypt', $usertype) . '/' . $par1 . '/' . (($page - 1) * $datashow) . '/' . $page . '>' . $page . '</a></li>';

			}
		}
		$foot .= '<li class="page-item"><a class="page-link last" href=' . $base_root . $base_path . 'duare-sarkar-official-karmasathi-list/' . encryption_decryption_fun('encrypt', $usertype) . '/' . $par1 . '/' . (($totalpage - 1) * $datashow) . '/' . ($totalpage - 1) . '>Last</a></li>';
		$foot .= ' </ul>
	</nav>';

		$footer[] = array(
			'<tr><td colspan="7"><div class="row"><div class="custom-table-footer">
	 <div  class="col-md-3 pull-left"><strong>TOTAL ' . $total_all . ' DATA FOUND.</strong></div>  
	 <div class="col-md-7 pull-left"><center>' . $foot . '</center></div>
	 <div class="col-md-2 pull-right text-right">' . $privious . ' || ' . $next1 . '</div> 
	 </div></div></td></tr>',


		);

		$variables = array(
			'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')),
			'header' => $header,
			'rows' => $rows,
			//'footer'=>$footer,
			'empty' => t("No data found!")
		);


		$output = theme('datatable', $variables);
	}

	return $output;





}




