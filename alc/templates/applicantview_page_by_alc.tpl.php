<?php 
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
drupal_add_css(drupal_get_path('module', 'alc') . '/css/customstyle.css');
drupal_add_js(drupal_get_path('module', 'alc') . '/js/mycustom.js');	
?>
    
    
<?php

global $base_root, $base_path, $user;


if(trim($_POST['clra_alc_gen_certificates'])=='Generate Certificate'){
	if($_POST['alc_clra'] != '' ){
	$applicant_address="select loc_e_subdivision,name_areatype,clra_qr_code,registration_number from l_clra_registration_master where user_id='".$_POST['user_id']."' and id='".$_POST['var_id']."'";
$applicant_address_result=db_query($applicant_address); 
if($applicant_address_result->rowcount()>0){
	foreach($applicant_address_result as $row) {
			$subdivision =trim($row->loc_e_subdivision);
			$area =trim($row->name_areatype);
			$clra_qr_code=$row->clra_qr_code;
			$registration_number=$row->registration_number;
	
	
		
	}
}			
			
$reg_date = date('m/d/Y h:i:s a', time());			
			
//-----------------Reg Genaration
		
$query_Is_reg_code 		 		= 	db_select('l_clra_registration_master', 'lcrm');
$query_Is_reg_code				->	fields('lcrm',array('registration_number'));
$query_Is_reg_code				->	condition('lcrm.registration_number','NULL','!=')
				 				->	orderBy('lcrm.id', 'DESC')
				    			->	range(0,1);
$query_Is_reg_code_result 		= 	$query_Is_reg_code->execute()->fetchObject()->registration_number;	
		
$getShortName 				    =   custom_user_short_name_fun($subdivision);  // ---------come from custom_user module  
$reg_code						=	get_registration_code($area);//-------------miscellaneous module
$getBlockCodeRes 				=   substr($reg_code, -2);
			
if(empty($query_Is_reg_code_result)){
			$reg		=	$getShortName.$getBlockCodeRes.'/'.'CLR'.'/'.'000001';
}else{
		
			
			$reg_query		=	db_query("select  max (NULLIF(substr(registration_number,11,6),'') :: integer) as serial_num  from l_clra_registration_master where loc_e_subdivision='".$subdivision."'");
			$x				=	$reg_query->fetchAssoc();
			$reg_code 		=	$x['serial_num'];
			if(empty($reg_code)){
						$reg		=	$getShortName.$getBlockCodeRes.'/'.'CLR'.'/'.'000001';

			}else{
					  $reg_code_next			= 	$reg_code+1;
					  $reg_firts				=	$getShortName.$getBlockCodeRes.'/'.'CLR'.'/';
					  $reg_second				=	str_pad($reg_code_next, 6, "0", STR_PAD_LEFT);
					  $reg						=	$reg_firts.$reg_second;
			}
}




$clra_qr_code					= 'CLRA-REG'.$_POST['var_id'].'A1U'.$_POST['user_id'].'T'.time();

$frm_v   					=  	array('is_from_v'=>1);
$frm_v_gen 					=  	db_update('l_contractor_info_master');
$frm_v_gen					->	fields($frm_v);
$frm_v_gen					->	condition('application_id',$_POST['var_id']);
$frm_v_gen					->	condition('user_id',$_POST['user_id']);
$frm_v_gen					->	execute();

$reg   						=  array('registration_number'=>$reg,'registration_date'=>$reg_date,'clra_qr_code' =>$clra_qr_code);
$reg_update  				=  db_update('l_clra_registration_master');
$reg_update					->	fields($reg);
$reg_update					->	condition('user_id',$_POST['user_id']);
$reg_update					->	condition('id',$_POST['var_id']);
$reg_update					->	execute();	
	
	
		
	}
drupal_goto('alc-visible-applications/'.encryption_decryption_fun('encrypt', trim($_POST['var_id'])).'/'.encryption_decryption_fun('encrypt', trim($_POST['user_id'])));
 	
}



if(trim($_POST['clr_alc_submit'])=='Submit'){
	if($_POST['alc_clra'] != '' ){
		$status_remark 	= trim($_POST['alc_clra'])	;
		$status   	=  array('status'=>$status_remark);
		
	 $uid = $user->uid;
	
	$user_type_query="select usr_id,usr_type,fullname,mobile,officenumber from l_custom_user_detail where usr_id='".$uid."'";

	$user_type_result=db_query($user_type_query); 
	if($user_type_result->rowcount()>0){
	foreach($user_type_result as $dataUser) {
			$fullname =trim($dataUser->fullname);
	
	
		
	}
}
if( trim($_POST['alc_clra']) == 'C' ){
	
		$remarks = trim($_POST['clra_alc_remarks']).' on '.trim($_POST['clra_alc_call_time']);
}else{
	$remarks = trim($_POST['clra_alc_remarks']);
}
if(empty($remarks)){
	$message= drupal_set_message_custom(t('Remark must be filled out'),'error');//-------------miscellaneous module
echo $message;
	
}
$remark_field_text = trim($_POST['field_name']);
$remark_details  =  array(
				'remarks'   		=> $remarks,
				'remark_by'   		=> trim($uid), 
				'remark_to' 		=> trim($_POST['user_id']),
				'application_id' 	=> trim($_POST['var_id']),
				'remark_date'  		=> time(),
				'remark_type' 		=> $status_remark,
				'remark_by_name'    => $fullname,
				'remark_field_text' =>	$remark_field_text,
				'remark_by_role' 	=>'4'
	);
db_insert('l_remarks_master')->fields($remark_details)->execute();

$QryTestInfoUpdt 	 =  db_update('l_clra_registration_master');
			$QryTestInfoUpdt	->	fields($status);
			$QryTestInfoUpdt	->	condition('id',$_POST['var_id']);
			$QryTestInfoUpdt	->	execute();
		
if( trim($_POST['alc_clra']) == 'I' ){
			
$redistration_number=$_POST['registration_number'];
$target_dir = 'upload/alc_upload_certificates_clra/';
$extension = end(explode(".", $_FILES["fileToUpload"]["name"]));

$target_file=$target_dir;


$destination =$target_file;


  $swap_file = file_save_upload('clra_signed_certificate', array(), file_build_uri($target_file), $replace=FILE_EXISTS_RENAME);  

if ($file = file_save_upload('clra_signed_certificate',$replace = FILE_EXISTS_RENAME))
   {
	  
	    $uri=trim($file->uri);
		
      	$file->status = FILE_STATUS_PERMANENT;
		file_save($file);
   		$certificates_fid=$file->fid;
     	$certificates_file_name=$file->filename;
   
 
   }



$file_uplod_array  					=  array('certificates_fid'=>$certificates_fid);
$file_uplod_update  				=  db_update('l_clra_registration_master');
$file_uplod_update					->	fields($file_uplod_array);
$file_uplod_update					->	condition('user_id',$_POST['user_id']);
$file_uplod_update					->	condition('id',$_POST['var_id']);
$file_uplod_update					->	execute();	
	

			
}

drupal_goto('alc-visible-applications/'.encryption_decryption_fun('encrypt', trim($_POST['var_id'])).'/'.encryption_decryption_fun('encrypt', trim($_POST['user_id'])));
}
}
?>

 

<link rel="stylesheet" type="text/css" href="<?php echo $base_root.$base_path;?>sites/all/modules/alc/js/jquery.datetimepicker.css" >

<!--<script  type="text/javascript" src="<?php // echo $base_root.$base_path;?>sites/all/modules/alc/js/jquery.js"></script>-->

<script  type="text/javascript"  src="<?php echo $base_root.$base_path;?>sites/all/modules/alc/js/jquery.datetimepicker.js"></script>

<script type="text/javascript">

