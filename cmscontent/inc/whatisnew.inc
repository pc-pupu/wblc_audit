<?php
drupal_add_css(drupal_get_path('module', 'cmscontent') . '/css/cms.css');


function add_whatisnew($form, &$form_states)
{
    global $base_root, $base_path, $user;
    
    $form['circularaddrfields']['cont_head'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="row"><div class="col-md-12"><div class="box box-primary"><div class="box-body">'
        
    );
    
    
    
    
    $form['circularaddrfields']['ltitle'] = array(
        
        '#title' => 'Link title',
        '#prefix' => '<div class="form-custom col-md-6"><label class="input">',
        '#suffix' => '</label></div>',
        '#type' => 'textfield',
        '#required' => TRUE,
        '#element_validate' => array(
            'lcontent_textfield_validate'
        ),
        '#attributes' => array(
            'class' => array(
                'form-control'
            )
        )
        
        
    );
    
    
    
    $form['circularaddrfields']['ctitle'] = array(
        
        '#title' => 'Content title',
        
        '#prefix' => '<div class="form-custom col-md-6"><label class="input">',
        '#suffix' => '</label></div>',
        '#type' => 'textfield',
        '#required' => TRUE,
        '#element_validate' => array(
            'lcontent_textfield_validate'
        ),
        '#attributes' => array(
            'class' => array(
                'form-control'
            )
        )
        
    );
    
    
    
    
    $form['circularaddrfields']['add_content_desc'] = array(        
			'#title' => 'Legal Basis - Does it have a basis in law/act/policy',
			'#cols' => 20,
			'#rows' => 2,
			'#type' => 'textarea',        
			'#format' => 'full_html',
			'#base_type' => 'textarea',        
			'#attributes' => array('class' => array('form-control')),
			'#prefix' => '<div class="form-custom col-md-12">',
			'#suffix' => '</div>'        
          );
    $form['circularaddrfields']['achieve_objectives'] = array(        
			'#title' => 'Its necessity - Does the license help government achieve its objectives',
			'#cols' => 20,
			'#rows' => 2,
			'#type' => 'textarea',        
			'#format' => 'full_html',
			'#base_type' => 'textarea',        
			'#attributes' => array('class' => array('form-control')),
			'#prefix' => '<div class="form-custom col-md-12">',
			'#suffix' => '</div>'        
          );
	$form['circularaddrfields']['business_friendly'] = array(        
			'#title' => 'Its business-friendly - Does it impose minimum burden on businesses to achieve the government’s objectives',
			'#cols' => 20,
			'#rows' => 2,
			'#type' => 'textarea',        
			'#format' => 'full_html',
			'#base_type' => 'textarea',        
			'#attributes' => array('class' => array('form-control')),
			'#prefix' => '<div class="form-custom col-md-12">',
			'#suffix' => '</div>'        
          );    
    
    $form['circularaddrfields']['meta_title'] = array(
        
        '#title' => 'Meta keyword',
        '#type' => 'textfield',
        '#element_validate' => array(
            'lcontent_textfield_validate'
        ),
        '#prefix' => '<div class="form-custom col-md-12"><label class="input">',
        '#suffix' => '</label></div>'
    );
    
    
    
    
    $form['circularaddrfields']['meta_content_desc'] = array(
        
        '#title' => 'Meta Description',
        '#cols' => 20,
        '#rows' => 2,
        '#type' => 'textarea',
        '#element_validate' => array(
            'lcontent_textfield_validate'
        ),
        '#prefix' => '<div class="form-custom col-md-12"><label class="input">',
        '#suffix' => '</label></div>',
        '#attributes' => array(
            'class' => array(
                'form-control'
            )
        )
    );
    
    
    
    $form['circularaddrfields']['order_no'] = array(
        
        '#title' => 'Order No',
        '#size' => t('7'),
        '#type' => 'textfield',
        '#maxlength' => 4,
        '#element_validate' => array(
            'ORDER_MANDATORY_NUMERIC_CHECKING_validate'
        ),
        '#attributes' => array(
            'title' => 'order no  should be numeric '
        ),
        '#attributes' => array(
            'class' => array(
                'form-control'
            )
        ),
        '#prefix' => '<div class="form-custom col-md-4"><label class="input">',
        '#suffix' => '</label></div>',
        '#required' => TRUE
    );
    
    $form['circularaddrfields']['is_new'] = array(
        '#type' => 'checkbox',
        '#title' => 'Is New',
        '#prefix' => '<div class="form-custom col-md-3 cus-checkbox-pad custom-chk-top-pad"><label class="input">',
        '#suffix' => '</label></div>',
        '#attributes' => array(
            'class' => array(
                'form-control'
            )
        )
        
    );
    
    $form['circularaddrfields']['cont_head1'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="col-md-12">'
    );
    
    $form['circularaddrfields']['add_content_pdf'] = array(
        
        '#type' => 'managed_file',
        '#title' => t(' Upload an Content  file (JPG,PDF only)'),
        '#process' => array(
            'import_my_file_element_process'
        ),
        '#attributes' => array(
            'class' => array(
                ''
            )
        ),
        //'#upload_validators'      => array('file_validate_extensions' => array('pdf')),
        '#upload_location' => 'public://contentpdf/',
        '#process' => array(
            'import_my_file_element_process'
        ),
        '#attributes' => array(
            'id' => '',
            'autocomplete' => 'off'
        ),
        '#required' => TRUE,
        '#file_check' => TRUE
        
    );
    
    $form['circularaddrfields']['cont_footer1'] = array(
        '#type' => 'markup',
        '#markup' => '</div></div></div></div>'
    );
    
    $form['circularaddrfields']['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Save',
        '#attributes' => array('class' => array('btn btn-primary pull-left')),
        '#prefix' => '<div class="col-md-6"><div class="col-md-6">',
        '#suffix' => '</div></div></div>'
     );
    
    /*$form['circularaddrfields']['cont_footer'] = array(
        '#type' => 'markup',
        '#markup' => ''
    );*/
    
    return $form;
    
    
}

