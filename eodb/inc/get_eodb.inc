<?php
function get_eodb() {
	global $base_url;
   $sort = 'DESC';
   $order = 'ord';
   $cms_eodb_query = db_select('l_cms_content', 't');
   $cms_eodb_query->join('file_managed', 'b', 't.fid=b.fid');
   $cms_eodb_query->fields('b', array('filename'));
   $cms_eodb_query->fields('t', array('fid','content_id','content_title','content_dt','content_desc', 'ord','is_active', 'is_new','show_date'));
   $cms_eodb_query->condition('t.content_typ','eodb','=');
   $cms_eodb_query->condition('t.is_active','Y','=');
   $cms_eodb_query->orderBy($order, $sort);
   $cms_eodb_query_result = $cms_eodb_query->execute();	
   
   $output = '';
   $output .='<div class="row">
				  <div class="col-sm-12">										
					  <div id="content-2" class="scrollContent content mCustomScrollbar light" data-mcs-theme="minimal-dark">
						  <table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table fixed_headers">
						  	<thead>
                      			<tr>
                        			<th width="8%">Sl. No.</th>
									<th width="15%">Notification Date</th>
                        			<th>Description</th>
                        			<th width="10%">Download</th>
                      			</tr>';
	$i = 0;	
	
   	foreach($cms_eodb_query_result as $data){
		$i++;
		global $base_root, $base_path;
		
		if($data->is_new == 'Y'){
			$new_image = '<img src="'.$base_root.$base_path.'sites/default/files/new_gif_icon.gif" style="border:none;">';
		}else{
			$new_image = '';
		}
		
		$aa = $base_root.$base_path.'sites/default/files/contentpdf/'.$data->filename;
		$link_s = l('<img title="Click here to download and print." src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'lcTheme').'/images/pdf.png" style="border:none;">', $aa, array('html' =>TRUE, 'attributes' => array('target' => '_blank')));
		
		if($data->show_date !=''){
			$dtshow = date("d/m/Y",strtotime($data->show_date));
		}else{
			$dtshow = '';
		}
		
		
		$output .= '<tr>';
		$output .= '<td>'.$i.'</td>';
		$output .= '<td>'.$dtshow.'</td>';
		$output .= '<td>'.$data->content_title.'&nbsp;'.$new_image.'</td>';
		$output .= '<td  style="text-align:center;">'.$link_s.'</td>';
		$output .= '</tr>';
	} 
	
	$output .= '</tbody></table></div></div></div>';

	return $output;
}
 
?>									
					
                   