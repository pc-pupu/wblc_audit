<?php
drupal_add_css(drupal_get_path('module', 'motor_transport') . '/css/sky-forms.css');
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
drupal_add_css(drupal_get_path('module', 'motor_transport').'/css/popup.css');

function addmore_part($form, &$form_state, $id=0){	
	global $base_root, $base_path, $user;
	
	//$mtw_id = $id;
    $mtw_id = encryption_decryption_fun('decrypt',$id);
	$user_id = $user->uid;
	
	$fetch_reg_det 		= db_select('l_mtw_registration_master', 'lmrm');
	$fetch_reg_det->fields('lmrm', array('final_submit_status','status'));
	$fetch_reg_det->condition('lmrm.user_id', $user_id);
	$fetch_reg_det->condition('lmrm.id', $mtw_id); 
	$fetch_reg_det_result 	= $fetch_reg_det->execute(); 
		
	
	
	$applicant_id = $user->uid;
	
	if($applicant_id != 0){
		
	
	$ownershiptype = array(
		''					=>'-Select-',
		'director'			=>'Company registered under the Companies Act,1956',
		'proprietor'		=>'Proprietorship Firm',
		'partner'			=>'Partnership Firm',
		'general_manager'	=>'Public Sector Undertaking',
		'other'				=> 'Other'
		
	);
	
	$form['add_more']['header1'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="panel panel-default panel-form">
						<div class="panel-heading">Ownership Information of the Motor Transport Undertaking</div>
						<div class="panel-body">
							<span>'.t("<b>Note:</b> For Point number 8.(i), 8.(ii), and 9 of FORM-I Select ownership type of the Motor Transport Undertaking and then click on Add Details button to add person's details.").'</span>'
	);
	
	$form['add_more']['onwer_type'] = array(
		'#title'		=> 'Ownership Type',
		'#required'		=> TRUE,
		'#type'			=> 'select',
		'#options'		=> $ownershiptype,
		'#default_value'=> '',
		'#attributes' 	=> '',
		'#id'			=> 'desgVal',
		'#prefix' 		=> '<div class="row"><div class="col-lg-4 col-md-6"><label class="select">',
		'#suffix' 		=> '</label></div></div>',
		'#ajax'			=> array(
							'event'		=> 'change',
							'callback'	=> 'get_ajax_callback',
							'effect'	=> 'fade',
							//'progress'	=>	'',
						)
	);
	
	
	$ownershipVal =	isset($form_state['values']['onwer_type'])?$form_state['values']['onwer_type']:0;
	
	$form['add_more']['markup_link_add'] = array(
		'#prefix'	=> '<div id="addBtnDiv">',
		'#suffix'	=> '</div>'
	);
	
	if(!empty($ownershipVal) || $ownershipVal != 0){	
		if($fetch_reg_det_result->rowCount() > 0 ){
			$obj2 					= $fetch_reg_det_result->fetchObject(); 
			if($obj2->final_submit_status != 'P' && $obj2->status == ''){
				
				$form['add_more']['markup_link_add'] = array(
					'#type' 		=> 'markup',
					'#markup'		=> '<div id="addBtnDiv">'.l('+ Add Details ', 'mtw-registration-addmore/person/'.$id.'/'.encryption_decryption_fun('encrypt', $ownershipVal), array('attributes' => array('html' => TRUE, 'class' => 'btn btn-primary'))).'</div>'
				);
			}
		}											  
	}
	
	$form['add_more']['footer1'] = array(
		'#type' => 'markup',
		'#markup' => '</div></div>'
	);
	
	$form['add_more']['header2'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="panel panel-default panel-form">
						<div class="panel-heading">List of Owners</div>
						<div class="panel-body">'
	);
	
	$fetchPartnerList = getPersonList($id);
	
	$form['add_more']['markup_link'] = array(
			'#type' 		=> 'markup',
			'#markup'		=> $fetchPartnerList,
			'#prefix'		=> '',
			'#suffix'		=> '',
	);
	
	$form['add_more']['footer2'] = array(
		'#type' => 'markup',
		'#markup' => '</div></div>'
	);
	
	$form['add_more']['back_button'] = array(
		'#type'		=>'markup',
		'#markup'	=>l(t('Back'),'mtw-registration-details/default/'.$id,array('attributes' => array('html'=>TRUE , 'title'=>'Click for Previous Tab', 'class' => array('btn btn-primary'))))
	); 	
		
	$form['add_more']['skip_button'] = array(
		'#type'		=>'markup',
		'#markup'	=>l(t('Next'),'mtw-registration-details/upload/'.$id,array('attributes' => array('html'=>TRUE , 'title'=>'Click for Next Tab', 'class' => array('btn btn-primary mleft-15'))))	
	); 	
	
		return $form;
	}else{
		drupal_goto('applicant-login');
	}
}


