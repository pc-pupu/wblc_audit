<?php
function alcinspection_list($form, &$form_state){
	 global $base_root, $base_path, $user;
	 $uid=$user->uid; 
	
	$sub_div_code_rs=db_query('select  sub_div_code from l_customuser_relation_address where  user_id=:user_id ', array(':user_id' => trim($uid)) );
	   
	         $result_sudivcode=$sub_div_code_rs->fetchAssoc();
	         $sub_div_code= trim($result_sudivcode['sub_div_code']);
	
	
	
	// drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');     
	   
	 $form['inspectionlistfields'] = array(
          '#title' => t(''),
          '#type' => 'fieldset',
          '#collapsed' => FALSE
          );   
 $img=  '<img  src="'.$base_root.$base_path.'/sites/all/themes/jackson/images/bullet-green.png" align="left" >';	   
	   $rows = array();
    $header = array();
  
  
   $header = array(
      
            array('data'=> 'Sl. No.', 'width' => '7%'),
			array('data'=> 'File No.', 'width' => '10%'),
			array('data'=> 'Date of Inspection', 'field' => 'ins_submit_dt', 'width' => '10%'),
		    array('data'=> 'Name of the Establishment/ Industry/ Shop', 'width' => ''),
            array('data' => 'Owner Information', 'width' => ''),
			array('data' => 'INSPECTION NOTE',  'width' => '7%'),
			array('data' => 'SHOW-CAUSE/ LETOFF APPROVAL ', 'width' => '11%'),
		    array('data' => 'COURT-CASE/ LETOFF APPROVAL',  'width' => '11%'),
		   
		   
	);
	 
	    // $sort = 'DESC';
	  //   $order = 'ins_submit_dt';
		 
		
         $ins_list_query = db_select('l_inspection_establishment', 't');
	     $ins_list_query->join('district_master', 'd', 'd.district_code = t.ind_est_dist');
         $ins_list_query->join('sub_division', 's', 's.sub_div_code = t.ind_est_subdivision and d.district_code = s.district_code ');
		 $ins_list_query->join('block_mun_master', 'bm', 'bm.block_code = t.ind_est_block and bm.distict_code = d.district_code and s.sub_div_code = bm.sub_division_code');
		 $ins_list_query->join('village_ward_master', 'v', 'v.village_code = t.est_vill_ward and v.block_code = bm.block_code');
		 //$ins_list_query->join('l_custom_user_detail', 'l', 'l.usr_id =  t.ins_user_id');
		 
         $ins_list_query->fields('d', array('district_name'));
         $ins_list_query->fields('s', array('sub_div_name')); 
		 $ins_list_query->fields('bm', array('block_mun_name')); 
		 $ins_list_query->fields('v', array('village_name'));
         $ins_list_query->fields('t', array('name_of_ind_est','name_of_prin_con','ind_est_dist','ind_est_subdivision','ind_est_block','est_vill_ward','ins_user_id','ins_file_number','ins_submit_status','show_court_cause_status','ins_dt','ins_submit_dt'));
         //$ins_list_query->condition('l.created_by',trim($uid),'=');
		 $ins_list_query->condition('t.type_of_insnote_user','inspection_note','=');
		 $ins_list_query->condition('t.ins_submit_status','S','=');
		 $ins_list_query->condition('s.sub_div_code',trim($sub_div_code),'=');
		 $ins_list_query->orderBy('t.ins_dt', 'desc');
	     $ins_list_query_result = $ins_list_query->execute();
		  
		  
		    
		  
		  
	
	
		
	     $i=0;
		foreach($ins_list_query_result as $data) {
			$i++;
			
			
			
			global $base_root, $base_path;
			
			
			$owner_name=db_query('select  name_mt_worker from l_mt_worker_details where  ins_file_number=:filenumber limit 1', array(':filenumber' => trim($data->ins_file_number)) );
	   
	         $result_own=$owner_name->fetchAssoc();
	         $name_mt_worker= trim($result_own['name_mt_worker']);
			
			
			
			
			
		     $aa = $base_root.$base_path.'alcinspection/inspectionnoteview/'.encryption_decryption_fun('encrypt',$data->ins_file_number);
		     $link_s=l(t('<span class="badge bg-blue"><i class="fa fa-eye"></i> View Details</span>'), $aa, array('html' => true));
			 
			 
			 $court_case_link=''; 
			 
		   	$show_case_link=''; 
			$let_off_approved='';
			$court_case_let_off=''; 
			$court_case_back_link='';
			$court_case_app_link='';
			$court_fwd_rec_link='';
			
			
			
	
	
	//$query_insnote_alc_thired=db_query('select  *  from l_inspection_establishment  where  pre_ins_file_number = :filenum and ins_submit_status= :sta and verified_showcause_status=:vs and type_of_insnote_user=:typ', array(':filenum' => trim($data->ins_file_number),':sta' => 'S',':vs' => 'S',':typ' => 'ins_show_cause'));
	
	
	
	
		
	
	
	$query_insfileno_fetch=db_query('select ins_file_number from l_inspection_establishment where pre_ins_file_number = :filenum and ins_submit_status= :sta and type_of_insnote_user= :tpins and verified_showcause_status=:vs', array(':filenum' => trim($data->ins_file_number),':sta' => 'S',':tpins' => 'ins_show_cause',':vs' => 'S'));

$result_fileno=$query_insfileno_fetch->fetchAssoc();
$ins_file_number = $result_fileno['ins_file_number']; 
	
	
	
	
	
	
    	
	
	
	 
	
	    $query_cc = db_select('l_infringment', 'a');
        $query_cc->join('l_inspection_establishment', 'b', 'a.ins_file_number = b.ins_file_number');
        $query_cc->condition('b.pre_ins_file_number',trim($data->ins_file_number),'=');
        $query_cc->condition('b.type_of_insnote_user', 'ins_court_case','=');
		$query_cc->condition('b.ins_submit_status', 'S','=');
		$query_cc->condition('b.verified_showcause_status', 'S','=');
		$query_cc->condition('a.per_no_full_sc_status',array('S','N','V','A','B'),'IN');
		
        $query_cc->fields('a', array('ins_file_number','per_no_full_sc_status','sc_cc_status','alc_cc_let_st'));
        $query_cc_result = $query_cc->execute();
	   if($query_cc_result->rowCount() > 0) 
	 			
	    {
			
		 foreach($query_cc_result as $result_cc)
			   {	
		
		$court_case_full_sc_status=$result_cc->per_no_full_sc_status; 
		$court_case_sc_cc_status=$result_cc->sc_cc_status;
		$ins_file_number_cc=$result_cc->ins_file_number; 
		$court_case_alc_cc_let_st=$result_cc->alc_cc_let_st; 
		//print $court_case_alc_cc_let_st;
		//exit;
	
	
	      if($court_case_full_sc_status=='V' && $court_case_sc_cc_status=='F'){
		  $msg='FORWARD BY INSPECTOR APPROVAL FOR LET OFF';
		  $cl='228B22';
		  $cc=$base_root.$base_path.'alcinspection/bfccletoffpermision/'.encryption_decryption_fun('encrypt',$ins_file_number_cc);
		  $court_case_let_off=l(t($img.'<font color="'.$cl.'"><b>'.$msg.'</b></font></br>'), $cc,array('html'=>TRUE)) ;
		 }
		if($court_case_full_sc_status=='B' && $court_case_sc_cc_status=='F'){
		  $msg='BACK TO INSPECTOR FOR LET OFF';
		  $cl='FF1493';
		  $cc=$base_root.$base_path.'alcinspection/bfccletoffpermision/'.encryption_decryption_fun('encrypt',$ins_file_number_cc);
		  $court_case_let_off=l(t($img.'<font color="'.$cl.'"><b>'.$msg.'</b></font></br>'), $cc,array('html'=>TRUE)) ;
		 }
		elseif(($court_case_full_sc_status=='N' && $court_case_sc_cc_status=='Y' && $court_case_alc_cc_let_st=='') || ($court_case_full_sc_status=='S' && $court_case_sc_cc_status=='Y' && $court_case_alc_cc_let_st=='') ){
		  $msg='FORWARD BY <br> INSPECTOR APPROVAL <br> FOR COURTCASE';
		  $cl='2f26b7';
		  $cc=$base_root.$base_path.'alcinspection/courtcasepermision/'.encryption_decryption_fun('encrypt',$ins_file_number_cc);
		  $court_case_link=l(t($img.'<font color="'.$cl.'">'.$msg.'</font></br>'), $cc,array('html'=>TRUE)) ;
		 }
		elseif(($court_case_full_sc_status=='N' && $court_case_sc_cc_status=='Y' && $court_case_alc_cc_let_st=='B') || ($court_case_full_sc_status=='S' && $court_case_sc_cc_status=='Y' && $court_case_alc_cc_let_st=='B') ){
		  $msg='BACK TO INSPECTOR  <br> FOR COURTCASE';
		  $cl='ff7d33';
		  $cc=$base_root.$base_path.'alcinspection/courtcasepermision/'.encryption_decryption_fun('encrypt',$ins_file_number_cc);
		  $court_case_back_link=l(t($img.'<font color="'.$cl.'">'.$msg.'</font></br>'), $cc,array('html'=>TRUE)) ;
		 } 
		 elseif(($court_case_full_sc_status=='N' && $court_case_sc_cc_status=='Y' && $court_case_alc_cc_let_st=='A') || ($court_case_full_sc_status=='S' && $court_case_sc_cc_status=='Y' && $court_case_alc_cc_let_st=='A') ){
		  $msg=' COURTCASE <br> APPROVED';
		  $cl='FF1493';
		  $cc=$base_root.$base_path.'alcinspection/courtcasepermision/'.encryption_decryption_fun('encrypt',$ins_file_number_cc);
		  $court_case_app_link=l(t($img.'<font color="'.$cl.'">'.$msg.'</font></br>'), $cc,array('html'=>TRUE)) ;
		 }	
		 elseif(($court_case_full_sc_status=='N' && $court_case_sc_cc_status=='Y' && $court_case_alc_cc_let_st=='R') || ($court_case_full_sc_status=='S' && $court_case_sc_cc_status=='Y' && $court_case_alc_cc_let_st=='R') ){
		  $msg='RECOMMENDATION FOR  <br> SEEKING SANCTION';
		  $cl='6f4a34';
		  $cc=$base_root.$base_path.'alcinspection/courtcasepermision/'.encryption_decryption_fun('encrypt',$ins_file_number_cc);
		  $court_fwd_rec_link=l(t($img.'<font color="'.$cl.'">'.$msg.'</font></br>'), $cc,array('html'=>TRUE)) ;
		 }  
	    elseif($court_case_full_sc_status=='A' && $court_case_sc_cc_status=='F'){
		  $msg='LETOFF APPROVED';
		  $cl='228B22';
		  $cc=$base_root.$base_path.'alcinspection/bfccletoffpermision/'.encryption_decryption_fun('encrypt',$ins_file_number_cc);
		  $court_case_let_off=l(t($img.'<font color="'.$cl.'">'.$msg.'</font></br>'), $cc,array('html'=>TRUE)) ;
		 }
	 }
	}
	 	
	//if($query_insnote_alc_thired->rowCount() > 0) 
	 			
	//{
		
		$det_act_sc_ts_two_query = db_select('l_infringment', 'a');
        $det_act_sc_ts_two_query->join('l_inspection_establishment', 'b', 'a.ins_file_number = b.ins_file_number');
        $det_act_sc_ts_two_query->condition('b.pre_ins_file_number',trim($data->ins_file_number),'=');
        $det_act_sc_ts_two_query->condition('b.type_of_insnote_user', 'ins_show_cause','=');
		$det_act_sc_ts_two_query->condition('b.ins_submit_status', 'S','=');
		$det_act_sc_ts_two_query->condition('b.verified_showcause_status', 'S','=');
		$det_act_sc_ts_two_query->condition('a.per_no_full_sc_status', array('S','V','N','A','B'),'IN');
		
        $det_act_sc_ts_two_query->fields('a', array('ins_file_number','per_no_full_sc_status','sc_cc_status'));
        $det_act_sc_ts_two_result = $det_act_sc_ts_two_query->execute(); 
		
		//$result_per_no_full_full_status=$det_act_sc_ts_two_result->fetchAssoc();
	//	$per_no_full_full_status=$result_per_no_full_full_status['per_no_full_sc_status'];
		
		
		if($det_act_sc_ts_two_result->rowCount() > 0) 
	 			
	    {
			//print "aaaaaaaaaaaaaaaaa".$det_act_sc_ts_two_result->rowCount();
		foreach($det_act_sc_ts_two_result as $result_sc)
			   {	
		
		$show_cause_full_sc_status=$result_sc->per_no_full_sc_status; 
		$show_cause_sc_cc_status=$result_sc->sc_cc_status;
		$show_cause_ins_file_number=$result_sc->ins_file_number;    
		//print $show_cause_ins_file_number.'/'.$show_cause_sc_cc_status.'/'.$show_cause_full_sc_status;
		
		if(($show_cause_full_sc_status=='S'  && $show_cause_sc_cc_status=='Y') || ($show_cause_full_sc_status=='N'  && $show_cause_sc_cc_status=='Y') ){
		  $msg='SHOW-CAUSE';
		  $cl='228B22';
		  $cc=$base_root.$base_path.'alcinspection/showcausenoticeview/'.encryption_decryption_fun('encrypt',$show_cause_ins_file_number);
		 $show_case_link=l(t($img.'<font color="'.$cl.'">'.$msg.'</font></br>'), $cc,array('html'=>TRUE)) ;
		 }
		
		   
		   
	   elseif($show_cause_full_sc_status=='V' && $show_cause_sc_cc_status=='F' ){
		  
		
		 $msg='FULL COMPLAINS AND FORWARD <br> BY INSPECTOR APPROVAL FOR LET OFF';
		  $cl='dd33ff';
		  $cc=$base_root.$base_path.'alcinspection/inspectionnoteview/'.encryption_decryption_fun('encrypt',$show_cause_ins_file_number);
		 $let_off_approved=l(t($img.'<font color="'.$cl.'">'.$msg.'</font></br>'), $cc,array('html'=>TRUE)) ;
	  
		
	      }	 
	    
		
		 elseif($show_cause_full_sc_status=='A'  && $show_cause_sc_cc_status=='F'){
		  $msg='LET OFF APPROVED';
		  $cl='228B22';
		  $cc=$base_root.$base_path.'alcinspection/inspectionnoteview/'.encryption_decryption_fun('encrypt',$show_cause_ins_file_number);
		 $let_off_approved=l(t($img.'<font color="'.$cl.'">'.$msg.'</font></br>'), $cc,array('html'=>TRUE)) ;
		 }
		elseif($show_cause_full_sc_status=='B'  && $show_cause_sc_cc_status=='F'){
		  $msg='LET OFF BACK TO INSPECTOR';
		   $cl='6e6d6e';
		   $cc=$base_root.$base_path.'alcinspection/inspectionnoteview/'.encryption_decryption_fun('encrypt',$show_cause_ins_file_number);
		 $let_off_approved=l(t($img.'<font color="'.$cl.'">'.$msg.'</font></br>'), $cc,array('html'=>TRUE)) ;
		}
		
		 	
		
	   }
			 
		}
			
	        // $court_case_let_off='';
			// $court_case_link='';     
				 
			   $rows[] = array(
      						$i++,
							$data->ins_file_number,
							date('d-m-Y', strtotime($data->ins_dt)),
							$data->name_of_ind_est,
							$name_mt_worker,
							$link_s,
							$show_case_link.$let_off_approved,
							$court_case_let_off.$court_case_app_link.$court_case_link.$court_case_back_link.$court_fwd_rec_link,
							//$ins_status,
							//$link_s,
							
							
			    
	  		   );   
}

 	$variables = array('attributes' => array('class' => array('table table-responsive admin-custom-table')), 'header' => $header, 'rows' => $rows);
	
	$output = theme('datatable', $variables);
	
	$form['inspectionlistfields']['addshowprin'] = array(		   
		    						'#prefix' => '<div class="box box-primary box-solid"><div class="box-header"></div><div class="box-body"><div class="feedback-scroll">',
									'#suffix' => '</div></div></div>',
									'#markup' => $output,
												);
 
  	return $form; 
}

