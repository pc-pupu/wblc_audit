<?php

function add_new_tender($form, &$form_state){
	
	global $base_root, $base_path, $user;
	$form['tenderaddform'] 			= array(
								'#title'		=> t('<font color="DarkGreen"><b> ADD CONTENT </b>'),
								'#type' 		=> 'fieldset',
								'#collapsed' 	=> FALSE
							); 
		 
		
	$form['tenderaddform']['ltitle']	= array(
								'#prefix' 			=> '<tr><td>',
								'#suffix' 			=> '</td></tr>',
								'#title'			=>'Link title',
								'#type' 			=> 'textfield',
								'#required' 		=> TRUE,
								//'#element_validate' => array('lcontent_textfield_validate'),
								'#attributes' 		=>  array('placeholder' => 'Enter Link Title'),
								'#default_value'	=>	isset($form_state['values']['ltitle']) ? $form_state['values']['ltitle'] : ''
								
							);
	$form['tenderaddform']['ctitle'] 		= array(
								'#prefix' 			=> '<tr><td>',
								'#suffix' 			=> '</td></tr>',
								'#title'			=>'Content title',
								'#type'				=> 'textfield',
								'#required' 		=> TRUE,
								//'#element_validate' => array('lcontent_textfield_validate'),
								'#attributes' 		=>  array('placeholder' => 'Enter Content Title'),
								'#default_value'	=>	isset($form_state['values']['ctitle']) ? $form_state['values']['ctitle'] : ''
							);
	$form['tenderaddform']['add_content_desc'] 	= array(
								'#prefix' 				=> '<tr><td>',
								'#suffix' 				=> '</td></tr>',
								'#title'				=>'Content Description :',
								'#cols' 				=> 20, 
								'#rows' 				=> 2,
								//'#type' 				=> 'text_format',
								'#required' 			=> TRUE,
								'#type' 				=> 'textarea',
								//'#element_validate' 	=> array('lcontent_textfield_validate'),
								'#attributes' 		=>  array('placeholder' => 'Enter Content Description'),
								'#default_value'	=>	isset($form_state['values']['add_content_desc']) ? $form_state['values']['add_content_desc'] : ''
							);
		 
		 
		 
	$form['tenderaddform']['meta_title'] 		= array(
								'#prefix' => '<tr><td>',
								'#suffix' => '</td></tr>',
								'#title'=>'Meta title',
								'#type' => 'textfield',
								'#required' => TRUE,
								//'#element_validate' => array('lcontent_textfield_validate'),
								'#attributes' => array('title' => 'Pin Code  should be numeric and 6 Digits','placeholder'=>'Enter Meta Title'),
								'#default_value'	=>	isset($form_state['values']['meta_title']) ? $form_state['values']['meta_title'] : ''
							);
	$form['tenderaddform']['meta_content_desc'] 	= array(
								'#prefix' => '<tr><td>',
								'#suffix' => '</td></tr>',
								'#title'=>'Meta Description :',
								'#cols' => 20, 
								'#rows' => 2,
								// '#type' => 'text_format',
								'#required' => TRUE,
								'#type' => 'textarea',
								//'#element_validate' => array('lcontent_textfield_validate'),
								'#default_value'	=>	isset($form_state['values']['meta_content_desc']) ? $form_state['values']['meta_content_desc'] : ''
							  );	 


	$form['tenderaddform']['order_no'] 		= array(
								'#prefix' => '<tr><td >',
								'#suffix' => '</td></tr>',
								'#title'=>'Order No',
								'#size'=>t('7'),
								
								'#type' => 'textfield',
								'#maxlength' => 4,
								
								'#required' => TRUE,
								//'#element_validate' => array('ORDER_MANDATORY_NUMERIC_CHECKING_validate'),
								'#attributes' => array('placeholder' => 'Order no should be numeric '),
								'#default_value'	=>	isset($form_state['values']['order_no']) ? $form_state['values']['order_no'] : ''
							);




	$form['tenderaddform']['add_tender_pdf'] 	= array(
								'#prefix' => '<tr><td >',
								'#suffix' => '</td></tr>',
								'#title' => t('Upload an Content  file (JPG,PDF only)'),
								'#required' => TRUE,
								'#type' => 'managed_file',
								'#upload_validators'=> array(
											     'file_validate_extensions' => array('jpg pdf'),
											     'file_validate_size' => array(125*1024*1024)
											),
								'#upload_location' 	=> 'public://upload/tender_upload_files/',
							);
	
	$options 	= array(
				'Y' => t('Yes'),
				'N' => t('No')
			);
	
	$form['tenderaddform']['is_new']		= array(
								'#prefix' => '<tr><td >',
								'#suffix' => '</td></tr>',
								'#title' => t('Is it New?'),
								'#type' => 'radios',
								'#options' => $options,
								'#default_value' => 'N',
								'#required' => TRUE,
							
							);
		
	$form['tenderaddform']['submit'] 		= array (
								'#type' => 'submit',
								'#value' => 'Save',
								'#prefix' => '<tr><td><div align="center">',  
								'#suffix' => '</div></td></tr></table>',
							); 
	  
	return  $form;
}
function add_new_tender_validate($form, &$form_state){
	$val 			=	$form_state['values'];
	$ltitle			=	trim(strip_tags($val['ltitle']));
	$ctitle			=	trim(strip_tags($val['ctitle']));
	$add_content_desc 	= 	$val['add_content_desc'];
	$meta_title		=	trim(strip_tags($val['meta_title']));
	$meta_content_desc 	= 	$val['meta_content_desc'];
	$order_no 		= 	trim($val['order_no']);
	$is_new			=	$val['is_new'];
	
	if($ltitle == ""){
		form_error($form['tenderaddform']['ltitle'], t('Link Title cannot be blank.'));
	}
	if($ctitle == ""){
		form_error($form['tenderaddform']['ctitle'], t('Content Title cannot be blank.'));
	}
	if($add_content_desc == ""){
		form_error($form['tenderaddform']['add_content_desc'],t('Content Description can not be blank.'));
	}
	if($meta_title == ""){
		form_error($form['tenderaddform']['meta_title'],t('Meta Title can not be blank.'));
	}
	if($meta_content_desc == ""){
		form_error($form['tenderaddform']['meta_content_desc'],t('Meta Description can not be blank.'));
	}
	if(!is_numeric($order_no)){
		form_error($form['tenderaddform']['order_no'],t('Order Number must be numeric.'));
	}
}
function add_new_tender_submit($form, &$form_state){
	global $base_root, $base_path, $full_base;
	$message		= 	"";
	$txn 			= 	db_transaction();
	$val 			=	$form_state['values'];
	$ltitle			=	trim(strip_tags($val['ltitle']));
	$ctitle			=	trim(strip_tags($val['ctitle']));
	$add_content_desc 	= 	$val['add_content_desc'];
	$meta_title		=	trim(strip_tags($val['meta_title']));
	$meta_content_desc 	= 	$val['meta_content_desc'];
	$order_no 		= 	trim($val['order_no']);
	$is_new			=	$val['is_new'];
	$file_info 		= 	file_load($val['add_tender_pdf']);
	$fid			=	0;
	$tender_file		=	"";
	$mime_type		=	"";
	try{
		if(!empty($file_info)){
			$mime_type 				= mime_content_type($file_info->uri);
			
			/* Upload For Tender */
			if(!empty($file_info) && (($mime_type=='image/jpeg') || ($mime_type=='application/pdf'))){
				if(!empty($file_info)){
					$file_uri 		= explode("/", $file_info->uri);
					$tender_file 		= $file_uri[4];
					$file_info->status 	= FILE_STATUS_PERMANENT;
					file_save($file_info);
					$original_file_name 	= $file_info->filename;
					$fid			= trim($file_info->fid);
				}
				/* Insert data into table l_cm_content */
				$insertArray	=	array(
							'content_typ'	=> t('tender'),
							'content_title'	=> $ctitle,
							'content_desc'	=> $add_content_desc,
							'content_dt'	=> date('Y-m-d'),
							'meta_title'	=> $meta_title,
							'meta_desc'	=> $meta_content_desc,
							'fid'		=> $fid,
							'is_new'	=> $is_new,
							'ord'		=> $order_no,
							'link_title'	=> $ltitle,
							'act_file_type'	=> t('tender'),
							'acts_and_rules_pdf' => $tender_file,
							'is_active'	=> 'Y',
							'create_date'	=> time()
						);
				db_insert('l_cms_content')->fields($insertArray)->execute();
				$message 	= 'Tender Added Successfully';
			}else{
				$message = 'Tender Not Added ';
			}
		}
	}catch(Exception $e){
		$txn->rollback();
		watchdog_exception('type', $e);
		$message = 'Tender Not Added ';
	}
	$full_base = $base_root.$base_path.'list-tender';
	drupal_set_message(t($message));
	drupal_goto($full_base);
}

