<?php

function remark_license_alc_callback($ajax, $wildcard_1='', $wildcard_2=''){
	//echo $ajax."<br>".$wildcard_1."<br>".$wildcard_2."<br>".$wildcard_3; exit;
	if ( $ajax == 'ajax' ) {
		ctools_include('ajax');
		ctools_include('modal');
		$textmsg = '';	
		if($wildcard_1 == 'A') $textmsg = 'Remark For Approved For Submit Fees';
		if($wildcard_1 == 'B') $textmsg = 'Remark For Back to Correction to Applicant';
    	if($wildcard_1 == 'BI') $textmsg = 'Remark For Back to Inspector';
		if($wildcard_1 == 'FW') $textmsg = 'Remark For froward to ALC'; 
		if($wildcard_1 == 'C') $textmsg = 'Remark For Call To Applicant';
 		if($wildcard_1 == 'I') $textmsg = 'Remark For Isuess Licence To Applicant';
		if($wildcard_1 == 'R') $textmsg = 'Remark For Reject Licence';

		$form_state = array(
		'ajax' => TRUE,
		'title' => t($textmsg),
		'wildcard' => array('param1' => $wildcard_1, 'param2' => $wildcard_2),
	);
		$output 	= ctools_modal_form_wrapper('remark_license_alc', $form_state);

		if (!empty($form_state['ajax_commands'])) {

			$output = $form_state['ajax_commands'];
		}

		print ajax_render($output);
		drupal_exit();
	}else {
		return drupal_get_form('remark_license_alc');
	}
}
function remark_license_alc($form,&$form_state){  
	drupal_add_library('system','ui.datepicker');
	
	global $user;	
	
	$form['remark_license'] = array(
          '#type' => 'fieldset',
		  '#prefix' => '<div class="remarks-form">',
          '#suffix' => '</div>',
    );	
	
    drupal_add_css(drupal_get_path('module', 'alc').'/css/modal-window.css');
	

	
	$form['remark_license']['check_data_text'] =  array(
          '#title' => t('For Correction Field'),
          '#type' => 'textfield',		   	     
		  '#attributes' => array('id' => 'check_data_text_id'),		 
    );
	
	
	$form['remark_license']['remark_text'] = array(
          '#title' => t('Remarks'),
          '#type' => 'textarea',
		  '#cols' => 35, 
          '#rows' => 5, 
		  '#attributes' => array('placeholder' => 'Enter your massage'),
		  '#required' =>  TRUE,
    );

if($form_state['wildcard']['param1']=='C'){
		
		drupal_add_css(drupal_get_path('module', 'alc').'/js/jquery.datetimepicker.css');
	    drupal_add_js(drupal_get_path('module', 'alc').'/js/jquery.js');
        drupal_add_js(drupal_get_path('module', 'alc').'/js/jquery.datetimepicker.js');	
        drupal_add_js(drupal_get_path('module', 'alc').'/js/custom_datepicker.js');
	

		
		$form['remark_license']['calltime'] = array(
							'#title'=>'<span class="">Calling Date Time</span>',
							'#size'=>t('20'),	
							'#type' => 'textfield',
							'#attributes' => array('id' => 'calltime', 'readonly'=>'readonly', 'placeholder' => 'Enter your calling time', 'onclick' => 'showdate()'),
							'#required' =>  TRUE,
			);
    }
	
	$form['remark_license']['remark_by'] = array(
		'#type' => 'hidden',		
		'#value' => $user->uid
	);
	
	$form['remark_license']['status'] = array(
		'#type' => 'hidden',		
		'#value' => $form_state['wildcard']['param1']
	);
	
	$form['remark_license']['remark_to'] = array(
		'#type' => 'hidden',		
		'#value' =>  $form_state['wildcard']['param2']
	);	
	
	
	
	$form['submit'] = array(
		'#type' => 'submit',		
		'#value' => 'SUBMIT',
		'#size' => 20
	);
	return $form;
}
function remark_license_alc_validate($form,&$form_state){
	$val	=	$form_state['values'];
	if(isset($form['remark_license']) && !empty($form['remark_license'])) {
		$form_field_array1 = $form['remark_license'];
		foreach($form_field_array1 as $element){
			if(is_array($element)){
				if(isset($element['#type']) and ($element['#type'] == 'textarea')){
					chkbadchar($element['#value'], $element['#title'], $element['#name']);
				}
			}
		}
	}
}
function remark_license_alc_submit($form,&$form_state)
 {
	 global $user;
	 $uid 					= $user->uid;
	 $license_issued		= 0;
	 $val   				= $form_state['values'];
	 $status  				= $val['status'];
	 $remark_text 			= $val['remark_text'];
     $calltime 				= $val['calltime'];
	 $remark_by  			= $val['remark_by'];
	 $remark_to  			= $val['remark_to'];
	 $remark_field_title	= $val['check_data'];
	
	 if(!empty($calltime)){
		 $remark_text=$remark_text.' on '.$calltime;
	 }
	 else{
		 $remark_text=$remark_text;
	 }
	 if($status=='I'){
		
		$license_issued			 =	1;
		 
		$query_Is_license_code 		 = 	db_select('l_contractor_license_application', 'lca');
		$query_Is_license_code		->	fields('lca',array('contractor_license_number'));
		$query_Is_license_code		->	condition('lca.contractor_license_number','NULL','!=')
				 					->	orderBy('lca.id', 'DESC')
				    				->	range(0,1);
	  $query_Is_license_code_result  = 	$query_Is_license_code->execute()->fetchObject()->contractor_license_number;
	  
	  $query_contractor_add 		 = 	db_select('l_particular_of_contract_labour', 'pcl');
	  $query_contractor_add			->	fields('pcl',array('contractor_subdivision','contractor_name_areatype','contractor_user_id'));
	  $query_contractor_add			->  condition('pcl.serial_no_from_v',$remark_to);
 	  $query_contractor_add_list  	= 	$query_contractor_add->execute()->fetchAssoc();
	  $contractor_subdv				=	$query_contractor_add_list['contractor_subdivision'];
	  $contractor_areacod			=	$query_contractor_add_list['contractor_name_areatype'];
	  $contractor_user_id			=	$query_contractor_add_list['contractor_user_id'];
	  
	  $get_sub_ShortName  			=   custom_user_short_name_fun($contractor_subdv);  // ---------come from custom_user module 
	  $get_con_area_code 			=   substr($contractor_areacod, -2); 
	  
	  if(empty($query_Is_license_code)){
					$license_number			=	$get_sub_ShortName.$get_con_area_code.'/'.'CLL'.'000001';
	  }else{
				    $license_query			=	db_query("select  max (cast(substr(contractor_license_number,12,6) as integer)) as serial_license_no  from l_contractor_license_application");
			 		$x_license				=	$license_query->fetchAssoc();
					$license_code 			=	$x_license['serial_license_no'];
					$license_code_next		= 	$license_code+1;
 			
				 	$license_code_first		=	$get_sub_ShortName.$get_con_area_code.'/'.'CLL'.'/';
				
				 	$license_code_second	=	str_pad($license_code_next, 6, "0", STR_PAD_LEFT);
				 	$license_number			=	$license_code_first.$license_code_second;
				}
		$license_date = date('m/d/Y h:i:s a', time());
		
		$update_user_id   				=  	array('contractor_user_id'=>$contractor_user_id);
	  	$update_contractor_user_id  	=  	db_update('l_contractor_info_master');
	  	$update_contractor_user_id		->	fields($update_user_id);
	  	$update_contractor_user_id		->	condition('id',$remark_to);
	  	$update_contractor_user_id		->	execute() ;
		
		$time = strtotime($license_date);
			
		$next_renweal_date = date("Y-m-d", strtotime("+364 days", $time));
		
	 }
			  
	  $remark_details  =  array(
				'remark_text'   		=> $remark_text, 
				'remark_by'   			=> $remark_by, 
				'remark_to' 			=> $remark_to,
				'remark_type' 			=> $status,
				'remark_date'  			=> time(),
				'remark_field_title' 	=> $remark_field_title,
				'remark_by_role' 		=> '4',
			
	);

	 db_insert('l_remark_license')->fields($remark_details)->execute();
	 
	 $update_status   	=  	array('status'=>$status,'modification_date' =>date("Y-m-d"),'remark_by_userid'=> $uid,'contractor_license_number'=>$license_number,'license_issued' =>$license_issued,'license_date'=> $license_date,'next_renweal_date'=> $next_renweal_date);
	  $apply_status  	=  	db_update('l_contractor_license_application');
	  $apply_status		->	fields($update_status);
	  $apply_status		->	condition('serial_no_from_v',$remark_to);
	  $apply_status		->	execute() ;
	
	 $message = "OK"; 
	 
	 $action = 'encrypt';
 	$serial_number=encryption_decryption_fun($action, $remark_to);//----miscellaneous module
	
	drupal_set_message($message);  
	$form_state['input']['remarks'] 	= '';  
	$form_state['rebuild'] 				= TRUE;	
	$redirect_url 						= 	$base_root.$base_path.'alc-view-license/'.$serial_number;
	$commands[] 						= 	ctools_ajax_command_redirect($redirect_url);
	print ajax_render($commands);
	exit;
		
		
}