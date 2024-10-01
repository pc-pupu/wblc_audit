<?php
function nature_of_work(){	
	$output 	= '';
	$header 	= array(					 
					  array('data' => 'NATURE OF WORK','align' => 'left', 'field' => 'cont_work_name'),
					  array('data' => 'NATURE OF WORK CODE', 'align' => 'left', 'field' => 'contractor_work_id')
					 
	  		 	  );
		
	$cw_query = db_select('contractor_works', 'cw');	
				$cw_query->fields('cw', array('cont_work_name', 'contractor_work_id'));
				$cw_query->condition('cw.is_default', 1,'=');
				// $cw_query->condition('cw.act_id', 1,'=');
	
	
	$cw_query_result = $cw_query->execute();
	

	if( $cw_query_result->rowCount() > 0 ){		
		while($data = $cw_query_result->fetchObject()) {						
			$rows[] = array(					
					array('data' => $data->cont_work_name, 'align' => 'left', 'class' => array('')),
					array('data' =>	$data->contractor_work_id, 'align' => 'left', 'class' => array(''))					
			  );
		}
	}
	$variables = array(
	  		'attributes' 		=> array('class' => array('clra_applications')), 
	  		'header' 			=> $header,
	  		'rows'				=> $rows,
			'empty' 			=> t("No data found!")
	  );
	
	$output = theme('datatable', $variables);
	return $output;

	return $user_role;
}