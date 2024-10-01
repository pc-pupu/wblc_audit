<?php
function documents_view($user_id = '', $application_id = '', $document_type_code = '', $table = '', $field = '', $documenttypefield = '', $filecontent = '', $filename = ''){
	$applicant_user_id = encryption_decryption_fun('decrypt', $user_id);
	$application_id = encryption_decryption_fun('decrypt', $application_id);
	$document_type_code = encryption_decryption_fun('decrypt', $document_type_code);
	
	if($table == ''){ $table = 'l_encrypted_uploaded_documents';}
	if($field == ''){ $field = 'application_id';}
	if($documenttypefield == ''){ $documenttypefield = 'document_type_code';}
	if($filecontent == ''){ $filecontent = 'filecontent';}
	if($filename == ''){ $filename = 'filename';}
	
	$query = db_select($table,'leud')
				->fields('leud',array('application_id' ,'document_type_code', $filename, $filecontent))
				->condition('user_id',$applicant_user_id)
				->condition($field, $application_id)
				->condition($documenttypefield, $document_type_code)
				->condition('status','1')				
				->execute();
	if($query->rowCount() > 0){
		$data = $query->fetchObject(); // echo '<pre>'; print_r($data); die;
		header('Content-type: application/pdf');
		// header('Content-Disposition: attachment; filename="avatar.pdf"');
		header('Content-Disposition: inline; filename="'.$data->$filename.'"');			
		echo $content = base64_decode($data->$filecontent); die;	
	}else{
		return 'File Not Uploaded';
	}
}