<?php 
drupal_add_js(drupal_get_path('module', 'alc') . '/js/mycustom.js');

function hard_copy_form($form, &$form_state,$date=''){
    global $base_root, $base_path, $user;
    $user_id=$user->uid;
    


    $insert_arr['uid']=$user->uid;
        $insert_arr['taskfor']= 'CHECKDATE';
        
        $insert_arr['source']= 'WBLC';
		$insert_arr['taskid']= 'ADDHARDCOPY';
    
     
      $service_url = "https://164.100.199.8:443/rest/wblcofficial";
				$ch = curl_init();
				$headers = array();
				$headers[] = 'Accept: application/json';
				$headers[] = 'Content-Type: application/json';
				$headers[] = 'charset=utf-8';
				curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
				curl_setopt($ch, CURLOPT_URL, $service_url);
				curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
				curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
				curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
				curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 0);
				curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($insert_arr));
				curl_setopt($ch, CURLOPT_POST, true);
				$results    = curl_exec($ch);
				$errors     = curl_error($ch); 
				//print_r($errors); 
				curl_close($ch);			
				$results_migrantcheck = json_decode(trim($results), TRUE);
                //print_r($results_migrantcheck);die;
                // if($user->uid==1094){
                //     echo '<pre>';
                //     print_r($results_migrantcheck);die;
                //    }



    $getblock = db_select('block_mun_master', 'block');
    $getblock->fields('block', array());
    $get_area_codes = db_select('l_customuser_relation_address', 'lcra');
	$get_area_codes->fields('lcra', array('district_code', 'sub_div_code', 'block_code', 'custom_area_jurisdiction', 'is_custom_area_jurisdiction', 'area_type'));
	$get_area_codes->condition('lcra.user_id', $user_id);
	$get_area_codes_result = $get_area_codes->execute()->fetchAssoc();

     $customjurisdiction = $get_area_codes_result['custom_area_jurisdiction'];
	$iscustom_juris = $get_area_codes_result['is_custom_area_jurisdiction'];

    if ($customjurisdiction == 'BMCNS' && $iscustom_juris == 'Y') {
        $block_arr = db_select('l_customuser_reletion_services_wise_user_settings', 'cra')->fields('cra', array('block_code'))->condition('cra.user_id', $user_id)->execute()->fetchObject()->block_code;
        $area_arr = explode(',', $block_arr);

        $getblock = db_select('block_mun_master', 'block');
        $getblock->fields('block', array());
        $getblock->condition('block.block_code',$area_arr,'IN');
        //echo '123';die;
    } else {
        //echo '456';die;
        $area_arr = db_select('l_customuser_relation_address', 'lcra')->fields('lcra', array('sub_div_code'))->condition('lcra.user_id', $user_id)->execute()->fetchAssoc();        
        
        $getblock = db_select('block_mun_master', 'block');
        $getblock->fields('block', array());
        $getblock->condition('block.sub_division_code',$area_arr,'IN');       
        
    }

    $getblock->orderBy('block.block_mun_name', 'ASC');
    $block_arr=$getblock->execute()->fetchAll();
   
    $date=date('Y-m-d');
    if($results_migrantcheck['content']['count_fount']==0 && $results_migrantcheck['content']['today_data_found']==0 ){
       
      $for_date=date('jS F Y', strtotime($date)); 
    }elseif($results_migrantcheck['content']['count_fount']==1 && $results_migrantcheck['content']['today_data_found']==1 && $results_migrantcheck['content']['date_increment']==0){
       // $for_date=date('jS F Y', strtotime($date)); 
       
        $form['cont_head'] = array(
            '#type' 	=> 'markup',
            '#markup' => '<div class="panel panel-primary">
                            <div class="panel-heading with-border">ADD HARD COPY FOR  </div>
                            <div class="panel-body"> DATA SUBMITTED FOR '.date('jS F Y', strtotime($results_migrantcheck['content']['updated_date'])),
        
        );
        $form['cont_foot'] = array(
            '#type' 	=> 'markup',
            '#markup' => '</div></div>'
        
        );
        return $form;

       
    }elseif($results_migrantcheck['content']['count_fount']==1 && $results_migrantcheck['content']['today_data_found']==1 && $results_migrantcheck['content']['date_increment']==1){
        $for_date=date('jS F Y', strtotime($date.'+1 day'));
    }
   

    $form['cont_head'] = array(
        '#type' 	=> 'markup',
        '#markup' => '<div class="panel panel-warning">
                        <div class="panel-heading with-border">ADD HARD COPY FOR '.$for_date.' </div>
                        <div class="panel-body">'

    );

    $form['table_head'] = array(
        '#type' 	=> 'markup',
        '#markup' => '<table class="table table-striped table-responsive admin-custom-table">'

    );
    $form['table_th1'] = array(
        '#type' 	=> 'markup',
        '#markup' => '<tr>'

    );
    $form['table_tr_up_1'] = array(
        '#type' 	=> 'markup',
        '#markup' => '<th>BLOCK NAME</th>'

    );
    $form['table_tr_up_2'] = array(
        '#type' 	=> 'markup',
        '#markup' => '<th>Forms Collected</th>'

    );
    $form['table_tr_up_3'] = array(
        '#type' 	=> 'markup',
        '#markup' => '<th>left out MW</th>'

    );
    $form['table_tr_up_4'] = array(
        '#type' 	=> 'markup',
        '#markup' => '<th>mw covered today</th>'

    );
    $form['table_tr_up_6'] = array(
        '#type' 	=> 'markup',
        '#markup' => '<th>cumulative coverage</th>'

    );
    $form['table_tr_up_5'] = array(
        '#type' 	=> 'markup',
        '#markup' => '<th>house visited</th>'

    );

    $form['table_th1_end'] = array(
        '#type' 	=> 'markup',
        '#markup' => '</tr>'

    );
    $block_leanth=count($block_arr);
    $form['block_leanth'] 	= array(
        '#type' 	=> 'hidden',
        '#value' 	=> $block_leanth,				  
    );
    
    $i=0;
    foreach($block_arr as $block){
        if($i< $block_leanth){
            $form['table_tr'.$i] = array(
                '#type' 	=> 'markup',
                '#markup' => '<tr>'
            
            );
            $newblock=$block->block_mun_name;
            $newblock_code=$block->block_code;
            $form['table_block'.$i] = array(
                '#type' 	=> 'markup',
                '#markup' => "<td><strong>".$newblock."</strong>",
            
            );
            $form['block_code'.$i] 	= array(
                '#type' 	=> 'hidden',
                '#value' 	=> $newblock_code,				  
            );
            $form['table_block123'.$i] = array(
                '#type' 	=> 'markup',
                '#markup' => "</td>",
            );
            
            $form['data'.$i] = array(
                '#prefix' => '<td>',
            
                '#suffix' => '</td>',
                '#default_value' 		=>!empty($result1['appointment_date']) ? trim($result1['appointment_date']) : 0,																				  
                //'#title' => 'Date of Appointment',
                '#attributes' => array('class' => array('form-control'),'autocomplete'=>'off'),
                '#type' => 'textfield',
                '#element_validate' => array('number_validation'),
                '#required' => TRUE,
            );
            $form['mw_left_out'.$i] = array(
                '#prefix' => '<td>',
            
                '#suffix' => '</td>',
                '#default_value' 		=>!empty($result1['appointment_date']) ? trim($result1['appointment_date']) : 0,																				  
                //'#title' => 'Date of Appointment',
                '#attributes' => array('class' => array('form-control'),'autocomplete'=>'off'),
                '#type' => 'textfield',
                '#element_validate' => array('number_validation'),
                '#required' => TRUE,
            );

            $form['mw_covered_today'.$i] = array(
                '#prefix' => '<td>',
            
                '#suffix' => '</td>',
                '#default_value' 		=>!empty($result1['appointment_date']) ? trim($result1['appointment_date']) : 0,																				  
                //'#title' => 'Date of Appointment',
                '#attributes' => array('class' => array('form-control'),'autocomplete'=>'off'),
                '#type' => 'textfield',
                '#element_validate' => array('number_validation'),
                '#required' => TRUE,
            );
            $form['cumulative_coverage'.$i] = array(
                '#prefix' => '<td>',
            
                '#suffix' => '</td>',
                '#default_value' 		=>!empty($result1['appointment_date']) ? trim($result1['appointment_date']) : 0,																				  
                //'#title' => 'Date of Appointment',
                '#attributes' => array('class' => array('form-control'),'autocomplete'=>'off'),
                '#type' => 'textfield',
                '#element_validate' => array('number_validation'),
                '#required' => TRUE,
            );
            $form['house_visited'.$i] = array(
                '#prefix' => '<td>',
            
                '#suffix' => '</td>',
                '#default_value' 		=>!empty($result1['appointment_date']) ? trim($result1['appointment_date']) : 0,																				  
                //'#title' => 'Date of Appointment',
                '#attributes' => array('class' => array('form-control'),'autocomplete'=>'off'),
                '#type' => 'textfield',
                '#element_validate' => array('number_validation'),
                '#required' => TRUE,
            );
            
            $form['table_tr_end'.$i] = array(
                '#type' 	=> 'markup',
                '#markup' => '</tr>'
            
            );
            $i++;
        }
    }


    $form['submit'] = array (
        '#prefix' => '</table>',  
        '#suffix' => '',  
        '#type' 		 => 'submit',
        '#attributes' => array('class' => array('btn btn-primary pull-left'),'onClick'=>'disableSubmit_app()','id'=>'submit_hrd'),
        '#value' 		 => 'ADD'
    );


    $form['cont_foot'] = array(
        '#type' 	=> 'markup',
        '#markup' => '</div></div>'

    );
    return $form;
}
function hard_copy_form_submit($form, &$form_state){
    global $base_root, $base_path, $user;
          $insert_arr=array();


          $fetch_users_role		= db_select('users_roles', 'ur');
						$fetch_users_role->leftJoin('role', 'ro', 'ro.rid=ur.rid');
						$fetch_users_role->leftJoin('l_custom_user_detail', 'lcud', 'ur.uid=lcud.usr_id');
						$fetch_users_role->fields('ro', array('rid'));
						$fetch_users_role->fields('lcud', array('fullname','employee_id'));
						$fetch_users_role->condition('ur.uid', $user->uid);
						$fetch_users_role_data 	= $fetch_users_role->execute()->fetchAssoc();
                        $remark_by_role			=	$fetch_users_role_data['rid'];
						$alc_full_name			=   $fetch_users_role_data['fullname'];
						$hrms_employee_id 		= 	$fetch_users_role_data['employee_id'];
					
                        



				$insert_arr['source']				= 'WBLC';
				$insert_arr['taskid']				= 'ADDHARDCOPY';
                $insert_arr['taskfor']				= 'ADD';
				$insert_arr['userid']				= $user->uid;
                $insert_arr['drive_code']				    = 'SD';
                $insert_arr['rid']				    = $remark_by_role;
                $insert_arr['createdby_hrmsid']	    = $hrms_employee_id;
                $insert_arr['createdby_fullname']   = $alc_full_name;

                $main_data=array();
                for($i= 0; $i<$form_state['values']['block_leanth'];$i++){
                    $addinfo = db_query("SELECT sub_division_code,distict_code FROM block_mun_master WHERE is_active ='Y' AND block_code=".$form_state['values']['block_code'.$i])->fetchAssoc();
                    $main_data[$i]['block_code']=$form_state['values']['block_code'.$i];
                    $main_data[$i]['dist_code']= $addinfo['distict_code'];
                    $main_data[$i]['subdiv_code']= $addinfo['sub_division_code'];
                    $main_data[$i]['data']=$form_state['values']['data'.$i];
                    $main_data[$i]['mw_left_out']=$form_state['values']['mw_left_out'.$i];
                    $main_data[$i]['mw_covered_today']=$form_state['values']['mw_covered_today'.$i];
                    $main_data[$i]['cumulative_coverage']=$form_state['values']['cumulative_coverage'.$i];
                    $main_data[$i]['house_visited']=$form_state['values']['house_visited'.$i];
                    
                    
                }
                $insert_arr['main_data']=$main_data;
              


				$service_url = "https://164.100.199.8:443/rest/wblcofficial";
				$ch = curl_init();
				$headers = array();
				$headers[] = 'Accept: application/json';
				$headers[] = 'Content-Type: application/json';
				$headers[] = 'charset=utf-8';
				curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
				curl_setopt($ch, CURLOPT_URL, $service_url);
				curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
				curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
				curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
				curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 0);
				curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($insert_arr));
				curl_setopt($ch, CURLOPT_POST, true);
				$results    = curl_exec($ch);
				$errors     = curl_error($ch); 
				//print_r($errors); 
				curl_close($ch);			
				$results_migrantcheck = json_decode(trim($results), TRUE);
                //print_r($results_migrantcheck);die;
				
				/*** End  */
                drupal_set_message('DATA ADDED SUCCESSFULLY');
                drupal_goto('hard-copy-list');
                drupal_set_message('DATA ADDED SUCCESSFULLY');
               


			

}
function hard_copy_list($form, &$form_state){

        global $user,$base_path,$base_root;
    
        
   
        $insert_arr['uid']=$user->uid;
        $insert_arr['taskfor']= 'DATELIST';
        $insert_arr['source']= 'WBLC';
		$insert_arr['taskid']= 'ADDHARDCOPY';
    
     
      $service_url = "https://164.100.199.8:443/rest/wblcofficial";
				$ch = curl_init();
				$headers = array();
				$headers[] = 'Accept: application/json';
				$headers[] = 'Content-Type: application/json';
				$headers[] = 'charset=utf-8';
				curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
				curl_setopt($ch, CURLOPT_URL, $service_url);
				curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
				curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
				curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
				curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 0);
				curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($insert_arr));
				curl_setopt($ch, CURLOPT_POST, true);
				$results    = curl_exec($ch);
				$errors     = curl_error($ch); 
				//print_r($errors); 
				curl_close($ch);			
				$results_migrantcheck = json_decode(trim($results), TRUE);
                //print_r($results_migrantcheck);die;
               
				/*** End  */
                $header = array(
                    array('data' => 'Sl. No.', 'width' => '5%'),
                    array('data' => 'DATE TIME', 'width' => '20%'),
                    array('data' => 'TOTAL Forms Collected ', 'width' => '15%'),
                    array('data' => 'TOTAL  left out MW', 'width' => '15%'),
                    array('data' => 'TOTAL mw covered today', 'width' => '15%'),
                    array('data' => 'TOTAL cumulative coverage', 'width' => '15%'),
                    array('data' => 'TOTAL house visited', 'width' => '15%'),
                    array('data' => 'ACTION', 'width' => '15%'),
                );
                $i=0;
                foreach($results_migrantcheck['content'] as $data){
                    $i++;
                    $report_date=date('Y-m-d H:i:s', strtotime($data['report_date']));
                    $edit_link=$base_root . $base_path . 'update-hard-copy/' . encryption_decryption_fun('encrypt',$report_date);
                    $edit= l(t('<span class="badge bg-purple">ACTION</span>'), $edit_link, array('html' => TRUE));
                $rows[] = array(
                    $i,
                    date('jS F Y g:i a', strtotime($data['report_date'])),
                    //$data['updated_date'],
                    $data['sum'],
                    $data['mw_left_out'],
                    $data['mw_covered_today'],
                    $data['cumulative_coverage'],
                    $data['house_visited'],
                    $edit,
                  
              
              
                  );
                }
    
     
        
      
    
    
      $variables = array(
        'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')),
        'header' => $header,
        'rows' => $rows,
        'empty' => t("No data found!")
      );
    
      $output =theme('datatable', $variables);
      $form['list_start'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="panel panel-warning">
                        <div class="panel-heading with-border">HARD COPY LIST</div>
                        <div class="panel-body">'
    
    
    );
    $form['list'] = array(
        '#type' => 'markup',
        '#markup' => $output
    );
      $form['list_end'] = array(
        '#type' => 'markup',
        '#markup' => '</div></div>'
    );
    return $form;
    
    
    }
    function update_hard_copy_form($form, &$form_state,$date=''){
        global $base_root, $base_path, $user;
        if(!empty($date)){
            $date = encryption_decryption_fun('decrypt', $date);

            $insert_arr['uid']=$user->uid;
            $insert_arr['taskfor']= 'GETDATA';
            $insert_arr['getdate']= $date;
            $insert_arr['source']= 'WBLC';

            $insert_arr['taskid']= 'ADDHARDCOPY';
        
         
          $service_url = "https://164.100.199.8:443/rest/wblcofficial";
                    $ch = curl_init();
                    $headers = array();
                    $headers[] = 'Accept: application/json';
                    $headers[] = 'Content-Type: application/json';
                    $headers[] = 'charset=utf-8';
                    curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
                    curl_setopt($ch, CURLOPT_URL, $service_url);
                    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
                    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
                    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
                    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 0);
                    curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($insert_arr));
                    curl_setopt($ch, CURLOPT_POST, true);
                    $results    = curl_exec($ch);
                    $errors     = curl_error($ch); 
                    //print_r($errors); 
                    curl_close($ch);			
                    $results_migrantcheck = json_decode(trim($results), TRUE);
                    //echo '<pre>';
                    //print_r($results_migrantcheck);die;
                   
                    if( date('Y-m-d H:i:s') > date('Y-m-d 18:00:00',strtotime($date)) ){
                        $form['fileldset'] = array(
                            '#type' 	=> 'markup',
                            '#markup' => '<fieldset disabled>'
                        
                        ); 
                    }
                    $form['cont_head'] = array(
                        '#type' 	=> 'markup',
                        '#markup' => '<div class="panel panel-warning">
                                        <div class="panel-heading with-border">UPDATE HARD COPY FOR '.date('jS F Y g:i a', strtotime($date)).' </div>
                                        <div class="panel-body">'
                    
                    );
                    
                    $form['table_head'] = array(
                        '#type' 	=> 'markup',
                        '#markup' => '<table class="table table-striped table-responsive admin-custom-table">'
                    
                    );
                    $form['table_th1'] = array(
                        '#type' 	=> 'markup',
                        '#markup' => '<tr>'
                    
                    );
                    $form['table_tr_up_1'] = array(
                        '#type' 	=> 'markup',
                        '#markup' => '<th>BLOCK NAME</th>'
                    
                    );
                    $form['table_tr_up_2'] = array(
                        '#type' 	=> 'markup',
                        '#markup' => '<th>Forms Collected </th>'
                    
                    );
                    $form['table_tr_up_3'] = array(
                        '#type' 	=> 'markup',
                        '#markup' => '<th>left out mw</th>'
                    
                    );
                    $form['table_tr_up_4'] = array(
                        '#type' 	=> 'markup',
                        '#markup' => '<th>mw covered today</th>'
                    
                    );
                    $form['table_tr_up_6'] = array(
                        '#type' 	=> 'markup',
                        '#markup' => '<th>cumulative coverage</th>'
                    
                    );
                    $form['table_tr_up_5'] = array(
                        '#type' 	=> 'markup',
                        '#markup' => '<th>house visited</th>'
                    
                    );
                    
                    $form['table_th1_end'] = array(
                        '#type' 	=> 'markup',
                        '#markup' => '</tr>'
                    
                    );
                    $block_leanth=count($results_migrantcheck['content']);
                    $form['block_leanth'] 	= array(
                        '#type' 	=> 'hidden',
                        '#value' 	=> $block_leanth,				  
                      );
                      $i=0;
                    foreach($results_migrantcheck['content'] as $block){
                      if($i< $block_leanth){
                        
                        $form['table_tr'.$i] = array(
                            '#type' 	=> 'markup',
                            '#markup' => '<tr>'
                        
                        );
                        //$tblock = db_query("SELECT block_mun_name,block_code FROM block_mun_master WHERE  block_code=".$block['block_mun'])->fetchAssoc();
                        
                        // $new_block=$tblock['block_mun_name'];
                        $form['table_block'.$i] = array(
                            '#type' 	=> 'markup',
                            '#markup' => "<td><strong>".$block['block_mun_name']."</strong>",
                        
                        );
                        
                        $form['block_code'.$i] 	= array(
                            '#type' 	=> 'hidden',
                            '#value' 	=> $block['block_mun'],				  
                          );
                          $form['table_block123'.$i] = array(
                            '#type' 	=> 'markup',
                            '#markup' => "</td>",
                          );
                        
                        $form['data'.$i] = array(
                            '#prefix' => '<td>',
                          
                            '#suffix' => '</td>',
                            '#default_value' 		=>$block['hardcopy_no'],																				  
                            //'#title' => 'Date of Appointment',
                            '#attributes' => array('class' => array('form-control'),'autocomplete'=>'off'),
                            '#type' => 'textfield',
                            '#element_validate' => array('number_validation'),
                            '#required' => TRUE,
                          );
                          $form['mw_left_out'.$i] = array(
                            '#prefix' => '<td>',
                          
                            '#suffix' => '</td>',
                            '#default_value' 		=>$block['mw_left_out'],																				  
                            //'#title' => 'Date of Appointment',
                            '#attributes' => array('class' => array('form-control'),'autocomplete'=>'off'),
                            '#type' => 'textfield',
                            '#element_validate' => array('number_validation'),
                            '#required' => TRUE,
                          );
                    
                          $form['mw_covered_today'.$i] = array(
                            '#prefix' => '<td>',
                          
                            '#suffix' => '</td>',
                            '#default_value' 		=>$block['mw_covered_today'],																				  
                            //'#title' => 'Date of Appointment',
                            '#attributes' => array('class' => array('form-control'),'autocomplete'=>'off'),
                            '#type' => 'textfield',
                            '#element_validate' => array('number_validation'),
                            '#required' => TRUE,
                          );
                          $form['cumulative_coverage'.$i] = array(
                            '#prefix' => '<td>',
                          
                            '#suffix' => '</td>',
                            '#default_value' 		=>$block['cumulative_coverage'],																				  
                            //'#title' => 'Date of Appointment',
                            '#attributes' => array('class' => array('form-control'),'autocomplete'=>'off'),
                            '#type' => 'textfield',
                            '#element_validate' => array('number_validation'),
                            '#required' => TRUE,
                          );
                          $form['house_visited'.$i] = array(
                            '#prefix' => '<td>',
                          
                            '#suffix' => '</td>',
                            '#default_value' 		=>$block['house_visited'],																				  
                            //'#title' => 'Date of Appointment',
                            '#attributes' => array('class' => array('form-control'),'autocomplete'=>'off'),
                            '#type' => 'textfield',
                            '#element_validate' => array('number_validation'),
                            '#required' => TRUE,
                          );
                          $form['id'.$i] 	= array(
                            '#type' 	=> 'hidden',
                            '#value' 	=> $block['id'],				  
                          );
                          $form['getdate'.$i] 	= array(
                            '#type' 	=> 'hidden',
                            '#value' 	=> $date,				  
                          );
                          
                        $form['table_tr_end'.$i] = array(
                            '#type' 	=> 'markup',
                            '#markup' => '</tr>'
                        
                        );
                        $i++;
                    }
                    
                    
                    }
                    
                    
                    $form['submit'] = array (
                        '#prefix' => '</table>',  
                        '#suffix' => '',  
                        '#type' 		 => 'submit',
                        '#attributes' => array('class' => array('btn btn-primary pull-left')),
                        '#value' 		 => 'UPDATE'
                    );
                    
                    
                    
                    $form['cont_foot'] = array(
                        '#type' 	=> 'markup',
                        '#markup' => '</div></div>'
                    
                    );
                    if( date('Y-m-d H:i:s') > date('Y-m-d 18:00:00',strtotime($date)) ){
                        $form['fileldset_end'] = array(
                            '#type' 	=> 'markup',
                            '#markup' => '</fieldset>'
                        
                        ); 
                    }
                    return $form;

        }else{
            drupal_set_message('NO DATA FOUND','error');
        }
    }
    function update_hard_copy_form_submit($form, &$form_state){
        global $base_root, $base_path, $user;
    $insert_arr['source']				= 'WBLC';
    $insert_arr['taskid']				= 'ADDHARDCOPY';
    $insert_arr['taskfor']				= 'UPDATE';
    $insert_arr['userid']				= $user->uid;
    $insert_arr['drive_code']				    = 'SD';
   

    $main_data=array();
    for($i= 0; $i<$form_state['values']['block_leanth'];$i++){
        $addinfo = db_query("SELECT sub_division_code,distict_code FROM block_mun_master WHERE is_active ='Y' AND block_code=".$form_state['values']['block_code'.$i])->fetchAssoc();
        $main_data[$i]['block_code']=$form_state['values']['block_code'.$i];
        $main_data[$i]['dist_code']= $addinfo['distict_code'];
        $main_data[$i]['subdiv_code']= $addinfo['sub_division_code'];
        $main_data[$i]['data']=$form_state['values']['data'.$i];
        $main_data[$i]['mw_left_out']=$form_state['values']['mw_left_out'.$i];
        $main_data[$i]['mw_covered_today']=$form_state['values']['mw_covered_today'.$i];
        $main_data[$i]['cumulative_coverage']=$form_state['values']['cumulative_coverage'.$i];
        $main_data[$i]['house_visited']=$form_state['values']['house_visited'.$i];
        $main_data[$i]['id']=$form_state['values']['id'.$i];
        $main_data[$i]['getdate']=$form_state['values']['getdate'.$i];
        
        
    }
    $insert_arr['main_data']=$main_data;
  


    $service_url = "https://164.100.199.8:443/rest/wblcofficial";
    $ch = curl_init();
    $headers = array();
    $headers[] = 'Accept: application/json';
    $headers[] = 'Content-Type: application/json';
    $headers[] = 'charset=utf-8';
    curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
    curl_setopt($ch, CURLOPT_URL, $service_url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 0);
    curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($insert_arr));
    curl_setopt($ch, CURLOPT_POST, true);
    $results    = curl_exec($ch);
    $errors     = curl_error($ch); 
    //print_r($errors); 
    curl_close($ch);			
    $results_migrantcheck = json_decode(trim($results), TRUE);
    
    if($results_migrantcheck['content']==1){
        drupal_set_message('DATA UPDATE SUCCESSFULLY');
        drupal_goto('hard-copy-list');
        drupal_set_message('DATA UPDATE SUCCESSFULLY');

    }elseif($results_migrantcheck['content']==0){
        drupal_set_message('YOU CAN NOT UPDATE THE DATA','error');
        drupal_goto('hard-copy-list');
        drupal_set_message('YOU CAN NOT UPDATE THE DATA','error');
    }
   
    /*** End  */
   
    }