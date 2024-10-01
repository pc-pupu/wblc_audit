<?php

drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');

function superadmin_advance_search($form, &$form_state){
	
	global $base_root, $base_path, $user;
	$user_id												=	$user->uid;
	$form['superadmin_advance_search']['header']			= 	array(
																	'#markup' 	=>	 t('<div class="content"><div class="sky-form" style="width:100%;">')
																);
			
	$options_services										=	array(0 => "Select Service", '1' => 'Contract Labour (Regulation And Abolition) Act, 1970');
	
	$form['superadmin_advance_search']['services'] 			= 	array(
																	'#prefix'			=>	'<fieldset><legend> Advanced Search </legend><section><label class="select">',
																	'#suffix' 			=> 	'</label></section>',
																	'#title'			=> 	'Services',
																	'#type' 			=> 	'select',
																	'#options' 			=>	$options_services,
																	'#required'			=>	TRUE,
																	'#default_value'	=>	1,
																	'#disabled'			=>	TRUE
																);
												
	$value_services 										=	isset($form_state['values']['services']) ? $form_state['values']['services'] : '';
	
	$options_dist 											= 	get_super_admin_all_districts();
	
	$form['superadmin_advance_search']['districts']  		= 	array(
																	'#prefix' 				=> 	'<section><label class="select">',
																	'#suffix' 				=> 	'</label></section></fieldset>',
																	'#title'				=> 	'Select District',
																	'#type' 				=> 	'select',
																	'#options' 				=> 	$options_dist,
																	'#required' 			=> 	TRUE
																);
	
	$form['superadmin_advance_search']['submit'] 			= 	array (
																  '#prefix' 			=> '<footer>',  
																  '#suffix' 			=> '</footer></div></div>', 
																  '#type' 				=> 'submit',
																  '#attributes' 		=> array('class' => array('button')),
																  '#value' 				=> 'Search'
																);
	
	
	return $form;
}

function superadmin_advance_search_validate($form, &$form_state){
	
}

function superadmin_advance_search_submit($form, &$form_state){
	$val 			=	$form_state['values'];
	$services		=	$val['services'];
	$districts		=	$val['districts'];
	if($services && $districts != ""){
		drupal_goto('super-admin-advanced-search/'.$services.'/'.$districts);
	}
}

function advce_search_showlist($form, &$form_state, $services='', $district=''){
	global $base_root, $base_path, $user;

	$user_id												=	$user->uid;
	$form['superadmin_advance_search']['header']			= 	array(
																	'#markup' 	=>	t('<div class="content"><div class="sky-form" style="width:100%;"><header><h3><b></b></h3></header>')
																);
			
	$options_services										=	array(0 => "Select Service", '1' => 'Contract Labour (Regulation And Abolition) Act, 1970');
	
	$form['superadmin_advance_search']['services'] 			= 	array(
																	'#prefix'			=>	'<fieldset><legend> Advanced Search </legend><section><label class="select">',
																	'#suffix' 			=> 	'</label></section>',
																	'#title'			=> 	'Services',
																	'#type' 			=> 	'select',
																	'#options' 			=>	$options_services,
																	'#default_value'	=>	!empty($services) ? $services : '',
																	'#required'			=>	TRUE,
																	'#default_value'	=>	1,
																	'#disabled'			=>	TRUE
																);
																																				
	$options_dist 											= 	get_super_admin_all_districts();
	
	$form['superadmin_advance_search']['districts']  		= 	array(
																	'#prefix' 				=> 	'<section><label class="select">',
																	'#suffix' 				=> 	'</label></section></fieldset>',
																	'#title'				=> 	'Select District',
																	'#type' 				=> 	'select',
																	'#options' 				=> 	$options_dist,
																	'#default_value'		=>	!empty($district) ? $district : '',
																	'#required' 			=> 	TRUE
																);
	
	$form['superadmin_advance_search']['submit'] 			= 	array (
																  '#prefix' 			=> '<footer>',  
																  '#suffix' 			=> '', 
																  '#type' 				=> 'submit',
																  '#attributes' 		=> array('class' => array('button')),
																  '#value' 				=> 'Search'
																);
	
	$form['superadmin_advance_search']['markup_link'] 		= 	array (
																  '#prefix' 			=> '<div class="row">',  
																  '#suffix' 			=> '</div></footer></div></div>', 
																  '#type' 				=> 'markup',
																  '#markup' 			=> get_subdivision_blocks($services, $district)
																);													
	return $form;
}

