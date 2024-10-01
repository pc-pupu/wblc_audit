<?php
drupal_add_css(drupal_get_path('module', 'cmscontent') . '/css/cms.css');
function get_actandrules(){
	 global $base_root, $base_path, $user;
	 drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
	 
	 $act_circular_query = db_select('l_act_master', 'lam');	 
	 $act_circular_query->fields('lam', array('id', 'act_title'));	 
	 $act_circular_query_result = $act_circular_query->execute();	 
		 	 
	 $rows = array();
	 $header = array();
      
	 $header = array(
          		array('data' => 'SL.NO'),
		  		array('data' => 'ACT'),
				array('data' => 'ACT FILE(S)'),
          		array('data' => 'RULES'),
				array('data' => 'NOTIFICATION')		   			
	 );
    
	 $i	= 0;
	 foreach($act_circular_query_result as $data){  
		  $i++;
		  global $base_root, $base_path;
		     
		  /* ACT START */
		     
		  $act_query = db_select('l_cms_content', 'lcc');	 
		  $act_query->fields('lcc', array('content_id', 'acts_and_rules_pdf'));
		  $act_query->condition('lcc.content_typ','actrule','=');
		  $act_query->condition('lcc.act_id',$data->id,'=');
		  $act_query->condition('lcc.act_file_type','ACT','='); 
		  $act_query_result = $act_query->execute();
		  $result = $act_query_result->fetchAssoc();
		  $act_file   = $result['acts_and_rules_pdf'];
		  $content_id = $result['content_id'];	
		  $act_title 	= $data->act_title;
		    
		  if(!empty($act_file)){		     
			 	$bb            = $GLOBALS['base_url'].'/sites/default/files/upload/content_act_rule_pdf/'.$act_file;
	    		$act_dwn	   = '&nbsp;&nbsp;'.$link_s=l(t('<span class="fn_black"><i class="fa fa-download"></i></span>'),$bb , array('html' => TRUE,'attributes'=>array('title'=>'Click to Print/Download','target'=> '_blank'))) ;
			 
			 
			  // $act_edit 	= l(t('Edit'),'cmscontent/edit-actandrules/'.encryption_decryption_fun('encrypt',$content_id));
			    $aa            =    $base_root.$base_path.'cmscontent/edit-actandrules/'.encryption_decryption_fun('encrypt',$content_id);
	    		$act_edit		   =    $link_s=l(t('<span class="fn_black"><i class="fa fa-edit"></i></span>'), $aa, array('html' => TRUE,'attributes'=>array('title'=>'Click to Edit Details'))) ;
		  }else{
			    $aa            =    $base_root.$base_path.'cmscontent/edit-actandrules/'.encryption_decryption_fun('encrypt',$content_id);
	    		$act_edit		   =    $link_s=l(t('<span class="fn_black"><i class="fa fa-edit"></i></span>'), $aa, array('html' => TRUE,'attributes'=>array('title'=>'Click to Edit Details'))) ;
	    		$act_dwn="";
		  }
		     
		  /* ACT END */
		     
		  /* RULE START */
		     
		  $rule_view 	= 'N/A';		
		  $rule_edit 	= '';
		  $result_rule = array();
		     
		  $rule_query = db_select('l_cms_content', 'lcc');	 
		  $rule_query->fields('lcc', array('content_id', 'acts_and_rules_pdf'));
		  $rule_query->condition('lcc.content_typ','actrule');
		  $rule_query->condition('lcc.act_id',$data->id);
		  $rule_query->condition('lcc.act_file_type','RULES'); 
		  $rule_query_result = $rule_query->execute();
		     
		  $result_rule = $rule_query_result->fetchAll();
		  //echo $data->id;echo "<pre>";print_r($result_rule);exit;			     
		  $rule_file   = $result_rule[0]->acts_and_rules_pdf;
		  $rule_content_id = $result_rule[0]->content_id;
		     
		  if(count($result_rule) == 1){			
						  
				$pdf_image='<span class="fn_black"><i class="fa fa-download"></i></span>';
			    $rule_view 	='&nbsp;&nbsp;'.l($pdf_image, ''.$GLOBALS['base_url'].'/sites/default/files/upload/content_act_rule_pdf/'.$rule_file, array('html'=>TRUE,'attributes' => array('target'=> '_blank','align','title'=> 'Click To Print/Download')));
			   
			    $aa            =    $base_root.$base_path.'cmscontent/edit-actandrules/'.encryption_decryption_fun('encrypt',$rule_content_id);
	    		$rule_edit		   =    $link_s=l(t('<span class="fn_black"><i class="fa fa-edit"></i></span>'), $aa, array('html' => TRUE,'attributes' => array('title'=> 'Edit Rules','terget'=>'_blank'))) ;			
		  }
		  
		  if(count($result_rule) > 1){	

			  	$rule_view 	= l(t('<span class="fn_black"><i class="fa fa-list-ul"></i> </span>'), 'cmscontent/more-actandrules/RULES/'.encryption_decryption_fun('encrypt', $data->id),  array('html' => TRUE,'attributes'=>array('title'=> count($result_rule).' Rules Click to View List')));
			   	$bb =    $GLOBALS['base_url'].'/sites/default/files/upload/content_act_rule_pdf/'.$rule_file;
	    		//$rule_view		   =    $link_s=l(t('<span class="badge bg-blue"><i class="fa fa-view"></i> View Details</span>'),$bb , array('html' => TRUE)) ;
			  	$rule_edit 	= '';
		  }

		  if(count($result_rule) < 1){	
		  		$aa =    $base_root.$base_path.'cmscontent/edit-actandrules/'.encryption_decryption_fun('encrypt',$rule_content_id);
	    		$rule_view	 =    $link_s=l(t('<span class="fn_black"><i class="fa fa-edit"></i></span>'), $aa, array('html' => TRUE,'attributes' => array('title'=> 'Edit Rules','terget'=>'_blank'))) ;
		  }

		  
		     
		  /* RULE END */
		     
		  /* NOTIFICATION START */
		     
		  $notification_view 	= 'N/A';		
		  $notification_edit 	= '';
		  $noti_query_result = array();
		  $result_noti = array();
		     
		  $noti_query = db_select('l_cms_content', 'lcc');	 
		  $noti_query->fields('lcc', array('content_id', 'acts_and_rules_pdf'));
		  $noti_query->condition('lcc.content_typ','actrule');
		  $noti_query->condition('lcc.act_id',$data->id);
		  $noti_query->condition('lcc.act_file_type','Notification');
		  $noti_query_result = $noti_query->execute();
		     
		  $result_noti = $noti_query_result->fetchAll();
		  // if(count($result_noti)>1)
		  // {
		  // 	echo $data->id.'-'.$i."<pre>";print_r($result_noti);exit;
		  // }		
		   
		  $noti_file   = $result_noti[0]->acts_and_rules_pdf;
		  $noti_content_id = $result_noti[0]->content_id;
		  
		  if(count($result_noti) == 1 && !empty($noti_file)){			
			 // $notification_view 	= l(t('<span class="badge bg-blue"><i class="fa fa-view"></i> View Details</span>'),''.$GLOBALS['base_url'].'/sites/default/files/upload/content_act_rule_pdf/'.$rule_file, array('html' => TRUE,'attributes'=>array('target'=>'_blank')));
			  $pdf_image='<span class="fn_black"><i class="fa fa-download"></i></span>';
			  $notification_view 	='&nbsp;&nbsp;'.l($pdf_image, ''.$GLOBALS['base_url'].'/sites/default/files/upload/content_act_rule_pdf/'.$noti_file, array('html'=>TRUE,'attributes' => array('target'=> '_blank','title'=>'Click To Print/Download')));
			  
			  $notification_edit 	= l(t('<span class="fn_black"><i class="fa fa-edit"></i></span>'), 'cmscontent/edit-actandrules/'.encryption_decryption_fun('encrypt',$noti_content_id),array('html' => TRUE,'attributes'=>array('title'=>'Click to Edit Notification')));
			  			
		  }
		  
		  if(count($result_noti) > 1){	
			 $pdf_image=$GLOBALS['base_url'].'/sites/all/modules/cmscontent/images/pdf.png';
			  //$notification_view 	=l("<img src='$pdf_image'/>", ''.$GLOBALS['base_url'].'/sites/default/files/upload/content_act_rule_pdf/'.$rule_file, array('html'=>TRUE,'attributes' => array('target'=> '_blank')));
			$notification_view 	= l(t('<span class="fn_black"><i class="fa fa-list-ul"></i></span>'), 'cmscontent/more-actandrules/Notification/'.encryption_decryption_fun('encrypt', $data->id),  array('html' => TRUE,'attributes'=>array('title'=>count($result_noti).' Notification Click to View Notification List')));
			  $notification_edit 	= '';
		  }	

		  if(count($result_noti) < 1){
		  	 $notification_view 	= l(t('<span class="fn_black"><i class="fa fa-edit"></i></span>'), 'cmscontent/edit-actandrules/'.encryption_decryption_fun('encrypt',$noti_content_id),array('html' => TRUE,'attributes'=>array('title'=>'Click to Edit Notification')));
		  }	
		     
		  /* NOTIFICATION END */		    
		  $rows[] = array(
				   $i,
				   $act_title,
				   //$act_view.'<br>'.$act_edit,
				   $act_edit.' '.$act_dwn,
				   $rule_edit.' '.$rule_view,
				   $notification_edit.' '.$notification_view,				
			   );   
	 }
	 
	 //exit;
	 $variables = array(
			   'attributes' 		=> array('class' => array('table table-striped table-responsive admin-custom-table')),
			   'header' 			=> $header,
			   'rows'				=> $rows,
		  );
	
	 $output = theme('datatable', $variables);
   
	 $dic_detail_hlep_element = array(                      
					     '#title' => t('<b></b>'),                       
					     '#collapsible' => true, 
					     '#collapsed'  => true,
				    );
	 $fieldset_dic_help_link = theme('fieldset', array('element' => $dic_detail_hlep_element));
	 return '<div class="box box-primary"><div class="box-body">'.$fieldset_dic_help_link.$output.'</div></div>';
 }
 
