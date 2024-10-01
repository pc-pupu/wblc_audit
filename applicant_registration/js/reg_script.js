function setCheckPasswordStrength(){ 
	var pwd1			=	jQuery("#passwordId").val();
	var pwd2			=	jQuery("#conpasswordId").val();
	var pwdIdSet 		= 	jQuery('#setPasswordMessage');
	
	var lowPassword 	= 	/(?=.{8,}).*/;
	var mediumPassword 	= 	/^(?=\S*?[a-z])(?=\S*?[0-9])\S{5,}$/;
	var averagePassword = 	/^(?=\S*?[A-Z])(?=\S*?[a-z])(?=\S*?[0-9])\S{5,}$/;
	var strongPassword 	= 	/^(?=\S*?[A-Z])(?=\S*?[a-z])(?=\S*?[0-9])(?=\S*?[^\w\*])\S{5,}$/;
	
	
	jQuery("#passwordId").keyup(function(){
		document.getElementById("setPasswordMessage").style.display="block"; 
		if(strongPassword.test(pwd1)) { 
			pwdIdSet.removeClass().addClass('strongPassword').html("Very Strong! Please use this password!"); 
		}	else if(averagePassword.test(pwd1)) { 
			pwdIdSet.removeClass().addClass('averagePassword').html("Strong! Tips: Enter special chars to make even stronger"); 
		}	else if(mediumPassword.test(pwd1)) { 
			pwdIdSet.removeClass().addClass('mediumPassword').html("Good! Tips: Enter uppercase letter to make strong"); 
		} else if(lowPassword.test(pwd1)) { 
			pwdIdSet.removeClass().addClass('stilllowPassword').html("Still Weak! Tips: Enter digits to make good password"); 
		} else { 
			pwdIdSet.removeClass().addClass('lowPassword').html("Very Weak! Please use 8 or more chars password"); 
		} 
		if( pwd1 == "" ){
			document.getElementById("setPasswordMessage").style.display="none";
		}
	});
	jQuery("#conpasswordId").keyup(function(){
		if( pwd1 !== pwd2 ) { 
			pwdIdSet.removeClass().addClass('lowPassword').html("Password do not match!");	
		}else{ 
			pwdIdSet.removeClass().addClass('goodpass').html("Password match!");	
		} 
	});
}
jQuery(window).load(function () {
//jQuery(document).ready(function() {
	document.getElementById("setPasswordMessage").style.display="block"; 
	jQuery('#setPasswordMessage').html('* Password must be atleast 8 characters, should have atleast 1 upper case letter, should have atleast 1 numeric digit, should have special characters');
	jQuery('#setPasswordMessage').css({'font-style': 'italic', 'color': '#2e2e2e', 'font-size' : '0.85em','font-weight':'400','line-height':'14px'});
	
	var changeTooltipPosition = function(event) {
		var tooltipX = event.pageX - 2;
		var tooltipY = event.pageY + 2;
		jQuery('div.tooltip').css({top: tooltipY, left: tooltipX});
	};

	var showTooltip = function(event) {
	  jQuery('div.tooltip').remove();
	  jQuery('<div class="tooltip">* Password must be atleast 8 characters.<br> * Password should have atleast 1 upper case letter.<br> * Password should have atleast 1 numeric digit.<br> * Password should have any of these special characters i.e. !@#$%^&*()_+</div>').appendTo('body');
	  changeTooltipPosition(event);
	};

	var hideTooltip = function() {
	   jQuery('div.tooltip').remove();
	};

	jQuery("span#hint").bind({
	   mousemove : changeTooltipPosition,
	   mouseenter : showTooltip,
	   mouseleave: hideTooltip
	});
	
	////////////////////////////////////    Phone  Markup  Tooltip      ////////////////////////////////////////////////////////////////
	
	var pchangeTooltipPosition = function(event) {
		var ptooltipX = event.pageX - 2;
		var ptooltipY = event.pageY + 2;
		jQuery('div.ptooltip').css({top: ptooltipY, left: ptooltipX});
	};

	var pshowTooltip = function(event) {
	  jQuery('div.ptooltip').remove();
	  jQuery('<div class="ptooltip"> Please give valid mobile number to get sms alert.</div>')
            .appendTo('body');
	  pchangeTooltipPosition(event);
	};

	var phideTooltip = function() {
	   jQuery('div.ptooltip').remove();
	};

	jQuery("span#hint_phone").bind({
	   mousemove : pchangeTooltipPosition,
	   mouseenter : pshowTooltip,
	   mouseleave: phideTooltip
	});
	
	///////////////////////////////////////////////  END  //////////////////////////////////////////////////////////////////////////////
	
});

jQuery(document).ready(function(e) { 
    if (jQuery.browser.mozilla) {
   		jQuery("#hint").css('margin', '0 0 0 -648px');
	}
	
	if (jQuery.browser.webkit) {
   		jQuery("#hint").css('margin', '0 0 0 -276px');
	}
	
	if (jQuery.browser.mozilla) {
   		jQuery("#hint_phone").css('margin', '0px 0 0 -607px');
	}
	
	if (jQuery.browser.webkit) {
   		jQuery("#hint_phone").css('margin', '-1px 0 0 -239px');
	}
	
});

