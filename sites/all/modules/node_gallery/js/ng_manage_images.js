/**
 * @file
 * jQuery additions to manage images page.
 */

/**
 * Image rotation preview.
 */
Drupal.theme.prototype.nodeGalleryRotateDialog = function (filepath) {
  var output = '';
  filepath = Drupal.settings.basePath + filepath;
  output += '<img src="' + filepath + '" class="ng3-rotate-90" />';
  output += '<img src="' + filepath + '" class="ng3-rotate-180" />';
  output += '<img src="' + filepath + '" class="ng3-rotate-270" />';
  return output;
};

Drupal.behaviors.nodeGalleryRotateImagePreview = function (context) {
  var self = this;
  this.link = undefined;

  var buttonOk = function () {
    var selected = $('#node-gallery-rotate-dialog .selected').attr('class');
    if (typeof selected != 'undefined') {
      var degrees = selected.match(/ng3-rotate-([0-9]+)/);
      self.link.parent().find(':input:radio[id$="'+degrees[1]+'"]').attr('checked', true).click();
    }
    self.dialog.dialog("close");
  };

  var _buttons = {};
  _buttons[Drupal.t('Ok')] = buttonOk;
  _buttons[Drupal.t('Cancel')] = function () {self.dialog.dialog("close");};
  if ($('a.ng3-rotate-link', context).length > 0) {
    this.dialog = $('<div id="node-gallery-rotate-dialog"></div>')
      .dialog({
        autoOpen: false,
        modal: true,
        resizable: true,
        title: 'Rotate image',
        width: 400,
        height: 250,
        minWidth: 400,
        minHeight: 250,
        buttons: _buttons
      });
  }
  $('a.ng3-rotate-link', context).each(function () {
    $(this).click(function () {
      self.link = $(this);
      self.dialog.html(Drupal.theme('nodeGalleryRotateDialog', $(this).attr('rel'))).dialog('open');
      $('#node-gallery-rotate-dialog img').each( function () {
        $(this).click(function () {
          $(this).addClass('selected').siblings('.selected').removeClass('selected');
        })
      });
      return false;
    });
  });
  $(':input:radio[name^="images"][name$="\[rotate\]"]', context).each(function () {
    if ($(this).val() == "0") {
      $(this).attr('checked', 'checked');
    }
    $(this).click(function () {
      $(this).parents('tr:first').find('img.imagecache-node-gallery-admin-thumbnail').removeClass().addClass('imagecache-node-gallery-admin-thumbnail ng3-rotate-'+$(this).val());
    });
  });
};
