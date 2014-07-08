<?php

/**
 * @file
 * Overrides modules/search/search-result.tpl.php.
 *
 * Variables provided by the Rich Snippets module:
 * - $url_shortened: The shortened URL of the result.
 * - $alternate_links: Alternate links to the result, such as links to a cached
 *   version of the page or a link to similar results.
 * - $image:
 *
 * Available variables:
 * - $url: URL of the result.
 * - $title: Title of the result.
 * - $snippet: A small preview of the result. Does not apply to user searches.
 * - $info: String of all the meta information ready for print. Does not apply
 *   to user searches.
 * - $info_split: Contains same data as $info, split into a keyed array.
 * - $module: The machine-readable name of the module (tab) being searched, such
 *   as "node" or "user".
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Default keys within $info_split:
 * - $info_split['type']: Node type (or item type string supplied by module).
 * - $info_split['user']: Author of the node linked to users profile. Depends
 *   on permission.
 * - $info_split['date']: Last update of the node. Short formatted.
 * - $info_split['comment']: Number of comments output as "% comments", %
 *   being the count. Depends on comment.module.
 *
 * Other variables:
 * - $classes_array: Array of HTML class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $title_attributes_array: Array of HTML attributes for the title. It is
 *   flattened into a string within the variable $title_attributes.
 * - $content_attributes_array: Array of HTML attributes for the content. It is
 *   flattened into a string within the variable $content_attributes.
 *
 * Since $info_split is keyed, a direct print of the item is possible.
 * This array does not apply to user searches so it is recommended to check
 * for its existence before printing. The default keys of 'type', 'user' and
 * 'date' always exist for node searches. Modules may provide other data.
 * @code
 *   <?php if (isset($info_split['comment'])): ?>
 *     <span class="info-comment">
 *       <?php print $info_split['comment']; ?>
 *     </span>
 *   <?php endif; ?>
 * @endcode
 *
 * To check for all available data within $info_split, use the code below.
 * @code
 *   <?php print '<pre>'. check_plain(print_r($info_split, 1)) .'</pre>'; ?>
 * @endcode
 *
 * @see template_preprocess()
 * @see template_preprocess_search_result()
 * @see template_process()
 */
?>
<li class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <div class="search-result-wrapper">
    <?php print render($title_prefix); ?>
    <h3 class="title"<?php print $title_attributes; ?>>
      <a href="<?php print $url; ?>"><?php print $title; ?></a>
    </h3>
    <?php print render($title_suffix); ?>
    <div class="search-result-url">
      <cite><?php print $url_shortened; ?></cite>
      <?php if (!empty($alternate_links)): ?>
        <span><?php print $alternate_links; ?></span>
      <?php endif; ?>
    </div>
    <div class="search-result-snippet-wrapper">
      <?php if ($image): ?>
        <div class="search-result-images">
          <?php print $image; ?>
        </div>
      <?php endif; ?>
      <?php if ($info): ?>
        <div class="search-result-snippet-info"><?php print $info; ?></div>
      <?php endif; ?>
      <?php if ($snippet): ?>
        <span class="search-result-snippet"<?php print $content_attributes; ?>><?php print $snippet; ?></span>
      <?php endif; ?>
    </div>
  </div>
</li>
