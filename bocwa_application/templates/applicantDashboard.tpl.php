<style type="text/css">
.heading{ 	
	font-size:20px;
	float:left;
	padding-bottom: 7px;
}

.varified{
	background-color: #006600;
    color: #fff;
    font-weight: bold;
    padding: 2px 1px 2px 4px;
    text-transform: uppercase;
    width: 410px;
}

</style>
<?php 
		global $base_root, $base_path, $user; 
?>

<table width="100%" border="0" class="application_view">

  <tr>  	
	<td colspan="2" style="border:none;">Contract labour (Regulation & Abolition) Application successfully verified and approved by ALC. <?php echo l(t('Payment'), 'epayments', array('attributes' => array('class' => 'styled-button-2')));?></td>
 </tr>
  <tr>
  	<td>Name:</td>
	<td ><?php echo $param1['fullname']; ?></td>
 </tr>
 <tr>
  	<td>Mobile Number:</td>
	<td ><?php echo $param1['uid_phone']; ?></td>
 </tr>
 <tr>
  	<td>Email:</td>
	<td ><?php echo $param1['mail']; ?></td>
 </tr>
</table>

<br /><br />


<table width="100%" border="0" class="application_view">
  	<tr>
  		<td colspan="4">Total Number of Contractors: &nbsp;&nbsp;&nbsp;&nbsp;<a href="applicant-profile/clra-contractor-info"><?php echo $count_res_1['id_count']; ?></a></td>
 	</tr>
 </table>
<br /><br /><br />

<h1 class="heading"><img alt="" src="/sites/all/themes/jackson/images/readmore.gif">Grips Transaction Details</h1>

<table width="100%" border="0" class="application_view">   
  <tr>
  	<th>SL.No.</th>
    <th>Event</th>
    <th>Amount</th>
	<th>Date</th>    
    <th>Transaction Status</th>
    <th>Actions</th>
  </tr>
  <tr>
  	<td>1.</td>
    <td>Contract labour (Regulation & Abolition) Application</td>
    <td></td>
	<td></td>    
    <td></td>
    <td><?php echo l('Click here to view details','#');?></td>
 </tr>
</table>

<br />


<h1 class="heading"><img alt="" src="/sites/all/themes/jackson/images/readmore.gif">THE CONTRACT LABOUR (REGULATION & ABOLITION) ACT STATUS</h1>
  <table width="100%" border="0" class="application_view">
       <tr>
          <th width="18%" style="border-right:none;"><strong>Serial No</strong></th>          
          <th width="18%" style="border-right:none;"><strong>Name</strong></th>
          <th width="18%" style="border-right:none;"><strong>Identification Number</strong></th>          
          <th width="18%" style="border-right:none;"><strong>Registration Number</strong></th>
          <th width="18%" style="border-right:none;"><strong>Application Date</strong></th>
          <th width="10%" style="border-right:none;"><strong>Status</strong></th>
      </tr>
      <?php
	  	if( $application_act_status["final_submit_status"] == "B" || $application_act_status["final_submit_status"] == "V" || $application_act_status["final_submit_status"] == "P" ){
	  ?>
	  <tr>
          <td style="border-right:none;">1</td>
          <td style="border-right:none;"><?php echo $application_act_status["full_name_principal_emp"]; ?></td>
          <td style="border-right:none;"><?php echo $_SESSION['sess_iden_num']; ?></td>
          <td style="border-right:none;"><?php echo $application_act_status["registration_number"]; ?></td>          
          <td><?php echo $application_act_status["application_date"]; ?></td>
          <td style="border-right:none;">
          
		  <?php if( $application_act_status["final_submit_status"] == "B" ){ ?>
			  <img alt="" src="<?php echo $base_root.$base_path; ?>sites/all/modules/applicant_forms/images/backed.png" style="width: 20px; height: 16px; padding: 11px 0 0 20px;">
		  <?php }else if( $application_act_status["final_submit_status"] == "P" ){ ?>
			  <img alt="" src="<?php echo $base_root.$base_path; ?>/sites/all/modules/applicant_forms/images/pending.png" style="width: 20px; height: 16px; padding: 11px 0 0 20px;">
		  <?php }else if( $application_act_status["final_submit_status"] == "V" ){ ?>
			  <img alt="" src="<?php echo $base_root.$base_path; ?>/sites/all/modules/applicant_forms/images/approved-icon.png" style="width: 20px; height: 16px; padding: 11px 0 0 20px;">
		  <?php }; ?>
		  </td>
      </tr>
      <?php } ?>
      <tr>      	  
          <td colspan="6" style="border-left: 1px solid #cccccc;"><i><b>Note: </b> <img alt="" src="<?php echo $base_root.$base_path; ?>/sites/all/modules/applicant_forms/images/approved-icon.png" style="width: 20px; height: 16px;"> = Final Submit, <img alt="" style="width: 20px; height: 16px;" src="<?php echo $base_root.$base_path; ?>sites/all/modules/applicant_forms/images/backed.png"> = Back For Correction, <img alt="" src="<?php echo $base_root.$base_path; ?>/sites/all/modules/applicant_forms/images/pending.png" style="width: 20px; height: 16px;"> = Verified & Forward to ALC</i></td>
      </tr>
  </table>
