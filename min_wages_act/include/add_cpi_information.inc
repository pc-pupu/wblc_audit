<?php
drupal_add_js(drupal_get_path('module', 'min_wages_act') . '/js/custom.js');
drupal_add_library('system','ui.datepicker');
 drupal_add_js( 'jQuery(document).ready(function(){
           jQuery("#yr_month_id").datepicker({
            changeMonth: true,
            changeYear: true,
            showButtonPanel: true,
            dateFormat: "MM yy",
            onClose: function(dateText, inst) { 
                jQuery(this).datepicker("setDate", new Date(inst.selectedYear, inst.selectedMonth, 1));
            }
            });
        });', 'inline');
 drupal_add_css( 
    '.ui-datepicker-calendar {
        	display: none;
	}','inline');
   
			
function add_cpi_information($form, &$form_state){
	
	$form['cpi']['header'] = array( 	
		'#prefix'=>'<div class="box box-primary"><div class="box-body">',
	);
	$form['cpi']['year_month'] = array( 	
		'#prefix'=>'<div class="col-md-4"><label class="input">',
		'#suffix'=>'</label></div>',
		'#title'=>'Year & Month ',
		'#type'=>'textfield',
		'#required'=>TRUE,
		'#id'=>'yr_month_id',
		'#attributes'=>array('class' => array('form-control'),'readonly' => 'readonly'),	
	);
	/*$form['cpi']['month'] = array( 	
		'#prefix'=>'<div class="col-md-4"><label class="input">',
		'#suffix'=>'</label></div>',
		'#title'=>'Month',
		'#type'=>'textfield',
		'#id' => 'month_id',
		'#required'=>TRUE,
		'#attributes'=>array('class' => array('form-control')),	
	);*/
	$form['cpi']['indstry_worker_value'] = array( 	
		'#prefix'=>'<div class="col-md-4"><label class="input">',
		'#suffix'=>'</label></div>',
		'#title'=>'Industry Worker Value',
		'#type'=>'textfield',
		'#required'=>TRUE,
		'#element_validate' => array('number_validation'),
		'#attributes'=>array('class' => array('form-control')),	
	);
	$form['cpi']['al_value'] = array( 	
		'#prefix'=>'<div class="col-md-4"><label class="input">',
		'#suffix'=>'</label></div>',
		'#title'=>'Agriculture Labourers Value',
		'#type'=>'textfield',
		//'#required'=>TRUE,
		'#element_validate' => array('number_validation'),
		'#attributes'=>array('class' => array('form-control')),	
	);
	/*$form['cpi']['avg_value'] = array( 	
		'#prefix'=>'<div class="col-md-4"><label class="input">',
		'#suffix'=>'</label></div>',
		'#type'=>'textfield',
		'#title'=>'Average Value',
		'#required'=>TRUE,
		'#attributes'=>array('class' => array('form-control')),	
	);*/
	$form['cpi']['is_active'] = array(
		'#prefix'=>'<div class="col-md-4"><label class="checkbox">',
		'#suffix'=>'</label></div>',
		'#type'=>'checkbox',
		'#title'=>'Active',
		'#required'=>TRUE,
		'#attributes'=>array('class' => array('form-control')),	
	); 
	$form['cpi']['footer'] = array(
		'#suffix'=>'</div></div>',	 
	);	
	$form['cpi']['submit'] = array(
		'#type'=>'submit',
		'#value'=>'SAVE',			
		'#attributes'=>array('class' => array('btn btn-primary pull-left')),
	); 
	

	return $form;
}

