 <html lang="en">
  <head>
    <meta content="text/html; charset=utf-8" http-equiv="Content-Type">
    <title>
      Smooth Drupal 6 Premium Theme
    </title>
	<style type="text/css">
	<?php print $css; ?>
	</style>
  </head>
  <body style="margin: 0; padding: 0; background: #080708 url('<?php print $path; ?>/htmlmail_images/bg_email.png'); background-color:#080708;" >
  	<table cellpadding="0" cellspacing="0" border="0" align="center" width="100%">
		  <tr>
		  	<td align="center" style="background: url('<?php print $path; ?>/htmlmail_images/bg_email.png'); padding: 35px 0">
				<table cellpadding="0" cellspacing="0" border="0" align="center" width="650" style="font-family: Helvetica, Arial, sans-serif; background: #1e1e1e url('<?php print $path; ?>/htmlmail_images/bg_table.png');" bgcolor="#1e1e1e">
			      	<tr>
				        <td align="left" colspan="5" style="height:3px; font-size: 0px; line-height: 0" height="3" valign="top"><img src="<?php print $path; ?>/htmlmail_images/bg_divider_top.png" alt="divider"></td>
				     </tr>
					<tr>
			        <td width="20" style="font-size: 0px;">&nbsp;</td>
					<td width="610" valign="top" align="left" style="font-family: Helvetica, Arial, sans-serif; padding: 0 0 20px;" class="content">
						<table cellpadding="0" cellspacing="0" border="0"  style="color: #B5B7B4; font: normal 11px Helvetica, Arial, sans-serif; margin: 0; padding: 0;" width="610">
						<tr>
							<td style="padding: 25px 0 8px; border-bottom: 1px solid #383838; font-family: Helvetica, Arial, sans-serif; "  valign="top">
								<h2 style="color:#8b7500; font-weight: normal; margin: 0; padding: 0; line-height: 16px; font-size: 16px;"><?php print $header; ?></h2>
							</td>
						</tr>
						<!-- repeater toc='true' -->
						<tr>
							<td style="padding: 10px 0 0 0; margin: 0;" valign="top">
								<table cellpadding="0" cellspacing="0" border="0"  style="color: #B5B7B4; font: normal 11px Helvetica, Arial, sans-serif; margin: 0; padding: 0;" width="610">
								<tr>
									<td style="padding: 10px 0 20px; border-bottom: 1px solid #383838;"  valign="top">
									<?php print $body ?>
									</td>
								</tr> 
								  
								</table>
							</td>
						</tr>
						<!-- repeater -->
						</table>
						<br>
					</td>
					<td width="20" style="font-size: 0px;" style="font-family: Helvetica, Arial, sans-serif;">&nbsp;</td>
			      </tr>
				  	<tr>
				        <td align="left" colspan="5" style="height: 4px; font-size: 0px;" height="4">
						<img src="<?php print $path; ?>/htmlmail_images/bg_divider.png" alt="divider">
				      </td>
				     </tr>
			      
				</table><!-- body -->
				<br><table cellpadding="0" cellspacing="0" border="0" align="center" width="650" style="font-family: Helvetica, Arial, sans-serif; line-height: 10px;" class="footer">
				<tr>
			        <td align="center" style="padding: 5px 0 10px; font-size: 11px; color:#7d7a7a; margin: 0; line-height: 1.2;font-family: Helvetica, Arial, sans-serif;" valign="top">
						<?php print $footer; ?>
					</td>
			      </tr>
				</table><!-- footer-->
		  	</td>
		</tr>
    </table>
  </body>
</html>