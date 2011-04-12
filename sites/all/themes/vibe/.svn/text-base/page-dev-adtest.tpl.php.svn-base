<?php
// $Id: page.tpl.php,v 1.14.2.6 2009/02/13 16:28:33 johnalbin Exp $

/**
 * @file page.tpl.php
 *
 * Theme implementation to display a single Drupal page.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $css: An array of CSS files for the current page.
 * - $directory: The directory the theme is located in, e.g. themes/garland or
 *   themes/garland/minelli.
 * - $is_front: TRUE if the current page is the front page. Used to toggle the mission statement.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Page metadata:
 * - $language: (object) The language the site is being displayed in.
 *   $language->language contains its textual representation.
 *   $language->dir contains the language direction. It will either be 'ltr' or 'rtl'.
 * - $head_title: A modified version of the page title, for use in the TITLE tag.
 * - $head: Markup for the HEAD section (including meta tags, keyword tags, and
 *   so on).
 * - $styles: Style tags necessary to import all CSS files for the page.
 * - $scripts: Script tags necessary to load the JavaScript files and settings
 *   for the page.
 * - $body_classes: A set of CSS classes for the BODY tag. This contains flags
 *   indicating the current layout (multiple columns, single column), the current
 *   path, whether the user is logged in, and so on.
 * - $body_classes_array: An array of the body classes. This is easier to
 *   manipulate then the string in $body_classes.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 * - $mission: The text of the site mission, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $search_box: HTML to display the search box, empty if search has been disabled.
 * - $primary_links (array): An array containing primary navigation links for the
 *   site, if they have been configured.
 * - $secondary_links (array): An array containing secondary navigation links for
 *   the site, if they have been configured.
 *
 * Page content (in order of occurrance in the default page.tpl.php):
 * - $left: The HTML for the left sidebar.
 *
 * - $breadcrumb: The breadcrumb trail for the current page.
 * - $title: The page title, for use in the actual HTML content.
 * - $help: Dynamic help text, mostly for admin pages.
 * - $messages: HTML for status and error messages. Should be displayed prominently.
 * - $tabs: Tabs linking to any sub-pages beneath the current page (e.g., the view
 *   and edit tabs when displaying a node).
 *
 * - $content: The main content of the current Drupal page.
 *
 * - $right: The HTML for the right sidebar.
 *
 * Footer/closing data:
 * - $feed_icons: A string of all feed icons for the current page.
 * - $footer_message: The footer message as defined in the admin settings.
 * - $footer : The footer region.
 * - $closure: Final closing markup from any modules that have altered the page.
 *   This variable should always be output last, after all other dynamic content.
 *
 * bwetter - added currentUrl to vibe_preprocess
 * @see template_preprocess()
 * @see template_preprocess_page()
 */
 
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
<html xmlns="http://www.w3.org/1999/xhtml" xmlns:fb="http://www.facebook.com/2008/fbml">

<head>
	<meta http-equiv="X-UA-Compatible" content="IE=8" />
  <title><?php print $head_title; ?></title>
  
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <script type="text/javascript">
  	$(document).ready(function() {
		// Fill in the ad slots
  		$(".ad").each(function(){
  			var ad_id = $(this).attr("id");
  			
  		});
  	});
  </script>
  
  <script type="text/javascript" src="/sites/all/themes/vibe/js/easySlider/js/easySlider1.7.js"></script>
  <script type="text/javascript" src="/sites/all/themes/vibe/js/jquery.dropshadow.js"></script>
  
  <script type="text/javascript">
	var gaJsHost = (("https:" == document.location.protocol) ? "https://ssl." : "http://www.");
	document.write(unescape("%3Cscript src='" + gaJsHost + "google-analytics.com/ga.js' type='text/javascript'%3E%3C/script%3E"));
	</script>
	<script type="text/javascript">
		try {
			var pageTracker = _gat._getTracker("UA-10468717-1");
			pageTracker._setDomainName(".vibe.com");
			pageTracker._trackPageview();
		} catch(err) {}
	</script>

   <script type="text/javascript" src="/sites/all/themes/vibe/js/thickbox-compressed.js"></script>
   <link rel="stylesheet" href="/sites/all/themes/vibe/thickbox.css" type="text/css" media="screen" />
	<script type="text/javascript">var _sf_startpt=(new Date()).getTime()</script>

	<!-- JPD Custom Ad Units --> 
