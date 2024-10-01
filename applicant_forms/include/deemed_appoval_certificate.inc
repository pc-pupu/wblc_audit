<?php
function deemed_appoval_certificate($form, &$form_state, $act_id = '', $application_id = ''){ 
	echo encryption_decryption_fun('decrypt', $act_id).'<==>'.encryption_decryption_fun('decrypt', $application_id); die;
}
