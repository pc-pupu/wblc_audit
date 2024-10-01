<?php
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
drupal_add_js(drupal_get_path('module', 'amendment_license_renewal') . '/js/checklabournumber.js');

function labour_worksite_info($form, &$form_state ,$serialno='' ,$renewallicenseammendment_id= '',$applicationflag= ''){
		global $base_root, $base_path, $user;
		$userid = $user->uid;
	drupal_add_library('system','ui.datepicker');
	
	
	$readonly_contarctor_dob 		= array( 'class' => array('ammendment_date'), 'onclick' => 'datepicker_lastrenewal();','readonly' => 'readonly',);
	$serial_no						=  encryption_decryption_fun('decrypt', $serialno);
	$renewal_license_ammendment_id	=  encryption_decryption_fun('decrypt', $renewallicenseammendment_id);
	$application_flag				=  encryption_decryption_fun('decrypt', $applicationflag);
	//$check_ammendment_table         =  check_ammendment_table($renewal_license_ammendment_id);
	//$check_ammendment_table 		= check_ammendment_table($renewal_license_ammendment_id);			
	$contractor_regmaster_details 	= contractor_regmaster_details($serial_no);
	if(!empty($contractor_regmaster_details)){
		$name_of_contractor 	 = 	$contractor_regmaster_details['name_of_contractor'];
		$application_id			 = 	$contractor_regmaster_details['id'];
		$pe_userid				 = 	$contractor_regmaster_details['user_id'];
		$max_no_of_labours_pe	 = 	$contractor_regmaster_details['contractor_max_no_of_labours_on_any_day'];
	}
	
	/*$form['labour_worksite_details']	= array(
				'#title' 				=>	 t('<div class="content"><div class="sky-form" style="width:100%;"><header><h3> </h3></header>'),
				'#type' 				=> 	'fieldset',
				'#attributes' 			=> 	array('class'=>array('')),
				'#collapsed' 			=> 	TRUE,
				'#collapsable'			=>	TRUE
				);
				*/
				
	/*$form['labour_worksite_details']	= array(
				'#title' 				=>	 t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading"></div>'),
				//'#type' 				=> 	'fieldset',
				'#attributes' 			=> 	array('class'=>array('')),
				'#collapsed' 			=> 	TRUE,
				'#collapsable'			=>	TRUE
				);	*/

				
	if($application_flag == 'L'){
		$license_id						=	$renewal_license_ammendment_id;//---kaj baki
	}if($application_flag == 'A'){
		$license_id						=	return_ammendement_license_id($renewal_license_ammendment_id);//---kaj baki
	}if($application_flag == 'R'){
		$license_id						=	return_renewal_license($renewal_license_ammendment_id);//---kaj baki
	}	
	if($application_flag == 'L'){
		
		$particular_data = particular_data($serial_no);
		
		
		if(!empty($particular_data)){
			
			$contractor_max_labours_license_renewal		= $particular_data['contractor_max_no_of_labours_on_any_day'];
			$worksite_address_line_license_renewal		= $particular_data['worksite_address_line'];
			$worksite_dist_license_renewal				= $particular_data['worksite_dist'];
			$work_site_areatype_license_renewal			= $particular_data['work_site_areatype'];
			$worksite_subdivision_license_renewal		= $particular_data['worksite_subdivision'];
			$name_work_site_areatype_license_renewal	= $particular_data['name_work_site_areatype'];
			$work_site_vill_ward_license_renewal		= $particular_data['work_site_vill_ward'];
			$worksite_pin_license_renewal				= $particular_data['worksite_pin'];
			$worksite_ps_license_renewal				= $particular_data['worksite_ps'];
			$max_no_of_labours_license_renewal			= $particular_data['contractor_max_no_of_labours_on_any_day'];
			$is_coparative_license_renewal				= $particular_data['is_coparative'];
			$amount_of_licence_renewal					= $particular_data['amount_of_licence'];
			$amount_of_security_deposit_licence_renewal	= $particular_data['amount_of_security_deposit'];
			$courrent_deposit_fees_amount				= $particular_data['amount_of_licence'];
			$amount_of_licence_time						= $particular_data['amount_of_licence'];
			//$payble_security_fees_license_renewal		= $particular_data['amount_of_security_deposit'];
			
		}
	}

	
  if($application_flag == 'R'){
	  $renewal_data 									= 	renewal_data_last($renewal_license_ammendment_id);
	  $particular_data 									= 	particular_data($serial_no);
	
	  $max_no_of_labours_license_renewal				= 	$renewal_data['max_of_contract_labour'];
	  $payble_security_fees_license_renewal				=	$renewal_data['payble_security_fees'];	
	  $balance_securit_amount_license_renewal			=	$renewal_data['balance_securit_amount'];
	  $due_security_fees_amount_license_renewal			=	$renewal_data['due_security_fees'];	
	  $amount_of_licence_renewal						=	$renewal_data['license_reneal_fees'];	
	  $payble_security_fees_license_renewal				=	$renewal_data['renewal_security_new'];
	  $contractor_max_labours_license_renewal			= 	$particular_data['contractor_max_no_of_labours_on_any_day'];
	  $worksite_address_line_license_renewal			= 	$particular_data['worksite_address_line'];
	  $worksite_dist_license_renewal					= 	$particular_data['worksite_dist'];
	  $work_site_areatype_license_renewal				= 	$particular_data['work_site_areatype'];
	  $worksite_subdivision_license_renewal				= 	$particular_data['worksite_subdivision'];
	  $name_work_site_areatype_license_renewal			= 	$particular_data['name_work_site_areatype'];
	  $work_site_vill_ward_license_renewal				= 	$particular_data['work_site_vill_ward'];
	  $worksite_pin_license_renewal						= 	$particular_data['worksite_pin'];
	  $worksite_ps_license_renewal						= 	$particular_data['worksite_ps'];
	 
	 
  }
  
  	$max_renewal_fees 	=  return_maximum_renewal_fees($license_id,$userid);
	$max_ammend_fees 	=  return_maximum_ammendment_fees($license_id,$userid);
	$max_license_fees	=  return_maximum_license_fees($serial_no,$userid);
	
	
	if($max_renewal_fees>$max_ammend_fees)
	    $max_fees1	= $max_renewal_fees;
    if($max_renewal_fees<$max_ammend_fees)
	    $max_fees1	= $max_ammend_fees;	 
	if($max_renewal_fees == $max_ammend_fees)
	    $max_fees1	= $max_ammend_fees;	 
	if($max_fees1>$max_license_fees)
		$max_fees2 = $max_fees1;
	if($max_fees1<$max_license_fees)
		$max_fees2 = $max_license_fees;
	if($max_fees1 == $max_license_fees)
		$max_fees2 = $max_license_fees;
		
		
	$maximum_renewal_labour = return_maximum_renewal_labour($license_id,$userid);
	$maximum_ammend_labour	= return_maximum_ammendment_labour($license_id,$userid);
	$maximum_license_labour = return_maximum_license_labour($serial_no,$userid);
	
	 
	 if($maximum_renewal_labour>$maximum_ammend_labour)
	    $max_labour1	= $maximum_renewal_labour;
		
    if($maximum_renewal_labour<$maximum_ammend_labour)
	    $max_labour1	= $maximum_renewal_labour;	
		 
	if($maximum_renewal_labour == $maximum_ammend_labour)
	    $max_labour1	= $maximum_ammend_labour;	
		 
	if($max_labour1>$maximum_license_labour)
		$max_labour2 = $max_labour1;
		
	if($max_labour1<$maximum_license_labour)
		$max_labour2 = $maximum_license_labour;
		
	if($max_labour1 == $maximum_license_labour)
		$max_labour2 = $maximum_license_labour;
		
	//$check_ammendment_table 			 				= 	check_ammendment_table_license_renewal_ammendment_id($renewal_license_ammendment_id);	
	//$details_ammendment_table 			 				=  $check_ammendment_table->fetchAssoc();
	$details_ammendment_table 			 				= check_ammendment_table_license_renewal_ammendment_id($renewal_license_ammendment_id);	

	
	//$max_of_contract_labour 			= !empty($details_ammendment_table['max_of_contract_labour'])?$details_ammendment_table['max_of_contract_labour']:$max_labour2;
	$worksite_address_line				= !empty($details_ammendment_table['worksite_address_line'])?$details_ammendment_table['worksite_address_line']:$worksite_address_line_license_renewal;
	$worksite_dist						= !empty($details_ammendment_table['worksite_dist'])?$details_ammendment_table['worksite_dist']:$worksite_dist_license_renewal; 
	$work_site_areatype					= !empty($details_ammendment_table['work_site_areatype'])?$details_ammendment_table['work_site_areatype']:$work_site_areatype_license_renewal;
	$worksite_subdivision				= !empty($details_ammendment_table['worksite_subdivision'])?($details_ammendment_table['worksite_subdivision']):$worksite_subdivision_license_renewal;
	$name_work_site_areatype			= !empty($details_ammendment_table['name_work_site_areatype'])?$details_ammendment_table['name_work_site_areatype']:$name_work_site_areatype_license_renewal;
	$work_site_vill_ward				= !empty($details_ammendment_table['work_site_vill_ward'])?$details_ammendment_table['work_site_vill_ward']:$work_site_vill_ward_license_renewal;
	$worksite_pin						= !empty($details_ammendment_table['worksite_pin'])?$details_ammendment_table['worksite_pin']:$worksite_pin_license_renewal;

	$worksite_ps						= !empty($details_ammendment_table['worksite_ps'])?$details_ammendment_table['worksite_ps']:$worksite_ps_license_renewal;
	$pe_userid							= !empty($details_ammendment_table['user_id'])?$details_ammendment_table['user_id']:$pe_userid;
	$application_id						= !empty($details_ammendment_table['application_id'])?$details_ammendment_table['application_id']:$application_id;
	$reference_no						= $details_ammendment_table['reference_no'];
	
	
	$is_coparative						= !empty($details_ammendment_table['is_coparative'])?$details_ammendment_table['is_coparative']:(!empty($is_coparative_license_renewal)?$is_coparative_license_renewal:0);
	$max_of_contract_labour_hidden		= !empty($details_ammendment_table['max_of_contract_labour'])?$details_ammendment_table['max_of_contract_labour']: $max_no_of_labours_license_renewal;
	//$max_of_contract_labour				= !empty($details_ammendment_table['max_of_contract_labour'])?$details_ammendment_table['max_of_contract_labour']:(!empty($max_no_of_labours_license_renewal)?$max_no_of_labours_license_renewal:$max_no_of_labours_pe);
	if($ammendment_status == 'I'){
		$max_of_contract_labour	 = !empty($details_ammendment_table['max_of_contract_labour'])?$details_ammendment_table['max_of_contract_labour']:(!empty($max_no_of_labours_license_renewal)?$max_no_of_labours_license_renewal:$max_no_of_labours_pe);	
	}if((empty($ammendment_status) || $ammendment_status == 'B' )){
    		$max_of_contract_labour	 = !empty($details_ammendment_table['max_of_contract_labour'])?$details_ammendment_table['max_of_contract_labour']:(!empty($max_no_of_labours_license_renewal)?$max_no_of_labours_license_renewal:$max_no_of_labours_pe);
		//$max_of_contract_labour	 = !empty($max_no_of_labours_license_renewal)?$max_no_of_labours_license_renewal:$max_no_of_labours_pe;	
				
	}

	$license_renewal_fees				= !empty($details_ammendment_table['license_renewal_fees'])?$details_ammendment_table['license_renewal_fees']:$amount_of_licence_renewal;
	$license_renewal_security_fees   	= !empty($details_ammendment_table['license_renewal_security_fees'])?$details_ammendment_table['license_renewal_security_fees']:$amount_of_security_deposit_licence_renewal;
	
	//$ammend_deposit_fees				= $details_ammendment_table['ammend_deposit_fees'];
	$deposited_max_security_fees		= $details_ammendment_table['deposited_max_security_fees'];	
	$ammend_deposit_fees				= $details_ammendment_table['previous_highest_deposit_fees'];
  	$supportive_documents				= $details_ammendment_table['supportive_documents_worksite'];
	$ammendment_status 					=  $details_ammendment_table['ammendment_status'];
	
    $supportive_documents_worksite		= fetch_uplaoded_file_details($supportive_documents);
	if(!empty($supportive_documents_worksite['fid'])){
			$url				= explode('//',$supportive_documents_worksite['uri']);
   			$document_url		= $url[1];
			$document_file		= $supportive_documents_worksite['filename']; 
			$view_file			= '<span><a title = "Click here to view Supportive Document" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$document_url.'" target="_blank">View supportive Document in favour of changing worksite address </a></span>';
	}
	
	//---------------------------------------------fees calcution for ammendment------------------------------------------------
	
	if($max_labour2>$max_of_contract_labour)
				$highest_labour = $max_labour2;
	if($max_labour2<$max_of_contract_labour)
			$highest_labour = $max_of_contract_labour;
	if($max_labour2 == $max_of_contract_labour)
			$highest_labour = $max_of_contract_labour;
			
		
			
		//echo $highest_labour;die();
	$ammendent_fees						 	=   applicable_ammendment_fees_calculation($max_of_contract_labour);
	
	if($max_fees2>$ammend_deposit_fees){
		$currecnt_deposit_fees = $max_fees2;
	}if($max_fees2<$ammend_deposit_fees){
		$currecnt_deposit_fees = $ammend_deposit_fees;
	}if($max_fees2 == $ammend_deposit_fees){
		$currecnt_deposit_fees = $ammend_deposit_fees;
	}
	
	/*if($highest_labour>400){
		$paymantable_ammenedent_fees 	= 0.00;
		$deposit_fees_value				= $currecnt_deposit_fees;
		
		
	}if($highest_labour<=400){*/
		
	//}

		
	$applicable_amendment_fee   	= !empty($details_ammendment_table['applicable_amendment_fee'])?$details_ammendment_table['applicable_amendment_fee']:!empty($ammendent_fees)?$ammendent_fees:'0';
	
	
	if(empty($ammendment_status) || $ammendment_status == 'I'){
	$previous_deposit_fees   		= $currecnt_deposit_fees;
	
	}
	else{
			$previous_deposit_fees   		= !empty($details_ammendment_table['previous_highest_deposit_fees'])?$details_ammendment_table['previous_highest_deposit_fees']:$currecnt_deposit_fees;
			
	}

	if($previous_deposit_fees>$ammendent_fees){
			$paymantable_ammenedent_fees 	= 0.00;
			$deposit_fees_value				= $previous_deposit_fees;
		}if($previous_deposit_fees<$ammendent_fees){
			  $paymantable_ammenedent_fees 	= $ammendent_fees-$previous_deposit_fees; 
			  $deposit_fees_value			= $ammendent_fees;
		}if($previous_deposit_fees==$ammendent_fees){
			 $paymantable_ammenedent_fees 	= 0.00;
			 $deposit_fees_value			= $ammendent_fees;
		}	
		//echo $paymantable_ammenedent_fees;die();
	//$payable_amendment_fees   		= !empty($details_ammendment_table['payable_amendment_fees'])?$details_ammendment_table['payable_amendment_fees']:$paymantable_ammenedent_fees;
	$payable_amendment_fees   		= $paymantable_ammenedent_fees;
	$balance_fess   				= $balance_fess1;
	$reason_changing_worksite   	= !empty($details_ammendment_table['reason_changing_worksite'])?$details_ammendment_table['reason_changing_worksite']:'Nil';
	
		
	$form['labour_worksite_details']['currecnt_deposit_fees'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($previous_deposit_fees) ? encryption_decryption_fun('encrypt', $previous_deposit_fees):'0.00',
				'#attributes' 		=> 	array('id' => 'currecnt_deposit_fees'),
												
		 );
	$form['labour_worksite_details']['max_labour_previous'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($highest_labour) ? encryption_decryption_fun('encrypt', $highest_labour):'0.00',
				'#attributes' 		=> 	array('id' => 'max_labour_previous'),
												
		 );
	$form['labour_worksite_details']['deposit_fees_value'] = array(
				
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($deposit_fees_value) ? encryption_decryption_fun('encrypt', $deposit_fees_value):'0.00',
				'#attributes' 		=> 	array('id' => 'deposit_fees'),
												
		 );	

	$form['labour_worksite_details']['max_of_contract_labour_hidden'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($max_of_contract_labour_hidden) ? encryption_decryption_fun('encrypt', $max_of_contract_labour_hidden):'0.00',
				'#attributes' 		=> 	array('id' => 'max_of_contract_labour_hidden'),
				
				
										
		 );	
	$form['labour_worksite_details']['ammendment_status'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($ammendment_status) ? encryption_decryption_fun('encrypt', $ammendment_status):'',
				
				
													
		 );	
	
	$form['labour_worksite_details']['renewallicenseammendment_id'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($renewallicenseammendment_id) ? $renewallicenseammendment_id:'',
				
				
													
		 );	
	

	$form['labour_worksite_details']['applicationflag'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($applicationflag) ? $applicationflag:'',
				'#attributes' 		=> 	array('id' => 'application_flag_id'),
				
				
				
													
		 );	
	$form['labour_worksite_details']['serialno'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($serialno) ? $serialno:'',
				'#attributes' 		=> 	array('id' => 'serialno_id'),
				
				
													
		 );				 
	$form['labour_worksite_details']['application_id'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($application_id) ?  encryption_decryption_fun('encrypt', $application_id):'',
				
				
													
		 );	
	$form['labour_worksite_details']['pe_userid'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($pe_userid) ?  encryption_decryption_fun('encrypt', $pe_userid):'',
				
				
													
		 );	
	$form['labour_worksite_details']['reference_no'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($reference_no) ?  encryption_decryption_fun('encrypt', $reference_no):'',
				
				
													
		 );	
	

	
	
	$last_remark_activity 					= last_remark_activity($reference_no);
	$remark_field_title 					= $last_remark_activity['remark_field_title'];
    $remark_type 							= $last_remark_activity['remark_type'];
    $readonly_ammendment_fees 				= array('class' => array('readonly_view'),'readonly' => 'readonly');
	$readonly_security_fees 				= array('class' => array('readonly_view security_fees'),'readonly' => 'readonly');
	$readonly_due_security_fees 			= array('class' => array('readonly_view due_security_fees'),'readonly' => 'readonly');
	$readonly_payble_security_fees 			= array('class' => array('readonly_view payble_security_fees'),'readonly' => 'readonly');
	$readonly_security_balance 				= array('class' => array('readonly_view security_balance_update'),'readonly' => 'readonly');
	$readonly_previous_security_fees 		= array('class' => array('readonly_view'),'readonly' => 'readonly');
	$readonly_labour						= array('class' => array('nolabour'),'onkeyup' => 'checkLabourNumber(this.val);');
	
	 
	if((empty($remark_field_title) && $remark_type =='B')|| (!empty($remark_field_title) && $remark_type =='B')){
			   $readonly_worksite_address_line1	 	= array('placeholder' => 'Enter your name','class' => array('correction_show'));
			   $readonly_worksite_dist 				= array('placeholder' => 'Enter your Father Name', 'class' => array('correction_show'));
			   $readonly_reason 					= array('class' => array('correction_show'));
			   $readonly_no_labour 					= array('class' => array('correction_show'));
			
			   $readonly_supportive			 		= array('class' => array('correction_show'));
			   $edit 								= 2;
			   $readonly_supportive			 		= array('class' => array('correction_show'));
			   $readonly_labour						= array('class' => array('correction_show nolabour'),'onkeyup' => 'checkLabourNumber(this.val);');
			 			  
	 }if(!empty($remark_field_title) && $remark_type !='I'){
				
			   $remark_field_title = explode(',', $remark_field_title);	
				foreach($remark_field_title as $row){
					switch($row){
						case  "contractor_max_no_of_labours_on_any_day":
							    $readonly_labour	= array('class' => array('readonly_view nolabour'),'onkeyup' => 'checkLabourNumber(this.val);','readonly' => 'readonly');
							 break;
						case  "worksite_address":
							  $readonly_worksite_address_line1	 = array('class' => array('readonly_view'),'readonly' => 'readonly');
							  $readonly_worksite_dist 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							  $edit_worksite_address_line1 = 1;
							 break;
						case  "co_oparative":
							  $readonly_co_oparative	 = array('class' => array('readonly_view'),'readonly' => 'readonly');
							 $edit_co_oparative			  =1;
							 break;
						case  "change_reason":
							 $readonly_reason	 = array('class' => array('readonly_view'),'readonly' => 'readonly');
							
							break;
						case "supportive_documents":
						 $readonly_supportive	 = array('class' => array('readonly_view'));
							$edit = 1;
							break;
					
					}
				}
		   }
	$ammendment = check_ammendment_fields($license_id,$serial_no)->fetchAssoc()	;
	if(!empty($ammendment)){
		
		$amendment_check_field 			= $ammendment['amendment_check_field'];
		
		$form['labour_worksite_details']['amendment_check_field'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($amendment_check_field) ?  encryption_decryption_fun('encrypt', $amendment_check_field):'',
				
				
													
		 );	
		if(!empty($amendment_check_field)){
			$editable_field 		= explode(',', $amendment_check_field);
			foreach($editable_field as $row){
				if($row == 'contractor_max_no_of_labours_on_any_day'){
				
				
			$form['labour_worksite_details']['test'] 		= array(
						'#prefix' 				=> '<div class="panel panel-default panel-form"><div class="panel-heading"><center>Contractor Labour Details</center></div><div class="panel-body"><div class="sky-form" style="width:100%;"><div class="col-sm-12"><h4 class="form-sub-title">Details of maximum contarct labour on any day</h4></div>',
					
				   );
				   
		if($edit_co_oparative == 1){
			 $form['labour_worksite_details']['is_coparative'] = array(
					'#prefix' 			=> '<section class="col col-6 ammend-row"><div class="inline-group">',
					'#suffix' 			=> '</section>',
					'#title'				=>	'Co-oparative Society',
					'#type' 				=> 	'textfield',
					'#size'					=>	120,
					'#default_value' 		=>	!empty($is_coparative) ? 'Yes':'No' ,
					'#required' 			=> 	TRUE,
					'#attributes' 			=>  $readonly_co_oparative,
				);	
			
		}else{
				
		$form['labour_worksite_details']['is_coparative'] = array(
					'#prefix' 			=> '<section class="col col-md-6 ammend-row"><div class="inline-group">',
					'#suffix' 			=> '</section>',
					'#type' 			=> 'radios',
					'#title' 			=>  t('Co-oparative Society <span class="correction_show con_cate"></span>'),     
					'#options' 			=> array('1'=>t('Yes'),'0'=>t('No')),
					'#required' 		=> TRUE,
					'#default_value' 	=> !empty($is_coparative) ? $is_coparative: 0,
					'#attributes'       => array('onChange' => "check_coparetive();") 
					/*'#ajax'			    =>  array(
													'event' 			=> 'change',
													'effect' 			=> 'fade',			
													'callback'			=> 'get_is_coparative',
													'method'			=> 'replace',
													'progress'			=> '',
												 ) */
					);	
		}
		
		if($edit_worksite_address_line1 ==1){
			$form['labour_worksite_details']['worksite_address_lineone'] = array(
					'#prefix' 		=> '<section class="col col-6 ammend-row"><div class="inline-group">',
					'#suffix' 		=> '</section>',
					'#title'		=> 'Worksite Line1',
					'#type' 		=> 'textarea',
					'#default_value' =>	!empty($is_coparative) ? 'Yes':'No' ,
					'#required'   => TRUE,
					'#attributes' => $readonly_co_oparative,
				);
		}
		
		 $form['labour_worksite_details']['no_labour'] = array(
					'#prefix' 		   => '<section class="col col-md-6 ammend-row"><div id ="nolabour_id"><label class="input">',
					'#suffix' 		   => '</label></div>',
					'#title'		   => 'Max Contarct Labour on any day',
					'#type' 		   => 'textfield',
					'#size'			   => 120,
					'#default_value'   => !empty($max_of_contract_labour) ? $max_of_contract_labour:'' ,
					'#attributes' 	   => $readonly_no_labour,
					'#required' 	   => TRUE,
					'#element_validate' => array('isnumaric_valiadtion'),
					'#attributes' 		=> $readonly_labour
				);
				
	
	
					
		$form['labour_worksite_details']['username_img_markup'] 		= array(
						
						'#type' 			=> 'markup', 
						'#markup' 			=> '<span id="common_username_img_div"></span>'
				   );
															
		$form['labour_worksite_details']['username_markup'] 			= array(
					'#type' 			=> 'markup', 
					'#markup' 			=> '<span id="common_username_div"></span>',
					'#suffix' 			=> '</section>',
			   );
					
	$value_is_coparative 	= 	isset($form_state['values']['is_coparative']) ? $form_state['values']['is_coparative'] :$is_coparative;
	$value_contract_labour 	= 	isset($form_state['values']['no_labour']) ? $form_state['values']['no_labour'] :$max_of_contract_labour;
	
			
		//---------------------------------------------fees calcution for security------------------------------------------------
	$max_license_security_fees 			= return_maximum_license_security_fees($serial_no,$userid);
	$max_ammendment_security_fess       = return_maximum_ammendment_security_fess($license_id,$userid);
	$max_renewal_security_fess			= return_maximum_renewal_security($license_id,$userid);
	/*if($application_flag == 'R'){
		$max_renewal_security_fess = $max_renewal_security_fess;
	}if($application_flag == 'L'){
		$max_renewal_security_fess = $max_license_security_fees;
	}*/
	
	if($max_license_security_fees>$max_ammendment_security_fess){
		$max_security_fees1	=$max_license_security_fees;	 
		
	}if($max_license_security_fees<$max_ammendment_security_fess){
		$max_security_fees1	=$max_ammendment_security_fess;	 
		
	}if($max_license_security_fees==$max_ammendment_security_fess){
		$max_security_fees1	=$max_ammendment_security_fess;	 
		
	}if($max_security_fees1>$max_renewal_security_fess){
		$max_security_fees2 = $max_security_fees1;
	}
	if($max_security_fees1<$max_renewal_security_fess){
		$max_security_fees2 = $max_renewal_security_fess;
	}
	if($max_security_fees1==$max_renewal_security_fess){
		$max_security_fees2 = $max_renewal_security_fess;
	}
	if($max_security_fees2>$deposited_max_security_fees){
		$current_security_deposit = $max_security_fees2;
	}
	if($max_security_fees2<$deposited_max_security_fees){
		$current_security_deposit = $deposited_max_security_fees;
	}
	if($max_security_fees2 == $deposited_max_security_fees){
		$current_security_deposit = $deposited_max_security_fees;
	}

    $payable_amendment_security_fees_pre   		=  $details_ammendment_table['payable_amendment_security_fees'];
    
	if($ammendment_status == 'I')
	$previous_max_security 		= $current_security_deposit+$payable_amendment_security_fees_pre;
	
	if(empty($ammendment_status))
	$previous_max_security 		= $current_security_deposit;
	if($ammendment_status == 'B')
	$previous_max_security 		= $details_ammendment_table['previous_highest_security'];
	if(!empty($ammendment_status) && $ammendment_status=='I')
	//$previous_max_security   	= !empty($details_ammendment_table['previous_highest_security'])?$details_ammendment_table['previous_highest_security']:$current_security_deposit;
	//$previous_max_security   	=  $current_security_deposit;
	$ammendent_security_fees 	=  applicable_ammendment_security_fees_calculation($value_contract_labour,$value_is_coparative);
	
	
	if($ammendent_security_fees>$previous_max_security){
		$paymantable_sucurity_ammenedent_fees = $ammendent_security_fees-$previous_max_security;
		if(!empty($due_security_fees_amount_license_renewal) && $due_security_fees_amount_license_renewal!=0){
			
			$paymantable_sucurity_ammenedent_fees 		= $paymantable_sucurity_ammenedent_fees+$due_security_fees_amount_license_renewal;
			$deposited_security_fees					= $ammendent_security_fees;
		}else{
			$deposited_security_fees					= $ammendent_security_fees;
		}
	}if($ammendent_security_fees<$previous_max_security){
		$paymantable_sucurity_ammenedent_fees = 0.00;
		if(!empty($due_security_fees_amount_license_renewal) && $due_security_fees_amount_license_renewal!=0){
			
			$paymantable_sucurity_ammenedent_fees 		= $due_security_fees_amount_license_renewal;
			$deposited_security_fees					= $previous_max_security;
		}else{
			
			$deposited_security_fees					= $previous_max_security;
		}
	}if($ammendent_security_fees==$previous_max_security){
		$paymantable_sucurity_ammenedent_fees = 0.00;
		if(!empty($due_security_fees_amount_license_renewal) && $due_security_fees_amount_license_renewal!=0){
			
			$paymantable_sucurity_ammenedent_fees 		= $due_security_fees_amount_license_renewal;
			$deposited_security_fees					= $previous_max_security;
		}else{
			
			$deposited_security_fees					= $previous_max_security;
		}
	}
		
		
			$applicable_security_fees   			= !empty($details_ammendment_table['applicable_security_fess'])?$details_ammendment_table['applicable_security_fess']:!empty($ammendent_security_fees)?$ammendent_security_fees:'0';
			
			//$payable_amendment_security_fees   		=  !empty($details_ammendment_table['payable_amendment_security_fees'])?$details_ammendment_table['payable_amendment_security_fees']:$paymantable_sucurity_ammenedent_fees;
		$payable_amendment_security_fees   		=  $paymantable_sucurity_ammenedent_fees;
			
							
		$form['labour_worksite_details']['previous_security'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($previous_max_security) ? encryption_decryption_fun('encrypt', $previous_max_security):'0',
				'#attributes' 		=> 	array('id' => 'previous_security'),
														
		 );	
		 $form['labour_worksite_details']['deposited_security_fees'] = array(
				'#type' 			=> 'hidden',
				'#value'			=> !empty($deposited_security_fees) ? encryption_decryption_fun('encrypt', $deposited_security_fees):'0',
				'#attributes' 		=> 	array('id' => 'deposited_security'),
														
		 );	
								
		 $form['labour_worksite_details']['security_fees'] = array(
					'#prefix' 				=> '<div id ="security_fees_id"><section class="col col-md-6"><label class="input">',
					'#suffix' 				=> '</label></section></div>',
					'#title'				=>	'Applicable Security Fees',
					'#type' 				=> 	'textfield',
					'#size'					=>	120,
					'#default_value' 		=>	!empty($applicable_security_fees) ? $applicable_security_fees:'' ,
					'#required' 			=> 	TRUE,
					'#attributes' 			=>  $readonly_security_fees,
				);	
			
			$form['labour_worksite_details']['ammendment_fees'] = array(
					'#prefix' 				=> '<div id ="ammendment_fees_id"><section class="col col-md-6"><label class="input">',
					'#suffix' 				=> '</label></section></div>',
					'#title'				=>	'Applicable Ammendment Fees',
					'#type' 				=> 	'textfield',
					'#size'					=>	120,
					'#default_value' 		=>	!empty($applicable_amendment_fee) ? $applicable_amendment_fee:'' ,
					'#required' 			=> 	TRUE,
					'#element_validate'		=>  array('isnumaric_valiadtion'),
					'#attributes'			=>	$readonly_ammendment_fees,
				);
				
			$form['labour_worksite_details']['due_security_fees'] = array(
					'#prefix' 				=> '<div id="due_security_fees_view"><section class="col col-md-6"><label class="input">',
					'#suffix' 				=> '</label></section></div>',
					'#title'				=>	'Due Security Fees',
					'#type' 				=> 	'textfield',
					'#size'					=>	120,
					'#default_value' 		=>	!empty($due_security_fees_amount_license_renewal) ? $due_security_fees_amount_license_renewal:'0.0' ,
					'#required' 			=> 	TRUE,
					'#element_validate'		=> array('isnumaric_valiadtion'),
					'#attributes' 			=> $readonly_due_security_fees,
				);
				
			$form['labour_worksite_details']['payable_amendment_fees'] = array(
					'#prefix' 				=> '<div id="payable_amendment_fees_id"><section class="col col-md-6"><label class="input">',
					'#suffix' 				=> '</label></section></div>',
					'#title'				=>	'Payble Ammendment Fees',
					'#type' 				=> 	'textfield',
					'#size'					=>	120,
					'#default_value' 		=>	!empty($payable_amendment_fees) ? $payable_amendment_fees:'0.00' ,
					'#attributes' 			=> 	array('placeholder' => 'Address Line1 the contractor'),
					'#required' 			=> 	TRUE,
					'#element_validate'		=> array('isnumaric_valiadtion'),
					'#attributes'			=>	$readonly_ammendment_fees,
				);
		  $form['labour_worksite_details']['payble_security_fees'] = array(
					'#prefix' 				=> '<div id="payble_security_fees_id"><section class="col col-md-6"><label class="input">',
					'#suffix' 				=> '</label></section></div>',
					'#title'				=>	'<span style="font-color:#FF0000;">Payble Ammendment security Fees</span>',
					'#type' 				=> 	'textfield',
					'#size'					=>	120,
					'#default_value' 				=>	!empty($payable_amendment_security_fees) ? $payable_amendment_security_fees:'0.0' ,
					'#attributes' 			=>  $readonly_payble_security_fees,
					'#required' 			=> 	TRUE,
					'#element_validate'			=> array('isnumaric_valiadtion'),
					
				);
				
			 $form['labour_worksite_details']['perevious_fees'] = array(
					'#prefix' 				=> '<section class="col col-md-6"><label class="input">',
					'#suffix' 				=> '</label></section>',
					'#title'				=>	'Highest Fees paid during license/subsequent amendments ',
					'#type' 				=> 	'textfield',
					'#size'					=>	120,
					'#default_value' 		=>	!empty($previous_deposit_fees) ? $previous_deposit_fees:'' ,
					'#attributes' 			=> 	array('placeholder' => 'Address Line1 the contractor'),
					'#required' 			=> 	TRUE,
					'#element_validate'		=>  array('isnumaric_valiadtion'),
					'#attributes' 			=> 	array('id' => 'currecnt_deposit_fees','class' => array('readonly_view'),'readonly' => 'readonly'),
					
				);
				
			$form['labour_worksite_details']['perevious_security_fees'] = array(
					'#prefix' 				=> '<section class="col col-md-6"><label class="input">',
					'#suffix' 				=> '</label></section>',
					'#title'				=>	'Highest Security Fees paid during license/subsequent amendments ',
					'#type' 				=> 	'textfield',
					'#size'					=>	120,
					'#default_value' 		=>	!empty($previous_max_security) ? $previous_max_security:'' ,
					'#attributes' 			=> 	array('placeholder' => 'Address Line1 the contractor'),
					'#required' 			=> 	TRUE,
					'#element_validate'			=> array('isnumaric_valiadtion'),
					'#attributes'			=>	$readonly_previous_security_fees,
				);
				
				
		}
		
		
			
				
		    
	}
  }
}
		  

$form['labour_worksite_details']['submit'] = array (
			  '#prefix' 		=> '<br class="clear"><div class="col col-md-12">',  
			  '#type' 			=> 'submit',
			  '#attributes' 	=> array('class' => array('button left-btn')),
			  '#value' 			=> 'Save'										  
		   );				 

$form['labour_worksite_details']['back_link']	 = array(
			 '#suffix' 		=> '</div></div></div></div>',  
			'#markup' 			=> l(t('Back'),'amendment_license_renewal/apply_amendment/'. $serialno.'/'.$renewallicenseammendment_id.'/'. $applicationflag, array('html' => TRUE,'attributes'=> array('class'=>array('button ')))),
																		
		);			  
								  
	return $form;
	
	
}



