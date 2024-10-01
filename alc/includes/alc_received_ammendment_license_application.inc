<?php

drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');

function alc_received_ammendment_license_application($form, &$form_state, $application_status = 0, $page_per = 0)
{

	global $base_root, $base_path, $user;


	$user_id = $user->uid;

	if (empty($application_status)) {
		$active = 'active';
	}
	if ($application_status == 'pending') {
		$activepending = 'active';
	} else {
		$activepending = '';
	}
	if ($application_status == 'rectification') {
		$activerectification = 'active';
	} else {
		$activerectification = '';
	}
	if ($application_status == 'issued') {
		$activeissued = 'active';
	} else {
		$activeissued = '';
	}
	if ($application_status == 'finalsubmit') {
		$activefinalsubmit = 'active';
	} else {
		$activefinalsubmit = '';
	}
	if ($application_status == 'rejected') {
		$activerejected = 'active';
	} else {
		$activerejected = '';
	}
	if ($application_status == 'forward') {
		$activeforward = 'active';
	} else {
		$activeforward = '';
	}


	if ($application_status == 0) {
		$header_title = 'All';
	} else {
		$header_title = ucfirst($application_status);
	}

	// $get_all_contractor_ammendment = contractor_ammendment_list();

	/*</div></div>*/

	$form['contractor_license_lists']['markup_data_1'] = array(
		'#prefix' => '<div class="nav-tabs-custom">
												<ul class="nav nav-tabs">
													<li class="' . $active . '">' . l('All', 'alc-received-ammendment-license') . '</li>
													<li class="' . $activepending . '">' . l('Pending', 'alc-received-ammendment-license/pending') . '</li>
													<li class="' . $activerectification . '">' . l('Sent Back for Rectification', 'alc-received-ammendment-license/rectification') . '</li>
													<li class="' . $activeforward . '">' . l('Forward', 'alc-received-ammendment-license/forward') . '</li>
													<li class="' . $activefinalsubmit . '">' . l('Final Submit', 'alc-received-ammendment-license/finalsubmit') . '</li>												
													<li class="' . $activeissued . '">' . l('Issued', 'alc-received-ammendment-license/issued') . '</li>
													<li class="' . $activerejected . '">' . l('Rejected', 'alc-received-ammendment-license/rejected') . '</li>
												</ul>
												<div class="tab-content"><div class="tab-pane active" id="fa-icons">',
		'#suffix' => '</div></div>',



	);
	$form['contractor_license_lists']['application_status'] = array(
		'#type' => 'hidden',
		'#value' => $application_status,
	);
	$form['contractor_license_lists']['name'] = array(
		'#prefix' => '<div class="col-md-3">',
		'#suffix' => '</div>',
		'#type' => 'textfield',
		'#title' => 'Contractor Name',
		'#attributes' => array('class' => array('form-control')),


	);
	$form['contractor_license_lists']['formv'] = array(
		'#prefix' => '<div class="col-md-3">',
		'#suffix' => '</div>',
		'#type' => 'textfield',
		'#title' => 'Form-V',
		'#attributes' => array('class' => array('form-control')),
	);


	$form['contractor_license_lists']['lienceno'] = array(
		'#prefix' => '<div class="col-md-3">',
		'#suffix' => '</div>',
		'#type' => 'textfield',
		'#title' => 'License No',
		'#attributes' => array('class' => array('form-control')),


	);

	$form['contractor_license_lists']['submit'] = array(
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

	$form['contractor_license_lists']['markup_data_2'] = array(
		'#markup' => '<div id="reports_by_search_div" class="feedback-scroll">' . contractor_ammendment_list($application_status, $page_per) . '</div>',
	);


	return $form;
}


function contractor_ammendment_list($application_status = 0, $page_per)
{

	global $user, $base_root, $base_path;
	$page_to = 100;

	$view_img = '<span class="badge bg-blue"><i class="fa fa-eye"></i> View Details<div>';

	$uid = $user->uid;
	$output = '';
	$particular_info = array();
	$duplicate = array();
	$row_arr = array();
	$header = array(
		array('data' => 'Sl. No', 'field' => 'slno', 'width' => '6%'),
		array('data' => 'FORM-V / REF.No.<br/>Contractor Name', 'field' => 'name_of_contractor', 'width' => '18%'),
		array('data' => 'PE Registration No.& Date', 'width' => '14%'),
		array('data' => 'License No.& Date', 'width' => '14%'),
		array('data' => 'B/M/C/NA/SEZ', 'field' => 'created_date', 'width' => '10%'),
		array('data' => 'Valid Upto', 'width' => '10%'),
		array('data' => 'Application Date', 'width' => '10%'),
		array('data' => 'Status', 'width' => '10%'),
		array('data' => 'Action', 'width' => '10%'),
	);
	if (isset($_GET['sort']) && isset($_GET['order'])) {
		// Sort it Ascending or Descending?
		if ($_GET['sort'] == 'asc')
			$sort = 'ASC';
		else
			$sort = 'DESC';

		// Which column will be sorted
		switch ($_GET['order']) {
			case 'Name':
				$order = 'identification_number';
				break;
			case 'Email':
				$order = 'f_name';
				break;
			default:
				$order = 'm_name';
		}
	} else {
		$sort = 'ASC';
		$order = 'f_name';
	}

	$query_subdivision = db_select('l_customuser_relation_address', 'lcra');
	$query_subdivision->fields('lcra', array('sub_div_code', 'district_code', 'block_code', 'custom_area_jurisdiction', 'is_custom_area_jurisdiction'));
	$query_subdivision->condition('lcra.user_id', $uid, '=');
	$result_query_subdivision = $query_subdivision->execute();
	$result_arr = $result_query_subdivision->fetchAssoc();
	$subdivision_alc = $result_arr['sub_div_code'];
	$customjurisdiction = $result_arr['custom_area_jurisdiction'];
	$iscustom_juris = $result_arr['is_custom_area_jurisdiction'];

	if ($customjurisdiction == 'BMCNS' && $iscustom_juris == 'Y') {
		$block_code_arr = db_select('l_customuser_reletion_services_wise_user_settings', 'cra')
			->fields('cra', array('block_code'))
			->condition('cra.user_id', $uid)
			->execute()->fetchObject()->block_code;
		$block_code_arr = explode(',', $block_code_arr);
	}


	$query = db_select('l_amendment_of_contract_labour_license', 'amendment');
	$query->LeftJoin('l_contractor_license_application', 'license', 'license.id= amendment.license_id');
	$query->LeftJoin('l_contractor_info_master', 'infomaster', 'infomaster.id= license.serial_no_from_v');
	$query->fields('amendment', array('is_flag', 'id', 'created_by', 'created_date', 'reference_no', 'license_id', 'license_renewal_ammendment_id', 'name_work_site_areatype', 'ammendment_status'));
	$query->fields('license', array('serial_no_from_v'));
	$query->fields('infomaster', array('name_of_contractor', 'contractor_type'));
	if (($iscustom_juris == 'Y') && (!empty($block_code_arr)) && ($customjurisdiction == 'BMCNS')) {
		$query->condition('amendment.name_work_site_areatype', $block_code_arr, 'IN');
	} else {
		$query->condition('amendment.worksite_subdivision', $subdivision_alc);
	}
	$query->condition('amendment.ammendment_final_status', 'F');
	$query->isNotNull('amendment.ammendment_status');

	if (!empty($application_status) && $application_status == 'pending') {
		$query->condition('amendment.ammendment_status', 'F');
		$query->orderBy('amendment.id', 'ASC');
	}
	if (!empty($application_status) && $application_status == 'rectification') {
		$query->condition('amendment.ammendment_status', 'B');
		$query->orderBy('amendment.id', 'ASC');
	}
	if (!empty($application_status) && $application_status == 'issued') {
		$query->condition('amendment.ammendment_status', 'I');
		$query->orderBy('amendment.id', 'DESC');
	}
	if (!empty($application_status) && $application_status == 'finalsubmit') {
		$query->condition('amendment.ammendment_status', 'U');
		/*if($uid == 453){
																																																																																																																																																																																																																																																																																																															   $query->condition('amendment.license_id',7037);//Testing at KGP
																																																																																																																																																																																																																																																																																																															  */
		$query->orderBy('amendment.id', 'ASC');
	}
	if (!empty($application_status) && $application_status == 'rejected') {
		$query->condition('amendment.ammendment_status', 'R');
		$query->orderBy('amendment.id', 'DESC');
	}
	if (!empty($application_status) && $application_status == 'forward') {
		$query->condition('amendment.ammendment_status', 'FW');
		$query->orderBy('amendment.id', 'ASC');
	}
	$query->isNotNull('amendment.created_by');
	$total_all = $query->execute()->rowCount();
	if ($page_per < 0) {
		$query->range(0, $page_to);
	} else {
		$query->range($page_per, $page_to);
	}


	$result = $query->execute();

	$sl_no = 1;


	foreach ($result as $val) {

		$sl_no = ++$page_per;
		$application_date = !empty($val->created_date) ? date('dS M, Y', strtotime($val->created_date)) : '';
		$record_details = db_query("select  * from fun_basic_information1('" . $val->created_by . "','" . $val->license_renewal_ammendment_id . "','" . $val->is_flag . "')")->fetchAssoc();

		$license_no = !empty($record_details['backlog_license_no']) ? '<strong>' . $record_details['backlog_license_no'] . '</strong><br/>' . $record_details['contractor_license_number'] : $record_details['contractor_license_number'];
		$license_date = !empty($record_details['backlog_license_date']) ? date('dS M, Y', strtotime($record_details['backlog_license_date'])) : date('dS M, Y', strtotime($record_details['license_date']));
		$validated_date = !empty($record_details['next_renweal_date']) ? date('dS M, Y', strtotime($record_details['next_renweal_date'])) : '';

		$serial_no = return_ammendement_serialno($val->license_id);
		$contractor_regmaster_details = contractor_regmaster_details($val->serial_no_from_v);
		$pe_reg_number = $contractor_regmaster_details['registration_number'];
		$registration_date = !empty($contractor_regmaster_details['registration_date']) ? date('dS M, Y', strtotime($contractor_regmaster_details['registration_date'])) : '';
		;
		$remark_type = fetch_latest_remark($val->reference_no);

		if ($val->contractor_type == 1) {
			$serialLabel = 'REF No : 00' . $val->serial_no_from_v;
		} else {
			$serialLabel = 'FORM-V : 00' . $val->serial_no_from_v;
		}

		$blockMunData = db_query('select block_mun_name from block_mun_master where block_code=' . $val->name_work_site_areatype)->fetchAssoc();
		$blockMunName = $blockMunData['block_mun_name'];


		if ($remark_type == "F" && $remark_type == "F") {
			$STATUS = '<span class="pending">Pending</span>';
		}
		if (trim($remark_type) == "A") {
			$STATUS = '<span class="feespending">Fees Pending</span>';
		}
		if ($remark_type == "B") {
			$STATUS = '<span class="backed">Rectification</span>';
		}
		if ($remark_type == "P") {
			$STATUS = '<span class="feespaid">Fees Paid</span>';
		}
		if ($remark_type == "BI") {
			$STATUS = '<span class="backtoins">Back to Inspector</span>';
		}
		if ($remark_type == "C") {
			$STATUS = '<span class="call_applicant">Call</span>';
		}
		if ($remark_type == "I") {
			$STATUS = '<span class="issued">Issued</span>';
		}
		if ($remark_type == "R") {
			$STATUS = '<span class="reject">Reject</span>';
		}
		if ($remark_type == "FW") {
			$STATUS = '<span class="forward">Forwarded</span>';
		}
		if ($remark_type == "U") {
			$STATUS = '<span class="finalsubmit">Form IV uploaded</span>';
		}
		if ($remark_type == "AW") {
			$STATUS = '<span class="approved">Approved</span>';
		}

		if ($sl_no == $total_all) {
			$next1 = "Next &raquo;";
		} else {
			$next1 = '<a href=' . $base_root . $base_path . 'alc-received-ammendment-license/' . $application_status . '/' . $page_per . '>Next &raquo;</a>';

		}
		if ($sl_no - 100 > 0) {
			$privious = '<a href=' . $base_root . $base_path . 'alc-received-ammendment-license/' . $application_status . '/' . ($sl_no - 100 - 100) . '>&laquo; Previous</a>';
		} else {
			$privious = '&laquo; Previous';
		}




		$view_link = l($view_img, 'alc-view-ammend-license/' . encryption_decryption_fun('encrypt', $val->serial_no_from_v) . '/' . encryption_decryption_fun('encrypt', $val->id) . '/' . encryption_decryption_fun('encrypt', $val->created_by) . '/' . encryption_decryption_fun('encrypt', $val->is_flag) . '/' . encryption_decryption_fun('encrypt', $val->license_renewal_ammendment_id), array('html' => TRUE));




		if ($sl_no > 0) {
			$rows[] = array(
				array('data' => $sl_no, 'align' => 'left', 'class' => array($class_name)),
				array('data' => '<strong>[ ' . $serialLabel . ' ]</strong> <br>' . $val->name_of_contractor, 'align' => 'left', 'class' => array($class_name)),
				array('data' => $pe_reg_number . '<br>' . $registration_date, 'align' => 'left', 'class' => array($class_name)),
				array('data' => $license_no . '<br>' . $license_date, 'align' => 'left', 'class' => array($class_name)),
				array('data' => $blockMunName, 'align' => 'left', 'class' => array($class_name)),
				array('data' => $validated_date, 'align' => 'left', 'class' => array($class_name)),
				array('data' => $application_date, 'align' => 'left', 'class' => array($class_name)),
				array('data' => $STATUS, 'align' => 'left', 'class' => array($class_name)),
				array('data' => $view_link, 'align' => 'left', 'class' => array($class_name)),
			);
		}

	}
	$footer[] = array(
		'<tr><td colspan="9"><div class="row"><div class="custom-table-footer">
	 <div  class="col-md-3 pull-left"><strong>TOTAL ' . $total_all . ' DATA FOUND.</strong></div>  
	 <div class="col-md-7 pull-left"><center></center></div>
	 <div class="col-md-2 pull-right text-right">' . $privious . ' || ' . $next1 . '</div> 
	 </div></div></td></tr>',
	);

	$variables = array(
		'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')),
		'header' => $header,
		'footer' => $footer,
		'rows' => $rows,
		'empty' => t(ucfirst($application_status) . " application not available")
	);

	$output = theme('table', $variables);

	return $output;
}

