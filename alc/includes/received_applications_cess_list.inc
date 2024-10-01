<?php 

function received_applications_cess_list($form, &$form_state,$actid='', $status=''){
		
	global $base_root, $base_path, $user;
	
	$active = $activepending = $activeforward = $activefinalsubmit = $activeissued = $activerejected = '';
	$act_id	= encryption_decryption_fun('decrypt', $actid);
	
	if($status == 'pending'){
		$activepending = 'active';
	}else if($status == 'forward'){
		$activeforward = 'active';
	}else if($status == 'finalsubmit'){
		$activefinalsubmit = 'active';
	}else if($status == 'issued'){
		$activeissued = 'active';
	}else if($status == 'rejected'){
		$activerejected = 'active';
	}
	
	if($status == ''){
		$header_title = 'All';
	}else{
		$header_title = ucfirst($status);
	}
	
	$form['cess']['markup_data_1'] = array(
										'#prefix'	=> '<div class="row"><div class="col-md-12"><div class="box box-primary"><div class="box-body">',	
										'#suffix'	=> '</div></div></div></div>',	
										'#markup' 	=> getCessApplications($act_id,$status)
									);	
		
	return $form;
}

function getCessApplications($act_id,$status){
	global $base_root, $base_path, $user;
	
	$user_id 				= $user->uid;
	$counter 				= 0; 
	$finalSubmitStatus		= ''; 
	$view_img 				= '<span class="badge bg-blue"><i class="fa fa-eye"></i> View Details';
	
	$fetch_users_role		=  db_select('users_roles', 'ur');
	$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role->fields('ro', array('rid'));
	$fetch_users_role->condition('ur.uid', $user_id);
	$fetch_users_role_data 	=  $fetch_users_role->execute()->fetchAssoc();

	$user_role				= $fetch_users_role_data['rid']; 
	
	$get_area_codes			=	db_select('l_customuser_relation_address', 'lcra');
	$get_area_codes->fields('lcra',array('sub_div_code','district_code','block_code'));
	$get_area_codes->condition('lcra.user_id',$user_id);
	$get_area_codes_result 	= 	$get_area_codes->execute()->fetchAssoc();
	
	$block_code 			= $get_area_codes_result['block_code'];
	$subdivision_code		= $get_area_codes_result['sub_div_code'];
	
	$getCessApplications 	= db_select('l_cess_application_master', 'master');
	$getCessApplications	->fields('master', array());
	$getCessApplications	->condition('master.final_submit_status','P');
	$getCessApplications	->condition('master.status','T');
	if ($user_role == 4 ){
		$getCessApplications->condition('master.loc_e_subdivision',$subdivision_code);
	}else if ($user_role == 7){
		$getCessApplications->condition('master.loc_e_areatype_code',$block_code);
	}
	$getCessApplications	->orderBy('master.id','DESC');
	$cessList 				= $getCessApplications->execute();
	
	$header = array(
					  array('data' => 'Sl. No.', 'width' => '8%', 'data-orderable' => false),
					  array('data' => 'Id. Number', 'width' => '12%'),
					  //array('data' => 'Reg. No. & Date', 'width' => '15%'),
					  array('data' => 'Establishment Name', 'field' => 'e_name'),
					  array('data' => 'Apply Date', 'width' => '14%'),
					  array('data' => 'Status', 'width' => '7%'),
					  array('data' => 'action', 'width' => '8%'),
					);
					
	if($cessList->rowCount() > 0 ){
		foreach($cessList as $data){
			$counter++;
			$view_details = '';
			
			if(!empty($data->application_date)){
				$apply_date 			= date('dS M, Y', strtotime($data->application_date));
				
				/*** Escalation ****/
				$application_date 		= strtotime($data->application_date); 
				$last_week_date 		= strtotime('-7 day');	
			}
			$actionStatus 				= get_user_application_status($data->status);
			$view_details  				= array('data' => l($view_img, 'cess_viewapplications/'.encryption_decryption_fun('encrypt', $data->id).'/'.encryption_decryption_fun('encrypt', $data->user_id), array('html' => TRUE)), 'align' => 'left', 'class' => array('exp_pendng'));
			
			if ( ($application_date < $last_week_date ) && (trim($data->status) == "0")){
				$rows[] = array(
						array('data' => $counter, 'align' => 'left', 'class' => array('')),
						array('data' => $data->identification_number.'<br/>'.$apply_category, 'align' => 'left', 'class' => array('exp_pendng')),
						//array('data' => '<strong>'.$registration_number.'</strong><br/>'.$issue_date, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => $data->e_name, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => $apply_date, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => $actionStatus, 'align' => 'left'),
						$view_details
						);
			}else{
				$rows[] = array(
						array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
						array('data' => $data->identification_number.'<br/>'.$apply_category, 'align' => 'left', 'class' => array('even')),
						//array('data' => '<strong>'.$registration_number.'</strong><br/>'.$issue_date, 'align' => 'left', 'class' => array('even')),
						array('data' => $data->e_name, 'align' => 'left', 'class' => array('odd')),
						array('data' => $apply_date, 'align' => 'left', 'class' => array('even')),
						array('data' => $actionStatus, 'align' => 'left', 'class' => array('odd')),
						$view_details
						);
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