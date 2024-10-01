<?php
function tracking_of_grievance($form, &$form_state){		
	$form['grievances_report']['ref_number'] = array(
						'#prefix' 		=> '<div class="col-md-3"><div class="search-form">',
						'#suffix' 		=> '',
						'#title'		=> 'Enter Reference number:',
						'#type' 		=> 'textfield',
						'#required'		=>	TRUE,
						'#size'			=>	25,
						'#attributes'	=> array('autocomplete' => 'off', 'class' => array('form-control')),
						// '#description'  => 'Sample Reference number: WBLC-INSP-123'
					);	
	
	
	/*$form['clra_backlog_master']['submit'] 	= array (
						'#prefix' 		=> '<tr><td>',
						'#suffix' 		=> '</td></tr></tbody></table></div>',  
						'#type' 		=> 'submit',
						'#value' 		=> 'SUBMIT ',
						'#attributes'	=> array('id' => 'submit_oneId' , 'class' => array('button skip'))
					);	*/									
													
	$form['grievances_report']['submit'] = array(	   
	    '#prefix' => '',
        '#suffix' => '</div></div>',		
		'#type' => 'button',
        '#default_value' => 'SEARCH',
        '#ajax' => array(
					'callback' => 'tracking_of_grievance_submit',
					'wrapper' => 'tracking_of_grievance',
					'method' => 'html',
					'effect' => 'fade'
    				)
		);
		
	$form['grievances_report']['submit_result'] = array(   
	   '#prefix' => '<div class="content-box-inner-top"><div id="tracking_of_grievance">',
       '#suffix' => '</div></div>',
	   );						
	return $form;
}

function tracking_of_grievance_submit($form,$form_state){
	$val =	$form_state['values'];
	$refer_id =	trim(substr($val['ref_number'],8));
	$match_pattern = '(WBLCGREF)';
	if(preg_match($match_pattern, $val['ref_number'])) {
	if(is_numeric($refer_id)){
	$header = 	array(
					array('data' => 'Sl.No.', 'width' => '7%'),
					array('data' => 'Date/Time', 'width' => '20%'),
					array('data' => 'Subject', 'width' => '25%'),
					array('data' => 'Comment'),
				);
	$fbrow_query = db_select('l_feedback', 'lfb');
	$fbrow_query->fields('lfb', array('id', 'subject', 'comment', 'submitted_date','status','status_details'));
		$db_or = db_or();
		$db_or->condition('lfb.master_id', $refer_id);
		$db_or->condition('lfb.id', $refer_id);
	$fbrow_query->condition($db_or);
	$fbrow_query->orderBy('lfb.id', 'desc');
	$fbrow_query = $fbrow_query->execute(); 
	$counter = 0;
	if($fbrow_query->rowCount()>0){
		$output = '<h1 class="inner-page-title">Details of Grievance Tracking [Reference No - '.$val['ref_number'].']</h1>';	
		while($data = $fbrow_query->fetchAssoc()){
			$counter++;
			if($data['status'] == 'I'){$status = 'Initiated';}
			if($data['status'] == 'P'){$status = 'Under Process';}
			if($data['status'] == 'C'){$status = 'Completed';}
			$enc_id = encryption_decryption_fun('encrypt', $data['id']);
			
			$rows[]  = 	array(
							array('data' =>	$counter, 'align' => 'left', 'class' => array($class)),
							//array('data' =>	l('WBLCGREF'.$data['id'], 'applicant-feedback/details/'.$enc_id), 'align' => 'left', 'class' => array($class)),
							date('d/m/Y h:m A', $data['submitted_date']),
							array('data' =>	l($data['subject'], 'applicant-feedback/details/'.$enc_id), 'align' => 'left', 'class' => array($class)),
							array('data' =>	l($data['comment'], 'applicant-feedback/details/'.$enc_id), 'align' => 'left', 'class' => array($class)),
							//$status,															
							);	
		}
	
	
	$variables 	= array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('view-act-rules-table')), 'empty' => t("No data found!"));
						
	$output .= theme('datatable', $variables);
	} else {
			drupal_set_message(t("Referance Number is not Exist.") ,'error');
	}
	} else {
		drupal_set_message(t("Please Enter a Valid Referance Number.") ,'error');
	}
	} else {
		drupal_set_message(t("Please Enter a Valid Referance Number.") ,'error');
	}
	return '<div class="col-md-9 content-box-inner">'.$output;	
	
}

