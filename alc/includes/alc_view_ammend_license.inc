<?php
drupal_add_js(drupal_get_path('module', 'alc').'/js/mycustom.js');
drupal_add_js(drupal_get_path('module', 'alc').'/js/view_doc.js');

function alc_view_ammend_license($form,&$form_state, $frmv_no='',$ammendid='' ,$createdby ='' ,$flag='',$license_renewal_ammendment_id=''){ 
	
	
	global $base_root, $base_path, $user;
	$alc_id 						= 	$user->uid;
	
	// if($alc_id ==460){
	
	$action		 					= 	'decrypt';
	$serial							=	encryption_decryption_fun($action, $frmv_no);
	$ammend_id						=	encryption_decryption_fun($action, $ammendid);
	$created_by						=	encryption_decryption_fun($action, $createdby);
	$application_flag 				=	encryption_decryption_fun($action, $flag);
	$licenserenewalammendmentid 	=	encryption_decryption_fun($action, $license_renewal_ammendment_id);
						  
	$return_data 		= return_data($ammend_id);
	$contrcator_name 	= $return_data['name_of_contractor'];					
 	$reference_no 		= return_reference_no_id($ammend_id);	
												  
	$last_remark_activity = last_remark_activity($reference_no);
	if(!empty($last_remark_activity['remark_field_title'])){
		$remark_field_title = $last_remark_activity['remark_field_title'];			
	}
	$not_amendment_check_field 	=	$last_remark_activity['not_amendment_check_field'];
	$amendment_check_field 		=	$last_remark_activity['amendment_check_field'];
	$remark_type 				=	$last_remark_activity['remark_type'];
		
	$lq = db_select('l_contractor_license_application', 'la');
	$lq->fields('la', array('id'));
	$lq->condition('la.serial_no_from_v', trim($serial));
	$lq->condition('la.status', 'I');
	$license_id = $lq->execute()->fetchObject()->id;
	
	
	$form['#attributes'] = array('enctype' => 'multipart/form-data');				  
	$form['view_ammend']['serial'] = array(							
						'#type' 				=> 	'hidden',
						'#default_value'		=> 	$frmv_no,		
						'#attributes'			=> 	array('readonly'=>'readonly')
						);
	
	$form['view_ammend']['particularid'] = array(
						'#type' 				=> 	'hidden',
						'#default_value'		=> 	encryption_decryption_fun('encrypt', $reference_no),
						);
	$form['view_ammend']['license_id'] = array(
						'#type' 				=> 	'hidden',
						'#default_value'		=> 	encryption_decryption_fun('encrypt', $license_id),
						);
	$form['view_ammend']['ammend_id'] = array(
						'#type' 				=> 	'hidden',
						'#default_value'		=> 	encryption_decryption_fun('encrypt', $ammend_id),
						);
	$form['view_ammend']['amendment_check_field'] = array(
						'#type' 				=> 	'hidden',
						'#default_value'		=> 	encryption_decryption_fun('encrypt', $amendment_check_field),
						);
						
	$form['view_ammend']['not_amendment_check_field'] = array(
						'#type' 				=> 	'hidden',
						'#default_value'		=> 	encryption_decryption_fun('encrypt', $not_amendment_check_field),
						);
	$form['view_ammend']['contrcator_name'] = array(
						'#type' 				=> 	'hidden',
						'#default_value'		=> 	encryption_decryption_fun('encrypt', $contrcator_name),
						);					
						
	$form['view_license']['subdivision_alc'] = array(
						'#type' 				=> 	'hidden',
						'#default_value'		=> 	$subdivision_alc,
						);
																		
	$form['view_ammend']['editable_fields_ammend'] = array(																					
						'#type' 				=> 	'hidden',
						'#default_value'		=> 	$remark_field_title,
						'#attributes'			=> 	array('readonly'=>'readonly','id' => 'editable_fields_license')
						);
															

	$form['view_ammend']['hidden_action_message'] = array(
						 '#type' => 'hidden',
						 );
				 
	$record_details  = db_query( "select  * from fun_basic_information1('".$created_by."','".$licenserenewalammendmentid."','".$application_flag."')")->fetchAssoc();
	
	if(!empty($record_details)){
	$license_no = !empty($record_details['backlog_license_no'])? $record_details['backlog_license_no'].' <br/>'.$record_details['contractor_license_number']:$record_details['contractor_license_number'];
	$license_date = !empty($record_details['backlog_license_date'])?date('dS M Y', strtotime($record_details['backlog_license_date'])):date('dS M Y', strtotime($record_details['license_date']));
	$validated_date = !empty($record_details['next_renweal_date'])?date('dS M Y', strtotime($record_details['next_renweal_date'])):'';
	}
	//print_r($record_details);die();	
	$show_details =	view_ammend_details($serial,$ammend_id,$licenserenewalammendmentid,$application_flag,$record_details['certificateid'],$created_by);
	$form['view_ammend']['note'] = array(
			'#prefix' 		=> '<div class="box box-primary box-solid">
									<div class="box-header with-border"><i class="ion ion-clipboard"></i> License Information</div>
									<div class="box-body"><div class="feedback-scroll">
										<table cellpadding="0" cellspacing="0" border="0" width="100%" class="table table-bordered table-responsive">
											<tr>
												<td width="25%"><strong>Form-V Number</strong></td>
												<td>00'.$serial.'</td>
												<td width="25%"><strong>License Number</strong></td>
												<td>'.$license_no.'</td>
											</tr>											
											<tr>
												<td><strong>Issue Date</strong></td>
												<td>'.date('d/m/Y', strtotime($license_date)).'</td>											
												<td><strong>Valid Upto</strong></td>
												<td>'.$validated_date.'</td>
											</tr>
										</table></div>
									</div>
								</div>',
			'#suffix' 		=> '',			
			'#markup' 		=> 	$show_details,		
			'#type' 		=> 'markup',														
		);
		
		
	 $pre_remark = remark_details_second_last($reference_no);	
	 
	 if(!empty($remark_type)){	
		if( trim($remark_type) == "A") { 
		
			$current_status = 'CURRENT STATUS: Application has been VERIFIED & GIVEN APPROVAL FOR FEES SUBMISSION';
			
		}else if( trim($remark_type) == "F") { 	
		
			$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> Current status: Pending application</h4><p>Application is applied by the Applicant. Any action can be taken for the application.</p></div>';
			
		}else if( trim($remark_type) == "WF") { 
								
			$current_status = 'CURRENT STATUS: Application has been VERIFIED & GIVEN APPROVAL WITH OUT FEES SUBMISSION';
							
		}else if($remark_type == "B"){
          		
			$current_status = 'CURRENT STATUS: Application has been BACKED to Applicant for rectification';
						
         }else if( $remark_type == "P" ){
				
			$current_status = 'CURRENT STATUS: TRANSACTION IS SUCCESSFULL for this Application';			  	
			  
          }else if( $remark_type == "BI" ){				
				
				$current_status 	= 'CURRENT STATUS: Application has been BACKED to Inspector';
				
          }else if($remark_type == "C"){
				
				$current_status = 'CURRENT STATUS: Application has been BACKED TO INSPECTOR';
				
          }else if( $remark_type == "I"){ 
		           		
				$current_status = '<div class="callout callout-success"><h4>CURRENT STATUS: Certificate (FORM-VI) is issued. </h4><p>Please check the License Certificate.</div>';				
				
          }else if( $remark_type == "R"){
				
				$current_status = 'CURRENT STATUS: Application has been REJECTED';				
				
          }else if( $remark_type == "FW"){
				$current_status = 'CURRENT STATUS: Application has been FORWARDED TO ALC';
				
          }else if( $remark_type == "U"){
				$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> Current status: Signed amendment application uploaded & application submission completed</h4><p>Signed amendment application is submitted by the Applicant. After verification of uploaded application, Issue of license can be generated now or back to rectification application.</p></div>';
				
          }else if( $remark_type == "AW"){
				$current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> Current status: Application has been verified &amp; approved without fees.</h4><p>Application is approved without fees. After submission of signed application by the applicant, the application can be further accessible. If you want to get back to the previous remark, delete the current remark by clicking the delete option.</p></div>';
				
          }else if( $remark_type == "AW" && $pre_remark == 'U'){
				$current_status = 'CURRENT STATUS:ALC backed the Application for rectification of Signed Application';
		  }
	}
	
	$form['view_ammend']['show_status'] = array(
					  '#prefix'	=> '<br class="clear"  />',
					  '#suffix'	=> '',
					  '#type' 	=> 'markup',
					  '#markup'	=> $current_status
					 );
   
   $form['view_ammend']['mark_up_div'] = array(
					'#prefix' => '<div class="box box-default box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">ACTIONS AND REMARKS</h3></div><div class="box-body">',
				   '#type' 	  => 'markup'
				  );			

	$ammendment_status = $return_data['ammendment_status']; 
	$qr_code = $return_data['qr_code'];
	$ammendment_certificate_id = $return_data['ammendment_certificate_id'];
	$payable_amendment_security_fees = $return_data['payable_amendment_security_fees'];
	$payable_amendment_fees	= $return_data['payable_amendment_fees'];	
	$upload_certificate_file = fetch_uplaoded_file_details($ammendment_certificate_id);
	
	if(!empty($upload_certificate_file)){ 
		$url			  = explode('//',$upload_certificate_file['uri']);
		$certificate_url  = $url[1];
		$certificate_file = $upload_certificate_file['filename'];
					
		$form['view_ammend']['show_certificates'] 		= array(
				  '#prefix'	=> '<div class="view_test" align="left">',
				  '#suffix'	=> '</div>',
				  '#type' 	=> 'markup',
				  '#markup' => 'View Certificate of Amendment <a href="'.$GLOBALS['base_url'].'/sites/default/files/'.$certificate_url.'" target="_blank"><i class="fa fa-file fa-lg"></i></a>'
				);
	}

	
	if($remark_type !== 'B' && $remark_type !== 'A' && $remark_type !== 'AW' && $remark_type !== 'I' && $remark_type !== 'BA' && $remark_type !== 'P'){
		$form['view_ammend']['action_type']	= array(
				'#prefix' 		=> '<div class="col-md-3">&nbsp;</div><div class="form-custom col-md-6"><label class="input">',
				'#suffix' 		=> '<i></i></label>',
				'#title'		=> 'Please Select Actions',
				'#type'			=> 'select',
				'#required' 	=>  TRUE,
				'#attributes' 	=> array('id' => 'edit-action-type', 'class' => array('form-control')),
				'#options'		=>  ammend_actions_dropdown_by_alc($ammendment_status,$payable_amendment_security_fees,$payable_amendment_fees,$ammend_id),
				'#ajax'			=> array(
										'event' 			=> 'change',
										'effect' 			=> 'fade',			
										'callback'			=> 'get_action_field',
										'progress'			=> '',
									),
			);
							
		$form['view_ammend']['download_certificates'] 		= array(
				'#prefix' => '<div id="genarate_certificates">',
				'#suffix' => '</div>'
				);
				
				
				
		$original_id = $element['#id'];
   		$element['#id'] .= '-upload';

		$form['view_ammend']['certificates'] = array(
				  //'#prefix' => '<div id="'.$original_id . '-ajax-wrapper">',
				  '#prefix'   => '<div id="signed_license_certificate_block">',				  
				  '#suffix'   => '</div>'
				);
																	
		$value_action_type = isset($form_state['values']['action_type']) ? $form_state['values']['action_type'] : '';
		$indicator = encryption_decryption_fun('encrypt', 'A');	
		
		$form['view_ammend']['remarks_text'] 		= array(
															  '#prefix' 		=> '<div id="remarks_text_div">',
															  '#suffix' 		=> '</div>'
															);
																							
		$form['view_ammend']['submit'] 				= array(
															  '#prefix' 		=> '<div id="submit_div">',
															  '#suffix' 		=> '</div>'
															);
										
		if($value_action_type == 'I'){
			
			if($serial != 118502){
			
			 	$form['view_ammend']['download_certificates']	= array(
													'#prefix' => '<div id="genarate_certificates">',
													'#markup' => l(t('<i class="fa fa-download" aria-hidden="true"></i>Download License Certificate'),'clra-amendment-renewal-certificates/'.$frmv_no.'/'.$ammendid.'/'.$createdby.'/'.encryption_decryption_fun('encrypt','A'), array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('acremark-btn-dn')))),
													'#suffix' => '</div><div class="clear"></div>',
						);	
			  
		
				$form['view_ammend']['certificates']= array(
													'#title'			=> 'Upload Signed Certificate :-',
													'#type' 			=> 'managed_file',
													'#upload_validators'=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
													'#upload_location' 	=> 'public://upload/alc_upload_certificates_license_amend/',
													'#process' 			=> array('import_my_file_element_process'),
													'#attributes' 		=> array('autocomplete' => 'off','class' => array('form-group')),
													'#prefix'			=> '<div id="signed_license_certificate_block">',
													'#suffix'			=> '</div>',
													'#required' 		=>  TRUE,
													);
				$form['view_ammend']['remarks_text'] = array(																									
														'#prefix' 		=> '<div id="remarks_text_div"><label class="textarea label-block">',
														'#suffix' 		=> '</label></div>',
														'#title'		=> 'Remarks',
														'#type'			=> 'textarea',
														'#attributes'   => array('class' => array('form-control')),
														'#rows'			=>  2,
														'#required' 	=>  TRUE,
													);
																												
				$form['view_ammend']['submit'] = array (
													  '#type' 		=> 'submit',
													  '#attributes' => array('class' => array('button')),
													  '#value' 		=> 'Submit',
													  '#prefix'		=> '<div id="submit_div">',
													  '#suffix' 	=> '</div></div><div class="col-md-3">&nbsp;</div></div></div>',	
													  '#attributes' => array('class' => array('btn btn-primary pull-left acremark-btn'))
													);
													
			}else{
													
			$form['view_ammend']['download_certificates'] 		= array (
														  '#prefix' 			=> '<div id="genarate_certificates">',
														  '#suffix' 			=> '</div><div class="clear"></div>',	
														  '#type' 				=> 'submit',
														  '#submit'				=> array('alc_view_ammend_license_submit'),
														  '#id'					=> 'ismw_empLicSubmitId',
														  '#attributes' 		=> array('class' => array('btn btn-primary pull-left acremark-btn')),
														  '#value' 				=> 'Generate Amendment Certificate'
													  
													);
			}

		}
		
		
		
		if($value_action_type != 'I'){
		
		
		
		
		
				$form['view_ammend']['remarks_text'] = array(																									
														'#prefix' 		=> '<div id="remarks_text_div"><label class="textarea label-block">',
														'#suffix' 		=> '</label></div>',
														'#title'		=> 'Remarks',
														'#type'			=> 'textarea',
														'#attributes'   => array('class' => array('form-control')),
														'#rows'			=>  2,
														'#required' 	=>  TRUE,
													);
																												
				$form['view_ammend']['submit'] = array (
													  '#type' 		=> 'submit',
													  '#attributes' => array('class' => array('button')),
													  '#value' 		=> 'Submit',
													  '#prefix'		=> '<div id="submit_div">',
													  '#suffix' 	=> '</div></div><div class="col-md-3">&nbsp;</div></div></div>',	
													  '#attributes' => array('class' => array('btn btn-primary pull-left acremark-btn'))
													);
			}
		}

		
	$form['view_ammend']['remark'] = array(
			'#prefix' => '<div class="box box-primary box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i> Remark details</div><div class="box-body"><div class="feedback-scroll">',
			'#suffix' => '</div></div></div>',
			'#markup' => show_remark_details($reference_no),
			'#type'   => 'markup',														
		);
	// }else{
		
	// 	echo 'Service Under Maintenanace'; die;	
	// }
		
		
	return $form;
	
}

