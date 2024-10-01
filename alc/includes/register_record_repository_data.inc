<?php
function register_record_admin_list(){	
	 global $base_root, $base_path, $user;
		
	 $user_id =	$user->uid;
	 $action = 	'encrypt';
		
	 $header = array(
		  array('data' => 'Sl. No', 'width' => '8%'),
		  array('data' => 'Name of the Establishment', 'width' => '20%'),
		  array('data' => 'Title', 'width' => '25%'),
		  array('data' => 'Description'),
		  array('data' => 'View', 'width' => '10%'),  
	  	);
	  
	
	 $query_subdivision=db_select('l_customuser_relation_address', 'lcra');
	 $query_subdivision->fields('lcra',array('sub_div_code','district_code'));
	 $query_subdivision->condition('lcra.user_id',$user_id,'=');
	 $result_query_subdivision = $query_subdivision->execute();
	 $result_arr = $result_query_subdivision->fetchAssoc();
	 $subdivision_alc = $result_arr['sub_div_code'];
	
	 $register_record_details 	= db_select('l_register_record_repository', 'lmrm');
	 $register_record_details->leftjoin('l_common_application_master', 'cam', 'cam.user_id = lmrm.user_id');
	 $register_record_details->fields('lmrm',array('id','repository_title','repository_description'));
	 $register_record_details->fields('cam',array('id','user_id','est_name'));
	 $register_record_details->condition('lmrm.is_active ','1');
	 $register_record_details->condition('lmrm.is_deleted ','0');
	 
	 if(!in_array('SUPER ADMIN', $user->roles)){
	 	$register_record_details->condition('lmrm.subdivision_code ',$subdivision_alc);
	 }
	 $register_record_details->orderBy('lmrm.id','DESC');
	 $rrd_result = $register_record_details->execute();
 
 	 if($rrd_result->rowCount() > 0 ){ 
  
  	$counter = 0;  
	$view_img = '<span class="badge bg-blue"><i class="fa fa-eye"></i> View Document<div>';
		
	foreach($rrd_result as $data){
		$counter++;
		$view_img = l('<img src =  "'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png">','register-record-repository-list-file/'.encryption_decryption_fun($action, $data->id),array('html' => TRUE,'attributes'=>array('target'=>'_blank','class'=>array('#pdf-img')))); 
		
		$rows[] = array(
				array('data' => $counter, 'align' => 'left', 'class' => array('')),
				array('data' => $data->est_name, 'align' => 'left'),
				array('data' => $data->repository_title, 'align' => 'left'),
				array('data' => $data->repository_description, 'align' => 'left'),
				array('data' => $view_img, 'align' => 'left'),
				);
		}
  	}
  
    $variables = array(
	  		'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')), 
	  		'header' => $header,
	  		'rows' => $rows,
			'empty' => t("No data found!")
	  );
	
	$output = '<div class="row"><div class="col-md-12"><div class="box box-primary"><div class="box-body">';
	$output .= theme('datatable', $variables);
	$output .= '</div></div></div></div>';	
  	
	
  return $output;		
  
}
?>