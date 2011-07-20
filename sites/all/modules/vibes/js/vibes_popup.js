/**
 * Getting the subscription popup passing varnish
 */

$(document).ready(function() {
      $.ajax({
      url: "/vibespopup/js",
      cache: false,
      success: function(){
        var url = "http://local.vibe.com/vibespopup/js/getpopup?KeepThis=true&TB_iframe=true&width=545&height=550";
        tb_show("My Caption", url);
      }
  });
  
});

