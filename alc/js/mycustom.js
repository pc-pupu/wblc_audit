
jQuery( window ).load(function() {  
	jQuery('#myModal'+id).modal('hide'); 
	jQuery('#myModalFamily'+id).modal('hide'); 
	jQuery('#myModalSchemes'+id).modal('hide'); 
	
	jQuery('#modal_map'+id).modal('hide');
	jQuery("#back_to").click(function(e){		
		var field_name = jQuery("#editable_fields").val(); 
		if(confirm('Want to continue?')){    
			jQuery("#check_data_text_id").val(field_name);
		}
	});
	
	jQuery(".viewfile_popup").click(function(e){	 
		var id = jQuery(this).attr('id');
        e.preventDefault(); // this will prevent the browser to redirect to the href
		jQuery( "#view_"+id ).dialog();
	});
	
	
	jQuery("#edit-remark-type").change(function(e){	 
		var option_value = jQuery("#edit-remark-type").val();
		
		if(option_value=='B'){
			jQuery('#r_text').val('Application is sent back for rectification. Kindly modify disapproved fields and re-submit the application.');	
		}else{
			jQuery('#r_text').val('');
		}
	});
	
	jQuery(".bocwa_verified_alc").on('ifChanged', function(e){						
			
		var bocwa_alc_fieldIdt = jQuery("#bocwa_field_name_alc").val();				
		
		if (jQuery(this).is(':checked')) {				
			var alc_fieldId_bocwa = jQuery(this).attr('id');
			
			if(bocwa_alc_fieldIdt == ''){
				
				var bocwa_alc_fieldIdt 	= alc_fieldId_bocwa;
				
			}else {
				
				var bocwa_alc_fieldIdt = bocwa_alc_fieldIdt+','+alc_fieldId_bocwa;	
			}
			
			jQuery("#bocwa_field_name_alc").val(bocwa_alc_fieldIdt);
		}
						
		var bocwa_field_name_alc = jQuery("#bocwa_field_name_alc").val();
			
		if (!jQuery(this).is(':checked')) {	
			
				var fieldIdt 			= '';			
				var fieldId 			= jQuery(this).attr('id');
								
				jQuery("#bocwa_field_name_alc").val('');
				
				var alc_bocwa_field_name_arr 		= bocwa_field_name_alc.split(',');
				
				for(var i=0;i<alc_bocwa_field_name_arr.length;i++){	
								
        			if(alc_bocwa_field_name_arr[i] != fieldId){
						
						var fieldIdt 	= fieldIdt+','+alc_bocwa_field_name_arr[i];
						fieldIdt 		= fieldIdt.replace(',,', '');
						
						jQuery("#bocwa_field_name_alc").val(fieldIdt);
						var t 			= jQuery("#bocwa_field_name_alc").val();
						
						if(t==','){
							jQuery("#bocwa_field_name_alc").val(null);
						}
					}
   				}
    			
			}				
	});	
	
	jQuery('.yn_license_verified').on('ifChanged', function(e){
		
		var clraL_fieldIdt = jQuery("#license_fieldname").val();
		
		if (jQuery(this).is(':checked')) {				
			var fieldId = jQuery(this).attr('id');
			
			if(clraL_fieldIdt == ''){
			var clraL_fieldIdt 	= fieldId;
			}else {
			var clraL_fieldIdt = clraL_fieldIdt+','+fieldId;	
			}
			jQuery("#license_fieldname").val(clraL_fieldIdt);
		}		
		
		var clraL_field_name = jQuery("#license_fieldname").val();
		
		if (!jQuery(this).is(':checked')) {	
			
			var clraL_fieldIdt 	= '';			
			var fieldId 		= jQuery(this).attr('id');
								
			jQuery("#license_fieldname").val('');
			var field_name_arr 	= clraL_field_name.split(',');
				
			for(var i=0;i<field_name_arr.length;i++){	
								
				if(field_name_arr[i] != fieldId){
					
					var clraL_fieldIdt 	= clraL_fieldIdt+','+field_name_arr[i];
					clraL_fieldIdt 		= clraL_fieldIdt.replace(',,', ',');
					
					jQuery("#license_fieldname").val(clraL_fieldIdt);
					var t 				= jQuery("#license_fieldname").val();
					
					if(t==','){
						jQuery("#license_fieldname").val(null);
					}
				}
				
			}
		}	
		
		
		
		
	});
	
	jQuery(".renewal_alc_verified").on('ifChanged', function(e){
	
		
		var fieldIdt		 =	jQuery("input[name=remark_alc_text_renewal]").val();
		if (jQuery(this).is(':checked')) {	
					
			var fieldId 	= jQuery(this).attr('id');
				
			if (fieldIdt == ''){
				var fieldIdt 	= fieldId;
			}else{
				var fieldIdt 	= fieldIdt+','+fieldId;
			}
			
			
			jQuery("input[name=remark_alc_text_renewal]").val(fieldIdt);
		}
		
		var field_name = jQuery("input[name=remark_alc_text_renewal]").val();
		
		if (!jQuery(this).is(':checked')) {	
		
				var fieldIdt = '';			
				var fieldId = jQuery(this).attr('id');
								
				
					
				var field_name_arr = field_name.split(',');
				
				for(var i=0;i<field_name_arr.length;i++){
										
        			if(field_name_arr[i] != fieldId){
						
						var fieldIdt = fieldIdt+','+field_name_arr[i];
						fieldIdt = fieldIdt.replace(',,',',');
						
						
						jQuery("input[name=remark_alc_text_renewal]").val(fieldIdt);
					
						var t		 =	jQuery("input[name=remark_alc_text_renewal]").val();
						
						if(t==','){
						
						jQuery("input[name=remark_alc_text_renewal]").val(null);
						}
					}
   				}
    			
			}	
		
		
	});
	
	jQuery(".ismw_EMP_verified_alc").on('ifChanged', function(e){
	
		var ismwemp_alc_fieldIdt = jQuery("#ismwlicenseEmp_fieldname_alc").val();				
		if (jQuery(this).is(':checked')) {				
			var alc_fieldId_ismwemp = jQuery(this).attr('id');
			if(ismwemp_alc_fieldIdt == ''){
				var ismwemp_alc_fieldIdt 	= alc_fieldId_ismwemp;
			}else {
				var ismwemp_alc_fieldIdt = ismwemp_alc_fieldIdt+','+alc_fieldId_ismwemp;	
			}
			jQuery("#ismwlicenseEmp_fieldname_alc").val(ismwemp_alc_fieldIdt);
		}
						
		var ismwlicenseEmp_fieldname_alc = jQuery("#ismwlicenseEmp_fieldname_alc").val();
		if (!jQuery(this).is(':checked')) {	
				var fieldIdt 	= '';			
				var fieldId 	= jQuery(this).attr('id');
				jQuery("#ismwlicenseEmp_fieldname_alc").val('');				
				var alc_ismwemp_field_name_arr = ismwlicenseEmp_fieldname_alc.split(',');				
				for(var i=0;i<alc_ismwemp_field_name_arr.length;i++){								
        			if(alc_ismwemp_field_name_arr[i] != fieldId){						
						var fieldIdt 	= fieldIdt+','+alc_ismwemp_field_name_arr[i];
						fieldIdt 		= fieldIdt.replace(',,', '');						
						jQuery("#ismwlicenseEmp_fieldname_alc").val(fieldIdt);
						var t 			= jQuery("#ismwlicenseEmp_fieldname_alc").val();						
						if(t==','){
							jQuery("#ismwlicenseEmp_fieldname_alc").val(null);
						}
					}
   				}
			}				
	});
	
});

