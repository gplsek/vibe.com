$Id: README.txt,v 1.1.4.2 2008/10/17 14:27:03 swentel Exp $

Description
-----------
This module makes a javascript toolbox action available thanks to the power
of Imagecache 2. It can currently 'hook' into several modules by adding
a 'javascript crop' link on the edit forms of supported modules and/or fields. 
The popup window will display all available imagecache presets with
a javascript crop action. In your theming you can use the imagecache theme function with
a preset. The imagecache action will make a database call to choose the right crop area.

The main difference with projects like eyedrop or imagefield_crop is that it doesn't 
provide it's own widget to upload images, instead it just 'hooks' into image modules/fields.

Everyone is invited to submit patches for more module support. I might
even give some people cvs access.

Supported modules
-----------------
image : Link is underneath the thumbnail on the edit page.
node_images : Underneath the thumbnail. Read on to implement a theme override.
imagefield : On node edit form. Previews & multiple values not supported (yet).

Installation
------------
You need imagecache 2, imageapi and jquery_ui
After you enable the module, you can go to admin/settings/imagecrop and enable 
support for modules & fields to display a link.

A cron task cleans up the imagecrop table clearing records from files and/or presets
which do not exist anymore, so make sure cron is running.

Extra Coolness
--------------
If you have the thickbox module installed, the popup isn't used but the thickbox overlay.

Specific installation instructions for node_images module
---------------------------------------------------------
You need to paste the code underneath in the template.php file of your theme.
This makes sure the javascript link is available for the thumbnail.

/**
 * Theme function override of node_images to support imagecrop module
 */
function phptemplate_node_images_list($form) {
  $header = array(t('Thumbnail'), t('Description').' / '.t('Path'), t('Size'), t('Weight'), t('Delete'));
  
  $rows = array();
  foreach($form['images']['#value'] as $id=>$image) {
    $row = array();
    if (isset($form['imagecrop']) && module_exists('thickbox'))
    $row[] = '<img src="'.file_create_url($image->thumbpath).'" vspace="5" /><br /><a class="thickbox" href="' . url('imagecrop/showcrop/'. $id .'/0/node_images', NULL, NULL, TRUE) . '?KeepThis=true&TB_iframe=true&height=600&width=700">'. t('Javascript crop') .'</a>';
    elseif (isset($form['imagecrop']) && !module_exists('thickbox'))
    $row[] = '<img src="'.file_create_url($image->thumbpath).'" vspace="5" /><br /><a href="javascript:;" onclick="window.open(\''. url('imagecrop/showcrop/'. $id .'/0/node_images', NULL, NULL, TRUE) .'\',\'imagecrop\',\'menubar=0,resizable=1,width=700,height=650\');">'. t('Javascript crop') .'</a>';
    else
    $row[] = '<img src="'.file_create_url($image->thumbpath).'" vspace="5" />';
    $row[] = drupal_render($form['rows'][$image->id]['description']).$image->filepath;
    $row[] = array('data' => format_size($image->filesize), 'style' => 'white-space: nowrap');
    $row[] = drupal_render($form['rows'][$image->id]['weight']);
    $row[] = array('data' => drupal_render($form['rows'][$image->id]['delete']), 'align' => 'center');
    $rows[] = $row;
  }

  $output = '<fieldset><legend>'.t('Uploaded images').'</legend>';
  $output .= theme('table', $header, $rows);
  $output .= drupal_render($form);
  $output .= '</fieldset>';
  
  return $output;
}

Features, support, bugs etc
---------------------------
File request,bugs,patches on http://drupal.org/project/imagecrop

Inspiration
-----------
Came from the imagefield_crop module on which I based the html and jquery 
with some adjustments.

Author
------
Kristof De Jaeger - http://drupal.org/user/107403 - http://realize.be
