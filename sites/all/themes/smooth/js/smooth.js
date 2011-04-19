Drupal.behaviors.smoothSuperfish = function (context) {
	  $("#primary-menu ul.sf-menu").supersubs({ 
          minWidth:    18,   // minimum width of sub-menus in em units 
          maxWidth:    27,   // maximum width of sub-menus in em units 
          extraWidth:  1     // extra width can ensure lines don't sometimes turn over 
                             // due to slight rounding differences and font-family 
      }).superfish({
	    hoverClass:  'sfHover',
	    delay:       250,
	    animation:   {opacity:'show', height:'show'},
	    speed:       'fast',
	    autoArrows:  true,
	    dropShadows: false,
	    disableHI:   true
	  }).supposition();
	};
	
	
// Wrap span tags around the anchor text in the primary menu.
$(document).ready(function(){
  $("div.breadcrumb a").wrapInner("<span>" + "</span>");
 // $("div.breadcrumb a:first").html(" ").wrapInner("<div class='home'>" + "</div>");
  $("div.breadcrumb span.seperator:last").addClass("last");
  $("div.breadcrumb a:last").addClass("last");
  
  	jQuery(".content, .view-content").preloadImages({
		delay:500,
		imgSelector:'.imgbox img',
		beforeShow:function(){
			jQuery(this).closest('.imgbox').addClass('preload').find('a').addClass('disable-zoom');
		},
		afterShow:function(){
			jQuery(this).closest('.imgbox').find('a').removeClass('disable-zoom');
			var image = jQuery(this).closest('.imgbox').removeClass('preload').children("a");
			enable_image_hover(image);
		}
	});
	$('a.imagefield-fancybox').hover(
		function(){
	jQuery("img", this).animate({"opacity": 0.5});
		}, function() {
			jQuery("img", this).animate({"opacity": 1});
		}
	);
	  
  	// enable image hover effect
	var enable_image_hover = function(image){
		if(image.is(".image_icon_zoom,.image_icon_play,.image_icon_doc")){
			if (jQuery.browser.msie && parseInt(jQuery.browser.version, 10) < 7) {} else {
				if (jQuery.browser.msie && parseInt(jQuery.browser.version, 10) < 9) {
					image.hover(function(){
						jQuery(".image_overlay",this).css("visibility", "visible");
					},function(){
						jQuery(".image_overlay",this).css("visibility", "hidden");
					}).children('img').after('<span class="image_overlay"></span>');
				}else{
					image.hover(function(){
						jQuery(".image_overlay",this).animate({
							opacity: '1'
						},"fast");
					},function(){
						jQuery(".image_overlay",this).animate({
							opacity: '0'
						},"fast");
					}).children('img').after(jQuery('<span class="image_overlay"></span>').css({opacity: '0',visibility:'visible'}));
				}
			}
		}		
	}
  
  
  //$('.quicktabs-style-smoothtab li.active').click().prev().addClass('prevle');
  
  $('ul.quicktabs-style-smoothtab li a').bind('click', function() {
	$('.quicktabs-style-smoothtab li').removeClass("prevtab");
	$('.quicktabs-style-smoothtab li.active').prev().addClass('prevtab');
  });
  
  $('.form-checkboxes input:checkbox').checkbox();
  $('.bef-checkboxes input:checkbox').checkbox({cls:'jquery-safari-checkbox'});
  $('.vote-form input:radio').checkbox({cls:'jquery-safari-checkbox'});
/*  $('#header-top-wrapper').append('<span class="toggle active"><a></a></span>');
  
  $('span.toggle').click(function() {
  $('#header-top').animate({
    opacity: 1,
    height: 'toggle'
  }, 500, function() {
    // Animation complete.
	 $('span.toggle').toggleClass("active");
  });
  }); */
 
  
});
(function($) {

	$.fn.preloadImages = function(options) {
		var settings = $.extend({}, $.fn.preloadImages.defaults, options);


		return this.each(function() {
			settings.beforeShowAll.call(this);
			var imageHolder = $(this);
			
			var images = imageHolder.find(settings.imgSelector).css({opacity:0, visibility:'hidden'});	
			var count = images.length;
			var showImage = function(image,imageHolder){
				if(image.data.source != undefined){
					imageHolder = image.data.holder;
					image = image.data.source;	
				};
				
				count --;
				if(settings.delay <= 0){
					image.css('visibility','visible').animate({opacity:1}, settings.animSpeed, function(){settings.afterShow.call(this)});
				}
				if(count == 0){
					imageHolder.removeData('count');
					if(settings.delay <= 0){
						settings.afterShowAll.call(this);
					}else{
						if(settings.gradualDelay){
							images.each(function(i,e){
								var image = $(this);
								setTimeout(function(){
									image.css('visibility','visible').animate({opacity:1}, settings.animSpeed, function(){settings.afterShow.call(this)});
								},settings.delay*(i+1));
							});
							setTimeout(function(){settings.afterShowAll.call(imageHolder[0])}, settings.delay*images.length+settings.animSpeed);
						}else{
							setTimeout(function(){
								images.each(function(i,e){
									$(this).css('visibility','visible').animate({opacity:1}, settings.animSpeed, function(){settings.afterShow.call(this)});
								});
								setTimeout(function(){settings.afterShowAll.call(imageHolder[0])}, settings.animSpeed);
							}, settings.delay);
						}
					}
				}
			};
			

			images.each(function(i){
				settings.beforeShow.call(this);
				
				image = $(this);
				
				if(this.complete==true){
					showImage(image,imageHolder);
				}else{
					image.bind('error load',{source:image,holder:imageHolder}, showImage);
					if($.browser.opera){
						image.trigger("load");//for hidden image
					}
				}
			});
		});
	};


	//Default settings
	$.fn.preloadImages.defaults = {
		delay:1000,
		gradualDelay:true,
		imgSelector:'img',
		animSpeed:500,
		beforeShowAll: function(){},
		beforeShow: function(){},
		afterShow: function(){},
		afterShowAll: function(){}
	};
})(jQuery);