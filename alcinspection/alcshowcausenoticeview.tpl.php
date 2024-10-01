<?php 
global $base_root,$base_path; 
?>

<link rel="Stylesheet" href="<?php print $base_path ?>sites/all/modules/inspection/default.css" type="text/css" media="screen" />
<script type="text/javascript" src="<?php print $base_path ?>sites/all/modules/inspection/DOMAlert.js"></script>
<script type="text/javascript" language="javascript">
function showcauseIns() {
	
	
	var megs='Are you sure, you want to show cause the application.<br/> <br/>Once submit, no changes can be made in the application.';
	var msg = new DOMAlert(
			{
				title: 'wblc',
				text: megs,
				skin: 'default',
				width: 300,
				height: 150,
				ok: {value: true, text: 'Yes', onclick: showValueShowCause},
				cancel: {value: false, text: 'No', onclick: showValueShowCause }
			});			
			msg.show();
}

function showValueShowCause(sender, value){
			sender.close();
			var confirmed=value;
			 
			 if(confirmed)
			 document.showcauseins.submit();
		}

</script>
<?php
 $aa = $base_root.$base_path.'alcinspection';
 $link_s = l(t('<button type="button" class="btn btn-default"><i class="fa fa-angle-left"></i> Back'), $aa, array('html'=>TRUE)) ; 	
