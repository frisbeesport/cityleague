<?php 

/**
 * @file
 * Default theme implementation to format an HTML mail.
 *
 * Copy this file in your default theme folder to create a custom themed mail.
 * Rename it to mimemail-message--[key].tpl.php to override it for a
 * specific mail.
 *
 * Available variables:
 * - $recipient: The recipient of the message
 * - $subject: The message subject
 * - $title: The message title
 * - $body: The message body
 * - $css: Internal style sheets
 * - $key: The message identifier
 *
 * @see template_preprocess_mimemail_message()
 */
 
global $base_url;
 
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">
<html lang="en">

<head>
	<meta content="text/html; charset=3Dutf-8" http-equiv="Content-Type">
	<style>
		@import url(http://fonts.googleapis.com/css?family=Shadows+Into+Light+Two);
		@import url(http://fonts.googleapis.com/css?family=Open+Sans:300italic,400,300);
		
        .script-font {
			font-family: "Shadows Into Light Two", "Open Sans";
			font-size: 1.4em;
			color: #ee1c25;
		}
		
		a {
            color: #ee1c25;
            text-decoration: none;
        }
	</style>
</head>

<body style="padding: 0; width: 100% !important; -webkit-text-size-adjust: 100%; margin: 0; -ms-text-size-adjust: 100%;" marginheight="0" marginwidth="0">
	<center>
		<table cellpadding="8" cellspacing="0" style="*width: 540px; padding: 0; width: 100% !important; background: #ffffff; margin: 0; background-color: #ffffff;" border="0">
			<tr>
				<td valign="top">
					<table cellpadding="0" cellspacing="0" style="border-radius: 5px; -webkit-border-radius: 5px; -moz-border-radius: 5px; border: 1px #cccccc solid;" border="0" align="center">
						<tr>
							<td colspan="3" height="25"></td>
						</tr>
						<tr style="line-height: 0px;">
							<td width="100%" style="font-size: 0px;" align="center" height="1"><img width="150px" style="max-height: 68px; width: 150; *width: 150px; *height: 68px;" alt="" src="<?php print($base_url . '/sites/all/themes/clresponsive/email/emaillogo.jpg'); ?>">
							</td>
						</tr>
						<tr>
							<td>
								<table cellpadding="0" cellspacing="0" style="line-height: 25px;" border="0" align="center">
									<tr>
										<td colspan="3" height="30"></td>
									</tr>
									<tr>
										<td width="36"></td>
										<td width="454" align="left" style="color: #444444; border-collapse: collapse; font-size: 11pt; font-family: 'Open Sans', 'Segoe UI', Verdana, 'Sans Serif'; max-width: 454px;" valign="top">
											<!-- ### BEGIN CONTENT ### -->
											<?php if (isset($title)) { ?>
												<div style="font-weight: bold; font-size: 18px; line-height: 24px; ">
													<?php print $title; ?>	
												</div>
												<br>
											<?php } ?>
											<?php if (isset($body)) print $body; ?>
											<!-- ### END CONTENT ### -->
										</td>
										<td width="36"></td>
									</tr>
									<tr>
										<td colspan="3" height="10"></td>
									</tr>
								</table>
							</td>
						</tr>
					
	
					</table>
					<table cellpadding="0" cellspacing="0" align="center" border="0">
						<tr>
							<td height="10"></td>
						</tr>
						<tr>
							<td style="padding: 0; border-collapse: collapse;">
								<table cellpadding="0" cellspacing="0" align="center" border="0">
									<tr style="color: #ccc; font-size: 11px; font-family: 'Open Sans', 'Segoe UI', Verdana, 'Sans Serif'; -webkit-text-size-adjust: none;">
										<td width="528" align="right">&#169; Nederlandse Frisbee Bond</td>
									</tr>
								</table>
							</td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
	</center>
</body>
</html>