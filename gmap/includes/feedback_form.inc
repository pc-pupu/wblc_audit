<?php
function get_feedback_form($form, &$form_state) {
	drupal_add_css(drupal_get_path('module', 'gmap') . '/css/back_to_top.css');	
	 
	$options_first = common_custom_user_district_list_new() + array("ALL" => "ALL");
	
	 $value_dropdown_first = isset($form_state['values']['disname']) ? $form_state['values']['disname'] :key($options_first);
	
	$form ['feedbackfrm']['name']=array(
			'#prefix' 			=> '<div class="feedback-bg">
										<div class="row">
											<div class="form-custom col-md-4 form-btm-pad-15"><label class="input">Name <span class="star" style="color:red;">*</span></label>
												<div class="cols-sm-10">
													<div class="input-group">
														<span class="input-group-addon"><i class="fa fa-user" aria-hidden="true"></i></span>',
			'#suffix' 			=> '</div></div></div>',
			'#type'				=> 'textfield',
			//'#title'			=> t('Name'),							
			'#required' 		=> TRUE,
			'#element_validate' => array('FEEDBACK_NAME_VALIDATION'),
			'#attributes' 		=> array('class'=>array('form-control'),'placeholder'=>'Enter your name'),
	);
						
	$form ['feedbackfrm']['email']=array(
								'#type'				=> 'textfield',
								'#prefix' 			=> '<div class="form-custom col-md-4 form-btm-pad-15"><label class="input">Email <span class="star" style="color:red;">*</span></label><div class="cols-sm-10"><div class="input-group">
														<span class="input-group-addon"><i class="fa fa-envelope fa" aria-hidden="true"></i></span>',
	    						'#suffix' 			=> '</div></div></div>',
								//'#title'			=> t('E-mail'),
								'#required'			=> TRUE,
								'#element_validate' => array('FEEDBACK_EMAIL_VALIDATE'),
								'#attributes' 		=> array('class'=>array('form-control'), 'placeholder'=>'Enter email address'),
								);
						
	$form ['feedbackfrm']['phone']=array(
								'#prefix' 			=> '<div class="form-custom col-md-4 form-btm-pad-15"><label class="input">Contact Number <span class="star" style="color:red;">*</span></label><div class="cols-sm-10"><div class="input-group">
													<span class="input-group-addon"><i class="fa fa-phone fa" aria-hidden="true"></i></span>',
	    						'#suffix' 			=> '</div></div></div>',		
								'#type'				=> 'textfield',
								//'#title'			=> t('Contact Number'),
								'#required'			=> TRUE,
								'#attributes'		=> array('autocomplete' => 'off', 'id' => 'phoneId', 'placeholder'=>'Enter 10 digits mobile number', 'maxlength' => 10,'class' => array('form-control')),
								'#element_validate' => array('FEEDBACK_PHONE_VALIDATE'),
						
								);
				  
	$form ['feedbackfrm']['disname']=array(
								'#type'			 	=> 'select',
								'#prefix' 		 	=> '<div class="form-custom col-md-4 form-btm-pad-15"><label class="input">District <span class="star" style="color:red;">*</span></label><div class="cols-sm-10"><div class="input-group">
														<span class="input-group-addon"><i class="fa fa-globe fa" aria-hidden="true"></i></span>',
	    						'#suffix' 		 	=> '</div></div></div>',
								'#options'		 	=> $options_first,
								'#default_value' 	=> $value_dropdown_first,
								//'#title'		 	=> t('District'),
								'#required'		 	=> TRUE,
								'#attributes' 	 	=> array('class'=>array('form-control')),
								'#ajax' 		 	=> array(
													'event'     => 'change',
													'callback'  => 'get_feedback_info_form_ajax_callback',
													),
					
								);
				
	$value_loc_e_dist = isset($form_state['values']['disname']) ? $form_state['values']['disname'] : '';
	
	if($value_loc_e_dist != "" || !empty($value_loc_e_dist) || is_numeric($value_loc_e_dist)){
								
	$form ['feedbackfrm']['subdiv']=array(
							  '#type'				=> 'select',
							  '#options' 			=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist),
							  //'#title'			=> t('Sub-Division'),
							  '#required'			=> TRUE,
							  '#prefix' 			=> '<div id="subdvsn_replace" class="form-custom col-md-4 form-btm-pad-15"><label class="input">Sub-Division <span class="star" style="color:red;">*</span></label><div class="cols-sm-10"><div class="input-group">
													<span class="input-group-addon"><i class="fa fa-phone fa" aria-hidden="true"></i></span>',
							  '#suffix' 			=> '</div></div></div>',
							  '#attributes' 		=> array('class'=>array('form-control')),
							  '#ajax' 		 		=> array(
													  'event'     => 'change',
													  'callback'  => 'get_feedback_info_form_ajax_callback',
													  ),
								  
							   );
			
	}else {
		
	$form ['feedbackfrm']['subdiv']=array(
							  '#type'				=> 'select',
							  '#options' 			=> common_custom_user_subdivisionlist_list_new($value_loc_e_dist),
							  //'#title'			=> t('Sub-Division'),
							  '#required'			=> TRUE,
							  '#prefix' 			=> '<div id="subdvsn_replace" class="form-custom col-md-4 form-btm-pad-15"><label class="input">Sub-Division <span class="star" style="color:red;">*</span></label><div class="cols-sm-10"><div class="input-group">
													<span class="input-group-addon"><i class="fa fa-minus-square fa" aria-hidden="true"></i></span>',
							  '#suffix' 			=> '</div></div></div>',
							  '#attributes' 		=> array('class'=>array('form-control')),
							  '#ajax' 				=> array(
													  'event'     => 'change',
													  'callback'  => 'get_feedback_info_form_ajax_callback',
													  ),
						      );
	}
	
	$feedbackOptions =	array('S'=>t('Suggestion'), 'C'=>t('Complaint'), 'O'=>t('Others'));
	
	$form['feedbackfrm']['feedback_type'] 			= 	array(
							'#prefix'				=> '<div class="form-custom col-md-4 form-btm-pad-15 custom-radio-box">',
							'#suffix'				=> '</div>',
							'#type' 				=> 'radios',
							'#title' 				=> t('Feedback Type'),
							//'#attributes' 	 		=> array('class'=>array('form-control')),
							'#options' 				=> $feedbackOptions,
							'#required' 			=> TRUE
						);
			  
	$form ['feedbackfrm']['cat_id']=array(
							'#prefix' 				=> '<div class="form-custom col-md-4 form-btm-pad-15"><label class="input">Category <span class="star" style="color:red;">*</span></label><div class="cols-sm-10"><div class="input-group">
													<span class="input-group-addon"><i class="fa fa-list-alt fa" aria-hidden="true"></i></span>',
							'#suffix' 				=> '</div></div></div>',
							'#type'		  			=> 'select',
							'#options'    			=> get_all_category_type(),
							//'#title'      		=> t('Category'),
							'#required' 			=> TRUE,
							'#attributes' 			=> array('class'=>array('form-control')),
						);	
	
	$form ['feedbackfrm']['act_id']=array(
						  '#prefix' 				=> '<div class="form-custom col-md-4 form-btm-pad-15"><label class="input">Service  <span class="star" style="color:red;">*</span></label><div class="cols-sm-10"><div class="input-group">
													<span class="input-group-addon"><i class="fa fa-gavel fa" aria-hidden="true"></i></span>',
						  '#suffix' 				=> '</div></div></div>',
						  '#type'		  			=> 'select',
						  '#options' 				=> labour_inspection_repo_option(),
						  //'#title'				=> t('Act'),
						  '#required'				=> TRUE,
						  '#attributes' 			=> array('class'=>array('form-control')),
					  );	
  
	$form ['feedbackfrm']['subject']=array(
						  '#prefix' 				=> '<div class="form-custom col-md-4 form-btm-pad-15"><label class="input">Subject <span class="star" style="color:red;">*</span></label><div class="cols-sm-10"><div class="input-group">
													<span class="input-group-addon"><i class="fa fa-book" aria-hidden="true"></i></span>',
						  '#suffix' 				=> '</div></div></div>',
						  '#type'		 			=>'textfield',
						  //'#title'	 			=>t('Subject'),
						  '#required'	 			=>TRUE,
						  '#element_validate' 		=> array('FEEDBACK_TEXTBOX_TEXT_VALIDATE'),
						  '#attributes'  			=> array('class'=>array('form-control'),'placeholder'=>'Topic title.'),
					  );	
	

	$form ['feedbackfrm']['comment']=array(
						  '#prefix' 				=> '<div class="form-custom col-md-12 form-btm-pad-15 custom-form-item-pad-remove"><label class="input">Comment <span class="star" style="color:red;">*</span></label>',
						  '#suffix' 				=> '</div>',
						  '#type'		 			=> 'textarea',
						  //'#title'	 			=> t('Comment'),
						  '#required'	 			=> TRUE,
						  '#attributes'				=> array('class'=>array('form-control'),'placeholder'=>'Write Comment here.'),
						);
													
	
	$form['feedbackfrm']['my_captcha_element'] = array(
		'#type' 	=> 'captcha',
		'#prefix' 	=> '<div class="form-custom col-md-12 custom-cap-design">',
		'#suffix' 	=>'</div>'
		
	);
	
	/*
	$form['feedbackfrm']['captcha_response'] = array(
		'#type' 	=> 'textfield',
		'#prefix' 	=> '<div class="form-custom col-md-4 form-btm-pad-15"><div class="cols-sm-10"><div class="input-group">',
	    '#suffix' 	=> '</div></div></div>',
		'#attributes'=> array('class'=>array('form-control')),
		'#id' => 'edit-captcha-response'
	);
	*/
	
	$form ['feedbackfrm']['feedback_save']			=array(
						  '#prefix' 				=> '<div class="form-custom col-md-12 form-btm-pad-15">',
						  '#type'					=>'submit',
						  '#value'					=>'Submit',						
						);
						
	$form ['feedbackfrm']['feedback_reset']			=array(
						  '#type'					=>'submit',
						  '#value'					=>'Reset',
						  '#attributes'				=> array('class'=>array('pull-right')),
						  '#prefix'					=>'',
						  '#suffix' 				=>'</div></div></div>',
						);
	
	return $form;					
	
}

