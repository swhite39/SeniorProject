<?php

	if(isset($_GET["cups"]) && isset($_GET["strength"]))
	{
		$data1 = $_GET["cups"];
		$data2 = $_GET["strength"];
	}

//	header("Location: http://localhost/WebPage/countdown2.html");
	exec("./client $data1 $data2",$output);

	foreach($output as $line)
	{
		if(strcmp($line,"Good") != 0)
		{	
			header("Location: http://localhost/WebPage/countdown2.html");
		}
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