function alc_insnote_rmks(){
	  
	  
	global $base_root, $base_path , $full_base;  
	  
	  
	
	   $ins_file_no=trim($_POST['ins_file_no']);
	   $alc_ins_note_rm=trim($_POST['alc_ins_note_rm']);
	   $txn = db_transaction();

      try { 
	  
	    $query = db_update('l_inspection_establishment');
         $query->fields(array(
        
		 'alc_insnote_rmks'=>$alc_ins_note_rm,
		 
		 'alc_insnote_rmks_dt'=>date("Y-m-d"),
		
		
		 
         ));
		 
		   $query->condition('ins_file_number',trim($ins_file_no),'=');
	       $query->condition('ins_submit_status','S','=');
		   //$query->condition('verified_showcause_status','S','=');
           $query->condition('type_of_insnote_user','inspection_note','=');
	       $query->execute();   
	  
	  $message = '  remarks successfully  send . ';
	  
	   }
		catch (Exception $e) {
   
         $txn->rollback();
   
      watchdog_exception('type', $e);
	  $message = '  remarks successfully not send . ';
      }
  
		
         $full_base = $base_root . $base_path.'alcinspection';
		  
       drupal_set_message(t($message));
	   drupal_goto($full_base); 
	   
}
function alc_letoff_approved(){
	  
	  
	global $base_root, $base_path , $full_base, $user ;
	 
	 
            $uid=$user->uid; 
	
	         $query_inspector_nm=db_query(' select   fullname   from l_custom_user_detail where  usr_id = :uid', array(':uid' => trim($uid) ));	
		     $result_insnm=$query_inspector_nm->fetchAssoc();
	         $ins_fullname = $result_insnm['fullname']; 
	  
	  
	
	   $ins_file_no=trim($_POST['ins_file_no']);
	   $alc_bfsc_note=trim($_POST['alc_bfsc_note']);
	   $ins_row=trim($_POST['ins_row']);
	   $alc_ins_status=trim($_POST['alc_ins_status']);
	   
	   
	//print $ins_row;
	//exit;
	 
	$txn = db_transaction();

      try { 
	  
	 if($alc_ins_status=='B'){
	   $query = db_update('l_inspection_establishment');
         $query->fields(array(
        
		 'alc_back_note'=>$alc_bfsc_note,
		 
		 'alc_back_note_dt'=>date("Y-m-d"),
		
		
		 
         ));
		 
		   $query->condition('ins_file_number',trim($ins_file_no),'=');
	       $query->condition('ins_submit_status','S','=');
		   $query->condition('verified_showcause_status','S','=');
           $query->condition('type_of_insnote_user','ins_show_cause','=');
	       $query->execute(); 
	   
	  } 
	  
	  elseif($alc_ins_status=='A'){
	   $query = db_update('l_inspection_establishment');
         $query->fields(array(
        
		 'alc_bfsc_note'=>$alc_bfsc_note,
		 
		 'alc_bfsc_lf_approved_dt'=>date("Y-m-d"),
		
		
		 
         ));
		 
		   $query->condition('ins_file_number',trim($ins_file_no),'=');
	       $query->condition('ins_submit_status','S','=');
		   $query->condition('verified_showcause_status','S','=');
           $query->condition('type_of_insnote_user','ins_show_cause','=');
	       $query->execute(); 
	   
	  }
	     for($j=1; $j<=$ins_row ; $j++){
		   $type_of_ins_act=trim($_POST['type_of_ins_act'.$j]); 
		   
		    $queryM = db_update('l_infringment');
		   $queryM->fields(array(
          'per_no_full_sc_status' =>$alc_ins_status,
		  'ins_user_id'=>$uid,
		  'ins_alc_name'=>$ins_fullname
		  
		  
		   ));
          $queryM->condition('ins_file_number',$ins_file_no,'=');
		  $queryM->condition('type_of_infring',$type_of_ins_act,'=');
		  $queryM->condition('per_no_full_sc_status',array('V','B'),'IN');
		  $queryM->condition('sc_cc_status','F','=');
	   
         $queryM->execute(); 
		   
		   
		   
		   
	   }
	   
	   
	    if($alc_ins_status=='A'){
		 $ss='letoff approved';	
		}
		 elseif($alc_ins_status=='B'){
	     $ss='back to inspector';
		 }
	   
	    $message = ' successfully . '.$ss;
	   
	    }
		catch (Exception $e) {
   
         $txn->rollback();
   
      watchdog_exception('type', $e);
	  $message = '  letoff successfully no approved . ';
      }
  
		
         $full_base = $base_root . $base_path.'alcinspection';
		  
       drupal_set_message(t($message));
	   drupal_goto($full_base); 
	 
	 

}