function get_all_category_type(){
	
	$options 				= array(
									1 => t('Registration'), 
									2 => t('License'), 
									3 => t('Renewal'),
									4 => t('Amendment'),
									5 => t('Others'),
									6 => t('Inspection'),
									7 => t('Register Report on visit By Inspector'),
									// 8 			=> t('Search Info'),
									// 9  			=> t('Reports'), 
									// 10 			=> t('Trade Union')
									);
	return $options;
}

function FEEDBACK_EMAIL_VALIDATE($element, &$form_state){
	 $email	= trim($element['#value']); 
	 if( $email == "" ){
		form_error($element, t('Email field can not be empty.')); 
	 }
	 if (!filter_var($email, FILTER_VALIDATE_EMAIL)) {
		form_error($element, t('Email format is invalid'));
	 }
}

function FEEDBACK_NAME_VALIDATION($element, &$form_state){
	$txtValue 			=	trim($form_state['values']['name']);
	$match_pattern 	= '/[][{}()*+?.\\^$|]/';
	if(strlen($txtValue)>0){
		if(preg_match($match_pattern, $txtValue)) {
			form_error($element, t('Name should only contain letters, space & dot(.)'));
		}
	} else {
		form_error($element, t('Name field cannot be blank.'));
	}
	
}
function FEEDBACK_PHONE_VALIDATE($element, &$form_state){
		$mob	= "/^[1-9]{1}[0-9]{9}$/";
		if(strlen(trim($form_state['values']['phone']))>0){
			  if(strlen(trim($form_state['values']['phone'])) < 10 ){
				 form_error($element, t('Contact Number must be of 10 digits.'));
			  }
			  if(! is_numeric(trim($form_state['values']['phone']))){
				 form_error($element, t('Contact Number should be numeric.'));
			  }
			  if(!preg_match($mob, $form_state['values']['phone'])){ 
				form_error($element, t('Contact Number should be valid.'));
			  }
		 }else{
			form_error($element, t('Contact Number field cannot be blank.'));
		 }
}

