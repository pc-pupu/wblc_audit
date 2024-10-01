<?php 

function ownership_list_form($form, &$form_state, $comParameter = '',$actId='' , $listType = ''){
	
	global $base_root, $base_path, $user;
	
	$user_id 			 = $user->uid;
	$act_id 			 = encryption_decryption_fun('decrypt', $actId);
	$list_type			 = encryption_decryption_fun('decrypt', $listType);
	$headingVar			 = '';
	
	/** HEADING VARIABLE ACCORDING TO TYPES **/
	
	if($list_type == 'DP'){
		$headingVar = 'NAME AND ADDRESS OF THE DIRECTORS/PARTNERS';	
	}
	
	$form['ownershipList']['header']		= array(
												'#markup' 	=> t('<div class="content"><div class="panel panel-default panel-form"><div class="panel-heading">'.$headingVar.'</div><div class="panel-body">'),
												'#type' 	=> 'markup',
												);
	
	if($act_id	== 100){
		
		$user_id		  = encryption_decryption_fun('decrypt', $comParameter);
		
		$form['ownershipList']['markup1']				= array(
																'#type' 	=> 'markup',
																'#markup' 	=> l('+ Add New Director/Partner', 'add-directorpartner/'.encryption_decryption_fun('encrypt', $act_id).'/'.encryption_decryption_fun('encrypt', $user_id), array('attributes' => array('html' => TRUE, 'class' => 'link-btn mtop-btm-15')))																			
																	);
																	
		$form['ownershipList']['markup2']				= array(
																'#type' 	=> 'markup',
																'#markup' 	=> l('<< BACK TO SELF CERTIFICATION', 'self-certification-application/particulars', array('attributes' => array('html' => TRUE, 'class' => 'link-btn mtop-btm-15 pull-right')))											 	
																	);
													
		$form['ownershipList']['directorPartner']		= array(
															'#markup' 		=> '<br/><br/>'.getDirectorPartner_details($act_id), //ISMW_LICENSE.MODULE (PAYAL)
															'#type' 		=> 'markup',
															);
		
	}
	
	$form['ownershipList']['footer']	= array(
												'#markup' 	=> t('</div></div></div>'),
												'#type' 	=> 'markup',
												);
	
	return $form;
		
}






