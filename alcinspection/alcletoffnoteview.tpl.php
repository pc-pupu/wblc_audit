<?php global $base_root, $base_path; 
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
$aa = $base_root.$base_path.'alcinspection';
$jj = l(t('<button type="button" class="btn btn-default"><i class="fa fa-angle-left"></i> Back'), $aa, array('html'=>TRUE)) ; 	
 
?>

<link rel="Stylesheet" href="<?php print $base_path ?>sites/all/modules/inspection/default.css" type="text/css" media="screen" />
<script type="text/javascript" src="<?php print $base_path ?>sites/all/modules/inspection/DOMAlert.js"></script>
    
<script type="text/javascript" language="javascript">
function alcapproved(st) {
	
	document.getElementById('alc_ins_status').value=st; 
	//alert('sssssss'+st);
	var alc_bfsc_note=document.getElementById('alc_bfsc_note').value;
	  if(!alc_bfsc_note.trim())	{
	    var megs='Please enter letoff note';
	    document.getElementById('alc_bfsc_note').style.border = "1px solid #ff0000";
		document.getElementById('alc_bfsc_note').focus();
		var msg = new DOMAlert(
			{
				title: 'wblc',
				text: megs,
				skin: 'default',
				width: 300,
				height: 150,
			ok: {value: false, text: 'OK', onclick: showValueApproved }
			});
			msg.show();
	}
	  else { 
	//var megs='Are you sure, you want to approved the application.<br/> <br/>Once approved, no changes can be made in the application.';
	
	if(st=='A')
	    var megs='Are you sure, you want to approved the application.<br/> <br/>Once approved , no changes can be made in the application.';
	  else if(st=='B')
	  var megs='Are you sure, you want to back to inspector the application.<br/> <br/> Once back to inspector , no changes can be made in the application.';
	
	var msg = new DOMAlert(
			{
				title: 'wblc',
				text: megs,
				skin: 'default',
				width: 300,
				height: 150,
				ok: {value: true, text: 'Yes', onclick: showValueApproved},
				cancel: {value: false, text: 'No', onclick: showValueApproved }
			});
			msg.show();

	
	
}
}
function showValueApproved(sender, value)
		{
			sender.close();
			var confirmed=value;
			 
			 if(confirmed)
			 document.alcltapprov.submit();
		}
function isPositiveInteger(n) {
    return 0 === n % (!isNaN(parseFloat(n)) && 0 <= ~~n);
}
</script>

<?php 
global $base_path ;
$confirmstatus = ""; 
$verified_showcause_status = "";
?>

<div class="box box-primary box-solid">
	<div class="box-header with-border"><i class="ion ion-clipboard"></i>Establishment / Employment / Shop Details <div class="pull-right"><?php print $jj;?></div></div>
    	<div class="box-body feedback scroll">   		      	
        <table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-bordered">
        	<tr>
       			<th>Parameters</th>
      			<th>Inputs</th>
            </tr>
            <?php 
			$alc_bfsc_note = '';
			$alc_back_note = '';
			
			foreach($content as $delta => $data){		
	            $verification_dt = $data->verification_dt;
                $verification_tm = $data->verification_tm;
				$verifyTxt = $data->ins_verify_note;
				$ins_verify_place = $data->ins_verify_place;
				$verified_showcause_status = trim($data->verified_showcause_status);				
				$alc_bfsc_note = trim($data->alc_bfsc_note);
				$alc_back_note = trim($data->alc_back_note);
			?>
      		<tr>
      			<td> Name of the Establishment/Employment/Shop </td>
                <td><?php print $data->name_of_ind_est;?></td>
            </tr>
     		<tr>
      			<td>Address of the Establishment/Employment/Shop</td>
                <td><?php print $data->addlineone;  ?>,District :<?php print $data->district_name;  ?>,Sub-Division Name :<?php print $data->sub_div_name;  ?>,Block Name :<?php print $data->block_mun_name;  ?>,GP / Ward :<?php print $data->village_name;  ?>,Police Station :<?php print $data->name_of_police_station;  ?> ,Pin Code :<?php print $data->ind_est_pin_code;  ?></td>
            </tr>
            <tr>
            	<td>Registration Number</td> 
                <td><?php if($data->reg_linc_no != ''){ echo $data->reg_linc_no;}else{ echo 'Not available';}?></td>
            </tr>
    		<tr>
      			<td>Nature of industry/business</td>    
      			<td><?php if($data->nature_work=='28' ) { print $data->other_nature_name; } else { print $data->cont_work_name;}?></td>
            </tr>
            <?php
    		}
			?>
          </table>
    </div>
