<?php

drupal_add_css(drupal_get_path('module', 'alcrandomization') . '/css/custom_css.css');

function alcrandomization_list($form, &$form_state){
	global $base_root, $base_path, $user;
	$uid					=	$user->uid;
	$query_alc_type			=	db_query('select area_division_type from l_custom_user_detail where usr_id =:uid', array(':uid' => trim($uid) ));
	$result					=	$query_alc_type->fetchAssoc();
	$area_division_type 	= 	$result['area_division_type'];
		
	$outputTable							=	outputTable($area_division_type);
	
	$form['alc_randomization']['markup']	=	array(
													'#prefix' 	=> '',
													'#suffix' 	=> '',
													'#type'   	=> 'markup',
													'#markup'	=>	$outputTable
												);
	
	return $form;
}

function outputTable($area_division_type=''){
	global $base_root, $base_path, $user;
	$uid							=	$user->uid;
	$count							=	1;
	$output 						= 	'';
	$header 						= 	array(
											array('data' => 'Sl. No', 'field' => 'slno', 'width' => '7%'),
											array('data' => 'Area Name', 'field' => '', 'width' => '20%'),
											array('data' => 'Inspector Name', 'field' => 'e_trade_union_name', 'width' => ''),
											array('data' => 'From Date', 'field' => 'e_trade_union_address', 'width' => '15%'),
											array('data' => 'To date', 'field' => '', 'width' => '15%'),
											array('data' => 'GP/WARD NAME', 'field' => '', 'width' => '20%'),
											array('data' => 'GP WISE RANDOMIZATION', 'field' => '', 'width' => '10%')
										);
	
	$new_queryz						=	db_query('SELECT y.master_tag_id AS master_tag_id FROM l_area_inspector_random_tag y INNER JOIN l_custom_user_detail x ON x.usr_id = y.inspector_id 
WHERE y.is_confirm =:is_confirm AND x.created_by =:created_by', array(':is_confirm' => 'C', ':created_by' => trim($uid)));
	$results_1						=	$new_queryz->fetchAssoc();
	$masters_tag_id 				= 	$results_1['master_tag_id'];
	
	//echo $masters_tag_id;									
	$order							=	'inspector_name';
	$sort							=	'ASC';
	$show_details_query 			= 	db_select('l_area_inspector_random_tag', 't');
	$show_details_query				->	join('l_custom_user_detail', 'v', 'v.usr_id =  t.inspector_id');
	$show_details_query				->	fields('t', array('inspector_name','inspector_code','distict_code','block_municipality_code','inspector_id', 'id', 'is_confirm', 'master_tag_id'));
	$show_details_query				->	fields('v', array('fullname'));
	$show_details_query				->	condition('t.is_confirm','C','=');
	$show_details_query				->	condition('v.created_by', trim($uid), '=');
	if($masters_tag_id != ""){
		$show_details_query				->	condition('t.master_tag_id', trim($masters_tag_id), '=');
	}
	$show_details_query				->	orderBy($order, $sort);
	$show_details_query 			= 	$show_details_query->extend('PagerDefault')->limit(50);
	$total_name 					=  	$show_details_query->execute();
	
	// $row_arr 						= 	$total_name->fetchAssoc(); print_r($row_arr);
	$numrow 						= 	$total_name->rowCount();
	
	
	$sub_div_query					=	db_query('select sub_div_code from l_customuser_relation_address where user_id=:uid', array(':uid' => trim($uid) ));
	$sub_div_res					=	$sub_div_query->fetchAssoc();
	$sub_division_code				=	$sub_div_res['sub_div_code'];
	
	$query_1						=	db_query('select * from l_area_inspector_random_order_master where officer_designation=2 and upload_status=:upload_status order by distict_code, subdivision_code ASC', array(':upload_status' => 'Z'));
	$query_1_res					=	$query_1->fetchAssoc();
	$signed_order_doc				=	$query_1_res['signed_order_doc'];
	$upload_status					=	$query_1_res['upload_status'];
	$order_uploaded_on				=	$query_1_res['order_uploaded_on'];

	if( $total_name->rowCount() > 0 ){
		while($data = $total_name->fetchObject()) {
			//print_r($data->inspector_name);
			$counter					=	$count++;
			//$dist_code 					= 	$row_arr['distict_code'];
			$inspector_code 			= 	$data->inspector_id; 
			$inspector_name 			= 	$data->inspector_name;
			$area_code 					= 	$data->block_municipality_code;
			$district_code 				= 	$data->distict_code;
			$id							=	$data->id;
			$master_tag_id				=	$data->master_tag_id;		
						
			$nq1						=	db_query('select * from l_area_inspector_random_order_master where tag_map_id=:tag_map_id and officer_designation=:officer_designation', array(':tag_map_id' => $masters_tag_id, ':officer_designation' => 2));
			$r_1						=	$nq1->fetchAssoc();
			$tag_map_id					=	$r_1['id'];
			$upload_status 				= 	$r_1['upload_status'];			
			
			$query_ar_name				=	db_query('select block_mun_name from block_mun_master where block_code =:wcode and distict_code =:dcode  ', array(':wcode' => trim($area_code) , ':dcode' => trim($district_code)));
	
			$result						=	$query_ar_name->fetchAssoc();
			$area_name 					= 	$result['block_mun_name'];
			
			$query_vill_name			=	db_query('select scheduling_status, village_name, scheduling_fdt, scheduling_tdt, category from l_gp_ward_inspector_tag a, village_ward_master b where a.block_code=b.block_code and a.village_code=b.village_code and inspector_id = :insid and confirm_s = :cst and masters_tag_id=:masters_tag_id', array(':insid' => trim($inspector_code) , ':cst' => 'C', ':masters_tag_id' => $masters_tag_id)); 
			$result2					=	$query_vill_name->fetchAssoc();
			
			$scheduling_status 			= 	$result2['scheduling_status']; 
			$scheduling_fdt				=	$result2['scheduling_fdt'];
			$scheduling_tdt				=	$result2['scheduling_tdt'];
			$village_name1				=	$result2['village_name'];
			
			$q1							=	db_query('select * from l_gp_ward_inspector_tag where masters_tag_id=:masters_tag_id', array(':masters_tag_id' => $masters_tag_id));
			$q1_res						=	$q1->fetchAssoc();
			if($q1->rowCount() > 0 && $upload_status == "" ){
				if($scheduling_fdt != "" && $scheduling_status!=""){
					$order_link_alc		=	$base_root.$base_path.'generate-alc-randomization-order/default/'.encryption_decryption_fun('encrypt', $tag_map_id).'/'.encryption_decryption_fun('encrypt', $district_code).'/'.encryption_decryption_fun('encrypt', $sub_division_code);
					$order_link			=	l('<div class="box-header with-border pull-right"><button type="button" class="btn btn-default"><i class="fa fa-edit"></i> Generate Order</button></div>', $order_link_alc, array('html' => TRUE, 'attributes'=> array('target' => '_blank', 'id' => '')));
					
					
					
					$start_date				=	date('d-m-Y', strtotime($scheduling_fdt));
					$end_date				=	date('d-m-Y', strtotime($scheduling_tdt));
					if($result2['category'] == "W"){
						$village_name1 			= 	"Ward-".$village_name1; 
					}
					
					$link_creation			=	t('<center><font style="font: bold 11px Arial; text-decoration: none; background-color: #EEEEEE; color: green; padding: 2px 6px 2px 6px; border-top: 1px solid #CCCCCC; border-right: 1px solid #333333; border-bottom: 1px solid #333333; border-left: 1px solid #CCCCCC;"><b>Submitted</b></font></center>');
				}else{
					$start_date				=	'Not Scheduled';
					$end_date				=	'Not Scheduled';
					$link					=	$base_root.$base_path.'alcrandomization/'.$district_code.'/'.$area_code.'/'.$inspector_code.'/'.$id.'/'.$masters_tag_id.'/'.'gprandom';
					$link_creation			=	l(t('<center><font style="font: bold 11px Arial; text-decoration: none; background-color: #EEEEEE; color: red; padding: 2px 6px 2px 6px; border-top: 1px solid #CCCCCC; border-right: 1px solid #333333; border-bottom: 1px solid #333333; border-left: 1px solid #CCCCCC;"><b>Randomize</b></font></center>'), $link, array('html'=>TRUE));
				}
			}else{
				$start_date				=	'Not Scheduled';
				$end_date				=	'Not Scheduled';
				$link					=	$base_root.$base_path.'alcrandomization/'.$district_code.'/'.$area_code.'/'.$inspector_code.'/'.$id.'/'.$masters_tag_id.'/'.'gprandom';
				$link_creation			=	l(t('<center><font style="font: bold 11px Arial; text-decoration: none; background-color: #EEEEEE; color: red; padding: 2px 6px 2px 6px; border-top: 1px solid #CCCCCC; border-right: 1px solid #333333; border-bottom: 1px solid #333333; border-left: 1px solid #CCCCCC;"><b>Randomize</b></font></center>'), $link, array('html'=>TRUE));
			}
			
			if($upload_status==""){
				$rows[] 					= 	array(
														$counter,
														$area_name,
														ucfirst($inspector_name),
														$start_date,
														$end_date,
														ucfirst($village_name1),
														$link_creation
												);
			}										
			
		}
	}
	
	$variables 	= 	array( 'header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')), 'empty' => t("No data found!"));
						
	$output = 	theme('datatable', $variables);
	
		
	return '<div class="box box-primary"><div class="box-body">'.$order_link.$output.'</div></div>';
}