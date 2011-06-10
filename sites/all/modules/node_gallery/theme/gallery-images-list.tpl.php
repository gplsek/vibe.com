<?php
// $Id: gallery-images-list.tpl.php,v 1.1.2.4 2010/05/08 14:02:30 justintime Exp $
?>
<div class="gallery-images-list clear-block">
  <?php
  if (is_array($images_list)) {
    print theme('item_list', $images_list);
  }
  else {
    print $empty_message;
  }
  ?>
</div>