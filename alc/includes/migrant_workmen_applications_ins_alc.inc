<?php 
function migrant_workmen_applications_ins_alc($form, &$form_state){

	global $base_root, $base_path, $user;
	
	
	$get_all_ismw_application_lists =  ismw_application_lists();
	
	$form['ismw_application_lists']['markup_data_1'] = array(
														'#prefix'	=> '<div class="row"><div class="col-md-12"><div class="box box-primary"><div class="box-body">',	
														'#suffix'	=> '</div></div></div></div>',	
														'#markup' 	=> $get_all_ismw_application_lists				
													);	
		
	return $form;
}

function ismw_application_lists(){
	
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
		
		
		$get_ismw_application 	= db_select('l_interstate_workman_master', 'ismw_master');
		$get_ismw_application	->fields('ismw_master',array('id','e_name','application_date','registration_date','identification_number','registration_number','final_submit_status','status','user_id'));
		
		if ($user_role == 4 ){
		
		$get_ismw_application	->condition('ismw_master.loc_e_subdivision',$subdivision_code);
		
		}else if ($user_role == 7){
		
		$get_ismw_application	->condition('ismw_master.loc_e_areatype_code',$block_code);
		
		}/*else{
		
		drupal_set_message('You are not authorized to access this page',error);
		
		}*/
		
		if($user->uid == 1094){
			$get_ismw_application->condition('ismw_master.user_id', 64);
	    }
		$get_ismw_application	->condition('ismw_master.status','I', '!=');
		$get_ismw_application	->condition('ismw_master.final_submit_status','P');
		$get_ismw_application	->orderBy('ismw_master.id','DESC');
		$get_ismw_application_result = $get_ismw_application->execute();
	
	}
	
	
	

	$header = array(
		  array('data' => 'Sl. No', 'field' => 'slno','width' => '6%'),
		  array('data' => 'Identification No.', 'field' => '','width' => '17%'),
		  array('data' => 'Registration No. & Date', 'field' => '','width' => '15%'),
		  array('data' => 'Establishment Name', 'field' => 'e_name'),
		  array('data' => 'Apply Date', 'field' => 'application_date', 'width' => '10%'),
		  array('data' => 'Status', 'field' => 'status', 'width' => '7%'),
		  array('data' => 'action', 'field' => 'actions', 'width' => '8%'),
	  );
	
  if( $get_ismw_application_result->rowCount() > 0 ){ 
  
  	$counter			= 0;  
	$view_img			= '<span class="badge bg-blue"><i class="fa fa-eye"></i> View Details<div>';
		
		while($data = $get_ismw_application_result->fetchObject()) {  
			 
			 $counter++;
			 
			if ($user_role == 4 ){
				
				$view_link	= 'ismwapplications-view/'.encryption_decryption_fun('encrypt', $data->id).'/'.encryption_decryption_fun('encrypt', $data->user_id);
			
			
			}else if($user_role == 7){
				
				$view_link = 'inspector/ismwapplications-view/'.encryption_decryption_fun('encrypt', $data->id).'/'.encryption_decryption_fun('encrypt', $data->user_id);
				
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
			
			/** Registration Date ***/
			
			if(!empty($data->registration_date)){
				$issue_date 		= date('dS M, Y', strtotime($data->registration_date));
			}else {
				$issue_date			= '';
			}
			
			/** Status Description ***/
			
			if( trim($data->status) == "0" ){
				$registration_number = 'NEW APPLICATION';
          	}
			if( trim($data->status) == "R" ){
				$registration_number = 'REJECT';
          	}
			if(!empty($data->registration_number)){
				 $registration_number = $data->registration_number;
			}else{
				$registration_number = 'UNDER PROCESS';	
			}
			
			if( ( $application_date < $last_week_date ) && trim($data->status) == "0"){ 				 
				
				$rows[] = array(
						array('data' => $counter, 'align' => 'left', 'class' => array('')),
						array('data' => $data->identification_number, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => $registration_number.'<br/>'.$issue_date, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => $data->e_name, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => $apply_date, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => get_user_application_status($data->status), 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => l($view_img, $view_link, array('html' => TRUE)), 'align' => 'left', 'class' => array('exp_pendng')),
						
				);
				
				
				
			}else{
				
				$rows[] = array(
						array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
						array('data' => $data->identification_number, 'align' => 'left', 'class' => array('even')),
						array('data' => '<strong>'.$registration_number.'</strong><br/>'.$issue_date, 'align' => 'left', 'class' => array('odd')),
						array('data' => $data->e_name, 'align' => 'left', 'class' => array('even')),
						array('data' => $apply_date, 'align' => 'left', 'class' => array('odd')),
						array('data' => get_user_application_status($data->status), 'align' => 'left', 'class' => array('even')),
						array('data' => l($view_img, $view_link, array('html' => TRUE, 'attributes' => array('target' => '_blank') )), 'align' => 'left', 'class' => array('odd')),
						
				);
			
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

