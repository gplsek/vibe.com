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
if($node->taxonomy){
	if($node->field_excerpt[0]['value'] == NULL || $node->field_excerpt[0]['value'] == "")
	{
		$description = excerpt($node->body, 165);
	}
	else
	{
		$description = $node->field_excerpt[0]['value'];
	}
 
	foreach($node->taxonomy as $ntax)
	{
	 $keywords[] = $ntax->name;
	// var_dump($keywords);
	}
	$keywords_string = implode(',',$keywords);
}
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>" xmlns:fb="http://www.facebook.com/2008/fbml" xmlns:og="http://opengraphprotocol.org/schema/">
<head>
	<meta name="description" content="<?php print $description; ?>" />
    <meta name="keywords" content="<?php print $keywords_string;?>"  />
	<meta http-equiv="X-UA-Compatible" content="IE=8" />
	<link rel="alternate" type="application/rss+xml" title="Vibe RSS Feed" href="http://www.vibe.com/rss" />
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
		
		var divTag = document.createElement("div"); 

    $('.view-celebrity-list').columnize({columns: 4, balanced: true, dontsplit:'li'});

		
  	});
  </script>
  

 <script src="http://www.realgravity.com/javascripts/rg_framework.js" type="text/javascript"></script> 
  <script type="text/javascript" src="/sites/all/themes/vibe/js/easySlider/js/easySlider1.7.js"></script>
  <script type="text/javascript" src="/sites/all/themes/vibe/js/jquery.dropshadow.js"></script>
  <script type="text/javascript" src="/sites/all/themes/vibe/js/jquery.columnize.js"></script>
<!-- <script type="text/javascript" src="/sites/all/themes/vibe/js/jquery.newsTicker-2.2.js"></script>
 <script type="text/javascript" src="/sites/all/themes/vibe/js/newstickercall.js"></script>-->
   <script type="text/javascript" src="/sites/all/themes/vibe/js/thickbox-compressed.js"></script>
   <link rel="stylesheet" href="/sites/all/themes/vibe/thickbox.css" type="text/css" media="screen" />
	<script type="text/javascript">var _sf_startpt=(new Date()).getTime()</script>

	<!-- JPD Custom Ad Units --> 
	<script type="text/javascript" src="http://ads.jetpackdigital.com/sites/vibe/jpd.js"></script>
    
    <script type="text/javascript">

		function popitup(url) {
			newwindow=window.open(url,'name','fullscreen=yes,scrollbars=yes,resizable=yes,toolbar=yes');
			if (window.focus) {newwindow.focus()}
			return false;
		
		}
	</script>
    
</head>
<body class="<?php print $body_classes; ?>">
<!--<script type="text/javascript">
 Meebo=function(){(Meebo._=Meebo._||[]).push(arguments)};
