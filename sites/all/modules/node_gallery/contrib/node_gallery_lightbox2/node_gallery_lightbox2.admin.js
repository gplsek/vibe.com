// $Id: node_gallery_lightbox2.admin.js,v 1.1.2.3 2010/05/08 14:02:30 justintime Exp $
$(document).ready(function(){
    // View Original as Lightbox2 Popup
    $lightbox2_input = $('input#edit-config-original-view-original-lightbox2');
    $lightbox2_preset = $('.lightbox2-preset');
    
    // View thumbnail information
    $thumbnail_number = $('#edit-config-teaser-thumbnails-num-wrapper');
    $thumbnail_radio = $('input#edit-config-teaser-gallery-display-type-thumbnails');
    $teaser_view_radios = $('.teaser-display-type-radios input:radio');
    $thumbnail_lightbox2 = $('input#edit-config-teaser-gallery-display-type-lightbox2-gallery');
    $thumbnail_lightbox2_preset = $('.lightbox2-gallery-preset');
    
    // View Gallery information
    $gallery_display_radios = $('.gallery-display-type-radios');
    $gallery_display_lightbox2 = $('input#edit-config-gallery-gallery-display-type-lightbox2-gallery');
    $gallery_display_lightbox2_preset = $('.gallery-lightbox2-gallery-preset');
    
    // Onload - If Lightbox is not selected, hide the imagecache preset field
    if ($lightbox2_input.attr('checked') == false) {
      $lightbox2_preset.hide();
    }
    
    // Onload - If teaser display "lightbox" is not selected,
    // hide the preset selection
    if ($thumbnail_lightbox2.attr('checked') == false) {
      $thumbnail_lightbox2_preset.hide();
    }
    
    // Onload - If gallery display "lightbox" is not selected,
    // hide the preset selection
    if ($gallery_display_lightbox2.attr('checked') == false) {
      $gallery_display_lightbox2_preset.hide();
    }
    
    // If the radios have changed, we will want evaluate to see if we want to 
    // show or hide some extra fields
    $view_original_radios.change(function () {
        if ($lightbox2_input.attr('checked')) {
          $lightbox2_preset.slideDown();
        }
        else {
          $lightbox2_preset.slideUp();
        }
    });
    
    // If the teaser display radios have changed, we will want evaluate to see
    // if we want to show or hide some extra fields
    $teaser_view_radios.change(function () {
        
        if ($thumbnail_lightbox2.attr('checked')) {
          $thumbnail_lightbox2_preset.slideDown();
        }
        else {
          $thumbnail_lightbox2_preset.slideUp();
        }
    });
    
    // If the gallery display radios have changed, we will want evaluate to see
    // if we want to show or hide some extra fields
    $gallery_display_radios.change(function () {
        if ($gallery_display_lightbox2.attr('checked')) {
          $gallery_display_lightbox2_preset.slideDown();
        }
        else {
          $gallery_display_lightbox2_preset.slideUp();
        }
    });
});