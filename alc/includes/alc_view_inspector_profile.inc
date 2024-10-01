<?php
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');

function alc_view_inspector_profile($form,&$form_state,$block='' ,$inspectoruserid=''){ 
	
	global $base_root, $base_path, $user;
	
	$loggedin_id 		= $user->uid;
	 $blockcode			=	encryption_decryption_fun('decrypt', $block)	;
	$inspector_user_id	=	encryption_decryption_fun('decrypt', $inspectoruserid)	;

	$output	=	view_inspector_profile($blockcode,$inspector_user_id);

	
	
																
		
				
	$form['inspector_profile']    = array(
											'#type' 		=> 'fieldset',
											'#attributes'	=> array('class'=>array(''))
									  	);
										
	$form['inspector_profile']['view_office'] = array(		
									'#prefix' 				=> '<div class="content"><div class="sky-form" style="width:100%;">',
									'#suffix' 				=> '</div></div>',																		
									'#markup' 			    => $output,																		
									);

	return $form;
	
}
function view_inspector_profile($blockcode,$inspector_user_id){
		global $base_root, $base_path, $user;
		$pending_appliaction_clra	=	0;
		$inprocess_application_clra	=	0;
		$issued_application			=	0;
		$reject_application			=	0;
		
		$pending_appliaction_bowca	=	0;
		$inprocess_application_bowca=	0;
		$issued_application_bowca	=	0;
		$reject_application_bowca	=	0;
		
		$pending_appliaction_license	=	0;
		$inprocess_application_license=	0;
		$issued_application_license	=	0;
		$reject_application_license	=	0;
		
		
		$query_inspector	 	=  db_select('l_customuser_relation_address', 'relation');
		$query_inspector		-> InnerJoin('district_master', 'dis','dis.district_code= relation.district_code');
		$query_inspector		-> InnerJoin('sub_division', 'subdv','subdv.sub_div_code= relation.sub_div_code');
		$query_inspector		-> InnerJoin('block_mun_master', 'block','block.block_code= relation.block_code');
		$query_inspector		-> InnerJoin('l_custom_user_detail', 'user_details','user_details.usr_id= relation.user_id');
		$query_inspector		-> InnerJoin('users', 'U','U.uid= relation.user_id');
		$query_inspector		-> fields('user_details', array('officenumber','fullname','officenumber','mobile','usr_id'));
		$query_inspector		-> fields('dis', array('district_name'));
		$query_inspector		-> fields('subdv', array('sub_div_name'));
		$query_inspector		-> fields('block', array('block_mun_name','block_code'));	
		$query_inspector		-> fields('U', array('mail'));	
		$query_inspector		-> condition('user_details.usr_id', $inspector_user_id);
		$result  				=  $query_inspector->execute()->fetchAssoc();
		
		if(!empty($result)){
				
				
				$officenumber	=	(!empty($result['officenumber']))?$result['officenumber']:'<font color="#FF0000">Not Available</font>';
				$mail	=	(!empty($result['mail']))?$result['mail']:'<font color="#FF0000">Not Available</font>';
				$mobile	=	(!empty($result['mobile']))?$result['mobile']:'<font color="#FF0000">Not Available</font>';
				$posting	= $result['block_mun_name'].','.$result['sub_div_name'].','.$result['district_name'];
				
			
					$ins_details .= '&nbsp;&nbsp;&nbsp;<font color="#663366" size="6">'.$result['fullname'].'</font>';
					if(!empty($result['mail']))
						$ins_details .='<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Email:- <a href="mailto:'.$result['mail'].'">'.$result['mail'].'</a>';
					else
						$ins_details .='<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Email:- <font color="#FF0000">Not Available</font>';
						
						
					if(!empty($result['mobile']))
						$ins_details .='<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Mobile:- <a href="callto:'.$result['mobile'].'">'.$result['mobile'].'</a>';
					else
						$ins_details .='<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Mobile:- <font color="#FF0000">Not Available</font>';
					
					
						
							$ins_details .='<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Office ph:-<font color="#006600">'.$officenumber.'</font><br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
						<font color="#6633FF">'.$posting.'</font></section>';
			
			
				$query_office		= 	db_select('l_rlo_additional_info', 'addition_office');
				$query_office 		->	 InnerJoin('district_master', 'dis','dis.district_code= addition_office.district_code');
				$query_office		->	fields('addition_office',array('clra_certificate_address','office_name','district_code'));
				$query_office		-> 	fields('dis', array('district_name'));
				$query_office		->	condition('addition_office.sub_div_code',$blockcode);
				$result_office		= 	$query_office->execute()->fetchAssoc();
				//echo $result_office['office_name'];die();
				
	if(!empty($result_office['office_name'])){
				if($result_office['office_name']=='INSPECTOR'){
					$office_name = '<font color="#FF9900">'.strtoupper($result['block_mun_name']).', LABOUR WELFARE FACILITATION CENTRE</font>';
				}elseif($result_office['office_name']=='ALC'){
					$office_name = '<font color="#FF9900">OFFICE OF THE ASSISTANT LABOUR COMMISSIONER</font>';
				}elseif($result_office['office_name']=='DLC'){
					$office_name = '<font color="#FF9900">OFFICE OF THE DEPUTY LABOUR COMMISSIONER</font>';
				}
	}else{
		$office_name = '<font color="#990033">OFFICE NAME IS NOT GIVEN BY AUTHORISED OFFICER</font>';
	}
				//$office_address =(!empty($result_office['clra_certificate_address']))?wordwrap($result_office['clra_certificate_address'], 60, "<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n"):'Office Address is not Available';
				$office_address =(!empty($result_office['clra_certificate_address']))?$result_office['clra_certificate_address']:'Office Address is not Available';
				$office_pin		 =(!empty($result['pin_number']))?$result['pin_number']:'NIL';
				
				$office_details .= '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp<strong>'.$office_name.'</font>
						<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbspOffice ph:-<font color="#006600">'.$officenumber.'
						<br/><font color="#6633FF"><div style="width: 788px; padding: 0px 0px 0px 20px; margin: 0 0 0 22px;">'.$office_address.'</div></font>';			
				
		
		}
		
			$clra_application		 	= db_select('l_clra_registration_master', 'clra_master');
		    $clra_application			->fields('clra_master',array('final_submit_status','status','user_id','name_areatype'));
			$clra_application			->condition('clra_master.name_areatype',$blockcode);
			$clra_application			->condition('clra_master.final_submit_status','P');
			$clra_application			->orderBy('clra_master.id','DESC');
			$clra_application_details 	= $clra_application->execute()->fetchAll();
			$clra_application_data 		= array_map("unserialize", array_unique(array_map("serialize", $clra_application_details)));
		//print_r($clra_application_data);
			
			if(!empty($clra_application_data)){
						foreach($clra_application_data as $data){
							if($data->final_submit_status == 'P' && $data->status	=='0'){
								$pending_appliaction_clra	=	$pending_appliaction_clra+1;
							}if($data->final_submit_status == 'P' && $data->status	=='B'){
								$inprocess_application_clra	=	$inprocess_application_clra+1;
							}if($data->final_submit_status == 'P' && $data->status	=='BI'){
								$inprocess_application_clra	=	$inprocess_application_clra+1;
							}if($data->final_submit_status == 'P' && $data->status	=='V'){
								$inprocess_application_clra	=	$inprocess_application_clra+1;
							}if($data->final_submit_status == 'P' && $data->status	=='VA'){
								$inprocess_application_clra	=	$inprocess_application_clra+1;
							}if($data->final_submit_status == 'P' && $data->status	=='F'){
								$inprocess_application_clra	=	$inprocess_application_clra+1;
							}if($data->final_submit_status == 'P' && $data->status	=='T'){
								$inprocess_application_clra	=	$inprocess_application_clra+1;
							}if($data->final_submit_status == 'P' && $data->status	=='T'){
								$inprocess_application_clra	=	$inprocess_application_clra+1;
							}if($data->final_submit_status == 'P' && $data->status	=='C'){
								$inprocess_application_clra	=	$inprocess_application_clra+1;
							}	if($data->final_submit_status == 'P' && $data->status	=='S'){
								$pending_appliaction_clra	=	$pending_appliaction_clra+1;
							}if($data->final_submit_status == 'P' && $data->status	=='I'){
								$issued_application	=	$issued_application+1;
							}if($data->final_submit_status == 'P' && $data->status	=='R'){
								$reject_application	=	$reject_application+1;
							}
							
				 
				  			
				 
						}
				if($pending_appliaction_clra!='0'){
				
				 		$pending_clra 	  = l('<span>Pending Application(s) at office:-'.$pending_appliaction_clra.'</span>', 'alc_official/view-activities/ALC/'.encryption_decryption_fun('encrypt', $blockcode).'/P/C', array('html' => TRUE));
				}else{
					$pending_clra 	  = 'Pending Application(s) at office:-'.$pending_appliaction_clra;
				}if($inprocess_application_clra!='0'){
				
				 $inprocess_clra 	  = l('<span>Inproces Application(s) at citizen:-'.$inprocess_application_clra.'</span>', 'alc_official/view-activities/ALC/'.encryption_decryption_fun('encrypt', $blockcode).'/In/C', array('html' => TRUE));
				}else{
					$inprocess_clra 	  = 'Inproces Application(s)at citizen:-'.$inprocess_application_clra;
				}if($issued_application	!='0'){
					$Issued_clra 	  = l('<span>Issued Application(s):-'.$issued_application.'</span>', 'alc_official/view-activities/ALC/'.encryption_decryption_fun('encrypt', $blockcode).'/I/C', array('html' => TRUE));
				}else{
					$Issued_clra 	  = 'Issued Application(s):-'.$issued_application;
				}if($reject_application	!='0'){
					$reject_clra 	  = l('<span>Rejected Application(s):-'.$reject_application.'</span>', 'alc_official/view-activities/ALC/'.encryption_decryption_fun('encrypt', $blockcode).'/R/C', array('html' => TRUE));
				}else{
					$reject_clra 	  = 'Rejected Application(s):-'.$reject_application;
				}
				  
				
				$clra_application_list .= '	<section class="col col-6">
												<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png"><strong>CLRA Application(s)</strong>
												<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; '.$pending_clra.'
												<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$inprocess_clra.'
												<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$Issued_clra.'
												<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$reject_clra.'
											</section>';	
			 }if(empty($clra_application_data)){
				 $clra_application_list	='';
				 $clra_application_list	=	'<section class="col col-6">
													<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png"><strong>CLRA Application(s)</strong>
													<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;No CLRA application is Available
												</section>';
				 
			 }
					
					
			$bowca_application		 	= db_select('l_bocwa_registration_master', 'bocwa');
		    $bowca_application			->fields('bocwa',array('final_submit_status','status','user_id','loc_e_areatype_code'));
			$bowca_application			->condition('bocwa.loc_e_areatype_code',$blockcode);
			$bowca_application			->condition('bocwa.final_submit_status','P');
			$bowca_application			->orderBy('bocwa.id','DESC');
			$bowca_application_details 	= $bowca_application->execute()->fetchAll();
			$bowca_application_data 	= array_map("unserialize", array_unique(array_map("serialize", $bowca_application_details)));
		
			
			if(!empty($bowca_application_data)){
						foreach($bowca_application_data as $data){
							if($data->final_submit_status == 'P' && $data->status	=='0'){
								$pending_appliaction_bowca	=	$pending_appliaction_bowca+1;
							}if($data->final_submit_status == 'P' && $data->status	=='B'){
								$inprocess_application_bowca	=	$inprocess_application_bowca+1;
							}if($data->final_submit_status == 'P' && $data->status	=='BI'){
								$inprocess_application_bowca	=	$inprocess_application_bowca+1;
							}if($data->final_submit_status == 'P' && $data->status	=='V'){
								$inprocess_application_bowca	=	$inprocess_application_bowca+1;
							}if($data->final_submit_status == 'P' && $data->status	=='VA'){
								$inprocess_application_bowca	=	$inprocess_application_bowca+1;
							}if($data->final_submit_status == 'P' && $data->status	=='F'){
								$inprocess_application_bowca	=	$inprocess_application_bowca+1;
							}if($data->final_submit_status == 'P' && $data->status	=='T'){
								$inprocess_application_bowca	=	$inprocess_application_bowca+1;
							}if($data->final_submit_status == 'P' && $data->status	=='T'){
								$inprocess_application_bowca	=	$inprocess_application_bowca+1;
							}if($data->final_submit_status == 'P' && $data->status	=='C'){
								$inprocess_application_bowca	=	$inprocess_application_bowca+1;
							}	if($data->final_submit_status == 'P' && $data->status	=='S'){
								$pending_appliaction_bowca	=	$pending_appliaction_bowca+1;
							}if($data->final_submit_status == 'P' && $data->status	=='I'){
								$issued_application_bowca	=	$issued_application_bowca+1;
							}if($data->final_submit_status == 'P' && $data->status	=='R'){
								$reject_application_bowca	=	$reject_application_bowca+1;
							}
						}
				if($pending_appliaction_bowca!='0'){
				
				 		$pending_bocwa	  = l('<span>Pending Application(s) at office:-'.$pending_appliaction_bowca.'</span>', 'alc_official/view-activities/ALC/'.encryption_decryption_fun('encrypt', $blockcode).'/P/B', array('html' => TRUE));
				}else{
					$pending_bocwa 	  = 'Pending Application(s) at office:-'.$pending_appliaction_bowca;
				}if($inprocess_application_bowca!='0'){
				
				 $inprocess_bocwa 	  = l('<span>Inproces Application(s) at citizen:-'.$inprocess_application_bowca.'</span>', 'alc_official/view-activities/ALC/'.encryption_decryption_fun('encrypt', $blockcode).'/In/B', array('html' => TRUE));
				}else{
					$inprocess_bocwa 	  = 'Inproces Application(s) at citizen:-'.$inprocess_application_bowca;
				}if($issued_application	!='0'){
					$Issued_bocwa 	  = l('<span>Issued Application(s):-'.$issued_application_bowca.'</span>', 'alc_official/view-activities/ALC/'.encryption_decryption_fun('encrypt', $blockcode).'/I/B', array('html' => TRUE));
				}else{
					$Issued_bocwa 	  = 'Issued Application(s):-'.$issued_application_bowca;
				}if($reject_application	!='0'){
					$reject_bocwa 	  = l('<span>Rejected Application(s):-'.$reject_application_bowca.'</span>', 'alc_official/view-activities/ALC/'.encryption_decryption_fun('encrypt', $blockcode).'/R/B', array('html' => TRUE));
				}else{
					$reject_bocwa 	  = 'Rejected Application(s):-'.$reject_application_bowca;
				}
				
				$bocwa_application_list .= '
											<section class="col col-6">
												<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png"><strong>BOCWA Application(s)</strong>
												<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; '.$pending_bocwa.'
												<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$inprocess_bocwa.'
												<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$Issued_bocwa.'
												<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$reject_bocwa.'
											</section>';	
			 }if(empty($bowca_application_data)){
				 $bocwa_application_list	='';
				 $bocwa_application_list	=	'<section class="col col-6">
													<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png"><strong>BOCWA Application(s)</strong>
													<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;No BOCWA application is Available
												</section>';
				 
			 }
			 
			$get_license_data					= db_select('l_contractor_info_master', 'infomaster');
			$get_license_data					->InnerJoin('l_particular_of_contract_labour', 'particular','particular.serial_no_from_v= infomaster.id'); 
			$get_license_data					->InnerJoin('l_contractor_license_application', 'license','particular.id= license.contractor_particular_id'); 
			$get_license_data					->InnerJoin('l_clra_registration_master', 'clra_master','clra_master.id= infomaster.application_id');
			
		
			$get_license_data					->	fields('infomaster',array('name_of_contractor','application_id'));
			$get_license_data					->	fields('clra_master',array('name_areatype'));
			$get_license_data					->	fields('license',array('user_id','serial_no_from_v','created_date','license_date','principal_reg_no','contractor_license_number','final_status','status'));
			$get_license_data					->	fields('particular',array('id'));
			$get_license_data					->	condition('clra_master.name_areatype',$blockcode);
			$get_license_data					->	condition('license.final_status','F');
			$get_license_data_result 			= 	$get_license_data->execute()->fetchAll();
		
			//print_r($get_license_data_result);
			if(!empty($get_license_data_result)){
					foreach($get_license_data_result as $data){
							if($data->final_status == 'F' && $data->status	=='F'){
								$pending_appliaction_license	=	$pending_appliaction_license+1;
							}if($data->final_status == 'F' && $data->status	=='B'){
								$inprocess_application_license	=	$inprocess_application_license+1;
							}if($data->final_status == 'F' && $data->status	=='BI'){
								$inprocess_application_license	=	$inprocess_application_license+1;
							}if($data->final_status == 'F' && $data->status	=='A'){
								$inprocess_application_license	=	$inprocess_application_license+1;
							}if($data->final_status == 'F' && $data->status	=='FW'){
								$inprocess_application_license	=	$inprocess_application_license+1;
							}if($data->final_status == 'F' && $data->status	=='P'){
								$inprocess_application_license	=	$inprocess_application_license+1;
							}if($data->final_status == 'F' && $data->status	=='U'){
								$inprocess_application_license=	$inprocess_application_license+1;
							}if($data->final_status == 'F' && $data->status	=='C'){
								$inprocess_application_license	=	$inprocess_application_license+1;
							}	if($data->final_status == 'F' && $data->status	=='U'){
								$pending_appliaction_license_	=	$pending_appliaction_license_+1;
							}if($data->final_status == 'F' && $data->status	=='I'){
								$issued_application_license	=	$issued_application_license+1;
							}if($data->final_status == 'F' && $data->status	=='R'){
								$reject_application_license	=	$reject_application_license+1;
							}
					}
				
				
				if($pending_appliaction_license!='0'){
				
				 		$pending_license	  = l('<span>Pending Application(s) at office:-'.$pending_appliaction_license.'</span>', 'alc_official/view-activities/ALC/'.encryption_decryption_fun('encrypt', $blockcode).'/P/L', array('html' => TRUE));
				}else{
					$pending_license 	  = 'Pending Application(s) at office:-'.$pending_appliaction_license;
				}if($inprocess_application_license!='0'){
				
				 $inprocess_license 	  = l('<span>Inproces Application(s) at citizen:-'.$inprocess_application_license.'</span>', 'alc_official/view-activities/ALC/'.encryption_decryption_fun('encrypt', $blockcode).'/In/L', array('html' => TRUE));
				}else{
					$inprocess_license 	  = 'Inproces Application(s) at citizen:-'.$inprocess_application_license;
				}if($issued_application_license	!='0'){
					$Issued_license 	  = l('<span>Issued Application(s):-'.$issued_application_license.'</span>', 'alc_official/view-activities/ALC/'.encryption_decryption_fun('encrypt', $blockcode).'/I/L', array('html' => TRUE));
				}else{
					$Issued_license 	  = 'Issued Application(s):-'.$issued_application_license;
				}if($reject_application_license	!='0'){
					$reject_license 	  = l('<span>Rejected Application(s):-'.$reject_application_license.'</span>', 'alc_official/view-activities/ALC/'.encryption_decryption_fun('encrypt', $blockcode).'/R/L', array('html' => TRUE));
				}else{
					$reject_license 	  = 'Rejected Application(s):-'.$reject_application_license;
				}
				
				$license_application_list .= '	<section class="col col-6">
												<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png"><strong>LICENSE Application(s)</strong>
												<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$pending_license.'
												<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$inprocess_license.'
												<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$Issued_license.'
												<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$reject_license.'
											</section>';	
			 }if(empty($get_license_data_result)){
				 $license_application_list	='';
				 $license_application_list	=	'<section class="col col-6">
													<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png"><strong>LICENSE Application(s)</strong>
													<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;No CLRA application is Available
											</section>';
				 
			 }
			  $reneal_application_list	=	'<section class="col col-6">
													<img src="'.$base_root.$base_path.'sites/all/themes/jackson/images/bullet-green.png"><strong>RENEWAL of License Application(s)</strong>
													<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;No RENEWAL application is Available
											</section>';
						
				
				
		
	
		$output	.= '<style type="text/css">
					table, td, th {
						border: 1px solid #006595;
						font-size:18px;
						font-family:Times New Roman;
						height:40px;
						margin-top:20px;
					}
					td{
						padding-left:13px;}
					th {
						background-color: #008BD1;
						color: white;
					}
					td:hover {
						background-color:#d4e3e5;
					}
					tr:nth-child(even) {background: #DBE5F0}
					tr:nth-child(odd) {background: #F1F1F1}
					
					
			   </style>
			  
				<table width="100%" border="0" cellspacing="0" cellpadding="0">
					<tr>
					<td colspan="3" align="center" style="text-align:center; padding-top: 8px;">
						<font color="#3366FF">'.$ins_details.'</font>
					</td>
					</tr>
					
					<tr>
					<td colspan="3" align="center" style="text-align:left;font-size:18px; padding-top: 8px;">
						<font color="#3366FF"><strong><u>OFFICE DETAILS</u>-</strong></font><br>'.
						
						
					$office_details.'</td>
				    </tr>
					<tr>
					<td colspan="3" align="center" style="text-align:left;font-size:18px; padding-top: 8px;">
						<font color="#3366FF"><strong><u>ACTIVITIES</u>-</strong></font><br><div class="row">'.
						
						
					$clra_application_list.$bocwa_application_list.$license_application_list.$reneal_application_list.'</div></td>
				    </tr>

 
 
  					
				</table>';
				
		
				return $output;
}






