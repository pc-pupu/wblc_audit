<?php 
function amendment_establishmentlocation_approval($form, &$form_state, $actid='',$id = ''){
	global $base_root, $base_path, $user;

	$actId				= encryption_decryption_fun('decrypt', $actid);
	$applicationId		= encryption_decryption_fun('decrypt', $id);
	$userId				= $user->uid;
	$output				= '';
	
	if($actId == 1){
		$applicantUserId = 'user_id';
		$masterTable 	= 'l_clra_registration_master';
		$amendTable		= 'l_clra_amendment';
		$addressLine	= 'loc_e_name';
		$district	 	= 'loc_e_dist';
		$subdivision 	= 'loc_e_subdivision';
		$areaType 	 	= 'loc_e_areatype';
		$areaCode	 	= 'name_areatype';
		$villWard	 	= 'loc_e_vill_ward';
		$ps			 	= 'l_e_ps';
		$pin		 	= 'loc_e_pin_number';
		$status			= 'status';
		$amendmentParent = 'amendment_parent_id';
		
	}
	if($areaType == 'B'){
		$areaName = 'Block';	
	}else if($areaType == 'C'){
		$areaName = 'Corporation';	
	}else if($areaType == 'M'){
		$areaName = 'Municipality';	
	}else if($areaType == 'N'){
		$areaName = 'Notified Area';	
	}else if($areaType == 'SEZ'){
		$areaName = 'SEZ';	
	}
	
	$masterData = db_select($masterTable,'master');
	$masterData->fields('master',array($amendmentParent,$addressLine,$areaType,$pin,$applicantUserId,$status));
	$masterData->fields('dm', array('district_name'));
	$masterData->leftJoin('district_master', 'dm', 'dm.district_code=master.'.$district);
	
	$masterData->fields('sd', array('sub_div_name'));
	$masterData->leftJoin('sub_division', 'sd', 'sd.sub_div_code=master.'.$subdivision);
		
	$masterData->fields('bmm', array('block_mun_name'));
	$masterData->leftJoin('block_mun_master', 'bmm', 'bmm.block_code=master.'.$areaCode);
	
	$masterData->fields('vwm', array('village_name'));
	$masterData->leftJoin('village_ward_master', 'vwm', 'vwm.village_code=master.'.$villWard);
	
	$masterData->fields('ps', array('name_of_police_station'));
	$masterData->leftJoin('police_station', 'ps', 'ps.police_station_code=master.'.$ps);
	 
	$masterData->condition('master.id', $applicationId); 
	$getCurrentData = $masterData->execute()->fetchAssoc();
	
	$amended_fields	= db_select($amendTable, 'lca');
	$amended_fields	->leftJoin('l_custom_user_detail', 'cus_detl','cus_detl.usr_id = lca.approved_by_uid');
	$amended_fields	->fields('cus_detl',array('fullname','employee_id'));
	$amended_fields	->fields('lca', array());
	$amended_fields	->condition('lca.application_id', $getCurrentData[$amendmentParent]);
	$amended_fields->condition('lca.applicant_uid', $getCurrentData[$applicantUserId]);
	$amended_fields->condition('lca.status', 1);
	$amended_fields->orderBy('lca.id', 'DESC');
	$amended_fields->range(0, 1);
	$amended_fields_result 	= $amended_fields->execute()->fetchAssoc(); 
	
	if($amended_fields_result['flag'] == 'R' && $amended_fields_result['approved_by_uid']!='' && $getCurrentData[$status] == 'B'){
	
		if($getCurrentData[$amendmentParent]!='' && $getCurrentData[$amendmentParent]!=0){
		
		$previousData = db_select($masterTable,'master');
		$previousData->fields('master',array($amendmentParent,$addressLine,$areaType,$pin,$applicantUserId));
		$previousData->fields('dm', array('district_name'));
		$previousData->leftJoin('district_master', 'dm', 'dm.district_code=master.'.$district);
		
		$previousData->fields('sd', array('sub_div_name'));
		$previousData->leftJoin('sub_division', 'sd', 'sd.sub_div_code=master.'.$subdivision);
			
		$previousData->fields('bmm', array('block_mun_name'));
		$previousData->leftJoin('block_mun_master', 'bmm', 'bmm.block_code=master.'.$areaCode);
		
		$previousData->fields('vwm', array('village_name'));
		$previousData->leftJoin('village_ward_master', 'vwm', 'vwm.village_code=master.'.$villWard);
		
		$previousData->fields('ps', array('name_of_police_station'));
		$previousData->leftJoin('police_station', 'ps', 'ps.police_station_code=master.'.$ps);
		 
		$previousData->condition('master.id', $getCurrentData[$amendmentParent]); 
		$getPreviousData = $previousData->execute()->fetchAssoc();
		
		if($getCurrentData[$applicantUserId] == $getPreviousData[$applicantUserId]){
		
			if(!empty($getPreviousData[$areaType])){
				$getPreviousLoc = $getPreviousArea = '';
				if($getPreviousData[$areaType]=="B"){
					$getPreviousLoc = "Block";
					$getPreviousArea = 'Gram Panchayat';
				}elseif($getPreviousData[$areaType]=="M" ){
					$getPreviousLoc = "Municipality";
					$getPreviousArea = 'Ward'; 
				}elseif($getPreviousData[$areaType]=="C"){
					$getPreviousLoc = "Corporation";
					$getPreviousArea = 'Ward'; 
				}elseif($getPreviousData[$areaType]=="N"){
					$getPreviousLoc = "Notified Area";
					$getPreviousArea = 'Notified Area';
				}elseif($getPreviousData[$areaType]=="S"){
					$getPreviousLoc = "SEZ"; 
					$getPreviousArea = 'Sector';
				}
			}
			
			if(!empty($getCurrentData[$areaType])){
				$getCurrentLoc	= $getCurrentArea ='';
				if( $getCurrentData[$areaType]=="B"){
					$getCurrentLoc = "Block";
					$getCurrentArea = 'Gram Panchayat';
				}elseif($getCurrentData[$areaType]=="M"){
					$getCurrentLoc = "Municipality";
					$getCurrentArea = 'Ward'; 
				}elseif($getCurrentData[$areaType]=="C"){
					$getCurrentLoc = "Corporation";
					$getCurrentArea = 'Ward'; 
				}elseif($getCurrentData[$areaType]=="N"){
					$getCurrentLoc = "Notified Area"; 
					$getCurrentArea = 'Notified Area';
				}elseif($getCurrentData[$areaType]=="S"){
					$getCurrentLoc = "SEZ";
					$getCurrentArea = 'Sector';
				}
			}
			
			$output	 .= '<div class="row"><div class="col-md-6">
							<table class="table table-bordered table-responsive admin-custom-table">
								<tr>
								  <th style="width:40%" colspan="2">Previous Location of the Establishment </th>
								</tr>
								<tr>
									<td>House/Flat No., Street/Village/ Locality Name </td>
									<td>'.$getPreviousData['loc_e_name'].'</td>
								</tr>
								<tr>
									<td>District</td>
									<td>'.$getPreviousData['district_name'].'</td>
								</tr>
								<tr>
									<td>Subdivision</td>
									<td>'.$getPreviousData['sub_div_name'].'</td>
								</tr>
								<tr>
									<td>Area Type</td>
									<td>'.$getPreviousLoc.'</td>
								</tr>
								<tr>
									<td>'.$getPreviousLoc.'  Name</td>
									<td>'.$getPreviousData['block_mun_name'].'</td>
								</tr>
								<tr>
									<td>'.$getPreviousArea.'</td>
									<td>'.ucwords($getPreviousData['village_name']).'</td>
								</tr>
								<tr>
									<td>Police Station</td>
									<td>'.$getPreviousData['name_of_police_station'].'</td>
								</tr>
								<tr>
									<td>PIN Code</td>
									<td>'.$getPreviousData['loc_e_pin_number'].'</td>
								</tr>
							</table>
						</div>
						<div class="col-md-6">
							<table class="table table-bordered table-responsive admin-custom-table">
								<tr>
								  <th style="width:40%" colspan="2">Current Location of the Establishment ( For Amendment Approval)</th>
								</tr>
								<tr>
									<td>House/Flat No., Street/Village/ Locality Name </td>
									<td>'.$getCurrentData['loc_e_name'].'</td>
								</tr>
								<tr>
									<td>District </td>
									<td>'.$getCurrentData['district_name'].'</td>
								</tr>
								<tr>
									<td>Subdivision </td>
									<td>'.$getCurrentData['sub_div_name'].'</td>
								</tr>
								<tr>
									<td>Area Type </td>
									<td>'.$getCurrentLoc.'</td>
								</tr>
								<tr>
									<td>'.$getCurrentLoc.' Name</td>
									<td>'.$getCurrentData['block_mun_name'].'</td>
								</tr>
								<tr>
									<td>'.$getCurrentArea.'</td>
									<td>'.ucwords($getCurrentData['village_name']).'</td>
								</tr>
								<tr>
									<td>Police Station</td>
									<td>'.$getCurrentData['name_of_police_station'].'</td>
								</tr>
								<tr>
									<td>PIN Code </td>
									<td>'.$getCurrentData['loc_e_pin_number'].'</td>
								</tr>
							</table>
						</div></div>';

			
			$form['applicantLocation']['markup_start'] 	= array(
																'#markup'  => '<div class="content">
																					<div class="panel panel-default panel-form">
																						<div class="panel-heading">Status for the Amendment of Location of the Establishment</div>
																							<div class="panel-body">
																									<div class="alert alert-danger"><strong><i class="fa fa-exclamation-triangle" aria-hidden="true"></i> Note :</strong> Your application is <b>not accepted</b> and sent back for Rectification of the Location of the Establishment by the Registering Authority from your applied jurisdiction.
																<p>For editing the application click on <b>CONTINUE</b> or if you want to keep the Location of the Establishment as in your previous Registration Certificate, click on <b>DISMISS</b> to proceed further.
																								</div>
																							'.$output.'',
																'#type'    => 'markup',
													);
			$form['applicantLocation']['act_id'] 		= array(
															'#type' 		=> 	'hidden',
															'#default_value'=>  !empty($actId) ? $actid: '',
															'#attributes'	=> 	array('readonly'=>'readonly')
												);
			$form['applicantLocation']['applicationid'] = array(
															'#type' 		=> 	'hidden',
															'#default_value'=>  !empty($applicationId) ? $id: '',
															'#attributes'	=> 	array('readonly'=>'readonly')
															);
														
			$form['approval']['parent_id'] 				= array(
															'#type' 		=> 	'hidden',
															'#default_value'=>  !empty($getCurrentData[$amendmentParent]) ? encryption_decryption_fun('encrypt', $getCurrentData[$amendmentParent]): '',
															'#attributes'	=> 	array('readonly'=>'readonly')
															);	
			$form['approval']['currentAdd'] 			= array(
															'#type' 		=> 	'hidden',
															'#default_value'=>  !empty($getCurrentData[$addressLine]) ? encryption_decryption_fun('encrypt', ($getCurrentData[$addressLine])): '',
															'#attributes'	=> 	array('readonly'=>'readonly')
															);
													
			$form['applicantLocation']['remark'] 		= array(
															'#prefix'		=>'<div class="panel panel-info panel-form">',
															'#markup' 		=>'<div class="panel-heading">REMARK SUMMARY</div><div class="panel-body">'.get_amend_remark_details($getCurrentData[$amendmentParent],$getCurrentData[$applicantUserId]).'</div>',
															'#suffix' 		=> '</div>',
															);
			$form['applicantLocation']['continue'] 		= array (
															'#prefix' 		=> '',
															'#suffix' 		=> '',	
															'#type' 		=> 'submit',
															'#attributes' => array('class' => array('btn btn-primary pull-right acremark-btn mleft-15')),
															'#value' 		=> 'CONTINUE'
														);
													
			$form['applicantLocation']['dismiss'] 		= array (
															'#prefix' 		=> '',
															'#suffix' 		=> '</div></div></div>',  
															'#type' 		=> 'submit',
															'#attributes' 	=> array('class' => array('btn btn-primary pull-right acremark-btn')),
															'#value' 		=> 'DISMISS'
															);
			return $form;
													
		}
	}
	}else{
		drupal_set_message('You are not authorized to access this page.','error');	
	}
}