function getRemarkTypeAction(){
 
	var act_id 	  = jQuery("input[type='hidden'][name='act_id']").val(); 
	var option_text  = jQuery("#remark_type_id option:selected").text();
	var option_value = jQuery("#remark_type_id").val();
	
	if(act_id == 1){
		
		var field_string = 		'e_name,loc_e_name,e_postal_address,full_name_principal_emp,address_principal_emp,full_name_manager,address_manager,e_nature_of_work,max_num_wrkmen,e_num_of_workmen_per_or_reg,e_num_of_workmen_temp_or_reg,workmen_if_same_similar_kind_of_work,con_lab_job_desc,con_lab_wage_rate_other_benefits,con_lab_cat_desig_nom,e_settlement_award_judgement_min_wage,e_any_day_max_num_of_workmen,backlog_clra_registration_certificate_file,trade_license_file,article_of_assoc_file,memorandum_of_cert_file,partnership_deed_file,factory_license_file,est_type,gender_pe';
		
	}else if(act_id == 2){
		
		var field_string = 		'e_name,loc_e_name,e_postal_address,e_full_name,per_address_est,full_name_manager,address_manager,e_nature_of_work,max_num_of_workmen,est_date_comm,est_date_completion,emp_name,emp_address,trade_license_file,article_of_assoc_file,memorandum_of_cert_file,partnership_deed_file,challan_file,work_order_file,form_one_asses_ses_file,supp_asses_ses_file,other_doc_file,bocwa_address_proof_file,est_type,emp_gender';
		
	}else if(act_id == 4){
		
		var field_string = 'e_name,loc_e_name,e_postal_address,emp_name,emp_guardian_name,emp_address,modify_manager,modify_contractors,modify_director_partner,e_nature_of_work,clra_registration_number,clra_registration_date,max_num_migrant_wrkmen,trade_license_file,article_of_assoc_file,factory_license_file,certificate_other_states,other_related_documents,signed_pdf_file,backlog_certificate,est_type,gender';
		
	}else if(act_id == 6){
		
		var field_string = 'worksite,const_area';
		
	}else if(act_id == 42){
		
		var field_string = 'ownership_type,dob,est_reg_no,nameAgentmanager,addressAgentmanager,worksite_address,address_recruited,convictedReason,revoking_date,fiveYears_info,licenceno_clra,form_six_file,workOrder_file,trade_license_file,addressProof_file,other_related_documents,workmen_details,dp_details,incharge_details';
		
	}else if(act_id == 43){
		
		var field_string = 'ownership_type,licenceno_clra,dob,est_name,est_address,pe_nature_of_work,pe_form_i_fid,pe_registration_number,pe_reg_date,pe_reg_cer_upload_fid,pe_name,pe_address,con_nature_work,work_order_date,nameAgentmanager,addressAgentmanager,contractor_migrant_wrkmen,dp_details,incharge_details,convictedReason,revoking_date,fiveYears_info,form_six_file,workOrder_file,trade_license_file,addressProof_file,other_related_documents,worksite_address,address_recruited,workmen_details,max_num_migrant_wrkmen';
		
	}else if(act_id == 12){
		
		var field_string = 'category_of_contractor,contractor_dist,is_cooparative,dob_contractor,name_of_agent,address_of_manager,category_designation,unskilled_rate_wages,semiskilled_rate_wages,skilled_rate_wages,highlyskilled_rate_wages,hours_work,overtime,overtime_wages,annual_leave_no,casual_leave_no,sick_leave_no,maternity_leave_no,other_leave_no,holiday,contractor_convicted,contractor_revoking,contractor_previous_employer,worksite_add,frm_v,frm_v_file_id,work_order_file_id,residential_file_id,other_doc_id';
		
	}else if(act_id == 13){
		
		var field_string = 'unskilled,semiskilled,skilled,highly,extended_wo';
	}
	
	if(option_text == 'Back For Correction' && option_value == 'B' ){
		
		jQuery("#formVId .icheckbox_flat-blue").removeClass('checked');
		jQuery('#remarks_text_id').html('Application is sent back for rectification. Kindly modify disapproved fields and re-submit the application.');
		jQuery('#remarks_text_id').val('Application is sent back for rectification. Kindly modify disapproved fields and re-submit the application.');
		
	}else if(  (option_text == 'Verified & Approval For Fees Submission' && option_value == 'V') || (option_text == 'Verified & Approval For Fees Submission' && option_value == 'A') || (option_text == 'Verified & Approval without Fees Submission' && option_value == 'VA')){
		
		if(confirm("All information are checked and verified. Do you want to continue?")){
			jQuery(".icheckbox_flat-blue").addClass('checked');
			jQuery("#formVId .icheckbox_flat-blue").removeClass('checked');
			
			if(act_id == 1){
				
				jQuery("#clrareg_fieldname").val(''); 
				jQuery("#clrareg_fieldname").val(field_string);
				if(option_value == 'VA'){
					jQuery('#remarks_text_id').html('Application is verified and approved by Registering Authority and is exempted from the fees. Download FORM-I from the dashboard and upload it after signing the downloaded FORM-I');	 
					jQuery('#remarks_text_id').val('Application is verified and approved by Registering Authority  and is exempted from the fees. Download FORM-I from the dashboard and upload it after signing the downloaded FORM-I.');
				}else{
					jQuery('#remarks_text_id').html('Application is verified and approved by Registering Authority and directed to pay fees. After successful payment download FORM-I from the dashboard and upload it after signing the downloaded FORM-I');	 
					jQuery('#remarks_text_id').val('Application is verified and approved by Registering Authority and directed to pay fees. After successful payment download FORM-I from the dashboard and upload it after signing the downloaded FORM-I.');
				}
				
			}else if(act_id == 2){
				
				jQuery("#bocwa_field_name_alc").val(''); 
				jQuery("#bocwa_field_name_alc").val(field_string);
				jQuery('#remarks_text_id').html('Application is verified and approved by Registering Authority and directed to pay fees. After successful payment download FORM-I from the dashboard and upload it after signing the downloaded FORM-I');	 
				jQuery('#remarks_text_id').val('Application is verified and approved by Registering Authority and directed to pay fees. After successful payment download FORM-I from the dashboard and upload it after signing the downloaded FORM-I.');
				
			}else if(act_id == 4){
				
				jQuery("#ismw_field_name_alc").val(''); 
				jQuery("#ismw_field_name_alc").val(field_string);
				jQuery('#remarks_text_id').html('Application is verified and approved by Registering Authority and directed to pay fees. After successful payment download FORM-I from the dashboard and upload it after signing the downloaded FORM-I');	 
				jQuery('#remarks_text_id').val('Application is verified and approved by Registering Authority and directed to pay fees. After successful payment download FORM-I from the dashboard and upload it after signing the downloaded FORM-I.');
				
			}else if(act_id == 42){
				
				jQuery("#ismwlicenseEmp_fieldname_alc").val(''); 
				jQuery("#ismwlicenseEmp_fieldname_alc").val(field_string);
				jQuery('#remarks_text_id').html('Application is verified and approved by Registering Authority and directed to pay fees. After successful payment download FORM-V from the dashboard and upload it after signing the downloaded FORM-V');	 
				jQuery('#remarks_text_id').val('Application is verified and approved by Registering Authority and directed to pay fees. After successful payment download FORM-V from the dashboard and upload it after signing the downloaded FORM-V.');
				
			}else if(act_id == 43){
				jQuery("#ismwlicense_fieldname").val(''); 
				jQuery("#ismwlicense_fieldname").val(field_string);
				jQuery('#remarks_text_id').html('Application is verified and approved by Registering Authority and directed to pay fees. After successful payment download FORM-V from the dashboard and upload it after signing the downloaded FORM-V');	 
				jQuery('#remarks_text_id').val('Application is verified and approved by Registering Authority and directed to pay fees. After successful payment download FORM-V from the dashboard and upload it after signing the downloaded FORM-V.');
				
			}else if(act_id == 12){
				jQuery("#license_fieldname").val(''); 
				jQuery("#license_fieldname").val(field_string);
				jQuery('#remarks_text_id').html('Application is verified and approved by Registering Authority and directed to pay fees. After successful payment download FORM-IV from the dashboard and upload it after signing the downloaded FORM-IV');	 
				jQuery('#remarks_text_id').val('Application is verified and approved by Registering Authority and directed to pay fees. After successful payment download FORM-IV from the dashboard and upload it after signing the downloaded FORM-IV.');
				
			}else if(act_id == 13){
				jQuery("#renewal_fieldname").val('');
				jQuery("#extraAmt .icheckbox_flat-blue").removeClass('checked'); 
				if(confirm("Want to include 25% additional fees. Do you want to continue?")){
					jQuery("#renewal_fieldname").val(field_string+',backlog_renewal_extra_amount');
					jQuery("#extraAmt .icheckbox_flat-blue").addClass('checked');
				}else{
					jQuery("#renewal_fieldname").val(field_string);
				}
				jQuery('#remarks_text_id').html('Application is verified and approved by Registering Authority and directed to pay fees. After successful payment download FORM-VII from the dashboard and upload it after signing the downloaded FORM-VII');	 
				jQuery('#remarks_text_id').val('Application is verified and approved by Registering Authority and directed to pay fees. After successful payment download FORM-VII from the dashboard and upload it after signing the downloaded FORM-VII.');
			}
		}else{
			jQuery("#remark_type_id").val('');
			jQuery("#remarks_text_id").html('');
		}
		
	}else if(option_text == 'Approved' && option_value == 'F' && act_id == 6) { 
		if(confirm("All information are checked and verified. Do you want to continue?")){
			jQuery(".icheckbox_flat-blue").addClass('checked');
			jQuery("#lb_cess_field_name").val(''); 
			jQuery("#lb_cess_field_name").val(field_string);
			jQuery('#remarks_text_id').html('Application is verified and approved by Local Body');	 
			jQuery('#remarks_text_id').val('Application is verified and approved by Local Body.');
		}else{
			jQuery("#remark_type_id").val('');
			jQuery("#remarks_text_id").html('');
		}
	}else if(option_value == 'U' || ( option_value == 'P' && act_id == 12 ) || ( option_value == 'P' && act_id == 13 )){
		
		jQuery("#formVId .icheckbox_flat-blue").removeClass('checked');
		if(act_id == 1){
			jQuery("#clrareg_fieldname").val(field_string);
		}else if(act_id == 2){
			jQuery("#bocwa_field_name_alc").val(field_string);
		}else if(act_id == 4){
			jQuery("#ismw_field_name_alc").val(field_string);
		}else if(act_id == 42){
			jQuery("#ismwlicenseEmp_fieldname_alc").val(field_string);
		}else if(act_id == 43){
			jQuery("#ismwlicense_fieldname").val(field_string);
		}else if(act_id == 12){
			jQuery("#license_fieldname").val(field_string);
		}else if(act_id == 13){
			var field_string 	 = jQuery("input[type='hidden'][name='renewal_fieldname']").val(); 
			jQuery("#renewal_fieldname").val(field_string);
		}
			
	}else if(option_text == 'Issue Certificate' && option_value == 'I' ){
		
		if(act_id == 1 || act_id == 2 || act_id == 4){
			var message = 'FORM-I is duly signed By the Applicant. If yes, then continue?'
		}else if(act_id == 12){
			var message = 'FORM-IV is duly signed By the Applicant. If yes, then continue?'
		}else if(act_id == 13){
			var message = 'FORM-VII is duly signed By the Applicant? If yes, then continue.';
		}else{
			var message = 'FORM-V is duly signed By the Applicant. If yes, then continue?';
		}
		if(confirm(message)){
		 jQuery(".icheckbox_flat-blue").addClass('checked');
		  if(act_id == 1){	
		 	jQuery("#clrareg_fieldname").val(field_string+',form_1_clra_signed_pdf_file'); 
			jQuery("#submit_block").hide();
		 }else if(act_id == 2){	
		 	jQuery("#bocwa_field_name_alc").val(field_string+',form_1_bocwa_signed_pdf_file'); 
		 }else if(act_id == 4){	
		 	jQuery("#ismw_field_name_alc").val(field_string+',signed_pdf_file'); 
		 }else if(act_id == 42){	
		 	jQuery("#ismwlicenseEmp_fieldname_alc").val(field_string+',formV_signed_pdf_file'); 
		 }else if(act_id == 43){
			jQuery("#ismwlicense_fieldname").val(field_string+',formV_signed_pdf_file');  
		 }else if(act_id == 12){
			 jQuery("#license_fieldname").val(field_string+',form_iv_id');  
		 }else if(act_id == 13){
			 var field_string 	 = jQuery("input[type='hidden'][name='renewal_fieldname']").val(); 
			jQuery("#renewal_fieldname").val(field_string);
			 jQuery("#renewal_fieldname").val(field_string+',form_vii'); 
		 }
		 jQuery("#formVId .icheckbox_flat-blue").addClass('checked');
		 jQuery("#remarks_text_id").html('Congratulations! Certificate is issued by the Registering Authority. You can download it from the dashboard.');
		 jQuery('#remarks_text_id').val('Congratulations! Certificate is issued by the Registering Authority. You can download it from the dashboard.');	
	  }else{
		  jQuery("#formVId .icheckbox_flat-blue").removeClass('checked');
		  if(act_id == 1){	
		 	jQuery("#clrareg_fieldname").val(field_string); 
		  }else if(act_id==2){
			  jQuery("#bocwa_field_name_alc").val(field_string); 
		  }else if(act_id == 4){	
		 	jQuery("#ismw_field_name_alc").val(field_string); 
		  }else if(act_id == 42){	
		 	jQuery("#ismwlicenseEmp_fieldname_alc").val(field_string); 
		  }else if(act_id == 43){
			jQuery("#ismwlicense_fieldname").val(field_string);  
		  }else if(act_id == 12){
			 jQuery("#license_fieldname").val(field_string);  
		  }else if(act_id == 13){
			 var field_string 	 = jQuery("input[type='hidden'][name='renewal_fieldname']").val(); 
			 jQuery("#renewal_fieldname").val(field_string);
			 jQuery("#renewal_fieldname").val(field_string); 
		  }
		  jQuery("#remark_type_id").val('');
		  jQuery("#remarks_text_id").val('');
		  jQuery("#remarks_text_id").html('');  
		  jQuery("#generate_certificate_block").hide();
	  }	
	}else{
		jQuery("#remarks_text_id").html('');  
		jQuery("#remarks_text_id").val('');
	}
	
	/*jQuery.ajax({
		type	: "POST",
		url		: edit_base_url+'ismwlicenseview-employment/'+license_id,
		data	: { option_text: option_text, option_value:option_value,field_string:field_string  },*/
		
		/*beforeSend: function() { 
		  jQuery("#ismw_empLicSubmitId").prop('disabled', true); // disable button
		}*/
		//success:function(data){ alert(option_text);
		 /* if(option_text == 'Back For Correction' && option_value == 'B' ){
			jQuery('#remarks_text_id').html('Application is sent back for rectification. Kindly modify disapproved fields and re-submit the application.');
			jQuery('#remarks_text_id').val('Application is sent back for rectification. Kindly modify disapproved fields and re-submit the application.');
				
		  
		  }else if(option_text == 'Verified & Approval For Fees Submission' && option_value == 'V' ){			  
			  
			if(confirm("All information are checked and verified. Do you want to continue?")){
				jQuery(".icheckbox_flat-blue").addClass('checked');	
				jQuery("#ismwlicenseEmp_fieldname_alc").val(''); 
				jQuery("#ismwlicenseEmp_fieldname_alc").val(field_string); 
				jQuery('#remarks_text_id').html('Application is verified and approved by Registering Authority and directed to pay fees. After successful payment download FORM-V from the dashboard and upload it after signing the downloaded FORM-V');	 
				jQuery('#remarks_text_id').val('Application is verified and approved by Registering Authority and directed to pay fees. After successful payment download FORM-V from the dashboard and upload it after signing the downloaded FORM-V.');
			}else{
				jQuery("#remark_type_id").val('');
				jQuery("#remarks_text_id").html('');
			}
			
		  
		  }else if(option_text == 'Back For Correction(Signed PDF)' && option_value == 'U' ){
			  
			     jQuery("#formVId .icheckbox_flat-blue").removeClass('checked');
				 jQuery("#ismwlicenseEmp_fieldname_alc").val(field_string); 
				
		  }else if(option_text == 'Issue Certificate' && option_value == 'I' ){
			  if(confirm("FORM-V is duly signed By the Applicant. If yes, then continue?")){
				 jQuery(".icheckbox_flat-blue").addClass('checked');	
				 jQuery("#ismwlicenseEmp_fieldname_alc").val(field_string+',formV_signed_pdf_file');  
				 jQuery("#formVId .icheckbox_flat-blue").addClass('checked');
				 jQuery("#remarks_text_id").html('Congratulations! Certificate is issued by the Registering Authority. You can download it from the dashboard.');
				 jQuery('#remarks_text_id').val('Congratulations! Certificate is issued by the Registering Authority. You can download it from the dashboard.');	
			  }else{
				  jQuery("#formVId .icheckbox_flat-blue").removeClass('checked');
				  jQuery("#ismwlicenseEmp_fieldname_alc").val(field_string); 
				  jQuery("#remark_type_id").val('');
				  jQuery("#remarks_text_id").val('');
				  jQuery("#remarks_text_id").html('');  
				  jQuery("#generate_certificate_block").hide();
			  }
			  
		  }else{
			jQuery("#remarks_text_id").html('');  
			jQuery("#remarks_text_id").val('');
		  }*/
		  //jQuery("#ismw_empLicSubmitId").prop('disabled', false); // enable button
		  //jQuery("#remarks_text_id").html('');  
		 // jQuery("#remarks_text_id").val('');
		//}
  //});
}

