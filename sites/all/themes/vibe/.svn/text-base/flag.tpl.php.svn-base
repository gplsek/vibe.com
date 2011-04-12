<?php
// $Id: flag.tpl.php,v 1.1.2.7.2.2 2009/10/28 00:01:32 quicksketch Exp $

/**
 * @file
 * Default theme implementation to display a flag link, and a message after the action
 * is carried out.
 *
 * Available variables:
 *
 * - $flag: The flag object itself. You will only need to use it when the
 *   following variables don't suffice.
 * - $flag_name_css: The flag name, with all "_" replaced with "-". For use in 'class'
 *   attributes.
 * - $flag_classes: A space-separated list of CSS classes that should be applied to the link.
 *
 * - $action: The action the link is about to carry out, either "flag" or "unflag".
 * - $last_action: The action, as a passive English verb, either "flagged" or
 *   "unflagged", that led to the current status of the flag.
 *
 * - $link_href: The URL for the flag link.
 * - $link_text: The text to show for the link.
 * - $link_title: The title attribute for the link.
 *
 * - $message_text: The long message to show after a flag action has been carried out.
 * - $after_flagging: This template is called for the link both before and after being
 *   flagged. If displaying to the user immediately after flagging, this value
 *   will be boolean TRUE. This is usually used in conjunction with immedate
 *   JavaScript-based toggling of flags.
 * - $setup: TRUE when this template is parsed for the first time; Use this
 *   flag to carry out procedures that are needed only once; e.g., linking to CSS
 *   and JS files.
 *
 * NOTE: This template spaces out the <div> tags for clarity only. When doing some
 * advanced theming you may have to remove all the whitespace.
 */

  if ($setup) {
    drupal_add_css(drupal_get_path('module', 'flag') .'/theme/flag.css');
    drupal_add_js(drupal_get_path('module', 'flag') .'/theme/flag.js');
  }
  
  $nid = arg(1);
?>

<?if($link_text != "plus" && $link_text != "unplus") {?>

<div class="<?php print $flag_wrapper_classes; ?>">
  <?php if ($link_href && !$flag->is_flagged($nid)) { ?>
    <a href="<?php print $link_href; ?>" class="<?php print $flag_classes?>" title="<?php print $link_title; ?>" rel="nofollow"><div class="<?php print $flag_classes?>"><?php print $link_text; ?></div></a>
  <?php } else { ?>
    <div class="<?php print $flag_classes ?>"><?php print $link_text; ?></div>
  <?php } ?>
  <?php if ($after_flagging){ ?>
    <div class="flag-message flag-<?php print $last_action; ?>-message">
      <?php print $message_text; ?>
    </div>
  <?php } ?>
 <div class="reaction_count"><a class="flag-waiting flag-throbber"><?=$flag->get_count($content_id)?></a></div>
</div>

<?} else {?>
	<script type='text/javascript'>
		<?php // HACKTASTIC!!!!?>
		<?php preg_match("/flag-rating-plus-(\d*)/",$flag_wrapper_classes,$matches); $nid = $matches[1];?>
		<?php $content_rating = "content_rating_" . $nid;?>
		$("#<?=$content_rating?>").html("<div class='<?php print $flag_wrapper_classes; ?>'><div class='reaction_count' style='float:left'><a class='flag-waiting flag-throbber'><?=$flag->get_count($content_id)?></a></div><div style='padding-top: 2px; float: left; margin-left: 5px;'><a href='<?php print str_replace("/flag/unflag","/flag/flag",$link_href); ?>'  class='<?php print $flag_classes?>' title='<?php print $link_title; ?>' rel='nofollow'><img src='/sites/all/themes/vibe/images/plus_button.png' border='0'></a></div><div style='padding-top: 2px; float: left; margin-left: 5px; margin-right: 3px;'><a href='<?php print str_replace("/flag/flag","/flag/unflag",$link_href); ?>'  class='<?php print $flag_classes?>' title='<?php print $link_title; ?>' rel='nofollow'><img src='/sites/all/themes/vibe/images/minus_button.png' border='0'></a></div><div class='clear-block'></div></div>");
	</script>
	
<?}?>



