function user_view(id){
  

    var loc_f 		= 	window.location.protocol+"//"+window.location.hostname+"/";
      //var data = 'row_id='+data2+',mobile='+mobile+',email='+email;
      //alert(data);
      jQuery.ajax({
        type: "POST",
        url:loc_f+'view-duare-sarkar-user',
        data: {id:id},
        success	: function(message){
          jQuery('#mod').html(message);
          var modal = document.getElementById("myModal");
          modal.style.display = "block";
       
        }
      
      });
    
  
}

jQuery( window ).load(function() { 

  var span = document.getElementsByClassName("close")[0];
  var modal = document.getElementById("myModal");

// When the user clicks the button, open the modal 
span.onclick = function() {
  modal.style.display = "none";
}

})

function setCheckPasswordStrength()
{

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
 jQuery(document).ready(function(){
   
	jQuery("#edit-aadhar,#edit-bank-account,#edit-mobile").keypress( function(event) {
		
      
		// Allow backspace, delete, tab, escape, and enter keys
		 // Get the key code of the pressed key
	var keycode = event.which;
	
	// Check if the key code is not a number (0-9) or a backspace or a minus sign
	if (keycode < 48 || keycode > 57) {
	  if (keycode != 8 && keycode != 45) {
		// Prevent the default action of the keypress event (i.e. typing the character)
		event.preventDefault();
	  }
	}
	  });


     
});
function CheckEpic(){
    // $error='EPIC Number not provided.';
    // alert($error);
	var message =
          "EPIC Number not provided . Do you want to continue ?";
        if (confirm(message)) {
        } else {
          jQuery("#edit-action-taken").val('');
		  window.location.reload();
        }
}
// jQuery("#bank_branch").keyup(function(){
// 	alert('ok');
// 	//var bank_branch			=	jQuery("#bank_branch").val();
   
// 	// var pattern1 = /^[a-zA-Z]+$/;
// 	// if (pattern1.test(bank_branch) == false){
// 	//   alert("Please Provide Valid Bank Branch");
// 	//   event.preventDefault();

		  
// 	// }
//   });