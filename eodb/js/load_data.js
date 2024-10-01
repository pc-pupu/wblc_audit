function ajax_load(serviceId) {
    let loc_f =	window.location.protocol+"//"+window.location.hostname+"/";
    let ajaxTarget = "#ajax-target-"+serviceId;
    $('#ajax-loader').show();
    jQuery(ajaxTarget).load(loc_f+'eodb_dashboard_service_details/'+serviceId, function(responseTxt, statusTxt, xhr){
        $('#ajax-loader').hide();
    });
}