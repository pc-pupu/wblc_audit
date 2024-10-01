<?php 

function license_history($form, &$form_state,$roleid, $actid=''){
	
	global $base_root, $base_path, $user;
	$act_id		= encryption_decryption_fun('decrypt', $actid);
	$role_id 	= encryption_decryption_fun('decrypt', $roleid);
	
	$fetch_users_role =  db_select('users_roles', 'ur');
	$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
	$fetch_users_role->fields('ro', array('rid'));
	$fetch_users_role->condition('ur.uid', $user->uid);
	$get_users_role  = $fetch_users_role->execute()->fetchAssoc();

	$user_role = $get_users_role['rid'];
	if($user_role!=''){
		$form['application_lists']['markup_data'] = array(
				'#type'  => 'markup',
				'#markup' => '<div class="nav-tabs-custom">								
								<div class="tab-content">
									<div class="tab-pane active" id="fa-icons">
										<section id="new"><div class="feedback-scroll">'.get_license_lists($user_role).'</div></section>
									</div>'
			);
		
	}else{
		drupal_set_message('Unauthorized access',error);
	}
	return $form; 
	
}

function get_license_lists($user_role=''){
	
	global $base_root, $base_path, $user;
	
	$view_img 	= '<span class="badge bg-blue"><i class="fa fa-eye"></i> View Details';
	
	$get_area_codes =	db_select('l_customuser_relation_address', 'lcra');
	$get_area_codes->fields('lcra',array('sub_div_code','district_code','block_code','custom_area_jurisdiction','is_custom_area_jurisdiction'));
	$get_area_codes->condition('lcra.user_id',$user->uid);
	$get_area_codes_result 	= 	$get_area_codes->execute()->fetchAssoc();
	
	if(!empty($get_area_codes_result)){
	
		$block_code 		= $get_area_codes_result['block_code'];
		$subdivision_code	= $get_area_codes_result['sub_div_code'];
		$customjurisdiction = $get_area_codes_result['custom_area_jurisdiction']; 
		$iscustom_juris		= $get_area_codes_result['is_custom_area_jurisdiction'];

		if($customjurisdiction=='BMCNS' && $iscustom_juris == 'Y'){
			$block_code_arr = db_select('l_customuser_reletion_services_wise_user_settings','cra')
			->fields('cra',array('block_code'))
			->condition('cra.user_id', $user->uid)
			->execute()->fetchObject()->block_code;
			$block_code_arr = explode(',', $block_code_arr); 
		}
	}
	
	
}