function labour_worksite_info_submit($form, &$form_state){
	
	global $base_root, $base_path, $user;
	$labour_details = array();
		
	 $contractor_id 	= 	$user->uid;
	 $val				=	$form_state['values'];
	
	 //-------------------------Worksite Changing Doc-----------------------------------------
	$supportive_doc_file 	= fileupload($val['supportive_doc']);
	$view_supportive_doc	= $val['view_supportive'];
	 if ($supportive_doc_file!="")  {
		 	 $supportive_documents_fid			 =	$supportive_doc_file->fid;
			 $supportive_documents_worksite_name =	$supportive_doc_file->filename;
	
		  
	}else{
		 $supportive_documents_fid			 =	$view_supportive_doc;
	}
	
	 //-------------------------Payment Chalane Doc-----------------------------------------
	
	

	$id											= 	$val['id'];
	$status_from 								= 	$val['status_from'];
	$renewallicenseammendment_id				=	encryption_decryption_fun('decrypt', $val['renewallicenseammendment_id']);
	$applicationflag							=	encryption_decryption_fun('decrypt', $val['applicationflag']);
	$serialno									=	encryption_decryption_fun('decrypt', $val['serialno']);
	$application_id								=	encryption_decryption_fun('decrypt', $val['application_id']);
	$pe_userid									=	encryption_decryption_fun('decrypt', $val['pe_userid']);
	$amendment_check_field						=   encryption_decryption_fun('decrypt', $val['amendment_check_field']);
	$ammendment_status							=	encryption_decryption_fun('decrypt', $val['ammendment_status']);
	
	$payment_recipt_file						=	$paymet_recipt_file_fid;
	

	
	if($applicationflag == 'L'){
		$license_id								=	$renewallicenseammendment_id;//---kaj baki
	}if($applicationflag == 'A'){
		$license_id								=	return_ammendement_license_id($renewallicenseammendment_id);//---kaj baki
	}if($applicationflag == 'R'){
		$license_id								=	return_renewal_license($renewallicenseammendment_id);//---kaj baki
	}
	
	$editable_field 							=   explode(',', $amendment_check_field);
	if(!empty($editable_field)){
	foreach($editable_field as $row){
		if($row == 'worksite_address'){
			$worksite_address_line						=	$val['worksite_address_line'];
			$worksite_dist 								= 	$val['worksite_dist'];
			$worksite_subdivision 						= 	$val['worksite_subdivision'];
			$worksite_areatype 							= 	$val['worksite_areatype'];
			$name_worksite_areatype						=	$val['name_worksite_areatype'];
			$worksite_vill_ward							=	$val['worksite_vill_ward'];
			$worksite_ps								= 	$val['worksite_ps'];
			$worksite_pin								=	$val['worksite_pin'];
			$reason_changing_worksite					=	$val['reason'];
			$supportive_documents_worksite				=	$supportive_documents_fid;
			$labour_details1 = array(
					'worksite_address_line'				=>	$worksite_address_line,
					'worksite_dist'						=>	$worksite_dist, 
					'worksite_subdivision'				=>  $worksite_subdivision,
					'work_site_areatype'				=>	$worksite_areatype,
					'name_work_site_areatype'			=>	$name_worksite_areatype,
					'work_site_vill_ward'				=>  $worksite_vill_ward,
					'worksite_pin'						=>  $worksite_pin,
					'worksite_ps'						=>  $worksite_ps,
					'supportive_documents_worksite'		=>  $supportive_documents_worksite,
					'reason_changing_worksite'			=>	$reason_changing_worksite,
			);
			$labour_details = (array_merge($labour_details,$labour_details1));
		}
		if($row == 'contractor_max_no_of_labours_on_any_day'){
				$no_labour									=	$val['no_labour'];
				$applicable_security_fess					=	$val['security_fees'];
				$payable_amendment_security_fees			=	$val['payble_security_fees'];
				$payable_amendment_fees						=	$val['payable_amendment_fees'];
				$applicable_amendment_fee					=	$val['ammendment_fees'];
				$ammend_deposit_fees						=	encryption_decryption_fun('decrypt',$val['deposit_fees_value']);
				$previous_highest_deposit_fees				=	$val['perevious_fees'];
				$deposited_max_security_fees				=	encryption_decryption_fun('decrypt',$val['previous_security']);
				
				$previous_highest_security					=	$val['perevious_security_fees'];
				if($val['is_coparative'] == 'Yes' || $val['is_coparative'] == '1'){
					$is_coparative								=	1;
				}else{
					$is_coparative								=	0;
				}
				
				
				if($ammendment_status == 'I')
				//$is_coparative								=	$val['is_coparative'];
				$due_security_fees							=   $val['due_security_fees'];	
				
				$labour_details2 = array(
					
					'max_of_contract_labour'				=> $no_labour,
					'applicable_security_fess'				=> $applicable_security_fess,
					'payable_amendment_security_fees'		=> $payable_amendment_security_fees,
					'payable_amendment_fees'				=> $payable_amendment_fees,
					'ammend_deposit_fees'					=> $ammend_deposit_fees,
					'previous_highest_deposit_fees'			=> $previous_highest_deposit_fees,
					'deposited_max_security_fees'			=> $deposited_max_security_fees,
					'previous_highest_security'				=> $previous_highest_security,
					'is_coparative'							=> $is_coparative,
					'due_security_fees'						=> $due_security_fees,
					'applicable_amendment_fee'				=> $applicable_amendment_fee
				  
				);
				
			
			$labour_details = (array_merge($labour_details,$labour_details2));
		}
	}
}
//print_r($labour_details2);die();	
	$labour_details3								= 	array(
			'license_renewal_ammendment_id'		=>  $renewallicenseammendment_id,
			'application_id'					=>	$application_id,
			'user_id'							=>  $pe_userid,
			'license_id'						=>  $license_id,
			'created_by'						=>	$contractor_id,
			'is_flag'                           =>  $applicationflag,
			'worksite_details_flag'				=>	1
		);
	$labour_details = (array_merge($labour_details,$labour_details3));
	

	
	if($ammendment_status=='I'){	
			$ammendment_id = return_ammendement_id($license_id);
			amemd_log($ammendment_id);
			$upadte_array	=	 array(
						
						'ammendment_status'			=>	'',
						'ammendment_final_status'	=>	'',
						'created_date'				=>	NULL,
						'alc_inspector_action_date'	=>	NULL,
						'remark_by_userid'			=>	NULL,
						'remark_by_userid'			=>	NULL,
						'ammendment_certificate_id'	=> NULL,
						
				);
				
			$update_ammendment_table 	= 	db_update('l_amendment_of_contract_labour_license');
 			$update_ammendment_table	->	fields($upadte_array);
 			$update_ammendment_table	->	condition('id',$amend_id);
			$update_ammendment_table	->	execute();
	}
	
	$details_ammendment_table        				 =  check_ammendment_table_license_renewal_ammendment_id($renewallicenseammendment_id);
	
	

	//if($check_ammendment_table->rowCount()==0){
	if(empty($details_ammendment_table['id'])|| $details_ammendment_table['id'] ==NULL || $details_ammendment_table['id'] == ''){	
		db_insert('l_amendment_of_contract_labour_license')->fields($labour_details)->execute();
		$ammendment_id 	= return_ammendement_id($license_id);
		contractor_activity_tag_insert($serialno,'A',$ammendment_id,NULL);
		
	}else{
	
		//print_r($labour_details);die();
	
		$update_ammendment 	= 	db_update('l_amendment_of_contract_labour_license');
 		$update_ammendment	->	fields($labour_details);
 		$update_ammendment	->	condition('license_id',$license_id);
		$update_ammendment	->	execute();
	
	
		$ammendment_id 				= return_ammendement_id($license_id);
		contractor_activity_tag_insert($serialno,'A',$ammendment_id,NULL);
		
	}
	
	$message = 2;
	   drupal_goto('amendment_license_renewal/apply_amendment/'.$val['serialno'].'/'.$val['renewallicenseammendment_id'].'/'.$val['applicationflag'].'#'.$message, array('fragment' => 'Labour and Worksite')) ;				 
	
}
function get_is_coparative($form, $form_state){
	$commands 	= array();
	
	//$commands[] = ajax_command_replace('#ammendment_fees_id', drupal_render($form['labour_worksite_details']['ammendment_fees']));
	$commands[] = ajax_command_replace('#security_fees_id', drupal_render($form['labour_worksite_details']['security_fees']));
	//$commands[] = ajax_command_replace('#payable_amendment_fees_id', drupal_render($form['labour_worksite_details']['payable_amendment_fees']));
	$commands[] = ajax_command_replace('#payble_security_fees_id', drupal_render($form['labour_worksite_details']['payble_security_fees']));
	$commands[] = ajax_command_replace('#view_balance_security_fees', drupal_render($form['labour_worksite_details']['view_balance_security_fees']));
	$commands[] = ajax_command_replace('#due_security_fees_view', drupal_render($form['labour_worksite_details']['due_security_fees']));
	$commands[] = ajax_command_replace('#nolabour_id', drupal_render($form['labour_worksite_details']['no_labour']));
	
	
  	return array('#type' => 'ajax', '#commands' => $commands);
}