function alc_inspection_note_view( $filenumber = NULL){
	global $base_root, $base_path;
	$id = encryption_decryption_fun('decrypt', trim($filenumber));	
		
	$show_details_query = db_select('l_inspection_establishment', 't');
	$show_details_query->join('district_master', 'd', 'd.district_code = t.ind_est_dist');
	$show_details_query->join('sub_division', 's', 's.sub_div_code = t.ind_est_subdivision and d.district_code = s.district_code ');
	$show_details_query->join('block_mun_master', 'bm', 'bm.block_code = t.ind_est_block and bm.distict_code = d.district_code and s.sub_div_code = bm.sub_division_code');
	$show_details_query->join('village_ward_master', 'v', 'v.village_code = t.est_vill_ward and v.block_code = bm.block_code');
	$show_details_query->join('police_station', 'p', 'p.police_station_code = t.police_station_one and p.dristrict_code = d.district_code');
	$show_details_query->join('contractor_works', 'cw', 'cw.contractor_work_id = t.nature_work ');
	$show_details_query->fields('d', array('district_name'));
	$show_details_query->fields('s', array('sub_div_name')); 
	$show_details_query->fields('bm', array('block_mun_name')); 
	$show_details_query->fields('v', array('village_name','category')); 
	$show_details_query->fields('p', array('name_of_police_station')); 
	$show_details_query->fields('t', array('name_of_ind_est','addlineone','reg_linc_no','verification_dt','verification_tm','other_nature_name','nature_work','ind_est_pin_code','ins_verify_note','ins_verify_place','verified_showcause_status','alc_bfsc_note','alc_back_note','alc_insnote_rmks'));
	$show_details_query->fields('cw', array('cont_work_name')); 
	$show_details_query->condition('t.ins_file_number',trim($id),'=');
	$show_details_result = $show_details_query->execute();  
		
	
    $content = array();
	
    foreach($show_details_result as $show_details_row) {
		$content[]=$show_details_row;
	}
	
	$pre_person_query = db_query('select  * from l_insp_present_person where  ins_file_number = :filenumber  ', array(':filenumber' => trim($id)));
    
	$content2 = array();
	
      
    foreach($pre_person_query as $person_result){
		$content2[] = $person_result;
	}
			 
	$det_act_sc_ts_query = db_select('l_infringment', 'a');
	$det_act_sc_ts_query->join('l_inspection_establishment', 'b', 'a.ins_file_number = b.ins_file_number');
	$det_act_sc_ts_query->condition('a.ins_file_number',trim($id),'=');
	$det_act_sc_ts_query->condition('b.type_of_insnote_user', 'ins_show_cause','=');
	$det_act_sc_ts_query->condition('b.ins_submit_status', 'S','=');
	$det_act_sc_ts_query->condition('b.verified_showcause_status', 'S','=');
	$det_act_sc_ts_query->condition('a.per_no_full_sc_status',array('A','V','B'),'IN');
	
	$det_act_sc_ts_query->fields('a', array('ins_file_number','per_no_full_sc_status','sc_cc_status'));
	$det_act_sc_ts_result = $det_act_sc_ts_query->execute(); 	
	
	$order='cust_infring_id';
	$sort='ASC';
	$infr_prin_query = db_select('l_infringment', 't');
	
	$infr_prin_query->condition('t.ins_file_number',trim($id),'=');
	
	if($det_act_sc_ts_result->rowCount() > 0) { 
		$infr_prin_query->condition('t.sc_cc_status','F','=');
	}
			   
	$infr_prin_query->fields('t', array('infring_name','type_of_infring','let_off_note','cc_lf_note_insp'));	
	
	$infr_prin_query->orderBy($order, $sort);
	
	$infr_prin_query_rst = $infr_prin_query->execute();	
	$numinfgM = $infr_prin_query_rst->rowCount();	
	$content4 = array();
    
	foreach($infr_prin_query_rst as $infgp_result){
		$content4[] = $infgp_result;
	}	
	
	$query_mt_worker= db_select('l_mt_worker_details', 'p');
	$query_mt_worker->leftjoin('state_master', 'sm', 'sm.id = p.state_id');
	$query_mt_worker->leftjoin('district_master', 'd', 'd.district_code = p.work_dist and d.state_id = sm.id');       
	$query_mt_worker->leftJoin('sub_division', 's', 's.sub_div_code = p.worker_sub_div and d.district_code = s.district_code ');
	$query_mt_worker->leftJoin('block_mun_master', 'bm', 'bm.block_code = p.worker_block and bm.distict_code = d.district_code and s.sub_div_code = bm.sub_division_code');
	$query_mt_worker->leftJoin('village_ward_master', 'v', 'v.village_code = p.worker_vill_word and v.block_code = bm.block_code');
	$query_mt_worker->leftJoin('police_station', 'r', 'r.police_station_code = p.worker_police_st and r.dristrict_code = d.district_code');
	$query_mt_worker->fields('sm', array('statename')); 
	$query_mt_worker->fields('d', array('district_name','district_code'));
	$query_mt_worker->fields('s', array('sub_div_name')); 
	$query_mt_worker->fields('bm', array('block_mun_name')); 
	$query_mt_worker->fields('v', array('village_name','category')); 
	$query_mt_worker->fields('r', array('name_of_police_station'));
	$query_mt_worker->fields('p', array('name_mt_worker','address_line','designation','own_pin','owner_email','owner_mobile','type_of_emp','other_type_name','other_country_name','country_id'));
	$query_mt_worker->condition('p.ins_file_number',trim($id),'=');
	$query_mt_worker_result = $query_mt_worker->execute(); 
	  
	$content5 = array();
    
	foreach ($query_mt_worker_result as $work_det){		
		$content5[]=$work_det;		
	} 
	 
	$content7 = trim($id);
	
	if($det_act_sc_ts_result->rowCount() > 0){
		return theme('alcletoffnoteview',array('content' => $content,'content2' => $content2,'content4' => $content4,'content5' =>$content5,'content7' => $content7,'content8' => $numinfgM,'content9' => '')); 	 
	}else{	  
	 	return theme('alcinsnoteview',array('content' => $content,'content2' => $content2,'content4' => $content4,'content5' =>$content5,'content7' => $content7,'content8' => $numinfgM,'content9' => ''));	 
	}
}

