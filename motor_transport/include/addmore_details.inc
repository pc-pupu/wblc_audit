<?php
drupal_add_js(drupal_get_path('module','motor_transport').'/js/mtw_upload_custom.js');
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
function addmore_details($form, &$form_state, $id=0, $p_type='', $p_id=0){
	global $base_root,$base_path,$user;
	
	$user_id                  	= $user->uid;
	$path 		= current_path();
	$uriArr		= explode('/',$path);
	$type 		= $uriArr[1];
	
	$application_id 									= encryption_decryption_fun('decrypt', $id);
	//$p_id												= encryption_decryption_fun('encrypt', $p_id);
	$p_type												= encryption_decryption_fun('decrypt', $p_type);
	
	//print_r($uriArr);
	
	// label-title variables
	$label												= 'Add '.ucwords($p_type);
	$btn_lbl											= 'Save & Continue';
	$person_name										= 'Name of '.ucwords($p_type);
	$designation										= $p_type;
	
	$person_lb2											= '';
	
	// data variables
	$person_name_old									= '';
	$person_address_old									= '';
	
	$person_country_old									= 0;
	$person_state_old									= '';
	
	$person_district_old								= 0;
	$person_subdivision_old								= 0;
	$person_areatype_old								= '';
	$person_areatype_code_old							= 0;
	$person_vill_ward_old								= '';
	$person_ps_old										= '';
	$person_pincode_old									= 0;
	$company_name_old									= '';
	
	
	$query 		 = db_select('l_mtw_remark_master', 'lbkrm')
						 ->	fields('lbkrm', array('remark_field_title'))
						 ->	condition('lbkrm.remark_to', trim($user_id))
						 ->	condition('lbkrm.application_id', $application_id)
						 ->	orderBy('lbkrm.id','DESC');
						 
	$remark_field_text_query = $query->execute();
	$remark_field_text_array = $remark_field_text_query->fetchAssoc();
	
	
	
	$verified_fields = explode(',', $remark_field_text_array['remark_field_title']);
	
	$key = 'owner_id_'.$p_id;
	
	$readonly_person_name 							= array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Name');
	$readonly_person_directors_compname 			= array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Company Name');
	$readonly_person_loc 							= array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Location');
	$readonly_person_district 						= array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select District');
	
	$readonly_person_subdiv 						= array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Sub-Division','onkeyup' => 'getSpanText()', 'onblur' => 'getSpanText()', 'onchange' => 'getSpanText()');
	$readonly_person_areatype 						= array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Block / Municipality / Corporation / SEZ / Notified Area');
	
	$readonly_person_areatype_code					= array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Gram Panchayat/Ward');
	$readonly_person_vill_ward						= array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Gram Panchayat/Ward/Sector');
	
	
	$readonly_person_ps								= array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Police Station');
	$readonly_person_pincode						= array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Pin Code','maxlength' => 6);
	
	$readonly_person_designation	= array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'','maxlength' => 6);
	
	if(in_array($key,$verified_fields)){
		$readonly_person_name 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));
		$readonly_person_designation 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
		$readonly_person_directors_compname 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
		$readonly_person_loc 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));
		$readonly_person_district 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
		$readonly_person_subdiv 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
		$readonly_person_areatype 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
		$readonly_person_areatype_code					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
		$readonly_person_vill_ward						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
		$readonly_person_ps								= array('disabled' => 'disabled', 'class' => array('readonly_view'));
		$readonly_person_pincode						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
	}					
		
	if($type=="edit"){
		
		$label											= 'Edit';
		$btn_lbl										= 'Update Info';
		
		
		$getallPersonqr 									= db_select('l_mtw_registration_master_extra', 'lmrme');
		$getallPersonqr->fields('lmrme', array());
		$getallPersonqr->condition('lmrme.id', $p_id); 
		$getallPersonqr->condition('lmrme.application_id', $application_id);
		$getallPersonExc 									= $getallPersonqr->execute(); 
				 
		$getallPersonResult									= $getallPersonExc->fetchObject();
		
		$designation									= $getallPersonResult->designation;
		$person_name_old								= $getallPersonResult->person_name;
		$person_address_old								= $getallPersonResult->person_address;
		
		$person_country_old								= $getallPersonResult->person_country;
		$person_state_old								= $getallPersonResult->person_state;
		
		$person_district_old							= $getallPersonResult->person_district;
		$person_subdivision_old							= $getallPersonResult->person_subdivision;
		$person_areatype_old							= $getallPersonResult->person_areatype;
		$person_areatype_code_old						= $getallPersonResult->person_areatype_code;
		$person_vill_ward_old							= $getallPersonResult->person_vill_ward;
		$person_ps_old									= $getallPersonResult->person_ps;
		$person_pincode_old								= $getallPersonResult->person_pincode;
		$company_name_old								= $getallPersonResult->company_name;
		
		$designationArray2 = array(
								'director'			=> "Director of the Company",
								'proprietor'		=> "Proprietor of the Firm",
								'partner'			=> "Partner of the Firm",
								'general_manager'	=> "General Manager of Public Sector Undertaking",
								'other'				=> "Other"
							);
		
		$desArry2			= array('director','proprietor','partner','general_manager','other');
		
		if(!in_array($designation,$desArry2)){
			$designation	= 'other';
		}/*else{
			$designationArray	= array(
									$designation => $designationArray2[$designation],
									'other'		 => 'Other'
									);
		}*/
		
		
	}
	$designationArray							=	array(''=>'-Select-');
	if($designation == "proprietor") {
		$person_name								= "Name of the ".$designation." in case of firm not registered under the Companies Act,1956";
		$fieldCompany								=	"Name of The Firm";
		$designationArray['proprietor']				=	"Proprietor of the Firm";
	}else if($designation == "partner"){
		$person_name								= "Name of the General Manager in the case or a public sector undertaking";
		$fieldCompany								=	"Name of The Firm";
		$designationArray['partner']				=	"Partner of the Firm";
	}else if($designation =="general_manager"){
		$person_name								= "Name of the General Manager in the case or a public sector undertaking";
		$fieldCompany								=	"Name of The PSU";
		$designationArray['general_manager']		=	"General Manager of Public Sector Undertaking";
	}else if($designation =="director"){
		$person_name								= "Name of the Director in case of company registered under the Companies Act,1956";
		$fieldCompany								=	"Name of The Company";
		$designationArray['director']				=	"Director of the Company";
	}else{
		$person_name								= "Name of the Person";
		$fieldCompany								=	"Name of The Organization";
		$designationArray							= array(
														''					=> "-Select-",
														'director'			=> "Director of the Company",
														'proprietor'		=> "Proprietor of the Firm",
														'partner'			=> "Partner of the Firm",
														'general manager'	=> "General Manager of Public Sector Undertaking",
														);
	}
	$designationArray['other']		=	"Other";
	
	$form['form_begin'] = array(
		'#type' 			=> 'markup',
		'#markup' 			=> '<div class="panel panel-default panel-form">
							<div class="panel-heading">'.$label.' Ownership Details</div>
							<div class="panel-body">',
	);
	
	$form['application_id'] = array(
		'#title' 			=> '',
		'#type' 			=> 'hidden',
		'#value'			=> $application_id,
	);
	$form['person_id'] 								= array(
														'#title' 			=> '',
														'#type' 			=> 'hidden',
														'#value' 			=> $p_id															
														);
	
	
	$form['designation']							= array(
														'#title'			=> 'Designation',
														'#required'			=> TRUE,
														'#type'				=> 'select',
														'#options'			=> $designationArray,
														'#default_value'	=> $designation,
														'#attributes' 		=> $readonly_person_designation,
														'#id'				=> 'desgVal',
														'#prefix' 			=> '<div class="col-md-6"><label class="select">',
														'#suffix' 			=> '</label></div>',
														'#ajax'				=> array(
																			'event'		=> 'change',
																			'callback'	=> 'get_otherdesignation_ajax_callback',
																			'effect'	=> 'fade',
																				)
															); /*<h1 class="inner-page-title">'.$label.' DETAILS</h1>*/
	$designationVal									= isset($form_state['values']['designation']) ? $form_state['values']['designation'] : $designation;
	
	$form['designation_other']						= array(
														'#prefix' 			=> '<div id="designation_other_div">',
														'#suffix' 			=> '</div>'
															
															);
	
	$form['mtw_directors_compname']					= array(
														'#prefix' 			=> '<div id="company_div">',
														'#suffix' 			=> '</div>'
															
															);
	
	$form['mtw_directorsname']						= array(
														'#prefix'			=> '<div id="name_div">',
														'#suffix'			=> '</div>'
															
															);
	
	if(!empty($designationVal) && $designationVal!=''){
		if($designationVal=='director'){
			$fieldCompany		=	"Name of The Company";
			$person_lb2			=	"Director";
		}elseif($designationVal=='proprietor'){
			$fieldCompany		=	"Name of The Firm";
			$person_lb2			=	"Proprietor";
		}elseif($designationVal=='partner'){
			$fieldCompany		=	"Name of The Firm";
			$person_lb2			=	"Partner";
		}elseif($designationVal=='general_manager'){
			$fieldCompany		=	"Name of The PSU";
			$person_lb2			=	"General Manager";
		}else{
			$fieldCompany		=	"Name of The Organization";
			$person_lb2			=	"Person";
		}
		
		$person_name								= 'Name of '.ucwords($person_lb2);
		
		if($designationVal=='other'){
			$form['designation_other']				= array(
														'#prefix' 			=> '<div id="designation_other_div"><section class="col-md-6"><label class="input">',
														'#suffix' 			=> '</label></section></div>',
														'#type'				=> 'textfield',
														'#title'			=> t('Other Designation'),
														'#default_value'	=> $getallPersonResult->designation,
														'#attributes' 		=> $readonly_person_designation,
															
															);
		}
		
		
		$form['mtw_directors_compname'] 			= array(
														'#title' 			=> $fieldCompany,
														'#type' 			=> 'textfield',
														'#attributes' 		=> $readonly_person_directors_compname,
														'#default_value'	=> $company_name_old,
														'#prefix' 			=> '<div id="company_div"><section class="col-md-6"><label class="input">',
														'#suffix' 			=> '</label></section></div>'
															);
		$form['mtw_directorsname'] 					= array(
														'#title' 			=> $person_name,
														'#type' 			=> 'textfield',
														'#attributes' 		=> $readonly_person_name,
														'#required' 		=>  TRUE,
														'#default_value'	=> $person_name_old,
														'#prefix' 			=> '<div id="name_div"><section class="col-md-6"><label id="label_replace" class="input">',
														'#suffix' 			=> '</label></section></div>'
															);
		
	}														
	
	
	$form['mtw_directorsloc']						= array(
														'#prefix' 			=>	'<section class="col-md-6"><label id="label_replace" class="input">',
														'#suffix' 			=>	'</label></section>',
														'#title'			=>	t('Address Line1 (if other Country/State please provide detail address)'),
														'#default_value'	=>	$person_address_old,
														'#type'				=>	'textfield',
														'#required' 		=> TRUE,
														'#attributes' 		=> $readonly_person_loc
													);
	
	$options_country 	= array(''=>'-Select-', 1 => 'India', 2 => 'Others'); 
	// country block start 	
	$form['mtw_country'] 							= array(
														'#prefix' 			=> '<section class="col-md-6"><label id="label_replace" class="select">',
														'#suffix' 			=> '</label></section>',
														'#title'			=> 'Country',
														'#type' 			=> 'select',
														'#options' 			=> $options_country,
														'#default_value'	=> $person_country_old,
														'#attributes' 		=> $readonly_person_loc,
														'#required' 		=> TRUE,
														'#ajax'				=> array(
																					'event' 		=> 'change',
																					'effect' 		=> 'fade',			
																					'callback'		=> 'get_info_form_ajax_callback',
																					'progress'		=>	array('type' => 'throbber','message' => t('Loading State...')),
																				)
													);
	// country block end
	// Default block start  									
	$value_country 									= isset($form_state['values']['mtw_country']) ? $form_state['values']['mtw_country'] : $person_country_old;
	
	$form['mtw_state']								= array(
														'#prefix' 			=> '<div id="country_others_div">',
														'#suffix' 			=> '</div>',
													);
	
	$value_state									= 0;
	$form['mtw_directorsdistrict']					= array(
														'#prefix' 			=> '<div id="distric_div">',
														'#suffix' 			=> '</div>',
													);
	$form['mtw_directorssubdivision']				= array(
														'#prefix' 			=> '<div id="subdivisn_div">',
														'#suffix' 			=> '</div>',
													);
	$form['mtw_directorsareatype']					= array(
														'#prefix' 			=> '<div id="areatype_div">',
														'#suffix' 			=> '</div>',
													);
	$form['mtw_directorsblock']						= array(
														'#prefix' 			=> '<div id="block_mun_div">',
														'#suffix' 			=> '</div>',
													);
	$form['mtw_directorspanchyat']					= array(
														'#prefix' 			=> '<div id="gp_ward_div">',
														'#suffix' 			=> '</div>',
													);
	$form['mtw_directorspolicestation']				= array(
														'#prefix' 			=> '<div id="ps_div">',
														'#suffix' 			=> '</div>',
													);
	$form['mtw_directorspincode']					= array(
														'#prefix' 			=> '<div id="pincode_div">',
														'#suffix' 			=> '</div>',
													);
	
	
	
	// Default block end
 	if(!empty($value_country) || $value_country !=''){
		$db_state_value								= db_query("SELECT COUNT(id) FROM {state_master} WHERE country_id = ".$value_country."")->fetchObject();
	}else{
		$db_state_value								= array();
	}
	if(!empty($value_country) && !empty($db_state_value) && ($db_state_value->count>0)){
		// india section
			/***************************** State block start **********************/
		$options_state								= get_all_state1($value_country);
		$form['mtw_state']							= array(
														'#prefix' 			=> '<div id="country_others_div"><section class="col-md-6"><label class="select">',
														'#suffix' 			=> '</label></section></div>',
														'#title'			=> t('Select State'),
														'#type' 			=> 'select',
														'#options' 			=> $options_state,
														'#default_value'	=> $person_state_old,
														'#required' 		=> TRUE,
														'#attributes' 		=> $readonly_person_loc,
														'#ajax'				=> array(
																					'event'		=> 'change',
																					'effect' 	=> 'fade',
																					'callback'	=> 'get_info_form_ajax_callback',
																				)
													);
			/***************************** District block start **********************/
		$value_state								= isset($form_state['values']['mtw_state'])?$form_state['values']['mtw_state'] : $person_state_old;
		$options_dist								= array(''=>'-Select-');																  
		
		if(($value_country!='' && (!empty($value_state) || $value_state!=0 || $value_state!=''))){
			$db_district_value							= db_query("SELECT COUNT(id) FROM {district_master} WHERE state_id =".$value_state." AND is_active='Y' ")->fetchObject();
			if(($db_district_value->count>0) && (!empty($value_state) || $value_state!=0)){
				
				$form['mtw_directorsdistrict']		= array(
														'#prefix' 			=> '<div id="distric_div"><section class="col-md-6"><label id="label_replace" class="select">',
														'#suffix' 			=> '</label></section></div>',
														'#title'			=> t('Select District'),
														'#type' 			=> 'select',
														'#options' 			=> get_discrict_by_id($value_state),
														'#default_value'	=> $person_district_old,
														'#attributes' 		=> $readonly_person_district,
														'#required' 		=> TRUE,
														'#ajax'				=> array(
																					'event'		=> 'change',
																					'effect' 	=> 'fade',
																					'callback'	=> 'get_info_form_ajax_callback',
																				)
														);
														
				/***************************** Sub Division block start **********************/
				$value_dis								= isset($form_state['values']['mtw_directorsdistrict'])?$form_state['values']['mtw_directorsdistrict'] : $person_district_old;
				
				if(!empty($value_dis) || $value_dis!='' || $value_dis!=0){
					$db_subdivision_value					= db_query("SELECT COUNT(id) FROM {sub_division} WHERE district_code ='".$value_dis."' AND is_active='Y' ")->fetchObject();
				}else{
					$db_subdivision_value					= 0;
				}
				
				
				
				if($db_subdivision_value->count>0){				
					if(($value_state!='' || $value_state!=0) && ($value_dis!="" || !empty($value_dis) || $value_dis!=0)){
						$form['mtw_directorssubdivision']	= array(
																'#prefix' 			=> '<div id="subdivisn_div"><section class="col-md-6"><label id="label_replace" class="select">',
																'#suffix' 			=> '</label></section></div>',
																'#title'			=> t('Select Sub-Division'),
																'#type' 			=> 'select',
																'#options' 			=> common_custom_user_subdivisionlist_list_new($value_dis),
																'#default_value'	=> $person_subdivision_old,
																'#attributes' 		=> $readonly_person_subdiv,
																'#required' 		=> TRUE,
																'#ajax'				=> array(
																							'event'		=> 'change',
																							'effect' 	=> 'fade',
																							'callback'	=> 'get_info_form_ajax_callback',
																						)
															);
					}
					/***************************** Area Type block start **********************/				
					
					$value_subdiv						= isset($form_state['values']['mtw_directorssubdivision']) ? $form_state['values']['mtw_directorssubdivision'] : $person_subdivision_old;
					if(($value_dis!='' || !empty($value_dis) || $value_dis!=0) && (!empty($value_subdiv) || $value_subdiv!='' || $value_subdiv!=0)){
						$db_areatype_value					= db_query("SELECT COUNT(id) FROM {block_mun_master} WHERE distict_code ='".$value_dis."' AND sub_division_code=".$value_subdiv." AND is_active='Y' ")->fetchObject(); 
					}else{
						$db_areatype_value					= 0;
					}
					
					
					if($db_areatype_value->count>0){
					
						$areaTypeData 							= array();
						if($value_dis != "" && $value_subdiv != ""){	
							$areatypeOptions 					= common_custom_user_areatype_list_new($value_dis, $value_subdiv);
							if(empty($areatypeOptions)){
								$areatypeOptions	= array();
							}
							
							$form['mtw_directorsareatype'] 		= array(
																		'#prefix' 			=> '<div id="areatype_div"><section class="col-md-6"><label id="label_replace" class="select">',
																		'#suffix' 			=> '</label></section></div>',
																		'#title'			=> 'Select Block / Municipality / Corporation / SEZ / Notified Area',
																		'#type' 			=> 'select',
																		'#options' 			=> $areatypeOptions,
																		'#default_value'	=> $person_areatype_old,
																		'#attributes' 		=> $readonly_person_areatype,
																		'#required' 		=> TRUE,
																		'#attributes' 		=> array('autocomplete' => 'off', 'placeholder'=>'Select Area Type'),
																		'#ajax'				=> array(
																								'event' 	=> 'change',
																								'effect' 	=> 'fade',			
																								'callback' 	=> 'get_info_form_ajax_callback',																								
																							)	
																);
							}else {
							$form['mtw_directorsareatype']		= array(
																		'#prefix' 			=> '<div id="areatype_div"><section class="col-md-6"><label id="label_replace" class="select">',
																		'#suffix' 			=> '</label></section></div>',
																		'#title'			=> 'Select Block / Municipality / Corporation / SEZ / Notified Area',
																		'#type' 			=> 'select',
																		'#options' 			=> $areaTypeData,
																		'#default_value'	=> $person_areatype_old,
																		'#attributes' 		=> $readonly_person_areatype,
																		'#required' 		=> TRUE,
																		'#attributes' 		=> array('autocomplete' => 'off', 'placeholder'=>'Select Area Type'),
																		'#ajax'				=> array(
																								'event' 	=> 'change',
																								'effect' 	=> 'fade',			
																								'callback' 	=> 'get_info_form_ajax_callback',
																								'progress'	=>	array('type' => 'throbber','message' => 'Loading...'),
																							)	
																);
						}
						
						
				/***************************** Block/Municiple block start **********************/
					$value_areatype 							= isset($form_state['values']['mtw_directorsareatype']) ? $form_state['values']['mtw_directorsareatype'] : $person_areatype_old;
		
					$blockData 									= array();
					$blockData[] 								= 'Select';	
					
					$form['mtw_directorsblock'] 				= array(
																		'#title'			=> 'Select Block',
																		'#type' 			=> 'select',
																		'#options' 			=> $blockData,
																		'#default_value'	=> $person_areatype_code_old,
																		'#attributes' 		=> $readonly_person_areatype_code,
																		'#required' 		=> TRUE,
																		'#attributes' 		=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Block / Municipality / Corporation / SEZ / Notified Area'),
																		'#ajax'				=> array(
																								'event' 	=> 'change',
																								'effect' 	=> 'fade',			
																								'callback' 	=> 'get_info_form_ajax_callback',
																								'progress'	=>	array('type' => 'throbber','message' => 'Loading...'),
																								),	
																		'#prefix' 			=> '<div id="block_mun_div"><section class="col-md-6"><label id="label_replace" class="select">',
																		'#suffix' 			=> '</label></section></div>'
														);
				
					
						if($value_dis != 0 && $value_subdiv != 0){
									if($value_areatype != ""){
										if ( $value_areatype == 'B' ){
											$areatype_name	= 'Block';
										}elseif( $value_areatype == 'M'){
											$areatype_name	= 'Municipality';
										}elseif( $value_areatype == 'C'){
											$areatype_name	= 'Corporation';
										}elseif( $value_areatype == 'S'){
											$areatype_name	= 'SEZ';
										}elseif($value_areatype == 'N'){
											$areatype_name	= 'Notified Area';
										}
										
										$form['mtw_directorsblock'] 	= array(
																		'#title'			=> 'Name of '.$areatype_name,
																		'#type' 			=> 'select',
																		'#options' 			=> common_custom_user_block_list_new($value_subdiv,$value_areatype),
																		'#default_value'	=> $person_areatype_code_old,
																		'#required' 		=>  TRUE,
																		//'#attributes' 	=> $readonly_person_areatype_code,
																		'#attributes' 		=> $readonly_person_areatype_code,
																		'#ajax'				=> array(
																								'event' 	=> 'change',
																								'effect' 	=> 'fade',			
																								'callback' 	=> 'get_info_form_ajax_callback',
																								'progress'	=>	array('type' => 'throbber','message' => 'Loading...'),
																								),	
																		'#prefix' 			=> '<div id="block_mun_div"><section class="col-md-6"><label id="label_replace" class="select">',
																		'#suffix' 			=> '</label></section></div>'
																								);
									}
							}	
							
				$value_block_mun 					= isset($form_state['values']['mtw_directorsblock']) ? $form_state['values']['mtw_directorsblock'] : $person_areatype_code_old;
				if(!empty($value_block_mun) || $value_block_mun!='' || $value_block_mun!=0){
					$db_vil_ward						= db_query("Select COUNT(id) FROM {village_ward_master} WHERE block_code=".$value_block_mun." ")->fetchObject();
				}else{
					$db_vil_ward						= 0;
				}			
				
				if($db_vil_ward->count>0){
							
					/***************************** GP/Ward block start **********************/
					
					$panchyatData 						= array();
					$panchyatData[] 					= 'Select';	
					
					$form['mtw_directorspanchyat'] 		= array(
																		'#title' 			=> 'Select Gram Panchayat/Ward',
																		'#type' 			=> 'select',
																		'#options' 			=> $panchyatData,
																		'#default_value'	=> $person_vill_ward_old,
																		'#required' 		=>  TRUE,
																		//'#attributes' 	=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Gram Panchayat/Ward'),
																		'#attributes' 		=> $readonly_person_vill_ward,
																		'#ajax'				=> array(
																								'event' 	=> 'change',
																								'effect' 	=> 'fade',			
																								'callback' 	=> 'get_info_form_ajax_callback',
																								'progress'	=>	array('type' => 'throbber','message' => 'Loading...'),
																							  	),	
																		'#prefix' 			=> '<div id="gp_ward_div"><section class="col-md-6"><label id="label_replace" class="select">',
																		'#suffix' 			=> '</label></section></div>'
														);
					
					if($value_dis != "" && $value_subdiv != ""){
							if($value_areatype != ""){
								if($value_block_mun != ""){
									if($value_areatype == 'B'){
										$titleLable = 'Gram Panchayat Name';
										$funName	= common_custom_user_villward_list_new($value_block_mun,'V');
									}else if($value_areatype == 'M'){
										$titleLable = 'Ward Number';
										$funName	= common_custom_user_villward_list_new($value_block_mun,'W');
									}else if($value_areatype == 'C'){
										$titleLable = 'Ward Number';
										$funName	= common_custom_user_corporation_list_new($value_block_mun);
									}else if($value_areatype == 'S'){
										$titleLable = 'Sector';
										$funName	= common_custom_user_villward_list_new($value_block_mun,'S');
									}else if($value_areatype == 'N'){
										$titleLable = 'Notified Area';
										$funName	= common_custom_user_villward_list_new($value_block_mun,'N');
									}
									
									
									$form['mtw_directorspanchyat'] = array(
																		'#type' 			=> 'select',
																		'#title' 			=> $titleLable,
																		'#options' 			=> $funName,
																		'#default_value'	=> $person_vill_ward_old,
																		'#required' 		=>  TRUE,
																		//'#attributes' 	=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Gram Panchayat'),
																		'#attributes' 		=> $readonly_person_vill_ward,
																		'#ajax'				=> array(
																								  'event' 		=> 'change',
																								  'effect' 		=> 'fade',			
																								  'callback' 	=> 'get_info_form_ajax_callback',
																								  'progress'	=>	array('type' => 'throbber','message' => 'Loading...'),
																								),	
																		'#prefix' 			=> '<div id="gp_ward_div"><section class="col-md-6"><label id="label_replace" class="select">',
																		'#suffix' 			=> '</label></section></div>'
																								);
							 }
						 }
					}
						
						/***************************** Police Station block start **********************/
					$value_gp_ward 								= isset($form_state['values']['mtw_directorspanchyat']) ? $form_state['values']['mtw_directorspanchyat'] : $person_vill_ward_old;
			
					$policeData 								= array();
					$policeData[] 								= '--Select--';		
					$form['mtw_directorspolicestation'] 		= array(
																		'#title' 			=> 'Select Police Station',
																		'#type' 			=> 'select',
																		'#options' 			=> $policeData,
																		'#default_value'	=> $person_ps_old,
																		'#required' 		=>  TRUE,
																		//'#attributes' 	=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Police Station'),
																		'#attributes' 		=> $readonly_person_ps,
																		'#prefix' 			=> '<div id="ps_div"><section class="col-md-6"><label id="label_replace" class="select">',
																		'#suffix' 			=> '</label></section></div>'
																);
					
					if($value_dis != "" && $value_subdiv != ""){
						if($value_areatype != "" && $value_areatype != "0"){
							if($value_block_mun != "" && $value_block_mun != "0"){
								if($value_gp_ward != "" && $value_gp_ward!='0'){
									$form['mtw_directorspolicestation'] = array(
																		'#title' 			=> 'Select Police Station',
																		'#type' 			=> 'select',
																		'#options' 			=> common_custom_user_ps_list_new($value_dis),
																		'#default_value'	=> $person_ps_old,
																		'#required' 		=> TRUE,
																		//'#attributes' 	=> array('id' => 'loc_e_ps_id','autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Police Station'),
																		'#attributes' 		=> $readonly_person_ps,
																		'#prefix' 			=> '<div id="ps_div"><section class="col-md-6"><label id="label_replace" class="select">',
																		'#suffix' 			=> '</label></section></div>'
																								);
								}
							}
						}
					}
					
						/***************************** Pin Code block start **********************/
						$form['mtw_directorspincode'] 			= array(
																		'#title' 			=> 'Pin Code',
																		'#type' 			=> 'textfield',
																		'#required' 		=> TRUE,
																		'#default_value'	=> $person_pincode_old,
																		'#element_validate' => array('numeric_validation'),
																		//'#attributes' 		=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Pin Code','maxlength' => 6),
																		'#attributes' 		=> $readonly_person_pincode,
																		'#prefix' 			=> '<div id="pincode_div"><section class="col-md-6"><label id="label_replace" class="input">',
																		'#suffix' 			=> '</label></section></div>'
																);
						}
					}
				}
			}												  
		}																		  
	}
					$form['markup_last'] = array(
						'#type' =>'markup',
						'#markup' => '</div></div>'
					);
														
	
	$form['back_button'] = array(
		'#type'		=>'markup',
		'#markup'	=>l(t('Back To List'),'mtw-registration-details/addmore/'.$id,array('attributes' => array('html'=>TRUE , 'title'=>'Click To Show List', 'class' => array('btn btn-primary pull-left'))))
	);
	
	
	$form['skip_button'] = array(
		'#type'		=>'markup',
		'#markup'	=>l(t('Next'),'mtw-registration-details/upload/'.$id,array('attributes' => array('html'=>TRUE , 'title'=>'Click for Next Tab', 'class' => array('btn btn-primary mleft-15'))))
	);
	
	$form['ownership_type'] = array(
		'#type'		=> 'hidden',
		'#value'	=> $p_type
	);
	
	
	$query_1	=	db_query('select final_submit_status,status from l_mtw_registration_master where id=:id', array(':id' => $application_id));
	$result_1	=	$query_1->fetchObject();
	if($result_1->final_submit_status !='P' && $result_1->status == '' ){
		$form['submit_button'] 	= array(
				'#type'		=>'submit',
				'#value'	=>t($btn_lbl),
				'#attributes' 	=> array('class' => array('btn btn-primary pull-right')),
		);	
	}
	
	
	
	
	
	
	
	
	return $form;
}


