<?php
function cpi_number_view(){
	$output = '';
   $output .='<div class="row">
				  <div class="col-sm-12">										
					  <div id="content-2" class="scrollContent content mCustomScrollbar light" data-mcs-theme="minimal-dark">
						  <table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table fixed_headers">
						  	<thead>';
	
	$output .= '<tr style="font-weight:700;"><td width="7%">Year</td>';
	$output .= '<td>Centre Desc</td>';
	$output .= '<td>January</td><td>February</td><td>March</td><td>April</td><td>May</td><td>June</td><td>July</td><td>August</td><td>September</td><td>October</td><td>November</td><td>December</td></tr>';	
	
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
		$output .= '<tr><td>'.$data->cpiyear.'</td>';
		$output .= '<td>KOLKATA</td>';				
		$output .= '<td>'.$data->janiw.'</td>';			
		$output .= '<td>'.$data->febiw.'</td>';	
		$output .= '<td>'.$data->mrciw.'</td>';			
		$output .= '<td>'.$data->apriw.'</td>';					
		$output .= '<td>'.$data->mayiw.'</td>';			
		$output .= '<td>'.$data->juneiw.'</td>';		
		$output .= '<td>'.$data->julyiw.'</td>';	
		$output .= '<td>'.$data->augiw.'</td>';			
		$output .= '<td>'.$data->septiw.'</td>';				
		$output .= '<td>'.$data->octiw.'</td>';				
		$output .= '<td>'.$data->noviw.'</td>';			
		$output .= '<td>'.$data->deciw.'</td><tr>';	
		
	}
	$output .= '</tbody></table></div></div></div>';

	return $output;
}
?>