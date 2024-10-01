<?php

function get_licence_report_result($action =''){
	global $base_root, $base_path;
	$clralicencequery = "
			select d.district_code , district_name, noappli, nobckappli, noappappli, nopaymtappli , noapprowappli , nopenappli, noinspappli , nofrwappli , nofrmiappli , noissuappli , norejappli
	from (
		select district_code, district_name
		from district_master
		group by district_code, district_name
		order by district_name
	) d
	left join (
		select count(*) noappli, district_code 
		from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d
		where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code
		and pa.contractor_user_id is not null 
		and la.application_id is not null 
		and la.is_backlog_license is null 
		and la.status is not null
		and la.final_status = 'F'  
		group by district_code
	) as totalAppli on totalAppli.district_code = d.district_code
	left join(
		select count(*) nobckappli, district_code 
		from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d
		where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code
		and pa.contractor_user_id is not null 
		and la.application_id is not null 
		and la.is_backlog_license is null 
		and la.status is not null
		and la.final_status = 'F'
		and la.status = 'B' 
		group by district_code
	) as backAppli on backAppli.district_code = d.district_code
	left join(
		select count(*) noappappli, district_code 
		from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d
		where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code
		and pa.contractor_user_id is not null 
		and la.application_id is not null 
		and la.is_backlog_license is null 
		and la.status is not null
		and la.final_status = 'F'
		and la.status = 'A' 
		group by district_code
	) as approAppli on approAppli.district_code = d.district_code
	left join(
		select count(*) nopaymtappli, district_code 
		from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d
		where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code
		and pa.contractor_user_id is not null 
		and la.application_id is not null 
		and la.is_backlog_license is null 
		and la.status is not null
		and la.final_status = 'F'
		and la.status = 'P' 
		group by district_code
	) as paymtAppli on paymtAppli.district_code = d.district_code
	left join(
		select count(*) noapprowappli, district_code 
		from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d
		where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code
		and pa.contractor_user_id is not null 
		and la.application_id is not null 
		and la.is_backlog_license is null 
		and la.status is not null
		and la.final_status = 'F'
		and la.status = 'AW' 
		group by district_code
	) as approwAppli on approwAppli.district_code = d.district_code
	left join(
		select count(*) noinspappli, district_code 
		from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d
		where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code
		and pa.contractor_user_id is not null 
		and la.application_id is not null 
		and la.is_backlog_license is null 
		and la.status is not null
		and la.final_status = 'F'
		and la.status = 'BI' 
		group by district_code
	) as inspAppli on inspAppli.district_code = d.district_code
	left join(
		select count(*) nopenappli, district_code 
		from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d
		where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code
		and pa.contractor_user_id is not null 
		and la.application_id is not null 
		and la.is_backlog_license is null 
		and la.status is not null
		and la.final_status = 'F'
		and la.status = 'F' 
		group by district_code
	) as penAppli on penAppli.district_code = d.district_code
	left join(
		select count(*) nofrwappli, district_code 
		from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d
		where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code
		and pa.contractor_user_id is not null 
		and la.application_id is not null 
		and la.is_backlog_license is null 
		and la.status is not null
		and la.final_status = 'F'
		and la.status = 'FW' 
		group by district_code
	) as frwAppli on frwAppli.district_code = d.district_code
	left join(
		select count(*) nofrmiappli, district_code 
		from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d
		where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code
		and pa.contractor_user_id is not null 
		and la.application_id is not null 
		and la.is_backlog_license is null 
		and la.status is not null
		and la.final_status = 'F'
		and la.status = 'U' 
		group by district_code
	) as frmiAppli on frmiAppli.district_code = d.district_code
	left join(
		select count(*) noissuappli, district_code 
		from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d
		where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code
		and pa.contractor_user_id is not null 
		and la.application_id is not null 
		and la.is_backlog_license is null 
		and la.status is not null
		and la.final_status = 'F'
		and la.status = 'I' 
		group by district_code
	) as issuAppli on issuAppli.district_code = d.district_code
	left join(
		select count(*) norejappli, district_code 
		from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d
		where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code
		and pa.contractor_user_id is not null 
		and la.application_id is not null 
		and la.is_backlog_license is null 
		and la.status is not null
		and la.final_status = 'F'
		and la.status = 'R' 
		group by district_code
	) as rejAppli on rejAppli.district_code = d.district_code
	order by district_name
	";
	$all_clra_licence_result = db_query($clralicencequery);
	
	$body = '<tbody>';
	
	if(!empty($all_clra_licence_result)){
		$total_application = 0;
		$total_citizen_pending_application = 0;
		$total_office_pen_application = 0;
		$total_final_submit = 0;
		$total_issue = 0;
		$total_reject = 0;
		
		
		foreach($all_clra_licence_result as $row){
			
			($row->noappli != '') ? $noappli = $row->noappli : $noappli = 0;
			$total_application = $total_application + $noappli;
			
			$sub_total_citizen_pending = 0;
			($row->nobckappli != '') ? $nobckappli = $row->nobckappli : $nobckappli = 0;
			($row->noappappli != '') ? $noappappli = $row->noappappli : $noappappli = 0;
			($row->nopaymtappli != '') ? $nopaymtappli = $row->nopaymtappli : $nopaymtappli = 0;
			($row->noapprowappli != '') ? $noapprowappli = $row->noapprowappli : $noapprowappli = 0;
			
			$sub_total_citizen_pending = ($nobckappli + $noappappli + $nopaymtappli + $noapprowappli);
			$total_citizen_pending_application = $total_citizen_pending_application + $sub_total_citizen_pending;
			
			($row->nopenappli != '') ? $nopenappli = $row->nopenappli : $nopenappli = 0;
			($row->noinspappli != '') ? $noinspappli = $row->noinspappli : $noinspappli = 0;
			($row->nofrwappli != '') ? $nofrwappli = $row->nofrwappli : $nofrwappli = 0;
			
			
			$sub_total_office_pending = 0;
			$sub_total_office_pending = ($nopenappli + $noinspappli + $nofrwappli); 
			$total_office_pen_application = $total_office_pen_application + $sub_total_office_pending;
			
			
			($row->nofrmiappli != '') ? $nofrmiappli = $row->nofrmiappli : $nofrmiappli = 0;
			$total_final_submit = $total_final_submit + $nofrmiappli;
			
			($row->noissuappli != '') ? $noissuappli = $row->noissuappli : $noissuappli = 0;
			$total_issue = $total_issue + $noissuappli;
			
			($row->norejappli != '') ? $norejappli = $row->norejappli : $norejappli = 0;
			$total_reject = $total_reject + $norejappli;
			
			$cc=$base_root.$base_path.'clralincnreport/district-wise-report/'.encryption_decryption_fun('encrypt',$row->district_code);
			
			if($action == 'print'){
				$link_dis_report = strtoupper($row->district_name);
			}else{
				$link_dis_report=l('<font color="black"><b>'.strtoupper($row->district_name).'</b></font>', $cc,array('html'=>TRUE)) ;
			}
			
			$tooltipdataC = 'Back For Correction : '.$nobckappli.';&#013; Verified & Approved :'.$noapprowappli.';&#013; Fees Pending : '.$noappappli.';&#013; Fees Paid : '.$nopaymtappli;
			$tooltipdataO = 'Pending : '.$nopenappli.';&#013; Forward To ALC : '.$nofrwappli.';&#013; Back To Insp : '.$noinspappli;			
			
			
			$body.= "<tr>";
			$body.= "<td align='left'>".$link_dis_report."</td>";
			$body.= "<td align='center'>".$noappli."</td>";		
			$body.= "<td align='center'><a href='#' data-toggle='tooltip' data-placement='right' title='".$tooltipdataC."'>".$sub_total_citizen_pending."</a></td>";		
			$body.= "<td align='center'><a href='#' data-toggle='tooltip' data-placement='right' title='".$tooltipdataO."'>".$sub_total_office_pending."</a></td>";
			$body.= "<td align='center'>".$nofrmiappli."</td>";
			$body.= "<td align='center'>".$noissuappli."</td>";
			$body.= "<td align='center' style='border-right: 1px solid #d1d1d1;'>".$norejappli."</td>";		
			$body.= "</tr>";
			
		}
		
		$body.= '</tbody>
				<tfoot>
					<tr>
						<td style="border-bottom: 1px solid #d1d1d1;"><strong>TOTAL</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_application.'</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_citizen_pending_application.'</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_office_pen_application.'</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_final_submit.'</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_issue.'</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_reject.'</td>
					</tr>
				</tfoot>';	
	}
	
	return $body;
}

