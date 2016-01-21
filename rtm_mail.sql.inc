<?php

// select rtm_users.name, rtm_users.mail, profile.value  from rtm_users 
// left join rtm_profile_values profile on profile.uid=rtm_users.uid and profile.fid=10 
// left join rtm_users_roles role on role.uid=rtm_users.uid where role.rid in ( 3 );

function _rtm_mail_get_roles() {

  $sql = "select * from {role} where rid > 2";
  $result = db_query($sql);

  $items = array();
  while ($row = db_fetch_object($result)) {
     array_push($items, $row);
  }

  return $items;
}

function _rtm_mail_get_single_role($rid) {

  $sql = "select name from {role} where rid = " . $rid;
  $result = db_query($sql);

  $row = db_fetch_object($result);
  return $row->name;
}


function _rtm_mail_get_addresses($form_state) {

  $sql =  "select {users}.name, {users}.mail, profile.value  from {users} ";
  $sql .= "left join {profile_values} profile on profile.uid=rtm_users.uid and profile.fid=10 ";
  $sql .= "left join {users_roles} role on role.uid={users}.uid where role.rid = " . $form_state['values']['to_addr'];

  $result = db_query($sql);

  $items = array();
  while ($row = db_fetch_object($result)) {
     array_push($items, $row);
  }

  return $items;
}
?>
