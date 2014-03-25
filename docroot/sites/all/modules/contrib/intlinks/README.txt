
CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Installing
 * Uninstalling
 * Frequently Asked Questions (FAQ)
 * Known Issues
 * More Information
 * How Can You Contribute?

INTRODUCTION
------------
Maintainer and Author: Lowell Montgomery (LoMo) <http://drupal.org/user/628196>
This module was written as a part of my internship as a Drupal developer with 
Cocomore <drupal.cocomore.com>. Thanks are due to Cocomore's senior developers,
especially Markus Kalkbrenner and Christian Spitzlay, who provided invaluable
guidance and support to help ensure my first module worked as conceived.

The Internal Links module, Intlinks, is a simple set (currently two) of input
filters which modify the output of internal, root-relative links in any text
input format for which they are enabled. 

The Internal Links Title filter adds HTML titles to links to other nodes on the
site (if no title attribute already exists) using the linked node's title as the
HTML title attribute's value.

It also replaces href values which are in the "normal Drupal path" form
(e.g. <a href="/node/123">) with a URL alias, if one exists. It leaves any
other attributes alone. It is especially useful for sites which have
a lot of related content which can more easily be cross-linked, with less effort
on the part of editorial staff. And if "normal Drupal paths" are used, it will
also maintain any links if the URL aliases are modified to (for example) improve
SEO or correct a typo.

The Internal Links "hide bad" filter could probably be better named. Its main
purpose is to remove internal links to nodes which aren't (yet or any longer)
published or existent to avoid users seeing "page not found" errors when follow-
ing links on your site. It checks URL aliases and/or normal Drupal paths to see
if the node exists and checks whether the node is published, if so. If it finds
no node under the linked URL, it also checks the "menu_router" table for non-
node paths which could also be linked (e.g. a path to a Views page). It only
removes internal, root-relative links which are not found as nodes or other
special paths and/or are unpublished. If a link is removed, the anchored text
is left in place, only the <a></a> tagset (and any attributes within) are removed.
The filter, by design, attempt to err on the side of leaving links alone, since
otherwise its effect could be problematic. We suggest turning it on only if you
know you have links to unpublished content, especially content you'll be making
public over time, or if there are many old links (to unpublished content) which 
you don't have time to more permanently remove.

Content should consist of reasonably well-structured HTML with double-quotes
around HREF values, and with all internal links "root-relative", i.e. starting
with a slash, ('/'), followed by the normal Drupal path (best) or path alias.
I could have used regular expressions which would allow just about any HTML a
browser will render, but that adds to processing and we should all be using
nicely written HTML. Normal output from the common WYSIWYG text editors
(CKEditor or TinyMCE, etc) should work fine.

INSTALLING
----------
See http://drupal.org/getting-started/install-contrib for instructions on
how to install or update Drupal modules.

Once the Internal Links module is installed and enabled, you can go to your
"Text formats" page (/admin/config/content/formats) to configure any text
format (filtered HTML/Full HTML, etc) to use either or both of the included
filters. Of course it's most efficient to have the "hide bad" links filter
first, so that processing is not wasted on adding titles to any links that
are to be removed. The "convert URLs to links" filter, included in core,
should ideally come after both of these, since it's only for "absolute" links
(normally external) and the current regular expressions capture all HREF values.

Other input filters may need to come before those in this module if you want
their output to receive filtering by intlinks filters. These filters would
include ones written to allow use of Markdown or BBCode. Their output is normal
HTML which should allow the intlinks filters to do their job.

You need to have clean URLs enabled for this module to function. It
is not designed to work without them.


UNINSTALLING
------------
Currently the Internal Links module does not write anything to the database
so there is no need to run update.php after installing/uninstalling the module.
If you turn it off and remove it from your modules folder, it's gone without
any trace of its existence (at least after any content caches are cleared.)


FREQUENTLY ASKED QUESTIONS (FAQ)
--------------------------------
There are no frequently asked questions at this time. Feel free to change
that if you have any questions. ;-)


KNOWN ISSUES
------------
Intlinks needs improvements for multilingual sites and this is taking place in the
7.x-2.x branch. If you have a multilingual site, you should try this branch, however
it does not yet support field translation (each node requires a language). Ideally,
we would like to support all forms of language negotiation as well as field translation
on sites which use the Title module to provide translation of the title field.

This module is really made for links to nodes, so may not get titles for non-node paths,
however we have plans for how to deal with this, as well, so this should be improved.


MORE INFORMATION
----------------
To issue any bug reports, feature or support requests, see the module's issue
queue at http://drupal.org/project/issues/intlinks.


HOW CAN YOU CONTRIBUTE?
-----------------------
- Report any bugs, feature requests, etc. in the issue tracker, and/or write
  patches if your coding skills are up to it and you have the time.
  http://drupal.org/project/issues/intlinks

- Help translate this module.
  http://localize.drupal.org/translate/projects/intlinks

- Write a review for this module at drupalmodules.com.
  http://drupalmodules.com/module/intlinks
