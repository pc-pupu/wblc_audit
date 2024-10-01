<?php 
	global $base_root,$base_path;
	
	drupal_add_css(drupal_get_path('theme', 'themeinner') . '/css/style.css');
	
	$aa					= $base_root.$base_path.'alcinspection';
	
	$jj					= l(t('<font color="#CC3366"><b>BACK</b></font>'), $aa,array('html'=>TRUE)) ; 	
	$confirmstatus  	= ""; 
	$no_workmen_male 	= '';
	$no_workmen_female 	= '';
	$male_second 		= '';
	$female_second 		='';
	$male_third 		= '';
	$female_third 		= '';
?>
<div class="content">
	<div class="sky-form" style="width:100%;">
		<fieldset>
		<table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table">
			<tr>
       			<th style="text-align:center">Parameters</th>
      			<th colspan="2" style="text-align:center">Inputs</th>
     		</tr>

            <tr >
            	<td colspan="3" align="center" style="text-align:center;font-size:17px; padding-top: 8px;">
                	<font color="#3366FF"><strong>Establishment / Employment / Shop Details</strong></font></td>
            </tr>
			<?php  foreach($content as $delta => $data){ 
			
					//$address_of_the_establishment = get_full_address('l_inspection_establishment','inspection',$data->ins_file_number,array('ind_est_dist','ind_est_subdivision','ind_est_block','est_vill_ward','police_station_one'));
					
					$verification_dt 	= $data->verification_dt;
					$verification_tm 	= $data->verification_tm;
					$verifyTxt 			= $data->ins_verify_note;
					$ins_verify_place 	= $data->ins_verify_place;
					$no_workmen_male	= $data->no_workmen_male;
					$no_workmen_female 	= $data->no_workmen_female;
					$male_second 		= $data->male_second;
					$female_second 		= $data->female_second;
					$male_third 		= $data->male_third;
					$female_third 		= $data->female_third;
	 
			?>
            <tr>
              <td width="40%">Registration Number : </td>
              <td colspan="2" ><?php print $data->reg_linc_no;  ?></td>
            </tr>
      		<tr>
      			<td>Name and Address of the Establishment/Employment/Shop : </td>
      			<td colspan="2"><?php print $data->name_of_ind_est; ?><br/><?php print $data->addlineone;?>,<br/> 
                				GP / Ward :<?php print $data->village_name; ?>,&nbsp;<?php print $data->block_mun_name;?>,<br/>
                                <?php print $data->sub_div_name;?>,&nbsp;<?php print $data->district_name;?>,<br/>
                                PS- <?php print $data->name_of_police_station; ?>,&nbsp;PIN- <?php print $data->ind_est_pin_code;  ?><br/><?php //echo $address_of_the_establishment;?></td>
    		</tr>
            <tr>
              <td>Nature of industry / business : </td>
              <td colspan="2"><?php if($data->nature_work=='28' ) { print $data->other_nature_name; } else { print $data->cont_work_name; }  ?></td>
            </tr>
            <!--<tr >
             <td colspan="3" align="center" style="text-align:center;font-size:17px; padding-top: 8px;">
             	 <font color="#3366FF"><strong>Address Of Establishment / Employment / Shop  </strong></font></td>
            </tr>
            <tr>
              <td align="center"  colspan="3">Address Line  : <?php //print $data->addlineone;  ?>,District :<?php //print $data->district_name;  ?>,Sub-Division Name :<?php //print $data->sub_div_name;  ?>,Block Name :<?php //print $data->block_mun_name;  ?>,GP / Ward :<?php //print $data->village_name;  ?>,Police Station :<?php //print $data->name_of_police_station;  ?> ,Pin Code :<?php //print $data->ind_est_pin_code;  ?></td>
           </tr>-->
    	    <?php }?>
      		<tr>
      			<td colspan="3" align="center" style="text-align:center;font-size:17px; padding-top: 8px;">
                	<font color="#3366FF"><strong>Name of the person present during the Inspection </strong></font>
                 </td>
      
    		</tr>
             <tr>
              <th style="text-align:center;">NAME</th> 
              <th style="text-align:center;">DESIGNATION </th> 
              <th style="text-align:center;">MOBILE </th>
            </tr>
             
    		<?php  foreach($content2 as $delta => $datacon){ ?>
       		<tr>
     			 <td align="center"> <?php print  strtoupper($datacon->name_person);  ?> </td> 
                 <td align="center" > <?php print  strtoupper($datacon->desg_person);  ?> </td> 
                 <td align="center" > <?php print  strtoupper($datacon->mobile_person);  ?> </td>
      		</tr>
  
  			<?php } ?>  
 		</table> 


   <table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table">
      <tr>
        <td colspan="3" align="center" style="text-align:center;font-size:17px; padding-top: 8px;"><font color="#3366FF"><strong> Workers Employed  Details</strong></font></td>
    </tr>
   <tr>
      <th style="text-align:center;">&nbsp;  </th> 
      <th style="text-align:center;"> MALE </th> 
      <th style="text-align:center;"> FEMALE </th>
    </tr>
   
  
   
    <tr>
      <td>  No of workman/employed directly: </td> <td >  <?php print  $no_workmen_male;  ?> </td> <td > <?php print  $no_workmen_female;  ?> </td>
      </tr>
    <tr>
      <td> No of contract labour : </td> <td >  <?php print  $male_second;  ?> </td> <td > <?php print  $female_second;  ?> </td>
      </tr>
      
       <tr>
      <td> No of other worker if engaged : </td> <td >  <?php print  $male_third;  ?> </td> <td > <?php print  $female_third;  ?> </td>
      </tr>   
  
  
   </table>
 
 
 

 
 
 
 
 
 
 <div style=' height:200px;overflow-y : auto;' align='center'> 
   <table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table">
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
      <td> <?php print  strtoupper($dataW->name_mt_worker)."(".$dataW->type_of_emp.")"; if(trim($dataW->type_of_emp)=='Others') { print  "( ".strtoupper($dataW->other_type_name).")"; }  ?> </td> <td >  <?php print  strtoupper($address_owner);  ?> </td> <td > <?php print  strtoupper($dataW->owner_mobile);  ?> </td>
      </tr>
      
  
  <?php
  
		}
  
  ?>
   </table></div>
   <div style=' height:200px;overflow-y : auto;' align='center'> 
  <table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table">
   <tr>
        <td colspan="3" align="center" style="text-align:center;font-size:17px; padding-top: 8px;"><font color="#3366FF"><strong>INFRINGEMENTS FOR  INSPECTION</strong></font><br/> </td>
    </tr>
 <?php
 
 //print_r($content7);
  $numinfgM==0 ;
  $addnuminfgPrin==0;
 foreach($content4 as $dataprin ){ 
 

 ?> 
 
 <tr>
        <td colspan="3" align="center" style="text-align:center;font-size:17px; padding-top: 8px;"><font color="#3366FF"><strong> Under <?php  print $dataprin->infring_name;  ?>   </strong></font><br/> </td>
    </tr>   
    <tr>
      <th style="text-align:center;">SL.NO</th> 
      <th style="text-align:center;" colspan="2">  INFRINGEMENTS </th> 
      
    </tr> 
    
    
   
  
   <?php  
 
	    	$infr_prin_query = db_select('l_pemp_infringment', 't');
	   
	      
			$infr_prin_query->fields('t', array('remarks_txt'));
		
           $infr_prin_query->condition('t.ins_file_number',trim($content7),'=');

          $infr_prin_query->condition('t.type_of_infring',$dataprin->type_of_infring,'=');
  
   
        
   
 
 $infr_prin_query_rst =  $infr_prin_query->execute(); 
 
 $numinfgM=$infr_prin_query_rst->rowCount();

    $prin_addi_infg=db_query('select  * from l_additional_infrigment where  ins_file_number = :filenumber  and type_of_infring=:tinf', array(':filenumber' => trim($content7),':tinf' => $dataprin->type_of_infring));
	$addnuminfgPrin=$prin_addi_infg->rowCount();
 if($numinfgM==0 && $addnuminfgPrin==0) {
	
	
	?>
    <tr bgcolor="<?php //print $bgcol ?>">
   
      <td colspan="3"  align="center"  > <center> <b>NO INFRINGEMENTS DETECTED </b></center>  </td>
   </tr>  
    	
	<?php	
	}
	
	else
	{
	 
	  $i=0;
     foreach ($infr_prin_query_rst as $infgp_result) { 
   
   //foreach ($content as $data) {
	 
		
		$i++;
	/*$personal_row=db_query('select  * from l_pemp_infringment where  ins_file_number = :filenumber and infring_id=:fid 
and type_of_infring=:tinf', array(':filenumber' => trim($content7),':fid' => trim($dataprin->ispection_id),':tinf' => 'P'));
	$numinfg=$personal_row->rowCount();*/
 if($i%2==0){
		        $bgcol = "#FCF6CF";
				 }
				else
				{
				$bgcol = "#FEFEF2";
				} 
?> 
<tr>
      <td  align="left" > <?php print $i;  ?> </td>
      <td  align="center" colspan="2" > <?php print $infgp_result->remarks_txt;  ?>  </td>
   <?php //if($numinfg > 0) { ?>
     <!-- <td  align="left" ><center> <b> <font color="DarkGreen"> <?php print " YES    " ; ?> </font> </b> </center></td>-->
      <?php
	   //} 
	  //else
	 // {
	  ?>
      <!-- <td  align="left" bgcolor="#FEFEF2"><center> <b> <font color="red"> <?php print " NO    " ; ?> </font> </b> </center></td>-->
       <?php  
	   
	 // }
	  ?>
    </tr> 
 
<?php
		}
		
$prin_addi_infg=db_query('select  * from l_additional_infrigment where  ins_file_number = :filenumber  and type_of_infring=:tinf', array(':filenumber' => trim($content7),':tinf' => $dataprin->type_of_infring));
	$addnuminfgPrin=$prin_addi_infg->rowCount();	
	if($addnuminfgPrin > 0) {
		?>
        <tr>
        <td colspan="3" align="center" style="text-align:center;font-size:17px; padding-top: 8px;"><font color="#3366FF"><strong> ADDITIONAL INFRINGEMENTS</strong></font><br/> </td>
    </tr>
     <?php   
	// $p=6;
	foreach($prin_addi_infg as $delta => $addidataPrin ){ 
	//$p++;
		?>
        
        
      <tr bgcolor="<?php //print $bgcol ?>">
     <!--<td  align="center"  > <?php print $p;  ?>  </td>-->
      <td colspan="3"  align="center"  > <center><?php print $addidataPrin->additional_infg;  ?> </center>  </td>
   </tr>   
		
<?php

	}
	}
	}
	}