function getPersonList($encyid){
	
	global $base_root, $base_path, $user;
 	$userid				= $user->uid;	
	$output 			= '';
	$counter			= 0;
	$view 				= 'Edit';
	$link_data			= 'person-edit/';
	$header 			= array (
							  array('data' => 'Sl. No', 'field' => 'slno', 'width' => '8%'),
							  array('data' => 'Name', 'field' => '', 'width' => '15%'),
							  array('data' => 'Designation', 'field' => '', 'width' => ''),
							  array('data' => 'Added Date', 'field' => '', 'width' => '12%'),
							  array('data' => 'Company\'s Name', 'field' => '', 'width' => '22%'),
							  array('data' => 'Action', 'field' => 'status', 'width' => '7%'),
							  //array('data' => 'Action', 'field' => 'actions', 'width' => '12%'),
						);
	$id 				= encryption_decryption_fun('decrypt', $encyid);
	$select_query_1		= db_select('l_mtw_registration_master_extra', 'lmrme');
	$select_query_1->fields('lmrme',array());
	$select_query_1->condition('lmrme.application_id', trim($id));
	$select_query_1->orderby('lmrme.id','DESC');
	
  	$result_1 			= $select_query_1->execute();
	$x 		  			= $result_1->fetchAll();
	
	//echo '<pre>'; print_r($x); die;
	
	$ownershiptype		= array(
							'director'	=>'Company registered under the Companies Act,1956',
							'proprietor'=>'Proprietorship Firm',
							'partner'	=>'Partnership Firm',
							'general_manager'		=>'Public Sector Undertaking',
							'other'		=> 'Other'
						);
	
	
	$rowNo 						=	$result_1->rowCount();
	if($rowNo>0){
		foreach($x as $record){
			$counter++;
			$peron_id			= 	$record->id;
			$name				=	$record->person_name;
			
			$desVal				=	$record->designation;
			
			if($desVal == 'director'){
				$designation	=	'Director of the Company registered under the Companies Act,1956';
			}else if($desVal == 'proprietor'){
				$designation	=	ucwords($desVal).' of the Company registered under the Companies Act,1956';
			}else if($desVal == 'partner'){
				$designation	=	ucwords($desVal).' of the Company registered under the Companies Act,1956';
			}else if($desVal == 'general_manager'){
				$designation	=	'General Manager of the Public Sector Undertaking';
			}else{
				$designation	=	$record->designation;
			}
			
			$person_district	=	'';
			$person_subdivision	=	'';
			$company_name		=	$record->company_name;
			$addeddt			=	date("dS M, Y",strtotime($record->added_date));
			$satus				=	$record->status;
			$del				=	$record->delete_status;	
			
			$application_status			=	'<img title="View Or Edit Information" alt="View Or Edit Information" src="'.$base_root.$base_path.'sites/all/themes/jackson/images/edit_icon.png">'.'';	
			$application_status_link	=	'mtw-registration-addmore/edit/'.trim($encyid).'//'.$peron_id;	
			$application_status_show	=	l($application_status,$application_status_link,array('html' => TRUE));
			
			
			$rows[] = array(
						array('data' =>	$counter, 'align' => 'center', 'class' => array('center')),
						array('data' =>	$name, 'align' => 'left', 'class' => array('')),
						array('data' =>	$designation, 'align' => 'left', 'class' => array('')),
						array('data' =>	$addeddt, 'align' => 'left', 'class' => array('')),
						array('data' =>	$company_name, 'align' => 'left', 'class' => array('')),
						array('data' =>	$application_status_show, 'align' => 'center', 'class' => array('center')),
					);
		}
	}
	// dashboard-table #sticky-enabled #datatables-processed #dataTable
	
		$variables = array(
					'attributes' 		=> array('class' => array('table table-striped table-responsive dashboard-table')), 
					'header' 			=> $header,
					'rows'				=> $rows,
					'empty' 			=> t("No data found!")
	  			 	);
	
		$output = theme('datatable', $variables); 
		$output.=   '<style type="text/css">
				.div-head{font-weight:bold;font-size:20px;text-align:left;font-family:cursive;padding:10px; margin-top: 50px;width: 100%;}
				</style>';
	return $output;
}
function get_ajax_callback($form, &$form_state){
	$commands	=	array();
	$commands[]	=	ajax_command_replace("#addBtnDiv",drupal_render($form['add_more']['markup_link_add']));
	return array('#type'=>'ajax', '#commands'=>$commands);
}