<!--	<script type="text/javascript" src="http://ads.jetpackdigital.com/dev/sites/vibe/jpd.js"></script> -->
    
    <script type="text/javascript">

function popitup(url) {
	newwindow=window.open(url,'name','fullscreen=yes,scrollbars=yes,resizable=yes,toolbar=yes');
	if (window.focus) {newwindow.focus()}
	return false;

}
</script>
</head>
<body class="<?php print $body_classes; ?>">

  <div id="page"><div id="page-inner">
    
    <div id="header">
      <div id="topbar" style="position: relative;" <?/*onclick="location.href='/in-the-mag'"*/?>>
	  <?php if($topbar): ?>
	  	<?php print $topbar; ?>
	  <?php endif;?>
	 
	  	<div style="position: absolute; top: 0px; left: 1px; width: 172px; height: 27px;cursor: pointer;" onclick="location.href='http://www.vibelifestylenetwork.com/'"></div>
	  	<div style="position: absolute; top: 6px; left: 273px; width: 392px; height: 26px; cursor: pointer;" onclick="location.href='http://www.vibe.com/posts/cover-trey-just-did-it'"></div>
	  <div style="position: absolute; top: 6px; left: 671px; width: 177px; height: 26px; cursor: pointer;" onclick="location.href='/in-the-mag'"></div>
	  </div>
      
      
    
      <div id="header-inner" class="clear-block">
	  
	  	<div id="header-inner-left">
      	<?php if ($logo): ?>
        	<div id="logo-title">

          	<?php if ($logo): ?>
            	<div id="logo"><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" id="logo-image" /></a></div>
          	<?php endif; ?>
		
        	</div> <!-- /#logo-title -->
      	<?php endif; ?>

      	<?/*php if ($header): ?>
        <div id="header-blocks" class="region region-header">
          <?php print $header; ?>
        </div> <!-- /#header-blocks -->
      	<?php endif; */?>

    	</div>
    	
    	<div id="header-inner-right">
    		<div id="header-inner-right-top">
    			<div id="searchbox">
    				<?php print $search_box; ?>
    			</div>
    			
    			<div id="userlogin">
    				<?php print vibe_user_bar(); ?>
    				<?/*php global $user; ?>
					<?php if ($user->uid) : ?>
						<?php print(t('Logged in as:'))?> <?php print l($user->name,'user/'.$user->uid); ?> |
						<?php print l((t('log out')),"logout"); ?>
					<?php else : ?>
						<form method="post" action="/user"><input type="hidden" name="edit[destination]" value="user" />
							<?php print l((t('Register')),"user/register"); ?> | <?php print(t('Login:'))?>
							<input type="text" maxlength="64" class="form-text" name="edit[name]" id="edit-name" size="15" value="" />	
							<input type="password" class="form-password" maxlength="64" name="edit[pass]" id="edit-pass" size="15" value="" />
							<input type="hidden" name="edit[form_id]" id="edit-form_id" value="user_login" />
							<input type="submit" name="op" class="form-submit">
						</form>
					<?php endif; */?>

    				
    				
    			</div>
    			<div class="clear-block"></div>
    		</div>
    		
    		<div id="header-inner-right-bottom">
    			<div id="main_menu">
             	 	<?php print vibe_menu_links(menu_navigation_links('menu-main-nav',0)); ?>
            	</div>
            	
    		</div>

    	</div>
    	
 		<div class="clear-block"></div> 
 		
 		<div id="menu_bar"></div>
 		  	
      </div> <!-- /#header-inner -->

	  <?//php print $ad_728x90; ?> 
	  <?/*<div id="728x90_1" class="ad728x90 ad">     
      <script type="text/javascript">
      	if(typeof(hide_728x90_1) == "undefined" || hide_728x90_1 != true) {
    		document.write('<scr' + 'ipt src="http://ad.doubleclick.net/adj/vibe.blackrock/homepage;sect=homepage;subs=homepage;subss=;page=6666cd76f96956469e7be39d750cc7d9;tile=1;dcopt=ist;sz=728x90;ord=1259016984?" type="text/javascript" language="javascript"></scr' + 'ipt>');
    	}
      </script>
      </div>*/?>
      <!-- /#header -->
	</div>
	
    <div id="main"><div id="main-inner" class="clear-block<?php if ($search_box || $primary_links || $secondary_links || $navbar) { print ' with-navbar'; } ?>">
	  
	  <div id="main-content">
	  	<div class="content-inner">
	  		<?php print $content; ?>
	  	</div>
	  </div>
		<div><script type="text/javascript" src="http://ads.jetpackdigital.com/dev/lineitems/662/0/jpdli.js?t=_blank&u=http://www&n=4902881"></script></div>
	  
	  <?/*<div id="right-column">
	  	<div id="content-inner">
	  		<?$block = module_invoke('block', 'block', '300ad', 1);?>
	  		<?//php print $block['content'];?>
	  		
	  		
	  		<?php print $right; ?>
	  	</div>
	  </div>*/?>

    </div></div> <!-- /#main-inner, /#main -->

    <?php if ($footer || $footer_message): ?>
      <div id="vln-footer"><?php include('sites/all/themes/vibe/vln-footer.php'); ?></div>
      <div id="footer"><div id="footer-inner" class="region region-footer">
        <?php print $footer; ?>

      </div></div> <!-- /#footer-inner, /#footer -->
    <?php endif; ?>

  </div></div> <!-- /#page-inner, /#page -->

  <?php if ($closure_region): ?>
    <div id="closure-blocks" class="region region-closure"><?php print $closure_region; ?></div>
  <?php endif; ?>

  <?php print $closure; ?>
