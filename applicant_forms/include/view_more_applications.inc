<?php
function view_more_applications(){
	$pr_info_query = db_select('l_establishment_unit_master', 'leum');			
	$pr_info_query->fields('leum', array('risk_status', 'max_number_of_worker'));
	// $pr_info_query->condition('leum.applicant_user_id', $user->uid);
	$pr_info_query_result = $pr_info_query->execute()->fetchAll();
	
	$html = '<table class="table table table-striped table-responsive dashboard-table" border="0" cellpadding="0" cellspacing="0">
				<thead>
					<tr>
						<th>Sl. No.</th>
						<th>Establishment Name</th>
					</tr>
				 </thead>
				 <tbody>
				 </tbody>	
			</table>';
			
	return $html;	
}

?>

