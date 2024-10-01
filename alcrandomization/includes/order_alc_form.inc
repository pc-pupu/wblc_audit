<?php

drupal_add_library('system','ui.datepicker');
drupal_add_js(drupal_get_path('module', 'alcrandomization').'/js/alc_custom_js.js');
drupal_add_css(drupal_get_path('module', 'alcrandomization').'/css/custom_css.css');

//drupal_add_js(drupal_get_path('module', 'alcrandomization').'/js/alcrandomization.js');

function order_alc_form($form, &$form_state, $pre_area_division_type='', $pre_district_code='', $pre_subdivision_code=''){ 
	
	global $base_root, $base_path, $user;
	$logged_in_id				=	$user->uid;
	$link						=	$base_root.$base_path.'sites/all/modules/dlc/sample_doc/inspection orders.docx';
	
	$action						=	'decrypt';
	$area_division_type			=	encryption_decryption_fun($action, $pre_area_division_type);
	$district_code				=	encryption_decryption_fun($action, $pre_district_code);
	$subdivision_code			=	encryption_decryption_fun($action, $pre_subdivision_code);
	//echo $district_code; exit;
	if($district_code==17){
		$query_kol				=	db_select('l_area_inspector_random_tag_master','lairtm');
		$query_kol				->	fields('lairtm', array('id', 'confirm_date', 'dlc_user_id', 'distict_code', 'subdivision_code'));
		$query_kol				->	condition('lairtm.distict_code', $district_code,'=');
		$query_kol				->	condition('lairtm.subdivision_code', $subdivision_code,'=');
		$query_kol				->	orderBy('id', 'DESC');
		$query_kol 				= 	$query_kol->extend('PagerDefault')->limit(1);
		$total_name 			=  	$query_kol->execute();		
		$query_kol_res			=	$total_name->fetchAssoc();
		
		$distict_code_kol		=	$query_kol_res['distict_code'];
		$subdivision_code_kol	=	$query_kol_res['subdivision_code'];
		$confirm_date_kol		=	$query_kol_res['confirm_date'];
		$dlc_user_id_kol		=	$query_kol_res['dlc_user_id'];
		$order_no_kol			=	$query_kol_res['id'].'-INSP/EL/LC/'.date("Y");
		$id_kol					=	$query_kol_res['id'];
		
		
		/*$today 					= 	date("Ymd");
		$rand 					= 	strtoupper(substr(uniqid(sha1(time())),0,4));
		$unique 				= 	$today . $rand;*/
		
		
	}else{
		$query_others			=	db_select('l_area_inspector_random_tag_master','lairtm');
		$query_others			->	fields('lairtm', array('id', 'confirm_date', 'dlc_user_id', 'distict_code', 'subdivision_code'));
		$query_others			->	condition('lairtm.distict_code', $district_code,'=');
		$query_others			->	condition('lairtm.subdivision_code', $subdivision_code,'=');
		$query_others			->	orderBy('id', 'DESC');
		$query_others 			= 	$query_others->extend('PagerDefault')->limit(1);
		$total_name_others 		=  	$query_others->execute();		
		$query_others_res		=	$total_name_others->fetchAssoc();
		
		$distict_code_others		=	$query_others_res['distict_code'];
		$subdivision_code_others	=	$query_others_res['subdivision_code'];
		$confirm_date_others		=	$query_others_res['confirm_date'];
		$dlc_user_id_others			=	$query_others_res['dlc_user_id'];
		$order_no_others			=	$query_others_res['id'].'-INSP/EL/LC/'.date("Y");
		$id_other					=	$query_others_res['id'];
		
		$officer_designation		=	2;
		//echo $id_other; exit;
		
		$query_others_pre			=	db_select('l_area_inspector_random_order_master','lairom');
		$query_others_pre			->	fields('lairom', array('id', 'office_name', 'office_address', 'order_no'));
		$query_others_pre			->	condition('lairom.distict_code', $district_code,'=');
		$query_others_pre			->	condition('lairom.subdivision_code', $subdivision_code,'=');
		$query_others_pre			->	condition('lairom.officer_designation', $officer_designation,'=');
		$query_others_pre			->	orderBy('id', 'DESC');
		$query_others_pre 			= 	$query_others_pre->extend('PagerDefault')->limit(1);
		$total_name_others_pre 		=  	$query_others_pre->execute();
		if($total_name_others_pre->rowCount() > 0){
			$query_others_res_pre		=	$total_name_others_pre->fetchAssoc();
			$office_name				=	$query_others_res_pre['office_name'];
			$office_address				=	$query_others_res_pre['office_address'];
			$order_no					=	$query_others_res_pre['order_no'];
			$notice_details				=	$query_others_res_pre['notice_details'];
		}
		
		
		
		
	}
	//echo $office_name."--".$office_address."--".$order_no; exit;
	
	
	$form['randomization_o_l']['sample_output']		=	array(
															'#prefix' 			=> 	'',
															'#suffix' 			=> 	'',
															'#type' 			=> 	'markup',
															'#markup'			=>	'<br/>'.l('Download Sample Randomization Order', $link, array('html' => TRUE,'attributes'=>array('target' => '_blank')))
														);
															
	if($district_code==17){
		
		$form['randomization_o_l']['id']			=	array(
															'#type' 			=> 	'hidden',
															'#default_value'	=>	!empty($id_kol) ? $id_kol : '',
															'#attributes' 		=> 	array('readonly'=>'readonly')
														);
														
		$form['randomization_o_l']['area_division_type'] =	array(
																'#type' 			=> 	'hidden',
																'#default_value'	=>	!empty($area_division_type) ? $area_division_type : '',
																'#attributes' 		=> 	array('readonly'=>'readonly')
															);
														
		$form['randomization_o_l']['district_code']	=	array(
															'#type' 			=> 	'hidden',
															'#default_value'	=>	!empty($distict_code_kol) ? $distict_code_kol : '',
															'#attributes' 		=> 	array('readonly'=>'readonly')
														);
														
		$form['randomization_o_l']['subdivision_code']	=	array(
															'#type' 			=> 	'hidden',
															'#default_value'	=>	!empty($subdivision_code_kol) ? $subdivision_code_kol : '',
															'#attributes' 		=> 	array('readonly'=>'readonly')
														);
														
		$form['randomization_o_l']['dlc_user_id']	=	array(
															'#type' 			=> 	'hidden',
															'#default_value'	=>	!empty($dlc_user_id_kol) ? $dlc_user_id_kol : '',
															'#attributes' 		=> 	array('readonly'=>'readonly')
														);
	}else{
		$form['randomization_o_l']['id']			=	array(
															'#type' 			=> 	'hidden',
															'#default_value'	=>	!empty($id_other) ? $id_other : '',
															'#attributes' 		=> 	array('readonly'=>'readonly')
														);
														
		$form['randomization_o_l']['area_division_type'] =	array(
																'#type' 			=> 	'hidden',
																'#default_value'	=>	!empty($area_division_type) ? $area_division_type : '',
																'#attributes' 		=> 	array('readonly'=>'readonly')
															);												
														
		$form['randomization_o_l']['district_code']	=	array(
															'#type' 			=> 	'hidden',
															'#default_value'	=>	!empty($distict_code_others) ? $distict_code_others : '',
															'#attributes' 		=> 	array('readonly'=>'readonly')
														);
														
		$form['randomization_o_l']['subdivision_code']	=	array(
															'#type' 			=> 	'hidden',
															'#default_value'	=>	!empty($subdivision_code_others) ? $subdivision_code_others : '',
															'#attributes' 		=> 	array('readonly'=>'readonly')
														);
														
		$form['randomization_o_l']['dlc_user_id']	=	array(
															'#type' 			=> 	'hidden',
															'#default_value'	=>	!empty($dlc_user_id_others) ? $dlc_user_id_others : '',
															'#attributes' 		=> 	array('readonly'=>'readonly')
														);
	}
	
	$form['randomization_o_l']['office_name']		=	array(
															'#prefix' 			=> 	'',
															'#suffix' 			=> 	'',
															'#type' 			=> 	'textfield',
															'#title'			=>	t('Name of the office'),
															'#required'			=>	TRUE,
															'#default_value'	=>	!empty($office_name) ? $office_name : 'Office of the Assistant Labour Commissioner'
														);
														
	$form['randomization_o_l']['office_address']	=	array(
															'#prefix' 			=> 	'',
															'#suffix' 			=> 	'',
															'#type' 			=> 	'textarea',
															'#title'			=>	t('Address of the Office'),
															'#required'			=>	TRUE,
															'#default_value'	=>	!empty($office_address) ? $office_address : 'New Secretariat Buildings, (11th Floor )  1, K.S.Roy Road: Kolkata – 700 001'
														);
	
	if($district_code == 17){
		$form['randomization_o_l']['order_no']			=	array(
															'#prefix' 			=> 	'',
															'#suffix' 			=> 	'',
															'#type' 			=> 	'textfield',
															'#title'			=>	t('Order Number'),
															'#required'			=>	TRUE,
															'#default_value'	=>  !empty($order_no_kol) ? $order_no_kol : '',
														);
														
		$form['randomization_o_l']['confirm_date']		=	array(
															'#prefix' 			=> 	'',
															'#suffix' 			=> 	'',
															'#type' 			=> 	'textfield',
															'#title'			=>	t('Order Date'),
															'#required'			=>	TRUE,
															'#attributes' 		=> 	array('id'=>'alc_date'),
															'#default_value'	=>  !empty($confirm_date_kol) ? $confirm_date_kol : '',
															);
	}else{
		$form['randomization_o_l']['order_no']			=	array(
															'#prefix' 			=> 	'',
															'#suffix' 			=> 	'',
															'#type' 			=> 	'textfield',
															'#title'			=>	t('Order Number'),
															'#required'			=>	TRUE,
															//'#default_value'	=>  !empty($order_no_others) ? $order_no_others : '',
															'#default_value'	=>  !empty($order_no) ? $order_no : '',
														);
														
		$form['randomization_o_l']['confirm_date']		=	array(
															'#prefix' 			=> 	'',
															'#suffix' 			=> 	'',
															'#type' 			=> 	'textfield',
															'#title'			=>	t('Order Date'),
															'#required'			=>	TRUE,
															'#attributes' 		=> 	array('id'=>'alc_date'),
															'#default_value'	=>  !empty($confirm_date_others) ? $confirm_date_others : '',
															);
	}
														
	$form['randomization_o_l']['notice_details']	=	array(
															'#prefix' 			=> 	'',
															'#suffix' 			=> 	'',
															'#type' 			=> 	'textarea',
															'#title'			=>	t('Notice Details'),
															'#required'			=>	TRUE,
															'#default_value'	=>  !empty($notice_details) ? $notice_details : 'The following Inspectors of Minimum Wages mentioned in the column number II have been allocated to inspect in the Blocks  mentioned in the column  number III of the following  table / Municipalities/Corporations/Development  Authorities/SEZ with the help of computerised randomization.The respective ALC will allocate the GP/GPs/Ward/Wards in the respective Block/Municipality/Corporation/Development Authority/SEZ, mentioning the period of inspection in due course through computerised randomization.'
															);
															
															
	if($district_code != 17 && $subdivision_code != 171001){
			$output		=	'<div style=" height:250px; overflow-y : auto;" align="center"><table border="1" width="100%">
								<tr><td style="border-bottom:1px solid #000; background-color:#c1c1c1;" colspan="6">CURRENT INSPECTOR RANDOMIZATION LIST. </td></tr>
								<tr>
									<td rowspan="2" style="border-bottom:1px solid #000; background-color:#c1c1c1;">SL.No.</td>
									<td rowspan="2" style="border-bottom:1px solid #000; background-color:#c1c1c1;">NAME</td>
									<td rowspan="2" style="border-bottom:1px solid #000; background-color:#c1c1c1;">AREA</td>									
									<td colspan="2" scope="col" style="border-bottom:1px solid #000; background-color:#c1c1c1;">SCHEDULED INSPECTION</td>
								</tr>
								<tr>
									<td style="border-bottom:1px solid #000; background-color:#c1c1c1;">FROM</td>
									<td style="border-bottom:1px solid #000; background-color:#c1c1c1;">TO</td>
							  	</tr>
								<tr>
									<td style="border-bottom:1px solid #000; background-color:#c1c1c1;">Column I</td>
									<td style="border-bottom:1px solid #000; background-color:#c1c1c1;">Column II</td>
									<td style="border-bottom:1px solid #000; background-color:#c1c1c1;">Column III</td>
									<td colspan="2" align="center" style="border-bottom:1px solid #000; background-color:#c1c1c1;">Column IV</td>								
							    </tr>';
			
			$new_queryz	= db_query('SELECT y.master_tag_id AS master_tag_id FROM l_area_inspector_random_tag y INNER JOIN l_custom_user_detail x ON x.usr_id = y.inspector_id 
WHERE y.is_confirm =:is_confirm AND x.created_by =:created_by', array(':is_confirm' => 'C', ':created_by' => trim($logged_in_id)));
			$results_1 = $new_queryz->fetchAssoc();
			$masters_tag_id = $results_1['master_tag_id'];
								
			$order					=	'inspector_name';
			$sort					=	'ASC';
			$show_details_query 	= 	db_select('l_area_inspector_random_tag', 't');
			$show_details_query		->	join('l_custom_user_detail', 'v', 'v.usr_id =  t.inspector_id');
			$show_details_query		->	fields('t', array('inspector_name','inspector_code','distict_code','block_municipality_code','inspector_id', 'id'));
			$show_details_query		->	condition('t.is_confirm','C','=');
			$show_details_query		->	condition('t.master_tag_id', trim($masters_tag_id), '=');
			$show_details_query		->	condition('v.created_by', trim($logged_in_id), '=');
			$show_details_query		->	orderBy($order, $sort);
			$show_details_query 	= 	$show_details_query->extend('PagerDefault')->limit(50);
			$total_name 			=  	$show_details_query->execute();  // print_r($total_name->fetchAll()); die;
			
			$i	=	0;
			if($total_name->rowCount() > 0){
				foreach($total_name as $data){
					
					$inspector_code 			= 	$data->inspector_id; 
					$inspector_name 			= 	$data->inspector_name;
					$area_code 					= 	$data->block_municipality_code;
					$district_code 				= 	$data->distict_code;
					$id							=	$data->id;
					
					// $query_ar_name				=	db_query('select area_name from l_inspector_random_master where  block_municipality_code =:wcode and distict_code =:dcode  ', array(':wcode' => trim($area_code) , ':dcode' => trim($district_code)));
					
					$query_ar_name				=	db_query('select block_mun_name from block_mun_master where  block_code =:wcode and distict_code =:dcode  ', array(':wcode' => trim($area_code) , ':dcode' => trim($district_code)));
			
					$result						=	$query_ar_name->fetchAssoc();
					$area_name 					= 	$result['block_mun_name'];
					
					$query_vill_name = db_query('select scheduling_status, village_name , scheduling_fdt , scheduling_tdt, category from l_gp_ward_inspector_tag a, village_ward_master b where a.block_code=b.block_code and a.village_code=b.village_code and inspector_id = :insid and confirm_s = :cst and masters_tag_id=:masters_tag_id', array(':insid' => trim($inspector_code) , ':cst' => 'C', ':masters_tag_id' => $masters_tag_id));
					$result2					=	$query_vill_name->fetchAssoc();
					if($result2['category']=="W"){
						$village_name 				= 	"Ward-".$result2['village_name'];	
					}else{
						$village_name 				= 	$result2['village_name'];
					}
					 
					$scheduling_status 			= 	$result2['scheduling_status']; 
					$scheduling_fdt				=	$result2['scheduling_fdt'];
					$scheduling_tdt				=	$result2['scheduling_tdt'];
				
				if($scheduling_fdt != "" && $scheduling_status!="" && $area_division_type!=""){ 
					$i++;
					$output 				.=	'<tr><td style="border-bottom:1px solid #000;">'.$i.'</td>';
					$output 				.=	'<td style="border-bottom:1px solid #000;border-left:1px solid #000;">'.$inspector_name.'</td>';
					$output 				.=	'<td style="border-bottom:1px solid #000;">'.$village_name.'<br>['.$area_name.']</td>';
					$output 				.=	'<td style="border-bottom:1px solid #000;">'.date('d-m-Y', strtotime($scheduling_fdt)).'</td>';
					$output 				.=	'<td style="border-bottom:1px solid #000;">'. date('d-m-Y', strtotime($scheduling_tdt)).'</td>';
					// $output 				.=	'<td style="border-bottom:1px solid #000;">'.$village_name.'</td></tr>';
				}
			}
		}
		$output .='</table></div>';
		$form['randomization_o_l']['random_prev_result'] = array(
																	'#prefix' => '<div id="randomprevshow">'.$output,
																	'#suffix' => '</div>',
																);
	}
															
	$designation_options								=	array( 1 => 'Deputy Labour Commissioner', 2 => 'Assistant labour Commisioner' );
														
	$form['randomization_o_l']['officer_designation']	=	array(
																'#prefix' 		=> 	'',
																'#suffix' 		=> 	'',
																'#type' 		=> 	'hidden',
																'#title'		=>	t('Designation of the Officer'),
																'#required'		=>	TRUE,
																'#default_value'=>  2
															);
															
	$query_posting		=	db_query('select user_place from l_custom_user_detail where usr_id=:usr_id', array(':usr_id' => $logged_in_id));
	$query_posting_res	=	$query_posting->fetchAssoc();
	$alc_posting		=	$query_posting_res['user_place'];
																
	$form['randomization_o_l']['posting_area']			=	array(
																'#prefix' 		=> 	'',
																'#suffix' 		=> 	'',
																'#type' 		=> 	'hidden',
																'#title'		=>	t('Posting Area'),
																'#default_value'=> !empty($alc_posting) ? $alc_posting : '',
														);
																												
	$options 										= 	array( 7 => 'LC', 6 => 'ADLC', 4 => 'DM', 5 => 'JLC', 1 => t('DLC'), 2 => t('BDOs'), 3 => 'SDO' );

	$form['randomization_o_l']['copy_info'] 		= 	array(
														  '#title'				=>	'Copy Information To',
														  '#type' 				=> 	'checkboxes',
														  '#options' 			=> 	$options,
														  '#prefix' 			=> 	'<div id="COPY">',
														  '#suffix' 			=> 	'</div>',
														  '#attributes'			=>	array('id' => 'copy_info_id', 'class' => array('copy_info_class'), 'onclick' => 'return copy_info_fun();')
														);
	
	$form['randomization_o_l']['alc_textfield'] 	= 	array(
															'#prefix' 			=> 	'<div id="DLC" style="display:none;">',
															'#suffix' 			=> 	'</div>',
															'#title'			=>	'Name of DLCs <span class="star_red">*</span>',
															'#type' 			=> 	'textfield',
															'#attributes'		=>	array('placeholder' => 'To write multiple DLCs ,use comma separated names')
														);
														
	$form['randomization_o_l']['bdo_textfield'] 	= 	array(
															'#prefix' 			=> 	'<div id="BDO" style="display:none;">',
															'#suffix' 			=> 	'</div>',
															'#title'			=>	'Name of BDOs <span class="star_red">*</span>',
															'#type' 			=> 	'textfield',
															'#attributes'		=>	array('placeholder' => 'To write multiple BDOs ,use comma separated names')
														);
														
	$form['randomization_o_l']['sdo_textfield'] 	= 	array(
															'#prefix' 			=> 	'<div id="SDO" style="display:none;">',
															'#suffix' 			=> 	'</div>',
															'#title'			=>	'Name of SDOs <span class="star_red">*</span>',
															'#type' 			=> 	'textfield',
															'#attributes'		=>	array('placeholder' => 'To write multiple SDOs ,use comma separated names')
														);
														
	$form['randomization_o_l']['dm_textfield'] 	= 	array(
															'#prefix' 			=> 	'<div id="DM" style="display:none;">',
															'#suffix' 			=> 	'</div>',
															'#title'			=>	'Name of DMs <span class="star_red">*</span>',
															'#type' 			=> 	'textfield',
															'#attributes'		=>	array('placeholder' => 'To write multiple DMs ,use comma separated names')
														);
														
	$form['randomization_o_l']['jlc_textfield'] 	= 	array(
															'#prefix' 			=> 	'<div id="JLC" style="display:none;">',
															'#suffix' 			=> 	'</div>',
															'#title'			=>	'Name of JLC <span class="star_red">*</span>',
															'#type' 			=> 	'textfield',
															'#attributes'		=>	array('placeholder' => 'To write multiple JLCs ,use comma separated names')
														);
														
	$form['randomization_o_l']['adlc_textfield'] 	= 	array(
															'#prefix' 			=> 	'<div id="ADLC" style="display:none;">',
															'#suffix' 			=> 	'</div>',
															'#title'			=>	'Name of ADLC <span class="star_red">*</span>',
															'#type' 			=> 	'textfield',
															'#attributes'		=>	array('placeholder' => 'To write multiple ADLCs ,use comma separated names')
														);
														
	$form['randomization_o_l']['lc_textfield'] 	= 	array(
															'#prefix' 			=> 	'<div id="LC" style="display:none;">',
															'#suffix' 			=> 	'</div>',
															'#title'			=>	'Name of LC <span class="star_red">*</span>',
															'#type' 			=> 	'textfield',
															'#attributes'		=>	array('placeholder' => 'To write multiple LCs ,use comma separated names')
														);
																												
	$form['randomization_o_l']['submit']			=	array(
															'#prefix' 			=> 	'<div align="center">',  
                                                            '#suffix' 			=> 	'</div>',
															'#type' 			=> 	'submit',
                                                            '#default_value' 	=> 	'Submit'
														);
	return $form;
}

function order_alc_form_validate($form, &$form_state){
	$val					=	$form_state['values'];
	$copy_info				=	$val['copy_info'];
	$alc_textfield			=	$val['alc_textfield'];
	$bdo_textfield			=	$val['bdo_textfield'];
	$sdo_textfield			=	$val['sdo_textfield'];
	$dm_textfield			=	$val['dm_textfield'];
	$jlc_textfield			=	$val['jlc_textfield'];
	$adlc_textfield			=	$val['adlc_textfield'];
	$lc_textfield			=	$val['lc_textfield'];
	
	/*if( !empty($copy_info) ){
		foreach($copy_info as $ValidValues){
			if (empty($ValidValues)) {
				continue;
			}
			if($ValidValues==1){
				if(empty($alc_textfield) || $alc_textfield == ""){
					form_error($form['randomization_o_l']['alc_textfield'], t('Name of ALC is required'));
				}
			}
			if($ValidValues==2){
				if(empty($bdo_textfield) || $bdo_textfield == ""){
					form_error($form['randomization_o_l']['bdo_textfield'], t('Name of BDO is required'));
				}
			}
			if($ValidValues==3){
				if(empty($sdo_textfield) || $sdo_textfield == ""){
					form_error($form['randomization_o_l']['sdo_textfield'], t('Name of SDO is required'));
				}
			}
			if($ValidValues==4){
				if(empty($dm_textfield) || $dm_textfield == ""){
					form_error($form['randomization_o_l']['dm_textfield'], t('Name of DM is required'));
				}
			}
			if($ValidValues==5){
				if(empty($jlc_textfield) || $jlc_textfield == ""){
					form_error($form['randomization_o_l']['jlc_textfield'], t('Name of JLC is required'));
				}
			}
			if($ValidValues==6){
				if(empty($adlc_textfield) || $adlc_textfield == ""){
					form_error($form['randomization_o_l']['adlc_textfield'], t('Name of ADLC is required'));
				}
			}
			if($ValidValues==7){
				if(empty($lc_textfield) || $lc_textfield == ""){
					form_error($form['randomization_o_l']['lc_textfield'], t('Name of LC is required'));
				}
			}
		}
	}*/
	//$form_state['rebuild'] = TRUE;
}

function order_alc_form_submit($form, &$form_state){
	$val					=	$form_state['values'];
	//print_r($val); exit;
	$increment 				= 	0;
	
	$id						=	trim($val['id']);
	$area_division_type		=	$val['area_division_type'];
	$dlc_user_id			=	trim($val['dlc_user_id']);
	$district_code			=	trim($val['district_code']);
	$subdivision_code		=	trim($val['subdivision_code']);
	$office_name			=	$val['office_name'];
	$office_address			=	$val['office_address'];
	$order_no				=	trim($val['order_no']);
	$confirm_date			=	$val['confirm_date'];
	$notice_details			=	$val['notice_details'];
	$officer_designation	=	$val['officer_designation'];
	$posting_area			=	$val['posting_area'];
	$copy_info				=	$val['copy_info'];
	
	
	$alc_textfield			=	$val['alc_textfield'];
	$bdo_textfield			=	$val['bdo_textfield'];
	$sdo_textfield			=	$val['sdo_textfield'];
	$dm_textfield			=	$val['dm_textfield'];
	$jlc_textfield			=	$val['jlc_textfield'];
	$adlc_textfield			=	$val['adlc_textfield'];
	$lc_textfield			=	$val['lc_textfield'];
	
	$orderFields			=	array(
									'tag_map_id'			=>	$id,
									'dlc_user_id'			=>	$dlc_user_id,
									'distict_code'			=>	$district_code,
									'subdivision_code'		=>	$subdivision_code,
									'office_name'			=>	$office_name,
									'office_address'		=>	$office_address,
									'order_no'				=>	$order_no,
									'confirm_date'			=>	$confirm_date,
									'notice_details'		=>	$notice_details,
									'officer_designation'	=>	$officer_designation,
									'posting_area'			=>	$posting_area,
									'alc_textfield'			=>	$alc_textfield,
									'bdo_textfield'			=>	$bdo_textfield,
									'sdo_textfield'			=>	$sdo_textfield,
									'dm_textfield'			=>	$dm_textfield,
									'jlc_textfield'			=>	$jlc_textfield,
									'adlc_textfield'		=>	$adlc_textfield,
									'lc_textfield'			=>	$lc_textfield
								);
	
	//echo $dlc_user_id."--".$distict_code."--".$subdivision_code."--".$order_no."--".$confirm_date."--".$id; exit;
	if( !empty($dlc_user_id) && !empty($district_code) && !empty($subdivision_code) && !empty($order_no) && !empty($confirm_date) && !empty($id) ){
		$last_id 			= 	db_insert('l_area_inspector_random_order_master')->fields($orderFields)->execute();
		
		if(!empty($copy_info)){
			foreach($copy_info as $LoopValue){
				if (empty($LoopValue)) {
    				continue;
				}
				$copyInfoFields 							= 	array( 
																	'order_master_map_id'		=>	$last_id,
																	'copy_info'					=>	$LoopValue
																);
																					
				db_insert('l_area_inspector_random_order_cpyinfo')->fields($copyInfoFields)->execute();
				$increment++;
			}
		}
		/*$action						=	'encrypt';
		drupal_goto('generate-alc-randomization-order/order-alc-upload/'.encryption_decryption_fun($action, $last_id).'/'.encryption_decryption_fun($action, $district_code).'/'.encryption_decryption_fun($action, $subdivision_code));*/
		drupal_goto('generate-alc-randomization-order/order-alc-upload/'.$last_id.'/'.$district_code.'/'.$subdivision_code);
	}
}