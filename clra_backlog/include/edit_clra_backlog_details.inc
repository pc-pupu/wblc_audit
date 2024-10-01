<?php

function edit_clra_backlog_details($form, $form_state, $pre_id){	
	drupal_add_library('system','ui.datepicker');
		
	drupal_add_js('jQuery(document).ready(function(){
					  jQuery("#edit-registration-date").datepicker({
						   dateFormat: "dd-mm-yy",
						   yearRange: "-90:-1",
						   changeMonth: true, 
						   changeYear: true,
						   showOn: "button",
						   buttonImage: "/sites/default/files/images/calendar.png",
						   buttonImageOnly: true
						});
					});', 'inline');	
					
	$id 								= 	encryption_decryption_fun('decrypt', $pre_id);
	$query_clra_reg_apps				=	db_select('l_clra_principle_emp_backlog_data', 'lcpebd');
	$query_clra_reg_apps				->	fields('lcpebd',array('name_of_the_establishment', 'address_of_the_establishment', 'name_of_the_principle_emp', 'address_of_the_principle_emp', 'registration_no', 'registration_date', 'maximum_no_of_contact_labour', 'fees', 'status', 'old_fees_chat'));
	$query_clra_reg_apps				->	condition('lcpebd.id', trim($id));
	$query_clra_reg_apps_result 		= 	$query_clra_reg_apps->execute();
	
	if( $query_clra_reg_apps_result -> rowCount() > 0 ){
		$data							=	$query_clra_reg_apps_result->fetchAssoc();
		$registration_no				=	$data['registration_no'];
		$registration_date				=   date('d-m-Y', strtotime($data['registration_date']));
		$name_of_the_establishment		=	$data['name_of_the_establishment'];
		$address_of_the_establishment	=	$data['address_of_the_establishment'];
		$name_of_the_principle_emp		=	$data['name_of_the_principle_emp'];
		$address_of_the_principle_emp	=	$data['address_of_the_principle_emp'];
		$maximum_no_of_contact_labour	=	$data['maximum_no_of_contact_labour'];
		$fees							=	$data['fees'];
	}
	
	$form['backlog_details']['id'] 	= array (
						  '#type'  			=>  'hidden',
						  // '#prefix' 		=> 	'<section class="col-lg-12 connectedSortable ui-sortable">',
						  '#suffix' 		=> 	'',
						  '#default_value' 	=>	!empty($id) ? $id : ''
						);
	
	$form['backlog_details']['registration_no'] = array (
						  '#prefix' 		=> 	'<div class="box box-primary box-solid">
													<div class="box-body"><div class="form-custom col-md-6"><label class="input">',
						  '#suffix' 		=> 	'</label></div>',
						  '#title' 			=>  '1.Registration Number:',
						  '#type'  			=>  'textfield',
						  '#default_value' 	=>	!empty($registration_no) ? $registration_no : '',
						  '#attributes' 	=>  array('placeholder' => 'Enter your Registration Name'),
						  '#required'		=>	TRUE,
						  '#attributes'		=>	array('class' => array('form-control'), 'readonly' => 'readonly')
					    );
	$form['backlog_details']['registration_date'] = array (
						  '#prefix' 		=> 	'<div class="form-custom col-md-6"><label class="input">',
						  '#suffix' 		=> 	'</label></div>',
						  '#title' 			=>  '2.Registration Date',
						  '#type'  			=>  'textfield',
						  '#default_value' 	=>	!empty($registration_date) ? $registration_date : '',
						  '#attributes'		=> array('id' => 'registration_date', 'readonly'=>'readonly', 'placeholder'=> t('Select registration date')),
						  '#required'		=>	TRUE,
						  '#attributes'		=>	array('class' => array('form-control'), 'readonly' => 'readonly')
						);
	
	$form['backlog_details']['name_of_the_establishment'] = array (
						  '#prefix' 		=> 	'<div class="form-custom col-md-6"><label class="input">',
						  '#suffix' 		=> 	'</label></div>',
						  '#title' 			=>  '3.Name of the Establishment',
						  '#type'  			=>  'textfield',
						  '#default_value' 	=>	!empty($name_of_the_establishment) ? $name_of_the_establishment : '',
						  '#attributes' 	=>  array('placeholder' => 'Enter your Establishment Name', 'class' => array('form-control')),
						  '#required'		=>	TRUE
						);
	$form['backlog_details']['name_of_the_principle_emp'] = array (
						  '#prefix' 		=> 	'<div class="form-custom col-md-6"><label class="input">',
						  '#suffix' 		=> 	'</label></div>',
						  '#title' 			=>  '4.Name of the Principal Employer',
						  '#type'  			=>  'textfield',
						  '#default_value' 	=>	!empty($name_of_the_principle_emp) ? $name_of_the_principle_emp : '',
						  '#attributes' 	=>  array('placeholder' => 'Enter your Principal Employer Name', 'class' => array('form-control')),
						  '#required'		=>	TRUE
						);														
	$form['backlog_details']['address_of_the_establishment'] = array (
						  '#prefix' 		=> 	'<div class="form-custom col-md-6"><label class="input">',
						  '#suffix' 		=> 	'</label></div>',
						  '#title' 			=>  '5.Address of the Establishment',
						  '#type'  			=>  'textarea',
						  '#default_value' 	=>	!empty($address_of_the_establishment) ? $address_of_the_establishment : '',
						  '#attributes' 	=>  array('placeholder' => 'Enter your Establishment Name', 'class' => array('form-control')),
						  '#required'		=>	TRUE
						);
																					
	$form['backlog_details']['address_of_the_principle_emp'] = array (
						  '#prefix' 		=> 	'<div class="form-custom col-md-6"><label class="input">',
						  '#suffix' 		=> 	'</label></div>',
						  '#title' 			=>  '6.Address of the Principal Employer',
						  '#type'  			=>  'textarea',
						  '#default_value' 	=>	!empty($address_of_the_principle_emp) ? $address_of_the_principle_emp : '',
						  '#attributes' 	=>  array('placeholder' => 'Enter your Principal Employer Name', 'class' => array('form-control')),
						  '#required'		=>	TRUE
						);
															
	$form['backlog_details']['maximum_no_of_contact_labour'] = array (
						  '#prefix' 		=> 	'<div class="form-custom col-md-4"><label class="input">',
						  '#suffix' 		=> 	'</label></div>',
						  '#title' 			=>  '7.Maximum Number of Contract Labours',
						  '#type'  			=>  'textfield',
						  '#default_value' 	=>	!empty($maximum_no_of_contact_labour) ? $maximum_no_of_contact_labour : '',
						  '#attributes' 	=>  array('placeholder' => 'Enter Maximum Number of Contract Labours', 'class' => array('form-control')),
						  '#required'		=>	TRUE
						);
	$form['backlog_details']['fees'] = array (
						  '#prefix' 		=> 	'<div class="form-custom col-md-4"><label class="input">',
						  '#suffix' 		=> 	'</label></div>',
						  '#title' 			=>  '8.Fees',
						  '#type'  			=>  'textfield',
						  '#default_value' 	=>	!empty($fees) ? $fees : '',
						  '#attributes' 	=>  array('placeholder' => 'Enter diposited amount', 'class' => array('form-control')),
						  '#required'		=>	TRUE
						);
	$fees_chat = array(1 => 'Yes', 0 => 'No');
	
	$form['backlog_details']['old_fees_chat'] = array (
						  '#prefix' 		=> 	'<div class="form-custom col-md-4"><label class="inline-group">',
						  '#suffix' 		=> 	'</label></div></div></div>',
						  '#title' 			=>  '9.This(No.-8) fees amount calculated by old fees chat',
						  '#options' 		=>  $fees_chat,
						  '#type'  			=>  'radios',
						  '#default_value' 	=>	!empty($data['old_fees_chat']) ? $data['old_fees_chat'] : 0,						  
						  '#required'		=>	TRUE
						);
	if($data['status'] != 1){
		$form['backlog_details']['effect_on_applicant'] = array (
							  '#prefix' 		=> '<div class="form-custom col-md-12"><label class="input">',
							  '#suffix' 		=> '</label></div></div></div>',
							  '#title' 			=> 'Effect to new applicant <span style="color:#ff0000">[Note: Not applicable for issued certificate]</span>',
							  '#type'  			=> 'checkbox',
							  // '#description'	=> '<span style="color:#ff0000">[Note: applicable for issued certificate]</span>'							  
							);
	}
	$form['backlog_details']['submit'] = array (
						  '#prefix' 		=> 	'<div class="col-md-12">',  
						  '#suffix' 		=> 	'</div>',
						  '#type'  			=>  'submit',
						  '#value' 			=>	'UPDATE',
						  '#attributes' 	=> 	array('class' => array('btn btn-primary pull-left')),
						);
															
	return $form;
	
}