function view_ammend_details($serial='',$ammendid='' ,$license_renewal_ammendment_id,$application_flag,$previous_license_certificate,$created_by){ 

	global $base_root, $base_path, $user;
	
	$pe_details_reg 	= 'View Details';
	$pe_details_reg 	= l($pe_details_reg, 'amendment_license_renewal/view_pe_details/'.encryption_decryption_fun('encrypt', trim($serial)).'/'.encryption_decryption_fun('encrypt', trim($created_by)), array('html' => TRUE,'target'=>'blank'));
	$details 							= 	contractor_regmaster_details($serial);
	
	$establishment_address				=	"";
	$establishment_address				=	miscellaneous_establishment_address_form($details['loc_e_areatype'],$serial_no,$details['id']);
	$establishment_address				=	$details['e_name'].'<br/>'.$details['loc_e_name'].'</br>'.$establishment_address;
	$nature_of_work_pe 					= 	'';
	$nature_of_work_pe 					= 	nature_of_work_pe($details['id']);
	$registration_date_timestamp	 	=  strtotime($details['registration_date']);
	$formatted_reg_date 				=  date('m/d/Y', $registration_date_timestamp);
	
	$establish_details_reg				=	"";
	$establish_details_reg				=	$establish_details_reg."Registration Number: ".$details['registration_number']."<br/>"."Date of Certificates: ".$formatted_reg_date."\nUnder The acts of CLRA";	 
	$principal_details					=	$details['full_name_principal_emp']."</br>".$details['address_principal_emp']."</br>".$principal_address;
	
	$result_work 						=   get_nature_of_work_contrcator($details['id'],$serial); //---miscellaneous module
	$contractor_max_no_of_labours_pe	=	$details['contractor_max_no_of_labours_on_any_day'];
	
	if($result_work->rowCount() > 0 ){
		 foreach($result_work as $objwork){			
				$nature_work=$nature_work.$objwork->cont_work_name.', ';		  
		  }
	}
	$sub_con    	= 	substr($nature_work, 0, -2);
	$nature_work	=	$sub_con;	
	$labour_from_date					=	$details['est_date_of_work_of_each_labour_from_date'];
	$labour_to_date						=	$details['est_date_of_work_of_each_labour_to_date'];
	$name_of_contractor_pervious	 	= 	$details['name_of_contractor'];
	$pe_contractor_address				=   $details['address_of_contractor'].'<br/>'. miscellaneous_contractor_address_form($details['con_loc_e_areatype'],$serial);
	
	$estimated_date					    =	date_calculation($labour_from_date,$labour_to_date);
	
	$check_ammendment_table 			=   check_ammendment_table_with_id($ammendid);	
	$details_ammendment_table 			=   $check_ammendment_table->fetchAssoc();
	
	$name_of_contractor 				=   $details_ammendment_table['name_of_contractor'];
	$father_contarctor_name 			= 	!empty($details_ammendment_table['father_contarctor_name'])?$details_ammendment_table['father_contarctor_name']:'Nil';
	$contractor_convicted				= 	$details_ammendment_table['details_contractor_convicted'];
	$contractor_revoking				= 	!empty($details_ammendment_table['details_contractor_revoking'])?date('dS F, Y', strtotime($details_ammendment_table['details_contractor_revoking'])):'Nil';
	$address_line1_of_contractor	    = 	$details_ammendment_table['contractor_areatype'];
	$name_of_agent						= 	$details_ammendment_table['name_of_agent'];
	$address_line1_of_manager			= 	$details_ammendment_table['contractor_manager_areatype'];
	$created_by							= 	$details_ammendment_table['created_by'];
	$address_of_contractor_line1		=	$details_ammendment_table['address_of_contractor'] ;
	$name_of_agent 					 	=   $details_ammendment_table['name_of_agent'];
	$category_of_contractor				= 	$details_ammendment_table['category_of_contractor'];
	$dob_contractor						=   !empty($details_ammendment_table['dob_contractor'])?date('dS M, Y', strtotime($details_ammendment_table['dob_contractor'])):'';
	$age_contractor						=   !empty($details_ammendment_table['age_contractor'])?$details_ammendment_table['age_contractor']:'Nil'; 
	$max_of_contract_labour_ammend		= 	$details_ammendment_table['max_of_contract_labour'];
	$is_coparative						= 	!empty($details_ammendment_table['is_coparative'])?$details_ammendment_table['is_coparative']:0;
	$category_designation				= 	$details_ammendment_table['category_designation'];
	$unskilled_rate_wages				= 	!empty($details_ammendment_table['unskilled_rate_wages'])?$details_ammendment_table['unskilled_rate_wages']:'Nil';
	$semiskilled_rate_wages				= 	!empty($details_ammendment_table['semiskilled_rate_wages'])?$details_ammendment_table['semiskilled_rate_wages']:'Nil';
	$skilled_rate_wages					=  !empty($details_ammendment_table['skilled_rate_wages'])?$details_ammendment_table['skilled_rate_wages']:'Nil';
	$highlyskilled_rate_wages			=  !empty($details_ammendment_table['highlyskilled_rate_wages'])?$details_ammendment_table['highlyskilled_rate_wages']:'Nil';
	$hours_work							=  !empty($details_ammendment_table['hours_work'])?$details_ammendment_table['hours_work']:'0';
    $spred_over 						=  !empty($details_ammendment_table['spred_over'])?$details_ammendment_table['spred_over']:'0';
	$overtime_wages						=  !empty($details_ammendment_table['overtime_wages'])?$details_ammendment_table['overtime_wages']:'0';
	$overtime							=  !empty($details_ammendment_table['overtime'])?$details_ammendment_table['overtime']:'0';	
	
	$annual_leave_no 					= !empty($details_ammendment_table['annual_leave_no'])?$details_ammendment_table['annual_leave_no']:'Nil';
	$casual_leave_no 					= !empty($details_ammendment_table['casual_leave_no'])?$details_ammendment_table['casual_leave_no']:'Nil';
	$sick_leave_no 						= !empty($details_ammendment_table['sick_leave_no'])?$details_ammendment_table['sick_leave_no']:'Nil';
	$maternity_leave_no 				= !empty($details_ammendment_table['maternity_leave_no'])?$details_ammendment_table['maternity_leave_no']:'Nil';
	$earned_leave_no 					= !empty($details_ammendment_table['earned_leave_no'])?$details_ammendment_table['earned_leave_no']:'Nil';
	$other_leave_no 					= !empty($details_ammendment_table['other_leave_no'])?$details_ammendment_table['other_leave_no']:'Nil';
	$holiday_wages						= !empty($details_ammendment_table['holiday_wages'])?$details_ammendment_table['holiday_wages']:(!empty($holiday_wages_license_renewal)?$holiday_wages_license_renewal:'Nil');
	$no_holiday							= !empty($details_ammendment_table['no_holiday'])?$details_ammendment_table['no_holiday']:'';
	$reference_no						= $details_ammendment_table['reference_no'];
	$is_flag							= $details_ammendment_table['is_flag'];
	$status 							= $details_ammendment_table['ammendment_status'];
	
	$rest_holiday		 				= substr($no_holiday, 0, -1);
	$no_holiday_list					= explode(',', $rest_holiday);
	$name_holiday		 				= "";
	$total_holiday		 				= 0;
	
	
	
    foreach($no_holiday_list as $row){
	  $total_holiday=$total_holiday+1;
		if($row=='1'){
			$name_holiday	.=	'Sunday'.',';
		}
		if($row=='2'){
			$name_holiday	.=	'Monday'.',';
		}
		if($row=='3'){
			$name_holiday	.=	'Tuesday'.',';
		}
		if($row=='4'){
			$name_holiday	.=	'Wednesday'.',';
		}
		if($row=='5'){
			$name_holiday	.=	'Thursday'.',';
		}
		if($row=='6'){
			$name_holiday	.=	'Friday'.',';
		}
		if($row=='7'){
			$name_holiday	.=	'Saturday'.',';
		}
		
	}
	$name_holidays 			= 	substr($name_holiday, 0, -1);
	if(empty($name_holidays))
		$name_holidays	=	'NIL';
		$special_benifites					= !empty($details_ammendment_table['special_benifites'])?$details_ammendment_table['special_benifites']:'Nil';
		$state_insurance					= !empty($details_ammendment_table['state_insurance'])?$details_ammendment_table['state_insurance']:'Nil';
		$details_contractor_convicted		= !empty($details_ammendment_table['details_contractor_convicted'])?date('dS F, Y', strtotime($details_ammendment_table['details_contractor_convicted'])):'Nil';
	//$contractor_revoking				= !empty($details_ammendment_table['details_contractor_revoking'])?$details_ammendment_table['details_contractor_revoking']:'Nil';
	$miscellaneous_provisions			= !empty($details_ammendment_table['miscellaneous_provisions'])?$details_ammendment_table['miscellaneous_provisions']:'Nil';
	
	$license_id = return_ammendement_license_id($ammendid);
	
	/*$ammendment_address_contractor_manager =  ammendment_address($license_id,'ammendment_manager_address');
	$ammendment_address_contractor =  ammendment_address($license_id,'ammendment_contrcator_address');
	$ammendment_address_worksite = ammendment_address($license_id,'ammendment_worksite_address'); */
	
	$ammendment_address_contractor_manager =  ammendment_address($details_ammendment_table['license_id'],'ammendment_manager_address');
	$ammendment_address_contractor =  ammendment_address($details_ammendment_table['license_id'],'ammendment_contrcator_address');
	$ammendment_address_worksite = ammendment_address($details_ammendment_table['license_id'],'ammendment_worksite_address'); 
	
	
	
	
	$payable_amendment_fees						 = $details_ammendment_table['payable_amendment_fees'];
	$payable_amendment_security_fees			 = $details_ammendment_table['payable_amendment_security_fees'];	
	$license_renewal_fees						 = $details_ammendment_table['license_renewal_fees'];
	$license_renewal_security_fees				 = $details_ammendment_table['license_renewal_security_fees']; 
	$applicable_amendment_fee					 = $details_ammendment_table['applicable_amendment_fee'];
	$applicable_security_fess					 = $details_ammendment_table['applicable_security_fess'];
	$created_by									 = $details_ammendment_table['created_by'];
	$previous_highest_deposit_fees				 = $details_ammendment_table['previous_highest_deposit_fees'];
	$previous_highest_security					 = $details_ammendment_table['previous_highest_security'];
	$upload_singed_from_id						 = $details_ammendment_table['upload_singed_from'];
	
	
	$file_details								=  particular_file($serial,$created_by);
	$frm_v_file_id_previous						=  $file_details['frm_v_file_id'];

	$residential_file_id_previous				=  $file_details['residential_file_id'];
	$other_file_id_previous						=  $file_details['other_file_id'];
	$work_order_file_id_previous				=  $file_details['work_order_file_id'];
	$payment_details 							= 	payment_details($reference_no,$created_by,'14');	
	
	if($application_flag == 'R'){
		$renewal_file_details 					=  renewal_file($license_renewal_ammendment_id);
		$form_iv_vii_id_previous				=  $renewal_file_details['upload_signed_form'];	
		$certificate_previous					=  $renewal_file_details['renewal_certificate_id'];	
	}if($application_flag == 'L'){
		$form_iv_vii_id_previous				=  $file_details['signed_license_id'];	
		$certificate_previous					=  $previous_license_certificate;	
	}
	
	//---------------AMMEND FILE DETAILS----------------------
	
	$other_file_id 								= !empty($details_ammendment_table['other_file_id'])?$details_ammendment_table['other_file_id']:$other_file_id_previous;
	//$form_iv_vii_id								= $frm_v_file_id_previous;
	
	$supportive_documents_worksite_id 			= $details_ammendment_table['supportive_documents_worksite'];
	$payment_recipt_file_id 					= $details_ammendment_table['payment_recipt_file'];
	
	$certificate_id								= $details_ammendment_table['ammendment_certificate_id'];
	
	$work_order_file_id							= !empty($details_ammendment_table['work_order_file_id'])?$details_ammendment_table['work_order_file_id']:$work_order_file_id_previous;
	
	
	
	$uploaded_workorder_file = fetch_uplaoded_file_details($work_order_file_id);
	if(!empty($uploaded_workorder_file)){ 
			$url			 	= 	explode('//',$uploaded_workorder_file['uri']);
   			$work_order_url		= 	$url[1];
			//$workorder_file		=	$uploaded_workorder_file['filename'];
	}
	$uploaded_frmv_file = fetch_uplaoded_file_details($frm_v_file_id_previous);
	if(!empty($uploaded_frmv_file)){ 
			$url 				= explode('//',$uploaded_frmv_file['uri']);
   			$frmv_url			= $url[1];
			//$frmv_file			= $uploaded_frmv_file['filename'];
	}
	$uploaded_residential_file = fetch_uplaoded_file_details($residential_file_id_previous);
	if(!empty($uploaded_residential_file)){ 
			$url				= explode('//',$uploaded_residential_file['uri']);
   			$residential_url	= $url[1];
			//$residential_file	= $uploaded_residential_file['filename'];
	}
	$upload_form_iv_vii_file = fetch_uplaoded_file_details($frm_v_file_id_previous);
	
	if(!empty($upload_form_iv_vii_file)){ 
			$url				= explode('//',$upload_form_iv_vii_file['uri']);
   			$form_iv_vii_url	= $url[1];
		//	$form_iv_vii_file	= $upload_form_iv_vii_file['filename'];
	}

	
	
	
	
	$supportive_documents_worksite_file = fetch_uplaoded_file_details($supportive_documents_worksite_id);
	if(!empty($supportive_documents_worksite_file)){ 
			$url									= explode('//',$supportive_documents_worksite_file['uri']);
   			$documents_worksite_file_url			= $url[1];
			//$documents_worksite_file_file			= $supportive_documents_worksite_file['filename'];
	}
	
	$other_file 							= fetch_uplaoded_file_details($other_file_id);
	
	if(!empty($other_file)){ 
			$url						= explode('//',$other_file['uri']);
   			$other_file_url				= $url[1];
			//$recipt_file_file						= $payment_recipt_file['filename'];
	}
	
	
	
	if($category_of_contractor=='1' && !empty($category_of_contractor)){
		    $type = 'Company';
			$title = 'Date of commencement of business '.$dob_contractor;
			
	}else{
		    $type = 'Individuals';
			$title = 'Date of Birth and age (incase of individuals):'.$dob_contractor;
			$age_contractor= 'Age: '.$age_contractor.' Year(s)';
	}
	 if($is_coparative == 1)
	      $coparative = 'Yes';
	  if($is_coparative != 1)
	 	 $coparative = 'No';
		 
		  $last_remark_activity = last_remark_activity($reference_no);
		 

	 if($is_flag == 'R'){
			$name_of_contractor_pervious	 						= 	$details['name_of_contractor'];
			$particular_data 										= 	particular_data($serial,$created_by);
		
			//print_r($particular_data);die();
			$renewal_data 											= 	renewal_data_last($license_renewal_ammendment_id);
			
			$work_subdivision_license_renewal						=   $particular_data['worksite_subdivision'];
			$particularid											= 	$particular_data['id'];
			$name_of_agent_license_renewal 							= 	$particular_data['name_of_agent'];
			$father_contarctor_name_license_renewal 				=   !empty($particular_data['father_contarctor_name'])?$particular_data['father_contarctor_name'] : 'Nil';
			$category_of_contractor_license_renewal					=   $particular_data['category_of_contractor'];
			$dob_contractor_license_renewal							= 	!empty($particular_data['dob_contractor'])?$particular_data['dob_contractor'] :'';
			$dob_age_contractor_license_renewal						= 	$particular_data['age_contractor'];
			$contractor_manager_areatype							=   !empty($particular_data['contractor_manager_areatype'])?miscellaneous_contractor_manager_address_form($particular_data['contractor_areatype'],$serial,$created_by):'';
			$work_site_address_license_renewal						=   !empty($particular_data['work_site_areatype'])?miscellaneous_worksite_address($serial,$particularid):'';
			
			$contractor_address_license_renewal						=   !empty($particular_data['contractor_areatype'])?miscellaneous_contractor_update_address_form($particular_data['contractor_areatype'],$serial,$created_by):''; 
			$is_coparative_license_renewal							=   $particular_data['is_coparative'];
		
			$address_of_manager_license_renewal			  			= 	$particular_data['address_of_manager'].'<br/>'.$contractor_manager_areatype; 
			$category_designation_license_renewal	 				= 	$particular_data['category_designation']; 
			$work_order_file_license_renewal_id						=	$particular_data['work_order_file_id'];
			$frm_v_file_id											=	$particular_data['frm_v_file_id'];
			$residential_file_id									=	$particular_data['residential_file_id'];
			$other_file_id_license_renewal							=	$particular_data['other_file_id'];
			$contractor_convicted_license_renewal					=	!empty($particular_data['details_contractor_convicted'])?date('dS F, Y', strtotime($particular_data['details_contractor_convicted'])):'Nil';
			
			$contractor_max_labours_license_renewal					=   $renewal_data['max_of_contract_labour'];
			
			$unskilled_rate_wages_license_renewal					= 	!empty($renewal_data['unskilled_rate_wages'])?'<del>&#2352;</del>'.$renewal_data['unskilled_rate_wages'].' ( Rupees '.convertNumberToWord($renewal_data['unskilled_rate_wages']).' only)':'Nil';
			$semiskilled_rate_wages_license_renewal					= 	!empty($renewal_data['semiskilled_rate_wages'])?'<del>&#2352;</del>'.$renewal_data['semiskilled_rate_wages'].' ( Rupees '.convertNumberToWord($renewal_data['semiskilled_rate_wages']).' only)':'Nil';
			$skilled_rate_wages_license_renewal						= 	!empty($renewal_data['skilled_rate_wages'])?'<del>&#2352;</del>'.$renewal_data['skilled_rate_wages'].' ( Rupees '.convertNumberToWord($renewal_data['skilled_rate_wages']).' only)':'Nil';
			$highlyskilled_rate_wages_license_renewal				= 	!empty($renewal_data['highlyskilled_rate_wages'])?'<del>&#2352;</del>'.$renewal_data['highlyskilled_rate_wages'].' ( Rupees '.convertNumberToWord($renewal_data['highlyskilled_rate_wages']).' only)':'Nil';
			
			
		
		   $annual_leave_no_license_renewal							= 	!empty($renewal_data['annual_leave_no'])?$renewal_data['annual_leave_no']:'Nil';
		   $casual_leave_no_license_renewal							= 	!empty($renewal_data['casual_leave_no'])?$renewal_data['casual_leave_no']:'Nil';
		   $sick_leave_no_license_renewal							= 	!empty($renewal_data['sick_leave_no'])?$renewal_data['sick_leave_no']:'Nil';
		   $other_leave_no_license_renewal							= 	!empty($renewal_data['other_leave_no'])?$renewal_data['other_leave_no']:'Nil';
		   $maternity_leave_no_license_renewal						=	!empty($renewal_data['maternity_leave_no'])?$renewal_data['maternity_leave_no']:'Nil';
		   $earned_leave_no_license_renewal							=	!empty($renewal_data['earned_leave_no'])?$renewal_data['earned_leave_no']:'Nil';
			
			
			
		
			$hours_work_license_renewal								=	$renewal_data['hours_work'];
			$spred_over_license_renewal								=	$renewal_data['spred_over'];
			$holiday_wages_license_renewal							=	$renewal_data['holiday_wages'];
			$weekly_holiday_license_renewal							= 	$renewal_data['weekly_holiday'];
		  	$no_holiday_license_renewal								= 	$renewal_data['no_holiday'];
			$overtime_license_renewal								= 	!empty($renewal_data['overtime'])?$renewal_data['overtime']:0;
			$overtime_wages_license_renewal							= 	$renewal_data['overtime_wages'];
			
			
			$special_benifites_license_renewal						=	!empty($renewal_data['special_benifites'])?$renewal_data['special_benifites']:'Nil';
			$state_insurance_license_renewal						= 	!empty($renewal_data['state_insurance'])?($renewal_data['state_insurance']):'Nil';
			$miscellaneous_provisions_license_renewal				=	!empty($renewal_data['miscellaneous_provisions'])?$renewal_data['miscellaneous_provisions']:'Nil';
			$contractor_revoking_license_renewal					=	!empty($renewal_data['details_contractor_revoking'])?date('dS F, Y', strtotime($renewal_data['details_contractor_revoking'])):'Nil';
			
			$form_iv_vii_id_license_renewal							=	$renewal_data['upload_signed_form'];
			$license_renewal_ammendment_certificate_id				=	$renewal_data['renewal_certificate_id'];
			
			$generated_form_iv_vii									=	l(t('Generated FORM-VII'),'pdf-form-vii/'.encryption_decryption_fun('encrypt', $license_renewal_ammendment_id).'/'.encryption_decryption_fun('encrypt', $created_by).'/Form-VII'.time(), array('html' => TRUE,'attributes'=> array('target'=>'_blank')));
				
		$payment_details_license_renewal_amand			= 	payment_details($license_renewal_ammendment_id,$created_by,'13');	
			
			
		
	 }
	if($is_flag == 'L'){
		$particular_data = 	particular_data($serial, $created_by);
			
		if(!empty($particular_data)){
		
			$particularid									= $particular_data['id'];
			$father_contarctor_name_license_renewal 		= !empty($particular_data['father_contarctor_name'])?$particular_data['father_contarctor_name'] : 'Nil';
			$contractor_address_license_renewal				= !empty($particular_data['contractor_areatype'])?miscellaneous_contractor_update_address_form($particular_data['contractor_areatype'],$serial,$created_by):''; 
			$address_line1_of_contractor_license_renewal	= !empty($particular_data['address_of_contractor'])?$particular_data['address_of_contractor'] : ''; 
			$category_of_contractor_license_renewal			= !empty($particular_data['category_of_contractor'])?'Company' :'Individuals' ;
			$dob_contractor_license_renewal					= !empty($particular_data['dob_contractor'])?$particular_data['dob_contractor'] :'';
			$dob_age_contractor_license_renewal				= $particular_data['age_contractor'];
			$contractor_manager_areatype					= !empty($particular_data['contractor_manager_areatype'])?miscellaneous_contractor_manager_address_form($particular_data['contractor_areatype'],$serial,$created_by):'';
			$name_of_agent_license_renewal 					= !empty($particular_data['name_of_agent'])?$particular_data['name_of_agent']:'Nil';
			$work_site_address_license_renewal				= !empty($particular_data['work_site_areatype'])?miscellaneous_worksite_address($serial,$particularid):'';
			
			$address_of_manager_license_renewal			  	= $particular_data['address_of_manager'];
			
			$contractor_max_labours_license_renewal			= $particular_data['contractor_max_no_of_labours_on_any_day'];
			
			$category_designation_license_renewal	 		= !empty($particular_data['category_designation'])?$particular_data['category_designation']:'Nil'; 
			$unskilled_rate_wages_license_renewal			= !empty($particular_data['unskilled_rate_wages'])?'<del>&#2352;</del>'.$particular_data['unskilled_rate_wages'].' ( Rupees '.convertNumberToWord($particular_data['unskilled_rate_wages']).' only)':'Nil';
			$semiskilled_rate_wages_license_renewal			=  !empty($particular_data['semiskilled_rate_wages'])?'<del>&#2352;</del>'.$particular_data['semiskilled_rate_wages'].' ( Rupees '.convertNumberToWord($particular_data['semiskilled_rate_wages']).' only)':'Nil';
			$skilled_rate_wages_license_renewal				=	!empty($particular_data['skilled_rate_wages'])?'<del>&#2352;</del>'.$particular_data['skilled_rate_wages'].' ( Rupees '.convertNumberToWord($particular_data['semiskilled_rate_wages']).' only)':'Nil';
			$highlyskilled_rate_wages_license_renewal		=	!empty($particular_data['highlyskilled_rate_wages'])?'<del>&#2352;</del>'.$particular_data['highlyskilled_rate_wages'].' ( Rupees '.convertNumberToWord($particular_data['highlyskilled_rate_wages']).' only)':'Nil';
			$hours_work_license_renewal						= 	$particular_data['hours_work'];
			$spred_over_license_renewal						=	'0';
			$overtime_license_renewal						= 	$particular_data['overtime'];
			$overtime_wages_license_renewal					= 	$particular_data['overtime_wages'];
			$annual_leave_no_license_renewal				= 	!empty($particular_data['annual_leave_no'])?$particular_data['annual_leave_no']:'Nil';
			$casual_leave_no_license_renewal				= 	!empty($particular_data['casual_leave_no'])?$particular_data['casual_leave_no']:'Nil';
			$sick_leave_no_license_renewal					= 	!empty($particular_data['sick_leave_no'])?$particular_data['sick_leave_no']:'Nil';
			$other_leave_no_license_renewal					= 	!empty($particular_data['other_leave_no'])?$particular_data['other_leave_no']:'Nil';
			$maternity_leave_no_license_renewal				=	!empty($particular_data['maternity_leave_no'])?$particular_data['maternity_leave_no']:'Nil';
			$weekly_holiday_license_renewal					= 	$particular_data['weekly_holiday'];
			$no_holiday_license_renewal						= 	$particular_data['no_holiday'];
			//$work_order_file_id_license_renewal				=	$result['work_order_file_id'];
			$work_order_file_license_renewal_id				=	$particular_data['work_order_file_id'];
			$frm_v_file_id									=	$particular_data['frm_v_file_id'];
			$residential_file_id							=	$particular_data['residential_file_id'];
			$other_file_id_license_renewal					=	$particular_data['other_file_id'];
			$form_iv_vii_id_license_renewal					=	$result['signed_license_id'];
			
			
			$contractor_revoking_license_renewal			=	!empty($particular_data['details_contractor_revoking'])?date('dS F, Y', strtotime($particular_data['details_contractor_revoking'])):'Nil';
			$holiday_wages_license_renewal					=	0;
			$work_subdivision_license_renewal				= 	$result['worksite_subdivision'];
			$is_coparative_license_renewal					=   $particular_data['is_coparative'];
			$amount_of_licence_renewal						=	$particular_data['amount_of_licence']; 
			$amount_of_security_deposit						=  	$particular_data['amount_of_security_deposit'];
			$due_security_fees								=	0.00;  
			$earned_leave_no_license_renewal				=	'Nil';
			$special_benifites_license_renewal				=	'Nil';
			$miscellaneous_provisions_license_renewal		=	'Nil';
			
			$contractor_convicted_license_renewal			=	!empty($particular_data['details_contractor_convicted'])?date('dS F, Y', strtotime($particular_data['details_contractor_convicted'])):'Nil';
			$generated_form_iv_vii							=	l(t('Generated FORM-IV'),'alc-view-formiv/'.encryption_decryption_fun('encrypt', $serial).'/'.encryption_decryption_fun('encrypt', $particularid).'/'.encryption_decryption_fun('encrypt', $created_by),array('html' => TRUE,'attributes'=> array('target'=>'_blank')));
			//l(t('Generated FORM - VII'),'pdf-form-vii/'.encryption_decryption_fun('encrypt', $license_renewal_ammendment_id).'/'.encryption_decryption_fun('encrypt', $created_by).'/Form-VII'.time(), array('html' => TRUE,'attributes'=> array('target'=>'_blank')));
			$payment_details_license_renewal_amand			= 	payment_details($serial,$created_by,'12');	
	
			
		}
	}
	if($is_flag == 'A'){
		 $result = 	contract_labour_amendment_log($ammendid);
		
		  if(!empty($result)){
		
			$log_amend_id									= $result['id'];
			$father_contarctor_name_license_renewal 		= !empty($result['father_contarctor_name'])?$result['father_contarctor_name'] : 'Nil';
			$contractor_address_license_renewal				= !empty($result['contractor_areatype'])?log_ammendment_address($log_amend_id,'ammendment_log_contrcator_address'):'Nil'; 
			$address_line1_of_contractor_license_renewal	= !empty($result['address_of_contractor'])?$result['address_of_contractor'] : ''; 
			$category_of_contractor_license_renewal			= !empty($result['category_of_contractor'])?'Company' :'Individuals' ;
			$dob_contractor_license_renewal					= !empty($result['dob_contractor'])?$result['dob_contractor'] :'';
			$dob_age_contractor_license_renewal				= $result['age_contractor'];
			$contractor_manager_areatype					= !empty($result['contractor_manager_areatype'])?log_ammendment_address($log_amend_id,'ammendment_manager_address_log'):'Nil';
			
			$work_site_address_license_renewal				= !empty($result['work_site_areatype'])?log_ammendment_address($log_amend_id,'ammendment_worksite_address_log'):'';
			$name_of_agent_license_renewal					= $result['name_of_agent'];
			$address_of_manager_license_renewal			  	= $result['address_of_manager'].'<br/>'.$contractor_manager_areatype; 
			
			$contractor_max_labours_license_renewal			= $result['max_of_contract_labour'];
			
			$category_designation_license_renewal	 		= $result['category_designation']; 
			$unskilled_rate_wages_license_renewal			= !empty($result['unskilled_rate_wages'])?'<del>&#2352;</del>'.$result['unskilled_rate_wages'].' ( Rupees '.convertNumberToWord($result['unskilled_rate_wages']).' only)':'Nil';
			$semiskilled_rate_wages_license_renewal			=  !empty($result['semiskilled_rate_wages'])?'<del>&#2352;</del>'.$result['semiskilled_rate_wages'].' ( Rupees '.convertNumberToWord($result['semiskilled_rate_wages']).' only)':'Nil';
			$skilled_rate_wages_license_renewal				=	!empty($result['skilled_rate_wages'])?'<del>&#2352;</del>'.$result['skilled_rate_wages'].' ( Rupees '.convertNumberToWord($result['semiskilled_rate_wages']).' only)':'Nil';
			$highlyskilled_rate_wages_license_renewal		=	!empty($result['highlyskilled_rate_wages'])?'<del>&#2352;</del>'.$result['highlyskilled_rate_wages'].' ( Rupees '.convertNumberToWord($result['highlyskilled_rate_wages']).' only)':'Nil';
			$hours_work_license_renewal						= 	$result['hours_work'];
			$spred_over_license_renewal						=	'0';
			$overtime_license_renewal						= 	$result['overtime'];
			$overtime_wages_license_renewal					= 	$result['overtime_wages'];
			$annual_leave_no_license_renewal				= 	!empty($result['annual_leave_no'])?$result['annual_leave_no']:'Nil';
			$casual_leave_no_license_renewal				= 	!empty($result['casual_leave_no'])?$result['casual_leave_no']:'Nil';
			$sick_leave_no_license_renewal					= 	!empty($result['sick_leave_no'])?$result['sick_leave_no']:'Nil';
			$other_leave_no_license_renewal					= 	!empty($result['other_leave_no'])?$result['other_leave_no']:'Nil';
			$maternity_leave_no_license_renewal				=	!empty($result['maternity_leave_no'])?$result['maternity_leave_no']:'Nil';
			$weekly_holiday_license_renewal					= 	$result['weekly_holiday'];
			$no_holiday_license_renewal						= 	$result['no_holiday'];
			//$work_order_file_id_license_renewal				=	$result['work_order_file_id'];
			$work_order_file_license_renewal_id				=	$result['work_order_file_id'];
			$frm_v_file_id									=	$result['frm_v_file_id'];
			$residential_file_id							=	$result['residential_file_id'];
			$other_file_id_license_renewal					=	$result['other_file_id'];
			$form_iv_vii_id_license_renewal					=	$result['signed_license_id'];
			
			$contractor_convicted_license_renewal			=	!empty($result['details_contractor_convicted'])?date('dS F, Y', strtotime($result['details_contractor_convicted'])):'Nil';
			$contractor_revoking_license_renewal			=	!empty($result['details_contractor_revoking'])?date('dS F, Y', strtotime($result['details_contractor_revoking'])):'Nil';
			$holiday_wages_license_renewal					=	0;
			$work_subdivision_license_renewal				= 	$result['worksite_subdivision'];
			$amount_of_licence_renewal						= 	$result['applicable_amendment_fee'];
			$amount_of_security_deposit						= 	$result['applicable_security_fess'];
			$special_benifites_license_renewal				=	!empty($result['special_benifites'])?$result['special_benifites']:'Nil';
			$state_insurance_license_renewal				= 	!empty($result['state_insurance'])?($result['state_insurance']):'Nil';
			$miscellaneous_provisions_license_renewal		=	!empty($result['miscellaneous_provisions'])?$result['miscellaneous_provisions']:'Nil';
			
			$form_iv_vii_id_previous						=  	$result['upload_singed_from'];
			$certificate_previous							=	$result['ammendment_certificate_id'];
			$application_id									=	$result['reference_no'];
			$contractor_user_id								=	$result['created_by'];
			
			$payment_details_license_renewal_amand			= 	payment_details($application_id,$contractor_user_id,'14');	
		}
	}
	  
	  $amendment_application_form	= fetch_uplaoded_file_details($upload_singed_from_id);
	if(!empty($amendment_application_form)){ 
			$url									= explode('//',$amendment_application_form['uri']);
   			$amendment_application_file_url			= $url[1];
			//$documents_worksite_file_file			= $supportive_documents_worksite_file['filename'];
	}
	 $rest_holiday_license_renewal		= substr($no_holiday_license_renewal, 0, -1);
	$no_holiday_license_renewal_list	= explode(',', $rest_holiday_license_renewal);
	$name_holidaylicense_renewal		= "";
	$total_holidaylicense_renewal		= 0;
	
	
	
    foreach($no_holiday_license_renewal_list as $row){
	  $total_holidaylicense_renewal=$total_holidaylicense_renewal+1;
		if($row=='1'){
			$name_holidaylicense_renewal	.=	'Sunday'.',';
		}
		if($row=='2'){
			$name_holidaylicense_renewal	.=	'Monday'.',';
		}
		if($row=='3'){
			$name_holidaylicense_renewal	.=	'Tuesday'.',';
		}
		if($row=='4'){
			$name_holidaylicense_renewal	.=	'Wednesday'.',';
		}
		if($row=='5'){
			$name_holidaylicense_renewal	.=	'Thursday'.',';
		}
		if($row=='6'){
			$name_holidaylicense_renewal	.=	'Friday'.',';
		}
		if($row=='7'){
			$name_holidaylicense_renewal	.=	'Saturday'.',';
		}
		
	}
	$name_holidayslicense_renewal = substr($name_holidaylicense_renewal, 0, -1);
	
	if(empty($name_holidayslicense_renewal))
		$name_holidays_license_renewal		=	'NIL';
	 
	 if(!empty($dob_contractor_license_renewal)){
		$dob_contractor_previous = date('dS M, Y',strtotime($dob_contractor_license_renewal));
	}else{
		$dob_contractor_previous = 'Nil';		
	}
	if(!empty($dob_contractor_license_renewal)){
		$age_contractor_previous = $dob_age_contractor_license_renewal.'Year(s)';
	}else{
		$age_contractor_previous = 'Nil';
	}
	
	if($category_of_contractor_license_renewal=='1' && !empty($category_of_contractor_license_renewal)){
		    $type_pervious = 'Company';
			$title_pervious = 'Date of commencement of business'.$dob_contractor_previous;
			$dob_contractor_previous='';
	}else{
		    $type_pervious = 'Individuals';
			$title_pervious = 'Date of Birth and age (incase of individuals).'.$dob_contractor_previous;
			$dob_contractor_previous= 'Age: '.$age_contractor_previous;
	}
	if($is_coparative_license_renewal == 1)
	      $coparative_license_renewal = 'Yes';
    if($is_coparative_license_renewal != 1)
	 $coparative_license_renewal = 'No';
	 	
	if(!empty($last_remark_activity['not_amendment_check_field'])){
		$not_amendment_check_field1 = substr($last_remark_activity['not_amendment_check_field'], 0, -1);
		$not_amendment_check_field = explode(',', $not_amendment_check_field1);
	
		foreach($not_amendment_check_field as $row){
			switch($row){//echo $row;
				case ( $row == "name_of_contractor"||$row =="contractor_dist"||$row =="father_name_of_contarctor"||$row =="category_of_contractor"||$row =="date_of_birth_age"):
						$personal_info  = 'NA';
						break;
				case  "worksite_address":
						$worksite_info  = 'NA';
						break;
				case  "contractor_max_no_of_labours_on_any_day":
						$max_no_of_labours  = 'NA';
						break;
				case  "address_of_manager":
						$address_of_manager  = 'NA';
						break;
				case  "category":
						$category  = 'NA';
						break;
				case ($row == "unskilled_rate_wages"||$row =="semiskilled_rate_wages"||$row =="skilled_rate_wages"||$row =="highlyskilled_rate_wages"):
						$wages_info  = 'NA';
						break;
				case ( $row == "hour_details"||$row =="spred_over"||$row =="Overtime"||$row =="overtime_wages"):
						$hour_details_info  = 'NA';
						break;
				case ($row == "annual_leave"||$row =="casual_leave"||$row =="eraned_leave"||$row =="sick_leave"||$row =="maternity_leave" || $row=="other_leave"||$row =="weekly_holiday"):
						$other_condition_info  = 'NA';
						break;
				case  "revoking":
						$revoking_info  = 'NA';
						break;
				case "offence" :
						$offence_info  = 'NA';
						break;
				case "miscellaneous" :
						$miscellaneous_info  = 'NA';
						break;
				case "special_benifites" :
						$special_benifites_info  = 'NA';
						break;
				 case "state_insurance" :
						$state_insurance_info  = 'NA';
						break;
				case "pervious_license" :
						$pervious_license_info  = 'NA';
						break;
			
			}
		}
	}
	if(!empty($last_remark_activity['remark_field_title'])){
		$remark_check_field = explode(',', $last_remark_activity['remark_field_title']);
		foreach($remark_check_field as $row){
			switch($row){//echo $row;
				case  "name_of_contractor":
						 $name_of_contractor_ok  	=	"checked='checked'";
						 break;
				case  "contractor_dist":
						 $contractor_dist_ok  		=  "checked='checked'";
						break;
				case  "father_name_of_contarctor":
						 $father_name_ok  			= "checked='checked'";
						break;
				case   "category_of_contractor":
						 $category_of_contracto_ok  = "checked='checked'";
						break;
										   
				case  "worksite_address":
						$worksite_address_ok  = "checked='checked'";
						break;
				case  "contractor_max_no_of_labours_on_any_day":
						$max_no_of_labours_ok  =  "checked='checked'";
						break;
				case  "co_oparative":
						$co_oparative_ok  =  "checked='checked'";
						break;
				case  "change_reason":
						$change_reason_ok  =  "checked='checked'";
						break;
				case  "address_of_manager":
						$address_of_manager_ok  = "checked='checked'";
						break;
				case  "category":
						$category_ok  = "checked='checked'";
						break;
				case "unskilled_rate_wages":
						 $unskilled_rate_wages_ok  = "checked='checked'";
						break;
				case  "semiskilled_rate_wages":
						$semiskilled_rate_wages_ok  = "checked='checked'";
						break;
				case 	"skilled_rate_wages":
						 $skilled_rate_wages_ok  = "checked='checked'";
						break;
				case   "highlyskilled_rate_wages" :
						$highlyskilled_rate_wages_ok  = "checked='checked'";
						break;
				case "hour_details":
						$hour_details_ok  = "checked='checked'";
						break;
				case "spred_over":
						$spred_over_ok  = "checked='checked'";
						break;
				case "Overtime":
						$overtime_ok  = "checked='checked'";
						break;
				case "overtime_wages":
						$overtime_wages_ok  = "checked='checked'";
						break;
				case  "annual_leave":
					  $annual_leave_ok = "checked='checked'";
						break;
				case "casual_leave":
						$casual_leave_ok  = "checked='checked'";
						break;
				case "eraned_leave":
						$eraned_leave_ok = "checked='checked'";
						break;
				case "sick_leave":
					   $sick_leave_ok  = "checked='checked'";
						break;
				case "other_leave" :
					   $other_leave_ok  = "checked='checked'";
						break;
				case "maternity_leave" :
					   $maternity_leave_ok  = "checked='checked'";
					  break;
				case "weekly_holiday" :
					   $weekly_holiday_ok  = "checked='checked'";
					  break;
				case "special_benifites" :
					   $special_benifites_ok = "checked='checked'";
					  break;
			 case "state_insurance" :
					   $state_insurance_ok = "checked='checked'";
					  break;
			case "miscellaneous" :
					   $miscellaneous_ok  = "checked='checked'";
					  break;
			case "miscellaneous" :
					   $miscellaneous_ok  = "checked='checked'";
					  break;
			case "offence" :
					   $offence_ok  = "checked='checked'";
					  break;
			case "revoking" :
					   $revoking_ok  = "checked='checked'";
					  break;
			case "pervious_license" :
					   $pervious_license_ok  = "checked='checked'";
					  break;	
			case "other_doc_id" :
					   $other_doc_id_ok  = "checked='checked'";
					  break;	
							
			case "supportive_documents" :
					   $supportive_documents_ok  = "checked='checked'";
					  break;
			case "work_order_id":
					$work_order_ok  = "checked='checked'";
					  break;
			case "application_id":
				$application_id_ok  = "checked='checked'";
					  break;
					
				
			}
		}
	}
	if(!empty($last_remark_activity['amendment_check_field'])){
		$amendment_check_field = explode(',', $last_remark_activity['amendment_check_field']);
			foreach($amendment_check_field as $row){
				switch($row){//echo $row;
			case  "name_of_contractor":
					 $name_of_contractor_chk  = '<input type="checkbox" id="name_of_contractor" class="yn_amend_verified"'.$name_of_contractor_ok.' />';
					break;
			case  "contractor_dist":
					 $contractor_dist_chk  = '<input type="checkbox" id="contractor_dist" class="yn_amend_verified"'.$contractor_dist_ok.' />';
					break;
			case  "father_name_of_contarctor":
					 $father_name_chk  = '<input type="checkbox" id="father_name_of_contarctor" class="yn_amend_verified"'.$father_name_ok.' />';
					break;
			case   "category_of_contractor":
					 $category_of_contracto_chk  = '<input type="checkbox" id="category_of_contractor" class="yn_amend_verified"'.$category_of_contracto_ok.' />';
					break;
									   
			case  "worksite_address":
					$worksite_address_chk  = '<input type="checkbox" id="worksite_address" class="yn_amend_verified"'.$worksite_address_ok.' />';
					break;
			case  "contractor_max_no_of_labours_on_any_day":
					$max_no_of_labours_chk  =  '<input type="checkbox" id="contractor_max_no_of_labours_on_any_day" class="yn_amend_verified"'.$max_no_of_labours_ok.' />';
					break;
				case  "co_oparative":
					$co_oparative_chk  =  '<input type="checkbox" id="co_oparative" class="yn_amend_verified"'.$co_oparative_ok.'/>';
					break;
			
			case  "address_of_manager":
					$address_of_manager_chk  = '<input type="checkbox" id="address_of_manager" class="yn_amend_verified"'.$address_of_manager_ok.' />';
					break;
			case  "category":
					$category_chk  = '<input type="checkbox" id="category" class="yn_amend_verified"'.$category_ok.' />';
					break;
			case "unskilled_rate_wages":
					 $unskilled_rate_wages_chk  = '<input type="checkbox" id="unskilled_rate_wages" class="yn_amend_verified"'.$unskilled_rate_wages_ok.' />';
					break;
			case  "semiskilled_rate_wages":
					$semiskilled_rate_wages_chk = '<input type="checkbox" id="semiskilled_rate_wages" class="yn_amend_verified"'.$semiskilled_rate_wages_ok.' />';
					break;
			case 	"skilled_rate_wages":
					 $skilled_rate_wages_chk = '<input type="checkbox" id="skilled_rate_wages" class="yn_amend_verified"   '.$skilled_rate_wages_ok.' />';
					break;
			case   "highlyskilled_rate_wages" :
					$highlyskilled_rate_wages_chk  = '<input type="checkbox" id="highlyskilled_rate_wages" class="yn_amend_verified"   '.$highlyskilled_rate_wages_ok.' />';
					break;
			case "hour_details":
					$hour_details_chk  = '<input type="checkbox" id="hour_details" class="yn_amend_verified"   '.$hour_details_ok.' />';
					break;
			case "spred_over":
					$spred_over_chk  = '<input type="checkbox" id="spred_over" class="yn_amend_verified"   '.$spred_over_ok.' />';
					break;
			case "overtime":
					$overtime_chk  = '<input type="checkbox" id="Overtime" class="yn_amend_verified"   '.$overtime_ok.' />';
					break;
			case "overtime_wages":
					$overtime_wages_chk  = '<input type="checkbox" id="overtime_wages" class="yn_amend_verified"   '.$overtime_wages_ok.' />';
					break;
			case  "annual_leave":
				  $annual_leave_chk  = '<input type="checkbox" id="annual_leave" class="yn_amend_verified"   '.$annual_leave_ok.' />';
					break;
			case "casual_leave":
					$casual_leave_chk  = '<input type="checkbox" id="casual_leave" class="yn_amend_verified"   '.$casual_leave_ok.' />';
					break;
			case "eraned_leave":
					$eraned_leave_chk  = '<input type="checkbox" id="eraned_leave" class="yn_amend_verified"   '.$eraned_leave_ok.' />';
					break;
			case "sick_leave":
				   $sick_leave_chk  = '<input type="checkbox" id="sick_leave" class="yn_amend_verified"   '.$sick_leave_ok.' />';
					break;				
			case "other_leave" :
				   $other_leave_chk  = '<input type="checkbox" id="other_leave" class="yn_amend_verified"   '.$other_leave_ok.' />';
					break;
			case "maternity_leave" :
				   $maternity_leave_chk  = '<input type="checkbox" id="maternity_leave" class="yn_amend_verified"   '.$maternity_leave_ok.' />';
				  break;
			case "weekly_holiday" :
				   $weekly_holiday_chk  = '<input type="checkbox" id="weekly_holiday" class="yn_amend_verified"   '.$weekly_holiday_ok.' />';
				  break;
			case "special_benifites" :
				   $special_benifites_chk  = '<input type="checkbox" id="special_benifites" class="yn_amend_verified"   '.$special_benifites_ok.' />';
				  break;
			case "state_insurance" :
				   $state_insurance_chk  = '<input type="checkbox" id="state_insurance" class="yn_amend_verified"   '.$state_insurance_ok.' />';
				  break;
			case "miscellaneous" :
				   $miscellaneous_chk  = '<input type="checkbox" id="miscellaneous" class="yn_amend_verified"   '.$miscellaneous_ok.' />';
				   break;
			case "offence" :
				   $offence_chk  = '<input type="checkbox" id="offence" class="yn_amend_verified"   '.$offence_ok.' />';
				   break;				
			case "revoking" :
				   $revoking_chk  = '<input type="checkbox" id="revoking" class="yn_amend_verified"   '.$revoking_ok.' />';
				  break;	
			case "pervious_license" :
				   $pervious_license_chk  = '<input type="checkbox" id="pervious_license" class="yn_amend_verified"   '.$pervious_license_ok.' />';
				  break;
			}
		}
	}
	
	$uploaded_workorder_file_pre = fetch_uplaoded_file_details($work_order_file_license_renewal_id);
	if(!empty($uploaded_workorder_file_pre)){ 
			$url			 		= 	explode('//',$uploaded_workorder_file_pre['uri']);
			$work_order_pre_url		= 	$url[1];
			$workorder_file_pre		=	$uploaded_workorder_file_pre['filename'];
	}
	if(!empty($other_file_id_license_renewal)){
		$other_file_pre = fetch_uplaoded_file_details($other_file_id_license_renewal);
			if(!empty($other_file_pre)){ 
					$url			 		= 	explode('//',$other_file_pre['uri']);
					$other_file_pre_url		= 	$url[1];
					$other_file_pre_name	=	$other_file_pre['filename'];
			}
	}
	
	
		$previous_form_iv_vii = fetch_uplaoded_file_details($form_iv_vii_id_previous);
	
		if(!empty($previous_form_iv_vii)){ 
				$url								= explode('//',$previous_form_iv_vii['uri']);
				$previous_form_iv_vii_url			= $url[1];
				//$previous_form_iv_vii_file			= $form_iv_vii_id_previous['filename'];
		}
	
		$previous_certificates = fetch_uplaoded_file_details($certificate_previous);
		if(!empty($previous_certificates)){ 
				$url										= explode('//',$previous_certificates['uri']);
				$previous_previous_certificates_url			= $url[1];
				//$previous_previous_certificates_file		= $previous_certificates['filename'];
		}
		$result =  particular_data($serial_no,$created_by);
		if($is_flag == 'R'){	
			//serial
			 $renewal_data 					= 	renewal_data_last($license_renewal_ammendment_id); 
			 $amount_of_licence_renewal		=	$renewal_data['license_reneal_fees'];  
			 $amount_of_security_deposit	=	$renewal_data['renewal_security_new'];  
			 $due_security_fees				=	$renewal_data['due_security_fees'];   
		}
		if($is_flag == 'L'){
		}
		if($is_flag == 'A'){
			$due_security_fees =	0.00; 
		}
				
		$output	.= '<div class="box box-primary box-solid">
					<div class="box-header with-border"><i class="ion ion-clipboard"></i> Contractor Information From Principal Employer</div>
					<div class="box-body"><div class="feedback-scroll">
						<table cellpadding="0" cellspacing="0" border="0" width="100%" class="table table-bordered table-responsive">
							<tr>
								<th width="45%">Parameters</th>
								<th>Inputs</th>								
								<th width="10%" class="center">Status</th>
							</tr>
							<tr>
								<td>Name &amp; address of the establishment</td>		
								<td>'.$establishment_address.'</td>
								<td><i class="fa fa-check" style="color:green;"></i> Verified</td>								
							</tr>
							<tr>
								<td>Type of Business, trade, industry, manufacture or occupation which is carried on in the establishment</td>
								<td>'.$nature_of_work_pe.'</td>
								<td><i class="fa fa-check" style="color:green;"></i> Verified</td>									
							</tr>  
							<tr>
								<td>Number and date of Certificate</td>
								<td>'.$establish_details_reg.'</td>
								<td><i class="fa fa-check" style="color:green;"></i> Verified</td>	
							</tr>
							<tr>
								<td>Name & Address of the Principal Employer</td>
								<td>'.$principal_details.'</td>
								<td><i class="fa fa-check" style="color:green;"></i> Verified</td>	
							</tr>
							<tr>
								<td>Nature of work in which Contract Labour is employed or is to be employed in the establishment</td>
								<td>'.$nature_work.'</td>
								<td><i class="fa fa-check" style="color:green;"></i> Verified</td>	
							</tr>
							<tr>
								<td>Address of the contractor(provided by principal employer)</td>
								<td>'.$pe_contractor_address.'</td>
								<td><i class="fa fa-check" style="color:green;"></i> Verified</td>								
							</tr>
							<tr>
								<td>Maximum number of Contract Labour proposed to he employed in the establishment on any date</td>
								<td>'.$estimated_date.'</td>
								<td><span style="color:#006600;">&nbsp;Verified</span></td>
							</tr>
							<tr>
								<td>Duration of the proposed contract work(give particulars of proposed date of ending)</td>	
								<td>'.$contractor_max_no_of_labours_pe.'('.convertNumberToWord($contractor_max_no_of_labours_pe).')</td>
								<td><i class="fa fa-check" style="color:green;"></i> Verified</td>	
							</tr>
						</table>					
						<div class="pull-right">
							<span class="popup" id='.$details['id'].' title="click here to view & download certificate"><i class="fa fa-certificate fa-lg"></i></a></span>&nbsp;
							<div id=dialog_'.$details['id'].' title="Registration Certificate of Principal Employer" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$certificates_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>
							<i class="fa fa-info-circle"></i><a target="_blank" href=" '.$GLOBALS['base_url'].'/alc-visible-applications/'.encryption_decryption_fun('encrypt', trim($details['id'])).'/'.encryption_decryption_fun('encrypt', trim($details['user_id'])).'"> View More information</a>			
							
						</div>						
					</div>				
				</div></div>
				<div class="box box-primary box-solid">
					<div class="box-header with-border"><i class="ion ion-clipboard"></i> Application Details</div>
					<div class="box-body"><div class="feedback-scroll">
						<table cellpadding="0" cellspacing="0" border="0" width="100%" class="table table-bordered table-responsive">
							<tr>
								<td width="30%"><b>Parameters</b></td>
								<td width="30%"><b>Current Application</b></td>
								<td colspan="2" width="30%"><b>Previous Application</b></td>
							</tr>
							<tr>
								<td colspan="4" style="font-weight:700; text-transform:uppercase; font-size:15px;"><i class="ion ion-clipboard"></i>  Name &amp; address of the contractor (Official Information)</td>
							</tr>
							<tr>
								<td width="30%">Name of the contractor</td>
								<td width="30%">'.$name_of_contractor.'</td>
								<td width="30%">'.$name_of_contractor_pervious.'</td>
								<td><i class="fa fa-check" style="color:green;"></i> Verified</td>	 						
							</tr>
							<tr>
								<td>Father&#13;s name of the contractor (including his father&#13;s name incase of individuals) </td>
								<td>'.$father_contarctor_name.'</td>
								<td>'.$father_contarctor_name_license_renewal.'</td>
								<td>'.$father_name_chk.' <span style="color:#006600;">&nbsp;'.$personal_info.'</span></td> 
							</tr> 
						    <tr>
								<td>Address of the contractor</td>
								<td>'.$ammendment_address_contractor.'</td>
								<td>'.$contractor_address_license_renewal.'</td>
								<td>'.$contractor_dist_chk.'<span style="color:#006600;">&nbsp;'.$personal_info.'</span></td>   
						    </tr>
						    <tr>
								<td>Category of Contractor <font color="red">**</font></td>
								<td>'.$type.'</br>'.$title.'</br>'.$age_contractor.'</td>
								<td>'.$type_pervious.'</br>'.$title_pervious.'</br>'.$dob_contractor_previous.'</td>
								<td>'.$category_of_contracto_chk.'<span style="color:#006600;">&nbsp;'.$personal_info.'</span></td>						
						 	</tr>
							<tr>
								<td colspan="4" style="font-weight:700; text-transform:uppercase; font-size:15px;"><i class="ion ion-clipboard"></i>  Worksite Address/ Number of Worker &amp; Fees Details</td>
							</tr>						 	
						  	<tr>
								<td>Work Site Address Details <font color="red"> **</font></td>
								<td>'.$ammendment_address_worksite.'</td>
								<td>'.$work_site_address_license_renewal.'</td>
								<td>'.$worksite_address_chk.'<span style="color:#006600;">'.$worksite_info.'</span></td>
							</tr>
						  	<tr>
								<td>Maximum number of Contract Labour proposed to he employed in the establishment on any day <font color="red">**</font></td>
								<td>'.$max_of_contract_labour_ammend.' ('.convertNumberToWord($max_of_contract_labour_ammend).')</td>
								<td>'.$contractor_max_labours_license_renewal.'('.convertNumberToWord($contractor_max_labours_license_renewal).')</td>
								<td>'.$max_no_of_labours_chk.'<span style="color:#006600;">'.$max_no_of_labours.'</span></td>
							</tr>
						
						<tr>
							<td>Co-oparative Society <font color="red">**</font></td>
							<td>'.$coparative.'</td>
							<td>'.$coparative_license_renewal.'</td>
							<td>'.$co_oparative_chk.'<span style="color:#006600;">'.$max_no_of_labours.'</span></td>						
						</tr>
						<tr>
							<td>Applicable Ammended Fees<font color ="red"> **</font></td>							
						    <td> <del>&#2352;</del>'.$applicable_amendment_fee.'</br>'.$alert.'</td>
						    <td><del>&#2352;</del>'.$amount_of_licence_renewal.'</td>
						    <td><i class="fa fa-check" style="color:green;"></i> Verified</td>
						</tr>
						<tr>
							<td>Payble Ammendment Fees <font color ="red"> **</font></td>
							<td><del>&#2352;</del>'.$payable_amendment_fees.'</td>
							<td><strong><del>&#2352;</del>'.$previous_highest_deposit_fees.'</strong> <i>Note:Highest Fees paid during license/ subsequent amendments</i></td>
							<td><i class="fa fa-check" style="color:green;"></i> Verified</td>   
						</tr>
						<tr>
							<td>Applicable Security Fees <font color ="red"> **</font></td>							
							<td><del>&#2352;</del>'.$applicable_security_fess.'</br>'.$alert.'</td>
							<td><del>&#2352;</del>'.$amount_of_security_deposit.'</td>
							<td><i class="fa fa-check" style="color:green;"></i> Verified</td>
						</tr>
						<tr>
							<td>Payable Ammended Security Fees <font color ="red"> **</font></td>	
							<td><del>&#2352;</del>'.$payable_amendment_security_fees.'</br>'.$alert.'</td>
							<td><strong><del>&#2352;</del>'.$previous_highest_security.'</strong> <i>Note:Highest Security Fees paid during license/ subsequent amendments/ subsequent amendments</i></td>
							<td><i class="fa fa-check" style="color:green;"></i> Verified</td>
					   </tr>  
					   <tr>
						    <td colspan="4" style="font-weight:700; text-transform:uppercase; font-size:15px;"><i class="ion ion-clipboard"></i>  Particular of Contract Labour</td>
					   </tr>

						<tr>
							<td>Name & address of the agent or Manager of Contractor at the work site<font color="red"> **</font></td>
							<td> '.$name_of_agent.'</br>'.$address_of_contractor_line1.$ammendment_address_contractor_manager.'</td>
							<td>'.$name_of_agent_license_renewal.'</br>'.$address_of_manager_license_renewal.'</br>'.$contractor_manager_areatype.'</td>
							<td>'.$address_of_manager_chk.'<span style="color:#006600;">'.$address_of_manager.'</span></td>   
						</tr>
						
						<tr>
							<td>Category/designation/ nomenclature of the contractor labour, namely, fitter, welder, carpenter, mazdor etc. <font color="red"> **</font></td>
							<td>'.str_replace(",", ", ", $category_designation).'</td>
							<td>'.str_replace(",", ", ", $category_designation_license_renewal).'</td>
							<td>'.$category_chk.'<span style="color:#006600;">'.$category.'</span> </td>   
						</tr>
						
						<tr>
							<td colspan="4" style="font-weight:700; text-transform:uppercase; font-size:15px;"><i class="ion ion-clipboard"></i>  Rate of Wages, DA and other cash benefits paid/ to be paid to each category(i.e (a):Unskilled (b)Semi-Skilled (c)Skilled (d)Highly-Skilled etc.)of contract labour</td>   
						</tr>
		
						<tr>
							<td>Rate of wages,DA and Other cash benefits paid/ to be paid to Unskilled of contract labour:</td>
							<td>'.$unskilled_rate_wages.'</td>
							<td>'.$unskilled_rate_wages_license_renewal.'</td>
							<td>'.$unskilled_rate_wages_chk.'<span style="color:#006600;">'.$wages_info.'</span> </td>
						</tr>
						<tr>
							<td>Rate of wages,DA and Other cash benefits paid/ to be paid to Semi-skilled of contract labour:</td>
							<td>'.$semiskilled_rate_wages.'</td>
							<td>'.$semiskilled_rate_wages_license_renewal.'</td>
							<td>'.$semiskilled_rate_wages_chk.'<span style="color:#006600;">'.$wages_info.'</span> </td>
						</tr>
						<tr>
							<td>Rate of Wages,DA and Other cash benefits paid/ to be paid to Skilled of contract labour:</td>
							<td>'.$skilled_rate_wages.'</td>
							<td>'.$skilled_rate_wages_license_renewal.'</td>
							<td>'.$skilled_rate_wages_chk.'<span style="color:#006600;">'.$wages_info.'</span> </td>
						<tr>
							<td>Rate of Wages,DA and Other cash benefits paid/ to be paid to Highly-skilled of contract labour:</td>
							<td>'.$highlyskilled_rate_wages.'</td>
							<td>'.$highlyskilled_rate_wages_license_renewal.'</td>
							<td>'.$highlyskilled_rate_wages_chk.'<span style="color:#006600;">'.$wages_info.'</span> </td>
						<tr>
  
    						<td colspan="4" style="font-weight:700; text-transform:uppercase; font-size:15px;"><i class="ion ion-clipboard"></i>  Hours of Work, Overtime and Overtime Wages</td>
   						<tr>
							<td>Hours of Work</td>
							<td>'.$hours_work.' hr(s)</td>
							<td>'.$hours_work_license_renewal.' hr(s)</td>
							
							<td>'.$hour_details_chk.'<span style="color:#006600;">'.$hour_details_info.'</span> </td>
						</tr>
						<tr>
							<td>Spred over time  </td>
							<td>'.$spred_over.' hr(s)</td>
							<td>'.$spred_over_license_renewal.'</td>
							<td>'.$spred_over_chk.'<span style="color:#006600;">'.$hour_details_info.'</span> </td>
						</tr>
						<tr>
							<td>Overtime</td>
							<td>'.$overtime.'(per hour)</td>
							<td>'.$overtime_license_renewal.'</td>
							<td>'.$overtime_chk.'<span style="color:#006600;">'.$hour_details_info.'</span> </td>
						</tr>
						<tr>
							<td>Overtime Wages</td>
							<td><del>&#2352;</del> '.$overtime_wages.'(per hour)</td>
							<td>'.$overtime_wages_license_renewal.'</td>
							<td>'.$overtime_wages_chk.'<span style="color:#006600;">'.$hour_details_info.'</span> </td>
						</tr>
						<tr>
							<td colspan="4" style="font-weight:700; text-transform:uppercase; font-size:15px;"><i class="ion ion-clipboard"></i>  Other Condition of service like leave (annual leave,casual leave,sick leave,maternity leave etc.)Holidays etc.of the contract labour</td>
						</tr>
						<tr>
						<td>Number of annual leave</td>
							<td>'.$annual_leave_no.'</td>
							<td>'.$annual_leave_no_license_renewal.'</td>
							<td>'.$annual_leave_chk.'<span style="color:#006600;">'.$other_condition_info.'</span> </td>
						</tr>
						<tr>
							<td>Number of casual leave</td>
							<td>'.$casual_leave_no.'</td>
							<td>'.$casual_leave_no_license_renewal.'</td>
							<td>'.$casual_leave_chk.'<span style="color:#006600;">'.$other_condition_info.'</span> </td>

						</tr>
						<tr>
						<td>Number of sick leave</td>
						<td>'.$sick_leave_no.'</td>
						<td>'.$sick_leave_no_license_renewal.'</td>
						<td>'.$sick_leave_chk.'<span style="color:#006600;">'.$other_condition_info.'</span> </td>

						</tr>
						<tr>
							<td>Number of earned leave</td>
							<td>'.$earned_leave_no.'</td>
							<td>'.$earned_leave_no_license_renewal.'</td>
							<td>'.$eraned_leave_chk.'<span style="color:#006600;">'.$other_condition_info.'</span> </td>

						</tr>
							<td>Number of maternity leave</td>
							<td>'.$maternity_leave_no.'</td>
							<td>'.$maternity_leave_no_license_renewal.'</td>
							<td>'.$maternity_leave_chk.'<span style="color:#006600;">'.$other_condition_info.'</span> </td>

						</tr>
						<tr>
							<td>Number of other leave</td>
							<td>'.$other_leave_no.'</td>
							<td>'.$other_leave_no_license_renewal.'</td>
							<td>'.$other_leave_chk.'<span style="color:#006600;">'.$other_condition_info.'</span> </td>
						</tr>
						<tr>
							<td>Holiday(s) and Holiday wages</td>
							<td>'.$total_holiday.' day(s) ('.$name_holidays.')</td>
							<td>'.$total_holidaylicense_renewal.' day(s) ('.$name_holidaylicense_renewal.')</td>
							<td>'.$weekly_holiday_chk.'<span style="color:#006600;">'.$other_condition_info.'</span> </td>
						</tr>
						<tr>
							<td>Special benifites provied, if any</td>
							<td>'.$special_benifites.' </td>
							<td>'.$special_benifites_license_renewal.' </td>
							<td>'.$special_benifites_chk.'<span style="color:#006600;">'.$special_benifites_info.'</span> </td>
						</tr>
						<tr>
							<td>Contribution made under the Employees State Insurance Act,1984:</td>
							<td>'.$state_insurance.'</td>
							<td>'.$state_insurance_license_renewal.'</td>
							<td>'.$state_insurance_chk.'<span style="color:#006600;">'.$state_insurance_info.'</span> </td>
						</tr>
						<tr>
							<td>Contribution made under the Employees Provident Fund and Miscellaneous Provision Act,1952 :</td>
							<td>'.$miscellaneous_provisions.'</td>
							<td>'.$miscellaneous_provisions_license_renewal.'</td>
							<td>'.$miscellaneous_chk.'<span style="color:#006600;">'.$miscellaneous_info.'</span> </td>
						</tr>
						<tr>
							<td>Whether the contractor was convicted of any offence within the preceding five years. If so, give details<font color ="red"> **</font>
							</td>
							<td>'.$details_contractor_convicted.'</td>
							<td>'.$contractor_convicted_license_renewal.'</td>
							<td>'.$offence_chk.'<span style="color:#006600;">'.$offence_info.'</span> </td>
						
						</tr>
						<tr>
							<td>Whether there was any order against the contract or revoking or suspending license or forfeiting security deposit in respect of an earlier contract. If so, the date of such order.<font color ="red"> **</font>   </td>
							<td>'.$contractor_revoking.'</td>
							<td>'.$contractor_revoking_license_renewal.'</td>
							<td>'.$revoking_chk.'<span style="color:#006600;">'.$revoking_info.'</span> </td>
					
						</tr>
						<tr>
							<td>Whether the contractor has worked in any other establishment within the past five years. If so. give details of the principal employer, establishment and nature of work <font color ="red"> **</font></td>
							<td>'.$pe_details_reg.'</td>
							<td>'.$pe_details_reg.'</td>
							<td>'.$pervious_license_chk.'<span style="color:#006600;">'.$pervious_license_info.'</span> </td>
						
						</tr>
					
					
						<tr>
							<td>Whether a certificate by the principal employer in Form V is Enclosed<font color ="red"> **</font></td>
							<td>Yes</td>
							<td>Tes</td>
							<td><i class="fa fa-check" style="color:green;"></i> Verified</td>
							
						</tr>';
  
	drupal_add_css(drupal_get_path('module', 'alc').'/css/jquery-ui.css');
	drupal_add_js(drupal_get_path('module', 'alc').'/js/view_doc_js.js');
	
	$action	 = 'encrypt';
	$application_id_contractor		= encryption_decryption_fun($action, $serial);
	$applicant_user_id_contractor	= encryption_decryption_fun($action, $contractor_user_id);	
	$act_id_contractor = encryption_decryption_fun($action, 12);
	
	
 	$output.='<tr><td colspan="4" style="font-weight:700; text-transform:uppercase; font-size:15px;"><i class="ion ion-clipboard"></i>  Uploaded Documents</td></tr> 
 
		   <tr><td>Work order <font color ="red"> **</font></td>';
		   if(empty($work_order_file_id)){
				$output=$output.'<td>N/A</td><td></td>';
				$output = $output.'<td><a title = "Click here to view & print work order" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$work_order_pre_url.'" target="_blank"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;<span class="popup" id='.$work_order_file_license_renewal_id.'><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span></td>';
				$dialog='dialog_'.$work_order_file_license_renewal_id;
				$output.='<div id='.$dialog.' title="Work order" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$work_order_pre_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>
				<td><input id="work_order_id" class="yn_amend_verified" type="checkbox" '.$work_order_ok.'></td>';
		   }else{
			    $output = $output.'<td><a title = "Click here to view & print work order" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$work_order_url.'" target="_blank"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;<span class="popup" id='.$work_order_file_id.'><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span></td>';
				$dialog='dialog_'.$work_order_file_id;
				$output.='<div id='.$dialog.' title="Work order" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$work_order_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>';
				 $output = $output.'<td><a title = "Click here to view & print work order" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$work_order_pre_url.'" target="_blank"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;<span class="popup" id='.$work_order_file_license_renewal_id.'><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span></td>';
				$dialog='dialog_'.$work_order_file_license_renewal_id;
				$output.='<div id='.$dialog.' title="Work order" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$work_order_pre_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>
				<td><input id="work_order_id" class="yn_amend_verified" type="checkbox" '.$work_order_ok.'></td>';
				
		   }
		   
		   $output.=' </tr><tr><td>Form V <font color ="red"> **</font></td>';
		   if(empty($frm_v_file_id_previous)){
		  	    $output=$output.'<td>N/A</td><td>N/A</td>';
		   }else{
				$output=$output.'<td><a title = "Click here to view & print Form V" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$frmv_url.'" target="_blank"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;<span class="popup" id='.$frm_v_file_id_previous.'><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>&nbsp;'.l(t('Generated FORM - V'),'alc-view-formV/'. trim($details['id']).'/'.$serial, array('html' => TRUE,'attributes'=> array('target'=>'_blank'))).'</td>';
				$dialog='dialog_'.$frm_v_file_id_previous;
				$output.='<div id='.$dialog.' title="From V" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$frmv_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div><td><i class="fa fa-check" style="color:green;"></i> Verified</td>
				<td><i class="fa fa-check" style="color:green;"></i> Verified</td>';
		   }
		   $output.=' </tr><tr><td>Residential Certificate/Trade Licence <font color ="red"> **</font></td>';
		   if(empty($residential_file_id_previous)){
			    $output=$output.'<td>N/A</td><td><i class="fa fa-check" style="color:green;"></i> Verified</td><td><i class="fa fa-check" style="color:green;"></i> Verified</td>';
 		   }else{
			   $output=$output.'<td><a title = "Click here to view & print residential certificate" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$residential_url.'" target="_blank"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;<span class="popup" id='.$residential_file_id_previous.'><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span></td>';
			   $dialog='dialog_'.$residential_file_id_previous;
			   $output.='<div id='.$dialog.' title="Residential Certificate" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$residential_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div><td><i class="fa fa-check" style="color:green;"></i> Verified</td><td><i class="fa fa-check" style="color:green;"></i> Verified</td>';
 		  }
	
	
	 $output.=' </tr><tr><td>Previous Form IV or Form VII or Amendment Application Form <font color = "red">**</font></td>';
	
	if(empty($form_iv_vii_id_previous)){
			    $output=$output.'<td>Nil</td><td><i class="fa fa-check" style="color:green;"></i> Verified</td><td><i class="fa fa-check" style="color:green;"></i> Verified</td>';
 		   }else{
			   $output=$output.'<td><a title = "Click here to view & print Previous Application" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$previous_form_iv_vii_url.'" target="_blank"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;<span class="popup" id='.$form_iv_vii_id_previous.' '.$form_iv_id_ck.'><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>&nbsp;&nbsp; '.$generated_form_iv_vii
	.'</td>';
			   $dialog='dialog_'.$form_iv_vii_id_previous;
			   $output.='<div id='.$dialog.' title="Previous Application" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$previous_form_iv_vii_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div><td><i class="fa fa-check" style="color:green;"></i> Verified</td><td><i class="fa fa-check" style="color:green;"></i> Verified</td>';
 		  }
		  
		  
		  
		 $output.=' </tr><tr><td>Previous Certificate<font color = "red">**</font></td>';
	
	if(empty($previous_license_certificate)){
			    $output=$output.'<td>Nil</td><td><i class="fa fa-check" style="color:green;"></i> Verified</td><td><i class="fa fa-check" style="color:green;"></i> Verified</td>';
 		   }else{
			   $output=$output.'<td><a title = "Click here to view & print Previous Certificate" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$previous_previous_certificates_url.'" target="_blank"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;<span class="popup" id='.$previous_license_certificate.' '.$form_iv_id_ck.'><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>&nbsp;&nbsp; '.'</td>';
			   $dialog='dialog_'.$previous_license_certificate;
			   $output.='<div id='.$dialog.' title="Previous Certificate" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$previous_previous_certificates_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div><td><i class="fa fa-check" style="color:green;"></i> Verified</td><td><i class="fa fa-check" style="color:green;"></i> Verified</td>';
 		  }
		    
	 $output.=' </tr><tr><td>Other Document</td>';

	
	if(empty($other_file_id)){
			    $output=$output.'<td>Nil</td>';
 		  
	if(empty($other_file_id_license_renewal)){
		 $output=$output.'<td>Nil</td>';
	}
	}
		   
	if(!empty($other_file_id)){
			   $output = $output.'<td><a title = "Click here to view & print other document" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$other_file_url.'" target="_blank"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;<span class="popup" id='.$other_file_id.'><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span></td>';
				$dialog='dialog_'.$other_file_id;
				$output.='<div id='.$dialog.' title="Other Document" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$other_file_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>';
 		 
    if(!empty($other_file_id_license_renewal)){
			   $output = $output.'<td><a title = "Click here to view & print other document" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$other_file_pre_url.'" target="_blank"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;<span class="popup" id='.$other_file_id_license_renewal.'><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span></td>';
				$dialog='dialog_'.$other_file_id_license_renewal;
				$output.='<div id='.$dialog.' title="Other Document" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$other_file_pre_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>';
 		  }
		  if(empty($other_file_id_license_renewal)){
		 $output=$output.'<td>Nil</td>';
	}
	}
		   $output.=' <td><input type="checkbox" id="other_doc_id" class="yn_amend_verified"   '.$other_doc_id_ok.' /></td></tr>';
	
	 $output.=' </tr><tr><td>Signed Amendment Application<font color = "red">**</font></td>';
	
	if(empty($upload_singed_from_id)){
			    $output=$output.'<td colspan="2">Application Form will be uploaded by the applicant after fees submission</td><td><input type="checkbox" id="application_id"  class="yn_amend_verified"  '.$application_id_ok.'   /></td>';
 		   }else{
			   $output=$output.'<td colspan="2"><a title = "Click here to view & Application From" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$amendment_application_file_url.'" target="_blank"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;<span class="popup" id='.$upload_singed_from_id.' '.$form_iv_id_ck.'><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>&nbsp;&nbsp; '.'</td>';
			   $dialog='dialog_'.$upload_singed_from_id;
			   $output.='<div id='.$dialog.' title="Application From" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$amendment_application_file_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div><td><input type="checkbox" id="application_id"  class="yn_amend_verified"  '.$application_id_ok.'   /></td>';
 		  }
		  
		  
		  if($status == 'I'){
			
			   $output.='<tr><td>Amendment Certificate</td>';
			  
		        if($certificate_id!=''){
					$upload_certificate_file = fetch_uplaoded_file_details($certificate_id);
					if(!empty($upload_certificate_file)){ 
						$url			  = explode('//',$upload_certificate_file['uri']);
						$certificate_url  = $url[1];
						$certificate_file = $upload_certificate_file['filename'];
					}
					$output=$output.'<td colspan="2"><a title = "Click here to view & print Amendment Certificate" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$certificate_url.'" target="_blank">
									 <i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;<span class="popup" id='.$upload_singed_from_id.'><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>&nbsp;&nbsp; '.'</td>';
			   		$dialog='dialog_'.$certificate_id;
			   		$output.='<div id='.$dialog.' title="Application From" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/'.$certificate_url.'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>';
			   
			   
			    	
				}else{ 
					$output=$output.'<td><a title = "Click here to view & print Amendment Certificate" href="'.$GLOBALS['base_url'].'/clra-amendment-renewal-certificates/'.encryption_decryption_fun('encrypt', trim($serial)).'/'.encryption_decryption_fun('encrypt', $ammendid).'/'.encryption_decryption_fun('encrypt', trim($created_by)).'/'.encryption_decryption_fun('encrypt','A').'" target="_blank"><i class="fa fa-file fa-lg"></i></a>&nbsp;&nbsp;<span class="popup" id='.$work_order_file_id.'><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span></td>';
					
				}
				
		  }
		  
	
	
 	//$output.=' <tr><td><strong>Payment Details</strong><font color = "red">**</font></td><td> '.$payment_details.'</td><td></td><td><input type="checkbox" id="payment_id" class="yn_amend_verified"  '.$payment_details_ck.'/>  <font color = "red">For Amendment</font></td></tr></table><br class="clear" /><center>';
	
	$output.=' <tr><td>Payment Details<font color = "red">**</font></td><td> '.$payment_details.'</td><td>'.$payment_details_license_renewal_amand.'</td><td><input type="checkbox" id="payment_id" class="yn_amend_verified"  '.$payment_details_ck.'/></td></tr></table></div><br class="clear" /><center>';
	
	
	
 	$output .='</div></div><div class="row">';
	$output .= '<ul style="padding:0;">';
	$output .= '<li class="col-md-2 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-arrow-left"></i> Back to list</div>', 'alc/alc-received-ammendment-license', array('attributes' => array('title' => 'Click here back to applications list', 'target' => '_blank'), 'html' => TRUE)).'</li>';
	
	$output .= '<li class="col-md-3 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-user"></i> View applicant profile</div>', 'view-applicant-profile/'.$application_id_contractor.'/'.$applicant_user_id_contractor.'/'.$act_id_contractor, array('attributes' => array('title' => 'Click here view Applicant Profile', 'target' => '_blank'), 'html' => TRUE)).'</li>';
	
	$output .= '<li class="col-md-3 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info"><i class="fa fa-info-circle"></i> Instructions to give remarks</div>', $base_root.$base_path.'sites/default/files/instructions-remarks.pdf', array('attributes' => array('title' => 'How to digitally sign your documents using USB Token', 'target' => '_blank'), 'html' => TRUE)).'</li>';
	
	$output .= '<li class="col-md-4 col-xs-12" style="list-style:none;margin-bottom:5px;">'.l('<div class="btn btn-block btn-info" style="white-space:normal;"><i class="fa fa-question-circle" aria-hidden="true"></i> How to digitally sign documents using USB token</div>', 'digitally-sign-process', array('attributes' => array('title' => 'How to digitally sign your documents using USB Token', 'target' => '_blank'), 'html' => TRUE)).'</li></ul></div>';
	
	return $output;	

}
function show_remark_details($reference_no){
	
	$remark_details = remark_details($reference_no);	
	
	
	/*** Show Reamrk ***/	
	$sl_no='0';
	 global $user;
	 $user_id 					= $user->uid;
	
	
	$header = array(
					array('data' => 'Sl. No'),
					array('data' => 'Remark'),
					array('data' => 'Status'),
					array('data' => 'Serial Number(Form V)'),
					array('data' => 'Date'),
					array('data' => 'Remark By'),
					//array('data' => 'Action', 'field' => 'action'),
				);

	
	if(!empty($remark_details)){
		foreach($remark_details as $row){
		$sl_no=$sl_no+1;
		
		$remark_by=$row->fullname.'('.$row->name.')';
		if($row->remark_type == 'B'){
				$current_status = '<span class="backed">Backed</span>';
			}
			if($row->remark_type == 'FW'){
				$current_status = '<span class="forward">Forwarded</span>';
			}if($row->remark_type == 'BA'){
				$current_status = '<span class="pending">Backed</span>';
			}
			if($row->remark_type == 'A'){
				$current_status = '<span class="feespending">Fees Pending</span>';
			}if($row->remark_type == 'AW'){
				/*$pre_remark = remark_details_second_last1($reference_no,$row->id);
				
				if($pre_remark == 'U'){
					$current_status = '<span class="pending">Backed</span>';
				}else{*/
				
				$current_status = '<span class="approved">Approved</span>';
				
			}
			if($row->remark_type == 'C'){
				$current_status = '<span class="call_applicant">Call Applicant</span>';
			}
			if($row->remark_type == 'I'){
				$current_status = '<span class="issued">Issued</span>';
			}
			if($row->remark_type == 'P'){
				$current_status = '<span class="feespaid">Fees Paid</span>';
			}if($row->remark_type == 'P' && $row->remark_by_role == '4'){
				$current_status = '<span class="pending">Backed</span>';
			}
				
				
			
			if($row->remark_type == 'U'){
				$current_status = '<span class="finalsubmit">Final Submit</span>';
				$pre_remark		=	remark_details_second_last($reference_no);
			}
			/*if($row->remark_type == 'AW' && $pre_remark == 'U')
				$current_status = '<span class="pending">Backed</span>';*/
			if($row->remark_type == 'R'){
				$current_status = '<span class="reject">Reject</span>';
			}
			if($row->remark_type == 'BI'){
				$current_status = '<span class="backtoins">Back to Inspector</span>';
			}if($row->remark_by_role == '4'){
				$remark_by_role1 = 'ALC';
			}if($row->remark_by_role == '8'){
				$remark_by_role1 = 'Inspector';
			}
			$query 	= 	db_select('l_remark_license', 'lrl')
				->	fields('lrl', array('remark_by','id','remark_type'))
			
				->	condition('lrl.particular_id', $reference_no)
				->	condition('lrl.flag', 'A')
				->	condition('lrl.is_active', 1)
				->	orderBy('id', 'DESC')
			    ->  extend('PagerDefault')->limit(1);
						 
		$remark_by_id = $query->execute()->fetchObject()->remark_by;
		$remark_id = $query->execute()->fetchObject()->id;
		$remark_type = $query->execute()->fetchObject()->remark_type;
		//print_r($remark_id);die('Under Development'.$particularid);
		//echo  $remark_by_id;die();
		if($user_id == $remark_by_id && $row->id == $remark_id ){
			$url 				= 'license-remarks-delete/'.encryption_decryption_fun('encrypt',$row->id).'/'.encryption_decryption_fun('encrypt',$particularid).'/'.encryption_decryption_fun('encrypt',$serial).'/'.$license_id;			
			$delete				= l('<div id="delBtnremark" class="delete-btn active-del" >'.t('Delete').'</div>',$url,array('html' => TRUE));	
		}else{
			$delete = '<div id="delBtn" class="delete-btn disable-del">'.t('Delete').'</div>';;
			
		}
		if($remark_type == 'R'){
			$delete = '<div id="delBtn" class="delete-btn disable-del">'.t('Delete').'</div>';;
		}
		
		$rows[] = array(
					array('data' => $sl_no, 'align' => 'left', 'class' => array('odd')),
					array('data' => $row->remark_text, 'align' => 'left', 'class' => array('odd')),
					array('data' => $current_status,$current_status1, 'align' => 'left', 'class' => array('odd')),
					array('data' => '00'.$row->remark_to, 'align' => 'left', 'class' => array('odd')),
					array('data' =>date('d/m/Y', $row->remark_date), 'align' => 'left', 'class' => array('odd')),
					array('data' =>$row->remark_by_name.'('.$remark_by_role1.')', 'align' => 'left', 'class' => array('odd')),
					//array('data' =>$delete, 'align' => 'left', 'class' => array('odd'))
				);
	}
		
	}

	


	$variables = array(
					'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')), 
					'header' 	 => $header,
					'rows'		 => $rows,
				);
	$output = theme('datatable', $variables);
	
	 return $output;			
}
function get_action_field($form, $form_state){
	$commands 	= array();	

	//$commands[] = ajax_command_replace('#'.$original_id .'-ajax-wrapper', drupal_render($form['view_ammend']['certificates']));
	$commands[] = ajax_command_replace('#signed_license_certificate_block', drupal_render($form['view_ammend']['certificates']));
	$commands[] = ajax_command_replace('#genarate_certificates', drupal_render($form['view_ammend']['download_certificates']));
	$commands[] = ajax_command_replace('#remarks_text_div', drupal_render($form['view_ammend']['remarks_text']));
	$commands[] = ajax_command_replace('#submit_div', drupal_render($form['view_ammend']['submit']));
		
	return array('#type' => 'ajax', '#commands' => $commands);	
}