function get_amend_remark_details($parentId='',$userId=''){
	global $base_root, $base_path, $user;
	$counter = 0;
	$status	 = '';
	
	$header = array(
				array('data' => 'Sl. No.', 'width' => '7%'),
				array('data' => 'Date - Time', 'width' => '15%'),
				array('data' => 'Status', 'width' => '15%'),
				array('data' => 'Remark'),
				array('data' => 'Remark By', 'width' => '20%'),
				
			);
	$get_remark = db_select('l_clra_amendment', 'lrm');
	$get_remark	->leftJoin('l_custom_user_detail', 'lcud', 'lcud.usr_id = lrm.approved_by_uid');
	$get_remark->fields('lrm', array());
	$get_remark->fields('lcud', array('fullname','usr_type'));
	$get_remark->condition('lrm.applicant_uid',$userId);
	$get_remark->condition('lrm.application_id',$parentId);
	$get_remark->condition('lrm.status',1);
	$get_remark->orderBy('lrm.id','ASC');
	$get_remark_result = $get_remark->execute()->fetchAll(); 
	
	foreach($get_remark_result as $row){ 
		$counter 		= $counter+1;
		$remark_by		= $row->employee_id.'&nbsp;('.$row->name.')';
		if(!empty($row->flag)){
			if($row->flag == 'D'){
				$status = '<span class="amendment-accepted" title="Accepted By ALC"></span>';
			}else if($row->flag == 'R'){
				$status = '<span class="amendment-rejected" title="Reject Application"></span>';
			}
			$rows[] 		= array(
							array('data' => $counter, 'align' => 'left', 'class' => array('odd')),
							array('data' => date('dS M, Y - g:i a', strtotime($row->remark_date)), 'align' => 'left', 'class' => array('odd')),
							array('data' => $status, 'align' => 'left', 'class' => array('odd')),
							array('data' => $row->remarks, 'align' => 'left', 'class' => array('odd')),
							array('data' => $row->usr_type.'&nbsp;( ' .$row->usr_type.' )', 'align' => 'left', 'class' => array('odd')),
			  );
		}
	}
	$variables = array(
	  		'attributes' 		=> array('class' => array('table table table-striped table-responsive dashboard-table')), 
	  		'header' 			=> $header,
	  		'rows'				=> $rows,
			'empty' 			=> t("No data found!")
	  );
	  
	$output = theme('datatable', $variables);
	return $output;
}


