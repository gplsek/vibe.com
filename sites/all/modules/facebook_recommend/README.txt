About
============
This is a simple module that adds a block with a few options to help you easily add the facebook recommend/like button to whatever content you desire.

Installation
============
1. Unzip/untar the module folder.
2. Copy the module folder into your <your drupal install>/sites/all/modules folder.
3. Navigate to admin/build/modules/list and enable the module. It will be listed under "Other".

Configuration
============
1. Navigate to admin/build/block/list and scroll down to the "Disabled" area. Click configure on the "Facebook recommend block".
2. Beyond the usual settings you also have "Show faces" and "Like or Recommend button". 
	"Show faces" is by default set to yes and will show the profile picture of the individuals who like/recommend your content. The faces appear below the like/recommend link.
	"Like or Recommend button" is by default set to recommend and just changes whether it says "John Smith likes <title> | <site>" vs "John Smith recommends <title> | <site>".
3. Set your title if you wish to have one and what pages you would like the link to appear on.
4. Press "Save Block" at the bottom of the page and it will take you back to the main block configuration page.
5. Click and drag your block to the area you would like it to be shown on your pages.
6. Press "Save Block" at the bottom of the page. Your readers are now ready to start recommending/liking your content!

If you require further assistance setting up or configuring the block check the block help here:
http://drupal.org/getting-started/6/admin/build/block

Known Issues
============
If facebook can not get to your link without a login it will not work. The like/recommend button will only work on pages that an anonymous user can view.