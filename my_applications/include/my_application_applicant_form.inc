<?php

	function my_application_applicant_form($form, &$form_state) {
		$form['username']	=array(
								'#type'=>'textfield',
								'#title'=>t('Search'),
								'#size'=>20,
								'#default_value'=>(isset($form_state['values']['username'])) ? $form_state['values']['username'] : ''
							);
							
		$form['submit_button']=array(
								'#type'=>'submit',
								'#value'=>t('Search')
								);
		$fetchAllApplicationInfo	=	getAllApplicationData();
		$form['showmsg'] = array(
			  '#prefix' => '<div id="">',
			  '#markup' => ''.$fetchAllApplicationInfo.'',
			  '#suffix' => '</div>',   
          );
		return $form; 
	}
	
	
	
	function my_application_applicant_form_validate($form, &$form_state) {
		
	}
	
	function applicant_registration_form_submit($form, &$form_state) {
		
	}
	
	function getAllApplicationData(){
		
		global $base_root,$base_path;
		
		$output = '';
  		$header = array(
				array('data'=> 'Column 1'),
				array('data'=> 'Column 2'),
				array('data'=> 'Column 3'),
				array('data'=> 'Column 4'),
				array('data'=> 'Column 5'),
			);
		$rows = array();
		$rows[] = array('arnab','def','ghgfd','sdsds','sdsds', array('html' => TRUE,'attributes'=>array('title' => 'View')));
  
		$output = theme_table(
						array(
						  'header' => $header,
						  'rows' => $rows,
						  'attributes' => array('class' => array('formtitle', 'viewtable')),
						  //'sticky' => true,
						  //'caption' => '',
						  //'colgroups' => array(),
						  'empty' => t("No data found!")
						)
  		).theme('pager');
		return $output;
	}