jQuery(document).ready(function() {		
	jQuery("#back_for_correction_alc").click(function(e){		
		var field_name = jQuery("#field_name").val();
		if(confirm('want to continue')){ 
			jQuery("#chech_data").val(field_name);
		}
	});
	
	
	jQuery(".viewfile_popup").click(function(e){	 
		var id = jQuery(this).attr('id');
        e.preventDefault(); // this will prevent the browser to redirect to the href
		jQuery( "#view_"+id ).dialog();
	});
	
});





jQuery(document).ready(function(){
	


	jQuery(".verified_alc_clra_yn").click(function(){
		
		var fieldIdt		 =	jQuery("input[name=field_name]").val();
		if (jQuery(this).is(':checked')) {	
					
			var fieldId 	= jQuery(this).attr('id');
				
			if (fieldIdt == ''){
				var fieldIdt 	= fieldId;
			}else{
				var fieldIdt 	= fieldIdt+','+fieldId;
			}
			
			
			jQuery("input[name=field_name]").val(fieldIdt);
		}
		
		var field_name = jQuery("input[name=field_name]").val();
		
		if (!jQuery(this).is(':checked')) {	
		
				var fieldIdt = '';			
				var fieldId = jQuery(this).attr('id');
								
				//jQuery("#editable_fields_license").val('');
					
				var field_name_arr = field_name.split(',');
				
				for(var i=0;i<field_name_arr.length;i++){
										
        			if(field_name_arr[i] != fieldId){
						
						var fieldIdt = fieldIdt+','+field_name_arr[i];
						fieldIdt = fieldIdt.replace(',,',',');
						
						
						jQuery("input[name=field_name]").val(fieldIdt);
					
						var t		 =	jQuery("input[name=field_name]").val();
						
						if(t==','){
						
						jQuery("input[name=field_name]").val(null);
						}
					}
   				}
    			
			}	
		
		
	});



	// jQuery("#clra_alc_call_time_id").datetimepicker();		
	var action = jQuery("#alc_clra_id").val(); 
		 
	var clra_qr_code=jQuery("input[name=clra_qr_code]").val();
	if(action == 'I'){
		if(clra_qr_code==""){
			document.getElementById('genarate_pdf').style.display = 'block';
			document.getElementById('action_call_time').style.display = 'none';
			document.getElementById('action_div').style.display = 'none';
			document.getElementById('submit_remark').style.display = 'none'; 
		}else{
			document.getElementById('action_div').style.display = 'block'; 
			document.getElementById('genarate_pdf').style.display = 'none';
			document.getElementById('action_call_time').style.display = 'none';
			document.getElementById('submit_remark').style.display = 'block';
		}
	}
});
</script>

<?php
 drupal_add_js(drupal_get_path('module', 'alc').'/js/view_doc_js.js');


$get_remark = db_select('l_remarks_master', 'lrm');
$get_remark->fields('lrm', array('remark_field_text','remark_by_role'));
$get_remark->condition('lrm.remark_to', $userid);
$get_remark->condition('lrm.application_id', $var_id);
$get_remark->condition('lrm.remark_by', $userid, '!=');
$get_remark->condition('lrm.is_active', 1);
$get_remark->orderBy('lrm.remark_id','DESC');
$get_remark->range(0, 1);
$get_remark_result = $get_remark->execute();

$editable_data = $get_remark_result->fetchAssoc();

$remark_by_role=$editable_data['remark_by_role'];



$remark_field_text_arr = explode(',', $editable_data['remark_field_text']);

for($i=0; $i<count($remark_field_text_arr); $i++){
	$fieldname = $remark_field_text_arr[$i];
		
	if($fieldname == 'e_name'){ $e_name_ck = "checked='checked'";}
	if($fieldname == 'loc_e_name'){ $loc_e_name_ck = "checked='checked'";}
	if($fieldname == 'e_postal_address'){ $e_postal_address_ck = "checked='checked'";}	
	if($fieldname == 'full_name_principal_emp'){ $full_name_principal_emp_ck = "checked='checked'";}
	if($fieldname == 'address_principal_emp'){ $address_principal_emp_ck = "checked='checked'";}	
	if($fieldname == 'full_name_manager'){ $full_name_manager_ck = "checked='checked'";}	
	if($fieldname == 'address_manager'){ $address_manager_ck = "checked='checked'";}	
	if($fieldname == 'e_nature_of_work'){ $e_nature_of_work_ck = "checked='checked'";}	
	if($fieldname == 'e_any_day_max_num_of_workmen'){ $e_any_day_max_num_of_workmen_ck = "checked='checked'";}	
	if($fieldname == 'e_num_of_workmen_per_or_reg'){ $e_num_of_workmen_per_or_reg_ck = "checked='checked'";}	
	if($fieldname == 'e_num_of_workmen_temp_or_reg'){ $e_num_of_workmen_temp_or_reg_ck = "checked='checked'";}	
	if($fieldname == 'workmen_if_same_similar_kind_of_work'){ $workmen_if_same_similar_kind_of_work_ck = "checked='checked'";}
	if($fieldname == 'con_lab_job_desc'){ $con_lab_job_desc_ck = "checked='checked'";}	
	if($fieldname == 'con_lab_wage_rate_other_benefits'){ $con_lab_wage_rate_other_benefits_ck = "checked='checked'";}	
	if($fieldname == 'con_lab_cat_desig_nom'){ $con_lab_cat_desig_nom_ck = "checked='checked'";}	
	if($fieldname == 'e_trade_union_name'){ $e_trade_union_name_ck = "checked='checked'";}
	if($fieldname == 'e_trade_union_address'){ $e_trade_union_address_ck = "checked='checked'";}	
	if($fieldname == 'e_settlement_award_judgement_min_wage'){ $e_settlement_award_judgement_min_wage_ck = "checked='checked'";}
	
	if($fieldname == 'backlog_clra_registration_certificate_file'){ $backlog_clra_registration_certificate_file_ck = "checked='checked'";}
	if($fieldname == 'form_1_clra_signed_pdf_file'){ $form_1_clra_signed_pdf_file_ck = "checked='checked'";}
	if($fieldname == 'max_num_wrkmen'){ $max_num_wrkmen_ck = "checked='checked'";}
	if($fieldname == 'trade_license_file'){ $trade_license_file_ck = "checked='checked'";}
	if($fieldname == 'article_of_assoc_file'){ $article_of_assoc_file_ck = "checked='checked'";}
	if($fieldname == 'memorandum_of_cert_file'){ $memorandum_of_cert_file_ck = "checked='checked'";}
	if($fieldname == 'partnership_deed_file'){ $partnership_deed_file_ck = "checked='checked'";}
	if($fieldname == 'factory_license_file'){ $factory_license_file_ck = "checked='checked'";}
	
	
	
	
}

$establishment_address 	= miscellaneous_clra_pe_location_address($var_id);
$postal_address 		= miscellaneous_postal_add_pe($var_id);
$pe_address 			= miscellaneous_clra_pe_address($var_id);
$manager_address	 	= miscellaneous_manager_pe($var_id);

?>


<table width="100%" border="0" class="view-act-rules-table">
    <tr>
		<td colspan="3" valign="middle" style="text-align:center">NOTE:- All inputs are provided by Principal Employer <br/><input type="checkbox" id="check_symbol" class="note check2" checked="checked" /><label class="label2" for="check_symbol"></label> Verified Inputs need to be checked &nbsp;&nbsp; <input type="checkbox" id="uncheck_symbol" class="note check2" /><label class="label2" for="uncheck_symbol"></label>&nbsp;&nbsp;Inputs need to be Rectified keep it blank </td>    
	</tr>
    <tr>
    	<th width="42%"><strong>Parameters</strong></th>
    	<th width="45%"><strong>Inputs</strong></th>
     	<th align="center"><strong>Verified?</strong></th>
    </tr>

