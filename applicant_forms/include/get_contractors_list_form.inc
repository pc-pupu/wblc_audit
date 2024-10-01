<?php
function get_contractors_list_form($form, &$form_state){
	global $base_root, $base_path, $user;
	
	$fetchContractorData	=	GET_CONTRACTOR_LIST();
	
	/*$form['contractor_list']['search']	=	array(
												  '#prefix' 			=> '',
												  '#suffix' 			=> '',
												  '#title'  			=> 'Search',
												  '#type'     			=> 'textfield'
												);
	$form['contractor_list']['submit'] = 	array (
												  '#prefix' => '',  
												  '#suffix' => '',
												  '#type' => 'submit',
												  '#attributes' => array('class' => array('button')),
												  '#value' => 'Search'
								   				);*/
	$form['show_contractor_list'] = array(
								  	'#prefix' => '<div id="">',
			  						'#markup' => ''.$fetchContractorData.'',
			  						'#suffix' => '</div>'
          						);
	return $form;
}

function GET_CONTRACTOR_LIST(){
	global $base_root, $base_path, $user;
	
	ctools_include('modal');
	ctools_modal_add_js();
	$view_img= '<img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/lc-pdf.png" alt="View" width="25px"/>';
	
	$query 		 = 	db_query("select lcim.id, lcim.identification_number, lcim.name_of_contractor,lcim.application_id,lcim.contractor_user_id, lcim.email_of_contractor from l_contractor_info_master lcim where lcim.user_id=:user_id", array(':user_id' => $_SESSION['uid']));
	
	$output 	 =	'';
	$output 	.= 	'<style type="text/css">
			.contractor_list {
					margin:0px;padding:0px;
					width:100%;
					box-shadow: 10px 10px 5px #888888;
					border:1px solid #000000;
					-moz-border-radius-bottomleft:12px;
					-webkit-border-bottom-left-radius:12px;
					border-bottom-left-radius:12px;
					-moz-border-radius-bottomright:12px;
					-webkit-border-bottom-right-radius:12px;
					border-bottom-right-radius:12px;
					-moz-border-radius-topright:12px;
					-webkit-border-top-right-radius:12px;
					border-top-right-radius:12px;
					-moz-border-radius-topleft:12px;
					-webkit-border-top-left-radius:12px;
					border-top-left-radius:12px;
			}
			.contractor_list table{
				border-collapse: collapse;
				border-spacing: 0;
				width:100%;
				height:100%;
				margin:0px;padding:0px;
			}
			.contractor_list tr:last-child td:last-child {
				-moz-border-radius-bottomright:12px;
				-webkit-border-bottom-right-radius:12px;
				border-bottom-right-radius:12px;
			}
			.contractor_list table tr:first-child td:first-child {
				-moz-border-radius-topleft:12px;
				-webkit-border-top-left-radius:12px;
				border-top-left-radius:12px;
			}
			.contractor_list table tr:first-child td:last-child {
				-moz-border-radius-topright:12px;
				-webkit-border-top-right-radius:12px;
				border-top-right-radius:12px;
			}
			.contractor_list tr:last-child td:first-child{
				-moz-border-radius-bottomleft:12px;
				-webkit-border-bottom-left-radius:12px;
				border-bottom-left-radius:12px;
			}
			.contractor_list tr:hover td{
				background-color:#ffffff;
			}
			.contractor_list td{
				vertical-align:middle;
				background:-o-linear-gradient(bottom, #e3e5ef 5%, #ffffff 100%);	background:-webkit-gradient( linear, left top, left bottom, color-stop(0.05, #e3e5ef), color-stop(1, #ffffff) ); 
				background:-moz-linear-gradient( center top, #e3e5ef 5%, #ffffff 100% );
				filter:progid:DXImageTransform.Microsoft.gradient(startColorstr="#e3e5ef", endColorstr="#ffffff");	background: -o-linear-gradient(top,#e3e5ef,ffffff);
				background-color:#e3e5ef;
				border:1px solid #000000;
				border-width:0px 1px 1px 0px;
				text-align:left;
				padding:10px;
				font-size:10px;
				font-family:Arial;
				font-weight:normal;
				color:#000000;
			}
			.contractor_list tr:last-child td{
				border-width:0px 1px 0px 0px;
			}
			.contractor_list tr td:last-child{
				border-width:0px 0px 1px 0px;
			}
			.contractor_list tr:last-child td:last-child{
				border-width:0px 0px 0px 0px;
			}
			.contractor_list tr:first-child td{
				background:-o-linear-gradient(bottom, #95bcbc 5%, #759999 100%);	background:-webkit-gradient( linear, left top, left bottom, color-stop(0.05, #95bcbc), color-stop(1, #759999) );
				background:-moz-linear-gradient( center top, #95bcbc 5%, #759999 100% );
				filter:progid:DXImageTransform.Microsoft.gradient(startColorstr="#95bcbc", endColorstr="#759999");	background: -o-linear-gradient(top,#95bcbc,759999);
				background-color:#95bcbc;
				border:0px solid #000000;
				text-align:center;
				border-width:0px 0px 1px 1px;
				font-size:11px;
				font-family:Times New Roman;
				font-weight:bold;
				color:#ffffff;
			}
			.contractor_list tr:first-child:hover td{
				background:-o-linear-gradient(bottom, #95bcbc 5%, #759999 100%);	background:-webkit-gradient( linear, left top, left bottom, color-stop(0.05, #95bcbc), color-stop(1, #759999) );
				background:-moz-linear-gradient( center top, #95bcbc 5%, #759999 100% );
				filter:progid:DXImageTransform.Microsoft.gradient(startColorstr="#95bcbc", endColorstr="#759999");	background: -o-linear-gradient(top,#95bcbc,759999);
				background-color:#95bcbc;
			}
			.contractor_list tr:first-child td:first-child{
				border-width:0px 0px 1px 0px;
			}
			.contractor_list tr:first-child td:last-child{
				border-width:0px 0px 1px 1px;
			}
	</style>
	
					<div class="contractor_list" >
						<table >
							<tr>
								<td>Identification Number</td>
								<td>Name of the Contractor</td>
								<td>Email of the Contractor</td>
								<td>Download From V</td>
								<td>View</td>
							</tr>';
							/*foreach($query as $obj)
							{ print_r($obj);}
							die();*/
		if($query->rowCount()>0){
			foreach($query as $obj){
			$fetch_info_fromV 			= 	db_select('l_contractor_info_master', 'lcim');
			$fetch_info_fromV->fields('lcim',  array('is_from_v'));
			$fetch_info_fromV->condition('lcim.id',$obj->id);
			$fetch_info_fromV_result 	= 	$fetch_info_fromV->execute()->fetchObject();
			$is_from_v						=	$fetch_info_fromV_result->is_from_v;  
			
	        if($is_from_v==0)
			    $Download_From_V ="PENDING";
			else
				$Download_From_V = l($view_img.'  '.$obj->name_of_contractor, 'pdf_from_v_download/'.$obj->application_id.'/'.$obj->contractor_user_id.'/'.str_replace(' ','-', strtolower($obj->name_of_contractor)), array('html' => TRUE,'attributes' => array('target' => '_blank', 'style' => 'color: #0090C7;font-weight: 20px;text-decoration: underline; font-size:10px')));
				
				/*<img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/pdf.png">*/

	
			$output 	.=	'<tr>
								<td>'.$obj->identification_number.'</td>
								<td>'.$obj->name_of_contractor.'</td>
								<td>'.$obj->email_of_contractor.'</td>
								<td>'.$Download_From_V.' </td>
								<td>'.l(t('<img src="'.$base_root.$base_path.'sites/default/files/view.png">'),'applicant-profile/view-all-contractors/nojs/'.$obj->id, array('html' => TRUE,'attributes'=>array('title' => 'Click to View', 'class' => 'ctools-use-modal'))).' </td>
							</tr>';
			}
		}else{
			$output 	.=	'<tr><th>No Data Found</th></tr>';
		}
	$output		.=	'</table>
            	</div>';
	return $output;
}

function contractor_list_callback($ajax, $wildcard){
	if ( $ajax == 'ajax' ) {
			
			ctools_include('ajax');
			ctools_include('modal');
	
			$form_state = array(
							'ajax' => TRUE,
							'title' => t('View Contractor List'),
							'wildcard' => $wildcard
							);
			$output 	= ctools_modal_form_wrapper('contractor_list', $form_state);
	
			if (!empty($form_state['ajax_commands'])) {

				$output = $form_state['ajax_commands'];
			}
	
			print ajax_render($output);
			drupal_exit();
	}else {
			return drupal_get_form('contractor_list');
	}
}

function contractor_list($form, &$form_state){
		$contractor_id 	 		= 	$form_state['wildcard'];
		$result 			 	= 	db_select('l_contractor_info_master', 'lcim')
									->fields('lcim')
									->condition('lcim.id', trim($contractor_id))
									->execute();
  		$data 				 	= 	$result->fetchObject();
		
		$query 					= 	db_select('l_clra_con_nature_of_work', 'lccnow');
		$query->fields('cw', array('cont_work_name'));
		$query->leftJoin('contractor_works', 'cw', 'cw.contractor_work_id=lccnow.contractor_nature_of_work');
		$query->condition('lccnow.contractor_id', $data->id);
		$resultum 				= $query->execute();
		$result_Id				=	array();
		while( $datum = $resultum->fetchObject() ){
			$result_Id[] = $datum->cont_work_name;
		}
		$nature_of_wrk 			= "";
		$prefix 				= '';
		foreach($result_Id as $val){
			$nature_of_wrk 	   .= $prefix.$val;
			$prefix 			= ', ';
		}
		
		$output				 	=	'<style type="text/css">
										.application_view table {
											border:1px solid red;
											font-family: Arial, Helvetica, sans-serif;
									  	}
									  	.application_view td {
											padding: 4px 8px;
											border:1px solid #CCC;
									  	}
									  	.application_view tbody tr:nth-child(odd) {
											background: #dbe5f0;
									  	}
									  	.application_view tbody tr:nth-child(even) {
											background:#f1f1f1;
									  	}
									</style>
									<table width="100%" border="0" class="application_view">
									    <tr>
										<td width="45%"><strong>STATISTIC</strong></td>
										<td><strong>VALUE</strong></td>
									    </tr>
									    <tr>
										<td width="55%">Name of the Contractor</td>
										<td>'.$data->name_of_contractor.'</td>
										</tr>
										<tr>
										<td>Address of the Contractor</td>
										<td>'.$data->address_of_contractor.'</td>
										</tr>
										<tr>
										<td>Nature of Work in which Contract Labour is Employed or is to be Employed</td>
										<td>'.$nature_of_wrk.'</td>
										</tr>
										<tr>
										<td>Maximum Number of Contractor Labour to be Employed on any day Through Each Contractor</td>
										<td>'.$data->contractor_max_no_of_labours_on_any_day.'</td>
										</tr>
										<tr>
										<td>Estimated Date of Employment of Each Contract Work Under Each Contractor</td>
										<td>'.date('dS M,Y',strtotime($data->est_date_of_work_of_each_labour_to_date)).' to '.date('dS M,Y',strtotime($data->est_date_of_work_of_each_labour_from_date)).'</td>
										</tr>
									</table>';

		$form['viewData'] 		= 	array(
									  '#prefix' => '<div id="">',
									  '#markup' => ''.$output.'',
									  '#suffix' => '</div>',   
          							);
		return $form;
}