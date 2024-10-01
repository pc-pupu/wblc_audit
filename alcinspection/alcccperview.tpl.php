<?php global $base_root, $base_path; 
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');

$aa = $base_root.$base_path.'alcinspection';
$jj = l(t('<button type="button" class="btn btn-default"><i class="fa fa-angle-left"></i> Back'), $aa, array('html'=>TRUE)) ; 	
?>

<link rel="Stylesheet" href="<?php print $base_path ?>sites/all/modules/inspection/default.css" type="text/css" media="screen" />
<script type="text/javascript" src="<?php print $base_path ?>sites/all/modules/inspection/DOMAlert.js"></script>
   
<script type="text/javascript" language="javascript">

function alcccperapproved() {
	var megs='Are you sure, you want to submit the application.<br/> <br/>Once submit , no changes can be made in the application.';
		
	var msg = new DOMAlert(
			{
				title: 'wblc',
				text: megs,
				skin: 'default',
				width: 300,
				height: 150,
				ok: {value: true, text: 'Yes', onclick: ccPerApproved},
				cancel: {value: false, text: 'No', onclick: ccPerApproved }
			});
			msg.show();

	
	
}

function ccPerApproved(sender, value)
		{
			sender.close();
			var confirmed=value;
			 
			 if(confirmed)
			 document.alcccapprov.submit();
		}
function isPositiveInteger(n) {
    return 0 === n % (!isNaN(parseFloat(n)) && 0 <= ~~n);
}
</script>

