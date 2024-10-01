<?php
drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/applicantform.css');

function cms_dash_board(){
    global $base_root, $base_path, $full_base, $user;
    
    $output = '<div class="row"><div class="col-md-12"><div class="box box-primary"><div class="box-body"><font size="4"><i>Thank you for choosing Admin Panel as your website management. This screen will give you a quick overview of all the various statistics of your website. The links on the left hand side of this screen allow you to control every aspect of your web experience. Each page will have instructions on how to use the tools.</i></font></div></div></div></div>';
	
	$pathedit_icon = path_to_theme().'/images/edit_icon.png';
    $preview_icon = path_to_theme().'/images/preview.jpg';  
    
    $output .='<div class="row"><div class="col-md-12"><div class="box-header with-border" style="padding:0px;padding-bottom:10px;"><i class="ion ion-clipboard"></i> Symbol Information</div>
				   <div class="box box-primary"><div class="box-body">				   
				   <table width="100%" border="0" class="table table-bordered table-responsive">			
					   <tr>
						  <th width="8%" align="center">Symbole</th>
						  <th>Meaning</th>
						  <th width="8%" align="center">Symbole</th>
						  <th>Meaning</th>
						  <th width="8%" align="center">Symbole</th>
						  <th>Meaning</th>
					  </tr>
					  <tr>
						<td align="center"><i class="fa fa-eye" aria-hidden="true" style="font-size:20px;"></td>
						<td>View Details</td>
						<td align="center"><i class="fa fa-edit" aria-hidden="true" style="font-size:20px;"></td>
						<td>Edit</td>
						<td align="center"><i class="fa fa-trash-o" aria-hidden="true" style="font-size:20px;"></td>
						<td>Delete</td>
					</tr>
					 <tr>
						<td align="center"><i class="fa fa-check-circle" aria-hidden="true" style="color:green;font-size:20px;"></td>
						<td>Active</td>
						<td align="center"><i class="fa fa-times-circle" aria-hidden="true" style="color:red;font-size:20px;"></td> 
						<td>Inactive</td>
						<td align="center"><i class="fa fa-times-circle" aria-hidden="true" style="color:#A1A1A1;font-size:20px;"></td>
						<td>Disable</td>
					</tr>
				</table>'; 
    
    return $output;
}

