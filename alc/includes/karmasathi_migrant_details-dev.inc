<?php 
drupal_add_js(drupal_get_path('module', 'alc') . '/js/mycustom.js');
drupal_add_js(drupal_get_path('module', 'alc') . '/js/duaresarkar.js');
drupal_add_css(drupal_get_path('module', 'addlc') . '/css/style.css');

function karmasathi_migrant_details($form, &$form_state, $app_id_en=''){

  global $base_root, $base_path, $user;

	$user_id 			= $user->uid;
  $id = encryption_decryption_fun('decrypt', $app_id_en);
  //echo $id;die;
	$get_area_details		=	db_select('l_customuser_relation_address', 'lcra');
	$get_area_details		->  leftJoin('users_roles', 'ur','ur.uid=lcra.user_id ');
	$get_area_details		->  leftJoin('role', 'ro', 'ro.rid = ur.rid');
	$get_area_details       ->leftJoin('l_custom_user_detail', 'lcud', 'ur.uid=lcud.usr_id');
	$get_area_details		->  fields('ro', array('name','rid'));
	$get_area_details		->	fields('lcra',array('district_code','sub_div_code','block_code','custom_area_jurisdiction','is_custom_area_jurisdiction','user_id'));
	
	$get_area_details       ->fields('lcud', array('fullname','employee_id'));
	$get_area_details		->	condition('lcra.user_id',$user_id);
	$get_area_details_result = 	$get_area_details->execute();

	if($get_area_details_result-> rowCount() > 0 ){
		//print_r($get_area_details_result->fetchAssoc());die;
		$officer_data 					= $get_area_details_result->fetchAssoc();  
		
		$user_role						= $officer_data['rid']; 
	}

	

  
  $curl_post_data = array (
		'source' 		=> 'WBLC',
		'taskid' 		=> 'VIEWAPPLICATION',
		'id' 	      => $id,						
	);
	
		
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
		curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($curl_post_data));
		curl_setopt($ch, CURLOPT_POST, true);
		$results    = curl_exec($ch);
		$errors     = curl_error($ch); 
		 //print_r($errors); die;
		curl_close($ch);			
		$results = json_decode(trim($results), TRUE); //echo '<pre>'; print_r($results['content']['flag']); die;
    $get_worksite_error=0;
    $error_message_worksite='';
     if(count(($results['content']['workDetailsArray']))>0){
    
        foreach($results['content']['workDetailsArray'] as $work){


          if($work['is_active']==1){


            if($work['present_country']==1){
                if($work['present_state'] ==0 || $work['present_state'] == '' || empty($work['present_state']) || empty($work['present_address'])){

                  $get_worksite_error=1;
                  $error_message_worksite='<span style="color : red"> WORKSITE DETIALS NOT VALID.</span>'; 
                }
            }else{//present_country
              if(empty($work['present_address'])){
                $get_worksite_error=1;
                $error_message_worksite='<span style="color : red"> WORKSITE DETIALS NOT VALID.</span>';
              }
            } 

          }
         
        }
      
     
    }else{//count
      $get_worksite_error=1;
      $error_message_worksite='<span style="color : red"> WORKSITE DETIALS NOT VALID.</span>';
    }

  

  if($results['status_code'] == 200){
    
    $results_aadhaar = $results['content']['checkApprovedAadhar'];
    
    
      if(!empty($results_aadhaar)){

        $approved_data = 'approved';
        $approved_mwin = $results_aadhaar['content']['identification_number'];
        $approved_name = $results_aadhaar['content']['name'];
        $approved_mobile = $results_aadhaar['content']['mobile'];
      }else{
        $approved_data = '';
        $approved_mwin = '';
        $approved_name = '';
        $approved_mobile = '';
      }
       $age=(date_diff(date_create($results['content']['dob']), date_create('today'))->y)*1;
      // echo gettype($age);die;
      $data_error=0;
      $error_message='';
     
      if($approved_data == 'approved' || $approved_data !=''){
          
        $data_error=1;
      }

      $attribute = array('class' => array('form-control'));
      if(($age>=18) && (empty($results['content']['epic_no']))){
        if($results['content']['flag']=='DS-SEP2023'){
          $data_error=0;
        }else{
          $data_error=1;
        }
        
         $attribute = array('class' => array('form-control'),'onChange'=>'CheckEpic()');
         $error_message='<tr>
                        <td style="color : red; font-size:16px" colspan="3"> *** EPIC Number not provided (Need to rectify by the applicant).</td>
                      </tr>';
      }
      // $khadyasathiArray = array('NA```','NA`','NA ','NAA','Na','na','nai','NA');

      //if(empty($results['content']['khadyasathi_no']) || $results['content']['khadyasathi_no'] =='NA')

    $identification_number = $results['content']['identification_number'];
    $mobile = $results['content']['mobile'];
    $applicant_status = $results['content']['status'];
    $name   = $results['content']['name'];
    $father_husband_name   = $results['content']['father_husband_name'];
    $gender   = $results['content']['gender'];
  

    if($results['content']['status'] == 'A'){
      $current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-warning"></i> CURRENT STATUS: Pending</h4><p>Application is Pending.</p></div>';
    }elseif($results['content']['status'] == 'C'){
        $current_status = '<div class="alert alert-success alert-dismissable"><h4><i class="icon fa fa-success"></i> CURRENT STATUS: Approved</h4><p>Application is Approved.</p></div>';
    }elseif($results['content']['status'] == 'B'){
      $current_status = '<div class="alert alert-warning alert-dismissable"><h4><i class="icon fa fa-success"></i> CURRENT STATUS: Back for Rectification</h4><p>Application has Send Back for Rectification.</p></div>';
    }elseif($results['content']['status'] == 'R'){
      $current_status = '<div class="alert alert-danger alert-dismissable"><h4><i class="icon fa fa-success"></i> CURRENT STATUS: Rejected</h4><p>Application is Rejected.</p></div>';
    }elseif($results['content']['status'] == 'I' || $results['content']['status'] == 'P'){
      $current_status = '<div class="alert alert-info alert-dismissable"><h4><i class="icon fa fa-success"></i> CURRENT STATUS: Incomplete</h4><p>Application is Incomplete.</p></div>';
    }

    $form['migrant']['id']		= array(
      '#type'					=> 'hidden',
      '#value' 				=>  encryption_decryption_fun('encrypt',$id),
    );

    $form['migrant']['identification_number']		= array(
      '#type'					=> 'hidden',
      '#value' 				=>  encryption_decryption_fun('encrypt',$results['content']['identification_number']),
    );
   
    
    $form['migrant']['markup_data']		= array(
      '#markup' 		=> view_migrant_application($results,$results_aadhaar,$error_message,$error_message_worksite),
      '#type' 		=> 'markup',
      );
                
    $form['migrant']['show_status'] 		= array(
      '#type' 		=> 'markup',
      '#markup'		=> $current_status,
      );

      
    
    if($user_role == 7){
      if($applicant_status == 'A' || $applicant_status == 'BA'){
        $form['migrant']['action_start'] 		= array(
          '#type' 		=> 'markup',
          '#markup'		=> '<div class="box box-primary box-solid"><div class="box-header with-border">
                    <h3 class="box-title">Action Against This Application</h3></div>
                    <div class="box-body">',
        );
        

        
        if($data_error==1||$get_worksite_error==1){
      
          $action_option = array(
            '' => '- Select -',
            'R' => 'Reject',
            'B' => 'Back for Rectification'	
          );
        }else{
          $action_option = array(
            '' => '- Select -',
            'C' => 'Approved',
            'R' => 'Reject',
            'B' => 'Back for Rectification'	
          );
        }
        
        $form['migrant']['action_taken']		= array(
          '#prefix' 			=> '<div class="row"><div class="form-custom col-md-6"><label class="input">',
          '#suffix' 			=> '</label></div>',
          '#title'			=> 'Action',
          '#type'				=> 'select',
          '#options'			=> $action_option,
          '#required' 			=> TRUE,
          '#attributes'		=> $attribute,
          '#ajax' => array(
            'event' => 'change',
            'effect' => 'fade',			
            'callback' => 'action_taken_callback',
            'progress' 			=> array(
              'type'    => 'throbber',
              'message' => 'Please wait. It will take few time',
              ),
            ),
        );	
        
        $action_taken = isset($form_state['values']['action_taken']) ? $form_state['values']['action_taken'] : '';
        if($action_taken == 'R' || $action_taken == 'B'){
          $form['migrant']['remark'] = array(
            '#prefix' 			=> '<div class="form-custom col-md-6" id="remark_replace"><label class="input">',
            '#suffix' 			=> '</label></div>',
            '#title'				=> t('Enter Remark'),
            '#type' 				=> 'textarea',
            '#rows'					=>  3,
            '#required' 		=> TRUE,
            '#attributes'		=> array('class' => array('form-control'),'autocomplete' => 'off'),
          );
          
        }elseif ($action_taken == 'C') {
          $form['migrant']['remark'] = array(
            '#prefix' 			=> '<div class="form-custom col-md-6" id="remark_replace"><label class="input">',
            '#suffix' 			=> '</label></div>',
            '#title'				=> t('Enter Remark'),
            '#type' 				=> 'textarea',
            '#rows'					=>  3,
            '#required' 		=> FALSE,
            '#attributes'		=> array('class' => array('form-control'),'autocomplete' => 'off'),
          );
        }else{
          $form['migrant']['remark'] = array(
            '#prefix' 			=> '<div id="remark_replace">',
            '#suffix' 			=> '</div>',
          );
          
        }
        
        
        $form['migrant']['submit'] 	= array (
          '#prefix' 			=> '</div><div class="row"><div class="col-md-6">',
          '#suffix' 			=> '</div></div>',
          '#type' 			=> 'submit',
          '#attributes' => array('class' => array('btn btn-primary pull-left acremark-btn'),'onClick'=>'disableSubmitApp()','id'=>'submit_block'),
          '#value' 			=> 'Submit'
        );	
       	
        // $form['migrant']['back_link']	 = array(
        //   '#prefix' 		=> '<div class="col-md-2">',
        //   '#suffix' 		=> '</div></div>', 
        //   '#markup' 		=> l(t('Back'),'official-karmasathi-list/' . $role_name . '/pending', array('html' => TRUE,'attributes'=> array('class'=>array('btn btn-info pull-left')))),															
        // );
        $form['migrant']['action_end']		= array(
          '#type' 				=> 'markup',
          '#markup'				=> '</div></div>'	
        );

        
      }else{

      }
    }
    
    $form['migrant']['show_remark_details'] 	= array(
      '#prefix'			=>'<div class="box box-default box-solid"><div class="box-header with-border"><i class="ion ion-clipboard"></i><h3 class="box-title">REMARK SUMMARY</h3></div><div class="box-body">',
      '#type' 			=> 'markup',
      '#markup'			=> show_remark_details($results['content']['remarkDetailsArray']),
      '#suffix' 			=> '</div></div>',
    );	
    			
    return $form;
  }
  
}
function karmasathi_migrant_details_submit($form, &$form_state){
	
	$form_values = $form_state['values'];
 
  
  global $base_root, $base_path, $user;

	$user_id 			= $user->uid;


  $get_area_details		=	db_select('l_customuser_relation_address', 'lcra');
	$get_area_details		->  leftJoin('users_roles', 'ur','ur.uid=lcra.user_id ');
	$get_area_details		->  leftJoin('role', 'ro', 'ro.rid = ur.rid');
	$get_area_details       ->leftJoin('l_custom_user_detail', 'lcud', 'ur.uid=lcud.usr_id');
	$get_area_details		->  fields('ro', array('name','rid'));
	$get_area_details		->	fields('lcra',array('district_code','sub_div_code','block_code','custom_area_jurisdiction','is_custom_area_jurisdiction','user_id'));
	
	$get_area_details       ->fields('lcud', array('fullname','employee_id'));
	$get_area_details		->	condition('lcra.user_id',$user_id);
	$get_area_details_result = 	$get_area_details->execute();

	if($get_area_details_result-> rowCount() > 0 ){
		$officer_data 					= $get_area_details_result->fetchAssoc();  
		
		$user_role						= $officer_data['rid']; 
    $role_name						= $officer_data['name']; 
		$user_role_id					= $officer_data['user_id']; 
		$full_name					  = $officer_data['fullname'];
		$hrms_employee_id 		= $officer_data['employee_id'];
	}

  $identification_number 	= encryption_decryption_fun('decrypt',$form_values['identification_number']);
  $id 	= encryption_decryption_fun('decrypt',$form_values['id']);
  
  $insarr = array(
    'source' 		=> 'WBLC',
    'taskid' 		=> 'UPDATESTATUS',
    'id' => $id,
    'identification_number' => $identification_number,
    'status' => $form_values['action_taken'],
    'remark' => $form_values['remark'],
    
    'user_id' => $user_role_id,
    'user_role' => $user_role,
    'remark_by_employee_id' => $hrms_employee_id,
    'action_to' => 0,
    'action_to_role' => 0,
    'action_to_hrms_id' => 0,
    'is_active' => 1,

  );

  //echo '<pre>'; print_r($insarr); die;
 
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
		curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($insarr));
		curl_setopt($ch, CURLOPT_POST, true);
		$results    = curl_exec($ch);
		$errors     = curl_error($ch); 
		// print_r($errors); die;
		curl_close($ch);			
		$results = json_decode(trim($results), TRUE);  //echo '<pre>'; print_r($results); die;

  if($form_values['action_taken'] == 'C'){
    $par = 'approved';
  }elseif ($form_values['action_taken'] == 'R') {
    $par = 'reject';
  }elseif ($form_values['action_taken'] == 'B') {
    $par = 'back';
  }

  
  if($results['status_code'] == 200){
    drupal_goto('official-karmasathi-list/'.$user_role. '/'.$par);
  }else{
    drupal_set_message(t('Unable to take action. Please try later.'), 'error');
    drupal_goto('karmasathi-migrant-details/'.$form_values['id']);
  }
  
}

