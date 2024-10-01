<?php


function annual_return_alc($form,&$form_state){
	
	$form['annaul']		= array(
		'#title'		=> t('Annual Return List'),
		'#type'			=> 'fieldset'
	);
	
	$form['annaul']['markup']	= array(
		'#type'			=> 'markup',
		'#prefix' 	=> '<div class="row"><div class="col-md-12"><div class="box box-primary"><div class="box-body">',
		'#suffix' 	=> '</div></div></div></div>',
		'#markup'		=> get_data()
	);
	
	
	
	return $form;
}

function get_data(){
	
	global $user;
	
	$rows	= array();
	$counter	= 0;
	$per_page	= 5;
	
	$user_id	= $user->uid;
	
	$header = array(
		array('data' => 'Sl. No', 'field' => 'slno','width' => '2%'),
		array('data' => 'NAME OF THE ESTABLISHMENT', 'field' => 'est_name','width' => '16%'),
		array('data' => 'NAME OF SERVICES', 'field' => ''),
		array('data' => 'APPLY YEAR', 'field' => 'applicationDate'),
		array('data' => 'ACTIONS', 'field' => 'actions'),
	);
	
	/**** Fetch ALC SUBDIVISION Code ****/
	
	$query_subdivision = db_select('l_customuser_relation_address', 'lcra');
	$query_subdivision->fields('lcra',array('sub_div_code','district_code'));
	$query_subdivision->condition('lcra.user_id',$user_id,'=');
	$result_query_subdivision = $query_subdivision->execute();	
	
	if($result_query_subdivision-> rowCount() > 0 ){		
		$subdivision_code_result = $result_query_subdivision->fetchAssoc();		
		$subdivision_alc = $subdivision_code_result['sub_div_code'];		
	}	
	
	// annual return query 
		
	$query_annual_return = db_select('l_annual_return_wizard_info','larw');
	$query_annual_return ->fields('larw',array());
	$query_annual_return ->LeftJoin('l_annual_return_common_data','larcd','larw.user_id=larcd.user_id');
	$query_annual_return ->fields('larcd',array()); 
	$query_annual_return ->condition('larcd.status',1); 
	$query_annual_return ->condition('larcd.est_subdivision',$subdivision_alc);
	
	$data = $query_annual_return->execute()->fetchAll();
	
	
	
	
	$current_page = pager_default_initialize(count($data), $per_page);
	

	$chunks = array_chunk($data, $per_page, TRUE);
	
	if($current_page>0){
		$counter = $current_page*$per_page;
	}
	
	$nodes	= array();
	

	
	if(count($data)>0){
		
		foreach($chunks[$current_page] as $row){
			$counter++;
			
			$returnQuery = db_select('l_annual_return_wizard_info','larw');
			$returnQuery->fields('larw',array());
			$returnQuery->condition('user_id',$row->user_id);
			
			$returnResult	=	$returnQuery->execute();
			if($returnResult->rowCount() > 0){
				$result		= $returnResult->fetchAll();
			}else{
				$result		= array();
			}
			
			if(!empty($result)){
				
				
				foreach($result as $rowa){
					$m=0;
					$year 		= $rowa->current_yr; 
					$act_list 	= '<ul>';
					if($rowa->clra_act==1){
						$m++;
						$act_list .= '<li>'.$m.') Contract Labour (R & A) Act, 1970</li>';
					}
					if($rowa->license_act==1){
						$m++;
						$act_list .= '<li>'.$m.') Contract License</li>';
					}
					if($rowa->bocwa_act==1){
						$m++;
						$act_list .= '<li>'.$m.') Building and Other Construction Worker Act, 1996</li>';
					}
					if($rowa->ismw_act==1){
						$m++;
						$act_list .= '<li>'.$m.') Inter State Migrant Workmen (Regulation of Employment and Conditions of Service) Act, 1979</li>';
					}
					if($rowa->mtw_act==1){
						$m++;
						$act_list .= '<li>'.$m.') Motor Transport Worker Act, 1961</li>';
					}
					if($rowa->minimum_wages_act==1){
						$m++;
						$act_list .= '<li>'.$m.') Payment of Minimum Wages Act</li>';
					}
					if($row->annual_return_bonus==1){
						$m++;
						$act_list .= '<li>'.$m.') Payment of Bonus Act</li>';
					}
					if($row->maternity_benefit==1){
						$m++;
						$act_list .= '<li>'.$m.')  Maternity Benefit Act</li>';
					}
					if($row->annual_return_wages==1){
						$m++;
						$act_list .= '<li>'.$m.')  Payment of Wages Act</li>';
					}
					$act_list 	.= '</ul>';
					
					$query = db_select('l_annual_return_common_data','larcd');
					$query->fields('larcd',array('status'));
					$query->condition('user_id', $row->user_id);
					$query->condition('wizard_id', $row->wizard_id);
					$queryResult	=	$query->execute();
					
					if($queryResult->rowCount() > 0){
						$resultCommon		= $queryResult->fetchAssoc();
						$status 			= $resultCommon['status'];
					}
					
					if($status=="" || empty($status)){
						$link = l(t('View/Edit'),'common-year-wise-annual-return/'.encryption_decryption_fun('encrypt', $row->id),array('attributes' => array('html'=>TRUE,'title'=>'Click to View Details')));
					}else{
						$link = l(t('View Details'),'annual-return-pdf/'.encryption_decryption_fun('encrypt', $row->wizard_id).'/'.encryption_decryption_fun('encrypt', $row->user_id),array('attributes' => array('html'=>TRUE,'target' => '_blank','title'=>'Click to View Details')));
					}
				}
			}
			
			
			
			$rows[]	= array(
						array('data' => $counter, 'class' => array()),
						array('data' => $row->est_name, 'class' => array()),
						array('data' => $act_list, 'class' => array()),
						array('data' => $row->current_yr, 'class' => array()),
						array('data' => $link, 'class' => array()),
			);
	
		}
	}
	
	
	$output .= theme_table(
			array(
				'header' => $header,
				'rows' => $rows,
				'attributes' => array('class' => array('clra_applications admin-custom-table')),
				'caption' => '',
				'colgroups' => array(),
				'sticky'=>'',
				'empty' => t("No data found!") 
			)
	).theme('pager');
	
	return $output;
}




