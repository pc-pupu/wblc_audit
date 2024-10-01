<?php

function view_cessapplication($form, &$form_state,$application_id='', $applicant_userId=''){
	global $base_root, $base_path, $user;
	drupal_add_library('system','ui.datepicker');
	drupal_add_js('jQuery(document).ready(function(){
		jQuery("#completion_date, #assesment_date, #appeal_date, #order_date").datepicker({
		 dateFormat: "dd-mm-yy",
		 yearRange: "-60:+1",
		 changeMonth: true,
		 changeYear: true,
		});
	});', 'inline');
	
	$applicantUserID  = encryption_decryption_fun('decrypt', $applicant_userId);
	$applicationId	  = encryption_decryption_fun('decrypt', $application_id);
	$user_id		  = $user->uid;						
	
	/**** Fetch ALC SUBDIVISION Code ****/
	
	$get_alcDetails		=	db_select('l_customuser_relation_address', 'lcra');
	$get_alcDetails		->  leftJoin('users_roles', 'ur','ur.uid=lcra.user_id ');
	$get_alcDetails		->  leftJoin('role', 'ro', 'ro.rid = ur.rid');
	$get_alcDetails		->  fields('ro', array('name','rid'));
	$get_alcDetails		->	fields('lcra',array('sub_div_code'));
	$get_alcDetails		->	condition('lcra.user_id',$user_id);
	$get_alcData 		= 	$get_alcDetails->execute();
	
	if($get_alcData-> rowCount() > 0 ){
		
		$alc_data 			  = $get_alcData->fetchAssoc();  
		$alc_subdivision_code = trim($alc_data['sub_div_code']);
		$alc_role_rid		  = $alc_data['rid']; 
		$alc_role_name		  = $alc_data['name']; 
		
	}
	
	/*** Get Applicant Block Code & Statuses ****/
	
	$get_cessDetails	=   db_select('l_cess_application_master', 'cess');
	$get_cessDetails	->  fields('cess',array('loc_e_subdivision','final_submit_status','status','identification_number'));
	$get_cessDetails	->	condition('cess.loc_e_subdivision',$alc_subdivision_code);
	$get_cessDetails	->  condition('cess.id', $applicationId);
	$get_cessDetails	->	condition('cess.final_submit_status','P');
	$cessData  			=   $get_cessDetails->execute()->fetchAssoc();
	
		
	if(!empty($cessData['status']) && $cessData['status']!=''){	
		if($cessData['status'] == '0'){
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Pending application</h4><p>Application is applied by the Applicant. Any action can be taken for the application.</p></div>';
		}
	}
	
	if($alc_subdivision_code == $cessData['loc_e_subdivision'] ){   /** Checking for If the Application can be viewed by the ALC of the Subdivision ****/
	
		$form['cess']['markup_data']	= array(
											'#markup' 		=> view_cess_application($applicationId,$applicantUserID),
											'#type' 		=> 'markup',
											);
		$form['cess']['show_status'] 	= array(
											'#type' 		=> 'markup',
											'#markup'		=> $current_status,
											);
											
		if ($applicant_status != 'B' && $applicant_status != 'V' && $applicant_status != 'I' && $applicant_status != 'VA' && $applicant_status != 'T' ){
			
			/*$form['cess']['hidden_message']= array(
											'#prefix'		=> '<div class="box box-default box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">ACTIONS AND REMARKS</h3></div><div class="box-body"><span style="margin:0; width:100%;">',
											'#suffix'		=> '</span>',
											'#type' 		=> 'markup',
											'#markup'		=> isset($form_state['values']['hidden_action_message']) ? $form_state['values']['hidden_action_message'] : ''
										);*/
			$form['cess']['applicant_userid']= array(
											'#type'			=> 'hidden',
											'#attributes' 	=> array('readonly' => 'readonly'),
											'#default_value'=> !empty($applicantUserID) ? encryption_decryption_fun('encrypt',$applicantUserID) : '',
											);
																									
			$form['cess']['application_id']	= array(
											'#type'			=> 'hidden',
											'#default_value'=> !empty($applicationId) ? encryption_decryption_fun('encrypt',$applicationId ): '',
											'#attributes' 	=> array('readonly' => 'readonly'),
											);
																				
			$form['cess']['identification_number']= array(
											'#title'		=> 'Identification Number',
											'#type'			=> 'hidden',
											'#default_value'=> !empty($cessData['identification_number']) ? encryption_decryption_fun('encrypt',$cessData['identification_number']): '',
											'#attributes' 	=> array('readonly' => 'readonly'),
											);
											
			/*<div class="form-group">
                <label>Date masks:</label>

                <div class="input-group">
                  <div class="input-group-addon">
                    <i class="fa fa-calendar"></i>
                  </div>
                  <input type="text" class="form-control" data-inputmask="'alias': 'dd/mm/yyyy'" data-mask="">
                </div>
                <!-- /.input group -->
              </div>https://adminlte.io/themes/AdminLTE/pages/forms/advanced.html*/
											
			$form['cess']['completion_date']= array(
											'#prefix'		=> '<div class="box box-default box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">TO BE FILLED BY ASSESSING OFFICER</h3></div><div class="box-body"><div class="col-md-4">',
											'#suffix'		=> '</div>',
											'#type' 		=> 'textfield',
											'#title'		=> 'Date of Completion',
											'#required' 	=>  TRUE,
											'#attributes' 	=> array('id'=>'completion_date','class'=> array('form-control'),'readonly' => 'readonly'),
										);
			$form['cess']['final_cost']= array(
											'#prefix'		=> '<div class="col-md-4">',
											'#suffix'		=> '</div>',
											'#type' 		=> 'textfield',
											'#title'		=> 'Final Cost',
											'#required' 	=>  TRUE,
											'#attributes' 	=> array('class'=> array('form-control')),
										);
			$form['cess']['assesment_date']= array(
											'#prefix'		=> '<div class="col-md-4">',
											'#suffix'		=> '</div>',
											'#type' 		=> 'textfield',
											'#title'		=> 'Date of Assessment',
											'#required' 	=>  TRUE,
											'#attributes' 	=> array('id'=>'assesment_date','class'=> array('form-control'),'readonly' => 'readonly'),
										);
			$form['cess']['assesed_amount']= array(
											'#prefix'		=> '<div class="col-md-4">',
											'#suffix'		=> '</div>',
											'#type' 		=> 'textfield',
											'#title'		=> 'Amount Assessed',
											'#required' 	=>  TRUE,
											'#attributes' 	=> array('class'=> array('form-control')),
										);
			$form['cess']['appeal_date']= array(
											'#prefix'		=> '<div class="col-md-4">',
											'#suffix'		=> '</div>',
											'#type' 		=> 'textfield',
											'#title'		=> 'Appeal Date, if any',
											'#required' 	=>  TRUE,
											'#attributes' 	=> array('id'=>'appeal_date','class'=> array('form-control'),'readonly' => 'readonly'),
										);
			$form['cess']['order_date']= array(
											'#prefix'		=> '<div class="col-md-4">',
											'#suffix'		=> '</div>',
											'#type' 		=> 'textfield',
											'#title'		=> 'Date of Order in Appeal',
											'#required' 	=>  TRUE,
											'#attributes' 	=> array('id'=>'order_date','class'=> array('form-control'),'readonly' => 'readonly'),
										);
			$form['cess']['appealed_amount']= array(
											'#prefix'		=> '<div class="col-md-4">',
											'#suffix'		=> '</div>',
											'#type' 		=> 'textfield',
											'#title'		=> 'Amount as per Order in Appeal',
											'#required' 	=>  TRUE,
											'#attributes' 	=> array('class'=> array('form-control')),
										);
										
			$form['cess']['transfer_date']= array(
											'#prefix'		=> '<div class="col-md-4">',
											'#suffix'		=> '</div>',
											'#type' 		=> 'textfield',
											'#title'		=> 'Date of transfer of cess to the Board',
											'#required' 	=>  TRUE,
											'#attributes' 	=> array('class'=> array('form-control')),
										);
										
			$form['cess']['remark_type']= array(
											'#prefix' 		=> '<div class="col-md-3">&nbsp;</div><div class="form-custom col-md-6"><label class="input">',
											'#suffix' 		=> '<i></i></label>',
											'#title'		=> 'Please Select Remark Actions',
											'#type'			=> 'select',
											'#required' 	=>  TRUE,
											'#id'			=> 'remark_type_id',
											'#options'		=> actions_dropdown_by_alc_act($cessData['status'],'Y',6),
											'#attributes'	=> array('onchange' => 'getRemarkTypeAction(this.value)', 'class'=>array('form-control')),
											'#ajax'			=> array(
																	'event' 			=> 'change',
																	'method' 			=> 'append',
																	'wrapper' 			=> 'content-wrapper',
																	'effect' 			=> 'fade',			
																	'callback'			=> 'get_call_date_time_field',
																	'progress'			=> array('type'=> 'throbber'),
																  ),
											);
											
			$value_remark_type = isset($form_state['values']['remark_type']) ? $form_state['values']['remark_type'] : '';
			
			$form['cess']['generate_acknowledgement'] 	= array(
											  '#prefix' 		=> '<div id="generate_acknowledgement_block">',
											  '#suffix' 		=> '</div>'
											);
			$form['cess']['remarks_text'] 	= array(
											  '#prefix' 		=> '<div id="remarks_text_block">',
											  '#suffix' 		=> '</div>'
											);
																							
			$form['cess']['submit'] 		= array(
											  '#prefix' 		=> '<div id="submit_block">',
											  '#suffix' 		=> '</div>'
											);
											
			if($value_remark_type == 'I'){
				
				//if(!empty($ismw_license_number) && !empty($iwm_license_qr_code)){
				if(!empty($cess['identification_number'])){
					
					$form['cess']['remarks_text']	= array(
											'#prefix' 			=> '<div id="remarks_text_block"><label class="input">',
											'#suffix' 			=> '</label></div>',
											'#title'			=> 'Remarks',
											'#type'				=> 'textarea',
											'#id'				=> 'remarks_text_id',
											'#rows'				=>  2,
											'#required' 		=>  TRUE,
											);
																									
					$form['cess']['submit'] = array (
										  '#prefix' 			=> '<div id="submit_block">',
										  '#suffix' 			=> '</div></div><div class="col-md-3">&nbsp;</div></div></div>',	
										  '#type' 				=> 'submit',
										  '#submit'				=> array('ismw_license_recruitment_view_submit'),
										  '#id'					=> 'ismw_empLicSubmitId',
										  '#attributes' 		=> array('class' => array('btn btn-primary pull-left acremark-btn')),
										  '#value' 				=> 'Submit'
									  
									);
				}else{
					$form['cess']['generate_acknowledgement']= array (
											  '#prefix' 		=> '<div id="generate_acknowledgement_block">',
											  '#suffix' 		=> '</div>',
											  '#type' 			=> 'submit',
											  '#attributes' 	=> array('class' => array('btn btn-primary pull-left acremark-btn')),
											  '#value' 			=> 'Generate BOCWA CESS Acknowledgement',
											  '#submit'			=> array('generation_cess_acknowledgement_number'),
											);
				}
			}else{
				$form['cess']['remarks_text']= array(
												'#prefix' 		=> '<div id="remarks_text_block"><label class="input">',
												'#suffix' 		=> '</label></div>',
												'#title'		=> 'Remarks',
												'#type'			=> 'textarea',
												'#id'			=> 'remarks_text_id',
												'#rows'			=>  2,
												'#required' 	=>  TRUE,
												);
																				
				$form['cess']['submit'] 	= array (
											  '#prefix' 		=> '<div id="submit_block">',
											  '#suffix' 		=> '</div></div><div class="col-md-3">&nbsp;</div></div></div>',
											  '#type' 			=> 'submit',
											  '#id'				=> 'ismw_empLicSubmitId',
											  '#attributes' 	=> array('class' => array('btn btn-primary pull-left acremark-btn')),
											  '#value' 			=> 'Submit'
												);
			}
										
		}
		$form['cess']['markup_data_5']	= array(
											//'#prefix'		=>'<div class="box box-default box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">REMARK SUMMARY</h3></div><div class="box-body">',
											//'#markup' 	    => $certificateDownloadLink.get_ismw_license_employment_remark_details($license_id,$contractor_user_id),
											'#suffix' 		=> '</div></div>',
											);
	}else{
		drupal_set_message(t('You are not permitted to view this application.'), 'error');
	}
	
	return $form;
}

