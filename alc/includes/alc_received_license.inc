<?php
function alc_received_license($form, &$form_state, $application_status = 0, $page_per = 0)
{

	global $base_root, $base_path, $user;

	$_SESSION['uid'] = $user->uid;
	$user_id = $user->uid;

	if (empty($application_status)) {
		$active = 'active';
	}
	if ($application_status == 'pending') {
		$activepending = 'active';
	} else {
		$activepending = '';
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

	/*$form['contractor_license_lists'] = array(
					'#type' 				=> 'fieldset',
					'#attributes'		 	=> array('class'=>array(''))
					);*/

	$get_all_contractor_license = contractor_license_list($application_status, $page_per);

	$form['contractor_license_lists']['markup_data_1'] = array(
		'#prefix' => '<div class="nav-tabs-custom">
													<ul class="nav nav-tabs">
														<li class="' . $active . '">' . l('All', 'alc-received-license') . '</li>
														<li class="' . $activepending . '">' . l('Pending', 'alc-received-license/pending') . '</li>
														<li class="' . $activeforward . '">' . l('Forward', 'alc-received-license/forward') . '</li>
														<li class="' . $activefinalsubmit . '">' . l('Final Submit', 'alc-received-license/finalsubmit') . '</li>												
														<li class="' . $activeissued . '">' . l('Issued', 'alc-received-license/issued') . '</li>
														<li class="' . $activerejected . '">' . l('Rejected', 'alc-received-license/rejected') . '</li>
													</ul>
													<div class="tab-content">
														<div class="tab-pane active" id="fa-icons">
															<section id="new">',


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
		'#markup' => '<div id="reports_by_search_div">' . $get_all_contractor_license . '<div></section>',
	);
	return $form;
}


function contractor_license_list($application_status = '', $page_per)
{

	global $user, $base_root, $base_path;
	$uid = $user->uid;
	$page_to = 100;
	$output = '';

	$duplicate = array();
	$row_arr = array();
	// Table header
	$header = array(
		array('data' => 'Sl. No', 'field' => 'slno', 'width' => '6%'),
		array('data' => 'FORM-V / REF.No.<br/>Contractor Name', 'field' => 'name_of_contractor'),
		array('data' => 'PE Registration No.', 'field' => 'reg', 'width' => '14%'),
		array('data' => 'B/M/C/NA/SEZ', 'field' => 'created_date', 'width' => '10%'),
		array('data' => 'License Number & Date', 'field' => 'contractor_license_number', 'width' => '15%'),
		array('data' => 'Application Date', 'field' => 'license_date', 'width' => '12%'),
		array('data' => 'Status', 'field' => 'status'),
		array('data' => 'Action', 'field' => 'actions'),
	);
	// Setting the sort conditions
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
	$query_subdivision->fields('lcra', array('sub_div_code', 'district_code', 'block_code', 'custom_area_jurisdiction', 'is_custom_area_jurisdiction', 'rlo_code', 'lwfc_code'));
	$query_subdivision->condition('lcra.user_id', $uid, '=');
	$result_query_subdivision = $query_subdivision->execute();
	$result_arr = $result_query_subdivision->fetchAssoc();

	$customjurisdiction = $result_arr['custom_area_jurisdiction'];
	$iscustom_juris 	= $result_arr['is_custom_area_jurisdiction'];
	$subdivision_alc 	= $result_arr['sub_div_code'];
	$rlo_code 			= $result_arr['rlo_code'];
	$lwfc_code 			= $result_arr['lwfc_code']; // echo '<pre>'; print_r($result_arr); die;

	if ($customjurisdiction == 'BMCNS' && $iscustom_juris == 'Y') {
		$block_code_arr = db_select('l_customuser_reletion_services_wise_user_settings', 'cra')
			->fields('cra', array('block_code'))
			->condition('cra.user_id', $uid)
			->execute()->fetchObject()->block_code;
		$block_code_arr = explode(',', $block_code_arr);
	}



	$query = db_select('l_contractor_info_master', 'infomaster');
	$query->InnerJoin('l_particular_of_contract_labour', 'particular', 'particular.serial_no_from_v= infomaster.id');
	$query->InnerJoin('l_contractor_license_application', 'license', 'particular.id= license.contractor_particular_id');
	$query->InnerJoin('l_clra_registration_master', 'clra_master', 'clra_master.id= infomaster.application_id');

	// Adding fields
	$query->fields('infomaster', array('name_of_contractor', 'application_id', 'id', 'contractor_type'));
	$query->fields('particular', array('id', 'serial_no_from_v', 'contractor_subdivision', 'name_work_site_areatype'));
	$query->fields('license', array('contractor_license_number', 'final_status', 'status', 'created_date', 'license_date', 'contractor_user_id'));
	$query->fields('clra_master', array('registration_number'));

	if (($iscustom_juris == 'Y') && (!empty($block_code_arr)) && ($customjurisdiction == 'BMCNS')) {

		$query->condition('particular.name_work_site_areatype', $block_code_arr, 'IN');
	} else {
		if ($user->uid != 1094) {			
			if($rlo_code!= ''){
				$query->condition('particular.rlo_code', $rlo_code);
			}else{
				$query->condition('particular.worksite_subdivision', $subdivision_alc);
			}
		}else{
			if($rlo_code!= ''){
				$query->condition('particular.rlo_code', $rlo_code);
			}else{
				$query->condition('particular.worksite_subdivision', $subdivision_alc);
			}
		}
	}
	$query->condition('license.final_status', 'F');
	$query->isNull('license.is_backlog_license');

	if (!empty($application_status) && $application_status == 'pending') {
		$query->condition('license.status', array('F', 'U'), 'IN');
		$query->orderBy('license.id', 'ASC');
	} elseif (!empty($application_status) && $application_status == 'issued') {
		$query->condition('license.status', 'I');
		$query->orderBy('license.id', 'DESC');
	} elseif (!empty($application_status) && $application_status == 'finalsubmit') {
		$query->condition('license.status', 'U');
		$query->orderBy('license.id', 'ASC');
	} elseif (!empty($application_status) && $application_status == 'rejected') {
		$query->condition('license.status', 'R');
		$query->orderBy('license.id', 'DESC');
	} elseif (!empty($application_status) && $application_status == 'forward') {
		$query->condition('license.status', 'FW');
		$query->orderBy('license.id', 'ASC');
	} elseif (empty($application_status)) {
		$query->orderBy('license.id', 'DESC');
	}
	$total_all = $query->execute()->rowCount();
	if ($page_per < 0) {
		$query->range(0, $page_to);
	} else {
		$query->range($page_per, $page_to);
	}
	$result = $query->execute()->fetchAll();


	$sl_no = 1;
	$a = 1;

	if (!empty($result)) {


		foreach ($result as $row) {
			$sl_no = ++$page_per;

			$blockMunData = db_query('select block_mun_name from block_mun_master where block_code=' . $row->name_work_site_areatype)->fetchAssoc();
			$blockMunName = $blockMunData['block_mun_name'];

			$query_frmv = db_select('l_contractor_info_master', 'cim');
			$query_frmv = $query_frmv->fields('cim', array('is_from_v'));
			$query_frmv->condition('cim.id', $row->id);
			$is_frm_v = $query_frmv->execute()->fetchObject();
			$is_frm_v = $is_frm_v->is_from_v;

			/*if($is_frm_v=='0'){ 
																																																											 $serialLabel='N/A';
																																																											 }else{*/
			if ($row->contractor_type == 1) {
				$serialLabel = 'REF No : 00' . $row->serial_no_from_v;
			} else {
				$serialLabel = 'FORM-V : 00' . $row->serial_no_from_v;
			}
			// }

			/**** END FROM V STATUS ****/


			$application_date = date('dS M, Y', strtotime($row->created_date));
			if (empty($row->contractor_license_number) && empty($row->license_date)) {
				$license_number = 'In Process';
			} else {
				$license_number = $row->contractor_license_number;
				$licensedate = date('dS M, Y', strtotime($row->license_date));
			}

			$last_week_date = strtotime('-7 day');
			$serial_no_from_v = encryption_decryption_fun('encrypt', $row->serial_no_from_v);
			$particular_id = encryption_decryption_fun('encrypt', $row->particular_id);

			$view_link = l('<span class="badge bg-blue"><i class="fa fa-eye"></i> View Details<div>', 'alc-view-license/' . $serial_no_from_v . '/' . $particular_id, array('html' => TRUE));

			if (($applicationdate < $last_week_date) && $formStatus == "PENDING") {
				$class_name = 'pendng';
			} else {
				$class_name = 'odd';
			}
			
			if ($row->status == 'U') {
				//if($row->serial_no_from_v == 384164){
				$info = check_final_submit_data(encryption_decryption_fun('encrypt', 12), $serial_no_from_v);
				
				if (!empty($info) && $info['remark_type'] == 'U') {
					$final_submit_date = $info['remark_date'];
					
					$today = time();
					$datediff = $today - $final_submit_date;					
					// echo date('Y-m-d', $final_submit_date);
				
					$diff = round($datediff / (60 * 60 * 24));
					

					if ($diff > 30) {
						common_deemed_license_update_status(encryption_decryption_fun('encrypt', 12), $serial_no_from_v, $particular_id, encryption_decryption_fun('encrypt', $info['remark_by']));
					}
				}
				//}


			}
			if ($sl_no == $total_all) {
				$next1 = "Next &raquo;";
			} else {
				$next1 = '<a href=' . $base_root . $base_path . 'alc-received-license/' . $application_status . '/' . $page_per . '>Next &raquo;</a>';

			}
			if ($sl_no - 100 > 0) {
				$privious = '<a href=' . $base_root . $base_path . 'alc-received-license/' . $application_status . '/' . ($sl_no - 100 - 100) . '>&laquo; Previous</a>';
			} else {
				$privious = '&laquo; Previous';
			}
			if ($sl_no > 0) {
				$rows[] = array(
					array('data' => $sl_no, 'align' => 'left', 'class' => array($class_name)),
					array('data' => '<strong>[ ' . $serialLabel . ' ]</strong> <br>' . $row->name_of_contractor, 'align' => 'left', 'class' => array($class_name)),
					array('data' => $row->registration_number, 'align' => 'left', 'class' => array($class_name)),
					array('data' => $blockMunName, 'align' => 'left', 'class' => array($class_name)),
					array('data' => $license_number . '<br>' . $licensedate, 'align' => 'left', 'class' => array($class_name)),
					array('data' => $application_date, 'align' => 'left', 'class' => array($class_name)),
					array('data' => get_user_license_status($row->status), 'align' => 'left', 'class' => array($class_name)),
					array('data' => $view_link, 'align' => 'left', 'class' => array($class_name)),

				);
			}
		}

	}
	$total_foot='<strong>TOTAL ' . $total_all . ' DATA FOUND.</strong>';
	$page_foot=  '<strong>'.$privious . ' || ' . $next1.'</strong>';
	$footer[] = array(
		array('data' => $total_foot, 'colspan'=>'6'),
		array('data' => $page_foot, 'colspan' => '2'),
	);

	$variables = array(
		'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')),
		'header' => $header,
		'footer' => $footer,
		'rows' => $rows,
		'empty' => t("Application not available")
	);

	$output = theme('table', $variables);
	return '<div class="feedback-scroll">' . $output . '</div>';

}