function add_cpi_information_submit($form_id , $form_state){
	$val = $form_state['values'];
	$val_year = date('Y',strtotime($val['year_month'])) ;
	$val_month = date('n',strtotime($val['year_month'])); 
	$val_month_title = date('F',strtotime($val['year_month'])); 
	
	/******** Industrial Worker CPI Avg calculation ***********/
	$iw_value_sum = 0;
	for($i=0; $i<=6; $i++){
		$year = date('Y',strtotime(date('Y-m',strtotime($val['year_month']))." -".$i." month")); '</br>';
		$month = date('n',strtotime(date('Y-m',strtotime($val['year_month']))." -".$i." month")); '</br>';
		$query = db_select('l_mnw_cpi_number_master', 'l');
		$query->fields('l', array('iwvalue'))->condition('cpiyear', $year)->condition('month_srl', $month);
		$query_result = $query->execute()->fetchAssoc();
		$iw_value_sum = $query_result['iwvalue'] + $iw_value_sum; 
	} 
	$iw_avg_value = $iw_value_sum / 6; 
	
	/******** Agriculture Labourers CPI Avg calculation ***********/
	$al_value_sum = 0;
	for($j=0; $j<=6; $j++){
		$year = date('Y',strtotime(date('Y-m',strtotime($val['year_month']))." -".$j." month")); '</br>';
		$month = date('n',strtotime(date('Y-m',strtotime($val['year_month']))." -".$j." month")); '</br>';
		$query = db_select('l_mnw_cpi_number_master', 'l');
		$query->fields('l', array('alvalue'))->condition('cpiyear', $year)->condition('month_srl', $month);
		$query_result = $query->execute()->fetchAssoc();
		$al_value_sum = $query_result['alvalue'] + $al_value_sum; 
	} 
	$al_avg_value = $al_value_sum / 6; 
	
	$query11 = db_select('l_mnw_cpi_number_master', 'lm');
	$query11->fields('lm', array('iwvalue'))->condition('cpiyear', $val_year)->condition('month_srl', $val_month);
	$query11 = $query11->execute();
	
	if($query11->rowCount() > 0){
		drupal_set_message('Already added for this month.');
	}else{	
		$fields = array(
					'cpiyear' => $val_year,
					'month_srl'=>$val_month,
					'month_title'=>$val_month_title,
					'iwvalue'=>!empty($val['indstry_worker_value']) ? $val['indstry_worker_value'] : 0,
					'is_active'=>$val['is_active'],
					'alvalue'=>!empty($val['al_value']) ? $val['al_value'] : 0,
					'iw_avg_value'=>!empty($iw_avg_value) ? $iw_avg_value : 0, 
					'al_avg_value'=>!empty($al_avg_value) ? $al_avg_value : 0, 
					'create_date'=>time(),
					'modify_date'=>time(),
					'centre_desc' => 1
					);
		$result = db_insert('l_mnw_cpi_number_master')->fields($fields)->execute();
		if($result){
			drupal_set_message('Added successfully');
		}else{
			drupal_set_message('Not added');
		}
	}
}

function edit_cpi_information($form, &$form_state , $year, $month){
	$year = encryption_decryption_fun('decrypt',$year);
	$month = encryption_decryption_fun('decrypt',$month);
	
	$query_cpi_info = db_select('l_mnw_cpi_number_master','l');
	$query_cpi_info->fields('l', array());
	$query_cpi_info->condition('l.cpiyear',$year); 
	$query_cpi_info->condition('l.month_srl',$month); 

	$query_cpi_info_result = $query_cpi_info->execute()->fetchAssoc();   
	
	$year_month_value = $query_cpi_info_result['month_title'].' '.$query_cpi_info_result['cpiyear'];
	//echo '<pre>'; print($query_cpi_info_result); die;
	
	$form['cpi']['header'] = array( 	
		'#prefix'=>'<div class="box box-primary"><div class="box-body">',
	);
	$form['cpi']['id'] = array( 	
		'#value'=>$id,
		'#type'=>'hidden'
		
	);
	$form['cpi']['year_month'] = array( 	
		'#prefix'=>'<div class="col-md-4"><label class="input">',
		'#suffix'=>'</label></div>',
		'#title'=>'Year & Month ',
		'#type'=>'textfield',
		'#default_value'=>$year_month_value,
		'#required'=>TRUE,
		'#id' => 'yr_month_id',
		'#attributes'=>array('class' => array('form-control'),'disabled' => 'disabled'),	
	);
	/*$form['cpi']['month'] = array( 	
		'#prefix'=>'<div class="col-md-4"><label class="input">',
		'#suffix'=>'</label></div>',
		'#title'=>'Month',
		'#type'=>'textfield',
		'#id' => 'month_id',
		'#required'=>TRUE,
		'#attributes'=>array('class' => array('form-control')),	
	);*/
	$form['cpi']['indstry_worker_value'] = array( 	
		'#prefix'=>'<div class="col-md-4"><label class="input">',
		'#suffix'=>'</label></div>',
		'#title'=>'Industry Worker Value',
		'#type'=>'textfield',
		'#default_value'=>$query_cpi_info_result['iwvalue'],
		'#required'=>TRUE,
		'#element_validate' => array('number_validation'),
		'#attributes'=>array('class' => array('form-control')),	
	);
	$form['cpi']['al_value'] = array( 	
		'#prefix'=>'<div class="col-md-4"><label class="input">',
		'#suffix'=>'</label></div>',
		'#title'=>'Agriculture Worker Value',
		'#type'=>'textfield',
		'#default_value'=>$query_cpi_info_result['alvalue'],
		'#required'=>TRUE,
		'#element_validate' => array('number_validation'),
		'#attributes'=>array('class' => array('form-control')),	
	);
	/*$form['cpi']['avg_value'] = array( 	
		'#prefix'=>'<div class="col-md-4"><label class="input">',
		'#suffix'=>'</label></div>',
		'#type'=>'textfield',
		'#title'=>'Average Value',
		'#default_value'=>$query_cpi_info_result['iw_avg_value'],
		'#required'=>TRUE,
		'#attributes'=>array('class' => array('form-control'),'readonly'=>'readonly'),	
	);*/
	$form['cpi']['is_active'] = array(
		'#prefix'=>'<div class="col-md-4"><label class="input">',
		'#suffix'=>'</label></div>',
		'#type'=>'checkbox',
		'#title'=>'Active',
		'#default_value'=>$query_cpi_info_result['is_active'],
		'#required'=>TRUE,
		'#attributes'=>array('class' => array('form-control')),	
	); 
	
	$form['cpi']['footer'] = array(
		'#suffix'=>'</div></div>',	 
	);
	
	$form['cpi']['submit'] = array(
		'#type'=>'submit',
		'#value'=>'Update',			
		'#attributes'=>array('class' => array('btn btn-primary pull-left')),
	); 
	
	return $form;
}

