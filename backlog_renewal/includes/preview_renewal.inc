<?php

drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
function preview_renewal($serial_number){

	global $base_root, $base_path, $user;
	
	$contractor_id 							= 	$user->uid;
	$action = 'decrypt';
	$serial = encryption_decryption_fun($action, $serial_number);
	
	$query 									= 	db_select('l_contractor_info_master', 'c_info_master');
 	$query									->	fields('c_info_master',array('user_id','application_id'))->condition('id',$serial,'=');
	$query									->	condition('act_id','1','=');
	$query_result							=	$query->execute();
	
	if( $query_result->rowCount() > 0 ){
		
		foreach($query_result as $row){
			$application_id 				= 	$row->application_id;
			$applicant_id 					= 	$row->user_id;
		}
	}

	// Contractor Personal Details

	$contractor_personal					=	array();
	
	$contractor_personal_query 				= 	db_select('l_contractor_info_master', 'c_info_master');
	$contractor_personal_query				->	fields('c_info_master',array())->condition('id',$serial,'=');
	$contractor_personal_query				->	condition('act_id','1','=');
	$contractor_personal_result 			= 	$contractor_personal_query->execute();
	
	if($contractor_personal_result->rowCount() > 0 ){
		
		foreach($contractor_personal_result as $obj){
			$id_l_contractor_info_master 	=	$obj->id;
			$name_of_contractor				=	$obj->name_of_contractor;
		}
	}

	
	/** Establishment Nature of work  ***/
	
	
	$query_work 							= 	db_select('l_clra_reg_nature_of_work', 'c_reg_nature');
	$query_work								->	leftJoin('contractor_works', 'con_work','con_work.contractor_work_id = c_reg_nature.e_nature_of_work');  
	$query_work								->	fields('c_reg_nature',array('identification_number','user_id','act_id'));
	$query_work								->	fields('con_work',array('cont_work_name'));
	$query_work								->	condition('c_reg_nature.application_id',$application_id,'=');
	$query_work								->	condition('c_reg_nature.act_id','1','=');
	$result_work 							= 	$query_work->execute();
	 
	$nature_work							=	"";
	
	if( $result_work->rowCount() > 0 ){
		foreach($result_work as $objwork){
			$nature_work_list				=	$nature_work_list.$objwork->cont_work_name.', ';				  
		}
	}
	$nature_work							= 	substr($nature_work_list, 0, -2);
	
	/** Establishment Details    ***/

	$establishment_details_query 			= 	db_select('l_clra_registration_master', 'c_r_m');
	$establishment_details_query			->	fields('c_r_m',array('id','e_name','loc_e_name','registration_number','act_id','loc_e_areatype','registration_date','loc_emp_areatype','full_name_principal_emp','address_principal_emp'))->condition('id',$application_id,'=');
	$establishment_details_query			->	condition('c_r_m.act_id','1','=');
	$establishment_details_result 			= 	$establishment_details_query->execute();
	

	if( $establishment_details_result->rowCount() > 0 ){
		
		foreach($establishment_details_result as $obj){
			
 			$id							=	$obj->id;
			$registration_number		=	$obj->registration_number;
 			$loc_e_name					=	$obj->loc_e_name;	
			$loc_e_areatype				=	$obj->loc_e_areatype;
			$e_name						=	$obj->e_name;
			$registration_date			=	$obj->registration_date;
			$loc_emp_areatype			=	$obj->loc_emp_areatype;
			$full_name_principal_emp	=	$obj->full_name_principal_emp;
			$address_principal_emp		=	$obj->address_principal_emp;
		}
	}
						
	$establishment_address				=	"";
	$establishment_address				=	miscellaneous_establishment_address_form($loc_e_areatype,$serial,$application_id);// call from miscellaneous module		
	$establishment_address				=	$e_name.'<br/>'.$loc_e_name.'<br>'.$establishment_address;
	
	
	$principal_address					=	miscellaneous_principal_employer($loc_emp_areatype,$serial,$application_id);//----miscellaneous module
	$principal_details					=	$full_name_principal_emp."<br>".$address_principal_emp."<br>".$principal_address;
	
	$registration_date_timestamp 		= 	strtotime($registration_date);
	$formatted_reg_date 				= 	date('m/d/Y', $registration_date_timestamp);
	
	$establish_details_reg				=	"";
	$establish_details_reg				=	$establish_details_reg."Registration Number: ".$registration_number."<br/>"."Date of Certificates: ".$formatted_reg_date."\nUnder The acts of CLRA";	
	
	
	//** Contract Labour Nature of work **/
	
	
	if(!empty($id_l_contractor_info_master)){
		$result_labourwork 				= 	get_nature_of_work_contrcator($application_id,$id_l_contractor_info_master); //---miscellaneous module	
  		$labour_nature_work				=	"";
		if($result_labourwork->rowCount() > 0 ){
			foreach($result_labourwork as $objwork){
				$labour_nature_work		=	$labour_nature_work.$objwork->cont_work_name.', ';			  
			}
			$labour_nature_work			=	substr($labour_nature_work, 0, -2);
		}
	}
	//-----------------------------------END work---------------------------------------------------------

	$query 								= 	db_select('l_contractor_info_master', 'con_info_master'); 
 	$query								->	fields('con_info_master',array('id','user_id','act_id','est_date_of_work_of_each_labour_from_date','est_date_of_work_of_each_labour_to_date'));
  	$query								->	condition('con_info_master.id',$serial,'=');
   	$result 							= 	$query->execute()->fetchAssoc(); 
	
	
  	$startend_date						=	"";

	if( !empty($result) ){
		
		
			
			
			$startdate				=	$result['est_date_of_work_of_each_labour_from_date'];
			$enddate				=	$result['est_date_of_work_of_each_labour_to_date']; 
			
    
	 		$diff=round(abs(strtotime($startdate)-strtotime($enddate))/86400).' day(s)';
	
	 

			
			
		
	}

	$start_date							=	date('dS F y', strtotime($startdate));
	$end_date							=	date('dS F y', strtotime($enddate));
	$startend_date						=	$start_date.' To '.$end_date;
	
	
	
$pervious_license_detais='';

$pervious_license_detais=license_details($serial,$contractor_id);

	
	
	
	

	//--------------------------------Fetch uploaded documents------------------------------
	
	$uploaded_doc			=	db_select('file_managed','fm');
	$uploaded_doc 			->	fields('fm',array());
	$uploaded_doc			->	condition('fm.fid',array($work_order_file_id,$frm_v_file_id,$residential_file_id),'IN');
	$uploaded_doc_result	=	$uploaded_doc -> execute()->fetchAll();

	
	//---------------------------Details of contractor------------------------------------
	
	$contractor_address					=	"";
	$query 								= 	db_select('l_particular_of_contract_labour', 'lpcl');
 	$query								->	fields('lpcl',array())->condition('serial_no_from_v',$serial,'=')->condition('contractor_user_id',$contractor_id,'=')->condition('act_id','1','=');
	$result 							= 	$query->execute();
	if($result->rowCount() > 0 ){
		foreach($result as $obj){
			$contractor_areatype						=	$obj->contractor_areatype; 
			$contractor_manager_areatype				=	$obj->contractor_manager_areatype;
			$work_order_file_id							=	$obj->work_order_file_id;
			$frm_v_file_id								=	$obj->frm_v_file_id;
			$residential_file_id						=	$obj->residential_file_id;
		
			
			$father_contarctor_name						=	$obj->father_contarctor_name;	
			$dob_contractor								=	$obj->dob_contractor;
			$dob_age_contractor							=	$obj->age_contractor;
			$address_of_manager							=	$obj->address_of_manager; 
			$contractor_max_no_of_labours_on_any_day	=	$obj->contractor_max_no_of_labours_on_any_day; 
			$address_line1_of_contractor				=	$obj->address_of_contractor; 
			$category_designation						=	$obj->category_designation;
			$manager_pin								=	$obj-> manager_pin;
			$contractor_pin								=	$obj->contractor_pin;
			
			
			$unskilled_wages							=	$obj->unskilled_wages;
			$unskilled_rate_wages						=	$obj->unskilled_rate_wages;
		
		  	$semiskilled_wages							=	$obj->semiskilled_wages;
			$semiskilled_rate_wages						=	$obj->semiskilled_rate_wages;
			
			
			$skilled_wages								=	$obj->skilled_wages;
			$skilled_rate_wages							=	$obj->skilled_rate_wages;
			
			
			$highlyskilled_wages						=	$obj->highlyskilled_wages;
			$highlyskilled_rate_wages					=	$obj->highlyskilled_rate_wages;
			
			
		  	$hours_work									= 	$obj->hours_work;
		  	$overtime									= 	$obj->overtime;
		  	$overtime_wages								= 	$obj->overtime_wages;
		  	$weekly_holiday								= 	$obj->weekly_holiday;
		  	$no_holiday									= 	$obj->no_holiday;
	  
		  	$annual_leave								= 	$obj->annual_leave;
		  	$annual_leave_no							= 	$obj->annual_leave_no;
		  	$casual_leave								= 	$obj->casual_leave;
		  	$casual_leave_no							= 	$obj->casual_leave_no;
		  	$sick_leave									= 	$obj->sick_leave;
		 	$sick_leave_no								= 	$obj->sick_leave_no;
		  	$other_leave								= 	$obj->other_leave;
		 	$other_leave_no								= 	$obj->other_leave_no;
			$maternity_leave_no							=	$obj->maternity_leave_no;
			$signed_license_id							=	$obj->signed_license_id;
			
			$particularid								=	$obj->id;
			$name_of_agent								=	$obj->name_of_agent;
			$address_of_manager							=	$obj->address_of_manager;
			$contractor_convicted						=	$obj->contractor_convicted;
			$category_of_contractor						=	$obj->category_of_contractor;
			
			
			
			$fetch_license_data 						= 	db_select('l_contractor_license_application', 'license');
  			$fetch_license_data							->	fields('license',array('id','backlog_license_date','backlog_license_no'));
  			$fetch_license_data							->	condition('license.contractor_particular_id',$particularid,'=');
			$license_data 								= 	$fetch_license_data->execute()->fetchAssoc();	
			$license_contractor_id 						= 	$license_data['id'];
			$backlog_license_date 						= 	$license_data['backlog_license_date'];
			$backlog_license_no 						= 	$license_data['backlog_license_no'];
			
			
			
			
			if(!empty($license_contractor_id)){
		
					$renewal_data						=	fetch_all_data_contractor_license_apply_renweal($license_contractor_id,$contractor_id);
					//$license_reneal_fees				=	$renewal_data['license_reneal_fees'];
					$license_reneal_fees				=	$renewal_data['increnent_fees'];
					$earned_leave_no					=	$renewal_data['earned_leave_no'];
					$spred_over							=	$renewal_data['spred_over'];
					$holiday_wages						=	$renewal_data['holiday_wages'];
					$special_benifites					=	$renewal_data['special_benifites'];
					$state_insurance					=	$renewal_data['state_insurance'];
					$miscellaneous_provisions			=	$renewal_data['miscellaneous_provisions'];
					$backlog_renewal_date				=	$renewal_data['backlog_renewal_date'];
					$backlog_renewal_fees				=	$renewal_data['backlog_renewal_fees'];
					$renewal_application_final_status	= 	$renewal_data['renewal_application_final_status'];
					$renewal_appliaction_status			= 	$renewal_data['renewal_appliaction_status'];	
					$upload_backlog_certificate_id		=   $renewal_data['upload_backlog_certificate'];
					$upload_signed_form					=   $renewal_data['upload_signed_form'];
					$is_extra_fees						=   $renewal_data['is_extra_fees'];	
					$created_date						= 	$renewal_data['created_date'];
			}
	
	
	if($backlog_license_date<='2015-11-27'){
		if(empty($backlog_renewal_date)){
			
				$appliyyear					= date('Y', strtotime($created_date));
				$backlog_license_year		= date('Y', strtotime($backlog_license_date));
				$backlog_license_month		= date('m', strtotime($backlog_license_date));
				$backlog_license_day		= date('d', strtotime($backlog_license_date));
				$next_renweal_date			= ($appliyyear+1).'-12-31';
				$renewal_expaire 			=  '31st Dec,'.$backlog_license_year;	
		}else{
				$renewal_expaire			= date("dS M, Y",strtotime($backlog_renewal_date));	
		}
			//echo $next_renweal_date;die();
		}else{
			/*$appliyyear						= date('Y', strtotime($created_date ));
				$backlog_license_year		= date('Y', strtotime($backlog_license_date));
				$backlog_license_month		= date('m', strtotime($backlog_license_date));
				$backlog_license_day		= date('d', strtotime($backlog_license_date));
				$next_renweal_date			= ($appliyyear+1).'-'.$backlog_license_date.'-'.$backlog_license_date;
				$renewal_expaire			= date("dS M, Y",strtotime($next_renweal_date));*/	
				if(empty($backlog_renewal_date)){
					$appliyyear					= date('Y', strtotime($created_date));
					$backlog_license_year		= date('Y', strtotime($backlog_license_date));
					$backlog_license_month		= date('m', strtotime($backlog_license_date));
					$backlog_license_day		= date('d', strtotime($backlog_license_date));
					$next_renweal_date			= ($backlog_license_year+1).'-'.$backlog_license_month.'-'.($backlog_license_day-1);
					
					$renewal_expaire			= date("dS M, Y",strtotime($next_renweal_date));
				}else{
					$renewal_expaire			= date("dS M, Y",strtotime($backlog_renewal_date));	
				}
				
		}
			
			
		$special_benifites_show	=	empty($special_benifites)?' Not Applicable':$special_benifites;
		$state_insurance_show	=	empty($state_insurance)?' Not Applicable':$state_insurance;
		/*	
		if($category_of_contractor==1 && !empty($category_of_contractor)){
			$title = 'Establishment date of the Company';
			$age_contractor='';
		}else{
			$title = 'Date of Birth and age (incase of individuals)';
			$age_contractor= 'Age: '.$dob_age_contractor.' Years';
		}*/
		
		$title = 'Date of Birth and age (incase of individuals)';
			$age_contractor= !empty($dob_contractor)?'DOB:-'.date('dS M, Y',strtotime($dob_contractor)).'<br>Age:-'.(!empty($dob_age_contractor)?$dob_age_contractor:'Nil') :'DOB:-Nil<br> Age:-'.(!empty($dob_age_contractor)?$dob_age_contractor:'Nil');
		

		if($contractor_convicted=='1')
			$details_contractor_convicted=	$obj->details_contractor_convicted;
		else
			$details_contractor_convicted=	'NO';

		$contractor_revoking			=	$obj->contractor_revoking;
		

		$contractor_previous_employer	=	$obj->contractor_previous_employer;
		if($contractor_previous_employer=='1')
			$details_previous_employer=$obj->details_previous_employer;
		else
			$details_previous_employer='NO';
		
		$amount_of_licence				=	$obj->amount_of_licence;
		$amount_of_security_deposit		=	$obj->amount_of_security_deposit;
			
		}
	}

	if(!empty($contractor_areatype)){
		$contractor_address				=	miscellaneous_contractor_update_address_form($contractor_areatype,$serial,$contractor_id);//call from miscellaneous
	}
	$contractor_manager_address			=	'';
	if(!empty($contractor_manager_areatype)){
		$contractor_manager_address		=	miscellaneous_contractor_manager_address_form($contractor_manager_areatype,$serial,$contractor_id)	;
	}
	
	if($father_contarctor_name != ''){
		$father_contarctor_name = $father_contarctor_name.'<br/>';
	}else{
		$father_contarctor_name	= '';
	}
	//---------------------------------------End contractor Address-----------------------------------	
	$output								=	'';
	
	
	// Establishment details


	//Labour
	
	$rest_holiday = substr($no_holiday, 0, -1);
	$no_holiday_list= explode(',', $rest_holiday);
	$name_holiday="";
	$total_holiday=0;
    foreach($no_holiday_list as $row){
		$total_holiday=$total_holiday+1;
		if($row=='1'){
			$name_holiday.='Sunday'.',';
		}
		if($row=='2'){
			$name_holiday.='Monday'.',';
		}
		if($row=='3'){
			$name_holiday.='Tuesday'.',';
		}
		if($row=='4'){
			$name_holiday.='Wednesday'.',';
		}
		if($row=='5'){
			$name_holiday.='Thursday'.',';
		}
		if($row=='6'){
			$name_holiday.='Friday'.',';
		}
		if($row=='7'){
			$name_holiday.='Saturday'.',';
		}
		
	}
	$name_holidays = substr($name_holiday, 0, -1);
	if(empty($name_holidays))
		$name_holidays='Nil';

	//--------------------------------Fetch uploaded documents------------------------------
	
	$uploaded_workorder			=	db_select('file_managed','fm');
	$uploaded_workorder 		->	fields('fm',array());
	$uploaded_workorder			->	condition('fm.fid',$work_order_file_id,'=');
	$uploaded_workorder_file	=	$uploaded_workorder -> execute()->fetchAssoc();
	
	if(!empty($uploaded_workorder_file)){ 
			$url			 	= 	explode('//',$uploaded_workorder_file['uri']);
   			$work_order_url		= 	$url[1];
			$workorder_file		=	$uploaded_workorder_file['filename'];
	}
	
	
	$uploaded_frmv				=	db_select('file_managed','fm');
	$uploaded_frmv 				->	fields('fm',array());
	$uploaded_frmv				->	condition('fm.fid',$frm_v_file_id,'=');
	$uploaded_frmv_file			=	$uploaded_frmv -> execute()->fetchAssoc();
	
	if(!empty($uploaded_frmv_file)){ 
			$url 				= explode('//',$uploaded_frmv_file['uri']);
   			$frmv_url			= $url[1];
			$frmv_file			= $uploaded_frmv_file['filename'];
	}
	
	$uploaded_residential		=	db_select('file_managed','fm');
	$uploaded_residential 		->	fields('fm',array());
	$uploaded_residential		->	condition('fm.fid',$residential_file_id,'=');
	$uploaded_residential_file	=	$uploaded_residential -> execute()->fetchAssoc();
	
	if(!empty($uploaded_residential_file)){ 
			$url				= explode('//',$uploaded_residential_file['uri']);
   			$residential_url	= $url[1];
			$residential_file	= $uploaded_residential_file['filename'];
	}
	
	
	$upload_formiv				=	db_select('file_managed','fm');
	$upload_formiv 				->	fields('fm',array());
	$upload_formiv				->	condition('fm.fid',$signed_license_id,'=');
	$upload_formiv_file			=	$upload_formiv -> execute()->fetchAssoc();
	
	if(!empty($upload_formiv_file)){ 
			$url				= explode('//',$upload_formiv_file['uri']);
   			$formiv_url			= $url[1];
			$formiv_file		= $upload_formiv_file['filename'];
	}
		

	
	
	$upload_backlog_certificates				=	db_select('file_managed','fm');
	$upload_backlog_certificates 				->	fields('fm',array());
	$upload_backlog_certificates				->	condition('fm.fid',$upload_backlog_certificate_id,'=');
	$upload_backlog_certificates_file			=	$upload_backlog_certificates -> execute()->fetchAssoc();
	
	if(!empty($upload_backlog_certificates_file)){ 
			$url								= explode('//',$upload_backlog_certificates_file['uri']);
   			$backlog_certificates_url			= $url[1];
			$backlog_certificates_file			= $upload_backlog_certificates_file['filename'];
	}
	
	
	
	$upload_signed_form_vii				=	db_select('file_managed','fm');
	$upload_signed_form_vii 				->	fields('fm',array());
	$upload_signed_form_vii				->	condition('fm.fid',$upload_signed_form,'=');
	$upload_signed_form_vii_file			=	$upload_signed_form_vii -> execute()->fetchAssoc();
	
	if(!empty($upload_signed_form_vii_file)){ 
			$url								= explode('//',$upload_signed_form_vii_file['uri']);
   			$upload_signed_form_vii_url			= $url[1];
			$upload_signed_form_vii_file_name			= $upload_signed_form_vii_file['filename'];
	}
	
	/*$download_link	=	l('VIEW&nbsp;&amp;&nbsp;PRINT','preview-backlog-renewal-pdf/'.encryption_decryption_fun('encrypt', $serial), array('html' => TRUE, 'attributes' => array('target' => '_blank', 'class' => array('button middle-align'))));*/
	$download_link	=	l('VIEW&nbsp;&amp;&nbsp;PRINT', 'preview-backlog-renewal-pdf/'.encryption_decryption_fun('encrypt', $serial).'/Application Preview'.time(), array('html' => TRUE, 'attributes' => array('target' => '_blank','class' => array('button middle-align'))));

	$output	.= '<style type="text/css">
					table, td, th {border: 1px solid #006595;font-size:15px;font-family:Times New Roman;height:40px;margin-top:20px;}
					td{padding-left:13px;}
					th {background-color: #008BD1;color: white;}
					td:hover {background-color:#d4e3e5;}
					tr:nth-child(even) {background: #DBE5F0}
					tr:nth-child(odd) {background: #F1F1F1}
		  		 </style>
			   
			   <div class="content"><div class="sky-form" style="width:100%;"><fieldset>
				<table width="100%" border="0" cellspacing="0" cellpadding="0">
				
				<tr><td colspan="3" align="center" style="text-align:center;font-size:17px; padding-top: 8px;">
					<font color="#3366FF"><strong>Application preview of Renewal of license</strong></font></td>
				</tr>
				
 				<tr>
					<th width="50%" style="text-align:center;font-size:17px; padding-top: 8px;" >Parameters</th>
					<th width="60%" style="text-align:center;font-size:17px; padding-top: 8px;">Inputs</th>
				</tr>
  					
				<tr>
					<td>1.Name & address of the contractor <br />(including his father&#13;s name incase of individuals) </td>
					<td>'.$name_of_contractor.'<br/>'.$father_contarctor_name.$address_line1_of_contractor.' <br />'. $contractor_address.'</td>
				</tr>
				
				<tr>
					<td>2(a).'.$title.'</td>
					<td>'.$age_contractor.'</td>
				</tr>
				<tr>
					<td>2(b).Number and date of the license:</td>
					<td>'.$backlog_license_no.'  dated  '.date('dS M, Y',strtotime($backlog_license_date)).' </td>
				</tr>
				<tr>
					<td>2(c).Date of expiry of the previous license:</td>
					<td>'.$renewal_expaire.'</td>
				</tr>
				<tr>
					<td colspan="3" align="center" style="text-align:center;font-size:17px; padding-top: 8px;">
					<font color="#3366FF"><strong>3.Particular of establishment where contract labour is to be employed</strong></font></td>
			  	</tr>
				
				<tr>
					<td>(a)Name & address of the establishment</td>
					<td>'.$establishment_address.'</td>
  				</tr>
  				<tr>
					<td>(b)Type of Business, trade, industry, manufacture or occupation which is carried on in the establishment</td>
					<td>'.$nature_work.'</td>
				</tr>
			    <tr>
					<td>(c)Number and date of Certificate</td>
					<td>'.$establish_details_reg.'</td>
			    </tr>
			    <tr>
					<td>(d)Name & Address of the Principal Employer</td>
					<td>'.$principal_details.'</td>
			  </tr>
			  
  			  <tr>
				<td colspan="3" align="center" style="text-align:center;font-size:17px; padding-top: 8px;">
				<font color="#3366FF"><strong>4.Particular of Contract Labour</strong></font></td>
			 </tr>
  
			  <tr>
				<td>(a)Nature of work in which Contract Labour is employed or is to be employed in the establishment</td>
				<td>'.$labour_nature_work.'</td>
			  </tr>
			  <tr>
				<td>(b)Duration of the proposed contract work <br />(give particulars of proposed date of ending) </td>
				<td>'.$startend_date.'<br /> Estimated Duration:  '.$diff.'</td>
			  </tr>
			  <tr>
				<td>(c)Name & address of the agent or Manager of Contractor at the work site</td>
				<td>'.$name_of_agent.'<br /> '.$address_of_manager.'<br /> '.$contractor_manager_address.'</td>
			  </tr>
			  <tr>
				<td>(d)Maximum number of Contract Labour proposed to he employed in the establishment on any date.</td>
				<td>'.$contractor_max_no_of_labours_on_any_day.' ('.convertNumberToWord($contractor_max_no_of_labours_on_any_day).')</td>

			  </tr>
			   <tr>
				<td>(e)(i)Category/designation/nomenclature of the contractor labour, namely,fitter,welder,carpenter,mazdor etc.</td>
				<td>'.$category_designation.'</td>
			  </tr>
			  <tr>
				<td colspan="2">
						<b>(e)(ii)Rate of Wages,DA and other cash benefits paid/ to be paid to each category(i.e (a):Unskilled (b)Semi_Skilled (c)Skilled (d)Highly_Skilled etc.)of contract labour</b>:</td>
			  </tr>
   			  <tr>
    			<td>(a)Rate of Wages,DA and other cash benefits paid/ to be paid to Unskilled of contract labour:</td>';
					if(empty($unskilled_rate_wages)){
						$unskilled_rate_wages_show='Nil';
					}else{
						$unskilled_rate_wages_show='<del>&#2352;</del>'.$unskilled_rate_wages.' ( Rupees '.convertNumberToWord($unskilled_rate_wages). 'only)';
					}
					$output	.='<td>'.$unskilled_rate_wages_show.'</td>
				</tr>';
	
					if(empty($semiskilled_rate_wages)){
						$semiskilled_rate_wages_show='Nil';
					}else{
						$semiskilled_rate_wages_show='<del>&#2352;</del>'.$semiskilled_rate_wages.' ( Rupees  '.convertNumberToWord($semiskilled_rate_wages). 'only)';
					}
   
  		$output.='<tr>
    				<td>(b)Rate of Wages,DA and other cash benefits paid/ to be paid to Semi-skilled of contract labour:</td>
					<td>'.$semiskilled_rate_wages_show.'</td>
				  </tr>';
	
	
				   if(empty($skilled_rate_wages)){
						$skilled_rate_wages_show='Nil';
					}else{
						$skilled_rate_wages_show='<del>&#2352;</del>'.$skilled_rate_wages.' (Rupees '.convertNumberToWord($skilled_rate_wages). 'only)';
					}
   
 		 $output.='<tr>
					<td>(c)Rate of Wages,DA and other cash benefits paid/ to be paid to Skilled of contract labour:</td>
					<td>'.$skilled_rate_wages_show.'</td>
				  </tr>';
	
					if(empty($highlyskilled_rate_wages)){
						$highlyskilled_rate_wages_show='Nil';
					}else{
						$highlyskilled_rate_wages_show='<del>&#2352;</del>'.$highlyskilled_rate_wages.' ( Rupees '.convertNumberToWord($highlyskilled_rate_wages). ' only)';
					}
    
   
	  $output	.='<tr>
					<td>(d)Rate of Wages,DA and other cash benefits paid/ to be paid to Highly-skilled of contract labour:</td>
					<td>'.$highlyskilled_rate_wages_show.'</td>
    			  </tr>
				  <tr>
					<td>(e)(iii)Hours of Work,Spread over time,Overtime and Overtime Wages</td>
					<td>Hours of Work:-'.$hours_work.' hr(s)<br> Spread over time:- '.$spred_over.' hr(s) <br> Overtime:- '.$overtime.' hr(s) <br>Overtime Wages:- <del>&#2352;</del> '.$overtime_wages.'(per hour)</td>
    			  </tr>
  				  <tr>
					<td colspan="2"><b>e(vi)Other Condition of service like leave (annual leave,casual leave,sick leave,maternity leave etc.) Holidays etc.of the contract labour</b></td>
				  </tr>
				  <tr>';
					if(empty($annual_leave_no)){
						$annual_leave_no_show='Nil';
					}else{
						$annual_leave_no_show=$annual_leave_no.' '.'day(s)';
					}
					
					if(empty($casual_leave_no)){
						$casual_leave_no_show='Nil';
					}else{
						$casual_leave_no_show=$casual_leave_no.' '.'day(s)';
					}
					
					if(empty($sick_leave_no)){
						$sick_leave_no_show='Nil';
					}else{
						$sick_leave_no_show=$sick_leave_no.' '.'day(s)';
					}
					
					if(empty($maternity_leave_no)){
						$maternity_leave_no_show='Nil';
					}else{
						$maternity_leave_no_show=$maternity_leave_no.' '.'day(s)';
					}
					
					if(empty($other_leave_no)){
						$other_leave_no_show='Nil';
					}else{
						$other_leave_no_show=$other_leave_no.' '.'day(s)';
					}
					if(empty($holiday_wages)){
						$other_leave_no_show='Nil';
					}else{
						$other_leave_no_show	=	$holiday_wages.' '.'day(s)';
					}
					if(empty($earned_leave_no)){
						$earned_leave_no_show	=	'Nil';
					}else{
						$earned_leave_no_show=$earned_leave_no.' '.'day(s)';
					}
			$output.='<td>Number of Annual leave</td>
					  <td>'.$annual_leave_no_show.'</td>
				  </tr>
				  <tr>
					<td>Number of Casual leave</td>
					<td>'.$casual_leave_no_show.'</td>
				  </tr>
				  <tr>
					<td>Number of Earned leave</td>
					<td>'.$earned_leave_no_show.'</td>
				</tr>
				  <tr>
					  <td>Number of Sick leave</td>
					  <td>'.$sick_leave_no_show.'</td>
				  </tr>
				<tr>
					<td>Number of Maternity leave</td>
					<td>'.$maternity_leave_no_show.'</td>
				</tr>
				<tr>
					<td>Number of Other leave</td>
					<td>'.$other_leave_no_show.'</td>
				</tr>
				<tr>
					<td>Whether weekly holiday(s) observed and on which day</td>
					<td>'.$total_holiday.' day(s) ('.$name_holidays.')</td>
				</tr>
				<tr>
					<td>Whether weekly holiday(s) so observed was paid holiday(s)</td>
					<td>'.$other_leave_no_show.'</td>
				</tr>
				<tr>
					<td>5.Whether the contractor was convicted of any offence within the preceding five years. If so, give details</td>
   					<td>'.$details_contractor_convicted.'</td>
  				</tr>
			   <tr>';
			   
				  if(!empty($details_contractor_revoking)){
					  $revoking_date	= date('dS M, Y', strtotime($details_contractor_revoking));
				  }else{
					  $revoking_date='Not Applicable';
				  }
	   $output.=' <td>6.Whether there was any order against the contract or revoking or suspending license or forfeiting security deposit in respect of an earlier contract. If so, the date of such order.   </td>
				  <td>'.$revoking_date.'</td>
			  </tr>
			  <tr>
				<td>7.Whether the contractor has worked in any other establishment within the past five years. If so. give details of the principal employer, establishment and nature of work</td>
				<td>'.$pervious_license_detais.'</td>
			  </tr>
			  <tr>
				<td>8.Special benefits provided, if any </td>
				<td>'.$special_benifites_show.'</td>
			  </tr>
			   <tr>
				<td>9.Contribution made under the Employees State Insurance Act,1948</td>
				<td>'.$state_insurance_show.'</td>
			  </tr>
			 
			  <tr>
				<td align="center"  colspan="2"><b>Uploaded Documents</b></td>
			  </tr>
			  <tr>
			  	<td>Work Order</td>';
			  		if(!empty($work_order_file_id)){ 
        		$output.='<td><a title = "Click here to view work order" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$work_order_url.'" target="_blank"><img src="'.$GLOBALS['base_url'].'/'. drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
			  		}else{
			   	$output.='<td>No Document uploaded</td>';
			  		}
	$output.='</tr>
			  <tr>
			  	<td>Form V</td>';
			  		if(!empty($frm_v_file_id)){ 
        		$output.='<td><a title = "Click here to view Form V" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$frmv_url.'" target="_blank"><img src="'.$GLOBALS['base_url'].'/'. drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
			  		}else{
			   	$output.='<td>No Document uploaded</td>';
			  		}
	$output.='</tr>
			  <tr>
			  	<td>Residential Certificate</td>';
			  		if(!empty($residential_file_id)){ 
        		$output.='<td><a title = "Click here to view residential certificate" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$residential_url.'" target="_blank"><img src="'.$GLOBALS['base_url'].'/'. drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
			  		}else{
			   	$output.='<td>No Document uploaded</td>';
			  		}
					
			  		
	$output.='</tr>
			  <tr>
			  	<td>Perivious certificates of license or renewal</td>';
			  		if(!empty($upload_backlog_certificate_id)){ 
        		$output.='<td><a title = "Click here to view periviuos certifiactes for renewal" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$backlog_certificates_url.'" target="_blank"><img src="'.$GLOBALS['base_url'].'/'. drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
			  		}else{
			   	$output.='<td>No Document uploaded</td>';
			  		}
					
	$output.='</tr>
			  	<td>Form VII</td>';
			  		if(!empty($upload_signed_form)){ 
        				$output.='<td><a title = "Click here to view form VI" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$upload_signed_form_vii_url.'" target="_blank"><img src="'.$GLOBALS['base_url'].'/'. drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
			  		}else{
			   			$output.='<td>Form -VII to be uploaded with duly signed by the Applicant after fees submission</td>';
			  		}
					
	
	$output.=' </tr> 
			  <tr>
			  	<td align="center" colspan="2" style="text-align:center;"><font color="#000000"><strong>Fees Details</strong></font></td>
			  </tr>
			  <tr>
				<td>8.Whether a certificate by the principal employer in Form V is Enclosed</td>
				<td>YES</td>
			  </tr>
			<tr>
				<td>9.Amount of Licence Fee paid</td>
				<td> <del>&#2352;</del> '.$amount_of_licence.' ( Rupees '.convertNumberToWord($amount_of_licence). ' only)</td>
			</tr>
			<tr>
				<td>10.Amount of security deposit</td>
				<td><del>&#2352;</del> ' .$amount_of_security_deposit.' ( Rupees '.convertNumberToWord($amount_of_security_deposit). ' only)</td>
			</tr>
			<tr>
				<td>10.Amount of last Renewal of Licence Fee paid</td>
				<td><del>&#2352;</del> ' .$backlog_renewal_fees.' ( Rupees '.convertNumberToWord($backlog_renewal_fees). ' only)</td>
			</tr>
			<tr>';
			if($is_extra_fees == '1'){
				$output.='<td>11.Amount of Renewal of Licence Fee</td>
				<td><del>&#2352;</del> ' .$license_reneal_fees.' ( Rupees '.convertNumberToWord($license_reneal_fees). ' only)<font color="#FF0000">[including 25% extra according to R29(3)]</font></td>
			</tr>';
			}else{
				$output.='<td>11.Amount of Renewal of Licence Fee</td>
				<td><del>&#2352;</del> ' .$license_reneal_fees.' ( Rupees '.convertNumberToWord($license_reneal_fees). ' only)</td>
			</tr>';
			}
		$output.='</table><br/>';
		$output.=$download_link.'<br/></fieldset></div></div>';
 
		$action 		= 	'encrypt';
		$serial			=	encryption_decryption_fun($action, $serial_number);//---miscellaneous module
		return $output;
}
?>