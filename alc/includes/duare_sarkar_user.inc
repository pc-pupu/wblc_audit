 <?php
//die('site under maintenance');
drupal_add_js(drupal_get_path('module', 'alc').'/js/duaresarkar.js');
drupal_add_css(drupal_get_path('module', 'alc').'/css/duaresarkar.css');

function add_slo_form($form, &$form_state,$id='')
{
	
   global $base_root, $base_path, $user;
   //if($user->uid!=1094){
	//die('site under maintenance');
   //}

	if(!empty($id)){
		
		$result = db_query("SELECT * from l_duare_sarkar_user_info where is_active=1 and id=".encryption_decryption_fun('decrypt',$id))->fetchAssoc();
		$block_result_arr=explode(',',$result['allotted_vill_ward_all']);
		
		
	}else{
			$get_all_gpword='';
			$gpword_arr = db_query("SELECT allotted_vill_ward_all from l_duare_sarkar_user_info where is_active=1")->fetchAll();
			foreach($gpword_arr as $gpword_value ){
				$get_all_gpword=$get_all_gpword.','.$gpword_value->allotted_vill_ward_all;
		}
		$block_get= array_map('intval', explode(',',ltrim($get_all_gpword,',')));

		//echo $abc = count($block_get);die;
	   // print_r($block_get);die;
		
        if(count($block_get)>1){ 
		$query1 = db_select('village_ward_master', 'vill');
		$query1->fields('vill', array('village_name','village_code'));
	 	$query1->condition('vill.village_code', $block_get, 'IN');
	 	$array_put = $query1->execute();
		while($word=$array_put->fetchAssoc())
		$gotarr[$word['village_code']]=$word['village_name'];
	}
}
	//print_r($gotarr);die;
   

	
	$user_id = $user->uid;

	if (in_array('ALC', $user->roles)) {
		$alc_check = db_select('l_customuser_relation_address', 'l');
		$alc_check->fields('l', array('district_code','sub_div_code'));
		$alc_check->condition('l.user_id', $user_id, '=');
		$alc_result = $alc_check->execute()->fetchAssoc();
		}

		$back_link  = $base_root . $base_path . 'duare-sarkar-user-list';
		$back = l(t('<span class="badge bg-yellow pull-right"><i class="fa fa-list"></i> DEO LIST</span>'), $back_link, array('html' => TRUE));

	$form['add_user_start'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="panel panel-primary">
						<div class="panel-heading with-border">ADD DEO'.$back.'</div>
						<div class="panel-body">'


	);

	



	  $value_subdivision=$alc_result['sub_div_code'];

	  $areatype_opt = common_custom_user_areatype_list_new($alc_result['district_code'], $value_subdivision);

	  $form['slo_id'] = array(
		'#type' => 'hidden',
		'#value' => $id,
	  );
	  if(empty($id)){
      $form['areatype'] = array(
        '#prefix' => '<div id="areatype_replace" class="col-md-3">',
        '#suffix' => '</div>',
        '#title' => strtoupper('Select Area Type'),
        '#default_value' => !empty($result['allotted_areatype']) ? $result['allotted_areatype'] : '',
        '#type' => 'select',
        '#options' => $areatype_opt,
        '#required' => TRUE,
        '#attributes' => array('class' => array('form-control'),'autocomplete'=>'off'),
        '#ajax' => array(
          'event' => 'change',
          'effect' => 'fade',
          'callback' => 'location_ajax_callback',
          'progress' => '',
        ),
      );
	  
	  $form['areatype_code'] = array(
		'#prefix' => '<div id="areatype_code_replace">',
		'#suffix' => '</div>',
	  );
	  $form['vill_ward'] = array(
		'#prefix' => '<div id="vill_ward_replace">',
		'#suffix' => '</div>',
	  );
	  $form['ps'] = array(
		'#prefix' => '<div id="ps_replace">',
		'#suffix' => '</div>',
	  );
    

	$value_areatype = isset($form_state['values']['areatype']) ? $form_state['values']['areatype'] : $result['allotted_areatype'];

    if ( $alc_result['district_code'] != "" && $value_subdivision != "" && $value_areatype != "") {

      if ($value_areatype == 'B') {
        $areatype_name = 'Block';
      } elseif ($value_areatype == 'M') {
        $areatype_name = 'Municipality';
      } elseif ($value_areatype == 'C') {
        $areatype_name = 'Corporation';
      } elseif ($value_areatype == 'S') {
        $areatype_name = 'SEZ';
      } elseif ($value_areatype == 'N') {
        $areatype_name = 'Notified Area';
      }


      $form['areatype_code'] = array(
        '#prefix' => '<div id="areatype_code_replace" class="col-md-3">',
        '#suffix' => '</div>',
        '#title' => strtoupper('Select ' . $areatype_name),
        '#type' => 'select',
        '#options' => common_custom_user_block_list_new($value_subdivision, $value_areatype),
        '#required' => TRUE,
        '#attributes' => array('class' => array('form-control'),'autocomplete'=>'off'),
        '#default_value' => !empty($result['allotted_areatype_code']) ? $result['allotted_areatype_code'] : '',
        '#ajax' => array(
          'event' => 'change',
          'effect' => 'fade',
          'callback' => 'location_ajax_callback',
          'progress' => '',
        )
      );


    }



    $value_areatype_code = isset($form_state['values']['areatype_code']) ? $form_state['values']['areatype_code'] : $result['allotted_areatype_code'];

    if ($alc_result['district_code'] != "" && $value_subdivision != "") {
      if ($value_areatype != "") {
        if ($value_areatype_code != "") {
          if ($value_areatype == 'B') {
			//if(empty($id)){

				//if(count($gotarr)>1){
					//$gpwordoption=array_diff_key(common_custom_user_villward_list_new($value_areatype_code, 'V'),$gotarr);
					//unset($gpwordoption[""]);
				//}else{
					//$gpwordoption=common_custom_user_villward_list_new($value_areatype_code, 'V');
					//unset($gpwordoption[""]);
				//}

				

			//}else{
				$gpwordoption=common_custom_user_villward_list_new($value_areatype_code, 'V');
				unset($gpwordoption[""]);	
			//}


            $form['vill_ward'] = array(
              '#prefix' => '<div id="vill_ward_replace" class="col-md-3">',
              '#suffix' => '</div>',
			  '#description'=>' <span style="color:red">TO SELECT MULTIPLE OPTION HOLD Ctrl BUTTON.</span>',
			  '#multiple' 		=> TRUE,
              '#title' => strtoupper('Select Gram Panchayat'),
              '#type' => 'select',
              '#attributes' => array('class' => array('form-control'),'autocomplete'=>'off'),
              '#options' => $gpwordoption,
              '#default_value' => $block_result_arr,
              '#required' => TRUE,
              '#ajax' => array(
                'event' => 'change',
                'effect' => 'fade',
                'callback' => 'location_ajax_callback',
                'progress' => '',
              )
            );
          }

          if ($value_areatype == 'M') {
			//if(empty($id)){


				//if(count($gotarr)>1){
					//$gpwordoption=array_diff_key(common_custom_user_villward_list_new($value_areatype_code, 'W'),$gotarr);
				   // unset($gpwordoption[""]);
				//}else{
					//$gpwordoption=common_custom_user_villward_list_new($value_areatype_code, 'W');
					//unset($gpwordoption[""]);
				//}

				

			//}else{
				$gpwordoption=common_custom_user_villward_list_new($value_areatype_code, 'W');
				unset($gpwordoption[""]);	
			//}

            $form['vill_ward'] = array(
              '#prefix' => '<div id="vill_ward_replace" class="col-md-3">',
              '#suffix' => '</div>',
              '#title' => strtoupper('Select Ward'),
              '#type' => 'select',
              '#options' =>$gpwordoption,
			  '#description'=>' <span style="color:red">TO SELECT MULTIPLE OPTION HOLD Ctrl BUTTON.</span>',
              '#default_value' =>  $block_result_arr,
              '#required' => TRUE,
			  '#multiple' 		=> TRUE,
              '#attributes' => array('class' => array('form-control'),'autocomplete'=>'off'),
              '#ajax' => array(
                'event' => 'change',
                'effect' => 'fade',
                'callback' => 'location_ajax_callback',
                'progress' => '',
              )
            );

          }

          if ($value_areatype == 'C') {
			//if(empty($id)){

				//if(count($gotarr)>1){
					//$gpwordoption=array_diff_key(common_custom_user_corporation_list_new($value_areatype_code),$gotarr);
					//unset($gpwordoption[""]);
				//}else{
					//$gpwordoption=common_custom_user_corporation_list_new($value_areatype_code);
					//unset($gpwordoption[""]);
				//}

				
				

			//}else{
				$gpwordoption=common_custom_user_corporation_list_new($value_areatype_code);
				unset($gpwordoption[""]);	
			//}
			

            $form['vill_ward'] = array(
              '#prefix' => '<div id="vill_ward_replace" class="col-md-3">',
              '#suffix' => '</div>',
              '#title' => strtoupper('Select Ward'),
              '#type' => 'select',
			  '#multiple' 		=> TRUE,
              '#options' => $gpwordoption,
			  '#description'=>' <span style="color:red">TO SELECT MULTIPLE OPTION HOLD Ctrl BUTTON.</span>',
              '#default_value' =>$block_result_arr,
              '#required' => TRUE,
              '#attributes' => array('class' => array('form-control'),'autocomplete'=>'off'),
              '#ajax' => array(
                'event' => 'change',
                'effect' => 'fade',
                'callback' => 'location_ajax_callback',
                'progress' => '',
              )
            );
          }
		 // if(empty($id)){

			//if(count($gotarr)>1){
				//$gpwordoption=array_diff_key(common_custom_user_villward_list_new($value_areatype_code,'S'),$gotarr);
				//unset($gpwordoption[""]);
			//}else{
				//$gpwordoption=common_custom_user_villward_list_new($value_areatype_code,'S');
				//unset($gpwordoption[""]);
			//}

			

		//}else{
			$gpwordoption=common_custom_user_villward_list_new($value_areatype_code,'S');
			unset($gpwordoption[""]);	
		//}

          if ($value_areatype == 'S') {

            $form['vill_ward'] = array(
              '#prefix' => '<div id="vill_ward_replace" class="col-md-3">',
              '#suffix' => '</div>',
              '#title' => strtoupper('Select Sector'),
              '#type' => 'select',
			  '#description'=>' <span style="color:red">TO SELECT MULTIPLE OPTION HOLD Ctrl BUTTON.</span>',
			  '#multiple' 		=> TRUE,
              '#options' => $gpwordoption,
              '#default_value' => $block_result_arr,
              '#required' => TRUE,
              '#attributes' => array('class' => array('form-control'),'autocomplete'=>'off'),
              '#ajax' => array(
                'event' => 'change',
                'effect' => 'fade',
                'callback' => 'location_ajax_callback',
                'progress' => '',
              )
            );

          }

          if ($value_areatype == 'N') {
			//if(empty($id)){

				//if(count($gotarr)>1){
					//$gpwordoption=array_diff_key(common_custom_user_villward_list_new($value_areatype_code,'N'),$gotarr);
					//unset($gpwordoption[""]);
				//}else{
					//$gpwordoption=common_custom_user_villward_list_new($value_areatype_code,'N');
					//unset($gpwordoption[""]);
				//}

				
	
			//}else{
				$gpwordoption=common_custom_user_villward_list_new($value_areatype_code,'N');
				unset($gpwordoption[""]);	
			//}

            $form['vill_ward'] = array(
              '#prefix' => '<div id="vill_ward_replace" class="col-md-3">',
              '#suffix' => '</div>',
              '#title' => strtoupper('Select Notified Area'),
              '#type' => 'select',
			  '#multiple' 		=> TRUE,
			  '#description'=>' <span style="color:red">TO SELECT MULTIPLE OPTION HOLD Ctrl BUTTON.</span>',
              '#options' => $gpwordoption,
              '#default_value' => $block_result_arr,
              '#required' => TRUE,
              '#attributes' => array('class' => array('form-control'),'autocomplete'=>'off'),
              '#ajax' => array(
                'event' => 'change',
                'effect' => 'fade',
                'callback' => 'location_ajax_callback',
                'progress' => '',
              )
            );

          }
        }
      }
      
    }
	$value_vill_ward = isset($form_state['values']['vill_ward']) ? $form_state['values']['vill_ward'] : $block_result_arr;

    if ($alc_result['district_code'] != "" && $value_subdivision != "") {
		if ($value_areatype != "") {
		  if ($value_areatype_code != "") {
			if ($value_vill_ward != "" && !in_array('', $value_vill_ward)) {
			  $form['ps'] = array(
				'#prefix' => '<div id="ps_replace" class="col-md-3">',
				'#suffix' => '</div>',
				'#title' => strtoupper('Select Police Station'),
				'#type' => 'select',
				'#options' => common_custom_user_ps_list_new($alc_result['district_code']),
				'#default_value' => !empty($result['allotted_ps']) ? $result['allotted_ps'] : '',
				'#attributes' => array('class' => array('form-control'),'autocomplete'=>'off'),
				//'#required' => TRUE
			  );
			}
		  }
		}
	  }
	  }




	

	$form['name'] = array(
		'#prefix' => '<div class="col-md-6">',
		'#default_value' 		=>!empty($result['name']) ? trim($result['name']) : '',	
		'#suffix' => '</div>',
		'#title' => 'NAME OF THE DEO',
		'#attributes' => array('class' => array('form-control'), 'autocomplete' => 'off'),
		'#type' => 'textfield',
		//'#element_validate' => array('MISC_TEXTBOX_NAME_VALIDATE'),
		'#required' => TRUE,
	);
	$form['mobile'] = array(
		'#prefix' => '<div class="col-md-6">',
		'#suffix' => '</div>',
		'#title' => 'MOBILE NUMBER',
		'#type' => 'textfield',
		'#element_validate' => array('MISC_PHONE_VALIDATE'),
		'#attributes' => array('class' => array('form-control'), 'autocomplete' => 'off', 'maxlength' => 10, 'minlength' => 10),
		'#required' => TRUE,
		'#default_value' => !empty($result['mobile']) ? $result['mobile'] : '',
	);
	$form['email'] = array(
		'#prefix' => '<div class="col-md-6">',
		'#suffix' => '</div>',
		'#title' => 'EMAIL',
		'#type' => 'textfield',
		'#attributes' => array('class' => array('form-control'), 'autocomplete' => 'off'),
		'#required' => TRUE,
		'#element_validate' => array('MISC_EMAIL_VALIDATE'),
		'#default_value' => !empty($result['email']) ? $result['email'] : '',
	);
	if(empty($id)){
	$form['username'] = array(
		'#prefix' => '<div class="col-md-6">',
		'#suffix' => '</div>',
		'#title' => 'USER NAME',
		'#type' => 'textfield',
		'#attributes' => array('class' => array('form-control'), 'autocomplete' => 'off'),
		'#element_validate'	=> array('MISC_USERNAME_VALIDATE'),
		'#description'=>' <span style="color:red">USERNAME DO NOT ACCEPT ANY SPACE</span>',
		//'#element_validate' => array('MISC_EMAIL_VALIDATE'),
		//'#default_value' => !empty($result1['comapny_e_mail']) ? $result1['comapny_e_mail'] : '',
		'#required' => TRUE,
	);
	$form['password'] = array(
		'#prefix' => '<div class="col-md-6">',
		'#suffix' => '</div>',
		'#title' => 'PASSWORD',
		'#type' => 'password',
		'#description'=>'<div class="clearfix"><div class ="col-md-12"><div id="setPasswordMessage" style="display:none;"></div></div></div>',
		'#attributes' => array('id' => 'passwordId','class' => array('form-control'), 'autocomplete' => 'off','onkeyup' => 'setCheckPasswordStrength(this.val);', 'onblur' => 'setCheckPasswordStrength(this.val);'),

		//'#default_value' => !empty($result1['comapny_e_mail']) ? $result1['comapny_e_mail'] : '',
		'#required' => TRUE,
	);

	$form['cpassword'] = array(
		'#prefix' => '<div class="col-md-6">',
		'#suffix' => '</div>',
		'#title' => 'CONFIRM PASSWORD',
		'#type' => 'password',
		'#attributes' => array('id' => 'conpasswordId','class' => array('form-control'), 'autocomplete' => 'off','onkeyup' => 'setCheckPasswordStrength(this.val);', 'onblur' => 'setCheckPasswordStrength(this.val);'),
		//'#default_value' => !empty($result1['comapny_e_mail']) ? $result1['comapny_e_mail'] : '',
		'#required' => TRUE,
	);
	// $form['markup_msg'] = array(
    //     '#type'                => 'markup',
    //     '#markup'            => '<div class="clearfix"><div class ="col-md-6"><div id="setPasswordMessage" style="display:none;"></div></div></div>'
    // );
	
}
	$form['aadhar'] = array(
		'#prefix'       => '<div class="col-md-6">',
		'#suffix'       => '</div>',
		'#title'        => t('AADHAAR NUMBER'),
		'#type'         => 'textfield',
		'#default_value'=> ($result['aadhaar_number']!='') ? $result['aadhaar_number'] : '',
		'#required'     => TRUE,
		'#attributes'   => array('class' => array('form-control'),'maxlength' => 12,'minlength' => 12,'accept' => 'number','autocomplete' => 'off'),  
	);
	$form['pan'] = array(
		'#prefix'       => '<div class="col-md-6">',
		'#suffix'       => '</div>',
		'#title'        => t('PAN NUMBER'),
		'#type'         => 'textfield',
		'#default_value'=> ($result['pan_number']!='') ? $result['pan_number'] : '',
		'#required'     => TRUE,
		'#attributes'   => array('class' => array('form-control'),'autocomplete' => 'off'),  
	);


	$form['ifsc_code'] = array(
		'#prefix' => '<div class="col-md-6">',
		'#suffix' => '</div>',
		'#title' => t('IFS Code'),
		'#type' => 'textfield',
		'#required' => TRUE,
		'#autocomplete_path' => 'migrantworker/autocomplete_ifsc',
		'#attributes' => array('class' => array('form-control'), 'autofocus' => 'autofocus'),
		'#default_value' => ($result['ifsc_code'] != '') ? $result['ifsc_code'] : '',
		'#ajax' => array(
			'callback' => 'get_bank_ajax_callback',
			'event' => 'change',
			'effect' => 'fade',
			'progress' => array(
				'type' => 'throbber',
				'message' => '',
			)
		),

	);
	
    
	$ifsc_code = isset($form_state['values']['ifsc_code']) ? $form_state['values']['ifsc_code'] : $result['ifsc_code'];
	if ($ifsc_code != "" && !empty($ifsc_code)) {
		// $getBankDetails = db_select('bank_branch_master','master')->fields('master', array())->condition('ifsc_code',$ifsc_code)->execute()->fetchAssoc();
		$query1 = db_query('select * from bank_branch_master where ifsc_code=:ifsc_code', array(':ifsc_code' => $ifsc_code));
		if ($query1->rowCount() > 0) {
			$getBankDetails = $query1->fetchAssoc();

			$form['bank_name'] = array(
				'#title' => t('BANK NAME'),
				'#type' => 'textfield',
				'#required' => TRUE,
				'#attributes' => array('class' => array('form-control'), 'autocomplete' => 'off','id'=>'bank_name','readonly'=>'readonly'),
				'#value' => ($getBankDetails['bank_name'] != '') ? $getBankDetails['bank_name'] : '',
				'#prefix' => '<div class="col-md-6" id="bank_name_replace">',
				'#suffix' => '</div>',
			);
			$form['bank_branch'] = array(
				'#title' => t('BRANCH NAME'),
				'#type' => 'textfield',
				'#required' => TRUE,
				'#attributes' => array('class' => array('form-control'), 'autocomplete' => 'off','readonly'=>'readonly'),
				'#value' => ($getBankDetails['branch_name'] != '') ? $getBankDetails['branch_name'] : '',
				'#prefix' => '<div class="col-md-6" id="bank_branch_replace">',
				'#suffix' => '</div>',
			);

		} else {
			$form['bank_name'] = array(
				'#title' => t('BANK NAME'),
				'#type' => 'textfield',
				//'#element_validate' => array('bank_du_validation'),
				'#required' => TRUE,
				'#default_value' => ($result['bank_name'] != '') ? $result['bank_name'] : '',
				'#attributes' => array('class' => array('form-control'), 'autocomplete' => 'off','id'=>'bank_name'),
				'#prefix' => '<div class="col-md-6" id="bank_name_replace">',
				'#suffix' => '</div>',
			);
			$form['bank_branch'] = array(
				'#title' => t('BRANCH NAME'),
				'#type' => 'textfield',
				//'#element_validate' => array('bank_du_validation'),
				'#required' => TRUE,
				'#default_value' => ($result['branch_name'] != '') ? $result['branch_name'] : '',
				'#attributes' => array('class' => array('form-control'), 'autocomplete' => 'off'),
				'#prefix' => '<div class="col-md-6" id="bank_branch_replace">',
				'#suffix' => '</div>',
			);
		}
	} else {
		$form['bank_name'] = array(
	
			'#prefix' => '<div id="bank_name_replace">',
			'#suffix' => '</div>',
		);
		$form['bank_branch'] = array(
		
			'#prefix' => '<div  id="bank_branch_replace">',
			'#suffix' => '</div>',
		);
	}

	$form['bank_account'] = array(
		'#title' => t('ACCOUNT NUMBER'),
		'#type' => 'textfield',
		'#required' => TRUE,
		'#attributes' => array('accept' => 'number','class' => array('form-control'), 'autocomplete' => 'off'),
		'#default_value' => ($result['bank_account_number'] != '') ? $result['bank_account_number'] : '',
		'#prefix' => '<div class="col-md-6">',
		'#suffix' => '</div>',
	);

	$form['address'] = array(
		'#prefix' => '<div class="col-md-12">',
		'#default_value' 		=>!empty($result['user_address']) ? trim($result['user_address']) : '',	
		'#suffix' => '</div>',
		'#title' => 'ADDRESS',
		'#attributes' => array('class' => array('form-control'), 'autocomplete' => 'off'),
		'#type' => 'textarea',
		//'#required' => TRUE,
	);
	if(empty($id)){
	$form['submit'] = array(
		'#prefix' => '<div class="col-md-12">',
		'#suffix' => '</div>',
		'#type' => 'submit',
		'#attributes' => array('class' => array('btn btn-primary pull-left')),
		'#value' => 'SUBMIT'
	);
}else{
	$form['submit'] = array(
		'#prefix' => '<div class="col-md-12">',
		'#suffix' => '</div>',
		'#type' => 'submit',
		'#attributes' => array('class' => array('btn btn-primary pull-left')),
		'#value' => 'UPDATE'
	);
}

	$form['add_user_end'] = array(
		'#type' => 'markup',
		'#markup' => '</div></div>'
	);
	return $form;

}