function view_cess_application($applicationId='',$applicantUserID=''){
	
	global $base_root, $base_path, $user;
	
	$output = '';
	
	$get_cessDetails	=   db_select('l_cess_application_master', 'cess');
	$get_cessDetails	-> fields('cess',array());
	$get_cessDetails	->  condition('cess.id', $applicationId);
	$get_cessDetails	->  condition('cess.user_id', $applicantUserID);
	$cessData  			=   $get_cessDetails->execute()->fetchAssoc();
	
	$establishmentAddress = $cessData['loc_e_name'].'<br/>'.get_full_address('l_cess_application_master', 'sub-table', $applicationId, array('loc_e_dist','loc_e_subdivision','loc_e_areatype','loc_e_areatype_code','loc_e_vill_ward','loc_e_ps','loc_e_pin_number'));
	
	$output .= '<div class="box box-primary box-solid">
					<div class="box-header with-border">
					  <h3 class="box-title">Inputs are provided by </h3>
					</div>
					<div class="box-body">
						<table class="table table-bordered">
							<tbody>
							 <tr>
							  <th style="width:8%">Sl</th>
							  <th style="width:40%">Parameters</th>
							  <th>Inputs</th>
							</tr>
							<tr>
							  <td>1.(a)</td>
							  <td>Registration Number under <i>Building and Other Construction Workers (Regulation of Employment and Conditions of Service) Act,1996 </i></td>
							  <td>'.$cessData['registration_number'].'<br/> issued on '.date('dS M, Y',strtotime($cessData['registration_date'])).'</td>
							</tr>
							<tr>
							  <td>1.(b)</td>
							  <td>Establishment Name</td>
							  <td>'.$cessData['e_name'].'</td>
							</tr>
							<tr>
							  <td>1.(c)</td>
							  <td>Establishment Address</td>
							  <td>'.$establishmentAddress.'</td>
							</tr>
							<tr>
							  <td>2.</td>
							  <td>Name of Work</td>
							  <td>'.$cessData['construction_type'].'</td>
							</tr>
							<tr>
							  <td>3.</td>
							  <td>Number of Workers Employed</td>
							  <td>'.$cessData['no_of_workers_employed'].'</td>
							</tr>
							<tr>
							  <td>5.(a)</td>
							  <td>Date of commencement of work</td>
							  <td>'.date('dS M, Y',strtotime($cessData['date_of_commencement_of_work'])).'</td>
							</tr>
							<tr>
							  <td>5.(b)</td>
							  <td>Estimated period of work</td>
							  <td>'.date('dS M, Y',strtotime($cessData['estimated_period_of_work_from'])).' To '.date('dS M, Y',strtotime($cessData['estimated_period_of_work_to'])).'</td>
							</tr>
						</table>
					</div>
				</div>';
	return $output;
}

function get_call_date_time_field($form, $form_state){
			
	$commands 	= array();
	//$commands[] = ajax_command_replace('#download_link_block', drupal_render($form['cess']['download_link']));
	//$commands[] = ajax_command_replace('#ismw_signed_certificate_block', drupal_render($form['cess']['ismwemp_signed_license']));
	//$commands[] = ajax_command_replace('#fees_block', drupal_render($form['license_recruitment']['fees']));
	//$commands[] = ajax_command_replace('#reason_fees_block', drupal_render($form['license_recruitment']['reason_fees']));
	$commands[] = ajax_command_replace('#generate_acknowledgement_block', drupal_render($form['cess']['generate_acknowledgement']));
	$commands[] = ajax_command_replace('#remarks_text_block', drupal_render($form['cess']['remarks_text']));
	$commands[] = ajax_command_replace('#submit_block', drupal_render($form['cess']['submit']));	
		
	return array('#type' => 'ajax', '#commands' => $commands);
	
}