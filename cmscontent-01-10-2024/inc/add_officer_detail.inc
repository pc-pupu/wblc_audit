<?php
drupal_add_library('system','ui.datepicker');
drupal_add_js('jQuery(document).ready(function(){
	Drupal.behaviors.addHighStockDatepicker = {
	  attach: function(context, settings){
        jQuery( "#date_of_birth").datepicker({
			dateFormat: "dd-mm-yy",
			changeMonth: true,
      		changeYear: true,
			yearRange: "-65:-17",
			maxDate : 0,
		});
	  }
	}	  
});', 'inline');
function add_officer_detail($form, $form_state, $sub_div_code, $dist_code){ 
	
	$form['officer_detail']['markup_start'] = array(
							'#type' => 'markup',
							'#markup'=> '<div class="box box-primary box-solid">
											<div class="box-header with-border"><i class="fa fa-clipboard"></i>&nbsp;Add New Particulars</div>
												<div class="box-body">'
							);
	$form['officer_detail']['sub_div_code'] = array(
							'#type'=>'hidden',
							'#value'=>$sub_div_code
							);
	$form['officer_detail']['dist_code'] = array(
							'#type'=> 'hidden',
							'#value'=> $dist_code
							);
	$value_emp_id = isset($form_state['values']['emp_id']) ? $form_state['values']['emp_id'] : '';
	if($value_emp_id != '' ){
		$query1 = db_select('l_office_emp_details', 'emp');
		$query1->fields('emp', array('name', 'lname', 'dob', 'gender', 'designation', 'category', 'per_mobile', 'personal_email'));
		$query1->condition('emp.emp_id', $value_emp_id, '=');
		$result1 = $query1->execute()->fetchAssoc();
		$name = $result1['name'].' '.$result1['lname'];
		$dob = date('d-m-Y', strtotime($result1['dob']));
		$gender = $result1['gender'];
		$designation = $result1['designation'];
		$per_mobile = $result1['per_mobile'];
		$category = $result1['category'];
		$personal_email = $result1['personal_email'];
	}else{
		$name = '';
	}
	$form['officer_detail']['emp_id'] = array(
							'#prefix' => '<div id="emp_id_replace" class="col-md-4">',
							'#suffix' => '</div>',
							'#title' => 'Employee ID(HRMS)',
							'#type' => 'textfield',
							'#attributes'=> array('autocomplete' => 'off', 'class' => array('form-control') ),
							'#required' => TRUE,
							'#autocomplete_path' => 'autocomplete_emp_id',
						  	'#ajax' => array(
										'event' => 'change',
										'effect' => 'fade',			
										'callback' => 'get_emp_info_form_ajax_callback',
										'progress' => '',
									 )
							);
	/*$value_emp_id = isset($form_state['values']['emp_id']) ? $form_state['values']['emp_id'] : '';
	if($value_emp_id != '' ){
		$query1 = db_select('l_office_emp_details', 'emp');
		$query1->fields('emp', array('name', 'lname', 'dob', 'gender', 'designation', 'category', 'per_mobile', 'personal_email'));
		$query1->condition('emp.emp_id', $value_emp_id, '=');
		$result1 = $query1->execute()->fetchAssoc();
		$name = $result1['name'].' '.$result1['lname'];
		$dob = date('d-m-Y', strtotime($result1['dob']));
		$gender = $result1['gender'];
		$designation = $result1['designation'];
		$per_mobile = $result1['per_mobile'];
		$category = $result1['category'];
		$personal_email = $result1['personal_email'];
	}else{
		$name = '';
	}*/
	$form['officer_detail']['name'] = array(
							'#prefix' => '<div id="name_replace" class="col-md-4">',
							'#suffix'=> '</div>',
							'#type' => 'textfield',
							'#title' => 'Full Name of the Officer',
							'#required' => TRUE,
							'#attributes'=> array('class' => array('form-control')),
							'#default_value' => !empty($name) ? $name:'',
							'#autocomplete_path' => 'autocomplete_fname',
						  	'#ajax' => array(
										'event' => 'change',
										'effect' => 'fade',			
										'callback' => 'get_emp_info_form_ajax_callback',
										'progress' => '',
									 )
							);
							
	$form['officer_detail']['date_of_birth'] = array(
							'#prefix'=> '<div id="date_of_birth_replace" class="col-md-4">',
							'#suffix'=> '</div>',
							'#title'=> 'Date of Birth',
							'#type'=> 'textfield',
							'#attributes'=> array('autocomplete' => 'off','readonly' => 'readonly','id'=>'date_of_birth','class' => array('form-control') ),
							'#required'=> TRUE,
							'#default_value' => !empty($dob) ? $dob:'',
							);						
	$form['officer_detail']['gender'] = array(
							'#prefix' => '<div id="gender_replace" class="col-md-4">',
							'#suffix' => '</div>',
							'#title' => 'Gender',
							'#type' => 'select',
							'#options' => array('' => 'Select', 'M' => t('Male'),'F' => t('Female'), 'O' => t('Other')),
							'#attributes'=> array('class' => array('form-control')),
							'#required' => TRUE,
							'#default_value' => !empty($gender) ? $gender:'',
													
							);	
	$form['officer_detail']['category'] = array(
							'#prefix' => '<div id="category_replace" class="col-md-4">',
							'#suffix' => '</div>',
							'#title' => 'Category',
							'#type' => 'select',
							'#options' => array('' => '-Select-', 'general' => t('General'), 'OBC' => t('OBC'), 'SC' => t('SC'), 'ST' => t('ST')),
							'#attributes'=> array('class' => array('form-control') ),
							//'#required' => TRUE,
							'#default_value' => !empty($category) ? $category:'',
							);
	$desig_option = array(
						'' => 'Select designation', 
						'LC' => 'LC', 
						'ADLC' => 'ADDL.LC',
						'REGISTRARTU' => 'REGISTRAR(TU)',
						'JLC' => 'JLC', 
						'DLC' => 'DLC', 
						'ALC' => 'ALC',
						'DYREGISTRARTU'	=> 'DY. REGISTRAR(TU)',
						'SUPERVISINGINSPECTORSE' => 'SUPERVISING INSPECTOR(S&E)',
						'INSPECTOR' => 'INSPECTOR',
						'HEADCLARK' => 'HEAD CLARK',
						'UDC' => 'UDC',						
						'LDC' => 'LDC',
						'STATISTICALASSISTANT'	=> 'STATISTICAL ASSISTANT',
						'CASHSARKAR' => 'CASH SARKAR',
						'TYPISTBG' => 'TYPIST BG',
						'TYPISTGRI' => 'TYPIST GR-I',
						'CLARKCUMTYPIST' => 'CLARK CUM TYPIST',
						'TYPISTSG' => 'TYPIST SG',
						'RECORDSUPPLIER' => 'RECORD SUPPLIER',
						'MUHURRIARGRI' => 'MUHURRIAR GR-I',
						'MUHURRIARGRII' => 'MUHURRIAR GR-II',
						'PEON' => 'PEON',
						'ORDERLYPEON' => 'ORDERLY PEON',
						'PCPS' => 'PCPS',
						'DCNW' => 'DCNW',
						'DRIVER' => 'DRIVER',
						'CKCO' => 'CKCO',
						'LIBRARY' => 'LIBRARY ASS.',
						'GROUPD' => 'GROUP-D'
					);						
	$form['officer_detail']['designation'] = array(
						'#prefix' => '<div id="designation_replace" class="col-md-4">',
						'#suffix' => '</div>',
						'#title' => 'Designation',
						'#type' => 'select',
						'#options' => $desig_option,
						'#attributes'=> array('class' => array('form-control') ),
						'#required' => TRUE,
						'#default_value' => !empty($designation) ? $designation:'',
						);
	
	$form['officer_detail']['mobile'] = array(
						'#prefix' => '<div id="mobile_replace" class="col-md-4">',
						'#suffix' => '</div>',
						'#title' => 'Mobile Number',
						'#type' => 'textfield',
						'#attributes'=> array('class' => array('form-control') , 'maxlength' => '10'),
						'#required' => TRUE,
						'#element_validate'=> array('MISC_PHONE_VALIDATE'),
						'#default_value' => !empty($mobile) ? $mobile:'',
						);
	$form['officer_detail']['email'] = array(
						'#prefix' => '<div id="email_replace" class="col-md-4">',
						'#suffix' => '</div>',
						'#title' => 'Email Address',
						'#type' => 'textfield',
						'#attributes'=> array('class' => array('form-control')),
						'#required' => TRUE,
						'#element_validate'=> array('MISC_EMAIL_VALIDATE'),
						'#default_value' => !empty($email) ? $email:'',
						);	
	$form['officer_detail']['markup_end'] = array(
						'#type' => 'markup',
						'#markup' => '</div></div>'
						);						
	$form['officer_detail']['save'] = array(
						'#type'=> 'submit',
						'#value'=> 'Save',
						'#attributes'=> array('class' => array('btn btn-primary'), ),
						);																																			
	
	return $form;
	
}


