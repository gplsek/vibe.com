Drupal.behaviors.gallery = function(context) {
	jQuery('#gallery-carousel').jcarousel({
		initCallback: gallery_jcarousel_init
	});	

	$('a.gallery-thumbnail').click(function(event){
		Drupal.settings.gallery.position = parseInt(gallery_get_position($(this).attr('name')));
		gallery_load_image($(this).attr('name'));
		event.preventDefault();
	});
	
	$('span#gallery-control-next a').click(function(event) {
		gallery_next();
		event.preventDefault();
	});	
	
	$('span#gallery-control-prev a').click(function(event) {
		gallery_prev();
		event.preventDefault();
	});
}

function gallery_jcarousel_init() {	
	
	$('#gallery-content').hide('fast', function() {		
		$('#gallery-content').load('/gallery/ajax/image/'+Drupal.settings.gallery.images[0], function() {
			$('#gallery-loader').fadeOut('fast', function() {
				$('#gallery-content').fadeIn('slow');
			});			
		});			
	});
	
}

function gallery_next() {
	Drupal.settings.gallery.position = ((Drupal.settings.gallery.position + 1) < Drupal.settings.gallery.images.length) ? Drupal.settings.gallery.position + 1 : 0;	
	gallery_load_image(Drupal.settings.gallery.images[Drupal.settings.gallery.position]);
		
}

function gallery_prev() {
	Drupal.settings.gallery.position = ((Drupal.settings.gallery.position - 1) >= 0) ? Drupal.settings.gallery.position - 1 : Drupal.settings.gallery.images.length - 1;	
	gallery_load_image(Drupal.settings.gallery.images[Drupal.settings.gallery.position]);
}

function gallery_get_position(nid) {
	for(var key in Drupal.settings.gallery.images) {
		if(Drupal.settings.gallery.images[key] == nid) {
			return key;
		}
	}
}

function gallery_load_image(nid) {	
	
	$('#gallery-content').fadeOut('slow', function() {		
		$('#gallery-loader').fadeIn('fast');
		$(this).load('/gallery/ajax/image/'+nid, function() {
			$('#gallery-loader').fadeOut('fast', function() {
				$('#gallery-content').fadeIn('slow');
				//update ads
				gallery_update_ads();
			});			
			
		});
	});	
}

function gallery_update_ads() {
   var neword = Math.round(Math.random() * 10000000000);
   $('iframe').each(function(i) {
        // Added additional check to prevent interference
        // with non ad iframes like Meebo Toolbar
        if($(this).attr('src')) {
       var url = $(this).attr('src').split(';');
       var newurl = '';
       
       for(var i=0;i<url.length;i++) {
           var k;
           if(i != 0) {
               k = url[i].split('=');
               if(k[0] == 'ord' ) {
                   k[1] = neword;
               }
               url[i] = ';' + k[0] + '=' + k[1];
           } else {
               if(url[i].indexOf('ad.doubleclick.net') < 0) {
                   return;
               }
           }
           newurl += url[i];
           
       }
       $(this).attr('src', newurl);
       //console.log(newurl);
        }
   });
}









