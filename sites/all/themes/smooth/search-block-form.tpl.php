<?php 
/**
 * $Id: search-theme-form.tpl.php 25 2011-02-01 19:27:23Z jackncoke $
 */

?>

<div id="search">
	<?php 
	  $search['search_block_form'] = preg_replace('/<label(.*)>(.*)<\/label>\n/i', '', $search['search_block_form']);
	  print $search['search_block_form'];
	  print $search['submit'];
	  print $search['hidden'];
	?>
</div>