function getRemarkAction(){
 
	var act_id 	  = jQuery("input[type='hidden'][name='act_id']").val(); 
	var option_text  = jQuery("#action_type_id option:selected").text(); 
	var option_value = jQuery("#action_type_id").val();
	
	if(act_id == 12){
		var field_string = 'category_of_contractor,contractor_dist,dob_contractor,name_of_agent,address_of_manager,unskilled_rate_wages,semiskilled_rate_wages,skilled_rate_wages,highlyskilled_rate_wages,hours_work,overtime,overtime_wages,annual_leave_no,casual_leave_no,sick_leave_no,maternity_leave_no,other_leave_no,frm_v_file_id,work_order_file_id,residential_file_id,category_designation,contractor_previous_employer,contractor_revoking,contractor_convicted,other_doc_id,is_cooparative,worksite_add';
	}
	
	if(option_text == 'Back For Correction' && option_value == 'B' ){
		jQuery('#remarks_text_id').html('Application is sent back for rectification. Kindly modify disapproved fields and re-submit the application.');
		jQuery('#remarks_text_id').val('Application is sent back for rectification. Kindly modify disapproved fields and re-submit the application.');
	}else if(option_text == 'Verified & Approval For Fees Submission' && option_value == 'V' ){
		if(confirm("All information are checked and verified. Do you want to continue?")){
			jQuery(".icheckbox_flat-blue").addClass('checked');	
			jQuery("#ismwlicenseEmp_fieldname_alc").val(''); 
			jQuery("#ismwlicenseEmp_fieldname_alc").val(field_string); 
			jQuery('#remarks_text_id').html('Application is verified and approved by Registering Authority and directed to pay fees. After successful payment download FORM-V from the dashboard and upload it after signing the downloaded FORM-V');	 
			jQuery('#remarks_text_id').val('Application is verified and approved by Registering Authority and directed to pay fees. After successful payment download FORM-V from the dashboard and upload it after signing the downloaded FORM-V.');
		}else{
			jQuery("#remark_type_id").val('');
			jQuery("#remarks_text_id").html('');
		}
	}else if(option_text == 'Back For Correction(Signed PDF)' && option_value == 'U' ){
		jQuery("#formVId .icheckbox_flat-blue").removeClass('checked');
		jQuery("#ismwlicenseEmp_fieldname_alc").val(field_string); 	
	}else if(option_text == 'Issue Certificate' && option_value == 'I' ){
		if(confirm("FORM-V is duly signed By the Applicant. If yes, then continue?")){
		 jQuery(".icheckbox_flat-blue").addClass('checked');	
		 jQuery("#ismwlicenseEmp_fieldname_alc").val(field_string+',formV_signed_pdf_file');  
		 jQuery("#formVId .icheckbox_flat-blue").addClass('checked');
		 jQuery("#remarks_text_id").html('Congratulations! Certificate is issued by the Registering Authority. You can download it from the dashboard.');
		 jQuery('#remarks_text_id').val('Congratulations! Certificate is issued by the Registering Authority. You can download it from the dashboard.');	
	  }else{
		  jQuery("#formVId .icheckbox_flat-blue").removeClass('checked');
		  jQuery("#ismwlicenseEmp_fieldname_alc").val(field_string); 
		  jQuery("#remark_type_id").val('');
		  jQuery("#remarks_text_id").val('');
		  jQuery("#remarks_text_id").html('');  
		  jQuery("#generate_certificate_block").hide();
	  }	
	}else{
		jQuery("#remarks_text_id").html('');  
		jQuery("#remarks_text_id").val('');
	}
}
jQuery(document).ready(function(){
	
	jQuery(".yn_amend_verified").on('ifChanged', function(e){
		
		var fieldIdt		 =	jQuery("input[name=editable_fields_ammend]").val();
	
		if (jQuery(this).is(':checked')) {	
					
			var fieldId 	= jQuery(this).attr('id');
				
			if (fieldIdt == ''){
				var fieldIdt 	= fieldId;
			}else{
				var fieldIdt 	= fieldIdt+fieldId+',';
			}
			
			
			jQuery("input[name=editable_fields_ammend]").val(fieldIdt);
		}
		
		var field_name = jQuery("input[name=editable_fields_ammend]").val();
	
		
		if (!jQuery(this).is(':checked')) {	
		
				var fieldIdt = '';			
				var fieldId = jQuery(this).attr('id');
								
				jQuery("input[name=editable_fields_ammend]").val();
					
				var field_name_arr = field_name.split(',');
				
				for(var i=0;i<field_name_arr.length;i++){
										
        			if(field_name_arr[i] != fieldId){
						
						var fieldIdt = fieldIdt+','+field_name_arr[i];
						fieldIdt = fieldIdt.replace(',,',',');
						
						

						jQuery("input[name=editable_fields_ammend]").val(fieldIdt);
					
						var t		 =	jQuery("input[name=editable_fields_ammend]").val();
						
						if(t==','){
						
						jQuery("input[name=editable_fields_ammend]").val(null);
						}
					}
   				}
    			
			}	
		
		
	})
	
	jQuery(".alc_renewal_verified").click(function(){ 
	
		
		var fieldIdt		 =	jQuery("input[name=remark_alc_text_renewal]").val();
		if (jQuery(this).is(':checked')) {	
					
			var fieldId 	= jQuery(this).attr('id');
				
			if (fieldIdt == ''){
				var fieldIdt 	= fieldId;
			}else{
				var fieldIdt 	= fieldIdt+','+fieldId;
			}
			
			
			jQuery("input[name=remark_alc_text_renewal]").val(fieldIdt);
		}
		
		var field_name = jQuery("input[name=remark_alc_text_renewal]").val();
		
		if (!jQuery(this).is(':checked')) {	
		
				var fieldIdt = '';			
				var fieldId = jQuery(this).attr('id');
								
				
					
				var field_name_arr = field_name.split(',');
				
				for(var i=0;i<field_name_arr.length;i++){
										
        			if(field_name_arr[i] != fieldId){
						
						var fieldIdt = fieldIdt+','+field_name_arr[i];
						fieldIdt = fieldIdt.replace(',,',',');
						
						
						jQuery("input[name=remark_alc_text_renewal]").val(fieldIdt);
					
						var t		 =	jQuery("input[name=remark_alc_text_renewal]").val();
						
						if(t==','){
						
						jQuery("input[name=remark_alc_text_renewal]").val(null);
						}
					}
   				}
    			
			}	
		
		
	});
	
	jQuery(".bocwa_verified_alc").click(function(){						
			
			var bocwa_alc_fieldIdt = jQuery("#bocwa_field_name_alc").val();				
			
			if (jQuery(this).is(':checked')) {				
				var alc_fieldId_bocwa = jQuery(this).attr('id');
				
				if(bocwa_alc_fieldIdt == ''){
					
					var bocwa_alc_fieldIdt 	= alc_fieldId_bocwa;
					
				}else {
					
					var bocwa_alc_fieldIdt = bocwa_alc_fieldIdt+','+alc_fieldId_bocwa;	
				}
				
				jQuery("#bocwa_field_name_alc").val(bocwa_alc_fieldIdt);
			}
						
			var bocwa_field_name_alc = jQuery("#bocwa_field_name_alc").val();
			
			if (!jQuery(this).is(':checked')) {	
			
				var fieldIdt 			= '';			
				var fieldId 			= jQuery(this).attr('id');
								
				jQuery("#bocwa_field_name_alc").val('');
				
				var alc_bocwa_field_name_arr 		= bocwa_field_name_alc.split(',');
				
				for(var i=0;i<alc_bocwa_field_name_arr.length;i++){	
								
        			if(alc_bocwa_field_name_arr[i] != fieldId){
						
						var fieldIdt 	= fieldIdt+','+alc_bocwa_field_name_arr[i];
						fieldIdt 		= fieldIdt.replace(',,', '');
						
						jQuery("#bocwa_field_name_alc").val(fieldIdt);
						var t 			= jQuery("#bocwa_field_name_alc").val();
						
						if(t==','){
							jQuery("#bocwa_field_name_alc").val(null);
						}
					}
   				}
    			
			}				
		});	
		
	jQuery(".clra_amend_alc").on('ifChanged', function(e){					
			
			var alc_clra_amend_fieldIdt = jQuery("#clrareg_fieldname").val();				
			
			if (jQuery(this).is(':checked')) {				
				var alc_fieldId_clra_amend = jQuery(this).attr('id');
				
				if(alc_clra_amend_fieldIdt == ''){
					
					var alc_clra_amend_fieldIdt 	= alc_fieldId_clra_amend;
					
				}else {
					
					var alc_clra_amend_fieldIdt = alc_clra_amend_fieldIdt+','+alc_fieldId_clra_amend;	
				}
				
				jQuery("#clrareg_fieldname").val(alc_clra_amend_fieldIdt);
			}
						
			var amand_field_name_alc = jQuery("#clrareg_fieldname").val();
			
			if (!jQuery(this).is(':checked')) {	
			
				var fieldIdt 			= '';			
				var fieldId 			= jQuery(this).attr('id');
								
				jQuery("#clrareg_fieldname").val('');
				
				var alc_amend_field_name_arr 		= amand_field_name_alc.split(',');
				
				for(var i=0;i<alc_amend_field_name_arr.length;i++){	
								
        			if(alc_amend_field_name_arr[i] != fieldId){
						
						var fieldIdt 	= fieldIdt+','+alc_amend_field_name_arr[i];
						fieldIdt 		= fieldIdt.replace(',,', '');
						
						jQuery("#clrareg_fieldname").val(fieldIdt);
						var t 			= jQuery("#clrareg_fieldname").val();
						
						if(t==','){
							jQuery("#clrareg_fieldname").val(null);
						}
					}
   				}
    			
			}				
	});
	
	jQuery('.mtw_verified_alc').on('ifChanged', function (e){ 				
		var mtw_alc_fieldIdt = jQuery("#mtw_field_name_alc").val();				
		if (jQuery(this).is(':checked')) {				
			var alc_fieldId_mtw = jQuery(this).attr('id');
			if(mtw_alc_fieldIdt == ''){
				var mtw_alc_fieldIdt 	= alc_fieldId_mtw;
			}else {
			var mtw_alc_fieldIdt = mtw_alc_fieldIdt+','+alc_fieldId_mtw;	
			}
			jQuery("#mtw_field_name_alc").val(mtw_alc_fieldIdt);
		}
	
		var mtw_field_name_alc = jQuery("#mtw_field_name_alc").val();
		if (!jQuery(this).is(':checked')) {
			var fieldIdt 			= '';			
			var fieldId 			= jQuery(this).attr('id');
			jQuery("#mtw_field_name_alc").val('');
			var alc_mtw_field_name_arr 		= mtw_field_name_alc.split(',');
			for(var i=0;i<alc_mtw_field_name_arr.length;i++){	
				if(alc_mtw_field_name_arr[i] != fieldId){
					var fieldIdt 	= fieldIdt+','+alc_mtw_field_name_arr[i];
					fieldIdt 		= fieldIdt.replace(',,', '');
					jQuery("#mtw_field_name_alc").val(fieldIdt);
					var t 			= jQuery("#mtw_field_name_alc").val();
					if(t==','){
						jQuery("#mtw_field_name_alc").val(null);
					}
				}
			}
		}				
	});
	
	jQuery(".mtw_verified_alc_all").change(function(){	
		jQuery(".mtw_verified_alc").attr('checked', this.checked);	
		
		var mtw_alc_fieldIdt = jQuery("#mtw_field_name_alc").val();				
		if (jQuery(this).is(':checked')) {				
			var alc_fieldId_mtw = jQuery(this).attr('id');
			if(mtw_alc_fieldIdt == ''){
				var mtw_alc_fieldIdt 	= alc_fieldId_mtw;
			}else {
			var mtw_alc_fieldIdt = mtw_alc_fieldIdt+','+alc_fieldId_mtw;	
			}
			jQuery("#mtw_field_name_alc").val(mtw_alc_fieldIdt);
		}
	
		var mtw_field_name_alc = jQuery("#mtw_field_name_alc").val();
		if (!jQuery(this).is(':checked')) {
			var fieldIdt 			= '';			
			var fieldId 			= jQuery(this).attr('id');
			jQuery("#mtw_field_name_alc").val('');
			var alc_mtw_field_name_arr 		= mtw_field_name_alc.split(',');
			for(var i=0;i<alc_mtw_field_name_arr.length;i++){	
				if(alc_mtw_field_name_arr[i] != fieldId){
					var fieldIdt 	= fieldIdt+','+alc_mtw_field_name_arr[i];
					fieldIdt 		= fieldIdt.replace(',,', '');
					jQuery("#mtw_field_name_alc").val(fieldIdt);
					var t 			= jQuery("#mtw_field_name_alc").val();
					if(t==','){
						jQuery("#mtw_field_name_alc").val(null);
					}
				}
			}
		}	
		alert(jQuery("#mtw_field_name_alc").val());	
	});	
	
	jQuery(".ismw_verified_alc").click(function(){	
			
		var ismw_alc_fieldIdt = jQuery("#ismw_field_name_alc").val();	
			
		if (jQuery(this).is(':checked')) {
							
			var alc_fieldId_ismw = jQuery(this).attr('id');
			
			if(ismw_alc_fieldIdt == ''){
				
				var ismw_alc_fieldIdt 	= alc_fieldId_ismw;
				
			}else {
				
				var ismw_alc_fieldIdt = ismw_alc_fieldIdt+','+alc_fieldId_ismw;	
			}
			
			jQuery("#ismw_field_name_alc").val(ismw_alc_fieldIdt);
		}
				
		var ismw_field_name_alc = jQuery("#ismw_field_name_alc").val();
	
		if (!jQuery(this).is(':checked')) {	
	
			var fieldIdt 			= '';			
			var fieldId 			= jQuery(this).attr('id');
						
			jQuery("#ismw_field_name_alc").val('');
		
			var alc_ismw_field_name_arr = ismw_field_name_alc.split(',');
		
			for(var i=0;i<alc_ismw_field_name_arr.length;i++){	
						
				if(alc_ismw_field_name_arr[i] != fieldId){
					
					var fieldIdt 	= fieldIdt+','+alc_ismw_field_name_arr[i];
					fieldIdt 		= fieldIdt.replace(',,', '');
					
					jQuery("#ismw_field_name_alc").val(fieldIdt);
					var t 			= jQuery("#ismw_field_name_alc").val();
					
					if(t==','){
						jQuery("#ismw_field_name_alc").val(null);
					}
				}
			}
		
		}				
	});	
	
});

