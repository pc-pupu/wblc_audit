<?php
function get_acts_rules_notification(){		
	global $base_url;
	
	$valid_period	= encryption_decryption_fun('decrypt', $valid_period);	
	
  	// Table header
  	$header = array(
    		array('data' => 'Sl No'),
    		array('data' => 'ACT'),
			array('data' => 'RULES'),		
			array('data' => 'Notification'),
  			);
	  

  $act_circular_query = db_select('l_act_master', 'lam');	 
  $act_circular_query->fields('lam', array('id', 'act_title'));
  $act_circular_query->condition('lam.is_active','Y');
  $act_circular_query->orderby('act_title', 'ASC');	 
  $act_circular_query_result = $act_circular_query->execute();


  $rows = array();						
  $count = 0;
  						
  // Looping for filling the table rows
	 
  while($data = $act_circular_query_result->fetchAssoc()){
	  $count++;
	  
	  $act_file_name   = '';
	  $rules_file_name = '';
	  $noti_file_name  = '';
	  
	  $act_download    = '';
	  $rules_download  = '--';
	  $noti_download   = '--';
	 
	  $act_file_name  = get_act_rule_noti_file_name($data['id'], 'ACT');
	  $download_act_file_path = $base_url.'/sites/default/files/upload/content_act_rule_pdf/'.$act_file_name;
	  
	  if(file_exists($_SERVER['DOCUMENT_ROOT'].'/sites/default/files/upload/content_act_rule_pdf/'.$act_file_name)){	
	  	  $act_download  = l($data['act_title'], $download_act_file_path, array('html' => TRUE, 'attributes'=>array('target'=>'_blank' ,'title'=>'Click To Print/Download')));
	  }else{
		   $act_download  = $data['act_title'];
	  }
	  
	  $rule_count = get_act_rule_noti_count($data['id'], 'RULES');
	  
	  if($rule_count > 1){	  	   
	  	   $download_rule_file_path = 'list-of-rules/'.encryption_decryption_fun('encrypt',$data['id']);
		   $rules_download = l('VIEW', $download_rule_file_path, array('html' => TRUE, 'attributes' => array('title' =>'Click here to view details', 'target' => '_blank')));
	  }else if($rule_count == 1){      
		   $rules_file_name  = get_act_rule_noti_file_name($data['id'], 'RULES');	 
	  	   $download_rule_file_path = $base_url.'/sites/default/files/upload/content_act_rule_pdf/'.$rules_file_name;
		   
		   if(file_exists($_SERVER['DOCUMENT_ROOT'].'/sites/default/files/upload/content_act_rule_pdf/'.$rules_file_name)){		
				$rules_download = l('VIEW', $download_rule_file_path, array('html' => TRUE, 'attributes' => array('title' =>'Click here to view details', 'target' => '_blank')));
		   }
	  }else{
		  $rules_download = '---';
	  }
	  
	  $noti_count = get_act_rule_noti_count($data['id'], 'Notification');
	  
	  if($noti_count > 1){	  	   
	  	   $download_noti_file_path = 'list-of-notification/'.encryption_decryption_fun('encrypt',$data['id']);
		   $noti_download = l('VIEW', $download_noti_file_path, array('html' => TRUE, 'attributes' => array('title' =>'Click here to view details', 'target' => '_blank')));
	  }else if($noti_count == 1){
		   $noti_file_name  = get_act_rule_noti_file_name($data['id'], 'Notification');	 
	  	   $download_noti_file_path = $base_url.'/sites/default/files/upload/content_act_rule_pdf/'.$noti_file_name;
		   
		   if(file_exists($_SERVER['DOCUMENT_ROOT'].'/sites/default/files/upload/content_act_rule_pdf/'.$noti_file_name)){		
				$noti_download = l('VIEW', $download_noti_file_path, array('html' => TRUE, 'attributes' => array('title' =>'Click here to view details', 'target' => '_blank')));
		   }
	  }else{
		  $noti_download = '---';
	  }
	
	$rows[] = array(
				array('data' => $count),
				array('data' => $act_download),
			    array('data' => $rules_download),				
				array('data' => $noti_download),	
		);		
    }
     
    $per_page = 30;
	
	// Initialize the pager
	$current_page = pager_default_initialize(count($rows), $per_page);
	
	// Split your list into page sized chunks
	$chunks = array_chunk($rows, $per_page, TRUE);
	
	// Show the appropriate items from the list
	$output = theme('table', array('header' => $header, 'rows' => $chunks[$current_page]));
	
	// Show the pager
	$output .= theme('pager', array('quantity',count($rows)));
	
	// Returning the output		
	
    return '<div id="synopId" class="datagrid">'.$output.'</div>';	
}