function alc_showcause_notice_view($id=NULL) {
	  global $base_root, $base_path;
	 
     $id= encryption_decryption_fun('decrypt', trim($id));
	
		
		$show_details_query = db_select('l_inspection_establishment', 't');
        $show_details_query->join('district_master', 'd', 'd.district_code = t.ind_est_dist');
        $show_details_query->join('sub_division', 's', 's.sub_div_code = t.ind_est_subdivision and d.district_code = s.district_code ');
		$show_details_query->join('block_mun_master', 'bm', 'bm.block_code = t.ind_est_block and bm.distict_code = d.district_code and s.sub_div_code = bm.sub_division_code');
		$show_details_query->join('village_ward_master', 'v', 'v.village_code = t.est_vill_ward and v.block_code = bm.block_code');
		$show_details_query->join('police_station', 'p', 'p.police_station_code = t.police_station_one and p.dristrict_code = d.district_code');
		$show_details_query->join('contractor_works', 'cw', 'cw.contractor_work_id = t.nature_work ');
        $show_details_query->fields('d', array('district_name'));
        $show_details_query->fields('s', array('sub_div_name')); 
		$show_details_query->fields('bm', array('block_mun_name')); 
		$show_details_query->fields('v', array('village_name','category')); 
		$show_details_query->fields('p', array('name_of_police_station')); 
        $show_details_query->fields('cw', array('cont_work_name')); 
        $show_details_query->fields('t', array('name_of_ind_est','addlineone','reg_linc_no','other_nature_name','nature_work','ind_est_pin_code','ins_dt','ins_tm','pre_ins_file_number','verification_dt','verification_tm'));
	    $show_details_query->condition('t.ins_file_number',trim($id),'=');
		
	    $show_details_result = $show_details_query->execute();  
		
	
      $content=array();
     foreach ($show_details_result as $show_details_row) {
		
		$content[]=$show_details_row;
		
	  }
	 
		
	  $pre_person_query=db_query('select  * from l_insp_present_person where  ins_file_number = :filenumber  ', array(':filenumber' => trim($id)));
      $content2=array();
     foreach ($pre_person_query as $person_result) {
		
		$content2[]=$person_result;
		
	  }
			 
	         $order='cust_infring_id';
		     $sort='ASC';
	    	$infr_prin_query = db_select('l_infringment', 't');
	   
	        $infr_prin_query->condition('t.ins_file_number',trim($id),'=');

           $infr_prin_query->condition('t.sc_cc_status','Y','=');
          $infr_prin_query->fields('t', array('infring_name','type_of_infring','verification_dt','verification_tm','ins_verify_place'));	
   
          $infr_prin_query->orderBy($order, $sort);
       
 
        $infr_prin_query_rst =  $infr_prin_query->execute(); 
	 
	$content4=array();
     foreach ($infr_prin_query_rst as $infgp_result) {
		
		$content4[]=$infgp_result;
		
	  }

	 
/*	  
	  */
	  
	  
	 //$personal_row=db_query('select  * from l_pemp_infringment where  ins_file_number = :filenumber  and type_of_infring=:tinf', array(':filenumber' =>trim($_SESSION['insMotorFileId']),':tinf' => 'P'));
	// $numinfgP=$personal_row->rowCount();
	
	$personal_row2=db_query('select  * from l_infringment where  ins_file_number = :filenumber  ', array(':filenumber' => trim($id)));
	$numinfgM=$personal_row2->rowCount();

     // print "sssssssssssssssssssssss".$numinfgM;
	  //exit;

    $inspection_status=db_query('select  * from l_inspection_establishment where  ins_file_number=:filenumber', array(':filenumber' => trim($id)) );
	   
	   $result=$inspection_status->fetchAssoc();
	   $ins_status= trim($result['type_of_insnote_user']);
    




	  
	$query_mt_worker= db_select('l_mt_worker_details', 'p');
	 $query_mt_worker->leftjoin('state_master', 'sm', 'sm.id = p.state_id');
	$query_mt_worker->leftjoin('district_master', 'd', 'd.district_code = p.work_dist and d.state_id = sm.id');       
	$query_mt_worker->leftjoin('sub_division', 's', 's.sub_div_code = p.worker_sub_div and d.district_code = s.district_code ');
	$query_mt_worker->leftjoin('block_mun_master', 'bm', 'bm.block_code = p.worker_block and bm.distict_code = d.district_code and s.sub_div_code = bm.sub_division_code');
	
	$query_mt_worker->leftjoin('village_ward_master', 'v', 'v.village_code = p.worker_vill_word and v.block_code = bm.block_code');
	$query_mt_worker->leftjoin('police_station', 'r', 'r.police_station_code = p.worker_police_st and r.dristrict_code = d.district_code');
	$query_mt_worker->fields('d', array('district_name','district_code'));
	$query_mt_worker->fields('s', array('sub_div_name')); 
	$query_mt_worker->fields('bm', array('block_mun_name')); 
	 $query_mt_worker->fields('sm', array('statename'));
	$query_mt_worker->fields('v', array('village_name','category')); 
	$query_mt_worker->fields('r', array('name_of_police_station'));
	$query_mt_worker->fields('p', array('name_mt_worker','address_line','designation','own_pin','owner_email','owner_mobile','type_of_emp','other_type_name','fhname','other_country_name','country_id'));
	$query_mt_worker->condition('p.ins_file_number',trim($id),'=');
	//if($ins_status=='ins_show_cause')
	// $query_mt_worker->range(0, 1);
	$query_mt_worker_result = $query_mt_worker->execute(); 
	
	
	
	 
		
	 $content5=array();
     foreach ($query_mt_worker_result as $work_det) {
		
		$content5[]=$work_det;
		
	  } 
	  
	  
	  
	  
	  
	  
	 
	  $content7=trim($id);
	  
	  
	   
	    
	  
	   
	      return theme('alcshowcausenoticeview',array('content' => $content,'content2' => $content2,'content4' => $content4,'content5' =>$content5,'content7' => $content7,'content8' => $numinfgM,'content9' => '','content3' => $content3));   


}

