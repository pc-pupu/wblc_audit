jQuery(window).load(function () { 
	/// DatePicker ////
	var fdate= jQuery('#est_date_of_commencement_id').val(); 
	if(fdate!=''){
		jQuery("#est_date_of_termination_id").attr('disabled',false);
	}else{
		jQuery("#est_date_of_termination_id").attr('disabled',true);
	}
	
	
	jQuery("#est_date_of_commencement_id").datepicker({
	  dateFormat: "dd/mm/yy",
	  yearRange: "-50:+30",
	  changeMonth: true, 
	  changeYear: true,
	  onSelect: function(selected) {
	      jQuery("#est_date_of_termination_id").datepicker("option","minDate", selected)
	  }
	});
	//jQuery("#est_date_of_commencement_id").datepicker("setDate", "0");
	jQuery("#est_date_of_termination_id").datepicker({
	  dateFormat: "dd/mm/yy",
	 // minDate: "0",
	  yearRange: "-50:+30",
	  changeMonth: true, 
	  changeYear: true,
	  onSelect: function(selected) {
	      jQuery("#est_date_of_commencement_id").datepicker("option","maxDate", selected)
	  }
	});
	
	
	/*jQuery("#est_date_of_commencement_id").datepicker({
							//dateFormat: "dd-mm-y",
						    yearRange: "-50:+20",
							changeYear: true,
							changeMonth: true, 
							onSelect: function (selected) {
								//jQuery("#est_date_of_termination_id").val('');	
								var dtMax = new Date(selected);
								dtMax.setDate(dtMax.getDate()); 
								var dd = dtMax.getDate();
								var mm = dtMax.getMonth() + 1;
								var y = dtMax.getFullYear();
								var dtFormatted = mm + "/"+ dd + "/"+ y;
								jQuery("#est_date_of_termination_id").datepicker("option", "minDate", dtFormatted);
								if(jQuery("#est_date_of_commencement_id").datepicker("getDate") != ''){
									jQuery("#est_date_of_termination_id").attr('disabled',false);
								}
								
							}
	});
			
	jQuery("#est_date_of_termination_id").datepicker({
							//dateFormat: "dd-mm-y",
							yearRange: "-50:+20",
							changeYear: true,
							changeMonth: true, 
							onSelect: function (selected) {
								var dtMax = new Date(selected);
								dtMax.setDate(dtMax.getDate()); 
								var dd = dtMax.getDate();
								var mm = dtMax.getMonth() + 1;
								var y = dtMax.getFullYear();
								var dtFormatted = mm + "/"+ dd + "/"+ y;
								jQuery("#est_date_of_commencement_id").datepicker("option", "maxDate", dtFormatted);
							}
							
	});*/
		
			
	//////////////////
	jQuery("#bocwa_trade_license_div").hide();
	jQuery("#bocwa_article_of_assoc_div").hide();
	jQuery("#bocwa_memorandum_of_cert_div").hide();
	jQuery("#bocwa_partnership_deed_div").hide();
	jQuery("#bocwa_challan_div").hide();
	jQuery("#bocwa_work_order_div").hide();
	jQuery("#bocwa_form_one_asses_ses_div").hide();
	jQuery("#bocwa_supp_asses_ses_div").hide();
	jQuery("#bocwa_other_doc_div").hide();
	jQuery("#bocwa_address_proof_div").hide();
	
	if(jQuery("#bocwa_upload_check_in_1_id").is(':checked')) {
		jQuery("#bocwa_trade_license_div").show();
	}
	if(jQuery("#bocwa_upload_check_in_2_id").is(':checked')) {
		jQuery("#bocwa_article_of_assoc_div").show();
	}
	if(jQuery("#bocwa_upload_check_in_3_id").is(':checked')) {
		jQuery("#bocwa_memorandum_of_cert_div").show();
	}
	if(jQuery("#bocwa_upload_check_in_4_id").is(':checked')) {
		jQuery("#bocwa_partnership_deed_div").show();
	}
	if(jQuery("#bocwa_upload_check_in_5_id").is(':checked')) {
		jQuery("#bocwa_challan_div").show();
	}
	if(jQuery("#bocwa_upload_check_in_6_id").is(':checked')) {
		jQuery("#bocwa_work_order_div").show();
	}
	
});

function bocwa_upload_check_fun_1(){
	if(jQuery("#bocwa_upload_check_in_1_id").is(':checked')) {
		jQuery("#bocwa_trade_license_div").show();
	}else{
		jQuery("#bocwa_trade_license_div").hide();
	}
}

function bocwa_upload_check_fun_2(){
	if(jQuery("#bocwa_upload_check_in_2_id").is(':checked')) {
		
		jQuery("#bocwa_article_of_assoc_div").show();
	}else{
		jQuery("#bocwa_article_of_assoc_div").hide();
	}
}

function bocwa_upload_check_fun_3(){
	if(jQuery("#bocwa_upload_check_in_3_id").is(':checked')) {
		jQuery("#bocwa_memorandum_of_cert_div").show();
	}else{
		jQuery("#bocwa_memorandum_of_cert_div").hide();
	}
}

function bocwa_upload_check_fun_4(){
	if(jQuery("#bocwa_upload_check_in_4_id").is(':checked')) {
		jQuery("#bocwa_partnership_deed_div").show();
	}else{
		jQuery("#bocwa_partnership_deed_div").hide();
	}
}

function bocwa_upload_check_fun_5(){
	if(jQuery("#bocwa_upload_check_in_5_id").is(':checked')) {
		jQuery("#bocwa_challan_div").show();
	}else{
		jQuery("#bocwa_challan_div").hide();
	}
}


function bocwa_upload_check_fun_6(){
	if(jQuery("#bocwa_upload_check_in_6_id").is(':checked')) {
		jQuery("#bocwa_work_order_div").show();
	}else{
		jQuery("#bocwa_work_order_div").hide();
	}
}

function bocwa_upload_check_fun_7(){
	if(jQuery("#bocwa_upload_check_in_7_id").is(':checked')) {
		jQuery("#bocwa_form_one_asses_ses_div").show();
	}else{
		jQuery("#bocwa_form_one_asses_ses_div").hide();
	}
}

function bocwa_upload_check_fun_8(){
	if(jQuery("#bocwa_upload_check_in_8_id").is(':checked')) {
		jQuery("#bocwa_supp_asses_ses_div").show();
	}else{
		jQuery("#bocwa_supp_asses_ses_div").hide();
	}
}


function bocwa_upload_check_fun_9(){
	if(jQuery("#bocwa_upload_check_in_9_id").is(':checked')) {
		jQuery("#bocwa_other_doc_div").show();
	}else{
		jQuery("#bocwa_other_doc_div").hide();
	}
}

function bocwa_upload_check_fun_10(){
	if(jQuery("#bocwa_upload_check_in_10_id").is(':checked')) {
		jQuery("#bocwa_address_proof_div").show();
	}else{
		jQuery("#bocwa_address_proof_div").hide();
	}
}

function getSpanText(){ 
	var x = jQuery("#loc_e_subdivision_id option:selected").text();
	var y = jQuery("#loc_e_subdivision_id").val();
	if( x != '- Select -' ){
		jQuery('#rlo_location_id').html('<b>Note: Your Application will be forwarded to RLO '+jQuery("#loc_e_subdivision_id option:selected").text()+'</b>').show();
	}
	if( y == "" ){
		jQuery('#rlo_location_id').hide();
	}
}