<?php

/**
 * @file
 * The functions in this file are used by the Internal Links filter as part of
 * the process of the handling the "bad links filter". Moving some of the
 * functions to separate files helps to streamline the code, especially for
 * use cases which only require one of the filters, and makes it more "ready
 * for the future", when additional filters could be added to this set.
 *
 * @author Lowell Montgomery | Cocomore AG
 *   @see http://drupal.org/user/628196
 */

include_once 'intlinks_common_functions.inc';

/**
 * More efficient way to get node status (un/published) than using node_load().
 *
 * @param $nid
 *   The node id corresponding to the node status this function returns.
 *
 * @return
 *   Value of "status" (1 if published; 0 if not),
 *   or FALSE if the db_query call fails for some reason, which means we can
 *   add handling to log links as possible errors (no node with $nid exists).
 */
function intlinks_is_published($nid) {
  return db_query("SELECT status FROM {node} WHERE nid = :nid",
  array(':nid' => $nid))->fetchField();
}

/*
 * Checks if $path is a special internal path (not a node, but still valid).
 *
 * Most such nodes are admin/ -type paths. Others are provided by modules
 * such as views paths for views pages. The 'fit' column is used as a field to
 * check since it appears to contain some non-zero data for every path.
 *
 * So if this query gets *anything*, we know it's a special internal path.
 * Then we can decide what to do with that information.
 *
 * Note: This function does NOT check that the visitor will have
 * access to a special Drupal path, nor that the path is even really valid
 * (i.e. numeric parts are substituted for the % wildcard, which can stand
 * for term IDs, node IDs, user IDs, etc. So if the function is passed
 * "user/500", it looks to see if "user/%" is a valid Drupal path, but does
 * does not check further to identify the argument types (and validate the IDs)
 * of users, terms, etc, so there could be no user with ID 500 and the function
 * will still return TRUE; i.e. it errs on the side of safety and doesn't go to
 * extremes to check the paths, but will recognize any special Drupal path type
 * so it doesn't get unlinked as "invalid".
 *
 * @param $path
 *   the path that we are checking to see if it's in the menu_router table).
 *
 * @return
 *   TRUE or FALSE, depending on whether or not the path is listed in
 *   the menu_router table.
 */
function intlinks_is_special_path($path) {
  // Change any numeric parts of the path to % placeholders.
  // This is the format used in the menu_router table. The % symbols take the
  // place of term ids, node ids, etc.
  $path_parts = explode('/', $path);
  foreach ($path_parts as $i => $part) {
    if (ctype_digit($part)) {
      $path_parts[$i] = '%';
    }
  }
  $path = implode('/', $path_parts);
  // Check $path for a match in the menu_router table.
  return (bool) db_query("SELECT fit FROM {menu_router} WHERE path = :path",
  array(':path' => $path))->fetchField();
}

/**
 * Processes regex matches of links for intlinks_hide_bad_filter_process().
 *
 * If no changes should be made (e.g. if link is external or is to a published
 * node), the function simply returns $matches[0], the unchanged link.
 *
 * However, if $matches[1] isn't a published node, function returns
 * $matches[2], the text (and any other tags) between the <a> tagset.
 *
 * Example link $matches array for external link:
 * [0]: <a href="http://jetlag.info"
 *         title="Good info about jetlag"><em>jetlag</em></a>
 * [1]: http://jetlag.info             [2]: <em>Jetlag</em>
 *
 * Example link $matches array for internal link with URL alias:
 * [0]: <a href="/articles/random-content.html">random article</a>
 * [1]: /articles/random-content.html  [2]: random article
 *
 * Example link to internal "normal Drupal path" Note: "#anchor" is possible
 * and should be dealt with before looking up nodes.
 * [0]: <a href="/node/2#jetlag" title="Read more about jetlag">jetlag</a>
 * [1]: /node/2#jetlag                 [2]: jetlag
 *
 * @param $matches
 *   Parameter passed by intlinks_hide_bad_filter_process();
 *   an array from regex match of the link.
 *
 * @return
 *   Original link, or link stripped of <a> tagset, if it seems visitor would
 *   have got a "page not found" error.
 */
function _intlinks_process_bad_link($matches) {
  if ($matches[1][0] != "/") {
    // Do nothing if the link is not internal (root relative).
    return $matches[0];
       // All links *should* be root-relative.
       // If you want this filter to ignore a link, make it "absolute". ;-)
       // @TODO: Possibly add handling for document-relative links.
  }
  //Strip the leading slash from link path to look up.
  $path = ltrim($matches[1], '/');
  $path_parts = explode('#', $path);
   // We don't want the "anchor", if there is one.
  $path = $path_parts[0];
  // We also don't want any arguments or queries for special paths.
  $path_parts = explode('&', $path);
  $path = $path_parts[0];
  $path_parts = explode('?', $path);
  $path = $path_parts[0];
  if (strpos($path, 'node/') === FALSE) {
    $node_path = drupal_get_normal_path($path);
    if ($path != $node_path) {
      // Drupal successfully got a node path from $path (the URL alias)
      $nid = ltrim($node_path, 'node/');
      if (intlinks_is_published($nid)) {
        // Return link, unchanged.
        return $matches[0];
      }
      else {
        // Remove link from text if node isn't published.
        return $matches[2];
      }
    }
    else {
      // Attempt to look up a node path for the alias failed.
      // Look in the menu_router table and see what we find there.
            if (intlinks_is_special_path($path)) {
            // Return the link, unchanged; it's a special Drupal path or Views
            // display path that's linked; we don't want to remove those links.
              return $matches[0];
            }
      // Otherwise hide the link; it will most likely not impress visitors.
      // @TODO: Possibly log such links (once) and pages where they are found
      // to assist with troubleshooting and site maintenance.
      return $matches[2];
    }
  }
  else {
  // We have a path that might be a node path. Test it.
    if ($nid = intlinks_is_node_path($path)) {
      if (intlinks_is_published($nid)) {
        return $matches[0];
      }
      else {
        return $matches[2];
      }
    }
    else {
      if (intlinks_is_special_path($path)) {
      // Return the link, unchanged; it's a special Drupal path or Views
      // display path that's linked; we don't want to remove those links.
        return $matches[0];
      }
      // Otherwise hide the link.
      return $matches[2];
    }
  }
}