function bfcc_let_off_permission($id=NULL) {
	  global $base_root, $base_path;
	 
     $id= encryption_decryption_fun('decrypt', trim($id));
	
		
		$show_details_query = db_select('l_inspection_establishment', 't');
        $show_details_query->join('district_master', 'd', 'd.district_code = t.ind_est_dist');
        $show_details_query->join('sub_division', 's', 's.sub_div_code = t.ind_est_subdivision and d.district_code = s.district_code ');
		$show_details_query->join('block_mun_master', 'bm', 'bm.block_code = t.ind_est_block and bm.distict_code = d.district_code and s.sub_div_code = bm.sub_division_code');
		$show_details_query->join('village_ward_master', 'v', 'v.village_code = t.est_vill_ward and v.block_code = bm.block_code');
		$show_details_query->join('police_station', 'p', 'p.police_station_code = t.police_station_one and p.dristrict_code = d.district_code');
		$show_details_query->join('contractor_works', 'cw', 'cw.contractor_work_id = t.nature_work ');
        $show_details_query->fields('d', array('district_name'));
        $show_details_query->fields('s', array('sub_div_name')); 
		$show_details_query->fields('bm', array('block_mun_name')); 
		$show_details_query->fields('v', array('village_name','category')); 
		$show_details_query->fields('p', array('name_of_police_station')); 
        $show_details_query->fields('cw', array('cont_work_name')); 
        $show_details_query->fields('t', array('name_of_ind_est','addlineone','reg_linc_no','other_nature_name','nature_work','ind_est_pin_code','ins_dt','ins_tm','pre_ins_file_number','verification_dt','verification_tm','alc_bf_cc_lf_back_note','alc_bf_cc_lf_app_note'));
	    $show_details_query->condition('t.ins_file_number',trim($id),'=');
		
	    $show_details_result = $show_details_query->execute();  
		
	
      $content=array();
     foreach ($show_details_result as $show_details_row) {
		
		$content[]=$show_details_row;
		
	  }
	 
		
	  $pre_person_query=db_query('select  * from l_insp_present_person where  ins_file_number = :filenumber  ', array(':filenumber' => trim($id)));
      $content2=array();
     foreach ($pre_person_query as $person_result) {
		
		$content2[]=$person_result;
		
	  }
			 
	         $order='cust_infring_id';
		     $sort='ASC';
	    	$infr_prin_query = db_select('l_infringment', 't');
	   
	        $infr_prin_query->condition('t.ins_file_number',trim($id),'=');

           $infr_prin_query->condition('t.sc_cc_status','F','=');
          $infr_prin_query->fields('t', array('infring_name','type_of_infring','verification_dt','verification_tm','ins_verify_place','cc_lf_note_insp'));	
   
          $infr_prin_query->orderBy($order, $sort);
       
 
        $infr_prin_query_rst =  $infr_prin_query->execute(); 
	 
	$content4=array();
     foreach ($infr_prin_query_rst as $infgp_result) {
		
		$content4[]=$infgp_result;
		
	  }

	 
/*	  
	  */
	  
	  
	 //$personal_row=db_query('select  * from l_pemp_infringment where  ins_file_number = :filenumber  and type_of_infring=:tinf', array(':filenumber' =>trim($_SESSION['insMotorFileId']),':tinf' => 'P'));
	// $numinfgP=$personal_row->rowCount();
	
	$personal_row2=db_query('select  * from l_infringment where  ins_file_number = :filenumber  ', array(':filenumber' => trim($id)));
	$numinfgM=$personal_row2->rowCount();

     // print "sssssssssssssssssssssss".$numinfgM;
	  //exit;

    $inspection_status=db_query('select  * from l_inspection_establishment where  ins_file_number=:filenumber', array(':filenumber' => trim($id)) );
	   
	   $result=$inspection_status->fetchAssoc();
	   $ins_status= trim($result['type_of_insnote_user']);
    




	  
	$query_mt_worker= db_select('l_mt_worker_details', 'p');
	$query_mt_worker->leftjoin('state_master', 'sm', 'sm.id = p.state_id');
	$query_mt_worker->leftjoin('district_master', 'd', 'd.district_code = p.work_dist and d.state_id = sm.id');  
	$query_mt_worker->leftjoin('sub_division', 's', 's.sub_div_code = p.worker_sub_div and d.district_code = s.district_code ');
	$query_mt_worker->leftjoin('block_mun_master', 'bm', 'bm.block_code = p.worker_block and bm.distict_code = d.district_code and s.sub_div_code = bm.sub_division_code');
	
	$query_mt_worker->leftjoin('village_ward_master', 'v', 'v.village_code = p.worker_vill_word and v.block_code = bm.block_code');
	$query_mt_worker->leftjoin('police_station', 'r', 'r.police_station_code = p.worker_police_st and r.dristrict_code = d.district_code');
	$query_mt_worker->fields('d', array('district_name','district_code'));
	$query_mt_worker->fields('s', array('sub_div_name')); 
	$query_mt_worker->fields('bm', array('block_mun_name')); 
	 $query_mt_worker->fields('sm', array('statename')); 
	$query_mt_worker->fields('v', array('village_name','category')); 
	$query_mt_worker->fields('r', array('name_of_police_station'));
	$query_mt_worker->fields('p', array('name_mt_worker','address_line','designation','own_pin','owner_email','owner_mobile','type_of_emp','other_type_name','fhname','other_country_name','country_id'));
	$query_mt_worker->condition('p.ins_file_number',trim($id),'=');
	//if($ins_status=='ins_show_cause')
	// $query_mt_worker->range(0, 1);
	$query_mt_worker_result = $query_mt_worker->execute(); 
	
	
	
	 
		
	 $content5=array();
     foreach ($query_mt_worker_result as $work_det) {
		
		$content5[]=$work_det;
		
	  } 
	  
	  
	  
	  
	  
	  
	 
	  $content7=trim($id);
	  
	  
	   
	    
	  
	   
	      return theme('alcccletoffview',array('content' => $content,'content2' => $content2,'content4' => $content4,'content5' =>$content5,'content7' => $content7,'content8' => $numinfgM,'content9' => '','content3' => $content3));   


}

