<?php
function alc_received_license($form, &$form_state, $application_status = ''){
	
	global $base_root, $base_path, $user;
		
	$_SESSION['uid'] = $user->uid;
	$user_id		 = $user->uid;
	
	if(empty($application_status)){$active = 'active';}	
	if($application_status == 'pending'){$activepending = 'active';}else{$activepending = '';}
	if($application_status == 'issued'){$activeissued = 'active';}else{$activeissued = '';}
	if($application_status == 'finalsubmit'){$activefinalsubmit = 'active';}else{$activefinalsubmit = '';}
	if($application_status == 'rejected'){$activerejected = 'active';}else{$activerejected = '';}
	if($application_status == 'forward'){$activeforward = 'active';}else{$activeforward = '';}
	
	if($application_status == ''){
		$header_title = 'All';
	}else{
		$header_title = ucfirst($application_status);
	}

	/*$form['contractor_license_lists'] = array(
									  '#type' 				=> 'fieldset',
									  '#attributes'		 	=> array('class'=>array(''))
									  );*/
	
	$get_all_contractor_license =  contractor_license_list($application_status);		
	
	$form['contractor_license_lists']['markup_data_1'] = array(
									'#prefix' => '<div class="nav-tabs-custom">
													<ul class="nav nav-tabs">
														<li class="'.$active.'">'.l('All', 'alc-received-license').'</li>
														<li class="'.$activepending.'">'.l('Pending', 'alc-received-license/pending').'</li>
														<li class="'.$activeforward.'">'.l('Forward', 'alc-received-license/forward').'</li>
														<li class="'.$activefinalsubmit.'">'.l('Final Submit', 'alc-received-license/finalsubmit').'</li>												
														<li class="'.$activeissued.'">'.l('Issued', 'alc-received-issued-license').'</li>
														<li class="'.$activerejected.'">'.l('Rejected', 'alc-received-license/rejected').'</li>
													</ul>
													<div class="tab-content">
														<div class="tab-pane active" id="fa-icons">
															<section id="new">',		
									'#markup' 			    => $get_all_contractor_license,	
									'#suffix'				=> '</section>',																	
									);		
	return $form;  
}