function add_slo_form_submit($form, &$form_state){
	global $base_root, $base_path, $user;
	
    $val = $form_state['values'];

	$vill_ward_arr=$val['vill_ward'];
	
	$time =	time();
	$date =	date('Y-m-d');
	$tempAckNumber = $time."-".$date;

	 $val['bank_branch'] = !empty($val['bank_branch']) ? $val['bank_branch'] : $_POST['bank_branch'];
	$val['bank_name'] = !empty($val['bank_name']) ? $val['bank_name'] : $_POST['bank_name'];
	
	if (in_array('ALC', $user->roles)) {
		$alc_check = db_select('l_customuser_relation_address', 'l');
		$alc_check->fields('l', array('district_code','sub_div_code'));
		$alc_check->condition('l.user_id', $user->uid, '=');
		$alc_result = $alc_check->execute()->fetchAssoc();
	}
	if(empty($val['slo_id'])){

		if(!empty($val['ifsc_code'])||!empty($val['bank_name'])||!empty($val['bank_branch'])){
		
			if($val['cpassword']===$val['password']){

				$user_name = trim($form_state['values']['username']);
				if(!empty($user_name)) {
					$check_usernme_res = checkScriptNameExist($user_name);
				}

				$users_check_query = db_select('users', 'u');
				$users_check_query->fields('u', array('name'));
				$users_check_query->condition('u.name',$user_name);
				$users_check_query_result = $users_check_query->execute();

				$num_of_results = $users_check_query_result->rowCount();



				/** Check In Migrant Username exists */

				$migrant_check_arr=array();

				$migrant_check_arr['source']				= 'WBLC';
				$migrant_check_arr['taskid']				= 'DUARESARKARCHECKUSER';
				$migrant_check_arr['username']				= $user_name;


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
				curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($migrant_check_arr));
				curl_setopt($ch, CURLOPT_POST, true);
				$results    = curl_exec($ch);
				$errors     = curl_error($ch); 
				//print_r($errors); 
				curl_close($ch);			
				$results_migrantcheck = json_decode(trim($results), TRUE);
				
				/*** End  */


				if ($num_of_results==0 && $results_migrantcheck['content']==0){

					$newUser = 	array(
								'name' 		=> trim($form_state['values']['username']),				
								'pass' 		=> trim($form_state['values']['password']), 
								'mail' 		=> trim($form_state['values']['email']),
								'status' 	=> 0,
								'init' 		=> trim($form_state['values']['email']),
							);				
			 		user_save(null, $newUser); 
					//$transaction = db_transaction();

					//try {

					$fetch_info_querys = db_select('users', 'U');
					$fetch_info_querys->fields('U',  array('uid'));
					$fetch_info_querys->condition('U.name',$user_name);
					$fetch_info_querys_result = $fetch_info_querys->execute()->fetchObject();
					$wblc_deoId	= $fetch_info_querys_result->uid;

					$userInfo = array(
								'uid' 			=> $wblc_deoId,
								'fullname'		=> $form_state['values']['name'],
								'uid_phone' 	=> $form_state['values']['mobile'],
								'pass_wd' 		=> trim($form_state['values']['password']), 
								'tempacknumber' => $tempAckNumber  
								);
					db_insert('l_user_additional_reg_details')->fields($userInfo)->execute();

					$userRole = array(
						'uid' => $wblc_deoId,
						'rid' => 27
					);
					db_insert('users_roles')->fields($userRole)->execute();
            
				
					$vill_ward_string='';

					foreach($vill_ward_arr as $vill_ward_val){
						$vill_ward_string=$vill_ward_string.",".$vill_ward_val;
					}
					$vill_ward_final=ltrim($vill_ward_string,',');

					$basic_arr=array(
							'user_role'				=>27,
							'user_id'				=>$wblc_deoId,
							'name'					=>$val['name'],
							'email'					=>$val['email'],
							'mobile'				=>$val['mobile'],						
							'created_by'			=>$user->uid,
							'created_by_role'		=>4,
							'aadhaar_number'		=>$val['aadhar'],
							'pan_number'			=>$val['pan'],
							'ifsc_code'				=>$val['ifsc_code'],
							'bank_name'				=>$val['bank_name'],
							'branch_name'			=>$val['bank_branch'],
							'bank_account_number'	=>$val['bank_account'],
							'user_address'		 	=>$val['address'],
							'allotted_district'		=>$alc_result['district_code'],
							'allotted_subdivision'	=>$alc_result['sub_div_code'],
							'allotted_areatype'		=>$val['areatype'],
							'allotted_areatype_code'=>$val['areatype_code'],
							'allotted_vill_ward_all'	=>$vill_ward_final,
							'allotted_ps'			=>$val['ps'],
							'updated_date'			=>NULL
							

					);
					db_insert('l_duare_sarkar_user_info')->fields($basic_arr)->execute();

					if(in_array('ALC', $user->roles)){
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
					}
				

					$val['app_createdby_hrmsid']	= $hrms_employee_id ;
					$val['app_createdby_fullname']	= $alc_full_name ;
					$val['source']					= 'WBLC';
					$val['taskid']					= 'DUARESARKARADDUSER';
					$val['taskfor']					= 'adduser';
					$val['user_id']  				= $wblc_deoId;
					$val['created_user_id']  		= $user->uid;
					$val['allotted_district']       = $alc_result['district_code'];
					$val['allotted_subdivision']    = $alc_result['sub_div_code'];
					$val['allotted_vill_ward_all']	=$vill_ward_final;
				
				
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
					curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($val));
					curl_setopt($ch, CURLOPT_POST, true);
					$results    = curl_exec($ch);
					$errors     = curl_error($ch); 
					//echo json_encode($val);die;
					curl_close($ch);			
					$results = json_decode(trim($results), TRUE);

			
					// send mail start
					$get_user = db_query("SELECT * from l_duare_sarkar_user_info where is_active=1 and user_id=".$wblc_deoId)->fetchAssoc();
					$get_subdiv = db_query("SELECT sub_div_name from sub_division where sub_div_code=".$alc_result['sub_div_code'])->fetchAssoc();

					if ($get_user['allotted_areatype'] == 'B') {
						$areatype_name = 'Block';
					} elseif ($get_user['allotted_areatype'] == 'M') {
						$areatype_name = 'Municipality';
						$ward='WARD:-';
					} elseif ($get_user['allotted_areatype'] == 'C') {
						$areatype_name = 'Corporation';
					} elseif ($get_user['allotted_areatype'] == 'S') {
						$areatype_name = 'SEZ';
					} elseif ($get_user['allotted_areatype']== 'N') {
						$areatype_name = 'Notified Area';
					}
			
					$block_get=explode(',',$get_user['allotted_vill_ward_all']);
					$query1 = db_select('village_ward_master', 'vill');
					$query1->fields('vill', array('village_name'));
					$query1->condition('vill.village_code',  $block_get, 'IN');
					$result1 = $query1->execute()->fetchall();

					$view_area='';
					foreach($result1 as $get_block_arr){

						$view_area=$view_area.','. strtoupper($get_block_arr->village_name);
				
					}
				
					$mailto  = trim($form_state['values']['email']);
					$subject = 'Welcome to Labour Commissionerate, Govt. Of West Bengal';
					
					$message='Hello <strong>'.trim($form_state['values']['name']).'</strong>,<br><br>
					You have been allocated in ALLOTTED AREA- '.ltrim($get_alloted_area,',').','.$areatype_name. '-'.$ward.ltrim($view_area,',').' by '. $alc_full_name.'( ALC)  , '.$get_subdiv['sub_div_name'].' <br><br>'.
					'<U> Your DEO credentials are as follows</u>:<br><br>
					<strong>Username : </strong>'.trim($form_state['values']['username']).'<br> 
					<strong>Password : </strong>'.$form_state['values']['password'].'<br><br>
					You have to login in the Provided Link : <a href="https://karmasathips.wblabour.gov.in/user/login" target="_blank">LOGIN LINK</a><br> 
					Kindly login and check with the provided Username and Password.
					For any queries, contact with your concerned ALC.<br>
					For any technical queries, email us at wbkarmasathi.nic@gmail.com';

					// $template_id = 1107161520430394545;	
					// $msg	=	"Thank you ".trim($form_state['values']['name'])." for registering on Labour Commissionerate. Please keep the username safely and change your password at regular intervals. Your Username- ".trim($form_state['values']['username'])." Labour Commissionerate, WB";
					// send_sms_for_user_alert($phone, $msg, $template_id);
					
					$mail_status=send_mail_for_user_alert($mailto, $subject, $message);
					if($mail_status==true){
					drupal_set_message('USER ADDED SUCCESSFULLY');
					}else{
						drupal_set_message('MAIL ID NOT VALID','error');
					}
				 
				
				

					// } catch (Exception $e) {
					// 	$transaction->rollback();
					// 	drupal_set_message(t('Unable to take action. Please try again.'), 'error');
					// 	return false;
					// }
					// unset($transaction);

				}else{
					$form_state['rebuild']=TRUE;
					drupal_set_message('Username already Exists.','error');
				}
			}else{
				$form_state['rebuild']=TRUE;
				drupal_set_message('Password and Confirm Password not match .','error');
			
			}
		}else{
			$form_state['rebuild']=TRUE;
			drupal_set_message('BANK DETAILS NOT FOUND .','error');
			

		}
	}else{

		//die("YOU CAN NOT UPDATE NOW");
		 $getsloid = db_query("SELECT user_id from l_duare_sarkar_user_info where is_active=1 and id=".encryption_decryption_fun('decrypt',$val['slo_id']))->fetchAssoc();
		 $newUser = 	array(
			'mail' 		=> trim($form_state['values']['email']),
			'init' 		=> trim($form_state['values']['email']),
		);


		
		db_update('users')->fields($newUser)->condition('uid', $getsloid['user_id'])->execute();

		 $userInfo = array(
			'fullname'		=> $form_state['values']['name'],
			'uid_phone' 	=> $form_state['values']['mobile'],
			);
			db_update('l_user_additional_reg_details')->fields($userInfo)->condition('uid', $getsloid['user_id'])->execute();
			
			$basic_arr=array(
				
				'name'					=>$val['name'],
				'email'					=>$val['email'],
				'mobile'				=>$val['mobile'],						
				'aadhaar_number'		=>$val['aadhar'],
				'pan_number'			=>$val['pan'],
				'ifsc_code'				=>$val['ifsc_code'],
				'bank_name'				=>$val['bank_name'],
				'branch_name'			=>$val['bank_branch'],
				'bank_account_number'	=>$val['bank_account'],
				'user_address'		 	=>$val['address'],
				'updated_date'			=>date('Y-m-d h:i:s'),
		);
		db_update('l_duare_sarkar_user_info')->fields($basic_arr)->condition('user_id', $getsloid['user_id'])->execute();

		//update in wblc complete=======================================================================

		$basic_arr['source']					= 'WBLC';
				$basic_arr['taskid']					= 'DUARESARKARADDUSER';
				$basic_arr['taskfor']					= 'updateuser';
				$basic_arr['wblc_user_id']  				= $getsloid['user_id'];
				$basic_arr['slo_id']=encryption_decryption_fun('decrypt',$val['slo_id']);
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
				curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($basic_arr));
				curl_setopt($ch, CURLOPT_POST, true);
				$results    = curl_exec($ch);
				$errors     = curl_error($ch); 
				//print_r($errors); 
				curl_close($ch);	
					
				$results = json_decode(trim($results), TRUE);
					
				if($results['status_code']==200){
					drupal_set_message('USER DETAILS UPDATE SUCCESSFULLY');

				}else{
					drupal_set_message('SOMETHING WRONG','error');
				}
				
	
}
}