function court_case_permission($id=NULL) {
	  global $base_root, $base_path;
	 
     $id= encryption_decryption_fun('decrypt', trim($id));
	
		
		$show_details_query = db_select('l_inspection_establishment', 't');
        $show_details_query->join('district_master', 'd', 'd.district_code = t.ind_est_dist');
        $show_details_query->join('sub_division', 's', 's.sub_div_code = t.ind_est_subdivision and d.district_code = s.district_code ');
		$show_details_query->join('block_mun_master', 'bm', 'bm.block_code = t.ind_est_block and bm.distict_code = d.district_code and s.sub_div_code = bm.sub_division_code');
		$show_details_query->join('village_ward_master', 'v', 'v.village_code = t.est_vill_ward and v.block_code = bm.block_code');
		$show_details_query->join('police_station', 'p', 'p.police_station_code = t.police_station_one and p.dristrict_code = d.district_code');
		$show_details_query->join('contractor_works', 'cw', 'cw.contractor_work_id = t.nature_work ');
        $show_details_query->fields('d', array('district_name'));
        $show_details_query->fields('s', array('sub_div_name')); 
		$show_details_query->fields('bm', array('block_mun_name')); 
		$show_details_query->fields('v', array('village_name','category')); 
		$show_details_query->fields('p', array('name_of_police_station')); 
        $show_details_query->fields('cw', array('cont_work_name')); 
        $show_details_query->fields('t', array('name_of_ind_est','addlineone','reg_linc_no','other_nature_name','nature_work','ind_est_pin_code','ins_dt','ins_tm','pre_ins_file_number','verification_dt','verification_tm','alc_cc_per_back_note','alc_cc_per_app_note','pre_ins_file_number'));
	    $show_details_query->condition('t.ins_file_number',trim($id),'=');
		
	    $show_details_result = $show_details_query->execute();  
		
	
      $content=array();
     foreach ($show_details_result as $show_details_row) {
		
		$content[]=$show_details_row;
		
	  }
	 
		
	  $pre_person_query=db_query('select  * from l_insp_present_person where  ins_file_number = :filenumber  ', array(':filenumber' => trim($id)));
      $content2=array();
     foreach ($pre_person_query as $person_result) {
		
		$content2[]=$person_result;
		
	  }
			 
	         $order='cust_infring_id';
		     $sort='ASC';
	    	$infr_prin_query = db_select('l_infringment', 't');
	   
	        $infr_prin_query->condition('t.ins_file_number',trim($id),'=');

           $infr_prin_query->condition('t.sc_cc_status','Y','=');
          $infr_prin_query->fields('t', array('infring_name','type_of_infring','verification_dt','verification_tm','ins_verify_place','cc_lf_note_insp','sc_cc_status','per_no_full_sc_status','alc_cc_let_st','alc_cc_lf_note'));	
   
          $infr_prin_query->orderBy($order, $sort);
       
 
        $infr_prin_query_rst =  $infr_prin_query->execute(); 
		$numinfgM=$infr_prin_query_rst->rowCount();
	 
	$content4=array();
     foreach ($infr_prin_query_rst as $infgp_result) {
		
		$content4[]=$infgp_result;
		
	  }

	 
/*	  
	  */
	  
	  
	 //$personal_row=db_query('select  * from l_pemp_infringment where  ins_file_number = :filenumber  and type_of_infring=:tinf', array(':filenumber' =>trim($_SESSION['insMotorFileId']),':tinf' => 'P'));
	// $numinfgP=$personal_row->rowCount();
	
	//$personal_row2=db_query('select  * from l_infringment where  ins_file_number = :filenumber  ', array(':filenumber' => trim($id)));
	

     // print "sssssssssssssssssssssss".$numinfgM;
	  //exit;

    $inspection_status=db_query('select  * from l_inspection_establishment where  ins_file_number=:filenumber', array(':filenumber' => trim($id)) );
	   
	   $result=$inspection_status->fetchAssoc();
	   $ins_status= trim($result['type_of_insnote_user']);
    




	  
	$query_mt_worker= db_select('l_mt_worker_details', 'p');
	$query_mt_worker->leftjoin('state_master', 'sm', 'sm.id = p.state_id');
    $query_mt_worker->leftjoin('district_master', 'd', 'd.district_code = p.work_dist and d.state_id = sm.id');
	$query_mt_worker->leftjoin('sub_division', 's', 's.sub_div_code = p.worker_sub_div and d.district_code = s.district_code ');
	$query_mt_worker->leftjoin('block_mun_master', 'bm', 'bm.block_code = p.worker_block and bm.distict_code = d.district_code and s.sub_div_code = bm.sub_division_code');
     $query_mt_worker->fields('sm', array('statename')); 
	
	$query_mt_worker->leftjoin('village_ward_master', 'v', 'v.village_code = p.worker_vill_word and v.block_code = bm.block_code');
	$query_mt_worker->leftjoin('police_station', 'r', 'r.police_station_code = p.worker_police_st and r.dristrict_code = d.district_code');
	$query_mt_worker->fields('d', array('district_name','district_code'));
	$query_mt_worker->fields('s', array('sub_div_name')); 
	$query_mt_worker->fields('bm', array('block_mun_name')); 
	
	$query_mt_worker->fields('v', array('village_name','category')); 
	$query_mt_worker->fields('r', array('name_of_police_station'));
	$query_mt_worker->fields('p', array('name_mt_worker','address_line','designation','own_pin','owner_email','owner_mobile','type_of_emp','other_type_name','fhname','other_country_name','country_id'));
	$query_mt_worker->condition('p.ins_file_number',trim($id),'=');
	//if($ins_status=='ins_show_cause')
	// $query_mt_worker->range(0, 1);
	$query_mt_worker_result = $query_mt_worker->execute(); 
	
	
	
	 
		
	 $content5=array();
     foreach ($query_mt_worker_result as $work_det) {
		
		$content5[]=$work_det;
		
	  } 
	  
	  
	  
	  
	  
	  
	 
	     $content7=trim($id);
	  
	  
	   
	    
	  
	   
	      return theme('alcccperview',array('content' => $content,'content2' => $content2,'content4' => $content4,'content5' =>$content5,'content7' => $content7,'content8' => $numinfgM,'content9' => '','content3' => $content3));   


}
?>