function edit_clra_backlog_details_validate($form, &$form_state){
	
}

function edit_clra_backlog_details_submit($form, &$form_state){
	$val = $form_state['values'];
	
	$id								=	$val['id'];
	$registration_date				=	$val['registration_date'];
	$name_of_the_establishment		=	$val['name_of_the_establishment'];
	$address_of_the_establishment	=	$val['address_of_the_establishment'];
	$name_of_the_principle_emp		=	$val['name_of_the_principle_emp'];
	$address_of_the_principle_emp	=	$val['address_of_the_principle_emp'];
	$maximum_no_of_contact_labour	=	$val['maximum_no_of_contact_labour'];
	$effect_on_applicant			=	$val['effect_on_applicant'];
	$fees							=	$val['fees'];
	
	if( $id != "" && is_numeric($id) ){
		$queryabc = db_update('l_clra_principle_emp_backlog_data');
        $queryabc->fields(array(
		
								'registration_date'				=>  date('Y-m-d', strtotime($registration_date)),
								'name_of_the_establishment'		=> $name_of_the_establishment,
								'address_of_the_establishment'	=> $address_of_the_establishment,
								'name_of_the_principle_emp'		=> $name_of_the_principle_emp,
								'address_of_the_principle_emp'	=> $address_of_the_principle_emp,
								'maximum_no_of_contact_labour'	=> $maximum_no_of_contact_labour,
								'fees'							=> $fees,
								'old_fees_chat'					=> $val['old_fees_chat'],
								'updated_date'					=> time()
         					));
							
        $queryabc->condition('id',trim($id));
        $queryabc->execute();
		
		if($effect_on_applicant == 1){
			$lcrm_queryabc 	= db_update('l_clra_registration_master');
			$lcrm_queryabc->fields(array(			
									'e_name'				  => $name_of_the_establishment,
									'e_postal_address'		  => $address_of_the_establishment,
									'full_name_principal_emp' => $name_of_the_principle_emp,
									'address_principal_emp'	  => $address_of_the_principle_emp									
								));
								
			$lcrm_queryabc->condition('backlog_id',trim($id));
			$lcrm_queryabc->condition('status', 'I', '!=');
			$lcrm_queryabc->isNull('registration_number');
			$lcrm_queryabc->execute();
	    }
		
		drupal_goto('clra-backlog-details');		
		drupal_set_message(t('Modified ata successfully updated.'));
	}
	
}