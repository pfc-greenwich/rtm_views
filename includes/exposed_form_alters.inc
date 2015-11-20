<?php


/**
 * These functions provide select list boxes for views exposed filters.
 * Specifically, for those views that display RTM Committee members
**/

function rtm_views_form_views_exposed_form_alter(&$form, $form_state) {

// View: edit_standing_cmtes
// Display: page_1
// Filter: field_cmte_value
// views-exposed-form-edit-standing-cmtes-page-1

  if ($form['#id'] == 'views-exposed-form-edit-standing-cmtes-page-1') {
//  $result = db_query('select distinct(c.field_cmte_value) from {content_field_cmte} c');
    $result = db_query("select distinct(c.field_cmte_value) from {content_field_cmte} c, {node} n where n.type='standing_cmte' and c.vid=n.vid order by c.field_cmte_value");
    while ($row = db_fetch_array($result)) {
      $options[$row['field_cmte_value']] = $row['field_cmte_value'];
    }
    $form['field_cmte_value'] = array(
    '#type' => 'select',
    '#default_value' => 'Appointments', 
    '#options' => $options
    );
  }


// View: cmte_officer
// Display: page_1
// Filter: field_cmte_value
// views-exposed-form-cmte-officer-page-1

  if ($form['#id'] == 'views-exposed-form-cmte-officer-page-1') {
    $result = db_query("select distinct(c.field_cmte_value) from {content_field_cmte} c order by c.field_cmte_value");
    while ($row = db_fetch_array($result)) {
      $options[$row['field_cmte_value']] = $row['field_cmte_value'];
    }
    $form['field_cmte_value'] = array(
    '#type' => 'select',
    '#default_value' => 0, 
    '#options' => $options,
    );
  }

// View: edit_rtm_cmte
// Display: page_1
// Filter: field_cmte_value
// views-exposed-form-edit-rtm-cmte-page-1

//if ($form['#id'] == 'views-exposed-form-edit-rtm-cmte-page-1') {
//  $result = db_query("select distinct(c.field_cmte_value) from {content_field_cmte} c, {node} n where n.type='rtm_committee' and c.vid=n.vid order by c.field_cmte_value");
//  while ($row = db_fetch_array($result))
//    $options[$row['field_cmte_value']] = $row['field_cmte_value'];
//    $form['field_cmte_value'] = array(
//    '#type' => 'select',
//    '#default_value' => 0, 
//    '#options' => $options,
//  );
//}

}

?>
