<?php
function changeconstatus($form, &$form_state){ 
	global $user; 
		
	$st_query = db_update('l_contractor_info_master')
  						->fields(array(
									'status' => $_POST['status'],    								
  									)
								)
  						->condition('id', $_POST['id'], '=')
						->condition('user_id', $user->uid, '=')
  						->execute();
	
	if($st_query){echo 'success'; die;}else{ echo 'fail'; die;}
	
	 // echo $_POST['id']; die;
	$queryabc = db_update('l_contractor_info_master');
    $queryabc->fields(array('status'=> $_POST['status']));
	$queryabc->condition('id',trim($_POST['id']));
	// $queryabc->condition('user_id',trim($user->uid));
    $queryabc->execute();
	echo 'success';
}
?>