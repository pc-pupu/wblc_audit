<?php 
global $base_root, $base_path, $user;

$add_1 				= 	$count_res_1['id_count_1'];
$add_2 				= 	$count_res_2['id_count_2'];
$result_add_all 	= 	$count_res_1['id_count_1']+$count_res_2['id_count_2'];

echo '<div class="panel panel-default panel-form">
		  <!--<div class="panel-heading">Registration</div>-->
		  	 <div class="panel-body">			   
				'.$all_applications_respect_applicant.
	 			'</div>
			
		
	</div>';

/*echo '<div class="panel panel-default panel-form">
		  <div class="panel-heading">License</div>
		  	 <div class="panel-body">			    
				'.$all_license_applications_respect_applicant.
	 			'</div>
			
		
	</div>'; */

?>