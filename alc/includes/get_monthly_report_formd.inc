<?php
function get_monthly_report_forma($form,&$form_state){
	global $base_root,$base_path;
	 
	 
	
	 
	$options_first =    common_custom_user_district_list_new() + array("ALL" => "ALL"); 
     
	 $value_dropdown_first = isset($form_state['values']['disname']) ? $form_state['values']['disname'] :key($options_first);		   
	 $form['insdiffstreports']['header'] = array(									   
							'#type' 		=> 'markup',
							'#markup' 		=> '<div class="row"><div class="col-md-2"><div class="box box-primary"><div class="box-body">',
	);	   
						   
      $form['insdiffstreports']['disname'] = array(		
							'#title'		 => 'District',	   
							'#type' 		 => 'select',
							'#prefix' 		 => '',
							'#required' 	 => TRUE,		
							'#options' 		 => $options_first,
							'#attributes'    => array('class'=>array('form-control')),
							'#default_value' => $value_dropdown_first,
							'#ajax' => array(
											'event' 	=> 'change',			
											'callback'  => 'inspection_diffst_rep_subdivsion_callback'
		   					),		
    );
	 
	 $options_subdiv_one = custom_user_subdivisionlist_list($form_state['values']['disname']);
	 
	 $form['insdiffstreports']['inspection_dropdown_second'] = array(	 
							'#prefix' 		 => '<div id="inspection_diffst_subdivsion_replace" class="estab-shop-row">',
							'#suffix' 		 => '</div>',	 
							'#type' 		 => 'select',
							'#title' 		 => 'Sub-Division',    
							'#options' 		 => $options_subdiv_one,
							'#attributes'    => array('class'=>array('form-control')),
							'#default_value' => isset($form_state['values']['inspection_dropdown_second']) ? $form_state['values']['inspection_dropdown_second'] :'',
							'#ajax' => array(
											'event' 	=> 'change',
											'callback'  => 'insreport_diffst_block_callback',
											'wrapper' 	=> 'inspection_diffst_dropdown_third_replace',
											'method' 	=> 'replace',
											),
	); 
	
	
	$form['insdiffstreports']['fodate'] = array(	     
		'#title' 		 => t('From Date:'),
		'#type' 		 => 'date',
		'#required' 	 => TRUE,
		'#default_value' => array(
								'month' => format_date(strtotime("-1 months"), 'custom', 'n'),
								'day' 	=> format_date(time(), 'custom', 'j'),
								'year'	=> format_date(time(), 'custom', 'Y'),
								),
							'#after_build' => array('get_report_set_year_range')
	);
	 
	$form['insdiffstreports']['todate'] = array(		
		'#title' 		 => t('To Date:'),
		'#type' 	 	 => 'date',
		'#required' 	 => TRUE,
		'#default_value' => array(
								'month' => format_date(time(), 'custom', 'n'),
								'day' 	=> format_date(time(), 'custom', 'j'),
								'year' 	=> format_date(time(), 'custom', 'Y'),
								),
							'#after_build' => array('get_report_set_year_range')	 	
	); 	
	
	$form['insdiffstreports']['ins_bt_srchbtn'] = array(
		'#type' 		 => 'button',
		'#prefix' => '<div class="form-item form-type-select col-md-12 row">',
		'#suffix' => '</div></div></div></div><div class="col-md-10"><div class="box box-primary"><div class="box-body">', 
		'#default_value' => 'Search',
		'#attributes' 	 => array('class' => array('btn btn-primary')),
		'#ajax' 		 => array(
								'callback' => 'monthly_report_details',
								'wrapper'  => 'lb_ins_diffst_reports_div',
								'method'   => 'html',
								'effect'   => 'fade'
								)
	);
							
	$form['insdiffstreports']['comrep'] = array(
		'#prefix' => '<div id="lb_ins_diffst_reports_div">MONTHLY REPORT',
	    '#suffix' => '</div></div></div>',		 
	); 	
    
	$form['insdiffstreports']['footer'] = array(									   
		'#type' 		 => 'markup',
		'#markup' 		 => '</div>',	    							
	);
	return $form;

}

