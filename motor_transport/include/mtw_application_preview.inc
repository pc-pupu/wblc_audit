<?php
//drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
//drupal_add_css(drupal_get_path('module', 'motor_transport') . '/css/popup.css');
drupal_add_js(drupal_get_path('module', 'motor_transport') . '/js/mtw_upload_custom.js');
function mtw_application_preview($form, &$form_state, $id=''){
	global $base_root, $base_path, $user;
	// if($user->uid !=64){
	// 	die('Service Under Maintenance. Sorry for the inconvenience');
	// }
	
	
	
	$user_id					= $user->uid;
	$action						= 'decrypt';
	$action_encrypt				= 'encrypt';
	$application_id				= encryption_decryption_fun($action, $id);
	
	// master table data
	$master_part_mtw_reg 		= db_select('l_mtw_registration_master', 'lmrm');
	$master_part_mtw_reg->fields('lmrm', array('final_submit_status', 'status', 'backlog_id', 'is_renew'));
	$master_part_mtw_reg->condition('lmrm.user_id', $user_id);
	$master_part_mtw_reg->condition('lmrm.id', $application_id); 
	$master_part_mtw_reg_result = $master_part_mtw_reg->execute(); 
	$applicant_details			= $master_part_mtw_reg_result->fetchAssoc();
	$final_submit_status		= trim($applicant_details['final_submit_status']);
	$status						= trim($applicant_details['status']);  
	$backlog_id					= trim($applicant_details['backlog_id']);
	$is_renew					= trim($applicant_details['is_renew']);
	  
	
	// l_mtw_registration_master_extra ownership data
	$ownership = db_select('l_mtw_registration_master_extra', 'lmrme');
	$ownership->fields('lmrme', array());
	$ownership->condition('lmrme.application_id', trim($application_id));
	$ownership_result = $ownership->execute();
	
	$ownership	=	$ownership_result->rowCount();
	
	
	
	$form['mtw_app_final_preview']['application_id'] = array(
		'#title'				=> 'Application Id',
		'#type'					=> 'hidden',
		'#default_value' 			=> !empty($application_id) ? $application_id : '',
		'#attributes' 				=> array('readonly' => 'readonly'),
	);
	$form['mtw_app_final_preview']['backlog_id'] = array(
		'#title'				=> 'Backlog Id',
		'#type'					=> 'hidden',
		'#default_value' 		=> !empty($backlog_id) ? $backlog_id : 0,
		'#attributes' 			=> array('readonly' => 'readonly'),
	);
	
	if($is_renew == 1){
		// l_mtw_registration_renewal data
		$renewal_query = db_select('l_mtw_registration_renewal','rmtw');
		$renewal_query->fields('rmtw',array('id','renewal_ref_number'));
		$renewal_query->condition('rmtw.application_id',$application_id);
		$renewal_query->condition('rmtw.status','1');
		$renewal_result = $renewal_query->execute();
		if($renewal_result->rowCount() > 0){
			$data = $renewal_result->fetchObject();
			$renewal_ref_number = $data->renewal_ref_number;
			$renewal_id = $data->id;
		}
	}else{
		$renewal_ref_number = '';
		$renewal_id = 0;
	}
	$form['mtw_app_final_preview']['renewal_ref_number'] = array(
		'#title'				=> 'Backlog Id',
		'#type'					=> 'hidden',
		'#default_value' 		=> !empty($renewal_ref_number) ? $renewal_ref_number : 0,
		'#attributes' 			=> array('readonly' => 'readonly'),
	);
	$form['mtw_app_final_preview']['renewal_id'] = array(
		'#title'				=> 'Backlog Id',
		'#type'					=> 'hidden',
		'#default_value' 		=> !empty($renewal_id) ? $renewal_id : 0,
		'#attributes' 			=> array('readonly' => 'readonly'),
	);
	
	
	
	if($backlog_id != 0 && $backlog_id !=''){
		// backlog renewal data
		$form['mtw_app_final_preview']['markup_data_1'] = array(
			'#type'	=> 'markup',
			'#markup' => '<div class="panel panel-default panel-form">
							<div class="panel-heading"> Registration Renewal Data </div>
							<div class="panel-body">'.view_mtw_app_final_preview_backlog($application_id).'</div>',
		);
	}else{
		// normal registration
		$form['mtw_app_final_preview']['markup_data_1'] = array(
			'#type'	=> 'markup',
			'#markup' => '<div class="panel panel-default panel-form">
							<div class="panel-heading"> Registration Data </div>
							<div class="panel-body">'.view_mtw_app_final_preview($application_id).'</div>',
		);
	}
	
	$form['mtw_app_final_preview']['markup_data_2'] = array(
			'#type' => 'markup',
			'#markup' => '</div>'
	);
	
	
	if($status == '' || $status == 'B'){
		if($ownership =='' || $ownership==0){
			$form['mtw_app_final_preview']['finalAgree'] = array(
				'#type'						=> 'markup',
				'#markup'					=> l('Add Ownership Details','mtw-registration-details/addmore/'.encryption_decryption_fun($action_encrypt, $application_id), array('html' => TRUE, 'attributes' => array('target' => '', 'class' => array('button')))),
			);
		}else{
			$form['mtw_app_final_preview']['finalAgree'] = array(																		 
					'#prefix' 				=> '<div class=""><label class="">',
					'#suffix' 				=> '</label></div>',
					'#type' 				=> 'checkbox',
					'#description' 			=> '<span class="red-star">*</span>I hereby declare that the particulars given above are true the best of my knowledge and belief.',
					'#attributes'			=> array('id' => 'finalAgreeId','onchange' => 'return checkFinalAppCheckBoxMTW()'),
					'#required' 			=> TRUE																									  
			);
			$form['mtw_app_final_preview']['submit'] = array(
					'#type' 				=> 'submit',
					'#id'					=> 'submitBtnMtw',
					'#value' 				=> 'SUBMIT',
					'#attributes' 			=> array('class' => array('button right'), 'onclick' => 'return finalsubmitConfirm(); ','style'=>'background-color:#ff6666')
			);
		}
	}else{
		$form['mtw_app_final_preview']['download_pdf'] = array(
			'#type'					=> 'markup',
			'#markup' 				=>  l(' VIEW&nbsp;&amp;&nbsp;PRINT','mtw-generate-pdf/'.encryption_decryption_fun($action_encrypt, $application_id).'/'.encryption_decryption_fun($action_encrypt, 'self'), array('html' => TRUE, 'attributes' => array('target' => '_blank', 'class' => array('btn btn-primary'))))
		);
	}
	return $form;						
}

