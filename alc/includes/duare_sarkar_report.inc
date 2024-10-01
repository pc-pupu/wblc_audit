<?php
drupal_add_library('system', 'ui.datepicker');

drupal_add_js('jQuery(document).ready(function(){
	jQuery("#report_date_id").datepicker({
	dateFormat: "dd-mm-yy",
    minDate: new Date(2014, 12, 01),
	maxDate: 0,
	changeMonth: true,
	changeYear: true,
	yearRange: "-60:+10",
	});
});', 'inline');

function admin_ds_report_form($form, &$form_state)
{
    global $user, $base_root, $base_path;;

    // $user_id = $user->uid;

    $form['markup_start'] = array(
        '#type'         => 'markup',
        '#markup'        => '<div class="row">
											<div class="col-md-12">
												<div class="box box-primary">
													<div class="box-body">'
    );

    // District starts --------------------------------- //

    $query2 = db_select('l_customuser_relation_address', 'cra');
    $query2->fields('cra', array());
    $query2->condition('cra.user_id', $user->uid);
    $result2 = $query2->execute()->fetchObject();

    if (in_array('SUPER ADMIN', $user->roles) || in_array('ADDLC', $user->roles) || in_array('DLC', $user->roles)) {
        $options_dist = common_custom_user_district_list_new();
    } else {
        $options_dist = get_district_jurisdiction($user->uid);
    }

    $form['district'] = array(
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>',
        '#title' => 'Select District',
        '#type' => 'select',
        '#options' => $options_dist,
        '#default_value' => !empty($result2->district_code) ? $result2->district_code : '',
        '#required'  => TRUE,
        '#attributes' => array('class' => array('form-control')),
        '#ajax' => array(
            'event' => 'change',
            'effect' => 'fade',
            'callback' => 'get_address_ajax_callback',
            'progress' => array('type' => 'throbber'),
        )
    );

    // District ends ----------------------------------- //

    // Subdivision starts ------------------------------- //

    $value_district = isset($form_state['values']['district']) ? $form_state['values']['district'] : $result2->district_code;

    if ($value_district != "" || !empty($value_district)) {
        $form['subdivision'] = array(
            '#prefix' => '<div class="col-md-3" id="subdvsn_code_replace">',
            '#suffix' => '</div>',
            '#title' => 'Select Subdivision',
            '#type' => 'select',
            '#options' => common_custom_user_subdivisionlist_list_new($value_district),
            '#default_value' => !empty($result2->sub_div_code) ? $result2->sub_div_code : '',
            '#attributes' => array('class' => array('form-control')),
            '#ajax'              => array(
                'event'             => 'change',
                'effect'             => 'fade',
                'callback'            => 'get_address_ajax_callback',
                'progress'            => array('type' => 'throbber'),
            )
        );
    } else {
        $form['subdivision'] = array(
            '#prefix' => '<div class="col-md-3" id="subdvsn_code_replace">',
            '#suffix' => '</div>',
            '#title' => 'Select Subdivision',
            '#type' => 'select',
            '#attributes' => array('class' => array('form-control')),
            // '#options' => common_custom_user_subdivisionlist_list_new($value_district),
            // '#default_value' => !empty($result2->sub_div_code) ? $result2->sub_div_code : '',
            // '#attributes' => array('class' => array('form-control')),
            // '#ajax'              => array(
            //     'event'             => 'change',
            //     'effect'             => 'fade',
            //     'callback'            => 'get_address_ajax_callback',
            //     'progress'            => array('type' => 'throbber'),
            // )
        );
    }

    // Subdivision ends ------------------------------- //

    // Block starts ----------------------------------- //

    $value_subdiv = isset($form_state['values']['subdivision']) ? $form_state['values']['subdivision'] : $result2->sub_div_code;

    if ($value_district != "" && !empty($value_subdiv) && is_numeric($value_subdiv)) {
        $form['blockmun_code']          = array(
            '#prefix'        => '<div class="col-md-3" id="blockmun_code_replace">',
            '#suffix'        => '</div>',
            '#title'        => 'Select Block/Mun./Crop./SEZ/NA',
            '#type'            => 'select',
            '#attributes'    => array('class' => array('form-control')),
            '#options'        => report_block_mun_list_new($value_district, $value_subdiv)
        );
    } else {
        $form['blockmun_code']          = array(
            '#prefix'        => '<div class="col-md-3" id="blockmun_code_replace">',
            '#suffix'        => '</div>',
            '#title'        => 'Select Block/Mun./Crop./SEZ/NA',
            '#type'            => 'select',
            '#attributes'    => array('class' => array('form-control')),
            // '#options'        => report_block_mun_list_new($value_district, $value_subdiv)
        );
    }

    // Block ends ------------------------------------- //

    $form['report_date'] = array(
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>',
        '#type' => 'textfield',
        '#title' => 'Report Date',
        '#required' => TRUE,
        '#default_value' =>    date('d-m-Y', time() - 60 * 60 * 30 * 24),
        '#attributes' => array('id' => 'report_date_id', 'class' => array('form-control'), 'readonly' => 'readonly'),
    );

    // Buttons starts ------------------------------------ //

    $form['search'] = array(
        '#prefix' => '<div class="flex">',
        // '#suffix' => '</div>',
        '#type' => 'button',
        '#value' => 'GENERATE',
        '#attributes' => array('class' => array('btn btn-primary pull-left')),
        '#ajax'     => array(
            'callback' => 'ds_reports_by_search',
            'wrapper'  => 'ds_reports_by_search_div',
            'method'   => 'html',
            'effect'   => 'fade'
        )
    );
    
    $form['reset'] = array(
        // '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>',
        // '#type' => 'reset',
        '#type' => 'button',
        '#value' => 'RESET',
        '#attributes' => array('class' => array('btn btn-default pull-left')),
        '#ajax'     => array(
            'callback' => 'reset_form_fields',
            'method'   => 'html',
            'effect'   => 'fade'
        )
    );

    // Buttons ends ------------------------------------ //

    $form['footer'] = array(
        '#markup' => '</div></div></div></div>',
        '#type' => 'markup'
    );

    $form['datamarkup'] = array(
        '#type'     => 'markup',
        '#markup'     => '<div class="box box-primary">
				<div class="box-header"><h3 class="box-title"><i class="fa fa-list"></i> Records of DS report </h3></div>
				<div class="box-body" id = "ds_reports_by_search_div"></div>
				</div>'
    );

    return $form;
}

