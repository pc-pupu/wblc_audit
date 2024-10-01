<?php
function alc_courtcase_permission(){
	  
	  
	global $base_root, $base_path , $full_base, $user ;
	 
	 
           $uid=$user->uid; 
	
	         $query_inspector_nm=db_query(' select   fullname   from l_custom_user_detail where  usr_id = :uid', array(':uid' => trim($uid) ));	
		     $result_insnm=$query_inspector_nm->fetchAssoc();
	         $ins_fullname = $result_insnm['fullname'];  
	  
	  
	
	   $ins_file_no=trim($_POST['ins_file_no']);
	   //$alc_bfsc_note=trim($_POST['alc_cc_per_note']);
	   $ins_row=trim($_POST['ins_row']);
	  // $alc_cc_per_status=trim($_POST['alc_cc_per_status']);
	   
	   
	//print $ins_file_no.'sssss'.$ins_row;
	//exit;
	 
	$txn = db_transaction();

      try { 
	  
	      
	 
	  
	 
	     for($j=1; $j<=$ins_row ; $j++){
		    $type_of_ins_act=trim($_POST['type_of_ins_act'.$j]); 
		     $ccsbtype=trim($_POST['ccsbtype'.$j]); 
			 $cc_note=trim($_POST['cc_note'.$j]); 
			 if(trim($ccsbtype)){
				 
			  if(trim($ccsbtype)=='A')	{
				  
				 $query = db_update('l_inspection_establishment');
                  $query->fields(array(
        
		
		 
		          'alc_cc_per_app_dt'=>date("Y-m-d"),
		
		
		 
                  ));
		 
		   $query->condition('ins_file_number',trim($ins_file_no),'=');
	       $query->condition('ins_submit_status','S','=');
		   $query->condition('verified_showcause_status','S','=');
           $query->condition('type_of_insnote_user','ins_court_case','=');
	       $query->execute();    
			  }
	   elseif(trim($ccsbtype)=='B'){
				  
				  $query = db_update('l_inspection_establishment');
                $query->fields(array(
        
		
		 
		          'alc_cc_per_back_dt'=>date("Y-m-d"),
		          'alc_cc_per_back_note'=>$cc_note,
		
		 
                  ));
		 
		   $query->condition('ins_file_number',trim($ins_file_no),'=');
	       $query->condition('ins_submit_status','S','=');
		   $query->condition('verified_showcause_status','S','=');
           $query->condition('type_of_insnote_user','ins_court_case','=');
	       $query->execute();   
			  }
				 
				 
				 
		    if(trim($ccsbtype)=='A')	{
			   
		    $queryM = db_update('l_infringment');
		    $queryM->fields(array(
            'alc_cc_let_st' =>$ccsbtype,
		    'ins_user_id'=>$uid,
		    'alc_cc_lf_note'=>$cc_note,
			'ins_alc_name'=>$ins_fullname,
		    'cc_lf_level_cc_app_dt'=>date("Y-m-d"),
		  
		   ));
          $queryM->condition('ins_file_number',$ins_file_no,'=');
		  $queryM->condition('type_of_infring',$type_of_ins_act,'=');
		  $queryM->condition('per_no_full_sc_status',array('S','N'),'IN');
		  $queryM->condition('sc_cc_status','Y','=');
	   
         $queryM->execute(); 
			}
				 
		 elseif(trim($ccsbtype)=='B')	{
			   
		    $queryM = db_update('l_infringment');
		    $queryM->fields(array(
            'alc_cc_let_st' =>$ccsbtype,
		    'ins_user_id'=>$uid,
		    'alc_cc_lf_note'=>$cc_note,
			'ins_alc_name'=>$ins_fullname,
		    'cc_lf_level_cc_back_dt'=>date("Y-m-d"),
		  
		   ));
          $queryM->condition('ins_file_number',$ins_file_no,'=');
		  $queryM->condition('type_of_infring',$type_of_ins_act,'=');
		  $queryM->condition('per_no_full_sc_status',array('S','N'),'IN');
		  $queryM->condition('sc_cc_status','Y','=');
	   
         $queryM->execute(); 
			}
	     elseif(trim($ccsbtype)=='R')	{
			   
		    $queryM = db_update('l_infringment');
		    $queryM->fields(array(
            'alc_cc_let_st' =>$ccsbtype,
		    'ins_user_id'=>$uid,
		    'alc_cc_lf_note'=>$cc_note,
			'ins_alc_name'=>$ins_fullname,
		    'cc_lf_level_reco_dt'=>date("Y-m-d"),
		  
		   ));
          $queryM->condition('ins_file_number',$ins_file_no,'=');
		  $queryM->condition('type_of_infring',$type_of_ins_act,'=');
		  $queryM->condition('per_no_full_sc_status',array('S','N'),'IN');
		  $queryM->condition('sc_cc_status','Y','=');
	   
         $queryM->execute(); 
			}
			
			
		   
		  $message = ' courtcase successfully submit . ';  
		  
		  
		  
		  
		   
		 }
		  
	 
	
	
	
	   
	   
	   
	   
	   
	    
		 }
	   
	    }
		catch (Exception $e) {
   
         $txn->rollback();
   
      watchdog_exception('type', $e);
	  $message = '  courtcase successfully not submit . ';
	  
	  
	  
      }
  
		
         $full_base = $base_root . $base_path.'alcinspection';
		  
       drupal_set_message(t($message));
	   drupal_goto($full_base); 
	 
	 

}