(function(q){
var args = arguments;
if (!document.body) { return setTimeout(function(){ args.callee.apply(this, args) }, 100); }
var d=document, b=d.body, m=b.insertBefore(d.createElement('div'), b.firstChild); s=d.createElement('script');
m.id='meebo'; m.style.display='none'; m.innerHTML='';
s.src='http'+(q.https?'s':'')+'://'+(q.stage?'stage-':'')+'cim.meebo.com/cim/cim.php?network='+q.network;
b.insertBefore(s, b.firstChild);

})({network:"vibecom_te88je"});
Meebo('makeEverythingSharable', {shadow:'none'});
</script>-->

  <div id="page"><div id="page-inner">
    
    <div id="header">
      <div id="topbar" style="position: relative;">
	  <?php if($topbar): ?>
	  	<?php print $topbar; ?>
	  <?php endif;?>
	 	
	 	<!-- <div id="vibe-lifestyle-network"></div> -->
	 	
		<a id="vibe-lifestyle-network" class="image-link" href="http://vibelifestylenetwork.com" target="_blank"><span>VIBE LifeStyle Network</span></a>
		<a id="vibe-blast-top" class="image-link vibe-blast" href="http://vibeblastsignup.com?keepThis=true&TB_iframe=true&height=540&width=720"><span>VIBE Blast</span></a>
	 	
		<!-- <div style="position: absolute; top: 0px; left: 280px; width: 250px; height: 26px; cursor: pointer;" id="show-panel"></div> -->
	  	<!-- <div style="position: absolute; top: 0px; left: 1px; width: 172px; height: 27px;cursor: pointer;" onclick="location.href='http://www.vibelifestylenetwork.com/'"></div> -->       
	  	<!-- <div style="position: absolute; top: 0px; left: 700px; width: 150px; height: 26px; cursor: pointer;" id="subscribe-panel"></div> -->
        <?php /* include("sites/all/themes/vibe/subscribe.php"); */?>
	  
      </div>
      <div class="clear-block"></div>
    
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
    				<a id="vibe-blast-middle" class="image-link vibe-blast" href="http://vibeblastsignup.com?keepThis=true&TB_iframe=true&height=540&width=720"><span>VIBE Blast</span></a>
    				<?php /* print vibe_user_bar(); */ ?>
    			</div>
    			<div class="clear-block"></div>
    		</div>
     <?php 
		$vixen_page = FALSE;
		if($node->taxonomy){
			foreach($node->taxonomy as $key => $value){
				if($value->name == "vixen")
				{
					$vixen_page = TRUE;
				}
			} 
		}?>		
   	<div id="header-inner-right-bottom">
    			<?php if (arg(0) == "vixen" || $vixen_page == TRUE){?>
                <div id="vixen_main_menu">
             	 	<?php print vibe_menu_links(menu_navigation_links('menu-main-nav',0)); ?>
            	</div>
                <?php } else { ?>
                <div id="main_menu">
             	 	<?php print vibe_menu_links(menu_navigation_links('menu-main-nav',0)); ?>
            	</div>
                <?php } ?> 
                
            	
    		</div>

    	</div>
    	<script type="text/javascript">

		  var _gaq = _gaq || [];
		  _gaq.push(['_setAccount', 'UA-10468717-1']);
		  _gaq.push(['_setDomainName', '.vibe.com']);
		  _gaq.push(['_setAllowLinker', true]);
		  if(typeof section != 'undefined'){
			_gaq.push(['_setCustomVar',5,'Section',section]); 
		  }   
		  _gaq.push(['_trackPageview']);
		
		  (function() {
			var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;
			ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
			var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);
		  })();
		
		</script>
 		<div class="clear-block"></div> 
       
     
 		<?php if (arg(0) == "vixen" || $vixen_page == TRUE){?>
        		<div id="vixen_menu_bar"></div>
        <?php } else { ?>
        		<div id="menu_bar"></div>
 		<?php } ?>  	
      

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
	        <?php print $content_bottom ?>
	  	</div>
	  </div>
	  
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
<div id="lightbox-subscribe-panel" >  
    <div id="subscribe_paragraph">2 years (12 issues) for only $14.95 Save 75% off the newstand price!</div>
    <div id="subscribe_wrapper">
        <div id="cover_page"><img src="<?php print url('sites/all/themes/vibe/images/VIBE_NEWSLETTER_MAGS.png');?>" alt="cover_page_image" /></div>
        <div id="subscribe_list">
            <ul>
            <li><a href="http://www.vibe.com/subscribe"><img id="subscribe_image" src="<?php print url('sites/all/themes/vibe/images/VIBE_SUBSCRIBE.png');?>" alt="cover_page_image" /></a></li>
            <li><a href="https://secure.palmcoastd.com/pcd/eSv?iMagId=26601&i4Ky=IGA1"><img id="give_image"src="<?php print url('sites/all/themes/vibe/images/VIBE_GIVE.png');?>" alt="cover_page_image" /></a></li>
            <li><a href="http://www.vibe.com/customerservice"><img id="cust_image" src="<?php print url('sites/all/themes/vibe/images/VIBE_CUSTSERVICE.png');?>" alt="cover_page_image" /></a></li>
            </ul>
        </div>
	</div>
