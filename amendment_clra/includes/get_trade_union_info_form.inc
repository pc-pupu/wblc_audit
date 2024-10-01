<?php

drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');

function get_trade_union_info_form($form, &$form_state, $id){
	
	drupal_add_js(drupal_get_path('module', 'applicant_forms') . '/js/upload_custom.js');
	
	global $base_root, $base_path, $user;
	
	
	
	$user_id 					= 	$user->uid;
	$action						=   'decrypt';
	$action_encrypt				=   'encrypt';
	$application_id				=  encryption_decryption_fun($action, $id);
	
	
	$master_part_clra_reg 		= db_select('l_clra_registration_master', 'lcrm');
	$master_part_clra_reg->fields('lcrm', array('final_submit_status', 'status'));
	$master_part_clra_reg->condition('lcrm.user_id', $user_id);
	$master_part_clra_reg->condition('lcrm.id', $application_id); 
	$master_part_clra_reg_result = $master_part_clra_reg->execute(); 
			 
	$applicant_details			=	$master_part_clra_reg_result->fetchAssoc();
	
	$final_submit_status		=	$applicant_details['final_submit_status'];
	$status						=	$applicant_details['status'];
	
	
	$fetch_data_status 											= 	db_query("select count(id) as counter_status from l_clra_reg_trade_union_master where application_id=:application_id and user_id=:user_id",array(':user_id'=> $user_id,':application_id'=>$application_id));
	
	$fetch_data_status_final									=	$fetch_data_status->fetchAssoc();
	
	
	$form['clra_reg_master']								    = array(
																	  '#type' 					=> 'fieldset',
																	  '#prefix' 				=> '<div class="content"><div class="sky-form" style="width:100%;">',
																	);
	
	if( $fetch_data_status_final['counter_status'] > 0 ){
		
		
		if( trim($status == 'B') || trim($final_submit_status) == ''){
		
		$form['clra_reg_master']['add_trade_union_markup']		=	array( 
																		'#prefix' 				=> '<div id="toggle_markup_trade_div">', 
																		'#suffix' 				=> '</div>', 
																		'#type' 				=> 'markup', 
																		'#markup' 				=> l('Add Trade Union', 'add-trade-union/'.encryption_decryption_fun($action_encrypt, $application_id), array( 'html' => TRUE, 'attributes'=> array('class' => array('button left-btn')))) 
																		);
																		
																		
																		
		}
		
		
		
		$get_trade_union_markup									=	get_trade_union_data_table($application_id);
		
		$form['clra_reg_master']['get_trade_union_markup']		=	array( 
																		'#type' 				=> 'markup', 
																		'#markup' 				=> $get_trade_union_markup 
																		);
		
	}else{
		
		$tradeOptions											= 	array( 0=> t('No'), 1=> t('Yes') );
								
		$form['clra_reg_master']['check_if_add_trade_union'] 	= 	array(
		 																'#prefix' 				=> '<fieldset>', 
																		'#type' 				=> 'radios', 
																		'#title' 				=> 'Click YES to add trade union', 
																		'#options' 				=> $tradeOptions,
																		'#default_value' 		=> 0,
																		'#suffix' 				=> '</fieldset>',  
																		'#attributes' 			=> array('onclick' => 'getContractorAddLink()' )  
																	);	
		
		
		$form['clra_reg_master']['add_trade_union_markup']		=	array(
																		'#prefix' 				=> '<div id="toggle_markup_trade_div"><section>', 
																		'#suffix' 				=> '</div>', 
																		'#type' 				=> 'markup', 
																		'#markup' 				=> l('+ Add New Trade Union', 'add-trade-union/'.encryption_decryption_fun($action_encrypt, $application_id), array( 'html' => TRUE, 'attributes'=> array('class' => 'button left-btn'))) 
																		);
		
		$get_trade_union_markup									=	get_trade_union_data_table($application_id);
		
		$form['clra_reg_master']['get_trade_union_markup']		=	array( 
																		'#type' 				=> 'markup', 
																		'#markup' 				=> $get_trade_union_markup,
																		'#suffix' 				=> '</div></div>', );
	}
	
	return $form;
}

function get_trade_union_data_table($application_id){
	global $base_root, $base_path, $user;
		
	
	$count							=	1;
	$uid							=	$user->uid;
	$output 						= 	'';
	$header 						= 	array(
											array('data' => 'Sl. No', 'field' => 'slno', 'width' => '10%'),
											array('data' => 'Trade Union Registration Number', 'field' => 'e_trade_union_regn_no', 'width' => '15%'),
											array('data' => 'Trade Union Name', 'field' => 'e_trade_union_name', 'width' => '20%'),
											array('data' => 'Address', 'field' => 'e_trade_union_address', 'width' => '20%'),
											array('data' => 'Operation', 'field' => '', 'width' => '20%')
										);
	
	
	$query 							= 	db_select('l_clra_reg_trade_union_master', 'lcrtum');
	$query							->	fields('lcrtum', array());
	$query							->	condition('lcrtum.user_id', trim($uid));
	$query							->	condition('lcrtum.application_id', trim($application_id));
	$query							->	orderBy('lcrtum.id', 'DESC');
	$result 						= 	$query->execute();
		
	if( $result->rowCount() > 0 ){
		while($data = $result->fetchObject()) {
			$counter				=	$count++;
			
			$editVal 				= 	array('data'=> 'Edit');
			$action					=	'encrypt';
			
			$editValue 				= 	l(t('<img width="42px" height="40px" src="'.$base_root.$base_path.'sites/default/files/edit.png">'),'edit-trade-union/'.encryption_decryption_fun($action, $data->id).'/'.encryption_decryption_fun($action, $application_id), array('html' => TRUE,'attributes'=>array('title' => 'Click to Edit', 'class' => '')));
			
			
			$rows[] 				= 	array(
											$counter,
											$data->e_trade_union_regn_no,
											$data->e_trade_union_name,
											$data->e_trade_union_address,
											$editValue
									);
		}	
	}
	
	$variables 						= 	array( 'header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('clra_applications')), 'empty' => t("No data found!"));
						
	$output 						= 	theme('datatable', $variables);
	return $output;
}