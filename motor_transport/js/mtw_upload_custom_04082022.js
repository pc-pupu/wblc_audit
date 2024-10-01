jQuery(window).load(function () {
	//// Date Picker ///
	jQuery("#expiry_date").attr('disabled',true);
	jQuery("#registration_date").datepicker({
		//dateFormat: "dd-mm-y",
		yearRange: "-50:+20",
		changeYear: true,
		changeMonth: true,
		maxDate : 0, 
		onSelect: function (selected) {
			//jQuery("#cont_end_dt_id").val('');	
			var dtMax = new Date(selected);
			dtMax.setDate(dtMax.getDate()); 
			var dd = dtMax.getDate();
			var mm = dtMax.getMonth() + 1;
			var y = dtMax.getFullYear();
			var dtFormatted = mm + "/"+ dd + "/"+ y;
			jQuery("#expiry_date").datepicker("option", "minDate", dtFormatted);
			if(jQuery("#registration_date").datepicker("getDate") != ''){
				jQuery("#expiry_date").attr('disabled',false);
			}
			
		}
	});
	//jQuery("#expiry_date").datepicker();
			
	jQuery("#expiry_date").datepicker({
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
								jQuery("#registration_date").datepicker("option", "maxDate", dtFormatted);
							}
							
						});
						
						
	
	
	/// Dtae Picker End /////
	jQuery("#mtw_trade_license_div").hide();
	jQuery("#mtw_article_of_assoc_div").hide();
	jQuery("#mtw_memorandum_of_cert_div").hide();
	jQuery("#mtw_partnership_deed_div").hide();
	jQuery("#mtw_challan_div").hide();
	jQuery("#mtw_work_order_div").hide();
	jQuery("#mtw_form_one_asses_ses_div").hide();
	jQuery("#mtw_supp_asses_ses_div").hide();
	jQuery("#mtw_other_doc_div").hide();
	jQuery("#mtw_address_proof_div").hide();
	
	jQuery(".viewfile_popup1").live("click", function(e){	 
		var id = jQuery(this).attr('id');
        e.preventDefault(); // this will prevent the browser to redirect to the href
		jQuery( "#view_"+id ).dialog();
		jQuery(".ui-dialog-titlebar-close").text("");
	});
	
	jQuery("#desgVal").change(function(){
		var dataSelect = jQuery("#desgVal").val();
		if(dataSelect == "Proprietor" || dataSelect == "Partner" || dataSelect == "MTU" ){
			jQuery('label[for = edit-mtw-directorsname]').text('Name of the Person in case of firm not registered under the Companies Act,1956');
		}else if(dataSelect == "General Manager"){
			jQuery('label[for = edit-mtw-directorsname]').text('Name of the Person in the case or a public sector undertaking');
		}else{
			jQuery('label[for = edit-mtw-directorsname]').text('Name of the Person in case of firm not registered under the Companies Act,1956');
		}
	});
	
	var changeTooltipPosition = function(event) {
		var tooltipX = event.pageX - 2;
		var tooltipY = event.pageY + 2;
		jQuery('div.tooltip').css({top: tooltipY, left: tooltipX});
	};

	var showTooltip = function(event) {
	  jQuery('div.tooltip').remove();
	  jQuery('<div class="tooltip">Address to which communications relating to the Motor Transport undertaking should be sent</div>')
            .appendTo('body');
	  changeTooltipPosition(event);
	  
	};

	var hideTooltip = function() {
	   jQuery('div.tooltip').remove();
	};

	jQuery("span#hint").bind({
	   mousemove : changeTooltipPosition,
	   mouseenter : showTooltip,
	   mouseleave: hideTooltip
	});
	
	var showTooltipNature = function(event) {
	  jQuery('div.tooltip').remove();
	  jQuery('<div class="tooltip">City service long distance, Passenger Service, Long distance freight service</div>')
            .appendTo('body');
	  changeTooltipPosition(event);
	  
	};	
	
	jQuery("span#hint_nature").bind({
	   mousemove : changeTooltipPosition,
	   mouseenter : showTooltipNature,
	   mouseleave: hideTooltip
	});
	
	// Add More Textbox Code
	jQuery("#addButton").submit(function(){
         return false;
    });
	
	
	var counter = 2;

	jQuery("#addButton").click(function () {
		alert('ok');
		if(counter>10){
				alert("Only 10 textboxes allow");
				return false;
		}
	
		var newTextBoxDiv =jQuery(document.createElement('div'))
			 .attr("id", 'TextBoxDiv' + counter);
	
		newTextBoxDiv.after().html('<label>Textbox #'+ counter + ' : </label>' +
			  '<input type="text" name="textbox' + counter +
			  '" id="textbox' + counter + '" value="" >');
	
		newTextBoxDiv.appendTo("#TextBoxesGroup");
	
	
		counter++;
	 });
/*
     jQuery("#removeButton").click(function () {
		if(counter==1){
			  alert("No more textbox to remove");
			  return false;
		   }
	
		counter--;
        jQuery("#TextBoxDiv" + counter).remove();

     });

     jQuery("#getButtonValue").click(function () {
		var msg = '';
		for(i=1; i<counter; i++){
		  msg += "\n Textbox #" + i + " : " + $('#textbox' + i).val();
		}
		alert(msg);
     });*/
	
	
	
	// End of Add More Textbox Code
	
	
	jQuery("#subBtnId").click(function(){
		if(confirm("Are you sure to pay for renewal?")){
			return true;
		}else{
			return false;
		}
	});
	
	
	
	
});

