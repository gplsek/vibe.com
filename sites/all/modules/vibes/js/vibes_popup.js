/**
 * Getting the subscription popup passing varnish
 */

$(document).ready(function() {
      $.ajax({
      url: "/vibespopup/js",
      cache: false,
      success: function(html){
        $("#main-content").prepend(html);
        $('.closeWin').click(function(){
          $('#vibe-overlay').hide();
        });
      }
  });
  
});

