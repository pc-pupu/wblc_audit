<?php


function add_trade_union_info_form($form, &$form_state, $id){
	
	
	$action																= 'decrypt';
	$action_encrypt														= 'encrypt';
	$application_id														=  encryption_decryption_fun($action, $id);
	
	
	$form['clra_reg_master']['header']								 	= array(
																				'#markup' 			=>	 t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">Trade Union Information</div><div class="panel-body">'),
																										 );
	
	
	$form['clra_reg_master']['clra_act_id'] 		 					= array(
																			  '#title'				=>	'Act Id',
																			  '#type' 				=> 	'hidden',
																			  '#default_value'		=> 	1,
																			  '#attributes'			=> 	array('readonly'=>'readonly')
																		   );
																		   
	$form['clra_reg_master']['application_id'] 							= array(
																			  '#title'				=> 'Application ID',
																			  '#type' 				=> 'hidden',
																			  '#default_value'		=> !empty($application_id) ? $application_id : '',
																			  '#attributes' 		=> array('readonly'=>'readonly')
																		  );
																					
	$form['clra_reg_master']['e_trade_union_regn_no'] 					= array(
																			   '#prefix' 			=> '<div class="col-md-6"><label class="input">',
																			  '#suffix' 			=> '</label></div>',
																			  '#title'				=> 'Registration Number',
																			  '#type' 				=> 'textfield',
																			  '#autocomplete_path' 	=> 'tradeunion/autocomplete',
																			  '#ajax'				=> array(
																										  'callback' 			=> 'get_trade_union_form_ajax_callback',
																										  'event'    			=> 'blur',
																										  'effect'   			=> 'fade',
																										  'progress' 			=> array(
																																	  'type'    => 'throbber',
																																	  'message' => '',
																																	)
																										),
																		  );
																		  
		
	$form['clra_reg_master']['e_trade_union_name'] 						= array(
																				'#prefix' 			=> '<div id="trade_name_replace">',
																				'#suffix' 			=> '</div>'
																			);
																																	
	$form['clra_reg_master']['e_trade_union_name'] 						= array(
																			  '#prefix' 			=> '<div id="trade_name_replace" class="col-md-6"><label class="input">',
																			  '#suffix' 			=> '</label></div>',
																			  '#title'				=> 'Name of the Trade Union',
																			  '#type' 				=> 'textfield',
																			  '#required' 			=> TRUE
																			);
	
	$form['clra_reg_master']['e_trade_union_address'] 					= array(
																				'#prefix' 			=> '<div id="trade_address_replace">',
																				'#suffix' 			=> '</div>'
																			);							
	
   	$form['clra_reg_master']['e_trade_union_address'] 					= array(
																			  '#prefix' 			=> '<div id="trade_address_replace"class="col-md-12"><label class="textarea">',
																			  '#suffix' 			=> '</label></div>',
																			  '#title'				=> 'Address',
																			  '#type' 				=> 'textarea',
																			  '#rows'				=> 2,
																			  '#required' 			=> TRUE
																			 );
																			 
	$form['clra_reg_master']['back_markup'] 							= array ( 
																			 '#type' 				=> 'markup',
																			 '#prefix' 				=> '</div></div>',  
																			 '#markup'				=> l('<< Back to Trade Union List','view-clra-application-details/view-trade-union-application/'.encryption_decryption_fun($action_encrypt, $application_id), array('attributes' => array('class' => 'link-btn')))
																			 );
	
	$form['clra_reg_master']['submit'] 									= array (
																			  
																			  '#type' 				=> 'submit',
																			  '#attributes' 		=> array('class' => array('btn btn-primary pull-right')),
																			  '#value' 				=> 'SAVE',
																			  '#suffix' 			=> '</div>',
																			);
																			
	
	return $form;
}

function add_trade_union_info_form_validate($form, &$form_state){
	/*$val	=	$form_state['values']; 
 	foreach($form as $element){
  		if(is_array($element)){
   			if(isset($element['#type']) and ($element['#type'] == 'textfield')){
    			chkbadchar($element['#value'], $element['#title'], $element['#name']);
   			}
  		}
 	}
 
	if(isset($form['clra_reg_master']) && !empty($form['clra_reg_master'])) {
		$form_field_array1 = $form['clra_reg_master'];
		foreach($form_field_array1 as $element){
			if(is_array($element)){
				if(isset($element['#type']) and ($element['#type'] == 'textfield')){
					chkbadchar($element['#value'], $element['#title'], $element['#name']);
				}
				if(isset($element['#type']) and ($element['#type'] == 'textarea')){
					chkbadchar($element['#value'], $element['#title'], $element['#name']);
				}
			}
		}
	}*/
}

