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
<div id="celeb_website">
	<?php 
		$website_image = $fields['phpcode']->content;
		$website_name = $fields['phpcode_3']->content;
		$website_text = $fields['phpcode_1']->content;
		$website_link = $fields['phpcode_2']->content;
	?>
   <div id="celeb_name_image">
   		
       <img src="<?php print url($website_name)?>" alt="<?php print $website_name?>" />
       <? //var_dump($node); ?> 
      
    </div>
   <div id="celeb_website_info">
    	<?php print $website_text; ?>
    </div> 
   <?php if(!empty($website_link)) { ?>
    <div id="celeb_website_link">
    	Link: <span id="link_list"><?php echo t($website_link);?></span>
    </div>
    <? } ?>
        
</div> 
<div class="clear-block"></div> 
  
		
