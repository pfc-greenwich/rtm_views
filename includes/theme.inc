<?php

/**
 * CUSTOM Theme Related Functions
 */

/**
 * Use this to return links or whatever
 */
function toplinks() {
  return '';
}

function phptemplate_menu_item_link($link) {

  if (empty($link['localized_options'])) {
    $link['localized_options'] = array();
  }

  global $user;                                           // Get the current user
  profile_load_profile($user);

// print '<pre>';
// print_r($user->roles);
// print '</pre>';

// ($item['type'] & MENU_IS_LOCAL_TASK)

//if ($link['mlid'] == 21) {
//   $link['title'] = "Your Personal Information";
//}
 
  if (stristr($link['href'],"edit/standing-cmte")) {
     if (in_array('District Chairman',$user->roles)) {
        $link['href'] = str_replace("edit/standing-cmte", "edit/standing-cmte/".$user->{profile_district}, $link['href']);
        return l($link['title'], $link['href'], $link['localized_options']);
     }
  }

  if (stristr($link['href'],"edit/district/officers")) {
     if (in_array('District Chairman',$user->roles)) {
        $link['href'] = str_replace("edit/district/officers", "edit/district/officers/".$user->{profile_district}, $link['href']);
        return l($link['title'], $link['href'], $link['localized_options']);
     }
  }


  return l($link['title'], $link['href'], $link['localized_options']);



//  if ($item['path'] == '<none>') {
//    $attributes['title'] = $link['description'];
//    return '<span'. drupal_attributes($attributes) .'>'. $item['title'] .'</span>';
//  }
//  else {
//    return l($item['title'], $link_item['path'], !empty($item['description']) ? array('title' => $item['description']) : array(), isset($item['query']) ? $item['query'] : NULL);
//  }

}




/**
  * Theme Function to /remove/ certain fields from the standard node edit forms
**/

function zeropoint_theme() {
   return array(
      'event_node_form' => array(
          'arguments' => array('form' => NULL),
      ),
      'announcement_node_form' => array(
          'arguments' => array('form' => NULL),
      ),
   );
}

function zeropoint_event_node_form($form) {
   $form['title']['#title'] = 'Meeting or Event Name';
   $form['body_field']['#title'] = 'Meeting or Event Description';
   $form['body_field']['body']['#rows'] = 4;
// $form['body_field']['teaser_include']['#access'] = FALSE;
   return(drupal_render($form));
// return print_r($form);
}

function zeropoint_announcement_node_form($form) {
   unset($form['comment_settings']);
   return(drupal_render($form));
}

?>
