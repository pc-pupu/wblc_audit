<?php

drupal_add_js('jQuery(document).ready(function(){
	jQuery("[data-toggle="tooltip"]").tooltip();
});', 'inline');

function get_clra_report_result($action =''){
	global $base_root, $base_path;
	
	$clraquery="
		select d.district_code,district_name, clratotal, backtotal, vnotptotal, vptotal, paidtotal, backinsptotal, frwtotal,pentotal,frmitotal,issutotal,rejtotal
from (
	select district_code, district_name
	from district_master
	group by district_code, district_name
	order by district_name 
) d
left join (
	select sum(clratotal) clratotal, district_code  
	from (
		select count(*) clratotal, district_code
		from l_clra_registration_master a,  district_master d 
		where a.loc_e_dist=d.district_code 
		and user_id is not null and user_id != 0 
		and final_submit_status = 'P' 
		and status is not null
		group by district_code  
	) v  group by district_code  
) as totalclra ON d.district_code = totalclra.district_code
left join ( 
	select sum(backtotal) backtotal, district_code  
	from(
		select count(*) backtotal, district_code 
		from l_clra_registration_master a,  district_master d  
		where a.loc_e_dist=d.district_code and user_id is not null 
		and user_id != 0 
		and final_submit_status='P' 
		and status = 'B' 
		group by district_code 
	) v  group by district_code  
) as clraback ON d.district_code = clraback.district_code
left join ( 
	select sum(vnotptotal) vnotptotal, district_code  
	from (
		select count(*) vnotptotal, district_code
		from l_clra_registration_master a, district_master d
		where a.loc_e_dist=d.district_code 
		and user_id is not null and user_id != 0 
		and final_submit_status ='P' 
		and status ='VA'
		group by district_code  
	) v  group by district_code 
) as clrawopay ON d.district_code = clrawopay.district_code
left join (
	select sum(vptotal) vptotal, district_code  
	from (
		select count(*) vptotal, district_code 
		from l_clra_registration_master a, district_master d  
		where   a.loc_e_dist=d.district_code 
		and user_id is not null and user_id != 0 
		and final_submit_status='P' 
		and status = 'V' 
		group by district_code  
	) v  group by district_code  
) as clrapay ON d.district_code = clrapay.district_code 
left join (
	select sum(paidtotal) paidtotal, district_code  
	from (
		select count(*) paidtotal, district_code
		from l_clra_registration_master a,  district_master d
		where a.loc_e_dist=d.district_code 
		and user_id is not null and user_id != 0 
		and final_submit_status='P' 
		and status = 'T'
		group by district_code
	) v  group by district_code 
) as clrapaid ON d.district_code = clrapaid.district_code
left join (
	select sum(backinsptotal) backinsptotal, district_code  
	from (
		select count(*) backinsptotal, district_code 
		from l_clra_registration_master a, district_master d  
		where   a.loc_e_dist=d.district_code 
		and user_id is not null and user_id != 0 
		and final_submit_status='P' 
		and status = 'BI' 
		group by district_code  
	) v  group by district_code  
) as clrabackinsp ON d.district_code = clrabackinsp.district_code 
left join ( 
	select sum(frwtotal) frwtotal, district_code  
	from (
		select count(*) frwtotal, district_code
		from l_clra_registration_master a, district_master d
		where a.loc_e_dist=d.district_code 
		and user_id is not null and user_id != 0 
		and final_submit_status ='P' 
		and status ='F'
		group by district_code  
	) v  group by district_code 
) as clrafrwalc ON d.district_code = clrafrwalc.district_code
left join ( 
	select sum(pentotal) pentotal, district_code  
	from (
		select count(*) pentotal, district_code
		from l_clra_registration_master a, district_master d
		where a.loc_e_dist=d.district_code 
		and user_id is not null and user_id != 0 
		and final_submit_status ='P' 
		and status ='0'
		group by district_code  
	) v  group by district_code 
) as clrapen ON d.district_code = clrapen.district_code
left join ( 
	select sum(frmitotal) frmitotal, district_code  
	from (
		select count(*) frmitotal, district_code
		from l_clra_registration_master a, district_master d
		where a.loc_e_dist=d.district_code 
		and user_id is not null and user_id != 0 
		and final_submit_status ='P' 
		and status ='S'
		group by district_code  
	) v  group by district_code 
) as clrafrmi ON d.district_code = clrafrmi.district_code
left join ( 
	select sum(issutotal) issutotal, district_code  
	from (
		select count(*) issutotal, district_code
		from l_clra_registration_master a, district_master d
		where a.loc_e_dist=d.district_code 
		and user_id is not null and user_id != 0 
		and final_submit_status ='P' 
		and status ='I'
		group by district_code  
	) v  group by district_code 
) as clraissu ON d.district_code = clraissu.district_code
left join (
	select sum(rejtotal) rejtotal , district_code  
	from (
		select count(*) rejtotal, district_code 
		from l_clra_registration_master a, district_master d  
		where a.loc_e_dist=d.district_code 
		and user_id is not null and user_id != 0 
		and final_submit_status='P' 
		and status = 'R'
		group by district_code 
	) v  group by district_code  
) as clrarej ON d.district_code = clrarej.district_code
order by district_name";
		
	$total_clra_result = db_query($clraquery);
	
	$body = '<tbody>';
	
	if(!empty($total_clra_result)){
		$total_application = 0;
		$total_citizen_pending_application = 0;
		$total_office_pending_application = 0;
		
		$total_final_submit = 0;
		$total_issue = 0;
		$total_reject = 0;
		
		$total_backtotal = 0;
		$total_vnotptotal = 0;
		$total_vptotal = 0;
		$total_paidtotal = 0;
		
		$total_backinsptotal = 0;
		$total_frwtotal = 0;
		$total_pentotal = 0;
		
		
		foreach($total_clra_result as $data){
			
			($data->clratotal != '') ? 	$clratotal = $data->clratotal : $clratotal = 0;
			$total_application = $total_application + $clratotal;
						
			($data->backtotal != '') ? $backtotal = $data->backtotal : $backtotal = 0;
			$total_backtotal = $total_backtotal + $backtotal;
			
			($data->vnotptotal != '') ? $vnotptotal = $data->vnotptotal : $vnotptotal = 0;
			$total_vnotptotal = $total_vnotptotal + $vnotptotal;
			
			($data->vptotal != '') ? $vptotal = $data->vptotal : $vptotal = 0;
			$total_vptotal = $total_vptotal + $vptotal;
			
			($data->paidtotal != '') ? $paidtotal = $data->paidtotal : $paidtotal = 0;
			$total_paidtotal = $total_paidtotal + $paidtotal;
			
			$sub_total_citizen_pending = 0;
			$sub_total_citizen_pending = ($backtotal + $vnotptotal + $vptotal + $paidtotal);
			
			$total_citizen_pending_application = $total_citizen_pending_application + $sub_total_citizen_pending;
			
			
			($data->backinsptotal != '') ? $backinsptotal = $data->backinsptotal : $backinsptotal = 0;
			$total_backinsptotal = $total_backinsptotal + $backinsptotal;
			
			($data->frwtotal != '') ? $frwtotal = $data->frwtotal : $frwtotal = 0;
			$total_frwtotal = $total_frwtotal + $frwtotal;
			
			($data->pentotal != '') ? $pentotal = $data->pentotal : $pentotal = 0;
			$total_pentotal = $total_pentotal + $pentotal;
			
			$sub_total_office_pending = 0;
			$sub_total_office_pending = ($backinsptotal + $frwtotal + $pentotal);
			$total_office_pending_application = $total_office_pending_application + $sub_total_office_pending;
			
			($data->frmitotal != '') ? $frmitotal = $data->frmitotal : $frmitotal = 0;
			$total_final_submit = $total_final_submit + $frmitotal;
			
			($data->issutotal != '') ? $issutotal = $data->issutotal : $issutotal = 0;
			$total_issue = $total_issue + $issutotal;
			
			($data->rejtotal != '') ? $rejtotal = $data->rejtotal : $rejtotal = 0;
			$total_reject = $total_reject + $rejtotal;
			
						
			$cc = $base_root.$base_path.'clrareport/subdistrictclrarep/'.encryption_decryption_fun('encrypt',$data->district_code);
		    
			if($action == 'print'){
				$link_sub_dis_report = strtoupper($data->district_name);
			}else{
				$link_sub_dis_report = l('<font color="black"><b>'.strtoupper($data->district_name).'</b></font>', $cc,array('html'=>TRUE)) ;
			}
			
			$tooltipdataC = 'Back For Correction : '.$backtotal.'; Verified & Approved :'.$vnotptotal.'; Fees Pending : '.$vptotal.'; Fees Paid : '.$paidtotal;
			$tooltipdataO = 'Pending : '.$pentotal.'; Forward To ALC : '.$frwtotal.'; Back To Insp : '.$backinsptotal;
			
			$body.= "<tr>"; 			
			$body.= "<td align='left'>".$link_sub_dis_report."</td>";
			$body.= "<td align='center'>".$clratotal."</td>";	
			$body.= "<td align='center'><a href='#' data-toggle='tooltip' data-placement='right' title='".$tooltipdataC."'>".$sub_total_citizen_pending."</a></td>";
			$body.= "<td align='center'><a href='#' data-toggle='tooltip' data-placement='right' title='".$tooltipdataO."'>".$sub_total_office_pending."</td>";
			$body.= "<td align='center'>".$frmitotal." </td>";
			$body.= "<td align='center'>".$issutotal." </td>";
			$body.= "<td align='center'>".$rejtotal." </td>";		 
			$body.= "</tr>"; 
					
		}
		
		$body.= '</tbody>
					<tfoot>
					<tr>
						<td style="border-bottom: 1px solid #d1d1d1;"><strong>TOTAL</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_application.'</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_citizen_pending_application.'</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_office_pending_application.'</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_final_submit.'</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_issue.'</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_reject.'</td>
					</tr>
				</tfoot>';	
		
	}
	return $body;
}

