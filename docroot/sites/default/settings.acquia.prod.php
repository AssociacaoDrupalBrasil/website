<?php

/**
 * Acquia Prod Environment Settings
 */
if ($_SERVER['HTTP_HOST'] == 'drupalbrasil.prod.acquia-sites.com' ||
  $_SERVER['HTTP_HOST'] == 'drupal.org.br') {
  header('HTTP/1.0 301 Moved Permanently');
  header('Location: http://www.drupal.org.br' . $_SERVER['REQUEST_URI']);
  exit();
}

$update_free_access = FALSE;

$conf['preprocess_js']  = 1;
$conf['preprocess_css'] = 1;
$conf['cache']          = 1;
$conf['block_cache']    = 1;