?>
 </table> </div>
 <?php 
 
 if($numinfgM != 0 || $addnuminfgPrin != 0) { 
 
 ?>
<table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table">
 <tr style="text-align:center;font-size:17px; padding-top: 8px;">
      <td colspan="3"> <font color="#3366FF"><strong> Compliance Date :</strong></font> <b><?php  print date('dS M, Y', strtotime($verification_dt));?> </b> Time:  <b> <?php  print $verification_tm;  ?></b> </td>
</tr>
 <tr style="text-align:center;font-size:17px; padding-top: 8px;">
      <td colspan="3"> <font color="#3366FF"><strong> Compliance Place : </strong></font> <b><?php  print $ins_verify_place;?> </b>   </td>
</tr>
   </table> 
 <?php  
 }
 $contentdoc=array();
 
$query_document_fetch = db_query('select  *  from lc_signed_document_attachment a , file_managed b  where  a.ins_file_number=:insid and  a.fid=b.fid and a.typ_fl=:tp  ', array(':insid' => trim($content7),':tp' => 'O' ));
$query_insnot_doc=db_query('select  *  from lc_signed_document_attachment a  , l_infringment c  where a.ins_file_number=c.ins_file_number and a.type_of_infring=c.type_of_infring and  a.ins_file_number=:insid', array(':insid' => trim($content7) ));
	 
	 	if($query_document_fetch->rowCount() > 0) { 
 
 ?>
 
 <table width="100%" border="0" cellspacing="0" cellpadding="0" >            
     <tr >
      <td colspan="3" align="center" style="text-align:center;font-size:17px; padding-top: 8px;"><font color="#3366FF"><strong>
          Inspection note download/upload docment
      </strong></font></td>
      
    </tr>
     <tr>
      <th style="text-align:center;">INSPECTION ACT</th> 
      <th style="text-align:center;"> ORIGINAL DOCUMENT </th> 
      <th style="text-align:center;"> SIGNED DOCUMENT </th>
    </tr>
 
 <?php
 
 foreach($query_document_fetch as $data){
				$i++;
				
				$act_query_name = db_select('l_inspection_master', 'lim');
				$act_query_name->fields('lim', array('inspection_name'));
				$act_query_name->condition('lim.inspection_txt_type',$data->type_of_infring,'=');
	
				$act_query_res = $act_query_name->execute();
				$result = $act_query_res->fetchAssoc();
				$inspection_name = $result['inspection_name']; 
	
				$fid = $data->fid;	       
				$filename = $data->filename;
				
			 $ori_doc=$base_root.$base_path.'sites/default/files/insorigidoc/'.$filename;
		     $ori_doc_line=l(  t('Download') , $ori_doc  ,array('attributes' => array('target'=>'_blank'))) ;	
				
				
				$query_signed_document_fetch = db_query('select  *  from lc_signed_document_attachment a , file_managed b  where  a.ins_file_number=:insid and  a.fid=b.fid and a.typ_fl=:tp  ', array(':insid' => trim($content7),':tp' => 'S' ));
				$result2 = $query_signed_document_fetch->fetchAssoc();
				$filename2 = $result2['filename'];
				$type_of_infring = $result2['type_of_infring'];
				
				if($query_signed_document_fetch->rowCount() > 0) {
				
				 $signed_doc=$base_root.$base_path.'sites/default/files/inssigneddoc/'.$filename2;
		         $signed_doc_line=l(  t('Download') , $signed_doc  ,array('attributes' => array('target'=>'_blank'))) ;
				 
				}
				else
				{
				 $aa=$base_root.$base_path.'inspectionprint/'.encryption_decryption_fun('encrypt',$content7).'/'.encryption_decryption_fun('encrypt',$data->type_of_infring).'/inspectionprintpdf';
		        $signed_doc_line=l(t('GENERATE'), $aa,array('attributes' => array('target'=>'_blank'))) ;	
				}
 ?>
 <tr>
      <td align="center"> <?php print  strtoupper($inspection_name);  ?> </td> <td align="center" > <?php print  $ori_doc_line;  ?> </td> <td align="center" > <?php print  $signed_doc_line;  ?> </td>
      </tr>
 
 
<?php

       }
	   ?>
</table>
		
 

<?php

      }

		 elseif($query_insnot_doc->rowCount() > 0) 
            { 
			
			
			?>
   <table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table">            
     <tr >
      <td colspan="3" align="center" style="text-align:center;font-size:17px; padding-top: 8px;"><font color="#3366FF"><strong>
          Inspection note download/upload docment
      </strong></font></td>
      
    </tr>
     <tr>
      <th style="text-align:center;">INSPECTION ACT</th> 
      <th style="text-align:center;"> ORIGINAL DOCUMENT </th> 
      <th style="text-align:center;"> SIGNED DOCUMENT </th>
    </tr>        
            
            
            
	 <?php 
	// print "sss".$query_insnot_doc->rowCount().'ww'.$content7;
	 	foreach ($query_insnot_doc as $docobj) {
			$fid=$docobj->fid;
			$fid2=$docobj->fid2;
			$infring_name=$docobj->infring_name;
			$signed_doc_line='No Upload';
			$ori_doc_line='No Upload';
			if($fid){
		    $query_file_id1=db_query(' select   filename   from file_managed where  fid = :fid  ', array(':fid' => trim($fid) ));	
		
	         $result1=$query_file_id1->fetchAssoc();
	         $filename1 = $result1['filename']; 
			 
			 $signed_doc=$base_root.$base_path.'sites/default/files/inssigneddoc/'.$filename1;
		     $signed_doc_line=l(  t('Download') , $signed_doc  ,array('attributes' => array('target'=>'_blank'))) ;
			 
			}
			if($fid2){
			  $query_file_id2=db_query(' select   filename   from file_managed where  fid = :fid  ', array(':fid' => trim($fid2) ));	
		
	         $result2=$query_file_id2->fetchAssoc();
	         $filename2 = $result2['filename']; 
			
			
			
		
		     $ori_doc=$base_root.$base_path.'sites/default/files/insorigidoc/'.$filename2;
		     $ori_doc_line=l(  t('Download') , $ori_doc  ,array('attributes' => array('target'=>'_blank'))) ;
			 
			}
			 
			 
			 
		?>
         <tr>
      <td align="center"> <?php print  strtoupper($infring_name);  ?> </td> <td align="center" > <?php print  $ori_doc_line;  ?> </td> <td align="center" > <?php print  $signed_doc_line;  ?> </td>
      </tr>
        <?php	 
		
	    }
	
	?>
     </table>
     <?php
		
		} 
   
     ?>
   
 
<!--<form action="<?php print url('equalremuins/forward');?>" id="equalrappli" name="equalrappli" method="post" enctype="multipart/form-data">
          <input type="hidden" name="ins_file_no" id="ins_file_no" value="<?php echo $content7; ?>" />
          


</form>-->
     </fieldset></div></div> <?php //} ?>