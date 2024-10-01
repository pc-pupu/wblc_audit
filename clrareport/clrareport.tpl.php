<?php
global $base_path, $user;

/************************ CLRA Start*******************/

$clra_fetch_result = db_query("
select t1.clra_total_appli 
from (
select count(*) clra_total_appli
from l_clra_registration_master
where user_id != 0 and user_id is not null
and loc_e_dist is not null
and status = 'I'
and final_submit_status = 'P' 
and amendment_parent_id is null
) as t1")->fetchObject();
if(!empty($clra_fetch_result)){
	$total_clra_application = $clra_fetch_result->clra_total_appli;
}else{
	$total_clra_application = 0;
}

/*************************************************/


/************************ CLRA Amendment Start*******************/

$db_clra_amnd_report = db_query("
select t1.clra_total_appli 
from (
select count(*) clra_total_appli
from l_clra_registration_master
where user_id != 0 and user_id is not null
and loc_e_dist is not null
and status = 'I'
and final_submit_status = 'P' 
and amendment_parent_id is not null
) as t1")->fetchObject();

if(!empty($db_clra_amnd_report)){
	$total_clra_amnd_application = $db_clra_amnd_report->clra_total_appli;
}else{
	$total_clra_amnd_application = 0;
}

/*************************************************/


/************************ BOCWA Start********************/

$db_bocwa_report = db_select('l_bocwa_registration_master','lbrm');
$db_bocwa_report->fields('lbrm', array('id'));
$db_bocwa_report->isNotNull('lbrm.user_id');
$db_bocwa_report->isNotNull('lbrm.loc_e_subdivision');
$db_bocwa_report->condition('lbrm.final_submit_status', 'P', '=');
$db_bocwa_report->condition('lbrm.status', 'I', '=');
$total_bocwa_application = $db_bocwa_report->execute()->rowCount();

/************************ License Start********************/

$queryLicence = db_query("select sum(noappli) as totalsum from (
select count(*) noappli, district_code 
from l_particular_of_contract_labour pa, l_contractor_license_application la, district_master d
where la.contractor_particular_id = pa.id and la.contractor_user_id = pa.contractor_user_id and pa.worksite_dist=d.district_code
and pa.contractor_user_id is not null 
and la.application_id is not null 
and la.is_backlog_license is null 
and la.status is not null
and la.final_status = 'F'  
and la.status = 'I'
group by district_code
) t"); 

$total_license_application = $queryLicence->fetchObject()->totalsum;
/********************************************************/
				
/*$db_license_renewal_report = db_select('l_contractor_license_apply_renweal','lclar');
$db_license_renewal_report	->fields('lclar', array('id'));
$db_license_renewal_report->isNotNull('lclar.renewal_application_final_status');
$db_license_renewal_report->isNotNull('lclar.renewal_appliaction_status');
$total_license_renewal_application = count($db_license_renewal_report->execute()->fetchAll());*/


$db_license_renewal_report = db_query("
select count(*) noissuappli
from l_contractor_license_apply_renweal re
inner join l_contractor_license_application as la on (la.id  = re.lincense_id and la.contractor_user_id = re.created_by)
inner join l_particular_of_contract_labour as pa on (pa.id = la.contractor_particular_id and pa.serial_no_from_v = la.serial_no_from_v)
inner join district_master as d on (d.district_code = pa.worksite_dist)
where pa.contractor_user_id is not null 
and la.application_id is not null 
and re.renewal_application_final_status = 'F' 
and re.renewal_appliaction_status = 'I'
and re.created_by != 0
")->fetchObject();

if(!empty($db_license_renewal_report)){
	$total_license_renewal_application = $db_license_renewal_report->noissuappli;
}else{
	$total_license_renewal_application = 0;
}

/********************************************************/

$ins_list_query = db_select('l_inspection_establishment', 't');
$ins_list_query->join('l_custom_user_detail', 'l', 't.ins_user_id = l.usr_id');
$ins_list_query->join('l_infringment', 'c', 't.ins_file_number = c.ins_file_number');
$ins_list_query->fields('t', array('ins_file_number'));
$ins_list_query->condition('t.type_of_insnote_user','inspection_note','=');
$ins_list_query->condition('t.ins_submit_status','S','=');
$total_inspection_reports= count($ins_list_query->execute()->fetchAll());


$db_reg_contractor = db_select('l_contractor_license_application','cla');
$db_reg_contractor->fields('cla', array('contractor_user_id'));
$db_reg_contractor->condition('cla.status', 'I', '=');
$db_reg_contractor->groupBy('cla.contractor_user_id');
$total_register_contractor = $db_reg_contractor->execute()->rowCount();

$trade_union_query = db_select('tradeunion_master','tu');
$trade_union_query->fields('tu', array('id'));
$trade_union_query->condition('tu.is_active', 1,'=');
$trade_union_query->condition('tu.is_disable', 'N','=');
$total_register_trade_union = $trade_union_query->execute()->rowCount();


// mtw application

$mtw_fetch_result = db_query("
	select t1.mtw_total_appli, t2.mtw_issued_appli 
from (
select count(*) mtw_total_appli
from l_mtw_registration_master a, l_mtw_registration_renewal as ren
where a.user_id != 0 and a.user_id is not null and a.id = ren.application_id
and a.mtw_loc_dist is not null
and a.status is not null
and a.final_submit_status = 'P' 
) as t1,
(
select count(*) mtw_issued_appli
from l_mtw_registration_master a, l_mtw_registration_renewal as ren
where a.user_id != 0 and a.user_id is not null and a.id = ren.application_id
and a.mtw_loc_dist is not null
and a.status = 'I'
and a.final_submit_status = 'P' 
) as t2
")->fetchObject();

if(!empty($mtw_fetch_result)){
	$mtw_total_appli = $mtw_fetch_result->mtw_total_appli;
	$mtw_issued_appli = $mtw_fetch_result->mtw_issued_appli;
}else{
	$mtw_total_appli = $mtw_issued_appli = 0;
}

$total_mtw_ren_application = $mtw_issued_appli;


$db_ismw_report = db_select('l_interstate_workman_master','liwm');
$db_ismw_report->fields('liwm', array('id'));
$db_ismw_report->isNotNull('liwm.status');
$db_ismw_report->isNotNull('liwm.user_id');	
$db_ismw_report->isNotNull('liwm.loc_e_subdivision');
$db_ismw_report->isNotNull('liwm.loc_e_areatype_code');
$db_ismw_report->condition('liwm.final_submit_status', 'P', '=');
$db_ismw_report->condition('liwm.status', 'I', '=');
$total_ismw_application = $db_ismw_report->execute()->rowCount();

?>

<div class="row">
  <div class="col-md-4">
    <div class="report-box"> <a href="<?php print $base_path ?>clrareport">
      <h4>Principal Employer under CLRA</h4>
      <div class="totle-application">Regi./Amend. Issued Application<br /><strong><?php echo $total_clra_application;?> / <?php echo $total_clra_amnd_application;?></strong></div>
      <span>view details</span></a> </div>
  </div>
  <div class="col-md-4">
    <div class="report-box"> <a href="<?php print $base_path ?>clralincnreport">
      <h4>Contractor license under CLRA</h4>
       <div class="totle-application">Issued Application<br /><strong><?php echo $total_license_application ; ?></strong></div>
      <span>view details</span></a> </div>
  </div>
  <div class="col-md-4">
    <div class="report-box"> <a href="<?php print $base_path ?>clralinrenewalreport">
     <h4>Renewal of contractor license under CLRA</h4>
      <div class="totle-application">Issued Application<br /><strong><?php echo $total_license_renewal_application ; ?></strong></div>
     <span>view details</span></a> </div>
  </div>
  <div class="col-md-4">
    <div class="report-box"> <a href="<?php print $base_path ?>bocwareport">
      <h4>Establishment registration under BOCWA</h4>
       <div class="totle-application">Issued Application<br /><strong><?php echo $total_bocwa_application ; ?></strong></div>
      <span>view details</span></a> </div>
  </div>
  <div class="col-md-4">
    <div class="report-box"> <a href="<?php print $base_path ?>inspectionreport">
      <h4>Area wise view and download Inspection Report for various Labour Act</h4>
      <div class="totle-application">Total Application<br /><strong><?php echo $total_inspection_reports ; ?></strong></div>
      <span>view details</span></a> </div>
  </div>
  <div class="col-md-4">
    <div class="report-box"> <a href="<?php print $base_path ?>inspectionstrep">
      <h4>Acts wise view &amp; download inspection reports under various labour Acts &amp; Rule</h4>
      <div class="totle-application">Total Application<br /><strong><?php echo $total_inspection_reports ; ?></strong></div>
      <span>view details</span></a> </div>
  </div>
  <div class="col-md-4">
    <div class="report-box"> <a href="<?php print $base_path ?>search-act-and-area-wise-establishment-registration-information">
      <h4>Acts &amp; services wise view establishment registration certificate information</h4>
       <div class="totle-application">&nbsp;<br /><strong>&nbsp;</strong></div>
      <span>view details</span></a> </div>
  </div>
  <div class="col-md-4">
    <div class="report-box"> <a href="<?php print $base_path ?>register-contractor-information">
      <h4>Nature of work wise contractor information</h4>
      <div class="totle-application">Total Registered Contractor<br /><strong><?php echo $total_register_contractor; ?></strong></div>
      <span>view details</span></a> </div>
  </div>
  <div class="col-md-4">
    <div class="report-box"> <a href="<?php print $base_path ?>search-registered-trade-union">
      <h4>Registered Trade Union information in West Bengal</h4>
      <div class="totle-application">Total Registered Trade Union<br /><strong><?php echo $total_register_trade_union; ?></strong></div>
      <span>view details</span></a> </div>
  </div>
  <div class="col-md-4">
    <div class="report-box"> <a href="<?php print $base_path ?>mtwreport">
      <h4>Application of Establishment registration under MTW</h4>
       <div class="totle-application">Registration / Renewal of Application<br /><strong><?php echo $total_mtw_ren_application; ?></strong></div>
      <span>view details</span></a> </div>
  </div>
  
   <div class="col-md-4">
    <div class="report-box"> <a href="<?php print $base_path ?>ismwreport">
      <h4>Application of Establishment registration under ISMW</h4>
       <div class="totle-application">Issued Application<br /><strong><?php echo $total_ismw_application ; ?></strong></div>
      <span>view details</span></a> </div>
  </div>
  <div class="col-md-4">
    <div class="report-box"> <a href="<?php print $base_path ?>eodb-dashboard">
      <h4>EODB Dashboard</h4>
       <div class="totle-application">Online Dashboard for EODB e-services w.e.f. 1st November,2020<br /><strong></strong></div>
      <span>view details</span></a> </div>
  </div>
  <div class="col-md-4">
    <div class="report-box"> <a href="<?php print $base_path ?>/eodb-notice">
      <h4>EODB Notice</h4>
       <div class="totle-application">Notification of Ease of doing business<br /><strong></strong></div>
      <span>view details</span></a> </div>
  </div>
  <div class="col-md-4">
    <div class="report-box"> <a href="<?php print $base_path ?>feedback-draft">
      <h4>Feedback</h4>
       <div class="totle-application">Feedback on Draft Regulation<br /><strong></strong></div>
      <span>view details</span></a> </div>
  </div>
