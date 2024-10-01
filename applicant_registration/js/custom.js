function checkConPassword(){
	var passwordId 		= jQuery("#passwordId").val();
	var conpasswordId	= jQuery("#conpasswordId").val();
	if( passwordId == conpasswordId ){
		// alert("Matched!!");
	}
}

jQuery(document).ready(function() {
	
	var changeTooltipPosition = function(event) {
	  var tooltipX = event.pageX - 2;
	  var tooltipY = event.pageY + 2;
	  jQuery('div.tooltip').css({top: tooltipY, left: tooltipX});
	};

	var showTooltip = function(event) {
	  jQuery('div.tooltip').remove();
	  jQuery('<div class="tooltip">Please remember to put a valid mobile<br>number as this will help you to get all<br> the sms alerts on the status of your<br> application.</div>')
            .appendTo('body');
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
	
});