function advce_search_showlist_validate($form, &$form_state){
	
}

function advce_search_showlist_submit($form, $form_state){
	$val 			=	$form_state['values'];
	$services		=	$val['services'];
	$districts		=	$val['districts'];
	if($services && $districts != ""){
		drupal_goto('super-admin-advanced-search/'.$services.'/'.$districts);
	}
}

function get_subdivision_blocks($value_services='', $district_code=''){
	//echo "hii"; exit;
	global $base_root, $base_path, $user;
	$output  		= 	'';
	$db_query_1 	= 	db_query("select mw.sub_div_code, mw.sub_div_name , pentotal ,vpaytotal , vnotptotal , issuetotal , backtotal , rejtotal , inctotal from ( SELECT district_master.district_code, sub_div_code, sub_div_name FROM district_master , sub_division where district_master.district_code=sub_division.district_code and district_master.district_code='".$district_code."' GROUP BY district_master.district_code , sub_div_code, sub_div_name ORDER BY sub_div_name )mw LEFT JOIN ( select sum(inctotal) inctotal , sub_div_code  from (select count(*) inctotal, sub_div_code from l_clra_registration_master a, district_master d, sub_division c where   a.loc_e_dist=d.district_code  and d.district_code=c.district_code and a.loc_e_subdivision=c.sub_div_code and d.district_code='".$district_code."' and final_submit_status is null and status is null group by  sub_div_code  ) v  group by sub_div_code  ) AS insinc ON mw.sub_div_code = insinc.sub_div_code LEFT JOIN ( select sum(pentotal) pentotal , sub_div_code from (select count(*) pentotal, sub_div_code from l_clra_registration_master a, district_master d, sub_division c where   a.loc_e_dist=d.district_code  and d.district_code=c.district_code and   a.loc_e_subdivision=c.sub_div_code and d.district_code='".$district_code."' and final_submit_status='P' and status = '0' group by  sub_div_code  ) v  group by sub_div_code  ) AS inspen ON mw.sub_div_code = inspen.sub_div_code LEFT JOIN (select sum(vpaytotal) vpaytotal, sub_div_code from (select count(*) vpaytotal, sub_div_code from l_clra_registration_master a, district_master d, sub_division c where   a.loc_e_dist=d.district_code  and d.district_code=c.district_code and   a.loc_e_subdivision=c.sub_div_code and d.district_code='".$district_code."' and final_submit_status='P' and status = 'T' group by sub_div_code ) v  group by sub_div_code  ) AS insvt ON mw.sub_div_code = insvt.sub_div_code LEFT JOIN (select sum(vnotptotal) vnotptotal, sub_div_code from (select count(*) vnotptotal, sub_div_code from l_clra_registration_master a, district_master d, sub_division c where a.loc_e_dist=d.district_code and d.district_code=c.district_code and a.loc_e_subdivision=c.sub_div_code and d.district_code='".$district_code."' and final_submit_status='P' and status = 'V' group by sub_div_code ) v  group by sub_div_code  ) AS insvnotp ON mw.sub_div_code = insvnotp.sub_div_code LEFT JOIN (select sum(issuetotal) issuetotal ,sub_div_code  from (select count(*) issuetotal, sub_div_code from l_clra_registration_master a, district_master d, sub_division c where a.loc_e_dist=d.district_code and d.district_code=c.district_code and a.loc_e_subdivision=c.sub_div_code and d.district_code='".$district_code."' and final_submit_status='P' and status = 'I' group by sub_div_code  ) v  group by sub_div_code  ) AS insissu ON mw.sub_div_code = insissu.sub_div_code LEFT JOIN (select sum(backtotal) backtotal , sub_div_code  from (select count(*) backtotal, sub_div_code from l_clra_registration_master a, district_master d, sub_division c where   a.loc_e_dist=d.district_code  and d.district_code=c.district_code and   a.loc_e_subdivision=c.sub_div_code and d.district_code='".$district_code."' and final_submit_status='P' and status = 'B' group by sub_div_code  ) v  group by sub_div_code  ) AS insback ON mw.sub_div_code = insback.sub_div_code LEFT JOIN (select sum(rejtotal) rejtotal, sub_div_code from (select count(*) rejtotal, sub_div_code from l_clra_registration_master a, district_master d, sub_division c where   a.loc_e_dist=d.district_code  and d.district_code=c.district_code and   a.loc_e_subdivision=c.sub_div_code and d.district_code='".$district_code."' and final_submit_status='P' and status = 'R' group by sub_div_code) v  group by sub_div_code ) AS insrej ON mw.sub_div_code = insrej.sub_div_code");
	
	$db_query_2 	= 	db_query("select mw.sub_div_code, mw.sub_div_name , pentotal ,vpaytotal , vnotptotal , issuetotal , backtotal , rejtotal , inctotal from ( SELECT district_master.district_code, sub_div_code, sub_div_name FROM district_master , sub_division where district_master.district_code=sub_division.district_code and district_master.district_code='".$district_code."' GROUP BY district_master.district_code , sub_div_code, sub_div_name ORDER BY sub_div_name )mw LEFT JOIN ( select sum(inctotal) inctotal , sub_div_code  from (select count(*) inctotal, sub_div_code from l_clra_registration_master a, district_master d, sub_division c where   a.loc_e_dist=d.district_code  and d.district_code=c.district_code and a.loc_e_subdivision=c.sub_div_code and d.district_code='".$district_code."' and final_submit_status is null and status is null group by  sub_div_code  ) v  group by sub_div_code  ) AS insinc ON mw.sub_div_code = insinc.sub_div_code LEFT JOIN ( select sum(pentotal) pentotal , sub_div_code from (select count(*) pentotal, sub_div_code from l_clra_registration_master a, district_master d, sub_division c where   a.loc_e_dist=d.district_code  and d.district_code=c.district_code and   a.loc_e_subdivision=c.sub_div_code and d.district_code='".$district_code."' and final_submit_status='P' and status = '0' group by  sub_div_code  ) v  group by sub_div_code  ) AS inspen ON mw.sub_div_code = inspen.sub_div_code LEFT JOIN (select sum(vpaytotal) vpaytotal, sub_div_code from (select count(*) vpaytotal, sub_div_code from l_clra_registration_master a, district_master d, sub_division c where   a.loc_e_dist=d.district_code  and d.district_code=c.district_code and   a.loc_e_subdivision=c.sub_div_code and d.district_code='".$district_code."' and final_submit_status='P' and status = 'T' group by sub_div_code ) v  group by sub_div_code  ) AS insvt ON mw.sub_div_code = insvt.sub_div_code LEFT JOIN (select sum(vnotptotal) vnotptotal, sub_div_code from (select count(*) vnotptotal, sub_div_code from l_clra_registration_master a, district_master d, sub_division c where a.loc_e_dist=d.district_code and d.district_code=c.district_code and a.loc_e_subdivision=c.sub_div_code and d.district_code='".$district_code."' and final_submit_status='P' and status = 'V' group by sub_div_code ) v  group by sub_div_code  ) AS insvnotp ON mw.sub_div_code = insvnotp.sub_div_code LEFT JOIN (select sum(issuetotal) issuetotal ,sub_div_code  from (select count(*) issuetotal, sub_div_code from l_clra_registration_master a, district_master d, sub_division c where a.loc_e_dist=d.district_code and d.district_code=c.district_code and a.loc_e_subdivision=c.sub_div_code and d.district_code='".$district_code."' and final_submit_status='P' and status = 'I' group by sub_div_code  ) v  group by sub_div_code  ) AS insissu ON mw.sub_div_code = insissu.sub_div_code LEFT JOIN (select sum(backtotal) backtotal , sub_div_code  from (select count(*) backtotal, sub_div_code from l_clra_registration_master a, district_master d, sub_division c where   a.loc_e_dist=d.district_code  and d.district_code=c.district_code and   a.loc_e_subdivision=c.sub_div_code and d.district_code='".$district_code."' and final_submit_status='P' and status = 'B' group by sub_div_code  ) v  group by sub_div_code  ) AS insback ON mw.sub_div_code = insback.sub_div_code LEFT JOIN (select sum(rejtotal) rejtotal, sub_div_code from (select count(*) rejtotal, sub_div_code from l_clra_registration_master a, district_master d, sub_division c where   a.loc_e_dist=d.district_code  and d.district_code=c.district_code and   a.loc_e_subdivision=c.sub_div_code and d.district_code='".$district_code."' and final_submit_status='P' and status = 'R' group by sub_div_code) v  group by sub_div_code ) AS insrej ON mw.sub_div_code = insrej.sub_div_code");
	
	$db_query_3 	= 	db_query("select mw.sub_div_code, mw.sub_div_name , pentotal ,vpaytotal , vnotptotal , issuetotal , backtotal , rejtotal , inctotal from ( SELECT district_master.district_code, sub_div_code, sub_div_name FROM district_master , sub_division where district_master.district_code=sub_division.district_code and district_master.district_code='".$district_code."' GROUP BY district_master.district_code , sub_div_code, sub_div_name ORDER BY sub_div_name )mw LEFT JOIN ( select sum(inctotal) inctotal , sub_div_code  from (select count(*) inctotal, sub_div_code from l_clra_registration_master a, district_master d, sub_division c where   a.loc_e_dist=d.district_code  and d.district_code=c.district_code and a.loc_e_subdivision=c.sub_div_code and d.district_code='".$district_code."' and final_submit_status is null and status is null group by  sub_div_code  ) v  group by sub_div_code  ) AS insinc ON mw.sub_div_code = insinc.sub_div_code LEFT JOIN ( select sum(pentotal) pentotal , sub_div_code from (select count(*) pentotal, sub_div_code from l_clra_registration_master a, district_master d, sub_division c where   a.loc_e_dist=d.district_code  and d.district_code=c.district_code and   a.loc_e_subdivision=c.sub_div_code and d.district_code='".$district_code."' and final_submit_status='P' and status = '0' group by  sub_div_code  ) v  group by sub_div_code  ) AS inspen ON mw.sub_div_code = inspen.sub_div_code LEFT JOIN (select sum(vpaytotal) vpaytotal, sub_div_code from (select count(*) vpaytotal, sub_div_code from l_clra_registration_master a, district_master d, sub_division c where   a.loc_e_dist=d.district_code  and d.district_code=c.district_code and   a.loc_e_subdivision=c.sub_div_code and d.district_code='".$district_code."' and final_submit_status='P' and status = 'T' group by sub_div_code ) v  group by sub_div_code  ) AS insvt ON mw.sub_div_code = insvt.sub_div_code LEFT JOIN (select sum(vnotptotal) vnotptotal, sub_div_code from (select count(*) vnotptotal, sub_div_code from l_clra_registration_master a, district_master d, sub_division c where a.loc_e_dist=d.district_code and d.district_code=c.district_code and a.loc_e_subdivision=c.sub_div_code and d.district_code='".$district_code."' and final_submit_status='P' and status = 'V' group by sub_div_code ) v  group by sub_div_code  ) AS insvnotp ON mw.sub_div_code = insvnotp.sub_div_code LEFT JOIN (select sum(issuetotal) issuetotal ,sub_div_code  from (select count(*) issuetotal, sub_div_code from l_clra_registration_master a, district_master d, sub_division c where a.loc_e_dist=d.district_code and d.district_code=c.district_code and a.loc_e_subdivision=c.sub_div_code and d.district_code='".$district_code."' and final_submit_status='P' and status = 'I' group by sub_div_code  ) v  group by sub_div_code  ) AS insissu ON mw.sub_div_code = insissu.sub_div_code LEFT JOIN (select sum(backtotal) backtotal , sub_div_code  from (select count(*) backtotal, sub_div_code from l_clra_registration_master a, district_master d, sub_division c where   a.loc_e_dist=d.district_code  and d.district_code=c.district_code and   a.loc_e_subdivision=c.sub_div_code and d.district_code='".$district_code."' and final_submit_status='P' and status = 'B' group by sub_div_code  ) v  group by sub_div_code  ) AS insback ON mw.sub_div_code = insback.sub_div_code LEFT JOIN (select sum(rejtotal) rejtotal, sub_div_code from (select count(*) rejtotal, sub_div_code from l_clra_registration_master a, district_master d, sub_division c where   a.loc_e_dist=d.district_code  and d.district_code=c.district_code and   a.loc_e_subdivision=c.sub_div_code and d.district_code='".$district_code."' and final_submit_status='P' and status = 'R' group by sub_div_code) v  group by sub_div_code ) AS insrej ON mw.sub_div_code = insrej.sub_div_code");
	
	$output .= '<script type="text/javascript" src="'.$base_root.$base_path.'sites/all/modules/admin_advance_search/js/loader.js"></script>
				<script type="text/javascript" src="'.$base_root.$base_path.'sites/all/modules/admin_advance_search/js/jquery.highchartTable.js"></script>
				<script type="text/javascript" src="'.$base_root.$base_path.'sites/all/modules/admin_advance_search/js/highcharts.js"></script>
				<script type="text/javascript" src="'.$base_root.$base_path.'sites/all/modules/admin_advance_search/js/advce_search.js"></script>
				<script src="'.$base_root.$base_path.'sites/all/modules/admin_advance_search/js/data.js"></script>
				<script src="'.$base_root.$base_path.'sites/all/modules/admin_advance_search/js/exporting.js"></script>
				<script src="'.$base_root.$base_path.'sites/all/modules/admin_advance_search/js/drilldown.js"></script>
				<script src="'.$base_root.$base_path.'sites/all/modules/admin_advance_search/js/highchartcolor.js"></script>';
				
	$output .= '<style type="text/css">
						.tabs-menu {
							height: 30px;
							float: left;
							clear: both;
						}
						
						.tabs-menu li {
							height: 30px;
							line-height: 30px;
							float: left;
							margin-right: 10px;
							background-color: #ccc;
							border-top: 1px solid #d4d4d1;
							border-right: 1px solid #d4d4d1;
							border-left: 1px solid #d4d4d1;
						}
						
						.tabs-menu li.current {
							position: relative;
							background-color: #fff;
							border-bottom: 1px solid #fff;
							z-index: 5;
						}
						
						.tabs-menu li a {
							padding: 10px;
							text-transform: uppercase;
							color: #fff;
							text-decoration: none; 
						}
						
						.tabs-menu .current a {
							color: #2e7da3;
						}
						
						.tab {
							border: 1px solid #d4d4d1;
							background-color: #fff;
							float: left;
							margin-bottom: 20px;
							width: 100%;
						}
						
						.tab-content {
							width: 95%;
							padding: 20px;
							display: none;
						}
						
						#tab-1 {
						 display: block;   
						}

				</style>';
			foreach($db_query_3 as $obj2) {
				$total_application				=	0;
				
			 	$sub_div_name					=	drupal_strtoupper(trim($obj2->sub_div_name));
			 	$total_sub_inctotal				= 	!empty($obj2->inctotal) ? trim($obj2->inctotal) : 0;
			 	$total_application				=	$total_application+$total_sub_inctotal;
			 
			 	$total_sub_pentotal				= 	!empty($obj2->pentotal) ? trim($obj2->pentotal) : 0;
			 	$total_application				=	$total_application+$total_sub_pentotal;
			 
			 	$total_sub_back_application		=	!empty($obj2->backtotal) ? trim($obj2->backtotal) : 0;
			 	$total_application				=	$total_application+$total_sub_back_application;
			 
			  	$total_sub_veri_application		=	!empty($obj2->vpaytotal) ? trim($obj2->vpaytotal) : 0;
			  	$total_application				=	$total_application+$total_sub_veri_application;
			 
			 	$total_sub_not_veri_application =	!empty($obj2->vnotptotal) ? trim($obj2->vnotptotal) : 0;
			 	$total_application				=	$total_application+$total_sub_not_veri_application;
			
			 	$total_sub_issue_application	=	!empty($obj2->issuetotal) ? trim($obj2->issuetotal) : 0;
			 	$total_application				=	$total_application+$total_sub_issue_application;
			
			  	$total_sub_rej_application		=	!empty($obj2->rejtotal) ? trim($obj2->rejtotal) : 0;
			  	$total_application				=	$total_application+$total_sub_rej_application;
				
				$data1[] 	 =	'{
										name: "'.$sub_div_name.'",
										y: '.$total_application.',
										drilldown: "'.$sub_div_name.'"
									}, '; 
								
				$data2[]	 =	'{
											name: "'.$sub_div_name.'",
											id: "'.$sub_div_name.'",
											data: [
												[\'INCOMPLETE WITH CITIZEN\', '.$total_sub_inctotal.'],
												[\'PENDING IN OFFICE\', '.$total_sub_pentotal.'],
												[\'BACK FOR CORRECTION\', '.$total_sub_back_application.'],
												[\'VERIFIED AND PAYMENT MADE\', '.$total_sub_veri_application.'],
												[\'VERIFIED BUT PAYMENT NOT MADE\', '.$total_sub_not_veri_application.'],
												[\'ISSUED\', '.$total_sub_issue_application.'],
												[\'REJECTED\', '.$total_sub_rej_application.']
											]
										}, ';
			}	
			
			
	$output .= 	'<script type="text/javascript">
					  $(function () {
							Highcharts.chart(\'piechart\', {
								chart: {
									type: \'pie\'
								},
								title: {
									text: \'District Wise Subdivision - Pie Chart\'
								},
								subtitle: {
									text: \'Click the slices to view more.\'
								},
								plotOptions: {
									series: {
										dataLabels: {
											enabled: true,
											format: \'{point.name}: {point.y:,.0f}\'
										}
									}
								},
						
								tooltip: {
									headerFormat: \'<span style="font-size:11px">{series.name}</span><br>\',
									pointFormat: \'<span style="color:{point.color}">{point.name}</span>: <b>{point.y:,.0f} <br/>\'
								},
								series: [{
									name: \'Brands\',
									colorByPoint: true,
									data: [';
									
									foreach($data1 as $val1){
										$output .= $val1;
									}
									
									$output .= ']
								}],
								drilldown: {
									series: [';
									
									foreach($data2 as $val2){
										$output .= $val2;
									}
									
									$output .= ']
								}
							});
						});
				</script>';
			
		
		$output .=	'<script type="text/javascript">
						$(function () {
							Highcharts.chart(\'container\', {
								data: {
									table: \'datatable\'
								},
								chart: {
									type: \'column\'
								},
								title: {
									text: \'Chart Table View\'
								},
								yAxis: {
									allowDecimals: false,
									title: {
										text: \'Number of Applications\'
									}
								},
								tooltip: {
									formatter: function () {
										return \'<b>\' + this.series.name + \'</b><br/>\' +
											this.point.y + \' \' + this.point.name.toLowerCase();
									}
								}
							});
						});
					 </script>';
				
				
		$output .= '<style type="text/css">
						.demo-table { border:1px solid #eee; color:#2e2e2e;font-family:Arial, Helvetica, sans-serif;font-size:14px;font-weight:400; }
						.demo-table th { background:#eee;border:1px solid #ccc; padding: 8px 5px;vertical-align:top;text-transform:none; }
						.demo-table td { background:#fff;border:1px solid #ccc; padding: 8px 5px; }
						.demo-table tr:nth-child(even) td {background: #EAEAEA;}
						.demo-table tr:nth-child(odd) td {background: #FFF}
						.demo-table th:hover { background:pink;color:#000; }
						.demo-table tr:hover td { background:#ccc;}
						.demo-table tr td:hover { background:#aaa;color:#000; }
					</style>';	
					
		$output .= '<div id="tabs-container">
						<ul class="tabs-menu">
							<li class="current"><a href="#tab-1">Pie Chart</a></li>
							<li><a href="#tab-2">Tabular Report</a></li>
							<li><a href="#tab-3">Column Chart</a></li>
						</ul>
						<div class="tab">
							<div id="tab-1" class="tab-content">
								<div id="piechart" style="width: 850px; height: 500px;"></div>
							</div>
							<div id="tab-2" class="tab-content">';
			
		$output .= '<table width="100%" border="0" cellspacing="0" cellpadding="0" class="demo-table">
					  <tr>
						<td width="10%">Sub Division</td>
						<th width="10%">TOTAL APPLICATION</th>
						<th width="10%">INCOMPLETE WITH CITIZEN</th>
						<th width="10%">PENDING IN OFFICE</th>
						<th width="10%">BACK FOR CORRECTION</th>
						<th width="10%">VERIFIED AND PAYMENT MADE</th>
						<th width="10%">VERIFIED BUT PAYMENT NOT MADE</th>
						<th width="10%">ISSUED</th>
						<th>REJECTED</th>
					  </tr>';
					  
		foreach($db_query_1 as $obj) {
				$total_application				=	0;
			 	$sub_div_name					=	drupal_strtoupper(trim($obj->sub_div_name));
			 	$total_sub_inctotal				= 	trim($obj->inctotal);
			 	$total_application				=	$total_application+$total_sub_inctotal;
			 
			 	$total_sub_pentotal				= 	trim($obj->pentotal);
			 	$total_application				=	$total_application+$total_sub_pentotal;
			 
			 	$total_sub_back_application		=	trim($obj->backtotal);
			 	$total_application				=	$total_application+$total_sub_back_application;
			 
			  	$total_sub_veri_application		=	trim($obj->vpaytotal);
			  	$total_application				=	$total_application+$total_sub_veri_application;
			 
			 	$total_sub_not_veri_application =	trim($obj->vnotptotal);
			 	$total_application				=	$total_application+$total_sub_not_veri_application;
			
			 	$total_sub_issue_application	=	trim($obj->issuetotal);
			 	$total_application				=	$total_application+$total_sub_issue_application;
			
			  	$total_sub_rej_application		=	trim($obj->rejtotal);
			  	$total_application				=	$total_application+$total_sub_rej_application;
				
				
				$output .= '<tr>
							<td>'.$sub_div_name.'</td>
							<td>'.$total_application.'</td>
							<td>'.$total_sub_inctotal.'</td>
							<td>'.$total_sub_pentotal.'</td>
							<td>'.$total_sub_back_application.'</td>
							<td>'.$total_sub_veri_application.'</td>
							<td>'.$total_sub_not_veri_application.'</td>
							<td>'.$total_sub_issue_application.'</td>
							<td>'.$total_sub_rej_application.'</td>
						  </tr>';
				
			}
			
			$output .= '</table>';
			
			$output .= '</div>
							<div id="tab-3" class="tab-content">
								<div id="container" style="width: 850px; height: 400px;"></div>
								<table id="datatable" width="100%" border="0" cellspacing="0" cellpadding="0" class="demo-table" >
								<thead>
									<tr>
										<th>Sub Division</th>
										<th>TOTAL APPLICATION</th>
										<th>INCOMPLETE WITH CITIZEN</th>
										<th>PENDING IN OFFICE</th>
										<th>BACK FOR CORRECTION</th>
										<th>VERIFIED AND PAYMENT MADE</th>
										<th>VERIFIED BUT PAYMENT NOT MADE</th>
										<th>ISSUED</th>
										<th>REJECTED</th>
									</tr>
								</thead>
								<tbody>';
								
								foreach($db_query_2 as $obj1) {
									$total_application				=	0;
									$sub_div_name					=	drupal_strtoupper(trim($obj1->sub_div_name));
									$total_sub_inctotal				= 	trim($obj1->inctotal);
									$total_application				=	$total_application+$total_sub_inctotal;
								 
									$total_sub_pentotal				= 	trim($obj1->pentotal);
									$total_application				=	$total_application+$total_sub_pentotal;
								 
									$total_sub_back_application		=	trim($obj1->backtotal);
									$total_application				=	$total_application+$total_sub_back_application;
								 
									$total_sub_veri_application		=	trim($obj1->vpaytotal);
									$total_application				=	$total_application+$total_sub_veri_application;
								 
									$total_sub_not_veri_application =	trim($obj1->vnotptotal);
									$total_application				=	$total_application+$total_sub_not_veri_application;
								
									$total_sub_issue_application	=	trim($obj1->issuetotal);
									$total_application				=	$total_application+$total_sub_issue_application;
								
									$total_sub_rej_application		=	trim($obj1->rejtotal);
									$total_application				=	$total_application+$total_sub_rej_application;
									
									
									$output .= '<tr>
												<th>'.$sub_div_name.'</th>
												<td>'.$total_application.'</td>
												<td>'.$total_sub_inctotal.'</td>
												<td>'.$total_sub_pentotal.'</td>
												<td>'.$total_sub_back_application.'</td>
												<td>'.$total_sub_veri_application.'</td>
												<td>'.$total_sub_not_veri_application.'</td>
												<td>'.$total_sub_issue_application.'</td>
												<td>'.$total_sub_rej_application.'</td>
											  </tr>';
									
								}
								
							$output .=	'</tbody>
							</table>
							</div>
						</div>
					</div>';	
	return $output;
}

function get_super_admin_all_districts() {
	$default_select 	= array(0=>"- Select District -");
	$result 			= db_query("SELECT district_code,district_name FROM {district_master} where is_active = 'Y' order by district_name");
	$list 				= array();
	foreach ($result as $record) {
		$list[$record->district_code] = t('@districtname', array(
			'@districtname' => $record->district_name,
		));
	}
	return ($default_select+$list);
}

/*function random_color_part() {
    return str_pad( dechex( mt_rand( 0, 255 ) ), 2, '0', STR_PAD_LEFT);
}

function random_color() {
    return random_color_part() . random_color_part() . random_color_part();
}*/