function alc_clra_action(){
	    var alc_clra_action =  document.getElementById('alc_clra_id').value;
		var clra_qr_code=jQuery("input[name=clra_qr_code]").val();
		if(alc_clra_action == 'I'){
			if(clra_qr_code==""){
				 document.getElementById('genarate_pdf').style.display = 'block';
				 document.getElementById('action_call_time').style.display = 'none';
				 document.getElementById('action_div').style.display = 'none';
				 document.getElementById('submit_remark').style.display = 'none'; 
			}else{
			 		document.getElementById('action_div').style.display = 'block'; 
			 	 	document.getElementById('genarate_pdf').style.display = 'none';
			 		document.getElementById('action_call_time').style.display = 'none';
			  		document.getElementById('submit_remark').style.display = 'block';
			}
			 

		}
		if(alc_clra_action == 'C'){
			 document.getElementById('action_div').style.display = 'none'; 
			 document.getElementById('action_call_time').style.display = 'block';
			  document.getElementById('genarate_pdf').style.display = 'none';
			   document.getElementById('submit_remark').style.display = 'block';
		
		}
		if(alc_clra_action == 'R'){
			 document.getElementById('action_div').style.display = 'none'; 
			   document.getElementById('action_call_time').style.display = 'none';
			    document.getElementById('genarate_pdf').style.display = 'none';
				 document.getElementById('submit_remark').style.display = 'block';

		}
		if(alc_clra_action == 'VA'){
			 document.getElementById('action_div').style.display = 'none'; 
			   document.getElementById('action_call_time').style.display = 'none';
			    document.getElementById('genarate_pdf').style.display = 'none';
				 document.getElementById('submit_remark').style.display = 'block';

		}
		if(alc_clra_action == 'V'){
			 document.getElementById('action_div').style.display = 'none'; 
			   document.getElementById('action_call_time').style.display = 'none';
			    document.getElementById('genarate_pdf').style.display = 'none';
				 document.getElementById('submit_remark').style.display = 'block';

		}
		if(alc_clra_action == 'T'){
			 document.getElementById('action_div').style.display = 'none'; 
			  document.getElementById('action_call_time').style.display = 'none';
			   document.getElementById('genarate_pdf').style.display = 'none';
			    document.getElementById('submit_remark').style.display = 'block'; 

		}
	
}

