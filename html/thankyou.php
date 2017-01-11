<?PHP  session_start();

	include "common.php";

	error_reporting  (E_ERROR | E_WARNING | E_PARSE);

    $myname = "thankyou.php";

	readfile("header.html");

    print "<font size=\"+3\"><span style=\"font-weight: normal;\">Thank you for your purchase</span></font> <br>";

	//print "<br>Post:<br>"; var_dump($_POST); print "<br>";

	print  "<p>Your request will be processed within two working days. (usually sooner)<p>";

	print  "For your review, we received the following details:<p>";

	print "<table width=75% align=center>" .
	 		"<tr><td width=50% align=right>&nbsp Name on the YellowMemo Licence ......... : <td align=left>'$_POST[invoice]'<br>" .
	 		"<tr><td width=50% align=right>&nbsp Notes from you to Yellowmemo ........... : <td align=left>'$_POST[item_number]'<br>" .
     		"<tr><td width=50% align=right>&nbsp Email address to send the License to ... : <td align=left>'$_POST[custom]'<p>" .
				"</table><p>";

	print  "An email was also sent to '$_POST[custom]' to confirm this transaction.<p>";
	//print  "You may receive an additional email from Paypal as well.<p>";

	print  "[<a href=index.html>Back to Home Page</a>]\r\n";


	// Mail details to
	$date =  date ("l dS F Y h:i:s A");

	// --------------------------------------------------------------------
	$body =
	"Dear " . $_POST['invoice'] . ",\n\n" .

	"This is a confirmation email that you have successfully completed Paypal payment.\n" .
	"Your request will be processed within two working days. (usually sooner)\n\n" .

	"This is an automated message, Please do not reply. \n\n" .

	"For your review, we received the following details:\n\n" .

	"  Name on the YellowMemo Licence ......... : '$_POST[invoice]'\n" .
	"  Notes from you to Yellowmemo ........... : '$_POST[item_number]'\n" .
    "  Email address to send the License to ... : '$_POST[custom]'\n\n" .

	"This message was generated on " . $date . "\n" .
	"If this message contains errors or omissions please email sales@yellowmemo.com\n\n" .

	"Thank You, Your YellowMemo Team.";
	// --------------------------------------------------------------------


    if($_POST[custom] != "")
    	{
    	mail($_POST[custom], "YellowMemo Paypal Payment", $body, "From: AutoMail@YellowMemo.com");
    	}

	// Add the contents of the transaction to local mail:
	$body .= "Transaction details:\n\n";

	foreach ($_POST as $key => $value)
		{
		$body .= $key . " = '" . $value . "'\n";
		}

    mail($emailto, "YellowMemo Paypal Payment", $body, "From: AutoMail@YellowMemo.com");

	readfile("footer.html");
?>