function add_whatisnew_submit($form_id, $form_states)
{
    $val = $form_states['values'];
    //echo "<pre>";print_r($val);exit;
    
    if (isset($val['submit'])) {
        
        $ctitle            = check_plain($val['ctitle']);
        //$add_content_desc=trim(check_plain(decode_entities($form_states['values']['add_content_desc']['value'])));
        $add_content_desc  = trim($val['add_content_desc']);
        $meta_title        = check_plain($val['meta_title']);
        $meta_content_desc = check_plain($val['meta_content_desc']);
        $content_typ       = trim('whatisnew');
        $order_no          = trim($val['order_no']);
        $is_new            = trim($val['is_new']);
        if (isset($is_new)) {
            $is_new = 'Y';
            
        } else {
            $is_new = 'N';
        }
        $ltitle  = check_plain(trim($val['ltitle']));
        $message = "";
        
        $txn = db_transaction();
        
        
        global $base_root, $base_path, $full_base;
        
        $arr           = array();
        $uploaded_file = file_load($val['add_content_pdf']);
        $file_name     = trim($_FILES['files']['name']['add_content_pdf']);
        
        
        
        
        try {

        	if($cfid && $file_name){
			   	$query_ent2=db_query('select  a.fid,filename from l_cms_content a , file_managed b where a.fid=b.fid and  a.fid = :cid', array(':cid'=>$cfid));
				
				 if($query_ent2->rowCount()> 0) 
			          { 
						foreach ($query_ent2 as $obj) {
					
							$fid=$obj->fid;
							$docfilename=$obj->filename;
							
						    } 
						   
						    $doc_deleted = db_delete('file_managed')
			               ->condition('fid',trim($cfid),'=')
			               ->execute();
						   
						   $new_path=$dest_dir.$docfilename;
					        unlink($new_path);
					  }
			   }
            
            if ($file_name) {
                $_FILES['files']['name']['add_content_pdf'] = time() . $file_name;
                
                if ($uploaded_file != "") {
                    $arr                   = explode("/", $uploaded_file->uri);
                    $uploaded_file->status = FILE_STATUS_PERMANENT;
                    file_save($uploaded_file);
                    $fid       = $uploaded_file->fid;
                    $fname     = $uploaded_file->filename;
                    $file_name = $arr[3];
                }
                
                
                
            }
            
            $result_contentid = db_query("select  max(content_id) content_id from l_cms_content");
            
            
            $result   = $result_contentid->fetchAssoc();
            $ownermax = $result['content_id'];
            
            if ($result_contentid->rowCount() == 0) {
                $count_id = 1;
            } else {
                $count_id = $ownermax + 1;
                
            }
            if (!$fid) {
                $fid = 0;
            }
            
            
            $fieldsE = array(
                'content_title' => $ctitle,
                'content_desc' => $add_content_desc,
                'meta_title' => $meta_title,
                'meta_desc' => $meta_content_desc,
                'content_dt' => date("Y-m-d"),
                'fid' => $fid,
                'content_id' => $count_id,
                'content_typ' => $content_typ,
                'is_active' => 'Y',
                'ord' => $order_no,
                'link_title' => $ltitle,
                'is_new' => $is_new,
                'acts_and_rules_pdf' => $file_name,
				'achieve_objectives' => $val['achieve_objectives'],
				'business_friendly' => $val['business_friendly']				
           	);
            
            db_insert('l_cms_content')->fields($fieldsE)->execute();
            
            $message = 'Content Add Successfully';
            
            
            
        }
        catch (Exception $e) {
            $txn->rollback();
            watchdog_exception('type', $e);
            $message = 'content Not Added ';
        }
        
        
        $full_base = $base_root . $base_path . 'cmscontent/what_is_new_list';
        
        drupal_set_message(t($message));
        
        
        
        drupal_goto($full_base);
        
    }
}


