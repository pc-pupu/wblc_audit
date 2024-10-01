<?php 
drupal_add_js(drupal_get_path('module', 'alc').'/js/mycustom.js'); 
function alc_amendment_approval($form, &$form_state,$id = '', $actid=''){
	
	global $base_root, $base_path, $user;

	$actId				= encryption_decryption_fun('decrypt', $actid);
	$applicationId		= encryption_decryption_fun('decrypt', $id);
	$alcUserId			= $user->uid;
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
		$amendmentParent = 'amendment_parent_id';
		$certificateFid	= 'certificates_fid';
		$baseCertificateUrl	=  $base_root.'/sites/default/files/';
		$tradeLicenseUrl = $base_root.'/sites/default/files/upload/trade_license/';
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
	$masterData->fields('master',array($amendmentParent,$addressLine,$areaType,$pin,$applicantUserId,$certificateFid));
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
	
	
	/** For Current Document Section ***/
	/*$currentEnc_doc_array 	  = array('TL'=> '');
	$currentfetchEncryptedDoc = db_query('select * from l_encrypted_uploaded_documents where act_id='.$actId.' and application_id = '.$applicationId)->condition('status','1')->fetchAll(); print_r($currentfetchEncryptedDoc);exit;
	if(!empty($currentfetchEncryptedDoc)){
		foreach($currentfetchEncryptedDoc as $data ){
			$currentEnc_doc_array[$data->document_type_code]['content'] = 'view_documents/'.encryption_decryption_fun('encrypt',$applicationId);
		}
	}*/
	
	$getCurrentTradeLicense = db_query('select trade_license_file from l_documents_upload where act_id='.$actId.' and application_id = '.$applicationId)->fetchAssoc();
	
	if($currentEnc_doc_array['TL']['content'] !=''){ 
		$currentTradeLicenseUrl = '<a title="" href="'.$currentEnc_doc_array['TL']['content'].'" target="_blank"><img src="'.$base_root.'sites/all/themes/jackson/images/pdf.png">
		&nbsp;&nbsp;<span class="viewfileins" id="tradeLicense_file"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
		<div id="fileins_tradeLicense_file"  title="Current Trade License" style="display:none;">
			<iframe src="'.$currentEnc_doc_array['TL']['content'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
		</div></a>';
	}else{
		$currentTradeLicenseUrl = '<a title="" href="'.$tradeLicenseUrl.$getCurrentTradeLicense['trade_license_file'].'" target="_blank"><img src="'.$base_root.'/sites/all/themes/jackson/images/pdf.png">&nbsp;&nbsp;
		<span class="viewfileins" id="fid_tradeLicense_file"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
		<div id="fileins_fid_tradeLicense_file"  title="Current Trade License" style="display:none;">
			<iframe src="'.$tradeLicenseUrl.$getCurrentTradeLicense['trade_license_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
		</div></a>';
	}
	
	/** Current Registration Certificate **/
	
	if($getCurrentData[$certificateFid]!='' && $getCurrentData[$certificateFid]!=0){
		$getCurrentCertificate = db_query('select * from file_managed where fid = '.$getCurrentData[$certificateFid])->fetchAssoc();
			if(!empty($getCurrentCertificate)){
				$url					 = explode('//',$getCurrentCertificate['uri']);
				$currentCertificateUrl	 = $url[1];
			}
		$currentRegistrationCertificateUrl = '<a title="Click here to view work order" href="'.$baseCertificateUrl.$currentCertificateUrl.'" target="_blank"><img src="'.$base_root.'/sites/all/themes/jackson/images/pdf.png"></a>';
	}else{
		$currentRegistrationCertificateUrl = 'No document uploaded';
	}
	
	
	
	
	if($getCurrentData[$amendmentParent]!='' && $getCurrentData[$amendmentParent]!=0){
		
		$previousData = db_select($masterTable,'master');
		$previousData->fields('master',array($amendmentParent,$addressLine,$areaType,$pin,$applicantUserId,$certificateFid));
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
		
		/*$previousEnc_doc_array 	  = array('TL'=> '');
	$previousfetchEncryptedDoc = db_query('select * from l_encrypted_uploaded_documents where act_id='.$actId.' and application_id = '.$applicationId)->condition('status','1')->fetchAll(); print_r($currentfetchEncryptedDoc);exit;
	if(!empty($previousfetchEncryptedDoc)){
		foreach($currentfetchEncryptedDoc as $data ){
			$previousfetchEncryptedDoc[$data->document_type_code]['content'] = 'view_documents/'.encryption_decryption_fun('encrypt',$applicationId);
		}
	}*/
		$getPreviousTradeLicense = db_query('select trade_license_file from l_documents_upload where act_id='.$actId.' and application_id = '.$getCurrentData[$amendmentParent])->fetchAssoc();
		
		if($previousEnc_doc_array['TL']['content'] !=''){ 
			$previousTradeLicenseUrl = '<a title="" href="'.$previousEnc_doc_array['TL']['content'].'" target="_blank"><img src="'.$base_root.'sites/all/themes/jackson/images/pdf.png">
			&nbsp;&nbsp;<span class="viewfileins" id="ptradeLicense_file"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
			<div id="fileins_ptradeLicense_file"  title="Current Trade License" style="display:none;">
				<iframe src="'.$previousEnc_doc_array['TL']['content'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
			</div></a>';
		}else{
			$previousTradeLicenseUrl = '<a title="" href="'.$tradeLicenseUrl.$getPreviousTradeLicense['trade_license_file'].'" target="_blank"><img src="'.$base_root.'/sites/all/themes/jackson/images/pdf.png">&nbsp;&nbsp;
			<span class="viewfileins" id="pfid_tradeLicense_file"><i class="fa fa-search-plus fa-lg" aria-hidden="true"></i></span>
			<div id="fileins_pfid_tradeLicense_file"  title="Previous Trade License" style="display:none;">
				<iframe src="'.$tradeLicenseUrl.$getPreviousTradeLicense['trade_license_file'].'" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
			</div></a>';
		}
		
		/**Previous Registration Certificate **/
		if($getPreviousData[$certificateFid]!='' && $getPreviousData[$certificateFid]!=0){
			$getPreviousCertificate = db_query('select * from file_managed where fid = '.$getPreviousData[$certificateFid])->fetchAssoc();
			if(!empty($getPreviousCertificate)){
				$url					 = explode('//',$getPreviousCertificate['uri']);
				$previousCertificateUrl	 = $url[1];
			}
			$previousRegistrationCertificateUrl = '<a title="Click here to view work order" href="'.$baseCertificateUrl.$previousCertificateUrl.'" target="_blank"><img src="'.$base_root.'/sites/all/themes/jackson/images/pdf.png"></a>';
		}else{
			$previousRegistrationCertificateUrl ='No document uploaded';
		}
		
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
		
			$output	 .= '<div class="col-md-6">
							<table class="table table-striped table-responsive admin-custom-table">
								<tr>
								  <th style="width:40%" colspan="2">Previous Location of the Establishment </th>
								</tr>
								<tr>
									<td>House/Flat No., Street/Village/ Locality Name </td>
									<td>'.$getPreviousData['loc_e_name'].'</td>
								</tr>
								<tr>
									<td>District </td>
									<td>'.$getPreviousData['district_name'].'</td>
								</tr>
								<tr>
									<td>Subdivision </td>
									<td>'.$getPreviousData['sub_div_name'].'</td>
								</tr>
								<tr>
									<td>Area Type </td>
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
									<td>PIN Code </td>
									<td>'.$getPreviousData['loc_e_pin_number'].'</td>
								</tr>
								<tr>
									<td>Previous Trade License</td>
									<td>'.$previousTradeLicenseUrl.'</td>
								</tr>
								<tr>
									<td>Registration Certificate</td>
									<td>'.$previousRegistrationCertificateUrl.'</td>
								</tr>
							</table>
						</div>
						<div class="col-md-6">
							<table class="table table-striped table-responsive admin-custom-table">
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
								<tr>
									<td>Current Trade License</td>
									<td>'.$currentTradeLicenseUrl.'</td>
								</tr>
								<tr>
									<td>Registration Certificate</td>
									<td>'.$currentRegistrationCertificateUrl.'</td>
								</tr>
							</table>
						</div>';
						
			$amended_fields	= db_select($amendTable, 'lca');
			$amended_fields	->leftJoin('l_custom_user_detail', 'cus_detl','cus_detl.usr_id = lca.approved_by_uid');
			$amended_fields	->fields('cus_detl',array('fullname','employee_id'));
			$amended_fields	->fields('lca', array());
			$amended_fields	->condition('lca.application_id', $getCurrentData[$amendmentParent]);
			$amended_fields->condition('lca.applicant_uid', $getCurrentData[$applicantUserId]);
			$amended_fields->condition('lca.status', 1);
			$amended_fields->orderBy('lca.id', 'DESC');
			$amended_fields->range(0, 1);
			$amendedBy 	= $amended_fields->execute()->fetchAssoc();
			if(!empty($amendedBy)){
				
				$get_remark = db_select('l_remarks_master', 'lrm');
				$get_remark->fields('lrm', array('remark_field_text','remark_by','remark_by_role'));
				$get_remark->condition('lrm.remark_to',$getCurrentData[$applicantUserId]);
				$get_remark->condition('lrm.application_id', $applicationId);
				$get_remark->condition('lrm.remark_by', $getCurrentData[$applicantUserId], '!=');
				$get_remark->orderBy('lrm.remark_id','DESC');
				$get_remark->range(0, 1);
				$get_remark_result = $get_remark->execute()->fetchAssoc();
				
				
				if($amendedBy['flag'] == 'D'){
					$message = '<div class="col-md-12"><div class="callout callout-success"><i class="fa fa-check" aria-hidden="true"></i>
 The amendment for Location of the Establishment is <b>APPROVED</b> by '.$amendedBy['fullname'].'&nbsp; <b> EMPID ['.$amendedBy['employee_id'].']</b></div></div>';
										
					$goTo = l('<i class="fa fa-arrow-left" aria-hidden="true"></i>&nbsp;
 Back To Application', 'alc-view-amended-application/'.encryption_decryption_fun('encrypt', $applicationId).'/'.encryption_decryption_fun('encrypt', $getCurrentData[$applicantUserId]), array('html' => TRUE, 'attributes' => array('class' => 'link-btn mtop-btm-15')));
					
				}else if($amendedBy['flag'] == 'R'){
					$message = '<div class="col-md-12"><div class="callout callout-success"><i class="fa fa-times" aria-hidden="true"></i>
The amendment for Location of the Establishment is <b>REJECTED</b> by '.$amendedBy['fullname'].'&nbsp;<b>EMPID ['.$amendedBy['employee_id'].']</b></div></div>';
					$goTo = '';
					
				}
			}
						
		
			$form['approval']['markup_start'] 		= array(
														'#markup' 		=> '<div class="box box-primary box-solid"><div class="box-header with-border"><h3 class="box-title">Approval for the Amendment of Location of the Establishment</h3></div><div class="box-body">
															<div class="col-md-12"><div class="alert alert-info"><strong><i class="fa fa-exclamation-triangle" aria-hidden="true"></i> Note :</strong> The application is sent to the Registering Authority for acceptance/rejection of Location of the Establishment. Here, provided both the previous location and current location (for acceptance/rejection) of the Establishment.
															<p>If accepted, the registration number will be changed with respect to the new jurisdiction, and, if rejected, the application will be sent back to the applicant for rectification to rectify location of the Establishment so that it can be addressed by the amended jurisdiction.</p> </div></div>'.$output.$message.'<div class="col-md-12">'.$goTo.'</div>',
														'#type' 		=> 'markup',
													);
													
			$form['approval']['markup_end'] 		= array(
														'#markup' 		=> '</div></div>',
														'#type' 		=> 'markup',
													);
			$form['approval']['act_id'] 			= array(
														'#type' 		=> 	'hidden',
														'#default_value'=>  !empty($actId) ? $actid: '',
														'#attributes'	=> 	array('readonly'=>'readonly')
												);
			$form['approval']['applicationid'] 		= array(
														'#type' 		=> 	'hidden',
														'#default_value'=>  !empty($applicationId) ? $id: '',
														'#attributes'	=> 	array('readonly'=>'readonly')
														);
			$form['approval']['applicant_userid'] 	= array(
														'#type' 		=> 	'hidden',
														'#default_value'=>  !empty($getCurrentData[$applicantUserId]) ? encryption_decryption_fun('encrypt', $getCurrentData[$applicantUserId]): '',
														'#attributes'	=> 	array('readonly'=>'readonly')
														);
														
			$form['approval']['parent_id'] 		= array(
														'#type' 		=> 	'hidden',
														'#default_value'=>  !empty($getCurrentData[$amendmentParent]) ? encryption_decryption_fun('encrypt', $getCurrentData[$amendmentParent]): '',
														'#attributes'	=> 	array('readonly'=>'readonly')
														);	
			$form['approval']['currentAdd'] 		= array(
														'#type' 		=> 	'hidden',
														'#default_value'=>  !empty($getCurrentData[$addressLine]) ? encryption_decryption_fun('encrypt', ($getCurrentData[$addressLine])): '',
														'#attributes'	=> 	array('readonly'=>'readonly')
														);
			
			if( ($alcUserId == $amendedBy['approved_by_uid'] || $amendedBy['approved_by_uid']=='') || ($get_remark_result['remark_by']==$alcUserId && $get_remark_result['remark_by_role'] == 4 )){
				$form['approval']['remark_type'] 			= array(
															'#prefix' 		=> '<div class="box box-primary"><div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">ACTIONS AND REMARKS</h3></div><div class="box-body"><div class="form-custom col-md-6 col-md-offset-3"><label class="input">',
															'#suffix' 		=> '<i></i></label>',
															'#title'	    => 'Please Select Action',
															'#type'		    => 'select',
															'#id'			=> 'remark_type_id',
															'#required'     =>  TRUE,
															'#attributes'   => array('onchange' => 'getRemarkTypeAction(this.value)', 'class'=>array('form-control')),
															'#options'	    => array('D' => 'Request Accepted','R' => 'Reject & Sent Back for Rectification'),
															'#ajax'		    => array(
																				'event' 	=> 'change',
																				'method' 	=> 'append',
																				'wrapper' 	=> 'content-wrapper',
																				'effect' 	=> 'fade',						
																				'callback'	=> 'get_ajax_remarkTextarea',
																				'progress'	=> array('type' => 'throbber'),
																			  ),
															);																				
		
														
				$remarkType 	= isset($form_state['values']['remark_type']) ? $form_state['values']['remark_type'] : 0;
				
				$form['approval']['remarkTextarea']  = array(
														  '#prefix' 	=> '<div id="remarkTextarea_replace">',
														  '#suffix' 	=> '</div>',
														  );
				$form['approval']['submit'] 		= array(
														  '#prefix' 	=> '<div id="submit_block">',
														  '#suffix' 	=> '</div>'
															);
				if(!empty($remarkType)){
					$form['approval']['remarkTextarea']  = array(
														  '#prefix' 	=> '<div id="remarkTextarea_replace"><label class="input label-block">',
														  '#suffix' 	=> '</label></div>',
														  '#type' 		=> 'textarea',
														  '#rows'		=>  2,
														  '#required' 	=>  TRUE,
														  '#attributes' 	=> array('autocomplete' => 'off', 'class' => array('form-control'), 'id' => 'remarks_text_id'),
														  );	
					$form['approval']['submit'] 		= array (
														  '#prefix' 	=> '<div id="submit_block">',
														  '#suffix' 	=> '</div></div></div></div>',	
														  '#type' 		=> 'submit',
														  '#attributes' => array('class' => array('btn btn-primary pull-left acremark-btn')),
														  '#value' 		=> 'Submit'
													  
													);
				}
				
				$form['approval']['submit'] 		= array (
														  '#prefix' 	=> '<div id="submit_block">',
														  '#suffix' 	=> '</div></div><div class="col-md-3">&nbsp;</div></div></div></div>',	
														  '#type' 		=> 'submit',
														  '#attributes' => array('class' => array('btn btn-primary pull-left acremark-btn')),
														  '#value' 		=> 'Submit'
													  
													);
				
													
				
			}
			$form['approval']['markup_data_5'] 		= array(
														'#prefix'		=>'<div class="box box-primary box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">REMARK SUMMARY</h3></div><div class="box-body">',
														'#markup' 		=> '<div class="feedback-scroll">'.get_amend_remark_details($getCurrentData[$amendmentParent],$getCurrentData[$applicantUserId]).'</div>',
														'#suffix' 		=> '</div>',
														);
			return $form;
		}
	}
}
function get_ajax_remarkTextarea($form, $form_state){
	$commands 	= array();
	$commands[] = ajax_command_replace('#remarkTextarea_replace', drupal_render($form['approval']['remarkTextarea']));
	$commands[] = ajax_command_replace('#submit_block', drupal_render($form['approval']['submit']));
	return array('#type' => 'ajax', '#commands' => $commands);	
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
							array('data' => $row->fullname.'&nbsp;( ' .$row->usr_type.' )', 'align' => 'left', 'class' => array('odd')),
			  );
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

