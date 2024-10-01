<?php
function get_acts_rules() {
	
	global $base_url;
	// $temp = 1;	
	/*$result = db_query("select t.*, fm.filename, fm.uri  from ( select n.nid, n.norder, n.title, fdb.body_value,frfauf.field_act_upload_file_fid from node as n
												inner join field_data_body as fdb on n.nid = fdb.entity_id
												inner join field_revision_field_act_upload_file as frfauf on n.nid = frfauf.entity_id
												where n.type = 'acts_rules') as t
												inner join file_managed as fm on t.field_act_upload_file_fid = fm.fid order by t.norder asc");	*/


  $output	 = '';

  $act_circular_query = db_select('l_act_master', 'lam');	 
  $act_circular_query->fields('lam', array('id', 'act_title'));
  $act_circular_query->condition('lam.is_active','Y');
  $act_circular_query->orderby('act_title', 'ASC');	 
  $act_circular_query_result = $act_circular_query->execute();


    $rows = array();						
    $count = 0;
	$output .='<div class="row">
				  <div class="col-sm-12">
					<div class="center">
					  <h2 class="inner-page-title">Acts and Rules</h2>
					  <hr> 
					  <div id="content-2" class="scrollContent content mCustomScrollbar light" data-mcs-theme="minimal-dark">
						  <table width="100%" border="1" cellspacing="0" cellpadding="0" class="view-act-rules-table fixed_headers">
						  	<thead>
								 <tr>
									<th width="65">Sl. No.</th>
									<th>ACT</th>
									<th>RULES</th>
									<th>Notification</th>
								  </tr> 
							</thead>
							<tbody>';
	
						
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
		   $rules_download = l('<img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'lcTheme').'/images/pdf.png" style="border:none;">', $download_rule_file_path, array('html' => TRUE, 'attributes' => array('title' =>'Click here to view details', 'target' => '_blank')));
		}else if($rule_count == 1){      
		   $rules_file_name  = get_act_rule_noti_file_name($data['id'], 'RULES');	 
		   $download_rule_file_path = $base_url.'/sites/default/files/upload/content_act_rule_pdf/'.$rules_file_name;
		   
		   if(file_exists($_SERVER['DOCUMENT_ROOT'].'/sites/default/files/upload/content_act_rule_pdf/'.$rules_file_name)){		
				$rules_download = l('<img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'lcTheme').'/images/pdf.png" style="border:none;">', $download_rule_file_path, array('html' => TRUE, 'attributes' => array('title' =>'Click here to view details', 'target' => '_blank')));
		   }
		}else{
		  $rules_download = '---';
		}
		
		$noti_count = get_act_rule_noti_count($data['id'], 'Notification');
		
		if($noti_count > 1){	  	   
		   $download_noti_file_path = 'list-of-notification/'.encryption_decryption_fun('encrypt',$data['id']);
		   $noti_download = l('<img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'lcTheme').'/images/pdf.png" style="border:none;">', $download_noti_file_path, array('html' => TRUE, 'attributes' => array('title' =>'Click here to view details', 'target' => '_blank')));
		}else if($noti_count == 1){
		   $noti_file_name  = get_act_rule_noti_file_name($data['id'], 'Notification');	 
		   $download_noti_file_path = $base_url.'/sites/default/files/upload/content_act_rule_pdf/'.$noti_file_name;
		   
		   if(file_exists($_SERVER['DOCUMENT_ROOT'].'/sites/default/files/upload/content_act_rule_pdf/'.$noti_file_name)){		
				$noti_download = l('<img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'lcTheme').'/images/pdf.png" style="border:none;">', $download_noti_file_path, array('html' => TRUE, 'attributes' => array('title' =>'Click here to view details', 'target' => '_blank')));
		   }
		}else{
		  $noti_download = '---';
		}
							   
						
	$output .= '<tr>';
	$output .= '<td class="sl-no">'.$count.'.</td>';
	$output .= '<td>'.$act_download.'</td>';
	$output .= '<td style="text-align:center;">'.$rules_download.'</td>';
	$output .= '<td style="text-align:center;">'.$noti_download.'</td>';
	//$output .= '<td class="view"><a href="acts-rules/'.encryption_decryption_fun('encrypt',$record->nid).'" title="View"><i class="fa fa-eye" aria-hidden="true"></i></a>';
	//.l(t('<i class="fa fa-eye" aria-hidden="true"></i>'), 'acts-rules/'.$record->nid,array('attributes' => array('html'=>TRUE, 'class'=>array()))).'</td>';
	$output .= '</tr>';
	$temp++;
} 
$output .=					'</tbody>
		  				  </table>
	  				  </div>
					</div>
  				</div>
			  </div>';

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
					
                   