/*==============  List Start ============*/

function whatisnew_list()
{
    global $base_root, $base_path, $user;
    drupal_add_css(drupal_get_path('module', 'applicant_forms') . '/css/clra_applications.css');
    
    $sort               = 'DESC';
    $order              = 'content_id';
    $cms_circular_query = db_select('l_cms_content', 't');
    $cms_circular_query->fields('t', array(
        'content_id',
        'content_title',
        'content_dt',
        'content_desc',
        'content_typ',
        'ord',
        'is_active',
        'acts_and_rules_pdf',
		
    ));
    $cms_circular_query->condition('t.content_typ', 'whatisnew', '=');
    $cms_circular_query->orderBy($order, $sort);
    $cms_circular_query_result = $cms_circular_query->execute();
    
    
	
    $rows   = array();
    $header = array();
    
    $c_type_value = array(
        'actrule' => t('ACTS & RULES'),
        'eodb' => t('EODB'),
        'tender' => t('Tender'),
        'whatisnew' => t('What is new'),
        'notification' => t('Notification'),
        'publication' => t('PUBLICATION'),
        'otheract' => t('Other Activites')
    );
    $header       = array(
        array(
            'data' => 'Sl.No'
        ),
        array(
            'data' => 'Title'
        ),
        array(
            'data' => 'Content Type',
            'width' => '10%'
        ),
        array(
            'data' => 'Order By',
            'width' => '8%'
        ),
        array(
            'data' => 'Status'
        ),
        array(
            'data' => 'Action',
			'width' => '10%'
        )
    );
    $i            = 0;
    foreach ($cms_circular_query_result as $data) {
        $i++;
        global $base_root, $base_path;
        $aa     = $base_root . $base_path . 'cmscontent/' . encryption_decryption_fun('encrypt', $data->content_id) . '/what_is_new_edit';
        $link_s = l(t('<span class="fn_black"><i class="fa fa-edit"></i></span>'), $aa, array(
            'html' => TRUE,
            'attributes' => array(
                'title' => 'Click to Edit Details'
            )
        ));
        
        $aalink   = '';
        $link_k   = '';
        $order_by = $data->ord;
        
        if ($data->is_active == 'Y') {
            
            $aalink = $base_root . $base_path . 'cmscontent/' . $data->content_id . '/what_is_new_active';
            $link_k = l('<span class="fn_green">&nbsp;<i class="fa fa-check-circle"></i>&nbsp;</span>', $aalink, array(
                'html' => TRUE,
                'attributes' => array(
                    'title' => 'Click to Inactive'
                )
            ));
        } elseif ($data->is_active == 'N') {
            
            $aalink = $base_root . $base_path . 'cmscontent/' . $data->content_id . '/what_is_new_active';
            $link_k = l('<span class="fn_red">&nbsp;<i class="fa fa-times-circle"></i>&nbsp;</span>', $aalink, array(
                'html' => TRUE,
                'attributes' => array(
                    'title' => 'Click to Active'
                )
            ));
        }
        $dwn = '';
        if ($data->acts_and_rules_pdf != "") {
            $bb  = $GLOBALS['base_url'] . '/sites/default/files/contentpdf/' . $data->acts_and_rules_pdf;
            $dwn = '&nbsp;&nbsp;' . $link_s1 = l(t('<span class="fn_black"><i class="fa fa-download"></i></span>'), $bb, array(
                'html' => TRUE,
                'attributes' => array(
                    'title' => 'Click to Print/Download',
                    'target' => '_blank'
                )
            ));
        }
        
		
	$cms_feedback_query = db_select('l_cms_feedback', 'l');
    $cms_feedback_query->fields('l', array('id'));
    $cms_feedback_query->condition('l.content_id',$data->content_id , '=');
    $cms_feedback_query_result = $cms_feedback_query->execute()->fetchAssoc();

	$feedback_list = '';
	if($cms_feedback_query_result != ''){
		$feedback_list = l(t('<span class="fn_black"><i class="fa fa-list"></i></span>'), 'cmscontent/feedback_list/'.encryption_decryption_fun('encrypt', $data->content_id), array( 'html' => TRUE, 'attributes' => array('title' => 'Feedback List','target' => '_blank')));
	}	
        $rows[] = array(
            $i,
            $data->content_title,
            $c_type_value[$data->content_typ],
            $order_by,
            $link_k,
            $link_s . '&emsp;' . $dwn . '&emsp;'. $feedback_list
        );
    }
    
    // $output = theme('table', array('header' => $header, 'rows'=> $rows,'empty' => t("<font color='red'><b>Table has no circular!</b></font>"))).theme("pager");
    $variables = array(
        'attributes' => array(
            'class' => array(
                'table table-striped table-responsive admin-custom-table'
            )
        ),
        'header' => $header,
        'rows' => $rows
    );
    
    $output = theme('datatable', $variables);
    
    
    $jj = l(t('Add new'), 'cmscontent/add_what_is_new', array(
        'attributes' => array(
            'class' => 'btn btn-primary pull-left'
        )
    ));
    
    $dic_detail_hlep_element = array(
        '#title' => t('<b></b>'),
        '#children' => $jj,
        '#collapsible' => true,
        '#collapsed' => true
        
    );
    
    $fieldset_dic_help_link = theme('fieldset', array(
        'element' => $dic_detail_hlep_element
    ));
    return '<div class="box box-primary"><div class="box-body">' . $output . '</div></div>';
}