function add_actandrules($form , &$form_states){
	 global $base_root, $base_path, $user;
	 $act_list 	= get_act_list();
	$form['actandrules']['cont_head'] = array(
          '#type' 	=> 'markup',
		  '#markup' => '<div class="row"><div class="col-md-12"><div class="box box-primary"><div class="box-body">',
          
          );
	 $form['actandrules']['act'] = array(
					     '#title' 			=> 'Select Act',
					     '#type' 			=> 'select',
					     '#required' 		=> TRUE,
						 
						 '#prefix'				=> '<div class="form-custom col-md-8"><label class="input">',
						 '#suffix'				=>  '</label></div>',
					     '#options' 		=> $act_list,
						 '#attributes' 		=> array('class' => array('form-control')),
						 //'#element_validate' 	=> array('MISC_TEXTBOX_TEXT_VALIDATE'),
					    
				    );
	 $form['actandrules']['act_type']    = array(
						  '#type' 			=> 'select',
						  '#title' 			=> 'Select type(Act/Rules/Notification)',
						  '#options' 		=>  array('' => 'Select any', 'ACT' =>t('ACTS'), 'RULES' =>t('RULES'),'Notification' =>t('Notification')),
						  '#default_value' 		=> '',
						  '#prefix'				=> '<div class="form-custom col-md-4"><label class="input">',
						  '#suffix'				=>  '</label></div>',
						  '#attributes' 		=> array('class' => array('form-control')),
						  '#required' 		=> TRUE
					     ); 	
		    
	 $form['actandrules']['description'] = array(		
						      '#title'			=> 'Description',						
						      '#type' 			=> 'textarea',
							  '#attributes' 		=> array('class' => array('form-control')),	
							  '#prefix'				=> '<div class="form-custom col-md-12"><label class="input">',
							  '#suffix'				=>  '</label></div>',					
						      '#default_value' 		=> '',
					     );	
						 		
	$form['actandrules']['order_no']    = array(		
						      '#title'			=> 'Order No',
						      '#size'			=> t('7'),		
						      '#type' 			=> 'textfield',
						      '#maxlength' 		=> 4,						
						      '#required' 		=> TRUE,
							  '#attributes' 		=> array('class' => array('form-control')),	
		  					  '#prefix'				=> '<div class="form-custom col-md-4"><label class="input">',
		  					  '#suffix'				=>  '</label></div>',
						      '#element_validate' 	=> array('ORDER_MANDATORY_NUMERIC_CHECKING_validate'),
					     );		
						 				
	 $form['actandrules']['pdf_file']    = array (		
						      '#type' => 'file',
						      '#upload_validators'	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(125*1024*1024)),
						      '#upload_location' 	=> 'public://upload/content_act_rule_pdf/',
							  '#attributes' 		=> array('class' => array('')),	
							  '#prefix'				=> '<div class="row"><div class="form-custom col-md-12"><div class="col-md-4">',
		                      '#suffix'				=>  '</div></div></div>',
						      '#title' 				=> t(' Upload an content  file (JPG,PDF only)'),
					     );
						
	  
	 $form['actandrules']['submit']      = array (
						      '#type' 			=> 'submit',
						      '#value' 			=> 'Save',
							  '#attributes' => array('class' => array('btn btn-primary pull-left')),
							  '#prefix'	  => '<div class="col-md-6">',
		                      '#suffix'	  =>  '</div>',	 						
					     ); 
	 $form['actandrules']['cont_footer'] = array(
          '#type' 	=> 'markup',
		  '#markup' => '</div></div></div></div>',
          
          );
	return  $form; 
}