function get_district_dashboard() {
		$default_select 	= array(""=>"- Select District -");
		$result 			= db_query("SELECT district_code,district_name FROM {district_master} where is_active = 'Y' order by district_name");
		$list 				= array();
		foreach ($result as $record) {
			$list[$record->district_code] = t('@districtname', array(
				'@districtname' => $record->district_name,
			));
		}
		return ($default_select+$list);
	}
	
	function get_subdivisionlist_dashboard($value_dist=''){

	if(!empty($value_dist)){
		$default_subdvselect 	= array(""=>"- Select Sub-division -");
		$all_subdvs 			= array();
		$query_subdv 			= db_select('sub_division', 'subdv');
		$query_subdv			->fields('subdv',array('sub_div_code','sub_div_name'));
		$query_subdv			->orderBy('subdv.sub_div_name', 'ASC');
		$query_subdv			->condition('subdv.is_active','Y');
		$query_subdv			->condition('subdv.district_code',(int)$value_dist);
		$result_subdv 			= $query_subdv->execute(); 
		if(!empty($result_subdv)) {      
			foreach($result_subdv as $rows) {
				$all_subdvs[$rows->sub_div_code] = t('@subdvsname', array('@subdvsname' => $rows->sub_div_name));
			}    
		}
		
		if (isset($all_subdvs)) {			
			return ($default_subdvselect + $all_subdvs);	
		} else {				
			return $default_subdvselect;
		}
	
	}else{
		$default_subdvselect 	= array(""=>"- Select Sub-division -");
		return $default_subdvselect;
	}
	
	}
	
	function get_areatype_dashboard($value_loc_e_dist, $value_loc_e_subdivision){
		if(!empty($value_loc_e_dist)&& !empty($value_loc_e_subdivision) ){
		$default_areatypeselect 	= array(""=>"- Select AreaType -");
		$all_areatype 				= array();
		$query_areatype 			= db_select('block_mun_master', 'bmm');
		$query_areatype				->fields('bmm',array('type'));
		$query_areatype				->condition('bmm.distict_code', trim($value_loc_e_dist));
		$query_areatype				->condition('bmm.sub_division_code', trim($value_loc_e_subdivision));
		$query_areatype				->groupBy('bmm.type');
		$result_areatype 			= $query_areatype->execute(); 
		if(!empty($result_areatype)) {      
			foreach($result_areatype as $rows) {
				if($rows->type=='B'){
					$all_areatype[$rows->type] = t('@areatype', array('@areatype' =>'Block'));
				}if($rows->type=='M'){
					$all_areatype[$rows->type] = t('@areatype', array('@areatype' =>'Municipality'));
				}if($rows->type=='C'){
					$all_areatype[$rows->type] = t('@areatype', array('@areatype' =>'Corporation'));
				}if($rows->type=='S'){
					$all_areatype[$rows->type] = t('@areatype', array('@areatype' =>'SEZ'));
				}if($rows->type=='N'){
					$all_areatype[$rows->type] = t('@areatype', array('@areatype' =>'Notified Area'));
				}
				
			}    
		}
		//print_r($all_areatype); //exit;
		if (isset($all_areatype)) {			
			return ($default_areatypeselect + $all_areatype);	
		} else {				
			return $default_areatypeselect;
		}
	}else{
		$default_areatypeselect 	= array(""=>"- Select AreaType -");
		return $default_areatypeselect;
	}
	}
	
	function get_block_dashboard($value_subdv, $value_areatype){
	if(!empty($value_subdv)&& !empty($value_areatype) ){
	$default_blockselect 	= array(""=>"- Select -");
	$all_blocks 			= array();
	$query_block 			= db_select('block_mun_master', 'blm');
	$query_block			->fields('blm',array('block_code','block_mun_name'));
	$query_block			->orderBy('blm.block_mun_name', 'ASC');
	$query_block			->condition('blm.sub_division_code',$value_subdv,'=');
	$query_block			->condition('blm.type',$value_areatype,'=');
	$result_block 			= $query_block->execute(); 
	if(!empty($result_block)) {      
	  	foreach($result_block as $rows) {
			$all_blocks[$rows->block_code] = t('@blockname', array('@blockname' => $rows->block_mun_name));
		}    
	}

	if (isset($all_blocks)) {
		return ($default_blockselect + $all_blocks);
	} else {
		return $default_blockselect;
	}
}else{
	$default_blockselect 	= array(""=>"- Select -");
	return $default_blockselect;

}
	}
	
	function get_villward_dashboard($value_villwared,$value_villwardtype){
		
	if(!empty($value_villwared) && !empty($value_villwardtype)){
	$default_villwardselect 	= array(""=>"- Select -");
	$all_villwards 				= array();
	$query_villwardkname 		= db_select('village_ward_master', 'vwm');
	$query_villwardkname		->fields('vwm',array('village_code','village_name'));
	$query_villwardkname		->orderBy('vwm.village_name', 'ASC');
	$query_villwardkname		->condition('vwm.block_code',$value_villwared,'=');
	$query_villwardkname		->condition('vwm.category',$value_villwardtype,'=');
	$result_villwardkname 		= $query_villwardkname->execute(); 
	if(!empty($result_villwardkname)) {      
		 foreach($result_villwardkname as $rows) {
			if($value_villwardtype == 'W')
				$all_villwards[$rows->village_code] = t('@villwardkname', array('@villwardkname' => 'Ward.'.$rows->village_name));	
			else				
				$all_villwards[$rows->village_code] = t('@villwardkname', array('@villwardkname' => ucwords($rows->village_name)));
		 }    
	}
	//print_r($all_villwards);

	if (isset($all_villwards)) {
		return ($default_villwardselect + $all_villwards);
	} else {
		return $default_villwardselect;
	}
}else{
	$default_villwardselect 	= array(""=>"- Select -");
	return $default_villwardselect;
}
}

