<?php 

function received_applications_list($form, &$form_state,$actid='', $status='', $type = ''){
		
	global $base_root, $base_path, $user;
	
	$active = $activepending = $activeforward = $activefinalsubmit = $activeissued = $activerejected = $activerectification = '';
	$act_id	= encryption_decryption_fun('decrypt', $actid);
	//echo $act_id.'--'.$status.'--'.$type; exit;
	//if(empty($status)){$active = 'active';}	
	//if($status == 'pending'){$activepending = 'active';}else{$activepending = '';}
	//if($status == 'forward'){$activeforward = 'active';}else{$activeforward = '';}
	//if($status == 'finalsubmit'){$activefinalsubmit = 'active';}else{$activefinalsubmit = '';}
	//if($status == 'issued'){$activeissued = 'active';}else{$activeissued = '';}
	//if($status == 'rejected'){$activerejected = 'active';}else{$activerejected = '';}
	
	$active = '';
	$activepending = '';
	$activerectification='';
	$activeforward = '';
	$activefinalsubmit = '';
	$activeissued = '';
	$activerejected = '';
	
	
	if($status == '' ){
		$active = 'active';
	}else if($status == 'pending'){
		$activepending = 'active';
	}else if($status == 'rectification'){
		$activerectification = 'active';
	}else if($status == 'forward'){
		$activeforward = 'active';
	}else if($status == 'finalsubmit'){
		$activefinalsubmit = 'active';
	}else if($status == 'issued'){
		$activeissued = 'active';
	}else if($status == 'rejected'){
		$activerejected = 'active';
	}else if($status == 'renewal'){
		$active = 'active';
	}
		
	if($status == ''){
		$header_title = 'All';
	}else{
		$header_title = ucfirst($status);
	}
	
	if($status == 'renewal'){
		$type = 'renewal'; //echo $act_id.'--'.$status.'--'.$type; exit;
		$calling_function = get_application_lists($act_id,$status,$type);
	}else if($type == 'renewal'){
		$calling_function = get_application_lists($act_id,$status,$type);
	}else{
		$calling_function = get_application_lists($act_id,$status);
	}
	
	if(is_numeric($act_id) && $act_id!=0 && $act_id!=''){
		
		if($act_id == '3' && ($status == 'renewal' || $type =='renewal')){  $type = 'renewal';
			$form['application_lists']['markup_data'] = array(
				'#type'  => 'markup',
				'#markup' => '<div class="nav-tabs-custom">
								<ul class="nav nav-tabs">
									<li class="'.$active.'">'.l('All', 'receivedapplications/'.$actid.'/'.$type).'</li>
									<li class="'.$activepending.'">'.l('Pending', 'receivedapplications/'.$actid.'/pending'.'/'.$type).'</li>
									<li class="'.$activeforward.'">'.l('Forward', 'receivedapplications/'.$actid.'/forward'.'/'.$type).'</li>
									<li class="'.$activefinalsubmit.'">'.l('Final Submit', 'receivedapplications/'.$actid.'/finalsubmit'.'/'.$type).'</li>												
									<li class="'.$activeissued.'">'.l('Issued', 'receivedapplications/'.$actid.'/issued'.'/'.$type).'</li>
									<li class="'.$activerejected.'">'.l('Rejected', 'receivedapplications/'.$actid.'/rejected'.'/'.$type).'</li>
								</ul>
								<div class="tab-content">
									<div class="tab-pane active" id="fa-icons">
										<section id="new"><div class="feedback-scroll">'.$calling_function.'</div></section></div>'
			);
		}else{
			$form['application_lists']['markup_data'] = array(
				'#type'  => 'markup',
				'#markup' => '<div class="nav-tabs-custom">
								<ul class="nav nav-tabs">
									<li class="'.$active.'">'.l('All', 'receivedapplications/'.$actid).'</li>
									<li class="'.$activepending.'">'.l('Pending', 'receivedapplications/'.$actid.'/pending').'</li>
									<li class="'.$activerectification.'">'.l('Sent Back for Rectification', 'receivedapplications/'.$actid.'/rectification').'</li>
									<li class="'.$activeforward.'">'.l('Forward', 'receivedapplications/'.$actid.'/forward').'</li>
									<li class="'.$activefinalsubmit.'">'.l('Final Submit', 'receivedapplications/'.$actid.'/finalsubmit').'</li>												
									<li class="'.$activeissued.'">'.l('Issued', 'receivedapplications/'.$actid.'/issued').'</li>
									<li class="'.$activerejected.'">'.l('Rejected', 'receivedapplications/'.$actid.'/rejected').'</li>
								</ul>
								<div class="tab-content">
									<div class="tab-pane active" id="fa-icons">
										<section id="new"><div class="feedback-scroll">'.$calling_function.'</div></section></div>'																	
			);
		}		
			
	}else{
		drupal_set_message('Invalid Response',error);
	}
	
	
	return $form;  
}

