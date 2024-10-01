<?php
drupal_add_library('system','ui.datepicker');	
drupal_add_js('jQuery(document).ready(function(){
	jQuery("#show_date").datepicker({
	  dateFormat: "dd-mm-yy",
	  //minDate: "0",
	  yearRange: "-60:2018",
	  changeMonth: true, 
	  changeYear: true,
	});
});', 'inline');
function view_and_approved_certificate_details($form, &$form_state, $application_id = '', $user_id = ''){
	drupal_add_css(drupal_get_path('module', 'alc').'/css/jquery-ui.css');
	drupal_add_js(drupal_get_path('module', 'alc').'/js/view_doc_js.js');
	
	$application_id = encryption_decryption_fun('decrypt', $application_id);
	$user_id = encryption_decryption_fun('decrypt', $user_id);
	
	$pre_certi_query = db_select('l_documents_upload', 'ldu');	 
	$pre_certi_query->fields('ldu', array('backlog_clra_registration_certificate_file'));			
	$pre_certi_query->condition('ldu.application_id', $application_id);
	$pre_certi_query->condition('ldu.act_id', 1);
	$pre_certi_query->condition('ldu.user_id', $user_id);		
	$pre_certi_query = $pre_certi_query->execute();
	
	$pre_certi_result = $pre_certi_query->fetchObject();		
				
	$form['clra_backlog']['note'] = array(                                           
					'#prefix' 	 => '',
					'#suffix' 	 => '',
					'#type'      => 'markup',
					'#markup'    => '<div class="box box-primary">									    
									    <div class="box-body">'
					 );
	
	 $form['gvtpyfrm']['application_id'] = array(                                           
					'#type'             => 'hidden',
					'#default_value'    => $application_id                                           
					);
	
	 $form['gvtpyfrm']['user_id'] = array(                                           
					'#type'            => 'hidden',
					'#default_value'   => $user_id                                           
					);
	
	 $form['clra_backlog']['previous_certificate'] = array(
					'#prefix' 	 => '<div class="col-md-12>',
					'#suffix' 	 => '</div>',                                            
					'#required'  => TRUE,
					'#type' 	 => 'markup',					 
					'#markup'    => '<span style="color:#f00;">Note: Click <span class="popup" id="old_rc" style="font-size:17px;font-weight:700;text-decoration:underline;">HERE</span> to view applicant uploaded registration certificate and fill up bellow information(No. 1, 2, 3).</span><div id="dialog_old_rc" title="Click here to view registration certificate" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/backlog_clra_registration_certificate/'.$pre_certi_result->backlog_clra_registration_certificate_file.'" style="min-width:600px; min-height:500px;" frameborder="0"></iframe></div>',                                       
				);
	 $form['clra_backlog']['registration_no'] = array(
					'#prefix' 	=> '<div class="col-md-12"><label class="input">',
					'#suffix' 	=> '</label></div>',
					'#title' 	=> t('1.Enter previous registration number'),
					'#required' => TRUE,
					'#type' 	=> 'textfield',
					'#attributes' => array('class' => array('form-control')),                                            
				);
	  $form['clra_backlog']['registration_date'] = array(
					'#prefix' 	=> '<div class="col-md-12"><label class="input">',
					'#suffix' 	=> '</label></div>',
					'#title' 	=> t('2.Enter previous registration date[dd-mm-yyyy]'),
					'#required' => TRUE,
					'#type' 	=> 'textfield',
					'#attributes' =>array('readonly' =>'readonly' ,'onclick' => 'datepicker_alcenter();','id' => 'show_date', 'class' => array('form-control'))
				);
																				
	 $form['clra_backlog']['paid_fees'] = array(
					'#prefix' 	=> '<div class="col-md-12"><label class="input">',
					'#suffix' 	=> '</label></div>',
					'#title' 	=> t('3.Enter previous paid fees()'),
					'#required' => TRUE,
					'#type' 	=> 'textfield',
					// '#description' => '<font color="#FF0000">* Allowed only numeric value.</font>',
					'#attributes' => array('class' => array('form-control')),                                           
				);   
    
     $form['clra_backlog']['submit'] = array (
					'#type' 	=> 'submit',
					'#value' 	=> 'SAVE & APPROVED',
					'#prefix' 	=> '<div class="col-md-2">',
					'#suffix' 	=> '</div>',
					'#attributes' => array('class' => array('btn btn-primary')),
				); 
	
	$form['clra_backlog']['skip_btn'] = array(
					'#prefix' 	=> '<div class="col-md-10">',
					'#suffix' 	=> '</div></div></div>',                                            
					'#markup' 	=> l('SKIP','alc-visible-applications/'.encryption_decryption_fun('encrypt', $application_id).'/'.encryption_decryption_fun('encrypt', $user_id),array('attributes' => array('class' => 'btn btn-primary'))),
					'#type' 	=> 'markup',                                            
				);
     return $form;	

}