<!-- Start Quantcast tag -->
<script type="text/javascript">
_qoptions={
qacct:"p-cbO7evNACrYx2"
};
</script>
<script type="text/javascript" src="http://edge.quantserve.com/quant.js"></script>
<noscript>
<img src="http://pixel.quantserve.com/pixel/p-cbO7evNACrYx2.gif" style="display: none;" border="0" height="1" width="1" alt="Quantcast"/>
</noscript>
<!-- End Quantcast tag -->
<!-- Begin comScore Tag -->
<script>
document.write(unescape("%3Cscript src='" + (document.location.protocol == "https:" ? "https://sb" : "http://b")
+ ".scorecardresearch.com/beacon.js' %3E%3C/script%3E"));
</script>
<script>
COMSCORE.beacon({
c1:2,
c2:"6035010",
c3:"",
c4:"<?php print $currentUrl; ?>",
c5:"",
c6:"",
c15:""
});
</script>
<noscript>
<img src="http://b.scorecardresearch.com/p?c1=2&c2=6035010&c3=&c4=<?php print $currentUrl; ?>&c5=&c6=&c15=&cj=1" />
</noscript>
<!-- End comScore Tag -->
<script type="text/javascript">
if (window.location.hostname.indexOf('cache.vibe') == -1 ) {
	var _sf_async_config={uid:4070,domain:"vibe.com"};
	(function(){
	  function loadChartbeat() {
	    window._sf_endpt=(new Date()).getTime();
	    var e = document.createElement('script');
	    e.setAttribute('language', 'javascript');
	    e.setAttribute('type', 'text/javascript');
	    e.setAttribute('src',
	       (("https:" == document.location.protocol) ? "https://s3.amazonaws.com/" : "http://") +
	       "static.chartbeat.com/js/chartbeat.js");
	    document.body.appendChild(e);
	  }
	  var oldonload = window.onload;
	  window.onload = (typeof window.onload != 'function') ?
	     loadChartbeat : function() { oldonload(); loadChartbeat(); };
	})();
}

</script>
<img src="http://ad.doubleclick.net/ad/vibe.blackrock/;kw=adid_224489095;sz=1x1;ord=1234567890?"
width="1" height="1" border="0" alt="">
</body>

</html>
