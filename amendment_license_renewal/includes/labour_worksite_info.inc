<?php
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
drupal_add_js(drupal_get_path('module', 'amendment_license_renewal') . '/js/checklabournumber.js');

// function labour_worksite_info($form, &$form_state ,$serialno='' ,$renewallicenseammendment_id= '',$applicationflag= ''){ 
	
		// 		global $base_root, $base_path, $user;
		// 		$userid = $user->uid;
				
		// 		//if($userid != '8772'){ echo 'Under Maintenance'; die;}
		// 	drupal_add_library('system','ui.datepicker');
			
			
		// 	$readonly_contarctor_dob 		= array( 'class' => array('ammendment_date'), 'onclick' => 'datepicker_lastrenewal();','readonly' => 'readonly',);
		// 	$serial_no						=  encryption_decryption_fun('decrypt', $serialno);
		// 	$renewal_license_ammendment_id	=  encryption_decryption_fun('decrypt', $renewallicenseammendment_id);
		// 	$application_flag				=  encryption_decryption_fun('decrypt', $applicationflag);
		// 	//$check_ammendment_table         =  check_ammendment_table($renewal_license_ammendment_id);
		// 	//$check_ammendment_table 		= check_ammendment_table($renewal_license_ammendment_id);			
		// 	$contractor_regmaster_details 	= contractor_regmaster_details($serial_no);
		// 	if(!empty($contractor_regmaster_details)){
		// 		$name_of_contractor 	 = 	$contractor_regmaster_details['name_of_contractor'];
		// 		$application_id			 = 	$contractor_regmaster_details['id'];
		// 		$pe_userid				 = 	$contractor_regmaster_details['user_id'];
		// 		$max_no_of_labours_pe	 = 	$contractor_regmaster_details['contractor_max_no_of_labours_on_any_day'];
		// 	}
			
		// 	/*$form['labour_worksite_details']	= array(
		// 				'#title' 				=>	 t('<div class="content"><div class="sky-form" style="width:100%;"><header><h3> </h3></header>'),
		// 				'#type' 				=> 	'fieldset',
		// 				'#attributes' 			=> 	array('class'=>array('')),
		// 				'#collapsed' 			=> 	TRUE,
		// 				'#collapsable'			=>	TRUE
		// 				);
		// 				*/
						
		// 	/*$form['labour_worksite_details']	= array(
		// 				'#title' 				=>	 t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading"></div>'),
		// 				//'#type' 				=> 	'fieldset',
		// 				'#attributes' 			=> 	array('class'=>array('')),
		// 				'#collapsed' 			=> 	TRUE,
		// 				'#collapsable'			=>	TRUE
		// 				);	*/

		// 	$form['labour_worksite_details']['markup']	= array(
		// 		'#type'		=> 'markup',
		// 		'#markup'	=> '<div class="content">
		// 							<div class="panel panel-default panel-form">
		// 								<div class="panel-heading"><center>Worksite Details</center></div>
		// 								<div class="panel-body">'
			
		// 	);		
		// 	if($application_flag == 'L'){
		// 		$license_id						=	$renewal_license_ammendment_id;//---kaj baki
		// 	}if($application_flag == 'A'){
		// 		$license_id						=	return_ammendement_license_id($renewal_license_ammendment_id);//---kaj baki
		// 	}if($application_flag == 'R'){
		// 		$license_id						=	return_renewal_license($renewal_license_ammendment_id);//---kaj baki 
				
		// 	}	
			
		// 	if($application_flag == 'L'){
				
		// 		$particular_data = particular_data($serial_no);
				
				
		// 		if(!empty($particular_data)){
					
		// 			$contractor_max_labours_license_renewal		= $particular_data['contractor_max_no_of_labours_on_any_day'];
		// 			$worksite_address_line_license_renewal		= $particular_data['worksite_address_line'];
		// 			$worksite_dist_license_renewal				= $particular_data['worksite_dist'];
		// 			$work_site_areatype_license_renewal			= $particular_data['work_site_areatype'];
		// 			$worksite_subdivision_license_renewal		= $particular_data['worksite_subdivision'];
		// 			$name_work_site_areatype_license_renewal	= $particular_data['name_work_site_areatype'];
		// 			$work_site_vill_ward_license_renewal		= $particular_data['work_site_vill_ward'];
		// 			$worksite_pin_license_renewal				= $particular_data['worksite_pin'];
		// 			$worksite_ps_license_renewal				= $particular_data['worksite_ps'];
		// 			$max_no_of_labours_license_renewal			= $particular_data['contractor_max_no_of_labours_on_any_day'];
		// 			$is_coparative_license_renewal				= $particular_data['is_coparative'];
		// 			$amount_of_licence_renewal					= $particular_data['amount_of_licence'];
		// 			$amount_of_security_deposit_licence_renewal	= $particular_data['amount_of_security_deposit'];
		// 			$courrent_deposit_fees_amount				= $particular_data['amount_of_licence'];
		// 			$amount_of_licence_time						= $particular_data['amount_of_licence'];
		// 			//$payble_security_fees_license_renewal		= $particular_data['amount_of_security_deposit'];
					
		// 		}
		// 	}

			
		//   if($application_flag == 'R'){
		// 	  $renewal_data 									= 	renewal_data_last($renewal_license_ammendment_id);
		// 	  $particular_data 									= 	particular_data($serial_no);
			
		// 	  $max_no_of_labours_license_renewal				= 	$renewal_data['max_of_contract_labour'];
		// 	  $payble_security_fees_license_renewal				=	$renewal_data['payble_security_fees'];	
		// 	  $balance_securit_amount_license_renewal			=	$renewal_data['balance_securit_amount'];
		// 	  $due_security_fees_amount_license_renewal			=	$renewal_data['due_security_fees'];	
		// 	  $amount_of_licence_renewal						=	$renewal_data['license_reneal_fees'];	
		// 	  $payble_security_fees_license_renewal				=	$renewal_data['renewal_security_new'];
		// 	  $contractor_max_labours_license_renewal			= 	$particular_data['contractor_max_no_of_labours_on_any_day'];
		// 	  $worksite_address_line_license_renewal			= 	$particular_data['worksite_address_line'];
		// 	  $worksite_dist_license_renewal					= 	$particular_data['worksite_dist'];
		// 	  $work_site_areatype_license_renewal				= 	$particular_data['work_site_areatype'];
		// 	  $worksite_subdivision_license_renewal				= 	$particular_data['worksite_subdivision'];
		// 	  $name_work_site_areatype_license_renewal			= 	$particular_data['name_work_site_areatype'];
		// 	  $work_site_vill_ward_license_renewal				= 	$particular_data['work_site_vill_ward'];
		// 	  $worksite_pin_license_renewal						= 	$particular_data['worksite_pin'];
		// 	  $worksite_ps_license_renewal						= 	$particular_data['worksite_ps'];
			
			
		//   }
		
		//   	$max_renewal_fees 	=  return_maximum_renewal_fees($license_id,$userid);
		// 	$max_ammend_fees 	=  return_maximum_ammendment_fees($license_id,$userid);
		// 	$max_license_fees	=  return_maximum_license_fees($serial_no,$userid);
		// 	//echo $max_renewal_fees.'-666-'.$max_ammend_fees.'-7777-'.$max_license_fees;exit;
			
		// 	if($max_renewal_fees>$max_ammend_fees)
		// 	    $max_fees1	= $max_renewal_fees;
		//     if($max_renewal_fees<$max_ammend_fees)
		// 	    $max_fees1	= $max_ammend_fees;	 
		// 	if($max_renewal_fees == $max_ammend_fees)
		// 	    $max_fees1	= $max_ammend_fees;	 
		// 	if($max_fees1>$max_license_fees)
		// 		$max_fees2 = $max_fees1;
		// 	if($max_fees1<$max_license_fees)
		// 		$max_fees2 = $max_license_fees;
		// 	if($max_fees1 == $max_license_fees)
		// 		$max_fees2 = $max_license_fees;
				
				
		// 	$maximum_renewal_labour = return_maximum_renewal_labour($license_id,$userid);
		// 	$maximum_ammend_labour	= return_maximum_ammendment_labour($license_id,$userid);
		// 	$maximum_license_labour = return_maximum_license_labour($serial_no,$userid);
			
			
		// 	 if($maximum_renewal_labour>$maximum_ammend_labour)
		// 	    $max_labour1	= $maximum_renewal_labour;
				
		//     if($maximum_renewal_labour<$maximum_ammend_labour)
		// 	    $max_labour1	= $maximum_renewal_labour;	
				
		// 	if($maximum_renewal_labour == $maximum_ammend_labour)
		// 	    $max_labour1	= $maximum_ammend_labour;	
				
		// 	if($max_labour1>$maximum_license_labour)
		// 		$max_labour2 = $max_labour1;
				
		// 	if($max_labour1<$maximum_license_labour)
		// 		$max_labour2 = $maximum_license_labour;
				
		// 	if($max_labour1 == $maximum_license_labour)
		// 		$max_labour2 = $maximum_license_labour;
				
		// 	//$check_ammendment_table 			 				= 	check_ammendment_table_license_renewal_ammendment_id($renewal_license_ammendment_id);	
		// 	//$details_ammendment_table 			 				=  $check_ammendment_table->fetchAssoc();
		// 	//echo $renewal_license_ammendment_id;
		// 	if($application_flag == 'A'){
		// 		$details_ammendment_table 			 				= check_ammendment_table_license_renewal_ammendment_id($renewal_license_ammendment_id);
		// 	}
		// 	// echo 2222;	
		// 	// echo '<pre>';
		// 	// print_r($details_ammendment_table);exit;
		// 	//$max_of_contract_labour 			= !empty($details_ammendment_table['max_of_contract_labour'])?$details_ammendment_table['max_of_contract_labour']:$max_labour2;
		// 	$worksite_address_line				= !empty($details_ammendment_table['worksite_address_line'])?$details_ammendment_table['worksite_address_line']:$worksite_address_line_license_renewal;
		// 	$worksite_dist						= !empty($details_ammendment_table['worksite_dist'])?$details_ammendment_table['worksite_dist']:$worksite_dist_license_renewal; 
		// 	$work_site_areatype					= !empty($details_ammendment_table['work_site_areatype'])?$details_ammendment_table['work_site_areatype']:$work_site_areatype_license_renewal;
		// 	$worksite_subdivision				= !empty($details_ammendment_table['worksite_subdivision'])?($details_ammendment_table['worksite_subdivision']):$worksite_subdivision_license_renewal;
		// 	$name_work_site_areatype			= !empty($details_ammendment_table['name_work_site_areatype'])?$details_ammendment_table['name_work_site_areatype']:$name_work_site_areatype_license_renewal;
		// 	$work_site_vill_ward				= !empty($details_ammendment_table['work_site_vill_ward'])?$details_ammendment_table['work_site_vill_ward']:$work_site_vill_ward_license_renewal;
		// 	$worksite_pin						= !empty($details_ammendment_table['worksite_pin'])?$details_ammendment_table['worksite_pin']:$worksite_pin_license_renewal;

		// 	$worksite_ps						= !empty($details_ammendment_table['worksite_ps'])?$details_ammendment_table['worksite_ps']:$worksite_ps_license_renewal;
		// 	$pe_userid							= !empty($details_ammendment_table['user_id'])?$details_ammendment_table['user_id']:$pe_userid;
		// 	$application_id						= !empty($details_ammendment_table['application_id'])?$details_ammendment_table['application_id']:$application_id;
		// 	$reference_no						= $details_ammendment_table['reference_no'];
			
			
		// 	$is_coparative						= !empty($details_ammendment_table['is_coparative'])?$details_ammendment_table['is_coparative']:(!empty($is_coparative_license_renewal)?$is_coparative_license_renewal:0);
		// 	$max_of_contract_labour_hidden		= !empty($details_ammendment_table['max_of_contract_labour'])?$details_ammendment_table['max_of_contract_labour']: $max_no_of_labours_license_renewal;
		// 	//$max_of_contract_labour				= !empty($details_ammendment_table['max_of_contract_labour'])?$details_ammendment_table['max_of_contract_labour']:(!empty($max_no_of_labours_license_renewal)?$max_no_of_labours_license_renewal:$max_no_of_labours_pe);
		// 	if($ammendment_status == 'I'){
		// 		$max_of_contract_labour	 = !empty($details_ammendment_table['max_of_contract_labour'])?$details_ammendment_table['max_of_contract_labour']:(!empty($max_no_of_labours_license_renewal)?$max_no_of_labours_license_renewal:$max_no_of_labours_pe);	
		// 	}
		// 	if((empty($ammendment_status) || $ammendment_status == 'B' )){
		//     		$max_of_contract_labour	 = !empty($details_ammendment_table['max_of_contract_labour'])?$details_ammendment_table['max_of_contract_labour']:(!empty($max_no_of_labours_license_renewal)?$max_no_of_labours_license_renewal:$max_no_of_labours_pe);
		// 		//$max_of_contract_labour	 = !empty($max_no_of_labours_license_renewal)?$max_no_of_labours_license_renewal:$max_no_of_labours_pe;	
						
		// 	}

		// 	$license_renewal_fees				= !empty($details_ammendment_table['license_renewal_fees'])?$details_ammendment_table['license_renewal_fees']:$amount_of_licence_renewal;
			
		// 	$license_renewal_security_fees   	= !empty($details_ammendment_table['license_renewal_security_fees'])?$details_ammendment_table['license_renewal_security_fees']:$amount_of_security_deposit_licence_renewal;
			
			
			
		// 	//$ammend_deposit_fees				= $details_ammendment_table['ammend_deposit_fees'];
		// 	$deposited_max_security_fees		= $details_ammendment_table['deposited_max_security_fees'];	
		// 	$ammend_deposit_fees				= $details_ammendment_table['previous_highest_deposit_fees'];
		//   	$supportive_documents				= $details_ammendment_table['supportive_documents_worksite'];
		// 	$ammendment_status 					=  $details_ammendment_table['ammendment_status'];
		// 	///echo $deposited_max_security_fees;exit;
		//     $supportive_documents_worksite		= fetch_uplaoded_file_details($supportive_documents);
		// 	if(!empty($supportive_documents_worksite['fid'])){
		// 			$url				= explode('//',$supportive_documents_worksite['uri']);
		//    			$document_url		= $url[1];
		// 			$document_file		= $supportive_documents_worksite['filename']; 
		// 			$view_file			= '<span><a title = "Click here to view Supportive Document" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$document_url.'" target="_blank">View supportive Document in favour of changing worksite address </a></span>';
		// 	}
			
		// 	//---------------------------------------------fees calcution for ammendment------------------------------------------------
			
		// 	if($max_labour2>$max_of_contract_labour)
		// 				$highest_labour = $max_labour2;
		// 	if($max_labour2<$max_of_contract_labour)
		// 			$highest_labour = $max_of_contract_labour;
		// 	if($max_labour2 == $max_of_contract_labour)
		// 			$highest_labour = $max_of_contract_labour;
					
				
					
		// 		//echo $highest_labour;die();
		// 	$ammendent_fees						 	=   applicable_ammendment_fees_calculation($max_of_contract_labour);
			
		// 	if($max_fees2>$ammend_deposit_fees){
		// 		$currecnt_deposit_fees = $max_fees2;
		// 	}if($max_fees2<$ammend_deposit_fees){
		// 		$currecnt_deposit_fees = $ammend_deposit_fees;
		// 	}if($max_fees2 == $ammend_deposit_fees){
		// 		$currecnt_deposit_fees = $ammend_deposit_fees;
		// 	}
			
				
		// 	$applicable_amendment_fee   	= !empty($details_ammendment_table['applicable_amendment_fee'])?$details_ammendment_table['applicable_amendment_fee']:!empty($ammendent_fees)?$ammendent_fees:'0';
			
			
		// 	if(empty($ammendment_status) || $ammendment_status == 'I'){
		// 	$previous_deposit_fees   		= $currecnt_deposit_fees;
			
		// 	}
		// 	else{
		// 			$previous_deposit_fees   		= !empty($details_ammendment_table['previous_highest_deposit_fees'])?$details_ammendment_table['previous_highest_deposit_fees']:$currecnt_deposit_fees;
					
		// 	}

		// 	if($previous_deposit_fees>$ammendent_fees){
		// 			$paymantable_ammenedent_fees 	= 0.00;
		// 			$deposit_fees_value				= $previous_deposit_fees;
		// 		}if($previous_deposit_fees<$ammendent_fees){
		// 			  $paymantable_ammenedent_fees 	= $ammendent_fees-$previous_deposit_fees; 
		// 			  $deposit_fees_value			= $ammendent_fees;
		// 		}if($previous_deposit_fees==$ammendent_fees){
		// 			 $paymantable_ammenedent_fees 	= 0.00;
		// 			 $deposit_fees_value			= $ammendent_fees;
		// 		}	
		// 		//echo $paymantable_ammenedent_fees;die();
		// 	//$payable_amendment_fees   		= !empty($details_ammendment_table['payable_amendment_fees'])?$details_ammendment_table['payable_amendment_fees']:$paymantable_ammenedent_fees;
		// 	$payable_amendment_fees   		= $paymantable_ammenedent_fees;
		// 	$balance_fess   				= $balance_fess1;
		// 	$reason_changing_worksite   	= !empty($details_ammendment_table['reason_changing_worksite'])?$details_ammendment_table['reason_changing_worksite']:'Nil';
			
		// 	$form['labour_worksite_details']['worksite_address_line'] = array(
		// 				'#type'		=> 'textarea',
		// 				'#title'	=> t('Address Line1'),
		// 				'#default_value' => $worksite_address_line,
		// 				'#attributes' 	=> array('class' => array('form-control')),
		// 	);
			
			
		// 	//echo $worksite_address_line.'-'.$worksite_dist.'-'.$worksite_subdivision.'-'.$work_site_areatype.'-'.$name_work_site_areatype.'-'.$work_site_vill_ward.'-'.$worksite_pin;
			
		// 	$form['labour_worksite_details']['worksite_dist'] = array(
		// 				'#type'		=> 'hidden',
		// 				'#value'	=> $worksite_dist
		// 	);
			
		// 	$form['labour_worksite_details']['worksite_subdivision'] = array(
		// 				'#type'		=> 'hidden',
		// 				'#value'	=> $worksite_subdivision
		// 	);
			
		// 	$form['labour_worksite_details']['worksite_areatype'] = array(
		// 				'#type'		=> 'hidden',
		// 				'#value'	=> $work_site_areatype
		// 	);
		// 	$form['labour_worksite_details']['name_worksite_areatype'] = array(
		// 				'#type'		=> 'hidden',
		// 				'#value'	=> $name_work_site_areatype
		// 	);
			
		// 	$form['labour_worksite_details']['worksite_vill_ward'] = array(
		// 				'#type'		=> 'hidden',
		// 				'#value'	=> $work_site_vill_ward
		// 	);
			
		// 	$form['labour_worksite_details']['worksite_ps'] = array(
		// 				'#type'		=> 'hidden',
		// 				'#value'	=> $worksite_ps
		// 	);
			
			
		// 	$form['labour_worksite_details']['worksite_pin'] = array(
		// 				'#type'		=> 'hidden',
		// 				'#value'	=> $worksite_pin
		// 	);
			
		// 	$form['labour_worksite_details']['currecnt_deposit_fees'] = array(
		// 				'#type' 			=> 'hidden',
		// 				'#default_value' 	=> !empty($previous_deposit_fees) ? encryption_decryption_fun('encrypt', $previous_deposit_fees):'0.00',
		// 				'#attributes' 		=> 	array('id' => 'currecnt_deposit_fees'),
														
		// 		 );
		// 	$form['labour_worksite_details']['max_labour_previous'] = array(
		// 				'#type' 			=> 'hidden',
		// 				'#default_value' 	=> !empty($highest_labour) ? encryption_decryption_fun('encrypt', $highest_labour):'0.00',
		// 				'#attributes' 		=> 	array('id' => 'max_labour_previous'),
														
		// 		 );
		// 	$form['labour_worksite_details']['deposit_fees_value'] = array(
						
		// 				'#type' 			=> 'hidden',
		// 				'#default_value' 	=> !empty($deposit_fees_value) ? encryption_decryption_fun('encrypt', $deposit_fees_value):'0.00',
		// 				'#attributes' 		=> 	array('id' => 'deposit_fees'),
														
		// 		 );	

		// 	$form['labour_worksite_details']['max_of_contract_labour_hidden'] = array(
		// 				'#type' 			=> 'hidden',
		// 				'#default_value' 	=> !empty($max_of_contract_labour_hidden) ? encryption_decryption_fun('encrypt', $max_of_contract_labour_hidden):'0.00',
		// 				'#attributes' 		=> 	array('id' => 'max_of_contract_labour_hidden'),
						
						
												
		// 		 );	
		// 	$form['labour_worksite_details']['ammendment_status'] = array(
		// 				'#type' 			=> 'hidden',
		// 				'#default_value' 	=> !empty($ammendment_status) ? encryption_decryption_fun('encrypt', $ammendment_status):'',
						
						
															
		// 		 );	
			
		// 	$form['labour_worksite_details']['renewallicenseammendment_id'] = array(
		// 				'#type' 			=> 'hidden',
		// 				'#default_value' 	=> !empty($renewallicenseammendment_id) ? $renewallicenseammendment_id:'',
						
						
															
		// 		 );	
			

		// 	$form['labour_worksite_details']['applicationflag'] = array(
		// 				'#type' 			=> 'hidden',
		// 				'#default_value' 	=> !empty($applicationflag) ? $applicationflag:'',
		// 				'#attributes' 		=> 	array('id' => 'application_flag_id'),
						
						
						
															
		// 		 );	
		// 	$form['labour_worksite_details']['serialno'] = array(
		// 				'#type' 			=> 'hidden',
		// 				'#default_value' 	=> !empty($serialno) ? $serialno:'',
		// 				'#attributes' 		=> 	array('id' => 'serialno_id'),
						
						
															
		// 		 );				 
		// 	$form['labour_worksite_details']['application_id'] = array(
		// 				'#type' 			=> 'hidden',
		// 				'#default_value' 	=> !empty($application_id) ?  encryption_decryption_fun('encrypt', $application_id):'',
						
						
															
		// 		 );	
		// 	$form['labour_worksite_details']['pe_userid'] = array(
		// 				'#type' 			=> 'hidden',
		// 				'#default_value' 	=> !empty($pe_userid) ?  encryption_decryption_fun('encrypt', $pe_userid):'',
						
						
															
		// 		 );	
		// 	$form['labour_worksite_details']['reference_no'] = array(
		// 				'#type' 			=> 'hidden',
		// 				'#default_value' 	=> !empty($reference_no) ?  encryption_decryption_fun('encrypt', $reference_no):'',
						
						
															
		// 		 );	
			

			
			
		// 	$last_remark_activity 					= last_remark_activity($reference_no);
		// 	$remark_field_title 					= $last_remark_activity['remark_field_title'];
		//     $remark_type 							= $last_remark_activity['remark_type'];
		//     $readonly_ammendment_fees 				= array('class' => array('readonly_view'),'readonly' => 'readonly');
		// 	$readonly_security_fees 				= array('class' => array('readonly_view security_fees'),'readonly' => 'readonly');
		// 	$readonly_due_security_fees 			= array('class' => array('readonly_view due_security_fees'),'readonly' => 'readonly');
		// 	$readonly_payble_security_fees 			= array('class' => array('readonly_view payble_security_fees'),'readonly' => 'readonly');
		// 	$readonly_security_balance 				= array('class' => array('readonly_view security_balance_update'),'readonly' => 'readonly');
		// 	$readonly_previous_security_fees 		= array('class' => array('readonly_view'),'readonly' => 'readonly');
		// 	$readonly_labour						= array('class' => array('nolabour'),'onkeyup' => 'checkLabourNumber(this.val);');
			
			
		// 	if((empty($remark_field_title) && $remark_type =='B')|| (!empty($remark_field_title) && $remark_type =='B')){
		// 			   $readonly_worksite_address_line1	 	= array('placeholder' => 'Enter your name','class' => array('correction_show'));
		// 			   $readonly_worksite_dist 				= array('placeholder' => 'Enter your Father Name', 'class' => array('correction_show'));
		// 			   $readonly_reason 					= array('class' => array('correction_show'));
		// 			   $readonly_no_labour 					= array('class' => array('correction_show'));
					
		// 			   $readonly_supportive			 		= array('class' => array('correction_show'));
		// 			   $edit 								= 2;
		// 			   $readonly_supportive			 		= array('class' => array('correction_show'));
		// 			   $readonly_labour						= array('class' => array('correction_show nolabour'),'onkeyup' => 'checkLabourNumber(this.val);');
								
		// 	 }if(!empty($remark_field_title) && $remark_type !='I'){
						
		// 			   $remark_field_title = explode(',', $remark_field_title);	
		// 				foreach($remark_field_title as $row){
		// 					switch($row){
		// 						case  "contractor_max_no_of_labours_on_any_day":
		// 							    $readonly_labour	= array('class' => array('readonly_view nolabour'),'onkeyup' => 'checkLabourNumber(this.val);','readonly' => 'readonly');
		// 							 break;
		// 						case  "worksite_address":
		// 							  $readonly_worksite_address_line1	 = array('class' => array('readonly_view'),'readonly' => 'readonly');
		// 							  $readonly_worksite_dist 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
		// 							 break;
		// 						case  "co_oparative":
		// 							  $readonly_co_oparative	 = array('class' => array('readonly_view'),'readonly' => 'readonly');
		// 							 $edit_co_oparative			  =1;
		// 							 break;
		// 						case  "change_reason":
		// 							 $readonly_reason	 = array('class' => array('readonly_view'),'readonly' => 'readonly');
									
		// 							break;
		// 						case "supportive_documents":
		// 						 $readonly_supportive	 = array('class' => array('readonly_view'));
		// 							$edit = 1;
		// 							break;
							
		// 					}
		// 				}
		// 		   }
		// 	$ammendment 			 		= check_ammendment_fields($license_id,$serial_no)->fetchAssoc()	;
		// 	if(!empty($ammendment)){
				
		// 		$amendment_check_field 			= $ammendment['amendment_check_field'];
				
		// 		$form['labour_worksite_details']['amendment_check_field'] = array(
		// 				'#type' 			=> 'hidden',
		// 				'#default_value' 	=> !empty($amendment_check_field) ?  encryption_decryption_fun('encrypt', $amendment_check_field):'',
						
						
															
		// 		 );	
		// 		if(!empty($amendment_check_field)){
		// 			$editable_field 		= explode(',', $amendment_check_field);
		// 			foreach($editable_field as $row){
		// 				if($row == 'contractor_max_no_of_labours_on_any_day'){
						
						
		// 			$form['labour_worksite_details']['test'] 		= array(
		// 						'#prefix' 				=> '<div class="panel panel-default panel-form"><div class="panel-heading"><center>Contractor Labour Details</center></div><div class="panel-body"><div class="sky-form" style="width:100%;"><div class="col-sm-12"><h4 class="form-sub-title">Details of maximum contarct labour on any day</h4></div>',
							
		// 				   );
						
		// 		if($edit_co_oparative == 1){
		// 			 $form['labour_worksite_details']['is_coparative'] = array(
		// 					'#prefix' 			=> '<section class="col col-6 ammend-row"><div class="inline-group">',
		// 					'#suffix' 			=> '</section>',
		// 					'#title'				=>	'Co-oparative Society',
		// 					'#type' 				=> 	'textfield',
		// 					'#size'					=>	120,
		// 					'#default_value' 		=>	!empty($is_coparative) ? 'Yes':'No' ,
		// 					'#required' 			=> 	TRUE,
		// 					'#attributes' 			=>  $readonly_co_oparative,
		// 				);	
					
		// 		}else{
						
		// 		$form['labour_worksite_details']['is_coparative'] = array(
		// 					'#prefix' 			=> '<section class="col col-md-6 ammend-row"><div class="inline-group">',
		// 					'#suffix' 			=> '</section>',
		// 					'#type' 			=> 'radios',
		// 					'#title' 			=>  t('Co-oparative Society <span class="correction_show con_cate"></span>'),     
		// 					'#options' 			=> array('1'=>t('Yes'),'0'=>t('No')),
		// 					'#required' 		=> TRUE,
		// 					'#default_value' 	=> !empty($is_coparative) ? $is_coparative: 0,
		// 					'#attributes'       => array('onChange' => "check_coparetive();") 
		// 					/*'#ajax'			    =>  array(
		// 													'event' 			=> 'change',
		// 													'effect' 			=> 'fade',			
		// 													'callback'			=> 'get_is_coparative',
		// 													'method'			=> 'replace',
		// 													'progress'			=> '',
		// 												 ) */
		// 					);	
		// 		}
				
		// 		 $form['labour_worksite_details']['no_labour'] = array(
		// 					'#prefix' 				=> '<section class="col col-md-6 ammend-row"><div id ="nolabour_id"><label class="input">',
		// 					'#suffix' 				=> '</label></div>',
		// 					'#title'				=>	'Max Contarct Labour on any day',
		// 					'#type' 				=> 	'textfield',
		// 					'#size'					=>	120,
		// 					'#default_value' 		=>	!empty($max_of_contract_labour) ? $max_of_contract_labour:'' ,
		// 					'#attributes' 			=> 	$readonly_no_labour,
		// 					'#required' 			=> 	TRUE,
		// 					'#element_validate'		=> array('isnumaric_valiadtion'),
		// 					'#attributes' 			=> $readonly_labour
		// 				);
						
			
			
							
		// 		$form['labour_worksite_details']['username_img_markup'] 		= array(
								
		// 						'#type' 			=> 'markup', 
		// 						'#markup' 			=> '<span id="common_username_img_div"></span>'
		// 				   );
																	
		// 		$form['labour_worksite_details']['username_markup'] 			= array(
		// 					'#type' 			=> 'markup', 
		// 					'#markup' 			=> '<span id="common_username_div"></span>',
		// 					'#suffix' 			=> '</section>',
		// 			   );
							
		// 	$value_is_coparative 	= 	isset($form_state['values']['is_coparative']) ? $form_state['values']['is_coparative'] :$is_coparative;
		// 	$value_contract_labour 	= 	isset($form_state['values']['no_labour']) ? $form_state['values']['no_labour'] :$max_of_contract_labour;
			
					
		// 		//---------------------------------------------fees calcution for security------------------------------------------------
		// 	$max_license_security_fees 			= return_maximum_license_security_fees($serial_no,$userid);
		// 	$max_ammendment_security_fess       = return_maximum_ammendment_security_fess($license_id,$userid);
		// 	$max_renewal_security_fess			= return_maximum_renewal_security($license_id,$userid);
			
		// 	//echo $max_license_security_fees.'--'.$max_ammendment_security_fess.'--'.$max_renewal_security_fess;exit;
		// 	/*if($application_flag == 'R'){
		// 		$max_renewal_security_fess = $max_renewal_security_fess;
		// 	}if($application_flag == 'L'){
		// 		$max_renewal_security_fess = $max_license_security_fees;
		// 	}*/
			
		// 	if($max_license_security_fees>$max_ammendment_security_fess){
		// 		$max_security_fees1	=$max_license_security_fees;	 
				
		// 	}if($max_license_security_fees<$max_ammendment_security_fess){
		// 		$max_security_fees1	=$max_ammendment_security_fess;	 
				
		// 	}if($max_license_security_fees==$max_ammendment_security_fess){
		// 		$max_security_fees1	=$max_ammendment_security_fess;	 
				
		// 	}if($max_security_fees1>$max_renewal_security_fess){
		// 		$max_security_fees2 = $max_security_fees1;
		// 	}
		// 	if($max_security_fees1<$max_renewal_security_fess){
		// 		$max_security_fees2 = $max_renewal_security_fess;
		// 	}
		// 	if($max_security_fees1==$max_renewal_security_fess){
		// 		$max_security_fees2 = $max_renewal_security_fess;
		// 	}
		// 	if($max_security_fees2>$deposited_max_security_fees){
		// 		$current_security_deposit = $max_security_fees2;
		// 	}
		// 	if($max_security_fees2<$deposited_max_security_fees){
		// 		$current_security_deposit = $deposited_max_security_fees;
		// 	}
		// 	if($max_security_fees2 == $deposited_max_security_fees){
		// 		$current_security_deposit = $deposited_max_security_fees;
		// 	}

		//     $payable_amendment_security_fees_pre = $details_ammendment_table['payable_amendment_security_fees'];
			
		// 	if($ammendment_status == 'I')
		// 	$previous_max_security 		= $current_security_deposit+$payable_amendment_security_fees_pre;
			
		// 	if(empty($ammendment_status))
		// 	$previous_max_security 		= $current_security_deposit;
		// 	if($ammendment_status == 'B')
		// 	$previous_max_security 		= $details_ammendment_table['previous_highest_security'];
		// 	if(!empty($ammendment_status) && $ammendment_status=='I')
		// 	//$previous_max_security   	= !empty($details_ammendment_table['previous_highest_security'])?$details_ammendment_table['previous_highest_security']:$current_security_deposit;
		// 	//$previous_max_security   	=  $current_security_deposit;
		// 	$ammendent_security_fees 	=  applicable_ammendment_security_fees_calculation($value_contract_labour,$value_is_coparative);
			
		// 	if($ammendent_security_fees>$previous_max_security){
		// 		$paymantable_sucurity_ammenedent_fees = $ammendent_security_fees-$previous_max_security;
		// 		if(!empty($due_security_fees_amount_license_renewal) && $due_security_fees_amount_license_renewal!=0){
					
		// 			$paymantable_sucurity_ammenedent_fees 		= $paymantable_sucurity_ammenedent_fees+$due_security_fees_amount_license_renewal;
		// 			$deposited_security_fees					= $ammendent_security_fees;
		// 		}else{
		// 			$deposited_security_fees					= $ammendent_security_fees;
		// 		}
		// 	}if($ammendent_security_fees<$previous_max_security){
		// 		$paymantable_sucurity_ammenedent_fees = 0.00;
		// 		if(!empty($due_security_fees_amount_license_renewal) && $due_security_fees_amount_license_renewal!=0){
					
		// 			$paymantable_sucurity_ammenedent_fees 		= $due_security_fees_amount_license_renewal;
		// 			$deposited_security_fees					= $previous_max_security;
		// 		}else{
					
		// 			$deposited_security_fees					= $previous_max_security;
		// 		}
		// 	}if($ammendent_security_fees==$previous_max_security){
		// 		$paymantable_sucurity_ammenedent_fees = 0.00;
		// 		if(!empty($due_security_fees_amount_license_renewal) && $due_security_fees_amount_license_renewal!=0){
					
		// 			$paymantable_sucurity_ammenedent_fees 		= $due_security_fees_amount_license_renewal;
		// 			$deposited_security_fees					= $previous_max_security;
		// 		}else{
					
		// 			$deposited_security_fees					= $previous_max_security;
		// 		}
		// 	}
				
				
		// 			$applicable_security_fees   			= !empty($details_ammendment_table['applicable_security_fess']) ? $details_ammendment_table['applicable_security_fess'] : (!empty($ammendent_security_fees) ? $ammendent_security_fees : '0');
					
		// 			//$payable_amendment_security_fees   		=  !empty($details_ammendment_table['payable_amendment_security_fees'])?$details_ammendment_table['payable_amendment_security_fees']:$paymantable_sucurity_ammenedent_fees;
		// 		$payable_amendment_security_fees   		=  $paymantable_sucurity_ammenedent_fees;
					
									
		// 		$form['labour_worksite_details']['previous_security'] = array(
		// 				'#type' 			=> 'hidden',
		// 				'#default_value' 	=> !empty($previous_max_security) ? encryption_decryption_fun('encrypt', $previous_max_security):'0',
		// 				'#attributes' 		=> 	array('id' => 'previous_security'),
																
		// 		 );	
		// 		 $form['labour_worksite_details']['deposited_security_fees'] = array(
		// 				'#type' 			=> 'hidden',
		// 				'#value'			=> !empty($deposited_security_fees) ? encryption_decryption_fun('encrypt', $deposited_security_fees):'0',
		// 				'#attributes' 		=> 	array('id' => 'deposited_security'),
																
		// 		 );	
										
		// 		 $form['labour_worksite_details']['security_fees'] = array(
		// 					'#prefix' 				=> '<div id ="security_fees_id"><section class="col col-md-6"><label class="input">',
		// 					'#suffix' 				=> '</label></section></div>',
		// 					'#title'				=>	'Applicable Security Fees',
		// 					'#type' 				=> 	'textfield',
		// 					'#size'					=>	120,
		// 					'#default_value' 		=>	!empty($applicable_security_fees) ? $applicable_security_fees : $license_renewal_security_fees  ,
		// 					'#required' 			=> 	TRUE,
		// 					'#attributes' 			=>  $readonly_security_fees,
		// 				);	
					
		// 			$form['labour_worksite_details']['ammendment_fees'] = array(
		// 					'#prefix' 				=> '<div id ="ammendment_fees_id"><section class="col col-md-6"><label class="input">',
		// 					'#suffix' 				=> '</label></section></div>',
		// 					'#title'				=>	'Applicable Ammendment Fees',
		// 					'#type' 				=> 	'textfield',
		// 					'#size'					=>	120,
		// 					'#default_value' 		=>	!empty($applicable_amendment_fee) ? $applicable_amendment_fee:'' ,
		// 					'#required' 			=> 	TRUE,
		// 					'#element_validate'		=>  array('isnumaric_valiadtion'),
		// 					'#attributes'			=>	$readonly_ammendment_fees,
		// 				);
						
		// 			$form['labour_worksite_details']['due_security_fees'] = array(
		// 					'#prefix' 				=> '<div id="due_security_fees_view"><section class="col col-md-6"><label class="input">',
		// 					'#suffix' 				=> '</label></section></div>',
		// 					'#title'				=>	'Due Security Fees',
		// 					'#type' 				=> 	'textfield',
		// 					'#size'					=>	120,
		// 					'#default_value' 		=>	!empty($due_security_fees_amount_license_renewal) ? $due_security_fees_amount_license_renewal:'0.0' ,
		// 					'#required' 			=> 	TRUE,
		// 					'#element_validate'		=> array('isnumaric_valiadtion'),
		// 					'#attributes' 			=> $readonly_due_security_fees,
		// 				);
						
		// 			$form['labour_worksite_details']['payable_amendment_fees'] = array(
		// 					'#prefix' 				=> '<div id="payable_amendment_fees_id"><section class="col col-md-6"><label class="input">',
		// 					'#suffix' 				=> '</label></section></div>',
		// 					'#title'				=>	'Payble Ammendment Fees',
		// 					'#type' 				=> 	'textfield',
		// 					'#size'					=>	120,
		// 					'#default_value' 		=>	!empty($payable_amendment_fees) ? $payable_amendment_fees:'0.00' ,
		// 					'#attributes' 			=> 	array('placeholder' => 'Address Line1 the contractor'),
		// 					'#required' 			=> 	TRUE,
		// 					'#element_validate'		=> array('isnumaric_valiadtion'),
		// 					'#attributes'			=>	$readonly_ammendment_fees,
		// 				);
		// 		  $form['labour_worksite_details']['payble_security_fees'] = array(
		// 					'#prefix' 				=> '<div id="payble_security_fees_id"><section class="col col-md-6"><label class="input">',
		// 					'#suffix' 				=> '</label></section></div>',
		// 					'#title'				=>	'<span style="font-color:#FF0000;">Payble Ammendment security Fees</span>',
		// 					'#type' 				=> 	'textfield',
		// 					'#size'					=>	120,
		// 					'#default_value' 				=>	!empty($payable_amendment_security_fees) ? $payable_amendment_security_fees:'0.0' ,
		// 					'#attributes' 			=>  $readonly_payble_security_fees,
		// 					'#required' 			=> 	TRUE,
		// 					'#element_validate'			=> array('isnumaric_valiadtion'),
							
		// 				);
						
		// 			 $form['labour_worksite_details']['perevious_fees'] = array(
		// 					'#prefix' 				=> '<section class="col col-md-6"><label class="input">',
		// 					'#suffix' 				=> '</label></section>',
		// 					'#title'				=>	'Highest Fees paid during license/subsequent amendments ',
		// 					'#type' 				=> 	'textfield',
		// 					'#size'					=>	120,
		// 					'#default_value' 		=>	!empty($previous_deposit_fees) ? $previous_deposit_fees:'' ,
		// 					'#attributes' 			=> 	array('placeholder' => 'Address Line1 the contractor'),
		// 					'#required' 			=> 	TRUE,
		// 					'#element_validate'		=>  array('isnumaric_valiadtion'),
		// 					'#attributes' 			=> 	array('id' => 'currecnt_deposit_fees','class' => array('readonly_view'),'readonly' => 'readonly'),
							
		// 				);
						
		// 			$form['labour_worksite_details']['perevious_security_fees'] = array(
		// 					'#prefix' 				=> '<section class="col col-md-6"><label class="input">',
		// 					'#suffix' 				=> '</label></section>',
		// 					'#title'				=>	'Highest Security Fees paid during license/subsequent amendments ',
		// 					'#type' 				=> 	'textfield',
		// 					'#size'					=>	120,
		// 					'#default_value' 		=>	!empty($previous_max_security) ? $previous_max_security:'' ,
		// 					'#attributes' 			=> 	array('placeholder' => 'Address Line1 the contractor'),
		// 					'#required' 			=> 	TRUE,
		// 					'#element_validate'			=> array('isnumaric_valiadtion'),
		// 					'#attributes'			=>	$readonly_previous_security_fees,
		// 				);
						
						
		// 		}
						
		// 	  }
		//     }
		//   }
				

		//     $form['labour_worksite_details']['submit'] = array (
		// 			  '#prefix' 		=> '<br class="clear"><div class="col col-md-12">',  
		// 			  '#type' 			=> 'submit',
		// 			  '#attributes' 	=> array('class' => array('button left-btn')),
		// 			  '#value' 			=> 'Save'										  
		// 		   );				 

		//     $form['labour_worksite_details']['back_link']	 = array(
		// 			'#suffix' 		=> '</div></div></div></div>',  
		// 			'#markup' 		=> l(t('Back'),'amendment_license_renewal/apply_amendment/'. $serialno.'/'.$renewallicenseammendment_id.'/'. $applicationflag, array('html' => TRUE,'attributes'=> array('class'=>array('btn btn-primary left-btn')))),
																				
		// 		);			  
			
		//   $form['labour_worksite_details']['footer']	= array(
		// 		'#type'		=> 'markup',
		// 		'#markup'	=> 			'</div>
		// 							</div>
		// 						</div>'
			
		// 	);
									
		// 	return $form;
			
			
		// }
		// function labour_worksite_info_submit($form, &$form_state){
			
		// 	global $base_root, $base_path, $user;
		// 	$labour_details = array();
				
		// 	 $contractor_id 	= 	$user->uid;
		// 	 $val				=	$form_state['values']; 
		// 	 	 //-------------------------Worksite Changing Doc-----------------------------------------
		// 	$supportive_doc_file 	= fileupload($val['supportive_doc']);
		// 	$view_supportive_doc	= $val['view_supportive'];
		// 	 if ($supportive_doc_file!="")  {
		// 		 	 $supportive_documents_fid			 =	$supportive_doc_file->fid;
		// 			 $supportive_documents_worksite_name =	$supportive_doc_file->filename;
			
				
		// 	}else{
		// 		 $supportive_documents_fid			 =	$view_supportive_doc;
		// 	}
			
		// 	 //-------------------------Payment Chalane Doc-----------------------------------------
			
			

		// 	$id											= 	$val['id'];
		// 	$status_from 								= 	$val['status_from'];
		// 	$renewallicenseammendment_id				=	encryption_decryption_fun('decrypt', $val['renewallicenseammendment_id']);
		// 	$applicationflag							=	encryption_decryption_fun('decrypt', $val['applicationflag']);
		// 	$serialno									=	encryption_decryption_fun('decrypt', $val['serialno']);
		// 	$application_id								=	encryption_decryption_fun('decrypt', $val['application_id']);
		// 	$pe_userid									=	encryption_decryption_fun('decrypt', $val['pe_userid']);
		// 	$amendment_check_field						=   encryption_decryption_fun('decrypt', $val['amendment_check_field']);
		// 	$ammendment_status							=	encryption_decryption_fun('decrypt', $val['ammendment_status']); 
			
		// 	$payment_recipt_file						=	$paymet_recipt_file_fid;
			
			
			

			
		// 	if($applicationflag == 'L'){
		// 		$license_id								=	$renewallicenseammendment_id;//---kaj baki
		// 	}if($applicationflag == 'A'){
		// 		$license_id								=	return_ammendement_license_id($renewallicenseammendment_id);//---kaj baki
		// 	}if($applicationflag == 'R'){
		// 		$license_id								=	return_renewal_license($renewallicenseammendment_id);//---kaj baki
		// 	}
			
		// 	$editable_field 							=   explode(',', $amendment_check_field);
		// 	if(!empty($editable_field)){
				
		// 	foreach($editable_field as $row){
		// 		if($row == 'worksite_address'){
		// 			$worksite_address_line						=	$val['worksite_address_line'];
		// 			$worksite_dist 								= 	$val['worksite_dist'];
		// 			$worksite_subdivision 						= 	$val['worksite_subdivision'];
		// 			$worksite_areatype 							= 	$val['worksite_areatype'];
		// 			$name_worksite_areatype						=	$val['name_worksite_areatype'];
		// 			$worksite_vill_ward							=	$val['worksite_vill_ward'];
		// 			$worksite_ps								= 	$val['worksite_ps'];
		// 			$worksite_pin								=	$val['worksite_pin'];
		// 			$reason_changing_worksite					=	$val['reason'];
		// 			$supportive_documents_worksite				=	$supportive_documents_fid;
		// 			$labour_details1 = array(
		// 					'worksite_address_line'				=>	$worksite_address_line,
		// 					'worksite_dist'						=>	$worksite_dist, 
		// 					'worksite_subdivision'				=>  $worksite_subdivision,
		// 					'work_site_areatype'				=>	$worksite_areatype,
		// 					'name_work_site_areatype'			=>	$name_worksite_areatype,
		// 					'work_site_vill_ward'				=>  $worksite_vill_ward,
		// 					'worksite_pin'						=>  $worksite_pin,
		// 					'worksite_ps'						=>  $worksite_ps,
		// 					'supportive_documents_worksite'		=>  $supportive_documents_worksite,
		// 					'reason_changing_worksite'			=>	$reason_changing_worksite,
		// 			);
		// 			$labour_details = (array_merge($labour_details,$labour_details1));
		// 		}
		// 		if($row == 'contractor_max_no_of_labours_on_any_day'){
		// 				$no_labour									=	$val['no_labour'];
		// 				$applicable_security_fess					=	$val['security_fees'];
		// 				$payable_amendment_security_fees			=	$val['payble_security_fees'];
		// 				$payable_amendment_fees						=	$val['payable_amendment_fees'];
		// 				$applicable_amendment_fee					=	$val['ammendment_fees'];
		// 				$ammend_deposit_fees						=	encryption_decryption_fun('decrypt',$val['deposit_fees_value']);
		// 				$previous_highest_deposit_fees				=	$val['perevious_fees'];
		// 				$deposited_max_security_fees				=	encryption_decryption_fun('decrypt',$val['previous_security']);
						
		// 				$previous_highest_security					=	$val['perevious_security_fees'];
		// 				if($val['is_coparative'] == 'Yes' || $val['is_coparative'] == '1'){
		// 					$is_coparative								=	1;
		// 				}else{
		// 					$is_coparative								=	0;
		// 				}
						
						
		// 				if($ammendment_status == 'I')
		// 				//$is_coparative								=	$val['is_coparative'];
		// 				$due_security_fees							=   $val['due_security_fees'];	
						
		// 				$labour_details2 = array(
							
		// 					'max_of_contract_labour'				=> $no_labour,
		// 					'applicable_security_fess'				=> $applicable_security_fess,
		// 					'payable_amendment_security_fees'		=> $payable_amendment_security_fees,
		// 					'payable_amendment_fees'				=> $payable_amendment_fees,
		// 					'ammend_deposit_fees'					=> $ammend_deposit_fees,
		// 					'previous_highest_deposit_fees'			=> $previous_highest_deposit_fees,
		// 					'deposited_max_security_fees'			=> $deposited_max_security_fees,
		// 					'previous_highest_security'				=> $previous_highest_security,
		// 					'is_coparative'							=> $is_coparative,
		// 					'due_security_fees'						=> $due_security_fees,
		// 					'applicable_amendment_fee'				=> $applicable_amendment_fee
						
		// 				);
		// 			$labour_details = (array_merge($labour_details,$labour_details2));
					
		// 		}
		// 	}
		//  }
			
		// 	$labour_details3								= 	array(
		// 			'license_renewal_ammendment_id'		=>  $renewallicenseammendment_id,
		// 			'application_id'					=>	$application_id,
		// 			'user_id'							=>  $pe_userid,
		// 			'license_id'						=>  $license_id,
		// 			'created_by'						=>	$contractor_id,
		// 			'is_flag'                           =>  $applicationflag,
		// 			'worksite_details_flag'				=>	1
		// 		);
		// 	$labour_details = (array_merge($labour_details,$labour_details3));
			

			
		// 	if($ammendment_status=='I'){	
		// 			$ammendment_id = return_ammendement_id($license_id); 
		// 			if($ammendment_id!='' ){
		// 				amemd_log($ammendment_id);
		// 					$upadte_array	=	 array(
									
		// 							'ammendment_status'			=>	'',
		// 							'ammendment_final_status'	=>	'',
		// 							'created_date'				=>	NULL,
		// 							'alc_inspector_action_date'	=>	NULL,
		// 							'remark_by_userid'			=>	NULL,
		// 							'remark_by_userid'			=>	NULL,
		// 							'ammendment_certificate_id'	=> NULL,
									
		// 					);
						
		// 					$update_ammendment_table 	= 	db_update('l_amendment_of_contract_labour_license');
		// 					$update_ammendment_table	->	fields($upadte_array);
		// 					$update_ammendment_table	->	condition('id',$amend_id);
		// 					$update_ammendment_table	->	execute();
		// 			}
		// 	}

		// 	//10112022

		// 	$query_amend 	= 	db_select('l_contractor_activity_tag', 'amend_log');
		// 	$query_amend	->	fields('amend_log', array())->condition('amend_log.flag','A')->condition('amend_log.application_status','I')->condition('amend_log.from_v_serial_no',$serialno);
		// 	$amend_result	=	$query_amend->execute()->fetchAssoc();
		// 	//10112022
			
		// 	if(!empty($amend_result)){	//10112022
			
		// 		$details_ammendment_table   =  check_ammendment_table_license_renewal_ammendment_id($renewallicenseammendment_id);
		// 		// print_r($details_ammendment_table); echo 1111;exit;
		// 	}	//10112022
			
			
			

		// 	//if($check_ammendment_table->rowCount()==0){
		// 	if(empty($details_ammendment_table['id'])|| $details_ammendment_table['id'] == NULL || $details_ammendment_table['id'] == ''){	
		// 		db_insert('l_amendment_of_contract_labour_license')->fields($labour_details)->execute();
		// 		$ammendment_id 	= return_ammendement_id($license_id);
		// 		contractor_activity_tag_insert($serialno,'A',$ammendment_id,NULL);
				
		// 	}else{
			
				
		// 		$update_ammendment 	= 	db_update('l_amendment_of_contract_labour_license');
		//  		$update_ammendment	->	fields($labour_details);
		//  		$update_ammendment	->	condition('license_id',$license_id);
		// 		$update_ammendment	->	execute();
			
			
		// 		$ammendment_id 				= return_ammendement_id($license_id);
				
		// 		contractor_activity_tag_insert($serialno,'A',$ammendment_id,NULL);
				
		// 	}
			
		// 	$message = 2;
		// 	   drupal_goto('amendment_license_renewal/apply_amendment/'.$val['serialno'].'/'.$val['renewallicenseammendment_id'].'/'.$val['applicationflag'].'#'.$message, array('fragment' => 'Labour and Worksite')) ;				 
	
