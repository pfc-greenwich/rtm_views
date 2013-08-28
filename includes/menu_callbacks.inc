<?php

/**
 * These menu callbacks can be used to access a list of RTM Committees or a list of
 * members of a committee via JSON. Basically, this would be used to have a GWT/AJAX editing
 * capability for RTM Committees
**/


function rtm_views_menu() {
   $items = array();

   $items['export/gwt/rtm-cmte-list'] = array (
      'page callback' => '_rtm_views_committee_list',
      'type' => MENU_CALLBACK,
      'access arguments' => array('access content'),
   );

   $items['export/gwt/rtm-cmte-members/%'] = array (
      'page callback' => '_rtm_views_committee_members',
      'page arguments' => array(3),
      'type' => MENU_CALLBACK,
      'access arguments' => array('access content'),
   );

   $items['export/gwt/autocomplete/rtm-member/%'] = array (
      'page callback' => '_rtm_views_members',
      'page arguments' => array(4),
      'type' => MENU_CALLBACK,
      'access arguments' => array('access content'),
   );

   $items['export/gwt/rtm-member/%/%'] = array (
      'page callback' => '_rtm_views_members_info',
      'page arguments' => array(3,4),
      'type' => MENU_CALLBACK,
      'access arguments' => array('access content'),
   );

/**
 * These items allow for updating RTM committees, hence the permissions
**/

   $items['edit/committee/members/add'] = array (
      'page callback' => '_rtm_views_members_add',
      'type' => MENU_CALLBACK,
      'access arguments' => array('update rtm committees'),
   );

   $items['edit/committee/members/delete'] = array (
      'page callback' => '_rtm_views_members_delete',
      'type' => MENU_CALLBACK,
      'access arguments' => array('update rtm committees'),
   );

   $items['edit/committee/delegates/update'] = array (
      'page callback' => '_rtm_views_members_delegate',
      'type' => MENU_CALLBACK,
      'access arguments' => array('update rtm committees'),
   );

   return $items;
}

?>