function submit_form(){
	  var alc_clra_action =  document.getElementById('alc_clra_id').value;
	  var result=0;
	  if(alc_clra_action == 'C'){
		  var x = document.getElementById('clra_alc_call_time_id').value;
   		  if (x == null || x == "") {
       		 alert("Calling Date Time must be filled out");
			 jQuery('#clra_alc_call_time_id').prop('required',true);
			
        	return false;
		  }
		
			
    }
	
    
	else if(alc_clra_action == 'I'){
		//jQuery('select[name^="alc_clra"] option[value="I"]').attr("selected","selected"); 
		
		
   		 var Z=jQuery('input[type=file]').val();
		 var Y = document.getElementById('clra_alc_remarks_id').value; 
	 if (Z == null || Z == "") {
       		 alert("Must be upload certificates");
			 jQuery('#edit-clra-signed-certificate-upload').prop('required',true);
			
        	return false;
		  }	
	  if (Y == null || Y == "") {
       		 alert("Remark must be filled out");
			 jQuery('#clra_alc_remarks_id').prop('required',true);
			
        	return false;
		  }	  
			
    }else{
		 var Y = document.getElementById('clra_alc_remarks_id').value;
	  if (Y == null || Y == "") {
       		 alert("Remark must be filled out");
			 jQuery('#clra_alc_remarks_id').prop('required',true);
			
        	return false;
		  }	
	}
	
	
	return true;
}


