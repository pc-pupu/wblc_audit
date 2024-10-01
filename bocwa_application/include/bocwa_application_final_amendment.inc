 <?php

drupal_add_js(drupal_get_path('module', 'applicant_forms').'/js/custom.js');

function bocwa_application_preview_amendment($form, &$form_state, $id=''){
	global $base_root, $base_path, $user;
	
	$user_id					= $user->uid;
	$action						= 'decrypt';
	$action_encrypt				= 'encrypt';
	$application_id				= encryption_decryption_fun($action, $id);
	
	
	$master_part_bocwa_reg 		= db_select('l_bocwa_registration_master', 'lbrm');
	$master_part_bocwa_reg->fields('lbrm', array('final_submit_status', 'status', 'backlog_id'));
	$master_part_bocwa_reg->condition('lbrm.user_id', $user_id);
	$master_part_bocwa_reg->condition('lbrm.id', $application_id); 
	$master_part_bocwa_reg_result = $master_part_bocwa_reg->execute(); 
			 
	$applicant_details			=	$master_part_bocwa_reg_result->fetchAssoc();
	
	$final_submit_status		=	trim($applicant_details['final_submit_status']);
	$status						=	trim($applicant_details['status']);  
	$backlog_id					=	trim($applicant_details['backlog_id']);  
	
	
																									
	$form['amendment_preview']['application_id']	= array(
															'#title'			=> 'Application Id',
															'#type'				=> 'hidden',
															'#default_value' 	=> !empty($application_id) ? $application_id : '',
															'#attributes' 		=> array('readonly' => 'readonly'),
															);
																									
																									
	$form['amendment_preview']['backlog_id']		= array(
															'#title'			=> 'Backlog Id',
															'#type'				=> 'hidden',
															'#default_value' 	=> !empty($backlog_id) ? $backlog_id : '',
															'#attributes' 		=> array('readonly' => 'readonly'),
															);
																									
	
																									
	$form['amendment_preview']['markup_data_1'] 	= array(
															'#prefix' 			=> '<div class="content">',
															'#markup' 			=> view_bocwa_app_final_preview($application_id),
															);
																									
	if($status == '' || $status == 'B'){ 
						
		$form['amendment_preview']['finalAgree'] = array(																		 
														  '#type' 				=> 'checkbox',
														  '#description' 		=> '<span class="red-star">*</span>I hereby declare that the particulars given above are true to the best of my knowledge and belief.',
														  '#attributes'			=> array('id' => 'finalAgreeId','onchange' => 'return checkFinalAppCheckBox()'),
														  '#required' 			=> TRUE,
														  
														);
	
		$form['amendment_preview']['submit']		= array(
															'#type' 			=> 'submit',
															'#id'				=> 'submitBtn1',
															'#value' 			=> 'SUBMIT',
															'#attributes' 		=> array('class' => array('btn btn-primary pull-right')),
															'#prefix' 			=> '</div></div>',  
															'#suffix' 			=> '</div>', 
															);
																									
																									
	} else {
	
		$form['amendment_preview']['download_pdf'] = array(
															'#type'				=> 'markup',
															'#markup' 			=>  l(' VIEW&nbsp;&amp;&nbsp;PRINT','bocwa-generate-pdf/'.encryption_decryption_fun($action_encrypt, $application_id).'/'.encryption_decryption_fun($action_encrypt, $user_id), array('html' => TRUE, 'attributes' => array('target' => '_blank', 'class' => array('btn btn-primary pull-right')))),
															'#prefix' 			=> '</div></div>',  
															'#suffix' 			=> '</div>',
															);
		}
	
	
	return $form;						
	
}