/*==============  List End ============*/

function what_is_new_inactive($contentid = NULL)
{
    global $base_root, $base_path;
    
    
    $cms_content_query = db_select('l_cms_content', 't');
    $cms_content_query->fields('t', array(
        'content_id',
        'is_active'
    ));
    $cms_content_query->condition('t.content_id', trim($contentid), '=');
    $cms_content_query_result = $cms_content_query->execute();
    
    
    $result = $cms_content_query_result->fetchAssoc();
    
    $is_active = trim($result['is_active']);
    if ($is_active == 'Y') {
        $txt      = 'N';
        $inactive = "Inactive";
    } elseif ($is_active == 'N') {
        $txt      = 'Y';
        $inactive = "Active";
    }
    $query = db_update('l_cms_content');
    $query->fields(array(
        
        'is_active' => $txt
    ));
    $query->condition('content_id', $contentid);
    $query->execute();
    $message = "Content are successfully " . $inactive;
    
    $full_base = $base_root . $base_path . 'cmscontent/what_is_new_list';
    
    
    drupal_set_message(t($message));
    
    
    
    drupal_goto($full_base);
}

function whatisnew_edit_fun($id)
{
    return drupal_get_form('whatisnew_edit', $id);
}


function whatisnew_edit($form, $form_state, $id)
{
    
    global $base_root, $base_path, $user;
    
    
    $cms_content_query = db_select('l_cms_content', 't');
    $cms_content_query->fields('t', array(
        'is_new',
        'is_active',
        'content_id',
        'content_title',
        'content_desc',
        'meta_title',
        'meta_desc',
        'fid',
        'content_typ',
        'ord',
        'link_title',
		'achieve_objectives',
		'business_friendly'
    ));
    $cms_content_query->condition('t.content_id', trim(encryption_decryption_fun('decrypt', $id)), '=');
    $cms_content_query_result = $cms_content_query->execute();
    
    
    $result = $cms_content_query_result->fetchAssoc();
    
    $content_id = $result['content_id'];
    
    $content_title = $result['content_title'];
    $content_desc  = decode_entities(trim($result['content_desc']));
    $meta_title    = $result['meta_title'];
    $meta_desc     = $result['meta_desc'];
    $fid           = $result['fid'];
    $content_typ   = $result['content_typ'];
    $is_active     = $result['is_active'];
    $is_new        = $result['is_new'];
    $ord           = $result['ord'];
    $link_title    = $result['link_title'];
    
    
    
    $form['circularaddrfields']['cont_head'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="row"><div class="col-md-12"><div class="box box-primary"><div class="box-body">'
        
    );
    $form['circularaddrfields']['contentid'] = array(
        '#type' => 'hidden',
        '#value' => $content_id
    );
    
    
    
    $form['circularaddrfields']['ltitle'] = array(
        
        '#title' => 'Link title',
        '#prefix' => '<div class="form-custom col-md-4"><label class="input">',
        '#suffix' => '</label></div>',
        '#type' => 'textfield',
        '#default_value' => trim($link_title),
        '#required' => TRUE,
        '#element_validate' => array(
            'lcontent_textfield_validate'
        ),
        '#attributes' => array(
            'class' => array(
                'form-control'
            )
        )
        
        
    );
    
    
    
    $form['circularaddrfields']['ctitle'] = array(
        
        '#title' => 'Content title',
        
        '#prefix' => '<div class="form-custom col-md-4"><label class="input">',
        '#suffix' => '</label></div>',
        '#type' => 'textfield',
        '#default_value' => trim($content_title),
        '#required' => TRUE,
        '#element_validate' => array(
            'lcontent_textfield_validate'
        ),
        '#attributes' => array(
            'class' => array(
                'form-control'
            )
        )
        
    );
    
    
    
    
    $form['circularaddrfields']['edit_content_desc'] = array(
        
        '#title' => 'Legal Basis - Does it have a basis in law/act/policy',
        '#cols' => 20,
        '#rows' => 2,
        '#type' => 'textarea',
        '#default_value' => $content_desc,
        '#format' => 'full_html',
        '#base_type' => 'textarea',
        
        '#attributes' => array(
            'class' => array(
                'form-control'
            )
        ),
        '#prefix' => '<div class="form-custom col-md-12"><label class="input">',
        '#suffix' => '</label></div>'
        
    );    
    
    $form['circularaddrfields']['achieve_objectives'] = array(        
			'#title' => 'Its necessity - Does the license help government achieve its objectives',
			'#cols' => 20,
			'#rows' => 2,
			'#type' => 'textarea',  
			'#default_value' => trim($result['achieve_objectives']),     
			'#format' => 'full_html',
			'#base_type' => 'textarea',        
			'#attributes' => array('class' => array('form-control')),
			'#prefix' => '<div class="form-custom col-md-12">',
			'#suffix' => '</div>'        
          );
	$form['circularaddrfields']['business_friendly'] = array(        
			'#title' => 'Its business-friendly - Does it impose minimum burden on businesses to achieve the government’s objectives',
			'#cols' => 20,
			'#rows' => 2,
			'#type' => 'textarea',        
			'#format' => 'full_html',
			'#base_type' => 'textarea', 
			'#default_value' => trim($result['business_friendly']),     
			'#attributes' => array('class' => array('form-control')),
			'#prefix' => '<div class="form-custom col-md-12">',
			'#suffix' => '</div>'        
          ); 
    
    $form['circularaddrfields']['meta_title'] = array(
        
        '#title' => 'Meta keyword',
        '#type' => 'textfield',
        '#default_value' => $meta_title,
        '#element_validate' => array(
            'lcontent_textfield_validate'
        ),
        '#prefix' => '<div class="form-custom col-md-12"><label class="input">',
        '#suffix' => '</label></div>'
    );
    
    
    
    
    $form['circularaddrfields']['meta_content_desc'] = array(
        
        '#title' => 'Meta Description',
        '#cols' => 20,
        '#rows' => 2,
        '#type' => 'textarea',
        '#default_value' => $meta_desc,
        '#element_validate' => array(
            'lcontent_textfield_validate'
        ),
        '#prefix' => '<div class="form-custom col-md-12"><label class="input">',
        '#suffix' => '</label></div>',
        '#attributes' => array(
            'class' => array(
                'form-control'
            )
        )
    );
    
    $form['l_is_new'] = array(
        '#type' => 'value',
        '#value' => array(
            'Y' => t('Is New')
        )
    );
    
    
    
    
    $form['circularaddrfields']['order_no'] = array(
        
        '#title' => 'Order No',
        '#size' => t('7'),
        '#type' => 'textfield',
        '#maxlength' => 4,
        '#default_value' => trim($ord),
        '#element_validate' => array(
            'ORDER_MANDATORY_NUMERIC_CHECKING_validate'
        ),
        '#attributes' => array(
            'title' => 'order no  should be numeric '
        ),
        '#attributes' => array(
            'class' => array(
                'form-control'
            )
        ),
        '#prefix' => '<div class="form-custom col-md-3"><label class="input">',
        '#suffix' => '</label></div>',
        '#required' => TRUE
    );
    
    $form['l_status'] = array(
        '#type' => 'value',
        '#value' => array(
            'Y' => t('Active'),
            'N' => t('Inactive')
        )
    );
    
    
    
    $form['circularaddrfields']['is_active'] = array(
        '#type' => 'select',
        '#title' => 'Status',
        '#options' => $form['l_status']['#value'],
        '#default_value' => trim($is_active),
        '#prefix' => '<div class="form-custom col-md-3 "><label class="input">',
        '#suffix' => '</label></div>',
        '#required' => TRUE,
        '#attributes' => array(
            'class' => array(
                'form-control'
            )
        )
        
    );
    
    
    $form['circularaddrfields']['is_new'] = array(
        '#type' => 'checkbox',
        '#title' => ' Is New',
        
        '#default_value' => trim($is_new),
        '#prefix' => '<div class="form-custom col-md-3 cus-checkbox-pad custom-chk-top-pad"><label class="">',
        '#suffix' => '</label></div>',
        
        '#attributes' => $is_new == 'Y' ? array(
            'checked' => 'checked'
        ) : array()
        
    );
    
    $form['circularaddrfields']['cont_head1'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="col-md-12">'
    );
    
    $form['circularaddrfields']['edit_content_pdf'] = array(
        
        '#type' => 'managed_file',
        '#title' => t(' Upload an Content  file (JPG,PDF only)'),
        '#attributes' => array(
            'class' => array(
                ''
            )
        ),
        '#process' => array(
            'import_my_file_element_process'
        ),
        '#upload_location' => 'public://contentpdf/'
    );
    
    $form['circularaddrfields']['cont_footer1'] = array(
        '#type' => 'markup',
        '#markup' => '</div">'
    );
    
    $form['circularaddrfields']['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Update',
        '#attributes' => array(
            'class' => array(
                'btn btn-primary'
            )
        ),
        '#prefix' => '<div class="col-md-6#  pull-left">',
        '#suffix' => '</div></div></div></div></div></div>'
    );
    
    return $form;
}




