<?php

/**
 * @file
 * The functions in this file are used by the Internal Links filter as part of
 * the process of the handling the "title filter". Moving some of the functions
 * to separate files helps to streamline the code, especially for users who are
 * only using one of the filters and makes it more "ready for the future", when
 * additional filters could be added to this set.
 *
 * @author Lowell Montgomery | Cocomore AG
 *   @see http://drupal.org/user/628196
 */

include_once 'intlinks_common_functions.inc';

/**
 * More efficient way to get node titles than loading nodes with node_load().
 *
 * @param $nid
 *   The node id corresponding to the node title this function returns.
 *
 * @return
 *   The node title corresponding to the node id passed, or FALSE if query fails.
 *   Note that the "FALSE" return value comes from the db_query function, which
 *   this function calls to get the title.
 */
function intlinks_get_node_title_from_nid($nid) {
  $title = db_query("SELECT title FROM {node} WHERE nid = :nid",
  array(':nid' => $nid))->fetchField();
  return $title;
}

/**
 * Process regex matches of links for intlinks_title_filter_process().
 *
 * If no changes should be made (e.g. if link is external or already has a
 * title), the function simply returns $matches[0], the unchanged link.
 *
 * Example link $matches array for link with no title (external):
 * [0]: <a href="http://www.newsoftheweird.com/archive/index.html">
 * [1]: href="http://www.newsoftheweird.com/archive/index.html"
 * [2]: http://www.newsoftheweird.com/archive/index.html
 *
 * Example link $matches array for link with no title (internal):
 * [0]: <a href="/articles/random-content">
 * [1]:  href="/articles/random-content"  [2]: /articles/random-content
 *
 * Example link with existing title (internal)
 * [0]: <a href="/node/2" title="Weird News Article - Node 2 - Custom Title">
 * [1]: href="/node/2" title="Weird News Article - Node 2 - Custom Title"
 * [2]: /node/2
 *
 * @param $matches
 *   Parameter passed by intlinks_title_filter_process();
 *   an array from a regex match to an HTML link.
 *
 * @return
 *   Original link, possibly modified to include HTML title="value" attribute
 *   and/or URL path alias if link passed was, e.g. <a href="/node/123">.
 */
function _intlinks_title_process_link($matches) {
  if (($matches[2][0] != "/") && ($matches[2][0] != '.')) {
    // Do nothing if the link is not internal (root rel or doc rel).
    return $matches[0];
    // All links *should* be root relative.
  }
  $has_query = FALSE;
  $trimmed_path = ltrim($matches[2], "/");
  if (strpos($trimmed_path, '?')) {
  	$has_query = TRUE;
  	$path_parts = explode('?', $trimmed_path);
  }
  else {
  	$path_parts = explode('#', $trimmed_path);
  }
  // If an HTML title attribute already exists...
  if (strpos($matches[1], 'title=') !== FALSE) {
     // and if the link is not to a "normal Drupal path" (path alias exists)...
    if (strpos($matches[2], '/node/') != 0) {
       // Do nothing if we already have a path alias and title.
      return $matches[0];
    }
    // Deal with links to normal Drupal paths which already have a title.
    // Change the Drupal path to a path alias, if one exists.
    elseif ($matches[2][0] == '/') {
       // The 2nd part of a normal Drupal path is the node id.
      if ($node_id = intlinks_is_node_path($path_parts[0])) {
        $node_path = 'node/' . $node_id;
         // Get alias, if one exists; otherwise, keep path as-is.
        $path = drupal_get_path_alias($node_path);
        $insert_path = '/' . check_plain($path);
        // Exploding around '#' produces 2-element array for paths w/ anchors.
        if (count($path_parts) == 2) {
          $anchor = $path_parts[1];
          $insert_path .= '#' . $anchor;
        }
        // Insert the path alias into the link and return
        return preg_replace('%href="([^"]+?)"%', 'href="' .
          $insert_path . '"', $matches[0]);
      }
      // Path starts with 'node/', but isn't a proper Drupal "normal path".
      // Return such links, unchanged.
      return $matches[0];
    }
    else {
       // No handling for document-relative links (for now).
       // @TODO: Placeholder to possibly add such handling.
      return $matches[0];
    }
  }
  else {
    // NO TITLE attribute exists.
    // Add title attribute to the link; then return the full link.
    // We need to get the node that the link refers to (if it's a node).
    if (strpos($matches[2], '/node/') === FALSE) {
      // Handle links which already have a path alias.
      // If the link is a normal, root-relative link, starting with "/":
      if ($matches[2][0] == '/') {
        $path_alias = $path_parts[0];
        // Calls to drupal_get_normal_path() return e.g. node/3 .
        $node_path = drupal_get_normal_path($path_alias);
        if (strpos($node_path, 'node/') === FALSE) {
          // The link must be an internal system path, views path, etc.
          // The link is not a node!! Keep the original link, unchanged.
          // @TODO: Get titles of any Views paths (e.g. page displays).
          return $matches[0];
        }
        // The 2nd part of a normal Drupal path is the node id.
        $node_id = ltrim($node_path, 'node/');
        $node_title = intlinks_get_node_title_from_nid($node_id);
        if ($node_title === FALSE) {
          // Terminate -- something is wrong; all nodes should have titles.
          return $matches[0];
        }
        if (count($path_parts) == 1) {
          // There is no anchor in the link.
          // Insert the node title and return the new full link.
          // substr_replace('eggs','x',-1,-1); //eggxs  (Example from PHP API).
          return substr_replace($matches[0], ' title="' .
            check_plain($node_title) . '"', -1, -1);
        }
        else {
          // There is an anchor we want to add to the node title.
          // We will leave out adding the "anchor" text to the HTML title,
          // but leave the code here to modify as an "option".
          // @TODO: Optionally include any "anchor text" with HTML title value.
          // $anchor = ' (' . ucwords($path_parts[1]) . ')';
          $insert_link = ' title="' . check_plain($node_title) . '"';
          // And we need to insert the node title to return the new full link.
          // substr_replace('eggs','x',-1,-1); //eggxs  (Example from PHP API).
          return substr_replace($matches[0], $insert_link, -1, -1);
        }
      }
      else {
         // No handling for document-relative links. Return original link.
         // @TODO: Possibly add handling for document-relative internal links.
        return $matches[0];
      }
    }
    else {
     // The link is propably a 'node/2' -type path (normal Drupal path).
     // But, better safe than sorry: Give it a proper test.
      if ($node_id = intlinks_is_node_path($matches[2])) {
        $node_path = 'node/' . $node_id;
        $path = drupal_get_path_alias($node_path);
        $node_title = intlinks_get_node_title_from_nid($node_id);
        if ($node_title === FALSE) {
          // Terminate. Something is wrong; all nodes should have titles.
          return $matches[0];
        }
        $insert_title = ' title="' . check_plain($node_title) . '"';
        $insert_path = '/' . check_plain($path);
        // There is an anchor or query if $path_parts array has 2 elements.
        if (count($path_parts) == 2) {
          if (!$has_query) {
          	// $path_parts[1] is the anchor.
          	$insert_path .= '#' . $path_parts[1];
          }
          else {
          	// $path_parts[1] is a query, possibly with an anchor.
          	$insert_path .= '?' . $path_parts[1];
          }
        }
        $return_link = preg_replace('%href="([^"]+?)"%', 'href="' . $insert_path
          . '"', $matches[0]);
        return substr_replace($return_link, $insert_title, -1, -1);
      }
      else {
        // The link has 'node/' in its path, but it's not a node view.
        return $matches[0];
      }
    }
  }
}
