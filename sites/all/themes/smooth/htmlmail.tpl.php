<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
   <style type="text/css" media="screen">
      a { color:#386690 }
      h1, h2, h3, h4, h5, h6 { color:#f4793a; }
   <?php print $css; ?>
   </style>
</head>

<body bgcolor="#686662">
<table width="600" border="1" align="center" cellpadding="0" cellspacing="0" bordercolor="#2a2723">
  <tr>
    <td bgcolor="#2a2723"><img src="<?php print $path; ?>/htmlmail_images/header.png" height="76" width="600"></td>
  </tr>
  <tr bgcolor="#2a2723" >
    <td style="padding: 10px 20px 10px 20px; font-family:Arial, Helvetica, sans-serif;color: #ffffff;font-size: 14px;" ><?php print $header; ?></td>
  </tr>
  <tr bgcolor="#dfe2e3">
    <td style="padding: 10px 20px 10px 20px;font-family:Arial, Helvetica, sans-serif;font-size: 12px;color: #2a2723;"><?php print $body; ?></td>
  </tr>
  <tr bgcolor="#2a2723">
    <td style="padding: 10px 20px 10px 20px;font-family:Arial, Helvetica, sans-serif;color: #ffffff;font-size: 10px;" ><?php print $footer; ?></td>
  </tr>
</table>
</body>
</html>