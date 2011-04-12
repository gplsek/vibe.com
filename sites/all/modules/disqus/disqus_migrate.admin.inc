<?php
// $Id: disqus_migrate.admin.inc,v 1.1.2.2 2008/11/29 10:03:46 robloach Exp $

/**
 * Menu callback; Import comments from Disqus to the Drupal comment system.
 */
function disqus_admin_settings_import() {
  $form = array();
  $form['#attributes'] = array('enctype' => "multipart/form-data");
  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Import'),
    '#description' => t('The comments.xml file generated in the <a href="@disqustools">Disqus tools administration</a>. You can download this file by clicking on <a href="@commentsxml">comments.xml</a>.', array('@commentsxml' => 'http://disqus.com/migrate/'. variable_get('disqus_domain', '') .'/comments.xml', '@disqustools' => 'http://disqus.com/admin/tools/'. variable_get('disqus_domain', '') .'/')),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
  return $form;
}

/**
 * Form submit handler for the import tool.
 */
function disqus_admin_settings_import_submit($form, &$form_state) {
  // See if the file was successfully saved.
  if ($file = file_save_upload('file')) {
    $simplexml = @simplexml_load_file($file->filepath);
    if ($simplexml === FALSE) {
      form_set_error('file', t('Invalid XML file uploaded.'));
    }
    else {
      
      // Keep track of the number of comments imported.
      $count = 0;

      foreach($simplexml->article as $article) {
        // Check if the URLs match a Drupal path.
        $url = parse_url($article->url);
        $path = substr($url['path'], strlen(base_path()));
        if (url($path, array('absolute' => TRUE)) == $article->url) {
          
          // Resolve any path aliases.
          if (function_exists('custom_url_rewrite_inbound')) {
            $path = drupal_get_normal_path($path);
          }
  
          // Import any node comments.
          if (substr($path, 0, 4) == 'node') {
            $nid = substr($path, 5, 1);
            // Loop through the comments and import them into Drupal.
            foreach ($article->comments as $thread) {
              foreach($thread->comment as $comment) {
                comment_save(array(
                  'nid' => $nid,
                  'mail' => $comment->email,
                  'homepage' => $comment->url,
                  'name' => $comment->name,
                  'comment' => $comment->message,
                  'subject' => $comment->message,
                  'format' => 0,
                  'cid' => NULL,
                  'pid' => 0,
                  'uid' => 0,
                  'timestamp' => strtotime($comment->date), // Convert the date to a timestamp.
                ));
                $count++;
              }
            }
          }
          
          // No other content type supports Disqus commenting 
        }
      }

      // Display a success message.
      drupal_set_message(t('There were @count comments that were imported. Visit the <a href="@comments">Comments administration</a> to see the imported comments.', array('@comments' => base_path() .'admin/content/comment', '@count' => $count)));
    }
  }
}