function addmore_details_validate($form, &$form_state, $id=0, $p_id=0){
	/*$val					= $form_state['values'];
	$reg_director_name		= $val['mtw_directorsname'];
	$reg_director_address		= $val['mtw_directorsloc'];
	$reg_director_dist		= $val['mtw_directorsdistrict'];
	$reg_director_subdivision	= $val['mtw_directorssubdivision'];
	$reg_director_areatype		= $val['mtw_directorsareatype'];
	$reg_director_areatype_code	= $val['mtw_directorsblock'];
	$reg_director_vill_ward		= $val['mtw_directorspanchyat'];
	$reg_director_ps		= $val['mtw_directorspolicestation'];
	$reg_director_pincode		= $val['mtw_directorspincode'];
	if($reg_director_pincode==""){
		form_error($form['mtw_directorspincode'], t('Pin Code cannot be blank.'));
	}else{
		$fieldValue=trim($reg_director_pincode); 
		$match_pattern = '/^[0-9]+$/i';
	
		if(!empty($fieldValue)){
			if(!preg_match($match_pattern, $fieldValue)) {
				form_error($form['mtw_directorspincode'], t($element['#title'].' SHOULD BE NUMBER ONLY'));
			}
			if(strlen($fieldValue)!=6){
				form_error($form['mtw_directorspincode'], t($element['#title'].' SHOULD BE 6 DIGIT'));
			}
		}	
	}*/
}