function monthly_report_details($form, $form_states){
	global $base_root, $base_path;
	
	$val = $form_states['values'];
	$fodate = trim($val['fodate']);
	$todate = trim($val['todate']);
	$dist_code = trim($val['disname']);
	$subdiv_code = trim($val['inspection_dropdown_second']);
	$fdt = $val['fodate']['year'].'-'.$val['fodate']['month'].'-'.$val['fodate']['day'];
	$fdt=strtotime($fdt);
	$tdt = $val['todate']['year'].'-'.$val['todate']['month'].'-'.$val['todate']['day'];
	$tdt=strtotime($tdt);
	
	

	$print_link = l('<div class="box-header with-border pull-right"><button type="button" class="btn btn-default"><i class="fa fa-print"></i> Print</button></div>', 'monthly-report/formapdf/'.encryption_decryption_fun('encrypt',$fdt).'/'.encryption_decryption_fun('encrypt',$tdt).'/'.encryption_decryption_fun('encrypt',$dist_code).'/'.encryption_decryption_fun('encrypt',$subdiv_code), array('attributes' => 	array('target'=>'_blank'), 'html' => true));
	$print_xls = l('<div class="box-header with-border pull-right"><button type="button" class="btn btn-default"><i class="fa fa-print"></i>Download xls</button></div>', 'monthly-report/formaxls/'.encryption_decryption_fun('encrypt',$fdt).'/'.encryption_decryption_fun('encrypt',$tdt).'/'.encryption_decryption_fun('encrypt',$dist_code).'/'.encryption_decryption_fun('encrypt',$subdiv_code), array('attributes' => 	array('target'=>'_blank'), 'html' => true));
	
	$header = array(
					array('data' => 'Sl.No.<br>(1)', 'field' => 'Sl.No.', 'width' => '8%'),
					array('data' => 'Notified Service(s)<br>(2)', 'field' => 'Notified Service(s)'),
					array('data' => 'No of Form-I issued<br>(3)', 'field' => 'No of Form-I issued', 'width' => '10%'),
					array('data' => 'No. of application disposed of during the month according to RTPS time frame for which Form-I were issued in the previous month<br>(4a)', 'field' => 'No of Form-I issued', 'width' => '10%'),
					array('data' => 'No of Application disposed of during the month according to RTPS time frame for which acknowledgement have been issued during the month<br>(4b)', 'field' => 'No of Applications disposed of during the month according to RTPS time frame for which acknowledgement have been issued during the month', 'width' => '12'),
					array('data' => 'No. of application pending disposal which are RTPS time frame<br>(4c)', 'field' => 'No. of application pending disposal which are RTPS time frame<br>(6)', 'width' => '12'),
					array('data' => 'No. of application rejected during the month<br>(4d(i))', 'field' => 'No. of application rejected during the month<br>(7)', 'width' => '12'),
					array('data' => 'No. of application rejected during the month for which acknowledgement have been issued in previous month<br>(4d(ii))', 'field' => 'No. of application rejected during the month for which acknowledgement have been issued in previous month.', 'width' => '14'),
					array('data' => 'No.of Applications disposed off in favour of applicants<br>(5)', 'field' => 'No. of application rejected during the month for which acknowledgement have been issued in previous month.', 'width' => '14'),
					array('data' => '% of timely disposal{(4b+4d(i)/3)}*100<br>(6)', 'field' => 'No. of application rejected during the month for which acknowledgement have been issued in previous month.', 'width' => '14'),
					array('data' => '% of favour of applicants{(4b/3)}*100<br>(7)', 'field' => 'No. of application rejected during the month for which acknowledgement have been issued in previous month.', 'width' => '14'),
				
			);
			
	
			
	$exa = $exb = $exc = $exExpct = 0;
	$exn = 0; 
	$ex4di = $ex4dii = $ex4diiRTPS = 0;	
	
	if($dist_code != "" && $subdiv_code == ""){
		if ($district_code!='all'){
			$a1 = db_select('sub_division', 't');
			$a1->fields('t', array('sub_div_code','sub_div_name','district_name'));
			$a1->condition('t.district_code', $dist_code, '=');
			$flag=1;
		}else{
			$flag=0;
			$a1 = db_select('sub_division', 't');
			$a1->fields('t', array('district_name','district_code'));
		}
		
		$a2 = $a1->execute();
	}
	// Total No of Certificate Issue
	$query = db_select('l_remarks_master','r');
	$query->leftJoin('l_clra_registration_master', 'clra', 'clra.id  = r.application_id');
	$query->fields('r', array('application_id', 'remark_date','remark_id'));
	$query->condition('r.remark_date', $fdt, '>=');	
	$query->condition('r.remark_date', $tdt, '<=');
	if($dist_code != "" && $subdiv_code != ""){
		$query->condition('clra.e_postal_dist', $dist_code, '<=');
		$query->condition('clra.e_postal_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query->condition('clra.e_postal_dist', $dist_code, '<=');
	}
	$query->condition('r.remark_type', 'I', '=');
	$query->condition('r.is_active', 1, '=');
	$result = $query->execute();
	$result->rowCount();
	//  Point 3 No of FORM-I Issue
	$query3 = "select application_id,count(application_id) from l_remarks_master
				where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='S'
				group by application_id
				order by count asc
				";
	
	$result3 = db_query($query3)->fetchAll();
	$point_3=$result->rowCount();
	
	$reject = 0;
	foreach($result3 as $r3){
		$resuluRject = db_query("select application_id from l_remarks_master where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='R' and application_id = ".$r3->application_id."")->fetchObject();
		if(!empty($resuluRject)){
			$reject++;
		}
	}
	//  Point 4 
	$query4a = db_select('l_remarks_master', 'r');	
	$query4a->leftJoin('l_clra_registration_master', 'clra', 'clra.id  = r.application_id');
	$query4a->fields('r', array('application_id', 'remark_date','remark_id'));
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('clra.e_postal_dist', $dist_code, '<=');
		$query4a->condition('clra.e_postal_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('clra.e_postal_dist', $dist_code, '<=');
	}
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'I', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			$query6 = db_select('l_remarks_master', 'r');	
			$query6->leftJoin('l_clra_registration_master', 'clra', 'clra.id  = r.application_id');
			$query6->fields('r', array('application_id', 'remark_date','remark_id'));
			$query6->condition('r.remark_date', $fdt, '>=');	
			$query6->condition('r.remark_date', $tdt, '<=');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('clra.e_postal_dist', $dist_code, '<=');
				$query6->condition('clra.e_postal_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('clra.e_postal_dist', $dist_code, '<=');
			}
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('remark_id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->remark_id;
			$min_remark_date1 = ($data4a->remark_date - (60*60*24*30));
			
			if($min_remark_date1 <=$remark_date1 && $remark_date1 < $fdt){
				$exa++;
			}
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$exb++;
			}
		}
	}
	
	$query4a = db_select('l_remarks_master', 'r');	
	$query4a->leftJoin('l_clra_registration_master', 'clra', 'clra.id  = r.application_id');
	$query4a->fields('r', array('application_id', 'remark_date','remark_id'));
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('clra.e_postal_dist', $dist_code, '<=');
		$query4a->condition('clra.e_postal_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('clra.e_postal_dist', $dist_code, '<=');
	}
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'R', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			$query6 = db_select('l_remarks_master', 'r');	
			$query6->leftJoin('l_clra_registration_master', 'clra', 'clra.id  = r.application_id');
			$query6->fields('r', array('application_id', 'remark_date','remark_id'));
			$query6->condition('r.remark_date', $fdt, '>=');	
			$query6->condition('r.remark_date', $tdt, '<=');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('clra.e_postal_dist', $dist_code, '<=');
				$query6->condition('clra.e_postal_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('clra.e_postal_dist', $dist_code, '<=');
			}
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('remark_id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->remark_id;
			$max_remark_date1 = ($remark_date1 + (60*60*24*30)); 
			
			if($remark_date1 <= $fdt ){
				$ex4di++;
			}
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$ex4dii++;
			}
			if($remark_date1 <= $max_remark_date1 ){
				$ex4diiRTPS++;
			}
		}
	}
	
	$row_5=$exa+$exb;
	$row_4d=$point_3-$exb;
	$row_6=(($exb+$ex4di)/$point_3)*100;
	$row_7=	($exb/$point_3)*100;
	
	$rows[] = array(
				array('data' =>	'1'),
				array('data' =>	'Registration and Amendment of Certificate of of Registration of Establishment of Principal Employer under the CLRA Act, 1970'),
				array('data' =>	$point_3),
				array('data' =>	$exa),
				array('data' => $exb),
				array('data' => ($exExpct)),
				array('data' => $ex4di),
				array('data' => $ex4dii),
				array('data' => round($row_5, 2)),
				array('data' => round($row_6, 2)),
				array('data' => round($row_7, 2))
		);
		
	/////////////CLRA END////
	$bexa = $bexb = $bexc = $bexExpct = 0;
	$bexn = 0; 
	$bex4di = $bex4dii = $bex4diiRTPS = 0;	
	// Total No of Certificate Issue
	$query = db_select('l_bocwa_remark_master','r');
	$query->leftJoin('l_bocwa_registration_master', 'bocwa', 'bocwa.id  = r.application_id');
	$query->fields('r', array('application_id', 'remark_date','id'));
	if($dist_code != "" && $subdiv_code != ""){
		$query->condition('bocwa.loc_e_dist', $dist_code, '<=');
		$query->condition('bocwa.loc_e_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query->condition('bocwa.loc_e_dist', $dist_code, '<=');
	}
	$query->condition('r.remark_date', $fdt, '>=');	
	$query->condition('r.remark_date', $tdt, '<=');
	$query->condition('r.remark_type', 'I', '=');
	$query->condition('r.is_active', 1, '=');
	$result = $query->execute();
	$result->rowCount();
	//  Point 3 No of FORM-I Issue
	$query3 = "select application_id,count(application_id) from l_bocwa_remark_master
				where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='S'
				group by application_id
				order by count asc
				";
	$result3 = db_query($query3)->fetchAll();
	$bocwa_3=count($result3);
	$breject = 0;
	foreach($result3 as $r3){
		$resuluRject = db_query("select application_id from l_bocwa_remark_master where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='R' and application_id = ".$r3->application_id."")->fetchObject();
		
		if(!empty($resuluRject)){
			$breject++;
		}
	}
	//  Point 4 
	$query4a = db_select('l_bocwa_remark_master', 'r');	
	$query4a->fields('r', array('application_id', 'remark_date','id'));
	$query4a->leftJoin('l_bocwa_registration_master', 'bocwa', 'bocwa.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('bocwa.loc_e_dist', $dist_code, '<=');
		$query4a->condition('bocwa.loc_e_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('bocwa.loc_e_dist', $dist_code, '<=');
	}
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'I', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_bocwa_remark_master', 'r');	
			$query6->fields('r', array('application_id', 'remark_date','id'));
			$query6->leftJoin('l_bocwa_registration_master', 'bocwa', 'bocwa.id  = r.application_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('bocwa.loc_e_dist', $dist_code, '<=');
				$query6->condition('bocwa.loc_e_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('bocwa.loc_e_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			$min_remark_date1 = ($data4a->remark_date - (60*60*24*30));
			//  Point 4a			
			if($min_remark_date1 <= $remark_date1 && $remark_date1 < $fdt){
				$bexa++;
			}
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$bexb++;
			}
		}
	}
	// Point 4d
	$query4a = db_select('l_bocwa_remark_master', 'r');	
	$query4a->fields('r', array('application_id', 'remark_date','id'));
	$query4a->leftJoin('l_bocwa_registration_master', 'bocwa', 'bocwa.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('bocwa.loc_e_dist', $dist_code, '<=');
		$query4a->condition('bocwa.loc_e_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('bocwa.loc_e_dist', $dist_code, '<=');
	}		
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'R', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_bocwa_remark_master', 'r');	
			$query6->fields('r', array('application_id', 'remark_date','id'));
			$query6->leftJoin('l_bocwa_registration_master', 'bocwa', 'bocwa.id  = r.application_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('bocwa.loc_e_dist', $dist_code, '<=');
				$query6->condition('bocwa.loc_e_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('bocwa.loc_e_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			$max_remark_date1 = ($remark_date1 + (60*60*24*30)); 
			//  Point 4di			
			if($remark_date1 <= $fdt ){
				$bex4di++;
			}
			//  Point 4dii
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$bex4dii++;
			}
			if($remark_date1 <= $max_remark_date1 ){
				$bex4diiRTPS++;
			}
			
		}
	}
	
	$brow_5=$bexa+$bexb;
	$row_4d=$point_3-$exb;
	$brow_6=(($bexb+$bex4di)/$bocwa_3)*100;
	$brow_7=($bexb/$bocwa_3)*100;
	
	$rows[] = array(
				array('data' =>	'2'),
				array('data' =>	'Registration and amendment of Certificate of registration of establishment under the BOCWA Act,1996'),
				array('data' =>	$bocwa_3),
				array('data' =>	$bexa),
				array('data' => $bexb),
				array('data' => ($bexExpct)),
				array('data' => $bex4di),
				array('data' => $bex4dii),
				array('data' => round($brow_5, 2)),
				array('data' => round($brow_6, 2)),
				array('data' => round($brow_7, 2))
		);
	////BOCWA End//
	/// MTW
	$mexa = $mexb = $mexc = $mexExpct = 0;
	$mexn = 0; 
	$mex4di = $mex4dii = $mex4diiRTPS = 0;	
	// Total No of Certificate Issue
	$query = db_select('l_mtw_remark_master','r');
	$query->fields('r', array('application_id', 'remark_date','id'));
	$query->leftJoin('l_mtw_registration_master', 'mtw', 'mtw.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query->condition('mtw.mtw_loc_dist', $dist_code, '<=');
		$query->condition('mtw.mtw_loc_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query->condition('mtw.mtw_loc_dist', $dist_code, '<=');
	}	
	$query->condition('r.remark_date', $fdt, '>=');	
	$query->condition('r.remark_date', $tdt, '<=');
	$query->condition('r.remark_type', 'I', '=');
	$query->condition('r.is_active', 1, '=');
	$result = $query->execute();
	$result->rowCount();
	//  Point 3 No of FORM-I Issue
	$query3 = "select application_id,count(application_id) from l_mtw_remark_master
				where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='S'
				group by application_id
				order by count asc
				";
	$result3 = db_query($query3)->fetchAll();
	$mtw_point3=count($result3);
	$mreject = 0;
	foreach($result3 as $r3){
		$resuluRject = db_query("select application_id from l_mtw_remark_master where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='R' and application_id = ".$r3->application_id."")->fetchObject();
		
		if(!empty($resuluRject)){
			$mreject++;
		}
	}
	//echo 'Rejct this month->'.$mreject;
	//  Point 4 
	$query4a = db_select('l_mtw_remark_master', 'r');	
	$query4a ->fields('r', array('application_id', 'remark_date','id'));
	$query4a->leftJoin('l_mtw_registration_master', 'mtw', 'mtw.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('mtw.mtw_loc_dist', $dist_code, '<=');
		$query4a->condition('mtw.mtw_loc_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('mtw.mtw_loc_dist', $dist_code, '<=');
	}		
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'I', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_mtw_remark_master', 'r');	
			$query6->fields('r', array('remark_date','id','application_id'));
			$query6->leftJoin('l_mtw_registration_master', 'mtw', 'mtw.id  = r.application_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('mtw.mtw_loc_dist', $dist_code, '<=');
				$query6->condition('mtw.mtw_loc_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('mtw.mtw_loc_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			$min_remark_date1 = ($data4a->remark_date - (60*60*24*30));
			
			if($min_remark_date1 <= $remark_date1 && $remark_date1 < $fdt){
				$mexa++;
			}
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$mexb++;
			}
		}
	}
	// Point 4d
	$query4a = db_select('l_mtw_remark_master', 'r');	
	$query4a ->fields('r', array('application_id', 'remark_date','id'));
	$query4a->leftJoin('l_mtw_registration_master', 'mtw', 'mtw.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('mtw.mtw_loc_dist', $dist_code, '<=');
		$query4a->condition('mtw.mtw_loc_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('mtw.mtw_loc_dist', $dist_code, '<=');
	}		
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'R', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_mtw_remark_master', 'r');	
			$query6->fields('r', array('remark_date','id','application_id'));
			$query6->leftJoin('l_mtw_registration_master', 'mtw', 'mtw.id  = r.application_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('mtw.mtw_loc_dist', $dist_code, '<=');
				$query6->condition('mtw.mtw_loc_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('mtw.mtw_loc_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			$max_remark_date1 = ($remark_date1 + (60*60*24*30)); 
			//  Point 4di			
			if($remark_date1 <= $fdt ){
				$mex4di++;
			}
			//  Point 4dii
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$mex4dii++;
			}
			if($remark_date1 <= $max_remark_date1 ){
				$mex4diiRTPS++;
			}
			
		}
	}
	$mrow_5=$mexa+$mexb;
	$row_4d=$point_3-$exb;
	$mrow_6=(($mexb+$mex4di)/$mtw_point3)*100;
	$mrow_7=($mexb/$mtw_point3)*100;
	
	$rows[] = array(
				array('data' =>	'3'),
				array('data' =>	'Registration and amendment,Renewal of certificate of registration of the MTW Act,1961 and Rules thereunder'),
				array('data' =>	$mtw_point3),
				array('data' =>	$mexa),
				array('data' => $mexb),
				array('data' => (count($result3)-$mexb)),
				array('data' => $mex4di),
				array('data' => $mex4dii),
				array('data' => round($mrow_5, 2)),
				array('data' => round($mrow_6, 2)),
				array('data' => round($mrow_7, 2))
					
		);
	//////////ISMW/////////////
	$iexa = $iexb = $iexc = $iexExpct = 0;
	$iexn = 0; 
	$iex4di = $iex4dii = $iex4diiRTPS = 0;	
	// Total No of Certificate Issue
	$query = db_select('l_interstate_remark_master','r');
	$query->fields('r', array('application_id', 'remark_date','id'));
	$query->leftJoin('l_interstate_workman_master', 'inst', 'inst.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query->condition('inst.loc_e_dist', $dist_code, '<=');
		$query->condition('inst.loc_e_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query->condition('inst.loc_e_dist', $dist_code, '<=');
	}	
	$query->condition('r.remark_date', $fdt, '>=');	
	$query->condition('r.remark_date', $tdt, '<=');
	$query->condition('r.remark_type', 'I', '=');
	$query->condition('r.is_active', 1, '=');
	$result = $query->execute();
	$result->rowCount();
	//  Point 3 No of FORM-I Issue
	$query3 = "select application_id,count(application_id) from l_interstate_remark_master
				where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='S'
				group by application_id
				order by count asc
				";
	$result3 = db_query($query3)->fetchAll();
	$I_point3=count($result3);
	$ireject = 0;
	foreach($result3 as $r3){
		$resuluRject = db_query("select application_id from l_interstate_remark_master where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='R' and application_id = ".$r3->application_id."")->fetchObject();
		
		if(!empty($resuluRject)){
			$ireject++;
		}
	}
	$ireject;
	//  Point 4 
	$query4a = db_select('l_interstate_remark_master', 'r');	
	$query4a ->fields('r', array('application_id', 'remark_date','id'));
	$query4a->leftJoin('l_interstate_workman_master', 'inst', 'inst.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('inst.loc_e_dist', $dist_code, '<=');
		$query4a->condition('inst.loc_e_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('inst.loc_e_dist', $dist_code, '<=');
	}			
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'I', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_interstate_remark_master', 'r');	
			$query6->fields('r', array('remark_date','id','application_id'));
			$query6->leftJoin('l_interstate_workman_master', 'inst', 'inst.id  = r.application_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('inst.loc_e_dist', $dist_code, '<=');
				$query6->condition('inst.loc_e_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('inst.loc_e_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			
			$min_remark_date1 = ($data4a->remark_date - (60*60*24*30));
			if($min_remark_date1 <= $remark_date1 && $remark_date1 < $fdt){
				$iexa++;
			}
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$iexb++;
			}
			
		}
	}
	
	// Point 4d
	$query4a = db_select('l_interstate_remark_master', 'r');	
	$query4a ->fields('r', array('application_id', 'remark_date','id'));
	$query4a->leftJoin('l_interstate_workman_master', 'inst', 'inst.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('inst.loc_e_dist', $dist_code, '<=');
		$query4a->condition('inst.loc_e_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('inst.loc_e_dist', $dist_code, '<=');
	}		
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'R', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_interstate_remark_master', 'r');	
			$query6->fields('r', array('remark_date','id','application_id'));
			$query6->leftJoin('l_interstate_workman_master', 'inst', 'inst.id  = r.application_id');
			$query6->condition('r.remark_type', 'S', '=');	
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('inst.loc_e_dist', $dist_code, '<=');
				$query6->condition('inst.loc_e_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('inst.loc_e_dist', $dist_code, '<=');
			}	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			$max_remark_date1 = ($remark_date1 + (60*60*24*30)); 
			//  Point 4di			
			if($remark_date1 <= $fdt ){
				$iex4di++;
			}
			//  Point 4dii
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$iex4dii++;
			}
			if($remark_date1 <= $max_remark_date1 ){
				$iex4diiRTPS++;
			}
		}
	}
	$irow_5=$iexa+$iexb;
	$row_4d=$point_3-$exb;
	$irow_6=(($iexb+$iex4di)/$I_point3)*100;
	$mrow_7=($iexb/$I_point3)*100;
	
	$rows[] = array(
				array('data' =>	'4'),
				array('data' =>	'Registration and amendment of certificate of registration of establishment of principal Employer under the ISMW Act,1979'),
				array('data' =>	$I_point3),
				array('data' =>	$iexa),
				array('data' => $iexb),
				array('data' => count($result3)-$iexb),
				array('data' => $iex4di),
				array('data' => $iex4dii),
				array('data' => round($irow_5, 2)),
				array('data' => round($irow_6, 2)),
				array('data' => round($mrow_7, 2))
		);
		
	
	$lexa = $lexb = $lexc = $lexExpct = 0;
	$lexn = 0; 
	$lex4di = $lex4dii = $lex4diiRTPS = 0;	
	// Total No of Certificate Issue
	$query = db_select('l_remark_license','r');
	$query->fields('r', array('particular_id', 'remark_date','id'));
	$query->leftJoin('l_particular_of_contract_labour', 'lince', 'lince.id  = r.particular_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query->condition('lince.worksite_dist', $dist_code, '<=');
		$query->condition('lince.worksite_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query->condition('lince.worksite_dist', $dist_code, '<=');
	}	
	$query->condition('r.remark_date', $fdt, '>=');	
	$query->condition('r.remark_date', $tdt, '<=');
	$query->condition('r.remark_type', 'I', '=');
	$query->condition('r.is_active', 1, '=');
	$result = $query->execute();
	$result->rowCount();
	//  Point 3 No of FORM-I Issue
	$query3 = "select particular_id,count(particular_id) from l_remark_license
				where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='U'
				group by particular_id
				order by count asc
				";
	
	$result3 = db_query($query3)->fetchAll();
	$li_point3=count($result3);
	$lreject = 0;
	foreach($result3 as $r3){
		$resuluRject = db_query("select particular_id from l_remark_license where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='R' and particular_id = ".$r3->particular_id."")->fetchObject();
		
		if(!empty($resuluRject)){
			$lreject++;
		}
	}
	$lreject;
	//  Point 4 
	$query4a = db_select('l_remark_license', 'r');	
	$query4a ->fields('r', array('particular_id', 'remark_date','id'));
	$query4a->leftJoin('l_particular_of_contract_labour', 'lince', 'lince.id  = r.particular_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('lince.worksite_dist', $dist_code, '<=');
		$query4a->condition('lince.worksite_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('lince.worksite_dist', $dist_code, '<=');
	}		
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'I', '=');
	$query4a->condition('r.is_active', 1, '=');		
	
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_remark_license', 'r');	
			$query6->fields('r', array('remark_date','id','particular_id'));
			$query6->leftJoin('l_particular_of_contract_labour', 'lince', 'lince.id  = r.particular_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('lince.worksite_dist', $dist_code, '<=');
				$query6->condition('lince.worksite_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('lince.worksite_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'U', '=');	
			$query6->condition('r.particular_id', $data4a->particular_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			$min_remark_date1 = ($data4a->remark_date - (60*60*24*30));
			if($min_remark_date1 <= $remark_date1 && $remark_date1 < $fdt){
				$lexa++;
			}
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$lexb++;
			}
		}
	}
	// Point 4d
	$query4a = db_select('l_remark_license', 'r');	
	$query4a ->fields('r', array('particular_id', 'remark_date','id'));
	$query4a->leftJoin('l_particular_of_contract_labour', 'lince', 'lince.id  = r.particular_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('lince.worksite_dist', $dist_code, '<=');
		$query4a->condition('lince.worksite_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('lince.worksite_dist', $dist_code, '<=');
	}		
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'R', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_remark_license', 'r');	
			$query6->fields('r', array('remark_date','id','particular_id'));
			$query6->leftJoin('l_particular_of_contract_labour', 'lince', 'lince.id  = r.particular_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('lince.worksite_dist', $dist_code, '<=');
				$query6->condition('lince.worksite_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('lince.worksite_dist', $dist_code, '<=');
			}
			$query6->condition('r.remark_type', 'U', '=');	
			$query6->condition('r.particular_id', $data4a->particular_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			$max_remark_date1 = ($remark_date1 + (60*60*24*30)); 
			//  Point 4di			
			if($remark_date1 <= $fdt ){
				$lex4di++;
			}
			//  Point 4dii
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$lex4dii++;
			}
			if($remark_date1 <= $max_remark_date1 ){
				$lex4diiRTPS++;
			}
		}
	}
	$lrow_5=$lexa+$lexb;
	$row_4d=$point_3-$exb;
	$lrow_6=(($lexb+$lex4di)/$li_point3)*100;
	$lrow_7=($lexb/$li_point3)*100;
	$lrow_c=(count($result3)-$lexb);
	
	$rows[] = array(
				array('data' =>	'5'),
				array('data' =>	'Licensing of Contractors and Amendment, Renewal of Licence under the CLRA Act,1970'),
				array('data' =>	$li_point3),
				array('data' =>	$lexa),
				array('data' => $lexb),
				array('data' => (count($result3)-$lexb)),
				array('data' => $lex4di),
				array('data' => $lex4dii),
				array('data' => round($lrow_5, 2)),
				array('data' => round($lrow_6, 2)),
				array('data' => round($lrow_7, 2))
		);
			
	$variables = array('attributes' => array('class' => array('box-body table-responsive no-padding table table-hover admin-custom-table x-s')), 'header' => $header, 'rows' => $rows, 'empty' => t("No data found!"));
	$output = theme('datatable', $variables);
				
	return '<div class="col-md-12 ">MONTHLY REPORT'.$print_xls.''.$print_link.'<br/><div class="col-md-12" style="overflow-x:auto;overflow:scroll;max-height:500px;">'.$output.'</div></div>' ; 
	
}

