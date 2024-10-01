

jQuery(document).ready(function() {		
	jQuery("#back_to_applicant").click(function(e){		
		var field_name = jQuery("#field_name").val();
		if(confirm('want to continue')){ 
			jQuery("#chech_data").val(field_name);
		}
	});
	
});



jQuery(document).ready(function(){		
		jQuery(".verified_yn").click(function(){						
					
			var fieldIdt = jQuery("#field_name").val();					
			
			if (jQuery(this).is(':checked')) {				
				var fieldId = jQuery(this).attr('id');
				var fieldIdt = fieldIdt+','+fieldId;
				jQuery("#field_name").val(fieldIdt);
			}
						
			var field_name = jQuery("#field_name").val();
			
			if (!jQuery(this).is(':checked')) {	
				var fieldIdt = '';			
				var fieldId = jQuery(this).attr('id');				
				jQuery("#field_name").val('');
				// alert(field_name);				
				var field_name_arr = field_name.split('|');
				for(var i=0;i<field_name_arr.length;i++){					
        			if(field_name_arr[i] != fieldId){
						var fieldIdt = fieldIdt+','+field_name_arr[i];
						fieldIdt = fieldIdt.replace(',,', ',');
						jQuery("#field_name").val(fieldIdt);
					}
   				}
    			
			}
									
			// alert(e_name);
			// alert(field_name);		
			// alert(applicant_id);					
			// jQuery(".tzCheckBox").eq(1).addClass("checked");		
		});			
	});
