<?php

function synopsys_page() {
	
	global $base_url;
	
	
 // Table header
  $header = array(
    array('data' => 'Sl No'),
    array('data' => 'Scheduled Employments'),
	array('data' => 'Area'),
	array('data' => 'Unskilled'),
	array('data' => 'Semi-skilled'),
	array('data' => 'Skilled'),
	array('data' => 'Highly Skilled'),
	array('data' => 'Valid upto'),
	// array('data' => 'Download'),
  );
	
	/*$result = db_query("select t.*, fm.filename, fm.uri  from (select n.nid, n.title, fdb.body_value,fdfa.field_area_value,fdfus.field_unskilled_rs__value,fdskr.field_semi_skilled_rs__value,fdfsr.field_skilled_rs__value,frfhsr.field_highly_skilled_rs__value,frfvup.field_valid_upto_value,fdfr.field_remarks_value,fwuff.field_wages_upload_file_fid from node as n
                        inner join field_data_body as fdb on n.nid = fdb.entity_id
						inner join field_data_field_area as fdfa on n.nid = fdfa.entity_id
						inner join field_data_field_unskilled_rs_ as fdfus on n.nid = fdfus.entity_id
						inner join field_data_field_semi_skilled_rs_ as fdskr on n.nid = fdskr.entity_id
						inner join field_data_field_skilled_rs_ as fdfsr on n.nid = fdfsr.entity_id
						inner join field_revision_field_highly_skilled_rs_ as frfhsr on n.nid = frfhsr.entity_id
						inner join field_revision_field_valid_upto as frfvup on n.nid = frfvup.entity_id
						inner join field_data_field_remarks as fdfr on n.nid = fdfr.entity_id
						inner join field_data_field_wages_upload_file as fwuff on n.nid = fwuff.entity_id
                        where n.type = 'wages_contents')as t
						inner join file_managed as fm on t.field_wages_upload_file_fid = fm.fid order by t.nid asc");*/	
  

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
					  left join file_managed as fm on t.field_wages_upload_file_fid = fm.fid order by t.nid asc");


  $rows = array();						
  $count = 0;						
	// Looping for filling the table rows 
  while($data = $result->fetchObject()) {
	 $count++;
	 
	 $download = l('Download File', 'download/'.$data->filename );
	 
    // Adding the rows
  /*  $rows[] = array($count, $data->title, $data->field_area_value, $data->field_unskilled_rs__value, $data->field_semi_skilled_rs__value, $data->field_skilled_rs__value, $data->field_highly_skilled_rs__value, $data->field_valid_upto_value, $download.$data->field_remarks_value);*/
	
	
	if($data->field_remarks_value != '--') {
		$rows[] = array(
			array('data' => $count),
			array('data' => $data->title),
			array('data' => $data->field_remarks_value, 'colspan' => 7, 'class' => array('remarks_align')),
			//array('data' => $data->field_unskilled_rs__value),
			//array('data' => $data->field_semi_skilled_rs__value),
			//array('data' => $data->field_skilled_rs__value),
			//array('data' => $data->field_highly_skilled_rs__value),
			//array('data' => $data->field_valid_upto_value),
			//array('data' => $download),	
		);
		
	}else {
		$rows[] = array(
			array('data' => $count),
			array('data' => $data->title),
			array('data' => $data->field_area_value),
			array('data' => $data->field_unskilled_rs__value),
			array('data' => $data->field_semi_skilled_rs__value),
			array('data' => $data->field_skilled_rs__value),
			array('data' => $data->field_highly_skilled_rs__value),
			array('data' => $data->field_valid_upto_value),
			// array('data' => $download),	
		);	
	 }	
   }
     
    $per_page = 20;
	// Initialize the pager
	$current_page = pager_default_initialize(count($rows), $per_page);
	// Split your list into page sized chunks
	$chunks = array_chunk($rows, $per_page, TRUE);
	// Show the appropriate items from the list
	$output = theme('table', array('header' => $header, 'rows' => $chunks[$current_page], 'attributes' => array('class' => array('view-act-rules-table'))));
	// Show the pager
	$output .= theme('pager', array('quantity',count($rows)));
	
	// Returning the output
    return '<div id="synopId" class="datagrid">'.$output.'</div>';
						
						
	/*$record = $result->fetchObject();							
	$output  = '<div id="ministry_main" class="CSSTableGenerator">';
	$output .= '<table width="100%" border="1" cellspacing="5" cellpadding="5">';
	$output .= '<tr><td>Sl No.</td><td>Title</td><td>Download</td>
                  </tr>';
	 
	$output .= '</table>';
	$output .= '</div>';					
	return $output;*/ 
	
}