function reports_by_search($form, &$form_state)
{
	global $user, $base_root, $base_path;
	$val = $form_state['values'];
	$val['formv'] = substr($val['formv'], 2);
	if (empty($val['name']) && empty($val['formv']) && empty($val['lienceno'])) {
		return '<span style:"color:red;">Select minimium one parameter for search</span>';
	}
	//$application_status = $val['application_status'];

	$uid = $user->uid;

	$output = '';

	$duplicate = array();
	$row_arr = array();
	// Table header
	$header = array(
		array('data' => 'Sl. No', 'field' => 'slno', 'width' => '6%'),
		array('data' => $application_status . 'FORM-V / REF.No.<br/>Contractor Name', 'field' => 'name_of_contractor'),
		array('data' => 'PE Registration No.', 'field' => 'reg', 'width' => '14%'),
		array('data' => 'B/M/C/NA/SEZ', 'field' => 'created_date', 'width' => '10%'),
		array('data' => 'License Number & Date', 'field' => 'contractor_license_number', 'width' => '15%'),
		array('data' => 'Application Date', 'field' => 'license_date', 'width' => '12%'),
		array('data' => 'Status', 'field' => 'status'),
		array('data' => 'Action', 'field' => 'actions'),
	);
	// Setting the sort conditions
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
	$query_subdivision->fields('lcra', array('sub_div_code', 'district_code', 'block_code', 'custom_area_jurisdiction', 'is_custom_area_jurisdiction', 'rlo_code', 'lwfc_code'));
	$query_subdivision->condition('lcra.user_id', $uid, '=');
	$result_query_subdivision = $query_subdivision->execute();
	$result_arr = $result_query_subdivision->fetchAssoc();

	$customjurisdiction = $result_arr['custom_area_jurisdiction'];
	$iscustom_juris = $result_arr['is_custom_area_jurisdiction'];
	$subdivision_alc = $result_arr['sub_div_code'];
	$rlo_code = $result_arr['rlo_code'];
	$lwfc_code = $result_arr['lwfc_code'];

	if ($customjurisdiction == 'BMCNS' && $iscustom_juris == 'Y') {
		$block_code_arr = db_select('l_customuser_reletion_services_wise_user_settings', 'cra')
			->fields('cra', array('block_code'))
			->condition('cra.user_id', $uid)
			->execute()->fetchObject()->block_code;
		$block_code_arr = explode(',', $block_code_arr);
	}



	$query = db_select('l_contractor_info_master', 'infomaster');
	$query->InnerJoin('l_particular_of_contract_labour', 'particular', 'particular.serial_no_from_v= infomaster.id');
	$query->InnerJoin('l_contractor_license_application', 'license', 'particular.id= license.contractor_particular_id');
	$query->InnerJoin('l_clra_registration_master', 'clra_master', 'clra_master.id= infomaster.application_id');

	// Adding fields
	$query->fields('infomaster', array('name_of_contractor', 'application_id', 'id', 'contractor_type'));
	$query->fields('particular', array('id', 'serial_no_from_v', 'contractor_subdivision', 'name_work_site_areatype'));
	$query->fields('license', array('contractor_license_number', 'final_status', 'status', 'created_date', 'license_date', 'contractor_user_id'));
	$query->fields('clra_master', array('registration_number'));

	if (($iscustom_juris == 'Y') && (!empty($block_code_arr)) && ($customjurisdiction == 'BMCNS')) {
		$query->condition('particular.name_work_site_areatype', $block_code_arr, 'IN');
	}else{				
		if($rlo_code!= ''){
			$query->condition('particular.rlo_code', $rlo_code);
		}else{
			$query->condition('particular.worksite_subdivision', $subdivision_alc);
		}	
	}
	$query->condition('license.final_status', 'F');
	$query->isNull('license.is_backlog_license');

	// if (!empty($application_status) && $application_status == 'pending') {
	// 	$query->condition('license.status', array('F', 'U'), 'IN');
	// 	$query->orderBy('license.id', 'ASC');
	// } elseif (!empty($application_status) && $application_status == 'issued') {
	// 	$query->condition('license.status', 'I');
	// 	$query->orderBy('license.id', 'DESC');
	// } elseif (!empty($application_status) && $application_status == 'finalsubmit') {
	// 	$query->condition('license.status', 'U');
	// 	$query->orderBy('license.id', 'ASC');
	// } elseif (!empty($application_status) && $application_status == 'rejected') {
	// 	$query->condition('license.status', 'R');
	// 	$query->orderBy('license.id', 'DESC');
	// } elseif (!empty($application_status) && $application_status == 'forward') {
	// 	$query->condition('license.status', 'FW');
	// 	$query->orderBy('license.id', 'ASC');
	// } elseif (empty($application_status)) {
	// 	$query->orderBy('license.id', 'DESC');
	// }
	$query->orderBy('license.id', 'DESC');
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

	$result = $query->execute()->fetchAll();


	$sl_no = 1;
	$a = 1;

	if (!empty($result)) {


		foreach ($result as $row) {
			$sl_no = ++$page_per;

			$blockMunData = db_query('select block_mun_name from block_mun_master where block_code=' . $row->name_work_site_areatype)->fetchAssoc();
			$blockMunName = $blockMunData['block_mun_name'];

			$query_frmv = db_select('l_contractor_info_master', 'cim');
			$query_frmv = $query_frmv->fields('cim', array('is_from_v'));
			$query_frmv->condition('cim.id', $row->id);
			$is_frm_v = $query_frmv->execute()->fetchObject();
			$is_frm_v = $is_frm_v->is_from_v;

			/*if($is_frm_v=='0'){ 
																																																													  $serialLabel='N/A';
																																																													  }else{*/
			if ($row->contractor_type == 1) {
				$serialLabel = 'REF No : 00' . $row->serial_no_from_v;
			} else {
				$serialLabel = 'FORM-V : 00' . $row->serial_no_from_v;
			}
			// }

			/**** END FROM V STATUS ****/


			$application_date = date('dS M, Y', strtotime($row->created_date));
			if (empty($row->contractor_license_number) && empty($row->license_date)) {
				$license_number = 'In Process';
			} else {
				$license_number = $row->contractor_license_number;
				$licensedate = date('dS M, Y', strtotime($row->license_date));
			}

			$last_week_date = strtotime('-7 day');
			$serial_no_from_v = encryption_decryption_fun('encrypt', $row->serial_no_from_v);
			$particular_id = encryption_decryption_fun('encrypt', $row->particular_id);

			$view_link = l('<span class="badge bg-blue"><i class="fa fa-eye"></i> View Details<div>', 'alc-view-license/' . $serial_no_from_v . '/' . $particular_id, array('html' => TRUE));

			if (($applicationdate < $last_week_date) && $formStatus == "PENDING") {
				$class_name = 'pendng';
			} else {
				$class_name = 'odd';
			}
			if ($row->status == 'U') {
				//if($row->serial_no_from_v == 384164){
				$info = check_final_submit_data(encryption_decryption_fun('encrypt', 12), $serial_no_from_v);
				if (!empty($info) && $info['remark_type'] == 'U') {
					$final_submit_date = $info['remark_date'];
					$today = time();
					$datediff = $today - $final_submit_date;
					$diff = round($datediff / (60 * 60 * 24));

					if ($diff > 30) {
						common_deemed_license_update_status(encryption_decryption_fun('encrypt', 12), $serial_no_from_v, $particular_id, encryption_decryption_fun('encrypt', $info['remark_by']));
					}
				}
				//}


			}

			if ($sl_no > 0) {
				$rows[] = array(
					array('data' => $sl_no, 'align' => 'left', 'class' => array($class_name)),
					array('data' => '<strong>[ ' . $serialLabel . ' ]</strong> <br>' . $row->name_of_contractor, 'align' => 'left', 'class' => array($class_name)),
					array('data' => $row->registration_number, 'align' => 'left', 'class' => array($class_name)),
					array('data' => $blockMunName, 'align' => 'left', 'class' => array($class_name)),
					array('data' => $license_number . '<br>' . $licensedate, 'align' => 'left', 'class' => array($class_name)),
					array('data' => $application_date, 'align' => 'left', 'class' => array($class_name)),
					array('data' => get_user_license_status($row->status), 'align' => 'left', 'class' => array($class_name)),
					array('data' => $view_link, 'align' => 'left', 'class' => array($class_name)),

				);
			}
		}

	}


	$variables = array(
		'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')),
		'header' => $header,
		'rows' => $rows,
		'empty' => t("Application not available")
	);

	$output = theme('table', $variables);
	return '<div class="feedback-scroll">' . $output . '</div>';

}

