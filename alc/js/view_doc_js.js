
  jQuery(document).ready(function(){
	
 		jQuery(".popup").click(function(e){
		  	var id = jQuery(this).attr('id'); 
			e.preventDefault(); // this will prevent the browser to redirect to the href
    		jQuery( "#dialog_"+id ).dialog();
			
	  });
	  
	  
	  jQuery(".viewfile").click(function(e){	 
		  	 var id = jQuery(this).attr('id'); 
        	e.preventDefault(); // this will prevent the browser to redirect to the href
    		jQuery( "#file_"+id ).dialog();
			
	  });
	  
	  
	    jQuery("#show_date").datepicker({
				 dateFormat: "dd-mm-yy",
				 changeMonth: true, 
				 changeYear: true,
				

      	});
	  
	  
	  
			
		
  });
  
  function datepicker_alcenter(){

							
	jQuery("#show_date").datepicker({
							   dateFormat: "dd-mm-yy",
							   //minDate: 0,
							   //maxDate: "+1Y+6M",
							   changeMonth: true, 
							   changeYear: true,
							  
      						});
	
							
	
     					
}
  
   
  
  
  
  