</div>
<div class="box box-primary box-solid">
	<div class="box-header with-border"><i class="ion ion-clipboard"></i>Person present during this inspection Details</div>
    	<div class="box-body feedback scroll">   		      	
        <table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-bordered">
      		<tr>
                <th>SL. No.</th>
                <th>NAME</th> 
                <th>DESIGNATION</th> 
                <th>MOBILE</th>
            </tr> 
    		<?php
			$i = 1;
			foreach($content2 as $delta => $datacon){
			?>
            <tr>
      			 <th width="10%"><?php echo $i++;?></th>
                <td><?php print  strtoupper($datacon->name_person);?></td> 
                <td><?php print  strtoupper($datacon->desg_person);?></td> 
                <td><?php print  strtoupper($datacon->mobile_person);?></td>
            </tr>
            <?php  
			} 
			?>  
 		</table>
     </div>
</div>
<div class="box box-primary box-solid">
	<div class="box-header with-border"><i class="ion ion-clipboard"></i>Owner  Details</div>
    	<div class="box-body feedback scroll">
           <table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-bordered">   
               <tr>
                  <th width="10%">SL. No.</th>
                  <th>NAME</th> 
                  <th>ADDRESS</th> 
                  <th>MOBILE</th>
                </tr>
			   <?php
               $d = 0;
               foreach($content5 as $delta => $dataW){ 
                   $d++;
                   if(trim($dataW->country_id)==1 )
                       $address_owner=strtoupper($dataW->address_line);
                   elseif(trim($dataW->country_id)==2 )
                        $address_owner=strtoupper($dataW->other_country_name);
               ?>
               <tr>
                    <td><?php echo $d++;?></td>
                    <td><?php print ucfirst($dataW->name_mt_worker)."(".$dataW->type_of_emp.")"; if(trim($dataW->type_of_emp)=='Others') { print "(".$dataW->other_type_name.")"; }  ?> </td>
                    <td><?php print  $address_owner;?> </td> 
                    <td><?php print  $dataW->owner_mobile;?></td>
                </tr>
                <?php  
                }
                ?>
         </table>
     </div>
</div>

