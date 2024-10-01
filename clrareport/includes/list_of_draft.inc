<?php 
function feedback_on_draft_regulation(){
   global $base_url;
   $cms_content_query = db_select('l_cms_content', 't');
	$cms_content_query->fields('t', array('content_id','content_title','link_title', 'content_desc','content_typ', 'fid', 'is_new','content_dt'));
	$cms_content_query->fields('fm', array('filename'));
	$cms_content_query->condition('t.content_typ','whatisnew','=');	
	$cms_content_query->condition('t.content_desc','','!=');
	$cms_content_query->condition('t.is_active','Y','=');
	$cms_content_query->leftJoin('file_managed', 'fm', 'fm.fid = t.fid');
	$cms_content_query->orderBy('t.ord','desc');
	$cms_content_query_result = $cms_content_query->execute()->fetchAll(); 
	
	$output = '';
    $output .='<div class="row">
				  <div class="col-sm-12">										
					  <div id="content-2" class="scrollContent content mCustomScrollbar light" data-mcs-theme="minimal-dark">
						  <table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table fixed_headers">
						  	<thead>
                      			<tr>
                        			<th width="6%">Sl. No.</th>
                        			<th>Title</th>
									<th width="12%">Published Date</th>
									<th width="17%">Feedback Closing Date</th>
									<th width="15%">Action</th>
                      			</tr>
							</thead>
							<tbody>';
	$i = 0;
	foreach($cms_content_query_result as $data){
		$i++;
		global $base_root, $base_path;
		if($data->is_new == 'Y'){
			$new_image = '<img src="'.$base_root.$base_path.'sites/default/files/new_gif_icon.gif" style="border:none;">';
		}else{
			$new_image = '';
		}
		
		$thirtyDays = strtotime(date('Y-m-d',strtotime("+30 days",strtotime($data->content_dt))));
		$presentDate= strtotime(date('Y-m-d'));
		
		//$a = date('Y-m-d',1607193000);
		//$b = date('Y-m-d',1607365800);
		
		//echo $thirtyDays.'--'.$presentDate.'<br/>';
		//echo $a.'--'.$b;exit;
		
		if($presentDate<$thirtyDays){	
			$output .= '<tr>';
			$output .= '<td>'.$i.'</td>';
			$output .= '<td>'.l(html_entity_decode($data->content_title).$new_icon, 'page-content/'.$data->content_typ.'/'.encryption_decryption_fun('encrypt', $data->content_id).'/'.$data->link_title, array('html' => 'true')).'&nbsp;'.$new_image.'</td>';
			$output .= '<td>'.date('dS M, Y', strtotime($data->content_dt)).'</td>';
			$output .= '<td>'.date('dS M, Y',strtotime("+30 days",strtotime($data->content_dt))).'</td>';
			$output .= '<td>'.l('<strong>Give Suggestion/Feedback</strong>', 'page-content/'.$data->content_typ.'/'.encryption_decryption_fun('encrypt', $data->content_id).'/'.$data->link_title, array('html' => 'true')).'&nbsp;</td>';
			$output .= '</tr>';
		}else{
			$output .= '<tr>';
			$output .= '<td colspan="5" align="center">No draft published</td>';
			$output .= '</tr>';
		}
		$output .= '</tbody></table></div></div></div>';
		return $output;
		
	}

}