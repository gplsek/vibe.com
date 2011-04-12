// $Id: disqus.js,v 1.1.2.2 2009/08/27 23:40:14 robloach Exp $

/**
 * Drupal Disqus behaviors.
 */
Drupal.behaviors.disqus = function(context) {
  if (Drupal.settings.disqusCommentDomain || false) {
    // Create the query.
    var query = '?';
    jQuery("a[href$='#disqus_thread']").each(function(i) {
      query += 'url' + i + '=' + encodeURIComponent($(this).attr('href')) + '&';
    });

    // Make sure we are actually processing some links.
    if (query.length > 2) {
      // Make the AJAX call to get the number of comments.
      jQuery.ajax({
        type: 'GET',
        url: 'http://disqus.com/forums/' + Drupal.settings.disqusCommentDomain + '/get_num_replies.js' + query,
        dataType: 'script',
        cache: true
      });
    }
  }
};
