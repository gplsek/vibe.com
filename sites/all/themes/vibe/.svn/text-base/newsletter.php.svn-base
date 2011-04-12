
	<script type="text/javascript">
		
		document.write('<img src="/sites/all/themes/vibe/images/VIBE_SUBMIT_BG.png" style="position: absolute; top: -9999px;"/>');
		function setupNewsletterForm() {
			var divTag = document.createElement("div"); 
				
			$(divTag).attr("id","close_div").css("width","15px").css("height","15px").css("margin-left","270px").css("margin-top","2px").css("position","absolute").css("cursor","pointer");
		
			$("#show-panel").click(function(){
									
				$("#lightbox-panel").show().css("display","block").css("background","transparent url('sites/all/themes/vibe/images/VIBE_SUBMIT_BG.png') 0% 0% no-repeat");
				var pos = $("#topbar").position();
				$("#lightbox-panel").css("top",pos.top+30 + "px").css("left",pos.left+275 + "px");
				$("input#email").val("");
				$("input#city").val("");
				$("select#state").val("");
				$("input#zip").val("");
				$("select#dob").val("");
				$("#email_error").html("(required)");
			
				
				$("#signup_paragraph").before(divTag);
				
				$("#close_div").click(function(){
					 $("#lightbox-panel").hide();
				});
	
			});
	
			$("#signup").click(function() {   
	   			var email = $("input#email").val();
			 	var city = $("input#city").val();
	   			var state = $("select#state").val();
	   			var zip = $("input#zip").val();
	   			var dob = $("select#dob").val();
	   			var data = "email=" + email + "&city=" + city + "&state=" + state + "&zip=" + zip + "&dob=" + dob;
	   			var validRegExp = /^[^@]+@[^@]+.[a-z]{2,}$/i;
	   			if(email.length == 0 || email == null)
	   			{
		   			$("#email_error").show().html("Please enter an email address");
		   			return false;
	   			}
	   
	   			if (email.search(validRegExp) == -1) 
	   			{
		  			$("#email_error").show().html('A valid e-mail address is required.');
		  			return false;
	   			} 
		 
	   			
	   			$.ajax({
					type: "POST",
					url: "email.php",
					data: data,
					success: function() {
						$("#lightbox-panel").html("<div id=\'newsletter_messsage\' style=\'margin-left: 40px; margin-top:65px;\'><h2>Thank You, you have subscribed to the VIBE newsletter.</h2></div>");
						$("#newsletter_messsage").before(divTag);
						$("#close_div").click(function(){
					 		$("#lightbox-panel").hide();
						});
			 		}
				});
			});
		}
</script> 


