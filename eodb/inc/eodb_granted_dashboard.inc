<?php
function eodb_granted_dashboard() {
   global $base_url;
    /* CLRA REGISTRATION SECTION April 1st 2020 = 1585699200000 **/
	
	
   $output = '';
   $output .='<div class="row">
				  <div class="col-sm-6">
				  	<table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table fixed_headers">
				  	<thead>
						<tr><th colspan="5" style="background:#fff;color:#3a2310;text-align:center;">Registration of Principal Employers under Contract Labour (Regulation &amp; Abolition) Act 1970.</th></tr>
						<tr>
							<th>Particulars</th>
							<th>Micro</th>
							<th>Small</th>
							<th>Medium</th>
							<th>Large</th>
						</tr>
				    </thead>
					<tbody>
						<tr>
							<td>Total limit prescribed as per the Public Service Guarantee Act</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Total Number of applications received</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Total Number of applications received from businesses with female owners</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Total Number of applications approved</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Total Number of applications approved of businesses with female owners</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Average time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Median time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Minimum time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Maximum time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
					</tbody>
				  </table>
				  </div>';
				  
	/* CLRA AMENDMENT SECTION **/
    
	$output.='	  <div class="col-sm-6">
				 	 <table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table fixed_headers">
				  	<thead>
						<tr><th colspan="5" style="background:#fff;color:#3a2310;text-align:center;">Amendment of registration certificate for Principal Employers under Contract Labour (Regulation & Abolition) Act 1970.</th></tr>
						<tr>
							<th>Particulars</th>
							<th>Micro</th>
							<th>Small</th>
							<th>Medium</th>
							<th>Large</th>
						</tr>
				    </thead>
					<tbody>
						<tr>
							<td>Total limit prescribed as per the Public Service Guarantee Act</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Total Number of applications received</td>
							<td>'.$clraAmendReceivedCountMicro.'</td>
							<td>'.$clraAmendReceivedCountSmall.'</td>
							<td>'.$clraAmendReceivedCountMedium.'</td>
							<td>'.$clraAmendReceivedCountLarge.'</td>
						</tr>
						<tr>
							<td>Total Number of applications received from businesses with female owners</td>
							<td>'.$clraAmendReceivedCountMicroFemale.'</td>
							<td>'.$clraAmendReceivedCountSmallFemale.'</td>
							<td>'.$clraAmendReceivedCountMediumFemale.'</td>
							<td>'.$clraAmendReceivedCountLargeFemale.'</td>
						</tr>
						<tr>
							<td>Total Number of applications approved</td>
							<td>'.$clraAmendReceivedCountMicroApproved.'</td>
							<td>'.$clraAmendReceivedCountSmallApproved.'</td>
							<td>'.$clraAmendReceivedCountMediumApproved.'</td>
							<td>'.$clraAmendReceivedCountLargeApproved.'</td>
						</tr>
						<tr>
							<td>Total Number of applications approved of businesses with female owners</td>
							<td>'.$clraAmendReceivedCountMicroFemaleApproved.'</td>
							<td>'.$clraAmendReceivedCountSmallFemaleApproved.'</td>
							<td>'.$clraAmendReceivedCountMediumFemaleApproved.'</td>
							<td>'.$clraAmendReceivedCountLargeFemaleApproved.'</td>
						</tr>
						<tr>
							<td>Average time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Median time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Minimum time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Maximum time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
					</tbody>
				  </table>
				  </div>';
	/* CLRA LICENSE SECTION **/				  
	$output.='	  <div class="col-sm-6">
				 	 <table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table fixed_headers">
				  	<thead>
						<tr><th colspan="5" style="background:#fff;color:#3a2310;text-align:center;">Licensing for Contractors under Contract Labour (Regulation & Abolition) Act 1970.</th></tr>
						<tr>
							<th>Particulars</th>
							<th>Micro</th>
							<th>Small</th>
							<th>Medium</th>
							<th>Large</th>
						</tr>
				    </thead>
					<tbody>
						<tr>
							<td>Total limit prescribed as per the Public Service Guarantee Act</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Total Number of applications received</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Total Number of applications received from businesses with female owners</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Total Number of applications approved</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Total Number of applications approved of businesses with female owners</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Average time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Median time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Minimum time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Maximum time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
					</tbody>
				  </table>
				  </div>';
				  
	/* CLRA LICENSE AMENDMENT SECTION **/
	$output.='	  <div class="col-sm-6">
				 	 <table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table fixed_headers">
				  	<thead>
						<tr><th colspan="5" style="background:#fff;color:#3a2310;text-align:center;">Amendment of License/Renewal for Contractors under Contract Labour (Regulation & Abolition) Act 1970.</th></tr>
						<tr>
							<th>Particulars</th>
							<th>Micro</th>
							<th>Small</th>
							<th>Medium</th>
							<th>Large</th>
						</tr>
				    </thead>
					<tbody>
						<tr>
							<td>Total limit prescribed as per the Public Service Guarantee Act</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Total Number of applications received</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Total Number of applications received from businesses with female owners</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Total Number of applications approved</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Total Number of applications approved of businesses with female owners</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Average time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Median time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Minimum time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Maximum time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
					</tbody>
				  </table>
				  </div>';
	
	/*BOCWA REGISTRATION SECTION **/
	
	$output.='	  <div class="col-sm-6">
				   <table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table fixed_headers">
				  	<thead>
						<tr><th colspan="5" style="background:#fff;color:#3a2310;text-align:center;">Registration of Establishments under Building and Other Construction Workers (Regulation of Employment and Conditions of Service) Act, 1996.</th></tr>
						<tr>
							<th>Particulars</th>
							<th>Micro</th>
							<th>Small</th>
							<th>Medium</th>
							<th>Large</th>
						</tr>
				    </thead>
					<tbody>
						<tr>
							<td>Total limit prescribed as per the Public Service Guarantee Act</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Total Number of applications received</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Total Number of applications received from businesses with female owners</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Total Number of applications approved</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Total Number of applications approved of businesses with female owners</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Average time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Median time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Minimum time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Maximum time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
					</tbody>
				  </table>
				  </div>';
				  
	/*ISMW REGISTRATION SECTION **/
	

			  
	$output.='	  <div class="col-sm-6">
				  <table width="100%" border="0" cellspacing="0" cellpadding="0" class="view-act-rules-table fixed_headers">
				  	<thead>
						<tr><th colspan="5" style="background:#fff;color:#3a2310;text-align:center;">Registration of Principal Employers under Inter State Migrant Workmen (Regulation of Employment and Conditions of Service) Act, 1979</th></tr>
						<tr>
							<th>Particulars</th>
							<th>Micro</th>
							<th>Small</th>
							<th>Medium</th>
							<th>Large</th>
						</tr>
				    </thead>
					<tbody>
						<tr>
							<td>Total limit prescribed as per the Public Service Guarantee Act</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Total Number of applications received</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Total Number of applications received from businesses with female owners</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Total Number of applications approved</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Total Number of applications approved of businesses with female owners</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Average time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Median time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Minimum time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
						<tr>
							<td>Maximum time taken to obtain registration/renewal</td>
							<td></td>
							<td></td>
							<td></td>
							<td></td>
						</tr>
					</tbody>
				  </table>
				  </div>
			  </div>';

	return $output;
	
	
}

							
					
                   