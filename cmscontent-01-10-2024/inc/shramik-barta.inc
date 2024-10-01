<?php
function shramik_mela(){
	$output = '<div class="carousel slide" id="myCarousel">
        			<div class="carousel-inner">
            			<div class="item active sb-slider">
                    		<ul class="thumbnails">';
     
	 $output .= '<li class="col-sm-3">
	 				<div class="white">
						<div class="thumbnail">'.l('<img src="'.base_path().'sites/default/files/books-and-news/sramik-barta-august-sept-2017.jpg">',base_path().'sites/default/files/books-and-news/sramik-barta-august-sept-2017.pdf', array('html' => TRUE,'attributes'=>array('target' => '_blank'))).'</div>
						<div class="caption">
							<h4><strong>শ্রমিক বার্তা</strong></h4>
							<p><font color="#0099FF">অগাস্ট - সেপ্টেম্বর ২০১৭.</font> <font color="#FF0033">চতুর্থ বর্ষ.</font> <font color="#0099FF">প্রথম সংখ্যা.</font></p>
							'.l('View',base_path().'sites/default/files/books-and-news/sramik-barta-august-sept-2017.pdf', array('html' => TRUE, 'attributes'=>array('target' => '_blank', 'class' => 'btn'))).'
						</div>
					</div>
                  </li>';
				  
	$output .= '<li class="col-sm-3">
	 				<div class="white">
						<div class="thumbnail">'.l('<img src="'.base_path().'sites/default/files/books-and-news/sramik-barta-april-may-2017.jpg">',base_path().'sites/default/files/books-and-news/sramik-barta-april-may-2017.pdf', array('html' => TRUE,'attributes'=>array('target' => '_blank'))).'</div>
						<div class="caption">
							<h4><strong>শ্রমিক বার্তা</strong></h4>
							<p><font color="#0099FF">এপ্রিল-মে ২০১৭.</font> <font color="#FF0033">তৃতীয় বর্ষ.</font> <font color="#0099FF">চতুর্থ সংখ্যা.</font></p>
							'.l('View',base_path().'sites/default/files/books-and-news/sramik-barta-april-may-2017.pdf', array('html' => TRUE, 'attributes'=>array('target' => '_blank', 'class' => 'btn'))).'
						</div>
					</div>
                  </li>';
				  
	$output .= '<li class="col-sm-3">
	 				<div class="white">
						<div class="thumbnail">'.l('<img src="'.base_path().'sites/default/files/books-and-news/sramik-barta-december-2015.jpg">',base_path().'sites/default/files/books-and-news/sramik-barta-december-2015.pdf', array('html' => TRUE,'attributes'=>array('target' => '_blank'))).'</div>
						<div class="caption">
							<h4><strong>শ্রমিক বার্তা</strong></h4>
							<p><font color="#0099FF">ডিসেম্বর ২০১৫.</font> <font color="#FF0033">দ্বিতীয় বর্ষ.</font> <font color="#0099FF">চতুর্থ সংখ্যা.</font></p>
							'.l('View',base_path().'sites/default/files/books-and-news/sramik-barta-december-2015.pdf', array('html' => TRUE, 'attributes'=>array('target' => '_blank', 'class' => 'btn'))).'
						</div>
					</div>
                  </li>';
				  		  
	 $output .= '<li class="col-sm-3">
	 				<div class="white">
						<div class="thumbnail">'.l('<img src="'.base_path().'sites/default/files/books-and-news/sramik-barta-august-2015.jpg">',base_path().'sites/default/files/books-and-news/sramik-barta-august-2015.pdf', array('html' => TRUE,'attributes'=>array('target' => '_blank'))).'</div>
						<div class="caption">
							<h4><strong>শ্রমিক বার্তা</strong></h4>
							<p><font color="#0099FF">অগাস্ট ২০১৫.</font> <font color="#FF0033">দ্বিতীয় বর্ষ.</font> <font color="#0099FF">তৃতীয় সংখ্যা.</font></p>
							'.l('View',base_path().'sites/default/files/books-and-news/sramik-barta-august-2015.pdf', array('html' => TRUE, 'attributes' => array('target' => '_blank', 'class' => 'btn'))).'
						</div>
					</div>
                  </li>';
				  
	 $output .= '<li class="col-sm-3">
	 				<div class="white">
						<div class="thumbnail">'.l('<img src="'.base_path().'sites/default/files/books-and-news/sramik-barta-april-2015.jpg">',base_path().'sites/default/files/books-and-news/sramik-barta-april-2015.pdf', array('html' => TRUE,'attributes'=>array('target' => '_blank'))).'</div>
						<div class="caption">
							<h4><strong>শ্রমিক বার্তা</strong></h4>
							<p><font color="#0099FF">এপ্রিল ২০১৫.</font> <font color="#FF0033">দ্বিতীয় বর্ষ.</font> <font color="#0099FF">দ্বিতীয় সংখ্যা.</font><br>&nbsp;</p>
							'.l('View',base_path().'sites/default/files/books-and-news/sramik-barta-april-2015.pdf', array('html' => TRUE, 'attributes'=>array('target' => '_blank', 'class' => 'btn'))).'
						</div>
					</div>
                  </li>';
					  
	 $output .= '<li class="col-sm-3">
	 				<div class="white">
						<div class="thumbnail">'.l('<img src="'.base_path().'sites/default/files/books-and-news/samajik-surakha-yojna-2017.jpg">',base_path().'sites/default/files/books-and-news/asanghatito-sramik-big -book-2017.pdf', array('html' => TRUE,'attributes'=>array('target' => '_blank'))).'</div>						
					</div>
                  </li>';	  
	    
	$output .= '</ul></div></div></div>';				
      
	return $output;
}