function view_bocwa_app_final_preview($application_id=''){ 
	
		global $base_root, $base_path, $user;
		
		$user_id = $user->uid;
				
		$master_part_bocwa_reg = db_select('l_bocwa_registration_master', 'lbrm');
		$master_part_bocwa_reg->leftJoin('district_master', 'dm_loc', 'dm_loc.district_code=lbrm.loc_e_dist');
		$master_part_bocwa_reg->leftJoin('sub_division', 'sd_loc', 'sd_loc.sub_div_code=lbrm.loc_e_subdivision');
		$master_part_bocwa_reg->fields('lbrm', array());
		$master_part_bocwa_reg->fields('dm_loc', array('district_name'));
		$master_part_bocwa_reg->fields('sd_loc', array('sub_div_name','sub_div_code'));
		$master_part_bocwa_reg->condition('lbrm.user_id', $user_id);
		$master_part_bocwa_reg->condition('lbrm.id', $application_id);
		$master_part_bocwa_reg_result = $master_part_bocwa_reg->execute();  
		$content			=	$master_part_bocwa_reg_result->fetchAssoc(); 
		
		if(!empty($content) && $application_id!=''){
			
			/** Fetch Address from Miscellaneous Module ****/

			$table	 	 = 'l_bocwa_registration_master';
			$type	 	 = 'sub-table';
			$estArr	 	 = array('loc_e_dist','loc_e_subdivision','loc_e_areatype','loc_e_areatype_code','loc_e_vill_ward','loc_e_ps','loc_e_pin_number');
			$postalArr	 = array('e_postal_dist','e_postal_subdivision','e_postal_areatype','e_postal_areatype_code','e_postal_vill_ward','e_postal_ps','e_postal_pin_number');
			$empArr	 	 = array('emp_dist','emp_subdivision','emp_areatype','emp_areatype_code','emp_vill_ward','emp_ps','emp_pin_number');
			$managerArr	 = array('address_manager_dist','address_manager_subdivision','address_manager_areatype','address_manager_areatype_code','address_manager_vill_ward','address_manager_ps','address_manager_pin_number');
			$perAddArr	 = array('e_permanent_add_dist','e_permanent_add_subdivision','e_permanent_add_areatype','e_permanent_add_areatype_code','e_permanent_add_vill_ward','e_permanent_add_ps','e_permanent_add_pin_number');
			
			
			$establishment_address 	= $content['loc_e_name'].'<br/>'.get_full_address($table, $type, $application_id, $estArr);
			$postal_address 	 	= $content['e_postal_address'].'<br/>'.get_full_address($table, $type, $application_id, $postalArr);
			$emp_address	 		= $content['emp_address'].'<br/>'.get_full_address($table, $type, $application_id, $empArr);
			$manager_address	 	= $content['address_manager'].'<br/>'.get_full_address($table, $type, $application_id, $managerArr);
			$permanenet_address	 	= $content['e_permanent_address'].'<br/>'.get_full_address($table, $type, $application_id, $perAddArr);
				
		}
		if($content['emp_gender']=='M'){
				$gender = "Male";
		}elseif($content['emp_gender']=='F'){
			$gender = "Female";
		}elseif($content['emp_gender']=='O'){
			$gender = "Other";
		}
		
		if($content['workmen_if_same_similar_kind_of_work']=="0"){
				$similar_kind_of_work = "No";
		}else{
			$similar_kind_of_work = "Yes";
		}
		
		$fees = $content['max_no_of_building_workers_employed'];
		
		if($fees <= 100 ){
			$retFees	=	'500';
		}else if($fees > 100 && $fees <= 500  ){
			$retFees	=	'2000';
		}else if($fees > 500){
			$retFees	=	'10000';
		}
				
		$master_part_bocwa_docs = db_select('l_documents_upload', 'ldu');
		$master_part_bocwa_docs->fields('ldu', array());
		$master_part_bocwa_docs->condition('ldu.user_id', $user_id);
		$master_part_bocwa_docs->condition('ldu.act_id', '2');
		$master_part_bocwa_docs->condition('ldu.application_id', $application_id);
		$master_part_bocwa_docs_result = $master_part_bocwa_docs->execute();
		
		$content_docs	=	$master_part_bocwa_docs_result->fetchAssoc();
		
		$master_part_bocwa_encrypted_docs = db_select('l_encrypted_uploaded_documents', 'leud');
		$master_part_bocwa_encrypted_docs->fields('leud', array());
		$master_part_bocwa_encrypted_docs->condition('leud.user_id', $user_id);
		$master_part_bocwa_encrypted_docs->condition('leud.act_id', '2');
		$master_part_bocwa_encrypted_docs->condition('leud.application_id', $application_id);
		$master_part_bocwa_encrypted_docs_result = $master_part_bocwa_encrypted_docs->execute();
		
		$content_encrypted_docs	=	$master_part_bocwa_encrypted_docs_result->fetchAssoc();
		
	$enc_doc_array = array('TL'=> '','PD'=> '','AOA'=> '','MOA'=> '','MOC'=>'','FL'=> '','AC'=> '','PC'=> '','BB'	=> '','SC'	=> '','IC'	=> '','AP'=> '','ODSC'=> '','CR'=> '','WO'=> '','ORC'=> '','CH'=>'');
	
	$pdficonlink = '<img title="View Documents" alt="View Documents" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/pdf.png"> &nbsp;';
	
	$query 			= db_select('l_encrypted_uploaded_documents','leud')->fields('leud',array())->condition('application_id',$application_id)->condition('act_id','2')->condition('status','1')->condition('user_id',$user_id);
	$query_result 	= $query->execute();
	$document_type_arr=array();
	if($query_result->rowCount() > 0){
		foreach($query_result->fetchAll() as $data ){
			$enc_doc_array[$data->document_type_code]['content'] = 'view_documents/'.encryption_decryption_fun('encrypt',$data->id);
		}
	}
	
	
	
	$global_link = $base_root.$base_path.'sites/default/files/upload/';
	
	
		//echo '/'.$application_id;
		if($content['status'] == 'T'){
			
			$feespaid_fsubmit = '<p><font color="#ff0000"><strong>Note : Your transaction has been successfully made but process not complete. Click '.l('here', 'bocwa-upload-signed-application/'.encryption_decryption_fun('encrypt', $application_id)).' to download system generated [ FORM-I ] . You must upload signed [ FORM-I ] and upload it for further process.</strong></font></p>';
		}
			
			
		$output ='';
		$output = '<div class="panel panel-default panel-form">
					  <div class="panel-heading">Establishment Details</div>
					  	<div class="panel-body">
	  						<div class="help"><strong>Authorized Registering office for this application:</strong> Regional Labour Office, '.$content['sub_div_name'].', '.$content['district_name'].'&nbsp;'.l('More info.', 'rlo-details/'.str_replace(' ', '-', strtolower($content['sub_div_name'])).'/'.encryption_decryption_fun('encrypt', $content['sub_div_code']), array('html' => TRUE,'attributes' => array('target' => '_blank'))).'</strong></div><br/>'.$feespaid_fsubmit.'<br/>
						
						<div class="row">
							<div class="col-lg-6 col-md-6">
								<table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-striped table-responsive dashboard-table sub-table-dashboard">
									<thead>		  
										  <tr>
											<th width="51%">Parameters</th>
											<th>Inputs</th>
										  </tr>
									</thead>
									<tbody>
									  <tr>
										<td>Name of the Establishment</td>
										<td>'.$content['e_name'].'</td>
									   </tr>
									   <tr>
										<td>Establishment Type</td>
										<td>'.ucfirst($content['est_type']).'</td>
									   </tr>
									  <tr>
										<td>Location of the Establishment</td>
										<td>'.$establishment_address.'</td>
									  </tr>
									  <tr>
										<td>Postal Address of the Establishment</td>
										<td>'.$postal_address.'</td>
									  </tr>
									  <tr>
										<td>Full Name of the Establishment</td>
										<td>'.$content['e_full_name'].'</td>
									  </tr>
									  <tr>
										<td>Permanent Address of the Establishment</td>
										<td>'.$permanenet_address.'</td>
									   </tr>
									   <tr>
										<td>Nature of building or other construction work is to be carried on</td>
										<td>'.$content['nature_of_build_const'].'</td>
									   </tr>
									   <tr>
										<td>Maximum number of building workers to be employed on any day</td>
										<td>'.$content['max_no_of_building_workers_employed'].'</td>
									  </tr>
									</tbody>
								</table>
							</div>
							
							<div class="col-lg-6 col-md-6">
								<table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-striped table-responsive dashboard-table sub-table-dashboard">
									<thead>		  
									  <tr>
										<th width="51%">Parameters</th>
										<th>Inputs</th>
									  </tr>
									</thead>
									<tbody>
									  <tr>
										<td>Full Name of the Employer</td>
										<td>'.$content['emp_name'].'</td>
									   </tr>
									   <tr>
										<td>Gender</td>
										<td>'.$gender.'</td>
									   </tr>
									  <tr>
										<td>Address of the Employer</td>
										<td>'.$emp_address.'</td>
									  </tr>					  
									 
									  <tr>
										<td>Full name of the Manager or Person Responsible for the Supervision  and control of the Establishment</td>
										<td>'.$content['full_name_manager'].'</td>
									  </tr>
									  <tr>
										<td>Address of the Manager or Person Responsible for the Supervision  and control of the Establishment</td>
										<td>'.$manager_address.'</td>
									  </tr>
									  <tr>
										<td>Estimated date of commencement of building or other construction work</td>
										<td>'.date('dS M, Y',strtotime($content['est_date_of_commencement_building'])).'</td>
									   </tr>
									  <tr>
										<td>Estimated date of the completion of building or other construction work</td>
										<td>'.date('dS M, Y',strtotime($content['est_date_of_completion_building'])).'</td>
									  </tr>
					  
					  <tr>
						<td>Total Fees</td>
						<td>'.$retFees.'<input type="hidden" name="retFees" id="retFees" value="'.$retFees.'" />&nbsp;&nbsp;&nbsp;<a target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/fees_chat/BOCWA-FEES-STRUCTURE.pdf"><span class="viewfile_popup blink" id="fees_chart_pdf" title="CLICK HERE TO VIEW FEES DETAILS">FEES CHART [ In <i class="fa fa-inr" aria-hidden="true"></i> ]</a><div id="fees_details" title="FEES CHART" style="display:none;"><iframe src="'.$GLOBALS['base_url'].'/sites/default/files/upload/fees_chat/BOCWA-FEES-STRUCTURE.pdf" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div></td>    
					  </tr>
					  </tbody>
					  	</table>
						</div>
						</div>
						
						
					  <table class="table table-striped table-responsive dashboard-table sub-table-dashboard" width="100%" cellspacing="0" cellpadding="0" border="0">
						<thead>
						<tr>
						<td colspan="2" align="center">Documents Uploaded</td>
					  </tr></thead>';
					  
					 if(!empty($content_docs)){ 
			$output .='<tbody><tr>
							<td width="51%">Upload Trade License</td>';
						if($enc_doc_array['TL']['content'] !=''){
									
									
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['TL']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['trade_license_file'])){ 
							$output .=' <td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_trade_license/'.$content_docs['trade_license_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
						   } else { 
							$output .=' <td>No Document Uploaded</td>';
						  } 
			$output .='</tr>
			
						<tr>
							<td>Upload Articles of Association and Memorandum of Association/Partnership Deed</td>';
							if($enc_doc_array['AOA']['content'] !=''){
									
									
									$output .= '<td>'.l($pdficonlink,$enc_doc_array['AOA']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
								}elseif(!empty($content_docs['article_of_assoc_file'])){ 
					
						 		$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_article_of_assoc/'.$content_docs['article_of_assoc_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
					 		} else { 
								$output .='<td>No Document Uploaded</td>';
						 	} 
				$output .='</tr>
							<tr>
								<td>Upload Any other document in support of correctness of the particulars mentioned in the application if required</td>';
								if($enc_doc_array['MOC']['content'] !=''){
									
									
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['MOC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['memorandum_of_cert_file'])){ 
					
								$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_memorandum_of_cert/'.$content_docs['memorandum_of_cert_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
								 } else { 
								$output .='<td>No Document Uploaded</td>';
								 } 
				$output .='</tr>
    
						  <tr>
							<td>Upload Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc</td>';
         					
							  if($enc_doc_array['PD']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['PD']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['partnership_deed_file'])){ 
							 
        						$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_partnership_deed/'. $content_docs['partnership_deed_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
       						 } else { 
        						$output .='<td>No Document Uploaded</td>';
       						 } 
      			$output .='</tr>
				
							<tr>
								<td>Upload Challan</td>';
								
						if($enc_doc_array['CH']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['CH']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['challan_file'])){ 
								 
									$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_challan/'. $content_docs['challan_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
								 } else { 
									$output .='<td>No Document Uploaded</td>';
								 } 
					$output .='</tr>
							   <tr>
								  <td>Upload Work Order</td>';
								
						if($enc_doc_array['WO']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['WO']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['work_order_file'])){ 
								 
									$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_work_order/'. $content_docs['work_order_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
								 } else { 
									$output .='<td>No Document Uploaded</td>';
								 } 
					$output .='</tr>
					
							  <tr>
								  <td>Upload Form I for Assesment of CESS</td>';
								
						if($enc_doc_array['AC']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['AC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['form_one_asses_ses_file'])){ 
								 
									$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_form_one_asses_of_ses/'. $content_docs['form_one_asses_ses_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
								 } else { 
									$output .='<td>No Document Uploaded</td>';
								 } 
					$output .='</tr>
					
							   <tr>
								  <td>Upload Documents in support of Payment of CESS</td>';
								
						if($enc_doc_array['PC']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['PC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['supp_asses_ses_file'])){ 
								 
									$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_support_payment_ses/'. $content_docs['supp_asses_ses_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
								 } else { 
									$output .='<td>No Document Uploaded</td>';
								 } 
					$output .='</tr>
					
							  <tr>
								  <td>Upload Documents in support of correctness of the Application</td>';
								
						if($enc_doc_array['ODSC']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['ODSC']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['other_doc_file'])){ 
								 
									$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_other_docs/'. $content_docs['other_doc_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
								 } else { 
									$output .='<td>No Document Uploaded</td>';
								 } 
					$output .='</tr>
					
							  <tr>
								  <td>Upload Address Proof</td>';
								
						if($enc_doc_array['AP']['content'] !=''){
									
								$output .= '<td>'.l($pdficonlink,$enc_doc_array['AP']['content'],array('html' => TRUE, 'attributes' => array('target' => '_blank'))).'</td>';
						}elseif(!empty($content_docs['address_proof_file'])){ 
								 
									$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/bocwa_address_proof/'. $content_docs['address_proof_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
								 } else { 
									$output .='<td>No Document Uploaded</td>';
								 } 
					$output .='</tr>
					
							   <tr>
							   		<td>Form -I </td>';
									if(!empty($content_docs['form_1_bocwa_signed_pdf_file'])){
										
										$output .='<td><a  target="_blank" href="'.$GLOBALS['base_url'].'/sites/default/files/upload/BOCWA_Form-I/'. $content_docs['form_1_bocwa_signed_pdf_file'].'"><img src="'.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png"></a></td>';
								 	} else { 
										$output .='<td>Form-I should be uploaded by the Applicant after successful payment of fees</td>';
								 	} 
					$output .='</tr>';
				
    				 }
					 
					  
	$output .='</table>';
			
  
					  
	return $output;
	
}

