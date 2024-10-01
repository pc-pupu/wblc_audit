<?php

function dashboard_view_more_applications($serviceName='' , $actId=''){
	
	global $user, $base_root, $base_path;
	
	$service_name			= $serviceName;//'contract-labour-registration-amendment';//$serviceName;
	$act_id					= encryption_decryption_fun('decrypt',$actId);//1;//$actId;
	$user_id				= $user->uid;
	
	
	if($act_id == 1){
		$act_name = 'Applications for Contract Labour Registration & Amendment';	
	}elseif($act_id == 2){
		$act_name = 'Applications for Registration of Establishments Under Building & Other Construction Workers Act,1996';
	}elseif($act_id == 3){
		$act_name = 'Applications for Registration of Motor Transport Workers, Act';
	}elseif($act_id == 4){
		$act_name = 'Applications for Registration of Inter-State Migrant Workers, Act';
	}
	
	$form['view_more']['header']				= array(
														'#markup' 		=> t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">'.$act_name.'</div><div class="panel-body">'),
														'#type' 		=> 'markup',
														);
														
	$form['view_more']['application_data']		= array(
														'#markup' 		=> '<br/><br/>'.getMoreDetails($service_name,$act_id),
														'#type' 		=> 'markup',
														'#suffix' 		=> '</div>',
														);
														
	return $form;
	
}

