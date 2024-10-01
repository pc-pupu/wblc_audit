<?php
//drupal_add_css(drupal_get_path('module', 'motor_transport') . '/css/sky-forms.css');
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
drupal_add_js(drupal_get_path('module', 'motor_transport') . '/js/mtw_upload_custom.js');
function mtw_application_details($form, &$form_state, $id='') {
	  global $base_root,$base_path,$user;
      $user_id                  	= $user->uid;
      $application_id             	= $id;
      $application_id           	= encryption_decryption_fun('decrypt',$id);
	
		/****** get data from database on the specific uri (application id) *****/
		$mtw_fetch_reg_det          	= db_query("select * from l_mtw_registration_master where id=:application_id", array( ':application_id' => $application_id ));
        
        $identification_number      = "";
        
        $mtw_name                   = "";        
        $mtw_loc_address            = "";
        $mtw_loc_dist               = "";
        $mtw_loc_subdivision        = "";
        $mtw_loc_areatype           = "";
        $mtw_loc_areatype_code      = "";
        $mtw_loc_vill_ward          = "";
        $mtw_loc_ps                 = "";
        $mtw_loc_pincode            = "";
        
        $mtw_nature                 = "";
		$mtw_nature_other			= "";
        $total_routes               = "";
		$mtw_route_details_old		= "";
        $total_route_milage         = "";
        $total_mtw_vehicle          = "";
        $mtw_maxworkers             = "";
                
        $created            = "";
        $backlog_id         = ""; 
        $payment_mode       = "";
        
		
		$readonly_mtw_name 						= array('id' => '','autocomplete' => 'off','class'=>array(''),'placeholder'=>'Motor Trasport Undertaking Name');
		$readonly_mtw_loc_address 				= array('id' => '','autocomplete' => 'off','class'=>array('addline-textarea'),'placeholder'=>'Motor Trasport Undertaking Location');
		$readonly_mtw_loc_dist 					= array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select District');
		$readonly_mtw_loc_subdivision			= array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Sub-Division','onkeyup' => 'getSpanText()', 'onblur' => 'getSpanText()', 'onchange' => 'getSpanText()');
		$readonly_mtw_loc_areatype				= array('autocomplete' => 'off', 'placeholder'=>'Select area type');
		$readonly_mtw_loc_areatype_code			= array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Gram Panchayat/Ward');
		$readonly_mtw_loc_ps					= array('id' => 'loc_e_ps_id','autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Police Station');
		$readonly_mtw_loc_pincode				= array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Pin Code','maxlength' => 6);
		$readonly_mtw_nature					= array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Motor Trasport Undertaking Nature');
		
		$readonly_mtw_totalroute				= array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Total number of routes');
		$readonly_mtw_route_details				= array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Total number of routes details'); 
		
		
		$readonly_mtw_totalmilage				= array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Total route mileage');
		$readonly_mtw_totalvehicle				= array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Total Number of Motor Trasport Vehicles');
		$readonly_mtw_maxworkers				= array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Maximum number of Motor Transport Workers');
		
        if($mtw_fetch_reg_det->rowCount() > 0){
            $fetArray = $mtw_fetch_reg_det->fetchAssoc();
            
            $identification_number      = $fetArray['identification_number'];
            
            $mtw_name                   = $fetArray['mtw_name'];
            $mtw_loc_address            = $fetArray['mtw_loc_address'];
            $mtw_loc_dist               = $fetArray['mtw_loc_dist'];
            
            $mtw_loc_subdivision        = $fetArray['mtw_loc_subdivision'];
            $mtw_loc_areatype           = $fetArray['mtw_loc_areatype'];
            $mtw_loc_areatype_code      = $fetArray['mtw_loc_areatype_code'];
            $mtw_loc_vill_ward          = $fetArray['mtw_loc_vill_ward'];
			
			
            $mtw_loc_ps                 = $fetArray['mtw_loc_ps'];
			
			
            $mtw_loc_pincode            = $fetArray['mtw_loc_pincode'];
            $mtw_nature                 = $fetArray['mtw_nature'];
			$mtw_nature_other			= $fetArray['mtw_nature'];
            $total_routes               = $fetArray['total_routes'];
			$mtw_route_details_old		= $fetArray['mtw_route_details'];
			
            $total_route_milage         = '';
            $total_mtw_vehicle         	= $fetArray['total_mtw_vehicle'];
			$vreg_number_details		= $fetArray['vreg_number_details'];
			
            $mtw_maxworkers             = $fetArray['mtw_maxworkers'];
            
            $created                    = $fetArray['created'];
            $backlog_id                 = $fetArray['backlog_id'];
            $payment_mode               = $fetArray['payment_mode'];
			$status						= $fetArray['status'];
			$final_submit_status		= $fetArray['final_submit_status'];
            
        }
        if(!empty($application_id) && $status == 'B') {
			
			
			$query 		 = db_select('l_mtw_remark_master', 'lbkrm')
						 ->	fields('lbkrm', array('remark_field_title'))
						 ->	condition('lbkrm.remark_to', trim($user_id))
						 ->	condition('lbkrm.application_id', $application_id)
						 ->	orderBy('lbkrm.id','DESC');
						 
			$remark_field_text_query = $query->execute();
			$remark_field_text_array = $remark_field_text_query->fetchAssoc(); 
			$verified_fields = explode(',', $remark_field_text_array['remark_field_title']);
			
			
			foreach($verified_fields as $fields){ 
			
			
				switch($fields){	
				
					case "mtw_name":
					
							$readonly_mtw_name 							= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
							
							
					case "mtw_loc_address":
							$readonly_mtw_loc_address 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_mtw_loc_dist 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_mtw_loc_subdivision 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_mtw_loc_areatype 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_mtw_loc_areatype_code 			= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_mtw_loc_vill_ward 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_mtw_loc_ps 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_mtw_loc_pincode 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
					case "mtw_nature":
							$readonly_mtw_nature 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
					case "total_routes": 		
							$readonly_mtw_totalroute 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_mtw_route_details 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
					case "total_route_milage": 
							$readonly_mtw_totalmilage 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
					case "total_mtw_vehicle": 
							$readonly_mtw_totalvehicle 				= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;
					case "mtw_maxworkers": 
							$readonly_mtw_maxworkers 					= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							break;					
					default:		
				}
			}
		}
		
	
	$form['form_begin'] = array(
		'#type' 		=> 'markup',
		'#markup' 		=> '<div class="panel panel-default panel-form">
								<div class="panel-heading">Name and Address to which communications relating to the Motor Transport undertaking should be sent</div>
								<div class="panel-body">',
	);
	
	$form['mtw_name'] = array(
		'#title' 			=> '1) Name of Motor Transport Undertaking',
		'#type' 			=> 'textfield',
		'#attributes'		=> $readonly_mtw_name,
		'#required'   		=>  TRUE,
		'#default_value'	=> $mtw_name,
		'#element_validate' => array('REGISTRATION_TEXTBOX_TEXT_VALIDATE'),
		'#prefix' 			=> '<div class="row"><div class="col-lg-4 col-md-6"><label class="input">',
		'#suffix' 			=> '</label></div>'
	);
	
	$form['mtw_location'] = array(
		'#title' 			=> '2.1) Address Line1',
		'#type' 			=> 'textarea',
		'#required' 		=>  TRUE,
		'#default_value'	=> $mtw_loc_address,
		'#attributes' 		=> $readonly_mtw_loc_address,
		'#prefix' 			=> '<div class="col-lg-8 col-md-8 addline-textarea"><label class="input">',
		'#suffix' 			=> '</label></div></div>'
	);
	
	$options_dist = common_custom_user_district_list_new();
	
	$form['mtw_district'] = array(
		'#title' 			=> '2.2) Select District',
		'#type' 			=> 'select',
		'#options' 			=> $options_dist,
		'#required' 		=>  TRUE,
		'#default_value'	=> $mtw_loc_dist,
		'#id'				=> 'loc_e_dist_id',
		'#ajax'				=> array(
									'event' 	=> 'change',
									'effect' 	=> 'fade',			
									'callback' 	=> 'get_mtw_registration_info_form_ajax_callback',
									'progress' 	=> '',
							),
		'#attributes' 		=> $readonly_mtw_loc_dist,
		'#prefix' 			=> '<div class="row"><div class="col-lg-4 col-md-6"><label class="select">',
		'#suffix' 			=> '</label></div>'
	);
	
	$value_loc_e_dist 			= isset($form_state['values']['mtw_district']) ? $form_state['values']['mtw_district'] : $mtw_loc_dist;
	
	if($value_loc_e_dist != "" || !empty($value_loc_e_dist) || is_numeric($value_loc_e_dist)){				
		$form['mtw_subdivision'] = array(
			'#title'			=> '2.3) Select Subdivision',
			'#type' 			=> 'select',
			'#options' 			=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist),
			'#required' 		=>  TRUE,
			'#default_value'	=> $mtw_loc_subdivision,
			'#id' 				=> 'loc_e_subdivision_id',
			'#ajax'				=> array(
										'event' 	=> 'change',
										'effect' 	=> 'fade',			
										'callback' => 'get_mtw_registration_info_form_ajax_callback',
										'progress' => '',
					),		
			'#attributes' 		=> $readonly_mtw_loc_subdivision,
			'#prefix' 			=> '<div id="mtw_subdvsn_replace"><div class="col-lg-4 col-md-6"><label class="select">',
			'#suffix' 			=> '</label></div></div>'
		);
	}else {
		$form['mtw_subdivision'] = array(
			'#title'			=> '2.3) Select Subdivision',
			'#type' 			=> 'select',
			'#options' 			=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist),
			'#required' 		=>  TRUE,
			'#default_value'	=> $mtw_loc_subdivision,
			'#id' 				=> 'loc_e_subdivision_id',
			'#ajax'				=> array(
										'event' 	=> 'change',
										'effect' 	=> 'fade',			
										'callback' => 'get_mtw_registration_info_form_ajax_callback',
										'progress' => '',
								),		
			'#attributes' 		=> $readonly_mtw_loc_subdivision,
			'#prefix' 			=> '<div id="mtw_subdvsn_replace"><div class="col-lg-4 col-md-6"><label class="select">',
			'#suffix' 			=> '</label></div></div>'
		);
	}
	$value_loc_e_subdivision1 = isset($form_state['values']['mtw_subdivision']) ? $form_state['values']['mtw_subdivision'] : $mtw_loc_subdivision;
	
	$form['divmkup'] = array(
		'#type' 			=> 'markup',
		//'#prefix' 			=> '<div class="row">',
	);
	
	$areaTypeData 				= array('-Select-');
	if($value_loc_e_dist != "" && $value_loc_e_subdivision1 != ""){	
		$areatypeOptions 		= common_custom_user_areatype_list_new($value_loc_e_dist, $value_loc_e_subdivision1);
		
		if(empty($areatypeOptions)){
			$areatypeOptions	= array();
		}
		$form['mtw_areatype'] = array(
			'#title'		=> '2.4) Select Block / Municipality / Corporation / SEZ / Notified Area',
			'#type' 		=> 'select',
			'#options' 		=> $areatypeOptions,
			'#default_value'=> $mtw_loc_areatype,
			'#id'           => 'loc_e_areatype_id',
			'#attributes'   => $readonly_mtw_loc_areatype,
			'#ajax'			=> array(
									'event' 	=> 'change',
									'effect' 	=> 'fade',			
									'callback' 	=> 'get_mtw_registration_info_form_ajax_callback',
									'progress'	=> '',
							),	
			'#prefix' 		=> '<div id="mtw_loc_e_areatype"><div class="col-lg-4 col-md-6"><label class="select">',
			'#suffix' 		=> '</label></div></div></div>'
		);
	}else {
		$form['mtw_areatype'] = array(
			'#title'		=> '2.4) Select Block/Municipality/Corporation/SEZ/Notified Area',
			'#type' 		=> 'select',
			'#options' 		=> $areaTypeData,
			'#required' 	=>  TRUE,
			'#default_value'	=> $mtw_loc_areatype,
			'#id'           	=> 'loc_e_areatype_id',
			'#attributes'   	=> $readonly_mtw_loc_areatype,
			'#ajax'			=> array(
									'event' 	=> 'change',
									'effect' 	=> 'fade',			
									'callback' 	=> 'get_mtw_registration_info_form_ajax_callback',
									'progress' 	=> '',
							),	
			'#prefix' 		=> '<div id="mtw_loc_e_areatype"><div class="col-lg-4 col-md-6"><label class="select">',
			'#suffix' 		=> '</label></div></div></div>'
		);
	}
 
 	$value_loc_e_areatype = isset($form_state['values']['mtw_areatype']) ? $form_state['values']['mtw_areatype'] : $mtw_loc_areatype;
 	
	$blockData = array();
	$form['mtw_block'] = array(
		'#title'		=> '2.5) Name of Block',
		'#type' 		=> 'select',
		'#options'		=> $blockData,
		'#required' 	=>  TRUE,
		'#attributes' 	=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Select Block/Municipality/Corporation/SEZ/Notified Area'),
		'#ajax'			=> array(
								'event' 	=> 'change',
								'effect' 	=> 'fade',			
								'callback' 	=> 'get_mtw_registration_info_form_ajax_callback',
								'progress' 	=> '',
			),	
		'#prefix' => '<div class="row"><div id="mtw_common_1_replace"><div class="col-lg-4 col-md-6"><label class="select">',
		'#suffix' => '</label></div></div>'
	);

	
		if($value_loc_e_dist != "" && $value_loc_e_subdivision1 != ""){
			if($value_loc_e_areatype != ""){	
				if ( $value_loc_e_areatype == 'B' ){
					$areatype_name	= 'Block';
				}elseif( $value_loc_e_areatype == 'M'){
					$areatype_name	= 'Municipality';
				}elseif( $value_loc_e_areatype == 'C'){
					$areatype_name	= 'Corporation';
				}elseif( $value_loc_e_areatype == 'S'){
					$areatype_name	= 'SEZ';
				}elseif($value_loc_e_areatype == 'N'){
					$areatype_name	= 'Notified Area';
				}
						
						
				$form['mtw_block']  = array(
					'#type' 			=> 'select',
					'#title'			=> '2.5) Name of '.$areatype_name,
					'#options' 			=> common_custom_user_block_list_new($value_loc_e_subdivision1,$value_loc_e_areatype),
					'#required' 		=>  TRUE,
					'#default_value'	=> $mtw_loc_areatype_code,
					'#attributes' 		=> $readonly_mtw_loc_areatype_code,
					'#ajax'	   			=> array(
											  'event' 	=> 'change',
											  'effect' 	=> 'fade',			
											  'callback'      => 'get_mtw_registration_info_form_ajax_callback',
											  'progress'      => '',
										  ),	
					'#prefix' 			=> '<div class="row"><div id="mtw_common_1_replace"><div class="col-lg-4 col-md-6"><label class="select">',
					'#suffix' 			=> '</label></div></div>'
				);
			}
	 	}
				
	$value_common_selection_1 = isset($form_state['values']['mtw_block']) ? $form_state['values']['mtw_block'] : $mtw_loc_areatype_code;
	
	
	$panchyatData = array();
	$form['mtw_panchyat'] = array(
		'#title' 			=> '2.6) Select Gram Panchayat/Ward',
		'#type' 			=> 'select',
		'#options' 			=> $panchyatData,
		'#required' 		=> TRUE,
		'#attributes'   	=>  $readonly_mtw_loc_areatype_code,
		'#ajax'				=> array(
									'event' 	=> 'change',
									'effect' 	=> 'fade',			
									'callback' 	=> 'get_mtw_registration_info_form_ajax_callback',
									'progress' 	=> '',
								  ),	
		'#prefix' 			=> '<div id="mtw_common_2_replace"><div class="col-lg-4 col-md-6"><label class="select">',
		'#suffix' 			=> '</label></div></div>'
	);
	
	if($value_loc_e_dist != "" && $value_loc_e_subdivision1 != ""){
	    if($value_loc_e_areatype != ""){
			if($value_common_selection_1 != ""){
				
				if($value_loc_e_areatype == 'B'){
					$function_name 		=	common_custom_user_villward_list_new($value_common_selection_1,'V');
					$title_word			=	'Gram Panchayat Name';
				}elseif($value_loc_e_areatype == 'M'){
					$function_name 		=	common_custom_user_villward_list_new($value_common_selection_1,'M');
					$title_word			=	'Ward Number';
				}elseif($value_loc_e_areatype == 'C'){
					$function_name 		=	common_custom_user_corporation_list_new($value_common_selection_1);
					$title_word			=	'Ward Number';
				}elseif($value_loc_e_areatype == 'S'){
					$function_name 		=	common_custom_user_villward_list_new($value_common_selection_1,'S');
					$title_word			=	'Sector';
				}elseif($value_loc_e_areatype == 'N'){
					$function_name 		=	common_custom_user_villward_list_new($value_common_selection_1,'N');
					$title_word			=	'Notified Area';
				}
				
				
				$form['mtw_panchyat']   = array(
					'#type'         => 'select',
					'#title'		=> '2.6) '.$title_word,
					'#options'      => $function_name,
					'#required'   	=>  TRUE,
					'#default_value'=> $mtw_loc_vill_ward,
					'#attributes'   => $readonly_mtw_loc_areatype_code,
					'#ajax'	    	=> array(
										'event' 	=> 'change',
										'effect' 	=> 'fade',			
										'callback' => 'get_mtw_registration_info_form_ajax_callback',
										'progress' => '',
									),	
					'#prefix' 		=> '<div id="mtw_common_2_replace"><div class="col-lg-4 col-md-6"><label class="select">',
					'#suffix' 		=> '</label></div></div>'
				);
			}
	    }
	}
			 
	$value_common_selection_2 = isset($form_state['values']['mtw_panchyat']) ? $form_state['values']['mtw_panchyat'] : $mtw_loc_vill_ward;
	
	$policeData = array('-Select-');		
	$form['mtw_policestation'] = array(
		'#type' 		=> 'select',
		'#title'		=> '2.7) Select Police Station',
		'#options' 		=> $policeData,
		'#required' 	=>  TRUE,
		'#default_value'	=> $mtw_loc_ps,
		'#attributes' 	=> $readonly_mtw_loc_ps,
		'#prefix' 		=> '<div id="mtw_ps_replace"><div class="col-lg-4 col-md-6"><label class="select">',
		'#suffix' 		=> '</label></div></div></div>'
	);
	
	if($value_loc_e_dist != "" && $value_loc_e_subdivision1 != ""){
		if($value_loc_e_areatype != "" && $value_loc_e_areatype != "0"){
			if($value_common_selection_1 != "" && $value_common_selection_1 != "0"){
				if($value_common_selection_2 != "" && $value_common_selection_2!='0'){
					$form['mtw_policestation'] = array(
						'#type' 			=> 'select',
						'#title'			=> '2.7) Select Police Station',
						'#options' 			=> common_custom_user_ps_list_new($value_loc_e_dist),
						'#required'			=>  TRUE,
						'#default_value'	=> $mtw_loc_ps,
						'#attributes' 		=> $readonly_mtw_loc_ps,
						'#prefix' 			=> '<div id="mtw_ps_replace"><div class="col-lg-4 col-md-6"><label class="select">',
						'#suffix' 			=> '</label></div></div></div>'
					);
				}
			}
		}
	}
	$form['mtw_pincode'] = array(
		'#title' 			=> '2.8) Pin Code',
		'#type' 			=> 'textfield',
		'#required' 		=>  TRUE,
		'#element_validate' => array('REGISTRATION__TEXTBOX_NUMERIC_VALIDATE'),
		'#default_value'	=> $mtw_loc_pincode,
		'#attributes' 		=> $readonly_mtw_loc_pincode,
		'#prefix' 			=> '<div class="row"><div id="mtw_loc_pincodee"><div class="col-lg-4 col-md-6"><label class="input">',
		'#suffix' 			=> '</label></div></div></div>'
	);
	
	$form['markup2']	= array(
		'#type' => 'markup',
		'#markup' => '</div></div>'
	);
	
	$mtwNatureArray = array('' => 'Select','city_service'=>'City Service','long_distance'=>'Long Distance','passenger_service'=>'Passenger Service','long_distance_freight_service'=>'Long distance freight service', 'other'=>'Other');
	
	if(array_key_exists($mtw_nature,$mtwNatureArray)){
		$mtw_nature = $mtw_nature;
	}else{
		$mtw_nature = 'other';
		$otherText	= $mtw_nature_other;		
	}
	
	$form['markup4'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="panel panel-default panel-form">
						<div class="panel-heading"> Nature of Motor Transport Service </div>
						<div class="panel-body">'
	);
	
	
	$form['mtw_nature'] = array(
		'#prefix' 			=> '<div class="row"><div class="col-lg-4 col-md-6"><label class="select">',
		'#suffix' 			=> '</label></div>',
		'#title' 			=> '3) Nature of Motor Transport Service<span id="hint_nature"><img src="'.$base_root.$base_path.'sites/all/modules/applicant_registration/images/tooltip.jpg"></span>',
		'#type' 			=> 'select',
		'#options'			=> $mtwNatureArray,
		'#default_value'	=> $mtw_nature,
		'#attributes' 		=> $readonly_mtw_nature,
		//'#required' 		=>  TRUE,
		
		'#ajax'				=> array(
								  'event' 	=> 'change',
								  'effect' 	=> 'fade',			
								  'callback' => 'get_other_div_ajax_callback',
								  'progress' => '',
								),
	);
	
	$form['nature_other'] = array(
		'#prefix' 			=> '<div id="nature_other_div">',
		'#suffix' 			=> '</div>'
	);
	
	$natureVal = isset($form_state['values']['mtw_nature'])?$form_state['values']['mtw_nature'] : $mtw_nature;
	
	if(!empty($natureVal) && $natureVal!='' && $natureVal=='other'){
		
		$form['nature_other'] = array(
			'#prefix' 			=> '<div id="nature_other_div"><div class="col-lg-4 col-md-6"><label class="input">',
			'#suffix' 			=> '</label></div></div>',
			'#type' 			=> 'textfield',
			'#default_value'	=> $otherText,
			'#title' 			=> 'Other Details',
			'#attributes'		=> $readonly_mtw_nature,
			'#required' 		=>  TRUE,
		);	
	}
	
	$form['markup5'] = array(
		'#type' => 'markup',
		'#markup' => '</div></div></div>'
	);
	
	$form['markup6'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="panel panel-default panel-form">
						<div class="panel-heading">Route Information Details</div>
						<div class="panel-body">'
	);
	
	$form['mtw_totalroute'] = array(
		'#title' 			=> '4) Total Number Of Routes',
		'#type' 			=> 'textfield',
		//'#required' 		=>  TRUE,
		'#default_value'	=> $total_routes,
		'#attributes' 		=> $readonly_mtw_totalroute,
		'#element_validate' => array('REGISTRATION__ONLY_NUMERIC_VALIDATE'),
		'#prefix' 			=> '<div class="row"><div class="col-lg-4 col-md-6"><label class="input">',
		'#suffix' 			=> '</label></div></div>',
		'#ajax'				=> array(
									'event'=> 'keyup',
									'effect'=> 'fade',
									'callback'=> 'add_more_ajax_callback'
								)
	);
	
	$routeNumber = isset($form_state['values']['mtw_totalroute']) ? $form_state['values']['mtw_totalroute'] : $total_routes;
	
	
	$form['mtw_route_details'] = array(		
		'#prefix'	=> '<div id="multiple_div"><div class="col-lg-4 col-md-6"><label class="input">',
		'#suffix'	=> '</label></div></div>',
	
	);
	$form['mtw_totalmilage'] = array(
		'#prefix'	=> '<div id="multiple_div_milage"><div class="col-lg-4 col-md-6"><label class="input">',
		'#suffix'	=> '</label></div></div>'
	);
	
	$i = 0;
	$j = 1;
	
	
	$getRouteDetailsData =	db_select('l_mtw_registration_extended','lmre')->fields('lmre',array())->condition('lmre.application_id',$application_id,'=');
	$getRouteDetailsFetch =	$getRouteDetailsData->execute();
	$getRouteDetails =	$getRouteDetailsFetch->fetchAll();
	
	if(($routeNumber!=0)){
	if($getRouteDetailsFetch->rowCount()>0){
		$total_route_milage_db	= 0;
		$old_no_route 			=	$getRouteDetailsFetch->rowCount();
		foreach($getRouteDetails as $sdata){
			
			$form['mtw_route_details']['route_edit_'.$j] = array(
				'#title' 			=> '4.1.'.$j.') Route Details',
				'#type' 			=> 'textfield',
				'#default_value'	=>	$sdata->route,
				'#attributes' 		=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Route Details'),
				'#prefix' 			=> '<div class="#col-lg-4 #col-md-6"><div id="multiple_div"><label class="input">',
				'#suffix' 			=> '</label></div></div>'
			);
			$form['mtw_totalmilage']['milage_edit_'.$j] = array(
				'#title' 			=> '4.2.'.$j.') Route Mileage (in km)',
				'#type' 			=> 'textfield',
				'#default_value'	=>  $sdata->milage,
				'#attributes' 		=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Route Mileage in numbers 20, 25'),
				'#prefix' 			=> '<div class="#col-lg-4 #col-md-6"><div id="multiple_div_milage"><label class="input">',
				'#suffix' 			=> '</label></div></div>'
			);
		
			$form['mtw_totalmilage']['row_id_'.$j] = array(
				'#type'				=> 'hidden',
				'#value'			=>	$sdata->id,
			);
			$j++;
			$total_route_milage_db = $total_route_milage_db+$sdata->milage;
		}
	}
	if($old_no_route<$routeNumber){
		$insertRoute				= $routeNumber - $old_no_route;
	}else{
		$insertRoute				= 0;
	}
	$form['insert_hidden'] = array(
		'#type'		=> 'hidden',
		'#value'	=> $insertRoute
	);
	for($i=1; $i<= $insertRoute; $i++){
		$form['mtw_route_details']['route_'.$i] = array(
			'#title' 			=> '4.1.'.$j.') Route Details',
			'#type' 			=> 'textfield',
			'#attributes' 		=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Route Details'),
			'#prefix' 			=> '<div class="#col-lg-4 #col-md-6"><div id="multiple_div"><label class="input">',
			'#suffix' 			=> '</label></div></div>'
		);
		$form['mtw_totalmilage']['milage_'.$i] = array(
			'#title' 			=> '4.2.'.$j.') Route Mileage (in km)',
			'#type' 			=> 'textfield',
			'#attributes' 		=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Route Mileage in numbers 20, 25'),
			//'#required' 		=>  TRUE,
			'#prefix' 			=> '<div class="#col-lg-4 #col-md-6"><div id="multiple_div"><label class="input">',
			'#suffix' 			=> '</label></div></div>'
		);			
		$j++;
		}
	}
	
	
	$form['mtw_totalmilage_ajax'] = array(
		'#title' 			=> '5) Total Routes Mileage',
		'#type' 			=> 'textfield',
		//'#required' 		=>  TRUE,
		'#default_value'	=> $total_route_milage_db,
		'#attributes' 		=> $readonly_mtw_totalroute,
		'#element_validate' => array('REGISTRATION__ONLY_NUMERIC_VALIDATE'),
		'#prefix' 			=> '<div class="row"><div class="col-lg-4 col-md-6"><label class="input">',
		'#suffix' 			=> '</label></div></div>',
		'#ajax'				=> array(
								'event'=> 'keyup',
								'effect'=> 'fade',
								'callback'=> 'add_more_ajax_callback'
								)
	);
	
	$form['markup7'] = array(
		'#type'					=> 'markup',
		'#markup'				=> '</div></div>'
	);
	
	/***************************************************************/
	
	$form['markup8'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="panel panel-default panel-form">
						<div class="panel-heading">Number of Motor Transport/ Vehicles on the last date of the preceeding year </div>
						<div class="panel-body">'
	);
	
	
	
	$form['mtw_totalvehicle'] = array(
		'#title' 			=> '6) Total Number of Motor Transport / Vehicles on the last date of the preceeding year',
		'#type' 			=> 'textfield',
		'#attributes' 		=> $readonly_mtw_totalvehicle,
		'#default_value'	=> $total_mtw_vehicle,
		'#element_validate' => array('REGISTRATION__ONLY_NUMERIC_VALIDATE'),
		'#required' 		=>  TRUE,
		'#prefix' 			=> '<div class="row"><div class="col-lg-4 col-md-6"><label class="input">',
		'#suffix' 			=> '</label></div></div>',
		'#ajax'				=>	array(
									'event'=> 'keyup',
									'effect'=> 'fade',
									'callback'=> 'add_reg_number_ajax_callback'
								)
	);
	
	$form['reg_number'] = array(
		'#prefix' 			=> '<div id="renum_div">',
		'#suffix' 			=> '</div>'
	);
	
	$vehicleNumber = isset($form_state['values']['mtw_totalvehicle']) ? $form_state['values']['mtw_totalvehicle'] : $total_mtw_vehicle;
	
	$vehiRegArray = explode('??',$vreg_number_details);
	
	if($vehicleNumber!=0){
		for($j = 1; $j<= $vehicleNumber; $j++){
			$form['reg_number']['singl_'.$j] = array(
				'#title'			=> '6.'.$j.') Motor Vehicle Registration Number',
				'#type'				=> 'textfield',
				'#default_value'	=> $vehiRegArray[$j-1],
				'#attributes'		=> array('autocomplete' => 'off','class'=>array(''),'placeholder'=>'Registration Number of Motor Trasport Vehicle'),
				'#prefix' 			=> '<section class="col-md-4"><div id="renum_div"><label class="input">',
				'#suffix' 			=> '</label></div></section>'
			);
		}
	}
	
	$form['markup9'] = array(
		'#type'				=> 'markup',
		'#markup'			=> '</div></div>'
	);

	/******** route details and reg number ***************/
	
	$form['markup10'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="panel panel-default panel-form">
						<div class="panel-heading">Number of Motor Transport Worker employed on any day during the preceeding year</div>
						<div class="panel-body">'
	);
	
	$form['mtw_maxworkers'] = array(
		'#title' 			=> '7) Maximum number motor transport workers employed on any day during the preceeding year',
		'#type' 			=> 'textfield',
		'#default_value'	=> $mtw_maxworkers,
		'#attributes' 		=> $readonly_mtw_maxworkers,
		'#required'   		=>  TRUE,
		'#element_validate' => array('REGISTRATION__ONLY_NUMERIC_VALIDATE'),
		'#prefix' 			=> '<div class="row"><div class="col-lg-4 col-md-6"><label class="input">',
		'#suffix' 			=> '</label></div></div>'
	);
	
	 
	 $form['markup11'] = array(
		'#type'				=> 'markup',
		'#markup'			=> '</div></div>'
	);

	 /******  details of mtw end ********/
    
	$form['application_id'] = array(
		'#type'     => 'hidden',
		'#value'    => $application_id
	);
	
	$form['back_db'] = array(
		'#type' => 'markup',
		'#markup' => l(t('Dashboard'),'applicant-dashboard',array('attributes' => array('html'=>TRUE , 'title'=>'Click for Dashboard', 'class' => array('btn btn-primary')))),
	);
	
	$form['skip_button'] = array(
		'#type'		=> 'markup',
		'#markup'	=> l(t('Next'),'mtw-registration-details/addmore/'.$id,array('attributes' => array('html'=>TRUE , 'title'=>'Click for Next Tab', 'class' => array('btn btn-primary mleft-15')))),
	);
	
	
	if($status == '' || $status == 'B'){
		$form['submit_button'] = array(
			'#type'		=>'submit',
			'#value'	=>t('Save & Continue'),
			'#prefix' 	=> '',
			'#suffix' 	=> '',
			'#attributes' 	=> array('class' => array('btn btn-primary pull-right')),
		);
	} else {
		$form['motor_markup2'] = array(
			'#type'	  => 'markup',
			'#markup' => ''
		);
	}
	return $form;														
}
function mtw_application_details_validate($form, &$form_state){
    $val 					= $form_state['values'];
	$mtw_name1				= $val['mtw_name'];
	if(empty($mtw_name1)){
		form_error($form['mtw_name'], t('Motor Transport Undertaking Name cannot be blank.'));
	}
	$mtw_loc_address		= $val['mtw_location'];
	if($mtw_loc_address==""){
		form_error($form['mtw_location'], t('Motor Transport Undertaking Location cannot be blank.'));
	}
	$mtw_loc_dist			= $val['mtw_district'];
	if($mtw_loc_dist==""){
		form_error($form['mtw_district'], t('District cannot be blank.'));
	}
	$mtw_loc_subdivision	= $val['mtw_subdivision'];
	if($mtw_loc_subdivision==""){
		form_error($form['mtw_subdivision'], t('Sub-Division cannot be blank.'));
	}
	$mtw_loc_areatype		= $val['mtw_areatype'];
	if($mtw_loc_subdivision==""){
		form_error($form['mtw_subdivision'], t('Sub-Division cannot be blank.'));
	}
	$mtw_loc_areatype_code	= $val['mtw_block'];
	if($mtw_loc_areatype_code==""){
		form_error($form['mtw_block'], t('Select Block/Municipality/Corporation/SEZ/Notified Area cannot be blank.'));
	}
	
	$mtw_loc_vill_ward	= isset($val['mtw_panchyat'])?$val['mtw_panchyat']:0;
	if($mtw_loc_vill_ward==""){
		form_error($form['mtw_panchyat'], t('Select Block/Municipality/Corporation/SEZ/Notified Area cannot be blank.'));
	}
	$mtw_loc_ps			= $val['mtw_policestation'];
	if($mtw_loc_ps==""){
		form_error($form['mtw_policestation'], t('Police Station cannot be blank.'));
	}
	$mtw_loc_pincode	= $val['mtw_pincode'];
	
	
	$mtw_nature			= $val['mtw_nature'];
	$total_routes		= $val['mtw_totalroute'];
	$total_route_milage	= $val['mtw_totalmilage'];
	$total_mtw_vehicle	= $val['mtw_totalvehicle'];
	$mtw_maxworkers		= $val['mtw_maxworkers'];
}
function REGISTRATION_TEXTBOX_TEXT_VALIDATE($element,&$form_state){
	$txtValue		=	trim($element['#value']); 
	$match_pattern 	= 	'/^[a-zA-Z0-9. ]+$/';
	if(!preg_match($match_pattern, $txtValue)) {
		form_error($element, t($element['#title'].' should be text only'));
	}
}
function REGISTRATION__TEXTBOX_NUMERIC_VALIDATE($element,&$form_state){
	$fieldValue=trim($element['#value']); 
	$match_pattern = '/^[0-9]+$/i';
	
	if(!empty($fieldValue)){
		if(!preg_match($match_pattern, $fieldValue)) {
			form_error($element, t($element['#title'].' should be number only'));
		}
		if(strlen($fieldValue)!=6){
			form_error($element, t($element['#title'].' should be 6 digit only'));
		}
	}
}
function REGISTRATION__ONLY_NUMERIC_VALIDATE($element,&$form_state){
	$fieldValue=trim($element['#value']); 
	$match_pattern = '/^[0-9]+$/i';
	
	if(!empty($fieldValue)){
		if(!preg_match($match_pattern, $fieldValue)) {
			form_error($element, t($element['#title'].' should be number only'));
		}
	}
}
function mtw_application_details_submit($form, &$form_state){
	
	global $base_root, $base_path, $user; 

	$val 					= $form_state['values'];
	
    $application_id         = $val['application_id'];
        
	$mtw_name				= $val['mtw_name'];
	$mtw_loc_address		= $val['mtw_location'];
	$mtw_loc_dist			= $val['mtw_district'];
	$mtw_loc_subdivision	= $val['mtw_subdivision'];
	$mtw_loc_areatype		= $val['mtw_areatype'];
	$mtw_loc_areatype_code	= $val['mtw_block'];
	$mtw_loc_vill_ward		= isset($val['mtw_panchyat'])?$val['mtw_panchyat']:0;
	$mtw_loc_ps				= $val['mtw_policestation'];
	$mtw_loc_pincode		= $val['mtw_pincode'];
	
	$mtw_nature				= $val['mtw_nature'];	
	if($mtw_nature=='other'){
		$mtw_nature			= $val['nature_other'];
	}
	
	
	$total_routes			= $val['mtw_totalroute'];
	//$total_route_milage		= $val['mtw_totalmilage'];
	$mtw_totalvehicle		= $val['mtw_totalvehicle'];
	
	$mtw_maxworkers			= $val['mtw_maxworkers'];
	$mtw_route_details		= $val['mtw_route_details'];
	$insert_val				= $val['insert_hidden'];
	
	
	// multiple registration number
	
	if($mtw_totalvehicle!=''){
		$tempString	='';
		for($j=1;$j<=$mtw_totalvehicle;$j++){
			$tempString .= $val['singl_'.$j].'??';
		}
	}
	
	
		// update array
		$updateArray	= array(
				'identification_number' => 'MTW'.time(),
				'act_id'				=> 3,
				'user_id'               => $user->uid,
                                
				'mtw_name'				=> $mtw_name,
				'mtw_loc_address'		=> $mtw_loc_address,
				'mtw_loc_dist'			=> $mtw_loc_dist,
				'mtw_loc_subdivision'	=> $mtw_loc_subdivision,
				'mtw_loc_areatype'		=> $mtw_loc_areatype,
				'mtw_loc_areatype_code'	=> $mtw_loc_areatype_code,
				'mtw_loc_vill_ward'		=> $mtw_loc_vill_ward,
				'mtw_loc_ps'			=> $mtw_loc_ps,
				'mtw_loc_pincode'		=> $mtw_loc_pincode,
				
				'mtw_nature'			=> $mtw_nature,
				'total_routes'          => $total_routes,
				'vreg_number_details'	=> $tempString,				
				'total_mtw_vehicle'		=> $mtw_totalvehicle,
				'mtw_maxworkers'		=> $mtw_maxworkers,	
		);
    //echo $application_id;
	//print_r($updateArray);
	
	    
	if( !empty($updateArray) ){
			
		$row = db_update('l_mtw_registration_master')->fields($updateArray)->condition('id',$application_id)->execute(); 
		
		
	
		if($total_routes!=0){
			for($i=1;$i<=$total_routes;$i++){
				$updateArray	=	array(
										'application_id'	=> $application_id,
										'route'				=> $val['route_edit_'.$i],
										'milage'			=> $val['milage_edit_'.$i],
										
									);
				db_update('l_mtw_registration_extended')->fields($updateArray)->condition('application_id',$application_id)->condition('id',$val['row_id_'.$i])->execute();
			}			
		}
		if($insert_val !=0 ){
			for($j=1; $j<=$insert_val; $j++ ){
				$insertArray3	=	array(
										'application_id'	=> $application_id,
										'route'				=> $val['route_'.$j],
										'milage'			=> $val['milage_'.$j],
										
									);
									
								
				db_insert('l_mtw_registration_extended')->fields($insertArray3)->execute();
			}
		
		}
		//die('ok');
		if($row != ''){
		    $message	=	'Application Saved Successfully';
		    drupal_set_message(t($message));
		    drupal_goto('mtw-registration-details/addmore/'.encryption_decryption_fun('encrypt', $application_id));
		}else{
            $message	=	'Application Not Saved ! Please Try Again';
		    drupal_set_message(t($message),'error');
			drupal_goto('mtw-registration-details/default/'.encryption_decryption_fun('encrypt', $application_id));
        }
	}	
}

