<?php
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
function view_all_list_inspector($form, &$form_state, $identifier=''){
	
  global $base_root,$base_path,$user;
  $custom_uid  	= 	$user->uid;
  $sl_no = 0;
  $identifier;

  $header = array(
					array('data' => 'SL. NO', 'field' => 'slno'),
					array('data' => 'POSTING AREA', 'field' => 'postiong'),
					array('data' => 'NAME', 'field' => 'name'),
					array('data' => 'CONTACT NO.', 'field' => 'mobile'),
					array('data' => 'Action', 'field' => 'view'),
						
				 );
  
  if($identifier	== 'zone'	){
	   		$query	 	=  db_select('l_customuser_relation_address', 'relation');
			$query		-> InnerJoin('district_master', 'dis','dis.district_code= relation.district_code');
			$query		-> InnerJoin('block_mun_master', 'block','block.block_code= relation.block_code');
			$query		-> InnerJoin('l_custom_user_detail', 'user_details','user_details.usr_id= relation.user_id');
		
			$query		-> fields('user_details', array('officenumber','fullname','mobile','usr_id'));
		
			$query		-> fields('dis', array('district_name'));
			$query		-> fields('block', array('block_mun_name','type','block_code'));	
			$query		-> condition('user_details.created_by', $custom_uid);	
			$details  	=  $query->execute()->fetchAll();
			$input 		= array_map("unserialize", array_unique(array_map("serialize", $details)));
		
			
		  if(!empty($input)){
			  foreach($input as $row){
				  $sl_no = $sl_no +1;
				  if($row->type == 'B'){
					  $Place_of_Posting	=	$row->block_mun_name.' Block <br/>'.$row->district_name;
				  }else{
					  $Place_of_Posting	=	$row->block_mun_name.'<br/>'.$row->district_name;
				  }
				  $profile		 	= 	'<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';	
				 $profile 	       .= l('<span>View profile</span>', 'alc_official/alc-view-inspector-profile/'.encryption_decryption_fun('encrypt', $row->block_code).'/'.encryption_decryption_fun('encrypt', $row->usr_id), array('html' => TRUE)); 
				 $edit_inspector	=	'<br/><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
				  $edit_inspector 	       .= l('<span>Edit</span>', 'custom_user/edit/'.encryption_decryption_fun('encrypt', $row->usr_id), array('html' => TRUE)); 
				$rows[] = array(
								array('data' => $sl_no, 'align' => 'left'),
								array('data' => $Place_of_Posting,'align' => 'left'),
								array('data' => $row->fullname,'align' => 'left'),
								array('data' =>	$row->mobile, 'align' => 'left'),
								array('data' =>	$profile.$edit_inspector, 'align' => 'left'),
								
						);
			  }
		  }
  }
  if($identifier	== 'all_wb'	){
	   		$query	 	=  db_select('l_customuser_relation_address', 'relation');
			$query		-> InnerJoin('district_master', 'dis','dis.district_code= relation.district_code');
			$query		-> InnerJoin('block_mun_master', 'block','block.block_code= relation.block_code');
			$query		-> InnerJoin('l_custom_user_detail', 'user_details','user_details.usr_id= relation.user_id');
		
			$query		-> fields('user_details', array('officenumber','fullname','mobile','usr_id','created_by'));
		
			$query		-> fields('dis', array('district_name'));
			$query		-> fields('block', array('block_mun_name','type','block_code'));	
			
			$details  	=  $query->execute()->fetchAll();
			$input 		= array_map("unserialize", array_unique(array_map("serialize", $details)));
			
			
		  if(!empty($input)){
			  foreach($input as $row){
				  $sl_no = $sl_no +1;
				  if($row->type == 'B'){
					  $Place_of_Posting	=	$row->block_mun_name.' Block <br/>'.$row->district_name;
				  }else{
					  $Place_of_Posting	=	$row->block_mun_name.'<br/>'.$row->district_name;
				  }
				  if($row->created_by == $custom_uid){
					   $edit_inspector	=	'<br/><img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';
				 	   $edit_inspector 	       .= l('<span>Edit</span>', 'custom_user/edit/'.encryption_decryption_fun('encrypt', $row->usr_id), array('html' => TRUE)); 
				  }
				   $profile		 	= 	'<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png">';	
				 $profile 	       .= l('<span>View profile</span>', 'alc_official/alc-view-inspector-profile/'.encryption_decryption_fun('encrypt', $row->block_code).'/'.encryption_decryption_fun('encrypt', $row->usr_id), array('html' => TRUE));
				 
				    
				$rows[] = array(
								array('data' => $sl_no, 'align' => 'left'),
								array('data' => $Place_of_Posting,'align' => 'left'),
								array('data' => $row->fullname,'align' => 'left'),
								array('data' =>	$row->mobile, 'align' => 'left'),
								array('data' =>	$profile.$edit_inspector, 'align' => 'left'),
								
						);
			  }
		  }
  }
		
	
	 $variables = array(
							'attributes' 		=> array('class' => array('clra_applications')),
							'header' 			=> $header,
							'rows'				=> $rows,
							'empty'				=> t('No data found')
					  );
	$output = theme('datatable', $variables);
	
	
	$form['alc_viw_inspector']    = array(
											'#type' 		=> 'fieldset',
											'#attributes'	=> array('class'=>array(''))
									  	);
										
	$form['alc_viw_inspector']['inspector_lis'] = array(		
									'#prefix' 				=> '<div class="sky-form"><div class="content">',
									'#suffix' 				=> '</div></div>',																		
									'#markup' 			    => $output,																		
									);
																

	return $form;	


		
		

}