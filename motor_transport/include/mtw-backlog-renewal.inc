<?php

drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
drupal_add_css(drupal_get_path('module', 'motor_transport') . '/css/popup.css');
drupal_add_js(drupal_get_path('module', 'motor_transport') . '/js/mtw_upload_custom.js');

function mtw_renewal($form, &$form_state, $id=''){
		
	global $base_root,$base_path,$user;
	// if( ($user->uid !=65861) && $user->uid !=64){
	//    die('Service Under Maintenance. Sorry for the inconvenience');
   	// }
	
	
	$user_id					= $user->uid;
	
	$action						= 'decrypt';
	$action_encrypt				= 'encrypt';
	$application_id				= encryption_decryption_fun($action, $id);
	
	
	//die($application_id.'111');
	
	
	// master table data
	$master_part_mtw_reg 		= db_select('l_mtw_registration_master', 'lmrm');
	$master_part_mtw_reg->fields('lmrm', array('final_submit_status', 'status', 'backlog_id','is_renew'));
	$master_part_mtw_reg->condition('lmrm.user_id', $user_id);
	if(!empty($application_id) || $application_id!=''){
		$master_part_mtw_reg->condition('lmrm.id', $application_id);
	}
	$master_part_mtw_reg_result = $master_part_mtw_reg->execute(); 		 
	$applicant_details			=	$master_part_mtw_reg_result->fetchAssoc();
	$final_submit_status		=	trim($applicant_details['final_submit_status']);
	$status						=	trim($applicant_details['status']);  
	$backlog_id					=	trim($applicant_details['backlog_id']);  
	$is_renew					=	trim($applicant_details['is_renew']);
	
	// l_documents upload data
	$master_part_mtw_docs = db_select('l_documents_upload', 'ldu');
	$master_part_mtw_docs->fields('ldu', array());
	$master_part_mtw_docs->condition('ldu.user_id', $user_id);
	$master_part_mtw_docs->condition('ldu.act_id', '3');
	$master_part_mtw_docs->condition('ldu.application_id', trim($application_id));
	$master_part_mtw_docs_result = $master_part_mtw_docs->execute();
	
	$content_docs	=	$master_part_mtw_docs_result->rowCount();
	
	// l_mtw_registration_master_extra ownership data
	$ownership = db_select('l_mtw_registration_master_extra', 'lmrme');
	$ownership->fields('lmrme', array());
	$ownership->condition('lmrme.application_id', trim($application_id));
	$ownership_result = $ownership->execute();
	
	$ownership	=	$ownership_result->rowCount();
	
	
	
	$form['mtw_app_final_preview'] 			= array(
									'#type' 				=> 'fieldset',
									'#attributes'		 		=> array('class'=>array(''))
								);
	$form['mtw_app_final_preview']['application_id']	= array(
									'#title'				=> 'Application Id',
									'#type'					=> 'hidden',
									'#default_value' 			=> !empty($application_id) ? $application_id : '',
									'#attributes' 				=> array('readonly' => 'readonly'),
								);
																									
																									
	$form['mtw_app_final_preview']['backlog_id']		= array(
									'#title'				=> 'Backlog Id',
									'#type'					=> 'hidden',
									'#default_value' 		=> !empty($backlog_id) ? $backlog_id : '',
									'#attributes' 			=> array('readonly' => 'readonly'),
								);
																									
	
																									
	$form['mtw_app_final_preview']['markup_data_1']										= array(
																									'#prefix' 				=> '<div class="content form-regs-mtw"><div class="sky-form sky-form-mtw" style="width:100%;"><fieldset>',
																									'#markup' 			    => view_mtw_app_final_preview($application_id),
																									);
																									
		
	
	
	if($status == '' || $status == 'B'){
		
			if(empty($content_docs) || $content_docs==0){
				$form['mtw_app_final_preview']['finalAgree'] = array(
																'#type'						=> 'markup',
																'#markup'					=> l('Upload Documents Now','mtw-registration-details/upload/'.encryption_decryption_fun($action_encrypt, $application_id), array('html' => TRUE, 'attributes' => array('target' => '', 'class' => array('button')))),
															);
			}else if($ownership =='' || $ownership==0){
				$form['mtw_app_final_preview']['finalAgree'] = array(
																	'#type'						=> 'markup',
																	'#markup'					=> l('Add Ownership Details','mtw-registration-details/addmore/'.encryption_decryption_fun($action_encrypt, $application_id), array('html' => TRUE, 'attributes' => array('target' => '', 'class' => array('button')))),
																);
			
			}else{
				$form['mtw_app_final_preview']['finalAgree'] 	= array(																		 
																		'#prefix' 				=> '<div class="clear row renewal-info-note"><label class="">',
																		'#suffix' 				=> '</label></div>',
																		'#type' 				=> 'checkbox',
																		'#description' 			=> '<span class="red-star">*</span>I hereby declare that the particulars given above are true the best of my knowledge and belief.',
																		'#attributes'			=> array('id' => 'finalAgreeId','onchange' => 'return checkFinalAppCheckBoxMTW()'),
																		'#required' 			=> TRUE,
																		
																																					  
																);
		
				$form['mtw_app_final_preview']['submit']		= array(
																		'#type' 				=> 'submit',
																		'#id'					=> 'submitBtnMtw',
																		'#value' 				=> 'SUBMIT',
																		'#attributes' 			=> array('class' => array('button'), 'onclick' => 'return finalsubmitConfirm(); ','style'=>'background-color:#ff6666'),
																		//'#prefix' 				=> '</fieldset><footer>',  
																		//'#suffix' 				=> '</footer></div></div>', 
																);
			}
	}else{

		$form['mtw_app_final_preview']['download_pdf']									= array(
																								'#type'					=> 'markup',
																								'#markup' 				=>  l(' VIEW&nbsp;&amp;&nbsp;PRINT','mtw-generate-pdf/'.encryption_decryption_fun($action_encrypt, $application_id).'/'.encryption_decryption_fun($action_encrypt, 'self'), array('html' => TRUE, 'attributes' => array('target' => '_blank', 'class' => array('button')))),
																								
																								//'#prefix' 				=> '</fieldset><footer>',  
																								//'#suffix' 				=> '</footer></div></div>',
																								);
	}
	
	$form['mtw_app_final_preview']['markup_data_2']			= array(
																	'#type'					=> 'markup',
																	'#markup' 				=> '</fieldset>',
																	//'#markup' 			    => abrt($application_id),
															);
		
	
	return $form;						
	
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
		
		if($content['workmen_if_same_similar_kind_of_work']=="0"){
				$similar_kind_of_work = "No";
		}else{
			$similar_kind_of_work = "Yes";
		}
		
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
						/*$pp[]	=	'<tr>
										<td style="text-align:center;">'.$slno.'.6</td>
										<td>Area Type</td>
										<td>'.$person_areatype_name.'</td>
									</tr>';	*/	
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
						/*$gm[]	=	'<tr>
										<td style="text-align:center;">'.$slno.'.6</td>
										<td>Area Type</td>
										<td>'.$person_areatype_name.'</td>
									</tr>';	*/	
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
								
							if(!empty($person_areatype_name) && $person_areatype_name!=''){
					/*$dire[]	=	'<tr>
									<td style="text-align:center;">'.$slno.'.6</td>
									<td>Area Type</td>
									<td>'.$person_areatype_name.'</td>
								</tr>';	*/	
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
									
								if(!empty($person_areatype_name) && $person_areatype_name!=''){
						/*$othr[]	=	'<tr>
										<td style="text-align:center;">'.$slno.'.6</td>
										<td>Area Type</td>
										<td>'.$person_areatype_name.'</td>
									</tr>';	*/	
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
		
		$output ='';			
		$output .= '<table width="100%" border="0" cellspacing="0" cellpadding="0" class="clra_applications view-act-rules-table">
					  <tr>
					   <td colspan="3" class="sub-title-mtw">Renewal of Certificate of Registration of Motor Transport Workers</td>
					  </tr>
					  <tr>
					  	<th width="6%">Sl. No.</th>
						<th width="40%">Parameters</th>
						<th>Inputs</th>
					  </tr>
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
					$output .= $pp[0].$pp[1].$pp[2].$pp[3].$pp[4].$pp[5].$pp[6].$pp[7].$pp[8];
				}
				if(empty($gm)){
					$output .= '<tr><td style="text-align:center;">8.2</td><td>Name and Address of the General Manager in case of a public sector undertaking </td><td class="red-star">No data available</td></tr>';
				}else{
					$output .= $gm[0].$gm[1].$gm[2].$gm[3].$gm[4].$gm[5].$gm[6].$gm[7].$gm[8];
				}
				if(!empty($othr)){
					$output .= $othr[0].$othr[1].$othr[2].$othr[3].$othr[4].$othr[5].$othr[6].$othr[7].$othr[8];
				}
				if(empty($dire)){
					$output .= '<tr><td style="text-align:center;">9</td><td>Name and Address of the Director in case of company registered under Co Act, 1956 </td><td class="red-star">No data available</td></tr>';			
				}else{
					$output .= $dire[0].$dire[1].$dire[2].$dire[3].$dire[4].$dire[5].$dire[6].$dire[7].$dire[8];
				}
			
			$output .='<tr>
							<td style="text-align:center;">10</td>
							<td>Fees (<span class="red-star">*</span>This is system generated fees depends on point 7)</td>
							<td> &#8377;'.number_format($retFees, 2).'<input type="hidden" name="retFees" id="retFees" value="'.$retFees.'" />&nbsp;&nbsp;&nbsp;
							<span class="view_fees_details viewfile_popup1" id="retFeesid">CLICK HERE TO VIEW FEES DETAILS</span>
							<div id="view_retFeesid" title="FEES CHART" style="display:none;">
							<iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/fees_chat/MTW-FEES-CHART.pdf" style="min-width:600px; min-height:600px;" frameborder="0"></iframe>
							</div></td>';
								
					//$output .='<td>Fees will be calculated after ALC verification</td>';
								 	
			$output .='</tr>';		  			
					  
					 //if(!empty($content_docs)){ 
			
			$output		.=   '<tr>
						<td colspan="3" align="center" class="sub-title-mtw">Uploaded Documents</td>
					  </tr>';
			
			$output .='<tr>
							<td style="text-align:center;">1</td>
							<td>Uploaded Trade License</td>';
							
						   if(!empty($content_docs['trade_license_file'])){ 
							$output .=' <td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/trade_license/'.$content_docs['trade_license_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png" title="Click to view and print Trade License"></a></td>';
						   } else { 
							$output .=' <td>Document not available</td>';
						  } 
			$output .='</tr>
						<tr>
							<td style="text-align:center;">2</td>
							<td>Uploaded Articles of Association / Partnership Deed</td>';
							 if(!empty($content_docs['article_of_assoc_file'])){ 
					
						 		$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/article_of_assoc/'.$content_docs['article_of_assoc_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png" title="Click to view and print Articles of Association and Memorandum of Association/Partnership Deed"></a></td>';
					 		} else { 
								$output .='<td>Document not available</td>';
						 	}
				$output .='</tr>
							  <tr>
							  	  <td style="text-align:center;">3</td>
								  <td>Uploaded Blue Book/ Smart Card Issued by Motor Vehicles</td>';
								
								 if(!empty($content_docs['form_one_asses_ses_file'])){ 
								 
									$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/blue_book/'. $content_docs['form_one_asses_ses_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png" title="Click to view and print Form I for Assesment of SES" ></a></td>';
								 } else { 
									$output .='<td>Document not available</td>';
								 } 			
				$output .='</tr>
							   <tr>
							   	  <td style="text-align:center;">4</td>
								  <td>Uploaded Insurance Certificate of Motor Vehicles</td>';
								
								 if(!empty($content_docs['supp_asses_ses_file'])){ 
								 
									$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/insurance/'. $content_docs['supp_asses_ses_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png" title="Click to view and print documents in support of Payment of SES"></a></td>';
								 } else { 
									$output .='<td>Document not available</td>';
								 } 			
					
					$output .='</tr>
					
							  <tr>
							  	  <td style="text-align:center;">5</td>
								  <td>Uploaded Documents in support of correctness of the application</td>';
								
								 if(!empty($content_docs['other_doc_file'])){ 
								 
									$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/OtherRelatedDocuments/'. $content_docs['other_doc_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png" title="Click to view and print other documents of application"></a></td>';
								 } else { 
									$output .='<td>Document not available</td>';
								 } 
					$output .='</tr>
					
							  <tr>
							  	  <td style="text-align:center;">6</td>
								  <td>Uploaded Address Proof</td>';
								
								 if(!empty($content_docs['address_proof_file'])){ 
								 
									$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/address_proof/'. $content_docs['address_proof_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png" title="Click to view and print Address Proof"></a></td>';
								 } else { 
									$output .='<td>Document not available</td>';
								 } 
					$output .='</tr>
							<tr>
									<td style="text-align:center;">7</td>
							   		<td>Form -I </td>';
									if(!empty($content_docs['form_1_mtw_signed_pdf_file'])){
										
										$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/MTW_Form-I/'. $content_docs['form_1_mtw_signed_pdf_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png" title="Click to view and print FORM-I"></a></td>';
								 	} else { 
										$output .='<td>Form-I should be uploaded by the Applicant after successful payment of fees</td>';
								 	} 
					$output .='</tr>';
					
					
				
    				// }
					 
					  
	$output .='</table>';
			
  
					  
	return $output;
	
}

function mtw_renewal_submit($form, &$form_state){
		global $base_root, $base_path, $user; 
		
		$user_id			= $user->uid;
		$backlog_field_ids 	= '';
		$previous_fees		= 0;
		$current_fees		= 0;
		$final_fees			= 0;
		$fees				= 0;
		
		$val				= $form_state['values'];  
		
		if($val['finalAgree']==1){
			$application_id		= $val['application_id'];
			$backlog_id			= $val['backlog_id'];
			$retFees			= $_POST['retFees'];
			
			/******* Registration process ********/
			
		
			$fetch_reg_det 			= db_select('l_mtw_registration_master', 'lmrm');
			$fetch_reg_det->fields('lmrm', array('application_date', 'created','mtw_name','mtw_maxworkers','status'));
			$fetch_reg_det->condition('lmrm.user_id', $user_id);
			$fetch_reg_det->condition('lmrm.id', $application_id); 
			$fetch_reg_det_result 	= $fetch_reg_det->execute(); 
			
				
			if( $fetch_reg_det_result->rowCount() > 0 ){
				$obj2 					= $fetch_reg_det_result->fetchAssoc(); 
				$application_date		= !empty($obj2['application_date']) ? $obj2['application_date'] :'';
				$created_date			= !empty($obj2['created'])? $obj2['application_date'] :'';
				$emp_name				= $obj2['mtw_name'];
				$status					= $obj2['status'];
				
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
			
			$fetch_users_role					= db_select('users_roles', 'ur');
			$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
			$fetch_users_role->fields('ro', array('rid'));
			$fetch_users_role->condition('ur.uid', $user_id);
			$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc();
		
			$user_role			=	$fetch_users_role_data['rid']; 
					
			if(empty($application_date) && empty($created_date)){
				
				$sql_query 			= db_update('l_mtw_registration_master');
				$sql_query->fields(array(
						 'status'  				=> '0',
						 'application_date' 	=> date("Y-m-d"),
						 'final_submit_status' 	=> 'P',
						 'finalfees' 			=> $retFees,
						 'created'				=> time()
				));
				$sql_query->condition('id',$application_id);
				$sql_query->condition('user_id',$user_id);
				$sql_query->execute();
				
			}else{
				
				$sql_query 			= db_update('l_mtw_registration_master');
				$sql_query->fields(array(
						 'status'  		=> '0',
						 'application_date' 	=> date("Y-m-d"),
						 'final_submit_status' 	=> 'P',
				));
				$sql_query->condition('id',$application_id);
				$sql_query->condition('user_id',$user_id);
				$sql_query->execute();
				
			}
			
			/*******end of registration ********/
			
			/*******start renewal ********/
			
			$fetch_reg_det 		= db_select('l_mtw_registration_master', 'lmrm');
			$fetch_reg_det->fields('lmrm', array('application_date', 'created','mtw_name','registration_date','finalfees','mtw_maxworkers','backlog_id'));
			$fetch_reg_det->condition('lmrm.user_id', $user_id);
			$fetch_reg_det->condition('lmrm.id', $application_id); 
			$fetch_reg_det_result 	= $fetch_reg_det->execute(); 
			
				
			if($fetch_reg_det_result->rowCount() > 0 ){
				$obj2 					= $fetch_reg_det_result->fetchAssoc(); 
				$application_date		= !empty($obj2['application_date']) ? $obj2['application_date'] :'';
				$created_date			= !empty($obj2['created'])? $obj2['application_date'] :'';
				$emp_name				= $obj2['mtw_name'];
				
				$registration_date		= $obj2['registration_date'];
				$previous_fees			= $obj2['finalfees'];
				$application_date		= $obj2['application_date'];
				$fees 					= $obj2['mtw_maxworkers'];
			}
			
			$fetch_users_role					= db_select('users_roles', 'ur');
			$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
			$fetch_users_role->fields('ro', array('rid'));
			$fetch_users_role->condition('ur.uid', $user_id);
			$fetch_users_role_data 				= $fetch_users_role->execute()->fetchAssoc();
		
			$user_role							=	$fetch_users_role_data['rid']; 
			
		
			$phone = get_common_function_for_phone();
			//module_load_include('inc', 'common_encryption_form', 'include/test_cryptography_form');
			if(!empty($application_id) && $application_id !=''){
				
				// if exist update previous data status in renewal table
				$checkFirst 	=	db_select('l_mtw_registration_renewal','lmrr');
				$checkFirst->fields('lmrr',array('id'));
				$checkFirst->condition('lmrr.id', $application_id); 
				$checkFirst_result 	= $checkFirst->execute(); 
				if($checkFirst_result->rowCount() > 0){
					$updateQuery	=	db_update('l_mtw_registration_renewal')->fields(array('status'=>'I'))->condition('application_id',$application_id)->execute();
				}
				
				
				
				
				// calculation of current fees
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
				
				$current_fees		= $retFees;
				
				$final_fees	= $current_fees + $previous_fees;
				
				// insert data in renewal table
				$insertRenewalCrData	=	array(
											'application_id'	 	=>  $application_id,
											'previous_fees' 		=>	$previous_fees,
											'current_fees' 			=>	$current_fees,
											//'final_fees' 			=>	$final_fees,
											'purpose'				=>  'renewal',
											'application_date'		=> 	date("Y-m-d"), // Applicant apply date
											'renewal_ref_number' 	=>  'MTW-RENEWAL-'.$application_id.'-'.time(),
											//'renewal_date' 			=>  date("Y-m-d"),
											'payment_status'		=>	'',
											'status'				=>	'1'
										); 
				
				if($status != 'B'){
					$renewal_id 	=	db_insert('l_mtw_registration_renewal')->fields($insertRenewalCrData)->execute();		
				}
				$sql_query 			= db_update('l_mtw_registration_master');
				$sql_query->fields(array(
						 //'application_date'		=>	date("Y-m-d"),
						 'status'  				=> 'RN', // RENEWAL STATUS
						 'final_submit_status' 	=> 'P',
						 'is_renew'				=> 1,	// cheking for renwal apply
						 'created'				=> time()
				));
				$sql_query->condition('id',$application_id);
				$sql_query->condition('user_id',$user_id);
				$sql_query->execute();
				
				// get reference number from renewal table to track
				
				$getRefQuery	=	db_select('l_mtw_registration_renewal','lmrr');
				$getRefQuery->fields('lmrr',array('renewal_ref_number'));
				$getRefQuery->condition('id',$renewal_id);
				$getRefData		=	$getRefQuery->execute();
				if($getRefData->rowCount() > 0){
					$getRefArray	=	$getRefData->fetchObject();
					$ref_number		= 	$getRefArray->renewal_ref_number;
				}else{
					$ref_number		=	'';
				}
				
				
				
				$fieldsRemarksData 	= 	array( 
					'remark_text'		=> 'Successfully Applied for Certificate Renewal',
					'remark_by'			=> $user_id,
					'remark_to'			=> $user_id,
					'application_id'    => $application_id,
					'remark_type' 		=> 'RN',
					'remark_field_title'=> '',
					'remark_date' 		=> time(),
					'remark_by_name'	=> $emp_name,
					'remark_by_role'	=> $user_role,
					'ref_number'		=> $ref_number
				);
				
				db_insert('l_mtw_remark_master')->fields($fieldsRemarksData)->execute();
				
				if(!empty($phone)){
					$msg	=	"Your application has been successfully submitted.You can track your application status from dashboard.";
					send_sms_for_user_alert($phone, $msg);
					//send_sms_for_user_alert('7278405418', $msg);
				}else{
					$msg	=	"Message not sent.Please try again later.";
					drupal_set_message(t($msg));
				}
			}else{			
				if(!empty($phone)){
					$msg	=	"Your modified application has been successfully submitted.";
					send_sms_for_user_alert($phone, $msg);
					//send_sms_for_user_alert('7278405418', $msg);
				}else{
					$msg	=	"Message not sent.Please try again later.";
					drupal_set_message(t($msg));
					//send_sms_for_user_alert('7278405418', $msg);
				}
			}
			$message = 'Renewal of Certificate Applied Successfully. Please wait for approval.';
			drupal_set_message(t($message));
		}else{
			$message = 'Please Check Declaration Box! And again Submit...';
			drupal_set_message(t($message));
		}
}