function get_all_rules($act_id = ''){
	$act_id	= encryption_decryption_fun('decrypt', $act_id);
	
	$header = array(
    		array('data' => 'Sl No'),
    		array('data' => 'RULE'),
			array('data' => 'DOWNLOAD'),			
  			);
			
	$act_query = db_select('l_cms_content', 'cms');
	$act_query->leftJoin('l_act_master', 'lam', 'lam.id = cms.act_id');
	$act_query-> fields('lam', array('act_title')); 
	$act_query->fields('cms', array('content_id', 'acts_and_rules_pdf'));
	$act_query->condition('cms.content_typ','actrule');
	$act_query->condition('cms.act_id',$act_id);
	$act_query->condition('cms.act_file_type', 'RULES'); 
	$act_query = $act_query->execute();
	if($act_query->rowCount() > 0){		
		while($data = $act_query->fetchAssoc()){
			
	  		$count++;
					
			$rule_download = 'X'; 
			 
		    if(file_exists($_SERVER['DOCUMENT_ROOT'].'/sites/default/files/upload/content_act_rule_pdf/'.$data['acts_and_rules_pdf'])){	
			    
				$download_rule_file_path = $base_url.'/sites/default/files/upload/content_act_rule_pdf/'.$data['acts_and_rules_pdf'];	
				$rule_download = l('<img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'lcTheme').'/images/pdf.png" style="border:none;">', $download_rule_file_path, array('html' => TRUE, 'attributes' => array('title' =>'Click here to view details', 'target' => '_blank')));
		    }		   
	  
			$rows[] = array(
					array('data' => $count),
					array('data' => $data['act_title']),
			    	array('data' => $rule_download),					
			);
		}		
	}
	
	$output = array( 'header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('view-act-rules-table')), 'empty' => t("No data found!"));	 
	 	 					
	$output = 	theme('datatable', $output);
	
	return $output;	
}

function get_all_notification($act_id = ''){
	$act_id	= encryption_decryption_fun('decrypt', $act_id);
	
	$header = array(
    		array('data' => 'Sl No'),
    		array('data' => 'NOTIFICATION'),
			array('data' => 'DOWNLOAD'),			
  			);
			
	$act_query = db_select('l_cms_content', 'cms');
	$act_query->leftJoin('l_act_master', 'lam', 'lam.id = cms.act_id');
	$act_query-> fields('lam', array('act_title')); 
	$act_query->fields('cms', array('content_id', 'acts_and_rules_pdf','content_title'));
	$act_query->condition('cms.content_typ','actrule');
	$act_query->condition('cms.act_id',$act_id);
	$act_query->condition('cms.act_file_type', 'Notification'); 
	$act_query = $act_query->execute();
	if($act_query->rowCount() > 0){		
		while($data = $act_query->fetchAssoc()){
			
	  		$count++;
					
			$rule_download = 'X'; 
			 
		    //if(file_exists($_SERVER['DOCUMENT_ROOT'].'/sites/default/files/upload/content_act_rule_pdf/'.$data['acts_and_rules_pdf'])){	
			    
				$download_rule_file_path = $base_url.'/sites/default/files/upload/content_act_rule_pdf/'.$data['acts_and_rules_pdf'];	
				$noti_download = l('<img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'lcTheme').'/images/pdf.png" style="border:none;">', $download_rule_file_path, array('html' => TRUE, 'attributes' => array('title' =>'Click here to view details', 'target' => '_blank')));
		   // }		   
			if($data['content_id'] == 780 || $data['content_id'] == 793 ){
				$act_title = $data['content_title'];
			}else{
				$act_title = $data['act_title'];
			}
			$rows[] = array(
					array('data' => $count),
					array('data' => $act_title),
			    	array('data' => $noti_download),					
			);
			
			
		}		
	}
		
	$output = array( 'header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('view-act-rules-table')), 'empty' => t("No data found!"));	 
	 	 					
	 $output = 	theme('datatable', $output);
	 
	 return $output;
}

function get_act_rule_noti_file_name($id = '', $act_file_type){
	$act_query = db_select('l_cms_content', 'cms');	 
	$act_query->fields('cms', array('content_id', 'acts_and_rules_pdf'));
	$act_query->condition('cms.content_typ','actrule');
	$act_query->condition('cms.act_id',$id);
	$act_query->condition('cms.act_file_type', $act_file_type); 
	$act_query = $act_query->execute();
	if($act_query->rowCount() > 0){
		$act_result = $act_query->fetchObject();
		return $act_result->acts_and_rules_pdf;	
	}else{
		return NULL;
	}	
}

function get_act_rule_noti_count($id = '', $act_file_type){
	$act_query = db_select('l_cms_content', 'cms');	 
	$act_query->fields('cms', array('content_id', 'acts_and_rules_pdf'));
	$act_query->condition('cms.content_typ','actrule');
	$act_query->condition('cms.act_id',$id);
	$act_query->condition('cms.act_file_type', $act_file_type); 
	$act_query = $act_query->execute();
	if($act_query->rowCount() > 0){
		return $act_query->rowCount();		
	}else{
		return 0;
	}	
}
?>