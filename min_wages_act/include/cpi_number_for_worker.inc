<?php
function cpi_number_for_worker(){
	global $user, $base_root, $base_path;
	
	$output = '';
	$header = array(
				  array('data' => 'Year', 'width' => '8%'),
				  array('data' => 'January', 'colspan' => 2, 'width' => '' ),
				  array('data' => 'February', 'colspan' => 2, 'width' => '' ),
				  array('data' => 'March', 'colspan' => 2, 'width' => ''),					  
				  array('data' => 'April', 'colspan' => 2, 'width' => '' ),
				  array('data' => 'May', 'colspan' => 2, 'width' => ''),
				  array('data' => 'June', 'colspan' => 2, 'width' => ''),
				  array('data' => 'July', 'colspan' => 2, 'width' => '' ),
				  array('data' => 'August', 'colspan' => 2, 'width' => '' ),
				  array('data' => 'September', 'colspan' => 2, 'width' => '' ),
				  array('data' => 'October', 'colspan' => 2, 'width' => '' ),
				  array('data' => 'November', 'colspan' => 2, 'width' => '' ),
				  array('data' => 'December', 'colspan' => 2, 'width' => '' ),
		  );
		  	
	$rows[] = 	array(					
					array('data' =>	''),
					array('data' =>	'IW', 'title' => 'Industrial Workers on Base 2001=100'),
					array('data' =>	'AL'),
					array('data' =>	'IW', 'title' => 'Industrial Workers on Base 2001=100'),
					array('data' =>	'AL'),
					array('data' =>	'IW', 'title' => 'Industrial Workers on Base 2001=100'),
					array('data' =>	'AL'),
					array('data' =>	'IW', 'title' => 'Industrial Workers on Base 2001=100'),
					array('data' =>	'AL'),
					array('data' =>	'IW', 'title' => 'Industrial Workers on Base 2001=100'),
					array('data' =>	'AL'),
					array('data' =>	'IW', 'title' => 'Industrial Workers on Base 2001=100'),
					array('data' =>	'AL'),
					array('data' =>	'IW', 'title' => 'Industrial Workers on Base 2001=100'),
					array('data' =>	'AL'),
					array('data' =>	'IW', 'title' => 'Industrial Workers on Base 2001=100'),
					array('data' =>	'AL'),
					array('data' =>	'IW', 'title' => 'Industrial Workers on Base 2001=100'),
					array('data' =>	'AL'),
					array('data' =>	'IW', 'title' => 'Industrial Workers on Base 2001=100'),
					array('data' =>	'AL'),
					array('data' =>	'IW', 'title' => 'Industrial Workers on Base 2001=100'),
					array('data' =>	'AL'),
					array('data' =>	'IW', 'title' => 'Industrial Workers on Base 2001=100'),
					array('data' =>	'AL'),					
														
				 );	
	$centre_desc = 1;
	
	$cpiresult = db_query('select cpi.cpiyear, janiw, febiw, mrciw, apriw, mayiw, juneiw, julyiw, augiw, septiw, octiw, noviw, deciw, janal, febal, mrcal, apral, mayal, juneal, julyal, augal, septal, octal, noval, decal from (select cpiyear, centre_desc from l_mnw_cpi_number_master group by cpiyear, centre_desc) as cpi
left join(select iwvalue as janiw, alvalue as janal, cpiyear from l_mnw_cpi_number_master where month_srl=1 and is_active =1) as j on cpi.cpiyear = j.cpiyear
left join(select iwvalue as febiw, alvalue as febal, cpiyear from l_mnw_cpi_number_master where month_srl=2 and is_active =1) as f on cpi.cpiyear = f.cpiyear
left join(select iwvalue as mrciw, alvalue as mrcal, cpiyear from l_mnw_cpi_number_master where month_srl=3 and is_active =1) as m on cpi.cpiyear = m.cpiyear
left join(select iwvalue as apriw, alvalue as apral, cpiyear from l_mnw_cpi_number_master where month_srl=4 and is_active =1) as a on cpi.cpiyear = a.cpiyear
left join(select iwvalue as mayiw, alvalue as mayal, cpiyear from l_mnw_cpi_number_master where month_srl=5 and is_active =1) as my on cpi.cpiyear = my.cpiyear
left join(select iwvalue as juneiw, alvalue as juneal, cpiyear from l_mnw_cpi_number_master where month_srl=6 and is_active =1) as jn on cpi.cpiyear = jn.cpiyear
left join(select iwvalue as julyiw, alvalue as julyal, cpiyear from l_mnw_cpi_number_master where month_srl=7 and is_active =1) as jl on cpi.cpiyear = jl.cpiyear
left join(select iwvalue as augiw, alvalue as augal, cpiyear from l_mnw_cpi_number_master where month_srl=8 and is_active =1) as ag on cpi.cpiyear = ag.cpiyear
left join(select iwvalue as septiw, alvalue as septal, cpiyear from l_mnw_cpi_number_master where month_srl=9 and is_active =1) as s on cpi.cpiyear = s.cpiyear
left join(select iwvalue as octiw, alvalue as octal, cpiyear from l_mnw_cpi_number_master where month_srl=10 and is_active =1) as o on cpi.cpiyear = o.cpiyear
left join(select iwvalue as noviw, alvalue as noval, cpiyear from l_mnw_cpi_number_master where month_srl=11 and is_active =1) as n on cpi.cpiyear = n.cpiyear
left join(select iwvalue as deciw, alvalue as decal, cpiyear from l_mnw_cpi_number_master where month_srl=12 and is_active =1) as d on cpi.cpiyear = d.cpiyear where cpi.centre_desc = '.$centre_desc.' order by cpi.cpiyear desc');
	
	foreach($cpiresult as $data) { 
		$janiw = l('<i aria-hidden="true" title="Edit CPI value">'.$data->janiw.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',1), array('html' => TRUE));
		$janal = l('<i aria-hidden="true" title="Edit CPI value">'.$data->janal.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',1), array('html' => TRUE));
		$febiw = l('<i aria-hidden="true" title="Edit CPI value">'.$data->febiw.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',2), array('html' => TRUE));
		$febal = l('<i aria-hidden="true" title="Edit CPI value">'.$data->febal.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',2), array('html' => TRUE));
		$mariw = l('<i aria-hidden="true" title="Edit CPI value">'.$data->mrciw.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',3), array('html' => TRUE));
		$maral = l('<i aria-hidden="true" title="Edit CPI value">'.$data->mrcal.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',3), array('html' => TRUE));
		$apriw = l('<i aria-hidden="true" title="Edit CPI value">'.$data->apriw.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',4), array('html' => TRUE));
		$apral = l('<i aria-hidden="true" title="Edit CPI value">'.$data->apral.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',4), array('html' => TRUE));	
		$mayiw = l('<i aria-hidden="true" title="Edit CPI value">'.$data->mayiw.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',5), array('html' => TRUE));
		$mayal = l('<i aria-hidden="true" title="Edit CPI value">'.$data->mayal.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',5), array('html' => TRUE));
		$juneiw = l('<i aria-hidden="true" title="Edit CPI value">'.$data->juneiw.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',6), array('html' => TRUE));
		$juneal = l('<i aria-hidden="true" title="Edit CPI value">'.$data->juneal.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',6), array('html' => TRUE));
		$julyiw = l('<i aria-hidden="true" title="Edit CPI value">'.$data->julyiw.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',7), array('html' => TRUE));
		$julyal = l('<i aria-hidden="true" title="Edit CPI value">'.$data->julyal.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',7), array('html' => TRUE));
		$augiw = l('<i aria-hidden="true" title="Edit CPI value">'.$data->augiw.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',8), array('html' => TRUE));
		$augal = l('<i aria-hidden="true" title="Edit CPI value">'.$data->augal.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',8), array('html' => TRUE));	
		$septiw = l('<i aria-hidden="true" title="Edit CPI value">'.$data->septiw.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',9), array('html' => TRUE));
		$septal = l('<i aria-hidden="true" title="Edit CPI value">'.$data->septal.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',9), array('html' => TRUE));
		$octiw = l('<i aria-hidden="true" title="Edit CPI value">'.$data->octiw.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',10), array('html' => TRUE));
		$octal = l('<i aria-hidden="true" title="Edit CPI value">'.$data->octal.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',10), array('html' => TRUE));
		$noviw = l('<i aria-hidden="true" title="Edit CPI value">'.$data->noviw.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',11), array('html' => TRUE));
		$noval = l('<i aria-hidden="true" title="Edit CPI value">'.$data->noval.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',11), array('html' => TRUE));
		$deciw = l('<i aria-hidden="true" title="Edit CPI value">'.$data->deciw.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',12), array('html' => TRUE));
		$decal = l('<i aria-hidden="true" title="Edit CPI value">'.$data->decal.'</i>', 'cpi-information-edit/'.encryption_decryption_fun('encrypt',$data->cpiyear).'/'.encryption_decryption_fun('encrypt',12), array('html' => TRUE));
		
		
		$rows[] = array(
					array('data' =>	(!empty($data->cpiyear) ? $data->cpiyear : '-')),					
					array('data' =>	(!empty($data->janiw) ? $janiw : '-'), 'title' => 'IW-January-'.$data->cpiyear),
					array('data' =>	(!empty($data->janal) ? $janal : '-'), 'title' => 'AL-January-'.$data->cpiyear),					
					array('data' =>	(!empty($data->febiw) ? $febiw : '-'), 'title' => 'IW-February-'.$data->cpiyear),					
					array('data' =>	(!empty($data->febal) ? $febal : '-'), 'title' => 'AL-February-'.$data->cpiyear),					
					array('data' =>	(!empty($data->mrciw) ? $mariw : '-'), 'title' => 'IW-March-'.$data->cpiyear),					
					array('data' =>	(!empty($data->mrcal) ? $maral : '-'), 'title' => 'AL-March-'.$data->cpiyear),					
					array('data' =>	(!empty($data->apriw) ? $apriw : '-'), 'title' => 'IW-April-'.$data->cpiyear),					
					array('data' =>	(!empty($data->apral) ? $apral : '-'), 'title' => 'AL-April-'.$data->cpiyear),					
					array('data' =>	(!empty($data->mayiw) ? $mayiw : '-'), 'title' => 'IW-May-'.$data->cpiyear),
					array('data' =>	(!empty($data->mayal) ? $mayal : '-'), 'title' => 'AL-May-'.$data->cpiyear),					
					array('data' =>	(!empty($data->juneiw) ? $juneiw : '-'), 'title' => 'IW-June-'.$data->cpiyear),
					array('data' =>	(!empty($data->juneal) ? $juneal : '-'), 'title' => 'AL-June-'.$data->cpiyear),					
					array('data' =>	(!empty($data->julyiw) ? $julyiw : '-'), 'title' => 'IW-July-'.$data->cpiyear),
					array('data' =>	(!empty($data->julyal) ? $julyal : '-'), 'title' => 'AL-July-'.$data->cpiyear),					
					array('data' =>	(!empty($data->augiw) ? $augiw : '-'), 'title' => 'IW-August-'.$data->cpiyear),
					array('data' =>	(!empty($data->augal) ? $augal : '-'), 'title' => 'AL-August-'.$data->cpiyear),					
					array('data' =>	(!empty($data->septiw) ? $septiw : '-'), 'title' => 'IW-September-'.$data->cpiyear),
					array('data' =>	(!empty($data->septal) ? $septal : '-'), 'title' => 'AL-September-'.$data->cpiyear),					
					array('data' =>	(!empty($data->octiw) ? $octiw : '-'), 'title' => 'IW-October-'.$data->cpiyear),
					array('data' =>	(!empty($data->octal) ? $octal : '-'), 'title' => 'AL-October-'.$data->cpiyear),					
					array('data' =>	(!empty($data->noviw) ? $noviw : '-'), 'title' => 'IW-November-'.$data->cpiyear),
					array('data' =>	(!empty($data->noval) ? $noval : '-'), 'title' => 'AL-November-'.$data->cpiyear),					
					array('data' =>	(!empty($data->deciw) ? $deciw : '-'), 'title' => 'IW-December-'.$data->cpiyear),
					array('data' =>	(!empty($data->decal) ? $decal : '-'), 'title' => 'AL-December-'.$data->cpiyear),													
				 );		
	}
		
	$variables = array(
	  		'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')), 
	  		'header' => $header,
	  		'rows' => $rows,
			'empty' => t("No data found!")
	  );	  
	
	$output1 = '<div class="row"><div class="col-md-12"><div class="box box-primary"><div class="box-body">';
	$output2 = '</div></div></div></div>';
	
	$output = theme('datatable', $variables);
	
	return $output1.$output.$output2; 	
}

function cpi_number_for_worker_print(){
	
	global $user, $base_root, $base_path;
	
	$content = '<style type="text/css">
					table, td, th {
							font-size:12px;
							font-family:Times New Roman;
							color:#000;
							background:#fff;
					}
					th {
						background-color: #008BD1;
						color: white;
					}
					td:hover {
						background-color:#d4e3e5;
					}
					table td{
						border:0px solid #ccc;
						padding: 2px 10px;
					}
					.title{
						font-size:18px;
					}
					.cont{
						font-size:16px;
						display:inline-block;
						border-bottom:1px solid #000;
						line-height:20px;
					}
					table.sub-table{
						border-top: 1px solid #333;
						border-right: 1px solid #333;
					}
					table.sub-table tr td{
						border-bottom: 1px solid #333;
						border-left:1px solid #333;
					}
					table.pdf tr td { padding: 6px 10px; }
					table.pdf tr td.border-top{
						border-top: 1px solid #333;
					}
					table.pdf tr td.border-left-top{
						border-top: 1px solid #333;
						border-left:1px solid #333;
					}
					table.pdf tr td.border-left{
						border-left:1px solid #333;
					}
					table.pdf tr td.border-bottom{
						border-bottom: 1px solid #333;
					}
					table.pdf tr td.border-right{
						border-right: 1px solid #333;
					}
					table.pdf tr td.border-right-bottom{
						border-right: 1px solid #333;
						border-bottom:1px solid #333;
					}
					table.td-top tr td { vertical-align:top; }
					.cont2{
						font-size:18px;
						text-align:center;
						line-height:20px;
						text-transform:uppercase;
					}
					.cont3{
						font-size:16px;
						text-align:center;
						line-height:20px;
					}
					.header-pad-10 { padding-bottom:15px; }
					table.pdf tr td.bg-color{ background:#e5e5e5; }
					.right-align { text-align:right; }
				</style>';
	$content .= '<table width="100%" cellspacing="0" cellpadding="0" class="pdf w-left td-top">				
				    
				    <tr><td colspan="26" align="center" class="border-top border-bottom border-right border-left bg-color" width="35%" style="font-size:20px;"><strong>Consumer Price Index Numbers(Monthly)</strong></td></tr>
					
					<tr>
						<td class="border-bottom border-left bg-color" width="5%">Year</td>
						<td colspan="2" class="border-bottom border-left bg-color" width="8%">January</td>
						<td colspan="2" class="border-bottom border-left bg-color" width="8%">February</td>
						<td colspan="2" class="border-bottom border-left bg-color" width="8%">March</td>
						<td colspan="2" class="border-bottom border-left bg-color" width="8%">April</td>
						<td colspan="2" class="border-bottom border-left bg-color" width="7%">May</td>
						<td colspan="2" class="border-bottom border-left bg-color" width="8%">June</td>
						<td colspan="2" class="border-bottom border-left bg-color" width="8%">July</td>
						<td colspan="2" class="border-bottom border-left bg-color" width="8%">August</td>
						<td colspan="2" class="border-bottom border-left bg-color" width="8%">September</td>
						<td colspan="2" class="border-bottom border-left bg-color" width="8%">October</td>
						<td colspan="2" class="border-bottom border-left bg-color" width="8%">November</td>
						<td colspan="2" class="border-bottom border-right border-left bg-color" width="8%">December</td>						
					</tr>
					<tr>
						<td class="border-bottom border-left bg-color">&nbsp;</td>
						<td class="border-bottom border-left bg-color">IW</td>
						<td class="border-bottom border-left bg-color">AL</td>
						<td class="border-bottom border-left bg-color">IW</td>
						<td class="border-bottom border-left bg-color">AL</td>
						<td class="border-bottom border-left bg-color">IW</td>
						<td class="border-bottom border-left bg-color">AL</td>
						<td class="border-bottom border-left bg-color">IW</td>
						<td class="border-bottom border-left bg-color">AL</td>
						<td class="border-bottom border-left bg-color">IW</td>
						<td class="border-bottom border-left bg-color">AL</td>
						<td class="border-bottom border-left bg-color">IW</td>
						<td class="border-bottom border-left bg-color">AL</td>
						<td class="border-bottom border-left bg-color">IW</td>
						<td class="border-bottom border-left bg-color">AL</td>
						<td class="border-bottom border-left bg-color">IW</td>
						<td class="border-bottom border-left bg-color">AL</td>
						<td class="border-bottom border-left bg-color">IW</td>
						<td class="border-bottom border-left bg-color">AL</td>
						<td class="border-bottom border-left bg-color">IW</td>
						<td class="border-bottom border-left bg-color">AL</td>
						<td class="border-bottom border-left bg-color">IW</td>
						<td class="border-bottom border-left bg-color">AL</td>
						<td class="border-bottom border-left bg-color">IW</td>
						<td class="border-bottom border-right border-left bg-color">AL</td>
					</tr>';	  	
		
	$centre_desc = 1;
	
	$cpiresult = db_query('select cpi.cpiyear, janiw, febiw, mrciw, apriw, mayiw, juneiw, julyiw, augiw, septiw, octiw, noviw, deciw, janal, febal, mrcal, apral, mayal, juneal, julyal, augal, septal, octal, noval, decal from (select cpiyear, centre_desc from l_mnw_cpi_number_master group by cpiyear, centre_desc) as cpi
left join(select iwvalue as janiw, alvalue as janal, cpiyear from l_mnw_cpi_number_master where month_srl=1 and is_active =1) as j on cpi.cpiyear = j.cpiyear
left join(select iwvalue as febiw, alvalue as febal, cpiyear from l_mnw_cpi_number_master where month_srl=2 and is_active =1) as f on cpi.cpiyear = f.cpiyear
left join(select iwvalue as mrciw, alvalue as mrcal, cpiyear from l_mnw_cpi_number_master where month_srl=3 and is_active =1) as m on cpi.cpiyear = m.cpiyear
left join(select iwvalue as apriw, alvalue as apral, cpiyear from l_mnw_cpi_number_master where month_srl=4 and is_active =1) as a on cpi.cpiyear = a.cpiyear
left join(select iwvalue as mayiw, alvalue as mayal, cpiyear from l_mnw_cpi_number_master where month_srl=5 and is_active =1) as my on cpi.cpiyear = my.cpiyear
left join(select iwvalue as juneiw, alvalue as juneal, cpiyear from l_mnw_cpi_number_master where month_srl=6 and is_active =1) as jn on cpi.cpiyear = jn.cpiyear
left join(select iwvalue as julyiw, alvalue as julyal, cpiyear from l_mnw_cpi_number_master where month_srl=7 and is_active =1) as jl on cpi.cpiyear = jl.cpiyear
left join(select iwvalue as augiw, alvalue as augal, cpiyear from l_mnw_cpi_number_master where month_srl=8 and is_active =1) as ag on cpi.cpiyear = ag.cpiyear
left join(select iwvalue as septiw, alvalue as septal, cpiyear from l_mnw_cpi_number_master where month_srl=9 and is_active =1) as s on cpi.cpiyear = s.cpiyear
left join(select iwvalue as octiw, alvalue as octal, cpiyear from l_mnw_cpi_number_master where month_srl=10 and is_active =1) as o on cpi.cpiyear = o.cpiyear
left join(select iwvalue as noviw, alvalue as noval, cpiyear from l_mnw_cpi_number_master where month_srl=11 and is_active =1) as n on cpi.cpiyear = n.cpiyear
left join(select iwvalue as deciw, alvalue as decal, cpiyear from l_mnw_cpi_number_master where month_srl=12 and is_active =1) as d on cpi.cpiyear = d.cpiyear where cpi.centre_desc = '.$centre_desc.' order by cpi.cpiyear desc');
	
	foreach($cpiresult as $data) {
		$content .= '<tr><td class="border-bottom border-left">'.$data->cpiyear.'</td>
			<td class="border-bottom border-left">'.(!empty($data->janiw) ? $data->janiw : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->janal) ? $data->janal : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->febiw) ? $data->febiw : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->febal) ? $data->febal : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->mrciw) ? $data->mrciw : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->mrcal) ? $data->mrcal : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->apriw) ? $data->apriw : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->apral) ? $data->apral : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->mayiw) ? $data->mayiw : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->mayal) ? $data->mayal : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->juneiw) ? $data->juneiw : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->juneal) ? $data->juneal : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->julyiw) ? $data->julyiw : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->julyal) ? $data->julyal : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->augiw) ? $data->augiw : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->augal) ? $data->augal : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->septiw) ? $data->septiw : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->septal) ? $data->septal : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->octiw) ? $data->octiw : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->octal) ? $data->octal : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->noviw) ? $data->noviw : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->noval) ? $data->noval : '--').'</td>
			<td class="border-bottom border-left">'.(!empty($data->deciw) ? $data->deciw : '--').'</td>
			<td class="border-bottom border-left border-right">'.(!empty($data->decal) ? $data->decal : '--').'</td>
		</tr>';		
	}		  
	
	$content .= '</table>';	
	
	return pdf_using_mpdf_api($content, 'cpi-numbers-monthly'._.time(), 'A4-L');	
}
?>