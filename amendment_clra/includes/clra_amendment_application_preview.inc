<?php

function clra_amendment_application_preview($form, &$form_state, $id=''){	
	drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
	drupal_add_css(drupal_get_path('module', 'alc').'/css/jquery-ui.css');
	drupal_add_js(drupal_get_path('module', 'applicant_forms').'/js/custom.js');
	
	global $base_root, $base_path, $user;
		
	$user_id					= $user->uid;
	$action						= 'decrypt';
	$action_encrypt				= 'encrypt';
	$application_id				= encryption_decryption_fun($action, $id);

	
	$master_part_clra_reg 		= db_select('l_clra_registration_master', 'lcrm');
	$master_part_clra_reg->fields('lcrm', array('id','final_submit_status', 'status', 'backlog_id', 'e_any_day_max_num_of_workmen', 'loc_e_subdivision', 'name_areatype'));
	$master_part_clra_reg->condition('lcrm.user_id', $user_id);
	$master_part_clra_reg->condition('lcrm.id', $application_id); 
	$master_part_clra_reg_result = $master_part_clra_reg->execute(); 
			 
	$applicant_details			= $master_part_clra_reg_result->fetchAssoc();
	
	$final_submit_status		= trim($applicant_details['final_submit_status']);
	$status						= trim($applicant_details['status']);  
	$backlog_id					= trim($applicant_details['backlog_id']);
	
	$loc_e_subdivision	= trim($applicant_details['loc_e_subdivision']);
	$name_areatype		= trim($applicant_details['name_areatype']); 
	
	if($status == 'B' || $status == ''){
	
		if(empty($loc_e_subdivision) && empty($name_areatype)){
		$form['clra_app_final_preview']['not_empty_est_location'] = array(
																		'#type'		=> 'markup',
																		'#markup' 	=>  '<div style="color:#f00;"><strong style="color:#f00;">&Prime;Name and location of the Establishment&Prime;</strong> details should not be empty. Please save<strong> Application Details</strong> first.</div>',																	
																		'#prefix' 	=> '<fieldset>',  
																		'#suffix' 	=> '',
																	);
	}else{
		
	/* Contract labour checking start */
	
	$total_cl_query = db_query("select max(contractor_max_no_of_labours_on_any_day) as totalcl from l_contractor_info_master where application_id='".$applicant_details['id']."' and status = 1 ");
	$total_cl_result = $total_cl_query->fetchObject();	
	
	if($applicant_details['e_any_day_max_num_of_workmen'] < $total_cl_result->totalcl){		
		
		$form['clra_app_final_preview']['contact_labour_check_msg']	= array(
																		'#type'		=> 'markup',
																		'#markup' 	=>  'The total value added for every contractors in field No. <u>5. Maximum number of contract labour to be employed on any day through each contractor</u> in <strong>Contractor Information</strong> Tab has exceeded from the value for field No. <u>6.) Maximum number of contract labour to be employed on any day through each contractor</u> in <strong>Application Details</strong> Tab.  <br>If want to continue , click continue button. ',																	
																		'#prefix' 	=> '<fieldset>',  
																		'#suffix' 	=> '',
																	);
		$form['clra_app_final_preview']['application_id']			= array(																		
																		'#type'			 => 'hidden',
																		'#default_value' => !empty($application_id) ? $application_id : '',
																		'#attributes' 	 => array('readonly' => 'readonly'),
																		);
	    $form['clra_app_final_preview']['e_any_day_max_num_of_workmen']	= array(																		
																		'#type'			 => 'hidden',
																		'#default_value' => $total_cl_result->totalcl,
																		'#attributes' 	 => array('readonly' => 'readonly'),
																		);
		$form['clra_app_final_preview']['update_status']			= array(																		
																		'#type'			 => 'hidden',
																		'#default_value' => 'Y',
																		'#attributes' 	 => array('readonly' => 'readonly'),
																		);
		
		$form['clra_app_final_preview']['submit']					= array(
																		'#type' 			=> 'submit',																		
																		'#value' 			=> 'CONTINUE',
																		'#attributes' 		=> array('class' => array('button')),
																		'#prefix' 			=> '',  
																		'#suffix' 			=> '', 
																		);
	}else{
			
	/* Contract labour checking end */ 
	
	
	$form['clra_app_final_preview']['header']												= array(
																								  '#type' 					=> 'markup',
																								  '#markup'		 			=> t('<div class="content"><div class="" style="width:100%;"><fieldset>'),
																								);
																								
	$form['clra_app_final_preview']['application_id']										= array(
																									'#title'				=> 'Application Id',
																									'#type'					=> 'hidden',
																									'#default_value' 		=> !empty($application_id) ? $application_id : '',
																									'#attributes' 			=> array('readonly' => 'readonly'),
																									);
																									
																									
	$form['clra_app_final_preview']['backlog_id']											= array(
																									'#title'				=> 'Backlog Id',
																									'#type'					=> 'hidden',
																									'#default_value' 		=> !empty($backlog_id) ? $backlog_id : '',
																									'#attributes' 			=> array('readonly' => 'readonly'),
																									);
																									
	$form['clra_app_final_preview']['markup_data_1']										= array(
																									'#type' 				=> 'markup',
																									'#markup' 				=> view_clra_app_final_preview($application_id),
																									);
																									
	if($status == '' || $status == 'B'){ 
			
			
						
			$form['clra_app_final_preview']['finalAgree'] 									= array(																		 
																								  '#type' 					=> 'checkbox',
																								  '#description' 			=> '<span class="red-star">*</span>I hereby declare that the particulars given above are true the best of my knowledge and belief.',
																								  '#attributes'				=> array('id' => 'finalAgreeId','onchange' => 'return checkFinalAppCheckBox()'),
																								  '#required' 				=> TRUE,
																								  
																								);
	
			$form['clra_app_final_preview']['submit']										= array(
																									'#type' 				=> 'submit',
																									'#id'					=> 'submitBtn1',
																									'#value' 				=> 'SUBMIT',
																									'#attributes' 			=> array('class' => array('button')),
																									'#prefix' 				=> '</fieldset>',  
																		 							'#suffix' 				=> '</div></div>', 
																									);
																									
																									
		} else {
	
			$form['clra_app_final_preview']['download_pdf']									= array(
																									'#type'					=> 'markup',
																									'#markup' 				=>  l(' VIEW&nbsp;&amp;&nbsp;PRINT','generate-pdf-applicant/'.encryption_decryption_fun($action_encrypt, $application_id), array('html' => TRUE, 'attributes' => array('target' => '_blank', 'class' => array('button middle-align')))),
																									
																									'#prefix' 				=> '</fieldset><footer>',  
																		 							'#suffix' 				=> '</footer></div></div>',
																									);
		}	
	}
	}
	
	}else{
		drupal_set_message('You are not authorized to access this page','error');
	}
	return $form;						
	
}