function clra_district_report($form, &$form_state){
	global $base_root, $base_path;
	$form['clralistfields'] = array(
		'#title' => t(''),
		'#type' => 'fieldset',
		'#collapsed' => FALSE
	);	
	
	$back_url = $base_root.$base_path.'labourreport';
    $back_link = l(t('BACK <i class="fa fa-angle-double-right" aria-hidden="true"></i>'), $back_url,array('html'=>TRUE)) ;
	
	$print = l(t('<i class="fa fa-print fa" title="Click here to download details"></i> PRINT'), 'clrareport/print', array('html' => TRUE, 'attributes' => array('target' => '_blank'))); 
	
	$output = '
	<div class="back">'.$print.'&nbsp;'.$back_link.'</div>
		<div id="report-table" class="scrollContent content mCustomScrollbar light report-table" data-mcs-theme="minimal-dark">
		<table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table fixed_headers">
			<thead>
				 <tr>';
	$header = common_header('district');
	if(!empty($header)){
		foreach($header as $val){
	$output .= '<th>'.$val.'</th>';		
		}
	}
	$output .= '</tr>
			</thead>';	
	$output .= get_clra_report_result();
	$output.='</table>
	  	</div>
		<div class="back">'.$back_link.'</div>';		
	$form['clralistfields']['showresult'] = array(
					'#markup'=>$output,
	);	
	
	return $form;
}

