
NODE GALLERY MODULE
-------------------


CONTENTS
-------- 
 * Description
 * Features
 * Requirements
 * Contributed Modules
 * Un-Install Notes
 * Items Created at Install
 * Installation
 * Create a Gallery
 * Edit a Gallery
 * Additional Configurations
 * Edit a Gallery Relationship and Configurations
 * Create a Gallery Relationship
 * Handbook Links
 * Drupal Links
 * Credits
 

DESCRIPTION
-----------
  The node gallery image management system provides administrators with a
  flexible and powerful image gallery. Administrators can designate the content
  type to be used for the "Gallery" node, and the content type to be used for
  the "Image" node.  Since the "Image" and the "Gallery" are both node or
  content types, they can use many of Drupal's contributed modules to enhance
  the galleries.  Upon installation, the required settings are automatically
  created so users can start posting galleries right away. Website users can
  upload "Images" into a "Gallery", edit them within the "Gallery" and delete
  them if needed.


FEATURES
--------
 - Naturally integrated with Drupal Taxonomy, Comments, Path, Pathauto, 
   Views, Token, VotingApi, Fivestar, CCK(Content Construction Kit),
	 Custom breadcrumbs, ImageCache, ImageAPI, Nodewords
	 and many other contributed modules.
 - Use Drupal's core batch system to do operations such as uploading, editing
   etc.
 - Theme or customize galleries with CSS and tpl.php files included with the
   system. 
 - Everything you can do with nodes you can do to your gallery and images.
 - Unlimited user albums
 - Set image size, maximum number of images and upload limits for each Drupal
   role.
 - Set image storage location and use custom tokens to help if desired. 
 - 4 custom tokens included to enhance and control the galleries.
 - Lightbox2 contributed module integration with lighbox2 slide show.
 - Titles, Descriptions, Captions and Image counts.
 - Drag handle image sorting and field sorting for submission forms.
 - Previous/Next pagers


REQUIREMENTS
------------
 * ImageCache module	http://drupal.org/project/ImageCache
 * ImageAPI module	http://drupal.org/project/ImageAPI


CONTRIBUTED MODULES
-------------------
 * Lightbox2 module	http://drupal.org/project/lightbox2
	 
ADD-ON MODULES
-------------------
 * Node_gallery_access module http://drupal.org/project/node_gallery_access
	 Control gallery access to public, private or password.
	

IMPORTANT UN-INSTALL NOTES
--------------------------
  The three ImageCache presets created by the Node Gallery module
  will be deleted if you disable the Node Gallery module, so it is recommended
  that you only use these presets with Node Gallery. However, if you modify the
  Node Gallery presets, they will be saved into the system and continue to exist
  without regard to the Node Gallery module being enabled.


ITEMS CREATED AT INSTALL TIME
-----------------------------
  1. Two node types; one for galleries and one for gallery images.
  
  2. Three ImageCache presets; for cover, display, and thumbnail.
  
  3. One node gallery relationship; linking the gallery node with the
     gallery image nodes.


INSTALLATION
------------
  1. Download node_gallery, ImageAPI, and ImageCache.
  
  2. Unzip or unpack the modules on your computer.
  
  3. Upload the module folders with all the contents (as is) to the
  sites/all/modules folder.
  
  4. Goto: Administer > Site configuration > Clean URLs
  Enable clean URLs (required for ImageCache to work).
  
  5. Goto: Administer > Site building > Modules
  Enable Node Gallery, ImageAPI, ImageAPI GD2, ImageCache, ImageCache UI,
  Node Gallery Lightbox2 Integration (if needed), and Node Gallery Access (if
  needed).
  
  6. Save the settings to install (updates the database).
  
  7. Goto: Administer > User management > Permissions
  Configure the Permissions for:
  -ImageCache module- 
      view ImageCache node-gallery-cover 
      view ImageCache node-gallery-display 
      view ImageCache node-gallery-thumbnail
  -node module-
      access content 
      administer nodes(use with caution) 
      create node_gallery_gallery content 
      create node_gallery_image content 
      delete any node_gallery_gallery content 
      delete any node_gallery_image content 
      delete own node_gallery_gallery content 
      delete own node_gallery_image content 
      edit any node_gallery_gallery content 
      edit any node_gallery_image content 
      edit own node_gallery_gallery content 
      edit own node_gallery_image content
  - node_gallery module -
      view node gallery  
  
  At this point, users can create galleries.