function get_ps_dashboard($value_dist){
	if(!empty($value_dist)){
	$default_pselect 	= array(""=>"- Select -");
	$all_ps 			= array();
	$query_psname 		= db_select('police_station', 'ps');
	$query_psname		->fields('ps',array('police_station_code','name_of_police_station'));
	$query_psname		->orderBy('ps.name_of_police_station', 'ASC');
	$query_psname		->condition('ps.dristrict_code',$value_dist,'=');
	$result_psname 		= $query_psname->execute(); 
	if(!empty($result_psname)) {      
		foreach($result_psname as $rows) {
			$default_pselect[$rows->police_station_code] = t('@psname', array('@psname' => $rows->name_of_police_station));
		}    
	}
 	if (isset($all_ps)) {
		return ($default_pselect + $all_ps);
	} else {
		return $default_pselect;
	}
}else{
	$default_pselect 	= array(""=>"- Select -");
	return $default_pselect; 
}
}

function get_corporation_dasboard($code){
	if(!empty($code)){
	$default_corporationselect 		= array(""=>"- Select -");
	$all_corporation 				= array();
	$query_corporationname 			= db_select('village_ward_master', 'vwm');
	$query_corporationname			->fields('vwm',array('village_code', 'village_name'));
	$query_corporationname			->condition('vwm.block_code', $code, '=');
	$query_corporationname			->condition('vwm.category', 'C', '=');
	$query_corporationname			->orderBy('vwm.ord', 'ASC');
	$result_corporationname 		= $query_corporationname->execute(); 		
	if(!empty($result_corporationname)) {      
		foreach($result_corporationname as $rows) {
			$all_corporation[$rows->village_code] = t('@village_name', array('@village_name' => $rows->village_name));
		}    
	}
 	if (isset($all_corporation)) {
		return ($default_corporationselect + $all_corporation);
	} else {
		return $default_corporationselect;
	}
}else{
	$default_corporationselect 		= array(""=>"- Select -");
	return $default_corporationselect;
}
}





