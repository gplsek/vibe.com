/**
 * @file
 * Simple jQuery based slideshow.
 */
Drupal.behaviors.initNodeGallerySlideshow = function(context) {
  var self = this;
  
  // settings and parameters
  this.gid = parseInt(Drupal.settings.node_gallery_slideshow.gid);
  this.imageIndex = parseInt(Drupal.settings.node_gallery_slideshow.imageOffset);
  this.slideshowIntervalTime = parseInt(Drupal.settings.node_gallery_slideshow.intervalTime);
  this.imageCount = parseInt(Drupal.settings.node_gallery_slideshow.imageCount);
  this.chunkSize = parseInt(Drupal.settings.node_gallery_slideshow.chunkSize);
  this.retrieveInterval = parseInt(Drupal.settings.node_gallery_slideshow.retrieveInterval);
  this.preloadCount = parseInt(Drupal.settings.node_gallery_slideshow.preloadCount);
  this.enableMicrothumbNavigator = Drupal.settings.node_gallery_slideshow.enableMicrothumbNavigator;
  this.microthumbCount = parseInt(Drupal.settings.node_gallery_slideshow.microthumbCount);
  
  // internal variables  
  this._microthumbNavigator = {};
  this._images = [];
  this._chunk = 0;
  this._preloadCache = [];
  this._context = $('#node-gallery-slideshow');
  this._container = $('div.slideshow-container', _context);
  this._halfThumbCount = Math.round((self.microthumbCount-1)/2);
  this._isInitialized = false;

  /**
   * Calculates a new cycling image index based on the passed offset.
   */
  var cycleIndex = function (offset) {
    offset = parseInt(offset);
    var index = (self.imageIndex + offset) % self.imageCount;
    if (index < 0) {
      index += self.imageCount;
    }
    return index;
  };

  /**
   * Preloads the image referenced by index through creating an image element
   * that is not attached to the DOM and keep a reference in self._preloadCache.
   * 
   * @param integer index Index of the image to preload.
   */
  var preload = function (index) {
    if (index > self._chunk*self.chunkSize) {
      setTimeout(function () { preload(index); }, self.retrieveInterval/2);
      return;
    }
    var cacheImage = new Image();
    cacheImage.src = Drupal.settings.basePath + self._images[index].display;
    $(cacheImage).load(function () {
      self._preloadCache[index] = cacheImage;      
    });
  };

  /**
   * Shows the next picture.
   */
  var showNext = function () {
    if (cycleIndex(1) > self._images.length-1) {
      setTimeout(showNext, self.retrieveInterval/2);
      return;
    }
    self.imageIndex = cycleIndex(1);
    showImage();
    preload(cycleIndex(self.preloadCount));
    delete self._preloadCache[cycleIndex(-self.preloadCount)];
  };

  /**
   * Shows the previous picture.
   */
  var showPrevious = function () {
    if (cycleIndex(-1)+1 > self._images.length) {
      preload(cycleIndex(-1));
      setTimeout(showPrevious, self.retrieveInterval/2);
      return;
    }
    self.imageIndex = cycleIndex(-1);
    showImage();
    preload(cycleIndex(-1));
    delete self._preloadCache[cycleIndex(self.preloadCount)];
  };

  /**
   * Inserts the image element identified by imageIndex, attaches it to the DOM and
   * fades it in, setting the height of the container element accordingly.
   */
  var showImage = function () {
    if ((self._preloadCache[self.imageIndex] == undefined) || !self._preloadCache[self.imageIndex].complete) {
      setTimeout(showImage, self.retrieveInterval/2);
      return;
    }
    $('img[id^="node-gallery-image"]', self._context).fadeOut(600, function () {$(this).remove()});
    var image = $('<img />').attr({
      id: 'node-gallery-image-' + self.imageIndex,
      src: Drupal.settings.basePath + self._images[self.imageIndex].display,
      title: self._images[self.imageIndex].title
    }).appendTo(self._container).hide();
    image.css('margin-left', -image.width()/2).fadeIn(400);
    if (image.height() > 0) {
      self._container.animate({height: image.height()}, 300);
    }
    if (self.enableMicrothumbNavigator) {
      updateNavigator();
    }
  };

  var updateNavigator = function () {
    var indexShift = -self._halfThumbCount;
    if (cycleIndex(indexShift) > self._chunk*self.chunkSize) {
      // navigator would wrap around to images that are not streamed yet.
      indexShift = 0;
    }
    self._microthumbNavigator.children().each(function(index) {
      var _index = cycleIndex(indexShift+index);
      var opacity = (_index == self.imageIndex) ? 1 : 0.6;
      $(this).attr('src', Drupal.settings.basePath + self._images[_index].thumb).data('index', _index).css({opacity: opacity});
    });
  }
  
  /**
   * Starts the automatic progression after self.slideshowIntervalTime milliseconds.
   */
  var startSlideshow = function () {
    self.slideshowInterval = setInterval(showNext, self.slideshowIntervalTime);
  };

  /**
   * Stops the automatic progression.
   */
  var stopSlideshow = function () {
    clearInterval(self.slideshowInterval);
    self.slideshowInterval = null;
  };

  /**
   * Toggles the automatic progression.
   */
  var toggleSlideshow = function () {
    if (self.slideshowInterval) {
      stopSlideshow();
    }
    else {
      startSlideshow();
    }
  };

  /**
   * Sets the interval time after which to progress to the next image.
   * @param integer newTime Interval time in milliseconds.
   */
  var setSlideshowIntervalTime = function (newTime) {
    stopSlideshow();
    self.slideshowIntervalTime = newTime;
    startSlideshow();
  };

  /**
   * Initializes the control links for navigation and for setting the interval time.
   */
  var initControls = function () {
    $('a.ng3-slideshow-play', self._context).click(function () {toggleSlideshow(); return false;});
    $('a.ng3-slideshow-previous', self._context).click(function () {stopSlideshow();showPrevious(); return false;});
    $('a.ng3-slideshow-next', self._context).click(function () {stopSlideshow();showNext(); return false;});
    $('div.slideshow-container', self._context).click(function () {stopSlideshow();showNext(); return false;});
    $('select.ng3-slideshow-interval', self._context).val(self.slideshowIntervalTime);
    $('select.ng3-slideshow-interval', self._context).change(function (event) {
      setSlideshowIntervalTime($('option:selected', event.target).val());
      showNext();
      return false;
    });
    if (self.enableMicrothumbNavigator) {
      $('<div id="node-gallery-navigator"></div>').appendTo(self._context);
      self._microthumbNavigator = $('#node-gallery-navigator');
      for (var i = 0; i < self.microthumbCount; ++i) {
        var _index = (cycleIndex(i-self._halfThumbCount) > ((self._chunk)*self.chunkSize)) ? cycleIndex(i) : cycleIndex(i-self._halfThumbCount);
        $('<img />').attr({
          src: Drupal.settings.basePath + self._images[_index].thumb,
          title: self._images[_index].title
        }).appendTo(self._microthumbNavigator).data('index', _index).click(function () {
          stopSlideshow();
          self.imageIndex = $(this).data('index');
          showImage();
        });
      }
    }
  };

  /**
   * Callback for processing a chunk of retrieved images. On the first call,
   * it initialises the slideshow. If there are additional chunks it schedules
   * the retrieval of further images.
   */
  var processRetrievedImages = function (response) {
    var data = Drupal.parseJson(response);
    var number = data.length;
    var offset = self._chunk*self.chunkSize;
    for (var i = 0; i < number; ++i) {
      self._images[offset+i] = data[i];
    }
    if (!self._isInitialized && ((self._chunk+1)*self.chunkSize) > self.imageIndex) {
      for (i = 0; i < self.preloadCount; ++i) {
        preload(cycleIndex(i));
      }
      initControls();
      self._container.removeClass('loading');
      showImage();
      startSlideshow();
      self._isInitialized = true;
    }
    self._chunk++;
    if ((self._chunk * self.chunkSize) < self.imageCount) {
      // schedule loading the next chunk.
      setTimeout(retrieveImages, self.retrieveInterval);
    }
  }

  var retrieveImages = function () {
    var url = Drupal.settings.basePath + 'node-gallery/json/gallery/' + self.gid + '/chunk/' + self.chunkSize + '/' + self._chunk;
    $.get(url, processRetrievedImages);
  }

  // start retrieving the images
  if (self.imageCount > 0) {
    if (self.chunkSize > self.imageCount) {
      self.chunkSize = self.imageCount;
    }
    if (self.preloadCount > self.chunkSize) {
      self.preloadCount = self.chunkSize;
    }
    retrieveImages();
  }
};