function add_actandrules_submit($form , &$form_states){	
	$val			= $form_states['values'];	
	
	/* Upload For Trade License */
	
	$arr_1									= array();
	$acts_nd_rules_pdf 							= file_load($val['pdf_file']);
	if( $acts_nd_rules_pdf != "" ){
		$arr_1 = explode("/", $acts_nd_rules_pdf->uri);
		$acts_nd_rules_pdf->status = FILE_STATUS_PERMANENT;
		file_save($acts_nd_rules_pdf);
		$acts_nd_rules_pdf_file = $arr_1[4];
	}
	
	$act 			=   $val['act'];
	$act_type 		=   $val['act_type'];
	$description 		=   $val['description'];
	$order_no 		=   $val['order_no'];
	
			
	$fields_Array	=	array(								
							'act_id'			=> $act, 
							'content_title' 		=> $ctitle,
							'content_desc' 			=> $description,		
							'act_file_type' 		=> $act_type,
							'content_typ' 			=> 'actrule',
							'ord' 				=> $order_no,
							'create_date'			=> time(),
							'acts_and_rules_pdf'		=> $acts_nd_rules_pdf_file,		
						   );					
	
	
	
	$query = db_insert('l_cms_content');
	$query->fields($fields_Array); 	
	$query->execute();
	
	$message = 'Saved successfully.';
	drupal_set_message(t($message));
}

