<?php
function list_of_minimum_wage_scheduled_employment(){
	
	$min_wage_info = db_select('l_min_wages_schedule_employment_details_info','l');
	$min_wage_info -> fields('l', array('id','employment_title','employment_id','year','session', 'is_active'));
	//$min_wage_info -> condition('l.is_active','Y');
	$min_wage_info -> orderBy('l.employment_title', 'ASC');
	$min_wage_info = $min_wage_info->execute()->fetchAll();
	
	
	$output .= '<div class="box box-primary">
					<div class="box-body">';					
	$header = array(
						array('data' => 'SL.NO.', 'width' => '5%'),
						array('data' => 'Name of Employment', 'width' => '35%'),
						array('data' => 'Year', 'width' => '10%'),
						array('data' => 'Session', 'width' => '10%'),
						array('data' => 'Status', 'width' => '10%'),
						//array('data' => 'View & Edit','width' => '10%'),
						array('data' => 'Generate & Download', 'width' => '10%'),
						//array('data' => 'Upload', 'width' => '10%'),
						
			);	

		$x = 0;	
		foreach($min_wage_info as $data){ 
			$x++;
			if($data->session == 1){$session = 'January';}else{$session = 'July';}
			$generate = l("<i class='fa fa-download' aria-hidden='true' title='Generate & Download'></i>", 'minimum-wage-pdf/'.encryption_decryption_fun('encrypt',$data->employment_id), array('html' => TRUE,'attributes'=>array('target'=>'_blank')));	
			/*$edit = l("<i class='fa fa-pencil' aria-hidden='true' title='View & Edit Details'></i>", 'minimumwage-information-edit/'.encryption_decryption_fun('encrypt',$data->employment_id), array('html' => TRUE,'attributes'=>array('target'=>'_blank')));
			$upload = l("<i class='fa fa-upload' aria-hidden='true' title='Upload'></i>", 'minimumwages/addminimumwages/'.encryption_decryption_fun('encrypt',$data->employment_id), array('html' => TRUE,'attributes'=>array('target'=>'_blank')));*/
			if($data->is_active == 1){
				$status = '<font color="#339933">Verified</font>';
			}elseif($data->is_active == 0){
				$status = '<font color="#ff0000">Not Verified</font>';
			}elseif($data->is_active == 2){
				$status = '<font color="#339933"><strong>Done</strong></font>';
			}
			
			$rows[] = array(
							array('data' => $x, 'align' => 'left'),
							array('data' => $data->employment_title, 'align' => 'left'),
							array('data' => $data->year, 'align' => 'left', 'class' => array('odd')),
							array('data' => $session, 'align' => 'left', 'class' => array('odd')),
							array('data' => $status, 'align' => 'left', 'class' => array('odd')),
							//array('data' => $edit, 'align' => 'left', 'class' => array('odd')),
							array('data' => $generate, 'align' => 'left', 'class' => array('odd')),
							//array('data' => $upload, 'align' => 'left', 'class' => array('odd')),
					  );
		}
		$variables = array(
							'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')), 
							'header' => $header,
							'rows' => $rows,
							'empty' => t("Data not available.")
					  );


	$output .= theme('datatable', $variables);
	$output .= ' 		</div>
					</div>';
	return $output;
}
?>