function bocwa_application_preview_amendment_submit($form, &$form_state){
	
		global $base_root, $base_path, $user; 
		
		$user_id				= $user->uid;
		$backlog_field_ids 		= '';
		
		$val					= $form_state['values'];  
		$application_id			= $val['application_id'];
		$backlog_id				= $val['backlog_id'];
		$retFees				= $_POST['retFees'];
		
		
		
		if(!empty($backlog_id)){
			
			 /*$backlog_field_ids	= 'e_name,e_postal_address,full_name_principal_emp,e_any_day_max_num_of_workmen,address_principal_emp';
			if(!empty($e_name)){
				$readonly_e_name 						= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
			}
			if(!empty($e_postal_address)){
				$readonly_e_postal_address				= array('readonly' => 'readonly', 'class' => array('readonly_view'));	
			}
			if(!empty($full_name_principal_emp)){
				$readonly_full_name_principal_emp 		= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
			}
			if(!empty($e_any_day_max_num_of_workmen)){
				$readonly_e_any_day_max_num_of_workmen 	= array('disabled' => 'disabled', 'class' => array('readonly_view'));	
			}
			if(!empty($address_principal_emp)){
				$readonly_address_principal_emp 		= array('readonly' => 'readonly', 'class' => array('readonly_view'));	
			}
			
			if(!empty($loc_e_subdivision)){
				$readonly_e_postal_subdivision 			= array('readonly' => 'readonly', 'class' => array('readonly_view'));	
			}
			if(!empty($name_areatype)){
				$readonly_e_postal_block 				= array('readonly' => 'readonly', 'class' => array('readonly_view'));	
			}*/
			
		}
		
		
		$fetch_reg_det 			= db_select('l_bocwa_registration_master', 'lbrm');
		$fetch_reg_det->fields('lbrm', array('application_date', 'created','emp_name'));
		$fetch_reg_det->condition('lbrm.user_id', $user_id);
		$fetch_reg_det->condition('lbrm.id', $application_id); 
		$fetch_reg_det_result 	= $fetch_reg_det->execute(); 
		
			
		if( $fetch_reg_det_result->rowCount() > 0 ){
			
				 	$obj2 									= $fetch_reg_det_result->fetchAssoc(); 
					$application_date						= !empty($obj2['application_date']) ? $obj2['application_date'] :'';
					$created_date							= !empty($obj2['created'])? $obj2['application_date'] :'';
					$emp_name								= $obj2['emp_name'];
				
		}
		
		$fetch_users_role		= db_select('users_roles', 'ur');
		$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
		$fetch_users_role->fields('ro', array('rid'));
		$fetch_users_role->condition('ur.uid', $user_id);
		$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc();
	
		$user_role				=	$fetch_users_role_data['rid']; 
		
		$fieldsRemarksData 		= 	array( 
									'remark_text'			=>	'Applied Successfully',
									'remark_by'				=>  $user_id,
									'remark_to'				=>	$user_id,
									'application_id'        =>  $application_id,
									'remark_type' 			=>  '0',
									'remark_field_title'	=>  $backlog_field_ids,
									'remark_date' 			=>  time(),
									'remark_by_name'		=> 	$emp_name,
									'remark_by_role'		=> 	$user_role
								);
				
	db_insert('l_bocwa_remark_master')->fields($fieldsRemarksData)->execute();
	
	
	if(empty($application_date) && empty($created_date)){
	
		$sql_query = db_update('l_bocwa_registration_master');
		$sql_query->fields(array(
				 'status'  				=> '0',
				 'application_date' 	=> date("Y-m-d"),
				 'final_submit_status' 	=> 'P',
				 'finalfees' 			=> $retFees,
				 'created'				=> time(),
				 'user_from_portal'		=> 1
		));
		$sql_query->condition('id',$application_id);
		$sql_query->condition('user_id',$user_id);
		$sql_query->execute();
		
		
	}else{
		
		$sql_query = db_update('l_bocwa_registration_master');
		$sql_query->fields(array(
				 'status'  				=> '0',
				 'application_date' 	=> date("Y-m-d"),
				 'final_submit_status' 	=> 'P',
				 
		));
		$sql_query->condition('id',$application_id);
		$sql_query->condition('user_id',$user_id);
		$sql_query->execute();
	}
		
	//////////////////  BOCWA API SERVICE FOR LD  /////////////////////////////////	
	
	$api_q2 					= 	db_select('l_common_application_master', 'lcam');
	$api_q2						->	fields('lcam', array('ld_uid'));
	$api_q2						->	condition('lcam.user_id', $user_id);
	$api_q2_result 				= 	$api_q2->execute(); 
	$api_ap2_details			=	$api_q2_result->fetchAssoc();
	
	$ld_id						=	trim($api_ap2_details['ld_uid']);
	
	if(!empty($ld_id) || $ld_id != ""){
		if($_SESSION['service_id'] == 2){
			drupal_goto('https://wblabour.gov.in/dashboard1');
		}
	}
	//////////////////  END BOCWA API SERVICE FOR LD  /////////////////////////////////			
		
	$message = 'Application is applied successfully. Inspection will be started shortly.';
	drupal_set_message(t($message));
	drupal_goto( 'bocwa-application-view/'.encryption_decryption_fun('encrypt', $application_id) );
	
}
