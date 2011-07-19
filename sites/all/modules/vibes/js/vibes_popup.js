/**
 * Getting the subscription popup passing varnish
 */

$(document).ready(function() {
      $.ajax({
      url: "/vibespopup/js",
      cache: false,
      success: function(html){
        $("#main-content").prepend(html);
        $("#vibe-overlay-close").click(function(){
          $("#vibe-overlay-wrapper").hide();
        });
      }
  });
  
});

