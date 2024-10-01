jQuery(document).ready(function(e){ 	
	jQuery(".changestatus").live('click', function(e){
		var base_url = jQuery("input[type='hidden'][name='base_url']").val();		
		var id = parseInt(jQuery(this).attr('data-id'));  
		var status = parseInt(jQuery(this).attr('data-st'));
		
		var cl_number = jQuery(this).attr('data-cln');
		var formVst = jQuery(this).attr('data-fv');
		var contractor_type = jQuery(this).attr('data-ctype');
		
		if(status == 1)	
		jQuery("#deacstatusId_"+id).hide();	
		
		if(status == 0)	
		jQuery("#acstatusId_"+id).hide();
		
		jQuery("#loading_"+id).show();		
				
		var data = 'id='+id+'&status='+status;
			
		jQuery.ajax({
			type:'POST',
			url: base_url+'/changeconstatus',
			data: data,				
			success: function(msg){	 // alert(msg);									 
				if(jQuery.trim(msg) == 'success'){
					jQuery("#loading_"+id).hide();
					if(status == 1){
						jQuery("#acstatusId_"+id).show();
						if(cl_number < 10){
							jQuery("#formVstatus_"+id).text('Not Applicable');
						}else{
							if(formVst == 1){
								if(contractor_type == 2){
									var download_link = '<a href="'+base_url+'/pdf_from_v_download/'+id+'" target="_blank">Download Form V</a>';
									jQuery("#formVstatus_"+id).html(download_link);
								}else{
									jQuery("#formVstatus_"+id).text(id);
								}
							}else{
								jQuery("#formVstatus_"+id).text('Pending');
							}
						}
					}
					if(status == 0){
						jQuery("#deacstatusId_"+id).show();
						jQuery("#formVstatus_"+id).html('<strong><font color="#f00">Cancelled</font></strong>');
					}					
				}else{
					alert('Fail. Try again!');
				}
			}
		 });
	});
});