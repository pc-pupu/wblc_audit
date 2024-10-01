<?php
function get_statistical_listing() {
	global $base_url;
   $result = db_query("select t.*, fm.filename, fm.uri  from ( select n.nid, n.title, fdb.body_value,fdfuf.field_upload_file_fid from node as n
							inner join field_data_body as fdb on n.nid = fdb.entity_id
							inner join field_data_field_upload_file as fdfuf on n.nid = fdfuf.entity_id
							where n.type = 'statistics') as t
							inner join file_managed as fm on t.field_upload_file_fid = fm.fid order by t.nid asc");	
   
   $output = '';
   $output .='<div class="row">
				  <div class="col-sm-12">										
					  <div id="content-2" class="scrollContent content mCustomScrollbar light" data-mcs-theme="minimal-dark">
						  <table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table fixed_headers">
						  	<thead>
                      			<tr>
                        			<th>Sl. No.</th>
                        			<th>Title</th>
                        			<th>Download</th>
                      			</tr>';
	$i = 0;	
	
   	$temp = 1;
	while($record = $result->fetchObject()){
		$i++;				
		$link_s = l('<img title="Click here to download and print." src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'lcTheme').'/images/pdf.png" style="border:none;">', $base_url.'/sites/default/files/'.$record->filename, array('html' =>TRUE, 'attributes' => array('target' => '_blank')));
		
		$output .= '<tr>';
		$output .= '<td>'.$i.'</td>';
		$output .= '<td>'.$temp.'&nbsp;'.$new_image.'</td>';
		$output .= '<td  style="text-align:center;">'.$link_s.'</td>';
		$output .= '</tr>';
	} 
	
	$output .= '</tbody></table></div></div></div>';

	return $output;
}
 
?>									
					
                   