function amendment_establishmentlocation_approval_submit($form, &$form_state){
	global $base_root, $base_path, $user;
	$val			 = $form_state['values'];

	$user_id		 = $user->uid;
	$actId			 = encryption_decryption_fun('decrypt', $val['act_id']);
	$applicationId	 = encryption_decryption_fun('decrypt', $val['applicationid']);
	$parentId		 = encryption_decryption_fun('decrypt', $val['parent_id']);
	$currentAddress	 = encryption_decryption_fun('decrypt',$val['currentAdd']);
	
	if($actId == 1){
		$masterTable 	= 'l_clra_registration_master';
		$amendTable		= 'l_clra_amendment';
		$remarkTable	= 'l_remarks_master';
		$addressLine	= 'loc_e_name';
		$district	 	= 'loc_e_dist';
		$subdivision 	= 'loc_e_subdivision';
		$areaType 	 	= 'loc_e_areatype';
		$areaCode	 	= 'name_areatype';
		$villWard	 	= 'loc_e_vill_ward';
		$ps			 	= 'l_e_ps';
		$pin		 	= 'loc_e_pin_number';
		$amendmentParent = 'amendment_parent_id';
	}
	
	if($val['op'] == 'DISMISS' && $val['dismiss'] == 'DISMISS'){
		
		$lastApplicationDetails = db_query("select id, status, amendment_parent_id,full_name_principal_emp, loc_e_name, loc_e_dist,loc_e_subdivision,loc_e_areatype,name_areatype,loc_e_vill_ward,l_e_ps, loc_e_pin_number FROM l_clra_registration_master WHERE user_id ='".$user->uid."' and id=".$parentId)->fetchAssoc();
		
		if(!empty($lastApplicationDetails) && $lastApplicationDetails['status'] == 'I'){
			
			$get_amended_fields	= db_select($amendTable, 'lca');
			$get_amended_fields	->fields('lca', array());
			$get_amended_fields	->condition('lca.application_id', $parentId);
			$get_amended_fields->condition('lca.status', 1);
			$get_amended_fields->orderBy('lca.id','DESC');
			$get_amended_fields->range(0, 1);
			$amended_fields_result  = $get_amended_fields->execute()->fetchAssoc();
			
			if(!empty($amended_fields_result)){
				$amendedFields = $amended_fields_result['amendment_fields'];
				$notAmendedFields = $amended_fields_result['not_amended_fields'];
					
				$amendedFieldArray = explode(",",$amendedFields);
				$notAmendedFieldArray = explode(",",$notAmendedFields);
				
				$findVariable = array("e_location", "location");
				$madeAmendedFields = str_replace($findVariable, "", $amendedFields);
				$madeNotAmendedFields = str_replace($findVariable, "", $notAmendedFields);
				
				$updatedAmendedFields = str_replace(',,', ",", $madeAmendedFields);
				$updatedNotAmendedFields = 'e_location,location,'.$madeNotAmendedFields;
				$finalNotAmendedFields = str_replace(',,', ",", $updatedNotAmendedFields);
				
				
				db_update($amendTable)->fields(array('amendment_fields'=> $updatedAmendedFields,'not_amended_fields'=>$finalNotAmendedFields))->condition('applicant_uid',$user_id)->condition('application_id',$parentId)->condition('status',1)->execute();
				db_update($masterTable)->fields(array('loc_e_name' => $lastApplicationDetails['loc_e_name'],'loc_e_dist'=>$lastApplicationDetails['loc_e_dist'],'loc_e_subdivision'=> $lastApplicationDetails['loc_e_subdivision'],'loc_e_areatype' => $lastApplicationDetails['loc_e_areatype'],'name_areatype' => $lastApplicationDetails['name_areatype'],'loc_e_vill_ward'=> $lastApplicationDetails['loc_e_vill_ward'],'l_e_ps'=>$lastApplicationDetails['l_e_ps'] ,'loc_e_pin_number'=>$lastApplicationDetails['loc_e_pin_number']))->condition('id',$applicationId)->condition('user_id',$user_id)->execute();
				drupal_goto('clra-reg-amendment/view-clra-application/'.encryption_decryption_fun('encrypt', $applicationId));
				
			}
		}
		
	}elseif($val['op'] == 'CONTINUE' && $val['continue'] == 'CONTINUE'){
		drupal_goto('apply-clra-reg-amendment/'.encryption_decryption_fun('encrypt', $parentId));
	}
}