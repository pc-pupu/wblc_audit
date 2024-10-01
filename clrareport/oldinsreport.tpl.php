<?php 
// drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');
global $base_root,$base_path; 
	
$fileid = '';
?>
<table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table">
    <tr>
         <th>Parameters</th>
         <th>Inputs</th>
    </tr>
	<tr>
		<td colspan="2" align="center" style="font-size:15px;"><strong>Establishment / Employment / Shop Details</strong></td>
	</tr>
	<?php 
	$fileid = '';
            
	foreach($content as $delta => $data){ 
		if($data->category == 'V'){
			$area_type = "Block";
			$area = 'Gram Panchayat';
			$area_name = ucwords($data->village_name);
		}elseif($data->category == 'W'){
			$area_type = "Municipality";
			$area = 'Ward';
			$area_name = 'Ward - '.$data->village_name;
		}elseif($data->category == 'C'){
			$area_type = "Corporation";
			$area = 'Ward';
			$area_name = $data->village_name;
		}elseif($data->category == 'S'){
			$area_type = "SEZ";
			$area = 'Sector';
			$area_name = $data->village_name;
		}
			
  		$fileid = trim($data->file_id);
    ?>
     <tr>
          <td> Name of the Establishment / Employment / Shop  </td>
          <td><?php print $data->name_of_est;  ?></td>
    </tr>
    
    <tr>
      <td>Nature of industry/business : </td>
      <td><?php if($data->old_nature_id=='28' ) { print $data->other_nature_nm; } else { print $data->cont_work_name; }  ?></td>
    </tr>
    <tr>
        <td colspan="2" style="font-size:15px;"><strong>Address Of Establishment / Employment / Shop</strong></td>
    </tr>
    <tr>
      <td>Address Line1 </td>
      <td><?php print $data->address_line;  ?></td>
    </tr>
    <tr>
      <td>District </td>
      <td><?php print $data->district_name;  ?></td>
    </tr>
    <tr>
      <td>Sub-Division </td>
      <td><?php print $data->sub_div_name;  ?></td>
    </tr>
    <tr>
      <td><?php echo $area_type; ?>  </td>
      <td><?php print $data->block_mun_name;  ?></td>
    </tr>
    <?php
	if(!empty($area)){
	?>
   <tr>
      <td><?php echo $area; ?>  </td>
      <td><?php print $area_name;  ?></td>
    </tr>
    <?php
	}
	?>
    <tr>
      <td>Police Station</td>
      <td><?php print $data->name_of_police_station;  ?></td>
    </tr>
    <tr>
      <td>Pin Code</td>
      <td><?php print $data->pincode;  ?></td>
    </tr>
    <tr>
		<td colspan="2" style="font-size:15px;"><strong>Workers Details</strong></td>
	  </tr>
      <tr>
      <td>No of workman/employed directly : </td>
      <td> Male:<?php print $data->workmen_male;?> &nbsp; Female <?php print $data->workmen_female;  ?> &nbsp; Total:<?php print $data->workmen_total;  ?> </td>
    </tr>
     <tr>
      <td>No of contract labour : </td>
      <td> Male:<?php print $data->contractor_male;?> &nbsp; Female <?php print $data->contractor_female;  ?> &nbsp; Total:<?php print $data->contractor_total;  ?> </td>
    </tr>
     <tr>
    <td>No of other worker if engaged : </td>
    <td> Male:<?php print $data->other_wk_male;  ?> &nbsp; Female <?php print $data->other_wk_female;  ?> &nbsp; Total:<?php print $data->other_wk_total;  ?> </td>
    </tr>
    <?php 
	}	
	?>
	
    <tr><td colspan="2" style="font-size:15px;"><strong>Inspection Note(PDF) View Documents</strong></td></tr>
        
    <?php 	
	$query_document_fetch = db_query('select  *  from l_old_ins_note_upload a  , file_managed c  where a.fid=c.fid  and  a.file_id=:insid ', array(':insid' => trim($fileid) ));
 
	 
	 //print $query_document_fetch->rowCount();
	 
		
		 if($query_document_fetch->rowCount() > 0) {
		 
	  
	
	   
		$i=0;
	      
		foreach ($query_document_fetch as $docobj) {
			$i++;
			 $fid=$docobj->fid;
			 $file_id=$docobj->file_id;
			 $filename = $docobj->filename; 
	        
			 
			 
		
			
$query_ins_nm = db_query('select inspection_name from l_inspection_master where inspection_txt_type = :instype limit 1 ', array(':instype' => trim($docobj->type_of_infring) ));	
		
	    $result			=   $query_ins_nm->fetchAssoc();
	    $infring_name   =   $result['inspection_name'];
	
	$infg_ori_doc=$base_root.$base_path.'sites/default/files/insolddoc/'.$filename;
			
			 
			$infgdocpg = '<img  src="'.$base_root.$base_path.'/sites/all/themes/lcTheme/images/pdf.png">'; 
		     $ori_infg_prf=l(  t($infgdocpg) , $infg_ori_doc  ,array('attributes' => array('target'=>'_blank'),'html'=>TRUE)) ;
	
	
	 ?>
    
    
    <tr>
     <td> <?php print $infring_name; ?> </td>
     <td><?php print $ori_infg_prf; ?></td>
 </tr> 
    
    
	<?php
	
	}
		 }
	$query_insp_addiinfo= db_select('l_old_ins_contractor_info', 'p');
	$query_insp_addiinfo->leftjoin('state_master', 'sm', 'sm.id = p.state_id');
	$query_insp_addiinfo->leftjoin('district_master', 'd', 'd.district_code = p.dis_id_con and d.state_id = sm.id'); 
	$query_insp_addiinfo->leftjoin('sub_division', 's', 's.sub_div_code = p.sub_div_con and d.district_code = s.district_code ');
	$query_insp_addiinfo->leftjoin('block_mun_master', 'bm', 'bm.block_code = p.block_id_con and bm.distict_code = d.district_code and s.sub_div_code = bm.sub_division_code');
	
	$query_insp_addiinfo->leftjoin('village_ward_master', 'v', 'v.village_code = p.vill_wd_con and v.block_code = bm.block_code');
	$query_insp_addiinfo->leftjoin('police_station', 'r', 'r.police_station_code = p.police_st and r.dristrict_code = d.district_code');
	$query_insp_addiinfo->fields('d', array('district_name','district_code'));
	$query_insp_addiinfo->fields('s', array('sub_div_name')); 
	$query_insp_addiinfo->fields('bm', array('block_mun_name')); 
	$query_insp_addiinfo->fields('sm', array('statename')); 
	$query_insp_addiinfo->fields('v', array('village_name','category')); 
	$query_insp_addiinfo->fields('r', array('name_of_police_station'));
	$query_insp_addiinfo->fields('p', array('ins_file_number','type_of_infring','con_address','state_id','dis_id_con','sub_div_con','police_st','vill_wd_con','block_id_con','con_id','nm_con' ,'con_own_pin'));
	$query_insp_addiinfo->condition('p.ins_file_number',trim($fileid),'=');
	
	
	 $query_insp_addiinfo_result = $query_insp_addiinfo->execute();  
  
         if($query_insp_addiinfo_result->rowCount() > 0) 
              { 
		?>
         
   
         <tr>
      <td colspan="2" style="font-size:15px;"><strong>Contractor Information</strong></td>
    
    </tr>         
        <?php	  
			  
			  
              foreach($query_insp_addiinfo_result as $squan) {
		                  $i++;
	                       $nm_con=trim($squan->nm_con);
	                       $con_address=trim($squan->con_address);
						 
						   $district_name=trim($squan->district_name);	
						   $sub_div_name=trim($squan->sub_div_name);
						   $block_mun_name=trim($squan->block_mun_name);
						   $village_name=trim($squan->village_name);
						   $name_of_police_station=trim($squan->name_of_police_station);
						 
						   $statename=trim($squan->statename);
						   $con_own_pin=trim($squan->con_own_pin);
  
    ?>
    
    <tr>
      <td>Contractor Name </td>
      <td><?php print $nm_con;  ?></td>
    </tr>
    
     <tr>
      <td>Address  </td>
      <td> Address Line : <?php print $con_address;  ?> , State Name : <?php print $statename;  ?> , District :<?php print $district_name;  ?> , Sub-Division : <?php print $sub_div_name;  ?><br /><br /> Block Name :  <?php print $block_mun_name;  ?> , Village Name: <?php print $village_name;  ?>  , Police Station : <?php print $name_of_police_station;  ?> ,Pin Code:  <?php print $con_own_pin;  ?> </td>
    </tr>
  
    
    
    <?php
			  }
			  }

   $aa=$base_root.$base_path.'insdataentrylist';
   $link_s=l(t('Back'), $aa);   
	
	?>
 <!--<tr>
     <td colspan="2"><font color="#006595"><strong><center> <?php //print $link_s; ?></center></strong></font> </a></td>
 </tr>-->	
	
	
	
    </table>