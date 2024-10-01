<?php
 //print_r($page);
 
 //die('ok');
?>
<?php
       //include('common.php');
?>
<?php
	$query = db_select('map_table', 'm');
  	$query->fields('m', array('rlo_name', 'telephone_no', 'mobile_no', 'email','address','latitude','longitude')); 
	$query->distinct('m.rlo_name');
  	$qry_data = $query->execute();
	$result = $qry_data->fetchAll();
	//print_r($result);
	//{ "DisplayText": "adcv", "ADDRESS": "Jamiya Nagar Kovaipudur Coimbatore-641042", "LatitudeLongitude": "10.9435131,76.9383790", "MarkerId": "Customer" }
	$addrStr = "";
	foreach($result as $val){
		$addrStr.= '{ "DisplayText": "Office Name - '.$val->rlo_name.'<br> Phone No - '.$val->telephone_no.'<br> Email - '.$val->email.'<br>'.$val->address.'", "ADDRESS": "'.$val->address.'", "LatitudeLongitude": "'.$val->latitude.','.$val->longitude.'", "MarkerId": "Customer" }';
		$addrStr.= ',';	
	}
	$addrStr = rtrim($addrStr,',');
	//$addrStr = '{ "DisplayText": "adcv", "ADDRESS": "Jamiya Nagar Kovaipudur Coimbatore-641042", "LatitudeLongitude": "10.9435131,76.9383790", "MarkerId": "Customer" },{ "DisplayText": "New Secratary Building", "ADDRESS": "Coimbatore-641042", "LatitudeLongitude": "22.569978,88.344078", "MarkerId": "Customer"}';
?>



  <div id="page-wrapper">
    <div id="page">
  
      <div id="header"><div class="container section header clearfix">
  
        <?php if ($logo): ?>
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo" class="logo">
            <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
          </a>
          
           <!-- here the image for the support here-->
            
             <?php print render ($page['customer_support']); ?>
             <?php endif; ?>
               <div class="ad_logo">
                 <img src="<?php echo base_path(). path_to_theme()?>/images/nationalportalIndia.jpg" alt="" class="img_box" />
               </div>
                <!-- here the image for the support ends here-->
                
                  <!--hit counter div starts here-->
                    <p class="hitcounter">Site Counter:<?php if(isset($page['header']['counter_counter']['#markup']))echo $page['header']['counter_counter']['#markup'];?></p>
                  <!--hit counter div ends here-->
          
        
         
        <?php /*?><div id="log-in-out">
        <?php if(!$logged_in) print l('log in', 'user/login'); ?>
        <?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary-menu', 'class' => array('links', 'inline', 'clearfix')), 'heading' => t('Secondary menu'))); ?>
        </div<?php */?>
  
        <?php if ($site_name || $site_slogan): ?>
          <div id="name-and-slogan">
            <?php if ($site_name): ?>
              <?php if ($title): ?>
                <div id="site-name"><strong>
                  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
                </strong></div>
              <?php else: /* Use h1 when the content title is empty */ ?>
                <h1 id="site-name">
                  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
                </h1>
              <?php endif; ?>
            <?php endif; ?>
  
            <?php if ($site_slogan): ?>
              <div id="site-slogan"><?php print $site_slogan; ?></div>
            <?php endif; ?>
          </div> <!-- /#name-and-slogan -->
        <?php endif; ?>
          <?php print render($page['header']); ?>

      </div><!-- /.section .header --> 
    </div> <!-- /#header -->
    
    
  
      <?php if($main_menu || $page['superfish_menu'] ): ?>
        <div id="navigation">
          <div class="container navigation section">
          <?php print render($page['superfish_menu']); ?>
            <?php if($page['superfish_menu']) {
             // print render($page['superfish_menu']);
            } else {
			//  print_r($main_menu);	
              //print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => 'main-menu', 'class' => array('links', 'clearfix'))));        
            }
            ?>
          </div><!-- /.section .navigation -->
        </div> <!-- /#navigation -->
      <?php endif; ?>
  
      <?php /*?><div id="banner-wrap" class="slider-content <?php if(!$page['banner']) print 'empty' ?>">
        <?php if ($page['banner']): ?>
             <div id="banner" class="clearfix">
               <div class="container region">
                 <?php print render ($page['banner']); ?>
               </div>
             </div>
        <?php endif; ?>
      </div><?php */?>
      
     

  
      <div id="main-wrapper">
      <?php print $messages; ?>
    
        <?php if ($page['preface_one']