function get_act_list(){
	$default_act 	= array(""=>"- Select act -");
	$opt_act_list 	= array();
	$act_list 		= db_select('l_act_master', 'lam'); 
	$act_list->fields('lam', array('id','act_title'));	
	$act_list->orderBy('lam.act_title', 'ASC');
	$act_list_arr 		= $act_list->execute();
	
	if(!empty($act_list_arr)) {      
		foreach($act_list_arr as $rows) {
			$opt_act_list[$rows->id] = t('@acttitle', array('@acttitle' => html_entity_decode($rows->act_title)));
		}    
	}	
	 
	if (isset($opt_act_list)) {
		return ($default_act + $opt_act_list);
	} else {
		return $default_act;
	}
}

function more_rules($file_type='', $act_id=''){
	 $rule_query = db_select('l_cms_content', 'lcc');	 
	 $rule_query->fields('lcc', array('content_id', 'acts_and_rules_pdf'));
	 $rule_query->condition('lcc.content_typ','actrule');
	 $rule_query->condition('lcc.act_id', encryption_decryption_fun('decrypt',$act_id));
	 $rule_query->condition('lcc.act_file_type', $file_type); 
	 $rule_query_result = $rule_query->execute();	 		 	 
	 $rows = array();
	 //$header = array();

  
	 $header = array(
			 array('data'=>'SL No.'),
			 array('data'=> 'List of '.$file_type),
			 array('data'=> 'Action')
			 );
    
	 $i	= 0;
	 foreach($rule_query_result as $data){
		  $i++;
		  global $base_root, $base_path;	
						
		  $rows[] = array(
				    $i,
				    $file_type.'-'.$i,
					l('<span class="fn_black"><i class="fa fa-download"></i></span>',''.$GLOBALS['base_url'].'/sites/default/files/upload/content_act_rule_pdf/'.$data->acts_and_rules_pdf, array('html' => TRUE,'attributes'=>array('target'=>'_blank' ,'title'=>'Click To Print/Download', 'class'=>array('#pdf-img')))).' '.l('<span class="fn_black"><i class="fa fa-edit"></i></span>', 'cmscontent/edit-actandrules/'.encryption_decryption_fun('encrypt',$data->content_id), array('html' => TRUE,'attributes'=>array('title'=>'Click To Edit')))		
				);   
	 }  
	 $addlink 	= $base_root.$base_path.'cmscontent/actandrules';
	 $markuplink	= l('Back', $addlink, array('html'=>true,'attributes'=>array('title'=>'Click To Back','class'=>array('button back-class'))));
	 
	 $variables 	= array(
	  			'attributes' 		=> array('class' => array('table table-striped table-responsive admin-custom-table')),
	  			'header' 		=> $header,
	  			'rows'			=> $rows
    			);
	
	 $output = theme('datatable', $variables);
	
	 $dic_detail_hlep_element = array(                      
					     //'#title'       => t('<b></b>'),                       
					     '#collapsible' => true, 
					     '#collapsed'  => true,
				    );
     
	 $fieldset_dic_help_link = theme('fieldset', array('element' => $dic_detail_hlep_element));
	 return '<div class="box box-primary"><div class="box-body">'.$fieldset_dic_help_link.$output.$markuplink.'</div></div>';
}

