<?php
	function get_clra_contractor_info_form($form, &$form_state,$id='') {
		
		global $base_root,$base_path,$user;
		
		$user_id 			= $user->uid;
	    $application_id		= encryption_decryption_fun('decrypt', $id);
		
		drupal_add_js(drupal_get_path('module', 'applicant_forms') . '/js/contractor_tab.js');
		
		$total_contractor 	= db_query("select id from l_contractor_info_master where application_id=:applicationid",array(':applicationid'=>$application_id));
		$clra_masterData 	= db_query("select lcrm.id, lcrm.final_submit_status, lcrm.identification_number, lcrm.status,lcrm.amendment_parent_id from l_clra_registration_master lcrm where lcrm.user_id=:user_id and lcrm.id=:applicationid", array(':user_id' => $user_id, ':applicationid' =>$application_id));
		
		if( $clra_masterData->rowCount() > 0 ){
			
			while($clra_masterData_res 	= $clra_masterData->fetchAssoc()){ 
				$identification_number	= $clra_masterData_res['identification_number'];
				$final_submit_status 	= $clra_masterData_res['final_submit_status'];
				$status					= $clra_masterData_res['status'];
				$amendment_parent_id 	= $clra_masterData_res['amendment_parent_id'];
			}
		}
																	
		if( trim($status) =='B' || trim($final_submit_status) =='' ) {	
		
			$form['contractor_markup']				= array(
															'#type' 	=> 'markup',
															'#markup' 	=> '<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">Contractor List</div><div class="panel-body">',
														);	
		
										
			$form['contractor_list']['markup1']		= array(
															'#type' 	=> 'markup',
															'#markup' 	=> l('+ Add New Contractor', 'add-contractor/'.encryption_decryption_fun('encrypt', $application_id), array('attributes' => array('html' => TRUE, 'class' => 'link-btn mtop-btm-15')))																			
															);
											
			/*if($total_contractor->rowCount() <= 0){
			$form['get_contractor_list']['contractor_markup_2']							=	array(
																			'#type' 			=> 'markup',
																			'#markup' 			=> '<div class="span_cls">In case of bulk contractors to be added at a time, please click '.l('here', 'view-clra-application-details/upload-contractor-doc-form/'.encryption_decryption_fun($action_encrypt, $application_id), array('attributes' => array('html' => TRUE,'class' => ''))).' and follow the instructions provided.</div>');
			}	*/																		
	
			$fetchDataInfo				=	GET_INSERTED_CONTRACTOR_LIST_TABLE_RESULT($status,$final_submit_status, $application_id); 
			$form['contractor_list']['showmsg'] 	= array(
														  '#type'		=> 'markup',
														  '#markup'		=> $fetchDataInfo,
														  '#suffix' 	=> '</div></div></div>',
													  );
		}else{
			drupal_set_message('You are not authorized to access this page','error');
		}
		
		return $form; 
	}
	
	function get_clra_contractor_info_form_validate($form, &$form_state) {
		
	}
	
	function get_clra_contractor_info_form_submit($form, &$form_state){
		
	}
	
	function GET_INSERTED_CONTRACTOR_LIST_TABLE_RESULT($status, $final_submit_status, $application_id){
		global $base_root, $base_path, $base_url, $user;
		
		drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
		drupal_add_js(drupal_get_path('module', 'amendment_clra') . '/js/custom.js');
		
		ctools_include('modal');
		ctools_modal_add_js();
		
		$uid							=	$user->uid;
		$output 						= 	'';
		$header 						= 	array(
												array('data' => 'Sl. No', 'field' => 'slno', 'width' => '5%'),
												array('data'=> 'Contractor Name', 'field' => 'name_of_contractor', 'width' => '15%'),
												array('data'=> 'Contractor Address', 'field' => 'address_of_contractor', 'width' => '10%'),
												array('data'=> 'Nature of Work', 'field' => '', 'width' => '15%'),
												array('data'=> 'Maximum Number of Contractor Labour', 'field' => '', 'width' => '10%'),
												array('data'=> 'Actions', 'field' => '', 'width' => '10%'),
												array('data'=> 'From V/Ref. No.', 'field' => '', 'width' => '10%')
											);
						
		
		/** Contractor Details ***/
		
		$query = 	db_select('l_contractor_info_master', 'lcim');
		$query->fields('lcim', array());
		$query->condition('lcim.application_id', $application_id);
		$query->condition('lcim.user_id', $user->uid);
		$query->orderBy('lcim.id','DESC');
		$contractors_list = $query->execute()->fetchAll();
		
		$counter 						= 	0;
		foreach($contractors_list as  $data){ 
		
			$counter++;
			
			/*$contractor_wrk_name 		= db_select('l_clra_con_nature_of_work', 'lccnw');
			$contractor_wrk_name		->leftJoin('contractor_works', 'cw', 'cw.contractor_work_id=lccnw.contractor_nature_of_work');
			$contractor_wrk_name		->fields('lccnw', array('contractor_nature_of_work'));
			$contractor_wrk_name		->fields('cw', array('cont_work_name'));
			$contractor_wrk_name		->condition('lccnw.user_id', $user->uid);
			$contractor_wrk_name		->condition('lccnw.contractor_id', $data->id);
			$contractor_wrk_name		->condition('lccnw.application_id', $application_id);
			$result_2 					= $contractor_wrk_name->execute()->fetchAll(); */
			
			$cont_work_name				= '';
			/*$prefix						= '';
		
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
				
			}*/
			$cont_work_name = getConNatureOfWrk($data->id, $application_id,1);
			
			if( trim($status) =='B' || trim($final_submit_status) =='' ){		
			
			$editVal 				= 	array('data'=> 'Edit');
			
			$editValue 				= 	l(t('<img width="16px" height="16px" src="'.$base_root.$base_path.'sites/default/files/edit.png">'),'edit-contractor-form/'.encryption_decryption_fun('encrypt',$data->id).'/'.encryption_decryption_fun('encrypt',$application_id), array('html' => TRUE,'attributes'=>array('title' => 'Click to Edit', 'class' => '')));				
			
			if($data->status == 1){
				$statusView = '<img data-ctype="'.$data->contractor_type.'" data-fv="'.$data->is_from_v.'" data-cln="'.$data->contractor_max_no_of_labours_on_any_day.'" id="acstatusId_'.$data->id.'" class="changestatus" data-st="0" data-id="'.$data->id.'" title="click here to deactive contractor" width="16px" height="16px" src="'.$base_root.$base_path.'sites/default/files/active_icon.png"><img data-ctype="'.$data->contractor_type.'" data-fv="'.$data->is_from_v.'" data-cln="'.$data->contractor_max_no_of_labours_on_any_day.'" id="deacstatusId_'.$data->id.'" style="display:none;" class="changestatus" data-st="1" data-id="'.$data->id.'" title="click here to active contractor" width="16px" height="16px" src="'.$base_root.$base_path.'sites/default/files/deactive_icon.png"><img id="loading_'.$data->id.'" style="display:none;" width="16px" height="16px" src="'.$base_root.$base_path.'sites/default/files/loadinhimage1.gif">';
			}else{
				$statusView = '<img data-ctype="'.$data->contractor_type.'" data-fv="'.$data->is_from_v.'" data-cln="'.$data->contractor_max_no_of_labours_on_any_day.'" id="acstatusId_'.$data->id.'" style="display:none;" class="changestatus" data-st="0" data-id="'.$data->id.'" title="click here to deactive contractor" width="16px" height="16px" src="'.$base_root.$base_path.'sites/default/files/active_icon.png"><img data-ctype="'.$data->contractor_type.'" data-fv="'.$data->is_from_v.'" data-cln="'.$data->contractor_max_no_of_labours_on_any_day.'" id="deacstatusId_'.$data->id.'" class="changestatus" data-st="1" data-id="'.$data->id.'" title="click here to active contractor" width="16px" height="16px" src="'.$base_root.$base_path.'sites/default/files/deactive_icon.png"><img id="loading_'.$data->id.'" style="display:none;" width="16px" height="16px" src="'.$base_root.$base_path.'sites/default/files/loadinhimage1.gif">';
			}
			
		}else{
			
			$editVal 				= 	array('data'=> 'Edit');
			$editValue 				= 	t('<img width="16px" height="16px" src="'.$base_root.$base_path.'sites/all/modules/applicant_forms/images/locked.png">');
		}
		
			if($data->is_from_v == 1 && $data->status == 1){
			if($data->contractor_max_no_of_labours_on_any_day > 9){
				if($data->contractor_type == 2){
					$Download_From_V = l('Download Form V','pdf_from_v_download/'.$data->id,  array('html' => TRUE,'attributes' => array('target' => '_blank')));
				}else{
					$Download_From_V = '00'.$data->id;
				}
			}else{
				$Download_From_V = 'Not Applicable';					
			}
			
		}else{
			if($data->status == 0){
				$Download_From_V = '<strong><font color="#f00">Cancelled</font></strong>';
			}else{
				$Download_From_V = 'Pending';
			}
		}
			
			$rows[] = array( 
					$counter,
					$data->name_of_contractor, 
					$data->address_of_contractor, 
					$cont_work_name,
					$data->contractor_max_no_of_labours_on_any_day, 
					l(t('<img width="16px" height="16px" src="'.$base_root.$base_path.'sites/default/files/view.png">'),'contractor-view/'.encryption_decryption_fun('encrypt', 1).'/'.encryption_decryption_fun('encrypt', $data->id), array('html' => TRUE,'attributes'=>array('title' => 'Click to View'))).'&nbsp;&nbsp;'.$editValue.'&nbsp;&nbsp;'.$statusView, 
					'<span id="formVstatus_'.$data->id.'">'.$Download_From_V.'</span>'				
					);
		}
		
		
		$variables 	= 	array( 'header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('table table table-striped table-responsive dashboard-table')), 'empty' => t("No data found!"));
						
		$output 	= 	theme('datatable', $variables);
		$output .= '<input type="hidden" name="base_url" value="'.$base_url.'">';
		$output1 = '<span id="changestatus_msg"></span>';
		return $output1.$output;
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
			$output 	= ctools_modal_form_wrapper('contractor_view', $form_state);
	
			if (!empty($form_state['ajax_commands'])) {

				$output = $form_state['ajax_commands'];
			}
	
			print ajax_render($output);
			drupal_exit();
		}else {
			return drupal_get_form('contractor_view');
		}
	}
	
	
	
	
	/*** Function For Contractor View Modal Box ****/
	
	function contractor_view($form, &$form_state){
		
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
			$areatype_name 		= $data_add->borough_name;
		}elseif($text_AreaType == "Municipality"){
			$text_AreaType_name	='Ward';
			$areatype_name 		= 'Ward -'.$data_add->village_name;
		}else{
			$text_AreaType_name	='Gram Panchayat';	
			$areatype_name 		= ucwords($data_add->village_name);
		}
		
		$output = '';
		$output .= '<div class="content">
						<div class="sky-form">
							<header><h3><b>Particulars of Contractors and Contract Labour</h3></b></header>
							<fieldset>					
								<div class="row">
									<section class="col col-6">
										<label class="input">1. Name of the Contractor
											<input type="text" value="'.$data->name_of_contractor.'" disabled="disabled">
										</label>
									</section>
									
									<section class="col col-6">
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
									<section class="col col-6">
										<label class="input">3.a.) District of the Contractor
											<input type="text" value="'.$data_add->district_name.'" disabled="disabled">
										</label>
									</section>
									<section class="col col-6">
										<label class="input">3.b.) Subdivision of the Contractor
											<input type="text" value="'.$data_add->sub_div_name.'" disabled="disabled">
										</label>
									</section>
								</div>
								<div class="row">
									<section class="col col-6">
										<label class="input">3.c.) Areatype of the Contractor
											<input type="text" value="'.$text_AreaType.'" disabled="disabled">
										</label>
									</section>
									<section class="col col-6">
										<label class="input">3.d.) '.$text_AreaType.' of the Contractor
											<input type="text" value="'.$data_add->block_mun_name.'" disabled="disabled">
										</label>
									</section>
								</div>
								<div class="row">
									<section class="col col-6">
										<label class="input">3.e.) '.$text_AreaType_name.' of the Contractor
											<input type="text" value="'.$areatype_name.'" disabled="disabled">
										</label>
									</section>
									<section class="col col-6">
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
									<section class="col col-6">
										<label class="input">5. Maximum Number of Contractor Labour to be Employed on any day Through Each Contractor
											<input type="text" value="'.$data->contractor_max_no_of_labours_on_any_day.'" disabled="disabled">
										</label>
									</section>
								
									<section class="col col-6">
										<label class="input">6. Estimated Date of Employment of Each Contract Work Under Each Contractor
											<input type="text" value="'.date('dS M, Y',strtotime($data->est_date_of_work_of_each_labour_to_date)).'&nbsp;to&nbsp;'.date('dS M, Y',strtotime($data->est_date_of_work_of_each_labour_from_date)).'" disabled="disabled"></textarea>
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
	