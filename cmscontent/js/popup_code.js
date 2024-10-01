// JavaScript Document

jQuery("a.apply_link").live('click', function(e){ 
	var href_val = jQuery(this).attr('data-val');
	jQuery('a#dir_id').attr('href',href_val);
});
