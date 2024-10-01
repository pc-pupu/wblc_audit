<?php
function get_clra_contractor_info_form($form, &$form_state,$id='') {
	global $base_root,$base_path,$user;
		
	$user_id = $user->uid;
	$action	= 'decrypt';
	$action_encrypt	= 'encrypt';
	$application_id	= encryption_decryption_fun($action, $id);
	
	drupal_add_js(drupal_get_path('module', 'applicant_forms') . '/js/contractor_tab.js');	
						  
	$dataClraMaster = db_query("select lcrm.id, lcrm.final_submit_status, lcrm.identification_number, lcrm.status, lcrm.amendment_parent_id from l_clra_registration_master lcrm where lcrm.user_id=:user_id and lcrm.id=:applicationid", array(':user_id' => $user_id, ':applicationid' =>$application_id));
		
	if( $dataClraMaster->rowCount() > 0 ){
		
		while($clraMaster_res 	= $dataClraMaster->fetchAssoc()){ 
			$identification_number				= $clraMaster_res['identification_number'];
			$final_submit_status 				= $clraMaster_res['final_submit_status'];
			$status								= $clraMaster_res['status'];
			$amendment_parent_id 				= $clraMaster_res['amendment_parent_id'];
		}
	}
		
	$total_contractor = db_query("select id  from l_contractor_info_master where application_id=:applicationid",array(':applicationid'=>$application_id));		
		
	$form['contractor_markup'] = array(
						'#type'   => 'markup',
						'#markup' => '<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">Contractor List</div><div class="panel-body">',
						);	
																	
																	
	if( trim($status) == 'B' || trim($final_submit_status) == '' ) {	
		
	if($amendment_parent_id==''){
		
		$form['get_contractor_list']['contractor_markup1'] = array(
									'#type'   => 'markup',
									'#markup' => l('+ Add New Contractor', 'view-clra-application-details/add-contractor/'.encryption_decryption_fun($action_encrypt, $application_id), array('attributes' => array('html' => TRUE, 'class' => 'link-btn mtop-btm-15')))																			
									);
	}
	
	if($total_contractor->rowCount() <= 0){
		/* $form['get_contractor_list']['contractor_markup_2'] = array(
											'#type' => 'markup',
											'#markup' => '<div class="span_cls">In case of bulk contractors to be added at a time, please click '.l('here', 'view-clra-application-details/upload-contractor-doc-form/'.encryption_decryption_fun($action_encrypt, $application_id), array('attributes' => array('html' => TRUE,'class' => ''))).' and follow the instructions provided.</div>'); */
		}																			
	}
	
	$print_link = l('<div class="box-header with-border pull-right"><button type="button" class ="btn" style="margin:5px"><i class="fa fa-print"></i> Print</button></div>', 'view-clra-application-details/clra-contractor-info/print/'.$id, array('attributes' => array('target'=>'_blank'), 'html' => true, 'class' => 'link-btn mtop-btm-15'));
	
	$excel_link = l('<div class="box-header with-border pull-right"><button type="button" class ="btn" style="margin:5px"><img src="/sites/all/themes/officeadministrator/images/excel-icon.png" height="14"> Download Excel</button></div>', 'view-clra-application-details/clra-contractor-info/excel/'.$id, array('attributes' => array('target'=>'_blank'), 'html' => true, 'class' => 'link-btn mtop-btm-15'));		
	
	$form['get_contractor_list']['contractor_markup2'] = array(
						'#type'   => 'markup',
						'#markup' => '<div class="row">'.$print_link.$excel_link.'</div>'																			
						);
			
			
	$fetchDataInfo = GET_INSERTED_CONTRACTOR_LIST_TABLE_RESULT($status,$final_submit_status, $application_id,$amendment_parent_id); 
	$form['get_contractor_list']['showmsg'] = array(
			  '#type'	=> 'markup',
			  '#markup'	=> '<div class="table-scroller">'.$fetchDataInfo.'</div>',
			  '#suffix' => '</div></div></div>',
	 );	
		
	 return $form; 
}
	
	function get_clra_contractor_info_form_validate($form, &$form_state) {
		
	}
	
	function get_clra_contractor_info_form_submit($form, &$form_state){
		
	}
	
	function GET_INSERTED_CONTRACTOR_LIST_TABLE_RESULT($status, $final_submit_status, $application_id,$amendment_parent_id){
		global $base_root, $base_path, $user;
		
		ctools_include('modal');
		ctools_modal_add_js();
		
		$uid							=	$user->uid;
		$output 						= 	'';
		$application_status				= 	'';
		$service_name					=   '';
		$license_file_url = '';
		
		if($amendment_parent_id!='' || $amendment_parent_id!=0){
			
			$license_number 		= '';
			
			
			$header 				= array(
										array('data' => 'Sl. No', 'field' => 'slno'),
										array('data'=> 'Contractor Name<br/>License Number','width'=>'18%'),
										array('data'=> 'Nature of Work', 'field' => ''),
										array('data'=> 'Duration of Contract', 'field' => '','width'=>'12%'),
										array('data'=> 'No. of Contract Labour', 'field' => '','width'=>'13%'),
										array('data'=> 'Status', 'field' => ''),
										array('data'=> 'View & Download', 'field' => '','width'=>'16%')
									);
			
			
		}else{ 
		
			if($status=='I'){
				$sixth_array = 'Status';
			}else{
				$sixth_array = 'Active & Deactive';
			}
		
			$header 				= array(
										array('data' => 'Sl. No', 'field' => 'slno'),
										array('data'=> 'Contractor Name','width'=>'18%'),
										array('data'=> 'Nature of Work', 'field' => ''),
										array('data'=> 'Duration of Contract','width'=>'12%'),
										array('data'=> 'No. of Contract Labour','width'=>'13%'),
										array('data'=> $sixth_array, 'field' => ''),
										array('data'=> 'View & Download', 'field' => '','width'=>'16%')
									);
		}

		/** Contractor Details ***/
		
		$counter = 0;
				
		$query = 	db_select('l_contractor_info_master', 'lcim');
		$query->fields('lcim', array());
		$query->condition('lcim.application_id', $application_id);
		$query->condition('lcim.user_id', $user->uid);
		$query->orderBy('lcim.id','DESC');
		$contractors_list = $query->execute()->fetchAll();
		
		if(!empty($contractors_list)){			
			$Download_From_V =   '';
			$reasonStatus	 =   '';
										
			foreach($contractors_list as  $data){ 
			
				$counter++;
				$license_file_url = '';
				$license_number = '';
				$license_res = '';
				$license_certificate_id = '';
				$download_license_certificate = '';
			
				/**  EDIT  AND VIEW CONTRACTOR LINK **/
							
				if($amendment_parent_id!='' || $amendment_parent_id!=0){
					
					$edit				= '<i class="fa fa-chevron-circle-right"></i><span style="color:#aeaeae;"> Edit Details</span>';
					/*$view_details 		= '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">'.l('View Details','applicant-profile/clra-contractor-info/nojs/'.$data->id, array('attributes' => array('target' => '_blank', 'class' => 'ctools-use-modal')));*/
					$view_details 		= l('<i class="fa fa-chevron-circle-right"></i> View Details','contractor-view/'.encryption_decryption_fun('encrypt',  $data->act_id).'/'.encryption_decryption_fun('encrypt', $data->id), array('attributes' => array('target' => '_blank'), 'html' => TRUE));
					
				}else{
					
					if( $status == 'B' || $status ==''){
						$edit			= l('<i class="fa fa-chevron-circle-right"></i> Edit Details','applicant-profile/edit-contractor-form/'.encryption_decryption_fun('encrypt',  $data->id).'/'.encryption_decryption_fun('encrypt',  $application_id), array('attributes' => array(), 'html' => TRUE));
					}else{
						$edit			= '<i class="fa fa-chevron-circle-right"></i><span style="color:#aeaeae;"> Edit Details</span>';
					}
					
					/*$view_details 		= '<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">'.l('View Details','applicant-profile/clra-contractor-info/nojs/'.$data->id, array('attributes' => array('target' => '_blank', 'class' => 'ctools-use-modal')));*/
					$view_details 		= l('<i class="fa fa-chevron-circle-right"></i> View Details','contractor-view/'.encryption_decryption_fun('encrypt',  $data->act_id).'/'.encryption_decryption_fun('encrypt', $data->id), array('attributes' => array('target' => '_blank'), 'html' => TRUE));
					
				}
				
				
				
				/*** ACTIVE & DEACTIVE CONTRACTOR ****/
					
				if($data->status != 'I'){ 
					
					if($data->status == 1){
						$active_con_IMG 			=	'<i class="fa fa-check-circle fa-2x" style="color:green;" title="Click here to Inactive"></i> Active';
						$contractor_status			= 	l($active_con_IMG, 'contractor-status-active-deactive/'.encryption_decryption_fun('encrypt', $application_id).'/'.encryption_decryption_fun('encrypt', $data->id).'/'.encryption_decryption_fun('encrypt', 0), array('html' => TRUE));
					}else{
						$deactive_con_IMG 			=	'<i class="fa fa-check-circle fa-2x" style="color:red;" title="Click here to Active"></i> In Active';
						$contractor_status			= 	l($deactive_con_IMG, 'contractor-status-active-deactive/'.encryption_decryption_fun('encrypt', $application_id).'/'.encryption_decryption_fun('encrypt', $data->id).'/'.encryption_decryption_fun('encrypt', 1), array('html' => TRUE));
					}
					
				}
				
				
				if($data->is_from_v == 1 && $data->contractor_max_no_of_labours_on_any_day > 9){ 				
								
					/*** CONTRACTOR PRESENT ACTIVITY ***/
					
					$contractor_last_activity 	= db_select('l_contractor_activity_tag','lcat');
					$contractor_last_activity	->fields('lcat', array('id', 'application_status', 'flag','from_v_serial_no','created_by'));
					$contractor_last_activity	->condition('lcat.from_v_serial_no', $data->id);
					$contractor_last_activity	->orderBy('lcat.id', 'DESC');
					$contractor_last_activity	->range(0,1);						
					$activity_result 			= $contractor_last_activity->execute()->fetchAssoc();
						
					if(!empty($activity_result)){
						
						if($activity_result['application_status'] == 'I'){
							$classname='status-issued'; 
							$application_status = 'ISSUED</div>';
							$service_flag 		= $activity_result['flag'];
							
						}elseif($activity_result['application_status'] == 'R'){
							$classname='status-rejected'; 
							$application_status = 'REJECTED</div>';
							$service_flag 		= $activity_result['flag'];
							
						}else{ 
							$classname='status-in-process'; 
							$application_status = 'IN PROCESS</div>';
							$service_flag 		= $activity_result['flag'];
						}
						if($service_flag == 'L'){
							$service_name = '<div class="status-btm '.$classname.'">LICENSE&nbsp;';
						}elseif($service_flag == 'R'){
							$service_name = '<div class="status-btm '.$classname.'">RENEWAL&nbsp;';
						}elseif($service_flag == 'A'){
							$service_name = '<div class="status-btm '.$classname.'">AMENDMENT&nbsp;';
						}
						
					}else{
						$service_name 		= '';
						$application_status	= '<div class="status-btm status-not-applied">NOT APPLIED</div>';
					}
					
					if($data->contractor_type == 2){
						//$Download_From_V 		= l('<i class="fa fa-chevron-circle-right"></i> Download Form-V','pdf_from_v_download/'.$data->id,  array('html' => TRUE,'attributes' => array('target' => '_blank')));
						
						$Download_From_V 		= l('<i class="fa fa-chevron-circle-right"></i> Download Form-V','download_pdf_formV/'.encryption_decryption_fun('encrypt',  $data->id),  array('html' => TRUE,'attributes' => array('target' => '_blank')));
						
					}else{
						$Download_From_V		= '<span style="color:#337ab7;"><i class="fa fa-chevron-circle-right"></i> <strong>REF. NO- 00'.$data->id.'</strong></span>';
					}
										
					if($data->contractor_user_id!='' || $data->contractor_user_id!=0){ 
						
						//die($data->contractor_user_id);
						
						$license_res = basic_information_for_ammendment($data->id, $data->contractor_user_id);
						
						/*if($user->uid == 16532) { echo '<pre>'; print_r($license_res); echo $data->id.'----'.$data->contractor_user_id; die;}*/
						
						if(!empty($license_res)){
							
							$license_number 		= '<br/><strong>'.$license_res['contractor_license_number'].'</strong>';
							$license_certificate_id = $license_res['certificateid'];
							
							if($license_certificate_id!=''){
								
								/** LICENSE CERTIFICATE DETAILS ***/
								
								$license_certificate = fetch_uplaoded_file_details($license_certificate_id);
								
								if(!empty($license_certificate)){ 
								
									  $url					= explode('//',$license_certificate['uri']);
									  $license_file_url		= $url[1];
									  $license_file_name	= $license_certificate['filename'];
								}
							  
								$download_license_certificate = l('<i class="fa fa-chevron-circle-right"></i>  Download License',$base_root.$base_path.'sites/default/files/'.$license_file_url, array('html' => TRUE, 'attributes' => array('target' => '_blank')));
								
							}else{
								
								$download_license_certificate = '<i class="fa fa-chevron-circle-right"></i> <span style="color:#aeaeae;">Download License</span>';
							}
						}
						
					}else{
						
						$license_number 				= '';
						$download_license_certificate   = '<i class="fa fa-chevron-circle-right"></i> <span style="color:#aeaeae;">Download License</span>';
						
					}
					
				}else{
					
					if($status == 'I' && $data->is_from_v == 0){
						$application_status				= '<div class="status-btm status-not-reqd">DEACTIVATED</div>';	
					}elseif($status != 'I' && $data->is_from_v == 0){
						$application_status				= '<div class="status-btm status-not-applied" style="font-size:11px; width:160px;">APPLICATION NOT ISSUED</div>';	
					}else{
						$application_status				= '<div class="status-btm status-not-reqd">LICENSE NOT REQD.</div>';
					}
					
					$Download_From_V				= '<i class="fa fa-chevron-circle-right"></i> License Not Applicable';
					$service_name 					= '';
					$download_license_certificate	= '';
					$license_number					= '';
				}
			
				
				
				$contractor_wrk_name 		= db_select('l_clra_con_nature_of_work', 'lccnw');
				$contractor_wrk_name		->leftJoin('contractor_works', 'cw', 'cw.contractor_work_id=lccnw.contractor_nature_of_work');
				$contractor_wrk_name		->fields('lccnw', array('contractor_nature_of_work'));
				$contractor_wrk_name		->fields('cw', array('cont_work_name'));
				$contractor_wrk_name		->condition('lccnw.user_id', $user->uid);
				$contractor_wrk_name		->condition('lccnw.contractor_id', $data->id);
				$contractor_wrk_name		->condition('lccnw.application_id', $application_id);
				$result_2 					= $contractor_wrk_name->execute()->fetchAll(); 
				
				$cont_work_name				= '';
				$prefix						= '';
				
				foreach ($result_2 as  $con_nature_of_wrk){
					
					if($con_nature_of_wrk->contractor_nature_of_work == 28){
						if(!empty($data->other_nature_work)){
							$cont_work_name  	.= $data->other_nature_work;
						}else{
							$cont_work_name  	.= 'Others';
						}
					}else{
						$cont_work_name  	.= $prefix.$con_nature_of_wrk->cont_work_name;
						$prefix  	 		 = ', ';
					}
					
				}
				
				if($activity_result['application_status'] == 'R'){
					$Download_From_V	= '<i class="fa fa-chevron-circle-right"></i> <font style="color:#ff0;">FORM-V Rejected </font>';
					
				}
											
				if($amendment_parent_id!='' || $amendment_parent_id!=0){
					
					$rows[] = array( 
								$counter,
								$data->name_of_contractor.$license_number, 
								$cont_work_name,
								'<center>'.date('dS M, Y',strtotime($data->est_date_of_work_of_each_labour_from_date)).'<br/>
								<b>To</b><br/>'.date('dS M, Y',strtotime($data->est_date_of_work_of_each_labour_to_date)).'</center>',
								$data->contractor_max_no_of_labours_on_any_day,
								'<center>'.$service_name.$application_status.'</center>',
								$view_details.'<br/>'.$edit.'<br/>'.$Download_From_V.'<br/>'.$download_license_certificate,			
								);
							
				}else{
					
					if($status=='I'){
						$sixth_array_row = $service_name.$application_status;
					}else{
						$sixth_array_row = '<center>'.$contractor_status.'</center>';
						$Download_From_V = '';
					}
					
					$rows[] = array( 
								$counter,
								$data->name_of_contractor, 
								$cont_work_name,
								'<center>'.date('dS M, Y',strtotime($data->est_date_of_work_of_each_labour_from_date)).'<br/>
								<b>To</b><br/>'.date('dS M, Y',strtotime($data->est_date_of_work_of_each_labour_to_date)).'</center>',
								$data->contractor_max_no_of_labours_on_any_day,
								$sixth_array_row,
								$view_details.'<br/>'.$edit.'<br/>'.$Download_From_V.'<br/>'.$download_license_certificate,			
								);
				}							
			}
		}		
		
		$variables 	= 	array( 'header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('table table table-striped table-responsive dashboard-table')), 'empty' => t("No data found!"));
						
		$output = theme('datatable', $variables);
		
		return $output;
	}
	
	function contractor_view_callback($ajax, $wildcard){
		if ( $ajax == 'ajax' ) {
			
			ctools_include('ajax');
			ctools_include('modal');
	
			$form_state = array(
							'ajax' => TRUE,
							'title' => t('View Contractor Details'),
							'wildcard' => $wildcard
							);
			$output 	= ctools_modal_form_wrapper('contractor_view_old', $form_state);
	
			if (!empty($form_state['ajax_commands'])) {

				$output = $form_state['ajax_commands'];
			}
	
			print ajax_render($output);
			drupal_exit();
		}else {
			return drupal_get_form('contractor_view_old');
		}
	}
	
	/*** Function For Contractor View Modal Box ****/
	
	function contractor_view_old($form, &$form_state){
		
		$contractor_id 	 	= 	$form_state['wildcard'];
		
		$result 			= 	db_select('l_contractor_info_master', 'lcim')
									->fields('lcim')
									->condition('lcim.id', trim($contractor_id))
									->execute();
  		$data 				= 	$result->fetchObject();
		
		
		$query 				= 	db_select('l_clra_con_nature_of_work', 'lccnow');
		$query->fields('cw', array('cont_work_name'));
		$query->leftJoin('contractor_works', 'cw', 'cw.contractor_work_id=lccnow.contractor_nature_of_work');
		$query->condition('lccnow.contractor_id', $data->id);
		$resultum 			= 	$query->execute();
		
		$result_Id			=	array();
		while( $datum = $resultum->fetchObject() ){
			$result_Id[] 	= 	$datum->cont_work_name;
		}
		
		$nature_of_wrk 		= 	"";
		$prefix 			= 	'';
		foreach($result_Id as $val){
			$nature_of_wrk .= 	$prefix.$val;
			$prefix 		= 	', ';
		}
		
		///////////////////////////  Address View part In Modal Box  /////////////////////////////////
		
		$fetch_e_address		= 	db_select('l_contractor_info_master ', 'lcim');
		$fetch_e_address->fields('dm', array('district_name'));
		$fetch_e_address->fields('sd', array('sub_div_name'));
		$fetch_e_address->fields('bmm', array('block_mun_name'));
		$fetch_e_address->fields('vwm', array('village_name'));
		$fetch_e_address->fields('bor', array('borough_name'));
		$fetch_e_address->fields('ps', array('name_of_police_station'));
		
		$fetch_e_address->leftJoin('district_master', 'dm', 'dm.district_code=lcim.con_loc_e_dist');
		$fetch_e_address->leftJoin('sub_division', 'sd', 'sd.sub_div_code=lcim.con_loc_e_subdivision');
		$fetch_e_address->leftJoin('block_mun_master', 'bmm', 'bmm.block_code=lcim.con_name_areatype');
		$fetch_e_address->leftJoin('village_ward_master', 'vwm', 'vwm.village_code=lcim.con_loc_e_vill_ward');
		$fetch_e_address->leftJoin('borough_master', 'bor', 'bor.borough_code=lcim.con_loc_e_borough');
		$fetch_e_address->leftJoin('police_station', 'ps', 'ps.police_station_code=lcim.con_l_e_ps');
		
		$fetch_e_address->condition('lcim.id', $contractor_id);
		$resultadd 				= 	$fetch_e_address->execute();
		$data_add 				= 	$resultadd->fetchObject();
		
		
		if($data->con_loc_e_areatype=="B"){
			$text_AreaType = "Block";
		}elseif($data->con_loc_e_areatype=="M"){
			$text_AreaType = "Municipality";
		}elseif($data->con_loc_e_areatype=="C"){
			$text_AreaType = "Corporation";
		}elseif($data->con_loc_e_areatype=="S"){
			$text_AreaType = "SEZ";
		}elseif($data->con_loc_e_areatype=="N"){
			$text_AreaType = "Notified Area";
		}elseif($data->con_loc_e_areatype=="O"){
			$text_AreaType = "Other";
		}
		
		
		if($text_AreaType == 'Corporation'){
			$text_AreaType_name	='Ward';
			$areatype_name 		= $data_add->village_name;
		}elseif($text_AreaType == "Municipality"){
			$text_AreaType_name	='Ward';
			$areatype_name 		= 'Ward -'.$data_add->village_name;
		}else{
			$text_AreaType_name	='Gram Panchayat';	
			$areatype_name 		= ucwords($data_add->village_name);
		}
		
		if($data->contractor_type == 1){
			$contractor_type = 'Offline generated License';
		}else{
			$contractor_type = 'New Contractor';
		}
		
		$output = '';
		$output .= '<div class="content">
						<div class="sky-form">
							<header><h3><b>Particulars of Contractors and Contract Labour</h3></b></header>
							<fieldset>
							
								<section>
									<label class="input">Contractor is
										<input type="text" value="'.$contractor_type.'" disabled="disabled">
									</label>
								</section>
														
								<div class="row">
									<section class="col-md-6">
										<label class="input">1. Name of the Contractor
											<input type="text" value="'.$data->name_of_contractor.'" disabled="disabled">
										</label>
									</section>
									
									<section class="col-md-6">
										<label class="input">1. Email of the Contractor
											<input type="text" value="'.$data->email_of_contractor.'" disabled="disabled">
										</label>
									</section>
									
								</div>
								
								<section>
									<label class="textarea">3. Address of the Contractor
										<textarea rows="2" disabled="disabled">'.$data->address_of_contractor.'</textarea>
									</label>
								</section>
								
								<div class="row">
									<section class="col-md-6">
										<label class="input">3.a.) District of the Contractor
											<input type="text" value="'.$data_add->district_name.'" disabled="disabled">
										</label>
									</section>
									<section class="col-md-6">
										<label class="input">3.b.) Subdivision of the Contractor
											<input type="text" value="'.$data_add->sub_div_name.'" disabled="disabled">
										</label>
									</section>
								</div>
								<div class="row">
									<section class="col-md-6">
										<label class="input">3.c.) Areatype of the Contractor
											<input type="text" value="'.$text_AreaType.'" disabled="disabled">
										</label>
									</section>
									<section class="col-md-6">
										<label class="input">3.d.) '.$text_AreaType.' of the Contractor
											<input type="text" value="'.$data_add->block_mun_name.'" disabled="disabled">
										</label>
									</section>
								</div>
								<div class="row">
									<section class="col-md-6">
										<label class="input">3.e.) '.$text_AreaType_name.' of the Contractor
											<input type="text" value="'.$areatype_name.'" disabled="disabled">
										</label>
									</section>
									<section class="col-md-6">
										<label class="input">3.f.) Police Station of the Contractor
											<input type="text" value="'.$data_add->name_of_police_station.'" disabled="disabled">
										</label>
									</section>
								</div>
								
								
								<section>
									<label class="textarea">4.Nature of Work in which Contract Labour is Employed or is to be Employed.<br>
										<textarea rows="2" disabled="disabled">'.$nature_of_wrk.'</textarea>
									</label>
								</section>
								
								<div class="row">
									<section class="col-md-6">
										<label class="input">5. Maximum Number of Contractor Labour to be Employed on any day Through Each Contractor
											<input type="text" value="'.$data->contractor_max_no_of_labours_on_any_day.'" disabled="disabled">
										</label>
									</section>
								
									<section class="col-md-6">
										<label class="input">6. Estimated Date of Employment of Each Contract Work Under Each Contractor
											<input type="text" value="'.date('dS M, Y',strtotime($data->est_date_of_work_of_each_labour_from_date)).'&nbsp;to&nbsp;'.date('dS M, Y',strtotime($data->est_date_of_work_of_each_labour_to_date)).'" disabled="disabled"></textarea>
										</label>
									</section>
								</div>	
							</fieldset>
							
							
							<footer>
								<div class="row">&nbsp;&nbsp;</div><div class="row">&nbsp;&nbsp;</div>
							</footer>
						</div>
				 </div>';

		$form['viewData'] 		= 	array(
									  '#prefix' => '<div id="">',
									  '#markup' => ''.$output.'',
									  '#suffix' => '</div>',   
          							);
		return $form;
	}
	
	
	
	
	function generateRandomString($length = 6) {
		$characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
		$charactersLength = strlen($characters);
		$randomString = '';
		for ($i = 0; $i < $length; $i++) {
			$randomString .= $characters[rand(0, $charactersLength - 1)];
		}
		return $randomString;
	}
	
	function contractor_status_activate_deactivate($form, &$form_state , $id='', $con_id='', $status=''){
		
		if($id !='' || !empty($con_id) && !empty($status)){
			
			$application_id								= encryption_decryption_fun('decrypt',$id);
			$contractor_id								= encryption_decryption_fun('decrypt',$con_id);	
			$active_status								= encryption_decryption_fun('decrypt',$status);						
			
			
			$update_contractor_status 					=  	db_update('l_contractor_info_master');
			$update_contractor_status					->	fields(array('status'  => $active_status));
			$update_contractor_status					->	condition('application_id',$application_id);
			$update_contractor_status					->	condition('id',$contractor_id);
			$update_contractor_status					->	execute();
			
		}
		
		drupal_goto('view-contractors-form-v/'.encryption_decryption_fun('encrypt',$application_id));
		
		
		
	}
	