function contractor_license_list($application_status = ''){
	global $user;
	$uid = $user->uid;
	$output = '';
	
	$duplicate = array();
	$row_arr = array();
   // Table header
    $header = array(
					array('data' => 'Sl. No', 'field' => 'slno','width' => '6%'),
					array('data' => 'Contractor Name', 'field' => 'name_of_contractor'),
					//array('data' => 'Form V Serial No.', 'field' => 'id', 'width' => '10%'),
					array('data' => 'Registration No. of Principal Employer', 'field' => 'reg', 'width' => '15%'),					
					array('data' => 'Apply Date', 'field' => 'created_date', 'width' => '10%'),
					array('data' => 'Form V Serial No/ Licence Number', 'field' => 'contractor_license_number', 'width' => '15%'),
					array('data' => 'Licence Date', 'field' => 'license_date', 'width' => '10%'),
					array('data' => 'Status', 'field' => 'status'),
					array('data' => 'Action', 'field' => 'actions'),
					);
  	 // Setting the sort conditions
  	if(isset($_GET['sort']) && isset($_GET['order'])) {
    // Sort it Ascending or Descending?
    if($_GET['sort'] == 'asc')
      	$sort = 'ASC';
    else
      	$sort = 'DESC';
 
    // Which column will be sorted
    switch($_GET['order']) {
      case 'Name':
        $order = 'identification_number';
        break;
      case 'Email':
        $order = 'f_name';
        break;
      default:
        $order = 'm_name';
    }
  }else {
    $sort = 'ASC';
    $order = 'f_name';
  }
  
	$query_subdivision=db_select('l_customuser_relation_address', 'lcra');
	$query_subdivision->fields('lcra',array('sub_div_code','district_code'));
	$query_subdivision->condition('lcra.user_id',$uid,'=');
	$result_query_subdivision = $query_subdivision->execute();
	
	$result_arr = $result_query_subdivision->fetchAssoc();
	
	$subdivision_alc = $result_arr['sub_div_code'];
	
	$query = db_select('l_contractor_info_master', 'infomaster');
	
  	$query->InnerJoin('l_particular_of_contract_labour', 'particular','particular.serial_no_from_v= infomaster.id'); 
 	$query->InnerJoin('l_contractor_license_application', 'license','particular.id= license.contractor_particular_id'); 
  	$query->InnerJoin('l_clra_registration_master', 'clra_master','clra_master.id= infomaster.application_id');
	
     
 	// Adding fields
  	$query->fields('infomaster',array('name_of_contractor','application_id','id'));
  	$query->fields('particular',array('id','serial_no_from_v','contractor_subdivision'));
  	$query->fields('license',array('contractor_license_number','final_status','status','created_date','license_date'));
  	$query->fields('clra_master',array('registration_number'));  
  	//$query->condition('clra_master.loc_e_subdivision',$subdivision_alc);
	$query->condition('particular.worksite_subdivision',$subdivision_alc);
	$query->condition('license.final_status','F');
	$query->isNull('license.is_backlog_license');
	$query->condition('license.status','I','!=');
	
	if(!empty($application_status) && $application_status == 'pending'){
		$query->condition('license.status', 'F');
	}
	
	if(!empty($application_status) && $application_status == 'issued'){
		$query->condition('license.status', 'I');
	}
	if(!empty($application_status) && $application_status == 'finalsubmit'){
		$query->condition('license.status', 'U');
	}
	if(!empty($application_status) && $application_status == 'rejected'){
		$query->condition('license.status', 'R');
	}
	if(!empty($application_status) && $application_status == 'forward'){
		$query->condition('license.status', 'FW');
	}
	
	// Set order by
  	$query->orderBy('particular.id','Asc');
	$result = $query->execute()->fetchAll();
	//print_r($result); exit;
	
	/*if(!empty($result)){
		foreach($result as $key => $val){
			     
		if(!in_array($val->id,$duplicate)){//echo 'hi';
			if($val->status == 'I'){
				array_push($duplicate,$val->id);
				//array_push($row_arr,$val);
				//break;
				
			}else{
				array_push($row_arr,$val);
			}
		}else{//echo 'hello';
				array_push($row_arr,$val);
			}	
		}
	}*/
	
	$sl_no = 0;	
  	
	if(!empty($result)){
	
	
		foreach($result as $row){
			
			
			
			$query_frmv = db_select('l_contractor_info_master', 'cim');
			$query_frmv = $query_frmv->fields('cim',array('is_from_v'));
			$query_frmv->condition('cim.id',$row->id);
			$is_frm_v = $query_frmv->execute()->fetchObject();
			$is_frm_v = $is_frm_v->is_from_v;
			
			if($is_frm_v=='0'){ $serial_no='N/A';}else{$serial_no='00'.$row->id;}
			
			/**** END FROM V STATUS ****/
			
			
			$sl_no++;
			$contractor_license_number = $row->contractor_license_number;
			
			if(empty($contractor_license_number)){
				$license_number = 'In Process';
			}else{
				$license_number = $contractor_license_number;
			}
			
			$applicationdate = strtotime($row->created_date);
			$application_date = date('j M Y', $applicationdate);
			$date_license=$row->license_date; 
			
			if(empty($date_license)){
				$licensedate='N/A';
			}else{
				$license_date = strtotime($row->license_date);
				$licensedate= date('j M Y', $license_date);
			}
	
			$last_week_date = strtotime('-7 day');
			
			/****  encrpting ****/
	
			$action 			= 	'encrypt';
			
			$serial_no_from_v	=	encryption_decryption_fun($action, $row->serial_no_from_v); //---miscellaneous module
			$particular_id		=	encryption_decryption_fun($action, $row->particular_id);
			
			/**** end encrpting ****/
			
			/**** Staus of Application  ****/
			
			$query 	= 	db_select('l_remark_license', 'lrl')
				->	fields('lrl', array('remark_type'))
				->	condition('lrl.remark_to', $row->serial_no_from_v)
				->	condition('lrl.particular_id', $row->particular_id)
				->	condition('lrl.is_active', 1)
				->	orderBy('id', 'DESC')
			    ->  extend('PagerDefault')->limit(1);
						 
	$remark_type = $query->execute()->fetchObject()->remark_type;
			if( trim($row->status) == "F" && trim($row->final_status) == "F" ){ 
			   $STATUS 	= '<span class="pending" title="Pending Application"></span>';   		
			}
			if(!empty($remark_type)){	
							
          	if( trim($remark_type) == "A") { 
			         		
				$STATUS 	= '<span class="feespending" title="Fees Pending"></span>';		
				
				
          	}if($remark_type == "B"){
          		
				$STATUS 	= '<span class="backed" title="Back for Rectification"></span>';
				
				
          	}if( $remark_type == "P" ){
				
				$STATUS		= '<span class="feespaid" title="Fees Paid"></span>';
			  	
			  
          	}if( $remark_type == "BI" ){
				
				$STATUS 	= '<span class="backtoins" title="Back to Inspector"></span>';	
				
          	}
          	if($remark_type == "C"){
				
				$STATUS		= '<span class="callapplicant" title="Call Applicant"></span>';	
				
				
          	}if( $remark_type == "I"){
          		
				$STATUS	= '<span class="issued" title="Certificate Issued"></span>';
				
				
          	}if( $remark_type == "R"){
				
				$STATUS 	= '<span class="reject" title="Rejected"></span>';
				
				
          	} if( $remark_type == "FW"){
				$STATUS 	= '<span class="forward" title="Forwarded"></span>';
				
          	}if( $remark_type == "U"){
				$STATUS 		= '<span class="finalsubmit" title="Final Submit"></span>';
				
          	}
			}
			$view_link	= l('<span class="badge bg-blue"><i class="fa fa-eye"></i> View Details<div>', 'alc-view-license/'.$serial_no_from_v.'/'.$particular_id, array('html' => TRUE));			
						
			if(($applicationdate < $last_week_date ) && $formStatus =="PENDING"){
				$class_name = 'pendng';
			}else{
				$class_name = 'odd';
			}
			$rows[] = array(
						array('data' => $sl_no, 'align' => 'left', 'class' => array($class_name)),
						array('data' => $row->name_of_contractor, 'align' => 'left', 'class' => array($class_name)),
						// array('data' => $serial_no, 'align' => 'left', 'class' => array($class_name)),
						array('data' => $row->registration_number, 'align' => 'left', 'class' => array($class_name)),						
						array('data' => $application_date, 'align' => 'left', 'class' => array($class_name)),
						array('data' => $serial_no.'<br>'.$license_number, 'align' => 'left', 'class' => array($class_name)),
						array('data' => $licensedate, 'align' => 'left', 'class' => array($class_name)),
						array('data' => $STATUS , 'align' => 'left', 'class' => array($class_name)),
						array('data' => $view_link, 'align' => 'left', 'class' => array($class_name)),					
						);
			
		}  
  
	}
	
    $variables = array(
	  		'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')), 
	  		'header' 	 => $header,
	  		'rows'		 => $rows,
			'empty' 	 => t("Application not available")
	  		);
	  
	 $output = theme('datatable', $variables);
	 return '<div class="feedback-scroll">'.$output.'</div>';

}


		
