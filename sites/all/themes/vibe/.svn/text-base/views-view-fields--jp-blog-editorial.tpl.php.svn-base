<?php
// $Id: views-view-fields.tpl.php,v 1.6 2008/09/24 22:48:21 merlinofchaos Exp $
/**
 * @file views-view-fields.tpl.php
 * Default simple view template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->separator: an optional separator that may appear before a field.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */

?>    
<? //var_dump(array_keys($fields)); ?> 
<div id="editorial_info">
	<?php 
		$editorial_image = $fields['phpcode']->content;
		$editorial_info = $fields['phpcode_1']->content;
	?>
    <div id="editorial_image">
   		<?php $node = node_load($fields['nid']->content);?>
       <img src="<?php print url($editorial_image)?>" alt="<?php print $editorial_image?>" />
     
       <?php print $editorial_info; ?>
    </div>
</div>
<div class="clear-block"></div> 
  
		
