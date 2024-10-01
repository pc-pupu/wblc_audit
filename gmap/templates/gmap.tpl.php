<?php
$query = db_select('sub_division', 'sd');
$query ->leftJoin('l_rlo_additional_info','ra', 'sd.sub_div_code=ra.sub_div_code');
$query->fields('sd', array('sub_div_name', 'district_name'));
$query->fields('ra', array('office_name','clra_certificate_address', 'office_mobile_number', 'latitude', 'longitude'));
$query->condition('sd.is_active', 'Y');
$query->condition('ra.latitude', 0, '!=');
$query->condition('ra.longitude', 0, '!=');
$query->orderBy('sd.district_name', 'ASC');
$result = $query->execute(); 

$addrStr = "";	
if($result->rowCount() > 0){
	  foreach($result as $data){ 
			if ($data->office_name == 'JLC'){
				$office_name = 'OFFICE OF THE JOINT LABOUR COMMISSIONER';
			}elseif ($data->office_name == 'DLC'){
				$office_name = 'OFFICE OF THE DEPUTY LABOUR COMMISSIONER';
			}elseif($data->office_name == 'ALC'){
				$office_name = 'OFFICE OF THE ASSISTANT LABOUR COMMISSIONER';
			}elseif($data->office_name == 'KLC'){
				$office_name = 'OFFICE OF THE LABOUR COMMISSIONER EL & MW SECTION';
			}
			
			// $addrStr.= "['".$office_name."<br>Address:".$data->clra_certificate_address."<br>Contact:".$data->office_mobile_number."', ".$data->latitude.", ".$data->longitude.", 1],";
			$addrStr.= "['".$office_name.', '.strtoupper($data->sub_div_name)."', ".$data->latitude.", ".$data->longitude.", 1],";
	  }
}

$query = db_select('map_table', 'm');
$query->leftJoin('map_block_table', 'b', 'm.dis_code=b.bdo_dis_code');
$query->fields('m', array('rlo_name', 'telephone_no', 'designation', 'mobile_no', 'email','address','latitude','longitude')); 
$query->fields('b', array('bdo_offices', 'bdo_ofc_address', 'bdo_latitude', 'bdo_longitude')); 
//$query->distinct('m.rlo_name');
$query->condition('m.srlno', array(1),'IN');
$qry_data = $query->execute();


foreach($qry_data as $val){
	$addrStr.= "['".strtoupper($val->rlo_name)."', ".$val->latitude.", ".$val->longitude.", 1],";	
}
// echo $addrStr; die;
?>
<script type="text/javascript" src="https://maps.googleapis.com/maps/api/js?key=AIzaSyDvk5Z488EnNtRYH9uWDF37-79MIg6SlgY"></script>
    <script type="text/javascript">
        /*var locations = [            
			['DARJEELING', 27.0410, 88.2663, 1],
            ['PURULIA', 23.3322, 86.3616, 2],
            ['NADIA', 23.4710, 88.5565, 3],
            ['India Gate', 28.620585, 77.228609, 4],
            ['Jantar Mantar', 28.636219, 77.213846, 5],
            ['Akshardham', 28.622658, 77.277704, 6],
			['NSB', 22.5698, 88.3434, 7]
        ];*/
		
		var locations = [<?php echo $addrStr;?>];

        function InitMap() {

            var map = new google.maps.Map(document.getElementById('map'), {
                zoom: 8,
                center: new google.maps.LatLng(22.5726, 88.3639),
                mapTypeId: google.maps.MapTypeId.ROADMAP
            });

            var infowindow = new google.maps.InfoWindow();

            var marker, i;

            for (i = 0; i < locations.length; i++) {
                marker = new google.maps.Marker({
                    position: new google.maps.LatLng(locations[i][1], locations[i][2]),
                    map: map
                });

                google.maps.event.addListener(marker, 'click', (function (marker, i) {
                    return function () {
                        infowindow.setContent(locations[i][0]);
                        infowindow.open(map, marker);
                    }
                })(marker, i));
            }
        }
    </script><body onLoad="InitMap();">    
    <div id="map" style="height: 500px; width: auto;">
    </div>
