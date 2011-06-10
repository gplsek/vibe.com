<?php
// $Id: image-detail-view.tpl.php,v 1.1.2.3 2010/05/08 14:02:30 justintime Exp $
?>
<div class="image-preview"><?php print $image; ?></div>
<?php if (!empty($extra)): ?>
  <div class="image-preview-extra"><?php print $extra; ?></div>
<?php endif; ?>