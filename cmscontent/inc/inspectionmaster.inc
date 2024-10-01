<?php
function inspection_master_act_list(){	
	 global $base_root, $base_path, $user;

	 $uid	=	$user->uid; 
    
    $header = array();
	
	$infr_prin_query_rst = db_query(' select inspection_txt_type  from  l_inspection_master group by inspection_txt_type order by inspection_txt_type asc ' );	
	
	$header = array(
				array('data'=>'SL No.', 'width' => '8%'),
				array('data'=> 'INSPECTION  ACT'),
				array('data'=> 'Action', 'width' => '12%'),
				);
									
	$p = 0;
	
	foreach($infr_prin_query_rst as $data) {		
		$p++;
			
		$query_ins_nm =	db_query('select inspection_name from l_inspection_master where inspection_txt_type = :instype limit 1 ', array(':instype' => trim($data->inspection_txt_type) ));	
		
	    $result	=   $query_ins_nm->fetchAssoc();
	    $infring_name = $result['inspection_name'];
		
		$aa = $base_root.$base_path.'cmscontent/editinspectioninfringements/'.encryption_decryption_fun('encrypt',$data->inspection_txt_type);
	    $link_s = $link_s=l(t('<span class="badge bg-blue"><i class="fa fa-edit"></i> Edit Details</span>'), $aa, array('html' => TRUE)) ;
		
		$rows[] = array(
						$p,
						$infring_name,
						$link_s,
	  		   ); 
	}
	
	
	$variables = array(
	  		'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')), 
	  		'header' => $header,
	  		'rows' => $rows,
			'empty' => t("No data found!")
	  		);
	
	
 	 $output = theme('datatable', $variables); 
 
	 $bb = $base_root.$base_path.'cmscontent/addnewinspectionact';
	 $add_new_act = l(t('ADD NEW INSPECTION ACT'), $bb, array('attributes' => array('class' => 'btn btn-primary pull-left')));
	 
	 return '<div class="row"><div class="col-md-12"><div class="box box-primary"><div class="box-body">'.$output.'</div></div></div></div>';
}
	function edit_inspection_master_infrigement($form, &$form_state, $typ_infg=NULL){
	

		global $base_root, $base_path,$full_base;
		  $typ_infg= encryption_decryption_fun('decrypt', trim($typ_infg));
		  $query_ins_nm	=	db_query('select inspection_name from l_inspection_master where inspection_txt_type = :instype limit 1 ', array(':instype' => trim($typ_infg) ));	
	      $result			=   $query_ins_nm->fetchAssoc();
	      $infring_name   =   $result['inspection_name'];  
		  
		  
 $form['inspmasterinfgfield'] = array(
          '#title' => t(''),
          '#type' => 'fieldset',
          '#collapsed' => FALSE
          );
		   $aa=$base_root.$base_path.'cmscontent/inspectionmasteract';
		   $jj=l(t('<font color="#CC3366"><b>BACK</b></font>'), $aa,array('html'=>TRUE)) ; 	
			$form['inspmasterinfgfield']['infglist'] = array(
		    '#markup'=>$jj,
	       );  
		  $form['inspmasterinfgfield']['act_name'] = array
        (
		    '#prefix' => '',
	        '#suffix' => '',
			'#title'=>'Modify Act Name',
		    '#size'=>t('50'),
		    '#default_value' => $infring_name,
		    '#type' => 'textfield',
			'#attributes'=> array('class' => array('form-control')),	
		    '#cols' => 10, 
            '#rows' => 1,
		
        );
		$bb=$base_root.$base_path.'cmscontent/addinsinfrigement/'.encryption_decryption_fun('encrypt',$typ_infg);;

		$ADD_ANOTHER_INFRINGEMENT =l(t('ADD OTHER INFRINGEMENT'), $bb, array('attributes' => array('class' => 'btn btn-primary pull-left')));	
		 $form['inspmasterinfgfield']['add_other_infg'] = array
        (
		    '#prefix' => '<b>',
	        '#suffix' => '</b>',
		    '#markup'=> $ADD_ANOTHER_INFRINGEMENT .'<BR><BR>',
	   			
	    
        );  
		  
		   $form['inspmasterinfgfield']['insslno'] = array
        (
		  '#prefix' => '<br class="clear"><table width="100%" class="table admin-custom-table"><tr><th width="5%"><b>',
	      '#suffix' => '</b></th>',
          '#markup'=>'SL.NO',
	   
	   
        );
		$form['inspmasterinfgfield']['pensionfc'] = array
        (
		    '#prefix' => '<th width="7%"><b>',
	        '#suffix' => '</b></th>',
		    '#markup'=>'CHECKBOX',
	   
	    
        );
		$form['inspmasterinfgfield']['insarea'] = array
        (
		    '#prefix' => '<th><b>',
	        '#suffix' => '</b></th></tr>',
		    '#markup'=>'INFRINGEMENTS',
	   
	   
        );
		$order='txt_order';
		$sort='ASC';
		
		$show_details_query = db_select('l_inspection_master', 't');
	    $show_details_query->fields('t', array('ispection_id','inspection_txt','inspection_txt_type','txt_order'));
		$show_details_query->condition('t.inspection_txt_type',  trim($typ_infg) , '='); 
		$show_details_query->orderBy($order, $sort); 
		$total_name =  $show_details_query->execute();
		$numrow=$total_name->rowCount(); 
		$i=0;
		foreach ($total_name as $data) {
			$i++;
	     $ispection_id = $data->ispection_id; 
         $inspection_txt = $data->inspection_txt;
		 
		 
	
	   
 $form['inspmasterinfgfield']['slno'.$i] = array
          (
		 '#prefix' => '<tr><td>',
	     '#suffix' => '</td>',
         '#markup'=>''. $i.'.'.'',
		
          );
		  
	
		  
	$form['inspmasterinfgfield']['insid'.$i] = array
          (
		 '#prefix' => '<td align="center">',
	      '#suffix' => '</td>',
	      '#type' => 'checkbox',
		
	

          ); 
  $form['inspmasterinfgfield']['reprin'.$i] = array
        (
		    '#prefix' => '<td align="center">',
	        '#suffix' =>  '</td></tr>',
		    '#attributes' => array('class' => array('textarea-inf-note')),
            '#type' => 'textarea',
			'#cols' => 30, 
			'#rows' => 2,    
			'#attributes'=> array('class' => array('form-control')),
           '#default_value' =>trim($inspection_txt)
			);
			
			}
		
		 $form['inspmasterinfgfield']['typeinfrg'] = 
	     array('#type' => 'hidden',
		 '#prefix' => '<tr><td>',
	      '#suffix' =>  '</td>', 
	     '#value' =>  trim($typ_infg),
	   ); 
	   
	   
		$form['inspmasterinfgfield']['totalins'] = 
	    array('#type' => 'hidden', 
		'#id' => 'totalins',
		'#prefix' => '<td >',
	    '#suffix' =>  '</td>', 
	    '#value' => $numrow,
	   
	   );
	   $form['inspmasterinfgfield']['hiddinsid'.$i] = 
	    array('#type' => 'hidden',  
	    '#value' => $ispection_id,
		'#prefix' => '<td>',
	    '#suffix' => '</td></tr>',
	   
	   );
	   
	  
	   $form['inspmasterinfgfield']['submit'] = array (
		  '#prefix' => '<tr class="even"><td colspan="2" class="submit-right-border">&nbsp;</td><td  class="submit-right-border">',
	      '#suffix' => '</td></tr></table>',
          '#type' => 'submit',
		  '#attributes' => array('class' => array('btn btn-primary pull-left')),
          '#default_value' => 'Update',
         
        );
		
		$form['inspmasterinfgfield']['infglisttwo'] = array(
		'#markup'=>$jj,
	                                                    );  
	
	
 return $form;
 
 }
 function INSPECTION_MASTER_INFG_CHECKING_validate($element,&$form_states)
{
      $val=$form_states['values']; 
      $totalins=$val['totalins'];
	  $typeinfrg=$val['typeinfrg'];	  
	  $k=0;
	  for($i=1; $i<=$totalins ; $i++){
        $inspectorid=trim($val['insid'.$i]);
	
		if (!$inspectorid) {
		 $k++;
   
    }
	
   }
   
   if ($k==$totalins ) {
		 $query_ins_nm=db_query(' select   inspection_name   from l_inspection_master where  inspection_txt_type = :instype limit 1 ', array(':instype' => trim($typeinfrg) ));	
         $result=$query_ins_nm->fetchAssoc();
	     $infring_name = $result['inspection_name'];
		
		  form_error($element, t('PLEASE SELECT (CHECKBOX) INFRINGEMENTS  UNDER ('.$infring_name .')'));
          }
          }
		  
		function edit_inspection_master_infrigement_submit($form, &$form_state) {
	 
          $val=$form_state['values'];
          $totalins=$val['totalins'];
          $typeinfrg=$val['typeinfrg'];
		  $act_name=$val['act_name'];
          $txn = db_transaction();
		
		

try { 

  for($i=1; $i<=$totalins ; $i++){ 
	
	
	     $queryA = db_update('l_inspection_master');
         $queryA->fields(array(
         'inspection_name' =>$act_name,
		
		
		));
        
	      $queryA->condition('inspection_txt_type',$typeinfrg,'=');
	      $queryA->execute(); 
	 
		  
	$insid=trim($val['insid'.$i]);
  
   if($insid){
	  $insid=trim($val['hiddinsid'.$i]);
      $reprin=trim($val['reprin'.$i]);
      $queryCN = db_update('l_inspection_master');
         $queryCN->fields(array(
         'inspection_txt' =>$reprin,
		
		
		));
        
	    $queryCN->condition('inspection_txt_type',$typeinfrg,'=');
		$queryCN->condition('ispection_id',$insid,'=');
		
        $queryCN->execute(); 
        
      $msg =' AND INFRINGEMENTS'  ; 
     } 
	 }
     $message = '  INSPECTION ACT NAME  '.$msg.' SUCCESSFULLY UPDATE';
     }
catch (Exception $e) {
    // Something went wrong somewhere, so roll back now.
    $txn->rollback();
    // Log the exception to watchdog.
    watchdog_exception('type', $e);
	$message = 'INFRINGEMENTS   NOT UPDATE ';
  }
                             drupal_set_message(t($message));
				             global $base_root, $base_path,$full_base;
					    
				             $full_base=$base_root.$base_path.'cmscontent/editinspectioninfringements/'.encryption_decryption_fun('encrypt',$typeinfrg);
                             drupal_goto($full_base);
                            
     } 
	 
	 
	function add_other_inspection_infrigement($form, &$form_state, $typ_infg=NULL){
	
	
		global $base_root, $base_path,$full_base;
		  $typ_infg= encryption_decryption_fun('decrypt', trim($typ_infg));
		  
		  $query_ins_nm	=	db_query('select inspection_name from l_inspection_master where inspection_txt_type = :instype limit 1 ', array(':instype' => trim($typ_infg) ));	
		
	   	  $result			=   $query_ins_nm->fetchAssoc();
	      $infring_name   =   $result['inspection_name'];  
		  $form['circularaddrfields']['cont_head'] = array(
          '#type' 	=> 'markup',
		  '#markup' => '<div class="row"><section class="col-lg-12 connectedSortable ui-sortable"><div class="box box-solid #box-warning collapsed-box">',
          
          );
		  
		  $form['inspmasterinfgfield']['infglisttwo'] = array(
		                                                '#markup'=>$jj,
	                                                    );  
														
		/* $form['inspmasterinfgfield'] = array(
				  '#title' => t(''),
				  '#type' => 'fieldset',
				  '#prefix'				=> '<div class="form-custom col-md-4"><label class="input">',
		    	  '#suffix'				=>  '</label></div>',
				  '#collapsed' => FALSE
				  );*/
		   
                                                        
                                                        
          $form['inspmasterinfgfield']['act_name'] = array(
		    '#prefix'				=> '<div class="form-custom col-md-12"><label class="input">',
		    '#suffix'				=>  '</label></div>',
		    '#markup'=>'INSPECTION ACT:'. $infring_name .'<BR><BR>',
	        '#attributes' 		=> array('class' => array('form-control')),	
	    
        );
        
         $form['inspmasterinfgfield']['ins_infringement'] = array
        (
		    
		  '#title' => t('INFRIGEMENT TEXT:'),
		  '#attributes' => array('class' => array('form-control')),
		  '#prefix'			=> '<div class="row"><div class="col-md-12"><div class="col-md-9">',
		  '#suffix'			=>  '</div>',
		  '#type' => 'textarea',
		  '#cols' => 30, 
          '#rows' => 2,    
		);
		
	
        
        $form['inspmasterinfgfield']['typeinfrg'] = 
	    array('#type' => 'hidden',  
	 
	   
	    '#value' =>  trim($typ_infg),
	   );
        $form['inspmasterinfgfield']['submit'] = array (
		
          '#type' => 'submit',
          '#default_value' => 'Update',
		  '#attributes' => array('class' => array('btn btn-primary pull-left')),
		  '#prefix'	  => '<div class="col-md-6">',
		  '#suffix'	  =>  '</div></section>',
          '#element_validate' => array('INSPECTION_OTHER_INFG_CHECKING_validate'),
        
	  
        );
		
		
	
	
 return $form;
 
 }
 
 function INSPECTION_OTHER_INFG_CHECKING_validate($element,&$form_states)
 {
     $val=$form_states['values']; 
     
	  //$typeinfrg=$val['typeinfrg'];	  
	
   
  $prename=trim($val['ins_infringement']);
  
    
    
     $prename="a".trim($prename);
     
    if(stripos($prename,"~") >0  || stripos($prename,"!") >0 || stripos($prename,"@")>0 || stripos($prename,"<")>0 || stripos($prename,">")>0 || stripos($prename,"'")>0 || stripos($prename,"$")>0 || stripos($prename,"#")>0 || stripos($prename,"%")>0 || stripos($prename,"^")>0 || stripos($prename,"&")>0 || stripos($prename,"*")>0){ 
     
      form_error($element, t(' infringement text contain illegal character'));
     
     }
    }
		  
	
	
	
		  
          
     function add_other_inspection_infrigement_submit($form, &$form_state) {
	 
	 

         $val=$form_state['values'];


		
		
		  
		   $typeinfrg=trim($val['typeinfrg']);
		   $ins_infringement=check_plain(decode_entities(preg_replace('/\s+/', ' ',trim($val['ins_infringement']))));
           
            $txn = db_transaction();
		
		

try { 
 
           $maxresult=db_query('select  max(ispection_id) maxsl from l_inspection_master');	
	      $result=$maxresult->fetchAssoc();
	      $max_infringement_id = trim($result['maxsl']) + 1; 
		  
		  $query_ins_nm	=	db_query('select inspection_name from l_inspection_master where inspection_txt_type = :instype limit 1 ', array(':instype' => trim($typeinfrg) ));	
		
	    $result_infg_nm			=   $query_ins_nm->fetchAssoc();
	    $infring_name   =   $result_infg_nm['inspection_name'];  
  
   $fieldsE = array('ispection_id' => $max_infringement_id, 'inspection_txt' =>$ins_infringement,'inspection_txt_type' =>$typeinfrg, 'txt_order' => $max_infringement_id, 'inspection_name' => $infring_name);
		  
		   
            db_insert('l_inspection_master')->fields($fieldsE)->execute();
   
   
   
     $message = 'INFRINGEMENTS    SUCCESSFULLY INSERT';
     }
catch (Exception $e) {
    // Something went wrong somewhere, so roll back now.
    $txn->rollback();
    // Log the exception to watchdog.
    watchdog_exception('type', $e);
	$message = 'INFRINGEMENTS   NOT INSERT ';
  }
                             drupal_set_message(t($message));
				             global $base_root, $base_path,$full_base;
					    
				             $full_base=$base_root.$base_path.'cmscontent/editinspectioninfringements/'.encryption_decryption_fun('encrypt',$typeinfrg);
                             drupal_goto($full_base);
                            
     }  
	
	
	function new_inspection_infringement_checking_validate($element,&$form_states)
 {
     $val=$form_states['values']; 
     
	  //$typeinfrg=$val['typeinfrg'];	  
	
   
 // $prename=trim($val['ins_infringement']);
  
    
    
    
        $val="a".trim($element['#value']);
		if(stripos($val,"~") >0  || stripos($val,"!") >0 || stripos($val,"@")>0 || stripos($val,"<")>0 || stripos($val,">")>0 || stripos($val,"'")>0 || stripos($val,"$")>0 || stripos($val,"#")>0 || stripos($val,"%")>0 || stripos($val,"^")>0  || stripos($val,"*")>0){
		
		
       form_error($element, t($element['#title'].'  contain illegal characters'));
	       
           
       
		    
	   }
	
   }
	
	
	 
	function add_new_inspection_act($form, &$form_state) {
	 global $base_root, $base_path , $user;
	
	  $form['newinspectionfld'] = array(
          
          '#type' => 'fieldset',
          '#collapsed' => FALSE,
		  '#attributes' => array('class' => array('fieldset-ins-note'))
          );
		  $form['newinspectionfld']['infglisttwo'] = array(
		                                                '#markup'=>$jj,
	                                                    ); 
		 												
														
		    $form['newinspectionfld']['inspection_act_name'] = array(
			  '#prefix'				=> '<div class="form-custom col-md-12"><label class="input">',
		      '#suffix'				=>  '</label></div>',
			  '#title'=>'Name  of the Inspection Act',
			  '#size'=>t('50'),
              '#attributes' 		=> array('class' => array('form-control')),	
			  '#type' => 'textfield',
			  '#required' => TRUE,
			  '#element_validate' => array('new_inspection_infringement_checking_validate'),
			  '#description' =>  t('<b> Name of the inspection act   do not  contains some special character such as (~,!,@,<,>,$,#,%,*)</b>'),
			   );
							   
							   
		    $form['newinspectionfld']['ins_infringement_1'] = array
        (
		    
		     '#title' => t('INFRINGEMENT TEXT(1):'),
			 '#prefix'				=> '<div class="form-custom col-md-6"><label class="input">',
		      '#suffix'				=>  '</label></div>',
		     '#attributes' 		=> array('class' => array('form-control')),	
	         '#type' => 'textarea',
			 '#cols' => 30, 
             '#rows' => 2,    
		     '#element_validate' => array('new_inspection_infringement_checking_validate'),
		     '#description' =>  t('<b> infringement   text do not  contains some special character such as (~,!,@,<,>,$,#,%,*)</b>'),
	   
	   
	  
        );	
		$form['newinspectionfld']['ins_infringement_2'] = array
        (
		    
		     '#title' => t('INFRINGEMENT TEXT(2):'),
		     '#attributes' 		=> array('class' => array('form-control')),	
		     '#type' => 'textarea',
		     '#cols' => 30, 
             '#rows' => 2,    
	 	     '#prefix'				=> '<div class="form-custom col-md-6"><label class="input">',
		     '#suffix'				=>  '</label></div>',
	         '#element_validate' => array('new_inspection_infringement_checking_validate'),
		     '#description' =>  t('<b> infringement   text do not  contains some special character such as (~,!,@,<,>,$,#,%,*)</b>'),
	   
	   
	   
	  
        );
		$form['newinspectionfld']['ins_infringement_3'] = array
        (
		    
		    '#title' => t('INFRINGEMENT TEXT(3):'),
		    '#prefix'				=> '<div class="form-custom col-md-6"><label class="input">',
		    '#suffix'				=>  '</label></div>',
		    '#attributes' => array('class' => array('textarea-inf-note')),
		    '#type' => 'textarea',
		    '#cols' => 30, 
            '#rows' => 2,    
		    '#element_validate' => array('new_inspection_infringement_checking_validate'),
		    '#description' =>  t('<b> infringement   text do not  contains some special character such as (~,!,@,<,>,$,#,%,*)</b>'),
	   
	   
	  
        );
		$form['newinspectionfld']['ins_infringement_4'] = array
        (
		    
		    '#title' => t('INFRINGEMENT TEXT(4):'),
		    '#attributes' => array('class' => array('textarea-inf-note')),
	        '#type' => 'textarea',
		    '#cols' => 30, 
            '#rows' => 2, 
			'#prefix'				=> '<div class="form-custom col-md-6"><label class="input">',
		    '#suffix'				=>  '</label></div>',   
		    '#element_validate' => array('new_inspection_infringement_checking_validate'),
		    '#description' =>  t('<b> infringement   text do not  contains some special character such as (~,!,@,<,>,$,#,%,*)</b>'),
	   
	   
	   
	  
        );
		$form['newinspectionfld']['ins_infringement_5'] = array
        (
		     '#prefix'				=> '<div class="form-custom col-md-6"><label class="input">',
		     '#suffix'				=>  '</label></div>',
		     '#title' => t('INFRINGEMENT TEXT(5):'),
		     '#attributes' => array('class' => array('textarea-inf-note')),
		     '#type' => 'textarea',
             '#cols' => 30, 
             '#rows' => 2,    
		     '#element_validate' => array('new_inspection_infringement_checking_validate'),
		     '#description' =>  t('<b>infringement   text do not  contains some special character such as (~,!,@,<,>,$,#,%,*)</b>'),
	   
	  
        );				   
							   
		 $form['newinspectionfld']['submit'] = array (
		
          '#type' => 'submit',
          '#default_value' => 'Save',
		  '#attributes' => array('class' => array('btn btn-primary pull-left')),
		  '#prefix'	  => '<div class="col-md-12">',
		  '#suffix'	  =>  '</div></div></div></section></div>',	       
	   
        );
		
		$form['newinspectionfld']['infglisttwo'] = array(
		                                                '#markup'=>$jj,
	                                                    );  
	
	
      return $form;					   
			
	}
	function add_new_inspection_act_submit($form, &$form_state) {
	     global $base_root, $base_path ,$full_base , $user;
	 

           $val=$form_state['values'];
		    $typeinfrg=NULL;
		    $a=array();
			$ins_typ_query_rst	= db_query(' select inspection_txt_type  from  l_inspection_master group by inspection_txt_type order by inspection_txt_type asc ' );	
	
             foreach($ins_typ_query_rst as $data) {
					
					 $a[]=$data->inspection_txt_type;
    
				}
	
             	
		       // print_r($a);  
				//exit; 
		
	                   
	     	$b = array('A', 'B', 'C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
			'AA', 'BB', 'CC','DD','EE','FF','GG','HH','II','JJ','KK','LL','MM','NN','OO','PP','QQ','RR','SS','TT','UU','VV','WW','XX','YY','ZZ');

             $arrlength=count($b);

               for($x=0; $x<$arrlength; $x++)
                     {
                    
					
					if (!in_array($b[$x], $a))
                      {
                           
					   $typeinfrg=trim($b[$x]);
					    break;
                      }

					
				
               
		
					 }
		  
		  
		        $inspection_act_name=check_plain(decode_entities(preg_replace('/\s+/', ' ',trim($val['inspection_act_name']))));
		        
		  // print $typeinfrg;
		   //exit;
        
            $txn = db_transaction();
		
		

         try { 
 
           
		   for($a=1; $a <= 5; $a++)
                     {
		         $ins_infringement=check_plain(decode_entities(preg_replace('/\s+/', ' ',trim($val['ins_infringement_'.$a]))));
			     $maxresult=db_query('select  max(ispection_id) maxsl from l_inspection_master');	
	             $result=$maxresult->fetchAssoc();
	             $max_infringement_id = trim($result['maxsl']) + 1; 
		  
  
   $fieldsE = array('ispection_id' => $max_infringement_id, 'inspection_txt' =>$ins_infringement,'inspection_txt_type' =>$typeinfrg, 'txt_order' => $max_infringement_id, 'inspection_name' => $inspection_act_name);
		  
		   
            db_insert('l_inspection_master')->fields($fieldsE)->execute();
					 }
   
   
     $message = 'INSPECTION ACT   AND INFRINGEMENT  SUCCESSFULLY INSERT';
     }
catch (Exception $e) {
    // Something went wrong somewhere, so roll back now.
    $txn->rollback();
    // Log the exception to watchdog.
    watchdog_exception('type', $e);
	$message = 'INSPECTION ACT   AND INFRINGEMENT    NOT INSERT ';
  }
                             drupal_set_message(t($message));
				            // global $base_root, $base_path,$full_base;
					    
				             $full_base=$base_root.$base_path.'cmscontent/inspectionmasteract';
                             drupal_goto($full_base);
                            
     }  
	 
	 
	  
		?>