function view_migrant_application($results,$results_aadhaar,$error_message='',$error_message_worksite=''){

  global $base_root, $base_path, $user;
  

  $fieldArr		        = array('permanent_dist','permanent_subdivision','permanent_areatype','permanent_areacode','permanent_villward','permanent_ps','permanent_pin');
  $address_result     = array($results['content']['permanent_dist'],$results['content']['permanent_subdivision'],$results['content']['permanent_areatype'],$results['content']['permanent_areacode'],$results['content']['permanent_villward'],$results['content']['permanent_ps'],$results['content']['permanent_pin'],1);
  $permanent_address 	= get_full_address_migrant($fieldArr,$address_result);
  
    if($results_aadhaar['status_code'] == 200){
      
      $approved_mwin = $results_aadhaar['content']['identification_number'];
      $approved_name = $results_aadhaar['content']['name'];
      $approved_mobile = $results_aadhaar['content']['mobile'];
      
      $approved_data = '<tr>
                        <td style="color : red; font-size:15px" colspan="4">This aadhaar number is already approved with Applicant name - '.$approved_name.' and  MWIN - '.$approved_mwin.' and Mobile - '.$approved_mobile.'</td>
                      </tr>';
    }else{
      $approved_data = '';
      $approved_mwin = '';
      $approved_name = '';
      $approved_mobile = '';
    }

      if(!empty($results['content']['aadhar_no'])){
        
        // $aadhaar_no = str_repeat('X',strlen(substr($results['content']['aadhar_no'], 0, 8))).substr($results['content']['aadhar_no'],-4);
        $aadhaar_no = $results['content']['aadhar_no'];
      }else{
        $aadhaar_no = 'Not Available';
      }

      if(!empty($results['content']['ssin_no'])){
        $ssin_no = $results['content']['ssin_no'];
      }else{
        $ssin_no = 'Not Available';
      }

      if(!empty($results['content']['pf'])){
        $pf = $results['content']['pf'];
      }else{
        $pf = 'Not Available';
      }

    if(!empty($results['content']['esic'])){
        $esic = $results['content']['esic'];
      }else{
        $esic = 'Not Available';
      }

      if(!empty($results['content']['bank_account'])){
      
        $bank_account = $results['content']['bank_account'];
        $bank_name = $results['content']['bank_name'];
        $ifsc_code = $results['content']['ifsc_code'];
        $bank_branch = $results['content']['bank_branch'];
      }else{
        $bank_account = 'Not Available';
        $bank_name = 'Not Available';
        $ifsc_code = 'Not Available';
        $bank_branch = $results['content']['bank_branch'];
      }

      if(!empty($results['content']['khadyasathi_no'])){
        $khadyasathi_no = $results['content']['khadyasathi_no'].' ( '.$results['content']['khadyasathi_type'].' )';
      }else{
        $khadyasathi_no = 'Not Available';
      }
      $count = 1;	
      if(!empty($results['content']['relationship'])){
        $relationship = ' ['.$results['content']['relationship'].']';
      }else{
        $relationship = ' ';
      }
      $age=(date_diff(date_create($results['content']['dob']), date_create('today'))->y)*1;
      if(($age>=18) && (empty($results['content']['epic_no']))){
      
        $data_error=1;
        // $error_message = '<span style="color : red">Epic is mandatory.</span>';
         $epic_no='<td style="color : red; font-size:16px" colspan="3"> <strong> *** EPIC Number not provided (Need to rectify by the applicant) </strong>.</td>';
      }elseif (!empty($results['content']['epic_no']) || $results['content']['epic_no'] !='') {
        $epic_no='<td>'.$results['content']['epic_no'].'</td>';

      }

      if ($results['content']['gender'] == 'M'){
        $gender = 'Male';
      }else if($results['content']['gender'] == 'F'){
          $gender = 'Female';
      }else if($results['content']['gender'] =='O'){
        $gender = 'Other';
      }
      
      if ($results['content']['caste'] == 'UR' || $results['content']['caste'] == 'ur'){
        $caste = 'UR';
      }else if($results['content']['caste'] == 'SC' || $results['content']['caste'] == 'sc'){
          $caste = 'SC';
      }else if($results['content']['caste'] == 'ST' || $results['content']['caste'] == 'st'){
        $caste = 'ST';
      }else if($results['content']['caste'] == 'OBC' || $results['content']['caste'] == 'obc'){
        $caste = 'OBC';
      }

      if ($results['content']['qualification'] == 'UM'){
        $qualification = 'Under Matric (8th Pass)';
      }else if($results['content']['qualification'] == 'SS'){
          $qualification = 'Matriculation/ 10th Pass';
      }else if($results['content']['qualification'] == 'HS'){
        $qualification = 'Higher Secondary';
      }else if($results['content']['qualification'] == 'PG'){
        $qualification = 'Post Graduation';
      }else if($results['content']['qualification'] == 'G'){
        $qualification = 'Graduation';
      }

      
      if(!empty($results['content']['qualification']) || $results['content']['qualification'] !=''){
        $qualification_data = '<tr>
                                <td>'.$count++.' .</td>
                                <td>Qualification</td>
                                <td>'.$qualification.'</td>
                              </tr>';
      }else{
        $qualification_data = '';
      }

      if(!empty($results['content']['technical_skill']) || $results['content']['technical_skill'] !=''){
        $technical_skill = '<tr>
                                <td>'.$count++.' .</td>
                                <td>Technical Skill</td>
                                <td>'.$results['content']['technical_skill'].'</td>
                              </tr>';
      }else{
        $technical_skill = '';
      }


      if(!empty($results['content']['sasthyasathi_no']) || $results['content']['sasthyasathi_no'] !=''){
        $sasthyasathi_no = $results['content']['sasthyasathi_no'];
      }else{
        $sasthyasathi_no = 'Not Available';
      }
      


     
     if($results['content']['flag']=='DS-SEP2023'){
      $ds='( DS-7 )';
     }elseif ($results['content']['flag']=='DS-DEC2023') {
      $ds='( DS-8 )';
     }else{
      $ds='';
     }
     
     if($results['content']['ds_found']=='yes' && !empty($results['content']['ds_id'])){
      $dsdata= '<tr><td style="text-align : center"><strong>Duare sarkar id : '.$results['content']['ds_id'].$ds.'</strong><br>';
    }else{
      $dsdata= '<tr><td style="text-align : center">';
    }

    if(!empty($results['content']['mwin']) || $results['content']['mwin'] != ''){
      $mwin = $results['content']['mwin'];
    }else{
      $mwin = 'Not Generated Yet';
    }


      
     if(!empty($results['content']['applicantScheme'])) {
      $migrantSchemes = '<a href="#" onClick="modal_schemes('.$results['content']['id'].')" class="small-box-footer" ><span class="badge bg-blue"><i class="fa fa-eye"></i> Schemes Details </span></a>';
     }else{
      $migrantSchemes = 'Not Available';
     }
     //$migrantSchemes = '<a href="#" onClick="modal_schemes('.$results['content']['id'].')" class="small-box-footer" ><span class="badge bg-blue"><i class="fa fa-eye"></i> Schemes Details </span></a>';

    $output = '<div class="panel panel-primary">
                <div class="panel-heading with-border">Basic Information  </div>
                <div class="panel-body">
                <div class="row">
                  <div class="col-md-12">
                    <table class="table table-bordered">
                      <tbody>';
                        
                        $output.= $dsdata.'<strong> Provisional MWIN : '.$results['content']['identification_number'].'
                          <br> Permanent MWIN : '.$mwin.'</strong></td></tr>

                      </tbody>
                    </table>
                  </div>
                </div>
                
                <div class="row"><div class="col-md-6">
                <table class="table table-bordered">
                        <tbody>
                            <tr>
                                <th style="width:8%">Sl.</th>
                                <th style="width:40%">Parameters</th>
                                <th>Information</th>
                            </tr>
                            <tr>
                                <td>'.$count++.' .</td>
                                <td>Name of the worker</td>
                                <td>'.$results['content']['name'].'</td>
                            </tr>
                            <tr>
                                <td>'.$count++.' .</td>
                                <td>Father&#39;s / Husband&#39;s Name</td>
                                <td>'.$results['content']['father_husband_name'].'</td>
                            </tr>
                            <tr>
                                <td>'.$count++.' .</td>
                                <td>Date of Birth</td>
                                <td>'.date('d-m-Y', strtotime($results['content']['dob'])).' [Age: '.date_diff(date_create($results['content']['dob']), date_create('today'))->y.']</td>
                            </tr>
                            <tr>
                                <td>'.$count++.' .</td>
                                <td>Gender</td>
                                <td>'.$gender.'</td>
                            </tr>
                            <tr>
                                <td>'.$count++.' .</td>
                                <td>Category</td>
                                <td>'.$caste.'</td>
                            </tr>
                            <tr>
                                <td>'.$count++.' .</td>
                                <td>Religion</td>
                                <td>'.$results['content']['religion_name'].'</td>
                            </tr>
                            <tr>
                                <td>'.$count++.' .</td>
                                <td>Mobile Number</td>
                                <td>'.l($results['content']['mobile'], 'tel:'.$results['content']['mobile']).'</td>
                            </tr>
                            <tr>
                              <td>'.$count++.' .</td>
                              <td>Emergency Contact Number</td>
                              <td>'.l($results['content']['emergency_contact_no'], 'tel:'.$results['content']['emergency_contact_no']).$relationship.'</td>
                            </tr>
                            
                            </table></div>
                            <div class="col-md-6">
                            <table class="table table-bordered">
                            <tbody>
                                <tr>
                                    <th style="width:8%">Sl.</th>
                                    <th style="width:40%">Parameters</th>
                                    <th>Information</th>
                                </tr>
                            
                           
                            <tr>
                                <td>'.$count++.' .</td>
                                <td>Worker Permanent Address</td>
                                <td>'.$results['content']['permanent_address'].' <br>'.$permanent_address.'</td>
                            </tr>'.$qualification_data.$technical_skill.'
                            
                            <tr>
                                <td>'.$count++.' .</td>
                                <td>Khadyasathi Number</td>
                                <td>'.$khadyasathi_no.'</td>
                            </tr>
                            <tr>
                                <td>'.$count++.' .</td>
                                <td>EPIC Number</td>'.$epic_no.'
                            </tr>
                            <tr>
                            <td>'.$count++.' .</td>
                            <td>Sasthyasathi Number</td>
                            <td>'.$sasthyasathi_no.'</td>
                          </tr>
                          <tr>
                            <td>'.$count++.' .</td>
                            <td>Schemes </td><td><b>';
                                  
                                  foreach ($results['content']['applicantScheme'] as $scheme_value) {
                                    
                                    $scheme_data = $scheme_value['schemes_name'].'<br>';
                                  }
                               $output .=$scheme_data.'</b></td></tr></tbody>
                    </table>
                    </div></div>
                   </div>    
                  </div>';
        $count2 = 1;
       
        $count3 = 1;
        $count4 = 1;
     // $mj_doc_phpto = '<a target="_blank" href="https://karmasathips.wblabour.gov.in/'.$results['content']['photo'].'"><i class="fa fa-file fa-lg"></i></a>';
     // $mj_doc_aadhar = '<a target="_blank" href="https://karmasathips.wblabour.gov.in/'.$results['content']['aadhar'].'"><i class="fa fa-file fa-lg"></i></a>';
      //$mj_doc_selfcertificate = '<a target="_blank" href="https://karmasathips.wblabour.gov.in/'.$results['content']['selfcertificate'].'"><i class="fa fa-file fa-lg"></i></a>';
     // $mj_doc_passbook = '<a target="_blank" href="https://karmasathips.wblabour.gov.in/'.$results['content']['passbook'].'"><i class="fa fa-file fa-lg"></i></a>';
     // $mj_doc_passport = '<a target="_blank" href="https://karmasathips.wblabour.gov.in/'.$results['content']['passport'].'"><i class="fa fa-file fa-lg"></i></a>';


      $mj_doc_passbook = '<a target="_blank" href="https://karmasathips.wblabour.gov.in/show-doc/passbook/'.encryption_decryption_fun('encrypt',$results['content']['id']).'"><i class="fa fa-file fa-lg"></i></a>';
      $mj_doc_passport = '<a target="_blank" href="https://karmasathips.wblabour.gov.in/show-doc/passport/'.encryption_decryption_fun('encrypt',$results['content']['id']).'"><i class="fa fa-file fa-lg"></i></a>';
      $mj_doc_selfcertificate = '<a target="_blank" href="https://karmasathips.wblabour.gov.in/show-doc/selfcertificate/'.encryption_decryption_fun('encrypt',$results['content']['id']).'"><i class="fa fa-file fa-lg"></i></a>';
      $mj_doc_phpto = '<a target="_blank" href="https://karmasathips.wblabour.gov.in/show-doc/photo/'.encryption_decryption_fun('encrypt',$results['content']['id']).'"><i class="fa fa-file fa-lg"></i></a>';
       //$mj_doc_phpto_i = '<iframe src="https://karmasathips.wblabour.gov.in/'.$results['content']['photo'].'" style="min-width:100px; min-height:100px;" frameborder="0"></iframe>';
      $mj_doc_aadhar = '<a target="_blank" href="https://karmasathips.wblabour.gov.in/show-doc/aadhar/'.encryption_decryption_fun('encrypt',$results['content']['id']).'"><i class="fa fa-file fa-lg"></i></a>';
           
          
    //echo $doc_file_name_aadhar;die;
    $output .= '<div class="panel panel-primary">
      <div class="panel-heading with-border">Document Information</div>
      <div class="panel-body">
          <table class="table table-bordered">
              <tbody>'.$approved_data.'
                  <tr>
                      <th style="width:8%">Sl.</th>
                      <th style="width:30%">Parameters</th>
                      <th>Information</th>
                      <th>Supporting Documents</th>
                  </tr>
                  <tr>
                    <td>'.$count3++.' .</td>
                    <td>Photo</td>  
                    <td> -- </td>
                    <td>'.$mj_doc_phpto.'</td>
                  </tr>
                  <tr>
                    <td>'.$count3++.' .</td>
                    <td>Self Certificate / Application Form with ANNEXURE -A (For Duare Sarkar Applications)</td>  
                    <td> -- </td>
                    <td>'.$mj_doc_selfcertificate.'</td>
                  </tr>
                  <tr>
                    <td>'.$count3++.' .</td>
                    <td>Bank Details</td>  
                    <td>Bank Name : '.$bank_name.'<br>Branch Name : '.$bank_branch.'<br> Account Number : '.$bank_account.'<br> IFSC Code : '.
                                                $ifsc_code.'</td>
                    <td>'.$mj_doc_passbook.'</td>
                  </tr>
                  <tr>
                    <td>'.$count3++.' .</td>
                    <td>Aadhaar Number</td>  
                    <td>'.$aadhaar_no.'</td>
                    <td>'.$mj_doc_aadhar.'</td>
                  </tr>
              </tbody>
          </table>
        </div>    
        </div>';
        $get_value = array_values($results['content']['workDetailsArray']);
       
        if(!empty($results['content']['passport']) || $results['content']['passport'] !=''){
            $th = '<th>Passport Details</th>';
            
         }else{
            $th = '';
         }
         
    $output .= '<div class="panel panel-primary">
                  <div class="panel-heading with-border">Worksite Information   </div>
                   <div class="panel-body">
                   <table class="table table-bordered">
                   <tbody>
                   <tr>'.$error_message_worksite.'</tr>
                   </tbody>
                   </table>
                    <table class="table table-bordered">
                        <tbody>
                            <tr>
                                <th style="width:4%">Sl.</th>
                                <th style="width:30%">Worksite Address</th>
                                <th>Migration Period</th>
                                <th>Nature of Work</th>
                                <th>Expected Wages per day</th>
                                '.$th.'
                                <th>View Details</th>
                            </tr>';
                                     
          foreach ($results['content']['workDetailsArray'] as $job_details) {
                
                  $migrated_from_date = date('d-m-Y', strtotime($job_details['migrated_from_date']));
                  $migrated_to_date = (!empty($job_details['migrated_to_date'])?date('d-m-Y', strtotime($job_details['migrated_to_date'])):'NA');
                  
                  $view = '<a href="#" onClick="get_modal_value('.$job_details['id'].')" class="small-box-footer" ><span class="badge bg-blue"><i class="fa fa-eye"></i> Job Details </span></a>';

                  //!empty($job_details->present_villwardname) ? $job_details->present_villwardname : '';

                  if($job_details['present_country'] == 1){
                    $present_country = 'India';
                    if(!empty($job_details['present_state'])){
                      $query_state = db_select('state_master','sm');
                      $query_state->fields('sm',array());
                      $query_state->condition('sm.id',$job_details['present_state'],'=');
                      $result_state = $query_state->execute()->fetchAssoc();
  
                        $present_state = $result_state['statename'].', ';

                      //$present_state = $job_details->present_state.', ';
                    }else{
                      $present_state = '';
                    } 

                    if(!empty($job_details['present_villwardname'])){
                      $present_villwardname = $job_details['present_villwardname'].', ';
                    }else{
                      $present_villwardname = '';
                    } 
                    if(!empty($job_details['present_blockmunname'])){
                      $present_blockmunname = $job_details['present_blockmunname'].', ';
                    }else{
                      $present_blockmunname = '';
                    } 
                    if(!empty($job_details['present_subdivision'])){
                      $present_subdivision = $job_details['present_subdivision'].', ';
                    }else{
                      $present_subdivision = '';
                    }
                    if(!empty($job_details['present_po'])){
                      $present_po = 'PO: '.$job_details['present_po'].', ';
                    }else{
                      $present_po = '';
                    }
                    if(!empty($job_details['present_pin'])){
                      $present_pin = 'PIN: '.$job_details['present_pin'].', ';
                    }else{
                      $present_pin = '';
                    }
                    if(!empty($job_details['present_dist'])){
                      $present_dist = $job_details['present_dist'].', ';
                    }else{
                      $present_dist = '';
                    }
                    if(!empty($job_details['present_ps'])){
                      $present_ps = 'PS: '.$job_details['present_ps'].', ';
                    }else{
                      $present_ps = '';
                    }

                  }else{
                    			
                     
                    $present_country = $job_details['country_name'];
                    $present_state = $present_ps = $present_dist = $present_pin = $present_po = $present_subdivision = $present_blockmunname = $present_villwardname = $present_state = '';
                  }

                  if(!empty($job_details['migrated_from_date'])){
                      $migrated_period_from = $job_details['migrated_from_date'];
                  }else{
                      $migrated_period_from= '';
                  }
                  if(!empty($job_details['migrated_to_date'])){
                        $migrated_period_to = '-'.$job_details['migrated_to_date'];
                  }else{
                        $migrated_period_to = 'NA';
                  }
                  
                  

                  
                    if($job_details['nature_of_work_id'] == 10){
                      $nature_work = $job_details['work_others'];
                    }else{
                      $nature_work = $job_details['nature_of_work'];
                    }
                  
                  

                    if(!empty($job_details['agency_name'])){
                        $agency_name = $job_details['agency_name'];
                    }else{
                      $agency_name = '&nbsp;';
                    }
                    if(!empty($job_details['agency_address'])){
                      $agency_address = $job_details['agency_address'];
                    }else{
                      $agency_address = '&nbsp;';
                    }
                    if(!empty($job_details['agency_mobile'])){
                      $agency_mobile = $job_details['agency_mobile'];
                    }else{
                      $agency_mobile = '&nbsp;';
                    }

                    if(!empty($job_details['employer_name'])){
                        $employer_name = $job_details['employer_name'];
                    }else{
                      $employer_name = '&nbsp;';
                    }
                    if(!empty($job_details['employer_address'])){
                      $employer_address = $job_details['employer_address'];
                    }else{
                      $employer_address = '&nbsp;';
                    }
                    if(!empty($job_details['employer_mobile'])){
                      $employer_mobile = $job_details['employer_mobile'];
                    }else{
                      $employer_mobile = '&nbsp;';
                    }
                    
                    
                    
                    if(!empty($job_details['passport_no'])){
                      $td = '<td>'.$job_details['passport_no'].'&nbsp;'.$mj_doc_passport.'</td>';
                    }else{
                      $td = '';
                    }

                  if($job_details['engaged_as'] == 'Self-employed'){
                    $agent = '';
                  }elseif ($job_details['engaged_as'] == 'Agency'){
                    $agent = '<div class="row">
                      <div class="col-md-5" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none; border-right:none;">Agency Name</div>
                      <div class="col-md-7" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none;">'.$agency_name.'</div>
                    </div>
                    <div class="row">
                      <div class="col-md-5" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none; border-right:none;">Agency Mobile</div>
                      <div class="col-md-7" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none;">'.$agency_mobile.'</div>
                    </div>
                    <div class="row">
                      <div class="col-md-5" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none; border-right:none;">Agency Address</div>
                      <div class="col-md-7" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none;">'.$agency_address.'</div>
                    </div>';
                  }elseif ($job_details['engaged_as'] == 'Without-agency') {
                    $agent = '<div class="row">
                      <div class="col-md-5" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none; border-right:none;">Employer Name</div>
                      <div class="col-md-7" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none;">'.$employer_name.'</div>
                    </div>
                    <div class="row">
                      <div class="col-md-5" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none; border-right:none;">Employer Mobile</div>
                      <div class="col-md-7" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none;">'.$employer_mobile.'</div>
                    </div>
                    <div class="row">
                      <div class="col-md-5" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none; border-right:none;">Employer Address</div>
                      <div class="col-md-7" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none;">'.$employer_address.'</div>
                    </div>
                    <div class="row">
                      <div class="col-md-5" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none; border-right:none;">Agency Name</div>
                      <div class="col-md-7" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none;">'.$agency_name.'</div>
                    </div>
                    <div class="row">
                      <div class="col-md-5" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none; border-right:none;">Agency Mobile</div>
                      <div class="col-md-7" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none;">'.$agency_mobile.'</div>
                    </div>
                    <div class="row">
                      <div class="col-md-5" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none; border-right:none;">Agency Address</div>
                      <div class="col-md-7" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none;">'.$agency_address.'</div>
                    </div>';
                  }
                    
    $output .= '<tr><td>'.$count2++.' .</td>
                  <td>'.$job_details['present_address'].'<br>'.$present_villwardname.$present_blockmunname.$present_subdivision.$present_po.$present_ps.$present_dist.$present_state.$present_pin.$present_country.'</td>
                  <td>'.$migrated_from_date.' - '.$migrated_to_date.'</td>
                  <td>'.$nature_work.'</td>
                  <td>'.$job_details['expected_salary'].'</td>
                  '.$td.'
                  <td>'.$view.' </td>
                </tr>
                
                <div class="modal fade" id="myModal'.$job_details['id'].'" role="dialog">
                  <div class="modal-dialog">
                    <div class="box box-primary box-solid">
                      <div class="box-header">
                        <button type="button" class="close" data-dismiss="modal" style="color:#fff;">&times;</button>
                        <h3 class="box-title">Work Related Information</h3>
                      </div>						
                      <div class="modal-body" style="margin-left: 5px; margin-right: 5px;">
                          <div class="row">
                            <div class="col-md-5" style="padding: 5px 10px; border: 1px solid #ccc; border-right:none;">Nature of Work </div>
                            <div class="col-md-7" style="padding: 5px 10px; border: 1px solid #ccc;">'.$nature_work.'</div>
                          </div> 
                          <div class="row">
                            <div class="col-md-5" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none; border-right:none;">Engaged As</div>
                            <div class="col-md-7" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none;">'.$job_details['engaged_as'].'</div>
                          </div>'.$agent.'
                          
                            <div class="row">
                            <div class="col-md-5" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none; border-right:none;">Migration Period</div>
                            <div class="col-md-7" style="padding: 5px 10px; border: 1px solid #ccc; border-top:none;">'.$migrated_period_from.' - '.$migrated_period_to.'</div>
                          </div>
                            
                          
                      </div>
                      <div class="modal-footer">
                        <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
                      </div>
                    </div>      
                  </div>
                </div>';
      }
      
    $output .='</tbody></table></div></div>'; 
    
    $output .= '<div class="panel panel-primary">
            <div class="panel-heading with-border">Nominee Details</div>
            <div class="panel-body">
                <table class="table table-bordered">
                    <tbody>
                      <tr>
                        <th>Sl.</th>
                        <th>Nominee Name</th>
                        <th>Relationship with the Nominee</th>
                        <th>Nominee Mobile Number</th>
                        <th>Nominee Aadhaar Number</th>
                      </tr>';
    if(!empty($results['content']['nomineeDetailsArray']))  {
      foreach ($results['content']['nomineeDetailsArray'] as $nominee) {
        $output .='<tr><td>'.$count4++.' .</td>
                        <td>'.$nominee['nominee_name'].'</td>
                        <td>'.$nominee['nominee_relationship'].'</td>
                        <td>'.$nominee['nominee_mobile'].'</td>
                        <td>'.$nominee['nominee_aadhar'].'</td>
                      </tr>';  
            } 
    } else{
      $output .='<tr><td style="text-align : center; color : red;" colspan="5"><strong> No Data Available</strong></td></tr>';
    }              
        
    $output .='</tbody> </table></div> </div>';       
    $output .= '<div class="panel panel-primary">
        <div class="panel-heading with-border">Family Details</div>
        <div class="panel-body">
        <div class="row"><div class="col-md-12">
            <table class="table table-bordered">
                <tbody>
                  <tr>
                    <th>Sl.</th>
                    <th>Family Member Name</th>
                    <th>Relationship with Family</th>
                    <th>Family Member Gender</th>
                    <th>Family Member Age</th>
                    <th>Family Member Aadhaar</th>
                    <th>Family Member Epic</th>
                    <th>Schemes</th>
                    
                  </tr>';
                  $count5=1;

                 
        if(!empty($results['content']['familyDetailsArray'])){
          foreach ($results['content']['familyDetailsArray'] as $family) {
            if ($family['member_gender'] == 'M'){
              $member_gender = 'Male';
            }else if($family['member_gender'] == 'F'){
                $member_gender = 'Female';
            }else if($family['member_gender'] =='O'){
              $member_gender = 'Other';
            }
            
            $schemes = '<a href="#" onClick="get_modal_schemes('.$family['id'].')" class="small-box-footer" ><span class="badge bg-blue"><i class="fa fa-eye"></i> Schemes Details </span></a>';
         $schemeCount = 1;
           
          $output .='<tr><td>'.$count5++.' .</td>
                      <td>'.$family['member_name'].'</td>
                      <td>'.$family['member_relationship'].'</td>
                      <td>'.$member_gender.'</td>
                      <td>'.$family['member_age'].'</td>
                      <td>'.$family['member_aadhar_no'].'</td>
                      <td>'.$family['member_epic'].'</td>
                      <td>'.$schemes.'</td>
                    </tr>
  
                  <div class="modal fade" id="myModalFamily'.$family['id'].'" role="dialog">
                    <div class="modal-dialog">
                      <div class="box box-primary box-solid">
                        <div class="box-header">
                          <button type="button" class="close" data-dismiss="modal" style="color:#fff;">&times;</button>
                          <h3 class="box-title">Schemes</h3>
                        </div>						
                        <div class="modal-body" style="margin-left: 5px; margin-right: 5px;">';
                       
                          foreach ($results['content']['familySchemes'][$family['id']] as $family_scheme_value) {
                            $family_scheme = '<div class="col-md-12"  style="padding: 5px 10px; border: 1px solid #ccc; text-align : center;"><strong>'.$family['member_name'].' ( '.$family['member_relationship'].' )<br>-- Availed Scheme -- </strong></div>
                            <div class="col-md-12" style="padding: 5px 10px; border: 1px solid #ccc;">'.$schemeCount++.'. '.$family_scheme_value['schemes_name'].'</div>';
                          }
              $output .= $family_scheme.'</div>
                        <div class="modal-footer">
                          <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
                        </div>
                      </div>      
                    </div>
                  </div>
              ';  
          } 
        }else{
          $output .='<tr><td style="text-align : center; color : red;" colspan="8"><strong> No Data Available</strong></td></tr>';
        }
        
    $output .='</tbody></table></div></div></div> </div>';  
  return $output;
}