function view_and_approved_certificate_details_submit($form, &$form_state){
	 global $user;
	 $val            = $form_state['values'];
     $application_id = $val['application_id'];
	 $user_id 		 = $val['user_id'];
	 
	 $pre_certi_query = db_select('l_clra_registration_master', 'lcrm');	 
	 $pre_certi_query->fields('lcrm', array('e_name', 'loc_e_name', 'loc_e_subdivision', 'loc_e_dist', 'name_areatype', 'full_name_principal_emp', 'address_principal_emp', 'loc_emp_dist', 'loc_emp_subdivision', 'l_emp_ps', 'loc_emp_subdivision', 'emp_name_areatype', 'loc_emp_vill_ward', 'e_any_day_max_num_of_workmen'));			
	 $pre_certi_query->condition('lcrm.id', $application_id);
	 $pre_certi_query->condition('lcrm.act_id', 1);
	 $pre_certi_query->condition('lcrm.user_id', $user_id);		
	 $pre_certi_query = $pre_certi_query->execute();
	
	 $pre_certi_result = $pre_certi_query->fetchObject();	
	 
	 $fielddarray =  array(
						'registration_no'				=>	$val['registration_no'],
						'registration_date'				=>	date("Y-m-d", strtotime($val['registration_date'])),					
						'name_of_the_establishment'		=>	$pre_certi_result->e_name,
						'address_of_the_establishment'	=>  $pre_certi_result->loc_e_name,
						'subdivision_id_establishment'	=>	$pre_certi_result->loc_e_subdivision,
						'subdivision_id'				=>	$pre_certi_result->loc_e_subdivision,						
						'district_code_establishment'	=>	$pre_certi_result->loc_e_dist,
						'block_mun_id_establishment'	=>	$pre_certi_result->name_areatype,						
						'name_of_the_principle_emp'		=>	$pre_certi_result->full_name_principal_emp,
						'address_of_the_principle_emp'	=>  $pre_certi_result->address_principal_emp,
						'district_code_pe'				=>	$pre_certi_result->loc_emp_dist,
						'subdivision_id_pe'				=>	$pre_certi_result->loc_emp_subdivision,
						'block_mun_id_pe'				=>	$pre_certi_result->emp_name_areatype,
						'village_code_pe'				=>	$pre_certi_result->loc_emp_vill_ward,
						'police_station_code_pe'		=>	$pre_certi_result->l_emp_ps,						
						'maximum_no_of_contact_labour'	=>	$pre_certi_result->e_any_day_max_num_of_workmen,				
						'fees'							=>	$val['paid_fees'],
						'status'						=>  1,
						'user_id'						=>  $user->uid
						);
	
	// echo '<pre>';
	// print_r($fielddarray); // die;
	 
	$backlog_id = db_insert('l_clra_principle_emp_backlog_data')->fields($fielddarray)->execute();
	
	if(!empty($backlog_id)){	
		$sql_query = db_update('l_clra_registration_master');
		$sql_query->fields(array(
					 'backlog_id'  	=> $backlog_id
				));
		$sql_query->condition('id', $application_id);
		// $sql_query->condition('status', '0');
		// $sql_query = db_or()->condition('status', '0')->condition('status', 'F');
		$sql_query->condition('user_id', $user_id);
		$sql_query->execute();
	}
	
	drupal_goto('alc-visible-applications/'.encryption_decryption_fun('encrypt', $application_id).'/'.encryption_decryption_fun('encrypt', $user_id));
}
?>