<?php
function add_faq($form , &$form_states, $id=''){
	$question = '';
	$answer = '';
	$order = '';
	$status = 'Y';
	if(!empty($id)){
		$id = encryption_decryption_fun('decrypt',$id);
		$faq_query = db_select('l_faq_master', 'f');	
		$faq_query->fields('f', array());
		$faq_query->condition('f.id', $id, '=');		
		$result = $faq_query->execute()->fetchAssoc();	 print_r($result); die;	
	}
	$form['faq']['question'] = array(
							'#prefix'=> '<div class="form-custom col-md-12"><label class="input">',
							'#suffix'=> '</label></div>',
							'#title' =>'Question',
							'#type' => 'textfield',
							'#required' => TRUE,
							'#default_value' => $question,
							'#attributes' => array('class' => array('form-control')),	
						  );
	$form['faq']['answer'] = array(
							'#prefix'=> '<div class="form-custom col-md-12"><label class="input">',
							'#suffix'=> '</label></div>',
							'#title' =>'Answer',
							'#cols' => 20, 
							'#rows' => 3,
							'#type' => 'textarea',
							'#required' => TRUE,
							'#default_value' => $answer,
							'#attributes' => array('class' => array('form-control')),	
						  );	
	$form['faq']['order'] = array(
							'#prefix'=> '<div class="form-custom col-md-3"><label class="input">',
							'#suffix'=> '</label></div>',
							'#title' =>'Order',
							'#type' => 'textfield',
							'#required' => TRUE,
							'#default_value' => $order,
							'#attributes' => array('class' => array('form-control')),	
						  );					   
	$form['faq']['is_active'] = array(
							'#prefix'=> '<div class="col col-md-3"><label class="radios">',
							'#suffix' => '</label></div>',
							'#type' => 'radios',
							'#title' =>'Status',
							'#required'	=> TRUE,
							'#type' => 'radios',
							'#default_value' => $status,
							'#optionS' => array('Y' => 'Active', 'N' => 'Inactive'),
							'#required' => TRUE,
							'#attributes' => array('class' => array('form-control')),	
						  );
						  
	return $form;	 
}

function list_faq(){
	drupal_add_js(drupal_get_path('module', 'faq').'/js/faq.js');										  
	$rows = array();   
	
   	$header = array(
           array('data'=> 'Sl. No.', 'width' => '7%'),	
		   array('data'=> 'Question/Answer'),
		   array('data'=> 'Status', 'width' => '7%'),
		   array('data'=> 'Action', 'width' => '7%')	  
		  );		
	
	$faq_query = db_select('l_faq_master', 'f');	
	$faq_query->fields('f', array());
	// $faq_query->condition('f.is_active', 'Y', '=');
	$faq_query = $faq_query->extend('TableSort')->extend('PagerDefault')->limit(15);
	$faq_query->orderBy('ord', 'desc');
	
	$faq_result = $faq_query->execute();
	$i = 1;
	if($faq_result->rowCount()>0){
		foreach($faq_result as $data){
			$answer = '<font color="red">Not Available</font>';
			if($data->is_active == 'Y'){$status = '<i class="fa fa-check-circle  fa-2x status" style="color:green; cursor:pointer;" st="Y" data-id="'.$data->id.'"></i>';}else{$status = '<i class="fa fa-times-circle fa-2x status" style="color:red;cursor:pointer;" st="N" data-id="'.$data->id.'"></i>';}
			$viewedit = l('<i class="fa fa-edit fa-2x"></i>', 'faq/edit/'.encryption_decryption_fun('encrypt',$data->id), array('html' => TRUE,'attributes'=> array('target'=>'_blank')));	
			if($data->answer != ''){$answer = $data->answer;}
			$rows[] = array(
						$i++,
						'<strong>Q:'.$data->question.'<div>Ans: </strong>'.$answer.'</div>',
						$status,
						$viewedit		
					);
		}
	}
		
	$variables = array(
		'attributes' 		=> array('class' => array('table table-striped table-responsive admin-custom-table')), 
		'header' 			=> $header,
		'rows'				=> $rows,
		'empty' => t(' No data .'),
		);
	
	// $output = '<div class="box box-primary"><div class="box-header"></div><div class="box-body">';
	$output .= theme('table', $variables).theme("pager");
	// $output .= '</div></div>';
	
	return $output; 
}
?>