function get_monthly_report_formapdf($form,&$form_state,$fdt,$tdt,$dist_code,$subdiv_code){
	
	$tdt=encryption_decryption_fun('decrypt',$tdt);
	$fdt=encryption_decryption_fun('decrypt',$fdt);
	$dist_code=encryption_decryption_fun('decrypt',$dist_code);
	$subdiv_code=encryption_decryption_fun('decrypt',$subdiv_code);
	$to_date=date("Y-m-d", $tdt);
	$form_date=date("Y-m-d", $fdt);
	$date=explode("-",$to_date);
	
	$exa = $exb = $exc = $exExpct = 0;
	$exn = 0; 
	$ex4di = $ex4dii = $ex4diiRTPS = 0;	
	// Total No of Certificate Issue
	$query = db_select('l_remarks_master','r');
	$query->fields('r', array('application_id', 'remark_date','remark_id'));
	$query->leftJoin('l_clra_registration_master', 'clra', 'clra.id  = r.application_id');
	$query->fields('r', array('application_id', 'remark_date','remark_id'));
	$query->condition('r.remark_date', $fdt, '>=');	
	$query->condition('r.remark_date', $tdt, '<=');
	if($dist_code != "" && $subdiv_code != ""){
		$query->condition('clra.e_postal_dist', $dist_code, '<=');
		$query->condition('clra.e_postal_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query->condition('clra.e_postal_dist', $dist_code, '<=');
	}
	$query->condition('r.remark_date', $fdt, '>=');	
	$query->condition('r.remark_date', $tdt, '<=');
	$query->condition('r.remark_type', 'I', '=');
	$query->condition('r.is_active', 1, '=');
	$result = $query->execute();
	$result->rowCount();
	//  Point 3 No of FORM-I Issue
	$query3 = "select application_id,count(application_id) from l_remarks_master
				where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='S'
				group by application_id
				order by count asc
				";
	
	$result3 = db_query($query3)->fetchAll();
	$point_3=$result->rowCount();
	
	$reject = 0;
	foreach($result3 as $r3){
		$resuluRject = db_query("select application_id from l_remarks_master where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='R' and application_id = ".$r3->application_id."")->fetchObject();
		
		if(!empty($resuluRject)){
			$reject++;
		}
	}
	//  Point 4 
	$query4a = db_select('l_remarks_master', 'r');	
	$query4a ->fields('r', array('application_id', 'remark_date','remark_id'));
	$query4a->leftJoin('l_clra_registration_master', 'clra', 'clra.id  = r.application_id');
	$query4a->fields('r', array('application_id', 'remark_date','remark_id'));
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('clra.e_postal_dist', $dist_code, '<=');
		$query4a->condition('clra.e_postal_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('clra.e_postal_dist', $dist_code, '<=');
	}	
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'I', '=');
	$query4a->condition('r.is_active', 1, '=');		
	
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_remarks_master', 'r');	
			$query6->fields('r', array('remark_date','remark_id','application_id'));
			$query6->leftJoin('l_clra_registration_master', 'clra', 'clra.id  = r.application_id');
			$query6->fields('r', array('application_id', 'remark_date','remark_id'));
			$query6->condition('r.remark_date', $fdt, '>=');	
			$query6->condition('r.remark_date', $tdt, '<=');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('clra.e_postal_dist', $dist_code, '<=');
				$query6->condition('clra.e_postal_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('clra.e_postal_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('remark_id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->remark_id;
			
			$min_remark_date1 = ($data4a->remark_date - (60*60*24*30));
			
			
			if($min_remark_date1 <=$remark_date1 && $remark_date1 < $fdt){
				$exa++;
			}
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$exb++;
			}
		}
	}
	
	$query4a = db_select('l_remarks_master', 'r');	
	$query4a ->fields('r', array('application_id', 'remark_date','remark_id'));
	$query4a->fields('r', array('remark_date','remark_id','application_id'));
	$query4a->leftJoin('l_clra_registration_master', 'clra', 'clra.id  = r.application_id');
	$query4a->fields('r', array('application_id', 'remark_date','remark_id'));
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('clra.e_postal_dist', $dist_code, '<=');
		$query4a->condition('clra.e_postal_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('clra.e_postal_dist', $dist_code, '<=');
	}		
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'R', '=');
	$query4a->condition('r.is_active', 1, '=');		
	
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_remarks_master', 'r');	
			$query6->fields('r', array('remark_date','remark_id','application_id'));
			$query6->leftJoin('l_clra_registration_master', 'clra', 'clra.id  = r.application_id');
			$query6->fields('r', array('application_id', 'remark_date','remark_id'));
			$query6->condition('r.remark_date', $fdt, '>=');	
			$query6->condition('r.remark_date', $tdt, '<=');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('clra.e_postal_dist', $dist_code, '<=');
				$query6->condition('clra.e_postal_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('clra.e_postal_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('remark_id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->remark_id;
			
			$max_remark_date1 = ($remark_date1 + (60*60*24*30)); 
			
		
			if($remark_date1 <= $fdt ){
				$ex4di++;
			}

			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$ex4dii++;
			}
			if($remark_date1 <= $max_remark_date1 ){
				$ex4diiRTPS++;
			}
		}
	}
	$row_5=$exa+$exb;
	$row_4d=$point_3-$exb;
	$row_6=(($exb+$ex4di)/$point_3)*100;
	$row_7=	($exb/$point_3)*100;
	//////clara end///
	//////////bocwstart///
	$bexa = $bexb = $bexc = $bexExpct = 0;
	$bexn = 0; 
	$bex4di = $bex4dii = $bex4diiRTPS = 0;	
	// Total No of Certificate Issue
	$query = db_select('l_bocwa_remark_master','r');
	$query->fields('r', array('application_id', 'remark_date','id'));
	$query->leftJoin('l_bocwa_registration_master', 'bocwa', 'bocwa.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query->condition('bocwa.loc_e_dist', $dist_code, '<=');
		$query->condition('bocwa.loc_e_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query->condition('bocwa.loc_e_dist', $dist_code, '<=');
	}	
	$query->condition('r.remark_date', $fdt, '>=');	
	$query->condition('r.remark_date', $tdt, '<=');
	$query->condition('r.remark_type', 'I', '=');
	$query->condition('r.is_active', 1, '=');
	$result = $query->execute();
	$result->rowCount();
	//  Point 3 No of FORM-I Issue
	$query3 = "select application_id,count(application_id) from l_bocwa_remark_master
				where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='S'
				group by application_id
				order by count asc
				";
	
	$result3 = db_query($query3)->fetchAll();
	$bocwa_3=count($result3);
	$breject = 0;
	foreach($result3 as $r3){
		$resuluRject = db_query("select application_id from l_bocwa_remark_master where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='R' and application_id = ".$r3->application_id."")->fetchObject();
		
		if(!empty($resuluRject)){
			$breject++;
		}
	}
	//  Point 4 
	$query4a = db_select('l_bocwa_remark_master', 'r');	
	$query4a ->fields('r', array('application_id', 'remark_date','id'));
	$query->leftJoin('l_bocwa_registration_master', 'bocwa', 'bocwa.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query->condition('bocwa.loc_e_dist', $dist_code, '<=');
		$query->condition('bocwa.loc_e_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query->condition('bocwa.loc_e_dist', $dist_code, '<=');
	}		
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'I', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_bocwa_remark_master', 'r');	
			$query6->fields('r', array('remark_date','id','application_id'));
			$query6->leftJoin('l_bocwa_registration_master', 'bocwa', 'bocwa.id  = r.application_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('bocwa.loc_e_dist', $dist_code, '<=');
				$query6->condition('bocwa.loc_e_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('bocwa.loc_e_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			$min_remark_date1 = ($data4a->remark_date - (60*60*24*30));
			//  Point 4a			
			if($min_remark_date1 <= $remark_date1 && $remark_date1 < $fdt){
				$bexa++;
			}
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$bexb++;
			}

		}
	}
	// Point 4d
	$query4a = db_select('l_bocwa_remark_master', 'r');	
	$query4a ->fields('r', array('application_id', 'remark_date','id'));
	$query4a->leftJoin('l_bocwa_registration_master', 'bocwa', 'bocwa.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('bocwa.loc_e_dist', $dist_code, '<=');
		$query4a->condition('bocwa.loc_e_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('bocwa.loc_e_dist', $dist_code, '<=');
	}		
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'R', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_bocwa_remark_master', 'r');	
			$query6->fields('r', array('remark_date','id','application_id'));
			$query6->leftJoin('l_bocwa_registration_master', 'bocwa', 'bocwa.id  = r.application_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('bocwa.loc_e_dist', $dist_code, '<=');
				$query6->condition('bocwa.loc_e_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('bocwa.loc_e_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			
			$max_remark_date1 = ($remark_date1 + (60*60*24*30)); 
			//  Point 4di			
			if($remark_date1 <= $fdt ){
				$bex4di++;
			}
			//  Point 4dii
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$bex4dii++;
			}
			if($remark_date1 <= $max_remark_date1 ){
				$bex4diiRTPS++;
			}
		}
	}
	
	$brow_5=$bexa+$bexb;
	$row_4d=$point_3-$exb;
	$brow_6=(($bexb+$bex4di)/$bocwa_3)*100;
	$brow_7=($bexb/$bocwa_3)*100;
	////bocwend////
	/// MTW
	$mexa = $mexb = $mexc = $mexExpct = 0;
	$mexn = 0; 
	$mex4di = $mex4dii = $mex4diiRTPS = 0;	
	// Total No of Certificate Issue
	$query = db_select('l_mtw_remark_master','r');
	$query->fields('r', array('application_id', 'remark_date','id'));
	$query->leftJoin('l_mtw_registration_master', 'mtw', 'mtw.id  = r.application_id');
	$query->condition('r.remark_date', $fdt, '>=');	
	$query->condition('r.remark_date', $tdt, '<=');
	if($dist_code != "" && $subdiv_code != ""){
		$query->condition('mtw.mtw_loc_dist', $dist_code, '<=');
		$query->condition('mtw.mtw_loc_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query->condition('mtw.mtw_loc_dist', $dist_code, '<=');
	}	
	$query->condition('r.remark_type', 'I', '=');
	$query->condition('r.is_active', 1, '=');
	$result = $query->execute();
	$result->rowCount();
	//  Point 3 No of FORM-I Issue
	$query3 = "select application_id,count(application_id) from l_mtw_remark_master
				where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='S'
				group by application_id
				order by count asc
				";
	$result3 = db_query($query3)->fetchAll();
	$mtw_point3=count($result3);
	$mreject = 0;
	foreach($result3 as $r3){
		$resuluRject = db_query("select application_id from l_mtw_remark_master where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='R' and application_id = ".$r3->application_id."")->fetchObject();
		
		if(!empty($resuluRject)){
			$mreject++;
		}
	}
	//echo 'Rejct this month->'.$mreject;
	//  Point 4 
	$query4a = db_select('l_mtw_remark_master', 'r');	
	$query4a ->fields('r', array('application_id', 'remark_date','id'));
	$query4a->leftJoin('l_mtw_registration_master', 'mtw', 'mtw.id  = r.application_id');	
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('mtw.mtw_loc_dist', $dist_code, '<=');
		$query4a->condition('mtw.mtw_loc_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('mtw.mtw_loc_dist', $dist_code, '<=');
	}	
	$query4a->condition('r.remark_type', 'I', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_mtw_remark_master', 'r');	
			$query6->fields('r', array('remark_date','id','application_id'));
			$query6->leftJoin('l_mtw_registration_master', 'mtw', 'mtw.id  = r.application_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('mtw.mtw_loc_dist', $dist_code, '<=');
				$query6->condition('mtw.mtw_loc_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('mtw.mtw_loc_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			$min_remark_date1 = ($data4a->remark_date - (60*60*24*30));
			if($min_remark_date1 <= $remark_date1 && $remark_date1 < $fdt){
				$mexa++;
			}
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$mexb++;
			}
			
		}
	}
	// Point 4d
	$query4a = db_select('l_mtw_remark_master', 'r');	
	$query4a ->fields('r', array('application_id', 'remark_date','id'));
	$query4a->leftJoin('l_mtw_registration_master', 'mtw', 'mtw.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('mtw.mtw_loc_dist', $dist_code, '<=');
		$query4a->condition('mtw.mtw_loc_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('mtw.mtw_loc_dist', $dist_code, '<=');
	}		
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'R', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_mtw_remark_master', 'r');	
			$query6->fields('r', array('remark_date','id','application_id'));
			$query6->leftJoin('l_mtw_registration_master', 'mtw', 'mtw.id  = r.application_id');
			$query6->condition('r.remark_type', 'S', '=');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('mtw.mtw_loc_dist', $dist_code, '<=');
				$query6->condition('mtw.mtw_loc_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('mtw.mtw_loc_dist', $dist_code, '<=');
			}		
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			$max_remark_date1 = ($remark_date1 + (60*60*24*30)); 
			//  Point 4di			
			if($remark_date1 <= $fdt ){
				$mex4di++;
			}
			//  Point 4dii
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$mex4dii++;
			}
			if($remark_date1 <= $max_remark_date1 ){
				$mex4diiRTPS++;
			}
		}
	}
	
	$mrow_5=$mexa+$mexb;
	$mrow_6=(($mexb+$mex4di)/$mtw_point3)*100;
	$mrow_7=($mexb/$mtw_point3)*100;
	$mrow_4dx=(count($result3)-$mexb);
	///mtw end///
	//////////ISMW/////////////
	$iexa = $iexb = $iexc = $iexExpct = 0;
	$iexn = 0; 
	$iex4di = $iex4dii = $iex4diiRTPS = 0;	
	//echo '<br>ISMW ----';
	// Total No of Certificate Issue
	$query = db_select('l_interstate_remark_master','r');
	$query->fields('r', array('application_id', 'remark_date','id'));
	$query->leftJoin('l_interstate_workman_master', 'inst', 'inst.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query->condition('inst.loc_e_dist', $dist_code, '<=');
		$query->condition('inst.loc_e_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query->condition('inst.loc_e_dist', $dist_code, '<=');
	}	
	$query->condition('r.remark_date', $fdt, '>=');	
	$query->condition('r.remark_date', $tdt, '<=');
	$query->condition('r.remark_type', 'I', '=');
	$query->condition('r.is_active', 1, '=');
	$result = $query->execute();
	$result->rowCount();
	
	//  Point 3 No of FORM-I Issue
		
	$query3 = "select application_id,count(application_id) from l_interstate_remark_master
				where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='S'
				group by application_id
				order by count asc
				";
	
	$result3 = db_query($query3)->fetchAll();
	
	$I_point3=count($result3);
	
	$ireject = 0;
	foreach($result3 as $r3){
		$resuluRject = db_query("select application_id from l_interstate_remark_master where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='R' and application_id = ".$r3->application_id."")->fetchObject();
		
		if(!empty($resuluRject)){
			$ireject++;
		}
	}
	$ireject;
	//  Point 4 
	$query4a = db_select('l_interstate_remark_master', 'r');	
	$query4a ->fields('r', array('application_id', 'remark_date','id'));
	$query4a->leftJoin('l_interstate_workman_master', 'inst', 'inst.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('inst.loc_e_dist', $dist_code, '<=');
		$query4a->condition('inst.loc_e_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('inst.loc_e_dist', $dist_code, '<=');
	}		
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'I', '=');
	$query4a->condition('r.is_active', 1, '=');		
	
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_interstate_remark_master', 'r');	
			$query6->fields('r', array('remark_date','id','application_id'));
			$query6->leftJoin('l_interstate_workman_master', 'inst', 'inst.id  = r.application_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('inst.loc_e_dist', $dist_code, '<=');
				$query6->condition('inst.loc_e_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('inst.loc_e_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			$min_remark_date1 = ($data4a->remark_date - (60*60*24*30));
			if($min_remark_date1 <= $remark_date1 && $remark_date1 < $fdt){
				$iexa++;
			}
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$iexb++;
			}
			
		}
	}
	
	// Point 4d
	$query4a = db_select('l_interstate_remark_master', 'r');	
	$query4a ->fields('r', array('application_id', 'remark_date','id'));
	$query4a->leftJoin('l_interstate_workman_master', 'inst', 'inst.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('inst.loc_e_dist', $dist_code, '<=');
		$query4a->condition('inst.loc_e_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('inst.loc_e_dist', $dist_code, '<=');
	}		
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'R', '=');
	$query4a->condition('r.is_active', 1, '=');		
	
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_interstate_remark_master', 'r');	
			$query6->fields('r', array('remark_date','id','application_id'));
			$query6->leftJoin('l_interstate_workman_master', 'inst', 'inst.id  = r.application_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('inst.loc_e_dist', $dist_code, '<=');
				$query6->condition('inst.loc_e_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('inst.loc_e_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			$max_remark_date1 = ($remark_date1 + (60*60*24*30)); 
			//  Point 4di			
			if($remark_date1 <= $fdt ){
				$iex4di++;
			}
			//  Point 4dii
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$iex4dii++;
			}
			if($remark_date1 <= $max_remark_date1 ){
				$iex4diiRTPS++;
			}

			
		}
	}
	$irow_5=$iexa+$iexb;
	$row_4d=$point_3-$exb;
	$irow_6=(($iexb+$iex4di)/$I_point3)*100;
	$irow_7=($iexb/$I_point3)*100;
	$ismwfourc=count($result3)-$iexb;
	/////////ismw end///////
	$lexa = $lexb = $lexc = $lexExpct = 0;
	$lexn = 0; 
	$lex4di = $lex4dii = $lex4diiRTPS = 0;	
	// Total No of Certificate Issue
	$query = db_select('l_remark_license','r');
	$query->fields('r', array('particular_id', 'remark_date','id'));
	$query->leftJoin('l_particular_of_contract_labour', 'lince', 'lince.id  = r.particular_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query->condition('lince.worksite_dist', $dist_code, '<=');
		$query->condition('lince.worksite_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query->condition('lince.worksite_dist', $dist_code, '<=');
	}	
	$query->condition('r.remark_date', $fdt, '>=');	
	$query->condition('r.remark_date', $tdt, '<=');
	$query->condition('r.remark_type', 'I', '=');
	$query->condition('r.is_active', 1, '=');
	$result = $query->execute();
	$result->rowCount();
	//  Point 3 No of FORM-I Issue
	$query3 = "select particular_id,count(particular_id) from l_remark_license
				where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='U'
				group by particular_id
				order by count asc
				";
	
	$result3 = db_query($query3)->fetchAll();
	$li_point3=count($result3);
	$lreject = 0;
	foreach($result3 as $r3){
		$resuluRject = db_query("select particular_id from l_remark_license where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='R' and particular_id = ".$r3->particular_id."")->fetchObject();
		
		if(!empty($resuluRject)){
			$lreject++;
		}
	}
	$lreject;
	//  Point 4 
	$query4a = db_select('l_remark_license', 'r');	
	$query4a ->fields('r', array('particular_id', 'remark_date','id'));
	$query4a->leftJoin('l_particular_of_contract_labour', 'lince', 'lince.id  = r.particular_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('lince.worksite_dist', $dist_code, '<=');
		$query4a->condition('lince.worksite_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('lince.worksite_dist', $dist_code, '<=');
	}		
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'I', '=');
	$query4a->condition('r.is_active', 1, '=');		
	
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_remark_license', 'r');	
			$query6->fields('r', array('remark_date','id','particular_id'));
			$query6->leftJoin('l_particular_of_contract_labour', 'lince', 'lince.id  = r.particular_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('lince.worksite_dist', $dist_code, '<=');
				$query6->condition('lince.worksite_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('lince.worksite_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'U', '=');	
			$query6->condition('r.particular_id', $data4a->particular_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			$min_remark_date1 = ($data4a->remark_date - (60*60*24*30));
			if($min_remark_date1 <= $remark_date1 && $remark_date1 < $fdt){
				$lexa++;
			}
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$lexb++;
			}
		}
	}
	// Point 4d
	$query4a = db_select('l_remark_license', 'r');	
	$query4a ->fields('r', array('particular_id', 'remark_date','id'));
	$query4a->leftJoin('l_particular_of_contract_labour', 'lince', 'lince.id  = r.particular_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('lince.worksite_dist', $dist_code, '<=');
		$query4a->condition('lince.worksite_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('lince.worksite_dist', $dist_code, '<=');
	}		
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'R', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_remark_license', 'r');	
			$query6->fields('r', array('remark_date','id','particular_id'));
			$query6->leftJoin('l_particular_of_contract_labour', 'lince', 'lince.id  = r.particular_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('lince.worksite_dist', $dist_code, '<=');
				$query6->condition('lince.worksite_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('lince.worksite_dist', $dist_code, '<=');
			}
			$query6->condition('r.remark_type', 'U', '=');	
			$query6->condition('r.particular_id', $data4a->particular_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			$max_remark_date1 = ($remark_date1 + (60*60*24*30)); 
			//  Point 4di			
			if($remark_date1 <= $fdt ){
				$lex4di++;
			}
			//  Point 4dii
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$lex4dii++;
			}
			if($remark_date1 <= $max_remark_date1 ){
				$lex4diiRTPS++;
			}
		}
	}
	$lrow_5=$lexa+$lexb;
	$row_4d=$point_3-$exb;
	$lrow_6=(($lexb+$lex4di)/$li_point3)*100;
	$lrow_7=($lexb/$li_point3)*100;
	$lrow_c=(count($result3)-$lexb);
	
	$output='<style>
			table.clra_report { border-top:0px solid #000;border-left:0px solid #000;font-family:Arial;border-collapse: collapse; }
			.clra_report td{padding: 6px; border-right:1px solid #000;border-bottom:1px solid #000; font-size:14px;}
			.clra_report th { font-size:26px; font-weight:400; text-align:left; vertical-align:top; }
			.clra_report td.border-left, .clra_report th.border-left { border-left:1px solid #000; }
			.clra_report td.border-none { border:none; }		
			.clra_report th{border-right:1px solid #000;border-bottom:1px solid #000;border-top:1px solid #000; padding: 5px; font-size:14px;}
			.note_text{padding-top: 6px; font-size:8px;}
			.clra_report td .table-sub { border-left:1px solid #333; border-top:1px solid #333; }
			.clra_report td .table-sub td { border-right:1px solid #333; border-bottom:1px solid #333; }
			</style>
				
			<table border="0" cellspacing="0" cellpadding="0" width="100%" class="clra_report">
						<tr><td colspan="11" align="right" class="border-none">Date : '.date('dS M, Y').'</td></tr>
						<tr><td colspan="8" style="font-size:10px; border:0px;">&nbsp;</td>
						<td colspan="3" class="border-none">
						
						</td></tr>
						<tr><td colspan="11" align="center" style="border:0px;"><font size="6"><strong>LABOUR COMMISSIONERATE</strong></font></td></tr>
						<tr><td align="center" colspan="11" style="border:0px;"><font size="5"><strong>GOVERNMENT OF WEST BENGAL</strong></font></td></tr>
						<tr><td align="center" colspan="11" style="border:0px;"><font size="5">MONTHLY REPORT ON IMPLEMENTATION OF WBRTPS ACT, 2013</font></td></tr>
						<tr><td align="center" colspan="11" style="font-size:12px; border:0px;"><font size="5">Labour Department</font></td></tr>
						<tr><td colspan="11" style="font-size:14px; border:0px;"></td></tr>
						<tr><td colspan="11" align="center" style="font-size:14px; border:0px;"><table border="0" cellspacing="0" cellpadding="0"><tr><td align="center" style="font-size:15px; border:1px solid #000;border-right:none">Part A:</td><td  align="center" style="border:1px solid #000;" >Performance of Designated Officers</td></tr></table></td></tr>
						<tr><td colspan="11" style="font-size:16px; border:0px;" align="center"><b><i>This report generated From '.date('dS M, Y', strtotime($form_date)).' To '. date('dS M, Y', strtotime($to_date)).'</i></b></td></tr>
						<tr><td colspan="11" style="font-size:12px; border:0px;"></td></tr>
						<tr>
						<th width="5%" class="border-left">Sl.No.<br>(1)</th>
						<th>Notified Service(s)<br>(2)</th>
						<th width="6%">No of Form-I issued<br>(3)</th>
						<th width="14%">No. of application disposed of during the month according to RTPS time frame for which Form-I were issued in the previous month<br>(4a)</th>
						<th width="14%">No of Application disposed of during the month according to RTPS time frame for which acknowledgement have been issued during the month<br>(4b)</th>
						<th width="8%">No. of application pending disposal which are RTPS time frame<br>(4c)</th>
						<th width="8%">No. of application rejected during the month<br>(4d(i))</th>
						<th width="10%">No. of application rejected during the month for which acknowledgement have been issued in previous month<br>(4d(ii))</th>
						<th width="8%">No.of Applications disposed off in favour of applicants<br>(5)</th>
						<th width="10%">% of timely disposal{(4b+4d(i)/3)}*100<br>(6)</th>
						<th width="8%">% of favour of applicants{(4b/3)}*100<br>(7)</th>
						</tr>
						<tr>
						<td class="border-left">1</td>
						<td>Registration and Amendment of Certificate of of Registration of Establishment of Principal Employer under the CLRA Act, 1970</td>
						<td>'.$point_3.'</td>
						<td>'.$exa.'</td>
						<td>'.$exb.'</td>
						<td>'.$exExpct.'</td>
						<td>'.$ex4di.'</td>
						<td>'.$ex4dii.'</td>
						<td>'.round($row_5, 2).'</td>
						<td>'.round($row_6, 2).'</td>
						<td>'.round($row_7, 2).'</td>
						</tr>
						<tr>
						<td class="border-left">2</td>
						<td>Registration and amendment of Certificate of registration of establishment under the BOCWA Act,1996</td>
						<td>'.$bocwa_3.'</td>
						<td>'.$bexa.'</td>
						<td>'.$bexb.'</td>
						<td>'.($bexExpct).'</td>
						<td>'.$bex4di.'</td>
						<td>'.$bex4dii.'</td>
						<td>'.round($brow_5, 2).'</td>
						<td>'.round($brow_6, 2).'</td>
						<td>'.round($brow_7, 2).'</td>
						</tr>
						<tr>
						<td class="border-left">3</td>
						<td>Registration and amendment,Renewal of certificate of registration of the MTW Act,1961 and Rules thereunder</td>
						<td>'.$mtw_point3.'</td>
						<td>'.$mexa.'</td>
						<td>'.$mexb.'</td>
						<td>'.$mrow_4dx.'</td>
						<td>'.$mex4di.'</td>
						<td>'.$mex4dii.'</td>
						<td>'.round($mrow_5, 2).'</td>
						<td>'.round($mrow_6, 2).'</td>
						<td>'.round($mrow_7, 2).'</td>
						</tr>
						<tr>
						<td class="border-left">4</td>
						<td>Registration and amendment of certificate of registration of establishment of principal Employer under the ISMW Act,1979</td>
						<td>'.$I_point3.'</td>
						<td>'.$iexa.'</td>
						<td>'.$iexb.'</td>
						<td>'.$ismwfourc.'</td>
						<td>'.$iex4di.'</td>
						<td>'.$iex4dii.'</td>
						<td>'.round($irow_5, 2).'</td>
						<td>'.round($irow_6, 2).'</td>
						<td>'.round($irow_7, 2).'</td>
						</tr>
						<tr>
						<td class="border-left">5</td>
						<td>Licensing of Contractors and Amendment, Renewal of Licence under the CLRA Act,1970</td>
						<td>'.$li_point3.'</td>
						<td>'.$lexa.'</td>
						<td>'.$lexb.'</td>
						<td>'.$lrow_c.'</td>
						<td>'.$lex4di.'</td>
						<td>'.$lex4dii.'</td>
						<td>'.round($lrow_5, 2).'</td>
						<td>'.round($lrow_6, 2).'</td>
						<td>'.round($lrow_7, 2).'</td>
						</tr>
					

</table><br><br><br><br><br>';
	
	$output .= "<table width='100%'><tr><td align='right'>------------------------------------------</td></tr><tr><td align='right'>(Signature of Nodal Officer)</td></tr></table>";
					
	return pdf_using_mpdf_api($output, 'Monthly-Report-From-D'.date("d-m-Y-h-m-s"));
	
}

function get_monthly_report_formaxls($form,&$form_state,$fdt,$tdt,$dist_code,$subdiv_code){
	
	
	$tdt=encryption_decryption_fun('decrypt',$tdt);
	$fdt=encryption_decryption_fun('decrypt',$fdt);
	$dist_code=encryption_decryption_fun('decrypt',$dist_code);
	$subdiv_code=encryption_decryption_fun('decrypt',$subdiv_code);
	$exa = $exb = $exc = $exExpct = 0;
	$exn = 0; 
	$ex4di = $ex4dii = $ex4diiRTPS = 0;	
	// Total No of Certificate Issue
	$query = db_select('l_remarks_master','r');
	$query->fields('r', array('application_id', 'remark_date','remark_id'));
	$query->leftJoin('l_clra_registration_master', 'clra', 'clra.id  = r.application_id');
	$query->fields('r', array('application_id', 'remark_date','remark_id'));
	$query->condition('r.remark_date', $fdt, '>=');	
	$query->condition('r.remark_date', $tdt, '<=');
	if($dist_code != "" && $subdiv_code != ""){
		$query->condition('clra.e_postal_dist', $dist_code, '<=');
		$query->condition('clra.e_postal_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query->condition('clra.e_postal_dist', $dist_code, '<=');
	}
	$query->condition('r.remark_date', $fdt, '>=');	
	$query->condition('r.remark_date', $tdt, '<=');
	$query->condition('r.remark_type', 'I', '=');
	$query->condition('r.is_active', 1, '=');
	$result = $query->execute();
	$result->rowCount();
	//  Point 3 No of FORM-I Issue
	$query3 = "select application_id,count(application_id) from l_remarks_master
				where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='S'
				group by application_id
				order by count asc
				";
	
	$result3 = db_query($query3)->fetchAll();
	$point_3=$result->rowCount();
	
	$reject = 0;
	foreach($result3 as $r3){
		$resuluRject = db_query("select application_id from l_remarks_master where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='R' and application_id = ".$r3->application_id."")->fetchObject();
		
		if(!empty($resuluRject)){
			$reject++;
		}
	}
	//  Point 4 
	$query4a = db_select('l_remarks_master', 'r');	
	$query4a ->fields('r', array('application_id', 'remark_date','remark_id'));
	$query4a->leftJoin('l_clra_registration_master', 'clra', 'clra.id  = r.application_id');
	$query4a->fields('r', array('application_id', 'remark_date','remark_id'));
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('clra.e_postal_dist', $dist_code, '<=');
		$query4a->condition('clra.e_postal_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('clra.e_postal_dist', $dist_code, '<=');
	}	
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'I', '=');
	$query4a->condition('r.is_active', 1, '=');		
	
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_remarks_master', 'r');	
			$query6->fields('r', array('remark_date','remark_id','application_id'));
			$query6->leftJoin('l_clra_registration_master', 'clra', 'clra.id  = r.application_id');
			$query6->fields('r', array('application_id', 'remark_date','remark_id'));
			$query6->condition('r.remark_date', $fdt, '>=');	
			$query6->condition('r.remark_date', $tdt, '<=');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('clra.e_postal_dist', $dist_code, '<=');
				$query6->condition('clra.e_postal_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('clra.e_postal_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('remark_id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->remark_id;
			
			$min_remark_date1 = ($data4a->remark_date - (60*60*24*30));
			
			
			if($min_remark_date1 <=$remark_date1 && $remark_date1 < $fdt){
				$exa++;
			}
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$exb++;
			}
		}
	}
	
	$query4a = db_select('l_remarks_master', 'r');	
	$query4a ->fields('r', array('application_id', 'remark_date','remark_id'));
	$query4a->fields('r', array('remark_date','remark_id','application_id'));
	$query4a->leftJoin('l_clra_registration_master', 'clra', 'clra.id  = r.application_id');
	$query4a->fields('r', array('application_id', 'remark_date','remark_id'));
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('clra.e_postal_dist', $dist_code, '<=');
		$query4a->condition('clra.e_postal_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('clra.e_postal_dist', $dist_code, '<=');
	}		
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'R', '=');
	$query4a->condition('r.is_active', 1, '=');		
	
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_remarks_master', 'r');	
			$query6->fields('r', array('remark_date','remark_id','application_id'));
			$query6->leftJoin('l_clra_registration_master', 'clra', 'clra.id  = r.application_id');
			$query6->fields('r', array('application_id', 'remark_date','remark_id'));
			$query6->condition('r.remark_date', $fdt, '>=');	
			$query6->condition('r.remark_date', $tdt, '<=');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('clra.e_postal_dist', $dist_code, '<=');
				$query6->condition('clra.e_postal_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('clra.e_postal_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('remark_id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->remark_id;
			
			$max_remark_date1 = ($remark_date1 + (60*60*24*30)); 
			
		
			if($remark_date1 <= $fdt ){
				$ex4di++;
			}

			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$ex4dii++;
			}
			if($remark_date1 <= $max_remark_date1 ){
				$ex4diiRTPS++;
			}
		}
	}
	$row_5=$exa+$exb;
	$row_4d=$point_3-$exb;
	$row_6=(($exb+$ex4di)/$point_3)*100;
	$row_7=	($exb/$point_3)*100;

	//////clara end///
	//////////bocwstart///
	$bexa = $bexb = $bexc = $bexExpct = 0;
	$bexn = 0; 
	$bex4di = $bex4dii = $bex4diiRTPS = 0;	
	// Total No of Certificate Issue
	$query = db_select('l_bocwa_remark_master','r');
	$query->fields('r', array('application_id', 'remark_date','id'));
	$query->leftJoin('l_bocwa_registration_master', 'bocwa', 'bocwa.id  = r.application_id');
	$query->condition('r.remark_date', $fdt, '>=');	
	$query->condition('r.remark_date', $tdt, '<=');
	if($dist_code != "" && $subdiv_code != ""){
		$query->condition('bocwa.loc_e_dist', $dist_code, '<=');
		$query->condition('bocwa.loc_e_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query->condition('bocwa.loc_e_dist', $dist_code, '<=');
	}	
	$query->condition('r.remark_type', 'I', '=');
	$query->condition('r.is_active', 1, '=');
	$result = $query->execute();
	$result->rowCount();
	
	//  Point 3 No of FORM-I Issue
	$query3 = "select application_id,count(application_id) from l_bocwa_remark_master
				where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='S'
				group by application_id
				order by count asc
				";
	
	$result3 = db_query($query3)->fetchAll();
	$bocwa_3=count($result3);
	$breject = 0;
	foreach($result3 as $r3){
		$resuluRject = db_query("select application_id from l_bocwa_remark_master where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='R' and application_id = ".$r3->application_id."")->fetchObject();
		
		if(!empty($resuluRject)){
			$breject++;
		}
	}
	//  Point 4 
	$query4a = db_select('l_bocwa_remark_master', 'r');	
	$query4a ->fields('r', array('application_id', 'remark_date','id'));
	$query4a->leftJoin('l_bocwa_registration_master', 'bocwa', 'bocwa.id  = r.application_id');	
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('bocwa.loc_e_dist', $dist_code, '<=');
		$query4a->condition('bocwa.loc_e_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('bocwa.loc_e_dist', $dist_code, '<=');
	}	
	$query4a->condition('r.remark_type', 'I', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_bocwa_remark_master', 'r');	
			$query6->fields('r', array('remark_date','id','application_id'));
			$query6->leftJoin('l_bocwa_registration_master', 'bocwa', 'bocwa.id  = r.application_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('bocwa.loc_e_dist', $dist_code, '<=');
				$query6->condition('bocwa.loc_e_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('bocwa.loc_e_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			$min_remark_date1 = ($data4a->remark_date - (60*60*24*30));
			
			//  Point 4a			
			if($min_remark_date1 <= $remark_date1 && $remark_date1 < $fdt){
				$bexa++;
			}
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$bexb++;
			}

		}
	}
	// Point 4d
	$query4a = db_select('l_bocwa_remark_master', 'r');	
	$query4a ->fields('r', array('application_id', 'remark_date','id'));
	$query4a->leftJoin('l_bocwa_registration_master', 'bocwa', 'bocwa.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('bocwa.loc_e_dist', $dist_code, '<=');
		$query4a->condition('bocwa.loc_e_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('bocwa.loc_e_dist', $dist_code, '<=');
	}	
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'R', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_bocwa_remark_master', 'r');	
			$query6->fields('r', array('remark_date','id','application_id'));
			$query6->leftJoin('l_bocwa_registration_master', 'bocwa', 'bocwa.id  = r.application_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('bocwa.loc_e_dist', $dist_code, '<=');
				$query6->condition('bocwa.loc_e_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('bocwa.loc_e_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			
			$max_remark_date1 = ($remark_date1 + (60*60*24*30)); 
			//  Point 4di			
			if($remark_date1 <= $fdt ){
				$bex4di++;
			}
			//  Point 4dii
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$bex4dii++;
			}
			if($remark_date1 <= $max_remark_date1 ){
				$bex4diiRTPS++;
			}
		}
	}
	
	$brow_5=$bexa+$bexb;
	$row_4d=$point_3-$exb;
	$brow_6=(($bexb+$bex4di)/$bocwa_3)*100;
	$brow_7=($bexb/$bocwa_3)*100;
	////bocwa end////
	/// MTW
	$mexa = $mexb = $mexc = $mexExpct = 0;
	$mexn = 0; 
	$mex4di = $mex4dii = $mex4diiRTPS = 0;	
	// Total No of Certificate Issue
	$query = db_select('l_mtw_remark_master','r');
	$query->fields('r', array('application_id', 'remark_date','id'));
	$query->leftJoin('l_mtw_registration_master', 'mtw', 'mtw.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query->condition('mtw.mtw_loc_dist', $dist_code, '<=');
		$query->condition('mtw.mtw_loc_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query->condition('mtw.mtw_loc_dist', $dist_code, '<=');
	}	
	$query->condition('r.remark_date', $fdt, '>=');	
	$query->condition('r.remark_date', $tdt, '<=');
	$query->condition('r.remark_type', 'I', '=');
	$query->condition('r.is_active', 1, '=');
	$result = $query->execute();
	$result->rowCount();
	//  Point 3 No of FORM-I Issue
	$query3 = "select application_id,count(application_id) from l_mtw_remark_master
				where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='S'
				group by application_id
				order by count asc
				";
	
	$result3 = db_query($query3)->fetchAll();
	$mtw_point3=count($result3);
	$mreject = 0;
	foreach($result3 as $r3){
		$resuluRject = db_query("select application_id from l_mtw_remark_master where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='R' and application_id = ".$r3->application_id."")->fetchObject();
		
		if(!empty($resuluRject)){
			$mreject++;
		}
	}
	//  Point 4 
	$query4a = db_select('l_mtw_remark_master', 'r');	
	$query4a ->fields('r', array('application_id', 'remark_date','id'));
	$query4a->leftJoin('l_mtw_registration_master', 'mtw', 'mtw.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('mtw.mtw_loc_dist', $dist_code, '<=');
		$query4a->condition('mtw.mtw_loc_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('mtw.mtw_loc_dist', $dist_code, '<=');
	}		
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'I', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_mtw_remark_master', 'r');	
			$query6->fields('r', array('remark_date','id','application_id'));
			$query6->leftJoin('l_mtw_registration_master', 'mtw', 'mtw.id  = r.application_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('mtw.mtw_loc_dist', $dist_code, '<=');
				$query6->condition('mtw.mtw_loc_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('mtw.mtw_loc_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			$min_remark_date1 = ($data4a->remark_date - (60*60*24*30));
			//  Point 4a			
			if($min_remark_date1 <= $remark_date1 && $remark_date1 < $fdt){
				$mexa++;
			}
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$mexb++;
			}
		}
	}
	// Point 4d
	$query4a = db_select('l_mtw_remark_master', 'r');	
	$query4a ->fields('r', array('application_id', 'remark_date','id'));
	$query4a->leftJoin('l_mtw_registration_master', 'mtw', 'mtw.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('mtw.mtw_loc_dist', $dist_code, '<=');
		$query4a->condition('mtw.mtw_loc_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('mtw.mtw_loc_dist', $dist_code, '<=');
	}		
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'R', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_mtw_remark_master', 'r');	
			$query6->fields('r', array('remark_date','id','application_id'));
			$query6->leftJoin('l_mtw_registration_master', 'mtw', 'mtw.id  = r.application_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('mtw.mtw_loc_dist', $dist_code, '<=');
				$query6->condition('mtw.mtw_loc_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('mtw.mtw_loc_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			$max_remark_date1 = ($remark_date1 + (60*60*24*30)); 
			//  Point 4di			
			if($remark_date1 <= $fdt ){
				$mex4di++;
			}
			//  Point 4dii
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$mex4dii++;
			}
			if($remark_date1 <= $max_remark_date1 ){
				$mex4diiRTPS++;
			}
		}
	}
	$mrow_5=$mexa+$mexb;
	$mrow_6=(($mexb+$mex4di)/$mtw_point3)*100;
	$mrow_7=($mexb/$mtw_point3)*100;
	$mrow_4dx=(count($result3)-$mexb);
	///mtw end///
	//////////ISMW/////////////
	$iexa = $iexb = $iexc = $iexExpct = 0;
	$iexn = 0; 
	$iex4di = $iex4dii = $iex4diiRTPS = 0;	
	// Total No of Certificate Issue
	$query = db_select('l_interstate_remark_master','r');
	$query->fields('r', array('application_id', 'remark_date','id'));
	$query->leftJoin('l_interstate_workman_master', 'inst', 'inst.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query->condition('inst.loc_e_dist', $dist_code, '<=');
		$query->condition('inst.loc_e_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query->condition('inst.loc_e_dist', $dist_code, '<=');
	}	
	$query->condition('r.remark_date', $fdt, '>=');	
	$query->condition('r.remark_date', $tdt, '<=');
	$query->condition('r.remark_type', 'I', '=');
	$query->condition('r.is_active', 1, '=');
	$result = $query->execute();
	$result->rowCount();
	//  Point 3 No of FORM-I Issue
	$query3 = "select application_id,count(application_id) from l_interstate_remark_master
				where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='S'
				group by application_id
				order by count asc
				";
	
	$result3 = db_query($query3)->fetchAll();
	$I_point3=count($result3);
	$ireject = 0;
	foreach($result3 as $r3){
		$resuluRject = db_query("select application_id from l_interstate_remark_master where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='R' and application_id = ".$r3->application_id."")->fetchObject();
		
		if(!empty($resuluRject)){
			$ireject++;
		}
	}
	$ireject;
	//  Point 4 
	$query4a = db_select('l_interstate_remark_master', 'r');	
	$query4a ->fields('r', array('application_id', 'remark_date','id'));
	$query4a->leftJoin('l_interstate_workman_master', 'inst', 'inst.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('inst.loc_e_dist', $dist_code, '<=');
		$query4a->condition('inst.loc_e_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('inst.loc_e_dist', $dist_code, '<=');
	}		
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'I', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_interstate_remark_master', 'r');	
			$query6->fields('r', array('remark_date','id','application_id'));
			$query6->leftJoin('l_interstate_workman_master', 'inst', 'inst.id  = r.application_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('inst.loc_e_dist', $dist_code, '<=');
				$query6->condition('inst.loc_e_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('inst.loc_e_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			$min_remark_date1 = ($data4a->remark_date - (60*60*24*30));
			//  Point 4a			
			if($min_remark_date1 <= $remark_date1 && $remark_date1 < $fdt){
				$iexa++;
			}
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$iexb++;
			}
		}
	}
	// Point 4d
	$query4a = db_select('l_interstate_remark_master', 'r');	
	$query4a ->fields('r', array('application_id', 'remark_date','id'));
	$query4a->leftJoin('l_interstate_workman_master', 'inst', 'inst.id  = r.application_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('inst.loc_e_dist', $dist_code, '<=');
		$query4a->condition('inst.loc_e_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('inst.loc_e_dist', $dist_code, '<=');
	}		
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'R', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_interstate_remark_master', 'r');	
			$query6->fields('r', array('remark_date','id','application_id'));
			$query6->leftJoin('l_interstate_workman_master', 'inst', 'inst.id  = r.application_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('inst.loc_e_dist', $dist_code, '<=');
				$query6->condition('inst.loc_e_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('inst.loc_e_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'S', '=');	
			$query6->condition('r.application_id', $data4a->application_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			$max_remark_date1 = ($remark_date1 + (60*60*24*30)); 
			//  Point 4di			
			if($remark_date1 <= $fdt ){
				$iex4di++;
			}
			//  Point 4dii
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$iex4dii++;
			}
			if($remark_date1 <= $max_remark_date1 ){
				$iex4diiRTPS++;
			}
		}
	}
	$irow_5=$iexa+$iexb;
	$row_4d=$point_3-$exb;
	$irow_6=(($iexb+$iex4di)/$I_point3)*100;
	$irow_7=($iexb/$I_point3)*100;
	$ismwfourc=count($result3)-$iexb;
	/////////ismw end///////
	$lexa = $lexb = $lexc = $lexExpct = 0;
	$lexn = 0; 
	$lex4di = $lex4dii = $lex4diiRTPS = 0;	
	// Total No of Certificate Issue
	$query = db_select('l_remark_license','r');
	$query->fields('r', array('particular_id', 'remark_date','id'));
	$query->leftJoin('l_particular_of_contract_labour', 'lince', 'lince.id  = r.particular_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query->condition('lince.worksite_dist', $dist_code, '<=');
		$query->condition('lince.worksite_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query->condition('lince.worksite_dist', $dist_code, '<=');
	}
	$query->condition('r.remark_date', $fdt, '>=');	
	$query->condition('r.remark_date', $tdt, '<=');
	$query->condition('r.remark_type', 'I', '=');
	$query->condition('r.is_active', 1, '=');
	$result = $query->execute();
	$result->rowCount();
	//  Point 3 No of FORM-I Issue
	$query3 = "select particular_id,count(particular_id) from l_remark_license
				where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='U'
				group by particular_id
				order by count asc
				";
	$result3 = db_query($query3)->fetchAll();
	$li_point3=count($result3);
	$lreject = 0;
	foreach($result3 as $r3){
		$resuluRject = db_query("select particular_id from l_remark_license where remark_date between $fdt and $tdt
				and is_active = 1 and remark_type='R' and particular_id = ".$r3->particular_id."")->fetchObject();
		
		if(!empty($resuluRject)){
			$lreject++;
		}
	}
	$lreject;
	//  Point 4 
	$query4a = db_select('l_remark_license', 'r');	
	$query4a ->fields('r', array('particular_id', 'remark_date','id'));
	$query4a->leftJoin('l_particular_of_contract_labour', 'lince', 'lince.id  = r.particular_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('lince.worksite_dist', $dist_code, '<=');
		$query4a->condition('lince.worksite_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('lince.worksite_dist', $dist_code, '<=');
	}
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'I', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_remark_license', 'r');	
			$query6->fields('r', array('remark_date','id','particular_id'));
			$query6->leftJoin('l_particular_of_contract_labour', 'lince', 'lince.id  = r.particular_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('lince.worksite_dist', $dist_code, '<=');
				$query6->condition('lince.worksite_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('lince.worksite_dist', $dist_code, '<=');
			}	
			$query6->condition('r.remark_type', 'U', '=');	
			$query6->condition('r.particular_id', $data4a->particular_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			$min_remark_date1 = ($data4a->remark_date - (60*60*24*30));
			
			//  Point 4a			
			if($min_remark_date1 <= $remark_date1 && $remark_date1 < $fdt){
				$lexa++;
			}
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$lexb++;
			}
		}
	}
	// Point 4d
	$query4a = db_select('l_remark_license', 'r');	
	$query4a ->fields('r', array('particular_id', 'remark_date','id'));
	$query4a->leftJoin('l_particular_of_contract_labour', 'lince', 'lince.id  = r.particular_id');
	if($dist_code != "" && $subdiv_code != ""){
		$query4a->condition('lince.worksite_dist', $dist_code, '<=');
		$query4a->condition('lince.worksite_subdivision', $subdiv_code, '<=');
	}elseif(($dist_code != "" && $subdiv_code == "")){
		$query4a->condition('lince.worksite_dist', $dist_code, '<=');
	}	
	$query4a->condition('r.remark_date', $fdt, '>=');	
	$query4a->condition('r.remark_date', $tdt, '<=');
	$query4a->condition('r.remark_type', 'R', '=');
	$query4a->condition('r.is_active', 1, '=');		
	$result4a = $query4a->execute();
	
	if($result4a->rowCount() > 0){
		
		foreach($result4a as $data4a){ 
			$remark_date1 = $remark_date2 = '';
			
			$query6 = db_select('l_remark_license', 'r');	
			$query6->fields('r', array('remark_date','id','particular_id'));
			$query6->leftJoin('l_particular_of_contract_labour', 'lince', 'lince.id  = r.particular_id');
			if($dist_code != "" && $subdiv_code != ""){
				$query6->condition('lince.worksite_dist', $dist_code, '<=');
				$query6->condition('lince.worksite_subdivision', $subdiv_code, '<=');
			}elseif(($dist_code != "" && $subdiv_code == "")){
				$query6->condition('lince.worksite_dist', $dist_code, '<=');
			}
			$query6->condition('r.remark_type', 'U', '=');	
			$query6->condition('r.particular_id', $data4a->particular_id, '=');
			$query6->condition('r.is_active', 1); 
			$query6->orderby('id','asc');
			$query6->range(0,1);	
			$remark_date1 = $query6->execute()->fetchObject()->remark_date;
			$id6 = $query6->execute()->fetchObject()->id;
			$max_remark_date1 = ($remark_date1 + (60*60*24*30)); 
			//  Point 4di			
			if($remark_date1 <= $fdt ){
				$lex4di++;
			}
			//  Point 4dii
			if(($remark_date1 >= $fdt ) && ($remark_date1 <= $tdt)){
				$lex4dii++;
			}
			if($remark_date1 <= $max_remark_date1 ){
				$lex4diiRTPS++;
			}
		}
	}
	
	$lrow_5=$lexa+$lexb;
	$row_4d=$point_3-$exb;
	$lrow_6=(($lexb+$lex4di)/$li_point3)*100;
	$lrow_7=($lexb/$li_point3)*100;
	$lrow_c=(count($result3)-$lexb);
	// Excel class file include
	$file = drupal_get_path('module', 'genfunction') . "/PHPExcel/Classes/PHPExcel.php";
	$file2 = drupal_get_path('module', 'genfunction') . "/PHPExcel/Classes/PHPExcel/IOFactory.php";
	require_once $file;
	require_once $file2;
	$objPHPExcel = new PHPExcel();
	$objPHPExcel->getProperties()->setCreator("Maarten Balliauw")
			->setLastModifiedBy("Maarten Balliauw")
			->setTitle("Office 2007 XLSX Test Document")
			->setSubject("Office 2007 XLSX Test Document")
			->setDescription("Test document for Office 2007 XLSX, generated using PHP classes.")
			->setKeywords("office 2007 openxml php")
			->setCategory("Test result file");
	$sheet = $objPHPExcel->getActiveSheet();
	// Header Section for excel	
	$default_border = array(
		'style' => PHPExcel_Style_Border::BORDER_THIN,
		'color' => array('rgb'=>'1006A3')
	);
	$style_header = array(
		'borders' => array(
			'bottom' => $default_border,
			'left' => $default_border,
			'top' => $default_border,
			'right' => $default_border,
		),
		'fill' => array(
			'type' => PHPExcel_Style_Fill::FILL_SOLID,
			'color' => array('rgb'=>'4BA0F4'),
		),
		'font' => array(
						'bold' => true,
					)
	);
	
	$style = array(
        'alignment' => array(
            'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
			'bold' => true,
			'style' => PHPExcel_Style_Border::BORDER_MEDIUM,
			'color' => array('rgb'=>'1006A3')
        )
    );
	$starting_pos = ord('A');
	$sheet->mergeCells('A1:K1');	
	$sheet->setCellValue('A1', 'LABOUR COMMISSIONERATE ');
	$sheet->getStyle("A1:K1")->applyFromArray($style);
	$starting_pos = ord('A');
	$sheet->mergeCells('A2:K2');	
	$sheet->setCellValue('A2', 'GOVERNMENT OF WEST BENGAL');
	$sheet->getStyle("A2:K2")->applyFromArray($style);
	$starting_pos = ord('A');
	$sheet->mergeCells('A3:K3');	
	$sheet->setCellValue('A3', 'MONTHLY REPORT ON IMPLEMENTATION OF WBRTPS ACT, 2013');
	$sheet->getStyle("A3:K3")->applyFromArray($style);
	$starting_pos = ord('A');
	$sheet->mergeCells('A4:K4');	
	$sheet->setCellValue('A4', 'Labour Department');
	$sheet->getStyle("A4:K4")->applyFromArray($style);
	$starting_pos = ord('A');
	$sheet->mergeCells('A5:K5');	
	$sheet->setCellValue('A5', 'Part A : Performance of Designated Officers');
	$sheet->getStyle("A5:K5")->applyFromArray($style);
	$sheet->getStyle('A6')->applyFromArray($style);
	$sheet->getStyle('B')->applyFromArray($style);
	$sheet->getStyle('C6')->applyFromArray($style);
	$sheet->getStyle('D6')->applyFromArray($style);
	$sheet->getStyle('E6')->applyFromArray($style);
	$sheet->getStyle('F6')->applyFromArray($style);
	$sheet->getStyle('G6')->applyFromArray($style);
	$sheet->getStyle('H6')->applyFromArray($style);
	$sheet->getStyle('I6')->applyFromArray($style);
	$sheet->getStyle('J6')->applyFromArray($style);
	$sheet->getStyle('K6')->applyFromArray($style);
	$sheet->setCellValue('A'.'6','SL No');
	$sheet->setCellValue('B'.'6','Notified Service(s)');
	$sheet->setCellValue('C'.'6','No of Form-I issued');
	$sheet->setCellValue('D'.'6','No. of application disposed of during the month according to RTPS time frame for which Form-I were issued in the previous month');
	$sheet->setCellValue('E'.'6','No of Application disposed of during the month according to RTPS time frame for which acknowledgement have been issued during the month');
	$sheet->setCellValue('F'.'6','No. of application pending disposal which are RTPS time frame');
	$sheet->setCellValue('G'.'6','No. of application rejected during the month');
	$sheet->setCellValue('H'.'6','No. of application rejected during the month for which acknowledgement have been issued in previous month');
	$sheet->setCellValue('I'.'6','No.of Applications disposed off in favour of applicants)');
	$sheet->setCellValue('J'.'6','% of timely disposal{(4b+4d(i)/3)}*100');
	$sheet->setCellValue('K'.'6','% of favour of applicants{(4b/3)}*100');
	///////////CLARA START///
	$sheet->setCellValue('A'.'7', 1);
	$sheet->setCellValue('B'.'7','Registration and Amendment of Certificate of of Registration of Establishment of Principal Employer under the CLRA Act, 1970');
	$sheet->setCellValue('C'.'7',$point_3);
	$sheet->setCellValue('D'.'7',$exa);
	$sheet->setCellValue('E'.'7',$exb);
	$sheet->setCellValue('F'.'7',$exExpct);
	$sheet->setCellValue('G'.'7',$ex4di);
	$sheet->setCellValue('H'.'7',$ex4dii);
	$sheet->setCellValue('I'.'7',round($row_5, 2));
	$sheet->setCellValue('J'.'7',round($row_6, 2));
	$sheet->setCellValue('K'.'7',round($row_7, 2));
	$sheet->setCellValue('A'.'8', 2);
	$sheet->setCellValue('B'.'8','Registration and amendment of Certificate of registration of establishment under the BOCWA Act,1996');
	$sheet->setCellValue('C'.'8',$bocwa_3);
	$sheet->setCellValue('D'.'8',$bexa);
	$sheet->setCellValue('E'.'8',$bexb);
	$sheet->setCellValue('F'.'8',($bexExpct));
	$sheet->setCellValue('G'.'8',$bex4di);
	$sheet->setCellValue('H'.'8',$bex4dii);
	$sheet->setCellValue('I'.'8',round($brow_5, 2));
	$sheet->setCellValue('J'.'8',round($brow_6, 2));
	$sheet->setCellValue('K'.'8',round($brow_7, 2));
	$sheet->setCellValue('A'.'9', 3);
	$sheet->setCellValue('B'.'9','Registration and amendment,Renewal of certificate of registration of the MTW Act,1961 and Rules thereunder');
	$sheet->setCellValue('C'.'9',$mtw_point3);
	$sheet->setCellValue('D'.'9',$mexa);
	$sheet->setCellValue('E'.'9',$mexb);
	$sheet->setCellValue('F'.'9',$mrow_4dx);
	$sheet->setCellValue('G'.'9',$mex4di);
	$sheet->setCellValue('H'.'9',$mex4dii);
	$sheet->setCellValue('I'.'9',round($mrow_5, 2));
	$sheet->setCellValue('J'.'9',round($mrow_6, 2));
	$sheet->setCellValue('K'.'9',round($mrow_7, 2));
	$sheet->setCellValue('A'.'10', 4);
	$sheet->setCellValue('B'.'10','Registration and amendment of certificate of registration of establishment of principal Employer under the ISMW Act,1979');
	$sheet->setCellValue('C'.'10',$I_point3);
	$sheet->setCellValue('D'.'10',$iexa);
	$sheet->setCellValue('E'.'10',$iexb);
	$sheet->setCellValue('F'.'10',$ismwfourc);
	$sheet->setCellValue('G'.'10',$iex4di);
	$sheet->setCellValue('H'.'10',$iex4dii);
	$sheet->setCellValue('I'.'10',round($irow_5, 2));
	$sheet->setCellValue('J'.'10',round($irow_6, 2));
	$sheet->setCellValue('K'.'10',round($irow_7, 2));
	$sheet->setCellValue('A'.'11', 5);
	$sheet->setCellValue('B'.'11','Licensing of Contractors and Amendment, Renewal of Licence under the CLRA Act,1970');
	$sheet->setCellValue('C'.'11',$li_point3);
	$sheet->setCellValue('D'.'11',$lexa);
	$sheet->setCellValue('E'.'11',$lexb);
	$sheet->setCellValue('F'.'11',$lrow_c);
	$sheet->setCellValue('G'.'11',$lex4di);
	$sheet->setCellValue('H'.'11',$lex4dii);
	$sheet->setCellValue('I'.'11',round($lrow_5, 2));
	$sheet->setCellValue('J'.'11',round($lrow_6, 2));
	$sheet->setCellValue('K'.'11',round($lrow_7, 2));
	$style_wrap = array(
		'alignment' => array(
			'horizontal' => PHPExcel_Style_Alignment::VERTICAL_TOP,
			'wrap' => true
			
		)
	);
	$sheet->getStyle('A6:K6')->applyFromArray($style_wrap);
	$sheet->getStyle('A2')->getAlignment()
			->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER)
			->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);
			
	$sheet->getStyle('A3')->getAlignment()
			->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER)
			->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);
	
	$sheet->getStyle('A4')->getAlignment()
			->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER)
			->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);
	
	$sheet->getStyle('A5')->getAlignment()
			->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER)
			->setVertical(PHPExcel_Style_Alignment::VERTICAL_CENTER);
	$objPHPExcel->getActiveSheet()->getColumnDimension('B')->setWidth(30);
	$objPHPExcel->getActiveSheet()->getColumnDimension('C')->setWidth(18);
	$objPHPExcel->getActiveSheet()->getColumnDimension('D')->setWidth(18);
	$objPHPExcel->getActiveSheet()->getColumnDimension('E')->setWidth(18);
	$objPHPExcel->getActiveSheet()->getColumnDimension('F')->setWidth(18);
	$objPHPExcel->getActiveSheet()->getColumnDimension('G')->setWidth(18);
	$objPHPExcel->getActiveSheet()->getColumnDimension('H')->setWidth(18);
	$objPHPExcel->getActiveSheet()->getColumnDimension('I')->setWidth(18);
	$objPHPExcel->getActiveSheet()->getColumnDimension('J')->setWidth(18);
	$objPHPExcel->getActiveSheet()->getColumnDimension('K')->setWidth(18);
	$objPHPExcel->getActiveSheet()->getProtection()->setSheet(true);
	$objPHPExcel->getActiveSheet()->getProtection()->setSort(true);
	$objPHPExcel->getActiveSheet()->getProtection()->setInsertRows(true);
	$objPHPExcel->getActiveSheet()->getProtection()->setFormatCells(true);
	$objPHPExcel->getActiveSheet()
                ->getStyle('A1:K11')
                ->getBorders()
                ->getTop()
                ->setBorderStyle(PHPExcel_Style_Border::BORDER_THIN );
	$objPHPExcel->getActiveSheet()
                ->getStyle('A1:K11')
                -> getBorders()
                ->getLeft()
                ->setBorderStyle(PHPExcel_Style_Border::BORDER_THIN );

    $objPHPExcel->getActiveSheet()
                ->getStyle('A1:K11')
                ->getBorders()
                ->getRight()
                ->setBorderStyle(PHPExcel_Style_Border::BORDER_THIN );

    $objPHPExcel->getActiveSheet()
                ->getStyle('A1:K11')
                ->getBorders()
                ->getBottom()
                ->setBorderStyle(PHPExcel_Style_Border::BORDER_THIN );
				
	$from = "A1"; // or any value
	$to = "K1"; // or any value
	$objPHPExcel->getActiveSheet()->getStyle("$from:$to")->getFont()->setBold( true );
	
	$from = "A4"; // or any value
	$to = "K4"; // or any value
	$objPHPExcel->getActiveSheet()->getStyle("$from:$to")->getFont()->setBold( true );
	
	$from = "A6"; // or any value
	$to = "K6"; // or any value
	$objPHPExcel->getActiveSheet()->getStyle("$from:$to")->getFont()->setBold( true );
	$objPHPExcel->setActiveSheetIndex(0);
	$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5');	
	ob_end_clean();
	// Redirect output to a client’s web browser (Excel5)
	header('Content-Type: application/vnd.ms-excel;charset:UTF-8');
	$filename = 'monthly-report'.date("d-m-Y").'.xls';
	header('Content-Disposition: attachment;filename='.basename(strtolower($filename)));
	header('Cache-Control: max-age=0');
	// If you're serving to IE 9, then the following may be needed
	header('Cache-Control: max-age=1');	
	$objWriter->save('php://output');
	
	
}

function get_report_set_year_range($form_element, $form_values) {
    $form_element['year']['#options'] = drupal_map_assoc(range(2014, date("Y")));
    return $form_element;
}