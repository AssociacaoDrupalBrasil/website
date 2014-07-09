<?php

function drupalbrasil_preprocess_node(&$vars) {
  if ($vars['type'] == 'webform') {
    unset($vars['user_picture']);
    unset($vars['classes_array'][array_search('node-with-picture', $vars['classes_array'])]);
    unset($vars['classes_array'][array_search('user-picture-available', $vars['classes_array'])]);
  }
}
