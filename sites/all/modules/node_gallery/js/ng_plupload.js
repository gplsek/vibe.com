/**
 * @file
 * Node Gallery additions to plupload page.
 *
 * I didn't find a singleton in plupload, but the Drupal module currently
 * hardcodes the element id, which makes it easy to retrieve the
 * plupload object anyway. If that changes, we have to update it here.
 */
Drupal.behaviors.nodeGalleryPlupload = function (context) {
  var self = this;
  this.context = context;
  this.gid = Drupal.settings.node_gallery.gid;
  this.galleryType = Drupal.settings.node_gallery.galleryType;
  this.errorMessage = '';

  var updateGallery = function (gallery) {
    if (typeof gallery.is_error != undefined) {
      self.errorMessage = gallery.message;
    }
    self.gid = gallery.nid;
  }

  var galleryCreationFailed = function () {
    self.errorMessage = Drupal.t("Failed to create gallery: Request failed.");
    self.uploader.stop();
  }

  var init = function () {
    // check if upload instance is already there, else try again later.
    self.uploader = $("#uploader", self.context).pluploadQueue();
    if (typeof self.uploader == 'undefined') {
      setTimeout(init, 500);
      return;
    }
    self.uploader.bind('FileUploaded', function () {
      if (self.uploader.total.queued == 0) {
        window.location.href = Drupal.settings.basePath + 'node/' + self.gid + '/edit';
      }
    });
    self.uploader.bind('BeforeUpload', function () {
      if (!self.gid || self.gid < 0) {
        // create the gallery on the fly.
        var url = Drupal.settings.basePath + 'node-gallery/json/gallery/create/' + self.galleryType;
        // we use a synchronous call here, so plupload waits for us.
        $.ajax({
          url: url,
          success: updateGallery,
          error: galleryCreationFailed,
          dataType: 'json',
          async: false
        });
        if (!self.gid || self.gid < 0) {
          if (self.errorMessage.length < 1) {
            self.errorMessage = Drupal.t("Failed to create gallery. Unknown error occured.");
          }
          alert(self.errorMessage);
          self.uploader.stop();
          self.uploader.trigger('Error', {code: "102", message: self.errorMessage, id: self.uploader.id});
        } else {
          // update plupload query string with correct gallery nid.
          self.uploader.settings.url = self.uploader.settings.url.replace(/gid=-1/i, "gid=" + self.gid);
        }
      }
    });
  }
  init();
};