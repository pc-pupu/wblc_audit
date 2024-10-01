<?php
function get_rlo_information($rlo_id = ''){	
	$sub_div_code = encryption_decryption_fun('decrypt', $rlo_id);
	
	$rlo_info_query = db_select('l_rlo_additional_info', 'lrai');	
	$rlo_info_query->leftJoin('district_master', 'dm', 'dm.district_code = lrai.district_code'); 
	$rlo_info_query->leftJoin('l_ofc_map_address', 'loma', 'loma.area_code = lrai.sub_div_code');
	$rlo_info_query->leftJoin('sub_division', 'sd', 'sd.sub_div_code=lrai.sub_div_code');
		
	$rlo_info_query->fields('lrai', array('office_designation', 'clra_certificate_address', 'office_name', 'about_office', 'pin_number', 'office_mobile_number', 'office_email_address'));
	$rlo_info_query->fields('dm', array('district_name'));
	$rlo_info_query->fields('sd', array('sub_div_name'));
	$rlo_info_query->fields('loma', array('map_link'));
							
	$rlo_info_query->condition('lrai.sub_div_code',$sub_div_code,'=');
		
	$rlo_info_query = $rlo_info_query->execute();
	$rlo_info_result = $rlo_info_query->fetchAssoc();
	
	// echo '<pre>';
	// print_r($rlo_info_result); die;
	
	if ($rlo_info_result['office_name']=='DLC'){
		$office_name = 'OFFICE OF THE DEPUTY LABOUR COMMISSIONER';
	}elseif($rlo_info_result['office_name']=='ALC'){
		$office_name = 'OFFICE OF THE ASSISTANT LABOUR COMMISSIONER';
	}elseif($rlo_info_result['office_name']=='JLC'){
		$office_name = 'OFFICE OF THE JOINT LABOUR COMMISSIONER';
	}elseif($rlo_info_result['office_name']=='INSPECTOR'){
		$office_name = strtoupper($dataUserInfo->block_mun_name).', LABOUR WELFARE FACILITATION CENTRE';
	}elseif($rlo_info_result['office_name']=='KLC'){
		$office_name = 'OFFICE OF THE LABOUR COMMISSIONER EL & MW SECTION';
	}
	
	if(!empty($rlo_info_result['pin_number'])){ $pin_number = $rlo_info_result['pin_number'];}else{ $pin_number = 'Not available';}
	if(!empty($rlo_info_result['office_mobile_number'])){ $office_mobile_number = $rlo_info_result['office_mobile_number'];}else{ $office_mobile_number = 'Not available';}
	if(!empty($rlo_info_result['office_email_address'])){ $office_email_address = $rlo_info_result['office_email_address'];}else{ $office_email_address = 'Not available';}
	
	$emain_arr = explode('@',$office_email_address);
	$email_part1 = $emain_arr[0].'[at]';
	$email_part2 = str_replace(".","[dot]",$emain_arr[1]);
	
	$email_final = $email_part1.$email_part2;
	
	// $office_emp_details = ''; <tr><td><strong>Mobile Number</strong></td><td>'.$office_mobile_number.'</td></tr>	
	$form ['rlo-info']['name']=array(								
								'#type'		=> 'markup',													
								'#markup' 	=> '<table cellpadding="0" cellspacing="0" border="0" width="100%" class="view-act-rules-table">
													<tr><td width="20%"><strong>Office Address </strong></td><td>'.$rlo_info_result['clra_certificate_address'].'</td></tr>
													<tr><td><strong>District </strong></td><td>'.$rlo_info_result['district_name'].'</td></tr>
													<tr><td><strong>Sub division</strong></td><td>'.$rlo_info_result['sub_div_name'].'</td></tr>
													<tr><td><strong>Pin Code</strong></td><td>'.$pin_number.'</td></tr>
													<tr><td><strong>Email Address</strong></td><td>'.$email_final.'</td></tr>																										
												</table>
												<div class="office_info_tab_details">
												<input id="tab1" type="radio" class="tab_rol_info" name="tabs" checked="checked">
												<label for="tab1">About Office</label>    
												<input id="tab2" type="radio" class="tab_rol_info" name="tabs">
												<label for="tab2">Officers</label>    
												<input id="tab3" type="radio" class="tab_rol_info" name="tabs">
												<label for="tab3">Gallery</label>    
												<input id="tab4" type="radio" class="tab_rol_info" name="tabs">
												<label for="tab4">Map</label>
												<input id="tab5" type="radio" class="tab_rol_info" name="tabs">
												<label for="tab5">Others Activity</label>												  
  												<section id="content1">'.$rlo_info_result['about_office'].'</section>
												<section id="content2">'.rlo_office_contact_view($sub_div_code).'</section>
												<section id="content3">'.get_rlo_publish_photo($sub_div_code).'</section>
												<section id="content4"><iframe src="https://www.google.com/maps/'.$rlo_info_result['map_link'].'" width="100%" height="600" frameborder="0" style="border:0" allowfullscreen></iframe></section>
												<section class="officr_info_content" id="content5">Not Available</section>
												</div>'															
								);
						
	
	return $form;					
	
}

