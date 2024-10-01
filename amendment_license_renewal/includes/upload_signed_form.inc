<?php
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');

function upload_signed_form($form, &$form_state, $from_v, $renewallicenseammendmentid){  
	global $base_root, $base_path, $user;
	
	$user_id						= $user->uid;
	$contractor_id 					= $user->uid;
	 $from_v_no = encryption_decryption_fun('decrypt', $from_v);
	
	$renewal_license_ammendment_id 	= encryption_decryption_fun('decrypt', $renewallicenseammendmentid);
	
	$check_ammendment_table 		= 	ammendment_table($renewal_license_ammendment_id);	
	$details_ammendment 			=  	$check_ammendment_table->fetchAssoc();  
	
	
	$status	=   $details_ammendment['ammendment_status'];
	$reference_no =	$details_ammendment['reference_no'];	
	
	//if($from_v_no == 42516){ echo $status; die;}
	
		
	if(trim($status == 'P') || trim($status == 'AW')){ 
		
		$form['upload_signed_pdf']['header'] = array(
						'#markup' => t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">Download and Upload Application Form</div><div class="panel-body">'),
						'#type' => 'markup',
						);
																	
		$form['upload_signed_pdf']['ammend_id'] = array(
						'#type' => 'hidden',
						'#default_value' 	=>	!empty($renewallicenseammendmentid) ? $renewallicenseammendmentid : '',																		 					'#attributes' => array('readonly'=>'readonly')
						);
		$form['upload_signed_pdf']['reference_no'] = array(
						'#type' => 'hidden',
						'#default_value' 	=>	!empty($reference_no) ? encryption_decryption_fun('encrypt', $reference_no) : '',																		 					'#attributes' => array('readonly'=>'readonly')
						);
		$form['upload_signed_pdf']['from_v_no'] = array(
						'#type' => 'hidden',
						'#default_value' =>	!empty($from_v_no) ? $from_v : '',
						
						);																	
		$form['upload_signed_pdf']['row1'] =  array(
						'#markup' => '<div class="col-lg-12 col-md-12">',
						);
		$form['upload_signed_pdf']['download_link'] = array(
						'#markup' => l(t('Download Application Form'),'amendment_license_renewal/create_formiv_vii/'.$from_v.'/'.$renewallicenseammendmentid.'/Application Form', array('html' => TRUE,'attributes'=> array('target'=>'_blank' , 'class'=>array('link-btn')))),
						'#type' => 'markup',
						);	
																			
																																
		$form['upload_signed_pdf']['row2'] =  array(
						'#type' => 'markup',
						'#markup' => '</div><div class="col-lg-6 col-md-12">',
						);																	  

		$form['upload_signed_pdf']['form_vii']	= array(
						'#type' 				=> 'managed_file',
						'#title'				=> '1. Upload Signed Application Form',
						'#upload_validators' 	=> array('file_validate_extensions' => array('pdf')),
						'#upload_location' 		=> 'public://upload/Filluped_and_Signed_FORM_VII',
						'#process' 				=> array('import_my_file_element_process'),
						'#attributes' 			=> array('id' => '','autocomplete' => 'off','class'=>array(''),'file_validate_size' => array(50*1024*1024)),													
						'#prefix' 				=> '<label class="input-file">',
						'#suffix' 				=> '</label>',
						'#required'				=> 'TRUE',
						);
																			
																			
		$form['upload_signed_pdf']['row3']	=  array(
						'#type' => 'markup',
						'#markup' => '</div>',
						);
		
		$form['upload_signed_pdf']['row6'] =  array(
						'#markup' => '<div class="col-lg-12 col-md-12"><span style="color:red;"><b><u>NOTE</u> :-&nbsp;  Application Form </b> </b>should be duly signed and uploaded for successfull submission of the Application.</b>.</span></div>',
						);																	
		
		
		$form['upload_signed_pdf']['submit']	= array(
						'#prefix' 			=> '</div></div>',  
						'#suffix' 			=> '</div>',  
						'#type' 				=> 'submit',
						'#attributes' 		=> array('class' => array('btn btn-primary pull-right')),
						'#value' 				=> 'SUBMIT'
						);
	}else {
		// if($from_v_no == 1222){echo 99229; echo $status.$renewal_license_ammendment_id; die;}
		
		/*if($user_id == 13602){
			echo $status.'---'.$renewal_license_ammendment_id; echo 'checking'; die;
		}*/
		
		if(!empty($renewal_license_ammendment_id) && $renewal_license_ammendment_id!=''){								
			$get_uploaded_document = get_uploaded_document($renewal_license_ammendment_id);
				$form['showmsg'] = array('#markup' => ''.$get_uploaded_document.''); 
			}
	}
	return $form;	
}
	
	
 function upload_signed_form_submit($form, &$form_state){
		
	global $base_root, $base_path, $user;
	
	if(!empty($user->uid )){
		$val			=	$form_state['values'];
		$ammend_id		=	$val['ammend_id'];
		$ammendid 		= 	encryption_decryption_fun('decrypt', $ammend_id); 
		$reference_no	=	encryption_decryption_fun('decrypt', $val['reference_no']); 
		$from_v			=	encryption_decryption_fun('decrypt', $val['from_v_no']);
		
		$last_remark_activity 			= last_remark_activity($reference_no);
		
		$remark_field_title 			=	$last_remark_activity['remark_field_title'];
		$not_amendment_check_field 		=	$last_remark_activity['not_amendment_check_field'];
		$amendment_check_field 			=	$last_remark_activity['amendment_check_field'];
		

		$userid 			= 	$user->uid;
		$contractor_id 		= 	$user->uid;
		$val				=	$form_state['values'];
		$form_vii 			= 	$val['form_vii'];

		$form_vii_upload 			= file_load($form_vii );
		$form_vii_upload->status 	= FILE_STATUS_PERMANENT;
		file_save($form_vii_upload);
		$form_vii_fid		= $form_vii_upload->fid;
	
		$upload_file	=	array('upload_singed_from' =>$form_vii_fid ,'ammendment_status' => 'U');
	
		$license_apply_renweal 	= 	db_update('l_amendment_of_contract_labour_license');
		$license_apply_renweal	->	fields($upload_file);
		$license_apply_renweal	->	condition('id',$ammendid);
		$license_apply_renweal	->	execute();		
		
		$renewal_remark_license = array(
										'remark_text'   		=> 'Signed Amended Licence Uploaded', 
										'remark_by'   			=> $contractor_id, 
										'remark_to' 			=> $from_v,
										'remark_type' 			=> 'U',
										'remark_date'  			=> time(),
										'particular_id'			=> $reference_no,
										'remark_by_role' 		=> '8',
										'flag'					=> 'A',
										'not_amendment_check_field'	=>	$not_amendment_check_field,
										'amendment_check_field' 	=>	$amendment_check_field,
										'remark_field_title'  		=>	$remark_field_title,
									
								);
								
								
	   
 		db_insert('l_remark_license')->fields($renewal_remark_license)->execute();	
		
		contractor_activity_tag_insert($from_v,'A',$ammendid,'U');			
		drupal_set_message("Application Successfully Submitted.");
	}else{
		drupal_goto('home');
	}
		
	
	
	}
