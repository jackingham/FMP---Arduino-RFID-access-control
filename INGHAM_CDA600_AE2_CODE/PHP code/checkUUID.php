<?php

	$dbusername = "user1";  
    $dbpassword = "inghamcda600"; 
    $server = "localhost"; 
	$uuid = htmlentities($_GET["uuid"]);

	$dbconnect = mysqli_connect($server, $dbusername, $dbpassword);
    $dbselect = mysqli_select_db($dbconnect, "users");

	$stmt=mysqli_prepare($dbconnect,"SELECT * FROM usertable where card_UUID=?");
	
	$stmt->bind_param('s',$uuid);
	$stmt->execute();
	$row=$stmt->fetch();
	if ($row == false){
	echo "[Access denied]}";
	}else{
	echo "[Access granted]}";
	}
	?>
