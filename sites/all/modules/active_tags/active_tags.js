// $Id: active_tags.js,v 1.1.2.14 2009/08/09 14:01:56 dragonwize Exp $

function activeTagsActivate(context) {
  var wrapper = $(context);
  if (wrapper.length == 1) {
    var tagarea = activeTagsWidget(context);
    wrapper.before(tagarea);
    Drupal.behaviors.autocomplete(document);
  }
  $('.add-tag:not(.tag-processed)').click(function() {
    jQuery.each($(this).prev().val().split(','), function(i, v) {
      if (jQuery.trim(v) != '') {
        activeTagsAdd(context, v);
      }
    });
    activeTagsUpdate(context);
    $(this).prev().val('');
  }).addClass('tag-processed');

  if ($.browser.mozilla) {
    $('.tag-entry:not(.tag-processed)').keypress(activeTagsCheckEnter).addClass('tag-processed');
  }
  else {
    $('.tag-entry:not(.tag-processed)').keydown(activeTagsCheckEnter).addClass('tag-processed');
  }

  jQuery.each(wrapper.find('input.form-text').attr('value').split(','), function(i, v) {
    if (jQuery.trim(v) != '') {
      activeTagsAdd(context, v);
    }
  });

  wrapper.hide();
}

function activeTagsCheckEnter(event) {
  if (event.keyCode == 13) {
    $('#autocomplete').each(function() {
      this.owner.hidePopup();
    })
    $(this).next().click();
    event.preventDefault();
    return false;
  }
}

function activeTagsAdd(context, v) {
  if (jQuery.trim(v) != '') {
    $(context).prev().children('.tag-holder').append(Drupal.theme('activeTagsTerm', v));
    $('.remove-tag:not(.tag-processed)').click(function() {
      $(this).parent().remove();
      activeTagsUpdate(context);
    }).addClass('tag-processed');
  }
}

function activeTagsUpdate(context) {
  var wrapper = $(context);
  var textFields = wrapper.children('input.form-text');
  textFields.val('');
  wrapper.prev().children('.tag-holder').children().children('.tag-text').each(function(i) {
    if (i == 0) {
      textFields.val($(this).text());
    }
    else {
      textFields.val(textFields.val() + ', ' + $(this).text());
    }
  });
}

function activeTagsWidget(context) {
  var vid = context.substring(20, context.lastIndexOf('-'));
  return Drupal.theme('activeTagsWidget', context, vid);
}


/**
 * Theme a selected term.
 */
Drupal.theme.prototype.activeTagsTerm = function(value) {
  return '<div class="tag-tag"><span class="tag-text">' + value + '</span><span class="remove-tag">x</span></div>';
};

/**
 * Theme Active Tags widget.
 */
Drupal.theme.prototype.activeTagsWidget = function(context, vid) {
  var wrapper = $(context);
  return '<div id="' + context + '-activetags" class="form-item">' +
    '<label for="' + context + '-edit-tags">' + wrapper.find('label').text() + '</label>' +
    '<div class="tag-holder"></div>' +
    '<input type="text" class="tag-entry form-autocomplete" size="30" id="active-tag-edit0' + vid + '" />' +
    '<input type="button" value="' + Drupal.t('Add') + '" class="add-tag">' +
    '<input class="autocomplete" type="hidden" id="active-tag-edit0' + vid + '-autocomplete" ' +
    'value="' + $(context.replace('-wrapper', '-autocomplete')).val() + '" disabled="disabled" />' +
    '<div class="description">' + wrapper.find('.description').text() + '</div>' +
  '</div>';
};

Drupal.behaviors.tagger = function(context) {
  jQuery.each(Drupal.settings['active_tags'], function(i, v) {
    var wrapper = $(v);
    if (wrapper.length == 1 && !wrapper.hasClass('active-tags-processed')) {
      activeTagsActivate(v);
      wrapper.addClass('active-tags-processed');
    }
  });
}
