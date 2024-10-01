<?php

drupal_add_js(drupal_get_path('module', 'eodb') . '/js/custom-js.js');
drupal_add_js(drupal_get_path('module', 'eodb') . '/js/load_data.js');
drupal_add_css(drupal_get_path('module', 'eodb') . '/css/custom-css.css');

function eodb_fee_breakdown($form, &$form_state, $actid = '', $page_no = '', $start_date = '', $end_date = '', $application_no = '')
{
	global $base_root, $base_path;
	$act_id = encryption_decryption_fun('decrypt', $actid);

	if ($act_id == 1) {
		$title = 'Application for Registration of Principal Employer Under Contract Labour (R&A) Act,1970';
	} else if ($act_id == 11) {
		$title = 'Application for Amendment of Registration Certificate for Principal Employer Under Contract Labour (R&A) Act,1970';
	} else if ($act_id == 2) {
		$title = 'Application for Registration of Establishments under Building and Other Construction Workers (RE&CS) Act, 1996';
	} else if ($act_id == 4) {
		$title = 'Application for Registration of Principal Employers under Inter State Migrant Workmen (RE&CS) Act, 1979';
	} else if ($act_id == 12) {
		$title = 'Application for Licensing for Contractors under The Contract Labour (R&A) Act 1970';
	} else if ($act_id == 13) {
		$title = 'Application for Auto-Renewal of License for Contractors under The Contract Labour (R&A) Act 1970';
	} else if ($act_id == 14) {
		$title = 'Application for Amendment of License for Contractors under The Contract Labour (R&A) Act 1970';
	} else if ($act_id == 42) {
		$title = 'Application for Licensing for Contractors under Inter State Migrant Workmen (RE&CS) Act, 1979';
	} else if ($act_id == 44) {
		$title = 'Application for Auto-Renewal of License for Contractors under Inter State Migrant Workmen (RE&CS) Act, 1979';
	}

	$form['eodb_fee']['markup_data_1'] = array(
		'#markup' => '<div class="col-sm-12 content-box-inner"><div class="center"><h4 class="inner-page-title">' . $title . '</h4><hr><br/><div class="row"><div style="float:right">'.l('&larr; EODB Dashboard','eodb-dashboard',array('html' => TRUE, 'attributes' => array('class' => array('form-submit')))).'</div></div>',
		'#type' => 'markup',
	);

	$form['eodb_fee']['application_status'] = array(
		'#type' => 'hidden',
	);

	$form['eodb_fee']['application_no'] = array(
		'#prefix' => '<div class="col-md-4 text-left">',
		'#suffix' => '</div>',
		'#type' => 'textfield',
		'#title' => 'Application no.',
		'#attributes' => array('class' => array('form-control')),
	);

	$form['eodb_fee']['act_id'] = array(
		'#type' => 'hidden',
		'#default_value' => $act_id
	);

	$form['eodb_fee']['page_no'] = array(
		'#type' => 'hidden',
		'#value' => $page_no
	);

	$form['eodb_fee']['input_app_no'] = array(
		'#type' => 'hidden',
		'#value' => $application_no,
	);

	$st_arr = explode('-', $start_date);
	$st_def = array(
		'year' => $st_arr[0] ? date((int)$st_arr[0]) : date('2020'),
		'month' => $st_arr[1] ? date((int)$st_arr[1]) : date('11'),
		'day' => $st_arr[2] ? date((int)$st_arr[2]) : date('01')
	);

	$form['eodb_fee']['start_date'] = array(
		'#prefix' => '<div class="form-custom col-md-4 col-sm-6 col-xs-12 form-full-label col-half-offset text-left from-date-width"><label class="input">',
		'#suffix' => '</label></div>',
		'#title' => t('From Date'),
		'#type' => 'date',
		'#required' => TRUE,
		'#default_value' => $st_def,
		'#after_build' => array('__set_year_range')
	);

	$en_arr = explode('-', $end_date);
	$en_def = array(
		'year' => $en_arr[0] ? date((int)$en_arr[0]) : date('Y'),
		'month' => $en_arr[1] ? date((int)$en_arr[1]) : (int)date('m'),
		'day' => $en_arr[2] ? date((int)$en_arr[2]) : date('d')
	);

	$form['eodb_fee']['end_date'] = array(
		'#prefix' => '<div class="form-custom col-md-4 col-sm-6 col-xs-12 form-full-label col-half-offset text-left from-date-width"><label class="input">',
		'#suffix' => '</label></div>',
		'#title' => t('To Date'),
		'#type' => 'date',
		'#required' => TRUE,
		'#default_value' => $en_def,
		'#after_build' => array('__set_year_range')
	);

	$form['eodb_fee_result']['reset'] = array(
		'#markup' => '<div class="col-md-12" style="margin-bottom: 24px"><div class="col-md-6 text-right">' . l('RESET', 'eodb-fee-breakdown/' . $actid, array('html' => TRUE, 'attributes' => array('class' => array('btn btn-default'), 'style' => array('margin-top: 0px')))) . '</div>',
		'#type' => 'markup',
	);


	$form['eodb_fee_result']['submit'] = array(
		'#id' => 'submitBtn',
		'#value' => 'Search',
		'#attributes' => array('class' => array('btn btn-primary')),
		'#prefix' => '<div class="col-md-6 text-left">',
		'#suffix' => '</div></div>',
		'#type' => 'submit',
	);

	

	$form['eodb_fee_result']['markup_data_2'] = array(
		'#markup' => '<div id="reports_by_search_div" class="col-md-12"><div class="table-responsive">' . get_loader_data($act_id, $page_no, $start_date, $end_date, $application_no) . '<div></div></div>',
	);

	return $form;
}