function get_mtw_registration_info_form_ajax_callback($form, $form_state) {
		$commands = array();
		$commands[] = ajax_command_replace('#mtw_subdvsn_replace', drupal_render( $form['mtw_subdivision']));
		$commands[] = ajax_command_replace('#mtw_loc_e_areatype', drupal_render($form['mtw_areatype']));
		$commands[] = ajax_command_replace('#mtw_common_1_replace', drupal_render($form['mtw_block']));
		$commands[] = ajax_command_replace('#mtw_common_2_replace', drupal_render($form['mtw_panchyat']));
		$commands[] = ajax_command_replace('#mtw_ps_replace', drupal_render($form['mtw_policestation']));
		
		
		
		
  		return array('#type' => 'ajax', '#commands' => $commands);
}
function get_propreitor_info_form_ajax_callback($form, $form_state) {
		$commands = array();
		$commands[] = ajax_command_replace('#propritor_subdvsn_replace', drupal_render( $form['mtw_propritorsubdivision']));
		$commands[] = ajax_command_replace('#propritor_areatype_replace', drupal_render($form['mtw_propritorareatype']));
		$commands[] = ajax_command_replace('#propritor_block_replace', drupal_render($form['mtw_propritorblock']));
		$commands[] = ajax_command_replace('#propritor_panchyat_replace', drupal_render($form['mtw_propritorpanchyat']));
		$commands[] = ajax_command_replace('#propritor_ps_replace', drupal_render($form['mtw_propritorpolicestation']));
		
  		return array('#type' => 'ajax', '#commands' => $commands);
}
function get_gm_info_ajax_callback($form, $form_state) {
		$commands = array();
		$commands[] = ajax_command_replace('#gm_subdvsn_replace', drupal_render( $form['mtw_gmsubdivision']));
		$commands[] = ajax_command_replace('#gm_areatype_replace', drupal_render($form['mtw_gmareatype']));
		$commands[] = ajax_command_replace('#gmblock_replace', drupal_render($form['mtw_gmblock']));
		$commands[] = ajax_command_replace('#gmpanchyat_replace', drupal_render($form['mtw_gmpanchyat']));
		$commands[] = ajax_command_replace('#gmpolice_replace', drupal_render($form['mtw_gmpolicestation']));
		
  		return array('#type' => 'ajax', '#commands' => $commands);
}
function get_director_info_ajax_callback($form, $form_state) {
		$commands = array();
		$commands[] = ajax_command_replace('#director_subdvsn_replace', drupal_render( $form['mtw_directorssubdivision']));
		$commands[] = ajax_command_replace('#director_areatype_replace', drupal_render($form['mtw_directorsareatype']));
		$commands[] = ajax_command_replace('#directorsblock_replace', drupal_render($form['mtw_directorsblock']));
		$commands[] = ajax_command_replace('#directorpanchyat_replace', drupal_render($form['mtw_directorspanchyat']));
		$commands[] = ajax_command_replace('#director_ps_replace', drupal_render($form['mtw_directorspolicestation']));
		
		
  		return array('#type' => 'ajax', '#commands' => $commands);
}
function get_other_div_ajax_callback($form, $from_state){
		$data = array();
		$data[]	= ajax_command_replace('#nature_other_div',drupal_render($form['nature_other']));
		return array('#type' => 'ajax', '#commands' => $data);
}
function add_more_ajax_callback($form, &$form_state){
		$commands = array();
		$commands[] = ajax_command_replace('#multiple_div', drupal_render($form['mtw_route_details']));
		$commands[] = ajax_command_replace('#multiple_div_milage', drupal_render($form['mtw_totalmilage']));
		return array('#type' => 'ajax', '#commands' => $commands);
}
function add_reg_number_ajax_callback($form, &$form_state){
		$commands	=	array();
		$commands[]	=	ajax_command_replace('#renum_div',drupal_render($form['reg_number']));
		return array('#type' => 'ajax', '#commands' => $commands);	
}