function show_remark_details($results){
	global $user;
	 
	$user_id 		= $user->uid;
	$counter		= 0;
	$status			= '';
  // echo "<pre>";
	// print_r($results);
	
	$header = array(
					array('data' => 'Sl','width' => '5%'),
          array('data' => 'Remark', 'width' => '18%'),
					array('data' => 'Action Taken By', 'width' => '12%'),
					array('data' => 'Current Status', 'width' => '12%'),
					array('data' => 'Remark Date', 'width' => '9%'),
				);
	
	foreach($results as $row){
	 
		if(!empty($row['remark_date'])){
      $remark_date =  date('dS M, Y', $row['remark_date']);
    }elseif(!empty($row['remark_date_l'])){
      $remark_date =  date('dS M, Y', strtotime($row['remark_date_l']));
    }else{
      $remark_date = '';
    }

    if(!empty($row['status']) && $row['status'] == 'P'){
      $status = 'Provisional';
    }elseif(!empty($row['status']) && $row['status'] == 'A'){
      $status = 'Submitted';
    }elseif(!empty($row['status']) && $row['status'] == 'I'){
      $status = 'Incomplete';
    }elseif(!empty($row['status']) && $row['status'] == 'C'){
      $status = 'Approved';
    }elseif(!empty($row['status']) && $row['status'] == 'B'){
      $status = 'Back for Correction';
    }elseif(!empty($row['status']) && $row['status'] == 'R'){
      $status = 'Reject';
    }elseif(!empty($row['status']) && $row['status'] == 'BI'){
      $status = 'Incomplete (Rectified)';
    }elseif(!empty($row['status']) && $row['status'] == 'BP'){
      $status = 'Provisional (Rectified)';
    }elseif(!empty($row['status']) && $row['status'] == 'BA'){
      $status = 'Submitted (Rectified)';
    }elseif(!empty($row['status']) && $row['status'] == 'E'){
      $status = 'Expired';
    }
    if(!empty($row['action_by'])){
      $remark_by = get_alc_dlc_ins($row['action_by']);
    }else{
      $remark_by = 'Applicant';
    }
		
    if(!empty($row['action_to'])){
      $remark_to = get_alc_dlc_ins($row['action_to']);
    }else{
      $remark_to = '-';
    }
		

		$counter++;
		
		$rows[] = array(
					array('data' => $counter, 'align' => 'left', 'class' => array('even')),
					array('data' => $row['remark'], 'align' => 'left', 'class' => array('odd')),
					array('data' => $remark_by, 'align' => 'left', 'class' => array('even')),
					array('data' => $status, 'align' => 'left', 'class' => array('even')),
					array('data' => $remark_date, 'align' => 'left', 'class' => array('odd')),
		);
	}


	$variables = array(
					'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')),  
					'header' 	 => $header,
					'rows'		 => $rows,
					'empty'		 => 'No data found', 
				);
	$output = theme('datatable', $variables);
	return '<div class="feedback-scroll">'.$output.'</div>';			
}
function get_alc_dlc_ins($remark_by_to=''){
	
	$fetch_users_role		= db_select('l_custom_user_detail', 'lcud');
	$fetch_users_role->fields('lcud', array('fullname','usr_type'));
	$fetch_users_role->condition('lcud.usr_id', $remark_by_to);
	$fetch_users_data 	= $fetch_users_role->execute();
	if($fetch_users_data-> rowCount() > 0 ){
		$fetch_users_role_data = $fetch_users_data->fetchAssoc();
	$remark_res	= $fetch_users_role_data['fullname'].'&nbsp;('.$fetch_users_role_data['usr_type'].')';
	}else{
		$remark_res = '';
	}
	return $remark_res;
}
function action_taken_callback($form, $form_state) {
	$commands = array();
	$commands[] = ajax_command_replace('#remark_replace', drupal_render($form['migrant']['remark']));
	return array('#type' => 'ajax', '#commands' => $commands);
}

    