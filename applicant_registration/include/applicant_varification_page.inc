<?php
function applicant_varify($form, &$form_state,$email=''){
	global $base_root, $base_path, $user;
	
	$fetch_common_det = db_query("select email, user_id from l_common_application_master where user_id=:userId",array(':userId' => $user->uid));
			  
	if( $fetch_common_det->rowCount() > 0 ){
		$obj = $fetch_common_det->fetchAssoc();
		$email = $obj['email'];	
	
		$form['email'] = array(
						'#prefix' 			=> '<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">VARIFY YOUR EMAIL</div><div class="panel-body"><div class="col-md-4"><label class="input">',
						'#suffix'			=>'</label></div>',					
						'#default_value'	=> $email,
						'#type' 			=> 'textfield',
						'#attributes' 		=> array('placeholder' => 'Enter your Email Id'),
						'#element_validate' => array('EMAIL_VALIDATE_PAGE')
					);
																
		$form['submit'] = array(			 
				  '#value' 		=> t('Verify & Update'),
				  '#prefix' 	=> '<div class="row">', 
				  '#suffix' 	=> '</div>',
				  '#type' 		=> 'submit',
				  '#attributes' => array('class' => array('btn2 btn-primary form-submit')),
			);
			
		$form['header_markup'] = 	array(
				'#prefix' 	=> '<div class="col-md-12">',
				'#suffix' 	=> '</div>',
				'#markup'	=> '<font color="red"><b>Note:</b> After click on varify button. Please check your respective email account.</font></div></div></div>'
			);
	}else{
		drupal_goto('applicant-login');
	}
																
	return $form; 
}
	
	
function applicant_varify_submit($form, &$form_state){
	global $base_root, $user;
	
	$euser_id = encryption_decryption_fun('encrypt', $user->uid);
	
	$val = $form_state['values'];
	
	$eemail = encryption_decryption_fun('encrypt', $val['email']);
	
	date_default_timezone_set("Asia/Kolkata");
	date_default_timezone_get();
	
	$ctime = encryption_decryption_fun('encrypt', time()+2*60*60);
		
	$fetch_common_det = db_query("select user_id from l_common_application_master where user_id=:user_id", array(':user_id' => $user->uid));
	
	if( $fetch_common_det->rowCount() > 0 ){
		
		$obj = $fetch_common_det->fetchAssoc();
		
		$link = l(t('Link'), $base_root.'/applicant-varification-link/'.$euser_id.'/'.$eemail.'/'.$ctime, array('html'=>TRUE,'attributes'=>array('target' => '_blank','class'=>'pull-right link')));
		
		$mailto  = trim($val['email']);
		
		$subject = 'Varification Email From Labour Commissionerate, Govt. Of West Bengal';
	
		$message = 'Hello, <br>Cleck here to varify your Email '.$link.'<br><br><br>Thank you.';

		send_mail_for_user_alert($mailto, $subject, $message);
		// send_mail_for_user_alert('aritrachowdhury26@gmail.com', $subject,$message, 'Varification');
	}
}
	
	
function applicant_varify_link($en_user_id='', $e_email='', $exptime = ''){

	global $base_root, $base_path, $user;
	
	$user_id = encryption_decryption_fun( 'decrypt', $en_user_id);
	
	$email = encryption_decryption_fun( 'decrypt', $e_email);
	
	$exptime = encryption_decryption_fun( 'decrypt', $exptime);
	
	if($user_id != $user->uid){
		echo 'Invalid User. Please click <strong>'.l('here','user/logout').'</strong> to logout user.'; die;
	}
	
	if($exptime > time() && $user_id == $user->uid){
	
		$fieldsInfo = array('is_varified'=> 1, 'email' => $email, 'updated' => time());
		
		$rs = db_update('l_common_application_master')->fields($fieldsInfo)->condition('user_id', $user_id)->execute();		
		
		if (!empty($user->uid)){		
			drupal_goto('applicant-dashboard');	
		}else{
			$link=l(t('here'),'/applicant-login', array('html'=>TRUE,'attributes'=>array('target'=>'_blank','class'=>'pull-right link')));
			echo '<table align="center" width="60%"><tr><td><font color="#FF0000">Your email is now varified. Please click <strong>'.$link.'</strong> to login</font></td></tr></table>'; die;
		}
	}else{
		echo 'Invalid/Expired link.'; die;
	}
}
	
function EMAIL_VALIDATE_PAGE($element, &$form_state){
	 $email =trim($form_state['values']['email']);
	 if( $email == "" ){
		form_error($element, t($element['#title'].' cannot be empty.')); 
	 }
	 if (!filter_var($email, FILTER_VALIDATE_EMAIL)) {
		form_error($element, t($element['#title'].' format invalid.'));
	 }
	 
	 /*$validateEmail					=	db_query("select mail from users where mail=:mailval", array(':mailval'=> $form_state['values']['email']));
	 if( $validateEmail->rowCount() > 0 ){
		form_error($element, t($element['#title'].' already exists.'));
		$form_state['rebuild'] = TRUE;
	}*/
	//$form_state['rebuild']	=	true;
}
	
	?>