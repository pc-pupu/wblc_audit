<?php
global $base_url;
$result = db_query("select t.*, fm.filename, fm.uri  from (
select n.nid, n.title, fdb.body_value, fdfmi.field_minister_image_fid from node as n
inner join field_data_body as fdb on n.nid = fdb.entity_id
inner join field_data_field_minister_image as fdfmi on n.nid = fdfmi.entity_id
where n.type = 'ministry_content') as t
inner join file_managed as fm on t.field_minister_image_fid= fm.fid order by t.nid asc limit 3
");
?>

<div id="ministry_main">
<ul>
<?php 
$temp = 0;
while($record = $result->fetchObject()) { ?>
<li><h3 style="font-size:13px;text-transform:uppercase;"><?php echo $record->title; ?></h3>
 <?php
 if($temp == 1)
 	$linkPath = "shramik-mela";
 if($temp == 0)
    $linkPath = "sites/default/files/Sramik Barta_corrected.pdf";
 if($temp == 2)
    $linkPath = "sop-e-services";
	
   echo l(t('<img src="'.base_path(). 'sites/default/files/Minister/'.$record->filename.'" width="270" height="200">'),$linkPath, array('html' => TRUE,'attributes'=>array('target' => '_blank'))); ?>
<!--<p><?php // echo substr($record->body_value, 0, 350).'.. '.l('Read More', 'node/'.$record->nid); ?></p>--></li>
<?php $temp++;} ?>
</ul>

</div>