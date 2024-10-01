<?php
function samajik_surahsha_yojana_notice(){
	$output = '';
	$output	.= '<div class="row">
				  <div class="col-sm-12">										
					  <div id="content-2" class="scrollContent content mCustomScrollbar light" data-mcs-theme="minimal-dark">
						  <table width="100%" border="1" cellspacing="0" cellpadding="0" class="view-act-rules-table fixed_headers">
						  	<thead>								
							  <tr>								
								<th width="10%">Sl. No.</th>
								<th width="35%">Title</th>
								<th width="20%">view</td>														
							  </tr>
							  <tr>								
									<td width="18%">1</td>
									<td>SSY Notification</td>	
									<td>'.l('<img src="https://wblc.gov.in/sites/all/themes/lcTheme/images/pdf.png" style="border:none;">', '/sites/default/files/upload/ssy/Schedule_Amendment_WBUSWWACT_2007.pdf', array('html' => TRUE,'attributes'=>array('target' => '_blank'))).'</td>							
							  </tr>							  
							  <tr>								
									<td width="18%">2</td>
									<td>SSY Notification</td>	
									<td>'.l('<img src="https://wblc.gov.in/sites/all/themes/lcTheme/images/pdf.png" style="border:none;">', '/sites/default/files/upload/ssy/Repeal_SASPFUW_WBBWWS_WBUSWHSS.pdf', array('html' => TRUE,'attributes'=>array('target' => '_blank'))).'</td>							
							  </tr>	
							  <tr>								
									<td width="18%">3</td>
									<td>SSY Notification</td>	
									<td>'.l('<img src="https://wblc.gov.in/sites/all/themes/lcTheme/images/pdf.png" style="border:none;">', '/sites/default/files/upload/ssy/Revised_BOCWA_Scheme.pdf', array('html' => TRUE,'attributes'=>array('target' => '_blank'))).'</td>							
							  </tr>	
							  <tr>								
									<td width="18%">4</td>
								<td>SSY Notification</td>	
								<td>'.l('<img src="https://wblc.gov.in/sites/all/themes/lcTheme/images/pdf.png" style="border:none;">', '/sites/default/files/upload/ssy/Revised_WBTransportWWS.pdf', array('html' => TRUE,'attributes'=>array('target' => '_blank'))).'</td>							
							  </tr>	
							  <tr>								
									<td width="18%">5</td>
									<td>SSY Notification</td>	
									<td>'.l('<img src="https://wblc.gov.in/sites/all/themes/lcTheme/images/pdf.png" style="border:none;">', '/sites/default/files/upload/ssy/Samajik_Suraksha_Yojana_2017.pdf', array('html' => TRUE,'attributes'=>array('target' => '_blank'))).'</td>							
							  </tr>
							  <tr>								
									<td width="18%">6</td>
									<td>SSY Notification</td>	
									<td>'.l('<img src="https://wblc.gov.in/sites/all/themes/lcTheme/images/pdf.png" style="border:none;">', '/sites/default/files/upload/ssy/FORM_I_SSY_2017.pdf', array('html' => TRUE,'attributes'=>array('target' => '_blank'))).'</td>							
							  </tr>						  							  
							</tbody>
						</table>
					</div>
				</div>
			</div>';
				
	return $output;
}