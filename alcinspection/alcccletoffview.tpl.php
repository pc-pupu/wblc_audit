<?php global $base_root, $base_path; 
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');

//print_r($content2);


             $aa=$base_root.$base_path.'alcinspection';
		     $jj=l(t('<font color="#CC3366"><b>BACK</b></font>'), $aa,array('html'=>TRUE)) ; 	
	
	        
?>
<style type="text/css">
	table, td, th {border: 1px solid #006595;font-size:15px;font-family:Times New Roman;height:40px;margin-top:20px;}
	td{padding-left:13px !important;}
	th {background-color: #008BD1;color: white;}
	td:hover {background-color:#d4e3e5;}
	tr:nth-child(even) {background: #DBE5F0}
	tr:nth-child(odd) {background: #F1F1F1}
	.red-star{color:#FF0000;}
	
</style>
    <link rel="Stylesheet" href="<?php print $base_path ?>sites/all/modules/inspection/default.css" type="text/css" media="screen" />

	<script type="text/javascript" src="<?php print $base_path ?>sites/all/modules/inspection/DOMAlert.js"></script>
    
<script type="text/javascript" language="javascript">




function alc_bf_cclt_approved(st) {
	
	document.getElementById('alc_bf_cc_lf_status').value=st; 
	
	var submsg='';
	var alc_bfsc_note=document.getElementById('alc_bfcc_lt_note').value;
	  if(!alc_bfsc_note.trim())	{
	    var megs='Please enter letoff note';
	    document.getElementById('alc_bfcc_lt_note').style.border = "1px solid #ff0000";
		document.getElementById('alc_bfcc_lt_note').focus();
		var msg = new DOMAlert(
			{
				title: 'wblc',
				text: megs,
				skin: 'default',
				width: 300,
				height: 150,
			ok: {value: false, text: 'OK', onclick: cclfApproved }
			});
			msg.show();
	}
	  else { 
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
				ok: {value: true, text: 'Yes', onclick: cclfApproved},
				cancel: {value: false, text: 'No', onclick: cclfApproved }
			});
			msg.show();

	
	
}
}
function cclfApproved(sender, value)
		{
			sender.close();
			var confirmed=value;
			 
			 if(confirmed)
			 document.alcbfccltapprov.submit();
		}
function isPositiveInteger(n) {
    return 0 === n % (!isNaN(parseFloat(n)) && 0 <= ~~n);
}
</script>


<!--<style type="text/css">


.adminem
{


font-family: Georgia,serif;
color:#ffffff;
font-weight:bold;
background:#4BA0F4; 


	

}
</style>-->
<div class="content"><div class="sky-form" style="width:100%;"><fieldset>
<?php 
//print_r($content5);

global $base_path ;
$confirmstatus =""; 
//print_r($content);

$verified_showcause_status="";


?>

<table width="100%" border="0" cellspacing="0" cellpadding="0"  >

  <tr >
      <td colspan="3" align="right" style="text-align:right;font-size:17px; padding-top: 8px;"><font color="#3366FF"><strong><?php print $jj;  ?></strong></font></td>
      
    </tr>


<tr>
       <td width="35%" style="text-align:center;font-size:17px; padding-top: 8px ; background-color: #008BD1;color: white;">Parameters   </td>
      <td colspan="2" style="text-align:center;font-size:17px; padding-top: 8px ; background-color: #008BD1;color: white;">  Inputs  </td>
        
      </tr>

    <tr >
      <td colspan="3" style="font-size:17px; padding-top: 8px;"><font color="#3366FF"><strong>Establishment / Employment / Shop Details</strong></font></td>
      
    </tr>
	
 

<?php 
$alc_bf_cc_lf_back_note='';
$alc_bf_cc_lf_app_note='';
$lf_cc_insp_note='';

 //foreach ($content as $data) {
		foreach($content as $delta => $data){ 
		
	            $verification_dt = $data->verification_dt;
                $verification_tm = $data->verification_tm;
				$verifyTxt = $data->ins_verify_note;
				$ins_verify_place = $data->ins_verify_place;
				$verified_showcause_status = trim($data->verified_showcause_status);
				
				$alc_bf_cc_lf_back_note = trim($data->alc_bf_cc_lf_back_note);
				$alc_bf_cc_lf_app_note = trim($data->alc_bf_cc_lf_app_note);
				$lf_cc_insp_note = trim($data->lf_cc_insp_note);
	 
?>
      <tr>
      <td  > Name of the Establishment/Employment/Shop : </td>
    
      <td colspan="2"  ><?php print $data->name_of_ind_est;  ?></td>
      
    </tr>
     <tr>
      <td  >Registration No: </td>
    
      <td colspan="2" ><?php print $data->reg_linc_no;  ?></td>
      
    </tr>
    <tr>
      <td  >Nature of industry/business : </td>
    
      <td  colspan="2"><?php if($data->nature_work=='28' ) { print $data->other_nature_name; } else { print $data->cont_work_name; }  ?></td>
      
    </tr>
    
    <tr >
     <td colspan="3" align="center" style="text-align:center;font-size:17px; padding-top: 8px;"><font color="#3366FF"><strong>Address Of Establishment / Employment / Shop  </strong></font></td>
      
    </tr>
    <tr>
      <td  align="center"  colspan="3">Address Line1 : <?php print $data->addlineone;  ?>,District :<?php print $data->district_name;  ?>,Sub-Division Name :<?php print $data->sub_div_name;  ?>,Block Name :<?php print $data->block_mun_name;  ?>,GP / Ward :<?php print $data->village_name;  ?>,Police Station :<?php print $data->name_of_police_station;  ?> ,Pin Code :<?php print $data->ind_est_pin_code;  ?></td></tr>
    
      
    
    <?php
    }
	?>
      <tr >
      <td colspan="3" align="center" style="text-align:center;font-size:17px; padding-top: 8px;"><font color="#3366FF"><strong>
          person present during this inspection Details
      </strong></font></td>
      
    </tr>
     <tr>
      <th style="text-align:center;">NAME</th> 
      <th style="text-align:center;"> DESIGNATION </th> 
      <th style="text-align:center;"> MOBILE </th>
    </tr> 
    <?php  //foreach ($content as $data) {
		foreach($content2 as $delta => $datacon){ 
		
	
	 
?>
  
    
    
       <tr>
      <td align="center"> <?php print  strtoupper($datacon->name_person);  ?> </td> <td align="center" > <?php print  strtoupper($datacon->desg_person);  ?> </td> <td align="center" > <?php print  strtoupper($datacon->mobile_person);  ?> </td>
      </tr>
    
     
      
   
 
    
   
    
    
    
    
  
  <?php  } ?>  
 </table> 
 <div style=' height:200px;overflow-y : auto;' align='center'> 
   <table width="100%" border="0" cellspacing="0" cellpadding="0">
      <tr>
        <td colspan="3" align="center" style="text-align:center;font-size:17px; padding-top: 8px;"><font color="#3366FF"><strong> Owner  Details</strong></font></td>
    </tr>
   <tr>
      <th style="text-align:center;">NAME</th> 
      <th style="text-align:center;"> ADDRESS </th> 
      <th style="text-align:center;"> MOBILE </th>
    </tr>
   <?php
   $d=0;
		foreach($content5 as $delta => $dataW){ 
		$d++;
		if(trim($dataW->country_id)==1 )
		$address_owner=strtoupper($dataW->address_line);
	     elseif(trim($dataW->country_id)==2 )
	     $address_owner=strtoupper($dataW->other_country_name);
	?>
  
   
    <tr>
      <td> <?php print  strtoupper($dataW->name_mt_worker)."(".$dataW->type_of_emp.")"; if(trim($dataW->type_of_emp)=='Others') { print  "( ".strtoupper($dataW->other_type_name).")"; }  ?> </td> <td > <?php print  strtoupper($address_owner);  ?> </td> <td > <?php print  strtoupper($dataW->owner_mobile);  ?> </td>
      </tr>
      
  
  <?php
  
		}
  
  ?>
   </table></div>
  
    <form action="<?php print url('alcinspection/alcbfcclfpermision');?>" id="alcbfccltapprov" name="alcbfccltapprov" method="post" enctype="multipart/form-data">
     <input type="hidden" name="alc_bf_cc_lf_status" id="alc_bf_cc_lf_status" value="" />
  <table width="100%" border="0" cellspacing="0" cellpadding="0">
   <tr>
        <td colspan="4" align="center" style="text-align:center;font-size:17px; padding-top: 8px;"><font color="#3366FF"><strong>INFRINGEMENTS FOR  INSPECTION</strong></font><br/> </td>
    </tr>
 <?php
 
 //print_r($content7);
  $numinfgM==0 ;
  $addnuminfgPrin==0;
  $r=0;
 foreach($content4 as $dataprin ){ 
 
 $r++;
 ?> 
   <input type="hidden" name="type_of_ins_act<?php print $r ;?>" id="type_of_ins_act<?php print $r ;?>" value="<?php print trim($dataprin->type_of_infring); ?>" /> 
 <tr>
        <td colspan="4" align="center" style="text-align:center;font-size:17px; padding-top: 8px;"><font color="#3366FF"><strong> Under <?php  print $dataprin->infring_name;  ?>   </strong></font><br/> </td>
    </tr>   
    <tr>
      <th>SL.No.</th> 
      <th colspan="2">&nbsp;INFRINGEMENTS</th> 
      <th align="center">COMPLIANCE</th> 
    </tr> 
    
    
   
  
   <?php  
 
	    	$infr_prin_query = db_select('l_pemp_infringment', 't');
	   
	      
			$infr_prin_query->fields('t', array('remarks_txt','verify_compl','infring_id'));
		
           $infr_prin_query->condition('t.ins_file_number',trim($content7),'=');

          $infr_prin_query->condition('t.type_of_infring',$dataprin->type_of_infring,'=');
  
   
        
   
 
 $infr_prin_query_rst =  $infr_prin_query->execute(); 
 
 $numinfgM=$infr_prin_query_rst->rowCount();

    $prin_addi_infg=db_query('select  * from l_additional_infrigment where  ins_file_number = :filenumber  and type_of_infring=:tinf', array(':filenumber' => trim($content7),':tinf' => $dataprin->type_of_infring));
	$addnuminfgPrin=$prin_addi_infg->rowCount();
	
	
	//print $numinfgM.'|'.$addnuminfgPrin.'|'.$content7;
	//exit;
	
 $i=0;
     foreach ($infr_prin_query_rst as $infgp_result) { 
	$i++;
	if($i%2==0){
		        $bgcol = "#FCF6CF";
				 }
				else
				{
				$bgcol = "#FEFEF2";
				}
				
		
		$query_infg_pdf_document_fetch=db_query('select  *  from infringement_document_upld a  , file_managed b  where a.fid=b.fid  and  a.ins_file_number=:insid and a.type_of_infring=:tp and a.infg_id=:infgid ', array(':insid' => trim($content7) ,':tp' => trim($dataprin->type_of_infring) ,':infgid' => trim($infgp_result->infring_id) ));
           $ori_infg_prf='';
           $infgdocpg ='';
		if($query_infg_pdf_document_fetch->rowCount() > 0){
		$result_pdf_infg=$query_infg_pdf_document_fetch->fetchAssoc();
		$fid_infg=$result_pdf_infg['fid'];
		$docfilename_infg = $result_pdf_infg['filename']; 

             $infg_ori_doc=$base_root.$base_path.'sites/default/files/infringementdoc/'.$docfilename_infg;
			
			 
			$infgdocpg = '<img  src="'.$base_root.$base_path.'/sites/all/themes/jackson/images/pdfimage.jpg" width="50" height="50"  >'; 
		     $ori_infg_prf=l(  t($infgdocpg) , $infg_ori_doc  ,array('attributes' => array('target'=>'_blank'),'html'=>TRUE)) ;
		}
		
				
				
				
				
	
	?> 
<tr bgcolor="<?php print $bgcol ?>">
      <td> <?php print $i;  ?> </td>
      <td colspan="2"> <?php print $infgp_result->remarks_txt;?> 
      <a onclick="window.open('<?php print $infg_ori_doc ;?>','mywindow','status=1,width=450,height=450'); " ><?php echo $infgdocpg; ?></a> 
      
      
       </td>
   <?php if(trim($infgp_result->verify_compl)=='Y') { ?>
     <td  align="right" ><center> <b> <font color="DarkGreen"> <?php print " YES    " ; ?> </font> </b> </center></td>
      <?php
	  } 
	  else
	  {
	  ?>
       <td  align="right" bgcolor="#FEFEF2"><center> <b> <font color="red"> <?php print " NO    " ; ?> </font> </b></center> </td>
       <?php  
	   
	 }
	  ?>
    </tr> 
 
     <?php
		}
		$prin_addi_infg=db_query('select  * from l_additional_infrigment where  ins_file_number = :filenumber  and type_of_infring=:tinf', array(':filenumber' => trim($content7),':tinf' => $dataprin->type_of_infring));
	$addnuminfgPrin=$prin_addi_infg->rowCount();	
	if($addnuminfgPrin > 0) {
		?>
     <!--   <tr  bgcolor="#66CCFF" style="font-family: Georgia,serif ">
 <td  align="center" colspan="4" class="adminem" > <center>ADDITIONAL INFRINGEMENTS </center></td>
      
   </tr>    
     <?php   
	
	foreach($prin_addi_infg as $delta => $addidataPrin ){ 
	
		?>
        
        
      <tr bgcolor="<?php //print $bgcol ?>">
   
      <td  align="center" colspan="4"  ><center> <?php print $addidataPrin->additional_infg;  ?> </center> </td>
   </tr> -->  
		
<?php

	}
	}
	
	?>
   
  
   
  
   <tr>
    <td  align="center" colspan="4"  ><b> LETOFF NOTE BY INSPECTOR : </b><?php print trim($dataprin->cc_lf_note_insp);  ?>  </td>
   </tr>  
    <?php
	
	
	
	 }
	
   ?>
		
    </table> 
    
    <?php  
 
  if($alc_bf_cc_lf_back_note){
	   
     ?>
    
  
<table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
    <td  align="center" colspan="4"  ><b> BACK TO INSPECTOR  NOTE BY ALC:</b> <?php print  strtoupper($alc_bf_cc_lf_back_note); ?>  </td>
   </tr>    </table>

 
 <?php 
 } 
  
   if($alc_bf_cc_lf_app_note){
?>
    
  

<table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
    <td  align="center" colspan="4"  ><b> LETOFF APPROVED  NOTE BY ALC:</b> <?php print  strtoupper($alc_bf_cc_lf_app_note); ?>   </td>
   </tr>    </table>



 
 <?php } ?>

<div > &nbsp;</div>
 <div > &nbsp;</div>
 <?php
if(!trim($alc_bf_cc_lf_app_note)){ ?>
  
 
          <input type="hidden" name="ins_file_no" id="ins_file_no" value="<?php echo $content7; ?>" />
          
          
        <div > <center> 
        <b> APPROVED/BACK TO INSPECTOR FOR LETOFF NOTE BY ALC : </b><br /><textarea name="alc_bfcc_lt_note" id="alc_bfcc_lt_note" rows="8" cols="100" style="overflow:hidden; alignment-adjust:central " width="100%"  > 
         </textarea>
        </center> 
        </div>  
          
        <div>       
       <!-- <tr>
         <td colspan="3"   style="text-align:center;font-size:17px; padding-top: 8px;" align="center"> <input type="button" name="forward" id="forward" value="APPROVED" onClick="alcapproved();" />        </td>
         </tr> -->
         
         <center>
          <input type="button" name="forward" id="forward" value="APPROVED" onClick="alc_bf_cclt_approved('A');" class="form-submit-cus-btn btn btn-primary" />
           <input type="button" name="forward" id="forward" value="BACK TO INSPECTOR" onClick="alc_bf_cclt_approved('B');" class="form-submit-cus-btn btn btn-primary" />
         
         </center> 
         
         
         </div>
   <input type="hidden" name="ins_row" id="ins_row" value="<?php echo $r; ?>" />
</form>     
          
 <?php } ?>           
            
            
	 
   
  <table width="100%" border="0" cellspacing="0" cellpadding="0" >   
      <tr><td style="font-size:17px; padding-top: 8px;"><font color="#3366FF"><strong>< <?php print $jj;?></strong></font></td></tr>
  </table>
  
  </fieldset>
  </div>
  </div>
  <?php //} ?>