jQuery(window).load(function (){ 
jQuery("#btnSubmit_contact").click(function (event) { 
 	var formName = jQuery('#form_name_id').val(); 
	var sub_div_code = jQuery('#sub_div_id').val(); 	
	if(formName == 'add_contact_number_form'){ 
		 event.preventDefault();
		
		// Get form
		var form = jQuery('#add_contact_number_form')[0]; 
		// Create an FormData object 
		var data = new FormData(form);
		
		var contact_number = jQuery("#contact_number_id").val();
		
		// Validation of Field
		var numbers = /^[0-9]+\.?[0-9]*$/;
			
		if(contact_number == "" || !contact_number.match(numbers)){
			document.getElementById('contact_span_id').style.display="block";
			document.getElementById('contact_number_id').focus();
		}else{
			document.getElementById('contact_span_id').style.display="none";
			var contact_number_value = 1; 
		}
		
		if(contact_number_value == 1){
			data.append("contact_number", contact_number); 
			data.append("sub_div_code", sub_div_code); 
			// disabled the submit button
			jQuery("#btnSubmit_contact").prop("disabled", true);
				jQuery.ajax({
					type: 'POST',
					url:'https://wblc.gov.in/add-edit-contact-number-submit',
					data: data,
					processData: false,
					contentType: false,
					cache: false,
					timeout: 600000,
					success: function (data) {
					 alert(data);
					 document.getElementById('contact_number').value = '';
					 window.location.href = "https://wblc.gov.in/office-contact-admin-details/"+sub_div_code;
					},
					error: function () {
						//jQuery("#result").text(e.responseText);
						alert("All fields are mandatory", e);
						//console.log("ERROR : ", e);
						jQuery("#btnSubmit_contact").prop("disabled", false);
					}
				});
		}
	}
});

jQuery("#btnSubmit_email").click(function (event) { 
 	var formName = jQuery('#form_email_id').val(); 
	var sub_div_code = jQuery('#sub_div_id').val(); 	
	if(formName == 'add_email_form'){ 
		 event.preventDefault();
		
		// Get form
		var form = jQuery('#add_email_form')[0]; 
		// Create an FormData object 
		var data = new FormData(form);
		
		var email = jQuery("#email_id").val();
		
		// Validation of Field
		var mailformat = /^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$/;
			
		if(email == "" || !email.match(mailformat)){
			document.getElementById('email_span_id').style.display="block";
			document.getElementById('email_id').focus();
		}else{
			document.getElementById('email_span_id').style.display="none";
			var email_value = 1; 
		}
		
		if(email_value == 1){
			data.append("email", email); 
			data.append("sub_div_code", sub_div_code); 
			// disabled the submit button
			jQuery("#btnSubmit_email").prop("disabled", true);
				jQuery.ajax({
					type: 'POST',
					url:'https://wblc.gov.in/add-edit-email-submit',
					data: data,
					processData: false,
					contentType: false,
					cache: false,
					timeout: 600000,
					success: function (data) {
					document.getElementById('email_id').value = ''; 
					window.location.href = "https://wblc.gov.in/office-contact-admin-details/"+sub_div_code;
					},
					error: function () {
						//jQuery("#result").text(e.responseText);
						alert("All fields are mandatory", e);
						//console.log("ERROR : ", e);
						jQuery("#btnSubmit_email").prop("disabled", false);
					}
				});
		}
	}
});

});