function get_bank_ajax_callback($form, $form_state)
{
	$commands = array();
	$commands[] = ajax_command_replace('#bank_name_replace', drupal_render($form['bank_name']));
	$commands[] = ajax_command_replace('#bank_branch_replace', drupal_render($form['bank_branch']));
	return array('#type' => 'ajax', '#commands' => $commands);
}

function location_ajax_callback($form, $form_state){

	$commands = array();
	$commands[] = ajax_command_replace('#areatype_code_replace', drupal_render($form['areatype_code']));
	$commands[] = ajax_command_replace('#areatype_replace', drupal_render($form['areatype']));
	$commands[] = ajax_command_replace('#vill_ward_replace', drupal_render($form['vill_ward']));
	$commands[] = ajax_command_replace('#ps_replace', drupal_render($form['ps']));
	return array('#type' => 'ajax', '#commands' => $commands);

}

function slo_list($form, $form_state){
	global $user,$base_path,$base_root;

	$query = db_select('l_duare_sarkar_user_info', 'larum');
	// $query -> leftJoin('users', 'ur','ur.uid=larum.user_id ');
	$query->fields('larum', array('mobile','user_id','email','name','id'));
	//$query->fields('ur', array('name'))
	if (!in_array('SUPER ADMIN', $user->roles)) {
	$query->condition('larum.created_by', $user->uid, '=');
	}
	$query->condition('larum.is_active', 1, '=');
	$query->orderBy('larum.name','ASC');
	$result = $query->execute();
	

	

	$header = array(
		array('data' => 'Sl. No.', 'width' => '10%'),
		array('data' => 'Username', 'width' => '10%'),
		array('data' => 'Name', 'width' => '20%'),
		array('data' => 'Mobile', 'width' => '15'),
		array('data' => 'E-MAIL', 'width' => '15%'),
		array('data' => 'ACTION', 'width' => '30%'),

  );

  $i = 0;
  while ($data = $result->fetchObject()) { //print_r($data);die;
	$getUsername     = db_select('users', 'master')->fields('master', array('name'))->condition('uid', $data->user_id)->execute()->fetchAssoc();
   

    $i++;
	if (in_array('SUPER ADMIN', $user->roles)) {
		$change_password_link=$base_root . $base_path . 'superadmin/duare-sarkar-change-deo-password/' . encryption_decryption_fun('encrypt',$data->user_id);
	}else{
	 $change_password_link=$base_root . $base_path . 'duare-sarkar-change-deo-password/' . encryption_decryption_fun('encrypt',$data->user_id);
	}
    $edit_link = $base_root . $base_path . 'duare-sarkar-add-user/' . encryption_decryption_fun('encrypt', $data->id);
	
	$view_link = '<input type="button"  value="VIEW" class="bg-purple" style="border: none;" onClick="user_view('.$data->id.')">';
    $change_password = l(t('<span class="badge bg-purple">CHANGE PASSWORD</span>'), $change_password_link, array('html' => TRUE));
	$edit = l(t('<span class="badge bg-purple">EDIT</span>'), $edit_link, array('html' => TRUE));
	if (in_array('ALC', $user->roles)) {
		$deactivatelink  = $base_root . $base_path . 'deactivate-deo/' . encryption_decryption_fun('encrypt', $data->user_id);
		$deactivate = l(t('<span class="badge bg-purple">Deactivate DEO</span>'), $deactivatelink, array('html' => TRUE));
		if($user->uid==1094){
			$all_link		 =	$view_link.' || '.$change_password.' || '.$deactivate.'||'.$edit;
		}else{
			$all_link		 =	$view_link.' || '.$change_password.' || '.$deactivate.'||'.$edit;
		}
		
	}elseif(in_array('SUPER ADMIN', $user->roles)){
		$deactivatelink  = $base_root . $base_path . 'deactivate-deo/' . encryption_decryption_fun('encrypt', $data->user_id);
		$deactivate = l(t('<span class="badge bg-purple">Deactivate DEO</span>'), $deactivatelink, array('html' => TRUE));
		$all_link		 =	$view_link.' || '.$change_password.' || '.$deactivate;
	
	}
	
    $rows[] = array(
      $i,
	  $getUsername['name'],
	  $data->name,
      $data->mobile,
	  $data->email,

	  $all_link

    );
  }


  $variables = array(
    'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')),
    'header' => $header,
    'rows' => $rows,
    'empty' => t("No data found!")
  );

  $output =theme('datatable', $variables);

  $back_link  = $base_root . $base_path . 'duare-sarkar-add-user';
		$back = l(t('<span class="badge bg-yellow pull-right"><i class="fa fa-user-plus"></i> ADD DEO</span>'), $back_link, array('html' => TRUE));
  $form['list_start'] = array(
	'#type' => 'markup',
	'#markup' => '<div class="panel panel-primary">
					<div class="panel-heading with-border">DEO LIST'.$back.'</div>
					<div class="panel-body">'


);
$form['list'] = array(
	'#type' => 'markup',
	'#markup' => '	<div id="myModal" class="modal dsul-modal"  style="display: none;">
	
	<!-- Modal content -->
	<div class="modal-content">
	  <span class="close">&times;</span>
	  <div id="mod"></div>
	</div>
  
  </div>'.$output
);
  $form['list_end'] = array(
	'#type' => 'markup',
	'#markup' => '</div></div>'
);
return $form;


}
function slo_details_view(){
	 $get_user = db_query("SELECT * from l_duare_sarkar_user_info where is_active=1 and id=".$_POST['id'])->fetchAssoc();

	 $table			= 'l_duare_sarkar_user_info';
	 $type			= '';
	 $did			=$get_user['id'];
	 $fieldArr		= array('allotted_district','allotted_subdivision','allotted_areatype','allotted_areatype_code','allotted_vill_ward','allotted_ps');
	 $keyField = 'id';
	 $get_alloted_area 	= get_full_address($table, $type, $did, $fieldArr,$keyField);
	 $ward='GP/WORD - ';
	 if ($get_user['allotted_areatype'] == 'B') {
        $areatype_name = 'Block';
		
      } elseif ($get_user['allotted_areatype'] == 'M') {
        $areatype_name = 'Municipality';
		
      } elseif ($get_user['allotted_areatype'] == 'C') {
        $areatype_name = 'Corporation';
      } elseif ($get_user['allotted_areatype'] == 'S') {
        $areatype_name = 'SEZ';
      } elseif ($get_user['allotted_areatype']== 'N') {
        $areatype_name = 'Notified Area';
      }

	  $block_get=explode(',',$get_user['allotted_vill_ward_all']);

	  $query1 = db_select('village_ward_master', 'vill');
		$query1->fields('vill', array('village_name'));
	 	$query1->condition('vill.village_code',  $block_get, 'IN');
	 	$result1 = $query1->execute()->fetchall();
		$view_area='';
		foreach($result1 as $get_block_arr){
			$view_area=$view_area.','. strtoupper($get_block_arr->village_name);

        	}
	        

         


    
	$output='<div class="panel panel-warning">
	<div class="panel-heading"><strong>User Details</strong></div>
	<div class="box-body">';
	$output.='<table width="100%" cellspacing="0" cellpadding="0" class="table table-striped table-responsive dashboard-table admin-custom-table">
	                                 <thead>
									 <tr>
									 <th colspan=2>
									 
									 DETAILS OF THE USER: '.strtoupper($get_user['name']).'
									 </th>
									 </tr>
									 </thead>
									
										<tr>
										<td>E-MAIL</td>
										<td>'.$get_user['email'].'</td>
									  </tr>
									  <tr>
									  <td>MOBILE</td>
										<td>'.$get_user['mobile'].'</td>
									  </tr>
									  <tr>
									  <td>AADHAAR NUMBER</td>
									  <td>'.$get_user['aadhaar_number'].'</td>
									</tr>
									<tr>
									<td>PAN NUMBER</td>
									<td>'.$get_user['pan_number'].'</td>
								  </tr>
								  <tr>
									<td>BANK NAME</td>
									<td>'.$get_user['bank_name'].'</td>
								  </tr>
								  <tr>
								  <td>BANK BRANCH</td>
								  <td>'.$get_user['branch_name'].'</td>
								</tr>
								<tr>
								  <td>IFSC CODE</td>
								  <td>'.$get_user['ifsc_code'].'</td>
								</tr>
								<td>BANK ACCOUNT NUMBER</td>
								<td>'.$get_user['bank_account_number'].'</td>
							  </tr>
							  </tr>
								<td>USER ADDRESS</td>
								<td>'.$get_user['user_address'].'</td>
							  </tr>
							  </tr>
								<td>ALLOTTED AREA</td>
								<td>'.ltrim($get_alloted_area,',').'</td>
							  </tr>
							  </tr>
								<td>'.$areatype_name.'</td>
								<td>'.$ward.ltrim($view_area,',').'</td>
							  </tr>
							  ';

								



                                       
	 $output.='</table></div></div>';
	echo $output;
}
function change_deo_password($form, &$form_state,$id=''){
	$get_user = db_query("SELECT name from l_duare_sarkar_user_info where is_active=1 and user_id=".encryption_decryption_fun('decrypt',$id))->fetchAssoc();
	$form['add_user_start'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="panel panel-primary">
						<div class="panel-heading with-border">CHANGE PASSWORD OF '.strtoupper($get_user['name']).'</div>
						<div class="panel-body">'


	);

	$form['uid'] = array(
		'#type' => 	'hidden',
		'#default_value' => 	!empty($id) ? $id : '',
		'#required' => TRUE
	 );

	$form['password'] = array(
		'#prefix' => '<div class="col-md-6">',
		'#suffix' => '</div>',
		'#title' => 'PASSWORD',
		'#type' => 'password',
		'#attributes' => array('id' => 'passwordId','class' => array('form-control'), 'autocomplete' => 'off','onkeyup' => 'setCheckPasswordStrength(this.val);', 'onblur' => 'setCheckPasswordStrength(this.val);'),

		//'#default_value' => !empty($result1['comapny_e_mail']) ? $result1['comapny_e_mail'] : '',
		'#required' => TRUE,
	);
	$form['markup_msg'] = array(
        '#type'                => 'markup',
        '#markup'            => '<div class="clearfix"><div class ="col-md-12"><div id="setPasswordMessage" style="display:none;"></div></div></div>'
    );
	$form['cpassword'] = array(
		'#prefix' => '<div class="col-md-6">',
		'#suffix' => '</div>',
		'#title' => 'CONFIRM PASSWORD',
		'#type' => 'password',
		'#attributes' => array('id' => 'conpasswordId','class' => array('form-control'), 'autocomplete' => 'off','onkeyup' => 'setCheckPasswordStrength(this.val);', 'onblur' => 'setCheckPasswordStrength(this.val);'),
		//'#default_value' => !empty($result1['comapny_e_mail']) ? $result1['comapny_e_mail'] : '',
		'#required' => TRUE,
	);
	
	$form['submit'] = array(
		'#prefix' => '<div class="col-md-12">',
		'#suffix' => '</div>',
		'#type' => 'submit',
		'#attributes' => array('class' => array('btn btn-primary')),
		'#value' => 'UPDATE'
	);
	$form['add_user_end'] = array(
		'#type' => 'markup',
		'#markup' => '</div></div>'
	);

	return $form;
}
function change_deo_password_submit($form, &$form_state){
	$val = $form_state['values'];
	
	if(!empty($val['uid'])){
		$user_id=encryption_decryption_fun('decrypt',$val['uid']);
		if($val['password']===$val['cpassword']){

		

		if($val['password'] != ''){
			define('DRUPAL_ROOT', getcwd());
			require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
			drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
			require_once DRUPAL_ROOT . '/includes/password.inc';
			$newpassword = user_hash_password($val['password']);
		}else{
			$user_query = db_select('users', 'u');	
			$user_query->fields('u', array('pass'));	
			$user_query->condition('u.uid',trim($user_id),'=');		
			$user_result = $user_query->execute()->fetchAssoc();		
			$newpassword = $user_result['pass'];		
		}
		$user_primary_info = array( 
			'pass'	=>	$newpassword					
			  );

		$query = db_update('users');
		$query->fields($user_primary_info);              	
		$query->condition('uid', $user_id);
		$query->execute();

		$user_others_info = array( 
			
			'pass_wd' 		=> trim($form_state['values']['password']), 
							
							);
		
		$query = db_update('l_user_additional_reg_details');
		$query->fields($user_others_info);              	
		$query->condition('uid', $user_id);
		$query->execute();

		$wblc_user_arr['source']				= 'WBLC';
		$wblc_user_arr['taskid']					= 'DEOPASSOWRDCHANGE';
		$wblc_user_arr['wblc_uid']  				= $user_id;
		$wblc_user_arr['new_password']  				= $form_state['values']['password'];
		
		
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
		curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($wblc_user_arr));
		curl_setopt($ch, CURLOPT_POST, true);
		$results    = curl_exec($ch);
		$errors     = curl_error($ch); 
		//print_r($errors); 
		curl_close($ch);			
		$results = json_decode(trim($results), TRUE);
		drupal_set_message('PASSWORD CHANGED SUCCESSFULLY');

	}else{
		$form_state['rebuild']=TRUE;
		drupal_set_message(t('PASSWORD AND CONFIRM PASSWORD NOT MATCH.','error'));
	}


	}else{

    drupal_set_message(t('USER NOT FOUND.','error'));

}
	
}


