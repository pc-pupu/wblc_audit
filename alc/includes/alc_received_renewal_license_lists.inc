<?php
function alc_received_renewal_license_lists($form, &$form_state, $application_status = ''){
	global $base_root, $base_path, $user;
	
	$user_id 					= $user->uid;
	$header_title				= '';
	
	
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
	
	$form['alc_renewal_lists']['markup_data'] = array(		
					'#prefix'	=> '<div class="nav-tabs-custom">
										<ul class="nav nav-tabs">
											<li class="'.$active.'">'.l('All', 'alc-received-renewal-details').'</li>
											<li class="'.$activepending.'">'.l('Pending', 'alc-received-renewal-details/pending').'</li>
											<li class="'.$activeforward.'">'.l('Forward', 'alc-received-renewal-details/forward').'</li>
											<li class="'.$activefinalsubmit.'">'.l('Final Submit', 'alc-received-renewal-details/finalsubmit').'</li>												
											<li class="'.$activeissued.'">'.l('Issued', 'alc-received-renewal-details/issued').'</li>
											<li class="'.$activerejected.'">'.l('Rejected', 'alc-received-renewal-details/rejected').'</li>
										</ul>
										<div class="tab-content"><div class="tab-pane active" id="fa-icons">',	
					'#suffix'	=> '',																		
					'#markup' 	=> '<div class="feedback-scroll">'.alc_get_all_renewal_license_list($application_status).'</div>',																		
				);	
	

	
	return $form;		
}
	
