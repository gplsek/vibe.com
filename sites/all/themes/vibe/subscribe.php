<script type="text/javascript">
		function setupSubscribe() {
			var divTag = document.createElement("div"); 
				
			$(divTag).attr("id","close_panel").css("width","15px").css("height","15px").css("margin-left","252px").css("margin-top","4px").css("position","absolute").css("cursor","pointer");
		
			$("#subscribe-panel").click(function(){	
				var pos = $("#topbar").position();
				$("#lightbox-subscribe-panel").show().css("display","block").css("top",pos.top+26 + "px").css("left",pos.left+570 + "px").css("background","transparent url('sites/all/themes/vibe/images/VIBE_SUBSCRIBE_OL_BG.png') 0% 0% no-repeat");
				
				
				$("#subscribe_paragraph").before(divTag);
				$("#close_panel").click(function(){
					$("#lightbox-subscribe-panel").hide();
				});
			 });
			
			$("#subscribe_image").hover(function(){
					$(this).attr("src","sites/all/themes/vibe/images/VIBE_SUBSCRIBE_RO.png")},function(){
					$(this).attr("src","sites/all/themes/vibe/images/VIBE_SUBSCRIBE.png");	
												 
												 });
			$("#give_image").hover(function(){
					$(this).attr("src","sites/all/themes/vibe/images/VIBE_GIVE_RO.png")},function(){
					$(this).attr("src","sites/all/themes/vibe/images/VIBE_GIVE.png");	
												 
												 });
				
			$("#cust_image").hover(function(){
					$(this).attr("src","sites/all/themes/vibe/images/VIBE_CUSTSERVICE_RO.png")},function(){
					$(this).attr("src","sites/all/themes/vibe/images/VIBE_CUSTSERVICE.png");	
												 
												 });
				
				

				
		}
</script> 
