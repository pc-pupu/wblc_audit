<?php
function general_timelines_deatils(){	
	$output 	= '';
	$header 	= array(					 
					  array('data' => 'Sl. No.','align' => 'left', 'field' => ''),
					  array('data' => 'Services', 'align' => 'left', 'field' => 'service_name'),
					  array('data' => 'Designated officer','align' => 'left', 'field' => 'designated_officer'),
					  array('data' => 'Stipulated time limit', 'align' => 'left', 'field' => 'designated_officer_stipulated_time_limit'),
					  array('data' => 'Appellate Officer','align' => 'left', 'field' => 'appellate_officer'),
					  array('data' => 'Stipulated time limit', 'align' => 'left', 'field' => 'appellate_officer_stipulated_time_limit'),
					  array('data' => 'Reviewing Officer','align' => 'left', 'field' => 'reviewing_officer'),
					  array('data' => 'Stipulated time limit', 'align' => 'left', 'field' => 'reviewing_officer_stipulated_time_limit')
					 
	  		 	  );
							
	$query = db_select('l_service_timeline_master', 'ltm');	
	$query->fields('ltm', array());
	$query->condition('ltm.is_active', 'Y');
	$query->orderby('ltm.ord', 'desc');
		
	$query_result = $query->execute();
	
	$sl_no = 1;

	if( $query_result->rowCount() > 0 ){		
		while($data = $query_result->fetchObject()) {						
			$rows[] = array(					
					array('data' => $sl_no++, 'align' => 'left', 'class' => array('')),
					array('data' =>	$data->service_name, 'align' => 'left', 'class' => array('')),
					array('data' =>	$data->designated_officer, 'align' => 'left', 'class' => array(''))	,
					array('data' =>	$data->designated_officer_stipulated_time_limit, 'align' => 'left', 'class' => array(''))	,
					array('data' =>	$data->appellate_officer, 'align' => 'left', 'class' => array(''))	,
					array('data' =>	$data->appellate_officer_stipulated_time_limit, 'align' => 'left', 'class' => array(''))	,
					array('data' =>	$data->reviewing_officer, 'align' => 'left', 'class' => array('')),
					array('data' =>	$data->reviewing_officer_stipulated_time_limit, 'align' => 'left', 'class' => array(''))					
			  );
		}
	}
	$variables = array(
					'attributes' 		=> array('class' => array('view-act-rules-table')), 
					'header' 			=> $header,
					'rows'				=> $rows,
					'empty' 			=> t("No data found!")
	  );
	
	$output = theme('datatable', $variables);
	
	$output.=  '<div class="field-items"><br><br><p><a href="'.$base_root.$base_path.'/sites/default/files/important_notification/RIGHT_TO_PUBLIC_SERVICES_NOTIFICATION.pdf" target="_blank"><i class="fa fa-file" aria-hidden="true"></i>View General notification</a><a href="'.$base_root.$base_path.'/sites/default/files/important_notification/gazette-notification-RTPS-shops-est-reg-timeline.pdf" target="_blank"><i class="fa fa-file" aria-hidden="true"></i>View notification(Shops & Est.)</a></p></div><br>';
	
	return $output;
}

?>