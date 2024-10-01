

function checkLabourNumber(){
	
	
	var nolabour 					= 	jQuery('.nolabour').val(); 
	var serialno_id 				= 	jQuery("#serialno_id").val();
	var application_flag_id 		= 	jQuery("#application_flag_id").val();
	var pre_contract_labour			=	jQuery("#pre_contract_labour").val();
	var currecnt_deposit_fees		=	jQuery("#currecnt_deposit_fees").val();
	var max_no_of_labours_license_renewal_id		 =	jQuery("input[name=max_of_contract_labour_hidden]").val();
	var max_labour_previous			=	jQuery("#max_labour_previous").val();
	var	co_oparative_yes			=	jQuery("#edit-is-coparative-1").is(":checked");
	var previous_deposit_fees		=	jQuery("#previous_security").val();	
	var due_security_fees			=   jQuery('.due_security_fees').val();
	var given_no_labour		 		=	jQuery("input[name=given_no_labour]").val();
	
		if( nolabour != "" ){
		var href 		=	window.location.href.split('/');
		var pathname	=	href[0]+'//'+href[2]+'/'+href[3]+'/';		
		jQuery.ajax({
	      	type: 'POST',
	      	url: pathname+'check_no_labour', 
			dataType: 'json', 
			data: { nolabour: nolabour, serialno_id: serialno_id , application_flag_id: application_flag_id, pre_contract_labour:pre_contract_labour ,currecnt_deposit_fees:currecnt_deposit_fees,max_no_of_labours_license_renewal_id:max_no_of_labours_license_renewal_id,max_labour_previous:max_labour_previous,co_oparative_yes :co_oparative_yes,previous_deposit_fees:previous_deposit_fees,due_security_fees:due_security_fees ,given_no_labour:given_no_labour},
	     
	        success: function(data){ 
			if(data){
			
					jQuery("#common_username_div").html("<font color='red'><strong>"+data.message+"</strong></font>").show();
				
					jQuery("#edit-ammendment-fees").val(data.applicable_ammount);
					if(data.payble == null)
					    var payable_amendment_fees = 0.00;
					else
						var payable_amendment_fees = data.payble;
					
					if(data.ammendent_security_fees == null)
					    var ammendent_security_fees = 0.00;
					else
						var ammendent_security_fees = data.ammendent_security_fees;
					
					jQuery("#edit-payable-amendment-fees").val(payable_amendment_fees);
					
					jQuery("#deposit_fees").val(data.deposit_fees_value);
					jQuery('.security_fees').val(ammendent_security_fees);
					jQuery('.payble_security_fees').val(data.paymantable_sucurity_ammenedent_fees);
					jQuery('.previous_max_security').val(data.paymantable_sucurity_ammenedent_fees);
					jQuery("#deposited_security").val(data.deposited_security_fees);
					
				}
			}
	    });
	}
}

function check_coparetive(){
	
		var nolabour 					= 	jQuery('.nolabour').val();
		var serialno_id 				= 	jQuery("#serialno_id").val();
		var application_flag_id 		= 	jQuery("#application_flag_id").val();
		var pre_contract_labour			=	jQuery("#pre_contract_labour").val();
		var currecnt_deposit_fees						 =	jQuery("#currecnt_deposit_fees").val();
		var max_no_of_labours_license_renewal_id		 =	jQuery("input[name=max_of_contract_labour_hidden]").val();
		var max_labour_previous							 =	jQuery("#max_labour_previous").val();
		var	co_oparative_yes							 =	jQuery("#edit-is-coparative-1").is(":checked");
		var previous_deposit_fees						=	jQuery("#previous_security").val();	
		var due_security_fees			=   jQuery('.due_security_fees').val();
		var given_no_labour		 		=	jQuery("input[name=given_no_labour]").val();
	  
		if( nolabour != "" ){
		// var href 		=	window.location.href.split('/');
		// var pathname	=	href[0]+'//'+href[2]+'/'+href[3]+'/';
		jQuery.ajax({
	      	type: 'POST',
	      	url: 'https://wblc.gov.in/amendment_license_renewal/security_fees', 
			dataType: 'application/json', 
			data: { 'nolabour': nolabour,'serialno_id' : serialno_id ,'application_flag_id': application_flag_id, 'pre_contract_labour':pre_contract_labour ,'currecnt_deposit_fees':currecnt_deposit_fees,'max_no_of_labours_license_renewal_id':max_no_of_labours_license_renewal_id,'max_labour_previous':max_labour_previous,'co_oparative_yes' :co_oparative_yes,'previous_deposit_fees':previous_deposit_fees,'due_security_fees':due_security_fees ,'given_no_labour':given_no_labour},
	  	
	        success: function(msg){ 
			
			// alert(msg);
			var arry = msg.split('||'); //alert(arry[2]);				
					
					jQuery('.payble_security_fees').val(arry[0]);
					jQuery('.previous_max_security').val(arry[0]);
					jQuery("#deposited_security").val(arry[1]);
					jQuery("#edit-security-fees").val(arry[2]);
					
				
			}
	    });
		
	}
      
}