function annual_return_alc_license($form,&$form_state){
	
	$form['annaul']		= array(
		'#title'		=> t('Annual Return List'),
		'#type'			=> 'fieldset'
	);
	
	$form['annaul']['markup']	= array(
		'#type'			=> 'markup',
		'#prefix' 	=> '<div class="row"><div class="col-md-12"><div class="box box-primary"><div class="box-body">',
		'#suffix' 	=> '</div></div></div></div>',
		'#markup'		=> get_alc_data()
	);
	
	
	
	return $form;
}

function get_alc_data(){
	
	global $user;
	
	$rows	= array();
	$counter	= 0;
	$per_page	= 5;
	
	$user_id	= $user->uid;
	
	$header = array(
		array('data' => 'Sl. No', 'field' => 'slno','width' => '6%'),
		array('data' => 'NAME OF THE ESTABLISHMENT', 'field' => 'est_name','width' => '16%'),
		array('data' => 'FORM-V NUMBER & Principal Employee Name', 'field' => ''),
		array('data' => 'APPLY YEAR', 'field' => 'applicationDate'),
		array('data' => 'ACTIONS', 'field' => 'actions'),
	);
	
	/**** Fetch ALC SUBDIVISION Code ****/
	
	$query_subdivision = db_select('l_customuser_relation_address', 'lcra');
	$query_subdivision->fields('lcra',array('sub_div_code','district_code'));
	$query_subdivision->condition('lcra.user_id',$user_id,'=');
	$result_query_subdivision = $query_subdivision->execute();	
	
	if($result_query_subdivision-> rowCount() > 0 ){		
		$subdivision_code_result = $result_query_subdivision->fetchAssoc();		
		$subdivision_alc = $subdivision_code_result['sub_div_code'];		
	}	
	
	
	// annual return query 
	

	$query_annual_return = db_select('l_annual_return_license_extended','larw');
	$query_annual_return ->fields('larw',array('form_v_no','wizard_id','user_id'));
	
	$query_annual_return ->LeftJoin('l_annual_return_common_data','larcd','larw.user_id=larcd.user_id');
	$query_annual_return ->fields('larcd',array()); 
	$query_annual_return ->LeftJoin('l_contractor_license_application', 'lcla','lcla.user_id=larw.user_id');
	$query_annual_return ->fields('lcla',array('application_id')); 
	$query_annual_return ->LeftJoin('l_common_application_master', 'lcpm', 'lcpm.user_id=larw.user_id');
	$query_annual_return ->fields('lcpm',array('est_name'));
	$query_annual_return ->LeftJoin('l_annual_return_wizard_info', 'larwi', 'larwi.user_id=larw.user_id');
	$query_annual_return ->fields('larwi',array('current_yr'));  
	$query_annual_return ->condition('larw.worksite_sub_district',$subdivision_alc);
	$data = $query_annual_return->execute()->fetchAll();
	
	$current_page = pager_default_initialize(count($data), $per_page);
	

	$chunks = array_chunk($data, $per_page, TRUE);
	
	if($current_page>0){
		$counter = $current_page*$per_page;
	}
	
	$nodes	= array();
	

	
	if(count($data)>0){
		
		foreach($chunks[$current_page] as $row){
			$counter++;
			
			$query = db_select('l_clra_registration_master', 'lcla');
			$query->fields('lcla', array('id','user_id','full_name_manager','address_manager'));
			$query->condition('lcla.id',$row->application_id);
			$result 	= $query->execute();
			$row_val = $result->fetchAssoc();
			
					
			
			$link = l(t('View Details'),'annual-return-pdf/'.encryption_decryption_fun('encrypt', $row->wizard_id).'/'.encryption_decryption_fun('encrypt', $row->user_id),array('attributes' => array('html'=>TRUE,'target' => '_blank','title'=>'Click to View Details')));
				
			
			
			
			$rows[]	= array(
						array('data' => $counter, 'class' => array()),
						array('data' => $row->est_name, 'class' => array()),
						array('data' => $row->form_v_no.' - '.$row_val['full_name_manager'], 'class' => array()),
						array('data' => $row->current_yr, 'class' => array()),
						array('data' => $link, 'class' => array()),
			);
	
		}
	}
	
	
	$output .= theme_table(
			array(
				'header' => $header,
				'rows' => $rows,
				'attributes' => array('class' => array('clra_applications admin-custom-table')),
				'caption' => '',
				'colgroups' => array(),
				'sticky'=>'',
				'empty' => t("No data found!") 
			)
	).theme('pager');
	
	return $output;
}