function tracking_of_grievance_details($refer_no_enc = ''){
	drupal_add_css(drupal_get_path('module', 'pensionstatus') . '/css/inspectionview.css');
	$refer_no = encryption_decryption_fun('decrypt', $refer_no_enc);	
	
	$ins_list_query = db_select('l_inspection_establishment', 't');
	
	$ins_list_query->join('district_master', 'd', 'd.district_code = t.ind_est_dist');
	$ins_list_query->join('sub_division', 's', 's.sub_div_code = t.ind_est_subdivision and d.district_code = s.district_code ');
	$ins_list_query->join('block_mun_master', 'bm', 'bm.block_code = t.ind_est_block and bm.distict_code = d.district_code and s.sub_div_code = bm.sub_division_code');
	$ins_list_query->join('village_ward_master', 'v', 'v.village_code = t.est_vill_ward and v.block_code = bm.block_code');
	$ins_list_query->fields('d', array('district_name'));
	$ins_list_query->fields('s', array('sub_div_name')); 
	$ins_list_query->fields('bm', array('block_mun_name')); 
	$ins_list_query->fields('v', array('village_name'));
	$ins_list_query->join('contractor_works', 'cw', 'cw.contractor_work_id = t.nature_work ');
	$ins_list_query->join('police_station', 'p', 'p.police_station_code = t.police_station_one and p.dristrict_code = d.district_code');
    $ins_list_query->fields('cw', array('cont_work_name'));
	$ins_list_query->fields('p', array('name_of_police_station'));  
	// $ins_list_query->fields('t', array('name_of_ind_est','name_of_prin_con','ind_est_dist','ind_est_subdivision','ind_est_block','est_vill_ward','ins_user_id','ins_file_number','ins_submit_status','show_court_cause_status'));
	$ins_list_query->fields('t', array('ins_file_number','name_of_ind_est','addlineone','reg_linc_no','other_nature_name','nature_work','ind_est_pin_code','ins_dt','ins_tm','pre_ins_file_number','no_workmen_male','no_workmen_female','male_second','female_second','male_third','female_third','male_forth','female_forth'));
	$ins_list_query->condition('t.ins_file_number',trim($refer_no),'=');
	
	$ins_list_query_result = $ins_list_query->execute();
	$ins_list_query_result = $ins_list_query_result->fetchAssoc();
	 if($ins_list_query_result['nature_work']=='28'){  
		$nature_name=$ins_list_query_result['other_nature_name'] ;
	 }else{
		$nature_name=$ins_list_query_result['cont_work_name'] ;
	 }	
	
	$content = '<table width="100%"  class="view-act-rules-table fixed_headers">
					<thead>
					 <tr>
					 <th colspan="3">Establishment/Industry/Shop DETAILS</th>
					 </tr> 
					</thead>
					<tbody>
					</tr>
					<tr>
						<td width="30%">Reference Number</td>
						<td colspan="2">'.$ins_list_query_result['ins_file_number'].'</td>
					</tr>
     				
					
					<tr>
     					<td> Name of Establishment</td>
						<td colspan="2">'.$ins_list_query_result['name_of_ind_est'].'</td>
					</tr>
					<tr>
     					<td> Nature of industry/business</td>
						<td colspan="2">'.$nature_name.'</td>
					</tr>
					<tr>
     					<td>Address of the establishment</td>
						<td colspan="2">'.ucfirst($ins_list_query_result['village_name']).',<br>'.$ins_list_query_result['block_mun_name'].',<br>'.$ins_list_query_result['sub_div_name'].',<br>'.$ins_list_query_result['district_name'].',<br> Pin code: '.$ins_list_query_result['ind_est_pin_code'].',<br> Police Station: '.$ins_list_query_result['name_of_police_station'].'</td>
					</tr>';
					
	
	
	
	$content .= '<thead>
					 <tr>
					 <th colspan="3">During this inspection person present  Details</th>
					 </tr> 
					</thead>
					';
	
	
	$pre_person_query=db_query('select  * from l_insp_present_person where  ins_file_number = :filenumber  ', array(':filenumber' => trim($refer_no)));
      $i=0;
     foreach ($pre_person_query as $person_result) {
		 $i++;
		  $name_person=strtoupper($person_result->name_person);
		  $desg_person=strtoupper($person_result->desg_person);
		  $mobile_person=$person_result->mobile_person;
	
	    $content .= '<tr>
     					<td> ('.$i.') Name ,Designation and Mobile </td>
						<td colspan="2">'.ucfirst($name_person).',<br>'.$desg_person.',<br>'.$mobile_person.'</td>
					</tr>';
	
	  }
	  
	  $content .= '<thead>
					 <tr>
					 <th colspan="3">No. of workers employed  Details</th>
					 </tr> 
					</thead>';
	
	           $no_workmen_male = $ins_list_query_result['no_workmen_male']; 
			   $no_workmen_female = $ins_list_query_result['no_workmen_female'];
	
	
	    $content .= '<tr>
     					<td> No of workman of master role </td>
						<td colspan="2"> Male : '.ucfirst($no_workmen_male).',Female:'.$no_workmen_female.'</td>
					</tr>';
	           $male_second = $ins_list_query_result['male_second'];
			  $female_second = $ins_list_query_result['female_second'];
	  
	   $content .= '<tr>
     					<td> No of contractual labour </td>
						<td colspan="2"> Male : '.ucfirst($male_second).',Female:'.$female_second.'</td>
					</tr>';
					
			  $male_third = $ins_list_query_result['male_third'];
			  $female_third = $ins_list_query_result['female_third'];		
	  $content .= '<tr>
     					<td> No of other worker engaged </td>
						<td colspan="2"> Male : '.ucfirst($male_third).',Female:'.$female_third.'</td>
					</tr>';	
			   $male_forth = $ins_list_query_result['male_forth'];
			   $female_forth = $ins_list_query_result['female_forth'];
			  		
		$content .= '<tr>
     					<td> Child Labour </td>
						<td colspan="2"> Male : '.ucfirst($male_forth).',Female:'.$female_forth.'</td>
					</tr>';					
	  
	  
	



   $query_mt_worker= db_select('l_mt_worker_details', 'p');
	$query_mt_worker->leftjoin('district_master', 'd', 'd.district_code = p.work_dist');
	$query_mt_worker->leftjoin('sub_division', 's', 's.sub_div_code = p.worker_sub_div and d.district_code = s.district_code ');
	$query_mt_worker->leftjoin('block_mun_master', 'bm', 'bm.block_code = p.worker_block and bm.distict_code = d.district_code and s.sub_div_code = bm.sub_division_code');
	
	$query_mt_worker->leftjoin('village_ward_master', 'v', 'v.village_code = p.worker_vill_word and v.block_code = bm.block_code');
	$query_mt_worker->leftjoin('police_station', 'r', 'r.police_station_code = p.worker_police_st and r.dristrict_code = d.district_code');
	$query_mt_worker->fields('d', array('district_name','district_code'));
	$query_mt_worker->fields('s', array('sub_div_name')); 
	$query_mt_worker->fields('bm', array('block_mun_name')); 
	
	$query_mt_worker->fields('v', array('village_name','category')); 
	$query_mt_worker->fields('r', array('name_of_police_station'));
	$query_mt_worker->fields('p', array('name_mt_worker','address_line','designation','own_pin','owner_email','owner_mobile','type_of_emp','other_type_name','fhname'));
	$query_mt_worker->condition('p.ins_file_number',trim($refer_no),'=');
    $query_mt_worker_result = $query_mt_worker->execute(); 


	
	$content .= '<thead>
					 <tr>
					 <th colspan="3">Detail in respect of the Owners/Principle Employer/CONTRACTOR/Manager/MD/CEO/Others</th>
					 </tr> 
					</thead>';
	      
			  $i=0;
	foreach($query_mt_worker_result as $squan) {
		$address_line = $squan->address_line;
		$designation = $squan->designation;
		$name_mt_worker = $squan->name_mt_worker;
		$own_pin = $squan->own_pin;
		$type_of_emp = $squan->type_of_emp;
		$other_type_name = $squan->other_type_name;
		$name_of_police_station = $squan->name_of_police_station;
		$village_name = $squan->village_name;
		$block_mun_name = $squan->block_mun_name;
		$sub_div_name = $squan->sub_div_name;
		$district_name = $squan->district_name;
		$owner_mobile = $squan->owner_mobile;
		
		$content .= '<tr>
     					<td>Name of the Owners/Principle Employer/Contractor/Manager/MD/CEO/Others </td>
						<td colspan="2">'.$name_mt_worker.'<b>('.$type_of_emp.')</b></td>
					</tr>
					<tr>
     					<td>Mobile No : </td>
						<td colspan="2"><b>'.$owner_mobile.'</b></td>
					</tr>
					<tr>
     					<td>Address of Employee</td>
						<td colspan="2"> Village : '.$village_name.',<br> Block/Municipality: '.$block_mun_name.',<br> Sub_division: '.$sub_div_name.',<br> District: '.$district_name.',<br> Pin code: '.$own_pin.',<br> Police Station: '.$name_of_police_station.'</td>
					</tr>';
	}
			   
			   
	$content .= '<tr>
      				<td align="center" colspan="3" class="ins_heading"><strong><center>Violated Infringement</center></strong></td>
				</tr></tbody></table>';
					
	$order='cust_infring_id';
	$sort='ASC';
	
	$infr_prin_query = db_select('l_infringment', 't');
	$infr_prin_query->condition('t.ins_file_number',trim($refer_no),'=');
	$infr_prin_query->fields('t', array('infring_name','type_of_infring'));
	$infr_prin_query->orderBy($order, $sort);
	
	$infr_prin_query_rst =  $infr_prin_query->execute();
	
	foreach($infr_prin_query_rst as $dataprin ){
		$content .= '<table width="100%"  class="view-act-rules-table fixed_headers">
							<thead>
								 <tr>
									<th colspan="3">'.$dataprin->infring_name.'</th>
								  </tr> 
							</thead>
				 	<tr class="trhead">
				 		<td>SL.NO.</td>
						<td colspan="2">INFRINGEMENTS</td>
					</tr>';
		
		$infr_prin_query = db_select('l_pemp_infringment', 't');
		$infr_prin_query->fields('t', array('remarks_txt'));
 		$infr_prin_query->condition('t.ins_file_number',trim($refer_no),'=');
 		$infr_prin_query->condition('t.type_of_infring',$dataprin->type_of_infring,'=');
 
  		$infr_prin_query_rst =  $infr_prin_query->execute(); 
 
 		$numinfgM=$infr_prin_query_rst->rowCount();
  		$i=0;
    
		foreach ($infr_prin_query_rst as $infgp_result) { 
			$i++;
			if($i%2==0){ $bgcol = "#FCF6CF";}else{$bgcol = "#FEFEF2";}
			$content .= '<tr bgcolor="'. $bgcol .'">
							<td align="left" >'. $i.' </td>
							<td colspan="2" > '. $infgp_result->remarks_txt.'</td>
						</tr> ';
				
			}
		
			$prin_addi_infg=db_query('select  * from l_additional_infrigment where  ins_file_number = :filenumber  and type_of_infring=:tinf', array(':filenumber' => trim($refer_no),':tinf' => $dataprin->type_of_infring));
			$addnuminfgPrin=$prin_addi_infg->rowCount();	
			if($addnuminfgPrin > 0) {
				// $content .= '<tr ><td align="center" colspan="3"  style=" text-transform:uppercase;"><strong><center>ADDITIONAL INFRINGEMENTS</center></strong></td></tr>';
				
				foreach($prin_addi_infg as $delta => $addidataPrin ){ 
					$i++; 
					$content .= '<tr><td>'. $i.' </td><td colspan="2"  >'. $addidataPrin->additional_infg.'</td></tr>  ';
				}
			}
			
			$content .= '</table>';
		}
				
		$content .= '<table width="100%"  class="view-act-rules-table fixed_headers">';
		$content .= '<thead><tr><th colspan="4">'.$dataprin->infring_name.'</th></tr></thead><tr>';
	
		$query_document_fetch = db_query('select  *  from lc_signed_document_attachment a , file_managed b  where  a.ins_file_number=:insid and  a.fid=b.fid and a.typ_fl=:tp  ', array(':insid' => trim($refer_no),':tp' => 'O' ));
	 
	 	if($query_document_fetch->rowCount() > 0) {
		 	$content .= "<tr class='trhead'><td >SR.NO</td><td  >ACT</td> <td>ORIGINAL</td> <td>SIGNED</td></tr>";
		 
		 	$i=0;
	      
			foreach($query_document_fetch as $data){
				$i++;
				
				$act_query_name = db_select('l_inspection_master', 'lim');
				$act_query_name->fields('lim', array('inspection_name'));
				$act_query_name->condition('lim.inspection_txt_type', array($type_of_infring = $data->type_of_infring),'=');
	
				$act_query_res = $act_query_name->execute();
				$result = $act_query_res->fetchAssoc();
				$inspection_name = $result['inspection_name']; 
	
				$fid = $data->fid;	       
				$filename = $data->filename;
				
				
				$query_signed_document_fetch = db_query('select  *  from lc_signed_document_attachment a , file_managed b  where  a.ins_file_number=:insid and  a.fid=b.fid and a.typ_fl=:tp  ', array(':insid' => trim($refer_no),':tp' => 'S' ));
				$result2 = $query_signed_document_fetch->fetchAssoc();
				$filename2 = $result2['filename']; 
				if($query_signed_document_fetch->rowCount() > 0) {
				 $signed_doc_line=l('View', '/sites/default/files/inssigneddoc/'.$filename2, array('attributes' => array('target' => '_blank')));
				}
				else
				{
				$aa=$base_root.$base_path.'inspectionprint/'.encryption_decryption_fun('encrypt',$content7).'/'.encryption_decryption_fun('encrypt',$type_of_infring).'/inspectionprintpdf';
		        $signed_doc_line=l(t('GENERATE'), $aa,array('attributes' => array('target'=>'_blank'))) ;
				}
				
				$content .= '<tr><td>'.$i.'</td><td>'.$inspection_name.'</td><td>'.l('View', '/sites/default/files/insorigidoc/'.$filename, array('attributes' => array('target' => '_blank'))).'</td><td>'.$signed_doc_line.' </td></tr>';
			}
		 }
		 
	$content .= '</table>';
	
	
	
	
	 $ins_sc_query = db_select('l_infringment', 'a');
  $ins_sc_query->join('l_inspection_establishment', 'b', 'a.ins_file_number = b.ins_file_number');
  $ins_sc_query->condition('b.pre_ins_file_number',trim($refer_no),'=');
  $ins_sc_query->condition('a.sc_cc_status','F','=');
  $ins_sc_query->condition('a.per_no_full_sc_status','A','=');
  $ins_sc_query->condition('b.type_of_insnote_user', 'ins_show_cause','=');
  $ins_sc_query->fields('a', array('ins_file_number','type_of_infring','infring_name'));
  
  $ins_sc_query_result = $ins_sc_query->execute();  
  

  $ins_cc_query = db_select('l_infringment', 'a');
  $ins_cc_query->join('l_inspection_establishment', 'b', 'a.ins_file_number = b.ins_file_number');
  $ins_cc_query->condition('b.pre_ins_file_number',trim($refer_no),'=');
  $ins_cc_query->condition('a.sc_cc_status','F','=');
  $ins_cc_query->condition('a.per_no_full_sc_status','A','=');
  $ins_cc_query->condition('b.type_of_insnote_user', 'ins_court_case','=');
  $ins_cc_query->fields('a', array('ins_file_number','type_of_infring','infring_name'));
  $ins_cc_query_result = $ins_cc_query->execute(); 




  
    if($ins_sc_query_result->rowCount() > 0 ) {
		$content .= '<table width="100%"  class="view-act-rules-table fixed_headers">';
		 foreach ($ins_sc_query_result as $result_sc) { 
		
	          $content .= '
							<thead>
								 <tr>
									<th colspan="4">'.$result_sc->infring_name.' ( LETOFF )</th>
								  </tr> 
							</thead>
				 	<tr class="trhead">
				 		<td>SL.NO.</td>
						<td colspan="2">INFRINGEMENTS</td>
						<td> COMPLIANCE </td>
					</tr>';	
		
		    $infr_prin_query_sc = db_select('l_pemp_infringment', 't');
	        $infr_prin_query_sc->fields('t', array('remarks_txt','verify_compl','infring_id'));
	        $infr_prin_query_sc->condition('t.ins_file_number',trim($result_sc->ins_file_number),'=');
			$infr_prin_query_sc->condition('t.type_of_infring',$result_sc->type_of_infring,'=');
		    $infr_prin_query_sc_rst =  $infr_prin_query_sc->execute();
		   $i=0;
           foreach ($infr_prin_query_sc_rst as $data_sc) { 
            $i++;
			if($i%2==0){ $bgcol = "#FCF6CF";}else{$bgcol = "#FEFEF2";}
			$content .= '<tr bgcolor="'. $bgcol .'">
							<td align="left" >'. $i.' </td>
							
							<td colspan="2" > '. $data_sc->remarks_txt.'</td>';
							if(trim($data_sc->verify_compl)=='Y') {
							$content .= '<td  >YES</td>';
							}
							else
							{
							$content .= '<td>NO </td>';
							
							}
							
						$content .= '</tr> ';
           
		 
		   }
   
        $insact_addi_sc_infg=db_query('select  * from l_additional_infrigment where  ins_file_number = :filenumber  and type_of_infring=:tinf ', array(':filenumber' => trim($result_sc->ins_file_number),':tinf' => trim($result_sc->type_of_infring)));
		
		  if($insact_addi_sc_infg->rowCount() > 0 ){
		foreach($insact_addi_sc_infg as $addidataPrin ){ 
		 $i++;
		
		   $content .= '<tr bgcolor="'. $bgcol .'">
							<td align="left" >'. $i.' </td>
							
							<td colspan="2" > '. $addidataPrin->additional_infg.'</td>';
							if(trim($addidataPrin->sc_status)=='N') {
							$content .= '<td  >YES</td>';
							}
							else
							{
							$content .= '<td>NO </td>';
							
							}
							
						$content .= '</tr> ';
		
		
		
		  }
		  }
		  
		  
		 }
           
	}
	
	$content .= '</table>';
	
	if($ins_cc_query_result->rowCount() > 0 ) {
		$content .= '<table width="100%"  class="view-act-rules-table fixed_headers">';
		 foreach ($ins_cc_query_result as $result_cc) { 
		
	          $content .= '
							<thead>
								 <tr>
									<th colspan="4">'.$result_cc->infring_name.'( LETOFF )</th>
								  </tr> 
							</thead>
				 	<tr class="trhead">
				 		<td>SL.NO.</td>
						<td colspan="2">INFRINGEMENTS</td>
						<td> COMPLIANCE </td>
					</tr>';	
		
		    $infr_prin_query_cc = db_select('l_pemp_infringment', 't');
	        $infr_prin_query_cc->fields('t', array('remarks_txt','verify_compl','infring_id'));
	        $infr_prin_query_cc->condition('t.ins_file_number',trim($result_cc->ins_file_number),'=');
			$infr_prin_query_cc->condition('t.type_of_infring',$result_cc->type_of_infring,'=');
		    $infr_prin_query_cc_rst =  $infr_prin_query_cc->execute();
		   $i=0;
           foreach ($infr_prin_query_cc_rst as $data_cc) { 
            $i++;
			if($i%2==0){ $bgcol = "#FCF6CF";}else{$bgcol = "#FEFEF2";}
			$content .= '<tr bgcolor="'. $bgcol .'">
							<td align="left" >'. $i.' </td>
							
							<td colspan="2" > '. $data_cc->remarks_txt.'</td>';
							if(trim($data_cc->verify_compl)=='Y') {
							$content .= '<td  >YES</td>';
							}
							else
							{
							$content .= '<td>NO </td>';
							
							}
							
						$content .= '</tr> ';
           
		 
		   }
   
        $insact_addi_cc_infg=db_query('select  * from l_additional_infrigment where  ins_file_number = :filenumber  and type_of_infring=:tinf ', array(':filenumber' => trim($result_cc->ins_file_number),':tinf' => trim($result_cc->type_of_infring)));
		
		  if($insact_addi_cc_infg->rowCount() > 0 ){
		foreach($insact_addi_cc_infg as $addidataPrin ){ 
		 $i++;
		
		   $content .= '<tr bgcolor="'. $bgcol .'">
							<td align="left" >'. $i.' </td>
							
							<td colspan="2" > '. $addidataPrin->additional_infg.'</td>';
							if(trim($addidataPrin->sc_status)=='N') {
							$content .= '<td  >YES</td>';
							}
							else
							{
							$content .= '<td>NO </td>';
							
							}
							
						$content .= '</tr> ';
		
		
		
		  }
		  }
		  
		  
		 }
           
	}
	
	$content .= '</table>';
	
	
	return $content;
	
}
?>