function list_tender($form, &$form_state){
	global $base_root, $base_path, $full_base;
	
	$addlink = $base_root.$base_path.'add-tender';
	$markuplink	=	l('Add New', $addlink);
	
	$form['markup_link']	=	array(
		'#type' => 'markup',
		'#markup' => $markuplink
	);
	
	$markuptable	=	getOutputTable();

	$form['markup_table']	=	array(
		'#type' => 'markup',
		'#markup' => $markuptable
	);
	return $form;
}

function getOutputTable(){
	drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
	
	global $base_url, $base_root, $base_path, $full_base;
	$sort = 'DESC';
	$order = 't.content_id';
	$cms_tender_query = db_select('l_cms_content', 't');
	$cms_tender_query->join('file_managed', 'b', 't.fid=b.fid');
	$cms_tender_query->fields('b', array('filename'));
	$cms_tender_query->fields('t', array('fid','content_id','content_title','content_dt','content_desc', 'ord','is_active', 'is_new'));
	$cms_tender_query->condition('t.content_typ','tender','=');
	$cms_tender_query->orderBy($order, $sort);
	$cms_tender_query_exe = $cms_tender_query->execute();	
	$cms_tender_query_result = $cms_tender_query_exe->fetchAll();
	//$rows 	= array();
	$header = array(
          		array('data' => 'Sl. No', 'field' => 'slno'),
		  	array('data'=> 'Title', 'field'=>'content_title'),
			array('data'=> 'Is Active', 'field'=>'is_active'),
			array('data'=> 'Action', 'field'=>'')
		);
	$output = '';
	$i=0;
	//print_r($cms_eodb_query_result);die;
	if(count($cms_tender_query_result)>0){
		foreach($cms_tender_query_result as $data){
			$i++;
			
			if($data->is_new == 'Y'){
				$new_image = '<img src="'.$base_root.$base_path.'sites/default/files/new_gif_icon.gif" style="border:none;">';
			}else{
				$new_image = '';
			}
			
			$aa 	= $base_root.$base_path.'sites/default/files/upload/tender_upload_files/'.$data->filename;
			$link_s = l(t('View'), $aa, array('attributes' => array('target'=>'_blank')));
			$editpath = $base_root.$base_path.'edit-tender/'.$data->content_id;
			$edit 	= l(t('Edit'), $editpath);
			$change_status = $base_root.$base_path.'change-status/'.$data->content_id;
			$status = l(t('<font color="#ff0000"><b>Inactive</b></font>'),$change_status,array('html'=>TRUE));
			if($data->is_active == 'Y'){
				$status = l(t('<font color="#008000"><b>Active</b></font>'),$change_status,array('html'=>TRUE));
			}
			$content_title = $data->content_title;
			$rows[] = array(
					$i,
					$content_title,
					$status,
					$edit
				);
		}
	}
   	$variables 		= 	array( 
						'header' => $header, 
						'rows' => $rows, 
						'attributes' => array('class' => array('clra_applications')), 
						'empty' => t("No data found!")
					);
	
	$output .= theme('datatable', $variables);
	return $output;
}

