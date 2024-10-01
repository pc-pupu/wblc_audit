<?php

drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');

function edit_trade_union_details($form, &$form_state, $pre_id, $pre_application_id){
	global $base_root, $base_path, $user;
	$action									=	'decrypt';
	$action_encrypt							=   'encrypt';
	$id										=	encryption_decryption_fun($action, $pre_id);
	$application_id							=	encryption_decryption_fun($action, $pre_application_id);
	$user_id								=	$user->uid;
	
	
	$master_part_clra_reg 		= db_select('l_clra_registration_master', 'lcrm');
	$master_part_clra_reg->fields('lcrm', array('final_submit_status', 'status'));
	$master_part_clra_reg->condition('lcrm.user_id', $user_id);
	$master_part_clra_reg->condition('lcrm.id', $application_id); 
	$master_part_clra_reg_result = $master_part_clra_reg->execute(); 
			 
	$applicant_details			=	$master_part_clra_reg_result->fetchAssoc();
	
	$final_submit_status		=	$applicant_details['final_submit_status'];
	$status						=	$applicant_details['status']; 
	
	$get_edit_trade_Union					=	db_query('select * from l_clra_reg_trade_union_master where id=:id and application_id=:application_id and user_id=:user_id', array( ':id' => $id, ':application_id' => $application_id, ':user_id' => $user_id ));
	
	$get_edit_trade_Union_final				=	$get_edit_trade_Union->fetchAssoc();
	
	$e_trade_union_regn_no					=   trim($get_edit_trade_Union_final['e_trade_union_regn_no']);
	$e_trade_union_name						=	trim($get_edit_trade_Union_final['e_trade_union_name']);
	$e_trade_union_address					=	trim($get_edit_trade_Union_final['e_trade_union_address']);
	
	$form['clra_reg_master']['id'] 		 								= array(
																			  '#type' 				=> 	'hidden',
																			  '#default_value'		=> 	!empty($id) ? $id : '',
																			  '#attributes'			=> 	array('readonly'=>'readonly')
																		   );
																		   
	$form['clra_reg_master']['application_id'] 							= array(
																			  '#type' 				=> 'hidden',
																			  '#default_value'		=> !empty($application_id) ? $application_id : '',
																			  '#attributes' 		=> array('readonly'=>'readonly')
																		  );
																					
	$form['clra_reg_master']['e_trade_union_regn_no'] 					= array(
																			  '#prefix' 			=> '<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">Trade Union Information</div><div class="panel-body"><div class="col-md-6"><label class="input">',
																			  '#suffix' 			=> '</label></div>',
																			  '#title'				=> 'Registration Number',
																			  '#type' 				=> 'textfield',
																			  '#default_value'		=> !empty($e_trade_union_regn_no) ? $e_trade_union_regn_no : '',
																			  '#disabled'			=> TRUE
																		  );
																	
	$form['clra_reg_master']['e_trade_union_name'] 						= array(
																			  '#prefix' 			=> '<div class="col-md-6"><label class="input">',
																			  '#suffix' 			=> '</label></div>',
																			  '#title'				=> 'Name of the Trade Union',
																			  '#type' 				=> 'textfield',
																			  '#attributes'			=> 	array('readonly'=>'readonly'),
																			  '#required' 			=> TRUE,
																			  '#default_value'		=> !empty($e_trade_union_name) ? $e_trade_union_name : ''
																		  );
								
	
   $form['clra_reg_master']['e_trade_union_address'] 					= array(
																			  '#prefix' 			=> '<div class="col-md-12"><label class="textarea">',
																			  '#suffix' 			=> '</label></div>',
																			  '#title'				=> 'Address',
																			  '#attributes'			=> 	array('readonly'=>'readonly'),
																			  '#type' 				=> 'textarea',
																			  '#rows'				=> 2,
																			  '#required' 			=> TRUE,
																			  '#default_value'		=> !empty($e_trade_union_address) ? $e_trade_union_address : ''
																		  );	
	
	
	$form['clra_reg_master']['back_markup'] 							= array ( 
																				'#type' 			=> 'markup', 
																				'#prefix' 			=> '</div></div>',
																				'#suffix' 			=> '', 
																				'#markup' 			=> l('<< Back to Trade Union List', 'view-clra-application-details/view-trade-union-application/'.encryption_decryption_fun($action_encrypt, $application_id), array( 'html' => TRUE, 'attributes'=> array('class' => array('link-btn'))))
																				);
																				
																				
	if( trim($status) =='B' || trim($final_submit_status) =='' ){
		
																						
	$form['clra_reg_master']['submit'] 									= array (
																			  '#type' 				=> 'submit',
																			  '#attributes' 		=> array('class' => array('btn btn-primary pull-right')),
																			  '#value' 				=> 'SAVE'
																		  );
																		  
	}
	
	$form['clra_reg_master']['markup'] 									= array ( 
																				'#suffix' 			=> '</div>', 
																				);
	return $form;
}

function edit_trade_union_details_validate($form, &$form_state){
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

function edit_trade_union_details_submit($form, &$form_state){
	global $base_root, $base_path, $user;
	$val						=	$form_state['values'];
	$action_encrypt				=   'encrypt';
	$user_id					=	$user->uid;
	$id							=	$val['id'];
	$act_id						=	1;
	$application_id				=	$val['application_id'];
	$e_trade_union_regn_no		=	$val['e_trade_union_regn_no'];
	$e_trade_union_name			=	$val['e_trade_union_name'];
	$e_trade_union_address		=	$val['e_trade_union_address'];
	
	if( $id != "" && $application_id != "" ){
		$query 					= 	db_update('l_clra_reg_trade_union_master');
        $query					->	fields(array(
										'e_trade_union_regn_no'				=>  $e_trade_union_regn_no,
										'e_trade_union_name'				=>	$e_trade_union_name,
										'e_trade_union_address'				=>  $e_trade_union_address
         							));
							
        $query					->	condition('id',trim($id));
		$query					->	condition('application_id',trim($application_id));
		$query					->	condition('act_id',trim($act_id));
		$query					->	condition('user_id',trim($user_id));
        $query					->	execute();
		
		drupal_goto( 'view-clra-application-details/view-trade-union-application/'.encryption_decryption_fun($action_encrypt, $application_id));
	}
}