function deactivate_deo_user($form, &$form_state ,$deouser_id =''){

	 global $user,$base_path,$base_root;
	
	 $deoUserId = encryption_decryption_fun('decrypt',$deouser_id);

	 $get_user = db_query("SELECT * from l_duare_sarkar_user_info where is_active=1 and user_id=".$deoUserId)->fetchAssoc();

     if(!empty($get_user)){

	 	$wblc_user_arr['source']		= 'WBLC';
	 	$wblc_user_arr['taskid']		= 'DEODEACTIVATE';
	 	$wblc_user_arr['deo_uid']  	    =  $deoUserId;

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
	 	curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($wblc_user_arr));
	 	curl_setopt($ch, CURLOPT_POST, true);
	 	$results    = curl_exec($ch);
	 	$errors     = curl_error($ch); 
	 	//echo json_encode($wblc_user_arr);die();
	 	//print_r($errors); 
	 	curl_close($ch);			
	 	$results = json_decode(trim($results), TRUE); 
	 	if($results['content']==1){

	 		$info = db_update('l_duare_sarkar_user_info')->fields(array('is_active'=>0,'deactivate_from'=>$user->uid,'deactivate_date'=>date('Y-m-d H:i:s')))->condition('is_active',1)->condition('user_id',$deoUserId)->execute();
	 		if($info == 1){
				drupal_set_message('DEO '.$get_user['name'].' DEACTIVATED. THE DEACTIVATED USERNAME CANNOT BE USED.');
				drupal_goto('duare-sarkar-user-list');
	 			drupal_set_message('DEO '.$get_user['name'].' DEACTIVATED. THE DEACTIVATED USERNAME CANNOT BE USED.');
	 		}
	 	}
	 }
}

function bank_du_validation($element,&$form_state){
	$txtValue		=	trim($element['#value']); 
	$match_pattern 	= 	 '/^[a-zA-Z0-9]*$/';
	if(empty($txtValue)) {
		form_error($element, t($element['#title'].'IS EMPTY'));
	}
	if(!preg_match($match_pattern, $txtValue)) {
		form_error($element, t($element['#title'].' NOT VALID'));
	}
}