function clra_license_district_report($form, &$form_state){
	global $base_root, $base_path;
	$form['clralicensefields'] = array(
		'#title' => t(''),
		'#type' => 'fieldset',
		'#collapsed' => FALSE
	);
	
	$back_url = $base_root.$base_path.'labourreport';
    $back_link = l(t('BACK <i class="fa fa-angle-double-right" aria-hidden="true"></i>'), $back_url,array('html'=>TRUE)) ;
	
	$print = l(t('<i class="fa fa-print fa" title="Click here to download details"></i> PRINT'), 'clralincnreport/print', array('html' => TRUE, 'attributes' => array('target' => '_blank'))); 
	
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
	$output .= get_licence_report_result();
	$output.='</table>
	  	</div>
		<div class="back">'.$back_link.'</div>';		
	$form['clralicensefields']['showresult'] = array(
					'#markup'=>$output,
	);	
	
	return $form;
	
}

function clra_license_district_report_print($form, &$form_state){
	global $base_root, $base_path;
	
	$output = '<style>			
			.clra_report tr td{border: 1px solid #d1d1d1; padding: 6px 8px; font-size: 8pt;}			
			.clra_report tr th{background: #ddd;border: 1px solid #d1d1d1; padding: 5px; font-size: 5pt;}
			.note_text{padding-top: 6px; font-size:11px;}
			</style>
			<table width="100%" border="0" cellspacing="0" cellpadding="0" class="clra_report">
				<thead>
					 <tr><td colspan="9" align="center" style="font-size:22px; border:0px;line-height:24px;padding-top:2px;padding-bottom:2px;"><strong>LABOUR COMMISSIONERATE</strong></td></tr>
					 <tr><td align="center" colspan="9" align="center" style="font-size:18px; border:0px;line-height:20px;padding-top:2px;padding-bottom:2px;">GOVERNMENT OF WEST BENGAL</td></tr>
					 <tr><td align="center" colspan="9" align="center" style="font-size:12px; border:0px;line-height:14px;padding-top:2px;padding-bottom:12px;"><i>Contractor license(CLRA) application status on '.date("dS M Y").' at '.date("h:m A").'</i></td></tr>
					 
					 
					 <tr>';
	$header = common_header('district');
	if(!empty($header)){
		foreach($header as $val){
			$output .= '<th style="border-right: 1px solid #d1d1d1; font-size: 14px;" > '.$val.' </th>';		
		}
	}					
	$output .='</tr> 
		</thead>';	
	
	$output .= get_licence_report_result('print');
	
	$output.='</table>'; 
		
	
		
	return pdf_using_mpdf_api($output, 'district-wise-application-status-licence-'.date("d-m-Y-h-m-s"));
}

function get_licence_subdivision_report_result($disid ='',$action = ''){
	global $base_root, $base_path;
	$district_code = encryption_decryption_fun('decrypt',$disid);
	
	$clralicencesubdivquery = "
			select  sd.sub_div_code, sd.sub_div_name , noappli, nobckappli, noappappli, nopaymtappli, noapprowappli, noinspappli, nopenappli, nofrwappli, nofrmiappli, noissuappli, norejappli
from (
	select district_master.district_code, sub_div_code, sub_div_name
	from district_master , sub_division
	where district_master.district_code=sub_division.district_code and district_master.district_code = '".$district_code."'
	group by district_master.district_code , sub_div_code, sub_div_name
	order by sub_div_name 
) sd
left join (
	select count(*) noappli, sub_div_code 
	from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d, sub_division sd
	where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code and pa.worksite_subdivision = sd.sub_div_code
	and pa.worksite_dist = '".$district_code."'
	and pa.contractor_user_id is not null 
	and la.application_id is not null 
	and la.is_backlog_license is null 
	and la.status is not null
	and la.final_status = 'F'
	group by sub_div_code
) as totalAppli on totalAppli.sub_div_code = sd.sub_div_code
left join(
	select count(*) nobckappli, sub_div_code 
	from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d, sub_division sd
	where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code and pa.worksite_subdivision = sd.sub_div_code
	and pa.worksite_dist = '".$district_code."'
	and pa.contractor_user_id is not null 
	and la.application_id is not null 
	and la.is_backlog_license is null 
	and la.status is not null
	and la.final_status = 'F'
	and la.status = 'B' 
	group by sub_div_code
) as backAppli on backAppli.sub_div_code = sd.sub_div_code
left join(
	select count(*) noappappli, sub_div_code 
	from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d, sub_division sd
	where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code and pa.worksite_subdivision = sd.sub_div_code
	and pa.worksite_dist = '".$district_code."'
	and pa.contractor_user_id is not null 
	and la.application_id is not null 
	and la.is_backlog_license is null 
	and la.status is not null
	and la.final_status = 'F'
	and la.status = 'A' 
	group by sub_div_code
) as approAppli on approAppli.sub_div_code = sd.sub_div_code
left join(
	select count(*) nopaymtappli, sub_div_code 
	from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d, sub_division sd
	where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code and pa.worksite_subdivision = sd.sub_div_code
	and pa.worksite_dist = '".$district_code."'
	and pa.contractor_user_id is not null 
	and la.application_id is not null 
	and la.is_backlog_license is null 
	and la.status is not null
	and la.final_status = 'F'
	and la.status = 'P' 
	group by sub_div_code
) as paymtAppli on paymtAppli.sub_div_code = sd.sub_div_code
left join(
	select count(*) noapprowappli, sub_div_code 
	from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d, sub_division sd
	where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code and pa.worksite_subdivision = sd.sub_div_code
	and pa.worksite_dist = '".$district_code."'
	and pa.contractor_user_id is not null 
	and la.application_id is not null 
	and la.is_backlog_license is null 
	and la.status is not null
	and la.final_status = 'F'
	and la.status = 'AW' 
	group by sub_div_code
) as approwAppli on approwAppli.sub_div_code = sd.sub_div_code
left join(
	select count(*) noinspappli, sub_div_code 
	from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d, sub_division sd
	where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code and pa.worksite_subdivision = sd.sub_div_code
	and pa.worksite_dist = '".$district_code."'
	and pa.contractor_user_id is not null 
	and la.application_id is not null 
	and la.is_backlog_license is null 
	and la.status is not null
	and la.final_status = 'F'
	and la.status = 'BI' 
	group by sub_div_code
) as inspAppli on inspAppli.sub_div_code = sd.sub_div_code
left join(
	select count(*) nopenappli, sub_div_code 
	from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d, sub_division sd
	where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code and pa.worksite_subdivision = sd.sub_div_code
	and pa.worksite_dist = '".$district_code."'
	and pa.contractor_user_id is not null 
	and la.application_id is not null 
	and la.is_backlog_license is null 
	and la.status is not null
	and la.final_status = 'F'
	and la.status = 'F' 
	group by sub_div_code
) as penAppli on penAppli.sub_div_code = sd.sub_div_code
left join(
	select count(*) nofrwappli, sub_div_code 
	from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d, sub_division sd
	where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code and pa.worksite_subdivision = sd.sub_div_code
	and pa.worksite_dist = '".$district_code."'
	and pa.contractor_user_id is not null 
	and la.application_id is not null 
	and la.is_backlog_license is null 
	and la.status is not null
	and la.final_status = 'F'
	and la.status = 'FW' 
	group by sub_div_code
) as frwAppli on frwAppli.sub_div_code = sd.sub_div_code
left join(
	select count(*) nofrmiappli, sub_div_code 
	from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d, sub_division sd
	where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code and pa.worksite_subdivision = sd.sub_div_code
	and pa.worksite_dist = '".$district_code."'
	and pa.contractor_user_id is not null 
	and la.application_id is not null 
	and la.is_backlog_license is null 
	and la.status is not null
	and la.final_status = 'F'
	and la.status = 'U' 
	group by sub_div_code
) as frmiAppli on frmiAppli.sub_div_code = sd.sub_div_code
left join(
	select count(*) noissuappli, sub_div_code 
	from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d, sub_division sd
	where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code and pa.worksite_subdivision = sd.sub_div_code
	and pa.worksite_dist = '".$district_code."'
	and pa.contractor_user_id is not null 
	and la.application_id is not null 
	and la.is_backlog_license is null 
	and la.status is not null
	and la.final_status = 'F'
	and la.status = 'I' 
	group by sub_div_code
) as issuAppli on issuAppli.sub_div_code = sd.sub_div_code
left join(
	select count(*) norejappli, sub_div_code 
	from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d, sub_division sd
	where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code and pa.worksite_subdivision = sd.sub_div_code
	and pa.worksite_dist = '".$district_code."'
	and pa.contractor_user_id is not null 
	and la.application_id is not null 
	and la.is_backlog_license is null 
	and la.status is not null
	and la.final_status = 'F'
	and la.status = 'R' 
	group by sub_div_code
) as rejAppli on rejAppli.sub_div_code = sd.sub_div_code
order by sub_div_name";
	
	
	$clralicencesubdiv_result = db_query($clralicencesubdivquery);
	
	
	$body = '<tbody>';
	
	if(!empty($clralicencesubdiv_result)){
		$total_application = 0;
		$total_citizen_pending_application = 0;
		$total_office_pen_application = 0;
		$total_final_submit = 0;
		$total_issue = 0;
		$total_reject = 0;
		
		
		foreach($clralicencesubdiv_result as $row){
			
			($row->noappli != '') ? $noappli = $row->noappli : $noappli = 0;
			$total_application = $total_application + $noappli;
			
			$sub_total_citizen_pending = 0;
			($row->nobckappli != '') ? $nobckappli = $row->nobckappli : $nobckappli = 0;
			($row->noappappli != '') ? $noappappli = $row->noappappli : $noappappli = 0;
			($row->nopaymtappli != '') ? $nopaymtappli = $row->nopaymtappli : $nopaymtappli = 0;
			($row->noapprowappli != '') ? $noapprowappli = $row->noapprowappli : $noapprowappli = 0;
			
			$sub_total_citizen_pending = ($nobckappli + $noappappli + $nopaymtappli + $noapprowappli);
			$total_citizen_pending_application = $total_citizen_pending_application + $sub_total_citizen_pending;
			
			($row->nopenappli != '') ? $nopenappli = $row->nopenappli : $nopenappli = 0;
			($row->noinspappli != '') ? $noinspappli = $row->noinspappli : $noinspappli = 0;
			($row->nofrwappli != '') ? $nofrwappli = $row->nofrwappli : $nofrwappli = 0;
			
			
			$sub_total_office_pending = 0;
			$sub_total_office_pending = ($nopenappli + $noinspappli + $nofrwappli); 
			$total_office_pen_application = $total_office_pen_application + $sub_total_office_pending;
			
			
			($row->nofrmiappli != '') ? $nofrmiappli = $row->nofrmiappli : $nofrmiappli = 0;
			$total_final_submit = $total_final_submit + $nofrmiappli;
			
			($row->noissuappli != '') ? $noissuappli = $row->noissuappli : $noissuappli = 0;
			$total_issue = $total_issue + $noissuappli;
			
			($row->norejappli != '') ? $norejappli = $row->norejappli : $norejappli = 0;
			$total_reject = $total_reject + $norejappli;
			
			$tooltipdataC = 'Back For Correction : '.$nobckappli.';&#013; Verified & Approved :'.$noapprowappli.';&#013; Fees Pending : '.$noappappli.';&#013; Fees Paid : '.$nopaymtappli;
			$tooltipdataO = 'Pending : '.$nopenappli.';&#013; Forward To ALC : '.$nofrwappli.';&#013; Back To Insp : '.$noinspappli;	
			
			
			$body.= "<tr>";
			$body.= "<td align='left'>".strtoupper($row->sub_div_name)."</td>";
			$body.= "<td align='center'>".$noappli."</td>";		
			$body.= "<td align='center'><a href='#' data-toggle='tooltip' data-placement='right' title='".$tooltipdataC."'>".$sub_total_citizen_pending."</a></td>";		
			$body.= "<td align='center'><a href='#' data-toggle='tooltip' data-placement='right' title='".$tooltipdataO."'>".$sub_total_office_pending."</a></td>";
			$body.= "<td align='center'>".$nofrmiappli."</td>";
			$body.= "<td align='center'>".$noissuappli."</td>";
			$body.= "<td align='center' style='border-right: 1px solid #d1d1d1;'>".$norejappli."</td>";		
			$body.= "</tr>";
			
		}
		
		
		$body.= '</tbody>
				<tfoot>
					<tr>
						<td style="border-bottom: 1px solid #d1d1d1;"><strong>TOTAL</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_application.'</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_citizen_pending_application.'</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_office_pen_application.'</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_final_submit.'</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_issue.'</td>
						<td align="center" style="border-bottom: 1px solid #d1d1d1;"><strong>'.$total_reject.'</td>
					</tr>
				</tfoot>';	
		
	}
	
	return $body;
}


function clra_license_subdivision_report($form, &$form_state, $disid = ''){
	global $base_root, $base_path;
	$district_code = encryption_decryption_fun('decrypt',$disid);
	$getinfo = db_select('district_master','d')->fields('d',array('district_name'))->condition('d.district_code',$district_code)->execute();
	$district_name = $getinfo->fetchObject()->district_name;
		
	$form['clralicensefields'] = array(
		'#title' => t(''),
		'#type' => 'fieldset',
		'#collapsed' => FALSE
	);
		
	$back_url = $base_root.$base_path.'labourreport';
	
    $back_link = l(t('BACK <i class="fa fa-angle-double-right" aria-hidden="true"></i>'), $back_url,array('html'=>TRUE)) ;
	
	$print = l(t('<i class="fa fa-print fa" title="Click here to download details"></i> PRINT'), 'clralincnreport/district-wise-report/print/'.$disid, array('html' => TRUE, 'attributes' => array('target' => '_blank'))); 
	
	
	$output = $district_name.'<div class="back">'.$print.'&nbsp;'.$back_link.'</div>
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
	
	$output .= get_licence_subdivision_report_result($disid);
	
	
	$output.='</table>
	  	</div>
		<div class="back">'.$back_link.'</div>';
		
	
		
	$form['clralicensefields']['showresult'] = array(
					'#markup'=>$output,
	);
	
	return $form;
}

function clra_license_subdivision_report_print($form, &$form_state, $disid = ''){
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
					 <tr><td align="center" colspan="7" align="center" style="font-size:12px; border:0px;line-height:14px;padding-top:2px;padding-bottom:12px;">Application Status of '.$district_name.'</td></tr>
					 
					 <tr>';
	$header = common_header();
	if(!empty($header)){
		foreach($header as $val){
	$output .= '<th style="border-right: 1px solid #d1d1d1; font-size: 14px;" > '.$val.' </th>';		
		}
	}					
	$output .='</tr> 
		</thead>
		<tbody>';	
	
	$output .= get_licence_subdivision_report_result($disid,'print');
	
	$output.='</tbody>
			</table>'; 
			
	return pdf_using_mpdf_api($output, 'subdivision-wise-application-status-licence-'.date("d-m-Y-h-m-s"));
}