function get_rlo_publish_photo($sub_div_code = ''){
	global $base_root, $base_path, $user;
	$pg_query = db_select('l_photo_gallery', 'lpg');
	$pg_query->fields('lpg', array('id', 'title', 'file_name', 'is_active', 'upload_by_uid','enc_file'));	
	$pg_query->condition('lpg.is_deleted', 0);
	$pg_query->condition('lpg.is_active', 1);
	$pg_query->condition('lpg.sub_div_code', $sub_div_code);
	$pg_query->orderBy('lpg.id', 'desc');
	
	$pg_query = 	$pg_query->execute();
		
	$gallery_arr = '';
	$i=0;
	if($pg_query->rowCount() > 0){
		while($query_result = $pg_query->fetchAssoc()){
			
			$dot = '';
			if(strlen($query_result['title']) > 15) $dot = '...';
			$title = strtoupper(substr($query_result['title'],0 ,15)).$dot;
			$img=$query_result['enc_file'];
			if(!empty($query_result['file_name'])){
				$file_name = $query_result['file_name'];
			}else{
				$file_name = 'no-photo-in-gallerey.png';
			}
			
			if(!empty($img)){
				$i++;		
				$gallery_arr.= '<li class="col-md-2 photo-gallery" id="photo_'.$query_result['id'].'"><a href="data:image/jpg;base64,'.$img.'" rel="lightbox[field_gallery_image]['.$title.']"><img alt="" src="data:image/jpg;base64,'.$img.'" style="height:auto; width:100%" alt="'.str_replace(" ", "-", $query_result['title']).'"><br><span class="photo_title" title="'.$query_result['title'].'">'.$title.'</span></a></li>';
			}
		}
	}
	
	if($i==0){
		$gallery_arr.= '<li>Photo not available</li>';
	}
	
	$html = '';	
	$html .= '<div class="publish_photo_gallery">';	
	$html .= '<ul>'.$gallery_arr.'</ul>';
	$html .= '</div>';
	return $html;
}

