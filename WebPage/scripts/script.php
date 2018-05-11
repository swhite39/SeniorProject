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
			if($data1 == 4)
			{
				$destination = "http://" . $_SERVER['SERVER_ADDR'] . "/WebPage/countdown4.html";
				header("Location: $destination");
				//header("Location: http://localhost/WebPage/countdown4.html");
			}
			else if($data1 == 6)
			{
				$destination = "http://" . $_SERVER['SERVER_ADDR'] . "/WebPage/countdown6.html";
				header("Location: $destination");
			}
			else if($data1 == 8)
			{
				$destination = "http://" . $_SERVER['SERVER_ADDR'] . "/WebPage/countdown8.html";
				header("Location: $destination");
			}
			else if($data1 == 10)
			{
				$destination = "http://" . $_SERVER['SERVER_ADDR'] . "/WebPage/countdown10.html";
				header("Location: $destination");
		
			}
			else if($data1 == 12)
			{
				$destination = "http://" . $_SERVER['SERVER_ADDR'] . "/WebPage/countdown12.html";
				header("Location: $destination");
			}
		}
		// This is the case when there is not enough water for a brew
		else if(strcmp($line,"2") == 0)
		{
			$destination = "http://" . $_SERVER['SERVER_ADDR'] . "/WebPage/waterError.html";
			header("Location: $destination");
		}
		// This is the case when there is not enough coffee grounds for a brew
		else if(strcmp($line,"3") == 0)
		{
			$destination = "http://" . $_SERVER['SERVER_ADDR'] . "/WebPage/coffeeError.html";
			header("Location: $destination");
		}
		// This is the case when there is no clean filters for a brew
		else if(strcmp($line,"4") == 0)
		{
			$destination = "http://" . $_SERVER['SERVER_ADDR'] . "/WebPage/filterError.html";
			header("Location: $destination");
		}
		// This case should never happen but just in case
		else
		{
			$destination = "http://" . $_SERVER['SERVER_ADDR'] . "/WebPage/index.html";
			header("Location: $destination");
		}
	}

/*	$dbhost = 'localhost';
	$dbuser = 'webuser';
	$dbpass = 'secretpass';
	$dbname = 'testing';
	$connection = mysqli_connect($dbhost,$dbuser,$dbpass, $dbname);

	mysqli_close($connection);*/
	
?>
