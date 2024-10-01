<?php 
function register_record_repository_confirm($form, &$form_state){
	global $user;
	$rrquery = db_select('l_register_record_repository','rr');
	$rrquery->fields('rr',array('id'));
	$rrquery->condition('rr.user_id', $user->uid);
	$rrquery = $rrquery->execute();	
	if($rrquery->rowCount() > 0 ){
		drupal_goto('register-record-repository-form');		
	}
	$form['register_record']['header']	= array(
						'#markup' =>t('<div class="content">
							<div class="panel panel-default panel-form">
								<div class="panel-heading">Register Record Repository</div>
								<div class="panel-body">'),
								);
																						
	$form['register_record']['confirm_msg'] = array(
								'#markup' => '<div class="col-md-12">test</div>',							
								'#type' => 'markup'
							 );
	$form['register_record']['submit'] = array(																					 
							  '#prefix' => '</div></div></div>',  
							  '#type' => 'submit',
							  '#attributes' => array('class' => array('btn btn-primary form-submit')),
							  '#value' => 'Confirm'
							);
	return $form;
}
function register_record_repository_confirm_submit($form, &$form_state){
	drupal_goto('register-record-repository-form');		
}
function register_record_form($form, &$form_state) {
	
	global $base_root, $base_path, $user;
	
	drupal_add_js(drupal_get_path('module', 'applicant_forms').'/js/custom.js');	
	
	$user_id= $user->uid;
		
	$form['register_record']['header']	= array(
										'#markup' =>t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">Register Record Repository Form</div><div class="panel-body">'),
										);
																						
	$form['register_record']['r_title'] = array(
						'#prefix' => '<div class="row"><div class="col-md-12">',
						'#suffix' => '</div>',
						'#title'=>  'Title ',
						'#attributes' => array('id' => '','autocomplete' => 'off','class'=>array('form-control')),
						'#type' => 'textfield',
						'#required' => TRUE,
						);
										
	$form['register_record']['r_desc'] 	= array(
					  '#prefix' =>  '<div class="col-md-12">',
					  '#suffix' =>  '</div></div>',
					  '#title'=> 'Description',
					  '#attributes' => array('id' => '','autocomplete' => 'off','class'=>array('form-control')),
					  '#type' => 'textarea',
					  '#required' => TRUE,
						);	
	$form['register_record']['docs'] 	= array(
						'#type' => 'managed_file',
						'#prefix' =>  '<div class="col-md-12">',
						'#suffix' =>  '</div>',
						'#title'=> 'File',
						'#process'=> array('import_my_file_element_process'),
						'#upload_validators'=> array('file_validate_extensions' => array('pdf')),
						'#upload_location'=> 'public://upload/common/',
						'#required' => TRUE,
						'#file_check'=> TRUE			
						 );
	$form['register_record']['finalAgree'] = array(																		 
						  '#type' => 'checkbox',
						  '#prefix' =>  '<br><div class="row"><div class="col-md-12">',
						  '#suffix' =>  '</div></div>',
						  '#description'=> '<span class="red-star">*</span>I hereby declare that the particulars given above are true the best of my knowledge and belief.',
						  '#attributes'	=> array('id' => 'finalAgreeId','onchange' => 'return checkFinalAppCheckBox()','class' => array('pull-left')),
						  '#required' => TRUE
						  );
$form['register_record']['submit'] = array(																					 
						  '#prefix' => '</div></div></div>',  
						  '#type' => 'submit',
						  '#id' => 'submitBtn1',
						  '#attributes' => array('class' => array('btn btn-primary form-submit')),
						  '#value' => 'SAVE'
						);
	
	
	
	return $form;
}

	
function register_record_form_submit($form, &$form_state){ 
	global $base_root, $base_path, $user;
	$user_id = $user->uid;
	$val = $form_state['values'];
	$r_title = $val['r_title'];
	$r_desc	= $val['r_desc'];
	
	$repository_docs = file_load($val['docs']);
	$repository_docs_filesize=$repository_docs->filesize;
	$repository_docs_file_name=$repository_docs->filename;
	$repository_docs_link=escapefile_url('public://upload/common/'.$repository_docs_file_name);
	$repository_docs_link_file=file_get_contents($repository_docs_link);
	$repository_docs_encode_image_file = base64_encode($repository_docs_link_file);
	
	$filesizekb=round($repository_docs_filesize/1024);
	
	print_r($repository_docs);
	
	echo '<br>'.get_mime_type($repository_docs);
	
	echo mime_content_type($repository_docs->uri);
	
	echo 'ddd';
	
	
	die('Page under working');
	
	if(round($filesizekb)<300){
	
		$fetch_common_det 	= db_query("select est_district, est_subdivision, est_loc_e_areatype, est_name_areatype, est_loc_e_vill_ward, est_pin_number, est_ps from l_common_application_master where user_id=:userId",array(':userId'=>$user_id));
			if( $fetch_common_det->rowCount() > 0 ){
				foreach($fetch_common_det as $obj){
					$dist_code			    =	$obj->est_district;
					$subdivision_code		=	$obj->est_subdivision;
					$areatype				=	$obj->est_loc_e_areatype;
					$block_code				=	$obj->est_name_areatype;
					$gp_ward_code			=	$obj->est_loc_e_vill_ward;
					$pin					=	$obj->est_pin_number;
					$police_station			=	$obj->est_ps;
				}
			}
		
		$fields = array( 
			'repository_title'=>$r_title,
			'repository_description'=>$r_desc,
			'file_content'=> $repository_docs_encode_image_file,
			'file_size_kb' => $repository_docs_filesize,
			'user_id' => $user_id,
			'dist_code'=> $dist_code,
			'subdivision_code'=> $subdivision_code,
			'areatype'=> $areatype,
			'block_code'=> $block_code,
			'gp_ward_code'=> $gp_ward_code,
			'pin'=> $pin,
			'police_station'=> $police_station
		);
		
		db_insert('l_register_record_repository')->fields($fields)->execute();
		$message = 'Register record repository details successfully saved.';
		drupal_set_message(t($message));
	}else{
		$message = 'File size should be less then or equal to 300kb';
		drupal_set_message(t($message), 'error');
	}
}

function register_record_list(){	
	 global $base_root, $base_path, $user;
	 $user_id = $user->uid;	
	 $header = array(
		  array('data' => 'Sl. No', 'width' => '10%'),
		  array('data' => 'Title', 'width' => '30%'),
		  array('data' => 'Description', 'width' => ''),
		  array('data' => 'View', 'width' => '10%'), 
		  array('data' => 'Action', 'width' => '10%'),  
	  	);
	  
	
	 $register_record_details 	= db_select('l_register_record_repository', 'lmrm');
	 $register_record_details->fields('lmrm',array('id','repository_title','repository_description'));
	 $register_record_details->condition('lmrm.is_deleted ','0');
	 $register_record_details->condition('lmrm.is_active ','1');
	 $register_record_details->condition('lmrm.user_id', $user->uid);
	 $register_record_details->orderBy('lmrm.id','DESC');
	 $rrd_result = $register_record_details->execute();
 
 	 if($rrd_result->rowCount() > 0 ){ 
  
  	$counter = 0;  
	$view_img = '<span class="badge bg-blue"><i class="fa fa-eye"></i> View Document<div>';
		
	foreach($rrd_result as $data){
		$counter++;
		$view_img = l('<img src =  "'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png">','register-record-repository-list-file/'.encryption_decryption_fun('encrypt', $data->id),array('html' => TRUE,'attributes'=>array('target'=>'_blank','class'=>array('#pdf-img')))); 
		
		$delete_img = l('<img src =  "'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/del.png" title="Delete">','register-record-repository-row-delete/'.encryption_decryption_fun('encrypt', $data->id),array('html' => TRUE,'attributes'=>array('class'=>array('#pdf-img')))); 
		
		$rows[] = array(
				array('data' => $counter, 'align' => 'left', 'class' => array('')),
				array('data' => $data->repository_title, 'align' => 'left'),
				array('data' => $data->repository_description, 'align' => 'left'),
				array('data' => $view_img, 'align' => 'left'),
				array('data' => $delete_img, 'align' => 'left'),
				);
		}
  	}else{
		drupal_goto('register-record-repository-form');
	}
  
    $variables = array(
	  		'attributes' => array('class' => array('table table table-striped table-responsive dashboard-table')), 
	  		'header' => $header,
	  		'rows' => $rows,
			'empty' => t("No data found!")
	  );
	  
	  $fetch_file_size 	= db_query("select SUM(file_size_kb) as file_size from l_register_record_repository where user_id='".$user_id."' and is_active =1 and is_deleted=0");
		if( $fetch_file_size->rowCount() > 0 ){
	  	foreach($fetch_file_size as $obj){
			$kb=$obj->file_size/1024;
			$mb=$kb/1024;
			if(round($mb)>0){
	  			$size='<div style="color:#008000">Sum of your total file size is '.round($mb).'MB out of 10MB</div>';
			}else{
				$size='<div style="color:#008000">Sum of your total file size is '.round($kb).'KB out of 10MB</div>';
			}
		}
		}
		
	if($size<=10){
	$output = '<div class="panel panel-default panel-form">
		  		   <!--<div class="panel-heading">Registration</div> --><br>
				   		<div class="col-md-12">'.t($size).''.l('Add Register','register-record-repository-form', array('html' => TRUE, 'attributes'=>array('target'=>'_blank','class'=>array('btn btn-primary pull-right')))).'</div>
		  	 			<div class="panel-body">';
	}else{
		$size_limit_up_text='<div style="color:#f91111">You have reached your file memory limit, your total memory limit is 10MB</div>';
		$output = '<div class="panel panel-default panel-form">
				   		<div class="col-md-12">'.t($size_limit_up_text).'</div>
		  	 			<div class="panel-body">';
		
	}
	$output .= theme('datatable', $variables);
	$output .= '</div></div>';	
  	
	
  return $output;		
  
}

function alc_view_plantation_view_file_applicant($form, &$form_state, $id = ''){

	global $base_root, $base_path, $user;
	$pk_id = encryption_decryption_fun('decrypt', $id);
	
	$register_record_details 	= db_select('l_register_record_repository', 'lmrm');
	$register_record_details->fields('lmrm',array('id','file_content'));
    $register_record_details->condition('lmrm.id',$pk_id);
   	$register_record_details_result = $register_record_details->execute();
	$re_info_result_file = $register_record_details_result->fetchAssoc();
	$id = $re_info_result_file['id'];
	$files = $re_info_result_file['file_content']; 
	
	if(!empty($files)){
			$pdf_decoded1 = base64_decode($files);
			$new_file_name='file-content'.'-'.$id.'.pdf';
			file_put_contents('sites/default/files/upload/common/'.$new_file_name, $pdf_decoded1);
	}else{
			$pdf_decoded1 = '';
	}
		
	drupal_goto($GLOBALS['base_url'].'/sites/default/files/upload/common/'.$new_file_name);

}

function register_record_repository_row_delete($form, &$form_state, $id = ''){

	global $base_root, $base_path, $user;
	$pk_id = encryption_decryption_fun('decrypt', $id);
	
	$fields = array( 
		'is_deleted' => 1,
		'deleted_time' => time(),
		
	);
	
	db_update('l_register_record_repository')->fields($fields)->condition('id',$pk_id )->execute();
		
	drupal_goto('register-record-repository-list');

}