function REGISTRATION__TEXTBOX_NUMERIC_VALIDATE($element,&$form_state){
	$fieldValue=trim($element['#value']); 
	$match_pattern = '/^[0-9]+$/i';
	
	if(!empty($fieldValue)){
		if(!preg_match($match_pattern, $fieldValue)) {
			form_error($element, t($element['#title'].' SHOULD BE NUMBER ONLY'));
		}
		if(strlen($fieldValue)!=6){
			form_error($element, t($element['#title'].' SHOULD BE 6 DIGIT'));
		}
	}
}

function addmore_details_submit($form, &$form_state, $id=0, $p_id=0){
	$val						= $form_state['values'];   //echo '<pre>'; print_r($val); echo $val['action_data']; die;
		
	$designation			= $val['designation'];
	if($designation=='other'){
		$designation		= $val['designation_other'];
	}
	
	$company_name			= $val['mtw_directors_compname'];
	$person_name			= $val['mtw_directorsname'];
	$person_address			= $val['mtw_directorsloc'];
	$person_country			= $val['mtw_country'];
	$person_state			= $val['mtw_state'];
	
	$person_district		= $val['mtw_directorsdistrict'];
	$person_subdivision		= $val['mtw_directorssubdivision'];
	$person_areatype		= $val['mtw_directorsareatype'];
	$person_areatype_code	= $val['mtw_directorsblock'];
	$person_vill_ward		= $val['mtw_directorspanchyat'];
	$person_ps				= $val['mtw_directorspolicestation'];
	$person_pincode			= $val['mtw_directorspincode'];
	
	$application_id			= $val['application_id'];
	$person_id				= $val['person_id'];
	//$action_data			= $val['action_data']; 
	
	$query = db_select('l_mtw_registration_master','lmrm')->fields('lmrm', array('identification_number'));
	$query->condition('lmrm.id',$application_id,'=');
	$result = $query->execute();
	if( $result->rowCount() > 0 ){
		// update chcking
		$resultArray	= $result->fetchAssoc();
		
		$dataArray	= array(
								'identification_number'	=> $resultArray['identification_number'],
								'application_id'		=> $application_id,
								'designation'			=> $designation,
								'company_name'			=> $company_name,
								'person_name'			=> $person_name,
								'person_address'		=> $person_address,
								'person_country'		=> $person_country,
								'person_state'			=> $person_state,
								'person_district'		=> $person_district,
								'person_subdivision'	=> $person_subdivision,
								'person_areatype'		=> $person_areatype,
								'person_areatype_code'	=> $person_areatype_code,
								'person_vill_ward'		=> $person_vill_ward,
								'person_ps'				=> $person_ps,
								'person_pincode'		=> $person_pincode,
								'status'				=> 'Y',
								'delete_status'			=> 'N'
						);
		
		//print_r($dataArray);die('ok');
		
		if(($person_id!=0 || $person_id!='')){
			// update data			
			$update = db_update('l_mtw_registration_master_extra')->fields($dataArray)->condition('id',$person_id)->execute();
		}else{
			// insert data
			$dataArray['added_date'] = date('Y-m-d h:i:s');
			$insert_id = db_insert('l_mtw_registration_master_extra')->fields($dataArray)->execute();			
		}
	} 
	drupal_goto('mtw-registration-details/addmore/'.encryption_decryption_fun('encrypt', $application_id));
}
/***************** ajax all functions ***************/