function FEEDBACK_TEXTBOX_TEXT_VALIDATE($element, &$form_state){
		$txtValue		=	trim($element['#value']); 
		$match_pattern 	= 	'/^[ A-Za-z0-9,.\/\(\) ]+$/';
		if( $txtValue == "" ){
			form_error($element, t($element['#title'].'Subject cannot be empty.'));
		}
		if(!preg_match($match_pattern, $txtValue)) {
			form_error($element, t($element['#title'].'Subject should be text only.'));
		}
		//$form_state['rebuild']	=	true;
	}

function get_feedback_info_form_ajax_callback($form, $form_state) {
		$commands = array();
		$commands[] = ajax_command_replace('#subdvsn_replace', drupal_render($form ['feedbackfrm']['subdiv']));
		
  		return array('#type' => 'ajax', '#commands' => $commands);
}

/*
function get_feedback_form_validate($form, &$form_state) {
	
}*/

function get_feedback_form_submit($form , &$form_state){	
	global $user;
	$val	= $form_state['values'];
	
	if(!empty($user->uid)){ $user_id = $user->uid;}else{ $user_id = 0;} 
	
	
	$fields_Array	=	array(								
							'submitted_date'	=> time(),
							'name'				=> strip_tags($val['name']),
							'email'				=> $val['email'],
							'contact_no'  		=> $val['phone'],
							'district'  		=> $val['disname'],
							'subdivision' 		=> $val['subdiv'],
							'category' 			=> $val['cat_id'], 
							'act' 				=> $val['act_id'],	
							'subject' 			=> strip_tags($val['subject']), 
							'comment' 			=> strip_tags($val['comment']),
							'feedback_type'		=> $val['feedback_type'],
							'user_id'	  		=> $user_id,
							'is_active' 		=> 'Y',
							'is_read'   		=> 'N',
							'deleted'   		=> 'N',
							'is_published'   	=> 'N'
						   );					
	
	// print_r($fields_Array); die;
	
	$query = db_insert('l_feedback');
	$query->fields($fields_Array); 	
	$query->execute();
	
	//$message = 'Your Feedback Submitted Successfully.';
	drupal_set_message(t('Your Feedback has been Submitted Successfully.'));
}

function get_feedback_form_validate($form, &$form_state){
	$val 			=	$form_state['values']; 
	$markup_hidden	=	base64_decode(base64_decode(base64_decode(base64_decode($val['markup_hidden']))));
	$entered_code	=	$val['entered_code'];
	if(!empty($markup_hidden)){
		if($markup_hidden != $entered_code){
			form_error($form['entered_code'], t('Captcha does not match !!'));
		}
	}else{
		form_error($form['entered_code'], t('There is a problem with captcha.Please reload the page or click refresh.'));
	}
}