function alc_view_ammend_license_submit($form,&$form_state){	
 
	$license_issued=0;	
	global $user;
	$uid = $user->uid;	
	$license_issued	= 0;
	
	$remark_by_role =  db_select('l_custom_user_detail', 'user_details');
	$remark_by_role	->	fields('user_details', array('fullname'))->condition('user_details.usr_id',$uid,'=');
	$remark_by_name	= $remark_by_role->execute()->fetchObject()->fullname;
		
	$val 				= $form_state['values']; 
	
	
	$ammend_id			= encryption_decryption_fun('decrypt', $val['ammend_id']);
	$contrcator_name    = encryption_decryption_fun('decrypt', $val['contrcator_name']);	
	$status  			= trim($val['action_type']);
	$remark_text 		= $val['remarks_text'];
	$calltime 			= $val['call_applicant'];
	$remark_by  		= $uid;
	$remark_to  		= encryption_decryption_fun('decrypt',$val['serial']);
	$remark_field_title	= $val['editable_fields_ammend'];
	$particularid		=  encryption_decryption_fun('decrypt', $val['particularid']);
	$license_id			= encryption_decryption_fun('decrypt', $val['license_id']); 
	$not_amendment_check_field		= encryption_decryption_fun('decrypt', $val['not_amendment_check_field']);
	$amendment_check_field			= encryption_decryption_fun('decrypt', $val['amendment_check_field']);
	
	if($license_id!=10943){
			$file 					= file_load($val['certificates']);   
			if(!empty($file)){	
				$uri 				= trim($file->uri); 
				$file->status 		= FILE_STATUS_PERMANENT;
				file_save($file);
				$certificates_fid	= $file->fid;
				$certificateName	= $file->filename;
			}
	}

	$remark_field_title	= $val['editable_fields_ammend'];	

	$getEmployeeid 		= db_select('l_custom_user_detail','cd')->fields('cd',array('employee_id','fullname'))->condition('usr_id',$uid)->execute()->fetchAssoc();	
	$employeeId = $getEmployeeid['employee_id'];

	if($status == 'I'){ 
		$contractor_id = return_created_by($ammend_id);
	
		 if($license_id==10943){ 
			  
			   // for time basis..need to be removed later// 
			   $getamendRef		= db_query("SELECT ain_amendment FROM l_amendment_of_contract_labour_license WHERE reference_no=".$particularid." and ammendment_status= 'U'")->fetchAssoc(); print_r($getamendRef); echo 999;
			   $getRefNo		= $getamendRef['ain_amendment'];
			   
			   if(empty($getRefNo)){ 
				   $ain_no  = 'AMEND-LIC-00'.$license_id.'-00'.$particularid.'-'.$contractor_id.'-'.date('Y');
				   $update_status = array('ammendment_status'=>$status,'alc_inspector_action_date' =>date("Y-m-d"),'remark_by_userid'=> $uid,'deemed'=>'N','ain_amendment'=>$ain_no);
				 
			   }else{
				   $update_status = array('ammendment_status'=>$status,'alc_inspector_action_date' =>date("Y-m-d"),'remark_by_userid'=> $uid,'deemed'=>'N');
			   }
			 
			  
				$apply_status = db_update('l_amendment_of_contract_labour_license');
				$apply_status->fields($update_status);
				$apply_status->condition('license_id',$license_id);
				$apply_status->condition('reference_no',$particularid);
				$apply_status->execute();
				$remark_details_license = array(
										'remark_text'   		=> 'Congratulations! Certificate is issued by the Licensing Authority. You can download it from the dashboard.', 
										'remark_by'   			=> $remark_by, 
										'remark_to' 			=> $remark_to,
										'remark_type' 			=> $status,
										'remark_date'  			=> time(),
										'remark_field_title' 	=> $remark_field_title,
										'particular_id'			=>	$particularid,
										'remark_by_role' 		=> '4',
										'remark_by_name' 		=>  $remark_by_name,
										'flag' 					=>  'A',
										'not_amendment_check_field' => $not_amendment_check_field,
										'amendment_check_field' => $amendment_check_field,
										'employee_id'			=> $employeeId
										
								);

				db_insert('l_remark_license')->fields($remark_details_license)->execute();
				contractor_activity_tag_insert($remark_to,'A',$ammend_id,$status,$contractor_id);
						
				drupal_set_message(t('Remark is sucessfully saved'));
			 
		 }
	 
		if(!empty($certificates_fid)&& $license_id!=10943){ 
				$update_status = array('ammendment_status'=>$status,'alc_inspector_action_date' =>date("Y-m-d"),'remark_by_userid'=> $uid,'ammendment_certificate_id'=>$certificates_fid);
				$apply_status = db_update('l_amendment_of_contract_labour_license');
				$apply_status->fields($update_status);
				$apply_status->condition('license_id',$license_id);
				$apply_status->condition('reference_no',$particularid);
				$apply_status->execute();
				
							
				
				$remark_details_license = array(
										'remark_text'   		=> $remark_text, 
										'remark_by'   			=> $remark_by, 
										'remark_to' 			=> $remark_to,
										'remark_type' 			=> $status,
										'remark_date'  			=> time(),
										'remark_field_title' 	=> $remark_field_title,
										'particular_id'			=>	$particularid,
										'remark_by_role' 		=> '4',
										'remark_by_name' 		=>  $remark_by_name,
										'flag' 					=>  'A',
										'not_amendment_check_field' => $not_amendment_check_field,
										'amendment_check_field' => $amendment_check_field,
										'employee_id'			=> $employeeId
								);

				db_insert('l_remark_license')->fields($remark_details_license)->execute();	
	
				$contractor_id = return_created_by($ammend_id);
				contractor_activity_tag_insert($remark_to,'A',$ammend_id,$status,$contractor_id);
						
				drupal_set_message(t('Remark is sucessfully saved'));
	
			}
		if(empty($certificates_fid) && $license_id!=10943){			
			drupal_set_message(t('Please do not click on submit button before upload the Certificate of Amendment'),'error');
		}
	}
	
	if($status != 'I'){
		
		$update_status =  array('ammendment_status' => $status, 'alc_inspector_action_date' => date("Y-m-d"), 'remark_by_userid' => (int)$uid);

		// if($uid==460){
		// 	echo $license_id.'---'.$particularid;
		// echo "<pre>";
		// print_r($update_status);die;
		// }
		$apply_status = db_update('l_amendment_of_contract_labour_license');
		$apply_status->fields($update_status);
		$apply_status->condition('license_id', $license_id);
		$apply_status->condition('reference_no',$particularid);
		$apply_status->execute();
		
		// print_r($update_status); echo $license_id.'---'.$particularid; die;
		
		$remark_details_license = array(
				'remark_text'   		=> $remark_text, 
				'remark_by'   			=> $remark_by, 
				'remark_to' 			=> $remark_to,
				'remark_type' 			=> $status,
				'remark_date'  			=> time(),
				'remark_field_title' 	=> $remark_field_title,
				'particular_id'			=>	$particularid,
				'remark_by_role' 		=> '4',
				'remark_by_name' 		=>  $remark_by_name,
				'flag' 					=>  'A',
				'not_amendment_check_field' => $not_amendment_check_field,
				'amendment_check_field' => $amendment_check_field,
				'employee_id'			=> $employeeId
		);
		//print_r($remark_details_license);

		db_insert('l_remark_license')->fields($remark_details_license)->execute();	
		$contractor_id = return_created_by($ammend_id);
		contractor_activity_tag_insert($remark_to,'A',$ammend_id,$status,$contractor_id);		
				
		drupal_set_message(t('Remark is sucessfully saved'));
		//die();
		
		
	}
	
 
}


