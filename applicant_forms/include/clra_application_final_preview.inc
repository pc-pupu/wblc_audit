<?php
function clra_application_preview($form, &$form_state, $id=''){
	
	drupal_add_js(drupal_get_path('module', 'applicant_forms').'/js/custom.js');	
	
	global $base_root, $base_path, $user; 
		
	$user_id						= $user->uid;
	$action							= 'decrypt';
	$action_encrypt					= 'encrypt';
	$application_id					= encryption_decryption_fun($action, $id);	
	
	$master_part_clra_reg 			=    db_select('l_clra_registration_master', 'lcrm');
	$master_part_clra_reg			->   fields('lcrm', array('id', 'final_submit_status', 'status', 'backlog_id', 'e_any_day_max_num_of_workmen', 'loc_e_subdivision', 'name_areatype'));
	$master_part_clra_reg			->  condition('lcrm.user_id', $user_id);
	$master_part_clra_reg			->  condition('lcrm.id', $application_id); 
	$master_part_clra_reg_result 	=   $master_part_clra_reg->execute(); 
	$applicant_details				=	$master_part_clra_reg_result->fetchAssoc();
	
	$final_submit_status			=	trim($applicant_details['final_submit_status']);
	$status							=	trim($applicant_details['status']);  
	$backlog_id						=	trim($applicant_details['backlog_id']);
	
	
	/* Contract labour checking start */
	
	$form['clra_app_final_preview']['markup_data']							 = array(
																				  '#type' 				=> 'markup',
																				  '#markup'		 		=> '<div class="content">',
																				);
	
	if(empty($applicant_details['loc_e_subdivision']) || empty($applicant_details['name_areatype'])){
		
		$form['clra_app_final_preview']['contact_labour_check_msg']			= array(
																				'#type'					=> 'markup',
																				'#markup' 				=>  '<span style="color:#ff0000; font-weight:700;">Click '.l('here', 'view-clra-application-details/view-clra-application/'.$id).' to filled up and save application details form.</span>',																	
																			);
	}else{	
	
		$total_cl_query 						= db_query("select max(contractor_max_no_of_labours_on_any_day) as totalcl from l_contractor_info_master where application_id='".$applicant_details['id']."' and status = 1 ");
		$total_cl_result 						= $total_cl_query->fetchObject();	
	
		if($applicant_details['e_any_day_max_num_of_workmen'] < $total_cl_result->totalcl){
		
			$form['clra_app_final_preview']['contact_labour_check_msg']		= array(
																				'#type'					=> 'markup',
																				'#markup' 				=> '<div class="panel panel-default panel-form">
																												<div class="panel-body">
																													<div class="application-pre-note">
																														The total value added for every contractors in field No. <u>5. Maximum number of contract labour to be employed on any day through each contractor</u> in <strong>Contractor Information</strong> Tab has exceeded from the value for field No. <u>6.) Maximum number of contract labour to be employed on any day through each contractor</u> in <strong>Application Details</strong> Tab. If want to continue , click continue button.</div>',																	
																					);
																	
																	
			$form['clra_app_final_preview']['application_id']				= array(																		
																				'#type'			 		=> 'hidden',
																				'#default_value' 		=> !empty($application_id) ? encryption_decryption_fun('encrypt', $application_id) : '',
																				'#attributes' 	 		=> array('readonly' => 'readonly'),
																				);
			$form['clra_app_final_preview']['e_any_day_max_num_of_workmen']	= array(																		
																				'#type'			 		=> 'hidden',
																				'#default_value' 		=> $total_cl_result->totalcl,
																				'#attributes' 	 		=> array('readonly' => 'readonly'),
																				);
			$form['clra_app_final_preview']['update_status']				= array(																		
																				'#type'			 		=> 'hidden',
																				'#default_value' 		=> 'Y',
																				'#attributes' 	 		=> array('readonly' => 'readonly'),
																				);
			
			$form['clra_app_final_preview']['submit']						= array(
																				'#type' 				=> 'submit',																		
																				'#value' 				=> 'CONTINUE',
																				'#attributes' 			=> array('class' => array('btn btn-primary pull-right form-submit')),
																				'#suffix' 				=> '</div></div>', 
																				);
																				
		}else{
			
			$form['clra_app_final_preview']['application_id']				= array(
																				'#title'				=> 'Application Id',
																				'#type'					=> 'hidden',
																				'#default_value' 		=> !empty($application_id) ?  encryption_decryption_fun('encrypt', $application_id) : '',
																				'#attributes' 			=> array('readonly' => 'readonly'),
																				);
																											
																											
			$form['clra_app_final_preview']['backlog_id']					= array(
																				'#title'				=> 'Backlog Id',
																				'#type'					=> 'hidden',
																				'#default_value' 		=> !empty($backlog_id) ?  encryption_decryption_fun('encrypt', $backlog_id) : '',
																				'#attributes' 			=> array('readonly' => 'readonly'),
																				);
																											
			$form['clra_app_final_preview']['markup_data_1']				= array(
																				'#type' 				=> 'markup',
																				'#markup' 				=> view_clra_app_final_preview($application_id),
																				);
																									
			if($status == '' || $status == 'B'){ 
			
			
						
				$form['clra_app_final_preview']['finalAgree'] 				= array(																		 
																				  '#type' 				=> 'checkbox',
																				  '#description' 		=> '<span class="red-star">*</span>I hereby declare that the particulars given above are true the best of my knowledge and belief.',
																				  '#attributes'			=> array('id' => 'finalAgreeId','onchange' => 'return checkFinalAppCheckBox()','class' => array('pull-left')),
																				  '#required' 			=> TRUE,
																				  
																				);
		
				$form['clra_app_final_preview']['submit']					= array(
																				 '#type' 				=> 'submit',
																				 '#id'					=> 'submitBtn1',
																				 '#value' 				=> 'SUBMIT',
																				 '#attributes' 			=> array('class' => array('btn btn-primary pull-right form-submit')),
																				 '#suffix'		 		=> '</div>',
																				 
																				);
																									
																									
			} else {
	
				$form['clra_app_final_preview']['download_pdf']				= array(
																				'#type'					=> 'markup',
																				'#markup' 				=>  l(' VIEW&nbsp;&amp;&nbsp;PRINT','generate-pdf-applicant/'.encryption_decryption_fun($action_encrypt, $application_id), array('html' => TRUE, 'attributes' => array('target' => '_blank', 'class' => array('btn btn-primary pull-right form-submit')))),
																				 '#suffix'		 		=> '</div>',
																				
																				
																				);
			}
	
		}
	}
	
	
	return $form;						
	
}


