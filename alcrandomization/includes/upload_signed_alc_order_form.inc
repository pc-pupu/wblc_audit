<?php

drupal_add_css(drupal_get_path('module', 'alcrandomization') . '/css/custom_css.css');

function upload_signed_alc_order_form($form, &$form_state, $pre_last_id='', $pre_district_code='', $pre_subdivision_code=''){
	global $base_root, $base_path, $user;
	
	$link =	$base_root.$base_path.'sites/all/modules/dlc/sample_doc/inspection orders.docx';
	
	$action =	'decrypt';
	/*$last_id = encryption_decryption_fun($action, $pre_last_id);
	$district_code = encryption_decryption_fun($action, $pre_district_code);
	$subdivision_code =	encryption_decryption_fun($action, $pre_subdivision_code);*/
	
	$last_id = $pre_last_id;
	$district_code = $pre_district_code;
	$subdivision_code =	$pre_subdivision_code;
	
	$download_link				=	"<br><br>".l('<b>Download system generated order</b>', 'generate-random-order/ALC/'.$area_division_type."/".$district_code."/".$subdivision_code."/ALC-ORDER", array('html' => TRUE, 'attributes'=> array('target' => '_blank', 'id' => 'rcorners1')));
	
	$form['randomization_s_d']['download_link']		=	array(
															'#type' 			=> 	'markup',
															'#markup'			=>	$download_link
														);
														
	$form['randomization_s_d']['last_id']=	array(
															'#type' 			=> 	'hidden',
															'#default_value'	=>	!empty($last_id) ? $last_id : '',
															'#attributes' 		=> 	array('readonly'=>'readonly'),
														);
														
	$form['randomization_s_d']['district_code']		=	array(
															'#type' 			=> 	'hidden',
															'#default_value'	=>	!empty($district_code) ? $district_code : '',
															'#attributes' 		=> 	array('readonly'=>'readonly'),
														);
														
	$form['randomization_s_d']['subdivision_code']		=	array(
															'#type' 			=> 	'hidden',
															'#default_value'	=>	!empty($subdivision_code) ? $subdivision_code : '',
															'#attributes' 		=> 	array('readonly'=>'readonly'),
														);
	
	$form['randomization_s_d']['signed_order_doc'] 	= 	array(
															'#type' 				=> 'managed_file',
															'#title'				=> 'Upload Signed Order',
															'#upload_validators' 	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(50*1024*1024)),
															'#upload_location' 		=> 'public://upload/randomizationalcorder/',
															'#process' 				=> array('import_my_file_element_process')
												  		);
														  
    $form['randomization_s_d']['submit'] 			= 	array(
															'#type' 				=> 'submit', 
															'#value' 				=> t('Final Upload')
    											  		);
	
	$form['randomization_s_d']['back'] 				= 	array(
															'#type' 				=> 'markup',
															'#markup' 				=> '<br/>'.l('BACK TO INSPECTOR RANDOMIZATION LIST', 'alcrandomization',  array('html' => TRUE))
														);
	return $form;
}

function upload_signed_alc_order_form_validate($form, &$form_state){
	
}