function edit_tender($form, &$form_state, $idenc=''){
	
	global $base_url, $base_path, $base_root, $full_base;
	$id			= encryption_decryption_fun('decrypt', $idenc);
	$cms_tender_query = db_select('l_cms_content', 't');
	$cms_tender_query->join('file_managed', 'b', 't.fid=b.fid');
	$cms_tender_query->fields('b', array('filename'));
	$cms_tender_query->fields('t', array('fid','content_id','content_title','meta_title', 'meta_desc', 'content_dt','content_desc', 'link_title', 'ord','is_active', 'is_new'));
	$cms_tender_query->condition('t.content_typ','tender','=');
	$cms_tender_query->condition('t.content_id',$id);
	$cms_tender_query_exe = $cms_tender_query->execute();	
	$cms_tender_query_result = $cms_tender_query_exe->fetchAssoc();
	
	//print_r($cms_tender_query_result);
	
	$form['tendereditform'] 			= array(
								'#title'		=> t('<font color="DarkGreen"><b> ADD CONTENT </b>'),
								'#type' 		=> 'fieldset',
								'#collapsed' 		=> FALSE
							); 
		 
		
	$form['tendereditform']['ltitle']	= array(
								'#prefix' 		=> '<tr><td>',
								'#suffix' 		=> '</td></tr>',
								'#title'		=>'Link title',
								'#type' 		=> 'textfield',
								'#required' 		=> TRUE,
								//'#element_validate' => array('lcontent_textfield_validate'),
								'#attributes' 		=>  array('placeholder' => 'Enter Link Title'),
								'#default_value'	=> $cms_tender_query_result['link_title']
								
							);
	$form['tendereditform']['ctitle'] 		= array(
								'#prefix' 		=> '<tr><td>',
								'#suffix' 		=> '</td></tr>',
								'#title'		=>'Content title',
								'#type'			=> 'textfield',
								'#required' 		=> TRUE,
								//'#element_validate' => array('lcontent_textfield_validate'),
								'#attributes' 		=> array('placeholder' => 'Enter Content Title'),
								'#default_value'	=> $cms_tender_query_result['content_title']
							);
	$form['tendereditform']['add_content_desc'] 	= array(
								'#prefix' 		=> '<tr><td>',
								'#suffix' 		=> '</td></tr>',
								'#title'		=>'Content Description :',
								'#cols' 		=> 20, 
								'#rows' 		=> 2,
								//'#type' 		=> 'text_format',
								'#required' 		=> TRUE,
								'#type' 		=> 'textarea',
								//'#element_validate' 	=> array('lcontent_textfield_validate'),
								'#attributes' 		=> array('placeholder' => 'Enter Content Description'),
								'#default_value'	=> $cms_tender_query_result['content_desc']
							);
		 
		 
		 
	$form['tendereditform']['meta_title'] 		= array(
								'#prefix' => '<tr><td>',
								'#suffix' => '</td></tr>',
								'#title'=>'Meta title',
								'#type' => 'textfield',
								'#required' => TRUE,
								//'#element_validate' => array('lcontent_textfield_validate'),
								'#attributes' => array('title' => 'Pin Code  should be numeric and 6 Digits','placeholder'=>'Enter Meta Title'),
								'#default_value'	=>	$cms_tender_query_result['meta_title']
							);
	$form['tendereditform']['meta_content_desc'] 	= array(
								'#prefix' => '<tr><td>',
								'#suffix' => '</td></tr>',
								'#title'=>'Meta Description :',
								'#cols' => 20, 
								'#rows' => 2,
								// '#type' => 'text_format',
								'#required' => TRUE,
								'#type' => 'textarea',
								//'#element_validate' => array('lcontent_textfield_validate'),
								'#default_value'	=>	$cms_tender_query_result['meta_desc']
							  );	 


	$form['tendereditform']['order_no'] 		= array(
								'#prefix' => '<tr><td >',
								'#suffix' => '</td></tr>',
								'#title'=>'Order No',
								'#size'=>t('7'),
								
								'#type' => 'textfield',
								'#maxlength' => 4,
								
								'#required' => TRUE,
								//'#element_validate' => array('ORDER_MANDATORY_NUMERIC_CHECKING_validate'),
								'#attributes' => array('placeholder' => 'Order no should be numeric '),
								'#default_value' => $cms_tender_query_result['ord']
							);




	$form['tendereditform']['add_tender_pdf'] 	= array(
								'#prefix' => '<tr><td >',
								'#suffix' => '</td></tr>',
								'#title' => t('Upload an Content  file (JPG,PDF only)'),
								'#type' => 'managed_file',
								'#upload_validators'=> array(
											     'file_validate_extensions' => array('jpg pdf'),
											     'file_validate_size' => array(125*1024*1024)
											),
								'#upload_location' 	=> 'public://upload/tender_upload_files/',
								//'#default_value' => $cms_tender_query_result['fid']
							);
	$form['tendereditform']['file_id']		= array(
								'#type' => 'hidden',
								'#value' => $cms_tender_query_result['fid']
							);
	
	$options 	= array(
				'Y' => t('Yes'),
				'N' => t('No')
			);
	if($cms_tender_query_result['is_new']=='Y'){
		$defaultData = 'Y';
	}else{
		$defaultData = 'N';
	}
	$form['tendereditform']['is_new']		= array(
								'#prefix' => '<tr><td >',
								'#suffix' => '</td></tr>',
								'#title' => t('Is it New?'),
								'#type' => 'radios',
								'#options' => $options,
								'#default_value' => $defaultData,
								'#required' => TRUE,
							
							);
	$form['tendereditform']['tender_id']		= array(
								'#type' => 'hidden',
								'#value' => $cms_tender_query_result['content_id']
							
							);
	$form['tendereditform']['submit'] 		= array (
								'#type' => 'submit',
								'#value' => 'Save',
								'#prefix' => '<tr><td><div align="center">',  
								'#suffix' => '</div></td></tr></table>',
							); 
	  
	return  $form;	
}

