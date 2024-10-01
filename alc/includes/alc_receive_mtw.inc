<?php 
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
	
function alc_receive_application($form, &$form_state, $type = ''){

	global $base_root, $base_path, $user;
	
	$_SESSION['uid'] 	= $user->uid;
	$user_id			= $user->uid;
	
	$form['mtw_application_lists'] 				= array(
				'#type' 		=> 'fieldset',
				'#attributes'	=> array('class'=>array(''))
	);
		
	$get_all_mtw_application_lists =  mtw_application_lists_alc($type);
		
		
	$form['mtw_application_lists']['markup_data_1'] = array(
				'#prefix' 		=> '<div class="row"><div class="col-md-12"><div class="box box-primary"><div class="box-body"><div class="feedback-scroll">',
				'#suffix' 		=> '</div></div></div></div></div>',
				'#markup' 		=> $get_all_mtw_application_lists,
	);
	return $form;
}

function mtw_application_lists_alc($type=''){
	
	global $base_root, $base_path, $user;
	
 	$user_id	=	$user->uid;
	$action		= 	'encrypt';
	
	 $header = array(
		  array('data' => 'Sl. No', 'field' => 'slno','width' => '6%'),
		  array('data' => 'IDENTIFICATION NUMBER', 'field' => 'reg_no','width' => '16%'),
		  array('data' => 'REGISTRATION NUMBER <br>REGISTRATION DATE', 'field' => 'reg_no','width' => '22%'),
		  array('data' => 'MTW NAME', 'field' => 'aname','width' => '17%'),
		  array('data' => 'APPLY DATE', 'field' => 'applicationDate'),
		  array('data' => 'STATUS', 'field' => 'status'),
		  array('data' => 'ACTIONS', 'field' => 'actions'),
	  );
	
	/**** Fetch ALC SUBDIVISION Code ****/
	
	$query_subdivision			 =db_select('l_customuser_relation_address', 'lcra');
	$query_subdivision			->fields('lcra',array('sub_div_code','district_code'));
	$query_subdivision			->condition('lcra.user_id',$user_id,'=');
	$result_query_subdivision 	= $query_subdivision->execute();
	if($result_query_subdivision-> rowCount() > 0 ){
		$subdivision_code_result = $result_query_subdivision->fetchAssoc();
		$subdivision_alc = $subdivision_code_result['sub_div_code'];
	}
	
	/** Fetch All MTW Application Details **/
  		
  $get_mtw_application_details 				= db_select('l_mtw_registration_master', 'lmrm');
  $get_mtw_application_details				->fields('lmrm',array('id','mtw_name','application_date','registration_date','identification_number','registration_number','final_submit_status','status','user_id','is_renew','is_amend'));
  $get_mtw_application_details				->condition('lmrm.mtw_loc_subdivision',$subdivision_alc);
  $get_mtw_application_details				->condition('lmrm.final_submit_status',"P");
  if($type == 'issued'){
			$db_or = db_or();
			$db_or->condition('lmrm.status', "I" , '=');
			$db_or->condition('lmrm.is_renew', 1 , '=');
			$db_or->condition('lmrm.is_amend', 1 , '=');
			$get_mtw_application_details->condition($db_or);
  }else{
  			//$db_or2 = db_or();
			//$db_or2->condition('lmrm.is_renew', 0 , '=');
			//$db_or2->condition('lmrm.is_amend', 0 , '=');
			//$get_mtw_application_details->condition($db_or2);
			$get_mtw_application_details->condition('lmrm.status', 'I' , '!=');
			$get_mtw_application_details->condition('lmrm.is_renew', 0 , '=');
			$get_mtw_application_details->condition('lmrm.is_amend', 0 , '=');
  
  }
  $get_mtw_application_details				->orderBy('lmrm.id','DESC');
  $get_mtw_application_details_result 		= $get_mtw_application_details->execute();
  
  
  //echo $get_mtw_application_details_result->rowCount();die('ok');
  //print_r($get_mtw_application_details_result);die('ok');
  
  if( $get_mtw_application_details_result->rowCount() > 0 ){ 
  
  	$counter			= 0;  
	$view_img			= '<span class="badge bg-blue"><i class="fa fa-eye"></i> View Details<div>';
		
		while($data = $get_mtw_application_details_result->fetchObject()) {  
			$counter++;
			
			if(!empty($data->application_date)){
				$apply_date 		= date('dS M, Y', strtotime($data->application_date));
				
				/*** Escalation ****/
				
				$application_date 		= strtotime($data->application_date); 
				$last_week_date 		= strtotime('-7 day');
				
			}
			if(!empty($data->registration_date)){
				$issue_date 		= date('dS M, Y', strtotime($data->registration_date));
			}else {
				$issue_date			= '';
			}
			
			
			
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
			
			if($data->is_renew ==1){
				$is_renewal_amend =	'<br>Apply For RENEWAL';
			}elseif($data->is_amend ==1){
				$is_renewal_amend =	'<br>Apply For AMENDMENT';
			}else{
				$is_renewal_amend =	'';
			}
			
			
			if( ( $application_date < $last_week_date ) && trim($data->status) == "0"){ 				 
				
				$rows[] = array(
						array('data' => $counter, 'align' => 'left', 'class' => array('')),
						array('data' => $data->identification_number.$is_renewal_amend, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => $registration_number.'<br/>'.$issue_date, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => $data->mtw_name, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => $apply_date, 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => get_user_application_status($data->status), 'align' => 'left', 'class' => array('exp_pendng')),
						array('data' => l($view_img, 'alc-application-details-mtw/'.encryption_decryption_fun($action, $data->id).'/'.encryption_decryption_fun($action, $data->user_id), array('html' => TRUE)), 'align' => 'left', 'class' => array('exp_pendng')),
						
				);
			}else{
				$rows[] = array(
						array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
						array('data' => $data->identification_number.$is_renewal_amend, 'align' => 'left', 'class' => array('even')),
						array('data' => '<strong>'.$registration_number.'</strong><br/>'.$issue_date, 'align' => 'left', 'class' => array('odd')),
						array('data' => $data->mtw_name, 'align' => 'left', 'class' => array('even')),
						array('data' => $apply_date, 'align' => 'left', 'class' => array('odd')),
						array('data' => get_user_application_status($data->status), 'align' => 'left', 'class' => array('even')),
						array('data' => l($view_img, 'alc-application-details-mtw/'.encryption_decryption_fun($action, $data->id).'/'.encryption_decryption_fun($action, $data->user_id), array('html' => TRUE)), 'align' => 'left', 'class' => array('odd')),
						
				);
			
			}
  		
 		}
  
  }
  
  $variables = array(
	  		'attributes' 		=> array('class' => array('table table-striped table-responsive admin-custom-table')), 
	  		'header' 			=> $header,
	  		'rows'				=> $rows,
			'empty' 			=> t("No data found!")
	  );
	
	
  $output = theme('datatable', $variables);
	
  return $output;		
  
}

