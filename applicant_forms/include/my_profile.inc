<?php

function my_profile_view() { 
	global $base_root, $base_path, $user;
	$user_id 		 	= $user->uid;
	
	$id						=	'';
	$f_name					=	'';
	$m_name					=	'';
	$l_name					=	'';
	$dob					=	'';
	$email					=	'';
	$mobile					=	'';
	$gender					=	'';
	
	
	$address				=	'';
	$country				=	'';
	$state					=	'';
	$district_code			=	'';
	$subdivision_code		=	'';
	$areatype_code			=	'';
	$name_areatype_code		=	'';
	$vill_ward_code			=	'';
	$ps_code				=	'';
	$pin					=	'';
	
	$borough_code			=	'';
	
	$ps_code				=	'';
	$pin					=	'';
	
	
	$card_type				=	'';
	$card_number			=	'';
	
	$est_name				= 	'';
	$est_location			= 	'';
	$est_district			=	'';
	$est_subdivision		=	'';
	$est_areatype			=	'';
	$est_areatype_code		=	'';
	$est_loc_e_vill_ward	=	'';
	$est_ps					=	'';
	$est_pin_number			= '';
	
	/*******   ********/
	
	$val_ownership				=	'';
	$val_other_ownership		=	'';
	
	$val_ownership_undertaking	=	'';
	$val_other_government_name	=	'';
	$val_local_bodies			=	'';
	$val_urban_local_bodies		=	'';
	
	
	$val_other_private_name		=	'';
	$master_role_adolescent_male 	= 	'';
	$master_role_adolescent_female	= 	'';
	
	$contractual_labour_adolescent_male	= '';
	$contractual_labour_adolescent_female	= '';
	
	$other_labour_adolescent_male	=   '';
	$other_labour_adolescent_female	=   '';
		
	$fetch_common_det 	= db_query("select * from l_common_application_master where user_id=:userId",array(':userId'=>$user_id));
	
	if( $fetch_common_det->rowCount() > 0 ){
		
		$obj 	= 	$fetch_common_det->fetchAssoc();
		
		$id						=	$obj['id'];
		$f_name					=	$obj['f_name'];
		$m_name					=	$obj['m_name'];
		$l_name					=	$obj['l_name'];
		
		if($obj['dob'] != '1111-11-11'){
			$dob =	date('d/m/Y', strtotime($obj['dob']));
		}else{
			$dob =	'N/A';
		}
		$email					=	$obj['email'];
		$mobile					=	$obj['mobile'];
		$gender					=	$obj['gender'];
		
		$address				=	$obj['address'];
		$country				=	$obj['country'];
		$state					=	$obj['state'];
		$district_code			=	$obj['dist_code'];
		$subdivision_code		=	$obj['subdivision'];
		$areatype_code			=	$obj['areatype'];
		$name_areatype_code		=	$obj['name_areatype'];
		$vill_ward_code			=	$obj['vill_ward'];
		$ps_code				=	$obj['ps'];
		$pin					=	$obj['pin'];
		
		$card_type				=	$obj['card_type'];
		$card_number			=	$obj['card_number'];
		
		$est_name = $obj['est_name'];
		$est_type = $obj['est_type'];
		$est_location			= 	$obj['est_loc'];
		$est_district			=	$obj['est_district'];
		$est_subdivision		=	$obj['est_subdivision'];
		$est_areatype			=	$obj['est_loc_e_areatype'];
		$est_areatype_code		=	$obj['est_name_areatype'];
		$est_loc_e_vill_ward	=	$obj['est_loc_e_vill_ward'];
		$est_ps					=	$obj['est_ps'];
		$est_pin_number			= 	$obj['est_pin_number'];
		$gst_number				= 	$obj['gst_number'];
		
		
		/******* Ownership  ********/
		
		$val_ownership				=	$obj['ownership'];
		$val_other_ownership		=	$obj['other_ownership'];
		
		$val_ownership_undertaking	=	$obj['ownership_undertaking'];
		$val_other_government_name	=	$obj['other_government_name'];
		$val_other_private_name		=	$obj['other_private_name'];
		$val_local_bodies			=	$obj['local_bodies'];
		$val_urban_local_bodies		=	$obj['urban_local_bodies'];
		
		/*******  worker info ********/
		$work_man_master_role_male		=	$obj['work_man_master_role_male'];
		$work_man_master_role_female	=	$obj['work_man_master_role_female'];
		$master_role_adolescent_male 	= 	$obj['work_man_master_role_adolescent_male'];
		$master_role_adolescent_female	= 	$obj['work_man_master_role_adolescent_female'];
		
		$contractual_labour_male		=	$obj['contractual_labour_male'];
		$contractual_labour_female		=	$obj['contractual_labour_female'];
		$contractual_labour_adolescent_male	= $obj['contractual_labour_adolescent_male'];
		$contractual_labour_adolescent_female	= $obj['contractual_labour_adolescent_female'];
		
		$child_labour_male				=	$obj['child_labour_male'];
		$child_labour_female			=	$obj['child_labour_female'];
		$other_labour_adolescent_male	=   $obj['child_labour_adolescent_male'];
		$other_labour_adolescent_female	=   $obj['child_labour_adolescent_female'];		
	}
	$estArr = array('est_district','est_subdivision','est_loc_e_areatype','est_name_areatype','est_loc_e_vill_ward','est_ps','est_pin_number');
	$establishment_address 	= $est_location.'<br/>'.get_full_address('l_common_application_master', 'sub-table', $id, $estArr);
	
	$userArr = array('dist_code','subdivision','areatype','name_areatype','vill_ward','ps','pin');
	$user_address 	= $address.'<br/>'.get_full_address('l_common_application_master', 'sub-table', $id, $userArr);

	if($m_name != ''){$name = $f_name.'&nbsp;'. $m_name.'&nbsp;'.$l_name;} else {$name = $f_name.'&nbsp;'. $l_name;	}
	if($gender == 'M'){$gen = 'Male';} else if($gender == 'F'){$gen = 'Female';} else {$gen = 'Other';}
	if($card_type == 1){$card_t = 'AADHAR';} else if($card_type == 2){$card_t = 'PAN';} else if($card_type == 3){$card_t = 'TAN';}else if($card_type == 4){$card_t = 'LIN';} else {$card_t = 'EPIC';}
	if($val_ownership == 1){$owner_type = 'Government';} else if($val_ownership == 2){$owner_type = 'Private';} else if($val_ownership == 3){$owner_type = 'Cooperative';}else if($val_ownership == 4){$owner_type = 'Trust';} else if($val_ownership == 5){$owner_type = 'NGO';}else if($val_ownership == 6){$owner_type = 'Society';} else {$owner_type = 'Others';}
	if(empty($gst_number)){$gst_number = 'Not Available';}
	
	$edit_User = l("<i class='fa fa-edit' style='color:yellow' 'title'='Edit'></i>", 'applicant-profile-update', array('attributes' =>array('title'=>'Edit'), 'fragment' => 'user_details', 'html' => TRUE));		
	$edit_Est = l("<i class='fa fa-edit'  style='color:yellow' 'title'='Edit'></i>", 'applicant-profile-update', array('attributes' =>array('title'=>'Edit'), 'fragment' => 'est_details', 'html' => TRUE));
	$edit_ownership = l("<i class='fa fa-edit' style='color:yellow' 'title'='Edit'></i>", 'applicant-profile-update', array('attributes' =>array('title'=>'Edit'), 'fragment' => 'owner_details', 'html' => TRUE));
	$edit_Worker = l("<i class='fa fa-edit' style='color:yellow' 'title'='Edit'></i>", 'applicant-profile-update', array('attributes' =>array('title'=>'Edit'), 'fragment' => 'worker_details', 'html' => TRUE));
	$edit_Document = l("<i class='fa fa-edit' style='color:yellow' 'title'='Edit'></i>", 'document-upload', array('attributes' =>array('title'=>'Edit'), 'fragment' => 'worker_details', 'html' => TRUE));
	
	
	
	if($val_ownership_undertaking == 8){$ownership_undertaking = 'Department';}else if($val_ownership_undertaking == 9){$ownership_undertaking = 'Directorate';}else if($val_ownership_undertaking == 10){ $ownership_undertaking = 'Public Sector Undertaking';}else if($val_ownership_undertaking == 11){$ownership_undertaking = 'Local Bodies';} else if($val_ownership_undertaking == 15){$ownership_undertaking = 'Other';}else if($val_ownership_undertaking == 16){$ownership_undertaking = 'Proprietor';}else if($ownership_undertaking == 17){$val_ownership_undertaking = 'Partnership';}else if($val_ownership_undertaking == 18){ $ownership_undertaking = 'Joint Venture Company';}else if($val_ownership_undertaking == 19){ $ownership_undertaking = 'Others';}else if($val_ownership_undertaking == 28){$ownership_undertaking = 'Factory Manager';}
	if($val_local_bodies == 20){$local_bodies = 'Zilla Parishad';}else if($val_local_bodies == 21){$local_bodies = 'Panchayat Samiti';}else if($val_local_bodies == 22){$local_bodies = 'Gram Panchayat';}else if($val_local_bodies == 23){$local_bodies = 'Urban Local Bodies';}
	if($val_urban_local_bodies == 24){$urban_local_bodies = 'Municipal Corporation';}else if($val_urban_local_bodies == 25){$urban_local_bodies = 'Municipalities';}else if($val_urban_local_bodies == 26){ $urban_local_bodies = 'Development Authorities';}else if($val_urban_local_bodies == 27){$urban_local_bodies = 'Notified Area';}
	
	$output = '';
	$output .= '<div class="content"><div class="panel panel-default panel-form">
					<div class="panel-heading" style="overflow:hidden;"><i class="fa fa-clipboard"></i>&nbsp;Establishment Information<div class="pull-right">'.$edit_Est.'</div></div>
					<div class="panel-body">
					<div class="row">
					<div class="col-lg-12 col-md-12">
						<table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-striped table-responsive dashboard-table sub-table-dashboard">
							  <tbody>
							  	 <tr>
									<td width="40%">Name of the Establishment </td>
									<td><strong>'.$est_name.'</strong></td>
								 </tr>
								 <tr>
									<td width="40%">Type of the Establishment </td>
									<td><strong>'.ucfirst($est_type).'</strong></td>
								 </tr>
								 <tr>
									<td>Address of the Establishment</td>
									<td><strong>'.$establishment_address.'</strong></td>
								  </tr>
								  <tr>
									<td>GST Number</td>
									<td><strong>'.$gst_number.'</strong></td>
								  </tr>
							 </tbody>
						</table>
					</div>
					</div>
					</div>
					</div>
				</div>';
	$output .=  '<div class="content"><div class="panel panel-default panel-form">
					<div class="panel-heading" style="overflow:hidden;"><i class="fa fa-clipboard"></i>&nbsp;Basic User Information<div class="pull-right">'.$edit_User.'</div></div>
					<div class="panel-body">
					<div class="row">
					<div class="col-lg-12 col-md-12">
						<table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-striped table-responsive dashboard-table sub-table-dashboard">
							<tbody>
								 <tr>
									 <td width="40%">Name</td>
									 <td><strong>'.$name.'</strong></td>
								 </tr>
								 <tr>
									 <td>Email Id</td>
									 <td><strong>'.$email.'</strong></td>
								 </tr>
								 <tr>
									 <td>Mobile Number</td>
									 <td><strong>'.$mobile.'</strong></td>
								 </tr>
								 <tr>
									 <td>Gender</td>
									 <td><strong>'.$gen.'</strong></td>
								 </tr>
								 <tr>
									 <td>Date Of Birth</td>
									 <td><strong>'.$dob.'</strong></td>
								 </tr>
								 <tr>
									 <td>Card Type</td>
									 <td><strong>'.$card_t.'</strong></td>
								 </tr>
								 <tr>
									 <td>Card Number</td>
									 <td><strong>'.$card_number.'</strong></td>
								 </tr>
								 <tr>
									 <td>Address</td>
									 <td><strong>'.$user_address.'</strong></td>
								 </tr>
							   </tbody>
						</table>
					</div>
					</div>
					</div>
					</div>
				</div>'	;
	$output .= '<div class="content"><div class="panel panel-default panel-form">
					<div class="panel-heading" style="overflow:hidden;"><i class="fa fa-clipboard"></i>&nbsp;OWNERSHIP DETAILS OF ESTABLISHMENT OF WHICH APPLICATION TO BE SUBMITED<div class="pull-right">'.			    $edit_ownership.'</div></div>
					<div class="panel-body">
					<div class="row">
					<div class="col-lg-12 col-md-12">
						<table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-striped table-responsive dashboard-table sub-table-dashboard">
							  <tbody>
							  	 <tr>
									<td width="40%">Type of Ownership</td>
									<td><strong>'.$owner_type.'</strong></td>
								 </tr>';
					if($val_ownership == 1){
		$output .= '<tr>
						<td>Type of Government Ownerships </td>
						<td><strong>'.$ownership_undertaking.'</strong></td>
				 	</tr>';
				if($val_ownership == 1 && $val_ownership_undertaking == 11){
		$output .= '<tr>
						<td>Type of Local Bodies </td>
						<td><strong>'.$local_bodies.'</strong></td>
					</tr>';
				if($val_ownership == 1 && $val_ownership_undertaking == 11 && $val_local_bodies == 23){
		$output .= '<tr>
						<td>Type of Urban Local Bodies</td>
						<td><strong>'.$urban_local_bodies.'</strong></td>
					</tr>';
					}
				}
				if($val_ownership == 1 && $val_ownership_undertaking == 15){
		$output .= '<tr>
						<td>Name of Other Government Ownerships </td>
						<td><strong>'.$val_other_government_name.'</strong></td>
					</tr>';
					}
				 }else if($val_ownership == 2){
		$output .= '<tr>
						<td>Type of Private Ownerships </td>
						<td><strong>'.$ownership_undertaking.'</strong></td>
					</tr>';
					if($val_ownership == 2 && $val_ownership_undertaking == 19){
		$output .= '<tr>
						<td>Name of Other Private Ownerships </td>
						<td><strong>'.$val_other_private_name.'</strong></td>
					</tr>';				
						}
				 }else if($val_ownership == 7){
					if($val_other_ownership==''){
						$default_value_ownership = 'Not Mentioned';
					}else{
						$default_value_ownership = $val_other_ownership;
					}
	
		$output .= '<tr>
						<td>Name of Other Ownerships </td>
						<td><strong>'.$default_value_ownership.'</strong></td>
					</tr>';
					}
		$output .= '</tbody>
					</table>
					</div>
					</div>
					</div>
					</div>
				</div>';
	$output .= '<div class="content"><div class="panel panel-default panel-form">
					<div class="panel-heading" style="overflow:hidden;"><i class="fa fa-clipboard"></i>&nbsp;Worker Details<div class="pull-right">'.$edit_Worker.'</div></div>
					<div class="panel-body">
					<div class="row">
					<div class="col-lg-12 col-md-12">
						<table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-striped table-responsive dashboard-table sub-table-dashboard">
							  <tbody>
							  	 <tr>
									<td width="40%"></td>
									<td width="15%">Male</td>
									<td width="15%">Female</td>
									<td width="15%">Adolescent Male</td>
									<td width="15%">Adolescent Female</td>
								 </tr>
								 <tr>
									<td width="40%">No of workman of master role / regular</td>
									<td width="15%">'.$work_man_master_role_male.'</td>
									<td width="15%">'.$work_man_master_role_female.'</td>
									<td width="15%">'.$master_role_adolescent_male.'</td>
									<td width="15%">'.$master_role_adolescent_female.'</td>
								 </tr>
								 <tr>
									<td width="40%">No of contractual labour</td>
									<td width="15%">'.$contractual_labour_male.'</td>
									<td width="15%">'.$contractual_labour_female.'</td>
									<td width="15%">'.$contractual_labour_adolescent_male.'</td>
									<td width="15%">'.$contractual_labour_adolescent_female.'</td>
								 </tr>
								 <tr>
									<td width="40%">No of other worker engaged</td>
									<td width="15%">'.$child_labour_male.'</td>
									<td width="15%">'.$child_labour_female.'</td>
									<td width="15%">'.$other_labour_adolescent_male.'</td>
									<td width="15%">'.$other_labour_adolescent_female.'</td>
								 </tr>
							  </tbody>
						</table>
					</div>
					</div>
					</div>
					</div>
				</div>';
				
		/******************* Uploaded Documents **********************/	
		
		$doc_arr['TL'] = 'Not uploaded';
		$doc_arr['PD'] = 'Not uploaded';
		$doc_arr['AOA'] = 'Not uploaded';
		$doc_arr['MOA'] = 'Not uploaded';
		$doc_arr['FL'] = 'Not uploaded';
		$doc_arr['AC'] = 'Not uploaded';
		$doc_arr['PC'] = 'Not uploaded';
		$doc_arr['BB'] = 'Not uploaded';
		$doc_arr['SC'] = 'Not uploaded';
		$doc_arr['IC'] = 'Not uploaded';
		$doc_arr['AP'] = 'Not uploaded';
		$doc_arr['ODSC'] = 'Not uploaded';
		$doc_arr['CR'] = 'Not uploaded';
		$doc_arr['WO'] = 'Not uploaded';
		$doc_arr['ORC'] = 'Not uploaded';
		
		$filepathSave 	= 'sites/default/files/upload/common/';
		
		$img  			= '<img src="'.$base_root.$base_path.drupal_get_path('theme', 'jackson').'/images/pdf.png">' ;
		
		$query 			= db_select('l_encrypted_uploaded_documents','leud')->fields('leud',array())->condition('user_id',$user_id)->condition('status',1);
		$query_result 	= $query->execute();
		
		if($query_result->rowCount() > 0){
			$data 			= $query_result->fetchAll();
			
			foreach($data as $key){
				$content		= base64_decode($key->filecontent);
					
				file_put_contents($filepathSave.$key->filename,$content);
				$doc_arr[$key->document_type_code] = l($img, $filepathSave.$key->filename, array('html' => TRUE, 'attributes'=> array('target'=>'_blank')));
			}
		}else{
			$view_link 	= '/';
			$f_id		= 0;
		}	
		
		
		$output .= '<div class="content"><div class="panel panel-default panel-form">
					<div class="panel-heading" style="overflow:hidden;"><i class="fa fa-clipboard"></i>&nbsp; Uploaded Documents<div class="pull-right">'.$edit_Document.'</div></div>
					<div class="panel-body">
					<div class="row">
					<div class="col-lg-12 col-md-12">
						<table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-striped table-responsive dashboard-table sub-table-dashboard">
							  <tbody>
							  	 <tr>
									<td width="5%">1. </td>
									<td width="40%"><strong>Trade License</strong></td>
									<td>'.$doc_arr['TL'].'</td>
								 </tr>
								 <tr>
									<td width="5%">2. </td>
									<td width="40%"><strong>Partnership Deed</strong></td>
									<td>'.$doc_arr['PD'].'</td>
								 </tr>
								 <tr>
									<td width="5%">3. </td>
									<td><strong>Article of Association and Memorandum of Association / Partership Deed</strong></td>
									<td>'.$doc_arr['AOA'].'</td>
								 </tr>
								 <tr>
									<td width="5%">4. </td>
									<td><strong>Memorandum of Association</strong></td>
									<td>'.$doc_arr['MOA'].'</td>
								 </tr>
								 <tr>
									<td width="5%">5. </td>
									<td><strong>Factory License (if any)</strong></td>
									<td>'.$doc_arr['FL'].'</td>
								 </tr>
								 <tr>
									<td width="5%">6. </td>
									<td><strong>Form I for Assesment of CESS</strong></td>
									<td>'.$doc_arr['AC'].'</td>
								 </tr>
								 <tr>
									<td width="5%">7. </td>
									<td><strong>Documents in support of Payment of CESS</strong></td>
									<td>'.$doc_arr['PC'].'</td>
								 </tr>
								 <tr>
									<td width="5%">8. </td>
									<td><strong>Blue Book Issued by Motor Vehicles</strong></td>
									<td>'.$doc_arr['BB'].'</td>
								 </tr>
								 <tr>
									<td width="5%">9. </td>
									<td><strong>Smart Card Issued by Motor Vehicles</strong></td>
									<td>'.$doc_arr['SC'].'</td>
								 </tr>
								 
								 <tr>
									<td width="5%">10. </td>
									<td><strong>Insurance Certificate of Motor Vehicles</strong></td>
									<td>'.$doc_arr['IC'].'</td>
								 </tr>
								 <tr>
									<td width="5%">11. </td>
									<td><strong>Address Proof</strong></td>
									<td>'.$doc_arr['AP'].'</td>
								</tr>
								<tr>
									<td width="5%">12. </td>
									<td><strong>Any other document in support of correctness of the particulars mentioned in the application if required</strong></td>
									<td>'.$doc_arr['ODSC'].'</td>
								</tr>
								<tr>
									<td width="5%">13. </td>
									<td><strong>Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc.</strong></td>
									<td>'.$doc_arr['CR'].'</td>
								</tr>																
							 </tbody>
						</table>
					</div>
					</div>
					</div>
					</div>
				</div>';						
		
		return $output;
	}
	
	/*<tr>
		<td width="5%">14. </td>
		<td><strong>Work order for BOCWA registration</strong></td>
		<td>'.$doc_arr['WO'].'</td>
	</tr>
	<tr>
		<td width="5%">15. </td>
		<td>Old Registration Certificate</td>
		<td>'.$doc_arr['ORC'].'</td>
	</tr>*/