function edit_tender_validate($form, &$form_state){
	$val 			=	$form_state['values'];
	$ltitle			=	trim(strip_tags($val['ltitle']));
	$ctitle			=	trim(strip_tags($val['ctitle']));
	$add_content_desc 	= 	$val['add_content_desc'];
	$meta_title		=	trim(strip_tags($val['meta_title']));
	$meta_content_desc 	= 	$val['meta_content_desc'];
	$order_no 		= 	trim($val['order_no']);
	$is_new			=	$val['is_new'];
	
	if($ltitle == ""){
		form_error($form['tendereditform']['ltitle'], t('Link Title cannot be blank.'));
	}
	if($ctitle == ""){
		form_error($form['tendereditform']['ctitle'], t('Content Title cannot be blank.'));
	}
	if($add_content_desc == ""){
		form_error($form['tendereditform']['add_content_desc'],'Content Description can not be blank.');
	}
	if($meta_title == ""){
		form_error($form['tendereditform']['meta_title'],t('Meta Title can not be blank.'));
	}
	if($meta_content_desc == ""){
		form_error($form['tendereditform']['meta_content_desc'],t('Meta Description can not be blank.'));
	}
	if(!is_numeric($order_no)){
		form_error($form['tendereditform']['order_no'],t('Order Number must be numeric.'));
	}
}