?>


		<?php
		 $complains_dt ='';
		 $complains_tm ='';
		 $ins_dt = '';
		 $ins_tm = '';
		
		 foreach($content as  $rsestab ){	      
			$name_of_ind_est = $rsestab->name_of_ind_est;
			$district_name_est = $rsestab->district_name;
			$sub_div_name_est = $rsestab->sub_div_name;
			$block_mun_name_est=$rsestab->block_mun_name;
			$village_name_est = $rsestab->village_name;			
			$pre_ins_file_number=	$rsestab->pre_ins_file_number;
			$complains_dt = $rsestab->verification_dt;
		    $complains_tm = $rsestab->verification_tm;			
		}
		
		$sub_dt_row = db_query('select  *  from l_inspection_establishment  where  ins_file_number = :filenum and ins_submit_status= :sta and fwd_verify=:vs  and type_of_insnote_user=:typ', array(':filenum' => trim($pre_ins_file_number),':sta' => 'S',':vs' => 'Y',':typ' => 'inspection_note'));	  
		 
		 $resultdt=$sub_dt_row->fetchAssoc();
		 $ins_submit_dt = $resultdt['ins_submit_dt'];
		 $verification_dt = $resultdt['verification_dt'];
		 $verification_tm = $resultdt['verification_tm'];
		 $ins_dt =$resultdt['ins_dt'];
		 $ins_tm =$resultdt['ins_tm'];
	?>
     <div class="pull-right"><?php print $link_s;?></div>
     <div class="row">
  	<?php
	$epm = 1;
	foreach($content5 as  $rsconper){
		$name_mt_worker = trim($rsconper->name_mt_worker);
		$district_name = trim($rsconper->district_name);
		$sub_div_name = trim($rsconper->sub_div_name);
		$block_mun_name = trim($rsconper->block_mun_name);
		$village_name = trim($rsconper->village_name); 
		$name_of_police_station = trim($rsconper->name_of_police_station); 
		$own_pin = trim($rsconper->own_pin); 
		$other_country_name = $rsconper->other_country_name;  
		$country_id = $rsconper->country_id;  
		$statename = $rsconper->statename;
		if($country_id==2){
		?>
		<!--<tr><td  align="left"><?php print $name_mt_worker ?></td></tr>  
		<tr><td  align="left">Country Name: <?php print $other_country_name ?></td></tr>-->
		<?php
		}elseif($country_id == 1){
		?>
       <!-- <div class="col-md-4">-->
       
       <div class="col-md-4">
		<div class="box box-primary box-solid">
			<div class="box-header with-border"><i class="ion ion-clipboard"></i>To:Employer/Owner-<?php echo $epm++;?></div>
            <div class="box-body feedback scroll">
                <table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-bordered">                        
                    <tr><td width="30%">Name</td><td><?php print $name_mt_worker ?></td></tr>  
                    <tr><td>GP/Ward</td><td><?php print $village_name ?></td></tr>
                    <tr><td>Block/Municipality</td><td><?php print $block_mun_name ?></td></tr>
                    <tr><td>Sub-Division</td><td><?php print $sub_div_name; ?></td></tr>
                    <tr><td>Police Station</td><td><?php print $name_of_police_station; ?></td></tr>
                    <tr><td> Pin Code</td><td><?php print $own_pin; ?></td></tr>
                    <tr><td>District</td><td><?php print $district_name; ?></td></tr>
                    <tr><td>State</td><td><?php print $statename;?></td></tr>
                 </table>
             </div>
        </div>
        </div>     
		<?php
        }
        ?>        
        <!--<tr><td  align="left" bgcolor="#FEFEF2">&nbsp; </td></tr>-->
        <?php
        }
        ?>        
		</div> 
        <div class="box box-primary box-solid">
			<div class="box-header with-border"></div>
            <div class="box-body feedback scroll">
            	<table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-bordered">
  					<tr><td>Subject</td><td>Show – cause Notice</td></tr>					 
  					<?php   
  					$con_act_query = db_select('l_infringment', 'a');
					$con_act_query->join('l_inspection_establishment', 'b', 'a.ins_file_number = b.ins_file_number');
					$con_act_query->condition('a.ins_file_number',trim($content7),'=');
					$con_act_query->condition('a.type_of_infring',array('C','F'),'IN');
					$con_act_query->condition('b.type_of_insnote_user', 'ins_show_cause','=');
					$con_act_query->fields('a', array('ins_file_number'));
					$con_act_query_result = $con_act_query->execute(); 
   					if($con_act_query_result->rowCount() > 0) {
						?>
                        <tr><td>Sir,<br />In course of my inspection  on <?php  print date('d-m-Y', strtotime($ins_dt)); ?> &nbsp; at &nbsp; <?php  print $ins_tm ?>the following Infringements were detected .</td></tr>
                    <?php
					}else{
					?>
 					<tr>
       					<td>Description</td><td>In course of my inspection in your establishment under the name and style of M/S : <b> <?php print $name_of_ind_est;   ?>
&nbsp; at &nbsp; District :  <?php print $district_name_est ; ?> ,  Sub-Division : <?php print $sub_div_name_est; ?>, Block/Municipality : <?php print $block_mun_name_est ?>, GP/Ward: <?php print $village_name_est ?>  on <?php  print date('d-m-Y', strtotime($ins_dt)); ?> &nbsp; at &nbsp; <?php  print $ins_tm ?> </b>the following Infringements were detected .</td></tr>
					<?php
					}
					?> 					
                </table>
            </div>
        </div>     
		<div class="box box-primary box-solid">
			<div class="box-header with-border"><i class="ion ion-clipboard"></i>Infringements details for Inspection</div>
            <div class="box-body feedback scroll">
            	<table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-bordered">
                	<?php
					$infg_name='';
					foreach($content4 as $dataprin ){ 
 						$infg_name = trim($dataprin->infring_name);
					?>   
  						<tr>
      						<th colspan="3"><?php  print $dataprin->infring_name;?></th>
      					</tr>
                        <tr>
      						<td width="7%">SL.No.</td> 
                            <td>Infringements/Additional Infringements</td>
       						<td>COMPLIANCE </td>
                        </tr>
                     <?php
                     $infr_prin_query = db_select('l_pemp_infringment', 't');
					 $infr_prin_query->fields('t', array('remarks_txt','verify_compl'));		
            		 $infr_prin_query->condition('t.ins_file_number',trim($content7),'=');
             		 $infr_prin_query->condition('t.type_of_infring',$dataprin->type_of_infring,'=');
					 
					 $infr_prin_query_rst =  $infr_prin_query->execute(); 
 					 $numinfgM = $infr_prin_query_rst->rowCount();
					 
					 $i = 0;
     				foreach ($infr_prin_query_rst as $infgp_result) { 
						$i++;
					?> 
						<tr>
      						<td><?php print $i;?></td>
      						<td><?php print $infgp_result->remarks_txt;?></td>
                            <td><?php  if(trim($infgp_result->verify_compl)=='Y') { print "Yes";}else{print "No";}?></td>
                        </tr>
                     <?php
					}
		
					$prin_addi_infg = db_query('select  * from l_additional_infrigment where  ins_file_number = :filenumber  and type_of_infring=:tinf', array(':filenumber' => trim($content7),':tinf' => $dataprin->type_of_infring));
					$addnuminfgPrin=$prin_addi_infg->rowCount();	
					if($addnuminfgPrin > 0) {
						foreach($prin_addi_infg as $delta => $addidataPrin ){ 
					?>
                    	<tr>       
      						<td><?php print $i;  ?> </td>
                    		<td>(Additional)<?php print $addidataPrin->additional_infg;?></td>
                    		<td><?php if(trim($addidataPrin->sc_status)=='N') { echo "Yes";}else{ echo "No";}?></td>
                       </tr>
                    <?php
					}
				}
				?>
              </table>
           </div>
       </div>
    
		
        <div class="box box-primary box-solid">
			<div class="box-header with-border"><i class="ion ion-clipboard"></i>Show Cause Note</div>
            <div class="box-body feedback scroll">
            	<table width="100%" border="0" cellspacing="0" cellpadding="0" class="table table-bordered">
                	<tr>                    	
                        <td colspan="3">You were requested to appear before undersigned on <b> <font color="DarkGreen"><?php print date('d-m-Y', strtotime($verification_dt)).' at  '.$verification_tm   ?>  </font> </b> To produce the registers/records or any other documentary evidences for my inspection and compliance. <?php  print trim($dataprin->show_cause_note);  ?>  , on appearing on<b> <font color="DarkGreen"> <?php print date('d-m-Y', strtotime($ins_dt)) ?>. </font> </b><br />You are, therefore, directed to showcause  within or on <b> <font color="DarkGreen"> <?php print date('d-m-Y', strtotime($dataprin->verification_dt)) .' at  '.$dataprin->verification_tm ?>. </font> </b>. as to why necessary legal action will not be taken against you as par the provision of <?php print $infg_name ;?> <br/><br/>It may please be noted that in case no reply is received within the period as stated above, it would be deemed that you have nothing to say in this regard and necessary action will be taken under the statute without any further reference.</td> 
                    </tr>
                     <?php
					 }
					 ?>                 
     			</table>
        	</div>
        </div>
        
        </form>
    
    