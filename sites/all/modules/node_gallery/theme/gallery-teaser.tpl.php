<?php
// $Id: gallery-teaser.tpl.php,v 1.1.2.4 2010/05/08 14:02:30 justintime Exp $
?>
<div class="gallery-teaser">
  <?php
  if (is_array($gallery_teaser)) {
    print theme('item_list', $gallery_teaser);
  }
  else {
    print $gallery_teaser;
  }
?>
</div>