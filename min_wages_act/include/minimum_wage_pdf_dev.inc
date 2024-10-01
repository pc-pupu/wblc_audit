<?php
function minimum_wage_pdf($employment_id, $year, $session){
	
	$employment_id = encryption_decryption_fun('decrypt',$employment_id);
	$year = encryption_decryption_fun('decrypt',$year);
	$session = encryption_decryption_fun('decrypt',$session);
	
	$min_wage_info = db_select('l_min_wages_schedule_employment_details_info_dev','l');
	$min_wage_info->fields('l', array());
	$min_wage_info->condition('l.employment_id',$employment_id);
	$min_wage_info->condition('l.year',$year);
	$min_wage_info->condition('l.session',$session);
	$min_wage_info = $min_wage_info->execute()->fetchAssoc(); // print_r($min_wage_info); die;
	
	$fix_al_value = db_select('l_mnw_fixation_neutralization_initial_fixed_point_master','l');
	$fix_al_value->fields('l', array('fix_al_value'));
	$fix_al_value->condition('l.is_active',1);
	$fix_al_value = $fix_al_value->execute()->fetchField(); 
	
	$min_wage_other_info = db_select('l_min_wages_schedule_employment_master','l');
	$min_wage_other_info->fields('l', array());
	$min_wage_other_info->condition('l.id',$employment_id);
	$min_wage_other_info = $min_wage_other_info->execute()->fetchAssoc();
	 
	$name_of_employment = $min_wage_info['employment_title'];
	$notification_number = $min_wage_other_info['notification_number']; 
	$notification_date = (!empty($min_wage_other_info['notification_date'])?date("d.m.Y",$min_wage_other_info['notification_date']):'');
	$notification_published_dt =  date("d.m.Y",$min_wage_other_info['notification_published_dt']);
	$type_of_sch_emp = $min_wage_other_info['type_of_sch_emp'];
		
	
	if($min_wage_info['session']=='1'){
		$period = '1st January, '.$min_wage_info['year'].' to 30th June, '.$min_wage_info['year'];
	}else if($min_wage_info['session']=='3'){
		$period = 'September, '.($min_wage_info['year']-1).' to February, '.$min_wage_info['year'];
	}else if($min_wage_info['session']=='4'){
		$period = 'March, '.($min_wage_info['year']).' to August, '.$min_wage_info['year'];
	}else{
		$period = '1st July,'.$min_wage_info['year'].' to 31st Dec, '.$min_wage_info['year'];
	}
	
	if(trim($min_wage_info['employment_id']) == 91){
		$notification_date = '3.7.1972 / 22.7.1982';
	}

	$output='<style type="text/css">
	.pdf-circular-table td,th,caption{background-color:#fff; padding: 5px 10px;}
	.pdf-circular-table { background-color:#333; }
	.pdf-circular-table th { font-size:14px;font-weight:700; }
	.pdf-circular-table td { font-size:12px; font-weight:300; }
	</style>'; 
	
	$img = $GLOBALS['base_url'].'/sites/all/themes/lcTheme/images/Gov-logo-sm.png';
	
	$output .= '<div style="text-align:center;"><img src="'.$img.'" alt=""><h5 style="font-size:14px;font-weight:bold;margin:4px 0 20px 0;">Government of West Bengal<br> Office of the Labour Commissioner<br> 6, Church Lane, 3rd floor, Kolkata –700 001.</h5></div>
	<div style="overflow:hidden;margin:0 0 20px 0;"><div style="float:left;width:50%;">No. &emsp;'.$min_wage_info['publish_memo_no1'].$min_wage_other_info['circular_no'].'</div><div style="float:right;width:40%;text-align:right;">Dated: __/__/____ </div></div>
	<div style="text-align:center;font-size:16px;font-weight:bold;margin:0 0 15px 0;">CIRCULAR</div>
	<div style="font-size:15px;font-weight:700; text-align:justify">The minimum rates of wages for those in the employment of  <b>'.$name_of_employment.' </b>in the state of West Bengal covered as per notification number '.$notification_number.' dated '.$notification_date.' are being hereby fixed as under';
	
	if(!empty($min_wage_other_info['notification_published_dt'])){ 
		$output .= ',as published in the Kolkata Gazette Extraordinary dated '.$notification_published_dt.'.';
	}else{$output .= '.';}
	if($type_of_sch_emp == '2' || $type_of_sch_emp == '4' || trim($min_wage_info['employment_id']) == '69'){
		$output .= 'These rates will be effective for the period from <b>'.$period.'</b>.';
	}
	$output .= '<ol type="a" style="line-height:20px text-align:justify">';
	if($type_of_sch_emp == '1' || $type_of_sch_emp == '3'){
	$output .= '<li> Now, with reference to the said notification, the following shall be the minimum rates of wages in the said employment for the period from <b>'.$period.'</b>.</li>';$output .= '</div>';
	}
	
	$output .= '<div style="margin:20px 0;">
					<table width="100%" cellspacing="1" class="pdf-circular-table">
  						<tr>'; 
	if($type_of_sch_emp == '1' || $type_of_sch_emp == '4'){
		 $rowspan = '2';$colspan = '2';
	}else if(trim($min_wage_info['employment_id']) == 1){
		 $rowspan = '3'; $colspan = '3'; 
	// }else if(trim($min_wage_info['employment_id']) == 75){
	// 	 $rowspan = '2'; $colspan = '9'; 
	}else{
		 $rowspan = '';$colspan = '';
	}
	if(trim($min_wage_info['employment_id']) == 91){
		$output .= '<th align="center" valign="top" width="">REGION</th>
					<th align="center">RATE PER 1000 BIDIS ROLLED (Rs.)</th>
					<th align="center">RATE PER DAY (Rs.)</th>
					<th align="center">RATE PER MONTH (Rs.)</th>
				</tr>
				<tr>
					<td align="center" valign="top">Calcutta, North 24 Pgs. &amp; South 24 Pgs.</td>
					<td align="center" valign="top">266.06</td>
					<td align="center" valign="top">264.96</td>
					<td align="center" valign="top">6888.96</td>
				</tr>
				<tr> 
					<td align="center" valign="top">Howrah &amp; Hooghly</td>
					<td align="center" valign="top">242.88</td>
					<td align="center" valign="top">241.93</td>
					<td align="center" valign="top">6290.23</td>
				</tr>
				<tr>
					<td align="center" valign="top">Other Districts</td>
					<td align="center" valign="top">259.82</td>
					<td align="center" valign="top">258.87</td>
					<td align="center" valign="top">6730.62</td>
				</tr>';
	}else{
		$output .= '<th rowspan="'.$rowspan.'" align="center" valign="top" width="">Categories of Employees</th>
					<th colspan="'.$colspan.'" align="center">Monthly Minimum Rates of Wages (Rs.)</th>
				</tr>';
	}
	/*************** AGRICULTUR ********************/				 
	if(trim($min_wage_info['employment_id']) == 1){				 
  		$output .= '<tr>
						<td rowspan="2" align="center" >Per Month (Without food)(Rs.)</td>
						<td colspan="2" align="center">Per Day(Rs.)</td>
					</tr>
					<tr>
						<td>Without food</td>
						<td>With food</td>
					</tr>
					<tr>
						<td>Unskilled</td>
						<td>'.number_format(round($min_wage_info['unskilled_rate_zone_a']),2).'</td>
						<td>'.number_format(round($min_wage_info['unskilled_rate_zone_a']/26),2).'</td>
						<td>'.number_format(round($min_wage_info['unskilled_rate_zone_a']/26)-17,2).'</td>
					</tr>
					<tr>
						<td>Semi-skilled</td>
						<td>'.number_format(round($min_wage_info['semiskilled_rate_zone_a']),2).'</td>
						<td>'.number_format(round($min_wage_info['semiskilled_rate_zone_a']/26),2).'</td>
						<td>'.number_format(round($min_wage_info['semiskilled_rate_zone_a']/26)-17,2).'</td>
					</tr>
					<tr>
						<td>Skilled</td>
						<td>'.number_format(round($min_wage_info['skilled_rate_zone_a']),2).'</td>
						<td>'.number_format(round($min_wage_info['skilled_rate_zone_a']/26),2).'</td>
						<td>'.number_format(round($min_wage_info['skilled_rate_zone_a']/26)-17,2).'</td>
					</tr>'
					;
					
	/*************** BEEDI LEAF PLUCKING ********************/
		
	}else if(trim($min_wage_info['employment_id']) == 6){				 
  		$output .= '<tr><td td align="left">1. Full-time workers <br>(a) Unskilled <br> (b) Semi- siklled <br> (c) Skilled</td>
						<td align="left">'.number_format(round($min_wage_info['unskilled_rate_zone_a']),2).' (Daily - '.number_format(round($min_wage_info['unskilled_rate_zone_a']/26),2).')
						<br>'.number_format(round($min_wage_info['semiskilled_rate_zone_a']),2).' (Daily - '.number_format(round($min_wage_info['semiskilled_rate_zone_a']/26),2).')
						<br>'.number_format(round($min_wage_info['skilled_rate_zone_a']),2).' (Daily - '.number_format(round($min_wage_info['skilled_rate_zone_a']/26),2).')
						</td>
					</tr>
					<tr><td>2. Part-time workers<br>(a) Munshi <br> (b) Moto/Paltai Labour <br> (c) Others</td>
						<td align="left">
							'.number_format(round($min_wage_info['unskilled_rate_zone_a']/2),2).' (Daily - '.number_format(round(($min_wage_info['unskilled_rate_zone_a']/2)/26),2).')
							<br>'.number_format(round($min_wage_info['unskilled_rate_zone_a']/3),2).' (Daily - '.number_format(round(($min_wage_info['unskilled_rate_zone_a']/3)/26),2).')
							<br>'.number_format(round($min_wage_info['unskilled_rate_zone_a']/2),2).' (Daily - '.number_format(round(($min_wage_info['unskilled_rate_zone_a']/2)/26),2).')
						</td>
					</tr>
					<tr><td>3. Piece-rated workers<br>(a) Plucker <br> (b) Processor <br> (c) Packer <br> (d) Packing Coolie</td>
						<td align="left">'.number_format((15*$min_wage_info['cpi_value_zone_b'])/$fix_al_value,2).' (per 100 leaves)
							<br>'.number_format((12*$min_wage_info['cpi_value_zone_b'])/$fix_al_value,2).' (per 3 kg. bundle Rs. 12.00) 
							<br>'.number_format((6*$min_wage_info['cpi_value_zone_b'])/$fix_al_value,2).' (per 50 kg. bag Rs. 6.00)
							<br>'.number_format((6*$min_wage_info['cpi_value_zone_b'])/$fix_al_value,2).' (per 50 kg. bag Rs. 6.00)
						</td>
					</tr>';
	/******************* CINCHONA PLANTATION  & FISHERY & TOBACCO GODOWN********************/				
	}else if(trim($min_wage_info['employment_id']) == 18 || trim($min_wage_info['employment_id']) == 34 ||trim($min_wage_info['employment_id']) == 38 ||trim($min_wage_info['employment_id']) == 92 || trim($min_wage_info['employment_id']) == 58){
		$output .= '<tr>
						<td align="center">Adult Worker</td>
						<td align="center">'.number_format(round($min_wage_info['unskilled_rate_zone_a']),2).'<br>
						(Daily - '.number_format(round($min_wage_info['unskilled_rate_zone_a']/26),2).')</td>
					</tr>';
	}else if(trim($min_wage_info['employment_id']) == 3){
		$output .= '<tr>
						<td align="center" >Per Month(Rs.)</td>
						<td align="center">Per Day(Rs.)</td>
					</tr>
					<tr>
						<td align="center">Unskilled</td>
						<td align="center">'.number_format(round($min_wage_info['unskilled_rate_zone_a']),2).'</td>
						<td align="center">'.number_format(round($min_wage_info['unskilled_rate_zone_a']/26),2).'</td>
					</tr>';
	}else if(trim($min_wage_info['employment_id']) == 13){
		$output .= '<tr>
						<td align="center" >Without Accommodation(Rs.)</td>
						<td align="center">With Accommodation(Rs.)</td>
					</tr>
					<tr>
						<td align="center">Unskilled</td>
						<td align="center">'.number_format(round($min_wage_info['unskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($min_wage_info['unskilled_rate_zone_a']/26),2).')</td>
						<td align="center">'.number_format(round($min_wage_info['unskilled_rate_zone_b']),2).'<br>(Daily - '.number_format(round($min_wage_info['unskilled_rate_zone_b']/26),2).')</td>
					</tr>
					<tr>
						<td align="center">Semi-Skilled</td>
						<td align="center">'.number_format(round($min_wage_info['semiskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($min_wage_info['semiskilled_rate_zone_a']/26),2).')</td>
						<td align="center">'.number_format(round($min_wage_info['semiskilled_rate_zone_b']),2).'<br>(Daily - '.number_format(round($min_wage_info['semiskilled_rate_zone_b']/26),2).')</td>
					</tr>
					<tr>
						<td align="center">Skilled</td>
						<td align="center">'.number_format(round($min_wage_info['skilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($min_wage_info['skilled_rate_zone_a']/26),2).')</td>
						<td align="center">'.number_format(round($min_wage_info['skilled_rate_zone_b']),2).'<br>(Daily - '.number_format(round($min_wage_info['skilled_rate_zone_b']/26),2).')</td>
					</tr>';
	}else if(trim($min_wage_info['employment_id']) == 71){
		$output .= '<tr>
						<td >Unskilled / Others &#8217; A &#8217;</td>
						<td align="center">'.number_format(round($min_wage_info['unskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($min_wage_info['unskilled_rate_zone_a']/26),2).')</td>
					</tr>
					<tr>
						<td >Semi-Skilled </td>
						<td align="center">'.number_format(round($min_wage_info['semiskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($min_wage_info['semiskilled_rate_zone_a']/26),2).')</td>
					</tr>
					<tr>
						<td>Skilled / Others &#8217; B &#8217;</td>
						<td align="center">'.number_format(round($min_wage_info['skilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($min_wage_info['skilled_rate_zone_a']/26),2).')</td>
					</tr>
					<tr>
						<td>Highly Skilled / Others &#8217; C &#8217;</td>
						<td align="center">'.number_format(round($min_wage_info['highskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($min_wage_info['highskilled_rate_zone_a']/26),2).')</td>
					</tr>
					';
	}else if(trim($min_wage_info['employment_id']) == 75){
		$output .= '<tr>
						<td align="center" >Calcutta, 24 Pgs (N) &amp; 24 Pgs (S)</td>
						<td align="center">Howrah &amp; Hooghly</td>
						<td align="center">Burdwan</td>
						<td align="center">Darjeeling, except Siliguri Sub-Division.</td>
						<td align="center">Jalpaigur, Coochbehar &amp; Siliguri Sub-Division.</td>
						<td align="center">Bankura, Midnapore &amp; Purulia</td>
						<td align="center">Birbhum</td>
						<td align="center">Nadia &amp; Murshidabad</td>
						<td align="center">Malda, Uttar Dinajpur &amp; Dakshin Dinajpur</td>
					</tr>
					<tr>
						<td align="left"><br>Unskilled / Peon <br></td>
						<td align="center">6896.00</td>
						<td align="center">6335.00</td>
						<td align="center">6851.00</td>
						<td align="center">5138.00</td>
						<td align="center">4883.00</td>
						<td align="center">6829.00</td>
						<td align="center">6773.00</td>
						<td align="center">6828.00</td>
						<td align="center">6540.00</td>
					</tr>
					<tr>
						<td align="left"><br>Semi-skilled / Durwan <br></td>
						<td align="center">6907.00</td>
						<td align="center">6346.00</td>
						<td align="center">6862.00</td>
						<td align="center">5149.00</td>
						<td align="center">4894.00</td>
						<td align="center">6840.00</td>
						<td align="center">6784.00</td>
						<td align="center">6839.00</td>
						<td align="center">6551.00</td>
					</tr>
					<tr>
						<td align="left"><br>Skilled / Clerks <br></td>
						<td align="center">6933.00</td>
						<td align="center">6372.00</td>
						<td align="center">6888.00</td>
						<td align="center">5175.00</td>
						<td align="center">4920.00</td>
						<td align="center">6866.00</td>
						<td align="center">6810.00</td>
						<td align="center">6865.00</td>
						<td align="center">6577.00</td>
					</tr>	';
	}else if(trim($min_wage_info['employment_id']) == 69){
		$output .= '<tr>
						<td align="center" >CLEANERS &amp; LORRY COOLIES</td>
						<td align="center">'.number_format(round($min_wage_info['unskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($min_wage_info['unskilled_rate_zone_a']/26),2).')</td>
					</tr>
					<tr>
						<td align="center">CONDUCTORS</td>
						<td align="center">'.number_format(round($min_wage_info['semiskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($min_wage_info['semiskilled_rate_zone_a']/26),2).')</td>
					</tr>
					<tr>
						<td align="center">FITTERS &amp; TIME-KEEPERS</td>
						<td align="center">'.number_format(round($min_wage_info['skilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($min_wage_info['skilled_rate_zone_a']/26),2).')</td>
					</tr>
					<tr>
						<td align="center">DRIVER &amp; MECHANICS</td>
						<td align="center">'.number_format(round($min_wage_info['highskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($min_wage_info['highskilled_rate_zone_a']/26),2).')</td>
					</tr>';
		
	}else{
		if($type_of_sch_emp == '1' || $type_of_sch_emp == '4'){
			$output .= '<tr>
							<th align="center">Zone A (Rs.)</th>
							<th align="center">Zone B (Rs.)</th>
						</tr>';
		}
		if(!empty($min_wage_info['unskilled_rate_zone_a'])){
		  $output .= '<tr>
			<td align="left">Unskilled</td>
			<td align="center">'.number_format(round($min_wage_info['unskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($min_wage_info['unskilled_rate_zone_a']/26),2).')</td>';
			if($type_of_sch_emp == '1' || $type_of_sch_emp == '4'){
			$output .= '<td align="center">'.number_format(round($min_wage_info['unskilled_rate_zone_b']),2).'<br>(Daily - '.number_format(round($min_wage_info['unskilled_rate_zone_b']/26),2).')</td>';
		}
			 $output .= '</tr>';
		}
		if(!empty($min_wage_info['semiskilled_rate_zone_a'])){
		  $output .= '<tr>
			<td align="left">Semi-Skilled</td>
			<td align="center">'.number_format(round($min_wage_info['semiskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($min_wage_info['semiskilled_rate_zone_a']/26),2).')</td>';
			if($type_of_sch_emp == '1' || $type_of_sch_emp == '4'){
				$output .= '<td align="center">'.number_format(round($min_wage_info['semiskilled_rate_zone_b']),2).'<br>(Daily - '.number_format(round($min_wage_info['semiskilled_rate_zone_b']/26),2).')</td>';
			}
		 $output .= '</tr>';
		}
		if(!empty($min_wage_info['semiskilled_rate_zone_a'])){
			$output .= '<tr>
			<td align="left">Skilled</td>
			<td align="center">'.number_format(round($min_wage_info['skilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($min_wage_info['skilled_rate_zone_a']/26),2).')</td>';
			if($type_of_sch_emp == '1' || $type_of_sch_emp == '4'){
				$output .= '<td align="center">'.number_format(round($min_wage_info['skilled_rate_zone_b']),2).'<br>(Daily - '.number_format(round($min_wage_info['skilled_rate_zone_b']/26),2).')</td>';
			}
		  $output .= '</tr>';
		}
		if(!empty($min_wage_info['highskilled_rate_zone_a'])){
		  $output .= '<tr>
			<td align="left">Highly Skilled</td>
			<td align="center">'.number_format(round($min_wage_info['highskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($min_wage_info['highskilled_rate_zone_a']/26),2).')</td>';
			if($type_of_sch_emp == '1' || $type_of_sch_emp == '4'){
				$output .= '
				<td align="center">'.number_format(round($min_wage_info['highskilled_rate_zone_b']),2).'<br>(Daily - '.number_format(round($min_wage_info['highskilled_rate_zone_b']/26),2).')</td>';
			}
		  $output .= '</tr>';
		}
	}