CREATE A GALLERY
----------------
   1. Goto: Create content > Gallery

   2. Name the gallery

   3. Provide a description for the gallery if desired.

   4. Click "Save"

   5. Click "Upload some images!"

   6. Browse and choose images.

   7. Click "Submit Files"

   8. Add a caption to each image if desired.

   9. Choose a cover image using the radio buttons on the right.

  10. Drag sort the order of images.

  11. Click "Submit"

  12. View your gallery.


EDIT A GALLERY
--------------
   1. Login and Goto: your gallery

   2. Click: Edit gallery, Upload images, Manage images or Delete Gallery


ADDITIONAL CONFIGURATIONS
-------------------------
   Configurations for a gallery relationship.
	(See the next section below, Edit a gallery relationship.)
	
   Configure the number of Albums on gallery list pages and the
 	number of images on each thumbnail page.
	Goto: Administer > Site configuration > Node gallery > Common settings

   Configure Gallery or Album submission form.
	Goto: Administer > Content management > Content types > Edit gallery

   Configure Gallery Image submission form.
	Goto: Administer > Content management > Content types > Edit gallery image

   Configure node gallery access.
	Goto: Administer > Site building > Modules > List
	Enable Node Gallery Access module to use this function.
		
   Customize galleries with css, views or tpl.php files.
	The node gallery handbook has more information about views and css.

   Customize paths and/or URLs with 4 custom node_gallery Tokens.
	[parent-node-gallery-path]	The path of the parent Gallery. 
	[parent-node-gallery-path-raw]	Unfiltered path of the parent Gallery.
	[parent-node-gallery-title]	The title of the parent Gallery. 
	[parent-node-gallery-title-raw]	Unfiltered title of the parent Gallery.
		
   Customize the ImageCache presets.
	Goto: Administer > Site building > ImageCache > List

	Add new, Configure, Edit and/or Delete the Presets as desired for
 	each part of the gallery. Add new presets or reuse existing
 	presets when Adding Gallery Relationships.

   Customize Lightbox settings.
	Goto: Administer > Site configuration > Lightbox2	

   GD2 image quality setting.
	Goto: Administer > Site configuration > ImageAPI > Configure


