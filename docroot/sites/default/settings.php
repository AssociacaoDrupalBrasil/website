<?php

/**
 * Settings router
 */
$drupal_hash_salt = 'lTtk9qSZ0uobIRiOwGEO5J2F3CBhYBZgF_mCvsVMFGs';

ini_set('session.gc_probability', 1);
ini_set('session.gc_divisor', 100);
ini_set('session.gc_maxlifetime', 200000);
ini_set('session.cookie_lifetime', 2000000);

$conf['404_fast_paths_exclude'] = '/\/(?:styles)\//';
$conf['404_fast_paths'] = '/\.(?:txt|png|gif|jpe?g|css|js|ico|swf|flv|cgi|bat|pl|dll|exe|asp)$/i';
$conf['404_fast_html'] = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN" "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd"><html xmlns="http://www.w3.org/1999/xhtml"><head><title>404 Not Found</title></head><body><h1>Not Found</h1><p>The requested URL "@path" was not found on this server.</p></body></html>';

if (isset($_ENV['AH_SITE_ENVIRONMENT'])) {
// On Acquia Cloud, this include file configures Drupal to use the correct
// database in each site environment (Dev, Stage, or Prod). To use this 
// settings.php for development on your local workstation, set $db_url
// (Drupal 5 or 6) or $databases (Drupal 7) as described in comments above.
  if (file_exists('/var/www/site-php')) {
    require('/var/www/site-php/drupalbrasil/drupalbrasil-settings.inc');
  }
  switch ($_ENV['AH_SITE_ENVIRONMENT']) {
    case 'dev':
      include 'settings.acquia.dev.php';
      break;

    case 'test':
      include 'settings.acquia.test.php';
      break;

    case 'prod':
      include 'settings.acquia.prod.php';
      break;
  }
}
else {
  include 'settings.non-acquia.local.php';
}