function view_mtw_app_final_preview_backlog($application_id=''){ 
	
		global $base_root, $base_path, $user;
		
		$user_id = $user->uid;
				
		$master_part_mtw_reg = db_select('l_mtw_registration_master', 'lmrm');
		$master_part_mtw_reg->fields('lmrm', array());
		
		/////////////////////////  MTW Address Fetch ////////////////////////////////
			
		$master_part_mtw_reg->fields('dm_loc', array('district_name'));
		$master_part_mtw_reg->leftJoin('district_master', 'dm_loc', 'dm_loc.district_code=lmrm.mtw_loc_dist');
		
		$master_part_mtw_reg->fields('sd_loc', array('sub_div_name'));
		$master_part_mtw_reg->leftJoin('sub_division', 'sd_loc', 'sd_loc.sub_div_code=lmrm.mtw_loc_subdivision');
		
		$master_part_mtw_reg->fields('bmm_loc', array('block_mun_name'));
		$master_part_mtw_reg->leftJoin('block_mun_master', 'bmm_loc', 'bmm_loc.block_code=lmrm.mtw_loc_areatype_code');
		
		$master_part_mtw_reg->fields('vwm_loc', array('village_name'));
		$master_part_mtw_reg->leftJoin('village_ward_master', 'vwm_loc', 'vwm_loc.village_code=lmrm.mtw_loc_vill_ward');
		
		$master_part_mtw_reg->fields('ps_loc', array('name_of_police_station'));
		$master_part_mtw_reg->leftJoin('police_station', 'ps_loc', 'ps_loc.police_station_code=lmrm.mtw_loc_ps');
		
		//////////////////////////////  End 1 Location of Undertaken MTW /////////////////////////////////////
		
		$master_part_mtw_reg->condition('lmrm.user_id', $user_id);
		$master_part_mtw_reg->condition('lmrm.id', $application_id);
		
		$master_part_mtw_reg_result = $master_part_mtw_reg->execute();  
		$content	=	$master_part_mtw_reg_result->fetchAssoc();	
		
		$fees = $content['mtw_maxworkers'];
		
		if($fees <= 5){
			$retFees	=	'10';
		}else if($fees > 5 && $fees <= 25  ){
			$retFees	=	'25';
		}else if($fees > 25 && $fees <= 50  ){
			$retFees	=	'50';
		}else if($fees > 50 && $fees <= 100  ){
			$retFees	=	'100';
		}else if($fees > 100 && $fees <= 250  ){
			$retFees	=	'250';
		}else if($fees >250 && $fees <= 500){
			$retFees	=	'500';
		}else if($fees > 500 && $fees <= 750  ){
			$retFees	=	'750';
		}else if($fees > 750){
			$retFees	=	'1000';
		}else{
			$retFees	= '0';
		}
				
		$master_part_mtw_docs = db_select('l_documents_upload', 'ldu');
		$master_part_mtw_docs->fields('ldu', array());
		$master_part_mtw_docs->condition('ldu.user_id', $user_id);
		$master_part_mtw_docs->condition('ldu.act_id', '3');
		$master_part_mtw_docs->condition('ldu.application_id', trim($application_id));
		$master_part_mtw_docs_result = $master_part_mtw_docs->execute();
		
		$content_docs	=	$master_part_mtw_docs_result->fetchAssoc();
		
		
		if($content['mtw_loc_areatype']=="B"){
			$text_AreaType_loc = "Block";
		}elseif($content['mtw_loc_areatype']=="M"){
			$text_AreaType_loc = "Municipality";
		}elseif($content['mtw_loc_areatype']=="C"){
			$text_AreaType_loc = "Corporation";
		}elseif($content['mtw_loc_areatype']=="S"){
			$text_AreaType_loc = "SEZ";
		}elseif($content['mtw_loc_areatype']=="N"){
			$text_AreaType_loc = "Notified Area";
		}elseif($content['mtw_loc_areatype']=="O"){
			$text_AreaType_loc = "Other";
		}
		
		if($text_AreaType_loc =='Corporation') {
			$text_AreaType_loc_name='Ward';
			$areatype_loc_name = $content['village_name'];
		}elseif($text_AreaType_loc=='Municipality'){
			$text_AreaType_loc_name='Ward';
			$areatype_loc_name = 'Ward-'.$content['village_name']; 
		}else {
			$text_AreaType_loc_name='Gram Panchayat';	
			$areatype_loc_name = ucwords($content['village_name']);
		}	
		
		
		$personqyery	= db_select('l_mtw_registration_master_extra','lmrme');
		$personqyery->fields('lmrme',array());
		
		
		////////////// Person State Part //////////
		$personqyery->fields('pre_state', array('statename'));
		$personqyery->leftJoin('state_master','pre_state','pre_state.id=lmrme.person_state');
		
		
		////////////// Person Distrct Part //////////
		$personqyery->fields('pre_dist', array('district_name'));
		$personqyery->leftJoin('district_master','pre_dist','pre_dist.district_code=lmrme.person_district');
		
		////////////// Person Sub-Divistion Part //////////
		$personqyery->fields('per_subdiv', array('sub_div_name'));
		$personqyery->leftJoin('sub_division', 'per_subdiv', 'per_subdiv.sub_div_code=lmrme.person_subdivision');
		
		////////////// Person Block Part //////////
		$personqyery->fields('per_block', array('block_mun_name'));
		$personqyery->leftJoin('block_mun_master', 'per_block', 'per_block.block_code=lmrme.person_areatype_code');
		
		////////////// Person Vill Ward Part //////////
		$personqyery->fields('pre_vilwar', array('village_name'));
		$personqyery->leftJoin('village_ward_master', 'pre_vilwar', 'pre_vilwar.village_code=lmrme.person_vill_ward');
		
		////////////// Person Police Station Part //////////
		$personqyery->fields('person_ps', array('name_of_police_station'));
		$personqyery->leftJoin('police_station', 'person_ps', 'person_ps.police_station_code=lmrme.person_ps');
		
		
		$personqyery->condition('lmrme.application_id', trim($application_id));
		
		$personqyery_result = $personqyery->execute();  
		$percontent	=	$personqyery_result->fetchAll();
		
		$designation		= '';
		$name				= '';
		$person_address		= '';
		$person_district	= '';
		$person_sub_div_name		= '';
		$person_block_mun_name		= '';
		$person_village_name		= '';
		$person_ps			= '';
		$pincode			=	'';
		$i					= 0;
		
		
		$persondata	= array();
		
		if($personqyery_result->rowCount()>0){
			
			foreach($percontent as $ob){
				
				$designation				= $ob->designation;
				$name						= $ob->person_name;
				$person_address				= $ob->person_address;
				$person_district			= $ob->district_name;
				$preson_sub_div_name		= $ob->sub_div_name;
				$person_areatype			= $ob->person_areatype;
				$person_block_mun_name		= $ob->block_mun_name;
				$preson_village_name		= $ob->village_name;
				$person_ps					= $ob->name_of_police_station;
				$pincode					= $ob->person_pincode;
				$country					= $ob->person_country;
				$state						= $ob->statename;
				$state_code					= $ob->person_state;
				
				if($person_areatype=="B"){
					$person_areatype_name	= 'Block';
					$person_areatype_name_value	= 'Gram Panchayat';
				}elseif($person_areatype=="C"){
					$person_areatype_name	= 'Corporation';
					$person_areatype_name_value	= 'Ward';
				}elseif($person_areatype=="M"){
					$person_areatype_name	= 'Municipality';
					$person_areatype_name_value	= 'Ward';
				}elseif($person_areatype=="S"){
					$person_areatype_name	= 'SEZ';
					$person_areatype_name_value	= '';
				}elseif($person_areatype=="N"){
					$person_areatype_name	= 'Notified Area';
					$person_areatype_name_value	= '';
				}elseif($person_areatype=="O"){
					$person_areatype_name	= 'Other';
					$person_areatype_name_value	= '';
				}else{
					$person_areatype_name	= '';
					$person_areatype_name_value	= '';
				}
				
				$options_country 	= array(''=>'-Select-', 1 => 'India', 2 => 'Others'); 
				if($country=='1'){
					$pr_country = 'India';						
				}elseif($country=='2'){
					$pr_country = 'Others';	
				}
				
				if(($designation=="proprietor") || ($designation=="partner")){
					$label1	= ucwords($designation).' if firm not registered under Co Act,1956';
					$slno	= '8.1';
					$slnoAdd= '8.1';
					
					$pp[]	= '
						<tr>
							<td style="text-align:center;">'.$slno.'</td>
							<td>Name of the '.$label1.' </td>
							<td>'.$name.'</td>
						</tr>
					  	<tr>
							<td style="text-align:center;">'.$slno.'.1</td>
							<td>Location of '.ucwords($designation).'</td>
							<td>'.$person_address.'</td>
						</tr>
						<tr>
							<td style="text-align:center;">'.$slnoAdd.'.2</td>
							<td>Country</td>
							<td>'.$pr_country.'</td>
						</tr>
						<tr>
							<td style="text-align:center;">'.$slnoAdd.'.3</td>
							<td>State</td>
							<td>'.$state.'</td>
						</tr>';
						
					if($state_code !=0 || !empty($state_code)){
					
						$pp[]	= '<tr>
										<td style="text-align:center;">'.$slnoAdd.'.3</td>
										<td>State</td>
										<td>'.$state.'</td>
									</tr>';
						
								if(!empty($person_district) || $person_district!=''){
						$pp[]	= '
									<tr>
										<td style="text-align:center;">'.$slno.'.4</td>
										<td>District</td>
										<td>'.$person_district.'</td>
									</tr>';
								}	
								if(!empty($preson_sub_div_name) || $preson_sub_div_name!=''){
						$pp[]	=	'<tr>
										<td style="text-align:center;">'.$slno.'.5</td>
										<td>Subdivision</td>
										<td>'.$preson_sub_div_name.'</td>
									</tr>';		
								}
									
								if(!empty($person_areatype_name) || $person_areatype_name!=''){
						$pp[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.7</td>
										<td>'.$person_areatype_name.'</td>
										<td>'.$person_block_mun_name.'</td>
									</tr>';
									
						$pp[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.8</td>
										<td>'.$person_areatype_name_value.'</td>
										<td>'.$preson_village_name.'</td>
									</tr>';		
								}
									
								if(!empty($person_ps) || $person_ps!=''){
						$pp[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.9</td>
										<td>Police Station</td>
										<td>'.$person_ps.'</td>
									</tr>';		
								}
									
								if(!empty($pincode) || $pincode!=''){
						$pp[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.10</td>
										<td>Pin Code</td>
										<td>'.$pincode.'</td>
									</tr>';
								}
					}
				}elseif($designation=="gm"){
					$label1	= 'General Manager in case of a public sector undertaking';
					$slno	= '8.2';
					$slnoAdd= '8.2';
					
					$gm[]	= '
						<tr>
							<td style="text-align:center;">'.$slno.'</td>
							<td>Name of the General Manager </td>
							<td>'.$name.'</td>
						</tr>
					  	<tr>
							<td style="text-align:center;">'.$slno.'.1</td>
							<td>Location of General Manager</td>
							<td>'.$person_address.'</td>
						</tr>
						<tr>
							<td style="text-align:center;">'.$slnoAdd.'.2</td>
							<td>Country</td>
							<td>'.$pr_country.'</td>
						</tr>
						<tr>
							<td style="text-align:center;">'.$slnoAdd.'.3</td>
							<td>State</td>
							<td>'.$state.'</td>
						</tr>';
						
						
					if($state_code !=0 || !empty($state_code)){	
							
						$gm[]	= '<tr>
										<td style="text-align:center;">'.$slnoAdd.'.3</td>
										<td>State</td>
										<td>'.$state.'</td>
									</tr>';
						
								if(!empty($person_district) && $person_district!=''){
						$gm[]	= '
									<tr>
										<td style="text-align:center;">'.$slno.'.4</td>
										<td>District</td>
										<td>'.$person_district.'</td>
									</tr>';
								}	
								if(!empty($preson_sub_div_name) && $preson_sub_div_name!=''){
						$gm[]	=	'<tr>
										<td style="text-align:center;">'.$slno.'.5</td>
										<td>Subdivision</td>
										<td>'.$preson_sub_div_name.'</td>
									</tr>';		
								}
									
								if(!empty($person_areatype_name) && $person_areatype_name!=''){
						$gm[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.7</td>
										<td>'.$person_areatype_name.'</td>
										<td>'.$person_block_mun_name.'</td>
									</tr>';
									
						$gm[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.8</td>
										<td>'.$person_areatype_name_value.'</td>
										<td>'.$preson_village_name.'</td>
									</tr>';		
								}
									
								if(!empty($person_ps) && $person_ps!=''){
						$gm[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.9</td>
										<td>Police Station</td>
										<td>'.$person_ps.'</td>
									</tr>';		
								}
									
								if(!empty($pincode) && $pincode!=''){
						$gm[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.10</td>
										<td>Pin Code</td>
										<td>'.$pincode.'</td>
									</tr>';
								}
					}
				}elseif($designation=="director"){
					$label1	= 'Director in case of company registered under Co Act, 1956';
					$slno	= '9.1';
					$slnoAdd= '9.1';
					
					$dire[]	= '
					<tr>
							<td style="text-align:center;">'.$slno.'</td>
							<td>Name of the Director </td>
							<td>'.$name.'</td>
						</tr>
					  	<tr>
							<td style="text-align:center;">'.$slno.'.1</td>
							<td>Location of Director</td>
							<td>'.$person_address.'</td>
						</tr>
						<tr>
							<td style="text-align:center;">'.$slnoAdd.'.2</td>
							<td>Country</td>
							<td>'.$pr_country.'</td>
						</tr>';
						
						
							if($state_code !=0 || !empty($state_code)){		
					$dire[]	= '<tr>
									<td style="text-align:center;">'.$slnoAdd.'.3</td>
									<td>State</td>
									<td>'.$state.'</td>
								</tr>';
					
							if(!empty($person_district) && $person_district!=''){
					$dire[]	= '
								<tr>
									<td style="text-align:center;">'.$slno.'.4</td>
									<td>District</td>
									<td>'.$person_district.'</td>
								</tr>';
							}	
							if(!empty($preson_sub_div_name) && $preson_sub_div_name!=''){
					$dire[]	=	'<tr>
									<td style="text-align:center;">'.$slno.'.5</td>
									<td>Subdivision</td>
									<td>'.$preson_sub_div_name.'</td>
								</tr>';		
							}
								
							if(!empty($person_areatype_name) || $person_areatype_name!=''){
					$dire[]	=	'<tr>
									<td style="text-align:center;">'.$slnoAdd.'.7</td>
									<td>'.$person_areatype_name.'</td>
									<td>'.$person_block_mun_name.'</td>
								</tr>';
								
					$dire[]	=	'<tr>
									<td style="text-align:center;">'.$slnoAdd.'.8</td>
									<td>'.$person_areatype_name_value.'</td>
									<td>'.$preson_village_name.'</td>
								</tr>';		
							}
								
							if(!empty($person_ps) || $person_ps!=''){
					$dire[]	=	'<tr>
									<td style="text-align:center;">'.$slnoAdd.'.9</td>
									<td>Police Station</td>
									<td>'.$person_ps.'</td>
								</tr>';		
							}
								
							if(!empty($pincode) || $pincode!=''){
					$dire[]	=	'<tr>
									<td style="text-align:center;">'.$slnoAdd.'.10</td>
									<td>Pin Code</td>
									<td>'.$pincode.'</td>
								</tr>';
							}		
				}
						
			
				}else{
					
					$label1	= 'Director in case of company registered under Co Act, 1956';
					$slno	= '8.3';
					$slnoAdd= '8.3';
					
					$othr[]	= '
					<tr>
							<td style="text-align:center;">'.$slno.'</td>
							<td>Name of the '.ucwords($designation).' </td>
							<td>'.$name.'</td>
						</tr>
					  	<tr>
							<td style="text-align:center;">'.$slno.'.1</td>
							<td>Location of '.ucwords($designation).'</td>
							<td>'.$person_address.'</td>
						</tr>
						<tr>
							<td style="text-align:center;">'.$slnoAdd.'.2</td>
							<td>Country</td>
							<td>'.$pr_country.'</td>
						</tr>';
						
						
								if($state_code !=0 || !empty($state_code)){		
						$othr[]	= '<tr>
										<td style="text-align:center;">'.$slnoAdd.'.3</td>
										<td>State</td>
										<td>'.$state.'</td>
									</tr>';
						
								if(!empty($person_district) && $person_district!=''){
						$othr[]	= '
									<tr>
										<td style="text-align:center;">'.$slno.'.4</td>
										<td>District</td>
										<td>'.$person_district.'</td>
									</tr>';
								}	
								if(!empty($preson_sub_div_name) && $preson_sub_div_name!=''){
						$othr[]	=	'<tr>
										<td style="text-align:center;">'.$slno.'.5</td>
										<td>Subdivision</td>
										<td>'.$preson_sub_div_name.'</td>
									</tr>';		
								}
									
								if(!empty($person_areatype_name) || $person_areatype_name!=''){
						$othr[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.6</td>
										<td>'.$person_areatype_name.'</td>
										<td>'.$person_block_mun_name.'</td>
									</tr>';
									
						$othr[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.7</td>
										<td>'.$person_areatype_name_value.'</td>
										<td>'.$preson_village_name.'</td>
									</tr>';		
								}
									
								if(!empty($person_ps) || $person_ps!=''){
						$othr[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.8</td>
										<td>Police Station</td>
										<td>'.$person_ps.'</td>
									</tr>';		
								}
									
								if(!empty($pincode) || $pincode!=''){
						$othr[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.9</td>
										<td>Pin Code</td>
										<td>'.$pincode.'</td>
									</tr>';
								}		
							}
				}
			}
		}
		
	$enc_doc_array = array('TL'=> '','PD'=> '','AOA'=> '','MOA'=> '','FL'=> '','AC'=> '','PC'=> '','BB'	=> '','SC'	=> '','IC'	=> '','AP'=> '','ODSC'=> '','CR'=> '','WO'=> '','ORC'=> '');
	
	$pdficonlink = '<img title="View Documents" alt="View Documents" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/pdf.png"> &nbsp;View';
	
	// data from encrypted data table
	$query 			= db_select('l_encrypted_uploaded_documents','leud')->fields('leud',array())->condition('status','1')->condition('user_id',$user_id);
	$query_result 	= $query->execute();
	
	if($query_result->rowCount() > 0){
		foreach($query_result->fetchAll() as $data ){
			$enc_doc_array[$data->document_type_code]['content'] = 'view_documents/'.encryption_decryption_fun('encrypt',$data->id);
		}
	}
		
	
	// l_documents upload data
	$master_part_mtw_docs = db_select('l_documents_upload', 'ldu');
	$master_part_mtw_docs->fields('ldu', array());
	$master_part_mtw_docs->condition('ldu.user_id', $user_id);
	$master_part_mtw_docs->condition('ldu.act_id', '3');
	$master_part_mtw_docs->condition('ldu.application_id', trim($application_id));
	$master_part_mtw_docs_result = $master_part_mtw_docs->execute();
	
	$content_docs_no = $master_part_mtw_docs_result->rowCount();
	if($content_docs_no > 0){
		$obj3    = $master_part_mtw_docs_result->fetchObject();
		
		if($enc_doc_array['TL'] == ''){
			$enc_doc_array['TL']['content'] = ($obj3->trade_license_file != '')? $global_link.'trade_license/'.$obj3->trade_license_file : '';
		}
		if($enc_doc_array['AOA'] == ''){
			$enc_doc_array['AOA']['content'] = ($obj3->article_of_assoc_file != '')? $global_link.'article_of_assoc/'.$obj3->article_of_assoc_file : '';
		}
		if($enc_doc_array['PD'] == ''){
			$enc_doc_array['PD']['content'] = ($obj3->article_of_assoc_file != '')? $global_link.'article_of_assoc/'.$obj3->article_of_assoc_file : '';
		}
		if($enc_doc_array['BB'] == ''){
			$enc_doc_array['BB']['content'] = ($obj3->form_one_asses_ses_file != '')? $global_link.'blue_book/'.$obj3->form_one_asses_ses_file : '';
		}
		if($enc_doc_array['SC'] == ''){
			$enc_doc_array['SC']['content'] = ($obj3->form_one_asses_ses_file != '')? $global_link.'blue_book/'.$obj3->form_one_asses_ses_file : '';
		}
		
		if($enc_doc_array['IC'] == ''){
			$enc_doc_array['IC']['content'] = ($obj3->supp_asses_ses_file != '')? $global_link.'insurance/'.$obj3->supp_asses_ses_file : '';
		}
		if($enc_doc_array['AP'] == ''){
			$enc_doc_array['AP']['content'] = ($obj3->address_proof_file != '')? $global_link.'address_proof/'.$obj3->address_proof_file : '';
		}
		if($enc_doc_array['ODSC'] == ''){
			$enc_doc_array['ODSC']['content'] = ($obj3->other_doc_file != '')? $global_link.'OtherRelatedDocuments/'.$obj3->other_doc_file : '';
		}
		
	}else{
		$obj3    = array();
		$enc_doc_array['TL']['content'] = '';
		$enc_doc_array['AOA']['content'] = '';
		$enc_doc_array['PD']['content'] = '';
		$enc_doc_array['BB']['content'] = '';
		$enc_doc_array['SC']['content'] = '';
		$enc_doc_array['IC']['content'] = '';
		$enc_doc_array['AP']['content'] = '';
		$enc_doc_array['ODSC']['content'] ='';
	}
	
	$global_link = $base_root.$base_path.'sites/default/files/upload/';
	
	
	
	//echo '<pre>'; print_r($enc_doc_array); die;
		
		
		
		$output ='';			
		$output .= '<table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-striped table-responsive dashboard-table sub-table-dashboard">
					  <thead>
						  <tr>
							<th width="6%">Sl. No.</th>
							<th width="40%">Parameters</th>
							<th>Inputs</th>
						  </tr>
					  </thead>
					  <tbody>
					  <tr>
					 	<td style="text-align:center;">1</td>
						<td>Name of Motor Trasport Undertaking</td>
						<td>'.$content['mtw_name'].'</td>
					  </tr>
					  <tr>
					  	<td style="text-align:center;">2</td>
						<td>Location of Motor Trasport Undertaking </td>
						<td>'.$content['mtw_loc_address'].'</td>
					  </tr>
					  <tr>
					  	<td style="text-align:center;">2.1</td>
						<td>District</td>
						<td>'.$content['district_name'].'</td>
					  </tr>
					  <tr>
					  	<td style="text-align:center;">2.2</td>
						<td>Subdivision</td>
						<td>'.$content['sub_div_name'].'<span class="red-star"> '.l(t('(Your Application will be verified by '.$content['sub_div_name'].' RLO office)'),'/contactinfo', array('attributes' => array('class' => 'red-star'))).'</span></td>
					  </tr>
					  <tr>
					  	<td style="text-align:center;">2.3</td>
						<td>'.$text_AreaType_loc.' </td>
						<td>'.$content['block_mun_name'].'</td>
					  </tr>
					  <tr>
					  	<td style="text-align:center;">2.4</td>
						<td>'.$text_AreaType_loc_name.' </td>
						<td>'.$areatype_loc_name.'</td>
					  </tr>
					  <tr>
					  	<td style="text-align:center;">2.5</td>
						<td>Police Station </td>
						<td>'.$content['name_of_police_station'].'</td>
					  </tr>
					   <tr>
					   	<td style="text-align:center;">2.6</td>
						<td>Pin Code </td>
						<td>'.$content['mtw_loc_pincode'].'</td>
					  </tr>
					  <!-------------------------------- MTW Address part end ------------------------------------->
					  <tr>
					  	<td style="text-align:center;">3</td>
						<td>Nature of Motor Trasport Undertaking</td>';
						
						
						if($content['mtw_nature']=='city_service'){
							$mtw_nature_contrnt		=	'City Service';
						}elseif($content['mtw_nature']=='long_distance'){
							$mtw_nature_contrnt		=	'Long Distance';
						}elseif($content['mtw_nature']=='passenger_service'){
							$mtw_nature_contrnt		=	'Passenger Service';
						}elseif($content['mtw_nature']=='long_distance_freight_service'){
							$mtw_nature_contrnt		=	'Long Distance Freight Service';
						}elseif(!empty($content['mtw_nature']) && $content['mtw_nature']!='city_service' && $content['mtw_nature']!='long_distance' && $content['mtw_nature']!='passenger_service' && $content['mtw_nature']!='long_distance_freight_service'){
							$mtw_nature_contrnt		=	$content['mtw_nature'];
						}
						
			$output .='<td>'.$mtw_nature_contrnt.'</td>
					  </tr>
					  
					  <tr>
					  	<td style="text-align:center;">4</td>
						<td>Total Number Of Routes</td>';
						if($content['total_routes'] != ''){
							// details of the route data
							
							$detailsroute = $content['mtw_route_details'];
							
						}else{
							$detailsroute = '';
						}
												
			$output .='<td>'.$content['total_routes'].'</td>
					  </tr>
					  
					  <tr>
					  	<td style="text-align:center;">5</td>
						<td>Route Details - Mileage</td>';
						$output2 = '';
						if($content['total_routes'] != ''){
							$total_milage_add 	= 0;
							$total_no		  	= 1;
							$routeDetails1	=	db_select('l_mtw_registration_extended','mtwe');
							$routeDetails1->fields('mtwe',array());
							$routeDetails1->condition('mtwe.application_id',$application_id);
							$fetdata = $routeDetails1->execute();
							
							
							$allDetails 	=	$fetdata->fetchAll();
							if(!empty($allDetails)){
								foreach($allDetails as $datum1){
									$total_no++;
									$total_milage_add =  $total_milage_add+$datum1->milage;
								}
							}
							$output2.= $total_no.'-'.$total_milage_add;
						}else{
							$output2.= '';
						}
			$output .=	'<td>'.$output2.'</td>
					  </tr>
					  <tr>
					  	<td style="text-align:center;">6</td>
						<td>Total Number of MT Vehicles on the last date or the preceeding year</td>
						<td>'.$content['total_mtw_vehicle'].'</td>
					  </tr>
					  <tr>
					  	<td style="text-align:center;">7 <span class="red-star">*</span></td>
						<td>Maximum Number of MTW Employed on any day during the preceeding year</td>
						<td><b><i>'.$content['mtw_maxworkers'].'</i></b></td>
					  </tr>
					  
					  ';
			
				if(empty($pp)){
					$output .= '<tr><td style="text-align:center;">8.1</td><td>Name and Address of the Proprietor or Partners of Motor Transport undetaking in the case of firm not registered under the companies Act, 1956 </td><td class="red-star">No data available</td></tr>';
				}else{
					if(isset($pp[0])){ $output .= $pp[0];}
					if(isset($pp[1])){ $output .= $pp[1];}
					if(isset($pp[2])){ $output .= $pp[2];}
					if(isset($pp[3])){ $output .= $pp[3];}
					if(isset($pp[4])){ $output .= $pp[4];}
					if(isset($pp[5])){ $output .= $pp[5];}
					if(isset($pp[6])){ $output .= $pp[6];}
					if(isset($pp[7])){ $output .= $pp[7];}
					if(isset($pp[8])){ $output .= $pp[8];}
					
				}
				if(empty($gm)){
					$output .= '<tr><td style="text-align:center;">8.2</td><td>Name and Address of the General Manager in case of a public sector undertaking </td><td class="red-star">No data available</td></tr>';
				}else{
					if(isset($gm[0])){ $output .= $gm[0];}
					if(isset($gm[1])){ $output .= $gm[1];}
					if(isset($gm[2])){ $output .= $gm[2];}
					if(isset($gm[3])){ $output .= $gm[3];}
					if(isset($gm[4])){ $output .= $gm[4];}
					if(isset($gm[5])){ $output .= $gm[5];}
					if(isset($gm[6])){ $output .= $gm[6];}
					if(isset($gm[7])){ $output .= $gm[7];}
					if(isset($gm[8])){ $output .= $gm[8];}
					
				}
				if(!empty($othr)){
					if(isset($othr[0])){ $output .= $othr[0];}
					if(isset($othr[1])){ $output .= $othr[1];}
					if(isset($othr[2])){ $output .= $othr[2];}
					if(isset($othr[3])){ $output .= $othr[3];}
					if(isset($othr[4])){ $output .= $othr[4];}
					if(isset($othr[5])){ $output .= $othr[5];}
					if(isset($othr[6])){ $output .= $othr[6];}
					if(isset($othr[7])){ $output .= $othr[7];}
					if(isset($othr[8])){ $output .= $othr[8];}
				}
				if(empty($dire)){
					$output .= '<tr><td style="text-align:center;">9</td><td>Name and Address of the Director in case of company registered under Co Act, 1956 </td><td class="red-star">No data available</td></tr>';			
				}else{
					if(isset($dire[0])){ $output .= $dire[0];}
					if(isset($dire[1])){ $output .= $dire[1];}
					if(isset($dire[2])){ $output .= $dire[2];}
					if(isset($dire[3])){ $output .= $dire[3];}
					if(isset($dire[4])){ $output .= $dire[4];}
					if(isset($dire[5])){ $output .= $dire[5];}
					if(isset($dire[6])){ $output .= $dire[6];}
					if(isset($dire[7])){ $output .= $dire[7];}
					if(isset($dire[8])){ $output .= $dire[8];}
					
				}
			
			$output .='<tr>
							<td style="text-align:center;">10</td>
							<td>Fees (<span class="red-star">*</span>This is system generated fees depends on point 7)</td>
							<td> &#8377;'.number_format($retFees, 2).'<input type="hidden" name="retFees" id="retFees" value="'.$retFees.'" />&nbsp;&nbsp;&nbsp;
							<span class="viewfile_popup1 blink" id="fees_chart_pdf" title="CLICK HERE TO VIEW FEES DETAILS">FEES CHART [ In <i class="fa fa-inr" aria-hidden="true"></i> ]</span>
							<div id="view_fees_chart_pdf" title="FEES CHART" style="display:none;">
								<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/fees_chat/MTW-FEES-CHART.pdf" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
							</div>
							</td>';
								
					//$output .='<td>Fees will be calculated after ALC verification</td>';
								 	
			$output .='</tr>';	
			$output .='</tbody></table>';	  			
					  
					 //if(!empty($content_docs)){ 
			
			$output1 =   '<table class="table table-striped table-responsive dashboard-table sub-table-dashboard" width="100%" cellspacing="0" cellpadding="0" border="0">
						<thead>
							<tr><td colspan="3" align="center">Documents Uploaded</td></tr></thead>
						<tbody>';
			
			$output1 .='<tr>
							<td style="text-align:center;">1</td>
							<td>Uploaded Trade License</td>';
								$viewlink1 = 'Document not available';
								if($enc_doc_array['TL']['content'] !=''){
									$viewlink1 = l($pdficonlink,$enc_doc_array['TL']['content'],array('html' => TRUE));
								}
			$output1 .='	 <td>'.$viewlink1.'</td>';
			$output1 .='</tr>
						<tr>
							<td style="text-align:center;">2</td>
							<td>Uploaded Articles of Association / Partnership Deed</td>';
							 	$viewlink2 = 'Document not available';
								if($enc_doc_array['AOA']['content'] !=''){
									$viewlink2 = l($pdficonlink,$enc_doc_array['AOA']['content'],array('html' => TRUE));
								}else if($enc_doc_array['PD']['content'] !=''){
									$viewlink2 = l($pdficonlink,$enc_doc_array['PD']['content'],array('html' => TRUE));
								}
			$output1 .=' 	<td>'.$viewlink2.'</td>';
			$output1 .='</tr>
						<tr>
						  <td style="text-align:center;">3</td>
						  <td>Uploaded Blue Book/ Smart Card Issued by Motor Vehicles</td>';
							$viewlink3 = 'Document not available';	
							if($enc_doc_array['BB']['content'] !=''){
								$viewlink3 = l($pdficonlink,$enc_doc_array['BB']['content'],array('html' => TRUE));
							}else if($enc_doc_array['SC']['content'] !=''){
								$viewlink3 = l($pdficonlink,$enc_doc_array['SC']['content'],array('html' => TRUE));
							}
			$output1 .=' 	<td>'.$viewlink3.'</td>';		
			$output1 .='</tr>
						<tr>
						  	<td style="text-align:center;">4</td>
						  	<td>Uploaded Insurance Certificate of Motor Vehicles</td>';
							$viewlink4 = 'Document not available';	
							if($enc_doc_array['IC']['content'] !=''){
							$viewlink4 = l($pdficonlink,$enc_doc_array['IC']['content'],array('html' => TRUE));
							}
			$output1 .=' <td>'.$viewlink4.'</td>';			
					
			$output1 .='</tr>
						<tr>
							<td style="text-align:center;">5</td>
							<td>Uploaded Documents in support of correctness of the application</td>';
								$viewlink5 = 'Document not available';	
								if($enc_doc_array['ODSC']['content'] !=''){
									$viewlink5 = l($pdficonlink,$enc_doc_array['ODSC']['content'],array('html' => TRUE));
								}
			$output1 .='	<td>'.$viewlink5.'</td>';
			$output1 .= '</tr>
						<tr>
							<td style="text-align:center;">6</td>
							<td>Uploaded Address Proof</td>';
							$viewlink6 = 'Document not available';	
							if($enc_doc_array['AP']['content'] !=''){
								$viewlink6 = l($pdficonlink,$enc_doc_array['AP']['content'],array('html' => TRUE));
							}
			$output1 .=' <td>'.$viewlink6.'</td>';
			$output1 .='</tr>
						<tr>
							<td style="text-align:center;">7</td>
							<td>Form -I </td>';
							if(!empty($content_docs['form_1_mtw_signed_pdf_file'])){
								$output1 .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/MTW_Form-I/'. $content_docs['form_1_mtw_signed_pdf_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png" title="Click to view and print FORM-I"></a></td>';
							} else { 
								$output1 .='<td>Form-I should be uploaded by the Applicant after successful payment of fees</td>';
							} 
			$output1 .='</tr>';
    				// }
	$output1 .='</tbody></table>';
					  
	return $output.$output1;
}


function view_mtw_app_final_preview($application_id=''){ 
	
		global $base_root, $base_path, $user;
		
		$user_id = $user->uid;
				
		$master_part_mtw_reg = db_select('l_mtw_registration_master', 'lmrm');
		$master_part_mtw_reg->fields('lmrm', array());
		
		/////////////////////////  MTW Address Fetch ////////////////////////////////
			
		$master_part_mtw_reg->fields('dm_loc', array('district_name'));
		$master_part_mtw_reg->leftJoin('district_master', 'dm_loc', 'dm_loc.district_code=lmrm.mtw_loc_dist');
		
		$master_part_mtw_reg->fields('sd_loc', array('sub_div_name'));
		$master_part_mtw_reg->leftJoin('sub_division', 'sd_loc', 'sd_loc.sub_div_code=lmrm.mtw_loc_subdivision');
		
		$master_part_mtw_reg->fields('bmm_loc', array('block_mun_name'));
		$master_part_mtw_reg->leftJoin('block_mun_master', 'bmm_loc', 'bmm_loc.block_code=lmrm.mtw_loc_areatype_code');
		
		$master_part_mtw_reg->fields('vwm_loc', array('village_name'));
		$master_part_mtw_reg->leftJoin('village_ward_master', 'vwm_loc', 'vwm_loc.village_code=lmrm.mtw_loc_vill_ward');
		
		$master_part_mtw_reg->fields('ps_loc', array('name_of_police_station'));
		$master_part_mtw_reg->leftJoin('police_station', 'ps_loc', 'ps_loc.police_station_code=lmrm.mtw_loc_ps');
		
		//////////////////////////////  End 1 Location of Undertaken MTW /////////////////////////////////////
		
		$master_part_mtw_reg->condition('lmrm.user_id', $user_id);
		$master_part_mtw_reg->condition('lmrm.id', $application_id);
		
		
		$master_part_mtw_reg_result = $master_part_mtw_reg->execute();  
		$content	=	$master_part_mtw_reg_result->fetchAssoc();	
		
		
		$fees = $content['mtw_maxworkers'];
		
		if($fees <= 5){
			$retFees	=	'10';
		}else if($fees > 5 && $fees <= 25  ){
			$retFees	=	'25';
		}else if($fees > 25 && $fees <= 50  ){
			$retFees	=	'50';
		}else if($fees > 50 && $fees <= 100  ){
			$retFees	=	'100';
		}else if($fees > 100 && $fees <= 250  ){
			$retFees	=	'250';
		}else if($fees >250 && $fees <= 500){
			$retFees	=	'500';
		}else if($fees > 500 && $fees <= 750  ){
			$retFees	=	'750';
		}else if($fees > 750){
			$retFees	=	'1000';
		}
				
		$master_part_mtw_docs = db_select('l_documents_upload', 'ldu');
		$master_part_mtw_docs->fields('ldu', array());
		$master_part_mtw_docs->condition('ldu.user_id', $user_id);
		$master_part_mtw_docs->condition('ldu.act_id', '3');
		$master_part_mtw_docs->condition('ldu.application_id', trim($application_id));
		$master_part_mtw_docs_result = $master_part_mtw_docs->execute();
		
		$content_docs	=	$master_part_mtw_docs_result->fetchAssoc();
		
		
		if($content['mtw_loc_areatype']=="B"){
			$text_AreaType_loc = "Block";
		}elseif($content['mtw_loc_areatype']=="M"){
			$text_AreaType_loc = "Municipality";
		}elseif($content['mtw_loc_areatype']=="C"){
			$text_AreaType_loc = "Corporation";
		}elseif($content['mtw_loc_areatype']=="S"){
			$text_AreaType_loc = "SEZ";
		}elseif($content['mtw_loc_areatype']=="N"){
			$text_AreaType_loc = "Notified Area";
		}elseif($content['mtw_loc_areatype']=="O"){
			$text_AreaType_loc = "Other";
		}
		
		if($text_AreaType_loc =='Corporation') {
			$text_AreaType_loc_name='Ward';
			$areatype_loc_name = $content['village_name'];
		}elseif($text_AreaType_loc=='Municipality'){
			$text_AreaType_loc_name='Ward';
			$areatype_loc_name = 'Ward-'.$content['village_name']; 
		}else {
			$text_AreaType_loc_name='Gram Panchayat';	
			$areatype_loc_name = ucwords($content['village_name']);
		}	
		
		
		$personqyery	= db_select('l_mtw_registration_master_extra','lmrme');
		$personqyery->fields('lmrme',array());
		
		
		////////////// Person State Part //////////
		$personqyery->fields('pre_state', array('statename'));
		$personqyery->leftJoin('state_master','pre_state','pre_state.id=lmrme.person_state');
		
		
		////////////// Person Distrct Part //////////
		$personqyery->fields('pre_dist', array('district_name'));
		$personqyery->leftJoin('district_master','pre_dist','pre_dist.district_code=lmrme.person_district');
		
		////////////// Person Sub-Divistion Part //////////
		$personqyery->fields('per_subdiv', array('sub_div_name'));
		$personqyery->leftJoin('sub_division', 'per_subdiv', 'per_subdiv.sub_div_code=lmrme.person_subdivision');
		
		////////////// Person Block Part //////////
		$personqyery->fields('per_block', array('block_mun_name'));
		$personqyery->leftJoin('block_mun_master', 'per_block', 'per_block.block_code=lmrme.person_areatype_code');
		
		////////////// Person Vill Ward Part //////////
		$personqyery->fields('pre_vilwar', array('village_name'));
		$personqyery->leftJoin('village_ward_master', 'pre_vilwar', 'pre_vilwar.village_code=lmrme.person_vill_ward');
		
		////////////// Person Police Station Part //////////
		$personqyery->fields('person_ps', array('name_of_police_station'));
		$personqyery->leftJoin('police_station', 'person_ps', 'person_ps.police_station_code=lmrme.person_ps');
		
		
		$personqyery->condition('lmrme.application_id', trim($application_id));
		
		$personqyery_result = $personqyery->execute();  
		$percontent	=	$personqyery_result->fetchAll();
		
		$designation			= '';
		$name					= '';
		$person_address			= '';
		$person_district		= '';
		$person_sub_div_name	= '';
		$person_block_mun_name	= '';
		$person_village_name	= '';
		$person_ps				= '';
		$pincode				=	'';
		$i						= 0;
		
		
		$persondata	= array();
		
		if($personqyery_result->rowCount()>0){
			
			foreach($percontent as $ob){
				
				$designation				= $ob->designation;
				$name						= $ob->person_name;
				$person_address				= $ob->person_address;
				$person_district			= $ob->district_name;
				$preson_sub_div_name		= $ob->sub_div_name;
				$person_areatype			= $ob->person_areatype;
				$person_block_mun_name		= $ob->block_mun_name;
				$preson_village_name		= $ob->village_name;
				$person_ps					= $ob->name_of_police_station;
				$pincode					= $ob->person_pincode;
				$country					= $ob->person_country;
				$state						= $ob->statename;
				$state_code					= $ob->person_state;
				
				if($person_areatype=="B"){
					$person_areatype_name	= 'Block';
					$person_areatype_name_value	= 'Gram Panchayat';
				}elseif($person_areatype=="C"){
					$person_areatype_name	= 'Corporation';
					$person_areatype_name_value	= 'Ward';
				}elseif($person_areatype=="M"){
					$person_areatype_name	= 'Municipality';
					$person_areatype_name_value	= 'Ward';
				}elseif($person_areatype=="S"){
					$person_areatype_name	= 'SEZ';
					$person_areatype_name_value	= '';
				}elseif($person_areatype=="N"){
					$person_areatype_name	= 'Notified Area';
					$person_areatype_name_value	= '';
				}elseif($person_areatype=="O"){
					$person_areatype_name	= 'Other';
					$person_areatype_name_value	= '';
				}else{
					$person_areatype_name	= '';
					$person_areatype_name_value	= '';
				}
				
				$options_country 	= array(''=>'-Select-', 1 => 'India', 2 => 'Others'); 
				if($country=='1'){
					$pr_country = 'India';						
				}elseif($country=='2'){
					$pr_country = 'Others';	
				}
				
				
				
				if(($designation=="proprietor") || ($designation=="partner")){
					$label1	= ucwords($designation).' if firm not registered under Co Act,1956';
					$slno	= '8.1';
					$slnoAdd= '8.1';
					
					$pp[]	= '
						<tr>
							<td style="text-align:center;">'.$slno.'</td>
							<td>Name of the '.$label1.' </td>
							<td>'.$name.'</td>
						</tr>
					  	<tr>
							<td style="text-align:center;">'.$slno.'.1</td>
							<td>Location of '.ucwords($designation).'</td>
							<td>'.$person_address.'</td>
						</tr>
						<tr>
							<td style="text-align:center;">'.$slnoAdd.'.2</td>
							<td>Country</td>
							<td>'.$pr_country.'</td>
						</tr>
						';
						
					if($state_code !=0 || !empty($state_code)){
					
						$pp[]	= '<tr>
										<td style="text-align:center;">'.$slnoAdd.'.3</td>
										<td>State</td>
										<td>'.$state.'</td>
									</tr>';
						
								if(!empty($person_district) || $person_district!=''){
						$pp[]	= '
									<tr>
										<td style="text-align:center;">'.$slno.'.4</td>
										<td>District</td>
										<td>'.$person_district.'</td>
									</tr>';
								}	
								if(!empty($preson_sub_div_name) || $preson_sub_div_name!=''){
						$pp[]	=	'<tr>
										<td style="text-align:center;">'.$slno.'.5</td>
										<td>Subdivision</td>
										<td>'.$preson_sub_div_name.'</td>
									</tr>';		
								}
																	
								if(!empty($person_areatype_name) || $person_areatype_name!=''){
						$pp[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.6</td>
										<td>'.$person_areatype_name.'</td>
										<td>'.$person_block_mun_name.'</td>
									</tr>';
									
						$pp[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.7</td>
										<td>'.$person_areatype_name_value.'</td>
										<td>'.$preson_village_name.'</td>
									</tr>';		
								}
									
								if(!empty($person_ps) || $person_ps!=''){
						$pp[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.8</td>
										<td>Police Station</td>
										<td>'.$person_ps.'</td>
									</tr>';		
								}
									
								if(!empty($pincode) || $pincode!=''){
						$pp[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.9</td>
										<td>Pin Code</td>
										<td>'.$pincode.'</td>
									</tr>';
								}
					}
				}elseif($designation=="general_manager"){
					$label1	= 'General Manager in case of a public sector undertaking';
					$slno	= '8.2';
					$slnoAdd= '8.2';
					
					$gm[]	= '
						<tr>
							<td style="text-align:center;">'.$slno.'</td>
							<td>Name of the General Manager </td>
							<td>'.$name.'</td>
						</tr>
					  	<tr>
							<td style="text-align:center;">'.$slno.'.1</td>
							<td>Location of General Manager</td>
							<td>'.$person_address.'</td>
						</tr>
						<tr>
							<td style="text-align:center;">'.$slnoAdd.'.2</td>
							<td>Country</td>
							<td>'.$pr_country.'</td>
						</tr>
						';
						
						
					if($state_code !=0 || !empty($state_code)){	
							
						$gm[]	= '<tr>
										<td style="text-align:center;">'.$slnoAdd.'.3</td>
										<td>State</td>
										<td>'.$state.'</td>
									</tr>';
						
								if(!empty($person_district) && $person_district!=''){
						$gm[]	= '
									<tr>
										<td style="text-align:center;">'.$slno.'.4</td>
										<td>District</td>
										<td>'.$person_district.'</td>
									</tr>';
								}	
								if(!empty($preson_sub_div_name) && $preson_sub_div_name!=''){
						$gm[]	=	'<tr>
										<td style="text-align:center;">'.$slno.'.5</td>
										<td>Subdivision</td>
										<td>'.$preson_sub_div_name.'</td>
									</tr>';		
								}
																	
								if(!empty($person_areatype_name) && $person_areatype_name!=''){
						$gm[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.6</td>
										<td>'.$person_areatype_name.'</td>
										<td>'.$person_block_mun_name.'</td>
									</tr>';
									
						$gm[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.7</td>
										<td>'.$person_areatype_name_value.'</td>
										<td>'.$preson_village_name.'</td>
									</tr>';		
								}
									
								if(!empty($person_ps) && $person_ps!=''){
						$gm[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.8</td>
										<td>Police Station</td>
										<td>'.$person_ps.'</td>
									</tr>';		
								}
									
								if(!empty($pincode) && $pincode!=''){
						$gm[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.9</td>
										<td>Pin Code</td>
										<td>'.$pincode.'</td>
									</tr>';
								}
					}
				}elseif($designation=="director"){
					$label1	= 'Director in case of company registered under Co Act, 1956';
					$slno	= '9.1';
					$slnoAdd= '9.1';
					
					$dire[]	= '
					<tr>
							<td style="text-align:center;">'.$slno.'</td>
							<td>Name of the Director </td>
							<td>'.$name.'</td>
						</tr>
					  	<tr>
							<td style="text-align:center;">'.$slno.'.1</td>
							<td>Location of Director</td>
							<td>'.$person_address.'</td>
						</tr>
						<tr>
							<td style="text-align:center;">'.$slnoAdd.'.2</td>
							<td>Country</td>
							<td>'.$pr_country.'</td>
						</tr>';
						
						
					if($state_code !=0 || !empty($state_code)){		
						$dire[]	= '<tr>
										<td style="text-align:center;">'.$slnoAdd.'.3</td>
										<td>State</td>
										<td>'.$state.'</td>
									</tr>';
						
								if(!empty($person_district) && $person_district!=''){
						$dire[]	= '
									<tr>
										<td style="text-align:center;">'.$slno.'.4</td>
										<td>District</td>
										<td>'.$person_district.'</td>
									</tr>';
								}	
								if(!empty($preson_sub_div_name) && $preson_sub_div_name!=''){
						$dire[]	=	'<tr>
										<td style="text-align:center;">'.$slno.'.5</td>
										<td>Subdivision</td>
										<td>'.$preson_sub_div_name.'</td>
									</tr>';		
								}
																
								if(!empty($person_areatype_name) || $person_areatype_name!=''){
						$dire[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.6</td>
										<td>'.$person_areatype_name.'</td>
										<td>'.$person_block_mun_name.'</td>
									</tr>';
									
						$dire[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.7</td>
										<td>'.$person_areatype_name_value.'</td>
										<td>'.$preson_village_name.'</td>
									</tr>';		
								}
									
								if(!empty($person_ps) || $person_ps!=''){
						$dire[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.8</td>
										<td>Police Station</td>
										<td>'.$person_ps.'</td>
									</tr>';		
								}
									
								if(!empty($pincode) || $pincode!=''){
						$dire[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.9</td>
										<td>Pin Code</td>
										<td>'.$pincode.'</td>
									</tr>';
								}		
					}
				}else{
					
					$label1	= 'Director in case of company registered under Co Act, 1956';
					$slno	= '8.3';
					$slnoAdd= '8.3';
					
					$othr[]	= '
					<tr>
							<td style="text-align:center;">'.$slno.'</td>
							<td>Name of the '.ucwords($designation).' </td>
							<td>'.$name.'</td>
						</tr>
					  	<tr>
							<td style="text-align:center;">'.$slno.'.1</td>
							<td>Location of '.ucwords($designation).'</td>
							<td>'.$person_address.'</td>
						</tr>
						<tr>
							<td style="text-align:center;">'.$slnoAdd.'.2</td>
							<td>Country</td>
							<td>'.$pr_country.'</td>
						</tr>';
						
						
								if($state_code !=0 || !empty($state_code)){		
						$othr[]	= '<tr>
										<td style="text-align:center;">'.$slnoAdd.'.3</td>
										<td>State</td>
										<td>'.$state.'</td>
									</tr>';
						
								if(!empty($person_district) && $person_district!=''){
						$othr[]	= '
									<tr>
										<td style="text-align:center;">'.$slno.'.4</td>
										<td>District</td>
										<td>'.$person_district.'</td>
									</tr>';
								}	
								if(!empty($preson_sub_div_name) && $preson_sub_div_name!=''){
						$othr[]	=	'<tr>
										<td style="text-align:center;">'.$slno.'.5</td>
										<td>Subdivision</td>
										<td>'.$preson_sub_div_name.'</td>
									</tr>';		
								}
																	
								if(!empty($person_areatype_name) || $person_areatype_name!=''){
						$othr[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.6</td>
										<td>'.$person_areatype_name.'</td>
										<td>'.$person_block_mun_name.'</td>
									</tr>';
									
						$othr[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.7</td>
										<td>'.$person_areatype_name_value.'</td>
										<td>'.$preson_village_name.'</td>
									</tr>';		
								}
									
								if(!empty($person_ps) || $person_ps!=''){
						$othr[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.8</td>
										<td>Police Station</td>
										<td>'.$person_ps.'</td>
									</tr>';		
								}
									
								if(!empty($pincode) || $pincode!=''){
						$othr[]	=	'<tr>
										<td style="text-align:center;">'.$slnoAdd.'.9</td>
										<td>Pin Code</td>
										<td>'.$pincode.'</td>
									</tr>';
								}		
							}
				}
			}
		}
		
		
	$enc_doc_array = array('TL'=> '','PD'=> '','AOA'=> '','MOA'=> '','FL'=> '','AC'=> '','PC'=> '','BB'	=> '','SC'	=> '','IC'	=> '','AP'=> '','ODSC'=> '','CR'=> '','WO'=> '','ORC'=> '');
	
	$pdficonlink = '<img title="View Documents" alt="View Documents" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/pdf.png"> &nbsp;View';
	
	// data from encrypted data table
	$query 			= db_select('l_encrypted_uploaded_documents','leud')->fields('leud',array())->condition('status','1')->condition('user_id',$user_id);
	$query_result 	= $query->execute();
	
	if($query_result->rowCount() > 0){
		foreach($query_result->fetchAll() as $data ){
			$enc_doc_array[$data->document_type_code]['content'] = 'view_documents/'.encryption_decryption_fun('encrypt',$data->id);
		}
	}
		
	
	// l_documents upload data
	$master_part_mtw_docs = db_select('l_documents_upload', 'ldu');
	$master_part_mtw_docs->fields('ldu', array());
	$master_part_mtw_docs->condition('ldu.user_id', $user_id);
	$master_part_mtw_docs->condition('ldu.act_id', '3');
	$master_part_mtw_docs->condition('ldu.application_id', trim($application_id));
	$master_part_mtw_docs_result = $master_part_mtw_docs->execute();
	
	$content_docs_no = $master_part_mtw_docs_result->rowCount();
	if($content_docs_no > 0){
		$obj3    = $master_part_mtw_docs_result->fetchObject();
	}else{
		$obj3    = array();
	}
	
	$global_link = $base_root.$base_path.'sites/default/files/upload/';
	
	if($enc_doc_array['TL'] == ''){
		$enc_doc_array['TL']['content'] = ($obj3->trade_license_file != '')? $global_link.'trade_license/'.$obj3->trade_license_file : '';
	}
	if($enc_doc_array['AOA'] == ''){
		$enc_doc_array['AOA']['content'] = ($obj3->article_of_assoc_file != '')? $global_link.'article_of_assoc/'.$obj3->article_of_assoc_file : '';
	}
	if($enc_doc_array['PD'] == ''){
		$enc_doc_array['PD']['content'] = ($obj3->article_of_assoc_file != '')? $global_link.'article_of_assoc/'.$obj3->article_of_assoc_file : '';
	}
	if($enc_doc_array['BB'] == ''){
		$enc_doc_array['BB']['content'] = ($obj3->form_one_asses_ses_file != '')? $global_link.'blue_book/'.$obj3->form_one_asses_ses_file : '';
	}
	if($enc_doc_array['SC'] == ''){
		$enc_doc_array['SC']['content'] = ($obj3->form_one_asses_ses_file != '')? $global_link.'blue_book/'.$obj3->form_one_asses_ses_file : '';
	}
	
	if($enc_doc_array['IC'] == ''){
		$enc_doc_array['IC']['content'] = ($obj3->supp_asses_ses_file != '')? $global_link.'insurance/'.$obj3->supp_asses_ses_file : '';
	}
	if($enc_doc_array['AP'] == ''){
		$enc_doc_array['AP']['content'] = ($obj3->address_proof_file != '')? $global_link.'address_proof/'.$obj3->address_proof_file : '';
	}
	if($enc_doc_array['ODSC'] == ''){
		$enc_doc_array['ODSC']['content'] = ($obj3->other_doc_file != '')? $global_link.'OtherRelatedDocuments/'.$obj3->other_doc_file : '';
	}
	
	//echo '<pre>'; print_r($enc_doc_array); die;
	
	
	$output = '';
				
	$output .= '<table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-striped table-responsive dashboard-table sub-table-dashboard">
					  <thead>		  
						  <tr>
						  	<th width="6%">Sl No.</th>
							<th width="40%">Parameters</th>
							<th>Inputs</th>
						  </tr>
					  </thead>
					  <tbody>
					  <tr>
					 	<td style="text-align:center;">1</td>
						<td>Name of Motor Trasport Undertaking </td>
						<td>'.$content['mtw_name'].'</td>
					  </tr>
					  <tr>
					  	<td style="text-align:center;">2</td>
						<td>Location of Motor Trasport Undertaking </td>
						<td>'.$content['mtw_loc_address'].'</td>
					  </tr>
					  <tr>
					  	<td style="text-align:center;">2.1</td>
						<td>District</td>
						<td>'.$content['district_name'].'</td>
					  </tr>
					  <tr>
					  	<td style="text-align:center;">2.2</td>
						<td>Subdivision</td>
						<td>'.$content['sub_div_name'].'<span class="red-star"> '.l(t('(Your Application will be verified by '.$content['sub_div_name'].' RLO office)'),'/contactinfo', array('attributes' => array('class' => 'red-star'))).'</span></td>						
					  </tr>					  
					  <tr>
					  	<td style="text-align:center;">2.3</td>
						<td>'.$text_AreaType_loc.' </td>
						<td>'.$content['block_mun_name'].'</td>
					  </tr>
					  <tr>
					  	<td style="text-align:center;">2.4</td>
						<td>'.$text_AreaType_loc_name.' </td>
						<td>'.$areatype_loc_name.'</td>
					  </tr>
					  <tr>
					  	<td style="text-align:center;">2.5</td>
						<td>Police Station </td>
						<td>'.$content['name_of_police_station'].'</td>
					  </tr>
					   <tr>
					   	<td style="text-align:center;">2.6</td>
						<td>Pin Code </td>
						<td>'.$content['mtw_loc_pincode'].'</td>
					  </tr>
					  <!-------------------------------- MTW Address part end ------------------------------------->
					  <tr>
					  	<td style="text-align:center;">3</td>
						<td>Nature of Motor Trasport Undertaking</td>';
						
						
						if($content['mtw_nature']=='city_service'){
							$mtw_nature_contrnt		=	'City Service';
						}elseif($content['mtw_nature']=='long_distance'){
							$mtw_nature_contrnt		=	'Long Distance';
						}elseif($content['mtw_nature']=='passenger_service'){
							$mtw_nature_contrnt		=	'Passenger Service';
						}elseif($content['mtw_nature']=='long_distance_freight_service'){
							$mtw_nature_contrnt		=	'Long Distance Freight Service';
						}elseif(!empty($content['mtw_nature']) && $content['mtw_nature']!='city_service' && $content['mtw_nature']!='long_distance' && $content['mtw_nature']!='passenger_service' && $content['mtw_nature']!='long_distance_freight_service'){
							$mtw_nature_contrnt		=	$content['mtw_nature'];
						}
						
			$output .='<td>'.$mtw_nature_contrnt.'</td>
					  </tr>
					  
					  <tr>
					  	<td style="text-align:center;">4</td>
						<td>Total Number Of Routes</td>';
						if($content['total_routes'] != ''){
							// details of the route data
							
							$detailsroute = $content['mtw_route_details'];
							
						}else{
							$detailsroute = '';
						}
												
			$output .='<td>'.$content['total_routes'].'</td>
					  </tr>
					  
					  <tr>
					  	<td style="text-align:center;">5</td>
						<td>Route Details - Mileage</td>';
						$output2 = '';
						if(($content['total_routes'] != '') && ($content['total_routes'] != '0') ){
							$total_milage_add 	= 0;
							$total_no		  	= 0;
							$routeDetails1		=	db_select('l_mtw_registration_extended','mtwe');
							$routeDetails1->fields('mtwe',array());
							$routeDetails1->condition('mtwe.application_id',$application_id);
							$fetdata = $routeDetails1->execute();
							
							$allDetails 	=	$fetdata->fetchAll();
							
							if(!empty($allDetails)){
								foreach($allDetails as $datum1){
									//$output2.= $datum1->route.'-'.$datum1->milage.'<br>';
									$total_no++;
									$total_milage_add =  $total_milage_add+$datum1->milage;
								}
							}
							$output2.= $total_no.'-'.$total_milage_add;
						}else{
							$output2.= 'N/A';
						}
			$output .=	'<td>'.$output2.'</td>
					  </tr>
					  <tr>
					  	<td style="text-align:center;">6</td>
						<td>Total Number of MT Vehicles on the last date or the preceeding year</td>
						<td>'.$content['total_mtw_vehicle'].'</td>
					  </tr>
					  <tr>
					  	<td style="text-align:center;">7 <span class="red-star">*</span></td>
						<td>Maximum Number of MTW Employed on any day during the preceeding year</td>
						<td><b><i>'.$content['mtw_maxworkers'].'</i></b></td>
					  </tr>
					  
					  ';
			
			
			if(empty($pp)){
				$output .= '<tr><td style="text-align:center;">8.1</td><td>Name and Address of the Proprietor or Partners of Motor Transport undetaking in the case of firm not registered under the companies Act, 1956 </td><td class="red-star">No data available</td></tr>';
			}else{
				if(isset($pp[0])){ $output .= $pp[0];}
				if(isset($pp[1])){ $output .= $pp[1];}
				if(isset($pp[2])){ $output .= $pp[2];}
				if(isset($pp[3])){ $output .= $pp[3];}
				if(isset($pp[4])){ $output .= $pp[4];}
				if(isset($pp[5])){ $output .= $pp[5];}
				if(isset($pp[6])){ $output .= $pp[6];}
				if(isset($pp[7])){ $output .= $pp[7];}
				if(isset($pp[8])){ $output .= $pp[8];}
			}
			if(empty($gm)){
				$output .= '<tr><td style="text-align:center;">8.2</td><td>Name and Address of the General Manager in case of a public sector undertaking </td><td class="red-star">No data available</td></tr>';
			}else{
				if(isset($gm[0])){ $output .= $gm[0];}
				if(isset($gm[1])){ $output .= $gm[1];}
				if(isset($gm[2])){ $output .= $gm[2];}
				if(isset($gm[3])){ $output .= $gm[3];}
				if(isset($gm[4])){ $output .= $gm[4];}
				if(isset($gm[5])){ $output .= $gm[5];}
				if(isset($gm[6])){ $output .= $gm[6];}
				if(isset($gm[7])){ $output .= $gm[7];}
				if(isset($gm[8])){ $output .= $gm[8];}
			}
			if(!empty($othr)){
				if(isset($othr[0])){ $output .= $othr[0];}
				if(isset($othr[1])){ $output .= $othr[1];}
				if(isset($othr[2])){ $output .= $othr[2];}
				if(isset($othr[3])){ $output .= $othr[3];}
				if(isset($othr[4])){ $output .= $othr[4];}
				if(isset($othr[5])){ $output .= $othr[5];}
				if(isset($othr[6])){ $output .= $othr[6];}
				if(isset($othr[7])){ $output .= $othr[7];}
				if(isset($othr[8])){ $output .= $othr[8];}
			}
			if(empty($dire)){
				$output .= '<tr><td style="text-align:center;">9</td><td>Name and Address of the Director in case of company registered under Co Act, 1956 </td><td class="red-star">No data available</td></tr>';			
			}else{
				if(isset($dire[0])){ $output .= $dire[0];}
				if(isset($dire[1])){ $output .= $dire[1];}
				if(isset($dire[2])){ $output .= $dire[2];}
				if(isset($dire[3])){ $output .= $dire[3];}
				if(isset($dire[4])){ $output .= $dire[4];}
				if(isset($dire[5])){ $output .= $dire[5];}
				if(isset($dire[6])){ $output .= $dire[6];}
				if(isset($dire[7])){ $output .= $dire[7];}
				if(isset($dire[8])){ $output .= $dire[8];}
			}
			
			$output .='<tr>
							<td style="text-align:center;">10</td>
							<td>Fees (<span class="red-star">*</span>This is system generated fees depends on point 7)</td>
							<td> &#8377;'.number_format($retFees, 2).'<input type="hidden" name="retFees" id="retFees" value="'.$retFees.'" />&nbsp;&nbsp;&nbsp;
							
							<span class="viewfile_popup blink" id="fees_chart_pdf" title="CLICK HERE TO VIEW FEES DETAILS">FEES CHART [ In <i class="fa fa-inr" aria-hidden="true"></i> ]</span>
							<div id="view_fees_chart_pdf" title="FEES CHART" style="display:none;">
							
							
								<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/fees_chat/MTW-FEES-CHART.pdf" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
							</div></td>';
								
					//$output .='<td>Fees will be calculated after ALC verification</td>';
								 	
			$output .='</tr>';		  			
			
			$output .='</tbody></table>';
			
			
			$output1 = '';		  
					 //if(!empty($content_docs)){ 
			
			$output1 ='<table class="table table-striped table-responsive dashboard-table sub-table-dashboard" width="100%" cellspacing="0" cellpadding="0" border="0">
						<thead>
							<tr><td colspan="3" align="center">Documents Uploaded</td></tr></thead>
						<tbody>';

			$output1 .='<tr>
							<td style="text-align:center;">1</td>
							<td>Uploaded Trade License</td>';
							$viewlink1 = 'Document not available';
							if($enc_doc_array['TL']['content'] !=''){
								$viewlink1 = l($pdficonlink,$enc_doc_array['TL']['content'],array('html' => TRUE));
							}
			$output1 .=' <td>'.$viewlink1.'</td>';
			$output1 .=' </tr>
						<tr>
							<td style="text-align:center;">2</td>
							<td>Uploaded Articles of Association / Partnership Deed</td>';
							$viewlink2 = 'Document not available';
							if($enc_doc_array['AOA']['content'] !=''){
								$viewlink2 = l($pdficonlink,$enc_doc_array['AOA']['content'],array('html' => TRUE));
							}else if($enc_doc_array['PD']['content'] !=''){
								$viewlink2 = l($pdficonlink,$enc_doc_array['PD']['content'],array('html' => TRUE));
							}
			$output1 .=' <td>'.$viewlink2.'</td>';
			$output1 .=' </tr>
						 <tr>
							<td style="text-align:center;">3</td>
							<td>Uploaded Blue Book/ Smart Card Issued by Motor Vehicles</td>';
							$viewlink3 = 'Document not available';	
							if($enc_doc_array['BB']['content'] !=''){
								$viewlink3 = l($pdficonlink,$enc_doc_array['BB']['content'],array('html' => TRUE));
							}else if($enc_doc_array['SC']['content'] !=''){
								$viewlink3 = l($pdficonlink,$enc_doc_array['SC']['content'],array('html' => TRUE));
							}
			$output1 .=' <td>'.$viewlink3.'</td>';
			$output1 .='</tr>
						<tr>
							<td style="text-align:center;">4</td>
							<td>Uploaded Insurance Certificate of Motor Vehicles</td>';
								$viewlink4 = 'Document not available';	
								if($enc_doc_array['IC']['content'] !=''){
								$viewlink4 = l($pdficonlink,$enc_doc_array['IC']['content'],array('html' => TRUE));
								}
			$output1 .=' <td>'.$viewlink4.'</td>';			
				
			$output1 .='</tr>
						<tr>
							<td style="text-align:center;">5</td>
							<td>Uploaded Documents in support of correctness of the application</td>';
							
							$viewlink5 = 'Document not available';	
							if($enc_doc_array['ODSC']['content'] !=''){
								$viewlink5 = l($pdficonlink,$enc_doc_array['ODSC']['content'],array('html' => TRUE));
							}
			$output1 .=' <td>'.$viewlink5.'</td>';
			$output1 .='</tr>
						<tr>
						  <td style="text-align:center;">6</td>
						  <td>Uploaded Address Proof</td>';
						
							$viewlink6 = 'Document not available';	
							if($enc_doc_array['AP']['content'] !=''){
								$viewlink6 = l($pdficonlink,$enc_doc_array['AP']['content'],array('html' => TRUE));
							}
			$output1 .=' <td>'.$viewlink6.'</td>';
			$output1 .='</tr>
						<tr>
							<td style="text-align:center;">7</td>
							<td>Form -I </td>';
							if(!empty($content_docs['form_1_mtw_signed_pdf_file'])){
								
								$output1 .='<td><a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/MTW_Form-I/'. $content_docs['form_1_mtw_signed_pdf_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png" title="Click to view and print FORM-I"></a></td>';
							} else { 
								$output1 .='<td>Form-I should be uploaded by the Applicant after successful payment of fees</td>';
							} 
			$output1 .='</tr>';
    				// }			 				  
	$output1 .='</tbody></table>';
					  
	return $output.$output1;
}

function mtw_application_preview_submit($form, &$form_state){
	
		global $base_root, $base_path, $user; 
		
		$user_id			= $user->uid;
		$remark_field_title = '';
		
		$val				= $form_state['values'];  //echo '<pre>'; print_r($val); die;
		$application_id		= $val['application_id'];
		$backlog_id			= $val['backlog_id'];
		$renewal_id			= $val['renewal_id'];
		$renewal_ref_number	= $val['renewal_ref_number'];
		
		//$retFees			= $_POST['retFees'];
		
		/******** common data part ********/
		
		$fetch_reg_det 			= db_select('l_mtw_registration_master', 'lmrm');
		$fetch_reg_det->fields('lmrm', array('application_date', 'created','mtw_name','mtw_maxworkers','status','backlog_id'));
		$fetch_reg_det->condition('lmrm.user_id', $user_id);
		$fetch_reg_det->condition('lmrm.id', $application_id); 
		$fetch_reg_det_result 	= $fetch_reg_det->execute(); 
		
		if( $fetch_reg_det_result->rowCount() > 0 ){
			$obj2 					= $fetch_reg_det_result->fetchAssoc(); 
			$application_date		= !empty($obj2['application_date']) ? $obj2['application_date'] :'';
			$created_date			= !empty($obj2['created'])? $obj2['application_date'] :'';
			$emp_name				= $obj2['mtw_name'];
			$status					= $obj2['status'];
			$backlog_id				= $obj2['backlog_id'];
			
			$fees 					= $obj2['mtw_maxworkers'];		
			if($fees <= 5){
				$retFees	=	'10';
			}else if($fees > 5 && $fees <= 25  ){
				$retFees	=	'25';
			}else if($fees > 25 && $fees <= 50  ){
				$retFees	=	'50';
			}else if($fees > 50 && $fees <= 100  ){
				$retFees	=	'100';
			}else if($fees > 100 && $fees <= 250  ){
				$retFees	=	'250';
			}else if($fees >250 && $fees <= 500){
				$retFees	=	'500';
			}else if($fees > 500 && $fees <= 750  ){
				$retFees	=	'750';
			}else if($fees > 750){
				$retFees	=	'1000';
			}
		}else{
				$retFees	= '0';
		}
		
		$current_fees		= $retFees;
		
		// last expiry date
		$qlexp = db_select('l_mtw_registration_backlog','mtwbk')->fields('mtwbk',array())->condition('mtwbk.id',$backlog_id)->execute();
		if($qlexp->rowCount() > 0){
			$qlexpr = $qlexp->fetchObject();
			$last_expiry_date = $qlexpr->expiry_date;
			$now_expire_date = date('Y-m-d', strtotime('+1 year', strtotime($last_expiry_date)));
			$previous_fees = $qlexpr->paidfees;
			$generate_ref_no = 'MTW-RENEWAL-'.$application_id.'-'.time();
			$remark_text = 'Successfully Applied for Certificate Renewal';
			$is_renew = 1;
		}else{
			$now_expire_date = date('Y-m-d', strtotime('12/31'));
			$previous_fees = 0;
			$generate_ref_no = 'MTW-REG-'.$application_id.'-'.time();
			$remark_text = 'Applied Successfully';
			$is_renew = 0;
		}
		
		$renewal_ref_number = $generate_ref_no;
		
		if($renewal_id != 0 && $renewal_id != ''){
			$renewal_ref_number = $val['renewal_ref_number'];
		}
		
		if($backlog_id !=0 && $backlog_id !=''){
			// renewal table data insert
			
			$insertRenewalData	=	array(
				'application_id'	 	=> $application_id,
				'previous_fees' 		=> $previous_fees,
				'current_fees' 			=> $current_fees,
				//'final_fees' 			=> $final_fees,
				'purpose'				=> 'renewal',
				'application_date'		=> date("Y-m-d"), // Applicant apply date
				'renewal_ref_number' 	=> $renewal_ref_number,
				//'renewal_date' 		=>  date("Y-m-d"),
				'payment_status'		=> '',
				'status'				=> '1',
				'expiry_date'			=> $now_expire_date,
				'user_id'				=> $user_id
			); 
						
			// check update or insert 
			$chk = db_query("select id from l_mtw_registration_renewal where application_id=".$application_id." and status = '1'");
			if($chk->rowCount() > 0){
				db_update('l_mtw_registration_renewal')->fields($insertRenewalData)->condition('application_id',$application_id)->condition('status','1')->execute();
			}else{
				$renewal_id = db_insert('l_mtw_registration_renewal')->fields($insertRenewalData)->execute();
			}
		}
				
		$fetch_users_role = db_select('users_roles', 'ur');
		$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
		$fetch_users_role->fields('ro', array('rid'));
		$fetch_users_role->condition('ur.uid', $user_id);
		$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc();
	
		$user_role			=	$fetch_users_role_data['rid']; 
		
		$fieldsRemarksData 		= 	array( 
			'remark_text'		=> $remark_text,
			'remark_by'			=> $user_id,
			'remark_to'			=> $user_id,
			'application_id'    => $application_id,
			'remark_field_title'=> $remark_field_title,
			'remark_date' 		=> time(),
			'remark_by_name'	=> $emp_name,
			'remark_by_role'	=> $user_role
		);
		
		if($renewal_id != 0 && $renewal_id != '' && $backlog_id !=0){
			$remark_type = 'RN';
			$fieldsRemarksData['ref_number'] = $renewal_ref_number;
			$fieldsRemarksData['renewal_id'] = $renewal_id;
			$fieldsRemarksData['remark_type'] = $remark_type;
			
		}else{
			$remark_type = '0';
			$is_renew = 0;
			$fieldsRemarksData['ref_number'] = 0;
			$fieldsRemarksData['renewal_id'] = 0;
			$fieldsRemarksData['remark_type'] = $remark_type;
		}			
		
		db_insert('l_mtw_remark_master')->fields($fieldsRemarksData)->execute();
		
		$master_update = array(
			'status' => $remark_type,
			'final_submit_status' => 'P',
			'finalfees' => $retFees,
			'application_date' 	=> date("Y-m-d"),
			'is_renew'	=> $is_renew,
		);
		
		if(empty($created_date) && $status != 'B'){
			$master_update['created'] = time();
		}

		if($is_renew == 0 && $backlog_id ==0 ){
			$master_update['reg_application_date'] =  date("Y-m-d"); // Added By Moumita 20/03/2023
			$master_update['reg_status'] = $remark_type; // Added By Moumita 20/03/2023
		}
		
		$sql_query = db_update('l_mtw_registration_master')->fields($master_update)->condition('id',$application_id)->condition('user_id',$user_id)->execute();
		
		// backlog status change
		
		db_update('l_mtw_registration_backlog')->fields(array('status'=>1))->condition('id',$backlog_id)->execute();
		
		// send sms start
		$phone = get_mobile_number($user_id);
		if(!empty($phone)){
			$msg	=	"Your application has been successfully submitted.You can track your application status from dashboard.";
			send_sms_for_user_alert($phone, $msg);
			//send_sms_for_user_alert('8017090753', $msg);
		}else{
			$msg1	=	"Message not sent.Please try again later.";
			drupal_set_message(t($msg1));
		}
		
		//////////////////    send mail start   //////////////////////////////////
		
				// $mailto  = get_email($user_id);
				// $subject = 'Welcome to Labour Commissionerate, Govt. Of West Bengal';
				// if(!empty($mailto)){
				// 	send_mail_for_user_alert($mailto, $subject, $msg);
				// }
		
		//////////////////  MTW API SERVICE FOR LD  /////////////////////////////////
		
		$api_q2 = 	db_select('l_common_application_master', 'lcam');
		$api_q2	->	fields('lcam', array('ld_uid'));
		$api_q2	->	condition('lcam.user_id', $user_id);
		$api_q2_result = $api_q2->execute(); 
		$api_ap2_details = $api_q2_result->fetchAssoc();
		
		$ld_id = trim($api_ap2_details['ld_uid']);
		
		if(!empty($ld_id) || $ld_id != ""){
			if($_SESSION['service_id'] == 3){
				drupal_goto('https://wblabour.gov.in/dashboard');
			}
		}
		//////////////////  END MTW API SERVICE FOR LD  /////////////////////////////////	
		
		$message = 'Application is applied successfully. Inspection will be started shortly.';
		drupal_set_message(t($message));
		drupal_goto('mtw-registration-details/view/'.encryption_decryption_fun('encrypt', $application_id));
		
		
		

}