function whatisnew_edit_submit($form_id, $form_state){
    
    $val = $form_state['values'];
    
    $contentid = $val['contentid'];
    
    $cfid      = $val['cfid'];
    $is_active = $val['is_active'];
    $ctitle    = trim($val['ctitle']);
    $is_new    = trim($val['is_new']);
    if (isset($is_new)) {
        $is_new = 'Y';
        
    } else {
        $is_new = 'N';
    }
    
    $contentdesc = trim($val['edit_content_desc']);
    //$contentdesc=trim(check_plain(decode_entities($form_states['values']['contentdesc']['value'])));
    
    $meta_title        = trim($val['meta_title']);
    $meta_content_desc = trim($val['meta_content_desc']);
    
    $content_typ = trim('whatisnew');
    
    $order_no = trim($val['order_no']);
    $ltitle   = check_plain(trim($val['ltitle']));
    
    
    $message = "";
    global $base_root, $base_path, $full_base;
    $dest_dir = file_default_scheme() . '://contentpdf/';
    
    $validators = array(
        'file_validate_extensions' => array(
            'jpg  pdf'
        )
    );
    
    $file_name     = trim($_FILES['files']['name']['edit_content_pdf']);
    $uploaded_file = file_load($val['edit_content_pdf']);
    $txn           = db_transaction();
    
    try {
        
        if (!$file_name) {
            $postdata = array(
                'content_title' => $ctitle,
                'content_desc' => $contentdesc,
                'meta_title' => $meta_title,
                'meta_desc' => $meta_content_desc,
                'content_typ' => $content_typ,
                'ord' => $order_no,
                'is_new' => $is_new,
                'is_active' => $is_active,
                'link_title' => $ltitle,
				'achieve_objectives' => $val['achieve_objectives'],
				'business_friendly' => $val['business_friendly']
				
            );
            $rs       = db_update('l_cms_content')->fields($postdata)->condition('content_id', $contentid)->execute();
            
            $message = 'Update content Successfully.';
            
            
        } else {
            
            $_FILES['files']['name']['edit_content_pdf'] = time() . $file_name;
            
            if ($cfid && $file_name) {
                $query_ent2 = db_query('select  a.fid,filename from l_cms_content a , file_managed b where a.fid=b.fid and  a.fid = :cid', array(
                    ':cid' => $cfid
                ));
                if ($query_ent2->rowCount() > 0) {

                    foreach ($query_ent2 as $obj) {
                        
                        $fid         = $obj->fid;
                        $docfilename = $obj->filename;
                        
                    }
                    
                    
                    $doc_deleted = db_delete('file_managed')->condition('fid', trim($cfid), '=')->execute();
                    
                    $new_path = $dest_dir . $docfilename;
                    unlink($new_path);
                }
            }
            if ($file_name) {
                
                
                
                if ($file = file_save_upload('edit_content_pdf', $validators, $dest_dir, $replace = FILE_EXISTS_RENAME)) {
                    
                    $finfo = @finfo_open(FILEINFO_MIME_TYPE);
                    $mime  = @finfo_file($finfo, $file->uri);
                    finfo_close($finfo);
                    
                    
                    
                    if ($file && $mime == 'application/pdf' || $file && $mime == 'application/jpg') {
                        
                        
                        $uri          = trim($file->uri);
                        $file->status = 1;
                        file_save($file);
                        
                        $fid       = $file->fid;
                        $file_name = $file->filename;
                        
                        
                        $query = db_update('l_cms_content');
                        $query->fields(array(
                            'content_title' => $ctitle,
                            'content_desc' => $contentdesc,
                            'meta_title' => $meta_title,
                            'meta_desc' => $meta_content_desc,
                            'content_typ' => $content_typ,
                            'fid' => $fid,
                            'is_new' => $is_new,
                            'is_active' => $is_active,
                            'ord' => $order_no,
                            'link_title' => $ltitle,
                            'acts_and_rules_pdf' => $file_name
                        ));
                        $query->condition('content_id', $contentid);
                        $query->execute();
                        
                        
                        $message = 'Update content Successfully.';
                    }
                }
                
            }
        }
        
        
    }
    catch (Exception $e) {
        
        $txn->rollback();
        
        watchdog_exception('type', $e);
        $message = ' content Not Update';
    }
    
    
    
    drupal_set_message(t($message));
    
    
}