</body>
<?php
	$header = array(
					array('data' => 'District', 'width' => '14%'),
					array('data' => 'Name of the office', 'width' => '35%'),					
					array('data' => 'Contact Number', 'width' => '14%'),
					// array('data' => 'Officer Name')
					array('data' => 'Email Address', 'width' => '30%'),
					array('data' => 'Action', 'width' => '7%')
				  ); 
				  
	$query = db_select('sub_division', 'sd');
	$query ->leftJoin('l_rlo_additional_info','ra', 'sd.sub_div_code=ra.sub_div_code');
	$query ->leftJoin('l_custom_user_detail','cu', 'cu.usr_id=ra.user_id');
  	$query->fields('sd', array('sub_div_code', 'sub_div_name', 'district_name'));
	$query->fields('ra', array('office_name', 'office_mobile_number', 'office_email_address'));
	$query->fields('cu', array('officenumber'));
	$query->condition('ra.is_active', 'Y');
	$query->condition('sd.is_active', 'Y');
	$query->condition('sd.sub_short_code', '', '!=');
	// $query->condition('sd.district_code', '14');
	$query->orderBy('sd.district_name', 'ASC');
	$result = $query->execute(); 
	
	// $result = $result->fetchAll(); echo '<pre>'; print_r($result); die;
	
	  $rows[] = array(			
						array('data' => ''),
						array('data' => 'OFFICE OF THE LABOUR COMMISSIONERATE'),						 
						array('data' => '03322488150'),
						array('data' => 'labourcommissioner[dot]wb[at]gmail[dot]com'),
						array('data' => l('<i class="fa fa-eye fa-lg" aria-hidden="true">', 'labour-commissionerate-contact-details', array('html' => TRUE,'attributes'=> array('target'=>'_blank'))))
						);
					
	  if($result->rowCount() > 0){
	  	  foreach($result as $data){ 
				if ($data->office_name == 'JLC'){
					$office_name = 'OFFICE OF THE JOINT LABOUR COMMISSIONER';
				}elseif ($data->office_name == 'DLC'){
					$office_name = 'OFFICE OF THE DEPUTY LABOUR COMMISSIONER';
				}elseif($data->office_name == 'ALC'){
					$office_name = 'OFFICE OF THE ASSISTANT LABOUR COMMISSIONER';
				}elseif($data->office_name == 'KLC'){
					$office_name = 'OFFICE OF THE LABOUR COMMISSIONER EL & MW SECTION';
				}
				
				$email1 = str_replace("@", "[at]", $data->office_email_address);
				$office_email_address = str_replace(".", "[dot]", $email1);
			$rows[] = array(			
						array('data' => strtoupper($data->district_name)),
						array('data' => $office_name.', '.strtoupper($data->sub_div_name)),						 
						array('data' => $data->officenumber),
						array('data' => $office_email_address),
						array('data' => l('<i class="fa fa-eye fa-lg" aria-hidden="true">', 'rlo-details/'.str_replace(' ', '-', strtolower($data->sub_div_name)).'/'.encryption_decryption_fun('encrypt', $data->sub_div_code), array('html' => TRUE,'attributes'=> array('target'=>'_blank'))))
					);	
		  }
  	  }
	  
	  $output = array( 'header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('view-act-rules-table')), 'empty' => t("No data found!"));	 	 					
	  $output = 	theme('datatable', $output);
	 
	  // $mor_info = l('<i class="fa fa-phone" aria-hidden="true"></i> <strong>More Information</strong>', 'others-contact-information', array('html' => TRUE));
	 
	  echo '<div class="feedback-scroll">'.$output.'</div>'.$mor_info;
?>






<?php
	$query = db_select('map_table', 'm');
	$query->leftJoin('map_block_table', 'b', 'm.dis_code=b.bdo_dis_code');
  	$query->fields('m', array('rlo_name', 'telephone_no', 'designation', 'mobile_no', 'email','address','latitude','longitude')); 
	$query->fields('b', array('bdo_offices', 'bdo_ofc_address', 'bdo_latitude', 'bdo_longitude')); 
	
	//$query->distinct('m.rlo_name');
	$query->condition('m.status','1','=');
  	$qry_data = $query->execute();
	$result = $qry_data->fetchAll();
	//print_r($result);
	//{ "DisplayText": "adcv", "ADDRESS": "Jamiya Nagar Kovaipudur Coimbatore-641042", "LatitudeLongitude": "10.9435131,76.9383790", "MarkerId": "Customer" }
	$addrStr = "";
	foreach($result as $val){
		$addrStr.= '{ "DisplayText": "Office Name - '.$val->rlo_name.'<br>'.$val->address.'<br> Phone No - '.$val->telephone_no.'<br> Email - '.$val->email.'", "ADDRESS": "'.$val->address.'", "DisplayOffice": "Office Name - '.$val->bdo_offices.'<br>'.$val->bdo_ofc_address.'", "LatLongBlock": "'.$val->bdo_latitude.','.$val->bdo_longitude.'", "Designation": "'.$val->designation.'", "LatitudeLongitude": "'.$val->latitude.','.$val->longitude.'", "MarkerId": "Customer" }';
		$addrStr.= ',';	
	}
	$addrStr = rtrim($addrStr,',');
	//$addrStr = '{ "DisplayText": "adcv", "ADDRESS": "Jamiya Nagar Kovaipudur Coimbatore-641042", "LatitudeLongitude": "10.9435131,76.9383790", "MarkerId": "Customer" },{ "DisplayText": "New Secratary Building", "ADDRESS": "Coimbatore-641042", "LatitudeLongitude": "22.569978,88.344078", "MarkerId": "Customer"}';
?>


<script src="https://maps.googleapis.com/maps/api/js?key=AIzaSyDvk5Z488EnNtRYH9uWDF37-79MIg6SlgY" type="text/javascript"></script>
    <script type="text/javascript">

        var map;
        var geocoder;
        var marker;
        var people = new Array();
        var latlng;
        var infowindow;
        jQuery(document).ready(function() {
			//alert(base_url);
            ViewCustInGoogleMap();
			
        });
		

        function ViewCustInGoogleMap() {

            var mapOptions = {
                center: new google.maps.LatLng(22.569978, 88.344078),   // Coimbatore = (11.0168445, 76.9558321)
                zoom: 9,
                mapTypeId: google.maps.MapTypeId.ROADMAP
            };
            map = new google.maps.Map(document.getElementById("map-canvas"), mapOptions);

            // Get data from database. It should be like below format or you can alter it.
			var data = '[<?php echo $addrStr;?>]';

            //var data = '[{ "DisplayText": "adcv", "ADDRESS": "Jamiya Nagar Kovaipudur Coimbatore-641042", "LatitudeLongitude": "10.9435131,76.9383790", "MarkerId": "Customer" },{ "DisplayText": "New Secratary Building", "ADDRESS": "Coimbatore-641042", "LatitudeLongitude": "22.569978,88.344078", "MarkerId": "Customer"}]';

            people = JSON.parse(data); 

            for (var i = 0; i < people.length; i++) {
                setMarker(people[i]);
            }

        }

        function setMarker(people) {
            geocoder = new google.maps.Geocoder();
            infowindow = new google.maps.InfoWindow();
            if ((people["LatitudeLongitude"] == null) || (people["LatitudeLongitude"] == 'null') || (people["LatitudeLongitude"] == '')) {
                geocoder.geocode({ 'address': people["Address"] }, function(results, status) {
                    if (status == google.maps.GeocoderStatus.OK) {
                        latlng = new google.maps.LatLng(results[0].geometry.location.lat(), results[0].geometry.location.lng());
						
                        marker = new google.maps.Marker({
                            position: latlng,
                            map: map,
                            draggable: false,
                            html: people["DisplayText"],
                            icon: "images/marker/" + people["MarkerId"] + ".png"
                        });
                        //marker.setPosition(latlng);
                        //map.setCenter(latlng);
                        google.maps.event.addListener(marker, 'click', function(event) {
                            infowindow.setContent(this.html);
                            infowindow.setPosition(event.latLng);
                            infowindow.open(map, this);
                        });
                    }
                    else {
                        alert(people["DisplayText"] + " -- " + people["Address"] + ". This address couldn't be found");
                    }
                });
            }
            else {	
			
				
				var LatLongBlock = people["LatLongBlock"];
			    var latlngStrBlock = people["LatLongBlock"].split(",");
                var latBlock = parseFloat(latlngStrBlock[0]);
                var lngBlock = parseFloat(latlngStrBlock[1]);
                latlngBlock = new google.maps.LatLng(latBlock, lngBlock);
					
				var designation = people["Designation"];
                var latlngStr = people["LatitudeLongitude"].split(",");
                var lat = parseFloat(latlngStr[0]);
                var lng = parseFloat(latlngStr[1]);
                latlng = new google.maps.LatLng(lat, lng);
				var base_url = window.location.origin;
				if(LatLongBlock.trim()!=""){
                marker = new google.maps.Marker({
                    position: latlngBlock,
                    map: map,
                    draggable: false,               // cant drag it
                    html: people["DisplayOffice"],    // Content display on marker click
                    //icon: "images/marker.png"       // Give ur own image
					icon: base_url+"/sites/all/themes/jackson/images/yellow-dot.png"
                });
                //marker.setPosition(latlng);
                //map.setCenter(latlng);
                google.maps.event.addListener(marker, 'click', function(event) {
                    infowindow.setContent(this.html);
                    infowindow.setPosition(event.latLng);
                    infowindow.open(map, this);
                });
				}
				if(designation.trim()=="Assistant Labour Commissioner"){
                marker = new google.maps.Marker({
                    position: latlng,
                    map: map,
                    draggable: false,               // cant drag it
                    html: people["DisplayText"],    // Content display on marker click
                    //icon: "images/marker.png"       // Give ur own image
					icon: base_url+"/sites/all/themes/jackson/images/purple-dot.png"
                });
                //marker.setPosition(latlng);
                //map.setCenter(latlng);
                google.maps.event.addListener(marker, 'click', function(event) {
                    infowindow.setContent(this.html);
                    infowindow.setPosition(event.latLng);
                    infowindow.open(map, this);
                });
				}else if(designation.trim()=="Deputy Labour Commissioner"){
					marker = new google.maps.Marker({
                    position: latlng,
                    map: map,
                    draggable: false,               // cant drag it
                    html: people["DisplayText"],    // Content display on marker click
                    //icon: "images/marker.png"       // Give ur own image
					icon: base_url+"/sites/all/themes/jackson/images/blue-dot.png"
                });
                //marker.setPosition(latlng);
                //map.setCenter(latlng);
                google.maps.event.addListener(marker, 'click', function(event) {
                    infowindow.setContent(this.html);
                    infowindow.setPosition(event.latLng);
                    infowindow.open(map, this);
                });
				}else{
					marker = new google.maps.Marker({
                    position: latlng,
                    map: map,
                    draggable: false,               // cant drag it
                    html: people["DisplayText"],    // Content display on marker click
                    //icon: "images/marker.png"       // Give ur own image
					icon: base_url+"/sites/all/themes/jackson/images/map-red.png"
                });
                //marker.setPosition(latlng);
                //map.setCenter(latlng);
                google.maps.event.addListener(marker, 'click', function(event) {
                    infowindow.setContent(this.html);
                    infowindow.setPosition(event.latLng);
                    infowindow.open(map, this);
                });
				}
            }
        }


    </script>

     <!--<div id="map-canvas" style="width:100%; height: 373px; border:5px solid grey;"></div>-->     
  <?php  
  
  
    // Initialising output
   $output = '';
 
  // Table header
 
  
  $header = array(
    array('data' => 'RLO NAME','width' => '25%'),
	array('data' => 'Designation'),
    array('data' => 'Phone'),
	array('data' => 'Officer Name'),
	array('data' => 'Email'),
	array('data' => 'Action'),
  ); 
    
	$query = db_select('map_table', 'm');
  	$query->fields('m', array('rlo_name', 'telephone_no', 'officer_name', 'designation','mobile_no', 'email','address','latitude','longitude'));
	$query->condition('m.is_active', 1);
	$query->orderBy('priority_col', 'ASC');
	$result_map_table = $query->execute(); 
	
	// $result = $result_map_table->fetchAll(); print_r($result); die;
	
	// while($result = $result_map_table->fetchObject()){	
	foreach($result_map_table as $result){	
		$rlo_name = str_replace(' ', '-', strtolower($result->rlo_name));
		$rlo_name = str_replace(',', '-', $rlo_name);
		$rlo_name = str_replace('&', 'and', $rlo_name);		
		$rlo_name = str_replace('(', '-', $rlo_name);
		$rlo_name = str_replace(')', '-', $rlo_name);
		$rlo_name = str_replace('--', '-', $rlo_name);
		
		$email1 = str_replace("@", "[at]", $result->email);
		$email = str_replace(".", "[dot]", $email1);
    	// Adding the rows	
    	 $map_rows[] = array(
				'rlo_name' 		=> $result->rlo_name, 
				'designation' 	=> $result->designation, 
				'telephone_no' 	=> $result->telephone_no,
				'officer_name' 	=> $result->officer_name,
				'email' 		=> $email,
				'view_details' 	=> l('<i class="fa fa-eye fa-lg" aria-hidden="true">', 'office-details/'.$rlo_name.'/'.encryption_decryption_fun('encrypt', $result->srlno), array('html' => TRUE))				
		);
	
  	}	
	
	$db_query = db_select('sub_division','sbdv');
	$db_query ->leftJoin('l_customuser_relation_address','lcra', 'sbdv.sub_div_code=lcra.sub_div_code');
	$db_query ->leftJoin('l_custom_user_detail','lcud', 'lcud.usr_id=lcra.user_id');
	$db_query ->leftJoin('users','u', 'u.uid = lcud.usr_id');
	$db_query ->fields('lcud', array('fullname', 'officenumber','degisnation','mobile'));
	$db_query->fields('sbdv', array('sub_div_name', 'sub_div_code')); 
	$db_query->fields('u', array('mail'));  
	$db_query->condition('lcud.usr_rid', 4);
	
	$result = $db_query->execute();
	
	  
  	$rows = array();
	  // Looping for filling the table rows
	while($data = $result->fetchObject()){ 
			$email1 = str_replace("@", "[at]", $data->mail);
			$email = str_replace(".", "[dot]", $email1);
			$rlo_array[] = array(
				'rlo_name' 		=> 'Regional Labour Office <br/>'.$data->sub_div_name,
				'sub_div_name' 	=> $data->sub_div_name, 
				'sub_div_code' 	=> $data->sub_div_code, 
				'designation' 	=> $data->degisnation, 
				'telephone_no' 	=> $data->officenumber,
				'officer_name' 	=> $data->fullname,
				'email' 		=> $email,//str_replace("@", "[at]", $data->mail),
				'view_details' 	=> l('<i class="fa fa-eye fa-lg" aria-hidden="true">', 'rlo-details/'.str_replace(' ', '-', strtolower($data->sub_div_name)).'/'.encryption_decryption_fun('encrypt', $data->sub_div_code), array('html' => TRUE))				 
			);	
  	 }
	 
	 $data = array_merge($map_rows, $rlo_array);
	  
	  for($i = 0; $i < count($data); $i++){
		  	$email1 = str_replace("@", "[at]", $data[$i]['email']);
			$email = str_replace(".", "[dot]", $email1);
			$rows[] = array(			
				array('data' => $data[$i]['rlo_name']), 
				array('data' => $data[$i]['designation']), 
				array('data' => $data[$i]['telephone_no']),
				array('data' => $data[$i]['officer_name']),
				array('data' => $email),
				array('data' => $data[$i]['view_details'])
			);	
  	  } 
	 	 
	 $output = array( 'header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('view-act-rules-table')), 'empty' => t("No data found!"));	 	 					
	 $output = 	theme('datatable', $output);
	 
	  $mor_info = l('<i class="fa fa-phone" aria-hidden="true"></i><strong>More Information</strong>', 'others-contact-information', array('html' => TRUE));
	 // Returning the output
    // echo $output_map.$output.$mor_info;
?>


<style>
a{cursor:pointer;}
</style>