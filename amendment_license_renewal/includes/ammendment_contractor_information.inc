<?php
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
//drupal_add_js(drupal_get_path('module', 'amendment_license_renewal') . '/js/ammendment.js');

function ammendment_contractor_information($form, &$form_state ,$serial_no='' ,$renewallicenseammendment_id= '',$applicationflag= ''){
	drupal_add_library('system','ui.datepicker');
	
	drupal_add_js('jQuery(document).ready(function(){
	jQuery("#edit-contarctor-dob").datepicker({
		  dateFormat: "dd-mm-yy",
		  minDate: "0",
		  yearRange: "-60:+1",
		  changeMonth: true, 
		  changeYear: true,
		  showOn: "button",  
		  buttonImage: "https://wblc.gov.in/sites/default/files/images/calendar.png",
		  buttonImageOnly: true
		});
	});', 'inline');
	
	$readonly_contarctor_dob 		= array('readonly' => 'readonly',);
	$serialno						=  encryption_decryption_fun('decrypt', $serial_no);
	$renewal_license_ammendment_id	=  encryption_decryption_fun('decrypt', $renewallicenseammendment_id);
	
	$application_flag				=  encryption_decryption_fun('decrypt', $applicationflag);
	
	//$check_ammendment_table         =  check_ammendment_table($renewal_license_ammendment_id);
	
	
	//$check_ammendment_table 		= check_ammendment_table($renewal_license_ammendment_id);			

	$contractor_regmaster_details 	= contractor_regmaster_details($serialno);
	if(!empty($contractor_regmaster_details)){
		$name_of_contractor 	 = 	$contractor_regmaster_details['name_of_contractor'];
		$application_id			 = 	$contractor_regmaster_details['id'];
		$pe_userid				 = 	$contractor_regmaster_details['user_id'];
	}
	//if($application_flag == 'L'){
		
		$particular_data = particular_data($serialno);
		
		if(!empty($particular_data)){
			$father_contarctor_name_license_renewal 		= $particular_data['father_contarctor_name'];
			$address_line1_of_contractor_license_renewal	= $particular_data['address_of_contractor']; 
			$contractor_dist_license_renewal				= $particular_data['contractor_dist']; 
			$contractor_subdivision_license_renewal			= $particular_data['contractor_subdivision']; 
			$contractor_name_areatype_license_renewal	    = $particular_data['contractor_name_areatype'];
			$contractor_areatype_license_renewal	    	= $particular_data['contractor_areatype'];
			$contractor_vill_ward_license_renewal	    	= $particular_data['contractor_vill_ward'];
			$contractor_ps_license_renewal	    			= $particular_data['contractor_ps'];
			$contractor_pin_license_renewal	    			= $particular_data['contractor_pin'];
			$contractor_state_license_renewal	    		= $particular_data['contractor_state'];
			$contrcator_country_license_renewal	    		= $particular_data['contrcator_country'];
			$category_of_contractor_license_renewal			= $particular_data['category_of_contractor'];
			$dob_contractor_license_renewal					= $particular_data['dob_contractor'];
			$age_contractor_license_renewal					= $particular_data['age_contractor']; 
		}
	// }
	
  
	       // $record							= $check_ammendment_table->fetchAssoc();
		   $record  = check_ammendment_table_new($renewal_license_ammendment_id,$serialno);
		    //$record							= check_ammendment_table_license_renewal_ammendment_id($renewal_license_ammendment_id);
			
			if($record['category_of_contractor'] == 0 || $record['category_of_contractor']==1){
				$category_of_contractor =$record['category_of_contractor'];
			}if($record['category_of_contractor']!=0 &&  $record['category_of_contractor']!=1){
				$category_of_contractor =$category_of_contractor_license_renewal;
			}
			
			
			$name_of_contractor 			= !empty($record['name_of_contractor'])?$record['name_of_contractor']:$name_of_contractor;
	 		$father_contarctor_name 		= !empty($record['father_contarctor_name'])?$record['father_contarctor_name']:$father_contarctor_name_license_renewal;
			$address_line1_of_contractor	= !empty($record['address_of_contractor'])?$record['address_of_contractor']:$address_line1_of_contractor_license_renewal; 
			$contractor_dist				= !empty($record['contractor_dist'])?$record['contractor_dist']:$contractor_dist_license_renewal; 
			$contractor_subdivision			= !empty($record['contractor_subdivision'])?$record['contractor_subdivision']:$contractor_subdivision_license_renewal; 
			$contractor_name_areatype	    = !empty($record['contractor_name_areatype'])?$record['contractor_name_areatype']:$contractor_name_areatype_license_renewal;
			$contractor_areatype	    	= !empty($record['contractor_areatype'])?$record['contractor_areatype']:$contractor_areatype_license_renewal;
			$contractor_vill_ward	    	= !empty($record['contractor_vill_ward'])?$record['contractor_vill_ward']:$contractor_vill_ward_license_renewal;
			$contractor_ps	    			= !empty($record['contractor_ps'])?$record['contractor_ps']:$contractor_ps_license_renewal;
			$contractor_pin	    			= !empty($record['contractor_pin'])?$record['contractor_pin']:$contractor_pin_license_renewal;
			$contractor_state	    		= !empty($record['contractor_state'])?$record['contractor_state']:$contractor_state_license_renewal;
			$contractor_country	    		= !empty($record['contractor_country'])?$record['contractor_country']:$contrcator_country_license_renewal;
			
			//$category_of_contractor			= !empty($record['category_of_contractor'])?$record['category_of_contractor']:$category_of_contractor_license_renewal;
			$dob_contractor					= !empty($record['dob_contractor'])?$record['dob_contractor']:$dob_contractor_license_renewal;
			$pe_userid						= !empty($record['user_id'])?$record['user_id']:$pe_userid;
			$application_id					= !empty($record['application_id'])?$record['application_id']:$application_id;
			$reference_no					= !empty($record['reference_no'])?$record['reference_no']:'';
			$age_contractor					= !empty($record['age_contractor'])?$record['age_contractor']:$age_contractor_license_renewal; 
			//echo $age_contractor.'--'.$record['age_contractor'].'--'.$age_contractor_license_renewal;die;
			$reference_no					= $record['reference_no'];
			$ammendment_status				= $record['ammendment_status'];
	   		$last_remark_activity 			= last_remark_activity($reference_no);
		   $remark_field_title 				= $last_remark_activity['remark_field_title'];
		   $remark_type 					= $last_remark_activity['remark_type'];
		   $readonly_contract_name			= array('placeholder' => 'Enter your name','class' => array('readonly_view'),'readonly' => 'readonly');
		   if((empty($remark_field_title) && $remark_type =='B')){
			   $readonly_contract_name			= array('placeholder' => 'Enter your name','class' => array('readonly_view'),'readonly' => 'readonly');
			   $readonly_father_contarctor_name = array('placeholder' => 'Enter your Father Name', 'class' => array('correction_show'));
			   $readonly_address_line1 			= array('placeholder' => 'Enter your address','class' => array('correction_show'));
			   $readonly_contract_dist 			= array('class' => array('correction_show'));
			   $readonly_contarctor_age 		= array('class' => array('correction_show'));
			   $readonly_category_of_contractor = array('class' => array('correction_show'));
			   $readonly_contarctor_dob 		= array('class' => array('correction_show'), 'onclick' => 'datepicker_lastrenewal();','readonly' => 'readonly',);
			  
		   }if((!empty($remark_field_title) && $remark_type =='B')){
			 	$readonly_contract_name			= array('placeholder' => 'Enter your name','class' => array('readonly_view'),'readonly' => 'readonly');
			   $readonly_father_contarctor_name = array('placeholder' => 'Enter your Father Name', 'class' => array('correction_show'));
			   $readonly_address_line1 			= array('placeholder' => 'Enter your address','class' => array('correction_show'));
			   $readonly_contract_dist 			= array('class' => array('correction_show'));
			   $readonly_contarctor_age 		= array('class' => array('correction_show'));
			   $readonly_category_of_contractor = array('class' => array('correction_show'));
			   $readonly_contarctor_dob 		= array('class' => array('correction_show'), 'onclick' => 'datepicker_lastrenewal();','readonly' => 'readonly',);
			  
				
	} 
		   
		   if(!empty($remark_field_title) && $remark_type =='I'){
			 $readonly_contract_name			= array('placeholder' => 'Enter your name','class' => array('readonly_view'),'readonly' => 'readonly');
			   $readonly_father_contarctor_name = '';
			   $readonly_address_line1 			= '';
			   $readonly_contract_dist 			= '';
			   $readonly_contarctor_age 		= '';
			   $readonly_category_of_contractor = '';
			   $readonly_contarctor_dob 		= '';
		   }if(!empty($remark_field_title) && $remark_type !='I'){
				
			   $remark_field_title = explode(',', $remark_field_title);	
			 //  print_r($remark_field_title);die();
				foreach($remark_field_title as $row){
					switch($row){
						case  "name_of_contractor":
							 $readonly_contract_name	= array('placeholder' => 'Enter your name','class' => array('readonly_view'),'readonly' => 'readonly');
							 break;
						case  "contractor_dist":
							$readonly_contract_dist 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));
							$readonly_address_line1 	= array('placeholder' => 'Enter your Father Name', 'readonly' => 'readonly', 'class' => array('readonly_view'));
							break;
						case  "father_name_of_contarctor":
							 $readonly_father_contarctor_name = array('placeholder' => 'Enter your Father Name', 'readonly' => 'readonly', 'class' => array('readonly_view'));
							break;
						case   "category_of_contractor":
							 $readonly_category_of_contractor  = array('readonly' => 'readonly', 'class' => array('readonly_view'));
							  $readonly_contarctor_age 		= array('readonly' => 'readonly', 'class' => array('readonly_view'));
							$readonly_contarctor_dob 		= array('class' => array('readonly_view'), 'onclick' => 'datepicker_lastrenewal();','readonly' => 'readonly',);
							 $edit = 1;
							break;
					}
				}
		   }
	//echo $category_of_contractor;die();
	/*$form['apply_amendment_contrcator_information']		= array(
				'#title' 				=>	 t('<div class="content"><div class="sky-form" style="width:100%;">'),
				'#type' 				=> 	'fieldset',
				'#attributes' 			=> 	array('class'=>array('')),
				'#collapsed' 			=> 	TRUE,
				'#collapsable'			=>	TRUE
				);*/
	$form['apply_amendment_contrcator_information']['renewallicenseammendment_id'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($renewallicenseammendment_id) ? $renewallicenseammendment_id:'',
				
				
													
		 );	
	$form['apply_amendment_contrcator_information']['applicationflag'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($applicationflag) ? $applicationflag:'',
				
				
													
		 );	
	$form['apply_amendment_contrcator_information']['serialno'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($serial_no) ? $serial_no:'',
				
				
													
		 );				 
	$form['apply_amendment_contrcator_information']['application_id'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($application_id) ?  encryption_decryption_fun('encrypt', $application_id):'',
				
				
													
		 );	
	$form['apply_amendment_contrcator_information']['pe_userid'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($pe_userid) ?  encryption_decryption_fun('encrypt', $pe_userid):'',
				
				
													
		 );	
		 $form['apply_amendment_contrcator_information']['reference_no'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($reference_no) ?  encryption_decryption_fun('encrypt', $reference_no):'',
				
				
													
		 );		
	      
	 $form['apply_amendment_contrcator_information']['ammendment_status'] = array(
				'#type' 			=> 'hidden',
				'#default_value' 	=> !empty($ammendment_status) ?  $ammendment_status:'',
				
				
													
		 );				
								
	/*$form['apply_amendment_contrcator_information']['contarctor_name'] = array(
				'#prefix' 			=> '<fieldset><legend>Contractor Details</legend><div class="row"><section class="col col-md-6"><label class="input">',
				'#suffix' 			=> '</label></section>',
				'#type' 			=> 'textfield',
				'#title' 			=> t('Name of Contractor'),  
				'#default_value' 	=> !empty($name_of_contractor) ? $name_of_contractor:'',
				'#attributes' 		=> 	$readonly_contract_name,
				
													
		 );*/
			$form['apply_amendment_contrcator_information']['contarctor_name']			= array(
						'#markup' 			=>	 t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading"><center>Contractor Details</center>
												
														</div>') );
																				
	$form['apply_amendment_contrcator_information']['option_ammend']			= array(
						'#prefix' 			=> 	'<div class="panel-body"><div class="sky-form">',
			 );	
	 $form['apply_amendment_contrcator_information']['start_design'] 	= array(
															
								'#prefix' 			=> '<div class="cus-amment-check-box"><div class="col-sm-12"><h4 class="form-sub-title">Name & address of the contractor(Official Information)</h4></div>',
								
			);		
							 
	if($edit=='1'){
		  
		   $form['apply_amendment_contrcator_information']['category_show'] = array(
						'#prefix' 			=> '<section class="col col-md-6"><label class="input">',
						'#suffix' 			=> '</label></section>',
						'#type' 			=> 'textfield',
						'#title' 			=> t('Category of Contractor'),  
						'#default_value' 	=> !empty($category_of_contractor) ? 'Company':'Individual',
						'#attributes' 		=> 	$readonly_category_of_contractor,
						
															
					  );
								  
			$form['apply_amendment_contrcator_information']['category'] = array(
	 					'#type' 			=> 'hidden',
						'#default_value' 	=> !empty($category_of_contractor) ? $category_of_contractor : '0',
					   );
			}else{
				
			$form['apply_amendment_contrcator_information']['category'] = array(
						'#prefix' 			=> '<section class="col col-md-6 ammend-row"><div class="inline-group ">',
						'#suffix' 			=> '</div></section>',
						'#type' 			=> 'radios',
						'#title' 			=>  t('Category of Contractor:<span class="correction_show con_cate"></span>'),     
						'#options' 			=> array('0'=>t('Individual'),'1'=>t('Company')),
						'#required' 		=> TRUE,
						'#default_value' 	=> !empty($category_of_contractor) ? $category_of_contractor : 0,
						
						
						'#ajax'			    =>  array(
														'event' 			=> 'change',
														'effect' 			=> 'fade',			
														'callback'			=> 'get_category',
														'progress'			=> '',
													 ) 
					 );
			}
								 
								 
		$form['apply_amendment_contrcator_information']['contarctor_fathername'] = array(
				'#prefix' 			=>  '<div id="add_fathername">',
				'#suffix' 			=>  '</div>',
					
				);
									
		$form['apply_amendment_contrcator_information']['contarctor_dob'] = array(
				'#prefix' 			=>  '<div id="add_dob">',
				'#suffix' 			=>  '</div>',
												
				);	
		$form['apply_amendment_contrcator_information']['contarctor_age'] = array(
					'#prefix' 			=>  '<div id="add_age">',
					'#suffix' 			=>  '</div>',
												
				);	
									
	 $value_contractor_category 	= 	isset($form_state['values']['category']) ? $form_state['values']['category'] :$category_of_contractor;
									
		if(!empty($value_contractor_category)&&($value_contractor_category=='1')){	
	 	
				 $form['apply_amendment_contrcator_information']['contarctor_dob'] 	= array(
							'#prefix' 					=>'<div id="add_dob"><section class="col col-md-6"><label class="input">',
							'#suffix' 					=> 	'</label></section></div>',
							'#type' 					=> 	'textfield',
							'#title'					=>	'Date of commencement of business',
							'#attributes'				=>	$readonly_contarctor_dob,
							'#default_value' 			=> 	!empty($dob_contractor) ? $dob_contractor : ''
						);		
		  
				
	
										  
		}else{
			$form['apply_amendment_contrcator_information']['contarctor_dob'] 	= array(
							'#prefix' 					=>'<div id="add_dob"><section class="col col-md-6"><label class="input">',
							'#suffix' 					=> 	'</label></section></div>',
							'#type' 					=> 	'textfield',
							'#title'					=>	'Date of birth',
							'#attributes'				=>	$readonly_contarctor_dob,
						 '#default_value' 			=> 	!empty($dob_contractor) ? $dob_contractor : ''
						);		
								 
		$form['apply_amendment_contrcator_information']['contarctor_fathername'] = array(
						'#prefix' 			=>  '<div id="add_fathername"><section class="col col-md-6"><label class="input">',
						'#suffix' 			=>  '</label></section></div>',
						'#title'			=>	'Father name of Contractor :',
						'#size'				=>	t('20'),	
						'#type'				=> 	'textfield',
						'#default_value' 	=>	!empty($father_contarctor_name) ? $father_contarctor_name : '',
						'#attributes' 		=> 	$readonly_father_contarctor_name,
						
					  );
		
				
			$form['apply_amendment_contrcator_information']['contarctor_age'] = array(
					'#prefix'				=> '<div id="add_age"><section class="col col-md-6"><label class="input">',
					 '#suffix'				=>	'</label></section></div>',
					'#title'				=>	'A(iv).Age:',
					'#type' 				=> 'textfield',
					'#attributes' 			=> 	array('placeholder' => 'Age'),
					'#default_value' 		=>	!empty($age_contractor) ? $age_contractor : '',
					'#attributes'			=>	$readonly_contarctor_age,
					'#element_validate'		=>  array('agenumaric_valiadtion') 
				  );
		}
		
		
		
			$options_country 	= array(''=>'Select','1' => 'India','2' => 'Others'); 
		$form['apply_amendment_contrcator_information']['contarctor_country'] = array(
		
					'#prefix' 			=> '<section class="col col-md-6"><label class="select">',
					'#suffix' 			=> '</label></section>',
					'#title'			=> 'Select Country',
					'#type' 			=> 'select',
					'#options' 			=> $options_country,
					'#default_value'	=> (!empty($contractor_country)) ? $contractor_country : '',
					'#required' 		=> TRUE,
					'#attributes' 		=> $readonly_contract_dist,
					'#ajax'				=> array(
													'event' 		=> 'change',
													'effect' 		=> 'fade',			
													'callback'		=> 'get_address_country',
													'progress'		=>	'',
											)
			);
		$value_country 	=  isset($form_state['values']['contarctor_country'])?$form_state['values']['contarctor_country']:$contractor_country;
		$form['apply_amendment_contrcator_information']['contarctor_address_line'] = array(
						'#prefix' 				=> '<div id="contarctor_address_replace"><section class="col col-md-6 contractor-textarea"><label class="textarea">',
						'#suffix' 				=> '</label></section></div>',
						'#title'				=>	'Address Line1 of contractor:',
						'#type' 				=> 	'textarea',
						'#rows'					=>  2,
						'#default_value' 		=>	!empty($address_line1_of_contractor) ? ($address_line1_of_contractor) : '',
						'#attributes' 			=> 	array('placeholder' => 'Address Line1 the contractor',),
						'#required' 			=> 	TRUE,
						'#attributes'			=>	$readonly_address_line1,
					);
								
		$form['apply_amendment_contrcator_information']['contarctor_state'] = array(
		
					'#prefix' 			=> '<div id="contarctor_state_replace"><section class="col col-md-6"><label class="select">',
					'#suffix' 			=> '</label></section></div>',
					'#title'			=> 'D.Select State',
					'#type' 			=> 'select',
					'#options' 			=>  get_all_state($value_country),
					'#default_value'	=> (!empty($contractor_state)) ? $contractor_state : '',
					'#required' 		=> TRUE,
					'#attributes' 		=> $readonly_contract_dist,
					
	);
		
	$value_state 	=  isset($form_state['values']['contarctor_state'])?$form_state['values']['contarctor_state']:$contractor_state;	
	$options_dist 	=  get_district_by_id($value_state);
	
	
	$form['apply_amendment_contrcator_information']['contract_dist'] = array(			
					  '#prefix'				=>	'<div id="contractor_dist_replace"><section class="col col-md-6"><label class="select">',
					  '#suffix'				=>	'</label></section></div>',
					  '#title'				=>	'Select District ',
					  '#type' 			    => 	'select',
					  '#options' 			=> $options_dist,
					  '#default_value' 		=> !empty($contractor_dist) ? $contractor_dist : '',
					  '#required' 			=> TRUE,
					  '#attributes'			=> $readonly_contract_dist,
					  '#ajax'				=> array(
															'event' 			=> 'change',
															'effect' 			=> 'fade',			
															'callback'			=> 'get_contractor_address',
															'progress'			=> '',
														  )	
					
					 
					 
					);
					
	$value_contract_dist = 	isset($form_state['values']['contract_dist']) ? $form_state['values']['contract_dist'] : $contractor_dist;
	
	$form['apply_amendment_contrcator_information']['contract_subdivision'] = array(
					  '#prefix' 			=> '<div id="contractor_subdvsn_replace"><section class="col col-md-6"><label class="select">',
					  '#suffix' 			=> '</label></section></div>',
					  '#title'				=> 'Select Subdivision',
					  '#type' 				=> 'select',
					  '#options' 			=> get_subdivisionlist_dashboard($value_contract_dist),
					  '#required' 			=> TRUE,
					  '#attributes' 		=>  $readonly_contract_dist,
					  '#default_value' 		=> !empty($contractor_subdivision) ? $contractor_subdivision : '',
					  '#ajax'				=> array(
												'event' 			=> 'change',
												'effect' 			=> 'fade',			
												'callback'			=> 'get_contractor_address',
												'progress'			=> '',
											  ),
				);
				
		$value_contract_subdivision	= isset($form_state['values']['contract_subdivision']) ? $form_state['values']['contract_subdivision'] : $contractor_subdivision;
					
		$areatypeOptions = get_areatype_dashboard($value_contract_dist, $value_contract_subdivision);	
		
		$form['apply_amendment_contrcator_information']['contract_areatype']  = array(
					  '#prefix' 			=> '<div id="contractor_areatype_replace"><section class="col col-md-6"><label class="select">',
					  '#suffix' 			=> '</label></section></div>',
					  '#title'				=> 'Select Areatype',
					  '#type' 				=> 'select',
					  '#options' 			=> array(),
					  '#attributes' 		=>  $readonly_contract_dist,
					  '#required' 			=> TRUE,
					  
					  '#ajax'				=> array(
												'event' 			=> 'change',
												'effect' 			=> 'fade',			
												'callback'			=> 'get_contractor_address',
												'progress'			=> '',
											  ),
						);
						
		$form['apply_amendment_contrcator_information']['name_contractor_areatype'] = array(
				 '#prefix' 				=> '<div id="name_contractor_areatype_replace"><section class="col col-md-6"><label class="select">',
				 '#suffix' 				=> '</label></section></div>',
				 '#title'				=> 'Select Block/Municipality/Corporation/SEZ/Notified Area',
				 '#type' 				=> 'select',
				 '#options' 			=> array(),
				 '#attributes' 			=>  $readonly_contract_dist,
				 '#required' 			=> TRUE,
				 
				
				 '#ajax'				=> array(
											'event' 			=> 'change',
											'effect' 			=> 'fade',			
											'callback'			=> 'get_contractor_address',
											'progress'			=> '',
										  )
	 );
	 $form['apply_amendment_contrcator_information']['contractor_vill_ward'] = array(
				 '#prefix' 				=> '<div id="contractor_vill_ward_replace"><section class="col col-md-6"><label class="select">',
				 '#suffix' 				=> '</label></section></div>',
				 '#title'				=> 'Select Gram Panchayat/Ward',
				 '#type' 				=> 'select',
				 '#options' 			=> array(),
				 '#attributes' 			=>  $readonly_contract_dist,
				 '#required' 			=> TRUE,
				
				 '#ajax'				=> array(
											'event' 			=> 'change',
											'effect' 			=> 'fade',			
											'callback'			=> 'get_contractor_address',
											'progress'			=> '',
										  )


			  );
			$form['apply_amendment_contrcator_information']['contractor_ps'] 						= array(
										  '#prefix' 			=> '<div id="contractor_ps_replace"><section class="col col-md-6"><label class="select">',
										  '#suffix' 			=> '</label></section></div>',
										  '#title'				=> 'Select Police Station',
										  '#type' 				=> 'select',
										  '#default_value' 		=> !empty($contractor_ps) ? $contractor_ps : '',
										  '#options' 			=> get_ps_dashboard($value_contract_dist),
										  '#required' 			=> TRUE,
										  '#attributes' 		=>  $readonly_contract_dist,
									  );
			$form['apply_amendment_contrcator_information']['contractor_pin'] = array(			
										  '#prefix' 		  => '<div id="contractor_pin_replace"><section class="col col-md-6"><label class="input">',
										  '#suffix' 		  => '</label></section></div>',
										  '#title'			  => 'PIN Code ',
										  '#size'			  => t('20'),	
										  '#type' 			  => 'textfield',
										  '#attributes' 	  => $readonly_contract_dist,												
										  '#default_value'	  => !empty($contractor_pin) ? $contractor_pin	:'',
										  '#required'		  => TRUE,
										  '#element_validate' => array('validate_integer')												 
										);	
										
			if($value_country == 1){
				 $form['apply_amendment_contrcator_information']['contarctor_address_line'] = array(
						'#prefix' 				=> '<div id="contarctor_address_replace"><section class="col col-md-6 contractor-textarea"><label class="textarea">',
						'#suffix' 				=> '</label></section></div>',
						'#title'				=>	'Address Line1 of contractor:',
						'#type' 				=> 	'textarea',
						'#rows'					=>  2,
						'#default_value' 		=>	!empty($address_line1_of_contractor) ? ($address_line1_of_contractor) : '',
						'#attributes' 			=> 	array('placeholder' => 'Address Line1 the contractor',),
						'#required' 			=> 	TRUE,
						'#attributes'			=>	$readonly_address_line1,
					);
								
		$form['apply_amendment_contrcator_information']['contarctor_state'] = array(
		
					'#prefix' 			=> '<div id="contarctor_state_replace"><section class="col col-md-6"><label class="select">',
					'#suffix' 			=> '</label></section></div>',
					'#title'			=> 'Select State',
					'#type' 			=> 'select',
					'#options' 			=>  get_all_state($value_country),
					'#default_value'	=> (!empty($contractor_state)) ? $contractor_state : '',
					'#required' 		=> TRUE,
					'#attributes' 		=> $readonly_contract_dist,
						'#ajax'				=> array(
													'event' 		=> 'change',
													'effect' 		=> 'fade',			
													'callback'		=> 'get_address_country',
													'progress'		=>	'',
											)
					
	);
		
	$value_state 		=  isset($form_state['values']['contarctor_state'])?$form_state['values']['contarctor_state']:$contractor_state;	
	$options_dist 		=  get_district_by_id($value_state);
	
	if(!empty($value_state)){ 
		$db_district_value	= db_query("SELECT count(id) as dis_id FROM {district_master} WHERE state_id = '".$value_state."'")->fetchObject()->dis_id;
	if($db_district_value>0){
		$form['apply_amendment_contrcator_information']['contract_dist'] = array(			
					  '#prefix'				=>	'<div id="contractor_dist_replace"><section class="col col-md-6"><label class="select">',
					  '#suffix'				=>	'</label></section></div>',
					  '#title'				=>	'Select District',
					  '#type' 			    => 	'select',
					  '#options' 			=> $options_dist,
					  '#default_value' 		=> !empty($contractor_dist) ? $contractor_dist : '',
					  '#required' 			=> TRUE,
					  '#attributes'			=> $readonly_contract_dist,
					  '#ajax'				=> array(
															'event' 			=> 'change',
															'effect' 			=> 'fade',			
															'callback'			=> 'get_contractor_address',
															'progress'			=> '',
														  )	
					 
										 
			);	
		$value_contract_dist = 	isset($form_state['values']['contract_dist'])?$form_state['values']['contract_dist']:$contractor_dist;	
		$form['apply_amendment_contrcator_information']['contract_subdivision'] = array(
								  '#prefix' 			=> '<div id="contractor_subdvsn_replace"><section class="col col-md-6"><label class="select">',
								  '#suffix' 			=> '</label></section></div>',
								
								  
							);	
	$form['apply_amendment_contrcator_information']['contract_areatype']  = array(
										  '#prefix' 			=> '<div id="contractor_areatype_replace"><section class="col col-md-6"><label class="select">',
										  '#suffix' 			=> '</label></section></div>',
										  
										  
							);
		$form['apply_amendment_contrcator_information']['name_contractor_areatype'] = array(
									 '#prefix' 				=> '<div id="name_contractor_areatype_replace"><section class="col col-md-6"><label class="select">',
									 '#suffix' 				=> '</label></section></div>',
									
						 );
						 
							 
		$form['apply_amendment_contrcator_information']['contractor_vill_ward'] = array(
									 '#prefix' 				=> '<div id="contractor_vill_ward_replace"><section class="col col-md-6"><label class="select">',
									 '#suffix' 				=> '</label></section></div>',
																

								  );
								  
		$form['apply_amendment_contrcator_information']['contractor_ps'] 	= array(
											  '#prefix' 			=> '<div id="contractor_ps_replace"><section class="col col-md-6"><label class="select">',
											  '#suffix' 			=> '</label></section></div>',
											  
										  );
		$form['apply_amendment_contrcator_information']['contractor_pin'] = array(			
										  '#prefix' 		  => '<div id="contractor_pin_replace"><section class="col col-md-6"><label class="input">',
										  '#suffix' 		  => '</label></section></div>',
										 											 
										);	
			
	
				
											
	}
	else{
		$value_contract_dist = "";
		$form['apply_amendment_contrcator_information']['contract_dist'] = array(			
					  '#prefix'				=>	'<div id="contractor_dist_replace"><section class="col col-md-6"><label class="select">',
					  '#suffix'				=>	'</label></section></div>',
					 
					
												 
			);	
	    
		$form['apply_amendment_contrcator_information']['contract_subdivision'] = array(
								  '#prefix' 			=> '<div id="contractor_subdvsn_replace"><section class="col col-md-6"><label class="select">',
								  '#suffix' 			=> '</label></section></div>',
								
								  
							);	
	$form['apply_amendment_contrcator_information']['contract_areatype']  = array(
										  '#prefix' 			=> '<div id="contractor_areatype_replace"><section class="col col-md-6"><label class="select">',
										  '#suffix' 			=> '</label></section></div>',
										  
										  
							);
		$form['apply_amendment_contrcator_information']['name_contractor_areatype'] = array(
									 '#prefix' 				=> '<div id="name_contractor_areatype_replace"><section class="col col-md-6"><label class="select">',
									 '#suffix' 				=> '</label></section></div>',
									
						 );
						 
							 
		$form['apply_amendment_contrcator_information']['contractor_vill_ward'] = array(
									 '#prefix' 				=> '<div id="contractor_vill_ward_replace"><section class="col col-md-6"><label class="select">',
									 '#suffix' 				=> '</label></section></div>',
																

								  );
								  
		$form['apply_amendment_contrcator_information']['contractor_ps'] 	= array(
											  '#prefix' 			=> '<div id="contractor_ps_replace"><section class="col col-md-6"><label class="select">',
											  '#suffix' 			=> '</label></section></div>',
											  
										  );
		$form['apply_amendment_contrcator_information']['contractor_pin'] = array(			
										  '#prefix' 		  => '<div id="contractor_pin_replace"><section class="col col-md-6"><label class="input">',
										  '#suffix' 		  => '</label></section></div>',
										 											 
										);	
						 
			
	}
	
	}else{
		$value_contract_dist = "";
		$form['apply_amendment_contrcator_information']['contract_dist'] = array(			
					  '#prefix'				=>	'<div id="contractor_dist_replace"><section class="col col-md-6"><label class="select">',
					  '#suffix'				=>	'</label></section></div>',
					 
					
												 
			);	
		
		$form['apply_amendment_contrcator_information']['contract_subdivision'] = array(
								  '#prefix' 			=> '<div id="contractor_subdvsn_replace"><section class="col col-md-6"><label class="select">',
								  '#suffix' 			=> '</label></section></div>',
								
								  
							);	
	$form['apply_amendment_contrcator_information']['contract_areatype']  = array(
										  '#prefix' 			=> '<div id="contractor_areatype_replace"><section class="col col-md-6"><label class="select">',
										  '#suffix' 			=> '</label></section></div>',
										  
										  
							);
		$form['apply_amendment_contrcator_information']['name_contractor_areatype'] = array(
									 '#prefix' 				=> '<div id="name_contractor_areatype_replace"><section class="col col-md-6"><label class="select">',
									 '#suffix' 				=> '</label></section></div>',
									
						 );
						 
							 
		$form['apply_amendment_contrcator_information']['contractor_vill_ward'] = array(
									 '#prefix' 				=> '<div id="contractor_vill_ward_replace"><section class="col col-md-6"><label class="select">',
									 '#suffix' 				=> '</label></section></div>',
																

								  );
								  
		$form['apply_amendment_contrcator_information']['contractor_ps'] 	= array(
											  '#prefix' 			=> '<div id="contractor_ps_replace"><section class="col col-md-6"><label class="select">',
											  '#suffix' 			=> '</label></section></div>',
											  
										  );
		$form['apply_amendment_contrcator_information']['contractor_pin'] = array(			
										  '#prefix' 		  => '<div id="contractor_pin_replace"><section class="col col-md-6"><label class="input">',
										  '#suffix' 		  => '</label></section></div>',
										 											 
										);	
			
	}
																
			
																		
		
		
		if(!empty($value_contract_dist)){
			$db_subdivision_value = db_query("SELECT count(id) as sub_id  FROM {sub_division} WHERE district_code = '".$value_contract_dist."'")->fetchObject()->sub_id;
			if($db_subdivision_value>0){
					$form['apply_amendment_contrcator_information']['contract_subdivision'] = array(
					  '#prefix' 			=> '<div id="contractor_subdvsn_replace"><section class="col col-md-6"><label class="select">',
					  '#suffix' 			=> '</label></section></div>',
					  '#title'				=> 'Select Subdivision',
					  '#type' 				=> 'select',
					  '#options' 			=> get_subdivisionlist_dashboard($value_contract_dist),
					  '#required' 			=> TRUE,
					  '#attributes' 		=>  $readonly_contract_dist,
					  '#default_value' 		=> !empty($contractor_subdivision) ? $contractor_subdivision : '',
					  '#ajax'				=> array(
												'event' 			=> 'change',
												'effect' 			=> 'fade',			
												'callback'			=> 'get_contractor_address',
												'progress'			=> '',
											  ),
				);
					$value_contract_subdivision	= isset($form_state['values']['contract_subdivision']) ? $form_state['values']['contract_subdivision'] : $contractor_subdivision;
				}else{		
			$form['apply_amendment_contrcator_information']['contract_subdivision'] = array(
										  '#prefix' 			=> '<div id="contractor_subdvsn_replace"><section class="col col-md-6"><label class="select">',
										  '#suffix' 			=> '</label></section></div>',
										
										  
									);
				$value_contract_subdivision ='';
				}
			
		}else{	
		$value_contract_subdivision ='';	
			$form['apply_amendment_contrcator_information']['contract_subdivision'] = array(
										  '#prefix' 			=> '<div id="contractor_subdvsn_replace"><section class="col col-md-6"><label class="select">',
										  '#suffix' 			=> '</label></section></div>',
										
										  
									);
		}
		
		$areatypeOptions = get_areatype_dashboard($value_contract_dist, $value_contract_subdivision);	
		if(empty($value_contract_subdivision)){
			$form['apply_amendment_contrcator_information']['contract_areatype']  = array(
										  '#prefix' 			=> '<div id="contractor_areatype_replace"><section class="col col-md-6"><label class="select">',
										  '#suffix' 			=> '</label></section></div>',
								);
		}if(!empty($value_contract_subdivision) && !empty($areatypeOptions)){
			$form['apply_amendment_contrcator_information']['contract_areatype']  = array(
					  '#prefix' 			=> '<div id="contractor_areatype_replace"><section class="col col-md-6"><label class="select">',
					  '#suffix' 			=> '</label></section></div>',
					  '#title'				=> 'Select Areatype',
					  '#type' 				=> 'select',
					  '#options' 			=> $areatypeOptions,
					  '#attributes' 		=>  $readonly_contract_dist,
					  '#required' 			=> TRUE,
					  '#default_value' 		=> !empty($contractor_areatype) ? $contractor_areatype : '',
					  '#ajax'				=> array(
												'event' 			=> 'change',
												'effect' 			=> 'fade',			
												'callback'			=> 'get_contractor_address',
												'progress'			=> '',
											  ),
						);
		}else{
			$form['contractor_contractlabour']['contract_areatype']  = array(
										  '#prefix' 			=> '<div id="contractor_areatype_replace"><section class="col col-md-6"><label class="select">',
										  '#suffix' 			=> '</label></section></div>',
										 );
		}
		$value_contract_areatype = isset($form_state['values']['contract_areatype']) ? $form_state['values']['contract_areatype'] : $contractor_areatype;
		
		if($value_contract_areatype == ""){
			$form['apply_amendment_contrcator_information']['name_contractor_areatype'] = array(
									 '#prefix' 				=> '<div id="name_contractor_areatype_replace"><section class="col col-md-6"><label class="select">',
									 '#suffix' 				=> '</label></section></div>',
									 
						 );
			
		}	
			

		if($value_contract_dist != "" && $value_contract_subdivision != ""){
			if($value_contract_areatype != ""){
				if($value_contract_areatype == 'B'){
					$form['apply_amendment_contrcator_information']['name_contractor_areatype'] = array(
							  '#prefix' 			=>'<div id="name_contractor_areatype_replace"><section class="col col-md-6"><label class="select">',
							  '#suffix' 			=> '</label></section></div>',
							  '#title'				=> 'Select Block',

							  '#type' 				=> 'select',
							  '#options' 			=> get_block_dashboard($value_contract_subdivision, $value_contract_areatype),
							  '#required' 			=> TRUE,
							  '#attributes' 		=>  $readonly_contract_dist,
							 '#default_value' 		=> !empty($contractor_name_areatype) ? $contractor_name_areatype : '',
							  '#ajax'				=> array(
														'event' 			=> 'change',
														'effect' 			=> 'fade',			
														'callback'			=> 'get_contractor_address',
														'progress'			=> '',
													  )
							);
				}if($value_contract_areatype == 'M'){
					$form['apply_amendment_contrcator_information']['name_contractor_areatype'] = array(
								  '#prefix' 			=>'<div id="name_contractor_areatype_replace"><section class="col col-md-6"><label class="select">',
								  '#suffix' 			=> '</label></section></div>',
								  '#title'				=> 'Select Municipality',
								  '#type' 				=> 'select',
								  '#options' 			=> get_block_dashboard($value_contract_subdivision, $value_contract_areatype),
								  '#required' 			=> TRUE,
								  '#attributes' 		=>  $readonly_contract_dist,
								 '#default_value' 		=> !empty($contractor_name_areatype) ? $contractor_name_areatype : '',
								  '#ajax'				=> array(
															'event' 			=> 'change',
															'effect' 			=> 'fade',			
															'callback'			=> 'get_contractor_address',
															'progress'			=> '',
														  )
								);
				}if($value_contract_areatype == 'C'){
					$form['apply_amendment_contrcator_information']['name_contractor_areatype'] = array(
								  '#prefix' 			=>'<div id="name_contractor_areatype_replace"><section class="col col-md-6"><label class="select">',
								  '#suffix' 			=> '</label></section></div>',
								  '#title'				=> 'Select Corporation',
								  '#type' 				=> 'select',
								  '#options' 			=> get_block_dashboard($value_contract_subdivision, $value_contract_areatype),
								  '#required' 			=> TRUE,
								  '#attributes' 		=>  $readonly_contract_dist,
								 '#default_value' 		=> !empty($contractor_name_areatype) ? $contractor_name_areatype : '',
								  '#ajax'				=> array(
															'event' 			=> 'change',
															'effect' 			=> 'fade',			
															'callback'			=> 'get_contractor_address',
															'progress'			=> '',
														  )
								);
				}if($value_contract_areatype == 'S'){
						$form['apply_amendment_contrcator_information']['name_contractor_areatype'] = array(
								  '#prefix' 			=>'<div id="name_contractor_areatype_replace"><section class="col col-md-6"><label class="select">',
								  '#suffix' 			=> '</label></section></div>',
								  '#title'				=> 'Select SEZ',
								  '#type' 				=> 'select',
								  '#options' 			=> get_block_dashboard($value_contract_subdivision, $value_contract_areatype),
								  '#required' 			=> TRUE,
								  '#attributes' 		=>  $readonly_contract_dist,
								  '#default_value' 		=> !empty($contractor_name_areatype) ? $contractor_name_areatype : '',
								  '#ajax'				=> array(
															'event' 			=> 'change',
															'effect' 			=> 'fade',			
															'callback'			=> 'get_contractor_address',
															'progress'			=> '',
														  )
								);
					}if($value_contract_areatype == 'N'){
							$form['apply_amendment_contrcator_information']['name_contractor_areatype'] = array(
									  '#prefix' 			=>'<div id="name_contractor_areatype_replace"><section class="col col-md-6"><label class="select">',
									  '#suffix' 			=> '</label></section></div>',
									  '#title'				=> 'Select Notified Area',
									  '#type' 				=> 'select',
									  '#options' 			=> get_block_dashboard($value_contract_subdivision, $value_contract_areatype),
									  '#required' 			=> TRUE,
									  '#attributes' 		=>  $readonly_contract_dist,
									 '#default_value' 		=> !empty($contractor_name_areatype) ? $contractor_name_areatype : '',
									  '#ajax'				=> array(
																'event' 			=> 'change',
																'effect' 			=> 'fade',			
																'callback'			=> 'get_contractor_address',
																'progress'			=> '',
															  )
									);
					}
			}
		}

											
		$value_name_contractor_areatype = isset($form_state['values']['name_contractor_areatype']) ? $form_state['values']['name_contractor_areatype'] : $contractor_name_areatype;
	
			if($value_contract_dist != "" && $value_contract_subdivision != ""){
				if($value_contract_areatype != ""){
					if($value_name_contractor_areatype != ""){
						if($value_contract_areatype == 'B'){
							$form['apply_amendment_contrcator_information']['contractor_vill_ward'] 						= array(
										 '#prefix' 				=> '<div id="contractor_vill_ward_replace"><section class="col col-md-6"><label class="select">',
										 '#suffix' 				=> '</label></section></div>',
										 '#title'				=> 'Select Gram Panchayat',
										 '#type' 				=> 'select',
										 '#options'				=> get_villward_dashboard($value_name_contractor_areatype,'V'),
										 '#attributes' 			=>  $readonly_contract_dist,
										 '#default_value' 		=> !empty($contractor_vill_ward) ? $contractor_vill_ward : '',
										 '#required' 			=> TRUE,
										
										 '#ajax'				=> array(
																	'event' 			=> 'change',
																	'effect' 			=> 'fade',			
																	'callback'			=> 'get_contractor_address',
																	'progress'			=> '',
																  )


									  );
						}if($value_contract_areatype == 'M'){
								
								$form['apply_amendment_contrcator_information']['contractor_vill_ward'] 						= array(
										  '#prefix' 			=> '<div id="contractor_vill_ward_replace"><section class="col col-md-6"><label class="select">',
										  '#suffix' 			=> '</label></section></div>',
										  '#title'				=> 'Select Ward',
										  '#type' 				=> 'select',
										  '#options' 			=> get_villward_dashboard($value_name_contractor_areatype,'W'),
										  '#required' 			=> TRUE,
										  '#attributes' 		=>  $readonly_contract_dist,
										  '#default_value' 		=> !empty($contractor_vill_ward) ? $contractor_vill_ward : '',
										  '#ajax'				=> array(
																		'event' 			=> 'change',
																		'effect' 			=> 'fade',			
																		'callback'			=> 'get_contractor_address',
																		'progress'			=> '',
																	  )
										);
																								
						}if($value_contract_areatype == 'C'){
								
								$form['apply_amendment_contrcator_information']['contractor_vill_ward'] 						= array(
										  '#prefix' 			=> '<div id="contractor_vill_ward_replace"><section class="col col-md-6"><label class="select">',
										  '#suffix' 			=> '</label></section></div>',
										  '#title'				=> 'Select Ward',
										  '#type' 				=> 'select',
										  '#options' 			=> get_corporation_dasboard($value_name_contractor_areatype),
										  '#required' 			=> TRUE,
										  '#attributes' 		=>  $readonly_contract_dist,
										  '#default_value' 		=> !empty($contractor_vill_ward) ? $contractor_vill_ward : '',
										  '#ajax'				=> array(
																		'event' 			=> 'change',
																		'effect' 			=> 'fade',			
																		'callback'			=> 'get_contractor_address',
																		'progress'			=> '',
																	  )
										);
																								
						}
						
						if($value_contract_areatype == 'N'){
								
								$form['apply_amendment_contrcator_information']['contractor_vill_ward'] 						= array(
											  '#prefix' 			=> '<div id="contractor_vill_ward_replace"><section class="col col-md-6"><label class="select">',
											  '#suffix' 			=> '</label></section></div>',
											  '#title'				=> 'Select Notified Area',
											  '#type' 				=> 'select',
											  '#options' 			=> get_villward_dashboard($value_name_contractor_areatype,'N'),
											  '#required' 			=> TRUE,
											  '#attributes' 		=>  $readonly_contract_dist,
											  '#default_value' 		=> !empty($contractor_vill_ward) ? $contractor_vill_ward : '',
											  '#ajax'				=> array(
																			'event' 			=> 'change',
																			'effect' 			=> 'fade',			
																			'callback'			=> 'get_contractor_address',
																			'progress'			=> '',
																		  )
											);
																								
						}if($value_contract_areatype == 'S'){
								
								$form['apply_amendment_contrcator_information']['contractor_vill_ward'] 	= array(
										  '#prefix' 			=> '<div id="contractor_vill_ward_replace"><section class="col col-md-6"><label class="select">',
										  '#suffix' 			=> '</label></section></div>',
										  '#title'				=> 'Select Sector',
										  '#type' 				=> 'select',
										  '#options' 			=> get_villward_dashboard($value_name_contractor_areatype,'S'),
										  '#required' 			=> TRUE,
										  '#attributes' 		=>  $readonly_contract_dist,
										  '#default_value' 		=> !empty($contractor_vill_ward) ? $contractor_vill_ward : '',
										  '#ajax'				=> array(
																			'event' 			=> 'change',
																			'effect' 			=> 'fade',			
																			'callback'			=> 'get_contractor_address',
																			'progress'			=> '',
																	  )
									);
					}
					}
					
				}
			}
			$value_contractor_villward 	= isset($form_state['values']['contractor_vill_ward']) ? $form_state['values']['contractor_vill_ward'] : $contractor_vill_ward;
																				  
																								  
																								  
			if($value_contract_dist != "" && $value_contract_subdivision != ""){
				if($value_contract_areatype != ""){
					if($value_name_contractor_areatype != ""){
						if($value_contractor_villward != ""){
							$form['apply_amendment_contrcator_information']['contractor_ps'] 						= array(
										  '#prefix' 			=> '<div id="contractor_ps_replace"><section class="col col-md-6"><label class="select">',
										  '#suffix' 			=> '</label></section></div>',
										  '#title'				=> 'Select Police Station',
										  '#type' 				=> 'select',
										  '#default_value' 		=> !empty($contractor_ps) ? $contractor_ps : '',
										  '#options' 			=> get_ps_dashboard($value_contract_dist),
										  '#required' 			=> TRUE,
										  '#attributes' 		=>  $readonly_contract_dist,
										  
									  );
									  
									  
						$form['apply_amendment_contrcator_information']['contractor_pin'] = array(			
										  '#prefix' 		  => '<div id="contractor_pin_replace"><section class="col col-md-6"><label class="input">',
										  '#suffix' 		  => '</label></section></div>',
										  '#title'			  => 'PIN Code ',
										  '#size'			  => t('20'),	
										  '#type' 			  => 'textfield',
										  '#attributes' 	  => $readonly_contract_dist,												
										  '#default_value'	  => !empty($contractor_pin) ? $contractor_pin	:'',
										  '#required'		  => TRUE,
										  '#element_validate' => array('validate_integer')												 
										);	
						}
					}
				}
			}
			
			}
			if($value_country == 2){
			 $form['apply_amendment_contrcator_information']['contarctor_address_line'] = array(
						'#prefix' 				=> '<div id="contarctor_address_replace"><section class="col col-md-6 contractor-textarea"><label class="textarea">',
						'#suffix' 				=> '</label></section></div>',
						'#title'				=>	'Address Line1 of contractor:',
						'#type' 				=> 	'textarea',
						'#rows'					=>  2,
						'#default_value' 		=>	!empty($address_line1_of_contractor) ? ($address_line1_of_contractor) : '',
						'#attributes' 			=> 	array('placeholder' => 'Address Line1 the contractor',),
						'#required' 			=> 	TRUE,
						'#attributes'			=>	$readonly_address_line1,
					);	
		$form['apply_amendment_contrcator_information']['contarctor_state'] = array(
		
					'#prefix' 			=> '<div id="contarctor_state_replace"><section class="col col-md-6">',
					'#suffix' 			=> '</section></div>',
					
			);
					
		$form['apply_amendment_contrcator_information']['contract_dist'] = array(			
					  '#prefix'				=>	'<div id="contractor_dist_replace"><section class="col col-md-6"><label class="select">',
					  '#suffix'				=>	'</label></section></div>',

					 
					
												 
			);	
	
		$form['apply_amendment_contrcator_information']['contract_subdivision'] = array(
								  '#prefix' 			=> '<div id="contractor_subdvsn_replace"><section class="col col-md-6"><label class="select">',
								  '#suffix' 			=> '</label></section></div>',
								
								  
							);	
	$form['apply_amendment_contrcator_information']['contract_areatype']  = array(
										  '#prefix' 			=> '<div id="contractor_areatype_replace"><section class="col col-md-6"><label class="select">',
										  '#suffix' 			=> '</label></section></div>',
										  
										  
							);
		$form['apply_amendment_contrcator_information']['name_contractor_areatype'] = array(
									 '#prefix' 				=> '<div id="name_contractor_areatype_replace"><section class="col col-md-6"><label class="select">',
									 '#suffix' 				=> '</label></section></div>',
									
						 );
						 
							 
		$form['apply_amendment_contrcator_information']['contractor_vill_ward'] = array(
									 '#prefix' 				=> '<div id="contractor_vill_ward_replace"><section class="col col-md-6"><label class="select">',
									 '#suffix' 				=> '</label></section></div>',
																

								  );
								  
		$form['apply_amendment_contrcator_information']['contractor_ps'] 	= array(
											  '#prefix' 			=> '<div id="contractor_ps_replace"><section class="col col-md-6"><label class="select">',
											  '#suffix' 			=> '</label></section></div>',
											  
										  );
		$form['apply_amendment_contrcator_information']['contractor_pin'] = array(			
										  '#prefix' 		  => '<div id="contractor_pin_replace"><section class="col col-md-6"><label class="input">',
										  '#suffix' 		  => '</label></section></div>',
										 											 
										);		 
						 
			
	
			}
			
		  $form['apply_amendment_contrcator_information']['submit'] = array (
					
					  '#prefix' 		=> '<section class="col col-md-12">',
					  '#type' 			=> 'submit',
					  '#attributes' 	=> array('class' => array('button left-btn')),
					  '#value' 			=> 'Save'										  
				   );
		$form['apply_amendment_contrcator_information']['back_link']	 = array(
			 '#suffix' 		=> '</section></div></div></div></div></div>', 
			'#markup' 			=> l(t('Back'),'amendment_license_renewal/apply_amendment/'. $serial_no.'/'.$renewallicenseammendment_id.'/'. $applicationflag, array('html' => TRUE,'attributes'=> array('class'=>array('button')))),
																		
		);						 

	  
								  
	return $form;
	
	
}