function alc_bfcourtcase_letoff_permission(){
	  
	  
	global $base_root, $base_path , $full_base , $user ;
	 
	 
             $uid=$user->uid; 
	
	         $query_inspector_nm=db_query(' select   fullname   from l_custom_user_detail where  usr_id = :uid', array(':uid' => trim($uid) ));	
		     $result_insnm=$query_inspector_nm->fetchAssoc();
	         $ins_fullname = $result_insnm['fullname'];  
	  
	  
	
	   $ins_file_no=trim($_POST['ins_file_no']);
	   $alc_bfcc_lt_note=trim($_POST['alc_bfcc_lt_note']);
	   $ins_row=trim($_POST['ins_row']);
	   $alc_bf_cc_lf_status=trim($_POST['alc_bf_cc_lf_status']);
	   
	   
	//print $ins_row.$alc_bf_cc_lf_status.$alc_bfcc_lt_note.$ins_file_no;
	//exit;
	 
	$txn = db_transaction();

      try { 
	  
	 if($alc_bf_cc_lf_status=='B'){
	   $query = db_update('l_inspection_establishment');
         $query->fields(array(
        
		 'alc_bf_cc_lf_back_note'=>$alc_bfcc_lt_note,
		 
		 'alc_bf_cc_lf_back_dt'=>date("Y-m-d"),
		
		
		 
         ));
		 
		   $query->condition('ins_file_number',trim($ins_file_no),'=');
	       $query->condition('ins_submit_status','S','=');
		   $query->condition('verified_showcause_status','S','=');
           $query->condition('type_of_insnote_user','ins_court_case','=');
	       $query->execute(); 
	   
	  } 
	  elseif($alc_bf_cc_lf_status=='A'){
	   $query = db_update('l_inspection_establishment');
         $query->fields(array(
        
		 'alc_bf_cc_lf_app_note'=>$alc_bfcc_lt_note,
		 
		 'alc_bf_cc_lf_app_dt'=>date("Y-m-d"),
		
		
		 
         ));
		 
		   $query->condition('ins_file_number',trim($ins_file_no),'=');
	       $query->condition('ins_submit_status','S','=');
		   $query->condition('verified_showcause_status','S','=');
           $query->condition('type_of_insnote_user','ins_court_case','=');
	       $query->execute(); 
	   
	  } 
	  
	 
	     for($j=1; $j<=$ins_row ; $j++){
		   $type_of_ins_act=trim($_POST['type_of_ins_act'.$j]); 
		   
		    $queryM = db_update('l_infringment');
		   $queryM->fields(array(
          'per_no_full_sc_status' =>$alc_bf_cc_lf_status,
		   'ins_user_id'=>$uid,
		  'ins_alc_name'=>$ins_fullname
		  
		  
		   ));
          $queryM->condition('ins_file_number',$ins_file_no,'=');
		  $queryM->condition('type_of_infring',$type_of_ins_act,'=');
		  $queryM->condition('per_no_full_sc_status',array('V','B'),'IN');
		  $queryM->condition('sc_cc_status','F','=');
	   
         $queryM->execute(); 
		   
		   
		   
		   
	   }
	   
	   
	    if($alc_bf_cc_lf_status=='A'){
		 $ss='letoff approved';	
		}
		 elseif($alc_bf_cc_lf_status=='B'){
	     $ss='letoff back to inspector';
		 }
	   
	    $message = ' successfully . '.$ss;
	   
	    }
		catch (Exception $e) {
   
         $txn->rollback();
   
      watchdog_exception('type', $e);
	  $message = '  courtcase successfully no approved . ';
	  
	  
	  
      }
  
		
         $full_base = $base_root . $base_path.'alcinspection';
		  
       drupal_set_message(t($message));
	   drupal_goto($full_base); 
	 
	 

}




?>