</div>

		<script type="text/javascript">setupSubscribe();</script>
        
 <div id="lightbox-panel" >  
  			<div id="signup_paragraph">Sign Up for the latest news and exclusive offers from VIBE!</div>
			<form name="newsletter_form" action="" id="newsletter_form" >
				<div class="newsletter_overlay">
                    <div id="email_div" class="wrapper">
                       <div class="label_wrapper">
                       	<label for="email" id="email_label" class="newsletter_label">Email Address </label>
                       </div>
                       <div class="input_wrapper">
                        <input type="text" id="email" name="email" value=" " size="20"  />
                       </div>
                       <div style='clear: both'></div>
                        <div id="email_error"></div>
                    </div>
                    <div id="city_div" class="wrapper">
                        <div class="label_wrapper">
                        <label for="city" id="city_label" class="newsletter_label">City </label>
                        </div>
                        <div class="input_wrapper">
                        <input type="text" id="city" name="city" value=" "  size="20"/>
                        </div>
                        <div style='clear: both'></div>
                    </div>
                     <div id="state_div" class="wrapper">
                     	<div class="label_wrapper">
                         <label for="state" id="state_label" class="newsletter_label">State </label>
                        </div>
                        <div class="input_wrapper">
                         <select name="state" id="state" value="" class="select">
                           <option value="">Select State</option>
                           <option value="Alaska">Alaska</option>
                           <option value="Alabama">Alabama</option>
                           <option value="Arkansas">Arkansas</option>
                           <option value="Arizona">Arizona</option>
                           <option value="California">California</option>
                           <option value="Colorado">Colorado</option>
                           <option value="Connecticut">Connecticut</option>
                           <option value="District of Columbia">District of Columbia</option>
                           <option value="Delaware">Delaware</option>
                           <option value="Florida">Florida</option>
                           <option value="Georgia">Georgia</option>
                           <option value="Hawaii">Hawaii</option>
                           <option value="Iowa">Iowa</option>
                           <option value="Idaho">Idaho</option>
                           <option value="Illinois">Illinois</option>
                           <option value="Indiana">Indiana</option>
                           <option value="Kansas">Kansas</option>
                           <option value="Kentucky">Kentucky</option>
                           <option value="Louisiana">Louisiana</option>
                           <option value="Massachusetts">Massachusetts</option>
                           <option value="Maryland">Maryland</option>
                           <option value="Maine">Maine</option>
                           <option value="Michigan">Michigan</option>
                           <option value="Minnesota">Minnesota</option>
                           <option value="Missouri">Missouri</option>
                           <option value="Mississippi">Mississippi</option>
                           <option value="Montana">Montana</option>
                           <option value="North Carolina">North Carolina</option>
                           <option value="North Dakota">North Dakota</option>
                           <option value="Nebraska">Nebraska</option>
                           <option value="New Hampshire">New Hampshire</option>
                           <option value="New Jersey">New Jersey</option>
                           <option value="New Mexico">New Mexico</option>
                           <option value="Nevada">Nevada</option>
                           <option value="New York">New York</option>
                           <option value="Ohio">Ohio</option>
                           <option value="Oklahoma">Oklahoma</option>
                           <option value="Oregon">Oregon</option>
                           <option value="Pennsylvania">Pennsylvania</option>
                           <option value="Puerto Rico">Puerto Rico</option>
                           <option value="Rhode Island">Rhode Island</option>
                           <option value="South Carolina">South Carolina</option>
                           <option value="South Dakota">South Dakota</option>
                           <option value="Tennessee">Tennessee</option>
                           <option value="Texas">Texas</option>
                           <option value="Utah">Utah</option>
                           <option value="Virginia">Virginia</option>
                           <option value="Vermont">Vermont</option>
                           <option value="Washington">Washington</option>
                           <option value="Wisconsin">Wisconsin</option>
                           <option value="West Virginia">West Virginia</option>
                           <option value="Wyoming">Wyoming</option>
                         </select>
                        </div>
                        <div style='clear: both'></div>
                     </div>
                     <div id="zip_div" class="wrapper">
                     	<div class="label_wrapper">
                        	<label for="zip" id="zip_label" class="newsletter_label">Zip Code </label>
                        </div>
                        <div class="input_wrapper">
                        	<input type="text" id="zip" name="zip" value=" " size="20" />
                        </div>
                        <div style='clear: both'></div>
                     </div>
                    <div id="dob_div" class="wrapper">
                        <div class="label_wrapper">
                        	<label for="dob" id="dob_label" class="newsletter_label">Birth Year </label>
                        </div>
                        <div class="input_wrapper">
                            <select id="dob" name="dob" value="" class="select">
                             <option value="">Year</option>
                             <option value="1997">1997</option>
                             <option value="1996">1996</option>
                             <option value="1995">1995</option>
                             <option value="1994">1994</option>
                             <option value="1993">1993</option>
                             <option value="1992">1992</option>
                             <option value="1991">1991</option>
                             <option value="1990">1990</option>
                             <option value="1989">1989</option>
                             <option value="1988">1988</option>
                             <option value="1987">1987</option>
                             <option value="1986">1986</option>
                             <option value="1985">1985</option>
                             <option value="1984">1984</option>
                             <option value="1983">1983</option>
                             <option value="1982">1982</option>
                             <option value="1981">1981</option>
                             <option value="1980">1980</option>
                             <option value="1979">1979</option>
                             <option value="1978">1978</option>
                             <option value="1977">1977</option>
                             <option value="1976">1976</option>
                             <option value="1975">1975</option>
                             <option value="1974">1974</option>
                             <option value="1973">1973</option>
                             <option value="1972">1972</option>
                             <option value="1971">1971</option>
                             <option value="1970">1970</option>
                             <option value="1969">1969</option>
                             <option value="1968">1968</option>
                             <option value="1967">1967</option>
                             <option value="1966">1966</option>
                             <option value="1965">1965</option>
                             <option value="1964">1964</option>
                             <option value="1963">1963</option>
                             <option value="1962">1962</option>
                             <option value="1961">1961</option>
                             <option value="1960">1960</option>
                             <option value="1959">1959</option>
                             <option value="1958">1958</option>
                             <option value="1957">1957</option>
                             <option value="1956">1956</option>
                             <option value="1955">1955</option>
                             <option value="1954">1954</option>
                             <option value="1953">1953</option>
                             <option value="1952">1952</option>
                             <option value="1951">1951</option>
                             <option value="1950">1950</option>
                             <option value="1949">1949</option>
                             <option value="1948">1948</option>
                             <option value="1947">1947</option>
                             <option value="1946">1946</option>
                             <option value="1945">1945</option>
                             <option value="1944">1944</option>
                             <option value="1943">1943</option>
                             <option value="1942">1942</option>
                             <option value="1941">1941</option>
                             <option value="1940">1940</option>
                             <option value="1939">1939</option>
                             <option value="1938">1938</option>
                             <option value="1937">1937</option>
                             <option value="1936">1936</option>
                             <option value="1935">1935</option>
                             <option value="1934">1934</option>
                             <option value="1933">1933</option>
                             <option value="1932">1932</option>
                             <option value="1931">1931</option>
                             <option value="1930">1930</option>
                             <option value="1929">1929</option>
                             <option value="1928">1928</option>
                             <option value="1927">1927</option>
                             <option value="1926">1926</option>
                             <option value="1925">1925</option>
                             <option value="1924">1924</option>
                             <option value="1923">1923</option>
                             <option value="1922">1922</option>
                             <option value="1921">1921</option>
                             <option value="1920">1920</option>
                           </select>
                         </div>
                         <div style='clear: both'></div>
                    </div>                                                         
                    <div id="submit_div" class="wrapper">                                    
                       <input type="button" style="height:28px; width:76px; background:transparent url('sites/all/themes/vibe/images/VIBE_NEWSLETTER_SUBMIT.jpg') no-repeat;" id="signup" />
                    </div>
				</div>             
			</form>
		</div>
<script type="text/javascript">
    //Meebo('domReady');
</script>
	<?php include("sites/all/themes/vibe/newsletter.php");?>
	<script type="text/javascript">setupNewsletterForm();</script>
</body>

</html>
