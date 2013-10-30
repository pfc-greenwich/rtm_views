<?php





function _rtm_views_members($search = NULL) {

  $sql = "select rn.realname, u.uid from {realname} rn, {users} u, {users_roles} ur, {role} r where rn.uid=u.uid and ur.uid=u.uid and ur.rid=r.rid and r.name='RTM Member' and rn.realname like CONCAT('%','$search','%')";

  $result = db_query($sql);

  $items = array();
  while ($row = db_fetch_object($result)) {
#    $row->realname = str_replace("\\"","'", $row->realname);
#    $row->realname = str_replace("\'","'", $row->realname);
     array_push($items, $row);
  }

  $obj = new stdClass();
  $obj->matches = $items;

//$json = drupal_to_js($result->fetchAll());
  header("Content-Type: application/json");
  print $_GET["callback"];
  print "(";
  print json_encode($obj);
  print ");";
//print_r($items);
  die;
}


function _rtm_views_members_info($search = NULL, $parm = NULL) {

  if (!isset($search)) {
     return;
  }

  if (!isset($parm)) {
     return;
  }

  $sql = "select * from rtm_all_members_secure where $search = $parm order by district,lastname,firstname";

  $result = db_query($sql);

  $items = array();
  while ($row = db_fetch_object($result)) {
#    $row->realname = str_replace("\\"","'", $row->realname);
#    $row->realname = str_replace("\'","'", $row->realname);
     array_push($items, $row);
  }

  $obj = new stdClass();
  $obj->matches = $items;

//$json = drupal_to_js($result->fetchAll());
  header("Content-Type: application/json");
  print $_GET["callback"];
  print "(";
  print json_encode($obj);
  print ");";
//print_r($items);
  die;
}







function _rtm_views_committee_list() {

  $result = db_query("select distinct(c.field_cmte_value) from {content_field_cmte} c, {node} n where n.type='rtm_committee' and c.vid=n.vid order by c.field_cmte_value");

  $items = array();
  while ($row = db_fetch_array($result))
     array_push($items, $row['field_cmte_value']);

  $json = drupal_to_js($items);
  print $json;
  die;
}


function _rtm_views_committee_members($committee_name = NULL) {

  $sql = "select c.nid,c.field_cmte_value,m.field_cmte_member_uid,r.realname from rtm_content_field_cmte c, rtm_content_field_cmte_member m,rtm_node n, rtm_realname r where c.nid=m.nid and c.vid = m.vid and n.nid=c.nid and n.type='rtm_committee'  and c.field_cmte_value='$committee_name' and m.field_cmte_member_uid=r.uid";


  $result = db_query($sql);

  $items = array();
//array_push($items, $committee_name);
//array_push($items, $sql);

  while ($row = db_fetch_array($result)) {
     $fields = array();
     array_push($fields, $row['nid']);
     array_push($fields, $row['field_cmte_value']);
     array_push($fields, $row['field_cmte_member_uid']);
     array_push($fields, $row['realname']);
     array_push($items, $fields);
  }
  $json = drupal_to_js($items);
  print $json;
  die;
}

function _rtm_views_members_add() {

    file_put_contents("/tmp/drupal.debug", "_rtm_views_members_add(): ".print_r($_POST, TRUE), FILE_APPEND);

    if (!isset($_POST['json'])) {
       watchdog('rtm_views','json not posted', NULL);
       return;
    }
    $obj = json_decode($_POST['json']);

    if (!isset($obj->uid)) {
       watchdog('rtm_views','uid not set', NULL);
       return;
    }

    if (!isset($obj->committee)) {
       watchdog('rtm_views','committee not set', NULL);
       return;
    }
//  file_put_contents("/tmp/drupal.debug", "_rtm_views_members_add(): JSON object=".print_r($obj, TRUE), FILE_APPEND);

    $node =  new StdClass();
    $node->type = 'rtm_committee';
    $node->title = $obj->committee . ' Committee';
    $node->body = '';
    $node->teaser = '';
    $node->uid = 1;
    $node->name = 'admin';
    $node->status = 1;
    $node->promote = 0;
    $node->field_cmte[0]['value'] = $obj->committee;
    $node->field_cmte_member[0]['uid'] = $obj->uid;

    node_save($node);
//  file_put_contents("/tmp/drupal.debug", "_rtm_views_members_add() node=: ".print_r($node, TRUE), FILE_APPEND);
    if ($node->nid) {
       drupal_set_message("member add called ... ".$obj->uid." into ".$obj->committee);
    }
    drupal_goto("/edit/committee/members");
}

function _rtm_views_members_delete() {

//  file_put_contents("/tmp/drupal.debug", "_rtm_views_members_add(): ".print_r($_POST, TRUE), FILE_APPEND);

    if (!isset($_POST['json'])) {
       watchdog('rtm_views','json not posted', NULL);
       return;
    }
    $obj = json_decode($_POST['json']);

    if (!isset($obj->uid)) {
       watchdog('rtm_views','uid not set', NULL);
       return;
    }

    if (!isset($obj->nid)) {
       watchdog('rtm_views','nid not set', NULL);
       return;
    }
    node_delete($obj->nid);
    drupal_goto("/edit/committee/members");
}

function _rtm_views_members_delegate() {

//  file_put_contents("/tmp/drupal.debug", "_rtm_views_members_add(): ".print_r($_POST, TRUE), FILE_APPEND);

    if (!isset($_POST['json'])) {
       watchdog('rtm_views','json not posted', NULL);
       drupal_set_header($_SERVER['SERVER_PROTOCOL'] . ' 406 Not Acceptable');
       return;
    }
    $obj = json_decode($_POST['json']);

    if (!isset($obj->uid)) {
       watchdog('rtm_views','uid not set', NULL);
       drupal_set_header($_SERVER['SERVER_PROTOCOL'] . ' 406 Not Acceptable');
       return;
    }

    if (!isset($obj->nid)) {
       watchdog('rtm_views','nid not set', NULL);
       drupal_set_header($_SERVER['SERVER_PROTOCOL'] . ' 406 Not Acceptable');
       return;
    }
//  file_put_contents("/tmp/drupal.debug", "_rtm_views_members_delegate() nid=".$obj->nid." uid=".$obj->uid."\n\n", FILE_APPEND);
    $node = node_load($obj->nid, NULL, TRUE);
    if ($obj->uid == "Anonymous") {
       unset($node->field_cmte_member[0]['uid']);
    } else {
       $node->field_cmte_member[0]['uid'] = $obj->uid;
    }
    node_submit($node);
    node_save($node);
//  file_put_contents("/tmp/drupal.debug", "_rtm_views_members_delegate() node=: ".print_r($node, TRUE), FILE_APPEND);
    if ($node->nid) {
       drupal_set_message("delegate update called ... ".$obj->uid." into ".$obj->nid);
    } else {
       drupal_set_header($_SERVER['SERVER_PROTOCOL'] . ' 406 Not Acceptable');
       return;
    }
    drupal_set_header($_SERVER['SERVER_PROTOCOL'] . ' 200 OK');
}

?>