<?php
global $base_path;
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
        $alc_cc_per_back_note = '';
        $alc_cc_per_app_note = '';
        $pre_ins_file_number = '';
        
        foreach($content as $delta => $data){		
            $verification_dt = $data->verification_dt;
            $verification_tm = $data->verification_tm;
            $verifyTxt = $data->ins_verify_note;
            $ins_verify_place = $data->ins_verify_place;
            $verified_showcause_status = trim($data->verified_showcause_status);				
            $alc_cc_per_back_note = trim($data->alc_cc_per_back_note);
            $alc_cc_per_app_note = trim($data->alc_cc_per_app_note);
            $pre_ins_file_number = trim($data->pre_ins_file_number);
            ?>
            <tr>
                <td> Name of the Establishment/Employment/Shop</td>    
                <td><?php print $data->name_of_ind_est;  ?></td>
            </tr>
            <tr>
                <td>Address of the Establishment/Employment/Shop</td>
                <td>Address Line1 : <?php print $data->addlineone;  ?>,District :<?php print $data->district_name;  ?>,Sub-Division Name :<?php print $data->sub_div_name;  ?>,Block Name :<?php print $data->block_mun_name;  ?>,GP / Ward :<?php print $data->village_name;  ?>,Police Station :<?php print $data->name_of_police_station;  ?> ,Pin Code :<?php print $data->ind_est_pin_code;  ?></td>
            </tr>
            <tr>
                <td>Registration No</td>    
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
                    <td width="10%"><?php echo $i++;?></td>
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
	<div class="box-header with-border"><i class="ion ion-clipboard"></i>Owner Details</div>
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
<div class="box box-primary box-solid">
	<div class="box-header with-border"><i class="ion ion-clipboard"></i>Infringements details for Inspection</div>
    <div class="box-body feedback scroll">        
    	<form action="<?php print url('alcinspection/ccpermision');?>" id="alcccapprov" name="alcccapprov" method="post" enctype="multipart/form-data">
        	<input type="hidden" name="ins_file_no" id="ins_file_no" value="<?php echo $content7; ?>" />
       		<input type="hidden" name="ins_row" id="ins_row" value="<?php echo $content8; ?>" />
  			
   				<?php
				$numinfgM = 0 ;
  				$addnuminfgPrin = 0;
  				$r = 0;
 				foreach($content4 as $dataprin ){ 
 					$r++;
				?>
                <table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-striped table-responsive admin-custom-table"> 
                <input type="hidden" name="type_of_ins_act<?php print $r ;?>" id="type_of_ins_act<?php print $r ;?>" value="<?php print trim($dataprin->type_of_infring); ?>" />
                <tr>
 					<th colspan="3">Under <?php  print $dataprin->infring_name;?></th>
                    <th width="20%">
 						<?php 
						if($dataprin->alc_cc_let_st=='A'){ 
							print "<strong>COURTCASE APPROVED</strong>";}
						elseif($dataprin->alc_cc_let_st=='R'){ 
							print "<strong>FORWARD TO RECOMMENDED</strong>";}
						else{
						?>
                        <strong>Courtcase status</strong>
                        <select name="ccsbtype<?php print $r ;?>" id="ccsbtype<?php print $r ;?>" class="input-inf-verify-sm form-control">
                            <option value="" >select permission type</option>
                            <option value="A" >APPROVED</option>
                            <option value="B" <?php if($dataprin->alc_cc_let_st=='B') echo "selected"; ?> > BACK TO INSPECTOR </option>
                            <option value="R">FORWARD TO  RECOMMENDED </option>
                       </select> 
          				<?php  
						} 
						?>
                      </th> 
    			 </tr>   
    			<tr>
                    <td width="7%"><strong>SL.No.</strong></td> 
                    <td><strong>Infringements/Additional Infringements</strong></td>        
                    <td width="10%"><center><strong>#</strong></center></td> 
                    <td width="20%"><strong>Is Compliance?</strong></td>  
                </tr>
                
                <?php 
	    		$infr_prin_query = db_select('l_pemp_infringment', 't');
				$infr_prin_query->fields('t', array('remarks_txt','verify_compl','infring_id'));
				$infr_prin_query->condition('t.ins_file_number',trim($content7),'=');
          		$infr_prin_query->condition('t.type_of_infring',$dataprin->type_of_infring,'=');
				$infr_prin_query_rst =  $infr_prin_query->execute(); 
 				$numinfgM = $infr_prin_query_rst->rowCount();
				
				$i = 0;
     			foreach ($infr_prin_query_rst as $infgp_result) { 
					$i++;
					
					$query_infg_pdf_document_fetch = db_query('select  *  from infringement_document_upld a  , file_managed b  where a.fid=b.fid  and  a.ins_file_number=:insid and a.type_of_infring=:tp and a.infg_id=:infgid ', array(':insid' => trim($content7) ,':tp' => trim($dataprin->type_of_infring) ,':infgid' => trim($infgp_result->infring_id) ));
           			$ori_infg_prf = '';
           			$infgdocpg = '';
					if($query_infg_pdf_document_fetch->rowCount() > 0){
						$result_pdf_infg = $query_infg_pdf_document_fetch->fetchAssoc();
						$fid_infg = $result_pdf_infg['fid'];
						$docfilename_infg = $result_pdf_infg['filename'];
             			$infg_ori_doc = $base_root.$base_path.'sites/default/files/infringementdoc/'.$docfilename_infg;			 
						$infgdocpg = '<i class="fa fa-file fa-2x" aria-hidden="true"></i>'; 
		     			$ori_infg_prf = l(  t($infgdocpg) , $infg_ori_doc  ,array('attributes' => array('target'=>'_blank'),'html'=>TRUE)) ;
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
				$prin_addi_infg = db_query('select  * from l_additional_infrigment where  ins_file_number = :filenumber  and type_of_infring=:tinf ', array(':filenumber' => trim($content7),':tinf' => $dataprin->type_of_infring));
				$addnuminfgPrin = $prin_addi_infg->rowCount();	
				if($addnuminfgPrin > 0) {
				
				foreach($prin_addi_infg as $delta => $addidataPrin ){
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
         	<tr>
    			<td colspan="4" align="center"><strong>COURTCASE NOTE BY INSPECTOR: </strong><?php print trim($dataprin->cc_lf_note_insp);?></td>
   			</tr> 
			<?php 
            if($dataprin->alc_cc_let_st=='A' || $dataprin->alc_cc_let_st=='R' ) { 
            ?>
            <tr>
                <td colspan="4" align="center"><strong>COURTCASE NOTE BY ALC: </strong><?php print trim($dataprin->alc_cc_lf_note);?></td>
            </tr>
            <?php 
            }else{
            ?>    
            <tr>                 
                <td align="center" colspan="4"><strong>Enter Note</strong><br /><textarea name="cc_note<?php print $r ;?>" id="cc_note<?php print $r ;?>" rows="3" cols="100" style="overflow:hidden; alignment-adjust:central " width="100%"></textarea></td>        
            </tr>
            <?php
            }
			?>
        </table>
        <?php
        }
        ?>       
   </div>
  
   <center><input type="button" name="forward" id="forward" value="SUBMIT" onClick="alcccperapproved();" class="form-submit-cus-btn btn btn-primary" /></center>
</form>
<br />
</div>

