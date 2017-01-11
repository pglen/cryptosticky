<?PHP  session_start();

	error_reporting  (E_ERROR | E_WARNING | E_PARSE);

	include "common.php";

    $myname = "scontact.php";

	readfile("header.html");

    print "<font size=\"+3\"><span style=\"font-weight: normal;\">Your Entry Submitted</span></font> <p>";


	//var_dump($_POST); print "<p>";
	//var_dump($_GET); print "<p>";

	$date =  date ("l dS F Y h:i:s A");

	print "Your post '$_POST[subject]' is sent to YellowMemo support level one.<p>\n\n";
	print "Your submission is dated: $date<p>\n\n";

	$body = "";

	$body .= "Message from " . $_POST[from] . "\n\n";

	$body .= "Date: " . $date . "\n";
	$body .= "From: " . $_POST[from] . "\n";
	if($_POST[email] != "")
		{
		$body .= "Mail: " . $_POST[email] . "\n";
		}

	$body .= "Subj: " . $_POST[subject] . "\n\n";

	//$body .= "Message follows:\n";
	$message = preg_replace("/\r\n/", "\n", $_POST[message]);

	$body .= "----- Start of Message----------------------------------------\n";
	$body .= $message .  "\n";
	$body .= "----- End of Message -----------------------------------------\n";

    mail($emailto, "YellowMemo Support Question", $body, "From: AutoMail@YellowMemo.com");

	if($_POST[email] != "")
		{
		print "A copy of your message is emailed to: '$_POST[email]<p>'";
		}

	print  "[<a href=index.html>Back to Home Page</a>]\r\n";
	print  "[<a href=contact.html>Back to Contact Page</a>]\r\n";
	print  "[<a href=download.html>Go to Download Page</a>]\r\n";

	readfile("footer.html");

?>