function clra_district_report_print($form, &$form_state ){
	global $base_root, $base_path;
	
	$output = '<style>			
			.clra_report tr td{border: 1px solid #d1d1d1; padding: 6px 8px; font-size: 8pt;}			
			.clra_report tr th{background: #ddd;border: 1px solid #d1d1d1; padding: 5px; font-size: 5pt;}
			.note_text{padding-top: 6px; font-size:11px;}
			</style>
			<table width="100%" border="0" cellspacing="0" cellpadding="0" class="clra_report">
				<thead>
					 <tr><td colspan="7" align="center" style="font-size:22px; border:0px;line-height:24px;padding-top:2px;padding-bottom:2px;"><strong>LABOUR COMMISSIONERATE</strong></td></tr>
					 <tr><td align="center" colspan="7" align="center" style="font-size:18px; border:0px;line-height:20px;padding-top:2px;padding-bottom:2px;">GOVERNMENT OF WEST BENGAL</td></tr>
					 <tr><td align="center" colspan="7" align="center" style="font-size:12px; border:0px;line-height:14px;padding-top:2px;padding-bottom:12px;"><i>Contractor license(CLRA) application status on '.date("dS M Y").' at '.date("h:m A").'</i></td></tr>
					 <tr><td align="center" colspan="7" align="center" style="font-size:12px; border:0px;line-height:14px;padding-top:2px;padding-bottom:12px;">All District Registration / Amendment Application Status</td></tr>
					 <tr>';
	$header = common_header('district');
	if(!empty($header)){
		foreach($header as $val){
	$output .= '<th style="border-right: 1px solid #d1d1d1; font-size: 14px;" > '.$val.' </th>';		
		}
	}					
	$output .='</tr> 
		</thead>
		<tbody>';	
	
	$output .= get_clra_report_result('print');
	
	$output.='</tbody>
			</table>'; 
			
	return pdf_using_mpdf_api($output, 'district-wise-application-status-clra-'.date("d-m-Y-h-m-s"));
}

function get_clra_subdivision_report_result($disid ='',$action = ''){
	global $base_root, $base_path;
	$district_code = encryption_decryption_fun('decrypt',$disid);
	
	$clra_subdiv_query = "
		select sd.sub_div_code, sd.sub_div_name , clratotal, backtotal,vnotptotal, vptotal, paidtotal, backinsptotal, frwtotal,pentotal,frmitotal,issutotal,rejtotal
		from (
			select district_master.district_code, sub_div_code, sub_div_name
			from district_master, sub_division
			where district_master.district_code=sub_division.district_code and district_master.district_code='".$district_code."'
			group by district_master.district_code , sub_div_code, sub_div_name
			order by sub_div_name 
		)sd
		left join (
			select sum(clratotal) clratotal , sub_div_code  
			from (
				select count(*) clratotal, sub_div_code 
				from l_clra_registration_master a, district_master d, sub_division c
				where a.loc_e_dist=d.district_code and d.district_code=c.district_code and a.loc_e_subdivision=c.sub_div_code 
				and d.district_code='".$district_code."'
				and user_id is not null and user_id != 0 
				and final_submit_status = 'P' 
				and status is not null				
				group by  sub_div_code  
			) v  group by sub_div_code  
		) as totalclra ON sd.sub_div_code = totalclra.sub_div_code
		left join (
			select sum(backtotal) backtotal , sub_div_code  
			from (
				select count(*) backtotal, sub_div_code 
				from l_clra_registration_master a, district_master d, sub_division c
				where a.loc_e_dist=d.district_code and d.district_code=c.district_code and a.loc_e_subdivision=c.sub_div_code 
				and d.district_code='".$district_code."'
				and user_id is not null and user_id != 0 
				and final_submit_status = 'P' 
				and status = 'B'				
				group by  sub_div_code  
			) v  group by sub_div_code  
		) as clraback ON sd.sub_div_code = clraback.sub_div_code
		left join (
			select sum(vnotptotal) vnotptotal , sub_div_code  
			from (
				select count(*) vnotptotal, sub_div_code 
				from l_clra_registration_master a, district_master d, sub_division c
				where a.loc_e_dist=d.district_code and d.district_code=c.district_code and a.loc_e_subdivision=c.sub_div_code 
				and d.district_code='".$district_code."'
				and user_id is not null and user_id != 0 
				and final_submit_status = 'P' 
				and status = 'VA'				
				group by  sub_div_code  
			) v  group by sub_div_code  
		) as clrawopay ON sd.sub_div_code = clrawopay.sub_div_code
		left join (
			select sum(vptotal) vptotal , sub_div_code  
			from (
				select count(*) vptotal, sub_div_code 
				from l_clra_registration_master a, district_master d, sub_division c
				where a.loc_e_dist=d.district_code and d.district_code=c.district_code and a.loc_e_subdivision=c.sub_div_code 
				and d.district_code='".$district_code."'
				and user_id is not null and user_id != 0 
				and final_submit_status = 'P' 
				and status = 'V'				
				group by  sub_div_code  
			) v  group by sub_div_code  
		) as clrapay ON sd.sub_div_code = clrapay.sub_div_code
		left join (
			select sum(paidtotal) paidtotal , sub_div_code  
			from (
				select count(*) paidtotal, sub_div_code 
				from l_clra_registration_master a, district_master d, sub_division c
				where a.loc_e_dist=d.district_code and d.district_code=c.district_code and a.loc_e_subdivision=c.sub_div_code 
				and d.district_code='".$district_code."'
				and user_id is not null and user_id != 0 
				and final_submit_status = 'P' 
				and status = 'T'				
				group by  sub_div_code  
			) v  group by sub_div_code  
		) as clrapaid ON sd.sub_div_code = clrapaid.sub_div_code
		left join (
			select sum(backinsptotal) backinsptotal , sub_div_code  
			from (
				select count(*) backinsptotal, sub_div_code 
				from l_clra_registration_master a, district_master d, sub_division c
				where a.loc_e_dist=d.district_code and d.district_code=c.district_code and a.loc_e_subdivision=c.sub_div_code 
				and d.district_code='".$district_code."'
				and user_id is not null and user_id != 0 
				and final_submit_status = 'P' 
				and status = 'BI'				
				group by  sub_div_code  
			) v  group by sub_div_code  
		) as clrabackinsp ON sd.sub_div_code = clrabackinsp.sub_div_code
		left join (
			select sum(frwtotal) frwtotal , sub_div_code  
			from (
				select count(*) frwtotal, sub_div_code 
				from l_clra_registration_master a, district_master d, sub_division c
				where a.loc_e_dist=d.district_code and d.district_code=c.district_code and a.loc_e_subdivision=c.sub_div_code 
				and d.district_code='".$district_code."'
				and user_id is not null and user_id != 0 
				and final_submit_status = 'P' 
				and status = 'F'				
				group by  sub_div_code  
			) v  group by sub_div_code  
		) as clrafrwalc ON sd.sub_div_code = clrafrwalc.sub_div_code
		left join (
			select sum(pentotal) pentotal , sub_div_code  
			from (
				select count(*) pentotal, sub_div_code 
				from l_clra_registration_master a, district_master d, sub_division c
				where a.loc_e_dist=d.district_code and d.district_code=c.district_code and a.loc_e_subdivision=c.sub_div_code 
				and d.district_code='".$district_code."'
				and user_id is not null and user_id != 0 
				and final_submit_status = 'P' 
				and status = '0'				
				group by  sub_div_code  
			) v  group by sub_div_code  
		) as clrapen ON sd.sub_div_code = clrapen.sub_div_code
		left join (
			select sum(frmitotal) frmitotal , sub_div_code  
			from (
				select count(*) frmitotal, sub_div_code 
				from l_clra_registration_master a, district_master d, sub_division c
				where a.loc_e_dist=d.district_code and d.district_code=c.district_code and a.loc_e_subdivision=c.sub_div_code 
				and d.district_code='".$district_code."'
				and user_id is not null and user_id != 0 
				and final_submit_status = 'P' 
				and status = 'S'				
				group by  sub_div_code  
			) v  group by sub_div_code  
		) as clrafrmi ON sd.sub_div_code = clrafrmi.sub_div_code
		left join (
			select sum(issutotal) issutotal , sub_div_code  
			from (
				select count(*) issutotal, sub_div_code 
				from l_clra_registration_master a, district_master d, sub_division c
				where a.loc_e_dist=d.district_code and d.district_code=c.district_code and a.loc_e_subdivision=c.sub_div_code 
				and d.district_code='".$district_code."'
				and user_id is not null and user_id != 0 
				and final_submit_status = 'P' 
				and status = 'I'				
				group by  sub_div_code  
			) v  group by sub_div_code  
		) as clraissu ON sd.sub_div_code = clraissu.sub_div_code
		left join (
			select sum(rejtotal) rejtotal , sub_div_code  
			from (
				select count(*) rejtotal, sub_div_code 
				from l_clra_registration_master a, district_master d, sub_division c
				where a.loc_e_dist=d.district_code and d.district_code=c.district_code and a.loc_e_subdivision=c.sub_div_code 
				and d.district_code='".$district_code."'
				and user_id is not null and user_id != 0 
				and final_submit_status = 'P' 
				and status = 'R'				
				group by  sub_div_code  
			) v  group by sub_div_code  
		) as clrarej ON sd.sub_div_code = clrarej.sub_div_code
		order by sub_div_name";
	
	$total_clra_result = db_query($clra_subdiv_query);
	
	$body = '<tbody>';
		
	if(!empty($total_clra_result)){
		$total_application = 0;
		$total_citizen_pending_application = 0;
		$total_office_pending_application = 0;
		
		$total_final_submit = 0;
		$total_issue = 0;
		$total_reject = 0;
		
		$total_backtotal = 0;
		$total_vnotptotal = 0;
		$total_vptotal = 0;
		$total_paidtotal = 0;
		
		$total_backinsptotal = 0;
		$total_frwtotal = 0;
		$total_pentotal = 0;
		
		
		foreach($total_clra_result as $data){
			
			($data->clratotal != '') ? 	$clratotal = $data->clratotal : $clratotal = 0;
			$total_application = $total_application + $clratotal;
						
			($data->backtotal != '') ? $backtotal = $data->backtotal : $backtotal = 0;
			$total_backtotal = $total_backtotal + $backtotal;
			
			($data->vnotptotal != '') ? $vnotptotal = $data->vnotptotal : $vnotptotal = 0;
			$total_vnotptotal = $total_vnotptotal + $vnotptotal;
			
			($data->vptotal != '') ? $vptotal = $data->vptotal : $vptotal = 0;
			$total_vptotal = $total_vptotal + $vptotal;
			
			($data->paidtotal != '') ? $paidtotal = $data->paidtotal : $paidtotal = 0;
			$total_paidtotal = $total_paidtotal + $paidtotal;
			
			$sub_total_citizen_pending = 0;
			$sub_total_citizen_pending = ($backtotal + $vnotptotal + $vptotal + $paidtotal);
			
			$total_citizen_pending_application = $total_citizen_pending_application + $sub_total_citizen_pending;
			
			
			($data->backinsptotal != '') ? $backinsptotal = $data->backinsptotal : $backinsptotal = 0;
			$total_backinsptotal = $total_backinsptotal + $backinsptotal;
			
			($data->frwtotal != '') ? $frwtotal = $data->frwtotal : $frwtotal = 0;
			$total_frwtotal = $total_frwtotal + $frwtotal;
			
			($data->pentotal != '') ? $pentotal = $data->pentotal : $pentotal = 0;
			$total_pentotal = $total_pentotal + $pentotal;
			
			$sub_total_office_pending = 0;
			$sub_total_office_pending = ($backinsptotal + $frwtotal + $pentotal);
			$total_office_pending_application = $total_office_pending_application + $sub_total_office_pending;
			
			($data->frmitotal != '') ? $frmitotal = $data->frmitotal : $frmitotal = 0;
			$total_final_submit = $total_final_submit + $frmitotal;
			
			($data->issutotal != '') ? $issutotal = $data->issutotal : $issutotal = 0;
			$total_issue = $total_issue + $issutotal;
			
			($data->rejtotal != '') ? $rejtotal = $data->rejtotal : $rejtotal = 0;
			$total_reject = $total_reject + $rejtotal;
			
			
			$tooltipdataC = 'Back For Correction : '.$backtotal.'; Verified & Approved :'.$vnotptotal.'; Fees Pending : '.$vptotal.'; Fees Paid : '.$paidtotal;
			$tooltipdataO = 'Pending : '.$pentotal.'; Forward To ALC : '.$frwtotal.'; Back To Insp : '.$backinsptotal;
					
			$body.= "<tr>"; 			
			$body.= "<td align='left'>".strtoupper($data->sub_div_name)."</td>";
			$body.= "<td align='center'>".$clratotal."</td>";	
			$body.= "<td align='center'><a href='#' data-toggle='tooltip' data-placement='right' title='".$tooltipdataC."'>".$sub_total_citizen_pending."</a></td>";
			$body.= "<td align='center'><a href='#' data-toggle='tooltip' data-placement='right' title='".$tooltipdataO."'>".$sub_total_office_pending."</a></td>";
			$body.= "<td align='center'>".$frmitotal." </td>";
			$body.= "<td align='center'>".$issutotal." </td>";
			$body.= "<td align='center'>".$rejtotal." </td>";		 
			$body.= "</tr>"; 		
		}
		
		$body.= '</tbody>
					<tfoot>
					<tr>
						<td style="border-bottom: 1px solid #d1d1d1;"><strong>TOTAL</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_application.'</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_citizen_pending_application.'</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_office_pending_application.'</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_final_submit.'</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_issue.'</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_reject.'</td>
					</tr>
				</tfoot>';	
		
	}
	return $body;
}

function sub_district_clra_report($form, &$form_state , $disid = ''){
	global $base_root, $base_path;
	
	$district_code = encryption_decryption_fun('decrypt',$disid);
	
	$getinfo = db_select('district_master','d')->fields('d',array('district_name'))->condition('d.district_code',$district_code)->execute();
	$district_name = $getinfo->fetchObject()->district_name;
	
	
	$form['clrafields'] = array(
		'#title' => t(''),
		'#type' => 'fieldset',
		'#collapsed' => FALSE
	);
	
	$back_url = $base_root.$base_path.'labourreport';
	
    $back_link = l(t('BACK <i class="fa fa-angle-double-right" aria-hidden="true"></i>'), $back_url,array('html'=>TRUE)) ;
	
	$print = l(t('<i class="fa fa-print fa" title="Click here to download details"></i> PRINT'), 'clrareport/subdistrictclrarep/print/'.$disid, array('html' => TRUE, 'attributes' => array('target' => '_blank'))); 
	
	
	$output = $district_name.': <div class="back">'.$print.'&nbsp;'.$back_link.'</div>
		<div id="report-table" class="scrollContent content mCustomScrollbar light report-table" data-mcs-theme="minimal-dark">
		<table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table fixed_headers">
			<thead>
				 <tr>';
	$header = common_header();
	if(!empty($header)){
		foreach($header as $val){
	$output .= '<th>'.$val.'</th>';		
		}
	}
	
	$output .= '</tr>
			</thead>';	
	
	$output .= get_clra_subdivision_report_result($disid);
	
	
	$output.='</table>
	  	</div>
		<div class="back">'.$back_link.'</div>';
		
	
		
	$form['clrafields']['showresult'] = array(
		'#markup' => $output,
	);
	
	return $form;
}

function sub_district_clra_report_print($form, &$form_state , $disid = ''){
	global $base_root, $base_path;
	
	$district_code = encryption_decryption_fun('decrypt',$disid);
	
	$getinfo = db_select('district_master','d')->fields('d',array('district_name'))->condition('d.district_code',$district_code)->execute();
	$district_name = $getinfo->fetchObject()->district_name;
	
	$output = '<style>			
			.clra_report tr td{border: 1px solid #d1d1d1; padding: 6px 8px; font-size: 8pt;}			
			.clra_report tr th{background: #ddd;border: 1px solid #d1d1d1; padding: 5px; font-size: 5pt;}
			.note_text{padding-top: 6px; font-size:11px;}
			</style>
			<table width="100%" border="0" cellspacing="0" cellpadding="0" class="clra_report">
				<thead>
					 <tr><td colspan="7" align="center" style="font-size:22px; border:0px;line-height:24px;padding-top:2px;padding-bottom:2px;"><strong>LABOUR COMMISSIONERATE</strong></td></tr>
					 <tr><td align="center" colspan="7" align="center" style="font-size:18px; border:0px;line-height:20px;padding-top:2px;padding-bottom:2px;">GOVERNMENT OF WEST BENGAL</td></tr>
					 <tr><td align="center" colspan="7" align="center" style="font-size:12px; border:0px;line-height:14px;padding-top:2px;padding-bottom:12px;"><i>Contractor license(CLRA) application status on '.date("dS M Y").' at '.date("h:m A").'</i></td></tr>
					 <tr><td align="center" colspan="7" align="center" style="font-size:12px; border:0px;line-height:14px;padding-top:2px;padding-bottom:12px;">Registration / Amendment Application Status of '.$district_name.'</td></tr>
					 
					 <tr>';
	$header = common_header();
	if(!empty($header)){
		foreach($header as $val){
	$output .= '<th style="border-right: 1px solid #d1d1d1; font-size: 14px;"> '.$val.' </th>';		
		}
	}					
	$output .='</tr> 
		</thead>
		<tbody>';	
	
	$output .= get_clra_subdivision_report_result($disid,'print');
	
	$output.='</tbody>
			</table>'; 
			
	return pdf_using_mpdf_api($output, 'subdivision-wise-application-status-clra-'.date("d-m-Y-h-m-s"));
}