function feedback_list($content_id){
	$cms_feedback_query = db_select('l_cms_feedback', 'l');
    $cms_feedback_query->fields('l', array());
    $cms_feedback_query->condition('l.content_id',encryption_decryption_fun('decrypt', $content_id) , '=');
    $cms_feedback_query_result = $cms_feedback_query->execute()->fetchAll();
	//print_r($cms_feedback_query_result); exit;
	$header = array(
					array('data' => 'SL.NO.', 'width' => '6.5%'),
					array('data' => 'Name', 'width' => '10%'),
					array('data' => 'Comment','width' => '10%'),
					array('data' => 'Mobile No' ,'width' => '10%'),
					array('data' => 'Submission Date' ,'width' => '10%'),
					array('data' => 'Is Publish' ,'width' => '10%'),
				);	
	$i = 0;			
	foreach ($cms_feedback_query_result as $data) {
		
        $i++;
		if ($data->is_publish == 1){
            $p_link = 'cmscontent/what_is_new_publish/'.encryption_decryption_fun('encrypt', $data->id) ;
            $link_p = l('<span class="fn_green">&nbsp;<i class="fa fa-check-circle"></i>&nbsp;</span>', $p_link, array( 'html' => TRUE,                 'attributes' => array( 'title' => 'Click to Remove' ) ));
        } 
		if($data->is_publish == 0){
            $p_link = 'cmscontent/what_is_new_publish/'.encryption_decryption_fun('encrypt', $data->id) ;
            $link_p = l('<span class="fn_red">&nbsp;<i class="fa fa-times-circle"></i>&nbsp;</span>', $p_link, array('html' => TRUE,                'attributes' => array('title' => 'Click to Publish')));
        }
		
		$rows[] = array(
					array('data' => $i, 'align' => 'left', 'class' => array('odd')),
					array('data' => $data->fullname, 'align' => 'left', 'class' => array('odd')),
					array('data' => $data->comment, 'align' => 'left', 'class' => array('odd')),
					array('data' => $data->mobile, 'align' => 'left', 'class' => array('odd')),
					array('data' => date('d-m-Y',strtotime($data->submitted_date)), 'align' => 'left', 'class' => array('odd')),
					array('data' => $link_p)
				  );
				  
	}
	$variables = array(
					'attributes' => array('class' => array('table table-striped table-responsive admin-custom-table')), 
					'header' => $header,
					'rows' => $rows,
					'empty' => t("Data not available.")
				  );
	
	
	$output .= theme('datatable', $variables);
	return '<div class="box box-primary"><div class="box-body">' . $output . '</div></div>';;
}

function what_is_new_publish($feedback_id = ''){
    global $base_root, $base_path;
    
    $cms_content_query = db_select('l_cms_feedback', 't');
    $cms_content_query->fields('t', array('content_id', 'is_publish'));
    $cms_content_query->condition('t.id', encryption_decryption_fun('decrypt', $feedback_id), '=');
    $cms_content_query_result = $cms_content_query->execute();
    
    $result = $cms_content_query_result->fetchAssoc();
    
    $is_publish = trim($result['is_publish']);
    if($is_publish == 1) {
        $publish_data = 0;
        $txt = "Remove";
    } 
	if($is_publish == 0) {
        $publish_data = 1;
        $txt = "Publish";
    }
    $query = db_update('l_cms_feedback');
    $query->fields(array('is_publish' => $publish_data ));
    $query->condition('id', encryption_decryption_fun('decrypt', $feedback_id));
    $query->execute();
    $message = "Content is successfully ".$txt;
    if($query){
		drupal_set_message(t($message));
	}else{
		drupal_set_message(t('Some error occured.'));
	}
    
    drupal_goto('cmscontent/feedback_list/'.encryption_decryption_fun('encrypt', $result['content_id']));
}

?>
