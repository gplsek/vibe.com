
-- NODE GALLERY SLIDESHOW --

SUMMARY
-------
  The node_gallery_slideshow module extends the Node Gallery module 3.x branch
  with a simple jquery based slideshow. It uses the API of node gallery to retrieve
  images and their related information and is intended to be an easy-to-install
  and integrated solution specifically for node gallery. You don't need to create
  or adapt Views, or download additional jquery libraries. Everything required
  is contained in the project, and the code is small and concise. After enabling
  the module, Gallery nodes will show an additional tab for slideshows, Image nodes
  will contain a link in the Node Gallery navigator to start a slideshow starting
  at this node. Because this module is integrated with Node Gallery, it does
  not require that all images are already attached to the DOM, like most other
  slideshow modules. It retrieves the image information per JSON requests and creates
  and attaches the HTML elements on demand, keeping memory requirements small
  for the client. If desired, the module can preload a specified number of images.

  For a full description of the module, visit the project page:
    http://drupal.org/project/node_gallery_slideshow

  To submit bug reports and feature suggestions, or to track changes:
    http://drupal.org/project/issues/node_gallery_slideshow


REQUIREMENTS
------------
  * Node Gallery 3.x   http://drupal.org/project/node_gallery
  * jQuery UI          http://drupal.org/project/jquery_ui

INSTALLATION
------------
  * Install as usual, see http://drupal.org/node/70151 for further information.

CONFIGURATION
-------------
  * Configure user permissions in Administer >> Site configuration >> Node Gallery >>
    Slideshow settings:

    - Initial interval time:
      This is the time after which the next image will be shown if the slideshow
      is running. Users can modify this themselves, but this is the default value.

    - Number of images to preload:
      The number of images that will be cached in the clients browser. Trades
      loading time for memory on the client.

CUSTOMIZATION
-------------
  * To customize the look of the slideshow, you can override the template
    node-gallery-slideshow.tpl.php. Also have a look at the included stylesheet.
    The default markup offers IDs for all fundamental parts of the slideshow, e.g.:
    #node-gallery-slideshow (a div around the whole slideshow)
    #node-gallery-slideshow-controls (the <ul> containing the controls).

CONTACT
-------

  Maintainers:
  * Andre Gemuend (scroogie) - http://drupal.org/user/35351
