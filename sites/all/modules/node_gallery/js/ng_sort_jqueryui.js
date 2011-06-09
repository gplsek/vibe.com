/**
 * @file
 * jQuery based sorting function.
 */
Drupal.theme.prototype.nodeGalleryOrderChangedWarning = function () {
  return '<div class="warning" id="node-gallery-order-changed-warning"><span class="warning tabledrag-changed">*</span> ' + Drupal.t("Changes made in this table will not be saved until the form is submitted.") + '</div>';
};

Drupal.behaviors.nodeGalleryInitSorting = function (context) {
  var self = this;
  this.context = $("#node-gallery-sort-images-grid", context);
  this.container = $("div.node-gallery-sort-images-grid-sortable", this.context);
  this.originalSortString = $("#edit-original-sort", context).val();
  this.originalSort = this.originalSortString.split(',');  
  this.newSort = [];
  this.newSortString = "";
  this.newSortElement = $("#edit-new-sort", context);

  var nodeGalleryUpdatePositions = function () {
    self.newSort = [];
    self.container.children().each(function () {
      //self.newSort.push(self.originalSort[element.id.substring(element.id.lastIndexOf('-')+1)]);
      if ($(this).data('nid') != undefined) {
        self.newSort.push($(this).data('nid'));
      }
    });
    self.newSortString = self.newSort.join(',');
    self.newSortElement.val(self.newSortString);
    var changed = (self.newSortString != self.originalSortString);
    // show warning on first change and remove it if original was restored
    if (self.context.hasClass("markedChanged")) {
      if (!changed) {
        // user restored original order, remove warning
        self.context.removeClass("markedChanged");
        $("#node-gallery-order-changed-warning", self.context).fadeOut('slow', function () { $(this).remove(); });
      }
    }
    else if (changed) {
      $(Drupal.theme('nodeGalleryOrderChangedWarning')).appendTo(self.context).hide().fadeIn('slow');
      self.context.addClass("markedChanged");
    }
  }

  var nodeGalleryPresetClickHandler = function () {
    op = this.id.substring(this.id.lastIndexOf('-')+1);
    url = Drupal.settings.basePath+'node-gallery/json/gallery/'+Drupal.settings.node_gallery.gid+'/sort/'+op;
    if ($(this).hasClass("node-gallery-sort-active")) {
      url += '/1';
    }
    $(this).toggleClass("node-gallery-sort-active").siblings().removeClass("node-gallery-sort-active");
    $.get(url, null, nodeGallerySortByNidArray);
    return false;
  }

  var nodeGallerySortByNidArray = function(response) {
    var result, index;
    result = Drupal.parseJson(response);
    for (key in result) {
      index = jQuery.inArray(result[key], self.originalSort);
      self.container.append($('#images-sort-'+index));
    }
    nodeGalleryUpdatePositions();
    self.container.sortable("refreshPositions");
  }

  // enable drag and drop and disable selection
  this.container.children().each(function (index) {
    $(this).data('nid', self.originalSort[index]);
  })
  this.container.sortable({
    update: nodeGalleryUpdatePositions
  }).disableSelection();
  // enable presets
  $('div.node-gallery-sort-presets a:not(.ng3-processed)', self.context).addClass('ng3-processed').bind('click', nodeGalleryPresetClickHandler);
};
