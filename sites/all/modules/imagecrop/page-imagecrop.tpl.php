<?php
// $Id: page-imagecrop.tpl.php,v 1.1.4.2 2008/10/17 14:27:03 swentel Exp $
/**
 * @file
 * Imagecrop template file.
 */

header("Cache-Control: no-cache, must-revalidate"); // HTTP/1.1
?>
<html>
<?php
  print $head;
  print $scripts;
  print $styles;
?>
<body>
<?php print $content; ?>
</body>
</html>