jQuery(document).ready(function(){ 
	jQuery("#delBtn").click(function(){	 
		var edit_base_url = jQuery("input[type='hidden'][name='base_url']").val(); 
		var remarkPKID 		= jQuery(this).attr('data-id'); 
		var loggedUserID 	= jQuery(this).attr('data-uid'); 
		var actID 			= jQuery(this).attr('data-act');
		jQuery("#loading_"+remarkPKID).show(100);	
		var data = 'remarkPKID='+remarkPKID+'&loggedUserID='+loggedUserID+'&actID='+actID;
		if(confirm('Are you confirm to delete your previous action ?')){
			jQuery.ajax({
						type	:'POST',
						url		: edit_base_url+'deleteRemarks',   /* miscellaneous.module **/
						data	: data,				
						success	: function(message){//alert(message);
								if(message!='' && jQuery.trim(message) == 'Deactivated'){								
									jQuery("#loading_"+remarkPKID).hide();
									location.reload();				
								}
						}
					 });
		}
	});

});
function get_modal_value(id)
{
	jQuery('#myModal'+id).modal('show');
	
}
function get_modal_schemes(id)
{
	jQuery('#myModalFamily'+id).modal('show');
	
}
function modal_schemes(id)
{
	jQuery('#myModalSchemes'+id).modal('show');
	
}
function get_modal_map(id)
{
	//alert(id);
	jQuery('#modal_map'+id).modal('show');
	
}


function disableSubmit_app(){
  
	jQuery('#submit_hrd').css('display', 'none');

}
function disableSubmitApp(){
  
	jQuery('#submit_block').css('display', 'none');

}
