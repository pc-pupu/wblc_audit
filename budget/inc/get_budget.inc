<?php
function get_budget($form,&$form_state){
	$bugdet_markup = bugdet_markup_table();
	$form['bugdet_markup'] = array(
		'#type' => 'markup',
		'#markup' => $bugdet_markup
	);
	return $form;
}

function bugdet_markup_table(){
	$output = '';
	
	$output	.= '<div class="row">
				  <div class="col-sm-12">										
					  <div id="content-2" class="scrollContent content mCustomScrollbar light" data-mcs-theme="minimal-dark">
						  <table width="100%" border="1" cellspacing="0" cellpadding="0" class="view-act-rules-table fixed_headers">
						  	<thead>
								<tr>
									<td colspan="3"><strong>Budget Allocation for the Financial Year 2020-21 for State Development Schemes administered by the Labour Commissionerate</strong></td>
								<tr>
							  <tr>								
								<th width="20%">Head of Account Code</th>
								<th>Scheme</th>
								<th width="20%">Budget Allocation 2020-21</td>														
							  </tr>
							  <tr>								
								<td width="18%">2230-01-001-00-007-V-13-04</td>
								<td>Strengthening of enforcement machinery or the Labour Directorate</td>	
								<td align="right">₹ 1,70,00,000/-</td>							
							  </tr>
							  <tr>								
								<td>2230-01-001-00-008-V-13-03</td>
								<td rowspan="2">Strengthening of Training Institute-cum-Central Library</td>
								<td>₹3,00,000/-</td>								
							  </tr>
							  <tr>
								<td>2230-01-001-00-008-V-13-04</td>
								<td>₹40,00,000/-</td>								
							  </tr>
							  <tr>								
								<td>2230-01-004-00-007-V-13-04</td>
								<td>Improvement of Labour Statistics</td>
								<td>₹9,00,000/-</td>								
							  </tr>
							  <tr>								
								<td>2230-01-101-00-004-V-13-04</td>
								<td>Improvement of Working Conditions of Child and Woman Labour</td>
								<td>₹20,00,000/-</td>								
							  </tr>
							  <tr>								
								<td>2230-01-101-00-007-V-13-03</td>
								<td rowspan="3">Statewide Survey to identify Child Labour in different employment</td>
								<td>₹3,00,000/-</td>								
							  </tr>
							  <tr>
								<td>2230-01-101-00-007-V-13-04</td>
								<td>₹7,00,000/-</td>								
							  </tr>
							  <tr>
								<td>2230-01-101-00-007-V-31-02</td>
								<td>₹35,00,000/-</td>								
							  </tr>
							  <tr>								
								<td>2230-02-001-00-003-V-77-00</td>
								<td>National e-Governance Action Plan (Total Labour Department)</td>
								<td>₹15,00,00,000/-</td>								
							  </tr>
							  <tr>								
								<td>2235-60-200-00-085-V-31-02</td>
								<td>Samajik Suraksha Yojana</td>
								<td>₹368,00,00,000/-</td>								
							  </tr>
							  <tr>								
								<td rowspan="2">2235-60-200-00-036-V-50-00</td>
								<td>Scheme for Financial Assistance to the Workers in Locked out</td>
								<td rowspan="2">₹48,53,57,000/-</td>								
							  </tr>
							  <tr>								
								<td>Industrial Units</td>			
							  </tr>	
							  <tr>								
								<td colspan="2" style="text-align:center;"><strong>TOTAL</strong></td>
								<td><strong>₹4344057000/-</strong></td>							
							  </tr>						 
							</tbody>
						</table>
					</div>
				</div>
			</div>';
			
	$output .= '<p>'.l('<i class="fa fa-file" aria-hidden="true"></i>&nbsp;Click here to Print Budget 2020-21', 'https://wblc.gov.in/sites/default/files/budget-details-2020-21.pdf', array('html' => TRUE,'attributes'=>array('target' => '_blank'))).'</p>';		
			
	$output	.= '<div class="row">
				  <div class="col-sm-12">										
					  <div id="content-2" class="scrollContent content mCustomScrollbar light" data-mcs-theme="minimal-dark">
						  <table width="100%" border="1" cellspacing="0" cellpadding="0" class="view-act-rules-table fixed_headers">
						  	<thead>
								<tr>
									<td colspan="3"><strong>Budget Details (F.Y. 2017-18)</strong></td>
								<tr>
							  <tr>								
								<th width="20%">Head of Account Code</th>
								<th width="35%">Scheme</th>
								<th width="20%">Budget Estimate 2017-18</td>														
							  </tr>
							  <tr>								
								<td width="18%">2230-01-001-SP-001-V-13-04</td>
								<td>Strengthening of enforcement machinery or the Labour Directorate</td>	
								<td>15,000,000</td>							
							  </tr>
							  <tr>								
								<td>2230-01-001-SP-002-V-13-03</td>
								<td rowspan="2">Strengthening of Training Institute-cum-Central Library</td>
								<td>300,000</td>								
							  </tr>
							  <tr>
								<td>2230-01-001-SP-002-V-13-04</td>
								<td>4,700,000</td>								
							  </tr>
							  <tr>								
								<td>2230-01-004-SP-001-V-13-04</td>
								<td>Improvement of Labour Statistics</td>
								<td>1,00,000</td>								
							  </tr>
							  <tr>								
								<td>2230-01-101-SP-003-V-33-05</td>
								<td rowspan="3">Bidi Workers Welfare Scheme</td>
								<td></td>								
							  </tr>
							  <tr>
								<td>2230-01-101-SP-003-V-50</td>
								<td></td>								
							  </tr>
							  <tr>
								<td>2230-01-10%9-SP-001-V-13-04</td>
								<td></td>								
							  </tr>
							  <tr>								
								<td>2230-01-101-SP-004-V-13-04</td>
								<td>Improvement of Working Conditions of Child and Woman Labour</td>
								<td>3,000,000</td>								
							  </tr>
							  <tr>								
								<td>2230-01-101-SP-007-V-13-03</td>
								<td width="739" rowspan="3">Statewide Survey to Identity Child Labour in different employment</td>
								<td>300,000</td>								
							  </tr>
							  <tr>
								<td width="181" height="14">2230-01-101-SP-007-V-13-04</td>
								<td>700,000</td>								
							  </tr>
							  <tr>
								<td width="181" height="14">2230-01-101-SP-007-V-31-02</td>
								<td>3,000,000</td>								
							  </tr>
							  <tr>								
								<td>2230-02-001-SP-003-V-77</td>
								<td>National e-Governance Action Plan[LB]</td>
								<td>100,000,000</td>								
							  </tr>
							  <tr>								
								<td>2235-60-200-SP-005-V-31-02</td>
								<td rowspan="3">Provident Fund Scheme for Unorganised Workers in Urban and Rural Areas[SASPFUW]</td>
								<td>1,506,900,000</td>								
							  </tr>
							  <tr>								
								<td>2235-60-789-SP-003-V-31-02</td>
								<td>480,000,000</td>								
							  </tr>
							  <tr>								
								<td>2235-60-796-SP-005-V-31-02</td>
								<td>150,000,000</td>								
							  </tr>
							  <tr>								
								<td>2235-60-200-SP-008-V-31-02</td>
								<td rowspan="3">Health Insurance Scheme for Unorganised Workers[WBUSWHSS]</td>
								<td>500,000,000</td>								
							  </tr>
							  <tr>								
								<td>2235-60-789-SP-010-V-31-02</td>
								<td>150,000,000</td>								
							  </tr>
							  <tr>								
								<td>2235-60-796-SP-010-V-31-02</td>
								<td>50,000,000</td>								
							  </tr>							  
							  <tr>
								<td colspan="2" style="text-align:center;"><strong>TOTAL</strong></td>
								<td><strong>2,964,900,000</strong></td>								
							  </tr>
							</tbody>
						</table>
					</div>
				</div>
			</div>';
	$output .= '<p>'.l('<i class="fa fa-file" aria-hidden="true"></i>&nbsp;Click here to Print Budget 2017-18', 'https://wblc.gov.in/sites/default/files/budget-details-2017-18.pdf', array('html' => TRUE,'attributes'=>array('target' => '_blank'))).'</p>';
	$output	.= '<div class="row">
				  <div class="col-sm-12">										
					  <div id="content-2" class="scrollContent content mCustomScrollbar light" data-mcs-theme="minimal-dark">
						  <table width="100%" border="1" cellspacing="0" cellpadding="0" class="view-act-rules-table fixed_headers">
						  	<thead>
								<tr>
									<td colspan="5"><strong>Annual Plan, Physical Achievement 2014-15, Target &amp; Proposal for 2015-16 under demand no. 35</strong></td>
								<tr>
							  <tr>
								<th width="10%">Sl. No.</th>
								<th width="20%">Head of Account Code</th>
								<th width="35%">Scheme</th>
								<th width="20%">Budget Estimate 2015-16</td>
								<th width="15%">Remarks</th>
							  </tr>
							  <tr>
								<td>5</td>
								<td>2230-01-001-SP-001-V-13-04</td>
								<td>Strengthening of enforcement machinery or the Labour Directorate</td>
								<td>12500000</td>
								<td>&nbsp;</td>
							  </tr>
							  <tr>
								<td rowspan="2">6</td>
								<td>2230-01-001-SP-002-V-13-03</td>
								<td rowspan="2">Strengthening of Training Institute-cum-Central Library</td>
								<td>300000</td>
								<td>&nbsp;</td>
							  </tr>
							  <tr>
								<td>2230-01-001-SP-002-V-13-04</td>
								<td>5700000</td>
								<td>&nbsp;</td>
							  </tr>
							  <tr>
								<td>7</td>
								<td>2230-01-004-SP-001-V-13-04</td>
								<td>Improvement of Labour Statistics</td>
								<td>1200000</td>
								<td>&nbsp;</td>
							  </tr>
							  <tr>
								<td rowspan="2">10</td>
								<td>2230-01-101-SP-003-V-33-05</td>
								<td rowspan="3">Bidi Workers Welfare Scheme</td>
								<td>47500000</td>
								<td>&nbsp;</td>
							  </tr>
							  <tr>
								<td>2230-01-101-SP-003-V-50</td>
								<td>2500000</td>
								<td>&nbsp;</td>
							  </tr>
							  <tr>
								<td>11</td>
								<td>2230-01-10%9-SP-001-V-13-04</td>
								<td>10000000</td>
								<td>&nbsp;</td>
							  </tr>
							  <tr>
								<td>12</td>
								<td>2230-01-101-SP-004-V-13-04</td>
								<td>Improvement of Working Conditions of Child and Woman Labour</td>
								<td>2000000</td>
								<td>&nbsp;</td>
							  </tr>
							  <tr>
								<td rowspan="2">13</td>
								<td>2230-01-101-SP-007-V-13-03</td>
								<td width="739" rowspan="2">Statewide Survey to Identity Child Labour in different employment</td>
								<td>300000</td>
								<td>&nbsp;</td>
							  </tr>
							  <tr>
								<td width="181" height="14">2230-01-101-SP-007-V-13-04</td>
								<td>700000</td>
								<td>&nbsp;</td>
							  </tr>
							  <tr>
								<td>15</td>
								<td>2230-01-101-SP-010-V-50</td>
								<td>Welfare of Agricultural Labours, Construction Labours and Unorganised Labours</td>
								<td>0</td>
								<td>&nbsp;</td>
							  </tr>
							  <tr>
								<td>21</td>
								<td>2230-02-001-SP-003-V-77</td>
								<td>National e-Governance Action Plan[LB]</td>
								<td>20000000</td>
								<td>&nbsp;</td>
							  </tr>
							  <tr>
								<td>27</td>
								<td>2235-60-200-SP-005-V-31-02</td>
								<td rowspan="3">Provident Fund Scheme for Unorganised Workers in Urban and Rural Areas[SASPFUW]</td>
								<td>1250000000</td>
								<td>&nbsp;</td>
							  </tr>
							  <tr>
								<td>28</td>
								<td>2235-60-10%9-SP-001-V-31-02</td>
								<td>480000000</td>
								<td>&nbsp;</td>
							  </tr>
							  <tr>
								<td>29</td>
								<td>2235-60-796-SP-001-V-31-02</td>
								<td>100000000</td>
								<td>&nbsp;</td>
							  </tr>
							  <tr>
								<td>30</td>
								<td>2235-60-200-SP-008-V-31-02</td>
								<td rowspan="3">Health Insurance Scheme for Unorganised Workers[WBUSWHSS]</td>
								<td>232300000</td>
								<td>&nbsp;</td>
							  </tr>
							  <tr>
								<td>31</td>
								<td>2235-60-10%9-SP-010-V-31-02</td>
								<td>60000000</td>
								<td>&nbsp;</td>
							  </tr>
							  <tr>
								<td>32</td>
								<td>2235-60-796-SP-010-V-31-02</td>
								<td>10000000</td>
								<td>&nbsp;</td>
							  </tr>
							  <tr>
								<td>3</td>
								<td>4250-00-201-SP-006-V-53-00</td>
								<td>Construction &amp; Renovation of Buildings under Labour Department</td>
								<td>120000000</td>
								<td>&nbsp;</td>
							  </tr>
							  <tr>
								<td colspan="3" style="text-align:center;"><strong>TOTAL</strong></td>
								<td><strong>23550,00,000</strong></td>
								<td>&nbsp;</td>
							  </tr>
							</tbody>
						</table>
					</div>
				</div>
			</div>';

	$output .= '<p>'.l('<i class="fa fa-file" aria-hidden="true"></i>&nbsp;Click here to Print Budget 2015-16', 'https://wblc.gov.in/sites/default/files/Budget-2015-2016.pdf', array('html' => TRUE,'attributes'=>array('target' => '_blank'))).'</p>';
	return $output;
}