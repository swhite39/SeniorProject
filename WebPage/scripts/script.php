<?php

	if(isset($_GET["cups"]) && isset($_GET["strength"]))
	{
		$data1 = $_GET["cups"];
		$data2 = $_GET["strength"];
	}

	exec("./client $data1 $data2",$output);

	foreach($output as $line)
	{
		// If brew is possible go to countdown screen based on how many cups
		if(strcmp($line,"1") == 0)
		{	
			if($data1 == 2)
			{
				header("Location: http://localhost/WebPage/countdown2.html");
			}

			else if($data1 == 4)
			{
				header("Location: http://localhost/WebPage/countdown4.html");
			}
			else if($data1 == 6)
			{
				header("Location: http://localhost/WebPage/countdown6.html");
			}
			else if($data1 == 8)
			{
				header("Location: http://localhost/WebPage/countdown8.html");
			}
			else if($data1 == 10)
			{
				header("Location: http://localhost/WebPage/countdown10.html");
			}
			else if($data1 == 12)
			{
				header("Location: http://localhost/WebPage/countdown12.html");
			}
		}
		// This is the case when there is not enough water for a brew
		else if(strcmp($line,"2") == 0)
		{
			header("Location: http://localhost/index.html");
		}
		// This is the case when there is not enough coffee grounds for a brew
		else if(strcmp($line,"3") == 0)
		{
			header("Location: http://localhost/WebPage/index.html");
		}
		// This is the case when there is no clean filters for a brew
		else if(strcmp($line,"4") == 0)
		{
			header("Location: http://localhost/WebPage/index.html");
		}
		// This case should never happen but just in case
		else
			header("Location: http://localhost/WebPage/index.html");
	}

/*	$dbhost = 'localhost';
	$dbuser = 'webuser';
	$dbpass = 'secretpass';
	$dbname = 'testing';
	$connection = mysqli_connect($dbhost,$dbuser,$dbpass, $dbname);

	mysqli_close($connection);*/
	
?>