function add_officer_detail_submit($form, $form_state){
	$val = $form_state['values'];
	
	$query1 = db_select('l_office_emp_details', 'emp');
	$query1->fields('emp', array('emp_id'));
	$query1->condition('emp.emp_id', trim($val['emp_id']), '=');
	$result1 = $query1->execute();
		
	$fields = array(
				'name'=> $val['name'],
				'dob'=> date('Y-m-d',strtotime($val['date_of_birth'])),
				'gender'=> $val['gender'],
				'category'=> $val['category'],
				'designation'=> $val['designation'],				
				'per_mobile'=> $val['mobile'],
				'mobile'=> $val['mobile'],
				'personal_email'=> $val['email'],				
				'present_posting_dist_code' => encryption_decryption_fun('decrypt', $val['dist_code']),
				'present_posting_subdivision_code' => encryption_decryption_fun('decrypt', $val['sub_div_code'])
				);
	if(!empty($fields) && $result1->rowCount() > 0){
		$query = db_update('l_office_emp_details');
		$query->fields($fields); 
		$query->condition('emp_id', $val['emp_id']);
		$add_official = $query->execute();
	}else{
		$fields1 = array( 
				'emp_id'=> $val['emp_id']
				);
		$fields = $fields1 + $fields;
		$add_official = db_insert('l_office_emp_details')->fields($fields)->execute(); 
	}
	if($add_official){
		drupal_set_message('Successfully Saved.');
	}else{
		drupal_set_message('Some Error Occurred');
	}			
}

