<?php

/**
  * CUSTOM --- Change print_mail_form to include our fields
**/

function rtm_views_form_print_mail_form_alter(&$form, &$form_state) {

// Move the from fields to the top of the form
  $form['fld_from_addr']['#weight'] = -9;
  $form['fld_from_name']['#weight'] = -8;

// Change the form validation and submit to be our functions
  $form['#validate'] = array('rtm_views_print_mail_validate');
  $form['#submit'] = array('rtm_views_print_mail_submit');

// Add a field set to organize the recipients
  unset($form['txt_to_addrs']);

  $form['to_addr_container'] = array (
    '#weight' => -7,
    '#type' => 'fieldset',
    '#title' => '',
//  '#description' => 'You may enter one or more e-mail addresses, and/or select one or more RTM districts or committees',
    '#description' => 'You may enter one or more e-mail addresses, and/or select one or more RTM Committees',
  );
  $form['to_addr_container']['txt_to_addr'] = array(
    '#type' => 'textarea',
    '#title' => t('E-Mail Addresses'),
    '#rows' => 3,
    '#resizable' => FALSE,
//  '#parents' => array('to_addr_container'),
    '#description' => t('Enter multiple addresses separated by commas and/or different lines.'),
//  '#required' => TRUE,
  );

// Retrieve the districts and committees to populate the list box
  $options = array();
  $result = db_query("select distinct field_cmte_value from {content_field_cmte} order by field_cmte_value");
  while ($row = db_fetch_array($result)) {
     $options[$row['field_cmte_value']] = $row['field_cmte_value'];
  }
//$result = db_query("select distinct field_district_value from {content_field_district} order by field_district_value");
//while ($row = db_fetch_array($result)) {
//   $options["-D-".$row['field_district_value']] = "District ".$row['field_district_value'];
//}
  $form['to_addr_container']['list_to_addrs'] = array(
    '#type' => 'select',
    '#title' => 'RTM Committees',
//  '#parents' => array('to_addr_container'),
    '#options' => $options,
    '#multiple' => TRUE,
    '#description' => 'Select one or more RTM Committees',
//  '#disabled' => TRUE
  );

}


function rtm_views_print_mail_validate($form, &$form_state) {

  if (array_key_exists('cancel', $form['#post'])) {
    return;
  }

//print "<pre>\n";
//print_r($form_state);
//print "</pre>\n";

  $from_addr = trim($form_state['values']['fld_from_addr']);
  $test = user_validate_mail($from_addr);
  if ($test) {
    form_set_error('fld_from_addr', $test);
  }

  if (strlen(trim($form_state['values']['txt_to_addr'])) > 0) {
     // All new-lines are replaced by commas
     $to_addrs = preg_replace('![\r|\n|,]+!', ',', trim($form_state['values']['txt_to_addr']));
     // Create an array from the string
     $txt_array = explode(',', $to_addrs);
     // Verify each element of the array
     foreach ($txt_array as $key => $address) {
        $address = trim($address);
        if (preg_match('/(.*?) <(.*)>/s', $address, $matches)) {
           // Address is of the type User Name <user@domain.tld>
           $test = user_validate_mail($matches[2]);
           $txt_array[$key] = trim($matches[1]) .' <'. $matches[2] .'>';
        } else {
           // Address must be user@domain.tld
           $test = user_validate_mail($address);
       }
       if ($test) {
          form_set_error('txt_to_addr', $test);
       }
     }
  } else {
     $txt_array = array();
  }

  if ((count($txt_array) == 0) && (count($form_state['values']['list_to_addrs']) == 0)) {
     form_set_error('list_to_addrs', "No E-Mail Addresses. You must enter at least one e-mail address or select one RTM Committee.");
  }

  // In all fields, prevent insertion of custom headers
  foreach ($form_state['values'] as $key => $string) {
    if ( (substr($key, 0, 4) == 'fld_') && ((strpos($string, "\n") !== FALSE) || (strpos($string, "\r") !== FALSE)) ) {
      form_set_error($key, 'Found invalid character');
    }
  }

  $form_state['values']['fld_from_addr'] = $from_addr;
  $form_state['values']['fld_from_name'] = trim($form_state['values']['fld_from_name']);
  // Re-create the string from the re-organized array
  $form_state['values']['txt_to_addrs'] = implode(', ', $txt_array);
//file_put_contents("/tmp/drupal.debug", "final to_array: ".print_r($txt_array, TRUE), FILE_APPEND);
}