function edit_actandrules($form, &$form_state, $idenc=''){
	 global $base_url, $base_path, $base_root, $full_base;
	 $id		= encryption_decryption_fun('decrypt', $idenc);
	 $act_list 	= get_act_list();
	 
	 $act 		= '';
	 $act_type	= '';
	 $description	= '';
	 $file_id	= 0;
	 $order_no	= '';
	 
	 
	 
	 
	 if($id!=''){
		  $cms_actandrule_query = db_select('l_cms_content', 't');
		  //$cms_actandrule_query->join('file_managed', 'b', 't.fid=b.fid');
		  //$cms_actandrule_query->fields('b', array('filename'));
		  $cms_actandrule_query->fields('t', array('fid','acts_and_rules_pdf','content_id','content_title','meta_title', 'meta_desc', 'content_dt','content_desc', 'link_title', 'ord','is_active', 'is_new','act_id','act_file_type'));
		  $cms_actandrule_query->condition('t.content_id',$id);
		  $cms_actandrule_query_exc = $cms_actandrule_query->execute();	
		  $cms_actandrule_query_result = $cms_actandrule_query_exc->fetchAssoc();
		  
		  $act 		= $cms_actandrule_query_result['act_id'];
		  $act_type	= $cms_actandrule_query_result['act_file_type'];
		  $description	= $cms_actandrule_query_result['content_desc'];
		  $file_id	= $cms_actandrule_query_result['fid'];
		  $order_no	= $cms_actandrule_query_result['ord'];
	 }

	 $form['actandrules']['cont_head'] = array(
          '#type' 	=> 'markup',
		  '#markup' => '<div class="row"><div class="col-md-12"><div class="box box-primary"><div class="box-body">',
          
          );

	 $form['actandrules']['content_id'] = array(
					     '#type' 			=> 'hidden',
					     '#value' 			=> $id,
				    );
	 
	 
	 
	 
	 	
		  
	
	 $form['actandrules']['act'] = array(
					     '#title' 			=> 'Select Act',
					     '#type' 			=> 'select',
					     '#required' 		=> TRUE,
						  '#default_value' 		=> $act,
						 '#prefix'				=> '<div class="form-custom col-md-8"><label class="input">',
						 '#suffix'				=>  '</label></div>',
					     '#options' 		=> $act_list,
						 '#attributes' 		=> array('class' => array('form-control')),
					    
				    );
	 $form['actandrules']['act_type']    = array(
						  '#type' 			=> 'select',
						  '#title' 			=> 'Select type(Act/Rules/Notification)',
						  '#options' 		=>  array('' => 'Select any', 'ACT' =>t('ACTS'), 'RULES' =>t('RULES'),'Notification' =>t('Notification')),
						  '#default_value' 		=>  $act_type,
						  '#prefix'				=> '<div class="form-custom col-md-4"><label class="input">',
						  '#suffix'				=>  '</label></div>',
						  
						  '#attributes' 		=> array('class' => array('form-control')),
						  '#required' 		=> TRUE
					     ); 	
		    
	 $form['actandrules']['description'] = array(		
						      '#title'			=> 'Description',						
						      '#type' 			=> 'textarea',
							  '#attributes' 		=> array('class' => array('form-control')),	
							  '#prefix'				=> '<div class="form-custom col-md-12"><label class="input">',
							  '#suffix'				=>  '</label></div>',					
						      '#default_value' 		=> html_entity_decode($description),
					     );	
						 		
	$form['actandrules']['order_no']    = array(		
						      '#title'			=> 'Order No',
						      '#size'			=> t('7'),		
						      '#type' 			=> 'textfield',
						      '#maxlength' 		=> 4,						
						      '#required' 		=> TRUE,
							  '#attributes' 		=> array('class' => array('form-control')),	
		  					  '#prefix'				=> '<div class="form-custom col-md-12"><div class="form-custom col-md-4"><label class="input">',
		  					  '#suffix'				=>  '</label></div></div>',
							  '#default_value'		=> $order_no,
						      '#element_validate' 	=> array('ORDER_MANDATORY_NUMERIC_CHECKING_validate'),
					     );		
						 				
	 $form['actandrules']['pdf_file']    = array (		
						      '#type' => 'file',
						      '#upload_validators'	=> array('file_validate_extensions' => array('pdf'), 'file_validate_size' => array(125*1024*1024)),
						      '#upload_location' 	=> 'public://upload/content_act_rule_pdf/',
							  '#attributes' 		=> array('class' => array('')),	
							  '#process' 			=> array('import_my_file_element_process'),
							  '#prefix'				=> '<div class="col-md-12">',
		                      '#suffix'				=> '</div>',
						      '#title' 				=> t(' Upload an content  file (JPG,PDF only)'),


					     );

	 

	$form['actandrules']['submit']      = array (
						      '#type' 			=> 'submit',
						      '#value' 			=> 'Update',
							  '#attributes' 	=> array('class' => array('btn btn-primary pull-left')),
							  '#prefix'	  		=> '<div class="col-md-6">',
		                      '#suffix'	  		=>  '</div>',	 						
					     );

	$form['actandrules']['cont_footer'] = array(
					          '#type' 	=> 'markup',
							  '#markup' => '</div></div></div></div>',
          
          					); 


	return  $form;
}
function pdf_file_process($element, &$form_state, $form){
    $element = file_managed_file_process($element, $form_state, $form);
    $element['upload_button']['#access'] = FALSE;
    return $element;
}
function edit_actandrules_submit($form, &$form_state){
	 global $base_root, $base_path, $full_base;
	 $message		= "";
	 $txn 			= db_transaction();
	 $val 			= $form_state['values'];    
	 
	 $content_id	= $val['content_id'];
	 $act 			= $val['act'];
	 $act_type 		= $val['act_type'];
	 $description 		= htmlentities($val['description']['value']);
	 $order_no 		= trim($val['order_no']);
	 $file_info 		= file_load($val['pdf_file']);
	 $publication_file	= "";
	 $file_id 		= $val['file_id'];
	 
	 
	 try{
		 /* Update data of the table l_cm_content */
		  $updateArray	=   array(
					 'content_typ'		=> t('actrule'),
					 'content_desc'		=> $description,
					 'ord'			=> $order_no,
					 'act_file_type'	=> $act_type,
					 'is_active'		=> 'Y',
					 'create_date'		=> time()
				    );
		 
		
		 
		  if(!empty($file_info)){
			 $mime_type 				= mime_content_type($file_info->uri);
			 
			 /* Upload For Notification */
			 if(!empty($file_info) && (($mime_type=='image/jpeg') || ($mime_type=='application/pdf'))){
				 if(!empty($file_info)){
					 $file_uri 		= explode("/", $file_info->uri);
					 $act_and_rule_file 	= $file_uri[4];
					 $file_info->status 	= FILE_STATUS_PERMANENT;
					 file_save($file_info);
					 $original_file_name 	= $file_info->filename;
					 $updateArray['fid']	= trim($file_info->fid);
					 $updateArray['acts_and_rules_pdf'] = $act_and_rule_file;
				 }
			 }
		  }
		  	 
		  
		 $rs = db_update('l_cms_content')->fields($updateArray)->condition('content_id',$content_id)->execute();
		 
		 $message 	= 'Act and Rule Update Successfully';
		 $full_base = $base_root.$base_path.'cmscontent/actandrules';
		 drupal_set_message(t($message));
		 drupal_goto($full_base);
	 }catch(Exception $e){
		 $txn->rollback();
		 watchdog_exception('type', $e);
		 $message = 'Act and Rule Not Updated ';		
		 $full_base = $base_root.$base_path.'cmscontent/actandrules';
		 drupal_set_message(t($message));
		 drupal_goto($full_base);
	 } 
}
?>