$(function()
 {
  $("table tr").hover(
   function()
   {
    $(this).addClass("highlight");
   },
   function()
   {
    $(this).removeClass("highlight");
   }
  )
 }
)

$(document).ready(function() {
  $("a[rel=external]").click(function() {
    this.target = "_blank";
  });

  $(".description").parent().each(function(){
    $(this).children('label').each(function(){
      $(this).removeClass("option");
      $(this).addClass("label-help");
    });
  });

});
