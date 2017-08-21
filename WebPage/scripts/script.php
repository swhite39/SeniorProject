<?php

	if(isset($_GET["cups"]) && isset($_GET["strength"]))
	{
		$data1 = $_GET["cups"];
		$data2 = $_GET["strength"];
	}

	exec("./a.out $data1 $data2",$output);

	/*foreach($output as $line)
	{
		echo "<li> $line </li>";
	}*/
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
	
?>