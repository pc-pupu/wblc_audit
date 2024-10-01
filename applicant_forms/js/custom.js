jQuery(window).load(function (){   
	jQuery("#submitBtn1").attr('disabled','disabled').addClass('disabled');
	jQuery("#submitBtn2").attr('disabled','disabled');
	
	jQuery(".view_fees_details").click(function(e){ 	
    	jQuery( "#fees_details").dialog();			
	});
	
	jQuery(".est_date #est_date_of_commencement_building_id").datepicker({ 
	   dateFormat: "dd-mm-yy",
	   //minDate: 0,
	   maxDate: "+1Y+6M",
	   changeMonth: true, 
	   changeYear: true,
	   
	});
	
	var fdate= jQuery('#est_date_of_commencement_id').val(); 
	if(fdate!=''){
		
		jQuery("#est_date_of_termination_id").attr('disabled',false);
	}else{
		jQuery("#est_date_of_termination_id").attr('disabled',true);
		alert('ok');
	}
	
	jQuery("#est_date_of_commencement_id").datepicker({
							//dateFormat: "dd-mm-y",
						    yearRange: "-50:+20",
							changeYear: true,
							changeMonth: true, 
							onSelect: function (selected) {
								jQuery("#est_date_of_work_of_each_labour_total_months_edit_id").val('');
								jQuery("#est_date_of_termination_id").val('');	
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
								jQuery("#est_date_of_work_of_each_labour_total_months_edit_id").val('');	
								var dtMax = new Date(selected);
								dtMax.setDate(dtMax.getDate()); 
								var dd = dtMax.getDate();
								var mm = dtMax.getMonth() + 1;
								var y = dtMax.getFullYear();
								var dtFormatted = mm + "/"+ dd + "/"+ y;
								jQuery("#est_date_of_commencement_id").datepicker("option", "maxDate", dtFormatted);
								var start = jQuery("#est_date_of_commencement_id").datepicker("getDate");
								var end = jQuery("#est_date_of_termination_id").datepicker("getDate");
								
								
								if(jQuery("#est_date_of_commencement_id").datepicker("getDate")!='' && jQuery("#est_date_of_termination_id").datepicker("getDate")!=''){
									var days = (end - start) / (1000 * 60 * 60 * 24);
									jQuery("#est_date_of_work_of_each_labour_total_months_edit_id").val(days);	
								}
							}
							
						});
	
});



function checkFinalAppCheckBox(){ 
	if(jQuery("#finalAgreeId").is(':checked')){
		jQuery("#submitBtn1").removeAttr('disabled').removeClass('disabled');
	}else{
		jQuery("#submitBtn1").attr('disabled','disabled').addClass('disabled');
		jQuery("#submitBtn1").attr('disabled','disabled');
		jQuery("#submitBtn2").attr('disabled','disabled');
	}
}

function datepicker_func(){ 
	
	jQuery(".est_date_of_work_of_each_labour_from_date_class").datepicker({ 
							   dateFormat: "dd-mm-yy",
							   //minDate: 0,
							   maxDate: "+1Y+6M",
							   changeMonth: true, 
							   changeYear: true,
							   onSelect: function (dateStr) {
									var min = jQuery(this).datepicker("getDate");
									jQuery(".est_date_of_work_of_each_labour_to_date_class").datepicker("option", "minDate", min || "0");
									
									if(jQuery(".est_date_of_work_of_each_labour_to_date_class").datepicker("getDate") != ''){
										var start 	= jQuery(".est_date_of_work_of_each_labour_from_date_class").datepicker("getDate");
										var end 	= jQuery(".est_date_of_work_of_each_labour_to_date_class").datepicker("getDate");
										var days 	= (end - start) / (1000 * 60 * 60 * 24);
											jQuery("#est_date_of_work_of_each_labour_total_months_edit_id").val(days);
									}
       							}
      						});
							
	jQuery(".est_date_of_work_of_each_labour_to_date_class").datepicker({
							   dateFormat: "dd-mm-yy",
							   //minDate: "0",
							   //maxDate: "+1Y+6M",
							   changeMonth: true, 
							   changeYear: true,
							   yearRange: "-10:+30",
							   onSelect: function (dateStr) {
									var max = jQuery(this).datepicker("getDate");
									jQuery("#datepicker").datepicker("option", "maxDate", max || "+1Y+6M");
									var start = jQuery(".est_date_of_work_of_each_labour_from_date_class").datepicker("getDate");
									var end = jQuery(".est_date_of_work_of_each_labour_to_date_class").datepicker("getDate");
									var days = (end - start) / (1000 * 60 * 60 * 24);
									 jQuery("#est_date_of_work_of_each_labour_total_months_edit_id").val(days);
       						   }
      						});
     					
}



