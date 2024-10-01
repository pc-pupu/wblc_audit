<?php

drupal_add_css(drupal_get_path('module', 'cmscontent') . '/css/cms.css');
drupal_add_js(drupal_get_path('module', 'cmscontent') . '/js/custom.js');


function get_content_details($form, $form_state, $param1 = '', $param2 = '',  $param3 = ''){
	global $base_url;
	$cms_content_query = db_select('l_cms_content', 't');
    $cms_content_query->fields('t', array('content_id','content_title','content_desc','content_typ', 'fid', 'achieve_objectives', 'business_friendly'));
	$cms_content_query->fields('fm', array('filename'));
	$cms_content_query->condition('t.content_typ',$param1,'=');
	$cms_content_query->condition('t.content_id',encryption_decryption_fun('decrypt', $param2),'=');
	$cms_content_query->leftJoin('file_managed', 'fm', 'fm.fid = t.fid');
	$cms_content_query_result = $cms_content_query->execute();
	$record = $cms_content_query_result->fetchObject();
	
	// print_r($record); echo $record->content_title; die;
	
	$output = '';
	
	//$output .= '<h1 id="page-title" class="title">'.$record->content_title.'</h1>';
	$output .= '<div class="row cms-feedback">';
	
	$output .= '<div class="col-md-12 legal-section">';
	$output .= '<h4>Legal Basis - Does it have a basis in law/act/policy</h4><p>'.$record->content_desc.'</p>';
	$output .= '<h4>Its necessity - Does the license help government achieve its objectives</h4><p>'.$record->achieve_objectives.'</p>';
	$output .= '<h4>Its business-friendly - Does it impose minimum burden on businesses to achieve the government’s objectives</h4><p>'.$record->business_friendly.'</p>';
	$output .= '</div>';
	if(!empty($record->filename)){
		$output .= '<div class="col-md-12"><h5><i class="fa fa-hand-o-right" aria-hidden="true"></i>&ensp;'.l('Click here to download the pdf version of the notification.', $base_url.'/sites/default/files/contentpdf/'.$record->filename, array('attributes' => array('target' => '_blank','class'=>'click-pdf'))).'</h5></div>';
	}
	$cms_feedback = db_select('l_cms_feedback','l');
	$cms_feedback->fields('l', array());
	$cms_feedback->condition('l.content_id',encryption_decryption_fun('decrypt', $param2),'=');
	$cms_feedback->condition('l.is_publish',1);
	$cms_feedback->orderBy('l.id', 'DESC');
	$cms_feedback_result = $cms_feedback->execute()->fetchAll();//print_r($cms_feedback_result); exit;
	
	$output .= '<div class="col-md-12 publish-section">
						<h4>Published Comments :</h4>
						<ul>';
			$x = 0;	
			foreach($cms_feedback_result as $data){ 
				$output .= '<li><span class="user">'.$data->fullname.'</span>&emsp;<span class="comment">'.$data->comment.'&emsp;<span class="date">'.date('d/m/Y  h:i A',strtotime($data->submitted_date)).'</span></span>
				</li>';
			}
		$output .= '</ul>
				</div>';
		$output = html_entity_decode($output);
		
		$form['circular']['content'] = array(
					'#type' => 'markup',
					'#markup' => $output        
					);
		$form['circular']['mrk1'] = array(
					'#type' => 'markup',
					'#markup' => '<div class="col-md-12 comment-section"><div class="col-md-12"><h4>Give your feedback / suggestion :</h4></div>',
					);					
		$form ['circular']['id']=array(
							'#type' => 'hidden',
							'#value' => $param2,
							);		
		$form ['circular']['comment']=array(
							'#prefix' => '<div class="col-md-12">',
							'#suffix' => '</div>',
							'#type' => 'textarea',
							'#title' => t('Comment'),
							'#required' => TRUE,
							'#element_validate' => array('MISC_SPECIAL_CHAR_EDIT_TEXT_VALIDATE'),
							'#attributes' => array('class'=>array('form-control'),'placeholder'=>'Write Comment here.'),
							);
		
		$form['circular']['fullname'] = array(        					
							'#prefix' => '<div class="col-md-4">',
							'#suffix' => '</div>',
							'#title' => 'Full Name',
							'#type' => 'textfield',
							'#required' => TRUE,
							'#element_validate' => array('MISC_TEXTBOX_TEXT_VALIDATE'),
							'#attributes' => array('class' => array('form-control'),'placeholder'=>'Enter email address')
							);	
		$form['circular']['mobile'] = array(        					
							'#prefix' => '<div class="col-md-4">',
							'#suffix' => '</div>',
							'#title' => 'Mobile Number',
							'#type' => 'textfield',
							'#required' => TRUE,
							'#element_validate' => array('MISC_PHONE_VALIDATE'),
							'#attributes' => array('class' => array('form-control'),'placeholder'=>'Enter 10 digits mobile number', 'maxlength' => 10,)
							);
		$form ['circular']['email']=array(
							'#prefix' => '<div class="col-md-4">',
							'#suffix' => '</div>',
							'#type' => 'textfield',
							'#title' => t('E-mail'),
							'#required' => TRUE,
							'#element_validate' => array('MISC_EMAIL_VALIDATE'),
							'#attributes' => array('class'=>array('form-control'), 'placeholder'=>'Enter email address'),
							);
		$options_country = array(''=>'-Select-', '1' => 'India', '2' => 'Others'); 
		
		$form['circular']['country']= array(
							'#prefix' => '<div class="col-md-4">',
							'#suffix' => '</div>',
							'#title' => 'Country',
							'#type' => 'select',
							'#default_value' => array( '' => '--Select--' ),
							'#options' => $options_country,
							'#required' => TRUE,
							'#attributes' => array('class'=>array('form-control')),
							'#ajax' => array(
										'event' => 'change',
										'effect' => 'fade',	
										'method' => 'replace',
										'callback' => 'get_info_form_ajax_callback',
										'progress' => '',
										)
				);	
				
		$value_country = isset($form_state['values']['country']) ? $form_state['values']['country'] : '';
		$form['circular']['state'] = array(
				'#prefix' => '<div id="state_div">',
				'#suffix' => '</div>'
				);
		$form['circular']['location'] = array(
				'#prefix' => '<div id="loc_div" class="col-md-8">',
				'#suffix' => '</div>',
				'#title' => t('Address Line (if other Country/State please provide detail address)'),
				'#type' => 'textarea',
				'#rows' => '1',
				'#required' => TRUE,
				'#element_validate' => array('MISC_TEXTBOX_LOCATION_VALIDATE'),
				'#attributes' => array('class'=>array('form-control'))
				);
		$form['circular']['dist'] = array(
				'#prefix' => '<div id="dist_div">',
				'#suffix' => '</div>'
				);	
		$form['circular']['subdivision'] = array(
				'#prefix' => '<div id="subdivision_div">',
				'#suffix' => '</div>'
				);
		$form['circular']['areatype'] = array(
				'#prefix' => '<div id="areatype_div">',
				'#suffix' => '</div>'
				);	
		$form['circular']['name_areatype'] = array(
				'#prefix' => '<div id="name_areatype_div">',
				'#suffix' => '</div>'
				);
		$form['circular']['panchyat_ward_sector'] = array(
				'#prefix' => '<div id="panchyat_ward_sector_div">',
				'#suffix' => '</div>'
				);		
		$form['circular']['police_station'] = array(
				'#prefix' => '<div id="police_station_div">',
				'#suffix' => '</div>'
				);
		$form['circular']['pincode'] = array(
				'#prefix' => '<div id="pincode_div">',
				'#suffix' => '</div>',
				);													
		if(!empty($value_country) && $value_country !='' && $value_country == '1'){
			
			$options_state = get_all_state($value_country);
			$form['circular']['state'] = array(
					'#prefix' => '<div id="state_div" class="col-md-4">',
					'#suffix' => '</div>',
					'#title' => t('Select State'),
					'#type' => 'select',
					'#options' => $options_state,
					'#required' => TRUE,
					'#attributes' => array('class'=>array('form-control')),
					'#ajax' => array(
										'event' => 'change',
										'effect' => 'fade',	
										'method' => 'replace',
										'callback' => 'get_info_form_ajax_callback',
										'progress' => '',
										)
					);
			
			$value_state = isset($form_state['values']['state']) ? $form_state['values']['state'] : '';

			$form['circular']['location'] = array(
					'#prefix' => '<div id="loc_div" class="col-md-4">',
					'#suffix' => '</div>',
					'#title' => t('Address'),
					'#type' => 'textarea',
					'#rows' => '1',
					'#required' => TRUE,
					'#element_validate' => array('MISC_TEXTBOX_LOCATION_VALIDATE'),
					'#attributes' => array('class'=>array('form-control'))
					);
				if(!empty($value_country) && !empty($value_state) && $value_state == '1'){
					$form['circular']['dist'] = array(
						'#prefix' => '<div id="dist_div" class="col-md-4">',
						'#suffix' => '</div>',
						'#title' => t('Select District'),
						'#type' => 'select',
						'#default_value' => array( '' => '--Select--' ),
						'#options' => common_custom_user_district_list_new(),
						'#attributes' => array('class'=>array('form-control')),
						'#required' => TRUE,
						'#ajax' => array(
									'event' => 'change',
									'effect' => 'fade',
									'callback' => 'get_info_form_ajax_callback',
									'progress' => '',
									)
						);
						$value_dist = isset($form_state['values']['dist']) ? $form_state['values']['dist'] : '';
						if(!empty($value_country) && !empty($value_state) && !empty($value_dist)){
							$form['circular']['subdivision'] = array(
								'#prefix' => '<div id="subdivision_div" class="col-md-4">',
								'#suffix' => '</div>',
								'#title' => 'Select Subdivision',
								'#type' => 'select',
								'#default_value' => array( '' => '--Select--' ),
								'#attributes' => array('class'=>array('form-control')),
								'#options' => common_custom_user_subdivisionlist_list_new($value_dist),
								'#required' => TRUE,
								'#ajax' => array(
												'event' => 'change',
												'effect' => 'fade',			
												'callback' => 'get_info_form_ajax_callback',
												'progress' => '',
												)
								);
						}
						$value_subdivision = isset($form_state['values']['subdivision']) ? $form_state['values']['subdivision'] : '';
						if(!empty($value_country) && !empty($value_state) && !empty($value_dist) && !empty($value_subdivision)){
							$areatypeOptions = common_custom_user_areatype_list_new($value_dist, $value_subdivision); 
		
							$form['circular']['areatype'] = array(
								'#prefix' => '<div id="areatype_div" class="col-md-4">',
								'#suffix' => '</div>',
								'#title' => 'Select Areatype',
								'#type' => 'select',
								'#default_value' => array( '' => '--Select--' ),
								'#attributes' => array('class'=>array('form-control')),
								'#options' => $areatypeOptions,
								'#required' => TRUE,
								'#ajax' => array(
												'event' => 'change',
												'effect' => 'fade',			
												'callback' => 'get_info_form_ajax_callback',
												'progress' => '',
												)
								);
						}
						$value_areatype = isset($form_state['values']['areatype']) ? $form_state['values']['areatype'] : '';
						if(!empty($value_country) && !empty($value_state) && !empty($value_dist) && !empty($value_subdivision) && !empty($value_areatype)){
							if( $value_areatype == 'B' ){
								$areatype_name = 'Block';
							}elseif( $value_areatype == 'M'){
								$areatype_name = 'Municipality';
							}elseif( $value_areatype == 'C'){
								$areatype_name = 'Corporation';
							}elseif( $value_areatype == 'S'){
								$areatype_name = 'SEZ';
							}elseif($value_areatype == 'N'){
								$areatype_name = 'Notified Area';
							}else{
								$areatype_name = 'Select Block/Municipality/Corporation/SEZ/Notified Area';
							}
			
							$form['circular']['name_areatype'] = array(
								'#prefix' => '<div id="name_areatype_div" class="col-md-4">',
								'#suffix' => '</div>',
								'#title' => 'Name of '.$areatype_name,
								'#type' => 'select',
								'#default_value' => array( '' => '--Select--' ),
								'#attributes' => array('class'=>array('form-control')),
								'#options' => common_custom_user_block_list_new($value_subdivision,$value_areatype),
								'#required' => TRUE,
								'#ajax' => array(
												'event' => 'change',
												'effect' => 'fade',			
												'callback' => 'get_info_form_ajax_callback',
												'progress' => '',
												)	
								);
						}
						$value_name_areatype = isset($form_state['values']['name_areatype']) ? $form_state['values']['name_areatype'] : '';
						if(!empty($value_country) && !empty($value_state) && !empty($value_dist) && !empty($value_subdivision) && !empty($value_areatype) && !empty($value_name_areatype)){
							if($value_areatype == 'B'){
								$titleLable = 'Gram Panchayat Name';
								$funName	= common_custom_user_villward_list_new($value_name_areatype,'V');
							}else if($value_areatype == 'M'){
								$titleLable = 'Ward Number';
								$funName	= common_custom_user_villward_list_new($value_name_areatype,'W');
							}else if($value_areatype == 'C'){
								$titleLable = 'Ward Number';
								$funName	= common_custom_user_corporation_list_new($value_name_areatype);
							}else if($value_areatype == 'S'){
								$titleLable = 'Sector';
								$funName	= common_custom_user_villward_list_new($value_name_areatype,'S');
							}else if($value_areatype == 'N'){
								$titleLable = 'Notified Area';
								$funName	= common_custom_user_villward_list_new($value_name_areatype,'N');
							}else{
								$titleLable = 'Select Gram Panchayat / Ward / Sector';
								$funName	= array( '' => '--Select--' );
							}
									
							$form['circular']['panchyat_ward_sector'] = array(
								 '#prefix' => '<div id="panchyat_ward_sector_div" class="col-md-4">',
								 '#suffix' => '</div>',
								 '#title' => $titleLable,
								 '#type' => 'select',
								 '#default_value' => array( '' => '--Select--' ),
								 '#attributes' => array('class'=>array('form-control')),
								 '#options' => $funName,
								 '#required' => TRUE,
								 '#ajax' => array(
												'event' => 'change',
												'effect' => 'fade',			
												'callback' => 'get_info_form_ajax_callback',
												'progress' => '',
												)
								  );
						
						}
						$value_panchyat_ward_sector = isset($form_state['values']['panchyat_ward_sector']) ? $form_state['values']['panchyat_ward_sector'] : '';					
						if(!empty($value_country) && !empty($value_state) && !empty($value_dist) && !empty($value_subdivision) && !empty($value_areatype) && !empty($value_name_areatype) && !empty($value_panchyat_ward_sector)){
							$form['circular']['police_station'] = array(
								 '#prefix' => '<div id="police_station_div" class="col-md-4">',
								 '#suffix' => '</div>',
								 '#title' => 'Select Police Station',
								 '#type' => 'select',
								 '#default_value' => array( '' => '--Select--' ),
								 '#attributes' => array('class'=>array('form-control')),
								 '#options' => common_custom_user_ps_list_new($value_dist),
								 '#required' => TRUE,
								 '#ajax' => array(
												'event' => 'change',
												'effect' => 'fade',			
												'callback' => 'get_info_form_ajax_callback',
												'progress' => '',
												)
								  );
						}
						$value_police_station = isset($form_state['values']['police_station']) ? $form_state['values']['police_station'] : '';					
							if(!empty($value_country) && !empty($value_state) && !empty($value_dist) && !empty($value_subdivision) && !empty($value_areatype) && !empty($value_name_areatype) && !empty($value_panchyat_ward_sector) && !empty($value_police_station)){
								$form['circular']['pincode'] = array(
									 '#prefix' => '<div id="pincode_div" class="col-md-4">',
									 '#suffix' => '</div>',
									 '#title' => 'PIN Code',
									 '#type' => 'textfield',
									 '#attributes' => array('class'=>array('form-control')),
									 '#placeholder' => 'Enter your Area Pin Code',
									 '#maxlength' => 6,
									 '#required' => TRUE,
									 '#element_validate' => array('MISC_PIN_NUMBER')
									 );
							}

				}
		}
		if($value_country == '2'){
			$form['circular']['location'] = array(
					'#prefix' => '<div id="loc_div" class="col-md-8">',
					'#suffix' => '</div>',
					'#title' => t('Address (if other Country/State please provide detail address)'),
					'#type' => 'textarea',
					'#rows' => '1',
					'#required' => TRUE,
					'#element_validate' => array('MISC_TEXTBOX_LOCATION_VALIDATE'),
					'#attributes' => array('class'=>array('form-control')),
					);
		}
		
		$form['circular']['save'] = array(
					'#prefix' => '<div class="col-md-12">',
					'#suffix' => '</div>',
					'#type' => 'submit',
					'#value' => 'Submit Feedback',
					'#attributes' => array('class' => array('btn btn-primary')),
					);
					
		$form['circular']['mrk1_end'] = array(
					'#type' => 'markup',
					'#markup' => '</div>',
					);
								
		$form['circular']['mrk2_end'] = array(
					'#type' => 'markup',
					'#markup' => '</div>',
					);			
		return $form;
}
function get_content_details_submit($form, $form_state, $param1 = '', $param2 = '',  $param3 = ''){
	$val = $form_state['values'];
	$content_id = encryption_decryption_fun('decrypt', $val['id']); 
	$fields = array(
					'content_id'=>$content_id,
					'comment'=>strip_tags($val['comment']),
					'fullname'=>strip_tags($val['fullname']),
					'mobile'=>$val['mobile'],
					'email'=>$val['email'],
					'country'=>$val['country'],
					'state'=>$val['state'],
					'location'=>$val['location'],
					'dist'=>$val['dist'],
					'subdivision'=>$val['subdivision'],
					'areatype'=>$val['areatype'],
					'name_areatype'=>$val['name_areatype'],
					'panchyat_ward_sector'=>$val['panchyat_ward_sector'],
					'police_station'=>$val['police_station'],
					'pincode'=>$val['pincode'],
					);
	$query = db_insert('l_cms_feedback')->fields($fields)->execute();
	if($query){
			drupal_set_message('Data successfully saved.');
	}else{
			drupal_set_message('Some Error Occured.', 'error');
	}				
	
}
function e_service_list(){	
	global $base_url;
	drupal_add_js(drupal_get_path('module', 'cmscontent') . '/js/popup_code.js'); 
	
	$content = '';
	
	$content .='<div class="row">
				<div class="col-lg-3 col-md-6 col-sm-6">
				<div class="eservice-box">
				<div class="eservice-box-logo"><img src="sites/all/themes/lcTheme/images/logo-regs.png" alt="Registration" title="Registration"></div>
				<h1>Registration<br>of Principal Employer</h1>
				<p>under The Contract Labour (Regulation & Abolition) Act 1970.</p>
				<div class="eservice-box-btm">
				<!--<span data-toggle="modal" data-target="#howtoclra"><a href="javascript:void(0)" data-val="clra" class="how-to-apply">Guideline</a></span>-->
				<span data-toggle="modal" data-target="#applicant_login"><a href="javascript:void(0)" data-val="clra" class="apply_link apply">Apply</a></span></div>
				</div></div>
				
				<div class="col-lg-3 col-md-6 col-sm-6">
				<div class="eservice-box">
					<div class="eservice-box-logo"><img src="sites/all/themes/lcTheme/images/logo-regs.png" alt="Amendment of Registration Certificate" title="Amendment of Registration Certificate"></div>				
						<h1>Amendment<br> of Registration Certificate</h1><p>for Principal Employers under The Contract Labour (Regulation & Abolition) Act 1970.</p>
						<div class="eservice-box-btm">
							<span data-toggle="modal" data-target="#user_guide_amndclra"><a href="javascript:void(0)" data-val="clra" class="how-to-apply">Guideline</a></span>
							<a id="dir_id" href="clra" class="apply_link apply">Apply</a>
						</div>
					</div>
				</div>
				
				<div class="col-lg-3 col-md-6 col-sm-6">
				<div class="eservice-box">
				<div class="eservice-box-logo"><img src="sites/all/themes/lcTheme/images/logo-licence-contractor.png" alt="Licensing for Contractors" title="Licensing for Contractors"></div>
				<h1>Licensing<br> for Contractors</h1>
				<p>under The Contract Labour (Regulation & Abolition) Act 1970.</p>
					<div class="eservice-box-btm">
						<!--<span data-toggle="modal" data-target="#user_guide_licenseclra"><a href="javascript:void(0)" data-val="clra" class="how-to-apply">Guideline</a></span>-->
						<span data-toggle="modal" data-target="#applicant_login"><a href="javascript:void(0)" data-val="clra" class="apply_link apply">Apply</a></span>
						</div>
				</div></div>
				
				<div class="col-lg-3 col-md-6 col-sm-6">
				<div class="eservice-box">
				<div class="eservice-box-logo"><img src="sites/all/themes/lcTheme/images/logo-licence-contractor.png" alt="Renewal of License for Contractors" title="Renewal of License for Contractors"></div>
				<h1>Renewal<br> of License for Contractors</h1>
				<p>under The Contract Labour (Regulation & Abolition) Act 1970.</p>
				<div class="eservice-box-btm">
					<span data-toggle="modal" data-target="#howtorenewalclra"><a href="javascript:void(0)" data-val="clra" class="how-to-apply#"><!--How to apply?--></a></span>
					<span data-toggle="modal" data-target="#applicant_loginss"><a href="javascript:void(0)" data-val="clra" class="apply_link apply">Apply</a></span></div>
					<!--<a id="dir_id" href="clra" class="apply_link apply">Apply</a>-->
				</div></div>
				
				<div class="col-lg-3 col-md-6 col-sm-6">
				<div class="eservice-box">
				<div class="eservice-box-logo"><img src="sites/all/themes/lcTheme/images/logo-licence-contractor.png" alt="Amendment of License / Renewal for Contractors" title="Amendment of License / Renewal for Contractors"></div>
				<h1>Amendment of <br>License / Renewal for Contractors</h1>
				<p>under The Contract Labour (Regulation & Abolition) Act 1970.</p>
				<div class="eservice-box-btm">
				<span data-toggle="modal" data-target="#howtoamlicclra"><a href="javascript:void(0)" data-val="clra" class="how-to-apply#"><!--How to apply?--></a></span>
				
				<a id="dir_id" href="clra" class="apply_link apply">Apply</a></span></div>
				</div></div>
				
				
				
				<div class="col-lg-3 col-md-6 col-sm-6">
				<div class="eservice-box">
				<div class="eservice-box-logo"><img src="sites/all/themes/lcTheme/images/logo-regs.png" alt="Registration of Principal Employers" title="Registration of Principal Employers"></div>
				<img src="sites/all/themes/lcTheme/images/new-animation.gif" alt="New" title="New" class="new-animation">
				<h1>Registration of <br>Principal Employers</h1>
				<p>under The Inter State Migrant Workmen (Regulation of Employment and Conditions of Service) Act, 1979</p>
				<div class="eservice-box-btm"><span data-toggle="modal" data-target="#howtoismw"><a href="javascript:void(0)" data-val="ismw" class="how-to-apply#"><!--How to apply?--></a></span><span data-toggle="modal" data-target="#applicant_login"><a href="javascript:void(0)" data-val="ismw" class="apply_link apply">Apply</a></span></div>
				</div></div>

				<div class="col-lg-3 col-md-6 col-sm-6">
				<div class="eservice-box">
				<div class="eservice-box-logo"><img src="sites/all/themes/lcTheme/images/logo-regs.png" alt="Licence of Principal Employers" title="Licence of Principal Employers"></div>
				
				<h1>Licence of <br>Contractors</h1>
				<p>under The Inter State Migrant Workmen (Regulation of Employment and Conditions of Service) Act, 1979</p>
					<div class="eservice-box-btm">
						<span data-toggle="modal" data-target="#howtoismw"><a href="javascript:void(0)" data-val="ismw" class="how-to-apply#"><!--How to apply?--></a></span>
						<!--<span data-toggle="modal" data-target="#applicant_login"><a href="javascript:void(0)" data-val="ismw" class="apply_link apply">Apply</a></span>-->
						<a id="dir_id" href="ismw" class="apply_link apply">Apply</a>
					</div>
				</div></div>
				
				
				<div class="col-lg-3 col-md-6 col-sm-6">
				<div class="eservice-box">
				<div class="eservice-box-logo"><img src="sites/all/themes/lcTheme/images/logo-building.png" alt="Registration of Establishments" title="Registration of Establishments"></div>
				<h1>Registration<br> of Establishments</h1>
				<p>under Building and Other Construction Workers (Regulation of Employment and Conditions of Service) Act, 1996.</p>
				<div class="eservice-box-btm">
						<span data-toggle="modal" data-target="#howtobocwa"><a href="javascript:void(0)" data-val="clra" class="how-to-apply#"><!--How to apply?--></a></span>
						<span data-toggle="modal" data-target="#applicant_login"><a href="javascript:void(0)" data-val="bocwa" class="apply_link apply">Apply</a></span>
						</div>
				</div></div>
				
				<div class="col-lg-3 col-md-6 col-sm-6">
				<div class="eservice-box">
				<div class="eservice-box-logo"><img src="sites/all/themes/lcTheme/images/logo-mtw.png" alt="Registration of Motor Transport Workers" title="Registration of Motor Transport Undertaking"></div>
				<h1>Registration of<br> Motor Transport undertaking</h1>
				<p>under Motor Transport Workers Act, 1961</p>
				
				<div class="eservice-box-btm">
					<span data-toggle="modal" data-target="#howtomtw"><a href="javascript:void(0)" data-val="mtw" class="how-to-apply">User Guide</a></span>
					<a href="https://wblc.gov.in/mtw" class="apply_link apply">Apply</a></span>
				</div>
				</div></div>
				
				<div class="col-lg-3 col-md-6 col-sm-6">
				<div class="eservice-box">
				<div class="eservice-box-logo"><img src="sites/all/themes/lcTheme/images/logo-mtw.png" alt="Renewal of Registration Certificate" title="Renewal of Registration Certificate"></div>
				<h1>Renewal of<br> Registration Certificate</h1>
				<p>for Motor Transport Undertaking under Motor Transport Workers Act, 1961</p>
				<div class="eservice-box-btm">
						<span data-toggle="modal" data-target="#howtomtwren"><a href="javascript:void(0)" data-val="clra" class="how-to-apply">User Guide</a></span>
						<!--<span data-toggle="modal" data-target="#applicant_login"><a href="javascript:void(0)" data-val="mtw" class="apply_link apply">Apply</a></span>-->
						<a href="https://wblc.gov.in/mtw" class="apply_link apply">Apply</a></span>
				</div>
				</div>
				</div>
				
				
				<div class="col-lg-3 col-md-6 col-sm-6">
				<div class="eservice-box">
				<div class="eservice-box-logo"><img src="sites/all/themes/lcTheme/images/logo-labour-law.png" alt="Registration" title="Registration"></div>
				<h1>Submission of returns</h1>
				<p>under Various Labour Laws</p>
				<div class="eservice-box-btm">
					<span data-toggle="modal" data-target="#howtoreturns"><a href="javascript:void(0)" class="how-to-apply#"><!--How to apply?--></a></span>
										
					<a href="https://wblc.gov.in/user/login" data-val="applicant-login" class="apply_link apply">Apply</a></span>
					</div>
				</div></div>
				
				<div class="col-lg-3 col-md-6 col-sm-6">
				<div class="eservice-box">
				<div class="eservice-box-logo"><img src="sites/all/themes/lcTheme/images/logo-contract-labour.png" alt="Registration" title="Registration"></div>
				<h1>View and download<br> inspection note</h1>
				<p>under Various Labour Laws</p>
				<div class="eservice-box-btm">
					<span data-toggle="modal" data-target="#howtoinsdwld"><a href="javascript:void(0)" data-val="clra" class="how-to-apply#"><!--How to apply?--></a></span>
					<a href="https://wblc.gov.in/view-inspection-report" data-val="view-inspection-report" class="apply_link apply">Apply</a>
					</div>
				</div></div>
				
				<div class="col-lg-3 col-md-6 col-sm-6">
				<div class="eservice-box">
				<div class="eservice-box-logo"><img src="sites/all/themes/lcTheme/images/logo-trade-union.png" alt="Annual Return" title="Annual Return"></div>
				<img src="sites/all/themes/lcTheme/images/new-animation.gif" alt="New" title="New" class="new-animation">
				<h1>Annual Return of<br>Trade Union / Federation</h1>
				<p>under Trade Union Act, 1926</p>
				<div class="eservice-box-btm">
					<span data-toggle="modal" data-target="#user_guide_tu_return"><a href="javascript:void(0)" data-val="clra" class="how-to-apply">User Guide</a></span>
					<a href="trade-union-registration" data-val="trade-union-registration" class="apply_link apply">Apply</a></div>
				</div>
				</div>
				
				<div class="col-lg-3 col-md-6 col-sm-6">
				<div class="eservice-box">
				<div class="eservice-box-logo"><img src="sites/all/themes/lcTheme/images/logo-principal-emp.png" alt="Registration" title="Registration"></div>
				<h1>Registration, <br> Renewal, Amendment etc.</h1>
				<p>under West Bengal Shop and Establishment Act, 1963</p>
				<div class="eservice-box-btm"><span data-toggle="modal" data-target="#abcd"><a href="javascript:void(0)" data-val="clra" class="how-to-apply#"><!--How to apply?--></a></span><a href="http://wbshopsonline.gov.in" class="apply_link apply" target="_blank">Apply</a></div>
				</div>
				</div>

				<div class="col-lg-3 col-md-6 col-sm-6">
				<div class="eservice-box">
				<div class="eservice-box-logo"><img src="sites/all/themes/lcTheme/images/logo-regs.png" alt="FAWLOI Declaration" title="FAWLOI Declaration"></div>
				<img src="sites/all/themes/lcTheme/images/coming-soon.png" alt="Coming Soon" title="Coming Soon" class="coming-soon-animation">
				<h1>FAWLOI Declaration</h1>
				<p>Financial Assistance to the Workers of Locked Out Industries</p>
				<div class="eservice-box-btm"><span data-toggle="modal" data-target="#user_guide_fawloi"><a href="javascript:void(0)" data-val="clra" class="how-to-apply">User Guide</a></span>
				<a href="fawloi-service" data-val="fawloi-service" class="apply_link apply">Apply</a></div>
				</div>
				</div>

				<!--<div class="col-lg-3 col-md-6 col-sm-6">
				<div class="eservice-box">
				<div class="eservice-box-logo"><img src="sites/all/themes/lcTheme/images/logo-building.png" alt="bocwa cess" title="bocwa cess"></div>
				<img src="sites/all/themes/lcTheme/images/coming-soon.png" alt="Coming Soon" title="Coming Soon" class="coming-soon-animation">
				<h1>Payment of Cess</h1>
				<p>Under The Building and Other Construction Workers&prime; Welfare Cess Act, 1996</p>
				<div class="eservice-box-btm"><span data-toggle="modal" data-target="#howtobocwacess"><a href="javascript:void(0)" data-val="clra" class="how-to-apply">How to apply?</a></span><span data-toggle="modal" data-target="#applicant_login"><a href="https://wblc.gov.in/bocwacess" data-val="bocwacess" class="apply_link apply">Apply</a></span></div>
				</div>
				</div>-->
				
				
				<div class="col-lg-3 col-md-6 col-sm-6">
				<div class="eservice-box">
				<div class="eservice-box-logo"><img src="sites/all/themes/lcTheme/images/icon-tea-garden.png" alt="tgs entry" title="bocwa cess"></div>
				<img src="sites/all/themes/lcTheme/images/new-animation.gif" alt="Coming Soon" title="Coming Soon" class="coming-soon-animation">
				<h1>Survey of Tea Gardens</h1>
				<p>Conducted by Regional Labour Offices under jurisdiction of Joint Labour Commissioner, North Bengal Zone</p>
				<div class="eservice-box-btm"><span><a href="https://wblc.gov.in/user/login" data-val="bocwacess" class="how-to-apply" style="float:right;">Data Entry</a></span></div>
				</div>
				</div>
				
				';
				 /* Payal 30-12-2022 To be updated below in applicant login Payal -----*/
			$content .= '<div id="applicant_login" class="modal fade applicant_login" role="dialog">
					<div class="modal-dialog">
						<div class="modal-content cus-modal-content">
						    <div class="modal-header modal-border-top">
							    <button type="button" class="close" data-dismiss="modal">&times;</button>
							    <h4 class="modal-title">Applicant Login</h4>
						    </div>
					  		<div class="modal-body modal-body-white">
								<p class="text-primary"><strong>You will be redirected to Shilpa Sathi Portal (https://silpasathi.wb.gov.in) to avail this service. Click on <strong>SINGLE WINDOW</strong> button to proceed.</strong></p>
								<div class="row">
									<div class="col-sm-12"><a href="https://silpasathi.wb.gov.in" href=""><img src="sites/all/themes/lcTheme/images/wblc-eodb-modal-bg-big.jpg" alt="" title=""></a></div>
									<div class="col-md-12" style="margin-top:15px;"><center><a href="https://silpasathi.wb.gov.in" href=""><img src="sites/all/themes/lcTheme/images/modal-btn-swindow.png" alt="" title=""></a></center></div>
								</div>
							</div>
						</div>					
					</div>
				</div>';
/*	$content .= '<div id="applicant_loginss" class="modal fade applicant_login" role="dialog">
					<div class="modal-dialog">
						<div class="modal-content cus-modal-content">
						    <div class="modal-header modal-border-top">
							    <button type="button" class="close" data-dismiss="modal">&times;</button>
							    <h4 class="modal-title">Applicant Login</h4>
						    </div>
					  		<div class="modal-body modal-body-white">
							<div class="row">
							<!--<div class="col-sm-12"><a href="https://silpasathi.wb.gov.in" href=""><img src="sites/all/themes/lcTheme/images/wblc-lc-modal-bg-big.jpg" alt="" title=""></a></div>
							<div class="col-md-12" style="margin-top:15px;"><center><a id="dir_id" href="clra" class="apply_link apply"><img src="sites/all/themes/lcTheme/images/modal-btn-lc.png" alt="" title=""></a></center></div>-->
							<div class="col-sm-6"><div class="modal-lc-bg">&nbsp;</div><br><a id="dir_id" href=""><img src="sites/all/themes/lcTheme/images/modal-btn-lc.png" alt="" title=""></a></div>
							<div class="col-sm-6 left-border"><div class="modal-eodb-login-bg">&nbsp;</div><br><a href="https://silpasathi.wb.gov.in" href=""><img src="sites/all/themes/lcTheme/images/modal-btn-swindow.png" alt="" title=""></a></div>	
							</div>
							</div>
						</div>					
					</div>
				</div>';*/	
	$content .= '<div id="applicant_loginss" class="modal fade applicant_login" role="dialog">
					<div class="modal-dialog">
						<div class="modal-content cus-modal-content">
						    <div class="modal-header modal-border-top">
							    <button type="button" class="close" data-dismiss="modal">&times;</button>
							    <h4 class="modal-title">Applicant Login</h4>
						    </div>
					  		<div class="modal-body modal-body-white">
							<p class="text-primary"><strong>Please select any one of the options -</strong></p>
							<div class="row">
							<div class="col-sm-6"><div class="modal-lc-bg">&nbsp;</div><br><a id="dir_id" href=""><img src="sites/all/themes/lcTheme/images/modal-btn-lc.png" alt="" title=""></a></div>
							<div class="col-sm-6 left-border"><div class="modal-eodb-login-bg">&nbsp;</div><br><a href="https://silpasathi.wb.gov.in" href=""><img src="sites/all/themes/lcTheme/images/modal-btn-swindow.png" alt="" title=""></a></div>
							</div>
							</div>
						</div>					
					</div>
				</div>';
				
	$content .= '<div id="howtoclra" class="modal fade applicant_login" role="dialog">
					<div class="modal-dialog">
						<div class="modal-content">
					  		<div class="modal-header">
								<button type="button" class="close" data-dismiss="modal">&times;</button>
								<h4 class="modal-title">User Guide</h4>
					  		</div>
					  		<div class="modal-body">
						 		<div class="tabbable">
									<ul class="nav nav-tabs">
									<li class="active"><a href="#tab1" data-toggle="tab">How to apply?</a></li>
									<li><a href="#tab2" data-toggle="tab">Checklist</a></li>
									<li><a href="#tab3" data-toggle="tab">Timeline</a></li>
									<li style="float:right;"><a href="'. $base_url.'/sites/default/files/contentpdf/user-guide-principal-employer-registration-contract-labour.pdf" target="_blank"><i class="fa fa-print"></i></a></li>
									</ul>
									<div class="tab-content">
										<div class="tab-pane active" id="tab1">
									<div class="sop_cntainer_box">
							 		<div class="sop_content">
										<p><strong>Responsibility: </strong>Principal Employer.</p>
										<div class="step_ord">Step-1</div>
										<div class="res_activity">If the you are an already registered user, he/she will log in using the credentials. For a new user he/she fills up a Common Application Form(CAF) in the Portal of wblc.gov.in, which includes mainly applicant’s Personal Information including PAN/Voters ID/Aadhar, any other unique number tagged with that establishment, Contact Information, Establishment Information, Number of workers employed, Login Information etc. with Preferred User name & Password and create a User Credential.</div>
									</div>
									<div class="sop_content">
										<div class="step_ord">Step-2</div>
										<div class="res_activity">Logs into the system with the valid credentials and captcha which lands up in the Dashboard.</div>
									</div>
									<div class="sop_content">
										<div class="step_ord">Step-3</div>
										<div class="res_activity"><strong>If Offine/Manual certificate available</strong> select ALREADY REGISTERED option from menu and apply for online application of Principal Employer under the Contract Labour (Regulation and Abolition) Act , 1970 by filling up necessary information in the Form and uploads self-certified copies of required documents.<br>
											
										<strong>NEW REGISTRATION:</strong> Select NEW REGISTRATION from menu and applies for online Registration of Principal Employer under the Contract Labour (Regulation and Abolition) Act , 1970 by filling up necessary information in the Form and uploads self-certified copies of required documents.<br>
										<strong>Documents Involved: </strong>I.Valid Trade License, II.Articles of Association and Memorandum of Association/Partnership Deed, III.Factory License if any, IV.Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc., V.Any other document in support of correctness of the particulars mentioned in the application if required</div>
									</div>
									<div class="sop_content">
										<div class="step_ord">Step-4</div>
										<div class="res_activity">Add valid contractor information into the system.</div>
									</div>
									<div class="sop_content">
										<div class="step_ord">Step-5</div>
										<div class="res_activity">Views the Filled in Information in the Application Preview Section before Final Submission or in case of correction rolls back to earlier sections and makes corrections and submit the application. The application is then reverted back to concerned officials according to jurisdiction  for verification.</div>
									</div>	
									<div class="sop_content">
										<div class="step_ord">Step-6</div>
										<div class="res_activity">Logs into the system with the credentials and views the status of application marked for correction if sent back by the Inspector, makes the corrections and resubmits the application.
										</div>
									</div>
									<div class="sop_content">
										<div class="step_ord">Step-7</div>
										<div class="res_activity">Views application status in the Dashboard marked as PAYMENT (PAY NOW) by ALC and makes payment Online through GRIPS. <br>After successfull payment of fees statutory FORM-I is automatically generated. Applicant can digitally/in-hand signs the FORM-I and upload it. After FORM-I is successfully uploaded, an acknowledgement is generated. <br>In case it is returned back by the ALC he views on the Dashboard the status of application marked for correction and makes the necessary corrections and resubmits the application.<br/>
											<strong>Documents Involved: </strong>FORM-I
										</div>
									</div>
									<div class="sop_content">
										<div class="step_ord">Step-8</div>
										<div class="res_activity">If the application is verified registration certificate is shown in the dashboard and FORM-V is generated. You can digitally/in-hand sign the FORM-V and send via e-mail or through SMS. The Serial number in FORM-V is needed to contractors for applying of License.
										</div>
									</div>
									<div class="sop_content">
										<div class="step_ord">Note:</div>
										<div class="res_activity">Track Status of Application through SMS Alerts generated through the System starting from Creating Login Credentials, Submission of Application, Correction Required, Payment of Fees , Status of Approval or Rejection , Status on Issue of Registration Certificate can be shown from the dashboard.
										</div>
									</div>											
								</div>
							</div>					
							<div class="tab-pane" id="tab2">
								<div class="sop_cntainer_box">
									<div class="sop_content">
										<div class="step_ord">Scanned copies of the following documents whichever is applicable :</div>
										<ul>
									</div>	
										<li>Trade License</li>
										<li>Article of Association and Memorandum of Association / Partnership Deed</li>
										<li>Factory License if any</li>
										<li>Previous Registration Certificate (If Already registered user for Registration)</li>
										<li>Any other document in support of correctness of the particulars mentioned in the application if required</li>
										<li>Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc</li>				
										<li>Form I</li>		
										</ul>
								</div>
							</div>	
							<div class="tab-pane" id="tab3">
								<div class="sop_cntainer_box">
									<div class="#sop_content">
										<table border="0" cellpadding="0" cellspacing="" class="view-act-rules-table">
											<tr>
												<th width="5%">Sl.No.</th>
												<th width="60%">Designated officer</th>
												<th>Stipulated time limit</th>
											</tr>
											<tr>
												<td>1</td>
												<td>Assistant Labour Commissioner</td>
												<td>30 days</td>
											</tr>
											<tr>
												<td>2</td>
												<td>Deputy Labour Commissioner</td>
												<td>45 days</td>
											</tr>
											<tr>
												<td>3</td>
												<td>Labour Commissioner</td>
												<td>60 days</td>
											</tr>
										</table>
										<p style="float:right;"><i class="fa fa-info-circle"></i> <a href="'.$base_url.'/sites/default/files/important_notification/RIGHT_TO_PUBLIC_SERVICES_NOTIFICATION.pdf" target="_blank">More Info.</a></p>															
									</div>	
								</div>
							</div>	
						</div>
					</div>	
				</div>				
			</div>
		</div>
	</div>';
	
	$content .= '<div id="user_guide_tu_return" class="modal fade applicant_login" role="dialog">
					<div class="modal-dialog">
						<div class="modal-content">
					  		<div class="modal-header">
								<button type="button" class="close" data-dismiss="modal">&times;</button>
								<h4 class="modal-title">User Guide</h4>
					  		</div>
					  		<div class="modal-body">
								<div class="tabbable">
									<ul class="nav nav-tabs">
										<li class="active"><a href="#tu_return_tab1" data-toggle="tab">How to apply?</a></li>
										<li><a href="#tu_return_tab2" data-toggle="tab">Checklist</a></li>
										<li><a href="#tu_return_tab3" data-toggle="tab">Timeline</a></li>
										<li style="float:right;"><a href="'.$base_url.'/sites/default/files/contentpdf/user-guide-trade-union-annual-return.pdf" target="_blank"><i class="fa fa-print"></i></a></li>
									</ul>
									<div class="tab-content">
										<div class="tab-pane active" id="tu_return_tab1">
											<div class="sop_cntainer_box">
											<p><strong>Responsibility: </strong>Trade Union Applicant</p>							
												<div class="sop_content">
													<div class="step_ord">Step-1:</div>
													<div class="res_activity"><strong>Activity:</strong>The applicant shall open ‘www.wblc.gov.in’ and will select e-services and Annual Return of Trade Union /  Federation thereafter. A guide line will appear, agreeing which he/she will land up in the log in page. If the applicant is an already registered user, he/she will log in using the credentials. For a new user he/she fills up a Registration  Form in the Portal of wblc.gov.in, which includes information of Trade Union / Federation, with Preferred User name & Password and creates a User Credential.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-2:</div>
													<div class="res_activity"><strong>Activity:</strong>Logs into the System with the Valid Credentials and Captcha.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-3:</div>
													<div class="res_activity"><strong>Activity:</strong>Lands up into the Dashboard.
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">Step-4:</div>
													<div class="res_activity"><strong>Activity:</strong>Select ‘Annual Return for Trade Union’ option from the left menu of Dashboard.
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">Step-5:</div>
													<div class="res_activity"><strong>Activity:</strong>Put the Registration number of Trade Union / Federation and check availability for return submission. User can submit Annual Return for a single Trade Union or Federation annually.
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">Step-6:</div>
													<div class="res_activity"><strong>Activity:</strong>Fill all the relevant & mandatory fields of each section of the Application like Application Details, Asset & Liability Statement, List of Securities, General Fund Account, etc and <strong>Upload the Scanned Signature **</strong>.
													</div>
												</div>	
												
												<div class="sop_content">
													<div class="step_ord">Step-7:</div>
													<div class="res_activity"><strong>Activity:</strong>Views the Filled in Information before Final Submit in the Preview page. In case of corrections, rolls back to specific sections through corresponding <strong> Rectify </strong> link, makes corrections, and Saves the application.
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">Step-8:</div>
													<div class="res_activity"><strong>Activity:</strong>Final Submission of the application. No changes can be made after Final Submission, If you have submitted your return under any affiliated body then it will be submitted after approved by your affiliated body.
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">Step-9:</div>
													<div class="res_activity"><strong>Activity:</strong>User can only view the Application after Final Submission.
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">** Procedure for Uploading the Signature:</div>
													<div class="res_activity">
														<ul style="list-style:disc; margin-left:20px;">
															<li>	While filling in the Online Application Form the applicant will be provided with a link to upload his signature.</li>
															<li>	Click on the link “Upload Signature” option.</li>
															<li>	By clicking "choose file", Browse & Select the location where the Scanned Signature file has been saved. Select the file by clicking on it.</li>
															<li>	Click the ‘Upload file’ button for uploading signature & view on server.</li>
															<li>	After verification of signature, click "Save signature image file" to complete the upload process & click "Next" to proceed further stages.</li>
														</ul>
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">**	Guidelines for Scanning the Signature:</div>
												    <div class="res_activity"><strong>Before applying online, applicant will be required to have a scanned (digital) image of his/her signature as per the specifications given bellow:</strong><br>
														<ul style="list-style:disc; margin-left:20px;">
															<li>	The applicant has to sign on white paper.</li>
															<li>	The signature must be signed only by the applicant, not by any other person.</li>
															<li>	Dimensions - Width 256 pixels’ x Height 64 pixels i.e. Width 1.4" /35cm X Height 0.44"/1.06cm (preferred).</li>
															<li>	Size of file should be between 10kb – 20kb.</li>
															<li>	Ensure that the size of the scanned image is not more than 20KB.</li>
														</ul><br>
													</div>
												</div>
																															
											</div>
										</div>
										
										<div class="tab-pane" id="tu_return_tab2">
											<div class="sop_cntainer_box">
												<div class="sop_content">
													<div class="step_ord">Scanned copies of the following documents whichever is applicable :</div>
														<ul style="list-style:disc; margin-left:20px;">
															<li>Signature of Consent Officer</li>
															<li>Signature of Secretary</li>
															<li>Signature of Treasurer</li>  	
														</ul>
												</div>	
											</div>
										</div>	
										<div class="tab-pane" id="tu_return_tab3">
											<div class="sop_cntainer_box">
												<div class="sop_content">
													<table border="0" cellpadding="0" cellspacing="" class="view-act-rules-table">
														<tr>
															<th width="5%">Sl.No.</th>
															<th width="50%">Designated officer</th>
															<th>Stipulated time limit</th>
														</tr>
														<tr>
															<td colspan="3">Not Available</td>
														</tr>														
													</table>													
												</div>	
											</div>
										</div>	
									</div>
								</div>	
							</div>				
						</div>
					</div>
				</div>';
				$content .= '<div id="user_guide_fawloi" class="modal fade applicant_login" role="dialog">
				<div class="modal-dialog">
					<div class="modal-content">
						  <div class="modal-header">
							<button type="button" class="close" data-dismiss="modal">&times;</button>
							<h4 class="modal-title">User Guide</h4>
						  </div>
						  <div class="modal-body">
							<div class="tabbable">
								<ul class="nav nav-tabs">
									<li class="active"><a href="#fawloi_tab1" data-toggle="tab">How to apply?</a></li>
									<li><a href="#fawloi_tab2" data-toggle="tab">Checklist</a></li>
									<li><a href="#fawloi_tab3" data-toggle="tab">Timeline</a></li>
									<li style="float:right;"><a href="'.$base_url.'/sites/default/files/contentpdf/user-guide-fawloi_declaration.pdf" target="_blank"><i class="fa fa-print"></i></a></li>
								</ul>
								<div class="tab-content">
									<div class="tab-pane active" id="fawloi_tab1">
										<div class="sop_cntainer_box">
										<p><strong>Responsibility: </strong>FAWLOI Declaration Submit</p>							
											<div class="sop_content">
												<div class="step_ord">Step-1:</div>
												<div class="res_activity"><strong>Activity:</strong> The Beneficiary shall open www.wblc.gov.in and will select schemes then click on Fawloi Option  .  All the notice appears in <strong>Notice for Receiving Declaration</strong> then click on Apply Now to date decaration. For a new user he/she fills up a Declaration Form Under Fowloi Scheme, in the portal of wblc.gov.in.
												</div>
											</div>
											<div class="sop_content">
												<div class="step_ord">Step-2:</div>
												<div class="res_activity"><strong>Activity:</strong> Click on Apply Now Button in action area.
												</div>
											</div>
											<div class="sop_content">
												<div class="step_ord">Step-3:</div>
												<div class="res_activity"><strong>Activity:</strong> Fill up all the details required<br>
												<strong>Unit Name:</strong> Type some letters of the name, and it suggest you matching Unit Names.
												<strong>Application No / Bank AC No:</strong> After enter application number or bank account number, the details will auto filled in in corresponding fields if the user exist with that application number or bank account number.<br> 
 												Then the address details have to enter and submit with SAVE and CONTINUE button. 

												</div>
											</div>	
											<div class="sop_content">
												<div class="step_ord">Step-4:</div>
												<div class="res_activity"><strong>Activity:</strong>Some document have to upload here like - Bank passbook (Mandatory), Photograph (Mandatory), Fawloi Receipt (Optional).
												PF Number, ESI Number, Aadhaar  is optional, but if any of these given must have to upload the corresponding document. Then Save and Continue.
												<p><strong>Documents Involved: All file should be PDF/JPG/JPEG/PNG(150 KB) except Photograph. Photograph should be JPG/JPEG/PNG(50 KB) </strong>
												
												</div>
											</div>	
											<div class="sop_content">
												<div class="step_ord">Step-5:</div>
												<div class="res_activity"><strong>Activity:</strong>Next go through the details properly then click on Checkbox. After the click on Checkbox a security code will send to register mobile number. Then enter code and submit. Download the system generated fawloi declaration.
												</div>
											</div>	
											<div class="sop_content">
												<div class="step_ord">Step-6:</div>
												<div class="res_activity"><strong>Activity:</strong>After Submit Beneficiary have to login with registered mobile number. 
												</div>
											</div>	
											<div class="sop_content">
												<div class="step_ord">Step-7:</div>
												<div class="res_activity"><strong>Activity:</strong> Upload the signed document. 
												<p><strong>Documents Involved: The Declaration form with Signature of the Beneficiary and Signature of the  Secretary of Registered Trade Union of the concerned unit along with Registration no. / Local MLA / Local MP with official Seal. </strong>
												 
												</div>
											</div>
											<br>																	
										</div>
									</div>
									
									<div class="tab-pane" id="fawloi_tab2">
										<div class="sop_cntainer_box">
											<div class="sop_content">
												<div class="step_ord">Scanned copies of the following documents whichever is applicable :</div>
													<ul style="list-style:disc; margin-left:20px;">
														<li>Bank passbook</li>
														<li>Recent Photograph</li>
														<li>Fawloi Receipt</li>
														<li>PF</li>
														<li>ESI</li>
														<li>Aadhaar</li>
														<li>The Declaration form with Signature</li>
														
													</ul>
											</div>	
										</div>
									</div>	
									<div class="tab-pane" id="fawloi_tab3">
										<div class="sop_cntainer_box">
											<div class="sop_content">
												<table border="0" cellpadding="0" cellspacing="" class="view-act-rules-table">
													<tr>
														<th width="5%">Sl.No.</th>
														<th width="50%">Designated officer</th>
														<th>Stipulated time limit</th>
													</tr>
													<tr>
														<td colspan="3">Not Available</td>
													</tr>														
												</table>													
											</div>	
										</div>
									</div>	
								</div>
							</div>	
						</div>				
					</div>
				</div>
			</div>';
	$content .= '<div id="user_guide_amndclra" class="modal fade applicant_login" role="dialog">
					<div class="modal-dialog">
						<div class="modal-content">
					  		<div class="modal-header">
								<button type="button" class="close" data-dismiss="modal">&times;</button>
								<h4 class="modal-title">User Guide</h4>
					  		</div>
					  		<div class="modal-body">
								<div class="tabbable">
									<ul class="nav nav-tabs">
									<li class="active"><a href="#amndclra_tab1" data-toggle="tab">How to apply?</a></li>
									<li><a href="#amndclra_tab2" data-toggle="tab">Checklist</a></li>
									<li><a href="#amndclra_tab3" data-toggle="tab">Timeline</a></li>
									<li style="float:right;"><a href="'.$base_url.'/sites/default/files/contentpdf/user-guide-principal-employer-registration-contract-labour.pdf" target="_blank"><i class="fa fa-print"></i></a></li>

									</ul>
									<div class="tab-content">
										<div class="tab-pane active" id="amndclra_tab1">
											<div class="sop_cntainer_box">
												<div class="sop_content">
													<p><strong>Responsibility: </strong>Principal Employer.</p>
													<div class="step_ord">Step-1</div>
													<div class="res_activity">If the you are an already registered user, he/she will log in using the credentials. For a new user he/she fills up a Common Application Form(CAF) in the Portal of wblc.gov.in, which includes mainly applicant’s Personal Information including PAN/Voters ID/Aadhar, any other unique number tagged with that establishment, Contact Information, Establishment Information, Number of workers employed, Login Information etc. with Preferred User name & Password and create a User Credential.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-2</div>
													<div class="res_activity">Logs into the system with the valid credentials and captcha which lands up in the Dashboard.</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-3</div>
													<div class="res_activity">
														<strong>If Offline/Manual certificate available</strong> select ALREADY REGISTERED option from menu and apply for online application of Principal Employer under the Contract Labour (Regulation and Abolition) Act , 1970 by filling up necessary information in the Form and uploads self-certified copies of required documents.<br>
														<strong>NEW REGISTRATION:</strong> Select NEW REGISTRATION from menu and applies for online Registration of Principal Employer under the Contract Labour (Regulation and Abolition) Act , 1970 by filling up necessary information in the Form and uploads self-certified copies of required documents.<br>
														<strong>Documents Involved: </strong>I.Valid Trade License, II.Articles of Association and Memorandum of Association/Partnership Deed, III.Factory License if any, IV.Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc., V.Any other document in support of correctness of the particulars mentioned in the application if required
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-4</div>
													<div class="res_activity">Add valid contractor information into the system.</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-5</div>
													<div class="res_activity">Views the Filled in Information in the Application Preview Section before Final Submission or in case of correction rolls back to earlier sections and makes corrections and submit the application. The application is then reverted back to concerned officials according to jurisdiction  for verification.</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">Step-6</div>
													<div class="res_activity">Logs into the system with the credentials and views the status of application marked for correction if sent back by the Inspector, makes the corrections and resubmits the application.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-7</div>
													<div class="res_activity">Views application status in the Dashboard marked as PAYMENT (PAY NOW) by ALC and makes payment Online through GRIPS. <br>After successful payment of fees statutory FORM-I is automatically generated. Applicant can digitally/in-hand signs the FORM-I and upload it. After FORM-I is successfully uploaded, an acknowledgement is generated. <br>In case it is returned back by the ALC he views on the Dashboard the status of application marked for correction and makes the necessary corrections and resubmits the application.<br/>
														<strong>Documents Involved: </strong>FORM-I
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-8</div>
													<div class="res_activity">If the application is verified registration certificate is shown in the dashboard and FORM-V is generated. You can digitally/in-hand sign the FORM-V and send via e-mail or through SMS. The Serial number in FORM-V is needed to contractors for applying of License.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Note:</div>
													<div class="res_activity">Track Status of Application through SMS Alerts generated through the System starting from Creating Login Credentials, Submission of Application, Correction Required, Payment of Fees , Status of Approval or Rejection , Status on Issue of Registration Certificate can be shown from the dashboard.
													</div>
												</div>											
											</div>
										</div>
										<div class="tab-pane" id="amndclra_tab2">
											<div class="sop_cntainer_box">
												<div class="sop_content">
													<div class="step_ord">Scanned copies of the following documents whichever is applicable :</div>
														<ul style="list-style:disc; margin-left:20px;">
															<li>Trade License</li>
															<li>Article of Association and Memorandum of Association / Partnership Deed</li>
															<li>Factory License if any</li>
															<li>Previous Registration Certificate (If Already registered user for Registration)</li>
															<li>Any other document in support of correctness of the particulars mentioned in the application if required</li>
															<li>Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc</li>				
															<li>Form I</li>		
														</ul>
												</div>
											</div>
										</div>	
										<div class="tab-pane" id="amndclra_tab3">
											<div class="sop_cntainer_box">
												<div class="sop_content">
													<table border="0" cellpadding="0" cellspacing="" class="view-act-rules-table">
														<tr>
															<th width="5%">Sl.No.</th>
															<th width="60%">Designated officer</th>
															<th>Stipulated time limit</th>
														</tr>
														<tr>
															<td>1</td>
															<td>Assistant Labour Commissioner</td>
															<td>30 days</td>
														</tr>
														<tr>
															<td>2</td>
															<td>Deputy Labour Commissioner</td>
															<td>45 days</td>
														</tr>
														<tr>
															<td>3</td>
															<td>Labour Commissioner</td>
															<td>60 days</td>
														</tr>
													</table>
													<p style="float:right;"><i class="fa fa-info-circle"></i> <a href="'.$base_url.'/sites/default/files/important_notification/RIGHT_TO_PUBLIC_SERVICES_NOTIFICATION.pdf" target="_blank">More Info.</a></p>
												</div>	
											</div>
										</div>	
									</div>
								</div>	
							</div>				
						</div>
					</div>
				</div>';
	$content .= '<div id="user_guide_licenseclra" class="modal fade applicant_login" role="dialog">
					<div class="modal-dialog">
						<div class="modal-content">
					  		<div class="modal-header">
								<button type="button" class="close" data-dismiss="modal">&times;</button>
								<h4 class="modal-title">User Guide</h4>
					  		</div>
					  		<div class="modal-body">
								<div class="tabbable">
									<ul class="nav nav-tabs">
										<li class="active"><a href="#licenseclra_tab1" data-toggle="tab">How to apply?</a></li>
										<li><a href="#licenseclra_tab2" data-toggle="tab">Checklist</a></li>
										<li><a href="#licenseclra_tab3" data-toggle="tab">Timeline</a></li>
										<li style="float:right;"><a href="'.$base_url.'/sites/default/files/contentpdf/user-guide-clra-license.pdf" target="_blank"><i class="fa fa-print"></i></a></li>
									</ul>
									<div class="tab-content">
										<div class="tab-pane active" id="licenseclra_tab1">
											<div class="sop_cntainer_box">
												<div class="sop_content">
													<div class="step_ord">Step-1</div>
													<p><strong>Responsibility: </strong>Contractor</p>
													<div class="res_activity"><strong>Activity:</strong>The applicant shall open ‘www.wblc.gov.in’ and will select e-services and licensing of contractors thereafter. A guide line will appear agreeing which he/she will land up in the log in page. If the applicant is an already registered user, he/she will log in using the credentials. For a new user he/she fills up a Common Application Form(CAF) in the Portal of wblc.gov.in, which includes mainly applicant’s Name, Address, Contact Details, E Mail, PAN, Voters ID, Aadhaar number, Driving License , Trade License Number, any other unique number tagged with that establishment, establishment name and details including address, etc. with Preferred User name & Password and creates a User Credential.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-2</div>
													<p><strong>Responsibility: </strong>Contractor</p>
													<div class="res_activity"><strong>Activity:</strong>Logs into the System with the Valid Credentials and Captcha.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-3</div>
													<p><strong>Responsibility: </strong>Contractor.</p>
													<div class="res_activity">
														<strong>Activity:</strong>Lands up into the Dashboard.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-4</div>
													<p><strong>Responsibility: </strong>Principal Employer </p>
													<div class="res_activity"><strong>Activity:</strong>If the contractor is yet to be provided the system generated unique Form-V number by the Principal Employer as mentioned in item 24 of the process for registration of establishment, the Principal Employer may Log into the system with the Credentials.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-5</div>
													<p><strong>Responsibility: </strong>Principal Employer.</p>
													<div class="res_activity"><strong>Activity:</strong>Views the Form-V, digitally signs or can sign manually in the hard copy and send via e mail or through SMS the Unique number of Form V to Contractors for applying of License.
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">Step-6</div>
													<p><strong>Responsibility: </strong>Contractor</p>
													<div class="res_activity"> <strong>Activity:</strong>Enters the system generated Unique Form V number issued by Principal Employer, through which maximum no of relevant fields for application for licensing would be populated in the Form.
													<p><strong>Documents Involved: </strong> I.Valid Form V, II.Valid Work Order, III.Any other documents in support of correctness of particulars furnished in
application form, IV.Trade license </p>
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-7</div>
													<p><strong>Responsibility: </strong>Contractor</p>
													<div class="res_activity"> <strong>Activity:</strong> Fills up the additional Information as required.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-8</div>
													<p><strong>Responsibility: </strong>Contractor</p>
													<div class="res_activity"><strong>Activity:</strong>Views the Filled in Information before Final Submit. In case of
corrections rolls back to earlier sections and makes corrections and Saves the application.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-9</div>
													<p><strong>Responsibility: </strong>Contractor</p>
													<div class="res_activity"><strong>Activity:</strong>Final Submission of the application.
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">Step-10</div>
													<p><strong>Responsibility: </strong>Inspector concerned</p>
													<div class="res_activity"><strong>Activity:</strong>Logs into the system with Credentials.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-11</div>
													<p><strong>Responsibility: </strong>Inspector concerned</p>
													<div class="res_activity"><strong>Activity:</strong>Lands up in the Dashboard and selects the Application List from the Left hand Panel.
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">Step-12</div>
													<p><strong>Responsibility: </strong>Inspector concerned</p>
													<div class="res_activity"><strong>Activity:</strong>He opens the application and verifies details one by one and marks Tick (√) which he finds correct and leaves the one as not verified which he finds Incorrect.
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">Step-13</div>
													<p><strong>Responsibility: </strong>Inspector concerned</p>
													<div class="res_activity"><strong>Activity:</strong>Logs into the system with the Credentials and downloads the BOCW Registration FORM-II digitally signed by ALC.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-14</div>
													<p><strong>Responsibility: </strong>Inspector concerned</p>
													<div class="res_activity"><strong>Activity:</strong>He may reverts back to applicant for Correction / Rectification with remarks in case any correction or rectification is required or may Call the applicant through System if required to answer specific queries or can forward it to the ALC for approval or rejection.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-15</div>
													<p><strong>Responsibility: </strong>Contractor</p>
													<div class="res_activity"><strong>Activity:</strong>Logs into the system with the Credentials.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-16</div>
													<p><strong>Responsibility: </strong>Contractor</p>
													<div class="res_activity"><strong>Activity:</strong>Views on the Dashboard the status of application marked for correction if sent back by the Inspector and makes the corrections and resubmits the application.
													<p><strong>Documents Involved: </strong> I.Valid Work Order, II.Any other documents in support of correctness of particulars furnished in
application form,III.Valid Form V, IV.Trade license </p>
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-17</div>
													<p><strong>Responsibility: </strong>Inspector concerned </p>
													<div class="res_activity"><strong>Activity:</strong>Logs into the system with Credentials. 
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-18</div>
													<p><strong>Responsibility: </strong>Inspector concerned </p>
													<div class="res_activity"><strong>Activity:</strong>Verifies or re-verifies the application as the case may be and if found satisfactory forwards the same to the ALC. 
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-19</div>
													<p><strong>Responsibility: </strong>ALC of the Subdivision</p>
													<div class="res_activity"><strong>Activity:</strong>Logs into the system with the Credentials.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-20</div>
													<p><strong>Responsibility: </strong>ALC of the Subdivision</p>
													<div class="res_activity"><strong>Activity:</strong>Lands up in the Dashboard and views all the applications forwarded by Inspector and if found satisfactory allows for Payment by the applicant. In case ALC finds something that is not satisfactory in the application he may revert it back either to the applicant or the Inspector as he deems fit. ALC can also reject the application if finds so after observing principles of natural justice. 
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-21</div>
													<p><strong>Responsibility: </strong>Contractor</p>
													<div class="res_activity"><strong>Activity:</strong>Logs into the system with the Credentials. 
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-22</div>
													<p><strong>Responsibility: </strong>Contractor</p>
													<div class="res_activity"><strong>Activity:</strong>Views on the Dashboard the status of application marked for PAYMENT (PAY NOW) by ALC and makes payment Online through GRIPS and digitally signs the Form IV and uploads in the system.In case it is returned back by the ALC he views on the Dashboard the status of application marked for correction and makes the corrections and resubmits the application.  
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-23</div>
													<p><strong>Responsibility: </strong>Contractor</p>
													<div class="res_activity"><strong>Activity:</strong>If the applicant cannot sign the application digitally through DSC or else he is required to submit the signed hard copy of the application form before the ALC. He is also required to upload a scanned copy of signed system generated application form.
													<p><strong>Document Involved:</strong> I. Original signed application, II.Proof of submission of fees and security deposit </p>
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-24</div>
													<p><strong>Responsibility: </strong>ALC of the Subdivision </p>
													<div class="res_activity"><strong>Activity:</strong>Logs into the system with the Credentials. 
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-25</div>
													<p><strong>Responsibility: </strong>ALC of the Subdivision </p>
													<div class="res_activity"><strong>Activity:</strong>After satisfactory payment ALC issues Digitally signed License Certificate (Form VI ) and Uploads in the system for Applicant to download it from the system. 
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-26</div>
													<p><strong>Responsibility: </strong>Contractor </p>
													<div class="res_activity"><strong>Activity:</strong>Logs into the system with the Credentials & downloads the Digitally signed License Certificate.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-27</div>
													<p><strong>Responsibility: </strong>Contractor </p>
													<div class="res_activity"><strong>Activity:</strong>Track Status of Application through SMS Alerts generated through the System starting from Creating Login Credentials, Submission of Application, Correction Required, Payment of Fees , Status of Approval or Rejection, Status on issue of License.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-28</div>
													<p><strong>Responsibility: </strong>Inspector Concerned  </p>
													<div class="res_activity"><strong>Activity:</strong>If the application is reverted back to the Inspector by the ALC, he will view the application along with remarks, if any, from the ALC, after logging into the system with credentials. He can either sent back the application to the applicant or forward the same to the ALC with remarks if any. 
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-29</div>
													<p><strong>Responsibility: </strong>Contractor </p>
													<div class="res_activity"><strong>Activity:</strong>The contractor, in some cases, may also apply for license without the unique form V number. In that  case the contractor will skip the above stated step and blank Form IV will be required to be filled up by the applicant after entering valid registration no of the Principal Employer. There after process as mentioned in 8 and onwards will follow. 
													<p><strong>Documents Involved: </strong> I.Valid Work Order, II.Any other documents in support of correctness of particulars furnished in
application form,III.Valid Form V, IV.Trade license </p>
													</div>
												</div> 
											</div>
										</div>
										<div class="tab-pane" id="licenseclra_tab2">
											<div class="sop_cntainer_box">
												<div class="sop_content">
													<div class="step_ord">Scanned copies of the following documents whichever is applicable :</div>
														<ul style="list-style:disc; margin-left:20px;">
															<li>Work Order</li>
															<li>Form V</li>	
															<li>Trade License / Address proof Certificate</li>
															<li>License Certificate or Last Renewal Certificate</li>
															<li>Other</li>
															<li>Form IV / Form VII</li>
														</ul>
												</div>	
											</div>
										</div>	
										<div class="tab-pane" id="licenseclra_tab3">
											<div class="sop_cntainer_box">
												<div class="sop_content">
													<table cellpadding="0" cellspacing="0" border="0" class="view-act-rules-table">
														<tr>
															<th width="5%">Sl.No.</th>
															<th width="60%">Designated officer</th>
															<th>Stipulated time limit</th>
														</tr>
														<tr>
															<td>1</td>
															<td>Assistant Labour Commissioner</td>
															<td>30 days</td>
														</tr>
														<tr>
															<td>2</td>
															<td>Deputy Labour Commissioner</td>
															<td>45 days</td>
														</tr>
														<tr>
															<td>3</td>
															<td>Labour Commissioner</td>
															<td>60 days</td>
														</tr>
													</table>
													<p style="float:right;"><i class="fa fa-info-circle"></i> <a href="'.$base_url.'/sites/default/files/important_notification/RIGHT_TO_PUBLIC_SERVICES_NOTIFICATION.pdf" target="_blank">More Info.</a></p>
												</div>	
											</div>
										</div>
									</div>
								</div>	
							</div>				
						</div>
					</div>
				</div>';
	$content .= '<div id="howtomtw" class="modal fade applicant_login" role="dialog">
					<div class="modal-dialog">
						<div class="modal-content">
					  		<div class="modal-header">
								<button type="button" class="close" data-dismiss="modal">&times;</button>
								<h4 class="modal-title">User Guide</h4>
					  		</div>
					  		<div class="modal-body">
								<div class="tabbable">
									<ul class="nav nav-tabs">
										<li class="active"><a href="#tab1_reg" data-toggle="tab">How to apply?</a></li>
										<li><a href="#tab2_reg" data-toggle="tab">Checklist</a></li>
										<li><a href="#tab3_reg" data-toggle="tab">Timeline</a></li>
										<li style="float:right;"><a href="'.$base_url.'/sites/default/files/user-manual-mtw.pdf" target="_blank"><i class="fa fa-print"></i></a></li>
									</ul>
									<div class="tab-content">
										<div class="tab-pane active" id="tab1_reg">
											<div class="sop_cntainer_box">
											<p><strong>Responsibility: </strong>Motor Transport Undertaking</p>							
												<div class="sop_content">
													<div class="step_ord">Step-1:</div>
													<div class="res_activity"><strong>Activity: </strong>The applicant shall open ‘www.wblc.gov.in’ and will select e-services and Motor Transport Undertaking Workers Act thereafter. A guide line will appear, agreeing which he/she will land up in the log in page. If the applicant is an already registered user, he/she will log in using the credentials. For a new user he/she fills up a Registration Form in the Portal of wblc.gov.in, which includes information of Motor Transport Undertaking, with Preferred User name & Password and creates a User Credential.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-2:</div>
													<div class="res_activity"><strong>Activity: </strong>Logs into the System with the Valid Credentials and Captcha.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-3:</div>
													<div class="res_activity"><strong>Activity: </strong>Lands up into the Dashboard.
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">Step-4:</div>
													<div class="res_activity"><strong>Activity: </strong>Select ‘New Registration’ option from the left menu of Dashboard.
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">Step-5:</div>
													<div class="res_activity"><strong>Activity: </strong>Fill all the relevant & mandatory fields then proceed.
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">Step-6:</div>
													<div class="res_activity"><strong>Activity: </strong>Upload all the mandatory signed documents. 
													<p><strong>Documents Involved:</strong> I.Trade License, II. Article of Association / Partnership Deed, III. Blue Book/ Smart Card Issued by Motor Vehicles, IV. Insurance Certificate of Motor Vehicles, V. Address Proof, VI. Documents substantiating correctness of particulars mentioned in the application.
													</p>
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-7:</div>
													<div class="res_activity"><strong>Activity: </strong>Views the Filled in Information before Final Submit in the Preview page. In case of corrections, rolls back to specific sections through corresponding and makes corrections and Saves the application.
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">Step-8:</div>
													<div class="res_activity"><strong>Activity: </strong>Fees payment and Final Submission of the application process.
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">Step-9:</div>
													<div class="res_activity"><strong>Activity:</strong>User can view and download the Registration Certificate after veryfing the Application by authorized officials after Final Submission.
													</div>
												</div>																				
											</div>
										</div>
										
										<div class="tab-pane" id="tab2_reg">
											<div class="sop_cntainer_box">
												<div class="sop_content">
													<div class="step_ord">Scanned copies of the following documents whichever is applicable :</div>
														<ul style="list-style:disc; margin-left:20px;">
															<li>Trade License</li>
															<li>Address Proof</li>
															<li>Article of Association</li>
															<li>Partnership Deed</li>
															<li>Blue Book/ Smart Card Issued by Motor Vehicles</li>
															<li>Insurance Certificate of Motor Vehicles</li>
														</ul>
												</div>	
											</div>
										</div>	
										<div class="tab-pane" id="tab3_reg">
											<div class="sop_cntainer_box">
												<div class="sop_content">
													<table border="0" cellpadding="0" cellspacing="" class="view-act-rules-table">
														<tr>
															<th width="5%">Sl.No.</th>
															<th width="60%">Designated officer</th>
															<th>Stipulated time limit</th>
														</tr>
														<tr>
															<td>1</td>
															<td>Assistant Labour Commissioner</td>
															<td>30 days</td>
														</tr>
														<tr>
															<td>2</td>
															<td>Deputy Labour Commissioner</td>
															<td>45 days</td>
														</tr>
														<tr>
															<td>3</td>
															<td>Labour Commissioner</td>
															<td>60 days</td>
														</tr>
													</table>
													<p style="float:right;"><i class="fa fa-info-circle"></i> <a href="'.$base_url.'/sites/default/files/important_notification/RIGHT_TO_PUBLIC_SERVICES_NOTIFICATION.pdf" target="_blank">More Info.</a></p>
												</div>	
											</div>
										</div>	
									</div>
								</div>	
							</div>				
						</div>
					</div>
				</div>';			
		$content .= '<div id="howtomtwren" class="modal fade applicant_login" role="dialog">
					<div class="modal-dialog">
						<div class="modal-content">
					  		<div class="modal-header">
								<button type="button" class="close" data-dismiss="modal">&times;</button>
								<h4 class="modal-title">User Guide</h4>
					  		</div>
					  		<div class="modal-body">
								<div class="tabbable">
									<ul class="nav nav-tabs">
										<li class="active"><a href="#tab1_mtw" data-toggle="tab">How to apply?</a></li>
										<li><a href="#tab2_mtw" data-toggle="tab">Checklist</a></li>
										<li><a href="#tab3_mtw" data-toggle="tab">Timeline</a></li>
										<li style="float:right;"><a href="'.$base_url.'/sites/default/files/user-manual-mtw.pdf" target="_blank"><i class="fa fa-print"></i></a></li>
									</ul>
									<div class="tab-content">
										<div class="tab-pane active" id="tab1_mtw">
											<div class="sop_cntainer_box">
											<p><strong>Responsibility: </strong>Motor Transport Undertaking</p>							
												<div class="sop_content">
													<div class="step_ord">Step-1:</div>
													<div class="res_activity"><strong>Activity: </strong>The applicant shall open ‘www.wblc.gov.in’ and will select e-services and Motor Transport Undertaking Workers Act thereafter. A guide line will appear, agreeing which he/she will land up in the log in page. If the applicant is an already registered user, he/she will log in using the credentials. For a new user he/she fills up a Registration Form in the Portal of wblc.gov.in, which includes information of Motor Transport Undertaking, with Preferred User name & Password and creates a User Credential.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-2:</div>
													<div class="res_activity"><strong>Activity: </strong>Logs into the System with the Valid Credentials and Captcha.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-3:</div>
													<div class="res_activity"><strong>Activity: </strong>Lands up into the Dashboard.
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">Step-4:</div>
													<div class="res_activity"><strong>Activity: </strong>Select ‘Apply for Renewal’ option from the left menu of Dashboard.
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">Step-5:</div>
													<div class="res_activity"><strong>Activity: </strong>Fill all the relevant & mandatory fields then proceed.
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">Step-6:</div>
													<div class="res_activity"><strong>Activity: </strong>Upload all the mandatory signed documents. 
													<p><strong>Documents Involved:</strong> I.Trade License, II. Article of Association / Partnership Deed, III. Blue Book/ Smart Card Issued by Motor Vehicles, IV. Insurance Certificate of Motor Vehicles, V. Address Proof, VI. Documents substantiating correctness of particulars mentioned in the application.
													</p>
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-7:</div>
													<div class="res_activity"><strong>Activity: </strong>Views the Filled in Information before Final Submit in the Preview page. In case of corrections, rolls back to specific sections through corresponding and makes corrections and Saves the application.
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">Step-8:</div>
													<div class="res_activity"><strong>Activity: </strong>Fees payment and Final Submission of the application process.
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">Step-9:</div>
													<div class="res_activity"><strong>Activity:</strong>User can view and download the Registration Certificate after veryfing the Application by authorized officials after Final Submission.
													</div>
												</div>																				
											</div>
										</div>
										
										<div class="tab-pane" id="tab2_mtw">
											<div class="sop_cntainer_box">
												<div class="sop_content">
													<div class="step_ord">Scanned copies of the following documents whichever is applicable :</div>
														<ul style="list-style:disc; margin-left:20px;">
															<li>Trade License</li>
															<li>Address Proof</li>
															<li>Article of Association</li>
															<li>Partnership Deed</li>
															<li>Blue Book/ Smart Card Issued by Motor Vehicles</li>
															<li>Insurance Certificate of Motor Vehicles</li>
														</ul>
												</div>	
											</div>
										</div>	
										<div class="tab-pane" id="tab3_mtw">
											<div class="sop_cntainer_box">
												<div class="sop_content">
													<table border="0" cellpadding="0" cellspacing="" class="view-act-rules-table">
														<tr>
															<th width="5%">Sl.No.</th>
															<th width="60%">Designated officer</th>
															<th>Stipulated time limit</th>
														</tr>
														<tr>
															<td>1</td>
															<td>Assistant Labour Commissioner</td>
															<td>30 days</td>
														</tr>
														<tr>
															<td>2</td>
															<td>Deputy Labour Commissioner</td>
															<td>45 days</td>
														</tr>
														<tr>
															<td>3</td>
															<td>Labour Commissioner</td>
															<td>60 days</td>
														</tr>
													</table>
													<p style="float:right;"><i class="fa fa-info-circle"></i> <a href="'.$base_url.'/sites/default/files/important_notification/RIGHT_TO_PUBLIC_SERVICES_NOTIFICATION.pdf" target="_blank">More Info.</a></p>
												</div>	
											</div>
										</div>	
									</div>
								</div>	
							</div>				
						</div>
					</div>
				</div>';
	
	$content .= '<div id="howtobocwacess" class="modal fade applicant_login" role="dialog">
					<div class="modal-dialog">
						<div class="modal-content">
					  		<div class="modal-header">
								<button type="button" class="close" data-dismiss="modal">&times;</button>
								<h4 class="modal-title">User Guide</h4>
					  		</div>
					  		<div class="modal-body">
								<div class="tabbable">
									<ul class="nav nav-tabs">
										<li class="active"><a href="#tab1_mtw" data-toggle="tab">How to apply?</a></li>
										<li><a href="#tab2_mtw" data-toggle="tab">Checklist</a></li>
										<li><a href="#tab3_mtw" data-toggle="tab">Timeline</a></li>
										<li style="float:right;"><a href="'.$base_url.'/sites/default/files/user-manual-bocwacess.pdf" target="_blank"><i class="fa fa-print"></i></a></li>
									</ul>
									<div class="tab-content">
										<div class="tab-pane active" id="tab1_mtw">
											<div class="sop_cntainer_box">
												<p><strong>Responsibility: </strong></p>							
												<div class="sop_content">
													<div class="step_ord">Step-1:</div>
													<div class="res_activity"><strong>Activity: </strong>The applicant shall open ‘www.wblc.gov.in’ and will select e-services and Under The Building and Other Construction Workers&prime; Welfare Cess Act, 1996. A guide line will appear, agreeing which he/she will land up in the log in page. If the applicant is an already registered user, he/she will log in using the credentials. For a new user he/she fills up a Registration Form in the Portal of wblc.gov.in, which includes information of The Building and Other Construction Workers&prime; Welfare Cess Act, 1996, with Preferred User name & Password and creates a User Credential.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-2:</div>
													<div class="res_activity"><strong>Activity: </strong>Logs into the System with the Valid Credentials and Captcha.
													</div>
												</div>
												<div class="sop_content">
													<div class="step_ord">Step-3:</div>
													<div class="res_activity"><strong>Activity: </strong>Lands up into the Dashboard.
													</div>
												</div>	
												<div class="sop_content">
													<div class="step_ord">Step-4:</div>
													<div class="res_activity"><strong>Activity: </strong></div>
												</div>	
											</div>
										</div>
										
										<div class="tab-pane" id="tab2_mtw">
											<div class="sop_cntainer_box">
												<div class="sop_content">
													
														
												</div>	
											</div>
										</div>	
										<div class="tab-pane" id="tab3_mtw">
											<div class="sop_cntainer_box">
												<div class="sop_content">													
													
												</div>	
											</div>
										</div>	
									</div>
								</div>	
							</div>				
						</div>
					</div>
				</div>';
	
	
	
	
	return $content;
}
	
function office_contact_add_list($office = ''){
	if($office == 'jlc' ){
		$active = 'active';
	}else if($office == 'dlc'){
		$activedlc = 'active';
	}else if($status == 'alc'){
		$activealc = 'active';
	}else if($status == 'inspector'){
		$activeinspector = 'active';
	}else if($status == 'others'){
		$activeothers = 'active';
	}
	
	$output = '<div class="nav-tabs-custom">
					<ul class="nav nav-tabs">
						<li class="'.$active.'">'.l('JLC Office', 'office-contact-admin-list/jlc').'</li>
						<li class="'.$activedlc.'">'.l('DLC Office', 'office-contact-admin-list/dlc').'</li>
						<li class="'.$activealc.'">'.l('ALC Office', 'office-contact-admin-list/alc').'</li>												
						<li class="'.$activeinspector.'">'.l('IMW Office', 'office-contact-admin-list/inspector').'</li>
						<li class="'.$activeothers.'">'.l('Others', 'office-contact-admin-list/others').'</li>
					</ul>
					<div class="tab-content">
						<div class="tab-pane active" id="fa-icons">
							<div id="new"><div class="feedback-scroll">'; 
	
   	$header = array(
           array('data'=> 'Sl. No.', 'width' => '8%'),	
		   array('data'=> 'Name and Address of the office'),
		   array('data'=> 'Contact Number', 'width' => '15%'),
		   array('data'=> 'Action', 'width' => '13%')	  
		  );		
	
	$query = db_select('sub_division', 'sd');
	$query->leftJoin('l_rlo_additional_info', 'rai', 'sd.sub_div_code = rai.sub_div_code');
	// $query->leftJoin('block_mun_master', 'bmm', 'bmm.block_code = rai.sub_div_code');	
	$query->fields('rai', array('id', 'office_name', 'office_designation', 'office_mobile_number', 'clra_certificate_address'));
	$query->fields('sd', array('sub_div_code', 'sub_div_name'));
	// $query->fields('bmm', array('block_mun_name'));
	$query->condition('rai.office_name', strtoupper($office), '=');
	$query->orderBy('office_name', 'asc');
	
	$query = $query->execute();
	$i = 1;
	if($query->rowCount()>0){
		foreach($query as $data){
			// $answer = '<font color="red">Not Available</font>';
			// if($data->is_active == 'Y'){$status = '<i class="fa fa-check-circle  fa-2x status" style="color:green; cursor:pointer;" st="Y" data-id="'.$data->id.'"></i>';}else{$status = '<i class="fa fa-times-circle fa-2x status" style="color:red;cursor:pointer;" st="N" data-id="'.$data->id.'"></i>';}
			$viewmore = l('<i class="fa fa-info-circle"></i> View More', 'office-contact-admin-details/'.encryption_decryption_fun('encrypt',$data->sub_div_code), array('html' => TRUE,'attributes'=> array('target'=>'_blank')));	
			// if($data->answer != ''){$answer = $data->answer;}
			if ($data->office_name == 'JLC'){
				$office_name = 'OFFICE OF THE JOINT LABOUR COMMISSIONER';
			}elseif ($data->office_name == 'DLC'){
				$office_name = 'OFFICE OF THE DEPUTY LABOUR COMMISSIONER';
			}elseif($data->office_name == 'ALC'){
				$office_name = 'OFFICE OF THE ASSISTANT LABOUR COMMISSIONER';
			}elseif($data->office_name == 'KLC'){
				$office_name = 'OFFICE OF THE LABOUR COMMISSIONER EL & MW SECTION';
			}elseif($data->office_name == 'INSPECTOR'){
				$office_name = strtoupper($data->block_mun_name).', LABOUR WELFARE FACILITATION CENTRE';
			}
			$rows[] = array(
						$i++,
						'<strong>'.$office_name.', '.strtoupper($data->sub_div_name).'</strong><br>'.$data->clra_certificate_address,
						$data->ofice_mobile_number,
						$viewmore		
					);
		}
	}
		
	$variables = array(
	  		'attributes' 		=> array('class' => array('table table-striped table-responsive admin-custom-table')), 
	  		'header' 			=> $header,
	  		'rows'				=> $rows,
			'empty' 			=> t("No data found !")
	  );
	
	$output  .= theme('datatable', $variables);
	
	$output .= '</div></div></div>';
	
	return $output;
}

function office_contact_add_view($sub_div_code = ''){ 
	$query = db_select('l_rlo_additional_info', 'rai');
	$query->leftJoin('sub_division', 'sd', 'sd.sub_div_code = rai.sub_div_code');
	$query->fields('rai', array('id', 'office_name', 'office_designation', 'office_mobile_number', 'office_email_address', 'clra_certificate_address'));
	$query->fields('sd', array('sub_div_code', 'sub_div_name', 'district_name','district_code'));
	$query->condition('rai.sub_div_code', encryption_decryption_fun('decrypt', $sub_div_code), '=');
	$obj = $query->execute()->fetchObject();
	
	if ($obj->office_name == 'JLC'){
		$office_name = 'OFFICE OF THE JOINT LABOUR COMMISSIONER';
	}elseif ($obj->office_name == 'DLC'){
		$office_name = 'OFFICE OF THE DEPUTY LABOUR COMMISSIONER';
	}elseif($obj->office_name == 'ALC'){
		$office_name = 'OFFICE OF THE ASSISTANT LABOUR COMMISSIONER';
	}elseif($obj->office_name == 'KLC'){
		$office_name = 'OFFICE OF THE LABOUR COMMISSIONER EL & MW SECTION';
	}elseif($obj->office_name == 'INSPECTOR'){
		// $office_name = strtoupper($data->block_mun_name).', LABOUR WELFARE FACILITATION CENTRE';
	}
	
	$query1 = db_select('l_customuser_relation_address', 'cr');
	$query1->leftJoin('l_custom_user_detail', 'cud', 'cud.usr_id = cr.user_id');
	$query1->leftJoin('users', 'u', 'u.uid = cr.user_id');
	$query1->fields('cud', array('srlno', 'usr_id', 'user_place,', 'fullname', 'mobile', 'degisnation', 'officenumber', 'retirement_date', 'employee_id', 'dob', 'govt_mobile', 'govt_email'));
	$query1->fields('cr', array('id'));
	$query1->fields('u', array('mail'));
	$query1->condition('cr.sub_div_code', encryption_decryption_fun('decrypt', $sub_div_code), '=');
	$query1->orderby('cud.usr_rid', 'asc');
	$query1->orderby('cud.dob', 'desc');
	$query1 = $query1->execute();
	
	if($query1->rowCount()>0){
		$header = array(
			   array('data'=> 'Sl. No.', 'width' => '8%'),
			    array('data'=> 'Office Posting', 'width' => '20%'),
			   array('data'=> 'Name'),
			   array('data'=> 'Designation', 'width' => '25%'),
			   array('data'=> 'Mobile', 'width' => '12%'),			  
			   array('data'=> 'Action', 'width' => '15%')	  
		  	);
		$i = 1;
		$offficer_data = '';
		
		foreach($query1 as $data){
			$viewinfo = '<a href="javascript:void(0)" data-toggle="modal" data-target="#myModal_'.$i.'"><i class="fa fa-info-circle"></i> View</a>&nbsp;&nbsp;
						'.l('<i class="fa fa-edit"></i> Edit', 'conatact-update/'.encryption_decryption_fun('encrypt', $data->srlno).'/'.encryption_decryption_fun('encrypt', $data->usr_id), array('html' => true)).'
						 <div class="modal fade" id="myModal_'.$i.'" role="dialog">
							<div class="modal-dialog">
								<div class="box box-primary box-solid">
									<div class="box-header">
										<button type="button" class="close" data-dismiss="modal">&times;</button>
										<h3 class="box-title">'.$data->user_place.'</h3>
									</div>
									<div class="modal-body">
										<table cellpadding="0" cellspacing="0" border="0" class="table table-striped table-responsive admin-custom-table">
											<tr>
												<td>Name</td>
												<td>'.$data->fullname.'</td>
											 </tr>
											 <tr>
												 <td>Designation(Posting)</td>
												 <td>'.$data->degisnation.'</td>
											 </tr>
											 <tr>
												 <td>Designation</td>
												 <td>'.$data->degisnation_officer.'</td>
											 </tr>
											 <tr>
												 <td>Employee Id</td>
												 <td>'.$data->employee_id.'</td>
											 </tr>
											 <tr>
												 <td>Date of Birth</td>
												<td>'.date('d/m/Y', strtotime($data->dob)).'</td>
											 </tr>
											 <tr>
												 <td>Mobile</td>
												 <td>'.$data->mobile.'</td>
											 </tr>
											 <tr>
												 <td>Office Contact Number</td>
												<td>'.$data->officenumber.'</td>
											 </tr>
											 <tr>
												 <td>Govt. Mobile</td>
												<td>'.$data->govt_mobile.'</td>
											 </tr>
											 <tr>
												 <td>Email</td>
												 <td>'.$data->mail.'</td>
											 </tr>
											  <tr>
												 <td>Govt. Email</td>
												 <td>'.$data->govt_email.'</td>
											 </tr>
											 <tr>
												 <td>Retirement Date</td>
												 <td>'.date('d/m/Y', strtotime($data->retirement_date)).'</td>
											 </tr>
										 </table>
									<div class="modal-footer">
										<a href="/conatact-update/'.encryption_decryption_fun('encrypt', $data->srlno).'/'.encryption_decryption_fun('encrypt', $data->usr_id).' class="btn btn-primary" style="color:#fff;"><i class="fa fa-edit"></i> Edit</a>
										<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
									</div>
								</div>
							</div>
						</div>';
			$rows[] = array(
						$i++,
						$data->user_place,
						'<strong>'.$data->fullname.'</strong>',						
						$data->degisnation,
						$data->mobile,						
						$viewinfo 	
					);
					
		}
	}
	
	$offficer_data = theme('datatable',array('attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')), 'header' => $header, 'rows' => $rows, 'empty' => t("No data found !")));
	
	$add_contact_number = '<a href="javascript:void(0)" data-toggle="modal" data-target="#myModal1"><i class="fa fa-edit"></i></a>';
	$add_email = '<a href="javascript:void(0)" data-toggle="modal" data-target="#myModal2"><i class="fa fa-edit"></i></a>';

	$addmore = l("<button type='button' class='btn btn-warning pull-right' title='Add more'><i class='fa fa-plus'></i> Add More</button>", 'add-officer-detail/'.$sub_div_code.'/'.encryption_decryption_fun('encrypt', $obj->district_code), array('html' => TRUE)); 
	
	// others user details
	$header1 = array(
			   array('data'=> 'Sl. No.', 'width' => '8%'),	
			   array('data'=> 'Name'),
			   array('data'=> 'Designation', 'width' => '25%'),
			   array('data'=> 'Mobile', 'width' => '12%'),
			   array('data'=> 'Email', 'width' => '20%'),
			   array('data'=> 'Action', 'width' => '10%')	  
		  	);
			
	$query2 = db_select('l_office_emp_details', 'oed');
	$query2->fields('oed', array('emp_id', 'name', 'designation', 'mobile', 'personal_email'));
	$query2->condition('oed.present_posting_subdivision_code', encryption_decryption_fun('decrypt', $sub_div_code), '=');
	$query2 = $query2->execute();
	
	if($query2->rowCount()>0){
		foreach($query2 as $data){
			$rows1[] = array(
						$i++,
						'<strong>'.$data->name,
						$data->designation,
						$data->mobile,
						$data->personal_email,
						$editinfo		
					);
					
		}
	}
	$othersoffficer_data = theme('datatable',array('attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')), 'header' => $header1, 'rows' => $rows1, 'empty' => t("No data found !")));
	
	$output = '<div class="box box-primary"><div class="box-body"><div class="row"><div class="col-md-6">
    				<div class="box box-primary box-solid">
						<div class="box-header with-border"><i class="ion ion-clipboard"></i> Name and Address of the office</div>
    					<div class="box-body">
							<div class="feedback-scroll">
							<table width="100%" border="0" class="table table-bordered table-responsive">
								<tr>
								    <td style="width:25%"><strong>Name</strong></td>
								    <td style="width:75%">'.$office_name.'</td>
								</tr>
								<tr>
								    <td><strong>Address</strong></td>
								    <td>'.$obj->clra_certificate_address.'</td>
								</tr>
								<tr>
								    <td><strong>Subdivision</strong></td>
								    <td>'.$obj->sub_div_name.'</td>
								</tr>
								<tr>
								    <td><strong>District</strong></td>
								    <td>'.$obj->district_name.'</td>
								</tr>
								<tr>
								    <td><strong>Contact Number</strong></td>
								    <td>'.$obj->office_mobile_number.' '.$add_contact_number.'</td>
								</tr>
								<tr>
								    <td><strong>Email Address</strong></td>
								    <td>'.$obj->office_email_address.' '.$add_email.'</td>
								</tr>
							</table>
							</div>
						</div>
					</div>
			   </div>
			   <div class="col-md-6">
					<div class="box box-primary box-solid">
						<div class="box-header with-border"><i class="ion ion-clipboard"></i> Others Information</div>
						<div class="box-body">
							<div class="feedback-scroll">
							<table width="100%" border="0" class="table table-bordered table-responsive">
								<tr>
								    <td style="width:50%">Number of offices</td>
								    <td style="width:50%"></td>
								</tr>
								<tr>
								    <td>Number of office staff</td>
								    <td></td>
								</tr>
								<tr>
								    <td>Number of office staff</td>
								    <td></td>
								</tr>
								<tr>
								    <td>Number of Block/Municipility</td>
								    <td></td>
								</tr>
								<tr>
								    <td>Number of Gram Panchayet/Ward</td>
								    <td></td>
								</tr>
								<tr>
								    <td>Number of Police Station</td>
								    <td></td>
								</tr>
							</table>
							</div>
						</div>
					</div>
				</div></div>
			   <div class="row">
			   <div class="col-md-12">
					<div class="box-header with-border"><i class="ion ion-clipboard"></i> Registrar User Details</div>
					<div class="box-body">
						<div class="feedback-scroll">'.$offficer_data.'</div>
					</div>
				</div>
				</div>
				<div class="row">
			    <div class="col-md-12">
					<div class="box-header with-border"><i class="ion ion-clipboard"></i> Others user Details '.$addmore.'</div>
					<div class="box-body">
						<div class="feedback-scroll">'.$othersoffficer_data.'</div>
					</div>
				</div>
				</div></div></div>';
				
	$output .= '<div class="modal fade" id="myModal1" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
					<div class="modal-dialog">
						<div class="modal-content">
							<div class="modal-header">
								<button type="button" class="close" data-dismiss="modal" onclick="remove_data()" aria-hidden="true">&times;</button>
								 <h4 class="modal-title">Add / Edit Contact Number</h4>
							</div>
							<div class="modal-body">
								<form method="POST" id="add_contact_number_form">
									<input type="hidden" id="sub_div_id" value="'.$sub_div_code.'">
									<input type="hidden" id="form_name_id" value="add_contact_number_form"> 
									<lable><b>Contact Number</b><font color="#FF0000">*</font></lable></br>
									<input type="input" name="contact_number" placeholder="Please enter Contact Number" id="contact_number_id" maxlength ="10" class="form-control" required="true"/></br>
									<span id="contact_span_id" style="display:none;color:red"> Contact Number field should numeric and not empty.</span>
								
								</form>
							</div>
							<div class="modal-footer">
								<button type="button" class="btn btn-default" data-dismiss="modal" onclick="remove_data()">Close</button>
								<button type="button" id="btnSubmit_contact" class="btn btn-primary">Save</button>
							</div>
						</div>
					</div>
				</div>';
	$output .= '<div class="modal fade" id="myModal2" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
					<div class="modal-dialog">
						<div class="modal-content">
							<div class="modal-header">
								<button type="button" class="close" data-dismiss="modal" onclick="remove_data()" aria-hidden="true">&times;</button>
								 <h4 class="modal-title">Add / Edit Email Id</h4>
							</div>
							<div class="modal-body">
								<form method="POST" id="add_email_form">
									<input type="hidden" id="sub_div_id" value="'.$sub_div_code.'">
									<input type="hidden" id="form_email_id" value="add_email_form"> 
									<lable><b>Email Id</b><font color="#FF0000">*</font></lable></br>
									<input type="input" name="email" placeholder="Please enter Email ID" id="email_id" class="form-control" required="true"/></br>
									<span id="email_span_id" style="display:none;color:red"> Email field should valid and not empty.</span>
								
								</form>
							</div>
							<div class="modal-footer">
								<button type="button" class="btn btn-default" data-dismiss="modal" onclick="remove_data()">Close</button>
								<button type="button" id="btnSubmit_email" data-dismiss="modal" class="btn btn-primary">Save</button>
							</div>
						</div>
					</div>
				</div>';						
	return $output; 
}
function get_info_form_ajax_callback($form, $form_state){
	$data = array();
	$data[] = ajax_command_replace('#loc_div', drupal_render($form['circular']['location']));
	$data[] = ajax_command_replace('#state_div', drupal_render($form['circular']['state']));
	$data[] = ajax_command_replace('#dist_div', drupal_render($form['circular']['dist']));
	$data[] = ajax_command_replace('#subdivision_div', drupal_render($form['circular']['subdivision']));
	$data[] = ajax_command_replace('#areatype_div', drupal_render($form['circular']['areatype']));
	$data[] = ajax_command_replace('#name_areatype_div', drupal_render($form['circular']['name_areatype']));
	$data[] = ajax_command_replace('#panchyat_ward_sector_div', drupal_render($form['circular']['panchyat_ward_sector']));
	$data[] = ajax_command_replace('#police_station_div', drupal_render($form['circular']['police_station']));
	$data[] = ajax_command_replace('#pincode_div', drupal_render($form['circular']['pincode']));
	return array('#type' => 'ajax', '#commands' => $data);
}

?>