function get_office_information($ofc_id = ''){
	
	$ofc_id = encryption_decryption_fun('decrypt', $ofc_id);
	
	$rlo_info_query = db_select('map_table', 'mt');			
	$rlo_info_query->fields('mt', array());							
	$rlo_info_query->condition('mt.srlno',$ofc_id,'=');
		
	$rlo_info_query = $rlo_info_query->execute();
	$rlo_info_result = $rlo_info_query->fetchAssoc();
		
	$form ['rlo-info']['name']=array(								
								'#type'		=> 'markup',													
								'#markup' 	=> '<table cellpadding="0" cellspacing="0" border="0" width="100%">
													<tr><td width="50%">
													<div><strong>'.$rlo_info_result['rlo_name'].'</strong><br>'.$rlo_info_result['designation'].'<br><br>'.$rlo_info_result['address'].'<br>Contact No: '.$rlo_info_result['telephone_no'].'</div>
													</td>
														<td valign="top">'.$office_emp_details.'</td>
													</tr>
												</table>
												<div class="office_info_tab_details">
												<input id="tab1" type="radio" class="tab_rol_info" name="tabs" checked="checked">
												<label for="tab1">About Office</label>    
												<input id="tab2" type="radio" class="tab_rol_info" name="tabs">
												<label for="tab2">Gallery</label>    
												<input id="tab3" type="radio" class="tab_rol_info" name="tabs">
												<label for="tab3">Review</label>    
												<input id="tab4" type="radio" class="tab_rol_info" name="tabs">
												<label for="tab4">Map</label>
												<input id="tab5" type="radio" class="tab_rol_info" name="tabs">
												<label for="tab5">Others Activity</label>												  
  												<section id="content1">'.$rlo_info_result['about_office'].'</section>
												<section id="content2">Not Fount</section>
												<section id="content3">Not Available</section>
												<section id="content4"><iframe width="850" height="400" frameborder="0" scrolling="no" marginheight="0" marginwidth="0" src="https://www.google.co.in/maps/place/New+Secretariat+Building/@22.5697792,88.3434463,15z/data=!4m5!3m4!1s0x0:0xca19a1b2688346e!8m2!3d22.5697792!4d88.3434463"></iframe></section>
												<section class="officr_info_content" id="content5">Not Available</section>
												</div>'															
								);
						
	
	return $form;	
}
function office_wise_contact_info(){
	// $content = '<table cellpadding="0" cellspacing="0" border="0" width="100%" class="table table-striped table-responsive admin-custom-table">';
	$header = array(
           // array('data'=> 'Office', 'width' => '15%'),	
		   array('data'=> 'Name and Address of the office'),
		   array('data'=> 'Action', 'width' => '13%')	  
		  );
	
	$query = db_select('sub_division', 'sd');
	$query->leftJoin('l_rlo_additional_info', 'rai', 'sd.sub_div_code = rai.sub_div_code');
	// $query->leftJoin('block_mun_master', 'bmm', 'bmm.block_code = rai.sub_div_code');	
	$query->fields('rai', array('id', 'office_name', 'office_designation', 'office_mobile_number', 'office_email_address', 'clra_certificate_address'));
	$query->fields('sd', array('sub_div_code', 'sub_div_name'));
	// $query->fields('bmm', array('block_mun_name'));
	// $query->condition('rai.office_name', strtoupper($office), '=');
	$query->orderBy('office_name', 'asc');
	
	$query = $query->execute(); //->fetchAll(); print_r($query); die;
	$i = 1;
	
	if($query->rowCount()>0){
		foreach($query as $data){
	
			if ($data->office_name == 'JLC'){
				$office_name = 'OFFICE OF THE JOINT LABOUR COMMISSIONER';
			}elseif ($data->office_name == 'DLC'){
				$office_name = 'OFFICE OF THE DEPUTY LABOUR COMMISSIONER';
			}elseif($data->office_name == 'ALC'){
				$office_name = 'OFFICE OF THE ASSISTANT LABOUR COMMISSIONER';
			}elseif($data->office_name == 'KLC'){
				$office_name = 'OFFICE OF THE LABOUR COMMISSIONER EL & MW SECTION';
			}elseif($data->office_name == 'INSPECTOR'){
				$office_name = strtoupper($data->block_mun_name).', LABOUR WELFARE FACILITATION CENTRE';
			}
			if(empty($data->office_mobile_number)){$office_mobile_number = 'Not Available';}else{$office_mobile_number = $data->office_mobile_number;}
			if(empty($data->office_email_address)){
				$office_email_address = 'Not Available';
			}else{
				$office_email_address1 = str_replace(".","[dot]",$data->office_email_address);
				$office_email_address = str_replace("@","[at]",$office_email_address1);
				 
			}
			
			$viewmore = l('<i class="fa fa-info-circle"></i> View More', 'office-contact-details/'.encryption_decryption_fun('encrypt',$data->sub_div_code), array('html' => TRUE,'attributes'=> array('target'=>'_blank')));
	
			$rows[] = array(
						'<strong>'.$office_name.', '.strtoupper($data->sub_div_name).'</strong><br>'.$data->clra_certificate_address.'<br>Phone: '.$office_mobile_number.'<br>Email Address: '.$office_email_address,
						$viewmore		
					);
		}
	}
	
	$variables = array(
	  		'attributes' 		=> array('class' => array('view-act-rules-table fixed_headers')), 
	  		'header' 			=> $header,
	  		'rows'				=> $rows,
			'empty' 			=> t("No data found !")
	  );
	
	$output  .= theme('datatable', $variables);
		
	return $output;
}
function office_contact_view($sub_div_code = ''){
	$query = db_select('l_rlo_additional_info', 'rai');
	$query->leftJoin('sub_division', 'sd', 'sd.sub_div_code = rai.sub_div_code');
	$query->fields('rai', array('id', 'office_name', 'office_designation', 'office_mobile_number', 'office_email_address', 'clra_certificate_address'));
	$query->fields('sd', array('sub_div_code', 'sub_div_name', 'district_name','district_code'));
	$query->condition('rai.sub_div_code', encryption_decryption_fun('decrypt', $sub_div_code), '=');
	$obj = $query->execute()->fetchObject();
	
	if ($obj->office_name == 'JLC'){
		$office_name = 'OFFICE OF THE JOINT LABOUR COMMISSIONER';
	}elseif ($obj->office_name == 'DLC'){
		$office_name = 'OFFICE OF THE DEPUTY LABOUR COMMISSIONER';
	}elseif($obj->office_name == 'ALC'){
		$office_name = 'OFFICE OF THE ASSISTANT LABOUR COMMISSIONER';
	}elseif($obj->office_name == 'KLC'){
		$office_name = 'OFFICE OF THE LABOUR COMMISSIONER EL & MW SECTION';
	}elseif($obj->office_name == 'INSPECTOR'){
		// $office_name = strtoupper($data->block_mun_name).', LABOUR WELFARE FACILITATION CENTRE';
	}
	
	$query1 = db_select('l_customuser_relation_address', 'cr');
	$query1->leftJoin('l_custom_user_detail', 'cud', 'cud.usr_id = cr.user_id');
	$query1->leftJoin('users', 'u', 'u.uid = cr.user_id');
	$query1->fields('cud', array('srlno', 'usr_id', 'user_place,', 'fullname', 'mobile', 'degisnation', 'officenumber', 'retirement_date', 'employee_id', 'dob', 'govt_mobile', 'govt_email'));
	$query1->fields('cr', array('id'));
	$query1->fields('u', array('mail'));
	$query1->condition('cr.sub_div_code', encryption_decryption_fun('decrypt', $sub_div_code), '=');
	$query1->orderby('cud.usr_rid', 'asc');
	$query1->orderby('cud.dob', 'desc');
	$query1 = $query1->execute();
	
	if($query1->rowCount()>0){
		$header = array(
			   array('data'=> 'Sl. No.', 'width' => '8%'),
			   array('data'=> 'Office Posting', 'width' => '20%'),
			   array('data'=> 'Name'),
			   array('data'=> 'Designation', 'width' => '25%'),
			   array('data'=> 'Mobile', 'width' => '12%'),			  
			   array('data'=> 'Action', 'width' => '10%')	  
		  	);
		$i = 1;
		$offficer_data = '';
		
		foreach($query1 as $data){
			
			$office_email_address1 = str_replace(".","[dot]",$data->govt_email);
			$office_email_address = str_replace("@","[at]",$office_email_address1);
			
			$viewinfo = '<a href="javascript:void(0)" data-toggle="modal" data-target="#myModal_'.$i.'"><i class="fa fa-info-circle"></i> View</a>
						 <div class="modal fade" id="myModal_'.$i.'" role="dialog">
							<div class="modal-dialog">
								<div class="box box-primary box-solid">
									<div class="box-header">
										<button type="button" class="close" data-dismiss="modal">&times;</button>
										<h3 class="box-title">'.$data->user_place.'</h3>
									</div>
									<div class="modal-body">
										<table cellpadding="0" cellspacing="0" border="0" class="view-act-rules-table fixed_headers">
											<tr>
												<td>Name</td>
												<td>'.$data->fullname.'</td>
											 </tr>
											 <tr>
												 <td>Designation(Posting)</td>
												 <td>'.$data->degisnation.'</td>
											 </tr>
											 <tr>
												 <td>Designation</td>
												 <td>'.$data->degisnation_officer.'</td>
											 </tr>
											 <tr>
												 <td>Office Contact Number</td>
												<td>'.$data->officenumber.'</td>
											 </tr>
											 <tr>
												 <td>Mobile</td>
												<td>'.$data->govt_mobile.'</td>
											 </tr>
											 <tr>
												 <td>Email</td>
												 <td>'.$office_email_address.'</td>
											 </tr>											 
										 </table>
										<div class="modal-footer">
										<a href="/conatact-update/'.encryption_decryption_fun('encrypt', $data->srlno).'/'.encryption_decryption_fun('encrypt', $data->usr_id).' class="btn btn-primary" style="color:#fff;"><i class="fa fa-edit"></i> Edit</a>
										<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
									</div>
									</div>
							   </div>
						</div>';
			$rows[] = array(
						$i++,
						$data->user_place,
						'<strong>'.$data->fullname.'</strong>',						
						$data->degisnation,
						$data->mobile,						
						$viewinfo 	
					);
					
		}
	}
	
	$offficer_data = theme('datatable',array('attributes' => array('class' => array('view-act-rules-table fixed_headers')), 'header' => $header, 'rows' => $rows, 'empty' => t("No data found !")));
	
	// others user details
	$header1 = array(
			   array('data'=> 'Sl. No.', 'width' => '8%'),	
			   array('data'=> 'Name'),
			   array('data'=> 'Designation', 'width' => '25%'),
			   array('data'=> 'Mobile', 'width' => '12%'),
			   array('data'=> 'Email', 'width' => '20%'),
			   // array('data'=> 'Action', 'width' => '10%')	  
		  	);
			
	$query2 = db_select('l_office_emp_details', 'oed');
	$query2->fields('oed', array('emp_id', 'name', 'designation', 'mobile', 'personal_email'));
	$query2->condition('oed.present_posting_subdivision_code', encryption_decryption_fun('decrypt', $sub_div_code), '=');
	$query2 = $query2->execute();
	
	if($query2->rowCount()>0){
		foreach($query2 as $data){
			
			$personal_email1 = str_replace(".","[dot]",$data->personal_email);
			$personal_email = str_replace("@","[at]",$personal_email1);
			
			$rows1[] = array(
						$i++,
						'<strong>'.$data->name,
						$data->designation,
						$data->mobile,
						$personal_email,
						// $editinfo		
			);	
		}
	}
	$othersoffficer_data = theme('datatable',array('attributes' => array('class' => array('view-act-rules-table fixed_headers')), 'header' => $header1, 'rows' => $rows1, 'empty' => t("No data found !")));
	
	$office_email_address1 = str_replace(".","[dot]",$obj->office_email_address);
	$office_email_address = str_replace("@","[at]",$office_email_address1);
	
	$output = '<div class="box box-primary"><div class="box-body"><div class="row"><div class="col-md-6">
    				<div class="box box-primary box-solid">
						<div class="box-header with-border"><i class="ion ion-clipboard"></i> Name and Address of the office</div>
    					<div class="box-body">
							<div class="feedback-scroll">
							<table width="100%" border="0" class="view-act-rules-table fixed_headers">
								<tr>
								    <td style="width:25%"><strong>Name</strong></td>
								    <td style="width:75%"><strong>'.$office_name.'</strong></td>
								</tr>
								<tr>
								    <td><strong>Address</strong></td>
								    <td>'.$obj->clra_certificate_address.'</td>
								</tr>
								<tr>
								    <td><strong>Subdivision</strong></td>
								    <td>'.$obj->sub_div_name.'</td>
								</tr>
								<tr>
								    <td><strong>District</strong></td>
								    <td>'.$obj->district_name.'</td>
								</tr>
								<tr>
								    <td><strong>Contact Number</strong></td>
								    <td>'.$obj->office_mobile_number.'</td>
								</tr>
								<tr>
								    <td><strong>Email Address</strong></td>
								    <td>'.$office_email_address.'</td>
								</tr>
							</table>
							</div>
						</div>
					</div>
			   </div>
			   <div class="col-md-6">
					<div class="box box-primary box-solid">
						<div class="box-header with-border"><i class="ion ion-clipboard"></i> Others Information</div>
						<div class="box-body">
							<div class="feedback-scroll">
							<table width="100%" border="0" class="view-act-rules-table fixed_headers">
								<tr>
								    <td style="width:50%">Number of offices</td>
								    <td style="width:50%"></td>
								</tr>
								<tr>
								    <td>Number of office staff</td>
								    <td></td>
								</tr>
								<tr>
								    <td>Number of office staff</td>
								    <td></td>
								</tr>
								<tr>
								    <td>Number of Block/Municipility</td>
								    <td></td>
								</tr>
								<tr>
								    <td>Number of Gram Panchayet/Ward</td>
								    <td></td>
								</tr>
								<tr>
								    <td>Number of Police Station</td>
								    <td></td>
								</tr>
							</table>
							</div>
						</div>
					</div>
				</div></div>
			   <div class="row">
			   <div class="col-md-12">
					<div class="box-header with-border"><i class="ion ion-clipboard"></i> Officers</div>
					<div class="box-body">
						<div class="feedback-scroll">'.$offficer_data.'</div>
					</div>
				</div>
				</div>
				<div class="row">
			    <div class="col-md-12">
					<div class="box-header with-border"><i class="ion ion-clipboard"></i> Others</div>
					<div class="box-body">
						<div class="feedback-scroll">'.$othersoffficer_data.'</div>
					</div>
				</div>
				</div></div></div>';
				
	$output .= '<div class="modal fade" id="myModal1" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
					<div class="modal-dialog">
						<div class="modal-content">
							<div class="modal-header">
								<button type="button" class="close" data-dismiss="modal" onclick="remove_data()" aria-hidden="true">&times;</button>
								 <h4 class="modal-title">Add / Edit Contact Number</h4>
							</div>
							<div class="modal-body">
								<form method="POST" id="add_contact_number_form">
									<input type="hidden" id="sub_div_id" value="'.$sub_div_code.'">
									<input type="hidden" id="form_name_id" value="add_contact_number_form"> 
									<lable><b>Contact Number</b><font color="#FF0000">*</font></lable></br>
									<input type="input" name="contact_number" placeholder="Please enter Contact Number" id="contact_number_id" maxlength ="10" class="form-control" required="true"/></br>
									<span id="contact_span_id" style="display:none;color:red"> Contact Number field should numeric and not empty.</span>
								
								</form>
							</div>
							<div class="modal-footer">
								<button type="button" class="btn btn-default" data-dismiss="modal" onclick="remove_data()">Close</button>
								<button type="button" id="btnSubmit_contact" class="btn btn-primary">Save</button>
							</div>
						</div>
					</div>
				</div>';
	$output .= '<div class="modal fade" id="myModal2" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
					<div class="modal-dialog">
						<div class="modal-content">
							<div class="modal-header">
								<button type="button" class="close" data-dismiss="modal" onclick="remove_data()" aria-hidden="true">&times;</button>
								 <h4 class="modal-title">Add / Edit Email Id</h4>
							</div>
							<div class="modal-body">
								<form method="POST" id="add_email_form">
									<input type="hidden" id="sub_div_id" value="'.$sub_div_code.'">
									<input type="hidden" id="form_email_id" value="add_email_form"> 
									<lable><b>Email Id</b><font color="#FF0000">*</font></lable></br>
									<input type="input" name="email" placeholder="Please enter Email ID" id="email_id" class="form-control" required="true"/></br>
									<span id="email_span_id" style="display:none;color:red"> Email field should valid and not empty.</span>
								
								</form>
							</div>
							<div class="modal-footer">
								<button type="button" class="btn btn-default" data-dismiss="modal" onclick="remove_data()">Close</button>
								<button type="button" id="btnSubmit_email" data-dismiss="modal" class="btn btn-primary">Save</button>
							</div>
						</div>
					</div>
				</div>';						
	return $output; 
}