function ds_reports_by_search($form, &$form_state)
{
    global $base_root, $base_path,$user;

    $val = $form_state['values'];


    $val['source']				         = 'WBLC';
	   $val['taskid']					= 'DUARESARKARGETREPORT';
	   $val['admin_id']					= $user->uid;;


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
				curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($val));
				curl_setopt($ch, CURLOPT_POST, true);
				$results    = curl_exec($ch);
				$errors     = curl_error($ch); 
				//print_r($errors); 
				curl_close($ch);			
				$results_migrantcheck = json_decode(trim($results), TRUE);
                print_r($results_migrantcheck);
  
}

function reset_form_fields($form, &$form_state){
    $val = $form_state['values'];
    $val['district_output'] = '17';

    return $val['district_output'];
}

function check_empty_validation($element, &$form_state)
{
    $field = trim($element['#value']);
    if ($field == '' || empty($field)) {
        form_error($element, t($element['#title'] . " field is required."));
    }
}

function get_address_ajax_callback($form, $form_state)
{
    $commands     = array();
    $commands[] = ajax_command_replace('#subdvsn_code_replace', drupal_render($form['subdivision']));
    $commands[] = ajax_command_replace('#blockmun_code_replace', drupal_render($form['blockmun_code']));
    return array('#type' => 'ajax', '#commands' => $commands);
}