// }

  // New by Jyoti start 23/02/2023
function labour_worksite_info_dev($form, &$form_state ,$serialno='' ,$renewallicenseammendment_id= '',$applicationflag= ''){ 
	//echo "Service Under Maintenance";die;
    drupal_add_library('system','ui.datepicker');

	global $base_root, $base_path, $user;

	$userid = $user->uid;		
	$readonly_contarctor_dob 		= array( 'class' => array('ammendment_date'), 'onclick' => 'datepicker_lastrenewal();','readonly' => 'readonly',);
	$serial_no						=  encryption_decryption_fun('decrypt', $serialno);
	$renewal_license_ammendment_id	=  encryption_decryption_fun('decrypt', $renewallicenseammendment_id);
	$application_flag				=  encryption_decryption_fun('decrypt', $applicationflag);		
	$contractor_regmaster_details 	= contractor_regmaster_details($serial_no);
     
	
	if(!empty($contractor_regmaster_details)){
		$name_of_contractor 	 = 	$contractor_regmaster_details['name_of_contractor'];
		$application_id			 = 	$contractor_regmaster_details['id'];
		$pe_userid				 = 	$contractor_regmaster_details['user_id'];
		$max_no_of_labours_pe	 = 	$contractor_regmaster_details['contractor_max_no_of_labours_on_any_day'];
	}
   
    $getLicenseId = db_query("SELECT id FROM l_contractor_license_application  where serial_no_from_v ='".$serial_no."' and contractor_user_id ='".$userid."' and status='I' ")->fetchAssoc();	
	$license_id    = $getLicenseId['id'];
	
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
		$particular_data 								= 	particular_data($serial_no);

		$max_no_of_labours_license_renewal				= 	$renewal_data['max_of_contract_labour']; 
		$payble_security_fees_license_renewal			=	$renewal_data['payble_security_fees'];	// doubt
		$balance_securit_amount_license_renewal			=	$renewal_data['balance_securit_amount'];
		$due_security_fees_amount_license_renewal		=	$renewal_data['due_security_fees'];	
		$amount_of_licence_renewal						=	$renewal_data['license_reneal_fees'];	
		$payble_security_fees_license_renewal			=	$renewal_data['renewal_security_new']; // doubt

		$contractor_max_labours_license_renewal			= 	$particular_data['contractor_max_no_of_labours_on_any_day']; 
		$worksite_address_line_license_renewal			= 	$particular_data['worksite_address_line'];
		$worksite_dist_license_renewal					= 	$particular_data['worksite_dist'];
		$work_site_areatype_license_renewal				= 	$particular_data['work_site_areatype'];
		$worksite_subdivision_license_renewal			= 	$particular_data['worksite_subdivision'];
		$name_work_site_areatype_license_renewal		= 	$particular_data['name_work_site_areatype'];
		$work_site_vill_ward_license_renewal			= 	$particular_data['work_site_vill_ward'];
		$worksite_pin_license_renewal					= 	$particular_data['worksite_pin'];
		$worksite_ps_license_renewal					= 	$particular_data['worksite_ps'];
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

    
    
	// $fetchTagTable  =  db_select('l_contractor_activity_tag', 'tag');
	// $fetchTagTable	-> fields('tag',array());	
	// $fetchTagTable	-> condition('tag.from_v_serial_no',$serial_no,'=');
	// $fetchTagTable	->	range(0,1);
	// $fetchTagTable	->	orderBy('id', 'DESC');
	// $resultTag		= 	$fetchTagTable->execute()->fetchAssoc(); 

	// if($resultTag['application_status']!='I'){ // flag= A , status =B

	// 	if($resultTag['application_status'] == 'B'){

	// 		$details_ammendment_table	=	db_query('select  * from l_amendment_of_contract_labour_license where  id=:ammendment_id', array(':ammendment_id' =>$resultTag['renewal_license_ammendment_id']))->fetchAssoc();
	// 	}


	// }else{
		$details_ammendment_table  = check_ammendment_table_new($renewal_license_ammendment_id,$serial_no);
	//}
	
	
	
	$worksite_address_line		= !empty($details_ammendment_table['worksite_address_line'])?$details_ammendment_table['worksite_address_line']:$worksite_address_line_license_renewal;
	$worksite_dist				= !empty($details_ammendment_table['worksite_dist'])?$details_ammendment_table['worksite_dist']:$worksite_dist_license_renewal; 
	$work_site_areatype			= !empty($details_ammendment_table['work_site_areatype'])?$details_ammendment_table['work_site_areatype']:$work_site_areatype_license_renewal;
	$worksite_subdivision		= !empty($details_ammendment_table['worksite_subdivision'])?($details_ammendment_table['worksite_subdivision']):$worksite_subdivision_license_renewal;
	$name_work_site_areatype	= !empty($details_ammendment_table['name_work_site_areatype'])?$details_ammendment_table['name_work_site_areatype']:$name_work_site_areatype_license_renewal;
	$work_site_vill_ward		= !empty($details_ammendment_table['work_site_vill_ward'])?$details_ammendment_table['work_site_vill_ward']:$work_site_vill_ward_license_renewal;
	$worksite_pin				= !empty($details_ammendment_table['worksite_pin'])?$details_ammendment_table['worksite_pin']:$worksite_pin_license_renewal;
	$worksite_ps				= !empty($details_ammendment_table['worksite_ps'])?$details_ammendment_table['worksite_ps']:$worksite_ps_license_renewal;
	$pe_userid					= !empty($details_ammendment_table['user_id'])?$details_ammendment_table['user_id']:$pe_userid;
	$application_id				= !empty($details_ammendment_table['application_id'])?$details_ammendment_table['application_id']:$application_id;

	$reference_no				= $details_ammendment_table['reference_no'];

	$is_coparative					= !empty($details_ammendment_table['is_coparative'])?$details_ammendment_table['is_coparative']:(!empty($is_coparative_license_renewal)?$is_coparative_license_renewal:0);
	$max_of_contract_labour_hidden	= !empty($details_ammendment_table['max_of_contract_labour'])?$details_ammendment_table['max_of_contract_labour']: $max_no_of_labours_license_renewal;

	$deposited_max_security_fees = $details_ammendment_table['deposited_max_security_fees'];	
	$ammend_deposit_fees		 = $details_ammendment_table['previous_highest_deposit_fees'];
	$supportive_documents		 = $details_ammendment_table['supportive_documents_worksite'];
	$ammendment_status 			 = trim($details_ammendment_table['ammendment_status']);

	if($ammendment_status == 'I'){		
		$max_of_contract_labour	 = !empty($details_ammendment_table['max_of_contract_labour'])?$details_ammendment_table['max_of_contract_labour']:(!empty($max_no_of_labours_license_renewal)?$max_no_of_labours_license_renewal:$max_no_of_labours_pe);	
	}
	
	if((empty($ammendment_status) || $ammendment_status == 'B' )){	
		
			$max_of_contract_labour_hidden	= !empty($max_no_of_labours_license_renewal)?$max_no_of_labours_license_renewal:$max_no_of_labours_pe;
			$max_of_contract_labour	 = !empty($details_ammendment_table['max_of_contract_labour'])?$details_ammendment_table['max_of_contract_labour']:(!empty($max_no_of_labours_license_renewal)?$max_no_of_labours_license_renewal:$max_no_of_labours_pe);
				
	}
	
			
	$license_renewal_fees				= !empty($details_ammendment_table['license_renewal_fees'])?$details_ammendment_table['license_renewal_fees']:$amount_of_licence_renewal;
	
	$license_renewal_security_fees   	= !empty($details_ammendment_table['license_renewal_security_fees'])?$details_ammendment_table['license_renewal_security_fees']:$amount_of_security_deposit_licence_renewal;
	
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

	$ammendent_fees	=   applicable_ammendment_fees_calculation($max_of_contract_labour);
	if($max_fees2>$ammend_deposit_fees){
		$currecnt_deposit_fees = $max_fees2;
	}if($max_fees2<$ammend_deposit_fees){
		$currecnt_deposit_fees = $ammend_deposit_fees;
	}if($max_fees2 == $ammend_deposit_fees){
		$currecnt_deposit_fees = $ammend_deposit_fees;
	}

	$applicable_amendment_fee   	= !empty($details_ammendment_table['applicable_amendment_fee'])?$details_ammendment_table['applicable_amendment_fee']:!empty($ammendent_fees)?$ammendent_fees:'0';

	if(empty($ammendment_status) || $ammendment_status == 'I'){
		$previous_deposit_fees   		= $currecnt_deposit_fees;
	}else{
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

	$payable_amendment_fees   		= $paymantable_ammenedent_fees;
	$balance_fess   				= $balance_fess1;
	$reason_changing_worksite   	= !empty($details_ammendment_table['reason_changing_worksite'])?$details_ammendment_table['reason_changing_worksite']:'Nil';

	$counter = 1;

	$form['labour_worksite_details']['license_id'] = array(
				'#type'		=> 'hidden',
				'#value'	=> $license_id
		);
	$form['labour_worksite_details']['worksite_dist'] = array(
				'#type'		=> 'hidden',
				'#value'	=> $worksite_dist
		);
	$form['labour_worksite_details']['worksite_subdivision'] = array(
				'#type'		=> 'hidden',
				'#value'	=> $worksite_subdivision
		);
	
	$form['labour_worksite_details']['worksite_areatype'] = array(
				'#type'		=> 'hidden',
				'#value'	=> $work_site_areatype
		);
	$form['labour_worksite_details']['name_worksite_areatype'] = array(
				'#type'		=> 'hidden',
				'#value'	=> $name_work_site_areatype
		);
	$form['labour_worksite_details']['worksite_vill_ward'] = array(
				'#type'		=> 'hidden',
				'#value'	=> $work_site_vill_ward
		);
	$form['labour_worksite_details']['worksite_ps'] = array(
				'#type'		=> 'hidden',
				'#value'	=> $worksite_ps
		);
	$form['labour_worksite_details']['worksite_pin'] = array(
				'#type'		=> 'hidden',
				'#value'	=> $worksite_pin
		);
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
				'#default_value' 	=> !empty($ammendment_status) ? $ammendment_status :'',
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
	}
	if(!empty($remark_field_title) && $remark_type !='I'){	
				$remark_field_title = explode(',', $remark_field_title);	
				foreach($remark_field_title as $row){
					switch($row){
						case  "contractor_max_no_of_labours_on_any_day":
								$readonly_labour	= array('class' => array('readonly_view nolabour'),'onkeyup' => 'checkLabourNumber(this.val);','readonly' => 'readonly');
							break;
						case  "worksite_address":
							$readonly_worksite_address_line1	 = array('class' => array('readonly_view'),'readonly' => 'readonly');
							$readonly_worksite_dist 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
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
	$form['labour_worksite_details']['markup']	= array(
		'#type'		=> 'markup',
		'#markup'	=> '<div class="content">
							<div class="panel panel-default panel-form">
								<div class="panel-heading"><center>Worksite and Contract Labour Details</center></div>
								<div class="panel-body">'
	
		);

	$form['labour_worksite_details']['worksite_address_line'] = array(
		'#prefix' 				=> '<div class="col-md-7"><label class="textarea">',
		'#suffix' 				=> '</label></div>',
		'#type'					=> 'textarea',
		'#rows'					=> 2,
		'#title'				=> $counter++.t('. Worksite Address Line'),
		'#default_value' 		=> $worksite_address_line,
		'#attributes' 			=> array('class' => array('form-control')),
		);
	
	$ammendment = check_ammendment_fields($license_id,$serial_no)->fetchAssoc();
		
	if(!empty($ammendment)){
		$amendment_check_field 	= $ammendment['amendment_check_field'];
			
		$form['labour_worksite_details']['amendment_check_field'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($amendment_check_field) ?  encryption_decryption_fun('encrypt', $amendment_check_field):'',
													
			);	
			if(!empty($amendment_check_field)){
				$editable_field 		= explode(',', $amendment_check_field);
				foreach($editable_field as $row){
					if($row == 'contractor_max_no_of_labours_on_any_day'){
						if($edit_co_oparative == 1){
							$form['labour_worksite_details']['is_coparative'] = array(
									'#prefix' 			=> '<div class="col-md-5 ammend-row" style="min-height:105px;"><div class="inline-group">',
									'#suffix' 			=> '</div></div>',
									'#title'				=>	$counter++.'. Co-oparative Society',
									'#type' 				=> 	'textfield',
									'#size'					=>	120,
									'#default_value' 		=>	!empty($is_coparative) ? 'Yes':'No' ,
									'#required' 			=> 	TRUE,
									'#attributes' 			=>  $readonly_co_oparative,
								);	
							
						}else{	
							$form['labour_worksite_details']['is_coparative'] = array(
										'#prefix' 			=> '<div class="col-md-5 ammend-row" style="min-height:105px;"><div class="inline-group">',
										'#suffix' 			=> '</div></div>',
										'#type' 			=> 'radios',
										'#title' 			=>  $counter++.t('. Co-oparative Society <span class="correction_show con_cate"></span>'),     
										'#options' 			=> array('1'=>t('Yes'),'0'=>t('No')),
										'#required' 		=> TRUE,
										'#default_value' 	=> !empty($is_coparative) ? $is_coparative: 0,
										'#attributes'       => array('onChange' => "check_coparetive();"),
										// '#ajax'			    =>  array(
										// 								'event' 			=> 'change',
										// 								'effect' 			=> 'fade',			
										// 								'callback'			=> 'get_is_coparative',
										// 								'method'			=> 'replace',
										// 								'progress'			=> '',
										// 							) 
										);	
						}
						//echo $max_of_contract_labour;die;
						$form['labour_worksite_details']['no_labour'] = array(
								'#prefix' 				=> '<div class="col-md-4"><div id ="nolabour_id"><label class="input">',
								'#suffix' 				=> '</label></div>',
								'#title'				=>	$counter++.'. Max Contract Labour on any day',
								'#type' 				=> 	'textfield',
								'#size'					=>	120,
								'#default_value' 		=>	!empty($max_of_contract_labour) ? $max_of_contract_labour:'' ,
								'#attributes' 			=> 	$readonly_no_labour,
								'#required' 			=> 	TRUE,
								'#element_validate'		=> array('isnumaric_valiadtion'),
								'#attributes' 			=> $readonly_labour
									
								);
								
						$form['labour_worksite_details']['username_img_markup'] = array(										
										'#type' 		=> 'markup', 
										'#markup' 		=> '<span id="common_username_img_div"></span>'
								);
																			
						$form['labour_worksite_details']['username_markup'] 			= array(
									'#type' 			=> 'markup', 
									'#markup' 			=> '<span id="common_username_div"></span>',
									'#suffix' 			=> '</div>',
							);
								
						$value_is_coparative 	= 	isset($form_state['values']['is_coparative']) ? $form_state['values']['is_coparative'] :$is_coparative;
						$value_contract_labour 	= 	isset($form_state['values']['no_labour']) ? $form_state['values']['no_labour'] :$max_of_contract_labour;
				
						
						//----------------------fees calcution for security-----------------------------------
						$max_license_security_fees 			= return_maximum_license_security_fees($serial_no,$userid);
						$max_ammendment_security_fess       = return_maximum_ammendment_security_fess($license_id,$userid);
						$max_renewal_security_fess			= return_maximum_renewal_security($license_id,$userid);
					
					
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
			
						$payable_amendment_security_fees_pre = $details_ammendment_table['payable_amendment_security_fees'];
					
						if($ammendment_status == 'I')
						$previous_max_security 		= $current_security_deposit+$payable_amendment_security_fees_pre;
						
						if(empty($ammendment_status))
						$previous_max_security 		= $current_security_deposit;
						if($ammendment_status == 'B')
						$previous_max_security 		= !empty($details_ammendment_table['previous_highest_security'])?$details_ammendment_table['previous_highest_security']:$current_security_deposit;

						
						if(!empty($ammendment_status) && $ammendment_status=='I')
						
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
						
					
						$applicable_security_fees   = !empty($details_ammendment_table['applicable_security_fess']) ?  $details_ammendment_table['applicable_security_fess'] : (!empty($ammendent_security_fees) ? $ammendent_security_fees : '0');
				 
						$payable_amendment_security_fees   =  $paymantable_sucurity_ammenedent_fees;
							
											
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
						$license_renewal_security_fees = 0;					
						$form['labour_worksite_details']['security_fees'] = array(
									'#prefix' 		=> '<div id ="security_fees_id"><div class="col-md-4"><label class="input">',
									'#suffix' 		=> '</label></div></div>',
									'#title'		=>	$counter++.'. Applicable Security Fees',
									'#type' 		=> 	'textfield',
									'#size'			=>	120,
									'#default_value'=>	!empty($applicable_security_fees) ? $applicable_security_fees : $license_renewal_security_fees,
									'#required' 	=> 	TRUE,
									'#attributes' 	=>  $readonly_security_fees,
								);	
							
						$form['labour_worksite_details']['ammendment_fees'] = array(
								'#prefix' 				=> '<div id ="ammendment_fees_id"><div class="col-md-4"><label class="input">',
								'#suffix' 				=> '</label></div></div>',
								'#title'				=>	$counter++.'. Applicable Amendment Fees',
								'#type' 				=> 	'textfield',
								'#size'					=>	120,
								'#default_value' 		=>	!empty($applicable_amendment_fee) ? $applicable_amendment_fee:'' ,
								'#required' 			=> 	TRUE,
								'#element_validate'		=>  array('isnumaric_valiadtion'),
								'#attributes'			=>	$readonly_ammendment_fees,
							);
							
						$form['labour_worksite_details']['due_security_fees'] = array(
								'#prefix' 				=> '<div id="due_security_fees_view"><div class="col-md-4"><label class="input">',
								'#suffix' 				=> '</label></div></div>',
								'#title'				=>	$counter++.'. Due Security Fees',
								'#type' 				=> 	'textfield',
								'#size'					=>	120,
								'#default_value' 		=>	!empty($due_security_fees_amount_license_renewal) ? $due_security_fees_amount_license_renewal:'0.0' ,
								'#required' 			=> 	TRUE,
								'#element_validate'		=> array('isnumaric_valiadtion'),
								'#attributes' 			=> $readonly_due_security_fees,
							);
							
						$form['labour_worksite_details']['payable_amendment_fees'] = array(
								'#prefix' 				=> '<div id="payable_amendment_fees_id"><div class="col-md-4"><label class="input">',
								'#suffix' 				=> '</label></div></div>',
								'#title'				=>	$counter++.'. Payble Amendment Fees',
								'#type' 				=> 	'textfield',
								'#size'					=>	120,
								'#default_value' 		=>	!empty($payable_amendment_fees) ? $payable_amendment_fees:'0.00' ,
								'#attributes' 			=> 	array('placeholder' => 'Address Line1 the contractor'),
								'#required' 			=> 	TRUE,
								'#element_validate'		=> array('isnumaric_valiadtion'),
								'#attributes'			=>	$readonly_ammendment_fees,
							);
						$form['labour_worksite_details']['payble_security_fees'] = array(
								'#prefix' 				=> '<div id="payble_security_fees_id"><div class="col-md-4"><label class="input">',
								'#suffix' 				=> '</label></div></div>',
								'#title'				=>	$counter++.'. <span style="font-color:#FF0000;">Payble Amendment security Fees</span>',
								'#type' 				=> 	'textfield',
								'#size'					=>	120,
								'#default_value' 		=>	!empty($payable_amendment_security_fees) ? $payable_amendment_security_fees:'0.0' ,
								'#attributes' 			=>  $readonly_payble_security_fees,
								'#required' 			=> 	TRUE,
								'#element_validate'		=> array('isnumaric_valiadtion'),
								
							);
							
						$form['labour_worksite_details']['perevious_fees'] = array(
								'#prefix' 				=> '<div class="col-md-4"><label class="input">',
								'#suffix' 				=> '</label></div>',
								'#title'				=>	$counter++.'. Highest Fees paid during license/subsequent amendments ',
								'#type' 				=> 	'textfield',
								'#size'					=>	120,
								'#default_value' 		=>	!empty($previous_deposit_fees) ? $previous_deposit_fees:'' ,
								'#attributes' 			=> 	array('placeholder' => 'Address Line1 the contractor'),
								'#required' 			=> 	TRUE,
								'#element_validate'		=>  array('isnumaric_valiadtion'),
								'#attributes' 			=> 	array('id' => 'currecnt_deposit_fees','class' => array('readonly_view'),'readonly' => 'readonly'),
								
							);
							
						$form['labour_worksite_details']['perevious_security_fees'] = array(
								'#prefix' 				=> '<div class="col-md-4"><label class="input">',
								'#suffix' 				=> '</label></div>',
								'#title'				=>	$counter++.'. Highest Security Fees paid during license/subsequent amendments ',
								'#type' 				=> 	'textfield',
								'#size'					=>	120,
								'#default_value' 		=>	!empty($previous_max_security) ? $previous_max_security:'' ,
								'#attributes' 			=> 	array('placeholder' => 'Address Line1 the contractor'),
								'#required' 			=> 	TRUE,
								'#element_validate'		=> array('isnumaric_valiadtion'),
								'#attributes'			=>	$readonly_previous_security_fees,
							);
					}
			    }
	        }
	    }
				 

	$form['labour_worksite_details']['back_link']	 = array(
			'#prefix' 		=> '<br class="clear"><div class="col-md-12">',
			'#suffix' 		=> '', 
			'#markup' 		=> l(t('Back'),'amendment_license_renewal/apply_amendment/'. $serialno.'/'.$renewallicenseammendment_id.'/'. $applicationflag, array('html' => TRUE,'attributes'=> array('class'=>array('btn btn-primary left-btn')))),															
		);	

	$form['labour_worksite_details']['submit'] = array (
		'#prefix' 		=> '',
		'#suffix' 		=> '</div>',  
		'#type' 		=> 'submit',
		'#attributes' 	=> array('class' => array('btn btn-primary pull-right')),
		'#value' 		=> 'Save'										  
		);			
	$form['labour_worksite_details']['footer']	= array(
		'#type'		=> 'markup',
		'#markup'	=> '</div></div></div>'
	     );							
	return $form;
}
function labour_worksite_info_dev_submit($form, &$form_state){

	global $base_root, $base_path, $user;
	$labour_details = array();
		
	 $contractor_id 		= 	$user->uid;
	 $val					=	$form_state['values']; 
	 $supportive_doc_file 	= fileupload($val['supportive_doc']);
	 $view_supportive_doc	= $val['view_supportive'];

	 if ($supportive_doc_file!="")  {
		$supportive_documents_fid			 =	$supportive_doc_file->fid;
		$supportive_documents_worksite_name  =	$supportive_doc_file->filename;
	}else{
		 $supportive_documents_fid			 =	$view_supportive_doc;
	}
	
	//  16/05/2023 start  ///////////////
	$serial_no							=  encryption_decryption_fun('decrypt', $val['serialno']);
	$labour_no  						=  $val['no_labour']; 
	$flag								=  encryption_decryption_fun('decrypt',$val['applicationflag']);
	$currecnt_deposit_fees				=  encryption_decryption_fun('decrypt',$val['currecnt_deposit_fees']);
	 
	$max_no_of_labours_license_renewal	=  encryption_decryption_fun('decrypt',$val['max_of_contract_labour_hidden']);
	 
	$record	   							=  contractor_regmaster_details($serial_no);
	$max_labour_pe						=  $record['contractor_max_no_of_labours_on_any_day'];
	
	$max_labour_previous				=	encryption_decryption_fun('decrypt',$val['max_labour_previous']);
	
	$ammendent_fees						=   applicable_ammendment_fees_calculation($labour_no);
	 
	$co_oparative_yes					=	$val['is_coparative'];
	$due_security_fees					=   $val['due_security_fees'];
	if($co_oparative_yes == 'true')
		$co_oparative = 1;
	if($co_oparative_yes == 'false')
		$co_oparative = 0;
   
    $previous_deposit_fees 		 =   encryption_decryption_fun('decrypt',$val['previous_security']);
 
	$ammendent_security_fees	 	=   applicable_ammendment_security_fees_calculation($labour_no,$co_oparative);

	if($ammendent_security_fees>$previous_deposit_fees){
		$paymantable_sucurity_ammenedent_fees = $ammendent_security_fees-$previous_deposit_fees;
		if(!empty($due_security_fees) || $due_security_fees!=0){
			
			$paymantable_sucurity_ammenedent_fees 		= $paymantable_sucurity_ammenedent_fees+$due_security_fees;
			$deposited_security_fees					= $ammendent_security_fees;
		}else{
			$deposited_security_fees					= $ammendent_security_fees;
		}
	}if($ammendent_security_fees<$previous_deposit_fees){
		$paymantable_sucurity_ammenedent_fees = 0.00;
		if(!empty($due_security_fees) || $due_security_fees!=0){
			
			$paymantable_sucurity_ammenedent_fees 		= $due_security_fees;
			$deposited_security_fees					= $previous_deposit_fees;
		}else{
			
			$deposited_security_fees					= $previous_deposit_fees;
		}
	}if($ammendent_security_fees==$previous_deposit_fees){
		$paymantable_sucurity_ammenedent_fees = 0.00;
		if(!empty($due_security_fees) || $due_security_fees!=0){
			
			$paymantable_sucurity_ammenedent_fees 		= $due_security_fees;
			$deposited_security_fees					= $previous_deposit_fees;
		}else{
			
			$deposited_security_fees					= $previous_deposit_fees;
		}
	}
	
	if($max_labour_pe>$labour_no){

		if($max_no_of_labours_license_renewal<$labour_no){ 
			
			if($max_labour_previous > 400){
				$payble 				= 0.00;
				$deposit_fees_value		= $currecnt_deposit_fees;
				$ammendent_fees 		= $ammendent_fees;
		
			}
			if($max_labour_previous <=400){
				if($currecnt_deposit_fees>$ammendent_fees){
					$payble 				= 0.00;
					$deposit_fees_value		= $currecnt_deposit_fees;
					$ammendent_fees 		= $ammendent_fees;
				}if($currecnt_deposit_fees<$ammendent_fees){
			  		$payble 					= $ammendent_fees-$currecnt_deposit_fees;
			  		$deposit_fees_value			= $ammendent_fees;
				    $ammendent_fees 			= $ammendent_fees;
				}if($currecnt_deposit_fees==$ammendent_fees){
			  		$payble 					= 0.00;
			  		$deposit_fees_value			= $ammendent_fees;
				    $ammendent_fees 			= $ammendent_fees;
				}
			}
			$output =  'Your are eligible for increasing contract labour to :'.$labour_no.'.Since maximum number of labour to be engaged by you as specified by your Principal Employer is '.$max_labour_pe;
		
		}
		if($max_no_of_labours_license_renewal>$labour_no){ 
					$payble 								= NULL;
			  		$deposit_fees_value						= NULL;
				    $ammendent_fees 						= NULL;
					$paymantable_sucurity_ammenedent_fees 	= NULL;
			  		$deposited_security_fees				= NULL;
				    $ammendent_security_fees 				= NULL;
		
		    $output =  'Please contact your Principal Employer';
				
		}
		if($max_no_of_labours_license_renewal==$labour_no){ 
			
		
			if($max_labour_previous > 400){
				$payble 				= 0.00;
				$deposit_fees_value		= $currecnt_deposit_fees;
				$ammendent_fees 		= $ammendent_fees;
		
			}if($max_labour_previous <=400){
				if($currecnt_deposit_fees>$ammendent_fees){
					$payble 				= 0.00;
					$deposit_fees_value		= $currecnt_deposit_fees;
					$ammendent_fees 		= $ammendent_fees;
				}if($currecnt_deposit_fees<$ammendent_fees){
			  		$payble 					= $ammendent_fees-$currecnt_deposit_fees;
			  		$deposit_fees_value			= $ammendent_fees;
				    $ammendent_fees 			= $ammendent_fees;
				}if($currecnt_deposit_fees==$ammendent_fees){
			  		$payble 					= 0.00;
			  		$deposit_fees_value			= $ammendent_fees;
				    $ammendent_fees 			= $ammendent_fees;
				}
			}
		  $output =  'Your request for increasing contract labour is declined since the number is specified by your Principal Employer is '.$max_labour_pe.'.Please contact your Principal Employer' ;	
		}
	}
	if($max_labour_pe == $labour_no){ 
		
		if($max_labour_previous > 400){
				$payble 				= 0.00;
				$deposit_fees_value		= $currecnt_deposit_fees;
				$ammendent_fees 		= $ammendent_fees;
		
			}if($max_labour_previous <=400){
				if($currecnt_deposit_fees>$ammendent_fees){
					$payble 				= 0.00;
					$deposit_fees_value		= $currecnt_deposit_fees;
					$ammendent_fees 		= $ammendent_fees;
				}if($currecnt_deposit_fees<$ammendent_fees){
			  		$payble 					= $ammendent_fees-$currecnt_deposit_fees;
			  		$deposit_fees_value			= $ammendent_fees;
				    $ammendent_fees 			= $ammendent_fees;
				}if($currecnt_deposit_fees==$ammendent_fees){
			  		$payble 					= 0.00;
			  		$deposit_fees_value			= $ammendent_fees;
				    $ammendent_fees 			= $ammendent_fees;
				}
			}
		$output =  'Your request for increasing contract labour is:'.$labour_no.'.You have number of labour(s) is(are)'.$max_no_of_labours_license_renewal.'.Rest number of labour'.$rest_labour.'.Assigned labour by PE'.$max_labour_pe.'.You are eligible for increasing number of labour is'.$max_labour_pe;	
		
	
	}
	if($max_labour_pe<$labour_no){
		
			$payble 								= NULL;
			$deposit_fees_value						= NULL;
			$ammendent_fees 						= NULL;
			$paymantable_sucurity_ammenedent_fees 	= NULL;
			$deposited_security_fees				= NULL;
			$ammendent_security_fees 				= NULL;
			$output 					= 'Your request for increasing contract labour is declined since the number is specified by your Principal Employer is '.$max_labour_pe.'.Please contact your Principal Employer' ;	
		
	}
  
	$val['ammendment_fees'] = $ammendent_fees;

	if($payble == null){
		$payable_amendment_fees = 0.00;
	}else{
		$payable_amendment_fees = $payble;
	}
	$val['payable_amendment_fees'] = $payable_amendment_fees;
	$val['deposit_fees_value'] = encryption_decryption_fun('encrypt',$deposit_fees_value);
	$val['security_fees'] = $ammendent_security_fees;
	$val['payble_security_fees'] = $paymantable_sucurity_ammenedent_fees;
	$val['deposited_security_fees'] = encryption_decryption_fun('encrypt',$deposited_security_fees);
					
	//  16/05/2023 end    ///////////////////


	//$id									= 	$val['id'];
	//$status_from 							= 	$val['status_from'];
	$license_id 							=   $val['license_id'];
	$renewallicenseammendment_id			=	encryption_decryption_fun('decrypt', $val['renewallicenseammendment_id']);
	$applicationflag						=	encryption_decryption_fun('decrypt', $val['applicationflag']);
	$serialno								=	encryption_decryption_fun('decrypt', $val['serialno']);
	$application_id							=	encryption_decryption_fun('decrypt', $val['application_id']);
	$pe_userid								=	encryption_decryption_fun('decrypt', $val['pe_userid']);
	$amendment_check_field					=   encryption_decryption_fun('decrypt', $val['amendment_check_field']);
	$ammendment_status						=	$val['ammendment_status']; 
	//echo $ammendment_status; die;
	
	//$query = db_query("SELECT ammendment_status FROM l_amendment_of_contract_labour_license  where license_id ='".$license_id."'");	
	//$ammendment_status = $query->fetchField();
	


	$editable_field 						=   explode(',', $amendment_check_field);
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
					//$is_coparative							=	$val['is_coparative'];
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
		
	if(trim($ammendment_status)=='I'){	
		 

		$amendment_rows_check = db_select('l_amendment_of_contract_labour_license', 'amd')->fields('amd',array('id'))->condition('license_id',$license_id)->execute();
		if($amendment_rows_check->rowCount() > 1){
			$applicant_mailto  = 'wblc.nic@gmail.com';
			$subject = 'Amendment Issue';
			$msg = 'Multiple row for amendment for License Id -'.$license_id.' Form-V-No -'.$serial_no.'. Place- labour worksite info';
			if($applicant_mailto){
				send_mail_for_user_alert($applicant_mailto, $subject, $msg);
			}
			form_set_error('reference_no' ,t('We apologize. There is a problem. Please try again later.'));
		}
    }
	
	

	$details_ammendment_table   =  check_ammendment_table_new($renewallicenseammendment_id,$serialno);

	if(empty($details_ammendment_table['id'])|| $details_ammendment_table['id'] == NULL || $details_ammendment_table['id'] == ''){ 

		db_insert('l_amendment_of_contract_labour_license')->fields($labour_details)->execute();
		$ammendment_id 	= return_ammendement_id($license_id);		
		contractor_activity_tag_insert($serialno,'A',$ammendment_id,NULL);

	}else{ 	
		
		$ammendment_id 	= trim($details_ammendment_table['id']);	

		if(trim($ammendment_status)=='I'){			
			
			// changes made 18/03/2024
			
			amemd_log($details_ammendment_table['id']);

			
			$upadte_array	=	 array(						
				'ammendment_status'			=>	'',
				'ammendment_final_status'	=>	'',
				'created_date'				=>	NULL,
				'alc_inspector_action_date'	=>	NULL,
				'remark_by_userid'			=>	NULL,
				'remark_by_userid'			=>	NULL,
				'ammendment_certificate_id'	=>  NULL,
				'modified_date_time'		=> date('Y-m-d H:i:s')
			);
			
			$labour_details = (array_merge($labour_details,$upadte_array));	

			// $update_ammendment_table 	= 	db_update('l_amendment_of_contract_labour_license');
			// $update_ammendment_table	->	fields($upadte_array);
			// $update_ammendment_table	->	condition('id',$details_ammendment_table['id']);
			// $update_ammendment_table	->	execute();

			
			$update_ammendment 	= 	db_update('l_amendment_of_contract_labour_license');
			$update_ammendment	->	fields($labour_details);
			$update_ammendment	->	condition('license_id',$license_id);
			$update_ammendment	->	condition('id',$details_ammendment_table['id']);
			$update_ammendment	->	execute();

			
			contractor_activity_tag_insert($serialno,'A',$details_ammendment_table['id'],NULL);
			

			

		} else if(trim($ammendment_status)=='B'){
			
					
			$update_ammendment_table 	= 	db_update('l_amendment_of_contract_labour_license');
			$update_ammendment_table	->	fields(array('modified_date_time' => date('Y-m-d H:i:s')));
			$update_ammendment_table	->	condition('id',$ammendment_id);
			$update_ammendment_table	->	execute();
		

			$update_ammendment 	= 	db_update('l_amendment_of_contract_labour_license');
			$update_ammendment	->	fields($labour_details);
			$update_ammendment	->	condition('license_id',$license_id);
			$update_ammendment	->	condition('id',$details_ammendment_table['id']);
			$update_ammendment	->	execute();

		}	
		
	}
	
	$message = 2;
	drupal_goto('amendment_license_renewal/apply_amendment/'.$val['serialno'].'/'.$val['renewallicenseammendment_id'].'/'.$val['applicationflag'].'#'.$message, array('fragment' => 'Labour and Worksite')) ;				 
	
}
// New by Jyoti end 23/02/2023
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