<?php
if(!empty($content['backlog_id'])){
	$get_bklg_tbl = db_select('l_clra_principle_emp_backlog_data', 'lcpebd');
	$get_bklg_tbl->fields('lcpebd', array('registration_no', 'registration_date', 'fees'));
	$get_bklg_tbl->condition('lcpebd.id', $content['backlog_id']);
													
	$get_bklg_tbl_res =	$get_bklg_tbl->execute();
	$get_bklg_tbl_res_data = $get_bklg_tbl_res->fetchObject();
?>
<tr>
	<td>Old Registration number/date</td>
    <td align="center"><?php echo 'Reg. No.'.$get_bklg_tbl_res_data->registration_no.'<br>Date: '.date('d/m/Y',strtotime($get_bklg_tbl_res_data->registration_date));?></td>
    <td align="center">
    	<input type="checkbox" id="e_name_bl" class="verified_alc_clra_yn check2" checked="checked" disabled="disabled" />
        <label class="label2" for="e_name_bl"></label>
     </td>
</tr>
<?php
}
?>
<tr>
	<td colspan="3"><font color="#7d5d02"><center><strong>1. Full Name and address of the Principal Employer</strong></center></font></td>
</tr>
<tr>
    <td>Full Name of the principal employer </td>
    <td><?php echo $content['full_name_principal_emp'];?></td>
    <td align="center"><input type="checkbox" id="full_name_principal_emp" class="verified_alc_clra_yn check2"  <?php echo $full_name_principal_emp_ck; ?> />
    <label class="label2" for="full_name_principal_emp"></label>
    </td>
</tr>

<tr>
    <td>Address Of the principal employer </td>
    <td><?php echo $content['address_principal_emp'].'<br/>'.$pe_address;?></td>
    <td align="center"><input type="checkbox" id="address_principal_emp" class="verified_alc_clra_yn check2" <?php echo $address_principal_emp_ck;?> />
    <label class="label2" for="address_principal_emp"></label>
    </td>
</tr>
<tr>
	<td colspan="3"><font color="#7d5d02"><center><strong>2. Full name of the Manager or Person Responsible for the Supervision and control of the Establishment</strong></center></font></td>
</tr>
<tr>
    <td>Full name of the manager or person responsible for the supervision and control of the establishment </td>
    <td><?php echo $content['full_name_manager'];?></td>
    <td align="center"><input type="checkbox" id="full_name_manager" class="verified_alc_clra_yn check2"  <?php echo $full_name_manager_ck; ?> />
    <label class="label2" for="full_name_manager"></label>
    </td>
</tr>
<tr>
	<td>Address of the manager or person responsible for the supervision  and control of the establishment </td>
	<td><?php echo $content['address_manager'].'<br/>'.$manager_address;?></td>
  	<td align="center"><input type="checkbox" id="address_manager" class="verified_alc_clra_yn check2" <?php echo $address_manager_ck;?> />
    <label class="label2" for="address_manager"></label>
    </td>
</tr>
<tr>
	<td colspan="3"><font color="#7d5d02"><center><strong>3. Name and location of the Establishment</strong></center></font></td>
</tr>

<tr>
    <td >Name of the Establishment </td>
    <td><?php echo $content['e_name'];?></td>
    <td align="center">
    	<input type="checkbox" id="e_name" class="verified_alc_clra_yn check2"  <?php echo $e_name_ck; ?> /> 
    	<label class="label2" for="e_name"></label>
    </td> 
</tr>

<tr>
	<td>Location of the establishment</td>
	<td><?php echo $content['loc_e_name'].'<br/>'.$establishment_address;?></td>
 	<td align="center">
    	<input type="checkbox" id="loc_e_name" class="verified_alc_clra_yn check2" <?php echo $loc_e_name_ck;?> />
    	<label class="label2" for="loc_e_name"></label>
    </td>
</tr>
<tr>
	<td colspan="3"><font color="#7d5d02"><center><strong>4. Registered Office address of the Establishment</strong></center></font></td>
</tr>

<tr>
    <td>Postal address of the establishment </td>
    <td> <?php echo $content['e_postal_address'].'<br/>'.$postal_address;?></td>
    <td align="center"><input type="checkbox" id="e_postal_address" class="verified_alc_clra_yn check2" <?php echo $e_postal_address_ck;?> />
    <label class="label2" for="e_postal_address"></label>
    </td>
</tr>
<tr>
	<td colspan="3"><font color="#7d5d02"><center><strong>5. Nature of Work Carried on in the Establishment</strong></center></font></td>
</tr>
<tr>
    <td>5. Nature of work carried on in the establishment </td>
    <td><?php echo $param_work;?></td>
    <td align="center"><input type="checkbox" id="e_nature_of_work" class="verified_alc_clra_yn check2" <?php echo $e_nature_of_work_ck;?> />
    <label class="label2" for="e_nature_of_work"></label>
    </td>
</tr>
<tr>
	<td>5.a) Maximum Number of workmen employed directly on any day in the establishment</td>
   <td><?php echo $content['max_num_wrkmen'];?></td>
   <td align="center"><input type="checkbox" id="max_num_wrkmen" class="verified_alc_clra_yn check2" <?php echo $max_num_wrkmen_ck;?> />
   <label class="label2" for="max_num_wrkmen"></label>
   </td>
</tr>
<tr>
    <td>5.b) Number of workmen engaged as permanent/regular Workmen </td>
    <td><?php echo $content['e_num_of_workmen_per_or_reg'];?></td>
    <td align="center"><input type="checkbox" id="e_num_of_workmen_per_or_reg" class="verified_alc_clra_yn check2" <?php echo $e_num_of_workmen_per_or_reg_ck;?> />
    <label class="label2" for="e_num_of_workmen_per_or_reg"></label>
    </td>
</tr>
<tr>
    <td>5.c) Number of workmen engaged as temporary/regular workmen </td>
    <td><?php echo $content['e_num_of_workmen_temp_or_reg'];?></td>
    <td align="center"><input type="checkbox" id="e_num_of_workmen_temp_or_reg" class="verified_alc_clra_yn check2" <?php echo $e_num_of_workmen_temp_or_reg_ck;?>  />
    <label class="label2" for="e_num_of_workmen_temp_or_reg"></label>
    </td>
</tr>
<tr>
    <td>5.d) Whether the workmen employed/intended to be employment by the contractor perform the same or similar kind of work as the workmen employed directly by the principal employer (if yes, please give here information as detailed below) </td>
    <td><?php if($content['workmen_if_same_similar_kind_of_work']=="0"){ echo "No";}else{ echo "Yes";} ?>
    </td>
    <td align="center"><input type="checkbox" id="workmen_if_same_similar_kind_of_work" class="verified_alc_clra_yn check2" <?php echo $workmen_if_same_similar_kind_of_work_ck;?> />
    <label class="label2" for="workmen_if_same_similar_kind_of_work"></label>
    </td>
</tr>
<tr>
    <td>5.d.i)  A complete job description of the contract labour </td>
    <td><?php echo $content['con_lab_job_desc'];?></td>
    <td align="center"><input type="checkbox" id="con_lab_job_desc" class="verified_alc_clra_yn check2" <?php echo $con_lab_job_desc_ck;?> />
    <label class="label2" for="con_lab_job_desc"></label>
    </td>
</tr>
<tr>
    <td>5.d.ii)  Wage rates and other cash benefits paid/to be paid </td>
    <td><?php echo $content['con_lab_wage_rate_other_benefits'];?></td>
    <td align="center"><input type="checkbox" id="con_lab_wage_rate_other_benefits" class="verified_alc_clra_yn check2" <?php echo $con_lab_wage_rate_other_benefits_ck;?> />
    <label class="label2" for="con_lab_wage_rate_other_benefits"></label>
    </td>
</tr>
<tr>
    <td>5.d.iii) Category/designation/nomenclature of the job </td>
    <td><?php echo $content['con_lab_cat_desig_nom'];?></td>
    <td align="center"><input type="checkbox" id="con_lab_cat_desig_nom" class="verified_alc_clra_yn check2" <?php echo $con_lab_cat_desig_nom_ck;?> />
    <label class="label2" for="con_lab_cat_desig_nom"></label>
    </td>
