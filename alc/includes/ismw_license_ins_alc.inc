<?php 
function ismw_license_list_ins_alc($form, &$form_state,$licenseType='',$act_id=''){

	global $base_root, $base_path, $user;
	
	$actId 	= 	encryption_decryption_fun('decrypt', $act_id);
	
	if($actId!='' && $actId!=0){
		$form['ismw_application_lists']['markup_data_0'] = array(
															'#type' 	=> 'fieldset'
															);	
			
		$form['ismw_application_lists']['markup_data_1'] = array(
															'#prefix' 	=> '<div class="row"><div class="col-md-12"><div class="box box-primary"><div class="box-body">',
															'#suffix' 	=> '</div></div></div></div>',
															'#markup' 	=> ismw_application_license_lists($actId),			
														);
	}else{
		drupal_set_message('Invalid Response','error');
	}
		
	return $form;
}

function ismw_application_license_lists($actId=''){
	
	global $base_root, $base_path, $user;
	
 	$user_id				=	$user->uid;
	
	$fetch_users_role		=  db_select('users_roles', 'ur');
	$fetch_users_role		-> leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role		-> fields('ro', array('rid'));
	$fetch_users_role		-> condition('ur.uid', $user_id);
	$fetch_users_role_data 	=  $fetch_users_role->execute()->fetchAssoc();

	$user_role				= $fetch_users_role_data['rid']; 
	
	$get_area_codes			=	db_select('l_customuser_relation_address', 'lcra');
	$get_area_codes			->	fields('lcra',array('sub_div_code','district_code','block_code'));
	$get_area_codes			->	condition('lcra.user_id',$user_id);
	$get_area_codes_result 	= 	$get_area_codes->execute()->fetchAssoc();
	
	if(!empty($get_area_codes_result)){
	
		$block_code 			= $get_area_codes_result['block_code'];
		$subdivision_code		= $get_area_codes_result['sub_div_code'];
		
		
		$get_ismw_licenseList 	= db_select('l_interstate_license_master', 'license_master');
		if($actId == 42){
			$get_ismw_licenseList 	->leftJoin('l_interstate_contractor_master', 'con_master', 'con_master.form_six = license_master.form_six');	
			$get_ismw_licenseList 	->leftJoin('l_interstate_workman_master', 'ismw_master', 'ismw_master.id = license_master.pe_application_id');	
			$get_ismw_licenseList	->fields('license_master',array('id','est_name','application_date','license_date','identification_number','license_number','final_submit_status','status','form_six','pe_application_id','pe_user_id'));
			//$get_ismw_licenseList	->fields('con_master',array('name_of_contractor'));
			$get_ismw_licenseList	->fields('ismw_master',array('e_name','registration_number','registration_date'));
			$get_ismw_licenseList	->condition('license_master.licence_type','EMP');
		}else{
			$get_ismw_licenseList	->fields('license_master',array('id','est_name','form_six','pe_registration_number','pe_reg_date','application_date','license_date','identification_number','license_number','final_submit_status','status','form_six_rec','pe_application_id','pe_user_id','con_subdiv','con_areacode','recruited_subdiv','recruited_areatype_code'));
			$get_ismw_licenseList	->condition('license_master.licence_type','REC');
		}
		if($actId == 42){ 
			if ($user_role == 4 ){		
			 $get_ismw_licenseList	->condition('license_master.worksite_subdiv',$subdivision_code);		
			}else if ($user_role == 7){		
			 $get_ismw_licenseList	->condition('license_master.worksite_areacode',$block_code);
			
			}
		}else{
			if ($user_role == 4 ){		
			 $get_ismw_licenseList	->condition('license_master.recruited_subdiv',$subdivision_code);		
			}else if ($user_role == 7){		
			 $get_ismw_licenseList	->condition('license_master.recruited_areatype_code',$block_code);
			
			}
		}
		$get_ismw_licenseList	->condition('license_master.applied_for','L');
		$get_ismw_licenseList	->condition('license_master.final_submit_status','P');
		$get_ismw_licenseList	->orderBy('license_master.id','DESC');
		$get_ismwLicenseEmp_result  = $get_ismw_licenseList->execute();
	

		$header = array(
			  array('data' => 'Sl', 'field' => 'slno','width' => '5%'),
			  array('data' => 'FORM-VI', 'field' => ''),
			  array('data' => 'Contractor Details', 'field' => ''),
			  array('data' => 'Establishment Details', 'field' => 'e_name'),
			  array('data' => 'License Details', 'field' => ''),
			  array('data' => 'Apply Date', 'field' => 'application_date'),
			  array('data' => 'Status', 'field' => 'status'),
			  array('data' => 'action', 'field' => 'actions'),
		  );
	
		if( $get_ismwLicenseEmp_result->rowCount() > 0 ){ 

			
			// if($user_id == 1094){
            // print_r($get_ismwLicenseEmp_result->fetchAll());die;
			// }
			$counter			= 0;  
			$view_img			= '<span class="badge bg-blue"><i class="fa fa-eye"></i> View Details';
	
			while($data = $get_ismwLicenseEmp_result->fetchObject()) { 
		 
				$counter++;
				
				/*** DEEMED UPDATE***/
					
			// $info = get_final_submit_data_ismw(encryption_decryption_fun('encrypt', 42), encryption_decryption_fun('encrypt', $data->id));			
			// if(!empty($info) && $info['remark_type'] == 'S'){					
			// 	$final_submit_date = $info['remark_date'];
			// 	$today = time();						
			// 	$datediff = $today - $final_submit_date;	
			// 	$diff = round($datediff / (60 * 60 * 24));
				
			// 	if($diff > 30){  
			// 		common_deemed_update_status(encryption_decryption_fun('encrypt', 42),encryption_decryption_fun('encrypt', $data->id),encryption_decryption_fun('encrypt', $data->user_id));
					
			// 	}
			// }

				if ($user_role == 4 ){
					if($actId == 42){
						$view_link			= 'ismwlicenseview-employment/'.encryption_decryption_fun('encrypt', $data->id);
						$registrationLink	= 'ismwapplications-view/'.encryption_decryption_fun('encrypt', $data->pe_application_id).'/'.encryption_decryption_fun('encrypt', $data->pe_user_id);
					}else if($actId == 43){
						$view_link			= 'ismwlicenseview-recruitment/'.encryption_decryption_fun('encrypt', $data->id);
						$registrationLink	= '';
					}
				}else if($user_role == 7){
					if($actId == 42){	
						$view_link 			= 'ismwlicenseinspector-employment/'.encryption_decryption_fun('encrypt', $data->id);
						$registrationLink	= 'inspector/ismwapplications-view/'.encryption_decryption_fun('encrypt', $data->pe_application_id).'/'.encryption_decryption_fun('encrypt', $data->pe_user_id);
					}else if($actId == 43){
						$view_link 			= 'ismwlicenseinspector-recruitment/'.encryption_decryption_fun('encrypt', $data->id);
						$registrationLink	= '';
					}
				}else{
						$view_link= '';
				}
				/** Application Date ***/
				
				if(!empty($data->application_date)){
					$apply_date 		= date('dS M, Y', strtotime($data->application_date));
					
					/*** Escalation ****/
					
					$application_date 		= strtotime($data->application_date); 
					$last_week_date 		= strtotime('-7 day');
					
				}
				
				/** Status Description ***/
				
				if( trim($data->status) == "0" ){$license_number = 'NEW APPLICATION';}
				if( trim($data->status) == "R" ){$license_number = 'REJECT';}
				
				if(!empty($data->license_number)){
					if(!empty($data->license_date)){
						$license_date 		= date('dS M, Y', strtotime($data->license_date));
					}else {
						$license_date		= '';
					}
					$license_number = $data->license_number.'<br/>Date: '.$license_date;
				}else{
					$license_number = 'UNDER PROCESS';	
				}
				
				if($data->status == '0'){
					$actionStatus 	= '<span class="pending" title="Applied"></span>';
				}else{
					$actionStatus 	= get_user_application_status($data->status,$actId);
				}
				
				/** ACTS SPECIFIC DATA **/
				if($actId == 42){
					$formSix = '00'.$data->form_six;
					$contractorName = $data->est_name;
					$regDate = 'REG NO: '.l($data->registration_number,$registrationLink,array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'<br/>Date: <strong>'.date('dS M, Y', strtotime($data->registration_date));
				}elseif($actId == 43){
					$formSix = $data->form_six_rec.'<br/><strong>REF : 00'.$data->form_six.'</strong>';
					$contractorName = $data->est_name;
					$regDate ='REG NO: '.$data->pe_registration_number.'<br/>Date: <strong>'.date('dS M, Y', strtotime($data->pe_reg_date)).'</strong>';
				}
		
				if( ( $application_date < $last_week_date ) && trim($data->status) == "0"){ 				 
					
					$rows[] = array(
							array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
							array('data' => $formSix.$sl , 'align' => 'left', 'class' => array('even')),
							array('data' => $contractorName, 'align' => 'left', 'class' => array('odd')),
							array('data' => $regDate.'</strong>', 'align' => 'left', 'class' => array('odd')),
							array('data' => '<strong>'.$license_number.'</strong>', 'align' => 'left', 'class' => array('even')),
							array('data' => $apply_date, 'align' => 'left', 'class' => array('odd')),
							array('data' => $actionStatus, 'align' => 'left', 'class' => array('even')),
							array('data' => l($view_img, $view_link, array('html' => TRUE, 'attributes' => array('target' => '_blank'))), 'align' => 'left', 'class' => array('odd')),
							
					);
					
					
					
				}else{
					
					$rows[] = array(
							array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
							array('data' => $formSix , 'align' => 'left', 'class' => array('even')),
							array('data' => $contractorName, 'align' => 'left', 'class' => array('odd')),
							array('data' => $regDate,array('html' => TRUE, 'attributes' => array('target' => '_blank')), 'align' => 'left', 'class' => array('odd')),
							array('data' => '<strong>'.$license_number.'</strong>', 'align' => 'left', 'class' => array('even')),
							array('data' => $apply_date, 'align' => 'left', 'class' => array('odd')),
							array('data' => $actionStatus, 'align' => 'left', 'class' => array('even')),
							array('data' => l($view_img, $view_link, array('html' => TRUE, 'attributes' => array('target' => '_blank'))), 'align' => 'left', 'class' => array('odd')),
							
					);
				
				}
	
			}
		}
  
  }
  
  $variables = array(
	  		'attributes' 		=> array('class' => array('table table-striped table-responsive admin-custom-table')), 
	  		'header' 			=> $header,
	  		'rows'				=> $rows,
			'empty' 			=> t("Application not available")
	  );
	
	
  $output = theme('datatable', $variables);
	
  return $output;		
  
}

