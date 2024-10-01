<?php
function digitally_sign_process($form, &$form_state){
	/*$content = 'How to Digitally Sign your Documents using USB Token
	1.	Plug in the USB Token into your System which is provided by the Service Provider
	2.	Install the Driver of that USB Token , which is either in the device or download it online from the Service Providers Portal
	3.	Verify the Validity and Credentials of the Certificate by Viewing Contents under Internet Options of IE ( Internet Explorer)
	4.	The Certificate should be listed under the CERTIFICATES section . View the Validity , etc of the certificate once 
	5.	The above process from Step 1-4 is a One time job only
	6.	Now download the PDF File from the Portal , which may be any Form, Certificate, etc which you intend to Digitally Sign
	7.	Save the PDF File in a desired folder in any destination of your Hard Drive
	8.	You should have Adobe Reader 11 or Adobe Reader DC in your system which is freely downloadable from https://get.adobe.com/reader/
	9.	Install the software into your system
	10.	Now open the PDF Document using Adobe Reader DC or Adobe 11
	11.	Go to the Tools option and click on to the Digitally Sign Button
	12.	Mark the Area with your mouse where you want to Digitally Sign
	13.	Your Certificate details shall be shown immediately
	14.	Enter your Private Key password provided by the Service Provider
	15.	Check on to the “Lock Document after Signing” Option
	16.	You have successfully signed the PDF document
	17.	Now save this pdf and rename it by adding a _signed  and store in the same folder earlier created.<br>';*/
	
	$content = '<style type="text/css">
					table, td, th {font-size:16px;font-family:Times New Roman;height:40px;margin-top:20px;}
					td{padding-left:13px;}
					th {background-color: #008BD1;color: white;}
					td:hover {background-color:#d4e3e5;}
					tr:nth-child(even) {background: #DBE5F0;}
					tr:nth-child(odd) {background: #F1F1F1;}
			   </style>
			   <table width="100%" border="0" cellspacing="0" cellpadding="0">
					<tr>
						<td>						
						<strong>Step-1.</strong> Plug in the USB Token into your System which is provided by the Service Provider.<br><br>
						<strong>Step-2.</strong>Install the Driver of that USB Token , which is either in the device or download it online from the Service Providers Portal.<br><br>
						<strong>Step-3.</strong> Verify the Validity and Credentials of the Certificate by Viewing Contents under Internet Options of IE ( Internet Explorer).<br><br>
						<strong>Step-4.</strong> The Certificate should be listed under the CERTIFICATES section . View the Validity , etc of the certificate once.<br><br>
						<strong>Step-5.</strong> The above process from Step 1-4 is a One time job only.<br><br>
						<strong>Step-6.</strong> Now download the PDF File from the Portal , which may be any Form, Certificate, etc which you intend to Digitally Sign.<br><br>
						<strong>Step-7.</strong> Save the PDF File in a desired folder in any destination of your Hard Drive.<br><br>
						<strong>Step-8.</strong> You should have Adobe Reader 11 or Adobe Reader DC in your system which is freely downloadable from https://get.adobe.com/reader/.<br><br>
						<strong>Step-9.</strong> Install the software into your system.<br><br>
						<strong>Step-10.</strong> Now open the PDF Document using Adobe Reader DC or Adobe 11.<br><br>
						<strong>Step-11.</strong> Go to the Tools option and click on to the Digitally Sign Button.<br><br>
						<strong>Step-12.</strong> Mark the Area with your mouse where you want to Digitally Sign.<br><br>
						<strong>Step-13.</strong> Your Certificate details shall be shown immediately.<br><br>
						<strong>Step-14.</strong> Enter your Private Key password provided by the Service Provider.<br><br>
						<strong>Step-15.</strong> Check on to the “Lock Document after Signing” Option.<br><br>
						<strong>Step-16.</strong> You have successfully signed the PDF document.<br><br>
						<strong>Step-17.</strong> Now save this pdf and rename it by adding a _signed  and store in the same folder earlier created.<br><br>
						</td>
					</tr>					
			 </table>';	
			
	$form['digitally_sign']['content'] = array(							
							'#type'   => 'markup',							
							'#markup' => $content
						);
						
	$form['digitally_sign']['pdf_download_link'] = array(							
							'#type'   => 'markup',							
							'#markup' => l('Click here to download and print', '/sites/default/files/digital-signing-of-docs-through-usb-token.pdf', array('attributes' => array('title' => 'Click here to download and print', 'target' => '_blank'))),
						);	
	
	 
	 return $form;
}
?>