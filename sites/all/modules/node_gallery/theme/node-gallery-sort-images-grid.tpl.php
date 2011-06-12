<?php
/**
 * @file
 * Default theme implementation for the sort images grid, used when jquery_ui is active.
 *
 * Available variables:
 *   $items - the markup for the image items.
 */
?>
<div id="node-gallery-sort-images-grid">
  <?php if (!empty($variables['images'])): ?>
  <div class="node-gallery-sort-presets">
    <span class="node-gallery-sort-presets-description description"><?php print t("Sort items by"); ?>: </span>
    <a href="#" id="node-gallery-sort-title"><?php print t("Title"); ?> &uarr;&darr;</a> |
    <a href="#" id="node-gallery-sort-filename"><?php print t("Filename"); ?> &uarr;&darr;</a> |
    <a href="#" id="node-gallery-sort-changed"><?php print t("Last changed date"); ?> &uarr;&darr;</a> |
    <a href="#" id="node-gallery-sort-current"><?php print t("Restore last saved order"); ?></a>
  </div>
  <div class="clear-block node-gallery-sort-images-grid-sortable">
  <?php
    foreach ($images as $image) {
      print theme('node_gallery_sort_images_grid_item', $image);
    }
  endif;
  ?>
  </div>
</div>
