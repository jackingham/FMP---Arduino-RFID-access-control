<?php

	$dbusername = "user1";  
    $dbpassword = "inghamcda600"; 
    $server = "localhost"; 

	$dbconnect = mysqli_connect($server, $dbusername, $dbpassword);
    $dbselect = mysqli_select_db($dbconnect, "users");

	$sql="SELECT * FROM usertable";

	$records=mysqli_query($dbconnect,$sql);
	$json_array=array();
	
	while($row=mysqli_fetch_assoc($records))
	{
		$json_array[]=$row;
		
	}
	echo json_encode($json_array);
?>