function get_otherdesignation_ajax_callback($form, &$form_state){
	$data	=	array();
	$data[]	=	ajax_command_replace('#designation_other_div',drupal_render($form['designation_other']));	
	$data[]	=	ajax_command_replace('#company_div',drupal_render($form['mtw_directors_compname']));
	$data[]	=	ajax_command_replace('#name_div',drupal_render($form['mtw_directorsname']));
	return array('#type'=>'ajax','#commands'=>$data);	
}


function get_info_form_ajax_callback($form, &$form_state){
	$data 	= array();
	$data[] = ajax_command_replace('#country_others_div', drupal_render($form['mtw_state']));
	$data[]	= ajax_command_replace('#distric_div', drupal_render($form['mtw_directorsdistrict']));
	$data[]	= ajax_command_replace('#subdivisn_div', drupal_render($form['mtw_directorssubdivision']));
	$data[]	= ajax_command_replace('#areatype_div', drupal_render($form['mtw_directorsareatype']));
	$data[]	= ajax_command_replace('#block_mun_div', drupal_render($form['mtw_directorsblock']));
	$data[]	= ajax_command_replace('#gp_ward_div', drupal_render($form['mtw_directorspanchyat']));
	$data[]	= ajax_command_replace('#ps_div', drupal_render($form['mtw_directorspolicestation']));
	$data[]	= ajax_command_replace('#pincode_div', drupal_render($form['mtw_directorspincode']));
	return array('#type' => 'ajax', '#commands' => $data);
}