function add_trade_union_info_form_submit($form, &$form_state){
	global $base_root, $base_path, $user;
	$action									= 'encrypt';
	$val									= $form_state['values'];
	
	$act_id									= $val['clra_act_id'];
	$user_id								= $user->uid;
	$application_id							= $val['application_id'];
	$e_trade_union_regn_no					= $val['e_trade_union_regn_no'];
	$e_trade_union_name 					= $val['e_trade_union_name'];
	$e_trade_union_address 					= $val['e_trade_union_address'];
	
	$fieldsTradeInfo 						= array( 
													'act_id'						=>	$act_id,
													'user_id'						=>	$user_id,
													'application_id'				=>	$application_id,
													'e_trade_union_regn_no'			=>  $e_trade_union_regn_no,
													'e_trade_union_name'			=>  $e_trade_union_name,
													'e_trade_union_address'			=>	$e_trade_union_address
													
											  );
											  
	if( $application_id != "" ){
		db_insert('l_clra_reg_trade_union_master')->fields($fieldsTradeInfo)->execute();
		drupal_goto( 'view-clra-application-details/view-trade-union-application/'.encryption_decryption_fun($action, $application_id) );
	}
	
}

function get_trade_union_form_ajax_callback($form, $form_state){
  	$commands 											= 	array();
  	$e_trade_union_regn_no 								= 	$form_state['values']['e_trade_union_regn_no'];
	
	$query												=	db_query("SELECT e_trade_union_name, e_trade_union_address FROM {tradeunion_master} WHERE e_trade_union_regn_no=:e_trade_union_regn_no", array( ':e_trade_union_regn_no' => $e_trade_union_regn_no ));
	$query_final										=	$query->fetchAssoc();
	if($query->rowCount()>0){
		$form['clra_reg_master']['e_trade_union_name'] 		= 	array(
																	  '#prefix' 			=> 	'<div id="trade_name_replace"  class="col-md-6"><label class="input">',
																	  '#suffix' 			=> 	'</label></div></div>',
																	  '#title'				=> 	'Name of the Trade Union',
																	  '#type' 				=> 	'textfield',
																	  '#required' 			=> 	TRUE,
																	  '#attributes'			=> array('readonly' => 'readonly'),
																	  '#name'				=>	'e_trade_union_name',
																	  '#value'				=>	$query_final['e_trade_union_name']
																);
		
		$form['clra_reg_master']['e_trade_union_address'] 	= 	array(
																	  '#prefix' 			=> 	'<div id="trade_address_replace" class="col-md-12"><label class="textarea">',
																	  '#suffix' 			=> 	'</label><br></div>',
																	  '#title'				=> 	'Address',
																	  '#type' 				=> 	'textarea',
																	  '#rows'				=> 	2,
																	  '#required' 			=> 	TRUE,
																	  '#attributes'			=> array('readonly' => 'readonly'),
																	  '#name'				=>	'e_trade_union_address',
																	  '#value'				=> 	$query_final['e_trade_union_address']
																);
		
		$commands[] 										= 	ajax_command_replace('#trade_name_replace', drupal_render($form['clra_reg_master']['e_trade_union_name']));
		$commands[] 										= 	ajax_command_replace('#trade_address_replace', drupal_render($form['clra_reg_master']['e_trade_union_address']));
	}else{
		$form['clra_reg_master']['e_trade_union_name'] 		= 	array(
																	  '#prefix' 			=> 	'<div id="trade_name_replace" class="col-md-6"><label class="input">',
																	  '#suffix' 			=> 	'</label></div></div>',
																	  '#title'				=> 	'Name of the Trade Union',
																	  '#type' 				=> 	'textfield',
																	  '#required' 			=> 	TRUE,
																	  '#attributes'			=> array('readonly' => 'readonly'),
																	  '#name'				=>	'e_trade_union_name',
																);
		
		$form['clra_reg_master']['e_trade_union_address'] 	= 	array(
																	  '#prefix' 			=> 	'<div id="trade_address_replace" class="col-md-12"><label class="textarea">',
																	  '#suffix' 			=> 	'</label><br></div>',
																	  '#title'				=> 	'Address',
																	  '#type' 				=> 	'textarea',
																	  '#rows'				=> 	2,
																	  '#required' 			=> 	TRUE,
																	  '#attributes'			=> array('readonly' => 'readonly'),
																	  '#name'				=>	'e_trade_union_address',
																);
		
		$commands[] 										= 	ajax_command_replace('#trade_name_replace', drupal_render($form['clra_reg_master']['e_trade_union_name']));
		$commands[] 										= 	ajax_command_replace('#trade_address_replace', drupal_render($form['clra_reg_master']['e_trade_union_address']));
	}
	$page 												= 	array( '#type' => 'ajax', '#commands' => $commands );
	return $page;
}