function edit_tender_submit($form, &$form_state){
	global $base_root, $base_path, $full_base;
	$message		= 	"";
	$txn 			= 	db_transaction();
	$val 			=	$form_state['values'];
	$tender_id		=	$val['tender_id'];
	$ltitle			=	trim(strip_tags($val['ltitle']));
	$ctitle			=	trim(strip_tags($val['ctitle']));
	$add_content_desc 	= 	$val['add_content_desc'];
	$meta_title		=	trim(strip_tags($val['meta_title']));
	$meta_content_desc 	= 	$val['meta_content_desc'];
	$order_no 		= 	trim($val['order_no']);
	$is_new			=	$val['is_new'];
	$file_info 		= 	file_load($val['add_tender_pdf']);
	$tender_file		=	"";
	$file_id 		=	$val['file_id'];
	try{
		/* Update data of the table l_cm_content */
		$updateArray	=	array(
					'content_typ'	=> t('tender'),
					'content_title'	=> $ctitle,
					'content_desc'	=> $add_content_desc,
					'meta_title'	=> $meta_title,
					'meta_desc'	=> $meta_content_desc,
					//'fid'		=> $fid,
					'is_new'	=> $is_new,
					'ord'		=> $order_no,
					'link_title'	=> $ltitle,
					'act_file_type'	=> t('tender'),
					//'acts_and_rules_pdf' => $tender_file,
					'is_active'	=> 'Y',
					'create_date'	=> time()
				);
		if(!empty($file_info)){
			$mime_type 				= mime_content_type($file_info->uri);
			
			/* Upload For Tender */
			if(!empty($file_info) && (($mime_type=='image/jpeg') || ($mime_type=='application/pdf'))){
				if(!empty($file_info)){
					$file_uri 		= explode("/", $file_info->uri);
					$tender_file 		= $file_uri[4];
					$file_info->status 	= FILE_STATUS_PERMANENT;
					file_save($file_info);
					$original_file_name 	= $file_info->filename;
					$updateArray['fid']	= trim($file_info->fid);
					$updateArray['acts_and_rules_pdf'] = $tender_file;
				}
			}
		}
		db_update('l_cms_content')->fields($updateArray)->condition('content_id',$tender_id)->execute();
		$message 	= 'Tender Update Successfully';
	}catch(Exception $e){
		$txn->rollback();
		watchdog_exception('type', $e);
		$message = 'Tender Not Updated ';
	}
	$full_base = $base_root.$base_path.'list-tender';
		
	drupal_set_message(t($message));
	drupal_goto($full_base);
}
function change_status($form, &$form_state, $id = ''){
	global $base_root, $base_path, $full_base;
	$txn 			= 	db_transaction();
	$message 	= "";
	
	// get last status data
	try{
		$get_data_query 	= db_select('l_cms_content','lcc')->fields('lcc',array('is_active'))->condition('content_id',$id)->execute();
		$get_data_result	= $get_data_query->fetchAssoc();
		if(!empty($get_data_result)){
			$prev_status	= $get_data_result['is_active'];
			if($prev_status=='Y'){
				$new_status = 'N';
				$new_status_show = 'Inactive';
			}else{
				$new_status	= "Y";
				$new_status_show = 'Active';
			}
			// update data with new data
			$statusArr 	= array('is_active'=>$new_status);
			db_update('l_cms_content')->fields($statusArr)->condition('content_id',$id)->execute();
			$message	= "Tender Status Updated to ".$new_status_show;
		}else{
			echo 'No Data Found';
		}
	}catch(Exception $e){
		$txn->rollback();
		watchdog_exception('type', $e);
		$message = 'Some Error! Please Try Again... ';
	}
	$full_base	= $base_root.$base_path.'list-tender';
	drupal_set_message(t($message));
	drupal_goto($full_base);
}