/********** end of all ajax *********/



function get_all_state1($value_country=''){
	$default_select 	= array(""=>"- Select State -");
	$result 			= db_query("SELECT id,statename FROM {state_master} where country_id = $value_country order by statename");
	$list 				= array();
	foreach ($result as $record) {
		$list[$record->id] = t('@districtname', array(
			'@districtname' => $record->statename,
		));
	}
	return ($default_select+$list);
}
function get_discrict_by_id($value_state=''){
	$default_select 	= array(""=>"- Select District -");
	$result 			= db_query("SELECT district_code,district_name FROM {district_master} where is_active = 'Y' and state_id = $value_state order by district_name");
	$list 				= array();
	foreach ($result as $record) {
		$list[$record->district_code] = t('@districtname', array(
			'@districtname' => $record->district_name,
		));
	}
	return ($default_select+$list);
}
function numeric_validation($element,&$form_state){
	$fieldValue=trim($element['#value']); 
	$match_pattern = '/^[0-9]+$/i';
	
	if(!empty($fieldValue)){
		if(!preg_match($match_pattern, $fieldValue)) {
			form_error($element, t($element['#title'].' SHOULD BE NUMBER ONLY'));
		}
		if(strlen($fieldValue)!=6){
			form_error($element, t($element['#title'].' SHOULD BE 6 DIGIT'));
		}
	}
}
function get_info_form_validate($form, &$form_state){
	
	if($form_state['values']['country']==""){
		form_error($form['info_form']['country'],t('Country should not be blank'));
	}
	if($form_state['values']['state']==""){
		form_error($form['info_form']['state'],t('State should not be blank'));
	}
}
