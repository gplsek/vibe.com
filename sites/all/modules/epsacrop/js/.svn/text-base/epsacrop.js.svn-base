var EPSACrop = {
 api: null,
 preset: null,
 delta: null,
// _presets: {},
 presets: {},
 init: false,
 dialog: function(delta, img, trueSize) {
    $('body').append('<div title="Cropping Image" id="EPSACropDialog"><img src="'+Drupal.settings.epsacrop.base+Drupal.settings.epsacrop.path+'/img/loading.gif" /></div>');
    $('#EPSACropDialog').dialog({
       bgiframe: true,
       height: 500,
       width: 850,
       modal: true,
       draggable: false,
       resizable: false,
       overlay: {
          backgroundColor: '#000',
          opacity: 0.5
       },
       buttons: {
          Save: function() {
            $('#edit-epsacropscalled').val(JSON.stringify(EPSACrop._presets));
            $('#edit-epsacropcoords').val(JSON.stringify(EPSACrop.presets));
            $(this).dialog('close');
            $('#EPSACropDialog').remove();
          },
          Cancel: function() {
             $(this).dialog('close');
             $('#EPSACropDialog').remove();
          }
       },
       close: function() {
          $('#EPSACropDialog').remove();
       }
    }).load(Drupal.settings.epsacrop.base+'crop/dialog', function(){
       //try{
	       var preset = $('.epsacrop-presets-menu a[@class=selected]').attr('rel'); 
	       var coords = preset.split('x');
	       EPSACrop.preset = preset;
	       EPSACrop.delta = delta;
	       if($('#edit-epsacropcoords').val().length > 0 && EPSACrop.init == false) {
	    	   // EPSACrop._presets = eval('(' + $('#edit-epsacropscalled').val() + ')'); //JSON.parse($('#edit-epsacropscalled').val());
           EPSACrop.presets = eval('(' + $('#edit-epsacropcoords').val() + ')'); //JSON.parse($('#edit-epsacropcoords').val()); 
           EPSACrop.init = true;
	       }
	       if((typeof EPSACrop.presets[EPSACrop.delta] == 'object') && (typeof EPSACrop.presets[EPSACrop.delta][EPSACrop.preset] == 'object') ) {
	    	   var c = EPSACrop.presets[EPSACrop.delta][EPSACrop.preset];
	       }
	       $('#epsacrop-target').attr({'src': img, 'width': trueSize[0], 'height': trueSize[1]});
         setTimeout(function(){
           EPSACrop.api = $.Jcrop('#epsacrop-target', {
              aspectRatio: (coords[0] / coords[1]),
              setSelect: (typeof c == 'object') ? [c.x, c.y, c.x2, c.y2] : [0, 0, coords[0], coords[1]],
              minSize: 0,//[coords[0], coords[1]], // bwetter added - to prevent resizing crop
              maxSize: 0,//[coords[0], coords[1]], // bwetter added - to prevent resizing crop
              boxWidth: 512,
              boxHeight: 384,
              trueSize: trueSize,
              onSelect: EPSACrop.update
           }); // $.Jcrop
          }, 100); // Sleep < d'une second
       //}catch(err) {
    	 //  alert(err.description);
       //}
    }); // fin load
 }, // dialog
 crop: function( preset ) {
    $('.epsacrop-presets-menu a').each(function(i){ $(this).removeClass('selected') });
    $('.epsacrop-presets-menu a[@rel='+preset+']').addClass('selected');
    var coords = preset.split('x');
    EPSACrop.preset = preset;
    if(typeof EPSACrop.presets[EPSACrop.delta] == 'object' && typeof EPSACrop.presets[EPSACrop.delta][EPSACrop.preset] == 'object' ) {
       var c = EPSACrop.presets[EPSACrop.delta][preset];
       EPSACrop.api.animateTo([c.x, c.y, c.x2, c.y2]);
    }else{
       EPSACrop.api.animateTo([0, 0, coords[0], coords[1]]);
    }
    EPSACrop.api.setOptions({aspectRatio: coords[0]/coords[1]});
 },
 update: function( c ) {
    var preset 	= EPSACrop.preset;
    var delta 	= EPSACrop.delta;
    // Informations de crop
    if(typeof EPSACrop.presets[delta] != 'object') {
    	EPSACrop.presets[delta] = {};
    }
    EPSACrop.presets[delta][preset] = c;
    
    // Informations de scalle
    // if(typeof EPSACrop._presets[delta] != 'object') {
    // 	EPSACrop._presets[delta] = {};
    // }
    // EPSACrop._presets[delta][preset] = EPSACrop.api.tellScaled();
 }
};
