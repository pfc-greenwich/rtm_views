<?php

function rtm_views_admin() {

   $form = array();

   $form['rtm_views_vertical'] = array (
      '#type' => 'textfield',
      '#title' => t('Vertical (Line) Spacing'),
      '#size' => 10,
      '#maxlength' => 10,
      '#description' => t('Spacing of each line on the card in inches'),
      '#required' => TRUE,
      '#default_value' => variable_get('rtm_views_vertical'),
   );

   $form['rtm_views_horizontal'] = array (
      '#type' => 'textfield',
      '#title' => t('Horizontal (between boxes) Spacing'),
      '#size' => 10,
      '#maxlength' => 10,
      '#description' => t('Spacing of each column on the card in inches'),
      '#required' => TRUE,
      '#default_value' => variable_get('rtm_views_horizontal'),
   );

   $form['rtm_views_top'] = array (
      '#type' => 'textfield',
      '#title' => t('Top Margin Spacing'),
      '#size' => 10,
      '#maxlength' => 10,
      '#description' => t('Space at the top of the card'),
      '#required' => TRUE,
      '#default_value' => variable_get('rtm_views_top'),
   );

   $form['rtm_views_left'] = array (
      '#type' => 'textfield',
      '#title' => t('Left Margin Spacing'),
      '#size' => 10,
      '#maxlength' => 10,
      '#description' => t('Left Margin Spacing'),
      '#required' => TRUE,
      '#default_value' => variable_get('rtm_views_left'),
   );

   return system_settings_form($form);

}

?>