function mtw_upload_check_fun_1(){
	if(jQuery("#mtw_upload_check_in_1_id").is(':checked')) {
		jQuery("#mtw_trade_license_div").show();
	}else{
		jQuery("#mtw_trade_license_div").hide();
	}
}

function mtw_upload_check_fun_2(){
	if(jQuery("#mtw_upload_check_in_2_id").is(':checked')) {
		
		jQuery("#mtw_article_of_assoc_div").show();
	}else{
		jQuery("#mtw_article_of_assoc_div").hide();
	}
}

function mtw_upload_check_fun_3(){
	if(jQuery("#mtw_upload_check_in_3_id").is(':checked')) {
		jQuery("#mtw_memorandum_of_cert_div").show();
	}else{
		jQuery("#mtw_memorandum_of_cert_div").hide();
	}
}

function mtw_upload_check_fun_4(){
	if(jQuery("#mtw_upload_check_in_4_id").is(':checked')) {
		jQuery("#mtw_partnership_deed_div").show();
	}else{
		jQuery("#mtw_partnership_deed_div").hide();
	}
}

function mtw_upload_check_fun_5(){
	if(jQuery("#mtw_upload_check_in_5_id").is(':checked')) {
		jQuery("#mtw_challan_div").show();
	}else{
		jQuery("#mtw_challan_div").hide();
	}
}


function mtw_upload_check_fun_6(){
	if(jQuery("#mtw_upload_check_in_6_id").is(':checked')) {
		jQuery("#mtw_work_order_div").show();
	}else{
		jQuery("#mtw_work_order_div").hide();
	}
}

function mtw_upload_check_fun_7(){
	if(jQuery("#mtw_upload_check_in_7_id").is(':checked')) {
		jQuery("#mtw_form_one_asses_ses_div").show();
	}else{
		jQuery("#mtw_form_one_asses_ses_div").hide();
	}
}

function mtw_upload_check_fun_8(){
	if(jQuery("#mtw_upload_check_in_8_id").is(':checked')) {
		jQuery("#mtw_supp_asses_ses_div").show();
	}else{
		jQuery("#mtw_supp_asses_ses_div").hide();
	}
}


function mtw_upload_check_fun_9(){
	if(jQuery("#mtw_upload_check_in_9_id").is(':checked')) {
		jQuery("#mtw_other_doc_div").show();
	}else{
		jQuery("#mtw_other_doc_div").hide();
	}
}

function mtw_upload_check_fun_10(){
	if(jQuery("#mtw_upload_check_in_10_id").is(':checked')) {
		jQuery("#mtw_address_proof_div").show();
	}else{
		jQuery("#mtw_address_proof_div").hide();
	}
}
function checkFinalAppCheckBoxMTW(){ 
	if(jQuery("#finalAgreeId").is(':checked')){
		jQuery("#submitBtnMtw").removeAttr('disabled').css('background-color','#2da5da');
	}else if(jQuery("#finalRenewalId").is(':checked')){
		jQuery("#subBtnId").removeAttr('disabled').css('background-color','#2da5da');
	}else{
		jQuery("#submitBtnMtw").attr('disabled','disabled').css('background-color','#ff6666');
		jQuery("#subBtnId").attr('disabled','disabled').css('background-color','#ff6666');	
	}
}
function finalsubmitConfirm() {
	if(jQuery("#finalAgreeId").is(':checked')){
		if(confirm("After Submission you can not edit further. Please check all data.")){
			return true;
		}else{
			jQuery("#finalAgreeId").attr('checked', false).css('border','#FF0000');
			return false;
		}
	}else{
		alert("Please Check Declaration then Submit.");
		jQuery("#finalAgreeId").css('border','#FF0000');
		return false;
	}
}
jQuery(document).ready(function() {
	//var hiddenValue	=	jQuery("#array_name_id").val();
	//alert(hiddenValue);
	
});	

	
	