function alc_get_all_renewal_license_list($application_status = ''){
		
	global $base_root, $base_path, $user;
	
 	$userid		=	$user->uid;
	$output 	= 	$view_link	= $view_img = $serialLabel = $applyDate = $license_issue_date = $status = $licenseInfo = $renewal_id = $serial_no_fromv = $particularid = $license_id = $licenseNoDate ='';
	$header 	= array(
					  array('data' => 'Sl. No', 'field' => 'slno','width' => '6%'),
					  array('data' => 'FORM-V / REF.No.<br/>Contractor Name', 'field' => 'name_of_contractor', 'width' => '18%'),
					  array('data' => 'License Details', 'field' => 'name_of_contractor', 'width' => '20%'),
					  array('data' => 'B/M/C/NA/SEZ', 'field' => 'created_date', 'width' => '10%'),
					  array('data' => 'Establishment Details', 'field' => 'reg', 'width' => '18%'),					 				  				 
					  array('data' => 'Apply Date', 'field' => 'applLic_date', 'width' => '12%'),
					  array('data' => 'Status', 'field' => 'status', 'width' => '10%'),
					  array('data' => 'ACTIONS', 'field' => 'actions', 'width' => '10%'),
	  		 	  );
				  
	if(isset($_GET['sort']) && isset($_GET['order'])) {
    	if($_GET['sort'] == 'asc')
      		$sort = 'ASC';
    	else
      		$sort = 'DESC';
 
		switch($_GET['order']) {
		  case 'Identification':
			$order = 'identification_number';
			break;
		  case 'Establishment':
			$order = 'e_name';
			break;
		  default:
			$order = 'full_name_principal_emp';
		}
	}else{
		$sort = 'ASC';
		$order = 'applLic_date';
	}
	
	/**** Fetch Inspector Block Code ****/
	
	$get_alc_subdiv_code		= db_select('l_customuser_relation_address', 'lcra');
	$get_alc_subdiv_code		->fields('lcra',array('sub_div_code','block_code','custom_area_jurisdiction','is_custom_area_jurisdiction'));
	$get_alc_subdiv_code		->condition('lcra.user_id',$userid);
	$get_alc_subdiv_code 		= $get_alc_subdiv_code->execute()->fetchAssoc();


	$alc_sub_div_code			= $get_alc_subdiv_code['sub_div_code'];
	$customjurisdiction 		= $get_alc_subdiv_code['custom_area_jurisdiction']; 
	$iscustom_juris				= $get_alc_subdiv_code['is_custom_area_jurisdiction'];	

	if($customjurisdiction=='BMCNS' && $iscustom_juris == 'Y'){
		$block_code_arr = db_select('l_customuser_reletion_services_wise_user_settings','cra')
		->fields('cra',array('block_code'))
		->condition('cra.user_id', $userid)
		->execute()->fetchObject()->block_code;
		$block_code_arr = explode(',', $block_code_arr); 
	}
	
	$query = db_select('l_contractor_license_apply_renweal', 'renewal_apply');	
  	$query->LeftJoin('l_contractor_license_application', 'license','license.id= renewal_apply.lincense_id');
	$query->LeftJoin('l_contractor_info_master', 'infomaster','infomaster.id= license.serial_no_from_v'); 	
  	$query->LeftJoin('l_clra_registration_master', 'clra_master','clra_master.id= infomaster.application_id');	
    $query->LeftJoin('l_particular_of_contract_labour', 'particular','particular.id= license.contractor_particular_id');
 	// Adding fields
  	$query->fields('license',array('contractor_license_number','license_date','serial_no_from_v','contractor_particular_id','backlog_license_no','backlog_license_date','is_backlog_license'));
	$query->fields('renewal_apply',array('id','lincense_id','created_by','renewal_application_final_status','renewal_appliaction_status','created_date','is_backlog_renewal','next_renweal_date'));
  	$query->fields('clra_master',array('registration_number','name_areatype','e_name'));  
	$query->fields('infomaster',array('name_of_contractor','contractor_type'));
	$query->fields('particular',array('name_work_site_areatype'));
	$query->addfield('license','id','license_pk_id'); 
	if(($iscustom_juris == 'Y') && (!empty($block_code_arr)) && ($customjurisdiction=='BMCNS')){  	
		$query->condition('particular.name_work_site_areatype', $block_code_arr, 'IN');
	}else{
		$query->condition('particular.worksite_subdivision',$alc_sub_div_code);
	}
	$query->condition('renewal_apply.renewal_application_final_status','F');
	
	if(!empty($application_status) && $application_status == 'pending'){
		$query->condition('renewal_apply.renewal_appliaction_status', 'F');
		$query->orderBy('renewal_apply.id','ASC');
	}
	if(!empty($application_status) && $application_status == 'issued'){
		$query->condition('renewal_apply.renewal_appliaction_status', 'I');
		$query->orderBy('renewal_apply.id','DESC');
	}
	if(!empty($application_status) && $application_status == 'finalsubmit'){
		$query->condition('renewal_apply.renewal_appliaction_status', 'U');
		$query->orderBy('renewal_apply.id','ASC');
	}
	if(!empty($application_status) && $application_status == 'rejected'){
		$query->condition('renewal_apply.renewal_appliaction_status', 'R');
		$query->orderBy('renewal_apply.id','DESC');
	}
	if(!empty($application_status) && $application_status == 'forward'){
		$query->condition('renewal_apply.renewal_appliaction_status', 'FW');
		$query->orderBy('renewal_apply.id','ASC');
	}
	if(empty($application_status)){
		$query->orderBy('renewal_apply.id','DESC');
	}
	
	$renewal_details = $query->execute()->fetchAll();
	
	if(!empty($renewal_details)){
		
   		foreach($renewal_details as $row){	
		   	
			$counter 		= $counter+1;            
			$view_img 		= '<span class="badge bg-blue"><i class="fa fa-eye"></i> View Details<div>';
			$estInfo 		= 'PE : '.$row->e_name.'<br/>REG NO : '.$row->registration_number;
				
			$renewal_id 	  = encryption_decryption_fun('encrypt',$row->id);
			$serial_no_fromv  = encryption_decryption_fun('encrypt',$row->serial_no_from_v);
			$particularid	  = encryption_decryption_fun('encrypt',$row->contractor_particular_id);	
			$license_id		  = encryption_decryption_fun('encrypt',$row->license_pk_id);	
			
			if($row->contractor_type == 1 ){
				$serialLabel = 'REF No : 00'.$row->serial_no_from_v;
			}else{
				$serialLabel = 'FORM-V : 00'.$row->serial_no_from_v;
			}
		
			
			if(!empty($row->created_date)){				
				$applyDate			= date('dS M, Y', strtotime($row->created_date));
				$application_date 	= strtotime($row->created_date); 
				$last_week_date 	= strtotime('-7 day');
			}
		
			if(!empty($row->license_date)){
				$license_issue_date = date('dS M, Y', strtotime($row->license_date));
			}else {
				$license_issue_date	= '';
			}
			
			if($row->next_renweal_date!=''){
				$validTill 			= '<strong>VALID TILL : '.date('dS M, Y', strtotime($row->next_renweal_date)).'</strong>';	
			}else{
				$validTill 			= '';
			}
			
			$blockMunData = db_query('select block_mun_name from block_mun_master where block_code='.$row->name_work_site_areatype)->fetchAssoc();
		    $blockMunName = $blockMunData['block_mun_name'];
				
				
			$get_remark_type 		= 	db_select('l_remark_license', 'lrl');
			$get_remark_type		->	fields('lrl', array('remark_type','remark_by_role'));
			$get_remark_type		->	condition('lrl.particular_id', $row->id);
			$get_remark_type		->	condition('lrl.flag','R');
			$get_remark_type		->	condition('lrl.is_active',1);
			$get_remark_type		->	orderBy('lrl.id','DESC');
			$get_remark_type		->	range(0, 1);
			$remark_type 		 	= 	$get_remark_type->execute()->fetchObject()->remark_type;
			$remark_by_role 		= 	$get_remark_type->execute()->fetchObject()->remark_by_role;
			
			if($remark_type == "P" && $remark_by_role == 4){
				$status 			= '<span class="backed" title=" Rectify FORM-IV"></span>';
			}else{
				$status 			= get_user_license_status($remark_type);
			}
			if($row->is_backlog_renewal!='1'){
				$view_link			=	'alc-view-contractor-license-renewal/'.$renewal_id.'/'.$license_id;
			}else{
				$view_link			=	'alc_view_backlog_contractor_license_renewal/'.$renewal_id.'/'.$serial_no_fromv.'/'.$particularid.'/'.$license_id.'/1';
			}
			
			if(!empty($row->backlog_license_no)){
				
				if($row->contractor_license_number==''){
					$licenseNoDate 	= 'In Process';	
				}else{
					$licenseNoDate 	= $row->contractor_license_number.' - '.date('dS M, Y',strtotime($row->license_date));	
				}
				$licenseInfo		=	'REF. LICENSE : '.$licenseNoDate.'<br/>LEGACY : '.$row->backlog_license_no.' - '.date('dS M, Y',strtotime($row->backlog_license_date)).'<br/>'.$validTill;
			}else{
				$licenseInfo		=	!empty($row->contractor_license_number) ? ($row->contractor_license_number.' - '.$license_issue_date).'<br/>'.$validTill  :'In Process';
			}
			
			
			if( ( $application_date < $last_week_date ) && trim($row->renewal_appliaction_status) == "F" ){

		
				$rows[] = array(
							array('data' => $counter, 'align' => 'left', 'class' => array('')),
							array('data' => '<strong>[ '.$serialLabel.' ]</strong> <br>'.$row->name_of_contractor, 'align' => 'left', 'class' => array('pendng')),
							array('data' =>	$licenseInfo,'align' => 'left', 'class' => array('pendng')),
							array('data' => $blockMunName, 'align' => 'left', 'class' => array('pendng')),
							array('data' => $estInfo, 'align' => 'left', 'class' => array('pendng')),						
							array('data' => $applyDate, 'align' => 'left', 'class' => array('pendng')),													
							array('data' =>	$status, 'align' => 'left', 'class' => array('pendng')),
							array('data' =>	l($view_img,$view_link, array('html' => TRUE, 'attributes' => array('target'=>'_blank') )),'align' => 'left', 'class' => array('')),
							
					  );
			}else {
			
				$rows[] = array(
						array('data' =>	$counter, 'align' => 'left', 'class' => array('')),
						array('data' => '<strong>[ '.$serialLabel.' ]</strong> <br>'.$row->name_of_contractor, 'align' => 'left', 'class' => array('')),
						array('data' =>	$licenseInfo,'align' => 'left', 'class' => array('pendng')),
						array('data' => $blockMunName, 'align' => 'left', 'class' => array('pendng')),
						array('data' => $estInfo, 'align' => 'left', 'class' => array($class_name)),						
						array('data' =>	$applyDate, 'align' => 'left', 'class' => array('')),													
						array('data' =>	$status, 'align' => 'left', 'class' => array('')),
						array('data' =>	l($view_img,$view_link, array('html' => TRUE , 'attributes' => array('target'=>'_blank') )), 'align' => 'left', 'class' => array(''))
						
				  );
			}
		}
	
   }

	$variables = array(
	  		'attributes' 		=> array('class' => array('table table-striped table-responsive admin-custom-table'),'id' => 'myTable'), 
	  		'header' 			=> $header,
	  		'rows'				=> $rows,
			'empty' 			=> t("No data found !")
	  );
	
	$output  = theme('datatable', $variables);
	
	return $output;
}