function getMoreDetails($service_name='',$act_id=''){
														
														
	global $user, $base_root, $base_path;	
	
	$user_id				= $user->uid;												
	
	$header 	= array(
					  array('data' => 'Sl. No', 'field' => 'slno'),
					  array('data' => 'REGISTRATION NO.<br/>ISSUE DATE', 'field' => 'registration_number'),
					  array('data' => 'ESTABLISHMENT NAME', 'field' => 'e_name','width'=>'25%'),
					  array('data' => 'ACT', 'field' => 'act_id'),
					  array('data' => 'STATUS', 'field' => 'status','class' => array('not-sorting')),
					  array('data' => 'ACTION', 'field' => 'actions','class' => array('not-sorting')),
	  		 	  );
	
	if($service_name == 'contract-labour-registration-amendment' && $act_id= 1){
		
		$contract_labour			= db_select('l_clra_registration_master','lcrm');
		$contract_labour			->fields('lcrm', array());
		$contract_labour			->condition('lcrm.user_id', $user_id);
		$contract_labour			->condition('lcrm.act_id', $act_id);
		$contract_labour			->condition('lcrm.status', 'I');
		$contract_labour			-> orderBy('lcrm.id', 'DESC');
		$result 					= $contract_labour->execute();
		
	}else if($service_name == 'building-construction-workers' && $act_id= 2){
		
		$building_construction		= db_select('l_bocwa_registration_master','lbrm');
		$building_construction		->fields('lbrm', array());
		$building_construction		->condition('lbrm.user_id', $user_id);
		$building_construction		->condition('lbrm.act_id', $act_id);
		$building_construction		->condition('lcrm.status', 'I');
		$building_construction		->orderBy('lbrm.id', 'DESC');
		$result 					= $building_construction->execute();
		
	}else if($service_name == 'motor-transport-registration-renewal' && $act_id= 3){
		
		$motor_transport		= db_select('l_mtw_registration_master','lmrm');
		$motor_transport		->fields('lmrm', array());
		$motor_transport		->condition('lmrm.user_id', $user_id);
		$motor_transport		->condition('lmrm.act_id', $act_id);
		$motor_transport		->condition('lcrm.status', 'I');
		$motor_transport		->orderBy('lmrm.id', 'DESC');
		$result 				= $motor_transport->execute();
		
	}else if($service_name == 'inter-state-registration-amendment' && $act_id= 4){
		
		$inter_state			= db_select('l_interstate_workman_master','liwm');
		$inter_state			->fields('liwm', array());
		$inter_state			->condition('liwm.user_id', $user_id);
		$inter_state			->condition('liwm.act_id', $act_id);
		$inter_state			->condition('lcrm.status', 'I');
		$inter_state			->orderBy('liwm.id', 'DESC');
		$result 				= $inter_state->execute();
	}
	
	
	$allApplications	= 	$result->fetchAll();
	
	$per_page			= 1;
	$current_page 		= pager_default_initialize(count($allApplications), $per_page);
	$chunks 			= array_chunk($allApplications, $per_page, TRUE);
	
	if($current_page>0){
		$counter = $current_page*$per_page;
	}
	
	if(count($allApplications)>0){
		
		foreach($chunks[$current_page] as $data){ 
			
			$counter++;
			$view 							= '<i class="fa fa-chevron-circle-right"></i>'.' View Details';
			$act_name						= '';
			$act_wise_link					= '';
			$download_certificate_link 		= '';
			$remark_link					= '';
			$payment_details				= '';
			$upload_forms_link				= '';
			$pay_now						= '';
			$download_certificate			= '';
			$registration_number			= '';
			$form_v_link					= '';
			
			
			/*** FOR CONTRACT LABOUR ***/
			
			if($data->act_id == 1){
				
				$act_name					= 'REGISTRATION';				
				$act_wise_view_link 		= 'clra-application/view/';
				$act_wise_link 				= 'view-clra-application-details/view-clra-application/';
				
				if(!empty($data->amendment_parent_id)){
					$act_wise_link 			= 'clra-reg-amendment/view-clra-application/';
					$act_name				= 'AMENDMENT';	
				}
				
				
				/** Fetch Certificate of CLRA *****/
				
				$upload_certificates_id 	= db_select('file_managed','fm');
				$upload_certificates_id		-> fields('fm',array());
				$upload_certificates_id		-> condition('fid',$data->certificates_fid);
				$upload_certificates_file	= $upload_certificates_id-> execute()->fetchAssoc();
								
							
				if(!empty($upload_certificates_file)){ 
				
					  $url									= explode('//',$upload_certificates_file['uri']);
					  $upload_certificates_file_url			= $url[1];
					  $upload_certificates_file_name		= $upload_certificates_file['filename'];
				}
			  
			 
			  $download_certificate_link 	= $base_root.$base_path.'sites/default/files/'.$upload_certificates_file_url;
				
			}
			
			/*** FOR BUILDING & OTHER CONSTRUCTION WORKERS  ***/
			
			if($data->act_id == 2){
				
				$act_name					= 'BOCWA REG'; 
				$act_wise_link 				= 'bocwa-application/default/';
				$act_wise_view_link 		= 'bocwa-application-view/';
				
				$upload_certificates_id 	= db_select('l_bocwa_registration_master','lbrm');
				$upload_certificates_id		-> fields('lbrm',array('bocwa_registration_certificate'));
				$upload_certificates_id		-> condition('lbrm.user_id',$userid);
				$upload_certificates_file	= $upload_certificates_id-> execute()->fetchAssoc();
				
				
				
				$download_certificate_link 	= $GLOBALS['base_url'].'/sites/default/files/upload/BOCWA_Registration_Certificate/'.$upload_certificates_file['bocwa_registration_certificate'];
				
			}
			
			/*** FOR MOTOR TRANSPORT WORKERS  ***/
			
			if($data->act_id == 3){
				$act_name					= 'MTW REG'; 
				
				$act_wise_link 				= 'mtw-registration-details/preview/';
				$act_wise_view_link 		= 'mtw-registration-details/view/';
				
				$upload_certificates_id 	= db_select('l_mtw_registration_master','lmrm');
				$upload_certificates_id		-> fields('lmrm',array('id','backlog_id','mtw_registration_certificate','is_renew','is_amend'));
				$upload_certificates_id		-> condition('lmrm.user_id',$userid);
				$upload_certificates_file	= $upload_certificates_id-> execute()->fetchAssoc();
				$application_id				= $upload_certificates_file['id'];
				$backlog_id					= $upload_certificates_file['backlog_id'];
				
				if(!empty($backlog_id) && $backlog_id!='' && $backlog_id!=0){
						$act_wise_link 				= 'mtw-backlog-renewal/';
						$act_wise_view_link 		= 'mtw-backlog-renewal/';
						$act_name					= 'MTW RENEWAL';
				}
				
				if($upload_certificates_file['is_renew']==1){
					$act_name					= 'MTW RENEWAL';
				}
				if($upload_certificates_file['is_amend']==1){
					$act_name					= 'MTW AMENDMENT';
				}
				
				// get renewal data
				$getRenewalQuery			= db_select('l_mtw_registration_renewal','lmrr');	
				$getRenewalQuery			->fields('lmrr',array('renewal_ref_number','renewal_date','expiry_date','mtw_registration_certificate'));
				$getRenewalQuery			->condition('lmrr.application_id',$application_id);
				$getRenewalQuery			->condition('lmrr.status','1');
				$getRenewalExe				= $getRenewalQuery->execute();
				if($getRenewalExe->rowCount()>0){
					
					$getRenewalResult		= $getRenewalExe->fetchObject();
					$upload_renewal_certificates_file	= $getRenewalResult->mtw_registration_certificate;
					
					$download_certificate_link 	= $GLOBALS['base_url'].'/sites/default/files/upload/MTW_Registration_Certificate/'.$upload_renewal_certificates_file;
				}else{
					$download_certificate_link 	= $GLOBALS['base_url'].'/sites/default/files/upload/MTW_Registration_Certificate/'.$upload_certificates_file['mtw_registration_certificate'];
				}
				$renewalLink					= 'mtw-renewal/'.encryption_decryption_fun('encrypt', $data->id);
			}
			
			/*** FOR INTER-STATE MIGRANT WORKERS  ***/
			
			if($data->act_id == 4){
				
				$act_name					= 'ISMW REG';				
				$act_wise_view_link 		= 'ismw_application/view/';
				$act_wise_link 				= 'ismw_application/default/';
				
				/** Fetch Certificate of ISMW *****/
				
				$ismw_certificates_id 		= db_select('file_managed','fm');
				$ismw_certificates_id		-> fields('fm',array());
				$ismw_certificates_id		-> condition('fid',$data->certificates_fid);
				$ismw_certificates_file		= $ismw_certificates_id-> execute()->fetchAssoc();
				
				if(!empty($ismw_certificates_file)){ 
				
					  $url									= explode('//',$ismw_certificates_file['uri']);
					  $upload_certificates_file_url			= $url[1];
					  $upload_certificates_file_name		= $ismw_certificates_file['filename'];
				}
				
				$download_certificate_link 	= $GLOBALS['base_url'].'/sites/default/files/'.$upload_certificates_file_url;	
				
			}
			
			if(!empty($data->application_date)){
				
				$application_date 			= date('dS M Y', strtotime($data->application_date));	
			}else{
				$application_date 			= '';
			}
			
			
			/**** ACTION LINKS START ****/
			
			if( trim($data->status) == "I" && !empty($data->final_submit_status) && !empty($data->registration_number) ){	
					
				$registration_number 	= '<strong>'.$data->registration_number.'</strong><br>'.date('dS M Y', strtotime($data->registration_date));
				$view_link				= l($view, $act_wise_link.encryption_decryption_fun('encrypt', $data->id), array('html' => TRUE));
				$download_certificate  .= '<br>'.l('<i class="fa fa-chevron-circle-right"></i> Download Certificate',$download_certificate_link, array('attributes' => array('target' => '_blank'), 'html' => TRUE));
				
				if($data->act_id == 1){
					
					if(!empty($data->amendment_parent_id) && $data->amendment_parent_id!='' && $counter== 1){
						
						$form_v_link	.= '<br>'. l('<i class="fa fa-chevron-circle-right"></i> Download Form-V', 'view-contractors-form-v/'.encryption_decryption_fun('encrypt', $data->id), array('html' => TRUE));
						
					}elseif($data->amendment_parent_id=='' && empty($data->amendment_parent_id ) && $counter== 1){
						
						$form_v_link   .= '<br>'. l('<i class="fa fa-chevron-circle-right"></i> Download Form-V', 'view-contractors-form-v/'.encryption_decryption_fun('encrypt', $data->id), array('html' => TRUE));
						
					}else{
						
						$form_v_link	= '<br><i class="fa fa-chevron-circle-right"></i> ';
						$form_v_link   .= '<span style="color:#aeaeae;"> Download Form-V</span>';
					}
					
				}else if($data->act_id == 3){
					
					$form_v_link	.= '<br>'. l('<i class="fa fa-chevron-circle-right"></i> Renewal', $renewalLink,array('html' => TRUE, 'attributes' => array('target' => '_blank'),'html' => TRUE));
					
				}else if($data->act_id == 4){
					
					$form_v_link	.= '<br>'. l('<i class="fa fa-chevron-circle-right"></i> Download Form-VI', '/ismw_application/contractor_details/'.encryption_decryption_fun('encrypt', $data->id), array('html' => TRUE, 'attributes' => array('target' => '_blank'),'html' => TRUE));
				}
				
          	}else{
				$registration_number = 'UNDER PROCESS';
			}
			
			
			if( $data->status == "R" && !empty($data->final_submit_status)){
				          		
				$view_link				= l($view, $act_wise_link.encryption_decryption_fun('encrypt', $data->id), array('html' => TRUE));
				$remark_link	 		= '<br>'.l('<i class="fa fa-chevron-circle-right"></i> View Remarks', 'view-remark/'.encryption_decryption_fun('encrypt', $data->id).'/'.encryption_decryption_fun('encrypt', $data->act_id), array('html' => TRUE));
				
				$download_certificate	= '<br><i class="fa fa-chevron-circle-right"></i> ';
				$download_certificate  .= '<span style="color:#aeaeae;"> Download Certificate</span>';
				
          	}
			
			if((!empty($data->final_submit_status)) && trim($data->status) != "B"){
								
				$view_link	= l($view, $act_wise_view_link.encryption_decryption_fun('encrypt', $data->id), array('html' => TRUE));
				
			}
			
			
			$rows[] = array(
								array('data' =>	$counter, 'align' => 'left', 'class' => array('')),
								array('data' => $registration_number, 'align' => 'left', 'class' => array('')),
								array('data' =>	$data->e_name, 'align' => 'left', 'class' => array('')),
								array('data' =>	'<strong>'.$act_name.'</strong>', 'align' => 'left', 'class' => array('')),
								array('data' =>	get_user_application_status($data->status), 'align' => 'left', 'style' => array('text-align:center;')),					
								array('data' =>	$view_link.$remark_link.$download_certificate.$payment_details, 'align' => 'left', 'class' => array(''))
						  );	
			
		}
	}
	

	$variables = array(
	  		'attributes' 		=> array('class' => array('table table table-striped table-responsive dashboard-table')), 
	  		'header' 			=> $header,
	  		'rows'				=> $rows,
			'empty' 			=> t("No data found!")
	  );
	
	
	$output = theme('table', $variables).theme('pager');
	
	return $output;
	
	
	
}

