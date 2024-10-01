<?php 
global $base_root,$base_path; 

$aa = $base_root.$base_path.'alcinspection';
$jj = l(t('<button type="button" class="btn btn-default"><i class="fa fa-angle-left"></i> Back'), $aa, array('html' => TRUE)) ; 	
	
?>
   
<link rel="Stylesheet" href="<?php print $base_path ?>sites/all/modules/inspection/default.css" type="text/css" media="screen" />
<script type="text/javascript" src="<?php print $base_path ?>sites/all/modules/inspection/DOMAlert.js"></script>    
<script type="text/javascript" language="javascript">

function alc_remarks_ins_note() {
	var alc_bfsc_note=document.getElementById('alc_ins_note_rm').value;
	if(!alc_bfsc_note.trim())	{
	    var megs='Please enter remarks';
	    document.getElementById('alc_ins_note_rm').style.border = "1px solid #ff0000";
		document.getElementById('alc_ins_note_rm').focus();
		var msg = new DOMAlert(
			{
				title: 'wblc',
				text: megs,
				skin: 'default',
				width: 300,
				height: 150,
			ok: {value: false, text: 'OK', onclick: insNoteRemarks }
			});
			msg.show();
	}else{ 
		var megs='Are you sure, you want to submit the remarks.<br/> <br/>Once submit, no changes can be made in the remarks.';
	
		var msg = new DOMAlert(
			{
				title: 'wblc',
				text: megs,
				skin: 'default',
				width: 300,
				height: 150,
				ok: {value: true, text: 'Yes', onclick: insNoteRemarks},
				cancel: {value: false, text: 'No', onclick: insNoteRemarks }
			});
			msg.show();
	}
}

function insNoteRemarks(sender, value){
	sender.close();
	var confirmed=value;
	 
	if(confirmed)
	 document.insnoteremarks.submit();
}

function isPositiveInteger(n) {
    return 0 === n % (!isNaN(parseFloat(n)) && 0 <= ~~n);
}
</script>
<div class="box box-primary box-solid">	 
	<?php
	global $base_path ;	
	$confirmstatus = ""; 	
	?>
    <div class="box-header with-border"><i class="ion ion-clipboard"></i>Establishment / Employment / Shop Details <div class="pull-right"><?php print $jj;  ?></div></div>
    <div class="box-body feedback scroll">   		      	
        <table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-bordered">		
            <tr>
                <th width="35%">Parameters</th>
                <th>Inputs</th>
            </tr>           
           
		   <?php
		   foreach($content as $delta => $data){		
			  $verification_dt = $data->verification_dt;
			  $verification_tm = $data->verification_tm;
			  $verifyTxt = $data->ins_verify_note;
			  $ins_verify_place = $data->ins_verify_place;
			  $alc_insnote_rmks=$data->alc_insnote_rmks;	 
		  ?>
      	  <tr>
      		 <td> Name of the Establishment/Employment/Shop </td>    
      		 <td><?php print $data->name_of_ind_est;?></td>
          </tr>
          <tr>
      		  <td>Address of the Establishment/Employment/Shop</td>
			  <td><?php print $data->addlineone;  ?>,<br /> <?php print $data->village_name;?>, <?php print $data->block_mun_name;?>, <?php print $data->sub_div_name;?><br /><?php print $data->district_name;?>,West Bengal,<br /> Police Station-<?php print $data->name_of_police_station;?>,Pin-<?php print $data->ind_est_pin_code;?></td>
          </tr>
     	  <tr>
      		<td>Registration No:</td>    
      		<td><?php if($data->reg_linc_no != ''){ echo $data->reg_linc_no;}else{ echo 'Not available';}?></td>
          </tr>
    	  <tr>
      	 	<td>Nature of industry/business : </td>    
      		<td><?php if($data->nature_work=='28' ) { print $data->other_nature_name; } else { print $data->cont_work_name; }?></td>     
    	  </tr>
        <?php
    	}
		?>
        </table>
        </div>
</div>