function view_clra_app_final_preview($application_id){
	
		global $base_root, $base_path, $user;
		
		$user_id = $user->uid;
				
		$master_part_clra_reg = db_select('l_clra_registration_master', 'lcrm');
		$master_part_clra_reg->fields('lcrm', array());
		
		/////////////////////////  LOCATION ADDRESS ////////////////////////////////
	
		$master_part_clra_reg->fields('dm', array('district_name'));
		$master_part_clra_reg->leftJoin('district_master', 'dm', 'dm.district_code=lcrm.loc_e_dist');
		
		$master_part_clra_reg->fields('sd', array('sub_div_name'));
		$master_part_clra_reg->leftJoin('sub_division', 'sd', 'sd.sub_div_code=lcrm.loc_e_subdivision');
		
		$master_part_clra_reg->fields('bmm', array('block_mun_name'));
		$master_part_clra_reg->leftJoin('block_mun_master', 'bmm', 'bmm.block_code=lcrm.name_areatype');
		
		$master_part_clra_reg->fields('vwm', array('village_name'));
		$master_part_clra_reg->leftJoin('village_ward_master', 'vwm', 'vwm.village_code=lcrm.loc_e_vill_ward');
		
		$master_part_clra_reg->fields('bor', array('borough_name'));
		$master_part_clra_reg->leftJoin('borough_master', 'bor', 'bor.borough_code=lcrm.loc_e_borough');
		
		$master_part_clra_reg->fields('ps', array('name_of_police_station'));
		$master_part_clra_reg->leftJoin('police_station', 'ps', 'ps.police_station_code=lcrm.l_e_ps');
		
			//////////////////////////////  POSTAL ADDRESS /////////////////////////////////////	
		
		$master_part_clra_reg->fields('dm_postal', array('district_name'));
		$master_part_clra_reg->leftJoin('district_master', 'dm_postal', 'dm_postal.district_code=lcrm.e_postal_dist');
		
		$master_part_clra_reg->fields('sd_postal', array('sub_div_name'));
		$master_part_clra_reg->leftJoin('sub_division', 'sd_postal', 'sd_postal.sub_div_code=lcrm.e_postal_subdivision');
		
		$master_part_clra_reg->fields('bmm_postal', array('block_mun_name'));
		$master_part_clra_reg->leftJoin('block_mun_master', 'bmm_postal', 'bmm_postal.block_code=lcrm.e_postal_name_areatype');
		
		$master_part_clra_reg->fields('vwm_postal', array('village_name'));
		$master_part_clra_reg->leftJoin('village_ward_master', 'vwm_postal', 'vwm_postal.village_code=lcrm.e_postal_vill_ward');
		
		$master_part_clra_reg->fields('bor_postal', array('borough_name'));
		$master_part_clra_reg->leftJoin('borough_master', 'bor_postal', 'bor_postal.borough_code=lcrm.e_postal_borough');
		
		$master_part_clra_reg->fields('ps_postal', array('name_of_police_station'));
		$master_part_clra_reg->leftJoin('police_station', 'ps_postal', 'ps_postal.police_station_code=lcrm.e_postal_ps');
	
		//////////////////////////////  PRINCIPAL EMPLOYER ADDRESS /////////////////////////////////////
		
		$master_part_clra_reg->fields('sm_emp', array('statename'));
		$master_part_clra_reg->leftJoin('state_master', 'sm_emp', 'sm_emp.id=lcrm.loc_emp_state');
		
		$master_part_clra_reg->fields('dm_emp', array('district_name'));
		$master_part_clra_reg->leftJoin('district_master', 'dm_emp', 'dm_emp.district_code=lcrm.loc_emp_dist');
		
		$master_part_clra_reg->fields('sd_emp', array('sub_div_name'));
		$master_part_clra_reg->leftJoin('sub_division', 'sd_emp', 'sd_emp.sub_div_code=lcrm.loc_emp_subdivision');
		
		$master_part_clra_reg->fields('bmm_emp', array('block_mun_name'));
		$master_part_clra_reg->leftJoin('block_mun_master', 'bmm_emp', 'bmm_emp.block_code=lcrm.emp_name_areatype');
		
		$master_part_clra_reg->fields('vwm_emp', array('village_name'));
		$master_part_clra_reg->leftJoin('village_ward_master', 'vwm_emp', 'vwm_emp.village_code=lcrm.loc_emp_vill_ward');
		
		$master_part_clra_reg->fields('bor_emp', array('borough_name'));
		$master_part_clra_reg->leftJoin('borough_master', 'bor_emp', 'bor_emp.borough_code=lcrm.loc_emp_borough');
		
		$master_part_clra_reg->fields('ps_emp', array('name_of_police_station'));
		$master_part_clra_reg->leftJoin('police_station', 'ps_emp', 'ps_emp.police_station_code=lcrm.l_emp_ps'); 
	
		//////////////////////////// MANAGER ADDRESS //////////////////////////////////////
		
		$master_part_clra_reg->fields('sm_manager', array('statename'));
		$master_part_clra_reg->leftJoin('state_master', 'sm_manager', 'sm_manager.id=lcrm.loc_manager_state');
		
		$master_part_clra_reg->fields('dm_manager', array('district_name'));
		$master_part_clra_reg->leftJoin('district_master', 'dm_manager', 'dm_manager.district_code=lcrm.loc_manager_dist');
		
		$master_part_clra_reg->fields('sd_manager', array('sub_div_name'));
		$master_part_clra_reg->leftJoin('sub_division', 'sd_manager', 'sd_manager.sub_div_code=lcrm.loc_manager_subdivision');
		
		$master_part_clra_reg->fields('bmm_manager', array('block_mun_name'));
		$master_part_clra_reg->leftJoin('block_mun_master', 'bmm_manager', 'bmm_manager.block_code=lcrm.manager_name_areatype');
		
	
		$master_part_clra_reg->fields('vwm_manager', array('village_name'));
		$master_part_clra_reg->leftJoin('village_ward_master', 'vwm_manager', 'vwm_manager.village_code=lcrm.loc_manager_vill_ward');
		
		$master_part_clra_reg->fields('bor_manager', array('borough_name'));
		$master_part_clra_reg->leftJoin('borough_master', 'bor_manager', 'bor_manager.borough_code=lcrm.loc_manager_borough');
		
		$master_part_clra_reg->fields('ps_manager', array('name_of_police_station'));
		$master_part_clra_reg->leftJoin('police_station', 'ps_manager', 'ps_manager.police_station_code=lcrm.l_manager_ps');
		
		
		
		$master_part_clra_reg->condition('lcrm.user_id', $user_id);
		$master_part_clra_reg->condition('lcrm.id', $application_id);  

		$master_part_clra_reg_result = $master_part_clra_reg->execute();
		
		$content	=	$master_part_clra_reg_result->fetchAssoc();		
		
		if($content['workmen_if_same_similar_kind_of_work']=="0"){
				$similar_kind_of_work = "No";
		}else{
			$similar_kind_of_work = "Yes";
		}
		
		if($content['gender_pe']=='M'){
			$gender = 'Male';	
		}elseif($content['gender_pe']=='F'){
			$gender = 'Female';	
		}elseif($content['gender_pe']=='O'){
			$gender = 'Transgender';
		}
		
		/** For Amendment ****/
	
		if(!empty($content['amendment_parent_id'])){
			$last_amendment_id			= $content['amendment_parent_id'];
		}	
				
		$fetch_nature_data	= db_select('l_clra_reg_nature_of_work', 'lcrnow');
		$fetch_nature_data->leftJoin('contractor_works', 'cw', 'cw.contractor_work_id=lcrnow.e_nature_of_work');
		$fetch_nature_data->fields('cw', array('cont_work_name'));
		$fetch_nature_data->fields('lcrnow', array('e_nature_of_work'));
		$fetch_nature_data->condition('lcrnow.user_id', $user_id);
		$fetch_nature_data->condition('lcrnow.application_id', $application_id);
		$fetch_nature_data_result = $fetch_nature_data->execute();
		
		$content_2	=	array();
		foreach ($fetch_nature_data_result as $master_part_clra_reg_nature_row) {
			$content_2[]	=	$master_part_clra_reg_nature_row;
		}
		
		$prefix	=	'';
		$nature_of_wrk	=	'';
		foreach ($content_2 as  $res_nature_of_wrk){ 
			if($res_nature_of_wrk->e_nature_of_work == 28){
				$nature_of_wrk  	.= '';
			}else{
				$nature_of_wrk  	.= $prefix.$res_nature_of_wrk->cont_work_name;
				$prefix  	 = ', ';
			}
		}
		
			
		/***Contractor Details **/
		
		$master_part_clra_cont = db_select('l_contractor_info_master', 'lcim');
		$master_part_clra_cont->fields('lcim', array());
		
		////////////////////////////// Contractor Address part //////////////////////////
		
		$master_part_clra_cont->fields('dm_con', array('district_name'));
		$master_part_clra_cont->fields('sd_con', array('sub_div_name'));
		$master_part_clra_cont->fields('bmm_con', array('block_mun_name'));
		$master_part_clra_cont->fields('bor_con', array('borough_name'));
		$master_part_clra_cont->fields('ps_con', array('name_of_police_station'));
		$master_part_clra_cont->fields('vwm_con', array('village_name'));
		
		$master_part_clra_cont->leftJoin('district_master', 'dm_con', 'dm_con.district_code=lcim.con_loc_e_dist');
		$master_part_clra_cont->leftJoin('sub_division', 'sd_con', 'sd_con.sub_div_code=lcim.con_loc_e_subdivision');
		$master_part_clra_cont->leftJoin('block_mun_master', 'bmm_con', 'bmm_con.block_code=lcim.con_name_areatype');
		$master_part_clra_cont->leftJoin('police_station', 'ps_con', 'ps_con.police_station_code=lcim.con_l_e_ps');
		$master_part_clra_cont->leftJoin('village_ward_master', 'vwm_con', 'vwm_con.village_code=lcim.con_loc_e_vill_ward');
		$master_part_clra_cont->leftJoin('borough_master', 'bor_con', 'bor_con.borough_code=lcim.con_loc_e_borough');
		
		$master_part_clra_cont->condition('lcim.user_id', trim($user_id));
		$master_part_clra_cont->condition('lcim.application_id', trim($application_id));
		$master_part_clra_cont->condition('lcim.status', 1);
		$master_part_clra_cont->orderBy('lcim.id', 'DESC');
		
		$master_part_clra_cont_result = $master_part_clra_cont->execute();
		
		$content_3	=	array();
		foreach ($master_part_clra_cont_result as $master_part_clra_cont_result_row) {
			$content_3[]	=	$master_part_clra_cont_result_row;
		}
		
		
		
		$master_clra_fees = db_select('l_clra_registration_master', 'lcrm');
		$master_clra_fees->fields('lcrm', array('e_any_day_max_num_of_workmen'));
		$master_clra_fees->condition('lcrm.user_id', trim($user->uid));
		$master_clra_fees->condition('lcrm.id', trim($application_id));
		$master_clra_fees_result = $master_clra_fees->execute();
		
		$content_5	=	$master_clra_fees_result->fetchAssoc();
		
		$fees = $content_5['e_any_day_max_num_of_workmen'];
		
		if($fees <= 20 ){
			$retFees	=	'200';
		}else if($fees > 20 && $fees <= 50  ){
			$retFees	=	'500';
		}else if($fees > 50 && $fees <= 100  ){
			$retFees	=	'1000';
		}else if($fees > 100 && $fees <= 200  ){
			$retFees	=	'2000';
		}else if($fees > 200 && $fees <= 400  ){
			$retFees	=	'4000';
		}else if($fees > 400){
			$retFees	=	'5000';
		}
		

		$master_part_clra_docs = db_select('l_documents_upload', 'ldu');
		$master_part_clra_docs->fields('ldu', array());
		$master_part_clra_docs->condition('ldu.user_id', trim($user->uid));
		$master_part_clra_docs->condition('ldu.application_id', trim($application_id));
		$master_part_clra_docs_result = $master_part_clra_docs->execute();
		
		$content_docs	=	$master_part_clra_docs_result->fetchAssoc();
		
		
		
		$master_part_trade_union = db_select('l_clra_reg_trade_union_master', 'lcrtu');
		$master_part_trade_union->fields('lcrtu', array());
		$master_part_trade_union->condition('lcrtu.user_id', trim($user->uid));
		$master_part_trade_union->condition('lcrtu.application_id', trim($application_id));
		$master_part_trade_union_result = $master_part_trade_union->execute();
		
		$trade_union_result	=	$master_part_trade_union_result->fetchAll();
		
			if($content['loc_e_areatype']=="B"){
				$text_AreaType_loc = "Block";
			}elseif($content['loc_e_areatype']=="M"){
				$text_AreaType_loc = "Municipality";
			}elseif($content['loc_e_areatype']=="C"){
				$text_AreaType_loc = "Corporation";
			}elseif($content['loc_e_areatype']=="S"){
				$text_AreaType_loc = "SEZ";
			}elseif($content['loc_e_areatype']=="N"){
				$text_AreaType_loc = "Notified Area";
			}elseif($content['loc_e_areatype']=="O"){
				$text_AreaType_loc = "Other";
			}
			
			if($text_AreaType_loc =='Corporation') {
				$text_AreaType_loc_name='Ward';
				$areatype_loc_name = $content['village_name'];
			}elseif($text_AreaType_loc=='Municipality'){
				$text_AreaType_loc_name='Ward';
				$areatype_loc_name = 'Ward-'.$content['village_name']; 
			}elseif($text_AreaType_loc == 'Notified Area'){
				$text_AreaType_loc_name='Notified Area';
				$areatype_loc_name = $content['village_name']; 
			}else {
				$text_AreaType_loc_name='Gram Panchayat';	
				$areatype_loc_name = ucwords($content['village_name']);
			}	
			
			if($content['e_postal_areatype']=="B"){
				$text_AreaType_postal = "Block";
			}elseif($content['e_postal_areatype']=="M"){
				$text_AreaType_postal = "Municipality";
			}elseif($content['e_postal_areatype']=="C"){
				$text_AreaType_postal = "Corporation";
			}elseif($content['e_postal_areatype']=="S"){
				$text_AreaType_postal = "SEZ";
			}elseif($content['e_postal_areatype']=="N"){
				$text_AreaType_postal = "Notified Area";
			}elseif($content['e_postal_areatype']=="O"){
				$text_AreaType_postal = "Other";
			}
			
			if($text_AreaType_postal =='Corporation') {
				$text_AreaType_postal_name='Ward';
				$areatype_postal_name = $content['vwm_postal_village_name'];
			}elseif($text_AreaType_postal=='Municipality'){
				$text_AreaType_postal_name='Ward';
				$areatype_postal_name = 'Ward-'.$content['vwm_postal_village_name']; 
			}elseif($text_AreaType_postal == 'Notified Area'){
				$text_AreaType_postal_name='Notified Area';
				$areatype_postal_name = $content['vwm_postal_village_name']; 
			}else {
				$text_AreaType_postal_name='Gram Panchayat';	
				$areatype_postal_name = ucwords($content['vwm_postal_village_name']);
			}
			
			if($content['loc_emp_areatype']=="B"){
				$text_AreaType_emp = "Block";
			}elseif($content['loc_emp_areatype']=="M"){
				$text_AreaType_emp = "Municipality";
			}elseif($content['loc_emp_areatype']=="C"){
				$text_AreaType_emp = "Corporation";
			}elseif($content['loc_emp_areatype']=="S"){
				$text_AreaType_emp = "SEZ";
			}elseif($content['loc_emp_areatype']=="N"){
				$text_AreaType_emp = "Notified Area";
			}elseif($content['loc_emp_areatype']=="O"){
				$text_AreaType_emp = "Other";
			}
			
			if($text_AreaType_emp =='Corporation') {
				$text_AreaType_emp_name='Ward';
				$areatype_emp_name = $content['vwm_emp_village_name'];
			}elseif($text_AreaType_emp=='Municipality'){
				$text_AreaType_emp_name='Ward';
				$areatype_emp_name = 'Ward-'.$content['vwm_emp_village_name']; 
			}elseif($text_AreaType_emp == 'Notified Area'){
				$text_AreaType_emp_name='Notified Area';
				$areatype_emp_name = $content['vwm_emp_village_name']; 
			}else {
				$text_AreaType_emp_name='Gram Panchayat';	
				$areatype_emp_name = ucwords($content['vwm_emp_village_name']);
			}
			
			if($content['loc_manager_areatype']=="B"){
				$text_AreaType_manager = "Block";
			}elseif($content['loc_manager_areatype']=="M"){
				$text_AreaType_manager = "Municipality";
			}elseif($content['loc_manager_areatype']=="C"){
				$text_AreaType_manager = "Corporation";
			}elseif($content['loc_manager_areatype']=="S"){
				$text_AreaType_manager = "SEZ";
			}elseif($content['loc_manager_areatype']=="N"){
				$text_AreaType_manager = "Notified Area";
			}elseif($content['loc_manager_areatype']=="O"){
				$text_AreaType_manager = "Other";
			}
			if($text_AreaType_manager =='Corporation') {
				$text_AreaType_manager_name='Ward';
				$areatype_manager_name = $content['vwm_manager_village_name'];
			}elseif($text_AreaType_manager=='Municipality'){
				$text_AreaType_manager_name='Ward';
				$areatype_manager_name = 'Ward-'.$content['vwm_manager_village_name']; 
			}elseif($text_AreaType_manager == 'Notified Area'){
				$text_AreaType_manager_name='Notified Area';
				$areatype_manager_name = $content['vwm_manager_village_name']; 
			}else {
				$text_AreaType_manager_name='Gram Panchayat';	
				$areatype_manager_name = ucwords($content['vwm_manager_village_name']);
			}
			
			
			if($content['e_trade_union_areatype']=="B"){
				$text_AreaType_trade = "Block";
			}elseif($content['e_trade_union_areatype']=="M"){
				$text_AreaType_trade = "Municipality";
			}elseif($content['e_trade_union_areatype']=="C"){
				$text_AreaType_trade = "Corporation";
			}elseif($content['e_trade_union_areatype']=="S"){
				$text_AreaType_trade = "SEZ";
			}elseif($content['e_trade_union_areatype']=="N"){
				$text_AreaType_trade = "Notified Area";
			}elseif($content['e_trade_union_areatype']=="O"){
				$text_AreaType_trade = "Other";
			}
			
			if($text_AreaType_trade =='Corporation') {
				$text_AreaType_trade_name='Ward';
				$areatype_trade_name = $content['vwm_trade_village_name'];
			}elseif($text_AreaType_postal=='Municipality'){
				$text_AreaType_trade_name='Ward';
				$areatype_trade_name = 'Ward-'.$content['vwm_trade_village_name']; 
			}elseif($text_AreaType_postal == 'Notified Area'){
				$text_AreaType_trade_name='Notified Area';
				$areatype_trade_name = $content['vwm_trade_village_name']; 
			}else {
				$text_AreaType_trade_name='Gram Panchayat';	
				$areatype_trade_name = ucwords($content['vwm_trade_village_name']);
			}
			
			
		$output ='';
		$output = '<style type="text/css">
						table, td, th {border: 1px solid #006595;font-size:15px;font-family:Times New Roman;height:40px;margin-top:20px;}
						td{padding-left:13px !important;}
						th {background-color: #008BD1;color: white;}
						td:hover {background-color:#d4e3e5;}
						tr:nth-child(even) {background: #DBE5F0}
						tr:nth-child(odd) {background: #F1F1F1}
						.red-star{color:#FF0000;}
					</style>
					
					
					<table width="100%" border="0" cellspacing="0" cellpadding="0">
					  <tr>
					   <td colspan="2" align="center" style="text-align:center;font-size:17px; padding-top: 8px;"><font color="#3366FF"><strong>CLRA Amendment Registration Information</strong></font></td>
					  </tr>
					  <tr>
						<th width="51%" style="text-align:center;font-size:17px; padding-top: 8px;" >Parameters</th>
						<th width="49%" style="text-align:center;font-size:17px; padding-top: 8px;">Inputs</th>
					  </tr>
					  <tr>
						<td>Name of the Establishment </td>
						<td>'.$content['e_name'].'</td>
					  </tr>
					  <tr>
						<td>Establishment type </td>
						<td>'.ucfirst($content['est_type']).'</td>
					  </tr>
					  <tr>
						<td>Location of the Establishment</td>
						<td>'.$content['loc_e_name'].'</td>
					  </tr>
					  <tr>
						<td>District of the Establishment</td>
						<td>'.$content['district_name'].'</td>
					  </tr>
					  <tr>
						<td>Subdivision of the Establishment</td>
						<td>'.$content['sub_div_name'].'</td>
					  </tr>
					  
					  <tr>
						<td>Area type of the Establishment</td>
						<td>'.$text_AreaType_loc.'</td>
					  </tr>
					  <tr>
						<td>'.$text_AreaType_loc.' of the Establishment</td>
						<td>'.$content['block_mun_name'].'</td>
					  </tr>
					  <tr>
						<td>'.$text_AreaType_loc_name.' of the Establishment</td>
						<td>'.$areatype_loc_name.'</td>
					  </tr>
					  <tr>
						<td>Police Station of the Establishment</td>
						<td>'.$content['name_of_police_station'].'</td>
					  </tr>
					   <tr>
						<td>Pin Number of the Establishment</td>
						<td>'.$content['loc_e_pin_number'].'</td>
					  </tr>
					  <!-- Establishment Address part-->
					  
					  <tr>
						<td>Postal Address of the Establishment</td>
						<td>'.$content['e_postal_address'].'</td>
					  </tr>
					  
					  <tr>
						<td>District of the Postal Address</td>
						<td>'.$content['dm_postal_district_name'].'</td>
					  </tr>
					  <tr>
						<td>Subdivision of the Postal Address</td>
						<td>'.$content['sd_postal_sub_div_name'].'</td>
					  </tr>
					  
					  <tr>
						<td>Area type of the Postal Address</td>
						<td>'.$text_AreaType_postal.'</td>
					  </tr>
					  <tr>
						<td>'.$text_AreaType_postal .' of the Establishment</td>
						<td>'.$content['bmm_postal_block_mun_name'].'</td>
					  </tr>
					  <tr>
						<td>'.$text_AreaType_postal_name.' of the Postal Address</td>
						<td>'.$areatype_postal_name.'</td>
					  </tr>
					  <tr>
						<td>Police Station of the Postal Address</td>
						<td>'.$content['ps_postal_name_of_police_station'].'</td>
					  </tr>
					   <tr>
						<td>Pin Number of the Postal Address</td>
						<td>'.$content['e_postal_pin_number'].'</td>
					  </tr>
					  <tr>
						<td>Full Name of the Principal Employer</td>
						<td>'.$content['full_name_principal_emp'].'</td>
					  </tr>
					  <tr>
						 <td>Gender</td>
						 <td>'.$gender.'</td>
					  </tr>
					 ';
					  if($content['loc_emp_country']==2){
						  
			$output .= '<tr>
						<td>Country Of the Principal Employer </td>
						<td>Others</td>
					 </tr>
					 <tr>
						<td>Address Of the Principal Employer </td>
						<td>'.$content['address_principal_emp'].'</td>
					 </tr>'; 
				  }else{
					  
			$output .= '<tr>
						<td>Country Of the Principal Employer </td>
						<td>India</td>
					 </tr>
					 <tr>
						<td>State Of the Principal Employer </td>
						<td>'.$content['statename'].'</td>
					 </tr>
					 <tr>
						<td>Address Of the Principal Employer</td>
						<td>'.$content['address_principal_emp'].'</td>
					  </tr>';
					  
					   if($content['statename']=='West Bengal'){
						   
			$output .='<tr>
						<td>District of the Principal Employer</td>
						<td>'.$content['dm_emp_district_name'].'</td>
					  </tr>
					  <tr>
						<td>Subdivision of the Principal Employer</td>
						<td>'.$content['sd_emp_sub_div_name'].'</td>
					  </tr>
					  
					  <tr>
						<td>Area type of the Principal Employer</td>
						<td>'.$text_AreaType_emp.'</td>
					  </tr>
					  <tr>
						<td>'.$text_AreaType_emp.' of the Principal Employer</td>
						<td>'.$content['bmm_emp_block_mun_name'].'</td>
					  </tr>
					  <tr>
						<td>'.$text_AreaType_emp_name.' of the Principal Employer</td>
						<td>'.$areatype_emp_name.'</td>
					  </tr>
					  <tr>
						<td>Police Station of the Principal Employer</td>
						<td>'.$content['ps_emp_name_of_police_station'].'</td>
					  </tr>
					  <tr>
						<td>Pin Number of the Principal Employer</td>
						<td>'.$content['loc_emp_pin_number'].'</td>
					  </tr>';
					   }
				  }
					  
				
			$output .='<tr>
						<td>Full name of the Manager or Person Responsible for the Supervision  and control of the Establishment</td>
						<td>'.$content['full_name_manager'].'</td>
					  </tr>';
					  
					if($content['loc_manager_country']==2){
								  
		$output .=  '<tr>
						<td>Country Of the Manager or Person Responsible </td>
						<td>Others</td>
					 </tr>
					 <tr>
						<td>Address of the Manager or Person Responsible </td>
						<td>'.$content['address_manager'].'</td>
					 </tr>'; 
				 
				 }else{	
				 
		 $output.=' <tr>
						<td>Country Of the Manager or Person Responsible</td>
						<td>India</td>
					 </tr>
					 <tr>
						<td>State of the Manager or Person Responsible </td>
						<td>'.$content['sm_manager_statename'].'</td>
					 </tr>
					 <tr>
						<td>Address of the Manager or Person Responsible</td>
						<td>'.$content['address_manager'].'</td>
					  </tr>';
					  
			 		if($content['sm_manager_statename']=='West Bengal'){  
					
			$output	.='	<!-- Manager or Person Responsible Address part-->	  
					  <tr>
						<td>District of the Manager or Person Responsible</td>
						<td>'.$content['dm_manager_district_name'].'</td>
					  </tr>
					  <tr>
						<td>Subdivision of the Manager or Person Responsible</td>
						<td>'.$content['sd_manager_sub_div_name'].'</td>
					  </tr>
					  
					  <tr>
						<td>Area type of the Manager or Person Responsible</td>
						<td>'.$text_AreaType_manager.'</td>
					  </tr>
					  <tr>
						<td>'.$text_AreaType_manager.' of the Manager or Person Responsible</td>
						<td>'.$content['bmm_manager_block_mun_name'].'</td>
					  </tr>
					  <tr>
						<td>'.$text_AreaType_manager_name .' of the Manager or Person Responsible</td>
						<td>'.$areatype_manager_name.'</td>
					  </tr>
					  <tr>
						<td>Police Station of the Manager or Person Responsible</td>
						<td>'.$content['ps_manager_name_of_police_station'].'</td>
					  </tr>
					  <tr>
						<td>Pin Number of the Manager or Person Responsible</td>
						<td>'.$content['loc_manager_pin_number'].'</td>
					  </tr>';
					}
				 }
					  
		$output	.='<tr>
						<td>Nature of Work Carried on in the Establishment</td>
						<td>'.$nature_of_wrk.'</td>
					  </tr>
					  <tr>
						<td>Maximum Number of Workmen Employed Directly on any day in the Establishment</td>
						<td>'.$content['max_num_wrkmen'].'</td>
					  </tr>
					  <tr>
						<td>Number of Workmen Engaged as Permanent/Regular Workmen</td>
						<td>'.$content['e_num_of_workmen_per_or_reg'].'</td>
					  </tr>
					  <tr>
						<td>Number of Workmen Engaged as Temporary/Casual Workmen</td>
						<td>'.$content['e_num_of_workmen_temp_or_reg'].'</td>
					  </tr>
					  <tr>
						<td>Whether the Workmen employed/intended to be Employment by the Contractor Perform the same or similar kind of work as the Workmen employed directly by the Principal Employer</td>
						<td>'.$similar_kind_of_work.'</td>
					  </tr>
					  <tr>
						<td>A complete job description of the contrator labour</td>
						<td>'.$content['con_lab_job_desc'].'</td>
					  </tr>
					  <tr>
						<td>Wage rates and other cash benefits paid/to be paid</td>
						<td>'.$content['con_lab_wage_rate_other_benefits'].'</td>
					  </tr>
					  <tr>
						<td>Category/designation/nomenclature of the job</td>
						<td>'.$content['con_lab_cat_desig_nom'].'</td>
					  </tr>
					  <tr>
						<td>Settlement or award or judgement or minimum wages (if any applicable in the establishment)</td>
						<td>'.$content['e_settlement_award_judgement_min_wage'].'</td>
					  </tr>
					  <tr>
						<td>Maximum number of contract labour to be employed on any day through each contractor</td>
						<td>'.$content['e_any_day_max_num_of_workmen'].'</td>
					  </tr>
					  <tr>
						<td colspan="2" align="center" style="text-align:center;font-size:17px; padding-top: 8px;"><font color="#3366FF"><strong>Documents Uploaded</strong></font></td>
					  </tr>';
					  
					 if(!empty($content_docs)){ 
			$output .='<tr>
							<td>Upload Trade License</td>';
							
						   if(!empty($content_docs['trade_license_file'])){ 
							$output .=' <td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/trade_license/'.$content_docs['trade_license_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
						   } else { 
							$output .=' <td>No Document Uploaded</td>';
						  } 
			$output .='</tr>
			
						<tr>
							<td>Upload Article of Association</td>';
							 if(!empty($content_docs['article_of_assoc_file'])){ 
					
						 		$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/article_of_assoc/'.$content_docs['article_of_assoc_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
					 		} else { 
								$output .='<td>No Document Uploaded</td>';
						 	} 
				$output .='</tr>
							<tr>
								<td>Upload Memorandum of Certificate</td>';
								if(!empty($content_docs['memorandum_of_cert_file'])){ 
					
								$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/memorandum_of_cert/'.$content_docs['memorandum_of_cert_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
								 } else { 
								$output .='<td>No Document Uploaded</td>';
								 } 
				$output .='</tr>
      
      
    
						  <tr>
							<td>Upload Partnership Deed</td>';
         					
							 if(!empty($content_docs['partnership_deed_file'])){ 
							 
        						$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/partnership_deed/'. $content_docs['partnership_deed_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
       						 } else { 
        						$output .='<td>No Document Uploaded</td>';
       						 } 
      			$output .='</tr>
				
							<tr>
								<td>Upload Factory License</td>';
								
								 if(!empty($content_docs['factory_license_file'])){ 
								 
									$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/FactoryLicense/'. $content_docs['factory_license_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
								 } else { 
									$output .='<td>No Document Uploaded</td>';
								 } 
					$output .='</tr>
							   <tr>
							   		<td>Form -I </td>';
									if(!empty($content_docs['form_1_clra_signed_pdf_file'])){
										
										$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/Form-I/'. $content_docs['form_1_clra_signed_pdf_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
								 	} else { 
										$output .='<td>No Document Uploaded</td>';
								 	} 
					$output .='</tr>';
				
    				 }
					 
			    $paid_fees = get_clra_pe_total_paid_amount($user_id, $application_id);
					  
				$output .='<tr>
							<td>Fees Details <br>[<i style="color:#f00;"> ** Fees calculation depend on "Maximum number of contract labour to be employed on any day through each contractor".</i>]</td>
							<td>Total Fees :- <font color="#FF6600"> &#8377;'.number_format($retFees, 2).'</font> 
									<input type="hidden" name="retFees" id="retFees" value="'.$retFees.'" />&nbsp;&nbsp;&nbsp;<span class="view_fees_details">CLICK HERE TO VIEW FEES DETAILS</span><br/>
								Previous deposited fees :- <font color="#FF6600">&#8377;'.$paid_fees.'</font><br>
								Payable fees :-<font color="#FF6600"> &#8377;'.($retFees - $paid_fees).'</font>
								<div id="fees_details" title="FEES CHART" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/fees_chat/CLRA-FEES-STRUCTURE.pdf" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
								</div>
							</td>    
						  </tr>
					</table>
					
					<div class="contractor_details" style="padding:12px 0; text-align:center; font-size:15px;"><strong>Contractors Details</strong></div>
						<div style="overflow-y:scroll;max-height:392px;">
							<table style="border:1px solid #000;width:100%;">';
								 if(!empty($content_3)){
										$i = 0;	
										foreach($content_3 as $delta_clra_cont => $datafetch_clra_cont){ 
											$i++;
											$cont_work_name = "";
											$prefix = '';
											
											
											$contractor_wrk_name 		= db_select('l_clra_con_nature_of_work', 'lccnw');
											$contractor_wrk_name		->leftJoin('contractor_works', 'cw', 'cw.contractor_work_id=lccnw.contractor_nature_of_work');
											$contractor_wrk_name		->fields('lccnw', array('contractor_nature_of_work'));
											$contractor_wrk_name		->fields('cw', array('cont_work_name'));
											$contractor_wrk_name		->condition('lccnw.user_id', trim($user->uid));
											$contractor_wrk_name		->condition('lccnw.contractor_id', $datafetch_clra_cont->id);
											$contractor_wrk_name		->condition('lccnw.application_id', $application_id);
											$result_2 					= $contractor_wrk_name->execute()->fetchAll(); 
											
											foreach ($result_2 as  $con_nature_of_wrk){
												
												if($con_nature_of_wrk->contractor_nature_of_work == 28){
													$cont_work_name  	.= 'Others';
												}else{
													$cont_work_name  	.= $prefix.$con_nature_of_wrk->cont_work_name;
													$prefix  	 = ', ';
												}
												
											}
											
											
											if ($datafetch_clra_cont->state_opts == 1){
												$table	 				= 'l_contractor_info_master';
												$type_con	 			= 'sub-table';
												$con_Arr	 			= array('con_loc_e_dist','con_loc_e_subdivision','con_loc_e_areatype','con_name_areatype','con_loc_e_vill_ward','con_l_e_ps','contractor_pin','state_opts');
												$contractor_address 	= '<br/>'.get_full_address($table, $type_con, $datafetch_clra_cont->id, $con_Arr);
											}else{
												
												$contractor_address		='<br/>'.$datafetch_clra_cont->state_others;
											}
											
											
			  
								  $status = '';
								  
								  /** For new Amended Contractors **FOR AMENDMENT**/
		
								if(!empty($datafetch_clra_cont->amendment_id ) &&  $last_amendment_id == $datafetch_clra_cont->amendment_id){
									$newly_add_contractor = '<font color="red"><i>New Amended Contractor</i></font>';
								}else{
									$newly_add_contractor ='';
								}
								  
								  if($datafetch_clra_cont->status == 0) $status = '<strong><font color="#f00">[CANCELLED]</font></strong>';
								  
								  if($datafetch_clra_cont->contractor_type == 1) { $contractor_type = 'Offline License' ; }else { $contractor_type = 'New Contractor'; }
								  
								  $output .='<tr>
												<td colspan="2" align="center" style="text-align:center;font-size:17px; padding-top: 8px;">
													<font color="#3366FF"><strong>CLRA Particulars of Contractors and Contract Labour('.$i.')&nbsp;'.$status.'</strong><br/>'.$newly_add_contractor.'</font>				
												</td>
											</tr>
											<tr>
												<th width="51%" style="text-align:center;font-size:17px; padding-top: 8px;">Parameters</th>
												<th width="49%" style="text-align:center;font-size:17px; padding-top: 8px;">Inputs</th>
											</tr>
											<tr>
												<td>Contractor Type</td>
												<td>'.$contractor_type.'</td>
											 </tr>
											 <tr>
												<td>Name of the Contractor</td>
												<td>'.$datafetch_clra_cont->name_of_contractor.'</td>
											  </tr>
											  <tr>
												<td>Email of the Contractor</td>
												<td>'.$datafetch_clra_cont->email_of_contractor.'</td>
											  </tr>
											  <tr>
												<td>Address of the Contractor</td>
												<td>'.$datafetch_clra_cont->address_of_contractor.$contractor_address.'</td>
											  </tr>
											 
											  <tr>
												<td>Nature of Work in which Contract Labour is Employed or is to be Employed</td>
												<td>'.$cont_work_name.'</td>
											  </tr>
											  <tr>
												<td>Maximum Number of Contractor Labour to be Employed on any day Through Each Contractor</td>
												<td>'.$datafetch_clra_cont->contractor_max_no_of_labours_on_any_day.'</td>
											  </tr>
											  <tr>
												<td>Estimated Date of Employment of Each Contract Work Under Each Contractor</td>
												<td>'.date('dS M, Y',strtotime($datafetch_clra_cont->est_date_of_work_of_each_labour_from_date)).' to '. date('dS M, Y',strtotime($datafetch_clra_cont->est_date_of_work_of_each_labour_to_date)).'</td>
											  </tr>';
				
				
								 		} 
										
								} else { 
        						
									$output .='<tr><td style="text-align:center"> No Contractors Added</td></tr>';
            					 } 
       						 $output .='</table>
    				</div>
    
    
					<div class="contractor_details" style="padding:12px 0; text-align:center; font-size:15px;"><strong>Trade Union Details</strong></div>
					<div style="overflow-y:scroll;max-height:392px;">
						<table style="border:1px solid #000;width:100%;">';
							 if(!empty($trade_union_result)){
									$x = 0;	
									foreach($trade_union_result as $trade_union_data){ 
										$x++;
										
							  
						 $output .='<tr>
										<td colspan="2" align="center" style="text-align:center;font-size:17px; padding-top: 8px;">
											<font color="#3366FF"><strong>Trade Union('.$x.')</strong></font>				
										</td>
									</tr>
									<tr>
										<th width="51%" style="text-align:center;font-size:17px; padding-top: 8px;">Parameters</th>
										<th width="49%" style="text-align:center;font-size:17px; padding-top: 8px;">Inputs</th>
									</tr>
									 <tr>
										<td>Registration of the Trade Union</td>
										<td>'.$trade_union_data->e_trade_union_regn_no.'</td>
									  </tr>
									  <tr>
										<td>Name of the Trade Union</td>
										<td>'.$trade_union_data->e_trade_union_name.'</td>
									  </tr>
									  <tr>
										<td>Address of the Trade Union</td>
										<td>'.$trade_union_data->e_trade_union_address.'</td>
									  </tr>';
							 } }else { 
							$output .='<tr><td style="text-align:center"> No Trade Union Added</td></tr>';
							 }  
			$output .='</table>
					</div>';
			
  
					  
					  return $output;
	
}


function clra_amendment_application_preview_submit($form, &$form_state){
	
		global $base_root, $base_path, $user; 
		
		$user_id				= $user->uid;
		$backlog_field_ids 		= '';
		
		$val					= $form_state['values'];  
		$application_id			= $val['application_id'];
		$backlog_id				= $val['backlog_id'];
		$retFees				= $_POST['retFees'];
		
		
		if($val['update_status'] == 'Y'){
			//update contract labour number
			
			$cln_sql_query = db_update('l_clra_registration_master');
			$cln_sql_query->fields(array(				 		
				 		'e_any_day_max_num_of_workmen' 	=> $val['e_any_day_max_num_of_workmen'],
				 
			));
			$cln_sql_query->condition('id',$application_id);
			$cln_sql_query->condition('user_id',$user_id);
			$cln_sql_query->execute();
			
			drupal_goto('clra-reg-amendment/verify/'.encryption_decryption_fun('encrypt', $application_id));
		}else{	
		
		
		$fetch_reg_det 			= db_select('l_clra_registration_master', 'lcrm');
		$fetch_reg_det->fields('lcrm', array('application_date', 'created','full_name_principal_emp'));
		$fetch_reg_det->condition('lcrm.user_id', $user_id);
		$fetch_reg_det->condition('lcrm.id', $application_id); 
		$fetch_reg_det_result 	= $fetch_reg_det->execute(); 
		
			
		if( $fetch_reg_det_result->rowCount() > 0 ){			
			$obj2 				= $fetch_reg_det_result->fetchAssoc(); 
			$application_date	= !empty($obj2['application_date']) ? $obj2['application_date'] :'';
			$created_date		= !empty($obj2['created'])? $obj2['application_date'] :'';
			$principal_emp_name	= $obj2['full_name_principal_emp'];				
		}
		
		$fetch_users_role = db_select('users_roles', 'ur');
		$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
		$fetch_users_role->fields('ro', array('rid'));
		$fetch_users_role->condition('ur.uid', $user_id);
		$fetch_users_role_data = $fetch_users_role->execute()->fetchAssoc();	
		$user_role = $fetch_users_role_data['rid'];		
		$fieldsRemarksData = array( 
									'remarks'				=>	'Applied for Amendment Successfully',
									'remark_by'				=>  $user_id,
									'remark_to'				=>	$user_id,
									'application_id'        =>  $application_id,
									'remark_type' 			=>  '0',
									'remark_field_text'		=>  $backlog_field_ids,
									'remark_date' 			=>  time(),
									'remark_by_name'		=> 	$principal_emp_name,
									'remark_by_role'		=> 	$user_role
								);
				
	db_insert('l_remarks_master')->fields($fieldsRemarksData)->execute();
	
	
	if(empty($application_date) && empty($created_date)){
	
		$sql_query = db_update('l_clra_registration_master');
		$sql_query->fields(array(
				 'status'  				=> '0',
				 'application_date' 	=> date("Y-m-d"),
				 'final_submit_status' 	=> 'P',
				 'finalfees' 			=> $retFees,
				 'created'				=> time()
		));
		$sql_query->condition('id',$application_id);
		$sql_query->condition('user_id',$user_id);
		$sql_query->execute();
		
		
	}else{
		
		$sql_query = db_update('l_clra_registration_master');
		$sql_query->fields(array(
				 'status'  				=> '0',
				 'application_date' 	=> date("Y-m-d"),
				 'final_submit_status' 	=> 'P',
				 
		));
		$sql_query->condition('id',$application_id);
		$sql_query->condition('user_id',$user_id);
		$sql_query->execute();
	}
		
	if(!empty($user_id)){
		$phone = get_common_function_for_phone();
		if(!empty($phone)){
			$msg	=	"Your application has been successfully submitted.You can track your application status from dashboard.";
			// module_load_include('inc', 'common_encryption_form', 'include/test_cryptography_form');
			send_sms_for_user_alert($phone, $msg);
			send_sms_for_user_alert('7603091500', $msg);
		}else{
			$msg	=	"Message not sent.Please try again later.";
			drupal_set_message(t($msg));
		}
	}	
		
		
	$message = 'Application is applied successfully. Inspection will be started shortly.';
	drupal_set_message(t($message));
	drupal_goto( 'clra-application/view/'.encryption_decryption_fun('encrypt', $application_id) );
	}	
}