// ------
function __set_year_range($form_element, $form_values)
{
	$form_element['year']['#options'] = drupal_map_assoc(range(2014, date("Y")));
	return $form_element;
}
// ------

function get_loader_data($act_id, $page_no, $start_date, $end_date, $application_no)
{
	$page_no = $page_no == '' ? $page_no = 1 : $page_no;
	$start_date = $start_date == '' ? $start_date = '2020-11-01' : $start_date;
	$end_date = $end_date == '' ? date('Y-m-d') : $end_date;

	$header = array(
		array('data' => 'Sl. No'),
		array('data' => 'Application no.'),
		array('data' => 'Application date'),
		array('data' => 'Approval date'),
		array('data' => 'Fee details (Registration fee)'),
		array('data' => 'Total fee charged'),
	);

	$per_page = 20;
	$range_start = ($page_no - 1) * $per_page;
	$range_end = $range_start + $per_page;
	$sl_no = ($page_no - 1) * $per_page + 1;

	// For Act I and XI starts ------
	if ($act_id == 1 || $act_id == 11) {
		$transaction_details = db_select('l_principle_epayments_receive_data', 'lpd');
		$transaction_details->leftJoin('l_principle_epayments_data', 'lped', 'lped.identification_no = lpd.transaction_id');
		$transaction_details->leftJoin('l_clra_registration_master', 'master', 'lped.application_id = master.id');
		$transaction_details->fields('lped', array('identification_no', 'application_id', 'act_id'));
		$transaction_details->fields('lpd', array('challanamount'));
		$transaction_details->fields('master', array('id', 'identification_number', 'application_date', 'registration_date'));
		$transaction_details->condition('lped.act_id', 1);
		if ($application_no) {
			$transaction_details->condition('master.identification_number', '%' . $application_no . '%', 'ILIKE');
		}
		if ($act_id == 1) {
			$transaction_details->isNull('master.amendment_parent_id');
		} else if ($act_id == 11) {
			$transaction_details->isNotNull('master.amendment_parent_id');
		}
		$transaction_details->condition('master.registration_date', array($start_date, $end_date), 'between');
		$transaction_details->orderBy('master.id', 'DESC');
	}
	// For Act I and XI ends ------

	// For Act II starts ------
	if ($act_id == 2) {
		$transaction_details = db_select('l_principle_epayments_receive_data', 'lpd');
		$transaction_details->leftJoin('l_principle_epayments_data', 'lped', 'lped.identification_no = lpd.transaction_id');
		$transaction_details->leftJoin('l_bocwa_registration_master', 'master', 'lped.application_id = master.id');
		$transaction_details->fields('lped', array('identification_no', 'application_id', 'act_id'));
		$transaction_details->fields('lpd', array('challanamount'));
		$transaction_details->fields('master', array('id', 'identification_number', 'application_date', 'registration_date'));
		$transaction_details->condition('lped.act_id', 2);
		if ($application_no) {
			$transaction_details->condition('master.identification_number', trim($application_no));
		}
		$transaction_details->condition('master.registration_date', array($start_date, $end_date), 'between');
		$transaction_details->orderBy('master.id', 'DESC');
	}
	// For Act II ends ------

	// For Act IV starts ------
	if ($act_id == 4) {
		$transaction_details = db_select('l_principle_epayments_receive_data', 'lpd');
		$transaction_details->leftJoin('l_principle_epayments_data', 'lped', 'lped.identification_no = lpd.transaction_id');
		$transaction_details->leftJoin('l_interstate_workman_master', 'master', 'lped.application_id = master.id');
		$transaction_details->fields('lped', array('identification_no', 'application_id', 'act_id'));
		$transaction_details->fields('lpd', array('challanamount'));
		$transaction_details->fields('master', array('id', 'identification_number', 'application_date', 'registration_date'));
		$transaction_details->condition('lped.act_id', 4);
		if ($application_no) {
			$transaction_details->condition('master.identification_number', '%' . $application_no . '%', 'ILIKE');
		}
		$transaction_details->condition('master.registration_date', array($start_date, $end_date), 'between');
		$transaction_details->orderBy('master.id', 'DESC');
	}
	// For Act IV ends ------

	// For Act XII starts ------
	if ($act_id == 12) {
		$transaction_details = db_select('l_principle_epayments_receive_data', 'lpd');
		$transaction_details->leftJoin(
			'l_principle_epayments_data',
			'lped',
			'lped.identification_no = lpd.transaction_id'
		);
		$transaction_details->leftJoin('l_contractor_license_application', 'master',	'lped.application_id = master.serial_no_from_v');
		$transaction_details->leftJoin('l_particular_of_contract_labour', 'part',	'part.serial_no_from_v = master.serial_no_from_v and master.contractor_user_id = part.contractor_user_id');
		$transaction_details->fields('lped', array('identification_no', 'application_id', 'act_id'));
		$transaction_details->fields('part', array('amount_of_licence'));
		$transaction_details->fields('master', array('id', 'caf_id_no', 'created_date', 'modification_date'));
		$transaction_details->condition('lped.act_id', 12);
		$transaction_details->condition('master.status', 'I');
		if ($application_no) {
			$transaction_details->condition('master.caf_id_no', '%' . $application_no . '%', 'ILIKE');
		}
		$transaction_details->condition('master.modification_date', array($start_date, $end_date), 'between');
		$transaction_details->orderBy('master.id', 'DESC');
	}
	// For Act XII ends ------

	// For Act XIII starts ------
	if ($act_id == 13) {
		$con_start_date = strtotime($start_date);
		$con_end_date = strtotime($end_date);
		$transaction_details = db_select('l_principle_epayments_receive_data', 'lpd');
		$transaction_details->leftJoin(
			'l_principle_epayments_data',
			'lped',
			'lped.identification_no = lpd.transaction_id'
		);
		$transaction_details->leftJoin(
			'l_contractor_license_apply_renweal',
			'master',
			'lped.application_id = master.id'
		);
		$transaction_details->fields('lped', array(
			'identification_no',
			'application_id',
			'act_id'
		));
		$transaction_details->fields('lpd', array('challanamount'));
		$transaction_details->fields('master', array(
			'id',
			'caf_id_no',
			'created_date',
			'modification_date',
			'ain_renewal'
		));
		$transaction_details->condition('lped.act_id', 13);
		if ($application_no) {
			$db_or = db_or();
			$db_or->condition('master.caf_id_no', '%' . $application_no . '%', 'ILIKE');
			$db_or->condition('master.ain_renewal', '%' . $application_no . '%', 'ILIKE');
			$transaction_details->condition($db_or);
		}
		$transaction_details->condition('master.modification_date', array($con_start_date, $con_end_date), 'between');
		$transaction_details->orderBy('master.id', 'DESC');
	}
	// For Act XIII ends ------

	// For Act XIV starts ------
	if ($act_id == 14) {
		if ($application_no) {
			$search_application = " and amendment.ain_amendment='" . trim($application_no) . "'";
		} else {
			$search_application = "";
		}

		$main_query = "SELECT application.serial_no_from_v, amendment.payable_amendment_fees AS total_amendment_fees, amendment.created_date,amendment.alc_inspector_action_date, amendment.ain_amendment,amendment.reference_no
			FROM 
				l_amendment_of_contract_labour_license amendment
			JOIN 
				l_contractor_license_application application
			ON amendment.license_id = application.id
			AND application.status = 'I'
			WHERE amendment.reference_no IN (
			SELECT 
				epay_data.application_id
			FROM 
				l_principle_epayments_data epay_data
			JOIN 
				l_principle_epayments_receive_data receive_data
				ON epay_data.identification_no = receive_data.transaction_id
			WHERE 
				epay_data.act_id = 14 and amendment.alc_inspector_action_date between '" . $start_date . "' and '" . $end_date . "'" . $search_application . " 
			)
			UNION ALL
			SELECT application.serial_no_from_v, log.payable_amendment_fees AS total_amendment_fees, log.created_date,log.alc_inspector_action_date, log.ain_amendment, log.reference_no
			FROM 
				l_amendment_of_contract_labour_license_log log
			JOIN 
				l_contractor_license_application application
			ON log.license_id = application.id
			AND application.status = 'I'
			WHERE log.reference_no IN (
			SELECT 
				epay_data.application_id
			FROM 
				l_principle_epayments_data epay_data
			JOIN 
				l_principle_epayments_receive_data receive_data
				ON epay_data.identification_no = receive_data.transaction_id
			WHERE 
				epay_data.act_id = 14 and log.alc_inspector_action_date between '" . $start_date . "' and '" . $end_date . "' 
			)
		order by reference_no desc";

		$for_row_count = db_query($main_query);
		$transaction_query = $main_query . " limit " . $per_page . " offset " . $range_start;
		$transaction_details = db_query($transaction_query);
	}
	// For Act XIV ends ------

	// For Act 42 starts ------
	if ($act_id == 42) {

		$transaction_details = db_select('l_principle_epayments_receive_data', 'lpd');
		$transaction_details->leftJoin('l_principle_epayments_data', 'lped', 'lped.identification_no = lpd.transaction_id');
		$transaction_details->leftJoin('l_interstate_license_master', 'master', 'lped.application_id = master.id');
		$transaction_details->fields('lped', array(
			'identification_no',
			'application_id',
			'act_id'
		));
		$transaction_details->fields('master', array(
			'id',
			'identification_number',
			'application_date',
			'license_date',
			'finalfees'
		));
		$transaction_details->condition('lped.act_id', array(42, 43), 'IN');
		if ($application_no) {
			$transaction_details->condition('master.identification_number', '%' . $application_no . '%', 'ILIKE');
		}
		$transaction_details->condition('master.license_date', array($start_date, $end_date), 'between');
		$transaction_details->orderBy('master.id', 'DESC');
	}
	// For Act 42 ends ------

	// For Act 44 starts ------
	if ($act_id == 44) {

		$transaction_details = db_select('l_principle_epayments_receive_data', 'lpd');
		$transaction_details->leftJoin('l_principle_epayments_data', 'lped', 'lped.identification_no = lpd.transaction_id');
		$transaction_details->leftJoin('l_interstate_license_master', 'master', 'lped.application_id = master.id');
		$transaction_details->fields('lped', array(
			'identification_no',
			'application_id',
			'act_id'
		));
		$transaction_details->fields('master', array(
			'id',
			'identification_number',
			'application_date',
			'renewal_amend_date',
			'finalfees'
		));
		$transaction_details->condition('lped.act_id', array(44, 45), 'IN');
		if ($application_no) {
			$transaction_details->condition('master.identification_number', '%' . $application_no . '%', 'ILIKE');
		}
		$transaction_details->orderBy('master.id', 'DESC');
	}
	// For Act 44 ends ------


	if ($act_id == 14) {
		$total_records = $for_row_count->rowCount();
		$trans_details_result = $transaction_details->fetchAll();
	} else {
		$total_records = $transaction_details->execute()->rowCount();
		$for_rows = $transaction_details->range($range_start, 20);
		$trans_details_result = $for_rows->execute()->fetchAll();
	}

	if ($act_id == 1 || $act_id == 11 || $act_id == 2 || $act_id == 4) {
		foreach ($trans_details_result as $index => $data) {
			$rows[] = array(
				array('data' => $sl_no . '.', 'align' => 'left'),
				array('data' => $data->identification_number, 'align' => 'left'),
				array('data' => $data->application_date, 'align' => 'left'),
				array('data' => date('d-m-Y', strtotime($data->registration_date)), 'align' => 'left'),
				array('data' => '&#8377;' . $data->challanamount, 'align' => 'left'),
				array('data' => '&#8377;' . $data->challanamount, 'align' => 'left'),
			);
			$sl_no++;
		}
	} else if ($act_id == 12) {
		foreach ($trans_details_result as $index => $data) {
			$rows[] = array(
				array('data' => $sl_no . '.', 'align' => 'left'),
				array('data' => $data->caf_id_no, 'align' => 'left'),
				array('data' => $data->created_date, 'align' => 'left'),
				array('data' => $data->modification_date, 'align' => 'left'),
				array('data' => '&#8377;' . $data->amount_of_licence, 'align' => 'left'),
				array('data' => '&#8377;' . $data->amount_of_licence, 'align' => 'left'),
			);
			$sl_no++;
		}
	} else if ($act_id == 13) {
		foreach ($trans_details_result as $index => $data) {
			$rows[] = array(
				array('data' => $sl_no . '.', 'align' => 'left'),
				array('data' => $data->ain_renewal . '<br/>' . $data->caf_id_no, 'align' => 'left'),
				array('data' => $data->created_date, 'align' => 'left'),
				array('data' => date('d-m-Y', $data->modification_date), 'align' => 'left'),
				array('data' => '&#8377;' . $data->challanamount, 'align' => 'left'),
				array('data' => '&#8377;' . $data->challanamount, 'align' => 'left'),
			);
			$sl_no++;
		}
	} else if ($act_id == 14) {
		foreach ($trans_details_result as $index => $data) {
			$rows[] = array(
				array('data' => $sl_no . '.', 'align' => 'left'),
				array('data' => $data->ain_amendment, 'align' => 'left'),
				array('data' => date('d-m-Y', strtotime($data->created_date)), 'align' => 'left'),
				array('data' => date('d-m-Y', strtotime($data->alc_inspector_action_date)), 'align' => 'left'),
				array('data' => '&#8377;' . $data->total_amendment_fees, 'align' => 'left'),
				array('data' => '&#8377;' . $data->total_amendment_fees, 'align' => 'left'),
			);
			$sl_no++;
		}
	} else if ($act_id == 42) {
		foreach ($trans_details_result as $index => $data) {
			$rows[] = array(
				array('data' => $sl_no . '.', 'align' => 'left'),
				array('data' => $data->identification_number, 'align' => 'left'),
				array('data' => $data->application_date, 'align' => 'left'),
				array('data' => $data->license_date, 'align' => 'left'),
				array('data' => '&#8377;' . $data->finalfees, 'align' => 'left'),
				array('data' => '&#8377;' . $data->finalfees, 'align' => 'left'),
			);
			$sl_no++;
		}
	} else if ($act_id == 44) {
		foreach ($trans_details_result as $index => $data) {
			$rows[] = array(
				array('data' => $sl_no . '.', 'align' => 'left'),
				array('data' => $data->identification_number, 'align' => 'left'),
				array('data' => $data->application_date, 'align' => 'left'),
				array('data' => $data->renewal_amend_date, 'align' => 'left'),
				array('data' => '&#8377;' . $data->finalfees, 'align' => 'left'),
				array('data' => '&#8377;' . $data->finalfees, 'align' => 'left'),
			);
			$sl_no++;
		}
	}

	// Setting up query string starts ------
	$query_string = '/' . $start_date . '/' . $end_date;
	if ($application_no) {
		$query_string .= '/' . $application_no;
	}
	// Setting up query string ends ------

	// Pagination logic starts ------
	if ($total_records > 10) {
		global $base_root, $base_path;
		$total_pages = ceil($total_records / $per_page);
		$act_id_enc = encryption_decryption_fun('encrypt', $act_id);

		$prev_page = (int)$page_no == 1 ? 1 : (int)$page_no - 1;
		$next_page = ($page_no == '' || (int)$page_no == 1) ? 2 : (int)$page_no + 1;

		$prev_url = '<a href=' . $base_root . $base_path . 'eodb-fee-breakdown/' . $act_id_enc . '/' . $prev_page . $query_string . '>&laquo; Previous</a>';
		$next_url = '<a href=' . $base_root . $base_path . 'eodb-fee-breakdown/' . $act_id_enc . '/' . $next_page . $query_string . '>Next &raquo;</a>';

		$total_foot = '<strong>TOTAL ' . $total_records . ' RECORDS FOUND IN ' . $total_pages . ' PAGES.</strong>';
		$page_foot =  '<strong>' . $prev_url . ' || ' . $next_url . '</strong>';
		$footer[] = array(
			array('data' => $total_foot, 'colspan' => '5'),
			array('data' => $page_foot, 'colspan' => '1'),
		);
	} else {
		$footer[] = array();
	}
	// Pagination logic ends ------

	$variables = array(
		'attributes' => array('class' => array('view-act-rules-table fixed-headers')),
		'header' => $header,
		'footer' => $footer,
		'rows' => $rows,
		'empty' => t("Application not available")
	);
	$output = theme('table', $variables);

	return $output;
}

function eodb_fee_breakdown_submit($form, &$form_state)
{
	$val = $form_state['values'];
	$application_no = $val['application_no'] ? $val['application_no'] : null;
	$act_id_enc = encryption_decryption_fun('encrypt', $val['act_id']);

	$start_date = $val['start_date'] ? $val['start_date'] : null;
	$filter_start_date = $start_date['year'] . '-' . $start_date['month'] . '-' . $start_date['day'];
	$formatted_start_date = date('Y-m-d', strtotime($filter_start_date));

	$end_date = $val['end_date'] ? $val['end_date'] : null;
	$filter_end_date = $end_date['year'] . '-' . $end_date['month'] . '-' . $end_date['day'];
	$formatted_end_date = date('Y-m-d', strtotime($filter_end_date));

	$query_string = '/' . $formatted_start_date . '/' . $formatted_end_date;
	if ($application_no) {
		$query_string .= '/' . $application_no;
	}

	drupal_goto('eodb-fee-breakdown/' . $act_id_enc . '/1' . $query_string);
}
