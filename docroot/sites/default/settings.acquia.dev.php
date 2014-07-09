<?php

/**
 * Acquia Dev Environment Settings
 */
$update_free_access = true;

// Display all errors and set high memory.
error_reporting(E_ALL | E_STRICT);
ini_set('display_errors', TRUE);
ini_set('display_startup_errors', TRUE);
ini_set('track_errors', TRUE);
ini_set('html_errors', TRUE);

// Set Drupal variables for development. Others are here:
// http://drupal.org/node/1525472
$conf['preprocess_js']  = 0;
$conf['preprocess_css'] = 0;
$conf['cache']          = 0;
$conf['block_cache']    = 0;
