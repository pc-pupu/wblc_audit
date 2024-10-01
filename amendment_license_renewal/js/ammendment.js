jQuery(document).ready(function(){
	
	
jQuery(".ammendment_date").datepicker({
							   dateFormat: "dd-mm-yy",
							   //minDate: 0,
							   //maxDate: "+1Y+6M",
							   changeMonth: true, 
							   changeYear: true,
							   onSelect: function (dateStr) {
									//var min = jQuery(this).datepicker("getDate");
									//jQuery(".last_renewal_date").datepicker("option", "minDate", min || "0");
									
									/*if(jQuery(".last_renewal_date").datepicker("getDate") != ''){
										var start 	= jQuery(".last_renewal_date").datepicker("getDate");
										var end 	= jQuery(".last_renewal_date").datepicker("getDate");
										var days 	= (end - start) / (1000 * 60 * 60 * 24);
										jQuery("#est_date_of_work_of_each_labour_total_months_edit_id").val(days);
									}*/
       							}

      						});
jQuery(".last_renewal_date_1").datepicker({
							   dateFormat: "dd-mm-yy",
							   //minDate: 0,
							   //maxDate: "+1Y+6M",
							   changeMonth: true, 
							   changeYear: true,
							   onSelect: function (dateStr) {
									//var min = jQuery(this).datepicker("getDate");
									//jQuery(".last_renewal_date").datepicker("option", "minDate", min || "0");
									
									/*if(jQuery(".last_renewal_date").datepicker("getDate") != ''){
										var start 	= jQuery(".last_renewal_date").datepicker("getDate");
										var end 	= jQuery(".last_renewal_date").datepicker("getDate");
										var days 	= (end - start) / (1000 * 60 * 60 * 24);
										jQuery("#est_date_of_work_of_each_labour_total_months_edit_id").val(days);
									}*/
       							}

      						});
	



});

function datepicker_lastrenewal(){

							
	jQuery(".ammendment_date").datepicker({
							   dateFormat: "dd-mm-yy",
							   //minDate: 0,
							   //maxDate: "+1Y+6M",
							   changeMonth: true, 
							   changeYear: true,
							   onSelect: function (dateStr) {
									//var min = jQuery(this).datepicker("getDate");
									//jQuery(".last_renewal_date").datepicker("option", "minDate", min || "0");
									
									/*if(jQuery(".last_renewal_date").datepicker("getDate") != ''){
										var start 	= jQuery(".last_renewal_date").datepicker("getDate");
										var end 	= jQuery(".last_renewal_date").datepicker("getDate");
										var days 	= (end - start) / (1000 * 60 * 60 * 24);
										jQuery("#est_date_of_work_of_each_labour_total_months_edit_id").val(days);
									}*/
       							}
      						});
	jQuery(".last_renewal_date_1").datepicker({
							   dateFormat: "dd-mm-yy",
							   //minDate: 0,
							   //maxDate: "+1Y+6M",
							   changeMonth: true, 
							   changeYear: true,
							   onSelect: function (dateStr) {
									//var min = jQuery(this).datepicker("getDate");
									//jQuery(".last_renewal_date").datepicker("option", "minDate", min || "0");
									
									/*if(jQuery(".last_renewal_date").datepicker("getDate") != ''){
										var start 	= jQuery(".last_renewal_date").datepicker("getDate");
										var end 	= jQuery(".last_renewal_date").datepicker("getDate");
										var days 	= (end - start) / (1000 * 60 * 60 * 24);
										jQuery("#est_date_of_work_of_each_labour_total_months_edit_id").val(days);
									}*/
       							}
      						});
							
	
     					
}

