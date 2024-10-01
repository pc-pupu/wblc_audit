<?php

function synopsys_pages($month = '', $year = '') {
	$header = array(
			array('data' => 'Sl. No.', 'width' => '10%'),
			array('data' => 'SCHEDULED EMPLOYMENTS', 'style' => ''),		
			array('data' => 'DOWNLOAD', 'field' => 'actions','width' => '15%')
    );
	
	$mw_query = db_select('l_min_wages_documents', 'mw');	
	$mw_query->fields('mw',array('year', 'month', 'node_type_name', 'file'));
	$mw_query->condition('mw.month', trim($month), '=');
	$mw_query->condition('mw.year', trim($year), '=');
	$mw_query->condition('mw.is_active', 1, '=');		
	$mw_query->orderby('mw.node_type_name', 'asc');	
	$mw_result = $mw_query->execute();
	
	if($mw_result->rowCount() > 0){
		$count = 1;
		foreach($mw_result as $mw_data){
			$foldername = strtolower($mw_data->month).'-'.$mw_data->year;	
			$download_file_path = '/sites/default/files/upload/min_wages/'.$foldername.'/'.trim($mw_data->file);		
			if(($mw_data->file != '') && (file_exists($_SERVER['DOCUMENT_ROOT'].$download_file_path))){								
				$bb = $GLOBALS['base_url'].'/sites/default/files/upload/min_wages/'.$foldername.'/'.$mw_data->file;			
	    		$pdf_image = $GLOBALS['base_url'].'/sites/all/modules/cmscontent/images/pdf.png';			
				$download = l("<img src='$pdf_image'/>", $bb, array('html'=>TRUE,'attributes' => array('target'=> '_blank', 'class' => array('aa'))));					
			}else{			
				$download = 'Minimum rates wages to be uploaded soon';							
			}
			
			$rows[] = array(
					array('data' => $count++),
					array('data' => strtoupper($mw_data->node_type_name)),							
					array('data' => $download, 'style' => 'text-align:center;'),	
				);
		}
	}
	
	$variables = array(
			   'attributes' 		=> array('class' => array('view-act-rules-table')),
			   'header' 			=> $header,
			   'rows'				=> $rows,
		  );
	
	$view_details = '';
	  
	if(file_exists($_SERVER['DOCUMENT_ROOT'].'/sites/default/files/synopsys/'.$valid_period.'/synopsis-of-minimum-rates-of-wages-in-scheduled-employments in-west-bengal-as-on-'.$valid_period.'.pdf')){
	   $view_details = l('<i class="fa fa-file" aria-hidden="true"></i> Click Here To View Synopsis', $base_url.'/sites/default/files/synopsys/'.$valid_period.'/synopsis-of-minimum-rates-of-wages-in-scheduled-employments in-west-bengal-as-on-'.$valid_period.'.pdf', array('html' => TRUE, 'attributes' => array('target' => '_blank')));
	}
	
	$output = theme('datatable', $variables);
	
	return '<div class = "field-items">'.$output.'<p class="pdf-link">'.$view_details.'</p>'.'</div>';	
}

/*function synopsys_pages1($fn = '', $valid_period = '') {
		
	global $base_url;	
	
	$valid_period	= encryption_decryption_fun('decrypt', $valid_period);	
	
  // Table header
  $header = array(
			array('data' => 'Sl. No.', 'width' => ''),
			array('data' => 'SCHEDULED EMPLOYMENTS', 'style' => ''),		
			array('data' => 'DOWNLOAD', 'field' => 'actions','width' => '')
  );
	  

 $result = db_query("select t.*, fm.filename, fm.uri  from (select n.nid, n.title, fdb.body_value,fdfa.field_area_value,fdfus.field_unskilled_rs__value,fdskr.field_semi_skilled_rs__value,fdfsr.field_skilled_rs__value,frfhsr.field_highly_skilled_rs__value,frfvup.field_valid_upto_value,fdfr.field_remarks_value,fwuff.field_wages_upload_file_fid from node as n
					  left join field_data_body as fdb on n.nid = fdb.entity_id
					  left join field_data_field_area as fdfa on n.nid = fdfa.entity_id
					  left join field_data_field_unskilled_rs_ as fdfus on n.nid = fdfus.entity_id
					  left join field_data_field_semi_skilled_rs_ as fdskr on n.nid = fdskr.entity_id
					  left join field_data_field_skilled_rs_ as fdfsr on n.nid = fdfsr.entity_id
					  left join field_revision_field_highly_skilled_rs_ as frfhsr on n.nid = frfhsr.entity_id
					  left join field_revision_field_valid_upto as frfvup on n.nid = frfvup.entity_id
					  left join field_data_field_remarks as fdfr on n.nid = fdfr.entity_id
					  left join field_data_field_wages_upload_file as fwuff on n.nid = fwuff.entity_id
					  where n.type = 'wages_contents' and n.status = 1)as t
					  left join file_managed as fm on t.field_wages_upload_file_fid = fm.fid order by t.title asc");


  $rows = array();						
  $count = 0;						
	// Looping for filling the table rows 
  while($data = $result->fetchObject()){	 
	 $download = '--';
	 $file_name = str_replace(",", " ", strtolower(trim($data->title)));
	 $file_name = str_replace(" ", "-", $file_name);
	 $file_name = str_replace("(", "-", $file_name);
	 $file_name = str_replace(")", "-", $file_name);
	 $file_name = str_replace("&", "and", $file_name);
	 $file_name = str_replace("/", "-", $file_name);
	 $file_name = str_replace(".", "-", $file_name);
	 $file_name = str_replace("--", "-", $file_name);
	 
	 $download_file_path = '/sites/default/files/synopsys/'.$valid_period.'/'.$file_name.'.pdf';
	
	 if(file_exists($_SERVER['DOCUMENT_ROOT'].$download_file_path)){	 	
		$count++;
		$download = l('<img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'lcTheme').'/images/pdf.png">', $base_url.$download_file_path, array('html' => TRUE, 'attributes' => array('title' =>'Click here to view & download file', 'target' => '_blank')));
	 
	
	$rows[] = array(
					array('data' => $count),
					array('data' => strtoupper($data->title)),							
					array('data' => $download, 'style' => 'text-align:center;'),	
				);
	  }	
    }
	
	
	$variables = array(
			   'attributes' 		=> array('class' => array('view-act-rules-table')),
			   'header' 			=> $header,
			   'rows'				=> $rows,
		  );
	
	 $output = theme('datatable', $variables);
	 
	 $dic_detail_hlep_element = t('<h1 class="inner-page-title">Synopsis of Minimum Rates of Wages in Scheduled Employments in West Bengal as on '.$valid_period.'.</h1>');
	 
	 $view_details = '';
	  
	 if(file_exists($_SERVER['DOCUMENT_ROOT'].'/sites/default/files/synopsys/'.$valid_period.'/synopsis-of-minimum-rates-of-wages-in-scheduled-employments in-west-bengal-as-on-'.$valid_period.'.pdf')){
	 	$view_details = l('<i class="fa fa-file" aria-hidden="true"></i> Click Here To View Synopsis', $base_url.'/sites/default/files/synopsys/'.$valid_period.'/synopsis-of-minimum-rates-of-wages-in-scheduled-employments in-west-bengal-as-on-'.$valid_period.'.pdf', array('html' => TRUE, 'attributes' => array('target' => '_blank')));
	 }
	 
	 return $dic_detail_hlep_element.'<div class = "field-items">'.$output.'<p class="pdf-link">'.$view_details.'</p>'.'</div>';
}
*/