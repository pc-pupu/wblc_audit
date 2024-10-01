<?php
function minimum_wage_pdf($employment_id){
	$employment_id = encryption_decryption_fun('decrypt',$employment_id);
	
	$min_wage_info = db_select('l_min_wages_schedule_employment_details_info','l');
	$min_wage_info -> fields('l', array());
	$min_wage_info -> condition('l.employment_id',$employment_id);
	$min_wage_info -> orderBy('l.id', 'DESC');
	$min_wage_info = $min_wage_info->execute()->fetchAssoc();
	
	$name_of_employment = $min_wage_info['employment_title'];
	$notification_number = $min_wage_info['notification_number']; 
	$notification_date =  (!empty($min_wage_info['notification_date'])?date("d.m.Y",$min_wage_info['notification_date']):'');
	$notification_published_dt =  date("d.m.Y",$min_wage_info['notification_published_dt']);
	$type_of_sch_emp = $min_wage_info['type_of_sch_emp'];
	
	/*$min_wage_desc = db_select('l_min_wages_schedule_employment_description_master','l');
	$min_wage_desc -> fields('l', array());
	//$min_wage_desc -> condition('l.employment_id',$employment_id);
	//$min_wage_desc -> orderBy('l.id', 'DESC');
	$min_wage_desc = $min_wage_desc->execute()->fetchAll();	*/
	
	/*$min_wage_area_worker_desc = db_select('l_min_wage_sc_emp_area_worker_description','l');
	$min_wage_area_worker_desc -> fields('l', array());
	$min_wage_area_worker_desc -> condition('l.employment_id',$employment_id);
	//$min_wage_desc -> orderBy('l.id', 'DESC');
	$result = $min_wage_area_worker_desc->execute()->fetchAssoc();*/	
	
	if($min_wage_info['session']=='1'){$period = '1st January, '.$min_wage_info['year'].' to 30th June, '.$min_wage_info['year'];} else {$period = '1st July,'.$min_wage_info['year'].' to 31st Dec, '.$min_wage_info['year'];}

	$output='<style type="text/css">
	.pdf-circular-table td,th,caption{background-color:#fff; padding: 5px 10px;}
	.pdf-circular-table { background-color:#333; }
	.pdf-circular-table th { font-size:14px;font-weight:700; }
	.pdf-circular-table td { font-size:12px; font-weight:300; }
	</style>';
	
	$img=$GLOBALS['base_url'].'/sites/all/themes/lcTheme/images/Gov-logo-sm.png';
	
	$output .= '<div style="text-align:center;"><img src='.$img.' alt=""><h5 style="font-size:14px;font-weight:bold;margin:4px 0 20px 0;">Government of West Bengal<br> Office of the Labour Commissioner, Statistics Section,<br> 6, Church Lane, 3rd floor, Kolkata –700 001.</h5></div>
	<div style="overflow:hidden;margin:0 0 20px 0;"><div style="float:left;width:50%;">No. &emsp;'.$min_wage_info['publish_memo_no'].$min_wage_info['no'].'</div><div style="float:right;width:40%;text-align:right;">Dated: 27/12/2019 </div></div>
	<div style="text-align:center;font-size:16px;font-weight:bold;margin:0 0 15px 0;">CIRCULAR</div>
	<div style="font-size:12px;font-weight:700; text-align:justify">The Government of West Bengal in the Labour Department has fixed minimum rates of wages in the employment of  <b>'.$name_of_employment.' </b>in the state of West Bengal as per notification number '.$notification_number.' Dated '.$notification_date;
	
	if(!empty($min_wage_info['notification_published_dt'])){ 
		$output .= ',as published in the Kolkata Gazette Extraordinary dated '.$notification_published_dt.'.';
	}else{$output .= '.';}
	if($type_of_sch_emp == '2' || $type_of_sch_emp == '4'){
		$output .= 'These rate will be effective for the period from <b>'.$period.'</b>.';
	}
	$output .= '<ol type="a" style="line-height:21px text-align:justify">';
	if($type_of_sch_emp == '1' || $type_of_sch_emp == '3'){
	$output .= '<li> Now, with reference to the said notification, the following shall be the minimum rates of wages in the said employment for the period from <b>'.$period.'</b>.</li>';$output .= '</div>';
	}
	
	$output .= '<div style="margin:20px 0;">
					<table width="100%" cellspacing="1" class="pdf-circular-table">
  						<tr>'; 
	if($type_of_sch_emp == '1' || $type_of_sch_emp == '4'){
		 $rowspan = '2';$colspan = '2';
	}else if(trim($min_wage_info['employment_category']) == 'AGR'){
		 $rowspan = '3'; $colspan = '3'; 
	}else{
		 $rowspan = '';$colspan = '';
	}
		$output .= '<th rowspan="'.$rowspan.'" align="center" valign="top" width="">Categories of Employees</th>
					<th colspan="'.$colspan.'" align="center">Monthly Minimum Rates of Wages (Rs.)</th>
				</tr>';
	/*************** AGRICULTUR ********************/				 
	if(trim($min_wage_info['employment_category']) == 'AGR'){				 
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
		
	}else if(trim($min_wage_info['employment_category']) == 'BLP'){				 
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
						<td align="left">'.number_format((15*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point'],2).' (per 100 leaves)
							<br>'.number_format((12*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point'],2).' (per 3 kg. bundle Rs. 12.00) 
							<br>'.number_format((6*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point'],2).' (per 50 kg. bag Rs. 6.00)
							<br>'.number_format((6*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point'],2).' (per 50 kg. bag Rs. 6.00)
						</td>
					</tr>';
	/******************* CINCHONA PLANTATION  & FISHERY & TOBACCO GODOWN********************/				
	}else if(trim($min_wage_info['employment_category']) == 'CHNP' || trim($min_wage_info['employment_category']) == 'FISH' ||trim($min_wage_info['employment_category']) == 'FRTO' ||trim($min_wage_info['employment_category']) == 'TOGO' ||trim($min_wage_info['employment_category']) == 'MDPL'){
		$output .= '<tr>
						<td align="center">Adult Worker</td>
						<td align="center">'.number_format(round($min_wage_info['unskilled_rate_zone_a']),2).'<br>
						(Daily - '.number_format(round($min_wage_info['unskilled_rate_zone_a']/26),2).')</td>
					</tr>';
	}else if(trim($min_wage_info['employment_category']) == 'SERI'){
		$output .= '<tr>
						<td align="center" >Per Month(Rs.)</td>
						<td align="center">Per Day(Rs.)</td>
					</tr>
					<tr>
						<td align="center">Unskilled</td>
						<td align="center">'.number_format(round($min_wage_info['unskilled_rate_zone_a']),2).'</td>
						<td align="center">'.number_format(round($min_wage_info['unskilled_rate_zone_a']/26),2).'</td>
					</tr>';
	}else if(trim($min_wage_info['employment_category']) == 'BRMF'){
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
	}else if(trim($min_wage_info['employment_category']) == 'REFI'){
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
if(trim($min_wage_info['employment_category']) == 'BRMF'){
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
						<td><br>'.number_format(round((268*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point']),2).'<br>'
								 .number_format(round((68*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point']),2).'<br>'
								 .number_format(round((34*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point']),2).'<br>'
								 .number_format(round((34*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point']),2).'<br>'
								 .number_format(round((11*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point']),2).'<br><br>'
								 .number_format(round((301*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point']),2).'</td>
								 
						<td><br>'.number_format(round((252*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point']),2).'<br>'
								 .number_format(round((60*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point']),2).'<br>'
								 .number_format(round((30*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point']),2).'<br>'
								 .number_format(round((30*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point']),2).'<br>'
								 .number_format(round((9*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point']),2).'<br><br>'
								 .number_format(round((285*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point']),2).'</td>
					</tr>
					<tr>
						<td>2.(a) Loader of Bricks in Bhatta <br>
							   &ensp;&nbsp;(b) Unloader of Bricks from Bhatta <br>
							   &ensp;&nbsp;(c) Earth Cutter</td>
						<td>'.number_format(round((143*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point']),2).'<br>'
							 .number_format(round((135*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point']),2).'<br>'
							 .number_format(round((113*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point']),2).'<br>
							 (Per 100 cft.)</td>
								 
						<td>'.number_format(round((120*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point']),2).'<br>'
							 .number_format(round((120*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point']),2).'<br>'
							 .number_format(round((97*$min_wage_info['cpi_value_zone_b'])/$min_wage_info['fixation_point']),2).'<br>
							 (Per 100 cft.)</td>
					</tr>
					
				</table></div>';
}
if($min_wage_info['employment_id'] == 4){
	$output .= '<li><b> Clarification: </b>It has been clarified by the Labour Department, Government of West Bengal by No. 134-MW/2W-18/2010 dated 27.08.2012 that the Employment of Bakery includes Biscuit Manufacturing Units also.</li> ';
}	
if($min_wage_info['implementing_area'] == 2 || $min_wage_info['implementing_area'] == 3){
	$implementing_area = '<li><b> Implementing areas: Zone ‘A’ </b>= ';
	if($min_wage_info['implementing_area'] == 2){
		$implementing_area .= '(i) Areas under Municipal Corporations, Municipalities, notified Areas/ Authorities, Development Authorities, Thermal Power Plants Areas including Industrial Township Areas.';
	}else if($min_wage_info['implementing_area'] == 3){
		$implementing_area .= '(i) Areas Notified under Kolkata Metropolitan Development Authority(KMDA), (ii) Asansol Municipal Corporation, (iii) Durgapur Municipal Corporation, (iv) Siliguri Municipal Corporation, (v) Digha-Shankarpur Development Authority, (vi) Thermal Power plant areas including Township areas, (vii) All the Municipal areas in different Districts, (viii) All the Sub-Divisions in the districts of Howrah, Hoogly, Burdwan and 24- Parganas (N)[excluding Barasat and Basirhat], (ix) Alipur Sub-Division and Baruipur Sub-Division in the district of 24-Parganas(S), (x) Midnapore Sadar Sub-Division and Kharagpur Sub-Division in the district of West Midnapore, (xi) Siliguri sub-division in the district of Darjeeling, (xii)Bankura Sub-Division and Bisnupur Sub-division in the district of Bankura, and (xiii) Raghunathpur Sub-Division in the district of Purulia.';
	}  
	$implementing_area .= '<br><b>Zone ‘B’</b>= Rest of the West Bengal. </li>';
}else if($min_wage_info['implementing_area'] == 1){
	$implementing_area .= '<li>There will be only one rate of minimum wage for the whole of West Bengal.';
}
if($min_wage_info['worker_type']== 1){
	$emp_clarification = '<li>The classification of employees in the above employment is as follows :- ';
	if(!empty($min_wage_info['unskilled_desc'])){$emp_clarification .= '<br><b>Unskilled Workers: </b>'.$min_wage_info['unskilled_desc'].'.';}
	if(!empty($min_wage_info['semiskilled_desc'])){$emp_clarification .= '<br><b>Semi-skilled Workers: </b>'.$min_wage_info['semiskilled_desc'].'.';}
	if(!empty($min_wage_info['skilled_desc'])){$emp_clarification .= '<br><b>Skilled Workers: </b>'.$min_wage_info['skilled_desc'].'.';}
	if(!empty($min_wage_info['highlyskilled_desc'])){$emp_clarification .= '<br><b>Highly Skilled Workers: </b>'.$min_wage_info['highlyskilled_desc'].'.</li>';}
}

if($min_wage_info['flip_option']== 1){
	$output .= $emp_clarification.$implementing_area;
}else{
	$output .= $implementing_area.$emp_clarification;
}

/************ Page Break ******************/
if($min_wage_info['implementing_area'] == 2 && $min_wage_info['worker_type']== ''){
	$p_break_start = '<p style="page-break-after:always">'; 
	$p_break_end = '</p>';
}

if(trim($min_wage_info['employment_category']) == 'CINM'){
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
if(trim($min_wage_info['employment_category']) == 'REFI'){
	$output .= '<b> NOTES :- </b> <br>
						1. To arrive at daily rates of wages, the monthly rates may be divided by 26 and rounded off to the nearest paise.<br>
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
}else{	
	
	$output .= '<li> To arrive at daily rates, monthly rates shall have to be divided by 26 and to be rounded off to the nearest rupee and to arrive at weekly rates, daily rates shall have to be multiplied by 6.</li>
	<li> A normal working day shall consist of eight and half hours of work including interval for half-an-hour for rest. </lip>
	<li> One day in any period of seven days as may suit the local convenience shall be the day of weekly rest. </li>
	<li> The minimum rates of wages include the wages for weekly day of rest.  Payment for work done on the day of weekly rest and for work done beyond the normal working hours shall be double the ordinary rate of wages. </li>
	<li> Where the existing rates of wages of any employee based on contract or agreement or otherwise are higher than the rates notified herein, the higher rates shall be protected and treated as minimum rates of wages applicable for the purpose of this notification to such employees. </li>
	<li> The minimum rates of wages are applicable to employees employed by contractors also. </li> 
	<li> The  minimum  rates  of  wages  for  disabled  persons  shall  be  same  as  payable  to  the  workers  of  appropriate category. </li>
	<li> The men and women employees shall get the same rates of wages for the same work or work of similar nature. </li>
	<li> The  minimum rates  of  wages  and  Variable  Dearness  Allowance,  if  any,  both together shall constitute  the  minimum rates of wages to be be enforceable under the Minimum Wages Act, 1948(11 of 1948).</li>';
	if(trim($min_wage_info['employment_category'])== 'BLP'){
		$output.= '<li>Piece rates of work, if there be any, shall in no case be less than the daily minimum time rates of wages.</li>
				   <li>For each principal meal, if supplied by the employer, the daily rates of minimum wages
shall be reduced by Rs. 5.37.</li>';
	}
	if(trim($min_wage_info['employment_category'])== 'AGR' || trim($min_wage_info['employment_category'])== 'RICE'){
		$output.= '<li>Where the practice of payment partly in cash and partly in produce is prevalent, the total value of produce supplied plus the cash wages paid shall not be less than the minimum wages.</li>';
	}else if(trim($min_wage_info['employment_category']) == 'BRMF'){
		$output.= '<li>The daily earnings of a piece rated employee shall not be less than the rate for a daily rated employee.</li>';
	}
	    $output.= '<p>This is issued with due approval of the Labour Commissioner, West Bengal. </p>';
	}
 /*}else if($type_of_sch_emp == '2'){
	
	$output .= '<p><b>The classification of employees is as follows:- </b> 
				<p><b>Unskilled Workers : </b> Coolie of all types such as, Coolie for loading, unloading, stacking, marking, boiling, bagging, unpacking, weighing, drying paddy, sewing, Haller-Coolie, Chatal Coolie, Peon, Bearer, Durwan and similar other workers.</p>
				<p><b>Semi-skilled workers:</b> Hushman, Khalasi and similar other workers.</p>
				<p><b>Skilled workers:</b> Mistry, Mechanic, Fitter, Tindal or Engine Driver, Machineman or Hallerman, Fireman, Boiler Attendant and similar other workers.</p>
				<p><b>Highly Skilled workers:</b> Sardars, Sardarnies, Supervisors, Accountant, Cashier and similar other workers.</p>
				<ol type="a">
				  <li>There will be only one rate of minimum wage for the whole of West Bengal.</li>
				  <li>To arrive at daily rates, monthly rates will have to be divided by 26 and to arrive at weekly rates, daily rates will have to be multiplied by 6.</li>
				  <li>A normal working day shall consist of eight and half hours of work including interval for half-an-hour for rest.</li>
				  <li>One day in any period of seven days as may suit the local convenience shall be the day of weekly rest.</li>
				  <li>The minimum rates of wages include the wages for weekly day of rest.  Payment for work done on the day of weekly rest and for work done beyond the normal working hours shall be double the ordinary rate of wages</li>
				  <li>Where the existing rates of wages of any employee based on contract or agreement or otherwise are higher than the rates notified herein, the higher rates shall be protected and treated as minimum rates of wages applicable for the purpose of this notification to such employees.</li>
				  <li>The minimum rates of wages are applicable to employees employed by contractors also.</li>
				  <li>The minimum rates of wages for disabled persons shall be same as payable to the workers of appropriate category.</li>
				  <li>The men and women employees shall get the same rates of wages for the same work or work of similar nature.</li>
				  <li>The minimum rates of wages and Variable Dearness Allowance, if any, both constitute the minimum rates of wages to be enforceable under the Minimum Wages Act, 1948 (11 of 1948).</li>
				  <li>Where the practice of payment partly in cash and partly in produce is prevalent, the total value of produce supplied plus the cash wages paid shall not be less than the minimum wages.</li>
				</ol>';
}*/
$output .= '
	<div style="overflow:hidden;margin:50px 0 40px 0;"><div style="float:right;width:40%;text-align:center;">(Heera Lal Roy)<br>Joint Labour Commissioner (P),<br>West Bengal</div></div><br>
	<div style="overflow:hidden;margin:0 0 20px 0;"><div style="float:left;width:60%;">MEMO. NO. &emsp;'.$min_wage_info['publish_memo_no'].$min_wage_info['memo_no'].'</div><div style="float:right;width:30%;text-align:right;">Dated: 27/12/2019</div></div>
	<div style="overflow:hidden;margin:0 0 6px 0;"><div style="float:left;width:3%;">1.</div><div style="float:right;width:95%;text-align:left;">The Additional Chief Secretary, Labour Department, New Secretariat Buildings, 12th floor, 1, K.S. Roy Road, Kolkata –700 001.</div></div>
	<div style="overflow:hidden;margin:0 0 6px 0;"><div style="float:left;width:3%;">2.</div><div style="float:right;width:95%;text-align:left;">The  Labour Commissioner,  W.B.,  N.S.  Bldgs.,  11th floor,  Kol.-1. </div></div>
	<div style="overflow:hidden;margin:0 0 6px 0;"><div style="float:left;width:3%;">3.</div><div style="float:right;width:95%;text-align:left;">The  Secretary  (Central  Trade  Unions/All  Chambers  of  Commerce) ________________________ with the request to circulate this among his constituents.</div></div>
	<div style="overflow:hidden;margin:0 0 6px 0;"><div style="float:left;width:3%;">4.</div><div style="float:right;width:95%;text-align:left;">The Joint/Deputy/Assistant  Labour  Commissioner ______________________ with  the  request  to circulate this among the Inspectors, M.W. under his jurisdiction.</div></div>
	<div style="overflow:hidden;margin:50px 0 20px 0;"><div style="float:right;width:40%;text-align:center;">(Heera Lal Roy)<br>Joint Labour Commissioner (P),<br>West Bengal</div></div>
	';
	return pdf_using_mpdf_api($output, $name_of_employment);
}
?>