function shramik_mela_venues(){
	global $base_root, $base_path, $user;
	$path 									= 	drupal_get_path('module', 'applicant_forms');
	require_once("./$path/Classes/PHPExcel.php");
	require_once("./$path/Classes/PHPExcel/IOFactory.php");	
		
	$inputFileName = $_SERVER['DOCUMENT_ROOT']."/".variable_get('file_public_path', conf_path())."/upload/officers_contact/venues_of_shramik_mela_with_time.xlsx";	
	
	try {
		$inputFileType = PHPExcel_IOFactory::identify($inputFileName);
		$objReader = PHPExcel_IOFactory::createReader($inputFileType);
		$objPHPExcel = $objReader->load($inputFileName);
	} catch(Exception $e) {
		die('Error loading file "'.pathinfo($inputFileName,PATHINFO_BASENAME).'": '.$e->getMessage());
	}
	
	//  Get worksheet dimensions
	$sheet = $objPHPExcel->getSheet(0); 
	$highestRow = $sheet->getHighestRow(); 
	$highestColumn = $sheet->getHighestColumn();
	
	//  Loop through each row of the worksheet in turn
	for ($row = 1; $row <= $highestRow; $row++){ 
		//  Read a row of data into an array
		$rowData = $sheet->rangeToArray('A2:'. $highestColumn . $row, NULL, TRUE, FALSE);		
	}
	
	$data = array_map('array_filter', $rowData);
	$data = array_filter($data);
	
	
	
	$slno = 0;
	$output = '';
	
	for($count=0; $count<= 66; $count++){
		$val = $data[$count];
		$slno++;
		$output .='<div class="col-lg-2 col-md-6 col-sm-6">
					<div class="eservice-box" style="min-height:160px;background: #e5e5e5 url(sites/all/themes/lcTheme/images/cal-mela2-2020.jpg) no-repeat left top;border:1px solid #d1d1d1;-webkit-border-radius:10px;border-radius:10px;margin-bottom:25px;">
						<div class="eservice-box-logo#" style="margin-top:0px;background:#8eb327;-webkit-border-radius: 6px 6px 0 0;border-radius:6px 6px 0 0;"><h1 style="color:#fff;padding:4px 0;font-weight:400;">'.$val[2].'</h1><!--<img src="sites/all/themes/lcTheme/images/logo-regs.png" alt="Shramik Mela" title="'.$val[2].'">--></div>
						<h1>'.$val[1].'</h1>										
						<p style="color:#333;text-shadow:none;padding:0 6px;text-align:center;font-weight:700;font-size:13px;">'.$val[3].'</p>						
					</div>				
				</div>';
	}   
	
	return '<div class="row"><div class="col-md-12"><div class="box box-primary"><div class="box-body" style="background:#f0efe5; overflow:hidden; padding-top:15px;padding-bottom:15px;">'.$output.'</div></div></div></div>';
	
}

?>


