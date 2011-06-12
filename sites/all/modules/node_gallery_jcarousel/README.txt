$Id: README.txt,v 1.1.2.1 2010/12/23 03:20:55 justintime Exp $

Summary
-------

Node Gallery jCarousel is a module that combines two other powerful modules
(Node Gallery and jCarousel) to provide an out-of-the-box solution for paging
through a gallery of nodes.

The module contains a predefined view along with some CSS and theming to provide
a site administrator an easy way to give his/her users more flexibility when
paging through image nodes. The pager uses AJAX, so no matter how big your
gallery is, the carousel will only need to render at most an unordered list of 7
imagecache presets.

Installation
------------
 * Copy the module file to your modules directory, extract it, and enable it at
/admin/build/modules.
 
 * Visit your Node Gallery Relationships at admin/settings/node_gallery/list and
click "Change Settings" on each relationship you wish to enable the jCarousel
on.

 * Check the box labeled "Enable jcarousel on image node pages?", select the
view you wish to use (select 'jcarousel' if you don't know), and press save.

 * At this point, you're most likely done.  Vist a gallery image page, and
and you should immediately see a jCarousel above your images.