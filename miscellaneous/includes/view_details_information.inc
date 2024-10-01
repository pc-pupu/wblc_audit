<?php 
function view_details_form($form, &$form_state, $url_details='', $id='', $act_id=''){
   
	global $base_root, $base_path, $user;
	
	if(!empty($url_details) && $id!=''){ 
		
		$getInfo 				=  getInfoViewDetails($url_details,$id,$act_id);
		
		$getContractorInfo 		=  getInfoContractorDetails($url_details,$id,$act_id);
		
		$getInfoData			=  getInfoDataInfo($url_details,$id);
		
		$form['view_details']['markup_data_0'] = array(
														'#type' 	=> 'fieldset'
														);
														
		if($url_details == 'licence-clra'){ 
			
			$form['view_details']['markup_data_1'] = array(
														'#markup' 	=> $getInfoData	,			
													);
													
		}elseif($url_details == 'contractor-info'){
			
			$form['view_details']['markup_data_1'] = array(
														'#markup' 	=> $getContractorInfo	,			
													);
			
		}else{
		
			$form['view_details']['markup_data_1'] = array(
														'#markup' 	=> $getInfo	,			
													);
		}
	}else{
		drupal_set_message('You are not authorized to access this page',error);
	}
	
	return $form;
}

function getInfoViewDetails($url_details='',$id='', $act_id=''){
	
	global $base_root, $base_path, $user;
	
 	$user_id				=	$user->uid;
	$applicationId 			= 	encryption_decryption_fun('decrypt', $id);
	$actId 					= 	encryption_decryption_fun('decrypt', $act_id);
	
	
	if($applicationId!='' && $applicationId!=0){
		
		if($url_details=='workmen-info'){
			
			$header = array(
						  array('data' => 'Sl', 'field' => 'slno','width' => '5%'),
						  array('data' => 'Name', 'field' => ''),
						  array('data' => 'Date of Birth', 'width' => '10%'),
						  array('data' => 'Skill', 'field' => ''),
						  array('data' => 'Contact Details', 'field' => ''),
						  array('data' => 'Actions',),
					  );
			
			$MigrantWorkmen			= db_select('l_interstate_license_employee_master','limwm');
			$MigrantWorkmen			->fields('limwm', array());
			$MigrantWorkmen			->condition('limwm.license_id', $applicationId);
			$MigrantWorkmen			->condition('limwm.type', 'W');
			$MigrantWorkmen			->condition('limwm.is_active', 1);
			$MigrantWorkmen			-> orderBy('limwm.id', 'DESC');
			$result 				= $MigrantWorkmen->execute()->fetchAll();
			
		}elseif($url_details=='directorpartner-info' || $url_details=='incharge-info'){ 
			
			if($url_details=='directorpartner-info'){
				$type		= 'D';	
			}elseif($url_details=='incharge-info'){
				$type		= 'M';	
			}
			
			if($actId == 4){
				
				$DP_Incharge	= db_select('l_interstate_employer_master','limwm');
				$DP_Incharge	->fields('limwm', array());
				$DP_Incharge	->condition('limwm.application_id', $applicationId);
				$DP_Incharge	->condition('limwm.is_active', 1);
				if($url_details=='directorpartner-info'){
					$db_or = db_or();
					$db_or -> condition('limwm.designation', 'director', '=');
					$db_or -> condition('limwm.designation', 'partner', '=');
					$DP_Incharge 	-> condition($db_or);	
				}else{
					$DP_Incharge	->condition('limwm.designation', 'manager', '=');
				}
				$DP_Incharge	-> orderBy('limwm.id', 'DESC');
				$result 		= $DP_Incharge->execute()->fetchAll(); 
								
			}else{
			
				$DP_Incharge		= db_select('l_interstate_license_employee_master','limwm');
				$DP_Incharge		->fields('limwm', array());
				$DP_Incharge		->condition('limwm.license_id', $applicationId);
				$DP_Incharge		->condition('limwm.type', $type);
				$DP_Incharge		->condition('limwm.is_active', 1);
				$DP_Incharge		->orderBy('limwm.id', 'DESC');
				$result 			= $DP_Incharge->execute()->fetchAll();
			}
					
		}
		
		if(!empty($result)){
			
			foreach($result as $valueData){ 
			
				$counter++;
				
				
				
				if($url_details=='workmen-info'){
					
					/** ISMW WORKMEN DETAILS ***/
				
					if(!empty($valueData->workmen_type) && $valueData->workmen_type == 1){
						$Wskill = 'Highly Skilled';
					}elseif($valueData->workmen_type == 2){
						$Wskill = 'Semi-Skilled';
					}elseif($valueData->workmen_type == 3){
						$Wskill = 'Skilled';
					}else{
						$Wskill = 'Unskilled';
					}
					
					$workmen_address 	= $valueData->address.'<br/>'.get_full_address('l_interstate_license_employee_master', 'sub-table', $valueData->id, array('district','subdivision','area_type','area_code','village','police_station','pin_code','state'));
					
					
				
					$rows[] = array(
								array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
								array('data' => $valueData->name , 'align' => 'left', 'class' => array('even')),
								array('data' => date('dS M, Y',strtotime($valueData->dob)), 'align' => 'left', 'class' => array('odd')),
								array('data' => $Wskill, 'align' => 'left', 'class' => array('odd')),
								array('data' => 'Email : '.$valueData->email.'<br/>Contact : '.$valueData->contact_number, 'align' => 'left', 'class' => array('even')),
								
								array('data' => '<button type="button" class="btn btn-info pull-center" data-toggle="modal" data-target="#WMore_'.$valueData->id.'"><i class="fa fa-info-circle"></i>&nbsp;More</button>','javascript.void(0);',array('html' => TRUE), 'align' => 'left', 'class' => array('odd')),
								
						);
						
					$output.='<div class="modal fade" id="WMore_'.$valueData->id.'" role="dialog">
								<div class="modal-dialog">
									<div class="box box-primary box-solid">
										<div class="box-header">
											<button type="button" class="close" data-dismiss="modal">&times;</button>
											<h3 class="box-title">Workmen Information: '.$valueData->name.'</h3>
										</div>
										<div class="modal-body">
											<table cellpadding="0" cellspacing="0" border="0" class="table table-bordered">
												<tr>
													 <td width="50%"><strong>Workmen Name</strong></td>
													 <td>'.$valueData->name.'</td>                                         
												 </tr>
												 <tr>
													 <td><strong>Address</strong></td>
													 <td>'.$workmen_address.'<br/>Police Station - '.$valueData->police_station.',&nbsp;PIN-'.$valueData->pin_code.'</td>                                         
												 </tr>
												 <tr>
													 <td><strong>Date of Birth</strong></td>
													 <td>'.date('dS M, Y',strtotime($valueData->dob)).'</td>                                         
												 </tr>
												 <tr>
													 <td><strong>Father/ Husband&#146;s Name</strong></td>
													 <td>'.$valueData->guardian_name.'</td>                                         
												 </tr>
												 
												  <tr>
													 <td><strong>Contact Details</strong></td>
													 <td>Email : '.$valueData->email.'<br/>Contact : '.$valueData->contact_number.'</td>
												 </tr>
												 <tr>
													 <td><strong>Aadhar / Voter ID</strong></td>
													 <td>'.$valueData->id_proof.'</td>                                         
												 </tr>
												  <tr>
													 <td><strong>Skill Type</strong></td>
													 <td>'.$Wskill.'</td>                                         
												 </tr>
											 </table>
										</div> 	
									</div>
								</div>
							</div>';
				
				/** ISMW WORKMEN DETAILS ENDS ***/
				
				}elseif($url_details=='directorpartner-info' || $url_details=='incharge-info'){ 
					
					if($url_details=='directorpartner-info'){
						$data_targetId	= '#DPMore_';
						$modal			= 'DPMore_';
					}elseif($url_details=='incharge-info'){
						$data_targetId	= '#INMore_';
						$modal			= 'INMore_';
					}
					
					$header = array(
						  array('data' => 'Sl', 'field' => 'slno','width' => '5%'),
						  array('data' => 'Name', 'field' => ''),
						  array('data' => 'Designation', 'width' => '10%'),
						  array('data' => 'Contact Details', 'field' => ''),
						  array('data' => 'Actions',),
					  );
					  
					  if(!empty($valueData->designation_others)){
						$designation = ucwords($valueData->designation_others);
					  }else{
						$designation = ucwords($valueData->designation);
					  }
					  
					  if($actId == 4){
						  $DPIN_address 	= $valueData->address_line.'<br/>'.get_full_address('l_interstate_employer_master', 'sub-table', $valueData->id, array('district_code','subdivision_code','area_type','area_type_code','village_code','police_station','pin','state_code')); 
					  }else{
					  		$DPIN_address 	= $valueData->address.'<br/>'.get_full_address('l_interstate_license_employee_master', 'sub-table', $valueData->id, array('district','subdivision','area_type','area_code','village','police_station','pin_code','state'));
					  }
					  
					  $guardianName = !empty($valueData->guardian_name) ? $valueData->guardian_name: 'Not Applicable';
					 
					  $rows[] = array(
								array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
								array('data' => $valueData->name , 'align' => 'left', 'class' => array('even')),
								array('data' => $designation, 'align' => 'left', 'class' => array('odd')),
								array('data' => 'Email : '.$valueData->email.'<br/>Contact : '.$valueData->contact_number.'', 'align' => 'left', 'class' => array('even')),
								
								array('data' => '<button type="button" class="btn btn-info pull-center" data-toggle="modal" data-target="'.$data_targetId.$valueData->id.'">
									<i class="fa fa-info-circle"></i>&nbsp;More</button>','javascript.void(0);',array('html' => TRUE), 'align' => 'left', 'class' => array('odd')),
								
						);
						
						$output.='<div class="modal fade" id="'.$modal.$valueData->id.'" role="dialog">
									<div class="modal-dialog">
										<div class="box box-primary box-solid">
											<div class="box-header">
												<button type="button" class="close" data-dismiss="modal">&times;</button>
												<h3 class="box-title">'.$designation.': '.$valueData->name.'</h3>
											</div>
											<div class="modal-body">
												<table cellpadding="0" cellspacing="0" border="0" class="table table-bordered">
													<tr>
														 <td width="40%"><strong>Name</strong></td>
														 <td>'.$valueData->name.'</td>                                         
													 </tr>
													 <tr>
														 <td><strong>Father / Guardian Name </strong></td>
														 <td>'.$guardianName.'</td>                                         
													 </tr>
													 <tr>
														 <td><strong>Designation</strong></td>
														 <td>'.$designation.'</td>                                         
													 </tr>
													 <tr>
														 <td><strong>Address</strong></td>
														 <td>'.$DPIN_address.'</td>                                         
													 </tr>
													 <tr>
													  <tr>
														 <td><strong>Contact Details</strong></td>
														 <td>Email : '.$valueData->email.'<br/>Contact : '.$valueData->contact_number.'</td>
													 </tr>
												 </table>
											</div> 	
										</div>
									</div>
							</div>';
					
				}
			}
			
		}
		
	}
  if(in_array('Inspector', $user->roles)){ 
  	$class_name = array('class' => array('view-act-rules-table'));
  }elseif(in_array('Applicant', $user->roles)){
	 $class_name = array('class' => array('table table table-striped table-responsive dashboard-table'));   
  }elseif(in_array('ALC', $user->roles)){
		$class_name = array('class' => array('table table-striped table-responsive admin-custom-table'));  
  }
  $variables = array(
	  		'attributes' 		=> $class_name, 
	  		'header' 			=> $header,
	  		'rows'				=> $rows,
			'empty' 			=> t("Data not available")
	  );
	
	
  $output .= theme('datatable', $variables);
	
  return $output;		
  
}

function getInfoContractorDetails($url_details='',$id='', $act_id=''){
	
	global $base_root, $base_path, $user; 
	$i 				= 	0;
	$applicationId 	= 	encryption_decryption_fun('decrypt', $id);
	$actId			= 	encryption_decryption_fun('decrypt', $act_id);
	
	
	if(($applicationId!='' && $applicationId!=0) ||  (!empty($applicationId)) ){
		
		if($url_details=='contractor-info'){
			
			$header = array(
						  array('data' => 'Sl', 'field' => 'slno','width' => '5%'),
						  array('data' => 'Contractor Name', 'field' => '','width' => '20%'),
						  array('data' => 'Nature of Work', 'field' => '','width' => '20%'),
						  array('data' => 'Maximum number of migrant workmen', 'field' => '','width' => '20%'),
						  array('data' => 'Duration of contract', 'field' => '','width' => '20%'),
						  array('data' => 'Action', 'field' => '','width' => '15%')
						  );
						  
			
			$data_targetId	= '#contractorMore_';
			$modal			= 'contractorMore_';
			
			if($actId == 4){
				$tableName = 'l_interstate_contractor_master';
			}
			
			$ismw_con = db_select($tableName, 'licm');
			$ismw_con -> fields('licm', array());
			$ismw_con -> condition('licm.application_id', $applicationId);	
			$ismw_con -> condition('licm.contractor_status', 1);
			$ismw_con -> orderBy('licm.id', 'DESC');
			$contractors = $ismw_con->execute()->fetchAll();
			
			foreach($contractors as $conData){ 
				$i++;
				
				$con_nature_work = db_select('l_interstate_con_nature_of_work', 'lcnw');
				$con_nature_work ->leftJoin('contractor_works', 'cw', 'lcnw.contractor_nature_of_work = cw.contractor_work_id');
				$con_nature_work ->fields('cw',array('cont_work_name','contractor_work_id'));
				$con_nature_work ->condition('lcnw.contractor_id',$conData->id);
				$con_nature_work ->condition('lcnw.act_id',4);
				$con_nature_work ->condition('lcnw.application_id',$applicationId);
				$con_nature_work_data 	= $con_nature_work->execute()->fetchAssoc();
				
				if($conData->state_value == 1) { $state_value = 'West Bengal' ; }else { $state_value = 'Others'; }
												
				if($conData->contractor_type == 1) { $contractor_type = 'Offline License' ; }else { $contractor_type = 'New Contractor'; }
				
				if ($datafetch_clra_cont->state_value == 1){
					$type_con	 		= 'ismw';
					$conArr	 			= array('con_dist','con_subdivision','con_areatype','con_areatype_code','con_vill_ward','con_ps','con_pin');
					$contractor_address	= '<br/>'.get_full_address($table_con, $tableName, $conData->id, $conArr);	
				}
				
				if($con_nature_work_data['contractor_work_id']==28){
					$other_con_nature_work		= ' - '.$conData->other_nature_of_work;
				}else{
					$other_con_nature_work 		= '';
				}
				
				
				$rows[] = array(
						array('data' => $i, 'align' => 'left', 'class' => array('odd')),
						array('data' => $conData->name_of_contractor, 'align' => 'left', 'class' => array('odd')),
						array('data' => $con_nature_work_data['cont_work_name'].$other_con_nature_work, 'align' => 'left', 'class' => array('even')),
						array('data' => $conData->max_num_migrant_wrkmen, 'align' => 'left', 'class' => array('even')),
						array('data' => 'From: '.date('dS M, Y',strtotime($conData->est_date_commencement)).'</br>To: '.date('dS M, Y',strtotime($conData->est_date_termination)), 'align' => 'left', 'class' => array('even')),
						array('data' => '<button type="button" class="btn btn-info pull-center" data-toggle="modal" data-target="'.$data_targetId.$conData->id.'">
							<i class="fa fa-info-circle"></i>&nbsp;More</button>','javascript.void(0);',array('html' => TRUE), 'align' => 'left', 'class' => array('odd')),
					);
				
				$output.= '<div class="modal fade" id="'.$modal.$conData->id.'" role="dialog">
							<div class="modal-dialog">
								<div class="box box-primary box-solid">
									<div class="box-header">
										<button type="button" class="close" data-dismiss="modal">&times;</button>
										<h3 class="box-title">'.$conData->name_of_contractor.'</h3>
									</div>
									<div class="modal-body">
										<table cellpadding="0" cellspacing="0" border="0" class="table table-bordered">
											<tr>
												 <td width="40%"><strong>Name</strong></td>
												 <td>'.$conData->name_of_contractor.'</td>                                         
											 </tr>
											 <tr>
												 <td><strong>Type</strong></td>
												 <td>'.$contractor_type.'</td>                                         
											 </tr>
											 <tr>
												 <td><strong>Email</strong></td>
												 <td>'.$conData->email_of_contractor.'</td>                                         
											 </tr>
											 <tr>
												 <td><strong>Address</strong></td>
												 <td>'.$conData->address_of_contractor.$contractor_address.'</td>                                         
											 </tr>
											  <tr>
												<td><strong>Nature of Work for which migrant workmen are to be recruited or are employed</strong></td>
												<td>'.$con_nature_work_data['cont_work_name'].$other_con_nature_work.'</td>
											  </tr>
											  <tr>
												<td><strong>Maximum number of migrant workmen are to be employed on any day through each contractor</strong></td>
												<td>'.$conData->max_num_migrant_wrkmen.'</td>
											  </tr>
											  <tr>
												<td><strong>Estimated date and commencement of work under each contractor</strong></td>
												<td>'.date('dS M, Y',strtotime($conData->est_date_commencement)).'</td>
											  </tr>
											  <tr>
												<td><strong>Estimated date of termination of employment of migrant workmen under each contractor</strong></td>
												<td>'.date('dS M, Y',strtotime($conData->est_date_termination)).'</td>
											  </tr>
										 </table>
									</div> 	
								</div>
							</div>
						   </div>';	
			}
			$variables = array(
						'attributes' 		=> array('class' => array('table table-striped table-responsive admin-custom-table')),  
						'header' 			=> $header,
						'rows'				=> $rows,
						'empty' 			=> t("Data not available")
						);
	
	
			$output .= theme('datatable', $variables);
			return $output;
		}
	}
}

function getInfoDataInfo($url_details='',$id){
	
	global $base_root, $base_path, $user;
	
 	
	$user_id	=	$user->uid;
	$infoId 	= 	encryption_decryption_fun('decrypt', $id);
	
	
	if(($infoId!='' && $infoId!=0) ||  (!empty($infoId)) ){
		
		if($url_details=='licence-clra'){
			
			$header 		= array(
								  array('data' => 'Sl', 'field' => 'slno','width' => '5%'),
								  array('data' => 'Contractor Name / Worksite Address', 'field' => '','width' => '25%'),
								  array('data' => 'License Details', 'field' => '','width' => '20%'),
								  array('data' => 'Establishment Details', 'field' => '','width' => '25%'),
								  array('data' => 'Principal Employer Details', 'field' => '','width' => '25%')
								  );
						  
			$licenceNo 		= explode(',', $infoId);
			$noLicense		= count($licenceNo);
			$counter		= 0;
			$counterRows	= 0;
			$result			= array();
			
			if(!empty($noLicense)){	
			
				for($counter = 0 ; $counter <= $noLicense-1 ; $counter++){
					
					$counterRows++;
					
					$getLicense = db_select('l_contractor_license_application', 'license');
					$getLicense->leftJoin('l_particular_of_contract_labour', 'part','part.id =license.contractor_particular_id');  
					$getLicense->leftJoin('l_clra_registration_master', 'pe','pe.user_id =license.user_id AND pe.registration_number =license.principal_reg_no');  
					$getLicense->leftJoin('l_contractor_info_master', 'con','con.contractor_user_id =license.contractor_user_id');
					$getLicense->fields('license',array('contractor_license_number','certificates_id','backlog_license_no','is_backlog_license','backlog_license_date','next_renweal_date','license_date'));
					$getLicense->fields('pe',array('id','e_name','full_name_principal_emp','address_principal_emp','loc_e_name','registration_date','registration_number'));
					$getLicense->fields('con',array('name_of_contractor'));
					$getLicense->fields('part',array('id','worksite_address_line'));
					$getLicense->condition('license.contractor_license_number', trim($licenceNo[$counter]));
					$licenseResult = $getLicense->execute()->fetchAssoc();
					
					isset($licenseResult['backlog_license_no'])? $backlog_license_no = '<b>Manual License No.: </b>'.$licenseResult['backlog_license_no'].'<br/>': '';
					isset($licenseResult['next_renweal_date'])? $renewaldate = '<b>Renewal Date: </b>'.date('dS M, Y',strtotime($licenseResult['next_renweal_date'])): '';
				
					
					$est_address = $licenseResult['loc_e_name'].'<br/>'.get_full_address('l_clra_registration_master', 'sub-table', $licenseResult['id'], array('loc_e_dist','loc_e_subdivision','loc_e_areatype','name_areatype','loc_e_vill_ward','l_e_ps','loc_e_pin_number'));
					$pe_address =  $licenseResult['address_principal_emp'].'<br/>'.get_full_address('l_clra_registration_master', 'sub-table', $licenseResult['id'], array('loc_emp_dist','loc_emp_subdivision','loc_emp_areatype','emp_name_areatype','loc_emp_vill_ward','l_emp_ps','loc_emp_pin_number')); 
					$worksite_address =  $licenseResult['worksite_address_line'].'<br/>'.get_full_address('l_particular_of_contract_labour', 'sub-table', $licenseResult['part_id'], array('worksite_dist','worksite_subdivision','work_site_areatype','name_work_site_areatype','work_site_vill_ward','worksite_ps','worksite_pin')); 
				
					$rows[] = array(
									array('data' => $counterRows, 'align' => 'left'),
									array('data' => '<b>Name: </b>'.$licenseResult['name_of_contractor'].'<br/><b>WORKSITE ADDRESS: </b>'.$worksite_address, 'align' => 'left'),
									array('data' => '<b>LIC NO.: </b>'.$licenseResult['contractor_license_number'].'<br/> <b>Issued Date:</b> '.date('dS M, Y',strtotime($licenseResult['license_date'])).'<br/>'.$backlog_license_no.$renewaldate , 'align' => 'left'),
									array('data' => '<b>REG NO.: </b>'.$licenseResult['registration_number'].'<br/> <b>Date:</b> '.date('dS M, Y',strtotime($licenseResult['registration_date'])).'<br/><b>Name: </b>'.$licenseResult['e_name'].'<br/><b>Address: </b>'.$est_address , 'align' => 'left'),
									array('data' => '<b>Name: </b>'.$licenseResult['full_name_principal_emp'].'<br/><b>Address: </b>'.$pe_address , 'align' => 'left'),
									
									
							);
				
				}
			}
			
			if(in_array('Inspector', $user->roles)){ 
				$class_name = array('class' => array('view-act-rules-table'));
			}elseif(in_array('Applicant', $user->roles)){
				$class_name = array('class' => array('table table table-striped table-responsive dashboard-table'));   
			}elseif(in_array('ALC', $user->roles)){
				$class_name = array('class' => array('table table-striped table-responsive admin-custom-table'));  
			}
			
			$variables = array(
							'attributes' 		=> $class_name,
							'header' 			=> $header,
							'rows'				=> $rows,
							'empty' 			=> t("Data not available")
							);
	
	
  			$output = theme('datatable', $variables);
			return $output;
		}
	}
}

