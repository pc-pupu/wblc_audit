
function showdate() {
	jQuery('#calltime').datetimepicker({
		dayOfWeekStart : 1,
		lang:'en',
		disabledDates:['1986/01/08','1986/01/09','1986/01/10'],
		startDate:	new Date(),
		//startDate:	'1990/01/01',
	}).datetimepicker("show");
}
jQuery(document).ready(function(e) {
//jQuery("#clra_alc_call_time_id").click(function(e){		
		//alert('hi');
		//jQuery('#clra_alc_call_time').datetimepicker({
			//dayOfWeekStart : 1,
		//lang:'en',
		////disabledDates:['1986/01/08','1986/01/09','1986/01/10'],
		//startDate:	new Date(),
		//startDate:	'1990/01/01',
	//}).datetimepicker("show");
//})
 jQuery('#clra_alc_call_time_id').data("DateTimePicker").FUNCTION()
 
});
/*function showdate_test() { ;
	/*jQuery('#clra_alc_call_time').datetimepicker({
		dayOfWeekStart : 1,
		lang:'en',
		disabledDates:['1986/01/08','1986/01/09','1986/01/10'],
		startDate:	new Date(),
		//startDate:	'1990/01/01',
	}).datetimepicker("show");*/
	/*
	jQuery('#clra_alc_call_time_id').datetimepicker({
		
		timeFormat: "hh:mm tt",
		
		hourMin: 8,
		
		hourMax: 16,
	
		numberOfMonths: 2,
		
		minDate: 0,
		
		maxDate: 30
	});
	
	jQuery('#clra_alc_call_time_id').timepicker();
};
*/



