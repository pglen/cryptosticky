<?PHP  session_start();

	//print "<html>";
	//print <head>;
	//print <title>PayPal Page</title>;
	//print </head>;

    $myname = "testpal.php";

	readfile("header.html");

    print "<font size=\"+3\"><span style=\"font-weight: normal;\">Test Paypal Page</span></font> <br>";

	print "<left><br>Get:<br>"; var_dump($_GET);
	print "<br>Session:<br>"; var_dump($_SESSION);
	print "<br>Post:<br>"; var_dump($_POST);

	print "<img src=$_POST[image_url]><p>";

	$url = $_POST['return'] . "?";

	foreach ($_POST as $key => $value)
		{
		print $key . " = " . urlencode($value) . "<br>";
		$url .= $key  . "=" . urlencode($value) .  "&";
		}


	print "<p><a href=$url>Continue</a><p>";

	print "<p><a href=$_POST[cancel_return]>Cancel</a><p>";

	//include "common.php";
	//connect_data_service();

	readfile("footer.html");

?>