function get_emp_info_form_ajax_callback($form, $form_state){ 
	$commands = array();
	$commands[] = ajax_command_replace('#emp_id_replace', drupal_render($form['officer_detail']['emp_id']));
	$commands[] = ajax_command_replace('#name_replace', drupal_render($form['officer_detail']['name']));
	$commands[] = ajax_command_replace('#date_of_birth_replace', drupal_render($form['officer_detail']['date_of_birth']));
	$commands[] = ajax_command_replace('#gender_replace', drupal_render($form['officer_detail']['gender']));
	$commands[] = ajax_command_replace('#designation_replace', drupal_render($form['officer_detail']['designation']));
	$commands[] = ajax_command_replace('#category_replace', drupal_render($form['officer_detail']['category']));
	$commands[] = ajax_command_replace('#mobile_replace', drupal_render($form['officer_detail']['mobile']));
	$commands[] = ajax_command_replace('#email_replace', drupal_render($form['officer_detail']['email']));
	
	$form_state['rebuild'] = TRUE;
	
	return array('#type' => 'ajax', '#commands' => $commands);
}
function update_officer_conatact($form, $form_state, $id, $user_id){ 

	drupal_add_library('system','ui.datepicker');
	drupal_add_js('jQuery(document).ready(function(){
		Drupal.behaviors.addHighStockDatepicker = {
		  attach: function(context, settings){
			jQuery( "#date_of_birth").datepicker({
				dateFormat: "dd-mm-yy",
				changeMonth: true,
				changeYear: true,
				yearRange: "-65:-17",
				maxDate : 0,
			});
			jQuery("#retirement_date").datepicker({
				dateFormat: "dd-mm-yy",
				changeMonth: true,
				changeYear: true,
				yearRange: "-65:-17",
				maxDate : 0,
			});
		  }
		}	  
	});', 'inline');
	$query1 = db_select('l_custom_user_detail', 'cud');
	$query1->leftJoin('users', 'u', 'u.uid = cud.usr_id');
	$query1->fields('cud', array('srlno', 'usr_id', 'category_caste,', 'fullname', 'gender', 'mobile', 'degisnation', 'officenumber', 'retirement_date', 'employee_id', 'dob', 'govt_mobile', 'govt_email', 'aadhaar_no', 'epic_no', 'pan', 'category_caste', 'degisnation_officer'));
	$query1->fields('u', array('mail'));
	$query1->condition('cud.srlno', encryption_decryption_fun('decrypt', $id), '=');
	$user_info = $query1->execute()->fetchAssoc();
	
	$form['officer_detail']['markup_start'] = array(
							'#type' => 'markup',
							'#markup'=> '<div class="box box-primary box-solid">
											<div class="box-header with-border"><i class="fa fa-clipboard"></i>&nbsp;Add New Particulars</div>
												<div class="box-body">'
							);
	
	$form['officer_detail']['id'] = array(
							'#type' => 'hidden',
							'#value' => encryption_decryption_fun('decrypt', $id),							
							);
	$form['officer_detail']['user_id'] = array(
							'#type' => 'hidden',
							'#value' => encryption_decryption_fun('decrypt', $user_id),							
							);
	$form['officer_detail']['emp_id'] = array(
							'#prefix' => '<div id="emp_id_replace" class="col-md-4">',
							'#suffix' => '</div>',
							'#title' => 'Employee ID(HRMS)',
							'#type' => 'textfield',
							'#attributes'=> array('autocomplete' => 'off','readonly' => 'readonly', 'class' => array('form-control') ),
							'#required' => TRUE, 
							'#default_value' => !empty($user_info['employee_id']) ? $user_info['employee_id']:'',
							/* '#autocomplete_path' => 'autocomplete_emp_id',
						  	'#ajax' => array(
										'event' => 'change',
										'effect' => 'fade',			
										'callback' => 'get_emp_info_form_ajax_callback',
										'progress' => '',
									 ) */
							);
	
	$form['officer_detail']['name'] = array(
							'#prefix' => '<div id="name_replace" class="col-md-4">',
							'#suffix'=> '</div>',
							'#type' => 'textfield',
							'#title' => 'Full Name of the Officer',
							'#required' => TRUE,
							'#attributes'=> array('autocomplete' => 'off','readonly' => 'readonly', 'class' => array('form-control')),
							'#default_value' => !empty($user_info['fullname']) ? $user_info['fullname']:'',
							/* '#autocomplete_path' => 'autocomplete_fname',
						  	'#ajax' => array(
										'event' => 'change',
										'effect' => 'fade',			
										'callback' => 'get_emp_info_form_ajax_callback',
										'progress' => '',
									 ) */
							);
							
	$form['officer_detail']['dob'] = array(
							'#prefix'=> '<div id="date_of_birth_replace" class="col-md-4">',
							'#suffix'=> '</div>',
							'#title'=> 'Date of Birth',
							'#type'=> 'textfield',
							'#attributes'=> array('autocomplete' => 'off','readonly' => 'readonly','id'=>'date_of_birth','class' => array('form-control') ),
							'#required'=> TRUE,
							'#default_value' => !empty($user_info['dob']) ? date('d-m-Y', strtotime($user_info['dob'])):'',
							);						
	$form['officer_detail']['gender'] = array(
							'#prefix' => '<div id="gender_replace" class="col-md-4">',
							'#suffix' => '</div>',
							'#title' => 'Gender',
							'#type' => 'select',
							'#options' => array('' => 'Select', 'M' => t('Male'),'F' => t('Female'), 'O' => t('Other')),
							'#attributes'=> array('class' => array('form-control')),
							'#required' => TRUE,
							'#default_value' => !empty($user_info['gender']) ? $user_info['gender']:'',
													
							);	
	$form['officer_detail']['category_caste'] = array(
							'#prefix' => '<div id="category_replace" class="col-md-4">',
							'#suffix' => '</div>',
							'#title' => 'Category',
							'#type' => 'select',
							'#options' => array('' => '-Select-', 'general' => t('General'), 'OBC' => t('OBC'), 'SC' => t('SC'), 'ST' => t('ST')),
							'#attributes'=> array('class' => array('form-control') ),
							//'#required' => TRUE,
							'#default_value' => !empty($user_info['category_caste']) ? $user_info['category_caste']:'',
							);
	$desig_option = array(
						'' => 'Select designation', 
						'LC' => 'LC', 
						'ADLC' => 'ADDL.LC',
						'REGISTRARTU' => 'REGISTRAR(TU)',
						'JLC' => 'JLC',
						'JLCP' => 'JLC(P)', 
						'DLC' => 'DLC', 
						'DLCP' => 'DLC(P)', 
						'ALC' => 'ALC',
						'DYREGISTRARTU'	=> 'DY. REGISTRAR(TU)',
						'SUPERVISINGINSPECTORSE' => 'SUPERVISING INSPECTOR(S&E)',
						'INSPECTOR' => 'INSPECTOR',
						'HEADCLARK' => 'HEAD CLARK',
						'UDC' => 'UDC',						
						'LDC' => 'LDC',
						'STATISTICALASSISTANT'	=> 'STATISTICAL ASSISTANT',
						'CASHSARKAR' => 'CASH SARKAR',
						'TYPISTBG' => 'TYPIST BG',
						'TYPISTGRI' => 'TYPIST GR-I',
						'CLARKCUMTYPIST' => 'CLARK CUM TYPIST',
						'TYPISTSG' => 'TYPIST SG',
						'RECORDSUPPLIER' => 'RECORD SUPPLIER',
						'MUHURRIARGRI' => 'MUHURRIAR GR-I',
						'MUHURRIARGRII' => 'MUHURRIAR GR-II',
						'PEON' => 'PEON',
						'ORDERLYPEON' => 'ORDERLY PEON',
						'PCPS' => 'PCPS',
						'DCNW' => 'DCNW',
						'DRIVER' => 'DRIVER',
						'CKCO' => 'CKCO',
						'LIBRARY' => 'LIBRARY ASS.',
						'GROUPD' => 'GROUP-D'
					);						
	$form['officer_detail']['degisnation_officer'] = array(
						'#prefix' => '<div id="designation_replace" class="col-md-4">',
						'#suffix' => '</div>',
						'#title' => 'Designation',
						'#type' => 'select',
						'#options' => $desig_option,
						'#attributes'=> array('class' => array('form-control') ),
						'#required' => TRUE,
						'#default_value' => !empty($user_info['degisnation_officer']) ? $user_info['degisnation_officer']:'',
						);
	
	$form['officer_detail']['mobile'] = array(
						'#prefix' => '<div id="mobile_replace" class="col-md-4">',
						'#suffix' => '</div>',
						'#title' => 'Mobile Number',
						'#type' => 'textfield',
						'#attributes'=> array('autocomplete' => 'off','readonly' => 'readonly', 'class' => array('form-control') , 'maxlength' => '10'),
						'#required' => TRUE,
						'#element_validate'=> array('MISC_PHONE_VALIDATE'),
						'#default_value' => !empty($user_info['mobile']) ? $user_info['mobile']:'',
						);
	$form['officer_detail']['govt_mobile'] = array(
						'#prefix' => '<div id="mobile_replace" class="col-md-4">',
						'#suffix' => '</div>',
						'#title' => 'Mobile Number(Govt.)',
						'#type' => 'textfield',
						'#attributes'=> array('class' => array('form-control') , 'maxlength' => '10'),
						'#required' => TRUE,
						'#element_validate'=> array('MISC_PHONE_VALIDATE'),
						'#default_value' => !empty($user_info['govt_mobile']) ? $user_info['govt_mobile']:'',
						);
	$form['officer_detail']['officenumber'] = array(
						'#prefix' => '<div id="mobile_replace" class="col-md-4">',
						'#suffix' => '</div>',
						'#title' => 'Office Contact Number',
						'#type' => 'textfield',
						'#attributes'=> array('class' => array('form-control') , 'maxlength' => '10'),
						// '#required' => TRUE,
						// '#element_validate'=> array('MISC_PHONE_VALIDATE'),
						'#default_value' => !empty($user_info['officenumber']) ? $user_info['officenumber']:'',
						);
	$form['officer_detail']['email'] = array(
						'#prefix' => '<div id="email_replace" class="col-md-4">',
						'#suffix' => '</div>',
						'#title' => 'Email Address',
						'#type' => 'textfield',
						'#attributes'=> array('autocomplete' => 'off','readonly' => 'readonly', 'class' => array('form-control')),
						'#required' => TRUE,
						'#element_validate'=> array('MISC_EMAIL_VALIDATE'),
						'#default_value' => !empty($user_info['mail']) ? $user_info['mail']:'',
						);	
	$form['officer_detail']['govt_email'] = array(
						'#prefix' => '<div id="email_replace" class="col-md-4">',
						'#suffix' => '</div>',
						'#title' => 'Email Address(Govt.)',
						'#type' => 'textfield',
						'#attributes'=> array('class' => array('form-control')),
						'#required' => TRUE,
						'#element_validate'=> array('MISC_EMAIL_VALIDATE'),
						'#default_value' => !empty($user_info['govt_email']) ? $user_info['govt_email']:'',
						);
	$form['officer_detail']['retirement_date'] = array(
						'#prefix' => '<div id="email_replace" class="col-md-4">',
						'#suffix' => '</div>',
						'#title' => 'Retirement Date',
						'#type' => 'textfield',
						'#attributes'=> array('autocomplete' => 'off','readonly' => 'readonly','id'=>'retirement_date','class' => array('form-control') ),
						'#required' => TRUE,
						'#default_value' => !empty($user_info['retirement_date']) ? date('d-m-Y', strtotime($user_info['retirement_date'])):'',
						);
	$form['officer_detail']['aadhaar_no'] = array(
						'#prefix' => '<div id="email_replace" class="col-md-4">',
						'#suffix' => '</div>',
						'#title' => 'Aadhaar Number',
						'#type' => 'textfield',
						'#attributes'=> array('class' => array('form-control'), 'maxlength' => '12'),
						// '#required' => TRUE,
						'#default_value' => !empty($user_info['aadhaar_no']) ? $user_info['aadhaar_no']:'',
						);
	$form['officer_detail']['epic_no'] = array(
						'#prefix' => '<div id="email_replace" class="col-md-4">',
						'#suffix' => '</div>',
						'#title' => 'EPIC',
						'#type' => 'textfield',
						'#attributes'=> array('class' => array('form-control')),
						// '#required' => TRUE,
						'#default_value' => !empty($user_info['epic_no']) ? $user_info['epic_no']:'',
						);
	$form['officer_detail']['pan'] = array(
						'#prefix' => '<div id="email_replace" class="col-md-4">',
						'#suffix' => '</div>',
						'#title' => 'PAN',
						'#type' => 'textfield',
						'#attributes'=> array('class' => array('form-control'), 'maxlength' => '10'),
						// '#required' => TRUE,
						'#default_value' => !empty($user_info['pan']) ? $user_info['pan']:'',
						);
	$form['officer_detail']['markup_end'] = array(
						'#type' => 'markup',
						'#markup' => '</div></div>'
						);						
	$form['officer_detail']['save'] = array(
						'#type'=> 'submit',
						'#value'=> 'Save',
						'#attributes'=> array('class' => array('btn btn-primary'), ),
						);																																			
	
	return $form;	
}
function update_officer_conatact_submit($form, $form_state){ 
	$val = $form_state['values'];
	
	$fields_Array =	array(								
						// 'fullname' => $val['fullname'],
						// 'employee_id' => $val['employee_id'],
						// 'mobile' => $val['mobile'],
						'officenumber' => $val['officenumber'],
						// 'degisnation' => $val['degisnation'],
						// 'joining_date' => date('Y-m-d',strtotime($val['joining_date'])),
						'retirement_date' => date('Y-m-d', strtotime($val['retirement_date'])),
						'dob' => date('Y-m-d',strtotime($val['dob'])),
						'gender' => $val['gender'],
						'category_caste' => $val['category_caste'],
						'degisnation_officer' => $val['degisnation_officer'],
						'govt_mobile' => $val['govt_mobile'],
						'govt_email' => $val['govt_email'],
						'aadhaar_no' => !empty($user_info['aadhaar_no']) ? $user_info['aadhaar_no']: 0, 
						'epic_no' => $val['epic_no'],
						'pan' => $val['pan']													
						);		
		
	$query = db_update('l_custom_user_detail');
	$query->fields($fields_Array); 
	$query->condition('usr_id', $val['user_id']);
	$query->condition('srlno', $val['id']);
	$rt = $query->execute();
	
	if(!empty($rt)){
		drupal_set_message('Successfully Saved.'); 
	}
}