<?php
/*
 * @file
 * Template for simple jquery based slideshow.
 */
$sec = t('sec.');
?>
<div id="node-gallery-slideshow">
  <ul id="node-gallery-slideshow-controls">
    <li><a href="#" class="ng3-slideshow-previous">&lt; <?php print t('Previous'); ?></a></li>
    <li><a href="#" class="ng3-slideshow-play"><?php print t('Play / Pause'); ?></a></li>
    <li><a href="#" class="ng3-slideshow-next"><?php print t('Next'); ?> &gt;</a></li>
    <li>
      <select name="ng3-slideshow-interval" class="ng3-slideshow-interval">
        <option value="1000">1 <?php print $sec; ?></option>
        <option value="2000">2 <?php print $sec; ?></option>
        <option value="3000">3 <?php print $sec; ?></option>
        <option value="4000">4 <?php print $sec; ?></option>
        <option value="5000">5 <?php print $sec; ?></option>
      </select>
    </li>
  </ul>
 <div class="slideshow-container loading">
 </div>
</div>
