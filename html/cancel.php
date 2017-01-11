<?PHP  session_start();

	include "common.php";

    $myname = "thankyou.php";

	readfile("header.html");

    print "<font size=\"+3\"><span style=\"font-weight: normal;\">Cancelled Transaction</span></font> <p>";

	print "We are sorry to see you change your mind about purchasing Yellowmemo.<p>";

	print  "[<a href=index.html>Back to Home Page</a>]\r\n";

	readfile("footer.html");

?>



