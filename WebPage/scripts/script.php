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
	header("Location: http://localhost/WebPage/test.html");
?>