function get_uploaded_document($license_renewal_id){
	
	$check_ammendment_table = 	check_ammendment_table($license_renewal_id);	
	$details_ammendment =  	$check_ammendment_table->fetchAssoc();
	$upload_singed_from	=   $details_ammendment['upload_singed_from'];
	if(!empty($upload_singed_from)){		
		$upload_signed_form_id	=	$upload_singed_from;
	}
		
	
	if(empty($upload_singed_from)){
		$output='';
	}else{
		$upload_signed_form_details = db_select('file_managed','fm');
		$upload_signed_form_details->fields('fm',array());
		$upload_signed_form_details->condition('fid',$upload_singed_from);
		
		$upload_signed_form_result = $upload_signed_form_details->execute()->fetchAssoc();
								
		if(!empty($upload_signed_form_result)){ 
				$url						= 	explode('//',$upload_signed_form_result['uri']);
				$upload_signed_form_url		= 	$url[1];
				$upload_signed_file			= 	$upload_signed_form_result['filename'];
		}
		

						
		
		$output .= '
				<style type="text/css">
				.btn {
				  background: #3498db;
				  background-image: -webkit-linear-gradient(top, #3498db, #2980b9);
				  background-image: -moz-linear-gradient(top, #3498db, #2980b9);
				  background-image: -ms-linear-gradient(top, #3498db, #2980b9);
				  background-image: -o-linear-gradient(top, #3498db, #2980b9);
				  background-image: linear-gradient(to bottom, #3498db, #2980b9);
				  -webkit-border-radius: 28;
				  -moz-border-radius: 28;
				  border-radius: 24px;
				  font-family: Arial;
				  color: #ffffff;
				  font-size: 10px;
				  padding: 10px 20px 10px 20px;
				  text-decoration: none;
				}
				
				.btn:hover {
				  background: #3cb0fd;
				  background-image: -webkit-linear-gradient(top, #3cb0fd, #3498db);
				  background-image: -moz-linear-gradient(top, #3cb0fd, #3498db);
				  background-image: -ms-linear-gradient(top, #3cb0fd, #3498db);
				  background-image: -o-linear-gradient(top, #3cb0fd, #3498db);
				  background-image: linear-gradient(to bottom, #3cb0fd, #3498db);
				  text-decoration: none;
				}
				</style>
				
				
				<table width="100%" border="0" class="application_view">
				
					<tr>
					<td>Duly signed and uploaded Form-VII<td>
					<td></td>
						
  					</tr>
					<tr>
					<td>'.$upload_signed_file.'<a href="'.$GLOBALS['base_url'].'/sites/default/files/'.$upload_signed_form_url.'" target="_blank"><img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/pdf.png"></a><td>
					<td></td>
						
  					</tr>
  					
 					
					
				</table> <br>';
	}
	return $output;
}
	
?>