</tr>
<tr>
<td>5.e) Settlement or award or judgement or minimum wages (if any applicable in the establishment) </td>
<td><?php echo $content['e_settlement_award_judgement_min_wage'];?></td>
 <td align="center"><input type="checkbox" id="e_settlement_award_judgement_min_wage" class="verified_alc_clra_yn check2" <?php echo $e_settlement_award_judgement_min_wage_ck;?> />
 <label class="label2" for="e_settlement_award_judgement_min_wage"></label>
 </td>
</tr>
<tr>
    <td>6.) Maximum number of contract labour to be employed on any day through each contractor </td>
    <td><?php echo $content['e_any_day_max_num_of_workmen'].'&nbsp;<font color="#006600">[FEES IS CALCULATED BASED ON THIS VALUE ]</font>';?></td>
    <td align="center"><input type="checkbox" id="e_any_day_max_num_of_workmen" class="verified_alc_clra_yn check2" <?php echo $e_any_day_max_num_of_workmen_ck;?> />
    <label class="label2" for="e_any_day_max_num_of_workmen"></label>
    </td>
</tr>

<tr>
    <td colspan="3" align="center"><strong>Documents uploaded</strong></td>
  </tr>
  <?php
 
	$doc_id='1'  ;
	$file='file_'.$doc_id;
    foreach($content_docs as $docs){
	 if(!empty($docs->trade_license_file)){
	?>
  <tr>
    <td>Trade license</td>
    <td>
    		<a target="_blank" href="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/trade_license/<?php echo $docs->trade_license_file; ?>">
    			<img title="Click here to view &amp; print Trade license" alt="trade-license" src="<?php echo $GLOBALS['base_url']; ?>/<?php echo drupal_get_path('theme', 'jackson');?>/images/pdf.png"></a>
    
    <span class="viewfile_popup" id="trade_license"><img src="<?php echo $GLOBALS['base_url']; ?>/<?php echo drupal_get_path('theme', 'jackson');?>/images/zoom.png" title="Click here to view Trade license" alt="trade-license"></span>
    
    <div id="view_trade_license"  title="Trade License" style="display:none;"><iframe src="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/trade_license/<?php echo $docs->trade_license_file;?>" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>
    </td>
     <td align="center">
     	<input type="checkbox" id="trade_license_file" class="verified_alc_clra_yn check2" <?php echo $trade_license_file_ck;?> />
        <label class="label2" for="trade_license_file"></label>
     </td>
  
  </tr>
  <?php }else{?>
	  
	   <tr>
   <td>Trade license</td>
    <td>No document uploaded</td>
   <td align="center">
   		<input type="checkbox" id="trade_license_file" class="verified_alc_clra_yn check2" <?php echo $trade_license_file_ck;?> />
        <label class="label2" for="trade_license_file"></label>
   </td>
  </tr>
	  
	  
  <?php }
  
  
  if(!empty($docs->article_of_assoc_file)){
  $doc_id=  $doc_id+1; 
  $file='file_'.$doc_id;
  ?>
    <tr>
    <td>Articles of Association and Memorandum of Association/Partnership Deed</td>
    <td><a target="_blank" title="Click here to download documents" href="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/article_of_assoc/<?php echo $docs->article_of_assoc_file; ?>"><img src="<?php echo $GLOBALS['base_url']; ?>/<?php echo drupal_get_path('theme', 'jackson');?>/images/pdf.png"></a>
    
    <span class="viewfile_popup" id="article_of_assoc_file_1"><img src="<?php echo $GLOBALS['base_url']; ?>/<?php echo drupal_get_path('theme', 'jackson');?>/images/zoom.png" title="Click here to view Articles of Association and Memorandum of Association/Partnership Deed" alt="trade-license"></span>
    
    <div id="view_article_of_assoc_file_1"  title="Articles of Association and Memorandum of Association/Partnership Deed"  style="display:none;"><iframe src="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/article_of_assoc/<?php echo $docs->article_of_assoc_file;?>" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div></td>
     <td align="center">
     	<input type="checkbox" id="article_of_assoc_file" class="verified_alc_clra_yn check2" <?php echo $article_of_assoc_file_ck;?> />
        <label class="label2" for="article_of_assoc_file"></label>
     </td>
  
  </tr>
  
  <?php  }else { ?>
	  
	 <tr>
   <td>Articles of Association and Memorandum of Association/Partnership Deed</td>
    <td>No document uploaded</td>
   <td align="center">
   		<input type="checkbox" id="article_of_assoc_file" class="verified_alc_clra_yn check2" <?php echo $article_of_assoc_file_ck;?> />
        <label class="label2" for="article_of_assoc_file"></label>
  </td>
  </tr> 
 <?php  }
 
 
 if(!empty($docs->memorandum_of_cert_file)){
  $doc_id=  $doc_id+1; 
  $file='file_'.$doc_id;
  ?>
 <tr>
    <td>Any other document in support of correctness of the particulars mentioned in the application if required</td>
    <td><a target="_blank" title="Click here to download documents." href="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/memorandum_of_cert/<?php echo $docs->memorandum_of_cert_file; ?>"><img src="<?php echo $GLOBALS['base_url']; ?>/<?php echo drupal_get_path('theme', 'jackson');?>/images/pdf.png"></a>
    
    <span class="viewfile_popup" id="abcd"  title="Click here to view document in support of correctness of the application"><img src="<?php echo $GLOBALS['base_url']; ?>/<?php echo drupal_get_path('theme', 'jackson');?>/images/zoom.png"></span>
    
    <div id="view_abcd" style="display:none;"  title="Any other document in support of correctness of the particulars mentioned in the application if required"><iframe src="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/memorandum_of_cert/<?php echo $docs->memorandum_of_cert_file; ?>" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div></td>
    
   <td align="center">
   		<input type="checkbox" id="memorandum_of_cert_file" class="verified_alc_clra_yn check2" <?php echo $memorandum_of_cert_file_ck;?> />
        <label class="label2" for="memorandum_of_cert_file"></label>
   </td>
  
  </tr>
  
   
  <?php }else{ ?>
  
   <tr>
   <td>Any other document in support of correctness of the particulars mentioned in the application if required</td>
    <td>No document uploaded</td>
   <td align="center">
   		<input type="checkbox" id="memorandum_of_cert_file" class="verified_alc_clra_yn check2" <?php echo $memorandum_of_cert_file_ck;?> />
        <label class="label2" for="memorandum_of_cert_file"></label>
   </td>
  </tr> 
  
  <?php }  
  
  
  if(!empty($docs->partnership_deed_file)){
  $doc_id=  $doc_id+1; 
  $file='file_'.$doc_id;
  ?>
  
  <tr>
    <td>Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc.</td>
    <td><a target="_blank" title="Click here to download documents." href="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/partnership_deed/<?php echo $docs->partnership_deed_file; ?>"><img src="<?php echo $GLOBALS['base_url']; ?>/<?php echo drupal_get_path('theme', 'jackson');?>/images/pdf.png"></a>
    
    <span class="viewfile_popup" id="abcd"  title="Click here to view other certificates of registration"><img src="<?php echo $GLOBALS['base_url']; ?>/<?php echo drupal_get_path('theme', 'jackson');?>/images/zoom.png"></span>    
    <div id="view_abcd" style="display:none;" title="Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc."><iframe src="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/partnership_deed/<?php echo $docs->partnership_deed_file; ?>" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div></td>
   <td align="center">
   		<input type="checkbox" id="partnership_deed_file" class="verified_alc_clra_yn check2" <?php echo $partnership_deed_file_ck;?> />
        <label class="label2" for="partnership_deed_file"></label>
  </td>
  
  </tr>
 
 
  
  <?php } else{ ?>
	  <tr>
   <td>Other certificates of registration in case of other than company, proprietorship or partnership firm like cooperative, Trustees etc.</td>
    <td>No document uploaded</td>
   <td align="center">
   		<input type="checkbox" id="partnership_deed_file" class="verified_alc_clra_yn check2" <?php echo $partnership_deed_file_ck;?> />
        <label class="label2" for="partnership_deed_file"></label>
   </td>
  </tr>
  <?php	  
  } 
   
   if(!empty($docs->factory_license_file)){
  $doc_id=  $doc_id+1; 
  $file='file_'.$doc_id;
  ?>
  
  <tr>
    <td>Factory License if any</td>
    <td>    
    <a target="_blank" title="Click here to download documents." href="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/FactoryLicense/<?php echo $docs->factory_license_file;?>"><img src="<?php echo $GLOBALS['base_url']; ?>/<?php echo drupal_get_path('theme', 'jackson');?>/images/pdf.png"></a>
    
    <span class="viewfile_popup" id="factory_license_file_pdf" title="Click here to view Factory License"><img src="<?php echo $GLOBALS['base_url']; ?>/<?php echo drupal_get_path('theme', 'jackson');?>/images/zoom.png"></span>
    
    <div id="view_factory_license_file_pdf" style="display:none;" title="Factory License" ><iframe src="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/FactoryLicense/<?php echo $docs->factory_license_file;?>" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>
    
    
    </td>
   <td align="center">
   		<input type="checkbox" id="factory_license_file" class="verified_alc_clra_yn check2" <?php echo $factory_license_file_ck;?> />
        <label class="label2" for="factory_license_file"></label>
   </td>
  
  </tr>
 
 
  
  <?php } else{?>
 <tr>
   <td>Factory License if any</td>
    <td>No document uploaded</td>
   <td align="center">
   		<input type="checkbox" id="factory_license_file" class="verified_alc_clra_yn check2" <?php echo $factory_license_file_ck;?> />
        <label class="label2" for="factory_license_file"></label>
   </td>
  </tr>
   <?php }
   
   if(!empty($docs->form_1_clra_signed_pdf_file)){
	  $doc_id=  $doc_id+1; 
      $file='file_'.$doc_id;
  ?>

    <tr> 
  <td>FORM-I</td>
    <td>
    <a target="_blank" title="Click here to signed form-I document." href="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/FORM-I/<?php echo $docs->form_1_clra_signed_pdf_file; ?>"><img src="<?php echo $GLOBALS['base_url']; ?>/<?php echo drupal_get_path('theme', 'jackson');?>/images/pdf.png"></a>
     
    <span class="viewfile_popup" id="form_1_clra_signed_pdf_file_1" title="Click here to view FORM-I"><img src="<?php echo $GLOBALS['base_url']; ?>/<?php echo drupal_get_path('theme', 'jackson');?>/images/zoom.png"></span>
    
    <div id="view_form_1_clra_signed_pdf_file_1" style="display:none;" title="FORM-I"><iframe src="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/FORM-I/<?php echo $docs->form_1_clra_signed_pdf_file;?>" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>&nbsp;&nbsp;
    
    <?php echo l(t('Generated FORM - I'),'generate-pdf-applicant/'.encryption_decryption_fun('encrypt', $content['id']).'/'.encryption_decryption_fun('encrypt', $content['user_id']), array('html' => TRUE,'attributes'=> array('target'=>'_blank'))); ?>
   
    
    </td>
   <td align="center">
   		<input type="checkbox" id="form_1_clra_signed_pdf_file" class="verified_alc_clra_yn check2" <?php echo $form_1_clra_signed_pdf_file_ck;?> />
        <label class="label2" for="form_1_clra_signed_pdf_file"></label>
   </td>
  
 </tr>
 <?php }else{?>
 <tr> 
  <td>FORM-I</td>
 <td>Form I will be uploaded after fees payment</td>
 <td align="center">
 	<input type="checkbox" id="form_1_clra_signed_pdf_file" class="verified_alc_clra_yn check2" <?php echo $form_1_clra_signed_pdf_file_ck;?> />
    <label class="label2" for="form_1_clra_signed_pdf_file"></label>
 </td>
 </tr>
 <?php }?> 


<?php 
if(!empty($docs->backlog_clra_registration_certificate_file)){
	  $doc_id=  $doc_id+1; 
  	   $file='file_'.$doc_id;
?>
<tr>
    <td>Previous Registration Certificate</td>
    <td><a target="_blank" title="Click here to view old registration certificate." href="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/backlog_clra_registration_certificate/<?php echo $docs->backlog_clra_registration_certificate_file; ?>"><img src="<?php echo $GLOBALS['base_url']; ?>/<?php echo drupal_get_path('theme', 'jackson');?>/images/pdf.png"></a>
    
    
   <span class="viewfile_popup" id="backlog_clra_registration_certificate_file_1" title="Click here to view Previous Registration Certificate"><img src="<?php echo $GLOBALS['base_url']; ?>/<?php echo drupal_get_path('theme', 'jackson');?>/images/zoom.png"></span>
    
    <div id="view_backlog_clra_registration_certificate_file_1" style="display:none;" title="Previous Registration Certificate"><iframe src="<?php echo  $GLOBALS['base_url'];?>/sites/default/files/upload/backlog_clra_registration_certificate/<?php echo $docs->backlog_clra_registration_certificate_file;?>" style="min-width:600px; min-height:600px;" frameborder="0"></iframe></div>
    
    </td>
   <td align="center">
   		<input type="checkbox" id="backlog_clra_registration_certificate_file" class="verified_alc_clra_yn check2" <?php echo $backlog_clra_registration_certificate_file_ck;?> />
        <label class="label2" for="backlog_clra_registration_certificate_file"></label>
  </td>
  
 </tr>
 <?php }else{?>
 <tr>
 <td>Previous Registration Certificate</td>
 <td>For Previous Registered Applicant , this certificate should be uploaded</td> 
  <td align="center">
  	  <input type="checkbox" id="backlog_clra_registration_certificate_file" class="verified_alc_clra_yn check2" <?php echo $backlog_clra_registration_certificate_file_ck;?> />
      <label class="label2" for="backlog_clra_registration_certificate_file"></label>
  </td></tr>
 <?php 
 }
 }
 
 /* Payment Details Start */
 if($content['payment_mode'] == 'G'){	 
	 $govt_pay_query = db_select('l_govt_payment', 'lgp');
	 $govt_pay_query->fields('lgp', array());
	 $govt_pay_query->condition('lgp.application_id', $content['id']);
	 
	 $govt_pay_query_result = $govt_pay_query->execute();
	

	if($govt_pay_query_result->rowCount() > 0 ){		
		$govt_paydata = $govt_pay_query_result->fetchAssoc();
				
		$tv_number = 'Not available';
		$challan_number = 'Not available';
		$total_amount = 'Not available';
		$challan_date = 'Not available';
				
		if(!empty($govt_paydata['tv_number'])) $tv_number = $govt_paydata['tv_number'];
		if(!empty($govt_paydata['challan_number'])) $challan_number = $govt_paydata['challan_number'];
		if(!empty($govt_paydata['total_amount'])) $total_amount = $govt_paydata['total_amount'];
		if(!empty($govt_paydata['challan_date'])) $challan_date = $govt_paydata['challan_date'];
	}
	
	 $payment_details = '<u>Government Payment[Head to Head transfer through treasury]</u><br>';
	 $payment_details .= 'Transaction Voucher Number:<span class="color_orange">'.$tv_number.'</span><br>';
	 $payment_details .= 'Challan Number:<span class="color_orange">'.$challan_number.'</span><br>';
	 $payment_details .= 'Challan Date:<span class="color_orange">'.date('dS M Y', strtotime($challan_date)).'</span><br>';
	 $payment_details .= 'Total amount:<span class="color_orange">'.number_format($total_amount,2).'</span><br>';
	 $payment_details .= 'Challan Details:'.l('<img src='.$GLOBALS['base_url'].'/'.drupal_get_path('theme', 'jackson').'/images/pdf.png>', '/sites/default/files/upload/challan_upload/'.$govt_paydata['challan_file'], array('html' => TRUE, 'title' => 'Click here to view challan'));
  }else if($content['payment_mode'] == 'O'){
	  $offline_pay_query = db_select('l_offline_payment_info', 'lopi');								
	  $offline_pay_query->fields('lopi', array('grn_number', 'approved_by'));
	  $offline_pay_query->condition('lopi.application_id', $content['id']);
	  $offline_pay_query->condition('lopi.act_id', 1);								
	  $offline_pay_query = $offline_pay_query->execute();
	
	  $offline_pay_result = $offline_pay_query->fetchObject();
	  
	  $payment_details = '<u>Manual Payment</u><br>';
	  $payment_details .= 'GRN NO.:<span class="color_orange">'.$offline_pay_result->grn_number.'&nbsp;'.l('Click here view details', 'https://wbfin.wb.nic.in/GRIPS/grn_status.do', array('attributes' => array('title' =>'Click here to view details', 'target' => '_blank'))).'</span><br>';
	  $payment_details .= 'Total Amount:<span class="color_orange"> &#8377; '.$content['finalfees'].'</span><br>';
	  $payment_details .= 'Challan:<span class="color_orange">No available</span><br>';	 
  }else{		
	 $transaction_details  			= db_select('l_principle_epayments_receive_data', 'lpd');
	 $transaction_details			->leftJoin('l_principle_epayments_data', 'lped', 'lped.identification_no = lpd.transaction_id');
	 $transaction_details			->fields('lped', array('identification_no', 'application_id'));
	 $transaction_details			->fields('lpd', array());
	 $transaction_details			->condition('lped.act_id', '1');
	 $transaction_details			->condition('lped.application_id', $content['id']);		
	 $trans_details_result 			= $transaction_details->execute();
	
	 $bank_code = 'Not available';
	 $bankTransactionID = 'Not available';
	 $total_amount = 'Not available';
	 $challan_fid_date = 'Not available';
	 $payment_status = 'Not available';
		
	 if($trans_details_result-> rowCount() > 0 ){			
		$transaction_det	= $trans_details_result->fetchAssoc();
		$bankTransactionID	= $transaction_det['transaction_id'];
		$grn_number			= $transaction_det['challanrefid'];
		$challan_fid_date	= !empty($transaction_det['challanrefid_date']) ? $transaction_det['challanrefid_date'] : '';
		$total_amount		= number_format($transaction_det['challanamount'], 2);
		$bank_code			= $transaction_det['bank_cd'];
		if($transaction_det['banktransactionstatus'] == 'Success') 
			$payment_status = '<span class="color_green">'.$transaction_det['banktransactionstatus'].'</span>';
	    }else{
			$payment_status = '<span class="color_red">'.$transaction_det['banktransactionstatus'].'</span>';
	    }
	
	 $payment_details = '<u>Grips Payment[Online/Counter]</u><br>';
	 $payment_details .= 'IFSC Code:<span class="color_orange">'.$bank_code.'</span><br>';
	 $payment_details .= 'Bank Transaction Id:<span class="color_orange">'.$bankTransactionID.'</span><br>';
	 $payment_details .= 'Total Amount:<span class="color_orange"> &#8377;'.$total_amount.'</span><br>';
	 $payment_details .= 'Transaction Date:<span class="color_orange">'.$challan_fid_date.'</span><br>';
	 $payment_details .= 'Transaction Status: '.$payment_status;
 }
 
 /* Payment Details End */ 
 
 if(!empty($content['backlog_id'])){
	 if($get_bklg_tbl_res_data->fees < $content['finalfees']){
		 $present_payble_amount = $content['finalfees'] - $get_bklg_tbl_res_data->fees;
	 }else{
		 $present_payble_amount = 0;
	 }
 }else{
	 $present_payble_amount = $content['finalfees'];
 }
 
 
 
 ?> 
 
 <tr>
 	<td>Payment Details<br /><span style="color:#f00; font-weight:700;">Payable Amount:<?php echo $present_payble_amount;?></span>
    <?php if(!empty($content['backlog_id'])){?>
    <br /><font color="#f00"><i>[As it's already manual registrar application so fees may be exempted.]</i></font>
    <?php } ?>
    
    </td>
 	<td colspan="2"><?php echo $payment_details;?></td>  	
  </tr>

</table>

<div class="contractor_details" ><strong>Trade Union Details</strong></div><!--style="padding:12px 0; text-align:center; font-size:15px;"-->
<div class="trade-union-view">
<table cellpadding="0" cellspacing="0" border="0" width="100%" class="view-act-rules-table">
<?php
if(!empty($param_trad)){
									$x = 0;	
									foreach($param_trad as $trade_union_data){ 
										$x++;
?>
							 
<tr>
	<td colspan="2" align="center" style="text-align:center;font-size:17px; padding-top: 8px;">
	<font color="#3366FF"><strong><?php echo $x;?>. Trade Union</strong></font></td>
</tr>
<tr>
<td>Registration of the Trade Union</td>
<td><?php echo $trade_union_data->e_trade_union_regn_no;?></td>
</tr>
<tr>
<td>Name of the Trade Union</td>
<td><?php echo $trade_union_data->e_trade_union_name;?></td>
</tr>
<tr>
<td>Address of the Trade Union</td>
<td><?php echo $trade_union_data->e_trade_union_address;?></td>
</tr>
<?php
}
}else{ 
?>
<tr><td colspan="2" style="text-align:center"> <center>No Trade Union Added</center></td></tr>
<?php } ?>
</table></div>
<div class="contractor_details" ><strong>Contractors Details</strong></div><!--style="padding:12px 0; text-align:center; font-size:15px;"-->
<div  class="contractors-view"><!--style="overflow-y:scroll;max-height:340px;"-->
<table width="100%" border="0" class="view-act-rules-table">
<?php
  	if(!empty($contractor_info)){
		$y = 0;	
		foreach($contractor_info as $delta_clra_cont => $datafetch_clra_cont){ 
			$y++;
			$nature_of_wrk = "";
			$prefix = '';
			foreach($contractor_nature_wrk as $key=>$val){
				if($datafetch_clra_cont->id == $key){ 
					foreach($val as $value){ 
						$nature_of_wrk .= $prefix.$value;
						$prefix = ', ';
					}
				} 
			}
			
			$contractor_info_address	= miscellaneous_contractor_info_address($application_id, $datafetch_clra_cont->id);
	
	
  ?>
  <tr>
    <td colspan="2" align="center" style="text-align:center;font-size:17px; padding-top: 8px;"><font color="#3366FF"><strong><?php echo $y;?> . CLRA Particulars of Contractors and Contract Labour<?php if($datafetch_clra_cont->status != 1) echo '&nbsp;<font color="#f00">[Inactive}</font>';?></strong></font></td>
</tr>
   
<tr>
    <td>Name of the Contractor</td>
    <td><?php echo $datafetch_clra_cont->name_of_contractor; ?></td>
</tr>
<tr>
    <td>Address of the Contractor</td>
    <td><?php echo $datafetch_clra_cont->address_of_contractor.'<br/>'.$contractor_info_address; ?></td>
</tr>
<tr>
    <td>Email of Contractor</td>
    <td><?php echo $datafetch_clra_cont->email_of_contractor; ?></td>
</tr>
<tr>
    <td>Nature of Work in which Contract Labour is Employed or is to be Employed</td>
    <td><?php echo $nature_of_wrk; ?></td>
</tr>
<tr>
    <td>Maximum Number of Contractor Labour to be Employed on any day Through Each Contractor</td>
    <td><?php echo $datafetch_clra_cont->contractor_max_no_of_labours_on_any_day; ?></td>
</tr> 
<tr>
    <td>Estimated Date of Employment of Each Contract Work Under Each Contractor</td>
    <td><?php echo date('dS M, Y',strtotime($datafetch_clra_cont->est_date_of_work_of_each_labour_from_date)).' to '. date('dS M, Y',strtotime($datafetch_clra_cont->est_date_of_work_of_each_labour_to_date)) ?></td>
</tr> 
<?php  } }else{
 ?>

   <tr><td colspan="2" style="text-align:center"> <center>Contractors Details Not Found</center></td></tr>



 <?php } ?>
 
</table>

</div>

<br />

<div style="height:20px;">
<?php
$action	 = 'encrypt';
$application_id_pre		= encryption_decryption_fun($action, $content['id']);
$applicant_user_id_pre	= encryption_decryption_fun($action, $content['user_id']);
$act_id	= encryption_decryption_fun($action, $content['act_id']);
?>
<i><?php echo l('Applications list', 'alc_receivedapplications', array('attributes' => array('class' => 'backapplication_view', 'title' => 'Click here back to applications list')))?></i>		
<i><?php echo l('View Applicant Profile', 'view-applicant-profile/'.$application_id_pre.'/'.$applicant_user_id_pre.'/'.$act_id, array('attributes' => array('class' => 'backapplication_view', 'title' => 'Click here view Applicant Profile ', 'target' => '_blank')))?></i>

<i><?php echo l('Instructions to give remarks ', $base_root.$base_path.'sites/default/files/instructions-remarks.pdf', array('attributes' => array('class' => 'backapplication_view', 'title' => 'Instructions to give remarks', 'target' => '_blank')))?></i> 

<i><?php echo l('How to digitally sign documents using USB Token', 'digitally-sign-process', array('attributes' => array('class' => 'backapplication_view right', 'title' => 'How to digitally sign your documents using USB Token', 'target' => '_blank')))?></i>
</div>

<?php
//print_r($content);die();
if($content['status'] == 'B'){
			$current_status = 'CURRENT STATUS: Application has been BACKED to Applicant for rectification';
			
		}
		else if($content['status'] == 'V'){
			$current_status = 'CURRENT STATUS: Application has been VERIFIED & GIVEN APPROVAL FOR FEES SUBMISSION';
			
		}
		else if($content['status'] == 'C'){
			$current_status = 'CURRENT STATUS: Applicant has been CALLED BY ALC';
			
		}
		else if($content['status'] == 'I'){
			$current_status = 'CURRENT STATUS:  CERTIFICATE has been ISSUED';
			
		}
		else if($content['status'] == 'F'){
			$current_status = 'CURRENT STATUS: Application has been FORWARDED TO ALC';
			
		}
		else if($content['status'] == 'T'){
			$current_status = 'CURRENT STATUS: TRANSACTION IS SUCCESSFULL for this Application';
			
		}
		else if($content['status'] == 'T' && ($remark_by_role == '4' ||$remark_by_role == '7') ){
			$current_status = 'CURRENT STATUS: FORM-I is BACKED for RECTIFICATION';
			
		}
		
		else if($content['status'] == 'R'){
			$current_status = 'CURRENT STATUS: Application is REJECTED';
			
		}
		
		else if($content['status'] == 'BI'){
			$current_status = 'CURRENT STATUS: Application has been BACKED TO INSPECTOR';
			
		}
		
		else if($content['status'] == 'S'){
			$current_status = 'CURRENT STATUS: Form-I is uploaded & Application has been FINALLY SUBMITTED';
			
		}
		
		else if($content['status'] == 'VA'){
			$current_status = 'Current status: Application has been verified &amp; approved &amp; does not require fees.';
			
		}
	

?>
<div class="comments" align="center"><?php echo $current_status; ?></div>
<div class="content">
	<div class="sky-form" style="width:100%;">
    	<header><h3><b>ACTIONS AND REMARK</b></h3></header>
		<div id="clra_alc">
			<div id="message">
			<?php
            	if(!empty($content['clra_qr_code'])&& empty($content['certificates_fid'])){
					
             		echo $message		= drupal_set_message_custom(t('CLRA Certificate has been successfully generated.Please Download it and Upload this Registration Certificate.Then click on submit button to complete this process'));//-------------miscellaneous module
					
           		 }else if (!empty($content['clra_qr_code'])&& (!empty($content['certificates_fid']))){
					 
                	echo $message='';
					
            	}else{
					
               	 	echo $message='';
            	}
            ?>
			</div>
		<?php if($content['status'] != 'V' && $content['status'] != 'I' && $content['status'] != 'C' && $content['status'] != 'R' && $content['status'] != 'S' && $content['status'] != 'B' && $content['status'] != 'T' && $content['status'] != 'VA'){?>
            <form name="clra_alc_action" id="clra_alc_action" action="" method="post" >
                   <label for="Actions">Please Select Actions  <font color="#FF0000">*</font></label>
                     <section><label class="select"><select name="alc_clra" id="alc_clra_id" required>
                            <option value="">Select process type</option>
                            <option value="B">Back for correction</option>
                            <option value="BI">Back to inspector</option>
                            <option value="R">Reject</option>
                            <?php if(!empty($content['backlog_id']) && $content['backlog_id']!=0) { ?>
                            <option value="VA">Verified & Approval without fees submission</option>
                            <option value="V">Verified & Approval for fees submission</option> 
                            <?php } else {?>
                            <option value="V">Verified & Approval for fees submission</option>
                             <?php } ?>
                                       
                        </select><br />
                       </label> </section>           
                         <label for="Remark">Remark <font color="#FF0000">*</font></label>
                         <section> <label class="textarea"><textarea name="clra_alc_remarks" id="clra_alc_remarks_id" rows="2" required></textarea></label> </section>
                          	<input type="hidden" size="300" id="field_name" name="field_name" value="<?php echo $editable_data['remark_field_text'];?>" />
                        	<input type="hidden" id="var_id" name="var_id" value="<?php echo $var_id; ?>" /> <!--var_id = application_id-->
                       		<input type="hidden" id="user_id" name="user_id" value="<?php echo $userid; ?>" />
                          <br />     
            
             <input type="submit" name="clr_alc_submit" value="Submit" id="clr_alc_submit_id" class="button" /> <br /><br/> <br /><br/>  
            
            </form>

<?php
}elseif($content['status'] == 'S' || $content['status'] == 'C'){
	
	
$remark_status	=	fetch_second_remarkrow($var_id); 
if($remark_status == 'V')
	$remark_status = 'T';
else
	$remark_status = $remark_status;


?>
<form name="clra_alc_action" id="clra_alc_action" action="" method="post"  enctype="multipart/form-data">
       <label for="Actions">Please Select Actions <font color="#FF0000">*</font></label>
       
    <?php     
   if(!empty($content['clra_qr_code'])&& empty($content['certificates_fid'])){
 ?>
    
       <section><label class="select"><select name="alc_clra" id="alc_clra_id" onchange="return alc_clra_action();" required>
            	<option value="">Select Process Type</option>
              
                <option value="C" >Call Applicant</option>
                <option value="I" selected="selected">Issue Certificate</option>              
                <option value="R" >Reject</option>
            </select>
           </label> </section>
       <?php }else if (!empty($content['clra_qr_code'])&& (!empty($content['certificates_fid']))){?> 
       
       <section><label class="select"><select name="alc_clra" id="alc_clra_id" onchange="return alc_clra_action();" required>
            	<option value="">Select Process Type</option>
                <option value="C" >Call Applicant</option>
                <option value="I" >Issue Certificate</option>
              
                <option value="R" >Reject</option>
            </select></label> </section> 
          <?php }else{?> 
           <section><label class="select"><select name="alc_clra" id="alc_clra_id" onchange="return alc_clra_action();" required>
            	<option value="">Select Process Type</option>
                  <option value=<?php echo $remark_status;?>>Back for correction</option>
                <option value="C" >Call Applicant</option>
                <option value="I" >Issue Certificate</option>
              
                <option value="R" >Reject</option>
            </select></label> </section> 
            <?php }?>
       
           <div id="genarate_pdf" style="display:none"> 
           		<input type="submit" name="clra_alc_gen_certificates" value="Generate Certificate" id="clra_alc_gen_certificates" class="button"  onclick="generate_certificates();" /> 
			</div> 
		 
			<?php
			if (!empty($content['clra_qr_code'])&& (!empty($content['registration_number']))){
				$display = 'block';
			}else{
				$display = 'none';
			}
			?>
            
          <div id="action_div" style="display:<?php echo $display;?>;">
            <a title="Click here to download Original Certificate" href="/download-pdf-clra-registration/<?php echo time();?>/<?php echo encryption_decryption_fun('encrypt', $var_id); ?>/<?php echo encryption_decryption_fun('encrypt', $userid); ?>" target="_blank">Download Certificate of CLRA
                <img src="<?php echo $GLOBALS['base_url']; ?>/<?php echo drupal_get_path('theme', 'jackson');?>/images/pdf.png"></a>[<font color="#FF0000">Please update RLO address from dashboard</font>]
                <label for="upload">Upload Certificates<font color="#FF0000">*</font></label>
                <div id="edit-clra-signed-certificate-upload" class=" form-managed-file">
                    <input id="edit-clra-signed-certificate-upload" class="form-file" type="file" size="22" name="files[clra_signed_certificate]">
                    <input type="hidden" value="0" name="clra_signed_certificate[fid]">
                </div>
           </div>
         
           <div id="action_call_time" style="display:none;">
           		<label for="call_time">Calling Date Time</label>
         		 <section> 
                 	<label class="input">
                    	<input type="text" name="clra_alc_call_time" id="clra_alc_call_time_id" />
                  	</label> 
                 </section>
           </div>
           
           <div id="submit_remark" style="display:block"> 
             	<label for="Remark">Remark <font color="#FF0000">*</font></label>
             		<section> 
                    	<label class="textarea">
                        	<textarea name="clra_alc_remarks" id="clra_alc_remarks_id" rows="2"></textarea>
                        </label> 
                    </section>
              		<input type="hidden" id="field_name" name="field_name" value="<?php echo $editable_data['remark_field_text'];?>" />
           			<input type="hidden" id="var_id" name="var_id" value="<?php echo $var_id; ?>" /> <!--var_id = application_id-->
            		<input type="hidden" id="user_id" name="user_id" value="<?php echo $userid; ?>" />
             		<input type="hidden" id="clra_qr_code" name="clra_qr_code" value="<?php echo $content['clra_qr_code']; ?>" />
             		<input type="hidden" id="registration_number" name="registration_number" value="<?php echo $content['registration_number']; ?>" />
              <br />           

			 <input type="submit" name="clr_alc_submit" value="Submit" id="clr_alc_submit_id" class="button" onclick=" return submit_form();" /> 
		</div> <br /><br/> <br /><br/> 
</form>


<?php
}?>

 

<?php

	
	
	$header = array(
		  array('data' => 'SL.NO.', 'field' => 'slno'),
		  array('data' => 'Remark', 'field' => 'remrk'),
		  array('data' => 'Remark On', 'field' => 'remark type'),
		  array('data' => 'DATE', 'field' => 'date'),
		  array('data' => 'Remark By', 'field' => 'licenseno'),
		  array('data' => 'Action')
	  );
	
	foreach($param_remark as $row){
			// get latest remark given by session user
			$actionItem = '';
			$applicant_user_id = '';
			
			// get payment info (checking  for data intsert into l_principle_epayments_data table)
			$getPayInfo				= db_query("SELECT COUNT(*) AS total_row FROM l_principle_epayments_data WHERE act_id=1 AND application_id = ".$row->application_id."")->fetchObject();
			$getId					= $getPayInfo->total_row;
			
			
			$getMaxData				= db_query("SELECT MAX(remark_id) AS MAXID FROM l_remarks_master WHERE is_active=1 and application_id = ".$row->application_id."")->fetchObject();
			$maxId					= $getMaxData->maxid;
			if($maxId>0){
				if(($row->remark_by==$user->uid) &&($maxId==$row->remark_id) && empty($getId)){
					$url 			= 'clra-remarks-delete/'.encryption_decryption_fun('encrypt',$row->remark_id).'/'.encryption_decryption_fun('encrypt',$row->application_id).'/'.encryption_decryption_fun('encrypt',$userid);			
					$actionItem			= l('<div id="delBtn" class="delete-btn active-del">'.t('Delete').'</div>',$url,array('html' => TRUE));	
				}else{
					$actionItem			= '<div id="delBtn" class="delete-btn disable-del">'.t('Delete').'</div>';
				}
			}
			
			$sl_no=$sl_no+1;
			
			if($row->remark_type=='B'){
				 $remak_type ='<span class="backed" title="Back for Rectification"></span>';
			}elseif($row->remark_type=='BI'){
				 $remak_type ='<span class="backtoins" title="Back to Inspector"></span>';	
			}elseif($row->remark_type=='V'){
				$remak_type ='<span class="feespending" title="Fees Pending"></span>';	
			}elseif($row->remark_type=='I'){
				$remak_type ='<span class="issued" title="Certificate Issued"></span>';
			}elseif($row->remark_type=='R'){
				$remak_type = '<span class="reject" title="Rejected"></span>';
			}elseif($row->remark_type=='F'){
				$remak_type ='<span class="forward" title="Forwarded"></span>';
			}elseif($row->remark_type=='C'){
				$remak_type ='<span class="callapplicant" title="Call Applicant"></span>';				
			}elseif($row->remark_type=='VA'){
				$remak_type ='<span class="approved" title="Approved"></span>';
			}elseif($row->remark_type=='T' && ($row->remark_by_role == '4' || $row->remark_by_role == '7')){
				  $remak_type ='<span class="backed" title="Back for Form-I Rectification"></span>';
			}
				$rows[] = array(
					array('data' => $sl_no, 'align' => 'left', 'class' => array('odd')),
					array('data' => nl2br($row->remarks), 'align' => 'left', 'class' => array('odd')),
					array('data' => $remak_type, 'align' => 'left', 'class' => array('odd')),
					array('data' =>date('dS M Y', $row->remark_date), 'align' => 'left', 'class' => array('odd')),
					array('data' =>$row->remark_by_name, 'align' => 'left', 'class' => array('odd')),
					array('data' =>$actionItem, 'align' => 'left', 'class' => array('odd'))		
				);
			
			
			
		}
		  $variables = array(
	  		'attributes' 		=> array('class' => array('view-act-rules-table')), 
	  		'header' 			=> $header,
	  		'rows'				=> $rows,
	  );

	
	$output = theme('datatable', $variables);
	
if($content['status'] != 'V' && $content['status'] != 'I' && $content['status'] != 'C' && $content['status'] != 'R' && $content['status'] != 'S' && $content['status'] != 'B' && $content['status'] != 'T'){
 echo $output.'</div>' ;


}else{
	if(!empty($content['certificates_fid'])){
		$upload_certificates_id 		= db_select('file_managed','fm');
		$upload_certificates_id			-> fields('fm',array());
		$upload_certificates_id			-> condition('fid',$content['certificates_fid']);
		
		$upload_certificates_file		= $upload_certificates_id-> execute()->fetchAssoc();
						
					
		  if(!empty($upload_certificates_file)){ 
			  $url									= explode('//',$upload_certificates_file['uri']);
			  $upload_certificates_file_url			= $url[1];
			  $upload_certificates_file_name		= $upload_certificates_file['filename'];
		  }
$download		=	'<b><font color="#0000FF">View Certificates</font> </b><a title="Click here to download Original Certificate" href="'.$GLOBALS['base_url'].'/sites/default/files/'.$upload_certificates_file_url.'" target="_blank"><img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/pdf.png" alt="'.$GLOBALS['base_url'].'"></a>';

//$download_1		=	'<b><font color="#0000FF">View Certificates</font> </b><a title="Click here to download Original Certificate" href="/'.$upload_certificates_file_url.'" target="_blank"><img src="'.$GLOBALS['base_url']."/".drupal_get_path('theme', 'jackson').'/images/pdf.png" ></a>';

		}
		$uri=$GLOBALS['base_url'].'/sites/default/files/'.$upload_certificates_file_url;
	
	echo $download.'</br>'.$output.'</fieldset>' ;
	
	

	
}
	
?>
 </div></div> </div>



