jQuery(window).load(function(e){	
	jQuery("#trade_license_div").hide();
	jQuery("#article_of_assoc_div").hide();
	jQuery("#memorandum_of_cert_div").hide();
	jQuery("#partnership_deed_div").hide();
	jQuery("#factory_license_div").hide();
	jQuery('#rlo_location_id').hide();
	jQuery('#other_nature_work_id').hide();
	jQuery('#edit_other_nature_work_id').hide();
	//jQuery('#edit_other_nature_work_id').css('display','none');
	//jQuery('#other_value_nature_of_work_id').css('display','none');		
		
	var addLinkValue = jQuery('input[name=check_if_add_trade_union]:checked').val();
	
	if( addLinkValue == 1 ){
		jQuery('#toggle_markup_trade_div').show();
	}else if( addLinkValue == 0 ){
		jQuery('#toggle_markup_trade_div').hide();
	}
	
	var value_nature_of_wrk = jQuery('#other_value_nature_of_work_id').val();
	if(value_nature_of_wrk != '') {
		jQuery('#edit_other_nature_work_id').show();
	}else{
		jQuery('#other_value_nature_of_work_id').attr('value', '');
		jQuery('#edit_other_nature_work_id').hide();
	}
	
	jQuery("#e_any_day_max_num_of_workmen_id").change(function(e){
		var  worker = jQuery(this).val();
		if(worker != '' && worker < 10){
			jQuery(".workernoten").show();
		}else{
			jQuery(".workernoten").hide();
		}
	});	
});

function upload_check_fun_1(){
	if(jQuery("#upload_check_in_1_id").is(':checked')) {
		jQuery("#trade_license_div").show();
		
	}else{
		jQuery("#trade_license_div").hide();
	}
}

function upload_check_fun_2(){
	if(jQuery("#upload_check_in_2_id").is(':checked')) {
		jQuery("#article_of_assoc_div").show();
	}else{
		jQuery("#article_of_assoc_div").hide();
	}
}

function upload_check_fun_3(){
	if(jQuery("#upload_check_in_3_id").is(':checked')) {
		jQuery("#memorandum_of_cert_div").show();
	}else{
		jQuery("#memorandum_of_cert_div").hide();
	}
}

function upload_check_fun_4(){
	if(jQuery("#upload_check_in_4_id").is(':checked')) {
		jQuery("#partnership_deed_div").show();
	}else{
		jQuery("#partnership_deed_div").hide();
	}
}

function upload_check_fun_5(){
	if(jQuery("#upload_check_in_5_id").is(':checked')) {
		jQuery("#factory_license_div").show();
	}else{
		jQuery("#factory_license_div").hide();
	}
}

function getSpanText(){ 
	var x = jQuery("#loc_e_subdivision_id option:selected").text();
	var y = jQuery("#loc_e_subdivision_id").val();
	if( x != '- Select -' ){
		jQuery('#rlo_location_id').html('<b>Note: Your Application will be forwarded to RLO '+jQuery("#loc_e_subdivision_id option:selected").text()+'</b>').show();
	}
	if( y == "" ){
		jQuery('#rlo_location_id').hide();
	}
}

function getContractorAddLink(){
	var addLinkValue	=	jQuery('input[name=check_if_add_trade_union]:checked').val();
	if( addLinkValue == 1 ){
		jQuery('#toggle_markup_trade_div').show();
	}else if( addLinkValue == 0 ){
		jQuery('#toggle_markup_trade_div').hide();
	}
}

function nature_wrk_others(){
	var selected= []; 
	var selected = jQuery('select#e_nature_of_work_id').val();
	for (i=0;i<selected.length;i++){
		 if(selected[i] == 28){ 
			 jQuery('#other_nature_work_id').show();
			 jQuery('#edit_other_nature_work_id').show();
		 }else{
			jQuery('#other_value_nature_of_work_id').removeAttr('value');
			jQuery('#other_nature_work_id').hide();	
			jQuery('#edit_other_nature_work_id').hide(); 
			
		 }
	}
}

