/*
 * jQuery.ImageSwitch
 * Version: 1.0
 * http://www.groject.com/ImageSwitch/
 *
 * Copyright (c) 2009 Hieu Pham - http://www.hieu.co.uk
 * COMMON DEVELOPMENT AND DISTRIBUTION LICENSE (CDDL)
 * http://www.opensource.org/licenses/cddl1.php
 *
 * Date: 13/03/2009
 * Revision: 50
 */
(function($){
	$.fn.ImageSwitch = function(Arguements, FuntionHandle) {
		var defaults = {
			Type: "FadeIn", // Type of effect to run the function
			NewImage: "", //The new image will be loaded in
			EffectOriginal: true,
			Speed: 1000, //Speed of the effect
			StartLeft: 50, //The position the effect start compare to the original position could be (-)
			StartTop: 0,
			StartOpacity: 0, //Which start opacity it would be
			EndLeft: -50, //The position the effect end compare to the original position could be (-)
			EndTop: 0,
			EndOpacity: 0, //Which start opacity it would be
			Direction: "RightLeft", //Which Way the image will be sroll
			Door1: "", //The image for the door 1
			Door2: "" //The image for the door 2
		}
		
		var Args = $.extend(defaults, Arguements);
		var Obj = this; // Just a way to reference to this obj in case we need to pass in another event handle
        var EndFunction = function(){};
		if($.isFunction(FuntionHandle)){
		    EndFunction = FuntionHandle;
		}
		//-----------------------------------------------------------------------------------------------------------
		//The original image will be fade out when the new image will fade in
		var FadeImage = function(){
			//Generate the effect map, move the effect map overlay the original map
			$("body").append("<img class='GrpEffectImg'/>");
			$(".GrpEffectImg").attr("src", Obj.attr("src"));
			$(".GrpEffectImg").css("position", "absolute");
			$(".GrpEffectImg").css("top", Obj.offset().top);
			$(".GrpEffectImg").css("left", Obj.offset().left);
			$(".GrpEffectImg").css("opacity", 1);
			
			//Change image of the original map
			Obj.attr("src", Args.NewImage);
			
			//Need something special when user want to keep no effect for the orignal
			if(Args.EffectOriginal)
			{
				//Set the start opacity, as the effect will fade out we set in start at 1, vice versa for the original
				Obj.css("opacity", Args.StartOpacity);		
				
				//Fade in the original image
				Obj.animate({"opacity":1}, Args.Speed);			
			}
			
			//Start effect animation
			$(".GrpEffectImg").animate({"opacity":0}, Args.Speed, function(){
					//Remove the effect image when finish the effect
					$(".GrpEffectImg").remove();
					    EndFunction();
			});			
		}
		//-----------------------------------------------------------------------------------------------------------
		//The new image will fly from the startPosition with the StartOpacity
		
		var Fly = function(FlyIn){
			//Generate the effect map, move the effect map overlay the original map
			$("body").append("<img class='GrpEffectImg'/>");
			$(".GrpEffectImg").css("position", "absolute");
			if(FlyIn){
				//As the new image will fly in, so we set the effect image src = new image
				$(".GrpEffectImg").attr("src", Args.NewImage);			
				$(".GrpEffectImg").css("top", Obj.offset().top + Args.StartTop);
				$(".GrpEffectImg").css("left", Obj.offset().left + Args.StartLeft);
				$(".GrpEffectImg").css("opacity", Args.StartOpacity);
				EndTop = Obj.offset().top;
				EndLeft = Obj.offset().left;
				//Change the opacity base on the input				
				EndOpacity = 1; 			
			}else{
				//As the old image will fly out, so we set the effect image src = new image
				//The effect image will be on top of the old image and hide the old image
				//So we could set the old image with the new src
				$(".GrpEffectImg").attr("src", Obj.attr("src"));						
				Obj.attr("src", Args.NewImage);
				$(".GrpEffectImg").css("top", Obj.offset().top);
				$(".GrpEffectImg").css("left", Obj.offset().left);
				$(".GrpEffectImg").css("opacity", 1);
				EndTop = Obj.offset().top + Args.EndTop;
				EndLeft = Obj.offset().left + Args.EndLeft;
				//Change the opacity base on the input				
				EndOpacity = Args.EndOpacity; 				
			}
			//Let the effect start fly in
			$(".GrpEffectImg").animate({"opacity":EndOpacity, "top":EndTop, 
										"left": EndLeft}, Args.Speed,
				function(){
					Obj.attr("src", Args.NewImage);
					$(".GrpEffectImg").remove();
					EndFunction();
			});
		}
		//-----------------------------------------------------------------------------------------------------------
		//The new image will scoll in and kick the old image out.
		//With the setting ScollIn = false, The original image will scroll out and reveal the new image
		var Scroll = function(ScrollIn){
			//Save the original status so we could set it in the end
			var backup = Obj.clone(true);		
			//Create a viewport for it
			Obj.wrap("<div id='GrpViewport'></div>");
			$("#GrpViewport").css("overflow","hidden");
			$("#GrpViewport").width(Obj.width());
			$("#GrpViewport").height(Obj.height());								
			//Generate the effect map, move the effect map overlay the original map				
			$("#GrpViewport").append("<img class='GrpEffectImg'/>");
			$(".GrpEffectImg").css("position", "absolute");
			//Find where the Effect Image start
			var StartTop = 0;
			var StartLeft = 0;				
			switch(Args.Direction){
				case "RightLeft":	StartLeft = -Obj.width();	break;
				case "LeftRight":	StartLeft = Obj.width();	break;
				case "TopDown":		StartTop = -Obj.height();	break;
				case "DownTop":		StartTop = Obj.height();	break;
			}
			//In scroll in using the Start position, else, Set it to 0 so it could scroll out
			//Also need o set the destination of the animate different
			if(ScrollIn){
				$(".GrpEffectImg").attr("src", Args.NewImage);
				$(".GrpEffectImg").css("top", StartTop);
				$(".GrpEffectImg").css("left", StartLeft);
				$(".GrpEffectImg").css("opacity", Args.StartOpacity);
				EndTop = 0;
				EndLeft = 0;
				//Don't change the opacity if it scroll in
				EndOpacity = 1; 
			}else{
				$(".GrpEffectImg").attr("src", Obj.attr("src"));
				$(".GrpEffectImg").css("left", 0);
				$(".GrpEffectImg").css("top", 0);
				Obj.attr("src", Args.NewImage);
				EndTop = StartTop;
				EndLeft = StartLeft;
				//Change the opacity base on the input				
				EndOpacity = Args.EndOpacity; 
			}
			//We need to treat absolute position different					
			//In some case there're text arround the image, it could be a bit mess up
			if(Obj.css("position")!="absolute")
			{
				$("#GrpViewport").css("position","relative");					
				Obj.css("position","absolute");
			}
			else
			{			
				$("#GrpViewport").css("position","absolute");
				$("#GrpViewport").css("left",Obj.css("left"));
				$("#GrpViewport").css("top",Obj.css("top"));		
				Obj.css("top",0);
				Obj.css("left",0);
			}
			//if effect the original image, then move it as well
			if(Args.EffectOriginal && ScrollIn)
			{			
				//Move the original image along
				Obj.animate({"top": - StartTop,
							"left": - StartLeft}, Args.Speed);									
			}			
			//Start the effect
			$(".GrpEffectImg").animate({"opacity":EndOpacity,"top":EndTop,"left":EndLeft}, Args.Speed, 
					function(){
						//Finish the effect, and replace the viewport with this area
						backup.attr("src",Args.NewImage);
						$("#GrpViewport").replaceWith(backup);
						EndFunction();
				});	
		}
		//-----------------------------------------------------------------------------------------------------------
		//A door come out create an effect door close.then open the new image
		var SingleDoor = function(){
			//Save the original status so we could set it in the end
			var backup = Obj.clone(true);
			//Create a viewport for it
			Obj.wrap("<div id='GrpViewport'></div>");
			$("#GrpViewport").css("overflow","hidden");
			$("#GrpViewport").width(Obj.width());
			$("#GrpViewport").height(Obj.height());								
			//Generate the effect map, move the effect map overlay the original map				
			$("#GrpViewport").append("<div class='GrpEffectDiv'/>");
			$(".GrpEffectDiv").attr("src", Args.NewImage);
			$(".GrpEffectDiv").css("position", "absolute");
			$(".GrpEffectDiv").css("background-color", "#FFF");
            if(Args.Door1.length>0)			
			    $(".GrpEffectDiv").css("background", Args.Door1);
			$(".GrpEffectDiv").width(Obj.width());
			$(".GrpEffectDiv").height(Obj.height());								
			//Find where the Effect Image start
			var StartTop = 0;
			var StartLeft = 0;				
			switch(Args.Direction){
				case "RightLeft":	StartLeft = -Obj.width();	break;
				case "LeftRight":	StartLeft = Obj.width();	break;
				case "TopDown":		StartTop = -Obj.height();	break;
				case "DownTop":		StartTop = Obj.height();	break;
			}				
			$(".GrpEffectDiv").css("top", StartTop);
			$(".GrpEffectDiv").css("left", StartLeft);	
			
			//We need to treat absolute position different	
			if(Obj.css("position")!="absolute")
			{
				$("#GrpViewport").css("position","relative");					
				Obj.css("position","absolute");			
			}
			else
			{
				$("#GrpViewport").css("position","absolute");
				$("#GrpViewport").css("left",Obj.css("left"));
				$("#GrpViewport").css("top",Obj.css("top"));		
				Obj.css("top",0);
				Obj.css("left",0);
			}
			//Start Close the Door
			$(".GrpEffectDiv").animate({"top":0,"left":0}, Args.Speed, function(){
				//Finish the first effect change the image and open the door
				Obj.attr("src", Args.NewImage);
				//Start open the door
				$(".GrpEffectDiv").animate({"top":StartTop,"left":StartLeft}, Args.Speed, function(){
					//Reset style
					backup.attr("src",Args.NewImage);
					$("#GrpViewport").replaceWith(backup);
					EndFunction();
				})
			});	
		}
		//-----------------------------------------------------------------------------------------------------------
		//Same with single door but with this effect, there will be 2 door
		var DoubleDoor = function(){
			//Save the original status so we could set it in the end
			var orgPosition = Obj.css("position");
			var orgLeft = Obj.css("left");
			var orgTop = Obj.css("top");
			//Create a viewport for it
			Obj.wrap("<div id='GrpViewport'></div>");
			$("#GrpViewport").css("overflow","hidden");
			$("#GrpViewport").width(Obj.width());
			$("#GrpViewport").height(Obj.height());								
			//Generate the effect map, move the effect map overlay the original map				
			$("#GrpViewport").append("<div class='GrpEffectDiv'/>");
			$(".GrpEffectDiv").css("position", "absolute");
			$(".GrpEffectDiv").css("background-color", "#FFF");
			if(Args.Door1.length>0)
                $(".GrpEffectDiv").css("background", Args.Door1);			
			$(".GrpEffectDiv").width(Obj.width());
			$(".GrpEffectDiv").height(Obj.height());								
			//We need the second door
			$("#GrpViewport").append("<div class='GrpEffectDiv1'/>");
			$(".GrpEffectDiv1").css("position", "absolute");
			$(".GrpEffectDiv1").css("background-color", "#FFF");
			if(Args.Door2.length>0)
                $(".GrpEffectDiv1").css("background", Args.Door2);				
			$(".GrpEffectDiv1").width(Obj.width());
			$(".GrpEffectDiv1").height(Obj.height());								
			
			//Find where the Effect Image start
			var StartTop = 0;
			var StartLeft = 0;				
			switch(Args.Direction){
				case "RightLeft":	StartLeft = -Obj.width();	break;
				case "LeftRight":	StartLeft = Obj.width();	break;
				case "TopDown":		StartTop = -Obj.height();	break;
				case "DownTop":		StartTop = Obj.height();	break;
			}				
			$(".GrpEffectDiv").css("top", StartTop);
			$(".GrpEffectDiv").css("left", StartLeft);	
			$(".GrpEffectDiv1").css("top", -StartTop);
			$(".GrpEffectDiv1").css("left", -StartLeft);	
			
			//set the background for the door effect so it look different
			if(!Args.EffectOriginal){
				$(".GrpEffectDiv").css("background","#FFF url("+Args.NewImage+") no-repeat "+ -StartLeft/2 +"px "+ -StartTop/2+"px");
				$(".GrpEffectDiv1").css("background","#FFF url("+Args.NewImage+") no-repeat "+ StartLeft/2+"px "+ StartTop/2 +"px");
			}			
			
			//We need to treat absolute position different					
			if(Obj.css("position")!="absolute")
			{
				$("#GrpViewport").css("position","relative");					
				Obj.css("position","absolute");			
			}
			else
			{
				$("#GrpViewport").css("position","absolute");
				$("#GrpViewport").css("left",orgLeft);
				$("#GrpViewport").css("top",orgTop);
				Obj.css("position","absolute");			
				Obj.css("top",0);
				Obj.css("left",0);
			}
			//Start Close the Door
			$(".GrpEffectDiv").animate({"top":StartTop/2,"left":StartLeft/2}, Args.Speed, function(){
				//Finish the first effect change the image and open the door
				Obj.attr("src", Args.NewImage);
				//If EffectOriginal isn't on mean two door stick into the new image, then stop here. Else carry on
				if(!Args.EffectOriginal){
					Obj.css("position", orgPosition);
					Obj.css("top", orgTop);
					Obj.css("left", orgLeft);				
					$("#GrpViewport").replaceWith(Obj);
				}else{
					//Start open the door
					$(".GrpEffectDiv").animate({"top":StartTop,"left":StartLeft}, Args.Speed, function(){
						//Reset style
						Obj.css("position", orgPosition);
						Obj.css("top", orgTop);
						Obj.css("left", orgLeft);
						$("#GrpViewport").replaceWith(Obj);
					});
				}
			});	
			$(".GrpEffectDiv1").animate({"top":-StartTop/2,"left":-StartLeft/2}, Args.Speed, function(){
				//Finish the first effect change the image and open the door
				Obj.attr("src", Args.NewImage);
				//If EffectOriginal isn't on mean two door stick into the new image, then stop here. Else carry on
				if(!Args.EffectOriginal){
					EndFunction();
				}else{
					//Start open the door
					$(".GrpEffectDiv1").animate({"top":-StartTop,"left":-StartLeft}, Args.Speed, function(){
						//Run the end effect
						EndFunction();
					});
				}
			});					
		}
		//-----------------------------------------------------------------------------------------------------------
		//The new image will flip from the back of the old one to the top
		//If FlipIn is false, then the old image will flip to the back reveal the new one
		var Flip = function(FlipIn){
		    var backup = Obj.clone(true);	
			if(Obj.css("z-index") == 'auto')	{
				Obj.css("z-index", 100);
			}
			//if (position different then absolute and relative then it should be relative)
			if(Obj.css("position") != "absolute"){
				Obj.css("position", "relative");
			}
			//Generate the effect map, move the effect map overlay the original map
			$("body").append("<img class='GrpEffectImg'/>");
			$(".GrpEffectImg").css("position", "absolute");
			$(".GrpEffectImg").css("top", Obj.offset().top);
			$(".GrpEffectImg").css("left", Obj.offset().left);
			
			if(FlipIn){
				$(".GrpEffectImg").css("opacity", Args.StartOpacity);
				//So this layer will be under the original image
				$(".GrpEffectImg").css("z-index", Obj.css("z-index")-1);
				$(".GrpEffectImg").attr("src", Args.NewImage);
			}else{
				$(".GrpEffectImg").css("opacity", 1);
				//This layer will be on top the original image
				$(".GrpEffectImg").css("z-index", Obj.css("z-index")+1);			
				//Turn in to the fake old image
				$(".GrpEffectImg").attr("src", Obj.attr("src"));
				Obj.attr("src", Args.NewImage);
				
			}
			
			//Find where the effect layer stop
			if(Math.abs(Args.EndTop)<Obj.height() && Math.abs(Args.EndLeft)<Obj.width()){
				EndTop = Obj.offset().top;
				EndLeft = Obj.offset().left + Obj.width();
			}else{
				EndTop = Obj.offset().top + Args.EndTop;
				EndLeft = Obj.offset().left + Args.EndLeft;				
			}
			EndOpacity = 1; 
			
			//Let the effect start, 
			$(".GrpEffectImg").animate({"opacity":EndOpacity, "top":EndTop, 
										"left": EndLeft}, Args.Speed,
				function(){
					//Now the effect image is out, move it back again
					if(FlipIn) {
						$(".GrpEffectImg").css("z-index", 101);
					}else{
						EndOpacity = Args.EndOpacity;
						$(".GrpEffectImg").css("z-index", 2);						
					}
					$(".GrpEffectImg").animate({"opacity":EndOpacity, "top":Obj.offset().top, 
												"left": Obj.offset().left}, Args.Speed,
						function(){
							//Restore the image to the original
							backup.attr("src", Args.NewImage);
							Obj.replaceWith(backup);
							$(".GrpEffectImg").remove();
							EndFunction();						
						});
			});
		}
		
		return this.each(function(){
		    var TempImg = new Image();
		    TempImg.src = Args.NewImage;
		    $(TempImg).load(function(){
			    switch(Args.Type){
				    case "FadeIn":		FadeImage();	break;
				    case "FlyIn": 		Fly(true);		break;
				    case "FlyOut":		Fly(false);		break;
				    case "FlipIn": 		Flip(true);		break;
				    case "FlipOut":		Flip(false);	break;				
				    case "ScrollIn":	Scroll(true);	break;
				    case "ScrollOut":	Scroll(false);	break;
				    case "SingleDoor":	SingleDoor();	break;
				    case "DoubleDoor":	DoubleDoor();	break;
			    }
			});
		})
	}
})(jQuery);