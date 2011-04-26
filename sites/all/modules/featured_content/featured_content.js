// $Id: featured_content.js,v 1.1.2.4 2010/08/11 05:54:09 kepol Exp $

Drupal.behaviors.featured_content = function(context) {
  // show manual or filter fieldset depending on type selection
  var type = $('#edit-featured-content-block-type');
  if (type) {
    showHideFeaturedBlockSettings(type);
  }
  $('#edit-featured-content-block-type').bind('change', {}, onchangeFeaturedBlockSettings);
 
  // show style select list depending on display selection
  var display = $('#edit-featured-content-block-display');
  if (display) {
    showHideFeaturedBlockStyleSettings(display);
  }
  $('#edit-featured-content-block-display').bind('change', {}, onchangeFeaturedBlockStyleSettings);
 
  // show read more fields depending on more selection
  var more = $('#edit-featured-content-block-more-display');
  if (more) {
    showHideFeaturedBlockMoreSettings(more);
  }
  $('#edit-featured-content-block-more-display').bind('change', {}, onchangeFeaturedBlockMoreSettings);
 
  // show rss fields depending on rss selection
  var rss = $('#edit-featured-content-block-rss-display');
  if (rss) {
    showHideFeaturedBlockRSSSettings(rss);
  }
  $('#edit-featured-content-block-rss-display').bind('change', {}, onchangeFeaturedBlockRSSSettings);
}

function onchangeFeaturedBlockSettings(event) {
  showHideFeaturedBlockSettings($(this));
}

function showHideFeaturedBlockSettings(select) {
  var choice = select.attr('value');
  if (choice == 'manual') {
    $('fieldset.featured-content-block-filter').hide();
    $('fieldset.featured-content-block-cck').hide();
    $('fieldset.featured-content-block-manual').show();
  }
  else if (choice == 'filter') {
    $('fieldset.featured-content-block-manual').hide();
    $('fieldset.featured-content-block-cck').hide();
    $('fieldset.featured-content-block-filter').show();
  }
  else if (choice == 'cck') {
    $('fieldset.featured-content-block-manual').hide();
    $('fieldset.featured-content-block-filter').hide();
    $('fieldset.featured-content-block-cck').show();
  }
}

function onchangeFeaturedBlockStyleSettings(event) {
  showHideFeaturedBlockStyleSettings($(this));
}

function showHideFeaturedBlockStyleSettings(select) {
  var choice = select.attr('value');
  if (choice == 'links') {
    $('#edit-featured-content-block-style-wrapper').show();
  }
  else {
    $('#edit-featured-content-block-style-wrapper').hide();
  }
}

function onchangeFeaturedBlockMoreSettings(event) {
  showHideFeaturedBlockMoreSettings($(this));
}

function showHideFeaturedBlockMoreSettings(select) {
  var choice = select.attr('value');
  if (choice == 'custom') {
    $('#fieldset-block-settings-featured-block-more').show();
    $('#edit-featured-content-block-more-text-wrapper').show();
    $('#edit-featured-content-block-more-url-wrapper').show();
    $('#edit-featured-content-block-more-num-wrapper').hide();
    $('#edit-featured-content-block-more-style-wrapper').hide();
    $('#edit-featured-content-block-more-title-wrapper').hide();
    $('#edit-featured-content-block-more-header-wrapper').hide();
    $('#edit-featured-content-block-more-footer-wrapper').hide();
  }
  else if (choice == 'none') {
    $('#fieldset-block-settings-featured-block-more').hide();
    $('#edit-featured-content-block-more-text-wrapper').hide();
    $('#edit-featured-content-block-more-url-wrapper').hide();
    $('#edit-featured-content-block-more-num-wrapper').hide();
    $('#edit-featured-content-block-more-style-wrapper').hide();
    $('#edit-featured-content-block-more-title-wrapper').hide();
    $('#edit-featured-content-block-more-header-wrapper').hide();
    $('#edit-featured-content-block-more-footer-wrapper').hide();
  }
  else if (choice == 'links') {
    $('#fieldset-block-settings-featured-block-more').show();
    $('#edit-featured-content-block-more-text-wrapper').show();
    $('#edit-featured-content-block-more-url-wrapper').hide();
    $('#edit-featured-content-block-more-num-wrapper').show();
    $('#edit-featured-content-block-more-style-wrapper').show();
    $('#edit-featured-content-block-more-title-wrapper').show();
    $('#edit-featured-content-block-more-header-wrapper').show();
    $('#edit-featured-content-block-more-footer-wrapper').show();
  }
  else {
    $('#fieldset-block-settings-featured-block-more').show();
    $('#edit-featured-content-block-more-text-wrapper').show();
    $('#edit-featured-content-block-more-url-wrapper').hide();
    $('#edit-featured-content-block-more-num-wrapper').show();
    $('#edit-featured-content-block-more-style-wrapper').hide();
    $('#edit-featured-content-block-more-title-wrapper').show();
    $('#edit-featured-content-block-more-header-wrapper').show();
    $('#edit-featured-content-block-more-footer-wrapper').show();
  }
}

function onchangeFeaturedBlockRSSSettings(event) {
  showHideFeaturedBlockRSSSettings($(this));
}

function showHideFeaturedBlockRSSSettings(checkbox) {
  if (checkbox.attr('checked')) {
    $('#fieldset-block-settings-featured-block-rss').show();
    $('#edit-featured-content-block-rss-title-wrapper').show();
  }
  else {
    $('#fieldset-block-settings-featured-block-rss').hide();
    $('#edit-featured-content-block-rss-title-wrapper').hide();
  }
}