<div class="box box-primary box-solid">
			<div class="box-header with-border"><i class="ion ion-clipboard"></i> Person present during this inspection Details</div>
			<div class="box-body">
      			<table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-bordered">
                    <tr>
                        <th width="7%">Sl. No.</th>
                        <th>NAME</th>
                        <th width="20%"> DESIGNATION </th> 
                        <th width="15%"> MOBILE </th>
                    </tr> 
                    <?php
					$slno = 0;
                    foreach($content2 as $delta => $datacon){
                    ?>
                        <tr>
                            <td><?php print ++$slno;?></td>
                            <td><?php print  ucfirst($datacon->name_person);?></td> 
                            <td><?php print  $datacon->desg_person;?></td> 				
                            <td><?php print  $datacon->mobile_person;?></td>
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
                    <th width="7%">Sl. No.</th>
                    <th>NAME</th> 
                    <th width="35%">ADDRESS </th> 
                    <th width="15%">MOBILE </th>
                </tr>
                <?php
                $d = 0;
				$slno = 0;
                foreach($content5 as $delta => $dataW){ 
                    $d++;
                    if(trim($dataW->country_id)==1 )
                        $address_owner=strtoupper($dataW->address_line);
                    elseif(trim($dataW->country_id)==2 )
                        $address_owner=strtoupper($dataW->other_country_name);
                ?>
                    <tr>
                        <td><?php print ++$slno;?></td>
                        <td><?php print  strtoupper($dataW->name_mt_worker)."(".$dataW->type_of_emp.")"; if(trim($dataW->type_of_emp)=='Others') { print  "( ".strtoupper($dataW->other_type_name).")";}?></td>
                        <td> <?php print  strtoupper($address_owner);?></td> 
                        <td> <?php print  strtoupper($dataW->owner_mobile);?></td>
                   </tr>
               <?php
               }  
               ?>
        </table>
    </div>
</div>

<div class="box box-primary box-solid">	 
    <div class="box-header with-border"><i class="ion ion-clipboard"></i>Infringements for Inspection</div>
        <div class="box-body feedback scroll">
            <table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-bordered">
            <?php
			$numinfgM = 0 ;
  			$addnuminfgPrin = 0;
			
			foreach($content4 as $dataprin){
			?>
            <tr>
        		<td colspan="3"><strong><?php  print $dataprin->infring_name;?></strong></td>
    		</tr>   
    		<tr>
      			<th width="7%">Sl. No.</th> 
      			<th>Infringements</th>       
    		</tr>
            
            <?php  
 
	    	$infr_prin_query = db_select('l_pemp_infringment', 't');	      
			$infr_prin_query->fields('t', array('remarks_txt'));		
            $infr_prin_query->condition('t.ins_file_number',trim($content7),'=');
          	$infr_prin_query->condition('t.type_of_infring',$dataprin->type_of_infring,'=');
			
			$infr_prin_query_rst =  $infr_prin_query->execute(); 
 
 			$numinfgM=$infr_prin_query_rst->rowCount();

    		$prin_addi_infg = db_query('select  * from l_additional_infrigment where  ins_file_number = :filenumber  and type_of_infring=:tinf', array(':filenumber' => trim($content7),':tinf' => $dataprin->type_of_infring));
			$addnuminfgPrin = $prin_addi_infg->rowCount();
 			
			if($numinfgM==0 && $addnuminfgPrin==0){
			?>
    		<tr>   
      			<td colspan="2">No Infringements Detected</td>
   			</tr> 
        <?php	
		}else{	 
	  		$i = 0;
     		foreach ($infr_prin_query_rst as $infgp_result){
				$i++;
			?> 
				<tr>
      				<td> <?php print $i;  ?> </td>
      				<td> <?php print $infgp_result->remarks_txt;?></td>
                </tr> 
 
		<?php
		}
		
		$prin_addi_infg = db_query('select  * from l_additional_infrigment where  ins_file_number = :filenumber  and type_of_infring=:tinf', array(':filenumber' => trim($content7),':tinf' => $dataprin->type_of_infring));
		$addnuminfgPrin=$prin_addi_infg->rowCount();	
		
		if($addnuminfgPrin > 0) {
		?>
        <tr>
        	<td colspan="2">ADDITIONAL INFRINGEMENTS</td>
    	</tr>
     	<?php 
		foreach($prin_addi_infg as $delta => $addidataPrin ){ 
		?>
        	<tr>
            	<td> <?php print $i;  ?> </td>
                <td><?php print $addidataPrin->additional_infg;?></td>
   			</tr>   
		
		<?php
		}
		}
	}
}
?>
</table> </div></div>
 
<?php 

if($numinfgM != 0 || $addnuminfgPrin != 0) { 
 
?>

<div class="box box-primary box-solid">	 
    <div class="box-header with-border"><i class="ion ion-clipboard"></i>Compliance Schedule</div>
    <div class="box-body feedback scroll">
        <table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-bordered">
            <tr>
                <td><strong>Compliance Date</strong></td>
                <td><?php print date('dS M, Y', strtotime($verification_dt));?></td>
            </tr>
            <tr>
                <td><strong>Time</strong></td>
                <td><?php print $verification_tm;?></td>
            </tr>
            <tr>
                <td><strong> Compliance Place</strong></td>
                <td><?php  print $ins_verify_place;?></td>
            </tr>
        </table>
     </div>
 </div>
 
 <?php  
 }
  $contentdoc = array();
  
  $query_insnot_doc = db_query('select  *  from lc_signed_document_attachment a  , l_infringment c  where a.ins_file_number=c.ins_file_number and a.type_of_infring=c.type_of_infring and  a.ins_file_number=:insid', array(':insid' => trim($content7)));
  
  if($query_insnot_doc->rowCount() > 0){ 
  	?>
  <div class="box box-primary box-solid">	 
      <div class="box-header with-border"><i class="ion ion-clipboard"></i>Uploaded Inspection note</div>
      <div class="box-body feedback scroll">
          <table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-bordered">
          	<tr>
                <th>ACT</th> 
                <th width="13%"><center>ORIGINAL DOCUMENT</center> </th> 
                <th width="13%"><center>SIGNED DOCUMENT</center></th>
            </tr>
            <?php
			foreach ($query_insnot_doc as $docobj) {
				$fid = $docobj->fid;
				$infring_name = $docobj->infring_name;
				$signed_doc_line = 'Not Uploaded';
				$ori_doc_line = 'Not Uploaded';
				
				if($fid){
		    		$query_file_id1=db_query(' select   filename   from file_managed a , lc_signed_document_attachment b where a.fid=b.fid and  a.fid = :fid and typ_fl= :tfl  ', array(':fid' => trim($fid),':tfl' => 'S' ));	
		 		if($query_file_id1->rowCount() > 0){
	         		$result1 = $query_file_id1->fetchAssoc();
	         		$filename1 = $result1['filename'];			 
			 		$signed_doc = $base_root.$base_path.'sites/default/files/inssigneddoc/'.$filename1;
		     		$signed_doc_line = l(  t('VIEW') , $signed_doc  ,array('attributes' => array('target'=>'_blank'))) ;
		      	}
			}
			if($fid){
			  	$query_file_id2=db_query(' select   filename   from file_managed a, lc_signed_document_attachment b where a.fid=b.fid and  a.fid = :fid and typ_fl= :tfl ', array(':fid' => trim($fid),':tfl' => 'O' ));	
		   		if($query_file_id2->rowCount() > 0){
	         		$result2 = $query_file_id2->fetchAssoc();
	         		$filename2 = $result2['filename'];
					
					$ori_doc = $base_root.$base_path.'sites/default/files/insorigidoc/'.$filename2;
		     		$ori_doc_line = l(  t('<i class="fa fa-file fa-2x" aria-hidden="true"></i>') , $ori_doc  , array('html' => true, 'attributes' => array('target'=>'_blank'))) ;
		    	}
			}
			?>
         	<tr>
      			<td><?php print $infring_name;?></td>
                <td align="center"><?php print  $ori_doc_line;?></td>
                <td align="center" > <?php print  $signed_doc_line;?></td>
      		</tr>
        	<?php	
	    	}	
			?>
     	</table>
     </div>
 </div>
 <?php    
  }
 ?>

<div class="box box-primary">
	<div class="box-header with-border">
  	 	<h3 class="box-title"><i class="ion ion-clipboard"></i> &nbsp; ACTIONS AND REMARK</h3>
    </div>
    <div class="box-body feedback scroll">
    	<form action="<?php print url('alcinspection/rmsinsnote');?>" id="insnoteremarks" name="insnoteremarks" method="post" enctype="multipart/form-data">
        	<input type="hidden" name="ins_file_no" id="ins_file_no" value="<?php echo $content7; ?>" /> 
            <div class="row">
            <div class="col-md-3"></div>
            <div class="form-custom col-md-6">
                <label class="input">
                    <div class="form-item form-type-select">
                        <label for="Remark">Enter remarks/comments<font color="#FF0000">*</font></label>
                        <textarea name="alc_ins_note_rm" id="alc_ins_note_rm" rows="2" class="form-control"></textarea>
                    </div>
                </label>
             </div>
            </div>           
            <div class="row">
                <div class="col-md-3"></div>
                <div class="col-md-6">   
                    <div class="box-footer">
                    	<input type="button" name="forward" id="forward" value="SUBMIT" onClick="alc_remarks_ins_note();" class="btn btn-primary pull-left form-submit" />
                     </div>
                </div>
            </div>
            <?php if($alc_insnote_rmks) { print $alc_insnote_rmks;}?>            
        </form>
     </div>
</div>