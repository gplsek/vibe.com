<?php
// $Id: comment.tpl.php 25 2011-02-01 19:27:23Z jackncoke $
?>

<div class="comment <?php print $comment_classes;?> clear-block">

  <?php if ($comment->new): ?>
  <a id="new"></a>
  <span class="new"><?php print $new ?></span>
  <?php endif; ?>


<?php print $picture ?>


<div class="comment-title"> <h2 class="cuprum" style="margin-bottom:0;" >   <?php print $author; ?></h2> </div>
<span class="submitted-info"> <?php  print $submitted ?> </span>

<div class="comment-body">
<?php print $content ?>
</div>

  <?php if ($links): ?>
  <div class="comment-links">
    <?php print $links ?>
  </div>
  <?php endif; ?>
  
</div><!-- /comment -->