function upload_signed_alc_order_form_submit($form, &$form_state){

	$val								=	$form_state['values']; print_r($val);
	$area_division_type					=	$val['area_division_type'];
	$distict_code						=	trim($val['district_code']);
	$subdivision_code					=	trim($val['subdivision_code']);
	$last_id							=	$val['last_id'];

	$arr								= 	array();
	$signed_order_doc 					= 	file_load($val['signed_order_doc']);
	if( $signed_order_doc != "" ){
		$arr 							= 	explode("/", $signed_order_doc->uri);
		$signed_order_doc->status 		= 	FILE_STATUS_PERMANENT;
		file_save($signed_order_doc);
		$location						=	$base_root.$base_path.'sites/default/files/upload/randomizationalcorder/';
		//$signed_order_doc_file 			= 	isStringInFile($location.$arr[4], 'adbe.pkcs7.detached');
		$signed_order_doc_file 			= 	$arr[4];
	}
	
	/*if( $signed_order_doc_file != "Digitally_Signed" ){
		form_error($form['randomization_s_d']['signed_order_doc'], t('Please upload a digitally signed document.'));
		$query							=	db_query('select fid from file_managed where uri=:uri and status=1', array(':uri' => $signed_order_doc->uri));
		$query_res						=	$query->fetchAssoc();
		$fid							=	$query_res['fid'];
		unlink($_SERVER['DOCUMENT_ROOT'].'/wblc/sites/default/files/upload/randomizationalcorder/'.$arr[4]);
		$query_del						=	db_query('delete from file_managed where fid=:fid and uri=:uri', array(':fid' => $fid, ':uri' => $signed_order_doc->uri));
	}else{*/
		$order_documents 					= 	array( 
													'signed_order_doc'	=>	$signed_order_doc_file,
													'order_uploaded_on'	=>	date('Y-m-d'),
													'upload_status'		=>	'Z'
												);
												
		if(!empty($order_documents)) {
			if( !empty($distict_code) && !empty($subdivision_code) ){
				$order					=	'id';
				$sort					=	'DESC';
				$query 					= 	db_update('l_area_inspector_random_order_master');
				$query->fields($order_documents);              	
				$query->condition('distict_code', trim($distict_code), '=');
				$query->condition('subdivision_code', trim($subdivision_code), '=');
				$query->condition('officer_designation', 2, '=');
				$query->condition('id', $last_id, '=');
				$query->execute();
				
				global $base_root, $base_path, $user;
				$logged_in_id			=	$user->uid;
				
				$query_alc_type			=	db_query('select area_division_type from l_custom_user_detail where usr_id =:uid', array(':uid' => trim($logged_in_id) ));
				$result					=	$query_alc_type->fetchAssoc();
				$area_division_type 	= 	$result['area_division_type'];
				
				
				$order					=	'inspector_id';
				$sort					=	'ASC';
				$show_details_query 	= 	db_select('l_area_inspector_random_tag', 't');
				$show_details_query		->	join('l_custom_user_detail', 'v', 'v.usr_id =  t.inspector_id');
				$show_details_query		->	fields('t', array('inspector_name','inspector_code','distict_code','block_municipality_code','inspector_id', 'id'));
				$show_details_query		->	condition('t.is_confirm','C','=');
				$show_details_query		->	condition('v.created_by', trim($logged_in_id), '=');
				$show_details_query		->	orderBy($order, $sort);
				$show_details_query 	= 	$show_details_query->extend('PagerDefault')->limit(50);
				$total_name 			=  	$show_details_query->execute();
				
				if($total_name->rowCount() > 0){
					foreach($total_name as $data){
						
						$inspector_code 			= 	$data->inspector_id; 
						$inspector_name 			= 	$data->inspector_name;
						$area_code 					= 	$data->block_municipality_code;
						$district_code 				= 	$data->distict_code;
						$id							=	$data->id;
						
						$query_vill_name			=	db_query('select scheduling_status, scheduling_fdt from l_gp_ward_inspector_tag a, village_ward_master b where a.block_code=b.block_code and a.village_code=b.village_code and inspector_id = :insid and confirm_s = :cst  ', array(':insid' => trim($inspector_code) , ':cst' => 'C')); 
						$result2					=	$query_vill_name->fetchAssoc();
						$scheduling_status 			= 	$result2['scheduling_status']; 
						$scheduling_fdt				=	$result2['scheduling_fdt'];
					
						if($scheduling_fdt != "" && $scheduling_status!="" && $area_division_type!=""){
							$phone_q	=	db_query('select mobile from l_custom_user_detail where usr_id=:inspector_code', array(':inspector_code' => $inspector_code));
							foreach($phone_q as $phone){
								$phone 				= 	$phone->mobile;
								$msg				=	"The order for allocation of Inspectors has been successfully generated by ALC.";
								// module_load_include('inc', 'common_encryption_form', 'include/test_cryptography_form');
								send_sms_for_user_alert($phone, $msg);
							}
						}
					}
					send_sms_for_user_alert('7603091500', $msg);
				}
			}
		}
	//}
}

function isStringInFile($file,$string){
	$handle = fopen($file, 'r');
	$valid 	= false;
	$signed	= "Not_Digitally_Signed";
	while (($buffer = fgets($handle)) !== false) {
		if (strpos($buffer, $string) !== false) {
			$valid 	= TRUE;
			$signed	= "Digitally_Signed";
			break;
		}      
	}
	fclose($handle);
	return $signed;
}