function reports_by_search($form, &$form_state)
{
	$val = $form_state['values'];
	$val['formv'] = substr($val['formv'], 2);

	if (empty($val['name']) && empty($val['formv']) && empty($val['lienceno'])) {
		return '<span style:"color:red;">Select minimium one parameter for search</span>';
	}
	//$application_status = $val['application_status'];


	global $user, $base_root, $base_path;
	$page_to = 10;

	$view_img = '<span class="badge bg-blue"><i class="fa fa-eye"></i> View Details<div>';

	$uid = $user->uid;
	$output = '';
	$particular_info = array();
	$duplicate = array();
	$row_arr = array();
	$header = array(
		array('data' => 'Sl. No', 'field' => 'slno', 'width' => '6%'),
		array('data' => 'FORM-V / REF.No.<br/>Contractor Name', 'field' => 'name_of_contractor', 'width' => '18%'),
		array('data' => 'PE Registration No.& Date', 'width' => '14%'),
		array('data' => 'License No.& Date', 'width' => '14%'),
		array('data' => 'B/M/C/NA/SEZ', 'field' => 'created_date', 'width' => '10%'),
		array('data' => 'Valid Upto', 'width' => '10%'),
		array('data' => 'Application Date', 'width' => '10%'),
		array('data' => 'Status', 'width' => '10%'),
		array('data' => 'Action', 'width' => '10%'),
	);
	if (isset($_GET['sort']) && isset($_GET['order'])) {
		// Sort it Ascending or Descending?
		if ($_GET['sort'] == 'asc')
			$sort = 'ASC';
		else
			$sort = 'DESC';

		// Which column will be sorted
		switch ($_GET['order']) {
			case 'Name':
				$order = 'identification_number';
				break;
			case 'Email':
				$order = 'f_name';
				break;
			default:
				$order = 'm_name';
		}
	} else {
		$sort = 'ASC';
		$order = 'f_name';
	}

	$query_subdivision = db_select('l_customuser_relation_address', 'lcra');
	$query_subdivision->fields('lcra', array('sub_div_code', 'district_code', 'block_code', 'custom_area_jurisdiction', 'is_custom_area_jurisdiction'));
	$query_subdivision->condition('lcra.user_id', $uid, '=');
	$result_query_subdivision = $query_subdivision->execute();
	$result_arr = $result_query_subdivision->fetchAssoc();
	$subdivision_alc = $result_arr['sub_div_code'];
	$customjurisdiction = $result_arr['custom_area_jurisdiction'];
	$iscustom_juris = $result_arr['is_custom_area_jurisdiction'];

	if ($customjurisdiction == 'BMCNS' && $iscustom_juris == 'Y') {
		$block_code_arr = db_select('l_customuser_reletion_services_wise_user_settings', 'cra')
			->fields('cra', array('block_code'))
			->condition('cra.user_id', $uid)
			->execute()->fetchObject()->block_code;
		$block_code_arr = explode(',', $block_code_arr);
	}


	$query = db_select('l_amendment_of_contract_labour_license', 'amendment');
	$query->LeftJoin('l_contractor_license_application', 'license', 'license.id= amendment.license_id');
	$query->LeftJoin('l_contractor_info_master', 'infomaster', 'infomaster.id= license.serial_no_from_v');
	$query->fields('amendment', array('is_flag', 'id', 'created_by', 'created_date', 'reference_no', 'license_id', 'license_renewal_ammendment_id', 'name_work_site_areatype', 'ammendment_status'));
	$query->fields('license', array('serial_no_from_v', 'contractor_license_number'));
	$query->fields('infomaster', array('name_of_contractor', 'contractor_type'));
	if (($iscustom_juris == 'Y') && (!empty($block_code_arr)) && ($customjurisdiction == 'BMCNS')) {
		$query->condition('amendment.name_work_site_areatype', $block_code_arr, 'IN');
	} else {
		$query->condition('amendment.worksite_subdivision', $subdivision_alc);
	}
	//$query->condition('amendment.ammendment_final_status', 'F');
	$query->isNotNull('amendment.ammendment_status');

	// if (!empty($application_status) && $application_status == 'pending') {
	// 	$query->condition('amendment.ammendment_status', 'F');
	// 	$query->orderBy('amendment.id', 'ASC');
	// }
	// if (!empty($application_status) && $application_status == 'rectification') {
	// 	$query->condition('amendment.ammendment_status', 'B');
	// 	$query->orderBy('amendment.id', 'ASC');
	// }
	// if (!empty($application_status) && $application_status == 'issued') {
	// 	$query->condition('amendment.ammendment_status', 'I');
	// 	$query->orderBy('amendment.id', 'DESC');
	// }
	// if (!empty($application_status) && $application_status == 'finalsubmit') {
	// 	$query->condition('amendment.ammendment_status', 'U');

	// 	$query->orderBy('amendment.id', 'ASC');
	// }
	// if (!empty($application_status) && $application_status == 'rejected') {
	// 	$query->condition('amendment.ammendment_status', 'R');
	// 	$query->orderBy('amendment.id', 'DESC');
	// }
	// if (!empty($application_status) && $application_status == 'forward') {
	// 	$query->condition('amendment.ammendment_status', 'FW');
	// 	$query->orderBy('amendment.id', 'ASC');
	// }
	$query->orderBy('amendment.id', 'ASC');
	$db_or = db_or();
	if (!empty($val['name'])) {
		$db_or->condition('infomaster.name_of_contractor', '%' . $val['name'] . '%', 'ilike');
		$db_or->condition('infomaster.name_of_contractor', '%' . $val['name'], 'ilike');
		$db_or->condition('infomaster.name_of_contractor', $val['name'] . '%', 'ilike');
		$query->condition($db_or);
	}
	if (!empty($val['formv'])) {
		$db_or->condition('license.serial_no_from_v', '%' . $val['formv'] . '%', 'ilike');
		$db_or->condition('license.serial_no_from_v', '%' . $val['formv'], 'ilike');
		$db_or->condition('license.serial_no_from_v', $val['formv'] . '%', 'ilike');
		$query->condition($db_or);
	}
	if (!empty($val['lienceno'])) {
		$db_or->condition('license.contractor_license_number', '%' . $val['lienceno'] . '%', 'ilike');
		$db_or->condition('license.contractor_license_number', '%' . $val['lienceno'], 'ilike');
		$db_or->condition('license.contractor_license_number', $val['lienceno'] . '%', 'ilike');
		$query->condition($db_or);
	}
	$query->isNotNull('amendment.created_by');
	$total_all = $query->execute()->rowCount();




	$result = $query->execute();

	$sl_no = 1;


	foreach ($result as $val) {

		$sl_no = ++$page_per;
		$application_date = !empty($val->created_date) ? date('dS M, Y', strtotime($val->created_date)) : '';
		$record_details = db_query("select  * from fun_basic_information1('" . $val->created_by . "','" . $val->license_renewal_ammendment_id . "','" . $val->is_flag . "')")->fetchAssoc();

		$license_no = !empty($record_details['backlog_license_no']) ? '<strong>' . $record_details['backlog_license_no'] . '</strong><br/>' . $record_details['contractor_license_number'] : $record_details['contractor_license_number'];
		$license_date = !empty($record_details['backlog_license_date']) ? date('dS M, Y', strtotime($record_details['backlog_license_date'])) : date('dS M, Y', strtotime($record_details['license_date']));
		$validated_date = !empty($record_details['next_renweal_date']) ? date('dS M, Y', strtotime($record_details['next_renweal_date'])) : '';

		$serial_no = return_ammendement_serialno($val->license_id);
		$contractor_regmaster_details = contractor_regmaster_details($val->serial_no_from_v);
		$pe_reg_number = $contractor_regmaster_details['registration_number'];
		$registration_date = !empty($contractor_regmaster_details['registration_date']) ? date('dS M, Y', strtotime($contractor_regmaster_details['registration_date'])) : '';
		;
		$remark_type = fetch_latest_remark($val->reference_no);

		if ($val->contractor_type == 1) {
			$serialLabel = 'REF No : 00' . $val->serial_no_from_v;
		} else {
			$serialLabel = 'FORM-V : 00' . $val->serial_no_from_v;
		}

		$blockMunData = db_query('select block_mun_name from block_mun_master where block_code=' . $val->name_work_site_areatype)->fetchAssoc();
		$blockMunName = $blockMunData['block_mun_name'];


		if ($remark_type == "F" && $remark_type == "F") {
			$STATUS = '<span class="pending">Pending</span>';
		}
		if (trim($remark_type) == "A") {
			$STATUS = '<span class="feespending">Fees Pending</span>';
		}
		if ($remark_type == "B") {
			$STATUS = '<span class="backed">Rectification</span>';
		}
		if ($remark_type == "P") {
			$STATUS = '<span class="feespaid">Fees Paid</span>';
		}
		if ($remark_type == "BI") {
			$STATUS = '<span class="backtoins">Back to Inspector</span>';
		}
		if ($remark_type == "C") {
			$STATUS = '<span class="call_applicant">Call</span>';
		}
		if ($remark_type == "I") {
			$STATUS = '<span class="issued">Issued</span>';
		}
		if ($remark_type == "R") {
			$STATUS = '<span class="reject">Reject</span>';
		}
		if ($remark_type == "FW") {
			$STATUS = '<span class="forward">Forwarded</span>';
		}
		if ($remark_type == "U") {
			$STATUS = '<span class="finalsubmit">Form IV uploaded</span>';
		}
		if ($remark_type == "AW") {
			$STATUS = '<span class="approved">Approved</span>';
		}

		if ($sl_no == $total_all) {
			$next1 = "Next &raquo;";
		} else {
			$next1 = '<a href=' . $base_root . $base_path . 'alc-received-ammendment-license/' . $application_status . '/' . $page_per . '>Next &raquo;</a>';

		}
		if ($sl_no - 10 > 0) {
			$privious = '<a href=' . $base_root . $base_path . 'alc-received-ammendment-license/' . $application_status . '/' . ($page_per - 10 - 10) . '>&laquo; Previous</a>';
		} else {
			$privious = '&laquo; Previous';
		}




		$view_link = l($view_img, 'alc-view-ammend-license/' . encryption_decryption_fun('encrypt', $val->serial_no_from_v) . '/' . encryption_decryption_fun('encrypt', $val->id) . '/' . encryption_decryption_fun('encrypt', $val->created_by) . '/' . encryption_decryption_fun('encrypt', $val->is_flag) . '/' . encryption_decryption_fun('encrypt', $val->license_renewal_ammendment_id), array('html' => TRUE));





		$rows[] = array(
			array('data' => $sl_no, 'align' => 'left', 'class' => array($class_name)),
			array('data' => '<strong>[ ' . $serialLabel . ' ]</strong> <br>' . $val->name_of_contractor, 'align' => 'left', 'class' => array($class_name)),
			array('data' => $pe_reg_number . '<br>' . $registration_date, 'align' => 'left', 'class' => array($class_name)),
			array('data' => $license_no . '<br>' . $license_date, 'align' => 'left', 'class' => array($class_name)),
			array('data' => $blockMunName, 'align' => 'left', 'class' => array($class_name)),
			array('data' => $validated_date, 'align' => 'left', 'class' => array($class_name)),
			array('data' => $application_date, 'align' => 'left', 'class' => array($class_name)),
			array('data' => $STATUS, 'align' => 'left', 'class' => array($class_name)),
			array('data' => $view_link, 'align' => 'left', 'class' => array($class_name)),
		);

	}


	$variables = array(
		'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')),
		'header' => $header,
		'rows' => $rows,
		'empty' => t(ucfirst($application_status) . " application not available")
	);

	return $output = theme('table', $variables);


}