/**
 * Process the send by-email form submission.
 *
 * @ingroup forms
 */
function rtm_views_print_mail_submit($form, &$form_state) {


  if (!array_key_exists('cancel', $form_state['values'])) {
    if (!empty($form_state['values']['fld_from_name'])) {
      $from = '"'. $form_state['values']['fld_from_name'] .'" <'. $form_state['values']['fld_from_addr'] .'>';
    }
    else {
      $from = $form_state['values']['fld_from_addr'];
    }
    $cid = isset($form_state['values']['cid']) ? $form_state['values']['cid'] : NULL;

/**
 * Adding text that shows where to whom message was sent
 */
    $to_string = $form_state['values']['txt_to_addrs'] . " " . implode(", ", $form_state['values']['list_to_addrs']);
    $to_string = preg_replace("(-D-)","District ", $to_string);
    $form_state['values']['txt_message'] = "This message was sent to: " . $to_string . "\n\n" . $form_state['values']['txt_message'];


//  $print_mail_text_message = filter_xss_admin(variable_get('print_mail_text_message', t('Message from sender')));
//  $sender_message = $print_mail_text_message .':<br /><br /><em>'. nl2br(check_plain($form_state['values']['txt_message'])) .'</em>';
    $sender_message = nl2br(check_plain($form_state['values']['txt_message'])) .'</em>';

    $print = print_controller($form_state['values']['path'], $cid, PRINT_MAIL_FORMAT, $form_state['values']['chk_teaser'], $sender_message);

    // Spaces in img URLs must be replaced with %20
    $pattern = '!<(img\s[^>]*?)>!is';
    $print['content'] = preg_replace_callback($pattern, '_print_replace_spaces', $print['content']);

    if ($print !== FALSE) {
      $params = array();
      $params['subject'] = $form_state['values']['fld_subject'];
      $params['message'] = $form_state['values']['txt_message'];
      $params['link'] = $print['url'];
      $params['title'] = $form_state['values']['title'];
      $params['headers']['Return-Path'] = "rtm-admin@greenwich.org";

      $node = $print['node'];
      $params['body'] = theme('print_page', $print, PRINT_MAIL_FORMAT, $node);
      $params['body'] = drupal_final_markup($params['body']);

      $ok = FALSE;
      if (function_exists('job_queue_add') && variable_get('print_mail_job_queue', PRINT_MAIL_JOB_QUEUE_DEFAULT)) {
        $use_job_queue = TRUE;
        $this_file = drupal_get_path('module', 'print_mail') .'/print_mail.inc';
      }
      else {
        $use_job_queue = FALSE;
      }

      if (strlen(trim($form_state['values']['txt_to_addrs'])) > 0) {
         $addresses = explode(', ', $form_state['values']['txt_to_addrs']);
      } else {
         $addresses = array();
      }
/**
 * The Chairmen query
 * select distinct rtm_users.name, rtm_users.mail, profile.value from rtm_users_roles, rtm_users  left join rtm_profile_values profile on profile.uid=rtm_users.uid and profile.fid=10 where rtm_users.uid=rtm_users_roles.uid and rtm_users_roles.rid=4;
 */
/**
 * The committee address query
 *
 * select distinct rtm_users.name, rtm_users.mail, profile.value from rtm_users  
 * left join rtm_profile_values profile on profile.uid=rtm_users.uid and profile.fid=10 
 * left join rtm_content_field_cmte_member member on member.field_cmte_member_uid=rtm_users.uid 
 * left join rtm_content_field_cmte cmte on member.vid=cmte.vid where cmte.field_cmte_value in ('Claims','Labor Contracts');
 */

//file_put_contents("/tmp/drupal.debug", "Committee Form Box: ".print_r($form_state['values']['list_to_addrs'], TRUE), FILE_APPEND);
      if (count($form_state['values']['list_to_addrs']) > 0) {
         $sql = "select distinct {users}.name, {users}.mail, profile.value from {users} left join {profile_values} profile on profile.uid=rtm_users.uid and profile.fid=10 left join {content_field_cmte_member} member on member.field_cmte_member_uid=rtm_users.uid left join {content_field_cmte} cmte on member.vid=cmte.vid where cmte.field_cmte_value in (" . db_placeholders($form_state['values']['list_to_addrs'], 'varchar') . ")";

         $cmtes = implode("','", $form_state['values']['list_to_addrs']);
         $result = db_query($sql, $form_state['values']['list_to_addrs']);
         while ($row = db_fetch_array($result)) {
            if ($row['value'] && strlen($row['value'] > 0)) {
               array_push($addresses, $row['value']);
            } else {
               array_push($addresses, $row['mail']);
            }
         }
       }

//file_put_contents("/tmp/drupal.debug", "---------------\n".print_r($params, TRUE)."-----------------\n", FILE_APPEND);
/** TESTING  **/
//    $addresses = array("xxxxxxxx@gmail.com");

      foreach ($addresses as $to) {
        // Call to hook_print_mail_before_send in order to know if the mail can be sent
        // Handlers must return TRUE or FALSE
        $can_send = module_invoke_all('print_mail_before_send', $node, $to, $from, $params);

        if (!in_array(FALSE, $can_send)) {
          if ($use_job_queue) {
            // Use job queue to send mails during cron runs
            job_queue_add('drupal_mail', t('print_mail: From %from', array('%from' => $from)), array('print_mail', $form_state['values']['fld_send_option'], $to, language_default(), $params, $from, TRUE), $this_file, TRUE);
          }
          else {
            // Send mail immediately using Drupal's mail handler
//          $ret = drupal_mail('print_mail', $form_state['values']['fld_send_option'], $to, language_default(), $params, $from, TRUE);
            $ret = drupal_mail('print_mail', "sendpage", $to, language_default(), $params, $from, TRUE);
          }
//file_put_contents("/tmp/drupal.debug", "---------------\n".print_r($ret, TRUE)."-----------------\n", FILE_APPEND);
          if ($ret['result'] || $use_job_queue) {
            // Call to hook_print_mail_after_send in order to provide information to other modules.
            module_invoke_all('print_mail_after_send', $node, $to, $from, $params);

            flood_register_event('print_mail');
            $ok = TRUE;
          }
        }
      }

      if ($ok) {
        watchdog('print_mail', '%name [%from] sent %page to [%to]', array('%name' => $form_state['values']['fld_from_name'], '%from' => $form_state['values']['fld_from_addr'], '%page' => $form_state['values']['path'], '%to' => $form_state['values']['txt_to_addrs']));
        $site_name = variable_get('site_name', t('us'));
        $print_mail_text_confirmation = variable_get('print_mail_text_confirmation', t('Thank you for spreading the word about !site.'));
        drupal_set_message(check_plain(t($print_mail_text_confirmation, array('!site' => $site_name))));

        $nodepath = drupal_get_normal_path($form_state['values']['path']);
        db_query("UPDATE {print_mail_page_counter} SET sentcount = sentcount + %d, sent_timestamp = %d WHERE path = '%s'", count($addresses), time(), $nodepath);
      }
    }
  }

  $form_state['redirect'] = preg_replace('!^book/export/html/!', 'node/', $form_state['values']['path']);
}

?>
