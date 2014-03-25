<?php


/**
 * @file
 * Theme setting callbacks for the Adminimal theme.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * @param $form
 *   The form.
 * @param $form_state
 *   The form state.
 */
function adminimal_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['theme_settings']['display_icons_config'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display icons in Configuration page'),
    '#default_value' => theme_get_setting('display_icons_config'),
  );

}
