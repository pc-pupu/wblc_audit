<?php
function get_acts_rules_details($param = ''){
	global $base_url;
	$result = db_query("select t.*, fm.filename, fm.uri  from ( select n.nid, n.title, fdb.body_value,frfauf.field_act_upload_file_fid from node as n
						inner join field_data_body as fdb on n.nid = fdb.entity_id
						inner join field_revision_field_act_upload_file as frfauf on n.nid = frfauf.entity_id
						where n.type = 'acts_rules' and n.nid = '".$param."') as t
						inner join file_managed as fm on t.field_act_upload_file_fid = fm.fid order by t.nid asc");	
    $record = $result->fetchObject();
	
	$output = '';
	
	$output .= '<h1 id="page-title" class="title">'.$record->title.'</h1>';
	
	$output .= $record->body_value;
	
	if(!empty($record->filename)){
		$output .= '<br><p class="rteright" style="font-family:verdana,geneva,sans-serif; size:15px; font-weight:700;">'.l('<i>Click here to download the pdf version</i>', $base_url.'/sites/default/files/'.$record->filename, array('attributes' => array('target' => '_blank'))).'</p>';
	}
	
	 $output = html_entity_decode($output);

	 return $output;
}
?>