function ammendment_contractor_information_submit($form, &$form_state){
	
	global $base_root, $base_path, $user;
		
	$contractor_id = 	$user->uid;	
	$val =	$form_state['values'];
	
	$id											= 	$val['id'];
	$status_from 								= 	$val['status_from'];
	$renewallicenseammendment_id				=	encryption_decryption_fun('decrypt', $val['renewallicenseammendment_id']);
	$applicationflag							=	encryption_decryption_fun('decrypt', $val['applicationflag']);
	$serialno_1									=	$val['serialno'];
	$renewallicenseammendment_id_1				=	$val['renewallicenseammendment_id'];
	$applicationflag_1							=	$val['applicationflag'];
	$serialno									=	encryption_decryption_fun('decrypt', $val['serialno']);
	$application_id								=	encryption_decryption_fun('decrypt', $val['application_id']);
	$pe_userid									=	encryption_decryption_fun('decrypt', $val['pe_userid']);
	
	$contarctor_address_line					=	$val['contarctor_address_line'];
	
	$category 									= 	$val['category'];
	
	$contarctor_name							=	$val['contarctor_name'];
	$contarctor_fathername 						= 	$val['contarctor_fathername'];
	$contarctor_dob								=	!empty($val['contarctor_dob'])? date('Y-m-d',strtotime($val['contarctor_dob'])) : NULL;
	$contarctor_age 							= 	$val['contarctor_age'];
	$applicant_id 								= 	$val['applicant_id'];
	$serial										=	$val['serial'];
	
	$contractor_dist 							= 	$val['contract_dist'];
	$contractor_subdivision 					= 	$val['contract_subdivision'];
	$contractor_areatype 						= 	$val['contract_areatype'];
	$name_contractor_areatype					=	$val['name_contractor_areatype'];
	$contractor_vill_ward						=	$val['contractor_vill_ward'];
	$contractor_ps								= 	$val['contractor_ps'];
	$contractor_pin								=	$val['contractor_pin'];
	$contractor_state							=	$val['contarctor_state'];
	$contractor_country							=	$val['contarctor_country'];
	$ammendment_status							=	$val['ammendment_status'];
	//$serialno 									= encryption_decryption_fun('decrypt', $serialno_1);
	
	
	// if($applicationflag == 'L'){
	// 	$license_id								=	$renewallicenseammendment_id;//---kaj baki
	// }if($applicationflag == 'A'){
	// 	$license_id								=	return_ammendement_license_id($renewallicenseammendment_id);//---kaj baki
	// }if($applicationflag == 'R'){
	// 	$license_id								=	return_renewal_license($renewallicenseammendment_id);//---kaj baki
	// }
   //echo encryption_decryption_fun('decrypt', $serialno_1).'--'.$contractor_id;die;

	$getLicenseId = db_query("SELECT id FROM l_contractor_license_application  where serial_no_from_v ='".$serialno."' and contractor_user_id ='".$contractor_id."' and status='I' ")->fetchAssoc();	
	$license_id    = $getLicenseId['id'];

	
	$details_array								= 	array(
			'category_of_contractor'			=>	$category,
			'license_renewal_ammendment_id'		=>  $renewallicenseammendment_id,
			'application_id'					=>	$application_id,
			'user_id'							=>  $pe_userid,
			'father_contarctor_name'			=>	$contarctor_fathername,
			'dob_contractor'					=>	$contarctor_dob, 
			'age_contractor'					=>	$contarctor_age	,
			'created_by'						=>	$contractor_id,
			'act_id'							=> '1',
			'contractor_dist'					=> $contractor_dist,
			'contractor_subdivision'			=> $contractor_subdivision,
			'contractor_areatype'				=> $contractor_areatype,
			'contractor_name_areatype'			=> $name_contractor_areatype,
			'contractor_vill_ward'				=> $contractor_vill_ward,
			'contractor_ps'                     => $contractor_ps,
			'contractor_state'					=> $contractor_state,
			'contractor_country'				=> $contractor_country,
			'address_of_contractor'				=> $contarctor_address_line,
			'contractor_pin'					=> $contractor_pin,
			'is_flag'                           => $applicationflag,
			'name_of_contractor'				=> $contarctor_name,
		    'license_id'						=> $license_id,
			'personal_info_flag'				=> 1,
			
			
		);
		
	
		
		if(trim($ammendment_status)=='I'){	
			$amendment_rows_check = db_select('l_amendment_of_contract_labour_license', 'amd')->fields('amd',array('id'))->condition('license_id',$license_id)->execute();
			if($amendment_rows_check->rowCount() > 1){
				$applicant_mailto  = 'wblc.nic@gmail.com';
				$subject = 'Amendment Issue';
				$msg = 'Multiple row for amendment for License Id -'.$license_id.' Form-V-No -'.$serialno.'. Place- labour worksite info';
				if($applicant_mailto){
					send_mail_for_user_alert($applicant_mailto, $subject, $msg);
				}
				form_set_error('reference_no' ,t('We apologize. There is a problem. Please try again later.'));
			}else{
				$ammendment_id = return_ammendement_id($license_id);

				$get_data = db_select('l_amendment_of_contract_labour_license', 'amd')->fields('amd',array('payable_amendment_fees','payable_amendment_security_fees','reference_no','deposited_max_security_fees','previous_highest_security'))->condition('id',$ammendment_id)->condition('license_id',$license_id)->execute()->fetchAssoc();
			}

			//$ammendment_id = return_ammendement_id($license_id);
			if(!empty($ammendment_id) || $ammendment_id!=''){
				$query = db_query("SELECT reference_no FROM l_amendment_of_contract_labour_license_log where previous_amendment_id ='".$ammendment_id."' ");	
				$reference_no = $query->fetchField();
				// if(empty($reference_no) || $reference_no == ''){
				// 	amemd_log($ammendment_id);		
				// }
				amemd_log($ammendment_id);

				$get_payment_data_query = db_select('l_principle_epayments_data', 'pay')->fields('pay',array('total_amount','identification_no','payment_status'))->condition('application_id',$get_data['reference_no'])->condition('act_id',14)->condition('payment_status','Success')->execute();
				if($get_payment_data_query->rowCount() > 0){
					$get_payment_data = $get_payment_data_query->fetchAssoc();

					$total_amount = $get_data['payable_amendment_fees']+$get_data['payable_amendment_security_fees'];
					if($get_payment_data['total_amount'] >= $total_amount){
						$payable_amendment_security_fees = 0;
						$payable_amendment_fees = 0;
					}else{
						
						if(($get_payment_data['total_amount'] - $get_data['payable_amendment_security_fees']) == $get_data['payable_amendment_fees']){
							$payable_amendment_fees = $get_data['payable_amendment_fees'];
							$payable_amendment_security_fees = 0;
						}else{
							$payable_amendment_fees = 0;
							$payable_amendment_security_fees = $total_amount-$get_payment_data['total_amount'];
						}
						
					}
					$deposited_max_security_fees = $get_data['deposited_max_security_fees']+$get_payment_data['total_amount'];
				}else{
					$get_payment_data_query = db_query("SELECT id,challanamount from l_principle_epayments_receive_data where transaction_id IN (
						select identification_no from l_principle_epayments_data where application_id =".$get_data['reference_no']." and act_id = 14)");

					if($get_payment_data_query->rowCount() > 0){
						$get_payment_data = $get_payment_data_query->fetchAssoc();
						
						$total_amount = $get_data['payable_amendment_fees']+$get_data['payable_amendment_security_fees'];

						if($get_payment_data['challanamount'] >= $total_amount){
							$payable_amendment_security_fees = 0;
							$payable_amendment_fees = 0;
						}else{
							
							if(($get_payment_data['challanamount'] - $get_data['payable_amendment_security_fees']) == $get_data['payable_amendment_fees']){
								$payable_amendment_fees = $get_data['payable_amendment_fees'];
								$payable_amendment_security_fees = 0;
							}else{
								$payable_amendment_fees = 0;
								$payable_amendment_security_fees = $total_amount-$get_payment_data['challanamount'];
							}
							
						}
						$deposited_max_security_fees = $get_data['deposited_max_security_fees']+$get_payment_data['challanamount'];

					}else{
						$payable_amendment_security_fees = $get_data['payable_amendment_security_fees'];
						$payable_amendment_fees = $get_data['payable_amendment_fees'];
						$deposited_max_security_fees = $get_data['deposited_max_security_fees'];
					}

					


				}

				$upadte_array	=	 array(
						
					'ammendment_status'			=>	'',
					'ammendment_final_status'	=>	'',
					'created_date'				=>	NULL,
					'alc_inspector_action_date'	=>	NULL,
					'remark_by_userid'			=>	NULL,
					'remark_by_userid'			=>	NULL,
					'ammendment_certificate_id'	=> NULL,
					'payable_amendment_security_fees' => $payable_amendment_security_fees,
					'payable_amendment_fees'	=> $payable_amendment_fees,
					'deposited_max_security_fees'	=> $deposited_max_security_fees,
					'previous_highest_security'	=> $deposited_max_security_fees,
				);
			
				$update_ammendment_table 	= 	db_update('l_amendment_of_contract_labour_license');
				$update_ammendment_table	->	fields($upadte_array);
				$update_ammendment_table	->	condition('id',$ammendment_id);
				$update_ammendment_table	->	execute();
			}	
			
		}
	
	// $check_ammendment_table        				 =  check_ammendment_table($renewallicenseammendment_id);
	// $details_ammendment_table 	= check_ammendment_table_license_renewal_ammendment_id($renewal_license_ammendment_id); // 24-02-2022 MOUMITA 

	//$details_ammendment_table 	= check_ammendment_table_license_renewal_ammendment_id($renewallicenseammendment_id);
	$details_ammendment_table 	= check_ammendment_table_new($renewallicenseammendment_id,$serialno);
	//if($check_ammendment_table->rowCount()==0){
		
	if(empty($details_ammendment_table['id'])|| $details_ammendment_table['id'] ==NULL || $details_ammendment_table['id'] == ''){
		db_insert('l_amendment_of_contract_labour_license')->fields($details_array)->execute();
		$ammendment_id = return_ammendement_id($license_id);
		contractor_activity_tag_insert($serialno,'A',$ammendment_id,'');
	}else{ 
		
	    //print_r($details_array);die();
		$update_ammendment_table = 	db_update('l_amendment_of_contract_labour_license');
 		$update_ammendment_table	->	fields($details_array);
 		$update_ammendment_table	->	condition('license_id',$license_id);
		$update_ammendment_table	->	execute();
		$ammendment_id = return_ammendement_id($license_id);
		if($ammendment_status == 'B')
			contractor_activity_tag_insert($serialno,'A',$ammendment_id,'B');
		else
		  contractor_activity_tag_insert($serialno,'A',$ammendment_id,'');
		
	}
	

	$message = 1;
	   drupal_goto('amendment_license_renewal/apply_amendment/'.$serialno_1.'/'.$renewallicenseammendment_id_1.'/'.$applicationflag_1.'#'.$message, array('fragment' => 'Personal Information')) ;												
	

		
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

function get_category($form, $form_state){
	$commands 	= array();
	
	$commands[] = ajax_command_replace('#add_fathername', drupal_render($form['apply_amendment_contrcator_information']['contarctor_fathername']));
	$commands[] = ajax_command_replace('#add_dob', drupal_render($form['apply_amendment_contrcator_information']['contarctor_dob']));
	$commands[] = ajax_command_replace('#add_age', drupal_render($form['apply_amendment_contrcator_information']['contarctor_age']));
	
	
  	return array('#type' => 'ajax', '#commands' => $commands);
}

function get_contractor_address($form, $form_state){
	$commands 	= array();
	
	$commands[] = ajax_command_replace('#contractor_subdvsn_replace', drupal_render($form['apply_amendment_contrcator_information']['contract_subdivision']));
	$commands[] = ajax_command_replace('#contractor_areatype_replace', drupal_render($form['apply_amendment_contrcator_information']['contract_areatype']));
	$commands[] = ajax_command_replace('#name_contractor_areatype_replace', drupal_render($form['apply_amendment_contrcator_information']['name_contractor_areatype']));
	$commands[] = ajax_command_replace('#contractor_vill_ward_replace', drupal_render($form['apply_amendment_contrcator_information']['contractor_vill_ward']));
	
	$commands[] = ajax_command_replace('#contractor_ps_replace', drupal_render($form['apply_amendment_contrcator_information']['contractor_ps']));
	
  	return array('#type' => 'ajax', '#commands' => $commands);
}

function get_address_country($form, $form_state){
	$commands 	= array();
	$commands[] = ajax_command_replace('#contarctor_state_replace', drupal_render($form['apply_amendment_contrcator_information']['contarctor_state']));
	$commands[] = ajax_command_replace('#contarctor_address_replace', drupal_render($form['apply_amendment_contrcator_information']['contarctor_address_line']));
	$commands[] = ajax_command_replace('#contractor_dist_replace', drupal_render($form['apply_amendment_contrcator_information']['contract_dist']));
	$commands[] = ajax_command_replace('#contractor_subdvsn_replace', drupal_render($form['apply_amendment_contrcator_information']['contract_subdivision']));
	$commands[] = ajax_command_replace('#contractor_areatype_replace', drupal_render($form['apply_amendment_contrcator_information']['contract_areatype']));
	$commands[] = ajax_command_replace('#name_contractor_areatype_replace', drupal_render($form['apply_amendment_contrcator_information']['name_contractor_areatype']));
	$commands[] = ajax_command_replace('#contractor_vill_ward_replace', drupal_render($form['apply_amendment_contrcator_information']['contractor_vill_ward']));
	
	$commands[] = ajax_command_replace('#contractor_ps_replace', drupal_render($form['apply_amendment_contrcator_information']['contractor_ps']));
	$commands[] = ajax_command_replace('#contractor_pin_replace', drupal_render($form['apply_amendment_contrcator_information']['contractor_pin']));
	
  	return array('#type' => 'ajax', '#commands' => $commands);
}
