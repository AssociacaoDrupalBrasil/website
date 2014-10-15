<?php

// Allow local update.php access.
$update_free_access = true;

// Display all errors and set high memory.
error_reporting(E_ALL | E_STRICT);
ini_set('display_errors', TRUE);
ini_set('display_startup_errors', TRUE);
ini_set('track_errors', TRUE);
ini_set('html_errors', TRUE);
ini_set('memory_limit', '256M');

// Set Drupal variables for development. Others are here:
// http://drupal.org/node/1525472
$conf['cache_inc'] = './includes/cache.inc'; // don't use memcache
$conf['preprocess_js']    = 0; // don't zip js files
$conf['preprocess_css']   = 0; // don't zip css files
$conf['cache']            = 0; // no page caching
$conf['block_cache']      = 0; // no block caching
$conf['views_skip_cache'] = 1;
$conf['file_temporary_path'] = "/tmp";

$databases = array();
$databases['default']['default'] = array(
  'driver' => 'mysql',
  'database' => 'drupalbrasil',
  'username' => 'root',
  'password' => '',
  'host' => 'localhost',
  'prefix' => '',
);