$output .= '</table></div>';
if(trim($min_wage_info['employment_id']) == 13){
	$output .= 'The following shall be the piece rates of wages for different categories of employees employed in <b>Brick Manufactories</b> in West Bengal: 
				<div style="margin:20px 0;"><table width="100%" cellspacing="1" class="pdf-circular-table">
					<tr>
						<th rowspan="2" width="40%"> Category</th>
						<th colspan="2"> Rates of Minimum Wage per thousand brick</th>
					</tr>
					<tr>
						<th> Without Accommodation(Rs.)</th>
						<th> With Accommodation(Rs.)</th>
					</tr>
					<tr>
						<td>1.(a) Brick moulding pugmill 
									<br>&emsp;&emsp;(i) Pathora 
									<br>&emsp;&emsp;(ii) Mutphulia 
									<br>&emsp;&emsp;(iii) Tubwaya
									<br>&emsp;&emsp;(iv) Mukkata 
									<br>&emsp;&emsp;(v) Boyel Hakka <br><br> 
							  &ensp;&nbsp;(b) Lathgung </td>
						<td><br>'.number_format(round((268*$min_wage_info['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
								 .number_format(round((68*$min_wage_info['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
								 .number_format(round((34*$min_wage_info['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
								 .number_format(round((34*$min_wage_info['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
								 .number_format(round((11*$min_wage_info['cpi_value_zone_b'])/$fix_al_value),2).'<br><br>'
								 .number_format(round((301*$min_wage_info['cpi_value_zone_b'])/$fix_al_value),2).'</td>
								 
						<td><br>'.number_format(round((252*$min_wage_info['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
								 .number_format(round((60*$min_wage_info['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
								 .number_format(round((30*$min_wage_info['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
								 .number_format(round((30*$min_wage_info['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
								 .number_format(round((9*$min_wage_info['cpi_value_zone_b'])/$fix_al_value),2).'<br><br>'
								 .number_format(round((285*$min_wage_info['cpi_value_zone_b'])/$fix_al_value),2).'</td>
					</tr>
					<tr>
						<td>2.(a) Loader of Bricks in Bhatta <br>
							   &ensp;&nbsp;(b) Unloader of Bricks from Bhatta <br>
							   &ensp;&nbsp;(c) Earth Cutter</td>
						<td>'.number_format(round((143*$min_wage_info['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
							 .number_format(round((135*$min_wage_info['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
							 .number_format(round((113*$min_wage_info['cpi_value_zone_b'])/$fix_al_value),2).'<br>
							 (Per 100 cft.)</td>
								 
						<td>'.number_format(round((120*$min_wage_info['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
							 .number_format(round((120*$min_wage_info['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
							 .number_format(round((97*$min_wage_info['cpi_value_zone_b'])/$fix_al_value),2).'<br>
							 (Per 100 cft.)</td>
					</tr>
					
				</table></div>';
}
if($min_wage_info['employment_id'] == 5){
	$output .= '<li><b> Clarification: </b>It has been clarified by the Labour Department, Government of West Bengal by No. 134-MW/2W-18/2010 dated 27.08.2012 that the Employment of Bakery includes Biscuit Manufacturing Units also.</li> ';
}	
if($min_wage_other_info['implementing_area'] == 2 || $min_wage_other_info['implementing_area'] == 3){
	$implementing_area = '<li><b> Implementing areas: Zone ‘A’ </b>= ';
	if($min_wage_other_info['implementing_area'] == 2){
		$implementing_area .= '(i) Areas under Municipal Corporations, Municipalities, notified Areas/ Authorities, Development Authorities, Thermal Power Plants Areas including Industrial Township Areas.';
	}else if($min_wage_other_info['implementing_area'] == 3){
		$implementing_area .= '(i) Areas Notified under Kolkata Metropolitan Development Authority(KMDA), (ii) Asansol Municipal Corporation, (iii) Durgapur Municipal Corporation, (iv) Siliguri Municipal Corporation, (v) Digha-Shankarpur Development Authority, (vi) Thermal Power plant areas including Township areas, (vii) All the Municipal areas in different Districts, (viii) All the Sub-Divisions in the districts of Howrah, Hoogly, Burdwan and 24- Parganas (N)[excluding Barasat and Basirhat], (ix) Alipur Sub-Division and Baruipur Sub-Division in the district of 24-Parganas(S), (x) Midnapore Sadar Sub-Division and Kharagpur Sub-Division in the district of West Midnapore, (xi) Siliguri sub-division in the district of Darjeeling, (xii)Bankura Sub-Division and Bisnupur Sub-division in the district of Bankura, and (xiii) Raghunathpur Sub-Division in the district of Purulia.';
	}  
	$implementing_area .= '<br><b>Zone ‘B’</b>= Rest of the West Bengal. </li>';
}else if($min_wage_other_info['implementing_area'] == 1){
	$implementing_area .= '<li>There will only be one rate of minimum wage for the whole of West Bengal.';
}
if($min_wage_other_info['worker_type']== 1){
	$emp_clarification = '<li>The classification of employees in the above employment is as follows :- ';
	if(!empty($min_wage_other_info['unskilled_desc'])){$emp_clarification .= '<br><b>Unskilled Workers: </b>'.$min_wage_other_info['unskilled_desc'].'.';}
	if(!empty($min_wage_other_info['semiskilled_desc'])){$emp_clarification .= '<br><b>Semi-skilled Workers: </b>'.$min_wage_other_info['semiskilled_desc'].'.';}
	if(!empty($min_wage_other_info['skilled_desc'])){$emp_clarification .= '<br><b>Skilled Workers: </b>'.$min_wage_other_info['skilled_desc'].'.';}
	if(!empty($min_wage_other_info['highlyskilled_desc'])){$emp_clarification .= '<br><b>Highly Skilled Workers: </b>'.$min_wage_other_info['highlyskilled_desc'].'.</li>';}
}

if($min_wage_other_info['flip_option']== 1){
	$output .= $emp_clarification.$implementing_area;
}else{
	$output .= $implementing_area.$emp_clarification;
}

/************ Page Break ******************/
if($min_wage_other_info['implementing_area'] == 2 && $min_wage_other_info['worker_type']== ''){
	$p_break_start = '<p style="page-break-after:always">'; 
	$p_break_end = '</p>';
}

if(trim($min_wage_info['employment_id']) == 19){
	$output .= '<li>
					<b>The classification of employees is as follows:-</b><br><h3>CINEMA HOUSE EXHIBITION</h3></br>
					<p><b>Unskilled:</b> Peon, Bearer, Maid-servants, Sweeper, Poster Boys, Durwans, Watchman, Shifter, Dispensers, Bar-attendants, Chocolate Boy, Soda Fountain Hamal, Mali, Traffic Linesman or any other employee engaged in a similar nature of unskilled work by whatever name called. </p>
					<p><b>Semi-skilled:</b> Usher, Doorman, Liftman, Gate-keeper, Fireman, Assistant Painter, Assistant Carpenter, Assistant Polishwala, MCOT (Member of Customer Dealing Team), Ticket Seller, Food/Snacks Seller or any other employee engaged in a similar nature of semi-skilled work by whatever name called.</p>
					<p><b>Skilled:</b> Assistant Engine Driver, Electrician, Operator, Projectionist, Polishwala, Painter, Carpenter, Caretaker, Telephone Operator, Soda Fountain Manager, Bar Manager, Booking &amp; other Clerks, Cashier, Publicity Officer, Engine Driver, 1 st Assistant/A.C. Operator, 1 st Assistant Operator or
any other employee engaged in a similar nature of skilled work by whatever name called.</p>
					<p><b>Highly Skilled:</b> Head Operator, Head A.C. Operator, Head Electrician, Assistant Manager, Supervisor, Accountant or any other employee engaged in a similar nature of highly skilled work by whatever name called.</p>
					<br><h3>DISTRIBUTION UNITS</h3></br>
					<p><b>Unskilled:</b>Peon, Sweeper, Mali, Bearer, Durwan, Watchman or any other employee engaged in a similar nature of unskilled work by whatever name called.</p>
					<p><b>Skilled:</b> Clerks, Accessories Clerks, Shipping, Filing and Mailing Clerks, Representative/Field Inspector, Stenographer, Booker/Salesman, Accountant/Cashier or any other employee engaged in a similar nature of skilled work by whatever name called.</p>
					<p><b>Highly Skilled:</b> Manager or any other employee engaged in a similar nature of highly skilled work by whatever name called.</p>
					<br><h3>PRODUCTION UNITS</h3></br>
					<p><b>Unskilled:</b> Production Boy or any other employee engaged in a similar nature of unskilled work by whatever name called.</p>
					<p><b>Semi-skilled:</b> 2nd Assistant Director and/or Continuity man, 2 nd Assistant Cameraman and/or Focus Puller, Assistant Production Manager, Junior Artist C-Grade or any other employee engaged in a similar nature of semi-skilled work by whatever name called.</p>
					<p><b>Skilled:</b> Clerks, Driver, Junior Artist Special Grade Child/Old, Junior Artist A-Group with costume + dialogue, Junior Artist B-Group (with dialogue), Cat work or any other employee engaged in a similar nature of skilled work by whatever name called.</p>
					<p><b>Highly Skilled:</b> Assistant Cameraman (1 st Asstt.), Assistant Art Director (1 st Asstt.), Stunt Man/ Fighter, Duplicate/Dummy, Effect Man, Dancer A Grade, Dancer B Grade, Dancer C Grade, Asstt. Fight Master, Hair Styler, Costume man, Still Photographer, Make-up Artist or any other employee engaged in a similar nature of highly skilled work by whatever name called.</p>
					<br><h3>STUDIOS</h3></br>					
					<p><b>Unskilled:</b> Office Boy, Durwan, Sweeper and Mali or any other employee engaged in a similar nature of unskilled work by whatever name called.</p>
					<p><b>Semi-skilled:</b> 3rd Assistant Cameraman, Camera Mazdoor, Junior Carpenter, Painter, Setting Mazdoor, Studio Lighting, Electrician, Assistant Make-up-Man, Dresser or any other employee engaged in a similar nature of semi-skilled work by whatever name called.</p>
					<p><b>Skilled:</b> 2 nd Assistant Cameraman and/or Focus Puller, Sound Boom Man, Senior Carpenter, Charge-hand setting Mazdoor, Setting Outdoor Indoor, Setting Construction/Art Setting, Studio Maintenance and Shop, Electrician, Clerks, Accountant/Cashier or any other employee engaged in a similar nature of skilled work by whatever name called.</p>
					<p style="margin-bottom:0px;"><b>Highly Skilled:</b> Assistant Cameraman (1 st Assistant), Assistant Sound Recordist, Modeller/Designer, Title Card Writer, Setting Deptt.-in Charge, Charge-hand Carpenter, Floor Charge hand Lighting Electrician (Head Electrician), Back-ground Painter, Assistant Manager/Laboratory Officer-in-charge or any other employee engaged in a similar nature of highly skilled work by whatever name called.</p>
					<br><h3>LABORATORIES</h3></br>					
					<p><b>Unskilled:</b> Sweeper, Durwan, Watchman, Office Boy or Laboratory Boy, Mali or any other employee engaged in a similar nature of unskilled work by whatever name called.</p>
					<p><b>Semi-skilled:</b> Dark room Mazdoor, Sound Mazdoor or any other employee engaged in a similar nature of semi-skilled work by whatever name called.</p>
					<p><b>Skilled:</b> Dark room Assistant, Grading Assistant, Assistant Projectionist, Assistant Electrician, Film Checking and Clearing-in-charge, 2 nd Assistant Sound Recordist, Chemical Bath-in-charge, Clerk or any other employee engaged in a similar nature of skilled work by whatever name called.</p>
					<p><b>Highly Skilled:</b> Chief Projectionist, Chief Electrician, Maintenance Mechanics, Assistant Sound Recordist, Accountant/Cashier, Assistant Manager/Laboratory Office-in-charge or any other employee engaged in a similar nature of highly skilled work by whatever name called.</p>
					
					
				</li>';
}
if(trim($min_wage_info['employment_id']) == 71){
	$output .= '<b> NOTES :- </b> <br>
						1. To arrive at daily rates of wages, the monthly rates may be divided by 26 (to be rounded off to the nearest rupee).<br>
						2. Wages of full-time piece-rated workers, if there be any, shall in no case be less than the daily rates of minimum wages.<br><br>
						<b>CATEGORIES OF EMLOYEES :</b><br><br>
						
						<b>UNSKILLED/OTHERS &#8217;A&#8217; : </b> General Mazdoor/Cooli/Kamin, Loader, Unloader, Porter, Asstt. Gardener, Tarman, Stageman, Canteen Boy, Car-cleaner.<br>
						<b>SEMI-SKILLED : </b>Crusher Mazdoor, Line Mazdoor, Grinder, Trollyman, Stamper, Checker, Setter, Semi-skilled Mason, Boilerman, Carpenter, Mixing Attendant, Pump Khalashi.<br>
						<b>SKILLED/OTHERS &#8217;B&#8217; :</b> Turner, Fitter, Electrician, Moulder, Fireman, Latheman, Pipe-plant Polisher, Klin Setter, Trindal Press Operator, Mixingman-cum-Operator, Welder, Gas Cutter, Blacksmith, Pattern Maker, Mill Attendant, Packerman.<br>
						<b>HIGHLY SKILLED/OTHERS &#8217;C&#8217; :</b> Head Mason, Head Carpenter, Head Fitter, Trindal, Shift Moulder, Head Welder, Head Turner, Moulder, Supervisor, Mechanical Fitter.<br>
						<b>OTHERS &#8217;A&#8217; :</b> Peon, Durwan, Security Guard.<br>
						<b>OTHERS &#8217;B&#8217; :</b> Junior Clerk, Typist, Compounder, Nurse, Driver, Stock-keeper, Laboratory Asstt.<br>
						<b>OTHERS &#8217;C&#8217; : </b>Senior Clerk, Head Clerk, Accountant, Supervisor, Security Officer, Foreman, Chemist.<br><br><br>
						<b>ADDITIONAL INFORMATION :</b><br><br>
						<b>C.P.I. &amp; BASE YEAR :</b> ASANSOL (BASE 1960 = 100)<br>
						<b>Fixation Point :</b> 1416. <br>
						<b>Average of C.P.I. from Apl..`19 Sept.`19., </b>= 7122 points.<br>
						<b>Rise in Index : </b>5706 points.<br>
						<b>Rate of adjustment of D.A. = </b>Re.1.00 per point per month <br>';
}else if(trim($min_wage_info['employment_id']) == 91){
	$output .= '<br><br><br>
					<b>ADDITIONAL INFORMATION :</b><br><br>
						<table>
							<tr>
								<td><b>Area :</b></td>
								<td>Kolkata &amp; 24-Pgs.</td>
								<td>Howrah and Hooghly</td>
								<td>Other Districts</td>
							</tr>
							<tr>
								<td><b>C.P.I :</b></td>
								<td>(Kolkata)</td>
								<td>(Howrah)</td>
								<td>(Nadia &amp; Murshidaba)</td>
							</tr>
							<tr>
								<td><b>Base Year  :</b></td>
								<td>(1960 = 100)</td>
								<td>(1960 = 100)</td>
								<td>(1951 = 100)</td>
							</tr>
							<tr>
								<td><b>Average of C.P.I. Nos. of the three centres from January 2019 to June 2019 :</b></td>
								<td>6982</td>
								<td>6416</td>
								<td>6888</td>
							</tr>
							<tr>
								<td><b>Fixation Point :</b></td>
								<td>421</td>
								<td>402</td>
								<td>402</td>
							</tr>
							<tr>
								<td><b>Rate of Neutralization per point per day :</b></td>
								<td>P. 3.8</td>
								<td>P. 3.8</td>
								<td>P. 3.8</td>
							</tr>
						</table>
						<p></p>';
}else if(trim($min_wage_info['employment_id']) == 69){
	$output .= '<br>
				<b>ADDITIONAL INFORMATION :</b><br><br>
				<b>C.P.I. No. for Industrial Workers =</b> Kolkata Centre.<br>
				<b>Base Year =</b> 1960=100. <br>
				<b>Half-yearly Average of C.P.I. nos. from July, 2019 to Dec, 2019 </b>= 7080 points.<br>
				<b>Fixation Point = </b>392<br>
				<b>Rate of adjustment of D.A. = </b>Upto 519 points over fixation point.@Re.1.00 per point <br>and above 519 points @Rs.1.20 per point per month. <br>';
}else if(trim($min_wage_info['employment_id']) == 41){
	$output .=' 1. The daily rates of minimum wages of the different categories of employees shall be
				computed by dividing the total monthly minimum wages by twenty six and shall be rounded
				off to the nearest paise.<br>
				2. To get the weekly rates of minimum wages, daily rates of minimum wages shall be multiplied
				by six and rounded off to the nearest paise.
				<br>
				<table width="100%">
				<tr>
					<th align="left">Unskilled</th>
					<th align="left">Semi-skilled</th>
					<th align="left">Skilled</th>
					<th align="left">Highly Skilled</th>
				<tr>
				<tr>
					<td>1. Annealing Chamber Coolies.</td>
					<td>Bubbler/Bubbler Helper.</td>
					<td>Lorry Driver</td>
					<td>Blower</td>
				</tr>
				<tr>
					<td>2. Furnace Coolies.</td>
					<td>Lineman.</td>
					<td>Fireman. </td>
					<td>Foreman</td>
				</tr>
				<tr>
					<td>3. Carrier (Jaliwala) Coolies.</td>
					<td>Airman.</td>
					<td>Flower (Ord)</td>
					<td>Shift-in-Charge</td>
				</tr>
				<tr>
					<td>4. General Coolies.</td>
					<td>Fireman Helper.</td>
					<td>Helper to Blower.</td>
					<td></td>
				</tr>
				<tr>
					<td>5.Poker.</td>
					<td>Meltors.</td>
					<td>Machineman (Cutter)</td>
					<td></td>
				</tr>
				<tr>
					<td>6. Mason (Helper).</td>
					<td>Blacksmith.</td>
					<td>Machineman (Helper)</td>
					<td></td>
				</tr>
				<tr>
					<td>7. Batch Coolies (Batch Room).</td>
					<td>Grinder and Polisher.</td>
					<td>Cutting off Fine Polishing</td>
					<td></td>
				</tr>
				<tr>
					<td>8. Sweeper.</td>
					<td>Cook.</td>
					<td>Machine Operator</td>
					<td></td>
				</tr>
				<tr>
					<td>9. Canteen Boys.</td>
					<td>Sorter.</td>
					<td>Fitter Mechanic</td>
					<td></td>
				</tr>
				<tr>
					<td>10. Chimney Grinder.</td>
					<td>Rulswala (Non-Maker)</td>
					<td>Electrician</td>
					<td></td>
				</tr>
				<tr>
					<td>11. Chimney Grinder.</td>
					<td>-</td>
					<td>Turner</td>
					<td></td>
				</tr>
				<tr>
					<td>12. Lorry Mazdoors.</td>
					<td>-</td>
					<td>Latheman</td>
					<td></td>
				</tr>
				<tr>
					<td>13. -</td>
					<td>-</td>
					<td>Welder</td>
					<td></td>
				</tr>
				</table>
				<br>
				<b>ADDITIONAL INFORMATION :</b><br>
						<b>C.P.I. (BASE 1960 = 100) :</b> KOLKATA <br>
						<b>Fixation Point :</b> 245<br>
						<b>Average of C.P.I. from July, 2019 to Dec, 2019 :</b>7180<br>
						<b>Difference in C.P.I. :</b>6935<br>
						<b>Rate of adjustment of VDA :</b>Re.1.00 per point per month.<br>';
	
}else{	
	
	$output .= '<li> To arrive at daily rates, monthly rates shall have to be divided by 26 (to be rounded off to the nearest rupee) and to arrive at weekly rates, daily rates shall have to be multiplied by 6.</li>
	<li> A normal working day shall consist of eight and half hours of work including interval for half-an-hour for rest. </lip>
	<li> One day in any period of seven days as may suit the local convenience shall be the day of weekly rest. The minimum rates of wages include the wages for weekly day of rest. For payment for work done on the day of weekly rest and for work done beyond the normal working hours shall be double the ordinary rate of wages. </li>
	<li> Where the existing rates of wages of any employee based on contract or agreement or otherwise are higher than the rates notified herein, the higher rates shall be protected and treated as minimum rates of wages applicable for the purpose of this notification to all such employees. </li>
	<li> These minimum rates of wages are applicable to employees employed by contractors also. </li> 
	<li> The  minimum  rates  of  wages  for  disabled  persons  shall  be  same  as  payable  to  the  workers  of  appropriate category. </li>
	<li> The men and women employees shall get the same rates of wages for the same work or work of similar nature. </li>
	<li> The  minimum rates  of  wages  and  Variable  Dearness  Allowance,  if  any,  both together shall constitute  the  minimum rates of wages to be be enforceable under the Minimum Wages Act, 1948(11 of 1948).</li>';
	if(trim($min_wage_info['employment_id'])== 6){
		$output.= '<li>Piece rates of work, if there be any, shall in no case be less than the daily minimum time rates of wages.</li>
				   <li>For each principal meal, if supplied by the employer, the daily rates of minimum wages
shall be reduced by Rs. 5.37.</li>';
	}
	if(trim($min_wage_info['employment_id'])== 1 || trim($min_wage_info['employment_id'])== 72){
		$output.= '<li>Where the practice of payment partly in cash and partly in produce is prevalent, the total value of produce supplied plus the cash wages paid shall not be less than the minimum wages.</li>';
	}else if(trim($min_wage_info['employment_id']) == 13){
		$output.= '<li>The daily earnings of a piece rated employee shall not be less than the rate for a daily rated employee.</li>';
	}
	    // $output.= '<p>This is issued with due approval of the Labour Commissioner, West Bengal. </p>';
	}
 
if(trim($min_wage_info['employment_id']) == 91 || trim($min_wage_info['employment_id']) == 69 || trim($min_wage_info['employment_id']) == 41){
	$output .= '
	<div style="overflow:hidden;margin:50px 0 40px 0;"><div style="float:right;width:40%;text-align:center;">('.$min_wage_info['signature_by_name'].')<br>'.$min_wage_info['signature_by_designation'].'<br>West Bengal</div></div><br>
	<div style="overflow:hidden;margin:0 0 20px 0;"><div style="float:left;width:60%;">MEMO. NO. &emsp;'.$min_wage_info['publish_memo_no1'].$min_wage_other_info['memo_no'].'</div><div style="float:right;width:30%;text-align:right;">Dated:__/__/____ </div></div>
	<div style="overflow:hidden;margin:0 0 6px 0;"><div style="float:left;width:3%;">1.</div><div style="float:right;width:95%;text-align:left;">The Principal Secretary, Labour Department, New Secretariat Buildings, 12th floor, 1, K.S. Roy Road, Kolkata –700 001.</div></div>	
	<div style="overflow:hidden;margin:0 0 6px 0;"><div style="float:left;width:3%;">2.</div><div style="float:right;width:95%;text-align:left;">The Joint/Deputy/Assistant  Labour  Commissioner ______________________ with  the  request  to circulate this among the Inspectors, M.W. under his jurisdiction.</div></div>
	<div style="overflow:hidden;margin:0 0 6px 0;"><div style="float:left;width:3%;">3.</div><div style="float:right;width:95%;text-align:left;">The  Secretary  (Central  Trade  Unions/All  Chambers  of  Commerce) ________________________ with the request to circulate this among his constituents.</div></div>
	
	<div style="overflow:hidden;margin:50px 0 20px 0;"><div style="float:right;width:40%;text-align:center;"><br>Labour Commissioner</div></div>';
}else{
$output .= '
	<div style="overflow:hidden;margin:50px 0 40px 0;"><div style="float:right;width:50%;text-align:center;">('.$min_wage_info['signature_by_name'].')<br>'.$min_wage_info['signature_by_designation'].'<br>West Bengal</div></div><br>
	<div style="overflow:hidden;margin:0 0 20px 0;"><div style="float:left;width:65%;">MEMO. NO.&emsp;'.$min_wage_info['publish_memo_no1'].$min_wage_other_info['memo_no'].'</div><div style="float:right;width:30%;text-align:right;">Dated: __/__/____ </div></div>
	<div style="overflow:hidden;margin:0 0 6px 0;"><div style="float:left;width:3%;">1.</div><div style="float:right;width:95%;text-align:left;">The Principal Secretary, Labour Department, New Secretariat Buildings, 12th floor, 1, K.S. Roy Road, Kolkata –700 001.</div></div>	
	<div style="overflow:hidden;margin:0 0 6px 0;"><div style="float:left;width:3%;">2.</div><div style="float:right;width:95%;text-align:left;">The  Secretary  (Central  Trade  Unions/All  Chambers  of  Commerce) ________________________ with the request to circulate this among his constituents.</div></div>
	<div style="overflow:hidden;margin:0 0 6px 0;"><div style="float:left;width:3%;">3.</div><div style="float:right;width:95%;text-align:left;">The Joint/Deputy/Assistant  Labour  Commissioner ______________________ with  the  request  to circulate this among the Inspectors, M.W. under his jurisdiction.</div></div>
	<div style="overflow:hidden;margin:50px 0 20px 0;"><div style="float:right;width:40%;text-align:center;"><br>Labour Commissioner</div></div>';
	
	/*<div style="overflow:hidden;margin:0 0 6px 0;"><div style="float:left;width:3%;">2.</div><div style="float:right;width:95%;text-align:left;">The  Labour Commissioner,  W.B.,  N.S.  Bldgs.,  11th floor,  Kol.-1. </div></div>*/
}
	return pdf_using_mpdf_api($output, $name_of_employment);
}
function minimum_wage_pdf_pk($employment_id, $year, $session){
	
	//$current_month =   12;  // date('n',time());7;
	//$current_year =  2020;//date('Y',time()); //2019;2019;
	/*echo 'Payal<br/>';
	echo strtotime('2020-09-01').'--September<br/>';
	echo strtotime('2021-04-30').'--April<br/>';
	echo date('Y-m-d',1598986513).'--<br/>';
	echo '291--'.date('Y-m-d',1620361733).'--<br/>';
	echo '297--'.date('Y-m-d',1619074297);
	exit;*/
	
	$signature_by_name = 'Sharmila Khatua';
	$signature_by_designation = 'Additional Labour Commissioner';	
	$publish_time = 1643653800;
				
	$min_wage_info = db_select('l_min_wages_documents','l');
	$min_wage_info->leftJoin('l_min_wages_schedule_employment_master', 'ma','ma.id=l.employment_id ');
	$min_wage_info->fields('l', array());
	$min_wage_info->fields('ma', array());
	$min_wage_info->condition('ma.id',$employment_id);
	$min_wage_info->condition('l.year',$year);
	$min_wage_info->condition('l.session',$session);
	$min_wage_data = $min_wage_info->execute();
	$minWageData = $min_wage_info->execute()->fetchAssoc();
	
	
	$getData = db_select('l_min_wages_schedule_employment_master','ma')->fields('ma', array())->condition('ma.id',$employment_id)->execute()->fetchAssoc();
	/*if($minWageData['employment_id'] == 71){
	print_r($getData);exit;
	}*/
	
	$tableNumber = 'TABLE '.$getData['table_category']; 
	$getMasterData = db_select('l_min_wages_master','l')-> fields('l', array())-> condition('l.is_active',1)-> condition('l.table_number',$tableNumber)->execute()->fetchAssoc();
	
	//if(($current_month >= 1) && ($current_month <= 6)){
	$current_year =  date('Y',time());	
	if($session == '2'){
		/*** MNW for July Session (Previous(Oct,Nov,Dec),Jan,Feb,Mar) --------- Therefore MNW should generate from March to June ***/
		$month_sl_1 = array(1,2,3);
		$month_sl_2 = array(10,11,12);
		$month = 'July';
		
		$db_or = db_or();
		$db_or->condition(
					db_and()->condition('l.cpiyear',$year-1)->condition('l.month_srl', ($month_sl_2), 'IN')
				);
		$db_or->condition(
					db_and()->condition('l.cpiyear',$year)->condition('l.month_srl', ($month_sl_1), 'IN')
				);
		
		$cpi_value_pre_Oct_Dec = db_select('l_mnw_cpi_number_master','l');
		$cpi_value_pre_Oct_Dec->fields('l', array());
		$cpi_value_pre_Oct_Dec->condition('l.is_active',1);
		$cpi_value_pre_Oct_Dec->condition($db_or);
		$cpi_value_pre_Oct_Dec = $cpi_value_pre_Oct_Dec->execute();
		
		foreach ($cpi_value_pre_Oct_Dec as $data) {
			$sum_cpi_zone_A = $sum_cpi_zone_A + $data->iwvalue;
			$sum_cpi_zone_B = $sum_cpi_zone_B + $data->alvalue;
		}
		/*if($minWageData['employment_id'] == 71){
		echo $sum_cpi_zone_A.'--'.$sum_cpi_zone_B;exit; //71-1862---5450
		}*/
		
	}else{ 
		/*** MNW for January Session (Apr, May, June, July, Aug, Sept)  --------- Therefore MNW should generate from July to Dec ***/
		$session = '1';
		$month = 'January';
		//2020<2021
		//echo $year.'--'.$current_year.'<br/>';
		if($year==$current_year){ //echo '111'; exit;//2021=2021
			$year1 = $year - 1; 
		}else if($year<$current_year){ //echo '222'; exit;//2019<2021
			$year1 = $year - 1;
		}else if($year>$current_year){ //echo '333'; exit;// 2022>2021
			$year1 = $year - 1;
		}
		 //echo $year1.'<br/>';
		
		$month_sl = array(4,5,6,7,8,9);
		
		$cpi_value_Apr_Sept_Zone_A = db_select('l_mnw_cpi_number_master','l');
		$cpi_value_Apr_Sept_Zone_A->fields('l', array());
		$cpi_value_Apr_Sept_Zone_A->condition('l.is_active',1);
		$cpi_value_Apr_Sept_Zone_A->condition('l.cpiyear',$year1);
		$cpi_value_Apr_Sept_Zone_A = $cpi_value_Apr_Sept_Zone_A->condition('l.month_srl', ($month_sl), 'IN')->execute()->fetchAll(); 
		
		foreach ($cpi_value_Apr_Sept_Zone_A as $data) {
			$sum_cpi_zone_A = $sum_cpi_zone_A + $data->iwvalue;
			$sum_cpi_zone_B = $sum_cpi_zone_B + $data->alvalue; //echo $sum_cpi_zone_A.'---'.$sum_cpi_zone_B;exit;
		}
	}
	/*** Avg CPI of 6 months ***/ 	
	$avg_cpi_value_Zone_A = $sum_cpi_zone_A / 6; 
	$avg_cpi_value_Zone_B = $sum_cpi_zone_B / 6; 
	
	/**** FIxation Point ****/
	$fixation_value_IW_Zone_A = $getMasterData['fixation_point_zonea'];
	$fixation_value_IW_Zone_B = $getMasterData['fixation_point_zoneb'];
	
	/**** Rise in CPI ****/
	$rise_cpi_zone_A = $avg_cpi_value_Zone_A - $fixation_value_IW_Zone_A; 
	$rise_cpi_zone_B = $avg_cpi_value_Zone_B - $fixation_value_IW_Zone_B;
	
	/**** Initial Fixed Value ****/
		
	$initial_fixed_minwage_unskill_zoneA = $getMasterData['unskilled_initial_rate_zonea'];
	$initial_fixed_minwage_semiskill_zoneA = $getMasterData['semiskilled_initial_rate_zonea'];
	$initial_fixed_minwage_skill_zoneA = $getMasterData['skilled_initial_rate_zonea'];
	$initial_fixed_minwage_highskill_zoneA = $getMasterData['highskilled_initial_rate_zonea'];
	
	$initial_fixed_minwage_unskill_zoneB = $getMasterData['unskilled_initial_rate_zoneb'];
	$initial_fixed_minwage_semiskill_zoneB = $getMasterData['semiskilled_initial_rate_zoneb'];
	$initial_fixed_minwage_skill_zoneB = $getMasterData['skilled_initial_rate_zoneb'];
	$initial_fixed_minwage_highskill_zoneB = $getMasterData['highskilled_initial_rate_zoneb'];
	
	/**** Neutralization Point ****/
	$neutralization_point_unskill_zoneA = $getMasterData['cpi_unskilled_rate_zonea'];
	$neutralization_point_semiskill_zoneA = $getMasterData['cpi_semiskilled_rate_zonea'];
	$neutralization_point_skill_zoneA = $getMasterData['cpi_skilled_rate_zonea'];
	$neutralization_point_highskill_zoneA = $getMasterData['cpi_highskilled_rate_zonea'];
	
	$neutralization_point_unskill_zoneB = $getMasterData['cpi_unskilled_rate_zoneb'];
	$neutralization_point_semiskill_zoneB = $getMasterData['cpi_semiskilled_rate_zoneb'];
	$neutralization_point_skill_zoneB = $getMasterData['cpi_skilled_rate_zoneb'];
	$neutralization_point_highskill_zoneB = $getMasterData['cpi_highskilled_rate_zoneb'];
	
	/*** Minimum Wage Worker & Zone Wise ***/
	if($year == 2022 && $session==1){
		$unskilled_wage_A = 8904; //($initial_fixed_minwage_unskill_zoneA + ($neutralization_point_unskill_zoneA*$rise_cpi_zone_A));	
		$semiskilled_wage_A = 9795; //($initial_fixed_minwage_semiskill_zoneA + ($neutralization_point_semiskill_zoneA*$rise_cpi_zone_A)); 
		$skilled_wage_A = 10775; //($initial_fixed_minwage_skill_zoneA + ($neutralization_point_skill_zoneA*$rise_cpi_zone_A)); 
		$highskilled_wage_A = 11852; //($initial_fixed_minwage_highskill_zoneA + ($neutralization_point_highskill_zoneA*$rise_cpi_zone_A)); 
	}else{
		$unskilled_wage_A = ($initial_fixed_minwage_unskill_zoneA + ($neutralization_point_unskill_zoneA*$rise_cpi_zone_A));	
		$semiskilled_wage_A = ($initial_fixed_minwage_semiskill_zoneA + ($neutralization_point_semiskill_zoneA*$rise_cpi_zone_A)); 
		$skilled_wage_A = ($initial_fixed_minwage_skill_zoneA + ($neutralization_point_skill_zoneA*$rise_cpi_zone_A)); 
		$highskilled_wage_A = ($initial_fixed_minwage_highskill_zoneA + ($neutralization_point_highskill_zoneA*$rise_cpi_zone_A)); 
	}

	$unskilled_wage_B = ($initial_fixed_minwage_unskill_zoneB + ($neutralization_point_unskill_zoneB*$rise_cpi_zone_B));
	$semiskilled_wage_B = ($initial_fixed_minwage_semiskill_zoneB + ($neutralization_point_semiskill_zoneB*$rise_cpi_zone_B)); 
	$skilled_wage_B = ($initial_fixed_minwage_skill_zoneB + ($neutralization_point_skill_zoneB*$rise_cpi_zone_B)); 
	$highskilled_wage_B = ($initial_fixed_minwage_highskill_zoneB + ($neutralization_point_highskill_zoneB*$rise_cpi_zone_B));  
	
	
	$avCpi_ZoneA             = array(1,3,6,13,18,34,38,43,52,58,72,78,92);
	$avCpi_ZoneB             = array();
	
	$unskilled_rate_zone_a 	 = array(1,3,6,13,18,34,38,43,52,58,72,76,78,92);
	$unskilled_rate_zone_b 	 = array(35,76,85,16);
	$semiskilled_rate_zone_a = array(1,2,3,5,6,12,13,18,34,35,38,42,43,47,49,51,52,53,55,58,72,73,76,78,83,85,92);
	$semiskilled_rate_zone_b = array(2,3,5,12,16,18,34,35,38,42,43,47,49,51,53,55,58,73,76,83,85,92);
	$skilled_rate_zone_a 	 = array(1,2,3,5,6,12,13,18,34,35,38,42,43,47,49,51,52,53,55,58,72,73,76,78,83,85,92);
	$skilled_rate_zone_b 	 = array(2,3,5,12,16,18,34,35,38,42,43,47,49,51,53,55,58,73,76,83,85,92);
	$highskilled_rate_zone_a = array(1,2,3,5,6,7,8,10,12,13,14,15,18,29,30,34,35,38,39,42,43,44,47,48,49,51,52,53,55,58,60,62,63,64,65,66,67,72,73,74,75,76,78,79,80,81,83,84,85,86,87,88,89,92);
	$highskilled_rate_zone_b = array(2,3,5,6,7,8,10,12,13,14,15,16,18,29,30,34,35,38,39,42,43,44,47,48,49,51,52,53,55,58,60,62,63,64,65,66,67,73,74,75,76,79,80,81,83,84,85,86,87,88,89,92);
	
	$unskilledWageValueA = $unskilledWageValueB = $semiskilledWageValueA = $semiskilledWageValueB = $skilledWageValueA = $skilledWageValueB = $highskilledWageValueA = $highskilledWageValueB = $avCpi_ZoneA_value=0;
	
	if (in_array($employment_id, $avCpi_ZoneA)) {
		$avCpi_ZoneA_value = 0;
	}else{
		$avCpi_ZoneA_value = $avg_cpi_value_Zone_A;
	}
	
	if (in_array($employment_id, $unskilled_rate_zone_a)) {
		$unskilledWageValueA = 0;
	}else{
		$unskilledWageValueA = $unskilled_wage_A;
	}
	
	if (in_array($employment_id, $unskilled_rate_zone_b)) {
		$unskilledWageValueB = 0;
	}else{
		$unskilledWageValueB = $unskilled_wage_B;
	}
	
	if (in_array($employment_id, $semiskilled_rate_zone_a)) {
		$semiskilledWageValueA = 0;
	}else{
		$semiskilledWageValueA = $semiskilled_wage_A;
	}
	
	if (in_array($employment_id, $semiskilled_rate_zone_b)) {
		$semiskilledWageValueB = 0;
	}else{
		$semiskilledWageValueB = $semiskilled_wage_B;
	}
	
	if (in_array($employment_id, $skilled_rate_zone_a)) {
		$skilledWageValueA = 0;
	}else{
		$skilledWageValueA = $skilled_wage_A;
	}
	
	if (in_array($employment_id, $skilled_rate_zone_b)) {
		$skilledWageValueB = 0;
	}else{
		$skilledWageValueB = $skilled_wage_B;
	}
	
	if (in_array($employment_id, $highskilled_rate_zone_a)) {
		$highskilledWageValueA = 0;
	}else{
		$highskilledWageValueA = $highskilled_wage_A;
	}
	
	if (in_array($employment_id, $highskilled_rate_zone_b)) {
		$highskilledWageValueB = 0;
	}else{
		$highskilledWageValueB = $highskilled_wage_B;
	}
	
	$updatedArray = array(
					'unskilled_rate_zone_a' => $unskilledWageValueA,
					'unskilled_rate_zone_b' => $unskilledWageValueB,
					'semiskilled_rate_zone_a' => $semiskilledWageValueA,
					'semiskilled_rate_zone_b' => $semiskilledWageValueB,
					'skilled_rate_zone_a' => $skilledWageValueA,
					'skilled_rate_zone_b' => $skilledWageValueB,
					'highskilled_rate_zone_a'=>$highskilledWageValueA,
					'highskilled_rate_zone_b' => $highskilledWageValueB,
					'cpi_value_zone_a' => $avCpi_ZoneA_value,
					'cpi_value_zone_b' => $avg_cpi_value_Zone_B,
					'employment_id' => $employment_id,
					'session' => $session,
					'signature_by_name' => $signature_by_name,
					'signature_by_designation' => $signature_by_designation,
					'publish_time' => $publish_time
				);	
	/*if($minWageData['employment_id'] == 71){
		print_r($updatedArray);	exit;		
		}*/
	
	if($min_wage_data->rowCount()>0){ //echo 777;exit;
		
		$update_status  =  db_update('l_min_wages_documents');
		$update_status->fields($updatedArray);
		$update_status->condition('year',$year);
		$update_status->condition('session',$session);
		$update_status->condition('employment_id',$employment_id);
		$update_status->condition('is_active',1);
		$update_status->execute();
		
	}else{ 
		//echo 888;exit;
		$insertedArray = array(
							'node_type_name' => $getData['node_type_name'],
							'month' => $month,
							'node_master_id' =>$getData['node_master_id'],
							'is_active' => 1,
							'monthint' => 1,
							'unskilled_rate_zone_a' => $unskilledWageValueA,
							'unskilled_rate_zone_b' => $unskilledWageValueB,
							'semiskilled_rate_zone_a' => $semiskilledWageValueA,
							'semiskilled_rate_zone_b' =>$semiskilledWageValueB,
							'skilled_rate_zone_a' => $skilledWageValueA,
							'skilled_rate_zone_b'=> $skilledWageValueB,
							'highskilled_rate_zone_a'=>$highskilledWageValueA,
							'highskilled_rate_zone_b'=> $highskilledWageValueB,
							'cpi_value_zone_a'=> $avCpi_ZoneA_value,
							'cpi_value_zone_b'=> $avg_cpi_value_Zone_B,
							'year' =>$year,
							'created_date'=> time(),
							'employment_id'=> $employment_id,
							'session'=> $session,
							'signature_by_name' => $signature_by_name,
							'signature_by_designation' => $signature_by_designation,
							'publish_time' => $publish_time
						);
		
		
		db_insert('l_min_wages_documents')->fields($insertedArray)->execute();
	}
	
	drupal_goto('generated-minimum_wage_pdf/'.$employment_id.'/'.$year.'/'.$session);
}
function generate_minimum_wage_payal($employment_id, $year, $session){
	
	//echo $employment_id.'--'.$year.'--'.$session.'--payal';exit;
	$min_wage_info = db_select('l_min_wages_documents','l');
	$min_wage_info->leftJoin('l_min_wages_schedule_employment_master', 'ma','ma.node_master_id=l.node_master_id ');
	$min_wage_info->fields('l', array());
	$min_wage_info->fields('ma', array('id','notification_number','notification_date','notification_published_dt','type_of_sch_emp','implementing_area','worker_type','unskilled_desc','semiskilled_desc','skilled_desc','highlyskilled_desc','circular_no','flip_option','table_category','memo_no'));
	$min_wage_info->condition('l.employment_id',$employment_id);
	$min_wage_info->condition('l.year',$year);
	$min_wage_info->condition('l.session',$session);
	$minWagesMaster = $min_wage_info->execute()->fetchAssoc(); 
			
	$min_wage_info = db_select('l_min_wages_schedule_employment_details_info_dev','l');
	$min_wage_info->fields('l', array());
	$min_wage_info->condition('l.employment_id',$employment_id);
	$min_wage_info->condition('l.year',$year);
	$min_wage_info->condition('l.session',$session);
	$min_wage_info = $min_wage_info->execute()->fetchAssoc();
	 
		
	$fix_al_value = db_select('l_mnw_fixation_neutralization_initial_fixed_point_master','l');
	$fix_al_value->fields('l', array('fix_al_value'));
	$fix_al_value->condition('l.is_active',1);
	$fix_al_value = $fix_al_value->execute()->fetchField(); 
	
	$name_of_employment = $minWagesMaster['node_type_name'];
	$notification_number = $minWagesMaster['notification_number']; 
	$notification_date = (!empty($minWagesMaster['notification_date'])?date("d.m.Y",$minWagesMaster['notification_date']):'');
	$notification_published_dt =  date("d.m.Y",$minWagesMaster['notification_published_dt']);
	$type_of_sch_emp = $minWagesMaster['type_of_sch_emp'];
	 
	if($minWagesMaster['session']=='1'){
		$period = '1st January, '.$minWagesMaster['year'].' to 30th June, '.$minWagesMaster['year'];
	}else if($minWagesMaster['session']=='3'){
		$period = 'September, '.($minWagesMaster['year']-1).' to February, '.$minWagesMaster['year'];
	}else if($minWagesMaster['session']=='4'){
		$period = 'March, '.($minWagesMaster['year']).' to August, '.$minWagesMaster['year'];
	}else{
		$period = '1st July,'.$minWagesMaster['year'].' to 31st Dec, '.$minWagesMaster['year'];
	}
	
	if(trim($employment_id) == 91){
		$notification_date = '3.7.1972 / 22.7.1982';
	}
	
	if($minWagesMaster['publish_memo_no'] == 0){
		$publish_memo_no = '';
	}else{
		$publish_memo_no = $minWagesMaster['publish_memo_no'];
	}
	$signature_by_location = 'Statistics Section, Kolkata';
	
	$output='<style type="text/css">
	.pdf-circular-table td,th,caption{background-color:#fff; padding: 5px 10px;}
	.pdf-circular-table { background-color:#333; }
	.pdf-circular-table th { font-size:14px;font-weight:700; }
	.pdf-circular-table td { font-size:12px; font-weight:300; }
	</style>'; 
	
	$img = $GLOBALS['base_url'].'/sites/all/themes/lcTheme/images/Gov-logo-sm.png';
	
	$output .= '<div style="text-align:center;"><img src="'.$img.'" alt=""><h5 style="font-size:14px;font-weight:bold;margin:4px 0 20px 0;">Government of West Bengal<br> Office of the Labour Commissioner<br> 6, Church Lane, 3rd floor, Kolkata –700 001.</h5></div>
	<div style="overflow:hidden;margin:0 0 20px 0;"><div style="float:left;width:50%;">No. &emsp;'.$publish_memo_no.$minWagesMaster['circular_no'].'</div><div style="float:right;width:40%;text-align:right;">Dated: __/__/____ </div></div>
	<div style="text-align:center;font-size:16px;font-weight:bold;margin:0 0 15px 0;">CIRCULAR</div>';
	if(trim($employment_id) == 71){
		$output .= '<div style="font-size:15px;font-weight:700; text-align:justify">The Government of West Bengal in the Labour Department has fixed minimum rates of wages in the employment of  <b>'.$name_of_employment.' </b>in the state of West Bengal covered as per notification number '.$notification_number.' dated '.$notification_date;
	}else{
		$output .= '<div style="font-size:13px;font-weight:700; text-align:justify">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The minimum rates of wages for those in the employment of  <b>'.$name_of_employment.' </b>in the state of West Bengal covered as per notification number '.$notification_number.' dated '.$notification_date.' are being hereby fixed as under';
	}
	
	if(!empty($minWagesMaster['notification_published_dt'])){ 
		$output .= ',as published in the Kolkata Gazette Extraordinary dated '.$notification_published_dt.'.';
	}else{$output .= '.';}
	if($type_of_sch_emp == '2' || $type_of_sch_emp == '4' || trim($employment_id) == '69'){
		$output .= 'These rates will be effective for the period from <b>'.$period.'</b>.';
	}
	
	if($year == 2022 && $session==1 && $minWagesMaster['table_category'] != '3'){
	$output .= '<div style="font-size:13px;font-weight:700; text-align:justify;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Though there is a decrease in the average consumer price index for Industrial Workers for Kolkata centre for the period from April 2021 to September 2021, there shall be, as per Labour Department Notification No Labr/79/(LC/IR)/ 25034/3/2021 dated 24.01.2022 no increase in Variable Dearness Allowance for the Period from 01.01.2022 to 30.06.2022 for Zone A.';
	}
	
	$output .= '<ol type="a" style="line-height:20px text-align:justify">';
	// if($type_of_sch_emp == '1' || $type_of_sch_emp == '3'){
	if($year == 2022 && $session==1 && $minWagesMaster['table_category'] != '3'){
		$output .= '<li> Now, with reference to the said notifications, the following shall be the minimum rates of wages in the said employment for the period from <b>'.$period.'</b>.</li>';
	}else{
		$output .= '<li> Now, with reference to the said notification, the following shall be the minimum rates of wages in the said employment for the period from <b>'.$period.'</b>.</li>';
	}
	$output .= '</div>';
	// }
	
	
	$output .= '<div style="margin:20px 0;">
					<table width="100%" cellspacing="1" class="pdf-circular-table">
  						<tr>'; 
	if($type_of_sch_emp == '1' || $type_of_sch_emp == '4'){
		 $rowspan = '2';$colspan = '2';
	}else if(trim($employment_id) == 1){
		 $rowspan = '3'; $colspan = '3'; 
	// }else if(trim($employment_id) == 75){
		 // $rowspan = '2'; $colspan = '9'; 
	}else{
		 $rowspan = '';$colspan = '';
	}
	if(trim($employment_id) == 91){
		$output .= '<th align="center" valign="top" width="">REGION</th>
					<th align="center">RATE PER 1000 BIDIS ROLLED (Rs.)</th>
					<th align="center">RATE PER DAY (Rs.)</th>
					<th align="center">RATE PER MONTH (Rs.)</th>
				</tr>
				<tr>
					<td align="center" valign="top">Calcutta, North 24 Pgs. &amp; South 24 Pgs.</td>
					<td align="center" valign="top">266.06</td>
					<td align="center" valign="top">264.96</td>
					<td align="center" valign="top">6888.96</td>
				</tr>
				<tr> 
					<td align="center" valign="top">Howrah &amp; Hooghly</td>
					<td align="center" valign="top">242.88</td>
					<td align="center" valign="top">241.93</td>
					<td align="center" valign="top">6290.23</td>
				</tr>
				<tr>
					<td align="center" valign="top">Other Districts</td>
					<td align="center" valign="top">259.82</td>
					<td align="center" valign="top">258.87</td>
					<td align="center" valign="top">6730.62</td>
				</tr>';
	}else{
		$output .= '<th rowspan="'.$rowspan.'" align="center" valign="top" width="">Categories of Employees</th>
					<th colspan="'.$colspan.'" align="center">Monthly Minimum Rates of Wages(Rs.)</th>
				</tr>';
	}
	
	/*************** AGRICULTURE ********************/				 
	if(trim($employment_id) == 1){				 
  		$output .= '<tr>
						<td rowspan="2" align="center" >Per Month (Without food)(Rs.)</td>
						<td colspan="2" align="center">Per Day(Rs.)</td>
					</tr>
					<tr>
						<td>Without food</td>
						<td>With food</td>
					</tr>
					<tr>
						<td>Unskilled</td>
						<td>'.number_format(round($minWagesMaster['unskilled_rate_zone_b']),2).'</td>
						<td>'.number_format(round($minWagesMaster['unskilled_rate_zone_b']/26),2).'</td>
						<td>'.number_format(round($minWagesMaster['unskilled_rate_zone_b']/26)-17,2).'</td>
					</tr>
					<tr>
						<td>Semi-skilled</td>
						<td>'.number_format(round($minWagesMaster['semiskilled_rate_zone_b']),2).'</td>
						<td>'.number_format(round($minWagesMaster['semiskilled_rate_zone_b']/26),2).'</td>
						<td>'.number_format(round($minWagesMaster['semiskilled_rate_zone_b']/26)-17,2).'</td>
					</tr>
					<tr>
						<td>Skilled</td>
						<td>'.number_format(round($minWagesMaster['skilled_rate_zone_b']),2).'</td>
						<td>'.number_format(round($minWagesMaster['skilled_rate_zone_b']/26),2).'</td>
						<td>'.number_format(round($minWagesMaster['skilled_rate_zone_b']/26)-17,2).'</td>
					</tr>';
					
	/*************** BEEDI LEAF PLUCKING ********************/
		
	}else if(trim($employment_id) == 6){	
				 
  		$output .= '<tr><td td align="left">1. Full-time workers <br>(a) Unskilled <br> (b) Semi- skilled <br> (c) Skilled</td>
						<td align="left">'.number_format(round($minWagesMaster['unskilled_rate_zone_b']),2).' (Daily - '.number_format(round($minWagesMaster['unskilled_rate_zone_b']/26),2).')
						<br>'.number_format(round($minWagesMaster['semiskilled_rate_zone_b']),2).' (Daily - '.number_format(round($minWagesMaster['semiskilled_rate_zone_b']/26),2).')
						<br>'.number_format(round($minWagesMaster['skilled_rate_zone_b']),2).' (Daily - '.number_format(round($minWagesMaster['skilled_rate_zone_b']/26),2).')
						</td>
					</tr>
					<tr><td>2. Part-time workers<br>(a) Munshi <br> (b) Moto/Paltai Labour <br> (c) Others</td>
						<td align="left">
							'.number_format(round($minWagesMaster['unskilled_rate_zone_b']/2),2).' (Daily - '.number_format(round(($minWagesMaster['unskilled_rate_zone_b']/2)/26),2).')
							<br>'.number_format(round($minWagesMaster['unskilled_rate_zone_b']/3),2).' (Daily - '.number_format(round(($minWagesMaster['unskilled_rate_zone_b']/3)/26),2).')
							<br>'.number_format(round($minWagesMaster['unskilled_rate_zone_b']/2),2).' (Daily - '.number_format(round(($minWagesMaster['unskilled_rate_zone_b']/2)/26),2).')
						</td>
					</tr>
					<tr><td>3. Piece-rated workers<br>(a) Plucker <br> (b) Processor <br> (c) Packer <br> (d) Packing Coolie</td>
						<td align="left">'.number_format((15*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value,2).' (per 100 leaves)
							<br>'.number_format((12*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value,2).' (per 3 kg. bundle Rs. 12.00) 
							<br>'.number_format((6*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value,2).' (per 50 kg. bag Rs. 6.00)
							<br>'.number_format((6*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value,2).' (per 50 kg. bag Rs. 6.00)
						</td>
					</tr>';
	/******************* CINCHONA PLANTATION  & FISHERY & TOBACCO GODOWN********************/				
	}else if(trim($employment_id) == 18 || trim($employment_id) == 34 ||trim($employment_id) == 38 ||trim($employment_id) == 92 || trim($employment_id) == 58){
		$output .= '<tr>
						<td align="center">Adult Worker</td>
						<td align="center">'.number_format(round($minWagesMaster['unskilled_rate_zone_b']),2).'<br>
						(Daily - '.number_format(round($minWagesMaster['unskilled_rate_zone_b']/26),2).')</td>
					</tr>';
	}else if(trim($employment_id) == 43){
		$output .= '<tr>
						<td align="center">Unskilled</td>
						<td align="center">'.number_format(round($minWagesMaster['unskilled_rate_zone_b']),2).'<br>
						(Daily - '.number_format(round($minWagesMaster['unskilled_rate_zone_b']/26),2).')</td>
					</tr>';
	}else if(trim($employment_id) == 3){
		$output .= '<tr>
						<td align="center" >Per Month(Rs.)</td>
						<td align="center">Per Day(Rs.)</td>
					</tr>
					<tr>
						<td align="center">Unskilled</td>
						<td align="center">'.number_format(round($minWagesMaster['unskilled_rate_zone_b']),2).'</td>
						<td align="center">'.number_format(round($minWagesMaster['unskilled_rate_zone_b']/26),2).'</td>
					</tr>';
	}else if(trim($employment_id) == 13){
		$output .= '<tr>
						<td align="center" >Without Accommodation(Rs.)</td>
						<td align="center">With Accommodation(Rs.)</td>
					</tr>
					<tr>
						<td align="center">Unskilled</td>
						<td align="center">'.number_format(round($minWagesMaster['unskilled_rate_zone_b']),2).'<br>(Daily - '.number_format(round($minWagesMaster['unskilled_rate_zone_b']/26),2).')</td>
						<td align="center">'.number_format(round($minWagesMaster['unskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($minWagesMaster['unskilled_rate_zone_a']/26),2).')</td>
					</tr>
					<tr>
						<td align="center">Semi-Skilled</td>
						<td align="center">'.number_format(round($minWagesMaster['semiskilled_rate_zone_b']),2).'<br>(Daily - '.number_format(round($minWagesMaster['semiskilled_rate_zone_b']/26),2).')</td>
						<td align="center">'.number_format(round($minWagesMaster['semiskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($minWagesMaster['semiskilled_rate_zone_a']/26),2).')</td>
					</tr>
					<tr>
						<td align="center">Skilled</td>
						<td align="center">'.number_format(round($minWagesMaster['skilled_rate_zone_b']),2).'<br>(Daily - '.number_format(round($minWagesMaster['skilled_rate_zone_b']/26),2).')</td>
						<td align="center">'.number_format(round($minWagesMaster['skilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($minWagesMaster['skilled_rate_zone_a']/26),2).')</td>
					</tr>';
	}/*else if(trim($employment_id) == 71){
		$output .= '<tr>
						<td >Unskilled / Others &#8217; A &#8217;</td>
						<td align="center">'.number_format(round($minWagesMaster['unskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($minWagesMaster['unskilled_rate_zone_a']/26),2).')</td>
					</tr>
					<tr>
						<td >Semi-Skilled </td>
						<td align="center">'.number_format(round($minWagesMaster['semiskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($minWagesMaster['semiskilled_rate_zone_a']/26),2).')</td>
					</tr>
					<tr>
						<td>Skilled / Others &#8217; B &#8217;</td>
						<td align="center">'.number_format(round($minWagesMaster['skilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($minWagesMaster['skilled_rate_zone_a']/26),2).')</td>
					</tr>
					<tr>
						<td>Highly Skilled / Others &#8217; C &#8217;</td>
						<td align="center">'.number_format(round($minWagesMaster['highskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($minWagesMaster['highskilled_rate_zone_a']/26),2).')</td>
					</tr>
					';
    }*/else if(trim($employment_id) == 52 || trim($employment_id) == 72 || trim($employment_id) == 78){
		$output .= '<tr>
						<td >Unskilled</td>
						<td align="center">'.number_format(round($minWagesMaster['unskilled_rate_zone_b']),2).'<br>(Daily - '.number_format(round($minWagesMaster['unskilled_rate_zone_b']/26),2).')</td>
					</tr>
					<tr>
						<td >Semi-Skilled </td>
						<td align="center">'.number_format(round($minWagesMaster['semiskilled_rate_zone_b']),2).'<br>(Daily - '.number_format(round($minWagesMaster['semiskilled_rate_zone_b']/26),2).')</td>
					</tr>
					<tr>
						<td>Skilled</td>
						<td align="center">'.number_format(round($minWagesMaster['skilled_rate_zone_b']),2).'<br>(Daily - '.number_format(round($minWagesMaster['skilled_rate_zone_b']/26),2).')</td>
					</tr>';	
    if(trim($employment_id) == 72 || trim($employment_id) == 78){		
		$output .= '<tr>
						<td>Highly Skilled</td>
						<td align="center">'.number_format(round($minWagesMaster['highskilled_rate_zone_b']),2).'<br>(Daily - '.number_format(round($minWagesMaster['highskilled_rate_zone_b']/26),2).')</td>
					</tr>';
	}
	/*}else if(trim($employment_id) == 75){
		$output .= '<tr>
						<td align="center" >Calcutta, 24 Pgs (N) &amp; 24 Pgs (S)</td>
						<td align="center">Howrah &amp; Hooghly</td>
						<td align="center">Burdwan</td>
						<td align="center">Darjeeling, except Siliguri Sub-Division.</td>
						<td align="center">Jalpaigur, Coochbehar &amp; Siliguri Sub-Division.</td>
						<td align="center">Bankura, Midnapore &amp; Purulia</td>
						<td align="center">Birbhum</td>
						<td align="center">Nadia &amp; Murshidabad</td>
						<td align="center">Malda, Uttar Dinajpur &amp; Dakshin Dinajpur</td>
					</tr>
					<tr>
						<td align="left"><br>Unskilled / Peon <br></td>
						<td align="center">6896.00</td>
						<td align="center">6335.00</td>
						<td align="center">6851.00</td>
						<td align="center">5138.00</td>
						<td align="center">4883.00</td>
						<td align="center">6829.00</td>
						<td align="center">6773.00</td>
						<td align="center">6828.00</td>
						<td align="center">6540.00</td>
					</tr>
					<tr>
						<td align="left"><br>Semi-skilled / Durwan <br></td>
						<td align="center">6907.00</td>
						<td align="center">6346.00</td>
						<td align="center">6862.00</td>
						<td align="center">5149.00</td>
						<td align="center">4894.00</td>
						<td align="center">6840.00</td>
						<td align="center">6784.00</td>
						<td align="center">6839.00</td>
						<td align="center">6551.00</td>
					</tr>
					<tr>
						<td align="left"><br>Skilled / Clerks <br></td>
						<td align="center">6933.00</td>
						<td align="center">6372.00</td>
						<td align="center">6888.00</td>
						<td align="center">5175.00</td>
						<td align="center">4920.00</td>
						<td align="center">6866.00</td>
						<td align="center">6810.00</td>
						<td align="center">6865.00</td>
						<td align="center">6577.00</td>
					</tr>	';
	*/}else if(trim($employment_id) == 69){
		$output .= '<tr>
						<td align="center" >CLEANERS &amp; LORRY COOLIES</td>
						<td align="center">'.number_format(round($min_wage_info['unskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($min_wage_info['unskilled_rate_zone_a']/26),2).')</td>
					</tr>
					<tr>
						<td align="center">CONDUCTORS</td>
						<td align="center">'.number_format(round($min_wage_info['semiskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($min_wage_info['semiskilled_rate_zone_a']/26),2).')</td>
					</tr>
					<tr>
						<td align="center">FITTERS &amp; TIME-KEEPERS</td>
						<td align="center">'.number_format(round($min_wage_info['skilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($min_wage_info['skilled_rate_zone_a']/26),2).')</td>
					</tr>
					<tr>
						<td align="center">DRIVER &amp; MECHANICS</td>
						<td align="center">'.number_format(round($min_wage_info['highskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($min_wage_info['highskilled_rate_zone_a']/26),2).')</td>
					</tr>';
		
	}else{
		if($type_of_sch_emp == '1' || $type_of_sch_emp == '4'){
			$output .= '<tr>
							<th align="center">Zone A (Rs.)</th>
							<th align="center">Zone B (Rs.)</th>
						</tr>';
		}
		
		if(!empty($minWagesMaster['unskilled_rate_zone_a'])){
		  $output .= '<tr>
			<td align="left">Unskilled</td>
			<td align="center">'.number_format(round($minWagesMaster['unskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($minWagesMaster['unskilled_rate_zone_a']/26),2).')</td>';
			if($type_of_sch_emp == '1' || $type_of_sch_emp == '4'){
			$output .= '<td align="center">'.number_format(round($minWagesMaster['unskilled_rate_zone_b']),2).'<br>(Daily - '.number_format(round($minWagesMaster['unskilled_rate_zone_b']/26),2).')</td>';
		}
			 $output .= '</tr>';
		}
		if(!empty($minWagesMaster['semiskilled_rate_zone_a'])){
		  $output .= '<tr>
			<td align="left">Semi-Skilled</td>
			<td align="center">'.number_format(round($minWagesMaster['semiskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($minWagesMaster['semiskilled_rate_zone_a']/26),2).')</td>';
			if($type_of_sch_emp == '1' || $type_of_sch_emp == '4'){
				$output .= '<td align="center">'.number_format(round($minWagesMaster['semiskilled_rate_zone_b']),2).'<br>(Daily - '.number_format(round($minWagesMaster['semiskilled_rate_zone_b']/26),2).')</td>';
			}
		 $output .= '</tr>';
		}
		if(!empty($minWagesMaster['skilled_rate_zone_a'])){
			$output .= '<tr>
			<td align="left">Skilled</td>
			<td align="center">'.number_format(round($minWagesMaster['skilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($minWagesMaster['skilled_rate_zone_a']/26),2).')</td>';
			if($type_of_sch_emp == '1' || $type_of_sch_emp == '4'){
				$output .= '<td align="center">'.number_format(round($minWagesMaster['skilled_rate_zone_b']),2).'<br>(Daily - '.number_format(round($minWagesMaster['skilled_rate_zone_b']/26),2).')</td>';
			}
		  $output .= '</tr>';
		}
		if(!empty($minWagesMaster['highskilled_rate_zone_a'])){
		  $output .= '<tr>
			<td align="left">Highly Skilled</td>
			<td align="center">'.number_format(round($minWagesMaster['highskilled_rate_zone_a']),2).'<br>(Daily - '.number_format(round($minWagesMaster['highskilled_rate_zone_a']/26),2).')</td>';
			if($type_of_sch_emp == '1' || $type_of_sch_emp == '4'){
				$output .= '
				<td align="center">'.number_format(round($minWagesMaster['highskilled_rate_zone_b']),2).'<br>(Daily - '.number_format(round($minWagesMaster['highskilled_rate_zone_b']/26),2).')</td>';
			}
		  $output .= '</tr>';
		}
	}
$output .= '</table></div>';

if(trim($employment_id) == 13){
	$output .= 'The following shall be the piece rates of wages for different categories of employees employed in <b>Brick Manufactories</b> in West Bengal: 
				<div style="margin:20px 0;"><table width="100%" cellspacing="1" class="pdf-circular-table">
					<tr>
						<th rowspan="2" width="40%"> Category</th>
						<th colspan="2"> Rates of Minimum Wage per thousand brick</th>
					</tr>
					<tr>
						<th> Without Accommodation(Rs.)</th>
						<th> With Accommodation(Rs.)</th>
					</tr>
					<tr>
						<td>1.(a) Brick moulding pugmill 
									<br>&emsp;&emsp;(i) Pathora 
									<br>&emsp;&emsp;(ii) Mutphulia 
									<br>&emsp;&emsp;(iii) Tubwaya
									<br>&emsp;&emsp;(iv) Mukkata 
									<br>&emsp;&emsp;(v) Boyel Hakka <br><br> 
							  &ensp;&nbsp;(b) Lathgung </td>
						<td><br>'.number_format(round((268*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
								 .number_format(round((68*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
								 .number_format(round((34*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
								 .number_format(round((34*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
								 .number_format(round((11*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value),2).'<br><br>'
								 .number_format(round((301*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value),2).'</td>
								 
						<td><br>'.number_format(round((252*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
								 .number_format(round((60*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
								 .number_format(round((30*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
								 .number_format(round((30*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
								 .number_format(round((9*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value),2).'<br><br>'
								 .number_format(round((285*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value),2).'</td>
					</tr>
					<tr>
						<td>2.(a) Loader of Bricks in Bhatta <br>
							   &ensp;&nbsp;(b) Unloader of Bricks from Bhatta <br>
							   &ensp;&nbsp;(c) Earth Cutter</td>
						<td>'.number_format(round((143*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
							 .number_format(round((135*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
							 .number_format(round((113*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value),2).'<br>
							 (Per 100 cft.)</td>
								 
						<td>'.number_format(round((120*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
							 .number_format(round((120*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value),2).'<br>'
							 .number_format(round((97*$minWagesMaster['cpi_value_zone_b'])/$fix_al_value),2).'<br>
							 (Per 100 cft.)</td>
					</tr>
					
				</table></div>';
}

if($employment_id== 5){
	$output .= '<li><b> Clarification: </b>It has been clarified by the Labour Department, Government of West Bengal by No. 134-MW/2W-18/2010 dated 27.08.2012 that the Employment of Bakery includes Biscuit Manufacturing Units also.</li> ';
}
	
if($minWagesMaster['implementing_area'] == 2 || $minWagesMaster['implementing_area'] == 3){
	$implementing_area = '<li><b> Implementing areas: Zone ‘A’ </b>= ';
	if($minWagesMaster['implementing_area'] == 2){
		$implementing_area .= '(i) Areas under Municipal Corporations, Municipalities, notified Areas/ Authorities, Development Authorities, Thermal Power Plants Areas including Industrial Township Areas.';
	}else if($minWagesMaster['implementing_area'] == 3){
		$implementing_area .= '(i) Areas Notified under Kolkata Metropolitan Development Authority(KMDA), (ii) Asansol Municipal Corporation, (iii) Durgapur Municipal Corporation, (iv) Siliguri Municipal Corporation, (v) Digha-Shankarpur Development Authority, (vi) Thermal Power plant areas including Township areas, (vii) All the Municipal areas in different Districts, (viii) All the Sub-Divisions in the districts of Howrah, Hoogly, Burdwan and 24- Parganas (N)[excluding Barasat and Basirhat], (ix) Alipur Sub-Division and Baruipur Sub-Division in the district of 24-Parganas(S), (x) Midnapore Sadar Sub-Division and Kharagpur Sub-Division in the district of West Midnapore, (xi) Siliguri sub-division in the district of Darjeeling, (xii)Bankura Sub-Division and Bisnupur Sub-division in the district of Bankura, and (xiii) Raghunathpur Sub-Division in the district of Purulia.';
	}  
	$implementing_area .= '<br><b>Zone ‘B’</b>= Rest of the West Bengal. </li>';
}else if($minWagesMaster['implementing_area'] == 1){
	$implementing_area .= '<li>There will only be one rate of minimum wage for the whole of West Bengal.';
}
if($minWagesMaster['worker_type']== 1){
	$emp_clarification = '<li>The classification of employees in the above employment is as follows :- ';
	if(!empty($minWagesMaster['unskilled_desc'])){$emp_clarification .= '<br><b>Unskilled Workers: </b>'.$minWagesMaster['unskilled_desc'].'.';}
	if(!empty($minWagesMaster['semiskilled_desc'])){$emp_clarification .= '<br><b>Semi-skilled Workers: </b>'.$minWagesMaster['semiskilled_desc'].'.';}
	if(!empty($minWagesMaster['skilled_desc'])){$emp_clarification .= '<br><b>Skilled Workers: </b>'.$minWagesMaster['skilled_desc'].'.';}
	if(!empty($minWagesMaster['highlyskilled_desc'])){$emp_clarification .= '<br><b>Highly Skilled Workers: </b>'.$minWagesMaster['highlyskilled_desc'].'.</li>';}
}

if($minWagesMaster['flip_option']== 1){
	$output .= $emp_clarification.$implementing_area;
}else{
	$output .= $implementing_area.$emp_clarification;
}

/************ Page Break ******************/
if($minWagesMaster['implementing_area'] == 2 && $minWagesMaster['worker_type']== ''){
	$p_break_start = '<p style="page-break-after:always">'; 
	$p_break_end = '</p>';
}

if(trim($employment_id) == 19){
	$output .= '<li>
					<b>The classification of employees is as follows:-</b><br><h3>CINEMA HOUSE EXHIBITION</h3></br>
					<p><b>Unskilled:</b> Peon, Bearer, Maid-servants, Sweeper, Poster Boys, Durwans, Watchman, Shifter, Dispensers, Bar-attendants, Chocolate Boy, Soda Fountain Hamal, Mali, Traffic Linesman or any other employee engaged in a similar nature of unskilled work by whatever name called. </p>
					<p><b>Semi-skilled:</b> Usher, Doorman, Liftman, Gate-keeper, Fireman, Assistant Painter, Assistant Carpenter, Assistant Polishwala, MCOT (Member of Customer Dealing Team), Ticket Seller, Food/Snacks Seller or any other employee engaged in a similar nature of semi-skilled work by whatever name called.</p>
					<p><b>Skilled:</b> Assistant Engine Driver, Electrician, Operator, Projectionist, Polishwala, Painter, Carpenter, Caretaker, Telephone Operator, Soda Fountain Manager, Bar Manager, Booking &amp; other Clerks, Cashier, Publicity Officer, Engine Driver, 1 st Assistant/A.C. Operator, 1 st Assistant Operator or
any other employee engaged in a similar nature of skilled work by whatever name called.</p>
					<p><b>Highly Skilled:</b> Head Operator, Head A.C. Operator, Head Electrician, Assistant Manager, Supervisor, Accountant or any other employee engaged in a similar nature of highly skilled work by whatever name called.</p>
					<br><h3>DISTRIBUTION UNITS</h3></br>
					<p><b>Unskilled:</b>Peon, Sweeper, Mali, Bearer, Durwan, Watchman or any other employee engaged in a similar nature of unskilled work by whatever name called.</p>
					<p><b>Skilled:</b> Clerks, Accessories Clerks, Shipping, Filing and Mailing Clerks, Representative/Field Inspector, Stenographer, Booker/Salesman, Accountant/Cashier or any other employee engaged in a similar nature of skilled work by whatever name called.</p>
					<p><b>Highly Skilled:</b> Manager or any other employee engaged in a similar nature of highly skilled work by whatever name called.</p>
					<br><h3>PRODUCTION UNITS</h3></br>
					<p><b>Unskilled:</b> Production Boy or any other employee engaged in a similar nature of unskilled work by whatever name called.</p>
					<p><b>Semi-skilled:</b> 2nd Assistant Director and/or Continuity man, 2 nd Assistant Cameraman and/or Focus Puller, Assistant Production Manager, Junior Artist C-Grade or any other employee engaged in a similar nature of semi-skilled work by whatever name called.</p>
					<p><b>Skilled:</b> Clerks, Driver, Junior Artist Special Grade Child/Old, Junior Artist A-Group with costume + dialogue, Junior Artist B-Group (with dialogue), Cat work or any other employee engaged in a similar nature of skilled work by whatever name called.</p>
					<p><b>Highly Skilled:</b> Assistant Cameraman (1 st Asstt.), Assistant Art Director (1 st Asstt.), Stunt Man/ Fighter, Duplicate/Dummy, Effect Man, Dancer A Grade, Dancer B Grade, Dancer C Grade, Asstt. Fight Master, Hair Styler, Costume man, Still Photographer, Make-up Artist or any other employee engaged in a similar nature of highly skilled work by whatever name called.</p>
					<br><h3>STUDIOS</h3></br>					
					<p><b>Unskilled:</b> Office Boy, Durwan, Sweeper and Mali or any other employee engaged in a similar nature of unskilled work by whatever name called.</p>
					<p><b>Semi-skilled:</b> 3rd Assistant Cameraman, Camera Mazdoor, Junior Carpenter, Painter, Setting Mazdoor, Studio Lighting, Electrician, Assistant Make-up-Man, Dresser or any other employee engaged in a similar nature of semi-skilled work by whatever name called.</p>
					<p><b>Skilled:</b> 2 nd Assistant Cameraman and/or Focus Puller, Sound Boom Man, Senior Carpenter, Charge-hand setting Mazdoor, Setting Outdoor Indoor, Setting Construction/Art Setting, Studio Maintenance and Shop, Electrician, Clerks, Accountant/Cashier or any other employee engaged in a similar nature of skilled work by whatever name called.</p>
					<p style="margin-bottom:0px;"><b>Highly Skilled:</b> Assistant Cameraman (1 st Assistant), Assistant Sound Recordist, Modeller/Designer, Title Card Writer, Setting Deptt.-in Charge, Charge-hand Carpenter, Floor Charge hand Lighting Electrician (Head Electrician), Back-ground Painter, Assistant Manager/Laboratory Officer-in-charge or any other employee engaged in a similar nature of highly skilled work by whatever name called.</p>
					<br><h3>LABORATORIES</h3></br>					
					<p><b>Unskilled:</b> Sweeper, Durwan, Watchman, Office Boy or Laboratory Boy, Mali or any other employee engaged in a similar nature of unskilled work by whatever name called.</p>
					<p><b>Semi-skilled:</b> Dark room Mazdoor, Sound Mazdoor or any other employee engaged in a similar nature of semi-skilled work by whatever name called.</p>
					<p><b>Skilled:</b> Dark room Assistant, Grading Assistant, Assistant Projectionist, Assistant Electrician, Film Checking and Clearing-in-charge, 2 nd Assistant Sound Recordist, Chemical Bath-in-charge, Clerk or any other employee engaged in a similar nature of skilled work by whatever name called.</p>
					<p><b>Highly Skilled:</b> Chief Projectionist, Chief Electrician, Maintenance Mechanics, Assistant Sound Recordist, Accountant/Cashier, Assistant Manager/Laboratory Office-in-charge or any other employee engaged in a similar nature of highly skilled work by whatever name called.</p>
					
					
				</li>';}
if(trim($employment_id) == 71){
	$output .= '<b> NOTES :- </b> <br>
						1. To arrive at daily rates of wages, the monthly rates may be divided by 26 (to be rounded off to the nearest rupee).<br>
						2. Wages of full-time piece-rated workers, if there be any, shall in no case be less than the daily rates of minimum wages.<br><br>
						<b>CATEGORIES OF EMLOYEES :</b><br><br>
						
						<b>UNSKILLED/OTHERS &#8217;A&#8217; : </b> General Mazdoor/Cooli/Kamin, Loader, Unloader, Porter, Asstt. Gardener, Tarman, Stageman, Canteen Boy, Car-cleaner.<br>
						<b>SEMI-SKILLED : </b>Crusher Mazdoor, Line Mazdoor, Grinder, Trollyman, Stamper, Checker, Setter, Semi-skilled Mason, Boilerman, Carpenter, Mixing Attendant, Pump Khalashi.<br>
						<b>SKILLED/OTHERS &#8217;B&#8217; :</b> Turner, Fitter, Electrician, Moulder, Fireman, Latheman, Pipe-plant Polisher, Klin Setter, Trindal Press Operator, Mixingman-cum-Operator, Welder, Gas Cutter, Blacksmith, Pattern Maker, Mill Attendant, Packerman.<br>
						<b>HIGHLY SKILLED/OTHERS &#8217;C&#8217; :</b> Head Mason, Head Carpenter, Head Fitter, Trindal, Shift Moulder, Head Welder, Head Turner, Moulder, Supervisor, Mechanical Fitter.<br>
						<b>OTHERS &#8217;A&#8217; :</b> Peon, Durwan, Security Guard.<br>
						<b>OTHERS &#8217;B&#8217; :</b> Junior Clerk, Typist, Compounder, Nurse, Driver, Stock-keeper, Laboratory Asstt.<br>
						<b>OTHERS &#8217;C&#8217; : </b>Senior Clerk, Head Clerk, Accountant, Supervisor, Security Officer, Foreman, Chemist.<br><br>
						<b>ADDITIONAL INFORMATION :</b><br><br>
						<b>C.P.I. &amp; BASE YEAR :</b> ASANSOL (BASE 1960 = 100)<br>
						<b>Fixation Point :</b> 1416. <br>
						<b>Average of C.P.I. from Apl..`19 Sept.`19., </b>= 7122 points.<br>
						<b>Rise in Index : </b>5706 points.<br>
						<b>Rate of adjustment of D.A. = </b>Re.1.00 per point per month <br>';
}else if(trim($employment_id) == 91){
	$output .= '<br><br><br>
					<b>ADDITIONAL INFORMATION :</b><br><br>
						<table>
							<tr>
								<td><b>Area :</b></td>
								<td>Kolkata &amp; 24-Pgs.</td>
								<td>Howrah and Hooghly</td>
								<td>Other Districts</td>
							</tr>
							<tr>
								<td><b>C.P.I :</b></td>
								<td>(Kolkata)</td>
								<td>(Howrah)</td>
								<td>(Nadia &amp; Murshidaba)</td>
							</tr>
							<tr>
								<td><b>Base Year  :</b></td>
								<td>(1960 = 100)</td>
								<td>(1960 = 100)</td>
								<td>(1951 = 100)</td>
							</tr>
							<tr>
								<td><b>Average of C.P.I. Nos. of the three centres from January 2019 to June 2019 :</b></td>
								<td>6982</td>
								<td>6416</td>
								<td>6888</td>
							</tr>
							<tr>
								<td><b>Fixation Point :</b></td>
								<td>421</td>
								<td>402</td>
								<td>402</td>
							</tr>
							<tr>
								<td><b>Rate of Neutralization per point per day :</b></td>
								<td>P. 3.8</td>
								<td>P. 3.8</td>
								<td>P. 3.8</td>
							</tr>
						</table>
						<p></p>';
}else if(trim($employment_id) == 69){
	$output .= '<br>
				<b>ADDITIONAL INFORMATION :</b><br><br>
				<b>C.P.I. No. for Industrial Workers =</b> Kolkata Centre.<br>
				<b>Base Year =</b> 1960=100. <br>
				<b>Half-yearly Average of C.P.I. nos. from July, 2019 to Dec, 2019 </b>= 7080 points.<br>
				<b>Fixation Point = </b>392<br>
				<b>Rate of adjustment of D.A. = </b>Upto 519 points over fixation point.@Re.1.00 per point <br>and above 519 points @Rs.1.20 per point per month. <br>';
}else if(trim($employment_id) == 41){
	$output .=' 1. The daily rates of minimum wages of the different categories of employees shall be
				computed by dividing the total monthly minimum wages by twenty six and shall be rounded
				off to the nearest paise.<br>
				2. To get the weekly rates of minimum wages, daily rates of minimum wages shall be multiplied
				by six and rounded off to the nearest paise.
				<br>
				<table width="100%">
				<tr>
					<th align="left">Unskilled</th>
					<th align="left">Semi-skilled</th>
					<th align="left">Skilled</th>
					<th align="left">Highly Skilled</th>
				<tr>
				<tr>
					<td>1. Annealing Chamber Coolies.</td>
					<td>Bubbler/Bubbler Helper.</td>
					<td>Lorry Driver</td>
					<td>Blower</td>
				</tr>
				<tr>
					<td>2. Furnace Coolies.</td>
					<td>Lineman.</td>
					<td>Fireman. </td>
					<td>Foreman</td>
				</tr>
				<tr>
					<td>3. Carrier (Jaliwala) Coolies.</td>
					<td>Airman.</td>
					<td>Flower (Ord)</td>
					<td>Shift-in-Charge</td>
				</tr>
				<tr>
					<td>4. General Coolies.</td>
					<td>Fireman Helper.</td>
					<td>Helper to Blower.</td>
					<td></td>
				</tr>
				<tr>
					<td>5.Poker.</td>
					<td>Meltors.</td>
					<td>Machineman (Cutter)</td>
					<td></td>
				</tr>
				<tr>
					<td>6. Mason (Helper).</td>
					<td>Blacksmith.</td>
					<td>Machineman (Helper)</td>
					<td></td>
				</tr>
				<tr>
					<td>7. Batch Coolies (Batch Room).</td>
					<td>Grinder and Polisher.</td>
					<td>Cutting off Fine Polishing</td>
					<td></td>
				</tr>
				<tr>
					<td>8. Sweeper.</td>
					<td>Cook.</td>
					<td>Machine Operator</td>
					<td></td>
				</tr>
				<tr>
					<td>9. Canteen Boys.</td>
					<td>Sorter.</td>
					<td>Fitter Mechanic</td>
					<td></td>
				</tr>
				<tr>
					<td>10. Chimney Grinder.</td>
					<td>Rulswala (Non-Maker)</td>
					<td>Electrician</td>
					<td></td>
				</tr>
				<tr>
					<td>11. Chimney Grinder.</td>
					<td>-</td>
					<td>Turner</td>
					<td></td>
				</tr>
				<tr>
					<td>12. Lorry Mazdoors.</td>
					<td>-</td>
					<td>Latheman</td>
					<td></td>
				</tr>
				<tr>
					<td>13. -</td>
					<td>-</td>
					<td>Welder</td>
					<td></td>
				</tr>
				</table>
				<br>
				<b>ADDITIONAL INFORMATION :</b><br>
						<b>C.P.I. (BASE 1960 = 100) :</b> KOLKATA <br>
						<b>Fixation Point :</b> 245<br>
						<b>Average of C.P.I. from July, 2019 to Dec, 2019 :</b>7180<br>
						<b>Difference in C.P.I. :</b>6935<br>
						<b>Rate of adjustment of VDA :</b>Re.1.00 per point per month.<br>';
	
}else{	
	
	$output .= '<li> To arrive at daily rates, monthly rates shall have to be divided by 26 (to be rounded off to the nearest rupee) and to arrive at weekly rates, daily rates shall have to be multiplied by 6.</li>
	<li> A normal working day shall consist of eight and half hours of work including interval for half-an-hour for rest. </lip>
	<li> One day in any period of seven days as may suit the local convenience shall be the day of weekly rest. The minimum rates of wages include the wages for weekly day of rest. For payment for work done on the day of weekly rest and for work done beyond the normal working hours shall be double the ordinary rate of wages. </li>
	<li> Where the existing rates of wages of any employee based on contract or agreement or otherwise are higher than the rates notified herein, the higher rates shall be protected and treated as minimum rates of wages applicable for the purpose of this notification to all such employees. </li>
	<li> These minimum rates of wages are applicable to employees employed by contractors also. </li> 
	<li> The  minimum  rates  of  wages  for  disabled  persons  shall  be  same  as  payable  to  the  workers  of  appropriate category. </li>
	<li> The men and women employees shall get the same rates of wages for the same work or work of similar nature. </li>
	<li> The  minimum rates  of  wages  and  Variable  Dearness  Allowance,  if  any,  both together shall constitute  the  minimum rates of wages to be be enforceable under the Minimum Wages Act, 1948(11 of 1948).</li>';
	if(trim($employment_id)== 6){
		$output.= '<li>Piece rates of work, if there be any, shall in no case be less than the daily minimum time rates of wages.</li>
				   <li>For each principal meal, if supplied by the employer, the daily rates of minimum wages
shall be reduced by Rs. 5.37.</li>';
	}
	if(trim($employment_id)== 1 || trim($employment_id)== 72){
		$output.= '<li>Where the practice of payment partly in cash and partly in produce is prevalent, the total value of produce supplied plus the cash wages paid shall not be less than the minimum wages.</li>';
	}else if(trim($employment_id) == 13){
		$output.= '<li>The daily earnings of a piece rated employee shall not be less than the rate for a daily rated employee.</li>';
	}
	    $output.= '<span style="font-size:13px;padding-top:-5px;">This is issued with due approval of the Labour Commissioner, West Bengal.</span>';
	}
 
 if(trim($employment_id) == 13){
	  $pagebreak = '<br><br><br><br><br><br><br><br><br><br><br><br><br><br>';
	  $pagebreak1 = '<br><br><br><br><br>';
 }else if(trim($employment_id) == 71){
	  $pagebreak = '<br><br><br><br><br><br><br><br><br><br><br><br>';
	  $pagebreak1 = '<br><br><br><br>'; 
 }else{
	 $pagebreak = '';
	 $pagebreak1 = '';
 }
 	
if(trim($employment_id) == 91 || trim($employment_id) == 69 || trim($employment_id) == 41){
	$output .= '<div style="overflow:hidden;margin:40px 0 30px 0;"><div style="float:right;width:40%;text-align:center;">('.$minWagesMaster['signature_by_name'].')<br>'.$min_wage_info['signature_by_designation'].'<br>West Bengal</div></div><br>'.$pagebreak.'
	<div style="overflow:hidden;margin:0 0 20px 0;"><div style="float:left;font-size:13px;width:60%;">MEMO. NO. ______'.$publish_memo_no.$minWagesMaster['memo_no'].'</div><div style="float:right;width:30%;text-align:right;font-size:14px;">Dated:__/__/____ </div></div>
	<div style="overflow:hidden;margin:0 0 6px 0;"><div style="float:left;width:3%;">1.</div><div style="float:right;width:95%;text-align:left;font-size:13px;">The Principal Secretary, Labour Department, New Secretariat Buildings, 12th floor, 1, K.S. Roy Road, Kolkata –700 001.</div></div>
	<div style="overflow:hidden;margin:0 0 6px 0;"><div style="float:left;width:3%;">2.</div><div style="float:right;width:95%;text-align:left;font-size:13px;">The  Labour Commissioner, New Secretariat Building, 11th floor, 1, K.S. Roy Road, Kolkata –700 001.</div></div>	
	<div style="overflow:hidden;margin:0 0 6px 0;"><div style="float:left;width:3%;">3.</div><div style="float:right;width:95%;text-align:left;font-size:13px;">The Joint/Deputy/Assistant  Labour  Commissioner ______________________ with  the  request  to circulate this among the Inspectors, M.W. under his jurisdiction.</div></div>
	<div style="overflow:hidden;margin:0 0 6px 0;"><div style="float:left;width:3%;">4.</div><div style="float:right;width:95%;text-align:left;font-size:13px;">The  Secretary  (Central  Trade  Unions/All  Chambers  of  Commerce) ________________________ with the request to circulate this among his constituents.</div></div>
	
	<div style="overflow:hidden;margin:30px 0 0px 0;"><div style="float:right;width:50%;text-align:center;">('.$minWagesMaster['signature_by_name'].')<br>'.$minWagesMaster['signature_by_designation'].'<br>West Bengal</div></div>';
}else{
	$output .= $pagebreak1.'<div style="overflow:hidden;margin:10px 0 20px 0;"><div style="float:right;width:50%;text-align:center;">('.$minWagesMaster['signature_by_name'].')<br>'.$minWagesMaster['signature_by_designation'].'<br>West Bengal</div></div><br>'.$pagebreak.'
	<div style="overflow:hidden;margin:0 0 15px 0;"><div style="float:left;font-size:13px;width:60%;">MEMO. NO. ______'.$publish_memo_no.$minWagesMaster['memo_no'].'</div><div style="float:right;width:30%;text-align:right;font-size:14px;">Dated: __/__/____ </div></div>
	<div style="overflow:hidden;margin:0 0 6px 0;"><div style="float:left;width:3%;">1.</div><div style="float:right;width:95%;text-align:left;font-size:13px;">The Principal Secretary, Labour Department, New Secretariat Buildings, 12th floor, 1, K.S. Roy Road, Kolkata –700 001.</div></div>
	<div style="overflow:hidden;margin:0 0 6px 0;"><div style="float:left;width:3%;">2.</div><div style="float:right;width:95%;text-align:left;font-size:13px;">The  Labour Commissioner, New Secretariat Building, 11th floor, 1, K.S. Roy Road, Kolkata –700 001.</div></div>	
	<div style="overflow:hidden;margin:0 0 6px 0;"><div style="float:left;width:3%;">3.</div><div style="float:right;width:95%;text-align:left;font-size:13px;">The  Secretary  (Central  Trade  Unions/All  Chambers  of  Commerce) ________________________ with the request to circulate this among his constituents.</div></div>
	<div style="overflow:hidden;margin:0 0 6px 0;"><div style="float:left;width:3%;">4.</div><div style="float:right;width:95%;text-align:left;font-size:13px;">The Joint/Deputy/Assistant  Labour  Commissioner ______________________ with  the  request  to circulate this among the Inspectors, M.W. under his jurisdiction.</div></div>
	<div style="overflow:hidden;margin:30px 0 0px 0;"><div style="float:right;width:50%;text-align:center;font-size:13px;">('.$minWagesMaster['signature_by_name'].')<br>'.$minWagesMaster['signature_by_designation'].'<br>West Bengal</div></div>';
}
return pdf_using_mpdf_api($output, $name_of_employment);
}
?>