function create_pdf($form, &$form_state) {
	 global $user;
	 $uid 					= $user->uid;
	 $val   				= $form_state['values'];
 	 $serial_no_from_v  	= $val['serial'];
 	 $subdivision_alc		= $val['subdivision_alc'];
  	 $particularid			= $val['particularid'];
	 $remark_field_title	= $val['editable_fields_license'];
	 $editablefield_array 	= explode(',', $remark_field_title);
	
	 $array2 = array("payment_id","frm_v_file_id");
	 $result1 = array_diff($array2, $editablefield_array);	
	
	 if(empty($result1)){
 	 $form_state['values']['action_type'] 	= 'I';
  	// $licence_qr_code						= 'CONTRACTOR-LICENSE'.$serial_no_from_v.'A1C'.'T'.time();
 
    	 $query_name_alc				=	db_select('l_custom_user_detail', 'user_details');
	 	$query_name_alc					->	fields('user_details',array('fullname'));
	 	$query_name_alc					->	condition('user_details.usr_id',$uid,'=');
		$name_alc 					 	 = 	$query_name_alc->execute()->fetchObject()->fullname;
			
 
 		$query_Is_license_code 		 	= 	db_select('l_contractor_license_application', 'lca');
		$query_Is_license_code			->	fields('lca',array('contractor_license_number'));
		$query_Is_license_code			->	condition('lca.contractor_license_number','NULL','!=')
				 						->	orderBy('lca.id', 'DESC')
				    					->	range(0,1);
		$query_Is_license_code_result  	= 	$query_Is_license_code->execute()->fetchObject()->contractor_license_number;

 		$query_contractor_add 		 	= 	db_select('l_particular_of_contract_labour', 'pcl');
		$query_contractor_add			->	fields('pcl',array('contractor_subdivision','contractor_name_areatype','contractor_user_id'));
		$query_contractor_add			->  condition('pcl.serial_no_from_v',$serial_no_from_v);
		$query_contractor_add			->  condition('pcl.id',$particularid);
		$query_contractor_add_list  	= 	$query_contractor_add->execute()->fetchAssoc();
		/*$contractor_subdv				=	$query_contractor_add_list['contractor_subdivision'];
		$contractor_areacod				=	$query_contractor_add_list['contractor_name_areatype'];*/
		$contractor_user_id				=	$query_contractor_add_list['contractor_user_id'];
		
		$query_sub_name 		 		= 	db_select('sub_division', 'sub');
		$query_sub_name					->	InnerJoin('block_mun_master', 'blockmaster','blockmaster.sub_division_code = sub.sub_div_code');
		
		$query_sub_name					->	fields('sub',array('sub_div_name'));
		$query_sub_name					->	fields('blockmaster',array('block_code'));
		$query_sub_name					->  condition('sub.sub_div_code',$subdivision_alc);
		$contractor_subdv  				= 	$query_sub_name->execute()->fetchObject()->sub_div_name;
		
		
		$query_blockcode 				= 	db_select('l_contractor_info_master', 'infomaster');
		$query_blockcode				->	InnerJoin('l_clra_registration_master', 'regmaster','regmaster.id = infomaster.application_id');  
		$query_blockcode				->	fields('regmaster',array('name_areatype'));
		$query_blockcode				->	fields('infomaster',array('name_of_contractor'));
		$query_blockcode				->	condition('infomaster.id',$serial_no_from_v,'=');
	
	 	$pe_blockcode					= 	$query_blockcode->execute()->fetchObject()->name_areatype; 
		$name_of_contractor				= 	$query_blockcode->execute()->fetchObject()->name_of_contractor; 
		
		
		
		//$contractor_subdv				=	$query_contractor_add_list['contractor_subdivision'];
		//$contractor_areacod				=	$query_contractor_add_list['contractor_name_areatype'];
		/*$contractor_user_id				=	$query_contractor_add_list['contractor_user_id'];*/

	
 		$get_sub_ShortName  			=   custom_user_short_name_fun($subdivision_alc);  // ---------come from custom_user module 
		
		$reg_contractor_areacod			=	get_registration_code($pe_blockcode);  //-------------miscellaneous module

	  	$get_con_area_code 				=   substr($reg_contractor_areacod, -2); 
		
		 if(empty($query_Is_license_code_result)){
					$license_number			=	$get_sub_ShortName.$get_con_area_code.'/'.'CLL'.'/'.'000001';
	  }
	  else{
		  
		  //$area_code = $get_sub_ShortName.$get_con_area_code;
		  $area_code = $get_sub_ShortName;
		
			//$query_count				=	db_query("SELECT max(contractor_license_number) FROM l_contractor_license_application WHERE substr(contractor_license_number,0,4) ='".$get_sub_ShortName."' order by id desc LIMIT 1 ");
			//$query_count		=	db_query("select  max (cast(substr(contractor_license_number,11,6) as integer)) as serial_no  from l_contractor_license_application where substr(contractor_license_number,0,6) ='".$area_code."'");
			$query_count		=	db_query("select  max (cast(substr(contractor_license_number,11,6) as integer)) as serial_no  from l_contractor_license_application where substr(contractor_license_number,0,4) ='".$area_code."'");
			$query_total_count			=	$query_count->fetchAssoc();	//print_r($query_total_count);die();
	 		$serial_no	=   $query_total_count['serial_no'];	
			//print_r($contractor_license_number);die();				
		if(!empty($serial_no )){
				
 			//$license_code				= 	explode('/', $contractor_license_number);
			$license_code_first			=	$get_sub_ShortName.$get_con_area_code.'/'.'CLL'.'/';	
			$license_code_second		=	$serial_no+1;	
			//$license_code_third			=	(int)$license_code[2];
			//$license_code_third			=	$license_code_third+1;	
			//$license_number_gen_1		=	$license_code_first.'/'.$license_code_second.'/';
			$license_number_gen_2		=	str_pad($license_code_second, 6, "0", STR_PAD_LEFT);
			$license_number				=	$license_code_first.$license_number_gen_2	;
			//die();
		}
				 
				else{
					$license_number		=	$get_sub_ShortName.$get_con_area_code.'/'.'CLL'.'/'.'000001';

				}
	}
	  
	 //print_r($license_number);die();

		//$time = strtotime($license_date);
			
		//$next_renweal_date 			= date("Y-m-d", strtotime("+364 days", $time));	  
	    $license_issued				= 1;
		$license_date 				= date('m/d/Y h:i:s a', time());
		$time 						= strtotime($license_date);
        $next_renweal_date 			= date("Y-m-d", strtotime("+364 days", $time));
		
		$SLNo						= 'CLL'.$serial_no_from_v.'V'.'T'.time();
		
		
		$licence_qr_code			= encryption_decryption_fun('encrypt',"License No:".$license_number."\nIssue Date:". date("dS M, Y",strtotime($license_date))."\nName of Contractor:".$name_of_contractor."\nValid upto:".date("dS M, Y",strtotime($next_renweal_date))."\nIsussing Authority:".$name_alc."\nSL.No:".$SLNo);
		
		
 	  	$update_details   		=  	array('licence_qr_code'=>$licence_qr_code,'contractor_license_number' =>$license_number,'license_date'=> $license_date,'next_renweal_date'=> $next_renweal_date,'modification_date' =>date("Y-m-d"),'remark_by_userid'=> $uid,'block_next_renwal_date' =>$next_renweal_date);
	  $update_query  	=  	db_update('l_contractor_license_application');
	  $update_query		->	fields($update_details);
	  $update_query		->	condition('serial_no_from_v',$serial_no_from_v);
	   $update_query	->	condition('contractor_particular_id',$particularid);
	  $update_query		->	execute() ;
	  
	  
	
		
				  
	 
		
		$update_user_id   				=  	array('contractor_user_id'=>$contractor_user_id);
	  	$update_contractor_user_id  	=  	db_update('l_contractor_info_master');
	  	$update_contractor_user_id		->	fields($update_user_id);
	  	$update_contractor_user_id		->	condition('id',$serial_no_from_v);
	  	$update_contractor_user_id		->	execute() ;
		
	
	
	$message= drupal_set_message_custom(t('License Certificate has been successfully generated.Please download and upload this License Certificate along with signature.</br>Then click on submit button to complete this process'));//-------------miscellaneous module

 $form_state['values']['hidden_action_message'] = $message;

  $form_state['rebuild'] =TRUE;
	 }else{
		 	$action 		= 	'encrypt';
			$serial			=	encryption_decryption_fun($action, $serial_no_from_v);//---miscellaneous module
			$particular_id	=	encryption_decryption_fun($action, $particularid);//
		 drupal_set_message(t('Please verify the <strong>"Form IV and Payment Details" by checking  check box in </strong>"Is Verified?"</strong> column then you can proceed for Issue Certificate of application remark'),'error');
						drupal_goto('alc-view-license/'.$serial.'/'.$particular_id);
	 }
 
}
function license_remarks_delete ($remarkid,$particularid,$serial,$licenseid){//print_r($particularid);
	$remark_id 			= encryption_decryption_fun('decrypt',$remarkid);
	$particular_id 		= encryption_decryption_fun('decrypt',$particularid);//die($particularid);
	$serial_no 			= encryption_decryption_fun('decrypt',$serial);
	$license_id 	    = encryption_decryption_fun('decrypt',$licenseid);
	
	
	$query_last_status 	= 	db_select('l_remark_license', 'lrl')
						->	fields('lrl', array('remark_type'))
						->	condition('lrl.id', $remark_id);
	$last_status		=  $query_last_status->execute()->fetchObject()->remark_type;
	if($last_status == 'A'){
		$delquery			= db_delete('l_remark_license')->condition('id', $remark_id)->execute();
		db_query("SELECT l_del_satus_during_unapproved ('".$serial_no."')");
		
		
	}if($last_status != 'A'){
		$delquery			= db_delete('l_remark_license')->condition('id', $remark_id)->execute();
		
	}

	
	$query 	= 	db_select('l_remark_license', 'lrl')
				->	fields('lrl', array('remark_type','remark_by'))
				->	condition('lrl.remark_to', $serial_no)
				->	condition('lrl.particular_id', $particular_id)
				->	condition('lrl.flag', 'L')
				->  condition('lrl.is_active', 1, '=')
				->	orderBy('id', 'DESC')
			    ->  extend('PagerDefault')->limit(1);
						 
	$query_details		= $query->execute()->fetchAssoc();
 $remark_type		= $query_details['remark_type'];	
 $remark_by			= $query_details['remark_by'];
	if($remark_type	==	'U'){
		$update				= db_update('l_contractor_license_application')
							->fields(array(
									'status' 			=> $remark_type,
									'license_issued'	=>	0,
									'certificates_id'	=>	0	
									
									
								  ))
							->condition('serial_no_from_v',$serial_no)
							->condition('contractor_particular_id',$particular_id)
							->execute();
	}else{
	
	
	$update				= db_update('l_contractor_license_application')
							->fields(array(
									'status' 			=> $remark_type,
									'remark_by_userid'	=> $remark_by
								  ))
							->condition('serial_no_from_v',$serial_no)
							->condition('contractor_particular_id',$particular_id)
							->execute();
	}
	
		update_activity_tag($license_id,$remark_type);
	drupal_set_message(t('Remark is sucessfully deleted'));
	drupal_goto('alc-view-license/'.$serial.'/'.$particularid);
	return $form;
}
function payment_details($application_id,$contractor_user_id,$act_id){
	
	
		$transaction_details  			= 	db_select('l_principle_epayments_receive_data', 'lpd');
		$transaction_details			->	leftJoin('l_principle_epayments_data', 'lped', 'lpd.transaction_id = lped.identification_no');
		$transaction_details			->  fields('lped', array('identification_no', 'application_id'));
		$transaction_details			->  fields('lpd', array());
		$transaction_details			->	condition('lped.act_id',$act_id);
		$transaction_details			->	condition('lped.application_id',$application_id);
		$transaction_details			->	condition('lped.deposited_by',$contractor_user_id); 
		$transaction_det				= 	$transaction_details->execute()->fetchAssoc();
	
	
			
				
		$bank_code 						= 	'Not available';
	 	$bankTransactionID 				= 	'Not available';
	 	$total_amount 					= 	'Not available';
	 	$challan_fid_date 				= 	'Not available';
	 	$payment_status 				=   'Not available';
		if(!empty($transaction_det)){
				$bankTransactionID				= $transaction_det['transaction_id'];
				$grn_number						= $transaction_det['challanrefid'];
				$challan_fid_date				= !empty($transaction_det['challanrefid_date']) ? $transaction_det['challanrefid_date'] : '';
				$total_amount					= number_format($transaction_det['challanamount'], 2);
				$bank_code						= $transaction_det['bank_cd'];
				
				if($transaction_det['banktransactionstatus'] == 'Success') {
					$payment_status = '<font color="#060">'.$transaction_det['banktransactionstatus'].'</font>';
				}else{
					$payment_status = '<font color="#CC0000">'.$transaction_det['banktransactionstatus'].'</font>';
				}
		}
	 $payment_details = '<u>Grips Payment[Online/Counter]</u><br>';
	 $payment_details .= 'IFSC Code:<font color="#FF6600">'.$bank_code.'</font><br>';
	 $payment_details .= 'Bank Transaction Id:<font color="#FF6600">'.$bankTransactionID.'</font><br>';
	 $payment_details .= 'Total Amount:<font color="#FF6600">'.$total_amount.'</font><br>';
	 $payment_details .= 'Transaction Date:<font color="#FF6600">'.$challan_fid_date.'</font><br>';
	 $payment_details .= 'Transaction Status: '.$payment_status.trim($remark_type);	
	 return $payment_details;
	
}