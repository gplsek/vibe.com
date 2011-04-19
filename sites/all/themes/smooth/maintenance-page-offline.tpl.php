<?php
// $Id: page.tpl.php,v 1.1.2.5 2010/04/08 07:02:59 sociotech Exp $
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language; ?>" xml:lang="<?php print $language->language; ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <!--[if IE 8]>
  <?php print $ie8_styles; ?>
  <![endif]-->
  <!--[if IE 7]>
  <?php print $ie7_styles; ?>
  <![endif]-->
  <!--[if lte IE 6]>
  <?php print $ie6_styles; ?>
  <![endif]-->
  <?php print $scripts; ?>
  
  <style type="text/css">
	/* grid widths */
.grid12-12 {width: 960px;}

/*body.style-dark #breadcrumbs-wrapper,*/
#preface-top-wrapper,
#smooth-header,
#social-network,
#footer-group,
 ul.pager li.pager-current,
div.poll div.bar,
.block.nav-menu-list ul li a:hover,
.block.nav-menu-list ul li.active-trail a {
    background-color: #1d1d1d;
}
  </style>
  
  
</head>
<body class="in-maintenance no-sidebars layout-main sidebars-split font-family-lucida font-size-12 grid-type-960 grid-width-12 style-dark">
	<div id="page" class="page">
    <div id="page-inner" class="page-inner">
      <!-- header-top row: width = grid_width -->
	<div id="smooth-header" class="full-width">
		<div id="smooth-header-nav">
			<div id="smooth-header-nav-inner" class="row inner clearfix grid12-12" style="overflow:visible;">
				<?php if ($logo): ?>
				<div id="logo" class="grid12-3 nested">
				<img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
				</div>
				<?php endif; ?>
			</div>
		</div>
	</div>
	
	<div class="row inner clearfix grid12-12" style="margin-top:25px;" >
	<h2 class="title"><?php print $title; ?></h1>
	<div class="hr"></div>
	<?php print $content; ?>
	</div>

 </div>
</div> 
  <?php print $closure; ?>

</body>
</html>