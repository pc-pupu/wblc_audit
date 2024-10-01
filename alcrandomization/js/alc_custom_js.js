function submit_button(area_code,inspector_code,i,id){
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

/*jQuery(document).ready(function(e) {
    var checked_val 		= 	new Array();
    jQuery('.copy_info_class :checked').each(function(i){
      checked_val[i] 		= 	jQuery(this).val();
    });
	var copy_info			=	checked_val.toString().split(',');
	for(i=0; i<checked_val.length; i++){
		if(copy_info[i] == 1){
			jQuery('#DLC').show(1000);
		}
		if(copy_info[i] == 2){
			jQuery('#BDO').show(1000);
		}
		if(copy_info[i] == 3){
			jQuery('#SDO').show(1000);
		}
		if(copy_info[i] == 4){
			jQuery('#DM').show(1000);
		}
		if(copy_info[i] == 5){
			jQuery('#JLC').show(1000);
		}
		if(copy_info[i] == 6){
			jQuery('#ADLC').show(1000);
		}
		if(copy_info[i] == 7){
			jQuery('#LC').show(1000);
		}
	}
	
	var unchecked_val 			= 	new Array();
    jQuery('.copy_info_class').not(':checked').each(function(i){
      	unchecked_val[i] 		= 	jQuery(this).val();
    });
	var unchecked_copy_info		=	unchecked_val.toString().split(',');
	for(i=1; i<unchecked_copy_info.length; i++){
		if(unchecked_copy_info[i] == 1){
			jQuery('#DLC').hide(1000);
		}
		if(unchecked_copy_info[i] == 2){
			jQuery('#BDO').hide(1000);
		}
		if(unchecked_copy_info[i] == 3){
			jQuery('#SDO').hide(1000);
		}
		if(unchecked_copy_info[i] == 4){
			jQuery('#DM').hide(1000);
		}
		if(unchecked_copy_info[i] == 5){
			jQuery('#JLC').hide(1000);
		}
		if(unchecked_copy_info[i] == 6){
			jQuery('#ADLC').hide(1000);
		}
		if(unchecked_copy_info[i] == 7){
			jQuery('#LC').hide(1000);
		}
	}
});

function copy_info_fun(){
	
	var checked_val 		= 	new Array();
    jQuery('.copy_info_class :checked').each(function(i){
      checked_val[i] 		= 	jQuery(this).val();
    });
	var copy_info			=	checked_val.toString().split(',');
	for(i=0; i<checked_val.length; i++){
		if(copy_info[i] == 1){
			jQuery('#DLC').show(1000);
		}
		if(copy_info[i] == 2){
			jQuery('#BDO').show(1000);
		}
		if(copy_info[i] == 3){
			jQuery('#SDO').show(1000);
		}
		if(copy_info[i] == 4){
			jQuery('#DM').show(1000);
		}
		if(copy_info[i] == 5){
			jQuery('#JLC').show(1000);
		}
		if(copy_info[i] == 6){
			jQuery('#ADLC').show(1000);
		}
		if(copy_info[i] == 7){
			jQuery('#LC').show(1000);
		}
	}
	
	var unchecked_val 			= 	new Array();
    jQuery('.copy_info_class').not(':checked').each(function(i){
      	unchecked_val[i] 		= 	jQuery(this).val();
    });
	var unchecked_copy_info		=	unchecked_val.toString().split(',');
	for(i=1; i<unchecked_copy_info.length; i++){
		if(unchecked_copy_info[i] == 1){
			jQuery('#DLC').hide(1000);
		}
		if(unchecked_copy_info[i] == 2){
			jQuery('#BDO').hide(1000);
		}
		if(unchecked_copy_info[i] == 3){
			jQuery('#SDO').hide(1000);
		}
		if(unchecked_copy_info[i] == 4){
			jQuery('#DM').hide(1000);
		}
		if(unchecked_copy_info[i] == 5){
			jQuery('#JLC').hide(1000);
		}
		if(unchecked_copy_info[i] == 6){
			jQuery('#ADLC').hide(1000);
		}
		if(unchecked_copy_info[i] == 7){
			jQuery('#LC').hide(1000);
		}
	}
}*/

jQuery(document).ready(function(e) {
    jQuery( "#alc_date" ) .datepicker({
      	changeMonth: true,
      	changeYear: true,
		dateFormat: 'yy-mm-dd'
    });
});