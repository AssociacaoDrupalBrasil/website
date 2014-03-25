<?php

/**
 * @file
 * This file includes functions which are used in more than one of the filters
 * included in the Internal Links (intlinks) module.
 *
 * @author Lowell Montgomery | Cocomore AG
 *   @see http://drupal.org/user/628196
 */

/**
 * Tests a path to see if it is a proper "normal Drupal path": node/[nid].
 *
 * It also does some basic clean-up of the path internally, to save this being
 * done in every block that looks at possible "normal Drupal paths".
 *
 * @param $path
 *   A path passed to the function.
 *
 * @return
 *   The node ID (does not test to see if the node is real or published) or
 *   FALSE, if the "node path" is actually a node/add, node/[nid]/edit, etc
 *   -type path (special system path related to nodes, but not actually to a
 *   node.
 */
function intlinks_is_node_path($path) {
  if (strpos($path, 'node/') === FALSE) {
    return FALSE;
  }
  // Trim any leading slash from original link.
  $path = ltrim($path, '/');
  // This function should also deal with the possibility that it was passed a
  // path that still has a "anchor" text appended to it. The part
  // after the slash should all be numeric for the next test.
  $path_parts = explode('#', $path);
  $path = $path_parts[0];
  // We also want to omit any queries.
  $path_parts = explode('?', $path);
  $path = $path_parts[0];
  // Now the path is trimmed of anything extraneous.
  $path_parts = explode('/', $path);
  if ((count($path_parts) == 2) && ctype_digit($path_parts[1])) {
    // $path_parts[1] is the node id.
      return $path_parts[1];
  }
  // Otherwise we have some kind of path that includes '/node', but is not
  // a real node path. Maybe a node edit/add or other special system path.
  else {
    return FALSE;
  }
}
