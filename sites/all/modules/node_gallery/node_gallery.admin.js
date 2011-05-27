// $Id: node_gallery.admin.js,v 1.2.4.11 2010/05/08 14:02:30 justintime Exp $

$(document).ready(function(){
    // View original as text link
    $view_original_text = $('input#edit-config-original-view-original-text');
    $view_original_text_value = $('.view-original-text-value');
    $view_original_radios = $('.view-original-radios input:radio');
    
    // View thumbnail information
    $thumbnail_number = $('#edit-config-teaser-thumbnails-num-wrapper');
    $thumbnail_radio = $('input#edit-config-teaser-gallery-display-type-thumbnails');
    $teaser_view_radios = $('.teaser-display-type-radios input:radio');
    
    // View Gallery information
    $gallery_display_radios = $('.gallery-display-type-radios');
    
    // Onload - If text is not selected, hide the textfield
    if ($view_original_text.attr('checked') == false) {
      $view_original_text_value.hide();
    }
    
    // Onload - If teaser display "thumbnail" is not selected,
    // hide the textfield
    if ($thumbnail_radio.attr('checked') == false) {
      $thumbnail_number.hide();
    }
    
    // If the radios have changed, we will want evaluate to see if we want to 
    // show or hide some extra fields
    $view_original_radios.change(function () {
        
        if ($view_original_text.attr('checked')) {
          $view_original_text_value.slideDown();
        }
        else {
          $view_original_text_value.slideUp();
        }
    });
    
    // If the teaser display radios have changed, we will want evaluate to see
    // if we want to show or hide some extra fields
    $teaser_view_radios.change(function () {
        if ($thumbnail_radio.attr('checked')) {
          $thumbnail_number.slideDown();
        }
        else {
          $thumbnail_number.slideUp();
        }
    });
});
