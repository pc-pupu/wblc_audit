jQuery(document).ready(function(e) {
	//alert(11);
	var year_1 			= 	new Date();
	var current_yr 		= 	year_1.getFullYear();
	var year_2			=	new Date();
	var next_yr			=	year_2.getFullYear()+1;
	var finalYearrange	=	current_yr+':'+next_yr;
		
	for(i=1; i<=100; i++){
		jQuery("#loading_"+i).hide();
		jQuery( "#date_from_id_"+i ).datepicker({
												dateFormat: "dd-mm-yy", 
												onSelect: function(selected) {
																jQuery( "#date_to_id_"+i ).datepicker("option","minDate", selected)
												}, 
												changeMonth: true, 
												changeYear: true, 
												autoSize: true,
												yearRange: finalYearrange
		}).datepicker("setDate", "0");
		
		jQuery( "#date_to_id_"+i ).datepicker({
												dateFormat: "dd-mm-yy", 
												onSelect: function(selected) {
	     														jQuery( "#date_from_id_"+i ).datepicker("option","maxDate", selected) 
												}, 
												changeMonth: true, 
												changeYear: true, 
												autoSize: true,
												yearRange: finalYearrange
		}).datepicker("setDate", "0");
	}
	
	jQuery( "#alc_date" ) .datepicker({
      	changeMonth: true,
      	changeYear: true,
		dateFormat: 'yy-mm-dd'
    });
	
});

function submit_button_kol(area_code,inspector_code,i,id){
	var href 			=	window.location.href.split('/');
	var pathname		=	href[0]+'//'+href[2]+'/'+href[3]+'/';
	var loc 			= 	window.location.protocol+"//"+window.location.hostname;
	var loc_f 			= 	window.location.protocol+"//"+window.location.hostname+"/";
	var current_date	=	jQuery("#current_date_"+i).val();
	
	if( current_date != "" && area_code != "" && inspector_code != "" && i != "" && id != "" ){
		if(confirm("Are you sure you want to submit?")){
		
			jQuery("#loading_"+i).show();
			var date_from			=	jQuery("#date_from_id_"+i).val();
			var date_to				=	jQuery("#date_to_id_"+i).val();
			
			var date_from_exp		=	date_from.split('-');
			var from_d				=	date_from_exp[0];
			var from_m				=	date_from_exp[1];
			var from_y				=	date_from_exp[2];
			
			var finalFrom			=	from_y+'-'+from_m+'-'+from_d;
			
			var date_to_exp			=	date_to.split('-');
			var to_d				=	date_to_exp[0];
			var to_m				=	date_to_exp[1];
			var to_y				=	date_to_exp[2];
			
			var finalTo				=	to_y+'-'+to_m+'-'+to_d;
			
			if(date_from != "" && date_to != ""){
				if( finalFrom < current_date  ){
					jQuery("#date_from_id_"+i).css("background-color","#ff8080");
					jQuery("#loading_"+i).hide();
					return false;
				}else if( finalFrom > finalTo ){
					jQuery("#date_from_id_"+i).css("background-color","#ff8080");
					jQuery("#loading_"+i).hide();
					return false;
				}else if( finalFrom < finalTo ){
					jQuery("#date_from_id_"+i).css("background-color","#DBF2FE");
					jQuery("#date_to_id_"+i).css("background-color","#DBF2FE");
					jQuery.ajax({
						type: 'POST',
						url: loc_f+'randomization-submit', 
						dataType: 'json', 
						data: { area_code: area_code, inspector_code: inspector_code, finalFrom: finalFrom, finalTo: finalTo, id: id  },
						success: function(data){
							if( data == "Submitted" ){
								jQuery("#loading_"+i).hide();
								jQuery("#submission_"+i).hide();
								jQuery("#submission_"+i).html("<font color='red'><strong>Submitted</strong></font>").show();
								window.location = loc_f + 'alcrandomization';
							}else if( data == "NotSubmitted" ){
								jQuery("#loading_"+i).hide();
								jQuery("#submission_"+i).show();
								jQuery("#submission_"+i).html("<font color='red'><strong>Submitted</strong></font>").hide();
							}
						}
					});
				}
			}
			
		}else{
			return false;
		}
	}
}