function rlo_office_contact_view($sub_div_code){
	$query1 = db_select('l_customuser_relation_address', 'cr');
	$query1->leftJoin('l_custom_user_detail', 'cud', 'cud.usr_id = cr.user_id');
	// $query1->leftJoin('users', 'u', 'u.uid = cr.user_id');
	$query1->fields('cud', array('srlno', 'usr_id', 'user_place,', 'fullname', 'mobile', 'degisnation_officer', 'officenumber', 'govt_mobile', 'govt_email'));
	$query1->fields('cr', array('id'));
	// $query1->fields('u', array('mail'));
	$query1->condition('cr.sub_div_code', $sub_div_code, '=');
	$query1->condition('cud.is_publish_in_contact_list', 1, '=');
	$query1->orderby('cud.usr_rid', 'asc');
	$query1->orderby('cud.dob', 'desc');
	$query1 = $query1->execute();
	
	if($query1->rowCount()>0){
		$header = array(
			   array('data'=> 'Sl. No.', 'width' => '10%'),			   
			   array('data'=> 'Name', 'width' => '20%'),
			   array('data'=> 'Designation', 'width' => '20%'),
			   array('data'=> 'Office Posting', 'width' => '15%'),
			   array('data'=> 'Email Address', 'width' => '20%'),
			   array('data'=> 'Mobile', 'width' => '15%')			   	  
		  	);
		$i = 1;
		$offficer_data = '';
		
		foreach($query1 as $data){
			
			$email1 = str_replace(".","[dot]",$data->govt_email);
			$email = str_replace("@","[at]",$email1);
			$rows[] = array(
						$i++,						
						'<strong>'.$data->fullname.'</strong>',						
						$data->degisnation_officer,
						$data->user_place,
						$email,
						$data->mobile
					);
					
		}
	}
	
	$offficer_data = theme('datatable',array('attributes' => array('class' => array('view-act-rules-table fixed_headers')), 'header' => $header, 'rows' => $rows, 'empty' => t("No data found !")));
	
	$output = '<div class="box box-primary"><div class="box-body">
			   <div class="row">
			   <div class="col-md-12">
					<div class="box-header with-border"><i class="ion ion-clipboard"></i> Officers</div>
					<div class="box-body">
						<div class="feedback-scroll">'.$offficer_data.'</div>
					</div>
				</div>
				</div></div></div>';	
							
	return $output; 
}
function heade_office_contact_details(){
	$output = '';
	$query = db_select('map_table', 'm');	
  	$query->fields('m', array('rlo_name', 'telephone_no', 'designation', 'mobile_no', 'email','address'));
	$query->condition('m.is_active','1','=');
	$query->orderBy('m.priority_col', 'ASC');	
  	$result = $query->execute();
	
	if($result->rowCount() > 0){
		foreach($result as $data){
			
			$email1 = str_replace("@", "[at]", $data->email);
			$email = str_replace(".", "[dot]", $email1);
			$dataarr .= '<div class="col-md-4"><div class="report-box min-height-300"><h4>'.strtoupper($data->rlo_name).'</h4>
								 <div class="totle-application">Address: '.$data->address.'<br>Contact No.:'.$data->telephone_no.'<br>Email:<br>'.$email.'</div>							         
							</div>
					  </div>';
		}
	}
	
	
	$output = '<div class="box box-primary"><div class="box-body">
			   <div class="row">
			   <div class="col-md-12">					
					<div class="box-body">
						<div class="row">'.$dataarr.'</div>
					</div>
			   </div></div></div>';
	
	return $output;
}
?>