function get_application_lists($act_id='',$status='', $type =''){
	global $base_root, $base_path, $user;
	$table	= $apply_category = $unit_name= '';
 	$user_id = $user->uid;
	$counter = 0; 
	$finalSubmitStatus = ''; 
	$view_img = '<span class="badge bg-blue"><i class="fa fa-eye"></i> View Details';
	$getdatafields = array();
	
	$fetch_users_role =  db_select('users_roles', 'ur');
	$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role->fields('ro', array('rid'));
	$fetch_users_role->condition('ur.uid', $user_id);
	$fetch_users_role_data 	=  $fetch_users_role->execute()->fetchAssoc();

	$user_role = $fetch_users_role_data['rid']; 
	
	$get_area_codes =	db_select('l_customuser_relation_address', 'lcra');
	$get_area_codes->fields('lcra',array('sub_div_code','district_code','block_code','custom_area_jurisdiction','is_custom_area_jurisdiction', 'rlo_code', 'lwfc_code'));
	$get_area_codes->condition('lcra.user_id', $user_id);
	$get_area_codes_result 	= 	$get_area_codes->execute()->fetchAssoc(); // echo '<pre>'; print_r($get_area_codes_result); die;
	
	if(!empty($get_area_codes_result)){	
		$block_code = $get_area_codes_result['block_code'];
		$subdivision_code = $get_area_codes_result['sub_div_code'];
		$customjurisdiction = $get_area_codes_result['custom_area_jurisdiction']; 
		$iscustom_juris	= $get_area_codes_result['is_custom_area_jurisdiction'];
		$rlo_code = $get_area_codes_result['rlo_code'];
		$lwfc_code = $get_area_codes_result['lwfc_code'];
		
		if($customjurisdiction=='BMCNS' && $iscustom_juris == 'Y'){
			$block_code_arr = db_select('l_customuser_reletion_services_wise_user_settings','cra')
			->fields('cra',array('block_code'))
			->condition('cra.user_id', $user_id)
			->execute()->fetchObject()->block_code;
			$block_code_arr = explode(',', $block_code_arr); 
		}
		
		if($act_id!=0 && $act_id!='' && $act_id == 1){
			$table = 'l_clra_registration_master';			
			$fieldname = 'loc_e_subdivision';				
			$fieldvalue = $subdivision_code;
			if($rlo_code!= ''){
				$fieldname = 'rlo_code';
				$fieldvalue = $rlo_code;
			}
			$blockfield = 'name_areatype';
			$getdatafields = array('id','user_id','identification_number','registration_number','final_submit_status','status','application_date','e_name','loc_e_subdivision','name_areatype','registration_date','amendment_parent_id','backlog_id','amendment_date','deemed');
			$finalSubmitStatus	= 'final_submit_status';

		}else if($act_id!=0 && $act_id!='' && $act_id == 2){

			$table			= 'l_bocwa_registration_master';			
			$fieldname 		= 'loc_e_subdivision';
			$fieldvalue = $subdivision_code;
			
			if($rlo_code!= ''){
				$fieldname = 'rlo_code';
				$fieldvalue = $rlo_code;
			}
			
			$blockfield 	= 'loc_e_areatype_code';
			$getdatafields  = array('id','user_id','identification_number','registration_number','final_submit_status','status','application_date','e_name','loc_e_subdivision','loc_e_areatype_code','registration_date','amendment_parent_id','deemed');
			$finalSubmitStatus	= 'final_submit_status';


		}else if($act_id!=0 && $act_id!='' && $act_id == 3){
			$table			= 'l_mtw_registration_master';
			$fieldname 		= 'mtw_loc_subdivision';
			$fieldvalue = $subdivision_code;
			
			/*if($rlo_code!= ''){
				$fieldname = 'rlo_code';
				$fieldvalue = $rlo_code;
			}*/
			
			$blockfield 	= 'mtw_loc_areatype_code';
			$getdatafields  = array('id','user_id','identification_number','registration_number','final_submit_status','status','application_date','mtw_name','mtw_loc_subdivision','mtw_loc_areatype_code','registration_date','is_renew','is_amend','deemed');
			$finalSubmitStatus	= 'final_submit_status';
			
		}else if($act_id!=0 && $act_id!='' && $act_id == 4){
			$table			= 'l_interstate_workman_master';
			$fieldname 		= 'loc_e_subdivision';
			$fieldvalue = $subdivision_code;
			
			/*if($rlo_code!= ''){
				$fieldname = 'rlo_code';
				$fieldvalue = $rlo_code;
			}*/
			
			$blockfield 	= 'loc_e_areatype_code';
			$getdatafields 	= array('id','user_id','identification_number','registration_number','final_submit_status','status','application_date','e_name','loc_e_subdivision','loc_e_areatype_code','registration_date','deemed');
			$finalSubmitStatus	= 'final_submit_status';
		}
		
		$getApplications 	= db_select($table, 'master');
		$getApplications->fields('master', $getdatafields);
		
		if(($iscustom_juris == 'Y') && (!empty($block_code_arr))){			
			$getApplications->condition('master.'.$blockfield, $block_code_arr, 'IN');
		}else{			
			// $getApplications->condition('master.'.$fieldname, $subdivision_code);	
			$getApplications->condition('master.'.$fieldname, $fieldvalue);				
		}
		$getApplications->condition('master.'.$finalSubmitStatus,'P');
		
		if($status == 'pending'){
			if($type != ''){ 
				$getApplications->condition('master.status','RN','=');
			}else{
				//$getApplications->condition('master.status', array('0', 'F', 'S'), 'IN');
				$getApplications->condition('master.status', array('0', 'S'), 'IN');
			}
		}elseif($status == 'rectification'){
			$getApplications->condition('master.status','B');
		}elseif($status == 'forward'){
			$getApplications->condition('master.status','F');
		}elseif($status == 'finalsubmit'){
				$getApplications->condition('master.status','S');
		}elseif($status == 'issued'){
			$getApplications->condition('master.status','I');
		}elseif($status == 'rejected'){
			$getApplications->condition('master.status','R');
		}else{
			$getApplications->condition('master.status', array('V','T','VA','U'), 'IN');
		}
		
		
		if($act_id == 3 && $type != ''){
			$getApplications->condition('master.is_renew',1,'=');
		}else if($act_id == 3 && $type == ''){
			$getApplications->condition('master.is_renew',0,'=');
		}
		if($user->uid == 1094 && $act_id == 1){
			// $arrin = array(64, 70, 11181, 32778, 23247);
			// $getApplications->condition('master.user_id', $arrin, 'IN');
	    }
		//$getApplications->range(0,10);
		$getApplications->orderBy('master.id','DESC');
		$applicationList = $getApplications->execute();
		//->fetchAll(); 
		//print_r($applicationList); exit;

				 
		$header = array(
					  array('data' => 'Sl. No.', 'width' => '6%', 'data-orderable' => false),
					  array('data' => 'Id. Number', 'width' => '12%'),
					  array('data' => 'Reg. No. & Date', 'width' => '12%'),
					  array('data' => 'B/M/C/NA/SEZ', 'width' => '12%'),
					  array('data' => 'Establishment Name', 'field' => 'e_name'),
					  array('data' => 'Apply Date', 'width' => '10%'),
					  array('data' => 'Status', 'width' => '7%'),
					  array('data' => 'action', 'width' => '8%'),
					);

		if($applicationList->rowCount() > 0 ){ 
		
			foreach($applicationList as $data){
				 
				$counter++;
				$view_details = '';
				
				if(!empty($data->application_date)){
					$apply_date 			= date('dS M, Y', strtotime($data->application_date));
					
					/*** Escalation ****/
					$application_date 		= strtotime($data->application_date); 
					$last_week_date 		= strtotime('-7 day');	
				}
				if(!empty($data->registration_date) && !empty($data->registration_number)){
					$issue_date 			= date('dS M, Y', strtotime($data->registration_date));
					$registration_number 	= $data->registration_number;
						
				}else {
					$issue_date				= '';
					$registration_number 	= 'NEW APPLICATION';
				}
				
				
					
				$actionStatus = get_user_application_status($data->status); 				
					
				/** ACTS SPECIFIC ***/
					
				if($act_id!=0 && $act_id!='' && $act_id == 1){
					
					$blockMunData = db_query('select block_mun_name from block_mun_master where block_code='.$data->name_areatype)->fetchAssoc();
					
					$blockMunName = $blockMunData['block_mun_name'];
					
					// if($user->uid == 1094){echo $data->name_areatype.'------<br>';}	
					
					
					$unit_name = $data->e_name;
					
					if(!empty($backlog_rc_upload_result->backlog_clra_registration_certificate_file) && empty($data->backlog_id)){
						
						$backlog_rc_upload = db_select('l_documents_upload', 'ldu');
						$backlog_rc_upload->fields('ldu',array('backlog_clra_registration_certificate_file'));					  
						$backlog_rc_upload->condition('ldu.application_id', $data->id);
						$backlog_rc_upload->condition('ldu.act_id', 1);  
						$backlog_rc_upload = $backlog_rc_upload->execute();
						$backlog_rc_upload_result = $backlog_rc_upload->fetchObject();				  
									  
						if(!empty($backlog_rc_upload_result->backlog_clra_registration_certificate_file) && empty($data->backlog_id)){
									
							$view_details = array('data' => l($view_img, 'alc-approve-backlog-applications/'.encryption_decryption_fun('encrypt', trim($data->id)).'/'.encryption_decryption_fun('encrypt', trim($data->user_id)), array('html' => TRUE)), 'align' => 'left', 'class' => array('exp_pendng'));
						}	
						
					}elseif($data->amendment_parent_id!='' ){ 	
						$apply_category 	= '<span style="font-size:14px; color:#E08E0B">[AMENDMENT APPLICATION]</span>';
						$amended_fields		= db_select('l_clra_amendment', 'lca')
											->fields('lca', array('amendment_fields','approved_by_uid','flag'))
											->condition('lca.application_id', $data->amendment_parent_id)
											->condition('lca.applicant_uid', $data->user_id)
											->condition('lca.status', 1)
											->orderBy('id', 'DESC')
											->range(0, 1);
						$amendedField 		= $amended_fields->execute()->fetchAssoc(); 
						$amendedFieldArray 	= explode(',', $amendedField['amendment_fields']);
						
						if ((in_array("e_location", $amendedFieldArray)) && $amendedField['flag'] != 'D' ){  
							$view_details 		= array('data' => l($view_img, 'alc-amendment-approval/'.encryption_decryption_fun('encrypt', trim($data->id)).'/'.encryption_decryption_fun('encrypt', trim($act_id)), array('html' => TRUE)), 'align' => 'left', 'class' => array('exp_pendng'));
						}else {
							$view_details 		= array('data' => l($view_img, 'alc-view-amended-application/'.encryption_decryption_fun('encrypt', trim($data->id)).'/'.encryption_decryption_fun('encrypt', trim($data->user_id)), array('html' => TRUE)), 'align' => 'left', 'class' => array('exp_pendng'));
						}
					}else{
						$apply_category 	= '';
						$view_details  		= array('data' => l($view_img, 'alc-visible-applications/'.encryption_decryption_fun('encrypt', $data->id).'/'.encryption_decryption_fun('encrypt', $data->user_id), array('html' => TRUE)), 'align' => 'left', 'class' => array('exp_pendng'));
						
					}
					
					/*** DEEMED UPDATE***/
					
					$info = get_final_submit_data(encryption_decryption_fun('encrypt', 1), encryption_decryption_fun('encrypt', $data->id));			
					if(!empty($info) && $info['remark_type'] == 'S'){					
						$final_submit_date = $info['remark_date'];
						$today = time();						
						$datediff = $today - $final_submit_date;	
						$diff = round($datediff / (60 * 60 * 24));
						
						if($diff > 30){  
							common_deemed_update_status(encryption_decryption_fun('encrypt', 1),encryption_decryption_fun('encrypt', $data->id),encryption_decryption_fun('encrypt', $data->user_id));
							
						}
					}
					
					
						
				}elseif($act_id!=0 && $act_id!='' && $act_id == 2){
					
					$unit_name = $data->e_name;
					$blockMunData = db_query('select block_mun_name from block_mun_master where block_code='.$data->loc_e_areatype_code)->fetchAssoc();
					$blockMunName = $blockMunData['block_mun_name'];
					
					if($data->amendment_parent_id!='' ){
						$view_details = array('data' => l($view_img, 'alc_receivedapplications_bocwa_amendment/'.encryption_decryption_fun('encrypt', trim($data->id)).'/'.encryption_decryption_fun('encrypt', trim($data->user_id)), array('html' => TRUE)), 'align' => 'left', 'class' => array('exp_pendng'));
					}else{
						/*** DEEMED UPDATE BOCW***/
						$info = get_final_submit_data(encryption_decryption_fun('encrypt', 2), encryption_decryption_fun('encrypt', $data->id));

						if(!empty($info) && $info['remark_type'] == 'S'){
							$final_submit_date = $info['remark_date'];
							$today = time();						
							$datediff = $today - $final_submit_date;	
							$diff = round($datediff / (60 * 60 * 24));
							
							if($diff > 30){  
								common_deemed_update_status(encryption_decryption_fun('encrypt', 2),encryption_decryption_fun('encrypt', $data->id),encryption_decryption_fun('encrypt', $data->user_id));
							}
						}
						 $view_details = array('data' => l($view_img, 'alc_receivedapplications_bocwa/'.encryption_decryption_fun('encrypt', $data->id), array('html' => TRUE)), 'align' => 'left', 'class' => array('exp_pendng '));
					}	
				}else if ($act_id!=0 && $act_id!='' && $act_id == 3){
					
					$blockMunData = db_query('select block_mun_name from block_mun_master where block_code='.$data->mtw_loc_areatype_code)->fetchAssoc();
					$blockMunName = $blockMunData['block_mun_name'];
					
					if($data->is_renew == 1){
						$view_details = array('data' => l($view_img, 'alc-mtw-renewal-details/'.encryption_decryption_fun('encrypt', $data->id).'/'.encryption_decryption_fun('encrypt', $data->user_id), array('html' => TRUE)));
					}else{
						
						/*** DEEMED UPDATE***/
					
						$info = get_final_submit_data(encryption_decryption_fun('encrypt', 3), encryption_decryption_fun('encrypt', $data->id));			
						if(!empty($info) && $info['remark_type'] == 'S'){	 			
							$final_submit_date = $info['remark_date'];
							$today = time();						
							$datediff = $today - $final_submit_date;	
							$diff = round($datediff / (60 * 60 * 24));
							
							if($diff > 30){  
								common_deemed_update_status(encryption_decryption_fun('encrypt', 3),encryption_decryption_fun('encrypt', $data->id),encryption_decryption_fun('encrypt', $data->user_id));
								
							}
						}
					
						
						
						
						$view_details = array('data' => l($view_img, 'alc-application-details-mtw/'.encryption_decryption_fun('encrypt', $data->id).'/'.encryption_decryption_fun('encrypt', $data->user_id), array('html' => TRUE)), 'align' => 'left', 'class' => array('exp_pendng'));
					}
					
					$unit_name = $data->mtw_name;
					
				}elseif($act_id!=0 && $act_id!='' && $act_id == 4){
					$unit_name = $data->e_name;
					
					$blockMunData = db_query('select block_mun_name from block_mun_master where block_code='.$data->loc_e_areatype_code)->fetchAssoc();
					$blockMunName = $blockMunData['block_mun_name'];
					
					$view_details = array('data' => l($view_img, 'ismwapplications-view/'.encryption_decryption_fun('encrypt', $data->id).'/'.encryption_decryption_fun('encrypt', $data->user_id), array('html' => TRUE)), 'align' => 'left', 'class' => array('exp_pendng '));
					
					/*** DEEMED UPDATE ISMW***/
					
					$info = get_final_submit_data(encryption_decryption_fun('encrypt', 4), encryption_decryption_fun('encrypt', $data->id));

					if(!empty($info) && $info['remark_type'] == 'S'){
						$final_submit_date = $info['remark_date'];
						$today = time();						
						$datediff = $today - $final_submit_date;	
						$diff = round($datediff / (60 * 60 * 24));
						
						if($diff > 30){  
							common_deemed_update_status(encryption_decryption_fun('encrypt', 4),encryption_decryption_fun('encrypt', $data->id),encryption_decryption_fun('encrypt', $data->user_id));
						}
					}
					
					
					
				}
					
				
				
				if(($application_date < $last_week_date ) && (trim($data->status) == "0" || trim($data->status) == "RN" )){ 	
					$rows[] = array(
						array('data' => $counter, 'align' => 'left', 'class' => array('')),
						array('data' => $data->identification_number.'<br/>'.$apply_category, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => '<strong>'.$registration_number.'</strong><br/>'.$issue_date, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => $blockMunName, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => $unit_name, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => $apply_date, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => $actionStatus, 'align' => 'left'),
						$view_details						
					);
				}else{
					$rows[] = array(
						array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
						array('data' => $data->identification_number.'<br/>'.$apply_category, 'align' => 'left', 'class' => array('even')),
						array('data' => '<strong>'.$registration_number.'</strong><br/>'.$issue_date, 'align' => 'left', 'class' => array('even')),
						array('data' => $blockMunName, 'align' => 'left', 'class' => array('odd')),
						array('data' => $unit_name, 'align' => 'left', 'class' => array('even')),
						array('data' => $apply_date, 'align' => 'left', 'class' => array('odd')),
						array('data' => $actionStatus, 'align' => 'left', 'class' => array('even')),
						$view_details,					
					); 	
				} 
			} 		
		}
	}
	
	$variables = array(
					'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')), 
					'header' 	 => $header,
					'rows'		 => $rows,
					'empty' 	 => t("No data found!"));
	
	
	$output = theme('datatable', $variables);
	return $output;
}
