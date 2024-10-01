<?php

function mtw_common_pdf_generator($application_id='',$u_id='', $renewal_id = ''){
	global $base_root, $base_path, $user;
		$application_id		= encryption_decryption_fun('decrypt', $application_id);
		$u_id				= encryption_decryption_fun('decrypt', $u_id);
		$renewal_id = encryption_decryption_fun('decrypt', $renewal_id);
		
		if($u_id=='self'){
			$user_id 			= $user->uid;
		}else{
			$user_id 			= $u_id;
		}
				
		$master_part_mtw_reg = db_select('l_mtw_registration_master', 'lmrm');
		$master_part_mtw_reg->fields('lmrm', array());
		
		
		$master_part_mtw_reg->condition('lmrm.user_id', $user_id);
		$master_part_mtw_reg->condition('lmrm.id', $application_id);
		
		
		$master_part_mtw_reg_result = $master_part_mtw_reg->execute(); 
		 
		if($master_part_mtw_reg_result->rowCount() >0 ){
			$content	=	$master_part_mtw_reg_result->fetchAssoc();	
			
			
			/////////////////////////  MTW Renewal Table Data Fetch on condition ///////////////////
			$renwalDataQry	= db_select('l_mtw_registration_renewal','lmrr');
			$renwalDataQry	->fields('lmrr',array('current_fees')); 
			$renwalDataQry	->condition('lmrr.application_id',trim($application_id)); 
			$renwalDataQry	->condition('lmrr.status',1);
			if($content['is_renew']==1 ){
				$renwalDataQry	->condition('lmrr.purpose','renewal');
			}else if($content['is_amend']==1){
				$renwalDataQry	->condition('lmrr.purpose','amend');
			}else{
				$renwalDataQry	->condition('lmrr.purpose','registration');
			}
			$renwalData		= $renwalDataQry->execute();
			if($renwalData->rowCount() >0){
				$recontent	= $renwalData->fetchObject();
				$retFees = $recontent->current_fees;
			}else{
				$retFees = '0.00';
			}
			
			/////////////////////////  MTW Address Fetch ////////////////////////////////
			$table			= 'l_mtw_registration_master';
			$type			= 'mtw';
			$id				= $application_id;
			$fieldArr		= array('mtw_loc_dist','mtw_loc_subdivision','mtw_loc_areatype','mtw_loc_areatype_code','mtw_loc_vill_ward','mtw_loc_ps','mtw_loc_pincode');
			$mtw_address 	= get_full_address($table, $type, $application_id, $fieldArr);	
			
			
			/////////////////////////  MTW Uploaded Docs ////////////////////////////////
				
			$master_part_mtw_docs = db_select('l_documents_upload', 'ldu');
			$master_part_mtw_docs->fields('ldu', array());
			$master_part_mtw_docs->condition('ldu.user_id', $user_id);
			$master_part_mtw_docs->condition('ldu.act_id', '3');
			$master_part_mtw_docs->condition('ldu.application_id', trim($application_id));
			$master_part_mtw_docs_result = $master_part_mtw_docs->execute();
			
			$content_docs	=	$master_part_mtw_docs_result->fetchAssoc();
			
			
			
			/////////////////////////  MTW Backlog Data Fetch ////////////////////////////////
			
			$backlog_id			=	$content['backlog_id'];
			
			if($backlog_id !=0){
			$backlogQuery	= db_select('l_mtw_registration_backlog','lmrb');
			$backlogQuery	->fields('lmrb',array('registration_number','registration_date','expiry_date'));
			$backlogQuery	->condition('lmrb.id',$backlog_id);
			$backlogExe		= $backlogQuery->execute();
			if($backlogExe->rowCount() >0 ){
					$backlogResult	= $backlogExe->fetchObject();
					$old_registration_number = '<tr><td colspan="2" align="center"><b>Registration Number(Manual):</b> '.$backlogResult->registration_number.'</td></tr><tr><td colspan="2"></td></tr>';			
				}
			}else{
				$old_registration_number	= '';
			}
			
			
			/*********Fetching Person Data*******/
			// common veriable
			$tablePr	= 'l_mtw_registration_master_extra';
			$fieldArrPr	= array('person_district','person_subdivision','person_areatype','person_areatype_code','person_vill_ward','person_ps','person_pincode');
			$selectArr = array('id','person_name','person_address');
			
			$getAllQuery = db_select('l_mtw_registration_master_extra','lmrme');
			$getAllQuery->fields('lmrme',array());
			$getAllQuery->condition('application_id', $application_id);
			$getAllQuery->orderby('id','DESC');
			$getData = $getAllQuery->execute();
			
			$persondata					= array();
			$ppPersonCount				= 0;
			$prPersonCount				= 0;
			$mtuPersonCount				= 0;
			$gmPersonCount				= 0;
			$drPersonCount				= 0;
			$othrPersonCount			= 0;
			$id							= 1;
			
			if($getData->rowCount()>0){
				
				$drPersonData[] = '<tr>
									<td width="10%" align="center" style="border:1px solid #d1d1d1; border-top:none;border-right:none;"><strong>SL.No.</strong></td>
									<td width="40%" style="border:1px solid #d1d1d1; border-top:none;border-right:none;"><strong>'.$label.'</strong></td>
									<td width="50%" style="border:1px solid #d1d1d1; border-top:none;"><strong>Address</strong></td>
									</tr>
									';
				
				
				
				foreach($getData->fetchAll() as $single){
					$label				= '';
					$designation		= $single->designation;
					
					if(($designation == "proprietor") || ($designation == "partner")) {
						$label			= "Name of the ".ucwords($designation)." in case of firm not registered under the Companies Act,1956";
					}else if($designation =="general_manager"){
						$label			= "Name of the General Manager in the case or a public sector undertaking";
					}else if($designation =="director"){
						$label			= "Name of the Director in case of company registered under the Companies Act,1956";
					}else{
						$label			= "Name of the ".ucwords($designation)." ";
					}
					
					if($single->company_name!=''){
						$company		= '<br>'.$single->company_name;
					}else{
						$company		= '';
					}
					
					// person out of country code
					$personCountry	= $single->person_country;
					if($personCountry=='1'){
						$pr_country = 'India';						
					}elseif($personCountry=='2'){
						$pr_country = 'Others';	
					}
					
					// person out of wb state code
					$personState	= $single->person_state;
					$getStateQuery	= db_select('state_master','sm')->fields('sm',array())->condition('sm.id',$personState,'=')->execute();
					$getStateData	= $getStateQuery->fetchObject();
					if(!empty($getStateData)){
						$pr_state	= $getStateData->statename.', ';
					}else{
						$pr_state	= '';
					}
					
					
					if($designation == "proprietor"){
						$ppPersonData[]		= '<tr>
												<td width="5%"></td>
												<td width="30%" align="center" style="font-size:15px;">'.$label.' </td>
												<td width="60%" align="center" style="font-size:15px;">'.$single->person_name.'<br>(Firm Name:'.$single->company_name.')</td>
											   </tr>
											   <tr>
											   <td>'.$id.'</td>
												<td width="30%" align="center" style="font-size:15px;">Address</td>
												<td width="60%" align="center" style="font-size:15px;">'.$single->person_address.'<br>'.get_full_address($tablePr, $single->designation, $single->id, $fieldArrPr).'<br>'.$pr_state.$pr_country.'</td>
											   </tr>';
						
					}else if($designation == "partner"){
						$prPersonData[]		= '<tr>
												<td width="5%"></td>
												<td width="30%" align="center" style="font-size:15px;">'.$label.' </td>
												<td width="60%" align="center" style="font-size:15px;">'.$single->person_name.'<br>(Firm Name:'.$single->company_name.')</td>
											   </tr>
											   <tr>
											   <td>'.$id.'</td>
												<td width="30%" align="center" style="font-size:15px;">Address</td>
												<td width="60%" align="center" style="font-size:15px;">'.$single->person_address.'<br>'.get_full_address($tablePr, $single->designation, $single->id, $fieldArrPr).'<br>'.$pr_state.$pr_country.'</td>
											   </tr>';
						
					}else if($designation == "general_manager"){
						$gmPersonData[]		= '<tr>
												<td width="5%"></td>
												<td width="30%" align="center" style="font-size:15px;">'.$label.' </td>
												<td width="60%" align="center" style="font-size:15px;">'.$single->person_name.'<br>(Public Sector Undertaking:'.$single->company_name.')</td>
											   </tr>
											   <tr>
											   <td>'.$id.'</td>
												<td width="30%" align="center" style="font-size:15px;">Address</td>
												<td width="60%" align="center" style="font-size:15px;">'.$single->person_address.'<br>'.get_full_address($tablePr, $single->designation, $single->id, $fieldArrPr).'<br>'.$pr_state.$pr_country.'</td>
											   </tr>';
						
					}else if($designation == "director"){
						$drPersonData[]	= '<tr>
											<td align="center" style="border:1px solid #d1d1d1; border-top:none;border-right:none;">'.$id.'</td>
											<td style="border:1px solid #d1d1d1; border-top:none;border-right:none;">'.$single->person_name.'<br>('.$single->company_name.')</td>
											<td style="border:1px solid #d1d1d1; border-top:none;">'.$single->person_address.'<br>'.get_full_address($tablePr, $single->designation, $single->id, $fieldArrPr).'<br>'.$pr_state.$pr_country.'</td>
										 </tr>';
						
					}else{
						$othrPersonData[] = '<tr>
												<td width="5%"></td>
												<td width="30%" align="center" style="font-size:15px;">'.$label.' </td>
												<td width="60%" align="center" style="font-size:15px;">'.$single->person_name.'<br>(Company Name:'.$single->company_name.')</td>
											   </tr>
											   <tr>
											   <td>'.$id.'</td>
												<td width="30%" align="center" style="font-size:15px;">Address</td>
												<td width="60%" align="center" style="font-size:15px;">'.$single->person_address.'<br>'.get_full_address($tablePr, $single->designation, $single->id, $fieldArrPr).'<br>'.$pr_state.$pr_country.'</td>
											   </tr>';
						
					}
					$i++;
				}
			}else{
				$persondata[]	= '<span class="note-span">You have not yet submit any data about Proprietor / General Manager / Directors </span>';
				$ppPersonData	= array();
				$prPersonData	= array();
				$mtuPersonData	= array();
				$gmPersonData	= array();
				$drPersonData	= array();
				$othrPersonData	= array();
			}
			
			if($content['total_routes'] != ''){
				// details of the route data
				$detailsroute = $content['mtw_route_details'];
			}else{
				$detailsroute = '';
			}
			
			if($content['total_routes'] != ''){	
				$total_milage_add 	= 0;
				$total_no		  	= 0;			
				$routeDetails1	=	db_select('l_mtw_registration_extended','mtwe');
				$routeDetails1->fields('mtwe',array());
				$routeDetails1->condition('mtwe.application_id',$application_id);
				$fetdata = $routeDetails1->execute();
	
				$allDetails 	=	$fetdata->fetchAll();
				if(!empty($allDetails)){
					foreach($allDetails as $datum1){
						$total_no++;
						$total_milage_add =  $total_milage_add+$datum1->milage;
					}
				}
				$output2.= $total_milage_add;
			}else{
				$output2.= '';
			}
			
			if($content['mtw_nature']=='city_service'){
				$mtw_nature_contrnt		=	'City Service';
			}elseif($content['mtw_nature']=='long_distance'){
				$mtw_nature_contrnt		=	'Long Distance';
			}elseif($content['mtw_nature']=='passenger_service'){
				$mtw_nature_contrnt		=	'Passenger Service';
			}elseif($content['mtw_nature']=='long_distance_freight_service'){
				$mtw_nature_contrnt		=	'Long Distance Freight Service';
			}elseif(!empty($content['mtw_nature']) && $content['mtw_nature']!='city_service_long_distance' && $content['mtw_nature']!='passenger_service' && $content['mtw_nature']!='long_distance_freight_service'){
				$mtw_nature_contrnt		=	$content['mtw_nature'];
			}else{
				$mtw_nature_contrnt		=	'';
			}
			
			
			$output ='';			
			$output .= '<style>
							table, td, th , tr{font-size:14px;font-family:Times New Roman;height:14px;color:#000;background:#fff;padding:5px;}
							.div-head{font-weight:bold;font-size:14px;text-align:center;font-family:Times New Roman;padding:8px;}
						</style>
						
						<table width="100%" border="0" cellspacing="0" cellpadding="0" style="font-family:Times New Roman;  margin-top:5px;">
						  <tr>
							<td colspan="2" align="center" style="font-size:18px;"><strong>FORM - I</strong></td>
						  </tr>
						  <tr>
							<td colspan="2" align="center" style="font-size:10px;"><strong>(SEE RULE 4 & 8)</strong></td>
						  </tr>
						  <tr>
							<td colspan="2" align="center" style="font-size:14px;">APPLICATION FOR REGISTRATION OF UNDERTAKING AND GRANT OF RENEWAL OF CERTIFICATE OF REGISTRATION</td>
						  </tr>
						  <tr><td colspan="2"></td></tr>'.$old_registration_number.'
						  <tr><td colspan="2"></td></tr>
						  <tr><td colspan="2"></td></tr>						 
						  <tr>
							<td>1. Name of Motor Transport Undertaking: </td>
							<td>'.$content['mtw_name'].'</td>
						  </tr>						 
						  <tr>
							<td>2. Full Address to which communications relating to the Motor Transport Undertaking should be sent: </td>
							<td>'.$content['mtw_loc_address'].','.$mtw_address.'</td>
						  </tr>						  
						  <tr>
							<td>3. Nature of Motor Transport Service: </td>
							<td>'.$mtw_nature_contrnt.'</td>
						  </tr>						 
						  <tr>
							<td>4. Total Number Of Routes: </td>
							<td>'.$content['total_routes'].'</td>
						  </tr>						  
						  <tr>
							<td>5. Route Details - Mileage: </td>
							<td>'.$output2.'</td>
						  </tr>						  
						  <tr>
							<td>6. Total Number of Motor Transport Vehicles on the last date or the preceeding year: </td>
							<td>'.$content['total_mtw_vehicle'].'</td>
						  </tr>						  
						  <tr>
							<td>7. Maximum Number of Motor Transport Workers employed on any day during the preceeding year : </td>
							<td>'.$content['mtw_maxworkers'].'</td>
						  </tr>
						  <tr><td></td></tr>
						  <tr>
							<td>8. Full Name and Residential Address of the <br> i) Proprietor or Partners or The Motor Transport Undertaking in the case of firm not registered under the company act, 1956 : <br> or <br> ii) General Manager in the case of a public sector undertaking : </td>
							<td><br>Annexure Attached <br><br><br> Annexure Attached </td>
						  </tr>						 
						  <tr>
							<td>9. Full Name and Residential Address of the Directors in the case of company registered under Companies Act, 1956 : </td>
							<td>Annexure Attached </td>
						  </tr>';
						  
					
					/* Payment Details Start */
					
					 if(trim($content['status'])== 'T' || trim($content['status'])== 'S' || trim($content['status'])== 'I'){												
						$renwal_tdata = db_select('l_mtw_registration_renewal', 'mr');
						$renwal_tdata->fields('mr', array('id'));
						$renwal_tdata->condition('mr.application_id', $content['id']);
						$renwal_tdata->condition('mr.id', $renewal_id);
						// $renwal_tdata->condition('mr.status', 'T');	
						$renwal_tdata = $renwal_tdata->execute()->fetchObject();
						
										
						$transaction_details = db_select('l_principle_epayments_receive_data', 'lpd');
						$transaction_details->leftJoin('l_principle_epayments_data', 'lped', 'lpd.transaction_id=lped.identification_no');
						$transaction_details->fields('lped', array('identification_no', 'application_id'));
						$transaction_details->fields('lpd', array());
						$transaction_details->condition('lped.act_id', '3');
						$transaction_details->condition('lped.application_id', $renwal_tdata->id);		
						$trans_details_result = $transaction_details->execute();
						
						if($trans_details_result-> rowCount() > 0 ){
							
							$transaction_det	= $trans_details_result->fetchAssoc();
							$bankTransactionID	= $transaction_det['transaction_id'];
							$grn_number			= $transaction_det['challanrefid'];
							$challan_fid_date	= !empty($transaction_det['challanrefid_date']) ? $transaction_det['challanrefid_date'] : '';
							$total_amount		= number_format($transaction_det['challanamount'], 2);
							$bank_code			= $transaction_det['bank_cd'];
							$payment_status		= $transaction_det['banktransactionstatus'];	
						}
					 }
					 
					 $payment_details = '<u>Grips Payment[Online/Counter]</u><br>';
					 $payment_details .= 'IFSC Code: <span class="color_orange">'.$bank_code.'</span><br>';
					 $payment_details .= 'GRN Number: <span class="color_orange">'.$grn_number.'</span><br>';
					 $payment_details .= 'Bank Transaction Id: <span class="color_orange">'.$bankTransactionID.'</span><br>';
					 $payment_details .= 'Total Amount: <span class="color_orange">'.$total_amount.'</span><br>';
					 $payment_details .= 'Transaction Date: <span class="color_orange">'.$challan_fid_date.'</span><br>';
					// $payment_details .= 'Transaction Status: '.$payment_status;
					 
 					/* Payment Details End */ 
					
					
					$output .= '<tr>
									<td> 10. Amount Fees ,enclosed <br/>(GRN, Date, Amount): </td>';
									
									if(trim($content['status'])== 'T' || trim($content['status'])== 'S' || trim($content['status'])== 'I'){
									
										$output .='<td>'.$payment_details.'</td>';
									}else {
									
										$output .='<td> &#8377; '.$retFees.' &nbsp; '.date("dS M, Y").'</td>'; 
									}
					$output .='</tr>
								<tr><td colspan="2"></td></tr>								
								<tr><td colspan="2"></td></tr>
								<tr><td colspan="2"></td></tr>	
								<tr><td>Date of receipt of application: '.date("dS M Y").'</td><td align="center">Signature of the Employer</td></tr>
															
								<tr><td></td><td align="center">Seal and Stamp</td></tr>
								<tr><td></td></tr>								
								<tr><td colspan="2" align="left"></td></tr>
						   </table>';
								
							if(!empty($ppPersonData) || !empty($prPersonData) || !empty($gmPersonData) || !empty($drPersonData) || !empty($othrPersonData)){
								$output	.= '<p style="page-break-after:always">
											<table width="100%" border="0" cellspacing="0" cellpadding="0">
											<tr>
												<th colspan="3" align="center" style="font-size:18px;"><strong>ANNEXURE</strong></th>
												<tr><td></td></tr>
											  </tr>
											</table>';			
								
								if(!empty($ppPersonData) || $ppPersonData!=''){
												foreach($ppPersonData as $ppSingle){
													$ppPersonCount		= $ppPersonCount+1; 
									$output	.= '<table width="100%" border="0" cellspacing="0" cellpadding="0" style="border:1px solid #ccc;">
												<tr>
													<th colspan="3" align="center" style="font-size:15px; border:1px solid #d1d1d1;">Proprietor - '.$ppPersonCount.' Details</strong></th>
												</tr>';
									$output	.= 	$ppSingle;				
									$output .= '</table><br><br>';
												}
											}
											
											if(!empty($prPersonData) || $prPersonData!=''){
												foreach($prPersonData as $prSingle){
													$prPersonCount		= $prPersonCount+1; 
									$output	.= '<table width="100%" border="0" cellspacing="0" cellpadding="0">
												<tr>
													<th colspan="3" align="center" style="font-size:15px; border:1px solid #d1d1d1;">Partners - '.$prPersonCount.' Details</strong></th>
												</tr>';
									$output	.= 	$prSingle;				
									$output .= '</table><br><br>';
												}
											}
											
											if(!empty($gmPersonData) || $gmPersonData!=''){
												foreach($gmPersonData as $gmSingle){
													$gmPersonCount		= $gmPersonCount+1; 
									$output	.= '<table width="100%" border="0" cellspacing="0" cellpadding="0">
												<tr>
													<th colspan="3" align="center" style="font-size:15px; border:1px solid #d1d1d1;">General Manager - '.$gmPersonCount.' Details</strong></th>
												</tr>';
									$output	.= 	$gmSingle;				
									$output .= '</table><br><br>';
												}
											}
											
											if(!empty($drPersonData) || $drPersonData!=''){
												foreach($drPersonData as $drSingle){
													$drPersonCount		= $drPersonCount+1; 
									$output	.= '<table width="100%" border="0" cellspacing="0" cellpadding="0">
												<tr>
													<th colspan="3" align="center" style="font-size:15px; border:1px solid #d1d1d1;">Director - '.$drPersonCount.' Details</strong></th>
												</tr>';
									$output	.= 	$drSingle;				
									$output .= '</table><br><br>';
												}
											}
											
											if(!empty($othrPersonData) || $othrPersonData!=''){
												foreach($othrPersonData as $othrSingle){
													$othrPersonCount		= $othrPersonCount+1; 
									$output	.= '<table width="100%" border="0" cellspacing="0" cellpadding="0">
												<tr>
													<th colspan="3" align="center" style="font-size:15px; border:1px solid #d1d1d1;">Other - '.$othrPersonCount.' Details</strong></th>
												</tr>';
									$output	.= 	$othrSingle;				
									$output .= '</table><br><br>';
												}
											}
											
								
								$output	.= 	'<table width="100%" border="0" cellspacing="0" cellpadding="0">
												<tr><td colspan="2"></td></tr>
												<tr><td colspan="2"></td></tr>
												<tr><td colspan="2"></td></tr>												
												<tr><td>Date of receipt of application: '.date("dS M Y").'</td><td align="center">Signature of the Employer</td></tr>												
												<tr><td width="60%"></td><td align="center">Seal and Stamp</td></tr>																								
											</table></p>';				
								}
				//print_r($output);die;
				return pdf_using_mpdf_api($output, 'MTW-FORM-I-'.time());
		}else{
			drupal_goto($base_path);
		}
}
