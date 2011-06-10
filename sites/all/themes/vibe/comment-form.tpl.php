TESTING
<br>
<?//var_dump(array_keys($form))?>

<pre>
<?//php print_r($form);?>
</pre>

<?$form['author']['#title'] = "TESTING AUTHOR";?>
<?= drupal_render($form['author']) ?>

<?php debug($form); ?>