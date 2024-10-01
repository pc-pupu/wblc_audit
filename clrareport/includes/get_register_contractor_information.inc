<?php
function get_register_contractor_information($form, &$form_state){
	 
	$options_first = common_custom_user_district_list_new();
	
	$value_dropdown_first = isset($form_state['values']['disname']) ? $form_state['values']['disname'] :key($options_first);	
	
	$reg_contractor_list = get_reg_contractor_list_all();	
						   			  
	$form ['rnd_report']['disname']=array(
								'#type'			 => 'select',
								'#options'		 => $options_first,
								'#default_value' => $value_dropdown_first,
								'#title'		 => t('District'),
								'#required'		 => TRUE,
								'#prefix' 		 => '<div class="col-md-3"><div class="search-form">',
	    						'#suffix' 		 => '',
								'#attributes' 	 => array('class'=>array('form-control')),
								'#ajax' 		 => array(
													'event'     => 'change',
													'callback'  => 'get_feedback_info_form_ajax_callback',
													),
					
								);
				
	$value_loc_e_dist = isset($form_state['values']['disname']) ? $form_state['values']['disname'] : '';
	
	if($value_loc_e_dist != "" || !empty($value_loc_e_dist) || is_numeric($value_loc_e_dist)){
								
	$form ['rnd_report']['subdiv']=array(
							  '#type'			=> 'select',
							  '#options' 		=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist),
							  '#title'			=> t('Sub-Division'),
							  '#required'		=> TRUE,
							  '#prefix' 		=> '<div id="subdvsn_replace">',
							  '#suffix' 		=> '</div>',
							  '#attributes' 	=> array('class'=>array('form-control')),							  
							   );
			
	}else {
		
	$form ['rnd_report']['subdiv']=array(
							  '#type'		=> 'select',
							  '#options' 	=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist),
							  '#title'		=> t('Sub-Division'),
							  '#required'	=> TRUE,
							  '#prefix' 	=> '<div id="subdvsn_replace">',
							  '#suffix' 	=> '</div>',
							  '#attributes' => array('class'=>array('form-control')),							  
						      );
	}	
												
	$form ['rnd_report']['search_btn']=array(
							'#type'		=> 'submit',
							'#value'	=> 'SEARCH',
							'#prefix' 	=> '',
							'#suffix' 	=> '</div></div>',
							'#ajax' => array(
											'callback' => 'get_current_randomized_list_submit',
											'wrapper' => 'current_randamized',
											'method' => 'html',
											'effect' => 'fade'
											)					
						);	
	$form['rnd_report']['search_info'] = array(   
						   '#prefix' => '<div class="col-md-9 content-box-inner"><h1 class="inner-page-title">Nature of work wise contractor information</h1><div id="current_randamized">'.$reg_contractor_list,
						   '#suffix' => '</div></div>',
						   );
	return $form;
}

function get_reg_contractor_list_all(){
	$header = 	array(
					array('data' => 'Sl.No.', 'field' => 'slno', 'width' => '7%'),					
					array('data' => 'Contractor Name/Address', 'field' => '', 'width' => '35%'),
					array('data' => 'Nature of Work', 'field' => '', 'width' => ''),
					array('data' => 'Operation', 'field' => '', 'width' => '10%'),					
				);
					
	$query_cr = db_select('l_contractor_license_application', 'lcla');		
	$query_cr->fields('lcla', array('contractor_user_id'));	
	$query_cr->condition('lcla.status', 'I');	
	$query_cr->groupBy('lcla.contractor_user_id');		
	$query_cr = $query_cr->execute();
	
	$counter = 0;
	if($query_cr->rowCount()>0){
		foreach ($query_cr as $obj) {
			$query_cr1 = db_select('l_contractor_info_master', 'cim');
			$query_cr1->leftjoin('l_clra_con_nature_of_work', 'cnw', 'cnw.contractor_id = cim.id');
			$query_cr1->leftjoin('contractor_works', 'cw', 'cw.contractor_work_id = cnw.contractor_nature_of_work');	
			$query_cr1->fields('cim', array('id', 'name_of_contractor'));
			$query_cr1->fields('cw', array('cont_work_name'));	
			$query_cr1->condition('cim.contractor_user_id', $obj->contractor_user_id);						
			$query_cr1 = $query_cr1->execute()->fetchAssoc();		
			
			$counter++;
			$rows[]  = 	array(
							$counter,
							$query_cr1['name_of_contractor'],
							$query_cr1['cont_work_name'].'&nbsp;&nbsp;'.l('<strong style="color:#FF9333;">View More</strong>', '#', array('html' => TRUE)),
							l('<span class="view_details"></span>', '#', array('html' => TRUE)),						
							);
		}
	}
	$variables 	= array( 'header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('view-act-rules-table')), 'empty' => t("No data found!"));						
	$output 	= theme('datatable', $variables);
	
	return $output;	
}

function get_register_contractor_information_submit($form, &$form_state){
}

function get_feedback_info_form_ajax_callback($form, $form_state) {
	$commands = array();
	$commands[] = ajax_command_replace('#subdvsn_replace', drupal_render($form ['rnd_report']['subdiv']));		
	return array('#type' => 'ajax', '#commands' => $commands);
}
?>