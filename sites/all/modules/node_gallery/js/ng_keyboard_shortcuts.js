/**
 * @file
 * jQuery-based keyboard shortcuts.
 */
Drupal.behaviors.initKeyboardShortcuts = function(context) {
  $(document).keydown(function(event) {
    if (event.which == '37') {
      // left arrow
      if ($('td.image-navigator-prev a').length > 0) {
        window.location.href = $('td.image-navigator-prev a').attr('href');
      }
    }
    if (event.which == '39') {
      // right arrow
      if ($('td.image-navigator-next a').length > 0) {
        window.location.href = $('td.image-navigator-next a').attr('href');
      }
    }
  });
}
