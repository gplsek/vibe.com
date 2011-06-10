 $('document').ready( function () {
		billboardPos = 1;
		$("#billboard_right_arrow").click(function(){
			billboardScrollToPos(billboardPos+1);
			return false;
		});
    
		$("#billboard_left_arrow").click(function(){
			billboardScrollToPos(billboardPos-1);
			return false;
		});
		
		$(".billboard_step").click(function() {
			billboardScrollToPos(parseInt(this.id.replace("billboard_step_","")));
			return false;
		});
		
		billboardTimer = setTimeout("billboardAutoscroll()", 10000);
		
  });
    
   function billboardAutoscroll() {
    	if (billboardPos == 5) {
    		billboardScrollToPos(1);
    	} else {
    		billboardScrollToPos(parseInt(billboardPos+1));
    	}
    }
    
    function billboardScrollToPos(stepNum) {
    	if (stepNum > 5) stepNum = 1;
    	else if(stepNum < 1) stepNum = 5;
    	
    	clearTimeout(billboardTimer);
    	billboardPos = stepNum;
    	
    	if (stepNum == 1) {
    		pos = 0;
    	} else {
    		pos = ((stepNum-1)*534) * -1;
    	}
    	
    	$("#billboard").animate({"left": pos+"px"}, "slow");
    	$('.billboard_step a').css('background-color', '#ffffff');
		$("#billboard_step_"+stepNum+" a").css('background-color', '#FFCC00');
		
		billboardTimer = setTimeout("billboardAutoscroll()", 10000);
    }
    
    function billboardScroll(dir) {
    	clearTimeout(billboardTimer);
    	if (dir == "left") {
			if (billboardPos > 1 && $("#billboard").position().left < 0) {
				billboardPos--;
				$("#billboard").animate({"left": "+=534px"}, "slow");
			}
    	} else if (dir == "right") {
			if (billboardPos < 5 && $("#billboard").position().left > -1550) {
				billboardPos++;
				$("#billboard").animate({"left": "-=534px"}, "slow");
			}
    	}
		
		$('.billboard_step a').css('background-color', '#ffffff');
		$("#billboard_step_"+billboardPos+" a").css('background-color', '#FFCC00');

		billboardTimer = setTimeout("billboardAutoscroll()", 5000);
    }
	
	