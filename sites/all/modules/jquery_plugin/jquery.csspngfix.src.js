// $Id: jquery_csspngfix.src.js,v 1.0 2008/06/14 18:29:32 skilip Exp $
jQuery.fn.cssPNGFix = function(sizingMethod) {
  var sizingMethod = (sizingMethod) || 'scale';
  if($.browser.msie && parseInt($.browser.version) < 7) {
    $(this).each(function() {
      var css_bg_img = $(this).css('background-image');
      var bg_img = css_bg_img.substring(5, css_bg_img.length - 2);
      $(this).css({'background':'none', 'filter':'progid:DXImageTransform.Microsoft.AlphaImageLoader(src="' + bg_img + '", sizingMethod="' + sizingMethod + '")'});
    });
  };
  return $(this);
};

jQuery.fn.imgPNGFix = function(sizingMethod) {
  var sizingMethod = (sizingMethod) || 'scale';
  if($.browser.msie && parseInt($.browser.version) < 7) {
    $(this).each(function() {
      var spacer_src = Drupal.settings.spacer_src;
      var the_img = $(this);
      var src = $(this).attr('src');
      $(the_img).attr('src', spacer_src);
      $(the_img).css({
        'background':'none',
        'filter':'progid:DXImageTransform.Microsoft.AlphaImageLoader(src="' + src + '", sizingMethod="' + sizingMethod + '")'
      });
    });
  };
  return $(this);
};

$(function() {
  if($.browser.msie && parseInt($.browser.version) < 7) {
    $('.png_bg').each(function() {
      var css_bg_img = $(this).css('background-image');
      var bg_img = css_bg_img.substring(5, css_bg_img.length - 2);
      $(this).css({'background':'none', 'filter':'progid:DXImageTransform.Microsoft.AlphaImageLoader(src="' + bg_img + '", sizingMethod="image")'});
    });
    $('.png_img').imgPNGFix('image');
  };
});