function alc_amendment_approval_submit($form, &$form_state){
	global $base_root, $base_path, $user;
	$val			 = $form_state['values'];
	$user_id		 = $user->uid;
	$actId			 = encryption_decryption_fun('decrypt', $val['act_id']);
	$applicationId	 = encryption_decryption_fun('decrypt', $val['applicationid']);
	$parentId		 = encryption_decryption_fun('decrypt', $val['parent_id']);
	$applicantUserId = encryption_decryption_fun('decrypt',$val['applicant_userid']);	
	$currentAddress	 = encryption_decryption_fun('decrypt',$val['currentAdd']);
	$remarkTextarea	 = $val['remarkTextarea'];
	$remark_type 	 = $val['remark_type'];
	
	/*** Get User role of ALC **/
	
	$fetch_users_role		= db_select('users_roles', 'ur');
	$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role->leftJoin('l_custom_user_detail', 'lcud', 'ur.uid=lcud.usr_id');
	$fetch_users_role->fields('ro', array('rid'));
	$fetch_users_role->fields('lcud', array('fullname','employee_id'));
	$fetch_users_role->condition('ur.uid', $user_id);
	$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc();

	$remark_by_role			=	$fetch_users_role_data['rid'];
	$alc_full_name			=   $fetch_users_role_data['fullname'];	
	$employee_id			= 	$fetch_users_role_data['employee_id'];
	
	if($actId == 1){
		$master = 'l_clra_registration_master';
		$amend	= 'l_clra_amendment';
		$remark = 'l_remarks_master';
	}
	$amended_fields	= db_select($amend, 'lca');
	$amended_fields	->fields('lca', array());
	$amended_fields	->condition('lca.application_id', $parentId);
	$amended_fields->condition('lca.applicant_uid', $applicantUserId);
	$amended_fields->orderBy('lca.id', 'DESC');
	$amended_fields->range(0, 1);
	$amendedBy 	= $amended_fields->execute()->fetchAssoc();
	
	$presentApplicationDetails = db_query("select loc_e_name, loc_e_dist,loc_e_subdivision,loc_e_areatype,name_areatype,loc_e_vill_ward,l_e_ps, loc_e_pin_number FROM l_clra_registration_master WHERE user_id ='".$applicantUserId."' and id=".$applicationId)->fetchAssoc();
	if(!empty($presentApplicationDetails)){
		$establishment_address 	= $presentApplicationDetails['loc_e_name'].','.$presentApplicationDetails['loc_e_dist'].','.$presentApplicationDetails['loc_e_subdivision'].','.$presentApplicationDetails['loc_e_areatype'].','.$presentApplicationDetails['name_areatype'].','.$presentApplicationDetails['loc_e_vill_ward'].','.$presentApplicationDetails['l_e_ps'].','.$presentApplicationDetails['loc_e_pin_number'];
	}
	

	
	if($remark_type == 'D'){
		
		db_update($amend)->fields(array('approved_by_uid' => $user_id,'employee_id' => $employee_id,'flag' => 'D','remark_date' => date('Y-m-d'),'amend_location'=> $establishment_address,'remarks' => $remarkTextarea))->condition('application_id',$parentId)->condition('status',1)->execute();
		db_update($master)->fields(array('status' => '0'))->condition('id',$applicationId)->condition('user_id',$applicantUserId)->execute();
		db_insert($remark)->fields(array('remarks'=> $remarkTextarea,'remark_by' => $applicantUserId,'remark_to' => $applicantUserId,'application_id'=> $applicationId, 'remark_type' => 'D','remark_date' => time(),'remark_by_name' => $alc_full_name,'remark_by_role' => 4, 'is_active' => 1, 'hrms_employee_id'=> $employee_id))->execute();
		drupal_goto('alc-view-amended-application/'.encryption_decryption_fun('encrypt', $applicationId).'/'.encryption_decryption_fun('encrypt', $applicantUserId));
		
	}else if($remark_type == 'R'){
		
			db_update($amend)->fields(array('approved_by_uid' => $user_id,'employee_id' => $employee_id,'flag' => 'R','remark_date' => date('Y-m-d'),'remarks' => $remarkTextarea))->condition('application_id',$parentId)->condition('status',1)->execute();
			db_insert($amend)->fields(array('application_id' => $parentId,'status'=>0,'amend_apply_date'=> $amendedBy['amend_apply_date'],'amendment_fields'=> $amendedBy['amendment_fields'],'applicant_subdivision_code' => $amendedBy['applicant_subdivision_code'],'applicant_block_code' => $amendedBy['applicant_block_code'],'applicant_uid' =>  $applicantUserId,'remark_date'=> date('Y-m-d'),'not_amended_fields' => $amendedBy['not_amended_fields'],'approved_by_uid' => $user_id,'employee_id' => $employee_id,'flag' => 'R','amend_location'=> $establishment_address,'remarks' => $remarkTextarea))->execute();
			db_update($master)->fields(array('status' => 'B'))->condition('id',$applicationId)->condition('user_id',$applicantUserId)->execute();
			db_insert($remark)->fields(array('remarks'=> $remarkTextarea,'remark_by' => $user_id,'remark_to' => $applicantUserId,'application_id'=> $applicationId, 'remark_type' => 'B','remark_date' => time(),'remark_by_name' => $alc_full_name,'remark_by_role' => 4, 'is_active' => 1, 'hrms_employee_id'=> $employee_id,'remark_field_text' =>$amendedBy['not_amended_fields']))->execute();
		drupal_set_message(t('Amendment of the Location of the Establishment is Rejected and the application is sent back for Rectification.'),'error');
		
	}
	
}