function edit_cpi_information_submit($form_id , $form_state){ 
	$val = $form_state['values']; echo '';
	$iw_value_sum = 0;
	$val_year = date('Y',strtotime($val['year_month'])) ;
	$val_month = date('n',strtotime($val['year_month'])); 
	$val_month_title = date('F',strtotime($val['year_month'])); 
	
	for($i=0; $i<=6; $i++){
		$year = date('Y',strtotime(date('Y-m',strtotime($val['year_month']))." -".$i." month")); echo '</br>';
		$month = date('n',strtotime(date('Y-m',strtotime($val['year_month']))." -".$i." month")); echo '</br>';
		$query = db_select('l_mnw_cpi_number_master', 'l');
		$query->fields('l', array('iwvalue'))->condition('cpiyear', $year)->condition('month_srl', $month);
		$query_result = $query->execute()->fetchAssoc();
		$iw_value_sum = $query_result['iwvalue'] + $iw_value_sum; 
	} 
	$iw_avg_value = $iw_value_sum / 6; 
	
	$fields = array(
					'cpiyear' => $val_year,
					'month_srl'=>$val_month,
					'month_title' => $val_month_title,
					'iwvalue' => $val['indstry_worker_value'],
					'is_active' => $val['is_active'],
					'alvalue' => $val['al_value'],
					'iw_avg_value' => $iw_avg_value, 
					'al_avg_value' => '0', 
					'modify_date' => time(),
					'centre_desc' => 1
					); 
	$result = db_update('l_mnw_cpi_number_master')->fields($fields)->condition('cpiyear',$val_year)->condition('month_srl',$val_month)->execute();
	if($result){
		drupal_set_message('Updated successfully');
		drupal_goto('cpi-number-for-worker');
	}else{
		drupal_set_message('Not updated');
	}
}

function list_cpi_information(){
	$query_cpi_info = db_select('l_mnw_cpi_number_master','l');
	$query_cpi_info->fields('l', array());
	$query_cpi_info_result = $query_cpi_info->execute();
	$header = array(
				array('data' => 'Sl.No','width' => '7%'),
				array('data' => 'Year'),
				array('data' => 'Month','width' => '10%'),
				array('data' => 'Avarage Value' ),
				array('data' => 'Last Modified Date'),		    
				array('data' => 'Status'),
				array('data' => 'Action'),
			);  
	$rows = array();
	$i = 0;
	if($query_cpi_info_result->rowCount() > 0){
		foreach($query_cpi_info_result->fetchAll() as $data){
			$i++;
			if($data->is_active == 1){				
				$status = '<span class="badge bg-green">&nbsp;Active&nbsp;</span>';					
			}elseif($data->is_active == 0){				
				$status = '<span class="badge bg-red">&nbsp;Inactive&nbsp;</span>';	
			}
			$link_edit = l(t('<span class="badge bg-blue"><i class="fa fa-edit"></i> Edit Details</span>'), 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->id).                    '/cpi-management_edit', array('html' => TRUE ,'attributes' =>              array('target'=> '_blank')));
			$rows[] = array(
							$i,
							$data->cpiyear,							
							$data->month_title,
							$data->iw_avg_value,
							date('d-m-Y', $data->modify_date),						
							$status,
							$link_edit
						); 
		}
	}
	$variables = array(
					'attributes'=> array('class' => array('table table-striped table-responsive admin-custom-table')),
					'header'=> $header,
					'rows'=> $rows,
					);
	$output = theme('datatable', $variables);

	return '<div class="box box-primary"><div class="box-body">'.$output.'</div></div>';
}