function get_worksite_address($form, $form_state){
	$commands 	= array();
	
	$commands[] = ajax_command_replace('#worksite_subdvsn_replace_1', drupal_render($form['labour_worksite_details']['worksite_subdivision']));
	$commands[] = ajax_command_replace('#worksite_areatype_replace', drupal_render($form['labour_worksite_details']['worksite_areatype']));
	$commands[] = ajax_command_replace('#name_worksite_areatype_replace', drupal_render($form['labour_worksite_details']['name_worksite_areatype']));
	$commands[] = ajax_command_replace('#worksite_vill_ward_replace', drupal_render($form['labour_worksite_details']['worksite_vill_ward']));
	
	$commands[] = ajax_command_replace('#worksite_ps_replace', drupal_render($form['labour_worksite_details']['worksite_ps']));
	$commands[] = ajax_command_replace('#office_locatiuon_replace', drupal_render($form['labour_worksite_details']['office_locatiuon']));
	
  	return array('#type' => 'ajax', '#commands' => $commands);
}

function isnumaric_valiadtion($element, &$form_state){
	
  
  	$val			=	$form_state['values'];
	$txtValue		=	trim($element['#value']);
	if($txtValue != '' && !is_numeric($txtValue)) {
		form_error($element, t($element['#title'].' SHOULD BE VALID'));
		$form_state['rebuild'] = TRUE;
	}

	
}
