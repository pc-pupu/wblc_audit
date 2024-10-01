// JavaScript Document

	
 function showme(id) {
	 //alert('sssssssssssss'+id);
        var divid = document.getElementById(id);
        if (divid.style.display == 'block') {
          divid.style.display = 'none';
          $('#toggleDisplay').text('Show Widget');
        }
        else{ 
          divid.style.display = 'block';
          $('#toggleDisplay').text('Hide Widget');
        }
    }
	
	 // JavaScript Document