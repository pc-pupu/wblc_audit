<?php

/*==============  List Start ============*/

function board_info($content_type){
	global $base_root, $base_path;
	$query = db_select('l_cms_content', 't');
    $query->fields('t', array('content_id', 'content_title', 'show_date', 'acts_and_rules_pdf'));
    $query->join('file_managed','fm','fm.fid = t.fid');
	$query->fields('fm',array('filename'));
	$query->condition('t.content_typ', $content_type, '=');
	$query->condition('t.is_active', 'Y', '=');
    $query->orderBy('show_date', 'DESC');
    $result = $query->execute();
		
	$header = array(
					array('data' => 'SL.NO.', 'width' => '10%'),
					array('data' => 'Details'),
					array('data' => 'Meeting Date','width' => '15%'),
					array('data' => 'Download' ,'width' => '10%')
	);	
	$i = 0;			
	foreach ($result as $data) {
		
        $i++;
		
		$p_link = 'sites/default/files/contentpdf/'.str_replace(' ','_',$data->filename);
		
        $link_p = l('<img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'lcTheme').'/images/pdf.png" style="border:none;">', $p_link, array( 'html' => TRUE,'attributes' => array( 'title' => 'Click to Remove', 'target' => '_blank' ) ));
        
		
		$rows[] = array(
					array('data' => $i, 'align' => 'left', 'class' => array('odd')),
					array('data' => $data->content_title),
					array('data' => date('d/m/y',strtotime($data->show_date))),
					array('data' => $link_p)
				  );
				  
	}
	$variables = array(
					'attributes' => array('class' => array('view-act-rules-table fixed_headers')), 
					'header' => $header,
					'rows' => $rows,
					'empty' => t("Data not available.")
				  );
	
	
	$output .= theme('datatable', $variables);
	return $output;
}


