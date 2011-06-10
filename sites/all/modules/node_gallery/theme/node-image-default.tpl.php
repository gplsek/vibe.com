<?php
// $Id: node-image-default.tpl.php,v 1.1.2.5 2010/05/08 14:02:30 justintime Exp $
?>
<div class="node <?php print $node_classes; ?>" id="node-<?php print $node->nid; ?>"><div id="node-inner" class="node-inner">

  <?php if ($page == 0): ?>
    <h2 class="title">
      <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
    </h2>
  <?php endif; ?>

  <?php if ($unpublished): ?>
    <div class="unpublished"><?php print t('Unpublished'); ?></div>
  <?php endif; ?>

  <?php if ($submitted): ?>
    <div class="submitted">
      <?php print $submitted; ?>
    </div>
  <?php endif; ?>
  
  <div class="content">
    <?php print $content; ?>
  </div>
  <?php if ($terms): ?>
    <div class="taxonomy"><?php print t(' in ') . $terms; ?></div>
  <?php endif; ?>

  <?php if ($links): ?>
    <div class="links">
      <?php print $links; ?>
    </div>
  <?php endif; ?>

</div></div> <!-- /node-inner, /node -->
<?php if ($page): ?>
  <?php print $comments; ?>
<?php endif; ?>