function view_clra_app_final_preview($application_id){
	
		global $base_root, $base_path, $user;
		
		drupal_add_js(drupal_get_path('module', 'alc') . '/js/mycustom.js');	
		
		$user_id 			 = $user->uid;
				
		$master_part_clra_reg = db_select('l_clra_registration_master', 'lcrm');
		$master_part_clra_reg->leftJoin('district_master', 'dm_loc', 'dm_loc.district_code=lcrm.loc_e_dist');
		$master_part_clra_reg->leftJoin('sub_division', 'sd_loc', 'sd_loc.sub_div_code=lcrm.loc_e_subdivision');
		$master_part_clra_reg->leftJoin('l_clra_principle_emp_backlog_data', 'lcpbd', 'lcpbd.id=lcrm.backlog_id');
		$master_part_clra_reg->fields('lcpbd', array('fees'));
		$master_part_clra_reg->fields('lcrm', array());
		$master_part_clra_reg->fields('dm_loc', array('district_name'));
		$master_part_clra_reg->fields('sd_loc', array('sub_div_name'));
		$master_part_clra_reg->condition('lcrm.user_id', $user_id);
		$master_part_clra_reg->condition('lcrm.id', $application_id);

		$master_part_clra_reg_result = $master_part_clra_reg->execute();
		
		$content					 = $master_part_clra_reg_result->fetchAssoc();
		
		if(!empty($content) && $application_id!=''){
			
			if($content['workmen_if_same_similar_kind_of_work']=="0"){
				$similar_kind_of_work = "No";
			}else{
				$similar_kind_of_work = "Yes";
			}
			
		/** Fetch Address from Miscellaneous Module ****/

			$table	 	 = 'l_clra_registration_master';
			$type	 	 = 'sub-table';
			$estArr	 	 = array('loc_e_dist','loc_e_subdivision','loc_e_areatype','name_areatype','loc_e_vill_ward','l_e_ps','loc_e_pin_number');
			$postalArr	 = array('e_postal_dist','e_postal_subdivision','e_postal_areatype','e_postal_name_areatype','e_postal_vill_ward','e_postal_ps','e_postal_pin_number');
			$empArr	 	 = array('loc_emp_dist','loc_emp_subdivision','loc_emp_areatype','emp_name_areatype','loc_emp_vill_ward','l_emp_ps','loc_emp_pin_number','loc_emp_state');
			$managerArr	 = array('loc_manager_dist','loc_manager_subdivision','loc_manager_areatype','manager_name_areatype','loc_manager_vill_ward','l_manager_ps','loc_manager_pin_number','loc_manager_state');
			
			
			$establishment_address 	= $content['loc_e_name'].'<br/>'.get_full_address($table, $type, $application_id, $estArr);
			$postal_address 	 	= $content['e_postal_address'].'<br/>'.get_full_address($table, $type, $application_id, $postalArr);
			$pe_address	 			= $content['address_principal_emp'].'<br/>'.get_full_address($table, $type, $application_id, $empArr);
			$manager_address	 	= $content['address_manager'].'<br/>'.get_full_address($table, $type, $application_id, $managerArr);
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
		$master_part_clra_cont->condition('lcim.user_id', trim($user_id));
		$master_part_clra_cont->condition('lcim.application_id', trim($application_id));
		//$master_part_clra_cont->condition('lcim.status', 1);
		$master_part_clra_cont->orderBy('lcim.id','DESC');
		
		$master_part_clra_cont_result = $master_part_clra_cont->execute();
		
		$content_3	=	array();
		foreach ($master_part_clra_cont_result as $master_part_clra_cont_result_row) {
			$content_3[]	=	$master_part_clra_cont_result_row;
		}
		if($content['gender_pe']=='M'){
			$gender = 'Male';	
		}elseif($content['gender_pe']=='F'){
			$gender = 'Female';	
		}elseif($content['gender_pe']=='O'){
			$gender = 'Other';
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
		
		$feespaid_fsubmit = '';
		
		if($content['status'] == 'T'){
			$feespaid_fsubmit = '<p><font color="#ff0000"><strong>Note : Your transaction has been successfully made but process not complete. Click '.l('here', 'upload_signed_application_form/'.encryption_decryption_fun('encrypt', $content['id'])).' to download system generated [ FORM-I ] . You must upload signed [ FORM-I ] and upload it for further process.</strong></font></p>';
		}
		$output ='';
		
		$output = '<div class="panel panel-default panel-form">
					  <div class="panel-heading">Establishment Details</div>
					  	<div class="panel-body">
	  						<div class="help">
								<strong>Authorized Registering office for this application : </strong> Regional Labour Office - '.$content['sub_div_name'].', '.$content['district_name'].' &nbsp;'.l('More info.', 'rlo-details/'.str_replace(' ', '-', strtolower($content['sub_div_name'])).'/'.encryption_decryption_fun('encrypt', $content['loc_e_subdivision'])).'</p>'.$feespaid_fsubmit.'
							</div>
							
							<div class="row">
							<div class="col-lg-6 col-md-12">
							<table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-striped table-responsive dashboard-table sub-table-dashboard">
							<thead>		  
							  <tr>
								<th width="45%">Parameters</th>
								<th>Inputs</th>
							  </tr>
							  </thead>
							  <tbody>
							  	 <tr>
									<td>Name of the Establishment </td>
									<td><strong>'.$content['e_name'].'</strong></td>
								  </tr>
								   <tr>
									<td>Establishment type </td>
									<td><strong>'.ucfirst($content['est_type']).'</strong></td>
								  </tr>
								  <tr>
									<td>Location of the Establishment</td>
									<td><strong>'.$establishment_address.'</strong></td>
								  </tr>
								  <tr>
									 <td>Postal Address of the Establishment</td>
									<td><strong>'.$postal_address.'</strong></td>
								 </tr>
								 <tr>
								 <td>Nature of Work Carried on in the Establishment</td>
								 <td><strong>'.$nature_of_wrk.'</strong></td>
								 </tr>
								  <tr>
									<td>Maximum Number of Workmen Employed Directly on any day in the Establishment</td>
									<td><strong>'.$content['max_num_wrkmen'].'</strong></td>
								  </tr>
								  <tr>
								 <td>Number of Workmen Engaged as Permanent / Regular Workmen</td>
								 <td><strong>'.$content['e_num_of_workmen_per_or_reg'].'</strong></td>
								 </tr>
								  <tr>
									<td>Number of Workmen Engaged as Temporary / Regular Workmen</td>
									<td><strong>'.$content['e_num_of_workmen_temp_or_reg'].'</strong></td>
								  </tr>
								  <tr>
									<td>A complete job description of the contract labour</td>
									<td><strong>'.$content['con_lab_job_desc'].'</strong></td>
								  </tr>
							  </tbody>
							</table>
							</div>
							<div class="col-lg-6 col-md-12">
							<table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-striped table-responsive dashboard-table sub-table-dashboard">
							<thead>		  
							  <tr>
							  <th width="45%">Parameters</th>
								<th>Inputs</th>
							 </tr>
							  </thead> 
							  <tbody>
							  	 <tr>
								 <td>Full Name of the Principal Employer</td>
								 <td><strong>'.$content['full_name_principal_emp'].'</strong></td>
								 </tr>
								 <tr>
								 <td>Gender</td>
								 <td><strong>'.$gender.'</strong></td>
								 </tr>
								 <tr>
								 <td>Mobile No.</td>
								 <td><strong>'.$content['mobile_principal_emp'].'</strong></td>
								 </tr>
								 <tr>
								 <td>Address of the Principal Employer</td>
									<td><strong>'.$pe_address.'</strong></td>
								 </tr>
								 <tr>
								 <td>Full name of the Manager or Person Responsible for the Supervision  and control of the Establishment</td>
								 <td><strong>'.$content['full_name_manager'].'</strong></td>
								 </tr>
								 <tr>
									<td>Address of the Manager or Person Responsible for the Supervision  and control of the Establishment</td>
									<td><strong>'.$manager_address.'</strong></td>
								  </tr>
								 
								 <tr>
								 <td>Whether the Workmen employed / intended to be Employment by the Contractor Perform the same or similar kind of work as the Workmen employed directly by the Principal Employer</td>
								 <td><strong>'.$similar_kind_of_work.'</strong></td>
								 </tr>
								 <tr>
								 <td>Wage rates and other cash benefits paid/to be paid</td>
								 <td><strong>'.$content['con_lab_wage_rate_other_benefits'].'</strong></td>
								 </tr>
								 <tr>
								 <td>Settlement or award or judgement or minimum wages (if any applicable in the establishment)</td>
								 <td><strong>'.$content['e_settlement_award_judgement_min_wage'].'</strong></td>
								 </tr>
								 <tr>
									<td>Maximum number of contract labour to be employed on any day through each contractor</td>
									<td><strong>'.$content['e_any_day_max_num_of_workmen'].'<font color="#f00">**</font></strong></td>
								  </tr>
								  <tr>
									<td>Category / designation / nomenclature of the job</td>
									<td><strong>'.$content['con_lab_cat_desig_nom'].'</strong></td>
								  </tr>
							  </tbody>
							</table>
							</div>
							</div>
					  		<table class="table table-striped table-responsive dashboard-table sub-table-dashboard" width="100%" cellspacing="0" cellpadding="0" border="0">
					  			<thead>
								  <tr><td colspan="2" align="center"><strong>Documents Uploaded</strong></td></tr>
								</thead>
								<tbody>';
									if(!empty($content_docs)){ 
										$output .='<tr>
													<td width="45%">Trade License</td>';
														if(!empty($content_docs['trade_license_file'])){ 
															$output .=' <td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/trade_license/'.$content_docs['trade_license_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/lc-pdf.png" title="Click here to view details"></a></td>';
														} else { 
															$output .=' <td><strong>No Document Uploaded</strong></td>';
														}
										$output .='</tr>
												   <tr>
													 <td>Article of Association and Memorandum of Association / Partnership Deed</td>';
														  if(!empty($content_docs['article_of_assoc_file'])){ 
															$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/article_of_assoc/'.$content_docs['article_of_assoc_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/lc-pdf.png" title="Click here to view details"></a></td>';
														  } else { 
															$output .='<td><strong>No Document Uploaded</strong></td>';
														  }
										$output .='</tr>
												   <tr>
													 <td>Any other document in support of correctness of the particulars mentioned in the application if required</td>';
														 if(!empty($content_docs['memorandum_of_cert_file'])){ 
															$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/memorandum_of_cert/'.$content_docs['memorandum_of_cert_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/lc-pdf.png" title="Click here to view details"></a></td>';
														 } else { 
															$output .='<td><strong>No Document Uploaded</strong></td>';
														 } 
										$output .='</tr>
												   <tr>
													<td>Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc</td>';
														if(!empty($content_docs['partnership_deed_file'])){ 
															$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/partnership_deed/'. $content_docs['partnership_deed_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/lc-pdf.png" title="Click here to view details"></a></td>';
														} else { 
															$output .='<td><strong>No Document Uploaded</strong></td>';
														} 
										$output .='</tr>
												   <tr>
													 <td>Factory License if any</td>';
														 if(!empty($content_docs['factory_license_file'])){ 
															$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/FactoryLicense/'. $content_docs['factory_license_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/lc-pdf.png" title="Click here to view details"></a></td>';
														 } else { 
															$output .='<td><strong>No Document Uploaded</strong></td>';
														 } 
										$output .='</tr>
												   <tr>
													 <td>Form -I </td>';
														if(!empty($content_docs['form_1_clra_signed_pdf_file'])){
															$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/FORM-I/'. $content_docs['form_1_clra_signed_pdf_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/lc-pdf.png" title="Click here to view details"></a></td>';
														} else { 
															$output .='<td><strong>No Document Uploaded</strong></td>';
														} 
										$output .='</tr>';
										
										if ($content['backlog_id']!=''){								
										
										$output   .='<tr>
														<td>Previous Registration Certificate </td>';
															if(!empty($content_docs['backlog_clra_registration_certificate_file'])){
																$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/backlog_clra_registration_certificate/'. $content_docs['backlog_clra_registration_certificate_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/lc-pdf.png" title="Click here to view details"></a></td>';
															} else { 
																$output .='<td><strong>No Document Uploaded</strong></td>';
															} 
											$output .='</tr>';
										}
										
									}else{
										$output .='<tr><td colspan="2" align="center"><strong> No Documents Uploaded</strong></td></tr>';
									}
					$output  .='</tbody>
							</table>
							
							<table class="table table-striped table-responsive dashboard-table sub-table-dashboard" width="100%" cellspacing="0" cellpadding="0" border="0">
								<thead>
								  <tr><td colspan="2" align="center"><strong>Fees Details</strong></td></tr>
								</thead>
								<tbody>';
									if(empty($content['amendment_parent_id'])){
										if($content['fees']!=''){
											$paid_fees = $content['fees'];
										}else{
											$paid_fees = 'Not Available';
										}
									}else{
										$p_amount = db_select('l_clra_registration_master', 'clm');
										$p_amount->fields('clm', array('finalfees'));
										$p_amount->condition('clm.id', $content['amendment_parent_id']);
																 
										$p_amount = $p_amount->execute();			 
										$p_amount_r	= $p_amount->fetchAssoc();	
										
										if($p_amount_r['finalfees']!=''){
											$paid_fees = $p_amount_r['finalfees'];
										}else{
											$paid_fees = 'Not Available';
										}
									}
									
									$paid_fees = get_clra_pe_total_paid_amount($user->uid, $application_id);
									
						$output  .='<tr>
										<td width="45%">Fees Details<br>[<i style="color:#f00;"> ** Fees calculation depend on "Maximum number of contract labour to be employed on any day through each contractor".</i>]</td>
										<td>Total Fees :-<font color="#FF6600"> &#8377;'.number_format($retFees, 2).'</font>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
										<a href="'.$GLOBALS['base_url'].'/sites/default/files/upload/fees_chat/CLRA-FEES-STRUCTURE.pdf" target="_blank"><span class="viewfile_popup blink" id="fees_chart_pdf" title="FEES CHART">FEES CHART [ In <i class="fa fa-inr" aria-hidden="true"></i> ]</span></a><br/>									
										
											Previous deposited fees :- <font color="#FF6600">&#8377;'.$paid_fees.'</font><br>
											Payable fees :-<font color="#FF6600"> &#8377;'.($retFees - $paid_fees).'</font>
											<input type="hidden" name="retFees" id="retFees" value="'.$retFees.'" />
											
											<div id="view_fees_chart_pdf" style="display:none;" title="Fees Chart">
<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/fees_chat/CLRA-FEES-STRUCTURE.pdf" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
</div>
											
										</td> 
						  			</tr>
								</tbody>
							</table>												
						</div>
				   </div>				  
				  <br class="res-br-tag" />
				  <br class="res-br-tag" />				  
				  <div class="panel panel-default panel-form">
      				<div class="panel-heading">Contractors and Contract Labour Details</div>
	  					<div class="panel-body" style="overflow-y:scroll;overflow:auto;max-height:392px;">
							<table class="table table-striped table-responsive dashboard-table sub-table-dashboard" width="100%" cellspacing="0" cellpadding="0" border="0">';
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
													if(!empty($datafetch_clra_cont->other_nature_work)){
														$cont_work_name  .= $datafetch_clra_cont->other_nature_work;
													}else{
														$cont_work_name  .= 'Others';
													}
												}else{
													$cont_work_name  .= $prefix.$con_nature_of_wrk->cont_work_name;
													$prefix  	 	  = ', ';
												}
												
											}

											
											if ($datafetch_clra_cont->state_opts == 1){
												$table	 				= 'l_contractor_info_master';
												$type_con	 			= 'sub-table';
												$con_Arr	 			= array('con_loc_e_dist','con_loc_e_subdivision','con_loc_e_areatype','con_name_areatype','con_loc_e_vill_ward','con_l_e_ps','contractor_pin','state_opts');
												$contractor_address 	= $datafetch_clra_cont->address_of_contractor.'<br/>'.get_full_address($table, $type_con, $datafetch_clra_cont->id, $con_Arr);
											}else{
												
												$contractor_address		='<br/>'.$datafetch_clra_cont->address_of_contractor;
											}
						
			  
								  $status = '';
								  
											/** For new Amended Contractors **FOR AMENDMENT**/
					
											if(!empty($datafetch_clra_cont->amendment_id ) &&  $last_amendment_id == $datafetch_clra_cont->amendment_id){
												$newly_add_contractor = '<font color="red"><i>New Amended Contractor</i></font>';
											}else{
												$newly_add_contractor ='';
											}
								  
								  
										  if($datafetch_clra_cont->status == 0) $status = '<strong>[ <font color="#fba35d">CANCELLED</font> ]</strong>';
										  
										  if($datafetch_clra_cont->contractor_type == 1) { $contractor_type = 'Offline License' ; }else { $contractor_type = 'New Contractor'; }
								  
						$output .='<thead>
								  		<tr><td colspan="2" align="center"><strong>Contractor-'.$i.'&nbsp;'.$status.'</strong><br/>'.$newly_add_contractor.'</td></tr>
								  </thead>
								  </tbody>
										<tr>
											<td width="45%">Contractor Type</td>
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
											<td>'.$contractor_address.'</td>
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
					   $output .='</tbody>
							</table>
						</div>
				  </div>
				  <br class="res-br-tag" />
				  <br class="res-br-tag" />
				  <div class="panel panel-default panel-form">
      				<div class="panel-heading">Trade Union Details</div>
	  					<div class="panel-body" style="overflow-y:scroll;overflow:auto;max-height:392px;">
							<table class="table table-striped table-responsive dashboard-table sub-table-dashboard" width="100%" cellspacing="0" cellpadding="0" border="0">';
								if(!empty($trade_union_result)){
									
									$x = 0;	
									
									foreach($trade_union_result as $trade_union_data){ 
										
										$x++;
							
							$output .='<thead>
								  		<tr><td colspan="2" align="center"><strong>Trade Union - '.$x.'</td></tr>
								  	   </thead>
									   <tbody>
										 <tr>
											<td width="45%">Registration of the Trade Union</td>
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
									}
									
								}else{
									
									$output .='<tr><td style="text-align:center"> <strong>No Trade Union Added</strong></td></tr>';
								}
							$output .='</tbody>
							</table>
						</div>
				  </div>';
					  
	return $output;
	
}

function clra_application_preview_submit($form, &$form_state){
	global $base_root, $base_path, $user; 
	
	$user_id = $user->uid;
	$backlog_field_ids = '';
		
	$val = $form_state['values'];  
	$application_id	= encryption_decryption_fun('decrypt', $val['application_id']);
	$backlog_id	= encryption_decryption_fun('decrypt',$val['backlog_id']);
	$retFees = $_POST['retFees'];
		
	if($val['update_status'] == 'Y'){			
		$cl_number = $val['e_any_day_max_num_of_workmen'];
	
		if($cl_number <= 20 ){
			$totalretFees	=	'200';
		}else if($cl_number > 20 && $cl_number <= 50  ){
			$totalretFees	=	'500';
		}else if($cl_number > 50 && $cl_number <= 100  ){
			$totalretFees	=	'1000';
		}else if($cl_number > 100 && $cl_number <= 200  ){
			$totalretFees	=	'2000';
		}else if($cl_number > 200 && $cl_number <= 400  ){
			$totalretFees	=	'4000';
		}else if($cl_number > 400){
			$totalretFees	=	'5000';
		}
		
		$cln_sql_query = db_update('l_clra_registration_master');
		$cln_sql_query->fields(array(				 		
					'e_any_day_max_num_of_workmen' 	=> $val['e_any_day_max_num_of_workmen'],
					'finalfees' 					=> $totalretFees,
			 
		));
		$cln_sql_query->condition('id',$application_id);
		$cln_sql_query->condition('user_id',$user_id);
		$cln_sql_query->execute();
		
		drupal_goto('view-clra-application-details/verify/'.encryption_decryption_fun('encrypt', $application_id));
	}else{		
	
	if(!empty($backlog_id)){			
		 $backlog_field_ids	= 'e_name,e_postal_address,full_name_principal_emp,e_any_day_max_num_of_workmen,address_principal_emp';						
	}
			
	$fetch_reg_det = db_select('l_clra_registration_master', 'lcrm');
	$fetch_reg_det->fields('lcrm', array('application_date', 'created','full_name_principal_emp'));
	$fetch_reg_det->condition('lcrm.user_id', $user_id);
	$fetch_reg_det->condition('lcrm.id', $application_id); 
	$fetch_reg_det_result 	= $fetch_reg_det->execute(); 
	
		
	if( $fetch_reg_det_result->rowCount() > 0 ){		
		$obj2 = $fetch_reg_det_result->fetchAssoc(); 
		$application_date = !empty($obj2['application_date']) ? $obj2['application_date'] :'';
		$created_date = !empty($obj2['created'])? $obj2['application_date'] :'';
		$principal_emp_name	= $obj2['full_name_principal_emp'];			
	}
	
	$fetch_users_role = db_select('users_roles', 'ur');
	$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role->fields('ro', array('rid'));
	$fetch_users_role->condition('ur.uid', $user_id);
	$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc();

	$user_role =	$fetch_users_role_data['rid']; 
		
	$fieldsRemarksData = array( 
							'remarks'				=>	'Applied Successfully',
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
				 'created'				=> time(),
				 'user_from_portal'		=> 1
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
	
	//////////////////  CLRA API SERVICE FOR LD  /////////////////////////////////	
	
	$api_q2 = db_select('l_common_application_master', 'lcam');
	$api_q2->fields('lcam', array('ld_uid', 'ld_service_unique_id', 'ld_est_unique_id', 'clra_ld_service_id'));
	$api_q2->condition('lcam.user_id', $user_id);
	$api_q2_result = $api_q2->execute(); 
	$api_ap2_details = $api_q2_result->fetchAssoc();	
	$ld_id = trim($api_ap2_details['ld_uid']);
	
	if(!empty($ld_id) || $ld_id != ""){
		$status_arr = array (
 						'status' => '0',
 						'id' => $api_ap2_details['clra_ld_service_id'],
						'ld_uid' => $api_ap2_details['ld_uid'],
						'ld_est_unique_id' => $api_ap2_details['ld_est_unique_id'],
						);
		$array_enc = encrypt_swold($status_arr);
		$curl_post_data = array ('message' => $array_enc, 'data' => $array_enc); 		
				
		$service_url  =   "https://wblabour.gov.in/restld/establishmentStatusUpdate";
		$ch       =   curl_init();
		$headers    = array();
		$headers[]    = 'Accept: application/json';
		$headers[]    = 'Content-Type: application/json';
		$headers[]    =   'charset=utf-8';
		curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
		curl_setopt($ch, CURLOPT_URL, $service_url);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
		curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 0);
		curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode( $curl_post_data ) );
		curl_setopt($ch, CURLOPT_POST, true);
		$results    = curl_exec($ch);
		$errors     = curl_error($ch);
		curl_close($ch);
		$x = json_encode( $results );
		$tmp = json_decode(json_decode(trim($x)), TRUE);
 
	   if($errors){
		//  echo "<br>";
		//  print_r(curl_errno());
		//  print_r($errors);
	   }else{
		// echo "<br>";
		// echo "Ok";
		// echo "<br>";
	   }
	   
	   // print_r($results); echo "<br>";
	   // $return_msg_encrypted = json_decode($results, TRUE);
	   // $return_msg = decrypt_swold($return_msg_encrypted['message']);
	   // print_r($return_msg);	   
	   
		if($_SESSION['service_id'] == 1){
			drupal_goto('https://wblabour.gov.in/dashboard');
		}
	}
	//////////////////  END CLRA API SERVICE FOR LD  /////////////////////////////////	
		
	if(!empty($user_id)){ 
		$phone = get_common_function_for_phone();
		if(!empty($phone)){
			$msg	=	"Your application has been successfully submitted. You can track your application status from dashboard.";
						
			send_sms_for_user_alert($phone, $msg);
			drupal_goto('clra-application/view/'.encryption_decryption_fun('encrypt',$application_id));			
		}else{
			$msg	=	"Message not sent.Please try again later.";
			drupal_set_message(t($msg));
		}
	}	
		
	$message = 'Application is applied successfully. Inspection will be started shortly.';
	drupal_set_message(t($message));
	}
	
}