<form action="<?php print url('alcinspection/forward');?>" id="alcltapprov" name="alcltapprov" method="post" enctype="multipart/form-data">
<div class="box box-primary box-solid">
	<div class="box-header with-border"><i class="ion ion-clipboard"></i>Infringements for Inspection</div>
    	<div class="box-body feedback scroll">
           
     			<input type="hidden" name="alc_ins_status" id="alc_ins_status" value="" />
  				<table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-bordered">
                
 				<?php
				$numinfgM = 0;
  				$addnuminfgPrin = 0;
  				$r = 0;
 				foreach($content4 as $dataprin ){ 
 					$r++;
				?> 
   				<input type="hidden" name="type_of_ins_act<?php print $r ;?>" id="type_of_ins_act<?php print $r ;?>" value="<?php print trim($dataprin->type_of_infring); ?>" /> 
 				<tr>
        		<td colspan="4"><strong><?php print $dataprin->infring_name;?></strong></td>
    </tr>   
    <tr>
        <th width="7%">SL.No.</th> 
        <th>Infringements/Additional Infringements</th>        
        <th width="10%"><center>#</center></th> 
        <th>Is Compliance?</th> 
    </tr> 
    <?php
	$infr_prin_query = db_select('l_pemp_infringment', 't');
	$infr_prin_query->fields('t', array('remarks_txt','verify_compl','infring_id'));
	$infr_prin_query->condition('t.ins_file_number',trim($content7),'=');
	$infr_prin_query->condition('t.type_of_infring',$dataprin->type_of_infring, '=');
	$infr_prin_query_rst =  $infr_prin_query->execute();
	$numinfgM = $infr_prin_query_rst->rowCount();

    $prin_addi_infg = db_query('select  * from l_additional_infrigment where  ins_file_number = :filenumber  and type_of_infring=:tinf', array(':filenumber' => trim($content7),':tinf' => $dataprin->type_of_infring));
	$addnuminfgPrin = $prin_addi_infg->rowCount();
	
	$i = 0;
    foreach ($infr_prin_query_rst as $infgp_result) { 
		$i++;
		
		$query_infg_pdf_document_fetch = db_query('select * from infringement_document_upld a, file_managed b where a.fid = b.fid and a.ins_file_number=:insid and a.type_of_infring=:tp and a.infg_id=:infgid ', array(':insid' => trim($content7) ,':tp' => trim($dataprin->type_of_infring) ,':infgid' => trim($infgp_result->infring_id) ));
        
		$ori_infg_prf='';
        $infgdocpg ='';
		
		if($query_infg_pdf_document_fetch->rowCount() > 0){
			$result_pdf_infg = $query_infg_pdf_document_fetch->fetchAssoc();
			$fid_infg = $result_pdf_infg['fid'];
			$docfilename_infg = $result_pdf_infg['filename']; 
            $infg_ori_doc = $base_root.$base_path.'sites/default/files/infringementdoc/'.$docfilename_infg;
			
			$infgdocpg = '<i class="fa fa-file fa-2x" aria-hidden="true"></i>&nbsp;'; 
		    $ori_infg_prf = l(t($infgdocpg), $infg_ori_doc  ,array('attributes' => array('target' => '_blank'), 'html'=>TRUE));
		}
		?> 
		<tr>
      		<td><?php print $i;  ?> </td>
      		<td><?php print $infgp_result->remarks_txt;?></td>
            <td><center><a onclick="window.open('<?php print $infg_ori_doc ;?>','mywindow','status=1,width=450,height=450');"><?php echo $infgdocpg; ?></a></center></td>  			
     		<td><?php if(trim($infgp_result->verify_compl)=='Y'){echo "YES";}else{echo "NO";}?></td>        	
    	</tr> 
    	<?php
		}
		$prin_addi_infg = db_query('select  * from l_additional_infrigment where  ins_file_number = :filenumber  and type_of_infring=:tinf', array(':filenumber' => trim($content7),':tinf' => $dataprin->type_of_infring));
		$addnuminfgPrin = $prin_addi_infg->rowCount();	
		if($addnuminfgPrin > 0) {
			
		foreach($prin_addi_infg as $delta => $addidataPrin){ 	
		?>
        <tr>   
      		<td><?php print $i;  ?> </td>
            <td>(Additional)<?php print $addidataPrin->additional_infg;?></td>
            <td></td>           
     		<td><?php if(trim($addidataPrin->sc_status)=='N') { echo "Yes";}else{ echo "No";}?></td>      	
   		</tr>		
		<?php
		}
	}
	?>
    </table></div></div>
    <div class="box box-primary box-solid">
	<div class="box-header with-border"><i class="ion ion-clipboard"></i>Remarks/Note Details</div>
    	<div class="box-body feedback scroll">
        	<table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-bordered">
                <tr>
                    <td width="30%"><strong>LETOFF NOTE BY INSPECTOR:</strong></td><td><?php print trim($dataprin->let_off_note);?></td>
               </tr>    
                <?php
                }   
                if($alc_back_note){
                ?>
                <tr>
                    <td><strong>BACK TO INSPECTOR NOTE BY ALC</strong></td><td><?php print $alc_back_note;?></td>
               </tr>
               <?php 
                }  
                if($alc_bfsc_note){
                ?>
                <tr>
                    <td><strong>LETOFF APPROVED NOTE BY ALC</strong></td><td><?php print $alc_bfsc_note; ?></td>
               </tr>
                <?php
                } 
                ?>
            </table>
      </div>
  </div>
  
   

 	<?php
	if(!trim($alc_bfsc_note)){ 
	?>
 	<div class="box box-primary">
		<div class="box-header with-border">
  	 		<h3 class="box-title"><i class="ion ion-clipboard"></i> &nbsp; ACTIONS AND REMARK</h3>
    	</div>
    	<div class="box-body feedback scroll"> 
          	<input type="hidden" name="ins_file_no" id="ins_file_no" value="<?php echo $content7; ?>" />
            <div class="row">
            <div class="col-md-3"></div>
            <div class="form-custom col-md-6">
                <label class="input">
                    <div class="form-item form-type-select">
                        <label for="Remark">LETOFF Note/Remark By ALC<font color="#FF0000">*</font></label>
                        <textarea name="alc_bfsc_note" id="alc_bfsc_note" rows="2" class="form-control"></textarea>
                    </div>
               </label>
            </div>
        </div>
        <div class="row">
            <div class="col-md-3"></div>
                <div class="col-md-6">   
                    <div class="box-footer">                    	
                    	<input type="button" name="forward" id="forward" value="APPROVED" onClick="alcapproved('A');" class="btn btn-primary" />
           				<input type="button" name="forward" id="forward" value="BACK TO INSPECTOR" onClick="alcapproved('B');" class="btn btn-primary" />
                    </div>
                </div>
            </div>
        </div>
     </div>
      <input type="hidden" name="ins_row" id="ins_row" value="<?php echo $r; ?>" />
 </form>         
 <?php 
 } 
 ?>