EDIT A GALLERY RELATIONSHIP AND/OR CONFIGURATIONS
-------------------------------------------------
  1. Goto: Site configuration > Node gallery > List
  	Click "Edit" next to the gallery relationship which will open the
 	first configuration page.

  2. The first configuration page has the content type settings.
	Select the content types to be used for the "Gallery" and the
 	"Image" types. 

	To edit the gallery configurations, go to the
	bottom of the page and click next. Clicking "Next" will
	open the second configuration page.
	
  3. The second configuration page has the detailed gallery
 	configurations.
	
	Gallery Type Name:
		The name to show on create content page.
	
	Gallery Directory:
		Set the directory to hold images inside sites/default/files
		 - sites/default/files/[name or token]/[name or token] -

	Default Cover Image:
    		Specify a default cover image to show when there
		 are no images in the gallery.
	
	Number of uploads:
    		Select the number of upload buttons(fields)
		for images on each upload page. Use caution when
		adding more than 4 or 5 as php.ini settings may
		need to be changed.

	Fields:
    		Specify the fields to show(check boxes).
    		Title, caption, revision info, comment settings, path settings.

	Choose the content you want to display:
    		Specify the description text to be used when displaying image pages.
    		Choose gallery description (or) image description.

	Image Comment Setting:
    		Specify the comment setting to use. (Image or gallery)
		 as per setting in the content type.

	Images Sizes:
    		Select which ImageCache Preset to use for each
		 image display(cover, thumbnail, display).
	
	Original Image Display:
    		Set the option to allow viewing full sized image
		 disabled, with a link or lightbox2 pop-up.

	Teaser setting:
    		Specify how to display the gallery and image
		 when viewing the node as a teaser.
	
	Gallery Landing Page Setting:
    		Choose thumbnails or thumbnails that open a lightbox2 gallery.
	
    -- Image Upload Limitation Default Settings: --

	Default permitted file extensions:
    		Specify allowed file types ( jpg jpeg png )
		 (do not use a dot, just a space between each)

	Maximum resolution for uploaded images:
    		Specify (width x height) in pixels. (use 0 for no limit,
		 if there is trouble with uploads check this)

	Default maximum file size per upload:
    		Specify a number in MB (calculated after 
		ImageCache Preset re-sizes the files)

	Default total file size per user:
    		Specify a number in MB (combined total size of all
		 files a user is allowed to have in the system)

	Default total file number per user:
    		Specify a number. (combined total number of files
	  	each user is allowed to have in the system)

   -- Image Upload Limitation Settings for user roles: --
		note: This will not be visible until permissions have been set.
		Each user role will have the following settings.

       Default maximum file size per upload:
          	Specify a number in MB (calculated after ImageCache
		 Preset resize's the files)

       Default total file size per user:
          	Specify number in MB (combined total size of all files
		 a user is allowed to have in the system)

       Default total file number per user:
          	Specify a number (combined total number of files each user
		is allowed to have in the system) 	
	

	SUBMIT AFTER CHANGING SETTINGS


CREATE A NEW GALLERY RELATIONSHIP
---------------------------------
  1. Create a content type for the gallery 
	 	WARNING: do not re-use a gallery content type,
	 	as it will over write the relationship that uses it.

  2. Create an image content type -or- plan to re-use an existing one.

  3. Create 3 image cache presets or plan on re-using existing ones.
	 	one for cover, thumbnail and image.

  4. Set permissions for the new and/or re-used content types.

  5. Set permissions for the 3 new and/or re-used ImageCache presets.

  6. Goto: Administer > Site configuration > Node Gallery > Add a
 	Gallery Relationship

  7. Select the gallery and image content types. Click "Next"

  8. Configure the new gallery relationship. (as in the above section)

  9. The Image content type needs to be removed from the create content list.
	Goto: Administer > Site building > Menus
	Click on the Navigation menu to open it.
	Locate the Image content type and un-check it and "Save" 
 
   A new custom made node gallery has been created.


HANDBOOK LINKS
--------------
 Node Gallery Handbook		http://drupal.org/node/544050

 Install Node Gallery		http://drupal.org/node/544242

 Create & Edit a Gallery	http://drupal.org/node/544530

 Configure Node Gallery		http://drupal.org/node/544642

 Add a Gallery Relationship	http://drupal.org/node/544710

 Style, Views & Theming		http://drupal.org/node/544748

 FAQ for Node Gallery		http://drupal.org/node/544760

 Trouble shooting node_gallery	http://drupal.org/node/584170

 Websites Using Node Gallery	http://drupal.org/node/547788


DRUPAL LINKS
------------
 Project Page	http://drupal.org/project/node_gallery

 Drupal Forum	http://drupal.org/forum/22

 Issue Queue	http://drupal.org/project/issues/node_gallery 


CREDITS
-------
 Authored by Wilson Wu(wilson98); < martic98@gmail.com >

 Co-maintained by Kevin Montgomery(Kmonty); New Eon Media < http://neweonmedia.com >


END OF FILE
-----------