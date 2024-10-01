<?php
function get_fawloi_notice() {
	global $base_url;
	$sort = 'DESC';
	$order = 'ord';
	$fawloi_query = db_select('l_cms_content', 't');
	$fawloi_query->join('file_managed', 'b', 't.fid=b.fid');
	$fawloi_query->fields('b', array('filename'));
	$fawloi_query->fields('t', array('fid','content_id','content_title','content_dt','content_desc', 'ord','is_active', 'is_new'));
	$fawloi_query->condition('t.content_typ','fawloi','=');
	
	$fawloi_query->orderBy($order, $sort);
	$fawloi_result = $fawloi_query->execute();
	
	
	/******** Screening Committee Decision ********* */
	$fawloi_sc_com_query = db_select('l_cms_content', 't');
	$fawloi_sc_com_query->join('file_managed', 'b', 't.fid=b.fid');
	$fawloi_sc_com_query->fields('b', array('filename'));
	$fawloi_sc_com_query->fields('t', array('fid','content_id','content_title','content_dt','content_desc', 'ord','is_active', 'is_new'));
	$fawloi_sc_com_query->condition('t.content_typ','fawloi_sc_com','=');
	
	$fawloi_sc_com_query->orderBy('ord', 'desc');
	$fawloi_sc_com_result = $fawloi_sc_com_query->execute();
	
		
   
   $output = '';
   
   $output .='<div class="row">
   				<div class="col-md-12">
						<table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table fixed_headers">
						  	<thead>
                      			<tr>
                        			<th width="10%"><strong>Sl. No.</strong></th>
                        			<th><strong>Title</strong></th>
                        			<th style="text-align:center;" width="15%"><strong>Download</strong></th>
                      			</tr>
							</thead>
							<tbody>';
	$i = $j = 0;	
	
   	foreach($fawloi_result as $data){
		$i++;
		global $base_root, $base_path;
		
		if($data->is_new == 'Y'){
			//$new_image = '<img src="'.$base_root.$base_path.'sites/default/files/new_gif_icon.gif" style="border:none;">';
		}else{
			$new_image = '';
		}
		
		$aa = $base_root.$base_path.'sites/default/files/contentpdf/'.$data->filename;
		
		// $link_s = l(t('View'), $aa, array('attributes' => array('target'=>'_blank')));
		
		$link_s = l('<img title="Click here to download and print." src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'lcTheme').'/images/pdf.png" style="border:none;">', $aa, array('html' =>TRUE, 'attributes' => array('target' => '_blank')));
		
		$output .= '<tr>';
		$output .= '<td>'.$i.'</td>';
		$output .= '<td>'.$data->content_title.'&nbsp;'.$new_image.'</td>';
		$output .= '<td  style="text-align:center;">'.$link_s.'</td>';
		$output .= '</tr>';
	}					
							
	$output .='				</tbody>
						</table>
				</div>	
					
				<div class="col-md-12">
						<table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table fixed_headers">
						  	<thead>
								<tr><th colspan="3">Screening Committee Decision</th></tr>
                      			<tr>
                        			<td width="10%"><strong>Sl. No.</strong></td>
                        			<td><strong>Title</strong></td>
                        			<td style="text-align:center;" width="15%"><strong>Download</strong></td>
                      			</tr>
							</thead>
							<tbody>';
	
	foreach($fawloi_sc_com_result as $data2){
		$j++;
		global $base_root, $base_path;
		
		if($data2->is_new == 'Y'){
			$new_image2 = '<img src="'.$base_root.$base_path.'sites/default/files/new_gif_icon.gif" style="border:none;">';
		}else{
			$new_image2 = '';
		}
		
		$aa = $base_root.$base_path.'sites/default/files/contentpdf/'.$data2->filename;
		
		// $link_s = l(t('View'), $aa, array('attributes' => array('target'=>'_blank')));
		
		$link_s2 = l('<img title="Click here to download and print." src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'lcTheme').'/images/pdf.png" style="border:none;">', $aa, array('html' =>TRUE, 'attributes' => array('target' => '_blank')));
		
		$output .= '<tr>';
		$output .= '<td>'.$j.'</td>';
		$output .= '<td>'.$data2->content_title.'&nbsp;'.$new_image2.'</td>';
		$output .= '<td  style="text-align:center;">'.$link_s2.'</td>';
		$output .= '</tr>';
	}
							
	$output .='				</tbody>
						</table>
   				</div>
			</div>';
   
   	
	return $output;
}
 
?>									
					
                   