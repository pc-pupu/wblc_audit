<?php

function contractor_view($form, &$form_state,$act_id=0,$id=0){
		
	$act_id 			= encryption_decryption_fun('decrypt',  $act_id);
	$contractor_id 	 	= encryption_decryption_fun('decrypt',  $id);
	
	$result 			= db_select('l_contractor_info_master', 'lcim')->fields('lcim')->condition('lcim.id', trim($contractor_id))->execute();
  	$data 				= $result->fetchObject();
		
		
	$query 				= db_select('l_clra_con_nature_of_work', 'lccnow');
	$query				->fields('cw', array('cont_work_name'));
	$query				->leftJoin('contractor_works', 'cw', 'cw.contractor_work_id=lccnow.contractor_nature_of_work');
	$query				->condition('lccnow.contractor_id', $contractor_id);
	$resultum 			= $query->execute();
	
	$nature_of_wrk = getConNatureOfWrk($contractor_id, $data->application_id,1);
	
	/*$result_Id			=	array();
	while( $datum = $resultum->fetchObject() ){
		$result_Id[] 	= 	$datum->cont_work_name;
	}
	
	$nature_of_wrk 		= 	"";
	$prefix 			= 	'';
	foreach($result_Id as $val){
		$nature_of_wrk .= 	$prefix.$val;
		$prefix 		= 	', ';
	}*/
	
	$con_address 		= $data->address_of_contractor.'<br/>'.get_full_address('l_contractor_info_master', 'sub-table', $contractor_id, array('con_loc_e_dist','con_loc_e_subdivision','con_loc_e_areatype','con_name_areatype','con_loc_e_vill_ward','con_l_e_ps','contractor_pin'));
	
	
	if($data->contractor_type == 1){
		$adj 			 =  'has';
		$contractor_type = 'Offline generated License';
	}else{
		$adj 			 =  'is';
		$contractor_type = 'New Contractor';
	}
		
	$output = '';
	$output.= '<div class="content">
				<div class="panel panel-default panel-form">
					<div class="panel-heading">Particulars of Contractors and Contract Labour</div>
					<div class="panel-body">
						<ul class="contractor-view">
							<li>1. Contractor '.$adj.'<span class="right-side">'.$contractor_type.'</span></li>
							<li>2. Name of the Contractor <span class="right-side">'.$data->name_of_contractor.'</span></li>
							<li>3. Email of the Contractor <span class="right-side">'.$data->email_of_contractor.'</span></li>
							<li>4. Address of the Contractor <span class="right-side">'.$con_address.'</span></li>
							<li>5. Nature of Work in which Contract Labour is Employed or is to be Employed <span class="right-side">'.$nature_of_wrk.'</span></li>
							<li>6. Maximum Number of Contractor Labour to be Employed on any day Through Each Contractor <span class="right-side">'.$data->contractor_max_no_of_labours_on_any_day.'</span></li>
							<li>7. Estimated Date of Employment of Each Contract Work Under Each Contractor <span class="right-side">'.date('dS M, Y',strtotime($data->est_date_of_work_of_each_labour_from_date)).'&nbsp;to&nbsp;'.date('dS M, Y',strtotime($data->est_date_of_work_of_each_labour_to_date)).'</span></li>
						</ul>
					</div>
				</div>
			';
		
		
							
							
							

		$form['viewData']['markup'] 	= 	array(
									  			'#markup' => ''.$output.'',
          										);
									
		$form['viewData']['markup1']	= array(
												'#suffix' => '</div>',
												'#type'   => 'markup',
												'#markup' => l('<< Back to Contractors List', 'view-clra-application-details/clra-contractor-info/'.encryption_decryption_fun('encrypt', $data->application_id), array('attributes' => array('html' => TRUE, 'class' => 'link-btn')))																			
												);
		
		return $form;
	}