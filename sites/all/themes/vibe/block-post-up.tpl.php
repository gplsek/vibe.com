<div id="latest_on_vibe_header" style="margin-bottom: 8px; padding-left: 0px; width: 300px !important; "><div class="latest_on_vibe_header_text" style="color: #c0100b; font-size: 15px;">VIBE <span class="latest_on_vibe_section" style="color: #a9a8b2;">POST UP</span> REAL TIME</div><div id="latest_on_vibe_header_bar"  style="width: 79px; background-color: #c0100b;"></div><div class="clear-block"></div></div>

<div id="post-up">
<?php
$qtid = 2; // write here your quicktabs id.
$quicktabs = quicktabs_load($qtid);
print theme('quicktabs', $quicktabs);
?>
</div>
