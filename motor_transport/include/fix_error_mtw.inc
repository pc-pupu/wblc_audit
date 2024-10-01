<?php

function fix_error_mtw(){
	
	$count = $count_su = 0;
	$mainloop = db_query("select id,user_id from l_mtw_registration_master order by id asc");
	if($mainloop->rowCount() > 0){
		$mainloopdata = $mainloop->fetchAll();
		foreach($mainloopdata as $row){
			echo $row->id.'--'.$row->user_id.'<br>';
			$user_id = $row->user_id;
			$application_id = $row->id;
			
			//$count_su = db_update('l_mtw_registration_renewal')->fields(array('user_id' =>$user_id))->condition('application_id',$application_id)->execute();
			//$count = $count+$count_su;
		}
		
	
	}
	echo $count;

}