function checkUserName(){
	var username 		= 	jQuery("#username_id").val();
	if( username != "" ){
		var href 		=	window.location.href.split('/');
		var pathname	=	href[0]+'//'+href[2]+'/'+href[3]+'/';
		var loc 		= 	window.location.protocol+"//"+window.location.hostname;
		var loc_f 		= 	window.location.protocol+"//"+window.location.hostname+"/";
		jQuery.ajax({
	      	type: 'POST',
	      	url: loc_f+'checkname', 
			dataType: 'json', 
	     	data: { username: username },
	      	success: function(data){ 
				if( data == "Exist" ){
					jQuery("#common_username_div").html("<font color='red'><strong>This username already exists.</strong></font>").show();
					jQuery("#common_username_img_div").html("<img src="+loc+"/sites/all/modules/applicant_registration/images/DeleteRed.png height='20px' width='24px'>").show();
				}else if( data == "DoesNotExist" ){
					jQuery("#common_username_div").html("<font color='green'><strong>Username available.</strong></font>").show();
					jQuery("#common_username_img_div").html("<img src="+loc+"/sites/all/modules/applicant_registration/images/tick-128.png height='20px' width='24px'>").show();
				}
			}
	    });
	}else{
		jQuery("#common_username_div").html("<font color='red'><strong>This username already exists.</strong></font>").hide();
		jQuery("#common_username_img_div").html("<img src="+loc+"/sites/all/modules/applicant_registration/images/DeleteRed.png height='20px' width='24px'>").hide();
		jQuery("#common_username_div").html("<font color='green'><strong>Username available.</strong></font>").hide();
		jQuery("#common_username_img_div").html("<img src="+loc+"/sites/all/modules/applicant_registration/images/tick-128.png height='20px' width='24px'>").hide();
	}
}

function checkEmail(){
	var email 		= 	jQuery("#email_id").val();
	if( email != "" ){
		//alert(11);
		var href 		=	window.location.href.split('/');
		var pathname	=	href[0]+'//'+href[2]+'/'+href[3]+'/';
		var loc 		= 	window.location.protocol+"//"+window.location.hostname;
		var loc_f 		= 	window.location.protocol+"//"+window.location.hostname+"/";
		//alert(loc);
		
		jQuery.ajax({
	      	type: 'POST',
	      	url: loc_f+'checkemail', 
			dataType: 'json', 
	     	data: { email: email },
	      	success: function(data){ 
				if( data == "Exist" ){
					jQuery("#common_email_div").html("<font color='red'><strong>This email already exists.</strong></font>").show();
					jQuery("#common_email_img_div").html("<img src="+loc+"/sites/all/modules/applicant_registration/images/DeleteRed.png height='20px' width='24px'>").show();
					jQuery("#email_id").val('');
				}else if( data == "DoesNotExist" ){
					jQuery("#common_email_div").html("<font color='green'><strong>Email available.</strong></font>").show();
					jQuery("#common_email_img_div").html("<img src="+loc+"/sites/all/modules/applicant_registration/images/tick-128.png height='20px' width='24px'>").show();
				}
			}
	    });
	}else{
		jQuery("#common_email_div").html("<font color='red'><strong>This email already exists.</strong></font>").hide();
		jQuery("#common_email_img_div").html("<img src="+loc+"/sites/all/modules/applicant_registration/images/DeleteRed.png height='20px' width='24px'>").hide();
		jQuery("#common_email_div").html("<font color='green'><strong>Email available.</strong></font>").hide();
		jQuery("#common_email_img_div").html("<img src="+loc+"/sites/all/modules/applicant_registration/images/tick-128.png height='20px' width='24px'>").hide();
	}
}

var CryptoJSAesJson = {
    stringify: function (cipherParams) {
        var j = {ct: cipherParams.ciphertext.toString(CryptoJS.enc.Base64)};
        if (cipherParams.iv) j.iv = cipherParams.iv.toString();
        if (cipherParams.salt) j.s = cipherParams.salt.toString();
        return JSON.stringify(j);
    },
    parse: function (jsonStr) {
        var j = JSON.parse(jsonStr);
        var cipherParams = CryptoJS.lib.CipherParams.create({ciphertext: CryptoJS.enc.Base64.parse(j.ct)});
        if (j.iv) cipherParams.iv = CryptoJS.enc.Hex.parse(j.iv)
        if (j.s) cipherParams.salt = CryptoJS.enc.Hex.parse(j.s)
        return cipherParams;
    }
}

function pwd_handler(form){	
	//var x = form.pass.value;
	var password = document.getElementById('passwordId').value;
	var con_password = document.getElementById('conpasswordId').value;
	var card_number = document.getElementById('card_number_id').value;
	
	
	
	var key = "Lc#Nic@1720";
	var encrypted = CryptoJS.AES.encrypt(JSON.stringify(password), key, {format: CryptoJSAesJson}).toString();
	var con_encrypted = CryptoJS.AES.encrypt(JSON.stringify(con_password), key, {format: CryptoJSAesJson}).toString();
	var card_numberen = CryptoJS.AES.encrypt(JSON.stringify(card_number), key, {format: CryptoJSAesJson}).toString();
	
	var j = JSON.parse(encrypted);
	var i = JSON.parse(con_encrypted);
	var cn = JSON.parse(card_numberen);
	
	
	jQuery("#passwordId").val(j.iv);
	jQuery("#ct_val").val(j.ct);
	jQuery("#s_val").val(j.s);
	
	jQuery("#conpasswordId").val(i.iv);
	jQuery("#ct_val_con").val(i.ct);
	jQuery("#s_val_con").val(i.s);
	
	jQuery("#card_number_id").val(cn.iv);
	jQuery("#ct_val_cn").val(cn.ct);
	jQuery("#s_val_cn").val(cn.s);
	
	
}


